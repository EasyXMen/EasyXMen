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
 **  @file               : BswM_ComM.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_COMM_H
#define BSWM_COMM_H

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
#if (BSWM_COMM_ENABLED == STD_ON)

/**
 * @brief       Function called by ComM to signal a shutdown.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-66236
 */
void BswM_ComM_InitiateReset(void);

/**
 * @brief       Function called by ComM to indicate the current communication mode of a ComM channel.
 * @param[in]   Network : The ComM communication channel that the indicated state corresponds to.
 * @param[in]   RequestedMode : The current state of the ComM communication channel.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66238
 */
void BswM_ComM_CurrentMode(NetworkHandleType Network, ComM_ModeType RequestedMode);

/**
 * @brief       Function called by ComM to indicate the current mode of the PNC.
 * @param[in]   PNC : The handle of the PNC for which the current state is reported.
 * @param[in]   CurrentPncMode : The current mode of the PNC.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66239
 */
void BswM_ComM_CurrentPNCMode(PNCHandleType PNC, ComM_PncModeType CurrentPncMode);

#endif /*BSWM_COMM_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_COMM_H */
