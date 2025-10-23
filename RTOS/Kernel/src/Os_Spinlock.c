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
 **  FILENAME    :  Os_Spinlock.c                                              **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION :  Spinlock manager                                           **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/

/*=======[V E R S I O N  C H E C K]===========================================*/

/*=======[M A C R O S]========================================================*/
#define OS_SPINLOCK_UNLOCK            0x0u
#define OS_SPINLOCK_LOCK              0x1u
#define OS_SPINLOCK_OCCUPY_SUCCESS    0x0u
#define OS_SPINLOCK_OCCUPY_NO_SUCCESS 0x1u

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (CFG_SPINLOCK_MAX > 0U)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the spin lock>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitSpinlock(void)
/* PRQA S 1532 -- */
{
    Os_SpinlockIdType i;

    for (i = 0u; i < CFG_SPINLOCK_MAX; i++)
    {
        Os_SLCB[i].objOccupyType = OBJECT_MAX;
        Os_SLCB[i].objOccupyId   = OS_OBJECT_INVALID;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:GetSpinlock>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <SpinlockId>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_GetSpinlock(SpinlockIdType SpinlockId)
/* PRQA S 3450 -- */
{
    OS_ARCH_DECLARE_CRITICAL();

    uint32 result;
    /* PRQA S 4544, 3120 ++ */ /*  VL_Os_4544, VL_QAC_MagicNum */
    Os_CoreIdType coreIndex = Os_SCB.sysCore << 12;
    /* PRQA S 4544, 3120 -- */
    const Os_SpinlockCfgType* pSpinlockCfg = &Os_SpinlockCfg[SpinlockId];

    switch (pSpinlockCfg->SpinlockMethod)
    {
    case LOCK_ALL_INTERRUPTS:
        Os_SuspendAllInterrupts();
        break;

    case LOCK_CAT2_INTERRUPTS:
        Os_SuspendOSInterrupts();
        break;

    case LOCK_WITH_RES_SCHEDULER:
#if (TRUE == CFG_USERESSCHEDULER)
        (void)Os_GetResource(RES_SCHEDULER);
#endif
        break;

    case LOCK_NOTHING:
        /* Nothing to do. */
        break;

    default:
        Os_Panic();
        break;
    }

    OS_ARCH_ENTRY_CRITICAL();
    Os_SCB.CurrentSpinlockOccupied[SpinlockId] = TRUE;
    OS_ARCH_EXIT_CRITICAL();

    do
    {
        result = Os_CmpSwapW(&Os_Spinlock[SpinlockId], OS_SPINLOCK_UNLOCK, OS_SPINLOCK_LOCK);
    } while (result > 0u); /*Os_00687*/

    Os_SLCB[SpinlockId].occupied = TRUE;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        Os_TCBType* pTCB = Os_SCB.sysRunningTCB;

        Os_SLCB[SpinlockId].objOccupyId   = (coreIndex | Os_SCB.sysRunningTaskID);
        Os_SLCB[SpinlockId].objOccupyType = OBJECT_TASK;

        /*res and spinlock should together release as LIFO order*/

        OS_ARCH_ENTRY_CRITICAL();

        pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount]  = OBJECT_SPINLOCK;
        pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount] = SpinlockId;
        pTCB->taskCurrentSpinlockOccupyLevel                     = SpinlockId;
        pTCB->taskCriticalZoneCount++;

        OS_ARCH_EXIT_CRITICAL();
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        Os_ICBType* pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];

        Os_SLCB[SpinlockId].objOccupyId   = (coreIndex | Os_SCB.sysRunningIsrCat2Id);
        Os_SLCB[SpinlockId].objOccupyType = OBJECT_ISR;

        OS_ARCH_ENTRY_CRITICAL();

        /*res and spinlock should together release as LIFO order*/
        pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount]  = OBJECT_SPINLOCK;
        pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount] = SpinlockId;
        pICB->isrCurrentSpinlockOccupyLevel                      = SpinlockId;
        pICB->isr2CriticalZoneCount++;

        OS_ARCH_EXIT_CRITICAL();
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
/******************************************************************************/
/*
 * Brief                <The get spin lock function inside the OS>
 * Service ID           <0x19>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <SpinlockId>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 3006 ++ */ /* VL_MTR_Os_STMIF, VL_Os_3006 */
