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
 **  @file               : DoIP_Internal_TCP_Non_Diagnostic.c
 **  @author             : hao.wen
 **  @date               : 2024/11/01
 **  @vendor             : isoft
 **  @description        : Implement the non-diagnostic part of TCP
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

#include "DoIP_Internal_Common.h"
#include "DoIP_Internal_TCP_Common.h"
#include "DoIP_Internal_TCP_Non_Diagnostic.h"
#include "SchM_DoIP.h"

/* ===================================================== macros ===================================================== */

/* ra */

#define DOIP_RA_REQ_MSG_FIELD_POSITION_SA 8u /**< Location of SA field in routing activation request message */
#define DOIP_RA_REQ_MSG_FILED_POSITION_ACTIVATION_TYPE \
    10u /**< Location of activation type field in routing activation request message */

#define DOIP_RA_RSP_MSG_LEN         21u /**< Length of routing activation response message */
#define DOIP_RA_RSP_MSG_PAYLOAD_LEN 13u /**< Length of routing activation response message payload */

#define DOIP_RA_RSP_MSG_FIELD_POSITION_TESTER_SA \
    8u /**< Location of tester's SA field in routing activation response message */
#define DOIP_RA_RSP_MSG_FIELD_POSITION_SERVER_SA \
    10u /**< Location of DoIP's SA field in routing activation response message */
#define DOIP_RA_RSP_MSG_FIELD_POSITION_CODE 12u /**< Location of code field in routing activation response message */

#define DOIP_RA_DENIED_UNKNOWN_SA 0x00u /**< Routing activation denied due to unknown source address */
#define DOIP_RA_DENIED_NO_FREE_SOCKET                                                                                  \
    0x01u /**< Routing activation denied because all concurrently supported TCP_DATA sockets are registered and active \
           */
#define DOIP_RA_DENIED_SA_NOT_MATCH                                                                                   \
    0x02u /**< Routing activation denied because the SA different from the table connection entry was received on the \
             already actiate TCP_DATA socket */
#define DOIP_RA_DENIED_SA_ALREADY_REGISTERED                                                                     \
    0x03u /**< Routing activation denied because the SA is already registered and active on a different TCP_DATA \
             socket */
#define DOIP_RA_DENIED_MISSING_AUTHEN   0x04u /**< Routing activation denied due to missing authentication */
#define DOIP_RA_DENIED_REJECTED_CONFIRM 0x05u /**< Routing activation denied due to rejected confirmation */
#define DOIP_RA_DENIED_UNSUPPORTED_TYPE \
    0x06u                     /**< Routing activation denied due to unsupported routing activation type */
#define DOIP_RA_SUCCESS 0x10u /**< Routing successfully activated */

/* alive check */

#define DOIP_ALIVE_CHECK_REQ_MSG_LEN         8u /**< Length of alive check request message */
#define DOIP_ALIVE_CHECK_REQ_MSG_PAYLOAD_LEN 0u /**< Length of alive check request payload */

#define DOIP_ALIVE_CHECK_RSP_MSG_PAYLOAD_LEN       2u /**< Length of alive check response payload */
#define DOIP_ALIVE_CHECK_RSP_MSG_FIELD_POSITION_SA 8u /**< Location of SA field in alive check response message */

/* ============================================ internal data definitions =========================================== */

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/**
 * @brief Record routing activation request
 *        @range NA
 */
static DoIP_PendingRaType DoIP_PendingRa;
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"

/* ========================================== internal function definitions ========================================= */

/* pending */

/**
 * @brief       Get message from queue.
 * @param[in]   queue: Specify queue.
 * @return      DoIP_NonDiagItemType
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
/* PRQA S 5016 ++ */ /* VL_DoIP_5016 */
DOIP_LOCAL_INLINE DoIP_NonDiagItemType* DoIP_DequeueNonDiagMsg(DoIP_NonDiagQueueType* queue);
/* PRQA S 5016 -- */

/* routing activate  */

