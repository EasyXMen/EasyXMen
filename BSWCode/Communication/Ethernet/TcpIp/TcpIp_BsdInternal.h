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
**  FILENAME    : TcpIp_BsdInternal.h                                                                                 **
**                                                                                                                    **
**  Created on  : 2022/11/14                                                                                          **
**  Author      : fupeng.yu                                                                                           **
**  Vendor      :                                                                                                     **
**  DESCRIPTION : internal type definition for BsdTcpIp                                                               **
**                                                                                                                    **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                                                              **
**                                                                                                                    **
***********************************************************************************************************************/

#ifndef TCPIP_BSDINTERNAL_H_
#define TCPIP_BSDINTERNAL_H_

/***********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
#include "TcpIp_PBcfg.h"
#if (STD_ON == TCPIP_BSDSOCKET_SUPPORT)
#if (STD_ON == TCIP_USED_MOUDLE_DET)
#include "Det.h"
#endif /* (STD_ON == TCIP_USED_MOUDLE_DET) */

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 ***********************************************************************************************************************/
#if !defined(TCPIP_LOCAL)
#define TCPIP_LOCAL static
#endif /* !defined(TCPIP_LOCAL) */

#if !defined(TCPIP_LOCAL_INLINE)
#define TCPIP_LOCAL_INLINE static inline
#endif /* !defined(TCPIP_LOCAL_INLINE) */

#if !defined(TCPIP_UNUSED_ARG)
#define TCPIP_UNUSED_ARG(x) (void)(x)
#endif /* !defined(TCPIP_UNUSED_ARG) */

#define TCPIP_SOCKET_NUM (TCPIP_UDP_SOCKET_MAX + TCPIP_TCP_SOCKET_MAX)

#if (STD_ON == TCIP_USED_MOUDLE_DET)
#define TCPIP_DET(api, error)                                                                                \
    do                                                                                                       \
    {                                                                                                        \
        (void)Det_ReportError((uint16)TCPIP_MODULE_ID, (uint8)TCPIP_INSTANCE, (uint8)(api), (uint8)(error)); \
    } while (0)

#else
#define TCPIP_DET(api, error)
#endif /* STD_ON == TCIP_USED_MOUDLE_DET */

typedef uint8 TcpIp_InitStateType;
#define TCPIP_STATE_UNINIT 0x0u
#define TCPIP_STATE_INIT   0x1u

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* socket parameters handle type */
typedef Std_ReturnType (*praHanldFuncPtrType)(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) ParameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) ParameterValue);
typedef struct
{
    VAR(uint32, AUTOMATIC)
    paramId;
    VAR(praHanldFuncPtrType, AUTOMATIC)
    praHanldFuncPtr;
} TcpIp_SocketParaHandleType;

BEGIN_C_DECLS
typedef unsigned long uintx;
/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 ***********************************************************************************************************************/

FUNC(void, TCPIP_CODE)
TcpIp_BsdInit(P2CONST(TcpIp_ConfigType, AUTOMATIC, TCPIP_APPL_CONST) ConfigPtr);

FUNC(void, TCPIP_CODE) TcpIp_BsdDeInit(void);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdCloseSocketHandle(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(boolean, AUTOMATIC) Abort);

#if ((TCPIP_LOCAL_ADR_NUM > 0u) && (TCPIP_SOCKET_NUM > 0u))
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdBind(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAddrId,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) portPtr);
#endif /* ((TCPIP_LOCAL_ADR_NUM > 0u)&&(TCPIP_SOCKET_NUM > 0u)) */

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdTcpConnect(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdTcpListen(VAR(TcpIp_SocketIdType, AUTOMATIC) socketId, VAR(uint8, AUTOMATIC) maxChannels);

#if (TCPIP_CONTROLLER_NUM > 0)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdRequestComMode(VAR(uint8, AUTOMATIC) ctrlIdx, VAR(TcpIp_StateType, AUTOMATIC) state);
#endif /* (TCPIP_CONTROLLER_NUM > 0) */

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdRequestIpAddrAssignment(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAddrId,
    VAR(TcpIp_IpAddrAssignmentType, AUTOMATIC) type,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) localIpAddrPtr,
    VAR(uint8, AUTOMATIC) Netmask,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) defaultRouterPtr);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdReleaseIpAddrAssignment(VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAddrId);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdChangeParameter(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    VAR(TcpIp_ParamIdType, AUTOMATIC) parameterId,
    P2CONST(void, AUTOMATIC, TCPIP_APPL_CONST) parameterValue);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdGetIpAddr(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) localAdrIndx,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) netmaskPtr,
    P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) defaultRouterPtr);

#if (STD_ON == TCPIP_UDP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdUdpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) remoteAddrPtr,
    VAR(uint16, AUTOMATIC) totalLength);
#endif /* (STD_ON == TCPIP_UDP_ENABLED) */

#if (STD_ON == TCPIP_TCP_ENABLED)
FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdTcpTransmit(
    VAR(TcpIp_SocketIdType, AUTOMATIC) socketId,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_CONST) dataPtr,
    VAR(uint32, AUTOMATIC) availableLength,
    VAR(boolean, AUTOMATIC) forceRetrieve);
#endif /* (STD_ON == TCPIP_TCP_ENABLED) */

FUNC(void, TCPIP_CODE) TcpIp_BsdMainFunction(void);

FUNC(Std_ReturnType, TCPIP_CODE)
TcpIp_BsdGetSocket(
    VAR(TcpIp_DomainType, AUTOMATIC) domain,
    VAR(TcpIp_ProtocolType, AUTOMATIC) protocol,
#if (STD_ON == TCPIP_MUILT_SOCKET_OWNERCFG)
    VAR(uint8, AUTOMATIC) socketOwnererId,
#endif /* (STD_ON == TCPIP_MUILT_SOCKET_OWNERCFG) */
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr);

END_C_DECLS

#endif /* #if (STD_ON == TCPIP_BSDSOCKET_SUPPORT) */
#endif /* TCPIP_BSDINTERNAL_H_ */
