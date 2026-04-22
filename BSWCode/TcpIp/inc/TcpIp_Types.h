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
 **  @file               : TcpIp_Types.h
 **  @author             : darren.zhang
 **  @date               : 2018/12/03
 **  @vendor             : isoft
 **  @description        : TcpIp header file for external type definitions
 **
 ***********************************************************************************************************************/

#ifndef TCPIP_TYPES_H_
#define TCPIP_TYPES_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_PBcfg.h"
#include "lwip/netif.h"
#if LWIP_RAW
#include "lwip/raw.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/**
 * @brief IP address wildcard.
 */
#define TCPIP_IPADDR_ANY 0x00000000u

/**
 * @brief IP6 address wildcard.
 */
#define TCPIP_IP6ADDR_ANY

/**
 * @brief Port wildcard.
 */
#define TCPIP_PORT_ANY ((uint16)0x0000u)

/**
 * @brief LocalAddrId wildcard.
 */
#define TCPIP_LOCALADDRID_ANY ((TcpIp_LocalAddrIdType)0xFFu)

/**
 * @brief IPv6 address buffer size.Store with uint32 array.
 */
#define TCPIP_IPV6ADDR_BUFFER_SIZE 4u

/**
 * @brief Physical address buffer size.Store with uint8 array.
 */
#define TCPIP_PHYSICALADDR_BUFFER_SIZE 6u

/**
 * @name TcpIpIPAddressManipulatingMacros
 * @{
 * @brief A set of utility functions for manipulating IP address.
 */
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
/* PRQA S 3472 ++ */                                                 /* VL_TcpIp_3472 */
#define TCPIP_SET_IPADDR_ANY(a) (((a).addr[0u]) = TCPIP_IPADDR_ANY)  /**< set ip address */
#define TCPIP_IS_IPADDR_ANY(a)  (TCPIP_IPADDR_ANY == ((a).addr[0u])) /**< check if ip address is any */
#define TCPIP_EQ_IPADDR(a, b)   (((a).addr[0u]) == ((b).addr[0u]))   /**< compare ip addresses */
/* PRQA S 3472 -- */
#else
#define TCPIP_SET_IPADDR_ANY(a)       \
    do                                \
    {                                 \
        ((a).addr[0u]) = 0x00000000u; \
        ((a).addr[1u]) = 0x00000000u; \
        ((a).addr[2u]) = 0x00000000u; \
        ((a).addr[3u]) = 0x00000000u; \
    } while (0)
#define TCPIP_IS_IPADDR_ANY(a)                                                                             \
    ((0x00000000u == ((a).addr[0u])) && (0x00000000u == ((a).addr[1u])) && (0x00000000u == ((a).addr[2u])) \
     && (0x00000000u == ((a).addr[3u])))
#define TCPIP_EQ_IPADDR(a, b)                                                                                       \
    ((((a).addr[0u]) == ((b).addr[0u])) && (((a).addr[1u]) == ((b).addr[1u])) && (((a).addr[2u]) == ((b).addr[2u])) \
     && (((a).addr[3u]) == ((b).addr[3u])))
#endif
/**
 * @}
 */

/* ================================================ type definitions ================================================ */
/**
 * @name TcpIpAddressFamilyEnum
 * @{
 * @brief TcpIp address families.
 */
typedef uint16 TcpIp_DomainType;
#define TCPIP_AF_INET  ((uint16)0x02u) /**< Use IPv4 */
#define TCPIP_AF_INET6 ((uint16)0x1Cu) /**< Use IPv6 */
/**
 * @}
 */

/**
 * @name TcpIpProtocolTypeEnum
 * @{
 * @brief Protocol type used by a socket.
 */
typedef uint8 TcpIp_ProtocolType;
#define TCPIP_IPPROTO_TCP ((uint8)0x06u) /**< Use TCP */
#define TCPIP_IPPROTO_UDP ((uint8)0x11u) /**< Use UDP */
/**
 * @}
 */

/**
 * @brief This structure defines an IPv4 address type which can be derived from the generic address
 *        structure via cast.
 */
