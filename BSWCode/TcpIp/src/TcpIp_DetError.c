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
 **  @file               : TcpIp_DetError.c
 **  @author             : darren.zhang
 **  @date               : 2018/12/03
 **  @vendor             : isoft
 **  @description        : TcpIp source file for DET error detections
 **
 ***********************************************************************************************************************/
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* PRQA S 3120 EOF */ /* VL_QAC_MagicNum */
/* PRQA S 3673 EOF */ /* VL_QAC_3673 */
/* PRQA S 1505 EOF */ /* VL_TcpIp_1505 */
/* PRQA S 6030 EOF */ /* VL_MTR_TcpIp_STMIF */
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

#if (STD_ON == TCPIP_DEV_ERROR_DETECT)
/* ===================================================== macros ===================================================== */
/* PRQA S 3472 ++ */ /* VL_TcpIp_3472 */
#define TcpIp_ValidateCtrlId(ctrlId) \
    ((TcpIp_PbCfgPtr->EthIfCtrlId2IndexNum <= (ctrlId)) || (TcpIp_PbCfgPtr->CtrlNum <= TCPIP_CTRL_INDEX(ctrlId)))

#define TcpIp_ValidateLocalAddrId(localAddrId) \
    ((TCPIP_LOCALADDR_NUMBER <= (localAddrId)) || (TcpIp_PbCfgPtr->LocalAddrNum <= TcpIp_LocalAddrId2Var(localAddrId)))
/* PRQA S 3472 -- */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
/**
 * @brief              Development error validation for local address.
 * @param[in]          apiId: service ID of API
 * @param[in]          partitionIndex: index of partition
 * @param[in]          localAddrId: ID of local address
 * @return             Std_ReturnType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              CPD-PLACEHOLDER
 */
