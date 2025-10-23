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
 **  @file               : BswM_J1939Dcm.h
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : BSW Mode Manager
 **
 ***********************************************************************************************************************/

#ifndef BSWM_J1939DCM_H
#define BSWM_J1939DCM_H

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
#if (BSWM_J1939DCM_ENABLED == STD_ON)

/**
 * @brief       This API tells the BswM the desired communication status of the available networks.
 *              The status will typically be activated via COM I-PDU group switches.
 * @param[in]   NetworkMask : Mask containing one bit for each available network.
 *                            - 1: Network enabled
 *                            - 0: Network disabled
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-66312
 */
void BswM_J1939DcmBroadcastStatus(uint16 NetworkMask);

#endif /*BSWM_J1939DCM_ENABLED == STD_ON*/

#ifdef __cplusplus
}
#endif

#endif /* BSWM_J1939DCM_H */
