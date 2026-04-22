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
 **  @file               : Os_Sprot.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Sprot API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Sprot.h"
#include "Os_Appl.h"
#include "Os_Event.h"
#include "Os_Task.h"
#include "Os_Spinlock.h"
#include "Os_ReadyQue.h"
#include "Os_Tprot.h"
#include "Os_Interrupt.h"
#include "Os_Resource.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"
#include "Os_Monitor.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)

#if (CFG_SPINLOCK_MAX > 0U)
/**
 * @brief           Checks and releases occupied spinlocks for an ISR
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       pIcb: Pointer to the ISR Control Block
 * @param[out]      pSpinlockFlag: Pointer to flag indicating if spinlocks were released
 * @param[out]      pResouceFlag: Pointer to flag indicating if resources were released
 * @return          boolean
 * @retval          TRUE: Resources or spinlocks were released
 * @retval          FALSE: No resources or spinlocks were released
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_SProCheckOccupySpinlock(Os_SCBType *pScb, const Os_ICBType *pIcb, boolean *pSpinlockFlag, boolean *pResouceFlag);
#endif

/**
 * @brief           Checks and releases occupied resources
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       objectType: Type of object (TASK or ISR)
 * @return          boolean
 * @retval          TRUE: Resources were released
 * @retval          FALSE: No resources were released
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_SProCheckOccupyResource(Os_SCBType *pScb, Os_ObjectTypeType objectType);

/**
 * @brief           Checks and restores any disabled/suspended interrupts
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       pIcb: Pointer to the ISR Control Block
 * @return          boolean
 * @retval          TRUE: Interrupts were restored
 * @retval          FALSE: No interrupts were restored
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL boolean Os_SProCheckOccupyInterrupt(Os_SCBType *pScb, const Os_ICBType *pIcb);

/**
 * @brief           Deinitializes a Task Control Block during service protection
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SProDeinitializationTCB(const Os_SCBType *pScb);

/**
 * @brief           Terminates a task as part of service protection mechanism
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_SProTerminateTask(Os_SCBType *pScb);

/**
 * @brief           Handles a task that ends without proper termination
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TaskEndNoTerminate(void);

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
/**
 * @brief           Determines memory access rights for a task or Isr
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       address: Start address of memory area to check
 * @param[in]       size: Size of memory area to check
 * @param[in]       appId: ID of the application
 * @param[in]       objType: Type of object (TASK or ISR)
 * @param[in]       objId: ID of the task or ISR
 * @return          AccessType
 * @retval          Access rights for the specified memory area
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL AccessType Os_CheckObjMemAccess(const Os_SCBType *pScb, MemoryStartAddressType address, MemorySizeType size,
                                            uint16 appId, ObjectTypeType objType, ObjectType objId);
#else
/**
 * @brief           Determines the relationship between stack space and checked memory area
 * @param[in]       stack: Pointer to stack configuration array
 * @param[in]       startId: Start index in stack array
 * @param[in]       endId: End index in stack array
 * @param[in]       address: Start address of memory area to check
 * @param[in]       size: Size of memory area to check
 * @return          Os_StackAndCheckedSpaceStatus
 * @retval          STACK_INCLUDE_CHEKEDSPACE: Stack fully contains the checked space
 * @retval          STACK_CROSS_CHEKEDSPACE: Stack and checked space partially overlap
 * @retval          CHEKEDSPACE_INCLUDE_STACK: Checked space contains the entire stack
 * @retval          STACK_NOTMIXED_CHEKEDSPACE: Stack and checked space do not overlap
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL Os_StackAndCheckedSpaceStatus Os_StackAndCheckedSpace(const Os_StackType *stack, uint16 startId, uint16 endId, MemoryStartAddressType address, MemorySizeType size);

/**
 * @brief           Checks if an address range is in stack space
 * @param[out]      access: Pointer to access rights to be modified
 * @param[in]       address: Start address of memory area to check
 * @param[in]       size: Size of memory area to check
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          StatusType
 * @retval          E_OK: Address is not in stack space
 * @retval          E_NOT_OK: Address is in stack space
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_CheckAddrNotInStackSpace(AccessRefType access, MemoryStartAddressType address, MemorySizeType size, const Os_SCBType *pScb);
#endif /* OS_MPROT_REGION_CFG_ENABLE == TRUE */
/**
 * @brief           Determines access rights for an ISR2 to a memory area
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       IsrId: ID of the ISR
 * @param[in]       address: Start address of memory area to check
 * @param[in]       size: Size of memory area to check
 * @return          AccessType
 * @retval          Access rights for the specified memory area
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL AccessType Os_CheckISR2Access(const Os_SCBType *pScb, ISRType IsrId, MemoryStartAddressType address, MemorySizeType size);

/**
 * @brief           Determines memory access rights for an ISR
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       IsrId: ID of the ISR
 * @param[in]       address: Start address of memory area to check
 * @param[in]       size: Size of memory area to check
 * @return          AccessType
 * @retval          Access rights for the specified memory area
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL AccessType Os_CheckISRMemoryAccess(const Os_SCBType *pScb, ISRType IsrId, MemoryStartAddressType address, MemorySizeType size);

/**
 * @brief           Determines access rights for a task to a memory area
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       taskId: ID of the task
 * @param[in]       address: Start address of memory area to check
 * @param[in]       size: Size of memory area to check
 * @return          AccessType
 * @retval          Access rights for the specified memory area
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL AccessType Os_CheckTaskAccess(const Os_SCBType *pScb, TaskType taskId, MemoryStartAddressType address, MemorySizeType size);

/**
 * @brief           Determines memory access rights for a task
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       taskId: ID of the task
 * @param[in]       address: Start address of memory area to check
 * @param[in]       size: Size of memory area to check
 * @return          AccessType
 * @retval          Access rights for the specified memory area
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL AccessType Os_CheckTaskMemoryAccess(const Os_SCBType *pScb, TaskType taskId, MemoryStartAddressType address, MemorySizeType size);

#endif

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Os_TaskErrBack
 */
