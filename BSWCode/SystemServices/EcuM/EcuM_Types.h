/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : EcuM_Types.h                                                **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : This file is partly ECU dependent.                          **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11.                     **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-06-28  qinchun.yang    R19_11 EcuM initial version.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 */
#ifndef ECUM_TYPES_H
#define ECUM_TYPES_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Rte_EcuM_Type.h"
#include "EcuM_Generated_Types.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

#define ECUM_TIMER_USE_GPT 0u
#define ECUM_TIMER_USE_OS  1u

/*A service was called prior to initialization*/
#define ECUM_E_UNINIT 0
/*A function was called which was disabled by configuration*/
#define ECUM_E_SERVICE_DISABLED 1
/*A invalid pointer was passed as an argument*/
#define ECUM_E_NULL_POINTER 2
/*A parameter was invalid (unspecific)*/
#define ECUM_E_INVALID_PAR 3
/*A state, passed as an argument to a service, was out of range (specific
 * parameter test)*/
#define ECUM_E_STATE_PAR_OUT_OF_RANGE 4
/*An unknown wakeup source was passed as a parameter to an API*/
#define ECUM_E_UNKNOWN_WAKEUP_SOURCE 5
/*The initialization failed*/
#define ECUM_E_INIT_FAILED 6
/*The RAM check during wakeup failed*/
#define ECUM_E_RAM_CHECK_FAILED 7
/*Postbuild configuration data is inconsistent*/
#define ECUM_E_CONFIGURATION_DATA_INCONSISTENT 8
/*On multiple requests by the same ID for requestRun()*/
#define ECUM_E_MULTIPLE_RUN_REQUESTS 9
/*On releasing without a matching request for releaseRun */
#define ECUM_E_MISMATCHED_RUN_RELEASE 10

#define ECUM_E_CALL_OS_FAILED         11

/*Reinit EcuM module */
#define ECUM_E_REINIT 0x20
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*Encode states and sub-states of the ECU Manager module.
 *States shall be encoded in the high-nibble, sub-states in the low-nibble.
 */

typedef uint8 EcuM_StateType;
#define ECUM_SUBSTATE_MASK      (EcuM_StateType)0x0f
#define ECUM_STATE_STARTUP      (EcuM_StateType)0x10
#define ECUM_STATE_RUN          (EcuM_StateType)0x30
#define ECUM_STATE_APP_RUN      (EcuM_StateType)0x32
#define ECUM_STATE_APP_POST_RUN (EcuM_StateType)0x33
#define ECUM_STATE_SHUTDOWN     (EcuM_StateType)0x40
#define ECUM_STATE_SLEEP        (EcuM_StateType)0x50

/*Result of the Run Request Protocol sent to BswM*/
typedef uint8 EcuM_RunStatusType;
#define ECUM_RUNSTATUS_UNKNOWN   (EcuM_RunStatusType)0
#define ECUM_RUNSTATUS_REQUESTED (EcuM_RunStatusType)1
#define ECUM_RUNSTATUS_RELEASED  (EcuM_RunStatusType)2

/*The bit field provides one bit for each wake up source.
 *In WAKEUP, all bits cleared indicates that no wake up source is known.
 *In STARTUP, all bits cleared indicates that no reason for restart or reset is known.
 *In this case, ECUM_WKSOURCE_RESET shall be assumed.
 *EcuM defines a bit field with 5 pre-defined positions.
 */
typedef uint32 EcuM_WakeupSourceType;
#define ECUM_WKSOURCE_POWER          (EcuM_WakeupSourceType)0x00000001
#define ECUM_WKSOURCE_RESET          (EcuM_WakeupSourceType)0x00000002
#define ECUM_WKSOURCE_INTERNAL_RESET (EcuM_WakeupSourceType)0x00000004
#define ECUM_WKSOURCE_INTERNAL_WDG   (EcuM_WakeupSourceType)0x00000008
#define ECUM_WKSOURCE_EXTERNAL_WDG   (EcuM_WakeupSourceType)0x00000010

/*The type describes the possible states of a wakeup source.*/
typedef uint8 EcuM_WakeupStatusType;
#define ECUM_WKSTATUS_NONE      (EcuM_WakeupStatusType)0
#define ECUM_WKSTATUS_PENDING   (EcuM_WakeupStatusType)1
#define ECUM_WKSTATUS_VALIDATED (EcuM_WakeupStatusType)2
#define ECUM_WKSTATUS_EXPIRED   (EcuM_WakeupStatusType)3
#define ECUM_WKSTATUS_ENABLED   (EcuM_WakeupStatusType)6

/*This type describes the reset mechanisms supported by the ECU State Manager.
 *It can be extended by configuration.*/
typedef uint8 EcuM_ResetType;
#define ECUM_RESET_MCU (EcuM_ResetType)0
#define ECUM_RESET_WDG (EcuM_ResetType)1
#define ECUM_RESET_IO  (EcuM_ResetType)2

#if (ECUM_MODE_HANDING == STD_ON) /* PRQA S 3332 */ /* Rule-20.9 */
#ifndef RTE_TRANSITION_EcuM_Mode
/*The following parts need to be defined in Rte_Type.h*/
typedef uint8 Rte_ModeType_EcuM_Mode;
#define RTE_TRANSITION_EcuM_Mode    5
#define RTE_MODE_EcuM_Mode_STARTUP  0
#define RTE_MODE_EcuM_Mode_RUN      1
#define RTE_MODE_EcuM_Mode_POST_RUN 2
#define RTE_MODE_EcuM_Mode_SHUTDOWN 3
#define RTE_MODE_EcuM_Mode_SLEEP    4
#endif
#endif /*ECUM_MODE_HANDING == STD_ON*/
/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#endif /* ECUM_TYPES_H */
