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
**  FILENAME    : Mcu_Timer.h                                                 **
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
#ifndef MCU_TIMER_H
#define MCU_TIMER_H

/*=======[I N C L U D E S]=================================================*/
#include "Arch_Define.h"
#include "Os_Cfg.h"

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
#define OS_STM0_ICR  (0xF000103Cu)
#define OS_STM1_ICR  (0xF000113Cu)
#define OS_STM2_ICR  (0xF000123Cu)
#define OS_STM3_ICR  (0xF000133Cu)
#define OS_STM4_ICR  (0xF000143Cu)
#define OS_STM5_ICR  (0xF000153Cu)

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

#define Os_TmProtFaultProc()

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)

#define Os_ArchLoadRatioInit()

#define OS_SYSTEM_TIMER_MAX_TICK_VALUE   (0xFFFFFFFFU)

#define OS_SYSTEM_TIMER_REG_PER_MS_VALUE (100000U)
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

/*=======[T Y P E   D E F I N I T I O N S]==================================*/

/*=======[E X T E R N A L   D A T A]========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
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
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchInitSystemTimer(void);
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

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
 * CallByAPI            <Os_ArchInitCPU>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchInitTimingProtTimer(void);
#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */
/*=======[I N T E R N A L   D A T A]========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

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
extern FUNC(uint32, OS_CODE) Os_ArchGetTimeTicks(void);
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

/*=======[I N T E R N A L   D A T A]========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

#endif /* MCU_TIMER_H */
/*=======[E N D   O F   F I L E]==============================================*/
