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
 **  @file               : Dcm_UDS0x38.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x38 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X38)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x38
 */
#define DCM_UDS38_MINREQLEN 4u
/**
 * @brief The modeOfOperation addFile
 */
#define DCM_UDS38_ADDFILE 1u
/**
 * @brief The modeOfOperation deleteFile
 */
#define DCM_UDS38_DELETEFILE 2u
/**
 * @brief The modeOfOperation replaceFile
 */
#define DCM_UDS38_REPLACEFILE 3u
/**
 * @brief The modeOfOperation readFile
 */
#define DCM_UDS38_READFILE 4u
/**
 * @brief The modeOfOperation readDirectory
 */
#define DCM_UDS38_READDIR 5u
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         This function coverts data to uint64
 * @param[in]     data : the input uint8_N data
 * @param[in]     size : the size of the data
 * @return        uint64
 * @retval        the result data
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL uint64 Dcm_UDS0x38_ConvertToU64(uint8* data, uint8 size);

/**
 * @brief         check general conditions for UDS 0x38
 * @param[inout]  pMsgContext          : Message-related information for one diagnostic protocol identifier. The
 *                                       pointers in pMsgContext shall point behind the SID.
 * @param[out]    dataFormatIdentifier : requested dataFormatIdentifier
 * @param[out]    fileSizeUncompressed : requested fileSizeUncompressed
 * @param[out]    fileSizeCompressed   : requested fileSizeCompressed
 * @param[out]    ErrorCode            : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                       module shall send a negative response with NRC code equal to the parameter
 *                                       ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x38_CheckCondition(
    Dcm_MsgContextType*           pMsgContext,
    uint8*                        dataFormatIdentifier,
    uint64*                       fileSizeUncompressed,
    uint64*                       fileSizeCompressed,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         check general conditions for UDS 0x38
 * @param[in]     OpStatus               : Indicates the current operation status
 * @param[in]     modeOfOperation        : requested modeOfOperation
 * @param[in]     dataFormatIdentifier   : requested dataFormatIdentifier
 * @param[inout]  fileSizeUncompressed   : requested fileSizeUncompressed
 * @param[inout]  fileSizeCompressed     : requested fileSizeCompressed
 * @param[inout]  pMsgContext            : Message-related information for one diagnostic protocol identifier. The
 *                                         pointers in pMsgContext shall point behind the SID.
 * @param[out]    maxNumberOfBlockLength : output maxNumberOfBlockLength
 * @param[out]    dirInfoLength          : output dirInfoLength
 * @param[out]    ErrorCode              : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                         module shall send a negative response with NRC code equal to the parameter
 *                                         ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application requests the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x38_HandleModeOfOperation(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    uint8                         modeOfOperation,
    uint8                         dataFormatIdentifier,
    uint64*                       fileSizeUncompressed,
    uint64*                       fileSizeCompressed,
    uint64*                       maxNumberOfBlockLength,
    uint64*                       dirInfoLength,
    Dcm_NegativeResponseCodeType* ErrorCode);

#if (STD_ON == DCM_UDS_0X36)
/**
 * @brief         handle modeOfOperation that is not DeleteFile
 * @param[in]     modeOfOperation        : requested modeOfOperation
 * @param[in]     fileSizeUncompressed   : requested fileSizeUncompressed
 * @param[inout]  pMsgContext            : Message-related information for one diagnostic protocol identifier. The
 *                                         pointers in pMsgContext shall point behind the SID.
 * @param[out]    maxNumberOfBlockLength : output maxNumberOfBlockLength
 * @param[out]    ErrorCode              : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                         module shall send a negative response with NRC code equal to the parameter
 *                                         ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x38_NonDeleteFileMode(
    uint8                         modeOfOperation,
    uint64                        fileSizeUncompressed,
    uint64*                       maxNumberOfBlockLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x38
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x38(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532 -- */
{
    Std_ReturnType result = E_OK;
    uint8          modeOfOperation;
    const uint8*   reqData = pMsgContext->reqData;

    uint8  dataFormatIdentifier = 0u;
    uint64 fileSizeUncompressed;
    uint64 fileSizeCompressed;
    result = Dcm_UDS0x38_CheckCondition(
        pMsgContext,
        &dataFormatIdentifier,
        &fileSizeUncompressed,
        &fileSizeCompressed,
        ErrorCode);

    uint64 maxNumberOfBlockLength = 0u;
    uint64 dirInfoLength;
    if (E_OK == result)
    {
        modeOfOperation = reqData[0u];
        result          = Dcm_UDS0x38_HandleModeOfOperation(
            OpStatus,
            pMsgContext,
            modeOfOperation,
            dataFormatIdentifier,
            &fileSizeUncompressed,
            &fileSizeCompressed,
            &maxNumberOfBlockLength,
            &dirInfoLength,
            ErrorCode);
    }

    if (E_OK == result)
    {
        uint16 offset   = 0u;
        uint8* resData  = pMsgContext->resData;
        resData[offset] = modeOfOperation;
        offset++;
        if (DCM_UDS38_DELETEFILE != modeOfOperation)
        {
            resData[offset] = DCM_REQUEST_FILE_TRANSFER_LENGTH_FORMAT_ID;
            offset++;
            DcmInternal_TransformArray_u64(&resData[offset], maxNumberOfBlockLength, DCM_OPAQUE);
            offset += (uint16)DCM_REQUEST_FILE_TRANSFER_LENGTH_FORMAT_ID;

            if (DCM_UDS38_READDIR == modeOfOperation)
            {
                resData[offset] = 0u;
            }
            else
            {
                resData[offset] = dataFormatIdentifier;
            }
            offset++;

            if ((DCM_UDS38_READFILE == modeOfOperation) || (DCM_UDS38_READDIR == modeOfOperation))
            {
                /* fileSizeOrDirInfoParameterLength */
                DcmInternal_TransformArray_u16(
                    &resData[offset],
                    DCM_REQUEST_FILE_TRANSFER_SIZE_OR_DIR_INFO_PARAM_LEN,
                    DCM_OPAQUE);
                offset += 2u; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
                /* fileSizeUncompressedOrDirInfoLength */
                if (DCM_UDS38_READFILE == modeOfOperation)
                {
                    DcmInternal_TransformArray_u64(&resData[offset], fileSizeUncompressed, DCM_OPAQUE);
                }
                else
                {
                    DcmInternal_TransformArray_u64(&resData[offset], dirInfoLength, DCM_OPAQUE);
                }
                offset += DCM_REQUEST_FILE_TRANSFER_SIZE_OR_DIR_INFO_PARAM_LEN;
                /* fileSizeCompressed */
                DcmInternal_TransformArray_u64(&resData[offset], fileSizeCompressed, DCM_OPAQUE);
                offset += DCM_REQUEST_FILE_TRANSFER_SIZE_OR_DIR_INFO_PARAM_LEN;
            }
        }
        pMsgContext->resDataLen = offset;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 3120,3673 ++ */ /* VL_QAC_MagicNum */
/**
 * Convert to uint64 from uint8 array with given size
 */
DCM_LOCAL uint64 Dcm_UDS0x38_ConvertToU64(uint8* data, uint8 size)
{
    uint64 result = 0u;
    for (uint8 index = 0u; index < size; index++)
    {
        result |= ((uint64)data[index] << ((size - index - 1u) * 8u));
    }
    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x38_CheckCondition(
    Dcm_MsgContextType*           pMsgContext,
    uint8*                        dataFormatIdentifier,
    uint64*                       fileSizeUncompressed,
    uint64*                       fileSizeCompressed,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_OK;
    uint8*         reqData    = pMsgContext->reqData;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
    uint8          modeOfOperation;
    uint16         filePathAndNameLength;

    if (reqDataLen < DCM_UDS38_MINREQLEN) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        modeOfOperation       = reqData[0u];
        filePathAndNameLength = DCM_U8N_TO_U16(&reqData[1u]);
    }

    *fileSizeUncompressed = 0u;
    *fileSizeCompressed   = 0u;
    uint16 expectedReqlen;
    if (E_OK == result)
    {
        switch (modeOfOperation)
        {
        case DCM_UDS38_DELETEFILE:
        case DCM_UDS38_READDIR:
        {
            expectedReqlen = filePathAndNameLength + 3u;
            break;
        }
        case DCM_UDS38_READFILE:
        {
            *dataFormatIdentifier = reqData[filePathAndNameLength + 3u];
            expectedReqlen        = filePathAndNameLength + 4u;
            break;
        }
        case DCM_UDS38_ADDFILE:
        case DCM_UDS38_REPLACEFILE:
        {
            *dataFormatIdentifier         = reqData[filePathAndNameLength + 3u];
            uint8 fileSizeParameterLength = reqData[filePathAndNameLength + 4u];
            if ((0u == fileSizeParameterLength) || (fileSizeParameterLength > 8u))
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                result     = E_NOT_OK;
            }
            if (E_OK == result)
            {
                *fileSizeUncompressed =
                    Dcm_UDS0x38_ConvertToU64(&reqData[filePathAndNameLength + 5u], fileSizeParameterLength);
                *fileSizeCompressed = Dcm_UDS0x38_ConvertToU64(
                    &reqData[filePathAndNameLength + fileSizeParameterLength + 5u],
                    fileSizeParameterLength);
                expectedReqlen = filePathAndNameLength + 5u + (2u * (uint16)fileSizeParameterLength);
            }
            break;
        }
        default:
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
            break;
        }
        }
    }

    if ((E_OK == result) && ((reqDataLen != expectedReqlen)))
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

#if (STD_ON == DCM_UDS_0X36)
    if ((E_OK == result) && (DCM_TRANSFER_IDLE != Dcm_TransferData.TransferStatus))
    {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        result     = E_NOT_OK;
    }
#endif

    return result;
}

