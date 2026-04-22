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
 **  @file               : Os_Task.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Task API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Task.h"
#include "Os_ReadyQue.h"
#include "Os_Resource.h"
#include "Os_Appl.h"
#include "Os_Spinlock.h"
#include "Os_Tprot.h"
#include "Os_Sprot.h"
#include "Os_Rpc.h"
#include "Os_Event.h"
#include "Os_StackMonitor.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Extend.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"
#include "Os_Monitor.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (CFG_TASK_MAX > 0U)
#if (OS_AUTOSAR_CORES > 1)
/**
 * @brief           RPC action handler for activating a task
 * @param[in]       inPara: Parameter array containing the task ID to activate
 * @return          StatusType
 * @retval          E_OK: Task activated successfully
 * @retval          E_OS_LIMIT: Task activation limit reached
 * @retval          E_OS_ID: Task ID is invalid under timing protection
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_ActivateTask(uint32 *inPara);
#endif

/**
 * @brief           Internal implementation for terminating a task
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       runningTaskID: ID of the task to be terminated
 * @param[in]       runningTCB: Pointer to the Task Control Block of the task
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TerminateTaskInternal(Os_SCBType *pScb, TaskType runningTaskID, Os_TCBType *runningTCB);

/**
 * @brief           Terminates the specified task
 * @param[in]       runningTaskID: ID of the task to be terminated
 * @return          StatusType
 * @retval          E_OK: Task terminated successfully
 * @retval          E_OS_CALLEVEL: Called from wrong context
 * @retval          E_OS_RESOURCE: Task still occupies resources
 * @retval          E_OS_SPINLOCK: Task still holds spinlocks
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TerminateTask(TaskType runningTaskID);

/**
 * @brief           Terminates the calling task and activates another specified task
 * @param[in]       taskId: ID of the task to be activated after termination
 * @return          StatusType
 * @retval          E_OK: Task chain completed successfully
 * @retval          E_OS_LIMIT: Task activation limit reached for the specified task
 * @retval          E_OS_ID: Task ID is invalid under timing protection
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_ChainTask(TaskType taskId);

/**
 * @brief           Retrieves the current state of a specified task
 * @param[in]       taskId: ID of the task to get state for
 * @param[out]      State: Pointer to store the task state
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetTaskState(TaskType taskId, TaskStateRefType State);

/**
 * @brief           Performs scheduling by checking for higher priority tasks
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_Schedule(Os_SCBType *pScb);

/**
 * @brief           Init the task control block
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       taskId: ID of the task to get state for
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InitTaskTCB(Os_SCBType *pScb, TaskType taskId);
#endif

/* ========================================== external function definitions ========================================= */
#if (CFG_TASK_MAX > 0U)

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the task control block
 */
