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
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
static Os_IPLType Os_cfgIsr2IplMax;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
static Os_CallLevelType Os_SaveLevel;
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#if (FALSE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static Os_ArchMsrType Os_SaveAllInt;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static Os_ArchMsrType Os_SaveAllIntNested;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
#endif /* FALSE == CFG_TIMING_PROTECTION_ENABLE */

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
void Os_InitInterrupt(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Os_IsrType i;

#if ((CFG_SPINLOCK_MAX > 0U) && (CFG_STD_RESOURCE_MAX > 0U))
    uint16      j;
    Os_ICBType* pICB;
#endif

    uint16 vCoreId = Os_SCB.sysCore;

    Os_CfgIsrMax     = Os_CfgIsrMax_Inf[vCoreId];
    Os_CfgIsr2Max    = Os_CfgIsr2Max_Inf[vCoreId];
    Os_ICB           = Os_ICB_Inf[vCoreId];
    Os_ISR2Stack     = Os_Isr2Stack_Inf[vCoreId];
    Os_IsrCfg        = Os_IsrCfg_Inf[vCoreId];
    Os_cfgIsr2IplMax = Os_CfgIsr2TplMax_Inf[vCoreId];

    Os_SuspendAllCount = 0u;
    Os_SuspendOsCount  = 0u;
    Os_SaveOsIntNested = 0u;

#if (CFG_STD_RESOURCE_MAX > 0U)
    const uint16     vResourceCfgStdMax = Os_CfgStdResourceMax_Inf[vCoreId];
    Os_ResourceType* pIsr2ResStackCfg   = Os_ICBIsrC2ResourceStack_Inf[vCoreId];

    if (vResourceCfgStdMax > 0U)
    {
        for (i = 0U; i < Os_CfgIsr2Max; i++)
        {
            /* PRQA S 0488 ++ */ /* VL_Os_0488 */
            Os_ICB[i].IsrC2ResourceStack = pIsr2ResStackCfg + (i * vResourceCfgStdMax);
            /* PRQA S 0488 -- */
        }
    }
#endif

    for (i = 0u; i < Os_CfgIsr2Max; i++)
    {
#if ((CFG_SPINLOCK_MAX > 0U) && (CFG_STD_RESOURCE_MAX > 0U))
        pICB                                = &Os_ICB[i];
        pICB->isr2CriticalZoneCount         = 0u;
        pICB->isrCurrentSpinlockOccupyLevel = OS_SPINLOCK_INVALID;
        for (j = 0u; j < CFG_CRITICAL_ZONE_MAX; j++)
        {
            pICB->isr2CriticalZoneStack[j] = OS_OBJECT_INVALID;
            pICB->isr2CriticalZoneType[j]  = OBJECT_MAX;
        }
#endif

#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_ICB[i].CallBackAppID = INVALID_OSAPPLICATION;
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
/* PRQA S 3450 ++ */ /* VL_Os_3450 */
static void Os_DisableAllInterrupts(void)
/* PRQA S 3450 -- */
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
    Os_SaveAllInt = Os_ArchSuspendInt();
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
/* PRQA S 3006, 1532, 1503 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
void DisableAllInterrupts(void)
/* PRQA S 3006, 1532, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_DisableAllInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
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
/* PRQA S 3006, 1532, 1503 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_QAC_NoUsedApi */
void EnableAllInterrupts(void)
/* PRQA S 3006, 1532, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_EnableAllInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
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
void ResumeAllInterrupts(void) /* PRQA S 3006, 1503 */ /* VL_Os_3006, VL_QAC_NoUsedApi */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_ResumeAllInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
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
void SuspendAllInterrupts(void) /* PRQA S 3006, 1503 */ /* VL_Os_3006, VL_QAC_NoUsedApi */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_SuspendAllInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
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
/* PRQA S 3006, 1503 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi */
void ResumeOSInterrupts(void)
/* PRQA S 3006, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_ResumeOSInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
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
/* PRQA S 3006, 1503 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi */
void SuspendOSInterrupts(void)
/* PRQA S 3006, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_SuspendOSInterrupts();

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
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
/* PRQA S 6030, 6070, 3006, 1503, 6010 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCAL, VL_Os_3006, VL_QAC_NoUsedApi, VL_MTR_Os_STCYC */
StatusType EnableInterruptSource(ISRType ISRID, boolean ClearPending)
/* PRQA S 6030, 6070, 3006, 1503, 6010 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err   = E_OK;
    ISRType    isrId = Os_GetObjLocalId(ISRID);
    uint32 vIsrSrc     = Os_IsrCfg[isrId].OsIsrSrc;
    uint32 vIsrSrcType = Os_IsrCfg[isrId].OsIsrSrcType;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432, 2986 ++ */ /* VL_Os_3432, VL_Os_2986 */
    if (CHECK_ID_INVALID(ISRID, Os_CfgIsr2Max_Inf))
    /* PRQA S 3432, 2986 -- */
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[isrId].OsIsrCatType)
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_ENABLED == Os_GetIsrSourceState(vIsrSrc, vIsrSrcType))
    {
        err = E_OS_NOFUNC;
    }
    /*SWS_Os_00809*/
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_ENABLE_INTERRUPT_SOURCE) != TRUE)
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
        /* PRQA S 3138, 3141 ++ */ /* VL_Os_3138, VL_Os_3141 */
        Os_TraceErrorHook(
            OSError_Save_EnableInterruptSource(ISRID, ClearPending),
            OSServiceId_EnableInterruptSource,
            err);
        /* PRQA S 3138, 3141 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

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
/* PRQA S 6030, 3006, 1503, 6070 ++ */ /* VL_MTR_Os_STMIF, VL_Os_3006, VL_QAC_NoUsedApi, VL_MTR_Os_STCAL */
StatusType DisableInterruptSource(ISRType ISRID)
/* PRQA S 6030, 3006, 1503, 6070 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;

    ISRType isrId = Os_GetObjLocalId(ISRID);
    uint32 vIsrSrc     = Os_IsrCfg[isrId].OsIsrSrc;
    uint32 vIsrSrcType = Os_IsrCfg[isrId].OsIsrSrcType;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432, 2986 ++ */ /* VL_Os_3432, VL_Os_2986 */
    if (CHECK_ID_INVALID(ISRID, Os_CfgIsr2Max_Inf))
    /* PRQA S 3432, 2986 -- */
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_CATEGORY2 != Os_IsrCfg[isrId].OsIsrCatType)
    {
        err = E_OS_ID;
    }
    else if (OS_ISR_DISABLED == Os_GetIsrSourceState(vIsrSrc, vIsrSrcType))
    {
        err = E_OS_NOFUNC;
    }
    /*SWS_Os_00809*/
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_DISABLE_INTERRUPT_SOURCE) != TRUE)
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
        /* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_DisableInterruptSource(ISRID), OSServiceId_DisableInterruptSource, err);
        /* PRQA S 3138 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

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
/* PRQA S 3006, 1503 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi */
StatusType ClearPendingInterrupt(ISRType ISRID)
/* PRQA S 3006, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    StatusType err = E_OK;

    ISRType isrId = Os_GetObjLocalId(ISRID);

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432, 2986 ++ */ /* VL_Os_3432, VL_Os_2986 */
    if (CHECK_ID_INVALID(ISRID, Os_CfgIsr2Max_Inf))
    /* PRQA S 3432, 2986 -- */
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
            uint32 vIsrSrc     = Os_IsrCfg[isrId].OsIsrSrc;
            uint32 vIsrSrcType = Os_IsrCfg[isrId].OsIsrSrcType;
            Os_ClearPendingInterrupt(vIsrSrc, vIsrSrcType);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        /* PRQA S 3138 ++ */ /* VL_Os_3138 */
        Os_TraceErrorHook(OSError_Save_ClearPendingInterrupt(ISRID), OSServiceId_ClearPendingInterrupt, err);
        /* PRQA S 3138 -- */
    }
