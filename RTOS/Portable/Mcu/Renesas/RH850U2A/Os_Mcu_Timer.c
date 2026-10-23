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

/*=======[I N C L U D E S]====================================================*/
#include "Os_Mcu_Timer.h"
#include "Os_Types.h"
#include "Os_Cfg.h"
#include "Os_Tprot_Cfg.h"
#include "Os_Osm_Cfg.h"
#include "Os_Arch_Irq.h"

/*=======[M A C R O S]=====================================================*/
/* OSTM0 - OSTM9 register */
#define OS_MCU_OSTM0_BASE_ADDR (0xFFBF0000U)
#define OS_MCU_OSTM1_BASE_ADDR (0xFFBF0100U)
#define OS_MCU_OSTM2_BASE_ADDR (0xFFBF0200U)
#define OS_MCU_OSTM3_BASE_ADDR (0xFFBF0300U)
#define OS_MCU_OSTM4_BASE_ADDR (0xFFBF0400U)
#define OS_MCU_OSTM5_BASE_ADDR (0xFFBF0500U)
#define OS_MCU_OSTM6_BASE_ADDR (0xFFBF0600U)
#define OS_MCU_OSTM7_BASE_ADDR (0xFFBF0700U)
#define OS_MCU_OSTM8_BASE_ADDR (0xFFBF0800U)
#define OS_MCU_OSTM9_BASE_ADDR (0xFFBF0900U)

/* OSTMn instance */
#define OS_MCU_STM0_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM0_BASE_ADDR))
#define OS_MCU_STM1_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM1_BASE_ADDR))
#define OS_MCU_STM2_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM2_BASE_ADDR))
#define OS_MCU_STM3_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM3_BASE_ADDR))
#define OS_MCU_STM4_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM4_BASE_ADDR))
#define OS_MCU_STM5_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM5_BASE_ADDR))
#define OS_MCU_STM6_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM6_BASE_ADDR))
#define OS_MCU_STM7_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM7_BASE_ADDR))
#define OS_MCU_STM8_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM8_BASE_ADDR))
#define OS_MCU_STM9_INSTANCE ((volatile Os_McuSTMnType*)(OS_MCU_OSTM9_BASE_ADDR))
/*=======[E X T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 0303, 1514 ++ */ /* VL_Os_0303 */ /* VL_Os_1514 */
volatile Os_McuSTMnType* const Os_SystickTimerArray[OS_AUTOSAR_CORES] = {
    OS_MCU_STM0_INSTANCE,
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    OS_MCU_STM1_INSTANCE,
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    OS_MCU_STM2_INSTANCE,
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    OS_MCU_STM3_INSTANCE
#endif
};
/* PRQA S 0303, 1514 -- */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif

/* TP timer instance array: unconditional because it is also the monitor
 * timer time base (each core reads its own instance counter). */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/* PRQA S 0303, 1504 ++ */ /* VL_Os_0303 */ /* VL_Os_1504 */
volatile Os_McuSTMnType* const Os_TPTimerkArray[OS_AUTOSAR_CORES] = {
    OS_MCU_STM4_INSTANCE,
#if (TRUE == CFG_CORE1_AUTOSAROS_ENABLE)
    OS_MCU_STM5_INSTANCE,
#endif
#if (TRUE == CFG_CORE2_AUTOSAROS_ENABLE)
    OS_MCU_STM6_INSTANCE,
#endif
#if (TRUE == CFG_CORE3_AUTOSAROS_ENABLE)
    OS_MCU_STM7_INSTANCE
#endif
};
/* PRQA S 0303, 1504 -- */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
static volatile uint32 Os_TpLastReadValue;
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_32
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"
/* Monitor timer state: each core reads its own TP timer instance counter
 * (OSTM4..7) and keeps its own delta accumulation state. */
