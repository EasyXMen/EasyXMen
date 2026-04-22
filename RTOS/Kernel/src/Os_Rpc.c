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
 *
 ************************************************************************************************************************
 **
 **  @file               : Os_Rpc.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Rpc API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Rpc.h"
#include "Os_Spinlock.h"
#include "Os_Core.h"
#include "Os_Kernel.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */
#if (OS_AUTOSAR_CORES > 1)
/* Multi-core Shared variables */
/* PRQA S 0791 ++ */ /* VL_QAC_identifier */
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
OS_LOCAL Os_RpcCoreType Os_InterCore[OS_AUTOSAR_CORES];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
OS_LOCAL Os_SpinlockType Os_SpinlockRpc[OS_AUTOSAR_CORES];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
/* PRQA S 0791 -- */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief           Retrieves a service node from the specified queue
 * @param[in]       rpcQue: Pointer to the queue to get a service node from
 * @param[in]       serviceStation: Pointer to the service station containing the nodes
 * @return          Os_RpcNodeIdType
 * @retval          Node ID of retrieved service, or OS_RPC_INVALID_TRAIN if no service is available
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL Os_RpcNodeIdType Os_RpcGetService(LinkQueRefType rpcQue, Os_RpcServiceNodeRefType serviceStation, Os_SpinlockRefType pSpinlock);

