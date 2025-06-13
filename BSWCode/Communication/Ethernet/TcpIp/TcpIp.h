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
**  FILENAME    : TcpIp.h                                                                                             **
**                                                                                                                    **
**  Created on  : 03/12/18                                                                                            **
**  Author      : darren.zhang                                                                                        **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : function declaration for TcpIp                                                                      **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

#ifndef TCPIP_H_
#define TCPIP_H_

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/

#include "TcpIp_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_PBcfg.h"
#include "TcpIp_SocketOwner.h"
#include "Eth_GeneralTypes.h"
#include "SchM_TcpIp.h"
/***********************************************************************************************************************
 *                              SOURCE FILE VERSION INFORMATION
 ***********************************************************************************************************************/
#define TCPIP_VENDOR_ID          62
#define TCPIP_MODULE_ID          170
#define TCPIP_INSTANCE           0
#define TCPIP_H_AR_MAJOR_VERSION 4
#define TCPIP_H_AR_MINOR_VERSION 5
#define TCPIP_H_AR_PATCH_VERSION 0

/* Service IDs definition */
#define TCPIP_SID_INIT                    0x01
#define TCPIP_SID_GETVERSIONINFO          0x02
#define TCPIP_SID_CLOSE                   0x04
#define TCPIP_SID_BIND                    0x05
#define TCPIP_SID_TCPCONNECT              0x06
#define TCPIP_SID_TCPLISTEN               0x07
#define TCPIP_SID_TCPRECEIVED             0x08
#define TCPIP_SID_REQUESTCOMMODE          0x09
#define TCPIP_SID_REQUESTIPADDRASSIGNMENT 0x0A
#define TCPIP_SID_RELEASEIPADDRASSIGNMENT 0x0B
#define TCPIP_SID_RESETIPASSIGNMENT       0x1B
#define TCPIP_SID_ICMPTRANSMIT            0x0C
#define TCPIP_SID_ICMPV6TRANSMIT          0x18
#define TCPIP_SID_DHCPREADOPTION          0x0D
#define TCPIP_SID_DHCPV6READOPTION        0x19
#define TCPIP_SID_DHCPWRITEOPTION         0x0E
#define TCPIP_SID_DHCPV6WRITEOPTION       0x1A
#define TCPIP_SID_CHANGEPARAMETER         0x0F
#define TCPIP_SID_GETIPADDR               0x10
#define TCPIP_SID_GETPHYSADDR             0x11
#define TCPIP_SID_GETREMOTEPHYSADDR       0x16
#define TCPIP_SID_GETCTRLIDX              0x17
#define TCPIP_SID_UDPTRANSMIT             0x12
#define TCPIP_SID_TCPTRANSMIT             0x13
#define TCPIP_SID_RXINDICATION            0x14
#define TCPIP_SID_MAINFUNCTION            0x15
#define TCPIP_SID_UP_GETSOCKET            0x03 /*TcpIp_<Up>GetSocket*/
#define TCPIP_SID_TLSPROTOCOL             0x22
/* DET error codes */
#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
#define TCPIP_E_NOTINIT       0x01
#define TCPIP_E_PARAM_POINTER 0x02
#define TCPIP_E_INV_ARG       0x03
#define TCPIP_E_NOBUFS        0x04
#define TCPIP_E_MSGSIZE       0x07
#define TCPIP_E_PROTOTYPE     0x08
#define TCPIP_E_ADDRINUSE     0x09
#define TCPIP_E_ADDRNOTAVAIL  0x0A
#define TCPIP_E_ISCONN        0x0B
#define TCPIP_E_NOTCONN       0x0C
#define TCPIP_E_NOPROTOOPT    0x0D
#define TCPIP_E_AFNOSUPPORT   0x0E
#define TCPIP_E_INIT_FAILED   0x0F
#endif /*STD_ON == TCPIP_DEV_ERROR_DETECT*/

extern CONST(TcpIp_ConfigType, TCPIP_CONST) TcpIp_Config;

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 ***********************************************************************************************************************/

BEGIN_C_DECLS

FUNC(void, TCPIP_CODE) TcpIp_Init(P2CONST(TcpIp_ConfigType, AUTOMATIC, TCPIP_APPL_CONST) ConfigPtr);

