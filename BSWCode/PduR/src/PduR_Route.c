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
static void PduR_IfRxIndBufferedDirect(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfo);
static void PduR_IfTxConfBufferedDirect(PduIdType srcPduId, PduIdType destPduId);
#endif
/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */
#if (                                                           \
    (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON) \
    || (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON))
static Std_ReturnType PduR_IfTransmitForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
static Std_ReturnType PduR_TriggerTransmitForward(PduIdType srcPduId, PduIdType destPduId, PduInfoType* pduInfoPtr);
static void           PduR_IfTxConfirmationForward(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
static void PduR_IfRxIndicationForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON
static Std_ReturnType PduR_TpTransmitForward(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
static BufReq_ReturnType PduR_CopyTxDataForward(
    PduIdType            srcPduId,
    PduIdType            destPduId,
    const PduInfoType*   pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType*       availableDataPtr);
static void PduR_TpTxConfirmationForward(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result);
#endif

static boolean PduR_EvaluateUpLayerConfirmationStatus(PduIdType srcPduId);

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
static void PduR_TpDestinationProcess(PduIdType srcPduId, PduIdType destPduId, uint8 apiId);
#endif

/* PRQA S 5016 ++ */ /* VL_PduR_5016 */
static inline const PduR_BswModuleType* PduR_GetBswModulePtrOfSrcPdu(PduIdType srcPduId);
static inline const PduR_BswModuleType* PduR_GetBswModulePtrOfDestPdu(PduIdType destPduId);
/* PRQA S 5016 -- */ /* VL_PduR_5016 */
/* ========================================== external function definition ========================================== */
/* PRQA S 3672 ++ */ /* VL_PduR_FuncPtrNonConst */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
/**
 * PduR interface transmit implement
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType PduR_PduRIfTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 1532 -- */
{
    Std_ReturnType retval = E_NOT_OK;
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    retval = PduR_IfTransmitForward(srcPduId, destPduId, pduInfoPtr);
    if ((retval != E_NOT_OK) && PduR_GetTransmissionConfirmationOfDestPdu(destPduId))
    {
        PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
    }
#endif
    return retval;
}
#endif

#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
/**
 * PduR transport protocal transmit implement
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType PduR_PduRTpTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 1532 -- */
{
    Std_ReturnType retval = E_NOT_OK;

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
    retval = PduR_TpTransmitForward(srcPduId, destPduId, pduInfoPtr);
#endif
    return retval;
}
#endif

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
/**
 * PduR cancel transmit implement
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType PduR_PduRCanceTransmit(PduIdType srcPduId, PduIdType destPduId)
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
    case PDUR_ROUTE_TP_TX_NOBUFFERED:
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
#endif

#if (PDUR_CANCEL_RECEIVE == STD_ON)
/**
 * PduR cancel receive implement
 */
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
#endif
    return retval;
}
#endif

#if (PDUR_RX_INDICATION == STD_ON)
/**
 * PduR interface receive indication implement
 */
/* PRQA S 1532, 6070 ++ */ /* VL_QAC_OneFunRef, VL_MTR_PduR_STCAL */
void PduR_PduRIfRxIndication(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
/* PRQA S 1532, 6070 -- */
{
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);

    switch (routeType)
    {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_RX_NOBUFFERED:
        PduR_IfRxIndicationForward(srcPduId, destPduId, pduInfoPtr);
        break;
#endif
#if (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_GW_NOBUFFERED:
    {
        PduInfoType pduInfoPtrVar = *pduInfoPtr;
        /* The rest of the received I-PDU shall be dropped if the configured length of the destination I-PDU is smaller
           than the received data length (PduLength of received I-PDU)(SWS_PduR_00783) */
        if (PduR_GetConfiguredLengthOfDestPdu(destPduId) < pduInfoPtrVar.SduLength)
        {
            pduInfoPtrVar.SduLength = PduR_GetConfiguredLengthOfDestPdu(destPduId);
        }
        (void)PduR_IfTransmitForward(srcPduId, destPduId, &pduInfoPtrVar);
        break;
    }
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
    case PDUR_ROUTE_IF_GW_BUFFERED:
    {
        PduInfoType pduInfoPtrVar = *pduInfoPtr;
        /* The rest of the received I-PDU shall be dropped if the configured length of the destination I-PDU is smaller
           than the received data length (PduLength of received I-PDU) (SWS_PduR_00746) */
        if (PduR_GetConfiguredLengthOfDestPdu(destPduId) < pduInfoPtrVar.SduLength)
        {
            pduInfoPtrVar.SduLength = PduR_GetConfiguredLengthOfDestPdu(destPduId);
        }
        if (PduR_GetDataProvisionOfDestPdu(destPduId) == PDUR_TRIGGERTRANSMIT)
        {
            PduR_QueueRuntimeType* queuePtr       = PduR_GetQueuePtrOfDestPdu(destPduId);
            uint8                  metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
            /* Enqueue to FIFO and call <DstLo>_Transmit if FIFO queue is empty (SWS_PduR_00786) */
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_QueueStoreType retval =
                PduR_QueueStorePduInfo(queuePtr, metaDataLength, &pduInfoPtrVar, PDUR_IFRXINDICATION_ID);
            const PduR_BufferType* tailBufferPtr = PduR_QueueGetTail(queuePtr);
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
}
#endif

#if ((PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
/**
 * PduR interface receive indication implement for IF gateWay with PDUR_DIRECT
 */
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
#endif

#if (PDUR_TX_CONFIRMATION == STD_ON)
/**
 * PduR interface transmit confirmation implement
 */
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

        PduR_RouteStatusType srcRouteStatus;
        boolean              finishFlag;
        switch (routeType)
        {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
        case PDUR_ROUTE_IF_TX_NOBUFFERED:
            finishFlag     = PduR_EvaluateUpLayerConfirmationStatus(srcPduId);
            srcRouteStatus = PduR_SourceRouteStatus[srcPduId];
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
}
#endif

#if ((PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
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
#endif

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
/**
 * PduR trigger transmit implement
 */
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
    return retval;
}
#endif

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
/**
 * PduR StartOfReception with buffered implement
 */
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
/**
 * PduR CopyRxData with buffered implement
 */
BufReq_ReturnType PduR_PduRCopyRxDataBuffered(
    PduIdType          srcPduId,
    PduIdType          destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType*     bufferSizePtr)
{
    BufReq_ReturnType            bufReqReturn = BUFREQ_E_NOT_OK;
    const PduR_QueueRuntimeType* queuePtr     = PduR_GetQueuePtrOfDestPdu(destPduId);
    SchM_Enter_PduR_ExclusiveArea_Route();
    PduR_BufferType* bufferPtr = PduR_QueueGetTail(queuePtr);
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
/**
 * PduR TpRxIndication with buffered implement
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void PduR_PduRTpRxIndicationBuffered(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
/* PRQA S 1532 -- */
{
    const PduR_QueueRuntimeType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
    SchM_Enter_PduR_ExclusiveArea_Route();
    PduR_BufferType* tailPtr = PduR_QueueGetTail(queuePtr);
    if ((tailPtr != NULL_PTR) && (tailPtr->WriteState != PDUR_BUFFER_WRITE_OK))
    {
        tailPtr->WriteState = (result == E_OK) ? PDUR_BUFFER_WRITE_OK : PDUR_BUFFER_WRITE_NOT_OK;

        if (tailPtr->WriteState != PDUR_BUFFER_WRITE_OK)
        {
            if ((tailPtr != PduR_QueueGetHead(queuePtr))
                || (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_PENDING))
            {
                PduR_QueuePopBack(queuePtr);
            }
        }
    }
    SchM_Exit_PduR_ExclusiveArea_Route();
    PduR_TpDestinationProcess(srcPduId, destPduId, PDUR_TPCOPYRXDATA_ID);
}
#endif /* end of PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED and PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED */

#if (PDUR_TP_COPYTXDATA == STD_ON)
/**
 * PduR CopyTxData implement
 */
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
    case PDUR_ROUTE_TP_TX_NOBUFFERED:
        bufReqReturn = PduR_CopyTxDataForward(srcPduId, destPduId, pduInfoPtr, retryInfoPtr, availableDataPtr);
        break;
#endif
#if ((PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
    case PDUR_ROUTE_TP_GW_BUFFERED:
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
#endif
    default:
        /* do nothing */
        break;
    }
    return bufReqReturn;
}
#endif

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
/**
 * PduR TpTxConfirmation implement
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void PduR_PduRTpTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
/* PRQA S 1532 -- */
{
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
    switch (routeType)
    {
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
    case PDUR_ROUTE_TP_TX_NOBUFFERED:
        PduR_TpTxConfirmationForward(srcPduId, destPduId, result);
        break;
#endif
#if (PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON)
    case PDUR_ROUTE_TP_GW_BUFFERED:
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
            /* do nothing  */
        }
        SchM_Exit_PduR_ExclusiveArea_Route();

        PduR_TpDestinationProcess(srcPduId, destPduId, PDUR_TPTXCONFIRMATION_ID);
        break;
#endif
    default:
        /* do nothing */
        break;
    }
}
#endif

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
/**
 * PduR destination process for transport protocol route with buffer
 */
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
                PduR_StartOfReceptionForward(destPduId, &pduInfo, pduInfo.SduLength, &bufferSize);
            PduR_TpRxIndicationForward(destPduId, (bufReqReturn == BUFREQ_OK) ? E_OK : E_NOT_OK);
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_QueuePopFront(queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
        }
#endif
    }
