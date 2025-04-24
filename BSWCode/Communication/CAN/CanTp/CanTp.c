/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
**************************************************************************** **
**                                                                           **
**  FILENAME    : CanTp.c                                                    **
**                                                                           **
**  Created on  : 2021/7/30 14:29:43                                         **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Public functions implementation                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanTp<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:Functions may be used in other projects.
 */

/*=======[I N C L U D E S]====================================================*/
#include "CanTp.h"
#include "CanTp_Internal.h"
#include "CanTp_Cbk.h"

/*=======[V E R S I O N  C H E C K]===========================================*/
/*check version information with CanTp*/
#if (                                                                                                           \
    (CANTP_AR_RELEASE_MAJOR_VERSION != CANTP_CFG_H_AR_MAJOR_VERSION)                                            \
    || (CANTP_AR_RELEASE_MINOR_VERSION != CANTP_CFG_H_AR_MINOR_VERSION)                                         \
    || (CANTP_AR_RELEASE_PATCH_VERSION != CANTP_CFG_H_AR_PATCH_VERSION) || (2u != CANTP_CFG_H_SW_MAJOR_VERSION) \
    || (1u != CANTP_CFG_H_SW_MINOR_VERSION) || (1u != CANTP_CFG_H_SW_PATCH_VERSION))
#error " CanTp cfg file version mismatching with CanTp"
#endif
/*=======[M A C R O S]========================================================*/
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define CANTP_START_SEC_VAR_POWER_ON_INIT_8
#include "CanTp_MemMap.h"
/*CANTP global variable for module state:CANTP_ON, CANTP_OFF*/
CANTP_LOCAL VAR(CanTp_StateType, CANTP_VAR_POWER_ON_INIT) CanTp_ModuleState = CANTP_OFF;
#if (STD_ON == CANTP_RX_QUEUE)
CANTP_LOCAL VAR(uint8, CANTP_VAR_POWER_ON_INIT) CanTp_RxQueueCurrentIndex = 0u;
#endif /* STD_ON == CANTP_RX_QUEUE */
#define CANTP_STOP_SEC_VAR_POWER_ON_INIT_8
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#include "CanTp_MemMap.h"
/*CANTP cfg ptr*/
P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CanTp_ConfigPtr = NULL_PTR;
#define CANTP_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#include "CanTp_MemMap.h"

#if (STD_ON == CANTP_TIME_MAINFUNCTION_ENABLED)
#define CANTP_START_SEC_VAR_POWER_ON_INIT_32
#include "CanTp_MemMap.h"
VAR(uint32, CANTP_VAR_POWER_ON_INIT) CanTp_Timer = 0u;
#define CANTP_STOP_SEC_VAR_POWER_ON_INIT_32
#include "CanTp_MemMap.h"
#endif /* STD_ON == CANTP_TIME_MAINFUNCTION_ENABLED */

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (STD_ON == CANTP_RX_QUEUE)
#define CANTP_START_SEC_VAR_NO_INIT_8
#include "CanTp_MemMap.h"
CANTP_LOCAL VAR(uint8, CANTP_VAR_NOINIT) CanTp_RxQueue[CANTP_CHANNEL_NUMBER];
#define CANTP_STOP_SEC_VAR_NO_INIT_8
#include "CanTp_MemMap.h"
#endif /* STD_ON == CANTP_RX_QUEUE */
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
/*Initialize the specific channel.*/
CANTP_LOCAL CANTP_INLINE FUNC(void, CANTP_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    CanTp_InitSubChannel(P2VAR(CanTp_ConnectionType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) SubchannelPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*Initialize the Tx specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_InitTxChannel(CanTp_ConnectionType* TxSubchannelPtr);

/*Initialize the specific channel.*/
CANTP_LOCAL CANTP_INLINE FUNC(void, CANTP_CODE) CanTp_InitChannel(CanTp_ConnectionChannelType* connectionChannel);

/*This function is called by the CanTp_RxIndication after a successful
reception of a RX CAN L-PDU, put the PDU into pool.*/
CANTP_LOCAL FUNC(void, CANTP_CODE)
    CanTp_RxSubDeal(PduIdType CanTpRxPduId, P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr);

/* SF come, just execute one time of PduR_CanTpCopyRxData.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithSF(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionChannelType* connectionChannel);

/* SF come, just execute one time of PduR_CanTpCopyRxData.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithSFToUp(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionChannelType* connectionChannel);

/* FC come, just execute one time of PduR_CanTpCopyRxData.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithFC(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    CanTp_ConnectionType* TxSubchannelPtr);

/* CF come, just execute one time of PduR_CanTpCopyRxData.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithCF(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionType* RxchannelPtr);

/* CF come, just execute one time of PduR_CanTpCopyRxData.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithCFToUp(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionType* RxchannelPtr);

/*FF come, just execute one time of PduR_CanTpCopyRxData.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithFF(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionChannelType* connectionChannel);

/*FF come, just execute one time of PduR_CanTpCopyRxData.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithFFToUp(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionChannelType* connectionChannel);

#if (STD_ON == CANTP_FD)
CANTP_LOCAL FUNC(boolean, CANTP_CODE) CanTp_checkSF(PduLengthType SF_DL, PduLengthType CAN_DL, PduLengthType Offset);
CANTP_LOCAL FUNC(boolean, CANTP_CODE)
    CanTp_checkLastCF(PduLengthType CF_DL, PduLengthType CAN_DL, PduLengthType Offset);
#endif /* STD_ON == CANTP_FD */

CANTP_LOCAL CANTP_INLINE FUNC(void, CANTP_CODE) CanTp_ReleaseRxChannel(CanTp_ConnectionType* RxChannelPtr);

CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_ReleaseTxChannel(CanTp_ConnectionType* TxSubchannelPtr);

/*Start a block by sending FC_CTS if needed.*/
CANTP_LOCAL FUNC(Std_ReturnType, CANTP_CODE) CanTp_RxBlockStart(
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionType* RxchannelPtr);

/*Handle RX event occurred to the specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_DispatchRxEvent(CanTp_ConnectionType* RxchannelPtr);
/*Handle RX event occurred to the specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_DispatchRxEventSubDeal(
    CanTp_ConnectionType* RxchannelPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr);

/* Get SDU configuration by N-SDU ID.*/
CANTP_LOCAL FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
    CanTp_GetRxSduCfgByNSduId(PduIdType CanTpRxSduId, uint8* ChannelId);

/*Handle TX event occurred to the specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_DispatchTxEvent(CanTp_ConnectionChannelType* connectionChannel);

/*Get TX-SDU configuration by TX-NSduId.*/
CANTP_LOCAL FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
    CanTp_GetTxSduCfgByTxSduId(PduIdType TxSduId, uint8* ChannelId);

/*Get Tx-SDU configuration by Tx NPDUID.*/
CANTP_LOCAL FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
    CanTp_GetTxSduCfgByTxNPdu(PduIdType CanTpTxPduId, uint8* ChannelId);

/* Get Rx-SDU configuration by Tx-FC NPDUID.*/
CANTP_LOCAL FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
    CanTp_GetRxSduCfgByTxFCNPdu(PduIdType CanTpTxPduId, uint8* ChannelId);

#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
/*Handle the diag gateway timer for specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_HandleGatewayTimers(uint8 ChannelId);
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */

/*Handle the timer for specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_HandleTimers(CanTp_ConnectionChannelType* connectionChannel);

/*Handle the tx timer for specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_HandleTxTimers(CanTp_ConnectionType* TxSubchannelPtr);

#if ((CANTP_CHANGE_PARAMETER == STD_ON) && (STD_ON == CANTP_DEV_ERROR_DETECT))
/*check Rx ChangeParameter*/
CANTP_LOCAL CANTP_INLINE FUNC(boolean, CANTP_CODE)
    CanTP_CheckRxChangeParameter(TPParameterType canTpParam, uint16 value);
#endif /* CANTP_CHANGE_PARAMETER == STD_ON && STD_ON == CANTP_DEV_ERROR_DETECT*/

#if (STD_ON == CANTP_RX_QUEUE)
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_AddRxQueue(uint8* buffer, uint8 ChannelId, uint8* accIndex);
#endif /* STD_ON == CANTP_RX_QUEUE */
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
CANTP_LOCAL FUNC(void, CANTP_APPL_CODE) CanTp_SFCFTxConfirmationSubDealWith(
    CanTp_ConnectionChannelType* ChannelPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr);

CANTP_LOCAL FUNC(void, CANTP_APPL_CODE) CanTp_FCTxConfirmationSubDealWith(PduIdType TxPduId);
#define CANTP_STOP_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/*
 * Brief               This function initializes the CanTp module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE)
CanTp_Init(P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CfgPtr)
{
    uint8 ChannelId;
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (NULL_PTR == CfgPtr)
    {
        /*invalid transmit request information pointer*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_INIT, CANTP_E_PARAM_POINTER);
    }
    else if (CfgPtr->ChannelNum > CANTP_MAX_CHANNEL_NUMBER)
    {
        /*invalid channel number*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_INIT, CANTP_E_INIT_FAILED);
    }
    else
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    {
        SchM_Enter_CanTp_ExclusiveArea(); /*lock state*/
        /*if the module is working, turn off it, then it can not receive or transmit new N-SDUs*/
        if (CANTP_ON == CanTp_ModuleState)
        {
            CanTp_ModuleState = CANTP_OFF;
        }
        CanTp_ConfigPtr = CfgPtr;
        SchM_Exit_CanTp_ExclusiveArea(); /*unlock state*/

        /*Initialize all the channels to stop all the working channels*/
        for (ChannelId = 0; ChannelId < CANTP_CHANNEL_NUMBER; ChannelId++)
        {
            CANTP_RXCHANNEL(ChannelId)->ChannelId = ChannelId;
            CanTp_InitSubChannel(CANTP_RXCHANNEL(ChannelId));
            CANTP_TXSUBCHANNEL(ChannelId)->ChannelId = ChannelId;
            CanTp_InitTxChannel(CANTP_TXSUBCHANNEL(ChannelId));
        }
        /*CanTp start to work*/
        CanTp_ModuleState = CANTP_ON;
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This function to shutdown the CanTp module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI           COM Manager
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE) CanTp_Shutdown(void)
{
    /*CanTp stops working*/
    CanTp_ModuleState = CANTP_OFF;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This service is used to request the transfer of segmented data.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be transmitted.
 *                     CanTpTxInfoPtr indicator of a structure with CAN N-SDU related
 *                                    data:indicator of a CAN N-SDU buffer and the length
 *                                    of this buffer.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:request can be started successfully
 *                     E_NOT_OK:request cannot be started
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_Transmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr)
{
    const CanTp_TxNSduType* TxNSduCfgPtr;
    Std_ReturnType result = E_NOT_OK;
    uint8 ChannelId;
    CanTp_ConnectionType* TxSubchannelPtr;
    const CanTp_ConnectionType* RxchannelPtr;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the request only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_UNINIT);
    }
    else if (NULL_PTR == PduInfoPtr)
    {
        /*invalid transmit request information pointer*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    {
        /*information pointer OK*/
        TxNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(TxPduId, &ChannelId);
        if (NULL_PTR != TxNSduCfgPtr)
        {
            /*A configured TX SDU matched. Try to accept the transmit request*/
            /*check sdu data total length of the transmit request*/
            if (E_OK == CanTp_CheckTxLengthInAddressMode(TxNSduCfgPtr, PduInfoPtr))
            {
                /*All request parameters are valid*/
                /*check if the channel is free*/
                TxSubchannelPtr = CANTP_TXSUBCHANNEL(ChannelId);
                RxchannelPtr = CANTP_RXCHANNEL(ChannelId);
#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
                CanTp_TxConnectionType* TxchannelPtr = CANTP_TXCHANNEL(ChannelId);
                TxchannelPtr->NeedRes.Started =
                    (TxchannelPtr->NeedRes.Started == TRUE) ? FALSE : TxchannelPtr->NeedRes.Started;
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */
                if ((CANTP_IDLE == TxSubchannelPtr->RootState)
                    && ((CANTP_MODE_FULL_DUPLEX == CanTp_ConfigPtr->CanTpChannel[ChannelId].ChannelMode)
                        || ((RxchannelPtr->RootState == CANTP_IDLE) || (RxchannelPtr->RootState == CANTP_OCCUPIED))))
                {
                    /*channel is free, and no event happened to it*/
                    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
                    TxSubchannelPtr->NSduId = TxNSduCfgPtr->TxNSduId;
                    TxSubchannelPtr->SduDataRemaining = PduInfoPtr->SduLength;
                    TxSubchannelPtr->SduDataTotalCount = PduInfoPtr->SduLength;
                    SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                    CanTp_SaveTxMetaDataInfo(TxSubchannelPtr, TxNSduCfgPtr, PduInfoPtr);
#endif              /* CANTP_DYN_ID_SUPPORT == STD_ON */
                    /*set channel as sender to avoid unexpected PDU reception,here can not make sure
                     * of SF*/
#if (CANTP_SYNCHRONOUS_TRANSMIT == STD_ON)
                    CanTp_TxHandleTransmitReq(TxSubchannelPtr, TxNSduCfgPtr, ChannelId);
#else
                    CanTp_TxHandleTransmitReq(TxSubchannelPtr, TxNSduCfgPtr);
                    /*start CS timer*/
                    CanTp_ChannelTimerType* channelTimer = &TxSubchannelPtr->ChannelTimer;
                    if (TxNSduCfgPtr->Ncs != NULL_PTR)
                    {
                        SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
                        channelTimer->EnabledTimer = CANTP_NC;
                        channelTimer->RemainTime = *(TxNSduCfgPtr->Ncs);
                        SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
                        CanTp_ResetTime(&(channelTimer->StartTime));
                    }
                    else
                    {
                        channelTimer->EnabledTimer = CANTP_TIMER_NONE;
                    }

#endif
                    result = E_OK;
                }
            }
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
            else
            {
                /*check TxTaType type, invalid*/
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TATYPE);
            }
#endif    /* STD_ON == CANTP_RUNTIME_ERROR_DETECT */
        } /*END OF if (NULL_PTR != txNSduCfgPtr)*/
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
        else
        {
            /*No configured TX SDU matched with this TxSduId*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_TRANSMIT, CANTP_E_INVALID_TX_ID);
        }
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    }  /* END OF if (CANTP_ON == CanTp_RuntimeControl.ModuleState)*/
    return result;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

#if (CANTP_TC == STD_ON)
/*
 * Brief               This service is used to cancel the transfer of pending
 *                     CAN N-SDUs. The connection is identified by CanTpTxPduId.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTpTxSduId ID of the CAN N-SDU to be canceled.
 *                     CanTpCancelReason The reason for cancelation.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(Std_ReturnType, CANTP_APPL_CODE) CanTp_CancelTransmit(PduIdType TxPduId)
{
    uint8 ChannelId;
    const CanTp_TxNSduType* txNSduCfgPtr;
    Std_ReturnType result = E_NOT_OK;
    CanTp_ConnectionType* TxSubchannelPtr;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the request only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_UNINIT);
    }
    else
    {
        txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(TxPduId, &ChannelId);
        if (NULL_PTR == txNSduCfgPtr)
        {
            /*Invalid TxNSduId, no configuration matched*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_PARAM_ID);
        }
        else
#else  /* STD_ON == CANTP_DEV_ERROR_DETECT */
    txNSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(TxPduId, &ChannelId);
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
        {
            if (CANTP_TC_MASK == (txNSduCfgPtr->CanTpPduFlag & CANTP_TC_MASK))
            {
                TxSubchannelPtr = CANTP_TXSUBCHANNEL(ChannelId);
                if ((CANTP_LARGE_TRANSMITTING == TxSubchannelPtr->RootState)
                    || (CANTP_SF_TRANSIMITTING == TxSubchannelPtr->RootState))
                {
                    /*A matched configuration matched*/ /*lock channel*/
                    if (TxSubchannelPtr->NSduId == TxPduId)
                    {
                        /* change tx RootState to idle , avoid release call the txconfiramation with
                         * E_NOT_OK*/
                        TxSubchannelPtr->RootState = CANTP_IDLE;
                        /*turn off the sub-state to reject the following events for this channel
                         * transmitting*/
                        CanTp_ReleaseTxChannel(TxSubchannelPtr);
                        PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_NOT_OK);
                        result = E_OK;
                    }
                }
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
                else
                {
                    CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_CANCELTRANSMITREQUEST, CANTP_E_OPER_NOT_SUPPORTED);
                }
#endif /* STD_ON == CANTP_RUNTIME_ERROR_DETECT */
            }
        }
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    }
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    return result;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               This service is used to cancel the reception of an ongoing N-SDU.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RxPduId This parameter contains the unique CanTp module identifier
 *                     of the N-SDU to be cancelled for transmission.
 *                     Range: 0..(maximum number of L-PDU IDs received ) - 1
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:Cancelation request is accepted
 *                     E_NOT_OK:Cancelation request is rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(Std_ReturnType, CANTP_APPL_CODE) CanTp_CancelReceive(PduIdType RxPduId)
{

    const CanTp_RxNSduType* rxNSduCfgPtr;
    Std_ReturnType result = E_NOT_OK;
    uint8 ChannelId;
    CanTp_ConnectionType* RxchannelPtr;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the request only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST, CANTP_E_UNINIT);
    }
    else
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    {
        /*Get configuration of the specified TX-NSDU*/
        rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(RxPduId, &ChannelId);
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
        if (NULL_PTR == rxNSduCfgPtr)
        {
            /*Invalid RxNSduId, no configuration matched*/
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST, CANTP_E_PARAM_ID);
        }
        else
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
        {
            /*A matched configuration matched*/
            RxchannelPtr = CANTP_RXCHANNEL(ChannelId);
            if (CANTP_LARGE_RECEIVING == RxchannelPtr->RootState)
            {
                /*last Consecutive Frame can not cancel*/
                if ((RxchannelPtr->NSduId == RxPduId) && (RxchannelPtr->SduDataRemaining > 0u))
                {
                    PduR_CanTpRxIndication(rxNSduCfgPtr->RxIPduId, E_NOT_OK);
                    /*Release the resource related with the channel and Notify the upper*/
                    CanTp_ReleaseRxChannel(RxchannelPtr);
                    result = E_OK;
                }
            }
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
            else
            {
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_CANCELRECEIVEREQUEST, CANTP_E_OPER_NOT_SUPPORTED);
            }
