/**
 * Copyright (C) 2008-2026 isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 */
/*
 ***********************************************************************************************************************
 **
 **  @file               : ComM_Channel.c
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM Channel Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "ComM_Channel.h"
#include "ComM_User.h"

#include "IStdLib.h"
#include "ComM_Internal.h"
#include "SchM_ComM.h"

#if (COMM_NM_ENABLED == STD_ON) || (COMM_PNC_SUPPORT == STD_ON)
#include "Nm.h"
#endif

#include "BswM_ComM.h"
#if COMM_DCM_ENABLED == STD_ON
#include "Dcm_ComM.h"
#endif

#if COMM_PNC_SUPPORT == STD_ON
#include "ComM_Pnc.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
typedef struct
{
    ComM_ModeType CurrentComMode;
    boolean       WakeUpIndication;
#if COMM_PNC_SUPPORT == STD_ON
    boolean PncNmRequest;
    boolean WakeupSleepRequest;
#endif
    boolean ActiveDiagnostic;
} ComM_ChannelGlobalVarType;

/* ============================================ internal data definitions =========================================== */
#define COMM_START_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"
COMM_LOCAL const ComM_ChannelConfigType* ComM_ChannelConfigs;
#define COMM_STOP_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"
COMM_LOCAL ComM_ChannelGlobalVarType ComM_ChannelGlobalVars[COMM_CHANNEL_NUMBER];
#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/**
 * @brief The function returns the current ComMNoCom inhibition status of the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @retval      TRUE    ComMNoCom is enabled
 * @retval      FALSE   ComMNoCom is disabled or ComMNoCom inhibition is inactivated by ComMEcuGroupClassification
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64896
 */
COMM_LOCAL_INLINE boolean ComM_ChannelIsNoCom(NetworkHandleType Channel)
{
    return ((ComM_InhibitionData.EcuGroupClassification & ComM_InhibitionData.InhibitionStatus[Channel])
            & COMM_LIMITED_TO_NO_COM_BflMask)
           == COMM_LIMITED_TO_NO_COM_YES;
}
#endif
/**
 * @brief The function returns the current ComMNoWakeUp inhibition status of the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @retval      TRUE    ComMNoWakeUp is enabled
 * @retval      FALSE   ComMNoWakeUp is disabled or ComMNoWakeUp inhibition is inactivated by ComMEcuGroupClassification
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64897
 */
#if COMM_WAKEUP_INHIBITION_ENABLED == STD_ON
COMM_LOCAL_INLINE boolean ComM_ChannelIsNoWakeUp(NetworkHandleType Channel)
{
    return ((ComM_InhibitionData.EcuGroupClassification & ComM_InhibitionData.InhibitionStatus[Channel])
            & COMM_WAKEUP_INHIBITION_ACTIVE_BflMask)
           == COMM_WAKEUP_INHIBITION_ACTIVE_YES;
}
#endif

/**
 * @brief On entry of channel state COMM_NO_COM_REQUEST_PENDING on the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64901
 */
COMM_LOCAL void ComM_ChBeh_EntryNoComRequestPending(NetworkHandleType Channel);

/**
 * @brief On entry of channel state COMM_NO_COM_NO_PENDING_REQUEST on the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64904
 */
COMM_LOCAL void ComM_ChBeh_EntryNoCom(NetworkHandleType Channel);

/**
 * @brief On entry of channel state COMM_FULL_COM_READY_SLEEP on the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64907
 */
COMM_LOCAL void ComM_ChBeh_EntryComReadySleep(NetworkHandleType Channel);

/**
 * @brief On entry of channel state COMM_FULL_COM_NETWORK_REQUESTED on the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @param[in]   ActiveRequested Channel is requested by a user (inhibition is disable) dcm or pnc, or not.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64913
 */
COMM_LOCAL void ComM_ChBeh_EntryFullComReq(NetworkHandleType Channel, boolean ActiveRequested);

/**
 * @brief On entry of channel state COMM_SILENT_COM on the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64916
 */
COMM_LOCAL void ComM_ChBeh_EntrySlientMode(NetworkHandleType Channel);

#if COMM_PNC_SUPPORT == STD_ON
/**
 * @brief Behavior in channel state COMM_FULL_COM_NETWORK_REQUESTED on the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64918
 */
COMM_LOCAL void ComM_ChannelDoFullComReq(NetworkHandleType Channel);
#endif

#if COMM_CURRENT_CHANNEL_REQUEST_NOTIFICATION_ENABLED == STD_ON
/**
 * @brief A SW-C that wants to get informed about, which users are currently requesting COMM_FULL_COMMUNICATION requires
 * the sender-receiver interface ComM_CurrentChannelRequest.
 * @param[in]   Channel see NetworkHandleType.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64922
 */
COMM_LOCAL void ComM_ChannelFullComRequestNotification(NetworkHandleType Channel);
#endif

/**
 * @brief Function to query whether there are active requests of the corresponding channel.
 * @param[in]   channel see NetworkHandleType.
 * @retval      TRUE  Channel is requested by a user (inhibition is disable) dcm or pnc.
 * @retval      FALSE There are no active requests or user request is inhibited.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64924
 */
