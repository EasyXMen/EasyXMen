/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
 **  @file               : ComM_Pnc.c
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM Pnc Implementation source
 **
 **********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
/* =================================================== inclusions =================================================== */
#include "ComM_Pnc.h"

#if COMM_PNC_SUPPORT == STD_ON
#include "ComM_Internal.h"
#include "ComM_Channel.h"
#include "ComM_User.h"

#include "BswM_ComM.h"

#if COMM_NM_ENABLED == STD_ON
#include "Nm.h"
#endif
#include "SchM_ComM.h"
#include "IStdLib.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
typedef struct
{
#if COMM_PNC_GATEWAY_ENABLED == STD_ON
    uint8* ChannelBitVectorERA;
#endif
    uint16           PrepareSleepTimer;
    boolean          WakeUpIndication;
    ComM_PncModeType PncMode;
} ComM_PncVarType;

/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */
/**
 * Store current pnc mode and notify to SW-C if current mode of user changed.
 * @param[in]    pnc      See PNCHandleType
 * @param[in]    pncMode  See ComM_PncModeType
 * @reentrant    TRUE (Reentrant for different Pncs. Non reentrant for the same Pnc.)
 * @synchronous  TRUE
 * @trace        CPD-65017
 */
COMM_LOCAL void ComM_PncSetMode(PNCHandleType pnc, ComM_PncModeType pncMode);

/**
 * On entry of PNC sub state COMM_PNC_READY_SLEEP.
 * @param[in]    pnc   See PNCHandleType
 * @reentrant    TRUE (Reentrant for different Pncs. Non reentrant for the same Pnc.)
 * @synchronous  TRUE
 * @trace        CPD-65018
 */
COMM_LOCAL void ComM_PncBeh_EntryReadySleep(PNCHandleType pnc);

/**
 * On entry of PNC main state COMM_PNC_NO_COMMUNICATION.
 * @param[in]    pnc   See PNCHandleType
 * @reentrant    TRUE (Reentrant for different Pncs. Non reentrant for the same Pnc.)
 * @synchronous  TRUE
 * @trace        CPD-65020
 */
COMM_LOCAL void ComM_PncBeh_EntryNoCom(PNCHandleType pnc);

/**
 * On entry of PNC sub state COMM_PNC_PREPARE_SLEEP.
 * @param[in]    pnc   See PNCHandleType
 * @reentrant    TRUE (Reentrant for different Pncs. Non reentrant for the same Pnc.)
 * @synchronous  TRUE
 * @trace        CPD-65021
 */
COMM_LOCAL void ComM_PncBeh_EntryPrepareSleep(PNCHandleType pnc);

/**
 * On entry of PNC sub state COMM_PNC_REQUESTED.
 * @param[in]    pnc   See PNCHandleType
 * @reentrant    TRUE (Reentrant for different Pncs. Non reentrant for the same Pnc.)
 * @synchronous  TRUE
 * @trace        CPD-65023
 */
COMM_LOCAL void ComM_PncBeh_EntryRequst(PNCHandleType pnc);

/**
 * Evaluate target PNC Mode.
 * @param[in]    pnc   See PNCHandleType
 * @reentrant    TRUE (Reentrant for different Pncs. Non reentrant for the same Pnc.)
 * @synchronous  TRUE
 * @trace        CPD-65024
 */
COMM_LOCAL ComM_PncModeType ComM_PncEvaluateTargetPncMode(PNCHandleType pnc);

/* ============================================ internal data definitions =========================================== */
#define COMM_START_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"
COMM_LOCAL const ComM_PncConfigType* ComM_PncConfigs;
#define COMM_STOP_SEC_VAR_CLEARED_PTR
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"
COMM_LOCAL ComM_PncVarType ComM_PncVar[COMM_PNC_NUMBER];
#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"

#define COMM_START_SEC_VAR_CLEARED_8
#include "ComM_MemMap.h"
COMM_LOCAL uint8 ComM_PncBitVectorEIRA[COMM_MAX_PNC_BIT_VECTOR_LENGTH];
#define COMM_STOP_SEC_VAR_CLEARED_8
#include "ComM_MemMap.h"

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/* PRQA S 1532, 2889 ++ */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/* ========================================== external function definitions ========================================= */
/**
 * The implementation of ComM_PncInit.
 */
