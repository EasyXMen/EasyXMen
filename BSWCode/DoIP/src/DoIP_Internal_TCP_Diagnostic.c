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
 **  @file               : DoIP_Internal_TCP_Diagnostic.c
 **  @author             : hao.wen
 **  @date               : 2024/11/01
 **  @vendor             : isoft
 **  @description        : Implement the diagnostic part of TCP
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

#include "DoIP_Internal_TCP.h"
#include "DoIP_Internal_TCP_Common.h"
#include "DoIP_Internal_TCP_Non_Diagnostic.h"
#include "PduR_DoIP.h"
#include "SchM_DoIP.h"
/* ===================================================== macros ===================================================== */

#define DOIP_DIAG_MSG_FIELD_POSITION_SA  8u  /**< Location of SA field in diagnostic message */
#define DOIP_DIAG_MSG_FIELD_POSITION_TA  10u /**< Location of TA field in diagnostic message */
#define DOIP_DIAG_MSG_FIELD_POSITION_UDS 12u /**< Location of UDS field in diagnostic message */

#define DOIP_DIAG_ACK_NACK_MSG_LEN                    5u  /**< Length of ACK/NACK message. */
#define DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_SA      8u  /**< Location of SA field in ACK/NACK message */
#define DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_TA      10u /**< Location of TA field in ACK/NACK message */
#define DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_CODE    12u /**< Location of code field in ACK/NACK message */
#define DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_PRE_UDS 13u /**< Location of pre UDS field in ACK/NACK message */

#define DOIP_DIAG_NACK_INVALID_SA               0x02u /**< Invalid source address */
#define DOIP_DIAG_NACK_UNKNOWN_TA               0x03u /**< Unknown target address */
#define DOIP_DIAG_NACK_MSG_TOO_LARGE            0x04u /**< Diagnostic message too larg */
#define DOIP_DIAG_NACK_OUT_OF_MEMORY            0x05u /**< Out of memory */
#define DOIP_DIAG_NACK_TARGET_UNREACHABLE       0x06u /**< Target unreachable */
#define DOIP_DIAG_NACK_UNKNOWN_NETWORK          0x07u /**< Unknown network */
#define DOIP_DIAG_NACK_TRANSPORT_PROTOCOL_ERROR 0x08u /**< Transport protocol error */

/* ========================================== internal function definitions ========================================= */

/* cfg */

/**
 * @brief       Get SA and TA from configuration of channel.
 * @param[in]   channelIdx: Index of channel.
 * @param[out]  sa: Source address.
 * @param[out]  ta: Tareget address.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE void DoIP_GetSaTaFromChannel(uint16 channelIdx, uint16* sa, uint16* ta);

/**
 * @brief       Get PduRTxPduRef from configuration of channel.
 * @param[in]   channelIdx: Index of channel.
 * @return      PduIdType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE PduIdType DoIP_GetPduRTxPduRef(uint16 channelIdx);

/* pending */

/**
 * @brief       Get message from queue.
 * @param[in]   queue: Specify queue.
 * @return      DoIP_TpItemType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE DoIP_TpItemType* DoIP_DequeueTpMsg(DoIP_TpQueueType* queue); /* PRQA S 5016 */ /* VL_DoIP_5016 */

/**
 * @brief       Get message from queue.
 * @param[in]   queue: Specify queue.
 * @return      DoIP_TpItemType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE DoIP_IfItemType* DoIP_DequeueIfMsg(DoIP_IfQueueType* queue); /* PRQA S 5016 */ /* VL_DoIP_5016 */
/* PRQA S 5016 ++ */                                                                             /* VL_DoIP_5016 */
DOIP_LOCAL_INLINE Std_ReturnType
    DoIP_UnreadqueueTpMsg(DoIP_TpQueueType* queue, PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr);

DOIP_LOCAL_INLINE Std_ReturnType
    DoIP_UnreadqueueIfMsg(DoIP_IfQueueType* queue, PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr);
/* PRQA S 5016 -- */
/* rcv */

/**
 * @brief       Check whether TA is valid.
 * @param[in]   ta: Target address.
 * @param[in]   tcpConnStatusIdx: Index of DoIP_TcpConnStatus.
 * @return      boolean
 * @retval      TRUE: Valid.
 * @retval      FALSE: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_IsTaInRaCfg(uint16 ta, uint8 tcpConnStatusIdx);

/**
 * @brief       Check diagnostic message is valid.
 * @param[in]   sa: Source address.
 * @param[in]   ta: Target address.
 * @param[in]   soConId: Used for get DoIP_TcpConnStatus.
 * @param[out]  nack: NACK code.
 * @return      boolean
 * @retval      TRUE: Valid.
 * @retval      FALSE: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_IsValidDiagMsg(uint16 sa, uint16 ta, SoAd_SoConIdType soConId, uint8* nack);

/**
 * @brief       Transfer diagnostic message to PduR.
 * @param[in]   pdurRxPduRef: PduRRxPdu
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @param[out]  bufferSizePtr: Available receive buffer after data has been copied.
 * @param[in]   handleDiagCtx: Context of handle diagnostic message.
 * @param[out]  nack: NACK code.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Success.
 * @retval      BUFREQ_E_NOT_OK: Fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL BufReq_ReturnType DoIP_TransferDiagMsgToPduR(
    PduIdType                         pdurRxPduRef,
    const PduInfoType*                pduInfoPtr,
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx,
    uint8*                            nack);

/**
 * @brief       Start transfer diagnostic message to PduR.
 * @param[out]  bufferSizePtr: Available receive buffer after data has been copied.
 * @param[in]   handleDiagCtx: Context of handle diagnostic message.
 * @param[out]  nack: NACK code.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Success.
 * @retval      BUFREQ_E_NOT_OK: Fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL BufReq_ReturnType DoIP_StartTransDataToPduR(
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx,
    uint8*                            nack);

/**
 * @brief       Handle diagnostic message header.
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @param[out]  bufferSizePtr: Available receive buffer after data has been copied.
 * @param[in]   handleDiagCtx: Context of handle diagnostic message.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Success.
 * @retval      BUFREQ_E_NOT_OK: Fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL BufReq_ReturnType DoIP_HandleReceivedDiagMsgHeader(
    const PduInfoType*                pduInfoPtr,
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx);

/**
 * @brief       Handle diagnostic message.
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @param[out]  bufferSizePtr: Available receive buffer after data has been copied.
 * @param[in]   handleDiagCtx: Context of handle diagnostic message.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Success.
 * @retval      BUFREQ_E_NOT_OK: Fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL BufReq_ReturnType DoIP_HandleDiagMsgCfReceiving(
    const PduInfoType*                pduInfoPtr,
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx);

/* send */

/**
 * @brief       Fill diagnostic message header.
 * @param[in]   pduInfoPtr: Provides number of bytes to be copied (SduLength).
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @param[in]   sa: Source address.
 * @param[in]   ta: Target address.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE void DoIP_FillDiagnosticHeader(const PduInfoType* pduInfoPtr, uint8 txRxCtxIdx, uint16 sa, uint16 ta);

/**
 * @brief        Send diagnostic message header.
 * @param[inout] ctx: Send receive context.
 * @param[in]    pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @param[out]   availableDataPtr: Indicates the remaining number of bytes that are available in the upper layer
 * module's Tx buffer.
 * @param[in]    retry: This parameter is used to acknowledge transmitted data or toretransmit data after transmission
 * problems.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Data has been copied to the transmit buffer completely as requested.
 * @retval      BUFREQ_E_NOT_OK: Data has not been copied. Request failed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL BufReq_ReturnType DoIP_SoAdTpCopyTxTpHeader(
    DoIP_TcpTxRxContextType* ctx,
    const PduInfoType*       pduInfoPtr,
    PduLengthType*           availableDataPtr,
    const RetryInfoType*     retry);

/**
 * @brief       Send diagnostic ACK.
 * @param[in]   soadTxPduRef: Used for send ACK message.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_SendDiagnosticAck(uint16 soadTxPduRef, uint8 txRxCtxIdx);

/**
 * @brief       Send diagnostic NACK.
 * @param[in]   soadTxPduRef: Used for send NACK message.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @param[in]   nack: NACK code.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_SendDiagnosticNack(uint16 soadTxPduRef, uint8 txRxCtxIdx, uint8 nack);

/* ========================================== external function definitions ========================================= */

