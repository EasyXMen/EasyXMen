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
**  FILENAME    : DOIP_Internal.c                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Zhiqiang.huang                                              **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "DoIP_Internal.h"
#include "DoIP_Types.h"
#include "SchM_DoIP.h"
/*******************************************************************************
**                      Internal Variable Definitions                         **
*******************************************************************************/
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/*vechile connection control*/
VAR(DoIPVehicleConnCtrlType, DOIP_VAR_INIT) DoIP_VehicleConnCtrl;
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/

#define DOIP_START_SEC_VAR_INIT_BOOLEAN
#include "DoIP_MemMap.h"
/*initialization status*/
VAR(DOIP_InitStatusType, DOIP_VAR_INIT) DoIP_InitStatusRunTime = DOIP_UNINIT;
#define DOIP_STOP_SEC_VAR_INIT_BOOLEAN
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/*udp connection status*/
VAR(DoIP_UdpConnStatusType, DOIP_VAR_CLEARED) DoIP_UdpConnStatus[DOIP_UDP_MAX_CONNECTIONS];
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/*tcp connection status*/
VAR(DoIP_TcpConnStatusType, DOIP_VAR_CLEARED) DoIP_TcpConnStatus[DOIP_MAX_TCP_CONNECTIONS];
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/*vehicle announcement connection status*/
VAR(DoIP_VehicleAncConnStatusType, DOIP_VAR_CLEARED)
DoIP_AncConnStatus[DOIP_UDP_VEHICLEANNOUNCEMENT_CONNECTIONS]; /* PRQA S 1504 */ /* MISRA Rule 8.7 */
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/*tcp internal buffer*/
VAR(DoIP_TcpInnerBufferType, DOIP_VAR_CLEARED) DoIP_TcpInnerBuf[DOIP_MAX_TCP_CONNECTIONS];
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)

#define DOIP_START_SEC_VAR_INIT_8
#include "DoIP_MemMap.h"
/*current connection tcp socket number*/
VAR(uint8, DOIP_VAR_INIT) DoIP_CurrentTcpSocketNbr = 0u;
#define DOIP_STOP_SEC_VAR_INIT_8
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/*pengding routing active message*/
VAR(PendingRoutingActiveType, DOIP_VAR_CLEARED) DoIP_PendingRoutMsg; /* PRQA S 1504 */ /* MISRA Rule 8.7 */
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

#define DOIP_START_SEC_VAR_INIT_BOOLEAN
#include "DoIP_MemMap.h"
/*GID synchronization status */
VAR(boolean, DOIP_VAR_INIT) DoIP_GidSynStatus = FALSE; /* PRQA S 1533 */ /* MISRA Rule 8.9 */
#define DOIP_STOP_SEC_VAR_INIT_BOOLEAN
#include "DoIP_MemMap.h"
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
/*******************************************************************************
**                      Private Functions Declarations                        **
*******************************************************************************/
DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_DiagMsgHandleCfReceiving(
    DoIP_RoleType role,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
    P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_VAR) receiveHandle);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_DiagMsgRxHandler(
    DoIP_RoleType role,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
    P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_VAR) receiveHandle);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

DOIP_LOCAL_INLINE FUNC(DoIP_RoleType, DOIP_CODE) DoIP_GetRoleByCfgTcpConnIdx(uint8 index);

DOIP_LOCAL_INLINE FUNC(DoIP_RoleType, DOIP_CODE) DoIP_GetRoleByCfgUdpConnIdx(uint8 index);

DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetTcpConnBySocketId(SoAd_SoConIdType socketId, uint8* index);

DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_HeaderEvaluate(DoIP_RoleType role, VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx);

DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_PendingNonDiagEnqueue(PendingTxQueueNonDiagMsgType* queueMsg, PduIdType soAdTxPduId, const PduInfoType* info);

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
DOIP_LOCAL_INLINE FUNC(P2VAR(TxPendingNonDiagQueueType, AUTOMATIC, DOIP_VAR), DOIP_CODE)
    DoIP_PendingNonDiagDequeue(P2VAR(PendingTxQueueNonDiagMsgType, AUTOMATIC, DOIP_VAR) queueMsg);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE) DoIP_RegisterSocket(
    VAR(uint8, AUTOMATIC) index,
    VAR(SoAd_SoConIdType, AUTOMATIC) socketId,
    VAR(uint16, AUTOMATIC) sa,
    VAR(uint16, AUTOMATIC) ta);

