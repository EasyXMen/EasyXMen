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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    :  Arch_Irq.h                                                 **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Deal with operations related to processor interrupts        **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
#ifndef ARCH_IRQ_H
#define ARCH_IRQ_H

/*=======[I N C L U D E S]=================================================*/
#include "Os_Internal.h"
/*=======[M A C R O S]=====================================================*/
#define INT_PRIORITY_MAX (16U)

/* Physical priority to logical priority (INT_PRIORITY_MAX-1 ~ 0   -    1 - INT_PRIORITY_MAX) */
#define INT_PRIORITY_PHY_TO_LOG(pri) (INT_PRIORITY_MAX - (pri))

/* Logical priority to physical priority*/
/* Based on the design of os interrupt control system, the interrupt priority
 * corresponds to the interrupt group.
 * logic interrupt priority: 1   . . .   16
 * cpu interrupt priority  : 15  . . .   0
 */
#define INT_PRIORITY_LOG_TO_PHY(pri) (INT_PRIORITY_MAX - (pri))

#define OS_ISR1_MAX_NESTED_DEPTH     (32U)

/* Set PSW.ID(bit5) = 1, PSW.NP(bit7) = 1 */
#define Os_ArchDisableInt() OS_ARCH_REG_WRITE(OS_PSW_NUM, (OS_ARCH_REG_READ(OS_PSW_NUM) | 0xA0U))

/* Set PSW.ID(bit5) = 0, PSW.NP(bit7) = 0 */
#define Os_ArchEnableInt() OS_ARCH_REG_WRITE(OS_PSW_NUM, (OS_ARCH_REG_READ(OS_PSW_NUM) & 0xFFFFFF5FU))

/*=======[Exception and Interrupt   M A C R O]========================================*/
/*
 * control the specific interrupt source, based on the service request control register
 */
/* UnInstall interrupt */
#define OS_INTERRUPT_UNSTALL(src) OS_INTERRUPT_DISABLEREQ(src)

#define Os_FEINT_ClearFlags(val)

#define Os_FEINT_UNSTALL(val)

/* set the bit7 EICn.MKn(request mask) */
#define OS_INTERRUPT_ENABLEREQ(irqNum) (OS_INTC_SELF_EICn(irqNum) &= ~0x0080)

/* clear the bit7 EICn.MKn(request mask) */
#define OS_INTERRUPT_DISABLEREQ(irqNum) (OS_INTC_SELF_EICn(irqNum) |= 0x0080)

/* check the bit7 EICn.MKn(request mask) */
#define OS_INTERRUPT_CHKREQ(irqNum) !((OS_INTC_SELF_EICn(irqNum) >> 12) & 0x01)

/* write 0 to the bit12 EICn.RFn(request flag) */
#define OS_INTERRUPT_CLEARREQ(irqNum) (OS_INTC_SELF_EICn(irqNum) &= 0xEFFF)

/* write 1 to the bit12 EICn.RFn(request flag) */
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_INTERRUPT_SETREQ(irqNum)            \
    {                                          \
        OS_ENTER_KERNEL();                     \
        (OS_INTC_SELF_EICn(irqNum) |= 0x1000); \
        OS_EXIT_KERNEL();                      \
    }
#else
#define OS_INTERRUPT_SETREQ(irqNum) (OS_INTC_SELF_EICn(irqNum) |= 0x1000)
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#define OS_ARCH_ISR2_PROLOGUE(isrId)  \
    {                                 \
        OS_PUSH_STACK();              \
        OS_SAVE_ISR_ID(isrId);        \
        Os_ArchMemProtDisable();      \
        Os_ArchSwitch2ISR2Stk(isrId); \
        Os_EnterISR2();               \
        OS_TPISR2_ENTER(isrId);       \
        Os_ArchMemProtEnable();       \
        OS_ISR_NEST_ENABLE(isrId);    \
    }

