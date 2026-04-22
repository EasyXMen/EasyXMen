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
 **  @file               : TcpIp.c
 **  @author             : darren.zhang
 **  @date               : 2018/12/03
 **  @vendor             : isoft
 **  @description        : TcpIp source file for external API implementations
 **
 ***********************************************************************************************************************/
/* PRQA S 1503 EOF */ /* VL_QAC_NoUsedApi */
/* PRQA S 1505 EOF */ /* VL_TcpIp_1505 */
/* PRQA S 1501 EOF */ /* VL_TcpIp_1501 */
/* PRQA S 1753 EOF */ /* VL_TcpIp_1753 */
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* PRQA S 6510 EOF */ /* VL_MTR_TcpIp_CONF */
/* PRQA S 6040 EOF */ /* VL_MTR_TcpIp_STPAR */
/* =================================================== inclusions =================================================== */
#include "Os_Types.h"
#include "ComStack_Types.h"
#include "TcpIp_Cfg.h"
#include "TcpIp_PBcfg.h"
#include "TcpIp_Types.h"
#include "TcpIp_CfgTypes.h"
#include "TcpIp.h"
#include "TcpIp_Internal.h"
#include "TcpIp_DetError.h"

#include "IStdLib.h"
#include "EthIf.h"

/* ===================================================== macros ===================================================== */
#if ((STD_ON != TCPIP_TCP_ENABLED) && (STD_ON != TCPIP_UDP_ENABLED))
#define TcpIp_InnerChangeParameterFramePrio NULL_PTR
#define TcpIp_InnerChangeParameterTtl       NULL_PTR
#endif
#if (STD_ON != TCPIP_TCP_ENABLED)
#define TcpIp_InnerChangeParameterTcpNagle     NULL_PTR
#define TcpIp_InnerChangeParameterTcpKeepAlive NULL_PTR
#endif
#if (STD_ON != TCPIP_TCP_TLS_ENABLED)
#define TcpIp_TlsDynamicConnect NULL_PTR
#endif

/* ================================================ type definitions ================================================ */
typedef Std_ReturnType (*TcpIp_InnerChangeParameterFuncType)(
    TcpIp_SocketRuntimeType* socketRtPtr,
    TcpIp_ParamIdType        paramId,
    const uint8*             paramValue);

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_TcpIp_3218 */
/**
 * @brief TcpIp_ChangeParameter() implementation table
 */
TCPIP_LOCAL const TcpIp_InnerChangeParameterFuncType TcpIp_ChangeParameterHandles[] = {
    NULL_PTR,                               /* TCPIP_PARAMID_TCP_RXWND_MAX */
    TcpIp_InnerChangeParameterFramePrio,    /* TCPIP_PARAMID_FRAMEPRIO */
    TcpIp_InnerChangeParameterTcpNagle,     /* TCPIP_PARAMID_TCP_NAGLE */
    TcpIp_InnerChangeParameterTcpKeepAlive, /* TCPIP_PARAMID_TCP_KEEPALIVE */
    TcpIp_InnerChangeParameterTtl,          /* TCPIP_PARAMID_TTL */
    TcpIp_InnerChangeParameterTcpKeepAlive, /* TCPIP_PARAMID_TCP_KEEPALIVE_TIME */
    TcpIp_InnerChangeParameterTcpKeepAlive, /* TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX */
    TcpIp_InnerChangeParameterTcpKeepAlive, /* TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL */
    NULL_PTR,                               /* TCPIP_PARAMID_TCP_OPTIONFILTER */
    NULL_PTR,                               /* TCPIP_PARAMID_PATHMTU_ENABLE */
    NULL_PTR,                               /* TCPIP_PARAMID_FLOWLABEL */
    NULL_PTR,                               /* TCPIP_PARAMID_DSCP */
    NULL_PTR,                               /* TCPIP_PARAMID_UDP_CHECKSUM */
    TcpIp_TlsDynamicConnect,                /* TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT */
                                            /* TCPIP_PARAMID_TCP_RETRANSMIT_TIMEOUT */
                                            /* TCPIP_PARAMID_TCP_MAXRTX */
                                            /* TCPIP_PARAMID_TCP_MAX_RETRANSMIT_TIMEOUT */
};
/* PRQA S 3218 -- */
#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"

