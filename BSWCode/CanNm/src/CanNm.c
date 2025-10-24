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
 ************************************************************************************************************************
 **
 **  @file               : CanNm.c
 **  @author             : caihong.liu
 **  @date               : 2024/12/12
 **  @vendor             : isoft
 **  @description        : Can network manager
 **
 ***********************************************************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "CanNm.h"
#include "CanNm_Internal.h"
#include "IStdLib.h"
#include "Det.h"
#include "Nm.h"
#include "SchM_CanNm.h"
#include "CanIf.h"
#include "CanSM_CanIf.h"

#if CANNM_COM_USERDATA_SUPPORT == STD_ON
#include "PduR_CanNm.h"
#endif

#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
#include "ComM.h"
#endif

/* ===================================================== macros ===================================================== */
#define CANNM_UNUSED(v) (void)(v)

/* ================================================ type definitions ================================================ */
typedef enum
{
    CANNM_UNINIT,
    CANNM_INIT,
} CanNm_InitStatusType;

/* ========================================== internal function declarations ======================================== */
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

CANNM_LOCAL_INLINE boolean         CanNm_TestAndClear(boolean* flagPtr);
CANNM_LOCAL void                   CanNm_SetTxPduCbvBit(CanNm_ChannelIndexType chIndex, uint8 mask, boolean flag);
CANNM_LOCAL CanNm_ChannelIndexType CanNm_FindChannelIndex(NetworkHandleType nmChannelHandle);

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
CANNM_LOCAL void CanNm_InnerTxConfHandle(CanNm_ChannelIndexType chIndex, Std_ReturnType result);
#endif

/**
 *  Function used to process the received data.
 *  */
#if CANNM_NODE_DETECTION_ENABLED == STD_ON
CANNM_LOCAL void CanNm_RxNodeDetectHandle(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

#if CANNM_CAR_WAKEUP_RX_ENABLED == STD_ON
#if CANNM_CAR_WAKEUP_FILTER_ENABLED == STD_ON
CANNM_LOCAL boolean CanNm_RxCarWakeUpFilter(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif
CANNM_LOCAL void CanNm_RxCarWakeUpHandle(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
CANNM_LOCAL void CanNm_CoordReadyToSleepIndHandle(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
CANNM_LOCAL boolean CanNm_RxPnFilter(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON) || (CANNM_NODE_DETECTION_ENABLED == STD_ON) \
    || (CANNM_CAR_WAKEUP_RX_ENABLED == STD_ON) || (CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
CANNM_LOCAL void CanNm_RxDataMainHandle(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr);
#endif

/**
 *  Functions for all timers, including timing, reload, and timeout handling.
 *  */
CANNM_LOCAL void CanNm_StateHoldTimeoutHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void CanNm_NmTimeoutHandle(CanNm_ChannelIndexType chIndex);
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
CANNM_LOCAL void CanNm_MsgCycleTimeHandle(CanNm_ChannelIndexType chIndex);
#endif
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
CANNM_LOCAL void CanNm_MsgTxTimeoutHandle(CanNm_ChannelIndexType chIndex);
#endif
#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
CANNM_LOCAL void CanNm_RemoteSleepCancellHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void CanNm_RemoteSleepTimeoutHandle(CanNm_ChannelIndexType chIndex);
#endif
CANNM_LOCAL void CanNm_TimerManagement(CanNm_ChannelIndexType chIndex);

/**
 *  Function to handle state machine switching.
 *  */
CANNM_LOCAL void CanNm_StateChange(CanNm_ChannelIndexType chIndex, Nm_StateType nmNewState);
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
CANNM_LOCAL void CanNm_StartTransmissionNmPdu(CanNm_ChannelIndexType chIndex, boolean isNetWorkRequest);
#endif
CANNM_LOCAL void CanNm_EnterRepeatMessageState(CanNm_ChannelIndexType chIndex, boolean isNetWorkRequest);
CANNM_LOCAL void CanNm_EnterPrepareBusSleepModeHandle(uint8 chIndex);

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
CANNM_LOCAL boolean CanNm_NodeDetectStateHandle(CanNm_ChannelIndexType chIndex);
#endif
CANNM_LOCAL boolean CanNm_NetworkStateHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void    CanNm_RepeatMessageStateHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void    CanNm_NormalOperationStateHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void    CanNm_ReadySleepStateHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL boolean CanNm_WakeUpSignalHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void    CanNm_PrepareBusSleepStateHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void    CanNm_BusSleepStateHandle(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void    CanNm_StateManagement(CanNm_ChannelIndexType chIndex);

/**
 *  Function for sending periodic packets.
 *  */
#if CANNM_COM_USERDATA_SUPPORT == STD_ON
CANNM_LOCAL void CanNm_WriteUserDataBytes(CanNm_ChannelIndexType chIndex);
#endif
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
CANNM_LOCAL Std_ReturnType CanNm_SendNmPdu(CanNm_ChannelIndexType chIndex);
CANNM_LOCAL void           CanNm_SendMsgMainHandle(CanNm_ChannelIndexType chIndex);
#endif

CANNM_LOCAL uint16 CanNm_GetPartitionIndex(void);
#if CANNM_DEV_ERROR_DETECT == STD_ON
CANNM_LOCAL boolean CanNm_ValidatePartitionContext(uint8 apiId);
CANNM_LOCAL boolean CanNm_ValidateInitStatus(uint8 apiId);
CANNM_LOCAL boolean CanNm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType nmChannelHandle);
CANNM_LOCAL boolean CanNm_ValidatePointer(uint8 apiId, const void* pointer);
CANNM_LOCAL boolean CanNm_ValidateTxPduId(uint8 apiId, PduIdType txPduId);
CANNM_LOCAL boolean CanNm_ValidateTxPdu(uint8 apiId, PduIdType txPduId, const PduInfoType* pduInfoPtr);
CANNM_LOCAL boolean CanNm_ValidateRxPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr);
CANNM_LOCAL boolean CanNm_ValidateUserDataPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr);
CANNM_LOCAL boolean CanNm_ValidateDeInit(void);
#endif

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define CANNM_START_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"
/**
 * @brief The initialization state of the CanNm module.
 */
CANNM_LOCAL CanNm_InitStatusType CanNm_InitStatus[CANNM_NUMBER_OF_PARTITION]; /**< @range NA */
#define CANNM_STOP_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_VAR_CLEARED_PTR
#include "CanNm_MemMap.h"
/**
 * @brief Point to the configuration of the CanNm module, only available after initialization.
 */
CANNM_LOCAL const CanNm_ChannelPBConfigType* CanNm_ChlPBCfgPtr; /**< @range NA */
#define CANNM_STOP_SEC_VAR_CLEARED_PTR
#include "CanNm_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
/**
 * Initialize the CanNm module.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void CanNm_Init(const CanNm_ConfigType* CanNmConfigPtr)
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_INIT) && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_INIT)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_INIT, CanNmConfigPtr))
#endif
    {
        CanNm_ChlPBCfgPtr = CanNmConfigPtr->ChlPBCfgPtr;
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
        ApplicationType applicationID = GetApplicationID();
#endif

        for (CanNm_ChannelIndexType chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
            if (chCfgPtr->ApplicationID == applicationID)
#endif
            {
                CanNm_InnerChannelType* chRTPtr = chCfgPtr->RuntimePrt;

                /** SWS_CanNm_00141 */
                chRTPtr->CanNmState = NM_STATE_BUS_SLEEP;

                /** SWS_CanNm_00144 */
                chRTPtr->CanNmMode = NM_MODE_BUS_SLEEP;

                /** SWS_CanNm_00143 */
                chRTPtr->NetRequestStatus = CANNM_NETWORK_RELEASED;

                /** Passive startup flag */
                chRTPtr->PassiveStartUp = FALSE;

                for (uint8 loop = 0u; loop < (uint8)CANNM_MAX_TIMER_TYPE; loop++)
                {
                    chRTPtr->TickTimers[loop]  = 0u;
                    chRTPtr->TimeoutFlag[loop] = FALSE;
                }

                /** SWS_CanNm_00500 */
                for (uint8 loop = 0u; loop < CANNM_PDU_MAX_LENGTH; loop++)
                {
#if (CANNM_USER_DATA_ENABLED == STD_ON) || (CANNM_NODE_ID_ENABLED == STD_ON) || (CANNM_NODE_DETECTION_ENABLED == STD_ON)
                    chRTPtr->RxPduData[loop] = 0u;
#endif
                    chRTPtr->TxPduData[loop] = 0u;
                }

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
                chRTPtr->SendNmMsgFlg  = FALSE;
                chRTPtr->NetRequestFlg = FALSE;
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
                chRTPtr->RetrySendMsgFlag = FALSE;
#endif
#endif

#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
                chRTPtr->ImmedMsgTxCnt = 0u;
#endif

#if CANNM_COM_CONTROL_ENABLED == STD_ON
                chRTPtr->CanNmTxEnable = TRUE;
#endif

#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
                chRTPtr->CoordReadyToSleepInd = CANNM_CSR_IND_INIT;
#endif
#if CANNM_NODE_DETECTION_ENABLED == STD_ON
                chRTPtr->RepeatMessageRequest  = FALSE;
                chRTPtr->RepeatMessageDetected = FALSE;
#endif

                const CanNm_ChannelPBConfigType* chPBCfgPtr = &CanNm_ChlPBCfgPtr[chIndex];
#if CANNM_NODE_ID_ENABLED == STD_ON
                /**
                 *  Set NodeId to config data
                 *  */
                if (chCfgPtr->PduNidPosition != CANNM_PDU_OFF)
                {
                    chRTPtr->TxPduData[chCfgPtr->PduNidPosition] = chPBCfgPtr->NodeId;
                }
#endif

                /**
                 *  Set CBV to 0x00
                 *  */
                if (chCfgPtr->PduCbvPosition != CANNM_PDU_OFF)
                {
                    chRTPtr->TxPduData[(uint8)chCfgPtr->PduCbvPosition] = 0u;
                }

#if CANNM_USER_DATA_ENABLED == STD_ON
                /** SWS_CanNm_00025 */
                for (uint8 loop = 0u; loop < chPBCfgPtr->UserDataLength; loop++)
                {
                    chRTPtr->TxPduData[loop + chPBCfgPtr->UserDataOffset] = CANNM_USER_DATA_INITIAL_BYTE;
                }
#endif

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
                /** SWS_CanNm_00413
                 *  The usage of the CBV is mandatory in case Partial Networking is used.
                 *  */
                if (chPBCfgPtr->PnEnabled)
                {
                    CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_PNI_MASK, TRUE);
                    chRTPtr->PnFilterEnable = FALSE;
                }
#endif
            }
        }
        CanNm_InitStatus[CanNm_GetPartitionIndex()] = CANNM_INIT;
    }
}

