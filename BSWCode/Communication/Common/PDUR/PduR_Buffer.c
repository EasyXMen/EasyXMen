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
**  FILENAME    : PduR_Buffer.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : xiaojian.liang                                              **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for multipartition buffer                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Internal.h"

#if (PDUR_NUMBER_OF_QUEUES > 0u)
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* DestPdu <==> Queue
Queue -> QueueDepth
      -> QueueBuffers
         Contains some dedicated buffer(may be shared to other queue) or global buffers, if no dedicated buffer
         avaliable, find an global buffer
      -> DedicatedBuffers

GlobalBuffers */

boolean PduR_QueueIsFull(const PduR_QueueType* queuePtr)
{
    return (queuePtr->bufferPtrs[queuePtr->depth - 1u] != NULL_PTR) ? TRUE : FALSE;
}

boolean PduR_QueueIsEmpty(const PduR_QueueType* queuePtr)
{
    return (queuePtr->bufferPtrs[0u] != NULL_PTR) ? TRUE : FALSE;
}

PduR_BufferType* PduR_QueueGetHead(const PduR_QueueType* queuePtr)
{
    return queuePtr->bufferPtrs[0];
}

PduR_BufferType* PduR_QueueGetTail(PduIdType srcPduId, const PduR_QueueType* queuePtr)
{
    PduR_BufferType* bufferPtr = NULL_PTR;
    for (PduR_QueueDepthType bufferIndex = queuePtr->depth; bufferIndex > 0u; --bufferIndex)
    {
        if ((queuePtr->bufferPtrs[bufferIndex - 1u] != NULL_PTR)
            && ((PDUR_SRC_PDU_SUM == srcPduId) || (queuePtr->bufferPtrs[bufferIndex - 1u]->srcPduId == srcPduId)))
        {
            bufferPtr = queuePtr->bufferPtrs[bufferIndex - 1u];
            break;
        }
    }
    return bufferPtr;
}

void PduR_QueueEnqueue(const PduR_QueueType* queuePtr, PduR_BufferType* bufferPtr)
{
    for (PduR_QueueDepthType bufferIndex = 0u; bufferIndex < queuePtr->depth; ++bufferIndex)
    {
        if (queuePtr->bufferPtrs[bufferIndex] == NULL_PTR)
        {
            queuePtr->bufferPtrs[bufferIndex] = bufferPtr;
            break;
        }
    }
}

void PduR_QueuePopFront(PduR_QueueType* queuePtr)
{
    PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
    if (bufferPtr != NULL_PTR)
    {
        PduR_BufferRelease(bufferPtr);
        queuePtr->bufferPtrs[0] = NULL_PTR;
        queuePtr->curPosition = 0u;
        queuePtr->curSduLength = 0u;
        for (PduR_QueueDepthType bufferIndex = 0u; bufferIndex < (queuePtr->depth - 1u); ++bufferIndex)
        {
            bufferPtr = queuePtr->bufferPtrs[bufferIndex + 1u];
            queuePtr->bufferPtrs[bufferIndex + 1u] = NULL_PTR;
            queuePtr->bufferPtrs[bufferIndex] = bufferPtr;
            if (bufferPtr == NULL_PTR)
            {
                break;
            }
        }
    }
}

void PduR_QueuePopBack(PduIdType srcPduId, const PduR_QueueType* queuePtr)
{
    for (PduR_QueueDepthType bufferIndex = queuePtr->depth; bufferIndex > 0u; --bufferIndex)
    {
        if ((queuePtr->bufferPtrs[bufferIndex - 1u] != NULL_PTR)
            && ((PDUR_SRC_PDU_SUM == srcPduId) || (queuePtr->bufferPtrs[bufferIndex - 1u]->srcPduId == srcPduId)))
        {
            PduR_BufferRelease(queuePtr->bufferPtrs[bufferIndex - 1u]);
            queuePtr->bufferPtrs[bufferIndex - 1u] = NULL_PTR;
            for (PduR_QueueDepthType bufIndex = bufferIndex; bufIndex < queuePtr->depth; ++bufIndex)
            {
                if (queuePtr->bufferPtrs[bufIndex] == NULL_PTR)
                {
                    break;
                }
                else
                {
                    queuePtr->bufferPtrs[bufIndex - 1u] = queuePtr->bufferPtrs[bufIndex];
                    queuePtr->bufferPtrs[bufIndex] = NULL_PTR;
                }
            }
            break;
        }
    }
}