void TcpIp_Init(const TcpIp_ConfigType* ConfigPtr)
{
#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateInit(ConfigPtr))
#endif
    {
        ApplicationType    partitionIndex = TcpIp_GetCurrentPartition();
        TcpIp_RuntimeType* runtimePtr     = TCPIP_RUNTIME(partitionIndex);

        TcpIp_PbCfgPtr       = ConfigPtr;
        TcpIp_MulticoreIndex = (uint8)partitionIndex;

        if (TCPIP_STATE_UNINIT == runtimePtr->InitedState)
        {
            (void)IStdLib_MemSet(runtimePtr, 0u, sizeof(TcpIp_RuntimeType));
            runtimePtr->InitedState = TCPIP_STATE_INIT;

            lwip_init();

            TcpIp_InitCtrlRuntime(partitionIndex);
#if (0u < TCPIP_SOCKET_NUM)
            TcpIp_InitSocketRuntime(partitionIndex);
#endif
            TcpIp_InitPeriodTimer(partitionIndex);
#if (STD_ON == TCPIP_TCP_TLS_ENABLED)
            TcpIp_TlsInit(TcpIp_PbCfgPtr->TlsCfgPtr);
#endif
        }
    }
}

#if (STD_ON == TCPIP_VERSION_INFO_API)
void TcpIp_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateGetVersionInfo(versioninfo))
#endif
    {
        versioninfo->moduleID         = TCPIP_MODULE_ID;
        versioninfo->vendorID         = TCPIP_VENDOR_ID;
        versioninfo->sw_major_version = TCPIP_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = TCPIP_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = TCPIP_SW_PATCH_VERSION;
    }
}
#endif

Std_ReturnType TcpIp_Close(TcpIp_SocketIdType SocketId, boolean Abort)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateClose(SocketId, Abort))
#endif
    {
        ApplicationType          partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_SocketIdType       socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
        TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);

        ret = TcpIp_InnerClose(socketRtPtr, Abort);
    }

    return ret;
}

Std_ReturnType TcpIp_Bind(TcpIp_SocketIdType SocketId, TcpIp_LocalAddrIdType LocalAddrId, uint16* PortPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateBind(SocketId, LocalAddrId, PortPtr))
#endif
    {
        ApplicationType          partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_SocketIdType       socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
        TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);
        TcpIp_LocalAddrIdType    localAddrId    = TcpIp_LocalAddrId2Var(LocalAddrId);

        ret = TcpIp_InnerBind(socketRtPtr, localAddrId, PortPtr);
    }

    return ret;
}

#if (STD_ON == TCPIP_TCP_ENABLED)
Std_ReturnType TcpIp_TcpConnect(TcpIp_SocketIdType SocketId, const TcpIp_SockAddrType* RemoteAddrPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateTcpConnect(SocketId, RemoteAddrPtr))
#endif
    {
        ApplicationType                partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_SocketIdType             socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
        const TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);

        ret = TcpIp_InnerTcpConnect(socketRtPtr, RemoteAddrPtr);
    }

    return ret;
}

Std_ReturnType TcpIp_TcpListen(TcpIp_SocketIdType SocketId, uint16 MaxChannels)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateTcpListen(SocketId, MaxChannels))
#endif
    {
        ApplicationType          partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_SocketIdType       socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
        TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);
        uint8 maxChannNum = (MaxChannels > (uint16)TCPIP_UNUSED_UINT8) ? TCPIP_UNUSED_UINT8 : (uint8)MaxChannels;

        ret = TcpIp_InnerTcpListen(socketRtPtr, maxChannNum);
    }

    return ret;
}

