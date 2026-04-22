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
 **  @file               : Os_Interrupt.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Interrupt API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Interrupt.h"
#include "Os_Appl.h"
#include "Os_Resource.h"
#include "Os_Task.h"
#include "Os_Tprot.h"
#include "Os_Sprot.h"
#include "Os_StackMonitor.h"
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
/**
 * @brief           Initializes the Interrupt Control Block (ICB) for the system
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ICBInit(const Os_SCBType *pScb);

/**
 * @brief           Disables all interrupts in the system
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_DisableAllInterrupts(Os_SCBType *pScb);

/**
 * @brief           Handles task switching after exiting an ISR2
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ExitISR2SwitchTask(Os_SCBType *pScb);

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief           Checks the disable all interrupts counter
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          StatusType
 * @retval          E_OK: Counter check was successful
 * @retval          E_NOT_OK: Counter check failed
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_DisableAllCountCheck(const Os_SCBType *pScb);

/**
 * @brief           Checks and decrements the suspend all interrupts counter
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          StatusType
 * @retval          E_OK: Counter check was successful
 * @retval          E_NOT_OK: Counter check failed
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_SuspendAllCountCheck(const Os_SCBType *pScb);

/**
 * @brief           Checks and decrements the suspend OS interrupts counter
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          StatusType
 * @retval          E_OK: Counter check was successful
 * @retval          E_NOT_OK: Counter check failed
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_SuspendOSCountCheck(const Os_SCBType *pScb);
#endif

#if (TRUE == CFG_STACK_CHECK)
/**
 * @brief           Monitors stack usage when entering an ISR1
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_EnterISR1StackMonitor(const Os_SCBType *pScb);

/**
 * @brief           Monitors stack usage when entering an ISR2
 * @param[in]       pScb: Pointer to the System Control Block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_EnterISR2StackMonitor(const Os_SCBType *pScb);
#endif

/* ========================================== external function definitions ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the Interrupt block control.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitInterrupt(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType sysCore = Os_GetCoreIdLocal();
    Os_SCBType *pScb = Os_GetSystemContext(sysCore);

    pScb->SuspendAllCount = 0u;
    pScb->SuspendOsCount = 0u;
    pScb->SaveOsIntNested = 0u;
    pScb->SaveLevel = OS_LEVEL_ISR2;
#if (FALSE == CFG_TIMING_PROTECTION_ENABLE)
    pScb->SaveAllInt = 0u;
    pScb->SaveAllIntNested = 0u;
#endif
    pScb->IntNestISR1 = 0u;
    pScb->IntNestISR2 = 0u;
    pScb->CfgIsrMax = Os_CfgIsrMax_Inf[sysCore];
    pScb->CfgIsr2Max = Os_CfgIsr2Max_Inf[sysCore];

#if (CFG_ISR2_MAX > 0)
    Os_ICBInit(pScb);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Interrupt init ICB
 */
OS_LOCAL void Os_ICBInit(const Os_SCBType *pScb)
{
#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ICBType *pICB;
#endif                                  /* CFG_STD_RESOURCE_MAX > 0U */

    Os_CoreIdType sysCore = pScb->SysCore;
    Os_IsrType idStartRange = Os_IsrIdRange[sysCore].Isr2.IsrStart;
    Os_IsrType idEndRange = Os_IsrIdRange[sysCore].Isr2.IsrEnd;

#if (CFG_STD_RESOURCE_MAX > 0U)
    if (Os_CfgStdResourceMax_Inf[sysCore] > 0U)
    {
        for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
        {
            pICB = Os_ICB[i];
            pICB->IsrC2ResCount = (Os_ResourceType)0u; /* PRQA S 4342 */ /* VL_Os_4342 */
            pICB->IsrC2ResourceStack = Os_ICBisrC2ResourceStack[i];
        }
    }
#endif

    for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
    {
#if ((CFG_SPINLOCK_MAX > 0U) && (CFG_STD_RESOURCE_MAX > 0U))
        pICB = Os_ICB[i];
        pICB->Isr2CriticalZoneCount = 0u;
        pICB->IsrCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID; /* PRQA S 4424, 4342 */ /* VL_Os_4424, VL_Os_4342 */
        for (uint16 j = 0u; j < CFG_CRITICAL_ZONE_MAX; j++)
        {
            pICB->Isr2CriticalZoneStack[j] = OS_OBJECT_INVALID; /* PRQA S 4424 */ /* VL_Os_4424 */
            pICB->Isr2CriticalZoneType[j] = OS_OBJECT_MAX;
        }
#endif

#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_ICB[i]->CallBackAppID = INVALID_OSAPPLICATION;
#endif

/* Init isr2 schedule count */
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
        Os_ICB[i]->isrScheduleCount = 0u;
#endif

/* Init isr2 stack */
#if ((TRUE == CFG_STACK_CHECK) && (CFG_ISR2_MAX > 0U))
        Os_FillStack(&Os_ISR2Stack[i]);
#endif
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * disable all interrupts
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_DisableAllInterrupts(Os_SCBType *pScb)
/* PRQA S 3673 -- */
{
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442 */
    /* PRQA S 4521, 4544, 2982 ++ */ /* VL_Os_4521, VL_Os_4544, VL_Os_2982 */
    OSRtiEnterApi(pScb, OSApiId_DisableAllInterrupts);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_DisableAllInterrupts_Start, 0);
    /* PRQA S 4521, 4544, 2982 -- */
    /* PRQA S 1317, 1259, 3432, 4442 -- */
    /* PRQA S 3138, 3141 -- */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        pScb->SysRunningTCB->TaskDisableAllCount = 1u;
    }
/*OS368*/
    if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2DisableAllCount = 1u;
    }
