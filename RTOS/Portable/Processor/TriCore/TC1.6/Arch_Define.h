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
**  FILENAME    : Arch_Define.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef ARCH_DEFINE_H
#define ARCH_DEFINE_H

/*=======[I N C L U D E S]=================================================*/
#include "Os_Cfg.h"
#include <machine/intrinsics.h>
#include <machine/wdtcon.h>

/*=======[M A C R O S]=====================================================*/
/*================@+General+@================*/
#define OS_ASM            __asm__ /* Embedded assembly. */
#define CHECK_STACK_USAGE (1U)

#define OS_ALIGN(exp)     __attribute__((aligned(exp)))
#define OS_ARCH_STACK_ALIGN(addr) \
    ((addr) & 0xFFFFFFFCU) /* PRQA S 3472 */ /* MISRA  Dir-4.9 */ /* ARCH_DEFINE_MACRO_001 */

#define OS_REG32(address) \
    (*(volatile uint32*)(address)) /* PRQA S 3472 */ /* MISRA  Dir-4.9 */ /* ARCH_DEFINE_MACRO_001 */

#define OS_ARCH_ISYNC()      OS_ASM("isync")
#define OS_ARCH_DSYNC()      OS_ASM("dsync") /* All data access is served before the next data access. */

#define OS_BEGIN_NO_OPTIMIZE __attribute__((optimize("O0"))) /* Disable Optimize */
#define OS_END_NO_OPTIMIZE

#if ((TRUE == CFG_SYSTEM_TIMER_ENABLE) || (TRUE == CFG_TIMING_PROTECTION_ENABLE))
/* PRQA S 3472 ++*/ /* MISRA  Dir-4.9 */             /* ARCH_DEFINE_MACRO_001 */
#define Os_ArchSetENDINIT()   lock_safety_wdtcon()   /* Lock the safety watchdog. */
#define Os_ArchResetENDINIT() unlock_safety_wdtcon() /* Unlock the safety watchdog. */
/* PRQA S 3472 --*/                                  /* MISRA  Dir-4.9 */
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE || TRUE == CFG_TIMING_PROTECTION_ENABLE */

#define OS_ARCH_MFCR(addr) \
    (Os_ArchMsrType)       \
        _mfcr((addr)) /* Move from core register. Move the contents of the core SFR register to adata register. */
/* Move to core register. Move the value from a data register to the coreSFR */
#define OS_ARCH_MTCR(addr, reg)     \
    {                               \
        (void)_mtcr((addr), (reg)); \
        OS_ARCH_ISYNC();            \
    }

#define Os_GetCurrentSp()                                          \
    do                                                             \
    {                                                              \
        __asm__ volatile("mov.aa  %0, %%a10" : "=a"(CurrentSp)::); \
    } while (0)

/* The interrupt occupies interrupt resources. */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_ISR2OCCUPYINTRES() Os_Isr2OccupyIntRes()
#else /* FALSE == CFG_SERVICE_PROTECTION_ENABLE */
#define OS_ISR2OCCUPYINTRES()
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */

/* Get effective address from register of PCX  */
#define OS_PCX_PCXUL(pcx) Os_Extru((pcx), 22u, 1u)

/* Get effective address from register of PCX */
/* PRQA S 3472 ++*/ /* MISRA  Dir-4.9 */ /* ARCH_PROCESSOR_MACRO_002 */
#define OS_PCX_TO_EA(pcx) ((Os_ArchCsaType*)((Os_Extru((pcx), 16u, 4u) << 28u) | (Os_Extru((pcx), 0u, 16u) << 6u)))

/* Get register of PCX from effective address */
#define OS_EA_TO_PCX(addr) ((Os_Extru((uint32)(addr), 28u, 4u) << 16u) | (Os_Extru((uint32)(addr), 6u, 16u)))
/* PRQA S 3472 --*/ /* MISRA  Dir-4.9 */

/* Get register of PCXI from PCX */
#define PCX_TO_PCXI(pcx) (*((volatile uint32*)OS_PCX_TO_EA((pcx))))

/* Point to the PCX precursor. */
#define OS_PCX_PREV(pcx) (((Os_ArchCsaType*)OS_PCX_TO_EA(pcx))->reg[0] & 0x000FFFFFU)

/* Get the PCXprecursor. */
#define GET_PREV_PCX(nextPcx, pcx)                  \
    do                                              \
    {                                               \
        OS_ARCH_DSYNC();                            \
        (nextPcx) = PCX_TO_PCXI(pcx) & 0x000FFFFFU; \
    } while (0)

/* Set the PCX precursor. */
#define SET_PREV_PCX(pcx, nextPcx) PCX_TO_PCXI((pcx)) = (nextPcx)

