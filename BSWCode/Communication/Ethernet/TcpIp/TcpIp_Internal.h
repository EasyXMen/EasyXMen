/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
************************************************************************************************************************
**                                                                                                                    **
**  FILENAME    : TcpIp_Internal.h                                                                                    **
**                                                                                                                    **
**  Created on  : 03/12/18                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : internal type definition for TcpIp                                                                  **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/
#ifndef TCPIP_INTERNAL_H_
#define TCPIP_INTERNAL_H_

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
/* lwip header file */
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"
#if IP_REASSEMBLY
#include "lwip/ip4_frag.h"
#endif /* IP_REASSEMBLY */
#if (STD_ON == TCPIP_TCP_ENABLED)
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#endif /* STD_ON == TCPIP_TCP_ENABLED */
#if (STD_ON == TCPIP_UDP_ENABLED)
#include "lwip/udp.h"
#endif /* STD_ON == TCPIP_UDP_ENABLE */
#if CHECKSUM_CHECK_ICMP
#include "lwip/inet_chksum.h"
#endif /* CHECKSUM_CHECK_ICMP */
#if LWIP_RAW
#include "lwip/raw.h"
#endif /* LWIP_RAW */
#if LWIP_TCP_IPERF
#include "lwip/apps/lwiperf.h"
#endif /* LWIP_TCP_IPERF */

#if (STD_ON == TCPIP_ARP_ENABLED)
#include "lwip/etharp.h"
#endif /* STD_ON == TCPIP_ARP_ENABLED */

#if (STD_ON == TCPIP_AUTOIP_ENABLED)
#include "lwip/autoip.h"
#endif /* STD_ON == TCPIP_AUTOIP_ENABLED */
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
#include "lwip/dhcp.h"
#endif /* STD_ON == TCPIP_DHCP_CLIENT_ENABLED */

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
#include "TcpIp_TlsBase.h"
#endif /* STD_ON == TCPIP_TCP_TLS_ENABLED */
#if (STD_ON == TCIP_USED_MOUDLE_DET)
#include "Det.h"
#endif /* STD_ON == TCIP_USED_MOUDLE_DET */
#include "Eth_GeneralTypes.h"
#include "TcpIp.h"

/***********************************************************************************************************************
 *  VERSION
 ***********************************************************************************************************************/
#define TCPIP_INTERNAL_H_AR_MAJOR_VERSION 4u
#define TCPIP_INTERNAL_H_AR_MINOR_VERSION 5u
#define TCPIP_INTERNAL_H_AR_PATCH_VERSION 0U

BEGIN_C_DECLS

typedef size_t uintx;

/* extend sid */
#define TCPIP_SID_TCPSHUTDOWN        0xb0
#define TCPIP_SID_GETTCPSOCKETSTATUS 0xb1

/*! Default definition of a static (module's internal and private) functions.
 * Used a re-definable constant to have better debugging support at customer side.
 * When needed, override this macro in a user configuration file: "#define TCPIP_LOCAL".
 */
#if !defined(TCPIP_LOCAL)
#define TCPIP_LOCAL static
#endif /* !defined TCPIP_LOCAL */

/*! Default definition of a local-inline functions.
 *  Used a re-definable constant to have better debugging support at customer side.
 *  When needed, override this macro in a user configuration file: "#define TCPIP_LOCAL_INLINE".
 */
#if !defined(TCPIP_LOCAL_INLINE)
#define TCPIP_LOCAL_INLINE static inline
#endif /* !defined TCPIP_LOCAL_INLINE */

#if !defined(TCPIP_UNUSED_ARG)
#define TCPIP_UNUSED_ARG(x) (void)(x)
#endif /* !defined TCPIP_UNUSED_ARG */

/* TCPIP_FASTTX_TCP = STD_ON:
 * Enable call tcp_write continue call tcp_output,this handle shall fast transmit tcp data
 */