/**
 * @brief       Check whether the SA is valid.
 * @param[in]   sa: LA of tester.
 * @return      boolean
 * @retval      TRUE: Valid.
 * @retval      FALSE: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE boolean DoIP_CheckSa(uint16 sa);

/**
 * @brief       Check whether the routing activation type is valid.
 * @param[in]   sa: LA of tester.
 * @param[in]   activationType: Routing activation type.
 * @return      boolean
 * @retval      TRUE: Valid.
 * @retval      FALSE: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE boolean DoIP_CheckRaType(uint16 sa, uint8 activationType);

/**
 * @brief       Routing activation authentication.
 * @param[in]   activationType: Routing activation type.
 * @return      boolean
 * @retval      TRUE: Authentication success.
 * @retval      FALSE: Authentication fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE boolean DoIP_IsSucceedAuthentication(uint8 activationType);

/**
 * @brief       Routing activation confirmation.
 * @param[in]   activationType: Routing activation type.
 * @return      boolean
 * @retval      TRUE: Confirmation success.
 * @retval      FALSE: Confirmation fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE boolean DoIP_IsSucceedConfirmation(uint8 activationType);

/**
 * @brief       Register socket to DoIP_TcpConnStatus.
 * @param[in]   index: Index of DoIP_TcpConnStatus.
 * @param[in]   soConId: Specified socket.
 * @param[in]   sa: LA of tester.
 * @param[in]   ta: LA of DoIP node.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL_INLINE void DoIP_RegisterSocket(uint8 index, SoAd_SoConIdType soConId, uint16 sa, uint16 ta);

/**
 * @brief       Is SA already registered.
 * @param[in]   soConId: Specified socket.
 * @param[in]   sa: LA of tester.
 * @param[out]  rspCode: Routing activation code.
 * @param[out]  socketAssignmentResult: Socket assignment result.
 * @return      boolean
 * @retval      TRUE: Registered.
 * @retval      FALSE: Not registered.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_CheckSaAlreadyRegistered(
    SoAd_SoConIdType                 soConId,
    uint16                           sa,
    uint8*                           rspCode,
    DoIP_SocketAssignmentResultType* socketAssignmentResult);

/**
 * @brief       Send alive check request.
 * @param[in]   sa: LA of tester.
 * @param[out]  socketAssignmentResult: Socket assignment result.
 * @return      boolean
 * @retval      TRUE: Send alive check requet success.
 * @retval      FALSE: Send alive check requet fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_StartAliveCheck(uint16 sa, DoIP_SocketAssignmentResultType* socketAssignmentResult);

/**
 * @brief       Assign socket.
 * @param[in]   soConId: Specified socket.
 * @param[in]   sa: LA of tester.
 * @param[in]   ta: LA of DoIP node.
 * @param[out]  rspCode: Routing activation code.
 * @return      DoIP_SocketAssignmentResultType
 * @retval      DOIP_SOCKET_ASSIGNMENT_FAILED: Assign socket fail.
 * @retval      DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL: Assign socket success.
 * @retval      DOIP_SOCKET_ASSIGNMENT_PENDING: Wait alive check result.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL DoIP_SocketAssignmentResultType
    DoIP_AssignSocket(SoAd_SoConIdType soConId, uint16 sa, uint16 ta, uint8* rspCode);

/**
 * @brief       Send routing activation response message.
 * @param[in]   soadTxPduRef: SoadTxPduRef.
 * @param[in]   sa: LA of tester.
 * @param[in]   raRspCode: Routing activation code.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_SendRaRsp(uint16 soadTxPduRef, uint16 sa, uint8 raRspCode, uint8 txRxCtxIdx);

/**
 * @brief       Check whether the routing activation request is valid.
 * @param[in]   sa: LA of tester.
 * @param[out]  ta: Obtain TA from the channel.
 * @param[in]   activationType: Routing activation type.
 * @param[out]  raRspCode: Routing activation code.
 * @return      boolean
 * @retval      TRUE: Valid.
 * @retval      FALSE: Invalid.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_IsValidRaReq(uint16 sa, uint16* ta, uint8 activationType, uint8* raRspCode);

/**
 * @brief       Handle routing activation request.
 * @param[in]   soadTxPduRef: SoadTxPduRef.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_HandleRaReq(PduIdType soadTxPduRef, uint8 txRxCtxIdx);

/* alive check */

/**
 * @brief       Send alive check request.
 * @param[in]   soadTxPduRef: SoadTxPduRef.
 * @param[in]   txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_SendAliveCheckReq(uint16 soadTxPduRef, uint8 txRxCtxIdx);

/**
 * @brief       Send alive check request to one tester.
 * @param[in]   sa: LA of Tester.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_StartSingleAliveCheck(uint16 sa);

/**
 * @brief       Send alive check request to all tester.
 * @param[in]   sa: LA of Tester.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_StartAllAliveCheck(void);

/**
 * @brief        Handle pending routing activation request.
 * @param[in]    sa: LA of Tester.
 * @param[in]    txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @param[in]    tcpConnStatusArray: Used for statistics on the number of remaining TCP connections.
 * @param[inout] tcpConnStatus: Element of DoIP_TcpConnStatus.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_HandlePendingRaReq(
    uint16                       sa,
    uint8                        txRxCtxIdx,
    const DoIP_TcpConnStatusType tcpConnStatusArray[],
    DoIP_TcpConnStatusType*      tcpConnStatus);

/**
 * @brief        Handle alive check response.
 * @param[in]    soadTxPduRef: To call SoAd_GetSoConId.
 * @param[in]    txRxCtxIdx: Index of DoIP_TcpTxRxContext.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_HandleAliveCheckRsp(uint16 soadTxPduRef, uint8 txRxCtxIdx);

/* timer */

/**
 * @brief       Handle initial inactive timer.
 * @param[inout]   tcpConnStatus: Element of DoIP_TcpConnStatus.
 * @return      boolean
 * @retval      TRUE: The timer was processed.
 * @retval      FALSE: The timer was not processed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL boolean DoIP_HandleInitInactiveTimer(DoIP_TcpConnStatusType* tcpConnStatus);

/**
 * @brief        Handle general inactive timer.
 * @param[inout]   tcpConnStatus: Element of DoIP_TcpConnStatus.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_HandleGeneralInactiveTimer(DoIP_TcpConnStatusType* tcpConnStatus);

/**
 * @brief        Handle alive check timer.
 * @param[inout]   tcpConnStatus: Element of DoIP_TcpConnStatus.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
DOIP_LOCAL void DoIP_HandleAliveCheckTimer(DoIP_TcpConnStatusType* tcpConnStatus);

/* ========================================== external function definitions ========================================= */

#define DOIP_START_SEC_CODE
#include "DoIP_MemMap.h"

/* ----------------------------------------- connect ----------------------------------------- */

/**
 * @brief Handle routing activation request when close tcp connection.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
void DoIP_HandleRaWhenCloseTcpConnection(void)
/* PRQA S 1532 -- */
{
    if (DoIP_PendingRa.IsPending == TRUE)
    {
        DoIP_PendingRa.IsPending = FALSE;
        DoIP_HandleRaReq(DoIP_PendingRa.SoadTxPduRef, DoIP_PendingRa.TxRxCtxIdx);

        /*Reset this pending activation message, if it is successfully processed*/
        if (DoIP_PendingRa.IsPending == FALSE)
        {
            DoIP_ClearPendingRaMsg();
        }
    }
}