/* Point to the CSA precursor. */
#define OS_CSA_PREV(csa)          (Os_ArchCsaType*)OS_PCX_TO_EA(csa->reg[0])

#define Os_ArchPreSwitch2System() /* PRQA S 3219 */ /* MISRA Rule 2.1 */ /*ARCH_PROCESSOR_STATICDEFINE_001*/ \
    {                                                                                                        \
        /* PRQA S 0306++ */ /* MISRA Rule 11.4 */ /*ARCH_PROCESSOR_TYPE_CAST_004*/                           \
        Os_ArchInitCsas();                                                                                   \
        OS_ARCH_MTCR(OS_REG_LCX, OS_EA_TO_PCX((uint32) & Os_Csas[1]));                                       \
        /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /*ARCH_PROCESSOR_H_MACRO_001*/                                 \
        OS_ARCH_MTCR(OS_REG_FCX, OS_EA_TO_PCX((uint32) & Os_Csas[Os_CfgCsaMax - 1u]));                       \
        /* PRQA S 3469 */ /* MISRA Dir 4.9 */ /*ARCH_PROCESSOR_H_MACRO_001*/                                 \
        OS_ARCH_MTCR(OS_REG_PCX, (uint32)0u);                                                                \
        /* PRQA S 0306-- */ /* MISRA Rule 11.4 */                                                            \
    }
/*================@-Operation about PCX and CSA-@================*/

/*================@+Recycle CSA+@================*/
/* Recycle the task's CSA. */
#define Os_ArchReclaimCsas(taskId)                                                             \
    do                                                                                         \
    {                                                                                          \
        OS_PCX_TO_EA(Os_TaskCBExt[taskId].pcxLast)->reg[0] = (uint32)OS_ARCH_MFCR(OS_REG_FCX); \
        OS_ARCH_MTCR(OS_REG_FCX, Os_TaskCBExt[taskId].pcx);                                    \
    } while (0)

/* Isr reclaim csa list */
#define Os_ArchReclaimIsrCsas(pcx, endPcx)                                         \
    do                                                                             \
    {                                                                              \
        Os_TempPcx = (uint32)(pcx);                                                \
        if ((0U != Os_TempPcx) && ((pcx) != (endPcx)))                             \
        {                                                                          \
            Os_LoopPcx = Os_TempPcx;                                               \
            GET_PREV_PCX(Os_NextPcx, Os_LoopPcx);                                  \
            while ((((endPcx) & 0x000FFFFFu) != Os_NextPcx) && (0U != Os_NextPcx)) \
            {                                                                      \
                Os_LoopPcx = Os_NextPcx;                                           \
                GET_PREV_PCX(Os_NextPcx, Os_LoopPcx);                              \
            }                                                                      \
            SET_PREV_PCX(Os_LoopPcx, (uint32)OS_ARCH_MFCR(OS_REG_FCX));            \
            OS_ARCH_MTCR(OS_REG_FCX, Os_TempPcx);                                  \
        }                                                                          \
    } while (0)

/* Reclaim isr_csa in TerminateApplication. */
#define Os_ArchReclaimIsrCsasTermApp(pcx, endPcx)                              \
    do                                                                         \
    {                                                                          \
        Os_TempPcxTermApp = (uint32)OS_ARCH_MFCR(OS_REG_PCX);                  \
        if (0U != Os_TempPcxTermApp)                                           \
        {                                                                      \
            Os_LoopPcxTermApp = Os_TempPcxTermApp;                             \
            GET_PREV_PCX(Os_NextPcxTermApp, Os_LoopPcxTermApp);                \
            while (((endPcx) & 0x000FFFFFu) != Os_NextPcxTermApp)              \
            {                                                                  \
                Os_LoopPcxTermApp = Os_NextPcxTermApp;                         \
                GET_PREV_PCX(Os_NextPcxTermApp, Os_LoopPcxTermApp);            \
                if (((pcx) & 0x000FFFFFu) == Os_NextPcxTermApp)                \
                {                                                              \
                    Os_BeginPcxTermApp = Os_LoopPcxTermApp;                    \
                }                                                              \
            }                                                                  \
            Os_TempPcxTermApp = (uint32)(pcx);                                 \
            SET_PREV_PCX(Os_LoopPcxTermApp, (uint32)OS_ARCH_MFCR(OS_REG_FCX)); \
            OS_ARCH_MTCR(OS_REG_FCX, Os_TempPcxTermApp);                       \
            SET_PREV_PCX(Os_BeginPcxTermApp, Os_NextPcxTermApp);               \
        }                                                                      \
    } while (0)

