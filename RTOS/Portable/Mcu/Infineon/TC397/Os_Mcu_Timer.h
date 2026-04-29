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
 ************************************************************************************************************************
 **
 **  @file               : Os_Mcu_Timer.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Mcu header file for Timer api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_TIMER_H
#define OS_TIMER_H

/*=======[I N C L U D E S]=================================================*/
#include "Os_Arch_Define.h"
#include "Os_Tprot.h"

/*=======[M A C R O S]=====================================================*/
#define OS_SCU_CCUCON0 OS_REG32(0xF0036030u)
#define OS_SCU_CCUCON1 OS_REG32(0xF0036034u)

/*
 * STM register
 */
#define OS_MODULE_BASE_STM0 (0xF0001000u)
#define OS_MODULE_BASE_STM1 (0xF0001100u)
#define OS_MODULE_BASE_STM2 (0xF0001200u)
#define OS_MODULE_BASE_STM3 (0xF0001300u)
#define OS_MODULE_BASE_STM4 (0xF0001400u)
#define OS_MODULE_BASE_STM5 (0xF0001500u)

/* Compare Match Control Register */
#define OS_STM0_CMCON (0xF0001038u)
#define OS_STM1_CMCON (0xF0001138u)
#define OS_STM2_CMCON (0xF0001238u)
#define OS_STM3_CMCON (0xF0001338u)
#define OS_STM4_CMCON (0xF0001438u)
#define OS_STM5_CMCON (0xF0001538u)

/* Compare Register */
#define OS_STM0_CMP0 (0xF0001030u)
#define OS_STM0_CMP1 (0xF0001034u)
#define OS_STM1_CMP0 (0xF0001130u)
#define OS_STM1_CMP1 (0xF0001134u)
#define OS_STM2_CMP0 (0xF0001230u)
#define OS_STM2_CMP1 (0xF0001234u)
#define OS_STM3_CMP0 (0xF0001330u)
#define OS_STM3_CMP1 (0xF0001334u)
#define OS_STM4_CMP0 (0xF0001430u)
#define OS_STM4_CMP1 (0xF0001434u)
#define OS_STM5_CMP0 (0xF0001530u)
#define OS_STM5_CMP1 (0xF0001534u)

/* Interrupt Control Register */
#define OS_STM0_ICR (0xF000103Cu)
#define OS_STM1_ICR (0xF000113Cu)
#define OS_STM2_ICR (0xF000123Cu)
#define OS_STM3_ICR (0xF000133Cu)
#define OS_STM4_ICR (0xF000143Cu)
#define OS_STM5_ICR (0xF000153Cu)

#define OS_STM0_TIM0 (0xF0001010u)
#define OS_STM1_TIM0 (0xF0001110u)
#define OS_STM2_TIM0 (0xF0001210u)
#define OS_STM3_TIM0 (0xF0001310u)
#define OS_STM4_TIM0 (0xF0001410u)
#define OS_STM5_TIM0 (0xF0001510u)

/* Interrupt Set/Clear Register */
#define OS_STM0_ISCR (0xF0001040u)
#define OS_STM1_ISCR (0xF0001140u)
#define OS_STM2_ISCR (0xF0001240u)
#define OS_STM3_ISCR (0xF0001340u)
#define OS_STM4_ISCR (0xF0001440u)
#define OS_STM5_ISCR (0xF0001540u)

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define Os_TmProtFaultProc()
#endif
/* PRQA S 3332 ++ */ /* VL_Os_3332 */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE || TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
/* PRQA S 3332 -- */
#define OS_SYSTEM_TIMER_MAX_TICK_VALUE (0xFFFFFFFFU)
/* Set max increment value = (OS_SYSTEM_TIMER_MAX_TICK_VALUE + 1)/ 2 */
#define OS_HAL_TP_MAX_INCREMENT_VALUE (0x80000000U)
#endif

#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
/* The direction of growth of the system clock */
#define OS_TIMER_COUNT_UP       0x01U
#define OS_TIMER_COUNT_DOWN     0x02U

/* The reload method of the system clock */
#define OS_TIMER_AUTO_RELOAD    0x01U
#define OS_TIMER_ONE_SHOT       0x02U

/* The bit width of the system clock */
#define OS_TIMER_16BIT          16U
#define OS_TIMER_24BIT          24U
#define OS_TIMER_32BIT          32U
#define OS_TIMER_64BIT          64U

#ifndef OS_TIMER_DIRECTION
#define OS_TIMER_DIRECTION      OS_TIMER_COUNT_UP
#endif

#ifndef OS_TIMER_RELOAD_MODE
#define OS_TIMER_RELOAD_MODE    OS_TIMER_AUTO_RELOAD
#endif

#ifndef OS_TIMER_BIT_WIDTH
#define OS_TIMER_BIT_WIDTH      OS_TIMER_32BIT
#endif

