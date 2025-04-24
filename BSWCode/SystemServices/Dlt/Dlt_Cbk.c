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
**  DESCRIPTION : Callback functions define of Dlt                            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#include "Dlt_Cbk.h"
#include "Dlt_internal.h"
#include "Dlt.h"
#include "SchM_Dlt.h"

typedef struct
{
    uint32 ApplicationID;
    uint32 ContextID;
    uint32 ServiceID;
    boolean MSBF;
    boolean UEH;
    uint16 RxIndex;
    uint32 SessionID;
} Dlt_DealRxDataTypes;

/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
typedef P2FUNC(
    Std_ReturnType,
    DCM_APPL_CODE,
    Dlt_ServiceDeal)(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */

#if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) && (DLT_RXPDU_NUM > 0))
#define DLT_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"
VAR(Dlt_RxStatusTypes, AUTOMATIC) Dlt_RxStatus[DLT_RXPDU_NUM];
#define DLT_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dlt_MemMap.h"

#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"

static FUNC(uint16, DLT_APPL_CODE) Dlt_GetRxIndexByRxPduId(PduIdType id);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_SubDealRxData(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(void, DLT_APPL_CODE) Dlt_ServicePrepareResponseHeaderData(
    uint16* SendOffset,
    uint16 ChannelIndex,
    uint16 Messagelength,
    Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_ServiceSendResponseData(uint16 ChannelIndex, uint16 Messagelength);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceNotSupport(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetLogLevel(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetTraceStatus(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetLogInfo(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetDefaultLogLevel(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceStoreConfiguration(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceResetToFactoryDefault(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetMessageFiltering(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetDefaultLogLevel(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetDefaultTraceStatus(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetSoftwareVersion(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetDefaultTraceStatus(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetLogChannelNames(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetTraceStatus(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetLogChannelAssignment(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetLogChannelThreshold(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetLogChannelThreshold(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSyncTimeStamp(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
#if (DLT_INJECTION_SUPPORT == STD_ON)
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceDealWithInjection(uint16 offset, uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData);
#endif

#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"
#endif

#define DLT_START_SEC_DLTRXINDICATION_CALLBACK_CODE
#include "Dlt_MemMap.h"
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
FUNC(void, DLT_APPL_CODE)
Dlt_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) PduInfoPtr)
{
#if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) && (DLT_BUFFER_MAX_LENGTH > 0) && (DLT_RXPDU_NUM > 0))
    Std_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Report uninitialized development errors to the DET module */
        DLT_DETREPORT(DLT_API_ID_RXINDICATION, DLT_E_UNINIT);
    }
    else
#endif
    {
        /* The receiving index number is extracted according by the receiving PDU identifier */
        uint16 RxIndex = Dlt_GetRxIndexByRxPduId(RxPduId);
#if (1u < DLT_RXPDU_NUM)
        /* Check if the received index number is valid */
        if (DLT_RXPDU_NUM != RxIndex)
#endif /* 1u < DLT_RXPDU_NUM */
        {
            Dlt_RxStatusTypes* RxStatusPtr = &(Dlt_RxStatus[RxIndex]);
            /* Parse the received message and reply the response message by receiving the index number */
            ret = Dlt_DealRxData(RxIndex, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
            /* Check if the message receiving task failed and the receiving status is not in use */
            if ((ret != E_OK) && (RxStatusPtr->Used == FALSE))
            {
                SchM_Enter_Dlt_RxStatus();
                /* Set the status of the given receive index number to used */
                RxStatusPtr->Used = TRUE;
                /* Set the "NeedDeal" status to TRUE for the given receive index number */
                RxStatusPtr->NeedDeal = TRUE;
                /* Set the data length for the given receive index number */
                RxStatusPtr->RxLength = PduInfoPtr->SduLength;
                /* Set the offset for the given receive index number */
                RxStatusPtr->RxOffset = 0;
                /* Stores the received message to the specified buffer queue */
                Dlt_MemCopy(RxStatusPtr->RxBuffer, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
                SchM_Exit_Dlt_RxStatus();
            }
        }
    }
#else
    DLT_UNUSED(RxPduId);
    DLT_UNUSED(PduInfoPtr);
#endif
}
#define DLT_STOP_SEC_DLTRXINDICATION_CALLBACK_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_DLTTRIGGERTRANSMIT_CALLBACK_CODE
#include "Dlt_MemMap.h"
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
FUNC(Std_ReturnType, DLT_APPL_CODE)
Dlt_TriggerTransmit(PduIdType TxPduId, P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) PduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Report uninitialized development errors to the DET module */
        DLT_DETREPORT(DLT_API_ID_TRIGGERTRANSMIT, DLT_E_UNINIT);
    }
    else
#endif
    {
        /* The channel index number is extracted according by the transmit PDU identifier */
        uint16 ChannelIndex = Dlt_GetChannelIndexByTxPduId(TxPduId);
#if (1u < DLT_SWC_NUM)
        /* Check if the channel index number is valid */
        if (DLT_CHANNEL_NUM != ChannelIndex)
#endif /* 1u < DLT_SWC_NUM */
        {
#if (DLT_CHANNEL_NUM > 0)
            /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
            const Dlt_LogChannelType* logChannelPtr =
                (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex]);
            /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
            PduIdType PduId = logChannelPtr->DltTxPdu->DltTxPduRPduId;
            boolean TxPduUsesTp = logChannelPtr->DltTxPdu->DltTxPduUsesTp;
            if (TxPduUsesTp == FALSE)
            {
                ret = PduR_DltTransmit(PduId, PduInfoPtr);
            }
#endif
        }
    }
    return ret;
}
#define DLT_STOP_SEC_DLTTRIGGERTRANSMIT_CALLBACK_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_DLTTXCONFIRMATION_CALLBACK_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               The lower layer communication interface module confirms the transmission of a PDU, or the failure
 * to transmit a PDU. ServiceId           0x40 Sync/Async          Synchronous Reentrancy          Reentrant for
 * different PduIds. Non reentrant for the same PduId. Param-Name[in]      TxPduId:ID of the PDU that has been
 * transmitted. result:E_OK: The PDU was transmitted. E_NOT_OK: Transmission of the PDU failed. Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
#ifdef DLT_TX_CONFIRM_R19_USED
FUNC(void, DLT_APPL_CODE) Dlt_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
#else
FUNC(void, DLT_APPL_CODE) Dlt_TxConfirmation(PduIdType TxPduId)
#endif
{
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Report uninitialized development errors to the DET module */
        DLT_DETREPORT(DLT_API_ID_TXCONFIRMATION, DLT_E_UNINIT);
    }
    else
#endif
    {
        /* The channel index number is extracted according by the transmit PDU identifier */
        uint16 ChannelIndex = Dlt_GetChannelIndexByTxPduId(TxPduId);
        /* Check if the channel index number is valid */
        if (DLT_CHANNEL_NUM != ChannelIndex)
        {
#if (DLT_CHANNEL_NUM > 0)
            Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);

            SchM_Enter_Dlt_MsgChannel();
            /* Check if the last transmission status of the given channel is not confirmation */
            if (ChannelPtr->LastSendStatus == DLT_SEND_NOT_CONFIRMATION)
            {
#ifdef DLT_TX_CONFIRM_R19_USED
                if (result == E_OK)
#endif
                {
                    /* Set the last transmission status of the given channel to waiting for transmission */
                    ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
                    /* Re-calculate the usable length of the buffer for a given channel */
                    ChannelPtr->UnusedLength += ChannelPtr->LastSendLength;
                    /* Reset the access location for the buffer of a given channel */
                    Dlt_MoveReadIndex(ChannelIndex, ChannelPtr->LastSendLength);
                }
#ifdef DLT_TX_CONFIRM_R19_USED
                else
                {
                    /* Set the last transmission status of the given channel to send need retry */
                    ChannelPtr->LastSendStatus = DLT_SEND_NEED_RETRY;
                }
#endif
            }
            /* Check if the last transmission status of the given channel has been sent */
            else if (ChannelPtr->LastSendStatus == DLT_CONTROL_SENDED)
            {
                /* Set the last transmission status of the given channel to waiting for transmission */
                ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
            }
            else
            {
                /*idle*/
            }
            SchM_Exit_Dlt_MsgChannel();
#endif
        }
    }
}
#define DLT_STOP_SEC_DLTTXCONFIRMATION_CALLBACK_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_DLTTPTXCONFIRMATION_CALLBACK_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               This function is called after the I-PDU has been transmitted on its network,
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
FUNC(void, DLT_APPL_CODE) Dlt_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Report uninitialized development errors to the DET module */
        DLT_DETREPORT(DLT_API_ID_TPTXCONFIRMATION, DLT_E_UNINIT);
    }
    else
#endif
    {
        /* The channel index number is extracted according by the transmit PDU identifier */
        uint16 ChannelIndex = Dlt_GetChannelIndexByTxPduId(id);
        /* Check if the channel index number is valid */
        if (DLT_CHANNEL_NUM != ChannelIndex)
        {
#if (DLT_CHANNEL_NUM > 0)
            Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);

            SchM_Enter_Dlt_MsgChannel();
            /* Check if the last transmission status of the given channel is not confirmation */
            if (ChannelPtr->LastSendStatus == DLT_SEND_NOT_CONFIRMATION)
            {
                if (result == E_OK)
                {
                    /* Set the last transmission status of the given channel to waiting for transmission */
                    ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
                    /* Re-calculate the usable length of the buffer for a given channel */
                    ChannelPtr->UnusedLength += ChannelPtr->LastSendLength;
                    /* Reset the access location for the buffer of a given channel */
                    Dlt_MoveReadIndex(ChannelIndex, ChannelPtr->LastSendLength);
                }
                else
                {
                    /* Set the last transmission status of the given channel to send need retry */
                    ChannelPtr->LastSendStatus = DLT_SEND_NEED_RETRY;
                }
            }
            /* Check if the last transmission status of the given channel has been sent */
            else if (ChannelPtr->LastSendStatus == DLT_CONTROL_SENDED)
            {
                /* Set the last transmission status of the given channel to waiting for transmission */
                ChannelPtr->LastSendStatus = DLT_WAIT_SEND;
            }
            else
            {
                /*idle*/
            }
            SchM_Exit_Dlt_MsgChannel();
#endif
        }
    }
}
#define DLT_STOP_SEC_DLTTPTXCONFIRMATION_CALLBACK_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_DLTCOPYTXDATA_CALLBACK_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 *                     Each call to this function provides the next part of the I-PDU data unless
 *                     retry->TpDataState is TP_DATARETRY. In this case the function restarts to copy the
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
FUNC(BufReq_ReturnType, DLT_APPL_CODE)
Dlt_CopyTxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
    P2CONST(RetryInfoType, AUTOMATIC, DLT_APPL_CONST) retry,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) availableDataPtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Report uninitialized development errors to the DET module */
        DLT_DETREPORT(DLT_API_ID_COPYTXDATA, DLT_E_UNINIT);
    }
    /* Check if the passed parameter is null pointer */
    else if ((availableDataPtr == NULL_PTR) || (retry != NULL_PTR) || (info == NULL_PTR))
    {
        /* Report null pointer development error to the DET module */
        DLT_DETREPORT(DLT_API_ID_COPYTXDATA, DLT_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* The channel index number is extracted according by the transmit PDU identifier */
        uint16 ChannelIndex = Dlt_GetChannelIndexByTxPduId(id);
        /* Check if the channel index number is valid */
        if (DLT_CHANNEL_NUM != ChannelIndex)
        {
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
            Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
            /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
            const Dlt_LogChannelType* logChannelPtr =
                (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex]);
            /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
            /* Check if the last transmission status of the given channel is not confirmation */
            if (ChannelPtr->LastSendStatus == DLT_SEND_NOT_CONFIRMATION)
            {
                SchM_Enter_Dlt_MsgChannel();
                if (info->SduLength == 0u)
                {
                    *availableDataPtr = (uint16)ChannelPtr->LastSendLength - ChannelPtr->MessageSendOffset;
                    ret = BUFREQ_OK;
                }
                else
                {
                    if ((info->SduLength + ChannelPtr->MessageSendOffset) <= (uint16)ChannelPtr->LastSendLength)
                    {
                        uint32 SendOffset;
                        /* PRQA S 4304,3344++ */ /* MISRA Rule 10.5,Rule 14.4 */
                        if ((uint32)((ChannelPtr->ReadIndex + (uint32)ChannelPtr->MessageSendOffset)
                                     >= logChannelPtr->DltLogChannelBufferSize))
                        /* PRQA S 4304,3344-- */ /* MISRA Rule 10.5,Rule 14.4 */
                        {
                            SendOffset = (uint32)((ChannelPtr->ReadIndex + (uint32)ChannelPtr->MessageSendOffset)
                                                  - logChannelPtr->DltLogChannelBufferSize);
                        }
                        else
                        {
                            SendOffset = ChannelPtr->ReadIndex + ChannelPtr->MessageSendOffset;
                        }
                        Dlt_Tx_CopyTxDataToSendBuffer(ChannelIndex, info->SduDataPtr, SendOffset, info->SduLength);
                        ChannelPtr->MessageSendOffset += info->SduLength;
                        *availableDataPtr = (uint16)ChannelPtr->LastSendLength - ChannelPtr->MessageSendOffset;
                        ret = BUFREQ_OK;
                    }
                }
                SchM_Exit_Dlt_MsgChannel();
            }
            /* Check if the last transmission status of the given channel has been sent */
            else if (ChannelPtr->LastSendStatus == DLT_CONTROL_SENDED)
            {
                SchM_Enter_Dlt_MsgChannel();
                if (info->SduLength == 0u)
                {
                    *availableDataPtr = ChannelPtr->LastControlSendLength - ChannelPtr->ControlSendOffset;
                    ret = BUFREQ_OK;
                }
                else
                {
                    if ((info->SduLength + ChannelPtr->ControlSendOffset) <= ChannelPtr->LastControlSendLength)
                    {
                        Dlt_MemCopy(
                            info->SduDataPtr,
                            &(ChannelPtr->SendControlBuffer[ChannelPtr->ControlSendOffset]),
                            info->SduLength);
                        ChannelPtr->ControlSendOffset += info->SduLength;
                        *availableDataPtr = ChannelPtr->LastControlSendLength - ChannelPtr->ControlSendOffset;
                        ret = BUFREQ_OK;
                    }
                }
                SchM_Exit_Dlt_MsgChannel();
            }
            else
            {
                /*idle*/
            }
#endif
        }
    }
    DLT_UNUSED(retry);
    return ret;
}
#define DLT_STOP_SEC_DLTCOPYTXDATA_CALLBACK_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_DLTSTARTOFRECEPTION_CALLBACK_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               This function is called at the start of receiving an N-SDU. The N-SDU might be fragmented into
 *                     multiple N-PDUs (FF with one or more following CFs) or might consist of a single N-PDU (SF).
 *                     The service shall provide the currently available maximum buffer size when invoked with
 * TpSduLength equal to 0. ServiceId           0x46 Sync/Async          Synchronous Reentrancy          Reentrant
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
FUNC(BufReq_ReturnType, DLT_APPL_CODE)
Dlt_StartOfReception(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
    PduLengthType TpSduLength,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) && (DLT_BUFFER_MAX_LENGTH > 0) && (DLT_RXPDU_NUM > 0))
    BufReq_ReturnType ret;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    ret = BUFREQ_E_NOT_OK;
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Report uninitialized development errors to the DET module */
        DLT_DETREPORT(DLT_API_ID_STARTOFRECEPTION, DLT_E_UNINIT);
    }
    /* Check if the passed parameter is null pointer */
    else if (NULL_PTR == bufferSizePtr)
    {
        /* Report null pointer development error to the DET module */
        DLT_DETREPORT(DLT_API_ID_STARTOFRECEPTION, DLT_E_UNINIT);
    }
    else
#endif
    {
        /* The receiving index number is extracted according by the receiving PDU identifier */
        uint16 RxIndex = Dlt_GetRxIndexByRxPduId(id);
#if (1u < DLT_RXPDU_NUM)
        /* Check if the Rx index number is valid */
        if (DLT_RXPDU_NUM != RxIndex)
#endif /* 1u < DLT_RXPDU_NUM */
        {
            Dlt_RxStatusTypes* RxStatusPtr = &(Dlt_RxStatus[RxIndex]);
            if (TpSduLength > DLT_BUFFER_MAX_LENGTH)
            {
                ret = BUFREQ_E_OVFL;
            }
            else if (RxStatusPtr->Used == TRUE)
            {
                ret = BUFREQ_E_BUSY;
            }
            else
            {
                SchM_Enter_Dlt_RxStatus();
                *bufferSizePtr = DLT_BUFFER_MAX_LENGTH;
                RxStatusPtr->RxLength = TpSduLength;
                if (info != NULL_PTR)
                {
                    Dlt_MemCopy(RxStatusPtr->RxBuffer, info->SduDataPtr, info->SduLength);
                    RxStatusPtr->RxOffset = info->SduLength;
                }
                else
                {
                    RxStatusPtr->RxOffset = 0;
                }
                RxStatusPtr->Used = TRUE;
                SchM_Exit_Dlt_RxStatus();
                ret = BUFREQ_OK;
            }
        }
    }
    return ret;
#else
    DLT_UNUSED(id);
    DLT_UNUSED(info);
    DLT_UNUSED(TpSduLength);
    DLT_UNUSED(bufferSizePtr);
    return BUFREQ_OK;
#endif
}
#define DLT_STOP_SEC_DLTSTARTOFRECEPTION_CALLBACK_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_DLTTPRXINDICATION_CALLBACK_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               Called after an I-PDU has been received via the TP API, the result indicates whether the
 * transmission was successful or not. ServiceId           0x45 Sync/Async          Synchronous Reentrancy Reentrant
 * Param-Name[in]      id:Identification of the received I-PDU.
 *                     result:Result of the reception.
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 */
FUNC(void, DLT_APPL_CODE) Dlt_TpRxIndication(PduIdType id, Std_ReturnType result)
{
#if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) && (DLT_BUFFER_MAX_LENGTH > 0) && (DLT_RXPDU_NUM > 0))
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Report uninitialized development errors to the DET module */
        DLT_DETREPORT(DLT_API_ID_TPRXINDICATION, DLT_E_UNINIT);
    }
    else
#endif
    {
        /* The receiving index number is extracted according by the receiving PDU identifier */
        uint16 RxIndex = Dlt_GetRxIndexByRxPduId(id);
#if (1u < DLT_RXPDU_NUM)
        /* Check if the Rx index number is valid */
        if (DLT_RXPDU_NUM != RxIndex)
#endif /* 1u < DLT_RXPDU_NUM */
        {
            Dlt_RxStatusTypes* RxStatusPtr = &(Dlt_RxStatus[RxIndex]);
            if ((RxStatusPtr->Used == TRUE) && (RxStatusPtr->RxOffset == RxStatusPtr->RxLength) && (result == E_OK))
            {
                /* If the conditions are met, the received messages from the DLT are parsed */
                Std_ReturnType ret = Dlt_DealRxData(RxIndex, RxStatusPtr->RxBuffer, RxStatusPtr->RxLength);
                SchM_Enter_Dlt_RxStatus();
                if (ret == E_OK)
                {
                    /* The DLT received message is parsed */
                    RxStatusPtr->Used = FALSE;
                    RxStatusPtr->NeedDeal = FALSE;
                }
                else
                {
                    /* The parsing of DLT received messages is suspended, delayed for reprocessing */
                    RxStatusPtr->NeedDeal = TRUE;
                }
                SchM_Exit_Dlt_RxStatus();
            }
            SchM_Enter_Dlt_RxStatus();
            /* Check if the DLT failed to receive messages */
            if ((RxStatusPtr->NeedDeal == FALSE) && (result == E_NOT_OK))
            {
                /* The message receiving action is terminated. */
                RxStatusPtr->Used = FALSE;
            }
            SchM_Exit_Dlt_RxStatus();
        }
    }
#else
    DLT_UNUSED(id);
    DLT_UNUSED(result);
#endif
}
#define DLT_STOP_SEC_DLTTPRXINDICATION_CALLBACK_CODE
#include "Dlt_MemMap.h"

#define DLT_START_SEC_DLTCOPYRXDATA_CALLBACK_CODE
#include "Dlt_MemMap.h"
/*
 * Brief               This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper
 * layer. Each call to this function provides the next part of the I-PDU data. The size of the remaining buffer is
 * written to the position indicated by bufferSizePtr. ServiceId           0x44 Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      id:Identification of the I-PDU.
 *                     info:Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 *                     An SduLength of 0 can be used to query the current amount of available buffer in the upper layer
 * module. In this case, the SduDataPtr may be a NULL_PTR. Param-Name[out]     bufferSizePtr:Available receive buffer
 * after data has been copied. Param-Name[in/out]  N/A Return              BufReq_ReturnType:BUFREQ_OK: Data copied
 * successfully BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 */
FUNC(BufReq_ReturnType, DLT_APPL_CODE)
Dlt_CopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
#if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) && (DLT_BUFFER_MAX_LENGTH > 0) && (DLT_RXPDU_NUM > 0))
    BufReq_ReturnType ret = BUFREQ_E_NOT_OK;
#if (STD_ON == DLT_DEV_ERROR_DETECT)
    /* Check if the DLT module has been initialized */
    if (DLT_STATE_UNINIT == Dlt_ModeState)
    {
        /* Report uninitialized development errors to the DET module */
        DLT_DETREPORT(DLT_API_ID_COPYRXDATA, DLT_E_UNINIT);
    }
    /* Check if the passed parameter is null pointer */
    else if ((NULL_PTR == bufferSizePtr) || (NULL_PTR == info))
    {
        /* Report null pointer development error to the DET module */
        DLT_DETREPORT(DLT_API_ID_COPYRXDATA, DLT_E_UNINIT);
    }
    else
#endif
    {
        /* The receiving index number is extracted according by the receiving PDU identifier */
        uint16 RxIndex = Dlt_GetRxIndexByRxPduId(id);
#if (1u < DLT_RXPDU_NUM)
        /* Check if the Rx index number is valid */
        if (DLT_RXPDU_NUM != RxIndex)
#endif /* 1u < DLT_RXPDU_NUM */
        {
            Dlt_RxStatusTypes* RxStatusPtr = &(Dlt_RxStatus[RxIndex]);
            SchM_Enter_Dlt_RxStatus();
            /* Check if the status of given receive channel is available */
            if (RxStatusPtr->Used == TRUE)
            {
                /* Calculate the remaining length of the receive buffer */
                *bufferSizePtr = DLT_BUFFER_MAX_LENGTH - RxStatusPtr->RxOffset;
                /* Check if the message length for given receiving channel is within valid range */
                if ((RxStatusPtr->RxOffset + info->SduLength) <= RxStatusPtr->RxLength)
                {
                    /* Copy the received PDU data into the buffer of the given receiving channel */
                    Dlt_MemCopy(&(RxStatusPtr->RxBuffer[RxStatusPtr->RxOffset]), info->SduDataPtr, info->SduLength);
                    /* Re-calculate the buffer offset position for given receive channel */
                    RxStatusPtr->RxOffset += info->SduLength;
                    ret = BUFREQ_OK;
                }
            }
            SchM_Exit_Dlt_RxStatus();
        }
    }
    return ret;
#else
    DLT_UNUSED(id);
    DLT_UNUSED(info);
    DLT_UNUSED(bufferSizePtr);
    return BUFREQ_OK;
#endif
}
#define DLT_STOP_SEC_DLTCOPYRXDATA_CALLBACK_CODE
#include "Dlt_MemMap.h"

#if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h"
static FUNC(uint16, DLT_APPL_CODE) Dlt_GetRxIndexByRxPduId(PduIdType id)
{
    uint16 RxIndex = 0;
#if (1u < DLT_RXPDU_NUM)
    for (; RxIndex < DLT_RXPDU_NUM; RxIndex++)
#endif /* 1u < DLT_RXPDU_NUM */
    {
        /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
        const Dlt_RxPduType* RxPduPtr = (Dlt_RxPduType*)&(Dlt_ConfigPtr->RxPdu[RxIndex]);
        /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
        if (RxPduPtr->DltRxPduId == id)
        {
#if (1u < DLT_RXPDU_NUM)
            break;
#endif /* 1u < DLT_RXPDU_NUM */
        }
    }
    return RxIndex;
}

FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_DealRxData(uint16 RxIndex, const uint8* Data, uint16 Length)
{
    Std_ReturnType ret = E_OK;
    uint16 offset = 0;
    uint8 HeaderType;
    uint8 MessageCounter;
    uint16 MessageLength;
    uint32 ECUID = 0;
    uint32 SessionID = 0;
    uint32 Timestamp;
    boolean UEH;
    boolean MSBF;
    boolean WEID;
    boolean WSID;
    boolean WithTMS;
    uint8 VERS;
    uint8 MessageInfo;
    uint8 NumberOfArguments;
    uint32 ApplicationID = 0;
    uint32 ContextID = 0;
    Dlt_DealRxDataTypes DealRxData;

    /* Byte 0: HTYP (Header Type) */
    HeaderType = Data[offset];
    /* PRQA S 4340,4394++ */ /* MISRA Rule 10.5, Rule 10.8 */
    UEH = (boolean)(HeaderType & 0x01u);
    MSBF = (boolean)((HeaderType & 0x02u) >> 1u);
    WEID = (boolean)((HeaderType & 0x04u) >> 2u);
    WSID = (boolean)((HeaderType & 0x08u) >> 3u);
    WithTMS = (boolean)((HeaderType & 0x10u) >> 4u);
    /* PRQA S 4340,4394-- */ /* MISRA Rule 10.5, Rule 10.8 */
    VERS = HeaderType >> 5u;
    offset++;
    /* Byte 1: MCNT (Message Counter) */
    MessageCounter = Data[offset];
    DLT_UNUSED(MessageCounter);
    offset++;
    /* Byte 2-3: LEN (Length) */
    Dlt_CopyArrayToIntBigEndian(&MessageLength, &(Data[offset]));
    offset += 2u;
    /* Check if the standard message header support the "ECU ID" field */
    if (WEID == TRUE)
    {
        /* Byte 4-7: ECU (ECU ID) */
        Dlt_CopyArrayToLongBigEndian(&ECUID, &(Data[offset]));
    }
    offset += 4u;
    /* Check if the standard message header support the "Session ID" field */
    if (WSID == TRUE)
    {
        /* Byte 8-11: SEID (Session ID) */
        Dlt_CopyArrayToLongBigEndian(&SessionID, &(Data[offset]));
    }
    offset += 4u;
    /* Check if the standard message header support the "Timestamp" field */
    if (WithTMS == TRUE)
    {
        /* Byte 12-15: TMSP (Timestamp) */
        Dlt_CopyArrayToLongBigEndian(&Timestamp, &(Data[offset]));
    }
    offset += 4u;
    /*The VERS bits shall always be set to '001'. then check MessageCounter and Length*/
    if ((VERS == 1u) && (MessageLength <= Length))
    {
        DealRxData.UEH = UEH;
        DealRxData.MSBF = MSBF;
        DealRxData.RxIndex = RxIndex;
        DealRxData.ApplicationID = ApplicationID;
        DealRxData.ContextID = ContextID;
        DealRxData.SessionID = SessionID;
        /* Check if the "UEH" field of the standard message header is "1" */
        if (UEH == TRUE)
        {
            /* Byte 16: MSIN (Message Info) */
            MessageInfo = Data[offset];
            /* PRQA S 4340,4394++ */ /* MISRA Rule 10.5, Rule 10.8 */
            boolean VERB = (boolean)(MessageInfo & 0x01u);
            uint8 MSTP = (uint8)(MessageInfo & 0x0Eu) >> 1u;
            uint8 MTIN = (uint8)(MessageInfo & 0xF0u) >> 4u;
            /* PRQA S 4340,4394-- */ /* MISRA Rule 10.5, Rule 10.8 */
            offset++;
            /* Byte 17: NOAR (Number of Arguments) */
            NumberOfArguments = Data[offset];
            DLT_UNUSED(NumberOfArguments);
            offset++;
            /* Byte 18-21: APID (Application ID) */
            Dlt_CopyArrayToLongBigEndian(&ApplicationID, &(Data[offset]));
            offset += 4u;
            /* Byte 22-25: CTID (Context ID) */
            Dlt_CopyArrayToLongBigEndian(&ContextID, &(Data[offset]));
            offset += 4u;
            /* Check if the extended headers support verbose mode and request messages for control command */
            if ((VERB == FALSE) && (MSTP == (uint8)DLT_TYPE_CONTROL) && (MTIN == (uint8)DLT_CONTROL_REQUEST))
            {
                DealRxData.ApplicationID = ApplicationID;
                DealRxData.ContextID = ContextID;
                DealRxData.SessionID = SessionID;
                /* Perform parsing action for control command request messages */
                ret = Dlt_SubDealRxData(offset, Data, Length, DealRxData);
            }
        }
        else
        {
            /* Perform parsing action for control command request messages */
            ret = Dlt_SubDealRxData(offset, Data, Length, DealRxData);
        }
    }
    return ret;
}

/* Create standard header for control command to send messages for given channel in DLT module */
FUNC(void, DLT_CODE)
Dlt_CreateControlStandardHeader(const Dlt_CreateStandardHeaderInfoTypes* CreateStandardHeader, uint8* Dest)
{
    uint8 HeaderType = 0;
    /* Specify field fill "UEH" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode == TRUE)
    {
        HeaderType = 0x01u;
    }
    /* Specify field fill "MSBF" to the DLT message buffer */
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    HeaderType |= 0x02u;
#endif
    /* Specify field fill "WEID" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseEcuId == TRUE)
    {
        HeaderType |= 0x04u;
        uint32 EcuId = Dlt_GetEcuId();
        Dlt_CopyLongToArrayBigEndian(EcuId, &(Dest[4]));
    }
    /* Specify field fill "WSID" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseSessionID == TRUE)
    {
        HeaderType |= 0x08u;
        Dlt_CopyLongToArrayBigEndian(CreateStandardHeader->SessionId, &(Dest[8]));
    }
    /* Specify field fill "WTMS" to the DLT message buffer */
    if (Dlt_ConfigPtr->Protocol->DltHeaderUseTimestamp == TRUE)
    {
        HeaderType |= 0x10u;
        Dlt_CopyLongToArrayBigEndian(CreateStandardHeader->timestamp, &(Dest[12]));
    }
    /* Specify field fill "VERS" to the DLT message buffer */
    HeaderType |= 0x20u;
    /* Specify field fill "Header Type" to the DLT message buffer */
    Dest[0u] = HeaderType;
    /* Specify field fill "MCNT" to the DLT message buffer */
    Dest[1u] = Dlt_Channel[CreateStandardHeader->ChannelIndex].MessageCounter;
    /* Specify field fill "LEN" to the DLT message buffer */
    Dlt_CopyIntToArrayBigEndian(CreateStandardHeader->Messagelength, &(Dest[2]));
}

/* Create extended header for control command to send messages for given channel in DLT module */
FUNC(void, DLT_CODE)
Dlt_CreateControlExtendedHeader(const Dlt_CreateExtendedHeaderInfoTypes* CreateExtendedHeader, uint8* Dest)
{
    uint8 MessageInfo = 0;
    /* Check if the extended header is support verbose mode */
    if (Dlt_ConfigPtr->Protocol->DltUseVerboseMode == TRUE)
    {
        MessageInfo = 0x01u;
        if (NULL_PTR != CreateExtendedHeader->logInfo)
        {
            /* Specify field fill "NOAR" to the DLT message buffer */
            Dest[1u] = (uint8)CreateExtendedHeader->logInfo->argCount;
        }
    }
    /* Check whether the DLT message type is "TRACE" data */
    if (CreateExtendedHeader->BufferDataType == DLT_TRACE_DATA)
    {
        if (NULL_PTR != CreateExtendedHeader->traceInfo)
        {
            /* Specify field fill "MSTP" to the DLT message buffer */
            MessageInfo |= (uint8)((uint8)CreateExtendedHeader->traceInfo->options.Dlt_Bits.message_type << 1u);
            /* Specify field fill "MTIN" to the DLT message buffer */
            MessageInfo |= (uint8)((uint8)CreateExtendedHeader->traceInfo->traceInfo << 4u);
        }
    }
    /* Check whether the DLT message type is "CONTROL" data */
    else if (CreateExtendedHeader->BufferDataType == DLT_CONTROL_DATA)
    {
        /* Specify field fill "MSTP" to the DLT message buffer */
        MessageInfo |= (uint8)((uint8)DLT_TYPE_CONTROL << 1u);
        /* Specify field fill "MTIN" to the DLT message buffer */
        MessageInfo |= (uint8)((uint8)DLT_CONTROL_RESPONSE << 4u);
    }
    /* Check whether the DLT message type is "LOG" data */
    else
    {
        if (NULL_PTR != CreateExtendedHeader->logInfo)
        {
            /* Specify field fill "MSTP" to the DLT message buffer */
            MessageInfo |= (uint8)((uint8)CreateExtendedHeader->logInfo->options.Dlt_Bits.message_type << 1u);
            /* Specify field fill "MTIN" to the DLT message buffer */
            MessageInfo |= (uint8)((uint8)CreateExtendedHeader->logInfo->logLevel << 4u);
        }
    }
    /* Specify field fill "MSIN" to the DLT message buffer */
    Dest[0u] = MessageInfo;
    /* Specify field fill "APID" to the DLT message buffer */
    Dlt_CopyLongToArrayBigEndian(CreateExtendedHeader->SwcContext.SwcApplicationId, &(Dest[2]));
    /* Specify field fill "CTID" to the DLT message buffer */
    Dlt_CopyLongToArrayBigEndian(CreateExtendedHeader->SwcContext.SwcContextId, &(Dest[6]));
}

/* Load the prepare header information for the given channel service response message */
static FUNC(void, DLT_APPL_CODE) Dlt_ServicePrepareResponseHeaderData(
    uint16* SendOffset,
    uint16 ChannelIndex,
    uint16 Messagelength,
    Dlt_DealRxDataTypes DealRxData)
{
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    Dlt_CreateExtendedHeaderInfoTypes CreateExtendedHeaderInfo;
    Dlt_CreateStandardHeaderInfoTypes CreateStandardHeaderInfo;

    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);

    uint8* SendControlBufferPtr;

    uint16 tx_offset = 0;

    /* Create standard header for the response message of the DLT given log channel */
    CreateStandardHeaderInfo.ChannelIndex = ChannelIndex;
    CreateStandardHeaderInfo.Messagelength = Messagelength;
    CreateStandardHeaderInfo.SessionId = DealRxData.SessionID;
    CreateStandardHeaderInfo.timestamp = Dlt_GetTimeElapsed();
    SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);
    Dlt_CreateControlStandardHeader(&CreateStandardHeaderInfo, SendControlBufferPtr);
    tx_offset = DLT_STANDARD_HEADER_LENGTH;
    /* Check if the DLT message extension header is support verbose mode */
    if (Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode == TRUE)
    {
        Dlt_SwcContextType SwcContext;
        /* Create extended header for the response message of the DLT given log channel */
        CreateExtendedHeaderInfo.ChannelIndex = ChannelIndex;
        SwcContext.SwcApplicationId = DealRxData.ApplicationID;
        SwcContext.SwcContextId = DealRxData.ContextID;
        CreateExtendedHeaderInfo.BufferDataType = DLT_CONTROL_DATA;
        CreateExtendedHeaderInfo.logInfo = NULL_PTR;
        CreateExtendedHeaderInfo.traceInfo = NULL_PTR;
        CreateExtendedHeaderInfo.SwcContext = SwcContext;
        Dlt_CreateControlExtendedHeader(&CreateExtendedHeaderInfo, &SendControlBufferPtr[tx_offset]);
        tx_offset += DLT_EXTENED_HEADER_LENGTH;
    }
    /* Load "ServiceID" for the response message of the DLT given log channel */
    Dlt_CopyLongToArrayBigEndian(DealRxData.ServiceID, &SendControlBufferPtr[tx_offset]);
    tx_offset += 4u;
    (*SendOffset) = tx_offset;
#endif
}

static FUNC(Std_ReturnType, DLT_APPL_CODE) Dlt_ServiceSendResponseData(uint16 ChannelIndex, uint16 Messagelength)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    PduInfoType PduInfoPtr;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
    const Dlt_LogChannelType* logChannelPtr =
        (Dlt_LogChannelType*)&(Dlt_ConfigPtr->LogOutput->LogChannel[ChannelIndex]);
    /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
    /* Check if the PDU sent by the DLT if support TP mode */
    if (logChannelPtr->DltTxPdu->DltTxPduUsesTp == TRUE)
    {
        /* Attach the data buffer of the given PDU */
        PduInfoPtr.SduDataPtr = NULL_PTR;
    }
    else
    {
        /* Attach the data buffer of the given PDU */
        PduInfoPtr.SduDataPtr = ChannelPtr->SendControlBuffer;
    }
    /* Set the data length of given PDU */
    PduInfoPtr.SduLength = Messagelength;
    /* Call "PduR_DltTransmit" to start transmit task at the transport layer */
    ret = PduR_DltTransmit(logChannelPtr->DltTxPdu->DltTxPduRPduId, &PduInfoPtr);
    if (ret == E_OK)
    {
        SchM_Enter_Dlt_MsgChannel();
        /* The message for setting the given log channel has been sent */
        ChannelPtr->LastSendStatus = DLT_CONTROL_SENDED;
        /* Clear the send buffer offset for given log channel */
        ChannelPtr->ControlSendOffset = 0U;
        /* Set the last message length sent for given log channel */
        ChannelPtr->LastControlSendLength = Messagelength;
        SchM_Exit_Dlt_MsgChannel();
    }
#endif
    return ret;
}

/*
 * Brief               ServiceID == 0u and ServiceID > 0x24u Service Not Support
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceNotSupport(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;

    /* "ServiceNotSupport" Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4)} */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1) */

    DLT_UNUSED(Data);
    DLT_UNUSED(offset);
    DLT_UNUSED(Length);

