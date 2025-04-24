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
**  FILENAME    : DoIP_Cbk.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : YB                                                          **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef DOIP_CBK_H
#define DOIP_CBK_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "TcpIp_Types.h"
#include "SoAd.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

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
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) availableDataPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
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
extern FUNC(void, DOIP_CODE)
    DoIP_SoAdTpTxConfirmation(VAR(PduIdType, AUTOMATIC) id, VAR(Std_ReturnType, AUTOMATIC) result);
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
extern FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyRxData(
    VAR(PduIdType, AUTOMATIC) id,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
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
extern FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpStartOfReception(
    VAR(PduIdType, AUTOMATIC) id,
    P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) info,
    VAR(PduLengthType, AUTOMATIC) TpSduLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DOIP_VAR) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
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
extern FUNC(void, DOIP_CODE)
    DoIP_SoAdTpRxIndication(VAR(PduIdType, AUTOMATIC) id, VAR(Std_ReturnType, AUTOMATIC) result);
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
extern FUNC(void, DOIP_CODE)
    DoIP_SoAdIfRxIndication(VAR(PduIdType, AUTOMATIC) RxPduId, P2CONST(PduInfoType, AUTOMATIC, DOIP_CONST) PduInfoPtr);
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
FUNC(void, DOIP_CODE) DoIP_SoAdIfTxConfirmation(VAR(PduIdType, AUTOMATIC) TxPduId);
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
extern FUNC(void, DOIP_CODE)
    DoIP_SoConModeChg(VAR(SoAd_SoConIdType, AUTOMATIC) SoConId, VAR(SoAd_SoConModeType, AUTOMATIC) Mode);
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
extern FUNC(void, DOIP_CODE) DoIP_LocalIpAddrAssignmentChg(
    VAR(SoAd_SoConIdType, AUTOMATIC) SoConId,
    VAR(TcpIp_IpAddrStateType, AUTOMATIC) State);
#endif /*DOIP_CBK_H*/
/*=======[E N D   O F   F I L E]==============================================*/