#define DOIP_START_SEC_CODE
#include "DoIP_MemMap.h"

/* ----------------------------------------- connect ----------------------------------------- */

/* PRQA S 1512 EOF */ /* VL_DoIP_1512 */
/**
 * @brief Notify PdurR when close tcp connection.
 */
/* PRQA S 3408,1503 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_QAC_NoUsedApi */
/* PRQA S 2905 ++ */      /* VL_DoIP_2905 */
void DoIP_NotifyPduRWhenCloseTcpConnection(uint8 txRxCtxIdx)
/* PRQA S 3408,1503 -- */
{
    /* PRQA S 2844,2934 ++ */ /* VL_DoIP_DerefInvalidPtr,VL_DoIP_ComputeInvalidPtr */
    const DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl; /* PRQA S 3408 */ /* VL_DoIP_3408 */

    if ((txCtrl->PdurTxPduRef != (PduIdType)DOIP_INVALID_UINT16) && (txCtrl->TxState != DOIP_TX_STATE_IDLE))
    {
        if (txCtrl->TxMsgType == DOIP_TX_DIAG_TP_MSG)
        {
            PduR_DoIPTpTxConfirmation(txCtrl->PdurTxPduRef, E_NOT_OK);
        }
        else if (txCtrl->TxMsgType == DOIP_TX_DIAG_IF_MSG)
        {
            PduR_DoIPIfTxConfirmation(txCtrl->PdurTxPduRef, E_NOT_OK);
        }
        else
        {
            /* for qac */
        }
    }

    DOIP_ClearIfQueue(&DoIP_TcpTxRxContext[txRxCtxIdx], TRUE);
    DOIP_ClearTpQueue(&DoIP_TcpTxRxContext[txRxCtxIdx], TRUE);
    /* PRQA S 2844,2934 -- */
}
/* PRQA S 2905 -- */
/* ----------------------------------------- send ----------------------------------------- */

/**
 * @brief Check whether the metadata is valid.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
boolean DoIP_IsValidMatadata(uint16 channelIdx, const PduInfoType* pduInfoPtr)
/* PRQA S 1532 -- */
{
    boolean ret = TRUE;
    /* PRQA S 2824 ++ */ /* VL_DoIP_ArithNullptr */
    const DoIP_PBChannelCfgType* pbChannel = &DoIP_PBCfgPtr->ChannelCfgArray[channelIdx];
    /* PRQA S 2824 -- */

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    const DoIP_PBPduRTxPduType* pdurTxPdu = pbChannel->PduRTxPdu;

    if ((pdurTxPdu->HasMetaData == TRUE) && (pduInfoPtr->MetaDataPtr != NULL_PTR))
    /* PRQA S 2814 -- */
    {
        uint16 channelSa = pbChannel->ChannelSaRef->TesterSA;
        uint16 channelTa = pbChannel->ChannelTa;

        /* PRQA S 3120,2814 ++ */ /* VL_QAC_MagicNum,VL_QAC_DerefNullPtr */
        /* PRQA S 2824,4397 ++ */ /* VL_DoIP_ArithNullptr,VL_DoIP_NotCastEssentialType */
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
        uint16 metaDataSa =
            (uint16)(((uint16)pduInfoPtr->MetaDataPtr[1] << (uint16)8u) + (uint16)pduInfoPtr->MetaDataPtr[0]);
        uint16 metaDataTa = ((uint16)pduInfoPtr->MetaDataPtr[3] << (uint16)8u) + pduInfoPtr->MetaDataPtr[2];
#else
        uint16 metaDataSa =
            (uint16)(((uint16)pduInfoPtr->MetaDataPtr[2] << (uint16)8u) + (uint16)pduInfoPtr->MetaDataPtr[3]);
        uint16 metaDataTa = ((uint16)pduInfoPtr->MetaDataPtr[0] << (uint16)8u) + pduInfoPtr->MetaDataPtr[1];
#endif
        /* PRQA S 2824,4397  -- */
        /* PRQA S 3120,2814  -- */
        if ((metaDataSa != channelSa) || (metaDataTa != channelTa))
        {
            ret = FALSE;
        }
    }

    return ret;
}

/**
 * @brief Trigger send TP message.
 */
