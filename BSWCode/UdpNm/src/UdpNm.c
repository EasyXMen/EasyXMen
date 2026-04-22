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
 ************************************************************************************************************************
 **
 **  @file               : UdpNm.c
 **  @author             : caihong.liu
 **  @date               : 2025/01/16
 **  @vendor             : isoft
 **  @description        : Udp Netwprk Manager
 **
 ***********************************************************************************************************************/
/* PRQA S 1503, 1532 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "UdpNm.h"
#include "UdpNm_Internal.h"
#include "IStdLib.h"
#include "Det.h"
#include "Nm.h"
#include "SchM_UdpNm.h"
#include "SoAd.h"
#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
#include "PduR_UdpNm.h"
#endif

/* ===================================================== macros ===================================================== */
#define UDPNM_UNUSED(x) (void)(x)

/* ================================================ type definitions ================================================ */
typedef enum
{
    UDPNM_UNINIT,
    UDPNM_INIT,
} UdpNm_InitStatusType;

/* ========================================== internal function declarations ======================================== */
#define UDPNM_START_SEC_CODE
#include "UdpNm_MemMap.h"

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS UdpNm_TestAndClear
#endif
UDPNM_LOCAL_INLINE boolean         UdpNm_TestAndClear(boolean* flagPtr);
UDPNM_LOCAL void                   UdpNm_SetTxPduCbvBit(UdpNm_ChannelIndexType chIndex, uint8 mask);
UDPNM_LOCAL void                   UdpNm_ClrTxPduCbvBit(UdpNm_ChannelIndexType chIndex, uint8 mask);
UDPNM_LOCAL UdpNm_ChannelIndexType UdpNm_FindChannelIndex(NetworkHandleType nmChannelHandle);

/**
 *  Function used to process the received data.
 *  */
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
UDPNM_LOCAL void UdpNm_RxNodeDetectHandle(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

#if UDPNM_CAR_WAKEUP_RX_ENABLED == STD_ON
#if UDPNM_CAR_WAKEUP_FILTER_ENABLED == STD_ON
UDPNM_LOCAL boolean UdpNm_RxCarWakeUpFilter(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif
UDPNM_LOCAL void UdpNm_RxCarWakeUpHandle(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
UDPNM_LOCAL void UdpNm_CoordReadyToSleepIndHandle(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
UDPNM_LOCAL boolean UdpNm_RxPnFilter(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON) || (UDPNM_NODE_DETECTION_ENABLED == STD_ON) \
    || (UDPNM_CAR_WAKEUP_RX_ENABLED == STD_ON) || (UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
UDPNM_LOCAL void UdpNm_RxDataMainHandle(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

/**
 *  Functions for all timers, including timing, reload, and timeout handling.
 *  */
UDPNM_LOCAL void UdpNm_StateHoldTimeoutHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void UdpNm_NmTimeoutHandle(UdpNm_ChannelIndexType chIndex);
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF && UDPNM_TX_PDU_NUMBER > 0u
UDPNM_LOCAL void UdpNm_MsgCycleTimeHandle(UdpNm_ChannelIndexType chIndex);
#endif
#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
UDPNM_LOCAL void UdpNm_RemoteSleepCancellHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void UdpNm_RemoteSleepTimeoutHandle(UdpNm_ChannelIndexType chIndex);
#endif
UDPNM_LOCAL void UdpNm_TimerManagement(UdpNm_ChannelIndexType chIndex);

/**
 *  Function to handle state machine switching.
 *  */
UDPNM_LOCAL void UdpNm_StateChange(UdpNm_ChannelIndexType chIndex, Nm_StateType nmOldState, Nm_StateType nmNewState);
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
UDPNM_LOCAL void UdpNm_StartTransmissionNmPdu(UdpNm_ChannelIndexType chIndex, boolean isNetWorkRequest);
#endif
UDPNM_LOCAL void
    UdpNm_EnterRepeatMessageState(UdpNm_ChannelIndexType chIndex, Nm_StateType nmOldState, boolean isNetWorkRequest);
UDPNM_LOCAL void UdpNm_EnterPrepareBusSleepModeHandle(UdpNm_ChannelIndexType chIndex);
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
UDPNM_LOCAL boolean UdpNm_NodeDetectStateHandle(UdpNm_ChannelIndexType chIndex);
#endif
UDPNM_LOCAL boolean UdpNm_NetworkStateHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void    UdpNm_RepeatMessageStateHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void    UdpNm_NormalOperationStateHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void    UdpNm_ReadySleepStateHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL boolean UdpNm_WakeUpSignalHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void    UdpNm_PrepareBusSleepStateHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void    UdpNm_BusSleepStateHandle(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void    UdpNm_StateManagement(UdpNm_ChannelIndexType chIndex);

/**
 *  Function for sending periodic packets.
 *  */
#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
UDPNM_LOCAL void UdpNm_WriteUserDataBytes(UdpNm_ChannelIndexType chIndex);
#endif
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF && UDPNM_TX_PDU_NUMBER > 0u
UDPNM_LOCAL Std_ReturnType UdpNm_SendNmPdu(UdpNm_ChannelIndexType chIndex);
UDPNM_LOCAL void           UdpNm_SendMsgMainHandle(UdpNm_ChannelIndexType chIndex);
#endif

UDPNM_LOCAL uint16 UdpNm_GetPartitionIndex(void);
#if UDPNM_DEV_ERROR_DETECT == STD_ON
UDPNM_LOCAL boolean UdpNm_ValidatePartitionContext(uint8 apiId);
UDPNM_LOCAL boolean UdpNm_ValidateInitStatus(uint8 apiId);
UDPNM_LOCAL boolean UdpNm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType nmChannelHandle);
UDPNM_LOCAL boolean UdpNm_ValidatePointer(uint8 apiId, const void* pointer);
UDPNM_LOCAL boolean UdpNm_ValidateTxPduId(uint8 apiId, PduIdType txPduId);
#if UDPNM_TRIGGER_TRANSMIT_API == STD_ON
UDPNM_LOCAL boolean UdpNm_ValidateTxPdu(uint8 apiId, PduIdType txPduId, const PduInfoType* pduInfoPtr);
#endif
UDPNM_LOCAL boolean UdpNm_ValidateRxPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr);
UDPNM_LOCAL boolean UdpNm_ValidateUserDataPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr);
#endif
#define UDPNM_STOP_SEC_CODE
#include "UdpNm_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define UDPNM_START_SEC_VAR_CLEARED_8
#include "UdpNm_MemMap.h"
/**
 * @brief The initialization state of the UdpNm module.
 */
UDPNM_LOCAL UdpNm_InitStatusType UdpNm_InitStatus[UDPNM_NUMBER_OF_PARTITION]; /**< @range NA */
#define UDPNM_STOP_SEC_VAR_CLEARED_8
#include "UdpNm_MemMap.h"

#define UDPNM_START_SEC_VAR_CLEARED_PTR
#include "UdpNm_MemMap.h"
/**
 * @brief Point to the configuration of the UdpNm module, only available after initialization.
 */
UDPNM_LOCAL const UdpNm_ConfigType* UdpNm_CfgPtr; /**< @range NA */
#define UDPNM_STOP_SEC_VAR_CLEARED_PTR
#include "UdpNm_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define UDPNM_START_SEC_CODE
#include "UdpNm_MemMap.h"
/**
 * Initialize the UdpNm module.
 */
void UdpNm_Init(const UdpNm_ConfigType* UdpNmConfigPtr)
{
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_INIT) && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_INIT)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_INIT, UdpNmConfigPtr))
#endif
    {
        UdpNm_CfgPtr = UdpNmConfigPtr;
#if UDPNM_MULTIPLE_PARTITION_USED == STD_ON
        ApplicationType applicationID = GetApplicationID();
#endif

        for (UdpNm_ChannelIndexType chIndex = 0u; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
#if UDPNM_MULTIPLE_PARTITION_USED == STD_ON
            if (chCfgPtr->ApplicationID == applicationID)
#endif
            {
                UdpNm_InnerChannelType* chRTPtr = chCfgPtr->RuntimePtr;

                /** SWS_UdpNm_00141 */
                chRTPtr->UdpNmState = NM_STATE_BUS_SLEEP;

                /** SWS_UdpNm_00144 */
                chRTPtr->UdpnmMode = NM_MODE_BUS_SLEEP;

                /** SWS_UdpNm_00143 */
                chRTPtr->NetRequestStatus = UDPNM_NETWORK_RELEASED;

                /** Passive startup flag */
                chRTPtr->PassiveStartUp = FALSE;

                for (uint8 loop = 0u; loop < (uint8)UDPNM_MAX_TIMER_TYPE; loop++)
                {
                    chRTPtr->TickTimers[loop]  = 0u;
                    chRTPtr->TimeoutFlag[loop] = FALSE;
                }

                /** SWS_UdpNm_00485 */
                for (uint8 loop = 0u; loop < UDPNM_PDU_MAX_LENGTH; loop++)
                {
#if (UDPNM_USER_DATA_ENABLED == STD_ON) || (UDPNM_NODE_ID_ENABLED == STD_ON) || (UDPNM_NODE_DETECTION_ENABLED == STD_ON)
                    chRTPtr->RxPduData[loop] = 0u;
#endif
                    chRTPtr->TxPduData[loop] = 0u;
                }

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
                chRTPtr->SendNmMsgFlg  = FALSE;
                chRTPtr->NetRequestFlg = FALSE;
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
                chRTPtr->RetrySendMsgFlag = FALSE;
#endif
#endif

#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
                chRTPtr->ImmedMsgTxCnt = 0u;
#endif

#if UDPNM_COM_CONTROL_ENABLED == STD_ON
                chRTPtr->UdpNmTxEnable = TRUE;
#endif

#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
                chRTPtr->CoordReadyToSleepInd = UDPNM_CSR_IND_INIT;
#endif
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
                chRTPtr->RepeatMessageRequest  = FALSE;
                chRTPtr->RepeatMessageDetected = FALSE;
#endif

                const UdpNm_ChannelPBConfigType* chPBCfgPtr = &UdpNm_CfgPtr->ChlPBCfgPtr[chIndex];
#if UDPNM_NODE_ID_ENABLED == STD_ON
                /**
                 *  Set NodeId to config data
                 *  */
                if (chCfgPtr->PduNidPosition != UDPNM_PDU_OFF)
                {
                    chRTPtr->TxPduData[chCfgPtr->PduNidPosition] = chPBCfgPtr->NodeId;
                }
#endif

                /**
                 *  Set CBV to 0x00
                 *  */
                if (chCfgPtr->PduCbvPosition != UDPNM_PDU_OFF)
                {
                    chRTPtr->TxPduData[chCfgPtr->PduCbvPosition] = 0x00u;
                }

#if UDPNM_USER_DATA_ENABLED == STD_ON
                /** SWS_UdpNm_00025 */
                for (uint8 loop = 0u; loop < chPBCfgPtr->UserDataLength; loop++)
                {
                    chRTPtr->TxPduData[loop + chPBCfgPtr->UserDataOffset] = UDPNM_USER_DATA_INITIAL_BYTE;
                }
#endif

#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
                /** SWS_UdpNm_00413
                 *  The usage of the CBV is mandatory in case Partial Networking is used.
                 *  */
                if (chPBCfgPtr->PnEnabled)
                {
                    UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_PNI_MASK);
                }
#endif
            }
        }
        UdpNm_InitStatus[UdpNm_GetPartitionIndex()] = UDPNM_INIT;
    }
}

