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
#ifndef ARCH_CM7_PROCESSOR_H
#define ARCH_CM7_PROCESSOR_H

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define OS_PROCESSOR_H_VENDOR_ID        62
#define OS_PROCESSOR_H_MODULE_ID        1
#define OS_PROCESSOR_H_AR_MAJOR_VERSION 4
#define OS_PROCESSOR_H_AR_MINOR_VERSION 2
#define OS_PROCESSOR_H_AR_PATCH_VERSION 2
#define OS_PROCESSOR_H_SW_MAJOR_VERSION 1
#define OS_PROCESSOR_H_SW_MINOR_VERSION 0
#define OS_PROCESSOR_H_SW_PATCH_VERSION 0
#define OS_PROCESSOR_H_VENDOR_API_INFIX 0

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Os_Types.h"
#include "Os.h"
#include "Arch_Extend.h"
#include "Arch_Irq.h"
#include "Arch_Mpu.h"
#include "Mcu_Core.h"
#include "Mcu_Timer.h"
#include "Mcu_Irq.h"
#include "Os_MprotCfg.h"
#ifdef COMPILER_GCC
#include "Os_Compiler_gcc.h"
#elif defined(COMPLIER_GHS)
#include "Os_Compiler_ghs.h"
#endif
/*=======[M A C R O S]========================================================*/
#define OS_REG32(address) (*(volatile uint32*)(address)) /* 32-bit register */ /* PRQA S 3472 */ /* MISRA Dir-4.9 */
#define OS_REG16(address) (*(volatile uint16*)(address))                                         /* 16-bit register */
#define OS_REG8(address)  (*(volatile uint8*)(address))                                          /* 8-bit register  */

/*----------------------------Cortex-CM7 Register-----------------------------*/
/*
 * Floating Point Unit
 * */
/*Floating-point Context Control Register*/
#define OS_REG_FPCCR OS_REG32(0xE000EF34UL)
/*Coprocessor Access Control Register*/
#define OS_REG_CPACR               OS_REG32(0xE000ED88UL)
#define OS_CPACR_CP10_MASK         (0x00300000UL)
#define OS_CPACR_CP11_MASK         (0x00C00000UL)
#define OS_MODE_SUPERVISOR_ENTRY() OS_ENTER_KERNEL()
#define OS_MODE_SUPERVISOR_EXIT()  OS_EXIT_KERNEL()
#define CONTROL_REGISTER           (0xE000ED14U)
/*
 * System timer,SysTick
 * */
/*SysTick Control and Status Register*/
#define OS_SYSTICK_BASE_ADDRESS (0xE000E010U)
#define OS_SYSTICK_CTRL_REG     OS_REG32(OS_SYSTICK_BASE_ADDRESS)
#define OS_SYSTICK_RELOAD_REG   OS_REG32(OS_SYSTICK_BASE_ADDRESS + 0x4u)
#define OS_SYSTICK_COUNTER_REG  OS_REG32(OS_SYSTICK_BASE_ADDRESS + 0x8u)
#define OS_SYSTICK_CHECK_REG    OS_REG32(OS_SYSTICK_BASE_ADDRESS + 0xCu)
#define OS_SYSTICK_ENABLE_BIT   ((uint32)(1u) << 0u)
#define OS_SYSTICK_INT_BIT      ((uint32)(1u) << 1u)
#define OS_SYSTICK_CLK_BIT      ((uint32)(1u) << 2u)

/*--------------------------------Porting Macro-------------------------------*/
/* Common macros */
#define OS_ASM                    __asm volatile

#define OS_ARCH_STACK_ALIGN(addr) ((addr) & 0xFFFFFFF8u) /* PRQA S 3472 */ /* MISRA Dir-4.9 */

#define OS_MSR_PRIMASK_BIT0       (0x00000001U)

/* Switch msp to system stack*/
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define Os_ArchSwitch2System()                                                        \
    {                                                                                 \
        do                                                                            \
        {                                                                             \
            Os_ArchTempSysSp = OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack->stackTop); \
            __asm__ volatile("msr msp, %[sysSP]" ::[sysSP] "r"(Os_ArchTempSysSp));    \
        } while (0);                                                                  \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
/* Msync/Isynch core instruction macros */
#define OS_ARCH_ISYNC() OS_ASM("isb")
#define OS_ARCH_DSYNC() OS_ASM("dsb")

