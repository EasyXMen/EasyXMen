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
/* PRQA S 3108-- */

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_ProtectHook.h"
#include "Os_Kernel2Port.h"
#include "Os_FaultManager.h"
/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
static char* Os_TrapMsg = "NULL";
/* PRQA S 3122, 0753, 3625, 3232, 3218 */ /* VL_Os_3122 */ /* VL_Os_0753 */ /* VL_Os_3625 */
/* VL_Os_3232 */ /* VL_Os_3218 */
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
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void Os_MipMdp(void)
/* PRQA S 1503 -- */
{
    uint32 mpuErr = (OS_ARCH_REG_READ(OS_FEIC_NUM) >> 4U) & 0x09UL; /* PRQA S 3120 */ /* VL_QAC_MagicNum */

    StatusType errType = E_OS_PROTECTION_EXCEPTION;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    errType = E_OS_PROTECTION_MEMORY;
#endif

    Os_ExceptionHandler((void *)0, errType);

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
void Os_FENMI(void)
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_FEIntHandler(void)
/* PRQA S 1532 -- */
{
    /* Just ignore this interrupt.  */
    while (1) /* PRQA S 2740 */ /* VL_Os_2740 */
    {
        /* Nothing to do. */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
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
/* PRQA S 3120 ++ */                                                                     /* VL_Os_3120 */
void Os_Hal_ExceptionInformationParse(Os_TrapContextType* TrapContext) /* PRQA S 1532 */ /* VL_Os_1532 */
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
/*=======[E N D   O F   F I L E]==============================================*/