/**
 * De-initializes the CanNm module.
 */
void CanNm_DeInit(void)
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_DEINIT) && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_DEINIT)
        && CanNm_ValidateDeInit())
#endif
    {
        CanNm_InitStatus[CanNm_GetPartitionIndex()] = CANNM_UNINIT;
    }
}
/* PRQA S 1503 -- */
/**
 * Passive startup of the AUTOSAR CanNm.
 */
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_PASSIVESTARUP)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_PASSIVESTARUP)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_PASSIVESTARUP, nmChannelHandle))
#endif
    {
        CanNm_InnerChannelType* chRTPtr =
            CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)].RuntimePrt;

        SchM_Enter_CanNm_Context();
        /** SWS_CanNm_00147 */
        if (chRTPtr->CanNmMode != NM_MODE_NETWORK)
        {
            chRTPtr->PassiveStartUp = TRUE;
            ret                     = E_OK;
        }
        SchM_Exit_CanNm_Context();
    }
    return ret;
}

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * Request the network, since ECU needs to communicate on the bus.
 */
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_NETWORKREQUEST)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_NETWORKREQUEST)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_NETWORKREQUEST, nmChannelHandle))
#endif
    {
        CanNm_InnerChannelType* chRTPtr =
            CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)].RuntimePrt;

        /** SWS_CanNm_00104 */
        SchM_Enter_CanNm_Context();
        chRTPtr->NetRequestStatus = CANNM_NETWORK_REQUESTED;
        chRTPtr->NetRequestFlg    = TRUE;
#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
        chRTPtr->CoordReadyToSleepInd = CANNM_CSR_IND_INIT;
#endif
        SchM_Exit_CanNm_Context();
        ret = E_OK;
    }

    return ret;
}
#endif

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * Release the network, since ECU doesn't have to communicate on the bus.
 */
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;

#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_NETWORKRELEASE)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_NETWORKRELEASE)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_NETWORKRELEASE, nmChannelHandle))
#endif
    {
        CanNm_InnerChannelType* chRTPtr =
            CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)].RuntimePrt;

        /** SWS_CanNm_00105 */
        SchM_Enter_CanNm_Context();
        chRTPtr->NetRequestStatus = CANNM_NETWORK_RELEASED;
        SchM_Exit_CanNm_Context();
        ret = E_OK;
    }

    return ret;
}
#endif

#if CANNM_COM_CONTROL_ENABLED == STD_ON
/**
 * Disable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service.
 */
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
/** SWS_UdpNm_00307 */
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_DISABLECOMMUNICATION)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_DISABLECOMMUNICATION)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_DISABLECOMMUNICATION, nmChannelHandle))
#endif
    {
        CanNm_InnerChannelType* chRTPtr =
            CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)].RuntimePrt;

        /** SWS_CanNm_00172 */
        SchM_Enter_CanNm_Context();
        if (chRTPtr->CanNmMode == NM_MODE_NETWORK)
        {
            /** SWS_CanNm_00170 */
            chRTPtr->CanNmTxEnable = FALSE;

            /** SWS_CanNm_00173 */
            chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = 0u;

            /** SWS_CanNm_00174 */
            chRTPtr->TickTimers[CANNM_NM_TIMEOUT_TIMER] = 0u;

#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
            /** SWS_CanNm_00175 */
            chRTPtr->TickTimers[CANNM_REMOTE_SLEEP_TIMER] = 0u;
#endif
            ret = E_OK;
        }
        SchM_Exit_CanNm_Context();
    }
#else
    CANNM_UNUSED(nmChannelHandle);
#endif

    return ret;
}
#endif

#if CANNM_COM_CONTROL_ENABLED == STD_ON
/**
 * Enable the NM PDU transmission ability due to a ISO14229 Communication Control (28hex) service.
 */
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
/** SWS_UdpNm_00306 */
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_ENABLECOMMUNICATION)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_ENABLECOMMUNICATION)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_ENABLECOMMUNICATION, nmChannelHandle))
#endif
    {
        const CanNm_ChannelLConfigType* chCfgPtr =
            &CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)];
        CanNm_InnerChannelType* chRTPtr = chCfgPtr->RuntimePrt;

        SchM_Enter_CanNm_Context();
        /** SWS_CanNm_00177 SWS_CanNm_00295 */
        if (!chRTPtr->CanNmTxEnable && (chRTPtr->CanNmMode == NM_MODE_NETWORK))
        {
            chRTPtr->CanNmTxEnable = TRUE;
            /** SWS_CanNm_00178
             *   Start transmission of NM PDUs within the next NM main function.
             *  */
            chRTPtr->SendNmMsgFlg                        = TRUE;
            chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;

            /** SWS_CanNm_00179 */
            chRTPtr->TickTimers[CANNM_NM_TIMEOUT_TIMER] = chCfgPtr->TimeoutTime;

/** SWS_CanNm_00180 */
#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
            chRTPtr->TickTimers[CANNM_REMOTE_SLEEP_TIMER] = chCfgPtr->RemoteSleepIndTime;
#endif
            ret = E_OK;
        }
        SchM_Exit_CanNm_Context();
    }
#else
    CANNM_UNUSED(nmChannelHandle);
#endif

    return ret;
}
#endif

#if (CANNM_USER_DATA_ENABLED == STD_ON) && (CANNM_COM_USERDATA_SUPPORT == STD_OFF) \
    && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * Set user data for NM PDUs transmitted next on the bus.
 */
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const uint8* nmUserDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_SETUSERDATA)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_SETUSERDATA)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_SETUSERDATA, nmChannelHandle)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_SETUSERDATA, nmUserDataPtr))
#endif
    {
        const CanNm_ChannelIndexType     chIndex    = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelPBConfigType* chPBCfgPtr = &CanNm_ChlPBCfgPtr[chIndex];
        const CanNm_InnerChannelType*    chRTPtr    = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

        /** SWS_CanNm_00159 */
        uint8  userDataLength = chPBCfgPtr->UserDataLength;
        uint8  userDataOffset = chPBCfgPtr->UserDataOffset;
        uint8* userDataPtr    = &chRTPtr->TxPduData[userDataOffset];

        SchM_Enter_CanNm_Context();
        (void)IStdLib_MemCpy(userDataPtr, nmUserDataPtr, userDataLength);
        SchM_Exit_CanNm_Context();

        ret = E_OK;
    }

    return ret;
}
#endif

#if CANNM_USER_DATA_ENABLED == STD_ON
/**
 * Get user data out of the most recently received NM PDU.
 */
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, uint8* nmUserDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_GETUSERDATA)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_GETUSERDATA)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_GETUSERDATA, nmChannelHandle)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_GETUSERDATA, nmUserDataPtr))
#endif
    {
        const CanNm_ChannelIndexType     chIndex    = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelPBConfigType* chPBCfgPtr = &CanNm_ChlPBCfgPtr[chIndex];
        const CanNm_InnerChannelType*    chRTPtr    = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

        /** SWS_CanNm_00160 */
        uint8        userDataLength = chPBCfgPtr->UserDataLength;
        uint8        userDataOffset = chPBCfgPtr->UserDataOffset;
        const uint8* userDataPtr    = &chRTPtr->RxPduData[userDataOffset];

        SchM_Enter_CanNm_Context();
        (void)IStdLib_MemCpy(nmUserDataPtr, userDataPtr, userDataLength);
        SchM_Exit_CanNm_Context();

        ret = E_OK;
    }
    return ret;
}
#endif

#if CANNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * Requests transmission of a PDU.
 */
Std_ReturnType CanNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_TRANSMIT) && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_TRANSMIT)
        && CanNm_ValidateUserDataPdu(CANNM_SERVICE_ID_TRANSMIT, TxPduId, PduInfoPtr))
#endif
    {
        for (uint8 chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const CanNm_UserDataTxPduType* userDataPduPtr = CanNm_ChlPBCfgPtr[chIndex].UserDataTxPdu;
            if ((userDataPduPtr != NULL_PTR) && (userDataPduPtr->TxUserDataPduId == TxPduId))
            {
                const CanNm_ChannelPBConfigType* chPBCfgPtr = &CanNm_ChlPBCfgPtr[chIndex];
                CanNm_InnerChannelType*          chRTPtr    = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

                /** SWS_CanNm_00333 */
                if (((chRTPtr->CanNmState == NM_STATE_NORMAL_OPERATION)
                     || (chRTPtr->CanNmState == NM_STATE_REPEAT_MESSAGE))
                    && (PduInfoPtr->SduLength >= chPBCfgPtr->UserDataLength))
                {
                    chRTPtr->SendNmMsgFlg = TRUE;
                    ret                   = E_OK;
                }
                break;
            }
        }
    }
    CANNM_UNUSED(PduInfoPtr);
    return ret;
}
#endif