#endif

/* Timing protection: resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    if (TRUE == pScb->SysInIsrCat2)
    {
        Os_TmProtIsrStart(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_DIS_ALL_INT);
    }
    else
    {
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            Os_TmProtTaskStart(pScb->SysCore, pScb->SysRunningTaskId, TP_DIS_ALL_INT);
        }
    }

    Os_Hal_DisableAllInt_ButTimingProtInt(pScb->SysCore);
#else
    pScb->SaveAllInt = Os_Hal_SuspendInt();
#endif
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_DisableAllInterrupts);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_DisableAllInterrupts_Return, 0);
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * disable all interrupts
 */
/* PRQA S 3006, 1503, 3408, 1512 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
void DisableAllInterrupts(void)
/* PRQA S 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_DisableAllInterrupts(Os_GetCurrentContext());

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * enable all interrupts
 */
/* PRQA S 3006, 1503, 3408, 1512 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
void EnableAllInterrupts(void)
/* PRQA S 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_EnableAllInterrupts(Os_GetCurrentContext());

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * resume all interrupts
 */
/* PRQA S 3006, 1532, 3408, 1503, 1512 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_Os_3408, VL_QAC_NoUsedApi, VL_Os_1512 */
void ResumeAllInterrupts(void)
/* PRQA S 3006, 1532, 3408, 1503, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_ResumeAllInterrupts(Os_GetCurrentContext());

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * suspend all interrupts
 */
/* PRQA S 3006, 1532, 3408, 1503, 1512 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_Os_3408, VL_QAC_NoUsedApi, VL_Os_1512 */
void SuspendAllInterrupts(void)
/* PRQA S 3006, 1532, 3408, 1503, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_SuspendAllInterrupts(Os_GetCurrentContext());

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * resume os interrupts
 */
/* PRQA S 3006, 1503, 3408, 1512 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
void ResumeOSInterrupts(void)
/* PRQA S 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_ResumeOSInterrupts(Os_GetCurrentContext());

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * suspend os interrupts
 */
/* PRQA S 3006, 1503, 3408, 1512 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
void SuspendOSInterrupts(void)
/* PRQA S 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_SuspendOSInterrupts(Os_GetCurrentContext());

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Enables the interrupt source by modifying the interrupt
 *                       controller registers. Additionally it may clear the
 *                       interrupt pending flag
 */
/* PRQA S 6030, 6070, 3006, 1503, 3408, 1512 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
StatusType EnableInterruptSource(ISRType ISRID, boolean ClearPending)
/* PRQA S 6030, 6070, 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    Os_CoreIdType sysCore = Os_GetCoreIdLocal();
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetSystemContext(sysCore);
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_EnableInterruptSource);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_EnableInterruptSource_Start, ISRID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    StatusType err = E_OK;
    uint32 isrSrc = Os_IsrCfg[ISRID].IsrSrc;
    uint32 isrSrcType = Os_IsrCfg[ISRID].IsrSrcType;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    Os_IsrType idStartRange = Os_IsrIdRange[sysCore].AllIsr.IsrStart;
    Os_IsrType idEndRange = Os_IsrIdRange[sysCore].AllIsr.IsrEnd;

    if ((ISRID < idStartRange) && (ISRID >= idEndRange))
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[ISRID].IsrCatType)
    {
        err = E_OS_ID;
    }
    /*SWS_Os_00809*/
    else if (OS_ISR_ENABLED == Os_Hal_GetIsrSourceState(isrSrc, isrSrcType))
    {
        err = E_OS_NOFUNC;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_ENABLE_INTERRUPT_SOURCE,
            .ObjectType = OS_OBJECT_ISR,
            .ObjectID = (Os_AppObjectId)ISRID,
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
            if (TRUE == ClearPending)
            {
                Os_Hal_ClearPendingInterrupt(isrSrc, isrSrcType);
            }
            Os_Hal_EnableInterruptSource(isrSrc, isrSrcType);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_EnableInterruptSource(ISRID, ClearPending), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_EnableInterruptSource,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_EnableInterruptSource);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_EnableInterruptSource_Return, err);
    UNUSED_PARAMETER(pScb);
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
 * Disables the interrupt source by modifying the interrupt
 *                       controller registers.
 */