#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))

    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0U;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_SERVICE_NOT_SUPPORT_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = (uint8)DLT_STATUS_NOT_SUPPORTED;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               ServiceID == 1u Service Set Log Level
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetLogLevel(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    uint32 applicationId;
    uint32 contextId;
    sint8 newLogLevel;
    uint16 loffset = offset;

    /* "SetLogLevel "Request message format: StandardHeader(16), ExternHeader(10), Payload(17) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),{ApplicationID(4),ContextID(4),newLogLevel(1),Reserved(4)} */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1) */

    /* Check if the DLT message format is support big-endian mode */
    if (DealRxData.MSBF == TRUE)
    {
        /* Extract the "applicationId" from the DLT service control message */
        Dlt_CopyArrayToLongBigEndian(&applicationId, &(Data[loffset]));
        loffset += 4u;
        /* Extract the "contextId" from the DLT service control message */
        Dlt_CopyArrayToLongBigEndian(&contextId, &(Data[loffset]));
        loffset += 4u;
    }
    else
    {
        /* Extract the "applicationId" from the DLT service control message */
        Dlt_CopyArrayToLongLittleEndian(&applicationId, &(Data[loffset]));
        loffset += 4u;
        /* Extract the "contextId" from the DLT service control message */
        Dlt_CopyArrayToLongLittleEndian(&contextId, &(Data[loffset]));
        loffset += 4u;
    }
    /* Extract the "newLogLevel" from the DLT service control message */
    newLogLevel = (sint8)Data[loffset];

    uint16 index = 0u;
    uint16 ChannelIndexList[DLT_CHANNEL_NUM];
