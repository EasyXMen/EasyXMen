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
/*
 ********************************************************************************
 **                                                                            **
 **  FILENAME    :  Os_Arch_Irq.h                                              **
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
#include "Os_Kernel2Port.h"
#include "Os_Arch_Define.h"
#include "Os_Cfg.h"
#include "Os_Appl_CfgData.h"
#include "Os_Task_CfgData.h"
#include "Os_Mcu_Irq.h"
/*=======[M A C R O S]=====================================================*/
#if (OS_AUTOSAR_CORES > 1U)
#define OS_TMPROT_POSITION 2
#else
#define OS_TMPROT_POSITION 1
#endif

/* EI Level Interrupt Control Register */
/* PRQA S 3472 ++*/ /* VL_Os_3472*/
#define OS_INTC_EICn(n)                                                     \
    (*(volatile uint16*)(((n) < 32U) ? ((OS_INTC1_SELF_BASE_ADDR) + ((n) * 0x02U))    \
                                        : ((OS_INTC2_BASE_ADDR) + ((n) * 0x02U))))

#define OS_EIC_P_FEILD_MASK   (uint16)(0xFU)   /* These bits specify the interrupt priority*/
#define OS_EIC_TB_BIT         6U     /* TB BIT */
#define OS_EIC_MK_BIT         7U     /* MK BIT */
#define OS_EIC_RF_BIT         12U    /* RF BIT */

/*The specification defines that an offset of 0x1000 should be added to the interrupt 
numbers at the EI level to differentiate them from interrupts at the FE level.
For more details, refer to Os_Mcu_Irq.h in the MCU section. */
#define OS_EIINT_TYPE          (0x1000U)
#define OS_FEINTSRC_2_NUM(src) ((src) & 0x001FU)
#define OS_EIINTSRC_2_NUM(src) ((src) & 0x0FFFU)
#define OS_FEINTFMSK_MASK (0xFFFFFFFFUL)

#define Os_HAL_INT_IS_CLEARPENDING_SUPPORTED() (TRUE)

#define OS_ISR1_MAX_NESTED_DEPTH (32U)

#define OS_PSW_GET_ID_BIT (0x00000020UL)

#define OS_EIIC_GET_FEILD (0x0FFFUL)
#define OS_FEIC_GET_FEILD (0x00FFUL)

#define INT_PRIORITY_MAX (16U)

/* Physical priority to logical priority (INT_PRIORITY_MAX-1 ~ 0   -    1 - INT_PRIORITY_MAX) */

#define INT_PRIORITY_PHY_TO_LOG(pri) (INT_PRIORITY_MAX - (pri))

/* Logical priority to physical priority*/
/* Based on the design of os interrupt control system, the interrupt priority
 * corresponds to the interrupt group.
 * logic interrupt priority: 1   . . .   16
 * cpu interrupt priority  : 15  . . .   0
 */
#define INT_PRIORITY_LOG_TO_PHY(pri) ((uint16)(INT_PRIORITY_MAX - (pri)))

/* Set PSW.ID(bit5) = 1, PSW.NP(bit7) = 1 */
#define Os_Hal_DisableInt() OS_ARCH_REG_WRITE(OS_PSW_NUM, (OS_ARCH_REG_READ(OS_PSW_NUM) | 0xA0UL))

/* Set PSW.ID(bit5) = 0, PSW.NP(bit7) = 0 */
#define Os_Hal_EnableInt() OS_ARCH_REG_WRITE(OS_PSW_NUM, (OS_ARCH_REG_READ(OS_PSW_NUM) & 0xFFFFFF5FUL))

#define OS_HAL_DECLARE_CRITICAL() Os_ArchMsrType msr
#define OS_HAL_ENTRY_CRITICAL()   (msr = Os_Hal_SuspendInt())
#define OS_HAL_EXIT_CRITICAL()    Os_Hal_RestoreInt(msr)

/* Switch Task */
#define OS_HAL_TASK_SWITCH_PROC(coreid) Os_SwitchTask(coreid)

/*=======[I N T E R N A L   M A C R O]========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define Os_ArchMemProtSwith2User()                                                    \
    do                                                                                \
    {                                                                                 \
        if ((TRUE != Os_AppCfg[Os_Kernel_GetSysRunningAppID()].Trusted)                      \
            || (TRUE == Os_AppCfg[Os_Kernel_GetSysRunningAppID()].TrustedAppWithProtection)) \
        {                                                                             \
            Os_ArchSwith2UserMode();                                                  \
        }                                                                             \
    } while (0)
#define Os_ArchMemProtSwith2Kernel()                                                  \
    do                                                                                \
    {                                                                                 \
        if ((TRUE != Os_AppCfg[Os_Kernel_GetSysRunningAppID()].Trusted)                      \
            || (TRUE == Os_AppCfg[Os_Kernel_GetSysRunningAppID()].TrustedAppWithProtection)) \
        {                                                                             \
            Os_ArchSwith2SvMode();                                                    \
        }                                                                             \
    } while (0)
#else
#define Os_ArchMemProtSwith2User()
#define Os_ArchMemProtSwith2Kernel()
#endif

/* switch to ISR2 stack for RH850 */
#if (CFG_ISR2_MAX > 0U)
#define Os_ArchSwitch2ISR2Stk(isrId)                                               \
    do                                                                             \
    {                                                                              \
        Os_ArchTempSp = OS_ARCH_STACK_ALIGN((uint32)Os_ISR2Stack[isrId].StackTop); \
        OS_SET_SP_VAL();                                                           \
    } while (0)
#endif

#define OS_PUSH_STACK()                                                 \
    do                                                                  \
    {                                                                   \
        if (((uint8)0) == Os_Kernel_GetIntNestNumISR2())                        \
        {                                                               \
            Os_TaskCBExt[Os_Kernel_GetSysRunningTaskID()] = Os_ArchTempSp; \
        }                                                               \
        else                                                            \
        {                                                               \
            Os_IsrNestStack[Os_Kernel_GetIntNestNumISR2()] = Os_ArchTempSp; \
        }                                                               \
    } while (0)

#define OS_POP_STACK()                                                  \
    do                                                                  \
    {                                                                   \
        if (((uint8)0) == Os_Kernel_GetIntNestNumISR2())                        \
        {                                                               \
            Os_ArchTempSp = Os_TaskCBExt[Os_Kernel_GetSysRunningTaskID()]; \
        }                                                               \
        else                                                            \
        {                                                               \
            Os_ArchTempSp = Os_IsrNestStack[Os_Kernel_GetIntNestNumISR2()];\
        }                                                               \
    } while (0)

#if (TRUE == CFG_INT_NEST_ENABLE)
#define OS_ISR_NEST_ENABLE(isrId)                  \
    do                                             \
    {                                              \
        if (TRUE == Os_IsrCfg[isrId].NestedEnable) \
        {                                          \
            Os_Hal_EnableInt();                    \
        }                                          \
    } while (0)

#define OS_ISR_NEST_EXIT(isrId)                    \
    do                                             \
    {                                              \
        if (TRUE == Os_IsrCfg[isrId].NestedEnable) \
        {                                          \
            Os_Hal_DisableInt();                   \
        }                                          \
    } while (0)
#else
#define OS_ISR_NEST_ENABLE(isrId)
#define OS_ISR_NEST_EXIT(isrId)
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_TPISR2_ENTER(isrId)                   \
    do                                           \
    {                                            \
        if (E_OK != Os_TmProtIsrFrameChk(isrId)) \
        {                                        \
            Os_ExitISR2(isrId);                  \
            Os_ISR2Exit();                       \
        }                                        \
    } while (0)
#define OS_TPTERMINATE_ISR()                                                         \
    do                                                                               \
    {                                                                                \
        if (OS_PROTECTION_TERMINATE_TPROT_ISR ==                                     \
            Os_Kernel_GetAndComparTerminateObject(OS_PROTECTION_TERMINATE_TPROT_ISR))\
        {                                                                            \
            Os_ArchReclaimIsrCsasTermApp(OS_TMPROT_POSITION);                        \
        }                                                                            \
    } while (0)
#else
#define OS_TPISR2_ENTER(isrId)
#define OS_TPTERMINATE_ISR()
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
#define OS_SERVER_PROTECTION(isrId) Os_Isr2OccupyIntRes(isrId)
#else
#define OS_SERVER_PROTECTION(isrId)
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */

#define OS_PRE_EXIT_ISR2() \
do { \
    Os_ISR2Exit(); \
} while (0)
#define OS_PRE_EXIT_ISR1(isrId)           \
    do                                    \
    {                                     \
        if (0 != (Os_IsrCfg[isrId].IsrSrc & OS_EIINT_TYPE)) \
        {                                 \
            Os_ISR1Exit();                \
        }                                 \
        else                              \
        {                                 \
            Os_FeISRExit();               \
        }                                 \
    } while (0)

#define OS_Hal_DisableIntApp(src)                               \
    do                                                          \
    {                                                           \
        Os_Hal_DisableInterruptSource(src, Os_Hal_GetCoreID()); \
        Os_Hal_ClearPendingInterrupt(src, Os_Hal_GetCoreID());  \
    } while (0)

#define OS_ARCH_ISR2_PROLOGUE(isrId)  \
    do                                \
    {                                 \
        OS_PUSH_STACK();              \
        Os_ArchSwitch2ISR2Stk(isrId); \
        OS_TPISR2_ENTER(isrId);       \
        Os_EnterISR2(isrId);          \
        OS_ISR_NEST_ENABLE(isrId);    \
        Os_ArchMemProtSwith2User();   \
    } while (0)

#define OS_ARCH_ISR2_EPILOGUE(isrId)  \
    do                                \
    {                                 \
        Os_ArchMemProtSwith2Kernel(); \
        OS_SERVER_PROTECTION(isrId);  \
        OS_ISR_NEST_EXIT(isrId);      \
        Os_ExitISR2(isrId);           \
        OS_TPTERMINATE_ISR();         \
        OS_POP_STACK();               \
        OS_PRE_EXIT_ISR2();           \
    } while (0)

#define OS_ARCH_ISR1_PROLOGUE(isrId)                       \
    do                                                     \
    {                                                      \
        Os_ISR1SP[Os_Kernel_GetIntNestNumISR1()] = Os_ArchTempSp; \
        if (0U == Os_Kernel_GetIntNestNumISR1())                  \
        {                                                  \
            Os_Hal_Switch2System();                        \
        }                                                  \
        Os_EnterISR1(isrId);                               \
        OS_ISR_NEST_ENABLE(isrId);                         \
    } while (0)

#define OS_ARCH_ISR1_EPILOGUE(isrId)                       \
    do                                                     \
    {                                                      \
        OS_ISR_NEST_EXIT(isrId);                           \
        Os_ExitISR1(isrId);                                \
        Os_ArchTempSp = Os_ISR1SP[Os_Kernel_GetIntNestNumISR1()]; \
        OS_PRE_EXIT_ISR1(isrId);                           \
    } while (0)

#define OS_HAL_ISR2_EPILOGUE_KILL_ISR(isrId) \
    do                                       \
    {                                        \
        Os_ArchReclaimIsrCsasTermApp(isrId); \
        OS_ARCH_ISR2_EPILOGUE(isrId);        \
    } while (0)

#if (TRUE == CFG_PROTECTIONHOOK)
#define OS_HAL_ISR2_EPILOGUE(isrId) OS_ARCH_ISR2_EPILOGUE(isrId)
#endif


#define OS_HAL_TERMINATE_NESTED_ISR(index) \
do { \
    Os_ArchReclaimIsrCsasTermApp(isrId); \
} while (0)
#define OS_HAL_PROCESS_NESTED_ISR_STACK(index)                       \
do {                                                                 \
    Os_IsrNestStack[(index) + 1u] = Os_IsrNestStack[(index) + 2u];       \
} while (0)
/* PRQA S 3472 --*/ /* VL_Os_3472 */
/*=======[E X T E R N A L   D A T A]==========================================*/
/* PRQA S 3672 ++ */ /* VL_Os_3672 */
extern Os_isrhnd Os_FE_Handler[OS_ARCH_FE_INTR_NUM];
extern Os_isrhnd Os_Vector_Handler[OS_ARCH_EI_INTR_NUM];
/* PRQA S 3672 -- */
/* PRQA S 3432 ++ */ /*VL_Os_3432 */
extern uint32 Os_IsrNestStack[CFG_ISR_MAX];
extern uint32 Os_ISR1SP[OS_ISR1_MAX_NESTED_DEPTH];
/* PRQA S 3432-- */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/


/* 1. ASM function and exception function*/
extern void            Os_ExceptionTable(void);
extern void            Os_ISR2Exit(void);
extern void            Os_ISR1Exit(void);
extern void            Os_FeISRExit(void);
extern void            Os_MipMdp(void);
extern void            Os_FENMI(void);
extern void            Os_FEIntHandler(void);
/* 2. Interrupt initialization and installation */
extern void            Os_InterruptInit(void);
/* 3. Root cause management of interrupts, overall management*/
extern void            Os_Hal_SetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc);
extern Os_IPLType      Os_Hal_GetIpl(void);
extern Os_ArchMsrType  Os_Hal_SuspendInt(void);
extern void            Os_Hal_RestoreInt(Os_ArchMsrType msr);
/* 4. Management of individual interrupt sources */
extern void            Os_Hal_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern void            Os_Hal_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern void            Os_Hal_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
extern void            Os_Hal_SetPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
extern Os_IsrStateType Os_Hal_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType);
/* 5. Current interrupt and exception information */
extern uint32          Os_Hal_GetCurrentIrq(void);
extern uint32          Os_Hal_GetCurrentExc(void);

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
extern void Os_Hal_ExceptionInformationParse(Os_TrapContextType* TrapContext);
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
extern void Os_ArchReclaimIsrCsasTermApp(uint32 isrId);
#endif

#endif
/*=======[E N D   O F   F I L E]==============================================*/
