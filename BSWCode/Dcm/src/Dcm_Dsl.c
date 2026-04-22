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
 **  @file               : Dcm_Dsl.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : DcmDsl Private Functions Definitions
 **
 ***********************************************************************************************************************/
/* PRQA S 6530, 1504, 1514 EOF */ /* VL_MTR_Dcm_STECT, VL_QAC_OneRefSymbol, VL_QAC_OneRefSymbol */
/* PRQA S 1503, 1532, 3120 EOF */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef, VL_QAC_MagicNum */
/* =================================================== inclusions =================================================== */
#include "ComM_Dcm.h"
#include "PduR_Dcm.h"
#include "Dcm_Internal.h"
#include "Dcm_ComM.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief service id of UDS service TesterPresent
 */
#define DCM_UDS_TESTERPRESENT_SID 0x3Eu
/* ============================================ external data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief protocol control unit @range NA
 */
Dcm_ProtocolCtrlType Dcm_ProtocolCtrl[DCM_PROTOCOLROW_NUM];
/**
 * @brief msgContext for storing reqData and resData @range NA
 */
Dcm_MsgContextType Dcm_MsgContext[DCM_PROTOCOLROW_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief current active diagnostic status @range NA
 */
Dcm_ActiveDiagnosticType Dcm_ActiveDiagnostic;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief current comm State @range NA
 */
Dcm_CommStateType Dcm_CommState[DCM_MAINCONNECTION_NUM];
boolean           Dcm_RequestRejectedDueToBoot    = FALSE;
boolean           Dcm_ResRejectedDueToAFTER_RESET = FALSE;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief the whole dcm buffer (all TX+RX) @range NA
 */
uint8 Dcm_Buffer[DCM_TOTAL_BUFFER_SIZE];
#if (DCM_METADATA_NUM > 0u)
/**
 * @brief stored metadata
 */
uint8 Dcm_MetaData[DCM_METADATA_NUM * DCM_METADATA_SIZE];
#endif
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief pduInfoBuffer for multicore transmission @range NA
 */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
PduInfoType Dcm_TempPduInfo[DCM_PROTOCOLROW_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
/**
 * @brief indicates if there is pending setProgramConditions globally @range 0..1
 */
boolean Dcm_PendingSetProg;
/**
 * @brief indicates if there is onging setProgramConditions which is process in Dcm_MainFunction @range 0..1
 */
boolean Dcm_ProcessingSetProg;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief The current request queue index @range 0..255
 */
uint8 Dcm_QueuedIndex;
/**
 * @brief request queue buffer @range NA
 */
uint8 Dcm_RequestQueuedBuffer[2u][DCM_REQUEST_QUEUED_BUFFER_SIZE];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief request queue control unit @range NA
 */
Dcm_QueuedRequestCtrlType Dcm_QueuedRequestCtrl[2u];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief confirmation info for delayed confirmation callout @range NA
 */
Dcm_ConfirmationHandleType Dcm_ConfirmationHandle[DCM_PROTOCOLROW_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief current security level @range NA
 */
Dcm_SecLevelType Dcm_CurrentSecurity;
/**
 * @brief current diagnostic state @range NA
 */
DCM_LOCAL Dcm_DiagnosticStateType Dcm_DiagnosticState[DCM_MAINCONNECTION_NUM]; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         This function checks whether the protocol shares a buffer with other protocols and those protocols are
 *                idle or not
 * @param[in]     protocolRow : the input protocolRow
 * @return        Std_ReturnType
 * @retval        E_OK     : all protocols that share the same buffer with the input protocol are idle
 * @retval        E_NOT_OK : at least one protocol that share the same buffer with the input protocol is not idle
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DslInternal_CheckProtocolWithSameBuffer(const Dcm_DslProtocolRowType* protocolRow);
/**
 * @brief         This function searches for matched protocol and connection Id by txPduId
 * @param[in]     Id           : Identification of the I-PDU.
 * @param[out]    ProtocolId   : the matched protocolId
 * @param[out]    connectionId : the matched connectionId (under the protocol)
 * @return        Std_ReturnType
 * @retval        E_OK     : Succesfully find the matched one
 * @retval        E_NOT_OK : Fail to find any mathced one
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DslInternal_FindProtocolRowByTxPduId(PduIdType Id, uint8* ProtocolId, uint16* connectionId);
/**
 * @brief         This function checks whether all the current State of running protocol except input protocol are idle
 *                or not
 * @param[in]     protocolId : the input protocolId
 * @return        Std_ReturnType
 * @retval        TRUE  : All protocols are idle
 * @retval        FALSE : at least one protocol is not idle
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL boolean DslInternal_CheckIdle(uint8 protocolId);

#if (STD_ON == DCM_DEV_ERROR_DETECT)
/**
 * @brief         This function checks common params and initialization status for callbacks
 * @param[in]     id            : Identification of the I-PDU.
 * @param[in]     info          : Pointer to a PduInfoType structure containing the payload data (without protocol
 *                                information) and payload length of the first frame or single frame of a transport
 *                                protocol I-PDU reception, and the MetaData related to this PDU. If neither
 *                                first/single frame data nor MetaData are available, this parameter is set to NULL_PTR.
 * @param[in]     bufferSizePtr : Available receive buffer in the receiving module. This parameter will be used to
 *                                compute the Block Size (BS) in the transport protocol module.
 * @param[in]     apiId         : The Id of the api
 * @return        uint8         : the result det error id
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL uint8
    DslInternal_RxCheckParam(PduIdType id, const PduInfoType* info, const PduLengthType* bufferSizePtr, uint8 apiId);

/**
 * @brief         This function checks params for copyTxData
 * @param[in]     id               : Identification of the I-PDU.
 * @param[in]     info             : Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 *                                   (SduLength). If not enough transmit data is available, no data is copied by the
 *                                   upper layer module and BUFREQ_E_BUSY is returned. The lower layer module may retry
 *                                   the call. An SduLength of 0 can be used to indicate state changes in the retry
 *                                   parameter or to query the current amount of available data in the upper layer
 *                                   module. In this case, the Sdu DataPtr may be a NULL_PTR.
 * @param[in]     availableDataPtr : Indicates the remaining number of bytes that are available in the upper layer
 *                                   module's Tx buffer. availableDataPtr can be used by TP modules that support dynamic
 *                                   payload lengths (e.g. FrIsoTp) to determine the size of the following CFs.
 * @return        uint8            : the result det error id
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL uint8
    DslInternal_CopyTxDataCheckParam(PduIdType id, const PduInfoType* info, const PduLengthType* availableDataPtr);
#endif

/**
 * @brief         This function do internal process for tpRxIndication
 * @param[in]     id           : Identification of the I-PDU.
 * @param[in]     protocolId   : current protocol id
 * @param[in]     connectionId : current connection id
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DslInternal_RxProcessRequest(PduIdType id, uint8 protocolId, uint16 connectionId);

/**
 * @brief         This function do internal process for fullComMode entered
 * @param[in]     connectionId : current connection id
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DslInternal_HandleFullComMode(uint16 connectionId);

/**
 * @brief         This function do internal process for copyTxData when sduLength > 0
 * @param[in]     connectionId : current connection id
 * @param[inout]  info : input pduInfo
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL BufReq_ReturnType DslInternal_HandleTxDataCopy(uint16 connectionId, const PduInfoType* info);

/**
 * @brief         This function handle startOfReception when the buffer is enough for storing data
 * @param[in]     connectionId  : current connection id
 * @param[in]     id            : input pduId
 * @param[in]     TpSduLength   : total request data length
 * @param[in]     info          : the input pduInfo
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL BufReq_ReturnType DslInternal_HandleStartOfReception(
    PduIdType          id,
    uint16             connectionId,
    PduLengthType      TpSduLength,
    const PduInfoType* info);

/**
 * @brief         This function handle tpTxConfirmation when the result is E_OK and condition check succeeds
 * @param[in]     protocolId : current protocol id
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 *
 */
DCM_LOCAL void DslInternal_HandleTpTxConfirmation(uint8 protocolId);

/**
 * @brief         This function sets from default session to non-default session
 * @param[in]     ProtocolId : current protocol id
 * @return        Std_ReturnType
 * @retval        E_OK          : the switch is succeeds
 * @retval        DCM_E_PENDING : the switch is pending
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType DslInternal_DefaultToNonDefault(uint8 ProtocolId);

/**
 * @brief         This handle copyTxData for positive response
 * @param[in]     protocolId : current protocol id
 * @param[inout]  info       : input pduInfo
 * @return        BufReq_ReturnType
 * @retval        BUFREQ_OK       : copy is successful
 * @retval        BUFREQ_E_NOT_OK : copy is not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL BufReq_ReturnType DslInternal_PosRespTxDataCopy(uint8 protocolId, const PduInfoType* info);

/**
 * @brief         This handle copyTxData for negative response
 * @param[in]     protocolId : current protocol id
 * @param[inout]  info       : input pduInfo
 * @return        BufReq_ReturnType
 * @retval        BUFREQ_OK       : copy is successful
 * @retval        BUFREQ_E_NOT_OK : copy is not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL BufReq_ReturnType DslInternal_NegRespTxDataCopy(uint8 protocolId, const PduInfoType* info);

/**
 * @brief         This process actual copy of copyRxData
 * @param[in]     id         : input pduId
 * @param[in]     info       : input pduInfo
 * @param[in]     protocolId : current protocol id
 * @return        BufReq_ReturnType
 * @retval        BUFREQ_OK       : copy is successful
 * @retval        BUFREQ_E_NOT_OK : copy is not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL BufReq_ReturnType DslInternal_ProcessCopyRxData(PduIdType id, const PduInfoType* info, uint8 protocolId);

#if (STD_ON == DCM_UDS_0X86)
/**
 * @brief         check copyTxData for Roe protocol
 * @param[in]     id           : input pduId
 * @param[in]     ProtocolId   : current protocol id
 * @param[out]    connectionId : the matched connection id
 * @return        boolean
 * @retval        TRUE  : it is an Roe response
 * @retval        FALSE : it is not an Roe response
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL boolean Dcm_CopyTxDataCheckRoe(PduIdType id, uint8* ProtocolId, uint16* connectionId);
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE_FAST
#include "Dcm_MemMap.h"
/**
 * check the occupancy of protocol buffer and return accordingly, copy data if provided and return BUFREQ_OK
 */
BufReq_ReturnType
    Dcm_StartOfReception(PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr)
{
    BufReq_ReturnType     bufResult    = BUFREQ_OK;
    uint8                 protocolId   = 0u;
    uint16                connectionId = 0u;
    Dcm_ProtocolCtrlType* protocolCtrlPtr;
    Dcm_MsgContextType*   msgContextPtr;
    /* PRQA S 3415 ++ */ /* VL_Dcm_3415 */
    if (
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        (0u == DslInternal_RxCheckParam(id, info, bufferSizePtr, DCM_STARTOFRECEPTION_ID)) &&
#endif
        (E_OK == DslInternal_FindProtocolRowByRxPduId(id, &protocolId, &connectionId))
        && (Dcm_RequestRejectedDueToBoot == FALSE))
    /* PRQA S 3415 -- */
    {
        const Dcm_DslProtocolRowType* DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[protocolId];
        protocolCtrlPtr                                    = &Dcm_ProtocolCtrl[protocolId];
        msgContextPtr                                      = &Dcm_MsgContext[protocolId];
        /* total length is 0 or provided data length is over total length*/
        if ((0u == TpSduLength) || (info->SduLength > TpSduLength))
        {
            bufResult = BUFREQ_E_NOT_OK;
        }
        /* rxBuffer size is enough for storing all data */
        else if (TpSduLength <= DcmDslProtocolRowPtr->RxBufferSize)
        {
            bufResult = DslInternal_HandleStartOfReception(id, connectionId, TpSduLength, info);
        }
        else
        {
            bufResult = BUFREQ_E_OVFL;
        }

        /* Request is accepted, do further process */
        if ((BUFREQ_OK == bufResult) && (!protocolCtrlPtr->ConcurrentTesterPresent))
        {
            Dcm_DslProtocolRxAddrType rxAddr = Dcm_DslProtocolRx[id].AddrType;

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            /* further process queued request, store infos in queued control unit */
            Dcm_QueuedRequestCtrlType* queuedCtrl = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
            if (DCM_PENDING_REQUEST_RECEIVE == queuedCtrl->PendingRequestState)
            {
                queuedCtrl->QueuedRequestProtocolId   = protocolId;
                queuedCtrl->QueuedRequestRxPduId      = id;
                queuedCtrl->QueuedRequestConnectionId = connectionId;
                queuedCtrl->QueuedRequestLength       = TpSduLength - 1u;
                queuedCtrl->QueuedRxCopyLen           = info->SduLength;
                queuedCtrl->QueuedRequestOldBuffer    = msgContextPtr->reqData;
                *bufferSizePtr                        = Dcm_DslProtocolRow[protocolId].RxBufferSize - info->SduLength;
                DcmInternal_Memcpy(Dcm_RequestQueuedBuffer[Dcm_QueuedIndex], info->SduDataPtr, info->SduLength);
                if (TRUE == Dcm_DslMainConnection[connectionId].MainConnectionUseGenericConnection)
                {
                    DcmInternal_Memcpy(protocolCtrlPtr->MetaDataPtr, info->MetaDataPtr, (uint32)DCM_METADATA_SIZE);
                }
            }
            else
            {
                /* restore reqData pointer for non-queued request */
                if (NULL_PTR != queuedCtrl->QueuedRequestOldBuffer)
                {
                    msgContextPtr->reqData = queuedCtrl->QueuedRequestOldBuffer;
                }
#endif
                /* store infos in protocol and message control unit */
                protocolCtrlPtr->RxPduId          = id;
                protocolCtrlPtr->ConnectionId     = connectionId;
                msgContextPtr->msgAddInfo.reqType = (uint8)rxAddr;
                msgContextPtr->reqDataLen         = (Dcm_MsgLenType)TpSduLength - 1uL;
                /* only process for sdulength != 0 */
                if (info->SduLength >= 1u)
                {
                    /* assign sid */
                    protocolCtrlPtr->Sid = info->SduDataPtr[0u];
                    /* copy the rest */
                    if (info->SduLength > 1u)
                    {
                        DcmInternal_Memcpy(
                            msgContextPtr->reqData,
                            &info->SduDataPtr[1u],
                            (uint32)info->SduLength - 1uL);
                    }
                }
                protocolCtrlPtr->RxCopyLen = info->SduLength;
                *bufferSizePtr =
                    (PduLengthType)Dcm_DslProtocolRow[protocolId].RxBufferSize - (PduLengthType)info->SduLength;
                if (Dcm_DslMainConnection[connectionId].MainConnectionUseGenericConnection)
                {
                    DcmInternal_Memcpy(protocolCtrlPtr->MetaDataPtr, info->MetaDataPtr, (uint32)DCM_METADATA_SIZE);
                }
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            }
#endif
        }
    }
    else
    {
        bufResult = BUFREQ_E_NOT_OK;
    }

    return bufResult;
}

/**
 * check current copied length and whether in the process of reception, returns accordingly, copy data if provided and
 * return BUFREQ_OK, provide remaining available bufferSize
 */
BufReq_ReturnType Dcm_CopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr)
{
    BufReq_ReturnType bufResult  = BUFREQ_E_NOT_OK;
    uint8             protocolId = 0u;
    uint16            connectionId;

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (0u == DslInternal_RxCheckParam(id, info, bufferSizePtr, DCM_COPYRXDATA_ID))
#endif
    {
        (void)DslInternal_FindProtocolRowByRxPduId(id, &protocolId, &connectionId);
        const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        Dcm_QueuedRequestCtrlType* queuedCtrl = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
        if ((queuedCtrl->QueuedRequestRxPduId == id) && (queuedCtrl->QueuedRequestProtocolId == protocolId)
            && (DCM_PENDING_REQUEST_RECEIVE == queuedCtrl->PendingRequestState))
        {
            if ((queuedCtrl->QueuedRxCopyLen + info->SduLength) <= (queuedCtrl->QueuedRequestLength + 1u))
            {
                queuedCtrl->QueuedRxCopyLen += info->SduLength;
                *bufferSizePtr = Dcm_DslProtocolRow[protocolId].RxBufferSize - queuedCtrl->QueuedRxCopyLen;
                DcmInternal_Memcpy(
                    &Dcm_RequestQueuedBuffer[Dcm_QueuedIndex][queuedCtrl->QueuedRxCopyLen],
                    info->SduDataPtr,
                    info->SduLength);
                bufResult = BUFREQ_OK;
            }
            else
            {
                DslInternal_InitChannelQueuedRequestCtrl(Dcm_QueuedIndex);
            }
        }
        else
#endif
        {
            bufResult = BUFREQ_OK;
            /* only process for sdulength > 0 */
            if (info->SduLength > 0u)
            {
                bufResult = DslInternal_ProcessCopyRxData(id, info, protocolId);
            }
            *bufferSizePtr =
                (PduLengthType)Dcm_DslProtocolRow[protocolId].RxBufferSize - (PduLengthType)protocolCtrlPtr->RxCopyLen;
        }
    }

    return bufResult;
}

/**
 * check current copied length (equal to reqeusted length) and whether in the process of reception, set state to
 * DCM_MSG_RECEIVED if successful
 */
void Dcm_TpRxIndication(PduIdType id, Std_ReturnType result)
{
    uint8  protocolId   = 0u;
    uint16 connectionId = 0u;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    uint8 errorId = 0u;
    if (NULL_PTR == Dcm_CfgPtr)
    {
        errorId = DCM_E_UNINIT;
    }
    else if (E_NOT_OK == DslInternal_FindProtocolRowByRxPduId(id, &protocolId, &connectionId))
    {
        errorId = DCM_E_PARAM;
    }
    else
#endif
    {
#if (STD_OFF == DCM_DEV_ERROR_DETECT)
        (void)DslInternal_FindProtocolRowByRxPduId(id, &protocolId, &connectionId);
#endif
        if (E_OK == result)
        {
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            Dcm_QueuedRequestCtrlType* queuedCtrl = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
            if ((queuedCtrl->QueuedRequestRxPduId == id) && (queuedCtrl->QueuedRequestProtocolId == protocolId)
                && (DCM_PENDING_REQUEST_RECEIVE == queuedCtrl->PendingRequestState))
            {
                /* check total copy length */
                if (queuedCtrl->QueuedRxCopyLen == (queuedCtrl->QueuedRequestLength + 1u))
                {
                    queuedCtrl->PendingRequestState = DCM_PENDING_REQUEST_READY;
                }
                else
                {
                    DslInternal_InitChannelQueuedRequestCtrl(Dcm_QueuedIndex);
                }
            }
            else
#endif
            {
                DslInternal_RxProcessRequest(id, protocolId, connectionId);
            }
        }
        else
        {
            /* result param is E_NOT_OK, stop the reception and reset channel */
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            Dcm_QueuedRequestCtrlType* queuedCtrl = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
            if ((queuedCtrl->QueuedRequestRxPduId == id) && (queuedCtrl->QueuedRequestProtocolId == protocolId)
                && (DCM_PENDING_REQUEST_RECEIVE == queuedCtrl->PendingRequestState))
            {
                DslInternal_InitChannelQueuedRequestCtrl(Dcm_QueuedIndex);
            }
            else
#endif
            {
                /** S3 Server error handling : Restart S3Server timer (because it has been stopped based on the
                 * previously received StartOfMessage indication). The server shall ignore the request.T_Data.ind that
                 * indicates an error during the reception of a multi-frame request message.  */
                if ((DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
                    && (Dcm_SessionCtrl.CurrentSessionProtocolId == protocolId))
                {
                    Dcm_SessionCtrl.SessionTimer = DCM_S3_TIMEOUT;
                }
                DslInternal_SetStateIdle(protocolId, NULL_PTR, FALSE, FALSE);
            }
        }
    }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (0u != errorId)
    {
        DCM_DET_REPORT(DCM_TPRXINDICATION_ID, errorId);
    }
#endif
}

/**
 * check current copied length and whether in the process of transmission and return accordingly, provide txData and
 * remaining available data size
 */
BufReq_ReturnType
    Dcm_CopyTxData(PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr)
{
    BufReq_ReturnType bufResult = BUFREQ_OK;
    uint8             protocolId;
    uint16            connectionId;
    DCM_UNUSED(retry);

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (0u == DslInternal_CopyTxDataCheckParam(id, info, availableDataPtr))
#endif
    {
#if (STD_ON == DCM_UDS_0X86)
        if (Dcm_CopyTxDataCheckRoe(id, &protocolId, &connectionId) == FALSE)
#endif
        {
            (void)DslInternal_FindProtocolRowByTxPduId(id, &protocolId, &connectionId);
        }
        const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
        if ((DCM_MSG_SENT == protocolCtrlPtr->State) || (DCM_MSG_PENDING_SENT == protocolCtrlPtr->State)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            || ((DCM_MSG_PAGEDBUFFER_SENT == protocolCtrlPtr->State) && (TRUE == protocolCtrlPtr->PagedBufferStarted))
#endif
        )
        /* only process when sduLength > 0 */
        {
            if (info->SduLength > 0u)
            {
                bufResult = DslInternal_HandleTxDataCopy(connectionId, info);
            }
            if (DCM_POS_RESP == protocolCtrlPtr->ErrorCode)
            {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
                if (TRUE == protocolCtrlPtr->PagedBufferStarted)
                {
                    *availableDataPtr = protocolCtrlPtr->RemainPageLength;
                }
#else
                const Dcm_MsgContextType* msgContextPtr = &Dcm_MsgContext[protocolId];
                *availableDataPtr =
                    (PduLengthType)msgContextPtr->resDataLen + 1u - (PduLengthType)protocolCtrlPtr->TxCopyLen;
#endif
            }
            else
            {
                *availableDataPtr = (PduLengthType)DCM_NEGATIVE_RSPMSGLEN - (PduLengthType)protocolCtrlPtr->TxCopyLen;
            }
        }
        else
        {
            bufResult = BUFREQ_E_NOT_OK;
        }
    }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    else
    {
        bufResult = BUFREQ_E_NOT_OK;
    }
#endif

    return bufResult;
}

/**
 * confirmation for response transmission, initialize protocol control and transfer confirmaton to Dsp
 */
void Dcm_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
    uint8                      protocolId;
    Dcm_ConfirmationStatusType confirmationStatus;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    uint8 errorId = 0u;
    if (NULL_PTR == Dcm_CfgPtr)
    {
        errorId = DCM_E_UNINIT;
    }
    else if (
#if (STD_ON == DCM_UDS_0X86)
        (Dcm_CopyTxDataCheckRoe(id, &protocolId, NULL_PTR) == FALSE) &&
#endif
        (E_NOT_OK == DslInternal_FindProtocolRowByTxPduId(id, &protocolId, NULL_PTR)))
    {
        errorId = DCM_E_PARAM;
    }
    else
#endif
    {
#if (STD_OFF == DCM_DEV_ERROR_DETECT)
#if (STD_ON == DCM_UDS_0X86)
        if (Dcm_CopyTxDataCheckRoe(id, &protocolId, NULL_PTR) == FALSE)
#endif
        {
            (void)DslInternal_FindProtocolRowByTxPduId(id, &protocolId, NULL_PTR);
        }
#endif
        const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
        if (E_OK == result)
        {
            DslInternal_HandleTpTxConfirmation(protocolId);
        }
        else
        {
            /** S3 Server error handling :T_Data.con from transport/network layer with a negative result value. Restart
             * S3Server timer (because it has been stopped based on the previously received request message). The server
             * shall not perform a retransmission of the response message. */
            if ((DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
                && (Dcm_SessionCtrl.CurrentSessionProtocolId == protocolId))
            {
                Dcm_SessionCtrl.SessionTimer = DCM_S3_TIMEOUT;
            }
            if (DCM_POS_RESP != protocolCtrlPtr->ErrorCode)
            {
                confirmationStatus = DCM_RES_NEG_NOT_OK;
                DslInternal_SetStateIdle(protocolId, &confirmationStatus, FALSE, FALSE);
            }
            else
            {
                confirmationStatus = DCM_RES_POS_NOT_OK;
                DslInternal_SetStateIdle(protocolId, &confirmationStatus, FALSE, FALSE);
            }
        }
    }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (0u != errorId)
    {
        DCM_DET_REPORT(DCM_TPTXCONFIRMATION_ID, errorId);
    }
#endif
}

/**
 * confirmation for periodic transmission
 */
void Dcm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    uint8 errorId = 0u;
    if (NULL_PTR == Dcm_CfgPtr)
    {
        errorId = DCM_E_UNINIT;
    }
    else
#endif
    {
        DCM_UNUSED(result);
#if ((STD_ON == DCM_UDS_0X2A) && (DCM_PERIODIC_CONNECTION_NUM > 0u))
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if (TxPduId >= DCM_PERIODIC_CONNECTION_NUM) /* PRQA S 2991,2995 */ /* VL_QAC_2991,VL_Dcm_2995 */
        {
            errorId = DCM_E_PARAM;
        }
        else
#endif
        {
            Dcm_PeriodicConnectionSent[TxPduId] = FALSE; /* PRQA S 2880 */ /* VL_Dcm_2880 */
        }
#else
        DCM_UNUSED(TxPduId);
#endif
    }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (0u != errorId) /* PRQA S 2991,2995 */ /* VL_QAC_2991,VL_Dcm_2995 */
    {
        DCM_DET_REPORT(DCM_TXCONFIRMATION_ID, errorId);
    }
#endif
}
#define DCM_STOP_SEC_CODE_FAST
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * set variable to activate and deactivate the call of ComM_DCM_ActiveDiagnostic() function.
 */
/* PRQA S 0624++ */ /* VL_Dcm_0624 */
Std_ReturnType Dcm_SetActiveDiagnostic(boolean active)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_SETACTIVEDIAGNOSTIC_ID, DCM_E_UNINIT);
    }
    else