void PduR_QueueFlush(PduR_QueueType* queuePtr)
{
    while (TRUE)
    {
        if (PduR_QueueGetHead(queuePtr) == NULL_PTR)
        {
            break;
        }
        PduR_QueuePopFront(queuePtr);
    }
}

PduR_BufferType* PduR_QueueProvideBuffer(
    PduR_QueueType* queuePtr,
    PduLengthType requiredSize,
    uint8 apiId,
    boolean* flushFlag)
{
    if (PduR_QueueIsFull(queuePtr))
    {
        *flushFlag = TRUE;
        PduR_QueueFlush(queuePtr);
        /* FIFO was flushed, report PDUR_E_PDU_INSTANCES_LOST to the DET (SWS_PduR_00670) */
        /* TP only support single buffer (dependent configure tool) */
        if (queuePtr->depth > 1u)
        {
            PduR_Det_ReportRuntimeError(apiId, PDUR_E_PDU_INSTANCES_LOST);
        }
    }

    PduR_BufferType* bufferPtr = NULL_PTR;
    const PduR_BufferPoolType* const bufferPoolPtrs[2] = {queuePtr->bufferPoolPtr, PduR_GetGlobalBufferPoolPtr()};

    for (uint8 bufferPoolIndex = 0u; (bufferPoolIndex < 2u) && (bufferPtr == NULL_PTR); ++bufferPoolIndex)
    {
        const PduR_BufferPoolType* bufferPoolPtr = bufferPoolPtrs[bufferPoolIndex];
        if (bufferPoolPtr == NULL_PTR)
        {
            continue;
        }
        for (PduR_BufferPoolSizeType bufferIndex = 0u; (bufferIndex < bufferPoolPtr->size) && (bufferPtr == NULL_PTR);
             ++bufferIndex)
        {
            PduR_BufferType* bufferPtrIter = &PduR_Buffers[bufferPoolPtr->indexes[bufferIndex]];
            /*  PduRTxBuffer can be occupied by any of the referrer PduRRoutingPaths as dedicated buffer */
            if ((bufferPtrIter->lockState == PDUR_BUFFER_UNLOCK) && (bufferPtrIter->pduMaxLength >= requiredSize))
            {
                bufferPtrIter->lockState = PDUR_BUFFER_LOCK;
                bufferPtr = bufferPtrIter;
                break;
            }
        }
    }

    if (bufferPtr != NULL_PTR)
    {
        PduR_QueueEnqueue(queuePtr, bufferPtr);
    }
    return bufferPtr;
}

