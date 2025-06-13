/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : CanIf_Internal.c                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for CANIF                                    **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19-11              **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      REVISION   HISTORY                                    **
*******************************************************************************/
/* Refer to CanIf.h */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CanIf_Internal.h"
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if (CANIF_DYNAMIC_TXPDU_NUMBER > 0u)
#define CANIF_START_SEC_VAR_NO_INIT_32
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(CanIf_IdType, CANIF_VAR) CanIf_TxPduDynamicCanIds[CANIF_DYNAMIC_TXPDU_NUMBER];
#define CANIF_STOP_SEC_VAR_NO_INIT_32
#include "CanIf_MemMap.h"
#endif

#if (CANIF_RXNOTIFYSTATUS_BUFFER > 0u)
#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_RxNotifStatus[CANIF_RXNOTIFYSTATUS_BUFFER];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
#endif

#if (CANIF_TXNOTIFYSTATUS_BUFFER > 0u)
#define CANIF_START_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
CANIF_LOCAL VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_TxNotifStatus[CANIF_TXNOTIFYSTATUS_BUFFER];
#define CANIF_STOP_SEC_VAR_NO_INIT_8
#include "CanIf_MemMap.h"
#endif
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
/*************************************************************************/
/*
 * Brief               Set RxNotifyStatus to CANIF_TX_RX_NOTIFICATION
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfRxSduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API is TRUE
 * CallByAPI           CanIf_RxIndicationHandle
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
CanIf_SetRxNotifStatusHandle(PduIdType CanIfRxSduId)
{
#if (CANIF_RXNOTIFYSTATUS_BUFFER > 0u)
    const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(CanIfRxSduId);
    SchM_Enter_CanIf_ExclusiveArea_Channel();
    if (rxPduConfigPtr->CanIfRxPduReadNotifyStatus)
    {
        /* set rx status to CANIF_TX_RX_NOTIFICATION */
        CanIf_RxNotifStatus[rxPduConfigPtr->CanIfRxNotifyIndex] = CANIF_TX_RX_NOTIFICATION;
    }
    SchM_Exit_CanIf_ExclusiveArea_Channel();
#else
    CANIF_NOUSED(CanIfRxSduId);
#endif
}
/*************************************************************************/
/*
 * Brief               Read RxNotifyStatus
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfRxSduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType
 * PreCondition        CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API is TRUE
 * CallByAPI           CanIf_ReadRxNotifStatus
 */
/*************************************************************************/
FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_ReadRxNotifStatusHandle(PduIdType CanIfRxSduId)
{
#if (CANIF_RXNOTIFYSTATUS_BUFFER > 0u)
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;
    const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(CanIfRxSduId);
    if (rxPduConfigPtr->CanIfRxPduReadNotifyStatus)
    {
        SchM_Enter_CanIf_ExclusiveArea_Channel();
        result = CanIf_RxNotifStatus[rxPduConfigPtr->CanIfRxNotifyIndex];
        /* clear STATUS */
        CanIf_RxNotifStatus[rxPduConfigPtr->CanIfRxNotifyIndex] = CANIF_NO_NOTIFICATION;
        SchM_Exit_CanIf_ExclusiveArea_Channel();
    }
    return result;
#else
    CANIF_NOUSED(CanIfRxSduId);
    return CANIF_NO_NOTIFICATION;
#endif
}
#endif /*STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API*/

#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
/*************************************************************************/
/*
 * Brief               Set TxNotifyStatus to CANIF_TX_RX_NOTIFICATION
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfTxSduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API is TRUE
 * CallByAPI           CanIf_TxConfirmation and CanIf_TransmitHandle
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
CanIf_SetTxNotifStatusHandle(PduIdType CanIfTxSduId)
{
#if (CANIF_TXNOTIFYSTATUS_BUFFER > 0u)
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanIfTxSduId);
    SchM_Enter_CanIf_ExclusiveArea_Channel();
    if (txPduConfigPtr->CanIfTxPduReadNotifyStatus)
    {
        /* change to notify status */
        CanIf_TxNotifStatus[txPduConfigPtr->CanIfTxNotifyIndex] = CANIF_TX_RX_NOTIFICATION;
    }
    SchM_Exit_CanIf_ExclusiveArea_Channel();
