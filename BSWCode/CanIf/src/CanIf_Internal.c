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
 **  @file               : CanIf_Internal.c
 **  @author             : zhengfei.li
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : CanIf source file for internal API implementations
 **
 ***********************************************************************************************************************/
/* PRQA S 1503 EOF */ /* VL_QAC_NoUsedApi */
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* PRQA S 1505 EOF */ /* VL_CanIf_1505 */
/* PRQA S 2842 EOF */ /* VL_CanIf_2842 */
/* PRQA S 1258 EOF */ /* VL_CanIf_1258 */
/* PRQA S 2983 EOF */ /* VL_CanIf_2983 */
/* =================================================== inclusions =================================================== */
#include "CanIf_Types.h"
#include "CanIf_CfgTypes.h"
#include "CanIf_Internal.h"
#if (STD_ON == CANIF_PARTITION_USED)
#include "Os.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
/**
 * Check if the Tx Pdu Buffer is used
 */
CANIF_LOCAL_INLINE boolean CanIf_TxPduBufferIsUsed(const CanIf_TxPduBufferType* txPduBufferPtr)
{
    return (txPduBufferPtr->TxPduIndex < CanIf_ConfigStd->TxPduConfigNumber);
}

/**
 * Reset the Tx Pdu Buffer
 */
CANIF_LOCAL_INLINE void CanIf_TxPduBufferResetUsed(CanIf_TxPduBufferType* txPduBufferPtr)
{
    txPduBufferPtr->TxPduIndex = CanIf_ConfigStd->TxPduConfigNumber;
}
#endif

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
/**
 *  Re-init rxNotifyStatus and txNotifyStatus of one controller
 */
void CanIf_FreeRxTxNotifyStatus(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr)
{
#if ((CANIF_TXNOTIFSTATUS_MAX_NUMBER > 0u) || (CANIF_RXNOTIFSTATUS_MAX_NUMBER > 0u))
    ApplicationType partitionIndex = CANIF_PARTITION_INDEX(ctrlPbConfigPtr);

#if (CANIF_TXNOTIFSTATUS_MAX_NUMBER > 0u)
    if ((CANIF_TXNOTIFSTATUS_NUMBER(partitionIndex) > 0u)
        && (ctrlPbConfigPtr->TxPduIndexMin < ctrlPbConfigPtr->TxPduIndexMaxExcluded))
    {
        PduIdType                    txPduIndex     = ctrlPbConfigPtr->TxPduIndexMin;
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(txPduIndex);

        for (; txPduIndex < ctrlPbConfigPtr->TxPduIndexMaxExcluded; ++txPduIndex)
        {
            CanIf_NotifStatusType* txNotifStatusPtr = txPduConfigPtr->TxNotifStatusPtr;

            if (NULL_PTR != txNotifStatusPtr)
            {
                *txNotifStatusPtr = CANIF_NO_NOTIFICATION;
            }

            ++txPduConfigPtr;
        }
    }
#endif
#if (CANIF_RXNOTIFSTATUS_MAX_NUMBER > 0u)
    if ((CANIF_RXNOTIFSTATUS_NUMBER(partitionIndex) > 0u)
        && (ctrlPbConfigPtr->RxPduIndexMin < ctrlPbConfigPtr->RxPduIndexMaxExcluded))
    {
        PduIdType                    rxPduIndex     = ctrlPbConfigPtr->RxPduIndexMin;
        const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(rxPduIndex);

        for (; rxPduIndex < ctrlPbConfigPtr->RxPduIndexMaxExcluded; ++rxPduIndex)
        {
            CanIf_NotifStatusType* rxNotifStatusPtr = rxPduConfigPtr->RxNotifStatusPtr;

            if (NULL_PTR != rxNotifStatusPtr)
            {
                *rxNotifStatusPtr = CANIF_NO_NOTIFICATION;
            }

            ++rxPduConfigPtr;
        }
    }
#endif
#else
    CANIF_NOUSED(ctrlPbConfigPtr);
#endif
}
#endif

#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
/**
 * init all rxNotifyStatus and txNotifyStatus
 */