#endif /* STD_ON == CANTP_RUNTIME_ERROR_DETECT */
        }
    }
    return result;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
#endif /* CANTP_TC == STD_ON */

#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"

#if (CANTP_CHANGE_PARAMETER == STD_ON)
/*
 * Brief               Request to change a specific transport protocol parameter (e.g. block size).
 * ServiceId           0x4b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id:Identification of the PDU which the parameter change shall affect.
 *                     parameter:ID of the parameter that shall be changed.
 *                     value: The new value of the parameter.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              E_OK:The parameter was changed successfully.
 *                     E_NOT_OK:The parameter change was rejected.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value)
{
    Std_ReturnType result = E_NOT_OK;
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    const CanTp_RxNSduType* rxNSduCfgPtr;
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    uint8 ChannelId;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_CHANGEPARAMETER, CANTP_E_UNINIT);
    }
    else
    {
        rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(id, &ChannelId);
        if ((FALSE == CanTP_CheckRxChangeParameter(parameter, value)) || (NULL_PTR == rxNSduCfgPtr))
        {
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_CHANGEPARAMETER, CANTP_E_PARAM_ID);
        }
        else /*All request parameters are valid*/
#else        /* STD_ON == CANTP_DEV_ERROR_DETECT */
    (void)CanTp_GetRxSduCfgByNSduId(id, &ChannelId);
#endif       /* STD_ON == CANTP_DEV_ERROR_DETECT */
        {
            CanTp_ConnectionType* RxchannelPtr = CANTP_RXCHANNEL(ChannelId);
            if (RxchannelPtr->RootState != CANTP_LARGE_RECEIVING)
            {
                if (TP_BS == parameter)
                {
                    RxchannelPtr->ChangedBs = (uint8)value;
                    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                    CANTP_SETFLAG(RxchannelPtr->EventFlags, CANTP_EVENT_RXBSCHANGED);
                    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
                    result = E_OK;
                }
                else
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
                    if (TP_STMIN == parameter)
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
                {
                    RxchannelPtr->ChangedSTmin = (uint8)value;
                    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                    CANTP_SETFLAG(RxchannelPtr->EventFlags, CANTP_EVENT_RXSTMINCHANGED);
                    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
                    result = E_OK;
                }
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
                else
                {
                    /* idle */
                }
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
            }
        }
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    }
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    return result;
}
#endif /* CANTP_CHANGE_PARAMETER == STD_ON */

#if (CANTP_READ_PARAMETER == STD_ON)
/*
 * Brief               This service is used to read the current value of
                       reception parameters BS and STmin for a specified N-SDU.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id:Identification of the PDU which the parameter change shall affect.
 *                     parameter:ID of the parameter that shall be changed.
 * Param-Name[out]     value:Pointer where the parameter value will be provided.
 * Param-Name[in/out]  N/A
 * Return              E_OK:request is accepted
 *                     E_NOT_OK:request is not accepted.
 * PreCondition        CanTp module initialized
 * CallByAPI           Upper Layer module
 */
FUNC(Std_ReturnType, CANTP_APPL_CODE)
CanTp_ReadParameter(PduIdType id, TPParameterType parameter, uint16* value)
{

    Std_ReturnType result = E_NOT_OK;
    uint8 ChannelId;
    const CanTp_RxNSduType* rxNSduCfgPtr;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_UNINIT);
    }
    else
    {
        rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(id, &ChannelId);
        if (((parameter != TP_STMIN) && (parameter != TP_BS)) || (NULL_PTR == rxNSduCfgPtr))
        {
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_PARAM_ID);
        }
        else if (NULL_PTR == value)
        {
            CANTP_DET_REPORTERROR(CANTP_SERVICEID_READPARAMETER, CANTP_E_PARAM_POINTER);
        }
        else /*All request parameters are valid*/
#else        /* STD_ON == CANTP_DEV_ERROR_DETECT */
    rxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(id, &ChannelId);
#endif       /* STD_ON == CANTP_DEV_ERROR_DETECT */
        {
            const CanTp_ConnectionType* RxchannelPtr = CANTP_RXCHANNEL(ChannelId);
            if (TP_BS == parameter)
            {
                /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                *value = CANTP_FLAGISSET(RxchannelPtr->EventFlags, CANTP_EVENT_RXSTMINCHANGED)
                             ? (uint16)RxchannelPtr->ChangedBs
                             : (uint16)RxchannelPtr->CurrentBs;
                /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
                result = E_OK;
            }
            else
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
                if (TP_STMIN == parameter) /* TP_STMIN == parameter */
#endif                                     /* STD_ON == CANTP_DEV_ERROR_DETECT */
            {
                /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                if (CANTP_FLAGISSET(RxchannelPtr->EventFlags, CANTP_EVENT_RXSTMINCHANGED))
                /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
                {
                    /*STmin have been changed*/
                    *value = (uint16)RxchannelPtr->ChangedSTmin;
                }
                else
                {
                    *value = (rxNSduCfgPtr->STmin != NULL_PTR) ? (*(rxNSduCfgPtr->STmin)) : 0u;
                }
                result = E_OK;
            }
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
            else
            {
                /*idle*/
            }
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
        }
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    }
#endif /* STD_OFF == CANTP_DEV_ERROR_DETECT */
    return result;
}
#endif /* CANTP_READ_PARAMETER == STD_ON */

#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/*
 * Brief               The main function for scheduling the CANTP.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp module initialized
 * CallByAPI           SchM
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE) CanTp_MainFunction(void)
{
#if (STD_ON == CANTP_RX_QUEUE)
    uint8 tempQueue[CANTP_CHANNEL_NUMBER];
    uint8 tempQueueSize = 0u;
    uint8 tempQueueIndex;
#endif /* STD_ON == CANTP_RX_QUEUE */
    uint8 index;
    CanTp_ConnectionChannelType* connectionChannel;

    /*check module state, handle only when module started*/
    if (CANTP_ON == CanTp_ModuleState)
    {
#if (STD_ON == CANTP_RX_QUEUE)
        uint8 ChannelId;
#endif /* STD_ON == CANTP_RX_QUEUE */
#if (STD_ON == CANTP_TIME_MAINFUNCTION_ENABLED)
        if (CanTp_Timer < (CANTP_TIME_OVERFLOW - CANTP_MAIN_FUNCTION_PERIOD))
        {
            CanTp_Timer += CANTP_MAIN_FUNCTION_PERIOD;
        }
        else
        {
            CanTp_Timer = CANTP_MAIN_FUNCTION_PERIOD - (CANTP_TIME_OVERFLOW - CanTp_Timer);
        }
#endif /* STD_ON == CANTP_TIME_MAINFUNCTION_ENABLED */
#if (STD_ON == CANTP_RX_QUEUE)
        if (CanTp_RxQueueCurrentIndex > 1u)
        {
            SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
            for (index = 0; index < CanTp_RxQueueCurrentIndex; index++)
            {
                tempQueue[index] = CanTp_RxQueue[index];
                tempQueueSize++;
            }
            tempQueueIndex = tempQueueSize;
            CanTp_RxQueueCurrentIndex = 0u;
            SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
            for (ChannelId = 0; ChannelId < CanTp_ConfigPtr->ChannelNum; ChannelId++)
            {
                CanTp_AddRxQueue(tempQueue, ChannelId, &tempQueueIndex);
            }
        }
        for (ChannelId = 0; ChannelId < CanTp_ConfigPtr->ChannelNum; ChannelId++)
        {
            index = (tempQueueSize > 0u) ? tempQueue[ChannelId] : ChannelId;
            connectionChannel = &CanTp_Channels[index];
#else  /* STD_ON == CANTP_RX_QUEUE */
        for (index = 0; index < CanTp_ConfigPtr->ChannelNum; index++)
        {
            connectionChannel = &CanTp_Channels[index];
#endif /* STD_ON == CANTP_RX_QUEUE */
            /*handle channel timers*/
            CanTp_HandleTimers(connectionChannel);

#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
            /*handle channel diag gateway timers*/
            CanTp_HandleGatewayTimers(connectionChannel);
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */

            /*handle receive events*/
            CanTp_DispatchRxEvent(&connectionChannel->RxConnection);

            /*handle transmit events*/
            CanTp_DispatchTxEvent(connectionChannel);
        }
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

#if (CANTP_VERSION_INFO_API == STD_ON)
/*
 * Brief               This function return the version information of the CANTP module.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     versioninfo indicator as to where to store the
 * version information of this module.
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
CanTp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) versioninfo)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        /*report development error*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_GETVERSIONINFO, CANTP_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    {
        /*NULL_PTR != versioninfo*/
        versioninfo->moduleID = CANTP_MODULE_ID;
        versioninfo->sw_major_version = CANTP_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CANTP_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CANTP_SW_PATCH_VERSION;
        versioninfo->vendorID = CANTP_VENDOR_ID;
    }
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
#endif /* CANTP_VERSION_INFO_API == STD_ON */

/*
 * Brief               This function is called by the CAN Interface after a successful
 *                     reception of a RX CAN L-PDU.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      RxPduId the received N-PDU ID
 *                     PduInfoPtr indicator of structure with received L-SDU(payload) and data length
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CAN Interface Receiving handling
 */
#define CANTP_START_SEC_CANTPRXINDICATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE)
CanTp_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr)
{
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the reception indication only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_UNINIT);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        /*INVALID pointer parameter*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    {
        /*fill the received NPDU into channel*/
        CanTp_RxSubDeal(RxPduId, PduInfoPtr);
    }
}
#define CANTP_STOP_SEC_CANTPRXINDICATION_CALLBACK_CODE
#include "CanTp_MemMap.h"

/*
 * Brief               All transmitted CAN frames belonging to the CAN Transport
 *                     Layer will be confirmed by this function.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId ID of CAN L-PDU that has been transmitted
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        CanTp initialized
 * CallByAPI           CAN Interface transmitting confirmation
 */
#define CANTP_START_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
FUNC(void, CANTP_APPL_CODE) CanTp_TxConfirmation(PduIdType TxPduId)
{
    uint8 ChannelId;
    const CanTp_TxNSduType* txNSduCfgPtr = CanTp_GetTxSduCfgByTxNPdu(TxPduId, &ChannelId);
    CanTp_ConnectionType* TxSubchannelPtr;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    /*check module state, handle the reception indication only when module started*/
    if (CANTP_ON != CanTp_ModuleState)
    {
        /*CanTp module not started*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_UNINIT);
    }
    else
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    {
        /*first check if it is a transmitted data NPDU*/
        if (NULL_PTR != txNSduCfgPtr)
        {
            TxSubchannelPtr = CANTP_TXSUBCHANNEL(ChannelId);
            /*A transmitted data PDU confirmed, Store the confirmation event only when the
            channel related with this TX SDU is transmitting this SDU and
            it is waiting for the confirmation*/
            if (((CANTP_TX_SF_V4SF_CFM == TxSubchannelPtr->Substate)
                 || (CANTP_TX_LARGE_V4DATA_CFM == TxSubchannelPtr->Substate))
                && (txNSduCfgPtr->TxNSduId == TxSubchannelPtr->NSduId))
            {
                CanTp_SFCFTxConfirmationSubDealWith(&CanTp_Channels[ChannelId], txNSduCfgPtr);
            }
            else
            {
                /*then check if it is a transmitted FC NPDU*/
                CanTp_FCTxConfirmationSubDealWith(TxPduId);
            }
        } /*END OF handling data frame confirmation*/
        else
        {
            CanTp_FCTxConfirmationSubDealWith(TxPduId);
        }
    }
}
#define CANTP_STOP_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"

