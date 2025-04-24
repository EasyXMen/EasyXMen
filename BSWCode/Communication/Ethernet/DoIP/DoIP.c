/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : DoIP.c                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YB                                                          **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:DoIP<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:Functions may be used in other projects.

    \li PRQA S 1505 MISRA Rule 8.7 .<br>
    Reason:Functions may be used in other projects.

    \li PRQA S 3408 MISRA Rule 8.4 .<br>
    Reason:Configuration parameter, design needs.

    \li PRQA S 1514 MISRA Rule 8.9 .<br>
    Reason:Configuration parameter, design needs.

    \li PRQA S 1531 MISRA Rule 8.7 .<br>
    Reason:Configuration parameter, design needs.

    \li PRQA S 2995 MISRA Rule 2.2 .<br>
    Reason:Authentication requires further implementation.

    \li PRQA S 2991 MISRA Rule 14.3 .<br>
    Reason:Authentication requires further implementation.

    \li PRQA S 2880 MISRA Rule 2.1 .<br>
    Reason:Authentication requires further implementation.

    \li PRQA S 2877 MISRA Dir 4.1 .<br>
    Reason:for loop condition is a macro definition, which varies in different projs.

    \li PRQA S 0779 MISRA Rule 5.2 .<br>
    Reason:Configuration parameter, design needs, namelength set to 63 in C99.

    \li PRQA S 0791 MISRA Rule 5.4 .<br>
    Reason:Memmap section, design needs, namelength set to 63 in C99.

    \li PRQA S 0431 MISRA Rule 1.1 .<br>
    Reason:The function parameter type is by design purpose.

    \li PRQA S 4342 MISRA Rule 10.5 .<br>
    Reason:Design required type conversions.

    \li PRQA S 3451 MISRA Rule 8.5 .<br>
    Reason:Different configurations produce variations;Compatible with multiple scenarios.

    \li PRQA S 3449 MISRA Rule 8.5 .<br>
    Reason:Different configurations produce variations;Compatible with multiple scenarios.

    \li PRQA S 3219 MISRA Rule 2.1 .<br>
    Reason:Design required.

    \li PRQA S 3678 MISRA Rule 8.13 .<br>
    Reason:Realize required.

    \li PRQA S 0624 MISRA Rule 8.13 .<br>
    Reason:Compatible with multiple scenarios.
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "DoIP_Cbk.h"
#include "DoIP_Internal.h"
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#ifndef DOIP_AR_RELEASE_MAJOR_VERSION
#error " DoIP version miss"
#endif
#ifndef DOIP_AR_RELEASE_MINOR_VERSION
#error " DoIP version miss"
#endif
#ifndef DOIP_AR_RELEASE_PATCH_VERSION
#error " DoIP version miss"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/*pointer to PCConfig*/
P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST) DoIP_PBCfgPtr;
#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIP_MemMap.h"
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/*
 * Brief               This service initializes all global variables of the DoIP module. After return of
 *                     this service the DoIP module is operational.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      DoIPConfigPtr: Pointer to the configuration data of the DoIP module
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
/******************************************************************************/
#define DOIP_START_SEC_CODE
#include "DoIP_MemMap.h"
FUNC(void, DOIP_CODE) DoIP_Init(P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST) DoIPConfigPtr)
{
    uint8 index;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    if (NULL_PTR == DoIPConfigPtr)
    {
        DOIP_DET_REPORT(DOIP_SID_INIT, DOIP_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        DoIP_PBCfgPtr = DoIPConfigPtr;
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        DoIP_CurrentTcpSocketNbr = 0u;
        DoIP_ResetPendingRoutingActiveMsg();
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
        for (index = 0u; index < DOIP_MAX_TCP_CONNECTIONS; index++)
        {
            DoIP_ResetTcpConn(index);
            DoIP_ResetTcpInnerBuf(index);
        }
        for (index = 0u; index < DOIP_UDP_MAX_CONNECTIONS; index++)
        {
            DoIP_ResetUdpConn(index);
        }
        /* PRQA S 2877++ */ /* MISRA Dir 4.1 */
        for (index = 0u; index < DOIP_UDP_VEHICLEANNOUNCEMENT_CONNECTIONS; index++)
        /* PRQA S 2877-- */ /* MISRA Dir 4.1 */
        {
            DoIP_AncConnStatus[index].socketId = DOIP_INVALID_UINT8;
            DoIP_AncConnStatus[index].soConState = SOAD_SOCON_OFFLINE;
        }
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
        DoIP_ResetVehicleConnCtrl();

#if (DOIP_MAX_VEHICLE > 0U)
        for (index = 0u; index < DOIP_MAX_VEHICLE; index++)
        {
            DoIP_VehicleConnCtrl.vehicleEntry[index].state = DOIP_VEHICLE_ENTRY_STATE_IDLE;
        }
#endif

#endif
    }
    DoIP_InitStatusRunTime = DOIP_INITED;
}
#if (TRUE == DOIP_VERSION_INFO_API)
/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     versioninfo:Pointer to where to store the version information of this module.
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, TCPIP_CODE) DoIP_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DOIP_VAR) versioninfo)
{
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        DOIP_DET_REPORT(DOIP_SID_GETVERSIONINFO, DOIP_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == DOIP_DEV_ERROR_DETECT */
    {
        versioninfo->moduleID = DOIP_MODULE_ID;
        versioninfo->vendorID = DOIP_VENDOR_ID;
        versioninfo->sw_major_version = DOIP_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = DOIP_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = DOIP_SW_PATCH_VERSION;
    }
}
#endif /* TRUE == DOIP_VERSION_INFO_API */
/******************************************************************************/
/*
 * Brief               Notification about a SoAd socket connection state change,
 *                     e.g. socket connection gets online
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different SoConIds. Non reentrant for the
 *                     same SoConId.
 * Param-Name[in]      SoConId,socket connection index specifying the socket
 *                     connection with the mode change.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
FUNC(void, DOIP_CODE)
DoIP_SoConModeChg(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode)
{
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    boolean det_status;
    det_status = DoIP_SoConModeChg_DetCheck(SoConId, Mode);
    if (TRUE == det_status)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        /*TCP socket connection control*/
        DoIP_SoConModeChgTcpNty(SoConId, Mode);
        /*UDP socket connection control*/
        DoIP_SoConModeChgUdpNty(SoConId, Mode);
        /*UDP VehicleAnnouncement socket connections control*/
        DoIP_SoConModeChgAncNty(SoConId, Mode);
    }
}

