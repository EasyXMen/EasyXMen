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
 **  @file               : Os_Event.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Event API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Event.h"
#include "Os_Spinlock.h"
#include "Os_Resource.h"
#include "Os_Task.h"
#include "Os_Rpc.h"
#include "Os_Sprot.h"
#include "Os_ReadyQue.h"
#include "Os_Tprot.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"
#include "Os_Monitor.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (CFG_EXTENDED_TASK_MAX > 0)
/**
 * @brief           Performs bitwise comparison of event masks
 * @param[in]       eventMask: Event mask to check
 * @param[in]       mask: Reference mask for comparison
 * @return          StatusType
 * @retval          E_OK: If eventMask is contained in mask
 * @retval          E_OS_ACCESS: If eventMask contains bits not in mask
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_EventBitCompare(EventMaskType eventMask, EventMaskType mask);

/**
 * @brief           Performs safety checks for GetEvent function
 * @param[in]       curCoreId: Current core ID
 * @param[in]       taskId: Task ID to check
 * @return          StatusType
 * @retval          E_OK: All checks pass
 * @retval          E_OS_CORE: Task belongs to another core
 * @retval          E_OS_ACCESS: Task is not an extended task
 * @retval          E_OS_STATE: Task is suspended
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_GetEventSafetyCheck(Os_CoreIdType curCoreId, TaskType taskId);

#if (OS_STATUS_EXTENDED == CFG_STATUS)
/**
 * @brief           Performs safety checks for WaitEvent function
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       mask: Event mask to wait for
 * @return          StatusType
 * @retval          E_OK: All checks pass
 * @retval          E_OS_CALLEVEL: Not called from task level
 * @retval          E_OS_ACCESS: Task is not an extended task
 * @retval          E_OS_RESOURCE: Task occupies resources
 * @retval          E_OS_SPINLOCK: Task occupies spinlocks
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_WaitEventSafetyCheck(const Os_SCBType *pScb, EventMaskType mask);

/**
 * @brief           Validates parameters for GetEvent function
 * @param[in]       taskId: Task ID to check
 * @param[in]       event: Pointer where to store events
 * @return          StatusType
 * @retval          E_OK: All parameters valid
 * @retval          E_OS_ID: Invalid task ID
 * @retval          E_OS_ILLEGAL_ADDRESS: Null event pointer
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_GetEventParamCheck(TaskType taskId, const Os_EventMaskType *event);

/**
 * @brief           Performs safety checks for ClearEvent function
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       mask: Event mask to clear
 * @return          StatusType
 * @retval          E_OK: All checks pass
 * @retval          E_OS_CALLEVEL: Not called from task level
 * @retval          E_OS_ACCESS: Task is not an extended task
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_ClearEventSafetyCheck(const Os_SCBType *pScb, EventMaskType mask);

/**
 * @brief           Performs safety checks for SetEvent function
 * @param[in]       taskId: Task ID to set events for
 * @return          StatusType
 * @retval          E_OK: All checks pass
 * @retval          E_OS_ACCESS: Task is not an extended task
 * @retval          E_OS_STATE: Task is suspended
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_SetEventSafetyCheck(TaskType taskId, EventMaskType mask);
#endif

#if (OS_AUTOSAR_CORES > 1)
/**
 * @brief           RPC action handler for SetEvent
 * @param[in]       inPara: Parameter array containing task ID and event masks
 * @return          StatusType
 * @retval          Result of Os_SetEvent operation
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_SetEvent(uint32 *inPara);
#endif

/**
 * @brief           Implements WaitEvent functionality
 * @param[in]       mask: Event mask to wait for
 * @param[in]       pScb: Pointer to system control block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_WaitEvent(EventMaskType mask, Os_SCBType *pScb);

/**
 * @brief           Implements WaitAllEvents functionality
 * @param[in]       mask: Event mask to wait for (all events)
 * @param[in]       pScb: Pointer to system control block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_WaitAllEvents(EventMaskType mask, Os_SCBType *pScb);
#endif

/* ========================================== external function definitions ========================================= */
#if (CFG_EXTENDED_TASK_MAX > 0)

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the Event control block
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitEvent(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_TaskType idStartRange = Os_TaskIdRange[coreId].ExtendTask.Start;
    Os_TaskType idEndRange = Os_TaskIdRange[coreId].ExtendTask.End;

    if (INVALID_TASK != idStartRange) /* PRQA S 1881, 1461*/ /* VL_Os_1881, VL_Os_1461*/
    {
        for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
        {
            if (Os_ECB[i] != NULL_PTR)
            {
                /* PRQA S 4342 ++ */ /* VL_Os_4342 */
                Os_ClearECB((Os_TaskType)i);
                /* PRQA S 4342 -- */
            }

#if ((TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) || (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR)) /* PRQA S 3332 */ /* VL_Os_3332 */
            /* PRQA S 4342 ++ */ /* VL_Os_4342 */
            Os_InitMonitorEvent((Os_TaskType)i);
            /* PRQA S 4342 -- */
#endif
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Bitwise comparison.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_EventBitCompare(EventMaskType eventMask, EventMaskType mask)
{
    StatusType err = E_OK;

    if (eventMask != (eventMask & mask))
    {
        err = E_OS_ACCESS;
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Safety check when the GetEvent function is called.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_GetEventSafetyCheck(Os_CoreIdType curCoreId, TaskType taskId)
{
    StatusType err = E_OK;

#if (OS_AUTOSAR_CORES > 1)
    if (curCoreId != OS_TASK_GET_COREID(taskId))
    {
        err = E_OS_CORE;
    }
    else
#endif
    {
/* Extended Status */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        if (!Os_CheckExternalTaskId(taskId, curCoreId))
        {
            err = E_OS_ACCESS;
        }
        else
        {
            if (OS_TASK_STATE_SUSPENDED == Os_TCB[taskId]->TaskState)
            {
                err = E_OS_STATE;
            }
        }
#endif
    }

    UNUSED_PARAMETER(taskId);
    UNUSED_PARAMETER(curCoreId);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_STATUS_EXTENDED == CFG_STATUS)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Safety check when the WaitEvent function is called.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_WaitEventSafetyCheck(const Os_SCBType *pScb, EventMaskType mask)
{
    StatusType err = E_OK;
    Os_TaskType runningTaskId = pScb->SysRunningTaskId;

    if (pScb->SysOsLevel != OS_LEVEL_TASK)
    {
        err = E_OS_CALLEVEL;
    }
    else
    {
        if (!Os_CheckExternalTaskId(runningTaskId, pScb->SysCore))
        {
            err = E_OS_ACCESS;
        }
#if (CFG_STD_RESOURCE_MAX > 0U)
        else if (Os_TCB[runningTaskId]->TaskResCount > 0u)
        {
            err = E_OS_RESOURCE;
        }
#endif
#if (CFG_SPINLOCK_MAX > 0U)
        else if (Os_SpinlockSafetyCheck(pScb->SysRunningTaskId) != E_OK)
        {
            err = E_OS_SPINLOCK;
        }
#endif
        else
        {
            /* Task access EVENT permission check */
            err = Os_EventPermissionCheck(mask, Os_TaskCfg[runningTaskId].EventAccessMask);
        }
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Check the validity of the arguments.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_GetEventParamCheck(TaskType taskId, const Os_EventMaskType *event)
{
    StatusType err = E_OK;

    if (Os_ObjectIDCheck((ObjectType)taskId, (uint8)OS_OBJECT_TASK) != TRUE)
    {
        err = E_OS_ID;
    }
    else if (NULL_PTR == event) /* PRQA S 2004 */ /* VL_Os_2004 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Safety check when the ClearEvent function is called.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_ClearEventSafetyCheck(const Os_SCBType *pScb, EventMaskType mask)
{
    StatusType err = E_OK;
    Os_TaskType runningTaskId = pScb->SysRunningTaskId;

    if (pScb->SysOsLevel != OS_LEVEL_TASK)
    {
        err = E_OS_CALLEVEL;
    }
    else
    {
        if (!Os_CheckExternalTaskId(runningTaskId, pScb->SysCore))
        {
            err = E_OS_ACCESS;
        }
        else
        {
            /* Task access EVENT permission check */
            err = Os_EventPermissionCheck(mask, Os_TaskCfg[runningTaskId].EventAccessMask);
        }
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Safety check when the SetEvent function is called.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_SetEventSafetyCheck(TaskType taskId, EventMaskType mask)
{
    StatusType err = E_OK;

    if (!Os_CheckExternalTaskId(taskId, OS_TASK_GET_COREID(taskId)))
    {
        err = E_OS_ACCESS;
    }
    else if (OS_TASK_STATE_SUSPENDED == Os_TCB[taskId]->TaskState)
    {
        err = E_OS_STATE;
    }
    else
    {
        /* Task access EVENT permission check */
        err = Os_EventPermissionCheck(mask, Os_TaskCfg[taskId].EventAccessMask);
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The service may be called from an interrupt service routine and
 *                       from the task level, but not from hook routines.
 *                       The events of task <taskId
 */
#if (OS_AUTOSAR_CORES > 1)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL StatusType Os_RpcAction_SetEvent(uint32 *inPara)
/* PRQA S 3673 -- */
{
    Os_EventMaskType EventLow = (Os_EventMaskType)(inPara[1]);
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Os_EventMaskType EventHigh = ((Os_EventMaskType)(inPara[2]) << 32u);
    /* PRQA S 3120 -- */
    return Os_SetEvent((TaskType)inPara[0], /* PRQA S 4342 */ /* VL_Os_4342 */
                       (EventMaskType)(EventLow | EventHigh));
}

/* PRQA S 1505 ++ */ /* VL_Os_1505 */
StatusType Os_RpcCall_SetEvent(
    Os_CoreIdType ownerCore,
    Os_RpcSyncType syncType,
    TaskType taskId,
    EventMaskType mask)
/* PRQA S 1505 -- */
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = syncType,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_SetEvent,
        .InPara[0] = (uint32)taskId,
        .InPara[1] = (uint32)(mask & OS_EVENT_32_MSAK_SIZE),
        /* PRQA S 0691, 3120 ++ */ /* VL_Os_0691, VL_QAC_MagicNum */
        .InPara[2] = (uint32)(mask >> 32u),
        /* PRQA S 0691, 3120 -- */
    };/* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

/* PRQA S 1503,3006,6070,3408,1532,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_3408, VL_QAC_OneFunRef, VL_Os_1512 */
StatusType SetEvent(TaskType TaskID, EventMaskType Mask)
/* PRQA S 1503,3006,6070,3408,1532,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_SetEvent);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetEvent_Start, TaskID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)TaskID, (uint8)OS_OBJECT_TASK) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_SET_EVENT,
            .ObjectType = OS_OBJECT_TASK,
            .ObjectID = (Os_AppObjectId)TaskID,
            /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
            .Address = NULL_PARA,
            /* PRQA S 1258 -- */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType ownerCore = OS_TASK_GET_COREID(TaskID);
            if (ownerCore != Os_GetCoreIdLocal())
            {
                err = Os_RpcCall_SetEvent(ownerCore, OS_RPC_SYNC, TaskID, Mask);
            }
            else
#endif
            {
                err = Os_SetEvent(TaskID, Mask);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetEvent(TaskID, Mask),
                          OSServiceId_SetEvent,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_SetEvent);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetEvent_Return, err);
    OS_HAL_EXIT_KERNEL();/* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Asynchronous version of the SetEvent() function.
 */
/* PRQA S 1503,3006,6070,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_3408, VL_Os_1512 */
void SetEventAsyn(TaskType TaskID, EventMaskType Mask) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
/* PRQA S 1503,3006,6070,3408,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_SetEventAsyn);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetEventAsyn_Start, TaskID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)TaskID, (uint8)OS_OBJECT_TASK) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_SET_EVENT_ASYN,
            .ObjectType = OS_OBJECT_TASK,
            .ObjectID = (Os_AppObjectId)TaskID,
            /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
            .Address = NULL_PARA,
            /* PRQA S 1258 -- */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType ownerCore = OS_TASK_GET_COREID(TaskID);
            if (ownerCore != Os_GetCoreIdLocal())
            {
                err = Os_RpcCall_SetEvent(ownerCore, OS_RPC_ASYNC, TaskID, Mask);
            }
            else
#endif
            {
                err = Os_SetEvent(TaskID, Mask);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetEventAsyn(TaskID, Mask),
                          OSServiceId_SetEventAsyn,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_SetEventAsyn);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetEventAsyn_Return, 0);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
    UNUSED_PARAMETER(err);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 6070++ */ /* VL_MTR_Os_STCAL */
OS_LOCAL void Os_WaitEvent(EventMaskType mask, Os_SCBType *pScb) /* PRQA S 3006*/ /* VL_Os_3006*/
/* PRQA S 6070 -- */
{
    Os_TaskType runningTaskId = pScb->SysRunningTaskId;
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    Os_ECB[runningTaskId]->WaitEventMask = mask;
    if (0u == (Os_ECB[runningTaskId]->SetEventMask & mask))
    {
        Os_PostTaskHook(pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
        Os_ReleaseInternalResource(pScb, runningTaskId);
#endif

        Os_TCB[runningTaskId]->TaskState = OS_TASK_STATE_WAITING;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Wait, runningTaskId);
        /* PRQA S 4543, 4523, 3762, 1277 -- */
        /* PRQA S 1821, 4532, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
        Os_ReadyQueueRemove(pScb->QueueMg, Os_TCB[runningTaskId]->TaskRunPrio);

        Os_TCB[runningTaskId]->TaskRunPrio =
            Os_TaskCfg[runningTaskId].TaskPriority;

/* Timing protection: reset task exe time. OS473. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(pScb->SysCore, runningTaskId, TP_EXE);
#endif

        Os_UpdateHighPrioTask(pScb);

        pScb->SysDispatchLocker = 0u;

        Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
    }
    OS_HAL_EXIT_CRITICAL();

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_MonitorEventEndTime(runningTaskId, mask);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The state of the calling task is set to waiting, unless
 *                       at least one of the events specified in <Mask
 */
/* PRQA S 1503,3006,6070,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_3408, VL_Os_1512 */
StatusType WaitEvent(EventMaskType Mask)
/* PRQA S 1503,3006,6070,3408,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_WaitEvent);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WaitEvent_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_WAIT_EVENT,
        .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
        .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
    err = Os_ServiceProtCheck(pScb, &SprotParam);
    if (E_OK == err)
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        err = Os_WaitEventSafetyCheck(pScb, Mask);
        if ((StatusType)E_OK == err)
#endif
        {
            Os_WaitEvent(Mask, pScb);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_WaitEvent(Mask), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_WaitEvent,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_WaitEvent);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WaitEvent_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 6070++ */ /* VL_MTR_Os_STCAL */
OS_LOCAL void Os_WaitAllEvents(EventMaskType mask, Os_SCBType *pScb) /* PRQA S 3006*/ /* VL_Os_3006*/
/* PRQA S 6070 -- */
{
    Os_TaskType runningTaskId = pScb->SysRunningTaskId;
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    Os_ECB[runningTaskId]->WaitEventMask = mask;
    Os_ECB[runningTaskId]->IsWaitAllEvents = TRUE;

    if (E_OK != Os_EventIsAllEventsTriggered(mask, Os_ECB[runningTaskId]->SetEventMask))
    {
        Os_PostTaskHook(pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

#if (CFG_INTERNAL_RESOURCE_MAX > 0)
        Os_ReleaseInternalResource(pScb, runningTaskId);
#endif

        Os_TCB[runningTaskId]->TaskState = OS_TASK_STATE_WAITING;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Wait, runningTaskId);
        /* PRQA S 4543, 4523, 3762, 1277 -- */
        /* PRQA S 1821, 4532, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
        Os_ReadyQueueRemove(pScb->QueueMg, Os_TCB[runningTaskId]->TaskRunPrio);

        Os_TCB[runningTaskId]->TaskRunPrio =
            Os_TaskCfg[runningTaskId].TaskPriority;

/* Timing protection: reset task exe time. OS473. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(pScb->SysCore, runningTaskId, TP_EXE);
#endif

        Os_UpdateHighPrioTask(pScb);

        pScb->SysDispatchLocker = 0u;

        Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
    }
    OS_HAL_EXIT_CRITICAL();

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_MonitorEventEndTime(runningTaskId, mask);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The state of the calling task is set to waiting, must
 *                       all the events specified in <Mask
 */
/* PRQA S 1503,3006,6070,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_3408, VL_Os_1512 */
StatusType WaitAllEvents(EventMaskType Mask)
/* PRQA S 1503,3006,6070,3408,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_WaitAllEvents);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WaitAllEvents_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_WAIT_ALL_EVENTS,
        .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
        .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
    err = Os_ServiceProtCheck(pScb, &SprotParam);
    if (E_OK == err)
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        err = Os_WaitEventSafetyCheck(pScb, Mask);
        if ((StatusType)E_OK == err)
#endif
        {
            Os_WaitAllEvents(Mask, pScb);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_WaitEvent(Mask), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_WaitAllEvents,
                          err, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_WaitAllEvents);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_WaitAllEvents_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service returns the current state of all event
 *                       bits of the task
 *                       <TaskID
 */
/* PRQA S 1503,3006,6070,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_3408, VL_Os_1512 */
StatusType GetEvent(TaskType TaskID, EventMaskRefType Event)
/* PRQA S 1503,3006,6070,3408,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
    OS_HAL_DECLARE_CRITICAL();

    Os_CoreIdType curCoreId = Os_GetCoreIdLocal();
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetSystemContext(curCoreId);
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetEvent);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetEvent_Start, TaskID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    err = Os_GetEventParamCheck(TaskID, Event);
    if (E_OK == err)
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_GET_EVENT,
            .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
            .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
            /* PRQA S 0306 ++ */ /* VL_Os_0306 */
            .Address = (uint32)Event,
            /* PRQA S 0306 -- */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
            OS_HAL_ENTRY_CRITICAL();
            err = Os_GetEventSafetyCheck(curCoreId, TaskID);
            if ((StatusType)E_OK == err)
            {
                *Event = Os_ECB[TaskID]->SetEventMask;
            }
            OS_HAL_EXIT_CRITICAL();
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetEvent(TaskID, Event),
                          OSServiceId_GetEvent,
                          err, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_GetEvent);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetEvent_Return, 0);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The events of the extended task calling ClearEvent are cleared
 *                       according to the event mask <Mask
 */
/* PRQA S 1503,3006,6070,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_3408,VL_Os_1512 */
StatusType ClearEvent(EventMaskType Mask)
/* PRQA S 1503,3006,6070,3408,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;

    OS_HAL_DECLARE_CRITICAL();
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_ClearEvent);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ClearEvent_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_CLEAR_EVENT,
        .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
        .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
    err = Os_ServiceProtCheck(pScb, &SprotParam);
    if (E_OK == err)
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        err = Os_ClearEventSafetyCheck(pScb, Mask);
        if ((StatusType)E_OK == err)
#endif
        {
            OS_HAL_ENTRY_CRITICAL();
#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
            Os_MonitorEventResponseTime(pScb->SysRunningTaskId, Mask);
#endif
#if (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
            Os_MonitorEventResponseRate(pScb->SysRunningTaskId, Mask);
#endif

            Os_ECB[pScb->SysRunningTaskId]->SetEventMask &= (~Mask);
            OS_HAL_EXIT_CRITICAL();
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_ClearEvent(Mask), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_ClearEvent,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ClearEvent);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ClearEvent_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The service may be called from an interrupt service routine and
 *                       from the task level, but not from hook routines.
 *                       The events of task <taskId
 */
/* PRQA S 1505,6030,6070 ++ */ /* VL_Os_1505, VL_MTR_Os_STMIF, VL_MTR_Os_STCAL */
StatusType Os_SetEvent(TaskType taskId, EventMaskType mask)
/* PRQA S 1505,6030,6070 -- */
{
    OS_HAL_DECLARE_CRITICAL();
    StatusType err = E_OK;

/* Extended Status */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    err = Os_SetEventSafetyCheck(taskId, mask);
    if ((StatusType)E_OK == err)
#endif
    {
        OS_HAL_ENTRY_CRITICAL();
/* Timing protection: Check inter-arrival time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        if ((StatusType)E_OK != Os_TmProtTaskFrameChk(taskId))
        {
            err = E_OS_PROTECTION_ARRIVAL;
        }
        else
#endif
        {
            /* Standard Status */
            Os_ECB[taskId]->SetEventMask |= mask;

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
            Os_MonitorEventStartTime(taskId, mask);
#endif
#if (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
            Os_AddEventResponseNum(taskId, mask);
#endif

            Os_EventMaskType SetEventMask = Os_ECB[taskId]->SetEventMask;
            Os_EventMaskType WaitEventMask = Os_ECB[taskId]->WaitEventMask;

            if (OS_TASK_STATE_WAITING == Os_TCB[taskId]->TaskState)
            {
                if (TRUE == Os_ECB[taskId]->IsWaitAllEvents)
                {
                    if (E_OK == Os_EventIsAllEventsTriggered(WaitEventMask, SetEventMask))
                    {
                        Os_ECB[taskId]->IsWaitAllEvents = FALSE;
                        Os_SetTaskToReady(taskId);
                    }
                }
                else
                {
                    if ((SetEventMask & WaitEventMask) != 0u)
                    {
                        Os_SetTaskToReady(taskId);
                    }
                }
            }
        }
        OS_HAL_EXIT_CRITICAL();
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