/* PRQA S 3006, 1532, 1503 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
void Os_TaskErrBack(void)
/* PRQA S 3006, 1532, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_TaskEndNoTerminate();
#endif
    OS_HAL_EXIT_KERNEL(); /* PRQA S 3138, 3141, 2743 */ /* VL_Os_PlatformNoDef *//* VL_Os_2743*/
    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* Service protection */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Service protection checks call context, access permissions, etc
 */
StatusType Os_ServiceProtCheck(const Os_SCBType *pScb, const Os_ServicePortParamType *servicePortParam)
{
    StatusType status = E_OK;
    uint16 AllowedContext = (uint16)(servicePortParam->AllowedContext);
    uint32 SprotCheckItem = servicePortParam->AllowedContext & OS_HIGH_16_BITS_MASK;

    if (Os_WrongContext((Os_SCBType *)pScb, AllowedContext) != TRUE) /* PRQA S 0311 */ /* VL_Os_0311 */
    {
        status = E_OS_CALLEVEL;
    }
    /* PRQA S 3415 ++ */ /* VL_Os_3415 */
    else if (((SprotCheckItem & OS_SERVICEPROT_CHECK_BIT_IGNORE) != 0U) && (Os_IgnoreService(pScb) != TRUE))
    {
        status = E_OS_DISABLEDINT;
    }
    else if (((SprotCheckItem & OS_SERVICEPROT_CHECK_BIT_OBJACS) != 0U) && (Os_CheckObjAcs(pScb, servicePortParam->ObjectType, servicePortParam->ObjectID) != TRUE))
    {
        status = E_OS_ACCESS;
    }
    else if (((SprotCheckItem & OS_SERVICEPROT_CHECK_BIT_WRITABLE) != 0U) && (Os_AddressWritable(pScb, servicePortParam->Address) != TRUE))
    /* PRQA S 3415 -- */
    {
        status = E_OS_ILLEGAL_ADDRESS;
    }
    else
    {
        /* Intentionally Empty */
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * If interrupts are disabled/suspended, OS service should ignore
 */
boolean Os_IgnoreService( const Os_SCBType *pScb)
{
    boolean status = TRUE;

    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        Os_TCBType *pTCB = pScb->SysRunningTCB; /* PRQA S 3678 */ /* VL_Os_3678 */
        if ((pTCB->TaskDisableAllCount > 0u) || (pTCB->TaskSuspendAllCount > 0u) || (pTCB->TaskSuspendOsCount > 0u))
        {
            status = FALSE; /*FALSE:ignore*/
        }
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        Os_ICBType* pIcb = Os_ICB[pScb->SysRunningIsrCat2Id]; /* PRQA S 3678 */ /* VL_Os_3678 */
        if ((pIcb->IsrC2DisableAllCount > 0u) || (pIcb->IsrC2SuspendAllCount > 0u) || (pIcb->IsrC2SuspendOSCount > 0u))
        {
            status = FALSE;
        }
    }
    else
    {
        /* Intentionally Empty */
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Indicate the address if writable by this OS-App
 */
boolean Os_AddressWritable(
    const Os_SCBType *pScb, uint32 address)
{
    AccessType access = 0U;
#if (OS_MPROT_REGION_CFG_ENABLE == FALSE)
    Os_ObjectTypeType ObjType = pScb->SysRunningAppObj;
    /*OS051*/
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    if (OS_OBJECT_TASK == ObjType)
    {
        Os_TaskType taskId = pScb->SysRunningTaskId;
        const Os_TaskCfgType *pTaskCfg = &Os_TaskCfg[taskId];
        if (((uint32)pTaskCfg->TaskStack.StackTop >= address) && ((uint32)pTaskCfg->TaskStack.StackBottom <= address))
        {
            access = OSMEMORY_SET_WRITEABLE(access);
        }
        else
        {
            access = Os_CheckTaskMemoryAccess(pScb, taskId, address, 0U);
        }
    }
    else if (OS_OBJECT_ISR == ObjType)
    {
        Os_IsrType Isr2ID = pScb->SysRunningIsrCat2Id;
        if (((uint32)Os_ISR2Stack[Isr2ID].StackTop >= address) && ((uint32)Os_ISR2Stack[Isr2ID].StackBottom <= address))
        {
            access = OSMEMORY_SET_WRITEABLE(access);
        }
        else
        {
            access = Os_CheckISRMemoryAccess(pScb, Isr2ID, address, 0U);
        }
    }
    else
    {
        access = OSMEMORY_SET_WRITEABLE(access);
    }
    /* PRQA S 0306 -- */
#else
    Os_ObjectTypeType ObjType = pScb->SysRunningAppObj;
    if (OS_OBJECT_TASK == ObjType)
    {
        access = Os_CheckObjMemAccess(pScb, address, 0U, pScb->SysRunningAppId, OS_OBJECT_TASK, pScb->SysRunningTaskId);
    }
    else if (OS_OBJECT_ISR == ObjType)
    {
        access = Os_CheckObjMemAccess(pScb, address, 0U, pScb->SysRunningAppId, OS_OBJECT_ISR, pScb->SysRunningIsrCat2Id);
    }

#endif
    boolean status = TRUE;
    if (0U != OSMEMORY_IS_WRITEABLE(access))
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Indicate the service if in right context
 */
boolean Os_WrongContext(const Os_SCBType *pScb, uint16 allowedContext)
{
    boolean status = FALSE;

    if ((uint16)OS_LEVEL_MAIN == (uint16)pScb->SysOsLevel)
    {
        if ((OS_CONTEXT_START_CORE == allowedContext) || (OS_CONTEXT_START_NON_AUTOSAR_CORE == allowedContext))
        {
            status = TRUE;
        }
    }
    else
    {
        /* Calculate Service Protection Level Bit */
        uint16 Os_LevelBit = (uint16)1U << ((uint16)pScb->SysOsLevel - 1U);
        if ((Os_LevelBit & allowedContext) > 0U)
        {
            status = TRUE;
        }
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_SPINLOCK_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Service protection check checks the occupancy
 *                      of the spinlock
 */
OS_LOCAL boolean Os_SProCheckOccupySpinlock(Os_SCBType *pScb, const Os_ICBType *pIcb,
                                            boolean *pSpinlockFlag, boolean *pResouceFlag)
{
    uint16 CurrentCount;
    ResourceType ResourceId;
    SpinlockIdType SpinlockId;

    for (uint16 CountIdx = pIcb->Isr2CriticalZoneCount; CountIdx > 0u; CountIdx--)
    {
        CurrentCount = pIcb->Isr2CriticalZoneCount - 1u;
        if (OS_OBJECT_SPINLOCK == pIcb->Isr2CriticalZoneType[CurrentCount])
        {
            SpinlockId = pIcb->Isr2CriticalZoneStack[CurrentCount]; /* PRQA S 4442 */ /* VL_Os_4442 */
            (void)Os_ReleaseSpinlock(pScb, SpinlockId);
            *pSpinlockFlag = TRUE;
        }
        else
        {
            ResourceId = pIcb->Isr2CriticalZoneStack[CurrentCount]; /* PRQA S 4442 */ /* VL_Os_4442 */
            (void)Os_ReleaseResource(pScb, ResourceId);
            *pResouceFlag = TRUE;
        }
    }

    boolean status = FALSE;
    if ((TRUE == *pSpinlockFlag) || (TRUE == *pResouceFlag))
    {
        status = TRUE;
    }
    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Service protection check checks the occupancy
 *                      of the resource
 */
OS_LOCAL boolean Os_SProCheckOccupyResource(Os_SCBType *pScb, Os_ObjectTypeType objectType)
{
    uint8 OsResLoopi;
    boolean status = FALSE;

    if (OS_OBJECT_TASK == objectType)
    {
        Os_TCBType *pCurTaskTcb = pScb->SysRunningTCB; /* PRQA S 3678 */ /* VL_Os_3678 */
        uint8 taskResCountTmp = pCurTaskTcb->TaskResCount;
        if (taskResCountTmp > 0U)
        {
            pScb->SysDispatchLocker = pScb->SysDispatchLocker + 1u;

            for (OsResLoopi = taskResCountTmp; OsResLoopi > 0U; OsResLoopi--)
            {
                (void)Os_ReleaseResource(pScb, pCurTaskTcb->TaskResourceStack[OsResLoopi - 1U]);
            }

            status = TRUE;
        }
    }
    else if (OS_OBJECT_ISR == objectType)
    {
        Os_ICBType *pIcb = Os_ICB[pScb->SysRunningIsrCat2Id];
        uint8 isr2ResCountTmp = (uint8)pIcb->IsrC2ResCount;
        /*OS369*/
        /* free the resources occupied on the ISRs*/
        if (isr2ResCountTmp > 0U)
        {
            for (OsResLoopi = isr2ResCountTmp; OsResLoopi > 0U; OsResLoopi--)
            {
                (void)Os_ReleaseResource(pScb, pIcb->IsrC2ResourceStack[OsResLoopi - 1U]);
            }

            pIcb->IsrC2ResCount = (Os_ResourceType)0U; /* PRQA S 4342 */ /* VL_Os_4342 */
            status = TRUE;
        }
    }
    else
    {
        /* Intentionally Empty */
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Service protection check checks the occupancy
 *                      of the interrupt
 */
OS_LOCAL boolean Os_SProCheckOccupyInterrupt(Os_SCBType *pScb, const Os_ICBType *pIcb)
{
    boolean status = FALSE;

    /*OS368*/
    if ((pIcb->IsrC2DisableAllCount > 0U) || (pIcb->IsrC2SuspendAllCount > 0U) || (pIcb->IsrC2SuspendOSCount > 0U))
    {
        if (pIcb->IsrC2DisableAllCount > 0U)
        {
            Os_EnableAllInterrupts(pScb);
        }
        while (pIcb->IsrC2SuspendAllCount > 0U) /* PRQA S 2872 */ /* VL_Os_2872 */
        {
            Os_ResumeAllInterrupts(pScb);
        }
        while (pIcb->IsrC2SuspendOSCount > 0U) /* PRQA S 2872 */ /* VL_Os_2872 */
        {
            Os_ResumeOSInterrupts(pScb);
        }

        status = TRUE;
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Service protection deinitialization the TCB
 */
OS_LOCAL void Os_SProDeinitializationTCB(const Os_SCBType *pScb)
{
    Os_TCBType *runningTCB = pScb->SysRunningTCB;

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Terminate, pScb->SysRunningTaskId);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */
#if ((OS_BCC1 == CFG_CC) || (OS_ECC1 == CFG_CC))
    runningTCB->TaskState = OS_TASK_STATE_SUSPENDED;
#else
    if (runningTCB->TaskActCount > 0U)
    {
        runningTCB->TaskActCount = runningTCB->TaskActCount - 1U;
    }

    if (runningTCB->TaskActCount > 0U)
    {
        runningTCB->TaskState = OS_TASK_STATE_START;
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
        /* PRQA S 4543, 4523, 3762, 1277, 2985 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277, VL_Os_2985 */
        ARTI_TRACE(NOSUSP, AR_CP_OS_TASK, Os, pScb->SysCore, OsTask_Activate, pScb->SysRunningTaskId);
        /* PRQA S 4543, 4523, 3762, 1277, 2985 -- */
        /* PRQA S 1821, 4532, 4544, 4542 -- */
        /* PRQA S 3138, 3141 -- */
    }
    else
    {
        runningTCB->TaskState = OS_TASK_STATE_SUSPENDED;
    }
#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
    runningTCB->TaskResCount = 0U;
#endif

#if (CFG_EXTENDED_TASK_MAX > 0U)
    if (Os_CheckExternalTaskId(pScb->SysRunningTaskId, pScb->SysCore))
    {
        Os_ClearECB(pScb->SysRunningTaskId);
    }
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Service protection terminates the task
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Os_STCAL */
OS_LOCAL void Os_SProTerminateTask(Os_SCBType *pScb) /* PRQA S 3006*/ /* VL_Os_3006*/
/* PRQA S 6070 -- */
{
    OS_HAL_DECLARE_CRITICAL();

#if (CFG_STD_RESOURCE_MAX > 0U)
    (void)Os_SProCheckOccupyResource(pScb, OS_OBJECT_TASK);
#endif

/*OS239*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_Hal_EnableAllInt_ButTimingProtInt(pScb->SysCore);
#else
    Os_Hal_EnableInt(); /* PRQA S 1753, 1512, 1513 */ /* VL_Os_1753, VL_Os_1512, VL_Os_1513 */
#endif

    OS_HAL_ENTRY_CRITICAL();
    Os_PostTaskHook(pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    Os_ReleaseInternalResource(pScb, pScb->SysRunningTaskId);
#endif

    /* Deinitialize the task control block */
    Os_SProDeinitializationTCB(pScb);

    Os_ReadyQueueRemove(pScb->QueueMg, Os_TCB[pScb->SysRunningTaskId]->TaskRunPrio);

    pScb->SysRunningTCB->TaskRunPrio = Os_TaskCfg[pScb->SysRunningTaskId].TaskPriority;

    if (pScb->SysHighTaskId == pScb->SysRunningTaskId)
    {
        Os_UpdateHighPrioTask(pScb);
    }

/* Timing protection: reset task exe time. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtTaskEnd(pScb->SysCore, pScb->SysRunningTaskId, TP_EXE);
#endif

    Os_ErrorHook(E_OS_MISSINGEND); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

    pScb->SysDispatchLocker = 0U;
    Os_Hal_Dispatch(); /* PRQA S 1006*/ /* VL_Os_1006*/
    OS_HAL_EXIT_CRITICAL();

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Task ends without calling a TerminateTask or ChainTask
 */
OS_LOCAL void Os_TaskEndNoTerminate(void)
{
    Os_SCBType *pScb = Os_GetCurrentContext();

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_TaskRecordTotalTick(pScb->SysRunningTaskId);
#endif

    /*OS052,OS070*/
    Os_SProTerminateTask(pScb);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ISR2 end with checks for occupancy of interrupts
 *                       and resources and spinlocks
 */
void Os_Isr2OccupyIntRes(Os_IsrType isrId)
{
    Os_SCBType *pScb = OS_ISR_GET_SCB(isrId);
    Os_ICBType *pIcb = Os_ICB[pScb->SysRunningIsrCat2Id]; /* PRQA S 3678 */ /* VL_Os_3678 */
    boolean status = FALSE;

#if (CFG_SPINLOCK_MAX > 0U)
    boolean SpinlockFlag = FALSE;
    boolean ResouceFlag = FALSE;
    status = Os_SProCheckOccupySpinlock(pScb, pIcb, &SpinlockFlag, &ResouceFlag);
    if (TRUE == status)
    {
        if (TRUE == SpinlockFlag)
        {
            Os_ErrorHook(E_OS_SPINLOCK); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
        }
        if (TRUE == ResouceFlag)
        {
            Os_ErrorHook(E_OS_RESOURCE); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
        }
    }
#elif (CFG_STD_RESOURCE_MAX > 0)
    status = Os_SProCheckOccupyResource(pScb, OS_OBJECT_ISR);
    if (TRUE == status)
    {
        Os_ErrorHook(E_OS_RESOURCE); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    status = Os_SProCheckOccupyInterrupt(pScb, pIcb);
    if (TRUE == status)
    {
        Os_ErrorHook(E_OS_DISABLEDINT); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }

    UNUSED_PARAMETER(isrId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Check object access to the calling Task/Isr2
 */
boolean Os_CheckObjAcs(
    const Os_SCBType *pScb,
    ObjectTypeType objectType,
    Os_AppObjectId objectID)
{
    boolean status = TRUE;

    /*OS056*/
    if ((OS_LEVEL_TASK == pScb->SysOsLevel) || (OS_LEVEL_ISR2 == pScb->SysOsLevel))
    {
        if (OS_NO_ACCESS == Os_CheckObjectAccess(pScb, pScb->SysRunningAppId, objectType, objectID))
        {
            status = FALSE; /*no access*/
        }
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function find out the according access right of
 *                       ISR on the assigned memory area
 */
/* PRQA S 1503, 3408, 3006, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
AccessType CheckISRMemoryAccess(
    ISRType ISRID,
    MemoryStartAddressType Address,
    MemorySizeType Size)
/* PRQA S 1503, 3408, 3006, 6070, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    AccessType Access = (AccessType)OS_NO_PERMISSION;
    StatusType Status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_CheckISRMemoryAccess);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CheckISRMemoryAccess_Start, ISRID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

/* OS268 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (!Os_CheckIsr2Id(ISRID))
    {
        Status = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)

        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_ISR_MEMORY_ACCESS,
            .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
            .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        Status = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == Status)
#endif
        {
            Access = Os_CheckISRMemoryAccess(pScb, ISRID, Address, Size);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (Status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_CheckISRMemoryAccess(ISRID, Address, Size),
                          OSServiceId_CheckISRMemoryAccess,
                          Status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_CheckISRMemoryAccess);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CheckISRMemoryAccess_Return, Access);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(Status);
    return Access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_MPROT_REGION_CFG_ENABLE == FALSE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function find out the checked space whether has
 *                      nothing to do with stack
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Os_STMIF */
OS_LOCAL Os_StackAndCheckedSpaceStatus Os_StackAndCheckedSpace(
    const Os_StackType *stack,
    uint16 startId,
    uint16 endId,
    MemoryStartAddressType address,
    MemorySizeType size)
/* PRQA S 6030 -- */
{
    Os_StackAndCheckedSpaceStatus Status = STACK_NOTMIXED_CHEKEDSPACE;

    /*  if the checked space is the stackspace*/
    for (uint16 i = startId; i < endId; i++) /* PRQA S 0771 */ /* VL_Os_0771 */
    {
        /* PRQA S 3679 ++ */ /*VL_Os_3679*/
        Os_StackPtrType stackBottom = stack[i].StackBottom;
        Os_StackPtrType stackTop = stack[i].StackTop;
        /* PRQA S 3679 -- */

        /* PRQA S 0306 ++ */ /* VL_Os_0306 */
        if ((address >= (MemoryStartAddressType)stackBottom) && (address <= (MemoryStartAddressType)stackTop))
        {
            /*the checked space is one part of the stack*/
            /*                         |    checked space     |          */
            /*                  |            stack                  |    */ /* PRQA S 2053 */ /* VL_Os_2053 */
            if ((address + size) <= (MemoryStartAddressType)stackTop)
            {
                Status = STACK_INCLUDE_CHEKEDSPACE;
                break;
            }
            /*the checked space cross upper the stack*/
            /*                 |    checked space     |                 */
            /*                         |       stack       |            */ /* PRQA S 2053 */ /* VL_Os_2053 */
            else
            {
                Status = STACK_CROSS_CHEKEDSPACE;
                break;
            }
        }
        else if (address < (MemoryStartAddressType)stackBottom)
        {
            /*the checked space include one whole stack, but not equeal to
             * the stack*/
            /*                         |    checked space     |         */
            /*                                | stack   |               */ /* PRQA S 2053 */ /* VL_Os_2053 */
            if ((address + size) > (MemoryStartAddressType)stackTop)
            {
                Status = CHEKEDSPACE_INCLUDE_STACK;
                break;
            }
            /*the checked space cross lower the stack*/
            /*                         |    checked space     |         */
            /*                      |       stack    |                  */ /* PRQA S 2053 */ /* VL_Os_2053 */
            else if ((address + size) >= (MemoryStartAddressType)stackBottom)
            {
                Status = STACK_CROSS_CHEKEDSPACE;
                break;
            }
            else
            {
                /* Intentionally Empty */
            }
        }
        else
        {
            /* Intentionally Empty */
        }
        /* PRQA S 0306 -- */

        /*if the code step to here, the stack and checked space are not mixed            */
        /*                                          |checked space|                    */
        /*                                                                       |stack |   */ /* PRQA S 2053 */ /* VL_Os_2053 */
        /*                           or                                                                            */
        /*                                          |checked space|                    */
        /*              |stack |                                                            */ /* PRQA S 2053 */ /* VL_Os_2053 */
        /*exclude the conditions before ,in this condition, should return no
         * access this should be checked at last,because there are lots of
         * stacks need to be check */
        Status = STACK_NOTMIXED_CHEKEDSPACE;
    }

    return Status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function find out the according access right of
 *                      task on the assigned memory area
 */
OS_LOCAL StatusType Os_CheckAddrNotInStackSpace(
    AccessRefType access,
    MemoryStartAddressType address,
    MemorySizeType size,
    const Os_SCBType *pScb)
{
    StatusType StackSpaceErr = E_OK;

    /*if the checked space is the system stackspace*/
    Os_StackAndCheckedSpaceStatus StackStatus = Os_StackAndCheckedSpace(pScb->SystemStack, 0U,
                                                                        CFG_SYSTEM_STACK_MAX, address, size);
    if (STACK_INCLUDE_CHEKEDSPACE == StackStatus)
    {
        *access = OSMEMORY_SET_STACKSPACE(*access);
        StackSpaceErr = E_NOT_OK;
    }
    else if ((CHEKEDSPACE_INCLUDE_STACK == StackStatus) || (STACK_CROSS_CHEKEDSPACE == StackStatus))
    {
        *access = (StatusType)OSMEMORY_SET_NOACCESS(*access);
        StackSpaceErr = E_NOT_OK;
    }
    else
    {
        /* nothing to do */
    }
    if ((StatusType)E_OK == StackSpaceErr)
    {
        /*if the checked space is the task stackspace*/
        Os_TaskType TaskStartId = Os_TaskIdRange[pScb->SysCore].AllTask.Start;
        Os_TaskType TaskEndId = Os_TaskIdRange[pScb->SysCore].AllTask.End;
        StackStatus = Os_StackAndCheckedSpace((const Os_StackType *)Os_TaskStack, (uint16)TaskStartId, (uint16)TaskEndId, address, size);
        if (STACK_INCLUDE_CHEKEDSPACE == StackStatus)
        {
            *access = OSMEMORY_SET_STACKSPACE(*access);
            StackSpaceErr = E_NOT_OK;
        }
        else if ((CHEKEDSPACE_INCLUDE_STACK == StackStatus) || (STACK_CROSS_CHEKEDSPACE == StackStatus))
        {
            *access = (StatusType)OSMEMORY_SET_NOACCESS(*access);
            StackSpaceErr = E_NOT_OK;
        }
        else
        {
            /* nothing to do */
        }
    }
    if ((StatusType)E_OK == StackSpaceErr)
    {
/*if the checked space is the ISR stackspace*/
#if (CFG_ISR2_MAX > 0U)
        Os_IsrType Isr2StartId = Os_IsrIdRange[pScb->SysCore].Isr2.IsrStart;
        Os_IsrType Isr2EndId = Os_IsrIdRange[pScb->SysCore].Isr2.IsrEnd;
        StackStatus = Os_StackAndCheckedSpace((const Os_StackType *)Os_ISR2Stack, (uint16)Isr2StartId, (uint16)Isr2EndId, address, size);
#endif

        if (STACK_INCLUDE_CHEKEDSPACE == StackStatus)
        {
            *access = OSMEMORY_SET_STACKSPACE(*access);
            StackSpaceErr = E_NOT_OK;
        }
        else if ((CHEKEDSPACE_INCLUDE_STACK == StackStatus) || (STACK_CROSS_CHEKEDSPACE == StackStatus))
        {
            *access = (StatusType)OSMEMORY_SET_NOACCESS(*access);
            StackSpaceErr = E_NOT_OK;
        }
        else
        {
            /*nothing to do*/
        }
    }

    return StackSpaceErr;
}

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function find out the according access right of
 *                      task on the assigned memory area
 */
/* PRQA S 6030, 6010 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC */
OS_LOCAL AccessType Os_CheckISR2Access(
    const Os_SCBType *pScb,
    ISRType IsrId,
    MemoryStartAddressType address,
    MemorySizeType size)
/* PRQA S 6030, 6010 -- */
{
    /*Indicate the ISR belong to which app*/
    AccessType access = (AccessType)OS_NO_PERMISSION;


#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
    uint16 appId = Os_GetObjectAppID(OS_OBJECT_ISR, (Os_AppObjectId)IsrId);
    access = Os_CheckObjMemAccess(pScb, address, size, appId, OS_OBJECT_ISR, IsrId);
#else
    /*the access area is on the ram*/
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    if (OS_HAL_ADDRESS_IS_IN_ALL_RAM(address, size))
    {
        if ((StatusType)E_OK == Os_CheckAddrNotInStackSpace(&access, address, size, pScb))
        {
            /*find out which app the isr is belong under,and the app is trusted
             * or not */
            uint16 appsuborID = Os_GetObjectAppID(OS_OBJECT_ISR, (Os_AppObjectId)IsrId);

            /*trusted app's ISRs and tasks have both read and write access on the
             *  whole ram*/
            if (TRUE == Os_AppCfg[appsuborID].Trusted)
            {
                access = OSMEMORY_SET_READABLE(access);
                if (Os_AppCfg[appsuborID].TrustedAppWithProtection != TRUE)
                {
                    access = OSMEMORY_SET_WRITEABLE(access);
                }
                else
                {
                    /*SWS_Os_00795*/
                    /*write access only for father app's private data and the Isr's
                     * data*/
                    if ((((uint32)Os_AppPriDataAddr[appsuborID].AppAddrStart <= address) && ((uint32)Os_AppPriDataAddr[appsuborID].AppAddrEnd >= (address + size))) || (((uint32)Os_IsrDAddr[IsrId].IsrAddrStart <= address) && ((uint32)Os_IsrDAddr[IsrId].IsrAddrEnd >= (address + size))))
                    {
                        access = OSMEMORY_SET_WRITEABLE(access);
                    }
                }
            }
            /*hand on non-trusted app's ISR*/
            /*read access for all the ram,write access only for father app's private
             *  data and the ISR's data*/
            else
            {
                /*read access for all the ram, except for ohter app's data*/
                if (OS_HAL_ADDRESS_IS_NOT_IN_OTHER_APP(address, size, pScb->SysCore, appsuborID)) /* PRQA S 2995*/ /* VL_Os_2995*/
                {
                    access = OSMEMORY_SET_READABLE(access);
                }
                /*write access only for father app's private data and the Task's
                 * data*/
                if ((((uint32)Os_AppPriDataAddr[appsuborID].AppAddrStart <= address) && ((uint32)Os_AppPriDataAddr[appsuborID].AppAddrEnd >= (address + size))) || (((uint32)Os_IsrDAddr[IsrId].IsrAddrStart <= address) && ((uint32)Os_IsrDAddr[IsrId].IsrAddrEnd >= (address + size))))
                {
                    access = OSMEMORY_SET_WRITEABLE(access);
                }
            }
        }
    }
    /*the access area is on the flash*/
    else if (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart <= address) && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd >= (address + size)))
    {
        /*need to access RoData area,read right*/
        if (((uint32)Os_RoData_DAddr.RodataAddrStart <= address) && ((uint32)Os_RoData_DAddr.RodataAddrEnd >= (address + size)))
        {
            access = OSMEMORY_SET_READABLE(access);
        }
        /*Rodata is on the right or left side of the checked space*/
        else if (((uint32)Os_RoData_DAddr.RodataAddrStart >= (address + size)) || ((uint32)Os_RoData_DAddr.RodataAddrEnd <= address))
        {
            access = OSMEMORY_SET_EXECUTABLE(access);
        }
        else /* the area cross Rodata and others,do not allow to access*/
        {
            access = (AccessType)OSMEMORY_SET_NOACCESS(access);
        }
    }
    else
    {
        access = (AccessType)OSMEMORY_SET_NOACCESS(access);
    }
    /* PRQA S 0306 -- */
#endif

    return access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:CheckISRMemoryAccess
 */
OS_LOCAL AccessType Os_CheckISRMemoryAccess(
    const Os_SCBType *pScb,
    ISRType IsrId,
    MemoryStartAddressType address,
    MemorySizeType size)
{
    AccessType access = (AccessType)OS_NO_PERMISSION;
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();

    if (OS_ISR_GET_COREID(IsrId) != pScb->SysCore)
    {
        access = (AccessType)OSMEMORY_SET_NOACCESS(access);
    }
    else
    {
        /*get the access right*/
        access = Os_CheckISR2Access(pScb, IsrId, address, size);
    }
    OS_HAL_EXIT_CRITICAL();

    return access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function find out the according access right of
 *                      task on the assigned memory area
 */
/* PRQA S 1503, 6070, 3006, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Os_STCAL, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
AccessType CheckTaskMemoryAccess(
    TaskType TaskID,
    MemoryStartAddressType Address,
    MemorySizeType Size)
/* PRQA S 1503, 6070, 3006, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    AccessType Access = (AccessType)OS_NO_PERMISSION;
    StatusType Status = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_CheckTaskMemoryAccess);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CheckTaskMemoryAccess_Start, TaskID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

    /* OS268 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (!CheckCoreTaskId(TaskID, pScb->SysCore)) /* PRQA S 3469 */ /* VL_Os_3469 */
    {
        Status = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)

        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_TASK_MEMORY_ACCESS,
            .ObjectType = (Os_ObjectTypeType)OS_OBJECT_INVALID,
            .ObjectID = (Os_AppObjectId)OS_OBJECT_INVALID,
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        Status = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == Status)
#endif
        {
            Access = Os_CheckTaskMemoryAccess(pScb, TaskID, Address, Size);
        }
    }
#if (CFG_ERRORHOOK == TRUE)
    if (Status != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_CheckTaskMemoryAccess(TaskID, Address, Size),
                          OSServiceId_CheckTaskMemoryAccess,
                          Status, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_CheckTaskMemoryAccess);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CheckTaskMemoryAccess_Return, Access);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(Status);
    return Access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function find out the according access right of
 *                      task on the assigned memory area
 */
/* PRQA S 6030, 6010 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC */
OS_LOCAL AccessType Os_CheckTaskAccess(
    const Os_SCBType *pScb,
    TaskType taskId,
    MemoryStartAddressType address,
    MemorySizeType size)
/* PRQA S 6030, 6010 -- */
{
    /*Indicate the Task belong to which app*/
    AccessType access = (AccessType)OS_NO_PERMISSION;

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
    uint16 appId = Os_GetObjectAppID(OS_OBJECT_TASK, (Os_AppObjectId)taskId);
    access = Os_CheckObjMemAccess(pScb, address, size, appId, OS_OBJECT_TASK, taskId);
#else
    /*the access area is on the ram*/
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    if (OS_HAL_ADDRESS_IS_IN_ALL_RAM(address, size))
    {
        if ((StatusType)E_OK == Os_CheckAddrNotInStackSpace(&access, address, size, pScb))
        {
            /*find out which app the isr is belong under,and the app is trusted
             * or not */
            uint16 appsuborID = Os_GetObjectAppID(OS_OBJECT_TASK, (Os_AppObjectId)taskId);

            /*trusted app's Tasks and tasks have both read and write access on the
             * whole ram*/
            if (TRUE == Os_AppCfg[appsuborID].Trusted)
            {
                access = OSMEMORY_SET_READABLE(access);
                if (Os_AppCfg[appsuborID].TrustedAppWithProtection != TRUE)
                {
                    access = OSMEMORY_SET_WRITEABLE(access);
                }
                else
                {
                    /*SWS_Os_00795*/
                    /*write access only for father app's private data and the
                     * Task's data*/
                    if ((((uint32)Os_AppPriDataAddr[appsuborID].AppAddrStart <= address) && ((uint32)Os_AppPriDataAddr[appsuborID].AppAddrEnd >= (address + size))) || (((uint32)Os_TaskDAddr[taskId].TaskAddrStart <= address) && ((uint32)Os_TaskDAddr[taskId].TaskAddrEnd >= (address + size))))
                    {
                        access = OSMEMORY_SET_WRITEABLE(access);
                    }
                }
            }
            /*hand on non-trusted app's Task*/
            else
            {
                /*read access for all the ram, except for ohter app's data*/
                if (OS_HAL_ADDRESS_IS_NOT_IN_OTHER_APP(address, size, pScb->SysCore, appsuborID)) /* PRQA S 2995*/ /* VL_Os_2995*/
                {
                    access = OSMEMORY_SET_READABLE(access);
                }
                /*write access only for father app's private data and the Task's
                 * data*/
                if ((((uint32)Os_AppPriDataAddr[appsuborID].AppAddrStart <= address) && ((uint32)Os_AppPriDataAddr[appsuborID].AppAddrEnd >= (address + size))) || (((uint32)Os_TaskDAddr[taskId].TaskAddrStart <= address) && ((uint32)Os_TaskDAddr[taskId].TaskAddrEnd >= (address + size))))
                {
                    access = OSMEMORY_SET_WRITEABLE(access);
                }
            }
        }
    }
    /*the access area is on the flash*/
    else if (((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomStart <= address) && ((uint32)Os_MemProtKnAddrCfg.OsKernelAddr.pRomEnd >= (address + size)))
    {
        /*need to access RoData area,read right*/
        if (((uint32)Os_RoData_DAddr.RodataAddrStart <= address) && ((uint32)Os_RoData_DAddr.RodataAddrEnd >= (address + size)))
        {
            access = OSMEMORY_SET_READABLE(access);
        }
        /*Rodata is on the right or left side of the checked space*/
        else if (((uint32)Os_RoData_DAddr.RodataAddrStart >= (address + size)) || ((uint32)Os_RoData_DAddr.RodataAddrEnd <= address))
        {
            access = OSMEMORY_SET_EXECUTABLE(access);
        }
        else /* the area cross Rodata and others,do not allow to access*/
        {
            access = (AccessType)OSMEMORY_SET_NOACCESS(access);
        }
    }
    else
    {
        access = (AccessType)OSMEMORY_SET_NOACCESS(access);
    }
    /* PRQA S 0306 -- */
#endif

    return access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:CheckTaskMemoryAccess
 */
OS_LOCAL AccessType Os_CheckTaskMemoryAccess(
    const Os_SCBType *pScb,
    TaskType taskId,
    MemoryStartAddressType address,
    MemorySizeType size)
{
    AccessType access = (AccessType)OS_NO_PERMISSION;
    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    if (OS_TASK_GET_COREID(taskId) != pScb->SysCore)
    {
        access = (AccessType)OSMEMORY_SET_NOACCESS(access);
    }
    else
    {
        /*get the access right*/
        access = Os_CheckTaskAccess(pScb, taskId, address, size);
    }
    OS_HAL_EXIT_CRITICAL();

    return access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_MPROT_REGION_CFG_ENABLE == TRUE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function find out the according access right of
 *                      task on the assigned memory area
 */
OS_LOCAL AccessType Os_CheckObjMemAccess(const Os_SCBType *pScb, MemoryStartAddressType address, MemorySizeType size, 
                                            uint16 appId, ObjectTypeType objType, ObjectType objId)
{
    AccessType access = (AccessType)OS_NO_PERMISSION;
    boolean readWariteFlag = FALSE;
    MemoryStartAddressType startAddress = address;
    MemoryStartAddressType endAddress = address + size;

    if ((TRUE == Os_AppCfg[appId].Trusted) && (TRUE != Os_AppCfg[appId].TrustedAppWithProtection))
    {
        access = (AccessType)OSMEMORY_SET_READABLE(access);
        access = (AccessType)OSMEMORY_SET_WRITEABLE(access);
    }
    else
    {
        const Os_MemoryRegionObjType * coreMemRegion = &Os_Memory_Region_Core_Inf[pScb->SysCore];
        if ((0 != coreMemRegion->regionsNum) && (NULL_PTR != coreMemRegion))
        {
            for (uint8 i = 0; i < coreMemRegion->regionsNum; i ++)
            {
                const Os_MemoryRegionAttrType *pRegion = coreMemRegion->regionsRef[i];
                if ((startAddress >= pRegion->startAddr) && (endAddress <= pRegion->endAddr))
                {
                    access |= pRegion->access;
                    if ((0U != OSMEMORY_IS_WRITEABLE(access)) && (0U != OSMEMORY_IS_READABLE(access)))
                    {
                        readWariteFlag = TRUE;
                        break;
                    }
                }
            }
        } 

        if (TRUE != readWariteFlag)
        {
        	const Os_MemoryRegionObjType * appMemRegion = &Os_Memory_Region_Application_Inf[appId];
            if ((0 != appMemRegion->regionsNum) && (NULL_PTR != appMemRegion))
            {
                for (uint8 i = 0; i < appMemRegion->regionsNum; i ++)
                {
                	const Os_MemoryRegionAttrType *pRegion = appMemRegion->regionsRef[i];
                    if ((startAddress >= pRegion->startAddr) && (endAddress <= pRegion->endAddr))
                    {
                        access |= pRegion->access;
                        if ((0U != OSMEMORY_IS_WRITEABLE(access)) && (0U != OSMEMORY_IS_READABLE(access)))
                        {
                            readWariteFlag = TRUE;
                            break;
                        }
                    }
                }
            }
        }

        if (TRUE != readWariteFlag)
        {
        	const Os_MemoryRegionObjType * objMemRegion;

            if (objType == OS_OBJECT_ISR)
            {
                objMemRegion = &Os_Memory_Region_Isr_Inf[objId];
            }
            else
            {
                objMemRegion = &Os_Memory_Region_Task_Inf[objId];
            }

            if ((0 != objMemRegion->regionsNum) && (NULL_PTR != objMemRegion))
            {
                for (uint8 i = 0; i < objMemRegion->regionsNum; i++)
                {
                	const Os_MemoryRegionAttrType *pRegion = objMemRegion->regionsRef[i];
                    if ((startAddress >= pRegion->startAddr) && (endAddress <= pRegion->endAddr))
                    {
                        access |= pRegion->access;
                        if ((0U != OSMEMORY_IS_WRITEABLE(access)) && (0U != OSMEMORY_IS_READABLE(access)))
                        {
                            break;
                        }
                    }
                }
            }
        }
    }

    return access;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_MPROT_REGION_CFG_ENABLE == TRUE */
#endif

/*=======[E N D   O F   F I L E]==============================================*/
