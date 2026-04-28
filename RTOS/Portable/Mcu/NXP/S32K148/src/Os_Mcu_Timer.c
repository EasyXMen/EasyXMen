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
/*
**                                                                            **
**  FILENAME    : Os_Mcu_Timer.c                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the processor timers                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Arch_Processor.h"

/*=======[I N T E R N A L   D A T A]=========================================*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
static volatile uint32 Os_ArchTpLastReadValue;
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

#endif
/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]=======*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <init STM Mode>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 */
/*****************************************************************************/
void Os_ArchInitTimingProtTimer(CoreIdType CoreId)
{
    switch (CoreId)
    {
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE_CORE0)
    case 0:
        /* Initialize Flex Timer instance as simple timer */
        PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK;
        /* The clock is off and the external clock can be started */
        PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(0) | PCC_PCCn_CGC_MASK;
        /*Write protection disabled */
        FTM0->MODE |= FTM_MODE_WPDIS_MASK;
        /*FTM enables , start writing registers*/
        FTM0->MODE |= FTM_MODE_FTMEN_MASK;
        /* Clear status register*/
        FTM0->SC = 0;
        /* Counter overflow Interrupt Frequency divider select 101B Divide by 32,3Mhz*/
        FTM0->SC |= FTM_SC_TOIE_MASK | FTM_SC_PS(5);
        /*Clear counter overflow flag*/
        FTM0->SC &= ~FTM_SC_TOF_MASK;

        /*DECAPENx, MCOMBINEx, COMBINEx=0*/
        FTM0->COMBINE = 0x00000000;
        /*Sets the polarity of the channel output*/
        FTM0->POL = 0x00000000;
        FTM0->CNTIN = 0;
        /*Sets the counter termination value to configure the interrupt time*/
        FTM0->MOD = OS_SYSTEM_TIMER_MAX_TICK_VALUE;
        // FTM0->MOD = 2999;
        /*Select clock source*/
        FTM0->SC |= FTM_SC_CLKS(1); /*Select clock source*/

        break;
#endif
        /*add comments to pass QAC.*/
    default:
        /* Nothing to do. */
        break;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Time protection get time pass, since last read>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
uint64 Os_Hal_TpGetTimePass(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
    (void)coreId;
    uint64 TimePass;
    uint32 CounterTime = FTM0->CNT; /* PRQA S 0303 */ /* VL_Os_0303 */

    /* PRQA S 3442, 1258++ */ /* VL_Os_3442,VL_Os_1258 */
    if (CounterTime >= Os_ArchTpLastReadValue)
    {
        TimePass = (uint64)CounterTime - Os_ArchTpLastReadValue;
    }
    else
    {
        TimePass = (uint64)OS_SYSTEM_TIMER_MAX_TICK_VALUE - Os_ArchTpLastReadValue + CounterTime;
    }
    /* PRQA S 3442, 1258-- */

    Os_ArchTpLastReadValue = CounterTime;
    return TimePass;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Time protection set compare register>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_Hal_TpSetCompare(Os_CoreIdType coreId, uint32 Increment)
/* PRQA S 1532 -- */
{
    (void)coreId;

    FTM0->SC &= ~FTM_SC_CLKS_MASK; /*Select clock source*/
    FTM0->MOD = FTM0->CNT + Increment;
    /*Select clock source*/
    FTM0->SC |= FTM_SC_CLKS(1); /*Select clock source*/
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* #if (TRUE == CFG_TIMING_PROTECTION_ENABLE) */ /* PRQA S 2053 */ /* MISRA Rule 18.8 */
