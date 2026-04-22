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

/*=======[I N C L U D E S]===================================================*/

#include "Std_Types.h"
#include "Os_Types.h"
#include "Os_Arch_Processor.h"
#include "Os_Task_CfgData.h"
#include "Os_Sprot.h"
#include "Os_Monitor.h"
/*=======[V E R S I O N  C H E C K]==========================================*/

/*=======[M A C R O S]=======================================================*/
#define M7_XPSR_VALUE 0x01000000U
/*=======[E X T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   D A T A]=========================================*/
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static Os_StackPtrType Os_TaskCBExt[CFG_TASK_MAX];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
/*=======[I N T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]=======*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <First entry task>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_PreTask>
 */
/*****************************************************************************/
void Os_Hal_FirstEnterTask(Os_TaskType taskId)
{
    /* PRQA S 3430,3432,0306 ++ */ /* MISRA Rule 20.7,Rule-11.4 */
    uint32 *Os_ArchTopStkPtr = (uint32*)OS_ARCH_STACK_ALIGN((uint32)(Os_TaskCfg[taskId].TaskStack.StackTop));
    /* PRQA S 3430,3432,0306 -- */ /* MISRA Rule 20.7,Rule-11.4 */

    /*registers stacked as if auto-saved on exception.
     * The follow registers will automatic restore when exiting an exception.
     * --xPSR, PC, LR, R12, R3, R2, R1, R0--*/
    /* PRQA S 3387,3440,0489,3120 ++ */    /* MISRA Rule 13.3,Rule-18.4,CWE-398 */
    *(--Os_ArchTopStkPtr) = M7_XPSR_VALUE; /* xPSR */
    /* PRQA S 0305 ++*/                    /* MISRA Rule 11.1 */
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskCfg[taskId].TaskEntry;
    *(--Os_ArchTopStkPtr) = (uint32)Os_TaskErrBack; /* PRQA S 0428 */ /* MISRA Rule 11.1 */
    /* PRQA S 0305 --*/                                               /* MISRA Rule 11.1 */
    *(--Os_ArchTopStkPtr) = 12u;                                      /* R12 */
    *(--Os_ArchTopStkPtr) = 3u;                                       /* R3 */
    *(--Os_ArchTopStkPtr) = 2u;                                       /* R2 */
    *(--Os_ArchTopStkPtr) = 1u;                                       /* R1 */
    *(--Os_ArchTopStkPtr) = 0u;                                       /* R0 */

    *(--Os_ArchTopStkPtr) = 0xFFFFFFFDu; /* R14: EXEC_RETURN */
    /*remaining registers saved on process stack*/
    *(--Os_ArchTopStkPtr) = 11U; /* R11 */
    *(--Os_ArchTopStkPtr) = 10U; /* R10 */
    *(--Os_ArchTopStkPtr) = 9U;  /* R9 */
    *(--Os_ArchTopStkPtr) = 8U;  /* R8 */
    *(--Os_ArchTopStkPtr) = 7U;  /* R7 */
    *(--Os_ArchTopStkPtr) = 6U;  /* R6 */
    *(--Os_ArchTopStkPtr) = 5U;  /* R5 */
    *(--Os_ArchTopStkPtr) = 4U;  /* R4 */
    /*CONTROL register*/
    Os_ArchTopStkPtr = Os_ArchTopStkPtr - 1;
    /* PRQA S 3387,3440,0489,3120 -- */ /* MISRA Rule 13.3,Rule-18.4,CWE-398 */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    /* PRQA S 2109 ++ */ /* MISRA CWE-192 */
    if ((TRUE == Os_AppCfg[Os_Kernel_GetSysRunningAppID()].Trusted)
        && (TRUE != Os_AppCfg[Os_Kernel_GetSysRunningAppID()].TrustedAppWithProtection))
    {
        *Os_ArchTopStkPtr = 0x00000000u;
    }
    else
    {
        *Os_ArchTopStkPtr = 0x00000001u;
    }
    /* PRQA S 2109 -- */ /* MISRA CWE-192 */
#else
    *Os_ArchTopStkPtr = 0x00000000u;
#endif
    Os_TaskCBExt[taskId] = (Os_StackPtrType)Os_ArchTopStkPtr;
    /* PRQA S 0303 ++ */ /* MISRA Rule-11.4 */
    OS_INTERRUPT_SYS_SET_PRIO(OS_PendSV_IRQn, OS_NVIC_CONVERT_SET_PRIO(OS_NVIC_PRIO_MIN));
    /* PRQA S 0303 -- */ /* MISRA Rule-11.4 */
}

