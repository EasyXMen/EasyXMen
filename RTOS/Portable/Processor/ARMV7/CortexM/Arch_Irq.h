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
**  FILENAME    : Arch_Irq.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Deal with operations related to processor interrupts        **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef ARCH_IRQ_H
#define ARCH_IRQ_H

/*=======[M I S R A C  R U L E  V I O L A T I O N]============================*/
/*  <MESSAGE ID>    <CODE LINE>    <REASON>
 */
/*============================================================================*/

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Os_Types.h"
#include "Os_CoreCfg.h"
#include "Mcu_Irq.h"
/*=======[M A C R O S]========================================================*/

/*----------------------------Cortex-CM7 Register-----------------------------*/
/*
 * CPU interrupts
 * */
#define OS_INITSTACK_IRQn        (0u)
#define OS_RESET_IRQn            (1u)
#define OS_NonMaskableInt_IRQn   (2u)  /**< Non Maskable Interrupt */
#define OS_HardFault_IRQn        (3u)  /**< Cortex-M7 SV Hard Fault Interrupt */
#define OS_MemoryManagement_IRQn (4u)  /**< Cortex-M7 Memory Management Interrupt */
#define OS_BusFault_IRQn         (5u)  /**< Cortex-M7 Bus Fault Interrupt */
#define OS_UsageFault_IRQn       (6u)  /**< Cortex-M7 Usage Fault Interrupt */
#define OS_SVCall_IRQn           (11u) /**< Cortex-M7 SV Call Interrupt */
#define OS_DebugMonitor_IRQn     (12u) /**< Cortex-M7 Debug Monitor Interrupt */
#define OS_PendSV_IRQn           (14u) /**< Cortex-M7 Pend SV Interrupt */
#define OS_ISR_SysTick_ADDR      (15u) /**< Cortex-M7 System Tick Interrupt */

/*
 * Nested Vectored Interrupt Controller
 * */
/*Interrupt Set-Enable Register 0-15: 0xE000E100 -> 0xE000E13C */
#define OS_NVIC_ISER_BASE ((volatile uint32*)(0xE000E100U))
/*Interrupt Clear-Enable Register 0-15: 0xE000E180 -> 0xE000E1BC */
#define OS_NVIC_ICER_BASE ((volatile uint32*)(0xE000E180U))
/*Interrupt Set-Pending Register 0-15: 0xE000E200 -> 0xE000E23C */
#define OS_NVIC_ISPR_BASE ((volatile uint32*)(0xE000E200U))
/*Interrupt Clear-Pending Register 0-15: 0xE000E280 -> 0xE000E2BC */
#define OS_NVIC_ICPR_BASE ((volatile uint32*)(0xE000E280U))
/*Active Bit Register Register 0-15: 0xE000E300 -> 0xE000E37C */
#define OS_NVIC_IABR_BASE ((volatile uint32*)(0xE000E300U))
/*Interrupt Priority Register 0-123: 0xE000E400 -> 0xE000E7EC */
#define OS_NVIC_IPR_BASE ((volatile uint8*)(0xE000E400U))
/*Software Trigger Interrupt Register: 0xE000EF00UL */
#define OS_NVIC_STIR OS_REG32(0xE000EF00U)
/*System Handler Priority Register 1-3: 0xE000ED18 -> 0xE000ED20*/
#define OS_SCB_SHPR_BASE ((volatile uint8*)(0xE000ED18U))
/*Interrupt Control and state Register: 0xE000ED04*/
#define OS_SCB_ICSE_BASE OS_REG32(0xE000ED04U)

/*
 * System control block
 * */
/*Vector table offset register*/
#define OS_REG_VTOR OS_REG32(0xE000ED08u)

/* Interrupt priority conversion */
#define OS_NVIC_PRIO_BITS     (4u)
#define OS_NVIC_PRIO_LEVELS   (1u << OS_NVIC_PRIO_BITS)
#define OS_NVIC_PRIO_SHIFT    (8u - OS_NVIC_PRIO_BITS)
#define OS_NVIC_PRIO_MIN      0x00u
#define OS_NVIC_PRIO_MAX      0xFFu
#define OS_NIVC_PENDSVSET_BIT (1u << 28u)

