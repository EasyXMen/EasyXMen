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
 **  FILENAME    : Arch_Processor.c                                            **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Processor Request Managment                                 **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
#include <v800_ghs.h>
/*=======[M A C R O S]========================================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/*Save the top of the stack for now.*/
VAR(volatile uint32, OS_VAR) Os_ArchTempSp;
/*Save sp.*/
VAR(volatile uint32, OS_VAR) Os_ArchMasterSp;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
/*A pointer to the TCB extension.*/
P2VAR(Os_TaskCBExtType, AUTOMATIC, OS_VAR) Os_TaskCBExt;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static VAR(volatile uint32, OS_VAR) Os_IsrPriority; /* PRQA S 3218 */ /* MISRA Rule 8.9 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/* PRQA S 0791 -- */ /* MISRA Rule 5.4 */
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialization of the CPU in the OS.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <void>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchInitCPU(void)
{
    /* Enables write access of protected registers */
    OS_MSRKCPROT = OS_KCPORT_ENABLE;

    /* The clocks of OSTM0 - OSTM9 Setting are supplied */
    OS_MSR_OSTM = 0x00;
    ASM("SYNCM");

/*Initialize system timer for system counter */
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    Os_ArchInitSystemTimer();
#endif

/*Initialize system timer for time protection */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_ArchInitTimingProtTimer();
#endif
}

/******************************************************************************/
/*
 * Brief                <first entry task>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <void>
 * PreCondition         <None>
 * CallByAPI            <Os_PreTask>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchFirstEnterTask(void)
{
    Os_ArchDisableInt();
    P2VAR(uint32, AUTOMATIC, OS_VAR)
    Os_ArchTopStkPtr = (uint32*)(OS_ARCH_STACK_ALIGN((uint32)Os_SCB.sysRunningTCB->taskTop));
    /* PRQA S 0306, 3432 */ /* MISRA Rule 11.4 20.7 */

    /* R1, R2, R6-R31 */
    /*Build the system register stack and restore to the register when entering the task*/
    /* PRQA S 3440, 0305++ */                                                        /* MISRA Rule 13.3 11.1 */
    Os_ArchTopStkPtr = Os_ArchTopStkPtr - 27U; /* PRQA S 0488 */                     /* MISRA Rule 18.4 */
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskErrBack;                                  /*LP*/
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskCfg[Os_SCB.sysRunningTaskID].osTaskEntry; /*PC*/

    /* PSW */
    Os_ArchTopStkPtr = Os_ArchTopStkPtr - 1U;
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    if ((TRUE == Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted)
        && (TRUE != Os_AppCfg[Os_SCB.sysRunningAppID].OsTrustedAppWithProtection))
    {
        *Os_ArchTopStkPtr = 0x00018000U; /* UM=0, CU2-CU0 = 001, EBV=1 */
    }
    else
    {
        *Os_ArchTopStkPtr = 0x40018000U; /* UM=1, CU2-CU0 = 001, EBV=1 */
    }
#else
    *Os_ArchTopStkPtr = 0x00018000U; /* CU2-CU0 = 001, EBV=1 */
#endif

#if (FALSE == CFG_FPU_ENABLE)
    *Os_ArchTopStkPtr &= ~((uint32)0x07U << 16U); /* CU2-CU0 = 000 */
#else
    *(--Os_ArchTopStkPtr) = 0x00020000U; /* fpsr  */
    *(--Os_ArchTopStkPtr) = 0x00000000U; /* fpepc */
    *(--Os_ArchTopStkPtr) = 0x00000000U; /* fpst  */
    *(--Os_ArchTopStkPtr) = 0x00000000U; /* fpcc  */
    *(--Os_ArchTopStkPtr) = 0x00000000U; /* fpcfg */
    *(--Os_ArchTopStkPtr) = 0x00000000U; /* fpec  */
#endif
    /* PRQA S 3440, 0305-- */ /* MISRA Rule 13.3 11.1 */

    /*save the new top-of-stack pointer*/
    Os_TaskCBExt[Os_SCB.sysRunningTaskID] = (uint32)Os_ArchTopStkPtr; /* PRQA S 0306 */ /* MISRA Rule 11.4 */
}

/******************************************************************************/
/*
 * Brief                <The OS starts scheduling.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <void>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchStartScheduler(void)
{
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
    /*notes: the code of start system timer must be lightweight, otherwise the
     * previous step 'synchronization' will not make sense*/
    OS_ARCH_ENABLE_SYSTIMER(Os_SCB.sysCore); /* PRQA S 0303 */ /* MISRA Rule 11.4 */
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    OS_ARCH_ENABLE_TPTIMER(Os_SCB.sysCore);
#endif

    Os_ArchFirstDispatch();
}
/*****************************************************************************/
/*
 * Brief                <OS_GetSystemStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) OS_GetSystemStackPointer(void)
{
    Os_ArchTempSp = OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack->stackTop); /* PRQA S 0306 */ /* MISRA Rule 11.4 */
}

/*****************************************************************************/
/*
 * Brief                <OS_GetTaskStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) OS_GetTaskStackPointer(void)
{
    Os_ArchTempSp = Os_TaskCBExt[Os_SCB.sysRunningTaskID];
}

/*****************************************************************************/
/*
 * Brief                <OS_SaveTaskStackPointer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Return               <>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
FUNC(void, OS_CODE) OS_SaveTaskStackPointer(void)
{
    Os_TaskCBExt[Os_SCB.sysRunningTaskID] = Os_ArchTempSp;
}
/******************************************************************************/
/*
 * Brief                  <OS_ShutdownAllCores is not provide user>
 *
 * Service ID             <None>
 * Sync/Async             <Synchronous>
 * Reentrancy             <Reentrant>
 * param-address[in]      <Pointer to the address>
 * param-compareVal[in]   <The value to compare with the value at the address>
 * param-exchangedVal[in] <The value to replace the value at the address>
 * param-Name[out]        <None>
 * param-Name[in/out]     <None>
 * return                 <None>
 * CallByAPI              <GetSpinlock and so on>
 * REQ ID                 <None>
 */
/******************************************************************************/
uint32 Os_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal)
{
    uint32 ret;
    ret = __CAXI2((int*)address, compareVal, exchangedVal);

    return !ret;
}

/* OS447: Disable all interrupts in os_app during TerminateApplication. */
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/******************************************************************************/
/*
 * Brief                <Terminate the task objects in TerminateApplication.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_TerminateTaskObjectHandler>
 * REQ ID               <DD_1_1078>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchAppTerminateTaskProc(Os_TaskType OsTaskID)
{
    if ((OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        || ((OS_LEVEL_ERRORHOOK_APP == Os_SCB.sysOsLevel) && (TRUE != Os_SCB.sysInIsrCat2)))
    {
        /* If TerminateApplication is called by task or isr2 or app_error_hook,
         * the csa of running task is reclaimed in the dispatcher at the end of TerminateApplication. */
        if (OsTaskID != Os_SCB.sysRunningTaskID)
        {
            /* nothing to do. */
        }
    }
    else
    {
        /* nothing to do. */
    }

    return;
}
#endif /* #if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC)) */

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 6012 ++ */
/*=======[E N D   O F   F I L E]==============================================*/