/**
 * Passive startup of the AUTOSAR UdpNm.
 */
Std_ReturnType UdpNm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_PASSIVESTARUP)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_PASSIVESTARUP)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_PASSIVESTARUP, nmChannelHandle))
#endif
    {
        const UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType*      chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        SchM_Enter_UdpNm_Context();
        /** SWS_UdpNm_00147 */
        if (chRTPtr->UdpnmMode != NM_MODE_NETWORK)
        {
            chRTPtr->PassiveStartUp = TRUE;
            ret                     = E_OK;
        }
        SchM_Exit_UdpNm_Context();
    }

    return ret;
}

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * Request the network, since ECU needs to communicate on the bus.
 */
Std_ReturnType UdpNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_NETWORKREQUEST)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_NETWORKREQUEST)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_NETWORKREQUEST, nmChannelHandle))
#endif
    {
        const UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType*      chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        /** SWS_UdpNm_00104 */
        SchM_Enter_UdpNm_Context();
        chRTPtr->NetRequestStatus = UDPNM_NETWORK_REQUESTED;
        chRTPtr->NetRequestFlg    = TRUE;
#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
        chRTPtr->CoordReadyToSleepInd = UDPNM_CSR_IND_INIT;
#endif
        SchM_Exit_UdpNm_Context();

        ret = E_OK;
    }
    return ret;
}
#endif

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * Release the network, since ECU doesn't have to communicate on the bus.
 */
Std_ReturnType UdpNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_NETWORKRELEASE)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_NETWORKRELEASE)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_NETWORKRELEASE, nmChannelHandle))
#endif
    {
        const UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType*      chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        /** SWS_UdpNm_00105 */
        SchM_Enter_UdpNm_Context();
        chRTPtr->NetRequestStatus = UDPNM_NETWORK_RELEASED;
        SchM_Exit_UdpNm_Context();

        ret = E_OK;
    }
    return ret;
}
#endif

#if UDPNM_COM_CONTROL_ENABLED == STD_ON
/**
 * Disable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service.
 */
Std_ReturnType UdpNm_DisableCommunication(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;

/** SWS_UdpNm_00307 */
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_DISABLECOMMUNICATION)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_DISABLECOMMUNICATION)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_DISABLECOMMUNICATION, nmChannelHandle))
#endif
    {
        const UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType*      chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        /** SWS_UdpNm_00172 */
        SchM_Enter_UdpNm_Context();
        if (chRTPtr->UdpnmMode == NM_MODE_NETWORK)
        {
            /** SWS_UdpNm_00170 */
            chRTPtr->UdpNmTxEnable = FALSE;

            /** SWS_UdpNm_00173 */
            chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = 0u;

            /** SWS_UdpNm_00174 */
            chRTPtr->TickTimers[UDPNM_NM_TIMEOUT_TIMER] = 0u;

#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
            /** SWS_UdpNm_00175 */
            chRTPtr->TickTimers[UDPNM_REMOTE_SLEEP_TIMER] = 0u;
#endif
            ret = E_OK;
        }
        SchM_Exit_UdpNm_Context();
    }
#else
    UDPNM_UNUSED(nmChannelHandle);
#endif

    return ret;
}
#endif

#if UDPNM_COM_CONTROL_ENABLED == STD_ON
/**
 * Enable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service.
 */
Std_ReturnType UdpNm_EnableCommunication(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
/** SWS_UdpNm_00306 */
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_ENABLECOMMUNICATION)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_ENABLECOMMUNICATION)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_ENABLECOMMUNICATION, nmChannelHandle))
#endif
    {
        const UdpNm_ChannelIndexType    chIndex  = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
        UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

        SchM_Enter_UdpNm_Context();
        /** SWS_UdpNm_00177 SWS_UdpNm_00305 */
        if (!chRTPtr->UdpNmTxEnable && (chRTPtr->UdpnmMode == NM_MODE_NETWORK))
        {
            chRTPtr->UdpNmTxEnable = TRUE;

            /** SWS_UdpNm_00178
             *   Start transmission of NM PDUs within the next NM main function.
             *  */
            chRTPtr->SendNmMsgFlg                        = TRUE;
            chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;

            /** SWS_UdpNm_00179 */
            chRTPtr->TickTimers[UDPNM_NM_TIMEOUT_TIMER] = chCfgPtr->TimeoutTime;

/** SWS_UdpNm_00180 */
#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
            chRTPtr->TickTimers[UDPNM_REMOTE_SLEEP_TIMER] = chCfgPtr->RemoteSleepIndTime;
#endif
            ret = E_OK;
        }
        SchM_Exit_UdpNm_Context();
    }
#else
    UDPNM_UNUSED(nmChannelHandle);
#endif

    return ret;
}
#endif

#if (UDPNM_USER_DATA_ENABLED == STD_ON) && (UDPNM_COM_USERDATA_SUPPORT == STD_OFF) \
    && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * Set user data for NM PDUs transmitted next on the bus.
 */
Std_ReturnType UdpNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_SETUSERDATA)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_SETUSERDATA)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_SETUSERDATA, nmChannelHandle)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_SETUSERDATA, nmUserDataPtr))
#endif
    {
        const UdpNm_ChannelIndexType     chIndex    = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelPBConfigType* chPBCfgPtr = &UdpNm_CfgPtr->ChlPBCfgPtr[chIndex];
        UdpNm_InnerChannelType*          chRTPtr    = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        /** SWS_UdpNm_00159 */
        uint16 userDataLength = chPBCfgPtr->UserDataLength;
        uint16 userDataOffset = chPBCfgPtr->UserDataOffset;

        SchM_Enter_UdpNm_Context();
        (void)IStdLib_MemCpy(&chRTPtr->TxPduData[userDataOffset], nmUserDataPtr, userDataLength);
        SchM_Exit_UdpNm_Context();

        ret = E_OK;
    }

    return ret;
}
#endif

#if UDPNM_USER_DATA_ENABLED == STD_ON
/**
 * Get user data out of the most recently received NM PDU.
 */
Std_ReturnType UdpNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_GETUSERDATA)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_GETUSERDATA)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_GETUSERDATA, nmChannelHandle)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_GETUSERDATA, nmUserDataPtr))
#endif
    {
        const UdpNm_ChannelIndexType     chIndex    = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelPBConfigType* chPBCfgPtr = &UdpNm_CfgPtr->ChlPBCfgPtr[chIndex];
        const UdpNm_InnerChannelType*    chRTPtr    = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        /** SWS_UdpNm_00160 */
        uint16       userDataLength = chPBCfgPtr->UserDataLength;
        uint16       userDataOffset = chPBCfgPtr->UserDataOffset;
        const uint8* userDataPtr    = &chRTPtr->RxPduData[userDataOffset];

        SchM_Enter_UdpNm_Context();
        (void)IStdLib_MemCpy(nmUserDataPtr, userDataPtr, userDataLength);
        SchM_Exit_UdpNm_Context();

        ret = E_OK;
    }
    return ret;
}
#endif

#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * Requests transmission of a PDU.
 */
Std_ReturnType UdpNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_TRANSMIT) && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_TRANSMIT)
        && UdpNm_ValidateUserDataPdu(UDPNM_SERVICE_ID_TRANSMIT, TxPduId, PduInfoPtr))
#endif
    {
        for (uint8 chIndex = 0; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const UdpNm_UserDataTxPduType* userDataPduPtr = UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].UserDataTxPdu;
            if ((userDataPduPtr != NULL_PTR) && (userDataPduPtr->TxUserDataPduId == TxPduId))
            {
                const UdpNm_ChannelPBConfigType* chPBCfgPtr = &UdpNm_CfgPtr->ChlPBCfgPtr[chIndex];
                UdpNm_InnerChannelType*          chRTPtr    = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

                /** SWS_UdpNm_00464 */
                if (((chRTPtr->UdpNmState == NM_STATE_NORMAL_OPERATION)
                     || (chRTPtr->UdpNmState == NM_STATE_REPEAT_MESSAGE))
                    && (PduInfoPtr->SduLength >= chPBCfgPtr->UserDataLength))
                {
                    chRTPtr->SendNmMsgFlg = TRUE;
                    ret                   = E_OK;
                }
                break;
            }
        }
    }
    UDPNM_UNUSED(PduInfoPtr);
    return ret;
}
#endif