Std_ReturnType TcpIp_TcpReceived(TcpIp_SocketIdType SocketId, uint32 Length)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateTcpReceived(SocketId, Length))
#endif
    {
        ApplicationType                partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_SocketIdType             socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
        const TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);
        uint16 recvLen = (Length > (uint32)TCPIP_UNUSED_UINT16) ? TCPIP_UNUSED_UINT16 : (uint16)Length;

        ret = TcpIp_InnerTcpReceived(socketRtPtr, recvLen);
    }

    return ret;
}
#endif

Std_ReturnType TcpIp_RequestComMode(uint8 CtrlIdx, TcpIp_StateType State)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateRequestComMode(CtrlIdx, State))
#endif
    {
        uint8                  ctrlIndex  = TCPIP_CTRL_INDEX(CtrlIdx);
        const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
        TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

        switch (ctrlRtPtr->CtrlStatus)
        {
        case TCPIP_STATE_ONLINE:
            ret = TcpIp_InnerRequestComModeOnline(ctrlCfgPtr, ctrlRtPtr, State);
            break;
        case TCPIP_STATE_ONHOLD:
            ret = TcpIp_InnerRequestComModeOnhold(ctrlCfgPtr, ctrlRtPtr, State);
            break;
        case TCPIP_STATE_OFFLINE:
            ret = TcpIp_InnerRequestComModeOffline(ctrlCfgPtr, ctrlRtPtr, State);
            break;
        case TCPIP_STATE_STARTUP:
            ret = TcpIp_InnerRequestComModeStartup(ctrlCfgPtr, ctrlRtPtr, State);
            break;
        default:
            /* do nothing */
            break;
        }
    }

    return ret;
}

Std_ReturnType TcpIp_RequestIpAddrAssignment(
    TcpIp_LocalAddrIdType      LocalAddrId,
    TcpIp_IpAddrAssignmentType Type,
    const TcpIp_SockAddrType*  LocalIpAddrPtr,
    uint8                      Netmask,
    const TcpIp_SockAddrType*  DefaultRouterPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateRequestIpAddrAssignment(LocalAddrId, Type, LocalIpAddrPtr, Netmask, DefaultRouterPtr))
#endif
    {
        TcpIp_LocalAddrIdType      localAddrId     = TcpIp_LocalAddrId2Var(LocalAddrId);
        const TcpIp_LocalAddrType* localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
        uint8                      ctrlIndex       = localAddrCfgPtr->CtrlIndex;
        const TcpIp_CtrlType*      ctrlCfgPtr      = &TCPIP_CTRL_CONFIG(ctrlIndex);
        TcpIp_CtrlRuntimeType*     ctrlRtPtr       = ctrlCfgPtr->RuntimePtr;

        if (TCPIP_STATE_OFFLINE != ctrlRtPtr->CtrlStatus)
        {
            ret = TcpIp_InnerRequestIpAddrAssignment(
                ctrlCfgPtr,
                ctrlRtPtr,
                localAddrCfgPtr,
                Type,
                LocalIpAddrPtr,
                Netmask,
                DefaultRouterPtr);
        }
    }

    return ret;
}

Std_ReturnType TcpIp_ReleaseIpAddrAssignment(TcpIp_LocalAddrIdType LocalAddrId)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateReleaseIpAddrAssignment(LocalAddrId))
#endif
    {
        TcpIp_LocalAddrIdType      localAddrId     = TcpIp_LocalAddrId2Var(LocalAddrId);
        const TcpIp_LocalAddrType* localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
        uint8                      ctrlIndex       = localAddrCfgPtr->CtrlIndex;
        const TcpIp_CtrlType*      ctrlCfgPtr      = &TCPIP_CTRL_CONFIG(ctrlIndex);
        TcpIp_CtrlRuntimeType*     ctrlRtPtr       = ctrlCfgPtr->RuntimePtr;

        if (TCPIP_STATE_OFFLINE != ctrlRtPtr->CtrlStatus)
        {
            ret = TcpIp_InnerReleaseIpAddrAssignment(ctrlCfgPtr, ctrlRtPtr, localAddrCfgPtr);
        }
    }

    return ret;
}

