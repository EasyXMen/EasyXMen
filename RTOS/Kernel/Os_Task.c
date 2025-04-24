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
**  FILENAME    :  Os_Task.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :  task manager                                               **
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

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (CFG_TASK_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Init the task control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitTask(void)
{
    VAR(Os_TaskType, OS_VAR) i;
#if ((CFG_SPINLOCK_MAX > 0U) && (CFG_STD_RESOURCE_MAX > 0U))
    VAR(uint16, OS_VAR) j;
#endif

    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_TASK_COMPILER_001 */
    P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    P2CONST(Os_TaskCfgType, AUTOMATIC, OS_VAR) pTaskCfg;

    Os_CoreIdType coreId = Os_SCB.sysCore;
    Os_TaskCfg = Os_TaskCfg_Inf[coreId];
    Os_TaskStack = Os_TaskStack_Inf[coreId];
    Os_TCB = Os_TCB_Inf[coreId];
    Os_SCB.sysTaskMax = Os_CfgTaskMax_Inf[coreId];
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    OS_TASK_IDLE = Os_GetObjLocalId(Os_TASK_IDLE_Inf[coreId]);
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

#if (CFG_STD_RESOURCE_MAX > 0U)
    P2CONST(TaskResourceStackType, AUTOMATIC, OS_VAR) pTaskResStackCfg = &Os_TCBTaskResourceStack_Inf[coreId];
    CONST(uint16, OS_CONST) vResourceCfgStdMax = Os_CfgStdResourceMax_Inf[coreId];
    if (vResourceCfgStdMax > 0U)
    {
        for (i = 0U; i < Os_SCB.sysTaskMax; i++)
        {
            /* PRQA S 0488 ++ */ /* MISRA Rule 18.4 */ /* OS_TASK_PTR_OPERATIONS_002 */
            Os_TCB[i].taskResourceStack = pTaskResStackCfg->TaskResourceStackPtr + (i * vResourceCfgStdMax);
            /* PRQA S 0488 -- */ /* MISRA Rule 18.4 */
        }
    }
#endif

    for (i = 0u; i < Os_SCB.sysTaskMax; i++)
    {
        pTCB = &Os_TCB[i];
        pTaskCfg = &Os_TaskCfg[i];

        pTCB->taskTop = Os_TaskStack[pTaskCfg->osTaskStackId].stackTop;
        pTCB->taskStackBottom = Os_TaskStack[pTaskCfg->osTaskStackId].stackBottom;

#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
        pTCB->taskActCount = 0u;
        pTCB->taskSelfActCount = 0u;
#endif

        pTCB->taskRunPrio = pTaskCfg->osTaskPriority;

/*multi-core for res and spinlock release as LIFO*/
#if ((CFG_SPINLOCK_MAX > 0U) && (CFG_STD_RESOURCE_MAX > 0U))
        pTCB->taskCriticalZoneCount = 0u;
        pTCB->taskCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID;
        for (j = 0u; j < (uint16)CFG_CRITICAL_ZONE_MAX; j++)
        {
            pTCB->taskCriticalZoneStack[j] = OS_OBJECT_INVALID;
            pTCB->taskCriticalZoneType[j] = OBJECT_MAX;
        }
#endif

        if (0U != (pTaskCfg->osTaskAutoStartMode & Os_SCB.sysActiveAppMode))
        {
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
            pTCB->taskActCount = pTCB->taskActCount + 1U;
#endif

            pTCB->taskState = TASK_STATE_START;
            Os_ReadyQueueInsert(i, OS_LEVEL_TASK, pTCB->taskRunPrio);

            if ((Os_SCB.sysHighPrio == OS_PRIORITY_INVALID) || (pTCB->taskRunPrio > Os_SCB.sysHighPrio))
            {
                Os_SCB.sysHighTaskID = i;
                Os_SCB.sysHighPrio = pTCB->taskRunPrio;
            }
        }
        else
        {
            pTCB->taskState = TASK_STATE_SUSPENDED;
        }
/* Init task stack */
#if ((TRUE == CFG_STACK_CHECK) && (CFG_TASK_MAX > 0U))
        Os_FillStack(Os_TaskStack[i]);
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ActivateTask>
 * Service ID           <OSServiceId_ActivateTask>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_ActivateTask(TaskType TaskID)
{
    VAR(StatusType, OS_VAR) status = E_OK;

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8 */ /* OS_TASK_PARAM_MACRO_005 */
    TaskID = Os_GetObjLocalId(TaskID);
/* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8 */
#endif

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    OS_ARCH_ENTRY_CRITICAL();
/* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
/* Timing protection: Check inter-arrival time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if ((StatusType)E_OK != Os_TmProtTaskFrameChk(TaskID))
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

        status = E_OS_ID;
    }
    else
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
    {
/* Basic status */
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        if (Os_TCB[TaskID].taskState != TASK_STATE_SUSPENDED)
#else
        if (Os_TCB[TaskID].taskActCount >= Os_TaskCfg[TaskID].osTaskActivation)
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */
        {
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

            status = E_OS_LIMIT;
        }
    }

    if ((StatusType)E_OK == status)
    {
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
        Os_TCB[TaskID].taskActCount = Os_TCB[TaskID].taskActCount + 1U;
        if (TaskID == Os_SCB.sysRunningTaskID)
        {
            Os_TCB[TaskID].taskSelfActCount = Os_TCB[TaskID].taskSelfActCount + 1U;
        }
#endif

        if (TASK_STATE_SUSPENDED == Os_TCB[TaskID].taskState)
        {
            Os_TCB[TaskID].taskState = TASK_STATE_START;
        }

        Os_ReadyQueueInsert(TaskID, OS_LEVEL_TASK, Os_TaskCfg[TaskID].osTaskPriority);

        if (Os_TaskCfg[TaskID].osTaskPriority > Os_SCB.sysHighPrio)
        {
            Os_SCB.sysHighTaskID = TaskID;
            Os_SCB.sysHighPrio = Os_TaskCfg[TaskID].osTaskPriority;

#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
            if (0U == Os_SCB.sysDispatchLocker)
            {
#if (TRUE == CFG_TRACE_ENABLE)
                Os_TraceTaskActive(TaskID);
#endif
                OS_START_DISPATCH();
                /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
                Os_Dispatch();
                /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            }
#endif
        }
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
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
 * Brief                <This service causes the termination of the calling task.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(StatusType, OS_CODE) Os_TerminateTask(void)
{
    VAR(StatusType, OS_VAR) status = E_OK;
    OS_ARCH_DECLARE_CRITICAL();

#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
    if (Os_SCB.sysOsLevel != OS_LEVEL_TASK)
    {
        status = E_OS_CALLEVEL;
    }
    else
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */
    {
#if (CFG_STD_RESOURCE_MAX > 0U)
        if (Os_TCB[Os_SCB.sysRunningTaskID].taskResCount > 0U)
        {
            status = E_OS_RESOURCE;
        }
        else
#endif /* CFG_STD_RESOURCE_MAX > 0U */
        {
#if (CFG_SPINLOCK_MAX > 0U)
            status = Os_SpinlockSafetyCheck();
#endif
        }
    }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

    if ((StatusType)E_OK == status)
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
        OS_ARCH_ENTRY_CRITICAL();
        Os_PostTaskHook();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
        Os_ReleaseInternalResource(Os_SCB.sysRunningTaskID);
#endif

#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_SCB.sysRunningTCB->taskState = TASK_STATE_SUSPENDED;
#else
        if (Os_SCB.sysRunningTCB->taskSelfActCount > 0U)
        {
            Os_SCB.sysRunningTCB->taskSelfActCount = Os_SCB.sysRunningTCB->taskSelfActCount - 1U;
        }
        if (Os_SCB.sysRunningTCB->taskActCount > 0U)
        {
            Os_SCB.sysRunningTCB->taskActCount = Os_SCB.sysRunningTCB->taskActCount - 1U;
        }

        if (Os_SCB.sysRunningTCB->taskActCount > 0U)
        {
            Os_SCB.sysRunningTCB->taskState = TASK_STATE_START;
        }
        else
        {
            Os_SCB.sysRunningTCB->taskState = TASK_STATE_SUSPENDED;
        }
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */

        Os_ReadyQueueRemove(OS_LEVEL_TASK, Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio);

#if (CFG_STD_RESOURCE_MAX > 0U)
        Os_SCB.sysRunningTCB->taskResCount = 0U;
#endif

        Os_SCB.sysRunningTCB->taskRunPrio = Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskPriority;

#if (CFG_EXTENDED_TASK_MAX > 0U)
        if (Os_SCB.sysRunningTaskID < Os_CfgExtendTaskMax)
        {
            Os_ECB[Os_SCB.sysRunningTaskID].eventSetEvent = 0U;
            Os_ECB[Os_SCB.sysRunningTaskID].eventWaitEvent = 0U;
        }
#endif

        if (Os_SCB.sysHighTaskID == Os_SCB.sysRunningTaskID)
        {
            Os_SCB.sysHighPrio = Os_GetHighPrio();
            Os_SCB.sysHighTaskID = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);
        }

/* Timing protection: reset task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_EXE);
#endif

        Os_SCB.sysDispatchLocker = 0U;
#if (TRUE == CFG_TRACE_ENABLE)
        Os_TraceTaskSwitch(
            Os_SCB.sysRunningTaskID,
            Os_SCB.sysHighTaskID,
            OS_TRACE_TASK_SWITCH_REASON_TERMINATE,
            OS_TRACE_TASK_SWITCH_REASON_TERMINATE_ACTIVE);
#endif

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE)
        Os_TaskRecordTotalTick(Os_SCB.sysRunningTaskID);
#endif /* TRUE == CFG_TASK_RESPONSE_TIME_ENABLE */

        OS_START_DISPATCH();
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
        Os_Dispatch();
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service causes the termination of the calling task. After
 *                       termination of the calling task a succeeding task <TaskID> is
 *                       activated. Using this service, it ensures that the succeeding
 *                       task starts to run at the earliest after the calling task has been
 *                       terminated.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(StatusType, OS_CODE) Os_ChainTask(TaskType TaskID)
{
    VAR(StatusType, OS_VAR) status = E_OK;
    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8 */ /* OS_TASK_PARAM_MACRO_005 */
    TaskID = Os_GetObjLocalId(TaskID);
/* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8 */
#endif

    /* Basic status */
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    OS_ARCH_ENTRY_CRITICAL();
/* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
    if ((Os_TCB[TaskID].taskActCount >= Os_TaskCfg[TaskID].osTaskActivation) && (TaskID != Os_SCB.sysRunningTaskID))
#else
    if ((Os_TCB[TaskID].taskState != TASK_STATE_SUSPENDED) && (TaskID != Os_SCB.sysRunningTaskID))
#endif /* OS_BCC2 == CFG_CC || OS_ECC2 == CFG_CC */
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        status = E_OS_LIMIT;
    }

    if ((StatusType)E_OK == status)
    {
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
        Os_PostTaskHook();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
        Os_ReleaseInternalResource(Os_SCB.sysRunningTaskID);
#endif

#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        Os_SCB.sysRunningTCB->taskState = TASK_STATE_SUSPENDED;
#else
        if (Os_SCB.sysRunningTCB->taskSelfActCount > 0U)
        {
            Os_SCB.sysRunningTCB->taskSelfActCount--;
        }
        if (Os_SCB.sysRunningTCB->taskActCount > 0U)
        {
            Os_SCB.sysRunningTCB->taskActCount--;
        }

        if (Os_SCB.sysRunningTCB->taskActCount > 0U)
        {
            Os_SCB.sysRunningTCB->taskState = TASK_STATE_START;
        }
        else
        {
            Os_SCB.sysRunningTCB->taskState = TASK_STATE_SUSPENDED;
        }
#endif /* OS_BCC1 == CFG_CC || OS_ECC1 == CFG_CC */

        Os_ReadyQueueRemove(OS_LEVEL_TASK, Os_TCB[Os_SCB.sysRunningTaskID].taskRunPrio);

#if (CFG_STD_RESOURCE_MAX > 0U)
        Os_SCB.sysRunningTCB->taskResCount = 0u;
#endif

        Os_SCB.sysRunningTCB->taskRunPrio = Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskPriority;

#if (CFG_EXTENDED_TASK_MAX > 0)
        if (Os_SCB.sysRunningTaskID < Os_CfgExtendTaskMax)
        {
            Os_ECB[Os_SCB.sysRunningTaskID].eventSetEvent = 0u;
            Os_ECB[Os_SCB.sysRunningTaskID].eventWaitEvent = 0u;
        }
#endif

/* Timing protection: reset task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_EXE);
#endif

/* Timing protection: Check inter-arrival time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if ((StatusType)E_OK != Os_TmProtTaskFrameChk(TaskID))
        {
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

            status = E_OS_ID;
        }
        else
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
        {
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
            Os_TCB[TaskID].taskActCount = Os_TCB[TaskID].taskActCount + 1U;

            if (TASK_STATE_SUSPENDED == Os_TCB[TaskID].taskState)
            {
                Os_TCB[TaskID].taskState = TASK_STATE_START;
            }
#else
            Os_TCB[TaskID].taskState = TASK_STATE_START;
#endif /* OS_BCC2 == CFG_CC || OS_ECC2 == CFG_CC */

            Os_ReadyQueueInsert(TaskID, OS_LEVEL_TASK, Os_TaskCfg[TaskID].osTaskPriority);

            if (Os_SCB.sysHighTaskID == Os_SCB.sysRunningTaskID)
            {
                Os_SCB.sysHighPrio = Os_GetHighPrio();
                Os_SCB.sysHighTaskID = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);
            }

            Os_SCB.sysDispatchLocker = 0u;

#if (TRUE == CFG_TRACE_ENABLE)
            Os_TraceTaskSwitch(
                Os_SCB.sysRunningTaskID,
                Os_SCB.sysHighTaskID,
                OS_TRACE_TASK_SWITCH_REASON_CHAIN_READY,
                OS_TRACE_TASK_SWITCH_REASON_CHAIN_ACTIVE);
#endif

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE)
            Os_TaskRecordTotalTick(Os_SCB.sysRunningTaskID);
#endif /* TRUE == CFG_TASK_RESPONSE_TIME_ENABLE */

            OS_START_DISPATCH();
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
            Os_Dispatch();

            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Returns the state of a task (running, ready, waiting, suspended)
 *                       at the time of calling GetTaskState.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-State[out]     <Reference to the state of the task <TaskID>>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_GetTaskState(TaskType TaskID, TaskStateRefType State)
{
    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA Dir 4.9, 17.8 */ /* OS_TASK_PARAM_MACRO_005 */
    TaskID = Os_GetObjLocalId(TaskID);
/* PRQA S 3469, 1338 -- */ /* MISRA Dir 4.9, 17.8 */
#endif

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    if (TASK_STATE_START == Os_TCB[TaskID].taskState)
    {
        *State = TASK_STATE_READY;
    }
    else
    {
        *State = Os_TCB[TaskID].taskState;
    }
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Returns the state of a task (running, ready, waiting, suspended)
 *                       at the time of calling GetTaskState.>
 * Service ID           <0xe3>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-State[out]     <Reference to the state of the task <TaskID>>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) GetTaskState(TaskType TaskID, TaskStateRefType State)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetTaskState);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == State)
    {
        status = E_OS_PARAM_POINTER;
    }
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_TASK_COMPILER_001 */
    else if (CHECK_ID_INVALID(TaskID, Os_CfgTaskMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        status = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_TASK_STATE) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_TASK_TYPE_CAST_003 */
    else if (Os_AddressWritable((uint32)State) != TRUE)
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    {
        status = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_TASK, TaskID) != TRUE)
    {
        status = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
        Os_CoreIdType coreId = Os_GetObjCoreId(TaskID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_GetTaskState,
                .srvPara0 = (uint32)TaskID,
                /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_TASK_TYPE_CAST_003 */
                .srvPara1 = (uint32)State,
                /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
                .srvPara2 = (uint32)NULL_PARA,
            };
            status = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            Os_GetTaskState(TaskID, State);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetTaskState(TaskID, State), OSServiceId_GetTaskState, status);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetTaskState);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The task <TaskID> is transferred from the suspended state into
 *                       the ready state>
 * Service ID           <0xde>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) ActivateTask(TaskType TaskID)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_ActivateTask);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_TASK_COMPILER_001 */
    if (CHECK_ID_INVALID(TaskID, Os_CfgTaskMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        status = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_ACTIVATE_TASK) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
    {
        status = E_OS_ACCESS;
    }
    else if (Os_CheckObjAcs(OBJECT_TASK, TaskID) != TRUE)
    {
        status = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
        Os_CoreIdType coreId = Os_GetObjCoreId(TaskID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_ActivateTask,
                .srvPara0 = (uint32)TaskID,
                .srvPara1 = (uint32)NULL_PARA,
                .srvPara2 = (uint32)NULL_PARA,
            };
            status = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            status = Os_ActivateTask(TaskID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ActivateTask(TaskID), OSServiceId_ActivateTask, status);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_ActivateTask);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Asynchronous version of the ActivateTask() function.>
 * Service ID           <0xde>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) ActivateTaskAsyn(TaskType TaskID)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_ActivateTaskAsyn);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_TASK_COMPILER_001 */
    if (CHECK_ID_INVALID(TaskID, Os_CfgTaskMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        status = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_ACTIVATE_TASK) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else if (APPLICATION_ACCESSIBLE != Os_AppCB[Os_SCB.sysRunningAppID].appState)
    {
        status = E_OS_ACCESS;
    }
    else if (Os_CheckObjAcs(OBJECT_TASK, TaskID) != TRUE)
    {
        status = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
        Os_CoreIdType coreId = Os_GetObjCoreId(TaskID);
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_ASYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_ActivateTask,
                .srvPara0 = (uint32)TaskID,
                .srvPara1 = (uint32)NULL_PARA,
                .srvPara2 = (uint32)NULL_PARA,
            };
            status = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            status = Os_ActivateTask(TaskID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ActivateTaskAsyn(TaskID), OSServiceId_ActivateTaskAsyn, status);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_ActivateTaskAsyn);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service causes the termination of the calling task.>
 * Service ID           <0xdf>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) TerminateTask(void)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_TerminateTask);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        status = E_OS_CALLEVEL;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_TERMINATE_TASK) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        status = Os_TerminateTask();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_TerminateTask(), OSServiceId_TerminateTask, status);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_TerminateTask);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <TaskID Reference to the task which is currently running
 *                       Description: GetTaskID returns the information about the
 *                       TaskID of the task which is currently running.>
 * Service ID           <0xe2>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-Name[out]      <TaskID>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) GetTaskID(TaskRefType TaskID)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == TaskID)
    {
        status = E_OS_PARAM_POINTER;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_TASK_ID) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_TASK_TYPE_CAST_003 */
    else if (Os_AddressWritable((uint32)TaskID) != TRUE)
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    {
        status = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
        *TaskID = Os_SCB.sysRunningTaskID | (Os_SCB.sysCore << 12u);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetTaskID(TaskID), OSServiceId_GetTaskID, status);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetTaskID);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <If a higher-priority task is ready, the internal
 *                       resource of the task is released, the current task
 *                       is put into the ready state, its context is saved and
 *                       the higher-priority task is executed.
 *                       Otherwise the calling task is continued.>
 * Service ID           <0xe1>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Schedule(void)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_Schedule);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        status = E_OS_CALLEVEL;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_SCHEDULE) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
        if (Os_SCB.sysOsLevel != OS_LEVEL_TASK)
        {
            status = E_OS_CALLEVEL;
        }
        else
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */
        {
#if (CFG_STD_RESOURCE_MAX > 0U)
            if (Os_TCB[Os_SCB.sysRunningTaskID].taskResCount > (Os_ResourceType)0)
            {
                status = E_OS_RESOURCE;
            }
            else
#endif /* CFG_STD_RESOURCE_MAX > 0U */
            {
#if (CFG_SPINLOCK_MAX > 0U)
                status = Os_SpinlockSafetyCheck();
#endif
            }
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == status)
        {
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
            OS_ARCH_ENTRY_CRITICAL();
/* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

/* Basic status */
#if (CFG_INTERNAL_RESOURCE_MAX > 0)
            if (Os_InterResCeiling[Os_SCB.sysRunningTaskID] != (Os_PriorityType)0)
            {
                Os_ReleaseInternalResource(Os_SCB.sysRunningTaskID);
                Os_SCB.sysRunningTCB->taskRunPrio = Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskPriority;
            }
#endif

            Os_SCB.sysHighPrio = Os_GetHighPrio();
            Os_SCB.sysHighTaskID = Os_ReadyQueueGetFirst(Os_SCB.sysHighPrio);

            if (Os_SCB.sysHighTaskID != Os_SCB.sysRunningTaskID)
            {
                Os_SCB.sysDispatchLocker = 0u;
#if (TRUE == CFG_TRACE_ENABLE)
                Os_TraceTaskSwitch(
                    Os_SCB.sysRunningTaskID,
                    Os_SCB.sysHighTaskID,
                    OS_TRACE_TASK_SWITCH_REASON_SCHEDULE_READY,
                    OS_TRACE_TASK_SWITCH_REASON_SCHEDULE_ACTIVE);
#endif

                OS_START_DISPATCH();
                /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
                Os_Dispatch();
                /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            }

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
            Os_GetInternalResource();
#endif

            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_Schedule(), OSServiceId_Schedule, status);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_Schedule);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <This service causes the termination of the calling task. After
 *                       termination of the calling task a succeeding task <TaskID> is
 *                       activated. Using this service, it ensures that the succeeding
 *                       task starts to run at the earliest after the calling task has been
 *                       terminated.>
 * Service ID           <0xe0>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-TaskID[in]     <Reference to the task>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) ChainTask(TaskType TaskID)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) status = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_ChainTask);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        status = E_OS_CALLEVEL;
    }
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_TASK_COMPILER_001 */
    else if (CHECK_ID_INVALID(TaskID, Os_CfgTaskMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        status = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_CHAIN_TASK) != TRUE)
    {
        status = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        status = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_TASK, TaskID) != TRUE)
    {
        status = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if (CFG_STD_RESOURCE_MAX > 0U)
        if (Os_TCB[Os_SCB.sysRunningTaskID].taskResCount > (Os_ResourceType)0)
        {
            status = E_OS_RESOURCE;
        }
        else
#endif /* CFG_STD_RESOURCE_MAX > 0U */
        {
#if (CFG_SPINLOCK_MAX > 0U)
            status = Os_SpinlockSafetyCheck();
#endif
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

        if ((StatusType)E_OK == status)
        {
#if (OS_AUTOSAR_CORES > 1)
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
            Os_CoreIdType coreId = Os_GetObjCoreId(TaskID);
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            if (coreId != Os_SCB.sysCore)
            {
                RpcInputType rpcData = {
                    .sync = RPC_SYNC,
                    .remoteCoreId = coreId,
                    .serviceId = OSServiceId_ChainTask,
                    .srvPara0 = (uint32)TaskID,
                    .srvPara1 = (uint32)NULL_PARA,
                    .srvPara2 = (uint32)NULL_PARA,
                };
                status = Os_RpcCallService(&rpcData);
                if (E_OK == status)
                {
                    status = Os_TerminateTask();
                }
            }
            else
#endif /* OS_AUTOSAR_CORES > 1 */
            {
                status = Os_ChainTask(TaskID);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ChainTask(TaskID), OSServiceId_ChainTask, status);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_ChainTask);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_TASK_MACRO_TO_FUNCTION_004 */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* CFG_TASK_MAX > 0U */

/*=======[E N D   O F   F I L E]==============================================*/