#if (1u < DLT_SWC_NUM)
    /* Traverse the list of all swc index */
    for (; index < DLT_SWC_NUM; index++)
#endif /* 1u < DLT_SWC_NUM */
    {
        /*Select target LogChannel*/
        Dlt_GetChannelIndex(index, applicationId, contextId, ChannelIndexList);
    }
    index = 0u;
#if (1u < DLT_SWC_NUM)
    /* Traverse the list of all log channel index */
    for (; ((ChannelIndexList[index] != 0xFFFFu) && (index < DLT_CHANNEL_NUM)); index++)
#else  /* 1u < DLT_SWC_NUM */
    /* Check if the index number of given log channel is valid */
    if (ChannelIndexList[index] != 0xFFFFu)
#endif /* 1u < DLT_SWC_NUM */
    {
        uint16 ChannelIndex = ChannelIndexList[index];
        /* Check if the parameters of the request is meet the matching condition */
        /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
        if (((DealRxData.ApplicationID == applicationId) && (DealRxData.ContextID == contextId))
            && (((DealRxData.UEH == FALSE) && (Length == Dlt_GetMessageLength(FALSE, DLT_SET_LOG_LEVEL_TOTAL_LENGTH)))
                || ((DealRxData.UEH == TRUE)
                    && (Length == Dlt_GetMessageLength(TRUE, DLT_SET_LOG_LEVEL_TOTAL_LENGTH)))))
        /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
        {
            Dlt_MessageLogLevelType destLogLevel;
            /* Check if the parameter "newLogLevel" of the request is -1 */
            if (newLogLevel == (sint8)(-1))
            {
                destLogLevel = Dlt_RunTime.DefaultLogLevel;
                /* Call the API "SetLogLevel" to update the current log level for the DLT module */
                if (E_OK == Dlt_InterSetLogLevel(ChannelIndex, applicationId, contextId, destLogLevel))
                {
                    status = (uint8)DLT_STATUS_OK;
                }
            }
            /* Check if the request parameter "newLogLevel" ranges from 0 to 7 */
            else if ((newLogLevel >= 0) && (newLogLevel < 7))
            {
                destLogLevel = (Dlt_MessageLogLevelType)newLogLevel; /* PRQA S 4332 */ /* MISRA Rule 10.5 */
                /* Call the API "SetLogLevel" to update the current log level for the DLT module */
                if (E_OK == Dlt_InterSetLogLevel(ChannelIndex, applicationId, contextId, destLogLevel))
                {
                    status = (uint8)DLT_STATUS_OK;
                }
            }
            else
            {
                /*idle*/
            }
        }

        Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
        /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
        if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
        {
            uint16 SendOffset = 0U;
            uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

            /* Check if the DLT message extension header is support verbose mode */
            boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
            /* Calculate the byte length of the DLT service response message */
            uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_SERVICE_SET_LOG_LEVEL_LENGTH);

            SchM_Enter_Dlt_MsgChannel();

            /* Prepare the message header for the DLT service control response message */
            Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

            /* Load "status" for the message of the DLT service control response */
            SendControlBufferPtr[SendOffset] = status;

            SchM_Exit_Dlt_MsgChannel();

            /* Control the response message for the DLT service, triggering the send action */
            ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
        }
    }