#else
    CANIF_NOUSED(CanIfTxSduId);
#endif
}
/*************************************************************************/
/*
 * Brief               Read TxNotifyStatus
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfTxSduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              CanIf_NotifStatusType
 * PreCondition        CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API is TRUE
 * CallByAPI           CanIf_ReadTxNotifStatus
 */
/*************************************************************************/
FUNC(CanIf_NotifStatusType, CANIF_CODE)
CanIf_ReadTxNotifStatusHandle(PduIdType CanIfTxSduId)
{
#if (CANIF_TXNOTIFYSTATUS_BUFFER > 0u)
    CanIf_NotifStatusType result = CANIF_NO_NOTIFICATION;
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanIfTxSduId);
    if (txPduConfigPtr->CanIfTxPduReadNotifyStatus)
    {
        SchM_Enter_CanIf_ExclusiveArea_Channel();
        result = CanIf_TxNotifStatus[txPduConfigPtr->CanIfTxNotifyIndex];
        /* clear STATUS */
        CanIf_TxNotifStatus[txPduConfigPtr->CanIfTxNotifyIndex] = CANIF_NO_NOTIFICATION;
        SchM_Exit_CanIf_ExclusiveArea_Channel();
    }
    return result;
#else
    CANIF_NOUSED(CanIfTxSduId);
    return CANIF_NO_NOTIFICATION;
#endif
}
#endif /*STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API*/

#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
/*************************************************************************/
/*
 * Brief               Re-init rxNotifyStatus and txNotifyStatus of one controller
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller: the id of can controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API is TRUE or
 *                     CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API is TRUE
 * CallByAPI           CanIf_FreeBufInStopped
 */
/*************************************************************************/
FUNC(void, CANIF_CODE) CanIf_FreeRxTxNotifyStatus(uint8 Controller)
{
#if (CANIF_TXNOTIFYSTATUS_BUFFER > 0u)
    PduIdType txPduId;
    Can_HwHandleType hthId;
    PduIdType txNotifyIndex;
    const CanIf_TxPduConfigType* txPduConfigPtr;
#endif
#if (CANIF_RXNOTIFYSTATUS_BUFFER > 0u)
    PduIdType rxPduId;
    Can_HwHandleType hrhId;
    PduIdType rxNotifyIndex;
    const CanIf_RxPduConfigType* rxPduConfigPtr;
#endif
#if ((CANIF_TXNOTIFYSTATUS_BUFFER > 0u) || (CANIF_RXNOTIFYSTATUS_BUFFER > 0u))
    uint8 controllerId;
#endif
#if (CANIF_TXNOTIFYSTATUS_BUFFER > 0u)
    /* re-init the txNotifStatus Of this controller */
    for (txPduId = 0u; txPduId < CanIf_ConfigStd->CanIfTxPduNumber; txPduId++)
    {
        txPduConfigPtr = &CANIF_TXPDU(txPduId);
        hthId = txPduConfigPtr->CanIfTxPduHthId;
        controllerId = CANIF_HTH(hthId).CanIfHthCanCtrlId;
        if ((txPduConfigPtr->CanIfTxPduReadNotifyStatus) && (Controller == controllerId))
        {
            txNotifyIndex = txPduConfigPtr->CanIfTxNotifyIndex;
            CanIf_TxNotifStatus[txNotifyIndex] = CANIF_NO_NOTIFICATION;
        }
    }
#endif
#if (CANIF_RXNOTIFYSTATUS_BUFFER > 0u)
    /* re-init the rxNotifStatus Of this controller */
    for (rxPduId = 0u; rxPduId < CanIf_ConfigStd->CanIfRxPduNumber; rxPduId++)
    {
        rxPduConfigPtr = &CANIF_RXPDU(rxPduId);
        hrhId = rxPduConfigPtr->CanIfRxPduHrhId;
        controllerId = CANIF_HRH(hrhId).CanIfHrhCanCtrlId;
        if ((rxPduConfigPtr->CanIfRxPduReadNotifyStatus) && (Controller == controllerId))
        {
            rxNotifyIndex = rxPduConfigPtr->CanIfRxNotifyIndex;
            CanIf_RxNotifStatus[rxNotifyIndex] = CANIF_NO_NOTIFICATION;
        }
    }
#endif
#if ((CANIF_TXNOTIFYSTATUS_BUFFER == 0u) && (CANIF_RXNOTIFYSTATUS_BUFFER == 0u))
    CANIF_NOUSED(Controller);
#endif
}
#endif
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
/*************************************************************************/
/*
 * Brief               init TxBuffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_InitAllBuffer
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_InitTxBuffer(VAR(void, AUTOMATIC))
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (0u < CANIF_TXBUFFER_NUMBER)
    PduIdType txBufferIndex;
    uint8 index;
    const uint8* bufferSizePtr = &CANIF_TXBUFFER_SIZE(0);
    for (txBufferIndex = 0u; txBufferIndex < CANIF_TXBUFFER_NUMBER; txBufferIndex++)
    {
        /* polling txbuffer, find the one already be used */
        for (index = 0u; index < *bufferSizePtr; index++)
        {
            CanIf_TxBuffer[txBufferIndex].CanIf_TxPduBufferRef[index].used = FALSE;
        }
        bufferSizePtr++;
    }