#endif
    {
        if (!active)
        {
            Dcm_ActiveDiagnostic = DCM_COMM_NOT_ACTIVE;
        }
        else
        {
            Dcm_ActiveDiagnostic = DCM_COMM_ACTIVE;
        }
    }

    return E_OK;
}
/* PRQA S 0624-- */ /* VL_Dcm_0624 */

/**
 * set variable to change to no communication mode
 */
void Dcm_ComM_NoComModeEntered(uint8 NetworkId)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_COMM_NOCOMMODEENTERED, DCM_E_UNINIT);
    }
    else
#endif
    {
        for (uint8 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
        {
            if (Dcm_DslMainConnection[index].ProtocolComMChannelRef == NetworkId)
            {
                Dcm_CommState[index] = DCM_COMM_NO_COMMUNICATION;
            }
        }
    }
}

/**
 * set variable to change to silent communication mode (receive only)
 */
void Dcm_ComM_SilentComModeEntered(uint8 NetworkId)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_COMM_SILENTCOMMODEENTERED, DCM_E_UNINIT);
    }
    else
#endif
    {
        for (uint8 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
        {
            if (Dcm_DslMainConnection[index].ProtocolComMChannelRef == NetworkId)
            {
                Dcm_CommState[index] = DCM_COMM_SILENT_COMMUNICATION;
            }
        }
    }
}

