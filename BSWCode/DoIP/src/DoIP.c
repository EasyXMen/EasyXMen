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
 **  @file               : DoIP.c
 **  @author             : YB
 **  @date               : 2019/8/30
 **  @vendor             : isoft
 **  @description        : Diagnostic over IP
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

#include "PduR_DoIP.h"
#include "DoIP.h"
#include "DoIP_Internal_TCP.h"
#include "DoIP_Internal_UDP.h"
#include "DoIP_Internal_DET.h"
#include "SchM_DoIP.h"
/* ============================================ external data definitions =========================================== */

#define DOIP_START_SEC_VAR_INIT_UNSPECIFIED
#include "DoIP_MemMap.h"
/**
 * @brief PC configuration
 */
const DoIP_PCConfigType* DoIP_PCCfgPtr = &DoIP_PCConfig;
#define DOIP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_PTR
#include "DoIP_MemMap.h"
/**
 * @brief PB configuration
 */
const DoIP_PBConfigType* DoIP_PBCfgPtr;
#define DOIP_STOP_SEC_VAR_CLEARED_PTR
#include "DoIP_MemMap.h"

/* ========================================== external function definitions ========================================= */

/* PRQA S 1512 EOF */ /* VL_DoIP_1512 */
#define DOIP_START_SEC_CODE
#include "DoIP_MemMap.h"
/* PRQA S 3415 ++ */ /* VL_DoIP_3415 */
/* PRQA S 4461 ++ */ /* VL_DoIP_4461 */
/**
 * @brief This service initializes all global variables of the DoIP module. After return of
 *        this service the DoIP module is operational.
 * @param[in]   doipConfigPtr: PB configuration.
 */
void DoIP_Init(const DoIP_ConfigType* doipConfigPtr) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    if (TRUE == DOIP_INIT_DET(doipConfigPtr))
    {
        DoIP_PBCfgPtr          = doipConfigPtr;
        DoIP_CurrentTcpConnCnt = 0u;

        DoIP_ClearPendingRaMsg();

        for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
        {
            DoIP_ResetTcpConnStatus(i);
            DoIP_ResetTcpTxRxContext(i);
        }

        for (uint8 i = 0u; i < DOIP_UDP_MAX_CONNECTION; i++)
        {
            DoIP_ResetUdpConnStatus(i);
        }

        for (uint8 i = 0u; i < DOIP_ANNOUNCEMENT_MAX_CONNECTION; i++)
        {
            DoIP_AncConnStatus[i].SoConId    = DOIP_INVALID_UINT8;
            DoIP_AncConnStatus[i].SoConState = SOAD_SOCON_OFFLINE;
        }
    }

    DoIP_InitStatus = DOIP_STATUS_INITED;
}

#if (TRUE == DOIP_VERSION_INFO_API)
/**
 * @brief       Returns the version information of this module.
 */
void DoIP_GetVersionInfo(Std_VersionInfoType* versionInfo) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    if (TRUE == DOIP_GETVERSIONINFO_DET(versionInfo))
    {
        versionInfo->moduleID         = DOIP_MODULE_ID; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        versionInfo->vendorID         = DOIP_VENDOR_ID;
        versionInfo->sw_major_version = DOIP_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = DOIP_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = DOIP_SW_PATCH_VERSION;
    }
}
#endif

/**
 * @brief This function is used to notify the DoIP on a switch of the DoIPActivationLine
 */
/* PRQA S 1532 ++ */                                              /* VL_QAC_OneFunRef */
void DoIP_ActivationLineSwitch(boolean* active) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    if (TRUE == DOIP_ACTIVATIONLINESWITCH_DET())
    {
        /* PRQA S 4558,4404,4115 ++ */ /* VL_DoIP_4558 */
        boolean ret = DoIP_SwitchUdp(active);
        ret         = ret && DoIP_SwitchTcp(active);
        ret         = ret && DoIP_SwitchAnc(active);
        /* PRQA S 4558,4404,4115 -- */

        if ((TRUE == *active) && (ret != TRUE)) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        {
            *active = FALSE;
        }
        else if ((FALSE == *active) && (ret != TRUE))
        {
            *active = TRUE;
        }
        else
        {
            /* for qac idle */
        }
    }
}