#endif /*0u < CANIF_TXBUFFER_NUMBER*/
}
#endif /*STD_ON == CANIF_PUBLIC_TX_BUFFERING*/
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/*************************************************************************/
/*
 * Brief               init RxBuffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_InitAllBuffer
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
CanIf_InitRxBuffer(VAR(void, AUTOMATIC))
{
#if (0u < CANIF_RXBUFFER_NUMBER)
    PduIdType rxBufferIndex;
    for (rxBufferIndex = 0u; rxBufferIndex < CANIF_RXBUFFER_NUMBER; rxBufferIndex++)
    {
        /* init the RxBuf, set length to 0. */
        CanIf_RxBuffer[rxBufferIndex].length = 0u;
    }
#endif
}
#endif /*STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API*/
#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
/*************************************************************************/
/*
 * Brief               init all rxNotifyStatus and txNotifyStatus
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_InitAllBuffer
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
CanIf_InitRxTxNotifyStatus(VAR(void, AUTOMATIC))
{
#if ((0u < CANIF_TXNOTIFYSTATUS_BUFFER) || (0u < CANIF_RXNOTIFYSTATUS_BUFFER))
    PduIdType notifyStatusIndex;
#endif
#if (0u < CANIF_TXNOTIFYSTATUS_BUFFER)
    /*Init all TxNotifStatus state*/
    for (notifyStatusIndex = 0u; notifyStatusIndex < CANIF_TXNOTIFYSTATUS_BUFFER; notifyStatusIndex++)
    {
        CanIf_TxNotifStatus[notifyStatusIndex] = CANIF_NO_NOTIFICATION;
    }
#endif

#if (0u < CANIF_RXNOTIFYSTATUS_BUFFER)
    /*Init all RxNotifStatus state*/
    for (notifyStatusIndex = 0u; notifyStatusIndex < CANIF_RXNOTIFYSTATUS_BUFFER; notifyStatusIndex++)
    {
        CanIf_RxNotifStatus[notifyStatusIndex] = CANIF_NO_NOTIFICATION;
    }
