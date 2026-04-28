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
 **  FILENAME    : Os_Osm_EventMonitor.h                                       **
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
#ifndef OS_EVENTMONITOR_H
#define OS_EVENTMONITOR_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Osm_CfgData.h"

/*=======[M A C R O S]========================================================*/
#if (TRUE == CFG_EVENT_RESPONSE_TIME_HOOK) /* PRQA S 3332 */ /* VL_Os_3332 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_EVENT_RESPONSE_TIME_HOOK(taskId, eventMask, respTime) EventResponseTimeHook(taskId, eventMask, respTime)
/* PRQA S 3472 -- */
#else
#define OS_EVENT_RESPONSE_TIME_HOOK(taskId, eventMask, respTime)
#endif

#if (TRUE == CFG_EVENT_RESPONSE_RATE_HOOK) /* PRQA S 3332 */ /* VL_Os_3332 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_EVENT_RESPONSE_RATE_HOOK(taskId, eventMask, respNum) EventResponseRateHook(taskId, eventMask, respNum)
/* PRQA S 3472 -- */
#else
#define OS_EVENT_RESPONSE_RATE_HOOK(taskId, eventMask, respNum)
#endif


/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if ((TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) || (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR)) /* PRQA S 3332 */ /* VL_Os_3332 */
/**
 * @brief          The service is used to initialize the event monitor.
 * @param[in]      TaskID: The id of task.
 * @reentrant      FALSE
 * @synchronous    FALSE
 * @trace          -
 */
extern void Os_InitMonitorEvent(Os_TaskType TaskID);
#endif

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
/**
 * @brief          The service is used to get the response time.
 * @param[in]      TaskID: The id of task.
 * @param[in]      Mask: The event mask value of task.
 * @reentrant      FALSE
 * @synchronous    FALSE
 * @trace          -
 */
extern void Os_MonitorEventResponseTime(Os_TaskType TaskID, Os_EventMaskType Mask);

 /**
 * @brief          The service is used to get the end time for monitoring
 *                 event response time.
 * @param[in]      TaskID: The id of task.
 * @param[in]      Mask: The event mask value of task.
 * @reentrant      FALSE
 * @synchronous    FALSE
 * @trace          -
 */
extern void Os_MonitorEventEndTime(Os_TaskType TaskID, Os_EventMaskType Mask);

/**
 * @brief          The service is used to get the start time for monitoring
 *                 event response time.
 * @param[in]      TaskID: The id of task.
 * @param[in]      Mask: The event mask value of task.
 * @reentrant      FALSE
 * @synchronous    FALSE
 * @trace          -
 */
extern void Os_MonitorEventStartTime(Os_TaskType TaskID, Os_EventMaskType Mask);
#endif

#if (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
/**
 * @brief          The service is used to add the response number.
 * @param[in]      TaskID: The id of task.
 * @param[in]      Mask: The event mask value of task.
 * @reentrant      FALSE
 * @synchronous    FALSE
 * @trace          -
 */
extern void Os_AddEventResponseNum(Os_TaskType TaskID, Os_EventMaskType Mask);

/**
 * @brief          The service is used to monitor the response number.
 * @param[in]      TaskID: The id of task.
 * @param[in]      Mask: The event mask value of task.
 * @reentrant      FALSE
 * @synchronous    FALSE
 * @trace          -
 */
extern void Os_MonitorEventResponseRate(Os_TaskType TaskID, Os_EventMaskType Mask);
#endif

#endif
/*=======[E N D   O F   F I L E]==============================================*/
