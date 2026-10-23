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
 **  FILENAME    : Os_Arch_Trap.c                                              **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : functions related to the processor exception.               **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"
#include "Os_Arch_Define.h"
#include "Os_Arch_Trap.h"
#include "Os_Arch_Processor.h"
#include "Os_FaultManager.h"
#include "Os_ProtectHook.h"
#include "Os_Kernel2Port.h"
#include "Os_Cfg_S.h"
/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
#if (TRUE == CFG_HARDWARE_FAULT_ENABLE)
extern Os_ExceptionContextType Os_HalContextExceptionContext[]; /* PRQA S 3684 */ /* VL_Os_3684 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static uint32 Os_Arch_ExceptionInformationParse(uint32 exceptionErr);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_HARDWARE_FAULT_ENABLE */
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
static const char* Os_TrapMsg = "NULL"; /* PRQA S 3625 */ /* VL_Os_3625 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Mdp and mip exception handler.>
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
/* PRQA S 1503, 3408 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3408 */
void Os_ArchMipMdp(void)
/* PRQA S 1503, 3408 -- */
{
    uint32 mpuErr = (OS_ARCH_REG_READ(OS_ARCH_FEIC_NUM) >> 4U) & 0x09UL; /* PRQA S 3120 */ /* VL_QAC_MagicNum */

    StatusType errType = E_OS_PROTECTION_EXCEPTION;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    errType = E_OS_PROTECTION_MEMORY;
#endif

    Os_ExceptionHandler((void*)0, errType);

    while (0UL != mpuErr) /* PRQA S 2487, 2872 */ /* VL_Os_2487, VL_Os_2872 */
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <FENMI exception handler.>
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
/* PRQA S 3408 ++ */ /* VL_Os_3408 */
void Os_ArchFENMI(void)
/* PRQA S 3408 -- */
{
    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <FE exception handler.>
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
/* PRQA S 3408 ++ */ /* VL_Os_3408 */
void Os_Arch_FEExceptionHandler(void)
/* PRQA S 3408 -- */
{
#if ((TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE))
    uint32  ExceptionErr;
    uint32  Psw;
    uint32  Ra;
    uint32* Sp;
    StatusType errType;
    Os_ArchTrapMsgType trapMsg;

    ExceptionErr = OS_ARCH_REG_READ(OS_ARCH_FEIC_NUM);
    Sp = (uint32*)Os_ArchTempSp;
#if ((1U) == CFG_FPU_ENABLE)
    /* enabling FPU will store 6 FPU registers */
    Psw = Sp[6]; /* R7 */
    Ra  = Sp[8]; /* R31/LP */
#else
    Psw = Sp[0]; /* R7 */
    Ra  = Sp[2]; /* R31/LP */
#endif /* (1U) == CFG_FPU_ENABLE */    
    ExceptionErr = Os_Arch_ExceptionInformationParse(ExceptionErr);

    trapMsg.Sp              = Sp;
    trapMsg.ExceptionSource = ExceptionErr;
    trapMsg.Psw             = Psw;
    trapMsg.Ra              = Ra;
    Os_ExceptionHandler(&trapMsg, errType);
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */
    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <FEINT handler.>
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
/* PRQA S 1532, 3408 ++ */ /* VL_QAC_OneFunRef, VL_Os_3408 */
void Os_ArchFEIntHandler(void)
/* PRQA S 1532, 3408 -- */
{
    /* Just ignore this interrupt.  */
    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((TRUE == CFG_FAULT_MANAGEMENT_ENABLE) && (TRUE == CFG_HARDWARE_FAULT_ENABLE))
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
/* PRQA S 3120 ++ */                                                                     /* VL_QAC_MagicNum */
void Os_Hal_ExceptionInformationParse(Os_TrapContextType* TrapContext) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    TrapContext->TrapMsg.ExceptionAddress       = TrapContext->TrapRegs.Ra;
    TrapContext->TrapMsg.ExceptionSP            = TrapContext->TrapRegs.Sp;
    TrapContext->TrapMsg.ExceptionSysteamStatus = TrapContext->TrapRegs.Psw;
    TrapContext->TrapMsg.ExceptionCase          = TrapContext->TrapRegs.ExceptionSource; /* FEIC/EIIC */
    TrapContext->TrapMsg.ExceptionCaseMsg       = Os_TrapMsg;
    /* PRQA S 3120 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_HalExceptionSaveContext(const void* TrapMsg)
{
    Os_ExceptionContextType* Context;
    uint32* Sp;
    uint8   Index;

    Context = &Os_HalContextExceptionContext[Os_Hal_GetCoreID()];
    Sp      = ((Os_ArchTrapMsgType*)TrapMsg)->Sp;

    Context->Sp              = (uint32)Sp;                           /* SP */
    Context->ExceptionSource= ((Os_ArchTrapMsgType*)TrapMsg)->ExceptionSource;        /* FEIC */
    Context->Psw = ((Os_ArchTrapMsgType*)TrapMsg)->Psw;                   /* STATUS */
    Context->Ra = ((Os_ArchTrapMsgType*)TrapMsg)->Ra;                    /* Ra */

#if ((1U) == CFG_FPU_ENABLE)
    /* enabling FPU will store 6 FPU registers */
    Context->GeneralRegisters[1] = Sp[36];
    Context->GeneralRegisters[2] = Sp[35];
    for (Index = 6U; Index < 31U; Index++)
    {
        Context->GeneralRegisters[Index] = Sp[37 - Index];   /* R1-R2, R6-R31 */
    }
#else
    Context->GeneralRegisters[1] = Sp[30];
    Context->GeneralRegisters[2] = Sp[29];
    for (Index = 6U; Index < 31U; Index++)
    {
        Context->GeneralRegisters[Index] = Sp[31 - Index];   /* R1-R2, R6-R31 */
    }
#endif /* (1U) == CFG_FPU_ENABLE */

    /* Nothing need to do Cause saved before enter Os_ExceptionHandler */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
void Os_HalExceptionResumeContext(const void* errMsg)
{
    (void)errMsg;

    /* Wait for reset - should never reach here */
    /* PRQA S 2740, 2870 ++ */ /* VL_Os_2740, VL_Os_2870 */
    while (1u)
    {
        /* Nothing to do. */
    }
    /* PRQA S 2740, 2870 -- */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static uint32 Os_Arch_ExceptionInformationParse(uint32 exceptionErr)
{
    uint32 exceptionCase_low  = exceptionErr & 0xFFFFUL;
    uint32 exceptionCase_high = exceptionErr & 0xFFFF0000UL; /* fix me */
    uint32 exceptionCase;

    if (exceptionCase_low == 0x1CU)
    {
        Os_TrapMsg = "System Error";
        exceptionCase = E_OS_System_Error;
    }
    else if (exceptionCase_low == 0x60U)
    {
        Os_TrapMsg = "Reserved Instruction Exception";
        exceptionCase = E_OS_Reserved_Instruction_Exception;
    }
    else if ((exceptionCase_low >= 0x80U) && (exceptionCase_low <= 0x82U))
    {
        Os_TrapMsg = "Coprocessor Unusable Exception";
        exceptionCase = E_OS_Coprocessor_Unusable_Exception;
    }
    else if (exceptionCase_low == 0xA0U)
    {
        Os_TrapMsg = "Privilege Instruction Exception";
        exceptionCase = E_OS_Privilege_Instruction_Exception;
    }
    else if (exceptionCase_low == 0xC0U)
    {
        Os_TrapMsg = "Misalignment exception";
        exceptionCase = E_OS_Misalignment_Exception;
    }
    else
    {
        Os_TrapMsg = "Unknown Exception";
        exceptionCase = E_OS_Unknown_Exception;
    }

    return exceptionCase;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */
/*=======[E N D   O F   F I L E]==============================================*/