#endif
}
#endif
#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
/*************************************************************************/
/*
 * Brief               CanIf call can_write which return CAN_BSUY,then set the write requece and data to tx buffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      pduPtr: CAN L-PDU to be transmitted
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 *                     E_OK:Busy Handle,pdu saved successful
 *                     E_NOT_OK:Busy Handle,pdu saved failure
 * PreCondition        CANIF_TXBUFFER_NUMBER > 0u
 * CallByAPI           CanIf_Transmit
 */
/*************************************************************************/
FUNC(Std_ReturnType, CANIF_CODE) CanIf_TxBusyHandle(P2CONST(Can_PduType, AUTOMATIC, CANIF_APPL_DATA) pduPtr)
{
#if (CANIF_TXBUFFER_NUMBER > 0u)
    Std_ReturnType result = E_NOT_OK;
    uint8 index;
    uint8 idleBufferIndex = CANIF_UNUSED_UINT8;
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(pduPtr->swPduHandle);
    PduIdType txBufIndex = txPduConfigPtr->TxBufferIdndex;
    /*the pdu support Txbuffer*/
    if (txBufIndex < CANIF_TXBUFFER_NUMBER)
    {
        uint8 txBufferSize = CANIF_TXBUFFER_SIZE(txBufIndex);
        CanIf_TxPduBufferType* txPduBufferPtr = CanIf_TxBuffer[txBufIndex].CanIf_TxPduBufferRef;
        for (index = 0u; (index < txBufferSize) && (E_NOT_OK == result); index++)
        {
            if (txPduBufferPtr[index].TxPduIndex == pduPtr->swPduHandle)
            {
                txPduBufferPtr[index].canId = pduPtr->id;
#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
                if (txPduConfigPtr->CanIfTxPduTriggerTransmit)
                {
                    txPduBufferPtr[index].length = 0u;
                }
                else
#endif /*STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT*/
                {
                    txPduBufferPtr[index].length = pduPtr->length;
                    (void)ILib_memcpy(txPduBufferPtr[index].data, pduPtr->sdu, (PduLengthType)pduPtr->length);
                }
                txPduBufferPtr[index].used = TRUE;
                result = E_OK;
            }
            if ((CANIF_UNUSED_UINT8 == idleBufferIndex) && (!txPduBufferPtr[index].used))
            {
                idleBufferIndex = index;
            }
        }
        if ((E_NOT_OK == result) && (CANIF_UNUSED_UINT8 != idleBufferIndex))
        {
            txPduBufferPtr[idleBufferIndex].canId = pduPtr->id;
            txPduBufferPtr[idleBufferIndex].TxPduIndex = pduPtr->swPduHandle;
#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
            if (txPduConfigPtr->CanIfTxPduTriggerTransmit)
            {
                txPduBufferPtr[idleBufferIndex].length = 0u;
            }
            else
#endif /*STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT*/
            {
                txPduBufferPtr[idleBufferIndex].length = pduPtr->length;
                (void)ILib_memcpy(txPduBufferPtr[idleBufferIndex].data, pduPtr->sdu, (PduLengthType)pduPtr->length);
            }
            txPduBufferPtr[idleBufferIndex].used = TRUE;
            result = E_OK;
        }
    }
    return result;
#else  /*CANIF_TXBUFFER_NUMBER > 0u*/
    CANIF_NOUSED(pduPtr);
    return E_NOT_OK;
#endif /*CANIF_TXBUFFER_NUMBER > 0u*/
}
/*************************************************************************/
/*
 * Brief               free the txbuffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      controller: the id of Can Controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_TXBUFFER_NUMBER > 0u
 * CallByAPI           CanIf_FreeBufInStopped
 */
