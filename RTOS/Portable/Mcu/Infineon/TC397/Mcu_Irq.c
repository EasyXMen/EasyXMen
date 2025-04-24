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
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Mcu_Irq.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Deal with operations related to processor interrupts        **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_CoreCfg.h"
#include "Os_Internal.h"
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 TPTimer_Addr[6] =
    {OS_ISR_STM0_SR1, OS_ISR_STM1_SR1, OS_ISR_STM2_SR1, OS_ISR_STM3_SR1, OS_ISR_STM4_SR1, OS_ISR_STM5_SR1};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    OS_INTERRUPT_ENABLEREQ(vIsrSrc);
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
FUNC(void, OS_CODE) Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    OS_INTERRUPT_DISABLEREQ(vIsrSrc);
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
FUNC(void, OS_CODE) Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    OS_INTERRUPT_CLEARREQ(vIsrSrc); /* PRQA S 0303 */ /* MISRA Rule 11.4 */ /* ARCH_IRQ_TYPE_CAST_005 */
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
FUNC(Os_IsrStateType, OS_CODE) Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType)
{
    (void)vIsrSrcType;
    Os_IsrStateType isrSourceState;
    /* PRQA S 0303 ++*/ /* MISRA Rule 11.4 */ /* ARCH_IRQ_TYPE_CAST_005 */
    if (((uint32)1 << OS_SRC_SRE_BIT) == ((*((uint32 volatile*)vIsrSrc)) & ((uint32)1 << OS_SRC_SRE_BIT)))
    /* PRQA S 0303 --*/ /* MISRA Rule 11.4 */
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
FUNC(void, OS_CODE) Os_InterruptInstall(const Os_IsrCfgType* IsrCfg, const Os_IntInstallType* IntInstall)
{
    uint32 srcAddr = IsrCfg->OsIsrSrc;
    uint32 srcType = IsrCfg->OsIsrSrcType;
    uint8 prio = IntInstall->IntPrio;

    OS_INTERRUPT_INSTALL(srcAddr, prio, srcType);
    Cdisptab[prio] = IntInstall->ISR;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (CFG_ISR_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"

/******************************************************************************/
/*
 * Brief                <TryToGetSpinlock has the same functionality as GetSpinlock with the difference
                         that if the spinlock is already occupied by a TASK on a different core the function
                         sets the OUT parameter "Success" and returns with E_OK>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <TerminateApplication>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchDisableIntInApp(P2CONST(Os_ApplicationCfgType, AUTOMATIC, OS_VAR) posCurAppCfg)
{
    VAR(uint16, OS_VAR) i;
    VAR(uint16, OS_VAR) osIsrRefCnt;
    VAR(Os_IsrType, OS_VAR) osIsrId;
    VAR(uint32, OS_VAR) osIsrRegVal;

    osIsrRefCnt = posCurAppCfg->OsAppIsrRefCnt;

    for (i = 0u; i < osIsrRefCnt; i++)
    {
        osIsrId = posCurAppCfg->OsAppObjectRef[OBJECT_ISR][i];
        /* PRQA S 3469 ++*/ /* MISRA Dir 4.9 */ /*ARCH_PROCESSOR_MACRO_001*/
        osIsrRegVal = Os_IsrCfg[Os_GetObjLocalId(osIsrId)].OsIsrSrc;
        /* PRQA S 3469 --*/                        /* MISRA Dir 4.9 */
        /* PRQA S 0303 ++ */ /* MISRA Rule 11.4 */ /*ARCH_PROCESSOR_VTYPE_CAST_005*/
        OS_INTERRUPT_DISABLEREQ(osIsrRegVal);
        OS_INTERRUPT_CLEARREQ(osIsrRegVal);
        OS_INTERRUPT_SET_SRPN_ZERO(osIsrRegVal);
        /* PRQA S 0303 -- */ /* MISRA Rule 11.4 */
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* CFG_ISR_MAX > 0 */
#endif

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
FUNC(void, OS_CODE) Os_ArchDisableAllInt_ButTimingProtInt(void)
{
    VAR(Os_IsrType, OS_VAR) i;
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;
    P2CONST(Os_IsrCfgType, AUTOMATIC, OS_VAR) pIsrCfg;
    for (i = 0; i < Os_CfgIsrMax_Inf[vCoreId]; i++)
    {
        pIsrCfg = &(Os_IsrCfg_Inf[vCoreId][i]);
        uint32 srcAddr = pIsrCfg->OsIsrSrc;
        uint32 srcType = pIsrCfg->OsIsrSrcType;
        if (TPTimer_Addr[vCoreId] != srcAddr)
        {
            OS_INTERRUPT_INSTALL(srcAddr, 0, srcType);
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
FUNC(void, OS_CODE) Os_ArchEnableAllInt_ButTimingProtInt(void)
{
    VAR(Os_IsrType, OS_VAR) i;
    VAR(uint16, OS_VAR) vCoreId = Os_SCB.sysCore;
    P2CONST(Os_IsrCfgType, AUTOMATIC, OS_VAR) pIsrCfg;
    P2CONST(Os_IntInstallType, AUTOMATIC, OS_VAR) pIntInstall;
    for (i = 0; i < Os_CfgIsrMax_Inf[vCoreId]; i++)
    {
        pIsrCfg = &(Os_IsrCfg_Inf[vCoreId][i]);
        pIntInstall = &(Os_IntInstall_Inf[vCoreId][i]);
        uint32 srcAddr = pIsrCfg->OsIsrSrc;
        uint32 srcType = pIsrCfg->OsIsrSrcType;
        uint8 prio = pIntInstall->IntPrio;
        if (TPTimer_Addr[vCoreId] != srcAddr)
        {
            OS_INTERRUPT_INSTALL(srcAddr, prio, srcType);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/*=======[E N D   O F   F I L E]==============================================*/
