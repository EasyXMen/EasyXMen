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
 *
 ************************************************************************************************************************
 **
 **  @file               : Os_Arch_Define.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Arch header file for Arch type definition
 **
 ***********************************************************************************************************************/

#ifndef ARCH_DEFINE_H
#define ARCH_DEFINE_H

/*=======[I N C L U D E S]=================================================*/
#include "Os_Cfg.h"
#include "Os_Arch_Compiler.h"

/*=======[M A C R O S]=====================================================*/
/*------------------Compiler OS-------------------------*/
#define OS_ALIGN(exp) __attribute__((aligned(exp))) /* Align by exp bytes. */
#define OS_REG32(address) (*(volatile uint32*)(address)) /* Define 32-bit register. */ /* PRQA S 3472 */ /* VL_Os_3472 */
/*------------------End Compiler OS-------------------------*/

#define OS_REG_CUS_ID  (0xFE50U)
#define OS_REG_CORE_ID (0xFE1CU)
#define OS_REG_ICR     (0xfe2cU)
#define OS_REG_FCX     (0xfe38U)
#define OS_REG_PCX     (0xfe00U)
#define OS_REG_LCX     (0xfe3cU)
#define OS_REG_PCXI    (0xfe00U)
#define OS_REG_PC      (0xfe08U)
#define OS_REG_PSW     (0xfe04U)
#define OS_REG_SYSCON  (0xfe14U)
#define OS_REG_ISP     (0xFE28U)
#define OS_REG_DBGSR   (0xFD00U)
#define OS_REG_BIV		 (0xFE20U)					
#define OS_REG_BTV		 (0xFE24U)

/*! Bit mask (bits 29..22) for previous CPU priority number */
#define OS_HAL_PCXI_PCPN_MASK (0x3FC00000UL)
/*! Start bit position of PCPN for shift operation */
#define OS_HAL_PCXI_PCPN_BIT_POSITION (22U)
/*! Start bit position of PIE for shift operation */
#define OS_HAL_PCXI_PIE_BIT_POSITION (21U)
/*! Bit mask (bit 21) for previous interrupt state */
#define OS_HAL_PCXI_PIE_MASK (0x00200000UL)
/*! Bit mask (bit 8) for GW (Global Address Register Write Permission) */
#define OS_HAL_PSW_GW_MASK (0x00000100UL)

#define PCX_MASK (0x000FFFFFU)
#define ALIGN_4BYTES_MASK (0xFFFFFFFCU)
#define PCXI_UL_BIT (20U)
#define PCXI_PIE_BIT (21U)
#define PCXI_UL_MASK (0x00100000U)

/*=======[Porting Macro]======================================================*/
#define OS_ARCH_STACK_ALIGN(addr) ((addr) & ALIGN_4BYTES_MASK) /* Align stack space to 4 bytes. */ /* PRQA S 3472 */ /* VL_Os_3472 */
/* All preceding instructions are executed by the CPU. Then the pipeline is flushed
before the next instruction is executed */
#define OS_ARCH_ISYNC() OS_ASM("isync")
#define OS_ARCH_DSYNC() OS_ASM("dsync") /* All data access is served before the next data access. */

#define Os_GetCurrentSp() /* PRQA S 3458 */ /* VL_Os_3458 */       \
    do                    \
    {                     \
        OS_ARCH_GET_SP(); \
    } while (0)

/* get effective address from register of PCX  */
#define OS_PCX_PCXUL(pcx) Os_ArchExtru((pcx), 22U, 1U)

/* Get effective address from register of PCX */
#define OS_PCX_TO_EA(pcx) ((Os_ArchCsaType*)((Os_ArchExtru((pcx), 16U, 4U) << 28U) | (Os_ArchExtru((pcx), 0U, 16U) << 6U))) /* PRQA S 3472 */ /* VL_Os_3472 */

/* Get register of PCX from effective address */
#define OS_EA_TO_PCX(addr) ((Os_ArchExtru((uint32)(addr), 28U, 4U) << 16U) | (Os_ArchExtru((uint32)(addr), 6U, 16U))) /* PRQA S 3472 */ /* VL_Os_3472 */

/* Get register of PCXI from PCX */
#define PCX_TO_PCXI(pcx) (*((volatile uint32*)OS_PCX_TO_EA((pcx))))

/* Point to the CSA precursor. */
#define OS_CSA_PREV(csa) ((Os_ArchCsaType*)OS_PCX_TO_EA(csa->reg[0])) /* PRQA S 3410 */ /* VL_Os_3410 */