/* The interrupt did not exit properly */
#define OS_ARCH_RECLAIM_CSAS_WITH_ISR_RECOVERY_EXCEPTION()              \
    do                                                                  \
    {                                                                   \
        if (0U != OS_ARCH_MFCR(OS_REG_PCX))                             \
        {                                                               \
            Os_EndPcx = OS_ARCH_MFCR(OS_REG_PCX);                       \
            while (0U != (Os_EndPcx & 0x00100000u))                     \
            {                                                           \
                Os_EndPcx = PCX_TO_PCXI(Os_EndPcx);                     \
            }                                                           \
            Os_ArchReclaimIsrCsas(OS_ARCH_MFCR(OS_REG_PCX), Os_EndPcx); \
        }                                                               \
    } while (0)
/*================@-Recycle CSA-@================*/

/*================@+Internal Register+@================*/
#define OS_REG_CORE_ID   (0xFE1CU)
#define OS_REG_ICR       (0xFE2cU)
#define OS_REG_FCX       (0xFE38U)
#define OS_REG_PCX       (0xFE00U)
#define OS_REG_LCX       (0xFE3cU)
#define OS_REG_PCXI      (0xFE00U)
#define OS_REG_PC        (0xFE08U)
#define OS_REG_PSW       (0xFE04U)
#define OS_REG_SYSCON    (0xFE14U)
#define OS_REG_ISP       (0xFE28U)
#define OS_REG_DBGSR     (0xFD00U)
#define CONTROL_REGISTER OS_REG_PSW
/*================@-Internal Register-@================*/

/*================@+Error Code+@================*/
#define E_OS_Illegal_Opcode                (25U)
#define E_OS_Unimplemented_Opcode          (26U)
#define E_OS_Invalid_Operand               (27U)
#define E_OS_Data_Address_Align            (28U)
#define E_OS_Invalid_Local_Memory          (29U)
#define E_OS_Program_Fetch_Synchronous     (30U)
#define E_OS_Data_Access_Synchronous       (31U)
#define E_OS_Data_Acess_Asynchronous       (32U)
#define E_OS_Coprocessor_Trap_Asynchronous (33U)
#define E_OS_Program_Memory_Integrity      (34U)
#define E_OS_Data_Memory_Integrity         (35U)
#define E_OS_Temporal_Asynchronous         (36U)
#define E_OS_Arithmetic_Overflow           (37U)
#define E_OS_Sticky_Arithmetic_Overflow    (38U)
#define E_OS_NMI                           (39U)
#define E_OS_Virtual_Address_Fill          (40U)
#define E_OS_Virtual_Address_Protection    (41U)
#define E_OS_Free_Context_List_Depletion   (42U)
#define E_OS_Call_Depth_Overflow           (43U)
#define E_OS_Call_Depth_Underflow          (44U)
#define E_OS_Free_Context_List_Underflow   (45U)
#define E_OS_Call_Stack_Underflow          (46U)
#define E_OS_Context_Type                  (47U)
#define E_OS_Nesting_Error                 (48U)
/*================@-Error Code-@================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Type of the variable to save the mcu status registration. */
typedef uint32 Os_ArchMsrType;
typedef uint32 osCsaUsageType;
/* Type of the variable to save the task control block extension. */
typedef struct
{
    uint32 pcx;
    uint32 pcxLast;
} Os_TaskCBExtType;

/* Type of the variable to save data. */
typedef struct
{
    uint32 reg[16];
} Os_ArchCsaType;

/*=======[E X T E R N A L   D A T A]========================================*/
extern P2VAR(Os_TaskCBExtType, AUTOMATIC, OS_VAR) Os_TaskCBExt;
extern VAR(volatile uint32, OS_VAR) Os_TempPcxTermApp;
extern VAR(volatile uint32, OS_VAR) Os_LoopPcxTermApp;
extern VAR(volatile uint32, OS_VAR) Os_NextPcxTermApp;
extern VAR(volatile uint32, OS_VAR) Os_BeginPcxTermApp;
extern VAR(volatile uint32, OS_VAR) Os_TempPcx;
extern VAR(volatile uint32, OS_VAR) Os_LoopPcx;
extern VAR(volatile uint32, OS_VAR) Os_EndPcx;
extern VAR(uint32, OS_VAR) Os_NextPcx;
extern P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR) Os_Csas;
extern VAR(uint16, OS_VAR) Os_CfgCsaMax;
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
extern uint32 Os_Extru(uint32 a, uint32 p, uint32 w);

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
extern FUNC(void, OS_CODE) Os_ArchInitCsas(void);

/*=======[I N T E R N A L   D A T A]========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

#endif /* ARCH_DEFINE_H */
/*=======[E N D   O F   F I L E]============================================*/