/**
 * set variable to change to full communication mode
 */
void Dcm_ComM_FullComModeEntered(uint8 NetworkId)
{
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if (NULL_PTR == Dcm_CfgPtr)
    {
        DCM_DET_REPORT(DCM_COMM_FULLCOMMODEENTERED, DCM_E_UNINIT);
    }
    else
#endif
    {
        for (uint16 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
        {
            if (Dcm_DslMainConnection[index].ProtocolComMChannelRef == NetworkId)
            {
                DslInternal_HandleFullComMode(index);
            }
        }
    }
}

/**
 * initialize Dcm_SessionCtrl
 */
void DslInternal_SessionInit(void)
{
    Dcm_SessionCtrl.CurrentSession       = DCM_DEFAULT_SESSION;
    Dcm_SessionCtrl.NewSession           = DCM_INVALID_UINT8;
    Dcm_SessionCtrl.NewSessionProtocolId = DCM_INVALID_UINT8;
    Dcm_SessionCtrl.SessionTimer         = 0u;
}

/**
 * initialize securityCtrlPtr
 */
void DslInternal_SecurityInit(void)
{
#if (STD_ON == DCM_UDS_0X27)
    for (uint8 index = 0u; index < DCM_SECURITY_NUM; index++)
    {
        Dcm_SecurityCtrlType* securityCtrlPtr = &Dcm_SecurityCtrl[index];
        securityCtrlPtr->SecurityTimer        = 0u;
        securityCtrlPtr->AttemptCounter       = 0u;
#if (STD_ON == DCM_SECURITY_READOUT)
        securityCtrlPtr->InitialGetAttemptCounter = Dcm_DspSecurityRow[index].AttemptCounterEnabled;
        securityCtrlPtr->PendingGetAttemptCounter = FALSE;
        securityCtrlPtr->PendingSetAttemptCounter = FALSE;
        securityCtrlPtr->DelayedSetAttemptCounter = FALSE;
        /* PRQA S 3494 ++ */ /* VL_Dcm_3494 */
        Dcm_SecurityReadoutTimer = (DCM_SECURITY_MAX_ATTEMPT_COUNTER_READOUT_TIME == 0u)
                                       ? DCM_MAINFUNCTION_PERIOD
                                       : DCM_SECURITY_MAX_ATTEMPT_COUNTER_READOUT_TIME;
        /* PRQA S 3494 -- */
#endif
    }
#endif
    Dcm_CurrentSecurity = DCM_SEC_LEV_LOCKED;
}

/**
 * initialize Dcm_ProtocolCtrl
 */
void DslInternal_ProtocolInit(void)
{
#if (0u != DCM_METADATA_NUM)
    uint8 metaDataIndex = 0u;
#endif

    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
        Dcm_ProtocolCtrlType*         protocolCtrlPtr      = &Dcm_ProtocolCtrl[index];
        const Dcm_DslProtocolRowType* DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[index];
        Dcm_MsgContextType*           msgContextPtr        = &Dcm_MsgContext[index];
        protocolCtrlPtr->State                             = DCM_MSG_IDLE;
        protocolCtrlPtr->ProtocolActive                    = FALSE;
        protocolCtrlPtr->OpStatus                          = DCM_INITIAL;
        protocolCtrlPtr->ServIndex                         = DCM_INVALID_UINT8;
        protocolCtrlPtr->SubServIndex                      = DCM_INVALID_UINT8;
        protocolCtrlPtr->RxCopyLen                         = 0u;
        protocolCtrlPtr->TxCopyLen                         = 0u;
#if (0u != DCM_METADATA_NUM)
        if (DcmDslProtocolRowPtr->ProtocolUseGenericConnection)
        {
            protocolCtrlPtr->MetaDataPtr = &Dcm_MetaData[metaDataIndex * DCM_METADATA_SIZE];
            metaDataIndex++;
        }
        else
#endif
        {
            protocolCtrlPtr->MetaDataPtr = NULL_PTR;
        }
        protocolCtrlPtr->P2Timer                 = 0u;
        protocolCtrlPtr->PendingCounter          = 0u;
        protocolCtrlPtr->ConcurrentTesterPresent = FALSE;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        DslInternal_PagedBufferInit(protocolCtrlPtr);
#endif
        msgContextPtr->reqData                        = &Dcm_Buffer[DcmDslProtocolRowPtr->RxBufferOffset];
        msgContextPtr->reqDataLen                     = 0u;
        msgContextPtr->resData                        = &Dcm_Buffer[DcmDslProtocolRowPtr->TxBufferOffset];
        msgContextPtr->resDataLen                     = 0u;
        msgContextPtr->msgAddInfo.reqType             = 0u;
        msgContextPtr->msgAddInfo.suppressPosResponse = 0u;
        msgContextPtr->resMaxDataLen                  = DcmDslProtocolRowPtr->TxBufferSize;
        msgContextPtr->idContext                      = 0u;
        msgContextPtr->dcmRxPduId                     = 0u;
    }

    for (uint8 i = 0u; i < DCM_MAINCONNECTION_NUM; i++)
    {
        Dcm_DiagnosticState[i] = DCM_INACTIVE;
    }
}

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
/**
 * initialize Dcm_ProtocolCtrl pagebuffer contents
 */