/*=============Module Internal Function Implementation====================*/
/*All transmitted CAN frames belonging to the CAN Transport Layer will be confirmed by this
 * function.*/
#define CANTP_START_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"
CANTP_LOCAL FUNC(void, CANTP_APPL_CODE) CanTp_SFCFTxConfirmationSubDealWith(
    CanTp_ConnectionChannelType* ChannelPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) txNSduCfgPtr)
{
    CanTp_ChannelTimerType* channelTimer;
    CanTp_STminType* STminTimer;
    CanTp_ConnectionType* TxSubchannelPtr = &ChannelPtr->TxConnection.Connection;
    /*A data frame transmit confirmed*/
    if (CANTP_TX_SF_V4SF_CFM == TxSubchannelPtr->Substate)
    {
        /* change tx RootState to idle , avoid release call the txconfiramation with E_NOT_OK*/
        TxSubchannelPtr->RootState = CANTP_IDLE;
        /*Confirmation for SF data transmit. The end of the SF transmitting, Release the channel
         * resource*/
        CanTp_ReleaseTxChannel(TxSubchannelPtr);
        /*Notify the upper*/
        PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_OK);
    }
    else /*Confirmation for large SDU data transmit*/
    {
        /*update next SN number*/
        TxSubchannelPtr->CurrentCfSn = (uint8)(TxSubchannelPtr->CurrentCfSn + 1u) & CANTP_CF_SN_MASK;
        if (CANTP_FTYPE_FF == TxSubchannelPtr->HandleType)
        {
            /*confirmation for FF, transit to state V4FC and start BS timer*/
            TxSubchannelPtr->Substate = CANTP_TX_LARGE_V4FC;
            channelTimer = &TxSubchannelPtr->ChannelTimer;
            if (txNSduCfgPtr->Nbs != NULL_PTR)
            {
                channelTimer->EnabledTimer = CANTP_NB;
                channelTimer->RemainTime = *(txNSduCfgPtr->Nbs);
                CanTp_ResetTime(&(channelTimer->StartTime));
            }
            else
            {
                channelTimer->EnabledTimer = CANTP_TIMER_NONE;
            }
        }
        else /*confirmation for CF*/
        {
            if (0u == TxSubchannelPtr->SduDataRemaining)
            {
                /* change tx RootState to idle , avoid release call the txconfiramation with
                 * E_NOT_OK*/
                TxSubchannelPtr->RootState = CANTP_IDLE;
                /*SDU transmitting finished, release resource of channel and notify the upper*/
                CanTp_ReleaseTxChannel(TxSubchannelPtr);
                PduR_CanTpTxConfirmation(txNSduCfgPtr->TxIPduId, E_OK);
            }
            else /*whole SDU transmitting not finished*/
            {
                channelTimer = &TxSubchannelPtr->ChannelTimer;
                /*update handled CF count*/
                if (0xFFu > TxSubchannelPtr->HandledCfCount)
                {
                    TxSubchannelPtr->HandledCfCount++;
                }
                if (TxSubchannelPtr->HandledCfCount == TxSubchannelPtr->CurrentBs)
                {
                    /*current block finished, wait another FC*/
                    TxSubchannelPtr->Substate = CANTP_TX_LARGE_V4FC;
                    if (txNSduCfgPtr->Nbs != NULL_PTR)
                    {
                        channelTimer->EnabledTimer = CANTP_NB;
                        channelTimer->RemainTime = *(txNSduCfgPtr->Nbs);
                        CanTp_ResetTime(&(channelTimer->StartTime));
                    }
                    else
                    {
                        channelTimer->EnabledTimer = CANTP_TIMER_NONE;
                    }
                }
                else /*current block not finished yet*/
                {
                    /*construct the next CF PCI, and transit to SENDCF, start STmin and CS*/
                    TxSubchannelPtr->Substate = CANTP_TX_LARGE_START;
                    if (txNSduCfgPtr->Ncs != NULL_PTR)
                    {
                        channelTimer->EnabledTimer = CANTP_NC;
                        channelTimer->RemainTime = *(txNSduCfgPtr->Ncs);
                        CanTp_ResetTime(&(channelTimer->StartTime));
                    }
                    else
                    {
                        channelTimer->EnabledTimer = CANTP_TIMER_NONE;
                    }
                    STminTimer = &TxSubchannelPtr->STminTimer;
                    STminTimer->Started = TRUE;
                    STminTimer->RemainTime = STminTimer->FcSTMin;
                    CanTp_ResetTime(&(STminTimer->StartTime));
#if (CANTP_STMIN_IMMEDIATE_TX_CONFIRMATION == STD_ON)
                    if (STminTimer->FcSTMin == 0u)
                    {

                        CanTp_TxHandleLargeStart(ChannelPtr);
                    }
#endif
                }
            } /*ENDOF whole SDU transmitting not finished*/
        }     /*END OF confirmation for CF*/
    }         /*END OF Confirmation for large SDU data transmit*/
}

CANTP_LOCAL FUNC(void, CANTP_APPL_CODE) CanTp_FCTxConfirmationSubDealWith(PduIdType TxPduId)
{
    uint8 ChannelId = 0u;
    const CanTp_RxNSduType* rxNSduCfgPtr = CanTp_GetRxSduCfgByTxFCNPdu(TxPduId, &ChannelId);
    CanTp_ConnectionType* RxchannelPtr;
    CanTp_ChannelTimerType* channelTimer;

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    if (NULL_PTR == rxNSduCfgPtr)
    {
        /*invalid confirmation*/
        CANTP_DET_REPORTERROR(CANTP_SERVICEID_TXCONFIRMATION, CANTP_E_INVALID_TX_ID);
    }
    else
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
    {
        RxchannelPtr = CANTP_RXCHANNEL(ChannelId);
        /*A transmitted FC PDU confirmed,Store the confirmation event only when the channel related
        with this RX SDU is receiving this SDU and it is waiting for the FC confirmation*/
        if (((CANTP_RX_LARGE_V4FCCTS_CFM == RxchannelPtr->Substate)
             || (CANTP_RX_LARGE_V4FCWT_CFM == RxchannelPtr->Substate))
            && (rxNSduCfgPtr->RxNSduId == RxchannelPtr->NSduId))
        {
            channelTimer = &RxchannelPtr->ChannelTimer;
            /*A data frame transmit confirmed*/
            if (CANTP_RX_LARGE_V4FCWT_CFM == RxchannelPtr->Substate)
            {
                /*transit to RX_LARGE_START, which will try to get buffer again*/
                RxchannelPtr->Substate = CANTP_RX_LARGE_START;
#if (STD_ON == CANTP_RX_QUEUE)
                CanTp_AddRxQueue(CanTp_RxQueue, ChannelId, &CanTp_RxQueueCurrentIndex);
#endif /* STD_ON == CANTP_RX_QUEUE */
                channelTimer->EnabledTimer = CANTP_NB;
                channelTimer->RemainTime = rxNSduCfgPtr->Nbr;
                CanTp_ResetTime(&(channelTimer->StartTime));
            }
            else if (CANTP_RX_LARGE_V4FCCTS_CFM == RxchannelPtr->Substate)
            {
                /*FC_CTS sent, start to waiting for CFs*/
                RxchannelPtr->Substate = CANTP_RX_LARGE_V4CF;
                if (rxNSduCfgPtr->Ncr != NULL_PTR)
                {
                    channelTimer->EnabledTimer = CANTP_NC;
                    channelTimer->RemainTime = *(rxNSduCfgPtr->Ncr);
                    CanTp_ResetTime(&(channelTimer->StartTime));
                }
                else
                {
                    channelTimer->EnabledTimer = CANTP_TIMER_NONE;
                }
            }
            else
            {
                /*will not happen with handling of filtering when indicated by CanIf*/
            }
        }
    }
}
#define CANTP_STOP_SEC_CANTPTXCONFIRMATION_CALLBACK_CODE
#include "CanTp_MemMap.h"

#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
/*Initialize the specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    CanTp_InitSubChannel(P2VAR(CanTp_ConnectionType, AUTOMATIC, CANTP_VAR_POWER_ON_INIT) SubchannelPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    CanTp_STminType* STminTimer = &SubchannelPtr->STminTimer;
    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
    /*initialize channel state information of RxConnection*/
    SubchannelPtr->RootState = CANTP_IDLE;
    SubchannelPtr->Substate = CANTP_SUBSTATE_NONE;
    SubchannelPtr->HandleType = CANTP_FTYPE_RESEVED;
    /*initialize channel ABC timer*/
    SubchannelPtr->ChannelTimer.EnabledTimer = CANTP_TIMER_NONE;
    SubchannelPtr->ChannelTimer.RemainTime = 0;
    SubchannelPtr->ChannelTimer.StartTime = 0;
    /*initialize channel flow control parameter*/
    SubchannelPtr->CurrentBs = 0;
#if (STD_ON == CANTP_FD)
    SubchannelPtr->FDDlc = 0;
#endif /* STD_ON == CANTP_FD */
    SubchannelPtr->ChangedBs = 0;
    SubchannelPtr->ChangedSTmin = 0;
    SubchannelPtr->CurrentCfSn = 0;
    SubchannelPtr->HandledCfCount = 0;
    SubchannelPtr->SentWftCount = 0;
    STminTimer->Started = FALSE;
    STminTimer->RemainTime = 0;
    STminTimer->StartTime = 0;
    STminTimer->FcSTMin = 0;
    /*initialize SDU information*/
    SubchannelPtr->NSduId = 0;
    SubchannelPtr->SduDataRemaining = 0;
    SubchannelPtr->SduDataTotalCount = 0;
    /*initialize event flags*/
    SubchannelPtr->EventFlags = 0;
#if ((CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON) || (CANTP_DYN_ID_SUPPORT == STD_ON))
    /*initialize address information of SDU with MetaData*/
    SubchannelPtr->MetaDataN_TA = 0;
    SubchannelPtr->MetaDataN_SA = 0;
    SubchannelPtr->MetaDataN_AE = 0;
    SubchannelPtr->MetaDataLength = 0;
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON) || (CANTP_DYN_ID_SUPPORT == STD_ON */
    SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
}

/*Initialize the Tx specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_InitTxChannel(CanTp_ConnectionType* TxSubchannelPtr)
{
    CanTp_TxConnectionType* TxchannelPtr = CANTP_TXCHANNEL(TxSubchannelPtr->ChannelId);
    uint8 unusedchannelId;
    const CanTp_TxNSduType* txSduCfgPtr;
#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
    CanTp_GatewayTimerType* NeedRes = &TxchannelPtr->NeedRes;
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */

    if (TxSubchannelPtr->RootState != CANTP_IDLE)
    {
        txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(TxSubchannelPtr->NSduId, &unusedchannelId);
        if (txSduCfgPtr != NULL_PTR)
        {
            PduR_CanTpTxConfirmation(txSduCfgPtr->TxIPduId, E_NOT_OK);
        }
    }
    CanTp_InitSubChannel(TxSubchannelPtr);
    for (uint8 iloop = 0; iloop <
#if (STD_ON == CANTP_FD)
                          CANTP_CANFD_FRAME_LEN_MAX
#else  /* STD_ON == CANTP_FD */
                          CANTP_CAN20_FRAME_LEN_MAX
#endif /* STD_ON == CANTP_FD */
         ;
         iloop++)
    {
        TxchannelPtr->LocalBuf[iloop] = 0;
    }
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    for (uint8 iloop = 0; iloop < 5u; iloop++)
    {
        TxchannelPtr->Metadata[iloop] = 0;
    }
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
    /*initialize channel diag gateway timer*/
    NeedRes->Started = FALSE;
    NeedRes->StartTime = 0;
    NeedRes->RemainTime = 0;
    NeedRes->ResponseTimer = 0;
    NeedRes->Sid = 0;
    NeedRes->PendingSid = 0;
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */
}

/*Initialize the specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_InitChannel(CanTp_ConnectionChannelType* connectionChannel)
{
    CanTp_InitSubChannel(&connectionChannel->RxConnection);
    CanTp_InitTxChannel(&connectionChannel->TxConnection.Connection);
}

CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_ReleaseRxChannel(CanTp_ConnectionType* RxChannelPtr)
{
    uint8 ChannelId = RxChannelPtr->ChannelId;
    if (CanTp_ConfigPtr->CanTpChannel[ChannelId].ChannelMode != CANTP_MODE_FULL_DUPLEX)
    {
        CanTp_InitChannel(&CanTp_Channels[ChannelId]);
    }
    else
    {
        CanTp_InitSubChannel(RxChannelPtr);
    }
}

CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_ReleaseTxChannel(CanTp_ConnectionType* TxSubchannelPtr)
{
    uint8 ChannelId = TxSubchannelPtr->ChannelId;
    if (CanTp_ConfigPtr->CanTpChannel[ChannelId].ChannelMode != CANTP_MODE_FULL_DUPLEX)
    {
        CanTp_InitChannel(&CanTp_Channels[ChannelId]);
    }
    else
    {
        CanTp_InitTxChannel(TxSubchannelPtr);
    }
}

#if (STD_ON == CANTP_FD)
/*
 * Brief               match CanFd Data length.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(PduLengthType, CANTP_CODE) CanTp_MatchDLC(PduLengthType frameDL)
{
    uint8 i;
    PduLengthType len = frameDL;

    /*ISO 11898-1:2014 define the DLC table*/
    const uint8 canDL[8] = {8, 12, 16, 20, 24, 32, 48, 64};
    if (frameDL > CANTP_CAN20_FRAME_LEN_MAX)
    {
        for (i = 0; i < 8u; i++)
        {
            if (frameDL <= canDL[i])
            {
                len = canDL[i];
                break;
            }
        }
    }
    return len;
}
#endif /* STD_ON == CANTP_FD */

