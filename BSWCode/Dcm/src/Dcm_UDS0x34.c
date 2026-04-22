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
 **  @file               : Dcm_UDS0x34.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x34 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X34)
#ifdef SEV_UDS_REQUEST_DOWNLOAD
#include "IdsM.h"
#endif
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x34
 */
#define DCM_UDS34_MINREQLEN 4u
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         check general conditions for UDS 0x34
 * @param[inout]  pMsgContext      : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                   pMsgContext shall point behind the SID.
 * @param[out]    memoryAddress    : the requested memoryAddress
 * @param[out]    memorySize       : the requested memorySize
 * @param[out]    memoryIdentifier : the configured memoryIdentifier
 * @param[out]    ErrorCode        : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                   module shall send a negative response with NRC code equal to the parameter
 *                                   ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x34_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    uint32*                       memoryAddress,
    uint32*                       memorySize,
    uint8*                        memoryIdentifier,
    Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x34
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x34(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532 -- */
{
    Std_ReturnType result = E_OK;
    uint8          protocolId;

    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);

    uint32 memoryAddress;
    uint32 memorySize;
    uint8  memoryIdentifier;
    result = Dcm_UDS0x34_ConditionCheck(pMsgContext, &memoryAddress, &memorySize, &memoryIdentifier, ErrorCode);

    uint32 blockLength = pMsgContext->resMaxDataLen;
    uint8  dataFormatIdentifier;
    if (E_OK == result)
    {
        dataFormatIdentifier = pMsgContext->reqData[0u];
        result               = Dcm_ProcessRequestDownload(
            OpStatus,
            dataFormatIdentifier,
            memoryIdentifier,
            memoryAddress,
            memorySize,
            &blockLength,
            ErrorCode);
#ifdef SEV_UDS_REQUEST_DOWNLOAD
        IdsM_SetSecurityEvent(SEV_UDS_REQUEST_DOWNLOAD);
#endif
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
#endif
    }

    if (E_OK == result)
    {
        if ((blockLength) > Dcm_DslProtocolRow[protocolId].RxBufferSize)
        {
            DCM_DET_REPORTRUNTIME(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_BUFFER_OVERFLOW);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
    }

    if (E_OK == result)
    {
        Dcm_UDS_SetBlockLength(blockLength, pMsgContext->resData, &pMsgContext->resDataLen);
#if (STD_ON == DCM_UDS_0X36)
        Dcm_UDS_InitTransferData(DCM_REQUEST_DOWNLOAD, memoryIdentifier, memoryAddress, blockLength, memorySize);
#endif
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x34_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    uint32*                       memoryAddress,
    uint32*                       memorySize,
    uint8*                        memoryIdentifier,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 3673 -- */
{
    Std_ReturnType result     = E_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;

    if (reqDataLen < DCM_UDS34_MINREQLEN) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    uint8* reqData = pMsgContext->reqData;
    if (E_OK == result)
    {
        result = Dcm_DataFormatIdentifierCheck(reqData[0u]);
        if (E_NOT_OK == result)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }

    *memoryIdentifier = 0u;
    if (E_OK == result)
    {
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        result = Dcm_UDS_CheckMemory(
            &Dcm_DspMemoryTransfer,
            reqData[1u],
            reqDataLen - 2uL,
            &reqData[2u],
            memoryIdentifier,
            memoryAddress,
            memorySize,
            DCM_WRITE_MEMORY,
            ErrorCode);
        /* PRQA S 3120 -- */
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
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
