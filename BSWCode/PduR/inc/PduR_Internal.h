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
 **  @file               : PduR_Internal.h
 **  @author             : zhengfei.li
 **  @date               : 2020/06/22
 **  @vendor             : isoft
 **  @description        : PDUR internal header for internal API declarations
 **
 ***********************************************************************************************************************/

#ifndef PDUR_INTERNAL_H_
#define PDUR_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "IStdLib.h"
#include "Det.h"
#include "SchM_PduR.h"
#include "PduR.h"
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
#include "Os.h"
#endif
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
#include "PduR_Buffer.h"
#endif
#include "PduR_Route.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */
/* Using for Conditional compilation PB-Config */
/* PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED
   PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED
   PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED
   PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED
   PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED */
#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/**
 * @brief The followings are configuration shortcuts
 */
#define PDUR_ROUTEPATHGROUP_CFG(id)                   PduR_ConfigStd->PduRRoutingPathGroupRef[id]
#define PDUR_DESTPDU_CFG(id)                          PduR_ConfigStd->PduRDestPduRef[id]
#define PDUR_SRCEPDU_CFG(id)                          PduR_ConfigStd->PduRSrcPduRef[id]
#define PDUR_ROUTINGPATH_CFG(id)                      PduR_ConfigStd->PduRRoutingTableRef[0u].PduRRoutingPathRef[id]
#define PduR_GetPduRRoutingPathNumOfPBConfig          PduR_ConfigStd->PduRRoutingPathNum
#define PduR_GetRoutingPathGroupCntOfPBConfig         PduR_ConfigStd->RoutingPathGroupCnt
#define PduR_GetSrcPduNumOfPBConfig                   PduR_ConfigStd->PduRSrcPduNum
#define PduR_GetDestPduNumOfSrcPdu(srcPduId)          PDUR_ROUTINGPATH_CFG(srcPduId).PduDestSum
#define PduR_GetDestPduOfRoutingPath(srcPduId, index) PDUR_ROUTINGPATH_CFG(srcPduId).PduRDestPduIdRef[index]
#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
#define PduR_GetBufDataInterPartitionOfRoutingPath(srcPduId) PDUR_ROUTINGPATH_CFG(srcPduId).PduRBufDataInterPartitionRef
#endif
#define PduR_GetSrcPduUpTxConfOfSrcPdu(srcPduId)   PDUR_SRCEPDU_CFG(srcPduId).PduRSrcPduUpTxConf
#define PduR_GetBswModulePduIdOfSrcPdu(pduId)      PDUR_SRCEPDU_CFG(pduId).PduRSrcModulePduIndex
#define PduR_GetSrcPduNumOfDestPdu(destPduId)      PDUR_DESTPDU_CFG(destPduId).PduSrcSum
#define PduR_GetSrcPduOfDestPdu(destPduId, index)  PDUR_DESTPDU_CFG(destPduId).PduRSrcPduRef[index]
#define PduR_GetDataProvisionOfDestPdu(destPduId)  PDUR_DESTPDU_CFG(destPduId).PduRDestPduDataProvision
#define PduR_GetBswModulePduIdOfDestPdu(destPduId) PDUR_DESTPDU_CFG(destPduId).PduRDestModulePduIndex
#if (PDUR_NUMBER_OF_BUFFERS > 0u)
#define PduR_GetQueueIndexOfDestPdu(destPduId) PDUR_DESTPDU_CFG(destPduId).PduRDestQueueIdRef
#endif
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
#define PduR_GetTpThresholdOfDestPdu(destPduId) PDUR_DESTPDU_CFG(destPduId).PduRTpThreshold
#endif

#define PduR_GetConfiguredLengthOfDestPdu(destPduId)         PDUR_DESTPDU_CFG(destPduId).ConfiguredLength
#define PduR_GetDefaultValuePtrOfDestPdu(destPduId)          PDUR_DESTPDU_CFG(destPduId).PduRDefaultValueRef
#define PduR_GetTransmissionConfirmationOfDestPdu(destPduId) PDUR_DESTPDU_CFG(destPduId).TransmissionConfirmation
#define PduR_GetRouteTypeOfDestPdu(destPduId)                PDUR_DESTPDU_CFG(destPduId).RouteType
#if (STD_ON == PDUR_MULITIPARTITION_SUPPORT)
#define PduR_GetPartitionIndexOfDestPdu(destPduId) PDUR_DESTPDU_CFG(destPduId).PartitionIndex
#define PduR_GetPartitionIndexOfSrcPdu(srcPduId)   PDUR_SRCEPDU_CFG(srcPduId).PartitionIndex
#endif