/**
 * Get node identifier out of the most recently received NM PDU.
 */
Std_ReturnType UdpNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_NODE_ID_ENABLED == STD_ON
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_GETNODEIDENTIFIER)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_GETNODEIDENTIFIER)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_GETNODEIDENTIFIER, nmChannelHandle)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_GETNODEIDENTIFIER, nmNodeIdPtr))
#endif
    {
        const UdpNm_ChannelIndexType    chIndex  = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];

        /** SWS_UdpNm_00132 */
        if (chCfgPtr->NodeIdEnabled)
        {
            const UdpNm_InnerChannelType* chRTPtr = chCfgPtr->RuntimePtr;

            SchM_Enter_UdpNm_Context();
            *nmNodeIdPtr = chRTPtr->RxPduData[(uint8)chCfgPtr->PduNidPosition];
            SchM_Exit_UdpNm_Context();
            ret = E_OK;
        }
    }
#else
    UDPNM_UNUSED(nmChannelHandle);
    UDPNM_UNUSED(nmNodeIdPtr);
#endif
    return ret;
}

/**
 * Get node identifier configured for the local node.
 */
Std_ReturnType UdpNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_NODE_ID_ENABLED == STD_ON
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_GETLOCALNODEIDENTIFIER)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_GETLOCALNODEIDENTIFIER)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_GETLOCALNODEIDENTIFIER, nmChannelHandle)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_GETLOCALNODEIDENTIFIER, nmNodeIdPtr))
#endif
    {
        const UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);

        /** SWS_UdpNm_00133 */
        if (UdpNm_ChannelLCfgData[chIndex].NodeIdEnabled)
        {
            *nmNodeIdPtr = UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].NodeId;
            ret          = E_OK;
        }
    }
#else
    UDPNM_UNUSED(nmChannelHandle);
    UDPNM_UNUSED(nmNodeIdPtr);
#endif
    return ret;
}

/**
 * Set Repeat Message Request Bit for NM PDUs transmitted next on the bus.
 */
Std_ReturnType UdpNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_REPEATMESSAGEREQUEST)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_REPEATMESSAGEREQUEST)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_REPEATMESSAGEREQUEST, nmChannelHandle))
#endif
    {
        const UdpNm_ChannelIndexType    chIndex  = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];

        if (chCfgPtr->NodeDetectionEnabled)
        {
            UdpNm_InnerChannelType* chRTPtr = chCfgPtr->RuntimePtr;

            SchM_Enter_UdpNm_Context();
            /** SWS_UdpNm_00137 */
            if ((chRTPtr->UdpNmState == NM_STATE_NORMAL_OPERATION) || (chRTPtr->UdpNmState == NM_STATE_READY_SLEEP))
            {
                chRTPtr->RepeatMessageRequest = TRUE;
                ret                           = E_OK;
            }
            SchM_Exit_UdpNm_Context();
        }
    }
#else
    UDPNM_UNUSED(nmChannelHandle);
#endif
    return ret;
}

/**
 * Get the whole PDU data out of the most recently received NM PDU.
 */
Std_ReturnType UdpNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_USER_DATA_ENABLED == STD_ON || UDPNM_NODE_ID_ENABLED == STD_ON || UDPNM_NODE_DETECTION_ENABLED == STD_ON
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_GETPDUDATA)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_GETPDUDATA)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_GETPDUDATA, nmChannelHandle)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_GETPDUDATA, nmPduDataPtr))
#endif
    {
        const UdpNm_ChannelIndexType    chIndex  = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];

/** SWS_UdpNm_00138 SWS_UdpNm_00375 */
#if UDPNM_USER_DATA_ENABLED == STD_OFF
        if (FALSE
#if UDPNM_NODE_ID_ENABLED == STD_ON
            || chCfgPtr->NodeIdEnabled
#endif
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
            || chCfgPtr->NodeDetectionEnabled
#endif
        )
#endif
        {
            SchM_Enter_UdpNm_Context();
            (void)IStdLib_MemCpy(
                nmPduDataPtr,
                chCfgPtr->RuntimePtr->RxPduData,
                UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].ChannelPduLength);
            SchM_Exit_UdpNm_Context();

            ret = E_OK;
        }
    }
#else
    UDPNM_UNUSED(nmChannelHandle);
    UDPNM_UNUSED(nmPduDataPtr);
#endif
    return ret;
}

/**
 * Returns the state and the mode of the network management.
 */
Std_ReturnType UdpNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_GETSTATE) && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_GETSTATE)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_GETSTATE, nmChannelHandle)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_GETSTATE, nmStatePtr)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_GETSTATE, nmModePtr))
#endif
    {
        const UdpNm_ChannelIndexType  chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        SchM_Enter_UdpNm_Context();
        *nmStatePtr = chRTPtr->UdpNmState;
        *nmModePtr  = chRTPtr->UdpnmMode;
        SchM_Exit_UdpNm_Context();
        ret = E_OK;
    }

    return ret;
}

#if UDPNM_VERSION_INFO_API == STD_ON
/**
 * This service returns the version information of this module.
 */
void UdpNm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePointer(UDPNM_SERVICE_ID_GETVERSIONINFO, versioninfo))
#endif
    {
        versioninfo->vendorID         = UDPNM_VENDOR_ID;
        versioninfo->moduleID         = UDPNM_MODULE_ID;
        versioninfo->sw_major_version = UDPNM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = UDPNM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = UDPNM_SW_PATCH_VERSION;
    }
}
#endif

#if (UDPNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * Request bus synchronization.
 */
Std_ReturnType UdpNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION, nmChannelHandle))
#endif
    {
        const UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        UdpNm_InnerChannelType*      chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        SchM_Enter_UdpNm_Context();
        /** SWS_UdpNm_00187 SWS_UdpNm_00181 */
        if ((chRTPtr->UdpnmMode == NM_MODE_NETWORK)
#if UDPNM_COM_CONTROL_ENABLED == STD_ON
            && chRTPtr->UdpNmTxEnable
#endif
        )
        {
            /** SWS_UdpNm_00130 */
            chRTPtr->SendNmMsgFlg = TRUE;
            ret                   = E_OK;
        }
        SchM_Exit_UdpNm_Context();
    }

    return ret;
}
#endif

#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
/**
 * Check if remote sleep indication takes place or not.
 */
Std_ReturnType UdpNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION, nmChannelHandle)
        && UdpNm_ValidatePointer(UDPNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION, nmRemoteSleepIndPtr))
#endif
    {
        const UdpNm_ChannelIndexType  chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        const UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

        SchM_Enter_UdpNm_Context();
        /** SWS_UdpNm_00154 */
        if ((chRTPtr->UdpNmState == NM_STATE_NORMAL_OPERATION) || (chRTPtr->UdpNmState == NM_STATE_READY_SLEEP))
        {
            *nmRemoteSleepIndPtr = chRTPtr->TimeoutFlag[UDPNM_REMOTE_SLEEP_TIMER];
            ret                  = E_OK;
        }
        SchM_Exit_UdpNm_Context();
    }

    return ret;
}
#endif

#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
/**
 * Set the NM Coordinator Sleep Ready bit in the Control Bit Vector.
 */
Std_ReturnType UdpNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit)
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_SETSLEEPREADYBIT)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_SETSLEEPREADYBIT)
        && UdpNm_ValidateNetworkHandle(UDPNM_SERVICE_ID_SETSLEEPREADYBIT, nmChannelHandle))
#endif
    {
        const UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);
        /** SWS_UdpNm_00321 */
        if (nmSleepReadyBit)
        {
            UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_CSR_MASK);
        }
        else
        {
            UdpNm_ClrTxPduCbvBit(chIndex, UDPNM_CBV_BIT_CSR_MASK);
        }
        UdpNm_ChannelLCfgData[chIndex].RuntimePtr->SendNmMsgFlg = TRUE;
        ret                                                     = E_OK;
    }

    return ret;
}
#endif

/**
 * The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
 */
void UdpNm_SoAdIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_IFTXCONFIRMATION)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_IFTXCONFIRMATION)
        && UdpNm_ValidateTxPduId(UDPNM_SERVICE_ID_IFTXCONFIRMATION, TxPduId))
#endif
    {
        uint8 chIndex;
        for (chIndex = 0u; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const UdpNm_TxPduType* txPduPtr = UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].TxPdu;
            if ((txPduPtr != NULL_PTR) && (txPduPtr->TxConfirmationPduId == TxPduId))
            {
                if (result == E_OK)
                {
                    UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;
                    SchM_Enter_UdpNm_Context();
                    /** SWS_UdpNm_00099 */
                    if (chRTPtr->UdpnmMode == NM_MODE_NETWORK)
                    {
                        chRTPtr->TickTimers[UDPNM_NM_TIMEOUT_TIMER] = UdpNm_ChannelLCfgData[chIndex].TimeoutTime;
                    }
                    SchM_Exit_UdpNm_Context();
                }
                else
                {
                    /** SWS_UdpNm_00379 */
                    Nm_TxTimeoutException(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
                }

#if (UDPNM_COM_USERDATA_SUPPORT == STD_ON) && (UDPNM_USERDATA_TX_PDU_NUM > 0)
                const UdpNm_UserDataTxPduType* userDataPduPtr = UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].UserDataTxPdu;
                if (userDataPduPtr != NULL_PTR)
                {
                    /** SWS_UdpNm_00316 */
                    PduR_UdpNmTxConfirmation(userDataPduPtr->UpperLayerPduId, result);
                }
#endif
                break;
            }
        }
    }
}