/**
 * Get node identifier out of the most recently received NM PDU.
 */
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_NODE_ID_ENABLED == STD_ON
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_GETNODEIDENTIFIER)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_GETNODEIDENTIFIER)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_GETNODEIDENTIFIER, nmChannelHandle)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_GETNODEIDENTIFIER, nmNodeIdPtr))
#endif
    {
        const CanNm_ChannelLConfigType* chCfgPtr =
            &CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)];

        /** SWS_CanNm_00132 */
        if (chCfgPtr->NodeIdEnabled)
        {
            const CanNm_InnerChannelType* chRTPtr = chCfgPtr->RuntimePrt;

            SchM_Enter_CanNm_Context();
            *nmNodeIdPtr = chRTPtr->RxPduData[(uint8)chCfgPtr->PduNidPosition];
            SchM_Exit_CanNm_Context();

            ret = E_OK;
        }
    }
#else
    CANNM_UNUSED(nmChannelHandle);
    CANNM_UNUSED(nmNodeIdPtr);
#endif
    return ret;
}

/**
 * Get node identifier configured for the local node.
 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, uint8* nmNodeIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_NODE_ID_ENABLED == STD_ON
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_GETLOCALNODEIDENTIFIER)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_GETLOCALNODEIDENTIFIER)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_GETLOCALNODEIDENTIFIER, nmChannelHandle)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_GETLOCALNODEIDENTIFIER, nmNodeIdPtr))
#endif
    {
        const CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);

        /** SWS_CanNm_00133 */
        if (CanNm_ChannelLCfgData[chIndex].NodeIdEnabled)
        {
            *nmNodeIdPtr = CanNm_ChlPBCfgPtr[chIndex].NodeId;
            ret          = E_OK;
        }
    }
#else
    CANNM_UNUSED(nmChannelHandle);
    CANNM_UNUSED(nmNodeIdPtr);
#endif
    return ret;
}

/**
 * Set Repeat Message Request Bit for NM PDUs transmitted next on the bus.
 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_NODE_DETECTION_ENABLED == STD_ON
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_REPEATMESSAGEREQUEST)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_REPEATMESSAGEREQUEST)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_REPEATMESSAGEREQUEST, nmChannelHandle))
#endif
    {
        const CanNm_ChannelLConfigType* chCfgPtr =
            &CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)];

        if (chCfgPtr->NodeDetectionEnabled)
        {
            CanNm_InnerChannelType* chRTPtr = chCfgPtr->RuntimePrt;

            SchM_Enter_CanNm_Context();
            /** SWS_CanNm_00137 */
            if ((chRTPtr->CanNmState == NM_STATE_NORMAL_OPERATION) || (chRTPtr->CanNmState == NM_STATE_READY_SLEEP))
            {
                chRTPtr->RepeatMessageRequest = TRUE;
                ret                           = E_OK;
            }
            SchM_Exit_CanNm_Context();
        }
    }
#else
    CANNM_UNUSED(nmChannelHandle);
#endif
    return ret;
}

/**
 * Get the whole PDU data out of the most recently received NM PDU.
 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduDataPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (CANNM_USER_DATA_ENABLED == STD_ON) || (CANNM_NODE_ID_ENABLED == STD_ON) || (CANNM_NODE_DETECTION_ENABLED == STD_ON)
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_GETPDUDATA)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_GETPDUDATA)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_GETPDUDATA, nmChannelHandle)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_GETPDUDATA, nmPduDataPtr))
#endif
    {
        const CanNm_ChannelIndexType    chIndex  = CanNm_FindChannelIndex(nmChannelHandle);
        const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];

/** SWS_CanNm_00138 SWS_CanNm_00407 */
#if CANNM_USER_DATA_ENABLED == STD_OFF
        if (FALSE
#if CANNM_NODE_ID_ENABLED == STD_ON
            || chCfgPtr->NodeIdEnabled
#endif
#if CANNM_NODE_DETECTION_ENABLED == STD_ON
            || chCfgPtr->NodeDetectionEnabled
#endif
        )
#endif
        {
            SchM_Enter_CanNm_Context();
            (void)IStdLib_MemCpy(
                nmPduDataPtr,
                chCfgPtr->RuntimePrt->RxPduData,
                CanNm_ChlPBCfgPtr[chIndex].ChannelPduLength);
            SchM_Exit_CanNm_Context();

            ret = E_OK;
        }
    }
#else
    CANNM_UNUSED(nmChannelHandle);
    CANNM_UNUSED(nmPduDataPtr);
#endif
    return ret;
}

/**
 * Returns the state and the mode of the network management.
 */
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_GETSTATE) && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_GETSTATE)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_GETSTATE, nmChannelHandle)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_GETSTATE, nmStatePtr)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_GETSTATE, nmModePtr))
#endif
    {
        const CanNm_InnerChannelType* chRTPtr =
            CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)].RuntimePrt;

        SchM_Enter_CanNm_Context();
        *nmStatePtr = chRTPtr->CanNmState;
        *nmModePtr  = chRTPtr->CanNmMode;
        SchM_Exit_CanNm_Context();
        ret = E_OK;
    }

    return ret;
}

#if CANNM_VERSION_INFO_API == STD_ON
/**
 * This service returns the version information of this module.
 */
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePointer(CANNM_SERVICE_ID_GETVERSIONINFO, versioninfo))
#endif
    {
        versioninfo->vendorID         = CANNM_VENDOR_ID;
        versioninfo->moduleID         = CANNM_MODULE_ID;
        versioninfo->sw_major_version = CANNM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CANNM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CANNM_SW_PATCH_VERSION;
    }
}
#endif

#if (CANNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * Request bus synchronization.
 */
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_REQUESTBUSSYNCHRONIZATION, nmChannelHandle))
#endif
    {
        CanNm_InnerChannelType* chRTPtr =
            CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)].RuntimePrt;

        SchM_Enter_CanNm_Context();
        /** SWS_CanNm_00187 SWS_CanNm_00181 */
        if ((chRTPtr->CanNmMode == NM_MODE_NETWORK)
#if CANNM_COM_CONTROL_ENABLED == STD_ON
            && chRTPtr->CanNmTxEnable
#endif
        )
        {
            /** SWS_CanNm_00130 */
            chRTPtr->SendNmMsgFlg = TRUE;
            ret                   = E_OK;
        }
        SchM_Exit_CanNm_Context();
    }

    return ret;
}
#endif

#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
/**
 * Check if remote sleep indication takes place or not.
 */
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, boolean* nmRemoteSleepIndPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION, nmChannelHandle)
        && CanNm_ValidatePointer(CANNM_SERVICE_ID_CHECKREMOTESLEEPINDICATION, nmRemoteSleepIndPtr))
#endif
    {
        const CanNm_InnerChannelType* chRTPtr =
            CanNm_ChannelLCfgData[(CanNm_ChannelIndexType)CanNm_FindChannelIndex(nmChannelHandle)].RuntimePrt;

        SchM_Enter_CanNm_Context();
        /** SWS_CanNm_00154 */
        if ((chRTPtr->CanNmState == NM_STATE_NORMAL_OPERATION) || (chRTPtr->CanNmState == NM_STATE_READY_SLEEP))
        {
            *nmRemoteSleepIndPtr = chRTPtr->TimeoutFlag[CANNM_REMOTE_SLEEP_TIMER];
            ret                  = E_OK;
        }
        SchM_Exit_CanNm_Context();
    }

    return ret;
}
#endif

#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
/**
 * Set the NM Coordinator Sleep Ready bit in the Control Bit Vector.
 */
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, boolean nmSleepReadyBit)
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_SETSLEEPREADYBIT)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_SETSLEEPREADYBIT)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_SETSLEEPREADYBIT, nmChannelHandle))
#endif
    {
        const CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        /** SWS_CanNm_00342 */
        if (nmSleepReadyBit)
        {
            CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_CSR_MASK, TRUE);
        }
        else
        {
            CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_CSR_MASK, FALSE);
        }
        CanNm_ChannelLCfgData[chIndex].RuntimePrt->SendNmMsgFlg = TRUE;
        ret                                                     = E_OK;
    }

    return ret;
}
#endif

#if (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/**
 * The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.
 */
void CanNm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_TXCONFIRMATION)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_TXCONFIRMATION)
        && CanNm_ValidateTxPduId(CANNM_SERVICE_ID_TXCONFIRMATION, TxPduId))
#endif
    {
        for (CanNm_ChannelIndexType chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const CanNm_TxPduType* txPduPtr = CanNm_ChlPBCfgPtr[chIndex].TxPdu;
            if ((txPduPtr != NULL_PTR) && (txPduPtr->TxConfirmationPduId == TxPduId))
            {
                CanNm_InnerTxConfHandle(chIndex, result);
                break;
            }
        }
    }
}
#endif

/**
 * Indication of a received PDU from a lower layer communication interface module.
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 2889 -- */
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_RXINDICATION)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_RXINDICATION)
        && CanNm_ValidateRxPdu(CANNM_SERVICE_ID_RXINDICATION, RxPduId, PduInfoPtr))
