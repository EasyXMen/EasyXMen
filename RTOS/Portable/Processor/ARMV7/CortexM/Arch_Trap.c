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
#include "Os_CfgData.h"
#include "Arch_Mpu.h"
/*=======[V E R S I O N  C H E C K]==========================================*/

/*=======[M A C R O S]=======================================================*/

/*=======[E X T E R N A L   D A T A]=========================================*/
extern FUNC(void, OS_CODE) Os_SetCpuMode(Os_CPUModeType mode);
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
extern FUNC(ProtectionReturnType, OS_CODE) Os_CallProtectionHook(StatusType osErrType, uint32 osWhoHook);
#endif
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
extern VAR(uint16, OS_VAR) HardFault_Flag;
#endif
extern FUNC(void, OS_CODE) SVC_Handler(void);
extern FUNC(void, OS_CODE) HardFault_Handler(void);
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
VAR(uint16, OS_VAR) HardFault_Flag = 0U;
#define OS_STOP_SEC_VAR_CLONE_16
#include "Os_MemMap.h"
#endif
/*=======[I N T E R N A L   D A T A]=========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]=======*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]===================*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
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
FUNC(void, OS_CODE) SVC_Handler(void)
{
    Os_SetCpuMode(OS_CPUMODE_SUPERVISOR);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
#if (CFG_MEMORY_PROTECTION_ENABLE == TRUE)
/* Memory access error handler */
FUNC(void, OS_CODE) HardFault_Handler(void)
{
    uint8 err = NO_FAULT_BEHAVIOR;
    HardFault_Flag = 1u;
    Os_SetCpuMode(OS_CPUMODE_SUPERVISOR);
    err = Os_GetFault();
    if (err != NO_FAULT_BEHAVIOR)
    {
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
        uint32 WhoHook = OS_TMPROT_HOOK_OSAPP;

        if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
        {
            WhoHook = OS_TMPROT_HOOK_TASK;
        }
        else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
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
        Os_SetCpuMode(OS_CPUMODE_SUPERVISOR);
#endif
    }
    HardFault_Flag = 0u;
}
#else
FUNC(void, OS_CODE) HardFault_Handler(void)
{
    uint8 i = 1;
#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    uint32 WhoHook = OS_TMPROT_HOOK_OSAPP;

    if (OS_LEVEL_TASK == Os_SCB.sysOsLevel)
    {
        WhoHook = OS_TMPROT_HOOK_TASK;
    }
    else if (OS_LEVEL_ISR2 == Os_SCB.sysOsLevel)
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