/**
 * Indication of a received PDU from a lower layer communication interface module.
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
void UdpNm_SoAdIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 2889 -- */ /* VL_QAC_MultiReturn */
{
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_RXINDICATION)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_RXINDICATION)
        && UdpNm_ValidateRxPdu(UDPNM_SERVICE_ID_RXINDICATION, RxPduId, PduInfoPtr))
#endif
    {
        for (uint8 chIndex = 0u; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const UdpNm_ChannelPBConfigType* chPBCfgPtr = &UdpNm_CfgPtr->ChlPBCfgPtr[chIndex];
            for (PduIdType pduIdx = 0u; pduIdx < chPBCfgPtr->RxPduNum; pduIdx++)
            {
                if (chPBCfgPtr->RxPdu[pduIdx].RxPduId == RxPduId)
                {
#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
                    if (UdpNm_RxPnFilter(chIndex, PduInfoPtr->SduDataPtr))
#endif
                    {
                        UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;
#if (UDPNM_USER_DATA_ENABLED == STD_ON) || (UDPNM_NODE_ID_ENABLED == STD_ON) || (UDPNM_NODE_DETECTION_ENABLED == STD_ON)
                        SchM_Enter_UdpNm_Context();
                        /** SWS_UdpNm_00035 */
                        (void)IStdLib_MemCpy(
                            chRTPtr->RxPduData,
                            PduInfoPtr->SduDataPtr,
                            ((PduInfoPtr->SduLength > chPBCfgPtr->ChannelPduLength) ? chPBCfgPtr->ChannelPduLength
                                                                                    : PduInfoPtr->SduLength));
                        SchM_Exit_UdpNm_Context();
#endif
#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON) || (UDPNM_NODE_DETECTION_ENABLED == STD_ON) \
    || (UDPNM_CAR_WAKEUP_RX_ENABLED == STD_ON) || (UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
                        UdpNm_RxDataMainHandle(chIndex, PduInfoPtr->SduDataPtr);
                        chRTPtr->RxPduExtFlg = TRUE;
#endif
/** SWS_UdpNm_00037 */
#if UDPNM_PDU_RX_INDICATION_ENABLED == STD_ON
                        Nm_PduRxIndication(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
#endif
                    }
                    return;
                }
            }
        }
    }
}

#if UDPNM_TRIGGER_TRANSMIT_API == STD_ON
/**
 * Within this API, the upper layer module (called module) shall check whether the available data
 * fits into the buffer size reported by PduInfoPtr->SduLength. If it fits, it shall copy its data into the
 * buffer provided by PduInfoPtr->SduDataPtr and update the length of the actual copied data in
 * PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType UdpNm_SoAdIfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
/* PRQA S 1503 -- */ /* VL_QAC_NoUsedApi */
{
    Std_ReturnType ret = E_NOT_OK;
#if UDPNM_DEV_ERROR_DETECT == STD_ON
    if (UdpNm_ValidatePartitionContext(UDPNM_SERVICE_ID_IFTRIGGERTRANSMIT)
        && UdpNm_ValidateInitStatus(UDPNM_SERVICE_ID_IFTRIGGERTRANSMIT)
        && UdpNm_ValidateTxPdu(UDPNM_SERVICE_ID_IFTRIGGERTRANSMIT, TxPduId, PduInfoPtr))
#endif
    {
        for (uint8 chIndex = 0u; chIndex < UDPNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const UdpNm_TxPduType* txPduCfgPtr = UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].TxPdu;
            if ((txPduCfgPtr != NULL_PTR) && (txPduCfgPtr->TxConfirmationPduId == TxPduId))
            {
/** SWS_UdpNm_00495 */
#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
                UdpNm_WriteUserDataBytes(chIndex);
#endif
                PduInfoPtr->SduLength = (UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].ChannelPduLength > PduInfoPtr->SduLength)
                                            ? PduInfoPtr->SduLength
                                            : UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].ChannelPduLength;
                /** SWS_UdpNm_00378 */
                SchM_Enter_UdpNm_Context();
                (void)IStdLib_MemCpy(
                    PduInfoPtr->SduDataPtr,
                    UdpNm_ChannelLCfgData[chIndex].RuntimePtr->TxPduData,
                    PduInfoPtr->SduLength);
                SchM_Exit_UdpNm_Context();

                ret = E_OK;
                break;
            }
        }
    }

    return ret;
}
#endif

/**
 * Main function of the UdpNm which processes the algorithm describes in that document.
 */
void UdpNm_MainFunction(UdpNm_ChannelIndexType chIndex)
{
    if (UdpNm_InitStatus[UdpNm_GetPartitionIndex()] == UDPNM_INIT)
    {
        UdpNm_TimerManagement(chIndex);
        UdpNm_StateManagement(chIndex);
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF && UDPNM_TX_PDU_NUMBER > 0u
        UdpNm_MsgCycleTimeHandle(chIndex);
        UdpNm_SendMsgMainHandle(chIndex);
#endif
        UdpNm_ChannelLCfgData[chIndex].RuntimePtr->RxPduExtFlg                         = FALSE;
        UdpNm_ChannelLCfgData[chIndex].RuntimePtr->TimeoutFlag[UDPNM_NM_TIMEOUT_TIMER] = FALSE;
    }
}

/* ========================================== internal function definitions ========================================= */
/**
 * @brief       Check and clear the flag.
 * @param[in]   flagPtr : Address of the flag.
 * @return      boolean
 * @retval      TRUE    : The flag was originally set to TRUE
 * @retval      FALSE   : The flag was originally set to FALSE
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71913
 */
UDPNM_LOCAL_INLINE boolean UdpNm_TestAndClear(boolean* flagPtr)
{
    boolean flag = *flagPtr;
    if (flag)
    {
        *flagPtr = FALSE;
    }
    return flag;
}

/**
 * @brief       Set the CBV bit to 1.
 * @param[in]   chIndex : Index value of the NM-channel.
 * @param[in]   mask    : The mask of the target bit.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71914
 */
UDPNM_LOCAL void UdpNm_SetTxPduCbvBit(UdpNm_ChannelIndexType chIndex, uint8 mask)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    if (chCfgPtr->PduCbvPosition != UDPNM_PDU_OFF)
    {
        SchM_Enter_UdpNm_Context();
        chCfgPtr->RuntimePtr->TxPduData[(uint8)chCfgPtr->PduCbvPosition] |= mask;
        SchM_Exit_UdpNm_Context();
    }
}

/**
 * @brief       Set the CBV bit to 0.
 * @param[in]   chIndex : Index value of the NM-channel.
 * @param[in]   mask    : The mask of the target bit.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71915
 */
UDPNM_LOCAL void UdpNm_ClrTxPduCbvBit(UdpNm_ChannelIndexType chIndex, uint8 mask)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    if (chCfgPtr->PduCbvPosition != UDPNM_PDU_OFF)
    {
        SchM_Enter_UdpNm_Context();
        uint8 antiMask = ~(mask);
        chCfgPtr->RuntimePtr->TxPduData[(uint8)chCfgPtr->PduCbvPosition] &= antiMask;
        SchM_Exit_UdpNm_Context();
    }
}

/**
 * @brief       Check the index number of the channel identifier in the UdpNm.
 * @param[in]   nmChannelHandle : Identification of the NM-channel.
 * @return      Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71916
 */
UDPNM_LOCAL UdpNm_ChannelIndexType UdpNm_FindChannelIndex(NetworkHandleType nmChannelHandle)
{
    UdpNm_ChannelIndexType index;
    for (index = 0u; index < UDPNM_NUMBER_OF_CHANNEL; index++)
    {
        if (UdpNm_ChannelLCfgData[index].ComMNetworkHandleRef == nmChannelHandle)
        {
            break;
        }
    }
    return index;
}

#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
/**
 * @brief       Determine whether have relevant PN information to this channel according CBV data and channel config
 * data.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 *                           containing the PDU, and the MetaData related to this PDU.
 * @return      boolean
 * @retval      TRUE       : The received NM-Pdu is relevant with this channel.
 * @retval      FALSE      : The received NM-Pdu is not relevant with this channel.
 * @reentrant   TRUE (but not for the same PduId)
 * @synchronous TRUE
 * @trace       CPD-71917
 */
UDPNM_LOCAL boolean UdpNm_RxPnFilter(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    boolean ret = TRUE;
    if (UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].PnEnabled)
    {
        const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
        uint8                           cbv      = pduDataPtr[(uint8)chCfgPtr->PduCbvPosition];
        if ((cbv & UDPNM_CBV_BIT_PNI_MASK) > 0u)
        {
            /** SWS_UdpNm_00486
             *   the PNI bit in the received NM-PDU is set to 1 and UdpNmSynchronizedPncShutdownEnabled is set to
             *   FALSE
             *  */

            boolean RelevantPncRequestDetected = FALSE;
            Nm_PncBitVectorRxIndication(
                chCfgPtr->ComMNetworkHandleRef,
                &pduDataPtr[NM_PN_BIT_VECTORY_OFFSET],
                &RelevantPncRequestDetected);
        }

        /** SWS_UdpNm_00329 SWS_UdpNm_487 */
        ret = (chCfgPtr->AllNmMessagesKeepAwake) ? TRUE : ret;
    }
    else
    {
        /** SWS_UdpNm_00328 */
        ret = TRUE;
    }
    return ret;
}
#endif