#endif
    {
        for (CanNm_ChannelIndexType chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const CanNm_ChannelPBConfigType* chPBCfgPtr = &CanNm_ChlPBCfgPtr[chIndex];
            for (PduIdType pduIdx = 0u; pduIdx < chPBCfgPtr->RxPduNum; pduIdx++)
            {
                if (chPBCfgPtr->RxPdu[pduIdx].RxPduId == RxPduId)
                {
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
                    if (CanNm_RxPnFilter(chIndex, PduInfoPtr->SduDataPtr))
#endif
                    {
                        CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;
#if (CANNM_USER_DATA_ENABLED == STD_ON) || (CANNM_NODE_ID_ENABLED == STD_ON) || (CANNM_NODE_DETECTION_ENABLED == STD_ON)
                        SchM_Enter_CanNm_Context();
                        /** SWS_CanNm_00035 */
                        uint8 pduLen = chPBCfgPtr->ChannelPduLength;
                        (void)IStdLib_MemCpy(
                            chRTPtr->RxPduData,
                            PduInfoPtr->SduDataPtr,
                            ((PduInfoPtr->SduLength > pduLen) ? pduLen : PduInfoPtr->SduLength));
                        SchM_Exit_CanNm_Context();
#endif
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON) || (CANNM_NODE_DETECTION_ENABLED == STD_ON) \
    || (CANNM_CAR_WAKEUP_RX_ENABLED == STD_ON) || (CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
                        CanNm_RxDataMainHandle(chIndex, PduInfoPtr->SduDataPtr);
#endif
                        chRTPtr->RxPduExtFlg = TRUE;
/** SWS_CanNm_00037 */
#if CANNM_PDU_RX_INDICATION_ENABLED == STD_ON
                        Nm_PduRxIndication(CanNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
#endif
                    }
                    return;
                }
            }
        }
    }
}

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
/**
 * Enables the PN filter functionality on the indicated NM channel.
 */
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle)
{
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_CONFIRMPNAVAILABILITY)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_CONFIRMPNAVAILABILITY)
        && CanNm_ValidateNetworkHandle(CANNM_SERVICE_ID_CONFIRMPNAVAILABILITY, nmChannelHandle))
#endif
    {
        const CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
        CanNm_InnerChannelType*      chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

        if (CanNm_ChlPBCfgPtr[chIndex].PnEnabled)
        {
            /** Record CanNm_ConfirmPnAvailability has been called */
            chRTPtr->PnFilterEnable = TRUE;
        }
    }
}
#endif

/**
 * Within this API, the upper layer module (called module) shall check whether the available data
 * fits into the buffer size reported by PduInfoPtr->SduLength. If it fits, it shall copy its data into the
 * buffer provided by PduInfoPtr->SduDataPtr and update the length of the actual copied data in
 * PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType CanNm_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
/* PRQA S 1503 -- */
{
    Std_ReturnType ret = E_NOT_OK;
#if CANNM_DEV_ERROR_DETECT == STD_ON
    if (CanNm_ValidatePartitionContext(CANNM_SERVICE_ID_TRIGGERTRANSMIT)
        && CanNm_ValidateInitStatus(CANNM_SERVICE_ID_TRIGGERTRANSMIT)
        && CanNm_ValidateTxPdu(CANNM_SERVICE_ID_TRIGGERTRANSMIT, TxPduId, PduInfoPtr))
#endif
    {
        for (CanNm_ChannelIndexType chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++)
        {
            const CanNm_ChannelPBConfigType* chPBCfgPtr  = &CanNm_ChlPBCfgPtr[chIndex];
            const CanNm_TxPduType*           txPduCfgPtr = chPBCfgPtr->TxPdu;
            if ((txPduCfgPtr != NULL_PTR) && (txPduCfgPtr->TxConfirmationPduId == TxPduId))
            {
/** SWS_CanNm_00510 */
#if CANNM_COM_USERDATA_SUPPORT == STD_ON
                CanNm_WriteUserDataBytes(chIndex);
#endif
                uint8 pduLen          = chPBCfgPtr->ChannelPduLength;
                PduInfoPtr->SduLength = (pduLen > PduInfoPtr->SduLength) ? PduInfoPtr->SduLength : pduLen;
                /** SWS_CanNm_00351 */
                SchM_Enter_CanNm_Context();
                (void)IStdLib_MemCpy(
                    PduInfoPtr->SduDataPtr,
                    CanNm_ChannelLCfgData[chIndex].RuntimePrt->TxPduData,
                    PduInfoPtr->SduLength);
                SchM_Exit_CanNm_Context();

                ret = E_OK;
                break;
            }
        }
    }

    return ret;
}

/**
 * Main function of the CanNm which processes the algorithm describes in that document.
 */
