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
 **  @file               : Os_ProtectHook.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for ProtectHook API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_ProtectHook.h"
#include "Os_Hook.h"
#include "Os_Spinlock.h"
#include "Os_Resource.h"
#include "Os_Task.h"
#include "Os_Tprot.h"
#include "Os_Sprot.h"
#include "Os_ReadyQue.h"
#include "Os_Interrupt.h"
#include "Os_Event.h"
#include "Os_Appl.h"
#include "Os_Event.h"
#include "Os_Kernel.h"
#include "Os_Arti.h"
#include "Os_TrustedFunc.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (TRUE == CFG_PROTECTIONHOOK)
/**
 * @brief           Verifies if the running task or ISR2 ID is valid
 * @param[in]       whoHook: Indicates whether the check is for a task (OS_TMPROT_HOOK_TASK) or ISR (OS_TMPROT_HOOK_ISR)
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          boolean
 * @retval          E_OK: The running task/ISR2 ID is valid
 * @retval          E_NOT_OK: The running task/ISR2 ID is invalid
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_CheckRunningTaskIDandIsr2ID(uint32 whoHook, const Os_SCBType *pScb);
#endif

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
/**
 * @brief           Resumes interrupt operations for a task in protection hook
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtHookTaskResumeInt(Os_SCBType *pScb);

/**
 * @brief           Resumes interrupt operations for an ISR in protection hook
 * @param[in]       isrID: ID of the ISR
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtHookIsrResumeInt(Os_IsrType isrID, Os_SCBType *pScb);

/**
 * @brief           Releases all critical resources held by a task
 * @param[in]       runningTaskTcb: Pointer to the Task Control Block of the running task
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtTaskCriticalRelease(Os_TCBType * const runningTaskTcb, Os_SCBType *pScb);

/**
 * @brief           Resets a task's TCB when terminating the task
 * @param[in]       runningTaskID: ID of the task being terminated
 * @param[in]       coreId: ID of the core
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtResetTaskTCB(Os_TaskType runningTaskID, Os_CoreIdType coreId);

/**
 * @brief           Terminates the running task from within the protection hook
 * @param[in]       errType: Type of error that triggered the protection hook
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtHookTerminateTask(StatusType errType, Os_SCBType *pScb);

#if (CFG_ISR2_MAX > 0)
/**
 * @brief           Releases all critical resources held by an ISR2
 * @param[in]       pIcb: Pointer to the Interrupt Control Block
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtIsr2CriticalRelease(const Os_ICBType *pIcb, Os_SCBType *pScb);

/**
 * @brief           Terminates the running ISR2 from within the protection hook
 * @param[in]       errType: Type of error that triggered the protection hook
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtHookTerminateIsrC2(StatusType errType, Os_SCBType *pScb);
#endif

/**
 * @brief           Terminates either a task or an ISR based on the hook type
 * @param[in]       whoHook: Indicates whether to terminate a task (OS_TMPROT_HOOK_TASK) or ISR (OS_TMPROT_HOOK_ISR)
 * @param[in]       errType: Type of error that triggered the protection hook
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtHookTermTaskIsr(uint32 whoHook, StatusType errType, Os_SCBType *pScb);

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief           Terminates an OS application from within the protection hook
 * @param[in]       errType: Type of error that triggered the protection hook
 * @param[in]       restartOption: Whether to restart the application or not
 * @param[in]       whoHook: Indicates whether the termination was triggered by a task or ISR
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtHookTermAppl(StatusType errType, RestartType restartOption, uint32 whoHook, Os_SCBType *pScb);
#endif
#endif

/**
 * @brief           Validates the running application ID and terminates the application
 * @param[in]       errType: Type of error that triggered the protection hook
 * @param[in]       restartOpt: Whether to restart the application or not
 * @param[in]       whoHook: Indicates whether the termination was triggered by a task or ISR
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtTermAppl(StatusType errType, Os_RestartType restartOpt, uint32 whoHook, Os_SCBType *pScb);

/**
 * @brief           Processes the return value from the protection hook
 * @param[in]       Ret: Return value from the protection hook
 * @param[in]       errType: Type of error that triggered the protection hook
 * @param[in]       whoHook: Indicates whether the hook was triggered by a task or ISR
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProtReturnProcess(ProtectionReturnType Ret, StatusType errType, uint32 whoHook, Os_SCBType *pScb);
#endif

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_PROTECTIONHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * check the running task ID is legal,
 *                          running task ID < idle task ID
 *                          running Isr2 ID < CFG_ISR2_MAX
 */