#if ((STD_ON == TCPIP_DHCP_CLIENT_ENABLED) && (STD_ON == TCPIP_RESET_IP_ASSIGNMENT_API))
Std_ReturnType TcpIp_ResetIpAssignment(void)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateResetIpAssignment())
#endif
    {
    }

    return ret;
}
#endif

#if (STD_ON == TCPIP_ICMP_ENABLED)
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
Std_ReturnType TcpIp_IcmpTransmit(
    TcpIp_LocalAddrIdType     LocalIpAddrId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    uint8                     Ttl,
    uint8                     Type,
    uint8                     Code,
    uint16                    DataLength,
    const uint8*              DataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateIcmpTransmit(LocalIpAddrId, RemoteAddrPtr, Ttl, Type, Code, DataLength, DataPtr))
#endif
    {
        TcpIp_LocalAddrIdType   localAddrId = TcpIp_LocalAddrId2Var(LocalIpAddrId);
        TcpIp_IcmpTransDataType icmpParam   = {0};

        icmpParam.Ttl        = Ttl;
        icmpParam.Type       = Type;
        icmpParam.Code       = Code;
        icmpParam.DataLength = DataLength;
        icmpParam.DataPtr    = DataPtr;
        TCPIP_LOCAL_TO_LWIP_IPADDR(icmpParam.RemoteIp.addr, RemoteAddrPtr->addr); /* PRQA S 3473 */ /* VL_TcpIp_3473 */

        ret = TcpIp_InnerIcmpTransmit(localAddrId, &icmpParam);
    }

    return ret;
}
#endif

#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
Std_ReturnType TcpIp_IcmpV6Transmit(
    TcpIp_LocalAddrIdType     LocalIpAddrId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    uint8                     HopLimit,
    uint8                     Type,
    uint8                     Code,
    uint16                    DataLength,
    const uint8*              DataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateIcmpV6Transmit(LocalIpAddrId, RemoteAddrPtr, HopLimit, Type, Code, DataLength, DataPtr))
#endif
    {
    }

    return ret;
}
#endif
#endif

#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
Std_ReturnType
    TcpIp_DhcpReadOption(TcpIp_LocalAddrIdType LocalIpAddrId, uint8 Option, uint8* DataLength, uint8* DataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateDhcpReadOption(LocalIpAddrId, Option, DataLength, DataPtr))
#endif
    {
    }

    return ret;
}
#endif

#if TCPIP_UNUSED_FUNCTION
#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
Std_ReturnType
    TcpIp_DhcpV6ReadOption(TcpIp_LocalAddrIdType LocalIpAddrId, uint16 Option, uint16* DataLength, uint8* DataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateDhcpV6ReadOption(LocalIpAddrId, Option, DataLength, DataPtr))
#endif
    {
    }

    return ret;
}
#endif
#endif

#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
Std_ReturnType
    TcpIp_DhcpWriteOption(TcpIp_LocalAddrIdType LocalIpAddrId, uint8 Option, uint8 DataLength, const uint8* DataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateDhcpWriteOption(LocalIpAddrId, Option, DataLength, DataPtr))
#endif
    {
    }

    return ret;
}
#endif

#if TCPIP_UNUSED_FUNCTION
#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
Std_ReturnType
    TcpIp_DhcpV6WriteOption(TcpIp_LocalAddrIdType LocalIpAddrId, uint16 Option, uint16 DataLength, const uint8* DataPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateDhcpV6WriteOption(LocalIpAddrId, Option, DataLength, DataPtr))
#endif
    {
    }

    return ret;
}
#endif
#endif

Std_ReturnType
    TcpIp_ChangeParameter(TcpIp_SocketIdType SocketId, TcpIp_ParamIdType ParameterId, const uint8* ParameterValue)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateChangeParameter(SocketId, ParameterId, ParameterValue))