void CanIf_InitTxBuffer(ApplicationType partitionIndex)
{
#if (CANIF_TXBUFFER_MAX_NUMBER > 0u)
    if (CANIF_TXBUFFER_CONFIG_NUMBER(partitionIndex) > 0u)
    {
        CanIf_TxByteBufferNumberType  txByteBufferIndex = 0u;
        const CanIf_BufferConfigType* bufferConfigPtr   = &CANIF_TXBUFFER_CONFIG(partitionIndex, 0u);
        CanIf_TxBufferType*           txBufferPtr       = &CANIF_TXBUFFER(partitionIndex, 0u);
        CanIf_TxPduBufferType*        txPduBufferPtr    = &CANIF_TXPDUBUFFER(partitionIndex, 0u);

        for (CanIf_TxBufferNumberType txBufferIndex = 0u; txBufferIndex < CANIF_TXBUFFER_CONFIG_NUMBER(partitionIndex);
             ++txBufferIndex)
        {
            txBufferPtr->BufferSize     = bufferConfigPtr->BufferSize;
            txBufferPtr->TxPduBufferRef = txPduBufferPtr;

            for (CanIf_TxPduBufferNumberType sizeIndex = 0u; sizeIndex < bufferConfigPtr->BufferSize; ++sizeIndex)
            {
                CanIf_TxPduBufferResetUsed(txPduBufferPtr);
                txPduBufferPtr->Data = &CANIF_TXBYTEBUFFER(partitionIndex, txByteBufferIndex);

                txByteBufferIndex += bufferConfigPtr->PduLength;
                ++txPduBufferPtr;
            }

            ++bufferConfigPtr;
            ++txBufferPtr;
        }
    }
#else
    CANIF_NOUSED(partitionIndex);
#endif
}
#endif

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/**
 * init RxBuffer
 */
void CanIf_InitRxBuffer(ApplicationType partitionIndex)
{
#if (CANIF_RXBUFFER_MAX_NUMBER > 0u)
    if (CANIF_RXBUFFER_CONFIG_NUMBER(partitionIndex) > 0u)
    {
        CanIf_RxByteBufferNumberType  rxByteBufferIndex = 0u;
        const CanIf_BufferConfigType* bufferConfigPtr   = &CANIF_RXBUFFER_CONFIG(partitionIndex, 0u);
        CanIf_RxBufferType*           rxBufferPtr       = &CANIF_RXBUFFER(partitionIndex, 0u);

        for (CanIf_RxBufferNumberType rxBufferIndex = 0u; rxBufferIndex < CANIF_RXBUFFER_CONFIG_NUMBER(partitionIndex);
             ++rxBufferIndex)
        {
            CanIf_RxPduBufferType* rxPduBufferPtr = &rxBufferPtr->RxPduBuffer;

            rxBufferPtr->PduLength = bufferConfigPtr->PduLength;

            rxPduBufferPtr->Length = 0u;
            rxPduBufferPtr->Data   = &CANIF_RXBYTEBUFFER(partitionIndex, rxByteBufferIndex);

            rxByteBufferIndex += bufferConfigPtr->PduLength;

            ++bufferConfigPtr;
            ++rxBufferPtr;
        }
    }
#else
    CANIF_NOUSED(partitionIndex);
#endif
}
#endif

#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
/**
 * init all rxNotifyStatus and txNotifyStatus
 */