COMM_LOCAL boolean ComM_ChannelActiveRequested(NetworkHandleType channel);

/**
 * @brief Channel state machine try transit to COMM_FULL_COM_NETWORK_REQUESTED on the corresponding channel.
 * @param[in]   Channel         see NetworkHandleType.
 * @param[in]   ActiveRequested Channel is requested by a user (inhibition is disable) dcm or pnc, or not.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64925
 */
COMM_LOCAL void ComM_ChannelStateTransitionToFullCom(NetworkHandleType Channel, boolean ActiveRequested);

/**
 * @brief Channel state machine try transit to COMM_NO_COM_NO_PENDING_REQUEST on the corresponding channel.
 * @param[in]   Channel see NetworkHandleType.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64926
 */
COMM_LOCAL void ComM_ChannelStateTransitionToNoCom(NetworkHandleType Channel);

#if COMM_NM_ENABLED == STD_ON
/**
 * @brief Atomically replaces the value pointed to by Object with the value of Desired.
 * @param[in]   Object Object
 * @param[in]   Desired Value to be stored
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64927
 */
COMM_LOCAL void ComM_ChannelStoreNmModeEvent(ComM_NmModeEventType* Object, ComM_NmModeEventType Desired);

/**
 * @brief Atomically compares the value pointed to by Object for equality with that in Expected, and if true, replaces
 * the value pointed to by Object with COMM_NM_MODE_EVENT_NONE.
 * @param[in]   Object Object
 * @param[in]   Expected Expected
 * @return      TRUE if *Object is equal to Expected, else FALSE
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-64928
 */
COMM_LOCAL boolean ComM_ChannelCompareAndClearNmModeEvent(ComM_NmModeEventType* Object, ComM_NmModeEventType Expected);

/**
 * @brief Handle channel state machine transition on the corresponding channel via NmModeEvent.
 * @param[in]   Channel see NetworkHandleType.
 * @param[in]   ActiveRequested Channel is requested by a user (inhibition is disable) dcm or pnc, or not.
 * @reentrant   TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous TRUE
 * @trace       CPD-64931
 */
COMM_LOCAL void ComM_ChannelNmModeEventHandle(NetworkHandleType Channel, boolean ActiveRequested);
#endif

#if COMM_USER_NUMBER > 0u
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS ComM_ChannelUserRequested
#endif

/**
 * @brief Handle channel state machine transition on the corresponding channel via NmModeEvent.
 * @param[in]   Channel see NetworkHandleType.
 * @return      TRUE if COMM_FULL_COMMUNICATION is requested by a user mapped to the corresponding channel else FALSE.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-65026
 */
COMM_LOCAL boolean ComM_ChannelUserRequested(NetworkHandleType Channel);
#endif

#if COMM_PNC_SUPPORT == STD_ON
COMM_LOCAL void ComM_ChannelUpdateIRA(NetworkHandleType Channel);
#endif

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/* PRQA S 1532, 2889 ++ */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/* ========================================== external function definitions ========================================= */
void ComM_ChannelGlobalInit(const ComM_ChannelConfigType* ChannelConfigs)
{
    for (NetworkHandleType channel = 0u; channel < COMM_CHANNEL_NUMBER; ++channel)
    {
        ComM_ChannelGlobalVarType* channelGlobalVarPtr = &ComM_ChannelGlobalVars[channel];

        channelGlobalVarPtr->CurrentComMode   = COMM_NO_COMMUNICATION;
        channelGlobalVarPtr->WakeUpIndication = FALSE;

#if COMM_PNC_SUPPORT == STD_ON
        channelGlobalVarPtr->PncNmRequest = FALSE;
#endif
        channelGlobalVarPtr->ActiveDiagnostic = FALSE;
    }

    ComM_ChannelConfigs = ChannelConfigs;
}

void ComM_ChannelLocalInit(void)
{
#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType application = GetApplicationID();
#endif

    for (NetworkHandleType channel = 0u; channel < COMM_CHANNEL_NUMBER; ++channel)
    {
#if (COMM_MULTIPLE_PARTITION_ENABLED == STD_ON) || (COMM_CURRENT_CHANNEL_REQUEST_NOTIFICATION_ENABLED == STD_ON) \
    || (COMM_PNC_SUPPORT == STD_ON)
        const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[channel];
#endif

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
        if (channelConfigPtr->Application == application)
#endif
        {
            ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[channel];

            channelLocalVarPtr->State = COMM_NO_COM_NO_PENDING_REQUEST;
#if COMM_NM_ENABLED == STD_ON
            channelLocalVarPtr->NmModeEvent = COMM_NM_MODE_EVENT_NONE;
#endif
            channelLocalVarPtr->Allowed = FALSE;

            channelLocalVarPtr->NmLightTimeout = 0u;

#if COMM_CURRENT_CHANNEL_REQUEST_NOTIFICATION_ENABLED == STD_ON
            channelLocalVarPtr->NumberOfUserRequesters = 0u;
            channelLocalVarPtr->UserRequesters         = channelConfigPtr->UserRequesters;
            if (channelLocalVarPtr->UserRequesters != NULL_PTR)
            {
                for (uint8 i = 0u; i < channelConfigPtr->NumberOfUsers; ++i)
                {
                    channelLocalVarPtr->UserRequesters[i] = COMM_NOT_USED_USER_ID;
                }
            }
#endif

#if COMM_PNC_SUPPORT == STD_ON
            channelLocalVarPtr->PncBitVectorIRA = channelConfigPtr->PncBitVectorIRA;
            (void)IStdLib_MemSet(channelLocalVarPtr->PncBitVectorIRA, 0, channelConfigPtr->PncBitVectorLength);
#endif
        }
    }
}

