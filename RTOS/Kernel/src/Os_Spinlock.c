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
 **  @file               : Os_Spinlock.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Spinlock API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Spinlock.h"
#include "Os_Appl.h"
#include "Os_Hook.h"
#include "Os_Sprot.h"
#include "Os_Resource.h"
#include "Os_Interrupt.h"
#include "Os_Task.h"
#include "Os_ScheduleTable.h"
#include "Os_ProtectHook.h"
#include "Os_Kernel.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */
#if (OS_AUTOSAR_CORES > 1U)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
Os_SpinlockType Os_SpinlockSync;
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
#endif

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (CFG_SPINLOCK_MAX > 0U)
/**
 * @brief           Performs safety checks before acquiring a spinlock
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       spinlockId: ID of the spinlock to be checked
 * @return          StatusType
 * @retval          E_OK: Spinlock can be safely acquired
 * @retval          E_OS_INTERFERENCE_DEADLOCK: The spinlock is already occupied by this core
 * @retval          E_OS_NESTING_DEADLOCK: Spinlock acquisition would violate order or grouping rules
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
OS_LOCAL StatusType Os_GetSpinlockCheck(const Os_SCBType *pScb, SpinlockIdType spinlockId);
#endif

/**
 * @brief           Performs safety checks before releasing a spinlock
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       spinlockId: ID of the spinlock to be released
 * @return          StatusType
 * @retval          E_OK: Spinlock can be safely released
 * @retval          E_OS_STATE: The spinlock is not owned by the calling task/ISR
 * @retval          E_OS_NOFUNC: Attempt to release a spinlock not following LIFO order
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
#if (OS_STATUS_EXTENDED == CFG_STATUS)
OS_LOCAL StatusType Os_ReleaseSpinlockCheck(const Os_SCBType *pScb, SpinlockIdType spinlockId);
#endif

/**
 * @brief           Applies the lock method specified for a spinlock
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       spinlockId: ID of the spinlock
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_MethodLock(Os_SCBType *pScb, SpinlockIdType spinlockId);

/**
 * @brief           Releases the lock method specified for a spinlock
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       spinlockId: ID of the spinlock
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_MethodRelease(Os_SCBType *pScb, SpinlockIdType spinlockId);

/**
 * @brief           Updates task or ISR control blocks after successfully acquiring a spinlock
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       spinlockId: ID of the acquired spinlock
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetCBModify(const Os_SCBType *pScb, SpinlockIdType spinlockId);

/**
 * @brief           Updates task control block after releasing a spinlock
 * @param[inout]    pTCB: Pointer to the Task Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ReleaseTCBModify(Os_TCBType *pTCB);

/**
 * @brief           Updates ISR control block after releasing a spinlock
 * @param[inout]    pICB: Pointer to the ISR Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ReleaseICBModify(Os_ICBType *pICB);

/**
 * @brief           Updates task or ISR control blocks after releasing a spinlock
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ReleaseCBModify(const Os_SCBType *pScb);

/**
 * @brief           Internal implementation for acquiring a spinlock
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       spinlockId: ID of the spinlock to acquire
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetSpinlock(Os_SCBType *pScb, SpinlockIdType spinlockId);

/**
 * @brief           Internal implementation for attempting to acquire a spinlock
 * @param[in]       pScb: Pointer to the System Control Block
 * @param[in]       spinlockId: ID of the spinlock to acquire
 * @param[out]      success: Pointer to store acquisition result
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_TryToGetSpinlock(Os_SCBType *pScb, SpinlockIdType spinlockId, TryToGetSpinlockType *success);
#endif

/* ========================================== external function definitions ========================================= */
#if (CFG_SPINLOCK_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Initialize the spin lock
 */
