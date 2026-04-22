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
 **  @file               : CanIf_Internal.h
 **  @author             : zhengfei.li
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : CanIf header file for internal API declarations
 **
 ***********************************************************************************************************************/

#ifndef CANIF_INTERNAL_H_
#define CANIF_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "CanIf.h"
#include "CanIf_Types.h"
#include "CanIf_CfgTypes.h"
#include "SchM_CanIf.h"
#include "IStdLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define CANIF_NOUSED(paramter) (void)(paramter)

#if !defined(CANIF_LOCAL)
#define CANIF_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

#if !defined(CANIF_LOCAL_INLINE)
#define CANIF_LOCAL_INLINE static inline
#endif

/* PRQA S 3472 ++ */ /* VL_CanIf_3472 */
#define CANIF_TXPDU_REF               CanIf_ConfigStd->TxPduConfigRef
#define CANIF_TXPDU(txPduIndex)       CANIF_TXPDU_REF[(txPduIndex)]
#define CANIF_HTH_REF                 CanIf_ConfigStd->HthConfigRef
#define CANIF_HTH(hthIndex)           CANIF_HTH_REF[(hthIndex)]
#define CANIF_RXPDU_REF               CanIf_ConfigStd->RxPduConfigRef
#define CANIF_RXPDU(rxPduIndex)       CANIF_RXPDU_REF[(rxPduIndex)]
#define CANIF_HRH_REF                 CanIf_ConfigStd->HrhConfigRef
#define CANIF_HRH(hrhIndex)           CANIF_HRH_REF[(hrhIndex)]
#define CANIF_CTRL_PBCFG_REF          CanIf_ConfigStd->CtrlPbConfigRef
#define CANIF_CTRL_PBCFG(ctrlIndex)   CANIF_CTRL_PBCFG_REF[(ctrlIndex)]
#define CANIF_TRCV_PBCFG_REF          CanIf_ConfigStd->TrcvPbConfigRef
#define CANIF_TRCV_PBCFG(trcvIndex)   CANIF_TRCV_PBCFG_REF[(trcvIndex)]
#define CANIF_HOH2HRH_REF             CanIf_ConfigStd->CanHoh2HrhRef
#define CANIF_HOH2HRH(ctrlIndex)      CANIF_HOH2HRH_REF[CANIF_CONTROLLER_DRV_ID(ctrlIndex)]
#define CANIF_TXPDU_VARINDEX_REF      CanIf_ConfigStd->TxPduIdVariantIndexRef
#define CANIF_TXPDU_VARINDEX(txPduId) CANIF_TXPDU_VARINDEX_REF[(txPduId)]
#define CANIF_RXPDU_VARINDEX_REF      CanIf_ConfigStd->RxPduIdVariantIndexRef
#define CANIF_RXPDU_VARINDEX(rxPduId) CANIF_RXPDU_VARINDEX_REF[(rxPduId)]

#if (STD_ON == CANIF_PARTITION_USED)
#define CANIF_PARTITION_INDEX(cfgPtr) (cfgPtr)->PartitionIndex
#else
#define CANIF_PARTITION_INDEX(cfgPtr) 0u
#endif

#define CANIF_TXBUFFER_CONFIG_NUMBER(partitionIndex) \
    (CanIf_ConfigStd->TxBufferConfigRef[(partitionIndex)].BufferConfigNumber)
#define CANIF_TXBUFFER_CONFIG(partitionIndex, txBufferIndex) \
    (CanIf_ConfigStd->TxBufferConfigRef[(partitionIndex)].BufferConfigRef[(txBufferIndex)])

#define CANIF_HTHBUFFER_CONFIG_NUMBER(partitionIndex) \
    (CanIf_ConfigStd->HthBufferConfigRef[(partitionIndex)].BufferConfigNumber)
#define CANIF_HTHBUFFER_CONFIG(partitionIndex, rxBufferIndex) \
    (CanIf_ConfigStd->HthBufferConfigRef[(partitionIndex)].BufferConfigRef[(rxBufferIndex)])

#define CANIF_RXBUFFER_CONFIG_NUMBER(partitionIndex) \
    (CanIf_ConfigStd->RxBufferConfigRef[(partitionIndex)].BufferConfigNumber)
