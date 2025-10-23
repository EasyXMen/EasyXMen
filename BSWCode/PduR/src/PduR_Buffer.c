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
 **  @file               : PduR_Buffer.c
 **  @author             : xiaojian.liang
 **  @date               : 2023/12/5
 **  @vendor             : isoft
 **  @description        : Implementation for multipartition buffer
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "PduR_Internal.h"

#if ((STD_OFF == PDUR_ZERO_COST_OPERATION) && (PDUR_NUMBER_OF_QUEUES_MAX > 0u))
/* ===================================================== macros ===================================================== */
/**   DestPdu <==> Queue
 * Queue -> QueueDepth
 *       -> QueueBuffers
 *          Contains some dedicated buffer(may be shared to other queue) or global buffers, if no dedicated buffer
 *          avaliable, find an global buffer
 *       -> DedicatedBuffers
 * GlobalBuffers
 */
/* ================================================ type definitions ================================================ */

/* ============================================ internal data definition ============================================ */
/**
 * @brief Point to a PduR_BufferType* array of size depth in run time, init to ALL NULL PTR
 */
#if (PDUR_NUM_OF_QUEUES_DEPTH_SUM_MAX > 0u)
#define PDUR_START_SEC_VAR_CLEARED_PTR
#include "PduR_MemMap.h"
/* PRQA S 1514,3218 ++ */ /* VL_PduR_IndirectUse,VL_PduR_IndirectUse */
PDUR_LOCAL PduR_BufferType* PduR_BufferPtrsOfQueue[PDUR_NUM_OF_QUEUES_DEPTH_SUM_MAX];
/* PRQA S 1514,3218 -- */ /* VL_PduR_IndirectUse,VL_PduR_IndirectUse */
#define PDUR_STOP_SEC_VAR_CLEARED_PTR
#include "PduR_MemMap.h"
#endif
/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */
static inline void    PduR_BufferRelease(PduR_BufferType* bufferPtr);
static inline boolean PduR_QueueIsFull(const PduR_QueueRuntimeType* queuePtr);
static void           PduR_QueueEnqueue(const PduR_QueueRuntimeType* queuePtr, PduR_BufferType* bufferPtr);
static void           PduR_QueueFlush(PduR_QueueRuntimeType* queuePtr);
/* ========================================== external function definition ========================================== */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/**
 * Get queue tail buffer pointer
 */
PduR_BufferType* PduR_QueueGetTail(const PduR_QueueRuntimeType* queuePtr) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    PduR_BufferType* bufferPtr = NULL_PTR;
    for (PduR_QueueDepthType bufferIndex = queuePtr->Depth; bufferIndex > 0u; --bufferIndex)
    {
        if (queuePtr->BufferPtrs[bufferIndex - 1u] != NULL_PTR)
        {
            bufferPtr = queuePtr->BufferPtrs[bufferIndex - 1u];
            break;
        }
    }
    return bufferPtr;
}
/**
 * push a buffer pointer into queue
 */
static void PduR_QueueEnqueue(const PduR_QueueRuntimeType* queuePtr, PduR_BufferType* bufferPtr)
{
    for (PduR_QueueDepthType bufferIndex = 0u; bufferIndex < queuePtr->Depth; ++bufferIndex)
    {
        if (queuePtr->BufferPtrs[bufferIndex] == NULL_PTR)
        {
            queuePtr->BufferPtrs[bufferIndex] = bufferPtr;
            break;
        }
    }
}
/**
 * pop a front buffer in the queue
 */
void PduR_QueuePopFront(PduR_QueueRuntimeType* queuePtr)
{
    PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
    if (bufferPtr != NULL_PTR)
    {
        PduR_BufferRelease(bufferPtr);
        queuePtr->BufferPtrs[0] = NULL_PTR;
        queuePtr->CurPosition   = 0u;
        for (PduR_QueueDepthType bufferIndex = 0u; bufferIndex < (queuePtr->Depth - 1u); ++bufferIndex)
        {
            bufferPtr                              = queuePtr->BufferPtrs[bufferIndex + 1u];
            queuePtr->BufferPtrs[bufferIndex + 1u] = NULL_PTR;
            queuePtr->BufferPtrs[bufferIndex]      = bufferPtr;
            if (bufferPtr == NULL_PTR)
            {
                break;
            }
        }
    }
}
/**
 * pop a back buffer in the queue
 */