/* Point to the PCX precursor. */
#define OS_PCX_PREV(pcx) (((Os_ArchCsaType*)OS_PCX_TO_EA(pcx))->reg[0] & PCX_MASK)

/* PRQA S 3458 ++ */ /* VL_Os_3458 */
/* Get the PCXprecursor. */
#define GET_PREV_PCX(nextPcx, pcx)                  \
    {                                               \
        OS_ARCH_DSYNC();                            \
        (nextPcx) = PCX_TO_PCXI(pcx) & PCX_MASK; \
    }

/* Set the PCX precursor. */
#define SET_PREV_PCX(pcx, nextPcx) (PCX_TO_PCXI((pcx)) = (nextPcx))

/* Get Upper or Lower Context Tag from register of PCX */
#define OS_PCX_TO_UL(pcx) ((pcx & PCXI_UL_MASK) >> PCXI_UL_BIT) /* PRQA S 3410 */ /* VL_Os_3410 */

/* Recycle the task's CSA. */
#define Os_ArchReclaimCsas(taskId)                                                             \
    {                                                                                          \
        OS_PCX_TO_EA(Os_TaskCBExt[taskId].pcxLast)->reg[0] = (uint32)OS_ARCH_MFCR(OS_REG_FCX); \
        OS_ARCH_MTCR(OS_REG_FCX, Os_TaskCBExt[taskId].pcx);                                    \
    }

/* Isr reclaim csa list */
#define Os_ArchReclaimIsrCsas(pcx, endPcx)                                         \
    {                                                                              \
        uint32 Os_TempPcx = (uint32)(pcx);                                         \
        uint32 Os_LoopPcx, Os_NextPcx;                                             \
        if ((0U != Os_TempPcx) && ((pcx) != (endPcx)))                             \
        {                                                                          \
            Os_LoopPcx = Os_TempPcx;                                               \
            GET_PREV_PCX(Os_NextPcx, Os_LoopPcx);                                  \
            while ((((endPcx) & PCX_MASK) != Os_NextPcx) && (0U != Os_NextPcx)) \
            {                                                                      \
                Os_LoopPcx = Os_NextPcx;                                           \
                GET_PREV_PCX(Os_NextPcx, Os_LoopPcx);                              \
            }                                                                      \
            SET_PREV_PCX(Os_LoopPcx, (uint32)OS_ARCH_MFCR(OS_REG_FCX));            \
            OS_ARCH_MTCR(OS_REG_FCX, Os_TempPcx);                                  \
        }                                                                          \
    }

/* Reclaim isr_csa in TerminateApplication. */
#define Os_ArchReclaimIsrCsasTermApp(pcx, endPcx)                              \
    {                                                                          \
        Os_TempPcxTermApp = (uint32)OS_ARCH_MFCR(OS_REG_PCX);                  \
        if (0U != Os_TempPcxTermApp)                                           \
        {                                                                      \
            Os_LoopPcxTermApp = Os_TempPcxTermApp;                             \
            GET_PREV_PCX(Os_NextPcxTermApp, Os_LoopPcxTermApp);                \
            while (((endPcx) & PCX_MASK) != Os_NextPcxTermApp)              \
            {                                                                  \
                Os_LoopPcxTermApp = Os_NextPcxTermApp;                         \
                GET_PREV_PCX(Os_NextPcxTermApp, Os_LoopPcxTermApp);            \
                if (((pcx) & PCX_MASK) == Os_NextPcxTermApp)                \
                {                                                              \
                    Os_BeginPcxTermApp = Os_LoopPcxTermApp;                    \
                }                                                              \
            }                                                                  \
            Os_TempPcxTermApp = (uint32)(pcx);                                 \
            SET_PREV_PCX(Os_LoopPcxTermApp, (uint32)OS_ARCH_MFCR(OS_REG_FCX)); \
            OS_ARCH_MTCR(OS_REG_FCX, Os_TempPcxTermApp);                       \
            SET_PREV_PCX(Os_BeginPcxTermApp, Os_NextPcxTermApp);               \
        }                                                                      \
    }

