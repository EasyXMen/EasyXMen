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
 ************************************************************************************************************************
 **
 **  @file               : Os_Arch_Processor.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Arch header file for Processor api declarations
 **
 ***********************************************************************************************************************/

#ifndef ARCH_PROCESSOR_H
#define ARCH_PROCESSOR_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Mpu.h"
#include "Os_Arch_Trap.h"
#include "Os_Arch_Define.h"
#include "Os_Arch_Compiler.h"
#include "Os_Mcu_Core.h"
#include "Os_Mcu_Irq.h"
#include "Os_Mcu_Timer.h"
#include "Os_Mcu_Mpu.h"
#include "Os_Mcu_Debug.h"
#include "Os_Kernel2Port.h"

/*=======[M A C R O S]=====================================================*/
#define PSWIO_BIT (10u)
#if (TRUE == CFG_SRV_SHELLOS)
/* Macro definition of CPU mode set,*/
/* Choose user mode for the Tricore platform, User0/1*/
/* Indicates which user mode the CPU uses. */
#define OS_CPUMODE_USER (OS_CPUMODE_USER1)

/* PRQA S 3432 ++ */                            /* VL_Os_3432 */
#define OS_CHANGCPUMODE(mode) /* PRQA S 3458 */ /* VL_Os_3458 */                                    \
    {                                                                                               \
        if ((uint32)OS_CPUMODE_SUPERVISOR == ((OS_ARCH_MFCR(OS_REG_PSW) & (~PSWIO_MASK)) >> PSWIO_BIT))     \
        {                                                                                           \
            if (OS_CPUMODE_USER == (mode))                                                          \
            {                                                                                       \
                OS_ASM("isync");                                                                    \
                OS_ARCH_MTCR(OS_REG_PSW, ((OS_ARCH_MFCR(OS_REG_PSW) & PSWIO_MASK) | PSWIO_USER1_MODE)); \
            }                                                                                       \
            else                                                                                    \
            {                                                                                       \
                OS_ASM("isync");                                                                    \
                OS_ARCH_MTCR(OS_REG_PSW, (OS_ARCH_MFCR(OS_REG_PSW) & PSW_PRS_MASK));                 \
            }                                                                                       \
        }                                                                                           \
        else                                                                                        \
        {                                                                                           \
            if (OS_CPUMODE_SUPERVISOR == (mode))                                                    \
            {                                                                                       \
                OS_SYSCALL(1u);                                                                     \
            }                                                                                       \
        }                                                                                           \
    }

/* Change to the user or supervisor mode */

#define OS_GETCPUMODE()                                                                                   \
    ((OS_CPUMODE_SUPERVISOR == ((OS_ARCH_MFCR(OS_REG_PSW) & (~PSWIO_MASK)) >> PSWIO_BIT)) ? OS_CPUMODE_SUPERVISOR \
                                                                                  : OS_CPUMODE_USER)
#endif /* TRUE == CFG_SRV_SHELLOS */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_HAL_ENTER_KERNEL() OS_CHANGCPUMODE(OS_CPUMODE_SUPERVISOR)
#else
#define OS_HAL_ENTER_KERNEL()
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#define OS_HAL_EXIT_KERNEL()

/* Dispatch*/
#define OS_HAL_TASK_SWITCH_PROC(coreId) Os_SwitchTask(coreId) /* PRQA S 3472 */ /* VL_Os_3472 */
#define Os_Hal_Dispatch()               OS_SYSCALL(0u) /* Generates a system call, this means generating a system trap */

#define OS_HAL_TERMINATE_TASK(taskId) Os_ArchReclaimCsas(taskId)
/* PRQA S 3410 ++ */ /* VL_Os_3410 */
#define OS_HAL_TERMINATE_NESTED_ISR(index) Os_ArchReclaimIsrCsasTermApp(Os_IsrNestPcxStack[index + 1u], Os_IsrNestPcxStack[index])
#define OS_HAL_PROCESS_NESTED_ISR_STACK(index)                       \
do {                                                                 \
    Os_IsrNestPcxStack[index + 1u] = Os_IsrNestPcxStack[index + 2u]; \
} while (0)
/* PRQA S 3410 -- */

/* PRQA S 3458 ++ *//* VL_Os_3458 */
#define Os_Hal_InitCPU()   \
{                                                                                      \
        /* PRQA S 0306 ++ */ /* VL_Os_0306 */                                          \
        Os_ArchInitCsas(Os_Csas, Os_CfgCsaMax);                                        \
        OS_ARCH_MTCR(OS_REG_LCX, OS_EA_TO_PCX((uint32) & Os_Csas[1]));                 \
        OS_ARCH_MTCR(OS_REG_FCX, OS_EA_TO_PCX((uint32) & Os_Csas[Os_CfgCsaMax - 1u])); \
        OS_ARCH_MTCR(OS_REG_PCX, (uint32)0u);                                          \
        /* PRQA S 0306 -- */                                                           \
}
/* PRQA S 3458 -- */

/* Switch to system stack */
extern volatile uint32 Os_ArchSp; /* PRQA S 3451, 3449 */ /* VL_Os_3451 */ /* VL_Os_3449 */
#define Os_Hal_Switch2System() /* PRQA S 3458 */                           /* VL_Os_3458 */ \
    {                                                                                       \
        Os_ArchSp = OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack_Inf[Os_McuGetCoreID()]->StackTop);             \
        OS_ARCH_STORE_SP();                                                                 \
    }

/* The interrupt occupies interrupt resources. */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_ISR2OCCUPYINTRES(isrId) Os_Isr2OccupyIntRes(isrId)
#else /* FALSE == CFG_SERVICE_PROTECTION_ENABLE */
#define OS_ISR2OCCUPYINTRES(isrId)
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
/*=======[E X T E R N A L   D A T A]==========================================*/

/* PRQA S 3432 -- */

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
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
extern void Os_Hal_FirstEnterTask(Os_TaskType taskId);
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
extern void Os_Hal_StartScheduler(void);
/******************************************************************************/
/*
 * Brief                <Initialization of the CSA module of TriCore Architecture>
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
extern void Os_ArchInitCsas(Os_ArchCsaType* csas, uint16 cfgCsaMax);

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
extern uint32 Os_Hal_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal);

#endif /* ARCH_PROCESSOR_H */
/*=======[E N D   O F   F I L E]==============================================*/
