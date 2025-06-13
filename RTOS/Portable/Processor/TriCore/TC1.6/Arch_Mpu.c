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
**  FILENAME    : Arch_Mpu.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the Memory Protection Unit (MPU)       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Arch_Mpu.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
P2CONST(OS_ISR_ADDR, AUTOMATIC, OS_VAR)
Os_IsrDAddr;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
P2CONST(OS_TASK_ADDR, AUTOMATIC, OS_VAR)
Os_TaskDAddr;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
VAR(uint32, OS_VAR)
Os_PSW_PRS;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
#endif
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Set the memory protection properties>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <MpSetIdx>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_ArchInitKnMemMap and so on>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchSetMemProtSet(uint32 MpSetIdx)
{
    Os_PSW_PRS = MpSetIdx << 12u;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Change csa PSW.IO bits to User-1 Mode>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchFirstEnterTask>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ModeModify(void)
{
#if (TRUE == CFG_SRV_SHELLOS)
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR) /* PRQA S 3432 */ /* MISRA Rule 20.7 */ /*ARCH_MPU_C_INLINE_ASM_001*/
    Csa1;

    /*make sure current app is trusted or not*/
    if (TRUE != Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted)
    {
        /*get the address of the PCX point to*/
        Csa1 = OS_PCX_TO_EA((Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_PCX));
        /* PRQA S 0306,3469 */ /* MISRA Rule 11.4 */ /* ARCH_MPU_TYPE_CAST_002 */

        /*change csa PSW.IO bits to User-1 Mode*/
        Csa1->reg[1] &= 0xfffff3ffu;
        Csa1->reg[1] |= 0x00000400u;
    }
#endif /* TRUE == CFG_SRV_SHELLOS */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Gets the memory protection property>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_ArchSetIsrMemMap,Os_ArchSetTaskMemMap>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(uint32, OS_CODE)
Os_ArchGetMemProtSet(void)
{
    return Os_PSW_PRS;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Enable the enable bits for memory protection in PSW and CORECON>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE)
Os_ArchMemProtEnable(void)
{
    P2VAR(Os_ArchCsaType, AUTOMATIC, OS_VAR)
    csa = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_PCX));
    csa->reg[1] &= 0xFFFFCFFFu;
    uint32 temp = 0x0u;
    temp = OS_ARCH_MFCR(OS_REG_SYSCON);
    temp |= 0x00000002u;
    OS_ARCH_MTCR(OS_REG_SYSCON, temp);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