#if !defined(TCPIP_FASTTX_TCP)
#define TCPIP_FASTTX_TCP STD_OFF
#endif /* !defined TCPIP_FASTTX_TCP */

/* TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET = 1:
 * request no com,tcpip module shall auto close all socket by control
 */
#if !defined(TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET)
#define TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET 0
#endif /* !defined(TCPIP_SUPPORT_AUTO_CLOSE_ALL_SOCKET */
/* TCPIP_SUPPORT_WRITE_MORE = 1:
 * call tcp_write shall set TCP_WRITE_FLAG_MORE,this shall not set PUSH flag in tcp haader
 */
#if !defined(TCPIP_SUPPORT_WRITE_MORE)
#define TCPIP_SUPPORT_WRITE_MORE 0
#endif /* !defined TCPIP_SUPPORT_WRITE_MORE */

#if !defined(TCPIP_LWIP_HOOKS)
#define TCPIP_LWIP_HOOKS 0
#endif /* !defined TCPIP_LWIP_HOOKS */

#if !defined(TCPIP_SUPPORT_AUTOIP_INITTIMER)
#define TCPIP_SUPPORT_AUTOIP_INITTIMER STD_OFF
#endif /* !defined TCPIP_SUPPORT_AUTOIP_INITTIMER */

/* TCPIP_REPEAT_GET_NETIF_MAC =1 :switch comm mode shall be regain get mac addr write to netif.
 * normal used eth_drv is asynchronous initialization mac addr
 */
#if !defined(TCPIP_REPEAT_GET_NETIF_MAC)
#define TCPIP_REPEAT_GET_NETIF_MAC 1
#endif /* !defined TCPIP_REPEAT_GET_NETIF_MAC */

/*
 * TCPIP_ALLOW_RX_INVALID_LOCAL_IP =1:local ip is invalid allow receiver ip packets
 * application scene: local ip release happend TCP closes the socket four times with a wave,if
 * TCPIP_ALLOW_RX_INVALID_LOCAL_IP=0,ware packet shall be destory in function TcpIp_HookIp4InputAccept
 */
#if !defined(TCPIP_ALLOW_RX_INVALID_LOCAL_IP)
#define TCPIP_ALLOW_RX_INVALID_LOCAL_IP 1
#endif /* !defined TCPIP_ALLOW_RX_INVALID_LOCAL_IP */

#if !defined(TCPIP_RXIND_FILTER_VLAN)
#if (TCP_SOCKET_VLAN == STD_ON)
#define TCPIP_RXIND_FILTER_VLAN 1
#else
#define TCPIP_RXIND_FILTER_VLAN 0
#endif /* TCP_SOCKET_VLAN == STD_ON */
#endif /* !defined TCPIP_RXIND_FILTER_VLAN */

#if !defined(TCPIP_HOOK_VLAN_SET)
#define TCPIP_HOOK_VLAN_SET 1
#endif /* !defined TCPIP_HOOK_VLAN_SET */

/* tcp iperf,need lwip support */
#if !defined(TCPIP_TCP_IPERF_SUPPORT)
#define TCPIP_TCP_IPERF_SUPPORT 0
#endif

/* iperf used default tcp port */
#if !defined(TCPIP_TCP_IPERF_PORT_DEFAULT)
#define TCPIP_TCP_IPERF_PORT_DEFAULT LWIPERF_TCP_PORT_DEFAULT
#endif

#if defined(TCPIP_SPPORT_TC8_TEST)
#if !defined(TCPIP_ARP_SUPPORT_DYNAMIC_TIMEOUT_CHANGE)
#define TCPIP_ARP_SUPPORT_DYNAMIC_TIMEOUT_CHANGE ETHARP_SUPPORT_DYNAMIC_TIMEOUT_CHANGE
#endif
#endif /* defined TCPIP_SPPORT_TC8_TEST */