/* ----------------------------------------- pending ----------------------------------------- */

/**
 * @brief Pending non diagnostic message.
 */
/*queue for diag ack/nack, routing active rsp/alive check req, tcp nack**/
Std_ReturnType
    DoIP_EnqueueNonDiagMsg(DoIP_NonDiagQueueType* queue, PduIdType soadTxPduRef, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType ret = E_OK;

    /* PRQA S 2814,2934,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ComputeInvalidPtr,VL_DoIP_DerefInvalidPtr */
    if ((queue->Tail - queue->Head) != DOIP_NON_DIAG_QUEUE_BUFFER_NUM) /*buffer full*/
    {
        DoIP_NonDiagItemType* item = &queue->Item[queue->Tail % DOIP_NON_DIAG_QUEUE_BUFFER_NUM];

        item->SoadTxPduRef       = soadTxPduRef;
        item->PduInfo.SduLength  = pduInfoPtr->SduLength;
        item->PduInfo.SduDataPtr = item->PduData.SduDataPtr;
        DoIP_MemCpy(item->PduData.SduDataPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);

        queue->Tail++;
        /* PRQA S 2814,2934,2844 -- */
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}

/**
 * @brief Handle non diagnostic message in queued.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
boolean DoIP_HandlePendingNonDiagMsg(DoIP_TcpTxRxContextType* ctx)
/* PRQA S 1532 -- */
{
    boolean ret = TRUE;

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    const DoIP_NonDiagItemType* item = DoIP_DequeueNonDiagMsg(&ctx->TxNonDiagQueue);
    /* PRQA S 2814 -- */

    if (NULL_PTR != item)
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        if (E_OK == DoIP_TriggerTpTransmit(item->SoadTxPduRef, &item->PduInfo, &ctx->TxCtrl, DOIP_TX_NON_DIAG_MSG))
        /* PRQA S 2844 -- */
        {
            /* Prepare the data to be sent */
            DoIP_MemCpy(ctx->TxBuf, item->PduData.SduDataPtr, item->PduInfo.SduLength);
        }
        else
        {
            (void)DoIP_EnqueueNonDiagMsg(&ctx->TxNonDiagQueue, item->SoadTxPduRef, &item->PduInfo);
        }

        ret = FALSE;
    }

    return ret;
}

/**
 * @brief Clear pending routing activation request.
 */
void DoIP_ClearPendingRaMsg(void)
{
    DoIP_PendingRa.TxRxCtxIdx   = DOIP_INVALID_UINT8;
    DoIP_PendingRa.IsPending    = FALSE;
    DoIP_PendingRa.Sa           = DOIP_INVALID_UINT16;
    DoIP_PendingRa.SoConId      = DOIP_INVALID_UINT8;
    DoIP_PendingRa.SoadTxPduRef = DOIP_INVALID_UINT16;
}

/* ----------------------------------------- rcv ----------------------------------------- */

/**
 * @brief Handle tcp non diagnostic message.
 */
/* PRQA S 1532 ++ */ /* VL_DoIP_RefeOneFile */
void DoIP_HandleTcpNonDiagMsg(PduIdType soadTxPduRef, uint8 txRxCtxIdx, uint16 payloadType)
/* PRQA S 1532 -- */
{
    switch (payloadType)
    {
    case DOIP_MSG_TYPE_ALIVE_CHECK_RSP:

        DoIP_HandleAliveCheckRsp(soadTxPduRef, txRxCtxIdx);
        break;

    case DOIP_MSG_TYPE_ROUTING_ACTIVATION_REQ:

        DoIP_HandleRaReq(soadTxPduRef, txRxCtxIdx);
        break;

    default:
        /*do nothing*/
        break;
    }
}

/* ----------------------------------------- timer ----------------------------------------- */

/**
 * @brief Handle timers.
 */
void DoIP_HandleTcpTimeout(void) /* PRQA S 1532 */ /* VL_DoIP_RefeOneFile */
{
    boolean ret;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        ret = DoIP_HandleInitInactiveTimer(&DoIP_TcpConnStatus[i]);

        if (ret != TRUE)
        {
            DoIP_HandleGeneralInactiveTimer(&DoIP_TcpConnStatus[i]);

            DoIP_HandleAliveCheckTimer(&DoIP_TcpConnStatus[i]);
        }
    }
}

/* ========================================== internal function definitions ========================================= */

/* ----------------------------------------- pending ----------------------------------------- */

/**
 * @brief Get message from queue.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE DoIP_NonDiagItemType* DoIP_DequeueNonDiagMsg(DoIP_NonDiagQueueType* queue)
/* PRQA S 1505 -- */
{
    DoIP_NonDiagItemType* item = NULL_PTR;

    /* PRQA S 2814,2934 ++ */         /* VL_QAC_DerefNullPtr,VL_DoIP_ComputeInvalidPtr */
    if ((queue->Tail != queue->Head)) /*queueMsg not empty*/
    {
        item = &queue->Item[queue->Head % DOIP_NON_DIAG_QUEUE_BUFFER_NUM];
        queue->Head++;
        /* PRQA S 2814,2934 -- */
    }

    return item;
}

/* ----------------------------------------- routing activate ----------------------------------------- */

/**
 * @brief Check whether the SA is valid.
 */
DOIP_LOCAL_INLINE boolean DoIP_CheckSa(uint16 sa) /* PRQA S 1505 */ /* VL_DoIP_RefeOneFile */
{
    boolean ret = FALSE;

    for (uint8 i = 0u; i < DoIP_PCCfgPtr->TesterNum; i++) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
        if (DoIP_PBCfgPtr->TesterCfgArray[i].TesterSA == sa)
        /* PRQA S 2814,2824 -- */
        {
            ret = TRUE;

            break;
        }
    }

    return ret;
}

