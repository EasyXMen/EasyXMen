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
**  FILENAME    : TcpIp_Types.h                                                                                       **
**                                                                                                                    **
**  Created on  : 03/12/18                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION :  type definition for TcpIp                                                                          **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

#ifndef TCPIP_TYPES_H_
#define TCPIP_TYPES_H_

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/

#include "ComStack_Types.h"
#include "TcpIp_Cfg.h"

typedef uint8 TcpIp_RefType;

/*TcpIp address families*/
#define TCPIP_AF_INET  ((uint16)0x02)
#define TCPIP_AF_INET6 ((uint16)0x1c)
typedef uint16 TcpIp_DomainType;

/*Protocol type used by a socket*/
#define TCPIP_IPPROTO_TCP ((uint8)0x06)
#define TCPIP_IPPROTO_UDP ((uint8)0x11)
typedef uint8 TcpIp_ProtocolType;

/*Address identification type for unique identification of a local IP address
and EthIf Controller configured in the TcpIp module*/
typedef uint8 TcpIp_LocalAddrIdType;

#if (0x100U > (1U + TCPIP_UDP_SOCKET_MAX + TCPIP_TCP_SOCKET_MAX))
/*socket identifier type for unique identification of a TcpIp stack socket.
TCPIP_SOCKETID_INVALID shall specify an invalid socket handle.
Type:uint8 or uint16,here is defined uint8*/
typedef uint8 TcpIp_SocketIdType;

/*TLS connection identifier type for unique identification of a TLS connection.
TCPIP_TLSCONNECTIONID_INVALID shall specify an invalid TLS connection handle.*/
typedef uint16 TcpIp_TlsConnectionIdType;

#else
typedef uint16 TcpIp_SocketIdType;
#endif /* 0x100U > (1U + TCPIP_UDP_SOCKET_MAX + TCPIP_TCP_SOCKET_MAX */

/*Type for the specification of all supported Parameter IDs*/
typedef uint8 TcpIp_ParamIdType;

#define TCPIP_PARAMID_TCP_RXWND_MAX             ((TcpIp_ParamIdType)0x00)
#define TCPIP_PARAMID_FRAMEPRIO                 ((TcpIp_ParamIdType)0x01)
#define TCPIP_PARAMID_TCP_NAGLE                 ((TcpIp_ParamIdType)0x02)
#define TCPIP_PARAMID_TCP_KEEPALIVE             ((TcpIp_ParamIdType)0x03)
#define TCPIP_PARAMID_TTL                       ((TcpIp_ParamIdType)0x04)
#define TCPIP_PARAMID_TCP_KEEPALIVE_TIME        ((TcpIp_ParamIdType)0x05)
#define TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX  ((TcpIp_ParamIdType)0x06)
#define TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL    ((TcpIp_ParamIdType)0x07)
#define TCPIP_PARAMID_TCP_OPTIONFILTER          ((TcpIp_ParamIdType)0x08)
#define TCPIP_PARAMID_PATHMTU_ENABLE            ((TcpIp_ParamIdType)0x09)
#define TCPIP_PARAMID_FLOWLABEL                 ((TcpIp_ParamIdType)0x0a)
#define TCPIP_PARAMID_DSCP                      ((TcpIp_ParamIdType)0x0b)
#define TCPIP_PARAMID_UDP_CHECKSUM              ((TcpIp_ParamIdType)0x0c)
#define TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT ((TcpIp_ParamIdType)0x0d)
#define TCPIP_PARAMID_VENDOR_SPECIFIC           ((TcpIp_ParamIdType)0x80)
#define TCPIP_PARAMID_SOCKET_VLAN               ((TcpIp_ParamIdType)0x81)

/*Index to select specific measurement data*/
typedef uint8 TcpIp_MeasurementIdxType;

#define TCPIP_MEAS_DROP_TCP  ((TcpIp_MeasurementIdxType)0x01)
#define TCPIP_MEAS_DROP_UDP  ((TcpIp_MeasurementIdxType)0x02)
#define TCPIP_MEAS_DROP_IPV4 ((TcpIp_MeasurementIdxType)0x03)
#define TCPIP_MEAS_DROP_IPV6 ((TcpIp_MeasurementIdxType)0x04)
#define TCPIP_MEAS_ALL       ((TcpIp_MeasurementIdxType)0xFF)
/*      TCPIP_MEAS_RESERVED_1       0x05-0x7F
 *      TCPIP_MEAS_RESERVED_2       0x80-0xEF
 *      TCPIP_MEAS_RESERVED_3       0xF0-0xFE
 */

