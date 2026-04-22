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
 ************************************************************************************************************************
 **
 **  @file               : Os_Osm_EventMonitor.c
 **  @author             : i-soft-os
 **  @date               : 2025/12/15
 **  @vendor             : isoft
 **  @description        : Os source file for EventMonitor API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Osm_EventMonitor.h"
#include "Os_Event.h"
#include "Os_Hook.h"

/* ========================================== external function definitions ========================================= */
#if ((TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) || (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR)) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Initialize the event monitor module.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitMonitorEvent(Os_TaskType TaskID)
/* PRQA S 1532 -- */
{
    const Os_EventMaskType* eventMonitorCfg = 
	Os_EventMonitorCfg_Inf[TaskID];
    Os_EventCountType eventIdx;

    for (eventIdx = 0U; eventIdx < CFG_EVENT_COUNT_FOR_TASK_MAX; ++eventIdx)
    {
        if (eventIdx >= Os_EventCountForTaskCfg_Inf[TaskID])
        {
            break;
        }

        Os_EventMonitor[TaskID][eventIdx].eventMask = eventMonitorCfg[eventIdx];

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR)
        Os_EventMonitor[TaskID][eventIdx].eventStartResponseTime = 0ULL;
        Os_EventMonitor[TaskID][eventIdx].eventEndResponseTime   = 0ULL;
        Os_EventMonitor[TaskID][eventIdx].eventResponseTime      = 0ULL;
#endif

#if (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR)
        Os_EventMonitor[TaskID][eventIdx].eventResponseNum = 0U;
#endif
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Gets the response time of the specified event.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_MonitorEventResponseTime(Os_TaskType TaskID, Os_EventMaskType Mask)
/* PRQA S 1532 -- */
{
    Os_EventCountType eventCount = Os_EventCountForTaskCfg_Inf[TaskID];
    Os_EventCountType index;

    for (index = 0U; index < eventCount; ++index)
    {
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if (0U != (Mask & Os_EventMonitor[TaskID][index].eventMask))
        /* PRQA S 3442 -- */
        {
            Os_TimerTickType startRespTime = Os_EventMonitor[TaskID][index].eventStartResponseTime;
            Os_TimerTickType endRespTime   = Os_EventMonitor[TaskID][index].eventEndResponseTime;

            if ((startRespTime != 0ULL) && (endRespTime != 0ULL))
            {
                Os_EventMonitor[TaskID][index].eventResponseTime = endRespTime - startRespTime;

                /* PRQA S 3442, 1258 ++ */ /* VL_Os_3442, VL_Os_ConstToIntegral */
                if (Os_EventMonitor[TaskID][index].eventResponseTime > (Os_TimerTickType)CFG_EVENT_RESPONSE_TIME_LIMIT )
                /* PRQA S 3442, 1258 -- */
                {
                    /* PRQA S 0404, 3345, 3442, 4461 ++ */ /* VL_Os_0404, VL_Os_3345, VL_Os_3442, VL_Os_4461 */
                    OS_EVENT_RESPONSE_TIME_HOOK(
                    /* PRQA S 0404, 3345, 3442, 4461 -- */
                        TaskID,
                        Os_EventMonitor[TaskID][index].eventMask,
                        Os_EventMonitor[TaskID][index].eventResponseTime);
                }
            }

            if (startRespTime > 0ULL)
            {
                Os_EventMonitor[TaskID][index].eventStartResponseTime = 0ULL;
                Os_EventMonitor[TaskID][index].eventEndResponseTime   = 0ULL;
                Os_EventMonitor[TaskID][index].eventResponseTime      = 0ULL;
            }
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Gets the end time for monitoring event response time.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_MonitorEventEndTime(Os_TaskType TaskID, Os_EventMaskType Mask)
/* PRQA S 1532 -- */
{
    OS_HAL_DECLARE_CRITICAL();
    OS_HAL_ENTRY_CRITICAL();
    Os_TimerTickType    endRespTime = Os_Hal_GetTimeTicks();
    Os_EventCountType eventCount  = Os_EventCountForTaskCfg_Inf[TaskID];
    Os_EventCountType index;

    /* PRQA S 1338 ++ */ /* VL_Os_1338 */
    Mask &= Os_ECB[TaskID]->SetEventMask;
    /* PRQA S 1338 -- */
    for (index = 0U; index < eventCount; ++index)
    {
        /* PRQA S 3442,3345 ++ */ /* VL_Os_3442,VL_Os_3345 */
        if ((((Mask) & (Os_EventMonitor[TaskID][index].eventMask)) > 0U)
        /* PRQA S 3442,3345 -- */
            && (0ULL == Os_EventMonitor[TaskID][index].eventEndResponseTime))
        {
            Os_EventMonitor[TaskID][index].eventEndResponseTime = endRespTime;
        }
    }
    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Gets the start time for monitoring event response time.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_MonitorEventStartTime(Os_TaskType TaskID, Os_EventMaskType Mask)
/* PRQA S 1532 -- */
{
    Os_TimerTickType    startRespTime = Os_Hal_GetTimeTicks();
    Os_EventCountType eventCount    = Os_EventCountForTaskCfg_Inf[TaskID];
    Os_EventCountType index;

    for (index = 0U; index < eventCount; ++index)
    {
        /* PRQA S 3442,3345 ++ */ /* VL_Os_3442,VL_Os_3345 */
        if ((((Mask) & (Os_EventMonitor[TaskID][index].eventMask)) > 0U)
        /* PRQA S 3442,3345 -- */
            && (0ULL == Os_EventMonitor[TaskID][index].eventStartResponseTime))
        {
            Os_EventMonitor[TaskID][index].eventStartResponseTime = startRespTime;
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Adds the response number.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_AddEventResponseNum(Os_TaskType TaskID, Os_EventMaskType Mask)
/* PRQA S 1532 -- */
{
    Os_EventCountType eventCount = Os_EventCountForTaskCfg_Inf[TaskID];
    Os_EventCountType index;

    for (index = 0U; index < eventCount; ++index)
    {
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if ((Mask & Os_EventMonitor[TaskID][index].eventMask) > 0U)
        /* PRQA S 3442 -- */
        {
            /* PRQA S 3442,4434 ++ */ /* VL_Os_3442,VL_Os_4434 */
            Os_EventMonitor[TaskID][index].eventResponseNum += 1U;
            /* PRQA S 3442,4434 -- */
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Monitors the response number.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_MonitorEventResponseRate(Os_TaskType TaskID, Os_EventMaskType Mask)
/* PRQA S 1532 -- */
{
    Os_EventCountType eventCount = Os_EventCountForTaskCfg_Inf[TaskID];
    Os_EventCountType index;

    for (index = 0U; index < eventCount; ++index)
    {
        /* PRQA S 3442 ++ */ /* VL_Os_3442 */
        if ((Mask & Os_EventMonitor[TaskID][index].eventMask) > 0U)
        /* PRQA S 3442 -- */
        {
            uint16 respNum = Os_EventMonitor[TaskID][index].eventResponseNum;
            if (respNum > 1U)
            {
                /* PRQA S 3442 ++ */ /* VL_Os_3442 */
                OS_EVENT_RESPONSE_RATE_HOOK(
                /* PRQA S 3442 -- */
                    TaskID,
                    Os_EventMonitor[TaskID][index].eventMask,
                    respNum);
            }

            if (respNum > 0U)
            {
                Os_EventMonitor[TaskID][index].eventResponseNum = 0U;
            }
        }
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif
/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
