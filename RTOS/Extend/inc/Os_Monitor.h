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
 **  FILENAME    : Os_Monitor.h                                              **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
#ifndef OS_MONITOR_H
#define OS_MONITOR_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Osm_LoadRatio.h"
#include "Os_Osm_TaskResponseTime.h"
#include "Os_Osm_EventMonitor.h"
#include "Os_Osm_InterLockTime.h"
#include "Os_Osm_ScheduleCount.h"

/*=======[M A C R O S]========================================================*/
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_MS_TO_TICKS(ms) ((Os_TimerTickType)(ms) * (Os_TimerTickType)OS_HAL_HWTIMER_MS_TO_TICKS)
/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#define OS_US_TO_TICKS(us) ((Os_TimerTickType)(us) * (Os_TimerTickType)OS_HAL_HWTIMER_MS_TO_TICKS / 1000U)
/* PRQA S 3120 -- */
/* PRQA S 3472 -- */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Initialize the OS monitor module.
 * @param[in]      coreId: The id of core.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern void Os_InitOsMonitor(Os_CoreIdType coreId); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if ((TRUE == CFG_INTERRUPT_MONITOR_ENABLE) || (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE))
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief          Gets the consumed time tick value.
 * @param[in]      startTicks: The start time tick value.
 * @return         Os_TimerTickType
 * @retval         Return the consumed time tick value.
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          -
 */
extern Os_TimerTickType Os_GetTickInterval(Os_TimerTickType startTicks); /* PRQA S 1512 */ /* VL_Os_1512 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
#endif

#endif
/*=======[E N D   O F   F I L E]==============================================*/