#ifdef COMM_TEST
ComM_StateType ComM_ChannelGetState(NetworkHandleType Channel)
{
    const ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    return channelLocalVarPtr->State;
}
#endif
/* PRQA S 6060 ++ */ /* VL_MTR_ComM_STM19 */
ComM_ModeType ComM_ChannelGetMaxComMode(NetworkHandleType Channel)
{
#if COMM_MODE_LIMITATION_ENABLED == STD_ON
    if (ComM_ChannelIsNoCom(Channel))
    {
        return COMM_NO_COMMUNICATION;
    }
#endif

#if COMM_WAKEUP_INHIBITION_ENABLED == STD_ON
    if (ComM_ChannelIsNoWakeUp(Channel) && (ComM_ChannelGetCurrentComMode(Channel) != COMM_FULL_COMMUNICATION))
    {
        return COMM_NO_COMMUNICATION;
    }
#endif

    return COMM_FULL_COMMUNICATION;
}
/* PRQA S 6060 -- */ /* VL_MTR_ComM_STM19 */
/**
 * The implementation of ComM_ChannelGetCurrentComMode.
 */
ComM_ModeType ComM_ChannelGetCurrentComMode(NetworkHandleType Channel)
{
    const ComM_ChannelGlobalVarType* channelGlobalVarPtr = &ComM_ChannelGlobalVars[Channel];
    return channelGlobalVarPtr->CurrentComMode;
}

void ComM_ChannelCommunicationAllowed(NetworkHandleType Channel, boolean Allowed)
{
    ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    channelLocalVarPtr->Allowed                  = Allowed;
}

/**
 * The implementation of ComM_ChannelBusSMModeIndication.
 * ComM shall propagate the indicated state to the users with means of the RTE and BswM.
 */
void ComM_ChannelBusSMModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode)
{
    ComM_ChannelGlobalVarType* channelGlobalVarPtr = &ComM_ChannelGlobalVars[Channel];

    if (ComMode != channelGlobalVarPtr->CurrentComMode)
    {
#if COMM_CURRENT_MODE_NOTIFICATION_ENABLED == STD_ON
        ComM_ModeType                 previousUserCurrentMode[COMM_USER_NUMBER] = {0};
        const ComM_ChannelConfigType* channelConfigPtr                          = &ComM_ChannelConfigs[Channel];
        SchM_Enter_ComM_UM();
        for (uint8 i = 0u; i < channelConfigPtr->NumberOfUsers; ++i)
        {
            previousUserCurrentMode[i] = ComM_UserGetCurrentMode(channelConfigPtr->Users[i]);
        }

        channelGlobalVarPtr->CurrentComMode = ComMode;

        for (uint8 i = 0u; i < channelConfigPtr->NumberOfUsers; ++i)
        {
            ComM_ModeType nextUserCurrentMode = ComM_UserGetCurrentMode(channelConfigPtr->Users[i]);
            if (previousUserCurrentMode[i] != nextUserCurrentMode)
            {
                ComM_UserSwitchCurrentMode(channelConfigPtr->Users[i], nextUserCurrentMode);
            }
        }
        SchM_Exit_ComM_UM();
#else
        channelGlobalVarPtr->CurrentComMode = ComMode;
#endif

        BswM_ComM_CurrentMode(Channel, ComMode);

        switch (ComMode)
        {
#if COMM_DCM_ENABLED == STD_ON
        case COMM_FULL_COMMUNICATION:
            Dcm_ComM_FullComModeEntered(Channel);
            break;

        case COMM_SILENT_COMMUNICATION:
            Dcm_ComM_SilentComModeEntered(Channel);
            break;
#endif
        case COMM_NO_COMMUNICATION:
#if COMM_DCM_ENABLED == STD_ON
            Dcm_ComM_NoComModeEntered(Channel);
#endif
#if COMM_RESET_AFTER_FORCING_NO_COMM == STD_ON && (COMM_USER_NUMBER > 0u)
            if (ComM_ChannelIsNoCom(Channel) && ComM_ChannelUserRequested(Channel))
            {
                BswM_ComM_InitiateReset();
            }
#endif
            break;

        default:
            /* nothing to do */
            break;
        }
    }
}