/**
 * @brief           Adds a service node to the tail of a queue
 * @param[in]       rpcQue: Pointer to the queue to add the service node to
 * @param[in]       serviceStation: Pointer to the service station containing the nodes
 * @param[in]       nodeId: ID of the node to add
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_RpcAddService(LinkQueRefType rpcQue, Os_RpcServiceNodeRefType serviceStation, Os_RpcNodeIdType nodeId, Os_SpinlockRefType pSpinlock);

/**
 * @brief           Sets up service node parameters based on input data
 * @param[in]       rpcData: Pointer to the RPC input data
 * @param[out]      srvNode: Pointer to the service node to populate
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_RpcProcessParameter(Os_RpcInputRefType rpcData, Os_RpcServiceNodeRefType srvNode);

/**
 * @brief           Waits for a service node to complete processing or timeout
 * @param[in]       coreId: ID of the core executing the service
 * @param[in]       srvNode: Pointer to the service node being processed
 * @return          StatusType
 * @retval          E_OK: Service completed successfully
 * @retval          E_OS_CORE: Target core is not running
 * @retval          E_OS_TIMEOUT: Service request timed out
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcWaitResult(Os_CoreIdType coreId, Os_RpcServiceNodeRefType srvNode);

/**
 * @brief           Copies results from a service node back to RPC data structure
 * @param[out]      rpcData: Pointer to the RPC data to update with results
 * @param[in]       srvNode: Pointer to the service node containing results
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_RpcProcessResult(Os_RpcInputRefType rpcData, Os_RpcServiceNodeRefType srvNode);

/**
 * @brief           Executes the action function associated with a service node
 * @param[inout]    srvNode: Pointer to the service node to process
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_RpcServiceAction(Os_RpcServiceNodeRefType srvNode);

/* ========================================== external function definitions ========================================= */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the RPC module
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitRpc(void)
/* PRQA S 1532 -- */
{
    Os_SCBType *pScb = Os_GetCurrentContext();  /* PRQA S 3678 */ /* VL_Os_3678 */

    /*only logic master core need to RUN this code.*/
    if (OS_CORE_ID_MASTER == pScb->SysCore)
    {
        for (Os_CoreIdType i = (Os_CoreIdType)0; i < OS_AUTOSAR_CORES; i++)
        {
            Os_InterCore[i].FreeQue.Head = OS_RPC_INVALID_TRAIN;
            Os_InterCore[i].FreeQue.Tail = OS_RPC_INVALID_TRAIN;
            Os_InterCore[i].WorkQue.Head = OS_RPC_INVALID_TRAIN;
            Os_InterCore[i].WorkQue.Tail = OS_RPC_INVALID_TRAIN;
            for (uint8 y = 0U; y < OS_RPC_MAX_NODE; y++)
            {
                Os_InterCore[i].ServiceStation[y].NextNode = OS_RPC_INVALID_TRAIN;
                Os_RpcAddService(&Os_InterCore[i].FreeQue,
                                 Os_InterCore[i].ServiceStation,
                                 y,
                                 &Os_SpinlockRpc[i]);
            }
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the number of the service node and remove
                         this node from Head of queue
 */
OS_LOCAL Os_RpcNodeIdType Os_RpcGetService(LinkQueRefType rpcQue,
                                  Os_RpcServiceNodeRefType serviceStation,
                                  Os_SpinlockRefType pSpinlock)
{
    Os_RpcNodeIdType retNode = OS_RPC_INVALID_TRAIN;
    OS_HAL_DECLARE_CRITICAL();

    if ((rpcQue != NULL_PTR) && (serviceStation != NULL_PTR))
    {
        OS_HAL_ENTRY_CRITICAL();
        Os_GetInternalSpinlock(pSpinlock);
        if (OS_RPC_INVALID_TRAIN == rpcQue->Head)
        {
            retNode = OS_RPC_INVALID_TRAIN;
        }
        else
        {
            retNode = rpcQue->Head;
            rpcQue->Head = serviceStation[retNode].NextNode;
            if (OS_RPC_INVALID_TRAIN == rpcQue->Head)
            {
                rpcQue->Tail = OS_RPC_INVALID_TRAIN;
            }
            else
            {
                serviceStation[retNode].NextNode = OS_RPC_INVALID_TRAIN;
            }
        }
        Os_ReleaseInternalSpinlock(pSpinlock);
        OS_HAL_EXIT_CRITICAL();
    }

    return retNode;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Add the service node nodeId to the queue rpcQue Tail
 *                       of service station serviceStation
 */
OS_LOCAL void Os_RpcAddService(LinkQueRefType rpcQue,
                               Os_RpcServiceNodeRefType serviceStation,
                               Os_RpcNodeIdType nodeId,
                               Os_SpinlockRefType pSpinlock)
{
    OS_HAL_DECLARE_CRITICAL();

    if ((nodeId < OS_RPC_MAX_NODE) && (rpcQue != NULL_PTR) &&
        (serviceStation != NULL_PTR))
    {
        OS_HAL_ENTRY_CRITICAL();
        Os_GetInternalSpinlock(pSpinlock);
        if (OS_RPC_INVALID_TRAIN == rpcQue->Tail)
        {
            rpcQue->Head = nodeId;
            rpcQue->Tail = nodeId;
            serviceStation[nodeId].NextNode = OS_RPC_INVALID_TRAIN;
        }
        else
        {
            uint8 Tail = rpcQue->Tail;
            serviceStation[Tail].NextNode = nodeId;
            serviceStation[nodeId].NextNode = OS_RPC_INVALID_TRAIN;
            rpcQue->Tail = nodeId;
        }
        Os_ReleaseInternalSpinlock(pSpinlock);
        OS_HAL_EXIT_CRITICAL();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Set the paramters in rpcData to the service node srvNode
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_RpcProcessParameter(Os_RpcInputRefType rpcData,
                                     Os_RpcServiceNodeRefType srvNode)
/* PRQA S 3673 -- */
{
    /* get destination core execution state */
    srvNode->ProcState = OS_RPC_START;
    srvNode->RetValue = E_NOT_OK; /* get return value */
    srvNode->ActionFn = rpcData->ActionFn;
    srvNode->RpcSync = rpcData->RpcSync;
    srvNode->SourceCoreId = Os_GetCoreIdLocal();

    for (uint8 i = 0; i < OS_RPC_IN_PARA_MAX; i++) /* PRQA S 1290 */ /* VL_Os_1290 */
    {
        srvNode->InPara[i] = rpcData->InPara[i];
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Wait the result until timeout
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL StatusType Os_RpcWaitResult(Os_CoreIdType coreId,
                                     Os_RpcServiceNodeRefType srvNode)
/* PRQA S 3673 -- */
{
    /* CORE1: FREE -> START  ===> CORE2: WORK -> DONE  ===> CORE1: FREE */
    StatusType vRet   = E_OK;

    while (OS_RPC_DONE != srvNode->ProcState) /* PRQA S 3442, 0771 */ /* VL_Os_3442, VL_Os_0771 */
    {
        if (Os_CoreCB.CoreStatus[coreId] != OS_RUN) /* PRQA S 3442 */ /* VL_Os_3442 */
        {
            vRet = E_OS_CORE;
            break;
        }
    }

    return vRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Set the service node srvNode to the paramters
 *                       in rpcData according to the service ID
 */
OS_LOCAL void Os_RpcProcessResult(Os_RpcInputRefType rpcData,
                                  Os_RpcServiceNodeRefType srvNode)
{
    for (uint8 i = 0; i < OS_RPC_IN_PARA_MAX; i++) /* PRQA S 1290 */ /* VL_Os_1290 */
    {
        rpcData->InPara[i] = srvNode->InPara[i];
    }
    srvNode->ProcState = OS_RPC_FREE;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Make a remote service call to given core with
 *                       given service ID and input parameters
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Os_STMIF */
StatusType Os_RpcCallService(Os_RpcInputRefType rpcData)
/* PRQA S 6030 -- */
{
    StatusType vRet = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == rpcData)
    {
        vRet = E_OS_ILLEGAL_ADDRESS;
    }
    else if (rpcData->RemoteCoreId >= OS_AUTOSAR_CORES)
    {
        vRet = E_OS_CORE;
    }
    else
#endif
    {
        uint16 coreId = rpcData->RemoteCoreId;
        Os_RpcNodeIdType nodeId = OS_RPC_INVALID_TRAIN;
        Os_RpcServiceNodeRefType srvNode;

        do
        {
            if (Os_CoreCB.CoreStatus[coreId] != OS_RUN) /* PRQA S 3442 */ /* VL_Os_3442 */
            {
                vRet = E_OS_CORE;
                break;
            }
            nodeId = Os_RpcGetService(&Os_InterCore[coreId].FreeQue,
                                      Os_InterCore[coreId].ServiceStation,
                                      &Os_SpinlockRpc[coreId]); 
        }while (OS_RPC_INVALID_TRAIN == nodeId);
        
        if (vRet == E_OK)
        {
            srvNode = &Os_InterCore[coreId].ServiceStation[nodeId];
            Os_RpcProcessParameter(rpcData, srvNode);
            Os_RpcAddService(&Os_InterCore[coreId].WorkQue,
                             Os_InterCore[coreId].ServiceStation,
                             nodeId,
                             &Os_SpinlockRpc[coreId]);
            /* PRQA S 0303, 1258, 3455, 3138 ++ */ /* VL_Os_0303, VL_Os_1258, VL_Os_3455, VL_Os_3138 */
            Os_Hal_RemoteCall(rpcData->RemoteCoreId);
            /* PRQA S 0303, 1258, 3455, 3138 -- */
            if (rpcData->RpcSync == OS_RPC_SYNC)
            {
                StatusType status = E_OK;
                status = Os_RpcWaitResult(coreId, srvNode);
                if (OS_RPC_DONE == srvNode->ProcState) /* PRQA S 3442 */ /* VL_Os_3442 */
                {
                    Os_RpcProcessResult(rpcData, srvNode);
                    vRet = srvNode->RetValue;
                }
                else
                {
                    vRet = status;
                }
                Os_RpcAddService(&Os_InterCore[coreId].FreeQue,
                                 Os_InterCore[coreId].ServiceStation,
                                 nodeId,
                                 &Os_SpinlockRpc[coreId]);
            }
        }
    }
    return vRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Call the specified function according to service ID
 */
OS_LOCAL void Os_RpcServiceAction(Os_RpcServiceNodeRefType srvNode)
{
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (srvNode->ActionFn == NULL_PTR)
    {
        Os_Panic();
    }
#endif

    /* PRQA S 2812 ++ */ /* VL_Os_2812 */
    srvNode->RetValue = srvNode->ActionFn(srvNode->InPara);
    /* PRQA S 2812 -- */
    srvNode->ProcState = OS_RPC_DONE;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Process all received RPC service request on the
                         core given coreId
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_RpcServiceOperation(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
    Os_RpcServiceNodeRefType srvNode;
    Os_RpcNodeIdType nodeId;

    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        nodeId = Os_RpcGetService(&Os_InterCore[coreId].WorkQue,
                                  Os_InterCore[coreId].ServiceStation,
                                  &Os_SpinlockRpc[coreId]);
        if (nodeId == OS_RPC_INVALID_TRAIN)
        {
            break;
        }
        srvNode = &Os_InterCore[coreId].ServiceStation[nodeId];
        srvNode->ProcState = OS_RPC_WORK;
        Os_RpcServiceAction(srvNode);
        if (srvNode->RpcSync != OS_RPC_SYNC)
        {
            srvNode->ProcState = OS_RPC_FREE;
            Os_RpcAddService(&Os_InterCore[coreId].FreeQue,
                             Os_InterCore[coreId].ServiceStation,
                             nodeId,
                             &Os_SpinlockRpc[coreId]);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