#endif

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

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
void Os_EnableAllInterrupts(void) /* PRQA S 1505 */ /* VL_Os_1505 */
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    StatusType err = E_OK;
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
void Os_ResumeAllInterrupts(void) /* PRQA S 1505 */ /* VL_Os_1505 */
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    StatusType err = E_OK;
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
                Os_InterMonitorEndRecord(OS_ALL_SUSPEND); /* PRQA S 3200 */ /* VL_Os_3200 */
            }
#endif
            Os_ArchEnableAllInt_ButTimingProtInt();
        }
#else
        {
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
            if (TRUE == Os_InterInitFlag)
            {
                Os_InterMonitorEndRecord(OS_ALL_SUSPEND); /* PRQA S 3200 */ /* VL_Os_3200 */
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
void Os_SuspendAllInterrupts(void) /* PRQA S 1505 */ /* VL_Os_1505 */
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
        Os_SaveAllIntNested = Os_ArchSuspendInt();
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
void Os_ResumeOSInterrupts(void) /* PRQA S 1505, 6030 */ /* VL_Os_1505, VL_MTR_Os_STMIF */
{
    if (OS_LEVEL_ISR1 != Os_SCB.sysOsLevel)
    {
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
        StatusType err = E_OK;
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
                    Os_InterMonitorEndRecord(OS_OS_SUSPEND); /* PRQA S 3200 */ /* VL_Os_3200 */
                }
#endif
                Os_ArchSetIpl(Os_SaveOsIntNested, OS_ISR_ENABLE);
            }
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
void Os_SuspendOSInterrupts(void) /* PRQA S 1505 */ /* VL_Os_1505 */
{
    if (OS_LEVEL_ISR1 != Os_SCB.sysOsLevel)
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

            OS_ARCH_SUSPEND_ALLINT();
            Os_SaveOsIntNested = Os_ArchGetIpl();
#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
            if (TRUE == Os_InterInitFlag)
            {
                Os_InterMonitorStartRecord(OS_OS_SUSPEND);
            }
#endif
            Os_ArchSetIpl(Os_cfgIsr2IplMax, OS_ISR_DISABLE);
            OS_ARCH_RESTORE_ALLINT();
        }

        Os_SuspendOsCount++;
    }
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
/* PRQA S 3006, 1503 ++ */ /* VL_Os_3006, VL_QAC_NoUsedApi */
ISRType GetISRID(void)
/* PRQA S 3006, 1503 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_ENTER_KERNEL();
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    ISRType osRet = INVALID_ISR;

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
                /* PRQA S 3120, 4397 ++ */ /* VL_QAC_MagicNum, VL_Os_4397 */
                osRet = Os_SCB.sysRunningIsrCat2Id | (Os_SCB.sysCore << 12u);
                /* PRQA S 3120, 4397 -- */
            }
        }