/*************************************************************************/
FUNC(void, CANIF_CODE) CanIf_FreeControllerTxBuffer(uint8 Controller)
{
#if (CANIF_TXBUFFER_NUMBER > 0u)
    PduIdType txBufferIndex;
    Can_HwHandleType hthId;
    uint8 controllerId;
    uint8 index;
    const uint8* txBufferSizePtr = &CANIF_TXBUFFER_SIZE(0);
    CanIf_TxPduBufferType* txPduBufferPtr;

    for (txBufferIndex = 0u; txBufferIndex < CANIF_TXBUFFER_NUMBER; txBufferIndex++)
    {
        txPduBufferPtr = &CanIf_TxBuffer[txBufferIndex].CanIf_TxPduBufferRef[0];
        for (index = 0u; index < *txBufferSizePtr; index++)
        {
            /* polling txbuffer, find the one already be used */
            if (txPduBufferPtr->used)
            {
                hthId = CANIF_TXPDU(txPduBufferPtr->TxPduIndex).CanIfTxPduHthId;
                controllerId = CANIF_HTH(hthId).CanIfHthCanCtrlId;
                /* check this txbuffer whether used for this controller */
                if (Controller == controllerId)
                {
                    /* set the used flag False so that this buffer can save new pdu */
                    txPduBufferPtr->used = FALSE;
                }
            }
            txPduBufferPtr++;
        }
        txBufferSizePtr++;
    }
#else
    CANIF_NOUSED(Controller);
#endif
}
/*************************************************************************/
/*
 * Brief               Transmit the buffered tx pdu
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTxPduId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_TxConfirmation
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_TransmitBufferedPdu(PduIdType CanTxPduId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (CANIF_TXBUFFER_NUMBER > 0u)
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanTxPduId);
    PduIdType txBufIndex = txPduConfigPtr->TxBufferIdndex;
    uint8 txBufSize = CANIF_TXBUFFER_SIZE(txBufIndex);
    if ((txBufIndex < CANIF_TXBUFFER_NUMBER) && (0u < txBufSize))
    {
        Can_IdType canIdValue;
        Can_IdType canIdValueMin = (Can_IdType)CANIF_UNUSED_UINT32;
        uint8 txIndex = CANIF_UNUSED_UINT8;
        CanIf_TxPduBufferType* txPduBufferPtr = CanIf_TxBuffer[txBufIndex].CanIf_TxPduBufferRef;
        SchM_Enter_CanIf_ExclusiveArea_Channel();
        for (uint8 index = 0u; index < txBufSize; index++)
        {
            if (txPduBufferPtr[index].used)
            {
                canIdValue = (txPduBufferPtr[index].canId) & CANIF_EXTENDED_CANID_MAX;
                if (CANIF_UNUSED_UINT8 == txIndex)
                {
                    canIdValueMin = canIdValue;
                    txIndex = index;
                }
                else
                {
                    if (canIdValueMin > canIdValue)
                    {
                        canIdValueMin = canIdValue;
                        txIndex = index;
                    }
                }
            }
        }
        if (CANIF_UNUSED_UINT8 != txIndex)
        {
            Can_PduType canPdu;
            Can_HwHandleType hthIndex = txPduConfigPtr->CanIfTxPduHthId;
            const CanIfHthCfgType* hthPtr = &CANIF_HTH(hthIndex);
            Can_HwHandleType hthId = hthPtr->CanObjectId;
            uint8 driverId = CanIf_CtrlCfgData[hthPtr->CanIfHthCanCtrlId].CanDriverId;
            canPdu.length = txPduBufferPtr[txIndex].length;
            canPdu.sdu = txPduBufferPtr[txIndex].data;
            canPdu.swPduHandle = txPduBufferPtr[txIndex].TxPduIndex;
            canPdu.id = txPduBufferPtr[txIndex].canId;
#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
            Std_ReturnType canRetValue = Can_DriverApi[driverId].CanWriteApi(hthId, &canPdu);
#else  /*The default version of can driver is 4.2.2*/
            Can_ReturnType canRetValue = Can_DriverApi[driverId].CanWriteApi(hthId, &canPdu);
