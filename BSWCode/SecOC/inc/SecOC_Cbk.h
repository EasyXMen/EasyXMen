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
 **  @file               : SecOC_Cbk.h
 **  @author             : HuRongbo
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : API declaration  of SecOC
 **
 ***********************************************************************************************************************/

/* ================================================ misar justifications ============================================ */

#ifndef SECOC_CBK_H
#define SECOC_CBK_H

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "Crypto_GeneralTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief          Indication of a received PDU from a lower layer
 *                 communication interface module.
 * @param[in]      RxPduId: ID of the received PDU.
 * @param[in]      PduInfoPtr: Contains the length (SduLength) of the
 *                 received PDU,a pointer to a buffer (SduDataPtr) containing
 *                 the PDU, and the MetaData related to this PDU.
 * @reentrant      Reentrant for different PduIds.
 * @synchronous    TRUE
 * @trace       CPD-70344
 */
void SecOC_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief          Indication of a received PDU from a lower layer
 *                 communication interface module.
 * @param[in]      id: Identification of the received I-PDU.
 * @param[in]      result: Result of the reception.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70345
 */
void SecOC_TpRxIndication(PduIdType id, Std_ReturnType result);

/**
 * @brief          The lower layer communication interface module confirms
 *                 the transmission of a PDU,or the failure transmit a PDU.
 * @param[in]      TxPduId: ID of the PDU that has been transmitted.
 * @param[in]      result: Result of the transmission.
 * @reentrant      Reentrant for different PduIds.
 * @synchronous    TRUE
 * @trace       CPD-70346
 */
void SecOC_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

/**
 * @brief          This function is called after the I-PDU has been
 *                 transmitted on its network, the result indicates whether
 *                 the transmission was successful or not.
 * @param[in]      id: Identification of the transmitted I-PDU.
 * @param[in]      result: Result of the transmission of the I-PDU.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70347
 */
void SecOC_TpTxConfirmation(PduIdType id, Std_ReturnType result);

/**
 * @brief          Within this API, the upper layer module (called module)
 *                 shall check whether the available data fits into the
 *                 buffer size reported by PduInfoPtr->SduLength.If it fits,
 *                 it shall copy its data into the buffer provided by
 *                 PduInfoPtr->SduDataPtr and update the length of the
 *                 actual copied data in PduInfoPtr->SduLength. If not, it
 *                 returns E_NOT_OK without changing PduInfoPtr.
 * @param[in]      TxPduId: ID of the SDU that is requested to be transmitted.
 * @param[inout]   PduInfoPtr: Contains a pointer to a buffer (SduDataPtr) to
 *                 where the SDU data shall be copied, and the available buffer
 *                 size in SduLengh.On return, the service will indicate the
 *                 length of the copied SDU data in SduLength.
 * @return         Std_ReturnType
 * @retval         E_OK: SDU has been copied and SduLength indicates the number
 *                 of copied bytes.
 * @retval         E_NOT_OK: No SDU data has been copied. PduInfoPtr must not be
 *                 used since it may contain a NULL pointer or point to invalid data.
 * @reentrant      Reentrant for different PduIds
 * @synchronous    TRUE
 * @trace       CPD-70348
 */
Std_ReturnType SecOC_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);

/**
 * @brief          This function is called to provide the received data of an
 *                 I-PDU segment (N-PDU) to the upper layer. Each call to this
 *                 function provides the next part of the I-PDU data.The size
 *                 of the remaining buffer is written to the position indicated
 *                 by bufferSizePtr.
 * @param[in]      id: Identification of the received I-PDU.
 * @param[in]      info: Provides the source buffer (SduDataPtr) and the number of
 *                 bytes to be copied (SduLength). An SduLength of 0 can be
 *                 used to query the current amount of available buffer in the
 *                 upper layer module. In this case, the SduDataPtr may be a
 *                 NULL_PTR.
 * @param[out]     bufferSizePtr: Available receive buffer after data has been
 *                 copied.
 * @return         BufReq_ReturnType
 * @retval         BUFREQ_OK: Data copied successfully
 * @retval         BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70349
 */