#endif
    {
        ApplicationType          partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_SocketIdType       socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
        TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);
        size_t limitLen = sizeof(TcpIp_ChangeParameterHandles) / sizeof(TcpIp_ChangeParameterHandles[0]);

        if (ParameterId < limitLen)
        {
            const TcpIp_InnerChangeParameterFuncType changeFuncPtr = TcpIp_ChangeParameterHandles[ParameterId];

            if (NULL_PTR != changeFuncPtr)
            {
                ret = changeFuncPtr(socketRtPtr, ParameterId, ParameterValue);
            }
        }
        else if (TCPIP_PARAMID_VENDOR_SPECIFIC == ParameterId)
        {
            /* do nothing */
        }
#if (STD_ON == TCPIP_SOCKET_VLAN)
        else if (TCPIP_PARAMID_SOCKET_VLAN == ParameterId)
        {
            ret = TcpIp_InnerChangeParameterSocketVlan(socketRtPtr, ParameterId, ParameterValue);
        }
#endif
        else
        {
            /* do nothing */
        }
    }

    return ret;
}

Std_ReturnType TcpIp_GetIpAddr(
    TcpIp_LocalAddrIdType LocalAddrId,
    TcpIp_SockAddrType*   IpAddrPtr,
    uint8*                NetmaskPtr,
    TcpIp_SockAddrType*   DefaultRouterPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateGetIpAddr(LocalAddrId, IpAddrPtr, NetmaskPtr, DefaultRouterPtr))
#endif
    {
        TcpIp_LocalAddrIdType             localAddrId     = TcpIp_LocalAddrId2Var(LocalAddrId);
        const TcpIp_LocalAddrType*        localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
        const TcpIp_LocalAddrRuntimeType* localAddrRtPtr  = localAddrCfgPtr->RuntimePtr;
        TcpIp_SockAddrType                netMaskAdr      = {0};

        TcpIp_EnterExclusiveArea();
        TCPIP_LWIP_TO_LOCAL_IPADDR(IpAddrPtr->addr, localAddrRtPtr->LocalIp.addr);
        TCPIP_LWIP_TO_LOCAL_IPADDR(netMaskAdr.addr, localAddrRtPtr->Netmask.addr);
        TCPIP_LWIP_TO_LOCAL_IPADDR(DefaultRouterPtr->addr, localAddrRtPtr->RouterIp.addr);
        TcpIp_ExitExclusiveArea();

        TcpIp_NetMaskInnerToExt(localAddrCfgPtr->DomainType, netMaskAdr.addr, NetmaskPtr);

        ret = E_OK;
    }

    return ret;
}

Std_ReturnType TcpIp_GetPhysAddr(TcpIp_LocalAddrIdType LocalAddrId, uint8* PhysAddrPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateGetPhysAddr(LocalAddrId, PhysAddrPtr))
#endif
    {
        TcpIp_LocalAddrIdType      localAddrId     = TcpIp_LocalAddrId2Var(LocalAddrId);
        const TcpIp_LocalAddrType* localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);

        EthIf_GetPhysAddr(localAddrCfgPtr->EthIfCtrlId, PhysAddrPtr);

        ret = E_OK;
    }

    return ret;
}

TcpIp_ReturnType
    TcpIp_GetRemotePhysAddr(uint8 CtrlIdx, const TcpIp_SockAddrType* IpAddrPtr, uint8* PhysAddrPtr, boolean initRes)
{
    TcpIp_ReturnType ret = TCPIP_E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateGetRemotePhysAddr(CtrlIdx, IpAddrPtr, PhysAddrPtr, initRes))
#endif
    {
#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        if (TCPIP_AF_INET == IpAddrPtr->domain)
#endif
        {
#if ((TCPIP_SC1 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
            uint8                  ctrlIndex  = TCPIP_CTRL_INDEX(CtrlIdx);
            const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
            TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;

            ret = TcpIp_InnerGetRemotePhysAddrByArp(ctrlRtPtr, IpAddrPtr, PhysAddrPtr, initRes);
#endif
        }
#if (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS)
        else
#endif
        {
#if ((TCPIP_SC2 == TCPIP_SCALABILITY_CLASS) || (TCPIP_SC3 == TCPIP_SCALABILITY_CLASS))
#endif
        }
    }

    return ret;
}

