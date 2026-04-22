/* PRQA S 3108++ */
/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
**  FILENAME    : Arch_Trap.c                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the processor exception                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Os_Types.h"
#include "Os_Kernel2Port.h"
#include "Os_Arch_Mpu.h"
#include "Os_Arch_Processor.h"
#include "Os_ProtectHook.h"
/*=======[V E R S I O N  C H E C K]==========================================*/

/*=======[M A C R O S]=======================================================*/

/*=======[E X T E R N A L   D A T A]=========================================*/
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
uint16 HardFault_Flag;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
static char* Os_TrapMsg = "NULL"; /* PRQA S 3122, 0753, 3625 */ /* VL_Os_3122 */ /* VL_Os_0753 */ /* VL_Os_3625 */
#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

/*=======[I N T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]=======*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <SVC_Handler>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <System services>
 */
/*****************************************************************************/
void Os_SVC_Handler(void)
{
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_ArchSetCpuMode(OS_CPUMODE_SUPERVISOR);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
#if (CFG_MEMORY_PROTECTION_ENABLE == TRUE)
/* Memory access error handler */
void HardFault_Handler(void)
{
    uint8 err = NO_FAULT_BEHAVIOR;
    HardFault_Flag = 1u;
    Os_ArchSetCpuMode(OS_CPUMODE_SUPERVISOR);
    err = Os_ArchGetFault();
    if (err != NO_FAULT_BEHAVIOR)
    {
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
        uint32 WhoHook = OS_TMPROT_HOOK_OSAPP;

        if (OS_LEVEL_TASK == Os_Kernel_GetSysLevel())
        {
            WhoHook = OS_TMPROT_HOOK_TASK;
        }
        else if (OS_LEVEL_ISR2 == Os_Kernel_GetSysLevel())
        {
            WhoHook = OS_TMPROT_HOOK_ISR;
        }
        else
        {
            /* Do nothing. */
        }
#endif
#endif

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_MEMORY);
#endif
/* OS245:  protection exception hook. */
#if (TRUE == CFG_PROTECTIONHOOK)
        (void)Os_CallProtectionHook(E_OS_PROTECTION_MEMORY, WhoHook);
#endif
#else
#if (TRUE == CFG_ERRORHOOK)
        (void)ErrorHook(E_OS_PROTECTION_MEMORY);
#endif
#endif
    }
    else
    {
#if (CFG_SRV_SHELLOS == TRUE)
        Os_ArchSetCpuMode(OS_CPUMODE_SUPERVISOR);
#endif
    }
    HardFault_Flag = 0u;
}
#else
void HardFault_Handler(void)
{
    uint8 i = 1;
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    uint32 WhoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_Kernel_GetSysLevel())
    {
        WhoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_Kernel_GetSysLevel())
    {
        WhoHook = OS_TMPROT_HOOK_ISR;
    }
    else
    {
        /* Do nothing. */
    }
#endif
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))

    /* OS245:  protection exception hook. */
    (void)Os_CallProtectionHook(E_OS_PROTECTION_MEMORY, WhoHook);
#else
    while (i)
        ;
#endif
}
#endif

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

