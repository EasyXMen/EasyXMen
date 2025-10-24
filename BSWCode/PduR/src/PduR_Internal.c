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
 **  @file               : PduR_Internal.c
 **  @author             : zhengfei.li
 **  @date               : 2020/06/22
 **  @vendor             : isoft
 **  @description        : PDUR internal API definitions
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "PduR_Internal.h"

#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definition ============================================ */

/* ============================================ external data definition ============================================ */
/**
 * @brief All route dest pdus is enable or disable
 */
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
#define PDUR_START_SEC_VAR_CLEARED_16
#include "PduR_MemMap.h"
PduR_RoutingPathGroupIdType PduR_RoutingPathEnabled[PDUR_ROUTINGPATH_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_16
#include "PduR_MemMap.h"
/**
 * @brief All route path group is enable or disable
 */
#define PDUR_START_SEC_VAR_CLEARED_BOOLEAN
#include "PduR_MemMap.h"
boolean PduR_RoutingPathGroupEnabled[PDUR_ROUTING_PATH_GROUP_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "PduR_MemMap.h"
#endif
/**
 * @brief PduR SrcPdu and DestPdu Run Time staus
 */
#if (PDUR_ROUTINGPATH_MAX > 0u)
#define PDUR_START_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
/* PRQA S 1532,1531 ++ */ /* VL_QAC_OneFunRef,VL_QAC_OneRefSymbol */
PduR_RouteStatusType PduR_DestinationRouteStatus[PDUR_ROUTINGPATH_MAX];
PduR_RouteStatusType PduR_SourceRouteStatus[PDUR_ROUTINGPATH_MAX];
/* PRQA S 1532,1531 -- */
#define PDUR_STOP_SEC_VAR_CLEARED_32
#include "PduR_MemMap.h"
#endif
/**
 * @brief PduR PB Queue Run Time point parameter
 */
#if (PDUR_NUMBER_OF_QUEUES_MAX > 0u)
#define PDUR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"
PduR_QueueRuntimeType PduR_RuntimeQueues[PDUR_NUMBER_OF_QUEUES_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"
#endif
/* ========================================= internal function declarations ========================================= */

/* ========================================== external function definition ========================================== */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
#if (PDUR_ROUTING_PATH_GROUP_MAX > 0u)
/**
 * Init all route path and path group status
 */
void PduR_RoutingPathGroupInit(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint16 index;
    /* init all dest pdu state base on RoutingPathGroup parameter PduRIsEnabledAtInit.
     * default state is enabled for dest pdus not included any path group */
    for (index = 0u; index < PduR_GetPduRRoutingPathNumOfPBConfig; index++)
    {
        PduR_RoutingPathEnabled[index] = PDUR_UNUSED_UINT16;
    }
    for (index = 0u; index < PduR_GetRoutingPathGroupCntOfPBConfig; index++)
    {
        if (PDUR_ROUTEPATHGROUP_CFG(index).PduRIsEnabledAtInit)
        {
            PduIdType destPduNumber = PDUR_ROUTEPATHGROUP_CFG(index).PduRDestPduRefNumber;
            for (PduIdType cnt = 0u; cnt < destPduNumber; cnt++)
            {
                PduIdType destIndex = PDUR_ROUTEPATHGROUP_CFG(index).PduRDestPduIdRef[cnt];
                if (PduR_RoutingPathEnabled[destIndex] == PDUR_UNUSED_UINT16)
                {
                    PduR_RoutingPathEnabled[destIndex] = 1u;
                }
                else
                {
                    PduR_RoutingPathEnabled[destIndex]++;
                }
            }
            PduR_RoutingPathGroupEnabled[index] = TRUE;
        }
        else
        {
            PduIdType destPduNumber = PDUR_ROUTEPATHGROUP_CFG(index).PduRDestPduRefNumber;
            for (PduIdType cnt = 0u; cnt < destPduNumber; cnt++)
            {
                PduIdType destIndex = PDUR_ROUTEPATHGROUP_CFG(index).PduRDestPduIdRef[cnt];
                if (PduR_RoutingPathEnabled[destIndex] == PDUR_UNUSED_UINT16)
                {
                    PduR_RoutingPathEnabled[destIndex] = 0u;
                }
            }
            PduR_RoutingPathGroupEnabled[index] = FALSE;
        }
    }
}
#endif

#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
/**
 * Get the object function index in muliti-patition
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
uint8 PduR_GetPartitionFuncIndex(
    const PduR_BswModuleType* modulePtr,
    PduIdType                 srcPduId,
    PduIdType                 destPduId,
    boolean                   isFromDest)
/* PRQA S 1532 -- */
{
    uint8 funcIndex;
    uint8 srcPduPartitionIndex  = PduR_GetPartitionIndexOfSrcPdu(srcPduId);
    uint8 destPduPartitionIndex = PduR_GetPartitionIndexOfDestPdu(destPduId);
    uint8 targetPartitionId     = isFromDest ? destPduPartitionIndex : srcPduPartitionIndex;
    if (modulePtr->BswModulePartitionNum == PDUR_UNUSED_UINT8)
    {
        /* Object module does not involve partitioning, direct call. */
        funcIndex = 0u;
    }
    else if (srcPduPartitionIndex == destPduPartitionIndex)
    {
        /* This path does not involve cross partitioning and can be called directly. */
        funcIndex = modulePtr->BswModulePartitionNum;
    }
    else
    {
        uint8        partitionNum = modulePtr->BswModulePartitionNum;
        const uint8* partitionPtr = modulePtr->BswModulePartitionRef;
        for (funcIndex = 0u; funcIndex < partitionNum; funcIndex++)
        {
            if (targetPartitionId == partitionPtr[funcIndex])
            {
                break;
            }
        }
    }
    return funcIndex;
}
/**
 * To save the pdu info when the route is Inter-Partition and no queue set
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
PduInfoType* PduR_PduDeepCopy(PduIdType id, const PduInfoType* srcPduInfoPtr, PduInfoType* tagetPduInfoPtr)
/* PRQA S 1532 -- */
{
    PduInfoType* infoPtr = NULL_PTR;
    if (srcPduInfoPtr != NULL_PTR)
    {
        /* The route is Inter-Partition and no queue set */
        uint8* interPartitionDataPtr = PduR_GetBufDataInterPartitionOfRoutingPath(id);
        /* copy data and metadata into Global buffer */
        if (NULL_PTR != srcPduInfoPtr->SduDataPtr)
        {
            (void)IStdLib_MemCpy(interPartitionDataPtr, srcPduInfoPtr->SduDataPtr, srcPduInfoPtr->SduLength);
            tagetPduInfoPtr->SduDataPtr = interPartitionDataPtr;
        }
        tagetPduInfoPtr->SduLength = srcPduInfoPtr->SduLength;
        if (NULL_PTR != srcPduInfoPtr->MetaDataPtr)
        {
            (void)IStdLib_MemCpy(
                &interPartitionDataPtr[tagetPduInfoPtr->SduLength],
                srcPduInfoPtr->MetaDataPtr,
                PduR_GetMetaDataLengthOfSrcPdu(id));
            tagetPduInfoPtr->MetaDataPtr = &interPartitionDataPtr[tagetPduInfoPtr->SduLength];
        }
        infoPtr = tagetPduInfoPtr;
    }
    return infoPtr;
}
#endif
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
/* ========================================== internal function definition ========================================== */

#endif
