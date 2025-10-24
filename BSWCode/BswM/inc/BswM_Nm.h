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
 **  @file               : BswM_Nm.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_NM_H
#define BSWM_NM_H

/* =================================================== inclusions =================================================== */
#include "BswM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (BSWM_NM_ENABLED == STD_ON)

/**
 * @brief       Function called by NmIf to indicate a CarWakeup.
 * @param[in]   Network : Identification of the Nm-Channel.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66325
 */
void BswM_Nm_CarWakeUpIndication(NetworkHandleType Network);

/**
 * @brief       Notification of current Nm state after state changes.
 * @param[in]   Network : Identification of the Nm-channel
 * @param[in]   currentState : Current (new) state of the Nm-channel
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66328
 */
void BswM_Nm_StateChangeNotification(NetworkHandleType Network, Nm_StateType currentState);

#endif /*BSWM_NM_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_NM_H */