void Os_InitSpinlock(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if (OS_CORE_ID_MASTER == coreId)
    {
        for (uint16 i = 0u; i < CFG_SPINLOCK_MAX; i++)
        {
            Os_SLCB[i].ObjOccupyType = OS_OBJECT_MAX;
            Os_SLCB[i].ObjOccupyId = OS_OBJECT_INVALID; /* PRQA S 4424 */ /* VL_Os_4424 */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (OS_STATUS_EXTENDED == CFG_STATUS)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * obtain a spin lock check
 */

OS_LOCAL StatusType Os_GetSpinlockCheck(const Os_SCBType *pScb, SpinlockIdType spinlockId)
{
    StatusType err = E_OK;
    const Os_SpinlockCfgType *pSpinlockCfg = &Os_SpinlockCfg[spinlockId];
    Os_TCBType *pTCB = pScb->SysRunningTCB; /* PRQA S 3678 */ /* VL_Os_3678 */
    Os_ICBType *pICB = Os_ICB[pScb->SysRunningIsrCat2Id]; /* PRQA S 3678 */ /* VL_Os_3678 */

    /*Os_00690*/
    if (pScb->CurrentSpinlockOccupied[spinlockId] == TRUE)
    {
        err = E_OS_INTERFERENCE_DEADLOCK;
    }
    /* PRQA S 1881, 4342, 3442, 3345 ++ */ /* VL_Os_AutosarBool, VL_Os_4342, VL_Os_3442, VL_Os_3345 */
    else if ((OS_LEVEL_TASK == pScb->SysOsLevel) && (OS_SPINLOCK_INVALID != pTCB->TaskCurrentSpinlockOccupyLevel) && ((pSpinlockCfg->SpinlockOrder <= Os_SpinlockCfg[pTCB->TaskCurrentSpinlockOccupyLevel].SpinlockOrder) || (Os_SpinlockCfg[pTCB->TaskCurrentSpinlockOccupyLevel].SpinlockGroupsId != pSpinlockCfg->SpinlockGroupsId)))
    /* PRQA S 1881, 4342, 3442, 3345 -- */
    {
        err = E_OS_NESTING_DEADLOCK;
    }
    /* PRQA S 1881, 4342, 2004 ++ */ /* VL_Os_AutosarBool, VL_Os_4342,VL_Os_2004 */
    else if ((OS_LEVEL_ISR2 == pScb->SysOsLevel) && (OS_SPINLOCK_INVALID != pICB->IsrCurrentSpinlockOccupyLevel) && ((pSpinlockCfg->SpinlockOrder <= Os_SpinlockCfg[pICB->IsrCurrentSpinlockOccupyLevel].SpinlockOrder) || (Os_SpinlockCfg[pICB->IsrCurrentSpinlockOccupyLevel].SpinlockGroupsId != pSpinlockCfg->SpinlockGroupsId)))
    /* PRQA S 1881, 4342, 2004 -- */
    {
        /*Os_00691*/
        err = E_OS_NESTING_DEADLOCK;
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Release a spin lock check
 */
OS_LOCAL StatusType Os_ReleaseSpinlockCheck(const Os_SCBType *pScb, SpinlockIdType spinlockId)
{
    StatusType err = E_OK;
    Os_TCBType *pTCB = pScb->SysRunningTCB; /* PRQA S 3678 */ /* VL_Os_3678 */
    Os_ICBType *pICB = Os_ICB[pScb->SysRunningIsrCat2Id]; /* PRQA S 3678 */ /* VL_Os_3678 */

    /* PRQA S 1881, 4342, 3442 ++ */ /* VL_Os_AutosarBool, VL_Os_4342, VL_Os_3442 */
    if ((OS_LEVEL_TASK == pScb->SysOsLevel) &&
        (((Os_SLCB[spinlockId].ObjOccupyId != pScb->SysRunningTaskId) &&
          (Os_SLCB[spinlockId].ObjOccupyType == OS_OBJECT_TASK)) ||
         (OS_SPINLOCK_INVALID == pTCB->TaskCurrentSpinlockOccupyLevel)))
    /* PRQA S 1881, 4342, 3442 -- */
    {
        err = E_OS_STATE;
    }
    /*Os_00701*/
    /*res and spinlock should together release as LIFO order*/
    else if ((OS_LEVEL_TASK == pScb->SysOsLevel) &&
             ((pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount - 1u] != OS_OBJECT_SPINLOCK) ||
              (pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount - 1u] != spinlockId))) /* PRQA S 1881 */ /* VL_Os_AutosarBool */
    {
        err = E_OS_NOFUNC;
    }

    /*Os_00699*/
    /* PRQA S 1881, 4342 ++ */ /* VL_Os_AutosarBool, VL_Os_4342 */
    else if ((OS_LEVEL_ISR2 == pScb->SysOsLevel) &&
             (((Os_SLCB[spinlockId].ObjOccupyId != pScb->SysRunningIsrCat2Id) &&
               (Os_SLCB[spinlockId].ObjOccupyType == OS_OBJECT_ISR)) ||
              (OS_SPINLOCK_INVALID == pICB->IsrCurrentSpinlockOccupyLevel)))
    /* PRQA S 1881, 4342 -- */
    {
        err = E_OS_STATE;
    }
    /*res and spinlock should together release as LIFO order*/
    else if ((OS_LEVEL_ISR2 == pScb->SysOsLevel) && /* PRQA S 2004 */ /* VL_Os_2004 */
             ((pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount - 1u] != OS_OBJECT_SPINLOCK) ||
              (pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount - 1u] != spinlockId))) /* PRQA S 1881 */ /* VL_Os_AutosarBool */
    {
        err = E_OS_NOFUNC;
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * spin lock locking with method
 */
OS_LOCAL void Os_MethodLock(Os_SCBType *pScb, SpinlockIdType spinlockId)
{
    OS_HAL_DECLARE_CRITICAL();

    switch (Os_SpinlockCfg[spinlockId].SpinlockMethod)
    {
    case LOCK_ALL_INTERRUPTS:
        Os_SuspendAllInterrupts(pScb);
        break;

    case LOCK_CAT2_INTERRUPTS:
        Os_SuspendOSInterrupts(pScb);
        break;

    case LOCK_WITH_RES_SCHEDULER:
#if (TRUE == CFG_USERESSCHEDULER)
        (void)Os_GetResource(pScb, pScb->ScheduleResId);
#endif
        break;

    case LOCK_NOTHING:
        /* Nothing to do. */
        break;

    default:
        Os_Panic();
        break;
    }

    OS_HAL_ENTRY_CRITICAL();
    pScb->CurrentSpinlockOccupied[spinlockId] = TRUE;
    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * modify Tcb or Icb after get spin lock successful
 */
OS_LOCAL void Os_MethodRelease(Os_SCBType *pScb, SpinlockIdType spinlockId)
{
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    pScb->CurrentSpinlockOccupied[spinlockId] = FALSE;
    OS_HAL_EXIT_CRITICAL();

    /*Os_00696*/
    switch (Os_SpinlockCfg[spinlockId].SpinlockMethod)
    {
    case LOCK_ALL_INTERRUPTS:
        Os_ResumeAllInterrupts(pScb);
        break;

    case LOCK_CAT2_INTERRUPTS:
        Os_ResumeOSInterrupts(pScb);
        break;

    case LOCK_WITH_RES_SCHEDULER:
#if (TRUE == CFG_USERESSCHEDULER)
        (void)Os_ReleaseResource(pScb, pScb->ScheduleResId);
#endif
        break;

    case LOCK_NOTHING:
        /* Nothing to do. */
        break;

    default:
        Os_Panic();
        break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * modify Tcb or Icb after get spin lock successful
 */
OS_LOCAL void Os_GetCBModify(const Os_SCBType *pScb, SpinlockIdType spinlockId)
{
    OS_HAL_DECLARE_CRITICAL();
    Os_SLCBType *pSLcb = &Os_SLCB[spinlockId];
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        Os_TCBType *pTCB = pScb->SysRunningTCB; /* PRQA S 3432 */ /* VL_Os_3432 */
        pSLcb->ObjOccupyId = pScb->SysRunningTaskId; /* PRQA S 4424 */ /* VL_Os_4424 */
        pSLcb->ObjOccupyType = OS_OBJECT_TASK;

        /*res and spinlock should together release as LIFO order*/
        OS_HAL_ENTRY_CRITICAL();
        pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount] = OS_OBJECT_SPINLOCK;
        pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount] = spinlockId; /* PRQA S 4424 */ /* VL_Os_4424 */
        pTCB->TaskCurrentSpinlockOccupyLevel = spinlockId; /* PRQA S 4424 */ /* VL_Os_4424 */
        pTCB->TaskCriticalZoneCount++;
        OS_HAL_EXIT_CRITICAL();
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        Os_ICBType *pICB = Os_ICB[pScb->SysRunningIsrCat2Id]; /* PRQA S 3432 */ /* VL_Os_3432 */
        pSLcb->ObjOccupyId = pScb->SysRunningIsrCat2Id; /* PRQA S 4424 */ /* VL_Os_4424 */
        pSLcb->ObjOccupyType = OS_OBJECT_ISR;

        OS_HAL_ENTRY_CRITICAL();
        /*res and spinlock should together release as LIFO order*/
        pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount] = OS_OBJECT_SPINLOCK;
        pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount] = spinlockId; /* PRQA S 4424 */ /* VL_Os_4424 */
        pICB->IsrCurrentSpinlockOccupyLevel = spinlockId; /* PRQA S 4424 */ /* VL_Os_4424 */
        pICB->Isr2CriticalZoneCount++;
        OS_HAL_EXIT_CRITICAL();
    }
    else
    {
        /* Intentionally Empty */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * modify Tcb before release spin lock successful
 * Service ID           <None
 */
OS_LOCAL void Os_ReleaseTCBModify(Os_TCBType *pTCB)
{
    OS_HAL_DECLARE_CRITICAL();

    /*res and spinlock should together release as LIFO order*/
    OS_HAL_ENTRY_CRITICAL();
    pTCB->TaskCriticalZoneCount--;
    pTCB->TaskCriticalZoneType[pTCB->TaskCriticalZoneCount] = OS_OBJECT_MAX;
    pTCB->TaskCriticalZoneStack[pTCB->TaskCriticalZoneCount] = OS_OBJECT_INVALID; /* PRQA S 4424 */ /* VL_Os_4424 */
    OS_HAL_EXIT_CRITICAL();

    uint32 spinlockTempCount = pTCB->TaskCriticalZoneCount;
    pTCB->TaskCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID; /* PRQA S 4424, 4342 */ /* VL_Os_4424, VL_Os_4342 */
    if (spinlockTempCount > 0u)
    {
        do
        {
            --spinlockTempCount;
            if (pTCB->TaskCriticalZoneType[spinlockTempCount] == OS_OBJECT_SPINLOCK)
            {
                SpinlockIdType spinlockId = pTCB->TaskCriticalZoneStack[spinlockTempCount]; /* PRQA S 4442 */ /* VL_Os_4442 */
                if (Os_SLCB[spinlockId].Occupied != FALSE)
                {
                    pTCB->TaskCurrentSpinlockOccupyLevel = spinlockId; /* PRQA S 4424 */ /* VL_Os_4424 */
                    break;
                }
            }
        } while (spinlockTempCount > 0u);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * modify Icb before release spin lock successful
 * Service ID           <None
 */
OS_LOCAL void Os_ReleaseICBModify(Os_ICBType *pICB)
{
    OS_HAL_DECLARE_CRITICAL();

    /*res and spinlock should together release as LIFO order*/
    OS_HAL_ENTRY_CRITICAL();
    pICB->Isr2CriticalZoneCount--;
    pICB->Isr2CriticalZoneType[pICB->Isr2CriticalZoneCount] = OS_OBJECT_MAX;
    pICB->Isr2CriticalZoneStack[pICB->Isr2CriticalZoneCount] = OS_OBJECT_INVALID; /* PRQA S 4424 */ /* VL_Os_4424 */
    OS_HAL_EXIT_CRITICAL();

    uint32 spinlockTempCount = pICB->Isr2CriticalZoneCount;
    pICB->IsrCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID; /* PRQA S 4424, 4342 */ /* VL_Os_4424, VL_Os_4342 */
    if (spinlockTempCount > 0u)
    {
        do
        {
            --spinlockTempCount;
            if (pICB->Isr2CriticalZoneType[spinlockTempCount] == OS_OBJECT_SPINLOCK)
            {
                SpinlockIdType spinlockId = pICB->Isr2CriticalZoneStack[spinlockTempCount]; /* PRQA S 4442 */ /* VL_Os_4442 */
                if (Os_SLCB[spinlockId].Occupied != FALSE)
                {
                    pICB->IsrCurrentSpinlockOccupyLevel = spinlockId; /* PRQA S 4424 */ /* VL_Os_4424 */
                    break;
                }
            }
        } while (spinlockTempCount > 0u);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * modify Tcb or Icb before release spin lock successful
 */
OS_LOCAL void Os_ReleaseCBModify(const Os_SCBType *pScb)
{
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        Os_ReleaseTCBModify(pScb->SysRunningTCB);
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        Os_ICBType *pICB = Os_ICB[pScb->SysRunningIsrCat2Id];
        Os_ReleaseICBModify(pICB);
    }
    else
    {
        Os_Panic();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:GetSpinlock
 */
OS_LOCAL void Os_GetSpinlock(Os_SCBType *pScb, SpinlockIdType spinlockId)
{
    uint32 result;

    Os_MethodLock(pScb, spinlockId);

    do
    {
        result = Os_Hal_CmpSwapW(&Os_Spinlock[spinlockId],
                             OS_SPINLOCK_UNLOCK, OS_SPINLOCK_LOCK);
    } while (result > 0u); /*Os_00687*/
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_SPINLOCK, Os, pScb->SysCore, OsSpinlock_Locked, spinlockId);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */

    Os_SLCB[spinlockId].Occupied = TRUE;

    Os_GetCBModify(pScb, spinlockId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The get spin lock function inside the OS
 */
/* PRQA S 1503, 3006, 3408, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType GetSpinlock(SpinlockIdType SpinlockId)
/* PRQA S 1503, 3006, 3408, 6070, 1512 -- */
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
    OSRtiEnterApi(pScb, OSApiId_GetSpinlock);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetSpinlock_Start, SpinlockId);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

/*Os_00689*/
#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (SpinlockId >= CFG_SPINLOCK_MAX) /* PRQA S 1880 */ /* VL_Os_1880 */
    {
        err = E_OS_ID;
    }
    else
#endif
    {
/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_GET_SPINLOCK,
            .ObjectType = OS_OBJECT_SPINLOCK,
            .ObjectID = SpinlockId, /* PRQA S 4424 */ /* VL_Os_4424 */
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
            if (E_OK != (err = Os_GetSpinlockCheck(pScb, SpinlockId))) /* PRQA S 3326 */ /* VL_Os_3326 */
            {
            }
            else
#endif
            {
                Os_GetSpinlock(pScb, SpinlockId);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_GetSpinlock(SpinlockId), OSServiceId_GetSpinlock, err, pScb); /* PRQA S 3138, 4424, 2880 */ /* VL_Os_PlatformDef, VL_Os_4424, VL_Os_2880 */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_GetSpinlock);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetSpinlock_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    /*Os_00688*/
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * ReleaseSpinlock releases a spinlock variable that was Occupied before.
 *                       Before terminating a TASK all spinlock variables that have been Occupied with
 *                       GetSpinlock() shall be released. Before calling WaitEVENT all Spinlocks
 *                       shall be released
 */
/* PRQA S 1503, 3006, 3408, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType ReleaseSpinlock(SpinlockIdType SpinlockId)
/* PRQA S 1503, 3006, 3408, 6070, 1512 -- */
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
    OSRtiEnterApi(pScb, OSApiId_ReleaseSpinlock);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReleaseSpinlock_Start, SpinlockId);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (SpinlockId >= CFG_SPINLOCK_MAX) /* PRQA S 1880 */ /* VL_Os_1880 */
    {
        err = E_OS_ID;
    }
    else
#endif
    {
/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_RELEASE_SPINLOCK,
            .ObjectType = OS_OBJECT_SPINLOCK,
            .ObjectID = SpinlockId, /* PRQA S 4424 */ /* VL_Os_4424 */
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
            if (E_OK != (err = Os_ReleaseSpinlockCheck(pScb, SpinlockId))) /* PRQA S 3326 */ /* VL_Os_3326 */
            {
            }
            else
#endif
            {
                Os_ReleaseSpinlock(pScb, SpinlockId);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 4424 ++ */ /* VL_Os_4424 */
        Os_TraceErrorHook(OSError_Save_ReleaseSpinlock(SpinlockId), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_ReleaseSpinlock, err, pScb);
        /* PRQA S 4424 -- */
        /* PRQA S 3138, 3141 -- */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ReleaseSpinlock);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ReleaseSpinlock_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    /*Os_00697*/
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:ReleaseSpinlock
 */
void Os_ReleaseSpinlock(Os_SCBType *pScb, SpinlockIdType spinlockId)
{
    Os_SLCB[spinlockId].ObjOccupyId = OS_TASK_INVALID; /* PRQA S 4424, 4342 */ /* VL_Os_4424, VL_Os_4342 */
    Os_SLCB[spinlockId].ObjOccupyType = OS_OBJECT_MAX;
    Os_SLCB[spinlockId].Occupied = FALSE;

    Os_ReleaseCBModify(pScb);

    /*Os_00696*/
    uint32 result;
    do
    {
        result = Os_Hal_CmpSwapW(&Os_Spinlock[spinlockId], OS_SPINLOCK_LOCK, OS_SPINLOCK_UNLOCK);
    } while (result > 0u); /*Os_00687*/
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_SPINLOCK, Os, pScb->SysCore, OsSpinlock_Released, spinlockId);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */

    Os_MethodRelease(pScb, spinlockId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:TryToGetSpinlock
 */
OS_LOCAL void Os_TryToGetSpinlock(
    Os_SCBType *pScb,
    SpinlockIdType spinlockId,
    TryToGetSpinlockType *success)
{
    Os_MethodLock(pScb, spinlockId);

    /*Os_00705*/
    uint32 result = Os_Hal_CmpSwapW(&Os_Spinlock[spinlockId], OS_SPINLOCK_UNLOCK, OS_SPINLOCK_LOCK);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_SPINLOCK, Os, pScb->SysCore, OsSpinlock_Locked, spinlockId);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */
    /*Os_00706*/
    if (OS_SPINLOCK_OCCUPY_SUCCESS == result)
    {
        *success = TRYTOGETSPINLOCK_SUCCESS;
        Os_SLCB[spinlockId].Occupied = TRUE;
        Os_GetCBModify(pScb, spinlockId);
    }
    else
    {
        *success = TRYTOGETSPINLOCK_NOSUCCESS;
        Os_MethodRelease(pScb, spinlockId);
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * TryToGetSpinlock has the same functionality as GetSpinlock with
 *                       the difference that if the spinlock is already Occupied by
 *                       a TASK on a different core the function sets the OUT parameter
 *                       "Success" and returns with E_OK
 */
/* PRQA S 1503, 3006, 3408, 6070, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
 StatusType TryToGetSpinlock(
    SpinlockIdType SpinlockId,
    TryToGetSpinlockType *Success)
/* PRQA S 1503, 3006, 3408, 6070, 1512 -- */
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
    OSRtiEnterApi(pScb, OSApiId_TryToGetSpinlock);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_TryToGetSpinlock_Start, SpinlockId);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /*Os_00689*/
    if (CFG_SPINLOCK_MAX <= SpinlockId) /* PRQA S 1880 */ /* VL_Os_1880 */
    {
        err = E_OS_ID;
    }
    else if (NULL_PTR == Success)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
    {
/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_TRY_TO_GET_SPINLOCK,
            .ObjectType = OS_OBJECT_SPINLOCK,
            .ObjectID = SpinlockId, /* PRQA S 4424 */ /* VL_Os_4424 */
            .Address = (uint32)Success, /* PRQA S 0306 */ /* VL_Os_0306 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
            if (E_OK != (err = Os_GetSpinlockCheck(pScb, SpinlockId))) /* PRQA S 3326 */ /* VL_Os_3326 */
            {
            }
            else
#endif
            {
                Os_TryToGetSpinlock(pScb, SpinlockId, Success);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 4424 ++ */ /* VL_Os_4424 */
        Os_TraceErrorHook(OSError_Save_TryToGetSpinlock(SpinlockId, Success), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_TryToGetSpinlock,
                          err, pScb);
        /* PRQA S 4424 -- */
        /* PRQA S 3138, 3141 -- */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_TryToGetSpinlock);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_TryToGetSpinlock_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    /*Os_00704*/
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * TCheck if there are any outstanding spinlocks
 *                         when scheduling is required
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
StatusType Os_SpinlockSafetyCheck(Os_TaskType runningTaskId)
/* PRQA S 1503 -- */
{
    StatusType status = E_OK;

    /*Os_00612*/
    for (uint16 i = 0u; i < CFG_SPINLOCK_MAX; i++)
    {
        if (OS_OBJECT_TASK == Os_SLCB[i].ObjOccupyType)
        {
            if (Os_SLCB[i].ObjOccupyId == runningTaskId) /* PRQA S 1881 */ /* VL_Os_1881 */
            {
                status = E_OS_SPINLOCK;
                break;
            }
        }
    }

    return status;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The get spin lock function inside the OS
 */
/* PRQA S 1503, 3408 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408 */
void Os_GetInternalSpinlock(Os_SpinlockRefType spinlock)
/* PRQA S 1503, 3408 -- */
{
    uint32 result;

    /*Os_00687*/
    do
    {
        result = Os_Hal_CmpSwapW(spinlock, OS_SPINLOCK_UNLOCK, OS_SPINLOCK_LOCK);
    } while (result > 0u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The release spin lock function inside the OS
 */
/* PRQA S 1503, 3408 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408 */
void Os_ReleaseInternalSpinlock(Os_SpinlockRefType spinlock)
/* PRQA S 1503, 3408 -- */
{
    uint32 result;

    do
    {
        result = Os_Hal_CmpSwapW(spinlock, OS_SPINLOCK_LOCK, OS_SPINLOCK_UNLOCK);
    } while (result > 0u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