typedef struct TcpIp_SockAddrInetTag
{
    TcpIp_DomainType domain; /**< This is the code for the address format of this address */
    uint16           port;   /**< port number */
    /* PRQA S 1039 ++ */     /* VL_TcpIp_FlexibleArray */
    uint32 addr[1];          /**< IPv4 address in network byte order */
    /* PRQA S 1039 -- */
} TcpIp_SockAddrInetType;

/**
 * @brief This structure defines a IPv6 address type which can be derived from the generic address
 *        structure via cast.
 */
typedef struct TcpIp_SockAddrInet6Tag
{
    TcpIp_DomainType domain;                           /**< This is the code for the address format of this address */
    uint16           port;                             /**< port number */
    uint32           addr[TCPIP_IPV6ADDR_BUFFER_SIZE]; /**< IPv6 address in network byte order */
} TcpIp_SockAddrInet6Type;

#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
/**
 * @brief Generic structure used by APIs to specify an IP address.
 */
typedef TcpIp_SockAddrInetType TcpIp_SockAddrType;
#else
typedef TcpIp_SockAddrInet6Type TcpIp_SockAddrType;
#endif

/**
 * @brief Address identification type for unique identification of a local IP address and EthIf
 *        Controller configured in the TcpIp module.
 */
typedef uint8 TcpIp_LocalAddrIdType;

#if (0x100u >= (TCPIP_UDP_SOCKET_MAX + TCPIP_TCP_SOCKET_MAX))
/**
 * @brief Socket identifier type for unique identification of a TcpIp stack socket.
 *        TCPIP_SOCKETID_INVALID shall specify an invalid socket handle.
 */
typedef uint8 TcpIp_SocketIdType;
#define TCPIP_SOCKETID_INVALID ((TcpIp_SocketIdType)0xFFu)
#else
typedef uint16 TcpIp_SocketIdType;
#define TCPIP_SOCKETID_INVALID ((TcpIp_SocketIdType)0xFFFFu)
#endif

/**
 * @name TcpIpControllerStateEnum
 * @{
 * @brief Specifies the TcpIp state for a specific EthIf controller.
 */
typedef uint8 TcpIp_StateType;
/**
 * TCP/IP stack state for a specific EthIf controller is ONLINE,
 * i.e. communication via at least one IP address is possible.
 */
#define TCPIP_STATE_ONLINE ((TcpIp_StateType)(0x00u))
/**
 * TCP/IP stack state for a specific EthIf controller is ONHOLD,
 * i.e. no communication is currently possible (e.g. link down).
 */
#define TCPIP_STATE_ONHOLD ((TcpIp_StateType)(0x01u))
/**
 * TCP/IP stack state for a specific EthIf controller is OFFLINE,
 * i.e. no communication is possible.
 */
#define TCPIP_STATE_OFFLINE ((TcpIp_StateType)(0x02u))
/**
 * TCP/IP stack state for a specific EthIf controller is STARTUP,
 * i.e. IP address assignment in progress or ready for manual start,
 * communication is currently not possible.
 */
#define TCPIP_STATE_STARTUP ((TcpIp_StateType)(0x03u))
/**
 * TCP/IP stack state for a specific EthIf controller is SHUTDOWN,
 * i.e. release of resources using the EthIf controller, release of IP address assignment.
 */
#define TCPIP_STATE_SHUTDOWN ((TcpIp_StateType)(0x04u))
/**
 * @}
 */

/**
 * @name TcpIpLocalAddressAssignmentEnum
 * @{
 * @brief Specifies the state of local IP address assignment.
 */
typedef uint8 TcpIp_IpAddrStateType;
/** local IP address is assigned */
#define TCPIP_IPADDR_STATE_ASSIGNED ((TcpIp_IpAddrStateType)(0x00u))
/** local IP address is assigned, but cannot be used as the network is not active */
#define TCPIP_IPADDR_STATE_ONHOLD ((TcpIp_IpAddrStateType)(0x01u))
/** local IP address is unassigned */
#define TCPIP_IPADDR_STATE_UNASSIGNED ((TcpIp_IpAddrStateType)(0x02u))
/**
 * @}
 */