/**
 * @brief Check whether the routing activation type is valid.
 */
DOIP_LOCAL_INLINE boolean DoIP_CheckRaType(uint16 sa, uint8 activationType) /* PRQA S 1505 */ /* VL_DoIP_RefeOneFile */
{
    boolean ret = FALSE;

    uint8 raNum = DoIP_PCCfgPtr->RaNum; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */

    for (uint8 i = 0u; i < raNum; i++) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        /* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_ArithNullptr */
        if (DoIP_PBCfgPtr->RaCfgArray[i].RaType == activationType)
        /* PRQA S 2814,2824 -- */
        {
            ret = TRUE;
            break;
        }
    }

    const DoIP_PBTesterCfgType* testerCfg;

    if (ret == TRUE)
    {
        ret = FALSE;

        uint8                       testerNum      = DoIP_PCCfgPtr->TesterNum;
        const DoIP_PBTesterCfgType* testerCfgArray = DoIP_PBCfgPtr->TesterCfgArray;

        for (uint8 i = 0u; i < testerNum; i++)
        {
            if (testerCfgArray[i].TesterSA == sa) /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */
            {
                ret       = TRUE;
                testerCfg = &testerCfgArray[i];
                break;
            }
        }
    }

    if (ret == TRUE)
    {
        ret = FALSE;

        raNum                              = testerCfg->RaNum; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        const uint8*            raRefArray = testerCfg->RaRefArray;
        const DoIP_PBRaCfgType* raCfgArray = DoIP_PBCfgPtr->RaCfgArray;

        for (uint8 j = 0u; j < raNum; j++)
        {
            if (activationType == raCfgArray[raRefArray[j]].RaType) /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */
            {
                ret = TRUE;
                break;
            }
        }
    }

    return ret;
}

/**
 * @brief Routing activation authentication.
 */
DOIP_LOCAL_INLINE boolean DoIP_IsSucceedAuthentication(uint8 activationType) /* PRQA S 1505 */ /* VL_DoIP_RefeOneFile */
{
    boolean req = FALSE;

    const DoIP_PBRaCfgType* pbRa;

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->RaNum; i++)
    {
        pbRa = &DoIP_PBCfgPtr->RaCfgArray[i]; /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */

        if (pbRa->RaType == activationType)
        {
            /* PRQA S 2814 -- */
            /* PRQA S 2824 ++ */ /* VL_DoIP_ArithNullptr */
            const DoIP_RaAuthenticationCbkType* cbk = DoIP_PCCfgPtr->RaCfgArray[i].AuthenticationCbk;
            /* PRQA S 2824 -- */

            if ((NULL_PTR == cbk) || (NULL_PTR == cbk->RaAuthenticationFunc))
            {
                req = FALSE;
            }
            else
            {
                req = TRUE;
            }

            break;
        }
    }

    return req;
}

/**
 * @brief Routing activation confirmation.
 */
DOIP_LOCAL_INLINE boolean DoIP_IsSucceedConfirmation(uint8 activationType) /* PRQA S 1505 */ /* VL_DoIP_RefeOneFile */
{
    boolean req = FALSE;

    const DoIP_PBRaCfgType* pbRa;

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    for (uint8 i = 0u; (i < DoIP_PCCfgPtr->RaNum); i++)
    {
        pbRa = &DoIP_PBCfgPtr->RaCfgArray[i]; /* PRQA S 2824 */ /* VL_DoIP_ArithNullptr */

        if (pbRa->RaType == activationType)
        {
            /* PRQA S 2814 -- */
            /* PRQA S 2824 ++ */ /* VL_DoIP_ArithNullptr */
            const DoIP_RaConfirmationCbkType* cbk = DoIP_PCCfgPtr->RaCfgArray[i].ConfirmationCbk;
            /* PRQA S 2824 -- */

            if ((NULL_PTR == cbk) || (NULL_PTR == cbk->RaConfirmationFunc))
            {
                req = FALSE;
            }
            else
            {
                req = TRUE;
            }

            break;
        }
    }

    return req;
}

/**
 * @brief Register socket to DoIP_TcpConnStatus.
 */
/* PRQA S 1505 ++ */ /* VL_DoIP_RefeOneFile */
DOIP_LOCAL_INLINE void DoIP_RegisterSocket(uint8 index, SoAd_SoConIdType soConId, uint16 sa, uint16 ta)
/* PRQA S 1505 -- */
{
    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    DoIP_TcpConnStatus[index].SoConId              = soConId;
    DoIP_TcpConnStatus[index].Sa                   = sa;
    DoIP_TcpConnStatus[index].Ta                   = ta;
    DoIP_TcpConnStatus[index].InitInactiveTimer    = 0u;
    DoIP_TcpConnStatus[index].GeneralInactiveTimer = 0u;
    DoIP_TcpConnStatus[index].AuthState            = FALSE;
    DoIP_TcpConnStatus[index].ConfirmState         = FALSE;
    DoIP_TcpConnStatus[index].RaState              = DOIP_SOCKET_ACTIVATED;
    /* PRQA S 2844 -- */
}

/**
 * @brief Is SA already registered.
 */