/**
 * @brief This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 *        Each call to this function provides the next part of the I-PDU data unless retry->TpDataState is TP_DATARETRY.
 */
/* PRQA S 3408,1503 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_QAC_NoUsedApi */
BufReq_ReturnType DoIP_SoAdTpCopyTxData(
    /* PRQA S 3408,1503 -- */
    PduIdType            soadTxPduId,
    const PduInfoType*   pduInfoPtr,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr)
{
    BufReq_ReturnType ret = DOIP_SOADTPCOPYTXDATA_DET(soadTxPduId, pduInfoPtr, retry, availableDataPtr);

    if (BUFREQ_OK == ret)
    {
        uint8                    index;
        DoIP_TcpTxRxContextType* ctx;

        if (E_OK == DoIP_GetTcpConnStatusIdxBySoAdTxPduId(soadTxPduId, &index))
        {
            /* PRQA S 2844, 2934 ++ */ /* VL_DoIP_DerefInvalidPtr,VL_DoIP_ComputeInvalidPtr */
            ctx = &DoIP_TcpTxRxContext[DoIP_TcpConnStatus[index].TxRxCtxIdx];
            /* PRQA S 2844, 2934 -- */

            DoIP_TcpConnStatus[index].GeneralInactiveTimer = 0u; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
        }
        else
        {
            ret = BUFREQ_E_NOT_OK;
        }

        if (BUFREQ_OK == ret)
        {
            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            const DoIP_TcpTxCtrlType* txCtrl = &ctx->TxCtrl;

            if ((txCtrl->TxMsgType == DOIP_TX_NON_DIAG_MSG) || (txCtrl->TxMsgType == DOIP_TX_DIAG_IF_MSG))
            /* PRQA S 2844 -- */ /* VL_DoIP_DerefInvalidPtr */
            {
                ret = DoIP_SoAdTpCopyTxNoTpData(ctx, pduInfoPtr, availableDataPtr);
            }
            else if (txCtrl->TxMsgType == DOIP_TX_DIAG_TP_MSG)
            {
                ret = DoIP_SoAdTpCopyTxTpData(ctx, pduInfoPtr, retry, availableDataPtr);
            }
            else
            {
                *availableDataPtr = 0u;
                ret               = BUFREQ_E_NOT_OK;
            }
        }
    }

    return ret;
}

/**
 * @brief Returns the version information of this module.
 */
/* PRQA S 3408,1503 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_QAC_NoUsedApi */
void DoIP_SoAdTpTxConfirmation(PduIdType soadTxPduId, Std_ReturnType result)
/* PRQA S 3408,1503 -- */
{
    if (TRUE == DOIP_SOADTPTXCONFIRMATION_DET(soadTxPduId, result))
    {
        uint8 index;
        if ((E_OK == DoIP_GetTcpConnStatusIdxBySoAdTxPduId(soadTxPduId, &index))
            && (DOIP_INVALID_UINT8
                != DoIP_TcpConnStatus[index].TxRxCtxIdx)) /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
        {
            uint8 txRxCtxIdx = DoIP_TcpConnStatus[index].TxRxCtxIdx;

            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            const DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
            /* PRQA S 2844 -- */

            if (DOIP_TX_DIAG_TP_MSG == txCtrl->TxMsgType) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            {
                PduR_DoIPTpTxConfirmation(txCtrl->PdurTxPduRef, result);
            }
            else if (DOIP_TX_DIAG_IF_MSG == txCtrl->TxMsgType)
            {
                PduR_DoIPIfTxConfirmation(txCtrl->PdurTxPduRef, result);
            }
            else
            {
                /*nothing*/
            }

            if (DoIP_TcpConnStatus[index].WaitCloseSocket == TRUE) /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
            {
                DoIP_CloseSoCon(DoIP_TcpConnStatus[index].SoConId, TRUE);
            }

            /* reset txbuffer including reset the processStatus to IDLE,except the pending tx buffer */
            if ((txCtrl->TxBufPos == txCtrl->TxMaxDataLength) || (E_NOT_OK == result))
            {
                DoIP_ResetTcpTxContext(txRxCtxIdx);
            }
        }
    }
}

