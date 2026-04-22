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
 **  @file               : Dcm_UDS0x36.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x36 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X36)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x36
 */
#define DCM_UDS36_MINREQLEN 1u
/**
 * @brief The header length of UDS 0x36 (serviceId and blockSequenceCounter)
 */
#define DCM_UDS36_HEADER_LENGTH 2u
/* ============================================ external data definitions =========================================== */
/**
 * @brief The data transfer control unit
 */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_TransferDataType Dcm_TransferData;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         check general conditions for UDS 0x36
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The
 *                              pointers in pMsgContext shall point behind the SID.
 * @param[out]    dataLength  : the data length of the following operations
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                              module shall send a negative response with NRC code equal to the parameter
 *                              ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x36_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    uint64*                       dataLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         handle actual data transfer
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[inout]  dataLength  : the data length of the following operations
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x36_HandleTransfer(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint64*                       dataLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         check the transfer sequence such as previous service request and block sequence counter
 * @param[in]     reqDataLen           : the request data length
 * @param[in]     blockSequenceCounter : requested block sequence counter
 * @param[out]    ErrorCode            : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                       module shall send a negative response with NRC code equal to the parameter
 *                                       ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x36_CheckSequence(
    Dcm_MsgLenType                reqDataLen,
    uint8                         blockSequenceCounter,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         process data transfer of read
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[in]     dataLength  : the data length of the following operations
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x36_ProcessRead(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint64                        dataLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         process data transfer of write
 * @param[in]     OpStatus    : Indicates the current operation status
 * @param[in]     dataLength  : the data length of the following operations
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x36_ProcessWrite(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint64                        dataLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x36
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x36(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532 -- */
{
    Std_ReturnType result = E_OK;
    uint64         dataLength;
    const uint8*   reqData = pMsgContext->reqData;

    result = Dcm_UDS0x36_ConditionCheck(pMsgContext, &dataLength, ErrorCode);

    uint8* resData = pMsgContext->resData;
    if (E_OK == result)
    {
        result = Dcm_UDS0x36_HandleTransfer(OpStatus, &dataLength, pMsgContext, ErrorCode);
    }

    uint8 blockSequenceCounter = reqData[0u];
    if (E_OK == result)
    {
        resData[0u] = blockSequenceCounter;
        pMsgContext->resDataLen += 1uL;
        Dcm_TransferData.FirstTransfer = FALSE;
        if (Dcm_TransferData.BlockSequenceCounter != blockSequenceCounter)
        {
            Dcm_TransferData.LastSize = (uint32)dataLength;
            Dcm_TransferData.MemorySizeRemain -= (uint32)dataLength;
            Dcm_TransferData.MemoryAddress += (uint32)dataLength;
            Dcm_TransferData.BlockSequenceCounter = blockSequenceCounter;
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x36_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    uint64*                       dataLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    uint8          blockSequenceCounter;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;

    if (reqDataLen < DCM_UDS36_MINREQLEN) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        blockSequenceCounter = pMsgContext->reqData[0u];
        result               = Dcm_UDS0x36_CheckSequence(reqDataLen, blockSequenceCounter, ErrorCode);
    }

    if (E_OK == result)
    {
        if ((DCM_REQUEST_DOWNLOAD == Dcm_TransferData.TransferStatus)
            || (DCM_REQUEST_WRITEFILE == Dcm_TransferData.TransferStatus))
        {
            *dataLength = (uint64)pMsgContext->reqDataLen - 1ULL;
            if ((*dataLength > Dcm_TransferData.BlockLength) || (0ULL == *dataLength))
            {
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                result     = E_NOT_OK;
            }
        }
        else
        {
            *dataLength = (Dcm_TransferData.BlockLength < Dcm_TransferData.MemorySizeRemain)
                              ? Dcm_TransferData.BlockLength
                              : Dcm_TransferData.MemorySizeRemain;
        }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x36_CheckSequence(
    Dcm_MsgLenType                reqDataLen,
    uint8                         blockSequenceCounter,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    boolean        IsSameBlockId =
        ((blockSequenceCounter == Dcm_TransferData.BlockSequenceCounter) && (Dcm_TransferData.LastSize != 0uL)) ? TRUE
                                                                                                                       : FALSE;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint8 tempBlockSequenceCounter =
        (Dcm_TransferData.BlockSequenceCounter == 0xFFu) ? 0u : (Dcm_TransferData.BlockSequenceCounter + 1u);
    /* PRQA S 3120 -- */

    if ((DCM_TRANSFER_IDLE == Dcm_TransferData.TransferStatus)
        || (((DCM_REQUEST_UPLOAD == Dcm_TransferData.TransferStatus)
             || (DCM_REQUEST_DOWNLOAD == Dcm_TransferData.TransferStatus))
            && (0uL == Dcm_TransferData.MemorySizeRemain) && (FALSE == IsSameBlockId)))
    {
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        result     = E_NOT_OK;
    }
    /* PRQA S 1252 ++ */ /* VL_QAC_1252 */
    else if (
        ((IsSameBlockId == FALSE) && (DCM_REQUEST_DOWNLOAD == Dcm_TransferData.TransferStatus)
         && (Dcm_TransferData.MemorySizeRemain < (reqDataLen - 1uL)))
        || ((IsSameBlockId == TRUE) && ((reqDataLen - 1uL) != Dcm_TransferData.LastSize))
        || (((DCM_REQUEST_DOWNLOAD == Dcm_TransferData.TransferStatus)
             || (DCM_REQUEST_WRITEFILE == Dcm_TransferData.TransferStatus))
            && (reqDataLen > (Dcm_TransferData.BlockLength - DCM_UDS36_HEADER_LENGTH))))
    /* PRQA S 1252 -- */
    {
        /* Downloaded Data Size goes over Required Length, send NRC 0x71 */
        *ErrorCode = DCM_E_TRANSFERDATASUSPENDED;
        result     = E_NOT_OK;
    }
    else if ((IsSameBlockId == FALSE) && (blockSequenceCounter != tempBlockSequenceCounter))
    {
        /* then calculate block index and check block index ,send NRC 0x73 */
        *ErrorCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;
        result     = E_NOT_OK;
    }
    else
    {
        /*idle*/
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x36_HandleTransfer(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint64*                       dataLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_OK;
    uint8*         reqData = pMsgContext->reqData;
    uint8*         resData = pMsgContext->resData;

    switch (Dcm_TransferData.TransferStatus)
    {
    case DCM_REQUEST_UPLOAD:
    {
        result = Dcm_UDS0x36_ProcessRead(OpStatus, *dataLength, pMsgContext, ErrorCode);
        break;
    }
    case DCM_REQUEST_DOWNLOAD:
    {
        result = Dcm_UDS0x36_ProcessWrite(OpStatus, *dataLength, pMsgContext, ErrorCode);
        break;
    }
    case DCM_REQUEST_WRITEFILE:
    {
        result = Dcm_WriteFile(OpStatus, *dataLength, &reqData[1u], ErrorCode);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) && (DCM_E_FORCE_RCRRP != result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
#endif
        break;
    }
    case DCM_REQUEST_READFILEORDIR:
    {
        result = Dcm_ReadFileOrDir(OpStatus, dataLength, &resData[1u], ErrorCode);
        if (E_OK == result)
        {
            if (*dataLength <= Dcm_TransferData.BlockLength)
            {
                pMsgContext->resDataLen += (uint32)*dataLength;
            }
            else
            {
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                result     = E_NOT_OK;
            }
        }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        else if ((E_NOT_OK != result) && (DCM_E_PENDING != result) && (DCM_E_FORCE_RCRRP != result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
#endif
    }
    break;
    default:
    {
        /* idle */
        break;
    }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x36_ProcessRead(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint64                        dataLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result        = E_OK;
    uint32         memoryAddress = (Dcm_TransferData.BlockSequenceCounter == pMsgContext->reqData[0u])
                                       ? (Dcm_TransferData.MemoryAddress - Dcm_TransferData.LastSize)
                                       : Dcm_TransferData.MemoryAddress;
    /* PRQA S 2784 ++ */ /* VL_Dcm_2784 */
    Dcm_ReturnReadMemoryType readMemoryResult = Dcm_ProcessTransferDataRead(
        OpStatus,
        Dcm_TransferData.MemoryIdentifier,
        memoryAddress,
        (uint32)dataLength,
        &pMsgContext->resData[1u],
        ErrorCode);
    /* PRQA S 2784 -- */
    switch (readMemoryResult)
    {
    case DCM_READ_OK:
    {
        pMsgContext->resDataLen += (uint32)dataLength;
        break;
    }
    case DCM_READ_PENDING:
    {
        result = DCM_E_PENDING;
        break;
    }
    case DCM_READ_FAILED:
    {
        result = E_NOT_OK;
        break;
    }
    case DCM_READ_FORCE_RCRRP:
    {
        result = DCM_E_FORCE_RCRRP;
        break;
    }
    default:
    {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        *ErrorCode = DCM_E_GENERALREJECT;
        result     = E_NOT_OK;
#endif
        break;
    }
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x36_ProcessWrite(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint64                        dataLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result        = E_OK;
    uint32         memoryAddress = (Dcm_TransferData.BlockSequenceCounter == pMsgContext->reqData[0u])
                                       ? (Dcm_TransferData.MemoryAddress - Dcm_TransferData.LastSize)
                                       : Dcm_TransferData.MemoryAddress;
    /* PRQA S 2784,3120 ++ */ /* VL_Dcm_2784,VL_QAC_MagicNum */
    Dcm_ReturnWriteMemoryType writeMemoryResult = Dcm_ProcessTransferDataWrite(
        OpStatus,
        Dcm_TransferData.MemoryIdentifier,
        memoryAddress,
        (uint32)dataLength,
        &pMsgContext->reqData[2u],
        ErrorCode);
    /* PRQA S 2784,3120 -- */

    switch (writeMemoryResult)
    {
    case DCM_WRITE_PENDING:
    {
        result = DCM_E_PENDING;
        break;
    }
    case DCM_WRITE_FAILED:
    {
        result = E_NOT_OK;
        break;
    }
    case DCM_WRITE_FORCE_RCRRP:
    {
        result = DCM_E_FORCE_RCRRP;
        break;
    }
    case DCM_WRITE_OK:
    {
        result = E_OK;
        break;
    }
    default:
    {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        *ErrorCode = DCM_E_GENERALREJECT;
        result     = E_NOT_OK;
#endif
        break;
    }
    }

    return result;
}
/* PRQA S 3673 -- */
#endif