BufReq_ReturnType SecOC_CopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);

/**
 * @brief          This function is called to acquire the transmit data of an
 *                 I-PDU segment (N-PDU).Each call to this function provides
 *                 the next part of the I-PDU data unless retry->TpDataState
 *                 is TP_DATARETRY . In this case the function restarts to copy
 *                 the data beginning at the offset from the current position
 *                 indicated by retry->TxTpDataCnt.The size of the remaining data
 *                 is written to the position indicated by availableDataPtr.
 * @param[in]      id: Identification of the transmitted I-PDU.
 * @param[in]      info: Provides the destination buffer (SduDataPtr) and
 *                       the number of bytes to be copied (SduLength).
 * @param[in]      retry: This parameter is used to acknowledge transmitted
 *                        data or to retransmit data after transmission problems.
 * @param[out]     availableDataPtr: Indicates the remaining number of bytes
 *                 that are available in the upper layer module's Tx buffer.
 * @return         BufReq_ReturnType
 * @retval         BUFREQ_OK: Data has been copied to the transmit buffer completely
 *                 as requested.
 * @retval         BUFREQ_E_BUSY: Request could not be fulfilled, because the
 *                 required amount of Tx data is not available. The lower layer
 *                 module may retry this call later on. No data has been copied.
 * @retval         BUFREQ_E_NOT_OK: Data has not been copied. Request failed.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70350
 */
BufReq_ReturnType SecOC_CopyTxData(
    PduIdType            id,
    const PduInfoType*   info,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr);

/**
 * @brief          This function is called at the start of receiving an N-SDU.
 *                 The N-SDU might be fragmented into multiple N-PDUs (FF with
 *                 one or more following CFs) or might consist of a single N-PDU
 *                 (SF).
 * @param[in]      id: Identification of the I-PDU.
 * @param[in]      info: Pointer to a PduInfoType structure containing the payload
 *                 data (without protocol information) and payload length of the
 *                 first frame or single frame of a transport protocol I-PDU
 *                 reception, and the MetaData related to this PDU. If neither
 *                 first/single frame data nor MetaData are available, this
 *                 parameter is set to NULL_PTR.
 * @param[in]      TpSduLength: Total length of the N-SDU to be received.
 * @param[out]     bufferSizePtr: Available receive buffer in the receiving module. This
 *                 parameter will be used to compute the Block Size (BS) in the
 *                 transport protocol module.
 * @return         BufReq_ReturnType
 * @retval         BUFREQ_OK: Connection has been accepted. bufferSizePtr
 *                 indicates the available receive buffer; reception is
 *                 continued. If no buffer of the requested size is available,
 *                 a receive buffer size of 0 shall be indicated by bufferSizePtr.
 * @retval         BUFREQ_E_NOT_OK: Connection has been rejected; reception is
 *                 aborted. bufferSizePtr remains unchanged.
 * @retval         BUFREQ_E_OVFL: No buffer of the required length can be
 *                 provided; reception is aborted. bufferSizePtr remains unchanged.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70351
 */
BufReq_ReturnType SecOC_StartOfReception(
    PduIdType          id,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);

/**
 * @brief          CSM notifies SecOC that a Authenticator generate job has
 *                 finished.
 * @param[in]      job: JobID of the operation that caused the callback.
 * @param[in]      result: Contains the result of cryptographic operation.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70352
 */
void SecOC_CsmGenerateJobFinishedIndication(const Crypto_JobType* job, Crypto_ResultType result);

/**
 * @brief          CSM notifies SecOC that a Authenticator verify job has
 *                 finished.
 * @param[in]      job: JobID of the operation that caused the callback.
 * @param[in]      result: Contains the result of cryptographic operation.
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace       CPD-70353
 */
void SecOC_CsmVerifyJobFinishedIndication(const Crypto_JobType* job, Crypto_ResultType result);

#ifdef __cplusplus
}
#endif
#endif /* SECOC_CBK_H */