void CanIf_InitRxTxNotifyStatus(ApplicationType partitionIndex)
{
    CANIF_NOUSED(partitionIndex);

#if (CANIF_TXNOTIFSTATUS_MAX_NUMBER > 0u)
    if (CANIF_TXNOTIFSTATUS_NUMBER(partitionIndex) > 0u)
    {
        CanIf_NotifStatusType* txNotifStatusPtr = &CANIF_TXNOTIFSTATUS(partitionIndex, 0u);

        for (PduIdType txNotifStatusIndex = 0u; txNotifStatusIndex < CANIF_TXNOTIFSTATUS_NUMBER(partitionIndex);
             ++txNotifStatusIndex)
        {
            *txNotifStatusPtr = CANIF_NO_NOTIFICATION;
            ++txNotifStatusPtr;
        }
    }
#endif
#if (CANIF_RXNOTIFSTATUS_MAX_NUMBER > 0u)
    if (CANIF_RXNOTIFSTATUS_NUMBER(partitionIndex) > 0u)
    {
        CanIf_NotifStatusType* rxNotifStatusPtr = &CANIF_RXNOTIFSTATUS(partitionIndex, 0u);

        for (PduIdType rxNotifStatusIndex = 0u; rxNotifStatusIndex < CANIF_RXNOTIFSTATUS_NUMBER(partitionIndex);
             ++rxNotifStatusIndex)
        {
            *rxNotifStatusPtr = CANIF_NO_NOTIFICATION;
            ++rxNotifStatusPtr;
        }
    }
#endif
}
#endif

#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
/**
 * CanIf call can_write which return CAN_BSUY,then set the write requece and data to tx buffer
 */
Std_ReturnType CanIf_TxBusyHandle(const CanIf_TxPduConfigType* txPduConfigPtr, const Can_PduType* canPduPtr)
{
    Std_ReturnType result = E_NOT_OK;

#if (CANIF_TXBUFFER_MAX_NUMBER > 0u)
    PduIdType                txPduId        = canPduPtr->swPduHandle;
    PduIdType                txPduIndex     = CanIf_TxPduId2VarIndex(txPduId);
    ApplicationType          partitionIndex = CANIF_PARTITION_INDEX(txPduConfigPtr);
    CanIf_TxBufferNumberType txBufferIndex  = txPduConfigPtr->TxBufferIndex;

    if (txBufferIndex < CANIF_TXBUFFER_NUMBER(partitionIndex))
    {
        const CanIf_TxBufferType* txBufferPtr        = &CANIF_TXBUFFER(partitionIndex, txBufferIndex);
        CanIf_TxPduBufferType*    txPduBufferPtr     = txBufferPtr->TxPduBufferRef;
        CanIf_TxPduBufferType*    idleTxPduBufferPtr = NULL_PTR;

        for (CanIf_TxPduBufferNumberType sizeIndex = 0u; sizeIndex < txBufferPtr->BufferSize; ++sizeIndex)
        {
            if (txPduBufferPtr->TxPduIndex == txPduIndex)
            {
                idleTxPduBufferPtr = txPduBufferPtr;
                break;
            }
            else
            {
                if ((NULL_PTR == idleTxPduBufferPtr) && !CanIf_TxPduBufferIsUsed(txPduBufferPtr))
                {
                    idleTxPduBufferPtr = txPduBufferPtr;
                }
            }

            ++txPduBufferPtr;
        }
        if (NULL_PTR != idleTxPduBufferPtr)
        {
            idleTxPduBufferPtr->CanId = canPduPtr->id;
#if (STD_ON == CANIF_TRIGGER_TRANSMIT_SUPPORT)
            if (txPduConfigPtr->UpTriggerTransmitApiIndex < CANIF_UPTRIGGERTRANSMIT_API_NUMBER)
            {
                idleTxPduBufferPtr->Length = 0u;
            }
            else
#endif
            {
                idleTxPduBufferPtr->Length = canPduPtr->length;
                (void)IStdLib_MemCpy(idleTxPduBufferPtr->Data, canPduPtr->sdu, canPduPtr->length);
            }
            idleTxPduBufferPtr->TxPduId    = txPduId;
            idleTxPduBufferPtr->TxPduIndex = txPduIndex;
            result                         = E_OK;
        }
    }
#else
    CANIF_NOUSED(txPduConfigPtr);
    CANIF_NOUSED(canPduPtr);
#endif

    return result;
}

/**
 * free the txbuffer
 */
