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
 **  @file               : TcpIp_DetError.h
 **  @author             : darren.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : TcpIp header file for DET error detections
 **
 ***********************************************************************************************************************/

#ifndef TCPIP_DETERROR_H_
#define TCPIP_DETERROR_H_

/* =================================================== inclusions =================================================== */
#include "Os_Types.h"
#include "ComStack_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_PBcfg.h"
#include "TcpIp_Types.h"
#include "TcpIp_CfgTypes.h"
#include "TcpIp.h"
#include "TcpIp_Internal.h"
#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
#include "Det.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/**
 * @name TcpIpServiceIdMacros
 * @{
 * @brief API service IDs are used to report to DET when an error occurs.
 */
#define TCPIP_SID_INIT                       0x01u /**< TcpIp_Init */
#define TCPIP_SID_GETVERSIONINFO             0x02u /**< TcpIp_GetVersionInfo */
#define TCPIP_SID_CLOSE                      0x04u /**< TcpIp_Close */
#define TCPIP_SID_BIND                       0x05u /**< TcpIp_Bind */
#define TCPIP_SID_TCPCONNECT                 0x06u /**< TcpIp_TcpConnect */
#define TCPIP_SID_TCPLISTEN                  0x07u /**< TcpIp_TcpListen */
#define TCPIP_SID_TCPRECEIVED                0x08u /**< TcpIp_TcpReceived */
#define TCPIP_SID_REQUESTCOMMODE             0x09u /**< TcpIp_RequestComMode */
#define TCPIP_SID_REQUESTIPADDRASSIGNMENT    0x0Au /**< TcpIp_RequestIpAddrAssignment */
#define TCPIP_SID_RELEASEIPADDRASSIGNMENT    0x0Bu /**< TcpIp_ReleaseIpAddrAssignment */
#define TCPIP_SID_RESETIPASSIGNMENT          0x1Bu /**< TcpIp_ResetIpAssignment */
#define TCPIP_SID_ICMPTRANSMIT               0x0Cu /**< TcpIp_IcmpTransmit */
#define TCPIP_SID_ICMPV6TRANSMIT             0x18u /**< TcpIp_IcmpV6Transmit */
#define TCPIP_SID_DHCPREADOPTION             0x0Du /**< TcpIp_DhcpReadOption */
#define TCPIP_SID_DHCPV6READOPTION           0x19u /**< TcpIp_DhcpV6ReadOption */
#define TCPIP_SID_DHCPWRITEOPTION            0x0Eu /**< TcpIp_DhcpWriteOption */
#define TCPIP_SID_DHCPV6WRITEOPTION          0x1Au /**< TcpIp_DhcpV6WriteOption */
#define TCPIP_SID_CHANGEPARAMETER            0x0Fu /**< TcpIp_ChangeParameter */
#define TCPIP_SID_GETIPADDR                  0x10u /**< TcpIp_GetIpAddr */
#define TCPIP_SID_GETPHYSADDR                0x11u /**< TcpIp_GetPhysAddr */
#define TCPIP_SID_GETREMOTEPHYSADDR          0x16u /**< TcpIp_GetRemotePhysAddr */
#define TCPIP_SID_GETCTRLIDX                 0x17u /**< TcpIp_GetCtrlIdx */
#define TCPIP_SID_GETARPCACHEENTRIES         0x1Du /**< TcpIp_GetArpCacheEntries */
#define TCPIP_SID_GETNDPCACHEENTRIES         0x1Cu /**< TcpIp_GetNdpCacheEntries */
#define TCPIP_SID_GETANDRESETMEASUREMENTDATA 0x45u /**< TcpIp_GetAndResetMeasurementData */
#define TCPIP_SID_ISCONNECTIONREADY          0x46u /**< TcpIp_IsConnectionReady */
#define TCPIP_SID_UDPTRANSMIT                0x12u /**< TcpIp_UdpTransmit */
#define TCPIP_SID_TCPTRANSMIT                0x13u /**< TcpIp_TcpTransmit */
#define TCPIP_SID_RXINDICATION               0x14u /**< TcpIp_RxIndication */
#define TCPIP_SID_MAINFUNCTION               0x15u /**< TcpIp_MainFunction */
#define TCPIP_SID_UP_GETSOCKET               0x03u /**< TcpIp_GetSocket */
#define TCPIP_SID_TLSPROTOCOL                0x22u /**< unused */
/**
 * @}
 */

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
/**
 * @name TcpIpDevelopmentErrorMacros
 * @{
 * @brief DET development errors.
 */
#define TCPIP_E_NO_ERROR      0x00u
#define TCPIP_E_UNINIT        0x01u /**< API service called before initializing the module */
#define TCPIP_E_PARAM_POINTER 0x02u /**< API service called with NULL pointer */
#define TCPIP_E_INV_ARG       0x03u /**< Invalid argument */
#define TCPIP_E_NOBUFS        0x04u /**< No buffer space available */
#define TCPIP_E_MSGSIZE       0x07u /**< Message too long */
#define TCPIP_E_PROTOTYPE     0x08u /**< Protocol wrong type for socket */
#define TCPIP_E_ADDRINUSE     0x09u /**< Address already in use */
#define TCPIP_E_ADDRNOTAVAIL  0x0Au /**< Can't assign requested address */
#define TCPIP_E_ISCONN        0x0Bu /**< Socket is already connected */
#define TCPIP_E_NOTCONN       0x0Cu /**< Socket is not connected */
#define TCPIP_E_NOPROTOOPT    0x0Du /**< Protocol not available */
#define TCPIP_E_AFNOSUPPORT   0x0Eu /**< Address family not supported by protocol family */
#define TCPIP_E_INIT_FAILED   0x0Fu /**< Invalid configuration set selection */
#define TCPIP_E_INV_PARTITION 0x10u /**< Invalid partition */
/**
 * @}
 */

/**
 * @name TcpIpRuntimeErrorMacros
 * @{
 * @brief DET runtime errors.
 */
#define TCPIP_E_TIMEDOUT    0x01u /**< Operation timed out */
#define TCPIP_E_CONNREFUSED 0x02u /**< Connection refused */
#define TCPIP_E_HOSTUNREACH 0x03u /**< No route to host */
#define TCPIP_E_PACKETTOBIG 0x04u /**< Path does not support frame size */
#define TCPIP_E_DADCONFLICT 0x05u /**< Duplicate IP Address detected */
/**
 * @}
 */
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (STD_ON == TCPIP_DEV_ERROR_DETECT)

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
/**
 * @brief              DET development error report.
 * @param[in]          api: service ID of API
 * @param[in]          error: error code
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_DetReportError(uint8 api, uint8 error)
{
    (void)Det_ReportError((uint16)TCPIP_MODULE_ID, (uint8)TCPIP_INSTANCE, api, error);
}

/**
 * @brief              DET runtime error report.
 * @param[in]          api: service ID of API
 * @param[in]          error: error code
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
TCPIP_LOCAL_INLINE void TcpIp_DetReportRuntimeError(uint8 api, uint8 error)
{
    (void)Det_ReportRuntimeError((uint16)TCPIP_MODULE_ID, (uint8)TCPIP_INSTANCE, api, error);
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

/**
 * @brief              Development error validation.
 * @param[in]          ConfigPtr: parameter of TcpIp_Init
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateInit(const TcpIp_ConfigType* ConfigPtr);

#if (STD_ON == TCPIP_VERSION_INFO_API)
/**
 * @brief              Development error validation.
 * @param[in]          versioninfo: parameter of TcpIp_GetVersionInfo
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateGetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_Close
 * @param[in]          Abort: parameter of TcpIp_Close
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateClose(TcpIp_SocketIdType SocketId, boolean Abort);

/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_Bind
 * @param[in]          LocalAddrId: parameter of TcpIp_Bind
 * @param[in]          PortPtr: parameter of TcpIp_Bind
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_ValidateBind(TcpIp_SocketIdType SocketId, TcpIp_LocalAddrIdType LocalAddrId, uint16* PortPtr);

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_TcpConnect
 * @param[in]          RemoteAddrPtr: parameter of TcpIp_TcpConnect
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateTcpConnect(TcpIp_SocketIdType SocketId, const TcpIp_SockAddrType* RemoteAddrPtr);

/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_TcpListen
 * @param[in]          MaxChannels: parameter of TcpIp_TcpListen
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateTcpListen(TcpIp_SocketIdType SocketId, uint16 MaxChannels);

/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_TcpReceived
 * @param[in]          Length: parameter of TcpIp_TcpReceived
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateTcpReceived(TcpIp_SocketIdType SocketId, uint32 Length);
#endif

/**
 * @brief              Development error validation.
 * @param[in]          CtrlIdx: parameter of TcpIp_RequestComMode
 * @param[in]          State: parameter of TcpIp_RequestComMode
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateRequestComMode(uint8 CtrlIdx, TcpIp_StateType State);

/**
 * @brief              Development error validation.
 * @param[in]          LocalAddrId: parameter of TcpIp_RequestIpAddrAssignment
 * @param[in]          Type: parameter of TcpIp_RequestIpAddrAssignment
 * @param[in]          LocalIpAddrPtr: parameter of TcpIp_RequestIpAddrAssignment
 * @param[in]          Netmask: parameter of TcpIp_RequestIpAddrAssignment
 * @param[in]          DefaultRouterPtr: parameter of TcpIp_RequestIpAddrAssignment
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateRequestIpAddrAssignment(
    TcpIp_LocalAddrIdType      LocalAddrId,
    TcpIp_IpAddrAssignmentType Type,
    const TcpIp_SockAddrType*  LocalIpAddrPtr,
    uint8                      Netmask,
    const TcpIp_SockAddrType*  DefaultRouterPtr);

/**
 * @brief              Development error validation.
 * @param[in]          LocalAddrId: parameter of TcpIp_ReleaseIpAddrAssignment
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateReleaseIpAddrAssignment(TcpIp_LocalAddrIdType LocalAddrId);

#if ((STD_ON == TCPIP_DHCP_CLIENT_ENABLED) && (STD_ON == TCPIP_RESET_IP_ASSIGNMENT_API))
/**
 * @brief              Development error validation.
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateResetIpAssignment(void);
#endif

#if (STD_ON == TCPIP_ICMP_ENABLED)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @brief              Development error validation.
 * @param[in]          LocalIpAddrId: parameter of TcpIp_IcmpTransmit
 * @param[in]          RemoteAddrPtr: parameter of TcpIp_IcmpTransmit
 * @param[in]          Ttl: parameter of TcpIp_IcmpTransmit
 * @param[in]          Type: parameter of TcpIp_IcmpTransmit
 * @param[in]          Code: parameter of TcpIp_IcmpTransmit
 * @param[in]          DataLength: parameter of TcpIp_IcmpTransmit
 * @param[in]          DataPtr: parameter of TcpIp_IcmpTransmit
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateIcmpTransmit(
    TcpIp_LocalAddrIdType     LocalIpAddrId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    uint8                     Ttl,
    uint8                     Type,
    uint8                     Code,
    uint16                    DataLength,
    const uint8*              DataPtr);
#endif

#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @brief              Development error validation.
 * @param[in]          LocalIpAddrId: parameter of TcpIp_IcmpV6Transmit
 * @param[in]          RemoteAddrPtr: parameter of TcpIp_IcmpV6Transmit
 * @param[in]          HopLimit: parameter of TcpIp_IcmpV6Transmit
 * @param[in]          Type: parameter of TcpIp_IcmpV6Transmit
 * @param[in]          Code: parameter of TcpIp_IcmpV6Transmit
 * @param[in]          DataLength: parameter of TcpIp_IcmpV6Transmit
 * @param[in]          DataPtr: parameter of TcpIp_IcmpV6Transmit
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateIcmpV6Transmit(
    TcpIp_LocalAddrIdType     LocalIpAddrId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    uint8                     HopLimit,
    uint8                     Type,
    uint8                     Code,
    uint16                    DataLength,
    const uint8*              DataPtr);
#endif
#endif

#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @brief              Development error validation.
 * @param[in]          LocalIpAddrId: parameter of TcpIp_DhcpReadOption
 * @param[in]          Option: parameter of TcpIp_DhcpReadOption
 * @param[in]          DataLength: parameter of TcpIp_DhcpReadOption
 * @param[in]          DataPtr: parameter of TcpIp_DhcpReadOption
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_ValidateDhcpReadOption(TcpIp_LocalAddrIdType LocalIpAddrId, uint8 Option, uint8* DataLength, uint8* DataPtr);
#endif

#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @brief              Development error validation.
 * @param[in]          LocalIpAddrId: parameter of TcpIp_DhcpV6ReadOption
 * @param[in]          Option: parameter of TcpIp_DhcpV6ReadOption
 * @param[in]          DataLength: parameter of TcpIp_DhcpV6ReadOption
 * @param[in]          DataPtr: parameter of TcpIp_DhcpV6ReadOption
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateDhcpV6ReadOption(
    TcpIp_LocalAddrIdType LocalIpAddrId,
    uint16                Option,
    uint16*               DataLength,
    uint8*                DataPtr);
#endif

#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @brief              Development error validation.
 * @param[in]          LocalIpAddrId: parameter of TcpIp_DhcpWriteOption
 * @param[in]          Option: parameter of TcpIp_DhcpWriteOption
 * @param[in]          DataLength: parameter of TcpIp_DhcpWriteOption
 * @param[in]          DataPtr: parameter of TcpIp_DhcpWriteOption
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateDhcpWriteOption(
    TcpIp_LocalAddrIdType LocalIpAddrId,
    uint8                 Option,
    uint8                 DataLength,
    const uint8*          DataPtr);
#endif

#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
/**
 * @brief              Development error validation.
 * @param[in]          LocalIpAddrId: parameter of TcpIp_DhcpV6WriteOption
 * @param[in]          Option: parameter of TcpIp_DhcpV6WriteOption
 * @param[in]          DataLength: parameter of TcpIp_DhcpV6WriteOption
 * @param[in]          DataPtr: parameter of TcpIp_DhcpV6WriteOption
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateDhcpV6WriteOption(
    TcpIp_LocalAddrIdType LocalIpAddrId,
    uint16                Option,
    uint16                DataLength,
    const uint8*          DataPtr);
#endif

/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_ChangeParameter
 * @param[in]          ParameterId: parameter of TcpIp_ChangeParameter
 * @param[in]          ParameterValue: parameter of TcpIp_ChangeParameter
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateChangeParameter(
    TcpIp_SocketIdType SocketId,
    TcpIp_ParamIdType  ParameterId,
    const uint8*       ParameterValue);

/**
 * @brief              Development error validation.
 * @param[in]          LocalAddrId: parameter of TcpIp_GetIpAddr
 * @param[in]          IpAddrPtr: parameter of TcpIp_GetIpAddr
 * @param[in]          NetmaskPtr: parameter of TcpIp_GetIpAddr
 * @param[in]          DefaultRouterPtr: parameter of TcpIp_GetIpAddr
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateGetIpAddr(
    TcpIp_LocalAddrIdType LocalAddrId,
    TcpIp_SockAddrType*   IpAddrPtr,
    uint8*                NetmaskPtr,
    TcpIp_SockAddrType*   DefaultRouterPtr);

/**
 * @brief              Development error validation.
 * @param[in]          LocalAddrId: parameter of TcpIp_GetPhysAddr
 * @param[in]          PhysAddrPtr: parameter of TcpIp_GetPhysAddr
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateGetPhysAddr(TcpIp_LocalAddrIdType LocalAddrId, uint8* PhysAddrPtr);

/**
 * @brief              Development error validation.
 * @param[in]          CtrlIdx: parameter of TcpIp_GetRemotePhysAddr
 * @param[in]          IpAddrPtr: parameter of TcpIp_GetRemotePhysAddr
 * @param[in]          PhysAddrPtr: parameter of TcpIp_GetRemotePhysAddr
 * @param[in]          initRes: parameter of TcpIp_GetRemotePhysAddr
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateGetRemotePhysAddr(
    uint8                     CtrlIdx,
    const TcpIp_SockAddrType* IpAddrPtr,
    uint8*                    PhysAddrPtr,
    boolean                   initRes);

/**
 * @brief              Development error validation.
 * @param[in]          LocalAddrId: parameter of TcpIp_GetCtrlIdx
 * @param[in]          CtrlIdxPtr: parameter of TcpIp_GetCtrlIdx
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateGetCtrlIdx(TcpIp_LocalAddrIdType LocalAddrId, uint8* CtrlIdxPtr);

/**
 * @brief              Development error validation.
 * @param[in]          ctrlIdx: parameter of TcpIp_Bind
 * @param[in]          numberOfElements: parameter of TcpIp_GetArpCacheEntries
 * @param[in]          entryListPtr: parameter of TcpIp_GetArpCacheEntries
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_ValidateGetArpCacheEntries(uint8 ctrlIdx, uint32* numberOfElements, TcpIp_ArpCacheEntryType* entryListPtr);

/**
 * @brief              Development error validation.
 * @param[in]          ctrlIdx: parameter of TcpIp_GetNdpCacheEntries
 * @param[in]          numberOfElements: parameter of TcpIp_GetNdpCacheEntries
 * @param[in]          entryListPtr: parameter of TcpIp_GetNdpCacheEntries
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_ValidateGetNdpCacheEntries(uint8 ctrlIdx, uint32* numberOfElements, TcpIp_NdpCacheEntryType* entryListPtr);

/**
 * @brief              Development error validation.
 * @param[in]          MeasurementIdx: parameter of TcpIp_GetAndResetMeasurementData
 * @param[in]          MeasurementResetNeeded: parameter of TcpIp_GetAndResetMeasurementData
 * @param[in]          MeasurementDataPtr: parameter of TcpIp_GetAndResetMeasurementData
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateGetAndResetMeasurementData(
    TcpIp_MeasurementIdxType MeasurementIdx,
    boolean                  MeasurementResetNeeded,
    uint32*                  MeasurementDataPtr);

/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_IsConnectionReady
 * @param[in]          RemoteAddrPtr: parameter of TcpIp_IsConnectionReady
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType
    TcpIp_ValidateIsConnectionReady(TcpIp_SocketIdType SocketId, const TcpIp_SockAddrType* RemoteAddrPtr);

#if (STD_ON == TCPIP_UDP_ENABLED)
/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_UdpTransmit
 * @param[in]          DataPtr: parameter of TcpIp_UdpTransmit
 * @param[in]          RemoteAddrPtr: parameter of TcpIp_UdpTransmit
 * @param[in]          TotalLength: parameter of TcpIp_UdpTransmit
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateUdpTransmit(
    TcpIp_SocketIdType        SocketId,
    const uint8*              DataPtr,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    uint16                    TotalLength);
#endif

#if (STD_ON == TCPIP_TCP_ENABLED)
/**
 * @brief              Development error validation.
 * @param[in]          SocketId: parameter of TcpIp_TcpTransmit
 * @param[in]          DataPtr: parameter of TcpIp_TcpTransmit
 * @param[in]          AvailableLength: parameter of TcpIp_TcpTransmit
 * @param[in]          ForceRetrieve: parameter of TcpIp_TcpTransmit
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateTcpTransmit(
    TcpIp_SocketIdType SocketId,
    const uint8*       DataPtr,
    uint32             AvailableLength,
    boolean            ForceRetrieve);
#endif

/**
 * @brief              Development error validation.
 * @param[in]          CtrlIdx: parameter of TcpIp_RxIndication
 * @param[in]          FrameType: parameter of TcpIp_RxIndication
 * @param[in]          IsBroadcast: parameter of TcpIp_RxIndication
 * @param[in]          PhysAddrPtr: parameter of TcpIp_RxIndication
 * @param[in]          DataPtr: parameter of TcpIp_RxIndication
 * @param[in]          LenByte: parameter of TcpIp_RxIndication
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateRxIndication(
    uint8         CtrlIdx,
    Eth_FrameType FrameType,
    boolean       IsBroadcast,
    const uint8*  PhysAddrPtr,
    const uint8*  DataPtr,
    uint16        LenByte);

/**
 * @brief              Development error validation.
 * @param[in]          Domain: parameter of TcpIp_GetSocket
 * @param[in]          Protocol: parameter of TcpIp_GetSocket
 * @param[in]          socketOwnerId: parameter of TcpIp_GetSocket
 * @param[in]          SocketIdPtr: parameter of TcpIp_GetSocket
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
extern Std_ReturnType TcpIp_ValidateGetSocket(
    TcpIp_DomainType   Domain,
    TcpIp_ProtocolType Protocol,
#if (1u < TCPIP_SOCKETOWNER_NUMBER)
    uint8 socketOwnerId,
#endif
    TcpIp_SocketIdType* SocketIdPtr);

#endif

#ifdef __cplusplus
}
#endif

#endif