#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendEntityStatusRsp(PduIdType txPduId);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendRoutineActiveRsp(
    VAR(uint16, AUTOMATIC) txPduId,
    VAR(uint16, AUTOMATIC) sa,
    VAR(uint8, AUTOMATIC) routingActivationResponseCode,
    VAR(uint8, AUTOMATIC) bufIdx,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseData,
    VAR(uint8, AUTOMATIC) responseDataLen);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendAliveCheckReq(VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendPowerModeInfoRsp(VAR(PduIdType, AUTOMATIC) TxPduId);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandlePowerModeReq(
    VAR(PduIdType, AUTOMATIC) TxPduId,
    SoAd_SoConIdType SoConId,
    VAR(uint32, AUTOMATIC) payloadlength);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleEntityStatusReq(
    VAR(PduIdType, AUTOMATIC) TxPduId,
    SoAd_SoConIdType SoConId,
    VAR(uint32, AUTOMATIC) payloadlength);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleVehicleIdReq(
    VAR(PduIdType, AUTOMATIC) TxPduId,
    SoAd_SoConIdType SoConId,
    VAR(uint32, AUTOMATIC) payloadlength,
    P2CONST(uint8, AUTOMATIC, DOIP_VAR) Buffer,
    uint16 Type);

DOIP_LOCAL FUNC(void, DOIP_CODE)
    DoIP_HandleRoutingActiveReq(VAR(PduIdType, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleAliveCheckRsp(VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendDiagnosticAck(VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendDiagnosticNack(
    VAR(uint16, AUTOMATIC) txPduId,
    VAR(uint8, AUTOMATIC) bufIdx,
    VAR(uint8, AUTOMATIC) NackCode);

DOIP_LOCAL FUNC(void, DOIP_CODE)
    DoIP_SendTcpNack(VAR(PduIdType, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) NackCode, VAR(uint8, AUTOMATIC) bufIdx);

DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_SaCheck(VAR(uint16, AUTOMATIC) sa);

DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_ActivationTypeCheck(VAR(uint8, AUTOMATIC) activationType);

DOIP_LOCAL FUNC(DoIP_SocketAssignmentResultType, DOIP_CODE) DoIP_SocketHandle(
    VAR(SoAd_SoConIdType, AUTOMATIC) socketId,
    VAR(uint16, AUTOMATIC) sa,
    VAR(uint16, AUTOMATIC) ta,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseCode);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendSingleSaAliveCheck(VAR(uint16, AUTOMATIC) sa);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendAllSocketsAliveCheck(void);

DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_IsAuthenticatioRequiredCheck(
    VAR(uint8, AUTOMATIC) activationType,
    P2CONST(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_CONST) buffer,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseData,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseDataLen);

DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_IsConfirmationRequiredCheck(
    VAR(uint8, AUTOMATIC) activationType,
    P2CONST(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_CONST) buffer,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseData,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseDataLen);

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_HandleAlivecheckTimeOut(VAR(uint8, AUTOMATIC) index);
#endif /* DOIP_ROLE_SERVER_SUPPORT == TRUE */

DOIP_LOCAL_INLINE FUNC(uint8, DOIP_CODE) DoIP_DataCompare(
    P2CONST(uint8, AUTOMATIC, DOIP_VAR) data1,
    P2CONST(uint8, AUTOMATIC, DOIP_VAR) data2,
    VAR(uint32, AUTOMATIC) len);

#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE)
    DoIP_HandleAliveCheckReq(uint8 vehicleId, VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx);

DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_SendAliveCheckRsp(VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx, uint16 SourceAddr);
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
/*******************************************************************************
**                      Internal Functions                                    **
*******************************************************************************/
DOIP_LOCAL_INLINE PduLengthType DOIP_MIN(PduLengthType x, PduLengthType y)
{
    return (((x) < (y)) ? (x) : (y));
}

FUNC(BufReq_ReturnType, DOIP_CODE)
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
DoIP_DiagMsgRxToPduR(
    PduIdType pdurRxId,
    const PduInfoType* info,
    PduLengthType* bufferSizePtr,
    DoIP_RoleType role,
    P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_CONST) receiveHandle,
    uint8* nackCode)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    BufReq_ReturnType ret;
    uint16 txpduId = receiveHandle->txpduId;
    uint8 bufIdx = receiveHandle->bufIdx;
    RxChannelCtrlType* rxCtrl = &receiveHandle->buffer->rxCtrl;
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
    uint8 index;
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/

    ret = PduR_DoIPTpCopyRxData(pdurRxId, info, bufferSizePtr);
    if (BUFREQ_OK == ret)
    {
        rxCtrl->BS = *bufferSizePtr;
        rxCtrl->rxDataRemain = rxCtrl->payloadLen - (rxCtrl->rxBufPos - (PduLengthType)DOIP_HEADER_LEN);

        /*SWS_DoIP_00259*/
        rxCtrl->rxBufRemain = DOIP_MIN(rxCtrl->rxDataRemain, *bufferSizePtr);
        *bufferSizePtr = rxCtrl->rxBufRemain;
        if (rxCtrl->rxDataRemain == 0u) /*rx done*/
        {
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
            /*0x8002 Send diagnostic message positive ack*/
            if ((DOIP_ROLE_SERVER & role) != 0u)
            {
                DoIP_SendDiagnosticAck(txpduId, bufIdx);
            }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
            if ((DOIP_ROLE_GATEWAY_IN & role) != 0u)
            {
                for (index = 0u; index < DOIP_MAX_TCP_CONNECTIONS; index++)
                {
                    DoIP_TcpConnStatusType* tcpConnStatus = &DoIP_TcpConnStatus[index];
                    if ((bufIdx == tcpConnStatus->bufferId) && (tcpConnStatus->gwCtrl.waitRspCfg != NULL_PTR))
                    {
                        tcpConnStatus->gwCtrl.waitRmtRsp = TRUE;
                        break;
                    }
                }
            }
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
            /*all data has been received*/
            /*SWS_DoIP_00221*/
            PduR_DoIPTpRxIndication(pdurRxId, E_OK);
            DoIP_ResetTcpInnerRxBuf(bufIdx);
        }
        else /*CF will be received in the next receiption*/
        {
            rxCtrl->procStatus = DOIP_RX_PROCESS_CF_RECEIVING;
        }
    }
    /*SWS_DoIP_00217*/
    else
    {
        /*SWS_DoIP_00174*/
        *nackCode = DOIP_DIAG_NACK_CODE_TRANSPORT_PROTOCOL_ERROR;
        PduR_DoIPTpRxIndication(pdurRxId, E_NOT_OK);
    }

    return ret;
}

FUNC(BufReq_ReturnType, DOIP_CODE)
/* PRQA S 3432,1505++ */ /* MISRA Rule 20.7,8.7 */
DoIP_DiagMsgHandleHeaderReceived(
    DoIP_RoleType role,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
    P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_VAR) receiveHandle)
/* PRQA S 3432,1505-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 i, j;
    uint8 index;
    PduLengthType Offset;
    PduInfoType pduInfo;
    uint16 sa;
    uint16 ta;
    PduIdType pdurRxId;
    DoIP_TcpInnerBufferType* rxBuffer = receiveHandle->buffer;
    uint16 txpduId = receiveHandle->txpduId;
    uint8 bufIdx = receiveHandle->bufIdx;
    boolean saRegested = FALSE;
    boolean taIsFound = FALSE;
    SoAd_SoConIdType SoConId = receiveHandle->SoConId;
    BufReq_ReturnType ret = BUFREQ_OK;
    BufReq_ReturnType upperlayer_ret;
    uint8 metaData[4];
    uint8 nackCode = DOIP_DIAG_NACK_CODE_NONE; /*Note:this code is reserved by 13400-2*/
    const DoIPRoutingActivationType* routineActivation;
    const DoIPChannelType* doipChannel;
    uint8* rxChannel = rxBuffer->rxChannel;
    RxChannelCtrlType* rxCtrl = &rxBuffer->rxCtrl;
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
    DoIP_TcpConnStatusType* tcpConnStatus;
#else
    const DoIP_TcpConnStatusType* tcpConnStatus;
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/

    Offset = info->SduLength + rxCtrl->rxBufPos;

    if (Offset > (PduLengthType)(DOIP_HEADER_LEN + DOIP_SA_TA_LEN))
    {
        /*copy sa ta to internal buffer*/
        DoIP_MemCpy(info->SduDataPtr, &rxChannel[rxCtrl->rxBufPos], info->SduLength);
        rxCtrl->rxBufPos += info->SduLength;
        /*check if the Routing was already activated*/
        if (((DOIP_ROLE_GATEWAY_OUT & role) != 0u) || ((DOIP_ROLE_CLIENT & role) != 0u))
        {
            ta = DOIP_U8_2_U16(&rxChannel[8]);
            sa = DOIP_U8_2_U16(&rxChannel[8 + 2]);
        }
        else
        {
            sa = DOIP_U8_2_U16(&rxChannel[8]);
            ta = DOIP_U8_2_U16(&rxChannel[8 + 2]);
        }

        if ((E_OK == DoIP_GetTcpConnIdxBySa(sa, &i))
            && (DOIP_SOCKET_CONNECTION_REGISTERED == DoIP_TcpConnStatus[i].routActiveState))
        {
            saRegested = TRUE;
            index = i;
            tcpConnStatus = &DoIP_TcpConnStatus[index];
        }
        if (TRUE == saRegested)
        {
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
            if ((DOIP_ROLE_SERVER & role) != 0u)
            {
                for (i = 0u; i < DoIP_PBCfgPtr->DoIPRoutingActivationNbr; i++)
                {
                    routineActivation = &DoIP_PBCfgPtr->DoIPRoutingActivation[i];
                    if (routineActivation->DoIPRoutingActivationNumber == tcpConnStatus->activationType)
                    {
                        for (j = 0u; j < routineActivation->DoIPTargetAddressRefNumber; j++)
                        {
                            if (ta == routineActivation->DoIPTargetAddressRef[j].DoIPTargetAddressValue)
                            {
                                taIsFound = TRUE;
                                break;
                            }
                        }
                    }
                }
                if (FALSE == taIsFound)
                {
                    nackCode = DOIP_DIAG_NACK_CODE_UNKNOWN_TARGET_ADDR;
                }
            }
            else
            {
                taIsFound = TRUE;
            }
            if (TRUE == taIsFound)
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
            {
                taIsFound = FALSE;
                for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
                {
                    doipChannel = &DoIP_PBCfgPtr->DoIPChannel[i];
                    if ((ta == tcpConnStatus->ta)
                        || ((doipChannel->DoIPChannelSARef->DoIPTesterSA == sa)
                            && (NULL_PTR == doipChannel->DoIPPduRTxPdu)
                            && (ta == doipChannel->DoIPChannelTARef->DoIPTargetAddressValue)))
                    {
                        taIsFound = TRUE;
                        break;
                    }
                    else
                    {
                        if ((ta != tcpConnStatus->ta) && (doipChannel->DoIPChannelSARef->DoIPTesterSA == sa)
                            && (ta == doipChannel->DoIPChannelTARef->DoIPTargetAddressValue))
                        {
                            taIsFound = TRUE;
                            break;
                        }
                    }
                }
                if (FALSE == taIsFound)
                {
                    /*SWS_DoIP_00127*/
                    nackCode = DOIP_DIAG_NACK_CODE_TARGET_UNREACHABLE;
                }
                else
                {
                    /* Based on req, payloadDataLength(length-4) is compared with MaxBytes-4, */
                    /*here we directly compare them without subtraction */
                    /*SWS_DoIP_00125*/
                    if (rxCtrl->payloadLen > (PduLengthType)(DOIP_MAX_REQUEST_BYTES))
                    {
                        nackCode = DOIP_DIAG_NACK_CODE_MSG_TOO_LARGE;
                    }
                    else
                    {
                        if (E_NOT_OK == DoIP_PduRRxIdExtract(sa, ta, &pdurRxId))
                        {
                            ret = BUFREQ_E_NOT_OK;
                            DoIP_ResetTcpInnerRxBuf(bufIdx);
                        }
                        else
                        {
                            rxCtrl->sa = sa;
                            rxCtrl->ta = ta;
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
                            DoIP_MemCpy(&rxChannel[8], &metaData[0], 4u);
#else
                            metaData[0] = rxChannel[9];
                            metaData[1] = rxChannel[8];
                            metaData[2] = rxChannel[11];
                            metaData[3] = rxChannel[10];
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
                            if (E_OK == DoIP_ExtractChannelByPduRRxPduId(pdurRxId, &i))
                            {
                                doipChannel = &DoIP_PBCfgPtr->DoIPChannel[i];
                                role = doipChannel->DoIPRole; /*If it is a gateway channel,
                                                                                add the gateway attribute to the role.*/
                                if ((DOIP_ROLE_GATEWAY_IN & role) != 0u)
                                {
                                    metaData[0] = (uint8)doipChannel->DoIPChannelMappingSARef->DoIPTesterSA;
                                    metaData[1] = (uint8)(doipChannel->DoIPChannelMappingSARef->DoIPTesterSA >> 8u);
                                    tcpConnStatus->gwCtrl.waitRspCfg = doipChannel->DoIPGatewayWaitResponse;
                                    tcpConnStatus->gwCtrl.lastSid = rxChannel[DOIP_SA_TA_LEN + DOIP_HEADER_LEN];
                                }
                                else if ((DOIP_ROLE_GATEWAY_OUT & role) != 0u)
                                {
                                    metaData[0] = rxChannel[11];
                                    metaData[1] = rxChannel[10];
                                    metaData[2] = rxChannel[9];
                                    metaData[3] = rxChannel[8];
                                }
                                else
                                {
                                    /*do nothing*/
                                }
                            }
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
#endif /*HIGH_BYTE_FIRST == CPU_BYTE_ORDER*/

                            pduInfo.MetaDataPtr = &metaData[0];
                            rxCtrl->pdurRxPduIdRef = pdurRxId;
                            pduInfo.SduLength = 0u;
                            upperlayer_ret = PduR_DoIPTpStartOfReception(
                                pdurRxId,
                                &pduInfo,
                                rxCtrl->payloadLen - DOIP_SA_TA_LEN,
                                bufferSizePtr);
                            if (BUFREQ_OK == upperlayer_ret)
                            {
                                pduInfo.SduLength =
                                    rxCtrl->rxBufPos - (PduLengthType)DOIP_HEADER_LEN - (PduLengthType)DOIP_SA_TA_LEN;
                                pduInfo.SduDataPtr = &rxChannel[DOIP_SA_TA_LEN + DOIP_HEADER_LEN];
                                ret = DoIP_DiagMsgRxToPduR(
                                    pdurRxId,
                                    &pduInfo,
                                    bufferSizePtr,
                                    role,
                                    receiveHandle,
                                    &nackCode);
                            }
                            /*req 13400 DoIP_073*/
                            else if (BUFREQ_E_OVFL == upperlayer_ret)
                            {
                                nackCode = DOIP_DIAG_NACK_CODE_OUT_OF_MEMORY;
                            }
                            /*SWS_DoIP_00216*/
                            else
                            {
                                nackCode = DOIP_DIAG_NACK_CODE_TRANSPORT_PROTOCOL_ERROR;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            /*SWS_DoIP_00123*/
            nackCode = DOIP_DIAG_NACK_CODE_INVALID_SOURCE_ADDR;
            if (E_OK == DoIP_GetTcpConnBySocketId(SoConId, &i))
            {
                DoIP_TcpConnStatus[i].wait4SocketClose = TRUE;
            }
        }
    }
    else /*Offset <= (PduLengthType)(DOIP_HEADER_LEN + DOIP_SA_TA_LEN), incomplete sa&ta received*/
    {
        DoIP_MemCpy(info->SduDataPtr, (uint8*)&rxChannel[rxCtrl->rxBufPos], info->SduLength);
        rxCtrl->rxBufPos += info->SduLength;
        rxCtrl->rxBufRemain = rxCtrl->payloadLen - (rxCtrl->rxBufPos - (PduLengthType)DOIP_HEADER_LEN);

        *bufferSizePtr = rxCtrl->rxBufRemain;
        ret = BUFREQ_OK;
        /*procStatus not change in this case*/
    }

    if (DOIP_DIAG_NACK_CODE_NONE != nackCode)
    {
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        if ((DOIP_ROLE_SERVER & role) != 0u)
        {

            DoIP_SendDiagnosticNack(txpduId, bufIdx, nackCode);
        }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
        rxCtrl->procStatus = DOIP_RX_PROCESS_DISCARD;
    }

    return ret;
}

DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE)
    /* PRQA S 3432,1505++ */ /* MISRA Rule 20.7,8.7 */
    DoIP_DiagMsgHandleCfReceiving(
        DoIP_RoleType role,
        P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
        P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
        P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_VAR) receiveHandle)
/* PRQA S 3432,1505-- */ /* MISRA Rule 20.7,8.7 */
{
    PduIdType pdurRxId;
    RxChannelCtrlType* rxCtrl = &(receiveHandle->buffer->rxCtrl);
    uint16 txpduId = receiveHandle->txpduId;
    uint8 bufIdx = receiveHandle->bufIdx;
    BufReq_ReturnType ret;
    uint8 nackCode = DOIP_DIAG_NACK_CODE_NONE; /*Note:this code is reserved by 13400-2*/

    pdurRxId = rxCtrl->pdurRxPduIdRef;
    /*SWS_DoIP_00253*/
    if (info->SduLength > rxCtrl->BS)
    {
        PduR_DoIPTpRxIndication(pdurRxId, E_NOT_OK);
        DoIP_ResetTcpInnerRxBuf(bufIdx);
        ret = BUFREQ_E_NOT_OK;
    }
    else
    {
        rxCtrl->rxBufPos += info->SduLength;
        ret = DoIP_DiagMsgRxToPduR(pdurRxId, info, bufferSizePtr, role, receiveHandle, &nackCode);
    }

    if (DOIP_DIAG_NACK_CODE_NONE != nackCode)
    {
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        if ((DOIP_ROLE_SERVER & role) != 0u)
        {
            DoIP_SendDiagnosticNack(txpduId, bufIdx, nackCode);
        }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
        rxCtrl->procStatus = DOIP_RX_PROCESS_DISCARD;
    }
    return ret;
}

DOIP_LOCAL FUNC(BufReq_ReturnType, DOIP_CODE)
    /* PRQA S 3432,1505++ */ /* MISRA Rule 20.7,8.7 */
    DoIP_DiagMsgRxHandler(
        DoIP_RoleType role,
        P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
        P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
        P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_VAR) receiveHandle)
/* PRQA S 3432,1505-- */ /* MISRA Rule 20.7,8.7 */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
    const RxChannelCtrlType* rxCtrl = &(receiveHandle->buffer->rxCtrl);

    if (rxCtrl->procStatus == DOIP_RX_PROCESS_HEADER_RECEIVED)
    {
        ret = DoIP_DiagMsgHandleHeaderReceived(role, info, bufferSizePtr, receiveHandle);
    }
    else if (rxCtrl->procStatus == DOIP_RX_PROCESS_CF_RECEIVING)
    {
        ret = DoIP_DiagMsgHandleCfReceiving(role, info, bufferSizePtr, receiveHandle);
    }
    else
    {
        /*do nothing*/
    }
    return ret;
}

FUNC(void, DOIP_CODE) DoIP_TcpSocketTimeoutHandler(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 i;
    SoAd_SoConIdType socketId;
    DoIP_TcpConnStatusType* tcpConnStatus;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        SchM_Enter_DoIP_ExclusiveArea();
        tcpConnStatus = &DoIP_TcpConnStatus[i];
        if ((tcpConnStatus->soConnState == SOAD_SOCON_ONLINE)
            && (tcpConnStatus->routActiveState != DOIP_SOCKET_CONNECTION_REGISTERED))
        {
            if (tcpConnStatus->initInactiveTimer >= DOIP_INITIAL_INACTIVE_TIME)
            {
                /*SWS_DoIP_00144*/
                socketId = tcpConnStatus->socketId;
                DoIP_CloseTcpConnection(tcpConnStatus->socketId);
                (void)SoAd_CloseSoCon(socketId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
                (void)SoAd_OpenSoCon(socketId);
#endif
            }
            else
            {
                tcpConnStatus->initInactiveTimer += DOIP_MAIN_FUNCTION_PERIOD;
            }
        }
        else if (
            (tcpConnStatus->soConnState == SOAD_SOCON_ONLINE)
            && (tcpConnStatus->routActiveState == DOIP_SOCKET_CONNECTION_REGISTERED))
        {
            /*SWS_DoIP_00146*/
            if (tcpConnStatus->genInactiveTimer >= DOIP_GENERAL_INACTIVE_TIME)
            {
                socketId = tcpConnStatus->socketId;
                DoIP_CloseTcpConnection(socketId);
                (void)SoAd_CloseSoCon(socketId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
                (void)SoAd_OpenSoCon(socketId);
#endif
            }
            else
            {
                tcpConnStatus->genInactiveTimer += DOIP_MAIN_FUNCTION_PERIOD;
            }
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
            /*handle alive check timer*/
            if (tcpConnStatus->wait4AliveChkRsp == TRUE)
            {
                if (tcpConnStatus->aliveChkTimer >= DOIP_ALIVE_CHECK_RESPONSE_TIMEOUT)
                {
                    DoIP_HandleAlivecheckTimeOut(i);
                }
                else
                {
                    tcpConnStatus->aliveChkTimer += DOIP_MAIN_FUNCTION_PERIOD;
                }
            }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
        }
        else
        {
            /*nothing should do*/
        }
        SchM_Exit_DoIP_ExclusiveArea();
    }
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_TpTransmitInternal(
    VAR(PduIdType, AUTOMATIC) DoIPPduRTxId,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) DoIPPduRTxInfoPtr)
{
    PduIdType soAdTxPduId;
    PduInfoType pduInfo;
    uint8 bufferId;
    uint16 sa;
    uint16 ta;
    uint8 index;
    uint16 doIPPduRTxPduRef;
    TxChannelCtrlType* txCtrl;
    Std_ReturnType ret = DoIP_GetPduRTxPduRefByTxId(DoIPPduRTxId, &doIPPduRTxPduRef, DOIP_TPPDU);

    if ((E_OK == DoIP_TransmitBufIdExtrac(DoIPPduRTxId, DoIPPduRTxInfoPtr, &bufferId)) && (E_OK == ret))
    {
        pduInfo.SduDataPtr = NULL_PTR;
        pduInfo.SduLength = DOIP_HEADER_LEN + DOIP_SA_TA_LEN + DoIPPduRTxInfoPtr->SduLength;
        ret = DoIP_PdurSaTaExtract(DoIPPduRTxId, DoIPPduRTxInfoPtr, &sa, &ta);
        txCtrl = &(DoIP_TcpInnerBuf[bufferId].txCtrl);

        if (E_OK == ret)
        {
            /*SWS_DoIP_00230*/
            if (E_OK == DoIP_SoAdTxIdExtrac(DoIPPduRTxId, DoIPPduRTxInfoPtr, &soAdTxPduId))
            {
                DoIP_DiagnosticHeaderFill(DoIPPduRTxInfoPtr, bufferId, sa, ta);
                /*SWS_DoIP_00220*/
                if (E_OK == DoIP_TriggerTpTransmit(soAdTxPduId, &pduInfo, txCtrl, DOIP_TX_DIAG_TP_MSG))
                {
                    txCtrl->pdurTxPduId = DoIPPduRTxId;
                    txCtrl->pdurTxPduIdRef = doIPPduRTxPduRef;

                    if (E_OK == DoIP_GetTcpConnIdxBySa(ta, &index))
                    {
                        DoIP_TcpConnStatus[index].diagMsgTimer = 0u;
                        DoIP_TcpConnStatus[index].wait4DiagMsgResponse = TRUE;
                    }
                }
                else
                {
                    DoIP_ResetTcpInnerTxBuf(bufferId);
                    /*SWS_DoIP_00223 SWS_DoIP_00228*/
                    /*discard the message*/
                    PduR_DoIPTpTxConfirmation(doIPPduRTxPduRef, E_NOT_OK);
                }
            }
        }
    }

    return ret;
}

FUNC(void, DOIP_CODE) DoIP_PendingDiagMsgHandler(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    uint8 i;
    const TxPendingTpQueueType* TpQueue;
    const TxPendingIfQueueType* IfQueue;
    const TxPendingNonDiagQueueType* NonDiagQueue;
    DoIP_TcpInnerBufferType* pBuffer;
    TxChannelCtrlType* txCtrl;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        pBuffer = &DoIP_TcpInnerBuf[i];
        txCtrl = &pBuffer->txCtrl;
        if ((txCtrl->bufferState == DOIP_BUFFER_IDLE) && (txCtrl->processStatus == DOIP_TX_PROCESS_IDLE))
        {
            /*check tp pengding message*/
            TpQueue = DoIP_PendingTpDequeue(&pBuffer->pendingTxTpQueue);
            if (NULL_PTR != TpQueue)
            {
                if (E_NOT_OK == DoIP_TpTransmitInternal(TpQueue->pendingPdurPduId, &TpQueue->pendingPduInfo))
                {
                    (void)DoIP_PendingTpEnqueue(
                        &pBuffer->pendingTxTpQueue,
                        TpQueue->pendingPdurPduId,
                        &TpQueue->pendingPduInfo);
                }
            }
            else
            {
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
                /*check if pengding message*/
                IfQueue = DoIP_PendingIfDequeue(&pBuffer->pendingTxIfQueue);
                if (NULL_PTR != IfQueue)
                {
                    if (E_NOT_OK == DoIP_IfTransmitInternal(IfQueue->pendingPdurPduId, &IfQueue->pendingPduInfo))
                    {
                        (void)DoIP_PendingIfEnqueue(
                            &pBuffer->pendingTxIfQueue,
                            IfQueue->pendingPdurPduId,
                            &IfQueue->pendingPduInfo);
                    }
                }
                else
#endif            /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
                { /*check non diag pengding message*/
                    NonDiagQueue = DoIP_PendingNonDiagDequeue(&pBuffer->pendingTxNonDiagQueue);
                    if (NULL_PTR != NonDiagQueue)
                    {
                        if (E_OK
                            == DoIP_TriggerTpTransmit(
                                NonDiagQueue->pendingSoAdTxPduId,
                                &NonDiagQueue->pendingPduInfo,
                                txCtrl,
                                DOIP_TX_NON_DIAG_MSG))
                        {
                            DoIP_MemCpy(
                                &NonDiagQueue->pendingPdu.dataPtr[0],
                                &pBuffer->txChannel[0],
                                NonDiagQueue->pendingPduInfo.SduLength);
                        }
                        else
                        {
                            (void)DoIP_PendingNonDiagEnqueue(
                                &pBuffer->pendingTxNonDiagQueue,
                                NonDiagQueue->pendingSoAdTxPduId,
                                &NonDiagQueue->pendingPduInfo);
                        }
                    }
                }
            }
        }
    }
}

FUNC(void, DOIP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
DoIP_SoConModeChgTcpNty(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint8 i;
    SoAd_SoConIdType local_SoConId;
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
    uint8 j;
    uint8 connIdx;
    SoAd_SoConIdType DoIP_Eth2EthSoConId = DOIP_INVALID_UINT8;
    boolean doip_gateway_connected = FALSE;
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/

    DoIP_TcpConnStatusType* tcpConnStatus;

    /*to determin the rxpduid in DoIP_TcpConnStatus*/
    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPAllTcpConnectionNbr; i++)
    {
        tcpConnStatus = &DoIP_TcpConnStatus[i];
        if (E_OK == SoAd_GetSoConId(DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId, &local_SoConId))
        {
            if (local_SoConId == SoConId)
            {
                if (SOAD_SOCON_ONLINE == Mode)
                {
                    tcpConnStatus->soConnState = Mode;
                    tcpConnStatus->initInactiveTimer = 0u;
                    tcpConnStatus->socketId = SoConId;
                    tcpConnStatus->rxPduId = DOIP_TCPCON_CFG(i)->DoIPSoAdRxPdu->DoIPSoAdRxPduId;
                    tcpConnStatus->txPduId = DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdTxPduId;
                    tcpConnStatus->lowerlayerTxPduId = DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
                    DoIP_CurrentTcpSocketNbr++;
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
                    tcpConnStatus->role = DoIP_GetRoleByCfgTcpConnIdx(i);
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
                    if ((DOIP_ROLE_CLIENT | DOIP_ROLE_GATEWAY_OUT) == tcpConnStatus->role)
                    {
                        doip_gateway_connected = TRUE;
                    }
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
                }
                else
                {
                    /*SWS_DoIP_00243*/
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
                    tcpConnStatus->role = DoIP_GetRoleByCfgTcpConnIdx(i);
                    if (((DOIP_ROLE_CLIENT | DOIP_ROLE_GATEWAY_OUT) == tcpConnStatus->role)
                        && (doip_gateway_connected == TRUE))
                    {
                        doip_gateway_connected = FALSE;

                        for (j = 0u; j < DoIP_PBCfgPtr->DoIPChannelNbr; j++)
                        {
                            if ((DOIP_ROLE_GATEWAY_ETH & DoIP_PBCfgPtr->DoIPChannel[j].DoIPRole) != 0u)
                            {
                                if (NULL_PTR != DoIP_PBCfgPtr->DoIPChannel[j].DoIPChannelSARef)
                                {
                                    if (E_OK
                                        == DoIP_GetTcpConnIdxBySa(
                                            DoIP_PBCfgPtr->DoIPChannel[j].DoIPChannelSARef->DoIPTesterSA,
                                            &connIdx))
                                    {
                                        tcpConnStatus = &DoIP_TcpConnStatus[connIdx];
                                        (void)SoAd_CloseSoCon(tcpConnStatus->socketId, TRUE);
                                        DoIP_Eth2EthSoConId = tcpConnStatus->socketId;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    if ((SoConId == DoIP_Eth2EthSoConId) && (SOAD_SOCON_OFFLINE == Mode))
                    {
                        DoIP_Eth2EthSoConId = 0xff;
                        (void)SoAd_OpenSoCon(SoConId);
                    }
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
                    DoIP_CloseTcpConnection(SoConId);
                }
                break;
            }
        }
    }
}

FUNC(void, DOIP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
DoIP_SoConModeChgUdpNty(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint8 i;
    SoAd_SoConIdType local_SoConId;
    DoIP_UdpConnStatusType* udpConnStatus;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPAllUdpConnectionNbr; i++)
    {
        udpConnStatus = &DoIP_UdpConnStatus[i];
        if (E_OK == SoAd_GetSoConId(DOIP_UDPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId, &local_SoConId))
        {
            if (local_SoConId == SoConId)
            {
                if (SOAD_SOCON_ONLINE == Mode)
                {
                    udpConnStatus->txPduId = DOIP_UDPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdTxPduId;
                    udpConnStatus->socketId = SoConId;
                    udpConnStatus->soConState = Mode;
                    udpConnStatus->role = DoIP_GetRoleByCfgUdpConnIdx(i);
                }
                else
                {
                    DoIP_ResetUdpConn(i);
                }
                break;
            }
        }
    }
}

FUNC(void, DOIP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
DoIP_SoConModeChgAncNty(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint8 i;
    SoAd_SoConIdType local_SoConId = 0u;
    const DoIPConnectionsType* DoIPConnections = DoIP_PBCfgPtr->DoIPConnections;
    DoIP_VehicleAncConnStatusType* AncConnStatus;

    for (i = 0u; i < DoIPConnections->DoIPAllVehicleAnnouncementConnectionNbr; i++)
    {
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
        if (i < DoIPConnections->DoIPClientVehicleAnnouncementConnectionNbr)
        {
            local_SoConId = DOIP_ANCCON_CFG(i)->soConId;
        }
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        if (i >= DoIPConnections->DoIPClientVehicleAnnouncementConnectionNbr)
        {
            (void)SoAd_GetSoConId(DOIP_ANCCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId, &local_SoConId);
        }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
        if (local_SoConId == SoConId)
        {
            if (SOAD_SOCON_ONLINE == Mode)
            {
                AncConnStatus = &DoIP_AncConnStatus[i];
                AncConnStatus->socketId = SoConId;
                AncConnStatus->soConState = Mode;
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
                if (i >= DoIPConnections->DoIPClientVehicleAnnouncementConnectionNbr)
                {
                    AncConnStatus->vehicleAncTimerFlag = TRUE;
                    AncConnStatus->vehicleAncTimer = 0u;
                    AncConnStatus->vehicleAncCnt = 0u;
                }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
            }
            else
            {
                DoIP_AncConnStatus[i].socketId = DOIP_INVALID_UINT8;
                DoIP_AncConnStatus[i].soConState = SOAD_SOCON_OFFLINE;
            }
            break;
        }
    }
}

DOIP_LOCAL_INLINE FUNC(DoIP_RoleType, DOIP_CODE)
    DoIP_GetRoleByCfgTcpConnIdx(uint8 index) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    return (index < DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr) ? (DOIP_ROLE_CLIENT)
                                                                                : (DOIP_ROLE_SERVER);
}

DOIP_LOCAL_INLINE FUNC(DoIP_RoleType, DOIP_CODE)
    DoIP_GetRoleByCfgUdpConnIdx(uint8 index) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    return (index < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr) ? (DOIP_ROLE_CLIENT)
                                                                                : (DOIP_ROLE_SERVER);
}

/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
DOIP_LOCAL_INLINE FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetTcpConnBySocketId(SoAd_SoConIdType socketId, uint8* index)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    uint8 ret = E_NOT_OK;
    uint8 i;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if (DoIP_TcpConnStatus[i].socketId == socketId)
        {
            *index = i;
            ret = E_OK;
            break;
        }
    }

    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
DoIP_SoAdTxIdExtrac(
    PduIdType DoIPPduRTxId,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    P2VAR(PduIdType, AUTOMATIC, DOIP_VAR) soadID)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    uint8 i;
    uint16 sa, ta;
    Std_ReturnType ret = E_NOT_OK;
    DoIP_RoleType role;
    const uint8* MetaDataPtr = info->MetaDataPtr;
    const DoIPChannelType* DoIPChannel;

    /*first to get sa*/
    if (NULL_PTR != MetaDataPtr)
    {
        role = DoIP_GetRoleByPduRTxPduId(DoIPPduRTxId);

        if (((role & DOIP_ROLE_GATEWAY_IN) != 0u) || (role == DOIP_ROLE_CLIENT))
        {
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
            sa = DOIP_U8_2_U16(&MetaDataPtr[0]);
            ta = DOIP_U8_2_U16(&MetaDataPtr[2]);
#else
            sa = (((uint16)(MetaDataPtr[1])) << 8u) + (uint16)(MetaDataPtr[0]);
            ta = (((uint16)(MetaDataPtr[3])) << 8u) + (uint16)(MetaDataPtr[2]);
#endif /*HIGH_BYTE_FIRST == CPU_BYTE_ORDER*/
            for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
            {
                DoIPChannel = &DoIP_PBCfgPtr->DoIPChannel[i];
                if ((sa == DoIPChannel->DoIPChannelMappingSARef->DoIPTesterSA)
                    && (ta == DoIPChannel->DoIPChannelTARef->DoIPTargetAddressValue))
                {
                    sa = DoIPChannel->DoIPChannelSARef->DoIPTesterSA;
                    break;
                }
            }
        }
        else if (role == DOIP_ROLE_SERVER)
        {
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
            sa = DOIP_U8_2_U16(&MetaDataPtr[2]);
#else
            sa = (((uint16)(MetaDataPtr[3])) << 8u) + (uint16)(MetaDataPtr[2]);
#endif /*HIGH_BYTE_FIRST == CPU_BYTE_ORDER*/
        }
        else
        {
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
            sa = DOIP_U8_2_U16(&MetaDataPtr[0]);
#else
            sa = (((uint16)(MetaDataPtr[1])) << 8u) + (uint16)(MetaDataPtr[0]);
#endif /* HIGH_BYTE_FIRST == CPU_BYTE_ORDE */
        }
    }
    else
    {
        for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
        {
            P2CONST(DoIPPduRTxPduType, AUTOMATIC, DOIP_VAR) pdurTxPdu = DoIP_PBCfgPtr->DoIPChannel[i].DoIPPduRTxPdu;
            DoIPChannel = &DoIP_PBCfgPtr->DoIPChannel[i];
            if ((pdurTxPdu != NULL_PTR) && (DoIPPduRTxId == pdurTxPdu->DoIPPduRTxPduId))
            {
                sa = DoIPChannel->DoIPChannelSARef->DoIPTesterSA;
                break;
            }
        }
    }

    if (E_OK == DoIP_GetTcpConnIdxBySa(sa, &i))
    {
        *soadID = DoIP_TcpConnStatus[i].lowerlayerTxPduId;
        ret = E_OK;
    }

    return ret;
}

FUNC(void, DOIP_CODE)
DoIP_DiagnosticHeaderFill(
    P2CONST(PduInfoType, AUTOMATIC, DOIP_VAR) info,
    VAR(uint8, AUTOMATIC) bufferId,
    VAR(uint16, AUTOMATIC) sa,
    VAR(uint16, AUTOMATIC) ta)
{
    uint8* txChannel = DoIP_TcpInnerBuf[bufferId].txChannel;

    DoIP_u16_2_u8(&txChannel[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    DoIP_u16_2_u8(&txChannel[2], DOIP_DIAG_MSG);
    DoIP_u32_2_u8(&txChannel[4], (PduLengthType)(DOIP_SA_TA_LEN + info->SduLength));
    DoIP_u16_2_u8(&txChannel[8], sa);
    DoIP_u16_2_u8(&txChannel[10], ta);
}

FUNC(boolean, DOIP_CODE) DoIP_SocketConnectedCheckFrmPdurId(VAR(PduIdType, AUTOMATIC) DoIPPduRTxId)
{
    uint16 sa;
    uint16 i;
    uint8 j;
    boolean ret = FALSE;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
    {
        if ((NULL_PTR != DoIP_PBCfgPtr->DoIPChannel[i].DoIPPduRTxPdu)
            && (DoIP_PBCfgPtr->DoIPChannel[i].DoIPPduRTxPdu->DoIPPduRTxPduId == DoIPPduRTxId))
        {
            sa = DoIP_PBCfgPtr->DoIPChannel[i].DoIPChannelSARef->DoIPTesterSA;
            if ((E_OK == DoIP_GetTcpConnIdxBySa(sa, &j)) && (SOAD_SOCON_ONLINE == DoIP_TcpConnStatus[j].soConnState))
            {
                ret = TRUE;
            }
            break;
        }
    }

    return ret;
}

FUNC(void, DOIP_CODE) DoIP_ResetTcpInnerBuf(VAR(uint8, AUTOMATIC) bufferId)
{
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufferId];
    PendingTxQueueTpMsgType* pendingTxTpQueue = &pBuffer->pendingTxTpQueue;

    DoIP_ResetTcpInnerRxBuf(bufferId);
    pBuffer->rxCtrl.doipSoadRxPduId = DOIP_INVALID_UINT16;
    pBuffer->rxCtrl.procStatus = DOIP_RX_PROCESS_IDLE;
    pendingTxTpQueue->tail = 0u;
    pendingTxTpQueue->head = 0u;
    DoIP_ResetTcpInnerTxBuf(bufferId);
    /*aaa, may cant reset queue,because it's not buffer related*/
    DoIP_ResetPendingTxTpQueue(pendingTxTpQueue->pendingQueue);
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
    DoIP_ResetPendingTxIfQueue(pBuffer->pendingTxIfQueue.pendingQueue);
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
    DoIP_ResetPendingTxNonDiagQueue(pBuffer->pendingTxNonDiagQueue.pendingQueue);
}

FUNC(void, DOIP_CODE) DoIP_CloseTcpConnection(VAR(SoAd_SoConIdType, AUTOMATIC) socketId)
{
    uint8 i;
    uint8 j;
    PduIdType rxPduId;
    PduIdType pdurTxId;
    TxPendingTpQueueType const* tempQueue;
    DoIP_TcpInnerBufferType* pBuffer;
    const DoIPPduRTxPduType* DoIPPduRTxPdu;
    /*reset Tcp socket connection*/
    if (E_OK == DoIP_GetTcpConnBySocketId(socketId, &i))
    {
        rxPduId = DoIP_TcpConnStatus[i].rxPduId;
        DoIP_ResetTcpConn(i);
        /*reducing the amount of tester connected by 1*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        DoIP_CurrentTcpSocketNbr--;
#endif
        /*reset Tcp internal buffer*/
        if (E_OK == DoIP_GetTcpBufIdxByRxPduId(rxPduId, &i))
        {
            /*colse shall notify DCM to reset resource*/
            pBuffer = &DoIP_TcpInnerBuf[i];
            pdurTxId = pBuffer->txCtrl.pdurTxPduIdRef;
            if (pdurTxId == DOIP_INVALID_UINT16)
            {
                /*check whether this is the pending message*/
                tempQueue = DoIP_PendingTpDequeue(&pBuffer->pendingTxTpQueue);
                while (tempQueue != NULL_PTR)
                {
                    for (j = 0u; j < DoIP_PBCfgPtr->DoIPChannelNbr; j++)
                    {
                        DoIPPduRTxPdu = DoIP_PBCfgPtr->DoIPChannel[j].DoIPPduRTxPdu;
                        if (NULL_PTR != DoIPPduRTxPdu)
                        {
                            if ((DoIPPduRTxPdu->DoIPPduRTxPduId == tempQueue->pendingPdurPduId)
                                && (DoIPPduRTxPdu->DoIPPduType == DOIP_TPPDU))
                            {
                                pdurTxId = DoIPPduRTxPdu->DoIPPduRTxPduRef;
                                break;
                            }
                        }
                    }
                    if (pdurTxId != DOIP_INVALID_UINT16)
                    {
                        PduR_DoIPTpTxConfirmation(pdurTxId, E_NOT_OK);
                    }
                    tempQueue = DoIP_PendingTpDequeue(&pBuffer->pendingTxTpQueue);
                }
            }
            else
            {
                PduR_DoIPTpTxConfirmation(pdurTxId, E_NOT_OK);
            }
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
            /*Handle the pending routing activation if a connection is closed.*/
            /*todo, check role*/
            if (DoIP_PendingRoutMsg.pendingFlag == TRUE)
            {
                DoIP_PendingRoutMsg.pendingFlag = FALSE;
                DoIP_HandleRoutingActiveReq(DoIP_PendingRoutMsg.pendingTxPduId, DoIP_PendingRoutMsg.pendingBufferIndex);
                /*Reset this pending activation message, if it is successfully processed*/
                if (DoIP_PendingRoutMsg.pendingFlag == FALSE)
                {
                    DoIP_ResetPendingRoutingActiveMsg();
                }
            }
#endif
            DoIP_ResetTcpInnerBuf(i);
        }
    }
}

DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_HeaderEvaluate(DoIP_RoleType role, VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    SoAd_SoConIdType SoConId;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];
    Std_ReturnType ret = E_OK;
    uint8 index;
    const uint8* rxChannel = pBuffer->rxChannel;
    RxChannelCtrlType* rxCtrl = &pBuffer->rxCtrl;

    (void)SoAd_GetSoConId(txPduId, &SoConId);
    /*Check Generic DoIP header synchronization pattern*/
    if (DOIP_GENERICHEADER_CHECK(&rxChannel[0], rxCtrl->rxBufPos))
    {
        rxCtrl->payloadType = DOIP_U8_2_U16(&rxChannel[2]);
        rxCtrl->payloadLen = (PduLengthType)DOIP_U8_2_U32(&rxChannel[4]);

#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        if ((DOIP_ROLE_SERVER & role) != 0u)
        {
            /*The 0x03 error code should have been checked here, but ignored because of the code structure design. */
            if (rxCtrl->payloadLen > DOIP_MAX_REQUEST_BYTES)
            {
                /*SWS_DoIP_00017*/
                DoIP_SendTcpNack(txPduId, DOIP_E_MESSAGE_TO_LARGE, bufIdx);
                ret = E_NOT_OK;
            }
            else
            {
                ret = DoIP_ServerHeaderEvaluateWithPayloadType(txPduId, bufIdx, SoConId);
            }
        }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
        if ((DOIP_ROLE_CLIENT & role) != 0u)
        {
            if (((rxCtrl->payloadType != (uint16)DOIP_DIAG_MSG_ACK)
                 && (rxCtrl->payloadType != (uint16)DOIP_DIAG_MSG_NACK) && (rxCtrl->payloadLen >= DOIP_RX_BUFFER_SIZE))
                || (rxCtrl->payloadLen >= DOIP_MAX_REQUEST_BYTES))
            {
                ret = E_NOT_OK;
            }
            else
            {
                ret = DoIP_ClientHeaderEvaluateWithPayloadType(bufIdx, SoConId);
            }
        }
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
    }
    else
    {
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        if ((DOIP_ROLE_SERVER & role) != 0u)
        {
            /*SWS_DoIP_00014 Generic DoIP header synchronization pattern Check failed  */
            DoIP_SendTcpNack(txPduId, DOIP_E_INCORRECT_PATTERN_FORMAT, bufIdx);
        }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
        if (E_OK == DoIP_GetTcpConnBySocketId(SoConId, &index))
        {
            DoIP_TcpConnStatus[index].wait4SocketClose = TRUE;
        }

        ret = E_NOT_OK;
    }
    return ret;
}

FUNC(void, DOIP_CODE)
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
DoIP_TcpNonDiagMsgHandler(
    DoIP_RoleType role,
    VAR(PduIdType, AUTOMATIC) txPduId,
    VAR(uint8, AUTOMATIC) bufIdx,
    uint16 payloadType)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    uint8 i;
    uint8 vehicleId = 0u;

    if ((DOIP_ROLE_CLIENT & role) != 0u)
    {
        for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr; i++)
        {
            if (txPduId == DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId)
            {
                vehicleId = DOIP_TCPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId;
                break;
            }
        }
    }
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
    switch (payloadType)
    {
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    case DOIP_ALIVE_CHECK_REQ:
        DoIP_HandleAliveCheckReq(vehicleId, txPduId, bufIdx);
        break;
    case DOIP_ROUTING_ACTIVATION_RSP:
        DoIP_HandleRoutingActiveRsp(vehicleId, txPduId, bufIdx);
        DoIP_ResetTcpInnerRxBuf(bufIdx);
        break;
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
    case DOIP_ALIVE_CHECK_RSP:
        DoIP_HandleAliveCheckRsp(txPduId, bufIdx);
        break;
    case DOIP_ROUTING_ACTIVATION_REQ:
        DoIP_HandleRoutingActiveReq(txPduId, bufIdx);
        break;
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
    default:
        /*do nothing*/
        break;
    }
    (void)role;
}

FUNC(Std_ReturnType, DOIP_CODE)
/* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
DoIP_PdurSaTaExtract(
    VAR(PduIdType, AUTOMATIC) pdurid,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    P2VAR(uint16, AUTOMATIC, DOIP_VAR) sa,
    P2VAR(uint16, AUTOMATIC, DOIP_VAR) ta)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    uint16 i;
    uint16 c_sa;
    uint16 c_ta;
    uint16 r_sa;
    uint16 r_ta;
    uint8 metaDataLength;
    Std_ReturnType ret = E_NOT_OK;
    DoIP_RoleType role;
    const DoIPChannelType* DoIPChannel;
    const DoIPPduRTxPduType* DoIPPduRTxPdu;
    const uint8* MetaDataPtr = info->MetaDataPtr;
    boolean ProcessOn = TRUE;

    for (i = 0u; ((i < DoIP_PBCfgPtr->DoIPChannelNbr) && (ProcessOn == TRUE)); i++)
    {
        DoIPChannel = &(DoIP_PBCfgPtr->DoIPChannel[i]);
        DoIPPduRTxPdu = DoIPChannel->DoIPPduRTxPdu;
        if ((NULL_PTR != DoIPPduRTxPdu) && (DoIPPduRTxPdu->DoIPPduRTxPduId == pdurid))
        {
            if (MetaDataPtr == NULL_PTR)
            {
                *sa = DoIPChannel->DoIPChannelTARef->DoIPTargetAddressValue;
                *ta = DoIPChannel->DoIPChannelSARef->DoIPTesterSA;
                ret = E_OK;
                ProcessOn = FALSE;
            }
            else if (DOIP_TPPDU == DoIPPduRTxPdu->DoIPPduType)
            {
                metaDataLength = DoIPPduRTxPdu->DoIPMetaDataMask & 0x0Fu;
                c_sa = DoIPChannel->DoIPChannelSARef->DoIPTesterSA;
                c_ta = DoIPChannel->DoIPChannelTARef->DoIPTargetAddressValue;
                if (metaDataLength != DOIP_SA_TA_LEN)
                {
                    ProcessOn = FALSE;
                }
                else
                {
                    role = DoIP_GetRoleByPduRTxPduId(pdurid);
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
                    r_sa = DOIP_U8_2_U16(&MetaDataPtr[2]);
                    r_ta = DOIP_U8_2_U16(&MetaDataPtr[0]);
#else
                    if (((role & DOIP_ROLE_GATEWAY_IN) != 0u) || ((role & DOIP_ROLE_GATEWAY_OUT) != 0u)
                        || (role == DOIP_ROLE_CLIENT))
                    {
                        r_ta = (((uint16)(MetaDataPtr[3])) << 8u) + (uint16)(MetaDataPtr[2]);
                        r_sa = (((uint16)(MetaDataPtr[1])) << 8u) + (uint16)(MetaDataPtr[0]);
                    }
                    else
                    {
                        r_ta = (((uint16)(MetaDataPtr[1])) << 8u) + (uint16)(MetaDataPtr[0]);
                        r_sa = (((uint16)(MetaDataPtr[3])) << 8u) + (uint16)(MetaDataPtr[2]);
                    }
#endif /*HIGH_BYTE_FIRST == CPU_BYTE_ORDER*/
                    if ((r_sa != c_sa) || (r_ta != c_ta))
                    {
                        ret = E_NOT_OK;
                        if ((role & DOIP_ROLE_GATEWAY_IN) != 0u)
                        {
                            if (r_sa == DoIPChannel->DoIPChannelMappingSARef->DoIPTesterSA)
                            {
                                r_sa = DoIPChannel->DoIPChannelSARef->DoIPTesterSA;
                                if ((r_sa == c_sa) && (r_ta == c_ta))
                                {
                                    /*exchange sa ta*/
                                    *sa = r_ta;
                                    *ta = r_sa;
                                    ret = E_OK;
                                    ProcessOn = FALSE;
                                }
                            }
                        }
                    }
                    else
                    {
                        if ((role & DOIP_ROLE_GATEWAY_OUT) != 0u)
                        {
                            *sa = r_sa;
                            *ta = r_ta;
                        }
                        else
                        {
                            *sa = r_ta;
                            *ta = r_sa;
                        }

                        ret = E_OK;
                        ProcessOn = FALSE;
                    }
                }
            }
            else
            {
                *sa = DoIPChannel->DoIPChannelSARef->DoIPTesterSA;
                *ta = DoIPChannel->DoIPChannelTARef->DoIPTargetAddressValue;
                ret = E_OK;
                ProcessOn = FALSE;
            }
        }
    }

    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_TransmitBufIdExtrac(
    VAR(PduIdType, AUTOMATIC) pdurid,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) bufferId) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    uint8 i;
    uint16 sa, ta;
    Std_ReturnType ret = E_NOT_OK;
    DoIP_RoleType role;
    const uint8* MetaDataPtr = info->MetaDataPtr;
    const DoIPChannelType* DoIPChannel;

    if (NULL_PTR != MetaDataPtr)
    {
        role = DoIP_GetRoleByPduRTxPduId(pdurid);

        if (((role & DOIP_ROLE_GATEWAY_IN) != 0u) || (role == DOIP_ROLE_CLIENT))
        {
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
            sa = DOIP_U8_2_U16(&MetaDataPtr[0]);
            ta = DOIP_U8_2_U16(&MetaDataPtr[2]);
#else
            sa = (((uint16)(MetaDataPtr[1])) << 8u) + (uint16)(MetaDataPtr[0]);
            ta = (((uint16)(MetaDataPtr[3])) << 8u) + (uint16)(MetaDataPtr[2]);
#endif /*HIGH_BYTE_FIRST == CPU_BYTE_ORDER*/
            for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
            {
                DoIPChannel = &DoIP_PBCfgPtr->DoIPChannel[i];
                if ((sa == DoIPChannel->DoIPChannelMappingSARef->DoIPTesterSA)
                    && (ta == DoIPChannel->DoIPChannelTARef->DoIPTargetAddressValue))
                {
                    sa = DoIPChannel->DoIPChannelSARef->DoIPTesterSA;
                    break;
                }
            }
        }
        else if (role == DOIP_ROLE_SERVER)
        {
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
            sa = DOIP_U8_2_U16(&MetaDataPtr[2]);
#else
            sa = (((uint16)(info->MetaDataPtr[3])) << 8u) + (uint16)(info->MetaDataPtr[2]);
#endif /*HIGH_BYTE_FIRST == CPU_BYTE_ORDER*/
        }
        else
        {
#if (HIGH_BYTE_FIRST == CPU_BYTE_ORDER)
            sa = DOIP_U8_2_U16(&MetaDataPtr[0]);
#else
            sa = (((uint16)(info->MetaDataPtr[1])) << 8u) + (uint16)(info->MetaDataPtr[0]);
#endif /*HIGH_BYTE_FIRST == CPU_BYTE_ORDER*/
        }
    }
    else
    {
        for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
        {
            P2CONST(DoIPPduRTxPduType, AUTOMATIC, DOIP_VAR) pdurTxPdu = DoIP_PBCfgPtr->DoIPChannel[i].DoIPPduRTxPdu;

            if ((pdurTxPdu != NULL_PTR) && (pdurid == pdurTxPdu->DoIPPduRTxPduId))
            {
                DoIPChannel = &DoIP_PBCfgPtr->DoIPChannel[i];
                sa = DoIPChannel->DoIPChannelSARef->DoIPTesterSA;
                break;
            }
        }
    }

    if (E_OK == DoIP_GetTcpConnIdxBySa(sa, &i))
    {
        *bufferId = DoIP_TcpConnStatus[i].bufferId;
        ret = E_OK;
    }

    return ret;
}

