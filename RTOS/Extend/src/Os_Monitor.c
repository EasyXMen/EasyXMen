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
 **  @file               : Os_Monitor.c
 **  @author             : i-soft-os
 **  @date               : 2025/12/15
 **  @vendor             : isoft
 **  @description        : Os source file for Monitor API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Monitor.h"
#include "Os_Osm_LoadRatio.h"
#include "Os_Osm_InterLockTime.h"
#include "Os_Osm_TaskResponseTime.h"

/* ========================================== external function definitions ========================================= */
#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Initialize the OS monitor module.
 */
/* PRQA S 1532, 3408, 1512 ++ */ /* VL_QAC_OneFunRef, VL_Os_3408, VL_Os_1512 */
void Os_InitOsMonitor(Os_CoreIdType coreId)
/* PRQA S 1532, 3408, 1512 -- */
{
#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
    Os_InitLoadRatio(coreId);
#endif

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE)
    Os_InitTaskResponseTime(coreId);
#endif

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE)
    Os_InterMonitorInit(coreId);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Gets the consumed time tick value.
 */
/* PRQA S 3408 ++ */ /* VL_Os_3408 */
Os_TimerTickType Os_GetTickInterval(Os_TimerTickType startTicks) /* PRQA S 1512 */ /* VL_Os_1512 */
/* PRQA S 3408 -- */
{
    Os_TimerTickType curTicks = Os_Hal_GetTimeTicks();

    return curTicks - startTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0553, 6610 EOF */ /* VL_QAC_UnUsedFiles, VL_MTR_Os_STCDN */
