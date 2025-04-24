/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : PduR_Buffer.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : xiaojian.liang                                              **
**  Vendor      :                                                             **
**  DESCRIPTION : Multipartition Buffer Header                                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11            **
**                                                                            **
*******************************************************************************/
#ifndef PDUR_BUFFER_H
#define PDUR_BUFFER_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(PDUR_PUBLISHED_INFORMATION)
#define PDUR_PUBLISHED_INFORMATION
#define PDUR_MODULE_ID                   51u
#define PDUR_VENDOR_ID                   62u
#define PDUR_AR_RELEASE_MAJOR_VERSION    4u
#define PDUR_AR_RELEASE_MINOR_VERSION    5u
#define PDUR_AR_RELEASE_REVISION_VERSION 0u
#define PDUR_SW_MAJOR_VERSION            2u
#define PDUR_SW_MINOR_VERSION            3u
#define PDUR_SW_PATCH_VERSION            5u
#elif ((PDUR_SW_MAJOR_VERSION != 2u) || (PDUR_SW_MINOR_VERSION != 3u) || (PDUR_SW_PATCH_VERSION != 5u))
#error "PduR: Mismatch in Software Version"
#endif

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
void PduR_BufferInit(void);
void PduR_BufferRelease(PduR_BufferType* bufferPtr);
boolean PduR_QueueIsFull(const PduR_QueueType* queuePtr);
boolean PduR_QueueIsEmpty(const PduR_QueueType* queuePtr);
PduR_BufferType* PduR_QueueGetHead(const PduR_QueueType* queuePtr);
PduR_BufferType* PduR_QueueGetTail(PduIdType srcPduId, const PduR_QueueType* queuePtr);
void PduR_QueueEnqueue(const PduR_QueueType* queuePtr, PduR_BufferType* bufferPtr);
void PduR_QueuePopFront(PduR_QueueType* queuePtr);
void PduR_QueuePopBack(PduIdType srcPduId, const PduR_QueueType* queuePtr);
void PduR_QueueFlush(PduR_QueueType* queuePtr);
PduR_BufferType* PduR_QueueProvideBuffer(
    PduR_QueueType* queuePtr,
    PduLengthType requiredSize,
    uint8 apiId,
    boolean* flushFlag);
PduR_QueueStoreType PduR_QueueStorePduInfo(
    PduR_QueueType* queuePtr,
    PduR_MetaDataLengthType metaDataLength,
    const PduInfoType* pduInfoPtr,
    uint8 apiId);
void PduR_QueueSetForward(PduR_QueueType* queuePtr, PduLengthType length);
void PduR_QueueSetBack(PduR_QueueType* queuePtr, PduLengthType length);
void PduR_QueueDataConf(PduR_QueueType* queuePtr, PduR_MetaDataLengthType metaDataLength);
void PduR_BufferStoreSduLength(PduR_BufferType* bufferPtr, PduLengthType sduLength);
void PduR_BufferStoreMetaData(
    PduR_BufferType* bufferPtr,
    const uint8* metaDataPtr,
    PduR_MetaDataLengthType metaDataLength);
void PduR_BufferStoreSduData(PduR_BufferType* bufferPtr, const uint8* sduDataPtr, PduLengthType sduLength);
void PduR_Buffer2PduInfo(
    PduR_BufferType* bufferPtr,
    PduInfoType* pduInfoPtr,
    PduR_MetaDataLengthType metaDataLength,
    PduLengthType curPosition);
PduLengthType PduR_BufferGetRemainingSize(const PduR_BufferType* bufferPtr);

#endif /* end of PDUR_BUFFER_H */
/*******************************************************************************
**                      End Of File                                           **
*******************************************************************************/