#endif
    return ret;
}

/*
 * Brief               Service Set Trace Status
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetTraceStatus(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    uint32 applicationId;
    uint32 contextId;
    sint8 newTraceStatus;
    uint16 loffset = offset;

    /* "SetTraceStatus "Request message format: StandardHeader(16), ExternHeader(10), Payload(17) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),{ApplicationID(4),ContextID(4),newTraceStatus(1),Reserved(4)} */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1) */

    /* Check if the DLT message format is support big-endian mode */
    if (DealRxData.MSBF == TRUE)
    {
        /* Extract the "applicationId" from the DLT service control message */
        Dlt_CopyArrayToLongBigEndian(&applicationId, &(Data[loffset]));
        loffset += 4u;
        /* Extract the "contextId" from the DLT service control message */
        Dlt_CopyArrayToLongBigEndian(&contextId, &(Data[loffset]));
        loffset += 4u;
    }
    else
    {
        /* Extract the "applicationId" from the DLT service control message */
        Dlt_CopyArrayToLongLittleEndian(&applicationId, &(Data[loffset]));
        loffset += 4u;
        /* Extract the "contextId" from the DLT service control message */
        Dlt_CopyArrayToLongLittleEndian(&contextId, &(Data[loffset]));
        loffset += 4u;
    }
    /* Extract the "newTraceStatus" from the DLT service control message */
    newTraceStatus = (sint8)Data[loffset];

    uint16 index = 0u;
    uint16 ChannelIndexList[DLT_CHANNEL_NUM];
#if (1u < DLT_SWC_NUM)
    /* Traverse the list of all swc index */
    for (; index < DLT_SWC_NUM; index++)
#endif /* 1u < DLT_SWC_NUM */
    {
        /*Select target LogChannel*/
        Dlt_GetChannelIndex(index, applicationId, contextId, ChannelIndexList);
    }
    index = 0u;