/* The interrupt did not exit properly */
/* PRQA S 3412 ++ */ /* VL_Os_3412 */
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#define OS_ARCH_RECLAIM_CSAS_WITH_ISR_RECOVERY_EXCEPTION() \
    do                                                     \
    {                                                      \
        uint32 startpcx = OS_ARCH_MFCR(OS_REG_PCX);        \
        if (0U != startpcx)                                \
        {                                                  \
            uint32 endpcx = 0U;                            \
            endpcx        = startpcx;                      \
            while (0U != (endpcx & PCXI_UL_MASK))           \
            {                                              \
                endpcx = PCX_TO_PCXI(endpcx);              \
            }                                              \
            Os_ArchReclaimIsrCsas(startpcx, endpcx);       \
        }                                                  \
    } while (0);

#define Os_ArchProtectionHookTerminateNestedIsr()                                                     \
    do                                                                                                \
    {                                                                                                 \
        OS_ARCH_RECLAIM_CSAS_WITH_ISR_RECOVERY_EXCEPTION()                                            \
        Os_CLEAR_CDC_ISR2_EPILOGUE();                                                                 \
        OS_ARCH_MTCR(OS_REG_PCX, Os_IsrNestPcxStack[Os_Kernel_GetIntNestNumISR2()]);                  \
    } while (0);

#else
#define OS_ARCH_RECLAIM_CSAS_WITH_ISR_RECOVERY_EXCEPTION()
#define Os_ArchProtectionHookTerminateNestedIsr()
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define Os_ArchTprotTerminateIsr()                                                                    \
    do                                                                                                \
    {                                                                                                 \
        uint8 isr2NestNum     = Os_Kernel_GetIntNestNumISR2();                                        \
        Os_ArchReclaimIsrCsas(Os_IsrNestPcxStack[isr2NestNum + 1U], Os_IsrNestPcxStack[isr2NestNum]); \
        OS_ARCH_MTCR(OS_REG_PCX, Os_IsrNestPcxStack[isr2NestNum]);                                    \
    } while (0);

#define Os_ArchTprotTerminateTask()                       \
    do                                                    \
    {                                                     \
        if (OS_PROTECTION_TERMINATE_TASK == Os_Kernel_GetAndComparTerminateObject(OS_PROTECTION_TERMINATE_TASK))          \
        {                                                 \
            Os_ArchReclaimCsas(Os_Kernel_GetSysRunningTaskID()); \
        }                                                 \
    } while (0);
/* PRQA S 3412 -- */
#else
#define Os_ArchTprotTerminateIsr()
#define Os_ArchTprotTerminateTask()
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

/* PRQA S 3458 -- */

#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
/* The macro is true when the hardware timer uses a compare register; otherwise, it is false. */
#define OS_HAL_SYSTEM_TIMER_IS_USED_CMP     TRUE
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */

/*=======[I N T E R N A L   M A C R O]========================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Type of the variable to save the mcu status registration. */
typedef uint32 Os_ArchMsrType;

/* Type of the variable to save the task control block extension. */
typedef struct
{
    uint32 pcx;
    uint32 pcxLast;
} Os_TaskCBExtType;

/* Type of the variable to save data. */
typedef struct
{
    uint32 reg[16]; /* PRQA S 3132 */ /* VL_QAC_MagicNum */
} Os_ArchCsaType;

/*=======[E X T E R N A L   D A T A]========================================*/
extern Os_TaskCBExtType* Os_TaskCBExt;
extern volatile uint32   Os_TempPcxTermApp;
extern volatile uint32   Os_LoopPcxTermApp;
extern volatile uint32   Os_NextPcxTermApp;
extern volatile uint32   Os_BeginPcxTermApp;
extern Os_ArchCsaType*   Os_Csas;
extern uint16            Os_CfgCsaMax;
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
/******************************************************************************/
/*
 * Brief                <Assembly shift operation.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Gtm_GetTbuChannelStatus>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1006++ */ /* VL_Os_1006 */
static inline OS_ALWAYS_INLINE uint32 Os_ArchExtru(uint32 a, uint32 p, uint32 w) /* PRQA S 3206, 3008, 3006 */ /* VL_Os_3206 */ /* VL_Os_3008, VL_Os_3006 */
{
    uint32 res;
    OS_ARCH_EXTRU(res, a, p, w); /* PRQA S 3138 */ /*VL_Os_3138 */
    return res; /* PRQA S 2961 */ /*VL_Os_2961 */
}
/* PRQA S 1006-- */
/*=======[I N T E R N A L   D A T A]========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

#endif /* OS_DEFINE_H */
/*=======[E N D   O F   F I L E]============================================*/