Std_ReturnType DoIP_TriggerTpTransmit(
    PduIdType           soadTxPduRef,
    const PduInfoType*  pduInfoPtr,
    DoIP_TcpTxCtrlType* txCtrl,
    DoIP_TcpTxMsgType   msgType)
{
    Std_ReturnType ret;
    SchM_Enter_DoIP_ExclusiveArea();
    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    txCtrl->TxBufState      = DOIP_BUFFER_OCCUPIED;
    txCtrl->TxMsgType       = msgType;
    txCtrl->TxState         = DOIP_TX_STATE_START;
    txCtrl->TxMaxDataLength = pduInfoPtr->SduLength;
    /* PRQA S 2814 -- */
    SchM_Exit_DoIP_ExclusiveArea();
    if (E_OK == SoAd_TpTransmit(soadTxPduRef, pduInfoPtr))
    {
        ret = E_OK;
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}

/**
 * @brief Trigger send TP message.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_DoIP_STCAL */
/* PRQA S 4461 ++ */ /* VL_DoIP_4461 */
Std_ReturnType DoIP_TpTransmitInternal(PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr)
{
    uint8  txRxCtxIdx;
    uint16 channelIdx = 0u;

    Std_ReturnType ret = E_NOT_OK;

    if ((E_OK == DoIP_GetChannelIdxByPduRTxPduId(pdurTxPduId, DOIP_TPPDU, &channelIdx))
        && (E_OK == DoIP_GetTcpTxRxCtxIdxByChannelIdx(channelIdx, &txRxCtxIdx))) /* PRQA S 3415 */ /* VL_DoIP_3415 */
    {
        uint16 sa;           /* server's la */
        uint16 ta;           /* tester's la */
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
        /* PRQA S 2844 -- */

        DoIP_GetSaTaFromChannel(channelIdx, &sa, &ta);

        /*SWS_DoIP_00230*/
        uint8 index;
        ret = DoIP_GetTcpConnStatusIdxBySa(ta, &index);
        if (E_OK == ret)
        {
            /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
            PduInfoType pduInfo;
            pduInfo.SduDataPtr  = NULL_PTR;
            pduInfo.MetaDataPtr = NULL_PTR;
            pduInfo.SduLength   = DOIP_HEADER_LEN + DOIP_SA_TA_LEN + pduInfoPtr->SduLength;
            /* PRQA S 2814 -- */
            SchM_Enter_DoIP_ExclusiveArea();
            DoIP_FillDiagnosticHeader(pduInfoPtr, txRxCtxIdx, sa, ta);
            /* PRQA S 2844 ++ */ /* VL_QAC_DerefNullPtr */
            txCtrl->PdurTxPduId  = pdurTxPduId;
            txCtrl->PdurTxPduRef = DoIP_GetPduRTxPduRef(channelIdx);
            ret = DoIP_TriggerTpTransmit(DoIP_TcpConnStatus[index].SoadTxPduRef, &pduInfo, txCtrl, DOIP_TX_DIAG_TP_MSG);
            /* PRQA S 2844 -- */

            /*SWS_DoIP_00220*/
            if (ret != E_OK)
            {
                DoIP_ResetTcpTxContext(txRxCtxIdx);
            }
            SchM_Exit_DoIP_ExclusiveArea();
        }
    }

    return ret;
}

/**
 * @brief Trigger send IF message.
 */
Std_ReturnType DoIP_IfTransmitInternal(PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr)
{
    uint8          txRxCtxIdx;
    uint16         channelIdx = 0u;
    Std_ReturnType ret        = E_NOT_OK;

    if ((E_OK == DoIP_GetChannelIdxByPduRTxPduId(pdurTxPduId, DOIP_IFPDU, &channelIdx))
        && (E_OK == DoIP_GetTcpTxRxCtxIdxByChannelIdx(channelIdx, &txRxCtxIdx))) /* PRQA S 3415 */ /* VL_DoIP_3415 */
    {
        PduInfoType pduInfo;
        /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
        pduInfo.SduLength = (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN + pduInfoPtr->SduLength;
        /* PRQA S 2814 -- */

        if ((PduLengthType)DOIP_TX_BUFFER_SIZE
            >= ((PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN + pduInfoPtr->SduLength))
        {
            ret = E_OK;
        }

        uint8  index;
        uint16 sa;
        uint16 ta;
        DoIP_GetSaTaFromChannel(channelIdx, &sa, &ta);

        if (ret == E_OK)
        {
            ret = DoIP_GetTcpConnStatusIdxBySa(ta, &index); /* TA is actually the LA for the client.  */
        }

        if (ret == E_OK)
        {
            /* PRQA S 2934 ++ */ /* VL_DoIP_ComputeInvalidPtr */
            DoIP_TcpTxRxContextType* ctx = &DoIP_TcpTxRxContext[txRxCtxIdx];
            /* PRQA S 2934 -- */
            SchM_Enter_DoIP_ExclusiveArea();
            DoIP_FillDiagnosticHeader(pduInfoPtr, txRxCtxIdx, sa, ta);

            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            DoIP_MemCpy(&ctx->TxBuf[DOIP_HEADER_LEN + DOIP_SA_TA_LEN], pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
            /* PRQA S 2844 -- */
            pduInfo.SduDataPtr       = &ctx->TxBuf[0];
            ctx->TxCtrl.PdurTxPduId  = pdurTxPduId;
            ctx->TxCtrl.PdurTxPduRef = DoIP_GetPduRTxPduRef(channelIdx);
            ret                      = DoIP_TriggerTpTransmit(
                /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
                DoIP_TcpConnStatus[index]
                    .SoadTxPduRef,
                /* PRQA S 2844 -- */
                &pduInfo,
                &ctx->TxCtrl,
                DOIP_TX_DIAG_IF_MSG);
            if (ret != E_OK)
            {
                ctx->TxCtrl.TxBufState = DOIP_BUFFER_IDLE;
                ctx->TxCtrl.TxState    = DOIP_TX_STATE_IDLE;
            }
            SchM_Exit_DoIP_ExclusiveArea();
        }
    }

    return ret;
}
/* PRQA S 6070 -- */

/**
 * @brief Copy non TP data form send buffer.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
BufReq_ReturnType DoIP_SoAdTpCopyTxNoTpData(
    DoIP_TcpTxRxContextType* ctx,
    const PduInfoType*       pduInfoPtr,
    PduLengthType*           availableDataPtr)
/* PRQA S 1532 -- */
{
    BufReq_ReturnType ret = BUFREQ_OK;

    /* PRQA S 2814,2844,2934 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr,VL_DoIP_ComputeInvalidPtr */
    DoIP_TcpTxCtrlType* txCtrl = &ctx->TxCtrl;
    const uint8*        txBuf  = ctx->TxBuf;

    if (txCtrl->TxState == DOIP_TX_STATE_START)
    {
        /*SWS_DoIP_00224*/
        if (pduInfoPtr->SduLength != 0u)
        {
            /*SWS_DoIP_00225*/
            DoIP_MemCpy(pduInfoPtr->SduDataPtr, &txBuf[txCtrl->TxBufPos], pduInfoPtr->SduLength);
            txCtrl->TxBufPos += pduInfoPtr->SduLength;
        }

        *availableDataPtr = txCtrl->TxMaxDataLength - txCtrl->TxBufPos;
        /* PRQA S 2814,2844,2934 -- */
    }
    else
    {
        ret = BUFREQ_E_NOT_OK;
    }

    return ret;
}

/**
 * @brief Copy TP data form send buffer.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
BufReq_ReturnType DoIP_SoAdTpCopyTxTpData(
    DoIP_TcpTxRxContextType* ctx,
    const PduInfoType*       pduInfoPtr,
    const RetryInfoType*     retry,
    PduLengthType*           availableDataPtr)
/* PRQA S 1532 -- */
{
    BufReq_ReturnType ret = BUFREQ_OK;

    DoIP_TcpTxCtrlType* txCtrl = &ctx->TxCtrl; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */

    /*first to transimit the diagnostic header*/
    if (txCtrl->TxState == DOIP_TX_STATE_START)
    {
        ret = DoIP_SoAdTpCopyTxTpHeader(ctx, pduInfoPtr, availableDataPtr, retry);
    }
    else if (txCtrl->TxState == DOIP_TX_STATE_TRANSMITED_HEADER) /*diagnostic header has been transmited*/
    {
        /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
        if (pduInfoPtr->SduLength == 0u)
        {
            *availableDataPtr = txCtrl->TxMaxDataLength - txCtrl->TxBufPos;
            /* PRQA S 2814 -- */
        }
        else
        {
            if (BUFREQ_OK == PduR_DoIPTpCopyTxData(txCtrl->PdurTxPduRef, pduInfoPtr, retry, availableDataPtr))
            {
                txCtrl->TxBufPos += pduInfoPtr->SduLength;
            }
            else
            {
                ret = BUFREQ_E_NOT_OK;
            }
        }
    }
    else
    {
        ret = BUFREQ_E_NOT_OK;
    }

    return ret;
}

/* ----------------------------------------- pending ----------------------------------------- */

/**
 * @brief Handle Tp message in queued.
 */
/* PRQA S 3408,1532 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_DoIP_RefeOneFile */
boolean DoIP_HandlePendingTpMsg(DoIP_TcpTxRxContextType* ctx)
/* PRQA S 3408,1532 -- */
{
    boolean ret = TRUE;
    SchM_Enter_DoIP_ExclusiveArea();
    const DoIP_TpItemType* item = DoIP_DequeueTpMsg(&ctx->TxTpQueue); /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    SchM_Exit_DoIP_ExclusiveArea();
    if (NULL_PTR != item)
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        if (E_NOT_OK == DoIP_TpTransmitInternal(item->PdurTxPduId, &item->PduInfo))
        /* PRQA S 2844 -- */
        {
            if (E_NOT_OK == DoIP_UnreadqueueTpMsg(&ctx->TxTpQueue, item->PdurTxPduId, &item->PduInfo))
            {
                PduR_DoIPTpTxConfirmation(ctx->TxCtrl.PdurTxPduRef, E_NOT_OK);
            }
            ctx->TxCtrl.TxBufState = DOIP_BUFFER_IDLE;
            ctx->TxCtrl.TxMsgType  = DOIP_TX_NON_MSG;
            ctx->TxCtrl.TxState    = DOIP_TX_STATE_IDLE;
        }

        ret = FALSE;
    }

    return ret;
}

/**
 * @brief Handle IF message in queued.
 */
/* PRQA S 3408,1532 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_DoIP_RefeOneFile */
boolean DoIP_HandlePendingIfMsg(DoIP_TcpTxRxContextType* ctx)
/* PRQA S 3408,1532 -- */
{
    boolean ret = TRUE;
    SchM_Enter_DoIP_ExclusiveArea();
    const DoIP_IfItemType* item = DoIP_DequeueIfMsg(&ctx->TxIfQueue); /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    SchM_Exit_DoIP_ExclusiveArea();
    if (NULL_PTR != item)
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        if (E_NOT_OK == DoIP_IfTransmitInternal(item->PdurTxPduId, &item->PduInfo))
        /* PRQA S 2844 -- */
        {
            if (E_NOT_OK == DoIP_UnreadqueueIfMsg(&ctx->TxIfQueue, item->PdurTxPduId, &item->PduInfo))
            {
                PduR_DoIPIfTxConfirmation(ctx->TxCtrl.PdurTxPduRef, E_NOT_OK);
            }
            ctx->TxCtrl.TxBufState = DOIP_BUFFER_IDLE;
            ctx->TxCtrl.TxMsgType  = DOIP_TX_NON_MSG;
            ctx->TxCtrl.TxState    = DOIP_TX_STATE_IDLE;
        }

        ret = FALSE;
    }

    return ret;
}

/**
 * @brief Enqueue TP message.
 */
Std_ReturnType DoIP_EnqueueTpMsg(DoIP_TpQueueType* queue, PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType ret = E_OK;
    uint8          pendingQueueNum;

    SchM_Enter_DoIP_ExclusiveArea();
    /* PRQA S 2814,2934,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ComputeInvalidPtr,VL_DoIP_DerefInvalidPtr */
    if ((queue->Tail > queue->Head) || (queue->Count == 0u))
    {
        pendingQueueNum = queue->Tail - queue->Head;
    }
    else
    {
        pendingQueueNum = (DOIP_TP_QUEUE_BUFFER_NUM - queue->Head) + queue->Tail;
    }

    if (pendingQueueNum < DOIP_TP_QUEUE_BUFFER_NUM)
    {
        DoIP_TpItemType* item   = &queue->Item[queue->Tail];
        item->PdurTxPduId       = pdurTxPduId;
        item->PduInfo.SduLength = pduInfoPtr->SduLength;
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            item->PduInfo.MetaDataPtr = item->PduData.SduDataPtr;
            DoIP_MemCpy(item->PduInfo.MetaDataPtr, pduInfoPtr->MetaDataPtr, DOIP_SA_TA_LEN);
        }
        else
        {
            item->PduInfo.MetaDataPtr = NULL_PTR;
        }

        queue->Tail++;
        if (queue->Tail >= DOIP_TP_QUEUE_BUFFER_NUM)
        {
            queue->Tail = 0u;
        }
        if (queue->Count < DOIP_TP_QUEUE_BUFFER_NUM)
        {
            queue->Count++;
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    SchM_Exit_DoIP_ExclusiveArea();
    /* PRQA S 2814,2934,2844 -- */
    return ret;
}

DOIP_LOCAL_INLINE Std_ReturnType
    DoIP_UnreadqueueTpMsg(DoIP_TpQueueType* queue, PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType ret = E_OK;
    uint8          pendingQueueNum;

    SchM_Enter_DoIP_ExclusiveArea();
    if ((queue->Tail > queue->Head) || (queue->Count == 0u))
    {
        pendingQueueNum = queue->Tail - queue->Head;
    }
    else
    {
        pendingQueueNum = (DOIP_TP_QUEUE_BUFFER_NUM - queue->Head) + queue->Tail;
    }
    if (pendingQueueNum < DOIP_TP_QUEUE_BUFFER_NUM) /*buffer full*/
    {
        if (queue->Head == 0u)
        {
            queue->Head = DOIP_TP_QUEUE_BUFFER_NUM - 1u;
        }
        else
        {
            queue->Head -= 1u;
        }
        DoIP_TpItemType* item   = &queue->Item[queue->Head];
        item->PdurTxPduId       = pdurTxPduId;
        item->PduInfo.SduLength = pduInfoPtr->SduLength;
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            item->PduInfo.MetaDataPtr = item->PduData.SduDataPtr;
            DoIP_MemCpy(item->PduInfo.MetaDataPtr, pduInfoPtr->MetaDataPtr, DOIP_SA_TA_LEN);
        }
        else
        {
            item->PduInfo.MetaDataPtr = NULL_PTR;
        }

        if (queue->Count < DOIP_TP_QUEUE_BUFFER_NUM)
        {
            queue->Count++;
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    SchM_Exit_DoIP_ExclusiveArea();

    return ret;
}
/**
 * @brief Enqueue IF message.
 */
Std_ReturnType DoIP_EnqueueIfMsg(DoIP_IfQueueType* queue, PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType ret = E_OK;
    uint8          pendingQueueNum;

    SchM_Enter_DoIP_ExclusiveArea();
    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if ((queue->Tail > queue->Head) || (queue->Count == 0u))
    {
        pendingQueueNum = queue->Tail - queue->Head;
    }
    else
    {
        pendingQueueNum = (DOIP_IF_QUEUE_BUFFER_NUM - queue->Head) + queue->Tail;
    }

    if ((pendingQueueNum >= DOIP_IF_QUEUE_BUFFER_NUM)
        || ((PduLengthType)DOIP_TX_BUFFER_SIZE
            < ((PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN + pduInfoPtr->SduLength))) /*buffer full*/
    /* PRQA S 2814 -- */
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* PRQA S 2934 ++ */ /* VL_DoIP_ComputeInvalidPtr */
        DoIP_IfItemType* item = &queue->Item[queue->Tail];
        /* PRQA S 2934 -- */
        item->PdurTxPduId       = pdurTxPduId; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
        item->PduInfo.SduLength = pduInfoPtr->SduLength;

        item->PduInfo.SduDataPtr = item->PduData.SduDataPtr;
        DoIP_MemCpy(item->PduData.SduDataPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            item->PduInfo.MetaDataPtr = item->PduData.MetaDataPtr;
            DoIP_MemCpy(item->PduInfo.MetaDataPtr, pduInfoPtr->MetaDataPtr, DOIP_SA_TA_LEN);
        }
        else
        {
            item->PduInfo.MetaDataPtr = NULL_PTR;
        }
        queue->Tail++;

        if (queue->Tail >= DOIP_IF_QUEUE_BUFFER_NUM)
        {
            queue->Tail = 0u;
        }
        if (queue->Count < DOIP_IF_QUEUE_BUFFER_NUM)
        {
            queue->Count++;
        }
    }
    SchM_Exit_DoIP_ExclusiveArea();

    return ret;
}

DOIP_LOCAL_INLINE Std_ReturnType
    DoIP_UnreadqueueIfMsg(DoIP_IfQueueType* queue, PduIdType pdurTxPduId, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType ret = E_OK;
    uint8          pendingQueueNum;

    SchM_Enter_DoIP_ExclusiveArea();
    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if ((queue->Tail > queue->Head) || (queue->Count == 0u))
    {
        pendingQueueNum = queue->Tail - queue->Head;
    }
    else
    {
        pendingQueueNum = (DOIP_IF_QUEUE_BUFFER_NUM - queue->Head) + queue->Tail;
    }

    if ((pendingQueueNum >= DOIP_IF_QUEUE_BUFFER_NUM)
        || ((PduLengthType)DOIP_TX_BUFFER_SIZE
            < ((PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN + pduInfoPtr->SduLength))) /*buffer full*/
    /* PRQA S 2814 -- */
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* PRQA S 2934 ++ */ /* VL_DoIP_ComputeInvalidPtr */
        if (queue->Head == 0u)
        {
            queue->Head = DOIP_IF_QUEUE_BUFFER_NUM - 1u;
        }
        else
        {
            queue->Head -= 1u;
        }
        DoIP_IfItemType* item = &queue->Item[queue->Head];
        /* PRQA S 2934 -- */
        item->PdurTxPduId       = pdurTxPduId; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
        item->PduInfo.SduLength = pduInfoPtr->SduLength;

        item->PduInfo.SduDataPtr = item->PduData.SduDataPtr;
        DoIP_MemCpy(item->PduData.SduDataPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            item->PduInfo.MetaDataPtr = item->PduData.MetaDataPtr;
            DoIP_MemCpy(item->PduInfo.MetaDataPtr, pduInfoPtr->MetaDataPtr, DOIP_SA_TA_LEN);
        }
        else
        {
            item->PduInfo.MetaDataPtr = NULL_PTR;
        }

        if (queue->Count < DOIP_IF_QUEUE_BUFFER_NUM)
        {
            queue->Count++;
        }
    }
    SchM_Exit_DoIP_ExclusiveArea();

    return ret;
}
/**
 * @brief Clear IF queue.
 */
void DOIP_ClearIfQueue(DoIP_TcpTxRxContextType* ctx, boolean isNotifyPdur)
{
    const DoIP_IfItemType* item;

    do
    {
        item = DoIP_DequeueIfMsg(&ctx->TxIfQueue); /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */

        if ((NULL_PTR != item) && (isNotifyPdur == TRUE))
        {
            uint16 channelIdx = 0u;

            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            if (E_OK == DoIP_GetChannelIdxByPduRTxPduId(item->PdurTxPduId, DOIP_IFPDU, &channelIdx))
            /* PRQA S 2844 -- */
            {
                PduR_DoIPIfTxConfirmation(DoIP_GetPduRTxPduRef(channelIdx), E_NOT_OK);
            }
        }

    } while (NULL_PTR != item);
}

/**
 * @brief Clear TP queue.
 */
void DOIP_ClearTpQueue(DoIP_TcpTxRxContextType* ctx, boolean isNotifyPdur)
{
    const DoIP_TpItemType* item;

    do
    {
        item = DoIP_DequeueTpMsg(&ctx->TxTpQueue); /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */

        if ((NULL_PTR != item) && (isNotifyPdur == TRUE))
        {
            uint16 channelIdx = 0u;

            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            if (E_OK == DoIP_GetChannelIdxByPduRTxPduId(item->PdurTxPduId, DOIP_TPPDU, &channelIdx))
            /* PRQA S 2844 -- */
            {
                PduR_DoIPTpTxConfirmation(DoIP_GetPduRTxPduRef(channelIdx), E_NOT_OK);
            }
        }

    } while (NULL_PTR != item);
}

/* ----------------------------------------- rcv ----------------------------------------- */

/**
 * @brief Handle diagnostic message.
 */
/* PRQA S 3408,1532 ++*/ /* VL_DoIP_WithoutPreDeclaration,VL_DoIP_RefeOneFile */
BufReq_ReturnType DoIP_HandleDiagMsg(
    const PduInfoType*                pduInfoPtr,
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx)
/* PRQA S 3408,1532 -- */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;

    /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
    const DoIP_TcpRxCtrlType* rxCtrl = &DoIP_TcpTxRxContext[handleDiagCtx->TxRxCtxIdx].RxCtrl;

    if (rxCtrl->RxState == DOIP_RX_STATE_RECEIVED_HEADER)
    {
        ret = DoIP_HandleReceivedDiagMsgHeader(pduInfoPtr, bufferSizePtr, handleDiagCtx);
    }
    else if (rxCtrl->RxState == DOIP_RX_STATE_CF)
    /* PRQA S 2814,2844 --*/
    {
        ret = DoIP_HandleDiagMsgCfReceiving(pduInfoPtr, bufferSizePtr, handleDiagCtx);
    }
    else
    {
        /*do nothing*/
    }

    return ret;
}

/* ========================================== internal function definitions ========================================= */

/* ----------------------------------------- cfg ----------------------------------------- */

/**
 * @brief Get SA and TA from configuration of channel.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE void DoIP_GetSaTaFromChannel(uint16 channelIdx, uint16* sa, uint16* ta)
/* PRQA S 1505 -- */
{
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    *sa = DoIP_PBCfgPtr->ChannelCfgArray[channelIdx].ChannelTa;
    *ta = DoIP_PBCfgPtr->ChannelCfgArray[channelIdx].ChannelSaRef->TesterSA;
    /* PRQA S 2814,2824 -- */
}

/**
 * @brief Get PduRTxPduRef from configuration of channel.
 */
DOIP_LOCAL_INLINE PduIdType DoIP_GetPduRTxPduRef(uint16 channelIdx) /* PRQA S 1505 */ /* VL_DoIP_RefeOneFile */
{
    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    return DoIP_PBCfgPtr->ChannelCfgArray[channelIdx].PduRTxPdu->PdurTxPduRef;
    /* PRQA S 2814,2824 -- */
}

/* ----------------------------------------- pending ----------------------------------------- */

/**
 * @brief Get message from queue.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE DoIP_TpItemType* DoIP_DequeueTpMsg(DoIP_TpQueueType* queue)
/* PRQA S 1505 -- */
{
    SchM_Enter_DoIP_ExclusiveArea();
    DoIP_TpItemType* item = NULL_PTR;

    /* PRQA S 2814,2934 ++ */                                /* VL_QAC_DerefNullPtr,VL_DoIP_ComputeInvalidPtr */
    if ((queue->Tail != queue->Head) || (queue->Count > 0u)) /*queueMsg not empty*/
    {
        item = &queue->Item[queue->Head];
        queue->Head++;
        /* PRQA S 2814,2934 -- */
    }
    if (queue->Head >= DOIP_TP_QUEUE_BUFFER_NUM)
    {
        queue->Head = 0u;
    }
    if (queue->Count > 0u)
    {
        queue->Count--;
    }
    SchM_Exit_DoIP_ExclusiveArea();
    return item;
}

/**
 * @brief Get message from queue.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE DoIP_IfItemType* DoIP_DequeueIfMsg(DoIP_IfQueueType* queue)
/* PRQA S 1505 -- */
{
    SchM_Enter_DoIP_ExclusiveArea();
    DoIP_IfItemType* item = NULL_PTR;

    /* PRQA S 2814,2934 ++ */                                /* VL_QAC_DerefNullPtr,VL_DoIP_ComputeInvalidPtr */
    if ((queue->Tail != queue->Head) || (queue->Count > 0u)) /*queueMsg not empty*/
    {
        item = &queue->Item[queue->Head];
        queue->Head++;
        /* PRQA S 2814,2934 -- */
    }
    if (queue->Head >= DOIP_IF_QUEUE_BUFFER_NUM)
    {
        queue->Head = 0u;
    }
    if (queue->Count > 0u)
    {
        queue->Count--;
    }
    SchM_Exit_DoIP_ExclusiveArea();
    return item;
}

/* ----------------------------------------- rcv ----------------------------------------- */

/**
 * @brief Check whether TA is valid.
 */
DOIP_LOCAL boolean DoIP_IsTaInRaCfg(uint16 ta, uint8 tcpConnStatusIdx)
{
    boolean                 ret = FALSE;
    const DoIP_PBRaCfgType* pbRa;

    /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
    for (uint8 i = 0u; (i < DoIP_PCCfgPtr->RaNum) && (ret == FALSE); i++)
    {
        pbRa = &DoIP_PBCfgPtr->RaCfgArray[i];

        if (pbRa->RaType == DoIP_TcpConnStatus[tcpConnStatusIdx].RaType) /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
        {
            for (uint8 j = 0u; j < pbRa->TaRefNum; j++)
            {
                if (ta == pbRa->TaRefArray[j])
                {
                    /* PRQA S 2814,2824 -- */
                    ret = TRUE;
                    break;
                }
            }
        }
    }

    return ret;
}

/**
 * @brief Check diagnostic message is valid.
 */
DOIP_LOCAL boolean DoIP_IsValidDiagMsg(uint16 sa, uint16 ta, SoAd_SoConIdType soConId, uint8* nack)
{
    boolean ret = FALSE;

    uint8 index;
    if ((E_OK == DoIP_GetTcpConnStatusIdxBySa(sa, &index))
        && (DOIP_SOCKET_ACTIVATED == DoIP_TcpConnStatus[index].RaState)) /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
    {
        if (TRUE == DoIP_IsTaInRaCfg(ta, index))
        {
            for (uint8 i = 0u; i < DoIP_PCCfgPtr->ChannelNum; i++)
            {
                if ((DoIP_PBCfgPtr->ChannelCfgArray[i].ChannelSaRef->TesterSA == sa)
                    && (DoIP_PBCfgPtr->ChannelCfgArray[i].ChannelTa == ta))
                {
                    ret = TRUE;
                    break;
                }
            }

            if (ret != TRUE)
            {
                /*SWS_DoIP_00127*/
                *nack = DOIP_DIAG_NACK_TARGET_UNREACHABLE; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            }
        }
        else
        {
            *nack = DOIP_DIAG_NACK_UNKNOWN_TA;
        }
    }
    else
    {
        /*SWS_DoIP_00123*/
        *nack = DOIP_DIAG_NACK_INVALID_SA;

        if (E_OK == DoIP_GetTcpConnStatusIdxBySoConId(soConId, &index))
        {
            DoIP_TcpConnStatus[index].WaitCloseSocket = TRUE; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
        }
    }

    return ret;
}

/**
 * @brief Transfer diagnostic message to PduR.
 */
DOIP_LOCAL BufReq_ReturnType DoIP_TransferDiagMsgToPduR(
    PduIdType                         pdurRxPduRef,
    const PduInfoType*                pduInfoPtr,
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx,
    uint8*                            nack)
{
    /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
    DoIP_TcpRxCtrlType* rxCtrl = &DoIP_TcpTxRxContext[handleDiagCtx->TxRxCtxIdx].RxCtrl;
    /* PRQA S 2814,2844 -- */

    BufReq_ReturnType ret = PduR_DoIPTpCopyRxData(pdurRxPduRef, pduInfoPtr, bufferSizePtr);

    if (BUFREQ_OK == ret)
    {
        rxCtrl->BlockSize    = *bufferSizePtr; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        rxCtrl->RxDataRemain = rxCtrl->PayloadLen - (rxCtrl->RxBufPos - (PduLengthType)DOIP_HEADER_LEN);

        /*SWS_DoIP_00259*/
        rxCtrl->RxBufRemain = DOIP_MIN(rxCtrl->RxDataRemain, *bufferSizePtr);
        *bufferSizePtr      = rxCtrl->RxBufRemain;

        if (rxCtrl->RxDataRemain == 0u) /*rx done*/
        {
            /*0x8002 Send diagnostic message positive ack*/
            DoIP_SendDiagnosticAck(handleDiagCtx->SoadTxPduRef, handleDiagCtx->TxRxCtxIdx);

            /*all data has been received*/
            /*SWS_DoIP_00221*/
            PduR_DoIPTpRxIndication(pdurRxPduRef, E_OK);
            DoIP_ResetTcpRxContext(handleDiagCtx->TxRxCtxIdx);
        }
        else /*CF will be received in the next receiption*/
        {
            rxCtrl->RxState = DOIP_RX_STATE_CF;
        }
    }
    else /*SWS_DoIP_00217*/
    {
        /*SWS_DoIP_00174*/
        *nack = DOIP_DIAG_NACK_TRANSPORT_PROTOCOL_ERROR; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */

        PduR_DoIPTpRxIndication(pdurRxPduRef, E_NOT_OK);
    }

    return ret;
}

/**
 * @brief Start transfer diagnostic message to PduR.
 */
DOIP_LOCAL BufReq_ReturnType DoIP_StartTransDataToPduR(
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx,
    uint8*                            nack)
{
    BufReq_ReturnType ret = BUFREQ_OK;

    /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
    uint8*              rxBuf  = DoIP_TcpTxRxContext[handleDiagCtx->TxRxCtxIdx].RxBuf;
    DoIP_TcpRxCtrlType* rxCtrl = &DoIP_TcpTxRxContext[handleDiagCtx->TxRxCtxIdx].RxCtrl;
    /* PRQA S 2814,2844 -- */

    /* Based on req, payloadDataLength(length-4) is compared with MaxBytes-4, */
    /* here we directly compare them without subtraction */
    /* SWS_DoIP_00125 */
    if (rxCtrl->PayloadLen
        <= (PduLengthType)(DOIP_MAX_REQUEST_BYTES - DOIP_SA_TA_LEN)) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        const DoIP_PBPduRRxPduType* pdurRxPdu;

        uint16 sa = DOIP_U8_2_U16(&rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_SA]);
        uint16 ta = DOIP_U8_2_U16(&rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_TA]);

        /* DoIP_IsValidDiagMsg has judged whether it is valid. */
        (void)DoIP_GetPduRRxPdu(sa, ta, &pdurRxPdu);

        uint8       metaData[DOIP_SA_TA_LEN];
        PduInfoType pduInfo;

        rxCtrl->Sa = sa;
        rxCtrl->Ta = ta;

        if (pdurRxPdu->HasMetaData == TRUE) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        {
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
            metaData[0] = rxBuf[11];
            metaData[1] = rxBuf[10];
            metaData[2] = rxBuf[9];
            metaData[3] = rxBuf[8];
#else
            DoIP_MemCpy(metaData, &rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_SA], DOIP_SA_TA_LEN);
#endif
            /* PRQA S 3120 -- */
            pduInfo.MetaDataPtr = metaData;
        }

        rxCtrl->PdurRxPduRef = pdurRxPdu->PdurRxPduRef;
        pduInfo.SduLength    = 0u;

        BufReq_ReturnType localRet = PduR_DoIPTpStartOfReception(
            pdurRxPdu->PdurRxPduRef,
            &pduInfo,
            rxCtrl->PayloadLen - DOIP_SA_TA_LEN,
            bufferSizePtr);

        if (BUFREQ_OK == localRet)
        {
            pduInfo.SduLength = rxCtrl->RxBufPos - (PduLengthType)DOIP_HEADER_LEN - (PduLengthType)DOIP_SA_TA_LEN;

            /* PRQA S 2977 ++ */ /* VL_DoIP_PartialInit */
            if (*bufferSizePtr >= pduInfo.SduLength)
            {
                pduInfo.SduDataPtr = &rxBuf[DOIP_SA_TA_LEN + DOIP_HEADER_LEN];
                ret = DoIP_TransferDiagMsgToPduR(pdurRxPdu->PdurRxPduRef, &pduInfo, bufferSizePtr, handleDiagCtx, nack);
                /* PRQA S 2977 -- */
            }
            else
            {
                PduR_DoIPTpRxIndication(pdurRxPdu->PdurRxPduRef, E_NOT_OK);
                rxCtrl->RxState = DOIP_RX_STATE_DISCARD;
            }
        }
        else if (BUFREQ_E_OVFL == localRet) /*req 13400 DoIP_073*/
        {
            *nack = DOIP_DIAG_NACK_OUT_OF_MEMORY; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        }
        else /*SWS_DoIP_00216*/
        {
            *nack = DOIP_DIAG_NACK_TRANSPORT_PROTOCOL_ERROR;
        }
    }
    else
    {
        *nack = DOIP_DIAG_NACK_MSG_TOO_LARGE;
    }

    return ret;
}