/**
 * @name TcpIpEventEnum
 * @{
 * @brief Events reported by TcpIp.
 */
typedef uint8 TcpIp_EventType;
/**
 * TCP connection was reset, TCP socket and all related resources have been released.
 */
#define TCPIP_TCP_RESET ((TcpIp_EventType)(0x01u))
/**
 * TCP connection was closed successfully, TCP socket and all related resources have been released.
 */
#define TCPIP_TCP_CLOSED ((TcpIp_EventType)(0x02u))
/**
 * A FIN signal was received on the TCP connection, TCP socket is still valid.
 */
#define TCPIP_TCP_FIN_RECEIVED ((TcpIp_EventType)(0x03u))
/**
 * UDP socket and all related resources have been released.
 */
#define TCPIP_UDP_CLOSED ((TcpIp_EventType)(0x04u))
/**
 * TLS handshake successfully established, TLS connection available.
 */
#define TCPIP_TLS_HANDSHAKE_SUCCEEDED ((TcpIp_EventType)(0x05u))
/**
 * @}
 */

/**
 * @name TcpIpIPAddressAssignmentPolicyEnum
 * @{
 * @brief Specification of IPv4/IPv6 address assignment policy.
 */
typedef uint8 TcpIp_IpAddrAssignmentType;
/** Static configured IPv4/IPv6 address. */
#define TCPIP_IPADDR_ASSIGNMENT_STATIC ((TcpIp_IpAddrAssignmentType)(0x00u))
/** Linklocal IPv4/IPv6 address assignment using DoIP parameters. */
#define TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP ((TcpIp_IpAddrAssignmentType)(0x01u))
/** Dynamic configured IPv4/IPv6 address by DHCP. */
#define TCPIP_IPADDR_ASSIGNMENT_DHCP ((TcpIp_IpAddrAssignmentType)(0x02u))
/** Linklocal IPv4/IPv6 address assignment. */
#define TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL ((TcpIp_IpAddrAssignmentType)(0x03u))
/** Dynamic configured IPv4/IPv6 address by Router Advertisement. */
#define TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER ((TcpIp_IpAddrAssignmentType)(0x04u))
/** All configured TcpIpAssignmentMethods with TcpIpAssignmentTrigger set to TCPIP_MANUAL */
#define TCPIP_IPADDR_ASSIGNMENT_ALL ((TcpIp_IpAddrAssignmentType)(0x05u))
/**
 * @}
 */

/**
 * @name TcpIpReturnTypeEnum
 * @{
 * @brief TcpIp specific return type.
 */
typedef uint8 TcpIp_ReturnType;
/** operation completed successfully. */
#define TCPIP_E_OK ((TcpIp_ReturnType)(0x00u))
/** operation failed. */
#define TCPIP_E_NOT_OK ((TcpIp_ReturnType)(0x01u))
/** operation failed because of an ARP/NDP cache miss. */
#define TCPIP_E_PHYS_ADDR_MISS ((TcpIp_ReturnType)(0x02u))
/** operation in progress */
#define TCPIP_E_PENDING ((TcpIp_ReturnType)(0x03u))
/**
 * @}
 */

/**
 * @name TcpIpParameterIDEnum
 * @{
 * @brief Type for the specification of all supported Parameter IDs and their data types.
 */
typedef uint8 TcpIp_ParamIdType;
/**
 * Specifies the maximum TCP receive window for the socket. [uint16]
 */
#define TCPIP_PARAMID_TCP_RXWND_MAX ((TcpIp_ParamIdType)0x00u)
/**
 * Specifies the frame priority for outgoing frames on the socket. [uint8]
 */
#define TCPIP_PARAMID_FRAMEPRIO ((TcpIp_ParamIdType)0x01u)
/**
 * Specifies if the Nagle Algorithm according to IETF RFC 1122
 * (chapter 4.2.3.4 When to Send Data) is enabled or not. [boolean]
 */