#else
    PDUR_NOUSED(destPduId);
#endif
}
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
/**
 * Call the target module transport protocol startofreception function
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
BufReq_ReturnType PduR_StartOfReceptionForward(
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
    PduIdType srcPduId  = PduR_GetSrcPduOfDestPdu(destPduId);
    uint8     funcIndex = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
    if (bswModulePtr->StartOfReceptionApi != NULL_PTR)
    {
        startOfReceptionApiPtr = bswModulePtr->StartOfReceptionApi[funcIndex];
    }
#else
    startOfReceptionApiPtr = bswModulePtr->StartOfReceptionApi;
#endif
    if (NULL_PTR != startOfReceptionApiPtr)
    {
        bufReqReturn = startOfReceptionApiPtr(bswModulePduId, pduInfoPtr, tpSduLength, bufferSizePtr);
    }
    return bufReqReturn;
}
#endif

#if (PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON)
/**
 * Call the target module transport protocol copyrxdata function
 */
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
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
/**
 * Call the target module transport protocol receive indication function
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void PduR_TpRxIndicationForward(PduIdType destPduId, Std_ReturnType result)
/* PRQA S 1532 -- */
{
    const PduR_BswModuleType*        bswModulePtr         = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType                        bswModulePduId       = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    PduR_ModuleTpRxIndicationApiType tpRxIndicationApiPtr = NULL_PTR;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    PduIdType srcPduId  = PduR_GetSrcPduOfDestPdu(destPduId);
    uint8     funcIndex = PduR_GetPartitionFuncIndex(bswModulePtr, srcPduId, destPduId, TRUE);
    if (bswModulePtr->TpRxIndicationApi != NULL_PTR)
    {
        tpRxIndicationApiPtr = bswModulePtr->TpRxIndicationApi[funcIndex];
    }
#else
    tpRxIndicationApiPtr = bswModulePtr->TpRxIndicationApi;
#endif
    if (NULL_PTR != tpRxIndicationApiPtr)
    {
        tpRxIndicationApiPtr(bswModulePduId, result);
    }
}
#endif
/**
 * Init all PduR_SourceRouteStatus and PduR_DestinationRouteStatus
 */
