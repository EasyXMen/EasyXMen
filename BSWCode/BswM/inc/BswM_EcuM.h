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
 **  @file               : BswM_EcuM.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_ECUM_H
#define BSWM_ECUM_H

/* =================================================== inclusions =================================================== */
#include "BswM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (BSWM_ECUM_ENABLED == STD_ON)

/**
 * @brief       Function called by EcuM to indicate the current state of a wakeup source.
 * @param[in]   source : Wakeup source(s) that changed state.
 * @param[in]   state : The new state of the wakeup source(s).
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66288
 */
void BswM_EcuM_CurrentWakeup(EcuM_WakeupSourceType source, EcuM_WakeupStatusType state);

/**
 * @brief       Function called by EcuM to notify about the current status of the Run Request Protocol.
 * @param[in]   State : The requested state by EcuMFlex.
 * @param[in]   CurrentStatus : The result of the Run Request Protocol.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66290
 */
void BswM_EcuM_RequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentState);

/**
 * @brief       Function called by EcuM to indicate the current ECU Operation Mode.
 * @param[in]   CurrentState : The requested ECU Operation Mode.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66292
 */
void BswM_EcuM_CurrentState(EcuM_StateType CurrentState);

#endif /*BSWM_ECUM_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_ECUM_H */