#if COMM_NM_ENABLED == STD_ON
void ComM_ChannelNmModeIndication(NetworkHandleType Channel, ComM_NmModeEventType NmModeEvent)
{
    const ComM_ChannelConfigType* channelConfigPtr   = &ComM_ChannelConfigs[Channel];
    ComM_ChannelLocalVarType*     channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];

    if ((channelConfigPtr->NmVariant == COMM_NM_VARIANT_FULL)
        || (channelConfigPtr->NmVariant == COMM_NM_VARIANT_PASSIVE))
    {
        ComM_ChannelStoreNmModeEvent(&channelLocalVarPtr->NmModeEvent, NmModeEvent);
    }
}
#endif

/**
 * The implementation of ComM_ChannelWakeUpIndication.
 */
void ComM_ChannelWakeUpIndication(NetworkHandleType Channel, boolean SynchronousWakeUp)
{
#if COMM_SYNCHRONOUS_WAKE_UP == STD_ON
    if (SynchronousWakeUp)
    {
        for (NetworkHandleType channel = 0u; channel < COMM_CHANNEL_NUMBER; ++channel)
        {
            ComM_ChannelGlobalVars[channel].WakeUpIndication = TRUE;
#if COMM_PNC_SUPPORT == STD_ON
            if (ComM_PncEnabled)
            {
                for (PNCHandleType pnc = 0u; pnc < COMM_PNC_NUMBER; ++pnc)
                {
                    if (ComM_PncIsChannelAssigned(pnc, channel))
                    {
                        ComM_PncWakeUpIndication(pnc);
                    }
                }
            }
#endif
        }
    }
    else
#else
    COMM_UNUSED_PARAM(SynchronousWakeUp);
#endif
    {
        ComM_ChannelGlobalVars[Channel].WakeUpIndication = TRUE;

        /* ComMNmVariant of a managed channel is set to LIGHT */
        NetworkHandleType managingChannel = ComM_ChannelConfigs[Channel].ManagingChannel;
        if (managingChannel != COMM_NOT_USED_CHANNEL_ID)
        {
            ComM_ChannelGlobalVars[Channel].WakeUpIndication = TRUE;
        }
    }
}

void ComM_ChannelMainFunction(NetworkHandleType Channel)
{
    ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    if (channelLocalVarPtr->NmLightTimeout > 0u)
    {
        channelLocalVarPtr->NmLightTimeout--;
    }

#if COMM_PNC_SUPPORT == STD_ON
    if (ComM_PncEnabled)
    {
        for (PNCHandleType pnc = 0u; pnc < COMM_PNC_NUMBER; ++pnc)
        {
            if (ComM_PncIsChannelAssigned(pnc, Channel))
            {
                ComM_PncMainFunction(pnc, Channel);
            }
        }
        ComM_ChannelUpdateIRA(Channel);
    }
#endif

#if COMM_CURRENT_CHANNEL_REQUEST_NOTIFICATION_ENABLED == STD_ON
    ComM_ChannelFullComRequestNotification(Channel);
#endif

    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    boolean                       activeRequested  = ComM_ChannelActiveRequested(Channel);

    /* event priority: NmModeEvent > activeRequested and WakeUpIndicati */
#if COMM_NM_ENABLED == STD_ON
    if ((channelConfigPtr->NmVariant == COMM_NM_VARIANT_FULL)
        || (channelConfigPtr->NmVariant == COMM_NM_VARIANT_PASSIVE))
    {
        ComM_ChannelNmModeEventHandle(Channel, activeRequested);
    }
#endif

    if (activeRequested)
    {
        ComM_ChannelStateTransitionToFullCom(Channel, TRUE);
    }
    else
    {
        /** Keep the WakeUpIndication until the COMM_FULL_COM_NETWORK_REQUESTED state is reached */
        if (ComM_ChannelGlobalVars[Channel].WakeUpIndication)
        {
            ComM_ChannelStateTransitionToFullCom(Channel, FALSE);
        }
        /* If there is no activeRequested, make sure that the channel with NmVaraint=FULL transitions to
         * COMM_FULL_COM_READY_SLEEP. Because Nm_NetworkRequest (due to activerequests) is only called when
         * COMM_FULL_COM_NETWORK_REQUESTED is entered from another state.
         */
        ComM_ChannelStateTransitionToNoCom(Channel);
    }
}

#if COMM_PNC_SUPPORT == STD_ON
void ComM_ChannelSetPncNmRequest(NetworkHandleType Channel)
{
    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    if (channelConfigPtr->PncNmRequest)
    {
        ComM_ChannelGlobalVars[Channel].PncNmRequest = TRUE;
    }
}

boolean ComM_ChannelSetWakeupSleepRequest(NetworkHandleType Channel)
{
    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    if (channelConfigPtr->WakeupSleepRequestEnabled)
    {
        ComM_ChannelGlobalVars[Channel].WakeupSleepRequest = TRUE;
        return TRUE;
    }
    return FALSE;
}
#endif

/**
 * The implementation of ComM_ChannelActiveDiagnostic.
 */
