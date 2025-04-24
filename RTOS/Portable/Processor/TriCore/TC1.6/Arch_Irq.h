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

/*=======[I N C L U D E S]=================================================*/
#include "Os_Internal.h"

/*=======[M A C R O S]=====================================================*/
#define Os_ArchRemoteClear()

/*================@+Maximum Number of Interrupt and Trap Requests+@================*/
#define OS_MAX_INTRS (256u)

/*================@-Maximum Number of Interrupt and Trap Requests-@================*/
#define Os_ArchDisableInt()        (void)_disable() /* Disables global interrupt. */
#define Os_ArchEnableInt()         (void)_enable()  /* Enables global interrupt. */

#define OS_ARCH_DECLARE_CRITICAL() Os_ArchMsrType msr
#define OS_ARCH_ENTRY_CRITICAL() \
    Os_ArchSuspendInt(&msr) /* PRQA S 3472 */ /* MISRA  Dir-4.9 */ /* ARCH_DEFINE_MACRO_001 */
#define OS_ARCH_EXIT_CRITICAL() \
    Os_ArchRestoreInt(msr) /* PRQA S 3472 */ /* MISRA  Dir-4.9 */ /* ARCH_DEFINE_MACRO_001 */

#define OS_ARCH_SUSPEND_ALLINT() Os_ArchSuspendInt(&msr)
#define OS_ARCH_RESTORE_ALLINT() Os_ArchRestoreInt(msr)

#define Os_ArchSwitch2System()                                                              \
    {                                                                                       \
        Os_ArchSp = (volatile uint32)OS_ARCH_STACK_ALIGN((uint32)Os_SystemStack->stackTop); \
        OS_ASM("movh.a %a10, hi:Os_ArchSp");                                                \
        OS_ASM("ld.a %a10, [%a10]lo:Os_ArchSp");                                            \
    }

/* Switch to ISR2 stack */
#if (CFG_ISR2_MAX > 0U)
#define Os_ArchSwitch2ISR2Stk(isrId)                                           \
    {                                                                          \
        Os_ArchSp = OS_ARCH_STACK_ALIGN((uint32)Os_ISR2Stack[isrId].stackTop); \
        OS_ASM("movh.a %a10, hi:Os_ArchSp");                                   \
        OS_ASM("ld.a %a10, [%a10]lo:Os_ArchSp");                               \
    }
#else /* #if CFG_ISR2_MAX == 0U */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */ /*ARCH_DEFINE_H_COMMENTS_001*/
#define Os_ArchSwitch2ISR2Stk(isrId)
#endif /* CFG_ISR2_MAX > 0U */

/*================@+Operation About Context+@================*/
/* Save the context. */
#define OS_ARCH_SAVE_CONTEXT() /* PRQA S 3472 */ /* MISRA  Dir-4.9 */ /* ARCH_PROCESSOR_MACRO_002 */ \
    do                                                                                               \
    {                                                                                                \
        Os_TaskCBExt[Os_SCB.sysRunningTaskID].pcx = OS_ARCH_MFCR(OS_REG_PCX);                        \
    } while (0)

/* Resume the context. */
/* Operation about resume context */
#define OS_ARCH_RESUME_CONTEXT_1()    \
    do                                \
    {                                 \
        OS_ASM("j Os_ResumeContext"); \
    } while (0)

#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
#define OS_ARCH_RESUME_CONTEXT() OS_ARCH_RESUME_CONTEXT_1()
#else /* OS_SC3 == CFG_SC || OS_SC4 == CFG_SC */
#define OS_ARCH_RESUME_CONTEXT()                            \
    do                                                      \
    {                                                       \
        OS_ARCH_RECLAIM_CSAS_WITH_ISR_RECOVERY_EXCEPTION(); \
        OS_ARCH_RESUME_CONTEXT_1();                         \
    } while (0)
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */

/*================@-Operation About Context-@================*/

/*================@+Interrupt Enter and Exit+@================*/

/*================@+ISR1 ENTER+@================*/
/* Some operations to be performed when ISR1 enters an interrupt. */
#define OS_ARCH_ISR1_PROLOGUE_1(isrId)                            \
    do                                                            \
    {                                                             \
        if (TRUE == CFG_INT_NEST_ENABLE)                          \
        {                                                         \
            if (0U == Os_IntNestISR1)                             \
            {                                                     \
                Os_ArchSwitch2System();                           \
                Os_SaveLevelISR1 = Os_SCB.sysOsLevel;             \
                Os_SCB.sysOsLevel = OS_LEVEL_ISR1;                \
            }                                                     \
        }                                                         \
        else                                                      \
        {                                                         \
            Os_ArchSwitch2System();                               \
            Os_SaveLevelISR1 = Os_SCB.sysOsLevel;                 \
            Os_SCB.sysOsLevel = OS_LEVEL_ISR1;                    \
        }                                                         \
        Os_IntCfgIsrId = isrId;                                   \
        Os_EnterISR1();                                           \
        if (TRUE == CFG_INT_NEST_ENABLE)                          \
        {                                                         \
            Os_IntNestISR1++;                                     \
            if (TRUE == Os_IsrCfg[Os_IntCfgIsrId].OsNestedEnable) \
            {                                                     \
                Os_ArchEnableInt();                               \
            }                                                     \
        }                                                         \
    } while (0)

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_ARCH_ISR1_PROLOGUE(isrId)    \
    do                                  \
    {                                   \
        Os_ArchSetMemProtSet(0x00UL);   \
        OS_ARCH_ISR1_PROLOGUE_1(isrId); \
        Os_UpdatePsw();                 \
    } while (0)