#if defined(TCPIP_SPPORT_TC8_TEST)
#if !defined(TCPIP_ARP_SUPPORT_STATIC_ENTRIES)
#define TCPIP_ARP_SUPPORT_STATIC_ENTRIES ETHARP_SUPPORT_STATIC_ENTRIES
#endif
#endif /* defined TCPIP_SPPORT_TC8_TEST */

typedef uint8 TcpIp_InitStateType;
#define TCPIP_STATE_UNINIT 0x0u
#define TCPIP_STATE_INIT   0x1u

#if (STD_ON == TCIP_USED_MOUDLE_DET)
static inline void TCPIP_DET(uint8 api, uint8 error)
{
    (void)Det_ReportError((uint16)TCPIP_MODULE_ID, (uint8)TCPIP_INSTANCE, (uint8)(api), (uint8)(error));
}

static inline void TCPIP_DETRUNTIME(uint8 api, uint8 error)
{
    (void)Det_ReportRuntimeError((uint16)TCPIP_MODULE_ID, (uint8)TCPIP_INSTANCE, (uint8)(api), (uint8)(error));
}

#else
#define TCPIP_DET(api, error)
#define TCPIP_DETRUNTIME(api, error)
#endif /* STD_ON == TCIP_USED_MOUDLE_DET */

#if (STD_ON == TCPIP_TCP_ENABLED)
/* socket event handle */
#define TCP_FLAG_PENDING_EVENT              (uint8)(0x04u)
#define TCPIP_TCP_IS_PENDINGEVENT(sockPtr)  (((sockPtr)->socketFlag) & TCP_FLAG_PENDING_EVENT) != 0u
#define TCPIP_TCP_SET_PENDINGEVENT(sockPtr) ((sockPtr)->socketFlag) |= TCP_FLAG_PENDING_EVENT
#define TCPIP_TCP_CLR_PENDINGEVENT(sockPtr) ((sockPtr)->socketFlag) &= (uint8)(~TCP_FLAG_PENDING_EVENT)
#endif /* STD_ON == TCPIP_TCP_ENABLED */

#if defined(TCPIP_FUNC_CHECK_OUTPUT_SUPPORT)
#include "func_trace.h"
#define TCPIP_CHECK
#define TCPIP_CHECK_OUTPUT(...) FuncCheckOutput(__VA_ARGS__)
#else
#define TCPIP_CHECK_OUTPUT(...)
#endif /* defined(TCPIP_FUNC_CHECK_OUTPUT_SUPPORT) */

#define TCPIP_SOCKET_NUM (TCPIP_UDP_SOCKET_MAX + TCPIP_TCP_SOCKET_MAX)

#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
#define TCPIP_LOCAL_TO_LWIP_IPADDR(lip, uip) (lip) = ((uip)[0])
#define TCPIP_LWIP_TO_LOCAL_IPADDR(uip, lip) (uip)[0] = (lip)
#else
#define TCPIP_LOCAL_TO_LWIP_IPADDR(lip, uip) \
    do                                       \
    {                                        \
        (lip)[0] = (uip)[0];                 \
        (lip)[1] = (uip)[1];                 \
        (lip)[2] = (uip)[2];                 \
        (lip)[3] = (uip)[3];                 \
    } while (0);

#define TCPIP_LWIP_TO_LOCAL_IPADDR(uip, lip) \
    do                                       \
    {                                        \
        (uip)[0] = (lip)[0];                 \
        (uip)[1] = (lip)[1];                 \
        (uip)[2] = (lip)[2];                 \
        (uip)[3] = (lip)[3];                 \
    } while (0)

#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS */

#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)
#if !defined(TCPIP_SUUPORT_CTRL_MAINHANDLE)
#define TCPIP_SUUPORT_CTRL_MAINHANDLE
#endif /* !defined TCPIP_SUUPORT_CTRL_MAINHANDLE */
#endif /* STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER */

/* timer support macro define */
#if (                                                                                                      \
    (STD_ON == TCPIP_AUTOIP_ENABLED) || (STD_ON == TCPIP_IPV6_ENABLED) || (STD_ON == TCPIP_AUTOIP_ENABLED) \
    || (LWIP_IPV6_MLD))