#if TCPIP_UNUSED_FUNCTION
Std_ReturnType TcpIp_GetCtrlIdx(TcpIp_LocalAddrIdType LocalAddrId, uint8* CtrlIdxPtr)
{
    TCPIP_UNUSED_ARG(LocalAddrId);
    TCPIP_UNUSED_ARG(CtrlIdxPtr);

    return E_OK;
}

Std_ReturnType TcpIp_GetArpCacheEntries(uint8 ctrlIdx, uint32* numberOfElements, TcpIp_ArpCacheEntryType* entryListPtr)
{
    TCPIP_UNUSED_ARG(ctrlIdx);
    TCPIP_UNUSED_ARG(numberOfElements);
    TCPIP_UNUSED_ARG(entryListPtr);

    return E_OK;
}

Std_ReturnType TcpIp_GetNdpCacheEntries(uint8 ctrlIdx, uint32* numberOfElements, TcpIp_NdpCacheEntryType* entryListPtr)
{
    TCPIP_UNUSED_ARG(ctrlIdx);
    TCPIP_UNUSED_ARG(numberOfElements);
    TCPIP_UNUSED_ARG(entryListPtr);

    return E_OK;
}

Std_ReturnType TcpIp_GetAndResetMeasurementData(
    TcpIp_MeasurementIdxType MeasurementIdx,
    boolean                  MeasurementResetNeeded,
    uint32*                  MeasurementDataPtr)
{
    TCPIP_UNUSED_ARG(MeasurementIdx);
    TCPIP_UNUSED_ARG(MeasurementResetNeeded);
    TCPIP_UNUSED_ARG(MeasurementDataPtr);

    return E_OK;
}
#endif

TcpIp_ReturnType TcpIp_IsConnectionReady(TcpIp_SocketIdType SocketId, const TcpIp_SockAddrType* RemoteAddrPtr)
{
    TcpIp_ReturnType ret = TCPIP_E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateIsConnectionReady(SocketId, RemoteAddrPtr))
#endif
    {
        ApplicationType                partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_SocketIdType             socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
        const TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);

        ret = TcpIp_InnerIsConnectionReady(socketRtPtr, RemoteAddrPtr);
    }

    return ret;
}

#if (STD_ON == TCPIP_UDP_ENABLED)
Std_ReturnType TcpIp_UdpTransmit(
    TcpIp_SocketIdType        SocketId,
    const uint8*              DataPtr,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    uint16                    TotalLength)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateUdpTransmit(SocketId, DataPtr, RemoteAddrPtr, TotalLength))
#endif
    {
        ApplicationType                partitionIndex = TcpIp_MulticoreIndex;
        TcpIp_SocketIdType             socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
        const TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);

        if ((NULL_PTR != DataPtr) || (0u == TotalLength))
        {
            ret = TcpIp_InnerUdpTransmitDirect(socketRtPtr, DataPtr, RemoteAddrPtr, TotalLength);
        }
        else
        {
            ret = TcpIp_InnerUdpTransmitCopy(socketRtPtr, RemoteAddrPtr, TotalLength);
        }
    }

    return ret;
}
#endif

#if (STD_ON == TCPIP_TCP_ENABLED)
Std_ReturnType
    TcpIp_TcpTransmit(TcpIp_SocketIdType SocketId, const uint8* DataPtr, uint32 AvailableLength, boolean ForceRetrieve)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateTcpTransmit(SocketId, DataPtr, AvailableLength, ForceRetrieve))