#define OS_NS_PER_SEC           1000000000ULL   /* 1s = 1,000,000,000ns */
#define OS_NS_PER_MS            1000000ULL  /* 1ms = 1,000,000ns */
#define OS_MS_PER_TICK          CFG_REG_OSTIMER_VALUE_CORE0  /* 1 ms = xxx ticks */
#define OS_HAL_SYSTEM_TIMER_REG_PER_MS_VALUE          OS_MS_PER_TICK  /* 1 ms = xxx ticks */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
/* Tick to ns */
#define OS_TIMER_TICKS_TO_NS(ticks) \
    ((uint64)(((ticks) * OS_NS_PER_SEC) / CFG_SYSTEM_TIMER_FREQ))

/* Get current hardware timer value */
#define OS_GET_HW_TIMER()          OS_REG32(OS_STM0_TIM0)

/* Get max timer value */
#ifndef OS_GET_HW_TIMER_MAX
#define OS_GET_HW_TIMER_MAX     ((1ULL << OS_TIMER_BIT_WIDTH) - 1)
#endif

/* Get last compare value, only applicable to the compare register. */
#if (TRUE == OS_HAL_SYSTEM_TIMER_IS_USED_CMP)
#define OS_GET_BASE_CMP()           (Os_TimerStamp.baseCmpValue)
#endif /* TRUE == OS_HAL_SYSTEM_TIMER_IS_USED_CMP */

#if (OS_TIMER_COUNT_UP == OS_TIMER_DIRECTION)
/* Calculate elapsed ticks, it is necessary to consider the direction of hardware timer growth, reload mode, and so on.*/
#if (OS_TIMER_AUTO_RELOAD == OS_TIMER_RELOAD_MODE)
#define OS_TIMER_CALCULATE_TICKS(current, last)  \
    (((current) >= (last)) ? ((current) - (last)) : \
        (OS_GET_HW_TIMER_MAX - (last) + (current) + 1U))
#endif /* OS_TIMER_AUTO_RELOAD == OS_TIMER_RELOAD_MODE */

/* Calculate base ticks, only applicable to the compare register. */
#if (TRUE == OS_HAL_SYSTEM_TIMER_IS_USED_CMP)
#define OS_TIMER_GET_BASE_TICKS(current, base)  \
    (((current) >= (base)) ? ((current) - OS_MS_PER_TICK) : \
        (OS_GET_HW_TIMER_MAX - (OS_MS_PER_TICK - (current)) + 1U))
#endif /* TRUE == OS_HAL_SYSTEM_TIMER_IS_USED_CMP */
#endif /* OS_TIMER_COUNT_UP == OS_TIMER_DIRECTION */

/* Get elapsed ticks */
#ifndef OS_TIMER_GET_ELAPSED_TICKS
#define OS_TIMER_GET_ELAPSED_TICKS()      OS_TIMER_CALCULATE_TICKS(OS_GET_HW_TIMER(), OS_GET_BASE_CMP())
#endif
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */
/* PRQA S 3472 -- */

/*=======[T Y P E   D E F I N I T I O N S]==================================*/
typedef uint32 Os_Hal_TpType;
/*=======[E X T E R N A L   D A T A]========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
/******************************************************************************/
/*
 * Brief                <Set System Clock>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_MultiCoreInit>
 * REQ ID               <None>
 */
/******************************************************************************/
extern void Os_McuSetSystemClock(void);

/******************************************************************************/
/*
 * Brief                <Initialize the system clock for each CPU running OS.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_Hal_InitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
extern void Os_McuInitSystemTimer(Os_CoreIdType coreId);

extern void Os_McuClearSysTimer(Os_CoreIdType coreId);
#endif /* #if (TRUE == CFG_SYSTEM_TIMER_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/******************************************************************************/
/*
 * Brief                <Initialize a time-protected clock for each CPU running OS>
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
extern void Os_McuInitTimingProtTimer(Os_CoreIdType coreId);

extern void Os_McuClearTmProtTimer(Os_CoreIdType coreId);

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
extern uint64 Os_Hal_TpGetTimePass(Os_CoreIdType coreId);

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
extern void Os_Hal_TpSetCompare(Os_CoreIdType coreId, uint32 Increment);
#endif /* #if (TRUE == CFG_SYSTEM_TIMER_ENABLE) */ /* PRQA S 2053 */ /* VL_Os_2053 */

#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
/******************************************************************************/
/*
 * Brief                <Get current time ticks>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <Current time ticks>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern Os_TimerTickType Os_Hal_GetTimeTicks(void);
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */
/*=======[I N T E R N A L   D A T A]========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

#endif /* OS_TIMER_H */
/*=======[E N D   O F   F I L E]==============================================*/