#define TCPIP_SUPPORT_TMR_100ms STD_ON
#else
#define TCPIP_SUPPORT_TMR_100ms STD_OFF

#endif /* STD_ON == TCPIP_AUTOIP_ENABLED || (STD_ON == TCPIP_IPV6_ENABLED) || (STD_ON == TCPIP_AUTOIP_ENABLED) \
     || LWIP_IPV6_MLD */

#if (STD_ON == TCPIP_TCP_ENABLED)
#define TCPIP_SUPPORT_TMR_250ms STD_ON
#else
#define TCPIP_SUPPORT_TMR_250ms STD_OFF
#endif /* STD_ON == TCPIP_TCP_ENABLED */

#if (defined TCPIP_TCP_SYNISN_RECALC) && (STD_ON == TCPIP_TCP_SYNISN_RECALC) && (TCPIP_MAINFUNCTION_PERIOD < 4u)
#define TCPIP_SUPPORT_TMR_4ms STD_ON
#else
#define TCPIP_SUPPORT_TMR_4ms STD_OFF
#endif /* defined TCPIP_TCP_SYNISN_RECALC && (STD_ON == TCPIP_TCP_SYNISN_RECALC) && (TCPIP_MAINFUNCTION_PERIOD < 4u) \
        */

#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
#define TCPIP_SUPPORT_TMR_500ms   STD_ON
#define TCPIP_SUPPORT_TMR_60000ms STD_ON
#else
#define TCPIP_SUPPORT_TMR_500ms   STD_OFF
#define TCPIP_SUPPORT_TMR_60000ms STD_OFF
#endif /* STD_ON == TCPIP_DHCP_CLIENT_ENABLED */

#if ((STD_ON == TCPIP_ARP_ENABLED) || (STD_ON == TCPIP_IPV6_ENABLED) || (IP_REASSEMBLY) || (LWIP_IPV6_REASS))
#define TCPIP_SUPPORT_TMR_1000ms STD_ON
#else
#define TCPIP_SUPPORT_TMR_1000ms STD_OFF
#endif /* STD_ON == TCPIP_ARP_ENABLED || (STD_ON == TCPIP_IPV6_ENABLED) || (IP_REASSEMBLY) || LWIP_IPV6_REASS */

#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
/* arry item number is control link local addr */
#define TCPIP_CTRL_LOCAL_NUMBER ((TCPIP_MAX_IPADT_PER_CTRL + 31u) / 32u)
#endif /* STD_ON == TCPIP_MUILT_IPADT_PER_CTRL */

typedef struct
{
    /* local struct member */
    VAR(TcpIp_StateType, TCPIP_VAR) ctrlStatus;
    VAR(uint8, TCPIP_VAR) stateFlag;
    /* control mask flag */
    VAR(uint8, TCPIP_VAR) maskFlg;
    VAR(TcpIp_SocketIdType, TCPIP_VAR) socketNum;
#if (STD_ON == TCPIP_MUILT_IPADT_PER_CTRL)
    /* multiple ip mark local addr used */
    VAR(uint32, TCPIP_VAR) localUsedMask[TCPIP_CTRL_LOCAL_NUMBER];
#endif /* STD_ON == TCPIP_MUILT_IPADT_PER_CTRL */
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
    /* dhcp struct for the ethernet netif */
    struct dhcp netif_dhcp;
#endif /* STD_ON == TCPIP_DHCP_CLIENT_ENABLED */
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
    /* autoip struct for the ethernet netif */
    struct autoip netif_autoip;
#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)
    VAR(uint32, TCPIP_VAR) autoipTimer;
#endif /* STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER */
#endif /* STD_ON == TCPIP_AUTOIP_ENABLED */
    struct netif netifVar;
} TcpIp_ControllerStatus;

