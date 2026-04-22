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
 **  @file               : Os_ScheduleTable.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for ScheduleTable api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_SCHEDULETABLE_H
#define OS_SCHEDULETABLE_H

/* =================================================== inclusions =================================================== */
#include "Os_ScheduleTable_CfgData.h"

/* ===================================================== macros ===================================================== */
/**
 * @brief SCB and CoreId are obtained by scheduleTableId.
 */
#define OS_SCHEDTBL_GET_COREID(scheduleTableId) (Os_SchedTblCfg[scheduleTableId].HostCore) /* PRQA S 3472 */ /* VL_Os_3472 */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_SCHEDTBL_GET_SCB(scheduleTableId) (&Os_SCB_FA_Inf[0])
#else
#define OS_SCHEDTBL_GET_SCB(scheduleTableId) (Os_SCB_FA_Inf[OS_SCHEDTBL_GET_COREID(scheduleTableId)])
#endif

/**
 * @brief Check if the scheduleTableId on a certain core is valid.
 *
 * Macro parameters:
 *  @param[in] scheduleTableId: The scheduleTableId that needs to be checked.
 *  @param[in] coreId: The core to which the checked scheduleTable belongs.
 */
/* PRQA S 3410 ++ */ /* VL_Os_3410 */
#define Os_CheckSchedTblId(scheduleTableId, coreId) (                          \
    (scheduleTableId >= Os_CfgScheduleTableIndex_Inf[coreId].SchedTblStart) && \
    (scheduleTableId < Os_CfgScheduleTableIndex_Inf[coreId].SchedTblEnd))
/* PRQA S 3410 -- */

/* ========================================== external function definitions ========================================= */
#if (CFG_SCHEDTBL_MAX > 0U)
/**
 * @brief              Init ScheduleTable during os_startup.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitScheduleTable(void);

/**
 * @brief              Os_WorkSchedTbl.
 * @param[in]          counterId: The counter that drives the scheduling table.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_WorkSchedTbl(Os_CounterType counterId);

/**
 * @brief              Internal implementation of OS service:StopScheduleTable.
 * @param[in]          schedTblId: ScheduleTable Id.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern StatusType Os_StopScheduleTable(ScheduleTableType schedTblId);
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