/**
 * @brief This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper layer.
 */
/* PRQA S 3408,1503 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_QAC_NoUsedApi */
BufReq_ReturnType
    DoIP_SoAdTpCopyRxData(PduIdType soadRxPduId, const PduInfoType* pduInfoPtr, PduLengthType* bufferSizePtr)
/* PRQA S 3408,1503 -- */
{
    uint8                      txRxCtxIdx   = 0u;
    uint16                     soadTxPduRef = 0u;
    DoIP_HandleDiagContextType handleDiagCtx;
    uint8                      tcpConnStatusIdx;

    BufReq_ReturnType ret = DOIP_SOADTPCOPYRXDATA_DET(soadRxPduId, pduInfoPtr, bufferSizePtr);

    if (BUFREQ_OK == ret)
    {
        ret = DoIP_GetResourseForRcvTpMsg(
            soadRxPduId,
            &soadTxPduRef,
            &handleDiagCtx.SoConId,
            &txRxCtxIdx,
            &tcpConnStatusIdx);
    }

    if ((BUFREQ_OK == ret))
    {
        /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
        DoIP_TcpConnStatus[tcpConnStatusIdx].GeneralInactiveTimer = 0u;

        DoIP_TcpRxCtrlType* rxCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl;

        /*SWS_DoIP_00208*/
        switch (rxCtrl->RxState)
        {
        case DOIP_RX_STATE_START:

            /*with the first step,does not need to copy data*/
            if (pduInfoPtr->SduLength == 0u)
            {
                *bufferSizePtr = DOIP_HEADER_LEN;
                /* PRQA S 2814,2844 -- */
                rxCtrl->RxState = DOIP_RX_STATE_RECEIVING_HEADER;
            }
            else
            {
                /*may not happen,this is depend on the soad module*/
                ret = BUFREQ_E_NOT_OK;
            }
            break;

        case DOIP_RX_STATE_RECEIVING_HEADER:

            ret = DoIP_HandleTpRxHeader(pduInfoPtr, soadTxPduRef, bufferSizePtr, txRxCtxIdx);
            break;

        case DOIP_RX_STATE_RECEIVED_HEADER:
        case DOIP_RX_STATE_CF:

            handleDiagCtx.TxRxCtxIdx = txRxCtxIdx;
            ret                      = DoIP_HandleTpCFRx(&handleDiagCtx, pduInfoPtr, soadTxPduRef, bufferSizePtr);
            break;

        case DOIP_RX_STATE_DISCARD:

            /*at discard state,we intend to not copy data,just change the offset*/
            rxCtrl->RxBufPos += pduInfoPtr->SduLength;
            if (rxCtrl->RxBufPos >= (rxCtrl->PayloadLen + (PduLengthType)DOIP_HEADER_LEN))
            {
                *bufferSizePtr = 0u;
                DoIP_ResetTcpRxContext(txRxCtxIdx);
            }
            else
            {
                *bufferSizePtr = rxCtrl->PayloadLen + (PduLengthType)DOIP_HEADER_LEN - rxCtrl->RxBufPos;
            }
            break;
        /* PRQA S 2016 ++ */ /* VL_DoIP_2016 */
        default:
            break;
            /* PRQA S 2016 -- */
        }
    }

    return ret;
}

/**
 * @brief This function is called at the start of receiving an N-SDU.
 *        The N-SDU might be fragmented into multiple N-PDUs (FF with one or more following CFs) or might consist of a
 * single N-PDU (SF).
 */