typedef struct
{
    /* socket up layer link timer owner cfg */
    P2CONST(TcpIp_SocketOwnerLCfgType, TYPEDEF, TCPIP_CONST) ownerCfgPtr;
    /* struct point loader lwip pcb */
    P2VAR(void, TYPEDEF, TCPIP_CONST) pcbPtr;
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
    VAR(TcpIp_SocketIdType, TCPIP_VAR) listenSocketId;
    P2VAR(TcpIp_TlsSocketDataType, TYPEDEF, TCPIP_CONST) tlsPtr;
#endif /* STD_ON == TCPIP_TCP_TLS_ENABLED */
    /* link next socket id */
    VAR(TcpIp_SocketIdType, TCPIP_VAR) nextSocketId;
    /* socket Protocol type,udp or tcp */
    VAR(TcpIp_ProtocolType, TCPIP_VAR) protocol;
    /* belong control index */
    VAR(uint8, TCPIP_VAR) ctrlIndex;
#if (STD_ON == TCPIP_TCP_ENABLED)
    struct pbuf* txBuf;
    /* tx transmit len, shall be next mainfunction handle txRemainLen to copy data */
    VAR(uint32, TCPIP_VAR) UpCopyTxRemainLen;
    VAR(uint8, TCPIP_VAR) socketFlag;
#endif /* STD_ON == TCPIP_TCP_ENABLED */

} TcpIp_SocketHandleType;

#define LOCALADR_FLAG_REQUEST_IP (uint32)0x1u
#define LOCALADR_FLAG_IP_VAID    (uint32)0x2u
typedef struct
{
    VAR(ip_addr_t, TCPIP_VAR) netmask;
    VAR(ip_addr_t, TCPIP_VAR) ipAdrSrc;
    VAR(ip_addr_t, TCPIP_VAR) defaultrouter;
#if LWIP_NETIF_HOSTNAME
    VAR(uint8, TCPIP_VAR) hostNameLen;
#endif /* LWIP_NETIF_HOSTNAME */
    VAR(uint32, TCPIP_VAR) flag;

} TcpIp_LocalAdrHandleType;
#if (STD_ON == TCPIP_ICMP_ENABLED)
typedef struct
{
    VAR(uint8, AUTOMATIC) ttl;
    VAR(uint8, AUTOMATIC) type;
    VAR(uint8, AUTOMATIC) code;
    VAR(uint8, AUTOMATIC) HopLimit;
    VAR(uint16, AUTOMATIC) dataLength;
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr;
} TcpIp_IcmpTransDataType;
#endif /* STD_ON == TCPIP_ICMP_ENABLED */

/* socket parameters handle type */
typedef Std_ReturnType (*praHanldFuncPtrType)(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) ParameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) ParameterValue);
typedef struct
{
    VAR(uint32, AUTOMATIC) paramId;
    VAR(praHanldFuncPtrType, AUTOMATIC) praHanldFuncPtr;
} TcpIp_SocketParaHandleType;

typedef struct
{
#if (STD_ON == TCPIP_SUPPORT_TMR_4ms)
    VAR(uint32, TCPIP_VAR) tmr_4ms;
#endif /* STD_ON == TCPIP_SUPPORT_TMR_4ms */

#if (STD_ON == TCPIP_SUPPORT_TMR_100ms)
    VAR(uint32, TCPIP_VAR) tmr_100ms;
#endif /*STD_ON == TCPIP_SUPPORT_TMR_100ms */

#if (STD_ON == TCPIP_SUPPORT_TMR_250ms)
    VAR(uint32, TCPIP_VAR) tmr_250ms;
#endif /* STD_ON == TCPIP_SUPPORT_TMR_250ms */

#if (STD_ON == TCPIP_SUPPORT_TMR_500ms)
    VAR(uint32, TCPIP_VAR) tmr_500ms;
#endif /* STD_ON == TCPIP_SUPPORT_TMR_500ms */

#if (STD_ON == TCPIP_SUPPORT_TMR_1000ms)
    VAR(uint32, TCPIP_VAR) tmr_1000ms;
#endif /* STD_ON == TCPIP_SUPPORT_TMR_1000ms */

#if (STD_ON == TCPIP_SUPPORT_TMR_60000ms)
    VAR(uint32, TCPIP_VAR) tmr_60000ms;
#endif /* STD_ON == TCPIP_SUPPORT_TMR_60000ms */
} TcpIP_PeriodTmrType;

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 ***********************************************************************************************************************/