#endif
    }

    OS_EXIT_KERNEL(); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */

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
void Os_EnterISR2(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    Os_EnterIsrRecordTick(Os_IntCfgIsrId);
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

#if ((TRUE == CFG_STACK_CHECK) && (CFG_ISR2_MAX > 0U))
    Os_StackPtrType StackPtr;

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
        Os_SaveLevel      = Os_SCB.sysOsLevel;
        Os_SCB.sysOsLevel = OS_LEVEL_ISR2;
    }
#else
    Os_SaveLevel      = Os_SCB.sysOsLevel;
    Os_SCB.sysOsLevel = OS_LEVEL_ISR2;
#endif /* TRUE == CFG_INT_NEST_ENABLE */

    Os_IntNestISR2           = Os_IntNestISR2 + 1u;
    Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker + 1u;

    /* Timing protection for isr C2. */
    if (Os_IntCfgIsrId < Os_CfgIsrMax)
    {
        Os_SCB.sysIsrNestQueue[Os_IntNestISR2 - 1U] = Os_IntCfgIsrId;
        Os_SCB.sysRunningIsrCat2Id                  = Os_IntCfgIsrId;
        Os_SCB.sysInIsrCat2                         = TRUE;
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR)
        Os_IncrementIsr2ScheduleCount(Os_IntCfgIsrId);