/*This function is called by the CanTp_RxIndication after a successful
reception of a RX CAN L-PDU, put the PDU into pool.*/
CANTP_LOCAL FUNC(void, CANTP_CODE)
    CanTp_RxSubDeal(PduIdType CanTpRxPduId, P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr)
{
    uint8 chnIdx;
    CanTp_FramePCIType FrameType = CANTP_FTYPE_RESEVED;
    /*first try to check if the received NPDU whether is CF SF or FF*/
    const CanTp_RxNSduType* rxNSduCfgPtr = CanTp_GetRxSduCfgByNpdu(CanTpRxPduId, PduInfoPtr, &FrameType, &chnIdx);
    const CanTp_TxNSduType* txNSduCfgPtr;

    if (NULL_PTR != rxNSduCfgPtr)
    {
        switch (FrameType)
        {
        case CANTP_FTYPE_CF:
            CanTp_RxSubDealWithCF(PduInfoPtr, rxNSduCfgPtr, CANTP_RXCHANNEL(chnIdx));
            break;
        case CANTP_FTYPE_FF:
            CanTp_RxSubDealWithFF(PduInfoPtr, rxNSduCfgPtr, &CanTp_Channels[chnIdx]);
            break;
        case CANTP_FTYPE_SF:
            CanTp_RxSubDealWithSF(PduInfoPtr, rxNSduCfgPtr, &CanTp_Channels[chnIdx]);
            break;
        default:
            /*this will never happen*/
            break;
        }
    }

    if (CANTP_FTYPE_RESEVED == FrameType)
    {
        /*then check whether it is FC, if FC received that means this is sender*/
        txNSduCfgPtr = CanTp_GetTxSduCfgByFCNpdu(CanTpRxPduId, PduInfoPtr, &chnIdx);
        if (NULL_PTR != txNSduCfgPtr)
        {
            /*A FC received*/
            CanTp_RxSubDealWithFC(PduInfoPtr, txNSduCfgPtr, CANTP_TXSUBCHANNEL(chnIdx));
        }
    }
}

/*
 * Brief               FC come, just execute one time of PduR_CanTpCopyRxData.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithFC(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    CanTp_ConnectionType* TxSubchannelPtr)
{
    CanTp_ChannelTimerType* channelTimer;
    CanTp_STminType* STminTimer;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    Std_ReturnType result;
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
    CanTp_FCInfoType fcInfo;
#if (STD_ON == CANTP_FD)
    PduLengthType MatchDLC = CanTp_MatchDLC(PduInfoPtr->SduLength);
#endif /* STD_ON == CANTP_FD */
    /*min-length check*/
    if ((PduInfoPtr->SduLength >= 3u)
#if (STD_ON == CANTP_FD)
        && ((MatchDLC == PduInfoPtr->SduLength) || (PduInfoPtr->SduLength < 8u))
        && (PduInfoPtr->SduLength <= TxNSduCfgPtr->TxNPduDLC)
#endif /* STD_ON == CANTP_FD */
    )
    {
        /*only accepted the received FC when the channel is transmitting a large SDU related with
        the FC NPDU, and it is waiting for a FC. Otherwise ignore the frame*/
        if ((TxNSduCfgPtr->TxNSduId == TxSubchannelPtr->NSduId) && (CANTP_TX_LARGE_V4FC == TxSubchannelPtr->Substate)
            && (CANTP_LARGE_TRANSMITTING == TxSubchannelPtr->RootState))
        {
            if ((8u > PduInfoPtr->SduLength) && (TxNSduCfgPtr->TxPaddingActivation == CANTP_PADDING_ON))
            {
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PADDING);
                CanTp_ReleaseTxChannel(TxSubchannelPtr);
            }
            else
            {
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                /*SWS_CanTp_00336,the CanTp module shall check the addressing information contained
                  in the MetaData (at the end of the data referenced by PduInfoPtr) against the stored values.*/
                result = CanTp_CheckRxFCMetaData(TxSubchannelPtr, TxNSduCfgPtr, PduInfoPtr);
                if (result == E_OK)
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
                {
                    CanTp_GetChannelFCInfo(TxNSduCfgPtr, PduInfoPtr, &fcInfo);

                    switch (fcInfo.FcFs)
                    {
                    case CANTP_FC_FS_CTS:
                        /*Clear to send. Get the channel ready to send CF, and make STmin timer timeout*/
                        SchM_Enter_CanTp_ExclusiveArea(); /*unlock channel*/
                        TxSubchannelPtr->CurrentBs = fcInfo.FcBS;
                        TxSubchannelPtr->HandledCfCount = 0;
                        STminTimer = &TxSubchannelPtr->STminTimer;
                        STminTimer->Started = TRUE;
                        STminTimer->RemainTime = fcInfo.FcSTMin;
                        STminTimer->FcSTMin = fcInfo.FcSTMin;
                        channelTimer = &TxSubchannelPtr->ChannelTimer;
                        CanTp_ResetTime(&(STminTimer->StartTime));
                        SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/

                        /*transit to state SENDCF*/
                        if (TxNSduCfgPtr->Ncs != NULL_PTR)
                        {
                            SchM_Enter_CanTp_ExclusiveArea(); /*unlock channel*/
                            channelTimer->EnabledTimer = CANTP_NC;
                            channelTimer->RemainTime = *(TxNSduCfgPtr->Ncs);
                            CanTp_ResetTime(&(channelTimer->StartTime));
                            SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
                        }
                        else
                        {
                            channelTimer->EnabledTimer = CANTP_TIMER_NONE;
                        }
                        SchM_Enter_CanTp_ExclusiveArea();
                        TxSubchannelPtr->HandleType = CANTP_FTYPE_CF;
                        TxSubchannelPtr->Substate = CANTP_TX_LARGE_START;
                        SchM_Exit_CanTp_ExclusiveArea();
                        break;
                    case CANTP_FC_FS_WT:
                        channelTimer = &TxSubchannelPtr->ChannelTimer;
                        /*reset the BS timer*/
                        if (TxNSduCfgPtr->Nbs != NULL_PTR)
                        {
                            SchM_Enter_CanTp_ExclusiveArea();
                            channelTimer->EnabledTimer = CANTP_NB;
                            channelTimer->RemainTime = *(TxNSduCfgPtr->Nbs);
                            CanTp_ResetTime(&(channelTimer->StartTime));
                            SchM_Exit_CanTp_ExclusiveArea();
                        }
                        else
                        {
                            channelTimer->EnabledTimer = CANTP_TIMER_NONE;
                        }
                        break;
                    case CANTP_FC_FS_OVFLW:
                    default:
                        /* change tx RootState to idle ,
                        avoid release call the txconfiramation with E_NOT_OK*/
                        TxSubchannelPtr->RootState = CANTP_IDLE;
                        /*release resource of channel*/
                        CanTp_ReleaseTxChannel(TxSubchannelPtr);
                        /*notify upper with NO_BUFFER*/
                        PduR_CanTpTxConfirmation(TxNSduCfgPtr->TxIPduId, E_NOT_OK);
                        break;
                    } /*ENDOF switch (fcInfo.FcFs)*/
                }
            }
        }
    }
}
/*
 * Brief               CF come, just execute one time of PduR_CanTpCopyRxData.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithCF(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionType* RxchannelPtr)
{
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    Std_ReturnType result;
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
#if (STD_ON == CANTP_FD)
    PduLengthType MatchDLC = CanTp_MatchDLC(PduInfoPtr->SduLength);
#endif /* STD_ON == CANTP_FD */
    uint8 cfSN;
    uint8 pcioffset;
    uint8 dataoffset;

    /*min-length check*/
    /*only accept it when the channel is receiving a large
    SDU related with this CF, and it is waiting a CF.*/
    if ((PduInfoPtr->SduLength >= 1u) && (CANTP_FUNCTIONAL_RX != RxNSduCfgPtr->RxTaType)
        && (RxNSduCfgPtr->RxNSduId == RxchannelPtr->NSduId) && (CANTP_LARGE_RECEIVING == RxchannelPtr->RootState)
        && (CANTP_RX_LARGE_V4CF == RxchannelPtr->Substate) && (PduInfoPtr->SduLength <= RxNSduCfgPtr->RxNPduDLC)
#if (STD_ON == CANTP_FD)
        && ((PduInfoPtr->SduLength == MatchDLC) || (PduInfoPtr->SduLength < 8u))
        && (CANTP_CANFD_FUNCTIONAL != RxNSduCfgPtr->RxTaType)
#endif /* STD_ON == CANTP_FD */
    )
    {
        if ((8u > PduInfoPtr->SduLength) && (RxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_ON))
        {
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
            CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PADDING);
#endif /* STD_ON == CANTP_RUNTIME_ERROR_DETECT */
            PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
            CanTp_ReleaseRxChannel(RxchannelPtr);
        }
        else
        {
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
            /*SWS_CanTp_00333,check the addressing information contained in the MetaData of the
            N-PDU (at the end of the data referenced by PduInfoPtr) against the stored values from the FF.*/
            result = CanTp_CheckRxCFMetaData(RxchannelPtr, RxNSduCfgPtr, PduInfoPtr);
            if (E_OK == result)
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
            {
                CanTp_RxGetCFOffset(RxNSduCfgPtr, &dataoffset, &pcioffset);
                cfSN = PduInfoPtr->SduDataPtr[pcioffset] & CANTP_CF_SN_MASK;
                if (cfSN != RxchannelPtr->CurrentCfSn)
                {
                    PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                    /*check SN value*/ /*wrong SN*/ /*release resource of channel*/
                    CanTp_ReleaseRxChannel(RxchannelPtr);
                }
                else
                {
                    CanTp_RxSubDealWithCFToUp(PduInfoPtr, RxNSduCfgPtr, RxchannelPtr);
                } /*END OF handling expected CF*/
            }
        }
    }
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
    else if (
        (CANTP_FUNCTIONAL_RX == RxNSduCfgPtr->RxTaType)
#if (STD_ON == CANTP_FD)
        || (CANTP_CANFD_FUNCTIONAL == RxNSduCfgPtr->RxTaType)
#endif /* STD_ON == CANTP_FD */
    )
    {
        /*invalid TAType with CF frame, wrong configuration*/
        CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_INVALID_TATYPE);
    }
    else
    {
        /* idle */
    }
#endif /* STD_ON == CANTP_RUNTIME_ERROR_DETECT */
}

/*
 * Brief               CF come, just execute one time of PduR_CanTpCopyRxData.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithCFToUp(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionType* RxchannelPtr)
{
    uint32 N_Cr;
    PduInfoType upperpduInfo;
    const PduInfoType* upperpduInfoPtr = &upperpduInfo;
    PduLengthType bufferSize;
    Std_ReturnType result;
    BufReq_ReturnType bufRslt;
    PduLengthType DataLength;
    uint8 pcioffset;
    uint8 dataoffset;
    CanTp_ChannelTimerType* channelTimer = &RxchannelPtr->ChannelTimer;

    /*It is the expected CF*/
    CanTp_RxGetCFOffset(RxNSduCfgPtr, &dataoffset, &pcioffset);
    DataLength = PduInfoPtr->SduLength - dataoffset;
    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
    RxchannelPtr->HandleType = CANTP_FTYPE_CF;
    channelTimer->EnabledTimer = CANTP_TIMER_NONE;
    if (0xFFu > RxchannelPtr->HandledCfCount)
    {
        RxchannelPtr->HandledCfCount++;
    }
    /*consider rx padding*/
    if (DataLength > RxchannelPtr->SduDataRemaining)
    {
        DataLength = RxchannelPtr->SduDataRemaining;
    }
    SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/

    upperpduInfo.SduDataPtr = &(PduInfoPtr->SduDataPtr[dataoffset]);
    upperpduInfo.SduLength = DataLength;
    bufRslt = PduR_CanTpCopyRxData(RxNSduCfgPtr->RxIPduId, upperpduInfoPtr, &bufferSize);
    if (BUFREQ_E_NOT_OK == bufRslt)
    {
        PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
        /*release resource of channel*/
        CanTp_ReleaseRxChannel(RxchannelPtr);
    }
    else
    {
        RxchannelPtr->SduDataRemaining -= DataLength;
        if (((DataLength < (PduLengthType)((PduLengthType)8u - (PduLengthType)dataoffset))
#if (STD_ON == CANTP_FD)
             || (PduInfoPtr->SduLength != RxchannelPtr->FDDlc)
#endif /* STD_ON == CANTP_FD */
                 )
            && (0u != RxchannelPtr->SduDataRemaining))
        {
            PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
            /*this CF is not the last and length is not match ,release resource of channel*/
            CanTp_ReleaseRxChannel(RxchannelPtr);
        }
        else
        {
            if (0u == RxchannelPtr->SduDataRemaining)
            {
                /*the whole SDU reception finished*/
#if (STD_ON == CANTP_FD)
                if (FALSE == CanTp_checkLastCF(DataLength, PduInfoPtr->SduLength, pcioffset))
                {
                    PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                    /*release resource of channel*/
                    CanTp_ReleaseRxChannel(RxchannelPtr);
                }
                else
#endif /* STD_ON == CANTP_FD */
                {
#if (STD_ON == CANTP_SYNCHRONOUS_RXINDICATION)
                    RxchannelPtr->RootState = CANTP_OCCUPIED;
                    PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_OK);
                    RxchannelPtr->RootState = CANTP_IDLE;
                    CanTp_ReleaseRxChannel(RxchannelPtr);
#else /* STD_ON == CANTP_SYNCHRONOUS_RXINDICATION */
                    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                    CANTP_SETFLAG(RxchannelPtr->EventFlags, CANTP_EVENT_RXNOTIFIUPPER);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#if (STD_ON == CANTP_RX_QUEUE)
                    CanTp_AddRxQueue(CanTp_RxQueue, RxchannelPtr->ChannelId, &CanTp_RxQueueCurrentIndex);
#endif /* STD_ON == CANTP_RX_QUEUE */
#endif /* STD_ON == CANTP_SYNCHRONOUS_RXINDICATION */
                }
            }
            else
            {
                /*SDU whole reception not finished*/
                if (RxchannelPtr->CurrentBs == RxchannelPtr->HandledCfCount)
                {
                    /*current block finished, check upper buffer is enough or not*/
                    result = CanTp_CompareBufferWithBs(RxNSduCfgPtr, RxchannelPtr, bufferSize);
                    if (E_NOT_OK == result)
                    {
                        /*the upper buffer is not enough to store the next block,so need to get more buffer*/
                        SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
                        RxchannelPtr->Substate = CANTP_RX_LARGE_START;
                        channelTimer->EnabledTimer = CANTP_NB;
                        channelTimer->RemainTime = RxNSduCfgPtr->Nbr;
                        CanTp_ResetTime(&(channelTimer->StartTime));
                        SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
#if (STD_ON == CANTP_RX_QUEUE)
                        CanTp_AddRxQueue(CanTp_RxQueue, RxchannelPtr->ChannelId, &CanTp_RxQueueCurrentIndex);
#endif /* STD_ON == CANTP_RX_QUEUE */
                    }
                    else
                    {
                        /*buffer enough, start a new block*/ /*calculate block size*/
                        CanTp_CalcBS(RxNSduCfgPtr, RxchannelPtr);
                        result = CanTp_RxBlockStart(RxNSduCfgPtr, RxchannelPtr);
                        if (E_NOT_OK == result)
                        {
                            PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                            CanTp_ReleaseRxChannel(RxchannelPtr);
                        }
                    }
                }
                else
                {
                    /*continue to wait another CF*/

                    RxchannelPtr->Substate = CANTP_RX_LARGE_V4CF;
                    if (RxNSduCfgPtr->Ncr != NULL_PTR)
                    {
                        SchM_Enter_CanTp_ExclusiveArea();
                        N_Cr = *(RxNSduCfgPtr->Ncr);
                        channelTimer->EnabledTimer = CANTP_NC;
                        channelTimer->RemainTime = N_Cr;
                        CanTp_ResetTime(&(channelTimer->StartTime));
                        SchM_Exit_CanTp_ExclusiveArea();
                    }
                    else
                    {
                        channelTimer->EnabledTimer = CANTP_TIMER_NONE;
                    }
                }
                /*update SN,expect next CF*/
                RxchannelPtr->CurrentCfSn = (RxchannelPtr->CurrentCfSn + 1u) & CANTP_CF_SN_MASK;
            } /*END OF SDU whole reception not finished*/
        }
    } /*END OF if (BUFREQ_E_NOT_OK == bufRslt)*/
}