#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
/**
 * @brief       Detects if a node detection request has been received, and if so, processes the request.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71918
 */
UDPNM_LOCAL void UdpNm_RxNodeDetectHandle(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    uint8                           cbvDta   = pduDataPtr[(uint8)chCfgPtr->PduCbvPosition];

    /** SWS_UdpNm_00019 SWS_UdpNm_00111 */
    if ((cbvDta & UDPNM_CBV_BIT_RMP_MASK) > 0u)
    {
        chCfgPtr->RuntimePtr->RepeatMessageDetected = TRUE;
#if UDPNM_REPEAT_MSG_IND_ENABLED == STD_ON
        if (chCfgPtr->RepeatMsgIndEnabled)
        {
            Nm_RepeatMessageIndication(chCfgPtr->ComMNetworkHandleRef);
        }
#endif
    }
}
#endif

#if UDPNM_CAR_WAKEUP_RX_ENABLED == STD_ON
#if UDPNM_CAR_WAKEUP_FILTER_ENABLED == STD_ON
/**
 * @brief       Handle of Car Wakeup filter.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71919
 */
UDPNM_LOCAL boolean UdpNm_RxCarWakeUpFilter(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    boolean                         filterRet = TRUE;
    const UdpNm_ChannelLConfigType* chCfgPtr  = &UdpNm_ChannelLCfgData[chIndex];
    if (chCfgPtr->CarWakeUpFilterEnabled)
    {
        if (chCfgPtr->PduNidPosition != UDPNM_PDU_OFF)
        {
            filterRet = (chCfgPtr->CarWakeUpFilterNodeId == pduDataPtr[(uint8)chCfgPtr->PduNidPosition]);
        }
    }
    return filterRet;
}
#endif

/**
 * @brief       Handle of Car Wakeup function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71920
 */
UDPNM_LOCAL void UdpNm_RxCarWakeUpHandle(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];

    uint8 carWakeupByte = pduDataPtr[(uint8)chCfgPtr->CarWakeUpBytePosition];
    uint8 carWakeupMask = (UDPNM_SINGLEBIT01_BASENUM << (chCfgPtr->CarWakeUpBitPosition));
    if (chCfgPtr->CarWakeUpRxEnabled && ((carWakeupByte & carWakeupMask) > 0u))
    {
/** SWS_UdpNm_00376 */
#if UDPNM_CAR_WAKEUP_FILTER_ENABLED == STD_ON
        if (UdpNm_RxCarWakeUpFilter(chIndex, pduDataPtr))
#endif
        {
            /** SWS_UdpNm_00374 */
            Nm_CarWakeUpIndication(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
        }
    }
}
#endif

#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
/**
 * @brief       NM Coordinator synchronization support handle.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71921
 */
UDPNM_LOCAL void UdpNm_CoordReadyToSleepIndHandle(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

    uint8 cbvDta = pduDataPtr[(uint8)chCfgPtr->PduCbvPosition];
    if ((cbvDta & UDPNM_CBV_BIT_CSR_MASK) == 0u)
    {
        /** SWS_UdpNm_00320 */
        if ((chRTPtr->UdpnmMode == NM_MODE_NETWORK) && (chRTPtr->CoordReadyToSleepInd == UDPNM_CSR_IND_INDICATED))
        {
            chRTPtr->CoordReadyToSleepInd = UDPNM_CSR_IND_CANCELLED;
            Nm_CoordReadyToSleepCancellation(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
        }
    }
    else
    {
        /** SWS_UdpNm_00364 */
        if (((chRTPtr->UdpnmMode == NM_MODE_NETWORK) && (chRTPtr->CoordReadyToSleepInd == UDPNM_CSR_IND_INIT))
            || (chRTPtr->CoordReadyToSleepInd == UDPNM_CSR_IND_CANCELLED))
        {
            chRTPtr->CoordReadyToSleepInd = UDPNM_CSR_IND_INDICATED;
            Nm_CoordReadyToSleepIndication(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
        }
    }
}
#endif

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON) || (UDPNM_NODE_DETECTION_ENABLED == STD_ON) \
    || (UDPNM_CAR_WAKEUP_RX_ENABLED == STD_ON) || (UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
/**
 * @brief       The state maintenance timer timeout handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71922
 */
UDPNM_LOCAL void UdpNm_RxDataMainHandle(UdpNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
    /** SWS_UdpNm_00151 */
    UdpNm_RemoteSleepCancellHandle(chIndex);
#endif

#if (UDPNM_NODE_DETECTION_ENABLED == STD_ON) || (UDPNM_CAR_WAKEUP_RX_ENABLED == STD_ON) \
    || (UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
#endif

#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
    /** SWS_UdpNm_00014 */
    if (chCfgPtr->NodeDetectionEnabled)
    {
        UdpNm_RxNodeDetectHandle(chIndex, pduDataPtr);
    }
#endif

#if UDPNM_CAR_WAKEUP_RX_ENABLED == STD_ON
    if (chCfgPtr->CarWakeUpRxEnabled)
    {
        UdpNm_RxCarWakeUpHandle(chIndex, pduDataPtr);
    }
#endif

#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
    if (chCfgPtr->PduCbvPosition != UDPNM_PDU_OFF)
    {
        UdpNm_CoordReadyToSleepIndHandle(chIndex, pduDataPtr);
    }
#endif
}
#endif

/**
 * @brief       The state maintenance timer timeout handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71923
 */
UDPNM_LOCAL void UdpNm_StateHoldTimeoutHandle(UdpNm_ChannelIndexType chIndex)
{
    UdpNm_ChannelLCfgData[chIndex].RuntimePtr->TimeoutFlag[UDPNM_STATE_HOLE_TIMER] = TRUE;
}

/**
 * @brief       The NM-Timeout timer timeout handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71924
 */
UDPNM_LOCAL void UdpNm_NmTimeoutHandle(UdpNm_ChannelIndexType chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

    chRTPtr->TimeoutFlag[UDPNM_NM_TIMEOUT_TIMER] = TRUE;

    if ((chRTPtr->UdpNmState == NM_STATE_NORMAL_OPERATION) || (chRTPtr->UdpNmState == NM_STATE_REPEAT_MESSAGE))
    {
        /** SWS_UdpNm_00117 SWS_UdpNm_00101 */
        chRTPtr->TickTimers[UDPNM_NM_TIMEOUT_TIMER] = UdpNm_ChannelLCfgData[chIndex].TimeoutTime;
        /** SWS_UdpNm_00466 SWS_UdpNm_00467 */
        (void)Det_ReportRuntimeError(
            UDPNM_MODULE_ID,
            UDPNM_INSTANCE_ID,
            UDPNM_SERVICE_ID_MAINFUNCTION,
            UDPNM_E_NETWORK_TIMEOUT);
    }
}

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF && UDPNM_TX_PDU_NUMBER > 0u
/**
 * @brief       The periodically send NM-Pdu timer timeout handling function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71925
 */
UDPNM_LOCAL void UdpNm_MsgCycleTimeHandle(UdpNm_ChannelIndexType chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;
    if (chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] > 0u)
    {
        chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] -= 1u;
        if ((chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] == 0u)
            && ((chRTPtr->NetRequestStatus == UDPNM_NETWORK_REQUESTED)
                || (chRTPtr->UdpNmState == NM_STATE_REPEAT_MESSAGE)))
        {
            chRTPtr->SendNmMsgFlg = TRUE;
        }
    }
}
#endif

#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
/**
 * @brief       Remote node sleep cancell handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71926
 */
UDPNM_LOCAL void UdpNm_RemoteSleepCancellHandle(UdpNm_ChannelIndexType chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

    if (chRTPtr->TimeoutFlag[UDPNM_REMOTE_SLEEP_TIMER]
        && ((chRTPtr->UdpNmState == NM_STATE_NORMAL_OPERATION) || (chRTPtr->UdpNmState == NM_STATE_READY_SLEEP)))
    {
        chRTPtr->TimeoutFlag[UDPNM_REMOTE_SLEEP_TIMER] = FALSE;
        Nm_RemoteSleepCancellation(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
    }
}

/**
 * @brief       The remote node sleep status timer timeout handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71927
 */
UDPNM_LOCAL void UdpNm_RemoteSleepTimeoutHandle(UdpNm_ChannelIndexType chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

    /** SWS_UdpNm_00150 */
    chRTPtr->TimeoutFlag[UDPNM_REMOTE_SLEEP_TIMER] = TRUE;
    Nm_RemoteSleepIndication(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
}
#endif

/**
 * @brief       Process used timers in mainfunction.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71928
 */
UDPNM_LOCAL void UdpNm_TimerManagement(UdpNm_ChannelIndexType chIndex)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

    /**
     *  Reset timer
     *  */
    if (chRTPtr->RxPduExtFlg)
    {
#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
        chRTPtr->TickTimers[UDPNM_REMOTE_SLEEP_TIMER] = chCfgPtr->RemoteSleepIndTime;
#endif

        if (chRTPtr->UdpnmMode == NM_MODE_NETWORK)
        {
            chRTPtr->TickTimers[UDPNM_NM_TIMEOUT_TIMER] = chCfgPtr->TimeoutTime;
        }
    }

    const UdpNm_TimerTimoutHandleType UdpNm_TimerTimoutHandle[(uint8)UDPNM_MAX_TIMER_TYPE - 1u] = {
        UdpNm_StateHoldTimeoutHandle,
        UdpNm_NmTimeoutHandle,
#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
        UdpNm_RemoteSleepTimeoutHandle,
#endif
    };

    /**
     *  Timer tick
     *  */
    for (uint8 index = 0u; index < ((uint8)UDPNM_MAX_TIMER_TYPE - 1u); index += 1u)
    {
        if (chRTPtr->TickTimers[index] > 0u)
        {
            chRTPtr->TickTimers[index] -= 1u;
            if (chRTPtr->TickTimers[index] == 0u)
            {
                UdpNm_TimerTimoutHandle[index](chIndex);
            }
        }
    }
}

/**
 * @brief       Store new state and notify new state to NM module.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   nmNewState : The next state of this channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71929
 */
UDPNM_LOCAL void UdpNm_StateChange(UdpNm_ChannelIndexType chIndex, Nm_StateType nmOldState, Nm_StateType nmNewState)
{
/** SWS_UdpNm_00166 */
#if UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON
    Nm_StateChangeNotification(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef, nmOldState, nmNewState);
#else
    UDPNM_UNUSED(nmOldState);
#endif

    SchM_Enter_UdpNm_Context();
    UdpNm_ChannelLCfgData[chIndex].RuntimePtr->UdpNmState = nmNewState;
    SchM_Exit_UdpNm_Context();
}

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * @brief       When entering repeate message state, the NM-Pdu sending capability is started
 *              by setting the NM-Pdu sending timer and flag.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @param[in]   isNetWorkRequest : Whether the network is request.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71930
 */
UDPNM_LOCAL void UdpNm_StartTransmissionNmPdu(UdpNm_ChannelIndexType chIndex, boolean isNetWorkRequest)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

    if ((!isNetWorkRequest) || (chCfgPtr->ImmediateNmTransmissions == 0u))
    {
        chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].MsgCycleOffset;
        if (chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] == 0u)
        {
            chRTPtr->SendNmMsgFlg                        = TRUE;
            chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
        }