void CanIf_FreeControllerTxBuffer(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr)
{
#if (CANIF_TXBUFFER_MAX_NUMBER > 0u)
    ApplicationType partitionIndex = CANIF_PARTITION_INDEX(ctrlPbConfigPtr);

    if ((CANIF_TXBUFFER_NUMBER(partitionIndex) > 0u)
        && (ctrlPbConfigPtr->TxPduIndexMin < ctrlPbConfigPtr->TxPduIndexMaxExcluded))
    {
        const CanIf_TxBufferType* txBufferPtr = &CANIF_TXBUFFER(partitionIndex, 0u);

        for (CanIf_TxBufferNumberType txBufferIndex = 0u; txBufferIndex < CANIF_TXBUFFER_NUMBER(partitionIndex);
             ++txBufferIndex)
        {
            CanIf_TxPduBufferType* txPduBufferPtr = txBufferPtr->TxPduBufferRef;

            for (CanIf_TxPduBufferNumberType sizeIndex = 0u; sizeIndex < txBufferPtr->BufferSize; ++sizeIndex)
            {
                if ((txPduBufferPtr->TxPduIndex >= ctrlPbConfigPtr->TxPduIndexMin)
                    && (txPduBufferPtr->TxPduIndex < ctrlPbConfigPtr->TxPduIndexMaxExcluded))
                {
                    CanIf_TxPduBufferResetUsed(txPduBufferPtr);
                }

                ++txPduBufferPtr;
            }

            ++txBufferPtr;
        }
    }
#else
    CANIF_NOUSED(ctrlPbConfigPtr);
#endif
}

/**
 * Transmit the buffered tx pdu
 */
void CanIf_TransmitBufferedPdu(const CanIf_TxPduConfigType* txPduConfigPtr)
{
#if (CANIF_TXBUFFER_MAX_NUMBER > 0u)
    ApplicationType          partitionIndex = CANIF_PARTITION_INDEX(txPduConfigPtr);
    CanIf_TxBufferNumberType txBufferIndex  = txPduConfigPtr->TxBufferIndex;

    if (txBufferIndex < CANIF_TXBUFFER_NUMBER(partitionIndex))
    {
        const CanIf_TxBufferType* txBufferPtr        = &CANIF_TXBUFFER(partitionIndex, txBufferIndex);
        CanIf_TxPduBufferType*    txPduBufferPtr     = txBufferPtr->TxPduBufferRef;
        CanIf_TxPduBufferType*    usedTxPduBufferPtr = NULL_PTR;
        Can_IdType                canIdValueMin      = CANIF_UNUSED_UINT32;

        for (CanIf_TxPduBufferNumberType sizeIndex = 0u; sizeIndex < txBufferPtr->BufferSize; ++sizeIndex)
        {
            if (CanIf_TxPduBufferIsUsed(txPduBufferPtr))
            {
                Can_IdType canIdValue = (txPduBufferPtr->CanId) & CANIF_EXTENDED_CANID_MAX;

                if ((NULL_PTR == usedTxPduBufferPtr) || (canIdValueMin > canIdValue))
                {
                    usedTxPduBufferPtr = txPduBufferPtr;
                    canIdValueMin      = canIdValue;
                }
            }

            ++txPduBufferPtr;
        }
        if (NULL_PTR != usedTxPduBufferPtr)
        {
            Can_HwHandleType            hthIndex      = txPduConfigPtr->CanIfTxPduHthId;
            const CanIf_HthConfigType*  hthConfigPtr  = &CANIF_HTH(hthIndex);
            uint8                       ctrlIndex     = hthConfigPtr->CanIfHthCanCtrlId;
            const CanIf_CtrlConfigType* ctrlConfigPtr = &CanIf_CtrlCfgData[ctrlIndex];
            uint8                       canDrvId      = ctrlConfigPtr->CanDriverId;
            Can_HwHandleType            canHthId      = hthConfigPtr->CanObjectId;
            Can_PduType                 canPdu        = {0};

            canPdu.length      = usedTxPduBufferPtr->Length;
            canPdu.sdu         = usedTxPduBufferPtr->Data;
            canPdu.swPduHandle = usedTxPduBufferPtr->TxPduId;
            canPdu.id          = usedTxPduBufferPtr->CanId;

#if (CANIF_CAN_AUTOSAR_VERSION >= CANIF_CAN_AUTOSAR_431)
            Std_ReturnType canRetValue = Can_DriverApi[canDrvId].CanWriteApi(canHthId, &canPdu);
#else /*The default version of can driver is 4.2.2*/
            Can_ReturnType canRetValue = Can_DriverApi[canDrvId].CanWriteApi(canHthId, &canPdu);
#endif
            if (CAN_OK == canRetValue)
            {
                CanIf_TxPduBufferResetUsed(usedTxPduBufferPtr);
            }
        }
    }
#else
    CANIF_NOUSED(txPduConfigPtr);
#endif
}
#endif

