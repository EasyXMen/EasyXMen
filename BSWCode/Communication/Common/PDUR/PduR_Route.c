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
**  FILENAME    : PduR_Route.c                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : xiaojian.liang                                              **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for PDUR Inter-Partition Routing             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Internal.h"

#if (                                                           \
    (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON) \
    || (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON))
static Std_ReturnType PduR_IfTransmitForward(PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
static Std_ReturnType PduR_TriggerTransmitForward(PduIdType srcPduId, PduInfoType* pduInfoPtr);
static void PduR_IfTxConfirmationForward(PduIdType srcPduId);
static void PduR_IfRxIndicationForward(PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON
static Std_ReturnType PduR_TpTransmitForward(PduIdType destPduId, const PduInfoType* pduInfoPtr);
#endif

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
static BufReq_ReturnType PduR_CopyTxDataForward(
    PduIdType srcPduId,
    const PduInfoType* pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType* availableDataPtr);
static void PduR_TpTxConfirmationForward(PduIdType srcPduId, Std_ReturnType result);
#endif

static void PduR_EvaluateUpLayerConfirmationStatus(PduIdType srcPduId);
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
Std_ReturnType PduR_PduRIfTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType retval = E_NOT_OK;
    PDUR_NOUSED(srcPduId);
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    retval = PduR_IfTransmitForward(destPduId, pduInfoPtr);
    if ((retval != E_NOT_OK) && PduR_GetTransmissionConfirmationOfDestPdu(destPduId))
    {
        PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
    }
#endif
    return retval;
}
#endif /* PDUR_TRANSMIT_SUPPORT == STD_ON */

#if (PDUR_TRANSMIT_SUPPORT == STD_ON)
Std_ReturnType PduR_PduRTpTransmit(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType retval = E_NOT_OK;
    PDUR_NOUSED(srcPduId);

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
    retval = PduR_TpTransmitForward(destPduId, pduInfoPtr);
#endif
    return retval;
}
#endif /* PDUR_TRANSMIT_SUPPORT == STD_ON */

#if (PDUR_CANCEL_TRANSMIT == STD_ON)
Std_ReturnType PduR_PduRCanceTransmit(PduIdType srcPduId, PduIdType destPduId)
{
    Std_ReturnType retval = E_NOT_OK;
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    ModuleIfTpCancelTransmitApiType cancelTransmitApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfDestPdu(destPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromDestPdu(bswModulePtr, destPduId);
    }
#endif

    switch (routeType)
    {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_TX_NOBUFFERED:
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
        cancelTransmitApi = bswModulePtr->IfCancelTransmitApi[funcIndex];
#else
        cancelTransmitApi = bswModulePtr->IfCancelTransmitApi;
#endif
        if (cancelTransmitApi != NULL_PTR)
        {
            retval = cancelTransmitApi(bswModulePduId);
        }
        break;
#endif /* PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON */
#if (PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON)
    case PDUR_ROUTE_TP_TX_NOBUFFERED:
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
        cancelTransmitApi = bswModulePtr->TpCancelTransmitApi[funcIndex];
#else
        cancelTransmitApi = bswModulePtr->TpCancelTransmitApi;
#endif
        if (cancelTransmitApi != NULL_PTR)
        {
            retval = cancelTransmitApi(bswModulePduId);
        }
        break;
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON */
    default:
        /* do nothing */
        break;
    }
    PDUR_NOUSED(srcPduId);
    return retval;
}
#endif /* PDUR_CANCEL_TRANSMIT == STD_ON */

#if (PDUR_CANCEL_RECEIVE == STD_ON)
Std_ReturnType PduR_PduRCancelReceive(PduIdType srcPduId, PduIdType destPduId)
{
    Std_ReturnType retval = E_NOT_OK;
#if (PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON)
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    ModuleCancelReceiveApiType cancelReceiveApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfSrcPdu(srcPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromSrcPdu(bswModulePtr, srcPduId);
    }
    cancelReceiveApi = bswModulePtr->CancelReceiveApi[funcIndex];
#else
    cancelReceiveApi = bswModulePtr->CancelReceiveApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if ((routeType == PDUR_ROUTE_TP_RX_NOBUFFERED) && (cancelReceiveApi != NULL_PTR))
    {
        retval = cancelReceiveApi(bswModulePduId);
    }
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON */
    return retval;
}
#endif /* PDUR_CANCEL_RECEIVE == STD_ON */

#if (PDUR_RX_INDICATION == STD_ON)
void PduR_PduRIfRxIndication(PduIdType srcPduId, PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
#if (PDUR_NUMBER_OF_QUEUES > 0u)
    PduR_MetaDataLengthType metaDataLength;
    PduR_QueueType* queuePtr;
#endif

    switch (routeType)
    {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_RX_NOBUFFERED:
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_INITIAL)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
            PduR_IfRxIndicationForward(destPduId, pduInfoPtr);
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        else
        {
            /* SWS_PduR_00903 In n:1,the most recent request has not returned,the PDU Router shall return immediately */
            PduR_Det_ReportRuntimeError(PDUR_IFRXINDICATION_ID, PDUR_E_PDU_INSTANCES_LOST);
        }
        break;
#if (PDUR_NUMBER_OF_QUEUES > 0u)
    case PDUR_ROUTE_IF_RX_BUFFERED:
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_INITIAL)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
            PduR_IfRxIndicationForward(destPduId, pduInfoPtr);

            queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
            metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
            while (!PduR_QueueIsEmpty(queuePtr))
            {
                PduInfoType pduInfoNext;
                SchM_Enter_PduR_ExclusiveArea_Route();
                PduR_BufferType* headBufferPtr = PduR_QueueGetHead(queuePtr);
                PduR_Buffer2PduInfo(headBufferPtr, &pduInfoNext, metaDataLength, 0u);
                PduR_QueuePopFront(queuePtr);
                SchM_Exit_PduR_ExclusiveArea_Route();
                PduR_IfRxIndicationForward(destPduId, &pduInfoNext);
            }
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        else
        {
            queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
            metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
            SchM_Enter_PduR_ExclusiveArea_Route();
            (void)PduR_QueueStorePduInfo(queuePtr, metaDataLength, pduInfoPtr, PDUR_IFRXINDICATION_ID);
            SchM_Exit_PduR_ExclusiveArea_Route();
        }
        break;
#endif
#endif
#if (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_GW_NOBUFFERED:
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_INITIAL)
        {
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
            (void)PduR_IfTransmitForward(destPduId, pduInfoPtr);
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
        }
        else
        {
            /* SWS_PduR_00903 In n:1,the most recent request has not returned,the PDU Router shall return immediately */
            PduR_Det_ReportRuntimeError(PDUR_IFRXINDICATION_ID, PDUR_E_PDU_INSTANCES_LOST);
        }
        break;