#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
/*IPV4 address wildcard,the TCPIP_IPADDR_ANY implementation specific*/
/* PRQA S 3472++*/ /* MISRA Dir 4.9 */
#define TCPIP_SET_IPADDR_ANY(x) ((x).addr[0u] = 0x00000000u)
#define TCPIP_IS_IPADDR_ANY(x)  ((x).addr[0u] == 0x00000000u)
#define TCPIP_EQ_IPADDR(x, y)   ((x).addr[0u] == (y).addr[0u])
/* PRQA S 3472--*/ /* MISRA Dir 4.9 */
#else              /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS */
/*IPV6 address wildcard,the TCPIP_IP6ADDR_ANY implementation specific*/
#define TCPIP_SET_IPADDR_ANY(x)     \
    do                              \
    {                               \
        (x).addr[0u] = 0x00000000u; \
        (x).addr[1u] = 0x00000000u; \
        (x).addr[2u] = 0x00000000u; \
        (x).addr[3u] = 0x00000000u; \
    } while (0)

#define TCPIP_IS_IPADDR_ANY(x)                                                                       \
    (((x).addr[0u] == 0x00000000u) && ((x).addr[1u] == 0x00000000u) && ((x).addr[2u] == 0x00000000u) \
     && ((x).addr[3u] == 0x00000000u))

#define TCPIP_EQ_IPADDR(x, y)                                                                           \
    (((x).addr[0u] == (y).addr[0u]) && ((x).addr[0u] == (y).addr[0u]) && ((x).addr[0u] == (y).addr[0u]) \
     && ((x).addr[0u] == (y).addr[0u]))

#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS */

#define TCPIP_IPADDR_ANY ((uint32)0x00000000)
typedef uint32 TcpIpIpAddrWildcardType;

/*Port wildcard*/
#define TCPIP_PORT_ANY ((uint16)0x0000)
typedef uint16 TcpIpPortWildcardType;

/*LocalAddrId wildcard*/
#define TCPIP_LOCALADDRID_ANY ((uint8)0xff)
typedef TcpIp_LocalAddrIdType TcpIpLocalAddrIdWildcardType;
/***********************************************************************************************************************
 *                                             ENUMS
 ***********************************************************************************************************************/
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

/*Specifies the TcpIp state for a specific EthIf controller*/
typedef enum
{
    TCPIP_STATE_ONLINE = 0,
    TCPIP_STATE_ONHOLD,
    TCPIP_STATE_OFFLINE,
    TCPIP_STATE_STARTUP,
    TCPIP_STATE_SHUTDOWN
} TcpIp_StateType;

/*Specifies the state of local IP address assignment*/
typedef enum
{
    TCPIP_IPADDR_STATE_ASSIGNED = 0,
    TCPIP_IPADDR_STATE_ONHOLD,
    TCPIP_IPADDR_STATE_UNASSIGNED
} TcpIp_IpAddrStateType;

/*Events reported by TcpIp*/
typedef enum
{
    TCPIP_TCP_RESET = 0,
    TCPIP_TCP_CLOSED,
    TCPIP_TCP_FIN_RECEIVED,
    TCPIP_UDP_CLOSED,
    TCPIP_TLS_HANDSHAKE_SUCCEEDED
} TcpIp_EventType;

/*Specification of IPv4/IPv6 address assignment policy*/
typedef enum
{
    TCPIP_IPADDR_ASSIGNMENT_STATIC = 0,
    TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP,
    TCPIP_IPADDR_ASSIGNMENT_DHCP,
    TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL,
    TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER,
    TCPIP_IPADDR_ASSIGNMENT_ALL
} TcpIp_IpAddrAssignmentType;

/*TcpIp specific return type*/
typedef enum
{
    TCPIP_OK = 0,
    TCPIP_E_NOT_OK,
    TCPIP_E_PHYS_ADDR_MISS
} TcpIp_ReturnType;

#else /* !defined (TCPIP_SPPORT_ENUM_TYPE */
typedef uint8 TcpIp_TlsConnectionInitStateType;
#define TLS_CLIENT                             ((TcpIp_TlsConnectionInitStateType)(0x0u))
#define TLS_SERVER                             ((TcpIp_TlsConnectionInitStateType)(0x1u))

typedef uint8 TcpIp_TlsVersionType;
#define TLS_VERSION_V12                        ((TcpIp_TlsVersionType)(0x0u))
#define TLS_VERSION_V13                        ((TcpIp_TlsVersionType)(0x1u))

typedef uint8 TcpIp_TlsCsmPRFSupportType;
#define TLS_PRF_CSM_FULL_SUPPORT               ((TcpIp_TlsCsmPRFSupportType)(0x0u))
#define TLS_PRF_CSM_INOUT_REDIRECT_SUPPORT     ((TcpIp_TlsCsmPRFSupportType)(0x1u))
#define TLS_PRF_CSM_NO_SUPPORT                 ((TcpIp_TlsCsmPRFSupportType)(0x2u))
#define TLS_PRF_NO_SUPPORT                     ((TcpIp_TlsCsmPRFSupportType)(0x3u))