/* PRQA S 6070, 3006, 1503, 3408, 1512 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
StatusType DisableInterruptSource(ISRType ISRID)
/* PRQA S 6070, 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    Os_CoreIdType sysCore = Os_GetCoreIdLocal();
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetSystemContext(sysCore);
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_DisableInterruptSource);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_DisableInterruptSource_Start, ISRID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    uint32 isrSrc = Os_IsrCfg[ISRID].IsrSrc;
    uint32 isrSrcType = Os_IsrCfg[ISRID].IsrSrcType;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    Os_IsrType idStartRange = Os_IsrIdRange[sysCore].AllIsr.IsrStart;
    Os_IsrType idEndRange = Os_IsrIdRange[sysCore].AllIsr.IsrEnd;

    if ((ISRID < idStartRange) && (ISRID >= idEndRange))
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[ISRID].IsrCatType)
    {
        err = E_OS_ID;
    }
    /*SWS_Os_00809*/
    else if (OS_ISR_DISABLED == Os_Hal_GetIsrSourceState(isrSrc, isrSrcType))
    {
        err = E_OS_NOFUNC;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_DISABLE_INTERRUPT_SOURCE,
            .ObjectType = OS_OBJECT_ISR,
            .ObjectID = (Os_AppObjectId)ISRID,
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
            Os_Hal_DisableInterruptSource(isrSrc, isrSrcType);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_DisableInterruptSource(ISRID), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_DisableInterruptSource,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_DisableInterruptSource);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_DisableInterruptSource_Return, err);
    UNUSED_PARAMETER(pScb);
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
 * Disable all interrupt owned by the application
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_DisableIntSourceInApp(const Os_ApplicationCfgType *posCurAppCfg, boolean *intSourceFlag)
/* PRQA S 1532 -- */
{
    uint16 isrRefCnt = posCurAppCfg->AppIsrRefCnt;
    /* PRQA S 3305, 3678, 0310 ++ */ /* VL_Os_3305, VL_Os_3678, VL_Os_0310 */
    Os_IsrType *arrayAppIsr = (Os_IsrType *)posCurAppCfg->AppObjectRef[OS_OBJECT_ISR];
    /* PRQA S 3305, 3678, 0310 -- */
    for (uint16 i = 0u; i < isrRefCnt; i++)
    {
        /* PRQA S 3305, 3678, 0310 ++ */ /* VL_Os_3305, VL_Os_3678, VL_Os_0310 */
        uint32 isrId = (uint32)arrayAppIsr[i];
        /* PRQA S 3305, 3678, 0310 -- */
        uint32 isrSrc = Os_IsrCfg[isrId].IsrSrc;
        uint32 isrSrcType = Os_IsrCfg[isrId].IsrSrcType;

        if (OS_ISR_DISABLED == Os_Hal_GetIsrSourceState(isrSrc, isrSrcType))
        {
            intSourceFlag[i] = FALSE;
        }
        else
        {
            intSourceFlag[i] = TRUE;
            /* PRQA S 0303, 3138 ++ */ /* VL_Os_0303, VL_Os_3138 */
            /* PRQA S 3442, 3345 ++ */ /* VL_Os_3442, VL_Os_3345 */
            Os_Hal_DisableInterruptSource(isrSrc, isrSrcType);
            /* PRQA S 3442, 3345 -- */
            /* PRQA S 0303, 3138 -- */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Resume the originally enabled interrupt owned by the application
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_ResumeIntSourceInApp(const Os_ApplicationCfgType *posCurAppCfg, const boolean *intSourceFlag)
/* PRQA S 1532 -- */
{
    uint16 isrRefCnt = posCurAppCfg->AppIsrRefCnt;
    /* PRQA S 3305, 3678, 0310 ++ */ /* VL_Os_3305, VL_Os_3678, VL_Os_0310 */
    Os_IsrType *arrayAppIsr = (Os_IsrType *)posCurAppCfg->AppObjectRef[OS_OBJECT_ISR];
    /* PRQA S 3305, 3678, 0310 -- */
    for (uint16 i = 0u; i < isrRefCnt; i++)
    {
        if (intSourceFlag[i] == TRUE)
        {
            /* PRQA S 3305, 3678, 0310 ++ */ /* VL_Os_3305, VL_Os_3678, VL_Os_0310 */
            uint32 isrId = (uint32)arrayAppIsr[i];
            /* PRQA S 3305, 3678, 0310 -- */
            uint32 IsrSrc = Os_IsrCfg[isrId].IsrSrc;
            uint32 isrSrcType = Os_IsrCfg[isrId].IsrSrcType;

            /* PRQA S 0303, 3138 ++ */ /* VL_Os_0303, VL_Os_3138 */
            /* PRQA S 3442, 3345 ++ */ /* VL_Os_3442, VL_Os_3345 */
            Os_Hal_EnableInterruptSource(IsrSrc, isrSrcType);
            /* PRQA S 0303, 3138 -- */
            /* PRQA S 3442, 3345 -- */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Clears the interrupt pending flag by modifying the interrupt
 *                       controller registers.
 */
/* PRQA S 6070, 3006, 1503, 3408, 1512 ++ */ /* VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_NoUsedApi, VL_Os_3408, VL_Os_1512 */
StatusType ClearPendingInterrupt(ISRType ISRID)
/* PRQA S 6070, 3006, 1503, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;
    Os_CoreIdType sysCore = Os_GetCoreIdLocal();
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetSystemContext(sysCore);
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_ClearPendingInterrupt);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ClearPendingInterrupt_Start, ISRID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */


#if (OS_STATUS_EXTENDED == CFG_STATUS)
    Os_IsrType idStartRange = Os_IsrIdRange[sysCore].AllIsr.IsrStart;
    Os_IsrType idEndRange = Os_IsrIdRange[sysCore].AllIsr.IsrEnd;

    if ((ISRID < idStartRange) && (ISRID >= idEndRange))
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[ISRID].IsrCatType)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_CLEAR_PENDING_INTERRUPT,
            .ObjectType = OS_OBJECT_ISR,
            .ObjectID = (Os_AppObjectId)ISRID,
            .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_1258 */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
            uint32 isrSrc = Os_IsrCfg[ISRID].IsrSrc;
            uint32 isrSrcType = Os_IsrCfg[ISRID].IsrSrcType;
            Os_Hal_ClearPendingInterrupt(isrSrc, isrSrcType);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_ClearPendingInterrupt(ISRID), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_ClearPendingInterrupt,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_ClearPendingInterrupt);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ClearPendingInterrupt_Return, err);
    UNUSED_PARAMETER(pScb);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:EnableAllInterrupts
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_DisableAllCountCheck(const Os_SCBType *pScb)
{
    StatusType err = E_OK;

    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        /*OS092*/
        if (pScb->SysRunningTCB->TaskDisableAllCount > 0u)
        {
            pScb->SysRunningTCB->TaskDisableAllCount = 0u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel) /* PRQA S 2004 */ /* VL_Os_2004 */
    {
        /*OS092, OS368*/
        if (Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2DisableAllCount > 0u)
        {
            Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2DisableAllCount = 0u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:ResumeAllInterrupts
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_SuspendAllCountCheck(const Os_SCBType *pScb)
{
    StatusType err = E_OK;

    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        /*OS092*/
        if (pScb->SysRunningTCB->TaskSuspendAllCount > 0u)
        {
            pScb->SysRunningTCB->TaskSuspendAllCount =
                pScb->SysRunningTCB->TaskSuspendAllCount - 1u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
#if (CFG_ISR_MAX > 0)
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel) /* PRQA S 2004 */ /* VL_Os_2004 */
    {
        /*OS092, OS368*/
        if (Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendAllCount > 0u)
        {
            Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendAllCount =
                Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendAllCount - 1u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
#endif

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:ResumeOSInterrupts
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE StatusType Os_SuspendOSCountCheck(const Os_SCBType *pScb)
{
    StatusType err = E_OK;

    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        /*OS092*/
        if (pScb->SysRunningTCB->TaskSuspendOsCount > 0u)
        {
            pScb->SysRunningTCB->TaskSuspendOsCount =
                pScb->SysRunningTCB->TaskSuspendOsCount - 1u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
#if (CFG_ISR_MAX > 0)
    else if (OS_LEVEL_ISR2 == pScb->SysOsLevel) /* PRQA S 2004 */ /* VL_Os_2004 */
    {
        /*OS092, OS368*/
        if (Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendOSCount > 0u)
        {
            Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendOSCount =
                Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendOSCount - 1u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
#endif

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:EnableAllInterrupts
 */
/* PRQA S 3673, 1505 ++ */ /* VL_QAC_3673, VL_Os_1505 */
void Os_EnableAllInterrupts(Os_SCBType *pScb)
/* PRQA S 3673, 1505 -- */
{
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544, 2982 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544, VL_Os_2982 */
    OSRtiEnterApi(pScb, OSApiId_EnableAllInterrupts);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_EnableAllInterrupts_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544, 2982 -- */
    /* PRQA S 3138, 3141 -- */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    StatusType err = Os_DisableAllCountCheck(pScb);
    if (E_OK == err)
#endif
    {
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)

        if (TRUE == pScb->SysInIsrCat2)
        {
            Os_TmProtIsrEnd(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_DIS_ALL_INT);
        }
        else
        {
            if (OS_LEVEL_TASK == pScb->SysOsLevel)
            {
                Os_TmProtTaskEnd(pScb->SysCore, pScb->SysRunningTaskId, TP_DIS_ALL_INT);
            }
        }
        Os_Hal_EnableAllInt_ButTimingProtInt(pScb->SysCore);
#else
        Os_Hal_RestoreInt(pScb->SaveAllInt);
#endif
    }
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_EnableAllInterrupts);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_EnableAllInterrupts_Return, 0);
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:ResumeAllInterrupts
 */
void Os_ResumeAllInterrupts(Os_SCBType *pScb) /* PRQA S 1505 */ /* VL_Os_1505 */
{
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544, 2982 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544, VL_Os_2982 */
    OSRtiEnterApi(pScb, OSApiId_ResumeAllInterrupts);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ResumeAllInterrupts_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544, 2982 -- */
    /* PRQA S 3138, 3141 -- */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    StatusType err = Os_SuspendAllCountCheck(pScb);
    if (E_OK == err)
#endif
    {
        /* AutoSar SC2: Timing protection, resource lock. */
        pScb->SuspendAllCount--;
        if (0U == pScb->SuspendAllCount)
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        {
            if (TRUE == pScb->SysInIsrCat2)
            {
                Os_TmProtIsrEnd(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_SUS_ALL_INT);
            }
            else
            {
                if (OS_LEVEL_TASK == pScb->SysOsLevel)
                {
                    Os_TmProtTaskEnd(pScb->SysCore, pScb->SysRunningTaskId, TP_SUS_ALL_INT);
                }
            }

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332*/ /* VL_Os_3332*/
            if (TRUE == pScb->InterInitFlag)
            {
                /* PRQA S 3200 ++ */ /* VL_Os_3200 */
                Os_InterMonitorEndRecord(OS_ALL_SUSPEND);
                /* PRQA S 3200 -- */
            }
#endif
            Os_Hal_EnableAllInt_ButTimingProtInt(pScb->SysCore);
        }
#else
        {

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
            if (TRUE == pScb->InterInitFlag)
            {
                Os_InterMonitorEndRecord(OS_ALL_SUSPEND); /* PRQA S 3200 */ /* VL_Os_3200 */
            }
#endif
            Os_Hal_RestoreInt(pScb->SaveAllIntNested);
        }
#endif
    }
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_ResumeAllInterrupts);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ResumeAllInterrupts_Return, 0);
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:SuspendAllInterrupts
 */
void Os_SuspendAllInterrupts(Os_SCBType *pScb)/* PRQA S 1505 */ /* VL_Os_1505 */
{
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442 */
    /* PRQA S 4521, 4544, 2982 ++ */ /* VL_Os_4521, VL_Os_4544, VL_Os_2982 */
    OSRtiEnterApi(pScb, OSApiId_SuspendAllInterrupts);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SuspendAllInterrupts_Start, 0);
    /* PRQA S 4521, 4544, 2982 -- */
    /* PRQA S 1317, 1259, 3432, 4442 -- */
    /* PRQA S 3138, 3141 -- */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (OS_LEVEL_TASK == pScb->SysOsLevel)
    {
        pScb->SysRunningTCB->TaskSuspendAllCount =
            pScb->SysRunningTCB->TaskSuspendAllCount + 1u;
    }
/*OS368*/
    if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
    {
        Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendAllCount =
            Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendAllCount + 1u;
    }
#endif

    /* AutoSar SC2: Timing protection, resource lock. */
    if (0U == pScb->SuspendAllCount)
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    {
        if (TRUE == pScb->SysInIsrCat2)
        {
            Os_TmProtIsrStart(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_SUS_ALL_INT);
        }
        else
        {
            if (OS_LEVEL_TASK == pScb->SysOsLevel)
            {
                Os_TmProtTaskStart(pScb->SysCore, pScb->SysRunningTaskId, TP_SUS_ALL_INT);
            }
        }

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332*/ /* VL_Os_3332*/
        if (TRUE == pScb->InterInitFlag)
        {
            Os_InterMonitorStartRecord(OS_ALL_SUSPEND);
        }
#endif
        Os_Hal_DisableAllInt_ButTimingProtInt(pScb->SysCore);
    }
#else
    {
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
        if (TRUE == pScb->InterInitFlag)
        {
            Os_InterMonitorStartRecord(OS_ALL_SUSPEND);
        }
#endif
        pScb->SaveAllIntNested = Os_Hal_SuspendInt();
    }
#endif
    pScb->SuspendAllCount++;
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
    OSRtiExitApi(pScb, OSApiId_SuspendAllInterrupts);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SuspendAllInterrupts_Return, 0);
    /* PRQA S 3432, 4544, 1259 -- */
    /* PRQA S 3138, 3141 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:ResumeOSInterrupts
 */
void Os_ResumeOSInterrupts(Os_SCBType *pScb) /* PRQA S 1505, 6030 */ /* VL_Os_1505, VL_MTR_Os_STMIF */
{
    if (OS_LEVEL_ISR1 != pScb->SysOsLevel)
    {
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544, 2982 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544, VL_Os_2982 */
        OSRtiEnterApi(pScb, OSApiId_ResumeOSInterrupts);
        ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ResumeOSInterrupts_Start, 0);
        /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544, 2982 -- */
        /* PRQA S 3138, 3141 -- */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        StatusType err = Os_SuspendOSCountCheck(pScb);
        if (E_OK == err)
#endif
        {
            pScb->SuspendOsCount--;
            if (0U == pScb->SuspendOsCount)
            {
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
                if (TRUE == pScb->SysInIsrCat2)
                {
                    Os_TmProtIsrEnd(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_SUS_OS_INT);
                }
                else
                {
                    if (OS_LEVEL_TASK == pScb->SysOsLevel)
                    {
                        Os_TmProtTaskEnd(pScb->SysCore, pScb->SysRunningTaskId, TP_SUS_OS_INT);
                    }
                }
#endif

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
                if (TRUE == pScb->InterInitFlag)
                {
                    /* PRQA S 3200 ++ */ /* VL_Os_3200 */
                    Os_InterMonitorEndRecord(OS_OS_SUSPEND);
                    /* PRQA S 3200 -- */
                }
#endif
                Os_Hal_SetIpl(pScb->SaveOsIntNested, OS_ISR_ENABLE);
            }
        }
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
        OSRtiExitApi(pScb, OSApiId_ResumeOSInterrupts);
        ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_ResumeOSInterrupts_Return, 0);
        /* PRQA S 3432, 4544, 1259 -- */
        /* PRQA S 3138, 3141 -- */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service:SuspendOSInterrupts
 */
/* PRQA S 1505 ++ */ /* VL_Os_1505 */
void Os_SuspendOSInterrupts(Os_SCBType *pScb)
/* PRQA S 1505 -- */
{
    if (OS_LEVEL_ISR1 != pScb->SysOsLevel)
    {
        OS_HAL_DECLARE_CRITICAL();
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 1317, 1259, 3432, 4442 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442 */
        /* PRQA S 4521, 4544, 2982 ++ */ /* VL_Os_4521, VL_Os_4544, VL_Os_2982 */
        OSRtiEnterApi(pScb, OSApiId_SuspendOSInterrupts);
        ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SuspendOSInterrupts_Start, 0);
        /* PRQA S 4521, 4544, 2982 -- */
        /* PRQA S 1317, 1259, 3432, 4442 -- */
        /* PRQA S 3138, 3141 -- */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        if (OS_LEVEL_TASK == pScb->SysOsLevel)
        {
            pScb->SysRunningTCB->TaskSuspendOsCount =
                pScb->SysRunningTCB->TaskSuspendOsCount + 1u;
        }
        /*OS368*/
        if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
        {
            Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendOSCount =
                Os_ICB[pScb->SysRunningIsrCat2Id]->IsrC2SuspendOSCount + 1u;
        }
#endif
        if (0U == pScb->SuspendOsCount)
        {
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
            if (TRUE == pScb->SysInIsrCat2)
            {
                Os_TmProtIsrStart(pScb->SysCore, pScb->SysRunningIsrCat2Id, TP_SUS_OS_INT);
            }
            else
            {
                if (OS_LEVEL_TASK == pScb->SysOsLevel)
                {
                    Os_TmProtTaskStart(pScb->SysCore, pScb->SysRunningTaskId, TP_SUS_OS_INT);
                }
            }
#endif

            OS_HAL_ENTRY_CRITICAL();
            pScb->SaveOsIntNested = Os_Hal_GetIpl();
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
            if (TRUE == pScb->InterInitFlag)
            {
                Os_InterMonitorStartRecord(OS_OS_SUSPEND);
            }
#endif
            Os_Hal_SetIpl(Os_CfgIsr2ExcludeTplMax_Inf[pScb->SysCore], OS_ISR_DISABLE);
            OS_HAL_EXIT_CRITICAL();
        }

        /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
        /* PRQA S 3432, 4544, 1259 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1259 */
        OSRtiExitApi(pScb, OSApiId_SuspendOSInterrupts);
        ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SuspendOSInterrupts_Return, 0);
        /* PRQA S 3432, 4544, 1259 -- */
        /* PRQA S 3138, 3141 -- */
        pScb->SuspendOsCount++;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * GetISRID.
 */
/* PRQA S 1503, 3006, 3408, 1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_Os_1512 */
ISRType GetISRID(void)
/* PRQA S 1503, 3006, 3408, 1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    ISRType ret = INVALID_ISR;/* PRQA S 1297*/ /* VL_Os_1297*/
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_1259, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetISRID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetISRID_Start, 0);
    /* PRQA S 1317, 1259, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    StatusType err = E_OK;
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_ISR_ID,
        .ObjectType = OS_OBJECT_INVALID,
        .ObjectID = (Os_AppObjectId)0U,
        .Address = NULL_PARA, /* PRQA S 1258 */ /* VL_Os_ConstToIntegral */
    };
    err = Os_ServiceProtCheck(pScb, &SprotParam);
    if (E_OK == err)
#endif
    {
/* OS263. */
        if (OS_LEVEL_ISR2 == pScb->SysOsLevel)
        {
            if (OS_ISR_CATEGORY2 == Os_IsrCfg[pScb->SysRunningIsrCat2Id].IsrCatType)
            {
                ret = pScb->SysRunningIsrCat2Id;
            }
        }
    }

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_GetISRID);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetISRID_Return, ret);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return ret;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_STACK_CHECK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * enter ISR Stack Monitor
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_EnterISR1StackMonitor(const Os_SCBType *pScb)
{
    const Os_StackType *stackPtr;

    if (0u == pScb->IntNestISR1)
    {
        if (0u == pScb->IntNestISR2)
        {
            /*Enter ISR1 from task.*/
            stackPtr = &Os_TaskStack[pScb->SysRunningTaskId];
        }
        else
        {
            /*Enter ISR1 from ISR2.*/
            stackPtr = &Os_ISR2Stack[pScb->SysIsrNestQueue[pScb->IntNestISR2 - 1U]];
        }
    }
    else
    {
        /* Enter ISR1 from ISR1. */
        stackPtr = pScb->SystemStack;
    }

    Os_StackMonitor(stackPtr);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * enter ISR Stack Monitor
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_EnterISR2StackMonitor(const Os_SCBType *pScb)
{
    const Os_StackType *stackPtr;

    if (0u == pScb->IntNestISR2)
    {
        /*EnterISR2 from task.*/
        stackPtr = &Os_TaskStack[pScb->SysRunningTaskId];
    }
    else
    {
        /*EnterISR2 from task.*/
        stackPtr = &Os_ISR2Stack[pScb->SysIsrNestQueue[pScb->IntNestISR2 - 1U]];
    }

    Os_StackMonitor(stackPtr);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * exit ISR2
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_ExitISR2SwitchTask(Os_SCBType *pScb)
{
#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
    if ((0U == pScb->SysDispatchLocker) && (pScb->SysHighTaskId != pScb->SysRunningTaskId))
#else /* CFG_SCHED_POLICY == OS_PREEMPTIVE_NON */
    if (((Os_TASK_IDLE_Inf[pScb->SysCore] == pScb->SysRunningTaskId) && (Os_TASK_IDLE_Inf[pScb->SysCore] != pScb->SysHighTaskId)) || (OS_TASK_STATE_SUSPENDED == pScb->SysRunningTCB->TaskState))
#endif
    {
        pScb->SysOsLevel = OS_LEVEL_TASK;
        OS_HAL_TASK_SWITCH_PROC(pScb->SysCore); /* PRQA S 1520 */ /* VL_Os_1520 */
    }
    else
    {
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        if (pScb->SysAppId != pScb->SysRunningAppId)
        {
            Os_ApplicationType appId  = Os_TaskCfg[pScb->SysRunningTaskId].ObjAppCfg->HostApp;
            Os_Hal_MemProtTaskMap(pScb->SysRunningTaskId, appId); /* PRQA S 1520 */ /* VL_Os_1520 */
        }
        else
        {
            /* SYS_APP, as OS kernel, have all access rights */
            Os_Hal_MemProtKernelMap();
        }
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TpResume(pScb, TP_FOR_TASK);
#endif
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_TprotTerminateIsr(Os_SCBType *pScb)
{
    /* PRQA S 3442 ++ */ /* VL_Os_3442 */
    if (OS_PROTECTION_TERMINATE_TPROT_ISR == pScb->ProtectionTerminateObj)
    /* PRQA S 3442 -- */                       
    {                                                           
        pScb->IntNestISR2--;                                    
        pScb->SysDispatchLocker = pScb->SysDispatchLocker - 1u;                         
        if (0U == pScb->IntNestISR2)                            
        {                                                       
            pScb->SysOsLevel = OS_LEVEL_TASK;                   
        }                                                        
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Memory Protection for ISR
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_MemProtIsrMap(Os_SCBType *pScb, Os_IsrType isrId)
/* PRQA S 3673 -- */
{
    if (pScb->SysAppId != pScb->SysRunningAppId)
    {
        Os_ApplicationType appId = Os_IsrCfg[isrId].ObjAppCfg->HostApp;
        Os_Hal_MemProtIsrMap(isrId, appId); /* PRQA S 1520 */ /* VL_Os_1520 */
    }
    else
    {
        Os_Hal_MemProtKernelMap();
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * enter ISR2
 */
/* PRQA S 6070, 1532 ++ */ /* VL_MTR_Os_STCAL, VL_QAC_OneFunRef */
void Os_EnterISR2(Os_IsrType isrId)
/* PRQA S 6070, 1532 -- */
{
    Os_SCBType *pScb = OS_ISR_GET_SCB(isrId);

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_EnterIsrRecordTick(isrId);
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_CAT2ISR, Os, pScb->SysCore, OsCat2Isr_Start, isrId);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */

#if ((TRUE == CFG_STACK_CHECK) && (CFG_ISR2_MAX > 0U))
    Os_EnterISR2StackMonitor(pScb);
#endif

#if (TRUE == CFG_INT_NEST_ENABLE)
    if (0U == pScb->IntNestISR2)
    {
        pScb->SaveLevel = pScb->SysOsLevel;
        pScb->SysOsLevel = OS_LEVEL_ISR2;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        /* FIX_ME */
        if( isrId != Os_TprotId_Inf[pScb->SysCore])
        {
            Os_TpSupend(pScb, TP_FOR_TASK);
        }
    }
    else
    {
        if( isrId != Os_TprotId_Inf[pScb->SysCore])
        {
            Os_TpSupend(pScb, TP_FOR_ISR);
        }
#endif
    }
#else
    pScb->SaveLevel = pScb->SysOsLevel;
    pScb->SysOsLevel = OS_LEVEL_ISR2;
#endif

    pScb->IntNestISR2 = pScb->IntNestISR2 + 1u;
    pScb->SysDispatchLocker = pScb->SysDispatchLocker + 1u;

    /* Timing protection for isr C2. */
    if ((uint16)isrId < CFG_ISR_MAX)
    {
        pScb->SysIsrNestQueue[pScb->IntNestISR2 - 1U] = isrId;
        pScb->SysRunningIsrCat2Id = isrId;
        pScb->SysInIsrCat2 = TRUE;
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
        Os_IncrementIsr2ScheduleCount(isrId);
#endif

/*Writing the running ApplID and Object */
#if (CFG_OSAPPLICATION_MAX > 0U)
        pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_ISR, (AppObjectId)pScb->SysRunningIsrCat2Id);
        pScb->SysRunningAppObj = OS_OBJECT_ISR;
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtIsrStart(pScb->SysCore, isrId, TP_EXE);
#endif

/*if memory protection is configured*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_MemProtIsrMap(pScb, isrId);
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * exit ISR2
 */
/* PRQA S 6070, 1532 ++ */ /* VL_MTR_Os_STCAL, VL_QAC_OneFunRef */
void Os_ExitISR2(Os_IsrType isrId)
/* PRQA S 6070, 1532 -- */
{
	Os_CoreIdType coreId = Os_IsrCfg[isrId].HostCore;
    Os_SCBType *pScb = OS_ISR_GET_SCB(isrId);
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_TimerTickType    curTicks;
#endif

#if ((TRUE == CFG_STACK_CHECK) && (CFG_ISR2_MAX > 0U))
    const Os_StackType *stackPtr = &Os_ISR2Stack[pScb->SysIsrNestQueue[pScb->IntNestISR2 - 1U]];
    Os_StackMonitor(stackPtr);
#endif

    pScb->IntNestISR2--;
    pScb->SysDispatchLocker = pScb->SysDispatchLocker - 1u;

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    /* Timing protection: stop counter for isr. */
    Os_TmProtIsrEnd(coreId, pScb->SysRunningIsrCat2Id, TP_EXE);
#endif

#if (TRUE == CFG_INT_NEST_ENABLE)
    if (0U == pScb->IntNestISR2)
#endif
    {
        pScb->SysInIsrCat2 = FALSE;

#if (CFG_OSAPPLICATION_MAX > 0U)
        if (Os_TCB[pScb->SysRunningTaskId]->CallBackAppID != INVALID_OSAPPLICATION)
        {
            pScb->SysRunningAppId = Os_TCB[pScb->SysRunningTaskId]->CallBackAppID;
        }
        else
        {
            pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_TASK, (AppObjectId)pScb->SysRunningTaskId);
        }
        pScb->SysRunningAppObj = OS_OBJECT_TASK;
#endif

        Os_ExitISR2SwitchTask(pScb); /* PRQA S 1520 */ /* VL_Os_1520 */

        pScb->SysOsLevel = pScb->SaveLevel;
    }
#if (TRUE == CFG_INT_NEST_ENABLE)
    else
    {
        pScb->SysRunningIsrCat2Id = pScb->SysIsrNestQueue[pScb->IntNestISR2 - 1U];
#if (CFG_OSAPPLICATION_MAX > 0U)
        /* IntNest: update SysRunningIsrCat2Id. */
        if (Os_ICB[pScb->SysRunningIsrCat2Id]->CallBackAppID != INVALID_OSAPPLICATION)
        {
            pScb->SysRunningAppId = Os_ICB[pScb->SysRunningIsrCat2Id]->CallBackAppID;
        }
        else
        {
            pScb->SysRunningAppId = Os_GetObjectAppID(OS_OBJECT_ISR, (AppObjectId)pScb->SysRunningIsrCat2Id);
        }
#endif

#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
        Os_IncrementIsr2ScheduleCount(pScb->SysRunningIsrCat2Id);
#endif

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_MemProtIsrMap(pScb, pScb->SysRunningIsrCat2Id); /* PRQA S 1520 */ /* VL_Os_1520 */
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TpResume(pScb, TP_FOR_ISR);
#endif
    }
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    curTicks = Os_ExitIsrRecordTick(isrId);
    
    if (Os_IsrIdRange[coreId].AllIsr.IsrStart == isrId)
    {
        Os_CalcLoadRatio(curTicks);
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1821, 4532, 4544, 4542 ++ */ /* VL_Os_1821, VL_Os_4532, VL_Os_4544, VL_Os_4542 */
    /* PRQA S 4543, 4523, 3762, 1277 ++ */ /* VL_Os_4543, VL_Os_4523, VL_Os_3762, VL_Os_1277 */
    ARTI_TRACE(NOSUSP, AR_CP_OS_CAT2ISR, Os, coreId, OsCat2Isr_Stop, isrId);
    /* PRQA S 4543, 4523, 3762, 1277 -- */
    /* PRQA S 1821, 4532, 4544, 4542 -- */
    /* PRQA S 3138, 3141 -- */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TprotTerminateIsr(pScb);                                                           
#endif

#if (TRUE == CFG_STACK_CHECK)
    Os_StackMonitor(pScb->SystemStack);
#endif

    UNUSED_PARAMETER(isrId);
    UNUSED_PARAMETER(coreId);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * enter ISR1
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_EnterISR1(Os_IsrType isrId)
/* PRQA S 1532 -- */
{
    Os_SCBType *pScb = OS_ISR_GET_SCB(isrId);

#if (TRUE == CFG_INT_NEST_ENABLE)
    if (0U == pScb->IntNestISR1)
#endif
    {
        pScb->SaveLevelISR1 = pScb->SysOsLevel;
        pScb->SysOsLevel = OS_LEVEL_ISR1;
    }
/* check stack overflow */
#if (TRUE == CFG_STACK_CHECK)
    Os_EnterISR1StackMonitor(pScb);
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_EnterIsrRecordTick(isrId);
#endif

    pScb->SysIsrNestQueue[pScb->IntNestISR2 + pScb->IntNestISR1] = isrId;

#if (TRUE == CFG_INT_NEST_ENABLE)
    pScb->IntNestISR1++;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * exit ISR1
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_ExitISR1(Os_IsrType isrId)
/* PRQA S 1532 -- */
{
    Os_SCBType *pScb = OS_ISR_GET_SCB(isrId);

/* check system stack overflow. */
#if (TRUE == CFG_STACK_CHECK)
    Os_StackMonitor(pScb->SystemStack);
#endif

#if (TRUE == CFG_INT_NEST_ENABLE)
    pScb->IntNestISR1--;
    if (0U == pScb->IntNestISR1)
#endif
    {
        pScb->SysOsLevel = pScb->SaveLevelISR1;
    }

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    (void)Os_ExitIsrRecordTick(isrId);
#endif

    UNUSED_PARAMETER(isrId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * make the priority to ipl
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_IPLType Os_PrioToIpl(const Os_SCBType *pScb, Os_PriorityType prio)
/* PRQA S 1532 -- */
{
    Os_IPLType Os_IPL;
    if (prio < (pScb->PriorityNum - 1u))
    {
        Os_IPL = 0u;
    }
    else
    {
        Os_IPL = prio - pScb->PriorityNum + 1u;
    }

    return Os_IPL;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * make the ipl to priority
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_PriorityType Os_IplToPrio(const Os_SCBType *pScb, Os_IPLType ipl)
/* PRQA S 1532 -- */
{
    return (Os_PriorityType)(ipl + pScb->PriorityNum - 1u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