#if (PDUR_NUMBER_OF_QUEUES > 0u)
    case PDUR_ROUTE_IF_GW_BUFFERED: {
        PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
        PduR_MetaDataLengthType metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
        PduInfoType pduInfo = *pduInfoPtr;
        PduR_BufferType* headBufferPtr;
        const PduR_BufferType* tailBufferPtr;
        /* The rest of the received I-PDU shall be dropped if the configured length of the destination I-PDU is smaller
           than the received data length (PduLength of received I-PDU) (SWS_PduR_00746) */
        if (PduR_GetConfiguredLengthOfDestPdu(destPduId) < pduInfo.SduLength)
        {
            pduInfo.SduLength = PduR_GetConfiguredLengthOfDestPdu(destPduId);
        }
        if (PduR_GetDataProvisionOfDestPdu(destPduId) == PDUR_TRIGGERTRANSMIT)
        {
            /* Enqueue to FIFO and call <DstLo>_Transmit if FIFO queue is empty (SWS_PduR_00786) */
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_QueueStoreType retval =
                PduR_QueueStorePduInfo(queuePtr, metaDataLength, &pduInfo, PDUR_IFRXINDICATION_ID);
            headBufferPtr = PduR_QueueGetHead(queuePtr);
            tailBufferPtr = PduR_QueueGetTail(PDUR_SRC_PDU_SUM, queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
            if ((retval != PDUR_QUEUESTORE_NOT_OK) && (headBufferPtr == tailBufferPtr))
            {
                (void)PduR_IfTransmitForward(destPduId, &pduInfo);
            }
        }
        /* PduR_GetDataProvisionOfDestPdu(destPduId) == PDUR_DIRECT */
        else
        {
            if ((PduR_DestinationRouteStatus[destPduId] != PDUR_RES_PENDING) || (queuePtr->depth == 1u))
            {
                PduInfoType pduInfoNext;
                SchM_Enter_PduR_ExclusiveArea_Route();
                headBufferPtr = PduR_QueueGetHead(queuePtr);
                if ((headBufferPtr == NULL_PTR) || (queuePtr->depth == 1u))
                {
                    /* transmit current data */
                    pduInfoNext = pduInfo;
                    /* last is best */
                    if (queuePtr->depth == 1u)
                    {
                        PduR_QueuePopFront(queuePtr);
                    }
                }
                else
                {
                    /* transmit the oldest buffer data in FIFO, don't store the data even the transmit not ok */
                    PduR_Buffer2PduInfo(headBufferPtr, &pduInfoNext, metaDataLength, 0u);
                    PduR_QueuePopFront(queuePtr);
                }
                SchM_Exit_PduR_ExclusiveArea_Route();
                if (E_OK == PduR_IfTransmitForward(destPduId, &pduInfoNext))
                {
                    PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
                }
                else if (queuePtr->depth > 1u)
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
                    == PduR_QueueStorePduInfo(queuePtr, metaDataLength, &pduInfo, PDUR_IFRXINDICATION_ID))
                {
                    PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
                }
                SchM_Exit_PduR_ExclusiveArea_Route();
            }
        }
        break;
    }
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
#endif /* PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON */
    default:
        /* do nothing */
        break;
    }
}
#endif /* PDUR_RX_INDICATION == STD_ON */

