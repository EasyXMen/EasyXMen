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
 **  @file               : Dcm_UDS0x2E.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x2E function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 3120, 1532, 1505 EOF */ /* VL_QAC_MagicNum, VL_QAC_OneFunRef, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X2E)
#if ((defined SEV_UDS_WRITE_INVALID_DATA) || (defined SEV_UDS_WRITE_DATA))
#include "IdsM.h"
#endif
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x2E
 */
#define DCM_UDS2E_MINREQLEN 3uL
/* ============================================ external data definitions =========================================== */
#if (STD_ON == DCM_USE_BNDM)
/**
 * @brief store the BndM result for asynchronous BndM write
 */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_BndMResult[DCM_BNDMDID_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#endif
#ifdef DCM_SECURE_CODING_DID_NUM
#define DCM_START_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
/**
 * @brief store the secure coding did offset of the secure coding buffer
 */
uint32 Dcm_SecureCodingDidOffset[DCM_SECURE_CODING_DID_NUM];
/**
 * @brief store the current secure coding total data length
 */
Dcm_MsgLenType Dcm_SecureCodingDataLength;
#define DCM_STOP_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
/**
 * @brief secure coding buffer
 */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
uint8 Dcm_SecureCodingBuffer[DCM_SECURE_CODING_DATA_BUFFER_SIZE];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
#endif
/* ========================================= internal function declarations ========================================= */
/**
 * @brief         General condition checks for UDS 0x2E
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers
 *                               in pMsgContext shall point behind the SID.
 * @param[out]    did          : The requested did
 * @param[out]    didIndex     : The index of requested did
 * @param[out]    didType      : The type of requested did
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                               module shall send a negative response with NRC code equal to the parameter
 *                               ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2E_ConditionCheck(
    const Dcm_MsgContextType*     pMsgContext,
    uint16*                       did,
    uint16*                       didIndex,
    Dcm_DidType*                  didType,
    Dcm_NegativeResponseCodeType* ErrorCode);
#ifdef DCM_SECURE_CODING_DID_NUM
/**
 * @brief         process secure coding for UDS 0x2E
 * @param[out]    secureCoding : whether the requested did is configured to be secure coding did
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers
 *                               in pMsgContext shall point behind the SID.
 * @param[in]     did          : The requested did
 * @param[in]     didIndex     : The index of requested did
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                               module shall send a negative response with NRC code equal to the parameter
 *                               ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2E_SecureCoding(
    boolean*                      secureCoding,
    Dcm_MsgContextType*           pMsgContext,
    uint16                        did,
    uint16                        didIndex,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

/**
 * @brief         process dspData write for signal
 * @param[in]     reqData      : the requested data
 * @param[inout]  reqDataLen   : the requested data length
 * @param[in]     DcmDspDidCfg : the configuration of requested did
 * @param[in]     OpStatus     : current operation status
 * @param[in]     isDynamic    : whether the requested did is a dynamic one
 * @param[out]    ErrorCode    : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depends on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2E_WriteDspDataSignal(
    const uint8*             reqData,
    Dcm_MsgLenType           reqDataLen,
    const Dcm_DspDidType*    DcmDspDidCfg,
    Dcm_ExtendedOpStatusType OpStatus,
#if (STD_ON == DCM_DYN_DATA)
    boolean isDynamic,
#endif
    Dcm_NegativeResponseCodeType* ErrorCode);

#if (STD_ON == DCM_USE_NVM)
/**
 * @brief         handle signal that use nvm block to write
 * @param[in]     blockId   : the configured nvm block id
 * @param[in]     OpStatus  : current operation status
 * @param[in]     data      : data to be written
 * @param[out]    ErrorCode : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Asynchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2E_NvMSignalHandle(
    NvM_BlockIdType               blockId,
    Dcm_ExtendedOpStatusType      OpStatus,
    const uint8*                  data,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x2E
 */
Std_ReturnType Dcm_UDS0x2E(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
#if (STD_ON == DCM_UDS_0X86)
    uint16 index;
#endif
    const uint8* reqData = pMsgContext->reqData;
    uint8*       resData = pMsgContext->resData;
    uint16       did;
    uint16       didIndex;
    Dcm_DidType  didType;

    result = Dcm_UDS0x2E_ConditionCheck(pMsgContext, &did, &didIndex, &didType, ErrorCode);

#ifdef DCM_SECURE_CODING_DID_NUM
    boolean secureCoding = FALSE;
    if (E_OK == result)
    {
        result = Dcm_UDS0x2E_SecureCoding(&secureCoding, pMsgContext, did, didIndex, ErrorCode);
    }
#endif

#ifdef DCM_SECURE_CODING_DID_NUM
    if (FALSE == secureCoding)
#endif
    {
        if (E_OK == result)
        {
            if (DCM_REGULAR_DID == didType)
            {
                result = Dcm_UDS0x2E_WriteDspData(OpStatus, &Dcm_DspDid[didIndex], reqData, reqDataLen, ErrorCode);
            }
#if (DCM_RANGE_DID_NUM > 0)
            else
            {
                result = Dcm_DspDidRange[didIndex].DidRangeWriteDidFnc(
                    did,
                    &reqData[2],
                    OpStatus,
                    (uint16)(pMsgContext->reqDataLen - 2u),
                    ErrorCode);
            }
#endif

#ifdef SEV_UDS_WRITE_INVALID_DATA
            if (E_NOT_OK == result)
            {
                IdsM_SetSecurityEvent(SEV_UDS_WRITE_INVALID_DATA);
            }
#endif
        }

        if (E_OK == result)
        {
#ifdef SEV_UDS_WRITE_DATA
            IdsM_SetSecurityEvent(SEV_UDS_WRITE_DATA);
#endif
#if (STD_ON == DCM_UDS_0X86)
            for (index = 0u; index < Dcm_RoeCtrlNum; index++)
            {
                Dcm_RoeCtrlType* roeCtrlPtr = &Dcm_RoeCtrl[index];
                if ((DCM_ROE_ONDID == roeCtrlPtr->RoeEvent) && (DCM_ROE_ON == roeCtrlPtr->RoeStatus)
                    && (did == roeCtrlPtr->Did))
                {
                    Dcm_UDS0x86_TriggerServiceRequest(roeCtrlPtr);
                }
            }
#endif
            DcmInternal_TransformArray_u16(resData, did, DCM_OPAQUE);
            pMsgContext->resDataLen += 2uL;
        }
    }

    return result;
}

/**
 * deal with data write, handle based on DidUsePort and DspDataUsePort
 */
Std_ReturnType Dcm_UDS0x2E_WriteDspData(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_DspDidType*         DcmDspDidCfg,
    const uint8*                  reqData,
    Dcm_MsgLenType                reqDataLen,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result      = E_OK;
    Std_ReturnType finalResult = E_OK;

#if (STD_ON == DCM_DYN_DATA)
    boolean isDynamic = FALSE;
    if (DCM_UINT8_DYN == Dcm_DspDidSignal[DcmDspDidCfg->DidSignalIndexEnd].DidDataRef->DspDataType)
    {
        isDynamic = TRUE;
    }
#endif

#if (DCM_DID_SIGNAL_NUM > 0)
#if (STD_ON == DCM_DYN_DATA)
    if (((isDynamic)
         && (((reqDataLen - 2uL) > DcmDspDidCfg->DidSize)
             || ((DcmDspDidCfg->DidSize - reqDataLen + 2uL)
                 > Dcm_DspDidSignal[DcmDspDidCfg->DidSignalIndexEnd].DidDataRef->DspDataByteSize)))
        || ((!isDynamic) && ((reqDataLen - 2uL) != DcmDspDidCfg->DidSize)))
#else
    if ((reqDataLen - 2uL) != DcmDspDidCfg->DidSize)
#endif
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
#endif

    if (E_OK == result)
    {
        if (DCM_USE_DATA_ELEMENT_SPECIFIC_INTERFACES == DcmDspDidCfg->DidUsePort)
        {
#if (DCM_DID_SIGNAL_NUM > 0)
            result = Dcm_UDS0x2E_WriteDspDataSignal(
                reqData,
                reqDataLen,
                DcmDspDidCfg,
                OpStatus,
#if (STD_ON == DCM_DYN_DATA)
                isDynamic,
#endif
                ErrorCode);
#endif
        }
        else
        {
            result = DcmDspDidCfg->DidWriteFnc(OpStatus, &reqData[2u], ErrorCode);

            if (((E_OK != result) && (DCM_E_PENDING != result) && (E_NOT_OK != result)))
            {
                result = E_NOT_OK;
            }
            if ((E_NOT_OK == result) && (DCM_POS_RESP == *ErrorCode))
            {
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            }
        }
    }

    if ((E_NOT_OK == result) || (DCM_E_PENDING == result))
    {
        finalResult = result;
    }

    return finalResult;
}

#ifdef DCM_SECURE_CODING_DID_NUM
/**
 * initialize secure coding vars
 */
void Dcm_UDS0x2E_Init(void)
{
    uint16 index;

    Dcm_SecureCodingDataLength = 0u;
    for (index = 0u; index < DCM_SECURE_CODING_DID_NUM; index++)
    {
        Dcm_SecureCodingDidOffset[index] = DCM_INVALID_UINT16;
    }
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 6010,6080 ++ */ /* VL_MTR_Dcm_STCYC,VL_MTR_Dcm_STPTH*/
DCM_LOCAL Std_ReturnType Dcm_UDS0x2E_ConditionCheck(
    const Dcm_MsgContextType*     pMsgContext,
    uint16*                       did,
    uint16*                       didIndex,
    Dcm_DidType*                  didType,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
    if (reqDataLen < DCM_UDS2E_MINREQLEN)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        *did   = DCM_U8N_TO_U16(&pMsgContext->reqData[0u]);
        result = Dcm_UDS_FindDid(*did, didType, didIndex, ErrorCode);
        if (result == E_OK)
        {
            const Dcm_DspDidReadWriteType* DcmDspDidReadWrite = NULL_PTR;
            if (DCM_REGULAR_DID == *didType)
            {
#if (DCM_DID_NUM > 0)
                DcmDspDidReadWrite = DCM_DID_INFO(*didIndex).DidWrite;
#endif
            }
            else if (DCM_RANGE_DID == *didType)
            {
#if (DCM_RANGE_DID_NUM > 0)
                DcmDspDidReadWrite = DCM_RANGEDID_INFO(*didIndex).DidWrite;
#endif
            }
            else
            {
                result     = E_NOT_OK; /* PRQA S 2982 */ /* VL_Dcm_2982 */
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            }

            if (NULL_PTR != DcmDspDidReadWrite)
            {
                result = Dcm_UDS0x10_CheckSession(
                    DcmDspDidReadWrite->DidReadWriteSessionRef,
                    DcmDspDidReadWrite->DidReadWriteSessionRefNum,
                    ErrorCode);
            }
            else
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                result     = E_NOT_OK;
            }
        }
    }

    const Dcm_DspDidType* DcmDspDidCfg;
    Dcm_MsgLenType        writeSize = reqDataLen - 2uL;
    if (E_OK == result)
    {
        if (DCM_REGULAR_DID == *didType) /* PRQA S 2991,2995 */ /* VL_Dcm_2991,VL_Dcm_2995 */
        {
#if (DCM_DID_NUM > 0)
            DcmDspDidCfg = &Dcm_DspDid[*didIndex];
#if (STD_ON == DCM_DYN_DATA)
            boolean isDynamic = FALSE;
            if (DCM_UINT8_DYN == Dcm_DspDidSignal[DcmDspDidCfg->DidSignalIndexEnd].DidDataRef->DspDataType)
            {
                isDynamic = TRUE;
            }
            if (((isDynamic)
                 && ((writeSize > DcmDspDidCfg->DidSize)
                     || ((DcmDspDidCfg->DidSize - writeSize)
                         > Dcm_DspDidSignal[DcmDspDidCfg->DidSignalIndexEnd].DidDataRef->DspDataByteSize)))
                || ((!isDynamic) && ((writeSize) != DcmDspDidCfg->DidSize)))
#else
            if (DcmDspDidCfg->DidSize != writeSize)
#endif
            {
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                result     = E_NOT_OK;
            }
#else
            DcmDspDidCfg = NULL_PTR;
            result       = E_NOT_OK;
#endif
        }
#if (DCM_RANGE_DID_NUM > 0)
        else if (Dcm_DspDidRange[*didIndex].DidRangeMaxDataLength < writeSize)
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
#endif

        if (E_OK == result)
        {
            uint16 connectionId;
            (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, NULL_PTR, &connectionId);
            result = Dcm_UDS_CheckReadWriteDid(connectionId, *did, DCM_DID_INFO(*didIndex).DidWrite, ErrorCode);
            if ((E_NOT_OK == result) && (*ErrorCode == DCM_POS_RESP))
            {
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            }
        }
    }

    return result;
}
/* PRQA S 6010,6080 -- */
#ifdef DCM_SECURE_CODING_DID_NUM
DCM_LOCAL Std_ReturnType Dcm_UDS0x2E_SecureCoding(
    boolean*                      secureCoding,
    Dcm_MsgContextType*           pMsgContext,
    uint16                        did,
    uint16                        didIndex,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    uint8          index;
    uint8          matchedIndex = 0u;

    for (index = 0u; index < DCM_SECURE_CODING_DID_NUM; index++)
    {
        if (Dcm_SecureCodingDataIdentifier[index] == didIndex)
        {
            matchedIndex  = index;
            *secureCoding = TRUE;
            break;
        }
    }
    if (TRUE == *secureCoding)
    {
#ifdef DCM_UDS_0X29_0X4
        if (TRUE == Dcm_TransmitCertificate)
        {
            Dcm_MsgLenType writeSize = pMsgContext->reqDataLen - 2u;
            if (Dcm_SecureCodingDidOffset[matchedIndex] == DCM_INVALID_UINT16)
            {
                if ((DCM_SECURE_CODING_DATA_BUFFER_SIZE - (Dcm_MsgLenType)Dcm_SecureCodingDataLength) < writeSize)
                {
                    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                    result     = E_NOT_OK;
                }
                else
                {
                    DcmInternal_TransformArray_u16(
                        &Dcm_SecureCodingBuffer[Dcm_SecureCodingDataLength],
                        did,
                        DCM_OPAQUE);
                    DcmInternal_Memcpy(
                        &Dcm_SecureCodingBuffer[Dcm_SecureCodingDataLength + 2u],
                        &pMsgContext->reqData[2],
                        writeSize);
                    Dcm_SecureCodingDidOffset[matchedIndex] = Dcm_SecureCodingDataLength;
                    Dcm_SecureCodingDataLength = (Dcm_MsgLenType)Dcm_SecureCodingDataLength + (writeSize + 2u);
                }
            }
            else
            {
                DcmInternal_Memcpy(
                    &Dcm_SecureCodingBuffer[Dcm_SecureCodingDidOffset[matchedIndex] + 2u],
                    &pMsgContext->reqData[2],
                    writeSize);
            }
        }
        else
        {
            result = E_NOT_OK;
        }
#else
        result = E_NOT_OK;
#endif
        if (E_OK == result)
        {
            DcmInternal_TransformArray_u16(pMsgContext->resData, did, DCM_OPAQUE);
            pMsgContext->resDataLen += 2u;
        }
        else
        {
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        }
    }

    return result;
}
#endif