DOIP_LOCAL boolean DoIP_CheckSaAlreadyRegistered(
    SoAd_SoConIdType                 soConId,
    uint16                           sa,
    uint8*                           rspCode,
    DoIP_SocketAssignmentResultType* socketAssignmentResult)
{
    boolean ret = FALSE;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        if ((DoIP_TcpConnStatus[i].SoConId == soConId) && (DoIP_TcpConnStatus[i].RaState == DOIP_SOCKET_ACTIVATED))
        {
            if (DoIP_TcpConnStatus[i].Sa == sa)
            {
                *socketAssignmentResult = DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            }
            else
            {
                /* req DoIP-106 */
                /* SA different from the table connection entry was received on the already activated TCP_DATA socket.
                 */
                *rspCode = DOIP_RA_DENIED_SA_NOT_MATCH; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */

                *socketAssignmentResult = DOIP_SOCKET_ASSIGNMENT_FAILED;
            }

            ret = TRUE;

            break;
        }
    }

    return ret;
}

/**
 * @brief Send alive check request.
 */
DOIP_LOCAL boolean DoIP_StartAliveCheck(uint16 sa, DoIP_SocketAssignmentResultType* socketAssignmentResult)
{
    boolean ret = FALSE;

    uint8 index;
    if ((E_OK == DoIP_GetTcpConnStatusIdxBySa(sa, &index))
        && (DOIP_SOCKET_ACTIVATED == DoIP_TcpConnStatus[index].RaState)) /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
    {
        /*SWS_DoIP_00105*/
        DoIP_StartSingleAliveCheck(sa);

        ret                     = TRUE;
        *socketAssignmentResult = DOIP_SOCKET_ASSIGNMENT_PENDING; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    }

    uint8 numRegisteredSockets = 0u;

    if (ret == FALSE)
    {
        /* Figure 13 in ISO13400-2 */
        for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
        {
            if (DOIP_SOCKET_ACTIVATED == DoIP_TcpConnStatus[i].RaState)
            {
                numRegisteredSockets++;
            }
        }

        /* req DoIP-094 */
        if (numRegisteredSockets >= DOIP_MAX_TESTER_CONNECTION)
        {
            DoIP_StartAllAliveCheck();

            ret                     = TRUE;
            *socketAssignmentResult = DOIP_SOCKET_ASSIGNMENT_PENDING;
        }
    }

    return ret;
}

/**
 * @brief Assign socket.
 */
DOIP_LOCAL DoIP_SocketAssignmentResultType
    DoIP_AssignSocket(SoAd_SoConIdType soConId, uint16 sa, uint16 ta, uint8* rspCode)
{
    DoIP_SocketAssignmentResultType ret = DOIP_SOCKET_ASSIGNMENT_FAILED;

    boolean condition = DoIP_CheckSaAlreadyRegistered(soConId, sa, rspCode, &ret);

    if (condition != TRUE)
    {
        condition = DoIP_StartAliveCheck(sa, &ret);
    }

    if (condition != TRUE) /* No alive check */
    {
        /* look up a free socket.*/
        for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
        {
            if ((DOIP_SOCKET_NONACTIVATED == DoIP_TcpConnStatus[i].RaState)
                && (DoIP_TcpConnStatus[i].SoConId == soConId))
            {
                /** req DoIP-090 */
                DoIP_RegisterSocket(i, soConId, sa, ta);

                ret = DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL;
            }
        }
    }

    return ret;
}

/**
 * @brief Send routing activation response message.
 */
DOIP_LOCAL void DoIP_SendRaRsp(uint16 soadTxPduRef, uint16 sa, uint8 raRspCode, uint8 txRxCtxIdx)
{
    /** todo*/ /** Consider whether the oem exists */
    uint8 buf[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE] = {0};

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;
    DoIP_u16_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_ROUTING_ACTIVATION_RSP);
    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], DOIP_RA_RSP_MSG_PAYLOAD_LEN);
    DoIP_u16_2_u8(&buf[DOIP_RA_RSP_MSG_FIELD_POSITION_TESTER_SA], sa);
    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    DoIP_u16_2_u8(&buf[DOIP_RA_RSP_MSG_FIELD_POSITION_SERVER_SA], DoIP_PBCfgPtr->LogicalAddress);
    /* PRQA S 2814 -- */
    buf[DOIP_RA_RSP_MSG_FIELD_POSITION_CODE] = raRspCode;

    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = DOIP_RA_RSP_MSG_LEN;

    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
    /* PRQA S 2844 -- */

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if ((txCtrl->TxBufState == DOIP_BUFFER_IDLE) && (txCtrl->TxState == DOIP_TX_STATE_IDLE))
    /* PRQA S 2814 -- */
    {
        DoIP_MemCpy(
            DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf,
            buf,
            pduInfo.SduLength); /*aaa,when dequeue,copy to internal buffer?*/
        (void)DoIP_TriggerTpTransmit(soadTxPduRef, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        (void)DoIP_EnqueueNonDiagMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxNonDiagQueue, soadTxPduRef, &pduInfo);
        /* PRQA S 2844 -- */
    }
}

/**
 * @brief Check whether the routing activation request is valid.
 */
DOIP_LOCAL boolean DoIP_IsValidRaReq(uint16 sa, uint16* ta, uint8 activationType, uint8* raRspCode)
{
    boolean ret = FALSE;

    if (DoIP_CheckSa(sa) == TRUE)
    {
        if (DoIP_CheckRaType(sa, activationType) == TRUE)
        {
            if (DoIP_GetTaFromChannel(sa, ta) == E_OK)
            {
                ret = TRUE;
            }
        }
        else
        {
            *raRspCode = DOIP_RA_DENIED_UNSUPPORTED_TYPE; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        }
    }

    return ret;
}

/**
 * @brief Handle routing activation request.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_DoIP_STCAL */