/** SWS_UdpNm_00122 */
#if UDPNM_IMMEDIATE_RESTART_ENABLED == STD_ON
        if (isNetWorkRequest && (chRTPtr->UdpnmMode == NM_MODE_PREPARE_BUS_SLEEP))
        {
            chRTPtr->SendNmMsgFlg = TRUE;
        }
#endif
    }
#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
    else
    {
        if ((chRTPtr->UdpnmMode == NM_MODE_BUS_SLEEP) || (chRTPtr->UdpnmMode == NM_MODE_PREPARE_BUS_SLEEP)
#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
            || ((chRTPtr->UdpnmMode == NM_MODE_NETWORK) && chCfgPtr->PnHandleMultipleNetworkRequests)
#endif
        )
        {
            /**
             * Set send NM message flag
             *  */
            chRTPtr->SendNmMsgFlg = TRUE;

            chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = chCfgPtr->ImmediateNmCycleTime;
            chRTPtr->ImmedMsgTxCnt                       = chCfgPtr->ImmediateNmTransmissions;
        }
    }
#endif
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    /** SWS_UdpNm_00007 */
    if ((chRTPtr->UdpnmMode == NM_MODE_BUS_SLEEP) || (chRTPtr->UdpnmMode == NM_MODE_PREPARE_BUS_SLEEP))
    {
        chRTPtr->RetrySendMsgFlag = chCfgPtr->RetryFirstMessageRequest;
    }
#endif
}
#endif

/**
 * @brief       Process when enter from other state to repeat msg state.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @param[in]   isNetWorkRequest : Whether the network is request.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71931
 */
UDPNM_LOCAL void
    UdpNm_EnterRepeatMessageState(UdpNm_ChannelIndexType chIndex, Nm_StateType nmOldState, boolean isNetWorkRequest)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

    chRTPtr->TickTimers[UDPNM_STATE_HOLE_TIMER]  = chCfgPtr->RepeatMessageTime;
    chRTPtr->TimeoutFlag[UDPNM_STATE_HOLE_TIMER] = FALSE;

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    UdpNm_StartTransmissionNmPdu(chIndex, isNetWorkRequest);
#endif

#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
    /** SWS_UdpNm_00152 */
    UdpNm_RemoteSleepCancellHandle(chIndex);
#endif

    /**
     * When the Network Mode is entered
     *  */
    if (chRTPtr->UdpnmMode != NM_MODE_NETWORK)
    {
#if UDPNM_COORDINATOR_SYNC_SUPPORT == STD_ON
        chRTPtr->CoordReadyToSleepInd = UDPNM_CSR_IND_INIT;
        UdpNm_ClrTxPduCbvBit(chIndex, UDPNM_CBV_BIT_CSR_MASK);
#endif

        /** SWS_UdpNm_00096 */
        chRTPtr->TickTimers[UDPNM_NM_TIMEOUT_TIMER] = chCfgPtr->TimeoutTime;
        /** SWS_UdpNm_00097 */
        chRTPtr->UdpnmMode = NM_MODE_NETWORK;
        Nm_NetworkMode(chCfgPtr->ComMNetworkHandleRef);
    }

    UdpNm_StateChange(chIndex, nmOldState, NM_STATE_REPEAT_MESSAGE);
}

/**
 * @brief       Processes when enter Bus-Sleep Mode from other state.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71932
 */
UDPNM_LOCAL void UdpNm_EnterPrepareBusSleepModeHandle(UdpNm_ChannelIndexType chIndex)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

    UdpNm_StateChange(chIndex, NM_STATE_READY_SLEEP, NM_STATE_PREPARE_BUS_SLEEP);
    chRTPtr->UdpnmMode = NM_MODE_PREPARE_BUS_SLEEP;

    /** SWS_UdpNm_00114 */
    Nm_PrepareBusSleepMode(chCfgPtr->ComMNetworkHandleRef);

    /** SWS_UdpNm_00088 */
    if (!chCfgPtr->StayInPbsEnabled)
    {
        chRTPtr->TickTimers[UDPNM_STATE_HOLE_TIMER] = chCfgPtr->WaitBusSleepTime;
    }

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    /** SWS_UdpNm_00367 */
    if (chCfgPtr->ActiveWakeupBitEnabled)
    {
        UdpNm_ClrTxPduCbvBit(chIndex, UDPNM_CBV_BIT_AW_MASK);
    }
#endif
}

#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
/**
 * @brief       According to the current state and active or passive node detect flag to state change.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71933
 */
UDPNM_LOCAL boolean UdpNm_NodeDetectStateHandle(UdpNm_ChannelIndexType chIndex)
{
    boolean                         stateChanged = FALSE;
    const UdpNm_ChannelLConfigType* chCfgPtr     = &UdpNm_ChannelLCfgData[chIndex];

    UdpNm_InnerChannelType* chRTPtr   = chCfgPtr->RuntimePtr;
    Nm_StateType            currState = chRTPtr->UdpNmState;
    if (currState == NM_STATE_REPEAT_MESSAGE)
    {
        UdpNm_ClrTxPduCbvBit(chIndex, UDPNM_CBV_BIT_RMP_MASK);
    }
    if ((chRTPtr->RepeatMessageRequest || chRTPtr->RepeatMessageDetected)
        && ((currState == NM_STATE_NORMAL_OPERATION) || (currState == NM_STATE_READY_SLEEP)))
    {
        if (chRTPtr->RepeatMessageRequest)
        {
            UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_RMP_MASK);
        }
        stateChanged = TRUE;

        chRTPtr->RepeatMessageRequest  = FALSE;
        chRTPtr->RepeatMessageDetected = FALSE;
    }

    return stateChanged;
}
#endif

/**
 * @brief       Processing network requests for network mode.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71934
 */
UDPNM_LOCAL boolean UdpNm_NetworkStateHandle(UdpNm_ChannelIndexType chIndex)
{
    const UdpNm_ChannelLConfigType* chCfgPtr     = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr      = chCfgPtr->RuntimePtr;
    boolean                         stateChanged = FALSE;
    Nm_StateType                    currState    = chRTPtr->UdpNmState;

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    if (UdpNm_TestAndClear(&chRTPtr->NetRequestFlg))
    {
#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
        /** SWS_UdpNm_00362 */
        if (chCfgPtr->PnHandleMultipleNetworkRequests)
        {
            UdpNm_EnterRepeatMessageState(chIndex, currState, TRUE);
            stateChanged = TRUE;
        }
#endif
    }
#endif

#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
    if (!stateChanged)
#endif
    {
        if ((chRTPtr->NetRequestStatus == UDPNM_NETWORK_REQUESTED) && (currState != NM_STATE_NORMAL_OPERATION))
        {
            UdpNm_StateChange(chIndex, currState, NM_STATE_NORMAL_OPERATION);
#if UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
            /** SWS_UdpNm_00150 */
            chRTPtr->TickTimers[UDPNM_REMOTE_SLEEP_TIMER] = chCfgPtr->RemoteSleepIndTime;
#endif
            stateChanged = TRUE;
        }

        if ((chRTPtr->NetRequestStatus == UDPNM_NETWORK_RELEASED) && (currState != NM_STATE_READY_SLEEP))
        {
            UdpNm_StateChange(chIndex, currState, NM_STATE_READY_SLEEP);
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
            chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = 0u;
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->RetrySendMsgFlag = FALSE;
#endif
#endif
            stateChanged = TRUE;
        }
    }

    return stateChanged;
}

/**
 * @brief       Handle state transition in Repeat Message State.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71935
 */