/**
 * @brief Handle diagnostic message header.
 */
DOIP_LOCAL BufReq_ReturnType DoIP_HandleReceivedDiagMsgHeader(
    const PduInfoType*                pduInfoPtr,
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx)
{
    uint8             nack = DOIP_INVALID_UINT8; /*Note:this code is reserved by 13400-2*/
    BufReq_ReturnType ret  = BUFREQ_OK;

    /* PRQA S 2814,2844,2934 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr,VL_DoIP_ComputeInvalidPtr */
    uint8*              rxBuf  = DoIP_TcpTxRxContext[handleDiagCtx->TxRxCtxIdx].RxBuf;
    DoIP_TcpRxCtrlType* rxCtrl = &DoIP_TcpTxRxContext[handleDiagCtx->TxRxCtxIdx].RxCtrl;

    /* copy sa ta to internal buffer */
    if (pduInfoPtr->SduLength + rxCtrl->RxBufPos > DOIP_RX_BUFFER_SIZE) /* PRQA S 3397 */ /* VL_DoIP_3397 */
    {
        ret = BUFREQ_E_NOT_OK;
    }
    else
    {
        DoIP_MemCpy(&rxBuf[rxCtrl->RxBufPos], pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
        /* PRQA S 2814,2844,2934 -- */
        rxCtrl->RxBufPos += pduInfoPtr->SduLength;
    }

    if (rxCtrl->RxBufPos > (PduLengthType)(DOIP_HEADER_LEN + DOIP_SA_TA_LEN))
    {
        /* check if the connect was already activated */
        uint16 sa = DOIP_U8_2_U16(&rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_SA]);
        uint16 ta = DOIP_U8_2_U16(&rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_TA]);

        if (DoIP_IsValidDiagMsg(sa, ta, handleDiagCtx->SoConId, &nack) == TRUE)
        {
            ret = DoIP_StartTransDataToPduR(bufferSizePtr, handleDiagCtx, &nack);
        }
    }
    else if (ret != BUFREQ_E_NOT_OK) /*Offset <= (PduLengthType)(DOIP_HEADER_LEN + DOIP_SA_TA_LEN), incomplete sa&ta
                                        received*/
    {
        rxCtrl->RxBufRemain = rxCtrl->PayloadLen - (rxCtrl->RxBufPos - (PduLengthType)DOIP_HEADER_LEN);

        *bufferSizePtr = rxCtrl->RxBufRemain; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        ret            = BUFREQ_OK;

        /*RxState not change in this case*/
    }
    else
    {
        /* for QAC */
    }

    if (DOIP_INVALID_UINT8 != nack)
    {
        DoIP_SendDiagnosticNack(handleDiagCtx->SoadTxPduRef, handleDiagCtx->TxRxCtxIdx, nack);

        rxCtrl->RxState = DOIP_RX_STATE_DISCARD;

        /* Don't set ret is BUFREQ_NOT_OK to receive remain data */
    }

    return ret;
}