#endif /* CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431 */
            if (CAN_OK == canRetValue)
            {
                txPduBufferPtr[txIndex].used = FALSE;
            }
        }
        SchM_Exit_CanIf_ExclusiveArea_Channel();
    }
#else
    CANIF_NOUSED(CanTxPduId);
#endif /*CANIF_TXBUFFER_NUMBER > 0u*/
}
#endif /*STD_ON == CANIF_PUBLIC_TX_BUFFERING*/
#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/*************************************************************************/
/*
 * Brief               init the Receive Buffer
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller:the id of can controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_READ_RX_PDU_DATA_API is TRUE
 * CallByAPI           CanIf_FreeBufInStopped
 */
/*************************************************************************/
FUNC(void, CANIF_CODE) CanIf_FreeControllerRxBuffer(uint8 Controller)
{
#if (0u < CANIF_RXBUFFER_NUMBER)
    PduIdType rxPduIndex;
    Can_HwHandleType hrhId;
    CanIf_RxPduBufferType* rxPduBuffer;
    CanIf_RxBufferNumType rxBufferIndex;
    const CanIf_RxPduConfigType* rxPduConfigPtr;
    for (rxPduIndex = 0u; rxPduIndex < CanIf_ConfigStd->CanIfRxPduNumber; rxPduIndex++)
    {
        rxPduConfigPtr = &CANIF_RXPDU(rxPduIndex);
        rxBufferIndex = rxPduConfigPtr->CanIf_RxBufferIndex;
        if (rxBufferIndex < CANIF_RXBUFFER_NUMBER)
        {
            rxPduBuffer = &CanIf_RxBuffer[rxBufferIndex];
            hrhId = rxPduConfigPtr->CanIfRxPduHrhId;
            if (Controller == CANIF_HRH(hrhId).CanIfHrhCanCtrlId)
            {
                /* init the RxBuf, set length to 0. */
                rxPduBuffer->length = 0u;
            }
        }
    }
#else
    CANIF_NOUSED(Controller);
#endif /*0u < CANIF_RXBUFFER_NUMBER*/
}
#endif /*STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API*/
#if (STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API)
/*************************************************************************/
/*
 * Brief               Set the Dynamic Pdu CanId
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanIfTxSduId,CanId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API is TRUE
 * CallByAPI           CanIf_SetDynamicTxId
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_SetDynamicTxIdHandle(PduIdType CanIfTxSduId, Can_IdType CanId)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (CANIF_DYNAMIC_TXPDU_NUMBER > 0u)
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanIfTxSduId);
    if (CANID_DYNAMIC == txPduConfigPtr->CanIfTxPduType)
    {
        SchM_Enter_CanIf_ExclusiveArea_Channel();
        /*CanId:no FD and EXTENDED flag*/
        CanIf_TxPduDynamicCanIds[txPduConfigPtr->CanIfDynamicCanIdIndex] = (CanIf_IdType)CanId;
        SchM_Exit_CanIf_ExclusiveArea_Channel();
    }
#else
    CANIF_NOUSED(CanIfTxSduId);
    CANIF_NOUSED(CanId);