/* OS exception priority configuration (logic interrupt priority)*/
#define OS_CM7_PRIO_CFG_PENDSV         (0xEu)
#define OS_NVIC_CONVERT_GET_PRIO(prio) (uint8)(((prio) ^ 0xFFu) >> OS_NVIC_PRIO_SHIFT)
#define OS_NVIC_CONVERT_SET_PRIO(prio) (uint8)(((prio) ^ 0xFFu) << OS_NVIC_PRIO_SHIFT)

/*-----------------------------Interrupt Macro-------------------------------*/
/* PRQA S 3472,2013 ++*/ /* MISRA Dir-4.9,Rule 15.7 */
#define OS_INTERRUPT_ENABLE(id)                                                       \
    do                                                                                \
    {                                                                                 \
        if ((uint32)(id) >= 16u)                                                      \
        {                                                                             \
            OS_NVIC_ISER_BASE[(uint32)((uint32)(id) - (uint32)16u) >> (uint32)5u] =   \
                (uint32)((uint32)1u << (uint32)(((uint32)(id)-16u) & (uint32)0x1FU)); \
        }                                                                             \
        else if ((uint32)(id) == 15u)                                                 \
        {                                                                             \
            OS_SYSTICK_CTRL_REG = (OS_SYSTICK_CTRL_REG | (uint32)1u);                 \
        }                                                                             \
        else                                                                          \
        {                                                                             \
        }                                                                             \
    } while (0)
/* PRQA S 3472,2013 --*/ /* MISRA Dir-4.9,Rule 15.7 */
/* PRQA S 3472,2013 ++*/ /* MISRA Dir-4.9,Rule 15.7 */
#define OS_INTERRUPT_DISABLE(id)                                                                            \
    do                                                                                                      \
    {                                                                                                       \
        if ((uint32)(id) >= 16u)                                                                            \
        {                                                                                                   \
            OS_NVIC_ICER_BASE[(uint32)((uint32)(id) - (uint32)16u) >> (uint32)5u] =                         \
                (uint32)((uint32)1u << (uint32)((uint32)(((uint32)(id) - (uint32)16u)) & (uint32)(0x1Fu))); \
        }                                                                                                   \
        else if ((uint32)(id) == 15u)                                                                       \
        {                                                                                                   \
            OS_SYSTICK_CTRL_REG = (OS_SYSTICK_CTRL_REG & (~((uint32)1u)));                                  \
        }                                                                                                   \
        else                                                                                                \
        {                                                                                                   \
        }                                                                                                   \
    } while (0)
/* PRQA S 3472,2013 --*/ /* MISRA Dir-4.9,Rule 15.7 */
/*Set interrupt pending */
#define OS_INTERRUPT_SET_PENDING(id) OS_NVIC_STIR = ((uint32)(id)-16u) & 0x1FFu /* PRQA S 3409*/ /* MISRA PRE02 */
#define OS_INTERRUPT_TRIGGER(isrId)  OS_INTERRUPT_SET_PENDING(Os_IsrCfg[Os_GetObjLocalId(isrId)].OsIsrSrc)
#define OS_INTERRUPT_CLEAR(isrId)    OS_INTERRUPT_SET_PENDING(Os_IsrCfg[Os_GetObjLocalId(isrId)].OsIsrSrc)
/*Clear interrupt pending */
/* PRQA S 3409++*/ /* MISRA PRE02 */
#define OS_INTERRUPT_CLEAR_PENDING(id)                              \
    OS_NVIC_ICPR_BASE[((uint32)(id) - (uint32)16u) >> (uint32)5u] = \
        (uint32)((uint32)1u << (uint32)(((uint32)(id) - (uint32)16u) & (uint32)(0x1FU)))
/* PRQA S 3409--*/ /* MISRA PRE02 */

/* Get interrupt pending status */
#define OS_INTERRUPT_GET_PENDING_STATUS(id)                                                        \
    (((uint32)(id) >= (uint32)16u)                                                                 \
         ? ((OS_NVIC_ICPR_BASE[((uint32)(id)-16u) >> 5u] >> (((uint32)(id)-16u) & 0x1Fu)) & 0x01u) \
         : 0u)

/* Get interrupt active status */
#define OS_INTERRUPT_GET_ACTIVE_STATUS(id)                                                                           \
    (((uint32)(id) >= 16u) ? ((OS_NVIC_IABR_BASE[((uint32)(id)-16u) >> 5u] >> (((uint32)(id)-16u) & 0x1Fu)) & 0x01u) \
                           : 0u)