void ComM_ChannelActiveDiagnostic(NetworkHandleType channel, boolean active)
{
    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[channel];
    if ((channelConfigPtr->NmVariant == COMM_NM_VARIANT_FULL) || (channelConfigPtr->NmVariant == COMM_NM_VARIANT_LIGHT)
        || (channelConfigPtr->NmVariant == COMM_NM_VARIANT_NONE))
    {
        ComM_ChannelGlobalVarType* channelGlobalVarPtr = &ComM_ChannelGlobalVars[channel];
        channelGlobalVarPtr->ActiveDiagnostic          = active;
    }
}

#if COMM_PNC_SUPPORT == STD_ON
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
void ComM_ChannelUpdateERA(NetworkHandleType Channel, const uint8* PncBitVectorPtr)
{
    if (ComM_PncEnabled)
    {
        for (PNCHandleType pnc = 0u; pnc < COMM_PNC_NUMBER; ++pnc)
        {
            if (ComM_PncIsChannelAssigned(pnc, Channel))
            {
                ComM_PncUpdateChannelBitVectorERA(pnc, Channel, PncBitVectorPtr);
            }
        }
    }
}
#endif
#endif

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
ApplicationType ComM_ChannelGetApplication(NetworkHandleType Channel)
{
    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    return channelConfigPtr->Application;
}
#endif
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
COMM_LOCAL void ComM_ChBeh_EntryNoComRequestPending(NetworkHandleType Channel)
{
    ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    channelLocalVarPtr->State                    = COMM_NO_COM_REQUEST_PENDING;
}

COMM_LOCAL void ComM_ChBeh_EntryNoCom(NetworkHandleType Channel)
{
    ComM_ChannelLocalVarType* chVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    chVarPtr->State                    = COMM_NO_COM_NO_PENDING_REQUEST;

    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    /* SWS_ComM_00073
     * on entering state comm no commuication the ComM channel state machine shall switch off the transmit and reception
     */
    (void)channelConfigPtr->BusSMRequestComMode(Channel, COMM_NO_COMMUNICATION);

#if COMM_NM_ENABLED == STD_ON
    /* SWS_ComM_00288 entry state COMM_NO_COMMUNICATION and ComMNmVariant=FULL comm moudle shall call Nm_NetworkRelease
     */
    if (channelConfigPtr->NmVariant == COMM_NM_VARIANT_FULL)
    {
        (void)Nm_NetworkRelease(Channel);
    }
#endif
}

COMM_LOCAL void ComM_ChBeh_EntryComReadySleep(NetworkHandleType Channel)
{
    ComM_ChannelLocalVarType*     channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    const ComM_ChannelConfigType* channelConfigPtr   = &ComM_ChannelConfigs[Channel];
    channelLocalVarPtr->State                        = COMM_FULL_COM_READY_SLEEP;
#if COMM_NM_ENABLED == STD_ON
    /* SWS_ComM_00133 ComMNmVariant=FULL shall release network */
    if (channelConfigPtr->NmVariant == COMM_NM_VARIANT_FULL)
    {
        (void)Nm_NetworkRelease(Channel);
    }
#endif /* COMM_NM_ENABLED == STD_ON */
    /* SWS_ComM_00891 ComMNmVariant=LIGHT time shall be start ComMNmLightTimeout */
    if (channelConfigPtr->NmVariant == COMM_NM_VARIANT_LIGHT)
    {
        channelLocalVarPtr->NmLightTimeout = channelConfigPtr->NmLightTimeout;
    }
}

COMM_LOCAL void ComM_ChBeh_EntryFullComReq(NetworkHandleType Channel, boolean ActiveRequested)
{
    ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    channelLocalVarPtr->State                    = COMM_FULL_COM_NETWORK_REQUESTED;

    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    /* SWS_ComM_00892 ComMNmVariant=LIGHT In sub-state COMM_FULL_COM_READY_SLEEP
       switch COMM_FULL_COM_NETWORK_REQUESTED the timer for ComMNmLightTimeout shall be canceled */
    if (channelConfigPtr->NmVariant == COMM_NM_VARIANT_LIGHT)
    {
        channelLocalVarPtr->NmLightTimeout = 0u;
    }

    ComM_ModeType comMode = COMM_FULL_COMMUNICATION;
#if COMM_PNC_SUPPORT == STD_ON
    if (ComM_ChannelGlobalVars[Channel].WakeupSleepRequest)
    {
        ComM_ChannelGlobalVars[Channel].WakeupSleepRequest = FALSE;
        comMode                                            = COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST;
    }
#endif
    (void)channelConfigPtr->BusSMRequestComMode(Channel, comMode);

    /* SWS_ComM_00899
     * entering state COMM_FULL_COMMUNICATION comm channel state shall switch COMM_FULL_COM_NETWORK_REQUESTED,when
     * switch from COMM_SILENT_COMMUNICATION ,comm channel state can switch COMM_FULL_COM_READY_SLEEP
     * Rationale SWS_ComM_00296
     */
#if COMM_NM_ENABLED == STD_ON
    /* SWS_ComM_00869 entry for anthoher state or substate ComMNmVariant=FULL */
    if ((channelConfigPtr->NmVariant == COMM_NM_VARIANT_FULL)
        || (channelConfigPtr->NmVariant == COMM_NM_VARIANT_PASSIVE))
    {
        if (ActiveRequested)
        {
#if COMM_PNC_SUPPORT == STD_ON
            ComM_ChannelGlobalVars[Channel].PncNmRequest = FALSE;
#endif
            (void)Nm_NetworkRequest(Channel);
        }
        else
        {
            (void)Nm_PassiveStartUp(Channel);
        }
    }
#endif
    /** The WakeUpIndication flag must be cleared after Nm_PassiveStartUp is called, to prevent repeated reporting of
     * the wake-up event before BusNm has actually processed the passive wake-up. */
    ComM_ChannelGlobalVars[Channel].WakeUpIndication = FALSE;
}