/******************************************************************************/
/*
 * Brief               This function gets called by the SoAd if an IP address
 *                     assignment related to a socket connection changes
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different SoConIds. Non reentrant for the
 *                     same SoConId.
 * Param-Name[in]      SoConId,socket connection index specifying the socket
 *                     connection where the IP address assigment has changed
 *                     State,state of IP address assignment
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
FUNC(void, DOIP_CODE)
DoIP_LocalIpAddrAssignmentChg(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(TcpIp_IpAddrStateType, AUTOMATIC) State)
{

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00195*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_UNINIT);
    }
    /*SWS_DoIP_00196*/
    else if ((SoConId == (SoAd_SoConIdType)DOIP_INVALID_UINT16) || (State > TCPIP_IPADDR_STATE_UNASSIGNED))
    {
        DOIP_DET_REPORT(DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_INVALID_PARAMETER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        /*SWS_DoIP_00154*/
        if (TCPIP_IPADDR_STATE_ASSIGNED == State)
        {
#if (TRUE == DOIP_VIN_GID_MASTER)
            if ((DoIP_General.DoIPTriggerGidSyncCbk != NULL_PTR)
                && (E_OK == (*DoIP_General.DoIPTriggerGidSyncCbk->DoIPTriggerGidSyncDirect)()))
            {
                DoIP_GidSynStatus = TRUE;
            }
            else
            {
                DoIP_GidSynStatus = FALSE;
            }
#endif /* TRUE == DOIP_VIN_GID_MASTER */
        }
        /*SWS_DoIP_00085*/
        else
        {
            DoIP_GidSynStatus = FALSE;
        }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
    }
}
/******************************************************************************/
/*
 * Brief               Indication of a received I-PDU from a lower layer
 *                     communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the
 *                     same PduId.
 * Param-Name[in]      RxPduId,ID of the received I-PDU.
 *                     PduInfoPtr,Contains the length (SduLength) of the
 *                     received I-PDU and a pointer to a buffer (SduDataPtr)
 *                     containing the I-PDU.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
FUNC(void, DOIP_CODE)
DoIP_SoAdIfRxIndication(VAR(PduIdType, AUTOMATIC) RxPduId, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr)
{
    uint32 PayloadLength;
    uint16 txpduId;
    uint16 PayloadType;
    SoAd_SoConIdType SoConId;
    DoIP_RoleType role;
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    if (TRUE == DoIP_SoAdIfRxIndication_DetCheck(RxPduId, PduInfoPtr))
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        /*Check Generic DoIP header synchronization pattern*/
        /*
        00:reserved 01:13400-2-2010 02:13400-2-2012 03:13400-2-2019 FF:default val for vehicle identification req
        */
        if (E_OK == DoIP_SoAdIfRxIndicationExtractParam(RxPduId, &txpduId, &SoConId, &role))
        {
            if (DOIP_GENERICHEADER_CHECK(PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength))
            {
                /*Get the PayloadType*/
                PayloadLength = DOIP_U8_2_U32(&PduInfoPtr->SduDataPtr[4]);
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
                if ((DOIP_ROLE_CLIENT & role) != 0u)
                {
                    if (PayloadLength <= (uint32)DOIP_MAX_REQUEST_BYTES)
                    {
                        DoIP_MsgIfRxHandler(role, txpduId, SoConId, PayloadLength, PduInfoPtr);
                    }
                }
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
                if ((DOIP_ROLE_SERVER & role) != 0u)
                {
                    PayloadType = DOIP_U8_2_U16(&PduInfoPtr->SduDataPtr[2]);

                    if ((PduInfoPtr->SduDataPtr[0] == 0xffu) && (PayloadType != DOIP_VEHICLE_INDENT_REQ)
                        && (PayloadType != DOIP_VEHICLE_INDENT_REQ_WITH_EID)
                        && (PayloadType != DOIP_VEHICLE_INDENT_REQ_WITH_VIN))
                    {
                        /*SWS_DoIP_00014 Generic DoIP header synchronization pattern Check failed  */
                        DoIP_UdpSendNack(txpduId, DOIP_E_INCORRECT_PATTERN_FORMAT);
                        (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
                        (void)SoAd_OpenSoCon(SoConId);
#endif
                    }
                    else if (PayloadLength > (uint32)DOIP_MAX_REQUEST_BYTES)
                    {
                        DoIP_UdpSendNack(txpduId, DOIP_E_MESSAGE_TO_LARGE);
                    }
                    else
                    {
                        DoIP_MsgIfRxHandler(role, txpduId, SoConId, PayloadLength, PduInfoPtr);
                    }
                }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
            }
            else
            {
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
                if ((DOIP_ROLE_SERVER & role) != 0u)
                {
                    DoIP_UdpSendNack(txpduId, DOIP_E_INCORRECT_PATTERN_FORMAT);
                }
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
                (void)SoAd_CloseSoCon(SoConId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
                (void)SoAd_OpenSoCon(SoConId);
#endif
            }
        }
    }
}
/******************************************************************************/
/*
 * Brief               This function is called to provide the received data of
 *                     an I-PDU segment (N-PDU) to the upper layer.
 * ServiceId           0x44
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id,Identification of the received I-PDU.
 *                     info,Provides the source buffer (SduDataPtr) and the
 *                     number of bytes to be copied (SduLength).
 * Param-Name[in/out]  None
 * Param-Name[out]     bufferSizePtr,BUFREQ_OK: Data copied successfully
 *                     BUFREQ_E_NOT_OK: Data was not copied because an
 *                     error occurred.
 * Return              None
 *
 */
FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_SoAdTpCopyRxData(
    VAR(PduIdType, AUTOMATIC) id,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
{
    uint8 i;
    uint8 bufIdx;
    BufReq_ReturnType ret = BUFREQ_OK;
    uint16 txpduId;
    DoIP_RoleType role;
    P2VAR(DoIP_TcpInnerBufferType, AUTOMATIC, DOIP_VAR) pBuffer; /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    DoIP_DiagRcvHandleType DoIP_DiagRcvHandle;
    DoIPTcpConnectionType const* pTcpConn;
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    ret = DoIP_SoAdTpCopyRxData_DetCheck(id, info, bufferSizePtr);
    if (BUFREQ_OK == ret)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {  /*get rxbuffer and SoConId*/
        pTcpConn = DoIP_ExtractTcpConnByDoIPSoadRxPduId(id);
        if (NULL_PTR != pTcpConn)
        {
            txpduId = pTcpConn->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
            if (E_OK == DoIP_GetTcpBufIdxByRxPduId(id, &bufIdx))
            {
                pBuffer = &DoIP_TcpInnerBuf[bufIdx];
                if (E_NOT_OK == SoAd_GetSoConId(txpduId, &(DoIP_DiagRcvHandle.SoConId)))
                {
                    ret = BUFREQ_E_NOT_OK;
                }
            }
            else
            {
                ret = BUFREQ_E_NOT_OK;
            }
        }
        else
        {
            ret = BUFREQ_E_NOT_OK;
        }
    }

    if ((E_OK == DoIP_GetTcpConnIdxByRxPduId(id, &i)) && (BUFREQ_OK == ret))
    {
        DoIP_TcpConnStatusType* pTcpConnStatus = &DoIP_TcpConnStatus[i];
        role = pTcpConnStatus->role;
        /*restart general Inactivity Timer*/
        if (pTcpConnStatus->routActiveState == DOIP_SOCKET_CONNECTION_REGISTERED)
        {
            pTcpConnStatus->genInactiveTimer = 0u;
        }
        /*SWS_DoIP_00208*/
        switch (pBuffer->rxCtrl.procStatus)
        {
        case DOIP_RX_PROCESS_RECEIVING:
            /*with the first step,does not need to copy data*/
            if (info->SduLength == 0u)
            {
                *bufferSizePtr = DOIP_HEADER_LEN;
                pBuffer->rxCtrl.procStatus = DOIP_RX_PROCESS_HEADER_RECEIVING;
            }
            else
            {
                /*may not happen,this is depend on the soad module*/
                ret = BUFREQ_E_NOT_OK;
            }
            break;
        case DOIP_RX_PROCESS_HEADER_RECEIVING:
            ret = DoIP_HeaderTpRxHandler(role, info, txpduId, bufferSizePtr, bufIdx);
            break;
        case DOIP_RX_PROCESS_HEADER_RECEIVED:
        case DOIP_RX_PROCESS_CF_RECEIVING:
            DoIP_DiagRcvHandle.bufIdx = bufIdx;
            ret = DoIP_CFTpRxHandler(role, &DoIP_DiagRcvHandle, info, txpduId, bufferSizePtr);
            break;
        case DOIP_RX_PROCESS_DISCARD:
            /*at discard state,we intend to not copy data,just change the offset*/
            pBuffer->rxCtrl.rxBufPos += info->SduLength;
            if (pBuffer->rxCtrl.rxBufPos >= (pBuffer->rxCtrl.payloadLen + (PduLengthType)DOIP_HEADER_LEN))
            {
                *bufferSizePtr = 0u;
                DoIP_ResetTcpInnerRxBuf(bufIdx);
            }
            else
            {
                *bufferSizePtr = pBuffer->rxCtrl.payloadLen + (PduLengthType)DOIP_HEADER_LEN - pBuffer->rxCtrl.rxBufPos;
            }
            break;
        default:
            /*do nothing*/
            break;
        }
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function is called at the start of receiving an
 *                     N-SDU. The N-SDU might be fragmented into multiple
 *                     N-PDUs (FF with one or more following CFs) or might
 *                     consist of a single N-PDU (SF).
 * ServiceId           0x46
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id,Identification of the I-PDU.
 *                     info,Pointer to a PduInfoType structure containing the
 *                     payload data (without protocol information) and payload
 *                     length of the first frame or single frame of a transport
 *                     protocol I-PDU reception.
 * Param-Name[in/out]  None
 * Param-Name[out]     bufferSizePtr,Available receive buffer in the receiving
 *                     module.
 * Return              BufReq_ReturnType,
 *                     BUFREQ_OK: Connection has been accepted.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected.
 *
 */
FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_SoAdTpStartOfReception(
    VAR(PduIdType, AUTOMATIC) id,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    VAR(PduLengthType, AUTOMATIC) TpSduLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 index;
    BufReq_ReturnType ret;
    uint8 bufIdx = 0u;
    DoIP_TcpInnerBufferType* pBuffer;
    RxChannelCtrlType* rxCtrl;
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    ret = DoIP_SoAdTpStartOfReception_DetCheck(id, TpSduLength, bufferSizePtr);
    if (BUFREQ_OK == ret)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        ret = BUFREQ_E_NOT_OK;
        /*there is already a buffer occupied by this rxpdu*/
        if ((E_OK == DoIP_GetTcpBufIdxByRxPduId(id, &index))
            && (SOAD_SOCON_ONLINE
                == DoIP_TcpConnStatus[index].soConnState)) /*todo,tcp idx is the same with buffer index*/
        {
            rxCtrl = &(DoIP_TcpInnerBuf[index].rxCtrl);
            if ((rxCtrl->rxBufState != DOIP_BUFFER_IDLE) && (rxCtrl->procStatus == DOIP_RX_PROCESS_IDLE))
            {
                rxCtrl->procStatus = DOIP_RX_PROCESS_RECEIVING;
                bufIdx = index;
                ret = BUFREQ_OK;
            }
            else
            {
                ret = BUFREQ_E_BUSY;
            }
        }
        /*get a new idle buffer*/
        else
        {
            for (index = 0u; index < DOIP_MAX_TCP_CONNECTIONS; index++)
            {
                pBuffer = &DoIP_TcpInnerBuf[index];
                rxCtrl = &pBuffer->rxCtrl;
                if ((rxCtrl->rxBufState == DOIP_BUFFER_IDLE) && (rxCtrl->procStatus == DOIP_RX_PROCESS_IDLE))
                {
                    rxCtrl->rxBufState = DOIP_BUFFER_OCCUPIED;
                    rxCtrl->procStatus = DOIP_RX_PROCESS_RECEIVING;
                    rxCtrl->doipSoadRxPduId = id;
                    bufIdx = index;
                    ret = BUFREQ_OK;
                    break;
                }
            }
        }
    }
    /*SWS_DoIP_00207*/
    if (BUFREQ_OK == ret)
    {
        if (E_OK == DoIP_GetTcpConnIdxByRxPduId(rxCtrl->doipSoadRxPduId, &index))
        {
            DoIP_TcpConnStatus[index].bufferId = bufIdx;
        } /*aaa, and else? may returen not ok*/
        *bufferSizePtr = DOIP_RX_BUFFER_SIZE;
    }
    (void)info; /*unused*/
    return ret;
}

/******************************************************************************/
/*
 * Brief               By calling this API with the corresponding DoIPPduRRxId the currently ongoing data reception
 *                     is terminated immediately. When the function returns, no reception is in progress anymore with
 *                     the given DoIPPduRRxId identifier.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      DoIPPduRTxId: DoIP unique identifier of the PDU for which reception shall be canceled by the PduR
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     Reception was canceled successfully
 *                     E_NOT_OK: E_NOT_OK: Reception was not canceled
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelReceive(VAR(PduIdType, AUTOMATIC) DoIPPduRRxId)
{
    Std_ReturnType ret;
    uint8 i;
    PduIdType upperId;
    PduIdType soadRxPduId;
    uint8 bufIdx;
    DoIPTcpConnectionType const* pTcpConn;
    const RxChannelCtrlType* rxCtrl;
    /*SWS_DoIP_00169*/
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    ret = DoIP_TpCancelReceive_DetCheck(DoIPPduRRxId);
    if (E_OK == ret)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        ret = E_NOT_OK;
        if (E_OK == DoIP_ExtractChannelByPduRRxPduId(DoIPPduRRxId, &i))
        {
            upperId = DoIP_PBCfgPtr->DoIPChannel[i].DoIPPduRRxPdu->DoIPPduRUpperlayerPduId;
            for (i = 0u; i < DOIP_MAX_TCP_CONNECTIONS; i++)
            {
                rxCtrl = &(DoIP_TcpInnerBuf[i].rxCtrl);
                if (rxCtrl->pdurRxPduIdRef == upperId)
                {
                    soadRxPduId = rxCtrl->doipSoadRxPduId;
                    bufIdx = i;
                    ret = E_OK;
                    break;
                }
            }
        }
        if (E_OK == ret)
        {
            pTcpConn = DoIP_ExtractTcpConnByDoIPSoadRxPduId(soadRxPduId);
            if (NULL_PTR != pTcpConn)
            {
                /*SWS_DoIP_00258*/
                (void)SoAd_TpCancelReceive(pTcpConn->DoIPSoAdRxPdu->DoIPSoAdRxPduIdRef);
                DoIP_ResetTcpInnerRxBuf(bufIdx);
            }
            else
            {
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               This service is called to request the transfer data from the PduRouter to the SoAd.
 *                     It is used to indicate the transmission which will be performed in the DoIP_Mainfunction.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      DoIPPduRTxId: DoIP unique identifier of the PDU to be transmitted by the PduR
 *                     DoIPPduRTxInfoPtr: Tx Pdu information structure which contains the length of the DoIPTxMessage.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     The request has been accepted
 *                     E_NOT_OK: The request has not been accepted
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE)
DoIP_TpTransmit(VAR(PduIdType, AUTOMATIC) DoIPPduRTxId, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) DoIPPduRTxInfoPtr)
{
    uint8 bufferId;
    const TxChannelCtrlType* txCtrl;
    DoIP_TcpInnerBufferType* pBuffer;
    Std_ReturnType ret;
    boolean checkresult;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    ret = DoIP_TpTransmit_DetCheck(DoIPPduRTxId, DoIPPduRTxInfoPtr);
    /*SWS_DoIP_00284*/
    if (E_OK == ret)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        /*SWS_DoIP_00130 check if the connection is closed*/
        checkresult = DoIP_SocketConnectedCheckFrmPdurId(DoIPPduRTxId);
        if ((E_OK == DoIP_TransmitBufIdExtrac(DoIPPduRTxId, DoIPPduRTxInfoPtr, &bufferId)) && (TRUE == checkresult))
        {
            txCtrl = &(DoIP_TcpInnerBuf[bufferId].txCtrl);
            if ((txCtrl->bufferState == DOIP_BUFFER_IDLE) && (txCtrl->processStatus == DOIP_TX_PROCESS_IDLE))
            {
                ret = DoIP_TpTransmitInternal(DoIPPduRTxId, DoIPPduRTxInfoPtr);
            }
            else
            {
                pBuffer = &DoIP_TcpInnerBuf[bufferId];
                ret = DoIP_PendingTpEnqueue(&pBuffer->pendingTxTpQueue, DoIPPduRTxId, DoIPPduRTxInfoPtr);
            }
        }
        else
        {
            ret = E_NOT_OK;
        }
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This service primitive is used to cancel the transfer of pending DoIPPduRTxIds. The connection is
 *                     identified by DoIPPduRTxId. When the function returns, no transmission is in progress anymore
 *                     with the given DoIPPduRTxId identifier.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      DoIPPduRTxId: DoIP unique identifier of the PDU to be transmitted by the PduR
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     Transmit cancellation request of the specified DoIPPduRTxId is accepted.
 *                     E_NOT_OK: The transmit cancellation request of the DoIPPduRTxId has been rejected.
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpCancelTransmit(VAR(PduIdType, AUTOMATIC) DoIPPduRTxId)
{
    Std_ReturnType ret;
    uint8 bufferId;
    DoIP_TcpInnerBufferType* pBuffer;
    TxPendingTpQueueType const* tempQueue;
    const DoIPTcpConnectionType* pTcpConn = NULL_PTR;
    /*SWS_DoIP_00166*/
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    ret = DoIP_TpCancelTransmit_DetCheck(DoIPPduRTxId);
    if (E_OK == ret)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        ret = E_NOT_OK;
        if (E_OK == DoIP_GetTcpBufIdxByTxPduId(DoIPPduRTxId, &bufferId))
        {
            pBuffer = &DoIP_TcpInnerBuf[bufferId];
            pTcpConn = DoIP_ExtractTcpConnByDoIPSoadRxPduId(pBuffer->rxCtrl.doipSoadRxPduId);
            if ((pBuffer->txCtrl.processStatus != DOIP_TX_PROCESS_IDLE)
                && (pBuffer->txCtrl.msgType == DOIP_TX_DIAG_TP_MSG) && (NULL_PTR != pTcpConn))
            {
                (void)SoAd_TpCancelTransmit(pTcpConn->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId);
                DoIP_ResetTcpInnerTxBuf(bufferId);
                /*reset txbuffer including reset the processStatus to IDLE,except the pending tx buffer*/
                ret = E_OK;
            }

            tempQueue = DoIP_PendingTpDequeue(&pBuffer->pendingTxTpQueue);
            while (tempQueue != NULL_PTR)
            {
                tempQueue = DoIP_PendingTpDequeue(&pBuffer->pendingTxTpQueue);
                ret = E_OK;
            }
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function is called to acquire the transmit data of
 *                     an I-PDU segment (N-PDU).
 *                     Each call to this function provides the next part of the
 *                     I-PDU data unless retry->TpDataState is TP_DATARETRY.
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id,Identification of the transmitted I-PDU;
 *                     info,Provides the destination buffer (SduDataPtr) and
 *                     the number of bytes to be copied (SduLength).
 *                     retry,This parameter is used to acknowledge transmitted
 *                     data or to retransmit data after transmission problems.
 * Param-Name[in/out]  None
 * Param-Name[out]     availableDataPtr,Indicates the remaining number of bytes
 *                     that are available in the upper layer module's Tx buffer.
 * Return              BufReq_ReturnType,BUFREQ_OK: Data has been copied to the
 *                     transmit buffer completely as requested. BUFREQ_E_BUSY:
 *                     Request could not be fulfilled, because the required
 *                     amount of Tx data is not available. The lower layer
 *                     module may retry this call later on. No data has been
 *                     copied. BUFREQ_E_NOT_OK: Data has not been copied.
 *                     Request failed.
 *
 */
/******************************************************************************/
FUNC(BufReq_ReturnType, DOIP_CODE)
DoIP_SoAdTpCopyTxData(
    VAR(PduIdType, AUTOMATIC) id,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    P2CONST(RetryInfoType, AUTOMATIC, DOIP_CONST) retry,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) availableDataPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    BufReq_ReturnType ret;
    uint8 i;
    uint16 doIPPduRTxPduRef;
    PduInfoType pduinfo;
    DoIP_TcpInnerBufferType* pBuffer;
    TxChannelCtrlType* txCtrl;
    const uint8* txChannel;
    uint8* SduDataPtr;
    PduLengthType SduLength;
    DoIP_TcpConnStatusType* tcpConnStatus;
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
    DoIP_GatewayCtrlType* gwCtrl;
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    ret = DoIP_SoAdTpCopyTxData_DetCheck(id, info, retry, availableDataPtr);
    if (BUFREQ_OK == ret)
#else
    ret = BUFREQ_OK;
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        SduDataPtr = info->SduDataPtr;
        SduLength = info->SduLength;
        if (E_OK == DoIP_GetTcpConnIdxByTxPduId(id, &i))
        {
            tcpConnStatus = &DoIP_TcpConnStatus[i];
            pBuffer = &DoIP_TcpInnerBuf[tcpConnStatus->bufferId];
            if (tcpConnStatus->routActiveState == DOIP_SOCKET_CONNECTION_REGISTERED)
            {
                /*reset general Inactivity Timer*/
                tcpConnStatus->genInactiveTimer = 0u;
            }
        }
        else
        {
            ret = BUFREQ_E_NOT_OK;
        }

        if (BUFREQ_OK == ret)
        {
            txCtrl = &pBuffer->txCtrl;
            txChannel = pBuffer->txChannel;
            if ((txCtrl->msgType == DOIP_TX_NON_DIAG_MSG) || (txCtrl->msgType == DOIP_TX_DIAG_IF_MSG))
            {
                if (txCtrl->processStatus == DOIP_TX_PROCESS_TRANSMITING)
                {
                    /*SWS_DoIP_00224*/
                    if (SduLength != 0u)
                    {
                        /*SWS_DoIP_00225*/
                        DoIP_MemCpy(&txChannel[txCtrl->TxBufferPos], SduDataPtr, SduLength);
                        txCtrl->TxBufferPos += SduLength;
                    }
                    *availableDataPtr = txCtrl->txMaxDataLength - txCtrl->TxBufferPos;
                }
                else
                {
                    ret = BUFREQ_E_NOT_OK;
                }
            }
            else if (txCtrl->msgType == DOIP_TX_DIAG_TP_MSG)
            {
                /*first to transimit the diagnostic header*/
                if (txCtrl->processStatus == DOIP_TX_PROCESS_TRANSMITING)
                {
                    if (SduLength == 0u)
                    {
                        *availableDataPtr =
                            (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN - txCtrl->TxBufferPos;
                    }
                    else if ((txCtrl->TxBufferPos + SduLength) > (PduLengthType)(DOIP_HEADER_LEN + DOIP_SA_TA_LEN))
                    {
                        /*maybe not happen,because this is depend on the lower layer application*/
                        /*first to copy header bytes from the internal buffer*/
                        DoIP_MemCpy(
                            &txChannel[txCtrl->TxBufferPos],
                            SduDataPtr,
                            (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN - txCtrl->TxBufferPos);

                        txCtrl->TxBufferPos += (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN;
                        /*then to copy the rest bytes from pdur*/
                        doIPPduRTxPduRef = txCtrl->pdurTxPduIdRef;
                        pduinfo.SduDataPtr = &SduDataPtr[txCtrl->TxBufferPos];
                        pduinfo.SduLength =
                            SduLength - ((PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN);

                        if (BUFREQ_OK == PduR_DoIPTpCopyTxData(doIPPduRTxPduRef, &pduinfo, retry, availableDataPtr))
                        {
                            txCtrl->TxBufferPos += SduLength;
                            txCtrl->processStatus = DOIP_TX_PROCESS_HEADER_TRANSMITED;
                        }
                        else
                        {
                            ret = BUFREQ_E_NOT_OK;
                        }
                    }
                    else
                    {
                        DoIP_MemCpy(&txChannel[txCtrl->TxBufferPos], SduDataPtr, SduLength);
                        txCtrl->TxBufferPos += SduLength;
                        if (txCtrl->TxBufferPos == ((PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN))
                        {
                            txCtrl->processStatus = DOIP_TX_PROCESS_HEADER_TRANSMITED;
                            *availableDataPtr = txCtrl->txMaxDataLength - (PduLengthType)DOIP_HEADER_LEN
                                                - (PduLengthType)DOIP_SA_TA_LEN;
                        }
                        else
                        {
                            *availableDataPtr =
                                (PduLengthType)DOIP_HEADER_LEN + (PduLengthType)DOIP_SA_TA_LEN - txCtrl->TxBufferPos;
                        }
                    }
                }
                /*diagnostic header has been transmited*/
                else if (txCtrl->processStatus == DOIP_TX_PROCESS_HEADER_TRANSMITED)
                {
                    if (SduLength == 0u)
                    {
                        *availableDataPtr = txCtrl->txMaxDataLength - txCtrl->TxBufferPos;
                    }
                    else
                    {
                        doIPPduRTxPduRef = txCtrl->pdurTxPduIdRef;
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
                        gwCtrl = &tcpConnStatus->gwCtrl;
                        if (((DoIP_GetRoleByPduRTxPduId(id) & DOIP_ROLE_GATEWAY_IN) != 0u)
                            && (gwCtrl->waitRmtRsp == TRUE) && (gwCtrl->waitRspCfg != NULL_PTR))
                        {
                            gwCtrl->waitRmtRsp = FALSE;
                            gwCtrl->txNrc78Cnt = 0u;
                            gwCtrl->nrc78Rcvd = FALSE;
                        }
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
                        if (BUFREQ_OK == PduR_DoIPTpCopyTxData(doIPPduRTxPduRef, info, retry, availableDataPtr))
                        {
                            txCtrl->TxBufferPos += SduLength;
                        }
                        else
                        {
                            ret = BUFREQ_E_NOT_OK;
                        }
                    }
                }
                else
                {
                    ret = BUFREQ_E_NOT_OK;
                }
            }
            else
            {
                ret = BUFREQ_E_NOT_OK;
            }
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x48
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id,Identification of the transmitted I-PDU.
 *                     result,Result of the transmission of the I-PDU.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
FUNC(void, DOIP_CODE)
DoIP_SoAdTpTxConfirmation(VAR(PduIdType, AUTOMATIC) id, VAR(Std_ReturnType, AUTOMATIC) result)
{
    uint8 i;
    uint8 bufferId;
    SoAd_SoConIdType socketId;
    DoIP_TcpInnerBufferType* pBuffer;
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
    DoIP_GatewayCtrlType* gwCtrl;
#endif /* DOIP_ROLE_GATEWAY_SUPPORT == TRUE */
    const TxChannelCtrlType* txCtrl;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    if (TRUE == DoIP_SoAdTpTxConfirmation_DetCheck(id, result))
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if ((E_OK == DoIP_GetTcpConnIdxByTxPduId(id, &i))
            && (DOIP_INVALID_UINT8 != DoIP_TcpConnStatus[i].bufferId)) /*there may be a bug, should have get bufferid*/
        {
            /* PRQA S 3678++ */ /* MISRA Dir 8.13 */
            DoIP_TcpConnStatusType* tcpConnStatus = &DoIP_TcpConnStatus[i];
            /* PRQA S 3678-- */ /* MISRA Dir 8.13 */
            bufferId = tcpConnStatus->bufferId;
            pBuffer = &DoIP_TcpInnerBuf[bufferId];
            txCtrl = &pBuffer->txCtrl;

            if (DOIP_TX_DIAG_TP_MSG == txCtrl->msgType)
            {
                PduR_DoIPTpTxConfirmation(txCtrl->pdurTxPduIdRef, result);
            }
            else if (DOIP_TX_DIAG_IF_MSG == txCtrl->msgType)
            {
                PduR_DoIPIfTxConfirmation(txCtrl->pdurTxPduIdRef);
            }
            else
            {
                /*nothing*/
            }

            if (tcpConnStatus->wait4SocketClose == TRUE)
            {
                socketId = tcpConnStatus->socketId;
                const TxPendingTpQueueType* TpQueue;
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
                const TxPendingIfQueueType* IfQueue;
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
                /*check tp pengding message*/
                do
                {
                    /* PRQA S 0431++ */ /* MISRA Rule 1.1 */
                    TpQueue = DoIP_PendingTpDequeue(&pBuffer->pendingTxTpQueue);
                    /* PRQA S 0431-- */ /* MISRA Rule 1.1 */
                    if (NULL_PTR != TpQueue)
                    {
                        PduIdType PduRTxPdu;
                        Std_ReturnType ret =
                            DoIP_GetPduRTxPduRefByTxId(TpQueue->pendingPdurPduId, &PduRTxPdu, DOIP_TPPDU);
                        if (E_OK == ret)
                        {
                            PduR_DoIPTpTxConfirmation(PduRTxPdu, E_NOT_OK);
                        }
                    }
                } while (NULL_PTR != TpQueue);
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
                /*check if pengding message*/
                do
                {
                    /* PRQA S 0431++ */ /* MISRA Rule 1.1 */
                    IfQueue = DoIP_PendingIfDequeue(&pBuffer->pendingTxIfQueue);
                    /* PRQA S 0431-- */ /* MISRA Rule 1.1 */
                    if (NULL_PTR != IfQueue)
                    {
                        PduIdType PduRTxPdu;
                        Std_ReturnType ret =
                            DoIP_GetPduRTxPduRefByTxId(IfQueue->pendingPdurPduId, &PduRTxPdu, DOIP_TPPDU);
                        if (E_OK == ret)
                        {
                            PduR_DoIPTpTxConfirmation(PduRTxPdu, E_NOT_OK);
                        }
                    }
                } while (NULL_PTR != IfQueue);
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
                /*Here cannot close waiting tcp connection,*/
                (void)SoAd_CloseSoCon(socketId, TRUE);
#if (STD_ON == DOIP_SOCKET_AUTO_OPEN)
                (void)SoAd_OpenSoCon(socketId);
#endif
            }
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
            gwCtrl = &tcpConnStatus->gwCtrl;
            /*NRC 0x10 confirmed, then close connection*/
            if ((gwCtrl->waitRmtRsp == TRUE) && (gwCtrl->waitRspCfg->fakeRspMaxCnt < gwCtrl->txNrc78Cnt))
            {
                /*close diag session, can't close sockect*/
                gwCtrl->waitRmtRsp = FALSE;
                gwCtrl->txNrc78Cnt = 0u;
            }
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
            /*reset txbuffer including reset the processStatus to IDLE,except the pending tx buffer*/
            if ((txCtrl->TxBufferPos == txCtrl->txMaxDataLength) || (E_NOT_OK == result))
            {
                DoIP_ResetTcpInnerTxBuf(bufferId);
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief               Called after an I-PDU has been received via the TP API,
 *                     the result indicates whether the transmission was
 *                     successful or not.
 * ServiceId           0x45
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id,Identification of the received I-PDU.
 *                     result,Result of the reception.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
FUNC(void, DOIP_CODE)
DoIP_SoAdTpRxIndication(VAR(PduIdType, AUTOMATIC) id, VAR(Std_ReturnType, AUTOMATIC) result)
{
    uint8 idx = 0;
    PduIdType pdurRxId = 0;
    const RxChannelCtrlType* rxCtrl = NULL_PTR;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00200*/
    if (TRUE == DoIP_SoAdTpRxIndication_DetCheck(id, result))
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if (E_OK == DoIP_GetTcpBufIdxByRxPduId(id, &idx))
        {
            rxCtrl = &(DoIP_TcpInnerBuf[idx].rxCtrl);
            if ((rxCtrl->procStatus == DOIP_RX_PROCESS_CF_RECEIVING) && (rxCtrl->payloadType == DOIP_DIAG_MSG))
            {
                if (E_OK == DoIP_PduRRxIdExtract(rxCtrl->sa, rxCtrl->ta, &pdurRxId))
                {
                    PduR_DoIPTpRxIndication(pdurRxId, result);
                }
            }
        }

        /*diag msg/diag ack/diag nack will be realease here*/
        if (E_OK == DoIP_GetTcpConnIdxByRxPduId(id, &idx))
        {
            DoIP_CloseTcpConnection(DoIP_TcpConnStatus[idx].socketId);
        }
    }
}

#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
/******************************************************************************/
/*
 * Brief               Requests transmission of an I-PDU.
 * ServiceId           0x49
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id: Identification of the I-PDU.
 *                     info:Length and pointer to the buffer of the I-PDU
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     request is accepted by the destination module.
 *                     E_NOT_OK: request is not accepted by the destination module.
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE)
DoIP_IfTransmit(VAR(PduIdType, AUTOMATIC) id, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info)
{
    Std_ReturnType ret;
    uint8 bufferId;
    DoIP_TcpInnerBufferType* pBuffer;
    boolean checkresult;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    ret = DoIP_IfTransmit_DetCheck(id, info);
    if (E_OK == ret)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        /*SWS_DoIP_00130 check if the connection is closed*/
        checkresult = DoIP_SocketConnectedCheckFrmPdurId(id);
        if ((E_OK == DoIP_TransmitBufIdExtrac(id, info, &bufferId)) && (TRUE == checkresult))
        {
            pBuffer = &DoIP_TcpInnerBuf[bufferId];
            if ((pBuffer->txCtrl.bufferState == DOIP_BUFFER_IDLE)
                && (pBuffer->txCtrl.processStatus == DOIP_TX_PROCESS_IDLE))
            {
                ret = DoIP_IfTransmitInternal(id, info);
            }
            else
            {
                ret = DoIP_PendingIfEnqueue(&pBuffer->pendingTxIfQueue, id, info);
            }
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}
#else
/*defined for PDUR reference, if server not support, DoIP_IfTransmit will not be called*/
FUNC(Std_ReturnType, DOIP_CODE)
DoIP_IfTransmit(VAR(PduIdType, AUTOMATIC) id, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info)
{
}
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/

/*server support apis*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
/******************************************************************************/
/*
 * Brief               The lower layer communication interface module confirms
 *                     the transmission of an I-PDU.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the
 *                     same PduId.
 * Param-Name[in]      TxPduId,ID of the I-PDU that has been transmitted.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
FUNC(void, DOIP_CODE) DoIP_SoAdIfTxConfirmation(VAR(PduIdType, AUTOMATIC) TxPduId)
{
    uint8 i;
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    boolean detResult;
    /*SWS_DoIP_00249*/
    if (DoIP_InitStatusRunTime == DOIP_UNINIT)
    {
        detResult = FALSE;
        DOIP_DET_REPORT(DOIP_SID_SOADIFTXCONFIRMATION, DOIP_E_UNINIT);
    }
    /*SWS_DoIP_00250*/
    else if (DOIP_FOR_SOAD_TX_MAX_ID <= TxPduId)
    {
        detResult = FALSE;
        DOIP_DET_REPORT(DOIP_SID_SOADIFTXCONFIRMATION, DOIP_E_INVALID_PDU_SDU_ID);
    }
    else
    {
        detResult = TRUE;
    }
    if (TRUE == detResult)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        for (i = 0u; i < DOIP_UDP_MAX_CONNECTIONS; i++)
        {
            if (DoIP_UdpConnStatus[i].txPduId == TxPduId)
            {
                SoAd_ReleaseRemoteAddr(DoIP_UdpConnStatus[i].socketId);
                break;
            }
        }
    }
}
/******************************************************************************/
/*
 * Brief               Requests cancellation of an ongoing transmission of an I-PDU in a lower layer
 *                     communication interface module.
 * ServiceId           0x4a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      id: Identification of the I-PDU to be cancelled.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType
 *                     E_OK:     Cancellation was executed successfully by the destination module.
 *                     E_NOT_OK: Cancellation was rejected by the destination module.
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_IfCancelTransmit(VAR(PduIdType, AUTOMATIC) id)
{
    Std_ReturnType ret;
    uint8 bufferId;
    PduIdType soadTxPduId;
    TxPendingIfQueueType const* tempQueue;
    DoIP_TcpInnerBufferType* pBuffer;
    DoIPTcpConnectionType const* pTcpConn;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    ret = DoIP_IfCancelTransmit_DetCheck(id);
    if (E_OK == ret)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        ret = E_NOT_OK;
        if (E_OK == DoIP_GetTcpBufIdxByTxPduId(id, &bufferId))
        {
            pBuffer = &DoIP_TcpInnerBuf[bufferId];
            if ((pBuffer->txCtrl.processStatus != DOIP_TX_PROCESS_IDLE)
                && (pBuffer->txCtrl.msgType == DOIP_TX_DIAG_IF_MSG))
            {
                pTcpConn = DoIP_ExtractTcpConnByDoIPSoadRxPduId(pBuffer->rxCtrl.doipSoadRxPduId);
                if (NULL_PTR != pTcpConn)
                {
                    soadTxPduId = pTcpConn->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                    (void)SoAd_TpCancelTransmit(soadTxPduId);
                    /*reset txbuffer including reset the processStatus to IDLE,except the pending tx buffer*/
                    DoIP_ResetTcpInnerTxBuf(bufferId);
                    ret = E_OK;
                }
            }
            /*check whether this is the pending message*/
            tempQueue = DoIP_PendingIfDequeue(&pBuffer->pendingTxIfQueue);
            while (tempQueue != NULL_PTR)
            {
                tempQueue = DoIP_PendingIfDequeue(&pBuffer->pendingTxIfQueue);
                ret = E_OK;
            }
        }
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               This function is used to notify the DoIP on a switch of
 *                     the DoIPActivationLine
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 *
 */
FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitch(boolean* Active)
{
    boolean det_status = TRUE;
    boolean assinment;
    uint8 cnnIdx;
    uint16 txPduId;
    Std_ReturnType result = E_OK;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00285*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_LOCALIPADDRASSIGNMENT, DOIP_E_UNINIT);
        det_status = FALSE;
    }
    if (TRUE == det_status)
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if (TRUE == *Active)
        {

            for (cnnIdx = DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr;
                 (cnnIdx < DoIP_PBCfgPtr->DoIPConnections->DoIPAllUdpConnectionNbr) && (result == E_OK);
                 cnnIdx++)
            {
                txPduId = DOIP_UDPCON_CFG(cnnIdx)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                assinment = DOIP_UDPCON_CFG(cnnIdx)->DoIPRequestAddressAssignment;
                result = DoIP_SwitchOnOneLine(txPduId, assinment);
            }

            for (cnnIdx = DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr;
                 (cnnIdx < DoIP_PBCfgPtr->DoIPConnections->DoIPAllTcpConnectionNbr) && (result == E_OK);
                 cnnIdx++)
            {
                txPduId = DOIP_TCPCON_CFG(cnnIdx)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                assinment = DOIP_TCPCON_CFG(cnnIdx)->DoIPRequestAddressAssignment;
                result = DoIP_SwitchOnOneLine(txPduId, assinment);
            }

            for (cnnIdx = DoIP_PBCfgPtr->DoIPConnections->DoIPClientVehicleAnnouncementConnectionNbr;
                 (cnnIdx < DoIP_PBCfgPtr->DoIPConnections->DoIPAllVehicleAnnouncementConnectionNbr) && (result == E_OK);
                 cnnIdx++)
            {
                txPduId = DOIP_ANCCON_CFG(cnnIdx)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                assinment = DOIP_ANCCON_CFG(cnnIdx)->DoIPRequestAddressAssignment;
                result = DoIP_SwitchOnOneLine(txPduId, assinment);
            }
            if (result != E_OK)
            {
                *Active = FALSE;
            }
        }
        else if (FALSE == *Active)
        {
            for (cnnIdx = DoIP_PBCfgPtr->DoIPConnections->DoIPClientUdpConnectionNbr;
                 (cnnIdx < DoIP_PBCfgPtr->DoIPConnections->DoIPAllUdpConnectionNbr) && (result == E_OK);
                 cnnIdx++)
            {
                txPduId = DOIP_UDPCON_CFG(cnnIdx)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                assinment = DOIP_UDPCON_CFG(cnnIdx)->DoIPRequestAddressAssignment;
                result = DoIP_SwitchOffOneLine(txPduId, assinment);
            }

            for (cnnIdx = DoIP_PBCfgPtr->DoIPConnections->DoIPClientTcpConnectionNbr;
                 (cnnIdx < DoIP_PBCfgPtr->DoIPConnections->DoIPAllTcpConnectionNbr) && (result == E_OK);
                 cnnIdx++)
            {
                txPduId = DOIP_TCPCON_CFG(cnnIdx)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                assinment = DOIP_TCPCON_CFG(cnnIdx)->DoIPRequestAddressAssignment;
                result = DoIP_SwitchOffOneLine(txPduId, assinment);
            }

            for (cnnIdx = DoIP_PBCfgPtr->DoIPConnections->DoIPClientVehicleAnnouncementConnectionNbr;
                 (cnnIdx < DoIP_PBCfgPtr->DoIPConnections->DoIPAllVehicleAnnouncementConnectionNbr) && (result == E_OK);
                 cnnIdx++)
            {
                txPduId = DOIP_ANCCON_CFG(cnnIdx)->DoIPSoAdTxPdu->DoIPSoAdLowerlayerTxPduId;
                assinment = DOIP_ANCCON_CFG(cnnIdx)->DoIPRequestAddressAssignment;
                result = DoIP_SwitchOffOneLine(txPduId, assinment);
            }
            if (result != E_OK)
            {
                *Active = TRUE;
            }
        }
        else
        {
            /*nothing*/
        }
    }
}
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/

#if ((DOIP_ROLE_SERVER_SUPPORT == TRUE) && (DOIP_ROLE_GATEWAY_SUPPORT == TRUE))
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to connect
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              E_OK,E_NOT_OK
 *                     indicates the process resulet of this function
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_ConnectToVehicle(uint8 VehicleID)
{
    /*0. start connection timer*/
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00195*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_CONNECTTOVEHICLE, DOIP_E_UNINIT);
    }
    else if (DOIP_MAX_VEHICLE <= VehicleID)
    {
        DOIP_DET_REPORT(DOIP_SID_CONNECTTOVEHICLE, DOIP_E_INVALID_PARAMETER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if ((E_OK == DoIP_ClientActivationLineSwitchOn(VehicleID))
            /*1. create and open socket,       ==>DoIP_ClientActivationLineSwitchOn*/)
        {
            /*Mutiple vehicle use a same tiemr, each new vehicle start will reset this timer*/
            DoIP_VehicleConnCtrl.vehicleConnState = DOIP_CONN_STATE_STARTED;
            DoIP_VehicleConnCtrl.connTimer = 0u;

#if (DOIP_MAX_VEHICLE > 0U)
            DoIP_VehicleConnCtrl.vehicleEntry[VehicleID].state = DOIP_VEHICLE_ENTRY_STATE_CONNECTING;
#endif

            ret = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    /*The following phases are processed asynchronous*/
    /*2. listen vehicle announcement&vehicle id response    ==>DoIP_SoAdIfRxIndication*/
    /*3. wait for all node sync complete,send routing activation    ==>main function*/
    /*4. received routing activation rsp ==>DoIP_SoAdTpCopyRxData, notify user connected. or connection timeout*/
    return ret;
}
#endif /* DOIP_ROLE_SERVER_SUPPORT == TRUE && DOIP_ROLE_GATEWAY_SUPPORT == TRUE */
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to disconnect
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              E_OK,E_NOT_OK
 *                     indicates the process resulet of this function
 *
 */
/******************************************************************************/
FUNC(Std_ReturnType, DOIP_CODE) DoIP_DisconnectToVehicle(uint8 VehicleID)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00195*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_DISCONNECTTOVEHICLE, DOIP_E_UNINIT);
    }
    else if (DOIP_MAX_VEHICLE <= VehicleID)
    {
        DOIP_DET_REPORT(DOIP_SID_DISCONNECTTOVEHICLE, DOIP_E_INVALID_PARAMETER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
#if (DOIP_MAX_VEHICLE > 0U)
        if ((E_OK == DoIP_ClientActivationLineSwitchOff(VehicleID))
            && (DOIP_VEHICLE_ENTRY_STATE_CONNECTED == DoIP_VehicleConnCtrl.vehicleEntry[VehicleID].state))
        {
            DoIP_VehicleConnCtrl.vehicleStartCnt--;
            DoIP_VehicleConnCtrl.vehicleEntry[VehicleID].state = DOIP_VEHICLE_ENTRY_STATE_IDLE;
            ret = E_OK;
        }
#endif
    }

    return ret;
    /*1.close socket and reset*/
}
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendEntityStatusReq(uint8 vehicleId)
{
    uint8 txBuffer[DOIP_GENERIC_HEADER_LEN], index;
    PduInfoType pduInfo;
    PduIdType txPduId;
    SoAd_SoConIdType SoConId;
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00195*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDENTITYSTATUSREQ, DOIP_E_UNINIT);
    }
    else if (DOIP_MAX_VEHICLE <= vehicleId)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDENTITYSTATUSREQ, DOIP_E_INVALID_PARAMETER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if (E_OK == DoIP_GetUdpTxPduByVehicleId(vehicleId, &txPduId))
        {
            DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
            DoIP_u16_2_u8(&txBuffer[2], DOIP_ENTITY_STATUS_REQ);
            DoIP_u32_2_u8(&txBuffer[4], DOIP_ENTITY_STATUS_REQ_PAY_LEN);

            pduInfo.SduLength = DOIP_GENERIC_HEADER_LEN;
            pduInfo.SduDataPtr = txBuffer;
            (void)SoAd_IfTransmit(txPduId, &pduInfo);
            (void)SoAd_GetSoConId(txPduId, &SoConId);
            if (E_OK == DoIP_GetUdpConnBySoAdId(SoConId, &index))
            {
                DoIP_UdpConnStatus[index].outgoingMsgType = DOIP_ENTITY_STATUS_REQ;
                DoIP_UdpConnStatus[index].doipMsgTimer = 0u;
            }
        }
    }
}
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendPowerModeInfoReq(uint8 vehicleId)
{
    uint8 index;
    SoAd_SoConIdType SoConId;
    uint8 txBuffer[DOIP_GENERIC_HEADER_LEN];
    PduInfoType pduInfo;
    PduIdType txPduId;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00195*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDPOWERMODEINFOREQ, DOIP_E_UNINIT);
    }
    else if (DOIP_MAX_VEHICLE <= vehicleId)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDPOWERMODEINFOREQ, DOIP_E_INVALID_PARAMETER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if (E_OK == DoIP_GetUdpTxPduByVehicleId(vehicleId, &txPduId))
        {
            DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
            DoIP_u16_2_u8(&txBuffer[2], DOIP_DIAG_POWER_MODE_INFO_REQ);
            DoIP_u32_2_u8(&txBuffer[4], DOIP_POWER_MODE_REQ_PAY_LEN);

            pduInfo.SduLength = DOIP_GENERIC_HEADER_LEN;
            pduInfo.SduDataPtr = txBuffer;
            (void)SoAd_IfTransmit(txPduId, &pduInfo);
            (void)SoAd_GetSoConId(txPduId, &SoConId);
            if (E_OK == DoIP_GetUdpConnBySoAdId(SoConId, &index))
            {
                DoIP_UdpConnStatus[index].outgoingMsgType = DOIP_DIAG_POWER_MODE_INFO_REQ;
                DoIP_UdpConnStatus[index].doipMsgTimer = 0u;
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendVehicleIdReq(uint8 vehicleId)
{
    uint8 txBuffer[DOIP_GENERIC_HEADER_LEN];
    PduInfoType pduInfo;
    PduIdType txPduId;
    uint8 index;
    SoAd_SoConIdType SoConId;
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00195*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDVEHICLEIDREQ, DOIP_E_UNINIT);
    }
    else if (DOIP_MAX_VEHICLE <= vehicleId)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDVEHICLEIDREQ, DOIP_E_INVALID_PARAMETER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if (E_OK == DoIP_GetUdpTxPduByVehicleId(vehicleId, &txPduId))
        {
            DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
            DoIP_u16_2_u8(&txBuffer[2], DOIP_VEHICLE_INDENT_REQ);
            DoIP_u32_2_u8(&txBuffer[4], DOIP_VEHICLE_IDENT_REQ_PAY_LEN);

            pduInfo.SduLength = DOIP_GENERIC_HEADER_LEN;
            pduInfo.SduDataPtr = txBuffer;
            (void)SoAd_IfTransmit(txPduId, &pduInfo);
            (void)SoAd_GetSoConId(txPduId, &SoConId);
            if (E_OK == DoIP_GetUdpConnBySoAdId(SoConId, &index))
            {
                DoIP_UdpConnStatus[index].outgoingMsgType = DOIP_VEHICLE_INDENT_REQ;
                DoIP_UdpConnStatus[index].doipMsgTimer = 0u;
            }
        }
    }
}
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendVehicleIdReqWithEID(uint8 vehicleId)
{
    uint8 txBuffer[DOIP_GENERIC_HEADER_LEN];
    PduInfoType pduInfo;
    PduIdType txPduId;
    uint8 index;
    SoAd_SoConIdType SoConId;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00195*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDVEHICLEIDREQWITHEID, DOIP_E_UNINIT);
    }
    else if (DOIP_MAX_VEHICLE <= vehicleId)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDVEHICLEIDREQWITHEID, DOIP_E_INVALID_PARAMETER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if (E_OK == DoIP_GetUdpTxPduByVehicleId(vehicleId, &txPduId))
        {
            DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
            DoIP_u16_2_u8(&txBuffer[2], DOIP_VEHICLE_INDENT_REQ_WITH_EID);
            DoIP_u32_2_u8(&txBuffer[4], DOIP_VEHICLE_IDENT_REQ_PAY_LEN);

            pduInfo.SduLength = DOIP_GENERIC_HEADER_LEN;
            pduInfo.SduDataPtr = txBuffer;
            (void)SoAd_IfTransmit(txPduId, &pduInfo);
            (void)SoAd_GetSoConId(txPduId, &SoConId);
            if (E_OK == DoIP_GetUdpConnBySoAdId(SoConId, &index))
            {
                DoIP_UdpConnStatus[index].outgoingMsgType = DOIP_VEHICLE_INDENT_REQ_WITH_EID;
                DoIP_UdpConnStatus[index].doipMsgTimer = 0u;
            }
        }
    }
}
/******************************************************************************/
/*
 * Brief
 *
 *
 *
 * ServiceId           0xxx
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      VehicleID,indicates the server doip device to send message
 *
 * Param-Name[in/out]  None
 * Param-Name[out]
 * Return              None
 *
 */