void ComM_PncInit(const ComM_PncConfigType* pncConfigs)
{
    for (uint8 i = 0u; i < COMM_PNC_NUMBER; ++i)
    {
        ComM_PncVarType* pncVarPtr = &ComM_PncVar[i];

        pncVarPtr->PrepareSleepTimer = 0u;
        pncVarPtr->PncMode           = COMM_PNC_NO_COMMUNICATION;
        pncVarPtr->WakeUpIndication  = FALSE;

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
        const ComM_PncConfigType* pncConfigPtr = &pncConfigs[i];
        pncVarPtr->ChannelBitVectorERA         = pncConfigPtr->ChannelBitVectorERA;
        (void)IStdLib_MemSet(pncVarPtr->ChannelBitVectorERA, 0, pncConfigPtr->ChannelBitVectorERALength);
#endif
    }

    (void)IStdLib_MemSet(ComM_PncBitVectorEIRA, 0, COMM_MAX_PNC_BIT_VECTOR_LENGTH);

    ComM_PncConfigs = pncConfigs;
}

ComM_PncModeType ComM_PncGetMode(PNCHandleType pnc)
{
    return ComM_PncVar[pnc].PncMode;
}

ComM_ModeType ComM_PncGetMaxComMode(PNCHandleType pnc)
{
    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];
    ComM_ModeType             maxComMode   = COMM_FULL_COMMUNICATION;

    for (NetworkHandleType channel = 0u; channel < COMM_CHANNEL_NUMBER; ++channel)
    {
        if (ComM_GetBit(pncConfigPtr->ChannelMappingBitVector, channel))
        {
            if (ComM_ChannelGetMaxComMode(channel) == COMM_NO_COMMUNICATION)
            {
                maxComMode = COMM_NO_COMMUNICATION;
                break;
            }
        }
    }
    return maxComMode;
}

uint16 ComM_PncGetBitIndex(PNCHandleType pnc)
{
    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];
    return pncConfigPtr->BitIndex;
}

void ComM_PncUpdateEIRA(const uint8* pncBitVectorPtr)
{
    (void)IStdLib_MemCpy(ComM_PncBitVectorEIRA, pncBitVectorPtr, COMM_MAX_PNC_BIT_VECTOR_LENGTH);
}

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
void ComM_PncUpdateChannelBitVectorERA(PNCHandleType pnc, NetworkHandleType channel, const uint8* pncBitVectorPtr)
{
    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];

    if (ComM_GetBit(pncConfigPtr->TxOnlyChannelMappingBitVector, channel))
    {
        return;
    }

    const ComM_PncVarType* varPtr = &ComM_PncVar[pnc];

    SchM_Enter_ComM_Pnc();
    if (ComM_GetBit(pncBitVectorPtr, pncConfigPtr->BitIndex))
    {
        ComM_SetBit(varPtr->ChannelBitVectorERA, channel);
    }
    else
    {
        ComM_ClrBit(varPtr->ChannelBitVectorERA, channel);
    }
    SchM_Exit_ComM_Pnc();
}

boolean ComM_PncPassiveChannelRequested(PNCHandleType pnc)
{
    const ComM_PncConfigType* configPtr = &ComM_PncConfigs[pnc];
    if (configPtr->PassiveChannel == COMM_NOT_USED_CHANNEL_ID)
    {
        return FALSE;
    }
    return ComM_GetBit(configPtr->ChannelBitVectorERA, configPtr->PassiveChannel);
}
#endif

void ComM_PncWakeUpIndication(PNCHandleType pnc)
{
    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];

    ComM_PncVar[pnc].WakeUpIndication = TRUE;

    for (NetworkHandleType channel = 0u; channel < COMM_CHANNEL_NUMBER; ++channel)
    {
        if (ComM_GetBit(pncConfigPtr->ChannelMappingBitVector, channel))
        {
            ComM_ChannelWakeUpIndication(channel, FALSE);
        }
    }
}

boolean ComM_PncUserOrActiveChannelRequested(PNCHandleType pnc)
{
    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];

#if COMM_USER_NUMBER > 0u
    for (ComM_UserHandleType user = 0u; user < COMM_USER_NUMBER; ++user)
    {
        if (ComM_GetBit(pncConfigPtr->UserMappingBitVector, user))
        {
            if (ComM_UserGetRequestedComMode(user) == COMM_FULL_COMMUNICATION)
            {
                return TRUE;
            }
        }
    }