FUNC(void, DOIP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
DoIP_MsgIfRxHandler(
    DoIP_RoleType role,
    PduIdType txPduId,
    SoAd_SoConIdType SoConId,
    VAR(uint32, AUTOMATIC) payloadlength,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
    uint16 PayloadType;
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    uint8 index;
#endif /* DOIP_ROLE_CLIENT_SUPPORT == TRUE */
    PayloadType = DOIP_U8_2_U16(&PduInfoPtr->SduDataPtr[2]);
    /* PRQA S 3678++ */ /* MISRA Dir 8.13 */
    uint8* PayloadPtr = &PduInfoPtr->SduDataPtr[DOIP_HEADER_LEN];
    /* PRQA S 3678-- */ /* MISRA Dir 8.13 */
    /*RX IF data may be udp message or annoucement*/
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    if ((DOIP_ROLE_CLIENT & role) != 0u)
    {
        if ((E_OK == DoIP_GetUdpConnBySoAdId(SoConId, &index))
            || (E_OK == DoIP_GetAncConnIdBySocketId(SoConId, &index)))
        {
            DoIP_UdpConnStatus[index].outgoingMsgType = DOIP_NO_REQ_W4_RSP;

            switch (PayloadType)
            {
            case DOIP_VEHICLE_ANNOUNCE_OR_VEHICLE_INDENT_RSP:
                DoIP_HandleVehicleIdentificationRsp(SoConId, PayloadPtr);
                break;

            case DOIP_ENTITY_STATUS_RSP:
                DoIP_HandleEntityStatusRsp(txPduId, SoConId, PayloadPtr);
                break;

            case DOIP_DIAG_POWER_MODE_INFO_RSP:
                DoIP_HandlePowerModeInfoRsp(txPduId, SoConId, PayloadPtr);
                break;

            case DOIP_GENERIC_DOIP_HEADER_NACK:
                (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
                (void)SoAd_OpenSoCon(SoConId);
#endif
                /*todo*/
                break;
            default:
                /*do nothing*/
                break;
            }
        }
    }
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
    if ((DOIP_ROLE_SERVER & role) != 0u)
    {
        switch (PayloadType)
        {
        case DOIP_VEHICLE_INDENT_REQ:
        case DOIP_VEHICLE_INDENT_REQ_WITH_EID:
        case DOIP_VEHICLE_INDENT_REQ_WITH_VIN:
            DoIP_HandleVehicleIdReq(txPduId, SoConId, payloadlength, PayloadPtr, PayloadType);
            break;

        case DOIP_ENTITY_STATUS_REQ:
            DoIP_HandleEntityStatusReq(txPduId, SoConId, payloadlength);
            break;

        case DOIP_DIAG_POWER_MODE_INFO_REQ:
            DoIP_HandlePowerModeReq(txPduId, SoConId, payloadlength);
            break;
        case DOIP_VEHICLE_ANNOUNCE_OR_VEHICLE_INDENT_RSP:
        case DOIP_GENERIC_DOIP_HEADER_NACK:
        case DOIP_ROUTING_ACTIVATION_REQ:
        case DOIP_DIAG_MSG:
            SoAd_ReleaseRemoteAddr(SoConId);
            /*Ignore*/
            break;
        default:
            DoIP_UdpSendNack(txPduId, DOIP_E_UNKNOWN_PAYLOAD_TYPE);
            break;
        }
    }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
}

FUNC(BufReq_ReturnType, DOIP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
DoIP_HeaderTpRxHandler(
    DoIP_RoleType role,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    uint16 txpduId,
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
    uint8 bufIdx)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType ret = BUFREQ_OK;
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_VAR) rxBuffer = &DoIP_TcpInnerBuf[bufIdx];
    RxChannelCtrlType* rxCtrl = &rxBuffer->rxCtrl;
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */

    if (info->SduLength == 0u)
    {
        *bufferSizePtr = (PduLengthType)DOIP_HEADER_LEN - rxCtrl->rxBufPos;
        rxCtrl->rxBufRemain = *bufferSizePtr;
    }
    else if (info->SduLength <= (PduLengthType)DOIP_HEADER_LEN)
    {
        DoIP_MemCpy(info->SduDataPtr, &rxBuffer->rxChannel[rxCtrl->rxBufPos], info->SduLength);
        rxCtrl->rxBufPos += info->SduLength;
        /*has received ennough data to evaluate DoIP header*/
        if (rxCtrl->rxBufPos == (PduLengthType)DOIP_HEADER_LEN)
        {
            rxCtrl->procStatus = DOIP_RX_PROCESS_HEADER_RECEIVED;
            /*try to evaluate the DOIP header*/
            if (E_OK == DoIP_HeaderEvaluate(role, txpduId, bufIdx))
            {
                /*this is to aviod if the diagnostic message payload is too large,*/
                /*and in the first copy to pdur,may return with buffer not ok*/
                *bufferSizePtr = DOIP_MIN(
                                     rxCtrl->payloadLen + (PduLengthType)DOIP_HEADER_LEN,
                                     (PduLengthType)DOIP_MAX_NON_DIAGNOSTIC_DATA_LENGTH)
                                 - rxCtrl->rxBufPos;
                rxCtrl->rxBufRemain = *bufferSizePtr;
            }
            else
            {
                rxCtrl->procStatus = DOIP_RX_PROCESS_DISCARD;
            }
        }
        else
        {
            *bufferSizePtr = (PduLengthType)DOIP_HEADER_LEN - rxCtrl->rxBufPos;
            rxCtrl->rxBufRemain = *bufferSizePtr;
        }
        /*how about rxBufPos > rxBufPos????*/
    } /*SduLength > DOIP_HEADER_LEN*/
    else
    {
        /*accident happen*/
        ret = BUFREQ_E_NOT_OK;
    }

    return ret;
}

FUNC(BufReq_ReturnType, DOIP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
DoIP_CFTpRxHandler(
    DoIP_RoleType role,
    DoIP_DiagRcvHandleType* DoIP_DiagRcvHandle,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    uint16 txpduId,
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    BufReq_ReturnType ret = BUFREQ_OK;
    DoIP_TcpInnerBufferType* pBuffer;
    RxChannelCtrlType* rxCtrl;
    uint8 bufIdx = DoIP_DiagRcvHandle->bufIdx;

    pBuffer = &DoIP_TcpInnerBuf[bufIdx];
    rxCtrl = &pBuffer->rxCtrl;
    if (info->SduLength == 0u)
    {
        *bufferSizePtr = rxCtrl->rxBufRemain;
    }
    else
    {
        switch (rxCtrl->payloadType)
        {
        case DOIP_DIAG_MSG:
            DoIP_DiagRcvHandle->txpduId = txpduId;
            DoIP_DiagRcvHandle->buffer = &DoIP_TcpInnerBuf[bufIdx];
            ret = DoIP_DiagMsgRxHandler(role, info, bufferSizePtr, DoIP_DiagRcvHandle);
            break;
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
        case DOIP_DIAG_MSG_ACK:
            if ((DOIP_ROLE_CLIENT & role) != 0u)
            {
                if ((info->SduLength + rxCtrl->rxBufPos) <= (PduLengthType)DOIP_RX_BUFFER_SIZE)
                {
                    DoIP_MemCpy(info->SduDataPtr, &pBuffer->rxChannel[rxCtrl->rxBufPos], info->SduLength);
                    rxCtrl->rxBufPos += info->SduLength;
                    rxCtrl->rxDataRemain = rxCtrl->payloadLen - (rxCtrl->rxBufPos - (PduLengthType)DOIP_HEADER_LEN);
                    rxCtrl->rxBufRemain = rxCtrl->rxDataRemain;
                    if (rxCtrl->rxDataRemain == 0u)
                    {
                        /*Non-Diagnostic message has been completely received*/
                        /*client handle ack*/
                        DoIP_ResetTcpInnerRxBuf(bufIdx);
                    }
                    else
                    {
                        *bufferSizePtr = rxCtrl->rxBufRemain;
                    }
                }
                else
                {
                    DoIP_ResetTcpInnerRxBuf(bufIdx);
                    ret = BUFREQ_E_NOT_OK;
                }
            }
            break;
        case DOIP_DIAG_MSG_NACK:
            if ((DOIP_ROLE_CLIENT & role) != 0u)
            {
                DoIP_DiagRcvHandle->txpduId = txpduId;
                DoIP_DiagRcvHandle->buffer = &DoIP_TcpInnerBuf[bufIdx];
                ret = DoIP_DiagNackRxHandler(info, bufferSizePtr, DoIP_DiagRcvHandle);
            }
            /*todo*/
            break;
        case DOIP_ALIVE_CHECK_REQ:
        case DOIP_ROUTING_ACTIVATION_RSP:
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        case DOIP_ALIVE_CHECK_RSP:
        case DOIP_ROUTING_ACTIVATION_REQ:
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
            if ((info->SduLength + rxCtrl->rxBufPos) <= (PduLengthType)DOIP_RX_BUFFER_SIZE)
            {
                DoIP_MemCpy(info->SduDataPtr, &pBuffer->rxChannel[rxCtrl->rxBufPos], info->SduLength);
                rxCtrl->rxBufPos += info->SduLength;
                rxCtrl->rxBufRemain = (PduLengthType)DOIP_RX_BUFFER_SIZE - rxCtrl->rxBufPos;
                rxCtrl->rxDataRemain = rxCtrl->payloadLen - (rxCtrl->rxBufPos - (PduLengthType)DOIP_HEADER_LEN);
                if (rxCtrl->rxDataRemain == 0u)
                {
                    /*Non-Diagnostic message has been completely received*/
                    DoIP_TcpNonDiagMsgHandler(role, txpduId, bufIdx, rxCtrl->payloadType);
                }
                else
                {
                    *bufferSizePtr = rxCtrl->rxBufRemain;
                }
            }
            else
            {
                DoIP_ResetTcpInnerRxBuf(bufIdx);
                ret = BUFREQ_E_NOT_OK;
            }
            break;
        default:
            /*nothing to do*/
            break;
        }
    }

    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_PendingTpEnqueue(
    PendingTxQueueTpMsgType* queueMsg,
    PduIdType pduRTxId,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_VAR) pdurTxInfo)
{
    Std_ReturnType ret = E_OK;
    TxPendingTpQueueType* pendingQueue;

    if ((queueMsg->tail - queueMsg->head) != DOIP_TP_QUEUE_BUFFER_NUM) /*buffer full*/
    {
        pendingQueue = &queueMsg->pendingQueue[queueMsg->tail % DOIP_TP_QUEUE_BUFFER_NUM];
        pendingQueue->pendingPdurPduId = pduRTxId;
        pendingQueue->pendingPduInfo.SduLength = pdurTxInfo->SduLength;
        pendingQueue->pendingPduInfo.MetaDataPtr = pendingQueue->pendingPdu.dataPtr;
        DoIP_MemCpy(pdurTxInfo->MetaDataPtr, pendingQueue->pendingPdu.dataPtr, DOIP_SA_TA_LEN);
        queueMsg->tail++;
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_SoAdIfRxIndicationExtractParam(
    VAR(PduIdType, AUTOMATIC) RxPduId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DOIP_VAR) txpduId,
    P2VAR(SoAd_SoConIdType, AUTOMATIC, DOIP_VAR) SoConId,
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */
    DoIP_RoleType* role)
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;
    uint16 tempTxPduId;
    SoAd_SoConIdType tempSoConId;
    Std_ReturnType returnGetSoConId;
    Std_ReturnType returnGetUdpConn;

    if (E_OK == DoIP_ExtractUdpConnBySoadRxPduId(RxPduId, &index))
    {
        tempTxPduId = DOIP_UDPCON_CFG(index)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
        returnGetSoConId = SoAd_GetSoConId(tempTxPduId, &tempSoConId);
        returnGetUdpConn = DoIP_GetUdpConnBySoAdId(tempSoConId, &index);
        if ((E_OK == returnGetSoConId) && (E_OK == returnGetUdpConn)
            && (DoIP_UdpConnStatus[index].soConState == SOAD_SOCON_ONLINE))
        {
            *SoConId = tempSoConId;
            *txpduId = tempTxPduId;
            *role = DoIP_UdpConnStatus[index].role;
            ret = E_OK;
        }
    }
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    else if (E_OK == DoIP_ExtractAnnounceConnBySoadRxPduId(RxPduId, &index))
    {
        tempSoConId = DOIP_ANCCON_CFG(index)->soConId;
        if ((E_OK == DoIP_GetAncConnIdBySocketId(tempSoConId, &index))
            && (DoIP_AncConnStatus[index].socketId == tempSoConId)
            && (DoIP_AncConnStatus[index].soConState == SOAD_SOCON_ONLINE))
        {
            *SoConId = tempSoConId;
            *role = DOIP_ROLE_CLIENT; /*who receives annoucement is always client*/
            ret = E_OK;
        }
    }
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/

    return ret;
}

/*queue for diag ack/nack, routing active rsp/alive check req, tcp nack**/
DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_PendingNonDiagEnqueue(PendingTxQueueNonDiagMsgType* queueMsg, PduIdType soAdTxPduId, const PduInfoType* info)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_OK;
    TxPendingNonDiagQueueType* pendingQueue;

    if ((queueMsg->tail - queueMsg->head) != DOIP_NON_DIAG_QUEUE_BUFFER_NUM) /*buffer full*/
    {
        pendingQueue = &queueMsg->pendingQueue[queueMsg->tail % DOIP_NON_DIAG_QUEUE_BUFFER_NUM];
        pendingQueue->pendingSoAdTxPduId = soAdTxPduId;
        pendingQueue->pendingPduInfo.SduLength = info->SduLength;
        pendingQueue->pendingPduInfo.SduDataPtr = pendingQueue->pendingPdu.dataPtr;
        DoIP_MemCpy(info->SduDataPtr, pendingQueue->pendingPdu.dataPtr, info->SduLength);
        queueMsg->tail++;
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}

DOIP_LOCAL_INLINE
FUNC(P2VAR(TxPendingNonDiagQueueType, AUTOMATIC, DOIP_VAR), DOIP_CODE) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
DoIP_PendingNonDiagDequeue(/* PRQA S 1505 */                                             /* MISRA Rule 8.7 */
                           /* PRQA S 3432++ */                                           /* MISRA Rule 20.7 */
                           P2VAR(PendingTxQueueNonDiagMsgType, AUTOMATIC, DOIP_VAR) queueMsg)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 qID;
    TxPendingNonDiagQueueType* tempQueue = NULL_PTR;

    if ((queueMsg->tail != queueMsg->head)) /*queueMsg not empty*/
    {
        qID = queueMsg->head % DOIP_NON_DIAG_QUEUE_BUFFER_NUM;
        tempQueue = &queueMsg->pendingQueue[qID];
        queueMsg->head++;
    }

    return tempQueue;
}

DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_RegisterSocket(
        VAR(uint8, AUTOMATIC) index,
        VAR(SoAd_SoConIdType, AUTOMATIC) socketId,
        VAR(uint16, AUTOMATIC) sa,
        VAR(uint16, AUTOMATIC) ta)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    DoIP_TcpConnStatusType* tcpConnStatus = &DoIP_TcpConnStatus[index];
    tcpConnStatus->socketId = socketId;
    tcpConnStatus->sa = sa;
    tcpConnStatus->ta = ta;
    tcpConnStatus->initInactiveTimer = 0u;
    tcpConnStatus->genInactiveTimer = 0u;
    tcpConnStatus->authState = FALSE;
    tcpConnStatus->confirmState = FALSE;
    tcpConnStatus->routActiveState = DOIP_SOCKET_CONNECTION_REGISTERED;
}