/* PRQA S 3408,1503,6030 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_QAC_NoUsedApi,VL_MTR_DoIP_STMIF */
BufReq_ReturnType DoIP_SoAdTpStartOfReception(
    PduIdType          soadRxPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType      sduLen,
    PduLengthType*     bufferSizePtr)
/* PRQA S 3408,1503,6030 -- */
{
    uint8               txRxCtxIdx    = 0u;
    uint8               connStatusIdx = 0u;
    DoIP_TcpRxCtrlType* rxCtrl        = NULL_PTR;
    BufReq_ReturnType   ret           = DOIP_SOADTPSTARTOFRECEPTION_DET(soadRxPduId, sduLen, bufferSizePtr);

    if (BUFREQ_OK == ret)
    {
        if ((DoIP_GetTcpConnStatusIdxBySoAdRxPduId(soadRxPduId, &connStatusIdx) != E_OK)
            || (SOAD_SOCON_ONLINE != DoIP_TcpConnStatus[connStatusIdx].SoConState))
        {
            ret = BUFREQ_E_NOT_OK;
        }
        /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
        else if ((DoIP_GetTcpTxRxCtxIdxBySoAdRxPduId(soadRxPduId, &txRxCtxIdx) == E_OK))
        {
            rxCtrl = &(DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl);
            if ((rxCtrl->RxBufState == DOIP_BUFFER_IDLE) && (rxCtrl->RxState == DOIP_RX_STATE_START))
            /* PRQA S 2814,2844 -- */
            {
                rxCtrl->RxBufState = DOIP_BUFFER_OCCUPIED;
                ret                = BUFREQ_OK;
            }
            else
            {
                ret = BUFREQ_E_BUSY;
            }
        }
        else /*get a new idle buffer*/
        {
            for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
            {
                rxCtrl = &DoIP_TcpTxRxContext[i].RxCtrl;
                /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
                if ((rxCtrl->RxBufState == DOIP_BUFFER_IDLE) && (rxCtrl->RxState == DOIP_RX_STATE_IDLE))
                {
                    rxCtrl->RxBufState  = DOIP_BUFFER_OCCUPIED;
                    rxCtrl->RxState     = DOIP_RX_STATE_START;
                    rxCtrl->SoadRxPduId = soadRxPduId;
                    /* PRQA S 2814 -- */ /* VL_QAC_DerefNullPtr */
                    txRxCtxIdx = i;
                    ret        = BUFREQ_OK;

                    break;
                }
            }
        }
    }

    /*SWS_DoIP_00207*/
    if (BUFREQ_OK == ret)
    {
        /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
        DoIP_TcpConnStatus[connStatusIdx].TxRxCtxIdx = txRxCtxIdx;

        *bufferSizePtr = DOIP_RX_BUFFER_SIZE;
        /* PRQA S 2814,2844 -- */
    }

    (void)pduInfoPtr;

    return ret;
}

/**
 * @brief Called after an I-PDU has been received via the TP API, the result indicates whether the transmission was
 * successful or not.
 */
/* PRQA S 3408,1503 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_QAC_NoUsedApi */
void DoIP_SoAdTpRxIndication(PduIdType soadRxPduId, Std_ReturnType result)
/* PRQA S 3408,1503 -- */
{
    if (TRUE == DOIP_SOADTPRXINDICATION_DET(soadRxPduId, result)) /*SWS_DoIP_00200*/
    {
        uint8 idx;
        if (E_OK == DoIP_GetTcpTxRxCtxIdxBySoAdRxPduId(soadRxPduId, &idx))
        {
            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            const DoIP_TcpRxCtrlType* rxCtrl = &(DoIP_TcpTxRxContext[idx].RxCtrl);
            /* PRQA S 2844 -- */

            /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
            if ((rxCtrl->RxState == DOIP_RX_STATE_CF) && (rxCtrl->PayloadType == DOIP_MSG_TYPE_DIAG))
            /* PRQA S 2814 -- */
            {
                PduIdType pdurRxPduId;
                if (E_OK == DoIP_GetPduRRxPduRef(rxCtrl->Sa, rxCtrl->Ta, &pdurRxPduId))
                {
                    PduR_DoIPTpRxIndication(pdurRxPduId, result);
                }
            }
        }

        /*diag msg/diag ack/diag nack will be realease here*/
        if (E_OK == DoIP_GetTcpConnStatusIdxBySoAdRxPduId(soadRxPduId, &idx))
        {
            DoIP_CloseTcpConnection(DoIP_TcpConnStatus[idx].SoConId); /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
        }
    }
}