void DslInternal_PagedBufferInit(Dcm_ProtocolCtrlType* protocolCtrlPtr)
{
    protocolCtrlPtr->PagedBufferStarted = FALSE;
    protocolCtrlPtr->FirstPageLength    = 0u;
    protocolCtrlPtr->RemainPageLength   = 0u;
}
#endif

/**
 * find configured protocolRow index based on rxPduId(which has the same index as DcmDslProtocolRx)
 */
Std_ReturnType DslInternal_FindProtocolRowByRxPduId(PduIdType Id, uint8* ProtocolId, uint16* connectionId)
{
    Std_ReturnType result = E_OK;

    if (Id >= DCM_RXPDU_NUM)
    {
        result = E_NOT_OK;
    }
    else
    {
        uint16 tempConnectionId = Dcm_DslProtocolRx[Id].ParentMainConnectionId;
        if ((NULL_PTR != Dcm_CfgPtr)
            && (Dcm_CfgPtr->ProtocolRowUsed[Dcm_DslMainConnection[tempConnectionId].ParentProtocolId]))
        {
            if (NULL_PTR != connectionId)
            {
                *connectionId = tempConnectionId;
            }
            if (NULL_PTR != ProtocolId)
            {
                *ProtocolId = Dcm_DslMainConnection[tempConnectionId].ParentProtocolId;
            }
        }
        else
        {
            result = E_NOT_OK;
        }
    }

    return result;
}

/**
 * call by Dcm_MainFunction to asynchronously transmit, call PduR_DcmTransmit to send data, pduInfo is from
 * Dcm_TempPduInfo(asynchronously stored when calling DsdInternal_SendResponse)
 */
void DslInternal_ProcessTransmit(uint8 ProtocolId)
{
    uint16 connectionId = 0u;

    (void)DslInternal_FindProtocolRowByRxPduId(Dcm_MsgContext[ProtocolId].dcmRxPduId, NULL_PTR, &connectionId);

    if (NULL_PTR != DCM_PROTOCOL_TX(connectionId))
    {
        Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[ProtocolId];
        Std_ReturnType        result;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (TRUE == protocolCtrlPtr->PagedBufferStarted)
        {
            protocolCtrlPtr->State = DCM_MSG_PAGEDBUFFER_SENT;
        }
        else
#endif
        {
            protocolCtrlPtr->State = DCM_MSG_SENT;
        }
        result = PduR_DcmTransmit(*DCM_PROTOCOL_TX(connectionId), &Dcm_TempPduInfo[ProtocolId]);
        if (E_NOT_OK == result)
        {
            Dcm_ConfirmationStatusType confirmationStatus;
            if (DCM_POS_RESP != protocolCtrlPtr->ErrorCode)
            {
                confirmationStatus = DCM_RES_NEG_NOT_OK;
            }
            else
            {
                confirmationStatus = DCM_RES_POS_NOT_OK;
            }
            DslInternal_SetStateIdle(ProtocolId, &confirmationStatus, TRUE, FALSE);
        }
        else if (DCM_E_RESPONSE_PENDING == protocolCtrlPtr->ErrorCode)
        {
            protocolCtrlPtr->PendingCounter++;
            protocolCtrlPtr->State = DCM_MSG_PENDING_SENT;
            uint8 sessionIndex;
            (void)DcmInternal_FindSessionIndex(Dcm_SessionCtrl.CurrentSession, &sessionIndex);
            protocolCtrlPtr->P2Timer = Dcm_DspSessionRow[sessionIndex].P2StarServerMax
                                       - Dcm_DslProtocolRow[ProtocolId].P2StarServerAdjust - DCM_MAINFUNCTION_PERIOD;
        }
        else
        {
            /** idle */
        }
    }
}

/**
 * set new securitylevel
 */
void DslInternal_SetSecurityLevel(Dcm_SecLevelType SecurityLevel)
{
    SchM_Enter_Dcm_ExclusiveArea();
#if (STD_ON == DCM_UDS_0X27)
    if (DCM_SEC_LEV_LOCKED == SecurityLevel)
    {
        Dcm_reqSeedSecurityLevel = 0u;
    }
#endif
    if (Dcm_CurrentSecurity != SecurityLevel)
    {
        Dcm_CurrentSecurity = SecurityLevel;
        SchM_Exit_Dcm_ExclusiveArea();
        (void)Rte_Switch_SecurityAccessModeSwitchInterface_securityAccess(SecurityLevel);
        (void)SchM_Switch_Dcm_DcmSecurityAccess(SecurityLevel);
#if ((STD_ON == DCM_UDS_0X2A) && (STD_ON == DCM_DYN_DID) && (STD_ON == DCM_DDDID_CHECK_SOURCE))
        Dcm_UDS0x2A_StatusChangeHandle();
#endif
    }
    else
    {
        SchM_Exit_Dcm_ExclusiveArea();
    }
}

