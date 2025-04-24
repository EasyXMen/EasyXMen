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
**  FILENAME    : PduR_Internal.c                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : PDUR internal API definitions                               **
**                                                                            **
**  SPECIFICATION(S) : AUTOSAR Classic Platform 4.2.2 and R19_11              **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "PduR_Internal.h"
#if (STD_OFF == PDUR_ZERO_COST_OPERATION)
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
/*all route dest pdus is enable or disable*/
#if (PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON)
#define PDUR_START_SEC_VAR_CLEARED_BOOLEAN
#include "PduR_MemMap.h"
boolean PduR_RoutingPathEnabled[PDUR_ROUTINGPATH_MAX];
#define PDUR_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "PduR_MemMap.h"
#endif

#if ((PDUR_DEST_PDU_SUM > 0u) && (PDUR_SRC_PDU_SUM > 0u))
#define PDUR_START_SEC_VAR_CLEARED_8
#include "PduR_MemMap.h"
PduR_RouteStatusType PduR_DestinationRouteStatus[PDUR_DEST_PDU_SUM];
PduR_RouteStatusType PduR_SourceRouteStatus[PDUR_SRC_PDU_SUM];
#define PDUR_STOP_SEC_VAR_CLEARED_8
#include "PduR_MemMap.h"
#endif
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"
#if (PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON)
void PduR_RoutingPathGroupInit(void)
{
    uint16 index;
    /* init all dest pdu state base on RoutingPathGroup parameter PduRIsEnabledAtInit.
     * default state is enabled for dest pdus not included any path group */
    for (index = 0u; index < PDUR_ROUTINGPATH_MAX; index++)
    {
        PduR_RoutingPathEnabled[index] = TRUE;
    }
    for (index = 0u; index < PduR_GetRoutingPathGroupCntOfPBConfig(); index++)
    {
        if (FALSE == PDUR_ROUTEPATHGROUP_CFG(index).PduRIsEnabledAtInit)
        {
            PduIdType destPduNumber = PDUR_ROUTEPATHGROUP_CFG(index).PduRDestPduRefNumber;
            for (PduIdType cnt = 0u; cnt < destPduNumber; cnt++)
            {
                PduIdType destIndex = PDUR_ROUTEPATHGROUP_CFG(index).PduRDestPduIdRef[cnt];
                PduR_RoutingPathEnabled[destIndex] = FALSE;
            }
        }
    }
}
#endif /* PDUR_ROUTING_PATH_GROUP_ENABLED == STD_ON */

#if (PDUR_MULITIPARTITION_SUPPORT == STD_ON)
uint8 PduR_GetPartitionFuncIndexFromDestPdu(const PduRBswModuleType* modulePtr, PduIdType destPduId)
{
    uint8 num;
    uint8 partitionNum = modulePtr->PduRBswModulePartitionNum;
    const uint8* partitionPtr = modulePtr->PduRBswModulePartitionRef;
    uint8 destPduPartitionIndex = PduR_GetPartitionIndexOfDestPdu(destPduId);
    for (num = 0u; num < partitionNum; num++)
    {
        if (destPduPartitionIndex == partitionPtr[num])
        {
            break;
        }
    }
    return num;
}
uint8 PduR_GetPartitionFuncIndexFromSrcPdu(const PduRBswModuleType* modulePtr, PduIdType srcPduId)
{
    uint8 num;
    uint8 partitionNum = modulePtr->PduRBswModulePartitionNum;
    const uint8* partitionPtr = modulePtr->PduRBswModulePartitionRef;
    uint8 srcPduPartitionIndex = PduR_GetPartitionIndexOfSrcPdu(srcPduId);
    for (num = 0u; num < partitionNum; num++)
    {
        if (srcPduPartitionIndex == partitionPtr[num])
        {
            break;
        }
    }
    return num;
}
#endif /* PDUR_MULITIPARTITION_SUPPORT == STD_ON */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"
#endif /*STD_OFF == PDUR_ZERO_COST_OPERATION*/
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
