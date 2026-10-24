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
 **  @file               : PduR_Route.c
 **  @author             : xiaojian.liang
 **  @date               : 2023/12/5
 **  @vendor             : isoft
 **  @description        : Implementation for PDUR Inter-Partition Routing
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "PduR_Internal.h"

#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definition ============================================ */
#if ((PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_IfRxIndBufferedDirect(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfo);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_IfTxConfBufferedDirect(PduIdType srcPduId, PduIdType destPduId);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */
#if (                                                           \
    (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON) \
    || (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON))
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static Std_ReturnType PduR_IfTransmitForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static Std_ReturnType PduR_TriggerTransmitForward(PduIdType srcPduId, PduIdType destPduId, PduInfoType* pduInfoPtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_IfTxConfirmationForward(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_IfRxIndicationForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static Std_ReturnType PduR_TpTransmitForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static BufReq_ReturnType PduR_CopyTxDataForward(
    PduIdType            srcPduId,
    PduIdType            destPduId,
    const PduInfoType*   pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType*       availableDataPtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_TpTxConfirmationForward(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_EvaluateUpLayerConfirmationStatus(PduIdType srcPduId);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_TpDestinationProcess(PduIdType srcPduId, PduIdType destPduId, uint8 apiId);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

/* PRQA S 5016 ++ */ /* VL_PduR_5016 */
static inline const PduR_BswModuleType* PduR_GetBswModulePtrOfSrcPdu(PduIdType srcPduId);
static inline const PduR_BswModuleType* PduR_GetBswModulePtrOfDestPdu(PduIdType destPduId);
/* PRQA S 5016 -- */ /* VL_PduR_5016 */
/* ========================================== external function definition ========================================== */
/* PRQA S 3672 ++ */ /* VL_PduR_FuncPtrNonConst */
#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
/**
 * PduR interface transmit implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType PduR_PduRIfTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 1532 -- */
{
    Std_ReturnType retval = E_NOT_OK;
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
    retval                                 = PduR_IfTransmitForward(srcPduId, destPduId, pduInfoPtr);
    if ((retval == E_NOT_OK) || (!PduR_GetTransmissionConfirmationOfDestPdu(destPduId)))
    {
        PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
    }
#else
    PDUR_NOUSED(srcPduId);
    PDUR_NOUSED(destPduId);
    PDUR_NOUSED(pduInfoPtr);
#endif
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
/**
 * PduR transport protocal transmit implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType PduR_PduRTpTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 1532 -- */
{
    Std_ReturnType retval = E_NOT_OK;

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
    retval = PduR_TpTransmitForward(srcPduId, destPduId, pduInfoPtr);
#if STD_ON == PDUR_MULTICAST_TP_TRANSMIT_SUPPORT
    if (retval != E_NOT_OK)
    {
        PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
    }
    else
    {
        PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
    }
#endif
#else
    PDUR_NOUSED(srcPduId);
    PDUR_NOUSED(destPduId);
    PDUR_NOUSED(pduInfoPtr);
#endif
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
/**
 * PduR cancel transmit implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType PduR_PduRCancelTransmit(PduIdType srcPduId, PduIdType destPduId)
/* PRQA S 1532 -- */
{
    Std_ReturnType                       retval            = E_NOT_OK;
    PduR_RouteType                       routeType         = PduR_GetRouteTypeOfDestPdu(destPduId);
    const PduR_BswModuleType*            bswModulePtr      = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType                            bswModulePduId    = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    PduR_ModuleIfTpCancelTransmitApiType cancelTransmitApi = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
#endif

    switch (routeType)
    {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_TX_NOBUFFERED:
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
        if (bswModulePtr->IfCancelTransmitApi != NULL_PTR)
        {
            cancelTransmitApi = bswModulePtr->IfCancelTransmitApi[funcIndex];
        }
#else
        cancelTransmitApi = bswModulePtr->IfCancelTransmitApi;
#endif
        if (cancelTransmitApi != NULL_PTR)
        {
            retval = cancelTransmitApi(bswModulePduId);
        }
        break;
#endif /* end of PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED */
#if (PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON)
    case PDUR_ROUTE_TP_TX_NOBUFFERED_UNICAST:
    case PDUR_ROUTE_TP_TX_NOBUFFERED_MULTICAST:
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
        if (bswModulePtr->TpCancelTransmitApi != NULL_PTR)
        {
            cancelTransmitApi = bswModulePtr->TpCancelTransmitApi[funcIndex];
        }
#else
        cancelTransmitApi = bswModulePtr->TpCancelTransmitApi;
#endif
        if (cancelTransmitApi != NULL_PTR)
        {
            retval = cancelTransmitApi(bswModulePduId);
        }
        break;
#endif /* end of PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED */
    default:
        /* do nothing */
        break;
    }
    PDUR_NOUSED(srcPduId);
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_CANCEL_RECEIVE == STD_ON)
/**
 * PduR cancel receive implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType PduR_PduRCancelReceive(PduIdType srcPduId, PduIdType destPduId)
/* PRQA S 1532 -- */
{
    Std_ReturnType retval = E_NOT_OK;
#if (PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON)
    PduR_RouteType                  routeType           = PduR_GetRouteTypeOfDestPdu(destPduId);
    const PduR_BswModuleType*       bswModulePtr        = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduR_ModuleCancelReceiveApiType cancelReceiveApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    if (bswModulePtr->CancelReceiveApi != NULL_PTR)
    {
        uint8 funcIndex     = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, FALSE);
        cancelReceiveApiPtr = bswModulePtr->CancelReceiveApi[funcIndex];
    }
#else
    cancelReceiveApiPtr = bswModulePtr->CancelReceiveApi;
#endif
    if ((routeType == PDUR_ROUTE_TP_RX_NOBUFFERED) && (cancelReceiveApiPtr != NULL_PTR))
    {
        PduIdType bswModulePduId = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
        retval                   = cancelReceiveApiPtr(bswModulePduId);
    }
#else
    PDUR_NOUSED(srcPduId);
    PDUR_NOUSED(destPduId);
#endif
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_RX_INDICATION == STD_ON)
/**
 * PduR interface receive indication implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532, 6070, 6010 ++ */ /* VL_QAC_OneFunRef, VL_MTR_PduR_STCAL, VL_MTR_PduR_STCYC */
void PduR_PduRIfRxIndication(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 1532, 6070, 6010 -- */
{
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);

    switch (routeType)
    {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_RX_NOBUFFERED:
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_INITIAL)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
            PduR_IfRxIndicationForward(srcPduId, destPduId, pduInfoPtr);
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        else
        {
            /* SWS_PduR_00903 In n:1,the most recent request has not returned,the PDU Router shall return immediately */
            PduR_Det_ReportRuntimeError(PDUR_IFRXINDICATION_ID, PDUR_E_PDU_INSTANCES_LOST);
        }
        break;
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    case PDUR_ROUTE_IF_RX_BUFFERED:
    {
        PduR_QueueRuntimeType* queuePtr;
        uint8                  metaDataLength;
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_INITIAL)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
            PduR_IfRxIndicationForward(srcPduId, destPduId, pduInfoPtr);

            queuePtr       = PduR_GetQueuePtrOfDestPdu(destPduId);
            metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
            while (!PduR_QueueIsEmpty(queuePtr)) /* PRQA S 2487 */ /* VL_PduR_2487 */
            {
                PduInfoType pduInfoNext;
                SchM_Enter_PduR_ExclusiveArea_Route();
                const PduR_BufferType* headBufferPtr = PduR_QueueGetHead(queuePtr);
                PduR_Buffer2PduInfo(headBufferPtr, &pduInfoNext, metaDataLength, 0u);
                PduR_QueuePopFront(queuePtr);
                SchM_Exit_PduR_ExclusiveArea_Route();
                PduR_IfRxIndicationForward(srcPduId, destPduId, &pduInfoNext);
            }
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        else
        {
            queuePtr       = PduR_GetQueuePtrOfDestPdu(destPduId);
            metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
            SchM_Enter_PduR_ExclusiveArea_Route();
            (void)PduR_QueueStorePduInfo(queuePtr, metaDataLength, pduInfoPtr, PDUR_IFRXINDICATION_ID);
            SchM_Exit_PduR_ExclusiveArea_Route();
        }
        break;
    }
#endif
#endif
#if (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_GW_NOBUFFERED:
    {
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_INITIAL)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
            PduInfoType pduInfoPtrVar              = *pduInfoPtr;
            /* The rest of the received I-PDU shall be dropped if the configured length of the destination I-PDU is
            smaller than the received data length (PduLength of received I-PDU)(SWS_PduR_00783) */
            pduInfoPtrVar.SduLength = (PduR_GetConfiguredLengthOfDestPdu(destPduId) < pduInfoPtrVar.SduLength)
                                          ? PduR_GetConfiguredLengthOfDestPdu(destPduId)
                                          : pduInfoPtrVar.SduLength;
            (void)PduR_IfTransmitForward(srcPduId, destPduId, &pduInfoPtrVar);
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        else
        {
            /* SWS_PduR_00903 In n:1,the most recent request has not returned,the PDU Router shall return immediately */
            PduR_Det_ReportRuntimeError(PDUR_IFRXINDICATION_ID, PDUR_E_PDU_INSTANCES_LOST);
        }
        break;
    }
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    case PDUR_ROUTE_IF_GW_BUFFERED:
    {
        PduInfoType pduInfoPtrVar = *pduInfoPtr;
        /* The rest of the received I-PDU shall be dropped if the configured length of the destination I-PDU is smaller
           than the received data length (PduLength of received I-PDU) (SWS_PduR_00746) */
        pduInfoPtrVar.SduLength = (PduR_GetConfiguredLengthOfDestPdu(destPduId) < pduInfoPtrVar.SduLength)
                                      ? PduR_GetConfiguredLengthOfDestPdu(destPduId)
                                      : pduInfoPtrVar.SduLength;
        if (PduR_GetDataProvisionOfDestPdu(destPduId) == PDUR_TRIGGERTRANSMIT)
        {
            PduR_QueueRuntimeType* queuePtr       = PduR_GetQueuePtrOfDestPdu(destPduId);
            uint8                  metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
            /* Enqueue to FIFO and call <DstLo>_Transmit if FIFO queue is empty (SWS_PduR_00786) */
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_QueueStoreType retval =
                PduR_QueueStorePduInfo(queuePtr, metaDataLength, &pduInfoPtrVar, PDUR_IFRXINDICATION_ID);
            const PduR_BufferType* tailBufferPtr = PduR_QueueGetTail(PDUR_PDU_ID_INVALID, queuePtr);
            const PduR_BufferType* headBufferPtr = PduR_QueueGetHead(queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
            if ((retval != PDUR_QUEUESTORE_NOT_OK) && (headBufferPtr == tailBufferPtr))
            {
                (void)PduR_IfTransmitForward(srcPduId, destPduId, &pduInfoPtrVar);
            }
        }
        /* PduR_GetDataProvisionOfDestPdu(destPduId) == PDUR_DIRECT */
        else
        {
            PduR_IfRxIndBufferedDirect(srcPduId, destPduId, &pduInfoPtrVar);
        }
        break;
    }
#endif
#endif /* end of PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED */
    default:
        /* do nothing */
        break;
    }
    PDUR_NOUSED(srcPduId);
    PDUR_NOUSED(pduInfoPtr);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if ((PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
/**
 * PduR interface receive indication implement for IF gateWay with PDUR_DIRECT
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_PduR_STCAL */
static void PduR_IfRxIndBufferedDirect(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfo)
/* PRQA S 6070 -- */
{
    PduR_QueueRuntimeType* queuePtr       = PduR_GetQueuePtrOfDestPdu(destPduId);
    uint8                  metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
    const PduInfoType*     pduInfoPtr     = pduInfo;
    if ((PduR_DestinationRouteStatus[destPduId] != PDUR_RES_PENDING) || (queuePtr->Depth == 1u))
    {
        PduInfoType pduInfoNext;
        SchM_Enter_PduR_ExclusiveArea_Route();
        const PduR_BufferType* headBufferPtr = PduR_QueueGetHead(queuePtr);
        if ((headBufferPtr == NULL_PTR) || (queuePtr->Depth == 1u))
        {
            /* transmit current data */
            pduInfoNext = *pduInfo;
            /* last is best */
            if (queuePtr->Depth == 1u)
            {
                PduR_QueuePopFront(queuePtr);
            }
        }
        else
        {
            /* transmit the oldest buffer data in FIFO, don't store the data even the transmit not ok */
            PduR_Buffer2PduInfo(headBufferPtr, &pduInfoNext, metaDataLength, 0u);
            PduR_QueuePopFront(queuePtr);
            (void)PduR_QueueStorePduInfo(queuePtr, metaDataLength, pduInfoPtr, PDUR_IFRXINDICATION_ID);
        }
        SchM_Exit_PduR_ExclusiveArea_Route();
        if (E_OK == PduR_IfTransmitForward(srcPduId, destPduId, &pduInfoNext))
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
        }
        else if (queuePtr->Depth > 1u)
        {
            PduR_Det_ReportRuntimeError(PDUR_IFRXINDICATION_ID, PDUR_E_PDU_INSTANCES_LOST);
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        SchM_Enter_PduR_ExclusiveArea_Route();
        if (PDUR_QUEUESTORE_FLUSH_OK
            == PduR_QueueStorePduInfo(queuePtr, metaDataLength, pduInfoPtr, PDUR_IFRXINDICATION_ID))
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        SchM_Exit_PduR_ExclusiveArea_Route();
    }
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TX_CONFIRMATION == STD_ON)
/**
 * PduR interface transmit confirmation implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532, 6030 ++ */ /* VL_QAC_OneFunRef, VL_MTR_PduR_STMIF */
void PduR_PduRIfTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
/* PRQA S 1532, 6030 -- */
{
    PduR_RouteType routeType      = PduR_GetRouteTypeOfDestPdu(destPduId);
    boolean        isFifoNotEmpty = FALSE;
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    if (routeType == PDUR_ROUTE_IF_GW_BUFFERED)
    {
        const PduR_QueueRuntimeType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
        isFifoNotEmpty                        = (PduR_QueueGetHead(queuePtr) == NULL_PTR) ? FALSE : TRUE;
    }
#endif
    if ((PduR_DestinationRouteStatus[destPduId] == PDUR_RES_PENDING) || isFifoNotEmpty)
    {
        PduR_DestinationRouteStatus[destPduId] = (result == E_OK) ? PDUR_RES_OK : PDUR_RES_NOT_OK;

        switch (routeType)
        {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
        case PDUR_ROUTE_IF_TX_NOBUFFERED:
        {
            SchM_Enter_PduR_ExclusiveArea_Route();
            boolean              finishFlag     = PduR_EvaluateUpLayerConfirmationStatus(srcPduId);
            PduR_RouteStatusType srcRouteStatus = PduR_SourceRouteStatus[srcPduId];
            SchM_Exit_PduR_ExclusiveArea_Route();
            if ((finishFlag) && ((srcRouteStatus == PDUR_RES_OK) || (srcRouteStatus == PDUR_RES_NOT_OK)))
            {
                /* The last transmit confirmation coming(SWS_PduR_00589) */
                PduR_SourceRouteStatus[srcPduId] = PDUR_RES_INITIAL;
                if (PduR_GetSrcPduUpTxConfOfSrcPdu(srcPduId))
                {
                    if (srcRouteStatus == PDUR_RES_OK)
                    {
                        PduR_IfTxConfirmationForward(srcPduId, destPduId, E_OK);
                    }
                    else
                    {
                        PduR_IfTxConfirmationForward(srcPduId, destPduId, E_NOT_OK);
                    }
                }
            }
            break;
        }
#endif
#if ((PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
        case PDUR_ROUTE_IF_GW_BUFFERED:
        {
            /* trigger transmit data provision is not interested in PduR_<DstLo>TxConfirmation (SWS_PduR_00640) */
            if (PduR_GetDataProvisionOfDestPdu(destPduId) == PDUR_DIRECT)
            {
                PduR_IfTxConfBufferedDirect(srcPduId, destPduId);
            }
            break;
        }
#endif
        default:
            /* do nothing */
            break;
        }

        if (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_PENDING)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
    }
    PDUR_NOUSED(srcPduId);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if ((PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_PduR_STCAL */
static void PduR_IfTxConfBufferedDirect(PduIdType srcPduId, PduIdType destPduId)
/* PRQA S 6070 -- */
{
    PduR_QueueRuntimeType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
    /* Transmit next entry if Lower layer return E_NOT_OK (SWS_PduR_00788) */
    while (TRUE) /* PRQA S 2740,0771 */ /* VL_PduR_Logical */
    {
        SchM_Enter_PduR_ExclusiveArea_Route();
        const PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
        if (bufferPtr == NULL_PTR)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }
        PduInfoType pduInfo;
        uint8       metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, 0u);
        SchM_Exit_PduR_ExclusiveArea_Route();

        Std_ReturnType retval = PduR_IfTransmitForward(srcPduId, destPduId, &pduInfo);
        SchM_Enter_PduR_ExclusiveArea_Route();
        if (retval == E_OK)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
            PduR_QueuePopFront(queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }
        PduR_QueuePopFront(queuePtr);
        SchM_Exit_PduR_ExclusiveArea_Route();
        if (queuePtr->Depth > 1u)
        {
            PduR_Det_ReportRuntimeError(PDUR_IFTXCONFIRMATION_ID, PDUR_E_PDU_INSTANCES_LOST);
        }
    }
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
/**
 * PduR trigger transmit implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532, 6070 ++ */ /* VL_QAC_OneFunRef, VL_MTR_PduR_STCAL */
Std_ReturnType PduR_PduRTriggerTransmit(PduIdType srcPduId, PduIdType destPduId, PduInfoType* pduInfoPtr)
/* PRQA S 1532, 6070 -- */
{
    Std_ReturnType retval    = E_NOT_OK;
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);

    switch (routeType)
    {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_TX_NOBUFFERED:
        retval = PduR_TriggerTransmitForward(srcPduId, destPduId, pduInfoPtr);
        if ((retval != E_NOT_OK) && PduR_GetTransmissionConfirmationOfDestPdu(destPduId))
        {
            PduR_SourceRouteStatus[srcPduId]       = PDUR_RES_PENDING;
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
        }
        break;
#endif
#if (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON)
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    case PDUR_ROUTE_IF_GW_BUFFERED:
    {
        PduR_QueueRuntimeType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
        SchM_Enter_PduR_ExclusiveArea_Route();
        const PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
        if (bufferPtr == NULL_PTR)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break; /* PRQA S 3333 */ /* VL_PduR_Logical */
        }

        PduInfoType pduInfo;
        uint8       metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, 0u);
        /* The PDU shall not be removed from the PduR buffer if the buffer is too small for the stored PDU data
         * (SWS_PduR_00819) */
        if (pduInfoPtr->SduLength < pduInfo.SduLength)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break; /* PRQA S 3333 */ /* VL_PduR_Logical */
        }
        SchM_Exit_PduR_ExclusiveArea_Route();
        pduInfoPtr->SduLength = pduInfo.SduLength;
        (void)IStdLib_MemCpy(pduInfoPtr->MetaDataPtr, pduInfo.MetaDataPtr, metaDataLength);
        (void)IStdLib_MemCpy(pduInfoPtr->SduDataPtr, pduInfo.SduDataPtr, pduInfoPtr->SduLength);
        retval = E_OK;
        /* The oldest FIFO entry shall be copied and then removed (SWS_PduR_00666) */
        /* The PDU Router shall buffer the latest I-PDU if last-is-best buffering is used (SWS_PduR_00809) */
        if (queuePtr->Depth > 1u) /* PRQA S 2812 */ /* VL_PduR_2812 */
        {
            /* Transmit next entry if Lower layer return E_NOT_OK (SWS_PduR_00788) */
            while (TRUE) /* PRQA S 2740,0771 */ /* VL_PduR_Logical */
            {
                SchM_Enter_PduR_ExclusiveArea_Route();
                PduR_QueuePopFront(queuePtr);
                bufferPtr = PduR_QueueGetHead(queuePtr);
                if (bufferPtr == NULL_PTR)
                {
                    SchM_Exit_PduR_ExclusiveArea_Route();
                    break;
                }
                PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, 0u);
                SchM_Exit_PduR_ExclusiveArea_Route();
                if (PduR_IfTransmitForward(srcPduId, destPduId, &pduInfo) == E_OK)
                {
                    break;
                }
                /* When <DstLo/DstLoTp>_Transmit() returns E_NOT_OK for a routing path using a FIFO, the PDU Router
                   shall report PduR.PDUR_E_PDU_INSTANCES_LOST to the DET module (SWS_PduR_00807) */
                PduR_Det_ReportRuntimeError(PDUR_IFTRIGGERTRANSMIT_ID, PDUR_E_PDU_INSTANCES_LOST);
            }
        }
        break;
    }
#endif
#endif /* end of PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED */
    default:
        /* do nothing */
        break;
    }
    PDUR_NOUSED(srcPduId);
    PDUR_NOUSED(pduInfoPtr);
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
/**
 * PduR StartOfReception with buffered implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532, 2889, 6070, 6060 ++ */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn, VL_MTR_PduR_STCAL, VL_MTR_PduR_STM19 */
BufReq_ReturnType PduR_PduRStartOfReceptionBuffered(
    PduIdType          srcPduId,
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType      tpSduLength,
    PduLengthType*     bufferSizePtr)
{
    BufReq_ReturnType bufReqReturn = BUFREQ_E_NOT_OK;
    /* PduR shall allocate enough buffers from PduRTxBuffer (for each destination). (SWS_PduR_00830) */
    PduR_QueueRuntimeType* queuePtr       = PduR_GetQueuePtrOfDestPdu(destPduId);
    uint8                  metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
    PduLengthType          requiredSize   = (PduLengthType)PduR_GetTpThresholdOfDestPdu(destPduId);

    if ((pduInfoPtr != NULL_PTR) && (pduInfoPtr->SduLength > requiredSize))
    {
        requiredSize = pduInfoPtr->SduLength;
    }
    requiredSize =
        (((tpSduLength == 0u) || (requiredSize < tpSduLength)) ? requiredSize : tpSduLength) + metaDataLength;
    boolean flushFlag = FALSE;
    SchM_Enter_PduR_ExclusiveArea_Route();
    PduR_BufferType* bufferPtr =
        PduR_QueueProvideBuffer(queuePtr, requiredSize, PDUR_TPSTARTOFRECEPTION_ID, &flushFlag);
    if (bufferPtr == NULL_PTR)
    {
        SchM_Exit_PduR_ExclusiveArea_Route();
        bufReqReturn = BUFREQ_E_OVFL;
        return bufReqReturn;
    }
    if (flushFlag)
    {
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_PENDING)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_NOT_OK;
        }
    }
    bufferPtr->SrcPduId   = srcPduId;
    bufferPtr->WriteState = PDUR_BUFFER_WRITE_PENDING;
    PduR_BufferStoreSduLength(bufferPtr, tpSduLength);
    if (pduInfoPtr != NULL_PTR)
    {
        PduR_BufferStoreMetaData(bufferPtr, pduInfoPtr->MetaDataPtr, metaDataLength);
        PduR_BufferStoreSduData(bufferPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
    }
    *bufferSizePtr = PduR_BufferGetRemainingSize(bufferPtr);
    SchM_Exit_PduR_ExclusiveArea_Route();
    bufReqReturn = BUFREQ_OK;

    PduR_TpDestinationProcess(srcPduId, destPduId, PDUR_TPSTARTOFRECEPTION_ID);
    return bufReqReturn;
}
/* PRQA S 1532, 2889, 6070, 6060 -- */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/**
 * PduR CopyRxData with buffered implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532, 2889, 6070, 6060 ++ */ /* VL_QAC_OneFunRef, VL_QAC_MultiReturn, VL_MTR_PduR_STCAL, VL_MTR_PduR_STM19 */
BufReq_ReturnType PduR_PduRCopyRxDataBuffered(
    PduIdType          srcPduId,
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType*     bufferSizePtr)
{
    BufReq_ReturnType            bufReqReturn = BUFREQ_E_NOT_OK;
    const PduR_QueueRuntimeType* queuePtr     = PduR_GetQueuePtrOfDestPdu(destPduId);
    SchM_Enter_PduR_ExclusiveArea_Route();
    PduR_BufferType* bufferPtr = PduR_QueueGetTail(srcPduId, queuePtr);
    if ((bufferPtr == NULL_PTR) || (bufferPtr->WriteState != PDUR_BUFFER_WRITE_PENDING))
    {
        SchM_Exit_PduR_ExclusiveArea_Route();
        return bufReqReturn;
    }

    PduLengthType bufferSize = PduR_BufferGetRemainingSize(bufferPtr);
    if (bufferSize < pduInfoPtr->SduLength)
    {
        bufferPtr->WriteState = PDUR_BUFFER_WRITE_NOT_OK;
        SchM_Exit_PduR_ExclusiveArea_Route();
        return bufReqReturn;
    }

    PduR_BufferStoreSduData(bufferPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
    *bufferSizePtr = PduR_BufferGetRemainingSize(bufferPtr);
    SchM_Exit_PduR_ExclusiveArea_Route();
    bufReqReturn = BUFREQ_OK;
    PduR_TpDestinationProcess(srcPduId, destPduId, PDUR_TPCOPYRXDATA_ID);

    /* SWS_PduR_00804: In case of gatewaying between TPs, when all destinations fail, the reception side shall be
 stopped by returning BUFREQ_E_NOT_OK for the current call of CopyRxData or StartOfReception. */
    SchM_Enter_PduR_ExclusiveArea_Route();
    bufferPtr = PduR_QueueGetHead(queuePtr);
    if (bufferPtr == NULL_PTR)
    {
        bufReqReturn = BUFREQ_E_NOT_OK;
    }
    SchM_Exit_PduR_ExclusiveArea_Route();
    return bufReqReturn;
}
/* PRQA S 1532, 2889, 6070, 6060 -- */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/**
 * PduR TpRxIndication with buffered implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void PduR_PduRTpRxIndicationBuffered(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
/* PRQA S 1532 -- */
{
    const PduR_QueueRuntimeType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
    SchM_Enter_PduR_ExclusiveArea_Route();
    PduR_BufferType* tailPtr = PduR_QueueGetTail(srcPduId, queuePtr);
    if ((tailPtr != NULL_PTR) && (tailPtr->WriteState != PDUR_BUFFER_WRITE_OK))
    {
        tailPtr->WriteState = (result == E_OK) ? PDUR_BUFFER_WRITE_OK : PDUR_BUFFER_WRITE_NOT_OK;

        if (tailPtr->WriteState != PDUR_BUFFER_WRITE_OK)
        {
            if ((tailPtr != PduR_QueueGetHead(queuePtr))
                || (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_PENDING))
            {
                PduR_QueuePopBack(srcPduId, queuePtr);
            }
        }
    }
    SchM_Exit_PduR_ExclusiveArea_Route();
    PduR_TpDestinationProcess(srcPduId, destPduId, PDUR_TPCOPYRXDATA_ID);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /* end of PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED and PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED */

#if (PDUR_TP_COPYTXDATA == STD_ON)
/**
 * PduR CopyTxData implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532, 6070 ++ */ /* VL_QAC_OneFunRef, VL_MTR_PduR_STCAL */
BufReq_ReturnType PduR_PduRCopyTxData(
    PduIdType            srcPduId,
    PduIdType            destPduId,
    const PduInfoType*   pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType*       availableDataPtr)
/* PRQA S 1532, 6070 -- */
{
    BufReq_ReturnType bufReqReturn = BUFREQ_E_NOT_OK;
    PduR_RouteType    routeType    = PduR_GetRouteTypeOfDestPdu(destPduId);

    switch (routeType)
    {
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
    case PDUR_ROUTE_TP_TX_NOBUFFERED_UNICAST:
    {
        bufReqReturn = PduR_CopyTxDataForward(srcPduId, destPduId, pduInfoPtr, retryInfoPtr, availableDataPtr);
        break;
    }
#if STD_ON == PDUR_MULTICAST_TP_TRANSMIT_SUPPORT
    case PDUR_ROUTE_TP_TX_NOBUFFERED_MULTICAST:
    {
        const RetryInfoType* retryPtr;
        SchM_Enter_PduR_ExclusiveArea_Route();
        if ((retryInfoPtr != NULL_PTR) && (retryInfoPtr->TpDataState == TP_DATARETRY))
        {
            /* SWS_PduR_00871 */
            SchM_Exit_PduR_ExclusiveArea_Route();
            break; /* PRQA S 3333 */ /* VL_PduR_Logical */
        }

        RetryInfoType localRetryInfo;
        if (PduR_SourceMultiCastTpTransmittedCnt[srcPduId] == 0u)
        {
            /* SWS_PduR_00631 */
            localRetryInfo.TpDataState = TP_CONFPENDING;
            localRetryInfo.TxTpDataCnt = 0u;
            retryPtr                   = &localRetryInfo;
        }
        else
        {
            if (PduR_SourceMultiCastTpTransmittedCnt[srcPduId] < PduR_DestinationMultiCastTpTransmittedCnt[destPduId])
            {
                /* SWS_PduR_00872 */
                bufReqReturn = BUFREQ_E_BUSY;
                SchM_Exit_PduR_ExclusiveArea_Route();
                break; /* PRQA S 3333 */ /* VL_PduR_Logical */
            }
            else
            {
                /* SWS_PduR_00632*/
                localRetryInfo.TpDataState = TP_DATARETRY;
                localRetryInfo.TxTpDataCnt = PduR_SourceMultiCastTpTransmittedCnt[srcPduId]
                                             - PduR_DestinationMultiCastTpTransmittedCnt[destPduId];
                PduR_SourceMultiCastTpTransmittedCnt[srcPduId] -= localRetryInfo.TxTpDataCnt;
                retryPtr = &localRetryInfo;
            }
        }
        SchM_Exit_PduR_ExclusiveArea_Route();
        bufReqReturn = PduR_CopyTxDataForward(srcPduId, destPduId, pduInfoPtr, retryPtr, availableDataPtr);

        SchM_Enter_PduR_ExclusiveArea_Route();
        if (bufReqReturn == BUFREQ_OK)
        {
            PduR_SourceMultiCastTpTransmittedCnt[srcPduId] += pduInfoPtr->SduLength;
            PduR_DestinationMultiCastTpTransmittedCnt[destPduId] += pduInfoPtr->SduLength;
        }
        SchM_Exit_PduR_ExclusiveArea_Route();
        break;
    }
#endif
#endif
#if ((PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
    case PDUR_ROUTE_TP_GW_BUFFERED:
    {
        SchM_Enter_PduR_ExclusiveArea_Route();
        if (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_PENDING)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break; /* PRQA S 3333 */ /* VL_PduR_Logical */
        }
        TpDataStateType        tpDataState = (retryInfoPtr != NULL_PTR) ? retryInfoPtr->TpDataState : TP_CONFPENDING;
        PduR_QueueRuntimeType* queuePtr    = PduR_GetQueuePtrOfDestPdu(destPduId);
        const PduR_BufferType* bufferPtr   = PduR_QueueGetHead(queuePtr);
        if ((bufferPtr == NULL_PTR) || (bufferPtr->WriteState == PDUR_BUFFER_WRITE_NOT_OK))
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break; /* PRQA S 3333 */ /* VL_PduR_Logical */
        }

        uint8       metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
        PduInfoType pduInfo;

        if (tpDataState == TP_DATACONF)
        {
            /*TP_DATACONF: all data that has been copied before this call is confirmed and can be removed from the TP
             * buffer.*/
            PduR_QueueDataConf(queuePtr, metaDataLength);
        }
        else if (tpDataState == TP_DATARETRY)
        {
            if (retryInfoPtr->TxTpDataCnt <= queuePtr->CurPosition)
            {
                PduR_QueueSetBack(queuePtr, retryInfoPtr->TxTpDataCnt);
            }
            else
            {
                SchM_Exit_PduR_ExclusiveArea_Route();
                break; /* PRQA S 3333 */ /* VL_PduR_Logical */
            }
        }
        else
        {
            /* do nothing */
        }

        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, queuePtr->CurPosition);
        if (pduInfo.SduLength < pduInfoPtr->SduLength)
        {
            bufReqReturn = BUFREQ_E_BUSY;
            SchM_Exit_PduR_ExclusiveArea_Route();
            break; /* PRQA S 3333 */ /* VL_PduR_Logical */
        }

        (void)IStdLib_MemCpy(pduInfoPtr->SduDataPtr, pduInfo.SduDataPtr, pduInfoPtr->SduLength);
        PduR_QueueSetForward(queuePtr, pduInfoPtr->SduLength);
        if (retryInfoPtr == NULL_PTR)
        {
            PduR_QueueDataConf(queuePtr, metaDataLength);
        }
        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, queuePtr->CurPosition);
        SchM_Exit_PduR_ExclusiveArea_Route();
        *availableDataPtr = pduInfo.SduLength;
        bufReqReturn      = BUFREQ_OK;
        break;
    }
#endif
    default:
        /* do nothing */
        break;
    }
    PDUR_NOUSED(srcPduId);
    PDUR_NOUSED(pduInfoPtr);
    PDUR_NOUSED(retryInfoPtr);
    PDUR_NOUSED(availableDataPtr);
    return bufReqReturn;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
/**
 * PduR TpTxConfirmation implement
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void PduR_PduRTpTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
/* PRQA S 1532 -- */
{
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
    switch (routeType)
    {
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
    case PDUR_ROUTE_TP_TX_NOBUFFERED_UNICAST:
    {
        PduR_TpTxConfirmationForward(srcPduId, destPduId, result);
        break;
    }
    case PDUR_ROUTE_TP_TX_NOBUFFERED_MULTICAST:
    {
#if STD_ON == PDUR_MULTICAST_TP_TRANSMIT_SUPPORT
        SchM_Enter_PduR_ExclusiveArea_Route();
        PduR_DestinationMultiCastTpTransmittedCnt[destPduId] = 0u;
        PduR_DestinationRouteStatus[destPduId]               = (result == E_OK) ? PDUR_RES_OK : PDUR_RES_NOT_OK;
        boolean              finishFlag                      = PduR_EvaluateUpLayerConfirmationStatus(srcPduId);
        PduR_RouteStatusType srcRouteStatus                  = PduR_SourceRouteStatus[srcPduId];
        SchM_Exit_PduR_ExclusiveArea_Route();
        if ((finishFlag) && ((srcRouteStatus == PDUR_RES_OK) || (srcRouteStatus == PDUR_RES_NOT_OK)))
        {
            /* SWS_PduR_00765 */
            if (srcRouteStatus == PDUR_RES_OK)
            {
                PduR_TpTxConfirmationForward(srcPduId, destPduId, E_OK);
            }
            else
            {
                PduR_TpTxConfirmationForward(srcPduId, destPduId, E_NOT_OK);
            }
            PduR_SourceMultiCastTpTransmittedCnt[srcPduId] = 0u;
            PduR_SourceRouteStatus[srcPduId]               = PDUR_RES_INITIAL;
        }
#endif
        break;
    }
#endif
#if (PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON)
    case PDUR_ROUTE_TP_GW_BUFFERED:
    {
        SchM_Enter_PduR_ExclusiveArea_Route();
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_NOT_OK)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        else if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_PENDING)
        {
            PduR_QueueRuntimeType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
            PduR_QueuePopFront(queuePtr);
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        else
        {
            /* do nothing */
        }
        SchM_Exit_PduR_ExclusiveArea_Route();
        PduR_TpDestinationProcess(srcPduId, destPduId, PDUR_TPTXCONFIRMATION_ID);
        break;
    }
#endif
    default:
        /* do nothing */
        break;
    }
    PDUR_NOUSED(srcPduId);
    PDUR_NOUSED(result);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
/**
 * PduR destination process for transport protocol route with buffer
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_PduR_STCAL */
static void PduR_TpDestinationProcess(PduIdType srcPduId, PduIdType destPduId, uint8 apiId)
/* PRQA S 6070 -- */
{
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    PduR_QueueRuntimeType* queuePtr       = PduR_GetQueuePtrOfDestPdu(destPduId);
    uint8                  metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
    PduLengthType          tpThreshold    = (PduLengthType)PduR_GetTpThresholdOfDestPdu(destPduId);

    /* Only Dest status is PDUR_RES_INITIAL, can do transmit or reception for next pdu */
    while (TRUE) /* PRQA S 2740,0771 */ /* VL_PduR_Logical */
    {
        SchM_Enter_PduR_ExclusiveArea_Route();
        if (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_INITIAL)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }

        const PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
        if (bufferPtr == NULL_PTR)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }
        PduR_RouteStatusType status            = PduR_DestinationRouteStatus[destPduId];
        PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;

        PduInfoType pduInfo;
        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, 0u);
        if ((pduInfo.SduLength < tpThreshold) && (bufferPtr->WriteState != PDUR_BUFFER_WRITE_OK))
        {
            PduR_DestinationRouteStatus[destPduId] = status;
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }
        pduInfo.SduLength        = bufferPtr->SduLength;
        PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON
        if (routeType == PDUR_ROUTE_TP_GW_BUFFERED)
        {
            pduInfo.SduDataPtr = NULL_PTR;
            SchM_Exit_PduR_ExclusiveArea_Route();
            if (PduR_TpTransmitForward(srcPduId, destPduId, &pduInfo) == E_OK)
            {
                break;
            }
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_DestinationRouteStatus[destPduId] = status;
            PduR_QueuePopFront(queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
            PduR_Det_ReportRuntimeError(apiId, PDUR_E_TP_GW_TX_REQ_REJECTED);
            PduR_Det_ReportRuntimeError(apiId, PDUR_E_PDU_INSTANCES_LOST);
            continue; /* PRQA S 0770 */ /* VL_PduR_0770 */
        }
#endif
        PduR_DestinationRouteStatus[destPduId] = status;
        SchM_Exit_PduR_ExclusiveArea_Route();
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
        if (routeType == PDUR_ROUTE_TP_RX_BUFFERED)
        {
            PduLengthType     bufferSize;
            BufReq_ReturnType bufReqReturn =
                PduR_StartOfReceptionForward(srcPduId, destPduId, &pduInfo, pduInfo.SduLength, &bufferSize);
            PduR_TpRxIndicationForward(srcPduId, destPduId, (bufReqReturn == BUFREQ_OK) ? E_OK : E_NOT_OK);
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_QueuePopFront(queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
        }
#endif
    }
#endif
    PDUR_NOUSED(srcPduId);
    PDUR_NOUSED(destPduId);
    PDUR_NOUSED(apiId);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
/**
 * Call the target module transport protocol startofreception function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
BufReq_ReturnType PduR_StartOfReceptionForward(
    PduIdType          srcPduId,
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType      tpSduLength,
    PduLengthType*     bufferSizePtr)
/* PRQA S 1532 -- */
{
    const PduR_BswModuleType*          bswModulePtr           = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType                          bswModulePduId         = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    BufReq_ReturnType                  bufReqReturn           = BUFREQ_E_NOT_OK;
    PduR_ModuleStartOfReceptionApiType startOfReceptionApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
    if (bswModulePtr->StartOfReceptionApi != NULL_PTR)
    {
        startOfReceptionApiPtr = bswModulePtr->StartOfReceptionApi[funcIndex];
    }
#else
    startOfReceptionApiPtr = bswModulePtr->StartOfReceptionApi;
    PDUR_NOUSED(srcPduId);
#endif
    if (NULL_PTR != startOfReceptionApiPtr)
    {
        bufReqReturn = startOfReceptionApiPtr(bswModulePduId, pduInfoPtr, tpSduLength, bufferSizePtr);
    }
    return bufReqReturn;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON)
/**
 * Call the target module transport protocol copyrxdata function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
BufReq_ReturnType PduR_CopyRxDataForward(
    PduIdType          srcPduId,
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType*     bufferSizePtr)
/* PRQA S 1532 -- */
{
    const PduR_BswModuleType*    bswModulePtr     = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType                    bswModulePduId   = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    BufReq_ReturnType            bufReqReturn     = BUFREQ_E_NOT_OK;
    PduR_ModuleCopyRxDataApiType copyRxDataApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
    if (bswModulePtr->CopyRxDataApi != NULL_PTR)
    {
        copyRxDataApiPtr = bswModulePtr->CopyRxDataApi[funcIndex];
    }
#else
    copyRxDataApiPtr = bswModulePtr->CopyRxDataApi;
    PDUR_NOUSED(srcPduId);
#endif
    if (NULL_PTR != copyRxDataApiPtr)
    {
        bufReqReturn = copyRxDataApiPtr(bswModulePduId, pduInfoPtr, bufferSizePtr);
    }
    return bufReqReturn;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
/**
 * Call the target module transport protocol receive indication function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void PduR_TpRxIndicationForward(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
/* PRQA S 1532 -- */
{
    const PduR_BswModuleType*        bswModulePtr         = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType                        bswModulePduId       = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    PduR_ModuleTpRxIndicationApiType tpRxIndicationApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
    if (bswModulePtr->TpRxIndicationApi != NULL_PTR)
    {
        tpRxIndicationApiPtr = bswModulePtr->TpRxIndicationApi[funcIndex];
    }
#else
    tpRxIndicationApiPtr = bswModulePtr->TpRxIndicationApi;
    PDUR_NOUSED(srcPduId);
#endif
    if (NULL_PTR != tpRxIndicationApiPtr)
    {
        tpRxIndicationApiPtr(bswModulePduId, result);
    }
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
/**
 * Init all PduR_SourceRouteStatus and PduR_DestinationRouteStatus
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_RouteInit(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (PDUR_ROUTINGPATH_MAX > 0u)
    for (PduIdType index = 0u; index < PduR_GetPduRRoutingPathNumOfPBConfig; ++index)
    {
        PduR_DestinationRouteStatus[index] = PDUR_RES_INITIAL;
#if STD_ON == PDUR_MULTICAST_TP_TRANSMIT_SUPPORT
        PduR_DestinationMultiCastTpTransmittedCnt[index] = 0u;
#endif
    }
    for (PduIdType index = 0u; index < PduR_GetSrcPduNumOfPBConfig; ++index)
    {
        PduR_SourceRouteStatus[index] = PDUR_RES_INITIAL;
#if STD_ON == PDUR_MULTICAST_TP_TRANSMIT_SUPPORT
        PduR_SourceMultiCastTpTransmittedCnt[index] = 0u;
#endif
    }
#endif
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/* ========================================== internal function definition ========================================== */
#if (                                                           \
    (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON) \
    || (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON))
/**
 * Call the target module interface transmit function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static Std_ReturnType PduR_IfTransmitForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType               retval           = E_NOT_OK;
    const PduR_BswModuleType*    bswModulePtr     = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType                    bswModulePduId   = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    PduR_ModuleIfTransmitApiType ifTransmitApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
    if (bswModulePtr->IfTransmitApi != NULL_PTR)
    {
        ifTransmitApiPtr = bswModulePtr->IfTransmitApi[funcIndex];
    }
#else
    ifTransmitApiPtr = bswModulePtr->IfTransmitApi;
    PDUR_NOUSED(srcPduId);
#endif
    if (NULL_PTR != ifTransmitApiPtr)
    {
        retval = ifTransmitApiPtr(bswModulePduId, pduInfoPtr);
    }
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
/**
 * Call the target module trigger transmit function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static Std_ReturnType PduR_TriggerTransmitForward(PduIdType srcPduId, PduIdType destPduId, PduInfoType* pduInfoPtr)
{
    Std_ReturnType                    retval                = E_NOT_OK;
    const PduR_BswModuleType*         bswModulePtr          = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType                         bswModulePduId        = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    PduR_ModuleTriggertransmitApiType triggertransmitApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    if (bswModulePtr->TriggertransmitApi != NULL_PTR)
    {
        uint8 funcIndex       = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, FALSE);
        triggertransmitApiPtr = bswModulePtr->TriggertransmitApi[funcIndex];
    }
#else
    triggertransmitApiPtr = bswModulePtr->TriggertransmitApi;
    PDUR_NOUSED(destPduId);
#endif
    if (NULL_PTR != triggertransmitApiPtr)
    {
        retval = triggertransmitApiPtr(bswModulePduId, pduInfoPtr);
    }
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/**
 * Call the target module interface transmit confirmation function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_IfTxConfirmationForward(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
{
    const PduR_BswModuleType*        bswModulePtr         = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType                        bswModulePduId       = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    PduR_ModuleTxConfirmationApiType txConfirmationApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    if (bswModulePtr->TxConfirmationApi != NULL_PTR)
    {
        uint8 funcIndex      = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, FALSE);
        txConfirmationApiPtr = bswModulePtr->TxConfirmationApi[funcIndex];
    }

#else
    txConfirmationApiPtr = bswModulePtr->TxConfirmationApi;
    PDUR_NOUSED(destPduId);
#endif
    if (NULL_PTR != txConfirmationApiPtr)
    {
        txConfirmationApiPtr(bswModulePduId, result);
    }
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/**
 * Call the target module interface receive indication function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_IfRxIndicationForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    const PduR_BswModuleType*        bswModulePtr         = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType                        bswModulePduId       = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    PduR_ModuleIfRxIndicationApiType ifRxIndicationApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    if (bswModulePtr->IfRxIndicationApi != NULL_PTR)
    {
        uint8 funcIndex      = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
        ifRxIndicationApiPtr = bswModulePtr->IfRxIndicationApi[funcIndex];
    }
#else
    ifRxIndicationApiPtr = bswModulePtr->IfRxIndicationApi;
    PDUR_NOUSED(srcPduId);
#endif
    if (NULL_PTR != ifRxIndicationApiPtr)
    {
        ifRxIndicationApiPtr(bswModulePduId, pduInfoPtr);
    }
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON
/**
 * Call the target module transport protocol transmit function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static Std_ReturnType PduR_TpTransmitForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    const PduR_BswModuleType*    bswModulePtr     = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType                    bswModulePduId   = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    Std_ReturnType               retval           = E_NOT_OK;
    PduR_ModuleTpTransmitApiType tpTransmitApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    if (bswModulePtr->TpTransmitApi != NULL_PTR)
    {
        uint8 funcIndex  = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
        tpTransmitApiPtr = bswModulePtr->TpTransmitApi[funcIndex];
    }
#else
    tpTransmitApiPtr = bswModulePtr->TpTransmitApi;
    PDUR_NOUSED(srcPduId);
#endif
    if (NULL_PTR != tpTransmitApiPtr)
    {
        retval = tpTransmitApiPtr(bswModulePduId, pduInfoPtr);
    }
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
/**
 * Call the target module transport protocol copytxdata function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static BufReq_ReturnType PduR_CopyTxDataForward(
    PduIdType            srcPduId,
    PduIdType            destPduId,
    const PduInfoType*   pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType*       availableDataPtr)
{
    const PduR_BswModuleType*    bswModulePtr     = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType                    bswModulePduId   = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    BufReq_ReturnType            retval           = BUFREQ_E_NOT_OK;
    PduR_ModuleCopyTxDataApiType copyTxDataApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    if (bswModulePtr->CopyTxDataApi != NULL_PTR)
    {
        uint8 funcIndex  = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, FALSE);
        copyTxDataApiPtr = bswModulePtr->CopyTxDataApi[funcIndex];
    }
#else
    copyTxDataApiPtr = bswModulePtr->CopyTxDataApi;
    PDUR_NOUSED(destPduId);
#endif
    if (NULL_PTR != copyTxDataApiPtr)
    {
        retval = copyTxDataApiPtr(bswModulePduId, pduInfoPtr, retryInfoPtr, availableDataPtr);
    }
    return retval;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/**
 * Call the target module transport protocol transmit confirmation function
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static void PduR_TpTxConfirmationForward(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
{
    const PduR_BswModuleType*          bswModulePtr           = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType                          bswModulePduId         = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    PduR_ModuleTpTxConfirmationApiType tpTxConfirmationApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    if (bswModulePtr->TpTxConfirmationApi != NULL_PTR)
    {
        uint8 funcIndex        = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, FALSE);
        tpTxConfirmationApiPtr = bswModulePtr->TpTxConfirmationApi[funcIndex];
    }
#else
    tpTxConfirmationApiPtr = bswModulePtr->TpTxConfirmationApi;
    PDUR_NOUSED(destPduId);
#endif
    if (NULL_PTR != tpTxConfirmationApiPtr)
    {
        tpTxConfirmationApiPtr(bswModulePduId, result);
    }
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
/**
 * Evaluate the PduR_SourceRouteStatus
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static boolean PduR_EvaluateUpLayerConfirmationStatus(PduIdType srcPduId)
{
    PduR_RouteStatusType txResult   = PduR_SourceRouteStatus[srcPduId];
    boolean              finishFlag = TRUE;
    if ((txResult == PDUR_RES_PENDING) || (txResult == PDUR_RES_OK))
    {
        uint8 destPduSum = PduR_GetDestPduNumOfSrcPdu(srcPduId);
        for (uint8 index = 0u; index < destPduSum; ++index)
        {
            PduIdType            destPduId       = PduR_GetDestPduOfRoutingPath(srcPduId, index);
            PduR_RouteStatusType destRouteStatus = PduR_DestinationRouteStatus[destPduId];
            if (destRouteStatus == PDUR_RES_PENDING)
            {
                finishFlag = FALSE;
            }
            else if (destRouteStatus == PDUR_RES_OK)
            {
                /* At least a dest route get confirmation,source route state can be set OK. */
                PduR_SourceRouteStatus[srcPduId] = PDUR_RES_OK;
            }
            else
            {
                /* Do nothig */
            }
        }
        if ((finishFlag) && (PduR_SourceRouteStatus[srcPduId] != PDUR_RES_OK))
        {
            PduR_SourceRouteStatus[srcPduId] = PDUR_RES_NOT_OK;
        }
    }
    return finishFlag;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/**
 * Get module configuration of SrcPdu
 */
static inline const PduR_BswModuleType* PduR_GetBswModulePtrOfSrcPdu(PduIdType srcPduId)
{
    return &PduR_BswModuleConfigData[PDUR_SRCEPDU_CFG(srcPduId).BswModuleIndex];
}
/**
 * Get module configuration of SrcPdu
 */
static inline const PduR_BswModuleType* PduR_GetBswModulePtrOfDestPdu(PduIdType destPduId)
{
    return &PduR_BswModuleConfigData[PDUR_DESTPDU_CFG(destPduId).BswModuleIndex];
}
/* PRQA S 3672 -- */ /* VL_PduR_FuncPtrNonConst */
#endif