/*server support apis*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
FUNC(void, DOIP_CODE) DoIP_SendVehicleIdentificationRsp(VAR(uint16, AUTOMATIC) txPduId)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    uint8 i, j;
    boolean findFlag = FALSE;
    uint16 sa;
    DoIPFurtherActionByteCallbackFncType GetFurtherActionByte;
    SoAd_SoConIdType SoConId;
    uint8 byte;
    uint8 txBuffer[41];
    PduInfoType pduInfo;
#if ((FALSE == DOIP_GID_CONFIGURED) && (FALSE == DOIP_USE_EID_AS_GID))
    uint8 temp[6];
    Std_ReturnType ret;
    DoIPGetGidCallbackFncType GetGid;
#endif /*FALSE == DOIP_GID_CONFIGURED*/
#if (TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS)
    uint8 vinGidStatus;
#endif /*TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS*/
    (void)SoAd_GetSoConId(txPduId, &SoConId);

    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    /*Vehicle Identification Response*/
    DoIP_u16_2_u8(&txBuffer[2], DOIP_VEHICLE_ANNOUNCE_OR_VEHICLE_INDENT_RSP);
    /*PayloadLength*/
#if (TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS)
    DoIP_u32_2_u8(&txBuffer[4], 33u);
#else
    DoIP_u32_2_u8(&txBuffer[4], 32u);
#endif /*TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS*/
    /*VIN---->BYTE=8->24 req SWS_DoIP_00072*/
    if (E_NOT_OK == Dcm_GetVin(&txBuffer[8]))
    {
        for (i = 0u; i < 17u; i++)
        {
            txBuffer[i + 8u] = (uint8)(DOIP_VIN_INVALIDITY_PATTERN & 0xffu);
        }
#if (TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS)
        /*SWS_DoIP_00089*/
        if (TRUE == DoIP_GidSynStatus)
        {
            vinGidStatus = 0x00;
        }
        /*SWS_DoIP_00088*/
        else
        {
            vinGidStatus = 0x10;
        }
#endif /*TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS*/
    }
#if (TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS)
    /*SWS_DoIP_00087*/
    else
    {
        vinGidStatus = 0x00;
    }
#endif /*TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS*/
    /*Byte25,Byte26----->DoIPLogicalAddress*/
    DoIP_u16_2_u8(&txBuffer[8 + 17], DoIP_PBCfgPtr->DoIPLogicalAddress);
    /*Byte27-Byte32---->EID*/
#if (FALSE == DOIP_USE_MAC_ADRESS_FOR_IDENTIFICATION)
    for (i = 27u; i < 33u; i++)
    {
        txBuffer[i] = (uint8)(DoIP_PBCfgPtr->DoIPEid >> ((32u - i) << 3));
    }
#else
    /*SWS_DoIP_00074*/
    (void)SoAd_GetPhysAddr(SoConId, &txBuffer[8 + 19 + 0]);
#endif /*FALSE == DOIP_USE_MAC_ADRESS_FOR_IDENTIFICATION*/
    /*Byte33-38---->GID*/
#if (TRUE == DOIP_USE_EID_AS_GID)
    for (i = 33u; i < 39u; i++)
    {
        txBuffer[i] = (uint8)(DoIP_PBCfgPtr->DoIPEid >> ((38u - i) << 3));
    }
#else /*TRUE == DOIP_USE_EID_AS_GID*/
#if (TRUE == DOIP_GID_CONFIGURED)
    for (i = 33u; i < 39u; i++)
    {
        txBuffer[i] = (uint8)(DoIP_PBCfgPtr->DoIPGid >> ((38u - i) << 3));
    }
#else  /*TRUE == DOIP_GID_CONFIGURED*/
    /*SWS_DoIP_00079 SWS_DoIP_00081*/
    if (NULL_PTR != DoIP_General.DoIPGetGidCbk)
    {
        if ((DoIP_General.DoIPGetGidCbk->DoIPGetGidDirect != NULL_PTR))
        {
            GetGid = DoIP_General.DoIPGetGidCbk->DoIPGetGidDirect;
            ret = (*GetGid)(temp);
            if (E_OK == ret)
            {
                for (i = 0u; i < 6u; i++)
                {
                    txBuffer[i + 8u + 25u] = temp[i];
                }
            }
            else
            {
                for (i = 0u; i < 6u; i++)
                {
                    txBuffer[i + 8u + 25u] = (uint8)(DOIP_GID_INVALIDITY_PATTERN & 0xffu);
                }
            }
        }
    }
#endif /*TRUE == DOIP_GID_CONFIGURED*/
#endif /*TRUE == DOIP_USE_EID_AS_GID*/
    /*Byte39---->Further action required Byte*/
    if (DoIP_General.DoIPFurtherActionByteCbk != NULL_PTR)
    {
        GetFurtherActionByte = DoIP_General.DoIPFurtherActionByteCbk->DoIPFurtherActionByteDirect;
        if (E_OK == (*GetFurtherActionByte)(&byte))
        {
            txBuffer[39] = byte;
        }
        else
        {
            for (i = 0u; i < DoIP_PBCfgPtr->DOIPTesterNbr; i++)
            {
                for (j = 0u; j < DoIP_PBCfgPtr->DoIPTester[i].DoIPRoutingActivationNbr; j++)
                {
                    if (0xE0u
                        == DoIP_PBCfgPtr
                               ->DoIPRoutingActivation[DoIP_PBCfgPtr->DoIPTester[i].DoIPRoutingActivationRef[j]]
                               .DoIPRoutingActivationNumber)
                    {
                        sa = DoIP_PBCfgPtr->DoIPTester[i].DoIPTesterSA;
                        findFlag = TRUE;
                        break;
                    }
                }
            }
            if (TRUE == findFlag)
            {
                /*SWS_DoIP_00084*/
                if ((E_OK == DoIP_GetTcpConnIdxBySa(sa, &i))
                    && (DOIP_SOCKET_CONNECTION_REGISTERED == DoIP_TcpConnStatus[i].routActiveState))
                {
                    txBuffer[39] = 0u;
                }
                /*SWS_DoIP_00082*/
                else
                {
                    txBuffer[39] = 0x10;
                }
            }
            /*SWS_DoIP_00083*/
            else
            {
                txBuffer[39] = 0u;
            }
        }
    }
    else
    {
        txBuffer[39] = 0u;
    }
#if (TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS)
    /*Byte40---->VIN/GID Status*/
    pduInfo.SduLength = 41u;
    txBuffer[40] = vinGidStatus;
#else  /*TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS*/
    pduInfo.SduLength = 40u;
#endif /*TRUE == DOIP_USE_VEHICLEIDENTIFICATIONSYNCSTATUS*/
    pduInfo.SduDataPtr = txBuffer;

    (void)SoAd_IfTransmit(txPduId, &pduInfo);
}

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendEntityStatusRsp(PduIdType txPduId) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 txbuffer[8 + 7];
    PduInfoType pduInfo;
#if (TRUE == DOIP_ENTITY_STATUS_MAXFIELD_USE)
    uint32 maxDataSize;
