/* PRQA S 3108++ */
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
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Interrupt.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : interrupt manager                                           **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
/* PRQA S 0791 ++*/ /* MISRA Rule 5.4 */ /*OS_INTERRUPT_SEGMENTNAME_SIMILAR_003*/
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
static VAR(Os_IPLType, OS_VAR) Os_cfgIsr2IplMax;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
static VAR(Os_CallLevelType, OS_VAR) Os_SaveLevel;
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#if (FALSE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static VAR(Os_ArchMsrType, OS_VAR) Os_SaveAllInt;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static VAR(Os_ArchMsrType, OS_VAR) Os_SaveAllIntNested;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
#endif              /* FALSE == CFG_TIMING_PROTECTION_ENABLE */
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Init the Interrupt block control.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
#if (CFG_ISR2_MAX > 0)
FUNC(void, OS_CODE) Os_InitInterrupt(void)
{
    VAR(Os_IsrType, OS_VAR) i;

#if ((CFG_SPINLOCK_MAX > 0U) && (CFG_STD_RESOURCE_MAX > 0U))
    VAR(uint16, OS_VAR) j;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_INTERRUPT_COMPILER_001 */
    P2VAR(Os_ICBType, AUTOMATIC, OS_VAR) pICB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif

    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;

    Os_CfgIsrMax = Os_CfgIsrMax_Inf[vCoreId];
    Os_CfgIsr2Max = Os_CfgIsr2Max_Inf[vCoreId];
    Os_ICB = Os_ICB_Inf[vCoreId];
    Os_ISR2Stack = Os_Isr2Stack_Inf[vCoreId];
    Os_IsrCfg = Os_IsrCfg_Inf[vCoreId];
    Os_cfgIsr2IplMax = Os_CfgIsr2TplMax_Inf[vCoreId];

    Os_SuspendAllCount = 0u;
    Os_SuspendOsCount = 0u;
    Os_SaveOsIntNested = 0u;

#if (CFG_STD_RESOURCE_MAX > 0U)
    CONST(uint16, OS_CONST) vResourceCfgStdMax = Os_CfgStdResourceMax_Inf[vCoreId];
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_INTERRUPT_COMPILER_001 */
    P2VAR(Os_ResourceType, AUTOMATIC, OS_VAR) pIsr2ResStackCfg = Os_ICBIsrC2ResourceStack_Inf[vCoreId];
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

    if (vResourceCfgStdMax > 0U)
    {
        for (i = 0U; i < Os_CfgIsr2Max; i++)
        {
            /* PRQA S 0488 ++ */ /* MISRA Rule 18.4 */ /* OS_INTERRUPT_PTR_OPERATIONS_002 */
            Os_ICB[i].IsrC2ResourceStack = pIsr2ResStackCfg + (i * vResourceCfgStdMax);
            /* PRQA S 0488 -- */ /* MISRA Rule 18.4 */
        }
    }
#endif

    for (i = 0u; i < Os_CfgIsr2Max; i++)
    {
#if ((CFG_SPINLOCK_MAX > 0U) && (CFG_STD_RESOURCE_MAX > 0U))
        pICB = &Os_ICB[i];
        pICB->isr2CriticalZoneCount = 0u;
        pICB->isrCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID;
        for (j = 0u; j < CFG_CRITICAL_ZONE_MAX; j++)
        {
            pICB->isr2CriticalZoneStack[j] = OS_OBJECT_INVALID;
            pICB->isr2CriticalZoneType[j] = OBJECT_MAX;
        }
#endif

/* Init isr2 stack */
#if ((TRUE == CFG_STACK_CHECK) && (CFG_ISR2_MAX > 0U))
        Os_FillStack(Os_ISR2Stack[i]);
#endif
    }
}
#endif /* CFG_ISR2_MAX > 0 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <disable all interrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_DisableAllInterrupts(void)
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningTCB->taskDisableAllCount = 1u;
    }
/*OS368*/
#if (CFG_ISR_MAX > 0)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2DisableAllCount = 1u;
    }
#endif
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

