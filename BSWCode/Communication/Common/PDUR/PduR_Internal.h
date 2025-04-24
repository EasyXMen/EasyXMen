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
**  FILENAME    : PduR_Internal.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : PDUR internal header for internal API declarations          **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19_11              **
**                                                                            **
*******************************************************************************/
#ifndef PDUR_INTERNAL_H
#define PDUR_INTERNAL_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR.h"
#if (PDUR_NUMBER_OF_QUEUES > 0u)
#include "PduR_Buffer.h"
#endif
#include "PduR_Route.h"
#include "Det.h"
#include "istd_lib.h"
#include "SchM_PduR.h"

/*******************************************************************************
**                      Macros                                                **
*******************************************************************************/
#if ((PDUR_NUMBER_OF_QUEUES > 0u) && (PDUR_DEFAULT_VALUE_LENGTH > 0u) && (PDUR_DEFAULT_VALUE_PDU > 0u))
#define PDUR_DEFAULT_VALUE_ENABLED STD_ON
#else
#define PDUR_DEFAULT_VALUE_ENABLED STD_OFF
#endif

#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
#define PDUR_ROUTING_PATH_GROUP_ENABLED STD_ON
#else
#define PDUR_ROUTING_PATH_GROUP_ENABLED STD_OFF
#endif /* PDUR_ROUTING_PATH_GROUP_MAX > 0u */

/* Using for Conditional compilation PB-Config */
/* PDUR_COMMUNICATION_INTERFACE_FORWARDING_ENABLED
   PDUR_COMMUNICATION_INTERFACE_GATEWAYING_ENABLED
   PDUR_TRANSPORT_PROTOCOL_FORWARDING_NOBUFFERED_ENABLED
   PDUR_TRANSPORT_PROTOCOL_FORWARDING_BUFFERED_ENABLED
   PDUR_TRANSPORT_PROTOCOL_GATEWAYING_ENABLED */
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* Published information */
#if !defined(PDUR_PUBLISHED_INFORMATION)
#define PDUR_PUBLISHED_INFORMATION
#define PDUR_MODULE_ID                   51u
#define PDUR_VENDOR_ID                   62u
#define PDUR_AR_RELEASE_MAJOR_VERSION    4u
#define PDUR_AR_RELEASE_MINOR_VERSION    2u
#define PDUR_AR_RELEASE_REVISION_VERSION 2u
#define PDUR_SW_MAJOR_VERSION            2u
#define PDUR_SW_MINOR_VERSION            3u
#define PDUR_SW_PATCH_VERSION            5u
#elif ((PDUR_SW_MAJOR_VERSION != 2u) || (PDUR_SW_MINOR_VERSION != 3u) || (PDUR_SW_PATCH_VERSION != 5u))
#error "PduR: Mismatch in Software Version"
#endif

#if !defined(PDUR_LOCAL)
#define PDUR_LOCAL static
#endif

#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/* Common Library */
#define PduR_Min(a, b)                       (((a) < (b)) ? (a) : (b))
#define PduR_Det_ReportError(apiId, errorId) (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, (apiId), (errorId))
#define PduR_Det_ReportRuntimeError(apiId, errorId) \
    (void)Det_ReportRuntimeError(PDUR_MODULE_ID, PDUR_INSTANCE_ID, (apiId), (errorId))

/* Get Interface of PBConfig */
#define PduR_GetRoutingPathGroupCntOfPBConfig()       PduR_ConfigStd->routingPathGroupCnt

#define PduR_GetDestPduNumOfSrcPdu(srcPduId)          PDUR_ROUTINGPATH_CFG(srcPduId).PduDestSum
#define PduR_GetDestPduOfRoutingPath(srcPduId, index) PDUR_ROUTINGPATH_CFG(srcPduId).PduRDestPduIdRef[index]

#define PduR_GetSrcPduUpTxConfOfSrcPdu(srcPduId)      PDUR_SRCEPDU_CFG(srcPduId).PduRSrcPduUpTxConf
#define PduR_GetBswModulePtrOfSrcPdu(pduId)           &PduR_BswModuleConfigData[PDUR_SRCEPDU_CFG(pduId).BswModuleIndex]
#define PduR_GetBswModulePduIdOfSrcPdu(pduId)         PDUR_SRCEPDU_CFG(pduId).PduRSrcModulePduIndex

#define PduR_GetSrcPduNumOfDestPdu(destPduId)         PDUR_DESTPDU_CFG(destPduId).PduSrcSum
#define PduR_GetSrcPduOfDestPdu(destPduId, index)     PDUR_DESTPDU_CFG(destPduId).PduRSrcPduRef[index]
#define PduR_GetDataProvisionOfDestPdu(destPduId)     PDUR_DESTPDU_CFG(destPduId).PduRDestPduDataProvision
#define PduR_GetBswModulePduIdOfDestPdu(destPduId)    PDUR_DESTPDU_CFG(destPduId).PduRDestModulePduIndex
#define PduR_GetBswModulePtrOfDestPdu(destPduId)      &PduR_BswModuleConfigData[PDUR_DESTPDU_CFG(destPduId).BswModuleIndex]
#if (PDUR_NUMBER_OF_QUEUES > 0u)
#define PduR_GetQueueIndexOfDestPdu(destPduId)  PDUR_DESTPDU_CFG(destPduId).PduRDestQueueIdRef
#define PduR_GetTpThresholdOfDestPdu(destPduId) PDUR_DESTPDU_CFG(destPduId).PduRTpThreshold
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */

#if (PDUR_NUMBER_OF_QUEUES > 0u)
#define PduR_GetQueuePtrOfDestPdu(destPduId)                       \
    (PduR_GetQueueIndexOfDestPdu(destPduId) == PDUR_UNUSED_UINT16) \
        ? NULL_PTR                                                 \
        : &PduR_Queues[PduR_GetQueueIndexOfDestPdu(destPduId)]
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */
#define PduR_GetConfiguredLengthOfDestPdu(destPduId)         PDUR_DESTPDU_CFG(destPduId).configuredLength
#define PduR_GetDefaultValuePtrOfDestPdu(destPduId)          PDUR_DESTPDU_CFG(destPduId).PduRDefaultValueRef
#define PduR_GetTransmissionConfirmationOfDestPdu(destPduId) PDUR_DESTPDU_CFG(destPduId).transmissionConfirmation
#define PduR_GetRouteTypeOfDestPdu(destPduId)                PDUR_DESTPDU_CFG(destPduId).routeType
#if (STD_ON == PDUR_MULITIPARTITION_SUPPORT)
#define PduR_GetPartitionIndexOfDestPdu(destPduId) PDUR_DESTPDU_CFG(destPduId).PartitionIndex
#define PduR_GetPartitionIndexOfSrcPdu(srcPduId)   PDUR_SRCEPDU_CFG(srcPduId).PartitionIndex
#endif /* STD_ON == PDUR_MULITIPARTITION_SUPPORT */

#define PduR_GetGlobalBufferPoolPtr() &PduR_GlobalBufferPool

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#if (PDUR_DEFAULT_VALUE_ENABLED == STD_ON)
extern CONST(uint8, PDUR_CONST) PduR_Default_value[PDUR_DEFAULT_VALUE_LENGTH];
#endif /* PDUR_DEFAULT_VALUE_ENABLED == STD_ON */

extern CONST(PduRBswModuleType, PDUR_CONST) PduR_BswModuleConfigData[PDUR_BSW_MODULE_SUM];

#if (PDUR_NUMBER_OF_QUEUES > 0u)
extern const PduR_BufferPoolType PduR_GlobalBufferPool;
extern PduR_QueueType PduR_Queues[PDUR_NUMBER_OF_QUEUES];
extern PduR_BufferType PduR_Buffers[PDUR_NUMBER_OF_BUFFERS];
#endif /* PDUR_NUMBER_OF_QUEUES > 0u */

/*********************************
** define in PduR_Internal.c   **
**********************************/
/*all route dest pdus is enable or disable*/
#if (PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON)
extern boolean PduR_RoutingPathEnabled[PDUR_ROUTINGPATH_MAX];
#endif /* PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON */

#if ((PDUR_DEST_PDU_SUM > 0u) && (PDUR_SRC_PDU_SUM > 0u))
extern PduR_RouteStatusType PduR_DestinationRouteStatus[PDUR_DEST_PDU_SUM];
extern PduR_RouteStatusType PduR_SourceRouteStatus[PDUR_SRC_PDU_SUM];
#endif
/*******************************************************************************
**                      External function declarations                        **
*******************************************************************************/
static inline PduR_MetaDataLengthType PduR_GetMetaDataLengthOfSrcPdu(PduIdType srcPduId)
{
#if (STD_ON == PDUR_META_DATA_SUPPORT)
    return PDUR_SRCEPDU_CFG(srcPduId).MetaDataLength;
#else
    PDUR_NOUSED(srcPduId);
    return 0u;
#endif /* STD_ON == PDUR_META_DATA_SUPPORT */
}

static inline PduR_MetaDataLengthType PduR_GetMetaDataLengthOfDestPdu(PduIdType destPduId)
{
#if (STD_ON == PDUR_META_DATA_SUPPORT)
    return PDUR_DESTPDU_CFG(destPduId).MetaDataLength;
#else
    PDUR_NOUSED(destPduId);
    return 0u;
#endif /* STD_ON == PDUR_META_DATA_SUPPORT */
}

static inline boolean PduR_IsRoutingPathEnable(PduIdType destPduId)
{
#if (PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON)
    return PduR_RoutingPathEnabled[destPduId];
#else
    PDUR_NOUSED(destPduId);
    return TRUE;
#endif /* PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON */
}

static inline boolean PduR_IsTpRouteOfSrcPdu(PduIdType srcPduId)
{
    return (TRUE == PDUR_ROUTINGPATH_CFG(srcPduId).TpRoute);
}

static inline boolean PduR_IsTpRouteOfDestPdu(PduIdType destPduId)
{
    return (TRUE == PDUR_ROUTINGPATH_CFG(PduR_GetSrcPduOfDestPdu(destPduId, 0u)).TpRoute);
}

#if (PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON)
extern FUNC(void, PDUR_CODE) PduR_RoutingPathGroupInit(void);
#endif /* PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON */

#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
uint8 PduR_GetPartitionFuncIndexFromDestPdu(const PduRBswModuleType* modulePtr, PduIdType destPduId);
uint8 PduR_GetPartitionFuncIndexFromSrcPdu(const PduRBswModuleType* modulePtr, PduIdType srcPduId);

#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */

#endif /*STD_OFF == PDUR_ZERO_COST_OPERATION*/

#endif /* end of PDUR_INTERNAL_H */
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