void PduR_RouteInit(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (PDUR_ROUTINGPATH_MAX > 0u)
    for (PduIdType index = 0u; index < PduR_GetPduRRoutingPathNumOfPBConfig; ++index)
    {
        PduR_DestinationRouteStatus[index] = PDUR_RES_INITIAL;
    }
    for (PduIdType index = 0u; index < PduR_GetSrcPduNumOfPBConfig; ++index)
    {
        PduR_SourceRouteStatus[index] = PDUR_RES_INITIAL;
    }
#endif
}
/* ========================================== internal function definition ========================================== */
#if (                                                           \
    (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON) \
    || (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON))
/**
 * Call the target module interface transmit function
 */
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
#endif

#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
/**
 * Call the target module trigger transmit function
 */
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
/**
 * Call the target module interface transmit confirmation function
 */
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
/**
 * Call the target module interface receive indication function
 */
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
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON
/**
 * Call the target module transport protocol transmit function
 */
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
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
/**
 * Call the target module transport protocol copytxdata function
 */
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
/**
 * Call the target module transport protocol transmit confirmation function
 */
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
#endif
/**
 * Evaluate the PduR_SourceRouteStatus
 */
static boolean PduR_EvaluateUpLayerConfirmationStatus(PduIdType srcPduId)
{
    PduR_RouteStatusType txResult   = PduR_SourceRouteStatus[srcPduId];
    boolean              finishFlag = TRUE;
    if ((txResult == PDUR_RES_PENDING) || (txResult == PDUR_RES_OK))
    {
        uint8 destPduSum = PduR_GetDestPduNumOfRoutingPath(srcPduId);
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
                /* do nothing */
            }
        }
        if ((finishFlag) && (PduR_SourceRouteStatus[srcPduId] != PDUR_RES_OK))
        {
            PduR_SourceRouteStatus[srcPduId] = PDUR_RES_NOT_OK;
        }
    }
    return finishFlag;
}
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
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 3672 -- */
#endif