DOIP_LOCAL void DoIP_HandleRaReq(PduIdType soadTxPduRef, uint8 txRxCtxIdx)
{
    boolean condition = FALSE;
    uint8   raRspCode = DOIP_RA_DENIED_UNKNOWN_SA; /* SWS_DoIP_00104 */
    /* PRQA S 2844 ++ */                           /* VL_DoIP_DerefInvalidPtr */
    uint8 activationType = DoIP_TcpTxRxContext[txRxCtxIdx].RxBuf[DOIP_RA_REQ_MSG_FILED_POSITION_ACTIVATION_TYPE];
    /* PRQA S 2844 -- */
    uint16 sa = DOIP_U8_2_U16(&DoIP_TcpTxRxContext[txRxCtxIdx].RxBuf[DOIP_RA_REQ_MSG_FIELD_POSITION_SA]);
    uint16 ta;

    SoAd_SoConIdType soConId;
    (void)SoAd_GetSoConId(soadTxPduRef, &soConId);

    uint8 index;
    if (DoIP_GetTcpConnStatusIdxBySoConId(soConId, &index) == E_OK)
    {
        if (DoIP_IsValidRaReq(sa, &ta, activationType, &raRspCode) == TRUE)
        {
            condition = TRUE;
        }
    }

    boolean isPending = FALSE;

    if (condition == TRUE)
    {
        /*ISO13400-2 7.2.4*/
        DoIP_SocketAssignmentResultType ret = DoIP_AssignSocket(soConId, sa, ta, &raRspCode);
        if (DOIP_SOCKET_ASSIGNMENT_SUCCESSFUL == ret)
        {
            raRspCode                        = DOIP_RA_SUCCESS;
            DoIP_TcpConnStatus[index].RaType = activationType; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */

            if (TRUE == DoIP_IsSucceedAuthentication(activationType))
            {
                (void)DoIP_IsSucceedConfirmation(activationType);
            }
        }
        else if (DOIP_SOCKET_ASSIGNMENT_PENDING == ret)
        {
            if (DoIP_PendingRa.IsPending == FALSE)
            {
                DoIP_PendingRa.Sa           = sa;
                DoIP_PendingRa.TxRxCtxIdx   = txRxCtxIdx;
                DoIP_PendingRa.SoConId      = soConId;
                DoIP_PendingRa.SoadTxPduRef = soadTxPduRef;
                DoIP_PendingRa.IsPending    = TRUE;
            }

            isPending = TRUE;
        }
        else /* DOIP_SOCKET_ASSIGNMENT_FAILED */
        {
        }
    }

    if (isPending != TRUE)
    {
        DoIP_SendRaRsp(soadTxPduRef, sa, raRspCode, txRxCtxIdx);
        DoIP_ResetTcpRxContext(txRxCtxIdx);

        /*ISO13400-2 Table 25*/
        if (raRspCode != DOIP_RA_SUCCESS)
        {
            DoIP_TcpConnStatus[index].WaitCloseSocket = TRUE; /* PRQA S 2844 */ /* VL_DoIP_DerefInvalidPtr */
        }
    }
}
/* PRQA S 6070 -- */

/* ----------------------------------------- alive check ----------------------------------------- */

/**
 * @brief Send alive check request.
 */
DOIP_LOCAL void DoIP_SendAliveCheckReq(uint16 soadTxPduRef, uint8 txRxCtxIdx)
{
    uint8 buf[DOIP_NON_DIAG_QUEUE_BUFFER_SIZE];

    buf[DOIP_HEADER_FIELD_POSITION_VERSION]         = DOIP_PROTOCOL_VERSION;
    buf[DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION] = DOIP_PROTOCOL_INVERSE_VERSION;

    /* payload type */
    DoIP_u16_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE], DOIP_MSG_TYPE_ALIVE_CHECK_REQ);

    /* payload len */
    DoIP_u32_2_u8(&buf[DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN], DOIP_ALIVE_CHECK_REQ_MSG_PAYLOAD_LEN);

    PduInfoType pduInfo;
    pduInfo.SduDataPtr  = buf;
    pduInfo.MetaDataPtr = NULL_PTR;
    pduInfo.SduLength   = DOIP_ALIVE_CHECK_REQ_MSG_LEN;

    /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
    DoIP_TcpTxCtrlType* txCtrl = &DoIP_TcpTxRxContext[txRxCtxIdx].TxCtrl;
    /* PRQA S 2844 -- */

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if ((txCtrl->TxBufState == DOIP_BUFFER_IDLE) && (txCtrl->TxState == DOIP_TX_STATE_IDLE))
    /* PRQA S 2814 -- */
    {
        DoIP_MemCpy(
            DoIP_TcpTxRxContext[txRxCtxIdx].TxBuf,
            buf,
            pduInfo.SduLength); /*aaa,when dequeue,copy to internal buffer?*/

        (void)DoIP_TriggerTpTransmit(soadTxPduRef, &pduInfo, txCtrl, DOIP_TX_NON_DIAG_MSG);
    }
    else
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        (void)DoIP_EnqueueNonDiagMsg(&DoIP_TcpTxRxContext[txRxCtxIdx].TxNonDiagQueue, soadTxPduRef, &pduInfo);
        /* PRQA S 2844 -- */
    }
}

/**
 * @brief Send alive check request to one tester.
 */
