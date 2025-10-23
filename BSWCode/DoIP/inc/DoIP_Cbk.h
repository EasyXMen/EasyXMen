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
/*
 ************************************************************************************************************************
 **
 **  @file               : DoIP_Cbk.h
 **  @author             : YB
 **  @date               : 2019/8/30
 **  @vendor             : isoft
 **  @description        : Provided for SoAd
 **
 ***********************************************************************************************************************/

#ifndef DOIP_CBK_H_
#define DOIP_CBK_H_

/* =================================================== inclusions =================================================== */

#include "ComStack_Types.h"
#include "SoAd.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */
/* PRQA S 1512 ++ */ /* VL_DoIP_1512 */
/**
 * @brief       This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 *              Each call to this function provides the next part of the I-PDU data unless retry->TpDataState is
 * TP_DATARETRY.
 * @param[in]   soadTxPduId: Identification of the transmitted I-PDU.
 * @param[in]   pduInfoPtr: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength).
 * @param[in]   retry: This parameter is used to acknowledge transmitted data or toretransmit data after transmission
 * problems.
 * @param[out]  availableDataPtr: Indicates the remaining number of bytes that are available in the upper layer module's
 * Tx buffer.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Data has been copied to the transmit buffer completely as requested.
 * @retval      BUFREQ_E_BUSY: Request could not be fulfilled, because the required amount of Tx data is not available.
 *                             The lower layer module may retry this call later on. No data has been copied.
 * @retval      BUFREQ_E_NOT_OK: Data has not been copied. Request failed.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BufReq_ReturnType DoIP_SoAdTpCopyTxData(
    PduIdType            soadTxPduId,
    const PduInfoType*   pduInfoPtr,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr);

/**
 * @brief       Returns the version information of this module.
 * @param[in]   soadTxPduId: Identification of the transmitted I-PDU.
 * @param[in]   result: E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.
 * @return      void.
 * @retval      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SoAdTpTxConfirmation(PduIdType soadTxPduId, Std_ReturnType result);

/**
 * @brief       This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper layer.
 * @param[in]   soadRxPduId: Identification of the received I-PDU.
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @param[out]  bufferSizePtr: Available receive buffer after data has been copied.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Data copied successfully.
 * @retval      BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
BufReq_ReturnType
    DoIP_SoAdTpCopyRxData(PduIdType soadRxPduId, const PduInfoType* pduInfoPtr, PduLengthType* bufferSizePtr);

/**
 * @brief       This function is called at the start of receiving an N-SDU.
 *              The N-SDU might be fragmented into multiple N-PDUs (FF with one or more following CFs) or might consist
 * of a single N-PDU (SF).
 * @param[in]   soadRxPduId: Identification of the I-PDU.
 * @param[in]   pduInfoPtr: Pointer to a PduInfoType structure containing the payload data (without protocol
 * information) and payload length of the first frame or single frame of a transport protocol I-PDU reception..
 * @param[in]   sduLen: Total length of the N-SDU to be received.
 * @param[out]  bufferSizePtr: Available receive buffer in the receiving module. This parameter will be used to compute
 * the Block Size (BS) in the transport protocol module.
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Connection has been accepted. bufferSizePtr indicates the available receive buffer; reception
 * is continued.
 * @retval      BUFREQ_E_NOT_OK: Connection has been rejected; reception is aborted. bufferSizePtr remains unchanged.
 * @retval      BUFREQ_E_OVFL: No buffer of the required length can be provided; reception is aborted. bufferSizePtr
 * remains unchanged.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
BufReq_ReturnType DoIP_SoAdTpStartOfReception(
    PduIdType          soadRxPduId,
    const PduInfoType* pduInfoPtr,
    PduLengthType      sduLen,
    PduLengthType*     bufferSizePtr);

/**
 * @brief       Called after an I-PDU has been received via the TP API, the result indicates whether the transmission
 * was successful or not.
 * @param[in]   soadRxPduId: Identification of the received I-PDU.
 * @param[in]   result: E_OK: The PDU was received. E_NOT_OK: Reception of the PDU failed.
 * @return      void
 * @retval      None.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SoAdTpRxIndication(PduIdType soadRxPduId, Std_ReturnType result);

/**
 * @brief       Indication of a received I-PDU from a lower layer communication interface module.
 * @param[in]   soadRxPduId: Identification of the received I-PDU.
 * @param[in]   pduInfoPtr: Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr)
 * containing the PDU, and the MetaData related to this PDU.
 * @return      void
 * @retval      None.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SoAdIfRxIndication(PduIdType soadRxPduId, const PduInfoType* pduInfoPtr);

/**
 * @brief       The lower layer communication interface module confirms the transmission of an I-PDU.
 * @param[in]   soadTxPduId: ID of the I-PDU that has been transmitted.
 * @param[in]   result: Result of the transmission.
 * @return      void
 * @retval      None.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SoAdIfTxConfirmation(PduIdType soadTxPduId, Std_ReturnType result);

/**
 * @brief       Notification about a SoAd socket connection state change,
 *              e.g. socket connection gets online
 * @param[in]   soConId: socket connection index specifying the socket connection with the mode change.
 * @param[in]   mode: new mode.
 * @return      void
 * @retval      None.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SoConModeChg(SoAd_SoConIdType soConId, SoAd_SoConModeType mode);

/**
 * @brief       This function gets called by the SoAd if an IP address assignment related to a socket connection
 * changes.
 * @param[in]   soConId: socket connection index specifying the socket connection where the IP address assigment has
 * changed
 * @param[in]   state: state of IP address assignment.
 * @return      void
 * @retval      None.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_LocalIpAddrAssignmentChg(SoAd_SoConIdType soConId, TcpIp_IpAddrStateType state);
/* PRQA S 1512 -- */

#ifdef __cplusplus
}
#endif
#endif
