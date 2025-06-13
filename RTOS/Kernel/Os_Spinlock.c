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
FUNC(void, OS_CODE) Os_InitSpinlock(void)
{
    VAR(Os_SpinlockIdType, OS_VAR) i;

    for (i = 0u; i < CFG_SPINLOCK_MAX; i++)
    {
        Os_SLCB[i].objOccupyType = OBJECT_MAX;
        Os_SLCB[i].objOccupyId = OS_OBJECT_INVALID;
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
static FUNC(void, OS_CODE) Os_GetSpinlock(SpinlockIdType SpinlockId)
{
    OS_ARCH_DECLARE_CRITICAL();

    VAR(uint32, OS_VAR) result;
    VAR(Os_CoreIdType, OS_VAR) coreIndex = Os_SCB.sysCore << 12;
    P2CONST(Os_SpinlockCfgType, AUTOMATIC, OS_VAR) pSpinlockCfg = &Os_SpinlockCfg[SpinlockId];

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

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    Os_SCB.CurrentSpinlockOccupied[SpinlockId] = TRUE;
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    do
    {
        result = Os_CmpSwapW(&Os_Spinlock[SpinlockId], OS_SPINLOCK_UNLOCK, OS_SPINLOCK_LOCK);
    } while (result > 0u); /*Os_00687*/

    Os_SLCB[SpinlockId].occupied = TRUE;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SPINLOCK_COMPILER_001 */
        P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB = Os_SCB.sysRunningTCB;
        /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
        Os_SLCB[SpinlockId].objOccupyId = (coreIndex | Os_SCB.sysRunningTaskID);
        Os_SLCB[SpinlockId].objOccupyType = OBJECT_TASK;

        /*res and spinlock should together release as LIFO order*/
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount] = OBJECT_SPINLOCK;
        pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount] = SpinlockId;
        pTCB->taskCurrentSpinlockOccupyLevel = SpinlockId;
        pTCB->taskCriticalZoneCount++;
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SPINLOCK_COMPILER_001 */
        P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
        /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
        Os_SLCB[SpinlockId].objOccupyId = (coreIndex | Os_SCB.sysRunningIsrCat2Id);
        Os_SLCB[SpinlockId].objOccupyType = OBJECT_ISR;

        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        /*res and spinlock should together release as LIFO order*/
        pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount] = OBJECT_SPINLOCK;
        pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount] = SpinlockId;
        pICB->isrCurrentSpinlockOccupyLevel = SpinlockId;
        pICB->isr2CriticalZoneCount++;
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
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
FUNC(StatusType, OS_CODE) GetSpinlock(SpinlockIdType SpinlockId)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;

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
        P2CONST(Os_SpinlockCfgType, AUTOMATIC, OS_VAR) pSpinlockCfg = &Os_SpinlockCfg[SpinlockId];
        /* PRQA S 3432,3678 ++ */ /* MISRA Rule 20.7,8.13 */ /* OS_SPINLOCK_COMPILER_001 */
        /*OS_SPINLOCK_LACK_CONST_004*/
        P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB = Os_SCB.sysRunningTCB;
        P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
        /* PRQA S 3432,3678 -- */ /* MISRA Rule 20.7,8.13 */
        /*Os_00690*/
        if (Os_SCB.CurrentSpinlockOccupied[SpinlockId] == TRUE)
        {
            err = E_OS_INTERFERENCE_DEADLOCK;
        }
        else if (
            (OS_LEVEL_TASK == Os_SCB.sysOsLevel) && (OS_SPINLOCK_INVALID != pTCB->taskCurrentSpinlockOccupyLevel)
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

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
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
FUNC(StatusType, OS_CODE) ReleaseSpinlock(SpinlockIdType SpinlockId)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;
    VAR(Os_CoreIdType, OS_VAR) coreIndex = coreId << 12;

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
        /* PRQA S 3432, 3678 ++ */ /* MISRA Rule 20.7, 8.13 */ /* OS_SPINLOCK_COMPILER_001 */
        P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB = Os_SCB.sysRunningTCB;
        P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
        /* PRQA S 3432, 3678 -- */ /* MISRA Rule 20.7, 8.13 */

        /*Os_00699*/
        if ((OS_LEVEL_TASK == Os_SCB.sysOsLevel)
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

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
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
FUNC(void, OS_CODE) Os_ReleaseSpinlock(SpinlockIdType SpinlockId)
{
    OS_ARCH_DECLARE_CRITICAL();
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SPINLOCK_COMPILER_001 */
    P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB = Os_SCB.sysRunningTCB;
    P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(Os_SpinlockIdType, OS_VAR) preSpinlockId;
    VAR(uint32, OS_VAR) spinlockTempCount;
    VAR(uint32, OS_VAR) result;

    preSpinlockId = SpinlockId;
    Os_SLCB[SpinlockId].objOccupyId = OS_TASK_INVALID;
    Os_SLCB[SpinlockId].objOccupyType = OBJECT_MAX;
    Os_SLCB[SpinlockId].occupied = FALSE;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        /*res and spinlock should together release as LIFO order*/
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        pTCB->taskCriticalZoneCount--;
        pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount] = OBJECT_MAX;
        pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount] = OS_OBJECT_INVALID;
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

        spinlockTempCount = pTCB->taskCriticalZoneCount;
        if (0u != spinlockTempCount)
        {
            while (TRUE) /* PRQA S 0771 */ /* MISRA Dir 15.4 */ /* OS_SPINLOCK_MULTI_BREAK_005 */
            {
                spinlockTempCount--;
                if (pTCB->taskCriticalZoneType[spinlockTempCount] == OBJECT_SPINLOCK)
                {
                    /* PRQA S 1338 ++ */ /* MISRA Dir 17.8  */ /*OS_SPINLOCK_PARAMETER_MODIFIED_006*/
                    SpinlockId = pTCB->taskCriticalZoneStack[spinlockTempCount];
                    /* PRQA S 1338 -- */ /* MISRA Dir 17.8  */
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
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        pICB->isr2CriticalZoneCount--;
        pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount] = OBJECT_MAX;
        pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount] = OS_OBJECT_INVALID;
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

        spinlockTempCount = pICB->isr2CriticalZoneCount;
        if (0u != spinlockTempCount)
        {
            /* PRQA S 0771 ++ */ /* MISRA Dir 15.4 */ /* OS_SPINLOCK_MULTI_BREAK_005 */
            while (spinlockTempCount > 0u)
            /* PRQA S 0771 -- */ /* MISRA Dir 15.4 */
            {
                spinlockTempCount--;
                if (pICB->isr2CriticalZoneType[spinlockTempCount] == OBJECT_SPINLOCK)
                {
                    /* PRQA S 1338 ++ */ /* MISRA Dir 17.8  */ /*OS_SPINLOCK_PARAMETER_MODIFIED_006*/
                    SpinlockId = pICB->isr2CriticalZoneStack[spinlockTempCount];
                    /* PRQA S 1338 -- */ /* MISRA Dir 17.8  */
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

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    Os_SCB.CurrentSpinlockOccupied[preSpinlockId] = FALSE;
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

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
static FUNC(void, OS_CODE) Os_TryToGetSpinlock(SpinlockIdType SpinlockId, TryToGetSpinlockType* Success)
{
    OS_ARCH_DECLARE_CRITICAL();
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_SPINLOCK_COMPILER_001 */
    P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB = Os_SCB.sysRunningTCB;
    P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    VAR(uint32, OS_VAR) result;
    VAR(Os_CoreIdType, OS_VAR) coreIndex = Os_SCB.sysCore << 12;

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

    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
    OS_ARCH_ENTRY_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
    Os_SCB.CurrentSpinlockOccupied[SpinlockId] = TRUE;
    /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
    OS_ARCH_EXIT_CRITICAL();
    /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

    /*Os_00705*/
    result = Os_CmpSwapW(&Os_Spinlock[SpinlockId], OS_SPINLOCK_UNLOCK, OS_SPINLOCK_LOCK);
    /*Os_00706*/
    if (OS_SPINLOCK_OCCUPY_SUCCESS == result)
    {
        *Success = TRYTOGETSPINLOCK_SUCCESS;
        Os_SLCB[SpinlockId].occupied = TRUE;
        if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        {
            Os_SLCB[SpinlockId].objOccupyId = (coreIndex | Os_SCB.sysRunningTaskID);
            Os_SLCB[SpinlockId].objOccupyType = OBJECT_TASK;

            /*res and spinlock should together release as LIFO order*/
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            pTCB->taskCriticalZoneType[pTCB->taskCriticalZoneCount] = OBJECT_SPINLOCK;
            pTCB->taskCriticalZoneStack[pTCB->taskCriticalZoneCount] = SpinlockId;
            pTCB->taskCurrentSpinlockOccupyLevel = SpinlockId;
            pTCB->taskCriticalZoneCount++;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }
        else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
        {
            Os_SLCB[SpinlockId].objOccupyId = (coreIndex | Os_SCB.sysRunningIsrCat2Id);
            Os_SLCB[SpinlockId].objOccupyType = OBJECT_ISR;

            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
            OS_ARCH_ENTRY_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
            /*res and spinlock should together release as LIFO order*/
            pICB->isr2CriticalZoneType[pICB->isr2CriticalZoneCount] = OBJECT_SPINLOCK;
            pICB->isr2CriticalZoneStack[pICB->isr2CriticalZoneCount] = SpinlockId;
            pICB->isrCurrentSpinlockOccupyLevel = SpinlockId;
            pICB->isr2CriticalZoneCount++;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
            OS_ARCH_EXIT_CRITICAL();
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        }
        else
        {
            /* Intentionally Empty */
        }
    }
    else
    {
        *Success = TRYTOGETSPINLOCK_NOSUCCESS;
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_ENTRY_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
        Os_SCB.CurrentSpinlockOccupied[SpinlockId] = FALSE;
        /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
        OS_ARCH_EXIT_CRITICAL();
        /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */

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
FUNC(StatusType, OS_CODE) TryToGetSpinlock(SpinlockIdType SpinlockId, TryToGetSpinlockType* Success)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

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
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_SPINLOCK_TYPE_CAST_002 */
    else if (Os_AddressWritable((uint32)Success) != TRUE)
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        P2CONST(Os_SpinlockCfgType, AUTOMATIC, OS_VAR) pSpinlockCfg = &Os_SpinlockCfg[SpinlockId];
        /* PRQA S 3432, 3678 ++ */ /* MISRA Rule 20.7, 8.13 */ /* OS_SPINLOCK_COMPILER_001 */
        P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) pTCB = Os_SCB.sysRunningTCB;
        P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB = &Os_ICB[Os_SCB.sysRunningIsrCat2Id];
        /* PRQA S 3432, 3678 -- */ /* MISRA Rule 20.7, 8.13 */
        /*Os_00690*/
        if (Os_SCB.CurrentSpinlockOccupied[SpinlockId] == TRUE)
        {
            err = E_OS_INTERFERENCE_DEADLOCK;
        }
        else if (
            (OS_LEVEL_TASK == Os_SCB.sysOsLevel) && (OS_SPINLOCK_INVALID != pTCB->taskCurrentSpinlockOccupyLevel)
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

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
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
FUNC(StatusType, OS_CODE) Os_SpinlockSafetyCheck(void)
{
    VAR(StatusType, OS_VAR) status = E_OK;
    VAR(SpinlockIdType, OS_VAR) i;
    VAR(uint16, OS_VAR) obj_id;

    /*Os_00612*/
    for (i = 0u; i < CFG_SPINLOCK_MAX; i++)
    {
        if (OBJECT_TASK == Os_SLCB[i].objOccupyType)
        {
            obj_id = Os_SLCB[i].objOccupyId;
            /* PRQA S 3469 ++ */ /* MISRA Dir 4.9 */ /* OS_SPINLOCK_MACRO_TO_FUNCTION_003 */
            if ((Os_GetObjLocalId(obj_id) == Os_SCB.sysRunningTaskID) && (Os_SCB.sysCore == Os_GetObjCoreId(obj_id)))
            /* PRQA S 3469 -- */ /* MISRA Dir 4.9 */
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
FUNC(void, OS_CODE) Os_GetInternalSpinlock(Os_SpinlockRefType spinlock)
{
    VAR(uint32, OS_VAR) result;

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
FUNC(void, OS_CODE) Os_ReleaseInternalSpinlock(Os_SpinlockRefType spinlock)
{
    VAR(uint32, OS_VAR) result;

    do
    {
        result = Os_CmpSwapW(spinlock, OS_SPINLOCK_LOCK, OS_SPINLOCK_UNLOCK);
    } while (result > 0u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_AUTOSAR_CORES > 1U || CFG_SPINLOCK_MAX > 0U */

/*=======[E N D   O F   F I L E]==============================================*/