/******************************************************************************/
FUNC(void, DOIP_CODE) DoIP_SendVehicleIdReqWithVIN(uint8 vehicleId)
{
    uint8 txBuffer[DOIP_GENERIC_HEADER_LEN];
    PduInfoType pduInfo;
    PduIdType txPduId;
    uint8 index;
    SoAd_SoConIdType SoConId;

#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    /*SWS_DoIP_00195*/
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDVEHICLEIDREQWITTHVIN, DOIP_E_UNINIT);
    }
    else if (DOIP_MAX_VEHICLE <= vehicleId)
    {
        DOIP_DET_REPORT(DOIP_SID_SENDVEHICLEIDREQWITTHVIN, DOIP_E_INVALID_PARAMETER);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        if (E_OK == DoIP_GetUdpTxPduByVehicleId(vehicleId, &txPduId))
        {
            DoIP_u16_2_u8(&txBuffer[0], DOIP_PROTOCOL_VERSION_AND_INVERSE);
            DoIP_u16_2_u8(&txBuffer[2], DOIP_VEHICLE_INDENT_REQ_WITH_VIN);
            DoIP_u32_2_u8(&txBuffer[4], DOIP_VEHICLE_IDENT_REQ_PAY_LEN);

            pduInfo.SduLength = DOIP_GENERIC_HEADER_LEN;
            pduInfo.SduDataPtr = txBuffer;
            (void)SoAd_IfTransmit(txPduId, &pduInfo);
            (void)SoAd_GetSoConId(txPduId, &SoConId);
            if (E_OK == DoIP_GetUdpConnBySoAdId(SoConId, &index))
            {
                DoIP_UdpConnStatus[index].outgoingMsgType = DOIP_VEHICLE_INDENT_REQ_WITH_VIN;
                DoIP_UdpConnStatus[index].doipMsgTimer = 0u;
            }
        }
    }
}
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
/******************************************************************************/
/*
 * Brief               Schedules the Diagnostic over IP module. (Entry point
 *                     for scheduling)
 * ServiceId           0x02
 *
 */