/**
 * @brief Indication of a received I-PDU from a lower layer communication interface module.
 */
/* PRQA S 3408,1503,6070 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_QAC_NoUsedApi,VL_MTR_DoIP_STCAL */
void DoIP_SoAdIfRxIndication(PduIdType soadRxPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 3408,1503,6070 -- */
{
    uint16           soadTxPduRef;
    SoAd_SoConIdType soConId;

    if ((TRUE == DOIP_SOADIFRXINDICATION_DET(soadRxPduId, pduInfoPtr))
        && (E_OK == DoIP_GetSoAdTxPduRefAndSoConId(soadRxPduId, &soadTxPduRef, &soConId)))
    {
        /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
        if (DOIP_GENERICHEADER_CHECK(pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength) == TRUE)
        /* PRQA S 2814 -- */
        {
            if (DoIP_CheckUdpMsgPayload(pduInfoPtr, soadTxPduRef, soConId) == E_OK)
            {
                /* PRQA S 2824 ++ */ /* VL_DoIP_ArithNullptr */
                uint32 payloadLen = DOIP_U8_2_U32(&pduInfoPtr->SduDataPtr[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN]);
                /* PRQA S 2824 -- */

                DoIP_HandleIfMsg(soadTxPduRef, soConId, payloadLen, pduInfoPtr);
            }
        }
        else
        {
            DoIP_SendUdpNack(soadTxPduRef, DOIP_HEADER_NACK_INCORRECT_PATTERN_FORMAT);
            DoIP_CloseSoCon(soConId, TRUE);
        }
    }
}

/* PRQA S 3408 ++ */ /* VL_DoIP_3408 */
/**
 * @brief The lower layer communication interface module confirms the transmission of an I-PDU.
 */
void DoIP_SoAdIfTxConfirmation(PduIdType soadTxPduId, Std_ReturnType result)
{
    if (TRUE == DOIP_SOADIFTXCONFIRMATION_DET(soadTxPduId))
    {
        for (uint8 i = 0u; i < DOIP_UDP_MAX_CONNECTION; i++)
        {
            if (DoIP_UdpConnStatus[i].SoadTxPduId == soadTxPduId)
            {
                SoAd_ReleaseRemoteAddr(DoIP_UdpConnStatus[i].SoConId);
                break;
            }
        }
    }

    for (uint8 i = 0u; i < DOIP_ANNOUNCEMENT_MAX_CONNECTION; i++)
    {
        if ((DoIP_PBCfgPtr->ConnsCfg->AncConnCfgArray[i].SoAdTxPdu.SoAdTxPduId == soadTxPduId) && (result == E_OK))
        {
            DoIP_AncConnStatus[i].AncCnt++;
            DoIP_AncConnStatus[i].AncTimer = 0u;
        }
    }
}
/* PRQA S 3408 -- */

/**
 * @brief Notification about a SoAd socket connection state change,
 *        e.g. socket connection gets online
 */
/* PRQA S 3408,1503 ++ */ /* VL_DoIP_WithoutPreDeclaration,VL_QAC_NoUsedApi */
void DoIP_SoConModeChg(SoAd_SoConIdType soConId, SoAd_SoConModeType mode)
{
    if (TRUE == DOIP_SOCONMODECFG_DET(soConId, mode))
    {
        DoIP_SoConModeChgTcp(soConId, mode);

        DoIP_SoConModeChgUdp(soConId, mode);

        DoIP_SoConModeChgAnc(soConId, mode);
    }
}

/**
 * @brief This function gets called by the SoAd if an IP address assignment related to a socket connection changes.
 */
void DoIP_LocalIpAddrAssignmentChg(SoAd_SoConIdType soConId, TcpIp_IpAddrStateType state)
/* PRQA S 3408,1503 -- */
{
    if (TRUE == DOIP_LOCALIPADDRASSIGNMENTCHG_DET(soConId, state))
    {
        /*SWS_DoIP_00154*/
        if (TCPIP_IPADDR_STATE_ASSIGNED == state)
        {
#if (TRUE == DOIP_VIN_GID_MASTER)
            if ((DoIP_General.TriggerGidSyncDirect != NULL_PTR) && (E_OK == DoIP_General.TriggerGidSyncDirect()))
            {
                DoIP_GidSynStatus = TRUE;
            }
            else
            {
                DoIP_GidSynStatus = FALSE;
            }
#endif
        }
        /*SWS_DoIP_00085*/
        else
        {
            DoIP_GidSynStatus = FALSE;
        }
    }
}

/**
 * @brief This service is called to request the transfer data from the PduRouter to the SoAd.
 *        It is used to indicate the transmission which will be performed in the DoIP_Mainfunction.
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
/* PRQA S 6070 ++ */ /* VL_MTR_DoIP_STCAL */
Std_ReturnType DoIP_TpTransmit(PduIdType DoIPPduRTxId, const PduInfoType* DoIPPduRTxInfoPtr)
/* PRQA S 1503 -- */
/* PRQA S 6070 -- */
{
    Std_ReturnType ret = DOIP_TPTRANSMIT_DET(DoIPPduRTxId, DoIPPduRTxInfoPtr);

    if (E_OK == ret) /*SWS_DoIP_00284*/
    {
        uint8  txRxCtxIdx;
        uint16 channelIdx;

        if ((E_OK == DoIP_GetChannelIdxByPduRTxPduId(DoIPPduRTxId, DOIP_TPPDU, &channelIdx))
            && (TRUE == DoIP_IsValidMatadata(channelIdx, DoIPPduRTxInfoPtr))
            && (E_OK == DoIP_GetTcpTxRxCtxIdxByChannelIdx(channelIdx, &txRxCtxIdx))
            && (TRUE == DoIP_IsSoConOnline(channelIdx)))
        {
            SchM_Enter_DoIP_ExclusiveArea();
            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            const DoIP_TcpTxCtrlType* txCtrl = &(DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl);
            /* PRQA S 2844 -- */
            const DoIP_TpQueueType* pendingQueue = &DoIP_TcpTxRxContext[txRxCtxIdx].TxTpQueue;
            uint8                   pendingQueueNum;

            if ((pendingQueue->Tail > pendingQueue->Head) || (pendingQueue->Count == 0u))
            {
                pendingQueueNum = pendingQueue->Tail - pendingQueue->Head;
            }
            else
            {
                pendingQueueNum = (DOIP_TP_QUEUE_BUFFER_NUM - pendingQueue->Head) + pendingQueue->Tail;
            }

            /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
            if ((txCtrl->TxBufState == DOIP_BUFFER_IDLE) && (txCtrl->TxState == DOIP_TX_STATE_IDLE)
                && (pendingQueueNum == 0u))
            /* PRQA S 2814 -- */
            {
                ret = DoIP_TpTransmitInternal(DoIPPduRTxId, DoIPPduRTxInfoPtr);
            }
            else
            {
                ret = DoIP_EnqueueTpMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxTpQueue, DoIPPduRTxId, DoIPPduRTxInfoPtr);
            }
            SchM_Exit_DoIP_ExclusiveArea();
        }
        else
        {
            ret = E_NOT_OK;
        }
    }

    return ret;
}

