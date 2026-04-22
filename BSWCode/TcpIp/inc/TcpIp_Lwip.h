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
 **  @file               : TcpIp_Lwip.h
 **  @author             : darren.zhang
 **  @date               : 2018/12/03
 **  @vendor             : isoft
 **  @description        : TcpIp header file for LwIp declarations
 **
 ***********************************************************************************************************************/
/* PRQA S 1536 EOF */ /* VL_TcpIp_1536 */
/* PRQA S 3472 EOF */ /* VL_TcpIp_3472 */
/* PRQA S 0388 EOF */ /* VL_TcpIp_0388 */

#ifndef TCPIP_LWIP_H_
#define TCPIP_LWIP_H_

/* =================================================== inclusions =================================================== */
#include "Platform_Types.h"
#include "Os_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_PBcfg.h"
#include "lwip/opt.h"
#include "lwip/ip.h"
#if LWIP_RAW
#include "lwip/raw.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define TCPIP_LWIP_RUNTIME(partitionIndex) TcpIp_LwipRuntime[(partitionIndex)]

/* ================================================ type definitions ================================================ */
typedef struct TcpIp_LwipRuntimeTag
{
    u8_t* const RamHeap;
} TcpIp_LwipRuntimeType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
extern const TcpIp_LwipRuntimeType TcpIp_LwipRuntime[TCPIP_PARTITION_NUMBER];

extern uint8 TcpIp_MulticoreIndex;

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