FUNC(void, DOIP_CODE) DoIP_MainFunction(void)
{
    if (DOIP_UNINIT != DoIP_InitStatusRunTime)
    {
#if (DOIP_ROLE_CLIENT_SUPPORT == TRUE)
        DoIP_VehicleConnTimeoutHandler();
        DoIP_DoIPMsgTimeoutHandler();
        DoIP_VehicleDiscoveryTimeoutHandler();
        DoIP_DiagnosticMsgTimeoutHandler();
#endif /*DOIP_ROLE_CLIENT_SUPPORT == TRUE*/
#if (DOIP_ROLE_GATEWAY_SUPPORT == TRUE)
        DoIP_GatewayTxFakeRespTimeoutHandler();
#endif /*DOIP_ROLE_GATEWAY_SUPPORT == TRUE*/
        /*TCP sockets timeout handle*/
        DoIP_TcpSocketTimeoutHandler();
#if (DOIP_HIGH_FREQ_TASK_SUPPORT == FALSE)
        /*pending message transmit handle*/
        DoIP_PendingDiagMsgHandler();
#endif /*DOIP_HIGH_FREQ_TASK_SUPPORT == FALSE*/
#if (DOIP_ROLE_SERVER_SUPPORT == TRUE)
        DoIP_VehicleAncTimeouthandler();
#endif /*DOIP_ROLE_SERVER_SUPPORT == TRUE*/
    }
}
#if (DOIP_HIGH_FREQ_TASK_SUPPORT == TRUE)
/******************************************************************************/
/*
 * Brief               Schedules the Diagnostic over IP module. (Entry point
 *                     for scheduling) for hight frequency tasks.
 * ServiceId           0xxx
 *
 */
FUNC(void, DOIP_CODE) DoIP_MainFunction_HighFrequency(void)
{
#if (STD_ON == DOIP_DEV_ERROR_DETECT)
    if (DOIP_UNINIT == DoIP_InitStatusRunTime)
    {
        DOIP_DET_REPORT(DoIP_SID_MAINFUNCTIONHIGHFEQ, DOIP_E_UNINIT);
    }
    else
#endif /*STD_ON == DOIP_DEV_ERROR_DETECT*/
    {
        /*pending message transmit handle*/
        DoIP_PendingDiagMsgHandler();
    }
}
#endif /*DOIP_HIGH_FREQ_TASK_SUPPORT == TRUE*/
#define DOIP_STOP_SEC_CODE
#include "DoIP_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