/**
 * @brief This service primitive is used to cancel the transfer of pending DoIPPduRTxIds. The connection is
 *        identified by DoIPPduRTxId. When the function returns, no transmission is in progress anymore
 */
Std_ReturnType DoIP_TpCancelTransmit(PduIdType DoIPPduRTxId) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    /*SWS_DoIP_00166*/
    Std_ReturnType ret = DOIP_TPCANCELTRANSMIT_DET(DoIPPduRTxId);

    if (E_OK == ret)
    {
        ret = E_NOT_OK;
        uint8 txRxCtxIdx;

        if (E_OK == DoIP_GetTcpTxRxCtxIdxByPduRTxPduId(DoIPPduRTxId, &txRxCtxIdx))
        {
            uint16 soadTxPduRef;

            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            if ((DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl.TxState != DOIP_TX_STATE_IDLE)
                /* PRQA S 2844 -- */
                && (DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl.TxMsgType == DOIP_TX_DIAG_TP_MSG)
                && (E_NOT_OK
                    != DoIP_GetSoAdTxPduRefBySoAdRxPduId(
                        DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl.SoadRxPduId,
                        &soadTxPduRef)))
            {
                (void)SoAd_TpCancelTransmit(soadTxPduRef);

                DoIP_ResetTcpTxContext(txRxCtxIdx);

                ret = E_OK;
            }

            /* PRQA S 2934 ++ */ /* VL_DoIP_ComputeInvalidPtr */
            DOIP_ClearTpQueue(&DoIP_TcpTxRxContext[txRxCtxIdx], FALSE);
            /* PRQA S 2934 -- */
        }
    }

    return ret;
}