/**
 * @brief Handle diagnostic message.
 */
DOIP_LOCAL BufReq_ReturnType DoIP_HandleDiagMsgCfReceiving(
    const PduInfoType*                pduInfoPtr,
    PduLengthType*                    bufferSizePtr,
    const DoIP_HandleDiagContextType* handleDiagCtx)
{
    BufReq_ReturnType ret  = BUFREQ_OK;
    uint8             nack = DOIP_INVALID_UINT8; /*Note:this code is reserved by 13400-2*/

    /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
    DoIP_TcpRxCtrlType* rxCtrl = &DoIP_TcpTxRxContext[handleDiagCtx->TxRxCtxIdx].RxCtrl;
    /* PRQA S 2814,2844 -- */

    /*SWS_DoIP_00253*/
    if (pduInfoPtr->SduLength > rxCtrl->BlockSize) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        PduR_DoIPTpRxIndication(rxCtrl->PdurRxPduRef, E_NOT_OK);
        DoIP_ResetTcpRxContext(handleDiagCtx->TxRxCtxIdx);
        ret = BUFREQ_E_NOT_OK;
    }
    else
    {
        rxCtrl->RxBufPos += pduInfoPtr->SduLength;
        ret = DoIP_TransferDiagMsgToPduR(rxCtrl->PdurRxPduRef, pduInfoPtr, bufferSizePtr, handleDiagCtx, &nack);
    }

    if (DOIP_INVALID_UINT8 != nack)
    {
        DoIP_SendDiagnosticNack(handleDiagCtx->SoadTxPduRef, handleDiagCtx->TxRxCtxIdx, nack);

        rxCtrl->RxState = DOIP_RX_STATE_DISCARD;
    }

    return ret;
}