/*****************************************************************************/
/*
 * Brief                <Os_Hal_Dispatch>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void Os_Hal_Dispatch(void)
{
    /* Call PendSV to switch task */
    uint32 NVIC_PENDSVSET = 0x10000000;
    Os_Hal_DisableInt();
    OS_SCB_ICSE_BASE |= NVIC_PENDSVSET;
    Os_Hal_EnableInt();
}

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/*****************************************************************************/
/*
 * Brief                <Os_ArchSyscall>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void Os_ArchSyscall(void)
{
    OS_ASM(" svc 0                                \n"
           " nop                                  \n");
}
#endif

/*****************************************************************************/
/*
 * Brief                <The First Task Scheduling>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void Os_ArchFirstDispatch(void)
{
    /* Call PendSV to switch task and switch to system stack*/
    Os_Hal_DisableInt();
    Os_Hal_Switch2System();
    uint32 NVIC_PENDSVSET = 0x10000000;
    OS_SCB_ICSE_BASE |= NVIC_PENDSVSET;
    Os_Hal_EnableInt();
}

/*****************************************************************************/
/*
 * Brief                <Os_Hal_StartScheduler>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <ShutdownOS>
 */
/*****************************************************************************/
void Os_Hal_StartScheduler(void)
{
    /*Run the scheduler*/
    Os_ArchFirstDispatch();   /* Switch stack and System call */
    while (1) /*PRQA S 2740*/ /* MISRA CWE-569,CWE-571 */
    {
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"


#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <Os_InitSysTimer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/*****************************************************************************/
void Os_ArchInitSystemTimer(CoreIdType CoreId)
{
    switch (CoreId)
    {
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE_CORE0)
    case OS_CORE_ID_0:
        /* Core_0 System Timer Initialize.Value = 48000000u*T - 1u (T--Second) */
        /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */
        OS_SYSTICK_CTRL_REG = 0x0u;
        OS_SYSTICK_COUNTER_REG = 0x0u;
        OS_SYSTICK_RELOAD_REG = CFG_REG_OSTIMER_VALUE_CORE0 - 0x1u;
        OS_SYSTICK_CTRL_REG = (OS_SYSTICK_CLK_BIT | OS_SYSTICK_INT_BIT| OS_SYSTICK_ENABLE_BIT);
        /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */

#if (TRUE == CFG_OS_MONITOR_ENABLE)
        Os_TimerStamp.softCounter = 0U;
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */
        break;
#endif

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE_CORE1)
    case OS_CORE_ID_1:
        /* Core_1 System Timer Initialize.Value = 48000000u*T - 1u (T--Second) */
        /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */
        OS_SYSTICK_CTRL_REG = 0x0u;
        OS_SYSTICK_COUNTER_REG = 0x0u;
        OS_SYSTICK_RELOAD_REG = CFG_REG_OSTIMER_VALUE_CORE1 - 0x1u;
        OS_SYSTICK_CTRL_REG = (OS_SYSTICK_CLK_BIT | OS_SYSTICK_INT_BIT| OS_SYSTICK_ENABLE_BIT);
        /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */
        break;
#endif

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE_CORE2)
    case OS_CORE_ID_2:
        /* Core_2 System Timer Initialize.Value = 48000000u*T - 1u (T--Second) */
        /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */
        OS_SYSTICK_CTRL_REG = 0x0u;
        OS_SYSTICK_COUNTER_REG = 0x0u;
        OS_SYSTICK_RELOAD_REG = CFG_REG_OSTIMER_VALUE_CORE2 - 0x1u;
        OS_SYSTICK_CTRL_REG = (OS_SYSTICK_CLK_BIT | OS_SYSTICK_INT_BIT| OS_SYSTICK_ENABLE_BIT);
        /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */
        break;
#endif

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE_CORE3)
    case OS_CORE_ID_3:
        /* Core_3 System Timer Initialize.Value = 48000000u*T - 1u (T--Second) */
        /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */
        OS_SYSTICK_CTRL_REG = 0x0u;
        OS_SYSTICK_COUNTER_REG = 0x0u;
        OS_SYSTICK_RELOAD_REG = CFG_REG_OSTIMER_VALUE_CORE3 - 0x1u;
        OS_SYSTICK_CTRL_REG = (OS_SYSTICK_CLK_BIT | OS_SYSTICK_INT_BIT| OS_SYSTICK_ENABLE_BIT);
        /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */
        break;