#endif

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
    const ComM_PncVarType* pncVarPtr = &ComM_PncVar[pnc];

    for (NetworkHandleType channel = 0u; channel < COMM_CHANNEL_NUMBER; ++channel)
    {
        if (ComM_GetBit(pncConfigPtr->ChannelMappingBitVector, channel))
        {
            if (channel != pncConfigPtr->PassiveChannel)
            {
                if (ComM_GetBit(pncVarPtr->ChannelBitVectorERA, channel))
                {
                    return TRUE;
                }
            }
        }
    }
#endif

    return FALSE;
}

void ComM_PncMainFunction(PNCHandleType pnc, NetworkHandleType channel)
{
    /* clang-format off */
    void (*const pncBehaviors[][4])(PNCHandleType pnc) = {
                                    /* COMM_PNC_REQUESTED       COMM_PNC_READY_SLEEP         COMM_PNC_PREPARE_SLEEP         COMM_PNC_NO_COMMUNICATION */
        /* COMM_PNC_REQUESTED */        { NULL_PTR,                ComM_PncBeh_EntryReadySleep, ComM_PncBeh_EntryPrepareSleep, ComM_PncBeh_EntryReadySleep, },
        /* COMM_PNC_READY_SLEEP */      { ComM_PncBeh_EntryRequst, NULL_PTR,                    ComM_PncBeh_EntryPrepareSleep, ComM_PncBeh_EntryPrepareSleep, },
        /* COMM_PNC_PREPARE_SLEEP */    { ComM_PncBeh_EntryRequst, ComM_PncBeh_EntryReadySleep, NULL_PTR,                      ComM_PncBeh_EntryNoCom, },
        /* COMM_PNC_NO_COMMUNICATION */ { ComM_PncBeh_EntryRequst, ComM_PncBeh_EntryReadySleep, ComM_PncBeh_EntryPrepareSleep, NULL_PTR, },
    };
    /* clang-format on */

    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];
    if (channel != pncConfigPtr->MainChannel)
    {
        return;
    }

    ComM_PncVarType* pncVarPtr = &ComM_PncVar[pnc];
    if (pncVarPtr->PrepareSleepTimer > 0u)
    {
        --pncVarPtr->PrepareSleepTimer;
    }

    ComM_PncModeType targetPncMode = ComM_PncEvaluateTargetPncMode(pnc);
    ComM_PncModeType previousPncMode;
    do
    {
        previousPncMode = pncVarPtr->PncMode;

        void (*const behavior)(PNCHandleType pnc) = pncBehaviors[pncVarPtr->PncMode][targetPncMode];
        if (behavior != NULL_PTR)
        {
            behavior(pnc);
        }
    } while (pncVarPtr->PncMode != previousPncMode);
}

#if COMM_USER_NUMBER > 0u
/**
 * The implementation of ComM_PncIsUserAssigned.
 */
boolean ComM_PncIsUserAssigned(PNCHandleType pnc, ComM_UserHandleType user)
{
    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];
    return ComM_GetBit(pncConfigPtr->UserMappingBitVector, user);
}
#endif

/**
 * The implementation of ComM_PncIsChannelAssigned.
 */
boolean ComM_PncIsChannelAssigned(PNCHandleType pnc, NetworkHandleType channel)
{
    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];
    return ComM_GetBit(pncConfigPtr->ChannelMappingBitVector, channel);
}

/* ========================================== internal function definitions ========================================= */
COMM_LOCAL void ComM_PncSetMode(PNCHandleType pnc, ComM_PncModeType pncMode)
{
    ComM_PncVarType* pncVarPtr = &ComM_PncVar[pnc];

#if COMM_CURRENT_MODE_NOTIFICATION_ENABLED == STD_ON
    const ComM_PncConfigType* pncConfigPtr                              = &ComM_PncConfigs[pnc];
    ComM_ModeType             previousUserCurrentMode[COMM_USER_NUMBER] = {0};

    SchM_Enter_ComM_UM();
    for (ComM_UserHandleType user = 0u; user < COMM_USER_NUMBER; ++user)
    {
        if (ComM_GetBit(pncConfigPtr->UserMappingBitVector, user))
        {
            previousUserCurrentMode[user] = ComM_UserGetCurrentMode(user);
        }
    }

    pncVarPtr->PncMode = pncMode;

    for (ComM_UserHandleType user = 0u; user < COMM_USER_NUMBER; ++user)
    {
        if (ComM_GetBit(pncConfigPtr->UserMappingBitVector, user))
        {
            ComM_ModeType nextUserCurrentMode = ComM_UserGetCurrentMode(user);

            if (previousUserCurrentMode[user] != nextUserCurrentMode)
            {
                ComM_UserSwitchCurrentMode(user, nextUserCurrentMode);
            }
        }
    }
    SchM_Exit_ComM_UM();
#else
    pncVarPtr->PncMode = pncMode;
#endif
}