#endif /*TRUE == DOIP_ENTITY_STATUS_MAXFIELD_USE*/

    DoIP_u16_2_u8(&txbuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    /*Entity status Response*/
    DoIP_u16_2_u8(&txbuffer[2], DOIP_ENTITY_STATUS_RSP);
    /*Node Type*/
    txbuffer[8] = (uint8)DOIP_NODE_TYPE;
    /*Max open sockets*/
    txbuffer[9] = DOIP_MAX_TESTER_CONNECTIONS;
    /*Currently open sockets*/
    txbuffer[10] = DoIP_CurrentTcpSocketNbr;
#if (TRUE == DOIP_ENTITY_STATUS_MAXFIELD_USE)
    DoIP_u32_2_u8(&txbuffer[4], 7u);
    pduInfo.SduLength = 15u;
    maxDataSize = DOIP_MAX_REQUEST_BYTES;
    DoIP_u32_2_u8(&txbuffer[11], maxDataSize);
#else
    DoIP_u32_2_u8(&txbuffer[4], 3);
    pduInfo.SduLength = 11u;
#endif /*TRUE == DOIP_ENTITY_STATUS_MAXFIELD_USE*/
    pduInfo.SduDataPtr = txbuffer;
    (void)SoAd_IfTransmit(txPduId, &pduInfo);
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_SendRoutineActiveRsp(
        VAR(uint16, AUTOMATIC) txPduId,
        VAR(uint16, AUTOMATIC) sa,
        VAR(uint8, AUTOMATIC) routingActivationResponseCode,
        VAR(uint8, AUTOMATIC) bufIdx,
        P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseData,
        VAR(uint8, AUTOMATIC) responseDataLen)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE] = {0};
    PduInfoType pduInfo;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];
    TxChannelCtrlType* txCtrl = &pBuffer->txCtrl;

    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    DoIP_u16_2_u8(&txBuffer[2], DOIP_ROUTING_ACTIVATION_RSP);
    DoIP_u32_2_u8(&txBuffer[4], 13u);
    DoIP_u16_2_u8(&txBuffer[8], sa);
    DoIP_u16_2_u8(&txBuffer[8 + 2], DoIP_PBCfgPtr->DoIPLogicalAddress);
    txBuffer[8 + 4] = routingActivationResponseCode;
    DoIP_u32_2_u8(&txBuffer[8 + 5], 0u); /* reserved */

    if (responseDataLen != 0u)
    {
        DoIP_MemCpy(responseData, &txBuffer[8 + 9], responseDataLen); /* oem */
    }

    pduInfo.SduLength = DoIP_ROUTINE_ACTIVE_RESPONSE_LENGTH;
    pduInfo.SduDataPtr = txBuffer;
    if ((txCtrl->bufferState == DOIP_BUFFER_IDLE) && (txCtrl->processStatus == DOIP_TX_PROCESS_IDLE))
    {
        DoIP_MemCpy(
            &txBuffer[0],
            &pBuffer->txChannel[0],
            pduInfo.SduLength); /*aaa,when dequeue,copy to internal buffer?*/
        (void)DoIP_TriggerTpTransmit(txPduId, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        (void)DoIP_PendingNonDiagEnqueue(&pBuffer->pendingTxNonDiagQueue, txPduId, &pduInfo);
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_SendAliveCheckReq(VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];
    PduInfoType pduInfo;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];

    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    /*Alive check request*/
    DoIP_u16_2_u8(&txBuffer[2], DOIP_ALIVE_CHECK_REQ);
    /*payload len is 0*/
    DoIP_u32_2_u8(&txBuffer[4], 0u);

    pduInfo.SduLength = DOIP_ALIVECHECK_REQUEST_LENGTH;
    pduInfo.SduDataPtr = txBuffer;
    if ((pBuffer->txCtrl.bufferState == DOIP_BUFFER_IDLE) && (pBuffer->txCtrl.processStatus == DOIP_TX_PROCESS_IDLE))
    {
        DoIP_MemCpy(
            &txBuffer[0],
            &pBuffer->txChannel[0],
            pduInfo.SduLength); /*aaa,when dequeue,copy to internal buffer?*/
        (void)DoIP_TriggerTpTransmit(txPduId, &pduInfo, &pBuffer->txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        (void)DoIP_PendingNonDiagEnqueue(&pBuffer->pendingTxNonDiagQueue, txPduId, &pduInfo);
    }
}

/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendPowerModeInfoRsp(VAR(PduIdType, AUTOMATIC) TxPduId)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    uint8 txbuffer[8 + 1];
    PduInfoType pduInfo;
    uint8 powerState;
    DoIPGetPowerModeCallbackFncType GetPowerModeCallback;

    DoIP_u16_2_u8(&txbuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    /*Vehicle Identification Response*/
    DoIP_u16_2_u8(&txbuffer[2], DOIP_DIAG_POWER_MODE_INFO_RSP);
    /*PayloadLength*/
    DoIP_u32_2_u8(&txbuffer[4], 1u);

    if (DoIP_General.DoIPGetPowerModeCbk != NULL_PTR)
    {
        if ((DoIP_General.DoIPGetPowerModeCbk->DoIPPowerModeDirect != NULL_PTR))
        {
            GetPowerModeCallback = DoIP_General.DoIPGetPowerModeCbk->DoIPPowerModeDirect;
            if (E_OK == (*GetPowerModeCallback)(&powerState))
            {
                /*power mode*/
                txbuffer[8] = powerState;
            }
            else
            {
                txbuffer[8] = 0u;
            }
        }
    }
    else
    {
        txbuffer[8] = 0u;
    }
    pduInfo.SduDataPtr = txbuffer;
    pduInfo.SduLength = 9u;
    (void)SoAd_IfTransmit(TxPduId, &pduInfo);
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_HandlePowerModeReq(
        VAR(PduIdType, AUTOMATIC) TxPduId,
        SoAd_SoConIdType SoConId,
        VAR(uint32, AUTOMATIC) payloadlength)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    if (payloadlength == 0u)
    {
        DoIP_SendPowerModeInfoRsp(TxPduId);
    }
    else
    {
        DoIP_UdpSendNack(TxPduId, DOIP_E_INVALID_PAYLOAD_LENGTH);
        (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
        (void)SoAd_OpenSoCon(SoConId);
#endif
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_HandleEntityStatusReq(
        VAR(PduIdType, AUTOMATIC) TxPduId,
        SoAd_SoConIdType SoConId,
        VAR(uint32, AUTOMATIC) payloadlength)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    if (payloadlength == 0u)
    {
        DoIP_SendEntityStatusRsp(TxPduId);
    }
    else
    {
        DoIP_UdpSendNack(TxPduId, DOIP_E_INVALID_PAYLOAD_LENGTH);
        (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
        (void)SoAd_OpenSoCon(SoConId);
#endif
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_HandleVehicleIdReq(
        VAR(PduIdType, AUTOMATIC) TxPduId,
        SoAd_SoConIdType SoConId,
        VAR(uint32, AUTOMATIC) payloadlength,
        P2CONST(uint8, AUTOMATIC, DOIP_VAR) Buffer,
        uint16 Type)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    uint8 Vin[17];
    uint8 Eid[6];
    boolean pos_ack = FALSE;
    uint8 i;
    uint16 txPduId;
    uint8 index;

    if (Type == DOIP_VEHICLE_INDENT_REQ)
    {
        /* SWS_DoIP_00059*/
        if (0u == payloadlength)
        {
            pos_ack = TRUE;
        }
        else
        {
            DoIP_UdpSendNack(TxPduId, DOIP_E_INVALID_PAYLOAD_LENGTH);
            (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
            (void)SoAd_OpenSoCon(SoConId);
#endif
        }
    }
    else if (Type == DOIP_VEHICLE_INDENT_REQ_WITH_EID)
    {
        if (6u == payloadlength)
        {
            /*SWS_DoIP_00066*/
#if (FALSE == DOIP_USE_MAC_ADRESS_FOR_IDENTIFICATION)
            for (i = 0u; i < 6u; i++)
            {
                Eid[i] = (uint8)((DoIP_PBCfgPtr->DoIPEid) >> ((5u - i) << 3));
            }
            /*SWS_DoIP_00065*/
#else  /*FALSE == DOIP_USE_MAC_ADRESS_FOR_IDENTIFICATION*/
            (void)SoAd_GetPhysAddr(SoConId, Eid);
#endif /*FALSE == DOIP_USE_MAC_ADRESS_FOR_IDENTIFICATION*/
            if (0u == DoIP_DataCompare(Eid, &(Buffer[0]), 6))
            {
                pos_ack = TRUE;
            }
        }
        else
        {
            DoIP_UdpSendNack(TxPduId, DOIP_E_INVALID_PAYLOAD_LENGTH);
            (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
            (void)SoAd_OpenSoCon(SoConId);
#endif
        }
    }
    else if (Type == DOIP_VEHICLE_INDENT_REQ_WITH_VIN)
    {
        if (17u == payloadlength)
        {
            /* SWS_DoIP_00070*/
            if (E_OK != Dcm_GetVin(Vin))
            {
                for (i = 0u; i < 17u; i++)
                {
                    Vin[i] = DOIP_VIN_INVALIDITY_PATTERN & (0xffu);
                }
            }
            if (0u == DoIP_DataCompare(Vin, &(Buffer[0]), 17))
            {
                pos_ack = TRUE;
            }
        }
        else
        {
            DoIP_UdpSendNack(TxPduId, DOIP_E_INVALID_PAYLOAD_LENGTH);
            (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
            (void)SoAd_OpenSoCon(SoConId);
#endif
        }
    }
    else
    {
        pos_ack = FALSE;
    }

    if ((E_OK == DoIP_GetUdpConnBySoAdId(SoConId, &index)) && (TRUE == pos_ack))
    {
        /*SWS_DoIP_00205*/
        txPduId = DOIP_UDPCON_CFG(index)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
        DoIP_SendVehicleIdentificationRsp(txPduId);
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_HandleRoutingActiveReq(VAR(PduIdType, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    SoAd_SoConIdType SoConId;
    uint16 sa;
    uint16 ta = 0u;
    uint8 activationType = 0xFFu;
    uint8 routingActivationResponseCode;
    DoIP_SocketAssignmentResultType SocketAssignmentResult;
    const DoIP_TcpInnerBufferType* buffer;
    uint8 i;
    uint8 j;
    boolean activationIsOk = FALSE;

    uint8 responseData[4] = {0};
    uint8 responseDataLen = 0u;

    buffer = &DoIP_TcpInnerBuf[bufIdx];

    (void)SoAd_GetSoConId(txPduId, &SoConId);
    sa = DOIP_U8_2_U16(&buffer->rxChannel[8]);

    if (TRUE == DoIP_SaCheck(sa))
    {
        activationType = buffer->rxChannel[8 + 2];
        if (FALSE == DoIP_ActivationTypeCheck(activationType))
        {
            /*Routing activation denied due to unsupported routing activation type*/
            routingActivationResponseCode = 0x06;
        }
        else
        {
            for (i = 0u; i < DoIP_PBCfgPtr->DoIPChannelNbr; i++)
            {
                if ((DoIP_PBCfgPtr->DoIPChannel[i].DoIPChannelSARef->DoIPTesterSA == sa)
                    && ((DOIP_ROLE_SERVER & DoIP_PBCfgPtr->DoIPChannel[i].DoIPRole) != 0u)
                    && (NULL_PTR != DoIP_PBCfgPtr->DoIPChannel[i].DoIPPduRTxPdu)
                    && (NULL_PTR != DoIP_PBCfgPtr->DoIPChannel[i].DoIPPduRRxPdu))
                {
                    ta = DoIP_PBCfgPtr->DoIPChannel[i].DoIPChannelTARef->DoIPTargetAddressValue;
                    break;
                }
            }
            /*ISO13400-2 7.2.4*/
            SocketAssignmentResult = DoIP_SocketHandle(SoConId, sa, ta, &routingActivationResponseCode);
            if (DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL == SocketAssignmentResult)
            {
                boolean authenticated = FALSE;

                if (TRUE == DoIP_IsAuthenticatioRequiredCheck(activationType, buffer, responseData, &responseDataLen))
                {
                    authenticated = TRUE;
                }

                if (TRUE == authenticated) /* PRQA S 2995,2991 */ /* MISRA Rule 2.2,14.3 */
                {
                    if (TRUE
                        == DoIP_IsConfirmationRequiredCheck(activationType, buffer, responseData, &responseDataLen))
                    {
                        /*SWS_DoIP_00113*/
                        for (i = 0u; i < DoIP_PBCfgPtr->DOIPTesterNbr; i++)
                        {
                            if (DoIP_PBCfgPtr->DoIPTester[i].DoIPTesterSA == sa)
                            {
                                for (j = 0u; j < DoIP_PBCfgPtr->DoIPTester[i].DoIPRoutingActivationNbr; j++)
                                {
                                    if (activationType
                                        == DoIP_PBCfgPtr
                                               ->DoIPRoutingActivation[DoIP_PBCfgPtr->DoIPTester[i]
                                                                           .DoIPRoutingActivationRef[j]]
                                               .DoIPRoutingActivationNumber)
                                    {
                                        activationIsOk = TRUE;
                                        break;
                                    }
                                }
                            }
                        }
                        if (TRUE == activationIsOk)
                        {
                            /*Routing successfully activated*/
                            routingActivationResponseCode = 0x10;
                        }
                        else
                        {
                            /*Routing activation denied due to unsupported routing activation type.*/
                            routingActivationResponseCode = 0x06;
                        }
                    }
                    else
                    {
                        /*Routing activation denied due to rejected confirmation. SWS_DoIP_00274*/
                        routingActivationResponseCode = 0x05;
                    }
                }
                else
                {
                    /*Routing activation denied due to missing authentication.  SWS_DoIP_00111 */
                    routingActivationResponseCode = 0x04; /* PRQA S 2880 */ /* MISRA Rule 2.1 */
                }
            }
            else if (DOIP_SOCKET_ASSIGNMENT_FAILED == SocketAssignmentResult)
            {
                switch (routingActivationResponseCode)
                {
                case 0x01:
                case 0x02:
                case 0x03:
                    break;
                default:
                    /*reserved by 13400*/
                    routingActivationResponseCode = 0x7e;
                    break;
                }
            }
            else if (DOIP_SOCKET_ASSIGNMENT_PENDING == SocketAssignmentResult)
            {
                if (DoIP_PendingRoutMsg.pendingFlag == FALSE)
                {
                    DoIP_PendingRoutMsg.pendingSa = sa;
                    DoIP_PendingRoutMsg.pendingBufferIndex = bufIdx;
                    DoIP_PendingRoutMsg.pendingSocketConId = SoConId;
                    DoIP_PendingRoutMsg.pendingTxPduId = txPduId;
                    DoIP_PendingRoutMsg.pendingFlag = TRUE;

                    DoIP_PendingRoutMsg.responseDataLen = responseDataLen;
                    DoIP_MemCpy(responseData, DoIP_PendingRoutMsg.responseData, responseDataLen);
                }
                return;
            }
            else
            {
                /*maybe error occur*/
            }
        }
    }
    else
    {
        /*SWS_DoIP_00104,Routing activation denied due to unknown source address.*/
        routingActivationResponseCode = 0x00;
    }

    DoIP_SendRoutineActiveRsp(txPduId, sa, routingActivationResponseCode, bufIdx, responseData, responseDataLen);
    DoIP_ResetTcpInnerRxBuf(bufIdx);
    /*ISO13400-2 Table 25*/
    switch (routingActivationResponseCode)
    {
    case 0x10:
        if (E_OK == DoIP_GetTcpConnIdxBySa(sa, &i))
        {
            DoIP_TcpConnStatus[i].activationType = activationType;
        }
        break;
    case 0x11:
    case 0x04:
        break;
    default:
        if (E_OK == DoIP_GetTcpConnBySocketId(SoConId, &i))
        {
            DoIP_TcpConnStatus[i].wait4SocketClose = TRUE;
        }
        break;
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_HandleAliveCheckRsp(VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    SoAd_SoConIdType SoConId;
    uint8 i;
    uint8 j;
    P2CONST(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_CONST) buffer;
    uint16 sa;
    uint16 remainingConnections = 0u;
    boolean saRegeisted = FALSE;
    uint8 routingActivationResponseCode = 0xFF;
    DoIP_TcpConnStatusType* tcpConnStatus;

    buffer = &DoIP_TcpInnerBuf[bufIdx];

    if (E_OK == SoAd_GetSoConId(txPduId, &SoConId))
    {
        for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
        {
            tcpConnStatus = &DoIP_TcpConnStatus[i];
            if ((tcpConnStatus->socketId == SoConId) && (buffer->rxCtrl.payloadLen == 2u))
            {
                sa = (uint16)(((uint16)buffer->rxChannel[8 + 0]) << 8) + ((uint16)buffer->rxChannel[8 + 1]);
                /*SWS_DoIP_00141*/
                if ((sa == tcpConnStatus->sa) && (tcpConnStatus->routActiveState == DOIP_SOCKET_CONNECTION_REGISTERED))
                {
                    /*reset genInactiveTimer*/
                    tcpConnStatus->genInactiveTimer = 0u;
                    tcpConnStatus->wait4AliveChkRsp = FALSE;
                    saRegeisted = TRUE;
                }

                for (j = 0u; j < DOIP_MAX_TCP_CONNECTIONS; j++)
                {
                    if (TRUE == DoIP_TcpConnStatus[j].wait4AliveChkRsp)
                    {
                        remainingConnections++;
                    }
                }

                if ((0u == remainingConnections) && (DoIP_PendingRoutMsg.pendingFlag == TRUE))
                {
                    /* 0x01: Routing activation denied because all concurrently supported TCP_DATA sockets are
                     * registered and active. */
                    routingActivationResponseCode = 0x01;
                }
                else if (
                    (DoIP_PendingRoutMsg.pendingFlag == TRUE) && (sa == DoIP_PendingRoutMsg.pendingSa)
                    && (tcpConnStatus->routActiveState == DOIP_SOCKET_CONNECTION_REGISTERED))
                {
                    /* 0x03: Routing activation denied because the SA is already registered and active on a
                     * different TCP_DATA socket.*/
                    routingActivationResponseCode = 0x03;
                }
                else
                {
                    /* idle */
                }

                if (routingActivationResponseCode != 0xFFu)
                {
                    DoIP_SendRoutineActiveRsp(
                        DoIP_PendingRoutMsg.pendingTxPduId,
                        DoIP_PendingRoutMsg.pendingSa,
                        routingActivationResponseCode,
                        DoIP_PendingRoutMsg.pendingBufferIndex,
                        DoIP_PendingRoutMsg.responseData,
                        DoIP_PendingRoutMsg.responseDataLen);

                    DoIP_ResetTcpInnerRxBuf(bufIdx);
                    for (j = 0u; j < DOIP_MAX_TCP_CONNECTIONS; j++)
                    {
                        if (DoIP_TcpConnStatus[j].socketId == DoIP_PendingRoutMsg.pendingSocketConId)
                        {
                            DoIP_TcpConnStatus[j].wait4SocketClose = TRUE;
                        }
                    }
                    DoIP_ResetPendingRoutingActiveMsg();
                }
            }
        }
        if (FALSE == saRegeisted)
        {
            DoIP_CloseTcpConnection(SoConId);
            (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
            (void)SoAd_OpenSoCon(SoConId);
#endif
        }
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_SendDiagnosticAck(VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];
    PduInfoType pduInfo;
    uint16 sa;
    uint8 i;
    uint16 dataLength = 0u;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];
    const uint8* rxChannel = pBuffer->rxChannel;
    TxChannelCtrlType* txCtrl = &pBuffer->txCtrl;

    sa = DOIP_U8_2_U16(&rxChannel[8]);
    for (i = 0u; i < DoIP_PBCfgPtr->DOIPTesterNbr; i++)
    {
        if (DoIP_PBCfgPtr->DoIPTester[i].DoIPTesterSA == sa)
        {
            dataLength = DoIP_PBCfgPtr->DoIPTester[i].DoIPNumByteDiagAckNack + 5u;
        }
    }

    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    DoIP_u16_2_u8(&txBuffer[2], DOIP_DIAG_MSG_ACK);
    DoIP_u32_2_u8(&txBuffer[4], dataLength);
    DoIP_MemCpy(&rxChannel[8 + 2], &txBuffer[8], 2u);     /*ta*/
    DoIP_MemCpy(&rxChannel[8 + 0], &txBuffer[8 + 2], 2u); /*sa*/
    txBuffer[8 + 4] = 0x0;                                /*ACK code: 0x00*/
    DoIP_MemCpy(&rxChannel[8 + 4], &txBuffer[8 + 5], (PduLengthType)((PduLengthType)dataLength - 5u));
    pduInfo.SduLength = 8u + (PduLengthType)dataLength;
    pduInfo.SduDataPtr = txBuffer;

    if ((txCtrl->bufferState == DOIP_BUFFER_IDLE) && (txCtrl->processStatus == DOIP_TX_PROCESS_IDLE))
    {
        DoIP_MemCpy(&txBuffer[0], &pBuffer->txChannel[0], pduInfo.SduLength);
        (void)DoIP_TriggerTpTransmit(txPduId, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        (void)DoIP_PendingNonDiagEnqueue(&pBuffer->pendingTxNonDiagQueue, txPduId, &pduInfo);
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_SendDiagnosticNack(
        VAR(uint16, AUTOMATIC) txPduId,
        VAR(uint8, AUTOMATIC) bufIdx,
        VAR(uint8, AUTOMATIC) NackCode)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];
    PduInfoType pduInfo;
    uint16 sa;
    uint8 i;
    uint16 dataLength = 5u;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];
    const uint8* rxChannel = pBuffer->rxChannel;
    TxChannelCtrlType* txCtrl = &pBuffer->txCtrl;

    sa = DOIP_U8_2_U16(&rxChannel[8]);

    for (i = 0u; i < DoIP_PBCfgPtr->DOIPTesterNbr; i++)
    {
        if (DoIP_PBCfgPtr->DoIPTester[i].DoIPTesterSA == sa)
        {
            dataLength = DoIP_PBCfgPtr->DoIPTester[i].DoIPNumByteDiagAckNack + 5u;
        }
    }

    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    DoIP_u16_2_u8(&txBuffer[2], DOIP_DIAG_MSG_NACK);
    DoIP_u32_2_u8(&txBuffer[4], dataLength);
    /*ta*/
    DoIP_MemCpy(&rxChannel[8 + 2], &txBuffer[8], 2u);
    /*sa*/
    DoIP_MemCpy(&rxChannel[8 + 0], &txBuffer[8 + 2], 2u);
    /*NACK code*/
    txBuffer[8 + 4] = NackCode;
    /* PRQA S 4391++ */ /* MISRA Rule 10.8 */
    DoIP_MemCpy(&rxChannel[8 + 4], &txBuffer[8 + 5], (PduLengthType)(dataLength - 5u));
    /* PRQA S 4391-- */ /* MISRA Rule 10.8 */
    pduInfo.SduLength = 8u + (PduLengthType)dataLength;
    pduInfo.SduDataPtr = txBuffer;

    if ((txCtrl->bufferState == DOIP_BUFFER_IDLE) && (txCtrl->processStatus == DOIP_TX_PROCESS_IDLE))
    {
        DoIP_MemCpy(&txBuffer[0], &pBuffer->txChannel[0], pduInfo.SduLength);
        (void)DoIP_TriggerTpTransmit(txPduId, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        (void)DoIP_PendingNonDiagEnqueue(&pBuffer->pendingTxNonDiagQueue, txPduId, &pduInfo);
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_SendTcpNack(VAR(PduIdType, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) NackCode, VAR(uint8, AUTOMATIC) bufIdx)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];
    PduInfoType pduInfo;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];
    TxChannelCtrlType* txCtrl = &pBuffer->txCtrl;

    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    /*0x0000->Negative Acknowledge*/
    DoIP_u16_2_u8(&txBuffer[2], 0x00);
    /*Length = 0x00000001*/
    DoIP_u32_2_u8(&txBuffer[4], 0x01);
    /*nackcode*/
    txBuffer[8] = NackCode;
    pduInfo.SduLength = 9u;
    pduInfo.SduDataPtr = txBuffer;

    if ((txCtrl->bufferState == DOIP_BUFFER_IDLE) && (txCtrl->processStatus == DOIP_TX_PROCESS_IDLE))
    {
        DoIP_MemCpy(&txBuffer[0], &pBuffer->txChannel[0], pduInfo.SduLength);
        (void)DoIP_TriggerTpTransmit(txPduId, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        (void)DoIP_PendingNonDiagEnqueue(&pBuffer->pendingTxNonDiagQueue, txPduId, &pduInfo);
    }
}

FUNC(void, DOIP_CODE) DoIP_UdpSendNack(VAR(PduIdType, AUTOMATIC) TxPduId, VAR(uint8, AUTOMATIC) NackCode)
{
    uint8 txBuffer[9];
    PduInfoType txpduPtr;

    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    /*0x0000->Negative Acknowledge*/
    DoIP_u16_2_u8(&txBuffer[2], 0x00);
    /*Length = 0x00000001*/
    DoIP_u32_2_u8(&txBuffer[4], 0x01);
    /*nackcode*/
    txBuffer[8] = NackCode;
    txpduPtr.SduLength = 9u;
    txpduPtr.SduDataPtr = txBuffer;

    (void)SoAd_IfTransmit(TxPduId, &txpduPtr);
}

DOIP_LOCAL_INLINE FUNC(uint8, DOIP_CODE)
    /* PRQA S 1505++ */ /* MISRA Rule 8.7 */
    DoIP_DataCompare(
        P2CONST(uint8, AUTOMATIC, DOIP_VAR) data1,
        P2CONST(uint8, AUTOMATIC, DOIP_VAR) data2,
        VAR(uint32, AUTOMATIC) len)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    uint32 i = 0u;
    uint8 res = 0u;

    while (i < len)
    {
        res = data1[i] - data2[i];
        if (0u != res)
        {
            break;
        }
        i++;
    }
    return res;
}

DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE)
    DoIP_SaCheck(VAR(uint16, AUTOMATIC) sa) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 i;
    boolean ret = FALSE;

    for (i = 0u; i < DoIP_PBCfgPtr->DOIPTesterNbr; i++)
    {
        if (DoIP_PBCfgPtr->DoIPTester[i].DoIPTesterSA == sa)
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE)
    DoIP_ActivationTypeCheck(VAR(uint8, AUTOMATIC) activationType) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 i;
    boolean ret = FALSE;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPRoutingActivationNbr; i++)
    {
        if (DoIP_PBCfgPtr->DoIPRoutingActivation[i].DoIPRoutingActivationNumber == activationType)
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

/*todo,ta extract*/
DOIP_LOCAL FUNC(DoIP_SocketAssignmentResultType, DOIP_CODE)
    /* PRQA S 1505,3432++ */ /* MISRA Rule 8.7,20.7 */
    DoIP_SocketHandle(
        VAR(SoAd_SoConIdType, AUTOMATIC) socketId,
        VAR(uint16, AUTOMATIC) sa,
        VAR(uint16, AUTOMATIC) ta,
        P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseCode)
/* PRQA S 1505,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    /*Figure 13 in ISO13400-2*/
    uint8 numRegisteredSockets = 0u;
    uint8 i;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if (DOIP_SOCKET_CONNECTION_REGISTERED == DoIP_TcpConnStatus[i].routActiveState)
        {
            numRegisteredSockets++;
        }
    }
    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if (DoIP_TcpConnStatus[i].socketId == socketId)
        {
            /*We found the TCP socket. Is it registered?*/
            if (DoIP_TcpConnStatus[i].routActiveState == DOIP_SOCKET_CONNECTION_REGISTERED)
            {
                /* We found the registered TCP socket. Is it assigned to this tester (SA)?*/
                if (DoIP_TcpConnStatus[i].sa == sa)
                {
                    return DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL;
                }
                else
                {
                    /** req DoIP-106 */
                    /* Routing activation denied because an SA different
                     * from the table connection entry was received on the
                     * already activated TCP_DATA socket. */
                    *responseCode = 0x02;
                    return DOIP_SOCKET_ASSIGNMENT_FAILED;
                }
            }
        }
    }
    /*
     *  For loop terminated; that means that the current socket is currently not registered.
     *
     *Check if SA is already registered to another socket...
     */
    if ((E_OK == DoIP_GetTcpConnIdxBySa(sa, &i))
        && (DOIP_SOCKET_CONNECTION_REGISTERED == DoIP_TcpConnStatus[i].routActiveState))
    {
        /*SWS_DoIP_00105*/
        DoIP_SendSingleSaAliveCheck(sa);
        return DOIP_SOCKET_ASSIGNMENT_PENDING;
    }
    /*
     * For loop terminated; that means that the current SA is not already registered to another socket.
     *
     * Next up: Check to see that there is a free socket slot available..
     */
    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if ((DOIP_SOCKET_CONNECTION_INVALID == DoIP_TcpConnStatus[i].routActiveState)
            && (DoIP_TcpConnStatus[i].socketId == socketId) && (numRegisteredSockets < DOIP_MAX_TESTER_CONNECTIONS))
        {
            /* Yes, we found a free slot */
            /** req DoIP-090 */
            DoIP_RegisterSocket(i, socketId, sa, ta);
            return DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL;
        }
    }

    /*
     * For loop terminated; that means that there are no free slots.
     *
     * Perform alive check on all registered sockets...
     */
    /** req DoIP-094 */
    if (numRegisteredSockets >= DOIP_MAX_TESTER_CONNECTIONS)
    {
        DoIP_SendAllSocketsAliveCheck();
        return DOIP_SOCKET_ASSIGNMENT_PENDING;
    }
    /*for other event,shall return failed,will not happen*/
    return DOIP_SOCKET_ASSIGNMENT_FAILED;
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    DoIP_SendSingleSaAliveCheck(VAR(uint16, AUTOMATIC) sa) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 i;
    uint16 txPduid;
    uint8 bufIdx;
    DoIP_TcpConnStatusType* tcpConnStatus;

    for (i = DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr;
         i < DoIP_PBCfgPtr->DoIPConnections->DoIPAllTcpConnectionNbr;
         i++)
    {
        tcpConnStatus = &DoIP_TcpConnStatus[i];
        if ((tcpConnStatus->sa == sa) && (FALSE == tcpConnStatus->wait4AliveChkRsp))
        {
            txPduid = DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
            tcpConnStatus->wait4AliveChkRsp = TRUE;
            tcpConnStatus->genInactiveTimer = 0u;
            tcpConnStatus->aliveChkTimer = 0u;
            bufIdx = tcpConnStatus->bufferId;
            DoIP_SendAliveCheckReq(txPduid, bufIdx);
        }
    }
}

DOIP_LOCAL FUNC(void, DOIP_CODE) DoIP_SendAllSocketsAliveCheck(void) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    uint8 i;
    uint8 j;
    uint16 txPduid;
    uint8 bufIdx;
    boolean isServerConn = FALSE;
    DoIP_TcpConnStatusType* tcpConnStatus;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        tcpConnStatus = &DoIP_TcpConnStatus[i];
        for (j = 0u; j < DoIP_PBCfgPtr->DoIPChannelNbr; j++)
        {
            if ((tcpConnStatus->sa == DoIP_PBCfgPtr->DoIPChannel[j].DoIPChannelSARef->DoIPTesterSA)
                && ((DOIP_ROLE_SERVER & DoIP_PBCfgPtr->DoIPChannel[j].DoIPRole) != 0u))
            {
                isServerConn = TRUE;
                break;
            }
        }
        if ((DOIP_SOCKET_CONNECTION_REGISTERED == tcpConnStatus->routActiveState) && (TRUE == isServerConn))
        {
            for (j = DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr;
                 j < DoIP_PBCfgPtr->DoIPConnections->DoIPAllTcpConnectionNbr;
                 j++)
            {
                if (DOIP_TCPCON_CFG(j)->DoIPSoAdRxPdu->DoIPSoAdRxPduId == tcpConnStatus->rxPduId)
                {
                    txPduid = DOIP_TCPCON_CFG(j)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                    tcpConnStatus->wait4AliveChkRsp = TRUE;
                    tcpConnStatus->genInactiveTimer = 0u;
                    tcpConnStatus->aliveChkTimer = 0u;
                    bufIdx = tcpConnStatus->bufferId;
                    DoIP_SendAliveCheckReq(txPduid, bufIdx);
                }
            }
        }
    }
}

/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_IsAuthenticatioRequiredCheck(
    VAR(uint8, AUTOMATIC) activationType,
    P2CONST(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_CONST) buffer,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseData,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseDataLen)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    boolean req = FALSE;
    uint8 i;
    const DoIPRoutingActivationType* DoIPRoutingActivation;

    for (i = 0u; (i < DoIP_PBCfgPtr->DoIPRoutingActivationNbr) && (FALSE == req); i++)
    {
        DoIPRoutingActivation = &DoIP_PBCfgPtr->DoIPRoutingActivation[i];
        if (DoIPRoutingActivation->DoIPRoutingActivationNumber == activationType)
        {
            if ((NULL_PTR == DoIPRoutingActivation->DoIPRoutingActivationAuthenticationCallback)
                || (NULL_PTR
                    == DoIPRoutingActivation->DoIPRoutingActivationAuthenticationCallback
                           ->DoIPRoutingActivationAuthenticationFunc))
            {
                req = TRUE;
            }
            else
            {
                const uint8* reqData = NULL_PTR;
                uint8* resData = NULL_PTR;

                uint8 reqDataLen = DoIPRoutingActivation->DoIPRoutingActivationAuthenticationCallback
                                       ->DoIPRoutingActivationAuthenticationReqLength;
                /* SWS_DoIP_00109 */
                if ((buffer->rxCtrl.rxBufPos == 19u) && /* include oem */
                    (reqDataLen != 0))
                {
                    reqData = &buffer->rxChannel[8 + 7];
                }

                /* SWS_DoIP_00161 */
                if (DoIPRoutingActivation->DoIPRoutingActivationAuthenticationCallback
                        ->DoIPRoutingActivationAuthenticationResLength
                    != 0)
                {
                    resData = responseData;
                    *responseDataLen = DoIPRoutingActivation->DoIPRoutingActivationAuthenticationCallback
                                           ->DoIPRoutingActivationAuthenticationResLength;
                }

                boolean isAuthen;
                Std_ReturnType ret = DoIPRoutingActivation->DoIPRoutingActivationAuthenticationCallback
                                         ->DoIPRoutingActivationAuthenticationFunc(&isAuthen, (uint8*)reqData, resData);

                if (ret == E_OK)
                {
                    req = isAuthen;
                }
                else if (ret == E_NOT_OK)
                {
                    req = FALSE;
                }
                else if (ret == 0x10) /* DOIP_E_PENDING */
                {
                    req = FALSE; /* Todo should not simply return true or false, because there is also a pending
                                    status. */
                }
                else
                {
                    /* for qac */
                }

                /* todo 3. DOIP_E_PENDING */
            }
            break;
        }
    }

    return req;
}

/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
DOIP_LOCAL_INLINE FUNC(boolean, DOIP_CODE) DoIP_IsConfirmationRequiredCheck(
    VAR(uint8, AUTOMATIC) activationType,
    P2CONST(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_CONST) buffer,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseData,
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) responseDataLen)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    boolean req = FALSE;
    uint8 i;
    const DoIPRoutingActivationType* DoIPRoutingActivation;

    for (i = 0u; (i < DoIP_PBCfgPtr->DoIPRoutingActivationNbr) && (FALSE == req); i++)
    {
        DoIPRoutingActivation = &DoIP_PBCfgPtr->DoIPRoutingActivation[i];
        if (DoIPRoutingActivation->DoIPRoutingActivationNumber == activationType)
        {
            if ((NULL_PTR == DoIPRoutingActivation->DoIPRoutingActivationConfirmationCallback)
                || (NULL_PTR
                    == DoIPRoutingActivation->DoIPRoutingActivationConfirmationCallback
                           ->DoIPRoutingActivationConfirmationCallbackFunc))
            {
                req = TRUE;
            }
            else
            {
                const uint8* reqData = NULL_PTR;
                uint8* resData = NULL_PTR;

                uint8 reqDataLen = DoIPRoutingActivation->DoIPRoutingActivationConfirmationCallback
                                       ->DoIPRoutingActivationConfirmationReqLength;
                /* SWS_DoIP_00112 */
                if ((buffer->rxCtrl.rxBufPos == 19u) && /* include oem  */
                    (reqDataLen != 0))
                {
                    reqData = &buffer->rxChannel[8 + 7];
                }

                if (DoIPRoutingActivation->DoIPRoutingActivationConfirmationCallback
                        ->DoIPRoutingActivationConfirmationResLength
                    != 0)
                {
                    resData = responseData;
                    *responseDataLen = DoIPRoutingActivation->DoIPRoutingActivationConfirmationCallback
                                           ->DoIPRoutingActivationConfirmationResLength;
                }

                boolean isAuthen;
                Std_ReturnType ret =
                    DoIPRoutingActivation->DoIPRoutingActivationConfirmationCallback
                        ->DoIPRoutingActivationConfirmationCallbackFunc(&isAuthen, (uint8*)reqData, resData);

                if (ret == E_OK)
                {
                    req = isAuthen;
                }
                else if (ret == E_NOT_OK)
                {
                    req = FALSE;
                }
                else if (ret == 0x10) /* DOIP_E_PENDING */
                {
                    req = FALSE; /* Todo should not simply return true or false, because there is also a pending status.
                                  */
                }
                else
                {
                    /* for qac */
                }

                /* todo 3. DOIP_E_PENDING */
            }

            break;
        }
    }

    return req;
}

FUNC(void, DOIP_CODE) DoIP_VehicleAncTimeouthandler(void)
{
    uint8 i;
    uint8 index;
    uint16 txPduId;
    DoIP_VehicleAncConnStatusType* ancConnStatus;

    for (i = 0u; i < DOIP_UDP_VEHICLEANNOUNCEMENT_CONNECTIONS; i++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        ancConnStatus = &DoIP_AncConnStatus[i];
        if ((E_OK == DoIP_ExtractAnnounceConnBySoConId(ancConnStatus->socketId, &index))
            && (TRUE == ancConnStatus->vehicleAncTimerFlag))
        {
            if (index >= DoIP_PBCfgPtr->DoIPConnections->DoIPClientVehicleAnnouncementConnectionNbr)
            {
                ancConnStatus->vehicleAncTimer += DOIP_MAIN_FUNCTION_PERIOD;

                if (ancConnStatus->vehicleAncCnt == 0u)
                {
                    if (ancConnStatus->vehicleAncTimer >= (uint32)DOIP_INITIAL_VEHICLE_ANNOUNCEMENT_TIME)
                    {
                        /*SWS_DoIP_00205*/
                        txPduId = DOIP_ANCCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                        DoIP_SendVehicleIdentificationRsp(txPduId);
                        ancConnStatus->vehicleAncCnt++;
                        ancConnStatus->vehicleAncTimer = 0u;
                    }
                }
                else if (ancConnStatus->vehicleAncCnt < DOIP_VEHICLE_ANNOUNCEMENT_REPETITION)
                {
                    if (ancConnStatus->vehicleAncTimer >= DOIP_VEHICLE_ANNOUNCEMENT_INTRRNAL)
                    {
                        /*SWS_DoIP_00205*/
                        txPduId = DOIP_ANCCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                        DoIP_SendVehicleIdentificationRsp(txPduId);
                        ancConnStatus->vehicleAncCnt++;
                        ancConnStatus->vehicleAncTimer = 0u;
                    }
                }
                else
                {
                    ancConnStatus->vehicleAncTimerFlag = FALSE;
                    ancConnStatus->vehicleAncCnt = 0u;
                    ancConnStatus->vehicleAncTimer = 0u;
                }
            }
        }
    }
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_IfTransmitInternal(VAR(PduIdType, AUTOMATIC) id, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info)
{
    PduInfoType pduInfo;
    uint8 bufferId;
    PduIdType soAdTxPduId;
    uint16 sa;
    uint16 ta;
    uint16 doIPPduRTxPduRef;
    Std_ReturnType ret = DoIP_GetPduRTxPduRefByTxId(id, &doIPPduRTxPduRef, DOIP_IFPDU);

    if ((E_OK == DoIP_TransmitBufIdExtrac(id, info, &bufferId)) && (E_OK == ret))
    {
        DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufferId];
        pduInfo.SduLength = (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN + info->SduLength;
        ret = DoIP_PdurSaTaExtract(id, info, &sa, &ta);
        if (E_OK == ret)
        {
            if ((PduLengthType)DOIP_TX_BUFFER_SIZE
                < ((PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN + info->SduLength))
            {
                ret = E_NOT_OK;
            }
        }
        if (E_OK == ret)
        {
            DoIP_DiagnosticHeaderFill(info, bufferId, sa, ta);
            DoIP_MemCpy(info->SduDataPtr, &pBuffer->txChannel[DOIP_HEADER_LEN + DOIP_SA_TA_LEN], info->SduLength);
            pduInfo.SduDataPtr = &pBuffer->txChannel[0];
            if (E_OK == DoIP_SoAdTxIdExtrac(id, info, &soAdTxPduId))
            {
                if (E_OK == DoIP_TriggerTpTransmit(soAdTxPduId, &pduInfo, &pBuffer->txCtrl, DOIP_TX_DIAG_IF_MSG))
                {
                    pBuffer->txCtrl.pdurTxPduId = id;
                    pBuffer->txCtrl.pdurTxPduIdRef = doIPPduRTxPduRef;
                }
                else
                {
                    /*SWS_DoIP_00223 SWS_DoIP_00228*/
                    /*discard the message*/
                    PduR_DoIPIfTxConfirmation(id);
                }
            }
        }
    }

    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_PendingIfEnqueue(
    PendingTxQueueIfMsgType* queueMsg,
    PduIdType pduRTxId,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_VAR) pdurTxInfo)
{
    Std_ReturnType ret = E_OK;
    uint8 qID;

    if (((queueMsg->tail - queueMsg->head) == DOIP_IF_QUEUE_BUFFER_NUM)
        || ((PduLengthType)DOIP_TX_BUFFER_SIZE
            < ((PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN + pdurTxInfo->SduLength))) /*buffer full*/
    {
        ret = E_NOT_OK;
    }
    else
    {
        qID = queueMsg->tail % DOIP_IF_QUEUE_BUFFER_NUM;
        queueMsg->pendingQueue[qID].pendingPdurPduId = pduRTxId;
        queueMsg->pendingQueue[qID].pendingPduInfo.SduLength = pdurTxInfo->SduLength;
        queueMsg->pendingQueue[qID].pendingPduInfo.MetaDataPtr = queueMsg->pendingQueue[qID].pendingPdu.metaDataPtr;
        queueMsg->pendingQueue[qID].pendingPduInfo.SduDataPtr = queueMsg->pendingQueue[qID].pendingPdu.sduDataPtr;
        DoIP_MemCpy(
            &pdurTxInfo->MetaDataPtr[0],
            &queueMsg->pendingQueue[qID].pendingPdu.metaDataPtr[0],
            DOIP_SA_TA_LEN);
        DoIP_MemCpy(
            &pdurTxInfo->SduDataPtr[0],
            &queueMsg->pendingQueue[qID].pendingPdu.sduDataPtr[0],
            pdurTxInfo->SduLength);

        queueMsg->tail++;
    }

    return ret;
}

DOIP_LOCAL FUNC(void, DOIP_CODE)
    DoIP_HandleAlivecheckTimeOut(VAR(uint8, AUTOMATIC) index) /* PRQA S 1505 */ /* MISRA Rule 8.7 */
{
    SoAd_SoConIdType socketId;

    socketId = DoIP_TcpConnStatus[index].socketId;
    DoIP_CloseTcpConnection(DoIP_TcpConnStatus[index].socketId);
    (void)SoAd_CloseSoCon(socketId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
    (void)SoAd_OpenSoCon(socketId);
#endif

    if (DoIP_PendingRoutMsg.pendingFlag == TRUE)
    {
        DoIP_HandleRoutingActiveReq(DoIP_PendingRoutMsg.pendingTxPduId, DoIP_PendingRoutMsg.pendingBufferIndex);
        DoIP_ResetPendingRoutingActiveMsg();
    }
}

FUNC(Std_ReturnType, DOIP_CODE)
/* PRQA S 1505++ */ /* MISRA Rule 8.7 */
DoIP_ServerHeaderEvaluateWithPayloadType(
    VAR(uint16, AUTOMATIC) txPduId,
    VAR(uint8, AUTOMATIC) bufIdx,
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
/* PRQA S 1505-- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 index;
    boolean ProcessOn = TRUE;
    boolean invalidPayLen = FALSE;
    DoIP_TcpInnerBufferType const* pBuffer = &DoIP_TcpInnerBuf[bufIdx];
    DoIP_TcpConnStatusType* pTcpConnStatus;

    if (E_OK == DoIP_GetTcpConnBySocketId(SoConId, &index))
    {
        pTcpConnStatus = &DoIP_TcpConnStatus[index];
        if ((pTcpConnStatus->sa == DOIP_INVALID_UINT16) && (pTcpConnStatus->activationType == DOIP_INVALID_UINT8)
            && (pTcpConnStatus->routActiveState == DOIP_SOCKET_CONNECTION_INVALID))
        {
            ProcessOn = FALSE;
        }

        switch (pBuffer->rxCtrl.payloadType)
        {
        case DOIP_ROUTING_ACTIVATION_REQ:
            if ((pBuffer->rxCtrl.payloadLen != 7u) && (pBuffer->rxCtrl.payloadLen != 11u))
            {
                invalidPayLen = TRUE;
            }
            break;
        case DOIP_ALIVE_CHECK_RSP:
            if (FALSE == ProcessOn)
            {
                ret = E_NOT_OK;
            }
            else if (pBuffer->rxCtrl.payloadLen != 2u)
            {
                invalidPayLen = TRUE;
            }
            else
            {
                /*nothing*/
            }
            break;
        case DOIP_DIAG_MSG:
            if (pBuffer->rxCtrl.payloadLen < 5u)
            {
                invalidPayLen = TRUE;
            }
            break;
        /*UDP message*/
        case DOIP_VEHICLE_INDENT_REQ:
        case DOIP_VEHICLE_INDENT_REQ_WITH_EID:
        case DOIP_VEHICLE_INDENT_REQ_WITH_VIN:
        case DOIP_VEHICLE_ANNOUNCE_OR_VEHICLE_INDENT_RSP:
        case DOIP_ENTITY_STATUS_REQ:
        case DOIP_ENTITY_STATUS_RSP:
        case DOIP_DIAG_POWER_MODE_INFO_REQ:
        case DOIP_DIAG_POWER_MODE_INFO_RSP:
            ret = E_NOT_OK;
            break;
        /*Invalid message*/
        default:
            DoIP_SendTcpNack(txPduId, DOIP_E_UNKNOWN_PAYLOAD_TYPE, bufIdx);
            ret = E_NOT_OK;
            break;
        }

        if (TRUE == invalidPayLen)
        {
            DoIP_SendTcpNack(txPduId, DOIP_E_INVALID_PAYLOAD_LENGTH, bufIdx);
            pTcpConnStatus->wait4SocketClose = TRUE;
            ret = E_NOT_OK;
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

#endif /*DOIP_ROLE_SERVER_SUPPORT*/
/*client support apis*/
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
FUNC(Std_ReturnType, DOIP_CODE) DoIP_ClientActivationLineSwitchOn(uint8 VehicleID)
{
    uint8 i;
    uint16 txPduId;
    Std_ReturnType ret = E_NOT_OK;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr; i++)
    {
        if (DOIP_TCPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId == VehicleID)
        {
            txPduId = DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
            ret = DoIP_SwitchOnOneLine(txPduId, DOIP_TCPCON_CFG(i)->DoIPRequestAddressAssignment);
            break;
        }
    }
    if (E_OK == ret)
    {
        for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr; i++)
        {
            if (DOIP_UDPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId == VehicleID)
            {
                txPduId = DOIP_UDPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                ret = DoIP_SwitchOnOneLine(txPduId, DOIP_UDPCON_CFG(i)->DoIPRequestAddressAssignment);
                break;
            }
        }
    }
    if (E_OK == ret)
    {
        for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientVehicleAnnouncementConnectionNbr; i++)
        {
            if (DOIP_ANCCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId == VehicleID)
            {
                ret = DoIP_SwitchOnOneLine(txPduId, DOIP_UDPCON_CFG(i)->DoIPRequestAddressAssignment);
                break;
            }
        }
    }

    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE) DoIP_ClientActivationLineSwitchOff(uint8 VehicleID)
{
    uint8 i;
    uint16 txPduId;
    SoAd_SoConIdType soConId;
    Std_ReturnType ret = E_NOT_OK;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr; i++)
    {
        if (DOIP_TCPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId == VehicleID)
        {
            txPduId = DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
            ret = DoIP_SwitchOffOneLine(txPduId, DOIP_TCPCON_CFG(i)->DoIPRequestAddressAssignment);
            break;
        }
    }
    if (E_OK == ret)
    {
        for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr; i++)
        {
            if (DOIP_UDPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId == VehicleID)
            {
                txPduId = DOIP_UDPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                ret = DoIP_SwitchOffOneLine(txPduId, DOIP_UDPCON_CFG(i)->DoIPRequestAddressAssignment);
            }
        }
    }
    if (E_OK == ret)
    {
        for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientVehicleAnnouncementConnectionNbr; i++)
        {
            if (DOIP_ANCCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId == VehicleID)
            {
                soConId = DOIP_ANCCON_CFG(i)->soConId;
                (void)SoAd_CloseSoCon(soConId, TRUE);
                if (TRUE == DOIP_ANCCON_CFG(i)->DoIPRequestAddressAssignment)
                {
                    ret = SoAd_ReleaseIpAddrAssignment(soConId);
                }
                break;
            }
        }
    }

    return ret;
}

FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_DiagNackRxHandler(
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr,
    P2CONST(DoIP_DiagRcvHandleType, AUTOMATIC, DOIP_CONST) receiveHandle)
{
    uint8 i;
    uint8 ackNackCode;
    uint16 sa;
    PduLengthType Offset;
    DoIP_TcpInnerBufferType* rxBuffer = receiveHandle->buffer;
    SoAd_SoConIdType SoConId = receiveHandle->SoConId;
    boolean saRegested = FALSE;
    BufReq_ReturnType ret;
    RxChannelCtrlType* rxCtrl = &rxBuffer->rxCtrl;

    /*start of diagnostic message ack/nack receiving*/
    Offset = info->SduLength + rxCtrl->rxBufPos;
    if ((rxCtrl->procStatus == DOIP_RX_PROCESS_HEADER_RECEIVED)
        && (Offset > (PduLengthType)(DOIP_HEADER_LEN + DOIP_SA_TA_LEN)))
    {
        /*copy sa ta to internal buffer*/
        DoIP_MemCpy(info->SduDataPtr, &rxBuffer->rxChannel[rxCtrl->rxBufPos], info->SduLength);
        rxCtrl->rxBufPos += info->SduLength;
        sa = DOIP_U8_2_U16(&rxBuffer->rxChannel[DOIP_HEADER_LEN]);
        ackNackCode = rxBuffer->rxChannel[DOIP_HEADER_LEN + DOIP_SA_TA_LEN];

        if ((E_OK == DoIP_GetTcpConnIdxBySa(sa, &i))
            && (DOIP_SOCKET_CONNECTION_REGISTERED == DoIP_TcpConnStatus[i].routActiveState))
        {
            saRegested = TRUE;
        }

        if (saRegested == TRUE)
        {
            switch (ackNackCode)
            {
            case DOIP_DIAG_NACK_CODE_INVALID_SOURCE_ADDR:
            case DOIP_DIAG_NACK_CODE_UNKNOWN_TARGET_ADDR:
            case DOIP_DIAG_NACK_CODE_MSG_TOO_LARGE:
            case DOIP_DIAG_NACK_CODE_OUT_OF_MEMORY:
            case DOIP_DIAG_NACK_CODE_TARGET_UNREACHABLE:
            case DOIP_DIAG_NACK_CODE_UNKNOWN_NETWORK:
            case DOIP_DIAG_NACK_CODE_TRANSPORT_PROTOCOL_ERROR:
                DoIP_CloseTcpConnection(SoConId);
                (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
                (void)SoAd_OpenSoCon(SoConId);
#endif
                /*todo ,reconnect*/
                break;
            default:
                /*nothing*/
                break;
            }
        }
        ret = BUFREQ_E_NOT_OK;
    }
    else if (
        (rxCtrl->procStatus == DOIP_RX_PROCESS_HEADER_RECEIVED)
        && (Offset <= (PduLengthType)(DOIP_HEADER_LEN + DOIP_SA_TA_LEN)))
    {
        DoIP_MemCpy(info->SduDataPtr, &rxBuffer->rxChannel[rxCtrl->rxBufPos], info->SduLength);
        rxCtrl->rxBufPos += info->SduLength;
        rxCtrl->rxBufRemain = rxCtrl->payloadLen - (rxCtrl->rxBufPos - (PduLengthType)DOIP_HEADER_LEN);
        *bufferSizePtr = rxCtrl->rxBufRemain;
        ret = BUFREQ_OK;
    }
    else
    {
        ret = BUFREQ_E_NOT_OK;
    }

    return ret;
}

FUNC(void, DOIP_CODE) DoIP_VehicleConnTimeoutHandler(void)
{
    DoIPVehicleConnStateType eventInfo = DOIP_CONN_STATE_TIMEOUT;
    uint8 i;
    uint8 vehicleId;

    SchM_Enter_DoIP_ExclusiveArea();
    if ((DoIP_VehicleConnCtrl.vehicleConnState >= DOIP_CONN_STATE_STARTED)
        && (DoIP_VehicleConnCtrl.vehicleConnState != DOIP_CONN_STATE_CONNECT_SUCCESS))
    {
        if (DoIP_VehicleConnCtrl.connTimer >= DOIP_VEHICLE_CONN_TIME)
        {
            SchM_Exit_DoIP_ExclusiveArea();
            /*notify user timeout*/
            if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
            {
                DoIP_General.DoIPClientEventCbk->DoIPClientEventDirect(
                    DOIP_VEHICLEID_DONT_CARE,
                    DOIP_CLIENT_EVENT_CONNECTION,
                    (void*)&eventInfo);
            }
            /*reset connection*/
        }
        /*if no announcement received from remote, auto send req*/
        else if (
            (DoIP_VehicleConnCtrl.connTimer >= DOIP_AUTO_ID_REQ_TIME)
            && (DoIP_VehicleConnCtrl.vehicleConnState == DOIP_CONN_STATE_STARTED))
        {
            SchM_Exit_DoIP_ExclusiveArea();
            /*loop and send vehicle identification req*/
#if (DOIP_MAX_VEHICLE > 0U)
            for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr; i++)
            {
                if (DOIP_VEHICLE_ENTRY_STATE_CONNECTING == DoIP_VehicleConnCtrl.vehicleEntry[i].state)
                {
                    vehicleId = DOIP_UDPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId;
                    DoIP_SendVehicleIdReq(vehicleId);
                }
            }
#endif
            DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_DISCOVERY_START;
        }
        else
        {
            DoIP_VehicleConnCtrl.connTimer += DOIP_MAIN_FUNCTION_PERIOD;
            SchM_Exit_DoIP_ExclusiveArea();
        }
    }
    else
    {
        SchM_Exit_DoIP_ExclusiveArea();
    }
}

FUNC(void, DOIP_CODE) DoIP_DoIPMsgTimeoutHandler(void)
{
    uint8 i;
    SoAd_SoConIdType socketId;

    for (i = 0u; i < DOIP_UDP_MAX_CONNECTIONS; i++)
    {
        SchM_Enter_DoIP_ExclusiveArea();
        if (DoIP_UdpConnStatus[i].outgoingMsgType != DOIP_NO_REQ_W4_RSP)
        {
            if (DoIP_UdpConnStatus[i].doipMsgTimer >= DOIP_CTRL_TIME)
            {
                /*SWS_DoIP_00144*/
                socketId = DoIP_UdpConnStatus[i].socketId;
                DoIP_ResetUdpConn(i);
                (void)SoAd_CloseSoCon(socketId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
                (void)SoAd_OpenSoCon(socketId);
#endif
            }
            else
            {
                DoIP_UdpConnStatus[i].doipMsgTimer += DOIP_MAIN_FUNCTION_PERIOD;
            }
        }
        SchM_Exit_DoIP_ExclusiveArea();
    }
}

FUNC(void, DOIP_CODE) DoIP_VehicleDiscoveryTimeoutHandler(void)
{
    uint8 i;
    uint8 vehicleId;

    SchM_Enter_DoIP_ExclusiveArea();
    if (DoIP_VehicleConnCtrl.vehicleConnState == DOIP_CONN_STATE_DISCOVERY_WAIT)
    {
        if (DoIP_VehicleConnCtrl.discoveryWaitTimer >= DOIP_DISCOVERY_TIME)
        {
#if (DOIP_MAX_VEHICLE > 0U)
            /*resend vehicle identification request on all udp connections*/
            for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr; i++)
            {
                if (DOIP_VEHICLE_ENTRY_STATE_CONNECTING == DoIP_VehicleConnCtrl.vehicleEntry[i].state)
                {
                    vehicleId = DOIP_UDPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId;
                    DoIP_SendVehicleIdReq(vehicleId);
                }
            }
#endif
            DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_DISCOVERY_START;
        }
        else
        {
            DoIP_VehicleConnCtrl.discoveryWaitTimer += DOIP_MAIN_FUNCTION_PERIOD;
        }
    }
    else if (DoIP_VehicleConnCtrl.vehicleConnState == DOIP_CONN_STATE_DISCOVERY_COMPLETE)
    {
        /*routing activation, loop all vehicle*/
        if (E_OK == DoIP_SendAllRoutingActiveReq())
        {
            DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_ROUTING_ACTIVE;
        }
    }
    else
    {
        /*do nothing*/
    }
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_ClientHeaderEvaluateWithPayloadType(VAR(uint8, AUTOMATIC) bufIdx, VAR(SoAd_SoConIdType, AUTOMATIC) SoConId)
{
    Std_ReturnType ret = E_OK;
    uint8 index;
    DoIP_TcpInnerBufferType const* pBuffer = &DoIP_TcpInnerBuf[bufIdx];

    switch (pBuffer->rxCtrl.payloadType)
    {
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
    case DOIP_ALIVE_CHECK_REQ:
        if (pBuffer->rxCtrl.payloadLen != 2u)
        {
            if (E_OK == DoIP_GetTcpConnBySocketId(SoConId, &index))
            {
                DoIP_TcpConnStatus[index].wait4SocketClose = TRUE;
            }
            ret = E_NOT_OK;
        }
        break;
    case DOIP_ROUTING_ACTIVATION_RSP:
        if ((pBuffer->rxCtrl.payloadLen != 9u) && (pBuffer->rxCtrl.payloadLen != 13u))
        {
            if (E_OK == DoIP_GetTcpConnBySocketId(SoConId, &index))
            {
                DoIP_TcpConnStatus[index].wait4SocketClose = TRUE;
            }
            ret = E_NOT_OK;
        }
        /* code */
        break;
    case DOIP_DIAG_MSG:
    case DOIP_DIAG_MSG_ACK:
    case DOIP_DIAG_MSG_NACK:
        if (pBuffer->rxCtrl.payloadLen < 5u)
        {
            if (E_OK == DoIP_GetTcpConnBySocketId(SoConId, &index))
            {
                DoIP_TcpConnStatus[index].wait4SocketClose = TRUE;
            }
            ret = E_NOT_OK;
        }
        break;
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
    /*UDP message*/
    case DOIP_VEHICLE_ANNOUNCE_OR_VEHICLE_INDENT_RSP:
    case DOIP_ENTITY_STATUS_RSP:
    case DOIP_DIAG_POWER_MODE_INFO_RSP:
        ret = E_NOT_OK;
        break;
    default:
        /*Invalid messages*/
        ret = E_NOT_OK;
        break;
    }

    return ret;
}

FUNC(void, DOIP_CODE)
DoIP_HandleVehicleIdentificationRsp(SoAd_SoConIdType SoConId, P2VAR(uint8, AUTOMATIC, DOIP_VAR) payloadPtr)
{
    uint8 i;
    uint8 syncStatus; /*00: synchronized        10:incomplete,VIN and GID not synchronized*/
    DoIP_UdpConnStatusType* udpConnStatus;

    syncStatus = payloadPtr[DOIP_SYNC_STATUS_OFFSET];

    if ((E_OK == DoIP_GetUdpConnBySoAdId(SoConId, &i))
        && (DoIP_UdpConnStatus[i].vinGidSyncState != DOIP_VINGID_SYNC_STATE_OK))
    {
        udpConnStatus = &DoIP_UdpConnStatus[i];
        if (syncStatus == DOIP_VIN_GID_NOT_SYNC)
        {
            udpConnStatus->vinGidSyncState = DOIP_VINGID_SYNC_STATE_INCOMPLETE;
            DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_DISCOVERY_WAIT;
            DoIP_VehicleConnCtrl.discoveryWaitTimer = 0u;
        }
        else
        {
            udpConnStatus->vinGidSyncState = DOIP_VINGID_SYNC_STATE_NONE;
            DoIP_MemCpy(udpConnStatus->vehicleEntry.vin, payloadPtr, DOIP_VIN_LEN);
            udpConnStatus->vehicleEntry.logicalAddr = DOIP_U8_2_U16(&payloadPtr[DOIP_LOGICAL_ADDR_OFFSET]);
            DoIP_MemCpy(udpConnStatus->vehicleEntry.eid, &payloadPtr[DOIP_EID_OFFSET], DOIP_EID_LEN);
            DoIP_MemCpy(udpConnStatus->vehicleEntry.gid, &payloadPtr[DOIP_GID_OFFSET], DOIP_GID_LEN);

            if ((DoIP_VehicleConnCtrl.vehicleConnState == DOIP_CONN_STATE_STARTED)
                || (DoIP_VehicleConnCtrl.vehicleConnState == DOIP_CONN_STATE_DISCOVERY_WAIT))
            {
                DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_DISCOVERY_START;
            }
            if (E_OK == DoIP_IsAllVehicleSync())
            {
                DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_DISCOVERY_COMPLETE;
                if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
                {
                    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr; i++)
                    {
                        DoIP_General.DoIPClientEventCbk->DoIPClientEventDirect(
                            DOIP_VEHICLEID_DONT_CARE,
                            DOIP_CLIENT_EVENT_DISCOVERY,
                            (void*)udpConnStatus);
                        /*report the vehicle info to user*/
                    }
                }
            }
        }
    }
}

FUNC(void, DOIP_CODE)
DoIP_HandlePowerModeInfoRsp(PduIdType txPduId, SoAd_SoConIdType SoConId, P2VAR(uint8, AUTOMATIC, DOIP_VAR) payloadPtr)
{
    uint8 i;
    uint8 vehicleId;
    DoIP_UdpConnStatusType* udpConnStatus;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr; i++)
    {
        if (txPduId == DOIP_UDPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId)
        {
            vehicleId = DOIP_UDPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId;
            break;
        }
    }

    for (i = 0u; i < DOIP_UDP_MAX_CONNECTIONS; i++)
    {
        udpConnStatus = &DoIP_UdpConnStatus[i];
        if (udpConnStatus->socketId == SoConId)
        {
            udpConnStatus->nodeInfo.powerMode = payloadPtr[0];
            if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
            {
                DoIP_General.DoIPClientEventCbk->DoIPClientEventDirect(
                    vehicleId,
                    DOIP_CLIENT_EVENT_POWER_MODE,
                    (void*)&udpConnStatus->nodeInfo);
            }

            break;
        }
    }
}

FUNC(void, DOIP_CODE)
DoIP_HandleEntityStatusRsp(PduIdType txPduId, SoAd_SoConIdType SoConId, P2VAR(uint8, AUTOMATIC, DOIP_VAR) payloadPtr)
{
    uint8 i;
    uint8 vehicleId;
    DoIP_UdpConnStatusType* udpConnStatus;

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr; i++)
    {
        if (txPduId == DOIP_UDPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId)
        {
            vehicleId = DOIP_UDPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId;
            break;
        }
    }

    for (i = 0u; i < DOIP_UDP_MAX_CONNECTIONS; i++)
    {
        udpConnStatus = &DoIP_UdpConnStatus[i];
        if (udpConnStatus->socketId == SoConId)
        {
            udpConnStatus->nodeInfo.nodeType = payloadPtr[0];
            udpConnStatus->nodeInfo.maxOpenSocket = payloadPtr[1];
            udpConnStatus->nodeInfo.currentlyOpenSocket = payloadPtr[2];
            udpConnStatus->nodeInfo.maxDataSize = DOIP_U8_2_U32(&payloadPtr[3]);

            if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
            {
                DoIP_General.DoIPClientEventCbk->DoIPClientEventDirect(
                    vehicleId,
                    DOIP_CLIENT_EVENT_POWER_MODE,
                    (void*)&DoIP_UdpConnStatus[i].nodeInfo);
            }
            break;
        }
    }
}

