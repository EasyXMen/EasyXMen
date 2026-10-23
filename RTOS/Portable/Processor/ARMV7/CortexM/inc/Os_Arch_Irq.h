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
**  FILENAME    : Os_Arch_Irq.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Deal with operations related to processor interrupts        **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform r23--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef OS_ARCH_IRQ_H
#define OS_ARCH_IRQ_H

/*=======[I N C L U D E S]====================================================*/

#include "Os_Types.h"
#include "Os_Arch_Define.h"
#include "Os_Mcu_Config.h"
#include "Os_Arch_Processor.h"
#include "Os_Interrupt_CfgData.h"

/*=======[M A C R O S]========================================================*/

/* ========================================
* Functional Macro Defination Local Used *
======================================== */

/* Interrupt Service Provider */
#define OS_ARCH_INT_CPU0            (0x00000000U)
#define OS_ARCH_INT_CPU1            (0x00000001U)

/* CPU interrupts Number */
#define OS_INITSTACK_IRQn           (OS_NUM_0)
#define OS_RESET_IRQn               (OS_NUM_1)
#define OS_NonMaskableInt_IRQn      (OS_NUM_2)  /* Non Maskable Interrupt */
#define OS_HardFault_IRQn           (OS_NUM_3)  /* Cortex-M4 SV Hard Fault Interrupt */
#define OS_MemoryManagement_IRQn    (OS_NUM_4)  /* Cortex-M4 Memory Management Interrupt */
#define OS_BusFault_IRQn            (OS_NUM_5)  /* Cortex-M4 Bus Fault Interrupt */
#define OS_UsageFault_IRQn          (OS_NUM_6)  /* Cortex-M4 Usage Fault Interrupt */
#define OS_SVCall_IRQn              (OS_NUM_11) /* Cortex-M4 SV Call Interrupt */
#define OS_DebugMonitor_IRQn        (OS_NUM_12) /* Cortex-M4 Debug Monitor Interrupt */
#define OS_PendSV_IRQn              (OS_NUM_14) /* Cortex-M4 Pend SV Interrupt */
#define OS_ISR_SysTick_IRQn         (OS_NUM_15) /* Cortex-M4 System Tick Interrupt */
#define OS_ARCH_ISR_EXTERNAL_NUM    (OS_NUM_16)

/* Nested Vectored Interrupt Controller, 
 fixed address specified by architecture */
#define OS_ARCH_NVIC_ISER_BASE                  ((volatile uint32*)0xE000E100U)     /*Interrupt Set-Enable Register 0-15: 0xE000E100 -> 0xE000E13C */
#define OS_ARCH_NVIC_ICER_BASE                  ((volatile uint32*)0xE000E180U)     /*Interrupt Clear-Enable Register 0-15: 0xE000E180 -> 0xE000E1BC */
#define OS_ARCH_NVIC_ISPR_BASE                  ((volatile uint32*)0xE000E200U)     /*Interrupt Set-Pending Register 0-15: 0xE000E200 -> 0xE000E23C */
#define OS_ARCH_NVIC_ICPR_BASE                  ((volatile uint32*)0xE000E280U)     /*Interrupt Clear-Pending Register 0-15: 0xE000E280 -> 0xE000E2BC */
#define OS_ARCH_NVIC_IABR_BASE                  ((volatile uint32*)0xE000E300U)     /*Active Bit Register Register 0-15: 0xE000E300 -> 0xE000E37C */
#define OS_ARCH_NVIC_IPR_BASE                   ((volatile uint8*)0xE000E400U)      /*Interrupt Priority Register 0-123: 0xE000E400 -> 0xE000E7EC */
#define OS_ARCH_SCB_SHPR_BASE                   ((volatile uint8*)0xE000ED18U)      /*System Handler Priority Register 1-3: 0xE000ED18 -> 0xE000ED20*/

/* Caculate Relative operation */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_ARCH_NVIC_GET_REG32_BIT_OFFSET(id)   (((uint32)(id) - OS_NUM_16) & (uint32)OS_5BIT_MASK)
#define OS_ARCH_NVIC_GET_REG32_BITMASK(id)      ((uint32)OS_1BIT_MASK << OS_ARCH_NVIC_GET_REG32_BIT_OFFSET(id))
/* PRQA S 3472 -- */ 
#define OS_ARCH_NVIC_GET_REG32_OFFSET(id)       (((uint32)(id) - (uint32)OS_NUM_16) >> (uint32)OS_NUM_5)
#define OS_ARCH_NVIC_IPR_OFFSET(id)             (((uint32)(id) - (uint32)OS_NUM_16))
#define OS_ARCH_SCB_SHPR_OFFSET(id)             (((uint32)(id) - (uint32)OS_NUM_4))