/* PRQA S 6040 ++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL Std_ReturnType Dcm_UDS0x38_HandleModeOfOperation(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    uint8                         modeOfOperation,
    uint8                         dataFormatIdentifier,
    uint64*                       fileSizeUncompressed,
    uint64*                       fileSizeCompressed,
    uint64*                       maxNumberOfBlockLength,
    uint64*                       dirInfoLength,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result                = E_OK;
    uint8*         reqData               = pMsgContext->reqData;
    uint16         filePathAndNameLength = DCM_U8N_TO_U16(&reqData[1u]);
    uint8*         filePathAndName       = &reqData[3u];

    switch (modeOfOperation)
    {
    case DCM_UDS38_ADDFILE:
    {
        result = Dcm_ProcessRequestAddFile(
            OpStatus,
            filePathAndNameLength,
            filePathAndName,
            dataFormatIdentifier,
            *fileSizeUncompressed,
            *fileSizeCompressed,
            maxNumberOfBlockLength,
            ErrorCode);
        break;
    }
    case DCM_UDS38_DELETEFILE:
    {
        result = Dcm_ProcessRequestDeleteFile(OpStatus, filePathAndNameLength, filePathAndName, ErrorCode);
        break;
    }
    case DCM_UDS38_REPLACEFILE:
    {
        result = Dcm_ProcessRequestReplaceFile(
            OpStatus,
            filePathAndNameLength,
            filePathAndName,
            dataFormatIdentifier,
            *fileSizeUncompressed,
            *fileSizeCompressed,
            maxNumberOfBlockLength,
            ErrorCode);
        break;
    }
    case DCM_UDS38_READFILE:
    {
        result = Dcm_ProcessRequestReadFile(
            OpStatus,
            filePathAndNameLength,
            filePathAndName,
            dataFormatIdentifier,
            fileSizeUncompressed,
            fileSizeCompressed,
            maxNumberOfBlockLength,
            ErrorCode);
        break;
    }
    case DCM_UDS38_READDIR:
    {
        result = Dcm_ProcessRequestReadDir(
            OpStatus,
            filePathAndNameLength,
            filePathAndName,
            dirInfoLength,
            maxNumberOfBlockLength,
            ErrorCode);
        break;
    }
    default:
    {
        /* idle */
        break;
    }
    }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
    if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result) && (DCM_E_FORCE_RCRRP != result))
    {
        DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        *ErrorCode = DCM_E_GENERALREJECT;
        result     = E_NOT_OK;
    }