OS_LOCAL boolean Os_CheckRunningTaskIDandIsr2ID(uint32 whoHook, const Os_SCBType *pScb)
{
    boolean err = (boolean)E_NOT_OK;
    Os_CoreIdType coreId = pScb->SysCore;

    if (OS_TMPROT_HOOK_TASK == whoHook)
    {
        Os_TaskType idStartRange = Os_TaskIdRange[coreId].AllTask.Start;
        Os_TaskType idEndRange = Os_TaskIdRange[coreId].AllTask.End;
        if (((uint32)idStartRange <= (uint32)pScb->SysRunningTaskId) && ((uint32)pScb->SysRunningTaskId < ((uint32)(idEndRange) - 1U)))
        {
            err = (boolean)E_OK;
        }
        else
        {
            err = (boolean)E_NOT_OK;
        }
    }

#if (CFG_ISR2_MAX > 0)
    if (OS_TMPROT_HOOK_ISR == whoHook)
    {
        if (Os_CheckIsr2Id(pScb->SysRunningIsrCat2Id))
        {
            err = (boolean)E_OK;
        }
        else
        {
            err = (boolean)E_NOT_OK;
        }
    }
#endif

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/* Protection Hook. */
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Resume isr operation in protection hook.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_ProtHookTaskResumeInt(Os_SCBType *pScb)
/* PRQA S 3673 -- */
{
    Os_CoreIdType SysCore = pScb->SysCore;
    Os_TaskType runningTaskID = pScb->SysRunningTaskId;
    Os_TCBType *pTcb = Os_TCB[runningTaskID]; /* PRQA S 3432, 3678 */ /* VL_Os_3432, VL_Os_3678 */

#if ((OS_SC2 == CFG_SC) && (TRUE == CFG_TIMING_PROTECTION_ENABLE))
    if(TRUE == pTcb->taskTpNode[TP_SUS_OS_INT].TpNodeInList)
    {
        Os_TmProtTaskEnd(SysCore, runningTaskID, TP_SUS_OS_INT);
        pScb->SuspendOsCount = 0u;
        Os_Hal_SetIpl(pScb->SaveOsIntNested, OS_ISR_ENABLE);       
    }

    if (TRUE == pTcb->taskTpNode[TP_SUS_ALL_INT].TpNodeInList)
    {
        Os_TmProtTaskEnd(SysCore, runningTaskID, TP_SUS_ALL_INT);
        pScb->SuspendAllCount = 0u;
        Os_Hal_EnableAllInt_ButTimingProtInt(SysCore);
    }

    if (TRUE == pTcb->taskTpNode[TP_DIS_ALL_INT].TpNodeInList)
    {
        Os_TmProtTaskEnd(SysCore, runningTaskID, TP_DIS_ALL_INT);
        pScb->SuspendAllCount = 0u;
        Os_Hal_EnableAllInt_ButTimingProtInt(SysCore);
    }   
#elif ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    /* Resume Interruput operation in SC3/4 */
    if(pTcb->TaskSuspendOsCount > 0u)
    {
        pScb->SuspendOsCount = 0u;
        pTcb->TaskSuspendOsCount = 0u;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(SysCore, runningTaskID, TP_SUS_OS_INT);
#endif
        Os_Hal_SetIpl(pScb->SaveOsIntNested, OS_ISR_ENABLE);
    }

    if(pTcb->TaskSuspendAllCount > 0u)
    {
        pScb->SuspendAllCount = 0u;
        pTcb->TaskSuspendAllCount = 0u;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(SysCore, runningTaskID, TP_SUS_ALL_INT);
        Os_Hal_EnableAllInt_ButTimingProtInt(SysCore);
#else
        Os_Hal_RestoreInt(pScb->SaveAllIntNested);
#endif        
    }

    if(pTcb->TaskDisableAllCount > 0u)
    {
        pTcb->TaskDisableAllCount = 0u;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(SysCore, runningTaskID, TP_DIS_ALL_INT);
        Os_Hal_EnableAllInt_ButTimingProtInt(SysCore);
#else
        Os_Hal_RestoreInt(pScb->SaveAllIntNested);
#endif        
    }
#endif
    UNUSED_PARAMETER(SysCore);
    UNUSED_PARAMETER(pTcb);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Resume isr operation in protection hook.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_ProtHookIsrResumeInt(Os_IsrType isrID, Os_SCBType *pScb)
/* PRQA S 3673 -- */
{
    Os_ICBType *pICB = Os_ICB[isrID]; /* PRQA S 3678 */ /* VL_Os_3678 */
    Os_CoreIdType SysCore = pScb->SysCore;

#if ((OS_SC2 == CFG_SC) && (TRUE == CFG_TIMING_PROTECTION_ENABLE))
    if(TRUE == pICB->isrTpNode[TP_SUS_OS_INT].TpNodeInList)
    {
        Os_TmProtIsrEnd(SysCore, isrID, TP_SUS_OS_INT);
        pScb->SuspendOsCount = 0u;
        Os_Hal_SetIpl(pScb->SaveOsIntNested, OS_ISR_ENABLE);       
    }

    if (TRUE == pICB->isrTpNode[TP_SUS_ALL_INT].TpNodeInList)
    {
        Os_TmProtIsrEnd(SysCore, isrID, TP_SUS_ALL_INT);
        pScb->SuspendAllCount = 0u;
        Os_Hal_EnableAllInt_ButTimingProtInt(SysCore);
    }

    if (TRUE == pICB->isrTpNode[TP_DIS_ALL_INT].TpNodeInList)
    {
        Os_TmProtIsrEnd(SysCore, isrID, TP_DIS_ALL_INT);
        pScb->SuspendAllCount = 0u;
        Os_Hal_EnableAllInt_ButTimingProtInt(SysCore);
    }   
#elif ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    /* Resume Interruput operation in SC3/4 */
    if(pICB->IsrC2SuspendOSCount > 0u)
    {
        pScb->SuspendOsCount = 0u;
        pICB->IsrC2SuspendOSCount = 0u;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtIsrEnd(SysCore, isrID, TP_SUS_OS_INT);
#endif
        Os_Hal_SetIpl(pScb->SaveOsIntNested, OS_ISR_ENABLE);
    }

    if(pICB->IsrC2SuspendAllCount > 0u)
    {
        pScb->SuspendAllCount = 0u;
        pICB->IsrC2SuspendAllCount = 0u;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtIsrEnd(SysCore, isrID, TP_SUS_ALL_INT);
        Os_Hal_EnableAllInt_ButTimingProtInt(SysCore);
#else
        Os_Hal_RestoreInt(pScb->SaveAllIntNested);
#endif        
    }

    if(pICB->IsrC2DisableAllCount > 0u)
    {
        pICB->IsrC2DisableAllCount = 0u;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtIsrEnd(SysCore, isrID, TP_DIS_ALL_INT);
        Os_Hal_EnableAllInt_ButTimingProtInt(SysCore);
#else
        Os_Hal_RestoreInt(pScb->SaveAllIntNested);
#endif        
    }
#endif
    UNUSED_PARAMETER(SysCore);
    UNUSED_PARAMETER(pICB);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Release critical resource while terminating current task.
 */
OS_LOCAL void Os_ProtTaskCriticalRelease(Os_TCBType * const runningTaskTcb, Os_SCBType *pScb) /* PRQA S 3673 */ /* VL_QAC_3673 */
{
    /* Release resources occupied by running task. */
    /* OESK 8.2:
       In case of multiple resource occupation within one task, the user has
       to request and release resources following the LIFO principle
       (stack like). */

#if (CFG_SPINLOCK_MAX > 0U)
    for (uint16 CountIdx = runningTaskTcb->TaskCriticalZoneCount; CountIdx > 0u; CountIdx--)
    {
        uint16 CurrentCount = runningTaskTcb->TaskCriticalZoneCount - 1u;
        if (OS_OBJECT_SPINLOCK == runningTaskTcb->TaskCriticalZoneType[CurrentCount])
        {
            SpinlockIdType SpinlockId = runningTaskTcb->TaskCriticalZoneStack[CurrentCount]; /* PRQA S 4442 */ /* VL_Os_4442 */
            (void)Os_ReleaseSpinlock(pScb, SpinlockId);
        }
        else
        {
            ResourceType ResourceId = runningTaskTcb->TaskCriticalZoneStack[CurrentCount]; /* PRQA S 4442 */ /* VL_Os_4442 */
            (void)Os_ReleaseResource(pScb, ResourceId);
        }
    }
#elif (CFG_STD_RESOURCE_MAX > 0U)
    if (runningTaskTcb->TaskResCount > 0u)
    {
        for (uint16 i = runningTaskTcb->TaskResCount; i > 0u; i--)
        {
            (void)Os_ReleaseResource(pScb, runningTaskTcb->TaskResourceStack[i - 1u]);
        }
    }
    runningTaskTcb->TaskResCount = 0U;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Reset TCB while terminating current task.
 */
OS_LOCAL void Os_ProtResetTaskTCB(Os_TaskType runningTaskID, Os_CoreIdType coreId)
{
    Os_TCBType *runningTaskTcb = Os_TCB[runningTaskID];
/* Update task state. */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, coreId, OsTask_Terminate, runningTaskID);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
    runningTaskTcb->TaskState = OS_TASK_STATE_SUSPENDED;
#else
    if (runningTaskTcb->TaskActCount > 0U)
    {
        runningTaskTcb->TaskActCount = runningTaskTcb->TaskActCount - 1u;
    }

    if (runningTaskTcb->TaskActCount > 0U)
    {
        runningTaskTcb->TaskState = OS_TASK_STATE_START;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 4523, 3762, 1277, 2985 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277, VL_Os_2985 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, coreId, OsTask_Activate, runningTaskID);
        /* PRQA S 4543, 4523, 3762, 1277, 2985 -- */
        /* PRQA S 1821, 4532, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
    }
    else
    {
        runningTaskTcb->TaskState = OS_TASK_STATE_SUSPENDED;
    }
#endif

    /* Reset TCB. */
    const Os_TaskCfgType *pTaskCfg = &Os_TaskCfg[runningTaskID];
    runningTaskTcb->TaskStackTop = pTaskCfg->TaskStack.StackTop;
    runningTaskTcb->TaskRunPrio = pTaskCfg->TaskPriority;

    UNUSED_PARAMETER(coreId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate task in protection hook. Terminate
 *                        running task.
 */
/* PRQA S 6070++ */ /* VL_MTR_Os_STCAL */
OS_LOCAL void Os_ProtHookTerminateTask(StatusType errType, Os_SCBType *pScb) /* PRQA S 3006*/ /* VL_Os_3006*/
/* PRQA S 6070 -- */
{
    Os_TaskType runningTaskID = pScb->SysRunningTaskId;
    Os_TCBType *runningTaskTcb = Os_TCB[runningTaskID]; /* PRQA S 3678 */ /* VL_Os_3678 */

    Os_ProtTaskCriticalRelease(runningTaskTcb, pScb);

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    Os_ReleaseInternalResource(pScb, runningTaskID);
#endif

    Os_ProtHookTaskResumeInt(pScb);

    /*If the task is a non-preemptive task, then release it.*/
    if (OS_PREEMPTIVE_NON == Os_TaskCfg[runningTaskID].TaskSchedule)
    {
        pScb->SysDispatchLocker = 1U;
    }
    /* Remove task from ready queue. */
    Os_ReadyQueueRemove(pScb->QueueMg, runningTaskTcb->TaskRunPrio);

    Os_ProtResetTaskTCB(runningTaskID, pScb->SysCore);

#if (CFG_EXTENDED_TASK_MAX > 0U)
    if (Os_CheckExternalTaskId(runningTaskID, pScb->SysCore))
    {
        Os_ClearECB(runningTaskID);
    }
#endif

    if (pScb->SysHighTaskId == runningTaskID)
    {
        Os_UpdateHighPrioTask(pScb);
    }
#if (TRUE == CFG_STACK_CHECK)
    if (E_OS_STACKFAULT == (StatusType)errType)
    {
        /*nothing to do*/
    }
    else
#endif
    {
/* Timing protection: reset task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(pScb->SysCore, runningTaskID, TP_EXE);
        /* Timing protection do task_dispatch at the end of tm_prot_isr. */
        if ((E_OS_PROTECTION_LOCKED != (StatusType)errType) && (E_OS_PROTECTION_TIME != (StatusType)errType))
        {
            pScb->SysDispatchLocker = 0U;
            Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
        }
#if ((OS_SC4 == CFG_SC) && (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U))
        /* When a trusted function requires deferred execution for hook time protection, 
        reschedule the task and terminate it. */
        Os_TrustedFuncTpDataDef *trustedFuncTp = &pScb->Os_TrustedFuncTp;
        if (TRUE == trustedFuncTp->TrustedFuncTporFlag)
        {
            trustedFuncTp->TrustedFuncTporFlag = FALSE;
            pScb->SysDispatchLocker = 0U;
            Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
        }
#endif
        /* When the time-protection HOOK is triggered normally, set the flag here 
        and terminate the Task upon exiting the time interrupt. */
        pScb->ProtectionTerminateObj = OS_PROTECTION_TERMINATE_TASK;
#else
        pScb->SysDispatchLocker = 0U;
        Os_Hal_Dispatch();
#endif
    }

    UNUSED_PARAMETER(errType);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ISR2_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Release critical resource while terminating current ISR2.
 */
OS_LOCAL void Os_ProtIsr2CriticalRelease(const Os_ICBType *pIcb, Os_SCBType *pScb)
{
/* Terminate resource. */
/* OESK 8.2:
   In case of multiple resource occupation within one task, the user has to request and release
   resources following the LIFO principle (stack like). */
#if (CFG_SPINLOCK_MAX > 0U)
    for (uint16 CountIdx = pIcb->Isr2CriticalZoneCount; CountIdx > 0u; CountIdx--)
    {
        uint16 CurrentCount = pIcb->Isr2CriticalZoneCount - 1u;
        if (OS_OBJECT_SPINLOCK == pIcb->Isr2CriticalZoneType[CurrentCount])
        {
            SpinlockIdType SpinlockId = pIcb->Isr2CriticalZoneStack[CurrentCount]; /* PRQA S 4442 */ /* VL_Os_4442 */
            (void)Os_ReleaseSpinlock(pScb, SpinlockId);
        }
        else
        {
            ResourceType ResourceId = pIcb->Isr2CriticalZoneStack[CurrentCount]; /* PRQA S 4442 */ /* VL_Os_4442 */
            (void)Os_ReleaseResource(pScb, ResourceId);
        }
    }
#elif (CFG_STD_RESOURCE_MAX > 0U)
    if ((uint16)pIcb->IsrC2ResCount > 0u)
    {
        for (uint16 i = (uint16)pIcb->IsrC2ResCount; i > 0u; i--)
        {
            (void)Os_ReleaseResource(pScb, pIcb->IsrC2ResourceStack[i - 1u]);
        }
    }
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate isr cat2 in protection hook.
 */
/* PRQA S 6070, 3006, 6030 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3006, VL_MTR_Os_STMIF */
OS_LOCAL void Os_ProtHookTerminateIsrC2(StatusType errType, Os_SCBType *pScb)
/* PRQA S 6070, 3006, 6030 -- */
{
    if (pScb->IntNestISR2 >= OS_TMPROT_TARGET_ISR_OFFSET)
    {
        pScb->ProtectionTerminateObj = OS_PROTECTION_TERMINATE_NESTED_ISR;
    }

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_IsrType RunIsrId = pScb->SysRunningIsrCat2Id;

    /* In timing protection. */
    if ((E_OS_PROTECTION_TIME == errType) || (E_OS_PROTECTION_LOCKED == errType))
    {

        if (pScb->IntNestISR2 >= OS_TMPROT_TARGET_ISR_OFFSET)
        {
            pScb->ProtectionTerminateObj = OS_PROTECTION_TERMINATE_TPROT_ISR;

            pScb->SysRunningIsrCat2Id =
                pScb->SysIsrNestQueue[pScb->IntNestISR2 - OS_TMPROT_TARGET_ISR_OFFSET];
        }
    }
#endif

#if ((CFG_STD_RESOURCE_MAX > 0U) || (CFG_SPINLOCK_MAX > 0U))
    const Os_ICBType *pIcb = Os_ICB[pScb->SysRunningIsrCat2Id];

    Os_ProtIsr2CriticalRelease(pIcb, pScb);
#endif

    Os_ProtHookIsrResumeInt(pScb->SysRunningIsrCat2Id, pScb);
    
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    /* Stop isr exe budget. */
    Os_TmProtIsrEnd(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_EXE);
    pScb->SysRunningIsrCat2Id = RunIsrId;
    /* Set terminate_isr flag. */

    /* Timing protection do task_dispatch at the end of tm_prot_isr. */
    if ((E_OS_PROTECTION_LOCKED != errType) && (E_OS_PROTECTION_TIME != errType) && (E_OS_STACKFAULT != errType))
    {
        /* PRQA S 2743, 3442, 0306, 3678, 1006 ++ */ /* VL_Os_2743, VL_Os_3442, VL_Os_0306, VL_Os_3678, VL_Os_1006 */
        /* PRQA S 1520, 1021, 1512, 1513, 1753 ++ */ /* VL_Os_1520, VL_Os_1021, VL_Os_1512, VL_Os_1513, VL_Os_1753 */
        OS_HAL_ISR2_EPILOGUE(RunIsrId);
        /* PRQA S 1520, 1021, 1512, 1513, 1753 -- */
        /* PRQA S 2743, 3442, 0306, 3678, 1006 -- */
    }
#else
    if (E_OS_STACKFAULT != errType)
    {
        /* PRQA S 2743, 3442, 0306, 3678, 1006, 1021 ++ */ /* VL_Os_2743, VL_Os_3442, VL_Os_0306, VL_Os_3678, VL_Os_1006, VL_Os_1021 */
        /* PRQA S 1520, 1753, 1512, 1513 ++ */ /* VL_Os_1520, VL_Os_1753, VL_Os_1512, VL_Os_1513 */
        OS_HAL_ISR2_EPILOGUE(pScb->SysRunningIsrCat2Id);
        /* PRQA S 1520, 1753, 1512, 1513 -- */
        /* PRQA S 2743, 3442, 0306, 3678, 1006, 1021 -- */
    }
#endif

    UNUSED_PARAMETER(errType);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * PRO_TERMINATETASKISR: terminate task or isr in hook.
 */
OS_LOCAL void Os_ProtHookTermTaskIsr(uint32 whoHook,
                                     StatusType errType, Os_SCBType *pScb)
{
    if (OS_TMPROT_HOOK_TASK == whoHook)
    {
        Os_ProtHookTerminateTask(errType, pScb);
    }

#if (CFG_ISR2_MAX > 0)
    if (OS_TMPROT_HOOK_ISR == whoHook)
    {
        Os_ProtHookTerminateIsrC2(errType, pScb); /* PRQA S 1520 */ /* VL_Os_1520 */
    }
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/* Terminate os_app process. */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate os_app process
 */
OS_LOCAL void Os_ProtHookTermAppl(StatusType errType, RestartType restartOption,
                                  uint32 whoHook, Os_SCBType *pScb)
{
    Os_ApplicationType RunApplIdTemp = pScb->SysRunningAppId;

/* In timing protection, the running_application maybe SYS_APP. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if ((E_OS_PROTECTION_LOCKED == errType) || (E_OS_PROTECTION_TIME == errType))
    {
        if (OS_TMPROT_HOOK_ISR == whoHook)
        {
            /* Update running_application_id. */
            if (pScb->IntNestISR2 >= OS_TMPROT_TARGET_ISR_OFFSET)
            {
                Os_IsrType IsrId = pScb->SysIsrNestQueue[pScb->IntNestISR2 - OS_TMPROT_TARGET_ISR_OFFSET];
                if (Os_CheckIsr2Id(IsrId))
                {
                    pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_ISR, (Os_AppObjectId)IsrId);
                }
            }
        }
        else if (OS_TMPROT_HOOK_TASK == whoHook) /* PRQA S 2004 */ /* VL_Os_2004 */
        {
            pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_TASK, (Os_AppObjectId)pScb->SysRunningTaskId);
        }
    }
#endif

    /*If the ProtectionHook returns PRO_TERMINATEAPPL_RESTART and no RestartTask was configured
     *for the faulty OS-Application, ShutdownOS is called.*/
    if ((!CheckCoreTaskId(Os_AppCfg[pScb->SysRunningAppId].RestartTask, pScb->SysCore)) &&
        (OS_RESTART == restartOption))
    {
        Os_ShutdownOS(errType, OS_SHUTDOWN_OS);
    }
    else
    {
        /* Call TerminateApplication. */
        Os_TerminateApplication(pScb, pScb->SysRunningAppId, restartOption); /* PRQA S 1520 */ /* VL_Os_1520 */
        pScb->SysRunningAppId = RunApplIdTemp;
    }

    UNUSED_PARAMETER(whoHook);
    UNUSED_PARAMETER(errType);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * check SysRunningAppId, then call Os_ProtHookTermAppl
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_ProtTermAppl(StatusType errType, Os_RestartType restartOpt,
                              uint32 whoHook, Os_SCBType *pScb)
/* PRQA S 3673 -- */
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if (Os_CheckCoreAppId(pScb->SysRunningAppId, pScb->SysCore))
#else
    /* PRQA S 1880, 4521 ++ */ /* VL_Os_1880, VL_Os_4521 */
    if ((pScb->SysRunningAppId >= Os_AppIdRange[pScb->SysCore].start) &&
        (pScb->SysRunningAppId < (Os_AppIdRange[pScb->SysCore].end - 1U)))
    /* PRQA S 1880, 4521 -- */
#endif
    {
        Os_ProtHookTermAppl(errType, restartOpt, whoHook, pScb); /* PRQA S 1520 */ /* VL_Os_1520 */
    }
    else
#endif
    {
        UNUSED_PARAMETER(restartOpt);
        UNUSED_PARAMETER(whoHook);
        UNUSED_PARAMETER(pScb);
        Os_ShutdownOS(errType, OS_SHUTDOWN_OS);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ProtectionHook process return code of ProtectionHook()
 */
OS_LOCAL void Os_ProtReturnProcess(ProtectionReturnType Ret, StatusType errType,
                                   uint32 whoHook, Os_SCBType *pScb)
{
    switch (Ret)
    {
    case PRO_IGNORE:
        /* SWS_Os_00475 */
        if (E_OS_PROTECTION_ARRIVAL != errType)
        {
            Os_ShutdownOS(errType, OS_SHUTDOWN_OS);
        }
        break;

    case PRO_TERMINATETASKISR:
        if ((boolean)E_OK == Os_CheckRunningTaskIDandIsr2ID(whoHook, pScb))
        {
            Os_ProtHookTermTaskIsr(whoHook, errType, pScb); /* PRQA S 1520 */ /* VL_Os_1520 */
            break; /* PRQA S 3333 */ /* VL_Os_3333 */
        }
        /*OS243
        if the reaction is to forcibly terminate the Task/Category 2 OsIsr and no Task
        or OsIsr can be associated with the error,the running OS-Application is forcibly
        terminated by the Operating System
        */
        Os_ProtTermAppl(errType, OS_NO_RESTART, whoHook, pScb); /* PRQA S 1520 */ /* VL_Os_1520 */
        break;

    case PRO_TERMINATEAPPL:
        /*OS244
         * If the reaction is to forcibly terminate the faulty OS-Application
         * and no OS-Application can be assigned,ShutdownOS() is called.
         */
        Os_ProtTermAppl(errType, OS_NO_RESTART, whoHook, pScb); /* PRQA S 1520 */ /* VL_Os_1520 */
        break;

    case PRO_TERMINATEAPPL_RESTART:
        Os_ProtTermAppl(errType, OS_RESTART, whoHook, pScb); /* PRQA S 1520 */ /* VL_Os_1520 */
        break;

    case PRO_SHUTDOWN:
        Os_ShutdownOS(errType, OS_SHUTDOWN_OS);
        break;
        
    case PRO_RESUME:
        break;
              
    default:
        /* OS107: ShutdownOS. */
        Os_ShutdownOS(errType, OS_SHUTDOWN_OS);
        break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ProtectionHook called by OS.
 */
/* PRQA S 1503, 1532 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
Os_ProtectionReturnType Os_CallProtectionHook(StatusType errType, uint32 whoHook)
/* PRQA S 1503, 1532 -- */
{
    Os_ProtectionReturnType osRet;
    Os_SCBType *pScb = Os_GetCurrentContext();

#if (TRUE == CFG_PROTECTIONHOOK)
    Os_CallLevelType osSameLevel;
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    osSameLevel = pScb->SysOsLevel;
    pScb->SysOsLevel = OS_LEVEL_PROTECTIONHOOK;
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ProtectionHook_Start, errType);
    /* PRQA S 1841, 4544, 4542, 4543, 1277 -- */
    /* PRQA S 3138, 3141 -- */
    osRet = ProtectionHook(errType);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1841, 4544, 4542, 4543, 1277 ++ */ /* VL_Os_1841, VL_Os_4544, VL_Os_4542, VL_Os_4543, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_HOOK, Os, pScb->SysCore, OsHook_ProtectionHook_Return, osRet);
    /* PRQA S 1841, 4544, 4542, 4543, 1277 -- */
    /* PRQA S 3138, 3141 -- */
    pScb->SysOsLevel = osSameLevel;
    OS_HAL_EXIT_CRITICAL();

    if ((E_OS_PROTECTION_ARRIVAL == errType) && (PRO_IGNORE != osRet) && 
         (PRO_RESUME != osRet))
    {
        /* SWS_Os_00506 */
        osRet = PRO_SHUTDOWN;
    }

#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
    Os_TrustedFuncCallerCheck(errType, whoHook, pScb, &osRet);
#endif

    /* Hook post process. */
    Os_ProtReturnProcess(osRet, errType, whoHook, pScb); /* PRQA S 1520 */ /* VL_Os_1520 */
#else
    UNUSED_PARAMETER(whoHook);
    /* OS107: ShutdownOS. */
    Os_ShutdownOS(errType, OS_SHUTDOWN_OS);
    osRet = PRO_SHUTDOWN;
#endif

    return osRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/

/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
