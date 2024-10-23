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
 **  FILENAME    : Arch_Irq.c                                                  **
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
#include "Arch_Irq.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/


/*=======[E X T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_VAR_CLEARED_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
VAR(uint32, OS_VAR)             Os_Isr1NestSp[CFG_ISR_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CLONE_UNSPECIFIED
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLEARED_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
VAR(uint32, OS_VAR)             Os_Isr2NestSp[CFG_ISR_MAX_CORE0];
#define OS_STOP_SEC_VAR_CLEARED_CLONE_UNSPECIFIED
#include "Os_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/


/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Save cpsr to variable msr and disable maskable interrupt>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <SuspendAllInterrupts>
 * REQ ID               <DD_1_0719, DD_1_0720, DD_1_1300>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchSuspendInt(Os_ArchMsrType* msr) /* PRQA S 3006 */ /* MISRA Rule 4.3 */
{
    *msr = csr_read_clear(CSR_STATUS, 0x2u);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <transfer variable msr back to cpsr>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeAllInterrupts>
 * REQ ID               <DD_1_0721, DD_1_0722, DD_1_1301>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchRestoreInt(Os_ArchMsrType msr) /* PRQA S 3006 */ /* MISRA Rule 4.3 */
{
    VAR(Os_ArchMsrType,OS_VAR) csr = msr & 0x2u;
    csr_set(CSR_STATUS, csr);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Enable the interrupt source.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <vIsrSrc,vIsrSrcType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    OS_ARCH_DECLARE_CRITICAL();
    OS_ARCH_ENTRY_CRITICAL();
    /* . . . */
    OS_ARCH_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Disable the interrupt source.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <vIsrSrc,vIsrSrcType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    OS_ARCH_DECLARE_CRITICAL();

    OS_ARCH_ENTRY_CRITICAL();
    /* . . . */
    OS_ARCH_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Clear software interrupt source.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <vIsrSrc,vIsrSrcType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    OS_ARCH_DECLARE_CRITICAL();
    OS_ARCH_ENTRY_CRITICAL();
    /* . . . */
    OS_ARCH_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get software interrupt state.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <vIsrSrc,vIsrSrcType>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <Os_IsrStateType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(Os_IsrStateType, OS_CODE) Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    VAR(Os_IsrStateType, OS_VAR) isrSourceState;

    /* . . . */

    return isrSourceState;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Ipl greater than 0, disable interrupt otherwise enable
 *                       interrupt>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <ipl>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <ResumeOSInterrupts and so on>
 * REQ ID               <DD_1_0715, DD_1_0716>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchSetIpl(Os_IPLType ipl, Os_IsrDescriptionType isrdesc)
{
    (void)isrdesc;

    OS_ARCH_DECLARE_CRITICAL();
    OS_ARCH_ENTRY_CRITICAL();
    /* . . . */
    OS_ARCH_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <get the IPL>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <0>
 * PreCondition         <None>
 * CallByAPI            <SuspendOSInterrupts and so on>
 * REQ ID               <DD_1_0717, DD_1_0718,DD_1_1299>
 */
/******************************************************************************/
FUNC(Os_IPLType, OS_CODE) Os_ArchGetIpl(void)
{
    VAR(uint32,OS_VAR) saveipl;

    OS_ARCH_DECLARE_CRITICAL();
    OS_ARCH_ENTRY_CRITICAL();
    /* . . . */
    OS_ARCH_EXIT_CRITICAL();

    return (Os_IPLType)saveipl;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/******************************************************************************/
/*
 * Brief                <Set the pending register.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_SetPending(uint32 vIsrSrc)
{

}

/******************************************************************************/
/*
 * Brief                <Clear the pending register.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ClearPending(uint32 vIsrSrc)
{

}

/******************************************************************************/
/*
 * Brief                <Handle interrupts for the PLIC.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_PLIC_Handler()
{

}

/******************************************************************************/
/*
 * Brief                <Handle interrupts for the CPU.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <Pending>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_CPU_Handler(uint32 Pending)
{

}

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Install interrupt source.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <irqId,prio,pFun>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitIntPrio>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InterruptInstall(uint32 irqId, uint8 prio, IntFun pFun)
{

}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize interrupts.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InterruptInit(void)
{
    csr_set(CSR_IE, 0xffffffffffffffffU);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
