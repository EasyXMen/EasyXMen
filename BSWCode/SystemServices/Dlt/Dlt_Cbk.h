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
**  FILENAME    : Dlt_Cbk.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : Callback extern FUNCtions declared of Dlt                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef DLT_CBK_H_
#define DLT_CBK_H_

/*============================================================================*/

#include "Dlt_Types.h"

/*
 * Brief               Indication of a received PDU from a lower layer communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      RxPduId:ID of the received PDU.
 *                     PduInfoPtr:Contains the length (SduLength) of the received PDU,
 *                     a pointer to a buffer (SduDataPtr) containing the PDU, and the MetaData related to this PDU.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
extern FUNC(void, DLT_APPL_CODE)
    Dlt_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) PduInfoPtr);

/*
 * Brief               Within this API, the upper layer module (called module) shall check whether the available data
 *                     fits into the buffer size reported by PduInfoPtr->SduLength. If it fits, it shall copy its data
 *                     into the buffer provided by PduInfoPtr->SduDataPtr and update the length of the actual copied
 *                     data in PduInfoPtr->SduLength. If not, it returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId:ID of the SDU that is requested to be transmitted.
 *                     PduInfoPtr:Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall
 *                     be copied, and the available buffer size in SduLengh. On return,
 *                     the service will indicate the length of the copied SDU data in SduLength.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType:E_OK: SDU has been copied and SduLength indicates the number of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must not be used since it may contain a
 *                     NULL pointer or point to invalid data.
 */
extern FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_TriggerTransmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) PduInfoPtr);

/*
 * Brief               The lower layer communication interface module confirms the transmission of a PDU, or the failure
 * to transmit a PDU. ServiceId           0x40 Sync/Async          Synchronous Reentrancy          Reentrant for
 * different PduIds. Non reentrant for the same PduId. Param-Name[in]      TxPduId:ID of the PDU that has been
 * transmitted. result:E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed. Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
#ifdef DLT_TX_CONFIRM_R19_USED
extern FUNC(void, DLT_APPL_CODE) Dlt_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
#else
extern FUNC(void, DLT_APPL_CODE) Dlt_TxConfirmation(PduIdType TxPduId);
#endif

/*
 * Brief               This extern FUNCtion is called after the I-PDU has been transmitted on its network,
 *                     the result indicates whether the transmission was successful or not.
 * ServiceId           0x48
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id:Identification of the transmitted I-PDU.
 *                     result:Result of the transmission of the I-PDU
 *                     result:E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
extern FUNC(void, DLT_APPL_CODE) Dlt_TpTxConfirmation(PduIdType id, Std_ReturnType result);

/*
 * Brief               This extern FUNCtion is called to acquire the transmit data of an I-PDU segment (N-PDU).
 *                     Each call to this extern FUNCtion provides the next part of the I-PDU data unless
 *                     retry->TpDataState is TP_DATARETRY. In this case the extern FUNCtion restarts to copy the
 *                     data beginning at the offset from the current position indicated by retry->TxTpDataCnt.
 *                     The size of the remaining data is written to the position indicated by availableDataPtr.
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id:Identification of the transmitted I-PDU.
 *                     info:Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 * (SduLength). If not enough transmit data is available, no data is copied by the upper layer module and BUFREQ_E_BUSY
 *                     is returned. The lower layer module may retry the call. An SduLength of 0 can be used to indicate
 * state changes in the retry parameter or to query the current amount of available data in the upper layer module. In
 * this case, the SduDataPtr may be a NULL_PTR. retry:This parameter is used to acknowledge transmitted data or to
 * retransmit data after transmission problems. If the retry parameter is a NULL_PTR, it indicates that the transmit
 * data can be removed from the buffer immediately after it has been copied. Otherwise, the retry parameter must point
 * to a valid RetryInfoType element. If TpDataState indicates TP_CONFPENDING, the previously copied data must remain in
 * the TP buffer to be available for error recovery. TP_DATACONF indicates that all data that has been copied before
 * this call is confirmed and can be removed from the TP buffer. Data copied by this API call is excluded and will be
 * confirmed later. TP_DATARETRY indicates that this API call shall copy previously copied data in order to recover from
 * an error. In this case TxTpDataCnt specifies the offset in bytes from the current data copy position. Param-Name[out]
 * availableDataPtr:Indicates the remaining number of bytes that are available in the upper layer module's Tx buffer.
 *                     availableDataPtr can be used by TP modules that support dynamic payload lengths (e.g. FrIsoTp) to
 * determine the size of the following CFs. Param-Name[in/out]  N/A Return              BufReq_ReturnType:BUFREQ_OK:
 * Data has been copied to the transmit buffer completely as requested. BUFREQ_E_BUSY: Request could not be fulfilled,
 * because the required amount of Tx data is not available. The lower layer module may retry this call later on. No data
 * has been copied. BUFREQ_E_NOT_OK: Data has not been copied. Request failed.
 */