#if (1u < DLT_SWC_NUM)
    /* Traverse the list of all log channel index */
    for (; ((ChannelIndexList[index] != 0xFFFFu) && (index < DLT_CHANNEL_NUM)); index++)
#else  /* 1u < DLT_SWC_NUM */
    /* Check if the index number of given log channel is valid */
    if (ChannelIndexList[index] != 0xFFFFu)
#endif /* 1u < DLT_SWC_NUM */
    {
        uint16 ChannelIndex = ChannelIndexList[index];
        /* Check if the parameters of the request is meet the matching condition */
        /* PRQA S 3415 ++ */ /* MISRA Rule 13.5 */
        if (((DealRxData.ApplicationID == applicationId) && (DealRxData.ContextID == contextId))
            && (((DealRxData.UEH == FALSE)
                 && (Length == Dlt_GetMessageLength(FALSE, DLT_SET_TRACE_STATUS_TOTAL_LENGTH)))
                || ((DealRxData.UEH == TRUE)
                    && (Length == Dlt_GetMessageLength(TRUE, DLT_SET_TRACE_STATUS_TOTAL_LENGTH)))))
        /* PRQA S 3415 -- */ /* MISRA Rule 13.5 */
        {
            boolean destTraceStatus;
            /* Check if the parameter "newTraceStatus" of the request is -1 */
            if (newTraceStatus == (sint8)(-1))
            {
                destTraceStatus = Dlt_RunTime.DefaultTraceStatus;
                /* Call the API "SetTraceStatus" to update the current trace status for the DLT module */
                if (E_OK == Dlt_InterSetTraceStatus(ChannelIndex, applicationId, contextId, destTraceStatus))
                {
                    status = (uint8)DLT_STATUS_OK;
                }
            }
            /* Check if the request parameter "newLogLevel" ranges from 0 to 2 */
            else if ((newTraceStatus >= 0) && (newTraceStatus < 2)) /* PRQA S 4330 */ /* MISRA Rule 10.5 */
            {
                destTraceStatus = (boolean)newTraceStatus; /* PRQA S 4330 */ /* MISRA Rule 10.5 */
                /* Call the API "SetTraceStatus" to update the current trace status for the DLT module */
                if (E_OK == Dlt_InterSetTraceStatus(ChannelIndex, applicationId, contextId, destTraceStatus))
                {
                    status = (uint8)DLT_STATUS_OK;
                }
            }
            else
            {
                /*idle*/
            }
        }

        Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
        /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
        if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
        {
            uint16 SendOffset = 0;
            uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

            /* Check if the DLT message extension header is support verbose mode */
            boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
            /* Calculate the byte length of the DLT service response message */
            uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_SERVICE_SET_TRACE_STATUS_LENGTH);

            SchM_Enter_Dlt_MsgChannel();

            /* Prepare the message header for the DLT service control response message */
            Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

            /* Load "status" for the message of the DLT service control response */
            SendControlBufferPtr[SendOffset] = status;

            SchM_Exit_Dlt_MsgChannel();

            /* Control the response message for the DLT service, triggering the send action */
            ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
        }
    }
#endif
    return ret;
}

/*
 * Brief               Service Get Log Info
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetLogInfo(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))

    uint32 applicationId;
    uint32 contextId;
    uint8 options;
    uint16 loffset = offset;

    /* "GetLogInfo "Request message format: StandardHeader(16), ExternHeader(10), Payload(17) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),{Options(1),ApplicationID(4),ContextID(4),Reserved(4)} */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5 + n + 4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),LogInfo(n),Reserved(4) */

    /* Extract the "options" from the DLT service control message */
    options = Data[loffset];
    loffset++;
    /* Check if the DLT message format is support big-endian mode */
    if (DealRxData.MSBF == TRUE)
    {
        /* Extract the "applicationId" from the DLT service control message */
        Dlt_CopyArrayToLongBigEndian(&applicationId, &(Data[loffset]));
        loffset += 4u;
        /* Extract the "contextId" from the DLT service control message */
        Dlt_CopyArrayToLongBigEndian(&contextId, &(Data[loffset]));
    }
    else
    {
        /* Extract the "applicationId" from the DLT service control message */
        Dlt_CopyArrayToLongLittleEndian(&applicationId, &(Data[loffset]));
        loffset += 4u;
        /* Extract the "contextId" from the DLT service control message */
        Dlt_CopyArrayToLongLittleEndian(&contextId, &(Data[loffset]));
    }

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((FALSE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(FALSE, DLT_GET_LOG_INFO_TOTAL_LENGTH)))
        || ((TRUE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(TRUE, DLT_GET_LOG_INFO_TOTAL_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        uint16 SwcIndex = 0;
#if (1u < DLT_SWC_NUM)
        /* Traverse the list of all log channel index */
        for (; SwcIndex < DLT_SWC_NUM; SwcIndex++)
#endif /* 1u < DLT_SWC_NUM */
        {
#if ((DLT_SWC_NUM > 0) & (DLT_SWC_MAX_CONTEXT_NUM > 0))
            const Dlt_SwcInfoType* SwcInfoPtr = &(Dlt_RunTime.SwcInfo[SwcIndex]);
            /* All SWC context information is traversed */
            for (uint16 SwcContextIndex = 0; SwcContextIndex < SwcInfoPtr->DltSwcContextNum; SwcContextIndex++)
            {
                const Dlt_SwcContextInofType* SwcContextInfoPtr =
                    &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[SwcContextIndex]);
                /* Check if the parameter applicationId/contextId tuple is matche */
                if ((applicationId == SwcContextInfoPtr->SwcContext.SwcApplicationId)
                    && (contextId == SwcContextInfoPtr->SwcContext.SwcContextId))
                {
                    /* Call the API "GetLogInfo" to get log information for the DLT module */
                    if (E_OK == Dlt_SendGetLogInfo(options, SwcIndex, SwcContextIndex))
                    {
                        ret = E_OK;
                    }
                }
            }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
        }
    }
    else
    {
        uint16 index = 0u;
        uint16 ChannelIndexList[DLT_CHANNEL_NUM];
#if (1u < DLT_SWC_NUM)
        /* Traverse the list of all Swc index */
        for (; index < DLT_SWC_NUM; index++)
#endif /* 1u < DLT_SWC_NUM */
        {
            /*Select target LogChannel*/
            Dlt_GetChannelIndex(index, applicationId, contextId, ChannelIndexList);
        }
        index = 0u;
#if (1u < DLT_SWC_NUM)
        /* Traverse the list of all log channel index */
        for (; ((ChannelIndexList[index] != 0xFFFFu) && (index < DLT_CHANNEL_NUM)); index++)
#else  /* 1u < DLT_SWC_NUM */
        /* Check if the index number of given log channel is valid */
        if (ChannelIndexList[index] != 0xFFFFu)
#endif /* 1u < DLT_SWC_NUM */
        {
            uint16 ChannelIndex = ChannelIndexList[index];
            Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
            /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
            if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
            {
                uint16 SendOffset = 0;
                uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

                /* Check if the DLT message extension header is support verbose mode */
                boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
                /* Calculate the byte length of the DLT service response message */
                uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_GET_LOG_INFO_ERRRES_LENGTH);

                SchM_Enter_Dlt_MsgChannel();

                /* Prepare the message header for the DLT service control response message */
                Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

                /* Load "status" for the message of the DLT service control response */
                SendControlBufferPtr[SendOffset] = (uint8)DLT_STATUS_ERROR;

                SchM_Exit_Dlt_MsgChannel();

                /* Control the response message for the DLT service, triggering the send action */
                ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
            }
        }
    }