#define OS_START_DISPATCH()

#if (TRUE == CFG_SRV_SHELLOS)
/* PRQA S 3412++ */ /* MISRA PRE10 */
#define OS_ENTER_KERNEL()                           \
    Os_ThreadModeType cpuMode = Os_GetThreadMode(); \
    OS_CHANGCPUMODE();
/* PRQA S 3412++ */                                           /* MISRA PRE10 */
#define OS_EXIT_KERNEL() OS_RESUMECPUMODE() /* PRQA S 3472 */ /* MISRA Dir-4.9 */
#else
#define OS_ENTER_KERNEL()
#define OS_EXIT_KERNEL()
#endif

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
/* Get/Set special register---control*/
#define Os_ArchGetControl()                   \
    OS_ASM("push {r0,r1}                  \n" \
           "ldr r1, =Os_ArchControl       \n" \
           "mrs r0, control               \n" \
           "str r0, [r1]                  \n" \
           "pop {r0,r1}                   \n")
#define Os_ArchSetControl()                   \
    OS_ASM("ldr r0, =Os_ArchControl       \n" \
           "ldr r0, [r0]                  \n" \
           "msr control, r0               \n")
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_CHANGCPUMODE()                         \
    {                                             \
        if (OS_THREAD_MODE_SUPERVISOR == cpuMode) \
        {                                         \
            /*nothing to do*/                     \
        }                                         \
        else                                      \
        {                                         \
            Os_ArchSyscall();                     \
        }                                         \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_RESUMECPUMODE()                      \
    {                                           \
        if (OS_THREAD_MODE_USER == cpuMode)     \
        {                                       \
            Os_SetCpuMode(OS_THREAD_MODE_USER); \
        }                                       \
        else                                    \
        {                                       \
            /* nothing to do. */                \
        }                                       \
    }
#endif
/* PRQA S 3458-- */ /* MISRA PRE10 */
#define Os_ArchPreSwitch2System()

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/* the mode of thread. */
typedef enum
{
    OS_THREAD_MODE_SUPERVISOR = 0U, /*PRQA S 1271*/ /* MISRA CWE-682 */
    OS_THREAD_MODE_USER = 1U /*PRQA S 1271*/        /* MISRA CWE-682 */
} Os_ThreadModeType;

/*=======[E X T E R N A L   D A T A]==========================================*/
extern VAR(volatile uint32, OS_VAR) Os_ArchControl;

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern FUNC(uint32, OS_CODE) Os_CmpSwapW(uint32* address, uint32 compareVal, uint32 exchangedVal);
extern FUNC(void, OS_CODE) Os_ArchInitIntPrio(void);
extern FUNC(void, OS_CODE) Os_ArchFirstEnterTask(void);
extern FUNC(void, OS_CODE) Os_ArchStartScheduler(void);
extern FUNC(void, OS_CODE) Os_ArchDispatch(void);
extern FUNC(void, OS_CODE) Os_ArchDispatch_ISR(void);
extern FUNC(uint32, OS_CODE) Os_GetControlVale(void);
extern FUNC(void, OS_CODE) Os_SetCpuMode(Os_CPUModeType mode);
extern FUNC(Os_ThreadModeType, OS_CODE) Os_GetThreadMode(void);
#if defined(ENABLE_FPU)
extern FUNC(void, OS_CODE) FPU_Init(void);
#endif

/* Shell protection */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
extern FUNC(void, OS_CODE) Os_ArchSyscall(void);
extern FUNC(void, OS_CODE) Os_ArchAppTerminateTaskProc(Os_TaskType OsTaskID);
#endif

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
extern FUNC(void, OS_CODE) Os_InitSysTimer(void);
#endif

/*============[STACK CHECK]=====================*/
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_FILL_SAFET_STACK()                                 \
    {                                                         \
        *ptr = OS_STACK_FILL_PATTERN;                         \
        *(ptr + (Os_StackDataType)1) = OS_STACK_FILL_PATTERN; \
        *(ptr + (Os_StackDataType)2) = OS_STACK_FILL_PATTERN; \
        *(ptr + (Os_StackDataType)3) = OS_STACK_FILL_PATTERN; \
    }
/* PRQA S 3458-- */                                        /* MISRA PRE10 */
#endif /* #ifdef ARCH_CM7_PROCESSOR_H */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */
/*=======[E N D   O F   F I L E]==============================================*/