/**
 * set current session to default session, only do session change synchronously when Immediate is TRUE, otherwise wait
 * until TpTxConfirmation
 */
void DslInternal_ResetToDefaultSession(boolean Immediate)
{
    Dcm_SessionCtrl.NewSession = DCM_DEFAULT_SESSION;
    if (Immediate)
    {
        Dcm_UDS0x10_SessionChange();
    }
}

/**
 * set current session to target session, need to check other protocols' occupancy when change to non-default session
 */
Std_ReturnType DslInternal_SetSesCtrlType(uint8 ProtocolId, Dcm_SesCtrlType SesCtrlType)
{
    Std_ReturnType result = E_OK;

    if (SesCtrlType != Dcm_SessionCtrl.CurrentSession)
    {
        /* Switch from default session to non-default one */
        if (DCM_DEFAULT_SESSION == Dcm_SessionCtrl.CurrentSession)
        {
            result = DslInternal_DefaultToNonDefault(ProtocolId);
        }

        if (DCM_E_PENDING != result)
        {
            Dcm_SessionCtrl.NewSession           = SesCtrlType;
            Dcm_SessionCtrl.NewSessionProtocolId = ProtocolId;
        }
    }
    else
    {
        DslInternal_SetSecurityLevel(DCM_SEC_LEV_LOCKED);
    }
    return result;
}

/**
 * set current diagnostic state, notify ComM accordingly
 */
void DslInternal_SetDiagnosticState(uint16 ConnectionId, Dcm_DiagnosticStateType diagnosticState)
{
    SchM_Enter_Dcm_ExclusiveArea();
    if (diagnosticState != Dcm_DiagnosticState[ConnectionId])
    {
        Dcm_DiagnosticState[ConnectionId] = diagnosticState;
        SchM_Exit_Dcm_ExclusiveArea();
        if ((DCM_ACTIVE == diagnosticState) && (DCM_COMM_ACTIVE == Dcm_ActiveDiagnostic))
        {
            ComM_DCM_ActiveDiagnostic(Dcm_DslMainConnection[ConnectionId].ProtocolComMChannelRef);
        }
        else if (DCM_INACTIVE == diagnosticState)
        {
            ComM_DCM_InactiveDiagnostic(Dcm_DslMainConnection[ConnectionId].ProtocolComMChannelRef);
        }
        else
        {
            /* idle */
        }
    }
    else
    {
        SchM_Exit_Dcm_ExclusiveArea();
    }
}

/**
 * reset protocol state to idle, initialize members, deal with confirmation notification synchronously if
 * immediateHandle is TRUE, otherwise store related info in Dcm_ConfirmationHandle
 */
void DslInternal_SetStateIdle(
    uint8                             protocolId,
    const Dcm_ConfirmationStatusType* confirmationStatus,
    boolean                           immediateHandle,
    boolean                           reachDsp)
{
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
    uint16                connectionId    = protocolCtrlPtr->ConnectionId;

    if (DCM_MSG_IDLE != protocolCtrlPtr->State)
    {
        if ((DCM_PENDING == protocolCtrlPtr->OpStatus) || (DCM_FORCE_RCRRP_OK == protocolCtrlPtr->OpStatus))
        {
            protocolCtrlPtr->OpStatus = DCM_CANCEL;
        }
        else
        {
            protocolCtrlPtr->OpStatus = DCM_INITIAL;
            if (reachDsp)
            {
                DspInternal_DcmConfirmation(protocolId, connectionId);
            }
            if (NULL_PTR != confirmationStatus)
            {
                if (immediateHandle)
                {
                    DsdInternal_HandleConfirmation(protocolId, *confirmationStatus);
                }
                else
                {
                    Dcm_ConfirmationHandleType* confirmationHandlePtr = &Dcm_ConfirmationHandle[protocolId];
                    confirmationHandlePtr->Pending                    = TRUE;
                    confirmationHandlePtr->Sid                        = protocolCtrlPtr->Sid;
                    confirmationHandlePtr->ReqType                    = Dcm_MsgContext[protocolId].msgAddInfo.reqType;
                    confirmationHandlePtr->ConnectionId               = connectionId;
                    confirmationHandlePtr->ConfirmationStatus         = *confirmationStatus;
                }
            }

            protocolCtrlPtr->RxPduId      = 0u;
            protocolCtrlPtr->ConnectionId = DCM_INVALID_UINT16;
            if (DslInternal_CheckIdle(protocolId))
            {
                DcmInternal_SetActiveDiagnostic(FALSE);
            }
            Dcm_MsgContext[protocolId].resDataLen = 0u;
            protocolCtrlPtr->P2Timer              = 0u;
            protocolCtrlPtr->PendingCounter       = 0u;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            protocolCtrlPtr->PagedBufferStarted = FALSE;
            protocolCtrlPtr->FirstPageLength    = 0u;
            protocolCtrlPtr->RemainPageLength   = 0u;
#endif
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_QueuedRequestCtrlType* curQueuedCtrl   = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
            uint8                      tempQueueIndex  = (Dcm_QueuedIndex == 0u) ? 1u : 0u;
            Dcm_QueuedRequestCtrlType* otherQueuedCtrl = &Dcm_QueuedRequestCtrl[tempQueueIndex];
            if ((curQueuedCtrl->QueuedRequestProtocolId == protocolId)
                || (otherQueuedCtrl->QueuedRequestProtocolId == protocolId))
            {
                if ((DCM_PENDING_REQUEST_READY == curQueuedCtrl->PendingRequestState)
                    && (DCM_PENDING_REQUEST_READY == otherQueuedCtrl->PendingRequestState))
                {
                    otherQueuedCtrl->PendingRequestState = DCM_PENDING_REQUEST_PROCESS;
                }
                else if (DCM_PENDING_REQUEST_READY == curQueuedCtrl->PendingRequestState)
                {
                    curQueuedCtrl->PendingRequestState = DCM_PENDING_REQUEST_PROCESS;
                }
                if (DCM_PENDING_REQUEST_PROCESS == curQueuedCtrl->PendingRequestState)
                {
                    DslInternal_InitChannelQueuedRequestCtrl(Dcm_QueuedIndex);
                }
                else if (DCM_PENDING_REQUEST_PROCESS == otherQueuedCtrl->PendingRequestState)
                {
                    DslInternal_InitChannelQueuedRequestCtrl(tempQueueIndex);
                }
                else
                {
                    /* idle */
                }
                SchM_Exit_Dcm_ExclusiveArea();
            }
            else
            {
                SchM_Exit_Dcm_ExclusiveArea();
#endif
                protocolCtrlPtr->State = DCM_MSG_IDLE;
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
            }
#endif
        }
    }
}

#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
/**
 * initialize Dcm_QueuedRequestCtrl for requested index
 */
void DslInternal_InitChannelQueuedRequestCtrl(uint8 index)
{
    Dcm_QueuedRequestCtrlType* queuedRequestCtrlPtr = &Dcm_QueuedRequestCtrl[index];
    SchM_Enter_Dcm_ExclusiveArea();
    queuedRequestCtrlPtr->QueuedRequestProtocolId = DCM_INVALID_UINT8;
    queuedRequestCtrlPtr->PendingRequestState     = DCM_PENDING_REQUEST_NONE;
    queuedRequestCtrlPtr->QueuedRequestRxPduId    = DCM_INVALID_UINT16;
    queuedRequestCtrlPtr->QueuedRxCopyLen         = 0u;
    queuedRequestCtrlPtr->QueuedRequestLength     = 0u;
    SchM_Exit_Dcm_ExclusiveArea();
}
#endif

/**
 * call stopProtocol callouts, return the result
 */