#define TCPIP_PARAMID_TCP_NAGLE ((TcpIp_ParamIdType)0x02u)
/**
 * Specifies if TCP Keep Alive Probes are sent on the socket connection. [boolean]
 */
#define TCPIP_PARAMID_TCP_KEEPALIVE ((TcpIp_ParamIdType)0x03u)
/**
 * Specifies the time to live value for outgoing frames on the socket.
 * For IPv6 this parameter specifies the value of the HopLimit field used in the IPv6 header. [uint8]
 */
#define TCPIP_PARAMID_TTL ((TcpIp_ParamIdType)0x04u)
/**
 * Specifies the time in [s] between the last data packet sent (simple ACKs are not considered data)
 * and the first keepalive probe. [uint32]
 */
#define TCPIP_PARAMID_TCP_KEEPALIVE_TIME ((TcpIp_ParamIdType)0x05u)
/**
 * Specifies the maximum number of times that a keepalive probe is retransmitted. [uint16]
 */
#define TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX ((TcpIp_ParamIdType)0x06u)
/**
 * Specifies the interval in [s] between subsequent keepalive probes. [uint32]
 */
#define TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL ((TcpIp_ParamIdType)0x07u)
/**
 * Specifies which TCP option filter shall be applied on the related socket. [uint8]
 */
#define TCPIP_PARAMID_TCP_OPTIONFILTER ((TcpIp_ParamIdType)0x08u)
/**
 * Specifies if the Path MTU Discovery shall be performed on the related socket. [boolean]
 */
#define TCPIP_PARAMID_PATHMTU_ENABLE ((TcpIp_ParamIdType)0x09u)
/**
 * The 20-bit Flow Label according to IETF RFC 6437. [uint32]
 */
#define TCPIP_PARAMID_FLOWLABEL ((TcpIp_ParamIdType)0x0Au)
/**
 * The 6-bit Differentiated Service Code Point according to IETF RFC 2474. [uint8]
 */
#define TCPIP_PARAMID_DSCP ((TcpIp_ParamIdType)0x0Bu)
/**
 * Specifies if UDP checksum handling shall be enabled (TRUE) or
 * skipped (FALSE) on the related socket. [boolean]
 */
#define TCPIP_PARAMID_UDP_CHECKSUM ((TcpIp_ParamIdType)0x0Cu)
/**
 * 0x0d is used to assign a TLS connection reference to a TCP socket.
 */
#define TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT ((TcpIp_ParamIdType)0x0Du)
/**
 * Initial TCP Retransmission timeout before an unacknowledged segment is retransmitted
 * (overrides TcpIpTcp RetransmissionTimeout (ECUC_ TcpIp_00068))
 */
#define TCPIP_PARAMID_TCP_RETRANSMIT_TIMEOUT ((TcpIp_ParamIdType)0x0Eu)
/**
 * Specifies the maximum number of TCP retransmissions
 */
#define TCPIP_PARAMID_TCP_MAXRTX ((TcpIp_ParamIdType)0x0Fu)
/**
 * Maximal TCP Retransmission timeout before an unacknowledged segment is retransmitted
 * (overrides TcpIpTcp MaxRetransmissionTimeout (ECUC_TcpIp_00340))
 */
#define TCPIP_PARAMID_TCP_MAX_RETRANSMIT_TIMEOUT ((TcpIp_ParamIdType)0x10u)
/**
 * Start of vendor specific range of parameter IDs. [vendor specific]
 */
#define TCPIP_PARAMID_VENDOR_SPECIFIC ((TcpIp_ParamIdType)0x80u)
/**
 * parameter ID for VLAN
 */
#define TCPIP_PARAMID_SOCKET_VLAN ((TcpIp_ParamIdType)0x81u)
/**
 * @}
 */

/**
 * @name TcpIp_ArpEntryStateType
 * @{
 * @brief state of ARP entry.
 */
