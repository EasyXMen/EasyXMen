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
**  FILENAME    : Mcu_Irq.c                                                   **
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
#include "Os_CoreCfg.h"
#include "Os_Internal.h"
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 TPTimer_Addr[CFG_CORE_MAX] =
    {OS_ISR_STM0_SR1, OS_ISR_STM1_SR1, OS_ISR_STM2_SR1, OS_ISR_STM3_SR1, OS_ISR_STM4_SR1, OS_ISR_STM5_SR1};
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
void Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
/* PRQA S 1532 -- */
{
    /* PRQA S 3345, 3432, 0303, 3138 ++ */ /* VL_Os_3345, VL_Os_3432, VL_Os_0303, VL_Os_3138 */
    /* PRQA S 1259, 3442, 1258 ++ */       /* VL_Os_1259, VL_Os_3442, VL_Os_1258 */
    OS_INTERRUPT_ENABLEREQ(vIsrSrc);
    /* 1259, 3442, 1258 -- */
    /* PRQA S 3345, 3432, 0303, 3138 -- */
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
void Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType)
/* PRQA S 1532 -- */
{
    /* PRQA S 3345, 3432, 0303, 3138 ++ */ /* VL_Os_3345, VL_Os_3432, VL_Os_0303, VL_Os_3138 */
    /* PRQA S 1259, 3442, 1258 ++ */       /* VL_Os_1259, VL_Os_3442, VL_Os_1258 */
    OS_INTERRUPT_DISABLEREQ(vIsrSrc);
    /* PRQA S 1259, 3442, 1258 -- */
    /* PRQA S 3345, 3432, 0303, 3138 -- */
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType)
/* PRQA S 1532 -- */
{
    OS_INTERRUPT_CLEARREQ(vIsrSrc); /* PRQA S 0303, 3442, 3345 */ /* VL_Os_0303, VL_Os_3442, VL_Os_3345 */
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_IsrStateType Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType)
/* PRQA S 1532 -- */
{
    (void)vIsrSrcType;
    Os_IsrStateType isrSourceState;
    /* PRQA S 0303, 1259, 3442 ++ */ /* VL_Os_0303, VL_Os_1259, VL_Os_3442 */
    if (((uint32)1u << OS_SRC_SRE_BIT) == ((*((volatile uint32*)vIsrSrc)) & ((uint32)1u << OS_SRC_SRE_BIT)))
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
void Os_InterruptInstall(const Os_IsrCfgType* IsrCfg, const Os_IntInstallType* IntInstall)
/* PRQA S 1532 -- */
{
    uint32 srcAddr = IsrCfg->OsIsrSrc;
    uint32 srcType = IsrCfg->OsIsrSrcType;
    uint8  prio    = IntInstall->IntPrio;

    OS_INTERRUPT_INSTALL(srcAddr, prio, srcType); /* PRQA S 0303, 3442, 3345 */ /* VL_Os_0303, VL_Os_3442, VL_Os_3345 */
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
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_ArchDisableIntInApp(const Os_ApplicationCfgType* posCurAppCfg)
/* PRQA S 1532 -- */
{
    uint16     i;
    uint16     osIsrRefCnt;
    Os_IsrType osIsrId;
    uint32     osIsrRegVal;

    osIsrRefCnt = posCurAppCfg->OsAppIsrRefCnt;

    for (i = 0u; i < osIsrRefCnt; i++)
    {
        osIsrId = posCurAppCfg->OsAppObjectRef[OBJECT_ISR][i];
        /* PRQA S 3469 ++*/ /* VL_Os_3469 */
        osIsrRegVal = Os_IsrCfg[Os_GetObjLocalId(osIsrId)].OsIsrSrc;
        /* PRQA S 3469 --*/
        /* PRQA S 0303, 3432, 3442, 1259 ++ */ /* VL_Os_0303, VL_Os_3432, VL_Os_3442, VL_Os_1259 */
        /* PRQA S 3138, 3345, 1258 ++ */       /* VL_Os_3138, VL_Os_3345, VL_Os_1258 */
        OS_INTERRUPT_DISABLEREQ(osIsrRegVal);
        OS_INTERRUPT_CLEARREQ(osIsrRegVal);
        OS_INTERRUPT_SET_SRPN_ZERO(osIsrRegVal);
        /* PRQA S 3138, 3345, 1258 -- */
        /* PRQA S 0303, 3432, 3442, 1259 -- */
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
void Os_ArchDisableAllInt_ButTimingProtInt(void)
{
    Os_IsrType           i;
    uint16               vCoreId = Os_SCB.sysCore;
    const Os_IsrCfgType* pIsrCfg;
    for (i = 0; i < Os_CfgIsrMax_Inf[vCoreId]; i++)
    {
        pIsrCfg        = &(Os_IsrCfg_Inf[vCoreId][i]);
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
void Os_ArchEnableAllInt_ButTimingProtInt(void)
{
    Os_IsrType               i;
    uint16                   vCoreId = Os_SCB.sysCore;
    const Os_IsrCfgType*     pIsrCfg;
    const Os_IntInstallType* pIntInstall;
    for (i = 0; i < Os_CfgIsrMax_Inf[vCoreId]; i++)
    {
        pIsrCfg        = &(Os_IsrCfg_Inf[vCoreId][i]);
        pIntInstall    = &(Os_IntInstall_Inf[vCoreId][i]);
        uint32 srcAddr = pIsrCfg->OsIsrSrc;
        uint32 srcType = pIsrCfg->OsIsrSrcType;
        uint8  prio    = pIntInstall->IntPrio;
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
