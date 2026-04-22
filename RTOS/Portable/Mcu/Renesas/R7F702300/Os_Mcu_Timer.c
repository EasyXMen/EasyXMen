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
 **  FILENAME    : Os_Mcu_Timer.c                                              **
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
/* PRQA S 3108-- */

/*=======[I N C L U D E S]====================================================*/
#include "Os_Mcu_Timer.h"

/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 0303, 1514 ++ */ /* VL_Os_0303 */ /* VL_Os_1514 */
volatile Os_STMnType* const Os_SystickTimerArray[OS_AUTOSAR_CORES] = {
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
/* PRQA S 0303, 1514 -- */ 
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 0303, 1504 ++ */ /* VL_Os_0303 */ /* VL_Os_1504 */
volatile Os_STMnType* const Os_TPTimerkArray[OS_AUTOSAR_CORES] = {
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
/* PRQA S 0303, 1504 -- */ 
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
static volatile uint32 Os_ArchTpLastReadValue;
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static volatile uint32 Os_McuLRLastCNT;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
static volatile uint64 Os_McuLRTotalCNT;
#define OS_STOP_SEC_VAR_CLONE_32
#include "Os_MemMap.h"
#endif

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
 * CallByAPI            <Os_Hal_InitCPU>
 */
/******************************************************************************/
void Os_ArchInitSystemTimer(Os_CoreIdType coreId)
{
    volatile Os_STMnType* pOSTM = (volatile Os_STMnType*)Os_SystickTimerArray[coreId];

    pOSTM->CNT = 0;
    /* Set the start value of the down-counter or the value for comparison in the OSTMnCMP register */
    pOSTM->CMP = CFG_REG_OSTIMER_VALUE_CORE0 - 1;
    /* OSTMn interrupt request is enabled. */
    pOSTM->CTL |= (uint8)(1U << 7);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*****************************************************************************/
/*
 * Brief                <CPU Load ratio initialization>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <>
 */
/*****************************************************************************/
void Os_Hal_LoadRatioInit(void)
{
    volatile Os_STMnType* pOSTM = (volatile Os_STMnType*)OS_STM8_INSTANCE;

    pOSTM->CNT = 0;

    /* Set the start value of the down-counter or the value for comparison in the OSTMnCMP register */
    pOSTM->CMP = 0xFFFFFFFFU;

    /* Free-run compare mode. */
    pOSTM->CTL |= (uint8)(1U << 1);

    pOSTM->TS = (uint8)0x01U;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
Os_TimerTickType Os_Hal_GetTimeTicks(void)
{
    /* TIM0 as global base time(us) */

    uint32 NowCNT = OS_STM8_INSTANCE->CNT;
    uint32 deltaCNT;


    if (NowCNT > Os_McuLRLastCNT)
    {
        deltaCNT = NowCNT - Os_McuLRLastCNT;
    }
    else
    {
        deltaCNT = OS_SYSTEM_TIMER_MAX_TICK_VALUE - Os_McuLRLastCNT + NowCNT;
    }

    Os_McuLRLastCNT = NowCNT;

    if (Os_McuLRTotalCNT < OS_SYSTEM_TIMER_MAX_TICK_VALUE)
    {
        Os_McuLRTotalCNT = NowCNT;
    }
    else{
        Os_McuLRTotalCNT += deltaCNT;
    }

    
    return (Os_TimerTickType)(Os_McuLRTotalCNT*((uint64)(OS_NS_PER_MS/CFG_REG_OSTIMER_VALUE_CORE0)));
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
 * CallByAPI            <Os_Hal_InitCPU>
 */
/******************************************************************************/
/* PRQA S 3442, 3345 ++ */ /* VL_Os_3442 */ /* VL_Os_3345 */
void Os_ArchInitTimingProtTimer(Os_CoreIdType coreId)  /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    volatile Os_STMnType* pOSTM = (volatile Os_STMnType*)Os_TPTimerkArray[coreId];

    pOSTM->CNT             = 0U;
    Os_ArchTpLastReadValue = pOSTM->CNT;
    pOSTM->CMP             = Os_ArchTpLastReadValue + (uint32)OS_HAL_TP_MAX_INCREMENT_VALUE; 
    /* Free-run compare mode. */
    pOSTM->CTL |= (uint8)(1U << 1U); 
    /* OSTMn interrupt request is enabled. */
    pOSTM->CTL |= (uint8)(1U << 7U); /* PRQA S 3120 */ /* VL_QAC_MagicNum*/
}
/* PRQA S 3442, 3345 -- */
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
    uint64                timePass;
    const volatile Os_STMnType* pOSTM       = (volatile Os_STMnType*)Os_TPTimerkArray[coreId];
    uint32                counterTime = pOSTM->CNT; /* PRQA S 0303 */

    /* PRQA S 3442, 1258++ */ /* VL_Os_3442,VL_Os_1258 */
    if (counterTime >= Os_ArchTpLastReadValue)
    {
        timePass = (uint64)counterTime - Os_ArchTpLastReadValue;
    }
    else
    {
        timePass = (uint64)OS_SYSTEM_TIMER_MAX_TICK_VALUE - Os_ArchTpLastReadValue + counterTime;
    }
    /* PRQA S 3442, 1258-- */

    Os_ArchTpLastReadValue = counterTime;
    return timePass;
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
    volatile Os_STMnType* pOSTM = (volatile Os_STMnType*)Os_TPTimerkArray[coreId];

    pOSTM->CMP = pOSTM->CNT + Increment;/* PRQA S 3442, 3345 */ /* VL_Os_3442 */ /* VL_Os_3345 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
/*=======[E N D   O F   F I L E]==============================================*/