/* Timing protection: resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_ISR2_MAX > 0)
    if (TRUE == Os_SCB.sysInIsrCat2)
    {
        Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_DIS_ALL_INT;
        Os_TmProtIsrStart(Os_SCB.sysRunningIsrCat2Id, TP_ISR_CAT2_DIS_ALL_INT);
    }
    else
#endif /* CFG_ISR2_MAX > 0 */
    {
        if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        {
            Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_DIS_ALL_INT;
            Os_TmProtTaskStart(Os_SCB.sysRunningTaskID, TP_TASK_DIS_ALL_INT);
        }
    }

    Os_ArchDisableAllInt_ButTimingProtInt();
#else
    Os_ArchSuspendInt(&Os_SaveAllInt);
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <disable all interrupts>
 * Service ID           <0xe5>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) DisableAllInterrupts(void)
{
    OS_ENTER_KERNEL();

    Os_DisableAllInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <enable all interrupts>
 * Service ID           <0xe4>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <WaitEvent>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) EnableAllInterrupts(void)
{
    OS_ENTER_KERNEL();

    Os_EnableAllInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <resume all interrupts>
 * Service ID           <0xe6>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SetRelAlarm and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) ResumeAllInterrupts(void)
{
    OS_ENTER_KERNEL();

    Os_ResumeAllInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <suspend all interrupts >
 * Service ID           <0xe7>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SetRelAlarm and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) SuspendAllInterrupts(void)
{
    OS_ENTER_KERNEL();

    Os_SuspendAllInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <resume os interrupts>
 * Service ID           <0xe8>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) ResumeOSInterrupts(void)
{
    OS_ENTER_KERNEL();

    Os_ResumeOSInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <suspend os interrupts>
 * Service ID           <0xe9>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) SuspendOSInterrupts(void)
{
    OS_ENTER_KERNEL();

    Os_SuspendOSInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Enables the interrupt source by modifying the interrupt
 *                       controller registers. Additionally it may clear the
 *                       interrupt pending flag>
 * Service ID           <0x31>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ISRID: The ID of a category 2 ISR.>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) EnableInterruptSource(ISRType ISRID, boolean ClearPending)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;
    /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9 */ /* OS_INTERRUPT_MACRO_004 */
    VAR(ISRType, OS_VAR) isrId = Os_GetObjLocalId(ISRID);
    /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3469,3432 ++ */ /* MISRA Rule 4.9,20.7 */ /* OS_INTERRUPT_MACRO_005 */
    if (CHECK_ID_INVALID(ISRID, Os_CfgIsr2Max_Inf))
    /* PRQA S 3469,3432 -- */ /* MISRA Rule 4.9,20.7 */
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[isrId].OsIsrCatType)
    {
        err = E_OS_ID;
    }
    /*SWS_Os_00809*/
    else
#endif
    {
        VAR(uint32, OS_VAR) vIsrSrc = Os_IsrCfg[isrId].OsIsrSrc;
        VAR(uint32, OS_VAR) vIsrSrcType = Os_IsrCfg[isrId].OsIsrSrcType;

        if (OS_ISR_ENABLED == Os_GetIsrSourceState(vIsrSrc, vIsrSrcType))
        {
            err = E_OS_NOFUNC;
        }
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        else if (Os_WrongContext(OS_CONTEXT_ENABLE_INTERRUPT_SOURCE) != TRUE)
        {
            err = E_OS_CALLEVEL;
        }
        else if (Os_CheckObjAcs(OBJECT_ISR, ISRID) != TRUE)
        {
            err = E_OS_ACCESS;
        }
        else
#endif
        {
            if (TRUE == ClearPending)
            {
                Os_ClearPendingInterrupt(vIsrSrc, vIsrSrcType);
            }
            Os_EnableInterruptSource(vIsrSrc, vIsrSrcType);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(
            OSError_Save_EnableInterruptSource(ISRID, ClearPending),
            OSServiceId_EnableInterruptSource,
            err);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Disables the interrupt source by modifying the interrupt
 *                       controller registers.>
 * Service ID           <0x30>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ISRID: The ID of a category 2 ISR.>
 * @param[in]           <ClearPending: Defines whether the pending flag shall be
 *                       cleared (TRUE) or not (FALSE).>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) DisableInterruptSource(ISRType ISRID)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;
    /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9 */ /* OS_INTERRUPT_MACRO_004 */
    VAR(ISRType, OS_VAR) isrId = Os_GetObjLocalId(ISRID);
    /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3469,3432 ++ */ /* MISRA Rule 4.9,20.7 */ /* OS_INTERRUPT_MACRO_005 */
    if (CHECK_ID_INVALID(ISRID, Os_CfgIsr2Max_Inf))
    /* PRQA S 3469,3432 -- */ /* MISRA Rule 4.9,20.7 */
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[isrId].OsIsrCatType)
    {
        err = E_OS_ID;
    }
    /*SWS_Os_00809*/
    else
