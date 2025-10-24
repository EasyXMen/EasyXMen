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
 **  @file               : BswM_LinSM.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_LINSM_H
#define BSWM_LINSM_H

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
#if (BSWM_LINSM_ENABLED == STD_ON)

/**
 * @brief       Function called by LinSM to indicate the currently active schedule table for a specific LIN channel.
 * @param[in]   Network : The LIN channel that the schedule table switch has occurred on.
 * @param[in]   CurrentSchedule : The currently active schedule table of the LIN channel.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66320
 */
void BswM_LinSM_CurrentSchedule(NetworkHandleType Network, LinIf_SchHandleType CurrentSchedule);

/**
 * @brief       Function called by LinSM to indicate its current state.
 * @param[in]   Network : The LIN channel that the indicated state corresponds to.
 * @param[in]   CurrentState : The current state of the LIN channel.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66319
 */
void BswM_LinSM_CurrentState(NetworkHandleType Network, LinSM_ModeType CurrentState);

#endif /*BSWM_LINSM_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_LINSM_H */
