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
 ***********************************************************************************************************************
 **
 **  @file               : ComM_EcuM.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : Callback of ComM Implementation header for EcuM
 **
 **********************************************************************************************************************/

#ifndef COMM_ECUM_H_
#define COMM_ECUM_H_

/* =================================================== inclusions =================================================== */
#include "ComM_Cfg.h"
#include "ComStack_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief Notification of a wake up on the corresponding channel.
 * @param[in]    Channel  Channel
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59642
 */
void ComM_EcuM_WakeUpIndication(NetworkHandleType Channel);

#if COMM_PNC_SUPPORT == STD_ON
/**
 * @brief Notification of a wake up on the corresponding partial network cluster.
 * @param[in]    Pnc  Identifier of the partial network cluster
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-59643
 */
void ComM_EcuM_PNCWakeUpIndication(PNCHandleType Pnc);
#endif
#ifdef __cplusplus
}
#endif

#endif