DOIP_LOCAL void DoIP_StartSingleAliveCheck(uint16 sa)
{
    for (uint8 i = 0u; i < DoIP_PCCfgPtr->ConnsCfg->TcpConnNum; i++) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
        if ((DoIP_TcpConnStatus[i].Sa == sa) && (FALSE == DoIP_TcpConnStatus[i].WaitAliveCheckRsp))
        {
            DoIP_TcpConnStatus[i].WaitAliveCheckRsp    = TRUE;
            DoIP_TcpConnStatus[i].GeneralInactiveTimer = 0u;
            DoIP_TcpConnStatus[i].AliveCheckTimer      = 0u;
            /* PRQA S 2844 -- */ /* VL_DoIP_DerefInvalidPtr */

            /* PRQA S 2814,2844 ++ */ /* VL_QAC_DerefNullPtr,VL_DoIP_DerefInvalidPtr */
            DoIP_SendAliveCheckReq(DOIP_TCP_PBCON_CFG(i)->SoAdTxPdu.SoAdTxPduRef, DoIP_TcpConnStatus[i].TxRxCtxIdx);
            /* PRQA S 2814,2844 -- */

            break;
        }
    }
}

/**
 * @brief Send alive check request to all tester.
 */
DOIP_LOCAL void DoIP_StartAllAliveCheck(void)
{
    boolean isConned = FALSE;

    for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
    {
        /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
        for (uint8 j = 0u; j < DoIP_PCCfgPtr->ChannelNum; j++)
        {
            /* PRQA S 2824 ++ */ /* VL_DoIP_ArithNullptr */
            if (DoIP_TcpConnStatus[i].Sa == DoIP_PBCfgPtr->ChannelCfgArray[j].ChannelSaRef->TesterSA)
            /* PRQA S 2824 -- */
            {
                /* PRQA S 2814 -- */
                isConned = TRUE;
                break;
            }
        }

        if ((DOIP_SOCKET_ACTIVATED == DoIP_TcpConnStatus[i].RaState) && (TRUE == isConned))
        {
            /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
            for (uint8 j = 0u; j < DoIP_PCCfgPtr->ConnsCfg->TcpConnNum; j++)
            {
                if (DOIP_TCP_PBCON_CFG(j)->SoAdRxPdu.SoAdRxPduId == DoIP_TcpConnStatus[i].SoadRxPduId)
                {
                    DoIP_TcpConnStatus[i].WaitAliveCheckRsp    = TRUE;
                    DoIP_TcpConnStatus[i].GeneralInactiveTimer = 0u;
                    DoIP_TcpConnStatus[i].AliveCheckTimer      = 0u;

                    DoIP_SendAliveCheckReq(
                        DOIP_TCP_PBCON_CFG(j)->SoAdTxPdu.SoAdTxPduRef,
                        DoIP_TcpConnStatus[i].TxRxCtxIdx);
                    /* PRQA S 2814 -- */
                }
            }
        }
    }
}

/**
 * @brief Handle pending routing activation request.
 */
DOIP_LOCAL void DoIP_HandlePendingRaReq(
    uint16                       sa,
    uint8                        txRxCtxIdx,
    const DoIP_TcpConnStatusType tcpConnStatusArray[],
    DoIP_TcpConnStatusType*      tcpConnStatus)
{
    uint16 remainingConnections = DOIP_MAX_TESTER_CONNECTION;
    for (uint8 j = 0u; j < DOIP_MAX_TESTER_CONNECTION; j++)
    {
        if ((TRUE != tcpConnStatusArray[j].WaitAliveCheckRsp)
            && (tcpConnStatusArray[j].RaState == DOIP_SOCKET_ACTIVATED)) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        {
            remainingConnections--;
        }
    }

    uint8 raRspCode = DOIP_INVALID_UINT8;
    if ((0u == remainingConnections) && (DoIP_PendingRa.IsPending == TRUE))
    {
        /* all concurrently supported TCP_DATA sockets are registered and active. */
        raRspCode = DOIP_RA_DENIED_NO_FREE_SOCKET;
    }
    else if (
        (DoIP_PendingRa.IsPending == TRUE) && (sa == DoIP_PendingRa.Sa)
        && (tcpConnStatus->RaState == DOIP_SOCKET_ACTIVATED))
    {
        /* the SA is already registered and active on a different TCP_DATA socket.*/
        raRspCode = DOIP_RA_DENIED_SA_ALREADY_REGISTERED;
    }
    else
    {
        /* nothing */
    }

    if (raRspCode != DOIP_INVALID_UINT8)
    {
        DoIP_SendRaRsp(DoIP_PendingRa.SoadTxPduRef, DoIP_PendingRa.Sa, raRspCode, DoIP_PendingRa.TxRxCtxIdx);

        DoIP_ResetTcpRxContext(txRxCtxIdx);

        /* close pending connection */
        for (uint8 j = 0u; j < DOIP_TCP_MAX_CONNECTION; j++)
        {
            if (tcpConnStatus->SoConId == DoIP_PendingRa.SoConId)
            {
                tcpConnStatus->WaitCloseSocket = TRUE;
            }
        }

        DoIP_ClearPendingRaMsg();
    }
}

/**
 * @brief Handle alive check response.
 */