#endif
    return ret;
}
/*
 * Brief               Service Get Default Log Level
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetDefaultLogLevel(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "GetDefaultLogLevel "Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4)} */

    /* Response message format: standardHeader(16), externHeader(10), Payload(6) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),defaultLogLevel(1) */

    DLT_UNUSED(Data);
    DLT_UNUSED(offset);

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((DealRxData.UEH == FALSE) && (Length == Dlt_GetMessageLength(FALSE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH)))
        || ((DealRxData.UEH == TRUE) && (Length == Dlt_GetMessageLength(TRUE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        status = (uint8)DLT_STATUS_OK;
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_GET_DEFAULT_LOG_INFO_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SendOffset++;
        /* Load "DefaultLogLevel" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = (uint8)Dlt_RunTime.DefaultLogLevel;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Store Configuration
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceStoreConfiguration(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "StoreConfiguration "Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4)} */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1) */

    DLT_UNUSED(Data);
    DLT_UNUSED(offset);

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((DealRxData.UEH == FALSE) && (Length == Dlt_GetMessageLength(FALSE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH)))
        || ((DealRxData.UEH == TRUE) && (Length == Dlt_GetMessageLength(TRUE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
#if (DLT_NVM_RAM_SUPPORT == STD_ON)
        /* Store DLT module specific configuration parameters to the NVM */
        ret = Dlt_Store();
        status = (uint8)DLT_STATUS_OK;
#else
        status = (uint8)DLT_STATUS_NOT_SUPPORTED;
#endif
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_STORE_CONFIGURATION_TOTAL_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Reset To Factory Default
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceResetToFactoryDefault(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "ResetToFactoryDefault "Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4)} */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1) */

    DLT_UNUSED(Data);
    DLT_UNUSED(offset);

    /* Check if the parameters of the request is meet the matching condition */
    /*PRQA S 3415++*/ /* MISRA Rule 13.5 */
    if (((DealRxData.UEH == FALSE) && (Length == Dlt_GetMessageLength(FALSE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH)))
        || ((DealRxData.UEH == TRUE) && (Length == Dlt_GetMessageLength(TRUE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH))))
    /*PRQA S 3415--*/ /* MISRA Rule 13.5 */
    {
        /* Copy initialization parameters from the global configuration information of the DLT module */
        Dlt_Init_From_Cfg(Dlt_ConfigPtr);
        status = (uint8)DLT_STATUS_OK;
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_RESET_TO_FACTORY_DEFAULT_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Set Message Filtering
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetMessageFiltering(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "SetMessageFiltering "Request message format: StandardHeader(16), ExternHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),newFilterstatus(1) } */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1) */

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((DealRxData.UEH == FALSE) && (Length == Dlt_GetMessageLength(FALSE, DLT_SET_MESSAGE_FILTERING_TOTAL_LENGTH)))
        || ((DealRxData.UEH == TRUE) && (Length == Dlt_GetMessageLength(TRUE, DLT_SET_MESSAGE_FILTERING_TOTAL_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        SchM_Enter_Dlt_SwcContext();
        /* Update the current message filtering status of the DLT module */
        /* PRQA S 4340++ */ /* MISRA Rule 10.5 */
        Dlt_RunTime.MessageFilterEnable = (boolean)Data[offset];
        /* PRQA S 4340-- */ /* MISRA Rule 10.5 */
        SchM_Exit_Dlt_SwcContext();
        status = (uint8)DLT_STATUS_OK;
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_SET_MESSAGE_FILTERING_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Set Default Log Level
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetDefaultLogLevel(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "SetDefaultLogLevel "Request message format: StandardHeader(16), ExternHeader(10), Payload(9) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),newLogLevel(1),Reserved(4) } */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1) */

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((DealRxData.UEH == FALSE) && (Length == Dlt_GetMessageLength(FALSE, DLT_SET_DEFAULT_LOG_LEVEL_TOTAL_LENGTH)))
        || ((DealRxData.UEH == TRUE) && (Length == Dlt_GetMessageLength(TRUE, DLT_SET_DEFAULT_LOG_LEVEL_TOTAL_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        SchM_Enter_Dlt_SwcContext();
        /* Check if the parameter "MessageFilterStatus" of the request is -1 */
        if ((sint8)Data[offset] == (sint8)-1)
        {
            /* Update the current message filter status of the DLT module */
            Dlt_RunTime.MessageFilterPass = DLT_MESSAGE_PASS;
            status = (uint8)DLT_STATUS_OK;
        }
        /* Check if the request parameter "MessageFilterStatus" ranges from 0 to 7 */
        else if (((sint8)Data[offset] > 0) && ((sint8)Data[offset] < 7))
        {
            /* Update the current message filter status of the DLT module */
            Dlt_RunTime.MessageFilterPass = DLT_MESSAGE_FILTER;
            /* Update the current default log level of the DLT module */
            /* PRQA S 4342++ */ /* MISRA Rule 10.5 */
            Dlt_RunTime.DefaultLogLevel = (Dlt_MessageLogLevelType)Data[offset];
            /* PRQA S 4342-- */ /* MISRA Rule 10.5 */
            status = (uint8)DLT_STATUS_OK;
        }
        /* Check if the parameter "MessageFilterStatus" of the request is 0 */
        else if ((sint8)Data[offset] == 0)
        {
            /* Update the current message filter status of the DLT module */
            Dlt_RunTime.MessageFilterPass = DLT_MESSAGE_BLOCKD;
            /* Update the current default log level of the DLT module */
            /* PRQA S 4342++ */ /* MISRA Rule 10.5 */
            Dlt_RunTime.DefaultLogLevel = (Dlt_MessageLogLevelType)Data[offset];
            /* PRQA S 4342-- */ /* MISRA Rule 10.5 */
            status = (uint8)DLT_STATUS_OK;
        }
        else
        {
            /*idle*/
        }
        SchM_Exit_Dlt_SwcContext();
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_SET_DEFAULT_LOG_LEVEL_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Set Default Trace Status
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetDefaultTraceStatus(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "SetDefaultTraceStatus "Request message format: StandardHeader(16), ExternHeader(10), Payload(9) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),newTraceStatus(1),Reserved(4) } */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1) */

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((DealRxData.UEH == FALSE)
         && (Length == Dlt_GetMessageLength(FALSE, DLT_SET_DEFAULT_TRACE_STATUS_TOTAL_LENGTH)))
        || ((DealRxData.UEH == TRUE)
            && (Length == Dlt_GetMessageLength(TRUE, DLT_SET_DEFAULT_TRACE_STATUS_TOTAL_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        /* Check if the request parameter "DefaultTraceStatus" ranges from 0 to 2 */
        if (((sint8)Data[offset] >= 0) && ((sint8)Data[offset] < 2))
        {
            SchM_Enter_Dlt_SwcContext();
            /* Update the current default trace status of the DLT module */
            /* PRQA S 4340++ */ /* MISRA Rule 10.5 */
            Dlt_RunTime.DefaultTraceStatus = (boolean)Data[offset];
            /* PRQA S 4340-- */ /* MISRA Rule 10.5 */
            SchM_Exit_Dlt_SwcContext();
            status = (uint8)DLT_STATUS_OK;
        }
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_SET_DEFAULT_TRACE_STATUS_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Get Soft ware Version
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetSoftwareVersion(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "GetEcuSoftwareVersion "Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4) } */

    /* Response message format: standardHeader(16), externHeader(10), Payload(5+4+n) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),Length(4),SoftwareVersion(n) */

    DLT_UNUSED(Data);
    DLT_UNUSED(offset);

    /* Check if the parameters of the request is meet the matching condition */
    if (((DealRxData.UEH == FALSE)
         /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
         && (Length == Dlt_GetMessageLength(FALSE, DLT_SET_DEFAULT_TRACE_STATUS_TOTAL_LENGTH)))
        || ((DealRxData.UEH == TRUE)
            && (Length == Dlt_GetMessageLength(TRUE, DLT_SET_DEFAULT_TRACE_STATUS_TOTAL_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        status = (uint8)DLT_STATUS_OK;
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_GET_SOFT_WARE_VERSION_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SendOffset++;
        uint32 SwVersionLength = (uint32)DLT_SOFT_WARE_VERSION_LENGTH;
        /* Load "SwVersionLength" for the message of the DLT service control response */
        Dlt_CopyLongToArrayBigEndian(SwVersionLength, &SendControlBufferPtr[SendOffset]);

        SendOffset += 4u;
        /* Load "SwVersionVendorID" for the message of the DLT service control response */
        Dlt_CopyIntToArrayBigEndian((uint16)DLT_VENDOR_ID, &SendControlBufferPtr[SendOffset]);

        SendOffset += 2u;
        /* Load "SwVersionModuleID" for the message of the DLT service control response */
        Dlt_CopyIntToArrayBigEndian((uint16)DLT_MODULE_ID, &SendControlBufferPtr[SendOffset]);

        SendOffset += 2u;
        /* Load "SwMajorVersion" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = DLT_SW_MAJOR_VERSION;

        SendOffset++;
        /* Load "SwMinorVersion" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = DLT_SW_MINOR_VERSION;

        SendOffset++;
        /* Load "SwPatchVersion" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = DLT_SW_PATCH_VERSION;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}
/*
 * Brief               Service Get Default Trace Status
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetDefaultTraceStatus(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "GetDefaultTraceStatus "Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4) } */

    /* Response message format: standardHeader(16), externHeader(10), Payload(6) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),traceStatus(1) */

    DLT_UNUSED(Data);
    DLT_UNUSED(offset);

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((FALSE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(FALSE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH)))
        || ((TRUE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(TRUE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH))))
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    {
        status = (uint8)DLT_STATUS_OK;
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_GET_DEFAULT_TRACE_STATUS_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SendOffset++;
        /* Load "DefaultTraceStatus" for the message of the DLT service control response */
        /* PRQA S 4304++ */ /* MISRA Rule 10.5 */
        SendControlBufferPtr[SendOffset] = (uint8)Dlt_RunTime.DefaultTraceStatus;
        /* PRQA S 4304-- */ /* MISRA Rule 10.5 */

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Get Log Channel Names
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetLogChannelNames(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "GetLogChannelNames" Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4) } */

    /* "GetLogChannelNames" Response message format: standardHeader(16), externHeader(10), Payload(10) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),countIf(1),logChannelNames(4) */

    DLT_UNUSED(Data);
    DLT_UNUSED(offset);

    /* "GetLogChannelNames" Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4) } */

    /* "GetLogChannelNames" Response message format: standardHeader(16), externHeader(10), Payload(10) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),countIf(1),logChannelNames(4) */

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((FALSE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(FALSE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH)))
        || ((TRUE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(TRUE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        status = (uint8)DLT_STATUS_OK;
    }
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_GET_LOG_CHANNEL_NAMES_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SendOffset++;
        /* Load "CountIf" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = (uint8)DLT_CHANNEL_NUM;

        SendOffset++;
        /* Load "logChannelNames" for the message of the DLT service control response */
        uint16 index = 0;
#if (1u < DLT_SWC_NUM)
        /* Traverse the index of each log channel */
        for (; index < DLT_CHANNEL_NUM; index++)
#endif /* 1u < DLT_SWC_NUM */
        {
            uint32 logChannelId = Dlt_ConfigPtr->LogOutput->LogChannel[index].DltLogChannelId;
            Dlt_CopyLongToArrayBigEndian(logChannelId, &SendControlBufferPtr[SendOffset]);
#if (1u < DLT_SWC_NUM)
            SendOffset++;
#endif /* 1u < DLT_SWC_NUM */
        }

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Get Trace Status
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetTraceStatus(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    uint32 applicationId;
    uint32 contextId;
    boolean VerboseMode;
    uint16 Messagelength;
    uint16 ChannelIndex;
    uint16 SendOffset = 0;
    uint16 loffset = offset;

    Dlt_ChannelType* ChannelPtr;
    uint8* SendControlBufferPtr;

    /* "GetTraceStatus" Request message format: StandardHeader(16), ExternHeader(10), Payload(12) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),ApplicationID(4),ContextID(4) } */

    /* "GetTraceStatus" Response message format: standardHeader(16), externHeader(10), Payload(6) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),traceStatus(1) */

    /* Check if the DLT message format is support big-endian mode */
    if (DealRxData.MSBF == TRUE)
    {
        /* Extract the "applicationId" from the DLT service control message */
        Dlt_CopyArrayToLongBigEndian(&applicationId, &(Data[loffset]));
        loffset += 4u;
        /* Extract the "contextId" from the DLT service control message */
        Dlt_CopyArrayToLongBigEndian(&contextId, &(Data[loffset]));
    }
    else
    {
        /* Extract the "applicationId" from the DLT service control message */
        Dlt_CopyArrayToLongLittleEndian(&applicationId, &(Data[loffset]));
        loffset += 4u;
        /* Extract the "contextId" from the DLT service control message */
        Dlt_CopyArrayToLongLittleEndian(&contextId, &(Data[loffset]));
    }

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((FALSE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(FALSE, DLT_GET_TRACE_STATUS_TOTAL_LENGTH)))
        || ((TRUE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(TRUE, DLT_GET_TRACE_STATUS_TOTAL_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        uint16 SwcIndex = 0;
#if (1u < DLT_SWC_NUM)
        /* Traverse the list of all Swc index */
        for (; SwcIndex < DLT_SWC_NUM; SwcIndex++)
#endif /* 1u < DLT_SWC_NUM */
        {
#if ((DLT_SWC_NUM > 0) && (DLT_SWC_MAX_CONTEXT_NUM > 0))
            const Dlt_SwcInfoType* SwcInfoPtr = &(Dlt_RunTime.SwcInfo[SwcIndex]);
            /* Traverse the list of all Swc Context index */
            for (uint16 SwcContextIndex = 0; SwcContextIndex < SwcInfoPtr->DltSwcContextNum; SwcContextIndex++)
            {
                const Dlt_SwcContextInofType* SwcContextInfoPtr =
                    &(Dlt_RunTime.SwcInfo[SwcIndex].SwcContextInfo[SwcContextIndex]);
                /* Check if the parameter applicationId/contextId tuple is matche */
                if ((applicationId == SwcContextInfoPtr->SwcContext.SwcApplicationId)
                    && (contextId == SwcContextInfoPtr->SwcContext.SwcContextId))
                {
                    status = (uint8)DLT_STATUS_OK;
                    /* Traverse the list of all log channel index */
                    for (uint16 index = 0; index < SwcContextInfoPtr->DltLogChannelRefNum; index++)
                    {
                        /* Extract target log channel */
                        ChannelIndex = SwcContextInfoPtr->DltLogChannelRef[index];
                        ChannelPtr = &(Dlt_Channel[ChannelIndex]);
                        /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
                        if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
                        {
                            SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

                            /* Check if the DLT message extension header is support verbose mode */
                            VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
                            /* Calculate the byte length of the DLT service response message */
                            Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_GET_TRACE_STATUS_RES_LENGTH);

                            SchM_Enter_Dlt_MsgChannel();

                            /* Prepare the message header for the DLT service control response message */
                            Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

                            /* Load "status" for the message of the DLT service control response */
                            SendControlBufferPtr[SendOffset] = status;

                            SendOffset++;
                            /* Load "TraceStatus" for the message of the DLT service control response */
                            /* PRQA S 4304++ */ /* MISRA Rule 10.5 */
                            SendControlBufferPtr[SendOffset] = (uint8)SwcContextInfoPtr->TraceStatus;
                            /* PRQA S 4304-- */ /* MISRA Rule 10.5 */

                            SchM_Exit_Dlt_MsgChannel();

                            /* Control the response message for the DLT service, triggering the send action */
                            ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
                        }
                    }
                }
            }
#endif /* DLT_SWC_NUM > 0 && DLT_SWC_MAX_CONTEXT_NUM > 0 */
        }
    }
    /* Check if the response status is EEROR */
    if (status == (uint8)DLT_STATUS_ERROR)
    {
        /* Extract target log channel */
        ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
        ChannelPtr = &(Dlt_Channel[ChannelIndex]);
        /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
        if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
        {
            SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

            /* Check if the DLT message extension header is support verbose mode */
            VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
            /* Calculate the byte length of the DLT service response message */
            Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_GET_TRACE_STATUS_RES_LENGTH);

            SchM_Enter_Dlt_MsgChannel();

            /* Prepare the message header for the DLT service control response message */
            Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

            /* Load "status" for the message of the DLT service control response */
            SendControlBufferPtr[SendOffset] = status;

            SendOffset++;
            /* Load "DefaultTraceStatus" for the message of the DLT service control response */
            /* PRQA S 4304++ */ /* MISRA Rule 10.5 */
            SendControlBufferPtr[SendOffset] = (uint8)Dlt_RunTime.DefaultTraceStatus;
            /* PRQA S 4304-- */ /* MISRA Rule 10.5 */

            SchM_Exit_Dlt_MsgChannel();

            /* Control the response message for the DLT service, triggering the send action */
            ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
        }
    }
#endif
    return ret;
}

/*
 * Brief               Service SetLog Channel Assignment
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetLogChannelAssignment(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "SetLogChannelAssignment" Request message format: StandardHeader(16), ExternHeader(10), Payload(12) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),ApplicationID(4),ContextID(4),logChannelName(4),addRemoveOp(1) } */

    /* "SetLogChannelAssignment" Response message format: standardHeader(16), externHeader(10), Payload(6) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),traceStatus(1) */

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((FALSE == DealRxData.UEH)
         && (Length == Dlt_GetMessageLength(FALSE, DLT_TOTAL_LENGTH_SET_LOG_CHANNEL_ASSIGNMENT)))
        || ((TRUE == DealRxData.UEH)
            && (Length == Dlt_GetMessageLength(TRUE, DLT_TOTAL_LENGTH_SET_LOG_CHANNEL_ASSIGNMENT))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        uint32 applicationId;
        uint32 contextId;
        uint32 logChannelName;
        uint16 loffset = offset;
        /* Check if the DLT message format is support big-endian mode */
        if (DealRxData.MSBF == TRUE)
        {
            /* Extract the "applicationId" from the DLT service control message */
            Dlt_CopyArrayToLongBigEndian(&applicationId, &(Data[loffset]));
            loffset += 4u;
            /* Extract the "contextId" from the DLT service control message */
            Dlt_CopyArrayToLongBigEndian(&contextId, &(Data[loffset]));
            loffset += 4u;
            /* Extract the "logChannelName" from the DLT service control message */
            Dlt_CopyArrayToLongBigEndian(&logChannelName, &(Data[loffset]));
            loffset += 4u;
        }
        else
        {
            /* Extract the "applicationId" from the DLT service control message */
            Dlt_CopyArrayToLongLittleEndian(&applicationId, &(Data[loffset]));
            loffset += 4u;
            /* Extract the "contextId" from the DLT service control message */
            Dlt_CopyArrayToLongLittleEndian(&contextId, &(Data[loffset]));
            loffset += 4u;
            /* Extract the "logChannelName" from the DLT service control message */
            Dlt_CopyArrayToLongLittleEndian(&logChannelName, &(Data[loffset]));
            loffset += 4u;
        }
        /* Extract the "addRemoveOp" from the DLT service control message */
        uint8 addRemoveOp = Data[loffset];
        /* PRQA S 4342++ */ /* MISRA Rule 10.5 */
        Dlt_AssignmentOperation destAddRemoveOp = (Dlt_AssignmentOperation)addRemoveOp;
        /* PRQA S 4342-- */ /* MISRA Rule 10.5 */
        if (E_OK == Dlt_InterSetLogChannelAssignment(applicationId, contextId, logChannelName, destAddRemoveOp))
        {
            status = (uint8)DLT_STATUS_OK;
        }
    }

    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_RES_LENGTH_SET_LOG_CHANNEL_ASSIGNMENT);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Set Log Channel Threshold
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSetLogChannelThreshold(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;
    uint16 loffset = offset;

    /* "SetLogChannelThreshold" Request message format: StandardHeader(16), ExternHeader(10), Payload(10) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),logChannelName(4),logLevelThreshold(1),traceStatus(1) } */

    /* "SetLogChannelThreshold" Response message format: standardHeader(16), externHeader(10), Payload(6) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),traceStatus(1) */

    Dlt_ChannelInfoType* ChannelInfoPtr;
    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;

    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((FALSE == DealRxData.UEH)
         && (Length == Dlt_GetMessageLength(FALSE, DLT_SET_LOG_CHANNEL_THRESHOLD_TOTAL_LENGTH)))
        || ((TRUE == DealRxData.UEH)
            && (Length == Dlt_GetMessageLength(TRUE, DLT_SET_LOG_CHANNEL_THRESHOLD_TOTAL_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        uint32 logChannelName;
        /* Check if the DLT message format is support big-endian mode */
        if (DealRxData.MSBF == TRUE)
        {
            /* Extract the "logChannelName" from the DLT service control message */
            Dlt_CopyArrayToLongBigEndian(&logChannelName, &(Data[loffset]));
            loffset += 4u;
        }
        else
        {
            /* Extract the "logChannelName" from the DLT service control message */
            Dlt_CopyArrayToLongLittleEndian(&logChannelName, &(Data[loffset]));
            loffset += 4u;
        }
        /* Extract the "logLevelThreshold" from the DLT service control message */
        uint8 logLevelThreshold = Data[loffset];
        loffset++;
        /* Extract the "traceStatus" from the DLT service control message */
        uint8 traceStatus = Data[loffset];

        /* The index number of the log channel is extracted by the log channel name */
        ChannelIndex = Dlt_GetChannelIndexByChannelName(logChannelName);
#if (1u < DLT_SWC_NUM)
        /* Check if the log channel index number is valid */
        if (ChannelIndex != DLT_CHANNEL_NUM)
#endif /* 1u < DLT_SWC_NUM */
        {
            /* Check if the parameter "logLevelThreshold/traceStatus" is within the valid range */
            if ((logLevelThreshold <= 6u) && (traceStatus <= 1u))
            {
                SchM_Enter_Dlt_MsgChannel();
                ChannelInfoPtr = &(Dlt_RunTime.ChannelInfo[ChannelIndex]);
                /* PRQA S 4342++ */ /* MISRA Rule 10.5 */
                ChannelInfoPtr->DltLogChannelThreshold = (Dlt_MessageLogLevelType)logLevelThreshold;
                /* PRQA S 4342-- */ /* MISRA Rule 10.5 */
                /* PRQA S 4340++ */ /* MISRA Rule 10.5 */
                ChannelInfoPtr->DltLogTraceStatusFlag = (boolean)traceStatus;
                /* PRQA S 4340-- */ /* MISRA Rule 10.5 */
                SchM_Exit_Dlt_MsgChannel();
                status = (uint8)DLT_STATUS_OK;
            }
        }
    }

    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if ((ChannelIndex != DLT_CHANNEL_NUM) && (ChannelPtr->LastSendStatus == DLT_WAIT_SEND))
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_SET_LOG_CHANNEL_THRESHOLD_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SendOffset++;
        /* Load "traceStatus" for the message of the DLT service control response */
        ChannelInfoPtr = &(Dlt_RunTime.ChannelInfo[ChannelIndex]);
        /* PRQA S 4304++ */ /* MISRA Rule 10.5 */
        SendControlBufferPtr[SendOffset] = (uint8)ChannelInfoPtr->DltLogTraceStatusFlag;
        /* PRQA S 4304-- */ /* MISRA Rule 10.5 */

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Get Log Channel Threshold
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceGetLogChannelThreshold(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
    uint8 status = (uint8)DLT_STATUS_ERROR;

    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;

    /* "GetLogChannelThreshold" Request message format: StandardHeader(16), ExternHeader(10), Payload(8) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),logChannelName(4) } */

    /* "GetLogChannelThreshold" Response message format: standardHeader(16), externHeader(10), Payload(7) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),logLevelThreshold(1),traceStatus(1) */

    /* Calculate the byte length of the DLT payload content */
    /* Check if the parameters of the request is meet the matching condition */
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    if (((DealRxData.UEH == FALSE)
         && (Length == Dlt_GetMessageLength(FALSE, DLT_GET_LOG_CHANNEL_THRESHOLD_TOTAL_LENGTH)))
        || ((DealRxData.UEH == TRUE)
            && (Length == Dlt_GetMessageLength(TRUE, DLT_GET_LOG_CHANNEL_THRESHOLD_TOTAL_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        uint32 logChannelName;

        /* Check if the DLT message format is support big-endian mode */
        if (DealRxData.MSBF == TRUE)
        {
            /* Extract the "logChannelName" from the DLT service control message */
            Dlt_CopyArrayToLongBigEndian(&logChannelName, &(Data[offset]));
        }
        else
        {
            /* Extract the "logChannelName" from the DLT service control message */
            Dlt_CopyArrayToLongLittleEndian(&logChannelName, &(Data[offset]));
        }

        /* The index number of the log channel is extracted by the log channel name */
        ChannelIndex = Dlt_GetChannelIndexByChannelName(logChannelName);
        /* Check if the log channel index number is valid */
        if (ChannelIndex != DLT_CHANNEL_NUM)
        {
            status = (uint8)DLT_STATUS_OK;
        }
        else
        {
            /* The index number of the log channel is extracted by the default log channel */
            ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
        }
    }

    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_GET_LOG_CHANNEL_THRESHOLD_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;
        SendOffset++;

        const Dlt_ChannelInfoType* ChannelInfoPtr = &(Dlt_RunTime.ChannelInfo[ChannelIndex]);

        /* Load "logChannelThreshold" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = (uint8)ChannelInfoPtr->DltLogChannelThreshold;

        SendOffset++;
        /* Load "traceStatus" for the message of the DLT service control response */
        /* PRQA S 4304++ */ /* MISRA Rule 10.5 */
        SendControlBufferPtr[SendOffset] = (uint8)ChannelInfoPtr->DltLogTraceStatusFlag;
        /* PRQA S 4304-- */ /* MISRA Rule 10.5 */

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}

/*
 * Brief               Service Sync Time Stamp
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceSyncTimeStamp(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if ((DLT_CHANNEL_NUM > 0) && (DLT_CHANNEL_MAX_BUFFER_LENGTH > 0))
#if (DLT_TIME_STAMP_SUPPORT == STD_OFF)
    /* The response message is service unsupported */
    ret = Dlt_ServiceNotSupport(offset, Data, Length, DealRxData);
#else
    uint8 status = (uint8)DLT_STATUS_ERROR;

    /* "SyncTimeStamp" Request message format: StandardHeader(16), ExternHeader(10), Payload(4) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4) } */

    /* "SyncTimeStamp" Response message format: standardHeader(16), externHeader(10), Payload(9) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1),SyncTimestamp(4) */

    DLT_UNUSED(Data);
    DLT_UNUSED(offset);
    /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
    /* Check if the parameters of the request is meet the matching condition */
    if (((FALSE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(FALSE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH)))
        || ((TRUE == DealRxData.UEH) && (Length == Dlt_GetMessageLength(TRUE, DLT_MESSAGE_NO_OTHER_INFO_LENGTH))))
    /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
    {
        status = (uint8)DLT_STATUS_OK;
    }

    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        StbM_UserDataType userData;
        StbM_TimeStampType TimeStamp;
        uint16 SendOffset = 0;

        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_SYNC_TIME_STAMP_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();
        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);
        SchM_Exit_Dlt_MsgChannel();

        /* Gets the current timestamp from the STBM module */
        ret = StbM_GetCurrentTime(DLT_TIME_REF, &TimeStamp, &userData);
        if (E_NOT_OK == ret)
        {
            status = (uint8)DLT_STATUS_ERROR;
        }

        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);

        SchM_Enter_Dlt_MsgChannel();

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SendOffset++;
        /* Load "synctimestamp.nanoseconds" for the message of the DLT service control response */
        Dlt_CopyLongToArrayBigEndian(TimeStamp.nanoseconds, &SendControlBufferPtr[SendOffset]);

        SendOffset += 4u;
        /* Load "synctimestamp.seconds" for the message of the DLT service control response */
        Dlt_CopyLongToArrayBigEndian(TimeStamp.seconds, &SendControlBufferPtr[SendOffset]);

        SendOffset += 4u;
        /* Load "synctimestamp.secondsHi" for the message of the DLT service control response */
        Dlt_CopyIntToArrayBigEndian(TimeStamp.secondsHi, &SendControlBufferPtr[SendOffset]);

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }

#endif
#endif
    return ret;
}

#if (DLT_INJECTION_SUPPORT == STD_ON)
/*
 * Brief               Service Deal With Injection
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different PduIds. Non reentrant for the same PduId.
 * Param-Name[in]      offset:rx data offset.
 *                     Data: rx data
 *                     Length:total length of rx data
 *                     DealRxData:
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              Std_ReturnType
 */
static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_ServiceDealWithInjection(uint16 offset, uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    Std_ReturnType ret = E_NOT_OK;
#if (DLT_CHANNEL_NUM > 0)
    uint8 status = (uint8)DLT_STATUS_ERROR;
    uint16 loffset = offset;

    /* "CallSwcInjection" Request message format: StandardHeader(16), ExternHeader(10), Payload(8+n) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),DataLength(4),DataBuffer(n) } */

    /* "CallSwcInjection" Response message format: standardHeader(16), externHeader(10), Payload(6) */
    /* Header format: {HTYP(1),MCNT(1),LEN(2),ECUID(4),SEID(4),TMSP(4)},{MSIN(1),NOAR(1),APID(4),CTID(4)} */
    /* Payload format: ServiceID(4),Status(1), */

    /* Check if the parameters of the request is meet the matching condition */
    if ((DealRxData.UEH == TRUE) && (Length >= Dlt_GetMessageLength(TRUE, 4u))) /* PRQA S 3415 */ /* MISRA Rule 13.5 */
    {
        uint32 dataLength;
        /* Check if the DLT message format is support big-endian mode */
        if (DealRxData.MSBF == TRUE)
        {
            /* Extract the "dataLength" from the DLT service control message */
            Dlt_CopyArrayToLongBigEndian(&dataLength, &(Data[loffset]));
            loffset += 4u;
        }
        else
        {
            /* Extract the "dataLength" from the DLT service control message */
            Dlt_CopyArrayToLongLittleEndian(&dataLength, &(Data[loffset]));
            loffset += 4u;
        }
        uint16 Payloadlength = 4U + dataLength + sizeof(dataLength); /* PRQA S 4461 */ /* MISRA Rule 10.3 */
        /* Check if the parameters of the request is meet the matching condition */
        if (Length == Dlt_GetMessageLength(TRUE, Payloadlength))
        {
            uint8* Indata = &(Data[loffset]);
            /* The Swc index number is extracted by the session identifier */
            uint16 SwcIndex = Dlt_GetSwcSessionIdIndex(DealRxData.SessionID);
#if (1u < DLT_SWC_NUM)
            if (SwcIndex != DLT_SWC_NUM)
#endif /* 1u < DLT_SWC_NUM */
            {
#if (DLT_SWC_NUM > 0)
                /* PRQA S 0311++ */ /* MISRA Rule 11.8 */
                const Dlt_SwcTypes* SwcPtr = (Dlt_SwcTypes*)&(Dlt_Swc.DltSwc[SwcIndex]);
                /* PRQA S 0311-- */ /* MISRA Rule 11.8 */
                if (NULL_PTR != SwcPtr->InjectionCallback)
                {
                    ret = SwcPtr->InjectionCallback(
                        DealRxData.ApplicationID,
                        DealRxData.ContextID,
                        DealRxData.ServiceID,
                        dataLength,
                        Indata);
                    if (ret == E_OK)
                    {
                        status = (uint8)DLT_STATUS_OK;
                    }
                }
                else
#endif /* 1u < DLT_SWC_NUM */
                {
                    ret = E_NOT_OK;
                }
            }
        }
    }

    uint16 ChannelIndex = Dlt_ConfigPtr->LogOutput->DltDefaultLogChannelRef;
    Dlt_ChannelType* ChannelPtr = &(Dlt_Channel[ChannelIndex]);
    /* Check if the last sent status of given log channel was "Waiting for response to be sent". */
    if (ChannelPtr->LastSendStatus == DLT_WAIT_SEND)
    {
        uint16 SendOffset = 0;
        uint8* SendControlBufferPtr = &(ChannelPtr->SendControlBuffer[0]);
        /* Check if the DLT message extension header is support verbose mode */
        boolean VerboseMode = Dlt_ConfigPtr->Protocol->DltUseExtHeaderInNonVerbMode;
        /* Calculate the byte length of the DLT service response message */
        uint16 Messagelength = Dlt_GetMessageLength(VerboseMode, DLT_CALL_SWC_INJECTION_RES_LENGTH);

        SchM_Enter_Dlt_MsgChannel();

        /* Prepare the message header for the DLT service control response message */
        Dlt_ServicePrepareResponseHeaderData(&SendOffset, ChannelIndex, Messagelength, DealRxData);

        /* Load "status" for the message of the DLT service control response */
        SendControlBufferPtr[SendOffset] = status;

        SchM_Exit_Dlt_MsgChannel();

        /* Control the response message for the DLT service, triggering the send action */
        ret = Dlt_ServiceSendResponseData(ChannelIndex, Messagelength);
    }
#endif
    return ret;
}
#endif

static FUNC(Std_ReturnType, DLT_APPL_CODE)
    Dlt_SubDealRxData(uint16 offset, const uint8* Data, uint16 Length, Dlt_DealRxDataTypes DealRxData)
{
    uint32 ServiceID;
    uint16 loffset = offset;
    Std_ReturnType ret;
    static CONST(Dlt_ServiceDeal, NVM_CONST) Dlt_ServiceTable[] = {
        Dlt_ServiceNotSupport,              /* ServiceID == 0u and ServiceID > 0x24u */
        Dlt_ServiceSetLogLevel,             /* ServiceID: 0x00000001 */
        Dlt_ServiceSetTraceStatus,          /* ServiceID: 0x00000002 */
        Dlt_ServiceGetLogInfo,              /* ServiceID: 0x00000003 */
        Dlt_ServiceGetDefaultLogLevel,      /* ServiceID: 0x00000004 */
        Dlt_ServiceStoreConfiguration,      /* ServiceID: 0x00000005 */
        Dlt_ServiceResetToFactoryDefault,   /* ServiceID: 0x00000006 */
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000007  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000008  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000009  not support*/
        Dlt_ServiceSetMessageFiltering,     /* ServiceID: 0x0000000A */
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000000B  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000000C  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000000D  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000000E  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000000F  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000010  not support*/
        Dlt_ServiceSetDefaultLogLevel,      /* ServiceID: 0x00000011 */
        Dlt_ServiceSetDefaultTraceStatus,   /* ServiceID: 0x00000012 */
        Dlt_ServiceGetSoftwareVersion,      /* ServiceID: 0x00000013 */
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000014  not support*/
        Dlt_ServiceGetDefaultTraceStatus,   /* ServiceID: 0x00000015 */
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000016  not support*/
        Dlt_ServiceGetLogChannelNames,      /* ServiceID: 0x00000017 */
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000018  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000019  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000001A  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000001B  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000001C  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000001D  not support*/
        Dlt_ServiceNotSupport,              /* ServiceID: 0x0000001E  not support*/
        Dlt_ServiceGetTraceStatus,          /* ServiceID: 0x0000001F */
        Dlt_ServiceSetLogChannelAssignment, /* ServiceID: 0x00000020 */
        Dlt_ServiceSetLogChannelThreshold,  /* ServiceID: 0x00000021 */
        Dlt_ServiceGetLogChannelThreshold,  /* ServiceID: 0x00000022 */
        Dlt_ServiceNotSupport,              /* ServiceID: 0x00000023  not support*/
        Dlt_ServiceSyncTimeStamp,           /* ServiceID: 0x00000024 */
    };
    /* Check if the DLT message format is support big-endian mode */
    if (DealRxData.MSBF == TRUE)
    {
        /* Load "ServiceID[hex]" for the message of the DLT service control response */
        Dlt_CopyArrayToLongBigEndian(&ServiceID, &(Data[loffset]));
    }
    else
    {
        /* Load "ServiceID[hex]" for the message of the DLT service control response */
        Dlt_CopyArrayToLongLittleEndian(&ServiceID, &(Data[loffset]));
    }
    /* Copy the service identifier to the specified global variable */
    DealRxData.ServiceID = ServiceID;
    loffset += 4u;

    /* Check if the value of the service identifier ranges from 0 to 37 */
    if ((ServiceID > 0u) && (ServiceID < 0x25u))
    {
        /* By the service identifier, execute the service response processed of the control command */
        ret = Dlt_ServiceTable[ServiceID](loffset, Data, Length, DealRxData);
    }
#if (DLT_INJECTION_SUPPORT == STD_ON)
    /* Check if the value of the service identifier is greater than 0xFFF */
    else if (ServiceID >= 0x00000FFFu)
    {
        /* Service responses are handled by SWC injection control */
        ret = Dlt_ServiceDealWithInjection(loffset, Data, Length, DealRxData);
    }
#endif
    /* Check if the value of the service identifier is within the invalid range */
    else
    {
        /* By the service identifier, execute the service response processed of the control command */
        ret = Dlt_ServiceTable[0](loffset, Data, Length, DealRxData);
    }
    return ret;
}

#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h"

#endif