OS_LOCAL void Os_InitTaskTCB(Os_SCBType *pScb, TaskType taskId)
{
    Os_TCBType *pTCB = Os_TCB[taskId];
    const Os_TaskCfgType *pTaskCfg = &Os_TaskCfg[taskId];

    pTCB->TaskStackTop = pTaskCfg->TaskStack.StackTop;
    pTCB->TaskStackBottom = pTaskCfg->TaskStack.StackBottom;

#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
    pTCB->TaskActCount = 0u;
#endif

    pTCB->TaskRunPrio = pTaskCfg->TaskPriority;
#if (CFG_OSAPPLICATION_MAX > 0U)
    pTCB->CallBackAppID = INVALID_OSAPPLICATION;
#endif

/*multi-core for res and spinlock release as LIFO*/
#if ((CFG_SPINLOCK_MAX > 0U) && (CFG_STD_RESOURCE_MAX > 0U))
    pTCB->TaskCriticalZoneCount = 0u;
    pTCB->TaskCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID; /* PRQA S 4424, 4342 */ /* VL_Os_4424, VL_Os_4342 */
    for (uint16 i = 0u; i < (uint16)CFG_CRITICAL_ZONE_MAX; i++)
    {
        pTCB->TaskCriticalZoneStack[i] = OS_OBJECT_INVALID; /* PRQA S 4424 */ /* VL_Os_4424 */
        pTCB->TaskCriticalZoneType[i] = OS_OBJECT_MAX;
    }
#endif

    if (0U != (pTaskCfg->TaskAutoStartMode & pScb->SysActiveAppMode))
    {
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
        pTCB->TaskActCount = pTCB->TaskActCount + 1U;
#endif

        pTCB->TaskState = OS_TASK_STATE_START;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 4523, 3762, 1277, 2985 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277, VL_Os_2985 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Activate, taskId);
        /* PRQA S 4543, 4523, 3762, 1277, 2985 -- */
        /* PRQA S 1821, 4532, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
        Os_ReadyQueueInsert(pScb->QueueMg, taskId, OS_LEVEL_TASK, pTCB->TaskRunPrio);

        if ((pScb->SysHighPrio == OS_PRIORITY_INVALID) || (pTCB->TaskRunPrio > pScb->SysHighPrio))
        {
            pScb->SysHighTaskId = taskId;
            pScb->SysHighPrio = pTCB->TaskRunPrio;
        }
    }
    else
    {
        pTCB->TaskState = OS_TASK_STATE_SUSPENDED;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 4523, 3762, 1277++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Terminate, taskId);
        /* PRQA S 4543, 4523, 3762, 1277 -- */
        /* PRQA S 1821, 4532, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
    }

/* Init task schedule count */
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
    pTCB->taskScheduleCount = 0u;
#endif

/* Init task stack */
#if ((TRUE == CFG_STACK_CHECK) && (CFG_TASK_MAX > 0U))
    Os_FillStack(&(pTaskCfg->TaskStack));
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Initialize control blocks for all tasks of the current core
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitTask(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_SCBType *pScb = Os_GetSystemContext(coreId);

    pScb->TaskIdle = (uint16)Os_TASK_IDLE_Inf[coreId];
    Os_TaskType idStartRange = Os_TaskIdRange[coreId].AllTask.Start;
    Os_TaskType idEndRange = Os_TaskIdRange[coreId].AllTask.End;

    pScb->SysTaskMax = (uint16)idEndRange - (uint16)idStartRange;

    if (idStartRange > idEndRange)
    {
        Os_Panic();
    }

#if (CFG_STD_RESOURCE_MAX > 0U)
    uint32 resMax = (uint32)Os_CfgStdResourceMax_Inf[coreId];
    if (resMax > 0U)
    {
        for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
        {
            Os_TCB[i]->TaskResourceStack = Os_TCBTaskResourceStack[i];
            Os_TCB[i]->TaskResCount = 0u;
        }
    }
#endif

    for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
    {
        Os_InitTaskTCB(pScb, (Os_TaskType)i); /* PRQA S 4342 */ /* VL_Os_4342 */
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:ActivateTask
 */
/* PRQA S 1505, 3006 ++ */ /* VL_Os_1505, VL_Os_3006 */
StatusType Os_ActivateTask(TaskType taskId)
/* PRQA S 1505, 3006 -- */
{
    StatusType status = E_OK;

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
/* Timing protection: Check inter-arrival time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if ((StatusType)E_OK != Os_TmProtTaskFrameChk(taskId)) /* PRQA S 1520 */ /* VL_Os_1520 */
    {
        OS_HAL_EXIT_CRITICAL();

        status = E_OS_PROTECTION_ARRIVAL;
    }
    else
#endif
    {
/* Basic status */
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
        if (Os_TCB[taskId]->TaskState != OS_TASK_STATE_SUSPENDED)
#else
        if (Os_TCB[taskId]->TaskActCount >= Os_TaskCfg[taskId].TaskActivation)
#endif
        {
            OS_HAL_EXIT_CRITICAL();

            status = E_OS_LIMIT;
        }
    }

    if ((StatusType)E_OK == status)
    {
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
        Os_TCB[taskId]->TaskActCount = Os_TCB[taskId]->TaskActCount + 1U;
#endif
        Os_SCBType *pScb = OS_TASK_GET_SCB(taskId);
        if (OS_TASK_STATE_SUSPENDED == Os_TCB[taskId]->TaskState)
        {
            Os_TCB[taskId]->TaskState = OS_TASK_STATE_START;
            /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
            /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
            /* PRQA S 4543, 4523, 3762, 1277, 2985 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277, VL_Os_2985 */
            ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Activate, taskId);
            /* PRQA S 4543, 4523, 3762, 1277, 2985 -- */
            /* PRQA S 1821, 4532, 4544, 4542 -- */
            /* PRQA S 3138, 3141 -- */
        }

        Os_ReadyQueueInsert(pScb->QueueMg, taskId, OS_LEVEL_TASK, Os_TaskCfg[taskId].TaskPriority);

        if (Os_TaskCfg[taskId].TaskPriority > pScb->SysHighPrio)
        {
            pScb->SysHighTaskId = taskId;
            pScb->SysHighPrio = Os_TaskCfg[taskId].TaskPriority;

#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
            if (0U == pScb->SysDispatchLocker)
            {
                Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
            }
#endif
        }
        OS_HAL_EXIT_CRITICAL();
    }

    return status;
}
#if (OS_AUTOSAR_CORES > 1)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL StatusType Os_RpcAction_ActivateTask(uint32 *inPara)
/* PRQA S 3673 -- */
{
    return Os_ActivateTask((TaskType)inPara[0]); /* PRQA S 4342 */ /* VL_Os_4342 */
}

/* PRQA S 1505 ++ */ /* VL_Os_1505 */
StatusType Os_RpcCall_ActivateTask(
    Os_CoreIdType ownerCore,
    Os_RpcSyncType syncType,
    TaskType taskId)
/* PRQA S 1505 -- */
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = syncType,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_ActivateTask, /* PRQA S 0674 */ /* VL_Os_0674 */
        .InPara[0] = (uint32)taskId, /* PRQA S 0691 */ /* VL_Os_0691 */
    }; /* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service causes the termination of the calling task.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_TerminateTaskInternal(
    Os_SCBType *pScb,
    TaskType runningTaskID,
    Os_TCBType *runningTCB)
/* PRQA S 3673 -- */
{
    Os_PostTaskHook(pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    Os_ReleaseInternalResource(pScb, runningTaskID);
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Terminate, runningTaskID);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
    runningTCB->TaskState = OS_TASK_STATE_SUSPENDED;
#else
    if (runningTCB->TaskActCount > 0U)
    {
        runningTCB->TaskActCount--;
    }

    if (runningTCB->TaskActCount > 0U)
    {
        runningTCB->TaskState = OS_TASK_STATE_START;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 4523, 3762, 1277, 2985++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277, VL_Os_2985 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Activate, runningTaskID);
        /* PRQA S 4543, 4523, 3762, 1277, 2985 -- */
        /* PRQA S 1821, 4532, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
    }
    else
    {
        runningTCB->TaskState = OS_TASK_STATE_SUSPENDED;
    }
#endif

    Os_ReadyQueueRemove(pScb->QueueMg, Os_TCB[runningTaskID]->TaskRunPrio);

#if (CFG_STD_RESOURCE_MAX > 0U)
    runningTCB->TaskResCount = 0U;
#endif

    runningTCB->TaskRunPrio = Os_TaskCfg[runningTaskID].TaskPriority;

#if (CFG_EXTENDED_TASK_MAX > 0U)
    if (Os_CheckExternalTaskId(runningTaskID, pScb->SysCore))
    {
        Os_ClearECB(runningTaskID);
    }
#endif

/* Timing protection: reset task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtTaskEnd(pScb->SysCore, runningTaskID, TP_EXE);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service causes the termination of the calling task.
 */
OS_LOCAL void Os_TerminateTask(TaskType runningTaskID) /* PRQA S 3408, 3006 */ /* VL_Os_3408 *//* VL_Os_3006*/
{
    Os_SCBType *pScb = OS_TASK_GET_SCB(runningTaskID);

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    Os_TerminateTaskInternal(pScb, runningTaskID, pScb->SysRunningTCB);

    if (pScb->SysHighTaskId == runningTaskID)
    {
        Os_UpdateHighPrioTask(pScb);
    }

    pScb->SysDispatchLocker = 0U;

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_TaskRecordTotalTick(runningTaskID);
#endif                            /* TRUE == CFG_TASK_RESPONSE_TIME_ENABLE */

    Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
    OS_HAL_EXIT_CRITICAL();

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service causes the termination of the calling task. After
 *                       termination of the calling task a succeeding task <taskId
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Os_STCAL */
OS_LOCAL StatusType Os_ChainTask(TaskType taskId) /* PRQA S 3006*/ /* VL_Os_3006*/
/* PRQA S 6070 -- */
{
    StatusType status = E_OK;
    OS_HAL_DECLARE_CRITICAL();

    Os_SCBType *pScb = OS_TASK_GET_SCB(taskId);
    Os_TaskType SysRunningTaskId = pScb->SysRunningTaskId;

    /* Basic status */
    OS_HAL_ENTRY_CRITICAL();
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
    if ((Os_TCB[taskId]->TaskActCount >= Os_TaskCfg[taskId].TaskActivation) && (taskId != SysRunningTaskId))
#else
    if ((Os_TCB[taskId]->TaskState != OS_TASK_STATE_SUSPENDED) &&
        (taskId != SysRunningTaskId))
#endif
    {
        OS_HAL_EXIT_CRITICAL();
        status = E_OS_LIMIT;
    }

    if ((StatusType)E_OK == status)
    {
        Os_TerminateTaskInternal(pScb, SysRunningTaskId, pScb->SysRunningTCB);

/* Timing protection: Check inter-arrival time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if ((StatusType)E_OK != Os_TmProtTaskFrameChk(taskId))
        {
            OS_HAL_EXIT_CRITICAL();

            status = E_OS_PROTECTION_ARRIVAL;
        }
        else
#endif
        {
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
            Os_TCB[taskId]->TaskActCount = Os_TCB[taskId]->TaskActCount + 1U;

            if (OS_TASK_STATE_SUSPENDED == Os_TCB[taskId]->TaskState)
            {
                Os_TCB[taskId]->TaskState = OS_TASK_STATE_START;
                /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
                /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
                /* PRQA S 4543, 4523, 3762, 1277, 2985 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277, VL_Os_2985 */
                ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Activate, taskId);
                /* PRQA S 4543, 4523, 3762, 1277, 2985 -- */
                /* PRQA S 1821, 4532, 4544, 4542 -- */
                /* PRQA S 3138, 3141 -- */
            }
#else
            Os_TCB[taskId]->TaskState = OS_TASK_STATE_START;
            /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
            /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
            /* PRQA S 4543, 4523, 3762, 1277, 2985 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277, VL_Os_2985 */
            ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Activate, taskId);
            /* PRQA S 4543, 4523, 3762, 1277, 2985 -- */
            /* PRQA S 1821, 4532, 4544, 4542 -- */
            /* PRQA S 3138, 3141 -- */
#endif

            Os_ReadyQueueInsert(pScb->QueueMg, taskId, OS_LEVEL_TASK, Os_TaskCfg[taskId].TaskPriority);

            if (pScb->SysHighTaskId == SysRunningTaskId)
            {
                Os_UpdateHighPrioTask(pScb);
            }

            pScb->SysDispatchLocker = 0u;

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
            Os_TaskRecordTotalTick(SysRunningTaskId);
#endif

            Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/

            OS_HAL_EXIT_CRITICAL();
        }
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Returns the state of a task (running, ready, waiting, suspended)
 *                       at the time of calling GetTaskState.
 */
OS_LOCAL void Os_GetTaskState(TaskType taskId, TaskStateRefType State)
{
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    if (OS_TASK_STATE_START == Os_TCB[taskId]->TaskState)
    {
        *State = OS_TASK_STATE_READY;
    }
    else
    {
        *State = Os_TCB[taskId]->TaskState;
    }
    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Returns the state of a task (running, ready, waiting, suspended)
 * at the time of calling GetTaskState.
 */
/* PRQA S 6070, 3006, 1503, 3408, 1512 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
StatusType GetTaskState(TaskType TaskID, TaskStateRefType State)
/* PRQA S 6070, 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetTaskState);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetTaskState_Start, TaskID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_TASK_STATE,
        .ObjectType = OS_OBJECT_TASK,
        .ObjectID = (Os_AppObjectId)TaskID,
        .Address = (uint32)State, /* PRQA S 0306 */ /* VL_Os_0306 */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)TaskID, (uint8)OS_OBJECT_TASK) != TRUE)
    {
        status = E_OS_ID;
    }
    else
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 3326, 2004 */ /* VL_Os_3326, VL_Os_2004 */
#endif
    {
        Os_GetTaskState(TaskID, State);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_GetTaskState(TaskID, State), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_GetTaskState,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1258, 4342, 2998, 2996 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1258, VL_Os_4342, VL_Os_2998, VL_Os_2996 */
    OSRtiExitApi(pScb, OSApiId_GetTaskState);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetTaskState_Return, (status != E_OK ) ? (TaskStateType)0U : *State);
    UNUSED_PARAMETER(pScb);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/  
    /* PRQA S 3432, 4544, 1258, 4342, 2998, 2996 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The task <TaskID
 */