#if (TCPIP_CONTROLLER_NUM > 0)
FUNC(void, TCPIP_CODE)
TcpIp_ControllerInit(VAR(uint32, AUTOMATIC) ctrlCnt, P2CONST(TcpIp_CtrlType, AUTOMATIC, TCPIP_APPL_DATA) ctrlCfgPtr);

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS TcpIp_GetControlState
#endif
FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_GetControlState(VAR(uintx, AUTOMATIC) ctrlIndex);
#endif /* TCPIP_CONTROLLER_NUM > 0 */

#if (TCPIP_SOCKET_NUM > 0u)
FUNC(void, TCPIP_CODE) TcpIp_SocketInit(VAR(uint8, AUTOMATIC) socketNum);
#endif /* TCPIP_SOCKET_NUM > 0u */

#if (TCPIP_LOCAL_ADR_NUM > 0u)
FUNC(void, TCPIP_CODE)
TcpIp_LocalAdrInit(
    VAR(uint32, AUTOMATIC) localAdrNum,
    P2CONST(TcpIp_LocalAddrType, AUTOMATIC, TCPIP_APPL_DATA) localAdrCfgPtr);

#endif /* TCPIP_LOCAL_ADR_NUM > 0u */

#if (TCPIP_TCP_SOCKET_MAX > 0u)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_CloseSocketHandle(VAR(TcpIp_SocketIdType, AUTOMATIC) socketId, VAR(boolean, AUTOMATIC) isAbort);
#endif /* TCPIP_TCP_SOCKET_MAX > 0u */

#if (STD_ON == TCPIP_TCP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_MallocTcpSocketId(
    VAR(TcpIp_DomainType, AUTOMATIC) domain,
    P2CONST(TcpIp_SocketOwnerLCfgType, AUTOMATIC, TCPIP_APPL_DATA) socketOwnerCfgPtr,
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr);
#endif /* STD_ON == TCPIP_TCP_ENABLED */

#if (STD_ON == TCPIP_UDP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_MallocUdpSocketId(
    VAR(TcpIp_DomainType, AUTOMATIC) domain,
    P2CONST(TcpIp_SocketOwnerLCfgType, AUTOMATIC, TCPIP_APPL_DATA) socketOwnerCfgPtr,
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr);
#endif /* STD_ON == TCPIP_UDP_ENABLED */

#if (TCPIP_CONTROLLER_NUM > 0)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerRequestComMode(VAR(uint8, AUTOMATIC) ctrlIdx, VAR(TcpIp_StateType, AUTOMATIC) reqState);
#endif /* TCPIP_CONTROLLER_NUM > 0 */

#if (TCPIP_LOCAL_ADR_NUM > 0u)

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_SetLocalAdrIpVar(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrIndx,
    P2CONST(TcpIp_LocalAddrType, AUTOMATIC, TCPIP_APPL_DATA) localAdrCfgPtr,
    P2CONST(TcpIp_StaticIpAddressConfigType, AUTOMATIC, TCPIP_APPL_DATA) ipAdrPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_ClearLocalAdrIpVar(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrIndx,
    VAR(uint8, AUTOMATIC) ctrlIdex,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) assignmentType);
#endif /* TCPIP_LOCAL_ADR_NUM > 0u */

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerChangeParameter(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) parameterValue);

#if (TCPIP_LOCAL_ADR_NUM > 0u)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerGetIpAddr(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrIndx,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) netmaskPtr,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) defaultRouterPtr);
#endif /* TCPIP_LOCAL_ADR_NUM > 0u */