void PduR_QueuePopBack(const PduR_QueueRuntimeType* queuePtr) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    for (PduR_QueueDepthType bufferIndex = queuePtr->Depth; bufferIndex > 0u; --bufferIndex)
    {
        if (queuePtr->BufferPtrs[bufferIndex - 1u] != NULL_PTR)
        {
            PduR_BufferRelease(queuePtr->BufferPtrs[bufferIndex - 1u]);
            queuePtr->BufferPtrs[bufferIndex - 1u] = NULL_PTR;
            break;
        }
    }
}
/**
 * flush all buffer in the queue
 */
static void PduR_QueueFlush(PduR_QueueRuntimeType* queuePtr)
{
    while (TRUE) /* PRQA S 2740,0771 */ /* VL_PduR_Logical */
    {
        if (PduR_QueueGetHead(queuePtr) == NULL_PTR)
        {
            break;
        }
        PduR_QueuePopFront(queuePtr);
    }
}
/**
 * request a available buffer for the queue
 */
PduR_BufferType* PduR_QueueProvideBuffer(
    PduR_QueueRuntimeType* queuePtr,
    PduLengthType          requiredSize,
    uint8                  apiId,
    boolean*               flushFlag)
{
    if (PduR_QueueIsFull(queuePtr))
    {
        *flushFlag = TRUE;
        PduR_QueueFlush(queuePtr);
        /* FIFO was flushed, report PDUR_E_PDU_INSTANCES_LOST to the DET (SWS_PduR_00670) */
        /* TP only support single buffer (dependent configure tool) */
        if (queuePtr->Depth > 1u)
        {
            PduR_Det_ReportRuntimeError(apiId, PDUR_E_PDU_INSTANCES_LOST);
        }
    }

    PduR_BufferType*                 bufferPtr                           = NULL_PTR;
    const PduR_BufferPoolType* const bufferPoolPtrs[PDUR_KIND_OF_BUFFER] = {
        queuePtr->BufferPoolPtr,
        PduR_GetGlobalBufferPoolPtr()};

    for (uint8 bufferPoolIndex = 0u; (bufferPoolIndex < PDUR_KIND_OF_BUFFER) && (bufferPtr == NULL_PTR);
         ++bufferPoolIndex)
    {
        const PduR_BufferPoolType* bufferPoolPtr = bufferPoolPtrs[bufferPoolIndex];
        if (bufferPoolPtr != NULL_PTR)
        {
            for (PduR_BufferPoolSizeType bufferIndex = 0u; (bufferIndex < bufferPoolPtr->Size); ++bufferIndex)
            {
                PduR_BufferType* bufferPtrIter = &PduR_Buffers[bufferPoolPtr->Indexes[bufferIndex]];
                /*  PduRTxBuffer can be occupied by any of the referrer PduRRoutingPaths as dedicated buffer */
                if ((bufferPtrIter->LockState == PDUR_BUFFER_UNLOCK) && (bufferPtrIter->PduMaxLength >= requiredSize))
                {
                    bufferPtrIter->LockState = PDUR_BUFFER_LOCK;
                    bufferPtr                = bufferPtrIter;
                    break;
                }
            }
        }
    }

    if (bufferPtr != NULL_PTR)
    {
        PduR_QueueEnqueue(queuePtr, bufferPtr);
    }
    else
    {
        /* No buffer available, report PDUR_E_NO_BUFFER to the DET (SWS_PduR_00669) */
        PduR_Det_ReportRuntimeError(apiId, PDUR_E_NO_BUFFER);
    }
    return bufferPtr;
}
/**
 * request a available buffer and save a Pdu Info into the queue
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
PduR_QueueStoreType PduR_QueueStorePduInfo(
    PduR_QueueRuntimeType* queuePtr,
    uint8                  metaDataLength,
    const PduInfoType*     pduInfoPtr,
    uint8                  apiId)
/* PRQA S 1532 -- */
{
    PduR_QueueStoreType retval    = PDUR_QUEUESTORE_NOT_OK;
    boolean             flushFlag = FALSE;
    PduR_BufferType*    bufferPtr =
        PduR_QueueProvideBuffer(queuePtr, metaDataLength + pduInfoPtr->SduLength, apiId, &flushFlag);
    if (bufferPtr != NULL_PTR)
    {
        PduR_BufferStoreMetaData(bufferPtr, pduInfoPtr->MetaDataPtr, metaDataLength);
        PduR_BufferStoreSduData(bufferPtr, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
        retval = (flushFlag) ? PDUR_QUEUESTORE_FLUSH_OK : PDUR_QUEUESTORE_OK;
    }
    return retval;
}
/**
 * all data that has been copied and confirmed, be removed from the TP buffer
 */
void PduR_QueueDataConf(PduR_QueueRuntimeType* queuePtr, uint8 metaDataLength) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
    bufferPtr->PduCurLength -= queuePtr->CurPosition;
    (void)IStdLib_MemCpy(
        &bufferPtr->Data[metaDataLength],
        &bufferPtr->Data[metaDataLength + queuePtr->CurPosition],
        (PduLengthType)(bufferPtr->PduCurLength - (PduLengthType)metaDataLength));
    queuePtr->CurPosition = 0u;
}