#endif
    {
        VAR(uint32, OS_VAR) vIsrSrc = Os_IsrCfg[isrId].OsIsrSrc;
        VAR(uint32, OS_VAR) vIsrSrcType = Os_IsrCfg[isrId].OsIsrSrcType;

        if (OS_ISR_DISABLED == Os_GetIsrSourceState(vIsrSrc, vIsrSrcType))
        {
            err = E_OS_NOFUNC;
        }
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        else if (Os_WrongContext(OS_CONTEXT_DISABLE_INTERRUPT_SOURCE) != TRUE)
        {
            err = E_OS_CALLEVEL;
        }
        else if (Os_CheckObjAcs(OBJECT_ISR, ISRID) != TRUE)
        {
            err = E_OS_ACCESS;
        }
        else
#endif
        {
            Os_DisableInterruptSource(vIsrSrc, vIsrSrcType);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_DisableInterruptSource(ISRID), OSServiceId_DisableInterruptSource, err);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Clears the interrupt pending flag by modifying the interrupt
 *                       controller registers.>
 * Service ID           <0x32>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ISRID: The ID of a category 2 ISR.>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) ClearPendingInterrupt(ISRType ISRID)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;
    /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9 */ /* OS_INTERRUPT_MACRO_004 */
    VAR(ISRType, OS_VAR) isrId = Os_GetObjLocalId(ISRID);
    /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3469,3432 ++ */ /* MISRA Rule 4.9,20.7 */ /* OS_INTERRUPT_MACRO_005 */
    if (CHECK_ID_INVALID(ISRID, Os_CfgIsr2Max_Inf))
    /* PRQA S 3469,3432 -- */ /* MISRA Rule 4.9,20.7 */
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[isrId].OsIsrCatType)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_CLEAR_PENDING_INTERRUPT) != TRUE)
        {
            err = E_OS_CALLEVEL;
        }
        else if (Os_CheckObjAcs(OBJECT_ISR, ISRID) != TRUE)
        {
            err = E_OS_ACCESS;
        }
        else