#define CANIF_RXBUFFER_CONFIG(partitionIndex, rxBufferIndex) \
    (CanIf_ConfigStd->RxBufferConfigRef[(partitionIndex)].BufferConfigRef[(rxBufferIndex)])
/* PRQA S 3472 -- */ /* VL_CanIf_3472 */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */
extern const CanIf_ConfigType* CanIf_ConfigStd;

/* ========================================== external function definitions ========================================= */
#if ((STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API) || (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API))
/**
 * @brief              Re-init rxNotifyStatus and txNotifyStatus of one controller
 * @param[in]          ctrlPbConfigPtr: pointer to PB config of CAN controller
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_FreeRxTxNotifyStatus(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr);

/**
 * @brief              init all rxNotifyStatus and txNotifyStatus
 * @param[in]          partitionIndex: index of partition
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_InitRxTxNotifyStatus(ApplicationType partitionIndex);
#endif

#if (STD_ON == CANIF_PUBLIC_TX_BUFFERING)
/**
 * @brief              init TxBuffer
 * @param[in]          partitionIndex: index of partition
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_InitTxBuffer(ApplicationType partitionIndex);

/**
 * @brief              tx busy handle
 * @param[in]          txPduConfigPtr: pointer to PB config of TxPdu
 * @param[in]          canPduPtr: pointer to the pdu struct
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Std_ReturnType CanIf_TxBusyHandle(const CanIf_TxPduConfigType* txPduConfigPtr, const Can_PduType* canPduPtr);

/**
 * @brief              free TxBuffer
 * @param[in]          ctrlPbConfigPtr: pointer to PB config of CAN controller
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_FreeControllerTxBuffer(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr);

/**
 * @brief              transmit pdu in TxBuffer
 * @param[in]          txPduConfigPtr: pointer to config of TxPdu
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_TransmitBufferedPdu(const CanIf_TxPduConfigType* txPduConfigPtr);
#endif

/**
 * @brief              notify negative tx confirmation
 * @param[in]          ctrlPbConfigPtr: pointer to PB config of CAN controller
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_NotifyNegativeTxConfirmation(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr);

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_DATA_API)
/**
 * @brief              init RxBuffer
 * @param[in]          partitionIndex: index of partition
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_InitRxBuffer(ApplicationType partitionIndex);

/**
 * @brief              init the Receive Buffer
 * @param[in]          ctrlPbConfigPtr: pointer to PB config of CAN controller
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_FreeControllerRxBuffer(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr);
#endif

#if (STD_ON == CANIF_META_DATA_SUPPORT)
/**
 * @brief              Get the base Pdu CanId
 * @param[in]          txPduConfigPtr: pointer to config of TxPdu
 * @param[in]          pduInfoPtr: pointer to PDU info
 * @param[out]         canIdPtr: pointer to ID of CAN controller
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_GetBaseCanIdHandle(
    const CanIf_TxPduConfigType* txPduConfigPtr,
    const PduInfoType*           pduInfoPtr,
    CanIf_IdType*                canIdPtr);
#else
extern void CanIf_GetBaseCanIdHandle(const CanIf_TxPduConfigType* txPduConfigPtr, CanIf_IdType* canIdPtr);
#endif

/**
 * @brief              init the Dynamic Transmit CanId of one controller
 * @param[in]          ctrlPbConfigPtr: pointer to PB config of CAN controller
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_FreeDynamicTxPduCanIds(const CanIf_CtrlPbConfigType* ctrlPbConfigPtr);

/**
 * @brief              init all Dynamic Transmit CanId
 * @param[in]          partitionIndex: index of partition
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_InitDynamicTxPduCanId(ApplicationType partitionIndex);

/**
 * @brief              init all tx confirmation status
 * @param[in]          partitionIndex: index of partition
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_InitUpConfirmTxPduStatus(ApplicationType partitionIndex);

/**
 * @brief              init all controller runtime status
 * @param[in]          partitionIndex: index of partition
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void CanIf_InitCtrlRuntime(ApplicationType partitionIndex);

#if (STD_ON == CANIF_PARTITION_USED)
#if (CANIF_PARTITION_ID2INDEX_NUMBER > 0u)
/**
 * @brief              find the partition index of the current partition ID
 * @return             ApplicationType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern ApplicationType CanIf_GetCurrentPartition(void);
#else
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
CANIF_LOCAL_INLINE ApplicationType CanIf_GetCurrentPartition(void)
{
    ApplicationType partitionIndex = GetApplicationID();

    return partitionIndex;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
#else
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
CANIF_LOCAL_INLINE ApplicationType CanIf_GetCurrentPartition(void)
{
    ApplicationType partitionIndex = 0u;

    return partitionIndex;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

#if (STD_ON == CANIF_PUBLIC_READ_RX_PDU_NOTIFY_STATUS_API)
/**
 * @brief              Set RxNotifyStatus to CANIF_TX_RX_NOTIFICATION
 * @param[in]          rxPduConfigPtr: pointer to config of RxPdu
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
CANIF_LOCAL_INLINE void CanIf_SetRxNotifStatusHandle(const CanIf_RxPduConfigType* rxPduConfigPtr)
{
#if (CANIF_RXNOTIFSTATUS_MAX_NUMBER > 0u)
    CanIf_NotifStatusType* rxNotifStatusPtr = rxPduConfigPtr->RxNotifStatusPtr;

    if (NULL_PTR != rxNotifStatusPtr)
    {
        *rxNotifStatusPtr = CANIF_TX_RX_NOTIFICATION;
    }
#else
    CANIF_NOUSED(rxPduConfigPtr);
#endif
}
#endif

#if (STD_ON == CANIF_PUBLIC_READ_TX_PDU_NOTIFY_STATUS_API)
/**
 * @brief              Set TxNotifyStatus to CANIF_TX_RX_NOTIFICATION
 * @param[in]          txPduConfigPtr: pointer to config of TxPdu
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
CANIF_LOCAL_INLINE void CanIf_SetTxNotifStatusHandle(const CanIf_TxPduConfigType* txPduConfigPtr)
{
#if (CANIF_TXNOTIFSTATUS_MAX_NUMBER > 0u)
    CanIf_NotifStatusType* txNotifStatusPtr = txPduConfigPtr->TxNotifStatusPtr;

    if (NULL_PTR != txNotifStatusPtr)
    {
        *txNotifStatusPtr = CANIF_TX_RX_NOTIFICATION;
    }
#else
    CANIF_NOUSED(txPduConfigPtr);
#endif
}
#endif

#if (STD_ON == CANIF_META_DATA_SUPPORT)
/**
 * @brief              MetaData-to-CanId transition
 * @param[in]          meteData: Pointer to MetaData array
 * @return             CanIf_IdType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
CANIF_LOCAL_INLINE CanIf_IdType CanIf_MetaDataToCanId(const uint8* meteData)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#if (TRUE == CPU_32_WITH_16_ADR)
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    return (CanIf_IdType)(meteData[0u] & CANIF_UNUSED_UINT8) | ((CanIf_IdType)(meteData[1u] & CANIF_UNUSED_UINT8) << 8u)
           | ((CanIf_IdType)(meteData[2u] & CANIF_UNUSED_UINT8) << 16u)
           | ((CanIf_IdType)(meteData[3u] & CANIF_UNUSED_UINT8) << 24u);
#else
    return (CanIf_IdType)(meteData[3u] & CANIF_UNUSED_UINT8) | ((CanIf_IdType)(meteData[2u] & CANIF_UNUSED_UINT8) << 8u)
           | ((CanIf_IdType)(meteData[1u] & CANIF_UNUSED_UINT8) << 16u)
           | ((CanIf_IdType)(meteData[0u] & CANIF_UNUSED_UINT8) << 24u);
#endif
#else
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    return (CanIf_IdType)meteData[0u] | ((CanIf_IdType)meteData[1u] << 8u) | ((CanIf_IdType)meteData[2u] << 16u)
           | ((CanIf_IdType)meteData[3u] << 24u);
#else
    return (CanIf_IdType)meteData[3u] | ((CanIf_IdType)meteData[2u] << 8u) | ((CanIf_IdType)meteData[1u] << 16u)
           | ((CanIf_IdType)meteData[0u] << 24u);
#endif
#endif
    /* PRQA S 3120 -- */
}

