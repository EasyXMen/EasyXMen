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
 **  FILENAME    : Os_Mcu_Timer.h                                              **
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
#ifndef OS_MCU_TIMER_H
#define OS_MCU_TIMER_H
/*=======[I N C L U D E S]====================================================*/
#include "Os_Cfg.h"
#include "Os_Tprot_Cfg.h"
#include "Os_Osm_Cfg.h"
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/
/* MSRKCPROT register is used for protection against writing operation to the registers which may have
a material effect on the system so that the application system is not incorrectly stopped due to program
malfunction and the like */
#define OS_MCU_MSRKCPROT     (*(volatile uint32*)(0xFF981710U))
#define OS_MCU_KCPORT_DIABLE (0xA5A5A500U) /* Disables write access of protected registers */
#define OS_MCU_KCPORT_ENABLE (0xA5A5A501U) /* Enables write access of protected registers */

/* This register is used to control the stop modes of the OSTM. */
#define OS_MCU_MSR_OSTM (*(volatile uint32*)(0xFF981180U))

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
/* enable OSTMx */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_MCU_ENABLE_SYSTIMER(Pen)                                     \
    do                                                                  \
    {                                                                   \
        const Os_CoreIdType local_core_id       = (Os_CoreIdType)(Pen); \
        Os_SystickTimerArray[local_core_id]->TS = (uint8)0x01U;         \
    } while (0)
/* PRQA S 3472 -- */
/* empty for U2Ax */
#define Os_Hal_ClearSysTimer(coreID)
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/* enable STM0 CNT3 Channel A */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_MCU_ENABLE_TPTIMER(Pen)                                  \
    do                                                              \
    {                                                               \
        const Os_CoreIdType local_core_id   = (Os_CoreIdType)(Pen); \
        Os_TPTimerkArray[local_core_id]->TS = (uint8)0x01U;         \
    } while (0)
/* PRQA S 3472 -- */
/* empty for U2Ax */
#define Os_Hal_ClearTmProtTimer(coreID)

#define Os_TmProtFaultProc()
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

#define OS_HAL_TP_MAX_INCREMENT_VALUE  (0x80000000UL)
/* Wrap value of the 32-bit free-running hardware timer shared by the
 * monitor and timing protection (NOT the system scheduler timer). */
#define OS_HAL_HWTIMER_MAX_TICK_VALUE (0xFFFFFFFFUL)

/* Monitor timer ticks per millisecond.
 * The monitor timer (TP timer instances OSTM4..7, shared) runs on the same clock source as the system timer
 * (CFG_SYSTEM_TIMER_FREQ); keep in sync with the clock configuration. */
#define OS_HAL_HWTIMER_MS_TO_TICKS (CFG_SYSTEM_TIMER_FREQ / 1000U)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef struct
{
    volatile uint32      CMP;                                                   /* CMP              */
    volatile uint32      CNT;                                                   /* CNT              */
    volatile uint8       TO;                                                    /* TO               */
    volatile uint8       dummy1101[3]; /* Reserved         */ /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    volatile uint8       TOE;                                                   /* TOE              */
    volatile uint8       dummy1102[3]; /* Reserved         */ /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    const volatile uint8 TE;                                                    /* TE               */
    volatile uint8       dummy1103[3]; /* Reserved         */ /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    volatile uint8       TS;                                                    /* TS               */
    volatile uint8       dummy1104[3]; /* Reserved         */ /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    volatile uint8       TT;                                                    /* TT               */
    volatile uint8       dummy1105[7]; /* Reserved         */ /* PRQA S 3132 */ /* VL_QAC_MagicNum */
    volatile uint8       CTL;                                                   /* CTL              */
} Os_McuSTMnType;

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
extern volatile Os_McuSTMnType* const Os_SystickTimerArray[OS_AUTOSAR_CORES];
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
#endif
/* TP timer instances: unconditional because they are also the monitor timer
 * time base (each core reads its own instance counter). */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
extern volatile Os_McuSTMnType* const Os_TPTimerkArray[OS_AUTOSAR_CORES];
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h"
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_McuInitSystemTimer(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

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
 * PreCondition         <None>
 * CallByAPI            <OSM monitor modules (Os_Osm_*)>
 * REQ ID               <None>
 */
/******************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_TimerTickType Os_Hal_GetHwTimerTicks(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void   Os_McuInitTimingProtTimer(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern uint64 Os_Hal_TpGetTimePass(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void   Os_Hal_TpSetCompare(Os_CoreIdType coreId, uint32 Increment);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

#endif
/*=======[E N D   O F   F I L E]==============================================*/
