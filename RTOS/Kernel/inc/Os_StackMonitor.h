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
 **  @file               : Os_StackMonitor.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for StackMonitor api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_STACKMONITOR_H
#define OS_STACKMONITOR_H

/* =================================================== inclusions =================================================== */
#include "Os_Types.h"

/* ===================================================== macros ===================================================== */
#define OS_STACK_USAGE_LEVEL_LOW         (0u)
#define OS_STACK_USAGE_LEVEL_MEDIUM_LOW  (1u)
#define OS_STACK_USAGE_LEVEL_MEDIUM_HIGH (2u)
#define OS_STACK_USAGE_LEVEL_HIGH        (3u)

#define OS_STACK_USAGE_PERCENT_25  (25u)
#define OS_STACK_USAGE_PERCENT_50  (50u)
#define OS_STACK_USAGE_PERCENT_75  (75u)
#define OS_STACK_USAGE_PERCENT_100 (100u)
/* ========================================= external function declarations ========================================= */
#if (TRUE == CFG_STACK_CHECK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Check whether the stack is overflowed.
 * @param[in]          stack: the stack of task or system.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_StackMonitor(const Os_StackType* stack);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Fill the stack of task or system with the value of 0xCC.
 * @param[in]          stack: the stack of task or system.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_FillStack(const Os_StackType* stack);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#if (TRUE == CFG_STACK_USAGE_CHECK)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Calculate stack usage in words.
 * @param[in]          stack: the stack of task or system.
 * @return             uint32: used words of the stack.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern uint32 Os_CalcStackUsage(const Os_StackType* stack);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Calculate stack usage level (0-3).
 * @param[in]          stack: the stack of task or system.
 * @return             uint8: usage level (0:low, 1:medium-low, 2:medium-high, 3:high).
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern uint8 Os_CalcStackUsageLevel(const Os_StackType* stack);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
