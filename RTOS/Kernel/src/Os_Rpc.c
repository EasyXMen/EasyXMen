/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static Os_RpcCoreType Os_InterCore[OS_AUTOSAR_CORES];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
static Os_SpinlockType Os_SpinlockRpc;
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
static void Os_RpcAddService(LinkQueRefType rpcQue, srvNodeRefType srvStation, RpcNodeIdType nodeId);

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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitRpc(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType ix;
    uint8         iy;

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
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static uint8 Os_RpcGetService(LinkQueRefType rpcQue, srvNodeRefType srvStation)
/* PRQA S 3450 -- */
{
    uint8 retNode = INVALID_TRAIN;
    OS_ARCH_DECLARE_CRITICAL();

    if ((rpcQue != NULL_PTR) && (srvStation != NULL_PTR))
    {
        OS_ARCH_ENTRY_CRITICAL();
        Os_GetInternalSpinlock(&Os_SpinlockRpc);
        if (INVALID_TRAIN == rpcQue->head)
        {
            retNode = INVALID_TRAIN;
        }
        else
        {
            retNode      = rpcQue->head;
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

        OS_ARCH_EXIT_CRITICAL();
    }

    return retNode;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == RPC_TIMEOUT_SUPPORT)
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
 * CallByAPI            <Os_RpcWaitResult>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 3450 ++ */ /* VL_MTR_Os_STMIF, VL_Os_3450 */
static StatusType Os_RpcRemoveService(LinkQueRefType rpcQue, srvNodeRefType srvStation, RpcNodeIdType nodeId)
/* PRQA S 6030, 3450 -- */
{
    StatusType status = E_NOT_OK;
    uint8      current;
    uint8      previous;
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
        OS_ARCH_ENTRY_CRITICAL();
        Os_GetInternalSpinlock(&Os_SpinlockRpc);
        if (rpcQue->head != INVALID_TRAIN)
        {
            current  = rpcQue->head;
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
                    status                       = E_OK;
                    break;
                }
                previous = current;
                current  = srvStation[current].nextNode;
            } while (current != INVALID_TRAIN);
        }
        Os_ReleaseInternalSpinlock(&Os_SpinlockRpc);

        OS_ARCH_EXIT_CRITICAL();
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

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
static void Os_RpcAddService(LinkQueRefType rpcQue, srvNodeRefType srvStation, RpcNodeIdType nodeId)
{
    uint8 tail;
    OS_ARCH_DECLARE_CRITICAL();

    if ((nodeId < RPC_MAX_NODE) && (rpcQue != NULL_PTR) && (srvStation != NULL_PTR))
    {
        OS_ARCH_ENTRY_CRITICAL();
        Os_GetInternalSpinlock(&Os_SpinlockRpc);
        if (INVALID_TRAIN == rpcQue->tail)
        {
            rpcQue->head                = nodeId;
            rpcQue->tail                = nodeId;
            srvStation[nodeId].nextNode = INVALID_TRAIN;
        }
        else
        {
            tail                        = rpcQue->tail;
            srvStation[tail].nextNode   = nodeId;
            srvStation[nodeId].nextNode = INVALID_TRAIN;
            rpcQue->tail                = nodeId;
        }
        Os_ReleaseInternalSpinlock(&Os_SpinlockRpc);

        OS_ARCH_EXIT_CRITICAL();
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
/* PRQA S 3450, 3673 ++ */ /* VL_Os_3450, VL_QAC_3673 */
static void Os_RpcProcessParameter(RpcInputRefType rpcData, srvNodeRefType srvNode)
/* PRQA S 3450, 3673 -- */
{
    /* get destination core execution state */
    srvNode->procState    = RPC_START;
    srvNode->retValue     = E_NOT_OK; /* get return value */
    srvNode->serviceId    = rpcData->serviceId;
    srvNode->sync         = rpcData->sync;
    srvNode->sourceCoreId = Os_SCB.sysCore;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    srvNode->interParameter[0] = rpcData->srvPara0;
    srvNode->extraParameter[0] = 0U;
    srvNode->extraParameter[1] = 0U;
    srvNode->extraParameter[2] = 0U;
    srvNode->extraParameter[3] = 0U;

    if (OSServiceId_GetElapsedValue == srvNode->serviceId)
    {
        /* PRQA S 0306 ++ */ /* VL_Os_0306 */
        srvNode->interParameter[1] = *(TickRefType)rpcData->srvPara1;
        srvNode->interParameter[2] = *(TickRefType)rpcData->srvPara2;
        /* PRQA S 0306 -- */
    }
    else
    {
        srvNode->interParameter[1] = rpcData->srvPara1;
        srvNode->interParameter[2] = rpcData->srvPara2;
    }
    /* PRQA S 3120 -- */
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
 * CallByAPI            <Os_RpcCallService>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450, 3673 ++ */ /* VL_Os_3450, VL_QAC_3673 */
static StatusType Os_RpcWaitResult(Os_CoreIdType vCoreId, srvNodeRefType srvNode, RpcNodeIdType nodeId)
/* PRQA S 3450, 3673 -- */
{
    /* CORE1: FREE -> START  ===> CORE2: WORK -> DONE  ===> CORE1: FREE */
    StatusType vRet    = E_OK;
    uint64     counter = 0U;
    /* PRQA S 3442 ++ */ /* VL_Os_3442 */
    while (RPC_DONE != srvNode->procState)
    /* PRQA S 3442 -- */
    {
        counter++;
        if (Os_CoreCB.coreStatus[vCoreId] != OS_RUN) /* PRQA S 3442 */ /* VL_Os_3442 */
        {
            vRet = E_OS_CORE;
            break;
        }

#if (TRUE == RPC_TIMEOUT_SUPPORT)
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if ((srvNode->procState < RPC_WORK) && (((uint64)RPC_WAIT_TIME) == counter))
        /* PRQA S 3442 -- */
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
#endif
    }

    UNUSED_PARAMETER(nodeId);
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
/* PRQA S 3450, 3673 ++ */ /* VL_Os_3450, VL_QAC_3673 */
static void Os_RpcProcessResult(RpcInputRefType rpcData, srvNodeRefType srvNode)
/* PRQA S 3450, 3673 -- */
{
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    switch (srvNode->serviceId)
    {
#if (CFG_TASK_MAX > 0U)
    case OSServiceId_GetTaskState:
        /* PRQA S 4342 ++ */ /* VL_Os_4342 */
        *(TaskStateRefType)rpcData->srvPara1 = (Os_TaskStateType)srvNode->interParameter[1];
        /* PRQA S 4342 -- */
        break;
#endif /* CFG_TASK_MAX > 0U */

    case OSServiceId_GetCounterValue:
        *(TickRefType)rpcData->srvPara1 = (TickType)srvNode->interParameter[1];
        break;

    case OSServiceId_GetElapsedValue:
        *(TickRefType)rpcData->srvPara1 = (TickType)srvNode->interParameter[1];
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        *(TickRefType)rpcData->srvPara2 = (TickType)srvNode->interParameter[2];
        /* PRQA S 3120 -- */
        break;

#if (CFG_ALARM_MAX > 0)
    case OSServiceId_GetAlarm:
        *(TickRefType)rpcData->srvPara1 = (TickType)srvNode->interParameter[1];
        break;

    case OSServiceId_GetAlarmBase:
    {
        AlarmBaseRefType almRet = (AlarmBaseRefType)rpcData->srvPara1;
        /* PRQA S 0310, 3679 ++ */ /* VL_Os_0310, VL_Os_3679 */
        AlarmBaseRefType almBaseInfo = (AlarmBaseRefType)&srvNode->extraParameter[0];
        /* PRQA S 0310, 3679 -- */
        almRet->maxallowedvalue = almBaseInfo->maxallowedvalue;
        almRet->mincycle        = almBaseInfo->mincycle;
        almRet->ticksperbase    = almBaseInfo->ticksperbase;
        break;
    }
    /* PRQA S 0306 -- */
#endif /* CFG_ALARM_MAX > 0 */

#if (CFG_SCHEDTBL_MAX > 0U)
    case OSServiceId_GetScheduleTableStatus:
        *(ScheduleTableStatusRefType)rpcData->srvPara1 = (Os_SchedTblStateType)srvNode->interParameter[1];
        break;
#endif /* CFG_SCHEDTBL_MAX > 0U */

    default: /* PRQA S 2016 */ /* VL_QAC_EmptyClause */
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
/* PRQA S 6030 ++ */ /* VL_MTR_Os_STMIF */
StatusType Os_RpcCallService(RpcInputRefType rpcData)
/* PRQA S 6030 -- */
{
    StatusType     vRet = E_OK;
    uint16         vCoreId;
    srvNodeRefType srvNode;
    RpcNodeIdType  nodeId = INVALID_TRAIN;

    if (NULL_PTR == rpcData)
    {
        vRet = E_OS_PARAM_POINTER;
    }
    else if (rpcData->remoteCoreId > OS_AUTOSAR_CORES)
    {
        vRet = E_OS_CORE;
    }
    else
    {
        vCoreId = rpcData->remoteCoreId;
        do
        {
            if (Os_CoreCB.coreStatus[vCoreId] != OS_RUN) /* PRQA S 3442 */ /* VL_Os_3442 */
            {
                vRet = E_OS_CORE;
                break;
            }
            nodeId = Os_RpcGetService(&Os_InterCore[vCoreId].freeQ, Os_InterCore[vCoreId].srvStation);
        }while (INVALID_TRAIN == nodeId);

        if(E_OK == vRet)
        {
            srvNode = &Os_InterCore[vCoreId].srvStation[nodeId];

            Os_RpcProcessParameter(rpcData, srvNode);

            Os_RpcAddService(&Os_InterCore[vCoreId].workQ, Os_InterCore[vCoreId].srvStation, nodeId);
            /* PRQA S 1258, 3138, 3455, 0303 ++ */ /* VL_Os_ConstToIntegral, VL_Os_3138, VL_Os_3455, VL_Os_0303 */
            /* OS_RPC_MACRO_TO_FUNCTION_008 */
            Os_ArchRemoteCall(Os_GetCorePhyID(rpcData->remoteCoreId));
            /* PRQA S 1258, 3138, 3455, 0303 -- */
            if (srvNode->sync == RPC_SYNC)
            {
                StatusType status = E_OK;
                status            = Os_RpcWaitResult(vCoreId, srvNode, nodeId);
                if (RPC_DONE == srvNode->procState) /* PRQA S 3442 */ /* VL_Os_3442 */
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
/* PRQA S 6010, 6020, 6070, 3450 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STLIN, VL_MTR_Os_STCAL, VL_Os_3450 */
static void Os_RpcServiceAction(srvNodeRefType srvNode)
/* PRQA S 6010, 6020, 6070, 3450 -- */
{
    Os_CoreIdType osCoreId = Os_SCB.sysCore;

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
            /* PRQA S 0310 ++ */ /* VL_Os_0310 */
            Os_GetTaskState((Os_TaskType)srvNode->interParameter[0], (Os_TaskStateRefType)&srvNode->interParameter[1]);
            /* PRQA S 0310 -- */
            srvNode->retValue = E_OK;
            break;
#endif /* CFG_TASK_MAX > 0U */

#if (CFG_EXTENDED_TASK_MAX > 0)
        case OSServiceId_SetEvent:
        {
            Os_EventMaskType EventLow = (Os_EventMaskType)(srvNode->interParameter[1]);
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            Os_EventMaskType EventHigh = ((Os_EventMaskType)(srvNode->interParameter[2]) << 32u);
            /* PRQA S 3120 -- */
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
                /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
                (Os_TickRefType)&srvNode->interParameter[2]);
            /* PRQA S 3120 -- */
            break;

#if (CFG_ALARM_MAX > 0)
        case OSServiceId_GetAlarm:
            srvNode->retValue =
                Os_GetAlarm((Os_AlarmType)srvNode->interParameter[0], (Os_TickRefType)&srvNode->interParameter[1]);
            break;

        case OSServiceId_GetAlarmBase:
            Os_GetAlarmBase(
                (Os_AlarmType)srvNode->interParameter[0],
                /* PRQA S 0310 ++ */ /* VL_Os_0310 */
                (Os_AlarmBaseRefType)&srvNode->extraParameter[0]);
            /* PRQA S 0310 -- */
            srvNode->retValue = E_OK;
            break;

        case OSServiceId_CancelAlarm:
            srvNode->retValue = Os_CancelAlarm((Os_AlarmType)srvNode->interParameter[0]);
            break;

        case OSServiceId_SetRelAlarm:
            srvNode->retValue = Os_SetRelAlarm(
                (Os_AlarmType)srvNode->interParameter[0],
                (Os_TickType)srvNode->interParameter[1],
                /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
                (Os_TickType)srvNode->interParameter[2]);
            /* PRQA S 3120 -- */
            break;

        case OSServiceId_SetAbsAlarm:
            srvNode->retValue = Os_SetAbsAlarm(
                (Os_AlarmType)srvNode->interParameter[0],
                (Os_TickType)srvNode->interParameter[1],
                /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
                (Os_TickType)srvNode->interParameter[2]);
            /* PRQA S 3120 -- */
            break;
#endif /* CFG_ALARM_MAX > 0 */

#if (CFG_SCHEDTBL_MAX > 0U)
        case OSServiceId_GetScheduleTableStatus:
            Os_GetScheduleTableStatus(
                (ScheduleTableType)srvNode->interParameter[0],

                (ScheduleTableStatusRefType)&srvNode->interParameter[1]);

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
        case OSServiceId_IocCallBackNotify:
        {
            /* PRQA S 3120, 4461 ++ */ /* VL_QAC_MagicNum, VL_Os_4461 */
            Os_IocComIdType    comId       = srvNode->interParameter[0];
            Os_IocU16Type      vReceiverId = srvNode->interParameter[1];
            Os_ApplicationType vRecAppId   = srvNode->interParameter[2];
            /* PRQA S 3120, 4461 -- */

            srvNode->retValue = Os_IocRemoteCallBack(comId, vReceiverId, vRecAppId);
            break;
        }
#endif /* CFG_IOC_MAX > 0u */

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        case OSServiceId_TerminateApplication:
            Os_TerminateApplication(
                (ApplicationType)srvNode->interParameter[0],
                /* PRQA S 4342 ++ */ /* VL_Os_4342 */
                (RestartType)srvNode->interParameter[1]);
            /* PRQA S 4342 -- */
            srvNode->retValue = E_OK;
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
void Os_RpcServiceOperation(Os_CoreIdType osCoreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    srvNodeRefType srvNode;
    RpcNodeIdType  nodeId;

    while (1) /* PRQA S 2870, 2740 */ /* VL_Os_2870, VL_Os_2740 */
    {
        nodeId = Os_RpcGetService(&Os_InterCore[osCoreId].workQ, Os_InterCore[osCoreId].srvStation);
        if (nodeId == INVALID_TRAIN)
        {
            break;
        }
        srvNode            = &Os_InterCore[osCoreId].srvStation[nodeId];
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
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