UDPNM_LOCAL void UdpNm_RepeatMessageStateHandle(UdpNm_ChannelIndexType chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

    /** SWS_UdpNm_00102 */
    if (UdpNm_TestAndClear(&chRTPtr->TimeoutFlag[UDPNM_STATE_HOLE_TIMER]))
    {
#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
        chRTPtr->ImmedMsgTxCnt = 0u;
#endif

/** SWS_UdpNm_00107 */
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
        if (UdpNm_ChannelLCfgData[chIndex].NodeDetectionEnabled)
        {
            (void)UdpNm_NodeDetectStateHandle(chIndex);
        }
#endif

        boolean stateChanged = UdpNm_NetworkStateHandle(chIndex);
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
        if (stateChanged)
        {
            chRTPtr->RepeatMessageRequest  = FALSE;
            chRTPtr->RepeatMessageDetected = FALSE;
        }
#else
        (void)stateChanged;
#endif
    }
}

/**
 * @brief       Handle state transition in Normal Operation State.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71936
 */
UDPNM_LOCAL void UdpNm_NormalOperationStateHandle(UdpNm_ChannelIndexType chIndex)
{
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    boolean stateChanged = FALSE;

    stateChanged = UdpNm_NetworkStateHandle(chIndex);

#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
    if (!stateChanged && UdpNm_ChannelLCfgData[chIndex].NodeDetectionEnabled)
    {
        if (UdpNm_NodeDetectStateHandle(chIndex))
        {
            UdpNm_EnterRepeatMessageState(chIndex, UdpNm_ChannelLCfgData[chIndex].RuntimePtr->UdpNmState, FALSE);
        }
    }
#endif
    UDPNM_UNUSED(stateChanged);
#endif
}

/**
 * @brief       Handle state transition in Ready Sleep State.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71937
 */
UDPNM_LOCAL void UdpNm_ReadySleepStateHandle(UdpNm_ChannelIndexType chIndex)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    if (chRTPtr->NetRequestFlg
#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
        && !chCfgPtr->PnHandleMultipleNetworkRequests
#endif
#if UDPNM_COM_CONTROL_ENABLED == STD_ON
        && chRTPtr->UdpNmTxEnable
#endif
    )
    {
        /** SWS_UdpNm_00116 */
        chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
        /** SWS_UdpNm_00006 */
        chRTPtr->SendNmMsgFlg = TRUE;
    }
#endif

    if (!UdpNm_NetworkStateHandle(chIndex))
    {
        SchM_Enter_UdpNm_Context();
#if UDPNM_NODE_DETECTION_ENABLED == STD_ON
        if (chCfgPtr->NodeDetectionEnabled && UdpNm_NodeDetectStateHandle(chIndex))
        {
            chRTPtr->UdpNmState = NM_STATE_REPEAT_MESSAGE;
        }
#endif

        /**
         *  Depends on the Network Management PDU transmission ability is enabled,
         *  because NM-Timeout will stop if function UdpNm_DisableCommunication has bean called.
         *  */
        if ((chRTPtr->UdpNmState == NM_STATE_READY_SLEEP)
            && (UdpNm_TestAndClear(&chRTPtr->TimeoutFlag[UDPNM_NM_TIMEOUT_TIMER])
                || (chRTPtr->TickTimers[UDPNM_NM_TIMEOUT_TIMER] == 0u)))
        {
            chRTPtr->UdpNmState = NM_STATE_PREPARE_BUS_SLEEP;
        }
        SchM_Exit_UdpNm_Context();

        switch (chRTPtr->UdpNmState)
        {
        case NM_STATE_REPEAT_MESSAGE:
            UdpNm_EnterRepeatMessageState(chIndex, NM_STATE_READY_SLEEP, FALSE);
            break;

        case NM_STATE_PREPARE_BUS_SLEEP:
            UdpNm_EnterPrepareBusSleepModeHandle(chIndex);
            break;

        default:
            /*nothing*/
            break;
        }
    }
}

/**
 * @brief       Network wake up signal processing for Non-network mode.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71938
 */
UDPNM_LOCAL boolean UdpNm_WakeUpSignalHandle(UdpNm_ChannelIndexType chIndex)
{
    boolean                 stateChanged = FALSE;
    UdpNm_InnerChannelType* chRTPtr      = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

    boolean rxWakeup = (chRTPtr->RxPduExtFlg && (chRTPtr->UdpNmState == NM_STATE_PREPARE_BUS_SLEEP)) ? TRUE : FALSE;

    boolean isNetWorkRequest = FALSE;
    boolean isPassiveStartUp = UdpNm_TestAndClear(&chRTPtr->PassiveStartUp) || rxWakeup;
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
    isNetWorkRequest = UdpNm_TestAndClear(&chRTPtr->NetRequestFlg);
#endif

    /** SWS_UdpNm_00128 SWS_UdpNm_00129 SWS_UdpNm_00123 SWS_UdpNm_00124
     *  1. if UdpNm_PassiveStartUp is called in the Bus-Sleep Mode or Prepare Bus-Sleep Mode
     *  2. if the network is requested in Bus-Sleep Mode or Prepare Bus-Sleep Mode
     *  3. At successful reception of a Network Management PDU in the Prepare Bus-Sleep Mode
     *  */
    if (isNetWorkRequest || isPassiveStartUp)
    {
        UdpNm_EnterRepeatMessageState(chIndex, chRTPtr->UdpNmState, isNetWorkRequest);

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
        /**
         *  Only if network request and not passive startup.
         *  */
        if (UdpNm_ChannelLCfgData[chIndex].ActiveWakeupBitEnabled && (isNetWorkRequest && !isPassiveStartUp))
        {
            /** SWS_UdpNm_00366 */
            UdpNm_SetTxPduCbvBit(chIndex, UDPNM_CBV_BIT_AW_MASK);
        }
#endif
        stateChanged = TRUE;
    }

    return stateChanged;
}

/**
 * @brief       Handle state transition in Prepare Bus Sleep State.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71939
 */
UDPNM_LOCAL void UdpNm_PrepareBusSleepStateHandle(UdpNm_ChannelIndexType chIndex)
{
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

    boolean stateChanged = UdpNm_WakeUpSignalHandle(chIndex);

    if (!stateChanged && UdpNm_TestAndClear(&chRTPtr->TimeoutFlag[UDPNM_STATE_HOLE_TIMER]))
    {
        UdpNm_StateChange(chIndex, chRTPtr->UdpNmState, NM_STATE_BUS_SLEEP);
        chRTPtr->UdpnmMode = NM_MODE_BUS_SLEEP;

        /** SWS_UdpNm_00126 */
        Nm_BusSleepMode(chCfgPtr->ComMNetworkHandleRef);
    }
}

/**
 * @brief       Handle state transition in Bus Sleep State.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71940
 */