#define OS_ARCH_NVIC_IRQNUM_VALID_BITMASK       (0x1FFU)

#define OS_ARCH_REG_NVIC_ISER(id)               OS_ARCH_NVIC_ISER_BASE[OS_ARCH_NVIC_GET_REG32_OFFSET(id)]
#define OS_ARCH_REG_NVIC_ICER(id)               OS_ARCH_NVIC_ICER_BASE[OS_ARCH_NVIC_GET_REG32_OFFSET(id)]
#define OS_ARCH_REG_NVIC_ISPR(id)               OS_ARCH_NVIC_ISPR_BASE[OS_ARCH_NVIC_GET_REG32_OFFSET(id)]
#define OS_ARCH_REG_NVIC_ICPR(id)               OS_ARCH_NVIC_ICPR_BASE[OS_ARCH_NVIC_GET_REG32_OFFSET(id)]
#define OS_ARCH_REG_NVIC_IABR(id)               OS_ARCH_NVIC_IABR_BASE[OS_ARCH_NVIC_GET_REG32_OFFSET(id)]
#define OS_ARCH_REG_NVIC_IPR(id)                OS_ARCH_NVIC_IPR_BASE[OS_ARCH_NVIC_IPR_OFFSET(id)]
#define OS_ARCH_REG_SCB_SHPR(id)                OS_ARCH_SCB_SHPR_BASE[OS_ARCH_SCB_SHPR_OFFSET(id)]

#define OS_ARCH_REG_NVIC_STIR                   OS_ARCH_REG32(0xE000EF00U)      /*Software Trigger Interrupt Register: 0xE000EF00 */
#define OS_ARCH_REG_SCB_ICSR                    OS_ARCH_REG32(0xE000ED04U)      /*Interrupt Control and state Register: 0xE000ED04*/

/*Vector table offset register*/
#define OS_ARCH_REG_VTOR                        OS_ARCH_REG32(0xE000ED08U)

/* Interrupt priority conversion */
#define OS_NVIC_PRIO_SHIFT (OS_NUM_8 - OS_MCU_NVIC_PRIO_BITS)
#define OS_NVIC_PRIO_MIN   ((uint8)0x01)
#define OS_NVIC_PRIO_MAX   ((uint8)0xFF)

/* OS exception priority configuration (logic interrupt priority)*/
/* The priority numbering for the current Cortex-M architecture starts from 1, but the actual hardware mapping starts from 0
 * Reflect Table(Example OS_MCU_NVIC_PRIO_BITS = 3):
 * Priority - - - - - - - RegisterVal
 *    1           ->           7
 *    2           ->           6
 *    3           ->           5
 *    4           ->           4
 *    5           ->           3
 *    6           ->           2
 *    7           ->           1
 *    8           ->           0
*/
#define OS_ARCH_NVIC_CONVERT_GET_PRIO(prio) (uint8)((((prio) ^ OS_NVIC_PRIO_MAX) >> OS_NVIC_PRIO_SHIFT) + OS_NUM_1)
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_ARCH_NVIC_CONVERT_SET_PRIO(prio) ((uint8)(((((prio) - OS_NUM_1) ^ OS_NVIC_PRIO_MAX) << OS_NVIC_PRIO_SHIFT) & 0xFFu))
/* PRQA S 3472 -- */ 

#if (TRUE == CFG_INT_NEST_ENABLE)
/* PRQA S 3458 ++ */ /* VL_Os_3458 */
#define OS_ARCH_ISR_NEST_ENTER(isrId)                                                           \
        {                                                                                       \
            if (TRUE == Os_IsrCfg[isrId].NestedEnable)                                          \
            {                                                                                   \
                Os_Hal_EnableInt();                                                             \
            }                                                                                   \
        }
/* PRQA S 3458 -- */ 
#else
#define OS_ARCH_ISR_NEST_ENTER(isrId)
#endif /* #if (TRUE == CFG_INT_NEST_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */

/* Nest interrupt operation */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_ARCH_SERVER_PROTECTION(isrId)                Os_Isr2OccupyIntRes(isrId)
#else
#define OS_ARCH_SERVER_PROTECTION(isrId)
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */

/* Timer Protection Enter */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_ARCH_TIME_PROT_ENTER(isrId)                                                          \
    {                                                                                           \
        if (E_OK != Os_TmProtIsrFrameChk(isrId))                                                \
        {                                                                                       \
            Os_Arch_DefaultIntExit(Os_Arch_GetMainSP());                                        \
        }                                                                                       \
    }

#define OS_ARCH_TIME_PROT_EXIT()                                                                \
    {                                                                                           \
        if (OS_PROTECTION_TERMINATE_TPROT_ISR ==                                                \
            Os_Kernel_GetAndComparTerminateObject(OS_PROTECTION_TERMINATE_TPROT_ISR))           \
        {                                                                                       \
            Os_Arch_ForceInterruptExit();  /* Exit residual interrupt state if present. */      \
            Os_ArchIsrTempIPSR = Os_ArchIsrxPSRNest[Os_Kernel_GetIntNestNumISR2()];             \
            Os_Arch_TerminateISR();                                                             \
        }                                                                                       \
    }

#else
#define OS_ARCH_TIME_PROT_ENTER(isrId)
#define OS_ARCH_TIME_PROT_EXIT()
#endif /* #if (TRUE == CFG_TIMING_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_ARCH_MEM_PROT_EXIT()                                                                 \
    {                                                                                           \
        if (TRUE == OS_ArchHardFaultFlag)                                                       \
        {                                                                                       \
            OS_ArchHardFaultFlag = (boolean)FALSE;                                              \
            Os_ArchIsrTempIPSR = Os_ArchIsrxPSRNest[Os_Kernel_GetIntNestNumISR2() - OS_NUM_1];  \
            Os_Arch_TerminateISR();                                                             \
        }                                                                                       \
    }
#else
#define OS_ARCH_MEM_PROT_EXIT()
#endif /* #if (TRUE == CFG_MEMORY_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */

/* Here is for test only */
#define OS_INTERRUPT_TRIGGER(CFG_ISR_ID) Os_Arch_SetPendingInterrupt(Os_IsrCfg[Os_GetObjLocalId(CFG_ISR_ID)].IsrSrc, OS_ARCH_INT_CPU0)
#define OS_INTERRUPT_CLEAR(CFG_ISR_ID)   Os_Hal_ClearPendingInterrupt(Os_IsrCfg[Os_GetObjLocalId(CFG_ISR_ID)].IsrSrc, OS_ARCH_INT_CPU0)

/* ================================================
* Functional Macro Defination Provided to Kernel *
================================================ */

/* Clear Pending Support by  */
#define Os_HAL_INT_IS_CLEARPENDING_SUPPORTED()  (TRUE)

/* Global Interrupt Enable/Disable */
#define Os_Hal_DisableInt()                     OS_ASM("cpsid i")
#define Os_Hal_EnableInt()                      OS_ASM("dsb\n cpsie i\n")

/* Enter Critical Area */
#define OS_HAL_DECLARE_CRITICAL() Os_ArchMsrType msr
#define OS_HAL_ENTRY_CRITICAL() (msr = Os_Hal_SuspendInt()) /* PRQA S 3472 */ /* VL_Os_3472 */
#define OS_HAL_EXIT_CRITICAL() Os_Hal_RestoreInt(msr) /* PRQA S 3472 */ /* VL_Os_3472 */
/* PRQA S 3458 ++ */ /* VL_Os_3458 */
#define OS_HAL_TASK_SWITCH_PROC(coreId)                                         \
        {                                                                       \
            Os_Hal_DisableInt();                                                \
            OS_ARCH_REG_SCB_ICSR |= OS_ARCH_PENDSV_REQUEST_MASK;                \
        }

/* Preparation before handling interrupt Cat 2 */
#define OS_HAL_ISR2_PROLOGUE(isrId)                                                             \
        {                                                                                       \
            OS_ARCH_SET_MSP_STACK(OS_ARCH_STACK_ALIGN((uint32)Os_ISR2Stack[isrId].StackTop));   \
            OS_ARCH_TIME_PROT_ENTER(isrId);                                                     \
            Os_EnterISR2(isrId);                                                                \
            OS_ARCH_ISR_NEST_ENTER(isrId);                                                      \
        }

/* preparation before quiting interrupt Cat 2 */
#define OS_HAL_ISR2_EPILOGUE(isrId)                                             \
        {                                                                       \
            OS_ARCH_SERVER_PROTECTION(isrId);                                   \
            Os_Hal_DisableInt();                                                \
            OS_ARCH_MEM_PROT_EXIT();                                            \
            Os_ExitISR2(isrId);                                                 \
            OS_ARCH_TIME_PROT_EXIT();                                           \
            Os_Arch_DefaultIntExit(Os_Arch_GetMainSP());                        \
        }