/**
 * notify negative tx confirmation to upper layer
 */
void CanIf_NotifyNegativeTxConfirmation(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr)
{
    if (ctrlPbConfigPtr->TxPduIndexMin < ctrlPbConfigPtr->TxPduIndexMaxExcluded)
    {
        PduIdType                    txPduIndex     = ctrlPbConfigPtr->TxPduIndexMin;
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(txPduIndex);

        for (; txPduIndex < ctrlPbConfigPtr->TxPduIndexMaxExcluded; ++txPduIndex)
        {
            if (NULL_PTR != txPduConfigPtr->UpConfirmTxPduPtr)
            {
                boolean unconfirmedTxPdu = FALSE;

                SchM_Enter_CanIf_ExclusiveArea_Channel();
                unconfirmedTxPdu = CanIf_ClearUpConfirmTxPduStatus(txPduConfigPtr);
                SchM_Exit_CanIf_ExclusiveArea_Channel();

                if (unconfirmedTxPdu)
                {
                    CanIf_UpTxConfirmationArray[txPduConfigPtr->UpTxConfirmationApiIndex](
                        txPduConfigPtr->CanIfUpPduId,
                        E_NOT_OK);
                }
            }

            ++txPduConfigPtr;
        }
    }
}

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/**
 * init the Receive Buffer
 */
void CanIf_FreeControllerRxBuffer(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr)
{
#if (CANIF_RXBUFFER_MAX_NUMBER > 0u)
    ApplicationType partitionIndex = CANIF_PARTITION_INDEX(ctrlPbConfigPtr);

    if ((CANIF_RXBUFFER_NUMBER(partitionIndex) > 0u)
        && (ctrlPbConfigPtr->RxPduIndexMin < ctrlPbConfigPtr->RxPduIndexMaxExcluded))
    {
        PduIdType                    rxPduIndex     = ctrlPbConfigPtr->RxPduIndexMin;
        const CanIf_RxPduConfigType* rxPduConfigPtr = &CANIF_RXPDU(rxPduIndex);

        for (; rxPduIndex < ctrlPbConfigPtr->RxPduIndexMaxExcluded; ++rxPduIndex)
        {
            CanIf_RxBufferNumberType rxBufferIndex = rxPduConfigPtr->RxBufferIndex;

            if (rxBufferIndex < CANIF_RXBUFFER_NUMBER(partitionIndex))
            {
                CanIf_RxBufferType*    rxBufferPtr    = &CANIF_RXBUFFER(partitionIndex, rxBufferIndex);
                CanIf_RxPduBufferType* rxPduBufferPtr = &rxBufferPtr->RxPduBuffer;

                /* init the RxBuf, set length to 0. */
                rxPduBufferPtr->Length = 0u;
            }

            ++rxPduConfigPtr;
        }
    }
#else
    CANIF_NOUSED(ctrlPbConfigPtr);
#endif
}
#endif

/**
 * Get the base Pdu CanId
 */