/*
 * Brief               FF come, just execute one time of PduR_CanTpStartOfReception.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithFF(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionChannelType* connectionChannel)
{
    uint8 Offset = 0;
    PduLengthType frameDl;
    const CanTp_ConnectionType* TxSubchannelPtr = &connectionChannel->TxConnection.Connection;
    CanTp_ConnectionType* RxchannelPtr = &connectionChannel->RxConnection;
    CanTp_ChannelTimerType* channelTimer;
#if (STD_ON == CANTP_FD)
    PduLengthType MatchDLC = CanTp_MatchDLC(PduInfoPtr->SduLength);
#endif /* STD_ON == CANTP_FD */
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    Std_ReturnType result;
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

    /*min-length check*/
    /*half duplex, if Tx is in processing, the same channel can not receiving data at the same time*/
    /*Only accept FF when the channel is IDLE or it is handling SDU receiving*/
    /*Function addressing mode not support multiple frames*/
    if ((PduInfoPtr->SduLength >= 8u)
        && ((CanTp_ConfigPtr->CanTpChannel[TxSubchannelPtr->ChannelId].ChannelMode != CANTP_MODE_HALF_DUPLEX)
            || (TxSubchannelPtr->RootState == CANTP_IDLE))
        && (CANTP_FUNCTIONAL_RX != RxNSduCfgPtr->RxTaType)
        && ((CANTP_IDLE == RxchannelPtr->RootState) || (CANTP_LARGE_RECEIVING == RxchannelPtr->RootState))
        && (PduInfoPtr->SduLength <= RxNSduCfgPtr->RxNPduDLC)
#if (STD_ON == CANTP_FD)
        && ((PduInfoPtr->SduLength == MatchDLC) || (PduInfoPtr->SduLength <= 8u))
        && (CANTP_CANFD_FUNCTIONAL != RxNSduCfgPtr->RxTaType)
#endif /* STD_ON == CANTP_FD */
    )
    {
        /*check data length of the FF, ignore the frame if invalid length*/
        frameDl = CanTp_RxGetFFDl((const uint8*)PduInfoPtr->SduDataPtr, &Offset, RxNSduCfgPtr);

#if (STD_ON == CANTP_FD)
        /* If CAN_DL > 8, SF needs one extra offset as the escape sequence */
        if (PduInfoPtr->SduLength > 8u)
        {
            Offset++;
        }
#endif /* STD_ON == CANTP_FD */
        /*+ 1u means this FF can be send by SF*/
        if (frameDl > (PduInfoPtr->SduLength - (PduLengthType)Offset + 1u))
        {
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
            result = CanTp_SaveRxMetaDataInfo(RxchannelPtr, RxNSduCfgPtr, PduInfoPtr);
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
            if ((CANTP_LARGE_RECEIVING == RxchannelPtr->RootState) && (RxchannelPtr->MetaDataLength >= 4u)
                && (CANTP_CAN_ID_32_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
            {
                /*When an SF or FF N-PDU with MetaData (indicating
                 a generic connection) is received,
                 * and the corresponding connection channel is
                  currently receiving, the SF or FF shall be ignored.*/
            }
            else
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
                if (E_OK == result)
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
                {
                    if ((CANTP_SF_RECEIVING == RxchannelPtr->RootState)
                        || (CANTP_LARGE_RECEIVING == RxchannelPtr->RootState))
                    {
                        /*Another SF or large received when the channel is receiving a SDU*/
                        /*Notify error for old SDU reception*/
                        PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                    }
                    /*update channel runtime information*/
                    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
                    RxchannelPtr->RootState = CANTP_LARGE_RECEIVING;
                    RxchannelPtr->Substate = CANTP_RX_FF_START;
                    RxchannelPtr->NSduId = RxNSduCfgPtr->RxNSduId;
                    RxchannelPtr->HandleType = CANTP_FTYPE_FF;
                    RxchannelPtr->SduDataRemaining = frameDl;
                    RxchannelPtr->SduDataTotalCount = frameDl;
                    /*initialize Flow control information*/
                    RxchannelPtr->CurrentBs = 0;
#if (STD_ON == CANTP_FD)
                    RxchannelPtr->FDDlc = MatchDLC;
#endif /* STD_ON == CANTP_FD */
                    RxchannelPtr->CurrentCfSn = 0;
                    RxchannelPtr->HandledCfCount = 0;
                    RxchannelPtr->SentWftCount = 0;
                    channelTimer = &RxchannelPtr->ChannelTimer;
                    /*SWS_CanTp_00166,the CanTp module shall start a
                    time-out N_Br before calling PduR_CanTpStartOfReception*/
                    channelTimer->EnabledTimer = CANTP_NB;
                    channelTimer->RemainTime = RxNSduCfgPtr->Nbr;
                    CanTp_ResetTime(&(channelTimer->StartTime));
                    SchM_Exit_CanTp_ExclusiveArea();

                    CanTp_RxSubDealWithFFToUp(PduInfoPtr, RxNSduCfgPtr, connectionChannel);
                }
#if ((CANTP_DYN_ID_SUPPORT == STD_ON) && (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON))
                else
                {
                    /*idle*/
                }
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON && CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
        }
    }
#if (STD_ON == CANTP_DEV_ERROR_DETECT)
    else if (
        (CANTP_FUNCTIONAL_RX == RxNSduCfgPtr->RxTaType)
#if (STD_ON == CANTP_FD)
        || (CANTP_CANFD_FUNCTIONAL == RxNSduCfgPtr->RxTaType)
#endif /* STD_ON == CANTP_FD */
    )
    {
        /*invalid TAType with FF frame, wrong configuration*/
        CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_INVALID_TATYPE);
    }
    else
    {
        /* idle */
    }
#endif /* STD_ON == CANTP_DEV_ERROR_DETECT */
}

/*
 * Brief               FF come, just execute one time of PduR_CanTpStartOfReception.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithFFToUp(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionChannelType* connectionChannel)
{
    CanTp_ConnectionType* RxchannelPtr = &connectionChannel->RxConnection;
    CanTp_FCInfoType CanTp_FC;
    uint8 Offset = 0;
    PduLengthType frameDl;
    PduLengthType bufferSize;
    BufReq_ReturnType bufRslt;
    PduInfoType upperpduInfo;
    Std_ReturnType result;
    const PduInfoType* upperpduInfoPtr = &upperpduInfo;
    CanTp_ChannelTimerType* ChannelTimer;
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 data[5] = {0, 0, 0, 0, 0};

    upperpduInfo.MetaDataPtr = data;
    if (((RxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK) > 0u)
        && (CANTP_SA16_AND_TA16_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK)))
    {
        CanTp_ConstructForwardRxMetaData(RxchannelPtr, RxNSduCfgPtr, upperpduInfoPtr);
    }
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
    upperpduInfo.SduLength = 0;
    frameDl = CanTp_RxGetFFDl((const uint8*)PduInfoPtr->SduDataPtr, &Offset, RxNSduCfgPtr);
    bufRslt = PduR_CanTpStartOfReception(RxNSduCfgPtr->RxIPduId, upperpduInfoPtr, frameDl, &bufferSize);
    switch (bufRslt)
    {
    case BUFREQ_OK:
        upperpduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[Offset];
        upperpduInfo.SduLength = PduInfoPtr->SduLength - (PduLengthType)Offset;
        bufRslt = PduR_CanTpCopyRxData(RxNSduCfgPtr->RxIPduId, upperpduInfoPtr, &bufferSize);
        if (BUFREQ_E_NOT_OK == bufRslt)
        {
            PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
            CanTp_ReleaseRxChannel(RxchannelPtr);
        }
        else
        {
#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
            CanTp_TxConnectionType* TxchannelPtr = &connectionChannel->TxConnection;
            TxchannelPtr->NeedRes.Sid = upperpduInfo.SduDataPtr[0];
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */
            RxchannelPtr->SduDataRemaining -= (PduInfoPtr->SduLength - (PduLengthType)Offset);
            /*calculate block size*/
            CanTp_CalcBS(RxNSduCfgPtr, RxchannelPtr);
            /*SWS_CanTp_00082,check upper layer buffer is enough or not for next block*/
            result = CanTp_CompareBufferWithBs(RxNSduCfgPtr, RxchannelPtr, bufferSize);
            if (E_NOT_OK == result)
            {
                SchM_Enter_CanTp_ExclusiveArea();
                /*the upper buffer is not enough to store the next block, so need to get more buffer*/
                RxchannelPtr->Substate = CANTP_RX_LARGE_START;
                ChannelTimer = &RxchannelPtr->ChannelTimer;
                ChannelTimer->EnabledTimer = CANTP_NB;
                ChannelTimer->RemainTime = RxNSduCfgPtr->Nbr;
                CanTp_ResetTime(&(ChannelTimer->StartTime));
                SchM_Exit_CanTp_ExclusiveArea();
#if (STD_ON == CANTP_RX_QUEUE)
                CanTp_AddRxQueue(CanTp_RxQueue, RxchannelPtr->ChannelId, &CanTp_RxQueueCurrentIndex);
#endif /* STD_ON == CANTP_RX_QUEUE */
            }
            else
            {
                /*buffer enough, start a new block*/
                result = CanTp_RxBlockStart(RxNSduCfgPtr, RxchannelPtr);
                if (E_NOT_OK == result)
                {
                    PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                    CanTp_ReleaseRxChannel(RxchannelPtr);
                }
            }
        }
        /*update expected next SN*/
        RxchannelPtr->CurrentCfSn = (uint8)(RxchannelPtr->CurrentCfSn + 1u) & CANTP_CF_SN_MASK;
        break;
    case BUFREQ_E_NOT_OK:
        CanTp_InitSubChannel(RxchannelPtr);
        break;
    case BUFREQ_E_OVFL:
        /*the CanTp module shall send a Flow Control N-PDU with overflow
         status (FC(OVFLW)) and abort the N-SDU reception*/
        /*when handling FF, send FC_OVFL*/
        CanTp_FC.FcFs = CANTP_FC_FS_OVFLW;
        CanTp_FC.FcBS = 0;
        CanTp_FC.FcSTMin = 0;
        (void)CanTp_SendFC(RxNSduCfgPtr, RxchannelPtr, CanTp_FC);
        /*release resource of the channel*/
        CanTp_ReleaseRxChannel(RxchannelPtr);
        break;
    default:
        /*idle*/
        break;
    }
}

/*
 * Brief               SF come, just execute one time of PduR_CanTpStartOfReception.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithSF(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionChannelType* connectionChannel)
{
    uint8 totaloffset = 0;
    PduLengthType frameDl;
    const CanTp_ConnectionType* TxSubchannelPtr = &connectionChannel->TxConnection.Connection;
    const CanTp_ConnectionType* RxchannelPtr = &connectionChannel->RxConnection;
#if (STD_ON == CANTP_FD)
    uint8 DataOffset = 0;
    PduLengthType MatchDLC = CanTp_MatchDLC(PduInfoPtr->SduLength);
#endif /* STD_ON == CANTP_FD */

    /*min-length check*/
    /*Handle FF and SF with the same strategy*/
    /*Only accept SF when the channel is IDLE or it is handling SDU receiving*/
    if ((PduInfoPtr->SduLength >= 1u)
        && ((CanTp_ConfigPtr->CanTpChannel[TxSubchannelPtr->ChannelId].ChannelMode != CANTP_MODE_HALF_DUPLEX)
            || (TxSubchannelPtr->RootState == CANTP_IDLE))
        && ((CANTP_IDLE == RxchannelPtr->RootState) || (CANTP_LARGE_RECEIVING == RxchannelPtr->RootState)))
    {
        /*check data length of the SF, ignore the frame if invalid length*/
        frameDl = CanTp_RxGetSFDl(
            PduInfoPtr,
            RxNSduCfgPtr,
#if (STD_ON == CANTP_FD)
            &DataOffset,
#endif /* STD_ON == CANTP_FD */
            &totaloffset);
#if (STD_ON == CANTP_FD)
        if (((PduInfoPtr->SduLength == MatchDLC) || (PduInfoPtr->SduLength < 8u))
            && (PduInfoPtr->SduLength <= RxNSduCfgPtr->RxNPduDLC))
#else  /* STD_ON == CANTP_FD */
        if (PduInfoPtr->SduLength <= RxNSduCfgPtr->RxNPduDLC)
#endif /* STD_ON == CANTP_FD */
        {
            if ((8u <= PduInfoPtr->SduLength) || (RxNSduCfgPtr->RxPaddingActivation != CANTP_PADDING_ON))
            {
                /*frameDl valid*/
                if (frameDl != 0x0u)
                {
#if (STD_ON == CANTP_FD)
                    if (FALSE == CanTp_checkSF(frameDl, MatchDLC, DataOffset))
                    {
                        /*No further deal*/
                    }
                    else
#endif /* STD_ON == CANTP_FD */
                        if (frameDl <= (PduInfoPtr->SduLength - totaloffset))
                        {
                            CanTp_RxSubDealWithSFToUp(PduInfoPtr, RxNSduCfgPtr, connectionChannel);
                        }
#if (STD_ON == CANTP_FD)
                        else
                        {
                            /*No further deal*/
                        }
#endif /* STD_ON == CANTP_FD */
                }
            }
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
            else
            {
                CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_RXINDICATION, CANTP_E_PADDING);
            }
#endif /* STD_ON == CANTP_RUNTIME_ERROR_DETECT */
        }
    }
}