/**
 * @brief              CanId-to-MetaData transition
 * @param[in]          canId: CAN ID
 * @param[out]         meteData: Pointer to MetaData array
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
CANIF_LOCAL_INLINE void CanIf_CanIdToMetaData(CanIf_IdType canId, uint8* meteData)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#if (TRUE == CPU_32_WITH_16_ADR)
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    meteData[0u] = (uint8)(canId & CANIF_UNUSED_UINT8);
    meteData[1u] = (uint8)(canId >> 8u & CANIF_UNUSED_UINT8);
    meteData[2u] = (uint8)(canId >> 16u & CANIF_UNUSED_UINT8);
    meteData[3u] = (uint8)(canId >> 24u & CANIF_UNUSED_UINT8);
#else
    meteData[3u] = (uint8)(canId & CANIF_UNUSED_UINT8);
    meteData[2u] = (uint8)(canId >> 8u & CANIF_UNUSED_UINT8);
    meteData[1u] = (uint8)(canId >> 16u & CANIF_UNUSED_UINT8);
    meteData[0u] = (uint8)(canId >> 24u & CANIF_UNUSED_UINT8);
#endif
#else
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
    meteData[0u] = (uint8)(canId);
    meteData[1u] = (uint8)(canId >> 8u);
    meteData[2u] = (uint8)(canId >> 16u);
    meteData[3u] = (uint8)(canId >> 24u);
#else
    meteData[3u] = (uint8)(canId);
    meteData[2u] = (uint8)(canId >> 8u);
    meteData[1u] = (uint8)(canId >> 16u);
    meteData[0u] = (uint8)(canId >> 24u);
#endif
#endif
    /* PRQA S 3120 -- */
}
#endif