/* ----------------------------------------- send ----------------------------------------- */

/**
 * @brief Fill diagnostic message header.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE void DoIP_FillDiagnosticHeader(const PduInfoType* pduInfoPtr, uint8 txRxCtxIdx, uint16 sa, uint16 ta)
/* PRQA S 1505 -- */
{
    /* PRQA S 2844,2814 ++ */ /* VL_DoIP_DerefInvalidPtr,VL_QAC_DerefNullPtr */
    DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;
    DoIP_u16_2_u8(&DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_DIAG);
    DoIP_u32_2_u8(
        &DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN],
        (PduLengthType)(DOIP_SA_TA_LEN + pduInfoPtr->SduLength));
    DoIP_u16_2_u8(&DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf[DOIP_DIAG_MSG_FIELD_POSITION_SA], sa);
    DoIP_u16_2_u8(&DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf[DOIP_DIAG_MSG_FIELD_POSITION_TA], ta);
    /* PRQA S 2844,2814 -- */
}

/**
 * @brief Send diagnostic message header.
 */
DOIP_LOCAL BufReq_ReturnType DoIP_SoAdTpCopyTxTpHeader(
    DoIP_TcpTxRxContextType* ctx,
    const PduInfoType*       pduInfoPtr,
    PduLengthType*           availableDataPtr,
    const RetryInfoType*     retry)
{
    BufReq_ReturnType ret = BUFREQ_OK;

    DoIP_TcpTxCtrlType* txCtrl = &ctx->TxCtrl; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    const uint8*        txBuf  = ctx->TxBuf;

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if (pduInfoPtr->SduLength == 0u)
    {
        *availableDataPtr = (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN - txCtrl->TxBufPos;
        /* PRQA S 2814 -- */
    }
    else if ((txCtrl->TxBufPos + pduInfoPtr->SduLength) > (PduLengthType)(DOIP_HEADER_LEN + DOIP_SA_TA_LEN))
    {
        /*maybe not happen,because this is depend on the lower layer application*/
        /*first to copy header bytes from the internal buffer*/
        DoIP_MemCpy(
            pduInfoPtr->SduDataPtr,
            /* PRQA S 2934 ++ */ /* VL_DoIP_ComputeInvalidPtr */
            &txBuf[txCtrl->TxBufPos],
            /* PRQA S 2934 -- */
            (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN - txCtrl->TxBufPos);

        txCtrl->TxBufPos = (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN;

        /*then to copy the rest bytes from pdur*/
        PduInfoType pduInfo;
        pduInfo.SduDataPtr  = &pduInfoPtr->SduDataPtr[txCtrl->TxBufPos];
        pduInfo.MetaDataPtr = NULL_PTR;
        pduInfo.SduLength   = pduInfoPtr->SduLength - (PduLengthType)DOIP_HEADER_LEN - (PduLengthType)DOIP_SA_TA_LEN;

        if (BUFREQ_OK == PduR_DoIPTpCopyTxData(txCtrl->PdurTxPduRef, &pduInfo, retry, availableDataPtr))
        {
            txCtrl->TxState = DOIP_TX_STATE_TRANSMITED_HEADER;
            txCtrl->TxBufPos += pduInfo.SduLength;
            *availableDataPtr = txCtrl->TxMaxDataLength - txCtrl->TxBufPos;
        }
        else
        {
            ret = BUFREQ_E_NOT_OK;
        }
    }
    else
    {
        DoIP_MemCpy(pduInfoPtr->SduDataPtr, &txBuf[txCtrl->TxBufPos], pduInfoPtr->SduLength);
        txCtrl->TxBufPos += pduInfoPtr->SduLength;
        if (txCtrl->TxBufPos == ((PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN))
        {
            txCtrl->TxState = DOIP_TX_STATE_TRANSMITED_HEADER;
            *availableDataPtr =
                txCtrl->TxMaxDataLength - (PduLengthType)DOIP_HEADER_LEN - (PduLengthType)DOIP_SA_TA_LEN;
        }
        else
        {
            *availableDataPtr = (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN - txCtrl->TxBufPos;
        }
    }

    return ret;
}

/**
 * @brief Send diagnostic ACK.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_DoIP_STCAL */
DOIP_LOCAL void DoIP_SendDiagnosticAck(uint16 soadTxPduRef, uint8 txRxCtxIdx)
/* PRQA S 6070 -- */
{
    uint16 payloadLen = 0u;
    uint8  buf[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];
    SchM_Enter_DoIP_ExclusiveArea();
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->TesterNum; i++) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        /* PRQA S 2814,2844,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr,VL_DoIP_ArithNullptr */
        if (DoIP_PBCfgPtr->TesterCfgArray[i].TesterSA
            == DOIP_U8_2_U16(&DoIP_TcpTxRxContext[txRxCtxIdx].RxBuf[DOIP_DIAG_MSG_FIELD_POSITION_SA]))
        /* PRQA S 2814,2844,2824 -- */
        {
            uint16 numByteDiagAckNack = DoIP_PBCfgPtr->TesterCfgArray[i].NumByteDiagAckNack;
            uint16 recvPayload = DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl.RxBufPos - DOIP_HEADER_LEN - DOIP_SA_TA_LEN;
            if (recvPayload > numByteDiagAckNack)
            {
                payloadLen = numByteDiagAckNack + DOIP_DIAG_ACK_NACK_MSG_LEN;
            }
            else
            {
                payloadLen = recvPayload + DOIP_DIAG_ACK_NACK_MSG_LEN;
            }

            break;
        }
    }

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;

    DoIP_u16_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_DIAG_ACK);

    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], payloadLen);

    /* sa */
    DoIP_MemCpy(
        &buf[DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_SA],
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        &DoIP_TcpTxRxContext[txRxCtxIdx]
            .RxBuf[DOIP_DIAG_MSG_FIELD_POSITION_TA],
        /* PRQA S 2844 -- */
        DOIP_LA_LEN);

    /* ta */
    DoIP_MemCpy(
        &buf[DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_TA],
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        &DoIP_TcpTxRxContext[txRxCtxIdx]
            .RxBuf[DOIP_DIAG_MSG_FIELD_POSITION_SA],
        /* PRQA S 2844 -- */
        DOIP_LA_LEN);

    /* ACK */
    buf[DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_CODE] = 0x00u;
    /* PRQA S 2907 ++ */ /* VL_DoIP_2907 */
    DoIP_MemCpy(
        &buf[DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_PRE_UDS],
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        &DoIP_TcpTxRxContext[txRxCtxIdx]
            .RxBuf[DOIP_DIAG_MSG_FIELD_POSITION_UDS],
        /* PRQA S 2844 -- */
        (PduLengthType)((PduLengthType)payloadLen - DOIP_DIAG_ACK_NACK_MSG_LEN));
    /* PRQA S 2907 -- */
    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = DOIP_HEADER_LEN + (PduLengthType)payloadLen;

    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
    /* PRQA S 2844 -- */

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if ((txCtrl->TxBufState == DOIP_BUFFER_IDLE) && (txCtrl->TxState == DOIP_TX_STATE_IDLE))
    /* PRQA S 2814 -- */
    {
        DoIP_MemCpy(DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf, buf, pduInfo.SduLength);
        if (E_NOT_OK == DoIP_TriggerTpTransmit(soadTxPduRef, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG))
        {
            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            (void)DoIP_EnqueueNonDiagMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxNonDiagQueue, soadTxPduRef, &pduInfo);
            /* PRQA S 2844 -- */ /* VL_DoIP_DerefInvalidPtr */
            DoIP_ResetTcpTxContext(txRxCtxIdx);
        }
    }
    else
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        (void)DoIP_EnqueueNonDiagMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxNonDiagQueue, soadTxPduRef, &pduInfo);
        /* PRQA S 2844 -- */ /* VL_DoIP_DerefInvalidPtr */
    }
    SchM_Exit_DoIP_ExclusiveArea();
}