TCPIP_LOCAL Std_ReturnType
    TcpIp_ValidateLocalAddr(uint8 apiId, ApplicationType partitionIndex, TcpIp_LocalAddrIdType localAddrId);
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateInit(const TcpIp_ConfigType* ConfigPtr)
{
    Std_ReturnType ret     = E_NOT_OK;
    uint8          apiId   = TCPIP_SID_INIT;
    uint8          errorId = TCPIP_E_NO_ERROR;

    if (NULL_PTR == ConfigPtr)
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else
    {
        ApplicationType partitionIndex = TcpIp_GetCurrentPartition();

        if (TCPIP_PARTITION_NUMBER <= partitionIndex)
        {
            errorId = TCPIP_E_INV_PARTITION;
        }
        else
        {
            ret = E_OK;
        }
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#if (STD_ON == TCPIP_VERSION_INFO_API)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateGetVersionInfo(Std_VersionInfoType* versioninfo)
{
    Std_ReturnType ret   = E_NOT_OK;
    uint8          apiId = TCPIP_SID_GETVERSIONINFO;

    if (NULL_PTR == versioninfo)
    {
        TcpIp_DetReportError(apiId, TCPIP_E_PARAM_POINTER);
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateClose(TcpIp_SocketIdType SocketId, boolean Abort)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_CLOSE;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(Abort);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateSocket(apiId, partitionIndex, SocketId, TCPIP_UNUSED_UINT8))
    {
        /* do nothing */
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateBind(TcpIp_SocketIdType SocketId, TcpIp_LocalAddrIdType LocalAddrId, uint16* PortPtr)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_BIND;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateSocket(apiId, partitionIndex, SocketId, TCPIP_UNUSED_UINT8))
    {
        /* do nothing */
    }
    else if ((TCPIP_LOCALADDRID_ANY != LocalAddrId) && TcpIp_ValidateLocalAddrId(LocalAddrId))
    {
        errorId = TCPIP_E_ADDRNOTAVAIL;
    }
    else if (NULL_PTR == PortPtr)
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#if (STD_ON == TCPIP_TCP_ENABLED)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateTcpConnect(TcpIp_SocketIdType SocketId, const TcpIp_SockAddrType* RemoteAddrPtr)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_TCPCONNECT;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateSocket(apiId, partitionIndex, SocketId, TCPIP_IPPROTO_TCP))
    {
        /* do nothing */
    }
    else if (NULL_PTR == RemoteAddrPtr)
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateTcpListen(TcpIp_SocketIdType SocketId, uint16 MaxChannels)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_TCPLISTEN;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(MaxChannels);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateSocket(apiId, partitionIndex, SocketId, TCPIP_IPPROTO_TCP))
    {
        /* do nothing */
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateTcpReceived(TcpIp_SocketIdType SocketId, uint32 Length)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_TCPRECEIVED;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(Length);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateSocket(apiId, partitionIndex, SocketId, TCPIP_IPPROTO_TCP))
    {
        /* do nothing */
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateRequestComMode(uint8 CtrlIdx, TcpIp_StateType State)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_REQUESTCOMMODE;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (TcpIp_ValidateCtrlId(CtrlIdx))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else if ((TCPIP_STATE_STARTUP == State) || (TCPIP_STATE_SHUTDOWN == State))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateRequestIpAddrAssignment(
    TcpIp_LocalAddrIdType      LocalAddrId,
    TcpIp_IpAddrAssignmentType Type,
    const TcpIp_SockAddrType*  LocalIpAddrPtr,
    uint8                      Netmask,
    const TcpIp_SockAddrType*  DefaultRouterPtr)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_REQUESTIPADDRASSIGNMENT;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(Type);

    if (TcpIp_ValidatePartition(apiId, partitionIndex) != E_OK)
    {
        /* do nothing */
    }
    else if (TcpIp_ValidateLocalAddr(apiId, partitionIndex, LocalAddrId) != E_OK)
    {
        /* do nothing */
    }
    else if (LocalIpAddrPtr != NULL_PTR)
    {
        TcpIp_LocalAddrIdType      localAddrId     = TcpIp_LocalAddrId2Var(LocalAddrId);
        const TcpIp_LocalAddrType* localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);
        TcpIp_DomainType           domain          = LocalIpAddrPtr->domain;
        ip_addr_t                  localIp;

        ip_addr_set_ip4_u32_val(localIp, LocalIpAddrPtr->addr[0U]);

        if ((domain != localAddrCfgPtr->DomainType) || ip_addr_isany_val(localIp))
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else if (((domain == TCPIP_AF_INET) && (Netmask > 32U)) || (domain == TCPIP_AF_INET6))
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else if ((DefaultRouterPtr != NULL_PTR) && (DefaultRouterPtr->domain != domain))
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else
        {
            ret = E_OK;
        }
    }
    else if ((Netmask != 0U) || (DefaultRouterPtr != NULL_PTR))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else
    {
        ret = E_OK;
    }

    if (errorId != TCPIP_E_NO_ERROR)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateReleaseIpAddrAssignment(TcpIp_LocalAddrIdType LocalAddrId)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_RELEASEIPADDRASSIGNMENT;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateLocalAddr(apiId, partitionIndex, LocalAddrId))
    {
        /* do nothing */
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#if ((STD_ON == TCPIP_DHCP_CLIENT_ENABLED) && (STD_ON == TCPIP_RESET_IP_ASSIGNMENT_API))
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateResetIpAssignment(void)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_RESETIPASSIGNMENT;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else
    {
        ret = E_OK;
    }

    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#if (TCPIP_ICMP_ENABLED == STD_ON)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateIcmpTransmit(
    TcpIp_LocalAddrIdType     LocalIpAddrId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    uint8                     Ttl,
    uint8                     Type,
    uint8                     Code,
    uint16                    DataLength,
    const uint8*              DataPtr)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_ICMPTRANSMIT;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(Ttl);
    TCPIP_UNUSED_ARG(Type);
    TCPIP_UNUSED_ARG(Code);

    if (TcpIp_ValidatePartition(apiId, partitionIndex) != E_OK)
    {
        /* do nothing */
    }
    else if (TcpIp_ValidateLocalAddr(apiId, partitionIndex, LocalIpAddrId) != E_OK)
    {
        /* do nothing */
    }
    else if ((RemoteAddrPtr == NULL_PTR) || (RemoteAddrPtr->domain != TCPIP_AF_INET))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else if (((DataLength == 0U) && (DataPtr != NULL_PTR)) || ((DataLength > 0U) && (DataPtr == NULL_PTR)))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else
    {
        TcpIp_LocalAddrIdType      localAddrId     = TcpIp_LocalAddrId2Var(LocalIpAddrId);
        const TcpIp_LocalAddrType* localAddrCfgPtr = &TCPIP_LOCALADDR_CONFIG(localAddrId);

        if (localAddrCfgPtr->DomainType != TCPIP_AF_INET)
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else if (localAddrCfgPtr->AddrType != TCPIP_ADDRESS_UNICAST)
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else
        {
            ret = E_OK;
        }
    }

    if (errorId != TCPIP_E_NO_ERROR)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateChangeParameter(
    TcpIp_SocketIdType SocketId,
    TcpIp_ParamIdType  ParameterId,
    const uint8*       ParameterValue)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_CHANGEPARAMETER;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(ParameterId);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateSocket(apiId, partitionIndex, SocketId, TCPIP_UNUSED_UINT8))
    {
        /* do nothing */
    }
    else if (NULL_PTR == ParameterValue)
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateGetIpAddr(
    TcpIp_LocalAddrIdType LocalAddrId,
    TcpIp_SockAddrType*   IpAddrPtr,
    uint8*                NetmaskPtr,
    TcpIp_SockAddrType*   DefaultRouterPtr)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_GETIPADDR;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateLocalAddr(apiId, partitionIndex, LocalAddrId))
    {
        /* do nothing */
    }
    else if ((NULL_PTR == IpAddrPtr) || (NULL_PTR == NetmaskPtr) || (NULL_PTR == DefaultRouterPtr))
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else if ((TCPIP_AF_INET != IpAddrPtr->domain) && (TCPIP_AF_INET6 != IpAddrPtr->domain))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else if (IpAddrPtr->domain != DefaultRouterPtr->domain)
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateGetPhysAddr(TcpIp_LocalAddrIdType LocalAddrId, uint8* PhysAddrPtr)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_GETPHYSADDR;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateLocalAddr(apiId, partitionIndex, LocalAddrId))
    {
        /* do nothing */
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateGetRemotePhysAddr(
    uint8                     CtrlIdx,
    const TcpIp_SockAddrType* IpAddrPtr,
    uint8*                    PhysAddrPtr,
    boolean                   initRes)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_GETREMOTEPHYSADDR;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(initRes);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (TcpIp_ValidateCtrlId(CtrlIdx))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else if ((NULL_PTR == IpAddrPtr) || (NULL_PTR == PhysAddrPtr))
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#if TCPIP_UNUSED_FUNCTION
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateGetCtrlIdx(TcpIp_LocalAddrIdType LocalAddrId, uint8* CtrlIdxPtr)
{
    TCPIP_UNUSED_ARG(LocalAddrId);
    TCPIP_UNUSED_ARG(CtrlIdxPtr);

    return E_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType
    TcpIp_ValidateGetArpCacheEntries(uint8 ctrlIdx, uint32* numberOfElements, TcpIp_ArpCacheEntryType* entryListPtr)
{
    TCPIP_UNUSED_ARG(ctrlIdx);
    TCPIP_UNUSED_ARG(numberOfElements);
    TCPIP_UNUSED_ARG(entryListPtr);

    return E_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType
    TcpIp_ValidateGetNdpCacheEntries(uint8 ctrlIdx, uint32* numberOfElements, TcpIp_NdpCacheEntryType* entryListPtr)
{
    TCPIP_UNUSED_ARG(ctrlIdx);
    TCPIP_UNUSED_ARG(numberOfElements);
    TCPIP_UNUSED_ARG(entryListPtr);

    return E_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateGetAndResetMeasurementData(
    TcpIp_MeasurementIdxType MeasurementIdx,
    boolean                  MeasurementResetNeeded,
    uint32*                  MeasurementDataPtr)
{
    TCPIP_UNUSED_ARG(MeasurementIdx);
    TCPIP_UNUSED_ARG(MeasurementResetNeeded);
    TCPIP_UNUSED_ARG(MeasurementDataPtr);

    return E_OK;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateIsConnectionReady(TcpIp_SocketIdType SocketId, const TcpIp_SockAddrType* RemoteAddrPtr)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_ISCONNECTIONREADY;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else
    {
        TcpIp_SocketIdType socketIndex = TCPIP_SOCKET_INDEX(partitionIndex, SocketId);

        if (TCPIP_SOCKET_NUM <= socketIndex)
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else if (NULL_PTR == RemoteAddrPtr)
        {
            errorId = TCPIP_E_PARAM_POINTER;
        }
        else
        {
            ret = E_OK;
        }
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#if (STD_ON == TCPIP_UDP_ENABLED)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateUdpTransmit(
    TcpIp_SocketIdType        SocketId,
    const uint8*              DataPtr,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    uint16                    TotalLength)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_UDPTRANSMIT;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(DataPtr);
    TCPIP_UNUSED_ARG(TotalLength);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateSocket(apiId, partitionIndex, SocketId, TCPIP_IPPROTO_UDP))
    {
        /* do nothing */
    }
    else if (NULL_PTR == RemoteAddrPtr)
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#if (STD_ON == TCPIP_TCP_ENABLED)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateTcpTransmit(
    TcpIp_SocketIdType SocketId,
    const uint8*       DataPtr,
    uint32             AvailableLength,
    boolean            ForceRetrieve)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_TCPTRANSMIT;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(DataPtr);
    TCPIP_UNUSED_ARG(ForceRetrieve);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (E_OK != TcpIp_ValidateSocket(apiId, partitionIndex, SocketId, TCPIP_IPPROTO_TCP))
    {
        /* do nothing */
    }
    else if ((uint32)TCPIP_UNUSED_UINT16 < AvailableLength)
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateRxIndication(
    uint8         CtrlIdx,
    Eth_FrameType FrameType,
    boolean       IsBroadcast,
    const uint8*  PhysAddrPtr,
    const uint8*  DataPtr,
    uint16        LenByte)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_RXINDICATION;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(FrameType);
    TCPIP_UNUSED_ARG(IsBroadcast);
    TCPIP_UNUSED_ARG(PhysAddrPtr);
    TCPIP_UNUSED_ARG(DataPtr);
    TCPIP_UNUSED_ARG(LenByte);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if (TcpIp_ValidateCtrlId(CtrlIdx))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateGetSocket(
    TcpIp_DomainType    Domain,
    TcpIp_ProtocolType  Protocol,
    uint8               SocketOwnerId,
    TcpIp_SocketIdType* SocketIdPtr)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_UP_GETSOCKET;
    uint8           errorId        = TCPIP_E_NO_ERROR;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

    TCPIP_UNUSED_ARG(Protocol);

    if (E_OK != TcpIp_ValidatePartition(apiId, partitionIndex))
    {
        /* do nothing */
    }
    else if ((TCPIP_AF_INET != Domain) && (TCPIP_AF_INET6 != Domain))
    {
        errorId = TCPIP_E_AFNOSUPPORT;
    }
    else if (TCPIP_SOCKETOWNER_NUMBER <= SocketOwnerId)
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else if (NULL_PTR == SocketIdPtr)
    {
        errorId = TCPIP_E_PARAM_POINTER;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidatePartition(uint8 apiId, ApplicationType partitionIndex)
{
    Std_ReturnType ret     = E_NOT_OK;
    uint8          errorId = TCPIP_E_NO_ERROR;

    if (TcpIp_PbCfgPtr == NULL_PTR)
    {
        errorId = TCPIP_E_UNINIT;
    }
    else if (partitionIndex >= TCPIP_PARTITION_NUMBER)
    {
        errorId = TCPIP_E_INV_PARTITION;
    }
    else
    {
        const TcpIp_RuntimeType* runtimePtr = TCPIP_RUNTIME(partitionIndex);

        if ((runtimePtr == NULL_PTR) || (runtimePtr->InitedState != TCPIP_STATE_INIT))
        {
            errorId = TCPIP_E_UNINIT;
        }
        else
        {
            ret = E_OK;
        }
    }

    if (errorId != TCPIP_E_NO_ERROR)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_ValidateSocket(
    uint8              apiId,
    ApplicationType    partitionIndex,
    TcpIp_SocketIdType socketId,
    TcpIp_ProtocolType protocol)
{
    Std_ReturnType     ret         = E_NOT_OK;
    uint8              errorId     = TCPIP_E_NO_ERROR;
    TcpIp_SocketIdType socketIndex = TCPIP_SOCKET_INDEX(partitionIndex, socketId);

    if (socketIndex >= TCPIP_SOCKET_NUM)
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else
    {
        const TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketIndex);

        if (socketRtPtr->PcbPtr == NULL_PTR)
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else if (
            ((protocol != TCPIP_UNUSED_UINT8) && (socketRtPtr->Protocol != protocol))
            || ((protocol == TCPIP_UNUSED_UINT8) && (socketRtPtr->Protocol != TCPIP_IPPROTO_TCP)
                && (socketRtPtr->Protocol != TCPIP_IPPROTO_UDP)))
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else if ((socketRtPtr->NextFreeIndex != TCPIP_SOCKET_NUM) || (socketRtPtr->OwnerCfgPtr == NULL_PTR))
        {
            errorId = TCPIP_E_INV_ARG;
        }
        else
        {
            ret = E_OK;
        }
    }

    if (errorId != TCPIP_E_NO_ERROR)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
TCPIP_LOCAL Std_ReturnType
    TcpIp_ValidateLocalAddr(uint8 apiId, ApplicationType partitionIndex, TcpIp_LocalAddrIdType localAddrId)
{
    Std_ReturnType ret     = E_NOT_OK;
    uint8          errorId = TCPIP_E_NO_ERROR;

    TCPIP_UNUSED_ARG(partitionIndex);

    if (TcpIp_ValidateLocalAddrId(localAddrId))
    {
        errorId = TCPIP_E_INV_ARG;
    }
    else
    {
        ret = E_OK;
    }

    if (TCPIP_E_NO_ERROR != errorId)
    {
        TcpIp_DetReportError(apiId, errorId);
    }
    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#endif