#endif
    {
        if (0uL < AvailableLength)
        {
            ApplicationType          partitionIndex = TcpIp_MulticoreIndex;
            TcpIp_SocketIdType       socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);
            TcpIp_SocketRuntimeType* socketRtPtr    = &TCPIP_SOCKET(partitionIndex, socketIndex);
            uint16                   totalLen =
                (AvailableLength > (uint32)TCPIP_UNUSED_UINT16) ? TCPIP_UNUSED_UINT16 : (uint16)AvailableLength;

            if (NULL_PTR != DataPtr)
            {
                /* PRQA S 1253, 1840 ++ */ /* VL_TcpIp_1253, VL_TcpIp_1840 */
                if ((uint16)TCP_SND_BUF >= totalLen)
                /* PRQA S 1253, 1840 -- */
                {
                    ret = TcpIp_InnerTcpTransmitDirect(socketRtPtr, DataPtr, totalLen);
                }
            }
            else
            {
                /* PRQA S 1253, 1840 ++ */ /* VL_TcpIp_1253, VL_TcpIp_1840 */
                if (((uint16)TCP_SND_BUF >= totalLen) || (!ForceRetrieve))
                /* PRQA S 1253, 1840 -- */
                {
                    ret = TcpIp_InnerTcpTransmitCopy(socketRtPtr, totalLen, ForceRetrieve);
                }
            }
        }
    }

    return ret;
}
#endif

void TcpIp_RxIndication(
    uint8         CtrlIdx,
    Eth_FrameType FrameType,
    boolean       IsBroadcast,
    const uint8*  PhysAddrPtr,
    const uint8*  DataPtr,
    uint16        LenByte)
{
    TCPIP_UNUSED_ARG(PhysAddrPtr);

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK == TcpIp_ValidateRxIndication(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte))
#endif
    {
        if ((NULL_PTR != DataPtr) && (0u < LenByte))
        {
            uint8                 ctrlIndex  = TCPIP_CTRL_INDEX(CtrlIdx);
            const TcpIp_CtrlType* ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIndex);
            uint8                 flag       = IsBroadcast ? PBUF_FLAG_LLBCAST : 0u;

            TcpIp_InnerRxIndication(ctrlCfgPtr, FrameType, flag, DataPtr, LenByte);
        }
    }
}

#if (STD_ON == TCPIP_PARTITION_USED)
void TcpIp_MainFunction(ApplicationType partitionIndex)
#else
void TcpIp_MainFunction(void)
#endif
{
#if (STD_ON != TCPIP_PARTITION_USED)
    ApplicationType partitionIndex = 0u;
#endif

    if (NULL_PTR != TcpIp_PbCfgPtr)
    {
        const TcpIp_RuntimeType* runtimePtr = TCPIP_RUNTIME(partitionIndex);

        if (TCPIP_STATE_INIT == runtimePtr->InitedState)
        {
#if defined(TCPIP_SUUPORT_CTRL_MAINHANDLE)
            TcpIp_ControlPollMainHandle(partitionIndex);
#endif
#if (0u < TCPIP_SOCKET_NUM)
            TcpIp_SocketPollMainHandle(partitionIndex);
#endif
            TcpIp_PeriodTimerMainHandle(partitionIndex);
        }
    }
}

Std_ReturnType TcpIp_GetSocket(
    TcpIp_DomainType   Domain,
    TcpIp_ProtocolType Protocol,
#if (1u < TCPIP_SOCKETOWNER_NUMBER)
    uint8 socketOwnerId,
#endif
    TcpIp_SocketIdType* SocketIdPtr)
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
    if (E_OK
        == TcpIp_ValidateGetSocket(
            Domain,
            Protocol,
#if (1u < TCPIP_SOCKETOWNER_NUMBER)
            socketOwnerId,
#endif
            SocketIdPtr))
#endif
    {
#if (1u == TCPIP_SOCKETOWNER_NUMBER)
        uint8 socketOwnerId = 0u;
#endif
        const TcpIp_SocketOwnerType* ownerCfgPtr = &TcpIp_LConfig.SocketOwnerPtr[socketOwnerId];

#if (STD_ON == TCPIP_TCP_ENABLED)
        if (TCPIP_IPPROTO_TCP == Protocol)
        {
            ret = TcpIp_MallocTcpSocket(Domain, ownerCfgPtr, SocketIdPtr);
        }
#endif

#if (STD_ON == TCPIP_UDP_ENABLED)
        if (TCPIP_IPPROTO_UDP == Protocol)
        {
            ret = TcpIp_MallocUdpSocket(Domain, ownerCfgPtr, SocketIdPtr);
        }
#endif
    }

    return ret;
}

/* ========================================== internal function definitions ========================================= */

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
