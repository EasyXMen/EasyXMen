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
 **  FILENAME    : Mcu_Timer.c                                                 **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Timer driver                                                **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"

/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
volatile STMn_t* systickTimerArray[OS_AUTOSAR_CORES] = {
    OS_STM0_INSTANCE,
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    OS_STM1_INSTANCE,
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    OS_STM2_INSTANCE,
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    OS_STM3_INSTANCE
#endif
};
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
volatile STMn_t* tpTimerkArray[OS_AUTOSAR_CORES] = {
    OS_STM4_INSTANCE,
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    OS_STM5_INSTANCE,
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    OS_STM6_INSTANCE,
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    OS_STM7_INSTANCE
#endif
};
#endif
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
/******************************************************************************/
/*
 * Brief                <Os_ArchInitSystemTimer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchInitSystemTimer(void)
{
    volatile STMn_t* pOSTM = (volatile STMn_t*)systickTimerArray[Os_SCB.sysCore];

    pOSTM->CNT = 0;

    /* Set the start value of the down-counter or the value for comparison in the OSTMnCMP register */
    pOSTM->CMP = CFG_REG_OSTIMER_VALUE_CORE0 - 1;

    /* OSTMn interrupt request is enabled. */
    pOSTM->CTL |= (uint8)(1U << 7);
}
#endif /* #if (TRUE == CFG_SYSTEM_TIMER_ENABLE) */

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
/******************************************************************************/
/*
 * Brief                <Get current time in the OS.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_GetCurrentTime>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(uint32, OS_CODE) Os_ArchGetTimeTicks(void)
{
    /* TIM1 as global base time(us) */
    return OS_OSTM1_CNT; /* PRQA S 0303 */ /* MISRA Rule 11.4 */
}
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/******************************************************************************/
/*
 * Brief                <Os_ArchInitTimingProtTimer>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ArchInitCPU>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_ArchInitTimingProtTimer(void)
{
    volatile STMn_t* pOSTM = (volatile STMn_t*)tpTimerkArray[Os_SCB.sysCore];

    pOSTM->CNT = 0;
    pOSTM->CMP = pOSTM->CNT + CFG_REG_OSTIMER_VALUE_CORE0;
    pOSTM->CTL |= (uint8)(1U << 7);
}
#endif /* #if (TRUE == CFG_TIMING_PROTECTION_ENABLE) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