/**
 * Dont need to call Nm_NetworkRelease, either ComM_Nm_NetworkMode or ComM_Nm_BusSleepMode will be called later anyway.
 * - Nm_NetworkRelease is not required after ComM_Nm_NetworkMode.
 * - Nm_NetworkRelease is called after ComM_Nm_BusSleepMode.
 */
COMM_LOCAL void ComM_ChBeh_EntrySlientMode(NetworkHandleType Channel)
{
    ComM_ChannelLocalVarType*     chVarPtr         = ComM_ChannelLocalVarPtrs[Channel];
    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    chVarPtr->State                                = COMM_SILENT_COM;
    /* SWS_ComM_00071
     * on entering state silent the ComM channel state machine shall switch off the transmit keep reception capability
     * on
     */
    (void)channelConfigPtr->BusSMRequestComMode(Channel, COMM_SILENT_COMMUNICATION);
}
#if COMM_PNC_SUPPORT == STD_ON
COMM_LOCAL void ComM_ChannelDoFullComReq(NetworkHandleType Channel)
{
    const ComM_ChannelConfigType* channelConfigPtr    = &ComM_ChannelConfigs[Channel];
    ComM_ChannelGlobalVarType*    channelGlobalVarPtr = &ComM_ChannelGlobalVars[Channel];

    if (channelGlobalVarPtr->PncNmRequest)
    {
        channelGlobalVarPtr->PncNmRequest = FALSE;
        (void)Nm_NetworkRequest(Channel);
    }
    if (channelGlobalVarPtr->WakeupSleepRequest)
    {
        channelGlobalVarPtr->WakeupSleepRequest = FALSE;
        (void)channelConfigPtr->BusSMRequestComMode(Channel, COMM_FULL_COMMUNICATION_WITH_WAKEUP_REQUEST);
    }
}
#endif

