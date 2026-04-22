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
 ***********************************************************************************************************************
 **
 **  @file               : CanSM_SchM.h
 **  @author             : xiaojian.liang
 **  @date               : 2025/04/25
 **  @vendor             : isoft
 **  @description        : CanSM ECU online active/passive mode implementation header for SchM.
 **
 **********************************************************************************************************************/

#ifndef CANSM_SCHM_H_
#define CANSM_SCHM_H_

/* =================================================== inclusions =================================================== */
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
 * @brief       This function can be used to set all CanSM channels of the network to a receive only mode.
 * @param[in]   controllerIds            controller ids of the network
 * @param[in]   networkControllerNumber  controller number of the network
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-54839
 */
void CanSM_SetNetworkPassive(const uint8* controllerIds, uint8 networkControllerNumber);

#ifdef __cplusplus
}
#endif
#endif