#endif
/*Writing the running ApplID and Object */
#if (CFG_OSAPPLICATION_MAX > 0U)
        Os_SCB.sysRunningAppID  = Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysRunningIsrCat2Id].hostApp;
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
void Os_ExitISR2(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if ((TRUE == CFG_LOAD_RATIO_CALC_ENABLE)||(TRUE == CFG_TRACE_ENABLE))
    Os_IsrType isrid = Os_SCB.sysRunningIsrCat2Id;
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
            /* PRQA S 1520 ++ */ /* VL_Os_1520 */
            OS_TASK_SWITCH_PROC();
            /* PRQA S 1520 -- */
        }
        else
        {
#if (CFG_OSAPPLICATION_MAX > 0U)
            if (Os_TCB[Os_SCB.sysRunningTaskID].CallBackAppID != INVALID_OSAPPLICATION)
            {
                Os_SCB.sysRunningAppID = Os_TCB[Os_SCB.sysRunningTaskID].CallBackAppID;
            }
            else
            {
                Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
            }
            Os_SCB.sysRunningAppObj = OBJECT_TASK; /* PRQA S 2982 */ /* VL_Os_2982 */
#endif

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
            /* PRQA S 1520 ++ */ /* VL_Os_1520 */
            Os_MemProtTaskCat2Map();
            /* PRQA S 1520 -- */
#endif
        }

#if (CFG_OSAPPLICATION_MAX > 0U)
        if (Os_TCB[Os_SCB.sysRunningTaskID].CallBackAppID != INVALID_OSAPPLICATION)
        {
            Os_SCB.sysRunningAppID = Os_TCB[Os_SCB.sysRunningTaskID].CallBackAppID;
        }
        else
        {
            Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_TASK][Os_SCB.sysRunningTaskID].hostApp;
        }
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
        if (Os_ICB[Os_SCB.sysRunningIsrCat2Id].CallBackAppID != INVALID_OSAPPLICATION)
        {
            Os_SCB.sysRunningAppID = Os_ICB[Os_SCB.sysRunningIsrCat2Id].CallBackAppID;
        }
        else
        {
            Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_ISR][Os_SCB.sysRunningIsrCat2Id].hostApp;
        }
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
    uint32 curTicks = Os_ExitIsrRecordTick(isrid);
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
void Os_EnterISR1(void) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
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
void Os_ExitISR1(void) /* PRQA S 1503 */ /* VL_QAC_NoUsedApi */
{
/* check system stack overflow. */
#if (TRUE == CFG_STACK_CHECK)
    Os_StackPtrType StackPtr = Os_SystemStack->stackBottom;
    Os_StackMonitor(StackPtr);
#endif

#if ((CFG_ISR2_MAX > 0) && (TRUE == CFG_INT_NEST_ENABLE))
    Os_IntCfgIsrId = Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1 - 1u];
#endif
#if (TRUE == CFG_TRACE_ENABLE)
    if ((Os_IntNestISR2 + Os_IntNestISR1) > 1u)
    {
        /* PRQA S 3120 ++ */ /* VL_Os_3120 */
        Os_TraceIsrExit(Os_IntCfgIsrId, Os_SCB.sysIsrNestQueue[Os_IntNestISR2 + Os_IntNestISR1 - 2u]);
        /* PRQA S 3120 -- */
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
Os_IPLType Os_PrioToIpl(Os_PriorityType prio) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
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
Os_PriorityType Os_IplToPrio(Os_IPLType ipl) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    return (Os_PriorityType)(ipl + Os_CfgPriorityMax - 1u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 6530 EOF */ /* VL_MTR_Os_STECT */
