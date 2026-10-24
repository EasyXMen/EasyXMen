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

/* ========================================= external function declarations ========================================= */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Init timing protection module
 * @param[in]          coreId: Current running coreID.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_InitTmProt(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Time frame of task: frame check process.
 * @param[in]          taskId: Task Id.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_TmProtTaskFrameChk(Os_TaskType taskId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Timing Protection isr callback.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtMainProc(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#if (CFG_STD_RESOURCE_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Start process for budget of resource locking.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          resId: Resource Id.
 * @param[in]          MonitoredType: The thread type that time protection monitord for.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtResStart(const Os_SCBType* pScb, ResourceType resId, Os_TpMonitoredType MonitoredType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              End process for budget of resource locking.
 * @param[in]          coreId: Current running coreID.
 * @param[in]          resId: Resource Id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TmProtResEnd(Os_CoreIdType coreId, ResourceType resId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Time frame of Isr: frame check process.
 * @param[in]          isrId: Isr Id.
 * @return             StatusType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern StatusType Os_TmProtIsrFrameChk(Os_IsrType isrId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Time protect reset current monitored Task then Suspend it
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TpResetAndSuspend(const Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Time protect reset current monitored Task then Continue to run it
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TpResetAndRun(const Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Time protect suspend current monitored thread
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          MonitoredType: The thread type that time protection monitord for.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TpSupend(const Os_SCBType* pScb, Os_TpMonitoredType MonitoredType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Time protect resume current monitored thread
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          MonitoredType: The thread type that time protection monitord for.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_TpResume(const Os_SCBType* pScb, Os_TpMonitoredType MonitoredType);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