PduR_QueueStoreType PduR_QueueStorePduInfo(
    PduR_QueueType* queuePtr,
    PduR_MetaDataLengthType metaDataLength,
    const PduInfoType* pduInfoPtr,
    uint8 apiId)
{
    PduR_QueueStoreType retval = PDUR_QUEUESTORE_NOT_OK;
    boolean flushFlag = FALSE;
    PduR_BufferType* bufferPtr =
        PduR_QueueProvideBuffer(queuePtr, metaDataLength + pduInfoPtr->SduLength, apiId, &flushFlag);
    if (bufferPtr != NULL_PTR)
    {
        PduR_BufferStoreMetaData(bufferPtr, pduInfoPtr->MetaDataPtr, metaDataLength);
        PduR_BufferStoreSduData(bufferPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
        retval = (flushFlag) ? PDUR_QUEUESTORE_FLUSH_OK : PDUR_QUEUESTORE_OK;
    }
    return retval;
}

void PduR_QueueSetForward(PduR_QueueType* queuePtr, PduLengthType length)
{
    queuePtr->curPosition += length;
}

void PduR_QueueSetBack(PduR_QueueType* queuePtr, PduLengthType length)
{
    queuePtr->curPosition -= length;
}

void PduR_QueueDataConf(PduR_QueueType* queuePtr, PduR_MetaDataLengthType metaDataLength)
{
    PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
    bufferPtr->pduCurLength -= queuePtr->curPosition;
    (void)ILib_memcpy(
        &bufferPtr->data[metaDataLength],
        &bufferPtr->data[metaDataLength + queuePtr->curPosition],
        (PduLengthType)(bufferPtr->pduCurLength - (PduLengthType)metaDataLength));
    queuePtr->curPosition = 0u;
}

void PduR_BufferStoreSduLength(PduR_BufferType* bufferPtr, PduLengthType sduLength)
{
    bufferPtr->sduLength = sduLength;
}

/* Shall be invoke at the start of reception or transmition and and throughout only once in reception or transmission.
 */
void PduR_BufferStoreMetaData(
    PduR_BufferType* bufferPtr,
    const uint8* metaDataPtr,
    PduR_MetaDataLengthType metaDataLength)
{
    (void)ILib_memcpy(bufferPtr->data, metaDataPtr, metaDataLength);
    bufferPtr->pduCurLength = metaDataLength;
}

void PduR_BufferStoreSduData(PduR_BufferType* bufferPtr, const uint8* sduDataPtr, PduLengthType sduLength)
{
    (void)ILib_memcpy(&bufferPtr->data[bufferPtr->pduCurLength], sduDataPtr, sduLength);
    bufferPtr->pduCurLength += sduLength;
}

void PduR_Buffer2PduInfo(
    PduR_BufferType* bufferPtr,
    PduInfoType* pduInfoPtr,
    PduR_MetaDataLengthType metaDataLength,
    PduLengthType curPosition)
{
    pduInfoPtr->MetaDataPtr = (metaDataLength == 0u) ? NULL_PTR : bufferPtr->data;
    pduInfoPtr->SduDataPtr = &bufferPtr->data[metaDataLength + curPosition];
    pduInfoPtr->SduLength = bufferPtr->pduCurLength - metaDataLength - curPosition;
}

/* Release all buffers (SWS_PduR_00645) and set to default value (SWS_PduR_00308) */
void PduR_BufferInit(void)
{
    PduR_QueueType* queueIterPtr = &PduR_Queues[0];
    for (; queueIterPtr < &PduR_Queues[PDUR_NUMBER_OF_QUEUES]; ++queueIterPtr)
    {
        queueIterPtr->curPosition = 0u;
        queueIterPtr->curSduLength = 0u;
        for (PduR_QueueDepthType index = 0u; index < queueIterPtr->depth; ++index)
        {
            queueIterPtr->bufferPtrs[index] = NULL_PTR;
        }
    }

    PduR_BufferType* bufferIterPtr = &PduR_Buffers[0];
    for (; bufferIterPtr < &PduR_Buffers[PDUR_NUMBER_OF_BUFFERS]; ++bufferIterPtr)
    {
        bufferIterPtr->sduLength = 0u;
        bufferIterPtr->pduCurLength = 0u;
        (void)ILib_memset(bufferIterPtr->data, 0, bufferIterPtr->pduMaxLength);
        bufferIterPtr->lockState = PDUR_BUFFER_UNLOCK;
        bufferIterPtr->writeState = PDUR_BUFFER_WRITE_NOT_OK;
        bufferIterPtr->srcPduId = PDUR_SRC_PDU_SUM;
    }

#if (PDUR_DEFAULT_VALUE_ENABLED == STD_ON)
    for (PduIdType destPduId = 0u; destPduId < PDUR_DEST_PDU_SUM; ++destPduId)
    {
        PduRDefaultValueType* defaultValuePtr = PduR_GetDefaultValuePtrOfDestPdu(destPduId);
        if (defaultValuePtr == NULL_PTR)
        {
            continue;
        }

        const PduR_QueueType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
        PduInfoType pduInfo;
        pduInfo.MetaDataPtr = NULL_PTR;
        pduInfo.SduLength = defaultValuePtr->DefaultValueLength;
        pduInfo.SduDataPtr = &PduR_Default_value[defaultValuePtr->DefaultValueStart];
        (void)PduR_QueueStorePduInfo(queuePtr, 0u, &pduInfo, PDUR_INIT_ID);
    }
#endif /* PDUR_DEFAULT_VALUE_ENABLED == STD_ON */
}

void PduR_BufferRelease(PduR_BufferType* bufferPtr)
{
    bufferPtr->lockState = PDUR_BUFFER_UNLOCK;
    bufferPtr->sduLength = 0u;
    bufferPtr->pduCurLength = 0u;
    bufferPtr->writeState = PDUR_BUFFER_WRITE_NOT_OK;
    bufferPtr->srcPduId = PDUR_SRC_PDU_SUM;
}

PduLengthType PduR_BufferGetRemainingSize(const PduR_BufferType* bufferPtr)
{
    return bufferPtr->pduMaxLength - bufferPtr->pduCurLength;
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
/*=======[E N D   O F   F I L E]==============================================*/