/*
 * Brief               SF come, just execute one time of PduR_CanTpStartOfReception.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_RxSubDealWithSFToUp(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionChannelType* connectionChannel)
{
    CanTp_ConnectionType* RxchannelPtr = &connectionChannel->RxConnection;
    PduLengthType bufferSize;
    BufReq_ReturnType bufRslt;
    PduInfoType upperpduInfo;
    uint8 totaloffset = 0;
    PduLengthType frameDl;
    const PduInfoType* upperpduInfoPtr = &upperpduInfo;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    Std_ReturnType result;
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
#if (STD_ON == CANTP_FD)
    uint8 DataOffset = 0;
#endif /* STD_ON == CANTP_FD */
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 data[5] = {0, 0, 0, 0, 0};

    upperpduInfo.MetaDataPtr = data;
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */

    upperpduInfo.SduLength = 0;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    result = CanTp_SaveRxMetaDataInfo(RxchannelPtr, RxNSduCfgPtr, PduInfoPtr);
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    if ((CANTP_LARGE_RECEIVING == RxchannelPtr->RootState) && (RxchannelPtr->MetaDataLength >= 4u)
        && (CANTP_CAN_ID_32_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_CAN_ID_32_MASK)))
    {
        /*When an SF or FF N-PDU with MetaData (indicating a generic connection) is received,
         * and the corresponding connection channel is currently receiving, the SF or FF shall be ignored.*/
    }
    else
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
        if (E_OK == result)
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
        {
            frameDl = CanTp_RxGetSFDl(
                PduInfoPtr,
                RxNSduCfgPtr,
#if (STD_ON == CANTP_FD)
                &DataOffset,
#endif /* STD_ON == CANTP_FD */
                &totaloffset);

            if (((CANTP_SF_RECEIVING == RxchannelPtr->RootState) || (CANTP_LARGE_RECEIVING == RxchannelPtr->RootState))
                && (!(
                    (PduInfoPtr->SduDataPtr[totaloffset] == 0x3Eu)
                    && (PduInfoPtr->SduDataPtr[totaloffset + 1u] == 0x80u)
                    && ((RxNSduCfgPtr->RxTaType == CANTP_FUNCTIONAL_RX)
                        || (RxNSduCfgPtr->RxTaType == CANTP_CANFD_FUNCTIONAL))
                    && (frameDl == 2u))))
            {
                /*Another SF or large received when the channel is receiving a SDU*/
                /*Notify error for old SDU reception*/
                PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
            }
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
            if (((RxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK) > 0u)
                && (CANTP_SA16_AND_TA16_MASK == (RxNSduCfgPtr->CanTpPduFlag & CANTP_SA16_AND_TA16_MASK)))
            {
                CanTp_ConstructForwardRxMetaData(RxchannelPtr, RxNSduCfgPtr, upperpduInfoPtr);
            }
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
            bufRslt = PduR_CanTpStartOfReception(RxNSduCfgPtr->RxIPduId, upperpduInfoPtr, frameDl, &bufferSize);
            /*handle according the result of request RX buffer*/
            switch (bufRslt)
            {
            case BUFREQ_OK:
                upperpduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[totaloffset];
                upperpduInfo.SduLength = frameDl;
                bufRslt = PduR_CanTpCopyRxData(RxNSduCfgPtr->RxIPduId, upperpduInfoPtr, &bufferSize);
                if (bufRslt == BUFREQ_OK)
                {
                    if ((PduInfoPtr->SduDataPtr[totaloffset] == 0x3Eu)
                        && (PduInfoPtr->SduDataPtr[totaloffset + 1u] == 0x80u)
                        && ((RxNSduCfgPtr->RxTaType == CANTP_FUNCTIONAL_RX)
                            || (RxNSduCfgPtr->RxTaType == CANTP_CANFD_FUNCTIONAL))
                        && (frameDl == 2u))
                    {
                        PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_OK);
                    }
                    else
                    {
#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
                        connectionChannel->TxConnection.NeedRes.Sid = upperpduInfo.SduDataPtr[0];
#endif                                                    /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */
                        SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
                        /*update channel runtime information*/
                        RxchannelPtr->RootState = CANTP_SF_RECEIVING;
                        RxchannelPtr->Substate = CANTP_RX_SF_START;
                        RxchannelPtr->NSduId = RxNSduCfgPtr->RxNSduId;
                        RxchannelPtr->HandleType = CANTP_FTYPE_SF;
                        SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
#if (STD_ON == CANTP_SYNCHRONOUS_RXINDICATION)
                        RxchannelPtr->RootState = CANTP_OCCUPIED;
                        PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_OK);
                        RxchannelPtr->RootState = CANTP_IDLE;
                        CanTp_ReleaseRxChannel(RxchannelPtr);
#else /* STD_ON == CANTP_SYNCHRONOUS_RXINDICATION */
                    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                    CANTP_SETFLAG(RxchannelPtr->EventFlags, CANTP_EVENT_RXNOTIFIUPPER);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#if (STD_ON == CANTP_RX_QUEUE)
                    CanTp_AddRxQueue(CanTp_RxQueue, RxchannelPtr->ChannelId, &CanTp_RxQueueCurrentIndex);
#endif /* STD_ON == CANTP_RX_QUEUE */
#endif /* STD_ON == CANTP_SYNCHRONOUS_RXINDICATION */
                    }
                }
                else
                {
                    PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                    CanTp_ReleaseRxChannel(RxchannelPtr);
                }
                break;
            case BUFREQ_E_NOT_OK:
                /*Release the resource of the channel*/
                CanTp_InitSubChannel(RxchannelPtr);
                break;
            case BUFREQ_E_OVFL:
            default:
                /*Notify the upper for error*/
                PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                /*Release the resource of the channel*/
                CanTp_ReleaseRxChannel(RxchannelPtr);
                break;
            } /*END OF switch (bufRslt)*/
        }
#if ((CANTP_DYN_ID_SUPPORT == STD_ON) && (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON))
        else
        {
            /*idle*/
        }
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON) && (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
}