#define OS_ARCH_ISR2_EPILOGUE() \
    {                           \
        Os_ArchDisableInt();    \
        Os_ArchSwitch2System(); \
        OS_SERVER_PROTECTION(); \
        Os_ExitISR2();          \
        OS_POP_STACK();         \
        OS_PRE_EXIT_ISR();      \
    }

#define OS_ARCH_ISR1_PROLOGUE(isrId)                 \
    {                                                \
        Os_ISR1SP[Os_IntNestISR1] = Os_ArchTempSp;   \
        if (0U == Os_IntNestISR1)                    \
        {                                            \
            Os_ArchMemProtDisable();                 \
            Os_SaveLevelISR1 = Os_SCB.sysOsLevel;    \
            Os_SCB.sysOsLevel = OS_LEVEL_ISR1;       \
            Os_ArchSwitch2System();                  \
        }                                            \
        Os_EnterISR1();                              \
        Os_IntNestISR1++;                            \
        if (TRUE == Os_IsrCfg[isrId].OsNestedEnable) \
        {                                            \
            Os_ArchEnableInt();                      \
        }                                            \
    }

#define OS_ARCH_ISR1_EPILOGUE()                    \
    {                                              \
        Os_ArchDisableInt();                       \
        Os_ExitISR1();                             \
        Os_IntNestISR1--;                          \
        Os_ArchTempSp = Os_ISR1SP[Os_IntNestISR1]; \
        if (0U == Os_IntNestISR1)                  \
        {                                          \
            Os_SCB.sysOsLevel = Os_SaveLevelISR1;  \
            Os_ArchMemProtEnable();                \
        }                                          \
        Os_ISR1Exit();                             \
    }

#define OS_ARCH_FE_PROLOGUE(isrId)                 \
    {                                              \
        Os_ISR1SP[Os_IntNestISR1] = Os_ArchTempSp; \
        if (0U == Os_IntNestISR1)                  \
        {                                          \
            Os_ArchMemProtDisable();               \
            Os_SaveLevelISR1 = Os_SCB.sysOsLevel;  \
            Os_SCB.sysOsLevel = OS_LEVEL_ISR1;     \
            Os_ArchSwitch2System();                \
        }                                          \
        Os_EnterISR1();                            \
        Os_IntNestISR1++;                          \
    }

#define OS_ARCH_FE_EPILOGUE()                      \
    {                                              \
        Os_ExitISR1();                             \
        Os_IntNestISR1--;                          \
        Os_ArchTempSp = Os_ISR1SP[Os_IntNestISR1]; \
        if (0U == Os_IntNestISR1)                  \
        {                                          \
            Os_SCB.sysOsLevel = Os_SaveLevelISR1;  \
            Os_ArchMemProtEnable();                \
        }                                          \
        OS_PRE_EXIT_FEISR();                       \
    }

#define OS_ARCH_ISR2_EPILOGUE_KILL_ISR()                                \
    {                                                                   \
        Os_ArchReclaimIsrCsasTermApp(0, 0, Os_SCB.sysRunningIsrCat2Id); \
        OS_ARCH_ISR2_EPILOGUE();                                        \
    }
/*=======[I N T E R N A L   M A C R O]========================================*/
#define OS_PUSH_STACK()                                            \
    {                                                              \
        if (0U == Os_IntNestISR2)                                  \
        {                                                          \
            Os_TaskCBExt[Os_SCB.sysRunningTaskID] = Os_ArchTempSp; \
        }                                                          \
        else                                                       \
        {                                                          \
            Os_IsrNestPcxStack[Os_IntNestISR2] = Os_ArchTempSp;    \
        }                                                          \
    }

#define OS_POP_STACK()                                             \
    {                                                              \
        if (0U == Os_IntNestISR2)                                  \
        {                                                          \
            Os_ArchTempSp = Os_TaskCBExt[Os_SCB.sysRunningTaskID]; \
        }                                                          \
        else                                                       \
        {                                                          \
            Os_ArchTempSp = Os_IsrNestPcxStack[Os_IntNestISR2];    \
        }                                                          \
    }

#define OS_SAVE_ISR_ID(isrId)   \
    {                           \
        Os_IntCfgIsrId = isrId; \
    }

#if (TRUE == CFG_INT_NEST_ENABLE)
#define OS_ISR_NEST_ENABLE(isrId)                    \
    {                                                \
        if (TRUE == Os_IsrCfg[isrId].OsNestedEnable) \
        {                                            \
            Os_ArchEnableInt();                      \
        }                                            \
    }

#define OS_ISR_NEST_EXIT(isrId)                      \
    {                                                \
        if (TRUE == Os_IsrCfg[isrId].OsNestedEnable) \
        {                                            \
            Os_ArchDisableInt();                     \
        }                                            \
    }
#else
#define OS_ISR_NEST_ENABLE(isrId)
#define OS_ISR_NEST_EXIT(isrId)
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_TPISR2_ENTER(isrId)                   \
    {                                            \
        if (E_OK != Os_TmProtIsrFrameChk(isrId)) \
        {                                        \
            Os_ExitISR2();                       \
            Os_ISR2Exit();                       \
        }                                        \
    }
#else
#define OS_TPISR2_ENTER(isrId)
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_SERVER_PROTECTION() Os_Isr2OccupyIntRes()
#else
#define OS_SERVER_PROTECTION()
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */

#define OS_PRE_EXIT_ISR()   Os_ISR2Exit();
#define OS_PRE_EXIT_FEISR() Os_FeISRExit();
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* type of an Interrupt Service Routine (ISR) */
typedef uint32 Os_ArchMsrType;
typedef uint32* Os_ArchMsrRefType;
/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
extern P2VAR(uint32, AUTOMATIC, OS_VAR) Os_IsrNestPcxStack;
extern P2VAR(uint32, AUTOMATIC, OS_VAR) Os_ISR1SP;
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

extern VAR(uint32, OS_VAR) Os_IsrFE_SP;

#if (CFG_ISR_MAX > 0U)
extern VAR(Os_CallLevelType, OS_VAR) Os_SaveLevelISR1;
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern VAR(volatile uint32, OS_VAR) Os_TprotTerminateIsr;
#endif
extern VAR(uint32, OS_VAR) InitInterruptFlag;
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern FUNC(void, OS_CODE) Os_ArchSetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc);
extern FUNC(Os_IPLType, OS_CODE) Os_ArchGetIpl(void);
extern FUNC(void, OS_CODE) Os_ArchSuspendInt(Os_ArchMsrRefType msr);
extern FUNC(void, OS_CODE) Os_ArchRestoreInt(Os_ArchMsrType msr);

extern FUNC(void, OS_CODE) Os_IntHandler(void);
extern FUNC(void, OS_CODE) Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(void, OS_CODE) Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(boolean, OS_CODE) Os_ArchIntIsClearPendingSupported(void);

extern FUNC(void, OS_CODE) Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(Os_IsrStateType, OS_CODE) Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType);

extern FUNC(void, OS_CODE) Os_ArchInitIntPrio(void);

extern FUNC(void, OS_CODE) Os_ExceptionTable(void);
extern FUNC(void, OS_CODE) Os_ISR2Exit(void);
extern FUNC(void, OS_CODE) Os_ISR1Exit(void);
extern FUNC(void, OS_CODE) Os_FeISRExit(void);

extern FUNC(void, OS_CODE) Os_MipMdp(void);
extern FUNC(void, OS_CODE) Os_FENMI(void);
extern FUNC(void, OS_CODE) Os_FEIntHandler(void);

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern FUNC(void, OS_CODE) Os_ArchDisableAllInt_ButTimingProtInt(void);
extern FUNC(void, OS_CODE) Os_ArchEnableAllInt_ButTimingProtInt(void);
#endif /* (TRUE == CFG_TIMING_PROTECTION_ENABLE) */

#endif
/*=======[E N D   O F   F I L E]==============================================*/