#if COMM_CURRENT_CHANNEL_REQUEST_NOTIFICATION_ENABLED == STD_ON
COMM_LOCAL void ComM_ChannelFullComRequestNotification(NetworkHandleType Channel)
{
    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    if (channelConfigPtr->CurrentChannelRequest != NULL_PTR)
    {
        uint8               numberOfUserRequesters                          = 0u;
        ComM_UserHandleType userRequesters[channelConfigPtr->NumberOfUsers] = {0};

        for (uint8 i = 0u; i < channelConfigPtr->NumberOfUsers; ++i)
        {
            if (ComM_UserGetRequestedComMode(channelConfigPtr->Users[i]) == COMM_FULL_COMMUNICATION)
            {
                userRequesters[numberOfUserRequesters] = channelConfigPtr->Users[i];
                ++numberOfUserRequesters;
            }
        }

        ComM_ChannelLocalVarType* chVarPtr = ComM_ChannelLocalVarPtrs[Channel];
        if ((numberOfUserRequesters != chVarPtr->NumberOfUserRequesters)
            || IStdLib_MemCmp(chVarPtr->UserRequesters, userRequesters, numberOfUserRequesters) != 0u)
        {
            channelConfigPtr->CurrentChannelRequest(numberOfUserRequesters, userRequesters);
            (void)IStdLib_MemCpy(chVarPtr->UserRequesters, userRequesters, numberOfUserRequesters);
        }
    }
}
#endif
/* PRQA S 6060 ++ */ /* VL_MTR_ComM_STM19 */
COMM_LOCAL boolean ComM_ChannelActiveRequested(NetworkHandleType channel)
{
    const ComM_ChannelGlobalVarType* channelGlobalVarPtr = &ComM_ChannelGlobalVars[channel];

    /* Only channels with ComMNmVariant=FULL|LIGHT|NONE will accept the indication of ComM_DCM_ActiveDiagnostic() */
    if (channelGlobalVarPtr->ActiveDiagnostic)
    {
        return TRUE;
    }

    if (ComM_ChannelGetMaxComMode(channel) == COMM_NO_COMMUNICATION)
    {
        return FALSE;
    }

    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[channel];

    /* User requests will be ignored on channels with ComMNmVariant=PASSIVE|SLAVE_PASSIVE */
    if (channelConfigPtr->NmVariant == COMM_NM_VARIANT_PASSIVE)
    {
        return FALSE;
    }

#if COMM_USER_NUMBER > 0u
    if (ComM_ChannelUserRequested(channel))
    {
        return TRUE;
    }
#endif

#if COMM_PNC_SUPPORT == STD_ON
    if (ComM_PncEnabled)
    {
        for (PNCHandleType pnc = 0u; pnc < COMM_PNC_NUMBER; ++pnc)
        {
            if (ComM_PncIsChannelAssigned(pnc, channel))
            {
                if (ComM_PncGetMode(pnc) == COMM_PNC_REQUESTED)
                {
                    return TRUE;
                }
            }
        }
    }
#endif

    return FALSE;
}
/* PRQA S 6060 -- */ /* VL_MTR_ComM_STM19 */
COMM_LOCAL void ComM_ChannelStateTransitionToFullCom(NetworkHandleType Channel, boolean ActiveRequested)
{
    const ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    ComM_StateType                  previousState;
    do
    {
        previousState = channelLocalVarPtr->State;
        switch (channelLocalVarPtr->State)
        {
        case COMM_NO_COM_NO_PENDING_REQUEST:
            ComM_ChBeh_EntryNoComRequestPending(Channel);
            break;
        case COMM_NO_COM_REQUEST_PENDING:
            if (channelLocalVarPtr->Allowed)
            {
                ComM_ChBeh_EntryFullComReq(Channel, ActiveRequested);
            }
            break;
        case COMM_FULL_COM_NETWORK_REQUESTED:
#if COMM_PNC_SUPPORT == STD_ON
            ComM_ChannelDoFullComReq(Channel);
#endif
            break;
        /* SWS_ComM_00877 SWS_ComM_00882
         * user reuqest full com and limit state shall be switch ful com state
         * SWS_ComM_00878 SWS_ComM_00883
         * dcm indicate ComM_DCM_ActiveDiagnostic ,and ComMNmVariant=FULL|LIGHT|NONE, comm state shall be switch
         * COMM_FULL_COMMUNICATION
         * SWS_ComM_00218
         * open ComMNoWakeup in COMM_SILENT_COMMUNICATION state shall limit switch COMM_FULL_COMMUNICATION
         */
        case COMM_FULL_COM_READY_SLEEP:
            if (ActiveRequested)
            {
                ComM_ChBeh_EntryFullComReq(Channel, ActiveRequested);
            }
            break;
        case COMM_SILENT_COM:
            ComM_ChBeh_EntryFullComReq(Channel, ActiveRequested);
            break;
        default:
            /* do nothing */
            break;
        }
    } while (channelLocalVarPtr->State != previousState);
}

COMM_LOCAL void ComM_ChannelStateTransitionToNoCom(NetworkHandleType Channel)
{
    const ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];
    const ComM_ChannelConfigType*   channelConfigPtr   = &ComM_ChannelConfigs[Channel];
    ComM_StateType                  oldState;

    do
    {
        oldState = channelLocalVarPtr->State;
        switch (oldState)
        {
        case COMM_FULL_COM_NETWORK_REQUESTED:
            /* SWS_ComM_00888
             * no user request and dcm diag is inactive ComMNmVariant=FULL in state
             * COMM_FULL_COM_NETWORK_REQUESTED,shall switch COMM_FULL_COM_READY_SLEEP
             */
            ComM_ChBeh_EntryComReadySleep(Channel);
            break;
        case COMM_FULL_COM_READY_SLEEP:
            /* SWS_ComM_00610
             * ComMNmVariant=LIGHT,ComMNmLightTimeout has expired ComM channel state machine shall switch to state
             * COMM_NO_COMMUNICATION, NmLightTimeout is always 0 if NmVariant != LIGHT
             */
            if ((channelConfigPtr->NmVariant == COMM_NM_VARIANT_LIGHT) && (channelLocalVarPtr->NmLightTimeout == 0u))
            {
                ComM_ChBeh_EntryNoCom(Channel);
            }
            break;
        case COMM_NO_COM_REQUEST_PENDING:
            /* SWS_ComM_00896 SWS_ComM_00897
             * no valid pending request COMM_FULL_COMMUNICATION, comm shall switch back to default
             * COMM_NO_COM_NO_PENDING_REQUEST
             */
            if (!ComM_ChannelGlobalVars[Channel].WakeUpIndication)
            {
                ComM_ChBeh_EntryNoCom(Channel);
            }
            break;
        default:
            /* do nothong */
            break;
        }
    } while (channelLocalVarPtr->State != oldState);
}

#if COMM_NM_ENABLED == STD_ON
COMM_LOCAL void ComM_ChannelStoreNmModeEvent(ComM_NmModeEventType* Object, ComM_NmModeEventType Desired)
{
    SchM_Enter_ComM_Channel();
    *Object = Desired;
    SchM_Exit_ComM_Channel();
}

