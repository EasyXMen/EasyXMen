/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Os_Event.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : event manager                                               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/
/* PRQA S 3472 ++ */ /* MISRA Rule 4.9 */ /* OS_EVENT_MACRO_003 */
#define OS_EVENT_PERMISSION_CHECKING(EventMask, AccessMask)   Os_EventBitCompare(EventMask, AccessMask)
#define OS_EVENT_IS_ALL_EVENTS_TRIGGERED(EventMask, SetEvent) Os_EventBitCompare(EventMask, SetEvent)
/* PRQA S 3472 -- */ /* MISRA Rule 4.9 */
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
#if (CFG_EXTENDED_TASK_MAX > 0)
#if (OS_STATUS_EXTENDED == CFG_STATUS)
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */ /*OS_EVENT_SEGMENTNAME_SIMILAR_003*/
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
static P2CONST(Os_EventMaskRefType, OS_VAR, OS_CONST) Os_TaskEventAccessMask;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */
#endif
/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Init the Event control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-eventId[in]    <Event control block id>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitEvent(void)
{
    VAR(Os_TaskType, OS_VAR) i;
    Os_CfgExtendTaskMax = Os_CfgExtendTaskMax_Inf[Os_SCB.sysCore];

    for (i = 0U; i < CFG_EXTENDED_TASK_MAX; i++)
    {
        if (i >= Os_CfgExtendTaskMax)
        {
            break;
        }
        Os_ECB[i].eventSetEvent = 0U;
        Os_ECB[i].eventWaitEvent = 0U;
        Os_ECB[i].eventIsWaitAllEvents = FALSE;

#if ((TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) || (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR))
        Os_InitMonitorEvent(i);
#endif /* (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) || (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR) */
    }

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 0310,0311 ++ */ /* MISRA Rule 11.8,11.3 */ /* OS_EVENT_TYPE_CAST_001 */
    Os_TaskEventAccessMask = (const Os_EventMaskRefType*)((void**)Os_TaskEventAccessMask_Inf[Os_SCB.sysCore]);
/* PRQA S 0310,0311 -- */ /* MISRA Rule 11.8,11.3 */
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the task status to ready and prepare for scheduling.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <TaskID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SetEvent>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_EventTaskDispatch(TaskType TaskID)
{
    Os_TCB[TaskID].taskState = TASK_STATE_READY;

    Os_ReadyQueueInsert(TaskID, OS_LEVEL_TASK, Os_TCB[TaskID].taskRunPrio);

    if (Os_TCB[TaskID].taskRunPrio > Os_SCB.sysHighPrio)
    {
        Os_SCB.sysHighTaskID = TaskID;
        Os_SCB.sysHighPrio = Os_TCB[TaskID].taskRunPrio;

#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
        if (Os_SCB.sysDispatchLocker == 0u)
        {
            OS_START_DISPATCH();
            Os_Dispatch(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
        }
#endif
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Bitwise comparison.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <StatusType>
 * @param[in]           <Mask>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <StatusType>
 * PreCondition         <None>
 * CallByAPI            <WaitEvent or ClearEvent>
 * REQ ID               <None>
 */
/******************************************************************************/
static inline FUNC(StatusType, OS_CODE) Os_EventBitCompare(EventMaskType EventMask, EventMaskType Mask)
{
    VAR(StatusType, OS_VAR) err = E_OK;

    if (EventMask != (EventMask & Mask))
    {
        err = E_OS_ACCESS;
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The service may be called from an interrupt service routine and
 *                       from the task level, but not from hook routines.
 *                       The events of task <TaskID> are set according to the event
 *                       mask <Mask>. Calling SetEvent causes the task <TaskID> to
 *                       be transferred to the ready state, if it was waiting for at least
 *                       one of the events specified in <Mask>>
 * Service ID           <0xec>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task for which one or several events are to be set.>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) SetEvent(TaskType TaskID, EventMaskType Mask)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_SetEvent);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */ /* OS_EVENT_MACRO_005 */
    if (CHECK_ID_INVALID(TaskID, Os_CfgTaskMax_Inf))
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_SET_EVENT) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_TASK, TaskID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Rule 4.9*/ /* OS_EVENT_MACRO_004 */
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_GetObjCoreId(TaskID);
        /* PRQA S 3469 -- */ /* MISRA Rule 4.9*/
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_SetEvent,
                .srvPara0 = (uint32)TaskID,
                .srvPara1 = (uint32)(Mask & 0xFFFFFFFFu),
                .srvPara2 = (uint32)((Mask >> 32u)),
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_SetEvent(TaskID, Mask);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetEvent(TaskID, Mask), OSServiceId_SetEvent, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_SetEvent);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Asynchronous version of the SetEvent() function.>
 * Service ID           <0xec>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task for which one or several events are to be set.>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) SetEventAsyn(TaskType TaskID, EventMaskType Mask)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_SetEventAsyn);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_EVENT_MACRO_005 */
    if (CHECK_ID_INVALID(TaskID, Os_CfgTaskMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_SET_EVENT) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_TASK, TaskID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Rule 4.9*/ /* OS_EVENT_MACRO_004 */
        Os_CoreIdType coreId = Os_GetObjCoreId(TaskID);
        /* PRQA S 3469 -- */ /* MISRA Rule 4.9*/
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_ASYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_SetEvent,
                .srvPara0 = (uint32)TaskID,
                .srvPara1 = (uint32)(Mask & 0xFFFFFFFFu),
                .srvPara2 = (uint32)((Mask >> 32u)),
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_SetEvent(TaskID, Mask);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetEvent(TaskID, Mask), OSServiceId_SetEvent, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_SetEventAsyn);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The state of the calling task is set to waiting, unless
 *                       at least one of the events specified in <Mask> has
 *                       already been set.
 *                       Particularities: This call enforces rescheduling,
 *                       if the wait condition occurs. If rescheduling takes
 *                       place, the internal resource of the task is released
 *                       while the task is in the waiting state.
 *                       This service must only be called from the extended task
 *                       owning the event.
 *                       The system service ClearEvent is restricted to extended
 *                       taskswhich own the event.>
 * Service ID           <0xef>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) WaitEvent(EventMaskType Mask)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_WaitEvent);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_WAIT_EVENT) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
        if (Os_SCB.sysOsLevel != OS_LEVEL_TASK)
        {
            err = E_OS_CALLEVEL;
        }
        else
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */
        {
            if (Os_SCB.sysRunningTaskID >= Os_CfgExtendTaskMax)
            {
                err = E_OS_ACCESS;
            }

#if (CFG_STD_RESOURCE_MAX > 0U)
            else if (Os_TCB[Os_SCB.sysRunningTaskID].taskResCount > 0u)
            {
                err = E_OS_RESOURCE;
            }
#endif /* CFG_STD_RESOURCE_MAX > 0U */
#if (CFG_SPINLOCK_MAX > 0U)
            else if (Os_SpinlockSafetyCheck() != E_OK)
            {
                err = E_OS_SPINLOCK;
            }
#endif
            else
            {
                /* Task access EVENT permission check */
                /* PRQA S 3469 ++ */ /* MISRA Rule 4.9*/ /* OS_EVENT_MACRO_004 */
                err = OS_EVENT_PERMISSION_CHECKING(Mask, *Os_TaskEventAccessMask[Os_SCB.sysRunningTaskID]);
                /* PRQA S 3469 -- */ /* MISRA Rule 4.9*/
            }
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == err)
        {
            err = Os_WaitEvent(Mask);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_WaitEvent(Mask), OSServiceId_WaitEvent, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_WaitEvent);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The state of the calling task is set to waiting, must
 *                       all the events specified in <Mask> has already been set.
 *                       Particularities: This call enforces rescheduling,
 *                       if the wait condition occurs. If rescheduling takes
 *                       place, the internal resource of the task is released
 *                       while the task is in the waiting state.
 *                       This service must only be called from the extended task
 *                       owning the event.
 *                       The system service ClearEvent is restricted to extended
 *                       taskswhich own the event.>
 * Service ID           <0xD7>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) WaitAllEvents(EventMaskType Mask)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_WaitAllEvents);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_WAIT_EVENT) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
        if (Os_SCB.sysOsLevel != OS_LEVEL_TASK)
        {
            err = E_OS_CALLEVEL;
        }
        else
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */
        {
            if (Os_SCB.sysRunningTaskID >= Os_CfgExtendTaskMax)
            {
                err = E_OS_ACCESS;
            }

#if (CFG_STD_RESOURCE_MAX > 0U)
            else if (Os_TCB[Os_SCB.sysRunningTaskID].taskResCount > 0u)
            {
                err = E_OS_RESOURCE;
            }
#endif /* CFG_STD_RESOURCE_MAX > 0U */
#if (CFG_SPINLOCK_MAX > 0U)
            else if (Os_SpinlockSafetyCheck() != E_OK)
            {
                err = E_OS_SPINLOCK;
            }
#endif
            else
            {
                /* Task access EVENT permission check */
                /* PRQA S 3469 ++ */ /* MISRA Rule 4.9*/ /* OS_EVENT_MACRO_004 */
                err = OS_EVENT_PERMISSION_CHECKING(Mask, *Os_TaskEventAccessMask[Os_SCB.sysRunningTaskID]);
                /* PRQA S 3469 -- */ /* MISRA Rule 4.9*/
            }
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == err)
        {
            err = Os_WaitAllEvents(Mask);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_WaitEvent(Mask), OSServiceId_WaitAllEvents, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_WaitAllEvents);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service returns the current state of all event
 *                       bits of the task
 *                       <TaskID>, not the events that the task is waiting for.
 *                       The service may be called from interrupt service
 *                       routines, task level and some hook routines
 *                       (see Figure 12-1).
 *                       The current status of the event mask of task <TaskID>
 *                       is copied to <Event>.
 *                       The referenced task must be an extended task.>
 * Service ID           <0xee>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task for which one or several events
 *                       are to be set.>
 * Param-Event[out]     <Reference to the memory of the return data>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) GetEvent(TaskType TaskID, EventMaskRefType Event)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetEvent);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_EVENT_MACRO_005 */
    if (CHECK_ID_INVALID(TaskID, Os_CfgTaskMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else if (NULL_PTR == Event)
    {
        err = E_OS_PARAM_POINTER;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_EVENT) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_EVENT_TYPE_CAST_002 */
    else if (Os_AddressWritable((uint32)Event) != TRUE)
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
        if (Os_SCB.sysCore != Os_GetObjCoreId(TaskID))
        /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
        {
            err = E_OS_CORE;
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
#if (OS_AUTOSAR_CORES > 1)
            /* PRQA S 3469,1338 ++ */ /* MISRA  Rule 17.8,4.9 */ /* OS_EVENT_MACRO_004 */
            TaskID = Os_GetObjLocalId(TaskID);
            /* PRQA S 3469,1338 -- */ /* MISRA  Rule 17.8,4.9 */
#endif

/* Extended Status */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
            if (TaskID >= Os_CfgExtendTaskMax)
            {
                err = E_OS_ACCESS;
            }
            else
            {
                /* PRQA S 3469 ++ */ /* MISRA Rule 4.9 */ /* OS_EVENT_MACRO_004 */
                OS_ARCH_ENTRY_CRITICAL();
                /* PRQA S 3469 -- */ /* MISRA Rule 4.9 */
                if (TASK_STATE_SUSPENDED == Os_TCB[TaskID].taskState)
                {
                    /* PRQA S 3469 ++ */ /* MISRA Rule 4.9 */ /* OS_EVENT_MACRO_004 */
                    OS_ARCH_EXIT_CRITICAL();
                    /* PRQA S 3469 -- */ /* MISRA Rule 4.9 */
                    err = E_OS_STATE;
                }

                if ((StatusType)E_OK == err)
                {
                    *Event = Os_ECB[TaskID].eventSetEvent;
                    /* PRQA S 3469 ++ */ /* MISRA Rule 4.9 */ /* OS_EVENT_MACRO_004 */
                    OS_ARCH_EXIT_CRITICAL();
                    /* PRQA S 3469 -- */ /* MISRA Rule 4.9 */
                }
            }
#else
            if ((StatusType)E_OK == err)
            {
                OS_ARCH_ENTRY_CRITICAL();
                *Event = Os_ECB[TaskID].eventSetEvent;
                OS_ARCH_EXIT_CRITICAL();
            }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetEvent(TaskID, Event), OSServiceId_GetEvent, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetEvent);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The events of the extended task calling ClearEvent are cleared
 *                       according to the event mask <Mask>.
 *                       The system service ClearEvent is restricted to extended tasks which own the event.>
 * Service ID           <0xed>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) ClearEvent(EventMaskType Mask)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_ClearEvent);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        err = E_OS_CALLEVEL;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_CLEAR_EVENT) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
        if (Os_SCB.sysOsLevel != OS_LEVEL_TASK)
        {
            err = E_OS_CALLEVEL;
        }
        else
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */
        {
            if (Os_SCB.sysRunningTaskID >= Os_CfgExtendTaskMax)
            {
                err = E_OS_ACCESS;
            }
            else
            {
                /* Task access EVENT permission check */
                /* PRQA S 3469 ++ */ /* MISRA Rule 4.9*/ /* OS_EVENT_MACRO_004 */
                err = OS_EVENT_PERMISSION_CHECKING(Mask, *Os_TaskEventAccessMask[Os_SCB.sysRunningTaskID]);
                /* PRQA S 3469 -- */ /* MISRA Rule 4.9*/
            }
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == err)
        {
            OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR)
            Os_MonitorEventResponseTime(Os_SCB.sysRunningTaskID, Mask);