/**
 * @brief Send diagnostic NACK.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_DoIP_STCAL */
DOIP_LOCAL void DoIP_SendDiagnosticNack(uint16 soadTxPduRef, uint8 txRxCtxIdx, uint8 nack)
/* PRQA S 6070 -- */
{
    PduLengthType payloadLen = DOIP_DIAG_ACK_NACK_MSG_LEN;
    uint8         buf[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];
    const uint8*  rxBuf = DoIP_TcpTxRxContext[txRxCtxIdx].RxBuf; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
    SchM_Enter_DoIP_ExclusiveArea();
    uint16 sa = DOIP_U8_2_U16(&rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_SA]);
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->TesterNum; i++) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
        if (DoIP_PBCfgPtr->TesterCfgArray[i].TesterSA == sa)
        /* PRQA S 2814,2824 -- */
        {
            uint16 numByteDiagAckNack = DoIP_PBCfgPtr->TesterCfgArray[i].NumByteDiagAckNack;
            uint16 recvPayload = DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl.RxBufPos - DOIP_HEADER_LEN - DOIP_SA_TA_LEN;
            if (recvPayload > numByteDiagAckNack)
            {
                payloadLen = numByteDiagAckNack + DOIP_DIAG_ACK_NACK_MSG_LEN;
            }
            else
            {
                payloadLen = recvPayload + DOIP_DIAG_ACK_NACK_MSG_LEN;
            }
        }
    }

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;

    DoIP_u16_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_DIAG_NACK);

    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], payloadLen);

    /* sa */
    DoIP_MemCpy(&buf[DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_SA], &rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_TA], DOIP_LA_LEN);

    /* ta */
    DoIP_MemCpy(&buf[DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_TA], &rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_SA], DOIP_LA_LEN);

    /* nack */
    buf[DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_CODE] = nack;

    /* previous diag msg */
    DoIP_MemCpy(
        &buf[DOIP_DIAG_ACK_NACK_MSG_FIELD_POSITION_PRE_UDS],
        &rxBuf[DOIP_DIAG_MSG_FIELD_POSITION_UDS],
        payloadLen - DOIP_DIAG_ACK_NACK_MSG_LEN);

    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = DOIP_HEADER_LEN + payloadLen;

    /* PRQA S 2844, 2814 ++ */ /* VL_DoIP_DerefInvalidPtr,VL_QAC_DerefNullPtr */
    DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
    if ((txCtrl->TxBufState == DOIP_BUFFER_IDLE) && (txCtrl->TxState == DOIP_TX_STATE_IDLE))
    {
        DoIP_MemCpy(DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf, buf, pduInfo.SduLength);
        if (E_NOT_OK == DoIP_TriggerTpTransmit(soadTxPduRef, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG))
        {
            (void)DoIP_EnqueueNonDiagMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxNonDiagQueue, soadTxPduRef, &pduInfo);
            DoIP_ResetTcpTxContext(txRxCtxIdx);
        }
    }
    else
    {
        (void)DoIP_EnqueueNonDiagMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxNonDiagQueue, soadTxPduRef, &pduInfo);
        /* PRQA S 2844, 2814 -- */
    }
    SchM_Exit_DoIP_ExclusiveArea();
}
/* PRQA S 4461 -- */
#define DOIP_STOP_SEC_CODE
#include "DoIP_MemMap.h"