/* Get interrupt pending/active status */
#define OS_INTERRUPT_PENDING_OR_ACTIVE_STATUS(id)                                                       \
    (((uint32)(id) >= 16u)                                                                              \
         ? (((OS_NVIC_IABR_BASE[((uint32)(id)-16u) >> 5u] >> (((uint32)(id)-16u) & 0x1Fu)) & 0x01u)     \
            || ((OS_NVIC_ICPR_BASE[((uint32)(id)-16u) >> 5u] >> (((uint32)(id)-16u) & 0x1Fu)) & 0x01u)) \
         : 0u)

/* Check interrupt enable status */
/* PRQA S 3472++ */ /* MISRA Dir-4.9 */
#define OS_INTERRUPT_CHECK_STATUS(id)                                                                                 \
    (((uint32)(id) >= (uint32)16u)                                                                                    \
         ? ((OS_NVIC_ISER_BASE[((uint32)(id) - (uint32)16u) >> (uint32)5u] >> (((uint32)(id) - (uint32)16u) & 0x1Fu)) \
            & 0x01u)                                                                                                  \
         : 0u)
/* PRQA S 3472-- */ /* MISRA Dir-4.9 */

/* Set interrupt priority */
/* PRQA S 3472++ */ /* MISRA Dir-4.9 */
#define OS_INTERRUPT_SYS_SET_PRIO(id, prio) (OS_SCB_SHPR_BASE[(uint32)(id)-4u] = (uint8)(prio))
/* PRQA S 3472-- */ /* MISRA Dir-4.9 */

/* PRQA S 2013 ++*/ /* MISRA Rule 15.7 */
#define OS_INTERRUPT_SET_PRIO(id, prio)                         \
    do                                                          \
    {                                                           \
        if ((uint32)(id) >= 16u)                                \
        {                                                       \
            OS_NVIC_IPR_BASE[(uint32)(id)-16u] = (uint8)(prio); \
        }                                                       \
        else if ((uint32)(id) >= 4u)                            \
        {                                                       \
            OS_SCB_SHPR_BASE[(uint32)(id)-4u] = (uint8)(prio);  \
        }                                                       \
        else                                                    \
        {                                                       \
        }                                                       \
    } while (0)
/* PRQA S 2013 --*/ /* MISRA Rule 15.7 */
/* Install interrupt */
#define OS_INTERRUPT_INSTALL(id, prio, srcType)                    \
    do                                                             \
    {                                                              \
        OS_INTERRUPT_SET_PRIO(id, OS_NVIC_CONVERT_SET_PRIO(prio)); \
        OS_INTERRUPT_ENABLE(id);                                   \
    } while (0)

/*Un-install interrupt*/
#define OS_INTERRUPT_UNLOAD(id, prio, srcType) OS_INTERRUPT_DISABLE(id)

/* Critical Macro */
#define OS_ARCH_DECLARE_CRITICAL() Os_ArchMsrType msr
/* PRQA S 3472++ */ /* MISRA Dir-4.9 */
#define OS_ARCH_ENTRY_CRITICAL() Os_ArchSuspendOsInt(&msr)
#define OS_ARCH_EXIT_CRITICAL()  Os_ArchRestoreOsInt(msr)
#define OS_ARCH_SUSPEND_ALLINT() Os_ArchSuspendInt(&msr)
#define OS_ARCH_RESTORE_ALLINT() Os_ArchRestoreInt(msr)
#define OS_TASK_SWITCH_PROC()    Os_ArchDispatch_ISR()
/* PRQA S 3472-- */ /* MISRA Dir-4.9 */

/* Disable/Enable HW Interrupts */
#define Os_ArchDisableInt() OS_ASM("cpsid i")
#define Os_ArchEnableInt()  OS_ASM("dsb\n cpsie i\n")

/*=======[Interrupt process Macro]========== */
#define OS_ARCH_ISR2_EPILOGUE_KILL_ISR() OS_ARCH_ISR2_EPILOGUE_1()
#define OS_ARCH_ISR2_EPILOGUE()          OS_ARCH_ISR2_EPILOGUE_1()