/* PRQA S 6040++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2E_WriteDspDataSignal(
    const uint8*             reqData,
    Dcm_MsgLenType           reqDataLen,
    const Dcm_DspDidType*    DcmDspDidCfg,
    Dcm_ExtendedOpStatusType OpStatus,
#if (STD_ON == DCM_DYN_DATA)
    boolean isDynamic,
#endif
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 6040-- */ /* VL_MTR_Dcm_STPAR */
{
    Std_ReturnType finalResult = E_OK;

    for (uint16 index = DcmDspDidCfg->DidSignalIndexStart;
         (index <= DcmDspDidCfg->DidSignalIndexEnd) && (E_NOT_OK != finalResult);
         index++)
    {
        uint16                    offset     = Dcm_DspDidSignal[index].DidByteOffset + 2u;
        const Dcm_DspDataCfgType* dspDataCfg = Dcm_DspDidSignal[index].DidDataRef;

        if (DCM_USE_BLOCK_ID == dspDataCfg->DspDataUsePort)
        {
#if (STD_ON == DCM_USE_NVM)
            finalResult = Dcm_UDS0x2E_NvMSignalHandle(dspDataCfg->DspDataBlockIdRef, OpStatus, &reqData[2u], ErrorCode);
#endif
        }
        else if (DCM_USE_ECU_SIGNAL != dspDataCfg->DspDataUsePort)
        {
            Dcm_MsgLenType writeSize;
#if (STD_ON == DCM_DYN_DATA)
            if (isDynamic)
            {
                writeSize = reqDataLen - offset;
            }
            else
#else
            DCM_UNUSED(reqDataLen);
#endif
            {
                writeSize = dspDataCfg->DspDataByteSize;
            }
            Std_ReturnType result = dspDataCfg->DataWriteFnc(&reqData[offset], (uint16)writeSize, OpStatus, ErrorCode);
            if (E_OK != result)
            {
                finalResult = result;
            }
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            if ((DCM_E_PENDING != result) && (E_OK != result) && (E_NOT_OK != result))
            {
                DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
                *ErrorCode  = DCM_E_GENERALREJECT;
                finalResult = E_NOT_OK;
            }
#endif
        }
        else
        {
            *ErrorCode  = DCM_E_CONDITIONSNOTCORRECT;
            finalResult = E_NOT_OK;
        }
    }

    return finalResult;
}

#if (STD_ON == DCM_USE_NVM)
DCM_LOCAL Std_ReturnType Dcm_UDS0x2E_NvMSignalHandle(
    NvM_BlockIdType               blockId,
    Dcm_ExtendedOpStatusType      OpStatus,
    const uint8*                  data,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    switch (OpStatus)
    {
    case DCM_INITIAL:
    {
        (void)NvM_CancelJobs(blockId);
        if (E_OK == NvM_WriteBlock(blockId, data))
        {
            result = DCM_E_PENDING;
        }
        else
        {
            result     = E_NOT_OK;
            *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;
        }
        break;
    }
    case DCM_PENDING:
    {
        NvM_RequestResultType requestResult = NVM_REQ_NOT_OK;
        (void)NvM_GetErrorStatus(blockId, &requestResult);
        if (NVM_REQ_PENDING == requestResult)
        {
            result = DCM_E_PENDING;
        }
        else if (NVM_REQ_OK != requestResult)
        {
            result     = E_NOT_OK;
            *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;
        }
        else
        {
            /* idle */
        }
        break;
    }
    case DCM_CANCEL:
    {
        (void)NvM_CancelJobs(blockId);
        break;
    }
    default:
    {
        /* idle */
        break;
    }
    }
    return result;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