#endif

#if (STD_ON == DCM_UDS_0X36)
    if ((E_OK == result) && (DCM_UDS38_DELETEFILE != modeOfOperation))
    {
        result = Dcm_UDS0x38_NonDeleteFileMode(
            modeOfOperation,
            *fileSizeUncompressed,
            maxNumberOfBlockLength,
            pMsgContext,
            ErrorCode);
    }
#endif

    return result;
}
/* PRQA S 6040 -- */

#if (STD_ON == DCM_UDS_0X36)
DCM_LOCAL Std_ReturnType Dcm_UDS0x38_NonDeleteFileMode(
    uint8                         modeOfOperation,
    uint64                        fileSizeUncompressed,
    uint64*                       maxNumberOfBlockLength,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    uint8          protocolId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);

    Dcm_TransferData.BlockSequenceCounter = 0u;
    Dcm_TransferData.FirstTransfer        = TRUE;
    if ((DCM_UDS38_ADDFILE == modeOfOperation) || (DCM_UDS38_REPLACEFILE == modeOfOperation))
    {
        if ((*maxNumberOfBlockLength + 2ULL) > Dcm_DslProtocolRow[protocolId].RxBufferSize)
        {
            result = E_NOT_OK;
        }
        Dcm_TransferData.TransferStatus = DCM_REQUEST_WRITEFILE;
    }
    else
    {
        if ((*maxNumberOfBlockLength + 2ULL) > pMsgContext->resMaxDataLen)
        {
            result = E_NOT_OK;
        }
        Dcm_TransferData.TransferStatus = DCM_REQUEST_READFILEORDIR;
    }
    if (0ULL != (*maxNumberOfBlockLength >> (DCM_REQUEST_FILE_TRANSFER_LENGTH_FORMAT_ID * 8u)))
    {
        result = E_NOT_OK;
    }
    if (E_OK == result)
    {
        Dcm_TransferData.BlockLength      = (uint32)*maxNumberOfBlockLength;
        Dcm_TransferData.MemorySizeRemain = (uint32)fileSizeUncompressed;
    }
    else
    {
        DCM_DET_REPORTRUNTIME(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_BUFFER_OVERFLOW);
        *ErrorCode = DCM_E_GENERALREJECT;
    }

    return result;
}
#endif
/* PRQA S 3120,3673 -- */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