void CanIf_GetBaseCanIdHandle(
    const CanIf_TxPduConfigType* txPduConfigPtr,
#if (STD_ON == CANIF_META_DATA_SUPPORT)
    const PduInfoType* pduInfoPtr,
#endif
    CanIf_IdType* canIdPtr)
{
    CanIf_IdType canId;

#if (CANIF_DYNAMICTXPDU_MAX_NUMBER > 0u)
    const CanIf_DynamicTxPduType* dynamicTxPduPtr = txPduConfigPtr->DynamicTxPduPtr;

    if (NULL_PTR != dynamicTxPduPtr)
    {
        CanIf_IdType baseId = dynamicTxPduPtr->CanId;

#if (STD_ON == CANIF_META_DATA_SUPPORT)
        if (txPduConfigPtr->TxMetaDataEnable)
        {
            CanIf_IdType canIdMask = txPduConfigPtr->CanIfTxPduCanIdMask;
            CanIf_IdType metaData  = CanIf_MetaDataToCanId(pduInfoPtr->MetaDataPtr);

            canId = (baseId & canIdMask) | (metaData & (~canIdMask));
        }
        else
#endif
        {
            canId = baseId;
        }
    }
    else
#endif
    {
        canId = txPduConfigPtr->CanIfTxPduCanId;
    }

    /* PRQA S 3315 ++ */ /* VL_CanIf_3315 */
    /* @ref https://gitlab.i-soft.com.cn/prd/r23_bsw/-/merge_requests/1130 */
    /* set the 2 bits of highest CanId base the CanIf_TxPduCanIdModeType */
    switch (txPduConfigPtr->CanIfTxPduCanIdType)
    {
#if (STD_ON == CANIF_TX_EXTENDED_CAN_SUPPORT)
    case CANIF_TX_EXTENDED_CAN:
        canId |= (CanIf_IdType)CANIF_CANID_EXTENDED_MASK;
        break;
#endif
#if (STD_ON == CANIF_TX_EXTENDED_FD_CAN_SUPPORT)
    case CANIF_TX_EXTENDED_FD_CAN:
        canId |= (CanIf_IdType)(CANIF_CANID32_CANFD_MASK | CANIF_CANID_EXTENDED_MASK);
        break;
#endif
#if (STD_ON == CANIF_TX_STANDARD_FD_CAN_SUPPORT)
    case CANIF_TX_STANDARD_FD_CAN:
        canId |= (CanIf_IdType)CANIF_CANID32_CANFD_MASK;
        break;
#endif
    default:
        /* do nothing */
        break;
    }
    /* PRQA S 3315 -- */ /* VL_CanIf_3315 */

    *canIdPtr = canId;
}

/**
 * init the Dynamic Transmit CanId of one controller
 */
void CanIf_FreeDynamicTxPduCanIds(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr)
{
#if (CANIF_DYNAMICTXPDU_MAX_NUMBER > 0u)
    ApplicationType partitionIndex = CANIF_PARTITION_INDEX(ctrlPbConfigPtr);

    if ((CANIF_DYNAMICTXPDU_NUMBER(partitionIndex) > 0u)
        && (ctrlPbConfigPtr->TxPduIndexMin < ctrlPbConfigPtr->TxPduIndexMaxExcluded))
    {
        PduIdType                    txPduIndex     = ctrlPbConfigPtr->TxPduIndexMin;
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(txPduIndex);

        for (; txPduIndex < ctrlPbConfigPtr->TxPduIndexMaxExcluded; ++txPduIndex)
        {
            CanIf_DynamicTxPduType* dynamicTxPduPtr = txPduConfigPtr->DynamicTxPduPtr;

            if (NULL_PTR != dynamicTxPduPtr)
            {
                /* init the DynamicTxPduCanId with the configured canId */
                dynamicTxPduPtr->CanId = txPduConfigPtr->CanIfTxPduCanId;
            }

            ++txPduConfigPtr;
        }
    }
#else
    CANIF_NOUSED(ctrlPbConfigPtr);
#endif
}

/**
 * init all Dynamic Transmit CanId
 */