FUNC(void, DOIP_CODE)
DoIP_HandleRoutingActiveRsp(uint8 vehicleId, VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx)
{
    uint8 tcpConnIdex;
    uint16 sa;
    uint16 ta;
    uint8 routingActivationRspCode;
    uint8 OemSpec[4];
    P2VAR(uint8, AUTOMATIC, DOIP_VAR) rxPayloadPtr;
    SoAd_SoConIdType SoConId;

    rxPayloadPtr = &DoIP_TcpInnerBuf[bufIdx].rxChannel[8];
    sa = DOIP_U8_2_U16(&rxPayloadPtr[0]);
    ta = DOIP_U8_2_U16(&rxPayloadPtr[2]);
    routingActivationRspCode = rxPayloadPtr[4]; /*byte5~8,reserved*/
    DoIP_MemCpy(&rxPayloadPtr[9], OemSpec, 4u);
    (void)SoAd_GetSoConId(txPduId, &SoConId);

#if (DOIP_MAX_VEHICLE > 0U)
    for (tcpConnIdex = 0u; tcpConnIdex < DOIP_MAX_TCP_CONNECTIONS; tcpConnIdex++)
    {
        /*match soConId,sa,vehicleId */
        if ((DoIP_TcpConnStatus[tcpConnIdex].socketId == SoConId) && (DoIP_TcpConnStatus[tcpConnIdex].sa == sa)
            && (vehicleId < DOIP_MAX_VEHICLE)
            && (DOIP_VEHICLE_ENTRY_STATE_CONNECTING == DoIP_VehicleConnCtrl.vehicleEntry[vehicleId].state))
        {
            /*tcpConnIdex will indicate if check pass*/
            break;
        }
    }
#endif

    if (tcpConnIdex < DOIP_MAX_TCP_CONNECTIONS)
    {
        switch (routingActivationRspCode)
        {
        case 0x11: /*activated but confirm required*/
            /*to do, confirm operation may occur in other layer*/
            if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
            {
                DoIP_General.DoIPClientEventCbk->DoIPClientEventDirect(
                    vehicleId,
                    DOIP_CLIENT_EVENT_CONNECTION,
                    (void*)&DoIP_VehicleConnCtrl.vehicleConnState);
            }

            break;
        case 0x10:                                             /*successfull*/
            DoIP_RegisterSocket(tcpConnIdex, SoConId, sa, ta); /*sa may reset with a same value, don't care*/

#if (DOIP_MAX_VEHICLE > 0U)
            DoIP_VehicleConnCtrl.vehicleEntry[vehicleId].state = DOIP_VEHICLE_ENTRY_STATE_CONNECTED;
#endif

            DoIP_VehicleConnCtrl.waitForActiveCnt--;
            if (0U == DoIP_VehicleConnCtrl.waitForActiveCnt)
            {
                DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_CONNECT_SUCCESS;
                if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
                {
                    DoIP_General.DoIPClientEventCbk->DoIPClientEventDirect(
                        DOIP_VEHICLEID_DONT_CARE,
                        DOIP_CLIENT_EVENT_CONNECTION,
                        (void*)&DoIP_VehicleConnCtrl.vehicleConnState);
                }
                /*Reset all VehicleConnCtrl params exclude the vehicle entries*/
                DoIP_ResetVehicleConnCtrl();
            }
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
            if (DoIP_PendingRoutMsg.pendingFlag == TRUE)
            {
                DoIP_SendRoutineActiveRsp(
                    DoIP_PendingRoutMsg.pendingTxPduId,
                    DoIP_PendingRoutMsg.pendingSa,
                    0x10,
                    DoIP_PendingRoutMsg.pendingBufferIndex,
                    DoIP_PendingRoutMsg.responseData,
                    DoIP_PendingRoutMsg.responseDataLen);

                DoIP_ResetTcpInnerRxBuf(DoIP_PendingRoutMsg.pendingBufferIndex);

                DoIP_ResetPendingRoutingActiveMsg();
            }
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
            break;
        case 0x00: /*unknown source address*/
        case 0x01: /*all sockets registered and active*/
        case 0x02: /*sa different*/
        case 0x03: /*sa already registerd on a diffrent socket*/
        case 0x04: /*missing authentication*/
        case 0x05: /*rejected confirm*/
        case 0x06: /*unsupporeted routing activation type*/
        case 0x07: /*secure socket required*/
            if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
            {
                DoIP_General.DoIPClientEventCbk->DoIPClientEventDirect(
                    vehicleId,
                    DOIP_CLIENT_EVENT_CONNECTION,
                    (void*)&DoIP_VehicleConnCtrl.vehicleConnState);
            }

            break;
        default:
            /*reserved and VM-spec*/
            if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
            {
                DoIP_General.DoIPClientEventCbk->DoIPClientEventDirect(
                    vehicleId,
                    DOIP_CLIENT_EVENT_CONNECTION,
                    (void*)&DoIP_VehicleConnCtrl.vehicleConnState);
            }

            break;
        }
    }
}

