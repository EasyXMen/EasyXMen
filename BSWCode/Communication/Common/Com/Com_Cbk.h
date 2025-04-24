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
**  FILENAME    : Com_Cbk.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Callback declaration supplied by COM                        **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* Refer to Com.h */

#ifndef COM_CBK_H
#define COM_CBK_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com_Types.h"
/*******************************************************************************
**                      Published information                                 **
*******************************************************************************/
/* Published information */
#if !defined(COM_PUBLISHED_INFORMATION)
#define COM_PUBLISHED_INFORMATION
#define COM_MODULE_ID                   50u
#define COM_VENDOR_ID                   62u
#define COM_AR_RELEASE_MAJOR_VERSION    4u
#define COM_AR_RELEASE_MINOR_VERSION    5u
#define COM_AR_RELEASE_REVISION_VERSION 0u
#define COM_SW_MAJOR_VERSION            2u
#define COM_SW_MINOR_VERSION            1u
#define COM_SW_PATCH_VERSION            25u
#elif ((COM_SW_MAJOR_VERSION != 2u) || (COM_SW_MINOR_VERSION != 1u))
#error "Com: Mismatch in Software Version"
#endif

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
/******************************************************************************/
/*
 * Brief               Within this API, the upper layer module (called module) shall check whether the
 *                     available data fits into the buffer size reported by PduInfoPtr->SduLength.
 *                     If it fits, it shall copy its data into the buffer provided by PduInfoPtr->SduDataPtr
 *                     and update the length of the actual copied data in PduInfoPtr->SduLength.
 *                     If not, it returns E_NOT_OK without changing PduInfoPtr.
 * ServiceId           0x41
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the SDU that is requested to be transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  PduInfoPtr: Contains a pointer to a buffer (SduDataPtr) to where the SDU
 *                     data shall be copied, and the available buffer size in SduLengh.
 *                     On return, the service will indicate the length of the copied SDU
 *                     data in SduLength.
 * Return              Std_ReturnType
 *                     E_OK: SDU has been copied and SduLength indicates the number of copied bytes.
 *                     E_NOT_OK: No SDU data has been copied. PduInfoPtr must not
 *                     be used since it may contain a NULL pointer or point to invalid data.
 */
/******************************************************************************/
extern FUNC(Std_ReturnType, COM_CODE)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    Com_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/******************************************************************************/
/*
 * Brief               Indication of a received I-PDU from a lower layer communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      RxPduId: ID of the received I-PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the received I-PDU and a pointer to
 *                     a buffer (SduDataPtr) containing the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
extern FUNC(void, COM_CODE)
    Com_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
/******************************************************************************/
/*
 * Brief               This service Initializes internal and external interfaces of
 *                     the CAN Interface for the further processing.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the received I-PDU.
 *                     result: Result of the reception.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
extern FUNC(void, COM_CODE) Com_TpRxIndication(PduIdType id, Std_ReturnType result);
/******************************************************************************/
/*
 * Brief               The lower layer communication interface module confirms the transmission of an IPDU.
 * ServiceId           0x40
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      TxPduId: ID of the I-PDU that has been transmitted.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
extern FUNC(void, COM_CODE) Com_TxConfirmation(PduIdType TxPduId);
/******************************************************************************/
/*
 * Brief               This function is called after the I-PDU has been transmitted on its network, the
 *                     result indicates whether the transmission was successful or not.
 * ServiceId           0x48
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     result: Result of the transmission of the I-PDU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
extern FUNC(void, COM_CODE) Com_TpTxConfirmation(PduIdType id, Std_ReturnType result);
/******************************************************************************/
/*
 * Brief               This function is called at the start of receiving an N-SDU. The N-SDU might be
 *                     fragmented into multiple N-PDUs (FF with one or more following CFs) or might
 *                     consist of a single N-PDU (SF).
 * ServiceId           0x46
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the I-PDU.
 *                     info: Pointer to a PduInfoType structure containing the payload
 *                     data (without protocol information) and payload length of the
 *                     first frame or single frame of a transport protocol I-PDU reception.
 *                     Depending on the global parameter MetaDataLength, additional bytes containing
 *                     MetaData (e.g. the CAN ID) are appended after the payload data, increasing the length
 *                     accordingly. If neither first/single frame data nor MetaData
 *                     are available, this parameter is set to NULL_PTR.
 *                     TpSduLength: Total length of the N-SDU to be received.
 * Param-Name[out]     bufferSizePtr: Available receive buffer in the receiving module. This parameter will
 *                     be used to compute the Block Size (BS) in the transport protocol module.
 * Param-Name[in/out]  None
 * Return              BufReq_ReturnType(BUFREQ_OK,BUFREQ_E_NOT_OK,BUFREQ_E_OVFL)
 *                     BUFREQ_OK: Connection has been accepted. bufferSizePtr
 *                     indicates the available receive buffer; reception is continued.
 *                     If no buffer of the requested size is available, a receive buffer
 *                     size of 0 shall be indicated by bufferSizePtr.
 *                     BUFREQ_E_NOT_OK: Connection has been rejected; reception is aborted. bufferSizePtr remains
 * unchanged. BUFREQ_E_OVFL: No buffer of the required length can be provided; reception is aborted. bufferSizePtr
 * remains unchanged.
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
extern FUNC(BufReq_ReturnType, COM_CODE) Com_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) bufferSizePtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/******************************************************************************/
/*
 * Brief               This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper
 * layer. Each call to this function provides the next part of the I-PDU data. The size of the remaining data is written
 * to the position indicated by bufferSizePtr. ServiceId           0x44 Sync/Async          Synchronous Reentrancy
 * Reentrant Param-Name[in]      id: Identification of the received I-PDU. info: Provides the source buffer (SduDataPtr)
 * and the number of bytes to be copied (SduLength). An SduLength of 0 can be used to query the current amount of
 * available buffer in the upper layer module. In this case, the SduDataPtr may be a NULL_PTR. Param-Name[out]
 * bufferSizePtr: Available receive buffer after data has been copied. Param-Name[in/out]  None Return BufReq_ReturnType
 *                     BUFREQ_OK: Data copied successfully
 *                     BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
extern FUNC(BufReq_ReturnType, COM_CODE) Com_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) bufferSizePtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/******************************************************************************/
/*
 * Brief               This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 *                     Each call to this function provides the next part of the I-PDU data unless retry->TpDataState
 *                     is TP_DATARETRY. In this case the function restarts to copy the data beginning at the offset
 *                     from the current position indicated by retry->TxTpDataCnt. The size of the remaining data is
 *                     written to the position indicated by availableDataPtr.
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id: Identification of the transmitted I-PDU.
 *                     retry: This parameter is used to acknowledge transmitted data or to retransmit data after
 * transmission problems. Param-Name[out]     availableDataPtr: Indicates the remaining number of bytes that are
 * available in the upper layer module's Tx buffer. availableDataPtr can be used by TP modules that support dynamic
 * payload lengths (e.g. FrIsoTp) to determine the size of the following CFs. Param-Name[in/out]  None Return
 * BufReq_ReturnType BUFREQ_OK: Data has been copied to the transmit buffer completely as requested. BUFREQ_E_BUSY:
 * Request could not be fulfilled, because the required amount of Tx data is not available. The lower layer module may
 * retry this call later on. No data has been copied. BUFREQ_E_NOT_OK: Data has not been copied. Request failed.
 */
/******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
extern FUNC(BufReq_ReturnType, COM_CODE) Com_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, COM_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) availableDataPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#endif /*end of COM_CBK_H*/