UDPNM_LOCAL void UdpNm_BusSleepStateHandle(UdpNm_ChannelIndexType chIndex)
{
    const UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;
    SchM_Enter_UdpNm_Context();
    if (!chRTPtr->PassiveStartUp
#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF
        && !chRTPtr->NetRequestFlg
#endif
        && chRTPtr->RxPduExtFlg)
    {
        /** SWS_UdpNm_00127 */
        /** To prevent duplicate reporting of Nm_NetworkStartIndication, this function shall be executed exclusively
         * with Nm_PassiveStartUp */
        Nm_NetworkStartIndication(UdpNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
    }
    SchM_Exit_UdpNm_Context();

    (void)UdpNm_WakeUpSignalHandle(chIndex);
}

/**
 * @brief       Handle state transition of UdpNm.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71941
 */
UDPNM_LOCAL void UdpNm_StateManagement(UdpNm_ChannelIndexType chIndex)
{
    const UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;
    switch (chRTPtr->UdpNmState)
    {
    case NM_STATE_BUS_SLEEP:
        UdpNm_BusSleepStateHandle(chIndex);
        break;

    case NM_STATE_PREPARE_BUS_SLEEP:
        UdpNm_PrepareBusSleepStateHandle(chIndex);
        break;

    case NM_STATE_REPEAT_MESSAGE:
        UdpNm_RepeatMessageStateHandle(chIndex);
        break;

    case NM_STATE_NORMAL_OPERATION:
        UdpNm_NormalOperationStateHandle(chIndex);
        break;

    case NM_STATE_READY_SLEEP:
        UdpNm_ReadySleepStateHandle(chIndex);
        break;

    default:
        /**
         *  nothing
         *  */
        break;
    }
}

#if UDPNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * @brief       Get the UserData and fill it into the UserData field of Tx buffer.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71942
 */
UDPNM_LOCAL void UdpNm_WriteUserDataBytes(UdpNm_ChannelIndexType chIndex)
{
    const UdpNm_UserDataTxPduType* userDataPduPtr = UdpNm_CfgPtr->ChlPBCfgPtr[chIndex].UserDataTxPdu;
    if (userDataPduPtr != NULL_PTR)
    {
        PduInfoType pduInfo                              = {0};
        uint8       userData[UDPNM_USER_DATA_MAX_LENGTH] = {0};
        PduIdType   pdurTxSduId                          = userDataPduPtr->UpperLayerPduId;

        const UdpNm_ChannelPBConfigType* chPBCfgPtr = &UdpNm_CfgPtr->ChlPBCfgPtr[chIndex];
        pduInfo.SduLength                           = chPBCfgPtr->UserDataLength;
        pduInfo.SduDataPtr                          = userData;
        pduInfo.MetaDataPtr                         = NULL_PTR;

        if (PduR_UdpNmTriggerTransmit(pdurTxSduId, &pduInfo) == E_OK)
        {
            SchM_Enter_UdpNm_Context();
            (void)IStdLib_MemCpy(
                &(UdpNm_ChannelLCfgData[chIndex].RuntimePtr->TxPduData[chPBCfgPtr->UserDataOffset]),
                userData,
                chPBCfgPtr->UserDataLength);
            SchM_Exit_UdpNm_Context();
        }
    }
}
#endif

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF && UDPNM_TX_PDU_NUMBER > 0u
/**
 * @brief       Assemble NM packets and send them.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71943
 */
UDPNM_LOCAL Std_ReturnType UdpNm_SendNmPdu(UdpNm_ChannelIndexType chIndex)
{
    Std_ReturnType                  ret      = E_NOT_OK;
    const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
    UdpNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePtr;

#if UDPNM_COM_CONTROL_ENABLED == STD_ON
    if (!chRTPtr->UdpNmTxEnable)
    {
        ret = E_NOT_OK;
    }
    else
#endif
    {
        const UdpNm_ChannelPBConfigType* chPBCfgPtr = &UdpNm_CfgPtr->ChlPBCfgPtr[chIndex];
#if UDPNM_GLOBAL_PN_ENABLED == STD_ON
        if (chPBCfgPtr->PnEnabled)
        {
            /** SWS_UdpNm_00500 SWS_UdpNm_00503 */
            Nm_PncBitVectorTxIndication(chCfgPtr->ComMNetworkHandleRef, &chRTPtr->TxPduData[NM_PN_BIT_VECTORY_OFFSET]);
        }
#endif

        if (chPBCfgPtr->TxPdu != NULL_PTR)
        {
/** SWS_UdpNm_00317 */
#if (UDPNM_COM_USERDATA_SUPPORT == STD_ON) && (UDPNM_IS_TRIGGER_TRANSMIT_IN_SOAD == STD_OFF)
            UdpNm_WriteUserDataBytes(chIndex);
#endif
            PduInfoType pduInfo       = {0};
            PduIdType   soAdIfTxSduId = chPBCfgPtr->TxPdu->LowerLayerPduId;
            pduInfo.SduDataPtr        = chRTPtr->TxPduData;
            pduInfo.SduLength         = chPBCfgPtr->ChannelPduLength;

            /** SWS_UdpNm_00497 */
            ret = SoAd_IfTransmit(soAdIfTxSduId, &pduInfo);
        }
    }
    return ret;
}
#endif

#if UDPNM_PASSIVE_MODE_ENABLED == STD_OFF && UDPNM_TX_PDU_NUMBER > 0u
/**
 * @brief       Send message in MainFunction.The purpose of this function is to aggregate multiple send requests,Only
 *              one frame will be sent out when there are multiple send requests in a MainFunction.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71944
 */
UDPNM_LOCAL void UdpNm_SendMsgMainHandle(UdpNm_ChannelIndexType chIndex)
{
    UdpNm_InnerChannelType* chRTPtr = UdpNm_ChannelLCfgData[chIndex].RuntimePtr;

    if (chRTPtr->SendNmMsgFlg)
    {
        chRTPtr->SendNmMsgFlg                    = FALSE;
        const UdpNm_ChannelLConfigType* chCfgPtr = &UdpNm_ChannelLCfgData[chIndex];
        if (UdpNm_SendNmPdu(chIndex) == E_OK)
        {
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->RetrySendMsgFlag = FALSE;
#endif
#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
            if (chRTPtr->ImmedMsgTxCnt > 0u)
            {
                chRTPtr->ImmedMsgTxCnt--;
                if (chRTPtr->ImmedMsgTxCnt == 0u)
                {
                    chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
                }
                else
                {
                    chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = chCfgPtr->ImmediateNmCycleTime;
                }
            }
            else
#endif
            {
                chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
            }
        }
        else
        {
#if UDPNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
            if (chRTPtr->ImmedMsgTxCnt > 0u)
            {
                chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = 1u;
            }
            else
#endif
            {
#if UDPNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
                if (chRTPtr->RetrySendMsgFlag)
                {
                    /** SWS_UdpNm_00330 */
                    chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = 1u;
                }
                else
#endif
                {
                    chRTPtr->TickTimers[UDPNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
                }
            }
        }
    }
}
#endif

/**
 * @brief       Returns the index of current partition in the ComM configuration.
 * @return      The index of current partition in the ComM configuration.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71945
 */
UDPNM_LOCAL uint16 UdpNm_GetPartitionIndex(void)
{
    uint16 index = 0u;
#if UDPNM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType applicationID = GetApplicationID();
    for (; index < UDPNM_NUMBER_OF_PARTITION; ++index)
    {
        if (UdpNm_OsApplicationList[index] == applicationID)
        {
            break;
        }
    }
#endif
    return index;
}

#if UDPNM_DEV_ERROR_DETECT == STD_ON
/**
 * @brief       Development error validation of invalid partition context.
 * @param[in]   apiId : ID of API service in which error is detected
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71946
 */
UDPNM_LOCAL boolean UdpNm_ValidatePartitionContext(uint8 apiId)
{
    boolean ret = TRUE;
    if (UdpNm_GetPartitionIndex() >= UDPNM_NUMBER_OF_PARTITION)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_PARTITION);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief       Development error validation of uninit.
 * @param[in]   apiId : ID of API service in which error is detected
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71947
 */
UDPNM_LOCAL boolean UdpNm_ValidateInitStatus(uint8 apiId)
{
    boolean              ret     = TRUE;
    uint8                errorId = UDPNM_E_INVALID_ID;
    UdpNm_InitStatusType status  = UdpNm_InitStatus[UdpNm_GetPartitionIndex()];

    if ((apiId == UDPNM_SERVICE_ID_INIT) && (status == UDPNM_INIT))
    {
        errorId = UDPNM_E_INIT_FAILED;
    }

    if ((apiId != UDPNM_SERVICE_ID_INIT) && (status == UDPNM_UNINIT))
    {
        errorId = UDPNM_E_UNINIT;
    }

    if (errorId != UDPNM_E_INVALID_ID)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, errorId);
        ret = FALSE;
    }

    return ret;
}

/**
 * @brief       Development error validation of networkhandle.
 * @param[in]   apiId : ID of API service in which error is detected
 * @param[in]   nmChannelHandle : The number of the NM-channel to be detected
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71948
 */
UDPNM_LOCAL boolean UdpNm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType nmChannelHandle)
{
    boolean                ret     = TRUE;
    uint8                  errorId = UDPNM_E_INVALID_ID;
    UdpNm_ChannelIndexType chIndex = UdpNm_FindChannelIndex(nmChannelHandle);

    if (chIndex >= UDPNM_NUMBER_OF_CHANNEL)
    {
        errorId = UDPNM_E_INVALID_CHANNEL;
    }

#if UDPNM_MULTIPLE_PARTITION_USED == STD_ON
    else if (UdpNm_ChannelLCfgData[chIndex].ApplicationID != GetApplicationID())
    {
        errorId = UDPNM_E_PARTITION;
    }
#endif

    if (errorId != UDPNM_E_INVALID_ID)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, errorId);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief       Development error validation of null pointer.
 * @param[in]   apiId   : ID of API service in which error is detected
 * @param[in]   pointer : The pointer to be detected
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71949
 */
UDPNM_LOCAL boolean UdpNm_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean ret = TRUE;
    if (pointer == NULL_PTR)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_PARAM_POINTER);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief       Development error validation of tx PduId.
 * @param[in]   apiId : ID of API service in which error is detected
 * @param[in]   txPduId : ID of the PDU that has been transmitted.
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71950
 */
UDPNM_LOCAL boolean UdpNm_ValidateTxPduId(uint8 apiId, PduIdType txPduId)
{
    boolean ret = TRUE;
    if (txPduId >= UDPNM_TX_PDU_NUMBER)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_INVALID_PDUID);
        ret = FALSE;
    }
    return ret;
}

#if UDPNM_TRIGGER_TRANSMIT_API == STD_ON
/**
 * @brief       Development error validation of Tx-Pdu.
 * @param[in]   apiId : ID of API service in which error is detected
 * @param[in]   txPduId : ID of the SDU that is requested to be transmitted
 * @param[in]   pduInfoPtr : Contains a pointer to a buffer about pduInfo
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71951
 */
UDPNM_LOCAL boolean UdpNm_ValidateTxPdu(uint8 apiId, PduIdType txPduId, const PduInfoType* pduInfoPtr)
{
    return UdpNm_ValidateTxPduId(apiId, txPduId) && UdpNm_ValidatePointer(apiId, pduInfoPtr)
           && UdpNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
}
#endif

/**
 * @brief       Development error validation of Rx-Pdu.
 * @param[in]   apiId : ID of API service in which error is detected
 * @param[in]   rxPduId : ID of the received PDU.
 * @param[in]   pduInfoPtr : Contains a pointer to a buffer about pduInfo
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71952
 */
UDPNM_LOCAL boolean UdpNm_ValidateRxPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr)
{
    boolean ret = TRUE;
    if (rxPduId >= UDPNM_RX_PDU_NUMBER)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_INVALID_PDUID);
        ret = FALSE;
    }

    ret = ret && UdpNm_ValidatePointer(apiId, pduInfoPtr) && UdpNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
    return ret;
}

/**
 * @brief       Development error validation of UserDataTxPdu.
 * @param[in]   apiId : ID of API service in which error is detected
 * @param[in]   rxPduId : Identifier of the PDU to be transmitted
 * @param[in]   pduInfoPtr : Contains a pointer to a buffer about pduInfo
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-71953
 */
UDPNM_LOCAL boolean UdpNm_ValidateUserDataPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr)
{
    boolean ret = TRUE;
    if (rxPduId >= UDPNM_USERDATA_TX_PDU_NUM)
    {
        (void)Det_ReportError(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, apiId, UDPNM_E_INVALID_PDUID);
        ret = FALSE;
    }

    ret = ret && UdpNm_ValidatePointer(apiId, pduInfoPtr) && UdpNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
    return ret;
}

#endif

#define UDPNM_STOP_SEC_CODE
#include "UdpNm_MemMap.h"
/* PRQA S 1503, 1532 -- */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
