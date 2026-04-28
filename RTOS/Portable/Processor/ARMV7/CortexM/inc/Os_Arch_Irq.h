/* PRQA S 3108++ */
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
#include "Os_Mcu_Irq.h"
#include "Os_Interrupt_CfgData.h"
#include "Os_Arch_Processor.h"
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
#define OS_NVIC_PRIO_SHIFT    (8u - OS_NVIC_PRIO_BITS)
#define OS_NVIC_PRIO_MIN      0x00u
#define OS_NVIC_PRIO_MAX      0xFFu

/* OS exception priority configuration (logic interrupt priority)*/
#define OS_CM7_PRIO_CFG_PENDSV         (0xEu)
#define OS_NVIC_CONVERT_GET_PRIO(prio) (uint8)(((prio) ^ 0xFFu) >> OS_NVIC_PRIO_SHIFT)
#define OS_NVIC_CONVERT_SET_PRIO(prio) (uint8)(((prio) ^ 0xFFu) << OS_NVIC_PRIO_SHIFT)

#define OS_ISR_EXTERNAL_NUM     16u
#define OS_ISR_SYSTICK_ID       15u

/*-----------------------------Interrupt Macro-------------------------------*/
/* PRQA S 3472,2013 ++*/ /* MISRA Dir-4.9,Rule 15.7 */
#define OS_INTERRUPT_ENABLE(id)                                                       \
    do                                                                                \
    {                                                                                 \
        if ((uint32)(id) >= 16u)                                                      \
        {                                                                             \
            OS_NVIC_ISER_BASE[(uint32)((uint32)(id) - (uint32)16u) >> (uint32)5u] =   \
                (uint32)((uint32)1u << (uint32)(((uint32)(id) - 16u) & (uint32)0x1FU)); \
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
#define OS_INTERRUPT_TRIGGER(isrId)  OS_INTERRUPT_SET_PENDING(Os_IsrCfg[Os_GetObjLocalId(isrId)].IsrSrc)
/*Clear interrupt pending */
/* PRQA S 3409++*/ /* MISRA PRE02 */
#define OS_INTERRUPT_CLEAR_PENDING(id)                              \
    OS_NVIC_ICPR_BASE[((uint32)(id) - (uint32)16u) >> (uint32)5u] = \
        (uint32)((uint32)1u << (uint32)(((uint32)(id) - (uint32)16u) & (uint32)(0x1FU)))
/* PRQA S 3409--*/ /* MISRA PRE02 */

#define OS_INTERRUPT_CLEAR(isrId) OS_INTERRUPT_CLEAR_PENDING(Os_IsrCfg[Os_GetObjLocalId(isrId)].IsrSrc)

/* Get interrupt pending status */
#define OS_INTERRUPT_GET_PENDING_STATUS(id)                                                            \
    (((uint32)(id) >= (uint32)16u)                                                                     \
         ? ((OS_NVIC_ICPR_BASE[((uint32)(id) - 16u) >> 5u] >> (((uint32)(id) - 16u) & 0x1Fu)) & 0x01u) \
         : 0u)

/* Get interrupt active status */
#define OS_INTERRUPT_GET_ACTIVE_STATUS(id)                                                                               \
    (((uint32)(id) >= 16u) ? ((OS_NVIC_IABR_BASE[((uint32)(id) - 16u) >> 5u] >> (((uint32)(id) - 16u) & 0x1Fu)) & 0x01u) \
                           : 0u)

/* Get interrupt pending/active status */
#define OS_INTERRUPT_PENDING_OR_ACTIVE_STATUS(id)                                                                                                                                                       \
    (((uint32)(id) >= 16u)                                                                                                                                                                              \
         ? (((OS_NVIC_IABR_BASE[((uint32)(id) - 16u) >> 5u] >> (((uint32)(id) - 16u) & 0x1Fu)) & 0x01u) || ((OS_NVIC_ICPR_BASE[((uint32)(id) - 16u) >> 5u] >> (((uint32)(id) - 16u) & 0x1Fu)) & 0x01u)) \
         : 0u)

/* Check interrupt enable status */
/* PRQA S 3472++ */ /* MISRA Dir-4.9 */
#define OS_INTERRUPT_CHECK_STATUS(id)                                                                                          \
    (((uint32)(id) >= (uint32)16u)                                                                                             \
         ? ((OS_NVIC_ISER_BASE[((uint32)(id) - (uint32)16u) >> (uint32)5u] >> (((uint32)(id) - (uint32)16u) & 0x1Fu)) & 0x01u) \
         : 0u)
/* PRQA S 3472-- */ /* MISRA Dir-4.9 */

/* Set interrupt priority */
/* PRQA S 3472++ */ /* MISRA Dir-4.9 */ 
#define OS_INTERRUPT_SYS_SET_PRIO(id, prio) (OS_SCB_SHPR_BASE[(uint32)(id) - 4u] = (uint8)(prio))
/* PRQA S 3472-- */ /* MISRA Dir-4.9 */

/* PRQA S 2013 ++*/ /* MISRA Rule 15.7 */
#define OS_INTERRUPT_SET_PRIO(id, prio)                           \
    do                                                            \
    {                                                             \
        if ((uint32)(id) >= 16u)                                  \
        {                                                         \
            OS_NVIC_IPR_BASE[(uint32)(id) - 16u] = (uint8)(prio); \
        }                                                         \
        else if ((uint32)(id) >= 4u)                              \
        {                                                         \
            OS_SCB_SHPR_BASE[(uint32)(id) - 4u] = (uint8)(prio);  \
        }                                                         \
        else                                                      \
        {                                                         \
        }                                                         \
    } while (0)
/* PRQA S 2013 --*/ /* MISRA Rule 15.7 */
/* Install interrupt */
#define OS_INTERRUPT_INSTALL(id, prio)                             \
    do                                                             \
    {                                                              \
        OS_INTERRUPT_SET_PRIO(id, OS_NVIC_CONVERT_SET_PRIO(prio)); \
        OS_INTERRUPT_ENABLE(id);                                   \
    } while (0)

/*Un-install interrupt*/
#define OS_INTERRUPT_UNLOAD(id) OS_INTERRUPT_DISABLE(id)
#define Os_HAL_INT_IS_CLEARPENDING_SUPPORTED() (TRUE)


/* Critical Macro */
#define OS_HAL_DECLARE_CRITICAL()           Os_ArchMsrType msr
/* PRQA S 3472++ */ /* MISRA Dir-4.9 */
#define OS_HAL_ENTRY_CRITICAL()             (msr = Os_Hal_SuspendInt())
#define OS_HAL_EXIT_CRITICAL()              Os_Hal_RestoreInt(msr)
#define OS_HAL_TASK_SWITCH_PROC(coreId)     Os_ArchDispatch_ISR()
/* PRQA S 3472-- */ /* MISRA Dir-4.9 */

/* Disable/Enable HW Interrupts */
#define Os_Hal_DisableInt()     OS_ASM("cpsid i")
#define Os_Hal_EnableInt()      OS_ASM("dsb\n cpsie i\n")

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
#define OS_Hal_DisableIntApp(src)            \
        do                                   \
        {                                    \
            OS_INTERRUPT_CLEAR_PENDING(src); \
            OS_INTERRUPT_DISABLE(src);       \
        } while(0)
#endif /* #if (CFG_ISR_MAX > 0) */ /* PRQA S 2053 */                          /* MISRA Rule 18.8 */
#endif /* #if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC)) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

/*=======[Interrupt process Macro]========== */
#define OS_HAL_ISR2_EPILOGUE_KILL_ISR(isrId)         OS_ARCH_ISR2_EPILOGUE_1(isrId)
#define OS_ARCH_ISR2_EPILOGUE(isrId)                 OS_ARCH_ISR2_EPILOGUE_1(isrId)

#if (TRUE == CFG_PROTECTIONHOOK)
#define OS_HAL_ISR2_EPILOGUE(isrId)                  OS_ARCH_ISR2_EPILOGUE_1(isrId)
#endif

/*=======[I N T E R N A L   M A C R O]=======================================*/
/*  */
#if (TRUE == CFG_INT_NEST_ENABLE)
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR1_PROLOGUE(isrId)                          \
    {                                                         \
        Os_ArchSetCpuMode(OS_CPUMODE_SUPERVISOR);                 \
        Os_EnterISR1(isrId);                                  \
        if (TRUE == Os_IsrCfg[isrId].NestedEnable)            \
        {                                                     \
            Os_Hal_EnableInt();                               \
        }                                                     \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR1_EPILOGUE(isrId)              \
    {                                             \
        Os_Hal_DisableInt();                      \
        Os_ExitISR1(isrId);                       \
        Os_SetMSPStack(Os_ArchGetMasterSP());  \
        Os_Hal_EnableInt();                       \
        OS_RESUME_CONTEXT();                      \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#else
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR1_PROLOGUE(isrId)          \
    {                                         \
        Os_EnterISR1(isrId);                  \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR1_EPILOGUE(isrId)          \
    {                                         \
        Os_ExitISR1(isrId);                   \
        Os_SetMSPStack(Os_ArchGetMasterSP());  \
        Os_Hal_EnableInt();                   \
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

#else
#define OS_TIME_PROT_ENTRE(isrId)
#endif /* #if (TRUE == CFG_TIMING_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */


#if (TRUE == CFG_INT_NEST_ENABLE)
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ISR_NEST_ENTER(isrId)                     \
    {                                                \
        Os_EnterISR2(isrId);                         \
        if (TRUE == Os_IsrCfg[isrId].NestedEnable)   \
        {                                            \
            Os_Hal_EnableInt();                      \
        }                                            \
    }
/* PRQA S 3458-- */                                         /* MISRA PRE10 */
#define OS_ISR_NEST_EXIT(isrId) Os_ExitISR2(isrId); /* PRQA S 3412 */ /* MISRA PRE10 */
#else
#define OS_ISR_NEST_ENTER(isrId)        Os_EnterISR2(isrId); /* PRQA S 3412 */ /* MISRA PRE10 */
#define OS_ISR_NEST_EXIT(isrId)         Os_ExitISR2(isrId); /* PRQA S 3412 */  /* MISRA PRE10 */
#endif /* #if (TRUE == CFG_INT_NEST_ENABLE) */ /* PRQA S 2053 */   /* MISRA Rule 18.8 */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_SERVER_PROTECTION(isrId) Os_Isr2OccupyIntRes(isrId)
#else
#define OS_SERVER_PROTECTION(isrId)
#endif /* #if (TRUE == CFG_SERVICE_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_MEM_PROT_EXIT()                                         \
    {                                                              \
        if (1U == HardFault_Flag)                                  \
        {                                                          \
            Os_IsrTempIPSR = Os_ISRxPSRStack[Os_Kernel_GetIntNestNumISR2() - 1u]; \
            TERMINATEISR_ISR();                                    \
        }                                                          \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#else
#define OS_MEM_PROT_EXIT()
#endif /* #if (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */
/* PRQA S 3458++ */                                                       /* MISRA PRE10 */
#define OS_RESUME_CONTEXT()                                                 \
    {                                                                       \
        OS_ASM("    mrs r0, msp                                        \n"  \
               "    isb                                                \n"  \
               "    ldmia r0!, {r3-r11, r14}                           \n"  \
               "    msr control,r3                                     \n"  \
               "    tst r14, #0x10                                     \n"  \
               "    it eq                                              \n"  \
               "    vldmiaeq r0!, {s16-s31}                            \n"  \
               "    msr msp, r0                                        \n"  \
               "    mrs r0, ipsr                                       \n"  \
               "    ldr r1, =0xABCDEE00                                \n"  \
               "    ORR r0, r0, r1                                     \n"  \
               "    dsb                                                \n"  \
               "    isb                                                \n"  \
               "    cpsie i                                            \n"  \
               "    bx lr                                              \n");\
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_SAVE_xPSR()                                               \
    {                                                                \
        Os_ArchGetSaveXpsrAddr();                                   \
        OS_ASM("    mrs r2, xpsr                               \n"   \
               "    str r2, [r0]                               \n"); \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
/* Interrupt Entry/Exit Preface */
/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR2_PROLOGUE(isrId)          \
    {                                         \
        OS_SAVE_xPSR();                           \
        Os_SetMSPStack(OS_ARCH_STACK_ALIGN((uint32)Os_ISR2Stack[isrId].StackTop));\
        OS_TIME_PROT_ENTRE(isrId);            \
        Os_ArchSetCpuMode(OS_CPUMODE_SUPERVISOR); \
        OS_ISR_NEST_ENTER(isrId);             \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */

/* PRQA S 3458++ */ /* MISRA PRE10 */
#define OS_ARCH_ISR2_EPILOGUE_1(isrId)  \
    {                                   \
        OS_SERVER_PROTECTION(isrId);    \
        Os_Hal_DisableInt();            \
        OS_MEM_PROT_EXIT();             \
        OS_ISR_NEST_EXIT(isrId);        \
        Os_SetMSPStack(Os_ArchGetMasterSP());  \
        OS_RESUME_CONTEXT();            \
    }
/* PRQA S 3458-- */ /* MISRA PRE10 */
#define OS_HAL_TERMINATE_NESTED_ISR(i)      \
    {                                       \
        Os_IsrTempIPSR = Os_ISRxPSRStack[i];\
        TERMINATEISR_ISR();                 \
    }

#define OS_HAL_PROCESS_NESTED_ISR_STACK(i)  Os_ArchMasterSp_ARRAY[i + 1U] = Os_ArchMasterSp_ARRAY[i + 2U]


/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A   D E C L A R A T I O N S]================*/
extern uint32 Os_ISRxPSRStack[CFG_ISR_MAX];

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern uint16 HardFault_Flag;
#endif
extern uint32 Os_Isr2_Ipl_Limit;
extern uint32 Os_ArchMasterSp_ARRAY[CFG_ISR_MAX];
extern uint32 Os_IsrTempIPSR;

extern uint32 Os_LocalVector[OS_NVIC_NUM + 1u];

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern void TERMINATEISR_ISR(void);
extern void HardFault_Handler(void);
extern void PendSV_Handler(void);
extern void Os_IntHandler(void);
extern void Reset_Handler(void);
extern void NMI_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void Os_SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void SysTick_Handler(void);
extern void armv7_default_isr(void);

extern void             Os_ArchDispatch_ISR(void);
extern void             Os_Hal_RestoreInt(Os_ArchMsrType msr);
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
extern Os_ArchMsrType   Os_Hal_SuspendInt(void);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*PRQA S 3672 ++*/ /*MISRA CWE-398*/
extern void             Os_ArchInterruptInstall(uint8 id, uint8 prio, Os_isrhnd isrProc);
/*PRQA S 3672 --*/ /*MISRA CWE-398*/
extern void             Os_ArchInterruptInit(void);
extern Os_IPLType       Os_Hal_GetIpl(void);
extern void             Os_Hal_SetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc);
extern void             Os_Hal_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern void             Os_Hal_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
extern Os_IsrStateType  Os_Hal_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType);
extern void             Os_Hal_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern uint32           Os_Hal_GetCurrentIrq(void);
extern uint32           Os_Hal_GetCurrentExc(void);
extern void             Os_ArchSaveMasterSP(uint32* MSP);
extern uint32           Os_ArchGetMasterSP(void);
extern uint32           Os_ArchGetSaveXpsrAddr(void);
#endif /* #ifndef ARCH_IRQ_H */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */
/*=======[E N D   O F   F I L E]==============================================*/
