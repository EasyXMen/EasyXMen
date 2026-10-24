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

#ifndef LWIP_HOOKS_H_
#define LWIP_HOOKS_H_

#include "TcpIp_Cfg.h"
#include "lwip/prot/ethernet.h"

#if TCPIP_LWIP_HOOKS

#if defined(LWIP_HOOK_IPERF_GET_TIME_100US)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
extern uint32 TcpIp_HookGetTimeNow100us(void);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#if (defined TCPIP_TCP_SYNISN_RECALC) && (STD_ON == TCPIP_TCP_SYNISN_RECALC)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
extern uint32
    TcpIp_HookTcpIsn(const ip_addr_t* local_ip, u16_t local_port, const ip_addr_t* remote_ip, u16_t remote_port);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
extern struct netif* TcpIp_HookIp4InputAccept(struct netif* netifPtr, const ip_addr_t* destIpPtr);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
extern uint8 TcpIp_HookEthArpInputForUs(const struct netif* netifPtr, const ip_addr_t* destIpPtr, ip_addr_t* srcIpPtr);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
extern const ip4_addr_t* TcpIp_HookEthArpGetGw(struct netif* netifPtr, const ip_addr_t* destIpPtr);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
extern void TcpIp_HookGratuitousArp(struct netif* netifPtr);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#if (STD_ON == TCPIP_UDP_ENABLED)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
extern uint8 TcpIp_UdpTransmitLocalIpCompare(const ip_addr_t* localIpPtr, const struct netif* netifPtr);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif
#endif

#endif

#endif
