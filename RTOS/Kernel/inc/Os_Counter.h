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
 **  @file               : Os_Counter.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for counter api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_COUNTER_H
#define OS_COUNTER_H

/* =================================================== inclusions =================================================== */
#include "Os_Counter_CfgData.h"

/* ===================================================== macros ===================================================== */
/**
 * @brief SCB and CoreId are obtained by counterId.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_COUNTER_GET_COREID(counterId) (Os_CounterCfg[counterId].HostCore)
/* PRQA S 3472 -- */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_COUNTER_GET_SCB(counterId) (&Os_SCB_FA_Inf[0])
#else
#define OS_COUNTER_GET_SCB(counterId) (Os_SCB_FA_Inf[OS_COUNTER_GET_COREID(counterId)])
#endif

/* ========================================== external function definitions ========================================= */
/**
 * @brief         Init the counter control block.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_InitCounter(void);

/**
 * @brief              Calculate the absolute ticks that baseTick add the offset
 * @param[in]          baseTick: the begin point.
 * @param[in]          offset: the offset from begin point.
 * @param[in]          counterId: counter id.
 * @return             Os_TickType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern Os_TickType Os_CalcAbsTicks(
    Os_TickType baseTick,
    Os_TickType offset,
    Os_CounterType counterId);

/**
 * @brief              Get the distance from baseTick to destTick
 * @param[in]          baseTick: the begin point.
 * @param[in]          destTick: the end point.
 * @param[in]          counterId: counter id.
 * @return             Os_TickType
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern Os_TickType Os_GetDistance(
    Os_TickType baseTick,
    Os_TickType destTick,
    Os_CounterType counterId);

/**
 * @brief              Increment Hardware counter
 * @param[in]          counterId: counter id.
 * @synchronous        TRUE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_IncrementHardCounter(CounterType counterId);

#endif

/*=======[E N D   O F   F I L E]============================================*/
