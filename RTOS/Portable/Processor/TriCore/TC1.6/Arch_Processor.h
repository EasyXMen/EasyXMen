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
**  FILENAME    : Arch_Processor.h                                            **
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

#ifndef ARCH_PROCESSOR_H
#define ARCH_PROCESSOR_H

/*=======[I N C L U D E S]====================================================*/
#include "Mcu_Core.h"
#include "Mcu_Irq.h"
#include "Arch_Irq.h"
#include "Mcu_Timer.h"
#include "Mcu_Mpu.h"

/*=======[M A C R O S]=====================================================*/
/*================@+General+@================*/
#if (TRUE == CFG_SRV_SHELLOS)
#define OS_CPUMODE_USER (OS_CPUMODE_USER1)
#define OS_CHANGCPUMODE(mode)                                                                       \
    {                                                                                               \
        if ((uint32)OS_CPUMODE_SUPERVISOR == ((OS_ARCH_MFCR(OS_REG_PSW) & 0x00000c00u) >> 10u))     \
        {                                                                                           \
            if (OS_CPUMODE_USER == (mode))                                                          \
            {                                                                                       \
                OS_ASM("isync");                                                                    \
                OS_ARCH_MTCR(OS_REG_PSW, ((OS_ARCH_MFCR(OS_REG_PSW) & 0xfffff3ffu) | 0x00000400u)); \
            }                                                                                       \
            else                                                                                    \
            {                                                                                       \
                OS_ASM("isync");                                                                    \
                OS_ARCH_MTCR(OS_REG_PSW, (OS_ARCH_MFCR(OS_REG_PSW) & 0xffff4fffu));                 \
            }                                                                                       \
        }                                                                                           \
        else                                                                                        \
        {                                                                                           \
            if (OS_CPUMODE_SUPERVISOR == (mode))                                                    \
            {                                                                                       \
                (void)_syscall(1u);                                                                 \
            }                                                                                       \
        }                                                                                           \
    }

#define OS_GETCPUMODE()                                                                                   \
    ((OS_CPUMODE_SUPERVISOR == ((OS_ARCH_MFCR(OS_REG_PSW) & 0x00000c00u) >> 10u)) ? OS_CPUMODE_SUPERVISOR \
                                                                                  : OS_CPUMODE_USER)
#endif /* TRUE == CFG_SRV_SHELLOS */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_ENTER_KERNEL() /* PRQA S 3219 */ /* MISRA Rule 2.1 */ /*ARCH_PROCESSOR_STATICDEFINE_001*/ \
    {                                                                                                \
        /* PRQA S 2741,2742 ++*/ /* MISRA Rule 14.3*/ /*ARCH_PROCESSOR_MACROWITHPARA_002*/           \
        OS_CHANGCPUMODE(OS_CPUMODE_SUPERVISOR);                                                      \
        /* PRQA S 2741,2742 --*/ /* MISRA Rule 14.3*/                                                \
    }
#else
#define OS_ENTER_KERNEL()
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#define OS_EXIT_KERNEL()

#define OS_MODE_SUPERVISOR_ENTRY() OS_ENTER_KERNEL()
#define OS_MODE_SUPERVISOR_EXIT()  OS_EXIT_KERNEL()
/*================@-General-@================*/

/*================@+Rpc+@================*/
#define RPC_TIMEOUT (100U)
#define TIMER_FRE   (100U)
/* stimate the number of instructions that will run */
#define RPC_INS_NUM   (5U)
#define RPC_WAIT_TIME (uint64)((RPC_TIMEOUT * TIMER_FRE * 1000U / RPC_INS_NUM))
#define E_BUSY        0x02u
/*================@-Rpc-@================*/

/*================@+Task Switch+@================*/
#define OS_TASK_SWITCH_PROC() Os_SwitchTask() /* PRQA S 3472 */ /* MISRA  Dir-4.9 */ /* ARCH_DEFINE_MACRO_001 */
#define Os_ArchDispatch()     (void)_syscall(0);
#define OS_START_DISPATCH()
/*================@-Task Switch-@================*/

/*================@+Operation About Stack+@================*/

#define OS_FILL_SAFET_STACK() /* PRQA S 3472 */ /* MISRA  Dir-4.9 */ /* ARCH_DEFINE_MACRO_001 */ \
    {                                                                                            \
        *ptr = OS_STACK_FILL_PATTERN;                                                            \
        *(ptr + (Os_StackDataType)1) = OS_STACK_FILL_PATTERN;                                    \
        *(ptr + (Os_StackDataType)2) = OS_STACK_FILL_PATTERN;                                    \
        *(ptr + (Os_StackDataType)3) = OS_STACK_FILL_PATTERN;                                    \
    }

#define osGetApiStackUsagePrologue()                                               \
    P2VAR(Os_StackDataType, AUTOMATIC, OS_VAR)                                     \
    ptr;                                                                           \
    P2VAR(Os_StackDataType, AUTOMATIC, OS_VAR)                                     \
    CurrentSp;                                                                     \
    P2VAR(Os_StackDataType, AUTOMATIC, OS_VAR)                                     \
    curTaskStackBottom = Os_SCB.sysRunningTCB->taskStackBottom;                    \
    Os_GetCurrentSp();                                                             \
    for (ptr = (Os_StackDataType*)CurrentSp; ptr > curTaskStackBottom - 1u; ptr--) \
    {                                                                              \
        *ptr = OS_STACK_FILL_PATTERN;                                              \
    }

#define osGetApiStackUsageEpilogue(Usage)                  \
    for (ptr = curTaskStackBottom; ptr < CurrentSp; ptr++) \
    {                                                      \
        if (OS_STACK_FILL_PATTERN != (*ptr))               \
        {                                                  \
            break;                                         \
        }                                                  \
    }                                                      \
    Usage = (uint32)(CurrentSp - ptr);                     \
    /*================@-Operation About Stack-@================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
/******************************************************************************/
/*
 * Brief                <Terminate the ISR objects of the calling application>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <OsIsrID>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_TerminateISRObjectHandler>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchAppTerminateIsrProc(Os_IsrType OsIsrID);
#endif /* CFG_ISR_MAX > 0 */

/******************************************************************************/
/*
 * Brief                <TerminateTask arch_process in TerminateApplication>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_TerminateTaskObjectHandler>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchAppTerminateTaskProc(Os_TaskType OsTaskID);
#endif /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */

/********************************************************************/
/*
 * Brief                <Get max usage of system csa>
 * Service ID           <none>
 * Sync/Async           <none>
 * Reentrancy           <Yes>
 * param-eventId[in]    <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <osCsaUsageType>
 * PreCondition         <None>
 * REQ ID               <None>
 */
/********************************************************************/
extern FUNC(osCsaUsageType, OS_CODE) OSGetSystemCsaUsage(void);

/******************************************************************************/
/*
 * Brief                <first entry task>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SwitchTask>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchFirstEnterTask(void);

/******************************************************************************/
/*
 * Brief                <Start scheduling to execute the first task.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchStartScheduler(void);

/******************************************************************************/
/*
 * Brief                <OS_ShutdownAllCores is not provide user>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <GetSpinlock and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern uint32 Os_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal);

/******************************************************************************/
/*
 * Brief                <Change csa PSW.IO bits to User-1 Mode>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchFirstEnterTask>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ModeModify(void);

/******************************************************************************/
/*
 * Brief                <Initialization of the CPU in the OS.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchInitCPU(void);

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#endif /* ARCH_PROCESSOR_H */
/*=======[E N D   O F   F I L E]==============================================*/