DOIP_LOCAL void DoIP_HandleAliveCheckRsp(uint16 soadTxPduRef, uint8 txRxCtxIdx)
{
    SoAd_SoConIdType soConId;

    if (E_OK == SoAd_GetSoConId(soadTxPduRef, &soConId))
    {
        boolean isSaRegeisted = FALSE;
        /* PRQA S 2934 ++ */ /* VL_DoIP_ComputeInvalidPtr */
        const DoIP_TcpTxRxContextType* ctx = &DoIP_TcpTxRxContext[txRxCtxIdx];
        /* PRQA S 2934 -- */

        for (uint8 i = 0u; i < DOIP_TCP_MAX_CONNECTION; i++)
        {
            /* PRQA S 2844 ++ */ /* VL_DoIP_DerefInvalidPtr */
            if ((DoIP_TcpConnStatus[i].SoConId == soConId)
                && (ctx->RxCtrl.PayloadLen == DOIP_ALIVE_CHECK_RSP_MSG_PAYLOAD_LEN))
            /* PRQA S 2844 -- */
            {
                /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
                uint16 sa = (uint16)((uint16)ctx->RxBuf[DOIP_ALIVE_CHECK_RSP_MSG_FIELD_POSITION_SA] << 8u)
                            + (uint16)ctx->RxBuf[DOIP_ALIVE_CHECK_RSP_MSG_FIELD_POSITION_SA + 1u];
                /* PRQA S 3120 -- */

                /*SWS_DoIP_00141*/
                if ((sa == DoIP_TcpConnStatus[i].Sa) && (DoIP_TcpConnStatus[i].RaState == DOIP_SOCKET_ACTIVATED))
                {
                    DoIP_TcpConnStatus[i].GeneralInactiveTimer = 0u;
                    DoIP_TcpConnStatus[i].WaitAliveCheckRsp    = FALSE;

                    isSaRegeisted = TRUE;
                }

                DoIP_HandlePendingRaReq(sa, txRxCtxIdx, DoIP_TcpConnStatus, &DoIP_TcpConnStatus[i]);

                break;
            }
        }

        if (FALSE == isSaRegeisted)
        {
            DoIP_CloseTcpConnection(soConId);
            DoIP_CloseSoCon(soConId, TRUE);
        }
        DoIP_ResetTcpRxContext(txRxCtxIdx);
    }
}

/* ----------------------------------------- timer ----------------------------------------- */

/**
 * @brief Handle initial inactive timer.
 */
DOIP_LOCAL boolean DoIP_HandleInitInactiveTimer(DoIP_TcpConnStatusType* tcpConnStatus)
{
    boolean ret = FALSE;

    boolean isTimeout = FALSE;

    SchM_Enter_DoIP_ExclusiveArea();

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if ((tcpConnStatus->SoConState == SOAD_SOCON_ONLINE) && (tcpConnStatus->RaState != DOIP_SOCKET_ACTIVATED))
    /* PRQA S 2814 -- */
    {
        if (tcpConnStatus->InitInactiveTimer >= DOIP_INITIAL_INACTIVE_TIME) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            isTimeout = TRUE;
        }
        else
        {
            tcpConnStatus->InitInactiveTimer += DOIP_MAIN_FUNCTION_PERIOD; /* PRQA S 1252 */ /* VL_QAC_1252 */
        }

        ret = TRUE;
    }

    SchM_Exit_DoIP_ExclusiveArea();

    if (isTimeout == TRUE)
    {
        /*SWS_DoIP_00144*/
        SoAd_SoConIdType soConId = tcpConnStatus->SoConId;
        DoIP_CloseTcpConnection(soConId);
        DoIP_CloseSoCon(soConId, TRUE);
    }

    return ret;
}

/**
 * @brief Handle general inactive timer.
 */
DOIP_LOCAL void DoIP_HandleGeneralInactiveTimer(DoIP_TcpConnStatusType* tcpConnStatus)
{
    boolean isTimeout = FALSE;

    SchM_Enter_DoIP_ExclusiveArea(); /* PRQA S 3141,3138 */ /* VL_DoIP_NullStatement */

    /* PRQA S 2814 ++ */ /* VL_QAC_DerefNullPtr */
    if ((tcpConnStatus->SoConState == SOAD_SOCON_ONLINE) && (tcpConnStatus->RaState == DOIP_SOCKET_ACTIVATED))
    /* PRQA S 2814 -- */
    {
        /*SWS_DoIP_00146*/
        if (tcpConnStatus->GeneralInactiveTimer >= DOIP_GENERAL_INACTIVE_TIME) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            isTimeout = TRUE;
        }
        else
        {
            tcpConnStatus->GeneralInactiveTimer += DOIP_MAIN_FUNCTION_PERIOD; /* PRQA S 1252 */ /* VL_QAC_1252 */
        }
    }

    SchM_Exit_DoIP_ExclusiveArea(); /* PRQA S 3141,3138 */ /* VL_DoIP_NullStatement */

    if (isTimeout == TRUE)
    {
        SoAd_SoConIdType soConId = tcpConnStatus->SoConId;
        DoIP_CloseTcpConnection(soConId);
        DoIP_CloseSoCon(soConId, TRUE);
    }
}

/**
 * @brief Handle alive check timer.
 */
DOIP_LOCAL void DoIP_HandleAliveCheckTimer(DoIP_TcpConnStatusType* tcpConnStatus)
{
    boolean isTimeout = FALSE;

    SchM_Enter_DoIP_ExclusiveArea(); /* PRQA S 3141,3138 */ /* VL_DoIP_NullStatement */

    if (tcpConnStatus->WaitAliveCheckRsp == TRUE) /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
    {
        if (tcpConnStatus->AliveCheckTimer >= DOIP_ALIVE_CHECK_RESPONSE_TIMEOUT) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            isTimeout = TRUE;
        }
        else
        {
            tcpConnStatus->AliveCheckTimer += DOIP_MAIN_FUNCTION_PERIOD; /* PRQA S 1252 */ /* VL_QAC_1252 */
        }
    }

    SchM_Exit_DoIP_ExclusiveArea(); /* PRQA S 3141,3138 */ /* VL_DoIP_NullStatement */

    if (isTimeout == TRUE)
    {
        /* tcpConnStatus is reset in DoIP_CloseTcpConnection, so soConId is required */
        SoAd_SoConIdType soConId = tcpConnStatus->SoConId;
        DoIP_CloseTcpConnection(soConId);
        DoIP_CloseSoCon(soConId, TRUE);
    }
}

#define DOIP_STOP_SEC_CODE
#include "DoIP_MemMap.h"