#endif
    /*add comments to pass QAC.*/
    default:
        /* Nothing to do. */
        break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_ArchClearSysTimer(Os_CoreIdType coreId)
{
    (void)coreId;
    OS_SYSTICK_CTRL_REG;

#if (TRUE == CFG_OS_MONITOR_ENABLE)/* PRQA S 3332 */ /* VL_Os_3332 */
    /* PRQA S 3387, 3442 ++ */ /* VL_Os_3387, VL_Os_3442 */
    ++Os_TimerStamp.softCounter;
    /* PRQA S 3387, 3442 -- */
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* if (TRUE == CFG_SYSTEM_TIMER_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get current time in the OS.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_GetCurrentTime>
 * REQ ID               <None>
 */
/******************************************************************************/
Os_TimerTickType Os_Hal_GetTimeTicks(void)
{
    /* Get the elapsed ticks of the current hardware timer */
    Os_TickType elapsedTicks = OS_TIMER_GET_ELAPSED_TICKS();

    /* Get the global time (ms) and calculate the current timestamp(ns). */
    Os_TimerTickType totalNs = Os_TimerStamp.softCounter * OS_NS_PER_MS;
    totalNs += OS_TIMER_TICKS_TO_NS(elapsedTicks);

    return totalNs;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"

#if defined(OS_ENABLE_FPU)
/*****************************************************************************/
/*
 * Brief                <Floating Point Unit initialization>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 */
/*****************************************************************************/
void Os_ArchFPU_Init(void)
{
    /* Enable CP10 and CP11 coprocessors */
    OS_REG_CPACR |= (OS_CPACR_CP10_MASK | OS_CPACR_CP11_MASK);

    /*Enable CONTROL.FPCA setting on execution of a floating-point instruction.*/
    OS_REG_FPCCR |= (1U << 31U);
    OS_REG_FPCCR |= (1U << 30U);
}
#endif

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the CONTROL Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <None>
 * Return               <uin32,the CONTOL Register vale>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchGetThreadMode>
 */
/******************************************************************************/
uint32 Os_ArchGetControlVale(void) /* PRQA S 3008 */ /* MISRA CWE-498 */
{
    register uint32 result; /* PRQA S 2011 */ /* MISRA CWE-398 */

    OS_ASM("MRS %0, control" : "=r"(result));

    return result;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the Thread Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-Name[in]       <None>
 * Param-Name[in/out]   <None>
 * Param-Name[out]      <None>
 * Return               <Os_ThreadModeType>
 * PreCondition         <None>
 * CallByAPI            <OS_CHANGCPUMODE>
 * REQ ID               <None>
 */
/******************************************************************************/
Os_ThreadModeType Os_ArchGetThreadMode(void)
{
    Os_ThreadModeType coreMode = OS_THREAD_MODE_SUPERVISOR;
    uint32 tpMode = (Os_ArchGetControlVale() & 0x00000001U);

    coreMode = (tpMode == 0U) ? OS_THREAD_MODE_SUPERVISOR : OS_THREAD_MODE_USER;

    return coreMode;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the CPU Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-core[in]       <Selects the core which mode is set>
 * Return               <Os_CpuModeType,the CPU mode>
 * PreCondition         <None>
 * CallByAPI            <ControlIdle>
 */
/******************************************************************************/
void Os_ArchSetCpuMode(Os_CpuModeType mode) /* PRQA S 3006 */ /* MISRA CWE-498 */
{
    if (OS_CPUMODE_SUPERVISOR == mode)
    {
        OS_ASM("mrs r0,control               \n"
               "and r0,r0,#0xFFFFFFFE        \n"
               "msr control, r0              \n");
    }
    else
    {
        OS_ASM("mrs r0,control               \n"
               "orr r0,r0,#0x01              \n"
               "msr control, r0              \n");
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CODE
#include "Os_MemMap.h"
Os_StackPtrType Os_ArchGetRunningTaskStack(void)
{
    return Os_TaskCBExt[Os_Kernel_GetSysRunningTaskID()];
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_ArchSetRunningTaskStack(Os_StackPtrType stackTop)
{
    Os_TaskCBExt[Os_Kernel_GetSysRunningTaskID()] = stackTop;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
uint8 Os_ArchIsFirstDispatch(void)
{
    return (INVALID_TASK == Os_Kernel_GetSysRunningTaskID());
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
