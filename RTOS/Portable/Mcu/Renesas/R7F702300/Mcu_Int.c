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
 **  FILENAME    : Mcu_Int.c                                                   **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Interrupt driver                                            **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
/*Holds the entry address of an FE interrupt.*/
VAR(Os_isrhnd, OS_VAR) Os_FE_Handler[OS_ARCH_FE_INTR_NUM];
/*Holds the entry address of an EI interrupt.*/
VAR(Os_isrhnd, OS_VAR) Os_Vector_Handler[OS_ARCH_EI_INTR_NUM];
/*The top of the stack that holds the FE level interrupt.*/
#define OS_STOP_SEC_VAR_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/******************************************************************************/
/*
 * Brief                <Initialize interrupts>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <DD_1_0902, DD_1_1298>
 */
/******************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
FUNC(void, OS_CODE) Os_InterruptInit(void)
{
    uint32 vecno;
    Os_CoreIdType phyCoreId = Os_ArchGetCoreID();

    /* Set exception handler vector address */
    OS_ARCH_REG_WRITE(OS_EBASE_NUM, (uint32)&Os_ExceptionTable); /* PRQA S 0305 */ /* MISRA Rule 11.1 */
    OS_ARCH_REG_WRITE(OS_PSW_NUM, OS_ARCH_REG_READ(OS_PSW_NUM) | ((uint32)1U << 15U));

    /* PRQA S 0303-- */ /* MISRA Rule 11.4 */
    /*Initialize the interrupt handlers.*/
    for (vecno = 0; vecno < OS_ARCH_EI_INTR_NUM; ++vecno)
    {
        Os_Vector_Handler[vecno] = (Os_isrhnd)Os_IntHandler;
    }

    for (vecno = 0; vecno < OS_ARCH_FE_INTR_NUM; ++vecno)
    {
        Os_FE_Handler[vecno] = Os_FEIntHandler;
    }

    /* masked all FE interrupt */
    OS_FEINTMSK_FACTOR_0(phyCoreId) = 0xFFFFFFFF;

#if (OS_AUTOSAR_CORES > 1)
    /* Initialize the multil cores interrupt. */
    OS_IPIR_IPInFCLRS(0) = 0xF;

    /* Write 1 to the x-th bit to enable the issuance of inter-PE interrupt requests from
    PEx to PEm. Write 0 to the x-th bit to disable the issuance of inter-PE interrupt
    requests from PEx to PEm */
    OS_IPIR_IPInENS(0) |= (uint8)(~(1U << Os_SCB.sysCore));
#endif /* OS_AUTOSAR_CORES > 1 */
}

/******************************************************************************/
/*
 * Brief                <Interrupt installation.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <irq,srcAddr,prio,isrProc,intc2SelectionBit>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <DD_1_0762>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InterruptInstall(const Os_IsrCfgType* IsrCfg, const Os_IntInstallType* IntInstall)
{
    uint32 coreId = IsrCfg->OsIsrSrcType;
    uint16 irqId = IntInstall->IntNumber;
    uint16 prio = IntInstall->IntPrio;
    Os_isrhnd isrHandler = IntInstall->ISR;
    uint16 intc2SelectionBit = IntInstall->intc2SelectionBit;

    if (OS_IRQTYPE_EIINT == intc2SelectionBit)
    {
        OS_INTC_SELF_EICn(irqId) &= (uint16)(~0x0F);
        OS_INTC_SELF_EICn(irqId) |= (uint16)INT_PRIORITY_LOG_TO_PHY(prio);

        /* Direct vector method, Interrupt processing is enabled */
        OS_INTC_SELF_EICn(irqId) &= (uint16)(~((1U << 6) | (1U << 7)));

        if (irqId >= 32)
        {
            OS_INTC_EIBDn(coreId, irqId) |= coreId;
        }

        /* insert the isr function address into the interrupt vector table */
        Os_Vector_Handler[irqId] = (Os_isrhnd)isrHandler;
    }
    else
    {
        /* FEINT */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
