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
 **  @file               : EcuM_Types.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

#ifndef ECUM_TYPES_H
#define ECUM_TYPES_H
/* =================================================== inclusions =================================================== */
#include "EcuM_Cfg.h"
#include "Rte_EcuM_Type.h"
#include "EcuM_Generated_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/** timer configuration of the GPT module */
#define ECUM_TIMER_USE_GPT 0u
/** timer configuration of the OS module */
#define ECUM_TIMER_USE_OS 1u

#ifndef ECUM_E_EARLIER_ACTIVE
/*ERROR Code of Std_ReturnType for an earlier alarm is already set*/
#define ECUM_E_EARLIER_ACTIVE 0x03u
#endif /* ECUM_E_EARLIER_ACTIVE */
#ifndef ECUM_E_PAST
/*ERROR Code of Std_ReturnType for the given point in time has already passed*/
#define ECUM_E_PAST 0x04u
#endif /* ECUM_E_PAST */
#ifndef ECUM_E_NOT_ACTIVE
/*ERROR Code of Std_ReturnType for no active alarm found*/
#define ECUM_E_NOT_ACTIVE 0x05u
#endif /* ECUM_E_NOT_ACTIVE */

/** a service was successful executed */
#define ECUM_E_OK 0xFFu
/** A service was called prior to initialization */
#define ECUM_E_UNINIT 0u
/** A function was called which was disabled by configuration */
#define ECUM_E_SERVICE_DISABLED 1u
/** A invalid pointer was passed as an argument */
#define ECUM_E_NULL_POINTER 2u
/** A parameter was invalid (unspecific) */
#define ECUM_E_INVALID_PAR 3u
/** A state, passed as an argument to a service, was out of range (specific parameter test) */
#define ECUM_E_STATE_PAR_OUT_OF_RANGE 4u
/** An unknown wakeup source was passed as a parameter to an API */
#define ECUM_E_UNKNOWN_WAKEUP_SOURCE 5u
/** The initialization failed */
#define ECUM_E_INIT_FAILED 6u
/** The RAM check during wakeup failed */
#define ECUM_E_RAM_CHECK_FAILED 7u
/** Postbuild configuration data is inconsistent */
#define ECUM_E_CONFIGURATION_DATA_INCONSISTENT 8u
/** On multiple requests by the same ID for requestRun() */
#define ECUM_E_MULTIPLE_RUN_REQUESTS 9u
/** On releasing without a matching request for releaseRun */
#define ECUM_E_MISMATCHED_RUN_RELEASE 10u
/** Call to OS_Start failed */
#define ECUM_E_CALL_OS_FAILED 11u
/** After a wake up, no wake up event was set in the given time (see EcuMCheckWakeupTimeout) */
#define ECUM_E_WAKEUP_TIMEOUT 12u

/** Reinit EcuM module */
#define ECUM_E_REINIT 0x20u
/* ================================================ type definitions ================================================ */

#if (ECUM_MODE_HANDING == STD_ON) /* PRQA S 3332 */ /* VL_EcuM_3332 */
#ifndef RTE_TRANSITION_EcuM_Mode
/** EcuM Mode Type definition */
typedef uint8 Rte_ModeType_EcuM_Mode;
/** Transition mode for EcuM */
#define RTE_TRANSITION_EcuM_Mode 5u
/** EcuM startup mode */
#define RTE_MODE_EcuM_Mode_STARTUP 0u
/** EcuM run mode */
#define RTE_MODE_EcuM_Mode_RUN 1u
/** EcuM post run mode */
#define RTE_MODE_EcuM_Mode_POST_RUN 2u
/** EcuM shutdown mode */
#define RTE_MODE_EcuM_Mode_SHUTDOWN 3u
/** EcuM sleep mode */
#define RTE_MODE_EcuM_Mode_SLEEP 4u
#endif
#endif /*ECUM_MODE_HANDING == STD_ON*/
/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif /* ECUM_TYPES_H */