#if (STD_ON == CANTP_FD)
CANTP_LOCAL FUNC(boolean, CANTP_CODE) CanTp_checkSF(PduLengthType SF_DL, PduLengthType CAN_DL, PduLengthType Offset)
{
    boolean ret = FALSE;
    switch (CAN_DL)
    {
    case 12u:
        if ((SF_DL >= (8u - Offset)) && (SF_DL <= (10u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 16u:
    case 20u:
    case 24u:
        if ((SF_DL >= ((CAN_DL - 5u) - Offset)) && (SF_DL <= ((CAN_DL - 2u) - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 32u:
        if ((SF_DL >= (23u - Offset)) && (SF_DL <= (30u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 48u:
        if ((SF_DL >= (31u - Offset)) && (SF_DL <= (46u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 64u:
        if ((SF_DL >= (47u - Offset)) && (SF_DL <= (62u - Offset)))
        {
            ret = TRUE;
        }
        break;
    default:
        if (CAN_DL <= 8u)
        {
            ret = TRUE;
        }
        break;
    }
    return ret;
}

CANTP_LOCAL FUNC(boolean, CANTP_CODE) CanTp_checkLastCF(PduLengthType CF_DL, PduLengthType CAN_DL, PduLengthType Offset)
{
    boolean ret = FALSE;
    switch (CAN_DL)
    {
    case 12u:
        if ((CF_DL >= (8u - Offset)) && (CF_DL <= (11u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 16u:
    case 20u:
    case 24u:
        if ((CF_DL >= ((CAN_DL - 4u) - Offset)) && (CF_DL <= ((CAN_DL - 1u) - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 32u:
        if ((CF_DL >= (24u - Offset)) && (CF_DL <= (31u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 48u:
        if ((CF_DL >= (32u - Offset)) && (CF_DL <= (47u - Offset)))
        {
            ret = TRUE;
        }
        break;
    case 64u:
        if ((CF_DL >= (48u - Offset)) && (CF_DL <= (63u - Offset)))
        {
            ret = TRUE;
        }
        break;
    default:
        if (CAN_DL <= 8u)
        {
            ret = TRUE;
        }
        break;
    }
    return ret;
}
#endif /* STD_ON == CANTP_FD */

/*Start a block by sending FC_CTS if needed.*/
CANTP_LOCAL FUNC(Std_ReturnType, CANTP_CODE) CanTp_RxBlockStart(
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    CanTp_ConnectionType* RxchannelPtr)
{
    Std_ReturnType ret;
    uint8 stMin;
    uint32 N_Ar;
    CanTp_FCInfoType CanTp_FC;
    CanTp_ChannelTimerType* channelTimer = &RxchannelPtr->ChannelTimer;

    /*check changParameter of STmin is done or not*/
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    if (CANTP_FLAGISSET(RxchannelPtr->EventFlags, CANTP_EVENT_RXSTMINCHANGED))
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    {
        stMin = RxchannelPtr->ChangedSTmin;
    }
    else
    {
        /*stmin is not changed by api*/
        stMin = (RxNSduCfgPtr->STmin != NULL_PTR) ? (*(RxNSduCfgPtr->STmin)) : 0u;
    }
    /*send FC_CTS*/
    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
    CanTp_FC.FcFs = CANTP_FC_FS_CTS;
    CanTp_FC.FcBS = RxchannelPtr->CurrentBs;
    CanTp_FC.FcSTMin = stMin;
    /*transit to waiting for confirmation of FC_CTS*/
    RxchannelPtr->Substate = CANTP_RX_LARGE_V4FCCTS_CFM;
    RxchannelPtr->SentWftCount = 0;
    RxchannelPtr->HandledCfCount = 0;
    if (RxNSduCfgPtr->Nar != NULL_PTR)
    {
        N_Ar = *(RxNSduCfgPtr->Nar);
        channelTimer->EnabledTimer = CANTP_NA;
        channelTimer->RemainTime = N_Ar;
        CanTp_ResetTime(&(channelTimer->StartTime));
        SchM_Exit_CanTp_ExclusiveArea();
    }
    else
    {
        channelTimer->EnabledTimer = CANTP_TIMER_NONE;
        SchM_Exit_CanTp_ExclusiveArea();
    }

    ret = CanTp_SendFC(RxNSduCfgPtr, RxchannelPtr, CanTp_FC);

    return ret;
}

/*Handle RX event occurred to the specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_DispatchRxEvent(CanTp_ConnectionType* RxchannelPtr)
{
    const CanTp_RxNSduType* RxNSduCfgPtr;
    PduLengthType buffersize;
    PduInfoType pduInfo;
    Std_ReturnType ret;
    uint8 ChannelId;

#if (STD_OFF == CANTP_SYNCHRONOUS_RXINDICATION)
    /*A receive has been finish */
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    if (CANTP_FLAGISSET(RxchannelPtr->EventFlags, CANTP_EVENT_RXNOTIFIUPPER))
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    {
        RxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(RxchannelPtr->NSduId, &ChannelId);
        if (RxNSduCfgPtr != NULL_PTR)
        {
#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
            const CanTp_ChannelType* channelCfgPtr = &CanTp_ConfigPtr->CanTpChannel[ChannelId];
            if (channelCfgPtr->DiagGatewayResponseEnable == TRUE)
            {
                CanTp_GatewayTimerType* NeedRes = &CANTP_TXCHANNEL(ChannelId)->NeedRes;
                NeedRes->Started = TRUE;
                NeedRes->RemainTime = channelCfgPtr->DiagGatewayResponseP2;
                NeedRes->ResponseTimer = 0;
                NeedRes->PendingSid = NeedRes->Sid;
                CanTp_ResetTime(&(NeedRes->StartTime));
#if ((CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON) || (CANTP_DYN_ID_SUPPORT == STD_ON))
                CanTp_ConnectionType* TxSubchannelPtr = CANTP_TXSUBCHANNEL(ChannelId);
                /*initialize address information of SDU with MetaData*/
                TxSubchannelPtr->MetaDataN_TA = RxchannelPtr->MetaDataN_SA;
                TxSubchannelPtr->MetaDataN_SA = RxchannelPtr->MetaDataN_TA;
                TxSubchannelPtr->MetaDataN_AE = RxchannelPtr->MetaDataN_AE;
                TxSubchannelPtr->MetaDataLength = RxchannelPtr->MetaDataLength;
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON) || (CANTP_DYN_ID_SUPPORT == STD_ON */
            }
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */
            RxchannelPtr->RootState = CANTP_OCCUPIED;
            PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_OK);
        }
        /*release resource of channel*/
        CanTp_InitSubChannel(RxchannelPtr);
    }
#endif /* STD_OFF == CANTP_SYNCHRONOUS_RXINDICATION */
    /*receiving large SDU, trying to get buffer*/
    if (CANTP_RX_LARGE_START == RxchannelPtr->Substate)
    {
        RxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(RxchannelPtr->NSduId, &ChannelId);
        if ((RxNSduCfgPtr != NULL_PTR) && ((RxchannelPtr->ChannelTimer.EnabledTimer == CANTP_NB)))
        {
            pduInfo.SduDataPtr = NULL_PTR;
            pduInfo.SduLength = 0;
            /*query the upper layer about the available buffer size*/
            (void)PduR_CanTpCopyRxData(RxNSduCfgPtr->RxIPduId, &pduInfo, &buffersize);
            /*if or not have enough buffer size in the upper layer to store the next block*/
            ret = CanTp_CompareBufferWithBs(RxNSduCfgPtr, RxchannelPtr, buffersize);
            if (E_NOT_OK == ret)
            {
                CanTp_DispatchRxEventSubDeal(RxchannelPtr, RxNSduCfgPtr);
            }
            else
            {
                /*upper buffer bigger than next block size*/
                /*FF reception finished or current BS reception finished, send FC_CTS to start a block*/
                ret = CanTp_RxBlockStart(RxNSduCfgPtr, RxchannelPtr);
                if (E_NOT_OK == ret)
                {
                    PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                    CanTp_ReleaseRxChannel(RxchannelPtr);
                }
            }
        }
    }
}

/*Handle RX event occurred to the specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_DispatchRxEventSubDeal(
    CanTp_ConnectionType* RxchannelPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr)
{
    uint8 wtfMax;
    Std_ReturnType ret = E_NOT_OK;
    CanTp_FCInfoType CanTp_FC;
    CanTp_ChannelTimerType* channelTimer = &RxchannelPtr->ChannelTimer;

    /*upper buffer smaller than next block size, can not receive*/
    if (0u == channelTimer->RemainTime)
    {
        wtfMax = (RxNSduCfgPtr->RxWftMax != NULL_PTR) ? (*(RxNSduCfgPtr->RxWftMax)) : 0xFFu;
        /*Nbr expires*/
        if (0u != wtfMax)
        {
            /*If the whole SDU will be send in more than one single
            block(I.E. BS is not zero) and it the end of
            current block but not the last block, or if currently it
            is handling First Frame, it is allowed to send FC_WAIT*/
            if ((CANTP_FTYPE_FF == RxchannelPtr->HandleType)
                || ((RxchannelPtr->CurrentBs == RxchannelPtr->HandledCfCount) && (RxchannelPtr->SduDataRemaining > 0u)))
            {
                /*Try to send FC_WAIT*/
                if (wtfMax <= RxchannelPtr->SentWftCount)
                {
                    PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
                    /*Can not send any more FC_WAIT. Abort this reception*/
                    /*release resource of this channel*/
                    CanTp_ReleaseRxChannel(RxchannelPtr);
                }
                else
                {
                    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
                    /*Send FC_WAIT and then wait for the confirmation*/
                    CanTp_FC.FcFs = CANTP_FC_FS_WT;
                    CanTp_FC.FcBS = 0;
                    CanTp_FC.FcSTMin = 0;
                    /*send FC OK*/
                    RxchannelPtr->SentWftCount++;
                    /*transit to waiting for confirmation of FC_WAIT*/
                    RxchannelPtr->Substate = CANTP_RX_LARGE_V4FCWT_CFM;
                    if (RxNSduCfgPtr->Nar != NULL_PTR)
                    {
                        channelTimer->EnabledTimer = CANTP_NA;
                        channelTimer->RemainTime = *(RxNSduCfgPtr->Nar);
                        CanTp_ResetTime(&(channelTimer->StartTime));
                        SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
                    }
                    else
                    {
                        channelTimer->EnabledTimer = CANTP_TIMER_NONE;
                        SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
                    }
                    ret = CanTp_SendFC(RxNSduCfgPtr, RxchannelPtr, CanTp_FC);
                }
            } /*END OF try to send FC_WAIT*/
        }
        if (E_NOT_OK == ret)
        {
            PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
            /*send FC failed or wtfMax is 0,release resource of channel*/
            CanTp_ReleaseRxChannel(RxchannelPtr);
        }
    } /*End of Nbr expires*/
}

/*Handle TX event occurred to the specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_DispatchTxEvent(CanTp_ConnectionChannelType* connectionChannel)
{
    const CanTp_ConnectionType* TxSubchannelPtr = &connectionChannel->TxConnection.Connection;
    const CanTp_ConnectionType* RxchannelPtr = &connectionChannel->RxConnection;

    if ((CANTP_RX_LARGE_V4FCWT_CFM != RxchannelPtr->Substate) && (CANTP_RX_LARGE_V4FCCTS_CFM != RxchannelPtr->Substate))
    {
        switch (TxSubchannelPtr->Substate)
        {
        case CANTP_TX_SF_START:
            /*transmitting SF, trying to get buffer*/
            CanTp_TxHandleSFStart(connectionChannel);
            break;
        case CANTP_TX_LARGE_START:
            /*transmitting large SDU, trying to get buffer*/
            CanTp_TxHandleLargeStart(connectionChannel);
            break;
        default:
            /*No special handling needed*/
            break;
        }
    }
}

/*handling for SF transmitting.*/
FUNC(void, CANTP_CODE) CanTp_TxHandleSFStart(CanTp_ConnectionChannelType* connectionChannel)
{
    CanTp_TxConnectionType* TxchannelPtr = &connectionChannel->TxConnection;
    CanTp_ConnectionType* TxSubchannelPtr = &TxchannelPtr->Connection;
    PduInfoType pduInfo;
    const CanTp_TxNSduType* txSduCfgPtr;
    BufReq_ReturnType bufRslt;
    Std_ReturnType rslt;
    uint8 totalOffset;
    uint8 unusedchannelId;
    PduLengthType unusedavailableDataPtr;
    /*local buffer of channel*/
    uint8* pLocalBuf = TxchannelPtr->LocalBuf;
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    pduInfo.MetaDataPtr = TxchannelPtr->Metadata;
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
    txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(TxSubchannelPtr->NSduId, &unusedchannelId);
    CanTp_ChannelTimerType* channelTimer;

    if (NULL_PTR != txSduCfgPtr)
    {
        /*construct SF PCI information*/
        CanTp_ConstructSFPci(TxSubchannelPtr, txSduCfgPtr, pLocalBuf, &totalOffset);
        pduInfo.SduDataPtr = &pLocalBuf[totalOffset];
        pduInfo.SduLength = TxSubchannelPtr->SduDataRemaining;
        /*upperBufRemaining is upper layer data left after copy data*/
        bufRslt = PduR_CanTpCopyTxData(txSduCfgPtr->TxIPduId, &pduInfo, NULL_PTR, &unusedavailableDataPtr);
        /*Handle according to the request buffer result*/
        switch (bufRslt)
        {
        case BUFREQ_OK: {
            /*get length of the data copy*/
            TxSubchannelPtr->SduDataRemaining -= pduInfo.SduLength;
            if (0u == TxSubchannelPtr->SduDataRemaining)
            {
                /*SF data copy finished, send it*/
                /*construct CANIF transmit request*/
                pduInfo.SduDataPtr = pLocalBuf;
                pduInfo.SduLength += (PduLengthType)totalOffset;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                CanTp_ConstructTxMetaDataInfo(TxSubchannelPtr, txSduCfgPtr, &pduInfo);
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
                /*pduInfo.SduLength will be set in the api below, if padding*/
                CanTp_TxSFPadding(TxSubchannelPtr, txSduCfgPtr, &pduInfo);
                SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
                /*Transit channel to wait for confirmation of SF*/
                TxSubchannelPtr->Substate = CANTP_TX_SF_V4SF_CFM;
                channelTimer = &TxSubchannelPtr->ChannelTimer;
                channelTimer->EnabledTimer = CANTP_NA;
                channelTimer->RemainTime = txSduCfgPtr->Nas;
                CanTp_ResetTime(&(channelTimer->StartTime));
                SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
                /*Request CANIF to transmit the SF frame*/
                rslt = CanIf_Transmit(txSduCfgPtr->TxLPduId, &pduInfo);
                if (E_NOT_OK == rslt)
                {
                    /* change tx RootState to idle , avoid release call the txconfiramation with E_NOT_OK*/
                    TxSubchannelPtr->RootState = CANTP_IDLE;
                    /*Release resources related to the channel*/
                    CanTp_ReleaseTxChannel(TxSubchannelPtr);
                    /*Notify the upper*/
                    PduR_CanTpTxConfirmation(txSduCfgPtr->TxIPduId, E_NOT_OK);
                }
            } /*END OF SF data copy finished*/
            break;
        } /*ENDOF case BUFREQ_OK*/
        case BUFREQ_E_NOT_OK: {
            /*Fail to get TX buffer, Release resources related to the channel*/
            CanTp_ReleaseTxChannel(TxSubchannelPtr);
            /*Notify the upper*/
            PduR_CanTpTxConfirmation(txSduCfgPtr->TxIPduId, E_NOT_OK);
            break;
        }
        case BUFREQ_E_BUSY:
            /*The upper is busy, no buffer provided. Try to get buffer next round*/
            break;
        default:
            /*invalid return value for PduR_CanTpProvideTxBuffer*/
            break;
        } /*END OF switch*/
    }
}

/* handling for large SDU transmitting, trying to get TX buffer.*/
FUNC(void, CANTP_CODE) CanTp_TxHandleLargeStart(CanTp_ConnectionChannelType* connectionChannel)
{
    CanTp_TxConnectionType* TxchannelPtr = &connectionChannel->TxConnection;
    CanTp_ConnectionType* TxSubchannelPtr = &TxchannelPtr->Connection;
    PduInfoType pduInfo;
    const CanTp_TxNSduType* txSduCfgPtr;
    BufReq_ReturnType bufRslt = BUFREQ_OK;
    Std_ReturnType rslt;
    uint8 totalOffset;
    uint8 unusedchannelId;
    PduLengthType unusedavailableDataPtr;
    PduLengthType CAN_DL;
    /*local buffer of channel*/
    uint8* pLocalBuf = TxchannelPtr->LocalBuf;
    CanTp_ChannelTimerType* channelTimer;
    CanTp_STminType* STminTimer;

#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    pduInfo.MetaDataPtr = TxchannelPtr->Metadata;
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
    txSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(TxSubchannelPtr->NSduId, &unusedchannelId);
    if (NULL_PTR != txSduCfgPtr)
    {
        if (CANTP_FTYPE_FF == TxSubchannelPtr->HandleType)
        {
            /*construct FF PCI information*/
            CanTp_ConstructFFPci(TxSubchannelPtr, txSduCfgPtr, pLocalBuf, &totalOffset);
        }
        else
        {
            /*construct CF PCI information*/
            CanTp_ConstructCFPci(TxSubchannelPtr, txSduCfgPtr, pLocalBuf, &totalOffset);
        }

        /*calculate the length which need to be copied*/
        CAN_DL = (txSduCfgPtr->TxNPduDLC <= CANTP_CAN20_FRAME_LEN_MAX) ? txSduCfgPtr->TxNPduDLC :
#if (STD_OFF == CANTP_FD)
                                                                       /*CAN2.0*/
                     CANTP_CAN20_FRAME_LEN_MAX;
#else
                                                                       /*CANFD*/
                     ((CANTP_CAN_20 == txSduCfgPtr->CanFrameType) ? CANTP_CAN20_FRAME_LEN_MAX
                                                                  : CanTp_MatchDLC(txSduCfgPtr->TxNPduDLC));
#endif /* STD_OFF == CANTP_FD */

        pduInfo.SduLength = CAN_DL - totalOffset;
        /*the last CF may less than the length of CAN Frame*/
        if (pduInfo.SduLength > TxSubchannelPtr->SduDataRemaining)
        {
            pduInfo.SduLength = TxSubchannelPtr->SduDataRemaining;
#if (STD_ON == CANTP_FD)
            CAN_DL = CanTp_MatchDLC((pduInfo.SduLength + totalOffset));
#endif /* STD_ON == CANTP_FD */
        }
        pduInfo.SduDataPtr = &pLocalBuf[totalOffset];

        /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
        if ((CANTP_FTYPE_FF == TxSubchannelPtr->HandleType)
            || ((!CANTP_FLAGISSET(TxSubchannelPtr->EventFlags, CANTP_EVENT_COPYANDWAITSTMIN))
                && (CANTP_FTYPE_CF == TxSubchannelPtr->HandleType)))
        /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
        {
            bufRslt = PduR_CanTpCopyTxData(txSduCfgPtr->TxIPduId, &pduInfo, NULL_PTR, &unusedavailableDataPtr);
        }

        switch (bufRslt)
        {
        case BUFREQ_OK:
            /*Set CopyandWaitStminFlg*/
            /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
            CANTP_SETFLAG(TxSubchannelPtr->EventFlags, CANTP_EVENT_COPYANDWAITSTMIN);
            /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
            if (CANTP_FTYPE_FF == TxSubchannelPtr->HandleType)
            {
                /*update channel runtime information, get length of the data copy*/
                TxSubchannelPtr->SduDataRemaining -= pduInfo.SduLength;
                /*send the FF immediately*/
                pduInfo.SduDataPtr = pLocalBuf;
                /*the length include pci info*/
                pduInfo.SduLength += totalOffset;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                CanTp_ConstructTxMetaDataInfo(TxSubchannelPtr, txSduCfgPtr, &pduInfo);
#endif                                            /* CANTP_DYN_ID_SUPPORT == STD_ON */
                SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
                /*Transit channel to wait for confirmation of FF*/
                TxSubchannelPtr->Substate = CANTP_TX_LARGE_V4DATA_CFM;
                channelTimer = &TxSubchannelPtr->ChannelTimer;
                channelTimer->EnabledTimer = CANTP_NA;
                channelTimer->RemainTime = txSduCfgPtr->Nas;
                CanTp_ResetTime(&(channelTimer->StartTime));
                SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
                rslt = CanIf_Transmit(txSduCfgPtr->TxLPduId, &pduInfo);
                /*Clear CopyandWaitStminFlg*/
                /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                CANTP_CLEARFLAG(TxSubchannelPtr->EventFlags, CANTP_EVENT_COPYANDWAITSTMIN);
                /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
                if (E_NOT_OK == rslt)
                {
                    /* change tx RootState to idle , avoid release call the txconfiramation with E_NOT_OK*/
                    CanTp_ReleaseTxChannel(TxSubchannelPtr);
                    /*Notify the upper*/
                    PduR_CanTpTxConfirmation(txSduCfgPtr->TxIPduId, E_NOT_OK);
                }
            }
            else /*handle the CF transmitting data copy*/
            {
                STminTimer = &TxSubchannelPtr->STminTimer;
                /*STmin timeout, send the CF immediately*/
                if ((TRUE == STminTimer->Started) && (0u == STminTimer->RemainTime))
                {
                    /*update channel runtime information, get length of the data copy*/
                    TxSubchannelPtr->SduDataRemaining -= pduInfo.SduLength;
                    pduInfo.SduDataPtr = pLocalBuf;
                    pduInfo.SduLength += totalOffset;
                    /*pduInfo.SduLength will be set in the api below*/
                    if ((0u == TxSubchannelPtr->SduDataRemaining)
                        && (CANTP_PADDING_ON == txSduCfgPtr->TxPaddingActivation))
                    {
                        /*CAN2.0 or small CAN FD*/
                        CAN_DL = (CAN_DL < CANTP_CAN20_FRAME_LEN_MAX) ? CANTP_CAN20_FRAME_LEN_MAX : CAN_DL;
                        CanTp_MemorySet(
                            &pduInfo.SduDataPtr[pduInfo.SduLength],
                            CANTP_PADDING_BYTE,
                            (uint16)(CAN_DL - pduInfo.SduLength));
                        pduInfo.SduLength = CAN_DL;
                    }
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                    CanTp_ConstructTxMetaDataInfo(TxSubchannelPtr, txSduCfgPtr, &pduInfo);
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
                    /*Transit channel to wait for confirmation of CF*/
                    TxSubchannelPtr->Substate = CANTP_TX_LARGE_V4DATA_CFM;
                    channelTimer = &TxSubchannelPtr->ChannelTimer;
                    channelTimer->EnabledTimer = CANTP_NA;
                    channelTimer->RemainTime = txSduCfgPtr->Nas;
                    CanTp_ResetTime(&(channelTimer->StartTime));
                    STminTimer->Started = FALSE;
                    rslt = CanIf_Transmit(txSduCfgPtr->TxLPduId, &pduInfo);
                    /*Clear CopyandWaitStminFlg*/
                    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
                    CANTP_CLEARFLAG(TxSubchannelPtr->EventFlags, CANTP_EVENT_COPYANDWAITSTMIN);
                    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
                    if (E_NOT_OK == rslt)
                    {
                        /* change tx RootState to idle , avoid release call the txconfiramation with E_NOT_OK*/
                        TxSubchannelPtr->RootState = CANTP_IDLE;
                        CanTp_ReleaseTxChannel(TxSubchannelPtr);
                        /*Notify the upper*/
                        PduR_CanTpTxConfirmation(txSduCfgPtr->TxIPduId, E_NOT_OK);
                    }
                }
            } /*END OF handle the CF transmitting data copy*/
            break;
        case BUFREQ_E_NOT_OK:
            /* change tx RootState to idle , avoid release call the txconfiramation with E_NOT_OK*/
            TxSubchannelPtr->RootState = CANTP_IDLE;
            /*Fail to get TX buffer, Release resources related to the channel*/
            CanTp_ReleaseTxChannel(TxSubchannelPtr);
            /*Notify the upper*/
            PduR_CanTpTxConfirmation(txSduCfgPtr->TxIPduId, E_NOT_OK);
            break;
        case BUFREQ_E_BUSY:
            if ((CANTP_NC == TxSubchannelPtr->ChannelTimer.EnabledTimer)
                && (0u == TxSubchannelPtr->ChannelTimer.RemainTime))
            {
                /* change tx RootState to idle , avoid release call the txconfiramation with E_NOT_OK*/
                TxSubchannelPtr->RootState = CANTP_IDLE;
                /*STmin timeout, abort send, Release resources related to the channel*/
                CanTp_ReleaseTxChannel(TxSubchannelPtr);
                /*Notify the upper*/
                PduR_CanTpTxConfirmation(txSduCfgPtr->TxIPduId, E_NOT_OK);
            }
            /*The upper is busy, no buffer provided. Try to get buffer next round*/
            break;
        default:
            /*invalid return value*/
            break;
        } /*END OF switch (bufRslt)*/
    }
}

#if (STD_ON == CANTP_RX_QUEUE)
/* Add new request to Rx Queue */
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_AddRxQueue(uint8* buffer, uint8 ChannelId, uint8* accIndex)
{
    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
    uint8 index;
    boolean find = FALSE;
    for (index = 0; index < *accIndex; index++)
    {
        if (buffer[index] == ChannelId)
        {
            find = TRUE;
            break;
        }
    }
    if (FALSE == find)
    {
        buffer[*accIndex] = ChannelId;
        (*accIndex)++;
    }
    SchM_Exit_CanTp_ExclusiveArea();
}
#endif /* STD_ON == CANTP_RX_QUEUE */

#if (CANTP_DIAG_GW_RES_ENABLE == STD_ON)
/*Handle the diag gateway timer for specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_HandleGatewayTimers(uint8 ChannelId)
{
    CanTp_TxConnectionType* TxchannelPtr = CANTP_TXCHANNEL(ChannelId);
    CanTp_ConnectionType* TxSubchannelPtr = CANTP_TXSUBCHANNEL(ChannelId);
    uint32 elapsedTick;
    CanTp_GatewayTimerType* NeedRes = &TxchannelPtr->NeedRes;

    if (TRUE == NeedRes->Started)
    {
        /*update the time information*/
        CanTp_GetTimeSpan(NeedRes->StartTime, &elapsedTick);
        if (NeedRes->RemainTime <= elapsedTick)
        {
            const CanTp_ChannelType* channelCfgPtr = &CanTp_ConfigPtr->CanTpChannel[ChannelId];
            PduInfoType pduInfo;
            uint8 totalOffset;
            const CanTp_TxNSduType* txSduCfgPtr = channelCfgPtr->TxNSdus;
            Std_ReturnType rslt;
            /*local buffer of channel*/
            uint8* pLocalBuf = TxchannelPtr->LocalBuf;

            if (NULL_PTR != txSduCfgPtr)
            {
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
                pduInfo.MetaDataPtr = TxchannelPtr->Metadata;
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
                TxSubchannelPtr->SduDataTotalCount = 3u;
                /*construct SF PCI information*/
                CanTp_ConstructSFPci(TxSubchannelPtr, txSduCfgPtr, pLocalBuf, &totalOffset);
                pLocalBuf[totalOffset] = 0x7F;
                totalOffset++;
                pLocalBuf[totalOffset] = NeedRes->PendingSid;
                totalOffset++;
                /* updata NeedRes */
                if ((NeedRes->ResponseTimer < channelCfgPtr->DiagGatewayResponseMaxNum)
                    || (channelCfgPtr->DiagGatewayResponseMaxNum == 0u))
                {
                    /* need to send NRC 78*/
                    NeedRes->ResponseTimer++;
                    NeedRes->RemainTime = channelCfgPtr->DiagGatewayResponseP2Star;
                    CanTp_ResetTime(&(NeedRes->StartTime));
                    pLocalBuf[totalOffset] = 0x78;
                    totalOffset++;
                }
                else
                {
                    /* need to send NRC 10*/
                    pLocalBuf[totalOffset] = 0x10;
                    totalOffset++;
                    NeedRes->Started = FALSE;
                }

                pduInfo.SduDataPtr = pLocalBuf;
                pduInfo.SduLength = (PduLengthType)totalOffset;
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
                CanTp_ConstructTxMetaDataInfo(TxSubchannelPtr, txSduCfgPtr, &pduInfo);
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
                /*pduInfo.SduLength will be set in the api below, if padding*/
                CanTp_TxSFPadding(TxSubchannelPtr, txSduCfgPtr, &pduInfo);
                /*Request CANIF to transmit the SF frame*/
                rslt = CanIf_Transmit(txSduCfgPtr->TxLPduId, &pduInfo);
                if (E_NOT_OK == rslt)
                {
                    /* change tx RootState to idle , avoid release call the txconfiramation with E_NOT_OK*/
                    TxSubchannelPtr->RootState = CANTP_IDLE;
                    /*Release resources related to the channel*/
                    CanTp_ReleaseTxChannel(TxSubchannelPtr);
                }
            }
        }
    }
}
#endif /* CANTP_DIAG_GW_RES_ENABLE == STD_ON */

/*Handle the timer for specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_HandleTimers(CanTp_ConnectionChannelType* connectionChannel)
{
    CanTp_ConnectionType* RxchannelPtr = &connectionChannel->RxConnection;
    CanTp_ConnectionType* TxSubchannelPtr = &connectionChannel->TxConnection.Connection;
    const CanTp_RxNSduType* RxNSduCfgPtr;
    uint32 elapsedTick;
    uint8 unusedchannelId;
    CanTp_ChannelTimerType* channelTimer;
    boolean timeOut = FALSE;

    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
    if (CANTP_LARGE_RECEIVING == RxchannelPtr->RootState)
    {
        channelTimer = &RxchannelPtr->ChannelTimer;
        /*channel is receiving SDU, Handle ABC timer*/
        if (CANTP_TIMER_NONE != channelTimer->EnabledTimer)
        {
            /*update the time information*/
            CanTp_GetTimeSpan(channelTimer->StartTime, &elapsedTick);
            if (channelTimer->RemainTime <= elapsedTick)
            {
                channelTimer->RemainTime = 0;
                /*Channel timer timeout occurred*/
                RxNSduCfgPtr = CanTp_GetRxSduCfgByNSduId(RxchannelPtr->NSduId, &unusedchannelId);
                if (NULL_PTR != RxNSduCfgPtr)
                {
                    /*CANTP_NB deal in CanTp_DispatchRxEvent*/
                    if (CANTP_NB != channelTimer->EnabledTimer)
                    {
                        timeOut = TRUE;
                    }
                    /*report development error*/
                    CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_MAINFUNCTION, CANTP_E_COM);
                }
            }
        }
    }
    SchM_Exit_CanTp_ExclusiveArea();
    if (TRUE == timeOut)
    {
        /*notify upper,receiving failed*/
        PduR_CanTpRxIndication(RxNSduCfgPtr->RxIPduId, E_NOT_OK);
        /*release resource of channel*/
        CanTp_ReleaseRxChannel(RxchannelPtr);
    }
    CanTp_HandleTxTimers(TxSubchannelPtr);
}

/*Handle the tx timer for specific channel.*/
CANTP_LOCAL FUNC(void, CANTP_CODE) CanTp_HandleTxTimers(CanTp_ConnectionType* TxSubchannelPtr)
{
    const CanTp_TxNSduType* TxSduCfgPtr;
    uint32 elapsedTick;
    uint8 unusedchannelId;
    CanTp_ChannelTimerType* channelTimer;
    CanTp_STminType* STminTimer;
    boolean timeOut = FALSE;

    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
    if ((CANTP_SF_TRANSIMITTING == TxSubchannelPtr->RootState)
        || (CANTP_LARGE_TRANSMITTING == TxSubchannelPtr->RootState))
    {
        /*handle STmin timer for transmitter if needed*/
        STminTimer = &TxSubchannelPtr->STminTimer;
        if ((CANTP_LARGE_TRANSMITTING == TxSubchannelPtr->RootState) && (TRUE == STminTimer->Started))
        {
            CanTp_GetTimeSpan(STminTimer->StartTime, &elapsedTick);
            if (STminTimer->RemainTime <= elapsedTick)
            {
                STminTimer->RemainTime = 0;
            }
        } /*END OF STMin timer handling*/

        channelTimer = &TxSubchannelPtr->ChannelTimer;
        /*channel is transmitting SDU*/
        if (CANTP_TIMER_NONE != channelTimer->EnabledTimer)
        {
            /*update the time information*/
            CanTp_GetTimeSpan(channelTimer->StartTime, &elapsedTick);
            if (channelTimer->RemainTime <= elapsedTick)
            {
                channelTimer->RemainTime = 0;
                /*Channel timer timeout occurred*/
                TxSduCfgPtr = CanTp_GetTxSduCfgByTxSduId(TxSubchannelPtr->NSduId, &unusedchannelId);
                if (NULL_PTR != TxSduCfgPtr)
                {
                    /*CANTP_NC deal in LargeStart*/
                    if (CANTP_NC != channelTimer->EnabledTimer)
                    {
                        timeOut = TRUE;
                    }
                    /*report development error*/
                    CANTP_DET_REPORTRUNTIMEERROR(CANTP_SERVICEID_MAINFUNCTION, CANTP_E_COM);
                }
            }
        }
    }
    if (TRUE == timeOut)
    {
        /* change tx RootState to idle , avoid
        release call the txconfiramation with E_NOT_OK*/
        TxSubchannelPtr->RootState = CANTP_IDLE;
        /*release resource of channel*/
        CanTp_ReleaseTxChannel(TxSubchannelPtr);
        /*notify upper,transmitting failed*/
        PduR_CanTpTxConfirmation(TxSduCfgPtr->TxIPduId, E_NOT_OK);
    }
    SchM_Exit_CanTp_ExclusiveArea();
}

/*
 * Brief               Get SDU configuration by N-SDU ID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
    CanTp_GetRxSduCfgByNSduId(PduIdType CanTpRxSduId, uint8* ChannelId)
{
    uint8 i;
    uint8 j;
    const CanTp_RxNSduType* RxNSduCfgPtr = NULL_PTR;
    const CanTp_ChannelType* channelCfgPtr;
    const CanTp_RxNSduType* RxNSdus;

    for (i = 0; (NULL_PTR == RxNSduCfgPtr) && (i < CanTp_ConfigPtr->ChannelNum); i++)
    {
        channelCfgPtr = &CanTp_ConfigPtr->CanTpChannel[i];
        for (j = 0; j < channelCfgPtr->RxNSduNum; j++)
        {
            RxNSdus = &channelCfgPtr->RxNSdus[j];
            if (RxNSdus->RxNSduId == CanTpRxSduId)
            {
                RxNSduCfgPtr = RxNSdus;
                *ChannelId = i;
                break;
            }
        }
    }

    return RxNSduCfgPtr;
}

/*Get TX-SDU configuration by TX-NSduId.*/
CANTP_LOCAL CANTP_INLINE FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
    CanTp_GetTxSduCfgByTxSduId(PduIdType TxSduId, uint8* ChannelId)
{
    const CanTp_TxNSduType* TxSduCfgPtr = NULL_PTR;
    const CanTp_TxNSduType* TxNSdus;
    const CanTp_ChannelType* channelCfgPtr;
    uint8 i;
    uint8 j;

    for (i = 0; (NULL_PTR == TxSduCfgPtr) && (i < CANTP_CHANNEL_NUMBER); i++)
    {
        channelCfgPtr = &CanTp_ConfigPtr->CanTpChannel[i];
        for (j = 0; j < channelCfgPtr->TxNsduNum; j++)
        {
            TxNSdus = &channelCfgPtr->TxNSdus[j];
            if (TxSduId == TxNSdus->TxNSduId)
            {
                TxSduCfgPtr = TxNSdus;
                *ChannelId = i;
                break;
            }
        }
    }
    return TxSduCfgPtr;
}

/*
 * Brief               Get Tx-SDU configuration by Tx NPDUID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
    CanTp_GetTxSduCfgByTxNPdu(PduIdType CanTpTxPduId, uint8* ChannelId)
{
    const CanTp_TxNSduType* TxSduCfgPtr = NULL_PTR;
    const CanTp_ChannelType* channelPtr;
    const CanTp_TxNSduType* TxNSdus;
    uint8 i;
    uint8 j;

    for (i = 0; (NULL_PTR == TxSduCfgPtr) && (i < CANTP_CHANNEL_NUMBER); i++)
    {
        channelPtr = &CanTp_ConfigPtr->CanTpChannel[i];
        for (j = 0; j < channelPtr->TxNsduNum; j++)
        {
            TxNSdus = &channelPtr->TxNSdus[j];
            if (CanTpTxPduId == TxNSdus->TxNPduId)
            {
                TxSduCfgPtr = TxNSdus;
                *ChannelId = i;
                break;
            }
        }
    }
    return TxSduCfgPtr;
}

/*
 * Brief               Get Rx-SDU configuration by Tx-FC NPDUID.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
CANTP_LOCAL FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
    CanTp_GetRxSduCfgByTxFCNPdu(PduIdType CanTpTxPduId, uint8* ChannelId)
{
    const CanTp_RxNSduType* RxNSduCfgPtr = NULL_PTR;
    const CanTp_RxNSduType* RxNSdus;
    const CanTp_ChannelType* channelPtr;
    uint8 i;
    uint8 j;

    for (i = 0; (NULL_PTR == RxNSduCfgPtr) && (i < CANTP_CHANNEL_NUMBER); i++)
    {
        channelPtr = &CanTp_ConfigPtr->CanTpChannel[i];
        for (j = 0; j < channelPtr->RxNSduNum; j++)
        {
            RxNSdus = &channelPtr->RxNSdus[j];
            if ((RxNSdus->TxFcNPduId == CanTpTxPduId)
                && ((CANTP_PHYSICAL_RX == RxNSdus->RxTaType)
#if (STD_ON == CANTP_FD)
                    || (CANTP_CANFD_PHYSICAL == RxNSdus->RxTaType)
#endif /* STD_ON == CANTP_FD */
                        ))
            {
                RxNSduCfgPtr = RxNSdus;
                *ChannelId = i;
                break;
            }
        }
    }
    return RxNSduCfgPtr;
}
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"

#if ((CANTP_CHANGE_PARAMETER == STD_ON) && (STD_ON == CANTP_DEV_ERROR_DETECT))
/*check Rx ChangeParameter*/
CANTP_LOCAL CANTP_INLINE FUNC(boolean, CANTP_CODE)
    CanTP_CheckRxChangeParameter(TPParameterType canTpParam, uint16 value)
{
    boolean paramValid = FALSE;

    if (((TP_BS == canTpParam) && (value < 256u)) || ((TP_STMIN == canTpParam) && (value <= 0x7Fu)))
    {
        paramValid = TRUE;
    }

    return paramValid;
}
#endif /* CANTP_CHANGE_PARAMETER == STD_ON && STD_ON == CANTP_DEV_ERROR_DETECT */

/*=======[E N D   O F   F I L E]==============================================*/