/**
 * Queue handle for disable a routing path group.
 */
/* PRQA S 1532,1503 ++ */ /* VL_QAC_OneFunRef,VL_PduR_DiffProject */
void PduR_DisableRoutingQueueHandle(PduIdType destPduId, boolean initialize)
/* PRQA S 1532,1503 -- */
{
    /* FIFO: flush and report PDUR_E_PDU_INSTANCES_LOST to DET (if enable) (SWS_PduR_00663) */
    PduR_QueueRuntimeType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
    if (queuePtr != NULL_PTR)
    {
        /* PRQA S 3678 ++ */ /* VL_PduR_DiffProject */
        PduR_BufferType* bufferPtr = PduR_QueueGetHead(queuePtr);
        /* PRQA S 3678 -- */ /* VL_PduR_DiffProject */
        if (bufferPtr != NULL_PTR)
        {
            if (queuePtr->Depth == 1u)
            {
#if (PDUR_DEFAULT_VALUE_PDU_ENABLE == STD_ON)
                /* Single buffer: set to the default value with initialize set to true (SWS_PduR_00810)
                 */
                if (initialize)
                {
                    const PduR_DefaultValueType* defaultValuePtr = PduR_GetDefaultValuePtrOfDestPdu(destPduId);
                    uint8                        metaDataLength  = PduR_GetMetaDataLengthOfDestPdu(destPduId);
                    if (defaultValuePtr != NULL_PTR)
                    {
                        bufferPtr->PduCurLength = metaDataLength + defaultValuePtr->DefaultValueLength;
                        (void)IStdLib_MemCpy(
                            &bufferPtr->Data[metaDataLength],
                            &PduR_ConfigStd->PduRDefaultValuePtr[defaultValuePtr->DefaultValueStart],
                            defaultValuePtr->DefaultValueLength);
                    }
                }
                else
#endif
                {
                    /* Single buffer: do nothing with initialize set to false (SWS_PduR_00810) */
                }
            }
            else
            {
                /* Multiple buffer: buffer flush,report DET(SWS_PduR_00663) */
                PduR_QueueFlush(queuePtr);
                PduR_Det_ReportRuntimeError(PDUR_DISABLEROUTING_ID, PDUR_E_PDU_INSTANCES_LOST);
            }
        }
    }
    PDUR_NOUSED(initialize);
}