#if ((TCPIP_LOCAL_ADR_NUM > 0u) && (TCPIP_SOCKET_NUM > 0u))
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerBind(
    VAR(uint8, AUTOMATIC) ctrlIdx,
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrIndx,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) portPtr);
#endif /* TCPIP_LOCAL_ADR_NUM > 0u && TCPIP_SOCKET_NUM > 0u */

#if (STD_ON == TCPIP_UDP_ENABLED)

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerUdpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    VAR(uint16, AUTOMATIC) totalLength);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerCopyUdpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    VAR(uint16, AUTOMATIC) totalLength);
#endif /* STD_ON == TCPIP_UDP_ENABLED */

#if (STD_ON == TCPIP_TCP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerTcpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    VAR(uint16, AUTOMATIC) totalLength);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerCopyTcpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(uint16, AUTOMATIC) totalLength,
    VAR(boolean, AUTOMATIC) forceRetrieve);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerTcpConnect(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerTcpListen(VAR(TcpIp_SocketIdType, AUTOMATIC) socketId, VAR(uint8, AUTOMATIC) maxCh);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerTcpReceived(VAR(TcpIp_SocketIdType, AUTOMATIC) socketId, VAR(uint16, AUTOMATIC) revcLen);

#endif /* STD_ON == TCPIP_TCP_ENABLED */

FUNC(void, TCPIP_CODE)
TcpIp_InnerRxIndication(
    VAR(uint8, AUTOMATIC) ctrlIdx,
    VAR(Eth_FrameType, AUTOMATIC) frameType,
    VAR(uint8, AUTOMATIC) flag,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
    VAR(uint16, AUTOMATIC) lenByte);

FUNC(void, TCPIP_CODE)
TcpIp_NetMaskExtToInner(
    VAR(uint8, AUTOMATIC) inNetMaskNum,
    VAR(TcpIp_DomainType, AUTOMATIC) domainType,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_VAR) outNteMaskArry);

FUNC(void, TCPIP_CODE)
TcpIp_NetMaskInnerToExt(
    VAR(TcpIp_DomainType, AUTOMATIC) domainType,
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_VAR) inNteMaskArry,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) outNetMaskNumPtr);

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
FUNC(void, TCPIP_CODE)
TcpIp_EventNotifyUpLayer(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,
    VAR(TcpIp_EventType, AUTOMATIC) event);
#endif /* STD_ON == TCPIP_TCP_TLS_ENABLED */

#if (STD_ON == TCPIP_TCP_ENABLED)