StatusType GetSpinlock(SpinlockIdType SpinlockId)
/* PRQA S 6030, 3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741 -- */

    StatusType err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (SpinlockId >= CFG_SPINLOCK_MAX)
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

    /*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_SPINLOCK) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_CheckObjAcs(OBJECT_SPINLOCK, SpinlockId) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        const Os_SpinlockCfgType* pSpinlockCfg = &Os_SpinlockCfg[SpinlockId];

        /* PRQA S 3678 ++ */ /* VL_Os_3678 */
        Os_TCBType* pTCB = Os_SCB.sysRunningTCB;
        Os_ICBType* pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
        /* PRQA S 3678 -- */
        /*Os_00690*/
        if (Os_SCB.CurrentSpinlockOccupied[SpinlockId] == TRUE)
        {
            err = E_OS_INTERFERENCE_DEADLOCK;
        }
        else if (
            /* PRQA S 3345, 3442 ++ */ /* VL_Os_3442, VL_Os_VolatileAccess */
            (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
            && (OS_SPINLOCK_INVALID != pTCB->taskCurrentSpinlockOccupyLevel)
            /* PRQA S 3345, 3442 -- */
            && ((pSpinlockCfg->SpinlockOrder <= Os_SpinlockCfg[pTCB->taskCurrentSpinlockOccupyLevel].SpinlockOrder)
                || (Os_SpinlockCfg[pTCB->taskCurrentSpinlockOccupyLevel].SpinlockGroupsId
                    != pSpinlockCfg->SpinlockGroupsId)))
        {
            err = E_OS_NESTING_DEADLOCK;
        }
        else if (
            (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel) && (OS_SPINLOCK_INVALID != pICB->isrCurrentSpinlockOccupyLevel)
            && ((pSpinlockCfg->SpinlockOrder <= Os_SpinlockCfg[pICB->isrCurrentSpinlockOccupyLevel].SpinlockOrder)
                || (Os_SpinlockCfg[pICB->isrCurrentSpinlockOccupyLevel].SpinlockGroupsId
                    != pSpinlockCfg->SpinlockGroupsId)))
        {
            /*Os_00691*/
            err = E_OS_NESTING_DEADLOCK;
        }
        else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            Os_GetSpinlock(SpinlockId);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetSpinlock(SpinlockId), OSServiceId_GetSpinlock, err);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    /*Os_00688*/
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <ReleaseSpinlock releases a spinlock variable that was occupied before.
 *                       Before terminating a TASK all spinlock variables that have been occupied with
 *                       GetSpinlock() shall be released. Before calling WaitEVENT all Spinlocks
 *                       shall be released>
 * Service ID           <0x1a>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <SpinlockId>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 6010, 3006 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_Os_3006 */