#endif /* CANIF_DYNAMIC_TXPDU_NUMBER > 0u */
}
#endif /*STD_ON == CANIF_PUBLIC_SET_DYNAMIC_TX_ID_API*/
/*************************************************************************/
/*
 * Brief               Get the base Pdu CanId
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      CanTxPduId
 * Param-Name[out]     canPdu
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanIf_PduPack
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_GetBaseCanIdHandle(
    /* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
    PduIdType CanTxPduId,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr,
#endif
    P2VAR(CanIf_IdType, AUTOMATIC, AUTOMATIC) CanId) /* PRQA S 3432 */
{
    const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(CanTxPduId);
#if (CANIF_DYNAMIC_TXPDU_NUMBER > 0u)
    if (CANID_DYNAMIC == txPduConfigPtr->CanIfTxPduType)
    {
        CanIf_IdType baseId = CanIf_TxPduDynamicCanIds[txPduConfigPtr->CanIfDynamicCanIdIndex];
#if (STD_ON == CANIF_META_DATA_SUPPORT)
        if (txPduConfigPtr->TxMetaDataEnable)
        {
            CanIf_IdType canIdMask = txPduConfigPtr->CanIfTxPduCanIdMask;
            CanIf_IdType metaData = CanIf_MetaDataToCanId(PduInfoPtr->MetaDataPtr);
            *CanId = (baseId & canIdMask) | (metaData & (~canIdMask));
        }
        else
        {
            *CanId = baseId;
        }
#else  /*STD_ON == CANIF_META_DATA_SUPPORT*/
        *CanId = baseId;
#endif /*STD_ON == CANIF_META_DATA_SUPPORT*/
    }
    else
    {
        *CanId = txPduConfigPtr->CanIfTxPduCanId;
    }
#else  /*CANIF_DYNAMIC_TXPDU_NUMBER > 0u*/
    *CanId = txPduConfigPtr->CanIfTxPduCanId;
#endif /*CANIF_DYNAMIC_TXPDU_NUMBER > 0u*/
}
/*************************************************************************/
/*
 * Brief               init the Dynamic Transmit CanId of one controller
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      Controller: the id of can controller
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        None
 * CallByAPI           CanIf_FreeBufInStopped
 */
/*************************************************************************/
FUNC(void, CANIF_CODE) CanIf_FreeDynamicTxPduCanIds(uint8 Controller)
{
#if (CANIF_DYNAMIC_TXPDU_NUMBER > 0u)
    PduIdType txPduIndex;
    Can_HwHandleType hthId;
    uint8 controllerId;
    const CanIf_TxPduConfigType* txPduConfigPtr;
    for (txPduIndex = 0u; txPduIndex < CanIf_ConfigStd->CanIfTxPduNumber; txPduIndex++)
    {
        txPduConfigPtr = &CANIF_TXPDU(txPduIndex);
        hthId = txPduConfigPtr->CanIfTxPduHthId;
        controllerId = CANIF_HTH(hthId).CanIfHthCanCtrlId;
        if (Controller == controllerId)
        {
            if (CANID_DYNAMIC == txPduConfigPtr->CanIfTxPduType)
            {
                /* init the DynamicTxPduCanId with the configured canId */
                CanIf_TxPduDynamicCanIds[txPduConfigPtr->CanIfDynamicCanIdIndex] = txPduConfigPtr->CanIfTxPduCanId;
            }
        }
    }
#else
    CANIF_NOUSED(Controller);
#endif /*CANIF_DYNAMIC_TXPDU_NUMBER > 0u*/
}
/*************************************************************************/
/*
 * Brief               init all Dynamic Transmit CanId
 * ServiceId           None
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              void
 * PreCondition        None
 * CallByAPI           CanIf_InitAllBuffer
 */
/*************************************************************************/
FUNC(void, CANIF_CODE)
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
CanIf_InitDynamicTxPduCanId(VAR(void, AUTOMATIC))
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (0u < CANIF_DYNAMIC_TXPDU_NUMBER)
    PduIdType txPduIndex;
    const CanIf_TxPduConfigType* txPduCfgPtr = &CANIF_TXPDU(0);
    for (txPduIndex = 0u; txPduIndex < CanIf_ConfigStd->CanIfTxPduNumber; txPduIndex++)
    {
        if (CANID_DYNAMIC == txPduCfgPtr->CanIfTxPduType)
        {
            CanIf_TxPduDynamicCanIds[txPduCfgPtr->CanIfDynamicCanIdIndex] = txPduCfgPtr->CanIfTxPduCanId;
        }
        txPduCfgPtr++;
    }
#endif
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