Std_ReturnType DslInternal_StopProtocol(uint8 ProtocolId)
{
    uint16         sourceAddress = 0u;
    Std_ReturnType result;

    DcmInternal_GetMetaDataSATA(ProtocolId, &sourceAddress, NULL_PTR);
#if (DCM_CALLBACK_DCM_REQUEST_SERVICE > 0u)
    for (uint16 index = 0u; index < DCM_CALLBACK_DCM_REQUEST_SERVICE; index++)
    {
        result = Dcm_DslCallbackDCMRequestService[index].StopProtocol(
            Dcm_DslProtocolRow[ProtocolId].ProtocolType,
            sourceAddress,
            Dcm_ProtocolCtrl[ProtocolId].ConnectionId);
        if (E_OK != result)
        {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            if (E_NOT_OK != result)
            {
                DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            }
#endif
            break;
        }
    }
    if (E_OK == result)
#endif
    {
        Dcm_ProtocolCtrl[ProtocolId].ProtocolActive = FALSE;
        result                                      = DcmInternal_CancelJobs(ProtocolId);
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        Dcm_QueuedRequestCtrlType* queuedCtrl = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
        if ((ProtocolId == queuedCtrl->QueuedRequestProtocolId)
            && (DCM_PENDING_REQUEST_NONE != queuedCtrl->PendingRequestState))
        {
            DslInternal_InitChannelQueuedRequestCtrl(Dcm_QueuedIndex);
        }
#endif
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * check whether the input protocol shares buffer with a protocol that is occupied
 */
DCM_LOCAL Std_ReturnType DslInternal_CheckProtocolWithSameBuffer(const Dcm_DslProtocolRowType* protocolRow)
{
    Std_ReturnType result = E_OK;

    for (uint8 index = 0u; index < protocolRow->ShareBufferProtocolIdNum; index++)
    {
        if (DCM_MSG_IDLE != Dcm_ProtocolCtrl[protocolRow->ShareBufferProtocolId[index]].State)
        {
            result = E_NOT_OK;
            break;
        }
    }

    return result;
}

/**
 * find protocol and mainconnection id based on txPduId
 */
DCM_LOCAL Std_ReturnType DslInternal_FindProtocolRowByTxPduId(PduIdType Id, uint8* ProtocolId, uint16* connectionId)
{
    Std_ReturnType result = E_NOT_OK;

    if (Id < DCM_MAINCONNECTION_NUM)
    {
        if (NULL_PTR != ProtocolId)
        {
            *ProtocolId = Dcm_DslMainConnection[Id].ParentProtocolId;
        }
        if (NULL_PTR != connectionId)
        {
            *connectionId = Id;
        }
        result = E_OK;
    }

    return result;
}

/**
 * iterate over all protocol except input one, check whether all protocols are idle
 */
DCM_LOCAL boolean DslInternal_CheckIdle(uint8 protocolId)
{
    boolean idleState = FALSE;

    if (DCM_DEFAULT_SESSION == Dcm_SessionCtrl.CurrentSession)
    {
        idleState = TRUE;
        for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
        {
            if ((DCM_MSG_IDLE != Dcm_ProtocolCtrl[index].State) && (index != protocolId))
            {
                idleState = FALSE;
                break;
            }
        }
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        if ((DCM_PENDING_REQUEST_NONE != Dcm_QueuedRequestCtrl[0u].PendingRequestState)
            || (DCM_PENDING_REQUEST_NONE != Dcm_QueuedRequestCtrl[1u].PendingRequestState))
        {
            idleState = FALSE;
        }
#endif
    }

    return idleState;
}

#if (STD_ON == DCM_DEV_ERROR_DETECT)
/* check condition for rx callbacks */
DCM_LOCAL uint8
    DslInternal_RxCheckParam(PduIdType id, const PduInfoType* info, const PduLengthType* bufferSizePtr, uint8 apiId)
{
    uint8 errorId = 0u;
    if (NULL_PTR == Dcm_CfgPtr)
    {
        errorId = DCM_E_UNINIT;
    }
    else if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
    {
        errorId = DCM_E_PARAM_POINTER;
    }
    else if (E_NOT_OK == DslInternal_FindProtocolRowByRxPduId(id, NULL_PTR, NULL_PTR))
    {
        errorId = DCM_E_PARAM;
    }
    else
    {
        /* idle */
    }

    if (0u != errorId)
    {
        DCM_DET_REPORT(apiId, errorId);
    }

    return errorId;
}

/* check condition for copyTxData */
DCM_LOCAL uint8
    DslInternal_CopyTxDataCheckParam(PduIdType id, const PduInfoType* info, const PduLengthType* availableDataPtr)
{
    uint8 errorId = 0u;
    if (NULL_PTR == Dcm_CfgPtr)
    {
        errorId = DCM_E_UNINIT;
    }
    else if ((NULL_PTR == info) || (NULL_PTR == availableDataPtr))
    {
        errorId = DCM_E_PARAM_POINTER;
    }
    else if (E_NOT_OK == DslInternal_FindProtocolRowByTxPduId(id, NULL_PTR, NULL_PTR))
    {
        errorId = DCM_E_PARAM;
    }
    else
    {
        /* idle */
    }

    if (0u != errorId)
    {
        DCM_DET_REPORT(DCM_COPYTXDATA_ID, errorId);
    }

    return errorId;
}
#endif

/* internal process of tpRxIndication */
/* PRQA S 6010,6080, 5017 ++ */ /* VL_MTR_Dcm_STCYC,VL_MTR_Dcm_STPTH,VL_Dcm_5017 */
DCM_LOCAL void DslInternal_RxProcessRequest(PduIdType id, uint8 protocolId, uint16 connectionId)
{
    Dcm_ProtocolCtrlType*         protocolCtrlPtr      = &Dcm_ProtocolCtrl[protocolId];
    Dcm_MsgContextType*           msgContextPtr        = &Dcm_MsgContext[protocolId];
    const Dcm_DslProtocolRowType* DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[protocolId];
    Dcm_DslProtocolRxAddrType     rxAddr               = Dcm_DslProtocolRx[id].AddrType;

    /* Concurrent Tester Present, do not further process */
    Std_ReturnType furtherProcess = E_OK;

    if ((DCM_FUNCTIONAL_TYPE == rxAddr) && (msgContextPtr->reqDataLen == 1uL)
        && (protocolCtrlPtr->Sid == DCM_UDS_TESTERPRESENT_SID) && (msgContextPtr->reqData[0u] == DCM_SUPPRESS_BIT))
    {
        furtherProcess = E_NOT_OK;
        /* Concurrent Tester Present process and current protocol state may be idle or other */
        if ((DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
            && (Dcm_SessionCtrl.CurrentSessionProtocolId == protocolId))
        {
            Dcm_SessionCtrl.SessionTimer = DCM_S3_TIMEOUT;
        }
    }

    if (E_OK == furtherProcess)
    {
        /* condition not correct, do not further process */
        if ((DCM_MSG_RECEIVING != protocolCtrlPtr->State) || (id != protocolCtrlPtr->RxPduId))
        {
            furtherProcess = E_NOT_OK;
        }
    }

    if (E_OK == furtherProcess)
    {
        /* only process in FULL/SILENT communication */
        if ((DCM_COMM_NO_COMMUNICATION == Dcm_CommState[connectionId])
#if (STD_ON == DCM_UDS_0X11)
            || (Dcm_ResetProcessing)
#endif
        )
        {
            furtherProcess = E_NOT_OK;
        }
        else
        {
            DcmInternal_SetActiveDiagnostic(TRUE);
        }
    }

    if (E_OK == furtherProcess)
    {
        /* total copy length does not match with declared length */
        if (protocolCtrlPtr->RxCopyLen != (msgContextPtr->reqDataLen + 1uL))
        {
            furtherProcess = E_NOT_OK;
        }
#if (DCM_DSL_RX_TX_SHARED_BUGGER == STD_ON)
        else
        {
            if (DcmDslProtocolRowPtr->SharedRxTxBuffer)
            {
                msgContextPtr->resMaxDataLen = DcmDslProtocolRowPtr->TxBufferSize - msgContextPtr->reqDataLen;
                msgContextPtr->resData       = &msgContextPtr->reqData[protocolCtrlPtr->RxCopyLen];
            }
        }
#endif
    }

    if (E_OK == furtherProcess)
    {
        /* asynchronously process Dsd request handling (including indication callout) */
        msgContextPtr->dcmRxPduId = id;
        uint8 sessionIndex;
        (void)DcmInternal_FindSessionIndex(Dcm_SessionCtrl.CurrentSession, &sessionIndex);
        protocolCtrlPtr->P2Timer = (uint32)Dcm_DspSessionRow[sessionIndex].P2ServerMax
                                   - DcmDslProtocolRowPtr->P2ServerAdjust - DCM_MAINFUNCTION_PERIOD;
        protocolCtrlPtr->State = DCM_MSG_RECEIVED;
    }
    else if (!protocolCtrlPtr->ConcurrentTesterPresent)
    {
        /* reset channel, do confirmation callout asynchronously, no need to reach Dsp */
        DslInternal_SetStateIdle(protocolId, NULL_PTR, FALSE, FALSE);
    }
    else
    {
        protocolCtrlPtr->ConcurrentTesterPresent = FALSE;
    }
}
/* PRQA S 6010,6080, 5017-- */ /* VL_MTR_Dcm_STPTH */

DCM_LOCAL void DslInternal_HandleFullComMode(uint16 connectionId)
{
    Dcm_CommState[connectionId]                   = DCM_COMM_FULL_COMMUNICATION;
    const Dcm_DslMainConnectionType* mainConnPtr  = &Dcm_DslMainConnection[connectionId];
    Dcm_ProtocolCtrlType*            protocolCtrl = &Dcm_ProtocolCtrl[mainConnPtr->ParentProtocolId];

    /* check if there is pending response waiting for full communication */
    if ((Dcm_ProgConditions.ResponseRequired)
        && (Dcm_DslMainConnection[connectionId].RxConnectionId == Dcm_ProgConditions.ConnectionId))
    {
        if ((mainConnPtr->ProtocolTx != NULL_PTR) && (DCM_MSG_IDLE == protocolCtrl->State))
        {
            protocolCtrl->State             = DCM_MSG_SENT;
            protocolCtrl->Sid               = Dcm_ProgConditions.Sid;
            Dcm_MsgContextType* pMsgContext = &Dcm_MsgContext[mainConnPtr->ParentProtocolId];
            uint8*              resData     = pMsgContext->resData;
            resData[0u]                     = Dcm_ProgConditions.SubFncId;
            Std_ReturnType result           = E_OK;
            if (0x10u == Dcm_ProgConditions.Sid)
            {
                uint8 sessionIndex;
                result = DcmInternal_FindSessionIndex(Dcm_ProgConditions.SubFncId, &sessionIndex);
                if (E_OK == result)
                {
                    const Dcm_DspSessionRowType* sessionRowCfg = &Dcm_DspSessionRow[sessionIndex];
                    resData[1u]                                = (uint8)(sessionRowCfg->P2ServerMax >> 8u);
                    resData[2u]                                = (uint8)sessionRowCfg->P2ServerMax;
                    resData[3u]                                = (uint8)((sessionRowCfg->P2StarServerMax / 10uL) >> 8u);
                    resData[4u]                                = (uint8)(sessionRowCfg->P2StarServerMax / 10uL);
                    pMsgContext->resDataLen                    = 5u;
                }
            }
            else if (0x11u == Dcm_ProgConditions.Sid)
            {
                pMsgContext->resDataLen = 1u;
            }
            else
            {
                result = E_NOT_OK;
            }

            if (E_OK == result)
            {
                PduInfoType pduInfo;
                pduInfo.SduLength  = (0x10u == Dcm_ProgConditions.Sid) ? 6u : 2u;
                pduInfo.SduDataPtr = NULL_PTR;
                if (E_OK == PduR_DcmTransmit(*mainConnPtr->ProtocolTx, &pduInfo))
                {
                    protocolCtrl->TxCopyLen = 0u;
                }
                else
                {
                    protocolCtrl->State = DCM_MSG_IDLE;
                }
            }
            else
            {
                protocolCtrl->State = DCM_MSG_IDLE;
            }
        }
        Dcm_ProgConditions.ResponseRequired = FALSE;
    }
}

DCM_LOCAL BufReq_ReturnType DslInternal_PosRespTxDataCopy(uint8 protocolId, const PduInfoType* info)
{
    BufReq_ReturnType         bufResult       = BUFREQ_OK;
    Dcm_ProtocolCtrlType*     protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
    const Dcm_MsgContextType* msgContextPtr   = &Dcm_MsgContext[protocolId];
    const Dcm_MsgItemType*    resData         = msgContextPtr->resData;

    /* check whether the copied length is over the total length */
    if (info->SduLength <= (msgContextPtr->resDataLen + 1uL - protocolCtrlPtr->TxCopyLen))
    {
        /* first copy, deal with response sid */
        if (0uL == protocolCtrlPtr->TxCopyLen)
        {
            info->SduDataPtr[0u] = protocolCtrlPtr->Sid | DCM_POSITIVE_RSPMASK;
            DcmInternal_Memcpy(&info->SduDataPtr[1u], resData, (uint32)info->SduLength - 1uL);
            protocolCtrlPtr->TxCopyLen = info->SduLength;
        }
        else
        {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            /* the current page has finished copying, need to flip the page */
            if ((TRUE == protocolCtrlPtr->PagedBufferStarted)
                && ((protocolCtrlPtr->TxCopyLen + info->SduLength) > protocolCtrlPtr->FirstPageLength))
            {
                PduLengthType pageCopyLength = protocolCtrlPtr->FirstPageLength - protocolCtrlPtr->TxCopyLen;
                DcmInternal_Memcpy(info->SduDataPtr, &resData[protocolCtrlPtr->TxCopyLen - 1u], pageCopyLength);
                DcmInternal_Memcpy(&info->SduDataPtr[pageCopyLength], resData, info->SduLength - pageCopyLength);
                protocolCtrlPtr->TxCopyLen = info->SduLength - pageCopyLength;
            }
            else
#endif
            {
                DcmInternal_Memcpy(info->SduDataPtr, &resData[protocolCtrlPtr->TxCopyLen - 1uL], info->SduLength);
                protocolCtrlPtr->TxCopyLen += info->SduLength;
            }
        }
    }
    else
    {
        bufResult = BUFREQ_E_NOT_OK;
    }

    return bufResult;
}

DCM_LOCAL BufReq_ReturnType DslInternal_NegRespTxDataCopy(uint8 protocolId, const PduInfoType* info)
{
    BufReq_ReturnType     bufResult       = BUFREQ_OK;
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];

    /* for specific cause, the length of negative response is 4, otherwwise is 3 */
#ifdef DCM_NRC22_SPECIFIC_CAUSE_CODE
    if ((DCM_E_CONDITIONSNOTCORRECT == protocolCtrlPtr->ErrorCode)
        && (info->SduLength <= ((DCM_NEGATIVE_RSPMSGLEN + 1u) - protocolCtrlPtr->TxCopyLen)))
    {
        uint8 tempData[DCM_NEGATIVE_RSPMSGLEN + 1u];
        /* retrive cause code from callout */
        if (4u == (info->SduLength + protocolCtrlPtr->TxCopyLen))
        {
            uint16 sourceAddress;
            DcmInternal_GetMetaDataSATA(protocolId, &sourceAddress, NULL_PTR);
            const Dcm_MsgContextType* msgContextPtr = &Dcm_MsgContext[protocolId];
            Std_ReturnType            result        = Dcm_SpecificNrc22CauseCode(
                protocolCtrlPtr->Sid,
                msgContextPtr->reqData,
                msgContextPtr->reqDataLen,
                msgContextPtr->msgAddInfo.reqType,
                (uint16)protocolCtrlPtr->ConnectionId,
                &tempData[3u],
                Dcm_DslProtocolRow[protocolId].ProtocolType,
                sourceAddress);
            if (E_NOT_OK == result)
            {
                tempData[3u] = 0xFFu;
            }
        }
    }
    else
    {
        bufResult = BUFREQ_E_NOT_OK;
    }
#else
    if (info->SduLength <= (DCM_NEGATIVE_RSPMSGLEN - protocolCtrlPtr->TxCopyLen))
    {
        uint8 tempData[DCM_NEGATIVE_RSPMSGLEN];
        tempData[0u] = DCM_NEGATIVE_RSPCODE;
        tempData[1u] = protocolCtrlPtr->Sid;
        tempData[2u] = protocolCtrlPtr->ErrorCode;
        DcmInternal_Memcpy(info->SduDataPtr, &tempData[protocolCtrlPtr->TxCopyLen], info->SduLength);
        protocolCtrlPtr->TxCopyLen += info->SduLength;
    }
    else
    {
        bufResult = BUFREQ_E_NOT_OK;
    }
#endif

    return bufResult;
}

/* handle copyTxData internal process when sduLength > 0 */
DCM_LOCAL BufReq_ReturnType DslInternal_HandleTxDataCopy(uint16 connectionId, const PduInfoType* info)
{
    BufReq_ReturnType     bufResult       = BUFREQ_OK;
    uint8                 protocolId      = Dcm_DslMainConnection[connectionId].ParentProtocolId;
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId]; /* PRQA S 3678 */ /* VL_Dcm_3678 */

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if ((TRUE == protocolCtrlPtr->PagedBufferStarted) && (protocolCtrlPtr->RemainPageLength < info->SduLength))
    {
        bufResult = BUFREQ_E_BUSY;
    }
    else
#endif
    {
        /* positive response */
        if (DCM_POS_RESP == protocolCtrlPtr->ErrorCode)
        {
            bufResult = DslInternal_PosRespTxDataCopy(protocolId, info);
        }
        else
        {
            bufResult = DslInternal_NegRespTxDataCopy(protocolId, info);
        }
    }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    if (TRUE == protocolCtrlPtr->PagedBufferStarted)
    {
        protocolCtrlPtr->RemainPageLength -= info->SduLength;
    }
#endif

    return bufResult;
}

DCM_LOCAL BufReq_ReturnType DslInternal_ProcessCopyRxData(PduIdType id, const PduInfoType* info, uint8 protocolId)
{
    Dcm_ProtocolCtrlType*     protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];
    const Dcm_MsgContextType* msgContextPtr   = &Dcm_MsgContext[protocolId];
    BufReq_ReturnType         bufResult       = BUFREQ_E_NOT_OK;
    Dcm_DslProtocolRxAddrType rxAddr          = Dcm_DslProtocolRx[id].AddrType;

    /* check if the protocol is processing the reception for this id */
    if ((DCM_MSG_RECEIVING == protocolCtrlPtr->State) && (id == protocolCtrlPtr->RxPduId))
    {
        if ((protocolCtrlPtr->RxCopyLen + info->SduLength) <= (msgContextPtr->reqDataLen + 1uL))
        {
            if (0uL == protocolCtrlPtr->RxCopyLen)
            {
                protocolCtrlPtr->Sid = info->SduDataPtr[0u];
                DcmInternal_Memcpy(msgContextPtr->reqData, &info->SduDataPtr[1u], (uint32)info->SduLength - 1uL);
            }
            else
            {
                DcmInternal_Memcpy(
                    &msgContextPtr->reqData[protocolCtrlPtr->RxCopyLen - 1uL],
                    info->SduDataPtr,
                    info->SduLength);
            }
            protocolCtrlPtr->RxCopyLen += info->SduLength;
            bufResult = BUFREQ_OK;
        }
    }
    /* the protocol is processing a different request, check for concurrent tester present */
    else if (
        (protocolCtrlPtr->ConcurrentTesterPresent)
        && ((DCM_PHYSICAL_TYPE == rxAddr) || (info->SduLength > DCM_UDS3E_MINREQLEN)
            || ((1u == info->SduLength)
                && (((protocolCtrlPtr->ConcurrentTesterPresentRxCopyLen == 0u)
                     && (info->SduDataPtr[0u] != DCM_UDS_TESTERPRESENT_SID))
                    || ((protocolCtrlPtr->ConcurrentTesterPresentRxCopyLen == 1u)
                        && (info->SduDataPtr[0u] != DCM_SUPPRESS_BIT))))
            || ((DCM_UDS3E_MINREQLEN == info->SduLength)
                && ((protocolCtrlPtr->ConcurrentTesterPresentRxCopyLen != 0u)
                    || (info->SduDataPtr[0u] != DCM_UDS_TESTERPRESENT_SID)
                    || (info->SduDataPtr[1u] != DCM_SUPPRESS_BIT)))))
    {
        protocolCtrlPtr->ConcurrentTesterPresent = FALSE;
    }
    /* possible concurrent tester present, proceed */
    else
    {
        bufResult = BUFREQ_OK;
    }

    return bufResult;
}