static Os_MonitorTimerType Os_MonitorTimer[OS_AUTOSAR_CORES];
#define OS_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#include "Os_MemMap.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_McuInitSystemTimer>
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
void Os_McuInitSystemTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    volatile Os_McuSTMnType* pOSTM = (volatile Os_McuSTMnType*)Os_SystickTimerArray[coreId];

    pOSTM->CNT = 0; /* PRQA S 1290 */ /* VL_Os_1290 */
    /* Set the start value of the down-counter or the value for comparison in the OSTMnCMP register */
    pOSTM->CMP = CFG_REG_OSTIMER_VALUE_CORE0 - 1; /* PRQA S 1840 */ /* VL_Os_1840 */
    /* OSTMn interrupt request is enabled. */
    pOSTM->CTL |= (uint8)(1U << 7); /* PRQA S 4544,3442,3120 */ /* VL_Os_4544,VL_Os_3442,VL_QAC_MagicNum */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the monitor timer (shared with TP: per-core OSTM4..7 free-running)>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <coreId> Core identifier
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_Mcu_InitMonitorTimer(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
    /* SHARED TIMER CONTRACT: the monitor shares the per-core TP timer
     * (OSTM4..7, Os_TPTimerkArray) with timing protection to save a dedicated
     * timer instance. Ownership split:
     * - TP owns CMP, the interrupt enable and the init sequence; it clears
     *   the counter once at init and writes CMP only at runtime.
     * - Monitor owns nothing on the hardware: it only reads CNT, plus the
     *   start-up responsibility when TP is disabled.
     * - Neither side may reset the counter at runtime.
     * Runs after the TP init/enable in Os_Hal_InitCPU. */
    volatile Os_McuSTMnType* pOSTM = (volatile Os_McuSTMnType*)Os_TPTimerkArray[coreId]; /* PRQA S 3678 */ /* VL_Os_3678 */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    /* TP has already configured and started the counter; just take the
     * baseline for this core's delta accumulation. */
#else
    /* Without timing protection nobody configures the TP timer instance:
     * start it ourselves as a free-running up-counter (same mode TP uses,
     * but without the interrupt) with the compare parked at the maximum so
     * that no compare event can ever disturb the count. */
    pOSTM->CNT = 0; /* PRQA S 1290 */ /* VL_Os_1290 */
    pOSTM->CMP = 0xFFFFFFFFU; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    pOSTM->CTL |= (uint8)(1U << 1); /* free-run compare mode */ /* PRQA S 4544,3442 */ /* VL_Os_4544,VL_Os_3442 */
    pOSTM->TS  = (uint8)0x01U;     /* start */
#endif

    /* Base the 64-bit accumulation of this core on the current counter value. */
    Os_MonitorTimer[coreId].lastTicks = pOSTM->CNT; /* PRQA S 3345 */ /* VL_Os_3345 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Get the extended 64-bit hardware timer ticks>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <coreId> Core identifier
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <Os_TimerTickType> Monotonic 64-bit accumulated ticks of the monitor timer
 * PreCondition         <None>
 * CallByAPI            <OSM monitor modules (Os_Osm_*)>
 * REQ ID               <None>
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_TimerTickType Os_Hal_GetHwTimerTicks(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
    Os_TimerTickType           curTicks;
    uint32                     now;
    uint32                     delta;
    const volatile Os_McuSTMnType* pOSTM = (volatile Os_McuSTMnType*)Os_TPTimerkArray[coreId];

    /* The per-core TP timer instance (OSTM4..7) is a 32-bit free-running
     * up-counter shared with timing protection (see the SHARED TIMER CONTRACT
     * in Os_Mcu_InitMonitorTimer). Extend it to a 64-bit monotonic time base
     * by per-core delta accumulation. The modulo-2^32 delta is correct across
     * wraps; constraint: the interval between two calls must stay below one
     * full wrap period. The critical section protects the per-core state
     * against nested calls (e.g. from an ISR). */
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    now = pOSTM->CNT; 
    /* PRQA S 3442,3345 ++ */ /* VL_Os_3442,VL_Os_3345 */
    if ((Os_MonitorTimer[coreId].totalTicks == 0U) && (Os_MonitorTimer[coreId].lastTicks == 0U))
    /* PRQA S 3442,3345 -- */
    {
        /* First call on this core: synchronize the baseline instead of
         * accumulating the counter value accumulated since timer start. */
        Os_MonitorTimer[coreId].lastTicks = now;
    }
    else
    {
        /* PRQA S 3442,3345,0404 ++ */ /* VL_Os_3442,VL_Os_3345,VL_Os_0404 */
        delta = (now >= Os_MonitorTimer[coreId].lastTicks)
                    ? (now - Os_MonitorTimer[coreId].lastTicks)
                    : (OS_HAL_HWTIMER_MAX_TICK_VALUE - Os_MonitorTimer[coreId].lastTicks + now + 1U);
        /* PRQA S 3442,3345,0404 -- */
        Os_MonitorTimer[coreId].totalTicks += (Os_TimerTickType)delta; /* PRQA S 3442 */ /*VL_Os_3442*/
        Os_MonitorTimer[coreId].lastTicks = now;
    }
    curTicks = Os_MonitorTimer[coreId].totalTicks;
    OS_HAL_EXIT_CRITICAL();

    return curTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_McuInitTimingProtTimer>
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
/* PRQA S 3442, 3345 ++ */ /* VL_Os_3442 */                            /* VL_Os_3345 */
void Os_McuInitTimingProtTimer(Os_CoreIdType coreId) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    volatile Os_McuSTMnType* pOSTM = (volatile Os_McuSTMnType*)Os_TPTimerkArray[coreId];

    pOSTM->CNT         = 0U;
    Os_TpLastReadValue = pOSTM->CNT;
    pOSTM->CMP         = Os_TpLastReadValue + (uint32)OS_HAL_TP_MAX_INCREMENT_VALUE;
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
    uint64                         timePass;
    const volatile Os_McuSTMnType* pOSTM       = (volatile Os_McuSTMnType*)Os_TPTimerkArray[coreId];
    uint32                         counterTime = pOSTM->CNT; /* PRQA S 0303 */ /* VL_Os_0303 */

    /* PRQA S 3442, 1258++ */ /* VL_Os_3442,VL_Os_1258 */
    if (counterTime >= Os_TpLastReadValue)
    {
        timePass = (uint64)counterTime - Os_TpLastReadValue;
    }
    else
    {
        timePass = (uint64)OS_HAL_HWTIMER_MAX_TICK_VALUE - Os_TpLastReadValue + counterTime;
    }
    /* PRQA S 3442, 1258-- */

    Os_TpLastReadValue = counterTime;
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
    volatile Os_McuSTMnType* pOSTM = (volatile Os_McuSTMnType*)Os_TPTimerkArray[coreId];

    pOSTM->CMP = pOSTM->CNT + Increment; /* PRQA S 3442, 3345 */ /* VL_Os_3442 */ /* VL_Os_3345 */
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */
/*=======[E N D   O F   F I L E]==============================================*/
