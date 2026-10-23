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
 **  @file               : TcpIp_TC8.c
 **  @author             : darren.zhang
 **  @date               : 2018/12/03
 **  @vendor             : isoft
 **  @description        : TcpIp source file for TC8 implementations
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_QAC_UnUsedFiles */
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

#include "lwip/opt.h"

#if (TCPIP_PARTITION_USED == STD_ON)
#include "Os.h"
#endif

#if (TCPIP_SUPPORT_TC8_TEST == STD_ON)

/* ===================================================== macros ===================================================== */
#define TCPIP_SID_TCPSHUTDOWN        0xb0
#define TCPIP_SID_GETTCPSOCKETSTATUS 0xb1

#if (TCPIP_TCP_ENABLED == STD_ON)
#define TCPSTATEID_CLOSED      0x00U
#define TCPSTATEID_LISTEN      0x01U
#define TCPSTATEID_SYNSENT     0x02U
#define TCPSTATEID_SYNRECEIVED 0x03U
#define TCPSTATEID_ESTABLISHED 0x04U
#define TCPSTATEID_CLOSEWAIT   0x05U
#define TCPSTATEID_FINWAIT1    0x06U
#define TCPSTATEID_CLOSING     0x07U
#define TCPSTATEID_LASTACK     0x08U
#define TCPSTATEID_FINWAIT2    0x09U
#define TCPSTATEID_TIMEWAIT    0x0AU
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */
#if (TCPIP_TCP_ENABLED == STD_ON)
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"
TCPIP_LOCAL const uint8 tcpStateTable[] = {
    TCPSTATEID_CLOSED,
    TCPSTATEID_LISTEN,
    TCPSTATEID_SYNSENT,
    TCPSTATEID_SYNRECEIVED,
    TCPSTATEID_ESTABLISHED,
    TCPSTATEID_FINWAIT1,
    TCPSTATEID_FINWAIT2,
    TCPSTATEID_CLOSEWAIT,
    TCPSTATEID_CLOSING,
    TCPSTATEID_LASTACK,
    TCPSTATEID_TIMEWAIT,
};
#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"
#endif

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/* ========================================== internal function definitions ========================================= */

#if (TCPIP_TCP_ENABLED == STD_ON)
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_GetTcpSocketState(TcpIp_SocketIdType socketId, uint8* statePtr)
{
    Std_ReturnType     ret            = E_NOT_OK;
    uint8              apiId          = TCPIP_SID_GETTCPSOCKETSTATUS;
    ApplicationType    partitionIndex = TcpIp_MulticoreIndex;
    TcpIp_SocketIdType socketIndex    = TCPIP_SOCKET_INDEX(partitionIndex, socketId);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    if (TcpIp_ValidatePartition(apiId, partitionIndex) != E_OK)
    {
        /* do nothing */
    }
    else if (socketIndex >= TCPIP_SOCKET_NUM)
    {
        TcpIp_DetReportError(apiId, TCPIP_E_INV_ARG);
    }
    else if (statePtr == NULL_PTR)
    {
        TcpIp_DetReportError(apiId, TCPIP_E_PARAM_POINTER);
    }
    else
#endif
    {
        const TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketIndex);

        TcpIp_EnterExclusiveArea();
        if (socketRtPtr->Protocol == TCPIP_IPPROTO_TCP)
        {
            uint8 srcState;

            if (socketRtPtr->PcbPtr != NULL_PTR)
            {
                const struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;

                srcState = pcbPtr->state;
            }
            else
            {
                srcState = 0U; /* TCPSTATEID_CLOSED */
            }

            if (srcState < (sizeof(tcpStateTable) / sizeof(tcpStateTable[0U])))
            {
                *statePtr = tcpStateTable[srcState];
                ret       = E_OK;
            }
        }
        TcpIp_ExitExclusiveArea();
    }

    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
Std_ReturnType TcpIp_TcpShutdown(TcpIp_SocketIdType socketId, uint32 shut_rx, uint32 shut_tx)
{
    Std_ReturnType  ret            = E_NOT_OK;
    uint8           apiId          = TCPIP_SID_TCPSHUTDOWN;
    ApplicationType partitionIndex = TcpIp_MulticoreIndex;

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    if (TcpIp_ValidatePartition(apiId, partitionIndex) != E_OK)
    {
        /* do nothing */
    }
    else if (TcpIp_ValidateSocket(apiId, partitionIndex, socketId, TCPIP_IPPROTO_TCP) != E_OK)
    {
        /* do nothing */
    }
    else
#endif
    {
        TcpIp_SocketIdType       socketIndex = TCPIP_SOCKET_INDEX(partitionIndex, socketId);
        TcpIp_SocketRuntimeType* socketRtPtr = &TCPIP_SOCKET(partitionIndex, socketIndex);

        TcpIp_EnterExclusiveArea();
        if (socketRtPtr->Protocol == TCPIP_IPPROTO_TCP)
        {
            struct tcp_pcb* pcbPtr = (struct tcp_pcb*)socketRtPtr->PcbPtr;
            err_t           err;

            TcpIp_SocketFlagSet(socketRtPtr, TCPIP_SOCKET_FLAG_TCP_PENDING_EVENT);
            err = tcp_shutdown(pcbPtr, shut_rx, shut_tx);

            if (err == (err_t)ERR_OK)
            {
                ret = E_OK;
            }
        }
        TcpIp_ExitExclusiveArea();
    }

    return ret;
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif

#if (TCPIP_ARP_ENABLED == STD_ON)
#if TCPIP_ARP_SUPPORT_DYNAMIC_TIMEOUT_CHANGE
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
void TcpIp_ArpClearAllDynamicEntry(uint8 ctrlIdx)
{
    if (ctrlIdx < TcpIp_PbCfgPtr->CtrlNum)
    {
        const TcpIp_CtrlType*  ctrlCfgPtr = &TCPIP_CTRL_CONFIG(ctrlIdx);
        TcpIp_CtrlRuntimeType* ctrlRtPtr  = ctrlCfgPtr->RuntimePtr;
        struct netif*          netifPtr   = &(ctrlRtPtr->Netif);
        etharp_clear_dynamic_entry(netifPtr);
    }
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
void TcpIp_ArpSetDynamicTimeout(uint16 timeVlu)
{
    etharp_set_dynamic_timeout(timeVlu);
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"

#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"
void TcpIp_ArpClearDynamicTimeout(void)
{
    etharp_clear_dynamic_timeout();
}
#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"
#endif
#endif

#endif