#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
#define PduR_GetGlobalBufferPoolPtr() PduR_ConfigStd->PduR_GlobalBufferPoolPtr
#endif

/**
 * @brief invalid values
 */
#define PDUR_NOUSED(paramter) (void)(paramter)

#if !defined(PDUR_LOCAL)
#define PDUR_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

/**
 * @brief PduR kind of buffer(Global and Dedicated)
 */
#define PDUR_KIND_OF_BUFFER 2u
/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline void PduR_Det_ReportRuntimeError(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, (ApiId), (ErrorId));
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 5016 ++ */ /* VL_PduR_5016 */
static inline PduR_BufferType* PduR_QueueGetHead(const PduR_QueueRuntimeType* queuePtr)
{
    return queuePtr->BufferPtrs[0];
}
/* PRQA S 5016 -- */ /* VL_PduR_5016 */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline void PduR_QueueSetForward(PduR_QueueRuntimeType* queuePtr, PduLengthType length)
{
    queuePtr->CurPosition += length;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline void PduR_QueueSetBack(PduR_QueueRuntimeType* queuePtr, PduLengthType length)
{
    queuePtr->CurPosition -= length;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline void PduR_BufferStoreSduLength(PduR_BufferType* bufferPtr, PduLengthType sduLength)
{
    bufferPtr->SduLength = sduLength;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/* Shall be invoke at the start of reception or transmition and and throughout only once in reception or transmission.
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline void PduR_BufferStoreMetaData(PduR_BufferType* bufferPtr, const uint8* metaDataPtr, uint8 metaDataLength)
{
    (void)IStdLib_MemCpy(bufferPtr->Data, metaDataPtr, metaDataLength);
    bufferPtr->PduCurLength = metaDataLength;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline void PduR_BufferStoreSduData(PduR_BufferType* bufferPtr, const uint8* sduDataPtr, PduLengthType sduLength)
{
    (void)IStdLib_MemCpy(&bufferPtr->Data[bufferPtr->PduCurLength], sduDataPtr, sduLength);
    bufferPtr->PduCurLength += sduLength;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline void PduR_Buffer2PduInfo(
    const PduR_BufferType* bufferPtr,
    PduInfoType*           pduInfoPtr,
    uint8                  metaDataLength,
    PduLengthType          curPosition)
{
    pduInfoPtr->MetaDataPtr = (metaDataLength == 0u) ? (uint8*)(NULL_PTR) : (uint8*)(bufferPtr->Data);
    pduInfoPtr->SduDataPtr  = &bufferPtr->Data[metaDataLength + curPosition];
    pduInfoPtr->SduLength   = bufferPtr->PduCurLength - metaDataLength - curPosition;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline PduLengthType PduR_BufferGetRemainingSize(const PduR_BufferType* bufferPtr)
{
    return bufferPtr->PduMaxLength - bufferPtr->PduCurLength;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
#define PDUR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"
extern PduR_QueueRuntimeType PduR_RuntimeQueues[PDUR_NUMBER_OF_QUEUES_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 5016 ++ */ /* VL_PduR_5016 */
static inline PduR_QueueRuntimeType* PduR_GetQueuePtrOfDestPdu(PduIdType destPduId)
{
    return (
        (PduR_GetQueueIndexOfDestPdu(destPduId) == PDUR_UNUSED_UINT16)
            ? (PduR_QueueRuntimeType*)NULL_PTR
            : &PduR_RuntimeQueues[PduR_GetQueueIndexOfDestPdu(destPduId)]);
}
/* PRQA S 5016 -- */ /* VL_PduR_5016 */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
/* =========================================== external data declarations =========================================== */
#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
extern const PduR_BswModuleType PduR_BswModuleConfigData[PDUR_BSW_MODULE_SUM];
#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"

#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
#define PDUR_START_SEC_VAR_INIT_UNSPECIFIED
#include "PduR_MemMap.h"
extern PduR_BufferType PduR_Buffers[PDUR_NUMBER_OF_BUFFERS];
#define PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "PduR_MemMap.h"
#endif

#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
#define PDUR_START_SEC_VAR_CLEARED_16
#include "PduR_MemMap.h"
extern PduR_RoutingPathGroupIdType PduR_RoutingPathEnabled[PDUR_ROUTINGPATH_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_16
#include "PduR_MemMap.h"
#define PDUR_START_SEC_VAR_CLEARED_BOOLEAN
#include "PduR_MemMap.h"
extern boolean PduR_RoutingPathGroupEnabled[PDUR_ROUTING_PATH_GROUP_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "PduR_MemMap.h"
#endif

#if (PDUR_ROUTINGPATH_MAX > 0u)
#define PDUR_START_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
extern PduR_RouteStatusType PduR_DestinationRouteStatus[PDUR_ROUTINGPATH_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
#define PDUR_START_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
extern PduR_RouteStatusType PduR_SourceRouteStatus[PDUR_ROUTINGPATH_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
#if STD_ON == PDUR_MULTICAST_TP_TRANSMIT_SUPPORT
#define PDUR_START_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
extern PduLengthType PduR_SourceMultiCastTpTransmittedCnt[PDUR_ROUTINGPATH_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
#define PDUR_START_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
extern PduLengthType PduR_DestinationMultiCastTpTransmittedCnt[PDUR_ROUTINGPATH_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
#endif
#endif
/* ========================================= external function declarations ========================================= */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline uint8 PduR_GetMetaDataLengthOfSrcPdu(PduIdType srcPduId)
{
#if (STD_ON == PDUR_META_DATA_SUPPORT)
    return PDUR_SRCEPDU_CFG(srcPduId).MetaDataLength;
#else
    PDUR_NOUSED(srcPduId);
    return 0u;
#endif
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline uint8 PduR_GetMetaDataLengthOfDestPdu(PduIdType destPduId)
{
#if (STD_ON == PDUR_META_DATA_SUPPORT)
    return PDUR_DESTPDU_CFG(destPduId).MetaDataLength;
#else
    PDUR_NOUSED(destPduId);
    return 0u;
#endif
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline boolean PduR_IsRoutingPathEnable(PduIdType destPduId)
{
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
    return (PduR_RoutingPathEnabled[destPduId] > 0u) ? TRUE : FALSE;
#else
    PDUR_NOUSED(destPduId);
    return TRUE;
#endif
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline boolean PduR_IsTpRouteOfSrcPdu(PduIdType srcPduId)
{
    return (PDUR_ROUTINGPATH_CFG(srcPduId).TpRoute);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline boolean PduR_IsTpRouteOfDestPdu(PduIdType destPduId)
{
    /* Multi-cast has the save routing type */
    return (PDUR_ROUTINGPATH_CFG(PduR_GetSrcPduOfDestPdu(destPduId, 0u)).TpRoute);
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

#if (PDUR_MULITIVARIANT_SUPPORT == STD_ON)
/**
 * get internal pdu id of the PduR module
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline PduIdType PduR_GetPduIdFromUserId(uint8 apiId, PduIdType id, boolean isSrc)
{
    PduIdType pduId = PDUR_PDU_ID_INVALID;
#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ROUTINGPATH_MAX_ALL_V <= id)
    {
        Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, apiId, PDUR_E_PDU_ID_INVALID);
    }
    else
#endif
    {
        const PduIdType* mapPtr;
        if (isSrc)
        {
            mapPtr = PduR_ConfigStd->UserIdMapPduRSrcPduIdPtr;
        }
        else
        {
            mapPtr = PduR_ConfigStd->UserIdMapPduRDestPduIdPtr;
        }
        pduId = mapPtr[id];
    }
    return pduId;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
/**
 * get internal pdu id of the PduR module
 */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
static inline PduR_RoutingPathGroupIdType PduR_GetGroupIdFromUserId(uint8 apiId, PduR_RoutingPathGroupIdType id)
{
    PduR_RoutingPathGroupIdType groupId = PDUR_UNUSED_UINT16;

#if (STD_ON == PDUR_DEV_ERROR_DETECT)
    if (PDUR_ROUTING_PATH_GROUP_MAX_ALL_V <= id)
    {
        Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, apiId, PDUR_E_ROUTING_PATH_GROUP_ID_INVALID);
    }
    else
#endif
    {
        groupId = PduR_ConfigStd->UserIdMapPduRPathGroupPtr[id];
    }
    return groupId;
}
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif
#endif

#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
extern void PduR_RoutingPathGroupInit(void);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
extern uint8 PduR_GetPartitionFuncIndex(
    const PduR_BswModuleType* modulePtr,
    PduIdType                 srcPduId,
    PduIdType                 destPduId,
    boolean                   isFromDest);
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
/* PRQA S 5016 ++ */ /* VL_PduR_5016 */
extern PduInfoType* PduR_PduDeepCopy(PduIdType id, const PduInfoType* srcPduInfoPtr, PduInfoType* tagetPduInfoPtr);
/* PRQA S 5016 -- */ /* VL_PduR_5016 */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif

#endif /* end if PDUR_ZERO_COST_OPERATION */

#ifdef __cplusplus
}
#endif
#endif