COMM_LOCAL void ComM_PncBeh_EntryReadySleep(PNCHandleType pnc)
{
    ComM_PncSetMode(pnc, COMM_PNC_READY_SLEEP);

    /* SWS_ComM_00908 notified by the API call BswM_ComM_CurrentPncMode() with the entered PNC state */
    BswM_ComM_CurrentPNCMode(pnc, COMM_PNC_READY_SLEEP);
}

COMM_LOCAL void ComM_PncBeh_EntryNoCom(PNCHandleType pnc)
{
    const ComM_PncVarType* pncVarPtr = &ComM_PncVar[pnc];

    if (pncVarPtr->PrepareSleepTimer == 0u)
    {
        ComM_PncSetMode(pnc, COMM_PNC_NO_COMMUNICATION);
        /* SWS_ComM_00908 notified by the API call BswM_ComM_CurrentPncMode() with the entered PNC state */
        BswM_ComM_CurrentPNCMode(pnc, COMM_PNC_NO_COMMUNICATION);
    }
}

COMM_LOCAL void ComM_PncBeh_EntryPrepareSleep(PNCHandleType pnc)
{
    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];
    ComM_PncVarType*          pncVarPtr    = &ComM_PncVar[pnc];

    ComM_PncSetMode(pnc, COMM_PNC_PREPARE_SLEEP);

    pncVarPtr->PrepareSleepTimer = pncConfigPtr->PrepareSleepTimer;

    /* SWS_ComM_00908 notified by the API call BswM_ComM_CurrentPncMode() with the entered PNC state */
    BswM_ComM_CurrentPNCMode(pnc, COMM_PNC_PREPARE_SLEEP);
}

COMM_LOCAL void ComM_PncBeh_EntryRequst(PNCHandleType pnc)
{
    ComM_PncSetMode(pnc, COMM_PNC_REQUESTED);

    const ComM_PncConfigType* pncConfigPtr              = &ComM_PncConfigs[pnc];
    boolean                   wakeupSleepRequestEnabled = FALSE;

    for (NetworkHandleType channel = 0u; channel < COMM_CHANNEL_NUMBER; ++channel)
    {
        if (ComM_GetBit(pncConfigPtr->ChannelMappingBitVector, channel))
        {
            if (pncConfigPtr->WakeupSleepRequestEnabled)
            {
                if (ComM_ChannelSetWakeupSleepRequest(channel))
                {
                    wakeupSleepRequestEnabled = TRUE;
                }
            }
            ComM_ChannelSetPncNmRequest(channel);
        }
    }

    BswM_ComM_CurrentPNCMode(
        pnc,
        wakeupSleepRequestEnabled ? COMM_PNC_REQUESTED_WITH_WAKEUP_REQUEST : COMM_PNC_REQUESTED);
}

COMM_LOCAL ComM_PncModeType ComM_PncEvaluateTargetPncMode(PNCHandleType pnc)
{
    ComM_PncVarType* pncVarPtr        = &ComM_PncVar[pnc];
    boolean          wakeUpIndication = pncVarPtr->WakeUpIndication;
    pncVarPtr->WakeUpIndication       = FALSE;

    if (ComM_PncUserOrActiveChannelRequested(pnc))
    {
        return COMM_PNC_REQUESTED;
    }

#if COMM_PNC_GATEWAY_ENABLED == STD_ON
    if (ComM_PncPassiveChannelRequested(pnc))
    {
        return COMM_PNC_REQUESTED;
    }
#endif

    const ComM_PncConfigType* pncConfigPtr = &ComM_PncConfigs[pnc];
    if (ComM_GetBit(ComM_PncBitVectorEIRA, pncConfigPtr->BitIndex))
    {
        return COMM_PNC_READY_SLEEP;
    }

    if (wakeUpIndication)
    {
        return COMM_PNC_PREPARE_SLEEP;
    }

    return COMM_PNC_NO_COMMUNICATION;
}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
/* PRQA S 1532, 2889 -- */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn */

#endif /* COMM_PNC_SUPPORT == STD_ON */
