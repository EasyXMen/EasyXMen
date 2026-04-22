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
 **  @file               : Os_Resource.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Resource API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Resource.h"
#include "Os_Interrupt.h"
#include "Os_ReadyQue.h"
#include "Os_Tprot.h"
#include "Os_Sprot.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Task.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"

/* ===================================================== macros ===================================================== */
#define FOUR 4u
#define LOW_4_BITS_MASK 0x0Fu

/* ============================================ external data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (CFG_RESOURCE_MAX > 0U)
#if (CFG_STD_RESOURCE_MAX > 0U)
/**
 * @brief           Initializes a Resource Control Block
 * @param[in]       pRcb: Pointer to the Resource Control Block to initialize
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_RCBInit(Os_RCBType *pRcb);

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/**
 * @brief           Initializes timing protection for resources
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ResourceTmPortInit(const Os_SCBType *pScb);
#endif

/**
 * @brief           Saves resource state when a task gets a resource
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       pTCB: Pointer to the Task Control Block of the running task
 * @param[in]       resId: ID of the resource being acquired
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_SaveResourceByTaskOrder(const Os_SCBType *pScb, Os_TCBType *pTCB, ResourceType resId);

/**
 * @brief           Restores resource state when a task releases a resource
 * @param[in]       pTCB: Pointer to the Task Control Block of the running task
 * @param[in]       resId: ID of the resource being released
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ResumeResourceByTaskOrder(Os_TCBType *pTCB, ResourceType resId);

/**
 * @brief           Saves resource state when an ISR gets a resource
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       pICB: Pointer to the Interrupt Control Block
 * @param[in]       resId: ID of the resource being acquired
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_SaveResourceByIsr2Order(const Os_SCBType *pScb, Os_ICBType *pICB, ResourceType resId);

/**
 * @brief           Restores resource state when an ISR releases a resource
 * @param[in]       pICB: Pointer to the Interrupt Control Block
 * @param[in]       resId: ID of the resource being released
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ResumeResourceByIsr2Order(Os_ICBType *pICB, ResourceType resId);

/**
 * @brief           Implements resource acquisition for tasks
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       resId: ID of the resource to acquire
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetResourceByTask(Os_SCBType *pScb, ResourceType resId);

/**
 * @brief           Implements resource acquisition for both tasks and interrupts
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       resId: ID of the resource to acquire
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetResourceByTaskOrInterrupt(Os_SCBType *pScb, ResourceType resId);

/**
 * @brief           Implements resource acquisition for interrupts
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       pICB: Pointer to the Interrupt Control Block
 * @param[in]       resId: ID of the resource to acquire
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetResourceByInterrupt(const Os_SCBType *pScb, Os_ICBType *pICB, ResourceType resId);

/**
 * @brief           Implements resource release for tasks
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       resId: ID of the resource to release
 * @return          StatusType
 * @retval          E_OK: Resource released successfully
 * @retval          E_OS_ACCESS: Task doesn't have sufficient priority
 * @retval          E_OS_NOFUNC: Resource not held by the task or LIFO order violation
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_ReleaseResourceByTask(const Os_SCBType *pScb, ResourceType resId);

/**
 * @brief           Implements resource release for both tasks and interrupts
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       resId: ID of the resource to release
 * @return          StatusType
 * @retval          E_OK: Resource released successfully
 * @retval          E_OS_NOFUNC: Resource not held by the task/interrupt or LIFO order violation
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_ReleaseResourceByTaskOrInterrupt(Os_SCBType *pScb, ResourceType resId);

/**
 * @brief           Implements resource release for interrupts
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       resId: ID of the resource to release
 * @return          StatusType
 * @retval          E_OK: Resource released successfully
 * @retval          E_OS_NOFUNC: Resource not held by the interrupt or LIFO order violation
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_ReleaseResourceByInterrupt(const Os_SCBType *pScb, ResourceType resId);

#if (OS_STATUS_EXTENDED == CFG_STATUS)
/**
 * @brief           Performs safety checks when acquiring a resource
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       resId: ID of the resource to check
 * @return          StatusType
 * @retval          E_OK: Safety check passed
 * @retval          E_OS_ACCESS: Resource already occupied or no access rights
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_GetResourceSafetyCheck(const Os_SCBType *pScb, ResourceType resId);

/**
 * @brief           Performs safety checks when releasing a resource
 * @param[in]       resId: ID of the resource to check
 * @return          StatusType
 * @retval          E_OK: Safety check passed
 * @retval          E_OS_NOFUNC: No resource to release
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_ReleaseResourceSafetyCheck(ResourceType resId);
#endif

/**
 * @brief           Validates a resource ID
 * @param[in]       resId: ID of the resource to validate
 * @return          StatusType
 * @retval          E_OK: Valid resource ID
 * @retval          E_OS_ID: Invalid resource ID
 * @retval          E_OS_CORE: Resource belongs to different core
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_ResourceIdSafetyCheck(ResourceType resId);
#endif
#endif

/* ========================================== external function definitions ========================================= */
#if (CFG_RESOURCE_MAX > 0U)
#if (CFG_STD_RESOURCE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the resource control block
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitResource(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE || TRUE == CFG_USERESSCHEDULER)
    Os_SCBType *pScb = Os_GetSystemContext(coreId);
