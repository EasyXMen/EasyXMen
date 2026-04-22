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
**  FILENAME    : Os_Arch_Processor.h                                            **
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
#define OS_PROCESSOR_H_VENDOR_ID 62
#define OS_PROCESSOR_H_MODULE_ID 1
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
#include "Os_Kernel2Port.h"
#include "Os_Arch_Extend.h"
#include "Os_Arch_Irq.h"
#include "Os_Arch_Mpu.h"
#include "Os_Mcu_Core.h"
#include "Os_Mcu_Timer.h"
#include "Os_Mcu_Irq.h"
#include "Os_Mprot_Cfg.h"
#include "Os_Mprot_CfgData.h"
#include "Os_Appl_CfgData.h"
#include "Os_Compiler.h"

/*=======[M A C R O S]========================================================*/
#define OS_REG32(address) (*(volatile uint32*)(address)) /* 32-bit register */ /* PRQA S 3472 */ /* MISRA Dir-4.9 */
#define OS_REG16(address) (*(volatile uint16*)(address))                                        /* 16-bit register */
#define OS_REG8(address)  (*(volatile uint8*)(address))                                          /* 8-bit register  */

/*----------------------------Cortex-CM7 Register-----------------------------*/
/*
 * Floating Point Unit
 * */
/*Floating-point Context Control Register*/
#define OS_REG_FPCCR               OS_REG32(0xE000EF34UL)
/*Coprocessor Access Control Register*/
#define OS_REG_CPACR               OS_REG32(0xE000ED88UL)
#define OS_CPACR_CP10_MASK         (0x00300000UL)
#define OS_CPACR_CP11_MASK         (0x00C00000UL)
#define OS_MODE_SUPERVISOR_ENTRY() OS_HAL_ENTER_KERNEL()
#define OS_MODE_SUPERVISOR_EXIT()  OS_HAL_EXIT_KERNEL()
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

/* PRQA S 3458++ */ /* MISRA PRE10 */
#define Os_SetMSPStack(SP)                   \
    {                                           \
        OS_ASM("msr msp, %0\n":: "r"(SP));      \
        OS_ASM("dsb");                          \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* Switch msp to system stack*/
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define Os_Hal_Switch2System()                                                        \
    {                                                                                  \
        Os_SetMSPStack(OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack_Inf[Os_Hal_GetCoreID()]->StackTop));\
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
/* Msync/Isynch core instruction macros */
#define OS_ARCH_ISYNC() OS_ASM("isb")
#define OS_ARCH_DSYNC() OS_ASM("dsb")


#if (TRUE == CFG_SRV_SHELLOS)
/* PRQA S 3412++ */ /* MISRA PRE10 */
#define OS_HAL_ENTER_KERNEL()                           \
    Os_ThreadModeType cpuMode = Os_ArchGetThreadMode(); \
    OS_CHANGCPUMODE();
/* PRQA S 3412++ */                                           /* MISRA PRE10 */
#define OS_HAL_EXIT_KERNEL() OS_RESUMECPUMODE() /* PRQA S 3472 */ /* MISRA Dir-4.9 */
#else
#define OS_HAL_ENTER_KERNEL()
#define OS_HAL_EXIT_KERNEL()
#endif

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
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
            Os_ArchSetCpuMode((Os_CpuModeType)OS_THREAD_MODE_USER); \
        }                                       \
        else                                    \
        {                                       \
            /* nothing to do. */                \
        }                                       \
    }
#endif
/* PRQA S 3458-- */ /* MISRA PRE10 */
#define OS_HAL_TERMINATE_TASK(i)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/* the mode of thread. */
typedef enum
{
    OS_THREAD_MODE_SUPERVISOR = 0U,
    /*PRQA S 1271*/                          /* MISRA CWE-682 */
    OS_THREAD_MODE_USER = 1U /*PRQA S 1271*/ /* MISRA CWE-682 */
} Os_ThreadModeType;

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern uint32 Os_Hal_CmpSwapW(uint32 *address, uint32 compareVal, uint32 exchangedVal);
extern void Os_Hal_FirstEnterTask(Os_TaskType taskID);
extern void Os_Hal_StartScheduler(void);
extern void Os_Hal_Dispatch(void);
extern void Os_ArchSetCpuMode(Os_CpuModeType mode);
extern Os_ThreadModeType Os_ArchGetThreadMode(void);
extern Os_StackPtrType Os_ArchGetRunningTaskStack(void);
extern void Os_ArchSetRunningTaskStack(Os_StackPtrType stackTop);
extern uint8 Os_ArchIsFirstDispatch(void);
#if defined(OS_ENABLE_FPU)
extern void Os_ArchFPU_Init(void);
#endif

/* Shell protection */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
extern void Os_ArchSyscall(void);
#endif

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
extern void Os_ArchInitSystemTimer(CoreIdType CoreId);
extern void Os_ArchClearSysTimer(Os_CoreIdType coreId);
#endif

#if (TRUE == CFG_OS_MONITOR_ENABLE)
extern Os_TimerTickType Os_Hal_GetTimeTicks(void);
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */

#endif /* #ifdef ARCH_CM7_PROCESSOR_H */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */
/*=======[E N D   O F   F I L E]==============================================*/