COMM_LOCAL boolean ComM_ChannelCompareAndClearNmModeEvent(ComM_NmModeEventType* Object, ComM_NmModeEventType Expected)
{
    boolean equal = FALSE;
    SchM_Enter_ComM_Channel();
    if (*Object == Expected)
    {
        equal   = TRUE;
        *Object = COMM_NM_MODE_EVENT_NONE;
    }
    SchM_Exit_ComM_Channel();
    return equal;
}

COMM_LOCAL void ComM_ChannelNmModeEventHandle(NetworkHandleType Channel, boolean ActiveRequested)
{
    ComM_ChannelLocalVarType* channelLocalVarPtr = ComM_ChannelLocalVarPtrs[Channel];

    if (ComM_ChannelCompareAndClearNmModeEvent(&channelLocalVarPtr->NmModeEvent, COMM_NM_MODE_EVENT_PREPARE_BUS_SLEEP))
    {
        /* if channel state keep in COMM_FULL_COM_NETWORK_REQUESTED, active request has requested via
         * Nm_NetworkRequest() */
        if (channelLocalVarPtr->State == COMM_FULL_COM_READY_SLEEP)
        {
            ComM_ChBeh_EntrySlientMode(Channel);
        }
    }

    if (ComM_ChannelCompareAndClearNmModeEvent(&channelLocalVarPtr->NmModeEvent, COMM_NM_MODE_EVENT_NETWORK))
    {
        if (channelLocalVarPtr->State == COMM_SILENT_COM)
        {
            ComM_ChBeh_EntryFullComReq(Channel, ActiveRequested);
        }
    }

    if (ComM_ChannelCompareAndClearNmModeEvent(&channelLocalVarPtr->NmModeEvent, COMM_NM_MODE_EVENT_BUS_SLEEP))
    {
        if ((channelLocalVarPtr->State != COMM_NO_COM_NO_PENDING_REQUEST)
            && (channelLocalVarPtr->State != COMM_NO_COM_REQUEST_PENDING))
        {
            ComM_ChBeh_EntryNoCom(Channel);
        }
    }
}
#endif

#if COMM_USER_NUMBER > 0u
/* PRQA S 6060 ++ */ /* VL_MTR_ComM_STM19 */
COMM_LOCAL boolean ComM_ChannelUserRequested(NetworkHandleType Channel)
{
    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    for (uint8 i = 0u; i < channelConfigPtr->NumberOfUsers; ++i)
    {
        if (ComM_UserGetRequestedComMode(channelConfigPtr->Users[i]) == COMM_FULL_COMMUNICATION)
        {
            return TRUE;
        }
    }
    return FALSE;
}
#endif
/* PRQA S 6060 -- */ /* VL_MTR_ComM_STM19 */

#if COMM_MULTIPLE_PARTITION_ENABLED == STD_ON
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
ApplicationType ComM_GetChannelApplicationID(NetworkHandleType Channel)
/* PRQA S 1503 -- */ /* VL_QAC_NoUsedApi */
{
    const ComM_ChannelConfigType* channelConfigPtr = &ComM_ChannelConfigs[Channel];
    return channelConfigPtr->Application;
}
#endif

#if COMM_PNC_SUPPORT == STD_ON
/**
 * @brief Calculate the IRA for the corresponding channel, and update it to NM if the IRA changes.
 * @param[in]    Channel  See NetworkHandleType
 * @reentrant    TRUE (Reentrant for different channels. Non reentrant for the same channel.)
 * @synchronous  TRUE
 * @trace        CPD-61614
 */
COMM_LOCAL void ComM_ChannelUpdateIRA(NetworkHandleType Channel)
{
    const ComM_ChannelConfigType* configPtr                                       = &ComM_ChannelConfigs[Channel];
    uint8                         pncBitVectorIRA[COMM_MAX_PNC_BIT_VECTOR_LENGTH] = {0};

    for (PNCHandleType pnc = 0u; pnc < COMM_PNC_NUMBER; ++pnc)
    {
        if (ComM_PncIsChannelAssigned(pnc, Channel))
        {
            uint16 pncBitIndex = ComM_PncGetBitIndex(pnc);
            if (ComM_PncUserOrActiveChannelRequested(pnc))
            {
                ComM_SetBit(pncBitVectorIRA, pncBitIndex);
            }
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
            else
            {
                if ((configPtr->PncGatewayType == COMM_GATEWAY_TYPE_ACTIVE) && ComM_PncPassiveChannelRequested(pnc))
                {
                    ComM_SetBit(pncBitVectorIRA, pncBitIndex);
                }
            }
#endif

            const ComM_ChannelLocalVarType* localVarPtr = ComM_ChannelLocalVarPtrs[Channel];

            if (IStdLib_MemCmp(localVarPtr->PncBitVectorIRA, pncBitVectorIRA, configPtr->PncBitVectorLength) != 0)
            {
                (void)IStdLib_MemCpy(localVarPtr->PncBitVectorIRA, pncBitVectorIRA, configPtr->PncBitVectorLength);
                Nm_UpdateIRA(Channel, pncBitVectorIRA);
            }
        }
    }
}
#endif
/* PRQA S 1532, 2889 -- */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn */
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