FUNC(void, TCPIP_CODE)
TcpIp_TcpTxConfNotifyUpLayer(
    P2CONST(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,
    VAR(uint16, AUTOMATIC) len);

FUNC(void, TCPIP_CODE)
TcpIp_TcpConnectNofiyUplayer(P2CONST(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TcpAcceptNofiyUplayer(
    VAR(TcpIp_SocketIdType, AUTOMATIC) listenSocketId,
    P2CONST(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr);

FUNC(err_t, TCPIP_CODE)
TcpIp_TcpRxIndNotifyUpLayer(const TcpIp_SocketHandleType* socketMngPtr, struct pbuf* buf);
#endif /* STD_ON == TCPIP_TCP_ENABLED */

#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_StartDhcp(VAR(uintx, AUTOMATIC) ctrlIndex);
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_StopDhcp(VAR(uintx, AUTOMATIC) ctrlIndex);
#endif /* STD_ON == TCPIP_DHCP_CLIENT_ENABLED */

#if (STD_ON == TCPIP_AUTOIP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_StartAutoIp(VAR(uintx, AUTOMATIC) ctrlIndex);
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_StopAutoIp(VAR(uintx, AUTOMATIC) ctrlIndex);
#endif /* STD_ON == TCPIP_AUTOIP_ENABLED */

#if (TCPIP_CONTROLLER_NUM > 0u)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
FUNC(TcpIp_ReturnType, TCPIP_CODE)
TcpIp_GetRemotePhysAddrByArp(
    VAR(uint8, AUTOMATIC) ctrlIdx,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) ipAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
    VAR(boolean, AUTOMATIC) initRes);
#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS || TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */

#if (STD_ON == TCPIP_ICMP_ENABLED)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_InnerIcmpTransmit(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) ipAdr,
    VAR(uint8, AUTOMATIC) ctrlIdx,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    P2CONST(TcpIp_IcmpTransDataType, AUTOMATIC, TCPIP_APPL_VAR) icmpParaPtr);
#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS || TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */
#endif /* STD_ON == TCPIP_ICMP_ENABLED */
#endif /* TCPIP_CONTROLLER_NUM > 0u */

FUNC(void, TCPIP_CODE) TcpIp_PeriodTimerInit(void);
#if (TCPIP_SOCKET_NUM > 0u)
FUNC(void, TCPIP_CODE) TcpIp_SocketPollMainHandle(void);
#endif /* TCPIP_TCP_SOCKET_MAX > 0u */
FUNC(void, TCPIP_CODE) TcpIp_PeriodTimerMainHandle(void);

#if (STD_ON == TCPIP_ARP_ENABLED)
#if ETHARP_SUPPORT_STATIC_ENTRIES
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_ArpAddStaticEntry(
    VAR(uint32, AUTOMATIC) ctrlIdx,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) ipaddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_ArpRemoveStaticEntry(
    VAR(uint32, AUTOMATIC) ctrlIdx,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) ipaddrPtr);
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */
#endif /* STD_ON == TCPIP_ARP_ENABLED */

#if (STD_ON == TCPIP_TCP_TLS_ENABLED)

FUNC(void, TCPIP_CODE) TcpIp_TlsInit(P2CONST(TcpIp_TlsConfigType, AUTOMATIC, TCPIP_APPL_DATA) tlsCfgPtr);

FUNC(void, TCPIP_CODE)
TcpIp_TlsSocketDeInit(P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr);

FUNC(err_t, TCPIP_CODE)
TcpIp_TlsWrite(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_VAR) dataptr,
    VAR(uint16, AUTOMATIC) len);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsRecved(P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr, VAR(uint16, AUTOMATIC) len);

FUNC(err_t, TCPIP_CODE)
TcpIp_TlsConnet(
    P2VAR(struct tcp_pcb, AUTOMATIC, TCPIP_APPL_VAR) tpcb,
    P2CONST(ip_addr_t, AUTOMATIC, TCPIP_APPL_VAR) ipaddr,
    VAR(uint16, AUTOMATIC) port);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TlsBaseDynamicConnect(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,
    VAR(uint8, AUTOMATIC) tlsConnetId);

#if (TCPIP_TCP_TLS_CONNECTION_NUM > 0u)
FUNC(void, TCPIP_CODE)
TcpIp_TlsBaseStaticConnect(
    P2VAR(TcpIp_SocketHandleType, AUTOMATIC, TCPIP_APPL_VAR) socketMngPtr,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    VAR(TcpIp_TlsConnectionInitStateType, AUTOMATIC) connType);

#endif /* TCPIP_TCP_TLS_CONNECTION_NUM > 0u */
#endif /* STD_ON == TCPIP_TCP_TLS_ENABLED */

#if defined(TCPIP_SUUPORT_CTRL_MAINHANDLE)
FUNC(void, TCPIP_CODE) TcpIp_ControlPollMainHandle(void);
#endif /* defined(TCPIP_SUUPORT_CTRL_MAINHANDLE) */

#if TCPIP_TCP_IPERF_SUPPORT
void* TcpIp_IperfStartTcp(const TcpIp_SockAddrType* socketAddrPtr, uint8 test_mode, boolean is_server);
void TcpIp_IperfStopSession(void* session_ptr);
#endif /* TCPIP_TCP_IPERF_SUPPORT */

END_C_DECLS

#endif /* TCPIP_INTERNAL_H_ */
