/* PRQA S 3108++ */
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
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : BswM_EcuM.h                                                 **
**                                                                            **
**  Created on  : 2020-03-24                                                  **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef BSWM_ECUM_H
#define BSWM_ECUM_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "BswM_Internal.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (BSWM_ECUM_ENABLED == STD_ON)

/**
 * Function called by EcuM to indicate the current state of a wakeup source.
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): source, wakeup source(s) that changed state
 *                 state, the new state of the wakeup source(s)
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_ECUM_CODE)
BswM_EcuM_CurrentWakeup(EcuM_WakeupSourceType source, EcuM_WakeupStatusType state);

/******Compatible with lower versions******/

/**
 * Function called by EcuM to notify about current Status of the Run Request
 * Protocol.
 * Service ID: 0x23
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): State, the requested state by EcuMFlex
 *                 CurrentStatus, result of the Run Request Protocol
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_ECUM_CODE)
BswM_EcuM_RequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentStatus);

/**
 * Function called by EcuM to indicate the current ECU Operation Mode.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): CurrentState, the requested ECU Operation Mode
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, BSWM_ECUM_CODE)
BswM_EcuM_CurrentState(EcuM_StateType CurrentState);

#endif /*BSWM_ECUM_ENABLED == STD_ON*/
#endif /* BSWM_ECUM_H */