void CanIf_InitDynamicTxPduCanId(ApplicationType partitionIndex)
{
#if (CANIF_DYNAMICTXPDU_MAX_NUMBER > 0u)
    if ((CanIf_ConfigStd->TxPduConfigNumber > 0u) && (CANIF_DYNAMICTXPDU_NUMBER(partitionIndex) > 0u))
    {
        const CanIf_TxPduConfigType* txPduConfigPtr = &CANIF_TXPDU(0u);

        for (PduIdType txPduIndex = 0u; txPduIndex < CanIf_ConfigStd->TxPduConfigNumber; ++txPduIndex)
        {
            CanIf_DynamicTxPduType* dynamicTxPduPtr = txPduConfigPtr->DynamicTxPduPtr;

            if ((CANIF_PARTITION_INDEX(txPduConfigPtr) == partitionIndex) && (NULL_PTR != dynamicTxPduPtr))
            {
                dynamicTxPduPtr->CanId = txPduConfigPtr->CanIfTxPduCanId;
            }

            ++txPduConfigPtr;
        }
    }
#else
    CANIF_NOUSED(partitionIndex);
#endif
}

/**
 * init all tx confirmation status
 */
void CanIf_InitUpConfirmTxPduStatus(ApplicationType partitionIndex)
{
#if (CANIF_UPCONFIRMTXPDU_MAX_NUMBER > 0u)
    if (CANIF_UPCONFIRMTXPDU_NUMBER(partitionIndex) > 0u)
    {
        CanIf_UpConfirmTxPduType* upCfmTxPduPtr = &CANIF_UPCONFIRMTXPDU(partitionIndex, 0u);

        for (PduIdType upCfmTxPduIndex = 0u; upCfmTxPduIndex < CANIF_UPCONFIRMTXPDU_NUMBER(partitionIndex);
             ++upCfmTxPduIndex)
        {
            upCfmTxPduPtr->UpConfirmStatus = 0u;
            ++upCfmTxPduPtr;
        }
    }
#else
    CANIF_NOUSED(partitionIndex);
#endif
}

/**
 * init all controller runtime status
 */
void CanIf_InitCtrlRuntime(ApplicationType partitionIndex)
{
    const CanIf_CtrlPbConfigType* ctrlPbConfigPtr = &CANIF_CTRL_PBCFG(0u);
    const CanIf_CtrlConfigType*   ctrlConfigPtr   = &CanIf_CtrlCfgData[0u];

    for (uint8 ctrlIndex = 0u; ctrlIndex < CANIF_CANCONTROLLER_NUMBER; ++ctrlIndex)
    {
        if (CANIF_PARTITION_INDEX(ctrlPbConfigPtr) == partitionIndex)
        {
            CanIf_CtrlRuntimeType* ctrlRuntimePtr = ctrlPbConfigPtr->RuntimePtr;

            ctrlRuntimePtr->ControllerMode = CAN_CS_STOPPED;
            ctrlRuntimePtr->PduMode        = CANIF_OFFLINE;
#if (STD_ON == CANIF_PUBLIC_WAKEUP_CHECK_VALID_SUPPORT)
            /* Init the first call rxindication event flag */
            ctrlRuntimePtr->FirstCallRxInd = FALSE;
#endif
#if (STD_ON == CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT)
            ctrlRuntimePtr->TxConfirmationState = CANIF_NO_NOTIFICATION;
#endif
#if (STD_ON == CANIF_PUBLIC_PN_SUPPORT)
            if (ctrlConfigPtr->CanIfPnFilter)
            {
                ctrlRuntimePtr->PnTxFilter = TRUE;
            }
            else
            {
                ctrlRuntimePtr->PnTxFilter = FALSE;
            }
#endif
        }

        ++ctrlPbConfigPtr;
        ++ctrlConfigPtr;
    }
}

#if (STD_ON == CANIF_PARTITION_USED)
#if (CANIF_PARTITION_ID2INDEX_NUMBER > 0u)
/**
 * get partition index of current application ID
 */
ApplicationType CanIf_GetCurrentPartition(void)
{
    ApplicationType partitionId = GetApplicationID();
    ApplicationType partitionIndex;

    if (partitionId < CANIF_PARTITION_ID2INDEX_NUMBER)
    {
        partitionIndex = CanIf_PartitionId2Index[partitionId];
    }
    else
    {
        partitionIndex = CANIF_PARTITION_NUMBER; /* invalid index */
    }

    return partitionIndex;
}
#endif
#endif

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
