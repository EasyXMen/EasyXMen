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
 **  FILENAME    : Os_Mcu_Irq.c                                                **
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
#include "Os_Kernel2Port.h"
#include "Os_Mcu_Irq.h"
#include "Os_Arch_Irq.h"
#include "Os_Mcu_Core.h"
#include "Os_Arch_Processor.h"
#include "Os_Interrupt_CfgData.h"
/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
static const uint32 Os_TPTimer_ISR[OS_AUTOSAR_CORES] = {
    OS_ISR_OSTM4TINT,
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    OS_ISR_OSTM5TINT,
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    OS_ISR_OSTM6TINT,
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    OS_ISR_OSTM7TINT
#endif
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
/* PRQA S 1532, 0303, 3442, 3345, 4399 ++ */ /* VL_QAC_OneFunRef *//* VL_Os_0303 */ /* VL_Os_3345 */ /* VL_Os_3442 */ /* VL_Os_4399 */ 
void Os_InterruptInstall(const Os_IsrCfgType* IsrCfg, const Os_IntInstallType* IntInstall)
{
    uint32          isrNum;
    uint32          coreId     = IsrCfg->IsrSrcType;
    uint32          isrSrc     = IsrCfg->IsrSrc;
    uint16          isrPrio    = IntInstall->IntPrio;
    const Os_isrhnd isrHandler = IntInstall->IntHandler;

    if (0U != (isrSrc & OS_EIINT_TYPE))
    {
        isrNum                      = OS_EIINTSRC_2_NUM(isrSrc);
        OS_INTC_EICn(isrNum) = (uint16)(OS_INTC_EICn(isrNum) & ((uint16)~OS_EIC_P_FEILD_MASK)) | 
                       (INT_PRIORITY_LOG_TO_PHY(isrPrio) & OS_EIC_P_FEILD_MASK);
        OS_INTC_EICn(isrNum) &= ~((uint16)1U << OS_EIC_TB_BIT);
        OS_INTC_EICn(isrNum) &= ~((uint16)1U << OS_EIC_MK_BIT);

        if (isrNum >= 32UL)
        {
            OS_INTC_EIBDn(isrNum) |= coreId;
        }
        Os_Vector_Handler[isrNum] = isrHandler;
    }
    else
    {
        isrNum = OS_FEINTSRC_2_NUM(isrSrc);
        OS_FEINTMSK(coreId) &= ~(uint32)(1UL << isrNum);
        Os_FE_Handler[isrNum] = isrHandler;
    }
}
/* PRQA S 1532, 0303, 3442, 3345, 4399 -- */
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
    uint16               i;
    const Os_IsrCfgType* pIsrCfg;
    uint32               isrSrc;
    uint16               idStartRange = (uint16)Os_IsrIdRange[CoreId].AllIsr.IsrStart;
    uint16               idEndRange   = (uint16)Os_IsrIdRange[CoreId].AllIsr.IsrEnd;

    for (i = idStartRange; i < idEndRange; i++)
    {
        pIsrCfg = &Os_IsrCfg[i];
        isrSrc  = pIsrCfg->IsrSrc;

        if (Os_TPTimer_ISR[CoreId] != isrSrc)
        {
            Os_Hal_DisableInterruptSource(isrSrc, CoreId);
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
    uint16               i;
    const Os_IsrCfgType* pIsrCfg;
    uint32               isrSrc;
    uint16               idStartRange = (uint16)Os_IsrIdRange[CoreId].AllIsr.IsrStart;
    uint16               idEndRange   = (uint16)Os_IsrIdRange[CoreId].AllIsr.IsrEnd;

    for (i = idStartRange; i < idEndRange; i++)
    {
        pIsrCfg = &Os_IsrCfg[i];
        isrSrc  = pIsrCfg->IsrSrc;
        if (Os_TPTimer_ISR[CoreId] != isrSrc)
        {
            Os_Hal_EnableInterruptSource(isrSrc, CoreId);
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
/*=======[E N D   O F   F I L E]==============================================*/