#endif

    if ((uint32)Os_CfgStdResourceMax_Inf[coreId] > 0U)
    {
#if (TRUE == CFG_USERESSCHEDULER)
        pScb->ScheduleResId = Os_ResScheduler_Inf[coreId];
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_ResourceTmPortInit(pScb);
#endif

        Os_ResourceType stdResIdStard = Os_StdResIdRange[coreId].StdResStart;
        Os_ResourceType stdResIdEnd = Os_StdResIdRange[coreId].StdResEnd;
        for (uint32 resId = (uint32)stdResIdStard; resId < (uint32)stdResIdEnd; resId++)
        {
            Os_RCBInit(Os_RCB[resId]);
            OSRtiClearLockerId(resId); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Resource init RCB
 */
OS_LOCAL void Os_RCBInit(Os_RCBType *pRcb)
{
    pRcb->SaveCount = 0u;
    pRcb->SavePrio = OS_PRIORITY_INVALID;
    pRcb->SaveLevel = OS_LEVEL_MAIN;

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_CoreIdType coreId = Os_GetCoreIdLocal();

    pRcb->WhichTaskOccupy = OS_TASK_INVALID; /* PRQA S 4342 */ /* VL_Os_4342 */
    pRcb->WhichIsrOccupy = INVALID_ISR; /* PRQA S 1297*/ /* VL_Os_1297*/

/* Autosar SC2: for timing protection. Budget for resoruce lock init
 * to invalid. Invalid means not be configed. */
#if (CFG_TASK_MAX > 0)
    if (NULL_PTR != pRcb->TmProtResBgtTask)
    {
        Os_TaskType idStartRange = Os_TaskIdRange[coreId].AllTask.Start;
        Os_TaskType idEndRange = Os_TaskIdRange[coreId].AllTask.End;
        for (uint32 taskId = (uint32)idStartRange; taskId < (uint32)idEndRange; taskId++)
        {
            pRcb->TmProtResBgtTask[taskId] = OS_TICK_INVALID; /* PRQA S 1258 */ /* VL_Os_1258 */
        }
    }
#endif

#if (CFG_ISR_MAX > 0)
    Os_IsrType isrStart = Os_IsrIdRange[coreId].Isr2.IsrStart;
    Os_IsrType isrEnd = Os_IsrIdRange[coreId].Isr2.IsrEnd;
    if (NULL_PTR != pRcb->TmProtResBgtIsr)
    {
        for (uint32 isrId = (uint32)isrStart; isrId < (uint32)isrEnd; isrId++)
        {
            pRcb->TmProtResBgtIsr[isrId] = OS_TICK_INVALID; /* PRQA S 1258 */ /* VL_Os_1258 */
        }
    }
#endif

    pRcb->osResTpNode.TpEndTime = 0u;
    pRcb->osResTpNode.Pre = NULL_PTR;
    pRcb->osResTpNode.Next = NULL_PTR;
    pRcb->osResTpNode.TpNodeInList = FALSE;
    pRcb->osResTpNode.TpNodeOpt = TP_RESOURCE;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Resource init TmPort module
 */
OS_LOCAL void Os_ResourceTmPortInit(const Os_SCBType *pScb)
{

    Os_CoreIdType coreId = pScb->SysCore;
    Os_ResourceType stdResIdStard = Os_StdResIdRange[coreId].StdResStart;
    Os_ResourceType stdResIdEnd = Os_StdResIdRange[coreId].StdResEnd;
    uint32 isrMaxNum = (uint32)Os_IsrIdRange[coreId].Isr2.IsrEnd -
                           (uint32)Os_IsrIdRange[coreId].Isr2.IsrStart;

    if (TRUE == Os_CfgTprot_Inf[coreId])
    {
        Os_RCB[stdResIdStard]->TmProtResBgtTask = NULL_PTR;
        Os_RCB[stdResIdStard]->TmProtResBgtIsr = NULL_PTR;

        /* PRQA S 3432, 0488 ++ */ /* VL_Os_3432, VL_Os_0488 */
        uint32* pRCBTmProtResBgtTask = Os_RCBTmProtResBgtTask_Inf;
        if ((pScb->SysTaskMax > 0U) && (NULL_PTR != pRCBTmProtResBgtTask))
        {
            for (uint32 i = (uint32)stdResIdStard; i < (uint32)stdResIdEnd; i++)
            {
                Os_RCB[i]->TmProtResBgtTask = pRCBTmProtResBgtTask + (CFG_TASK_MAX * i);
            }
        }
        uint32* pRCBTmProtResBgtIsr = Os_RCBTmProtResBgtIsr_Inf;
        if ((isrMaxNum > 0U) && (NULL_PTR != pRCBTmProtResBgtIsr))
        {
            for (uint32 i = (uint32)stdResIdStard; i < (uint32)stdResIdEnd; i++)
            {
                Os_RCB[i]->TmProtResBgtIsr = pRCBTmProtResBgtIsr + (CFG_ISR_MAX * i);
            }
        }
        /* PRQA S 3432, 0488 -- */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save the runtime environment and sequence of GetResource.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_SaveResourceByTaskOrder(const Os_SCBType *pScb, Os_TCBType *pTCB, ResourceType resId)
{
    Os_RCB[resId]->SaveLevel = pScb->SysOsLevel;

    pTCB->TaskResourceStack[pTCB->TaskResCount] = resId;
    pTCB->TaskResCount = pTCB->TaskResCount + 1u;
    Os_RCB[resId]->SaveCount = pTCB->TaskResCount;

#if (CFG_SPINLOCK_MAX > 0U)
    pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount] = OS_OBJECT_RESOURCE;
    pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount] = resId; /* PRQA S 4424 */ /* VL_Os_4424 */
    pTCB->TaskCriticalZoneCount = pTCB->TaskCriticalZoneCount + 1u;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Resume the runtime environment and sequence of Resource.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ResumeResourceByTaskOrder(Os_TCBType *pTCB, ResourceType resId)
{
    pTCB->TaskResCount = pTCB->TaskResCount - 1u;
    Os_RCB[resId]->SaveCount = 0u;
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    pTCB->TaskCriticalZoneCount = pTCB->TaskCriticalZoneCount - 1u;
    pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount] = OS_OBJECT_MAX;
    pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount] = OS_OBJECT_INVALID; /* PRQA S 4424 */ /* VL_Os_4424 */
#endif
    Os_RCB[resId]->SaveLevel = OS_LEVEL_MAIN;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Save the runtime environment and sequence of GetResource.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_SaveResourceByIsr2Order(const Os_SCBType *pScb, Os_ICBType *pICB, ResourceType resId)
{
    Os_RCB[resId]->SaveLevel = pScb->SysOsLevel;
    pICB->IsrC2ResourceStack[pICB->IsrC2ResCount] = resId;
    /* PRQA S 4521, 4442 ++ */ /* VL_Os_4521, VL_Os_4442 */
    pICB->IsrC2ResCount = pICB->IsrC2ResCount + 1u;
    /* PRQA S 4521, 4442 -- */
    Os_RCB[resId]->SaveCount = (uint8)pICB->IsrC2ResCount;
/* Multi core res and spinlock should together release as
 * LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount] = OS_OBJECT_RESOURCE;
    pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount] = resId; /* PRQA S 4424 */ /* VL_Os_4424 */
    pICB->Isr2CriticalZoneCount = pICB->Isr2CriticalZoneCount + 1u;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Resume the runtime environment and sequence of Resource.
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ResumeResourceByIsr2Order(Os_ICBType *pICB, ResourceType resId)
{
    /* PRQA S 4521, 4442 ++ */ /* VL_Os_4521, VL_Os_4442 */
    pICB->IsrC2ResCount = pICB->IsrC2ResCount - 1u;
    /* PRQA S 4521, 4442 -- */
    Os_RCB[resId]->SaveCount = 0u;
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    pICB->Isr2CriticalZoneCount = pICB->Isr2CriticalZoneCount - 1u;
    pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount] = OS_OBJECT_MAX;
    pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount] = OS_OBJECT_INVALID; /* PRQA S 4424 */ /* VL_Os_4424 */
#endif
    Os_RCB[resId]->SaveLevel = OS_LEVEL_MAIN;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get resources of type OS_RES_OCCUPIED_BY_TASK at the task level.
 */
OS_LOCAL void Os_GetResourceByTask(Os_SCBType *pScb, ResourceType resId)
{
    Os_TCBType *pTCB = pScb->SysRunningTCB;

    Os_SaveResourceByTaskOrder(pScb, pTCB, resId);

    /* it's need to change these codes about CFG_PRIORITY_MAX.in fact,
     * the Ceiling priority is related with CFG_PRIORITY_MAX but not
     * CFG_PRIORITY_MAX.otherwise this way will restrict the space of
     * OS_TASK_STATE_READY map
     */
    Os_PriorityType savePrioTemp = pTCB->TaskRunPrio;
    Os_PriorityType prio = Os_ResourceCfg[resId].Ceiling;
    if (prio > pTCB->TaskRunPrio)
    {
        /* PRQA S 4322 ++ */ /* VL_Os_4322 */
        Os_ReadyQueueInsert((const Os_ReadyQueueManageType *)pScb->QueueMg, (Os_TaskType)resId, OS_LEVEL_STANDARD_RESOURCE, prio);
        /* PRQA S 4322 -- */
        pTCB->TaskRunPrio = prio;
    }
    if (prio > pScb->SysHighPrio)
    {
        pScb->SysHighPrio = prio;
        pScb->SysHighTaskId = pScb->SysRunningTaskId;
    }

    Os_RCB[resId]->SavePrio = savePrioTemp;
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtResStart(pScb, resId, TP_FOR_TASK);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get resources of type OS_RES_OCCUPIED_BY_TASK_OR_INTERRUPT at the task level.
 */
OS_LOCAL void Os_GetResourceByTaskOrInterrupt(Os_SCBType *pScb, ResourceType resId)
{
    Os_SaveResourceByTaskOrder(pScb, pScb->SysRunningTCB, resId);
    Os_RCB[resId]->SavePrio = Os_IplToPrio(pScb, Os_Hal_GetIpl());

    Os_Hal_SetIpl(Os_PrioToIpl(pScb, Os_ResourceCfg[resId].Ceiling), OS_ISR_DISABLE);
    pScb->SysDispatchLocker = pScb->SysDispatchLocker + 1u;

/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtResStart(pScb, resId, TP_FOR_TASK);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get OS_RES_OCCUPIED_BY_INTERRUPT or OS_RES_OCCUPIED_BY_INTERRUPT type resources at the interrupt level.
 * Service ID           <None
 */
OS_LOCAL void Os_GetResourceByInterrupt(const Os_SCBType *pScb, Os_ICBType *pICB, ResourceType resId)
{
    Os_PriorityType savePrioTemp = 0U;
#if (TRUE == CFG_INT_NEST_ENABLE)
    Os_PriorityType prio = Os_ResourceCfg[resId].Ceiling;
    savePrioTemp = Os_IplToPrio(pScb, Os_Hal_GetIpl());
    if (prio > savePrioTemp)
    {
        Os_Hal_SetIpl(Os_PrioToIpl(pScb, prio), OS_ISR_DISABLE);
    }
#endif

/* AutoSar: add resId to stack. For protection hook. */
#if (CFG_ISR2_MAX > 0)
    if (TRUE == pScb->SysInIsrCat2)
    {
        Os_SaveResourceByIsr2Order(pScb, pICB, resId);
    }
#endif

    Os_RCB[resId]->SavePrio = savePrioTemp;
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtResStart(pScb, resId, TP_FOR_ISR);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Release resources of type OS_RES_OCCUPIED_BY_TASK at the task level.
 */
OS_LOCAL StatusType Os_ReleaseResourceByTask(const Os_SCBType *pScb, ResourceType resId)
{
    Os_TCBType *pTCB = pScb->SysRunningTCB;
    Os_PriorityType prio = Os_ResourceCfg[resId].Ceiling;
    StatusType err = E_OK;

    if (Os_TaskCfg[pScb->SysRunningTaskId].TaskPriority > prio)
    {
        err = E_OS_ACCESS;
    }
    else if (Os_RCB[resId]->SaveCount != pTCB->TaskResCount)
    {
        err = E_OS_NOFUNC;
    }
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    /* PRQA S 1881 ++ */ /* VL_Os_AutosarBool */
    else if ((pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount - 1u] != OS_OBJECT_RESOURCE) || (pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount - 1u] != resId))
    /* PRQA S 1881 -- */
    {
        err = E_OS_NOFUNC;
    }
#endif
    else
    {
        Os_PriorityType savePrioTemp = Os_RCB[resId]->SavePrio;
        if (prio > savePrioTemp)
        {
            Os_ReadyQueueRemove(pScb->QueueMg, pTCB->TaskRunPrio);
            pTCB->TaskRunPrio = savePrioTemp;
        }
        Os_ResumeResourceByTaskOrder(pTCB, resId);
        OSRtiClearLockerId(resId); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Release resources of type OS_RES_OCCUPIED_BY_TASK_OR_INTERRUPT at the task level.
 */
OS_LOCAL StatusType Os_ReleaseResourceByTaskOrInterrupt(Os_SCBType *pScb, ResourceType resId)
{
    Os_TCBType *pTCB = pScb->SysRunningTCB;
    StatusType err = E_OK;

    if (Os_RCB[resId]->SaveCount != pTCB->TaskResCount)
    {
        err = E_OS_NOFUNC;
    }
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    /* PRQA S 1881 ++ */ /* VL_Os_AutosarBool */
    else if ((pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount - 1u] != OS_OBJECT_RESOURCE) || (pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount - 1u] != resId))
    /* PRQA S 1881 -- */
    {
        err = E_OS_NOFUNC;
    }
#endif
    else
    {
        Os_Hal_SetIpl(Os_PrioToIpl(pScb, Os_RCB[resId]->SavePrio), OS_ISR_ENABLE);
        pScb->SysDispatchLocker = pScb->SysDispatchLocker - 1u;

        Os_ResumeResourceByTaskOrder(pTCB, resId);
        OSRtiClearLockerId(resId); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Release OS_RES_OCCUPIED_BY_INTERRUPT or OS_RES_OCCUPIED_BY_INTERRUPT type resources at the interrupt level.
 */
OS_LOCAL StatusType Os_ReleaseResourceByInterrupt(const Os_SCBType *pScb, ResourceType resId)
{
    StatusType err = E_OK;
    Os_ICBType *pICB = Os_ICB[pScb->SysRunningIsrCat2Id];

    if (Os_RCB[resId]->SaveCount != (uint8)pICB->IsrC2ResCount)
    {
        err = E_OS_NOFUNC;
    }
/*res and spinlock should together release as LIFO order*/
#if (CFG_SPINLOCK_MAX > 0U)
    /* PRQA S 1881 ++ */ /* VL_Os_AutosarBool */
    else if ((pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount - 1u] != OS_OBJECT_RESOURCE) || (pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount - 1u] != resId))
    /* PRQA S 1881 -- */
    {
        err = E_OS_NOFUNC;
    }
#endif
    else
    {
#if (TRUE == CFG_INT_NEST_ENABLE)
        Os_PriorityType savePrioTemp = Os_RCB[resId]->SavePrio;
        Os_PriorityType prio = Os_ResourceCfg[resId].Ceiling;
        if (prio > savePrioTemp)
        {
            Os_Hal_SetIpl(Os_PrioToIpl(pScb, savePrioTemp), OS_ISR_ENABLE);
        }
#endif

/* AutoSar: add resId to stack. For protection hook. */
#if (CFG_ISR2_MAX > 0)
        if (TRUE == pScb->SysInIsrCat2)
        {
            Os_ResumeResourceByIsr2Order(pICB, resId);
        }
#endif
        OSRtiClearLockerId(resId); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_STATUS_EXTENDED == CFG_STATUS)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Safety check when the GetResource function is called.
 */
OS_LOCAL StatusType Os_GetResourceSafetyCheck(const Os_SCBType *pScb, ResourceType resId)
{
    StatusType err = E_OK;

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    /* the resource was occupied already */
    if (Os_RCB[resId]->SaveCount > 0U)
    {
        err = E_OS_ACCESS;
    }
    /* resource has no accessing authentication. */
    /* PRQA S 1258, 4522, 4523 ++ */ /* VL_Os_1258, VL_Os_4522, VL_Os_4523 */
    else if ((OS_LEVEL_TASK == pScb->SysOsLevel) && (0U == (Os_TaskResourceAccessMask[pScb->SysRunningTaskId][resId >> FOUR] & ((uint32)1u << (resId & LOW_4_BITS_MASK)))))
    /* PRQA S 1258, 4522, 4523 -- */
    {
        err = E_OS_ACCESS;
    }
    /* PRQA S 1258, 4522, 4523 ++ */ /* VL_Os_1258, VL_Os_4522, VL_Os_4523 */
    else if ((OS_LEVEL_ISR2 == pScb->SysOsLevel) && (0U == (Os_IsrResourceAccessMask[pScb->SysRunningIsrCat2Id][resId >> FOUR] & ((uint32)1u << (resId & LOW_4_BITS_MASK)))))
    /* PRQA S 1258, 4522, 4523 -- */
    {

        err = E_OS_ACCESS;
    }
    else
    {
        /* nothing to do */
    }

    OS_HAL_EXIT_CRITICAL();
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Safety check when the ReleaseResource function is called.
 */
OS_LOCAL StatusType Os_ReleaseResourceSafetyCheck(ResourceType resId)
{
    StatusType err = E_OK;

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();

    /* means that no any resource to release */
    if (0U == Os_RCB[resId]->SaveCount)
    {
        err = E_OS_NOFUNC;
    }

    OS_HAL_EXIT_CRITICAL();

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Check the validity of resId.
 */
OS_LOCAL StatusType Os_ResourceIdSafetyCheck(ResourceType resId)
{
    StatusType err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)resId, (uint8)OS_OBJECT_RESOURCE) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    if (Os_GetCoreIdLocal() != OS_RESOURCE_GET_COREID(resId)) /* PRQA S 2004 */ /* VL_Os_2004 */
    {
        err = E_OS_CORE;
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * task or ISR ocuupy a resource referenced by <ResID
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType GetResource(ResourceType ResID)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
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
    OSRtiEnterApi(pScb, OSApiId_GetResource);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetResource_Start, ResID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    err = Os_ResourceIdSafetyCheck(ResID);
    if (E_OK == err)
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_GET_RESOURCE,
            .ObjectType = OS_OBJECT_RESOURCE,
            .ObjectID = (Os_AppObjectId)ResID,
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
            err = Os_GetResourceSafetyCheck(pScb, ResID);
            if ((StatusType)E_OK == err)
#endif
            {
                err = Os_GetResource(pScb, ResID);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetResource(ResID),
                          OSServiceId_GetResource,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_GetResource);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetResource_Return, err);
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
 * Internal implementation of OS service:GetResource
 */
/* PRQA S 1505 ++ */ /* VL_Os_1505 */
StatusType Os_GetResource(Os_SCBType *pScb, ResourceType resId)
/* PRQA S 1505 -- */
{
    StatusType err = E_OK;

    /* PRQA S 3432 ++ */ /* VL_Os_3432 */
    Os_ICBType *pICB = Os_ICB[pScb->SysRunningIsrCat2Id];
    /* PRQA S 3432 -- */
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    switch (Os_ResourceCfg[resId].ResourceOccupyType)
    {
    case OS_RES_OCCUPIED_BY_TASK:
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        if (Os_TaskCfg[pScb->SysRunningTaskId].TaskPriority > Os_ResourceCfg[resId].Ceiling)
        {
            err = E_OS_ACCESS;
        }
        else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
        {
            err = E_OS_ACCESS;
        }
        else
#endif
        {
            Os_GetResourceByTask(pScb, resId);
            OSRtiSaveTaskLockerId(resId, pScb->SysRunningTaskId); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
        }
        break;

    case OS_RES_OCCUPIED_BY_TASK_OR_INTERRUPT:
        /* it's necessary to distinguish the call level of task or ISR,
         * but the current means what use only Os_ResourceOccupyType
         * can't resolve it. in fact the way of old kernel is effective
         */
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            Os_GetResourceByTaskOrInterrupt(pScb, resId);
            OSRtiSaveTaskLockerId(resId, pScb->SysRunningTaskId); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
        }
        else
        {
            Os_GetResourceByInterrupt(pScb, pICB, resId);
            /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
            /* PRQA S 3432, 4522 ++ */ /* VL_Os_3432, VL_Os_4522 */
            OSRtiSaveIsr2LockerId(resId, pScb->SysRunningIsrCat2Id);
            /* PRQA S 3432, 4522 -- */
            /* PRQA S 3138, 3141 -- */
        }
        break;

    case OS_RES_OCCUPIED_BY_INTERRUPT:
        Os_GetResourceByInterrupt(pScb, pICB, resId);
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 3432, 4522 ++ */ /* VL_Os_3432, VL_Os_4522 */
        OSRtiSaveIsr2LockerId(resId, pScb->SysRunningIsrCat2Id);
        /* PRQA S 3432, 4522 -- */
        /* PRQA S 3138, 3141 -- */
        break;

    default:
        Os_Panic();
        break;
    }
    OS_HAL_EXIT_CRITICAL();

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * task or ISR release a resource occupied before
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType ReleaseResource(ResourceType ResID)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
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
    OSRtiEnterApi(pScb, OSApiId_ReleaseResource);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReleaseResource_Start, ResID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    err = Os_ResourceIdSafetyCheck(ResID);
    if (E_OK == err)
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_RELEASE_RESOURCE,
            .ObjectType = OS_OBJECT_RESOURCE,
            .ObjectID = (Os_AppObjectId)ResID,
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
            err = Os_ReleaseResourceSafetyCheck(ResID);
            if ((StatusType)E_OK == err)
#endif
            {
                err = Os_ReleaseResource(pScb, ResID);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ReleaseResource(ResID),
                          OSServiceId_ReleaseResource,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ReleaseResource);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReleaseResource_Return, err);
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
 * task or ISR release a resource occupied before
 */
/* PRQA S 6070, 1505 ++ */ /* VL_MTR_Os_STCAL, VL_Os_1505 */
StatusType Os_ReleaseResource(Os_SCBType *pScb, ResourceType resId) /* PRQA S 3006*/ /* VL_Os_3006*/
/* PRQA S 6070, 1505 -- */
{
    StatusType err = E_OK;

    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    switch (Os_ResourceCfg[resId].ResourceOccupyType)
    {
    case OS_RES_OCCUPIED_BY_TASK:
        err = Os_ReleaseResourceByTask(pScb, resId);
        break;

    case OS_RES_OCCUPIED_BY_TASK_OR_INTERRUPT:
        if (OS_LEVEL_TASK == Os_RCB[resId]->SaveLevel)
        {
            err = Os_ReleaseResourceByTaskOrInterrupt(pScb, resId);
        }
        else
        {
            err = Os_ReleaseResourceByInterrupt(pScb, resId);
        }
        break;

    case OS_RES_OCCUPIED_BY_INTERRUPT:
        err = Os_ReleaseResourceByInterrupt(pScb, resId);
        break;

    default: /* Nothing to do. */
        break;
    }

    if ((StatusType)E_OK == err)
    {
/*Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtResEnd(pScb->SysCore, resId);
#endif

        Os_RCB[resId]->SaveCount = 0u;
        Os_RCB[resId]->SavePrio = OS_PRIORITY_INVALID;
        Os_UpdateHighPrioTask(pScb);

#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
        if (pScb->SysHighTaskId != pScb->SysRunningTaskId)
        {
            if (0u == pScb->SysDispatchLocker)
            {
                Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
            }
        }
#endif
    }
    OS_HAL_EXIT_CRITICAL();

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * get the internal resource
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_GetInternalResource(Os_SCBType *pScb)
/* PRQA S 1532 -- */
{
    if (Os_InterResCeiling[pScb->SysRunningTaskId] > Os_TCB[pScb->SysRunningTaskId]->TaskRunPrio)
    {

        Os_PriorityType prio = Os_InterResCeiling[pScb->SysRunningTaskId];

        Os_TCB[pScb->SysRunningTaskId]->TaskRunPrio = prio;

        Os_ReadyQueueInsert(pScb->QueueMg, pScb->SysRunningTaskId, OS_LEVEL_INTERNAL_RESOURCE, prio);

        if (prio > pScb->SysHighPrio)
        {
            pScb->SysHighPrio = prio;
            pScb->SysHighTaskId = pScb->SysRunningTaskId;
        }

        Os_TCB[pScb->SysRunningTaskId]->TaskOccupyInterRes = TRUE;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Release the internal resource
 */
void Os_ReleaseInternalResource(const Os_SCBType *pScb, Os_TaskType taskId)
{
    Os_TCBType *pTCB = Os_TCB[taskId];

    if ((Os_InterResCeiling[taskId] != 0u) && (pTCB->TaskOccupyInterRes == TRUE))
    {
        pTCB->TaskRunPrio = Os_TaskCfg[taskId].TaskPriority;

        Os_ReadyQueueRemove(pScb->QueueMg, Os_InterResCeiling[taskId]);

        pTCB->TaskOccupyInterRes = FALSE;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

/*=======[E N D   O F   F I L E]==============================================*/