#if (STD_ON == TCPIP_VERSION_INFO_API)
FUNC(void, TCPIP_CODE)
TcpIp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, TCPIP_APPL_DATA) versioninfo);
#endif /*STD_ON == TCPIP_VERSION_INFO_API*/

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_Close(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(boolean, AUTOMATIC) Abort);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_Bind(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalAddrId,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr);

#if (STD_ON == TCPIP_TCP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TcpConnect(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) RemoteAddrPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TcpListen(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(uint16, AUTOMATIC) MaxChannels);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TcpReceived(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(uint32, AUTOMATIC) Length);
#endif /*STD_ON == TCPIP_TCP_ENABLED*/

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_RequestComMode(VAR(uint8, AUTOMATIC) CtrlIdx, VAR(TcpIp_StateType, AUTOMATIC) State);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_RequestIpAddrAssignment(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalAddrId,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) Type,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) LocalIpAddrPtr,
    VAR(uint8, AUTOMATIC) Netmask,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) DefaultRouterPtr);

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ReleaseIpAddrAssignment(VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalAddrId);

#if (STD_ON == TCPIP_RESET_IP_ASSIGNMENT_API)
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ResetIpAssignment(VAR(void, AUTOMATIC));
#endif /* STD_ON == TCPIP_RESET_IP_ASSIGNMENT_API */

#if (STD_ON == TCPIP_ICMP_ENABLED)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_IcmpTransmit(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalIpAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) RemoteAddrPtr,
    VAR(uint8, AUTOMATIC) Ttl,
    VAR(uint8, AUTOMATIC) Type,
    VAR(uint8, AUTOMATIC) Code,
    VAR(uint16, AUTOMATIC) DataLength,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) DataPtr);
#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS || TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */

#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_IcmpV6Transmit(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalIpAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) RemoteAddrPtr,
    VAR(uint8, AUTOMATIC) HopLimit,
    VAR(uint8, AUTOMATIC) Type,
    VAR(uint8, AUTOMATIC) Code,
    VAR(uint16, AUTOMATIC) DataLength,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) DataPtr);
#endif /* TCPIP_SC2 == TCPIP_SCALABILITY_CLASS || TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */
#endif /* STD_ON == TCPIP_ICMP_ENABLED */

#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_DhcpReadOption(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalIpAddrId,
    VAR(uint8, AUTOMATIC) Option,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_DhcpWriteOption(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalIpAddrId,
    VAR(uint8, AUTOMATIC) Option,
    VAR(uint8, AUTOMATIC) DataLength,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) DataPtr);
#endif /* TCPIP_SC1 == TCPIP_SCALABILITY_CLASS || TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */

#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_DhcpV6ReadOption(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalIpAddrId,
    VAR(uint8, AUTOMATIC) Option,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_DhcpV6WriteOption(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalIpAddrId,
    VAR(uint16, AUTOMATIC) Option,
    VAR(uint16, AUTOMATIC) DataLength,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) DataPtr);
#endif /* TCPIP_SC2 == TCPIP_SCALABILITY_CLASS || TCPIP_SC3 == TCPIP_SCALABILITY_CLASS */

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_ChangeParameter(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) ParameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) ParameterValue);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_GetIpAddr(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalAddrId,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_GetPhysAddr(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalAddrId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr);

FUNC(TcpIp_ReturnType, TCPIP_CODE)
TcpIp_GetRemotePhysAddr(
    VAR(uint8, AUTOMATIC) CtrlIdx,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
    VAR(boolean, AUTOMATIC) initRes);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_GetCtrlIdx(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) LocalAddrId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) CtrlIdxPtr);

#if (STD_ON == TCPIP_UDP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_UdpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) DataPtr,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) RemoteAddrPtr,
    VAR(uint16, AUTOMATIC) TotalLength);
#endif /* STD_ON == TCPIP_UDP_ENABLED */

#if (STD_ON == TCPIP_TCP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_TcpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) DataPtr,
    VAR(uint32, AUTOMATIC) AvailableLength,
    VAR(boolean, AUTOMATIC) ForceRetrieve);
#endif /* STD_ON == TCPIP_TCP_ENABLED */

FUNC(void, TCPIP_CODE)
TcpIp_RxIndication(
    VAR(uint8, AUTOMATIC) CtrlIdx,
    VAR(Eth_FrameType, AUTOMATIC) FrameType,
    VAR(boolean, AUTOMATIC) IsBroadcast,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) PhysAddrPtr,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    VAR(uint16, AUTOMATIC) LenByte);

END_C_DECLS

#endif /* TCPIP_H */
