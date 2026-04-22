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
 **  @file               : Os_Mcu_Irq.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : MCU source file for Irq API implementations
 **
 ***********************************************************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Core.h"
#include "Os_Interrupt.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 3207, 3132 ++ */ /* VL_Os_3207, VL_QAC_MagicNum */
static const uint32 TPTimer_Addr[6] =
    {OS_ISR_STM0_SR1, OS_ISR_STM1_SR1, OS_ISR_STM2_SR1, OS_ISR_STM3_SR1, OS_ISR_STM4_SR1, OS_ISR_STM5_SR1};
/* PRQA S 3207, 3132 -- */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Enable the given interrupt source vIsrSrc.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_Hal_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
/* PRQA S 1532 -- */
{
     /* PRQA S 0303, 1259, 3442 ++ */ /* VL_Os_0303, VL_Os_1259, VL_Os_3442 */
    (*((volatile uint32*)vIsrSrc)) |= ((uint32)1 << SRC_SRE_BIT);
     /* PRQA S 0303, 1259, 3442 -- */
    (void)vIsrSrcType;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Disable the given interrupt source vIsrSrc.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_Hal_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
/* PRQA S 1532 -- */
{
     /* PRQA S 0303, 1259, 3442 ++ */ /* VL_Os_0303, VL_Os_1259, VL_Os_3442 */
    (*((volatile uint32*)vIsrSrc)) &= ~((uint32)1 << SRC_SRE_BIT);
     /* PRQA S 0303, 1259, 3442 -- */
    (void)vIsrSrcType;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Clear the pending status of the given interrupt.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
void Os_Hal_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
{
     /* PRQA S 0303, 1259, 3442 ++ */ /* VL_Os_0303, VL_Os_1259, VL_Os_3442 */
    (*((volatile uint32*)vIsrSrc)) |= ((uint32)1 << SRC_CLRR_BIT);
     /* PRQA S 0303, 1259, 3442 -- */
    (void)vIsrSrcType;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the status of the given interrupt.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
Os_IsrStateType Os_Hal_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    Os_IsrStateType isrSourceState;

     /* PRQA S 0303, 1259, 3442 ++ */ /* VL_Os_0303, VL_Os_1259, VL_Os_3442 */
    if (((uint32)1 << SRC_SRE_BIT)
        == ((*((volatile uint32*)vIsrSrc)) & ((uint32)1 << SRC_SRE_BIT)))
     /* PRQA S 0303, 1259, 3442 -- */
    {
        isrSourceState = OS_ISR_ENABLED;
    }
    else
    {
        isrSourceState = OS_ISR_DISABLED;
    }

    return (isrSourceState);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Installing an Interrupt.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_McuInterruptInstall(const Os_IsrCfgType* IsrCfg, const Os_IntInstallType* IntInstall)
/* PRQA S 1532 -- */
{
    uint32 srcAddr = IsrCfg->IsrSrc;
    uint32 srcType = IsrCfg->IsrSrcType;
    uint8  prio    = IntInstall->IntPrio;

    /* PRQA S 0303, 3442, 3345 ++ */ /* VL_Os_0303, VL_Os_3442, VL_Os_3345 */
    OS_INTERRUPT_INSTALL(srcAddr, prio, srcType);
    /* PRQA S 0303, 3442, 3345 -- */
    Cdisptab[prio] = IntInstall->ISR;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Disable All interrupt But Timing Protect interrupt>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <None Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_DisableAllInt_ButTimingProtInt(Os_CoreIdType CoreId)
{
    uint32               i;
    const Os_IsrCfgType* pIsrCfg;
    Os_IsrType           idStartRange = Os_IsrIdRange[CoreId].AllIsr.IsrStart;
    Os_IsrType           idEndRange   = Os_IsrIdRange[CoreId].AllIsr.IsrEnd;
    for (i = (uint32)idStartRange; i < (uint32)idEndRange; i++)
    {
        pIsrCfg        = &Os_IsrCfg[i];
        uint32 srcAddr = pIsrCfg->IsrSrc;
        uint32 srcType = pIsrCfg->IsrSrcType;
        if (TPTimer_Addr[CoreId] != srcAddr)
        {
            /* PRQA S 0303, 3345, 3442, 1840 ++ */ /* VL_Os_0303, VL_Os_3345, VL_Os_3442, VL_Os_1840*/
            OS_INTERRUPT_INSTALL(srcAddr, 0, srcType);
            /* PRQA S 0303, 3345, 3442, 1840 -- */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Enable All interrupt But Timing Protect interrupt>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <None Reentrant>
 * param-Name[in]       <None>
 * param-Name[out]      <None>
 * param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
void Os_Hal_EnableAllInt_ButTimingProtInt(Os_CoreIdType CoreId)
{
    uint32                   i;
    const Os_IsrCfgType*     pIsrCfg;
    const Os_IntInstallType* pIntInstall;
    Os_IsrType               idStartRange = Os_IsrIdRange[CoreId].AllIsr.IsrStart;
    Os_IsrType               idEndRange   = Os_IsrIdRange[CoreId].AllIsr.IsrEnd;

    for (i = (uint32)idStartRange; i < (uint32)idEndRange; i++)
    {
        pIsrCfg        = &Os_IsrCfg[i];
        pIntInstall    = &Os_IntInstall[i];
        uint32 srcAddr = pIsrCfg->IsrSrc;
        uint32 srcType = pIsrCfg->IsrSrcType;
        uint8  prio    = pIntInstall->IntPrio;
        if (TPTimer_Addr[CoreId] != srcAddr)
        {
            /* PRQA S 0303, 3345, 3442 ++ */ /* VL_Os_0303, VL_Os_3345, VL_Os_3442*/
            OS_INTERRUPT_INSTALL(srcAddr, prio, srcType);
            /* PRQA S 0303, 3345, 3442 -- */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the current running IRQ.
 */
uint32 Os_Hal_GetCurrentIrq(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint32 temp = (uint32)OS_ARCH_MFCR(OS_REG_ICR);
    return (uint32)(temp & 0xFFu); /* PRQA S 3120 */ /* VL_QAC_MagicNum*/
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the current running Exception.
 */
uint32 Os_Hal_GetCurrentExc(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    return (0UL);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