#if (PDUR_TX_CONFIRMATION == STD_ON)
void PduR_PduRIfTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
{
    PDUR_NOUSED(result);

    if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_PENDING)
    {
        PduR_DestinationRouteStatus[destPduId] = PDUR_RES_OK;

        PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
        switch (routeType)
        {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
        case PDUR_ROUTE_IF_TX_NOBUFFERED:
            PduR_EvaluateUpLayerConfirmationStatus(srcPduId);
            if (PduR_SourceRouteStatus[srcPduId] == PDUR_RES_OK)
            {
                PduR_SourceRouteStatus[srcPduId] = PDUR_RES_INITIAL;
                if (PduR_GetSrcPduUpTxConfOfSrcPdu(srcPduId))
                {
                    PduR_IfTxConfirmationForward(srcPduId);
                }
            }
            break;
#endif /* PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON */
#if (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON)
#if (PDUR_NUMBER_OF_QUEUES > 0u)
        case PDUR_ROUTE_IF_GW_BUFFERED: {
            /* trigger transmit data provision is not interested in PduR_<DstLo>TxConfirmation (SWS_PduR_00640) */
            if (PduR_GetDataProvisionOfDestPdu(destPduId) == PDUR_TRIGGERTRANSMIT)
            {
                break;
            }

            PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
            /* Transmit next entry if Lower layer return E_NOT_OK (SWS_PduR_00788) */
            while (TRUE) /* PRQA S 0771 */ /* MISRA Rule 15.4 */
            {
                SchM_Enter_PduR_ExclusiveArea_Route();
                PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
                if (bufferPtr == NULL_PTR)
                {
                    SchM_Exit_PduR_ExclusiveArea_Route();
                    break;
                    SchM_Exit_PduR_ExclusiveArea_Route();
                }

                PduInfoType pduInfo;
                PduR_MetaDataLengthType metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
                PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, 0u);
                SchM_Exit_PduR_ExclusiveArea_Route();

                Std_ReturnType retval = PduR_IfTransmitForward(destPduId, &pduInfo);
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
                if (queuePtr->depth > 1u)
                {
                    PduR_Det_ReportRuntimeError(PDUR_IFTXCONFIRMATION_ID, PDUR_E_PDU_INSTANCES_LOST);
                }
            }
            break;
        }
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
#endif /* PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON */
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
#endif /* PDUR_TX_CONFIRMATION == STD_ON */

#if (PDUR_TRIGGER_TRANSMIT == STD_ON)
Std_ReturnType PduR_PduRTriggerTransmit(PduIdType srcPduId, PduIdType destPduId, PduInfoType* pduInfoPtr)
{
    Std_ReturnType retval = E_NOT_OK;
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);

    switch (routeType)
    {
#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
    case PDUR_ROUTE_IF_TX_NOBUFFERED:
        retval = PduR_TriggerTransmitForward(srcPduId, pduInfoPtr);
        if ((retval != E_NOT_OK) && PduR_GetTransmissionConfirmationOfDestPdu(destPduId))
        {
            PduR_SourceRouteStatus[srcPduId] = PDUR_RES_PENDING;
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
        }
        break;
#endif /* PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON */
#if (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON)
#if (PDUR_NUMBER_OF_QUEUES > 0u)
    case PDUR_ROUTE_IF_GW_BUFFERED: {
        PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
        SchM_Enter_PduR_ExclusiveArea_Route();
        PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
        if (bufferPtr == NULL_PTR)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }

        PduInfoType pduInfo;
        PduR_MetaDataLengthType metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, 0u);
        /* The PDU shall not be removed from the PduR buffer if the buffer is too small for the stored PDU data
         * (SWS_PduR_00819) */
        if (pduInfoPtr->SduLength < pduInfo.SduLength)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }
        SchM_Exit_PduR_ExclusiveArea_Route();
        pduInfoPtr->SduLength = pduInfo.SduLength;
        (void)ILib_memcpy(pduInfoPtr->MetaDataPtr, pduInfo.MetaDataPtr, metaDataLength);
        (void)ILib_memcpy(pduInfoPtr->SduDataPtr, pduInfo.SduDataPtr, pduInfoPtr->SduLength);
        retval = E_OK;
        /* The oldest FIFO entry shall be copied and then removed (SWS_PduR_00666) */
        /* The PDU Router shall buffer the latest I-PDU if last-is-best buffering is used (SWS_PduR_00809) */
        if (queuePtr->depth > 1u)
        {
            /* Transmit next entry if Lower layer return E_NOT_OK (SWS_PduR_00788) */
            while (TRUE) /* PRQA S 0771 */ /* MISRA Rule 15.4 */
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
                if (PduR_IfTransmitForward(destPduId, &pduInfo) == E_OK)
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
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
#endif /* PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON */
    default:
        /* do nothing */
        break;
    }
    return retval;
}
#endif /* PDUR_TRIGGER_TRANSMIT == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
BufReq_ReturnType PduR_PduRStartOfReceptionBuffered(
    PduIdType srcPduId,
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType tpSduLength,
    PduLengthType* bufferSizePtr)
{
    BufReq_ReturnType bufReqReturn = BUFREQ_E_NOT_OK;
    /* PduR shall allocate enough buffers from PduRTxBuffer (for each destination). (SWS_PduR_00830) */
    PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
    PduR_MetaDataLengthType metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
    PduLengthType requiredSize = (PduLengthType)PduR_GetTpThresholdOfDestPdu(destPduId);

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
    SchM_Exit_PduR_ExclusiveArea_Route();
    if (bufferPtr == NULL_PTR)
    {
        bufReqReturn = BUFREQ_E_OVFL;
        return bufReqReturn;
    }
    if (flushFlag)
    {
        PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
    }
    SchM_Enter_PduR_ExclusiveArea_Route();
    bufferPtr->srcPduId = srcPduId;
    bufferPtr->writeState = PDUR_BUFFER_WRITE_PENDING;
    PduR_BufferStoreSduLength(bufferPtr, tpSduLength);
    if (pduInfoPtr != NULL_PTR)
    {
        PduR_BufferStoreMetaData(bufferPtr, pduInfoPtr->MetaDataPtr, metaDataLength);
        PduR_BufferStoreSduData(bufferPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
    }
    *bufferSizePtr = PduR_BufferGetRemainingSize(bufferPtr);
    SchM_Exit_PduR_ExclusiveArea_Route();
    bufReqReturn = BUFREQ_OK;

    PduR_TpDestinationProcess(destPduId, PDUR_TPSTARTOFRECEPTION_ID);
    return bufReqReturn;
}

BufReq_ReturnType PduR_PduRCopyRxDataBuffered(
    PduIdType srcPduId,
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType* bufferSizePtr)
{
    BufReq_ReturnType bufReqReturn = BUFREQ_E_NOT_OK;
    const PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
    SchM_Enter_PduR_ExclusiveArea_Route();
    PduR_BufferType* bufferPtr = PduR_QueueGetTail(srcPduId, queuePtr);
    if ((bufferPtr == NULL_PTR) || (bufferPtr->writeState != PDUR_BUFFER_WRITE_PENDING))
    {
        SchM_Exit_PduR_ExclusiveArea_Route();
        return bufReqReturn;
    }

    PduLengthType bufferSize = PduR_BufferGetRemainingSize(bufferPtr);
    if (bufferSize < pduInfoPtr->SduLength)
    {
        bufferPtr->writeState = PDUR_BUFFER_WRITE_NOT_OK;
        SchM_Exit_PduR_ExclusiveArea_Route();
        return bufReqReturn;
    }

    PduR_BufferStoreSduData(bufferPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
    *bufferSizePtr = PduR_BufferGetRemainingSize(bufferPtr);
    SchM_Exit_PduR_ExclusiveArea_Route();
    bufReqReturn = BUFREQ_OK;
    PduR_TpDestinationProcess(destPduId, PDUR_TPCOPYRXDATA_ID);

    /* SWS_PduR_00804: In case of gatewaying between TPs, when all destinations fail, the reception side shall be
 stopped by returning BUFREQ_E_NOT_OK for the current call of CopyRxData or StartOfReception. */
    SchM_Enter_PduR_ExclusiveArea_Route();
    bufferPtr = PduR_QueueGetHead(queuePtr);
    SchM_Exit_PduR_ExclusiveArea_Route();
    if (bufferPtr == NULL_PTR)
    {
        bufReqReturn = BUFREQ_E_NOT_OK;
    }
    return bufReqReturn;
}

void PduR_PduRTpRxIndicationBuffered(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
{
    const PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
    SchM_Enter_PduR_ExclusiveArea_Route();
    PduR_BufferType* tailPtr = PduR_QueueGetTail(srcPduId, queuePtr);
    if (tailPtr != NULL_PTR)
    {
        tailPtr->writeState = ((result == E_OK) && (tailPtr->writeState == PDUR_BUFFER_WRITE_PENDING))
                                  ? PDUR_BUFFER_WRITE_OK
                                  : PDUR_BUFFER_WRITE_NOT_OK;

        if (tailPtr->writeState != PDUR_BUFFER_WRITE_OK)
        {
            if ((tailPtr != PduR_QueueGetHead(queuePtr))
                || (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_PENDING))
            {
                PduR_QueuePopBack(srcPduId, queuePtr);
            }
        }
    }
    SchM_Exit_PduR_ExclusiveArea_Route();
    PduR_TpDestinationProcess(destPduId, PDUR_TPCOPYRXDATA_ID);
}
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */

#if (PDUR_TP_COPYTXDATA == STD_ON)
BufReq_ReturnType PduR_PduRCopyTxData(
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType* availableDataPtr)
{
    BufReq_ReturnType bufReqReturn = BUFREQ_E_NOT_OK;
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
    PduIdType srcPduId;

    switch (routeType)
    {
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
    case PDUR_ROUTE_TP_TX_NOBUFFERED:
        srcPduId = PduR_GetSrcPduOfDestPdu(destPduId, 0u);
        bufReqReturn = PduR_CopyTxDataForward(srcPduId, pduInfoPtr, retryInfoPtr, availableDataPtr);
        break;
#endif
#if ((PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON) && (PDUR_NUMBER_OF_QUEUES > 0u))
    case PDUR_ROUTE_TP_GW_BUFFERED:
        if (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_PENDING)
        {
            break;
        }
        TpDataStateType tpDataState = (retryInfoPtr != NULL_PTR) ? retryInfoPtr->TpDataState : TP_CONFPENDING;
        PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
        SchM_Enter_PduR_ExclusiveArea_Route();
        PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
        if ((bufferPtr == NULL_PTR) || (bufferPtr->writeState == PDUR_BUFFER_WRITE_NOT_OK))
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }

        PduR_MetaDataLengthType metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
        PduInfoType pduInfo;

        if (tpDataState == TP_DATACONF)
        {
            /*TP_DATACONF: all data that has been copied before this call is confirmed and can be removed from the TP
             * buffer.*/
            PduR_QueueDataConf(queuePtr, metaDataLength);
        }
        else if (tpDataState == TP_DATARETRY)
        {
            if (retryInfoPtr->TxTpDataCnt <= queuePtr->curPosition)
            {
                PduR_QueueSetBack(queuePtr, retryInfoPtr->TxTpDataCnt);
            }
            else
            {
                SchM_Exit_PduR_ExclusiveArea_Route();
                break;
            }
        }
        else
        {
            /* do nothing */
        }

        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, queuePtr->curPosition);
        if (pduInfo.SduLength < pduInfoPtr->SduLength)
        {
            bufReqReturn = BUFREQ_E_BUSY;
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }

        (void)ILib_memcpy(pduInfoPtr->SduDataPtr, pduInfo.SduDataPtr, pduInfoPtr->SduLength);
        PduR_QueueSetForward(queuePtr, pduInfoPtr->SduLength);
        if (retryInfoPtr == NULL_PTR)
        {
            PduR_QueueDataConf(queuePtr, metaDataLength);
        }
        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, queuePtr->curPosition);
        SchM_Exit_PduR_ExclusiveArea_Route();
        *availableDataPtr = pduInfo.SduLength;
        bufReqReturn = BUFREQ_OK;
        break;
#endif
    default:
        /* do nothing */
        break;
    }
    return bufReqReturn;
}
#endif /* PDUR_TP_COPYTXDATA == STD_ON */