/**
 * @brief By calling this API with the corresponding DoIPPduRRxId the currently ongoing data reception
 *        is terminated immediately. When the function returns, no reception is in progress anymore with
 *        the given DoIPPduRRxId identifier.
 */
Std_ReturnType DoIP_TpCancelReceive(PduIdType DoIPPduRRxId) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    /*SWS_DoIP_00169*/
    Std_ReturnType ret = DOIP_TPCANCELRECEIVE_DET(DoIPPduRRxId);

    if (E_OK == ret)
    {
        ret = E_NOT_OK;
        uint8     txRxCtxIdx;
        uint16    index;
        PduIdType soadRxPduId;

        if (E_OK == DoIP_GetChannelIdxByPduRRxPduId(DoIPPduRRxId, &index))
        {
            const DoIP_TcpRxCtrlType* rxCtrl;
            /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
            PduIdType pdurRxPduRef = DoIP_PBCfgPtr->ChannelCfgArray[index].PduRRxPdu->PdurRxPduRef;

            for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
            {
                rxCtrl = &(DoIP_TcpTxRxContext[i].RxCtrl);

                if (rxCtrl->PdurRxPduRef == pdurRxPduRef)
                {
                    soadRxPduId = rxCtrl->SoadRxPduId;
                    /* PRQA S 2814,2824 -- */
                    txRxCtxIdx = i;
                    ret        = E_OK;
                    break;
                }
            }
        }

        if (E_OK == ret)
        {
            const DoIP_PBConnectionCfgType* tcpConn = DoIP_GetTcpConnBySoAdRxPduId(soadRxPduId);
            if (NULL_PTR != tcpConn)
            {
                /*SWS_DoIP_00258*/
                (void)SoAd_TpCancelReceive(tcpConn->SoAdRxPdu.SoAdRxPduRef);
                DoIP_ResetTcpRxContext(txRxCtxIdx);
            }
            else
            {
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}

/**
 * @brief Requests transmission of an I-PDU.
 */
/* PRQA S 1503, 6070 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_DoIP_STCAL */
Std_ReturnType DoIP_IfTransmit(PduIdType id, const PduInfoType* info)
/* PRQA S 1503, 6070 -- */
{
    Std_ReturnType ret = DOIP_IFTRANSMIT_DET(id, info);

    if (E_OK == ret)
    {
        uint8  txRxCtxIdx;
        uint16 channelIdx;

        if ((E_OK == DoIP_GetChannelIdxByPduRTxPduId(id, DOIP_IFPDU, &channelIdx))
            && (TRUE == DoIP_IsValidMatadata(channelIdx, info))
            && (E_OK == DoIP_GetTcpTxRxCtxIdxByChannelIdx(channelIdx, &txRxCtxIdx))
            && (TRUE == DoIP_IsSoConOnline(channelIdx)))
        {
            SchM_Enter_DoIP_ExclusiveArea();
            /* PRQA S 2844 ++ */ /* VL_DoIP_2844 */
            const DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
            /* PRQA S 2844 -- */

            /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
            if ((txCtrl->TxBufState == DOIP_BUFFER_IDLE) && (txCtrl->TxState == DOIP_TX_STATE_IDLE))
            /* PRQA S 2814 -- */
            {
                ret = DoIP_IfTransmitInternal(id, info);
            }
            else
            {
                ret = DoIP_EnqueueIfMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxIfQueue, id, info);
            }
            SchM_Exit_DoIP_ExclusiveArea();
        }
        else
        {
            ret = E_NOT_OK;
        }
    }

    return ret;
}

/**
 * @brief Requests cancellation of an ongoing transmission of an I-PDU in a lower layer communication interface module.
 */
Std_ReturnType DoIP_IfCancelTransmit(PduIdType id) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    Std_ReturnType ret = DOIP_IFCANCELTRANSMIT_DET(id);

    if (E_OK == ret)
    {
        uint8 txRxCtxIdx;

        ret = E_NOT_OK;

        if (E_OK == DoIP_GetTcpTxRxCtxIdxByPduRTxPduId(id, &txRxCtxIdx))
        {
            /* PRQA S 2844 ++ */ /* VL_DoIP_2844 */
            const DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
            /* PRQA S 2844 -- */

            /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
            if ((txCtrl->TxState != DOIP_TX_STATE_IDLE) && (txCtrl->TxMsgType == DOIP_TX_DIAG_IF_MSG))
            /* PRQA S 2814 -- */
            {
                uint16 soadTxPduRef;

                if (E_OK
                    == DoIP_GetSoAdTxPduRefBySoAdRxPduId(
                        DoIP_TcpTxRxContext[txRxCtxIdx].RxCtrl.SoadRxPduId,
                        &soadTxPduRef))
                {
                    (void)SoAd_TpCancelTransmit(soadTxPduRef);
                    /*reset txbuffer including reset the processStatus to IDLE,except the pending tx buffer*/
                    DoIP_ResetTcpTxContext(txRxCtxIdx);

                    ret = E_OK;
                }
            }

            /* PRQA S 2934 ++ */ /* VL_DoIP_ComputeInvalidPtr */
            DOIP_ClearIfQueue(&DoIP_TcpTxRxContext[txRxCtxIdx], FALSE);
            /* PRQA S 2934 -- */
        }
    }

    return ret;
}
/* PRQA S 4461 -- */
/**
 * @brief Schedules the Diagnostic over IP module. (Entry point for scheduling)
 */
void DoIP_MainFunction(void) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    if (DOIP_STATUS_UNINITED != DoIP_InitStatus)
    {
        DoIP_HandleTcpTimeout();

#if (DOIP_HIGH_FREQ_TASK_SUPPORT == FALSE)
        DoIP_HandlePendingMsg();
#endif

        DoIP_HandleVehicleAncTimeout();
    }
}

#if (DOIP_HIGH_FREQ_TASK_SUPPORT == TRUE)
/**
 * @brief Schedules the Diagnostic over IP module. (Entry point for scheduling) for hight frequency tasks.
 */
void DoIP_MainFunction_HighFrequency(void) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
    if (DOIP_STATUS_UNINITED != DoIP_InitStatus)
    {
        DoIP_HandlePendingMsg(); /*pending message transmit handle*/
    }
}
#endif
/* PRQA S 1532 -- */
/* PRQA S 3415 -- */
#define DOIP_STOP_SEC_CODE
#include "DoIP_MemMap.h"
