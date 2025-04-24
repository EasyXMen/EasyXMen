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
**  FILENAME    : Os_Rpc.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (OS_AUTOSAR_CORES > 1)
/* Multi-core Shared variables */
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */ /*OS_RPC_SEGMENTNAME_SIMILAR_007*/
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static Os_RpcCoreType Os_InterCore[OS_AUTOSAR_CORES];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
static VAR(Os_SpinlockType, OS_VAR) Os_SpinlockRpc;
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
static FUNC(void, OS_CODE) Os_RpcAddService(LinkQueRefType rpcQue, srvNodeRefType srvStation, RpcNodeIdType nodeId);

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Init the RPC module>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_RpcAddService>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitRpc(void)
{
    VAR(Os_CoreIdType, OS_VAR) ix;
    VAR(uint8, OS_VAR) iy;

    /*only logic master core need to RUN this code.*/
    if (0u == Os_SCB.sysCore)
    {
        for (ix = (Os_CoreIdType)0; ix < OS_AUTOSAR_CORES; ix++)
        {
            Os_InterCore[ix].freeQ.head = INVALID_TRAIN;
            Os_InterCore[ix].freeQ.tail = INVALID_TRAIN;
            Os_InterCore[ix].workQ.head = INVALID_TRAIN;
            Os_InterCore[ix].workQ.tail = INVALID_TRAIN;
            for (iy = 0U; iy < RPC_MAX_NODE; iy++)
            {
                Os_InterCore[ix].srvStation[iy].nextNode = INVALID_TRAIN;
                Os_RpcAddService(&Os_InterCore[ix].freeQ, Os_InterCore[ix].srvStation, iy);
            }
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the number of the service node and remove
                         this node from head of queue>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <uint8>
 * PreCondition         <None>
 * CallByAPI            <Os_GetInternalSpinlock>
 *                      <Os_ReleaseInternalSpinlock>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(uint8, OS_CODE) Os_RpcGetService(LinkQueRefType rpcQue, srvNodeRefType srvStation)
{
    VAR(uint8, OS_VAR) retNode = INVALID_TRAIN;
    OS_ARCH_DECLARE_CRITICAL();

    if ((rpcQue != NULL_PTR) && (srvStation != NULL_PTR))
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_RPC_MACRO_TO_FUNCTION_008 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        Os_GetInternalSpinlock(&Os_SpinlockRpc);
        if (INVALID_TRAIN == rpcQue->head)
        {
            retNode = INVALID_TRAIN;
        }
        else
        {
            retNode = rpcQue->head;
            rpcQue->head = srvStation[retNode].nextNode;
            if (INVALID_TRAIN == rpcQue->head)
            {
                rpcQue->tail = INVALID_TRAIN;
            }
            else
            {
                srvStation[retNode].nextNode = INVALID_TRAIN;
            }
        }
        Os_ReleaseInternalSpinlock(&Os_SpinlockRpc);
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_RPC_MACRO_TO_FUNCTION_008 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }

    return retNode;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Remove the service node nodeId from the queue rpcQue
 *                       of service station srvStation>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <Os_GetInternalSpinlock>
 *                      <Os_ReleaseInternalSpinlock>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(StatusType, OS_CODE)
    Os_RpcRemoveService(LinkQueRefType rpcQue, srvNodeRefType srvStation, RpcNodeIdType nodeId)
{
    VAR(StatusType, OS_VAR) status = E_NOT_OK;
    VAR(uint8, OS_VAR) current;
    VAR(uint8, OS_VAR) previous;
    OS_ARCH_DECLARE_CRITICAL();

    if (nodeId >= RPC_MAX_NODE)
    {
        status = E_OS_ID;
    }
    else if ((NULL_PTR == rpcQue) || (NULL_PTR == srvStation))
    {
        status = E_OS_PARAM_POINTER;
    }
    else
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_RPC_MACRO_TO_FUNCTION_008 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        Os_GetInternalSpinlock(&Os_SpinlockRpc);
        if (rpcQue->head != INVALID_TRAIN)
        {
            current = rpcQue->head;
            previous = INVALID_TRAIN;
            do
            {
                if (current == nodeId)
                {
                    if (nodeId == rpcQue->head)
                    {
                        rpcQue->head = srvStation[current].nextNode;
                    }
                    else
                    {
                        srvStation[previous].nextNode = srvStation[current].nextNode;
                    }
                    if (current == rpcQue->tail)
                    {
                        rpcQue->tail = previous;
                    }
                    srvStation[current].nextNode = INVALID_TRAIN;
                    status = E_OK;
                    break;
                }
                previous = current;
                current = srvStation[current].nextNode;
            } while (current != INVALID_TRAIN);
        }
        Os_ReleaseInternalSpinlock(&Os_SpinlockRpc);
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_RPC_MACRO_TO_FUNCTION_008 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Add the service node nodeId to the queue rpcQue tail
 *                       of service station srvStation>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_GetInternalSpinlock>
 *                      <Os_ReleaseInternalSpinlock>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_RpcAddService(LinkQueRefType rpcQue, srvNodeRefType srvStation, RpcNodeIdType nodeId)
{
    VAR(uint8, OS_VAR) tail;
    OS_ARCH_DECLARE_CRITICAL();

    if ((nodeId < RPC_MAX_NODE) && (rpcQue != NULL_PTR) && (srvStation != NULL_PTR))
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_RPC_MACRO_TO_FUNCTION_008 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        Os_GetInternalSpinlock(&Os_SpinlockRpc);
        if (INVALID_TRAIN == rpcQue->tail)
        {
            rpcQue->head = nodeId;
            rpcQue->tail = nodeId;
            srvStation[nodeId].nextNode = INVALID_TRAIN;
        }
        else
        {
            tail = rpcQue->tail;
            srvStation[tail].nextNode = nodeId;
            srvStation[nodeId].nextNode = INVALID_TRAIN;
            rpcQue->tail = nodeId;
        }
        Os_ReleaseInternalSpinlock(&Os_SpinlockRpc);
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_RPC_MACRO_TO_FUNCTION_008 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the paramters in rpcData to the service node srvNode>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_RpcProcessParameter(
    /* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */ /* OS_RPC_POINTER_CONST_009 */
    RpcInputRefType rpcData,
    srvNodeRefType srvNode
    /* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
)
{
    /* get destination core execution state */
    srvNode->procState = RPC_START;
    srvNode->retValue = E_NOT_OK; /* get return value */
    srvNode->serviceId = rpcData->serviceId;
    srvNode->sync = rpcData->sync;
    srvNode->sourceCoreId = Os_SCB.sysCore;
    srvNode->interParameter[0] = rpcData->srvPara0;
    srvNode->extraParameter[0] = 0U;
    srvNode->extraParameter[1] = 0U;
    srvNode->extraParameter[2] = 0U;
    srvNode->extraParameter[3] = 0U;

    if (OSServiceId_GetElapsedValue == srvNode->serviceId)
    {
        /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_RPC_TYPE_CAST_001 */
        srvNode->interParameter[1] = *(TickRefType)rpcData->srvPara1;
        srvNode->interParameter[2] = *(TickRefType)rpcData->srvPara2;
        /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    }
    else
    {
        srvNode->interParameter[1] = rpcData->srvPara1;
        srvNode->interParameter[2] = rpcData->srvPara2;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Wait the result until timeout>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <Os_RpcRemoveService>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(StatusType, OS_CODE) Os_RpcWaitResult(
    Os_CoreIdType vCoreId,
    srvNodeRefType srvNode,
    /* PRQA S 3673 */ /* MISRA Rule 8.13 */ /* OS_RPC_POINTER_CONST_009 */
    RpcNodeIdType nodeId)
{
    /* CORE1: FREE -> START  ===> CORE2: WORK -> DONE  ===> CORE1: FREE */
    VAR(StatusType, OS_VAR) vRet = E_OK;
    VAR(uint64, OS_VAR) counter = 0U;
    while (RPC_DONE != srvNode->procState)
    {
        counter++;
        if ((srvNode->procState < RPC_WORK) && (((uint64)RPC_WAIT_TIME) == counter))
        {
            StatusType status;
            /* Removing node from workQ, if fails,
            execution is underway, so keep waiting */
            status = Os_RpcRemoveService(&Os_InterCore[vCoreId].workQ, Os_InterCore[vCoreId].srvStation, nodeId);
            if (E_OK == status)
            {
                vRet = E_OS_TIMEOUT; /* time out */
                break;
            }
        }
    }

    return vRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the service node srvNode to the paramters
 *                       in rpcData according to the service ID>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_RpcProcessResult(
    RpcInputRefType rpcData,
    /* PRQA S 3673 */ /* MISRA Rule 8.13 */ /* OS_RPC_POINTER_CONST_009 */
    srvNodeRefType srvNode)
{
    switch (srvNode->serviceId)
    {
/* PRQA S 0306,4342 ++ */ /* MISRA Rule 11.4,10.5 */ /* OS_RPC_TYPE_CAST_002 */
#if (CFG_TASK_MAX > 0U)
    case OSServiceId_GetTaskState:
        *(TaskStateRefType)rpcData->srvPara1 = (Os_TaskStateType)srvNode->interParameter[1];
        break;
#endif /* CFG_TASK_MAX > 0U */

    case OSServiceId_GetCounterValue:
        *(TickRefType)rpcData->srvPara1 = (TickType)srvNode->interParameter[1];
        break;

    case OSServiceId_GetElapsedValue:
        *(TickRefType)rpcData->srvPara1 = (TickType)srvNode->interParameter[1];
        *(TickRefType)rpcData->srvPara2 = (TickType)srvNode->interParameter[2];
        break;

#if (CFG_ALARM_MAX > 0)
    case OSServiceId_GetAlarm:
        *(TickRefType)rpcData->srvPara1 = (TickType)srvNode->interParameter[1];
        break;

    case OSServiceId_GetAlarmBase: {
        AlarmBaseRefType almRet = (AlarmBaseRefType)rpcData->srvPara1;
        /* PRQA S 3679 ++ */ /* MISRA Rule 8.13 */ /* OS_RPC_POINTER_CONST_011 */
        AlarmBaseRefType almBaseInfo =
            /* PRQA S 3679 -- */                       /* MISRA Rule 8.13 */
            /* PRQA S 0310 ++ */ /* MISRA Rule 11.3 */ /*OS_RPC_CAST_TYPE_010*/
            (AlarmBaseRefType)&srvNode->extraParameter[0];
        /* PRQA S 0310 -- */ /* MISRA Rule 11.3 */
        almRet->maxallowedvalue = almBaseInfo->maxallowedvalue;
        almRet->mincycle = almBaseInfo->mincycle;
        almRet->ticksperbase = almBaseInfo->ticksperbase;
        break;
    }
#endif /* CFG_ALARM_MAX > 0 */

#if (CFG_SCHEDTBL_MAX > 0U)
    case OSServiceId_GetScheduleTableStatus:
        *(ScheduleTableStatusRefType)rpcData->srvPara1 = (Os_SchedTblStateType)srvNode->interParameter[1];
        break;
#endif                                               /* CFG_SCHEDTBL_MAX > 0U */
    /* PRQA S 0306,4342 -- */                        /* MISRA Rule 11.4,10.5 */
    default: /* PRQA S 2016 */ /* MISRA Rule 16.4 */ /* OS_RPC_DEFAULT_EMPTY_003 */
        break;
    }
    srvNode->procState = RPC_FREE;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Make a remote service call to given core with
 *                       given service ID and input parameters>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <Os_RpcGetService>, <Os_RpcProcessParameter>
 *                      <Os_RpcAddService>, <Os_ArchRemoteCall>
 *                      <Os_RpcWaitResult>, <Os_RpcProcessResult>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_RpcCallService(RpcInputRefType rpcData)
{
    VAR(StatusType, OS_VAR) vRet = E_OK;
    VAR(uint16, OS_VAR) vCoreId;
    VAR(srvNodeRefType, OS_VAR) srvNode;
    VAR(RpcNodeIdType, OS_VAR) nodeId = 0;

    if (NULL_PTR == rpcData)
    {
        vRet = E_OS_PARAM_POINTER;
    }
    else if (rpcData->remoteCoreId > OS_AUTOSAR_CORES)
    {
        vRet = E_OS_CORE;
    }
    else if (Os_CoreCB.coreStatus[rpcData->remoteCoreId] != OS_RUN)
    {
        vRet = E_OS_CORE;
    }
    else
    {
        vCoreId = rpcData->remoteCoreId;
        nodeId = Os_RpcGetService(&Os_InterCore[vCoreId].freeQ, Os_InterCore[vCoreId].srvStation);
        if (INVALID_TRAIN == nodeId)
        {
            vRet = E_BUSY;
        }
        else
        {
            srvNode = &Os_InterCore[vCoreId].srvStation[nodeId];

            Os_RpcProcessParameter(rpcData, srvNode);

            Os_RpcAddService(&Os_InterCore[vCoreId].workQ, Os_InterCore[vCoreId].srvStation, nodeId);
            /* PRQA S 0303, 3469 ++ */ /* MISRA Rule 11.4, Dir 4.9 */ /* OS_RPC_TYPE_CAST_004 */
            /* OS_RPC_MACRO_TO_FUNCTION_008 */
            Os_ArchRemoteCall(Os_GetCorePhyID(rpcData->remoteCoreId));
            /* PRQA S 0303, 3469 -- */ /* MISRA Rule 11.4, Dir 4.9 */
            if (srvNode->sync == RPC_SYNC)
            {
                StatusType status = E_OK;
                status = Os_RpcWaitResult(vCoreId, srvNode, nodeId);
                if (RPC_DONE == srvNode->procState)
                {
                    Os_RpcProcessResult(rpcData, srvNode);
                    vRet = srvNode->retValue;
                }
                else
                {
                    vRet = status;
                }
                Os_RpcAddService(&Os_InterCore[vCoreId].freeQ, Os_InterCore[vCoreId].srvStation, nodeId);
            }
        }
    }
    return vRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Call the specified function according to service ID>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Panic>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_RpcServiceAction(srvNodeRefType srvNode)
{
    VAR(Os_CoreIdType, OS_VAR) osCoreId = Os_SCB.sysCore;

    if (osCoreId < OS_AUTOSAR_CORES)
    {
        switch (srvNode->serviceId)
        {
#if (CFG_TASK_MAX > 0U)
        case OSServiceId_ActivateTask:
            /*SWS_Os_00596*/
            srvNode->retValue = Os_ActivateTask((Os_TaskType)srvNode->interParameter[0]);
            break;

        case OSServiceId_ChainTask:
            /*SWS_Os_00600*/
            /*the second part of ChainTask is ActivateTask*/
            srvNode->retValue = Os_ActivateTask((Os_TaskType)srvNode->interParameter[0]);
            break;

        case OSServiceId_GetTaskState:
            Os_GetTaskState(
                (Os_TaskType)srvNode->interParameter[0],
                /* PRQA S 0310 ++ */ /* MISRA Rule 11.3 */ /*OS_RPC_CAST_TYPE_010*/
                (Os_TaskStateRefType)&srvNode->interParameter[1]);
            /* PRQA S 0310 -- */ /* MISRA Rule 11.3 */
            srvNode->retValue = E_OK;
            break;
#endif /* CFG_TASK_MAX > 0U */

#if (CFG_EXTENDED_TASK_MAX > 0)
        case OSServiceId_SetEvent: {
            Os_EventMaskType EventLow = (Os_EventMaskType)(srvNode->interParameter[1]);
            Os_EventMaskType EventHigh = ((Os_EventMaskType)(srvNode->interParameter[2]) << 32u);
            srvNode->retValue =
                Os_SetEvent((Os_TaskType)srvNode->interParameter[0], (Os_EventMaskType)(EventLow | EventHigh));
            break;
        }
#endif /* CFG_EXTENDED_TASK_MAX > 0 */

        case OSServiceId_GetCounterValue:
            Os_GetCounterValue((CounterType)srvNode->interParameter[0], (Os_TickRefType)&srvNode->interParameter[1]);
            srvNode->retValue = E_OK;
            break;

        case OSServiceId_GetElapsedValue:
            srvNode->retValue = Os_GetElapsedValue(
                (CounterType)srvNode->interParameter[0],
                (Os_TickRefType)&srvNode->interParameter[1],
                (Os_TickRefType)&srvNode->interParameter[2]);
            break;

#if (CFG_ALARM_MAX > 0)
        case OSServiceId_GetAlarm:
            srvNode->retValue =
                Os_GetAlarm((Os_AlarmType)srvNode->interParameter[0], (Os_TickRefType)&srvNode->interParameter[1]);
            break;

        case OSServiceId_GetAlarmBase:
            Os_GetAlarmBase(
                (Os_AlarmType)srvNode->interParameter[0],
                /* PRQA S 0310 ++ */ /* MISRA Rule 11.3 */ /*OS_RPC_CAST_TYPE_010*/
                (Os_AlarmBaseRefType)&srvNode->extraParameter[0]);
            /* PRQA S 0310 -- */ /* MISRA Rule 11.3 */
            srvNode->retValue = E_OK;
            break;

        case OSServiceId_CancelAlarm:
            srvNode->retValue = Os_CancelAlarm((Os_AlarmType)srvNode->interParameter[0]);
            break;

        case OSServiceId_SetRelAlarm:
            srvNode->retValue = Os_SetRelAlarm(
                (Os_AlarmType)srvNode->interParameter[0],
                (Os_TickType)srvNode->interParameter[1],
                (Os_TickType)srvNode->interParameter[2]);
            break;

        case OSServiceId_SetAbsAlarm:
            srvNode->retValue = Os_SetAbsAlarm(
                (Os_AlarmType)srvNode->interParameter[0],
                (Os_TickType)srvNode->interParameter[1],
                (Os_TickType)srvNode->interParameter[2]);
            break;
#endif /* CFG_ALARM_MAX > 0 */

#if (CFG_SCHEDTBL_MAX > 0U)
        case OSServiceId_GetScheduleTableStatus:
            Os_GetScheduleTableStatus(
                (ScheduleTableType)srvNode->interParameter[0],
                /* PRQA S 0310 ++ */ /* MISRA Rule 11.3 */ /*OS_RPC_CAST_TYPE_010*/
                (ScheduleTableStatusRefType)&srvNode->interParameter[1]);
            /* PRQA S 0310 -- */ /* MISRA Rule 11.3 */
            srvNode->retValue = E_OK;
            break;

        case OSServiceId_StartScheduleTableRel:
            srvNode->retValue = Os_StartScheduleTableRel(
                (ScheduleTableType)srvNode->interParameter[0],
                (TickType)srvNode->interParameter[1]);
            break;

        case OSServiceId_StartScheduleTableAbs:
            srvNode->retValue = Os_StartScheduleTableAbs(
                (ScheduleTableType)srvNode->interParameter[0],
                (TickType)srvNode->interParameter[1]);
            break;

        case OSServiceId_StopScheduleTable:
            srvNode->retValue = Os_StopScheduleTable((ScheduleTableType)srvNode->interParameter[0]);
            break;
#endif /* CFG_SCHEDTBL_MAX > 0U */

        case OSServiceId_ShutdownOS:
            Os_ShutdownOS((StatusType)srvNode->interParameter[0], SHUTDOWN_ALL_OS);
            break;

#if (CFG_IOC_MAX > 0u)
        case OSServiceId_IocCallBackNotify: {
            /* PRQA S 4461 ++ */ /* MISRA Rule 10.3 */ /* OS_RPC_TYPE_CAST_005 */
            Os_IocComIdType comId = srvNode->interParameter[0];
            Os_IocU16Type vReceiverId = srvNode->interParameter[1];
            Os_ApplicationType vRecAppId = srvNode->interParameter[2];
            /* PRQA S 4461 -- */ /* MISRA Rule 10.3 */

            srvNode->retValue = Os_IocRemoteCallBack(comId, vReceiverId, vRecAppId);
            break;
        }
#endif /* CFG_IOC_MAX > 0u */

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        case OSServiceId_TerminateApplication:
            /* PRQA S 4342 ++ */ /* MISRA Rule 10.5 */ /* OS_RPC_TYPE_CAST_006 */
            Os_TerminateApplication(
                (ApplicationType)srvNode->interParameter[0],
                (RestartType)srvNode->interParameter[1]);
            srvNode->retValue = E_OK;
            /* PRQA S 4342 -- */ /* MISRA Rule 10.5 */
            break;
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

        /*add comments to pass QAC.*/
        default:
            Os_Panic();
            break;
        }

        srvNode->procState = RPC_DONE;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Process all received RPC service request on the
                         core given osCoreId>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_RpcGetService>, <Os_RpcServiceAction>
 *                      <Os_RpcAddService>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_RpcServiceOperation(Os_CoreIdType osCoreId)
{
    VAR(srvNodeRefType, OS_VAR) srvNode;
    VAR(RpcNodeIdType, OS_VAR) nodeId;

    while (1)
    {
        nodeId = Os_RpcGetService(&Os_InterCore[osCoreId].workQ, Os_InterCore[osCoreId].srvStation);
        if (nodeId == INVALID_TRAIN)
        {
            break;
        }
        srvNode = &Os_InterCore[osCoreId].srvStation[nodeId];
        srvNode->procState = RPC_WORK;
        Os_RpcServiceAction(srvNode);
        if (srvNode->sync != RPC_SYNC)
        {
            srvNode->procState = RPC_FREE;
            Os_RpcAddService(&Os_InterCore[osCoreId].freeQ, Os_InterCore[osCoreId].srvStation, nodeId);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_AUTOSAR_CORES > 1 */

/*=======[E N D   O F   F I L E]==============================================*/
