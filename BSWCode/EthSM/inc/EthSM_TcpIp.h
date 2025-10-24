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
 **  @file               : EthSM_TcpIp.h
 **  @author             : xiaojian.liang
 **  @date               : 2024/12/23
 **  @vendor             : isoft
 **  @description        : Callback notifications of EthSM for TcpIp
 **
 ***********************************************************************************************************************/

#ifndef ETHSM_TCPIP_H_
#define ETHSM_TCPIP_H_

/* =================================================== inclusions =================================================== */
#include "EthSM_Cfg.h"

#if ETHSM_TCPIP_ENABLED == STD_ON
#include "TcpIp.h" /* Import TcpIp_StateType */

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       This service is called by the TcpIp to report the actual TcpIp state (e.g. online, offline).
 * @param[in]   CtrlIdx    EthIf controller index to identify the communication network where the TcpIp state is changed
 * @param[in]   TcpIpState Actual TcpIp state of the specific network handle
 * @reentrant   TRUE (only for different Ethernet controllers)
 * @synchronous TRUE
 * @trace       CPD-69980
 */
void EthSM_TcpIpModeIndication(uint8 CtrlIdx, TcpIp_StateType TcpIpState);

#ifdef __cplusplus
}
#endif

#endif /* ETHSM_TCPIP_ENABLED == STD_ON */

#endif /* ETHSM_TCPIP_H_ */
