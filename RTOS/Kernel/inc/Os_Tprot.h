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
 **  @file               : Os_Tprot.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for Tprot api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_TPROT_H
#define OS_TPROT_H

/* =================================================== inclusions =================================================== */
#include "Os_Tprot_CfgData.h"

/* ===================================================== macros ===================================================== */
#define OS_TMPROT_TARGET_ISR_OFFSET (2U)

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/**
 * @brief Initialize the time protection control block.
 *
 * Macro parameters:
 *  @param[in] pCbData: Time protection control block.
 *  @param[in] type: The protected types of operations for time protection.
 *  @param[in] budget: Time protection budget.
 */
/* PRQA S 3412 ++ */ /* VL_Os_3412 */
#define Os_TmProtInitCbData(pCbData, type, budget) \
    (pCbData)->TpEndTime  = 0U;                    \
    (pCbData)->TpNodeOpt  = (type);                \
    (pCbData)->osTpBudget = (budget);              \
    (pCbData)->TpNodeInList = FALSE;               \
    (pCbData)->Pre = NULL_PTR;                     \
    (pCbData)->Next = NULL_PTR;
/* PRQA S 3412 -- */

/* ========================================= external function declarations ========================================= */
/**
 * @brief              Init timing protection module
 * @param[in]          coreId: Current running coreID.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitTmProt(Os_CoreIdType coreId);

/**
 * @brief              Timing protection start process of task.
 * @param[in]          coreId: Current running coreID.
 * @param[in]          taskId: Task Id.
 * @param[in]          osOptType: The types of operations for time protection.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtTaskStart(Os_CoreIdType coreId, Os_TaskType taskId, Os_TmProtOptType osOptType);

/**
 * @brief              Timing protection end process of task.
 * @param[in]          coreId: Current running coreID.
 * @param[in]          taskId: Task Id.
 * @param[in]          osOptType: The types of operations for time protection.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtTaskEnd(Os_CoreIdType coreId, Os_TaskType taskId, Os_TmProtOptType osOptType);

/**
 * @brief              Time frame of task: frame check process.
 * @param[in]          taskId: Task Id.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_TmProtTaskFrameChk(Os_TaskType taskId);

/**
 * @brief              Timing Protection isr callback.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtMainProc(void);

#if (CFG_STD_RESOURCE_MAX > 0)
/**
 * @brief              Start process for budget of resource locking.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          resId: Resource Id.
 * @param[in]          MonitoredType: The thread type that time protection monitord for.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtResStart(const Os_SCBType *pScb, ResourceType resId,
                                         Os_TpMonitoredType MonitoredType);

/**
 * @brief              End process for budget of resource locking.
 * @param[in]          coreId: Current running coreID.
 * @param[in]          resId: Resource Id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtResEnd(Os_CoreIdType coreId, ResourceType resId);
#endif

/**
 * @brief              Timing protection start process of isr.
 * @param[in]          coreId: Current running coreID.
 * @param[in]          isrId: Isr Id.
 * @param[in]          osOptType: The types of operations for time protection.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtIsrStart(Os_CoreIdType coreId, Os_IsrType isrId, Os_TmProtOptType osOptType);

/**
 * @brief              Timing protection end process of isr.
 * @param[in]          coreId: Current running coreID.
 * @param[in]          isrId: Isr Id.
 * @param[in]          osOptType: The types of operations for time protection.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtIsrEnd(Os_CoreIdType coreId, Os_IsrType isrId, Os_TmProtOptType osOptType);

/**
 * @brief              Time frame of Isr: frame check process.
 * @param[in]          isrId: Isr Id.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_TmProtIsrFrameChk(Os_IsrType isrId);

/**
 * @brief              Time protect suspend current monitored thread
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          MonitoredType: The thread type that time protection monitord for.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TpSupend(const Os_SCBType *pScb, Os_TpMonitoredType MonitoredType);

/**
 * @brief              Time protect resume current monitored thread
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          MonitoredType: The thread type that time protection monitord for.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TpResume(const Os_SCBType *pScb, Os_TpMonitoredType MonitoredType);

#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
