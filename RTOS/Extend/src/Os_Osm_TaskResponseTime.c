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
 **  @file               : Os_Osm_TaskResponseTime.c
 **  @author             : i-soft-os
 **  @date               : 2025/12/15
 **  @vendor             : isoft
 **  @description        : Os source file for TaskResponseTime API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Monitor.h"
#include "Os_Kernel.h"

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Initialize the task response time module.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitTaskResponseTime(Os_CoreIdType coreId)
/* PRQA S 1532 -- */
{
    Os_SCBType *pScb = Os_GetSystemContext(coreId);
    Os_TaskResponseTimeRefType Os_TaskResponseTime;
    uint16 i;
    Os_TaskType idStartRange = Os_TaskIdRange[coreId].AllTask.Start;
    Os_TaskType idEndRange = Os_TaskIdRange[coreId].AllTask.End;

    pScb->Os_TaskResponseTime = Os_TaskResponseTime_Inf;
    Os_TaskResponseTime = pScb->Os_TaskResponseTime;

    for (i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
    {
        Os_TaskResponseTime[i].tick.averageTicks = 0ULL;
        Os_TaskResponseTime[i].tick.maxTicks     = 0ULL;
        Os_TaskResponseTime[i].tick.minTicks     = 0ULL;
        Os_TaskResponseTime[i].tick.totalTicks   = 0ULL;
        Os_TaskResponseTime[i].tick.startTicks   = 0ULL;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Record the start time of the task.
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_TaskRecordStartTick(TaskType TaskID)
/* PRQA S 1532 -- */
{
    const Os_SCBType *pScb = Os_GetCurrentContext();

    pScb->Os_TaskResponseTime[TaskID].tick.startTicks = Os_Hal_GetTimeTicks();
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate the total duration of the computing task.
 */
void Os_TaskRecordTotalTick(TaskType TaskID) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    const Os_SCBType *pScb = Os_GetCurrentContext();
    Os_TaskResponseTimeRefType Os_TaskResponseTime = pScb->Os_TaskResponseTime;
    Os_TimerTickType            totalTicks   = Os_GetTickInterval(Os_TaskResponseTime[TaskID].tick.startTicks);
    Os_TaskResponseTimeRefType responseTime  = &Os_TaskResponseTime[TaskID];

    if (totalTicks > responseTime->tick.maxTicks)
    {
        responseTime->tick.maxTicks = totalTicks;
    }

    if ((totalTicks < responseTime->tick.minTicks) || (0ULL == responseTime->tick.minTicks))
    {
        responseTime->tick.minTicks = totalTicks;
    }

    responseTime->taskActCount++;
    responseTime->tick.totalTicks += totalTicks;

    if (responseTime->taskActCount < OS_TASK_RESPONSE_TIME_CALC_NUM)
    {
        /* PRQA S 2834 ++ */ /* VL_Os_2834 */
        responseTime->tick.averageTicks = (responseTime->tick.totalTicks / responseTime->taskActCount);
        /* PRQA S 2834 -- */
    }
    else
    {
        responseTime->taskActCount      = 0U;
        responseTime->tick.averageTicks = (responseTime->tick.totalTicks / OS_TASK_RESPONSE_TIME_CALC_NUM);
        responseTime->tick.totalTicks   = 0ULL;
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