#if (PDUR_TP_TXCONFIRMATION == STD_ON)
void PduR_PduRTpTxConfirmation(PduIdType srcPduId, PduIdType destPduId, Std_ReturnType result)
{
    PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
    switch (routeType)
    {
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
    case PDUR_ROUTE_TP_TX_NOBUFFERED:
        PduR_TpTxConfirmationForward(srcPduId, result);
        break;
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON */
#if (PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON)
    case PDUR_ROUTE_TP_GW_BUFFERED:
        if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_PENDING)
        {
            PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_QueuePopFront(queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
            PduR_DestinationRouteStatus[destPduId] = PDUR_RES_INITIAL;
            PduR_TpDestinationProcess(destPduId, PDUR_TPTXCONFIRMATION_ID);
        }
        break;
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON */
    default:
        /* do nothing */
        break;
    }
}
#endif /* PDUR_TP_TXCONFIRMATION == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
void PduR_TpDestinationProcess(PduIdType destPduId, uint8 apiId)
{
#if (PDUR_NUMBER_OF_QUEUES > 0u)
    PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
    PduR_MetaDataLengthType metaDataLength = PduR_GetMetaDataLengthOfDestPdu(destPduId);
    PduLengthType tpThreshold = (PduLengthType)PduR_GetTpThresholdOfDestPdu(destPduId);

    if (PduR_DestinationRouteStatus[destPduId] != PDUR_RES_INITIAL)
    {
        return;
    }

    while (TRUE) /* PRQA S 0771 */ /* MISRA Rule 15.4 */
    {
        SchM_Enter_PduR_ExclusiveArea_Route();
        PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
        if (bufferPtr == NULL_PTR)
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }

        PduInfoType pduInfo;
        PduR_Buffer2PduInfo(bufferPtr, &pduInfo, metaDataLength, 0u);
        if ((pduInfo.SduLength < tpThreshold) && (bufferPtr->writeState != PDUR_BUFFER_WRITE_OK))
        {
            SchM_Exit_PduR_ExclusiveArea_Route();
            break;
        }
        SchM_Exit_PduR_ExclusiveArea_Route();
        pduInfo.SduLength = bufferPtr->sduLength;
        PduR_RouteType routeType = PduR_GetRouteTypeOfDestPdu(destPduId);
#if PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON
        if (routeType == PDUR_ROUTE_TP_GW_BUFFERED)
        {
            pduInfo.SduDataPtr = NULL_PTR;
            if (PduR_TpTransmitForward(destPduId, &pduInfo) == E_OK)
            {
                PduR_DestinationRouteStatus[destPduId] = PDUR_RES_PENDING;
                break;
            }
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_QueuePopFront(queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
            PduR_Det_ReportRuntimeError(apiId, PDUR_E_PDU_INSTANCES_LOST);
            continue;
        }
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON */
#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
        if (routeType == PDUR_ROUTE_TP_RX_BUFFERED)
        {
            PduLengthType bufferSize;
            BufReq_ReturnType bufReqReturn =
                PduR_StartOfReceptionForward(destPduId, &pduInfo, pduInfo.SduLength, &bufferSize);
            PduR_TpRxIndicationForward(destPduId, bufReqReturn == BUFREQ_OK ? E_OK : E_NOT_OK);
            SchM_Enter_PduR_ExclusiveArea_Route();
            PduR_QueuePopFront(queuePtr);
            SchM_Exit_PduR_ExclusiveArea_Route();
        }
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */
    }
#else
    PDUR_NOUSED(destPduId);
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
}
#endif /* PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */

static void PduR_EvaluateUpLayerConfirmationStatus(PduIdType srcPduId)
{
    PduR_RouteStatusType txResult = PduR_SourceRouteStatus[srcPduId];
    boolean funishFlag = FALSE;
    if (txResult == PDUR_RES_PENDING)
    {
        uint8 destPduSum = PduR_GetDestPduNumOfSrcPdu(srcPduId);
        for (uint8 index = 0u; index < destPduSum; ++index)
        {
            PduIdType destPduId = PduR_GetDestPduOfRoutingPath(srcPduId, index);
            if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_NOT_OK)
            {
                txResult = PDUR_RES_NOT_OK;
                funishFlag = TRUE;
            }
            if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_PENDING)
            {
                txResult = PDUR_RES_PENDING;
                funishFlag = TRUE;
            }
            if (PduR_DestinationRouteStatus[destPduId] == PDUR_RES_OK)
            {
                txResult = PDUR_RES_OK;
            }
            if (funishFlag)
            {
                break;
            }
        }
        /* All dest route get confirmation,source route state set OK. */
        PduR_SourceRouteStatus[srcPduId] = txResult;
    }
}

#if (                                                           \
    (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON) \
    || (PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED == STD_ON))
static Std_ReturnType PduR_IfTransmitForward(PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    Std_ReturnType retval = E_NOT_OK;
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    ModuleIfTransmitApiType ifTransmitApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfDestPdu(destPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromDestPdu(bswModulePtr, destPduId);
    }

    ifTransmitApi = bswModulePtr->IfTransmitApi[funcIndex];
#else
    ifTransmitApi = bswModulePtr->IfTransmitApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != ifTransmitApi)
    {
        retval = ifTransmitApi(bswModulePduId, pduInfoPtr);
    }
    return retval;
}
#endif