DCM_LOCAL BufReq_ReturnType DslInternal_HandleStartOfReception(
    PduIdType          id,
    uint16             connectionId,
    PduLengthType      TpSduLength,
    const PduInfoType* info)
{
    BufReq_ReturnType         bufResult       = BUFREQ_E_NOT_OK;
    Dcm_DslProtocolRxAddrType rxAddr          = Dcm_DslProtocolRx[id].AddrType;
    uint8                     protocolId      = Dcm_DslMainConnection[connectionId].ParentProtocolId;
    Dcm_ProtocolCtrlType*     protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];

#if (STD_ON == DCM_UDS_0X29)
    /* new diagnostic request, stop timer for deauthentication(timeout) */
    Dcm_UDS0x29_SetAuthenticationCtrlOff(connectionId);
#endif
    SchM_Enter_Dcm_ExclusiveArea();
    /* check whether protocolrows share the same buffer are operating */
    if ((DCM_MSG_IDLE == protocolCtrlPtr->State)
        && (E_OK == DslInternal_CheckProtocolWithSameBuffer(&Dcm_DslProtocolRow[protocolId])))
    {
#if (DCM_DSL_RX_TX_SHARED_BUGGER == STD_ON)
        if ((Dcm_DslProtocolRow[protocolId].SharedRxTxBuffer)
            && ((Dcm_DslProtocolRow[protocolId].TxBufferSize - TpSduLength) < DCM_MIN_TX_BUFFER_SIZE))
        {
            bufResult = BUFREQ_E_OVFL;
        }
        else
#endif
        {
            bufResult              = BUFREQ_OK;
            protocolCtrlPtr->State = DCM_MSG_RECEIVING;
        }
        SchM_Exit_Dcm_ExclusiveArea();
    }
    else
    {
        SchM_Exit_Dcm_ExclusiveArea();
        /* status is not idle, cannot receive, check for queued request or concurrent tester present */
#if (STD_ON == DCM_REQUEST_QUEUED_ENABLED)
        const Dcm_DslProtocolRowType* DcmDslProtocolRowPtr = &Dcm_DslProtocolRow[protocolId];
        uint8                         tempQueueIndex;
        Dcm_QueuedRequestCtrlType*    queuedCtrl = &Dcm_QueuedRequestCtrl[Dcm_QueuedIndex];
        /* only process physical queued request for protocolrows with DcmDslProtocolRequestQueued set to True */
        if ((TRUE == DcmDslProtocolRowPtr->RequestQueued) && (DCM_PHYSICAL_TYPE == rxAddr))
        {
            /* no pending request, store to current queued ctrl unit */
            if (DCM_PENDING_REQUEST_NONE == queuedCtrl->PendingRequestState)
            {
                queuedCtrl->PendingRequestState = DCM_PENDING_REQUEST_RECEIVE;
                bufResult                       = BUFREQ_OK;
            }
            /* there is pending request, check status of the other queued ctrl unit */
            else if (DCM_PENDING_REQUEST_PROCESS == queuedCtrl->PendingRequestState)
            {
                tempQueueIndex = (Dcm_QueuedIndex == 0u) ? 1u : 0u;
                queuedCtrl     = &Dcm_QueuedRequestCtrl[tempQueueIndex];
                /* the other queued ctrl unit is idle, flip queuedIndex */
                if (DCM_PENDING_REQUEST_NONE == queuedCtrl->PendingRequestState)
                {
                    bufResult                       = BUFREQ_OK;
                    Dcm_QueuedIndex                 = tempQueueIndex;
                    queuedCtrl->PendingRequestState = DCM_PENDING_REQUEST_RECEIVE;
                }
            }
            else
            {
                /* idle */
            }
        }
        if (BUFREQ_OK == bufResult)
#endif
        {
            /* check the possbility of a concurrent tester present */
            if (
#if (STD_OFF == DCM_REQUEST_QUEUED_ENABLED)
                (DCM_PHYSICAL_TYPE == rxAddr) ||
#endif
                (DCM_UDS3E_MINREQLEN != TpSduLength) || (info->SduLength > 2u)
                || (((1u == info->SduLength) && (DCM_UDS_TESTERPRESENT_SID != info->SduDataPtr[0u]))
                    || ((DCM_UDS3E_MINREQLEN == info->SduLength)
                        && ((DCM_UDS_TESTERPRESENT_SID != info->SduDataPtr[0u])
                            || (DCM_SUPPRESS_BIT != info->SduDataPtr[1u])))))
            {
                bufResult = BUFREQ_E_NOT_OK;
            }
            else
            {
                /* it may be a concurrent tester present, save the status */
                bufResult                                         = BUFREQ_OK;
                protocolCtrlPtr->ConcurrentTesterPresent          = TRUE;
                protocolCtrlPtr->ConcurrentTesterPresentRxCopyLen = (uint8)info->SduLength;
            }
        }
    }

    return bufResult;
}

