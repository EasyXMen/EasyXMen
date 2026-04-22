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
/*******************************************************************************
**                                                                            **
**  FILENAME    : Arch_Mpu.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Interrupt Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Arch_Mpu.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/*=======[M A C R O S]========================================================*/
#define MPROT_ENABLE_BIT (1u)
/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
const OS_ISR_ADDR* Os_IsrDAddr;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
const OS_TASK_ADDR* Os_TaskDAddr;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
uint32 Os_PSW_PRS;
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_ArchSetMemProtSet(uint32 MpSetIdx)
/* PRQA S 1532 -- */
{
    Os_PSW_PRS = MpSetIdx << PPRS_SET_BITS;
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
/* PRQA S 1532 ++ */                       /* VL_QAC_OneFunRef */
void Os_ModeModify(void) /* PRQA S 3408 */ /* VL_Os_3408 */
/* PRQA S 1532 -- */
{
#if (TRUE == CFG_SRV_SHELLOS)
    Os_ArchCsaType* Csa1; /* PRQA S 3432 */ /* VL_Os_3432 */

    /*make sure current app is trusted or not*/
    if (TRUE != Os_AppCfg[Os_SCB.sysRunningAppID].OsTrusted)
    {
        /*get the address of the PCX point to*/
        /* PRQA S 3469 ++ */                                                             /* VL_Os_3469 */
        Csa1 = OS_PCX_TO_EA((Os_ArchMsrType)OS_ARCH_MFCR(OS_REG_PCX)); /* PRQA S 0306 */ /* VL_Os_0306 */
        /* PRQA S 3469 --*/

        /*change csa PSW.IO bits to User-1 Mode*/
        Csa1->reg[1] &= PSWIO_MASK;
        Csa1->reg[1] |= PSWIO_USER1_MODE; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
uint32 Os_ArchGetMemProtSet(void)
/* PRQA S 1532 -- */
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
/* PRQA S 1532 ++ */                              /* VL_QAC_OneFunRef */
void Os_ArchMemProtEnable(void) /* PRQA S 3006 */ /* VL_Os_3006 */
/* PRQA S 1532 -- */
{
    Os_ArchCsaType* csa = OS_PCX_TO_EA(OS_ARCH_MFCR(OS_REG_PCX)); /* PRQA S 0306 */ /* VL_Os_0306 */
    csa->reg[1] &= PPRS_MASK; /* PRQA S 3120 */                                   /* VL_QAC_MagicNum */
    uint32 temp = 0x0u;
    temp        = OS_ARCH_MFCR(OS_REG_SYSCON);
    temp |= (1u << MPROT_ENABLE_BIT); 
    OS_ARCH_MTCR(OS_REG_SYSCON, temp); /* PRQA S 3138, 1006 */ /* VL_Os_3138, VL_Os_1006 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
