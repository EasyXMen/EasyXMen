/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
 **  @file               : BswM_Types.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Generic data types defined by BswM
 **
 ***********************************************************************************************************************/

#ifndef BSWM_TYPES_H
#define BSWM_TYPES_H

/* =================================================== inclusions =================================================== */
#include "BswM_Cfg.h"
#include "ComStack_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/*Event request port status*/
#define BSWM_EVENT_IS_SET     1u
#define BSWM_EVENT_IS_CLEARED 0u
/* ================================================ type definitions ================================================ */

/**
 * @brief initialization status of BSWM module
 *
 */
typedef enum
{
    BSWM_UNINITED = 0, /**> BSWM module is not initialized. @range 0  */
    BSWM_INITED   = 1  /**> BSWM module is initialized. @range 1 */
} BswM_InitStatusType;

/**
 * @brief BswM request port process type
 *
 */
typedef enum
{
    BSWM_DEFERRED  = 0, /**> BswM request port process type is deferred. @range 0 */
    BSWM_IMMEDIATE = 1  /**> BswM request port process type is immediate. @range 1 */
} BswM_RqstPortProcessType;

/**
 * @brief  BswM rule state type
 *
 */
typedef enum
{
    BSWM_FALSE     = 0, /**> BswM rule state is false. @range 0 */
    BSWM_TRUE      = 1, /**> BswM rule state is true. @range 1 */
    BSWM_UNDEFINED = 2  /**> BswM rule state is undefined. @range 2 */
} BswM_RuleStateType;

/**
 * @brief  BswM action list execution type
 *
 */
typedef enum
{
    BSWM_CONDITION = 0, /**> BswM action list execution type is condition. @range 0 */
    BSWM_TRIGGER   = 1  /**> BswM action list execution type is trigger. @range 1 */
} BswM_ActionListExecutionType;

/**
 * @brief  BswM timer enumeration type
 *
 */
typedef enum
{
    BSWM_TIMER_START   = 0, /**> BswM timer enumeration type is start. @range 0 */
    BSWM_TIMER_STOP    = 1, /**> BswM timer enumeration type is stop. @range 1 */
    BSWM_TIMER_EXPIRED = 2  /**> BswM timer enumeration type is expired. @range 2 */
} BswM_TimerEnumType;
/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif /* BSWM_TYPES_H */