DCM_LOCAL void DslInternal_HandleTpTxConfirmation(uint8 protocolId)
{
    Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[protocolId];

    Dcm_ConfirmationStatusType confirmationStatus;
    /* pending nrc 78 is sent, keep processing */
    if (DCM_MSG_PENDING_SENT == protocolCtrlPtr->State)
    {
        /** A negative response with negative response code 0x78 does not restart the S3Server timer. */
        protocolCtrlPtr->State = DCM_MSG_PROCESSING;
    }
    else if (DCM_MSG_SENT == protocolCtrlPtr->State)
    {
        if (DCM_POS_RESP != protocolCtrlPtr->ErrorCode)
        {
            confirmationStatus = DCM_RES_NEG_OK;
        }
        else
        {
            confirmationStatus = DCM_RES_POS_OK;
        }
        DslInternal_SetStateIdle(protocolId, &confirmationStatus, FALSE, TRUE);
    }
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    else if (DCM_MSG_PAGEDBUFFER_SENT == protocolCtrlPtr->State)
    {
        confirmationStatus = DCM_RES_POS_OK;
        DslInternal_SetStateIdle(protocolId, &confirmationStatus, FALSE, TRUE);
    }
#endif
    else
    {
        /* idle */
    }
}

DCM_LOCAL Std_ReturnType DslInternal_DefaultToNonDefault(uint8 ProtocolId)
{
    Std_ReturnType                result         = E_OK;
    const Dcm_DslProtocolRowType* protocolRowCfg = &Dcm_DslProtocolRow[ProtocolId];
    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
        if ((index != ProtocolId)
            && (((DCM_MSG_IDLE != Dcm_ProtocolCtrl[index].State)
                 && (protocolRowCfg->Priority >= Dcm_DslProtocolRow[index].Priority))
                || ((Dcm_ProtocolCtrl[index].ProtocolActive)
                    && (E_OK != DslInternal_StopProtocol(index))))) /* PRQA S 3415 */ /* VL_Dcm_3415 */
        {
            result = DCM_E_PENDING;
            break;
        }
    }

    return result;
}

#if (STD_ON == DCM_UDS_0X86)
DCM_LOCAL boolean Dcm_CopyTxDataCheckRoe(PduIdType id, uint8* ProtocolId, uint16* connectionId)
{
    boolean roe = FALSE;
    (void)DslInternal_FindProtocolRowByTxPduId(id, ProtocolId, connectionId);

    const Dcm_ProtocolCtrlType* protocolCtrlPtr = &Dcm_ProtocolCtrl[*ProtocolId];
    if (!((DCM_MSG_SENT == protocolCtrlPtr->State) || (DCM_MSG_PENDING_SENT == protocolCtrlPtr->State)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
          || ((DCM_MSG_PAGEDBUFFER_SENT == protocolCtrlPtr->State) && (TRUE == protocolCtrlPtr->PagedBufferStarted))
#endif
              ))
    {
        for (uint8 index = 0u; index < Dcm_RoeCtrlNum; index++)
        {
            Dcm_RoeCtrlType* roeCtrlPtr       = &Dcm_RoeCtrl[index];
            uint16           tempConnectionId = Dcm_DslProtocolRx[roeCtrlPtr->RoePduId].ParentMainConnectionId;
            if (((DCM_MSG_SENT == Dcm_ProtocolCtrl[Dcm_DslMainConnection[tempConnectionId].ParentProtocolId].State)
                 || (DCM_MSG_PENDING_SENT
                     == Dcm_ProtocolCtrl[Dcm_DslMainConnection[tempConnectionId].ParentProtocolId].State))
                && (*Dcm_DslMainConnection[tempConnectionId].ProtocolTx == *Dcm_DslMainConnection[id].ProtocolTx))
            {
                *ProtocolId = Dcm_DslMainConnection[tempConnectionId].ParentProtocolId;
                if (NULL_PTR != connectionId)
                {
                    *connectionId = tempConnectionId;
                }
                roe = TRUE;
                break;
            }
        }
    }
    return roe;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
