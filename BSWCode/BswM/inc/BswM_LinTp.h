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
 **  @file               : BswM_LinTp.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_LINTP_H
#define BSWM_LINTP_H

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
#if (BSWM_LINTP_ENABLED == STD_ON)

/**
 * @brief       Function called by LinTP to request a mode for the corresponding LIN channel.
 *              The LinTp_Mode correlates to the LIN schedule table that should be used.
 * @param[in]   Network : The LIN channel that the LinTp mode request relates to.
 * @param[in]   LinTpRequestedMode : The requested LIN TP mode.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66324
 */
void BswM_LinTp_RequestMode(NetworkHandleType Network, LinTp_Mode LinTpRequestedMode);

#endif /*BSWM_LINTP_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif
#endif /* BSWM_LINTP_H */