typedef uint8 TcpIp_ArpEntryStateType;
/** static arp entry. */
#define TCPIP_ARP_ENTRY_STATIC ((TcpIp_ArpEntryStateType)(0x00u))
/** valid arp entry. */
#define TCPIP_ARP_ENTRY_VALID ((TcpIp_ArpEntryStateType)(0x01u))
/** stale arp entry. */
#define TCPIP_ARP_ENTRY_STALE ((TcpIp_ArpEntryStateType)(0x02u))
/**
 * @}
 */

/**
 * @brief TcpIp_ArpCacheEntries elements type.
 */
typedef struct TcpIp_ArpCacheEntryTag
{
    /** IPv4 address in network byte order */
    uint32 InetAddr[1];
    /** physical address in network byte order */
    uint8 PhysAddr[TCPIP_PHYSICALADDR_BUFFER_SIZE];
    /** state of the address entry */
    uint8 State;
} TcpIp_ArpCacheEntryType;

/**
 * @brief TcpIp_NdpCacheEntries elements type.
 */
typedef struct TcpIp_NdpCacheEntryTag
{
    /** IPv6 address in network byte order */
    uint32 Inet6Addr[TCPIP_IPV6ADDR_BUFFER_SIZE];
    /** physical address in network byte order */
    uint8 PhysAddr[TCPIP_PHYSICALADDR_BUFFER_SIZE];
    /** state of the address entry */
    uint8 State;
} TcpIp_NdpCacheEntryType;

/**
 * @name TcpIpMeasurementTypeEnum
 * @{
 * @brief Index to select specific measurement data.
 */
typedef uint8 TcpIp_MeasurementIdxType;
/** Measurement index of dropped PDUs caused by invalid destination TCP-Port */
#define TCPIP_MEAS_DROP_TCP ((TcpIp_MeasurementIdxType)0x01u)
/** Measurement index of dropped PDUs caused by invalid destination UDP-Port */
#define TCPIP_MEAS_DROP_UDP ((TcpIp_MeasurementIdxType)0x02u)
/** Measurement index of dropped datagrams caused by invalid IPv4 address */
#define TCPIP_MEAS_DROP_IPV4 ((TcpIp_MeasurementIdxType)0x03u)
/** Measurement index of dropped datagrams caused by invalid IPv6 address */
#define TCPIP_MEAS_DROP_IPV6 ((TcpIp_MeasurementIdxType)0x04u)
/** represents all measurement indexes */
#define TCPIP_MEAS_ALL ((TcpIp_MeasurementIdxType)0xFFu)
/**
 * @}
 */

/**
 * @name TcpIpTLSConnectionIdentifierType
 * @{
 * @brief TLS connection identifier type for unique identification of a TLS connection.
 *        TCPIP_TLSCONNECTIONID_INVALID shall specify an invalid TLS connection handle.
 */
typedef uint16 TcpIp_TlsConnectionIdType;
#define TCPIP_TLSCONNECTIONID_INVALID ((TcpIp_TlsConnectionIdType)0xFFFFu)
/**
 * @}
 */

#if defined(TCPIP_SPPORT_ENUM_TYPE)
typedef enum
{
    TLS_CLIENT,
    TLS_SERVER
} TcpIp_TlsConnectionInitStateType;

typedef enum
{
    TLS_VERSION_V12,
    TLS_VERSION_V13
} TcpIp_TlsVersionType;

typedef enum
{
    TLS_PRF_CSM_FULL_SUPPORT,
    TLS_PRF_CSM_INOUT_REDIRECT_SUPPORT,
    TLS_PRF_CSM_NO_SUPPORT,
    TLS_PRF_NO_SUPPORT
} TcpIp_TlsCsmPRFSupportType;
#else
typedef uint8 TcpIp_TlsConnectionInitStateType;
#define TLS_CLIENT                         ((TcpIp_TlsConnectionInitStateType)(0x0u))
#define TLS_SERVER                         ((TcpIp_TlsConnectionInitStateType)(0x1u))