#endif
#if (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR)
            Os_MonitorEventResponseRate(Os_SCB.sysRunningTaskID, Mask);
#endif

            Os_ECB[Os_SCB.sysRunningTaskID].eventSetEvent &= (~Mask);
            OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ClearEvent(Mask), OSServiceId_ClearEvent, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_ClearEvent);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The service may be called from an interrupt service routine and
 *                       from the task level, but not from hook routines.
 *                       The events of task <TaskID> are set according to the event
 *                       mask <Mask>. Calling SetEvent causes the task <TaskID> to
 *                       be transferred to the ready state, if it was waiting for at least
 *                       one of the events specified in <Mask>>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task for which one or several events are to be set.>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_SetEvent(TaskType TaskID, EventMaskType Mask)
{
    OS_ARCH_DECLARE_CRITICAL();
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_EventMaskType, OS_VAR) SetEventMask = 0U;
    VAR(Os_EventMaskType, OS_VAR) WaitEventMask = 0U;

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469,1338 ++ */ /* MISRA Rule 17.8,4.9*/ /* OS_EVENT_MACRO_004 */
    TaskID = Os_GetObjLocalId(TaskID);
/* PRQA S 3469,1338 -- */ /* MISRA Rule 17.8,4.9*/
#endif

/* Extended Status */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (TaskID >= Os_CfgExtendTaskMax)
    {
        err = E_OS_ACCESS;
    }
    else if (TASK_STATE_SUSPENDED == Os_TCB[TaskID].taskState)
    {
        err = E_OS_STATE;
    }
    else
    {
        /*nothing to do*/
    }
    if ((StatusType)E_OK == err)
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
    {
        OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
/* Timing protection: Check inter-arrival time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if ((StatusType)E_OK != Os_TmProtTaskFrameChk(TaskID))
        {
            err = E_OS_ID;
        }
        else
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
        {
            /* Standard Status */
            Os_ECB[TaskID].eventSetEvent |= Mask;

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR)
            Os_MonitorEventStartTime(TaskID, Mask);