#if (PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON)
static Std_ReturnType PduR_TriggerTransmitForward(PduIdType srcPduId, PduInfoType* pduInfoPtr)
{
    Std_ReturnType retval = E_NOT_OK;
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    ModuleTriggertransmitApiType triggertransmitApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfSrcPdu(srcPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromSrcPdu(bswModulePtr, srcPduId);
    }
    triggertransmitApi = bswModulePtr->TriggertransmitApi[funcIndex];
#else
    triggertransmitApi = bswModulePtr->TriggertransmitApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != triggertransmitApi)
    {
        retval = triggertransmitApi(bswModulePduId, pduInfoPtr);
    }
    return retval;
}

static void PduR_IfTxConfirmationForward(PduIdType srcPduId)
{
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    ModuleTxConfirmationApiType txConfirmationApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfSrcPdu(srcPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromSrcPdu(bswModulePtr, srcPduId);
    }
    txConfirmationApi = bswModulePtr->TxConfirmationApi[funcIndex];
#else
    txConfirmationApi = bswModulePtr->TxConfirmationApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != txConfirmationApi)
    {
        txConfirmationApi(bswModulePduId);
    }
}

static void PduR_IfRxIndicationForward(PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    ModuleIfRxIndicationApiType ifRxIndicationApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfDestPdu(destPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromDestPdu(bswModulePtr, destPduId);
    }
    ifRxIndicationApi = bswModulePtr->IfRxIndicationApi[funcIndex];
#else
    ifRxIndicationApi = bswModulePtr->IfRxIndicationApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != ifRxIndicationApi)
    {
        ifRxIndicationApi(bswModulePduId, pduInfoPtr);
    }
}
#endif /* PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON
static Std_ReturnType PduR_TpTransmitForward(PduIdType destPduId, const PduInfoType* pduInfoPtr)
{
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    Std_ReturnType retval = E_NOT_OK;
    ModuleTpTransmitApiType tpTransmitApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfDestPdu(destPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromDestPdu(bswModulePtr, destPduId);
    }
    tpTransmitApi = bswModulePtr->TpTransmitApi[funcIndex];
#else
    tpTransmitApi = bswModulePtr->TpTransmitApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != tpTransmitApi)
    {
        retval = tpTransmitApi(bswModulePduId, pduInfoPtr);
    }
    return retval;
}
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON
static BufReq_ReturnType PduR_CopyTxDataForward(
    PduIdType srcPduId,
    const PduInfoType* pduInfoPtr,
    const RetryInfoType* retryInfoPtr,
    PduLengthType* availableDataPtr)
{
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    BufReq_ReturnType retval = BUFREQ_E_NOT_OK;
    ModuleCopyTxDataApiType copyTxDataApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfSrcPdu(srcPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromSrcPdu(bswModulePtr, srcPduId);
    }
    copyTxDataApi = bswModulePtr->CopyTxDataApi[funcIndex];
#else
    copyTxDataApi = bswModulePtr->CopyTxDataApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != copyTxDataApi)
    {
        retval = copyTxDataApi(bswModulePduId, pduInfoPtr, retryInfoPtr, availableDataPtr);
    }
    return retval;
}