DOIP_LOCAL_INLINE FUNC(void, DOIP_CODE)
    DoIP_HandleAliveCheckReq(uint8 vehicleId, VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx)
{
    uint8 i;
    uint16 sourceAddr;
    P2VAR(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_VAR) buffer;

    buffer = &DoIP_TcpInnerBuf[bufIdx];
    sourceAddr = DOIP_U8_2_U16(&buffer->rxChannel[8]);

    /*reset general Inactivity Timer*/
    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if (DoIP_TcpConnStatus[i].bufferId == bufIdx)
        {
            DoIP_TcpConnStatus[i].genInactiveTimer = 0u;
            break;
        }
    }

    if (NULL_PTR != DoIP_General.DoIPClientEventCbk)
    {
        if (E_OK
            == DoIP_General.DoIPClientEventCbk
                   ->DoIPClientEventDirect(vehicleId, DOIP_CLIENT_EVENT_ALIVE_CHECK, (void*)&sourceAddr))
        {
            (void)DoIP_SendAliveCheckRsp(txPduId, bufIdx, sourceAddr);
        }
    }
}

FUNC(Std_ReturnType, DOIP_CODE) DoIP_SendAllRoutingActiveReq(void)
{
    uint8 i, j;
    uint8 bufIdx = 0;
    uint8 vehicleId;
    uint16 lowerTxPduId;
    SoAd_SoConIdType SoConId;
    Std_ReturnType ret = E_NOT_OK;
    DoIP_TcpInnerBufferType* pBuffer;

#if (DOIP_MAX_VEHICLE > 0U)
    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr; i++)
    {
        /*get txpduid*/
        vehicleId = DOIP_TCPCON_CFG(i)->DoIPConnVehicleRef->DoIPVehicleId;

        if (DOIP_VEHICLE_ENTRY_STATE_CONNECTING == DoIP_VehicleConnCtrl.vehicleEntry[vehicleId].state)
        {
            lowerTxPduId = DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
            /*get an idle buffer*/
            for (j = 0u; j < DOIP_MAX_TCP_CONNECTIONS; j++)
            {
                pBuffer = &DoIP_TcpInnerBuf[j];
                if ((pBuffer->txCtrl.bufferState == DOIP_BUFFER_IDLE)
                    && (pBuffer->txCtrl.processStatus == DOIP_TX_PROCESS_IDLE))
                {
                    bufIdx = j;
                    ret = E_OK;
                    break;
                }
            }
            /*get tcp conn, socketId registered after mode change*/
            if (E_OK == ret)
            {
                if (E_OK == SoAd_GetSoConId(lowerTxPduId, &SoConId))
                {
                    if (E_OK == DoIP_GetTcpConnBySocketId(SoConId, &j))
                    {
                        DoIP_TcpConnStatus[j].bufferId = bufIdx;
                        DoIP_TcpConnStatus[j].sa = DoIP_PBCfgPtr->DoIPConnections->DoIPSourceAddress->DoIPTesterSA;
                    }

                    if (E_NOT_OK
                        == DoIP_SendRoutingActReq(
                            lowerTxPduId,
                            bufIdx,
                            DoIP_PBCfgPtr->DoIPConnections->DoIPSourceAddress->DoIPTesterSA))
                    {
                        ret = E_NOT_OK;
                        break;
                    }
                }
                else
                {
                    ret = E_NOT_OK;
                    break;
                }
            }
        }
    }