/* PRQA S 6070, 3006, 1532, 3408, 1512 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_OneFunRef, VL_Os_3408, VL_Os_1512 */
StatusType ActivateTask(TaskType TaskID)
/* PRQA S 6070, 3006, 1532, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_ActivateTask);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ActivateTask_Start, TaskID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_ACTIVATE_TASK,
        .ObjectType = OS_OBJECT_TASK,
        .ObjectID = (Os_AppObjectId)TaskID,
        .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)TaskID, (uint8)OS_OBJECT_TASK) != TRUE)
    {
        status = E_OS_ID;
    }
    else
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (OS_APPLICATION_ACCESSIBLE != Os_GetAppStateInternal(pScb->SysRunningAppId))
    {
        status = E_OS_ACCESS;
    }
    else if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 3326, 2004 */ /* VL_Os_3326, VL_Os_2004 */
#endif
    {
#if (OS_AUTOSAR_CORES > 1)
        Os_CoreIdType ownerCore = OS_TASK_GET_COREID(TaskID);
        if (ownerCore != Os_GetCoreIdLocal())
        {
            status = Os_RpcCall_ActivateTask(ownerCore, OS_RPC_SYNC, TaskID);
        }
        else
#endif
        {
            status = Os_ActivateTask(TaskID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ActivateTask(TaskID),
                          OSServiceId_ActivateTask,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ActivateTask);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ActivateTask_Return, status);
    UNUSED_PARAMETER(pScb);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Asynchronous version of the ActivateTask() function.
 */
/* PRQA S 6070, 3006, 1503, 3408, 1512 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
void ActivateTaskAsyn(TaskType TaskID) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
/* PRQA S 6070, 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_ActivateTaskAsyn);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ActivateTaskAsyn_Start, TaskID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_ACTIVATE_TASK_ASYN,
        .ObjectType = OS_OBJECT_TASK,
        .ObjectID = (Os_AppObjectId)TaskID,
        .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)TaskID, (uint8)OS_OBJECT_TASK) != TRUE)
    {
        status = E_OS_ID;
    }
    else
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (OS_APPLICATION_ACCESSIBLE != Os_GetAppStateInternal(pScb->SysRunningAppId))
    {
        status = E_OS_ACCESS;
    }
    else if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 3326, 2004 */ /* VL_Os_3326, VL_Os_2004 */
#endif
    {
#if (OS_AUTOSAR_CORES > 1)
        Os_CoreIdType ownerCore = OS_TASK_GET_COREID(TaskID);
        if (ownerCore != Os_GetCoreIdLocal())
        {
            status = Os_RpcCall_ActivateTask(ownerCore, OS_RPC_ASYNC, TaskID);
        }
        else
#endif
        {
            status = Os_ActivateTask(TaskID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ActivateTaskAsyn(TaskID),
                          OSServiceId_ActivateTaskAsyn,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_ActivateTaskAsyn);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ActivateTaskAsyn_Return, 0);
    UNUSED_PARAMETER(pScb);
    UNUSED_PARAMETER(status);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service causes the termination of the calling task.
 */
/* PRQA S 6070, 3006, 1503, 3408, 1512 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
StatusType TerminateTask(void)
/* PRQA S 6070, 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_TerminateTask);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_TerminateTask_Start, pScb->SysRunningTaskId);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_TERMINATE_TASK,
        .ObjectType = OS_OBJECT_TASK,
        .ObjectID = (Os_AppObjectId)pScb->SysRunningTaskId,
        .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        status = E_OS_CALLEVEL;
    }
    else
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
    if (OS_LEVEL_TASK != pScb->SysOsLevel)
    {
        status = E_OS_CALLEVEL;
    }
    else
#endif
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 3326, 2004 */ /* VL_Os_3326, VL_Os_2004 */
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if (CFG_STD_RESOURCE_MAX > 0U)
        if (Os_TCB[pScb->SysRunningTaskId]->TaskResCount > 0U)
        {
            status = E_OS_RESOURCE;
        }
        else
#endif
        {
#if (CFG_SPINLOCK_MAX > 0U)
            status = Os_SpinlockSafetyCheck(pScb->SysRunningTaskId);
#endif
        }
#endif
        if ((StatusType)E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
        {
            Os_TerminateTask(pScb->SysRunningTaskId);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        /* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_TerminateTask(), OSServiceId_TerminateTask, status, pScb); /* PRQA S 2880 */ /* VL_Os_2880 */
        /* PRQA S 3138 -- */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_TerminateTask);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_TerminateTask_Return, status);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * TaskID Reference to the task which is currently running
 *                       Description: GetTaskID returns the information about the
 *                       TaskID of the task which is currently running.
 */
/* PRQA S 3006, 1503, 3408, 1512 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
StatusType GetTaskID(TaskRefType TaskID)
/* PRQA S 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetTaskID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetTaskID_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_TASK_ID,
        .ObjectType = OS_OBJECT_TASK,
        .ObjectID = (Os_AppObjectId)0U,
        .Address = (uint32)TaskID, /* PRQA S 0306 */ /* VL_Os_0306 */
    };
    if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 3326 */ /* VL_Os_3326 */
#endif
    {
        *TaskID = pScb->SysRunningTaskId;
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_GetTaskID(TaskID), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_GetTaskID,
                          status,
                          pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1258, 2996, 2998 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1258, VL_Os_2996, VL_Os_2998 */
    OSRtiExitApi(pScb, OSApiId_GetTaskID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetTaskID_Return, (status != E_OK ) ? INVALID_TASK :*TaskID);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1258, 2996, 2998 -- */
    /* PRQA S 3138, 3141 -- */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * If a higher-priority task is ready, the internal
 *                       resource of the task is released, the current task
 *                       is put into the ready state, its context is saved and
 *                       the higher-priority task is executed.
 *                       Otherwise the calling task is continued.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_Schedule(Os_SCBType *pScb) /* PRQA S 3006*/ /* VL_Os_3006*/
{
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
/* Basic status */
#if (CFG_INTERNAL_RESOURCE_MAX > 0)
    if (Os_InterResCeiling[pScb->SysRunningTaskId] != (Os_PriorityType)0)
    {
        Os_ReleaseInternalResource(pScb, pScb->SysRunningTaskId);
        pScb->SysRunningTCB->TaskRunPrio = Os_TaskCfg[pScb->SysRunningTaskId].TaskPriority;
    }
#endif

    Os_UpdateHighPrioTask(pScb);

    if (pScb->SysHighTaskId != pScb->SysRunningTaskId)
    {
        pScb->SysDispatchLocker = 0u;

        Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
    }

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
    Os_GetInternalResource(pScb);
#endif

    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * If a higher-priority task is ready, the internal
 *                       resource of the task is released, the current task
 *                       is put into the ready state, its context is saved and
 *                       the higher-priority task is executed.
 *                       Otherwise the calling task is continued.
 */
/* PRQA S 6070, 3006, 1503, 3408, 1512 ++ */ /* VL_MTR_Os_STCAL,VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
StatusType Schedule(void)
/* PRQA S 6070, 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType status = E_OK;
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_Schedule);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_Schedule_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_SCHEDULE,
        .ObjectType = OS_OBJECT_TASK,
        .ObjectID = (Os_AppObjectId)pScb->SysRunningTaskId,
        .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        status = E_OS_CALLEVEL;
    }
    else
#endif
#if (TRUE == CFG_GLOBAL_TASK_STACK_SHARING)
    if (Os_TaskCfg[pScb->SysRunningTaskId].StackSharing == TRUE)
    {
        status = E_OS_LIMIT;
    }
    else
#endif
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 3326, 2004 */ /* VL_Os_3326, VL_Os_2004 */
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
        if (pScb->SysOsLevel != OS_LEVEL_TASK)
        {
            status = E_OS_CALLEVEL;
        }
        else
#endif
        {
#if (CFG_STD_RESOURCE_MAX > 0U)
            if (Os_TCB[pScb->SysRunningTaskId]->TaskResCount > 0U)
            {
                status = E_OS_RESOURCE;
            }
            else
#endif
            {
#if (CFG_SPINLOCK_MAX > 0U)
                status = Os_SpinlockSafetyCheck(pScb->SysRunningTaskId);
#endif
            }
        }
#endif

        if ((StatusType)E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
        {
            Os_Schedule(pScb);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        /* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_Schedule(), OSServiceId_Schedule, status, pScb); /* PRQA S 2880 */ /* VL_Os_2880 */
        /* PRQA S 3138 -- */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_Schedule);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_Schedule_Return, status);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service causes the termination of the calling task. After
 *                       termination of the calling task a succeeding task <TaskID
 */
/* PRQA S 3006, 6070, 6010, 6030, 6080, 1503, 3408, 1512 ++ */ /* VL_Os_3006, VL_MTR_Os_STCAL, VL_MTR_Os_STCYC, VL_MTR_Os_STMIF, VL_MTR_Os_STPTH, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
StatusType ChainTask(TaskType TaskID)
/* PRQA S 3006, 6070, 6010, 6030, 6080, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType status = E_OK;

    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_ChainTask);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ChainTask_Start, TaskID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_CHAIN_TASK,
        .ObjectType = OS_OBJECT_TASK,
        .ObjectID = (Os_AppObjectId)TaskID,
        .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)TaskID, (uint8)OS_OBJECT_TASK) != TRUE)
    {
        status = E_OS_ID;
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        status = E_OS_CALLEVEL;
    }
    else
#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
    if (OS_LEVEL_TASK != pScb->SysOsLevel)
    {
        status = E_OS_CALLEVEL;
    }
    else
#endif
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 3326, 2004 */ /* VL_Os_3326, VL_Os_2004 */
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
#if (CFG_STD_RESOURCE_MAX > 0U)
        if (Os_TCB[pScb->SysRunningTaskId]->TaskResCount > 0U)
        {
            status = E_OS_RESOURCE;
        }
        else
#endif
        {
#if (CFG_SPINLOCK_MAX > 0U)
            status = Os_SpinlockSafetyCheck(pScb->SysRunningTaskId);
#endif
        }
#endif

        if ((StatusType)E_OK == status) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
        {
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType ownerCore = OS_TASK_GET_COREID(TaskID);
            if (ownerCore != Os_GetCoreIdLocal())
            {
                status = Os_RpcCall_ActivateTask(ownerCore, OS_RPC_SYNC, TaskID);
                if (E_OK == status)
                {
                    Os_TerminateTask(pScb->SysRunningTaskId);
                }
            }
            else
#endif
            {
                status = Os_ChainTask(TaskID);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ChainTask(TaskID),
                          OSServiceId_ChainTask,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ChainTask);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ChainTask_Return, status);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Set the task status to ready and prepare for scheduling.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_SetTaskToReady(TaskType taskId) /* PRQA S 3006*/ /* VL_Os_3006*/
/* PRQA S 1532 -- */
{
    Os_SCBType *pScb = OS_TASK_GET_SCB(taskId);
    Os_TCB[taskId]->TaskState = OS_TASK_STATE_READY;
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Release, taskId);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */

    Os_ReadyQueueInsert(pScb->QueueMg, taskId, OS_LEVEL_TASK,
                        Os_TCB[taskId]->TaskRunPrio);

    if (Os_TCB[taskId]->TaskRunPrio > pScb->SysHighPrio)
    {
        pScb->SysHighTaskId = taskId;
        pScb->SysHighPrio = Os_TCB[taskId]->TaskRunPrio;

#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
        if (pScb->SysDispatchLocker == 0u)
        {
            Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
        }
#endif
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Memory Protection for Task
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_MemProtTaskMap(Os_SCBType *pScb, Os_TaskType taskId)
/* PRQA S 3673 -- */
{
    if (pScb->SysAppId != pScb->SysRunningAppId)
    {
        Os_ApplicationType appId  = Os_TaskCfg[taskId].ObjAppCfg->HostApp;
        Os_Hal_MemProtTaskMap(taskId, appId); /* PRQA S 1520 */ /* VL_Os_1520 */
    }
    else
    {
        /* SYS_APP, as OS kernel, have all access rights */
        Os_Hal_MemProtKernelMap();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * pre entry highest priority task
 */
/* PRQA S 6070, 1532 ++ */ /* VL_MTR_Os_STCAL, VL_QAC_OneFunRef */
void Os_SwitchTask(Os_CoreIdType coreId)
/* PRQA S 6070, 1532 -- */
{
    Os_SCBType *pScb = Os_GetSystemContext(coreId);
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_TimerTickType curTicks;
#endif
    pScb->SysPrevTaskID = pScb->SysRunningTaskId;
#if (TRUE == CFG_STACK_CHECK)
    const Os_StackType *StackPtr;
#endif

    if (NULL_PTR != pScb->SysRunningTCB)
    {
/* check running task stack overflow. */
#if (TRUE == CFG_STACK_CHECK)
        StackPtr = &Os_TaskStack[pScb->SysRunningTaskId];
        Os_StackMonitor(StackPtr);
#endif

        if (OS_TASK_STATE_RUNNING == pScb->SysRunningTCB->TaskState)
        {
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
            Os_TpSupend(pScb, TP_FOR_TASK);
#endif
            pScb->SysRunningTCB->TaskState = OS_TASK_STATE_READY;
            /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
            /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
            /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
            ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Preempt, pScb->SysRunningTaskId);
            Os_PostTaskHook(pScb);
            /* PRQA S 4543, 4523, 3762, 1277 -- */
            /* PRQA S 1821, 4532, 4544, 4542 -- */
            /* PRQA S 3138, 3141 -- */
        }
    }

    pScb->SysRunningTaskId = pScb->SysHighTaskId;
    pScb->SysRunningTCB = Os_TCB[pScb->SysRunningTaskId];
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_IncrementTaskScheduleCount();
#endif
    /*Writing the running ApplID and Object */

#if (CFG_OSAPPLICATION_MAX > 0U)
    if (Os_TCB[pScb->SysRunningTaskId]->CallBackAppID != INVALID_OSAPPLICATION)
    {
        pScb->SysRunningAppId = Os_TCB[pScb->SysRunningTaskId]->CallBackAppID;
    }
    else
    {
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_TASK, (Os_AppObjectId)pScb->SysRunningTaskId);
    }
    pScb->SysRunningAppObj = OS_OBJECT_TASK;
#endif

#if (OS_PREEMPTIVE_MIXED == CFG_SCHED_POLICY)
    if (OS_PREEMPTIVE_NON == Os_TaskCfg[pScb->SysRunningTaskId].TaskSchedule)
    {
        pScb->SysDispatchLocker = 1U;
    }
#endif

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
    Os_GetInternalResource(pScb);
#endif

    Os_TaskStateType tempState = pScb->SysRunningTCB->TaskState;

    pScb->SysRunningTCB->TaskState = OS_TASK_STATE_RUNNING;
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Start, pScb->SysRunningTaskId);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */

    Os_PreTaskHook(pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    if (OS_TASK_STATE_START == tempState)
    {
        pScb->SysRunningTCB->TaskStackTop = Os_TaskCfg[pScb->SysRunningTaskId].TaskStack.StackTop;

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_MemProtTaskMap(pScb, pScb->SysRunningTaskId); /* PRQA S 1520 */ /* VL_Os_1520 */
#endif

/* Timing protection: start task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskStart(pScb->SysCore, pScb->SysRunningTaskId, TP_EXE);
#endif

        Os_Hal_FirstEnterTask(pScb->SysRunningTaskId);

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
        Os_TaskRecordStartTick(pScb->SysRunningTaskId);
#endif
    }
    else
    {
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_MemProtTaskMap(pScb, pScb->SysRunningTaskId); /* PRQA S 1520 */ /* VL_Os_1520 */
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TpResume(pScb, TP_FOR_TASK);
#endif
    }
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    curTicks = Os_ExitTaskRecordTick(pScb->SysPrevTaskID);
    Os_EnterTaskRecordTick(pScb->SysRunningTaskId, curTicks);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_UpdateHighPrioTask(Os_SCBType *pScb)
{
    pScb->SysHighPrio = Os_GetHighPrio(pScb->QueueMg);
    pScb->SysHighTaskId = Os_ReadyQueueGetFirst(pScb->QueueMg, pScb->SysHighPrio);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

/*=======[E N D   O F   F I L E]==============================================*/