#else
#define OS_ARCH_ISR1_PROLOGUE(isrId) OS_ARCH_ISR1_PROLOGUE_1(isrId)
#endif
/*================@+ISR1 ENTER+@================*/

/*================@+ISR1 EXIT+@================*/
/* Some operations to be performed when ISR1 exits an interrupt. */
#if (TRUE == CFG_INT_NEST_ENABLE)
#define OS_ARCH_ISR1_EPILOGUE()        \
    do                                 \
    {                                  \
        Os_ArchDisableInt();           \
        Os_ExitISR1();                 \
        Os_IntNestISR1--;              \
        OS_ASM("j Os_ReturnFromISR1"); \
    } while (0)

#else /* FALSE == CFG_INT_NEST_ENABLE */
#define OS_ARCH_ISR1_EPILOGUE()        \
    do                                 \
    {                                  \
        Os_ExitISR1();                 \
        OS_ASM("j Os_ReturnFromISR1"); \
    } while (0)
#endif /* TRUE == CFG_INT_NEST_ENABLE */
/*================@+ISR1 EXIT+@================*/

/*================@+ISR2 ENTER+@================*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_TMPROTISRFRAMECHK()                            \
    do                                                    \
    {                                                     \
        if (E_OK != Os_TmProtIsrFrameChk(Os_IntCfgIsrId)) \
        {                                                 \
            Os_ExitISR2();                                \
            OS_ARCH_DSYNC();                              \
            OS_ASM("rslcx");                              \
            OS_ASM("nop");                                \
            OS_ASM("rfe");                                \
        }                                                 \
    } while (0)
#else
#define OS_TMPROTISRFRAMECHK()
#endif

#define OS_ARCH_ISR2_PROLOGUE_1(isrId)                                               \
    do                                                                               \
    {                                                                                \
        if ((TRUE == CFG_TIMING_PROTECTION_ENABLE) || (TRUE == CFG_INT_NEST_ENABLE)) \
        {                                                                            \
            Os_IsrNestPcxStack[Os_IntNestISR2] = OS_ARCH_MFCR(OS_REG_PCX);           \
            if (0U == Os_IntNestISR2)                                                \
            {                                                                        \
                OS_ARCH_SAVE_CONTEXT();                                              \
            }                                                                        \
        }                                                                            \
        else                                                                         \
        {                                                                            \
            OS_ARCH_SAVE_CONTEXT();                                                  \
        }                                                                            \
        Os_ArchSwitch2ISR2Stk(isrId);                                                \
        Os_IntCfgIsrId = isrId;                                                      \
        Os_EnterISR2();                                                              \
        OS_TMPROTISRFRAMECHK();                                                      \
        if ((TRUE == CFG_TIMING_PROTECTION_ENABLE) || (TRUE == CFG_INT_NEST_ENABLE)) \
        {                                                                            \
            if (TRUE == Os_IsrCfg[Os_IntCfgIsrId].OsNestedEnable)                    \
            {                                                                        \
                Os_ArchEnableInt();                                                  \
            }                                                                        \
        }                                                                            \
    } while (0)

/* Some operations to be performed when ISR2 enters an interrupt. */
#if ((OS_SC4 == CFG_SC) || (OS_SC3 == CFG_SC))
#define OS_ARCH_ISR2_PROLOGUE(isrId)    \
    do                                  \
    {                                   \
        OS_ARCH_ISR2_PROLOGUE_1(isrId); \
        Os_UpdatePsw();                 \
    } while (0)
#else /* OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */
#define OS_ARCH_ISR2_PROLOGUE(isrId) OS_ARCH_ISR2_PROLOGUE_1(isrId)
#endif /* OS_SC4 == CFG_SC || OS_SC3 == CFG_SC */
/*================@+ISR2 ENTER+@================*/

/*================@+ISR2 EXIT+@================*/
/* Some operations that need to be performed when exiting the ISR2. */
#define OS_ARCH_ISR2_EPILOGUE_KILL_ISR() OS_ARCH_ISR2_EPILOGUE()

#if ((OS_NOSC == CFG_SC) || (OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC))
#define OS_ARCH_ISR2_EPILOGUE() OS_ARCH_ISR2_EPILOGUE_1()
#else /* OS_SC4 == CFG_SC || OS_SC3 == CFG_SC */
#if (TRUE == CFG_SRV_SHELLOS)
#define OS_ARCH_ISR2_EPILOGUE()    \
    do                             \
    {                              \
        (void)_syscall(2u);        \
        OS_ARCH_ISR2_EPILOGUE_1(); \
    } while (0)

#else /* FALSE == CFG_SRV_SHELLOS */
#define OS_ARCH_ISR2_EPILOGUE() OS_ARCH_ISR2_EPILOGUE_1()
#endif /* TRUE == CFG_SRV_SHELLOS */
#endif /* OS_NOSC == CFG_SC || OS_SC1 == CFG_SC || OS_SC2 == CFG_SC */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_ARCH_ISR2_EPILOGUE_1()      \
    do                                 \
    {                                  \
        OS_ISR2OCCUPYINTRES();         \
        Os_ArchDisableInt();           \
        OS_ASM("j Os_ReturnFromISR2"); \
    } while (0)
#elif (TRUE == CFG_INT_NEST_ENABLE)
#define OS_ARCH_ISR2_EPILOGUE_1()         \
    do                                    \
    {                                     \
        OS_ISR2OCCUPYINTRES();            \
        Os_ArchDisableInt();              \
        Os_ExitISR2();                    \
        if (0U == Os_IntNestISR2)         \
        {                                 \
            Os_CLEAR_CDC_ISR2_EPILOGUE(); \
            OS_ARCH_RESUME_CONTEXT();     \
        }                                 \
        OS_ARCH_DSYNC();                  \
        OS_ASM("rslcx");                  \
        OS_ASM("nop");                    \
        OS_ASM("rfe");                    \
    } while (0)
#else
#define OS_ARCH_ISR2_EPILOGUE_1()     \
    do                                \
    {                                 \
        OS_ISR2OCCUPYINTRES();        \
        Os_ExitISR2();                \
        Os_CLEAR_CDC_ISR2_EPILOGUE(); \
        OS_ARCH_RESUME_CONTEXT();     \
    } while (0)
#endif
/*================@+ISR2 EXIT+@================*/

/*================@+Interrupt Enter and Exit+@================*/

/*=======[T Y P E   D E F I N I T I O N S]==================================*/

/*=======[E X T E R N A L   D A T A]========================================*/
extern VAR(Os_isrhnd, OS_VAR) Cdisptab[OS_MAX_INTRS];
extern VAR(uint32, OS_VAR) TriCore_int_table[];
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern VAR(volatile boolean, OS_VAR) Os_TprotTerminateIsr;
extern VAR(volatile boolean, OS_VAR) Os_TprotTerminateTask;
#endif
extern P2VAR(uint32, AUTOMATIC, OS_VAR) Os_IsrNestPcxStack;

#if (CFG_ISR_MAX > 0U)
extern VAR(Os_CallLevelType, OS_VAR) Os_SaveLevelISR1;
#endif
extern VAR(volatile uint32, OS_VAR) Os_ArchSp;
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
/* Timing protection. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/******************************************************************************/
/*
 * Brief                <Disable all interrupts except for those related to time protection.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_DisableAllInterrupts,Os_SuspendAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchDisableAllInt_ButTimingProtInt(void);

/******************************************************************************/
/*
 * Brief                <Enable all interrupts except for the time protection interrupt.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_EnableAllInterrupts,Os_ResumeAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchEnableAllInt_ButTimingProtInt(void);
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

/******************************************************************************/
/*
 * Brief                <Initialize interrupts and traps>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InterruptInit(void);

/******************************************************************************/
/*
 * Brief                <get the IPL>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <0>
 * PreCondition         <None>
 * CallByAPI            <SuspendOSInterrupts and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(Os_IPLType, OS_CODE) Os_ArchGetIpl(void);
/******************************************************************************/
/*
 * Brief                <Ipl greater than 0, disable interrupt otherwise enable
 *                       interrupt>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeOSInterrupts and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchSetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc);
/******************************************************************************/
/*
 * Brief                <Save ccr to variable msr and disable maskable interrupt>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchSuspendInt(P2VAR(Os_ArchMsrType, AUTOMATIC, OS_VAR) msr);
/* PRQA S 3432 */ /* MISRA Rule 20.7 */ /* ARCH_IRQ_COMPILER_001 */
/******************************************************************************/
/*
 * Brief                <transfer variable msr back to ccr>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchRestoreInt(Os_ArchMsrType msr);

/*Default interrupt service function.*/
extern FUNC(void, OS_CODE) Os_IntHandler(void);

extern FUNC(void, OS_CODE) Os_ResumeContext(void);
extern FUNC(void, OS_CODE) Os_ReturnFromISR1(void);

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern FUNC(void, OS_CODE) Os_ReturnFromISR2(void);
#endif

extern FUNC(void, OS_CODE) Os_UpdatePsw(void);

/*=======[I N T E R N A L   D A T A]========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#endif /* ARCH_IRQ_H */
/*=======[E N D   O F   F I L E]==============================================*/