#endif
#if (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR)
            Os_AddEventResponseNum(TaskID, Mask);
#endif

            SetEventMask = Os_ECB[TaskID].eventSetEvent;
            WaitEventMask = Os_ECB[TaskID].eventWaitEvent;

            if (TASK_STATE_WAITING == Os_TCB[TaskID].taskState)
            {
                if (TRUE == Os_ECB[TaskID].eventIsWaitAllEvents)
                {
                    /* PRQA S 3469 ++ */ /* MISRA Rule 4.9*/ /* OS_EVENT_MACRO_004 */
                    if (E_OK == OS_EVENT_IS_ALL_EVENTS_TRIGGERED(WaitEventMask, SetEventMask))
                    /* PRQA S 3469 -- */ /* MISRA Rule 4.9*/
                    {
                        Os_ECB[TaskID].eventIsWaitAllEvents = FALSE;
#if (TRUE == CFG_TRACE_ENABLE)
                        Os_TraceTaskSwitch(
                            Os_SCB.sysRunningTaskID,
                            TaskID,
                            OS_TRACE_TASK_SWITCH_REASON_SETEVENT_READY,
                            OS_TRACE_TASK_SWITCH_REASON_SETEVENT_ACTIVE);
#endif
                        Os_EventTaskDispatch(TaskID);
                    }
                }
                else
                {
                    if ((SetEventMask & WaitEventMask) != 0u)
                    {
#if (TRUE == CFG_TRACE_ENABLE)
                        Os_TraceTaskSwitch(
                            Os_SCB.sysRunningTaskID,
                            TaskID,
                            OS_TRACE_TASK_SWITCH_REASON_SETEVENT_READY,
                            OS_TRACE_TASK_SWITCH_REASON_SETEVENT_ACTIVE);
#endif
                        Os_EventTaskDispatch(TaskID);
                    }
                }
            }
        }
        OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The state of the calling task is set to waiting, unless
 *                       at least one of the events specified in <Mask> has
 *                       already been set.
 *                       Particularities: This call enforces rescheduling,
 *                       if the wait condition occurs. If rescheduling takes
 *                       place, the internal resource of the task is released
 *                       while the task is in the waiting state.
 *                       This service must only be called from the extended task
 *                       owning the event.
 *                       The system service ClearEvent is restricted to extended
 *                       taskswhich own the event.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_WaitEvent(EventMaskType Mask)
{
    VAR(StatusType, OS_VAR) status = E_OK;
    OS_ARCH_DECLARE_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    Os_ECB[Os_SCB.sysRunningTaskID].eventWaitEvent = Mask;
    if (0u == (Os_ECB[Os_SCB.sysRunningTaskID].eventSetEvent & Mask))
    {
        Os_PostTaskHook(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
        Os_ReleaseInternalResource(Os_SCB.sysRunningTaskID);
#endif

        Os_TCB[Os_SCB.sysRunningTaskID].taskState = TASK_STATE_WAITING;

        Os_ReadyQueueRemove(OS_LEVEL_TASK, Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio);

        Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio = Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskPriority;

/* Timing protection: reset task exe time. OS473. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_EXE);
#endif

        Os_SCB.sysHighPrio = Os_GetHighPrio();
        Os_SCB.sysHighTaskID = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);
        Os_SCB.sysDispatchLocker = 0u;

#if (TRUE == CFG_TRACE_ENABLE)
        Os_TraceTaskSwitch(
            Os_SCB.sysRunningTaskID,
            Os_SCB.sysHighTaskID,
            OS_TRACE_TASK_SWITCH_REASON_WAITEVENT_WAIT,
            OS_TRACE_TASK_SWITCH_REASON_WAITEVENT_ACTIVE);
#endif

        OS_START_DISPATCH();
        Os_Dispatch(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    }

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR)
    Os_MonitorEventEndTime(Os_SCB.sysRunningTaskID, Mask);
#endif
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The state of the calling task is set to waiting, must
 *                       all the events specified in <Mask> has already been set.
 *                       Particularities: This call enforces rescheduling,
 *                       if the wait condition occurs. If rescheduling takes
 *                       place, the internal resource of the task is released
 *                       while the task is in the waiting state.
 *                       This service must only be called from the extended task
 *                       owning the event.
 *                       The system service ClearEvent is restricted to extended
 *                       taskswhich own the event.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Mask[in]       <Mask of the events to be set>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_WaitAllEvents(EventMaskType Mask)
{
    VAR(StatusType, OS_VAR) status = E_OK;
    OS_ARCH_DECLARE_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    Os_ECB[Os_SCB.sysRunningTaskID].eventWaitEvent = Mask;
    Os_ECB[Os_SCB.sysRunningTaskID].eventIsWaitAllEvents = TRUE;

    /* PRQA S 3469 ++ */ /* MISRA Rule 4.9*/ /* OS_EVENT_MACRO_004 */
    if (E_OK != OS_EVENT_IS_ALL_EVENTS_TRIGGERED(Mask, Os_ECB[Os_SCB.sysRunningTaskID].eventSetEvent))
    /* PRQA S 3469 -- */ /* MISRA Rule 4.9*/ /* OS_EVENT_MACRO_004 */
    {
        Os_PostTaskHook(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
        Os_ReleaseInternalResource(Os_SCB.sysRunningTaskID);
#endif

        Os_TCB[Os_SCB.sysRunningTaskID].taskState = TASK_STATE_WAITING;
        Os_ReadyQueueRemove(OS_LEVEL_TASK, Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio);

        Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio = Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskPriority;

/* Timing protection: reset task exe time. OS473. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_EXE);
#endif

        Os_SCB.sysHighPrio = Os_GetHighPrio();
        Os_SCB.sysHighTaskID = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);
        Os_SCB.sysDispatchLocker = 0u;

        OS_START_DISPATCH();
        Os_Dispatch(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */
    }

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR)
    Os_MonitorEventEndTime(Os_SCB.sysRunningTaskID, Mask);
#endif
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_EVENT_MACRO_004 */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_EXTENDED_TASK_MAX > 0 */

/*=======[E N D   O F   F I L E]==============================================*/