static void PduR_TpTxConfirmationForward(PduIdType srcPduId, Std_ReturnType result)
{
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfSrcPdu(srcPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfSrcPdu(srcPduId);
    ModuleTpTxConfirmationApiType tpTxConfirmationApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfSrcPdu(srcPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromSrcPdu(bswModulePtr, srcPduId);
    }
    tpTxConfirmationApi = bswModulePtr->TpTxConfirmationApi[funcIndex];
#else
    tpTxConfirmationApi = bswModulePtr->TpTxConfirmationApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != tpTxConfirmationApi)
    {
        tpTxConfirmationApi(bswModulePduId, result);
    }
}
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
BufReq_ReturnType PduR_StartOfReceptionForward(
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType tpSduLength,
    PduLengthType* bufferSizePtr)
{
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    BufReq_ReturnType bufReqReturn = BUFREQ_E_NOT_OK;
    ModuleStartOfReceptionApiType startOfReceptionApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfDestPdu(destPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromDestPdu(bswModulePtr, destPduId);
    }
    startOfReceptionApi = bswModulePtr->StartOfReceptionApi[funcIndex];
#else
    startOfReceptionApi = bswModulePtr->StartOfReceptionApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != startOfReceptionApi)
    {
        bufReqReturn = startOfReceptionApi(bswModulePduId, pduInfoPtr, tpSduLength, bufferSizePtr);
    }
    return bufReqReturn;
}
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */

#if (PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON)
BufReq_ReturnType PduR_CopyRxDataForward(
    PduIdType destPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType* bufferSizePtr)
{
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    BufReq_ReturnType bufReqReturn = BUFREQ_E_NOT_OK;
    ModuleCopyRxDataApiType copyRxDataApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfDestPdu(destPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromDestPdu(bswModulePtr, destPduId);
    }
    copyRxDataApi = bswModulePtr->CopyRxDataApi[funcIndex];
#else
    copyRxDataApi = bswModulePtr->CopyRxDataApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != copyRxDataApi)
    {
        bufReqReturn = copyRxDataApi(bswModulePduId, pduInfoPtr, bufferSizePtr);
    }
    return bufReqReturn;
}
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON */

#if PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
    || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON
void PduR_TpRxIndicationForward(PduIdType destPduId, Std_ReturnType result)
{
    const PduRBswModuleType* bswModulePtr = PduR_GetBswModulePtrOfDestPdu(destPduId);
    PduIdType bswModulePduId = PduR_GetBswModulePduIdOfDestPdu(destPduId);
    ModuleTpRxIndicationApiType tpRxIndicationApi;
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
    uint8 funcIndex = 0u;
    if (PduR_GetPartitionIndexOfDestPdu(destPduId) != PDUR_UNUSED_UINT8)
    {
        funcIndex = PduR_GetPartitionFuncIndexFromDestPdu(bswModulePtr, destPduId);
    }
    tpRxIndicationApi = bswModulePtr->TpRxIndicationApi[funcIndex];
#else
    tpRxIndicationApi = bswModulePtr->TpRxIndicationApi;
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
    if (NULL_PTR != tpRxIndicationApi)
    {
        tpRxIndicationApi(bswModulePduId, result);
    }
}
#endif /* PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED == STD_ON \
          || PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED == STD_ON */

void PduR_RouteInit(void)
{
#if ((PDUR_DEST_PDU_SUM > 0u) && (PDUR_SRC_PDU_SUM > 0u))
    for (PduIdType index = 0; index < PDUR_DEST_PDU_SUM; ++index)
    {
        PduR_DestinationRouteStatus[index] = PDUR_RES_INITIAL;
    }
    for (PduIdType index = 0; index < PDUR_SRC_PDU_SUM; ++index)
    {
        PduR_SourceRouteStatus[index] = PDUR_RES_INITIAL;
    }
#endif
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