#endif

    return ret;
}

FUNC(Std_ReturnType, DOIP_CODE)
DoIP_SendRoutingActReq(VAR(PduIdType, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx, uint16 sa)
{
    PduInfoType pduInfo;
    Std_ReturnType ret = E_NOT_OK;
    uint8 i;
    uint8 activationType;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];
#if (DOIP_OEM_SPEC_SUPPORT == TRUE)
    uint32 oem_specific;
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_GENERIC_HEADER_LEN + DOIP_ROUTING_ACTIVE_REQ_PAY_LEN_WITH_OEMSPEC];
#else
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_GENERIC_HEADER_LEN + DOIP_ROUTING_ACTIVE_REQ_PAY_LEN];
#endif /*DOIP_OEM_SPEC_SUPPORT == TRUE*/

    for (i = 0u; i < DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr; i++)
    {
        if (txPduId == DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId)
        {
            activationType = DOIP_TCPCON_CFG(i)->DoIPConnVehicleRef->DoIPRoutingActivationNumber;
#if (DOIP_OEM_SPEC_SUPPORT == TRUE)
            oem_specific = DOIP_TCPCON_CFG(i)->DoIPConnVehicleRef->oemSpec;
#endif /*DOIP_OEM_SPEC_SUPPORT == TRUE*/
            ret = E_OK;
            break;
        }
    }

    if (E_OK == ret)
    {
        DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
        DoIP_u16_2_u8(&txBuffer[2], DOIP_ROUTING_ACTIVATION_REQ);
        DoIP_u16_2_u8(&txBuffer[8], sa);
        txBuffer[10] = activationType;
        DoIP_u32_2_u8(&txBuffer[11], 0x00000000); /*reserved*/
#if (DOIP_OEM_SPEC_SUPPORT == TRUE)
        DoIP_u32_2_u8(&txBuffer[4], DOIP_ROUTING_ACTIVE_REQ_PAY_LEN_WITH_OEMSPEC);
        DoIP_u32_2_u8(&txBuffer[15], oem_specific);
        pduInfo.SduLength = DOIP_GENERIC_HEADER_LEN + DOIP_ROUTING_ACTIVE_REQ_PAY_LEN_WITH_OEMSPEC;
#else
        DoIP_u32_2_u8(&txBuffer[4], DOIP_ROUTING_ACTIVE_REQ_PAY_LEN);
        pduInfo.SduLength = DOIP_GENERIC_HEADER_LEN + DOIP_ROUTING_ACTIVE_REQ_PAY_LEN;
#endif /*DOIP_OEM_SPEC_SUPPORT == TRUE*/
        pduInfo.SduDataPtr = &txBuffer[0];

        if ((pBuffer->txCtrl.bufferState == DOIP_BUFFER_IDLE)
            && (pBuffer->txCtrl.processStatus == DOIP_TX_PROCESS_IDLE))
        {
            DoIP_MemCpy(&txBuffer[0], &pBuffer->txChannel[0], pduInfo.SduLength);
            if (E_OK == DoIP_TriggerTpTransmit(txPduId, &pduInfo, &pBuffer->txCtrl, DOIP_TX_NON_DIAG_MSG))
            {
                DoIP_VehicleConnCtrl.waitForActiveCnt++;
                ret = E_OK;
            }
            else
            {
                ret = E_NOT_OK;
            }
        }
        else
        {
            ret = DoIP_PendingNonDiagEnqueue(&pBuffer->pendingTxNonDiagQueue, txPduId, &pduInfo);
        }
    }

    return ret;
}

DOIP_LOCAL FUNC(Std_ReturnType, DOIP_CODE)
    DoIP_SendAliveCheckRsp(VAR(uint16, AUTOMATIC) txPduId, VAR(uint8, AUTOMATIC) bufIdx, uint16 SourceAddr)
{
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_GENERIC_HEADER_LEN + DOIP_ALIVE_CHECK_RESP_PAY_LEN];
    Std_ReturnType ret = E_NOT_OK;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];

    PduInfoType pduInfo;
    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    DoIP_u16_2_u8(&txBuffer[2], DOIP_ALIVE_CHECK_RSP);
    DoIP_u32_2_u8(&txBuffer[4], DOIP_ALIVE_CHECK_RESP_PAY_LEN);
    DoIP_u16_2_u8(&txBuffer[8], SourceAddr);

    pduInfo.SduLength = DOIP_GENERIC_HEADER_LEN + DOIP_ALIVE_CHECK_RESP_PAY_LEN;
    pduInfo.SduDataPtr = &txBuffer[0];
    if ((pBuffer->txCtrl.bufferState == DOIP_BUFFER_IDLE) && (pBuffer->txCtrl.processStatus == DOIP_TX_PROCESS_IDLE))
    { /*aaa,when dequeue,copy to internal buffer?*/
        DoIP_MemCpy(&txBuffer[0], &pBuffer->txChannel[0], pduInfo.SduLength);
        ret = DoIP_TriggerTpTransmit(txPduId, &pduInfo, &pBuffer->txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        ret = DoIP_PendingNonDiagEnqueue(&pBuffer->pendingTxNonDiagQueue, txPduId, &pduInfo);
    }

    return ret;
}
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
FUNC(void, DOIP_CODE) DoIP_GatewayTxFakeResp(uint8 index, uint8 rspCode)
{
    VAR(uint8, AUTOMATIC) txBuffer[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];
    uint8 bufIdx = DoIP_TcpConnStatus[index].bufferId;
    PduInfoType pduInfo;
    uint16 sa, ta;
    uint16 soadTxPduId;
    uint8 i;
    DoIP_TcpInnerBufferType* pBuffer = &DoIP_TcpInnerBuf[bufIdx];

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        if (DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdTxPduId == DoIP_TcpConnStatus[index].txPduId)
        {
            soadTxPduId = DOIP_TCPCON_CFG(i)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
        }
    }

    sa = DoIP_TcpConnStatus[index].ta;
    ta = DoIP_TcpConnStatus[index].sa;

    DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
    DoIP_u16_2_u8(&txBuffer[2], DOIP_DIAG_MSG);
    DoIP_u32_2_u8(&txBuffer[4], 7u);
    DoIP_u16_2_u8(&txBuffer[8], sa);
    DoIP_u16_2_u8(&txBuffer[10], ta);

    txBuffer[8 + 4] = 0x7f;
    txBuffer[8 + 5] = DoIP_TcpConnStatus[index].gwCtrl.lastSid;
    txBuffer[8 + 6] = rspCode;

    pduInfo.SduLength = 8u + 7u;
    pduInfo.SduDataPtr = txBuffer;

    if ((pBuffer->txCtrl.bufferState == DOIP_BUFFER_IDLE) && (pBuffer->txCtrl.processStatus == DOIP_TX_PROCESS_IDLE))
    {
        DoIP_MemCpy(&txBuffer[0], &pBuffer->txChannel[0], pduInfo.SduLength);
        (void)DoIP_TriggerTpTransmit(soadTxPduId, &pduInfo, &pBuffer->txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        (void)DoIP_PendingNonDiagEnqueue(&pBuffer->pendingTxNonDiagQueue, soadTxPduId, &pduInfo);
    }
}

FUNC(void, DOIP_CODE) DoIP_GatewayTxFakeRespTimeoutHandler(void)
{
    uint8 i;
    DoIP_GatewayCtrlType* gwCtrl;

    for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
    {
        gwCtrl = &DoIP_TcpConnStatus[i].gwCtrl;
        if ((gwCtrl->waitRmtRsp == TRUE) && (gwCtrl->waitRspCfg != NULL_PTR))
        {
            if ((0u == gwCtrl->txNrc78Cnt) && (gwCtrl->waitRmtTimer >= gwCtrl->waitRspCfg->wait1stRspTime))
            {
                gwCtrl->waitRmtTimer = 0u;
                gwCtrl->txNrc78Cnt++;
                DoIP_GatewayTxFakeResp(i, 0x78);
            }
            else if (gwCtrl->waitRmtTimer >= gwCtrl->waitRspCfg->waitRspTime)
            {
                if (gwCtrl->nrc78Rcvd == TRUE)
                {
                    if (gwCtrl->txNrc78Cnt >= gwCtrl->waitRspCfg->fakeRspMaxCnt)
                    {
                        DoIP_GatewayTxFakeResp(i, 0x10);
                        gwCtrl->nrc78Rcvd = FALSE;
                        gwCtrl->txNrc78Cnt++;
                    }
                    else
                    {
                        DoIP_GatewayTxFakeResp(i, 0x78);
                        gwCtrl->txNrc78Cnt++;
                    }

                    gwCtrl->waitRmtTimer = 0u;
                }
                else
                {
                    /*close diag session, can't close sockect*/
                    gwCtrl->waitRmtRsp = FALSE;
                    gwCtrl->txNrc78Cnt = 0u;
                }
            }
            else
            {
                /*do nothing*/
            }

            gwCtrl->waitRmtTimer += DOIP_MAIN_FUNCTION_PERIOD;
        }
    }
}
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
/*=======[E N D   O F   F I L E]==============================================*/