/* Preparation before handling interrupt Cat 1 */
#define OS_HAL_ISR1_PROLOGUE(isrId)                                             \
        {                                                                       \
            Os_EnterISR1(isrId);                                                \
            OS_ARCH_ISR_NEST_ENTER(isrId);                                      \
        }

/* preparation before quiting interrupt Cat 1 */
#define OS_HAL_ISR1_EPILOGUE(isrId)                                             \
        {                                                                       \
            Os_Hal_DisableInt();                                                \
            Os_ExitISR1(isrId);                                                 \
            Os_Arch_DefaultIntExit(Os_Arch_GetMainSP());                        \
        }

/* Disable a specified interrupt */
#define OS_Hal_DisableIntApp(src)                                               \
        {                                                                       \
            Os_Hal_DisableInterruptSource(src, Os_Hal_GetCoreID());             \
            Os_Hal_ClearPendingInterrupt(src, Os_Hal_GetCoreID());              \
        }

/* Terminate a Nested ISR operation */
#define OS_HAL_TERMINATE_NESTED_ISR(i)                                          \
        {                                                                       \
            Os_ArchIsrTempIPSR = Os_ArchIsrxPSRNest[i];                         \
            Os_Arch_TerminateISR();                                             \
        }
/* PRQA S 3458 -- */ 
/* Stack Recycling operation */
#define OS_HAL_PROCESS_NESTED_ISR_STACK(index)                                  \
        do                                                                      \
        {                                                                       \
            Os_ArchMainSpNest[index + 1u] = Os_ArchMainSpNest[index + 2u]; /* PRQA S 3410 */ /* VL_Os_3410 */  \
        } while(0)

/* Enable/Diable Specified interrupt */
#define Os_Hal_EnableIntSource(src) Os_Hal_EnableInterruptSource(src, Os_Hal_GetCoreID())
#define Os_Hal_DisableIntSource(src) Os_Hal_DisableInterruptSource(src, Os_Hal_GetCoreID())

/* SC3 or SC4 Used Isr exit */
#define OS_HAL_ISR2_EPILOGUE_KILL_ISR(isrId)     OS_HAL_ISR2_EPILOGUE(isrId)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/* Type defination of Os Isr Handler */
typedef void (*Os_IsrHandler)(void);

/* Type defination of Isr handler installation */
typedef struct
{
    Os_IsrHandler ISR; /* PRQA S 0784 */ /* VL_Os_0784 */
    uint8 IntPrio;
} Os_IntInstallType;

/*=======[E X T E R N A L   D A T A   D E C L A R A T I O N S]================*/

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)

#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
extern boolean OS_ArchHardFaultFlag;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"

#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
extern uint32 Os_ArchMainSpNest[CFG_ISR_MAX];
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h" 

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
extern uint32 Os_ArchIsrxPSRNest[CFG_ISR_MAX];
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
extern uint32 Os_ArchIsrTempIPSR;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
extern Os_IsrHandler Os_ArchExternIrqTable[OS_MCU_NVIC_NUM + OS_NUM_1]; /* PRQA S 3672 */ /* VL_Os_3672 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/* ============================
* Function Provided to Kernel *
============================ */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_ArchMsrType Os_Hal_SuspendInt(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_RestoreInt(Os_ArchMsrType msr);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_IPLType Os_Hal_GetIpl(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_SetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_IsrStateType Os_Hal_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern uint32 Os_Hal_GetCurrentIrq(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* ====================
* Function Local Used *
==================== */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_TerminateISR(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_HardFault_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_PendSV_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Reset_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void NMI_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void MemManage_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void BusFault_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void UsageFault_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_SVC_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void DebugMon_Handler(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_DefaultIntEntry(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_DefaultIntExit(uint32 vMsp);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_ForceInterruptExit(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_InterruptInit(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_InitIntPrio(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_SetPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_SaveMainSpAndXpsr(uint32 vMsp,uint32 vXpsr);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern uint32 Os_Arch_GetMainSP(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern uint32 Os_Arch_GetTerminatedIntMainSP(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern boolean Os_Arch_SystemStackISRSetCheck(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* OS_ARCH_IRQ_H */

/*=======[E N D   O F   F I L E]==============================================*/