typedef uint8 TcpIp_TlsVersionType;
#define TLS_VERSION_V12                    ((TcpIp_TlsVersionType)(0x0u))
#define TLS_VERSION_V13                    ((TcpIp_TlsVersionType)(0x1u))

typedef uint8 TcpIp_TlsCsmPRFSupportType;
#define TLS_PRF_CSM_FULL_SUPPORT           ((TcpIp_TlsCsmPRFSupportType)(0x0u))
#define TLS_PRF_CSM_INOUT_REDIRECT_SUPPORT ((TcpIp_TlsCsmPRFSupportType)(0x1u))
#define TLS_PRF_CSM_NO_SUPPORT             ((TcpIp_TlsCsmPRFSupportType)(0x2u))
#define TLS_PRF_NO_SUPPORT                 ((TcpIp_TlsCsmPRFSupportType)(0x3u))
#endif

typedef uint32 TcpIp_CsmJobsIdType;

typedef uint32 TcpIp_CsmKeyIdType;

typedef uint16 TcpIp_KeyMCertificateIdType;

#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
/** ceiling of a integer division */
#define TCPIP_DIV_CEILING(a, b) (((a) + (b) - 1u) / (b))
/** local address number of controller */
#define TCPIP_CTRL_LOCALADDR_NUMBER_32BITS TCPIP_DIV_CEILING(TCPIP_CTRL_MULTI_LOCALADDR_NUMBER, 32u)
#endif

#if !defined(TCPIP_SUPPORT_AUTOIP_INITTIMER)
/** unused macro */
#define TCPIP_SUPPORT_AUTOIP_INITTIMER STD_OFF
#endif

/**
 * @brief TcpIp controller runtime.
 */
typedef struct TcpIp_CtrlRuntimeTag
{
    /** controller index @range NA */
    uint8 CtrlIndex;
    /** state flag @range NA */
    uint8 StateFlag;
    /** controller status @range NA */
    TcpIp_StateType CtrlStatus;
#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)
    /** mask flag @range NA */
    uint8 maskFlg;
#endif
    /** number of sockets unsed by controller @range NA */
    TcpIp_SocketIdType SocketUsedNum;
#if (STD_ON == TCPIP_CTRL_MULTI_LOCALADDR)
    /** bit map for usage of local address @range NA */
    uint32 LocalAddrUsed[TCPIP_CTRL_LOCALADDR_NUMBER_32BITS];
#endif
    /** pointer to lwIp ICMP pcb @range NA */
    struct raw_pcb* IcmpPcbPtr;
    /** lwIp NetIf variable @range NA */
    struct netif Netif;
    /** lwIp NetIf callback variable @range NA */
    netif_ext_callback_t NetifExtCallback;
#if (STD_ON == TCPIP_DHCP_CLIENT_ENABLED)
    /** lwIp dhcp @range NA */
    struct dhcp netif_dhcp;
#endif
#if (STD_ON == TCPIP_AUTOIP_ENABLED)
#if (STD_ON == TCPIP_SUPPORT_AUTOIP_INITTIMER)
    /** AutoIp timer @range NA */
    uint32 autoipTimer;
#endif
    /** lwIp AutoIp variable @range NA */
    struct autoip netif_autoip;
#endif
} TcpIp_CtrlRuntimeType;

/**
 * @brief TcpIp local address runtime.
 */
typedef struct TcpIp_LocalAddrRuntimeTag
{
    /** local address ID @range NA */
    TcpIp_LocalAddrIdType LocalAddrId;
#if LWIP_NETIF_HOSTNAME
    /** host name length @range NA */
    uint8 HostNameLen;
#endif
    /** flag @range NA */
    uint8 Flag;
#if (STD_ON == TCPIP_BSDSOCKET_SUPPORT)
    /** flag for BSD implementation @range NA */
    uint32 BsdFlag;
#endif
    /** local IP address @range NA */
    ip_addr_t LocalIp;
    /** local IP netmask @range NA */
    ip_addr_t Netmask;
    /** default router IP address @range NA */
    ip_addr_t RouterIp;
} TcpIp_LocalAddrRuntimeType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