/**
 * @brief              Set status bit of remaining TxConfirmation call
 * @param[in]          txPduConfigPtr: Pointer to TxPdu configuration
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
CANIF_LOCAL_INLINE void CanIf_SetUpConfirmTxPduStatus(const CanIf_TxPduConfigType* txPduConfigPtr)
{
    CanIf_UpConfirmTxPduType* upConfirmTxPduPtr  = txPduConfigPtr->UpConfirmTxPduPtr;
    uint8                     upConfirmTxPduMask = txPduConfigPtr->UpConfirmTxPduMask;

    /* BMC: fetch_and_or */
    upConfirmTxPduPtr->UpConfirmStatus |= upConfirmTxPduMask;
}

/**
 * @brief              Reset status bit of remaining TxConfirmation call
 * @param[in]          txPduConfigPtr: Pointer to TxPdu configuration
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
CANIF_LOCAL_INLINE boolean CanIf_ClearUpConfirmTxPduStatus(const CanIf_TxPduConfigType* txPduConfigPtr)
{
    boolean                   result             = FALSE;
    CanIf_UpConfirmTxPduType* upConfirmTxPduPtr  = txPduConfigPtr->UpConfirmTxPduPtr;
    uint8                     upConfirmTxPduMask = txPduConfigPtr->UpConfirmTxPduMask;

    /* BMC: fetch_and_and */
    if ((upConfirmTxPduPtr->UpConfirmStatus & upConfirmTxPduMask) > 0u)
    {
        upConfirmTxPduPtr->UpConfirmStatus &= (uint8)(~upConfirmTxPduMask);
        result = TRUE;
    }
    return result;
}

CANIF_LOCAL_INLINE PduIdType CanIf_TxPduId2VarIndex(PduIdType txPduId)
{
    PduIdType txPduIndex = txPduId;

#if (CANIF_VARIANT_NUMBER > 1u)
    if (NULL_PTR != CANIF_TXPDU_VARINDEX_REF)
    {
        txPduIndex = CANIF_TXPDU_VARINDEX(txPduId);
    }
#endif

    return txPduIndex;
}

CANIF_LOCAL_INLINE PduIdType CanIf_RxPduId2VarIndex(PduIdType rxPduId)
{
    PduIdType rxPduIndex = rxPduId;

#if (CANIF_VARIANT_NUMBER > 1u)
    if (NULL_PTR != CANIF_RXPDU_VARINDEX_REF)
    {
        rxPduIndex = CANIF_RXPDU_VARINDEX(rxPduId);
    }
#endif

    return rxPduIndex;
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif
