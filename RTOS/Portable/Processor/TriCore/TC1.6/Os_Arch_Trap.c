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
 **  @file               : Os_Arch_Trap.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Arch source file for Trap API implementations
 **
 ***********************************************************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Task.h"
#include "Os_Interrupt.h"
#include "Os_ProtectHook.h"
#include "Os_Kernel2Port.h"
#include "Os_FaultManager.h"
#include "Os_Adapt.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
/* TriCore trap table */
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
/* This array holds the functions to be called when a trap occurs. */
Os_traphnd Tdisptab[MAX_TRAPS]; /* PRQA S 3672, 1533 */ /* VL_Os_3672 */ /* VL_Os_1533 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
volatile uint32 Os_ArchTrapSp; /* PRQA S 1502 */ /* VL_Os_1502 */
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
extern Os_ExceptionContextType Os_HalContextExceptionContext[]; /* PRQA S 3684 */ /* VL_Os_3684 */

static char* Os_TrapMsg = "NULL"; /* PRQA S 3122, 0753, 3625 */ /* VL_Os_3122 */ /* VL_Os_0753 */ /* VL_Os_3625 */
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/* This is the default trap vector table, which consists of eight
   entries, each consisting of eight words (32 bytes).  The table
   must be put into the section ".traptab", must be aligned to a
   256-byte boundary, and the entry symbol (trap class 0 -- reset)
   must be called "TriCore_trap_table", as it is referenced below
   in order to program the BTV register.  */
/* PRQA S 1006 ++ */ /* VL_Os_1006 */
OS_ARCH_DECLARE_TRAP_TAB()

DEFINE_TRAP(0); /* trap class 0 (Reset) */
DEFINE_TRAP(1); /* trap class 1 (Internal Protection Traps) */
DEFINE_TRAP(2); /* trap class 2 (Instruction Errors) */
DEFINE_TRAP(3); /* trap class 3 (Context Management) */
DEFINE_TRAP(4); /* trap class 4 (System Bus and Peripheral Errors) */
DEFINE_TRAP(5); /* trap class 5 (Assertion Traps) */
DEFINE_TRAP(6); /* trap class 6 (System Call) */
DEFINE_TRAP(7); /* trap class 7 (Non-Maskable Interrupt) */

OS_ARCH_DECLARE_TEXT_SECTION();

/*=======[E X C I P T I O N]==================================================*/
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3120 ++ */                                                                           /* VL_QAC_MagicNum */
static void Os_SaveCallStack(uint32 pcx, Os_ExceptionContextRefType context) /* PRQA S 3450 */ /* VL_Os_3450 */
{
    Os_ArchCsaType* CSA        = OS_PCX_TO_EA(pcx); /* PRQA S 0306, 3678 */ /* VL_Os_0306 */ /* VL_Os_3678 */
    boolean         UL_CONTEXT = OS_PCX_TO_UL(pcx);
    /* PRQA S 4542, 4461, 3432, 1841 */ /* VL_Os_4542 */ /* VL_Os_4461 */ /* VL_Os_3432 */ /* VL_Os_1841 */
    uint32 pcxi = pcx;

    for (uint8 i = 0; i < OS_STACK_TRACE_NUMBER; i++) /* PRQA S 1290 */ /* VL_Os_1290 */
    {
        /* Upper or Lower Context Tag */
        if (UL_CONTEXT != 0u)
        {
            /* Read the reg[3](RA register) in Upper Context */
            context->CallStack[i] = CSA->reg[3];
        }
        else
        {
            /* Read the reg[1](RA register) in low Context */
            context->CallStack[i] = CSA->reg[1];
        }
        pcxi       = CSA->reg[0];
        CSA        = OS_PCX_TO_EA(pcxi); /* PRQA S 0306 */ /* VL_Os_0306 */
        UL_CONTEXT = OS_PCX_TO_UL(pcxi);
        /* PRQA S 4461, 3432, 1841 */ /* VL_Os_4461 */ /* VL_Os_3432 */ /* VL_Os_1841 */
    }
}
/* PRQA S 3120 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
void Os_HalExceptionSaveContext(const void *trapMsg) /* PRQA S 3006 */ /* VL_Os_3006 */
{
    uint32 pcx = ((Os_ArchTrapMsgType *)trapMsg)->Pcx;
    Os_ExceptionContextRefType context;
    Os_ArchCsaType*            lowCSA; /* PRQA S 3678 */   /* VL_Os_3678 */
    Os_ArchCsaType*            upperCSA; /* PRQA S 3678 */ /* VL_Os_3678*/

    Os_RegisterType AddressRegisters0 = 0UL;
    Os_RegisterType AddressRegisters1 = 0UL;
    Os_RegisterType AddressRegisters8 = 0UL;
    Os_RegisterType AddressRegisters9 = 0UL;

    lowCSA   = OS_PCX_TO_EA(pcx); /* PRQA S 0306 */                        /*VL_Os_0306*/
    upperCSA = OS_CSA_PREV(lowCSA); /* PRQA S 0306, 3432 */ /*VL_Os_0306*/ /*VL_Os_3432*/

    context = &Os_HalContextExceptionContext[Os_McuGetCoreID()];

    /* Get global address registers */
    OS_ARCH_GET_GLOBAL_ADDR();

    /* Save all lower context registers. */
    context->AddressRegisters[2] = lowCSA->reg[2];
    context->AddressRegisters[3] = lowCSA->reg[3];
    context->DataRegisters[0]    = lowCSA->reg[4];
    context->DataRegisters[1]    = lowCSA->reg[5];
    context->DataRegisters[2]    = lowCSA->reg[6];
    context->DataRegisters[3]    = lowCSA->reg[7];
    context->AddressRegisters[4] = lowCSA->reg[8];
    context->AddressRegisters[5] = lowCSA->reg[9];
    context->AddressRegisters[6] = lowCSA->reg[10];
    context->AddressRegisters[7] = lowCSA->reg[11];
    context->DataRegisters[4]    = lowCSA->reg[12];
    context->DataRegisters[5]    = lowCSA->reg[13];
    context->DataRegisters[6]    = lowCSA->reg[14];
    context->DataRegisters[7]    = lowCSA->reg[15];
    context->Ra                  = lowCSA->reg[1];

    /* Save the previous interrupt state. */
    context->Pie  = (lowCSA->reg[0] & OS_HAL_PCXI_PIE_MASK) >> OS_HAL_PCXI_PIE_BIT_POSITION;
    context->Pcpn = (lowCSA->reg[0] & OS_HAL_PCXI_PCPN_MASK) >> OS_HAL_PCXI_PCPN_BIT_POSITION;

    /* Save all upper context registers. */
    context->Psw                  = upperCSA->reg[1];
    context->AddressRegisters[10] = upperCSA->reg[2];
    context->AddressRegisters[11] = upperCSA->reg[3];
    context->DataRegisters[8]     = upperCSA->reg[4];
    context->DataRegisters[9]     = upperCSA->reg[5];
    context->DataRegisters[10]    = upperCSA->reg[6];
    context->DataRegisters[11]    = upperCSA->reg[7];
    context->AddressRegisters[12] = upperCSA->reg[8];
    context->AddressRegisters[13] = upperCSA->reg[9];
    context->AddressRegisters[14] = upperCSA->reg[10];
    context->AddressRegisters[15] = upperCSA->reg[11];
    context->DataRegisters[12]    = upperCSA->reg[12];
    context->DataRegisters[13]    = upperCSA->reg[13];
    context->DataRegisters[14]    = upperCSA->reg[14];
    context->DataRegisters[15]    = upperCSA->reg[15];

    /* Save the additional information. */
    context->ExceptionSource     = ((Os_ArchTrapMsgType *)trapMsg)->Tin;
    context->AddressRegisters[0] = AddressRegisters0;
    context->AddressRegisters[1] = AddressRegisters1;
    context->AddressRegisters[8] = AddressRegisters8;
    context->AddressRegisters[9] = AddressRegisters9;

    context->MpuRegionForStackLow   = (uint32)OS_ARCH_MFCR(OS_DPR_0L);
    context->MpuRegionForStackUpper = (uint32)OS_ARCH_MFCR(OS_DPR_0U);

    /* Save the call stack */
    Os_SaveCallStack(pcx, context);
}
/* PRQA S 3120 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 3120 ++ */                                                   /* VL_QAC_MagicNum*/
void Os_HalExceptionResumeContext(const void *trapMsg) /* PRQA S 3006 */ /*VL_Os_3006*/
{
    Os_ExceptionContextRefType context; /* PRQA S 3679 */ /*VL_Os_3679*/
    uint32 pcx = ((Os_ArchTrapMsgType *)trapMsg)->Pcx;

    Os_ArchCsaType* lowCSA;
    Os_ArchCsaType* upperCSA;

    lowCSA   = OS_PCX_TO_EA(pcx); /* PRQA S 0306 */                        /*VL_Os_0306*/
    upperCSA = OS_CSA_PREV(lowCSA); /* PRQA S 0306, 3432 */ /*VL_Os_0306*/ /*VL_Os_3432*/

    context = &Os_HalContextExceptionContext[Os_McuGetCoreID()];

    /* Resume all lower context registers. */
    lowCSA->reg[1]  = context->Ra;
    lowCSA->reg[2]  = context->AddressRegisters[2];
    lowCSA->reg[3]  = context->AddressRegisters[3];
    lowCSA->reg[4]  = context->DataRegisters[0];
    lowCSA->reg[5]  = context->DataRegisters[1];
    lowCSA->reg[6]  = context->DataRegisters[2];
    lowCSA->reg[7]  = context->DataRegisters[3];
    lowCSA->reg[8]  = context->AddressRegisters[4];
    lowCSA->reg[9]  = context->AddressRegisters[5];
    lowCSA->reg[10] = context->AddressRegisters[6];
    lowCSA->reg[11] = context->AddressRegisters[7];
    lowCSA->reg[12] = context->DataRegisters[4];
    lowCSA->reg[13] = context->DataRegisters[5];
    lowCSA->reg[14] = context->DataRegisters[6];
    lowCSA->reg[15] = context->DataRegisters[7];

    /* Restore the previous interrupt state. */
    lowCSA->reg[0] = (lowCSA->reg[0] & (~OS_HAL_PCXI_PIE_MASK) & (~OS_HAL_PCXI_PCPN_MASK))
                     | (context->Pie << OS_HAL_PCXI_PIE_BIT_POSITION)
                     | (context->Pcpn << OS_HAL_PCXI_PCPN_BIT_POSITION);

    /* Resume all upper context registers. */
    upperCSA->reg[1]  = context->Psw;
    upperCSA->reg[2]  = context->AddressRegisters[10];
    upperCSA->reg[3]  = context->AddressRegisters[11];
    upperCSA->reg[4]  = context->DataRegisters[8];
    upperCSA->reg[5]  = context->DataRegisters[9];
    upperCSA->reg[6]  = context->DataRegisters[10];
    upperCSA->reg[7]  = context->DataRegisters[11];
    upperCSA->reg[8]  = context->AddressRegisters[12];
    upperCSA->reg[9]  = context->AddressRegisters[13];
    upperCSA->reg[10] = context->AddressRegisters[14];
    upperCSA->reg[11] = context->AddressRegisters[15];
    upperCSA->reg[12] = context->DataRegisters[12];
    upperCSA->reg[13] = context->DataRegisters[13];
    upperCSA->reg[14] = context->DataRegisters[14];
    upperCSA->reg[15] = context->DataRegisters[15];

    /* Resume the additional information. */
    OS_ARCH_MTCR(OS_REG_PSW, OS_ARCH_MFCR(OS_REG_PSW) | (OS_HAL_PSW_GW_MASK))
    OS_ARCH_RESUME_INFO();
}
/* PRQA S 3120 -- */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <exception information parse.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Trap function>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
void Os_Hal_ExceptionInformationParse(Os_TrapContextType* TrapContext)
{
    TrapContext->TrapMsg.ExceptionAddress       = TrapContext->TrapRegs.Ra;
    TrapContext->TrapMsg.ExceptionSP            = TrapContext->TrapRegs.AddressRegisters[10];
    TrapContext->TrapMsg.ExceptionSysteamStatus = TrapContext->TrapRegs.Psw;
    TrapContext->TrapMsg.ExceptionCase          = TrapContext->TrapRegs.DataRegisters[15];
    TrapContext->TrapMsg.ExceptionCaseMsg       = Os_TrapMsg;
    /* PRQA S 3120 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

/******************************************************************************/
/*
 * Brief                <Call PendSV to switch task.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_SystemCallTrap>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 3006, 1503, 6070, 6030 ++ */ /*VL_Os_3006, VL_QAC_NoUsedApi, VL_MTR_Os_STCAL, VL_MTR_Os_STMIF */
void Os_ArchDispatcher(void)
/* PRQA S 3006, 1503, 6070, 6030 -- */
{
    OS_ARCH_SAVE_CONTEXT(Os_Kernel_GetSysRunningTaskID()) /* PRQA S 3455 */ /*VL_Os_3455*/
    Os_Hal_Switch2System() /* PRQA S 0306, 1252 */ /*VL_Os_3006*/    /*VL_Os_1252*/
    /* !!! Don't use local variable after switching to system stack !!! */
    {
        OS_ARCH_MTCR(OS_REG_PCX, 0U)

        Os_TaskStateType taskState = Os_Kernel_GetSysRunningTaskState();
        /* avoid switch to task self */
        if ((OS_TASK_STATE_SUSPENDED == taskState) || (OS_TASK_STATE_START == taskState))
        {
            Os_ArchReclaimCsas(Os_Kernel_GetSysRunningTaskID()) /* PRQA S 0306, 3455, 3464 */ /*VL_Os_0306*/ /*VL_Os_3455*/ /*VL_Os_3464*/
        }

        Os_SwitchTask(Os_McuGetCoreID());
        
        /* PRQA S 0310, 0306, 3455 ++ *//* VL_Os_0310, VL_Os_0306, VL_Os_3455 */
        /* PRQA S 3138, 3442, 2743 ++ *//* VL_Os_3138, VL_Os_3442, VL_Os_2743 */
        /* PRQA S 2995 ++ */ /* VL_Os_2995 */
        OS_ARCH_RESUME_CONTEXT(Os_Kernel_GetSysRunningTaskID());
        /* PRQA S 2995 -- */
        /* PRQA S 0310, 0306, 3455 -- */
        /* PRQA S 3138, 3442, 2743 -- */
    }
}
/******************************************************************************/
/*
 * Brief                <trap 6>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_SystemCallTrap(uint32 Os_Tin) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
#if (TRUE == CFG_SRV_SHELLOS)
    Os_ArchCsaType* lowCsa; /* PRQA S 3678 */   /* VL_Os_3678*/
    Os_ArchCsaType* upperCsa; /* PRQA S 3678 */ /* VL_Os_3678*/

    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    lowCsa   = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_PCX));
    upperCsa = OS_CSA_PREV(lowCsa); /* PRQA S 3432 */ /* VL_Os_3432 */
    /* PRQA S 0306 -- */
#endif                   /* TRUE == CFG_SRV_SHELLOS */
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
    switch (Os_Tin & 0xFFFFUL)
    {
    case 0UL:
        /*Tasking should save lower csa, GNU save the lower csa before in the OS_ASM where regedit exception*/
        OS_ASM("j Os_ArchDispatcher");
        break;

#if (TRUE == CFG_SRV_SHELLOS)
    case 1UL:
        upperCsa->reg[1] = PSW_DEFAULT_VALUE | 0x1u; /* PSW  */
        OS_ARCH_DSYNC();
        OS_ASM("rslcx");
        OS_ASM("nop");
        OS_ASM("rfe");
        break;
#endif /* TRUE == CFG_SRV_SHELLOS */
#if (TRUE == CFG_SRV_SHELLOS)
    case 2UL:
        upperCsa->reg[1] &= (PSWIO_MASK & PSW_PRS_MASK); /* PSW  */ /* PRQA S 1252 */ /* VL_Os_1252 */
        upperCsa->reg[1] |= PSWIO_SUPERVISOR_MODE; /* PSW  */ /* PRQA S 1252 */     /* VL_Os_1252 */
        OS_ARCH_DSYNC();
        OS_ASM("rslcx");
        OS_ASM("nop");
        OS_ASM("rfe");
        break;
#endif /* TRUE == CFG_SRV_SHELLOS */
    /*add comments to pass QAC.*/
    default: /* PRQA S 2016 */ /*VL_Os_2016*/
        break;
    }
    /* PRQA S 3120 -- */
}

/******************************************************************************/
/*
 * Brief                <trap 0>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
void Trap0(uint32 Os_Tin) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    uint32     pcx = OS_ARCH_MFCR(OS_REG_PCX);
    StatusType errType;
    /* PRQA S 3120 ++ */                         /* VL_QAC_MagicNum */
    switch (Os_Tin & 0xFFFFUL) /* PRQA S 3120 */ /* VL_QAC_MagicNum*/
    {
    /* Virtual Address Fill */
    case 1UL:
        errType = E_OS_Virtual_Address_Fill;
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
        Os_TrapMsg = "Virtual Address Fill"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
        break;

    /* Virtual Address Protection */
    case 2UL:
        errType = E_OS_Virtual_Address_Protection;
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
        Os_TrapMsg = "Virtual Address Protection"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
        break;

    default:
        errType = E_OS_PROTECTION_EXCEPTION;
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
        Os_TrapMsg = "MMU Error"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
        break;
    }
    /* PRQA S 3120 -- */

    Os_ArchTrapMsgType trapMsg = {pcx, Os_Tin};
    Os_ExceptionHandler((void *)&trapMsg, errType);/* PRQA S 0314 */  /* VL_Os_0314 */
    OS_ARCH_DSYNC(); /* PRQA S 2880 */ /* VL_Os_2880 */
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}

/******************************************************************************/
/*
 * Brief                <trap 1>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
void Trap1(uint32 Os_Tin) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    uint32     pcx = OS_ARCH_MFCR(OS_REG_PCX);
    StatusType errType;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    switch (Os_Tin & 0xFFFFUL)
    {
    case 1UL:
        errType = E_OS_PROTECTION_EXCEPTION;
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
        Os_TrapMsg = "Privileged Instruction"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
        break;

    case 2UL: /* Memory Protection Read */
    case 3UL: /* Memory Protection Write */
    case 4UL: /* Memory Protection Execution */
    case 5UL: /* Memory Protection Peripheral Access */
    case 6UL: /* Memory Protection Peripheral Null Access */
        errType = E_OS_PROTECTION_MEMORY;
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
        Os_TrapMsg = "Memory Protection Error"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
        break;

    case 7UL: /* Global Register Write Protection */
        errType = E_OS_PROTECTION_EXCEPTION;
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
        Os_TrapMsg = "Global Register Write Protection"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
        break;

    default:
        errType = E_OS_PROTECTION_EXCEPTION;
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
        Os_TrapMsg = "Internal Protection Traps"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
        break;
    }
    /* PRQA S 3120 -- */
    Os_ArchTrapMsgType trapMsg = {pcx, Os_Tin};
    Os_ExceptionHandler((void *)&trapMsg, errType);/* PRQA S 0314 */  /* VL_Os_0314 */
    OS_ARCH_DSYNC(); /* PRQA S 2880 */ /* VL_Os_2880 */
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}

/******************************************************************************/
/*
 * Brief                <trap 2>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
void Trap2(uint32 Os_Tin) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    uint32     pcx = OS_ARCH_MFCR(OS_REG_PCX);
    StatusType errType;

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
    Os_TrapMsg = "Instruction Errors"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    switch (Os_Tin & 0xFFFFUL)
    {
    /* Illegal Opcode */
    case 1UL:
        errType = E_OS_Illegal_Opcode;
        break;

    /* Unimplemented Opcode */
    case 2UL:
        errType = E_OS_Unimplemented_Opcode;
        break;

    /* Invalid Operand specification */
    case 3UL:
        errType = E_OS_Invalid_Operand;
        break;

    /* Data Address Alignment */
    case 4UL:
        errType = E_OS_Data_Address_Align;
        break;

    /* Invalid Local Memory Address */
    case 5UL:
        errType = E_OS_Invalid_Local_Memory;
        break;

    default:
        while (1) /* PRQA S 2740, 2870 */ /* VL_Os_2740 */ /* VL_Os_2870 */
        {
            /* Nothing to do. */
        }
        break; /* PRQA S 2880 */ /* VL_Os_2880 */
    }
    /* PRQA S 3120 -- */
    Os_ArchTrapMsgType trapMsg = {pcx, Os_Tin};
    Os_ExceptionHandler((void *)&trapMsg, errType);/* PRQA S 0314 */  /* VL_Os_0314 */
    OS_ARCH_DSYNC(); /* PRQA S 2880 */ /* VL_Os_2880 */
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}

/******************************************************************************/
/*
 * Brief                <trap 3>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
void Trap3(uint32 Os_Tin) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    uint32     pcx = OS_ARCH_MFCR(OS_REG_PCX);
    StatusType errType;

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
    Os_TrapMsg = "Context Management"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum*/
    switch (Os_Tin & 0xFFFFUL)
    {
    /* Free Context List Depletion(FCX = LCX)*/
    case 1UL:
        errType = E_OS_Free_Context_List_Depletion;
        break;

    /* Call Depth Overflow */
    case 2UL:
        errType = E_OS_Call_Depth_Overflow;
        break;

    /* Call Depth Underflow */
    case 3UL:
        errType = E_OS_Call_Depth_Underflow;
        break;

    /* Free Context List Underflow(FCX = 0) */
    case 4UL:
        errType = E_OS_Free_Context_List_Underflow;
        break;

    /* Call Stack Underflow(PCX = 0) */
    case 5UL:
        errType = E_OS_Call_Stack_Underflow;
        break;

    /* Context Type(PCXI.UL wrong) */
    case 6UL:
        errType = E_OS_Context_Type;
        break;

    /* Nesting Error: RFE with non-zero call depth */
    case 7UL:
        errType = E_OS_Nesting_Error;
        break;

    default:
        while (1) /* PRQA S 2740, 2870 */ /* VL_Os_2740 */ /* VL_Os_2870 */
        {
            /* Nothing to do. */
        }
        break; /* PRQA S 2880 */ /* VL_Os_2880 */
    }
    /* PRQA S 3120 -- */
    Os_ArchTrapMsgType trapMsg = {pcx, Os_Tin};
    Os_ExceptionHandler((void *)&trapMsg, errType);/* PRQA S 0314 */  /* VL_Os_0314 */
    OS_ARCH_DSYNC(); /* PRQA S 2880 */ /* VL_Os_2880 */
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}

/******************************************************************************/
/*
 * Brief                <trap 4>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
void Trap4(uint32 Os_Tin) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    uint32     pcx = OS_ARCH_MFCR(OS_REG_PCX);
    StatusType errType;

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
    Os_TrapMsg = "System Bus and Peripheral Errors"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    switch (Os_Tin & 0xFFFFUL)
    {
    /* Program Fetch Synchronous Error */
    case 1UL:
        errType = E_OS_Program_Fetch_Synchronous;
        break;

    /* Data Access Synchronous Error */
    case 2UL:
        errType = E_OS_Data_Access_Synchronous;
        break;

    /* Data Acess Asynchronous Error */
    case 3UL:
        errType = E_OS_Data_Acess_Asynchronous;
        break;

    /* Coprocessor Trap Asynchronous Error. (TriCore 1.3.1)*/
    case 4UL:
        errType = E_OS_Coprocessor_Trap_Asynchronous;
        break;

    /* Program Memory Integrity Error. (TriCore 1.3.1) */
    case 5UL:
        errType = E_OS_Program_Memory_Integrity;
        break;

    /* Data Memory Integrity Error. (TriCore 1.3.1) */
    case 6UL:
        errType = E_OS_Data_Memory_Integrity;
        break;

    /* Temporal Asynchronous Error. (TriCore 1.6)*/
    case 7UL:
        errType = E_OS_Temporal_Asynchronous;
        break;

    default:
        while (1) /* PRQA S 2740, 2870 */ /* VL_Os_2740 */ /* VL_Os_2870 */
        {
            /* Nothing to do. */
        }
        break; /* PRQA S 2880 */ /* VL_Os_2880 */
    }
    /* PRQA S 3120 -- */
    Os_ArchTrapMsgType trapMsg = {pcx, Os_Tin};
    Os_ExceptionHandler((void *)&trapMsg, errType);/* PRQA S 0314 */  /* VL_Os_0314 */
    OS_ARCH_DSYNC(); /* PRQA S 2880 */ /* VL_Os_2880 */
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}

/******************************************************************************/
/*
 * Brief                <trap 5>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
void Trap5(uint32 Os_Tin) /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    uint32     pcx = OS_ARCH_MFCR(OS_REG_PCX);
    StatusType errType;

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
    Os_TrapMsg = "Assertion Traps"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    switch (Os_Tin & 0xFFFFUL)
    {
    /* Arithmetic Overflow */
    case 1UL:
        errType = E_OS_Arithmetic_Overflow;
        break;

    /* Sticky Arithmetic Overflow */
    case 2UL:
        errType = E_OS_Sticky_Arithmetic_Overflow;
        break;

    default:
        while (1) /* PRQA S 2740, 2870 */ /* VL_Os_2740 */ /* VL_Os_2870 */
        {
            /* Nothing to do. */
        }
        break; /* PRQA S 2880 */ /* VL_Os_2880 */
    }
    /* PRQA S 3120 -- */
    Os_ArchTrapMsgType trapMsg = {pcx, Os_Tin};
    Os_ExceptionHandler((void *)&trapMsg, errType);/* PRQA S 0314 */  /* VL_Os_0314 */
    OS_ARCH_DSYNC(); /* PRQA S 2880 */ /* VL_Os_2880 */
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}

/******************************************************************************/
/*
 * Brief                <trap 7>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ISR>
 * REQ ID               <None>
 */
/******************************************************************************/
void Trap7(uint32 Os_Tin) /* Non-Maskable Interrupt */ /* PRQA S 3006, 1532 */ /* VL_Os_3006, VL_QAC_OneFunRef */
{
    uint32     pcx = OS_ARCH_MFCR(OS_REG_PCX);
    StatusType errType;

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
    Os_TrapMsg = "Non-Maskable Interrupt"; /* PRQA S 3122, 0753 */ /* VL_Os_3122 */ /* VL_Os_0753 */
#endif
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    switch (Os_Tin & 0xFFFFUL)
    {
    /* NMI - Non-Maskable Interrupt (TIN 0) */
    case 1UL:
        errType = E_OS_NMI;
        break;

    default:
        while (1) /* PRQA S 2740, 2870 */ /* VL_Os_2740 */ /* VL_Os_2870 */
        {
            /* Nothing to do. */
        }
        break; /* PRQA S 2880 */ /* VL_Os_2880 */
    }
    /* PRQA S 3120 -- */
    Os_ArchTrapMsgType trapMsg = {pcx, Os_Tin};
    Os_ExceptionHandler((void *)&trapMsg, errType);/* PRQA S 0314 */  /* VL_Os_0314 */
    OS_ARCH_DSYNC(); /* PRQA S 2880 */ /* VL_Os_2880 */
    OS_ASM("rslcx");
    OS_ASM("nop");
    OS_ASM("rfe");
}
/* PRQA S 1006 -- */
/*=======[E N D   O F   F I L E]==============================================*/
