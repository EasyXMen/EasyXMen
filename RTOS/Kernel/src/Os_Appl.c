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
 **  @file               : Os_Appl.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Application API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Appl.h"
#include "Os_Task.h"
#include "Os_Counter.h"
#include "Os_Interrupt.h"
#include "Os_Resource.h"
#include "Os_ReadyQue.h"
#include "Os_Tprot.h"
#include "Os_Spinlock.h"
#include "Os_Alarm.h"
#include "Os_ScheduleTable.h"
#include "Os_Sprot.h"
#include "Os_Rpc.h"
#include "Os_Counter.h"
#include "Os_Event.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (CFG_OSAPPLICATION_MAX > 0U)
/**
 * @brief           Retrieves the access configuration for a specified object
 * @param[in]       objectType: Type of the object to check
 * @param[in]       objectId: ID of the object to check
 * @return          const Os_ObjectAppCfgType*
 * @retval          Pointer to object access configuration if found
 * @retval          NULL_PTR if not found
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL const Os_ObjectAppCfgType *Os_GetObjectAccess(ObjectTypeType objectType, AppObjectId objectId);

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_STD_RESOURCE_MAX > 0)
/**
 * @brief           Releases a resource occupied by a task
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       appObjId: Task ID that owns the resource
 * @param[in]       ceilPrio: Ceiling priority of the resource
 * @param[in]       savePrio: Original priority to restore
 * @return          StatusType
 * @retval          E_OK: Resource successfully released
 * @retval          E_NOT_OK: Resource cannot be released
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_ApplReleaseResourceTask(const Os_SCBType *pScb, Os_AppObjectIdType appObjId, Os_PriorityType ceilPrio, Os_PriorityType savePrio);

/**
 * @brief           Releases a resource occupied by task or ISR
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       object: Type of object (TASK or ISR)
 * @param[in]       appObjId: Task or ISR ID that owns the resource
 * @param[in]       ceilPrio: Ceiling priority of the resource
 * @param[in]       savePrio: Original priority to restore
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ApplReleaseResourceTaskOrISR(Os_SCBType *pScb, ObjectTypeType object, Os_AppObjectIdType appObjId, Os_PriorityType ceilPrio, Os_PriorityType savePrio);

/**
 * @brief           Releases a resource occupied by an ISR
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       appObjId: ISR ID that owns the resource
 * @param[in]       ceilPrio: Ceiling priority of the resource
 * @param[in]       savePrio: Original priority to restore
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ApplReleaseResourceISR(const Os_SCBType *pScb, Os_AppObjectIdType appObjId, Os_PriorityType ceilPrio, Os_PriorityType savePrio);

/**
 * @brief           Releases a resource occupied by the application object
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       resId: Resource ID to release
 * @param[in]       object: Type of object (TASK or ISR)
 * @param[in]       appObjId: Task or ISR ID that owns the resource
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ApplReleaseResource(Os_SCBType *pScb, ResourceType resId, ObjectTypeType object, Os_AppObjectIdType appObjId);
#endif

/**
 * @brief           Resumes ISR interrupt locks
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       isrId: ISR ID to resume
 * @param[inout]    pICB: Pointer to ISR control block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ApplResumeISRLockInt(Os_SCBType *pScb, Os_IsrType isrId, Os_ICBType *pICB);

#if (CFG_ISR_MAX > 0)
/**
 * @brief           Terminates an ISR and processes kernel-related cleanup
 * @param[in]       isrId: ISR ID to terminate
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_AppTerminateIsrKernelProc(Os_IsrType isrId);

#if (TRUE == CFG_INT_NEST_ENABLE)
/**
 * @brief           Terminates an ISR and handles nest counters
 * @param[in]       isrId: ISR ID to terminate
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_AppTerminateIsrProc(Os_IsrType isrId);
#endif

/**
 * @brief           Terminates a group of ISRs belonging to an application
 * @param[in]       appIsrRef: Array of ISR IDs to terminate
 * @param[in]       appIsrCnt: Count of ISRs in the array
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TerminateISRObjectHandler(const Os_IsrType *appIsrRef, Os_IsrType appIsrCnt);
#endif

/**
 * @brief           Releases all resources occupied by a task
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       taskId: Task ID whose resources need to be released
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ApplReleaseAllResource(Os_SCBType *pScb, Os_TaskType taskId);

#if (CFG_SPINLOCK_MAX > 0U)
/**
 * @brief           Releases all spinlocks held by a task
 * @param[in]       pScb: Pointer to system control block
 * @param[inout]    pTcb: Pointer to task control block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ApplReleaseAllSpinlock(Os_SCBType *pScb, Os_TCBType *pTcb);
#endif

/**
 * @brief           Resumes task interrupt locks
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       taskId: Task ID to resume
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_AppResumeTaskLockInt(Os_SCBType *pScb, Os_TaskType taskId);

/**
 * @brief           Terminates one task
 * @param[in]       taskId: Task ID to terminate
 * @return          StatusType
 * @retval          E_OK: Task successfully terminated
 * @retval          E_NOT_OK: Task cannot be terminated
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_ApplTerminateOneTask(Os_TaskType taskId);

/**
 * @brief           Terminates a group of tasks belonging to an application
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       appTaskRef: Array of task IDs to terminate
 * @param[in]       appTaskCnt: Count of tasks in the array
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TerminateTaskObjectHandler(const Os_SCBType *pScb, const Os_TaskType *appTaskRef, Os_TaskType appTaskCnt);

#if (CFG_ALARM_MAX > 0)
/**
 * @brief           Terminates a group of alarms belonging to an application
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       appAlarmRef: Array of alarm IDs to terminate
 * @param[in]       appAlarmCnt: Count of alarms in the array
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TerminateAlarmObjectHandler(const Os_SCBType *pScb, const Os_AlarmType *appAlarmRef, Os_AlarmType appAlarmCnt);
#endif

#if (CFG_SCHEDTBL_MAX > 0)
/**
 * @brief           Terminates a group of schedule tables belonging to an application
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       appSchTblRef: Array of schedule table IDs to terminate
 * @param[in]       appSchTblCnt: Count of schedule tables in the array
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TerminateScheduleTblObjectHandler(Os_SCBType *pScb, const Os_ScheduleTableType *appSchTblRef, Os_ScheduleTableType appSchTblCnt);
#endif

/**
 * @brief           Terminates all objects belonging to an application
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       curAppCfg: Pointer to application configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TerminateObjects(Os_SCBType *pScb, const Os_ApplicationCfgType *curAppCfg);

#if (CFG_ISR_MAX > 0)
/**
 * @brief           Disables all interrupts belonging to an application
 * @param[in]       posCurAppCfg: Pointer to application configuration
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_DisableIntInApp(const Os_ApplicationCfgType *posCurAppCfg);
#endif

#if (OS_AUTOSAR_CORES > 1)
/**
 * @brief           RPC action handler for TerminateApplication
 * @param[in]       inPara: Parameter array containing application ID and restart option
 * @return          StatusType
 * @retval          E_OK: Operation successful
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_TerminateApplication(uint32 *inPara);

/**
 * @brief           Makes RPC call to TerminateApplication on remote core
 * @param[in]       ownerCore: Core ID to execute the call
 * @param[in]       Application: Application ID to terminate
 * @param[in]       RestartOption: Whether to restart the application
 * @return          StatusType
 * @retval          E_OK: Remote call completed successfully
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_TerminateApplication(Os_CoreIdType ownerCore, ApplicationType Application, RestartType RestartOption);

/**
 * @brief           Internal implementation of OS service:CheckObjectOwnership
 * @param[in]       pScb: Pointer to system control block
 * @param[in]       object: Type of object (TASK or ISR)
 * @param[in]       appObjId: Task or ISR ID that owns the resource
 * @return          ApplicationType
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL ApplicationType Os_CheckObjectOwnership(const Os_SCBType *pScb, ObjectTypeType objectType, AppObjectId objectId);
#endif
#endif
#endif

/* ========================================== external function definitions ========================================= */
#if (CFG_OSAPPLICATION_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the Application control block
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitApplication(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_SCBType *pScb = Os_GetSystemContext(coreId);

    pScb->SysAppId = Os_AppSysId[coreId];/* PRQA S 4424, 2842 */ /* VL_Os_4424, VL_Os_2842 */
    pScb->SysRunningAppId = pScb->SysAppId;

    if (OS_CORE_ID_MASTER == coreId)
    {
        for (ApplicationType i = 0u; i < CFG_OSAPPLICATION_MAX; i++)
        {
            Os_AppCB[i].AppState = OS_APPLICATION_ACCESSIBLE;
            /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
            /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
            /* PRQA S 0499, 4543, 1861, 4397, 1277 ++ */ /* VL_Os_0499, VL_Os_4543, VL_Os_1861, VL_Os_4397, VL_Os_1277 */
            ARTI_TRACE(NOSUSP, AR_CP_OS_APPLICATION, Os, pScb->SysCore, OsApplication_Start, i);
            /* PRQA S 0499, 4543, 1861, 4397, 1277 -- */
            /* PRQA S 1821, 4532, 4544, 4542 -- */
            /* PRQA S 3138, 3141 -- */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
OS_LOCAL const Os_ObjectAppCfgType *Os_GetObjectAccess(
    ObjectTypeType objectType,
    AppObjectId objectId)
{
    const Os_ObjectAppCfgType *pObjectAcc = NULL_PTR;

    switch (objectType)
    {
#if (CFG_TASK_MAX > 0U)
    case OS_OBJECT_TASK:
    {
        pObjectAcc = Os_TaskCfg[objectId].ObjAppCfg;
    }
    break;
#endif
#if (CFG_ISR_MAX > 0U)
    case OS_OBJECT_ISR:
    {
        pObjectAcc = Os_IsrCfg[objectId].ObjAppCfg;
    }
    break;
#endif
#if (CFG_ALARM_MAX > 0U)
    case OS_OBJECT_ALARM:
    {
        pObjectAcc = Os_AlarmCfg[objectId].ObjAppCfg;
    }
    break;
#endif
#if (CFG_COUNTER_MAX > 0U)
    case OS_OBJECT_COUNTER:
    {
        pObjectAcc = Os_CounterCfg[objectId].ObjAppCfg;
    }
    break;
#endif
#if (CFG_SCHEDTBL_MAX > 0U)
    case OS_OBJECT_SCHEDULETABLE:
    {
        pObjectAcc = Os_SchedTblCfg[objectId].ObjAppCfg;
    }
    break;
#endif
#if (CFG_STD_RESOURCE_MAX > 0U)
    case OS_OBJECT_RESOURCE:
    {
        pObjectAcc = Os_ResourceCfg[objectId].ObjAppCfg;
    }
    break;
#endif
#if (CFG_SPINLOCK_MAX > 0U)
    case OS_OBJECT_SPINLOCK:
    {
        pObjectAcc = Os_SpinlockCfg[objectId].ObjAppCfg;
    }
#endif
    break;/* PRQA S 2880 */ /* VL_Os_2880 */
    default: /* Nothing to do. */
        break;
    }
    return pObjectAcc;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * release the resource occupied by task
 */
OS_LOCAL StatusType Os_ApplReleaseResourceTask(
    const Os_SCBType *pScb,
    Os_AppObjectIdType appObjId,
    Os_PriorityType ceilPrio,
    Os_PriorityType savePrio)
{
    StatusType err = E_OK;
    Os_TCBType *pTcb = Os_TCB[appObjId];
    const Os_TaskCfgType *pTaskCfg = &Os_TaskCfg[appObjId];

    if (pTaskCfg->TaskPriority > ceilPrio)
    {
        err = E_NOT_OK;
    }
    else
    {
        if (pTcb->TaskResCount > 0u)
        {
            pTcb->TaskResCount = pTcb->TaskResCount - 1u;
        }

        if (ceilPrio > savePrio)
        {
            Os_ReadyQueueRemove(pScb->QueueMg,
                                pTcb->TaskRunPrio);
            pTcb->TaskRunPrio = pTaskCfg->TaskPriority;
        }
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * release the resource occupied by task or ISR
 */
OS_LOCAL void Os_ApplReleaseResourceTaskOrISR(
    Os_SCBType *pScb,
    ObjectTypeType object,
    Os_AppObjectIdType appObjId,
    Os_PriorityType ceilPrio,
    Os_PriorityType savePrio)
{
    if (OS_OBJECT_TASK == object)
    {
        Os_TCBType *pTcb = Os_TCB[appObjId];

        if (pTcb->TaskResCount > 0u)
        {
            pTcb->TaskResCount = pTcb->TaskResCount - 1u;
        }

        Os_Hal_SetIpl(Os_PrioToIpl(pScb, savePrio), OS_ISR_ENABLE);
        pScb->SysDispatchLocker = pScb->SysDispatchLocker - 1u;
    }
    else if (OS_OBJECT_ISR == object) /* PRQA S 2004 */ /* VL_Os_2004 */
    {
        Os_ICBType *pIcb = Os_ICB[appObjId];

#if (TRUE == CFG_INT_NEST_ENABLE)
        if (ceilPrio > savePrio)
        {
            Os_Hal_SetIpl(Os_PrioToIpl(pScb, savePrio), OS_ISR_ENABLE);
        }
#endif

/* AutoSar: add resId to stack. For protection hook. */
#if (CFG_ISR2_MAX > 0)
        if ((uint16)pIcb->IsrC2ResCount > 0u)
        {
            /* PRQA S 4521, 4442 ++ */ /* VL_Os_4521, VL_Os_4442 */
            pIcb->IsrC2ResCount = pIcb->IsrC2ResCount - 1u;
            /* PRQA S 4521, 4442 -- */
        }
#endif
    }

    UNUSED_PARAMETER(ceilPrio);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/**
 * release the resource occupied by ISR
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ApplReleaseResourceISR(
    const Os_SCBType *pScb,
    Os_AppObjectIdType appObjId,
    Os_PriorityType ceilPrio,
    Os_PriorityType savePrio)
{
#if (TRUE == CFG_INT_NEST_ENABLE)
    if (ceilPrio > savePrio)
    {
        Os_Hal_SetIpl(Os_PrioToIpl(pScb, savePrio), OS_ISR_ENABLE);
    }
#endif

/* AutoSar: add resId to stack. For protection hook. */
#if (CFG_ISR2_MAX > 0)
    Os_ICBType *pIcb = Os_ICB[appObjId];
    if ((uint16)pIcb->IsrC2ResCount > 0u)
    {
        /* PRQA S 4521, 4442 ++ */ /* VL_Os_4521, VL_Os_4442 */
        pIcb->IsrC2ResCount = pIcb->IsrC2ResCount - 1u;
        /* PRQA S 4521, 4442 -- */
    }
#endif

    UNUSED_PARAMETER(ceilPrio);
    UNUSED_PARAMETER(savePrio);
    UNUSED_PARAMETER(pScb);
}

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * task or ISR terminate its resource occupied before
 */
OS_LOCAL void Os_ApplReleaseResource(
    Os_SCBType *pScb,
    ResourceType resId,
    ObjectTypeType object,
    Os_AppObjectIdType appObjId)
{
    StatusType err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* No any resource to release */
    if (0U == Os_RCB[resId]->SaveCount)
    {
        /*nothing to do*/
    }
    else
#endif
    {
        Os_PriorityType ceilPrio = Os_ResourceCfg[resId].Ceiling;
        Os_PriorityType savePrio = Os_RCB[resId]->SavePrio;

        switch (Os_ResourceCfg[resId].ResourceOccupyType)
        {
        case OS_RES_OCCUPIED_BY_TASK:
            err = Os_ApplReleaseResourceTask(pScb, appObjId, ceilPrio, savePrio);
            break;

        case OS_RES_OCCUPIED_BY_TASK_OR_INTERRUPT:
            Os_ApplReleaseResourceTaskOrISR(pScb, object, appObjId, ceilPrio, savePrio);
            break;

        case OS_RES_OCCUPIED_BY_INTERRUPT:
            Os_ApplReleaseResourceISR(pScb, appObjId, ceilPrio, savePrio);
            break;

        /*add comments to pass QAC.*/
        default:
            /* Nothing to do */
            break;
        }

        if ((StatusType)E_OK == err)
        {
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
            Os_TmProtResEnd(pScb->SysCore, resId);
#endif

            Os_RCB[resId]->SaveCount = 0u;
            Os_RCB[resId]->SavePrio = OS_PRIORITY_INVALID;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
OS_LOCAL void Os_ApplResumeISRLockInt(
    Os_SCBType *pScb,
    Os_IsrType isrId,
    Os_ICBType *pICB
)
{
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtIsrEnd(pScb->SysCore, isrId, TP_EXE);
#endif

    if (pICB->IsrC2DisableAllCount > 0u)
    {
        pICB->IsrC2DisableAllCount = 0u;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtIsrEnd(pScb->SysCore, isrId, TP_DIS_ALL_INT);
        Os_Hal_EnableAllInt_ButTimingProtInt(pScb->SysCore);
#else
        Os_Hal_RestoreInt(pScb->SaveAllInt);
#endif
    }

    if (pICB->IsrC2SuspendAllCount > 0u)
    {
        pScb->SuspendAllCount -= pICB->IsrC2SuspendAllCount;
        pICB->IsrC2SuspendAllCount = 0u;
        if (0u == pScb->SuspendAllCount)
        {
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
            Os_TmProtIsrEnd(pScb->SysCore, isrId, TP_SUS_ALL_INT);
            Os_Hal_EnableAllInt_ButTimingProtInt(pScb->SysCore);
#else
            Os_Hal_RestoreInt(pScb->SaveAllIntNested);
#endif
        }
    }

    if (pICB->IsrC2SuspendOSCount > 0u)
    {
        pScb->SuspendOsCount -= pICB->IsrC2SuspendOSCount;
        pICB->IsrC2SuspendOSCount = 0u;
        if (0u == pScb->SuspendOsCount)
        {
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
            Os_TmProtIsrEnd(pScb->SysCore, isrId, TP_SUS_OS_INT);
#endif
            Os_Hal_SetIpl(pScb->SaveOsIntNested, OS_ISR_ENABLE);

        }
    }
    UNUSED_PARAMETER(isrId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ISR_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * free the ISR running control block and the resources occupied
 */
OS_LOCAL void Os_AppTerminateIsrKernelProc(Os_IsrType isrId)
{
    Os_SCBType *pScb = OS_ISR_GET_SCB(isrId);

    if (Os_CheckIsr2Id(isrId))
    {
#if ((TRUE == CFG_TIMING_PROTECTION_ENABLE) || (CFG_STD_RESOURCE_MAX > 0) || (CFG_SPINLOCK_MAX > 0U))
        Os_ICBType *pICB = Os_ICB[isrId];
#endif

        Os_ApplResumeISRLockInt(pScb, isrId, pICB);

/* Release resource occupied by this isr. */
#if (CFG_STD_RESOURCE_MAX > 0)
        for (uint16 i = (uint16)pICB->IsrC2ResCount; i > 0u; i--)
        {
            Os_ApplReleaseResource(pScb,
                                   pICB->IsrC2ResourceStack[i - 1u],/* PRQA S 4521 */ /* VL_Os_4521 */
                                   OS_OBJECT_ISR,
                                   (Os_AppObjectIdType)isrId);
        }
#endif

#if (CFG_SPINLOCK_MAX > 0U)
        for (SpinlockIdType spinLockIdx = pICB->Isr2CriticalZoneCount; (uint16)spinLockIdx > 0u; spinLockIdx--) /* PRQA S 4442, 4527 */ /* VL_Os_4442, VL_Os_4527 */
        {
            if (OS_OBJECT_SPINLOCK == pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount - 1u])
            {
                SpinlockIdType SpinlockId = pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount - 1u]; /* PRQA S 4442 */ /* VL_Os_4442 */
                (void)Os_ReleaseSpinlock(pScb, SpinlockId);
            }
            else
            {
                pICB->Isr2CriticalZoneCount--;
            }
        }
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_INT_NEST_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate the ISR objects of the calling application
 */
/* PRQA S 6010, 6030, 6080, 3006 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STCAL, VL_MTR_Os_STPTH, VL_Os_3006 */
OS_LOCAL void Os_AppTerminateIsrProc(Os_IsrType isrId)
/* PRQA S 6010, 6030, 6080, 3006 -- */
{
    StatusType ulRet = E_OK;

    /* Isr_nest process. */
    /* Just find out the nested corresponding ISR2s(excluding the running one) */
    Os_SCBType *pScb = OS_ISR_GET_SCB(isrId);
    if (isrId != pScb->SysRunningIsrCat2Id)
    {
        /* Terminate one isr */
        uint16 i;
        for (i = 0u; i < pScb->IntNestISR2; i++)
        {
            /* find out the Isr nested position and reclaim its context saving area */
            if (isrId == pScb->SysIsrNestQueue[i])
            {
                /* Reclaim csa list used by this isr. */
                /* if OsIsrChkindex == Os_IntNestISR2, the isr is the innermost_isr,
                * can not reclaim csas here */
                /* PRQA S 0310, 3138, 1006, 3345 ++ */ /* VL_Os_0310, VL_Os_3138, VL_Os_1006, VL_Os_3345 */
                /* PRQA S 0404, 0306, 3442, 2743 ++ */ /* VL_Os_0404, VL_Os_0306, VL_Os_3442, VL_Os_2743 */
                OS_HAL_TERMINATE_NESTED_ISR(i);/* PRQA S 3432, 1021 */ /* VL_Os_3432, VL_Os_1021 */
                /* PRQA S 0404, 0306, 3442, 2743 -- */
                /* PRQA S 0310, 3138, 1006, 3345 -- */
                break;
            }
        }

        /* Not find out the Isr Id in the IsrStack from the SCB,
        * not include outermost_isr. */
        if (i >= pScb->IntNestISR2)
        {
            ulRet = E_NOT_OK;
        }
        else
        {
            /* Moving the following nested ISR2s to shift one position left */ /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            while (i < (pScb->IntNestISR2 - 1u)) /* PRQA S 1891 */ /* VL_Os_1891 */
            {
                pScb->SysIsrNestQueue[i] = pScb->SysIsrNestQueue[i + 1u];

                if (i < (pScb->IntNestISR2 - 2u)) /* PRQA S 1891 */ /* VL_Os_1891 */
                {
                    /* PRQA S 3432 ++ */ /* VL_Os_3432 */
                    OS_HAL_PROCESS_NESTED_ISR_STACK(i); /* PRQA S 2743*/ /* VL_Os_2743*/
                    /* PRQA S 3432 -- */
                }

                i++;
            }
            /* PRQA S 3120 -- */
        }

        if (E_OK == ulRet)
        {
            pScb->IntNestISR2--;

            if (pScb->SysDispatchLocker > 0u)
            {
                /*pScb->SysDispatchLocker--*/
                pScb->SysDispatchLocker = pScb->SysDispatchLocker - 1u;
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate the ISR objects of the calling application
 */
OS_LOCAL void Os_TerminateISRObjectHandler(
    const Os_IsrType *appIsrRef,
    Os_IsrType appIsrCnt)
{
    if ((0u == (uint16)appIsrCnt) || (Os_CfgIsrMax_Inf[Os_GetCoreIdLocal()] < (uint16)appIsrCnt))
    {
        /*nothing to do*/
    }
    else
    {
        for (uint16 OsIsrLoopi = 0U; OsIsrLoopi < (uint16)appIsrCnt; OsIsrLoopi++)
        {
/* MISRA-C: 17.4  Array subscripting applied to an object of pointer type.
   appIsrRef is pointor of static_cfg_array, appIsrCnt is the size of array,
   it must be used like below. */

/* Arch platform related process. */
#if (TRUE == CFG_INT_NEST_ENABLE)
            Os_AppTerminateIsrProc(appIsrRef[OsIsrLoopi]);
#endif
            Os_AppTerminateIsrKernelProc(appIsrRef[OsIsrLoopi]);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ApplReleaseAllResource(
    Os_SCBType *pScb,
    Os_TaskType taskId)
{
#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    Os_ReleaseInternalResource(pScb, taskId);
#endif

/* Release resource occupied by this task. */
#if (CFG_STD_RESOURCE_MAX > 0U)
    const Os_TCBType *pTcb = Os_TCB[taskId];
    for (uint16 i = pTcb->TaskResCount; i > 0u; i--)
    {
        Os_ApplReleaseResource(pScb,
                               pTcb->TaskResourceStack[i - 1u],
                               OS_OBJECT_TASK, (Os_AppObjectIdType)taskId);
    }
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
#if (CFG_SPINLOCK_MAX > 0U)
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ApplReleaseAllSpinlock(
    Os_SCBType *pScb,
    Os_TCBType *pTcb)
{
    for (SpinlockIdType spinLockIdx = pTcb->TaskCriticalZoneCount; (uint16)spinLockIdx > 0u; spinLockIdx--) /* PRQA S 4442, 4527 */ /* VL_Os_4442, VL_Os_4527 */
    {
        if (OS_OBJECT_SPINLOCK == pTcb->TaskCriticalZoneType[pTcb->TaskCriticalZoneCount - 1u])
        {
            SpinlockIdType SpinlockId = pTcb->TaskCriticalZoneStack[pTcb->TaskCriticalZoneCount - 1u]; /* PRQA S 4442 */ /* VL_Os_4442 */
            (void)Os_ReleaseSpinlock(pScb, SpinlockId);
        }
        else
        {
            pTcb->TaskCriticalZoneCount--;
        }
    }
}
#endif
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
OS_LOCAL void Os_AppResumeTaskLockInt(Os_SCBType *pScb, Os_TaskType taskId)
{
    Os_TCBType *pTcb = Os_TCB[taskId];

    /* Timing protection. */

    /* Task_exe budget. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtTaskEnd(pScb->SysCore, taskId, TP_EXE);
#endif

    if (pTcb->TaskDisableAllCount > 0u)
    {
        pTcb->TaskDisableAllCount = 0u;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtTaskEnd(pScb->SysCore, taskId, TP_DIS_ALL_INT);
        Os_Hal_EnableAllInt_ButTimingProtInt(pScb->SysCore);
#else
        Os_Hal_RestoreInt(pScb->SaveAllInt);
#endif
    }

    if (pTcb->TaskSuspendAllCount > 0u)
    {
        pScb->SuspendAllCount -= pTcb->TaskSuspendAllCount;
        pTcb->TaskSuspendAllCount = 0u;
        if (0u == pScb->SuspendAllCount)
        {
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
            Os_TmProtTaskEnd(pScb->SysCore, taskId, TP_SUS_ALL_INT);
            Os_Hal_EnableAllInt_ButTimingProtInt(pScb->SysCore);
#else
            Os_Hal_RestoreInt(pScb->SaveAllIntNested);
#endif
        }
    }

    if (pTcb->TaskSuspendOsCount > 0u)
    {
        pScb->SuspendOsCount -= pTcb->TaskSuspendOsCount;
        pTcb->TaskSuspendOsCount =0u;
        if (0u == pScb->SuspendOsCount)
        {
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
            Os_TmProtTaskEnd(pScb->SysCore, taskId, TP_SUS_OS_INT);
#endif
            Os_Hal_SetIpl(pScb->SaveOsIntNested, OS_ISR_ENABLE);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate one task.
 */
OS_LOCAL StatusType Os_ApplTerminateOneTask(Os_TaskType taskId)
{
    StatusType ret = E_OK;
    Os_TCBType *pTcb = Os_TCB[taskId];
    const Os_TaskCfgType *pTaskCfg = &Os_TaskCfg[taskId];
    Os_SCBType *pScb = OS_TASK_GET_SCB(taskId);

    /* Terminated task no need to process. */
    if (OS_TASK_STATE_SUSPENDED == pTcb->TaskState)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if (OS_TASK_STATE_START == pTcb->TaskState)
        {
            ret = E_NOT_OK;
        }
        else
        {
            Os_ApplReleaseAllResource(pScb, taskId);

#if (CFG_SPINLOCK_MAX > 0U)
            Os_ApplReleaseAllSpinlock(pScb, pTcb);
#endif

            /*If the current task is a non-preemptible task and is in the running state,
             * the scheduling lock should be reduced by one*/
            if ((OS_TASK_STATE_RUNNING == pTcb->TaskState) && (OS_PREEMPTIVE_NON == pTaskCfg->TaskSchedule))
            {
                pScb->SysDispatchLocker = pScb->SysDispatchLocker - 1U;
            }
        }
        /* Reset task_state. */
        pTcb->TaskState = OS_TASK_STATE_SUSPENDED;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Terminate, taskId);
        /* PRQA S 4543, 4523, 3762, 1277 -- */
        /* PRQA S 1821, 4532, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
        /* Remove from ready_queue. */
        Os_ApplReadyQueueRemove(pScb->QueueMg, taskId, pTcb->TaskRunPrio);

#if (CFG_STD_RESOURCE_MAX > 0U)
        pTcb->TaskResCount = 0U;
#endif

#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
        pTcb->TaskActCount = 0u;
#endif

        pTcb->TaskRunPrio = pTaskCfg->TaskPriority;

#if (CFG_EXTENDED_TASK_MAX > 0U)
        if (Os_CheckExternalTaskId(taskId, pScb->SysCore))
        {
            Os_ClearECB(taskId);
        }
#endif

        Os_UpdateHighPrioTask(pScb);

        Os_AppResumeTaskLockInt(pScb, taskId);
    }

    return ret;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate the task objects of the calling application
 */
/* PRQA S 3006, 6030 ++ */ /* VL_Os_3006, VL_MTR_Os_STMIF */
OS_LOCAL void Os_TerminateTaskObjectHandler(
    const Os_SCBType *pScb,
    const Os_TaskType *appTaskRef,
    Os_TaskType appTaskCnt)
/* PRQA S 3006, 6030 -- */
{
    /*check the count to be valid */
    if ((0u == (uint16)appTaskCnt) || (pScb->SysTaskMax <= (uint16)appTaskCnt))
    {
        /*nothing to do*/
    }
    else
    {
        for (uint16 i = 0u; i < (uint16)appTaskCnt; i++)
        {
            Os_TaskType taskId = appTaskRef[i];
            StatusType ret = Os_ApplTerminateOneTask(taskId);

            /* Os_App terminate task arch_related process. */
            if ((StatusType)E_OK == ret)
            {
                /* PRQA S 3432, 3138, 0306, 1006 ++ */ /* VL_Os_3432, VL_Os_PlatformNoDef, VL_Os_0306, VL_Os_1006 */
                Os_CallLevelType sysLevel = pScb->SysOsLevel;                                      
                if ((OS_LEVEL_TASK == sysLevel)                                                    
                    || (((OS_LEVEL_ERRORHOOK_APP == sysLevel) || (OS_LEVEL_ERRORHOOK == sysLevel)) 
                        && (TRUE != pScb->SysInIsrCat2)))                                         
                {                                                                                  
                    if (taskId != pScb->SysRunningTaskId) /* PRQA S 3410 */ /* VL_Os_3410 */
                    {                                                                              
                        OS_HAL_TERMINATE_TASK(taskId); /* PRQA S 1021, 2743 */ /* VL_Os_1021, VL_Os_2743 */
                    }                                                                              
                }                                                                                  
                else                                                                               
                {                                                                                  
                    OS_HAL_TERMINATE_TASK(taskId); /* PRQA S 1021, 2743 */ /* VL_Os_1021, VL_Os_2743 */                                                    
                }                                               
                /* PRQA S 3432, 3138, 0306, 1006 -- */
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ALARM_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate the Alarm objects of the calling application
 */
OS_LOCAL void Os_TerminateAlarmObjectHandler(
    const Os_SCBType *pScb,
    const Os_AlarmType *appAlarmRef,
    Os_AlarmType appAlarmCnt)
{
    /*check the count to be valid */
    if ((0u == (uint16)appAlarmCnt) || (pScb->SysAlarmMax < appAlarmCnt))
    {
        /*nothing to do*/
    }
    else
    {
        for (uint16 i = 0u; i < (uint16)appAlarmCnt; i++)
        {
            (void)Os_CancelAlarm(appAlarmRef[i]);
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (CFG_SCHEDTBL_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate the ScheduleTable objects of the calling application
 */
OS_LOCAL void Os_TerminateScheduleTblObjectHandler(
    Os_SCBType *pScb,
    const Os_ScheduleTableType *appSchTblRef,
    Os_ScheduleTableType appSchTblCnt)
{
    /*check the count to be valid */
    if ((0u == (uint16)appSchTblCnt) || (CFG_SCHEDTBL_MAX < (uint16)appSchTblCnt))
    {
        /*nothing to do*/
    }
    else
    {
        /* in order to pass service protection*/
        Os_CallLevelType sysCallLevel = pScb->SysOsLevel;
        pScb->SysOsLevel = OS_LEVEL_TASK; /* PRQA S 2982 */ /* VL_Os_2982 */
        for (uint16 i = 0u; i < (uint16)appSchTblCnt; i++) /* PRQA S 4527 */ /* VL_Os_4527 */
        {
            (void)Os_StopScheduleTable(appSchTblRef[i]);
        }
        pScb->SysOsLevel = sysCallLevel;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate the objects belonging to the calling application
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_TerminateObjects(Os_SCBType *pScb, const Os_ApplicationCfgType *curAppCfg)
/* PRQA S 3673 -- */
{
#if (CFG_ALARM_MAX > 0)
    /**
     * stop all active alarms
     */
    /* PRQA S 0310, 3305, 4342 ++ */ /* VL_Os_0310, VL_Os_3305, VL_Os_4342 */
    Os_TerminateAlarmObjectHandler(pScb, (Os_AlarmRefType)curAppCfg->AppObjectRef[OS_OBJECT_ALARM],
                                   (Os_AlarmType)curAppCfg->AppAlarmRefCnt);
    /* PRQA S 0310, 3305, 4342 -- */
#endif
/**
 * All other interrupt sources should be already disabled in above handling,
 * which as one precondition here.
 */
#if (CFG_ISR_MAX > 0)
    /**
     * Terminating ISR should check the current level to kill and free current
     * stack if in ISR Level, and take care if support nested ISRS. Besides,
     * As the resources occupied either on Task or ISR, here should free the resources
     * occupied on the ISR to continue the ISR terminating operation. And which occupied
     * on the Tasks will be handled while in the following terminating the Task objects.
     */
    /* MISRA-C: 17.4  Array subscripting applied to an object of pointer type.
       AppObjectRef is pointor of static_cfg_array, AppIsrRefCnt is the size of array,
       it must be used like below. */
    /* PRQA S 0310, 3305, 4342 ++ */ /* VL_Os_0310, VL_Os_3305, VL_Os_4342 */
    Os_TerminateISRObjectHandler((Os_IsrType *)curAppCfg->AppObjectRef[OS_OBJECT_ISR],
                                 (Os_IsrType)curAppCfg->AppIsrRefCnt);
    /* PRQA S 0310, 3305, 4342 -- */
#endif

#if (CFG_TASK_MAX > 0)
    /**
     * Terminate all the tasks of the application, kick out the queued request and free
     * all of the control block context, also will free the resources occupied on them.
     */
    /* PRQA S 0310, 3305 ++ */ /* VL_Os_0310, VL_Os_3305 */
    /* PRQA S 0432, 4342 ++ */ /* VL_Os_0432, VL_Os_4342 */
    Os_TerminateTaskObjectHandler(pScb, curAppCfg->AppObjectRef[OS_OBJECT_TASK],
                                  (Os_TaskType)curAppCfg->AppTaskCnt);
    /* PRQA S 0432, 4342 -- */
    /* PRQA S 0310, 3305 -- */
#endif

#if (CFG_SCHEDTBL_MAX > 0)
    /**
     * stop all schedule tables
     */
    Os_TerminateScheduleTblObjectHandler(pScb,
                                         curAppCfg->AppObjectRef[OS_OBJECT_SCHEDULETABLE], /* PRQA S 0432 */ /* VL_Os_0432 */
                                         curAppCfg->AppScheduleTableCnt); /* PRQA S 4442, 1441 */ /* VL_Os_4442, VL_Os_1441 */
#endif

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service determines the currently running
 *                      OS-Application (a unique identifier has to be
 *                      allocated to each application)
 */
/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006,VL_Os_3408,VL_MTR_Os_STCAL,VL_Os_1512 */
ApplicationType GetCurrentApplicationID(void)
/* PRQA S 1503,3006,3408,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    ApplicationType applId = INVALID_OSAPPLICATION;
    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544, 1259 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544, VL_Os_1259 */
    OSRtiEnterApi(pScb, OSApiId_GetCurrentApplicationID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetCurrentApplicationID_Start, 0);
    /* PRQA S 1317, 3432, 4442, 4521, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
    OS_HAL_DECLARE_CRITICAL();

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_CURRENT_APPLICATION_ID,
        .ObjectType = OS_OBJECT_INVALID,
        .ObjectID = INVALID_OSAPPLICATION,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };

    /* PRQA S 3326 ++ */ /* VL_Os_3326 */
    if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK)
    /* PRQA S 3326 -- */
#endif
    {
        OS_HAL_ENTRY_CRITICAL();

        /*OS798: return the identifier in which the current Task/ISR/hook is executed*/
        applId = pScb->SysRunningAppId;
        if (applId < CFG_OSAPPLICATION_MAX) /* OS799 */
        {
            if (pScb->SysAppId == applId)
            {
                applId = INVALID_OSAPPLICATION;
            }
        }
        else
        {
            applId = INVALID_OSAPPLICATION;
        }

        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetCurrentApplicationID(),
                          OSServiceId_GetCurrentApplicationID,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_GetCurrentApplicationID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetCurrentApplicationID_Return, applId);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(status);
    return applId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service sets the own state of an OS-Application
 *                         from OS_APPLICATION_RESTARTING to OS_APPLICATION_ACCESSIBLE.
 */
/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006,VL_Os_3408,VL_MTR_Os_STCAL,VL_Os_1512 */
StatusType AllowAccess(void)
/* PRQA S 1503,3006,3408,6070,1512 -- */
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
    /* PRQA S 1317, 3432, 4442, 4521, 4544, 1259 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544, VL_Os_1259 */
    OSRtiEnterApi(pScb, OSApiId_AllowAccess);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_AllowAccess_Start, 0);
    /* PRQA S 1317, 3432, 4442, 4521, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_ALLOW_ACCESS,
        .ObjectType = OS_OBJECT_INVALID,
        .ObjectID = INVALID_OSAPPLICATION,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };

    /* PRQA S 3326 ++ */ /* VL_Os_3326 */
    if ((err = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK)
    /* PRQA S 3326 -- */
#endif
    {
        OS_HAL_ENTRY_CRITICAL();
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if (OS_APPLICATION_RESTARTING != Os_AppCB[pScb->SysRunningAppId].AppState)
        /* PRQA S 3442 -- */
        {
            OS_HAL_EXIT_CRITICAL();
            err = E_OS_STATE;
        }
        else
        {
            Os_AppCB[pScb->SysRunningAppId].AppState = OS_APPLICATION_ACCESSIBLE;
            /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
            /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
            /* PRQA S 0499, 4543, 1861, 4397, 1277 ++ */ /* VL_Os_0499, VL_Os_4543, VL_Os_1861, VL_Os_4397, VL_Os_1277 */
            ARTI_TRACE(NOSUSP, AR_CP_OS_APPLICATION, Os, pScb->SysCore, OsApplication_AllowAccess, pScb->SysRunningAppId);
            /* PRQA S 0499, 4543, 1861, 4397, 1277 -- */
            /* PRQA S 1821, 4532, 4544, 4542 -- */
            /* PRQA S 3138, 3141 -- */
            OS_HAL_EXIT_CRITICAL();
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        /* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_AllowAccess(), OSServiceId_AllowAccess, err, pScb);
        /* PRQA S 3138 -- */
    }
#endif
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_AllowAccess);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_AllowAccess_Return, err);
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
 * This service returns the current state of an OS-Application.
 */
/* PRQA S 1503,3006,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
StatusType GetApplicationState(ApplicationType Application, ApplicationStateRefType Value)
/* PRQA S 1503,3006,3408,1512 -- */
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
    OSRtiEnterApi(pScb, OSApiId_GetApplicationState);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetApplicationState_Start, Application);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_APPLICATION_STATE,
        .ObjectType = OS_OBJECT_APP,
        .ObjectID = Application,
        /* PRQA S 0306 ++ */ /* VL_Os_0306 */
        .Address = (uint32)Value,
        /* PRQA S 0306 -- */
    };
    /* PRQA S 3326 ++ */ /* VL_Os_3326 */
    if ((err = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK)
    /* PRQA S 3326 -- */
#endif
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        if (CFG_OSAPPLICATION_MAX <= Application)
        {
            err = E_OS_ID;
        }
        else
#endif
        {
            *Value = Os_AppCB[Application].AppState;
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetApplicationState(Application, Value),
                          OSServiceId_GetApplicationState,
                          err, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1258, 4342 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1258, VL_Os_4342 */
    OSRtiExitApi(pScb, OSApiId_GetApplicationState);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetApplicationState_Return, (err == E_OK ) ? *Value : (Os_ApplicationStateType)0U);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/  
    /* PRQA S 3432, 4544, 1258, 4342 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service determines if the OS-Applications,
 *                       given by applId, is allowed to use the IDs of a
 *                       Task, ISR, Resource, Counter, Alarm or Schedule
 *                       Table in API calls.
 */
/* PRQA S 1503,3006,3408 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408 */
/* PRQA S 3334,6070,1512,1506 ++ */ /* VL_Os_3334, VL_MTR_Os_STCAL, VL_Os_1512, VL_Os_1506 */
ObjectAccessType CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, AppObjectId ObjectID)
/* PRQA S 3334,6070,1512,1506 -- */
/* PRQA S 1503,3006,3408 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    ObjectAccessType err = OS_NO_ACCESS;
    StatusType Status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_CheckObjectAccess);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CheckObjectAccess_Start, ApplID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_OBJECT_ACCESS,
        .ObjectType = ObjectType,
        .ObjectID = ApplID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
    /* PRQA S 3326 ++ */ /* VL_Os_3326 */
    if ((Status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK)
    /* PRQA S 3326 -- */
#endif
    {
        if (ApplID >= CFG_OSAPPLICATION_MAX)
        {
            err = OS_NO_ACCESS;
        }
        /*OS423: if the objectType is not a valid , return OS_NO_ACCESS*/
        else if (ObjectType >= OS_OBJECT_MAX)
        {
            err = OS_NO_ACCESS;
        }
        else if (Os_ObjectIDCheck((uint16)ObjectID, (uint8)ObjectType) != TRUE)
        {
            err = OS_NO_ACCESS;
        }
        else
        {
            err = Os_CheckObjectAccess(pScb, ApplID, ObjectType, ObjectID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (Status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_CheckObjectAccess(ApplID, ObjectType, ObjectID),
                          OSServiceId_CheckObjectAccess,
                          Status, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_CheckObjectAccess);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CheckObjectAccess_Return, ObjectType);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
    UNUSED_PARAMETER(Status);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ISR_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * TryToGetSpinlock has the same functionality as GetSpinlock with the difference
                         that if the spinlock is already occupied by a TASK on a different core the function
                         sets the OUT parameter "Success" and returns with E_OK
 */
OS_LOCAL void Os_DisableIntInApp( /* PRQA S 3006 */ /* VL_Os_3006 */
    const Os_ApplicationCfgType *posCurAppCfg)
{
    uint16 isrRefCnt = posCurAppCfg->AppIsrRefCnt;
    for (uint16 i = 0u; i < isrRefCnt; i++)
    {
        /* PRQA S 3305, 3678, 0310 ++ */ /* VL_Os_3305, VL_Os_3678, VL_Os_0310 */
        Os_IsrType *arrayAppIsr = (Os_IsrType *)posCurAppCfg->AppObjectRef[OS_OBJECT_ISR];
        uint32 isrId = (uint32)arrayAppIsr[i];
        /* PRQA S 3305, 3678, 0310 -- */
        uint32 isrRegVal = Os_IsrCfg[isrId].IsrSrc;
        /* PRQA S 0303, 3138 ++ */ /* VL_Os_0303, VL_Os_3138 */
        /* PRQA S 3442, 3345, 1006 ++ */ /* VL_Os_3442, VL_Os_3345, VL_Os_1006 */
         OS_Hal_DisableIntApp(isrRegVal);
        /* PRQA S 3442, 3345, 1006 -- */
        /* PRQA S 0303, 3138 -- */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:TerminateApplication
 */
/* PRQA S 6030, 6070, 3006 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCAL, VL_Os_3006 */
void Os_TerminateApplication
/* PRQA S 6030, 6070, 3006 -- */
    (
        Os_SCBType *pScb,
        ApplicationType application,
        RestartType restartOption)
{
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    Os_CallLevelType sysCallLevel = pScb->SysOsLevel;
    const Os_ApplicationCfgType *posCurAppCfg = &Os_AppCfg[application];
    OS_HAL_EXIT_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    /* PRQA S 3442 ++ */ /* VL_Os_3442 */
    if (OS_APPLICATION_ACCESSIBLE != Os_AppCB[application].AppState)
    /* PRQA S 3442 -- */
    {
        /* nothing to do */
    }
    else
    {
        /*@OS287: If Called from allowed context, terminate the calling OS-Application*/
        Os_TerminateObjects(pScb, posCurAppCfg);

/* OS447: Disable the interrupt source of the OsIsrs owned by the application */
#if (CFG_ISR_MAX > 0)
        Os_DisableIntInApp(posCurAppCfg);
#endif

        if (OS_RESTART == restartOption)
        {
            Os_AppCB[application].AppState = OS_APPLICATION_RESTARTING;
            /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
            /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
            /* PRQA S 0499, 4543, 1861, 4397, 1277 ++ */ /* VL_Os_0499, VL_Os_4543, VL_Os_1861, VL_Os_4397, VL_Os_1277 */
            ARTI_TRACE(NOSUSP, AR_CP_OS_APPLICATION, Os, pScb->SysCore, OsApplication_Restart, application);
            /* PRQA S 0499, 4543, 1861, 4397, 1277 -- */
            /* PRQA S 1821, 4532, 4544, 4542 -- */
            /* PRQA S 3138, 3141 -- */
        }
        else
        {
            Os_AppCB[application].AppState = OS_APPLICATION_TERMINATED;
            /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
            /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
            /* PRQA S 0499, 4543, 1861, 4397, 1277 ++ */ /* VL_Os_0499, VL_Os_4543, VL_Os_1861, VL_Os_4397, VL_Os_1277 */
            ARTI_TRACE(NOSUSP, AR_CP_OS_APPLICATION, Os, pScb->SysCore, OsApplication_Terminate, application);
            /* PRQA S 0499, 4543, 1861, 4397, 1277 -- */
            /* PRQA S 1821, 4532, 4544, 4542 -- */
            /* PRQA S 3138, 3141 -- */
        }

        /*@OS346: if OS_RESTART, activate the RestartTask of the application*/
        if ((OS_RESTART == restartOption) && (CheckCoreTaskId(posCurAppCfg->RestartTask,
                                                              pScb->SysCore)))
        {
            /* Set OsLevel to OS_LEVEL_TASK in order to call ActivateTask.
             * ActivateTask can not be called in OS_LEVEL_ERRORHOOK_APP level. */
            if (OS_LEVEL_ERRORHOOK_APP == sysCallLevel)
            {
                pScb->SysOsLevel = OS_LEVEL_TASK;
            }
            ApplicationStateType tempAppState = Os_AppCB[application].AppState;

            Os_AppCB[application].AppState = OS_APPLICATION_ACCESSIBLE;

            /*Activate the RestartTask*/
            pScb->SysDispatchLocker = pScb->SysDispatchLocker + (Os_LockerType)1;
            (void)Os_ActivateTask(posCurAppCfg->RestartTask); /* PRQA S 1520 */ /* VL_Os_1520 */
            pScb->SysDispatchLocker = pScb->SysDispatchLocker - (Os_LockerType)1;

            Os_AppCB[application].AppState = tempAppState;

            /* In level OS_LEVEL_ERRORHOOK_APP, restore OsLevel. */
            if (OS_LEVEL_ERRORHOOK_APP == sysCallLevel)
            {
                pScb->SysOsLevel = sysCallLevel;
            }
        }

        if (application == pScb->SysRunningAppId)
        {
#if (CFG_ISR2_MAX > 0)
            if (TRUE == pScb->SysInIsrCat2)
            {
                /* If this service is called by isr_c2, SysRunningTaskId can be
                 * set to IDLE_TASK, in order to call Os_SwitchTask in OS_ARCH_ISR2_EPILOGUE. */
                /* Tasks of this OsApp are all terminated, the SysRunningTaskId is also might be terminated,
                 * so Os_SwitchTask must be called in OS_ARCH_ISR2_EPILOGUE. */
                /*If called context is in ISR2, Now Exit */
                OS_HAL_EXIT_CRITICAL();
                Os_IsrType isrId = pScb->SysRunningIsrCat2Id;
                /* PRQA S 2743, 3442, 0306, 1006 ++ */ /* VL_Os_2743, VL_Os_3442, VL_Os_0306, VL_Os_1006 */
                /* PRQA S 1431, 1290, 4424, 1021 ++ */ /* VL_Os_1431, VL_Os_1290, VL_Os_4424, VL_Os_1021 */
                OS_HAL_ISR2_EPILOGUE_KILL_ISR(isrId);  /* PRQA S 1520, 1753, 1512, 1513 */ /* VL_Os_1520, VL_Os_1753, VL_Os_1512, VL_Os_1513 */
                /* PRQA S 1431, 1290, 4424, 1021 -- */
                /* PRQA S 2743, 3442, 0306, 1006 -- */
            }
            else
#endif
            {
                /* Task dispatch. */
                pScb->SysOsLevel = OS_LEVEL_TASK;
                pScb->SysDispatchLocker = 0u;

                Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
            }
        }
    }

    OS_HAL_EXIT_CRITICAL();
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Terminate Application
 */
#if (OS_AUTOSAR_CORES > 1)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL StatusType Os_RpcAction_TerminateApplication(uint32 *inPara)
/* PRQA S 3673 -- */
{
    Os_SCBType *pScb = Os_GetCurrentContext();
    Os_TerminateApplication(pScb,
                            (ApplicationType)inPara[0],
                            (RestartType)inPara[1]);/* PRQA S 4342 */ /* VL_Os_4342 */

    return E_OK;
}

/* PRQA S 3209 ++ */ /* VL_Os_3209 */
OS_LOCAL StatusType Os_RpcCall_TerminateApplication(
    Os_CoreIdType ownerCore,
    ApplicationType Application,
    RestartType RestartOption)
/* PRQA S 3209 -- */
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_SYNC,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_TerminateApplication,
        .InPara[0] = (uint32)Application,
        /* PRQA S 0691 ++ */ /* VL_Os_0691 */
        .InPara[1] = (uint32)RestartOption,
        /* PRQA S 0691 -- */
    };/* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

/* PRQA S 6030,6010,6070 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STCAL */
/* PRQA S 1503,3006,3408,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
StatusType TerminateApplication(ApplicationType Application, RestartType RestartOption)
/* PRQA S 1503,3006,3408,1512 -- */
/* PRQA S 6030,6010,6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_TerminateApplication);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_TerminateApplication_Start, Application);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /*SWS_Os_00493*/
    if (CFG_ECUCPARTITION_MAX <= Application)
    {
        err = E_OS_ID;
    }
    else
    {
#endif
        /* Convert the incoming PartitionId to AppId */
        Os_ApplicationType appId = Os_EcucPartitionIdMapToAppId[Application]; /* PRQA S 4424 */ /* VL_Os_4424 */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /*SWS_Os_00493*/
        if (CFG_OSAPPLICATION_MAX <= appId)
        {
            err = E_OS_ID;
        }
        /*SWS_Os_00459: if the <RestartOption> is invalid, return E_OS_VALUE*/
        else if ((OS_RESTART != RestartOption) && (OS_NO_RESTART != RestartOption))
        {
            err = E_OS_VALUE;
        }
        /*SWS_Os_00494*/
        else if ((FALSE == Os_AppCfg[pScb->SysRunningAppId].Trusted) && (appId != pScb->SysRunningAppId))
        {
            err = E_OS_ACCESS;
        }
        /*SWS_Os_00507*/
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if (OS_APPLICATION_TERMINATED == Os_AppCB[appId].AppState)
        /* PRQA S 3442 -- */
        {
            err = E_OS_STATE;
        }
        /*SWS_Os_00508,SWS_Os_00548*/
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        else if ((OS_APPLICATION_RESTARTING == Os_AppCB[appId].AppState) && ((pScb->SysRunningAppId != appId) || (OS_RESTART == RestartOption)))
        /* PRQA S 3442 -- */
        {
            err = E_OS_STATE;
        }
        /* When calling TerminateApplication in ErrorHook, it can only terminate itself */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        else if (((OS_LEVEL_ERRORHOOK == pScb->SysOsLevel) || (OS_LEVEL_ERRORHOOK_APP == pScb->SysOsLevel)) && (appId != pScb->SysRunningAppId))
        {
            err = E_OS_ACCESS;
        }
#endif
        else
#endif
        {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
            Os_ServicePortParamType SprotParam = {
                .AllowedContext = OS_SERVICEPORT_CHECK_TERMINATE_APPLICATION,
                .ObjectType = OS_OBJECT_INVALID,
                .ObjectID = appId,
                /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
                .Address = NULL_PARA,
                /* PRQA S 1258 -- */
            };
            /* PRQA S 3326 ++ */ /* VL_Os_3326 */
            if ((err = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK)
            /* PRQA S 3326 -- */
#endif
            {
#if (OS_AUTOSAR_CORES > 1)
                Os_CoreIdType coreId = Os_AppCfg[appId].HostCore;
                if (coreId != pScb->SysCore)
                {
                    /* PRQA S 3200 ++ */ /* VL_Os_3200 */
                    Os_RpcCall_TerminateApplication(coreId, appId, RestartOption);
                    /* PRQA S 3200 -- */
                }
                else
#endif
                {
                    Os_TerminateApplication(pScb, appId, RestartOption);
                }
            }
        }
#if (CFG_ERRORHOOK == TRUE)
        if (err != E_OK)
        {
            Os_TraceErrorHook(OSError_Save_TerminateApplication(appId, RestartOption),
                                OSServiceId_TerminateApplication,
                                err, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
        }
#endif
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_TerminateApplication);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_TerminateApplication_Return, err);
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
 * Internal implementation of OS service:CheckObjectAccess
 */
Os_ObjectAccessType Os_CheckObjectAccess(
    const Os_SCBType *pScb,
    ApplicationType applId,
    ObjectTypeType objectType,
    AppObjectId objectId)
{
    Os_ObjectAccessType err = OS_ACCESS;
    const Os_ObjectAppCfgType *pObjectAppCfgRef = Os_GetObjectAccess(objectType, objectId);

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    if ((OS_OBJECT_RESOURCE == objectType) || (OS_OBJECT_SPINLOCK == objectType))
    {
        /*object do not belong to any application*/
    }
    else
    {
        /* SWS_Os_00504, SWS_Os_00509 */
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if ((pObjectAppCfgRef->HostApp != pScb->SysRunningAppId) && (Os_AppCB[pObjectAppCfgRef->HostApp].AppState != OS_APPLICATION_ACCESSIBLE))
        /* PRQA S 3442 -- */
        {
            OS_HAL_EXIT_CRITICAL();
            err = OS_NO_ACCESS;
        }
    }

    if ((Os_ObjectAccessType)OS_ACCESS == err)
    {
/* OS318: if the object type is OS_OBJECT_RESOURCE and the object is
 * RES_SCHEDULER , return OS_ACCESS */
#if (TRUE == CFG_USERESSCHEDULER)
        /* PRQA S 4342 ++ */ /* VL_Os_4342 */
        if ((OS_OBJECT_RESOURCE == objectType) && (pScb->ScheduleResId == (Os_ResourceType)objectId))
        /* PRQA S 4342 -- */
        {
            err = OS_ACCESS;
        }
        else
#endif
        {
            /* applId in which bit_group(uint16). */
            ApplicationType accAppNodePos = OS_APPGETACCESS_GP(applId);
            /* applId in which bit_pos of uint16. */
            /* PRQA S 1840, 0582 ++ */ /* VL_Os_1840, VL_Os_0582 */
            ApplicationType AccAppBitPos = OS_APPACCESS_MASK(applId);
            /* PRQA S 1840, 0582 -- */
            /* Access right bit map. */
            const ApplicationType *accAppRef = pObjectAppCfgRef->AccAppRef;

/* Check object access right by app_id. */
            if ((pObjectAppCfgRef->AccAppRefNodeCnt > 0U) &&
                (accAppRef != NULL_PTR) &&
                ((AccAppBitPos & accAppRef[accAppNodePos]) > 0u))
            {
                err = OS_ACCESS;
            }
            else
            {
                err = OS_NO_ACCESS;
            }
        }
        OS_HAL_EXIT_CRITICAL();
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service determines the OS-Application (a unique
 *                         identifier has to be allocated to each application)
 *                         where the caller originally belongs to (was configured to)
 */
/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
ApplicationType GetApplicationID(void)
/* PRQA S 1503,3006,3408,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    ApplicationType applId = INVALID_OSAPPLICATION;
    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544, 1259, 2986 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544, VL_Os_1259, VL_Os_2986 */
    OSRtiEnterApi(pScb, OSApiId_GetApplicationID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetApplicationID_Start, 0);
    /* PRQA S 1317, 3432, 4442, 4521, 4544, 1259, 2986 -- */
    /* PRQA S 3138, 3141 -- */

    OS_HAL_DECLARE_CRITICAL();

/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_APPLICATION_ID,
        .ObjectType = OS_OBJECT_INVALID,
        .ObjectID = INVALID_OSAPPLICATION,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };

    /* PRQA S 3326 ++ */ /* VL_Os_3326 */
    if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK)
    /* PRQA S 3326 -- */
#endif
    {
        OS_HAL_ENTRY_CRITICAL();
/*OS261: return the identifier to which the executing Task/ISR/hook was configured*/
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
        if (0u != pScb->Os_TrustedFuncTp.TrustedFuncNest)
        {
            applId = pScb->TrustedFuncNestQueue[pScb->Os_TrustedFuncTp.TrustedFuncNest - 1U];
        }
        else
#endif
        {
            applId = pScb->SysRunningAppId;
        }

        if (applId < CFG_OSAPPLICATION_MAX) /* OS262 */
        {
            applId = Os_AppIdMapToPartitionId[applId]; /* AppId to EcucPartitionId */
        }
        else
        {
            applId = INVALID_OSAPPLICATION;
        }

        OS_HAL_EXIT_CRITICAL();
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_GetApplicationID(), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_GetApplicationID,
                          status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_GetApplicationID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetApplicationID_Return, applId);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(status);
    return applId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:CheckObjectOwnership
 */
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
OS_LOCAL ApplicationType Os_CheckObjectOwnership(
    const Os_SCBType *pScb,
    ObjectTypeType objectType,
    AppObjectId objectId)
/* PRQA S 3450 -- */
{
    ApplicationType applId = INVALID_OSAPPLICATION;

    OS_HAL_DECLARE_CRITICAL();

    if (objectType >= OS_OBJECT_MAX)
    {
        applId = INVALID_OSAPPLICATION;
    }
    else
    {
        if (Os_ObjectIDCheck((ObjectType)objectId, (uint8)objectType) == TRUE)
        {
            OS_HAL_ENTRY_CRITICAL();

            const Os_ObjectAppCfgType *pObjectAppCfgRef = Os_GetObjectAccess(objectType, objectId);

/* OS318: if the object type is OS_OBJECT_RESOURCE and the object is
 * RES_SCHEDULER , return OS_ACCESS */
#if (TRUE == CFG_USERESSCHEDULER)
            /* PRQA S 4342 ++ */ /* VL_Os_4342 */
            if ((OS_OBJECT_RESOURCE == objectType) && (pScb->ScheduleResId == (Os_ResourceType)objectId))
            /* PRQA S 4342 -- */
            {
                applId = INVALID_OSAPPLICATION;
            }
            else
#endif
            {
                applId = pObjectAppCfgRef->HostApp;
            }
            OS_HAL_EXIT_CRITICAL();
        }
        else
        {
            applId = INVALID_OSAPPLICATION;
        }
    }

    return applId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This service determines to which OS-Application
 *                      a given Task, ISR, Resource, Counter, Alarm or
 *                      Schedule Table belongs
 */
/* PRQA S 1503,3006,3408 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408 */
/* PRQA S 3334,6070,1512,1506 ++ */ /* VL_Os_3334, VL_MTR_Os_STCAL, VL_Os_1512, VL_Os_1506 */
ApplicationType CheckObjectOwnership(ObjectTypeType ObjectType, AppObjectId ObjectID)
/* PRQA S 3334,6070,1512,1506 -- */
/* PRQA S 1503,3006,3408 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    ApplicationType applId = INVALID_OSAPPLICATION;
    StatusType status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544, 1259 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544, VL_Os_1259 */
    OSRtiEnterApi(pScb, OSApiId_CheckObjectOwnership);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CheckObjectOwnership_Start, ObjectType);
    /* PRQA S 1317, 3432, 4442, 4521, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_OBJECT_OWNERSHIP,
        .ObjectType = ObjectType,
        .ObjectID = ObjectID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };

    /* PRQA S 3326 ++ */ /* VL_Os_3326 */
    if ((status = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK)
    /* PRQA S 3326 -- */
#endif
    {
        applId = Os_CheckObjectOwnership(pScb, ObjectType, ObjectID);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (status != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_CheckObjectOwnership(ObjectType, ObjectID), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_CheckObjectOwnership,
                          status, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_CheckObjectOwnership);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CheckObjectOwnership_Return, applId);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(status);
    return applId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /*CFG_OSAPPLICATION_MAX >0U */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 6530 EOF */ /* VL_MTR_Os_STECT */
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