extern FUNC(BufReq_ReturnType, DLT_APPL_CODE) Dlt_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
    P2CONST(RetryInfoType, AUTOMATIC, DLT_APPL_CONST) retry,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) availableDataPtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Brief               This extern FUNCtion is called at the start of receiving an N-SDU. The N-SDU might be fragmented
 * into multiple N-PDUs (FF with one or more following CFs) or might consist of a single N-PDU (SF). The service shall
 * provide the currently available maximum buffer size when invoked with TpSduLength equal to 0. ServiceId 0x46
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id:Identification of the I-PDU.
 *                     info:Pointer to a PduInfoType structure containing the payload data (without protocol
 * information) and payload length of the first frame or single frame of a transport protocol I-PDU reception, and the
 * MetaData related to this PDU. If neither first/single frame data nor MetaData are available, this parameter is set to
 * NULL_PTR. TpSduLength:Total length of the N-SDU to be received. Param-Name[out]     bufferSizePtr:Available receive
 * buffer in the receiving module. This parameter will be used to compute the Block Size (BS) in the transport protocol
 * module. Param-Name[in/out]  N/A Return              BufReq_ReturnType:BUFREQ_OK: Connection has been accepted.
 * bufferSizePtr indicates the available receive buffer; reception is continued. If no buffer of the requested size is
 * available, a receive buffer size of 0 shall be indicated by bufferSizePtr. BUFREQ_E_NOT_OK: Connection has been
 * rejected; reception is aborted. bufferSizePtr remains unchanged. BUFREQ_E_OVFL: No buffer of the required length can
 * be provided; reception is aborted. bufferSizePtr remains unchanged.
 */
extern FUNC(BufReq_ReturnType, DLT_APPL_CODE) Dlt_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
    PduLengthType TpSduLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

/*
 * Brief               Called after an I-PDU has been received via the TP API, the result indicates whether the
 * transmission was successful or not. ServiceId           0x45 Sync/Async          Synchronous Reentrancy Reentrant
 * Param-Name[in]      id:Identification of the received I-PDU.
 *                     result:Result of the reception.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
extern FUNC(void, DLT_APPL_CODE) Dlt_TpRxIndication(PduIdType id, Std_ReturnType result);

/*
 * Brief               This extern FUNCtion is called to provide the received data of an I-PDU segment (N-PDU) to the
 * upper layer. Each call to this extern FUNCtion provides the next part of the I-PDU data. The size of the remaining
 * buffer is written to the position indicated by bufferSizePtr. ServiceId           0x44 Sync/Async Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id:Identification of the I-PDU.
 *                     info:Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 *                     An SduLength of 0 can be used to query the current amount of available buffer in the upper layer
 * module. In this case, the SduDataPtr may be a NULL_PTR. Param-Name[out]     bufferSizePtr:Available receive buffer
 * after data has been copied. Param-Name[in/out]  N/A Return              BufReq_ReturnType:BUFREQ_OK: Data copied
 * successfully BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 */
extern FUNC(BufReq_ReturnType, DLT_APPL_CODE) Dlt_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#endif /* DLT_CBK_H_ */