/**
 * Release all buffers (SWS_PduR_00645) and set to default value (SWS_PduR_00308)
 */
void PduR_BufferInit(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if (PduR_ConfigStd->PduRQueueNum > 0u)
    {
        for (uint16 num = 0u; num < PduR_ConfigStd->PduRQueueDepthSumNum; num++)
        {
            PduR_BufferPtrsOfQueue[num] = NULL_PTR;
        }

        PduR_QueueRuntimeType*   queueIterPtr = &PduR_RuntimeQueues[0];
        PduR_QueueDepthType      depthPreSum  = 0u;
        const PduR_QueueCfgType* queueCfgPtr  = PduR_ConfigStd->PduR_QueuesCfgPtr;
        for (uint16 num = 0u; num < PduR_ConfigStd->PduRQueueNum; num++)
        {
            queueIterPtr->Depth         = queueCfgPtr->Depth;
            queueIterPtr->CurPosition   = 0u;
            queueIterPtr->BufferPtrs    = &PduR_BufferPtrsOfQueue[depthPreSum];
            queueIterPtr->BufferPoolPtr = queueCfgPtr->BufferPoolPtr;
            depthPreSum += queueCfgPtr->Depth;
            queueCfgPtr++;
            queueIterPtr++;
        }
    }

    PduR_BufferType* bufferIterPtr = &PduR_Buffers[0];
    for (uint16 count = 0u; count < PDUR_NUMBER_OF_BUFFERS; count++)
    {
        bufferIterPtr->SduLength    = 0u;
        bufferIterPtr->PduCurLength = 0u;
        (void)IStdLib_MemSet(bufferIterPtr->Data, 0, bufferIterPtr->PduMaxLength);
        bufferIterPtr->LockState  = PDUR_BUFFER_UNLOCK;
        bufferIterPtr->WriteState = PDUR_BUFFER_WRITE_NOT_OK;
        bufferIterPtr++;
    }
#if (PDUR_DEFAULT_VALUE_PDU_ENABLE == STD_ON)
    if (PduR_ConfigStd->PduRDefaultValueNum > 0u)
    {
        for (PduIdType destPduId = 0u; destPduId < PduR_GetPduRRoutingPathNumOfPBConfig; ++destPduId)
        {
            const PduR_DefaultValueType* defaultValuePtr = PduR_GetDefaultValuePtrOfDestPdu(destPduId);
            if (defaultValuePtr != NULL_PTR)
            {
                PduR_QueueRuntimeType* queuePtr = PduR_GetQueuePtrOfDestPdu(destPduId);
                PduInfoType            pduInfo;
                pduInfo.MetaDataPtr = NULL_PTR;
                pduInfo.SduLength   = defaultValuePtr->DefaultValueLength;
                pduInfo.SduDataPtr  = (uint8*)&PduR_ConfigStd->PduRDefaultValuePtr[defaultValuePtr->DefaultValueStart];
                (void)PduR_QueueStorePduInfo(queuePtr, 0u, &pduInfo, PDUR_INIT_ID);
            }
        }
    }
#endif
}
/* ========================================== internal function definition ========================================== */
static inline void PduR_BufferRelease(PduR_BufferType* bufferPtr)
{
    bufferPtr->LockState    = PDUR_BUFFER_UNLOCK;
    bufferPtr->SduLength    = 0u;
    bufferPtr->PduCurLength = 0u;
    bufferPtr->WriteState   = PDUR_BUFFER_WRITE_NOT_OK;
}

static inline boolean PduR_QueueIsFull(const PduR_QueueRuntimeType* queuePtr)
{
    return (queuePtr->BufferPtrs[queuePtr->Depth - 1u] != NULL_PTR) ? TRUE : FALSE;
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /* end of PDUR_NUMBER_OF_QUEUES_MAX and PDUR_ZERO_COST_OPERATION */