StatusType ReleaseSpinlock(SpinlockIdType SpinlockId)
/* PRQA S 6030, 6010, 3006 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741 -- */

    StatusType    err    = E_OK;
    Os_CoreIdType coreId = Os_SCB.sysCore;
    /* PRQA S 4544, 3120 ++ */ /*  VL_Os_4544, VL_QAC_MagicNum */
    Os_CoreIdType coreIndex = coreId << 12;
    /* PRQA S 4544, 3120 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (SpinlockId >= CFG_SPINLOCK_MAX)
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_RELEASE_SPINLOCK) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (FALSE == Os_CheckObjAcs(OBJECT_SPINLOCK, SpinlockId))
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        /* PRQA S 3678 ++ */ /* VL_Os_3678 */
        Os_TCBType* pTCB = Os_SCB.sysRunningTCB;
        Os_ICBType* pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
        /* PRQA S 3678 -- */
        /*Os_00699*/
        if ((OS_LEVEL_TASK == Os_SCB.sysOsLevel) /* PRQA S 3442 */ /* VL_Os_3442 */
            && (((Os_SLCB[SpinlockId].objOccupyId != (coreIndex | Os_SCB.sysRunningTaskID))
                 && (Os_SLCB[SpinlockId].objOccupyType == OBJECT_TASK))
                || (OS_SPINLOCK_INVALID == pTCB->taskCurrentSpinlockOccupyLevel)))
        {
            err = E_OS_STATE;
        }
        /*Os_00701*/
        /*res and spinlock should together release as LIFO order*/
        else if (
            (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
            && ((pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount - 1u] != OBJECT_SPINLOCK)
                || (pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount - 1u] != SpinlockId)))
        {
            err = E_OS_NOFUNC;
        }
        /*Os_00699*/
        else if (
            (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
            && (((Os_SLCB[SpinlockId].objOccupyId != (coreIndex | Os_SCB.sysRunningIsrCat2Id))
                 && (Os_SLCB[SpinlockId].objOccupyType == OBJECT_ISR))
                || (OS_SPINLOCK_INVALID == pICB->isrCurrentSpinlockOccupyLevel)))
        {
            err = E_OS_STATE;
        }
        /*res and spinlock should together release as LIFO order*/
        else if (
            (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
            && ((pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount - 1u] != OBJECT_SPINLOCK)
                || (pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount - 1u] != SpinlockId)))
        {
            err = E_OS_NOFUNC;
        }
        else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            Os_ReleaseSpinlock(SpinlockId);
        }
    }