/*=======[I N T E R N A L   M A C R O]=======================================*/
/*  */
#if (TRUE == CFG_INT_NEST_ENABLE)
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR1_PROLOGUE(isrId)                          \
    {                                                         \
        Os_IntCfgIsrId = (isrId);                             \
        if (0U == Os_IntNestISR1)                             \
        {                                                     \
            Os_SaveLevelISR1 = Os_SCB.sysOsLevel;             \
            Os_SCB.sysOsLevel = OS_LEVEL_ISR1;                \
        }                                                     \
        Os_SetCpuMode(OS_CPUMODE_SUPERVISOR);                 \
        Os_EnterISR1();                                       \
        Os_IntNestISR1++;                                     \
        if (TRUE == Os_IsrCfg[Os_IntCfgIsrId].OsNestedEnable) \
        {                                                     \
            Os_ArchEnableInt();                               \
        }                                                     \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR1_EPILOGUE()                   \
    {                                             \
        Os_ArchDisableInt();                      \
        Os_ExitISR1();                            \
        Os_IntNestISR1--;                         \
        if (0U == Os_IntNestISR1)                 \
        {                                         \
            Os_SCB.sysOsLevel = Os_SaveLevelISR1; \
        }                                         \
        Os_ISR1Switch2PreStk();                   \
        Os_ArchEnableInt();                       \
        OS_RESUME_CONTEXT();                      \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#else
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR1_PROLOGUE(isrId)          \
    {                                         \
        Os_SaveLevelISR1 = Os_SCB.sysOsLevel; \
        Os_SCB.sysOsLevel = OS_LEVEL_ISR1;    \
        Os_EnterISR1();                       \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR1_EPILOGUE()               \
    {                                         \
        Os_SCB.sysOsLevel = Os_SaveLevelISR1; \
        Os_ExitISR1();                        \
        Os_ISR1Switch2PreStk();               \
        Os_ArchEnableInt();                   \
        OS_RESUME_CONTEXT();                  \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#endif /* #if (TRUE == CFG_INT_NEST_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_TIME_PROT_ENTRE(isrId)                \
    {                                            \
        if (E_OK != Os_TmProtIsrFrameChk(isrId)) \
        {                                        \
            return;                              \
        }                                        \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#if (CFG_SC == OS_SC4)
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_TIME_PROT_EXIT()                                                                    \
    {                                                                                          \
        if ((TRUE == Os_TprotTerminateIsr) && (Os_IntNestISR2 >= 2U) && (HardFault_Flag == 0)) \
        {                                                                                      \
            Os_IntNestISR2--;                                                                  \
            Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u;                          \
            Os_TprotTerminateIsr = FALSE;                                                      \
            Os_IsrTempIPSR = Os_ISRxPSRStack[Os_IntNestISR2 - 1];                              \
            TERMINATEISR_ISR();                                                                \
        }                                                                                      \
        if (HardFault_Flag == 1)                                                               \
        {                                                                                      \
            HardFault_Flag = 0;                                                                \
        }                                                                                      \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#else
#define OS_TIME_PROT_EXIT()                                           \
    {                                                                 \
        if ((TRUE == Os_TprotTerminateIsr) && (Os_IntNestISR2 >= 2U)) \
        {                                                             \
            Os_IntNestISR2--;                                         \
            Os_SCB.sysDispatchLocker = Os_SCB.sysDispatchLocker - 1u; \
            Os_TprotTerminateIsr = FALSE;                             \
            Os_IsrTempIPSR = Os_ISRxPSRStack[Os_IntNestISR2 - 1];     \
            TERMINATEISR_ISR();                                       \
        }                                                             \
    }
#endif
#else
#define OS_TIME_PROT_ENTRE(isrId)
#define OS_TIME_PROT_EXIT()
#endif /* #if (TRUE == CFG_TIMING_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

/* switch msp to ISR2 stack */
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define Os_Switch2ISR2Stk()                                        \
    {                                                              \
        OS_ASM("    ldr r0, =Os_IntCfgIsrId                 \n"    \
               "    ldrb r0, [r0]                              \n" \
               "    ldr r1, =Os_ISR2Stack   \n"                    \
               "    ldr r1,[r1] \n"                                \
               "    lsl r0, r0, #3   \n"                           \
               "    add r1,r1, r0\n"                               \
               "    dsb  \n"                                       \
               "    ldr r2,[r1] \n"                                \
               "    msr msp, r2                   \n");            \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* Resume ISR1 msp */
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define Os_ISR1Switch2PreStk()                                     \
    {                                                              \
        OS_ASM("    ldr r0, =Os_IntNestISR2                 \n"    \
               "    ldrb r0, [r0]                              \n" \
               "    ldr r1, =Os_IntNestISR1   \n"                  \
               "    ldrb r1,[r1] \n"                               \
               "    add r0,r0,r1   \n"                             \
               "   ldr r1, =Os_ArchMasterSp_ARRAY\n"               \
               "   lsl r0, r0, #2\n"                               \
               "    add r1,r1, r0\n"                               \
               "    ldr r2,[r1] \n"                                \
               "    msr msp, r2                   \n");            \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* Resume ISR2 msp */
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define Os_ISR2Switch2PreStk()                                      \
    {                                                               \
        OS_ASM("    ldr r0, =Os_IntNestISR2                 \n"     \
               "    ldrb r0, [r0]                              \n"  \
               "    ldr r1, =Os_ArchMasterSp_ARRAY   \n"            \
               "    lsl r0, r0, #2   \n"                            \
               "    add r1,r1, r0\n"                                \
               "    ldr r1,[r1]                               \n"   \
               "    msr msp, r1                               \n"); \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#if (TRUE == CFG_INT_NEST_ENABLE)
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ISR_NEST_ENTER(isrId)                     \
    {                                                \
        Os_EnterISR2();                              \
        if (TRUE == Os_IsrCfg[isrId].OsNestedEnable) \
        {                                            \
            Os_ArchEnableInt();                      \
        }                                            \
    }
/* PRQA S 3458-- */                                         /* MISRA PRE10 */
#define OS_ISR_NEST_EXIT() Os_ExitISR2(); /* PRQA S 3412 */ /* MISRA PRE10 */

#else
#define OS_ISR_NEST_ENTER(isrId) Os_EnterISR2(); /* PRQA S 3412 */ /* MISRA PRE10 */
#define OS_ISR_NEST_EXIT()       Os_ExitISR2(); /* PRQA S 3412 */  /* MISRA PRE10 */
#endif /* #if (TRUE == CFG_INT_NEST_ENABLE) */ /* PRQA S 2053 */   /* MISRA Rule 18.8 */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_SERVER_PROTECTION() Os_Isr2OccupyIntRes()
#else
#define OS_SERVER_PROTECTION()
#endif /* #if (TRUE == CFG_SERVICE_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_MEM_PROT_EXIT()                                         \
    {                                                              \
        if (1U == HardFault_Flag)                                  \
        {                                                          \
            Os_IsrTempIPSR = Os_ISRxPSRStack[Os_IntNestISR2 - 1u]; \
            TERMINATEISR_ISR();                                    \
        }                                                          \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#else
#define OS_MEM_PROT_EXIT()
#endif /* #if (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */
/* PRQA S 3458++ */                                                       /* MISRA PRE10 */
#define OS_RESUME_CONTEXT()                                                \
    {                                                                      \
        OS_ASM("    mrs r0, msp                                        \n" \
               "    isb                                                \n" \
               "    ldmia r0!, {r3-r11, r14}                           \n" \
               "    msr control,r3                                     \n" \
               "    tst r14, #0x10                  \n"                    \
               "    it eq                           \n"                    \
               "    vldmiaeq r0!, {s16-s31}         \n"                    \
               "    msr msp, r0                                        \n" \
               "    mrs r0, ipsr                                       \n" \
               "    ldr r1, =0xABCDEE00                                \n" \
               "    ORR r0, r0, r1                                     \n" \
               "    dsb                                                \n" \
               "    isb                                                \n" \
               "    cpsie i                                            \n" \
               "   bx lr       \n");                                       \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_SAVE_xPSR()                                               \
    {                                                                \
        OS_ASM("    ldr r0, =Os_IntNestISR2                 \n"      \
               "    ldrb r0, [r0]                              \n"   \
               "    ldr r1, =Os_ISRxPSRStack   \n"                   \
               "    lsl r0, r0, #2   \n"                             \
               "    add r1,r1, r0\n"                                 \
               "    mrs r2, xpsr                               \n"   \
               "    str r2, [r1]                               \n"); \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
/* Interrupt Entry/Exit Preface */
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR2_PROLOGUE(isrId)          \
    {                                         \
        Os_IntCfgIsrId = (isrId);             \
        OS_SAVE_xPSR();                       \
        Os_Switch2ISR2Stk();                  \
        OS_TIME_PROT_ENTRE(isrId);            \
        Os_SetCpuMode(OS_CPUMODE_SUPERVISOR); \
        OS_ISR_NEST_ENTER(isrId);             \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR2_EPILOGUE_1() \
    {                             \
        Os_ArchDisableInt();      \
        OS_SERVER_PROTECTION();   \
        OS_MEM_PROT_EXIT();       \
        OS_TIME_PROT_EXIT();      \
        OS_ISR_NEST_EXIT();       \
        Os_ISR2Switch2PreStk();   \
        OS_RESUME_CONTEXT();      \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* type of an Interrupt Service Routine (ISR) */
typedef void (*Os_isrhnd)(void);
typedef uint32 Os_ArchMsrType;
/*=======[E X T E R N A L   D A T A   D E C L A R A T I O N S]================*/
extern VAR(uint32, OS_VAR) Os_ISRxPSRStack[OS_ISR_MAXNUM];

#if (CFG_ISR_MAX > 0U)
extern VAR(Os_CallLevelType, OS_VAR) Os_SaveLevelISR1;
#endif /* #if (CFG_ISR_MAX > 0U) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern VAR(uint16, OS_VAR) HardFault_Flag;
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern VAR(volatile uint32, OS_VAR) Os_TprotTerminateIsr;
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern VAR(volatile uint32, OS_VAR) Os_TprotTerminateTask;
#endif

extern VAR(uint32, OS_VAR) Os_ArchTempSysSp;
extern VAR(uint32, OS_VAR) Os_ArchMasterSp_ARRAY[OS_ISR_MAXNUM];
extern VAR(uint32, OS_VAR) Os_IsrTempIPSR;

extern uint32 Os_GlobalVector[OS_NVIC_NUM + 1u];
extern uint32 Os_LocalVector[OS_NVIC_NUM + 1u];

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, OS_CODE) Os_ArchSuspendInt(P2VAR(Os_ArchMsrType, AUTOMATIC, OS_VAR) msr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
extern FUNC(void, OS_CODE) Os_ArchRestoreInt(Os_ArchMsrType msr);
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
extern FUNC(void, OS_CODE) Os_ArchAppTerminateIsrProc(Os_IsrType OsIsrID);
extern FUNC(void, OS_CODE) Os_ArchDisableIntInApp(P2CONST(Os_ApplicationCfgType, AUTOMATIC, OS_VAR) posCurAppCfg);
#endif
#endif

extern FUNC(void, OS_CODE) TERMINATEISR_ISR(void);

extern FUNC(void, OS_CODE) HardFault_Handler(void);
extern FUNC(void, OS_CODE) PendSV_Handler(void);
extern FUNC(void, OS_CODE) Os_IntHandler(void);
extern FUNC(void, OS_CODE) Reset_Handler(void);
extern FUNC(void, OS_CODE) NMI_Handler(void);
extern FUNC(void, OS_CODE) MemManage_Handler(void);
extern FUNC(void, OS_CODE) BusFault_Handler(void);
extern FUNC(void, OS_CODE) UsageFault_Handler(void);
extern FUNC(void, OS_CODE) SVC_Handler(void);
extern FUNC(void, OS_CODE) DebugMon_Handler(void);
extern FUNC(void, OS_CODE) SysTick_Handler(void);
extern FUNC(void, OS_CODE) armv7_default_isr(void);
/*PRQA S 3672 ++*/ /*MISRA CWE-398*/
extern FUNC(void, OS_CODE) Os_InterruptInstall(uint8 id, uint8 prio, uint32 srcType, Os_isrhnd isrProc);
extern FUNC(void, OS_CODE) Os_InterruptUnload(uint8 id, uint8 prio, uint32 srcType, Os_isrhnd isrProc);
/*PRQA S 3672 --*/ /*MISRA CWE-398*/
extern FUNC(void, OS_CODE) Os_InterruptInit(void);
extern FUNC(Os_IPLType, OS_CODE) Os_ArchGetIpl(void);
extern FUNC(void, OS_CODE) Os_ArchSetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc);
extern FUNC(void, OS_CODE) Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(void, OS_CODE) Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(Os_IsrStateType, OS_CODE) Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(void, OS_CODE) Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern FUNC(void, OS_CODE) Os_ArchSuspendOsInt(P2VAR(Os_ArchMsrType, AUTOMATIC, OS_VAR) msr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
extern FUNC(void, OS_CODE) Os_ArchRestoreOsInt(Os_ArchMsrType msr);
#endif /* #ifndef ARCH_IRQ_H */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */
/*=======[E N D   O F   F I L E]==============================================*/