void CanNm_MainFunction(CanNm_ChannelIndexType chIndex)
{
    if (CanNm_InitStatus[CanNm_GetPartitionIndex()] == CANNM_INIT)
    {
        CanNm_TimerManagement(chIndex);
        CanNm_StateManagement(chIndex);
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
        CanNm_MsgCycleTimeHandle(chIndex);
        CanNm_SendMsgMainHandle(chIndex);
#endif
        CanNm_InnerChannelType* chRTPtr              = CanNm_ChannelLCfgData[chIndex].RuntimePrt;
        chRTPtr->RxPduExtFlg                         = FALSE;
        chRTPtr->TimeoutFlag[CANNM_NM_TIMEOUT_TIMER] = FALSE;
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
 * @trace       CPD-70076
 */
CANNM_LOCAL_INLINE boolean CanNm_TestAndClear(boolean* flagPtr)
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
 * @trace       CPD-70077
 */
CANNM_LOCAL void CanNm_SetTxPduCbvBit(CanNm_ChannelIndexType chIndex, uint8 mask, boolean flag)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    const CanNm_PduPositionType     cbvPos   = chCfgPtr->PduCbvPosition;
    if (cbvPos != CANNM_PDU_OFF)
    {
        SchM_Enter_CanNm_Context();
        uint8* cbvPtr = &chCfgPtr->RuntimePrt->TxPduData[(uint8)cbvPos];
        if (flag)
        {
            *cbvPtr |= mask;
        }
        else
        {
            uint8 antiMask = ~(mask);
            *cbvPtr &= antiMask;
        }
        SchM_Exit_CanNm_Context();
    }
}

/**
 * @brief       Check the index number of the channel identifier in the CanNm.
 * @param[in]   nmChannelHandle : Identification of the NM-channel.
 * @return      Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70079
 */
CANNM_LOCAL CanNm_ChannelIndexType CanNm_FindChannelIndex(NetworkHandleType nmChannelHandle)
{
    CanNm_ChannelIndexType index;
    for (index = 0u; index < CANNM_NUMBER_OF_CHANNEL; index++)
    {
        if (CanNm_ChannelLCfgData[index].ComMNetworkHandleRef == nmChannelHandle)
        {
            break;
        }
    }
    return index;
}

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
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
 * @trace       CPD-70080
 */
CANNM_LOCAL boolean CanNm_RxPnFilter(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    boolean ret = FALSE;
    if (CanNm_ChlPBCfgPtr[chIndex].PnEnabled)
    {
        const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
        uint8                           cbv      = pduDataPtr[(uint8)chCfgPtr->PduCbvPosition];
        if ((cbv & CANNM_CBV_BIT_PNI_MASK) > 0u)
        {
            /** SWS_CanNm_00502
             *   the PNI bit in the received NM-PDU is set to 1 and  CanNmSynchronizedPncShutdownEnabled is set to
             *   FALSE
             *  */

            boolean RelevantPncRequestDetected = FALSE;
            Nm_PncBitVectorRxIndication(
                chCfgPtr->ComMNetworkHandleRef,
                &pduDataPtr[NM_PN_BIT_VECTORY_OFFSET],
                &RelevantPncRequestDetected);

            /** SWS_CanNm_00503
             *  Only set to FALSE only under CanNm_ConfirmPnAvailability has not been called and
             * RelevantPncRequestDetected is set FALSE conditions.
             *  */
            ret = (chCfgPtr->RuntimePrt->PnFilterEnable && !RelevantPncRequestDetected) ? FALSE : TRUE;
        }

        /** SWS_CanNm_00410 SWS_CanNm_00503 */
        ret = (chCfgPtr->AllNmMessagesKeepAwake) ? TRUE : ret;
    }
    else
    {
        /** SWS_CanNm_00409 */
        ret = TRUE;
    }
    return ret;
}
#endif

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * @brief       Process the NM-Pdu sending result
 * @param[in]   chIndex   : Index value of the NM-channel.
 * @param[in]   result    : E_OK: The PDU was transmitted
 *                          E_NOT_OK: Transmission of the PDU failed.
 * @reentrant   TRUE (but not for the same PduId)
 * @synchronous TRUE
 * @trace       CPD-70081
 */
CANNM_LOCAL void CanNm_InnerTxConfHandle(CanNm_ChannelIndexType chIndex, Std_ReturnType result)
{
    if (result == E_OK)
    {
        CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;
        SchM_Enter_CanNm_Context();
        /** SWS_CanNm_00099 */
        if (chRTPtr->CanNmMode == NM_MODE_NETWORK)
        {
            chRTPtr->TickTimers[CANNM_NM_TIMEOUT_TIMER] = CanNm_ChannelLCfgData[chIndex].TimeoutTime;
        }

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
        /** SWS_CanNm_00065 */
        chRTPtr->TickTimers[CANNM_TXMSG_TIMEOUT_TIMER]  = 0u;
        chRTPtr->TimeoutFlag[CANNM_TXMSG_TIMEOUT_TIMER] = FALSE;
#endif
        SchM_Exit_CanNm_Context();
    }

#if (CANNM_COM_USERDATA_SUPPORT == STD_ON) && (CANNM_USERDATA_TX_PDU_NUM > 0)
    const CanNm_UserDataTxPduType* userDataPduPtr = CanNm_ChlPBCfgPtr[chIndex].UserDataTxPdu;
    if (userDataPduPtr != NULL_PTR)
    {
        /** SWS_CanNm_00329 */
        PduR_CanNmTxConfirmation(userDataPduPtr->UpperLayerPduId, result);
    }
#endif
}
#endif

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
/**
 * @brief       Detects if a node detection request has been received, and if so, processes the request.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70082
 */
CANNM_LOCAL void CanNm_RxNodeDetectHandle(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    uint8                           cbvDta   = pduDataPtr[(uint8)chCfgPtr->PduCbvPosition];

    /** SWS_CanNm_00119 SWS_CanNm_00111 */
    if ((cbvDta & CANNM_CBV_BIT_RMP_MASK) > 0u)
    {
        chCfgPtr->RuntimePrt->RepeatMessageDetected = TRUE;
#if CANNM_REPEAT_MSG_IND_ENABLED == STD_ON
        if (chCfgPtr->RepeatMsgIndEnabled)
        {
            Nm_RepeatMessageIndication(chCfgPtr->ComMNetworkHandleRef);
        }
#endif
    }
}
#endif

#if CANNM_CAR_WAKEUP_RX_ENABLED == STD_ON
#if CANNM_CAR_WAKEUP_FILTER_ENABLED == STD_ON
/**
 * @brief       Handle of Car Wakeup filter.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70083
 */
CANNM_LOCAL boolean CanNm_RxCarWakeUpFilter(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    boolean                         filterRet = TRUE;
    const CanNm_ChannelLConfigType* chCfgPtr  = &CanNm_ChannelLCfgData[chIndex];
    if (chCfgPtr->CarWakeUpFilterEnabled)
    {
        if (chCfgPtr->PduNidPosition != CANNM_PDU_OFF)
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
 * @trace       CPD-70084
 */
CANNM_LOCAL void CanNm_RxCarWakeUpHandle(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];

    uint8 carWakeupByte = pduDataPtr[(uint8)chCfgPtr->CarWakeUpBytePosition];
    uint8 carWakeupMask = (CANNM_SINGLEBIT01_BASENUM << (chCfgPtr->CarWakeUpBitPosition));
    if (chCfgPtr->CarWakeUpRxEnabled && ((carWakeupByte & carWakeupMask) > 0u))
    {
/** SWS_CanNm_00408 */
#if CANNM_CAR_WAKEUP_FILTER_ENABLED == STD_ON
        if (CanNm_RxCarWakeUpFilter(chIndex, pduDataPtr))
#endif
        {
            /** SWS_CanNm_00406 */
            Nm_CarWakeUpIndication(CanNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
        }
    }
}
#endif

#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
/**
 * @brief       NM Coordinator synchronization support handle.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70085
 */
CANNM_LOCAL void CanNm_CoordReadyToSleepIndHandle(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePrt;

    uint8 cbvDta = pduDataPtr[(uint8)chCfgPtr->PduCbvPosition];
    if ((cbvDta & CANNM_CBV_BIT_CSR_MASK) == 0u)
    {
        /** SWS_CanNm_00348 */
        if ((chRTPtr->CanNmMode == NM_MODE_NETWORK) && (chRTPtr->CoordReadyToSleepInd == CANNM_CSR_IND_INDICATED))
        {
            chRTPtr->CoordReadyToSleepInd = CANNM_CSR_IND_CANCELLED;
            Nm_CoordReadyToSleepCancellation(CanNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
        }
    }
    else
    {
        /** SWS_CanNm_00341 */
        if (((chRTPtr->CanNmMode == NM_MODE_NETWORK) && (chRTPtr->CoordReadyToSleepInd == CANNM_CSR_IND_INIT))
            || (chRTPtr->CoordReadyToSleepInd == CANNM_CSR_IND_CANCELLED))
        {
            chRTPtr->CoordReadyToSleepInd = CANNM_CSR_IND_INDICATED;
            Nm_CoordReadyToSleepIndication(CanNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
        }
    }
}
#endif

#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON) || (CANNM_NODE_DETECTION_ENABLED == STD_ON) \
    || (CANNM_CAR_WAKEUP_RX_ENABLED == STD_ON) || (CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
/**
 * @brief       Synchronous process received data in MainFunction.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @param[in]   pduDataPtr : The address of SduDataPtr in received PDU.
 * @reentrant   TRUE (but not for the same PduId)
 * @synchronous TRUE
 * @trace       CPD-70086
 */
CANNM_LOCAL void CanNm_RxDataMainHandle(CanNm_ChannelIndexType chIndex, const uint8* pduDataPtr)
{
#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
    /** SWS_CanNm_00151 */
    CanNm_RemoteSleepCancellHandle(chIndex);
#endif

#if (CANNM_NODE_DETECTION_ENABLED == STD_ON) || (CANNM_CAR_WAKEUP_RX_ENABLED == STD_ON) \
    || (CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
#endif

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
    /** SWS_CanNm_00014 */
    if (chCfgPtr->NodeDetectionEnabled)
    {
        CanNm_RxNodeDetectHandle(chIndex, pduDataPtr);
    }
#endif

#if CANNM_CAR_WAKEUP_RX_ENABLED == STD_ON
    if (chCfgPtr->CarWakeUpRxEnabled)
    {
        CanNm_RxCarWakeUpHandle(chIndex, pduDataPtr);
    }
#endif

#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
    if (chCfgPtr->PduCbvPosition != CANNM_PDU_OFF)
    {
        CanNm_CoordReadyToSleepIndHandle(chIndex, pduDataPtr);
    }
#endif
}
#endif

/**
 * @brief       The state maintenance timer timeout handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70087
 */
CANNM_LOCAL void CanNm_StateHoldTimeoutHandle(CanNm_ChannelIndexType chIndex)
{
    CanNm_ChannelLCfgData[chIndex].RuntimePrt->TimeoutFlag[CANNM_STATE_HOLE_TIMER] = TRUE;
}

/**
 * @brief       The NM-Timeout timer timeout handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70088
 */
CANNM_LOCAL void CanNm_NmTimeoutHandle(CanNm_ChannelIndexType chIndex)
{
    CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

    chRTPtr->TimeoutFlag[CANNM_NM_TIMEOUT_TIMER] = TRUE;

    if ((chRTPtr->CanNmState == NM_STATE_NORMAL_OPERATION) || (chRTPtr->CanNmState == NM_STATE_REPEAT_MESSAGE))
    {
        /** SWS_CanNm_00117 SWS_CanNm_00101 */
        chRTPtr->TickTimers[CANNM_NM_TIMEOUT_TIMER] = CanNm_ChannelLCfgData[chIndex].TimeoutTime;
        /** SWS_CanNm_00194 SWS_CanNm_00193 */
        (void)Det_ReportRuntimeError(
            CANNM_MODULE_ID,
            CANNM_INSTANCE_ID,
            CANNM_SERVICE_ID_MAINFUNCTION,
            CANNM_E_NETWORK_TIMEOUT);
    }
}

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * @brief       The periodically send NM-Pdu timer timeout handling function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70089
 */
CANNM_LOCAL void CanNm_MsgCycleTimeHandle(CanNm_ChannelIndexType chIndex)
{
    CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;
    if (chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] > 0u)
    {
        chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] -= 1u;
        if ((chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] == 0u)
            && ((chRTPtr->NetRequestStatus == CANNM_NETWORK_REQUESTED)
                || (chRTPtr->CanNmState == NM_STATE_REPEAT_MESSAGE)))
        {
            chRTPtr->SendNmMsgFlg = TRUE;
        }
    }
}
#endif

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
/**
 * @brief       The NM-Pdu transmision timer timeout handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70090
 */
CANNM_LOCAL void CanNm_MsgTxTimeoutHandle(CanNm_ChannelIndexType chIndex)
{
/** SWS_CanNm_00073
 *   If CanNmPassiveModeEnabled is set to TRUE or CanNmImmediateTxconfEnabled is set to TRUE CanNm shall not perform
 *   transmission error handling.
 *  */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF)
    const CanNm_ChannelLConfigType* chCfgPtr                     = &CanNm_ChannelLCfgData[chIndex];
    chCfgPtr->RuntimePrt->TimeoutFlag[CANNM_TXMSG_TIMEOUT_TIMER] = TRUE;
    if (CanNm_ChlPBCfgPtr[chIndex].PnEnabled)
    {
        /** SWS_CanNm_00066 */
        CanSM_TxTimeoutException(chCfgPtr->ComMNetworkHandleRef);
        /** SWS_CanNm_00446 */
        Nm_TxTimeoutException(chCfgPtr->ComMNetworkHandleRef);
    }
#endif
}
#endif

#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
/**
 * @brief       Remote node sleep cancell handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70091
 */
CANNM_LOCAL void CanNm_RemoteSleepCancellHandle(CanNm_ChannelIndexType chIndex)
{
    CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

    if (chRTPtr->TimeoutFlag[CANNM_REMOTE_SLEEP_TIMER]
        && ((chRTPtr->CanNmState == NM_STATE_NORMAL_OPERATION) || (chRTPtr->CanNmState == NM_STATE_READY_SLEEP)))
    {
        chRTPtr->TimeoutFlag[CANNM_REMOTE_SLEEP_TIMER] = FALSE;
        Nm_RemoteSleepCancellation(CanNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
    }
}

/**
 * @brief       The remote node sleep status timer timeout handler function.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70092
 */
CANNM_LOCAL void CanNm_RemoteSleepTimeoutHandle(CanNm_ChannelIndexType chIndex)
{
    CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

    /** SWS_CanNm_00150 */
    chRTPtr->TimeoutFlag[CANNM_REMOTE_SLEEP_TIMER] = TRUE;
    Nm_RemoteSleepIndication(CanNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef);
}
#endif

/**
 * @brief       Process used timers in mainfunction.
 * @param[in]   chIndex    : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70093
 */
CANNM_LOCAL void CanNm_TimerManagement(CanNm_ChannelIndexType chIndex)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePrt;

    /**
     *  Reset timer
     *  */
    if (chRTPtr->RxPduExtFlg)
    {
#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
        chRTPtr->TickTimers[CANNM_REMOTE_SLEEP_TIMER] = chCfgPtr->RemoteSleepIndTime;
#endif

        /** SWS_CanNm_00098 */
        if (chRTPtr->CanNmMode == NM_MODE_NETWORK)
        {
            chRTPtr->TickTimers[CANNM_NM_TIMEOUT_TIMER] = chCfgPtr->TimeoutTime;
        }
    }

    const CanNm_TimerTimoutHandleType CanNm_TimerTimoutHandle[(uint8)CANNM_MAX_TIMER_TYPE - 1u] = {
        CanNm_StateHoldTimeoutHandle,
        CanNm_NmTimeoutHandle,
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
        CanNm_MsgTxTimeoutHandle,
#endif
#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
        CanNm_RemoteSleepTimeoutHandle,
#endif
    };

    /**
     *  Timer tick
     *  */
    for (uint8 index = 0u; index < ((uint8)CANNM_MAX_TIMER_TYPE - 1u); index += 1u)
    {
        if (chRTPtr->TickTimers[index] > 0u)
        {
            chRTPtr->TickTimers[index] -= 1u;
            if (chRTPtr->TickTimers[index] == 0u)
            {
                CanNm_TimerTimoutHandle[index](chIndex);
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
 * @trace       CPD-70094
 */
CANNM_LOCAL void CanNm_StateChange(CanNm_ChannelIndexType chIndex, Nm_StateType nmNewState)
{
    CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

/** SWS_CanNm_00166 */
#if CANNM_STATE_CHANGE_IND_ENABLED == STD_ON
    Nm_StateChangeNotification(CanNm_ChannelLCfgData[chIndex].ComMNetworkHandleRef, chRTPtr->CanNmState, nmNewState);
#endif

    SchM_Enter_CanNm_Context();
    chRTPtr->CanNmState = nmNewState;
    SchM_Exit_CanNm_Context();
}

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * @brief       When entering repeate message state, the NM-Pdu sending capability is started
 *              by setting the NM-Pdu sending timer and flag.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @param[in]   isNetWorkRequest : Whether the network is request.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70095
 */
CANNM_LOCAL void CanNm_StartTransmissionNmPdu(CanNm_ChannelIndexType chIndex, boolean isNetWorkRequest)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePrt;

    if ((!isNetWorkRequest) || (chCfgPtr->ImmediateNmTransmissions == 0u))
    {
        chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = CanNm_ChlPBCfgPtr[chIndex].MsgCycleOffset;
        if (chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] == 0u)
        {
            chRTPtr->SendNmMsgFlg                        = TRUE;
            chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
        }

/** SWS_CanNm_00122 */
#if CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON
        if (isNetWorkRequest && (chRTPtr->CanNmMode == NM_MODE_PREPARE_BUS_SLEEP))
        {
            chRTPtr->SendNmMsgFlg = TRUE;
        }
#endif
    }
#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
    else
    {
        /** SWS_CanNm_00334 SWS_CanNm_00454
         *  When entering the Repeat Message State and CanNmImmediateNmTransmissions is greater zero:
         *  1. from Bus Sleep Mode or Prepare Bus Sleep Mode because of CanNm_NetworkRequest(), or
         *  2. from Network Mode because of CanNm_NetworkRequest() and CanNmPnHandleMultipleNetworkRequests is set TRUE
         *     and PDU transmission ability is enabled.
         *  */
        if ((chRTPtr->CanNmMode == NM_MODE_BUS_SLEEP) || (chRTPtr->CanNmMode == NM_MODE_PREPARE_BUS_SLEEP)
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
            || ((chRTPtr->CanNmMode == NM_MODE_NETWORK) && chCfgPtr->PnHandleMultipleNetworkRequests
                && chRTPtr->CanNmTxEnable)
#endif
        )
        {
            /**
             * Set send NM message flag
             *  */
            chRTPtr->SendNmMsgFlg = TRUE;

            chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = chCfgPtr->ImmediateNmCycleTime;
            chRTPtr->ImmedMsgTxCnt                       = chCfgPtr->ImmediateNmTransmissions;
        }
    }
#endif
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
    if ((chRTPtr->CanNmMode == NM_MODE_BUS_SLEEP) || (chRTPtr->CanNmMode == NM_MODE_PREPARE_BUS_SLEEP))
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
 * @trace       CPD-70096
 */
CANNM_LOCAL void CanNm_EnterRepeatMessageState(CanNm_ChannelIndexType chIndex, boolean isNetWorkRequest)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePrt;

    /** SWS_CanNm_00102 */
    chRTPtr->TickTimers[CANNM_STATE_HOLE_TIMER]  = chCfgPtr->RepeatMessageTime;
    chRTPtr->TimeoutFlag[CANNM_STATE_HOLE_TIMER] = FALSE;

/** SWS_CanNm_00100 */
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
    CanNm_StartTransmissionNmPdu(chIndex, isNetWorkRequest);
#endif

#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
    /** SWS_CanNm_00152 */
    CanNm_RemoteSleepCancellHandle(chIndex);
#endif

    /**
     * When the Network Mode is entered
     *  */
    if (chRTPtr->CanNmMode != NM_MODE_NETWORK)
    {
#if CANNM_COORDINATOR_SYNC_SUPPORT == STD_ON
        chRTPtr->CoordReadyToSleepInd = CANNM_CSR_IND_INIT;
        CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_CSR_MASK, FALSE);
#endif

        /** SWS_CanNm_00096 */
        chRTPtr->TickTimers[CANNM_NM_TIMEOUT_TIMER] = chCfgPtr->TimeoutTime;
        /** SWS_CanNm_00097 */
        chRTPtr->CanNmMode = NM_MODE_NETWORK;
        Nm_NetworkMode(chCfgPtr->ComMNetworkHandleRef);
    }

    CanNm_StateChange(chIndex, NM_STATE_REPEAT_MESSAGE);
}

/**
 * @brief       Processes when enter Bus-Sleep Mode from other state.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70097
 */
CANNM_LOCAL void CanNm_EnterPrepareBusSleepModeHandle(uint8 chIndex)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePrt;

    CanNm_StateChange(chIndex, NM_STATE_PREPARE_BUS_SLEEP);
    chRTPtr->CanNmMode = NM_MODE_PREPARE_BUS_SLEEP;

    /** SWS_CanNm_00114 */
    Nm_PrepareBusSleepMode(chCfgPtr->ComMNetworkHandleRef);

    /** SWS_CanNm_00088 */
    if (!chCfgPtr->StayInPbsEnabled)
    {
        chRTPtr->TickTimers[CANNM_STATE_HOLE_TIMER] = chCfgPtr->WaitBusSleepTime;
    }

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
    /** SWS_CanNm_00402 */
    if (chCfgPtr->ActiveWakeupBitEnabled)
    {
        CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_AW_MASK, FALSE);
    }
#endif
}

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
/**
 * @brief       According to the current state and active or passive node detect flag to state change.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70098
 */
CANNM_LOCAL boolean CanNm_NodeDetectStateHandle(CanNm_ChannelIndexType chIndex)
{
    boolean                         stateChanged = FALSE;
    const CanNm_ChannelLConfigType* chCfgPtr     = &CanNm_ChannelLCfgData[chIndex];

    CanNm_InnerChannelType* chRTPtr   = chCfgPtr->RuntimePrt;
    Nm_StateType            currState = chRTPtr->CanNmState;
    if (currState == NM_STATE_REPEAT_MESSAGE)
    {
        CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_RMP_MASK, FALSE);
    }
    if ((chRTPtr->RepeatMessageRequest || chRTPtr->RepeatMessageDetected)
        && ((currState == NM_STATE_NORMAL_OPERATION) || (currState == NM_STATE_READY_SLEEP)))
    {
        CanNm_EnterRepeatMessageState(chIndex, FALSE);
        if (chRTPtr->RepeatMessageRequest)
        {
            CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_RMP_MASK, TRUE);
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
 * @trace       CPD-70099
 */
CANNM_LOCAL boolean CanNm_NetworkStateHandle(CanNm_ChannelIndexType chIndex)
{
    const CanNm_ChannelLConfigType* chCfgPtr     = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr      = chCfgPtr->RuntimePrt;
    boolean                         stateChanged = FALSE;

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
    if (CanNm_TestAndClear(&chRTPtr->NetRequestFlg))
    {
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
        if (chCfgPtr->PnHandleMultipleNetworkRequests)
        {
            CanNm_EnterRepeatMessageState(chIndex, TRUE);
            stateChanged = TRUE;
        }
#endif
    }
#endif

#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
    if (!stateChanged)
#endif
    {
        Nm_StateType currState = chRTPtr->CanNmState;
        if ((chRTPtr->NetRequestStatus == CANNM_NETWORK_REQUESTED) && (currState != NM_STATE_NORMAL_OPERATION))
        {
            CanNm_StateChange(chIndex, NM_STATE_NORMAL_OPERATION);
#if CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON
            chRTPtr->TickTimers[CANNM_REMOTE_SLEEP_TIMER] = chCfgPtr->RemoteSleepIndTime;
#endif
            stateChanged = TRUE;
        }

        if ((chRTPtr->NetRequestStatus == CANNM_NETWORK_RELEASED) && (currState != NM_STATE_READY_SLEEP))
        {
            CanNm_StateChange(chIndex, NM_STATE_READY_SLEEP);
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
            chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = 0u;
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
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
 * @trace       CPD-70100
 */
CANNM_LOCAL void CanNm_RepeatMessageStateHandle(CanNm_ChannelIndexType chIndex)
{
    CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

    if (CanNm_TestAndClear(&chRTPtr->TimeoutFlag[CANNM_STATE_HOLE_TIMER]))
    {
#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
        chRTPtr->ImmedMsgTxCnt = 0u;
#endif

/** SWS_CanNm_00107 */
#if CANNM_NODE_DETECTION_ENABLED == STD_ON
        if (CanNm_ChannelLCfgData[chIndex].NodeDetectionEnabled)
        {
            (void)CanNm_NodeDetectStateHandle(chIndex);
        }
#endif

        (void)CanNm_NetworkStateHandle(chIndex);
    }
}

/**
 * @brief       Handle state transition in Normal Operation State.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70101
 */
CANNM_LOCAL void CanNm_NormalOperationStateHandle(CanNm_ChannelIndexType chIndex)
{
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
#if (CANNM_NODE_DETECTION_ENABLED == STD_ON) || (CANNM_BUSLOAD_REDUCTION_ENABLED == STD_ON)
    const CanNm_ChannelLConfigType* chCfgPtr     = &CanNm_ChannelLCfgData[chIndex];
    boolean                         stateChanged = FALSE;
    stateChanged                                 = CanNm_NetworkStateHandle(chIndex);
#else
    (void)CanNm_NetworkStateHandle(chIndex);
#endif

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
    if (!stateChanged && chCfgPtr->NodeDetectionEnabled)
    {
        stateChanged = CanNm_NodeDetectStateHandle(chIndex);
    }
#endif

/** SWS_CanNm_00157 */
#if CANNM_BUSLOAD_REDUCTION_ENABLED == STD_ON
    CanNm_InnerChannelType* chRTPtr = chCfgPtr->RuntimePrt;
    if (!stateChanged && chRTPtr->RxPduExtFlg)
    {
        if (chCfgPtr->BusLoadReductionActive)
        {
            chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = CanNm_ChlPBCfgPtr[chIndex].MsgReducedTime;
        }
    }
#endif
#endif
}

/**
 * @brief       Handle state transition in Ready Sleep State.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70102
 */
CANNM_LOCAL void CanNm_ReadySleepStateHandle(CanNm_ChannelIndexType chIndex)
{
    const CanNm_ChannelLConfigType* chCfgPtr     = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr      = chCfgPtr->RuntimePrt;
    boolean                         stateChanged = FALSE;

/** SWS_CanNm_00110
 *  Ready Sleep State shall enter Normal Operation State
 *  */
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
    if (chRTPtr->NetRequestFlg
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
        && !chCfgPtr->PnHandleMultipleNetworkRequests
#endif
#if CANNM_COM_CONTROL_ENABLED == STD_ON
        && chRTPtr->CanNmTxEnable
#endif
    )
    {
        /** SWS_CanNm_00116 */
        chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
        /** SWS_CanNm_00006 */
        chRTPtr->SendNmMsgFlg = TRUE;
    }
#endif

    stateChanged = CanNm_NetworkStateHandle(chIndex);

#if CANNM_NODE_DETECTION_ENABLED == STD_ON
    if (!stateChanged && chCfgPtr->NodeDetectionEnabled)
    {
        stateChanged = CanNm_NodeDetectStateHandle(chIndex);
    }
#endif

    /**
     *  Depends on the Network Management PDU transmission ability is enabled,
     *  because NM-Timeout will stop if function CanNm_DisableCommunication has bean called.
     *  */
    if (!stateChanged && CanNm_TestAndClear(&chRTPtr->TimeoutFlag[CANNM_NM_TIMEOUT_TIMER]))
    {
        /** SWS_CanNm_00109 */
        CanNm_EnterPrepareBusSleepModeHandle(chIndex);
    }
}

/**
 * @brief       Network wake up signal processing for Non-network mode.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70103
 */
CANNM_LOCAL boolean CanNm_WakeUpSignalHandle(CanNm_ChannelIndexType chIndex)
{
    boolean                 stateChanged = FALSE;
    CanNm_InnerChannelType* chRTPtr      = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

    boolean rxWakeup = (chRTPtr->RxPduExtFlg && (chRTPtr->CanNmState == NM_STATE_PREPARE_BUS_SLEEP)) ? TRUE : FALSE;

    boolean isNetWorkRequest = FALSE;
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
    isNetWorkRequest = chRTPtr->NetRequestFlg;
    /**
     *  TRUE only if network request and not passive startup.
     *  */
    boolean activeWakeUpBit = (chRTPtr->NetRequestFlg && !chRTPtr->PassiveStartUp) ? TRUE : FALSE;
#endif

    /** SWS_CanNm_00128 SWS_CanNm_00123 SWS_CanNm_00124
     *  1. if CanNm_PassiveStartUp is called in the Bus-Sleep Mode or Prepare Bus-Sleep Mode
     *  2. if the network is requested in the Prepare Bus-Sleep Mode
     *  3. At successful reception of a Network Management PDU in the Prepare Bus-Sleep Mode
     *  */
    if (CanNm_TestAndClear(&chRTPtr->PassiveStartUp) || rxWakeup
#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
        || CanNm_TestAndClear(&chRTPtr->NetRequestFlg)
#endif
    )
    {
        CanNm_EnterRepeatMessageState(chIndex, isNetWorkRequest);

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
        if (CanNm_ChannelLCfgData[chIndex].ActiveWakeupBitEnabled && activeWakeUpBit)
        {
            /** SWS_CanNm_00401 */
            CanNm_SetTxPduCbvBit(chIndex, CANNM_CBV_BIT_AW_MASK, TRUE);
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
 * @trace       CPD-70104
 */
CANNM_LOCAL void CanNm_PrepareBusSleepStateHandle(CanNm_ChannelIndexType chIndex)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePrt;

    boolean stateChanged = CanNm_WakeUpSignalHandle(chIndex);

    if (!stateChanged && CanNm_TestAndClear(&chRTPtr->TimeoutFlag[CANNM_STATE_HOLE_TIMER]))
    {
        CanNm_StateChange(chIndex, NM_STATE_BUS_SLEEP);
        chRTPtr->CanNmMode = NM_MODE_BUS_SLEEP;

        /** SWS_CanNm_00126 */
        Nm_BusSleepMode(chCfgPtr->ComMNetworkHandleRef);
    }
}

/**
 * @brief       Handle state transition in Bus Sleep State.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70105
 */
CANNM_LOCAL void CanNm_BusSleepStateHandle(CanNm_ChannelIndexType chIndex)
{
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    if (chCfgPtr->RuntimePrt->RxPduExtFlg)
    {
        /** SWS_CanNm_00127 */
        Nm_NetworkStartIndication(chCfgPtr->ComMNetworkHandleRef);
        /** SWS_CanNm_00336 */
        (void)Det_ReportRuntimeError(
            CANNM_MODULE_ID,
            CANNM_INSTANCE_ID,
            CANNM_SERVICE_ID_RXINDICATION,
            CANNM_E_NET_START_IND);
    }

    (void)CanNm_WakeUpSignalHandle(chIndex);
}

/**
 * @brief       Handle state transition of CanNm.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70106
 */
CANNM_LOCAL void CanNm_StateManagement(CanNm_ChannelIndexType chIndex)
{
    switch (CanNm_ChannelLCfgData[chIndex].RuntimePrt->CanNmState)
    {
    case NM_STATE_BUS_SLEEP:
        CanNm_BusSleepStateHandle(chIndex);
        break;

    case NM_STATE_PREPARE_BUS_SLEEP:
        CanNm_PrepareBusSleepStateHandle(chIndex);
        break;

    case NM_STATE_REPEAT_MESSAGE:
        CanNm_RepeatMessageStateHandle(chIndex);
        break;

    case NM_STATE_NORMAL_OPERATION:
        CanNm_NormalOperationStateHandle(chIndex);
        break;

    case NM_STATE_READY_SLEEP:
        CanNm_ReadySleepStateHandle(chIndex);
        break;

    default:
        /**
         *  nothing
         *  */
        break;
    }
}

#if CANNM_COM_USERDATA_SUPPORT == STD_ON
/**
 * @brief       Get the UserData and fill it into the UserData field of Tx buffer.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70107
 */
CANNM_LOCAL void CanNm_WriteUserDataBytes(CanNm_ChannelIndexType chIndex)
{
    const CanNm_UserDataTxPduType* userDataPduPtr = CanNm_ChlPBCfgPtr[chIndex].UserDataTxPdu;
    if (userDataPduPtr != NULL_PTR)
    {
        PduInfoType pduInfo                              = {0};
        uint8       userData[CANNM_USER_DATA_MAX_LENGTH] = {0};
        PduIdType   pdurTxSduId                          = userDataPduPtr->UpperLayerPduId;

        const CanNm_ChannelPBConfigType* chPBCfgPtr = &CanNm_ChlPBCfgPtr[chIndex];
        pduInfo.SduLength                           = chPBCfgPtr->UserDataLength;
        pduInfo.SduDataPtr                          = userData;
        pduInfo.MetaDataPtr                         = NULL_PTR;

        /** SWS_CanNm_00450 */
        if (PduR_CanNmTriggerTransmit(pdurTxSduId, &pduInfo) == E_OK)
        {
            SchM_Enter_CanNm_Context();
            (void)IStdLib_MemCpy(
                &(CanNm_ChannelLCfgData[chIndex].RuntimePrt->TxPduData[chPBCfgPtr->UserDataOffset]),
                userData,
                chPBCfgPtr->UserDataLength);
            SchM_Exit_CanNm_Context();
        }
    }
}
#endif

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * @brief       Assemble NM packets and send them.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70108
 */
CANNM_LOCAL Std_ReturnType CanNm_SendNmPdu(CanNm_ChannelIndexType chIndex)
{
    Std_ReturnType                  ret      = E_NOT_OK;
    const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
    CanNm_InnerChannelType*         chRTPtr  = chCfgPtr->RuntimePrt;

#if CANNM_COM_CONTROL_ENABLED == STD_ON
    if (!chRTPtr->CanNmTxEnable)
    {
        ret = E_NOT_OK;
    }
    else
#endif
    {
        const CanNm_ChannelPBConfigType* chPBCfgPtr = &CanNm_ChlPBCfgPtr[chIndex];
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
        if (chPBCfgPtr->PnEnabled)
        {
            /** SWS_CanNm_00515 SWS_CanNm_00518 */
            Nm_PncBitVectorTxIndication(chCfgPtr->ComMNetworkHandleRef, &chRTPtr->TxPduData[NM_PN_BIT_VECTORY_OFFSET]);
        }
#endif

#if CANNM_TX_PDU_NUMBER > 0u
/** SWS_CanNm_00328 */
#if CANNM_COM_USERDATA_SUPPORT == STD_ON
        if (!chPBCfgPtr->TxPdu->TxPduIsTriggerTrans)
        {
            CanNm_WriteUserDataBytes(chIndex);
        }
#endif
        PduInfoType pduInfo      = {0};
        PduIdType   canIfTxSduId = chPBCfgPtr->TxPdu->LowerLayerPduId;
        pduInfo.SduDataPtr       = chRTPtr->TxPduData;
        pduInfo.SduLength        = chPBCfgPtr->ChannelPduLength;

        /** SWS_CanNm_00512 */
        ret = CanIf_Transmit(canIfTxSduId, &pduInfo);
        if (ret == E_OK)
        {
/** SWS_CanNm_00064 */
#if CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF
#if CANNM_GLOBAL_PN_SUPPORT == STD_ON
            chRTPtr->TickTimers[CANNM_TXMSG_TIMEOUT_TIMER]  = chPBCfgPtr->MsgTimeoutTime;
            chRTPtr->TimeoutFlag[CANNM_TXMSG_TIMEOUT_TIMER] = FALSE;
#endif
#else
            CanNm_InnerTxConfHandle(chIndex, E_OK);
#endif
        }
#endif
    }
    return ret;
}
#endif

#if CANNM_PASSIVE_MODE_ENABLED == STD_OFF
/**
 * @brief       Send message in MainFunction.The purpose of this function is to aggregate multiple send requests,Only
 *              one frame will be sent out when there are multiple send requests in a MainFunction.
 * @param[in]   chIndex          : Index value of the NM-channel.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70109
 */
CANNM_LOCAL void CanNm_SendMsgMainHandle(CanNm_ChannelIndexType chIndex)
{
    CanNm_InnerChannelType* chRTPtr = CanNm_ChannelLCfgData[chIndex].RuntimePrt;

    if (chRTPtr->SendNmMsgFlg)
    {
        chRTPtr->SendNmMsgFlg                    = FALSE;
        const CanNm_ChannelLConfigType* chCfgPtr = &CanNm_ChannelLCfgData[chIndex];
        if (CanNm_SendNmPdu(chIndex) == E_OK)
        {
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
            chRTPtr->RetrySendMsgFlag = FALSE;
#endif
#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
            if (chRTPtr->ImmedMsgTxCnt > 0u)
            {
                chRTPtr->ImmedMsgTxCnt--;
                if (chRTPtr->ImmedMsgTxCnt == 0u)
                {
                    chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
                }
                else
                {
                    chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = chCfgPtr->ImmediateNmCycleTime;
                }
            }
            else
#endif
            {
                /** SWS_CanNm_00040 */
                chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
            }
        }
        else
        {
#if CANNM_IMMEDIATE_TRANSMIT_ENABLED == STD_ON
            if (chRTPtr->ImmedMsgTxCnt > 0u)
            {
                /** SWS_CanNm_00335 */
                chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = 1u;
            }
            else
#endif
            {
#if CANNM_RETRY_FIRST_MESSAGE_REQUEST == STD_ON
                if (chRTPtr->RetrySendMsgFlag)
                {
                    chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = 1u;
                }
                else
#endif
                {
                    /** SWS_CanNm_00040 */
                    chRTPtr->TickTimers[CANNM_TXMSG_CYCLE_TIMER] = chCfgPtr->MsgCycleTime;
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
 * @trace       CPD-70110
 */
CANNM_LOCAL uint16 CanNm_GetPartitionIndex(void)
{
    uint16 index = 0u;
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType applicationID = GetApplicationID();
    for (; index < CANNM_NUMBER_OF_PARTITION; ++index)
    {
        if (CanNm_OsApplicationList[index] == applicationID)
        {
            break;
        }
    }
#endif
    return index;
}

#if CANNM_DEV_ERROR_DETECT == STD_ON
/**
 * @brief       Development error validation of invalid partition context.
 * @param[in]   apiId : ID of API service in which error is detected
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70111
 */
CANNM_LOCAL boolean CanNm_ValidatePartitionContext(uint8 apiId)
{
    boolean ret = TRUE;
    if (CanNm_GetPartitionIndex() >= CANNM_NUMBER_OF_PARTITION)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_PARTITION);
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
 * @trace       CPD-70112
 */
CANNM_LOCAL boolean CanNm_ValidateInitStatus(uint8 apiId)
{
    boolean              ret     = TRUE;
    uint8                errorId = CANNM_E_INVALID_ID;
    CanNm_InitStatusType status  = CanNm_InitStatus[CanNm_GetPartitionIndex()];

    if ((apiId == CANNM_SERVICE_ID_INIT) && (status == CANNM_INIT))
    {
        errorId = CANNM_E_INIT_FAILED;
    }

    if ((apiId != CANNM_SERVICE_ID_INIT) && (status == CANNM_UNINIT))
    {
        errorId = CANNM_E_UNINIT;
    }

    if (errorId != CANNM_E_INVALID_ID)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, errorId);
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
 * @trace       CPD-70113
 */
CANNM_LOCAL boolean CanNm_ValidateNetworkHandle(uint8 apiId, NetworkHandleType nmChannelHandle)
{
    boolean                ret     = TRUE;
    uint8                  errorId = CANNM_E_INVALID_ID;
    CanNm_ChannelIndexType chIndex = CanNm_FindChannelIndex(nmChannelHandle);
    if (chIndex >= CANNM_NUMBER_OF_CHANNEL)
    {
        errorId = CANNM_E_INVALID_CHANNEL;
    }
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
    else if (CanNm_ChannelLCfgData[chIndex].ApplicationID != GetApplicationID())
    {
        errorId = CANNM_E_PARTITION;
    }
#endif

    if (errorId != CANNM_E_INVALID_ID)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, errorId);
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
 * @trace       CPD-70114
 */
CANNM_LOCAL boolean CanNm_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean ret = TRUE;
    if (pointer == NULL_PTR)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_PARAM_POINTER);
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
 * @trace       CPD-70115
 */
boolean CanNm_ValidateTxPduId(uint8 apiId, PduIdType txPduId)
{
    boolean ret = TRUE;
    if (txPduId >= CANNM_TX_PDU_NUMBER)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_INVALID_PDUID);
        ret = FALSE;
    }
    return ret;
}

/**
 * @brief       Development error validation of Tx-Pdu.
 * @param[in]   apiId : ID of API service in which error is detected
 * @param[in]   txPduId : ID of the SDU that is requested to be transmitted
 * @param[in]   pduInfoPtr : Contains a pointer to a buffer about pduInfo
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70116
 */
CANNM_LOCAL boolean CanNm_ValidateTxPdu(uint8 apiId, PduIdType txPduId, const PduInfoType* pduInfoPtr)
{
    return CanNm_ValidateTxPduId(apiId, txPduId) && CanNm_ValidatePointer(apiId, pduInfoPtr)
           && CanNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
}

/**
 * @brief       Development error validation of Rx-Pdu.
 * @param[in]   apiId : ID of API service in which error is detected
 * @param[in]   rxPduId : ID of the received PDU.
 * @param[in]   pduInfoPtr : Contains a pointer to a buffer about pduInfo
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70117
 */
CANNM_LOCAL boolean CanNm_ValidateRxPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr)
{
    boolean ret = TRUE;
    if (rxPduId >= CANNM_RX_PDU_NUMBER)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_INVALID_PDUID);
        ret = FALSE;
    }

    ret = ret && CanNm_ValidatePointer(apiId, pduInfoPtr) && CanNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
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
 * @trace       CPD-70118
 */
CANNM_LOCAL boolean CanNm_ValidateUserDataPdu(uint8 apiId, PduIdType rxPduId, const PduInfoType* pduInfoPtr)
{
    boolean ret = TRUE;
    if (rxPduId >= CANNM_USERDATA_TX_PDU_NUM)
    {
        (void)Det_ReportError(CANNM_MODULE_ID, CANNM_INSTANCE_ID, apiId, CANNM_E_INVALID_PDUID);
        ret = FALSE;
    }

    ret = ret && CanNm_ValidatePointer(apiId, pduInfoPtr) && CanNm_ValidatePointer(apiId, pduInfoPtr->SduDataPtr);
    return ret;
}

/**
 * @brief       Development error validation of deinit condition.
 * @retval      TRUE  : Development error not occurred
 * @retval      FALSE : Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70119
 */
CANNM_LOCAL boolean CanNm_ValidateDeInit(void)
{
    boolean ret = TRUE;
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType applicationID = GetApplicationID();
#endif
    for (CanNm_ChannelIndexType chIndex = 0u; chIndex < CANNM_NUMBER_OF_CHANNEL; chIndex++)
    {
#if CANNM_MULTIPLE_PARTITION_USED == STD_ON
        if (CanNm_ChannelLCfgData[chIndex].ApplicationID == applicationID)
#endif
        {
            if (CanNm_ChannelLCfgData[chIndex].RuntimePrt->CanNmState != NM_STATE_BUS_SLEEP)
            {
                ret = FALSE;
                (void)Det_ReportError(
                    CANNM_MODULE_ID,
                    CANNM_INSTANCE_ID,
                    CANNM_SERVICE_ID_DEINIT,
                    CANNM_E_NOT_IN_BUS_SLEEP);
                break;
            }
        }
    }
    return ret;
}
#endif
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
/* PRQA S 1532 -- */
