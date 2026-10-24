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
 **  @file               : PduR_Buffer.h
 **  @author             : xiaojian.liang
 **  @date               : 2023/12/5
 **  @vendor             : isoft
 **  @description        : Multipartition Buffer Header
 **
 ***********************************************************************************************************************/

#ifndef PDUR_BUFFER_H_
#define PDUR_BUFFER_H_

/* =================================================== inclusions =================================================== */
#ifdef __cplusplus
extern "C" {
#endif

#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_BufferInit(void);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_DisableRoutingQueueHandle(PduIdType destPduId, boolean initialize);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 5016 ++ */ /* VL_PduR_5016 */
PduR_BufferType* PduR_QueueGetTail(PduIdType srcPduId, const PduR_QueueRuntimeType* queuePtr);
/* PRQA S 5016 -- */ /* VL_PduR_5016 */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
boolean PduR_QueueIsEmpty(const PduR_QueueRuntimeType* queuePtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_QueuePopFront(PduR_QueueRuntimeType* queuePtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_QueuePopBack(PduIdType srcPduId, const PduR_QueueRuntimeType* queuePtr);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 5016 ++ */ /* VL_PduR_5016 */
PduR_BufferType* PduR_QueueProvideBuffer(
    PduR_QueueRuntimeType* queuePtr,
    PduLengthType          requiredSize,
    uint8                  apiId,
    boolean*               flushFlag);
/* PRQA S 5016 -- */ /* VL_PduR_5016 */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
PduR_QueueStoreType PduR_QueueStorePduInfo(
    PduR_QueueRuntimeType* queuePtr,
    uint8                  metaDataLength,
    const PduInfoType*     pduInfoPtr,
    uint8                  apiId);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
void PduR_QueueDataConf(PduR_QueueRuntimeType* queuePtr, uint8 metaDataLength);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
