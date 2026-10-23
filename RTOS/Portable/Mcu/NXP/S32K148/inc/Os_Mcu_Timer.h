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
 **  @file               : Os_Arch_Timer.h
 **  @author             : i-soft-os
 **  @date               : 2025/12/12
 **  @vendor             : isoft
 **  @description        : Arch header file for Arch trap handle
 **
 ***********************************************************************************************************************/

/* Peripheral Timer Used, Not realized here, move to Os_Mcu_Timer.c for more inforamtion */

#ifndef OS_MCU_TIMER_H
#define OS_MCU_TIMER_H

/*=======[I N C L U D E S]====================================================*/

#include "Os_Arch_Define.h"
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/

/* ================================================
* Functional Macro Defination Provided to Kernel *
================================================ */

/* 1s = 1,000,000,000ns */
#define OS_NS_PER_SEC                        (1000000000ULL)

/* System Timer counter value per 1 ms */
#define OS_HAL_SYSTEM_TIMER_REG_PER_MS_VALUE CFG_REG_OSTIMER_VALUE_CORE0

/* Hardware Tick to ns */
#define OS_HAL_HWTICKS2NS(ticks)             (((uint64)(ticks) * OS_NS_PER_SEC) / CFG_SYSTEM_TIMER_FREQ)

/* DWT CYCCNT counts core clock cycles, the same source that drives the SysTick
 * programmed by Os_Arch_InitSysTimer() (SYST_CSR.CLKSOURCE = 1). Monitor ticks
 * and system timer ticks therefore advance at the same rate, so the ms->ticks
 * conversion below and the OS_HAL_HWTICKS2NS() conversion above both hold for
 * monitor ticks unchanged, with no separate frequency constant needed. */
/* Monitor Timer counter ownership.
 * TRUE : the OS owns DWT CYCCNT - it is enabled, cleared and used as the OSM
 *        time base.
 * FALSE: CYCCNT is already owned and running (application or debugger); the OS
 *        only adopts the current counter value as baseline and leaves the
 *        counter untouched. */
#define OS_MCU_MONITOR_TIMER_INIT_ENABLE     (TRUE)
#define OS_MCU_DEBUG_DEMCR_ADDR              (0xE000EDFCU) /* bit24 TRCENA */
#define OS_MCU_DWT_CTRL_ADDR                 (0xE0001000U) /* bit0 CYCCNTENA, bit25 NOCYCCNT (RO) */
#define OS_MCU_DWT_CYCCNT_ADDR               (0xE0001004U) /* 32-bit free-running cycle counter */

#define OS_MCU_DEBUG_DEMCR_TRCENA            (OS_1BIT_MASK << OS_NUM_24)
#define OS_MCU_DWT_CTRL_CYCCNTENA            (OS_1BIT_MASK << OS_NUM_0)

/* Monitor Timer counter value per 1 ms */
#define OS_HAL_HWTIMER_MS_TO_TICKS           (CFG_SYSTEM_TIMER_FREQ / 1000U)

/* Tp Timer Max value allowed */
#define OS_HAL_TP_MAX_INCREMENT_VALUE        (0xFFFFFFFFUL)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* ============================
* Function Provided to Kernel *
============================ */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern uint64 Os_Hal_TpGetTimePass(Os_CoreIdType CoreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_TpSetCompare(Os_CoreIdType CoreId, uint32 Increment);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_ClearTmProtTimer(Os_CoreIdType CoreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/* ====================
* Function Local Used *
==================== */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_InitTmProtTimer(Os_CoreIdType CoreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

/******************************************************************************/
/*
 * Brief                <Initialize the monitor timer (DWT CYCCNT free-running, no interrupt)>
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
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Mcu_InitMonitorTimer(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
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
 * PreCondition         <Os_Mcu_InitMonitorTimer has run; call interval < 32-bit CYCCNT wrap>
 * CallByAPI            <OSM monitor modules (Os_Osm_*)>
 * REQ ID               <None>
 */
/******************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_TimerTickType Os_Hal_GetHwTimerTicks(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* OS_MCU_TIMER_H */

/*=======[E N D   O F   F I L E]==============================================*/