#endif
        {
            VAR(uint32, OS_VAR) vIsrSrc = Os_IsrCfg[isrId].OsIsrSrc;
            VAR(uint32, OS_VAR) vIsrSrcType = Os_IsrCfg[isrId].OsIsrSrcType;
            Os_ClearPendingInterrupt(vIsrSrc, vIsrSrcType);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_ClearPendingInterrupt(ISRID), OSServiceId_ClearPendingInterrupt, err);
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:EnableAllInterrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_EnableAllInterrupts(void)
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    VAR(StatusType, OS_VAR) err = E_OK;
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        /*OS092*/
        if (Os_SCB.sysRunningTCB->taskDisableAllCount > 0u)
        {
            Os_SCB.sysRunningTCB->taskDisableAllCount = 0u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
#if (CFG_ISR_MAX > 0)
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        /*OS092, OS368*/
        if (Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2DisableAllCount > 0u)
        {
            Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2DisableAllCount = 0u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
    else
    {
        /*nothing to do*/
    }
#endif /* CFG_ISR_MAX > 0 */
    if (E_OK == err)
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
    {
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_ISR2_MAX > 0)
        if (TRUE == Os_SCB.sysInIsrCat2)
        {
            Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_OPT_BUTT;
            Os_TmProtIsrEnd(Os_SCB.sysRunningIsrCat2Id, TP_ISR_CAT2_DIS_ALL_INT);
        }
        else
#endif /* CFG_ISR2_MAX > 0 */
        {
            if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
            {
                Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_OPT_BUTT;
                Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_DIS_ALL_INT);
            }
        }
        Os_ArchEnableAllInt_ButTimingProtInt();
#else
        Os_ArchRestoreInt(Os_SaveAllInt);
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ResumeAllInterrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ResumeAllInterrupts(void)
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    VAR(StatusType, OS_VAR) err = E_OK;
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        /*OS092*/
        if (Os_SCB.sysRunningTCB->taskSuspendAllCount > 0u)
        {
            Os_SCB.sysRunningTCB->taskSuspendAllCount = Os_SCB.sysRunningTCB->taskSuspendAllCount - 1u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
#if (CFG_ISR_MAX > 0)
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        /*OS092, OS368*/
        if (Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendAllCount > 0u)
        {
            Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendAllCount =
                Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendAllCount - 1u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
    else
    {
        /*nothing to do*/
    }
#endif /* CFG_ISR_MAX > 0 */
    if (E_OK == err)
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
    {
        /* AutoSar SC2: Timing protection, resource lock. */
        Os_SuspendAllCount--;
        if (0U == Os_SuspendAllCount)
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        {
#if (CFG_ISR2_MAX > 0)
            if (TRUE == Os_SCB.sysInIsrCat2)
            {
                Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_OPT_BUTT;
                Os_TmProtIsrEnd(Os_SCB.sysRunningIsrCat2Id, TP_ISR_CAT2_SUS_ALL_INT);
            }
            else
#endif /* CFG_ISR2_MAX > 0 */
            {
                if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
                {
                    Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_OPT_BUTT;
                    Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_SUS_ALL_INT);
                }
            }

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
            if (TRUE == Os_InterInitFlag)
            {
                Os_InterMonitorEndRecord(OS_ALL_SUSPEND);
            }
#endif
            Os_ArchEnableAllInt_ButTimingProtInt();
        }
#else
        {
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
            if (TRUE == Os_InterInitFlag)
            {
                Os_InterMonitorEndRecord(OS_ALL_SUSPEND);
            }
#endif
            Os_ArchRestoreInt(Os_SaveAllIntNested);
        }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:SuspendAllInterrupts >
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_SuspendAllInterrupts(void)
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningTCB->taskSuspendAllCount = Os_SCB.sysRunningTCB->taskSuspendAllCount + 1u;
    }
/*OS368*/
#if (CFG_ISR_MAX > 0)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendAllCount =
            Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendAllCount + 1u;
    }
#endif
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

    /* AutoSar SC2: Timing protection, resource lock. */
    if (0U == Os_SuspendAllCount)
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    {
#if (CFG_ISR2_MAX > 0)
        if (TRUE == Os_SCB.sysInIsrCat2)
        {
            Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_SUS_ALL_INT;
            Os_TmProtIsrStart(Os_SCB.sysRunningIsrCat2Id, TP_ISR_CAT2_SUS_ALL_INT);
        }
        else
#endif /* CFG_ISR2_MAX > 0 */
        {
            if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
            {
                Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_SUS_ALL_INT;
                Os_TmProtTaskStart(Os_SCB.sysRunningTaskID, TP_TASK_SUS_ALL_INT);
            }
        }

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
        if (TRUE == Os_InterInitFlag)
        {
            Os_InterMonitorStartRecord(OS_ALL_SUSPEND);
        }
#endif
        Os_ArchDisableAllInt_ButTimingProtInt();
    }
#else
    {
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
        if (TRUE == Os_InterInitFlag)
        {
            Os_InterMonitorStartRecord(OS_ALL_SUSPEND);
        }
#endif
        Os_ArchSuspendInt(&Os_SaveAllIntNested);
    }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
    Os_SuspendAllCount++;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:ResumeOSInterrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ResumeOSInterrupts(void)
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    VAR(StatusType, OS_VAR) err = E_OK;
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        /*OS092*/
        if (Os_SCB.sysRunningTCB->taskSuspendOSCount > 0u)
        {
            Os_SCB.sysRunningTCB->taskSuspendOSCount = Os_SCB.sysRunningTCB->taskSuspendOSCount - 1u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
#if (CFG_ISR_MAX > 0)
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        /*OS092, OS368*/
        if (Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendOSCount > 0u)
        {
            Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendOSCount =
                Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendOSCount - 1u;
        }
        else
        {
            err = E_NOT_OK;
        }
    }
    else
    {
        /*nothing to do*/
    }
#endif /* CFG_ISR_MAX > 0 */
    if (E_OK == err)
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
    {
        Os_SuspendOsCount--;
        if (0U == Os_SuspendOsCount)
        {
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_ISR2_MAX > 0)
            if (TRUE == Os_SCB.sysInIsrCat2)
            {
                Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_OPT_BUTT;
                Os_TmProtIsrEnd(Os_SCB.sysRunningIsrCat2Id, TP_ISR_CAT2_SUS_OS_INT);
            }
            else
#endif /* CFG_ISR2_MAX > 0 */
            {
                if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
                {
                    Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_OPT_BUTT;
                    Os_TmProtTaskEnd(Os_SCB.sysRunningTaskID, TP_TASK_SUS_OS_INT);
                }
            }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
            if (TRUE == Os_InterInitFlag)
            {
                Os_InterMonitorEndRecord(OS_OS_SUSPEND);
            }
#endif
            Os_ArchSetIpl(Os_SaveOsIntNested, OS_ISR_ENABLE);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service:SuspendOSInterrupts>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_SuspendOSInterrupts(void)
{
    OS_ARCH_DECLARE_CRITICAL();

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        Os_SCB.sysRunningTCB->taskSuspendOSCount = Os_SCB.sysRunningTCB->taskSuspendOSCount + 1u;
    }
/*OS368*/
#if (CFG_ISR2_MAX > 0)
    if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
    {
        Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendOSCount =
            Os_ICB[Os_SCB.sysRunningIsrCat2Id].isrC2SuspendOSCount + 1u;
    }
#endif
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
    if (0U == Os_SuspendOsCount)
    {
/* AutoSar SC2: Timing protection, resource lock. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_ISR2_MAX > 0)
        if (TRUE == Os_SCB.sysInIsrCat2)
        {
            Os_ICB[Os_SCB.sysRunningIsrCat2Id].IsrC2IsrOpt = TP_SUS_OS_INT;
            Os_TmProtIsrStart(Os_SCB.sysRunningIsrCat2Id, TP_ISR_CAT2_SUS_OS_INT);
        }
        else
#endif /* CFG_ISR2_MAX > 0 */
        {
            if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
            {
                Os_TCB[Os_SCB.sysRunningTaskID].taskIsrOpt = TP_SUS_OS_INT;
                Os_TmProtTaskStart(Os_SCB.sysRunningTaskID, TP_TASK_SUS_OS_INT);
            }
        }
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

        OS_ARCH_SUSPEND_ALLINT(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
        Os_SaveOsIntNested = Os_ArchGetIpl();
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
        if (TRUE == Os_InterInitFlag)
        {
            Os_InterMonitorStartRecord(OS_OS_SUSPEND);
        }
#endif
        Os_ArchSetIpl(Os_cfgIsr2IplMax, OS_ISR_DISABLE);
        OS_ARCH_RESTORE_ALLINT(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
    }

    Os_SuspendOsCount++;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/********************************************************************/
/*
 * Brief                <GetISRID.>
 * Service ID           <0x01>
 * Sync/Async           <None>
 * Reentrancy           <yes>
 * param-eventId[in]    <Fatalerror>
 * Param-Name[out]      <ProtectionReturnType>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
FUNC(ISRType, OS_CODE) GetISRID(void)
{
    OS_ENTER_KERNEL();
    VAR(ISRType, OS_VAR) osRet = INVALID_ISR;

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    if (Os_WrongContext(OS_CONTEXT_GET_ISR_ID) != TRUE)
    {
        osRet = INVALID_ISR;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        osRet = INVALID_ISR;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* OS263. */
#if (CFG_ISR2_MAX > 0)
        if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
        {
            if (OS_ISR_CATEGORY2 == Os_IsrCfg[Os_SCB.sysRunningIsrCat2Id].OsIsrCatType)
            {
                osRet = Os_SCB.sysRunningIsrCat2Id | (Os_SCB.sysCore << 12u);
            }
        }
#endif
    }

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */

    return osRet;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_ISR2_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <enter ISR2>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_EnterISR2(void)
{
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    Os_EnterIsrRecordTick(Os_IntCfgIsrId);
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

#if ((TRUE == CFG_STACK_CHECK) && (CFG_ISR2_MAX > 0U))
    VAR(Os_StackPtrType, OS_VAR) StackPtr;

    if (0u == Os_IntNestISR2)
    {
        /*EnterISR2 from task.*/
        StackPtr = Os_SCB.sysRunningTCB->taskStackBottom;
    }
    else
    {
        /*EnterISR2 from task.*/
        StackPtr = Os_ISR2Stack[Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - 1U]].stackBottom;
    }
    Os_StackMonitor(StackPtr);
#endif /* TRUE == CFG_STACK_CHECK && CFG_ISR2_MAX > 0U */

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
    Os_InterMonitorRATIO();
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceIsrEnter(Os_IntCfgIsrId);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (TRUE == CFG_INT_NEST_ENABLE)
    if (0U == Os_IntNestISR2)
    {
        Os_SaveLevel = Os_SCB.sysOsLevel;
        Os_SCB.sysOsLevel = OS_LEVEL_ISR2;
    }
#else
    Os_SaveLevel = Os_SCB.sysOsLevel;
    Os_SCB.sysOsLevel = OS_LEVEL_ISR2;
#endif /* TRUE == CFG_INT_NEST_ENABLE */

    Os_IntNestISR2 = Os_IntNestISR2 + 1u;
    Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker + 1u;

    /* Timing protection for isr C2. */
    if (Os_IntCfgIsrId < Os_CfgIsrMax)
    {
        Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - 1U] = Os_IntCfgIsrId;
        Os_SCB.sysRunningIsrCat2Id = Os_IntCfgIsrId;
        Os_SCB.sysInIsrCat2 = TRUE;
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR)
        Os_IncrementIsr2ScheduleCount(Os_IntCfgIsrId);
#endif
/*Writing the running ApplID and Object */
#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysRunningIsrCat2Id].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_ISR;
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtIsrStart(Os_IntCfgIsrId, TP_ISR_CAT2_EXE);
#endif

/*if memory protection is configured*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_MemProtIsrMap();
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <exit ISR2>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ExitISR2(void)
{
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    Os_IsrType isrid = Os_SCB.sysRunningIsrCat2Id;
    VAR(uint32, OS_VAR) curTicks;
#endif

#if ((TRUE == CFG_STACK_CHECK) && (CFG_ISR2_MAX > 0U))
    Os_StackPtrType StackPtr = Os_ISR2Stack[Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - 1U]].stackBottom;
    Os_StackMonitor(StackPtr);
#endif

    Os_IntNestISR2--;
    Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;

    /* Timing protection: stop counter for isr. */
    if (Os_SCB.sysRunningIsrCat2Id < CFG_ISR_MAX)
    {
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
        Os_TmProtIsrEnd(Os_SCB.sysRunningIsrCat2Id, TP_ISR_CAT2_EXE);
#endif
    }

#if (TRUE == CFG_INT_NEST_ENABLE)
    if (0U == Os_IntNestISR2)
#endif
    {
        Os_SCB.sysInIsrCat2 = FALSE;

#if (CFG_SCHED_POLICY != OS_PREEMPTIVE_NON)
        if ((0U == Os_SCB.sysDispatchLocker)
            && ((Os_SCB.sysHighTaskID != Os_SCB.sysRunningTaskID)
/*bcc2 and ecc2*/
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
                || (Os_SCB.sysRunningTCB->taskSelfActCount > 0u)
#endif /* bcc2 and ecc2 */
                    ))
#else /* CFG_SCHED_POLICY == OS_PREEMPTIVE_NON */
        if (((OS_TASK_IDLE == Os_SCB.sysRunningTaskID) && (OS_TASK_IDLE != Os_SCB.sysHighTaskID))
            || (TASK_STATE_SUSPENDED == Os_SCB.sysRunningTCB->taskState))
#endif
        {
            Os_SCB.sysOsLevel = OS_LEVEL_TASK;
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
            if (Os_SCB.sysRunningTCB->taskSelfActCount > 0u)
            {
                Os_SCB.sysRunningTCB->taskSelfActCount = Os_SCB.sysRunningTCB->taskSelfActCount - 1u;
            }
#endif
            OS_TASK_SWITCH_PROC(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_INTERRUPT_MACRO_004 */
        }
        else
        {
#if (CFG_OSAPPLICATION_MAX > 0U)
            Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
            Os_SCB.sysRunningAppObj = OBJECT_TASK;
#endif

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
            Os_MemProtTaskCat2Map();
#endif
        }

#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
        Os_SCB.sysRunningAppObj = OBJECT_TASK;
#endif

        Os_SCB.sysOsLevel = Os_SaveLevel;
    }
#if (TRUE == CFG_INT_NEST_ENABLE)
    else
    {
        Os_SCB.sysRunningIsrCat2Id = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - 1U];
#if (CFG_OSAPPLICATION_MAX > 0U)
        /* IntNest: update sysRunningIsrCat2Id. */
        Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysRunningIsrCat2Id].hostApp;
#endif

#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR)
        Os_IncrementIsr2ScheduleCount(Os_SCB.sysRunningIsrCat2Id);
#endif

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
        Os_MemProtIsrMap();
#endif
    }
#endif /* TRUE == CFG_INT_NEST_ENABLE */

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceIsrExit(isrid, Os_SCB.sysRunningIsrCat2Id);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    curTicks = Os_ExitIsrRecordTick(isrid);
    if (0u == Os_GetObjLocalId(isrid))
    {
        Os_CalcLoadRatio(curTicks);
    }
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* CFG_ISR2_MAX > 0 */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <enter ISR1>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_EnterISR1(void)
{
/* check stack overflow */
#if (TRUE == CFG_STACK_CHECK)
    Os_StackPtrType StackPtr;

    if (0u == Os_IntNestISR1)
    {
        if (0u == Os_IntNestISR2)
        {
            /*Enter ISR1 from task.*/
            StackPtr = Os_SCB.sysRunningTCB->taskStackBottom;
        }
        else
        {
            /*Enter ISR1 from ISR2.*/
            StackPtr = Os_ISR2Stack[Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - 1U]].stackBottom;
        }
    }
    else
    {
        /* Enter ISR1 from ISR1. */
        StackPtr = Os_SystemStack->stackBottom;
    }
    /* check system stack overflow */
    Os_StackMonitor(StackPtr);
#endif /* TRUE == CFG_STACK_CHECK */

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    Os_EnterIsrRecordTick(Os_IntCfgIsrId);
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceIsrEnter(Os_IntCfgIsrId);
#endif /* TRUE == CFG_TRACE_ENABLE */

    Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1] = Os_IntCfgIsrId;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <exit ISR1>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ExitISR1(void)
{
/* check system stack overflow. */
#if (TRUE == CFG_STACK_CHECK)
    Os_StackPtrType StackPtr = Os_SystemStack->stackBottom;
    Os_StackMonitor(StackPtr);
#endif

    Os_IntCfgIsrId = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1 - 1u];
#if (TRUE == CFG_TRACE_ENABLE)
    if ((Os_IntNestISR2 + Os_IntNestISR1) > 1u)
    {
        Os_TraceIsrExit(Os_IntCfgIsrId, Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1 - 2u]);
    }
    else
    {
        Os_TraceIsrExit(Os_IntCfgIsrId, Os_IntCfgIsrId);
    }
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    (void)Os_ExitIsrRecordTick(Os_IntCfgIsrId);
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <make the priority to ipl>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <prio:the priority value of one os object>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <IPL>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchSetIpl>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(Os_IPLType, OS_CODE) Os_PrioToIpl(Os_PriorityType prio)
{
    Os_IPLType Os_IPL;
    if (prio < (Os_CfgPriorityMax - 1u))
    {
        Os_IPL = 0u;
    }
    else
    {
        Os_IPL = prio - Os_CfgPriorityMax + 1u;
    }

    return Os_IPL;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <make the ipl to priority >
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl:interrupt priority level>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <the priority value>
 * PreCondition         <None>
 * CallByAPI            <GetResource and so on >
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(Os_PriorityType, OS_CODE) Os_IplToPrio(Os_IPLType ipl)
{
    return (Os_PriorityType)(ipl + Os_CfgPriorityMax - 1u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