#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ReleaseSpinlock(SpinlockId), OSServiceId_ReleaseSpinlock, err);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    /*Os_00697*/
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ReleaseSpinlock>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <SpinlockId>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 6010, 6080, 6050 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_MTR_Os_STPTH, VL_MTR_Os_STST3 */
void Os_ReleaseSpinlock(SpinlockIdType SpinlockId)
/* PRQA S 6030, 6010, 6080, 6050 -- */
{
    OS_ARCH_DECLARE_CRITICAL();

    Os_TCBType* pTCB = Os_SCB.sysRunningTCB;
    Os_ICBType* pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];

    Os_SpinlockIdType preSpinlockId;
    uint32            spinlockTempCount;
    uint32            result;

    preSpinlockId                     = SpinlockId;
    Os_SLCB[SpinlockId].objOccupyId   = OS_TASK_INVALID;
    Os_SLCB[SpinlockId].objOccupyType = OBJECT_MAX;
    Os_SLCB[SpinlockId].occupied      = FALSE;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        /*res and spinlock should together release as LIFO order*/

        OS_ARCH_ENTRY_CRITICAL();

        pTCB->taskCriticalZoneCount--;
        pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount]  = OBJECT_MAX;
        pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount] = OS_OBJECT_INVALID;

        OS_ARCH_EXIT_CRITICAL();

        spinlockTempCount = pTCB->taskCriticalZoneCount;
        if (0u != spinlockTempCount)
        {
            while (TRUE) /* PRQA S 2740, 0771 */ /* VL_Os_2740, VL_Os_0771 */
            {
                spinlockTempCount--;
                if (pTCB->taskCriticalZoneType[spinlockTempCount] == OBJECT_SPINLOCK)
                {
                    /* PRQA S 1338 ++ */ /* VL_Os_1338 */
                    SpinlockId = pTCB->taskCriticalZoneStack[spinlockTempCount];
                    /* PRQA S 1338 -- */
                    if ((boolean)TRUE == Os_SLCB[SpinlockId].occupied)
                    {
                        pTCB->taskCurrentSpinlockOccupyLevel = SpinlockId;
                        break;
                    }
                }

                if (0u == spinlockTempCount)
                {
                    pTCB->taskCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID;
                    break;
                }
            }
        }
        else
        {
            pTCB->taskCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID;
        }
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        /*res and spinlock should together release as LIFO order*/

        OS_ARCH_ENTRY_CRITICAL();

        pICB->isr2CriticalZoneCount--;
        pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount]  = OBJECT_MAX;
        pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount] = OS_OBJECT_INVALID;

        OS_ARCH_EXIT_CRITICAL();

        spinlockTempCount = pICB->isr2CriticalZoneCount;
        if (0u != spinlockTempCount)
        {
            /* PRQA S 2990, 0771 ++ */ /* VL_Os_2990, VL_Os_0771 */
            while (spinlockTempCount > 0u)
            /* PRQA S 2990, 0771 -- */
            {
                spinlockTempCount--;
                if (pICB->isr2CriticalZoneType[spinlockTempCount] == OBJECT_SPINLOCK)
                {
                    /* PRQA S 1338 ++ */ /* VL_Os_1338 */
                    SpinlockId = pICB->isr2CriticalZoneStack[spinlockTempCount];
                    /* PRQA S 1338 -- */
                    if ((boolean)TRUE == Os_SLCB[SpinlockId].occupied)
                    {
                        pICB->isrCurrentSpinlockOccupyLevel = SpinlockId;
                        break;
                    }
                }

                if (0u == spinlockTempCount)
                {
                    pICB->isrCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID;
                    break;
                }
            }
        }
        else
        {
            pICB->isrCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID;
        }
    }
    else
    {
        Os_Panic();
    }

    /*Os_00696*/
    do
    {
        result = Os_CmpSwapW(&Os_Spinlock[preSpinlockId], OS_SPINLOCK_LOCK, OS_SPINLOCK_UNLOCK);
    } while (result > 0u); /*Os_00687*/

    OS_ARCH_ENTRY_CRITICAL();

    Os_SCB.CurrentSpinlockOccupied[preSpinlockId] = FALSE;

    OS_ARCH_EXIT_CRITICAL();

    /*Os_00696*/
    switch (Os_SpinlockCfg[preSpinlockId].SpinlockMethod)
    {
    case LOCK_ALL_INTERRUPTS:
        Os_ResumeAllInterrupts();
        break;

    case LOCK_CAT2_INTERRUPTS:
        Os_ResumeOSInterrupts();
        break;

    case LOCK_WITH_RES_SCHEDULER:
#if (TRUE == CFG_USERESSCHEDULER)
        (void)Os_ReleaseResource(RES_SCHEDULER);
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
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:TryToGetSpinlock>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <SpinlockId>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6010, 6070, 3450 ++ */ /* VL_MTR_Os_STCYC, VL_MTR_Os_STCAL, VL_Os_3450 */
static void Os_TryToGetSpinlock(SpinlockIdType SpinlockId, TryToGetSpinlockType* Success)
/* PRQA S 6010, 6070, 3450 -- */
{
    OS_ARCH_DECLARE_CRITICAL();

    Os_TCBType* pTCB = Os_SCB.sysRunningTCB;
    Os_ICBType* pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];

    uint32 result;
    /* PRQA S 4544, 3120 ++ */ /*  VL_Os_4544, VL_QAC_MagicNum */
    Os_CoreIdType coreIndex = Os_SCB.sysCore << 12;
    /* PRQA S 4544, 3120 -- */

    switch (Os_SpinlockCfg[SpinlockId].SpinlockMethod)
    {
    case LOCK_ALL_INTERRUPTS:
        Os_SuspendAllInterrupts();
        break;

    case LOCK_CAT2_INTERRUPTS:
        Os_SuspendOSInterrupts();
        break;

    case LOCK_WITH_RES_SCHEDULER:
#if (TRUE == CFG_USERESSCHEDULER)
        (void)Os_GetResource(RES_SCHEDULER);
#endif
        break;

    case LOCK_NOTHING:
        /* Nothing to do. */
        break;

    /*add to pass QAC*/
    default:
        /* Nothing to do. */
        break;
    }

    OS_ARCH_ENTRY_CRITICAL();

    Os_SCB.CurrentSpinlockOccupied[SpinlockId] = TRUE;

    OS_ARCH_EXIT_CRITICAL();

    /*Os_00705*/
    result = Os_CmpSwapW(&Os_Spinlock[SpinlockId], OS_SPINLOCK_UNLOCK, OS_SPINLOCK_LOCK);
    /*Os_00706*/
    if (OS_SPINLOCK_OCCUPY_SUCCESS == result)
    {
        *Success                     = TRYTOGETSPINLOCK_SUCCESS;
        Os_SLCB[SpinlockId].occupied = TRUE;
        if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        {
            Os_SLCB[SpinlockId].objOccupyId   = (coreIndex | Os_SCB.sysRunningTaskID);
            Os_SLCB[SpinlockId].objOccupyType = OBJECT_TASK;

            /*res and spinlock should together release as LIFO order*/

            OS_ARCH_ENTRY_CRITICAL();

            pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount]  = OBJECT_SPINLOCK;
            pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount] = SpinlockId;
            pTCB->taskCurrentSpinlockOccupyLevel                     = SpinlockId;
            pTCB->taskCriticalZoneCount++;

            OS_ARCH_EXIT_CRITICAL();
        }
        else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
        {
            Os_SLCB[SpinlockId].objOccupyId   = (coreIndex | Os_SCB.sysRunningIsrCat2Id);
            Os_SLCB[SpinlockId].objOccupyType = OBJECT_ISR;

            OS_ARCH_ENTRY_CRITICAL();

            /*res and spinlock should together release as LIFO order*/
            pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount]  = OBJECT_SPINLOCK;
            pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount] = SpinlockId;
            pICB->isrCurrentSpinlockOccupyLevel                      = SpinlockId;
            pICB->isr2CriticalZoneCount++;

            OS_ARCH_EXIT_CRITICAL();
        }
        else
        {
            /* Intentionally Empty */
        }
    }
    else
    {
        *Success = TRYTOGETSPINLOCK_NOSUCCESS;

        OS_ARCH_ENTRY_CRITICAL();

        Os_SCB.CurrentSpinlockOccupied[SpinlockId] = FALSE;

        OS_ARCH_EXIT_CRITICAL();

        switch (Os_SpinlockCfg[SpinlockId].SpinlockMethod)
        {
        case LOCK_ALL_INTERRUPTS:
            Os_ResumeAllInterrupts();
            break;

        case LOCK_CAT2_INTERRUPTS:
            Os_ResumeOSInterrupts();
            break;

        case LOCK_WITH_RES_SCHEDULER:
#if (TRUE == CFG_USERESSCHEDULER)
            (void)Os_ReleaseResource(RES_SCHEDULER);
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
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <TryToGetSpinlock has the same functionality as GetSpinlock with
 *                       the difference that if the spinlock is already occupied by
 *                       a TASK on a different core the function sets the OUT parameter
 *                       "Success" and returns with E_OK>
 * Service ID           <0x1b>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <SpinlockId>
 * param[out]           <Success>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 6030, 6010, 3006, 1503 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC, VL_Os_3006, VL_QAC_NoUsedApi */
StatusType TryToGetSpinlock(SpinlockIdType SpinlockId, TryToGetSpinlockType* Success)
/* PRQA S 6030, 6010, 3006, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741 -- */
    StatusType err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /*Os_00689*/
    if (SpinlockId >= CFG_SPINLOCK_MAX)
    {
        err = E_OS_ID;
    }
    else if (NULL_PTR == Success)
    {
        err = E_OS_PARAM_POINTER;
    }
    else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
/*service protection*/
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_SPINLOCK) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_CheckObjAcs(OBJECT_SPINLOCK, SpinlockId) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    else if (Os_AddressWritable((uint32)Success) != TRUE)
    /* PRQA S 0306 -- */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        const Os_SpinlockCfgType* pSpinlockCfg = &Os_SpinlockCfg[SpinlockId];
        /* PRQA S 3678 ++ */ /* VL_Os_3678 */
        Os_TCBType* pTCB = Os_SCB.sysRunningTCB;
        Os_ICBType* pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
        /* PRQA S 3678 -- */
        /*Os_00690*/
        if (Os_SCB.CurrentSpinlockOccupied[SpinlockId] == TRUE)
        {
            err = E_OS_INTERFERENCE_DEADLOCK;
        }
        else if (
            /* PRQA S 3345, 3442 ++ */ /* VL_Os_3442, VL_Os_VolatileAccess */
            (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
            && (OS_SPINLOCK_INVALID != pTCB->taskCurrentSpinlockOccupyLevel)
            /* PRQA S 3345, 3442 -- */
            && ((pSpinlockCfg->SpinlockOrder <= Os_SpinlockCfg[pTCB->taskCurrentSpinlockOccupyLevel].SpinlockOrder)
                || (Os_SpinlockCfg[pTCB->taskCurrentSpinlockOccupyLevel].SpinlockGroupsId
                    != pSpinlockCfg->SpinlockGroupsId)))
        {
            /*Os_00691*/
            err = E_OS_NESTING_DEADLOCK;
        }
        else if (
            (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel) && (OS_SPINLOCK_INVALID != pICB->isrCurrentSpinlockOccupyLevel)
            && ((pSpinlockCfg->SpinlockOrder <= Os_SpinlockCfg[pICB->isrCurrentSpinlockOccupyLevel].SpinlockOrder)
                || (Os_SpinlockCfg[pICB->isrCurrentSpinlockOccupyLevel].SpinlockGroupsId
                    != pSpinlockCfg->SpinlockGroupsId)))
        {
            /*Os_00691*/
            err = E_OS_NESTING_DEADLOCK;
        }
        else
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
        {
            Os_TryToGetSpinlock(SpinlockId, Success);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_TryToGetSpinlock(SpinlockId, Success), OSServiceId_TryToGetSpinlock, err);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    /*Os_00704*/
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <TCheck if there are any outstanding spinlocks
 *                         when scheduling is required>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <TerminateTask and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
StatusType Os_SpinlockSafetyCheck(void)
{
    StatusType     status = E_OK;
    SpinlockIdType i;
    uint16         obj_id;

    /*Os_00612*/
    for (i = 0u; i < CFG_SPINLOCK_MAX; i++)
    {
        if (OBJECT_TASK == Os_SLCB[i].objOccupyType)
        {
            obj_id = Os_SLCB[i].objOccupyId;

            if ((Os_GetObjLocalId(obj_id) == Os_SCB.sysRunningTaskID) && (Os_SCB.sysCore == Os_GetObjCoreId(obj_id)))

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
#endif /* CFG_SPINLOCK_MAX > 0U */

#if ((OS_AUTOSAR_CORES > 1U) || (CFG_SPINLOCK_MAX > 0U))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The get spin lock function inside the OS>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <spinlock>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <ShutdownAllCores and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_GetInternalSpinlock(Os_SpinlockRefType spinlock)
{
    uint32 result;

    /*Os_00687*/
    do
    {
        result = Os_CmpSwapW(spinlock, OS_SPINLOCK_UNLOCK, OS_SPINLOCK_LOCK);
    } while (result > 0u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The release spin lock function inside the OS>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <spinlock>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <ShutdownAllCores and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_ReleaseInternalSpinlock(Os_SpinlockRefType spinlock)
{
    uint32 result;

    do
    {
        result = Os_CmpSwapW(spinlock, OS_SPINLOCK_LOCK, OS_SPINLOCK_UNLOCK);
    } while (result > 0u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_AUTOSAR_CORES > 1U || CFG_SPINLOCK_MAX > 0U */

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