/* Specifies the TcpIp state for a specific EthIf controller */
typedef uint8 TcpIp_StateType;
#define TCPIP_STATE_ONLINE                     ((TcpIp_StateType)(0x0u))
#define TCPIP_STATE_ONHOLD                     ((TcpIp_StateType)(0x1u))
#define TCPIP_STATE_OFFLINE                    ((TcpIp_StateType)(0x2u))
#define TCPIP_STATE_STARTUP                    ((TcpIp_StateType)(0x3u))
#define TCPIP_STATE_SHUTDOWN                   ((TcpIp_StateType)(0x4u))

/* Specifies the state of local IP address assignment */
typedef uint8 TcpIp_IpAddrStateType;
#define TCPIP_IPADDR_STATE_ASSIGNED            ((TcpIp_IpAddrStateType)(0x0u))
#define TCPIP_IPADDR_STATE_ONHOLD              ((TcpIp_IpAddrStateType)(0x1u))
#define TCPIP_IPADDR_STATE_UNASSIGNED          ((TcpIp_IpAddrStateType)(0x2u))

/* Events reported by TcpIp */
typedef uint8 TcpIp_EventType;
#define TCPIP_TCP_RESET                        ((TcpIp_EventType)(0x0u))
#define TCPIP_TCP_CLOSED                       ((TcpIp_EventType)(0x1u))
#define TCPIP_TCP_FIN_RECEIVED                 ((TcpIp_EventType)(0x2u))
#define TCPIP_UDP_CLOSED                       ((TcpIp_EventType)(0x3u))
#define TCPIP_TLS_HANDSHAKE_SUCCEEDED          ((TcpIp_EventType)(0x4u))

/* Specification of IPv4/IPv6 address assignment policy */
typedef uint8 TcpIp_IpAddrAssignmentType;
#define TCPIP_IPADDR_ASSIGNMENT_STATIC         ((TcpIp_IpAddrAssignmentType)(0x0u))
#define TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP ((TcpIp_IpAddrAssignmentType)(0x1u))
#define TCPIP_IPADDR_ASSIGNMENT_DHCP           ((TcpIp_IpAddrAssignmentType)(0x2u))
#define TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL      ((TcpIp_IpAddrAssignmentType)(0x3u))
#define TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER    ((TcpIp_IpAddrAssignmentType)(0x4u))
#define TCPIP_IPADDR_ASSIGNMENT_ALL            ((TcpIp_IpAddrAssignmentType)(0x5u))

/*TcpIp specific return type*/
typedef uint8 TcpIp_ReturnType;
#define TCPIP_OK                               ((TcpIp_ReturnType)(0x0u))
#define TCPIP_E_NOT_OK                         ((TcpIp_ReturnType)(0x1u))
#define TCPIP_E_PHYS_ADDR_MISS                 ((TcpIp_ReturnType)(0x2u))

#endif /* defined(TCPIP_SPPORT_ENUM_TYPE) */

/***********************************************************************************************************************
 *                                STRUCTURES AND OTHER TYPEDEFS
 ***********************************************************************************************************************/

typedef uint32 TcpIp_CsmJobsIdType;

typedef uint32 TcpIp_CsmKeyIdType;

typedef uint16 TcpIp_KeyMCertificateIdType;

/* This structure defines an IPv4 address type which can be derived from the generic address structure via cast. */
typedef struct
{
    VAR(TcpIp_DomainType, TCPIP_VAR) domain;
    VAR(uint16, TCPIP_VAR) port;
    VAR(uint32, TCPIP_VAR) addr[1];
} TcpIp_SockAddrInetType;

/* This structure defines a IPv6 address type which can be derived from the generic address structure via cast. */
typedef struct
{
    VAR(TcpIp_DomainType, TCPIP_VAR) domain;
    VAR(uint16, TCPIP_VAR) port;
    VAR(uint32, TCPIP_VAR) addr[4];
} TcpIp_SockAddrInet6Type;

/* Generic structure used by APIs to specify an IP address. (A specific address type can be derived from this structure
via a cast to the specific struct type.) */
#if (TCPIP_SC1 == TCPIP_SCALABILITY_CLASS)
typedef TcpIp_SockAddrInetType TcpIp_SockAddrType;
#else  /* TCPIP_SC1 != TCPIP_SCALABILITY_CLASS */
typedef TcpIp_SockAddrInet6Type TcpIp_SockAddrType;
#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS */

typedef struct
{
    VAR(uint32, TCPIP_VAR) InetAddr[1u];
    VAR(uint8, TCPIP_VAR) PhysAddr[6u];
    /*state of the address entry (TCPIP_ARP_ENTRY_STATIC,
     * TCPIP_ARP_ENTRY_VALID, TCPIP_ARP_ENTRY_STALE)*/
    VAR(uint8, TCPIP_VAR) state;
} TcpIp_ArpCacheEntryType;

#endif /* SOURCECODE_SRC_TCPIP_TYPES_H_ */
