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
 **  @file               : Dcm_UDS0x2C.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x2C function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X2C)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x2C
 */
#define DCM_UDS2C_MINREQLEN 1u
#ifdef DCM_UDS_0X2C_0X1
/**
 * @brief The subfunction defineByIdentifier
 */
#define DCM_UDS2C_DEFINEBYID 1u
#endif
#ifdef DCM_UDS_0X2C_0X2
/**
 * @brief The subfunction defineByMemoryAddress
 */
#define DCM_UDS2C_DEFINEDBYADD 2u
#endif
#ifdef DCM_UDS_0X2C_0X3
/**
 * @brief The subfunction clearDynamicallyDefinedDataIdentifier
 */
#define DCM_UDS2C_CLEARDEFINE 3u
#endif
/* ============================================ external data definitions =========================================== */
#if (DCM_DDDID_NUM > 0u)
/**
 * @brief The dynamic defined did manager structure @range NA
 */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
Dcm_DDDIDType Dcm_DDDID[DCM_DDDID_NUM]; /* PRQA S 1037,1504 */ /* VL_Dcm_1037,VL_Dcm_1504 */
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
#endif
/* ========================================= internal function declarations ========================================= */
/**
 * @brief         General condition checks for UDS 0x2C
 * @param[in]     connectionId : current connection id
 * @param[inout]  pMsgContext  : Message-related information for one diagnostic protocol identifier. The pointers
 *                               in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode    : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                               module shall send a negative response with NRC code equal to the parameter
 *                               ErrorCode parameter value.
 * @param[out]    didIndex     : The index of requested did
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_ConditionCheck(
    uint16                        connectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode,
    uint16*                       didIndex);

#ifdef DCM_UDS_0X2C_0X1
/**
 * @brief         The service interpreter for UDS 0x2C subfunction 0x01
 * @param[in]     connectionId     : current connection id
 * @param[in]     DDDIDIndex       : DDDID index for reqeusted did
 * @param[in]     DcmDspDidInfoPtr : the DcmDspDidInfo configuration
 * @param[inout]  pMsgContext      : Message-related information for one diagnostic protocol identifier. The pointers
 *                                   in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode        : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                   module shall send a negative response with NRC code equal to the parameter
 *                                   ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_01(
    uint16                        connectionId,
    Dcm_MsgContextType*           pMsgContext,
    uint16                        DDDIDIndex,
    const Dcm_DspDidInfoType*     DcmDspDidInfoPtr,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X2C_0X2
/**
 * @brief         The service interpreter for UDS 0x2C subfunction 0x02
 * @param[in]     DDDIDIndex       : DDDID index for reqeusted did
 * @param[in]     DcmDspDidInfoPtr : the DcmDspDidInfo configuration
 * @param[inout]  pMsgContext      : Message-related information for one diagnostic protocol identifier. The pointers
 *                                   in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode        : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                   module shall send a negative response with NRC code equal to the parameter
 *                                   ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_02(
    Dcm_MsgContextType*           pMsgContext,
    uint16                        DDDIDIndex,
    const Dcm_DspDidInfoType*     DcmDspDidInfoPtr,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         general condition check for UDS 0x2C, subFunction 0x2
 * @param[in]     DcmDspDidInfoPtr  : the DcmDspDidInfo configuration
 * @param[inout]  pMsgContext       : Message-related information for one diagnostic protocol identifier. The pointers
 *                                    in pMsgContext shall point behind the SID.
 * @param[out]    SourceElementsNum : the requestged source element number
 * @param[out]    ErrorCode         : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                    module shall send a negative response with NRC code equal to the parameter
 *                                    ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_02_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    const Dcm_DspDidInfoType*     DcmDspDidInfoPtr,
    Dcm_MsgLenType*               SourceElementsNum,
    Dcm_NegativeResponseCodeType* ErrorCode);
#endif

#ifdef DCM_UDS_0X2C_0X3
/**
 * @brief         The service interpreter for UDS 0x2C subfunction 0x03
 * @param[in]     reqDataLen   : the request data length
 * @param[in]     DDDIDIndex   : DDDID index for reqeusted did
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   Synchronous
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_03(Dcm_MsgLenType reqDataLen, uint16 DDDIDIndex);
#if ((STD_ON == DCM_UDS_0X2A) && (DCM_DDDID_NUM > 0u))
DCM_LOCAL void Dcm_UDS0x2C_GetDidIndex(uint16 DDDIDIndex, uint16* Didindex);
#endif
#endif
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x2C
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x2C(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532 -- */
{
    Dcm_MsgLenType reqDataLen  = pMsgContext->reqDataLen;
    const uint8*   reqData     = pMsgContext->reqData;
    uint8          subFunction = reqData[0u];
    uint16         connectionId;
    DCM_UNUSED(OpStatus);

    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, NULL_PTR, &connectionId);

    uint16         didIndex = 0u;
    Std_ReturnType result   = Dcm_UDS0x2C_ConditionCheck(connectionId, pMsgContext, ErrorCode, &didIndex);

    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    /* General Condition check passes, process by subfunction */
    if (E_OK == result)
    {
        const Dcm_DspDidInfoType* DcmDspDidInfoPtr = &DCM_DID_INFO(didIndex);
        uint16                    DDDIDIndex       = (reqDataLen >= 3uL) ? DCM_DID_INFO(didIndex).DDDidIndex : 0u;
        switch (subFunction)
        {
#ifdef DCM_UDS_0X2C_0X1
        case DCM_UDS2C_DEFINEBYID:
        {
            result = Dcm_UDS0x2C_01(connectionId, pMsgContext, DDDIDIndex, DcmDspDidInfoPtr, ErrorCode);
            break;
        }
#endif
#ifdef DCM_UDS_0X2C_0X2
        case DCM_UDS2C_DEFINEDBYADD:
        {
            result = Dcm_UDS0x2C_02(pMsgContext, DDDIDIndex, DcmDspDidInfoPtr, ErrorCode);
            break;
        }
#endif
#ifdef DCM_UDS_0X2C_0X3
        case DCM_UDS2C_CLEARDEFINE:
        {
            result = Dcm_UDS0x2C_03(reqDataLen, DDDIDIndex);
            break;
        }
#endif
        default:
        {
            /* idle */
            break;
        }
        }
    }

    uint8*          resData    = pMsgContext->resData;
    Dcm_MsgLenType* resDataLen = &pMsgContext->resDataLen;
    /* subfunction processing succeeds, set did to response data */
    if (E_OK == result)
    {
        resData[0u] = subFunction;
        (*resDataLen)++;

        if (reqDataLen >= 3uL)
        {
            DcmInternal_TransformArray_u16(&pMsgContext->resData[1u], Dcm_DspDid[didIndex].DidIdentifier, DCM_OPAQUE);
            (*resDataLen) += 2uL;
        }
#ifdef DCM_DDDID_STORAGE_NVMBLOCK
        (void)NvM_SetRamBlockStatus(DCM_DDDID_STORAGE_NVMBLOCK, TRUE);
#endif
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 6080 ++ */ /* VL_MTR_Dcm_STPTH */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_ConditionCheck(
    uint16                        connectionId,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode,
    uint16*                       didIndex)
{
    Std_ReturnType result     = E_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;

    /* Minimum length check */
    if (reqDataLen < DCM_UDS2C_MINREQLEN) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    const uint8* reqData = pMsgContext->reqData;
    uint16       did;
    if (reqDataLen >= 3uL)
    {
        did    = DCM_U8N_TO_U16(&reqData[1u]);
        result = Dcm_UDS_FindDid(did, NULL_PTR, didIndex, ErrorCode);
    }

    /* length check for different subfunctions */
#ifdef DCM_UDS_0X2C_0X3
    uint8 subFunction = reqData[0u];
#endif
    if ((E_OK == result)
#ifdef DCM_UDS_0X2C_0X3
        && (DCM_UDS2C_CLEARDEFINE != subFunction)
#endif
        && (reqDataLen < 6uL))
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

#ifdef DCM_UDS_0X2C_0X3
    if ((E_OK == result) && (DCM_UDS2C_CLEARDEFINE == subFunction) && (reqDataLen != 1uL) && (reqDataLen != 3uL))
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
#endif

    const Dcm_DspDidInfoType* DcmDspDidInfoPtr;
    if ((E_OK == result) && (reqDataLen >= 3uL))
    {
        DcmDspDidInfoPtr = &DCM_DID_INFO(*didIndex);
        /* The didInfo is not configured to be dynamically defined */
        if (FALSE == DcmDspDidInfoPtr->DynamicallyDefined)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
    }

    if ((E_OK == result) && (reqDataLen >= 3uL))
    {
        result = Dcm_UDS_CheckReadWriteDid(connectionId, did, DcmDspDidInfoPtr->DidRead, ErrorCode);
        if ((E_NOT_OK == result) && (DCM_POS_RESP == *ErrorCode))
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }

    return result;
}

#ifdef DCM_UDS_0X2C_0X1
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_01(
    uint16                        connectionId,
    Dcm_MsgContextType*           pMsgContext,
    uint16                        DDDIDIndex,
    const Dcm_DspDidInfoType*     DcmDspDidInfoPtr,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result                     = E_OK;
    Dcm_MsgLenType sourceLen                  = pMsgContext->reqDataLen - 3uL;
    const uint8*   reqData                    = pMsgContext->reqData;
    Dcm_MsgLenType SourceElementsNum          = 0u;
    uint16         currentSourceElementsCount = 0u;
#if (DCM_DDDID_NUM > 0u)
    currentSourceElementsCount = Dcm_DDDID[DDDIDIndex].SourceElementsNum;
#endif
    /* check for total length and whether input source elements number over configured max */
    if ((sourceLen % 4uL) != 0uL)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        /* PRQA S 2985 ++ */ /* VL_Dcm_2985 */
        SourceElementsNum = (sourceLen / 4uL) + currentSourceElementsCount;
        /* PRQA S 2985 -- */
        if ((0u != DcmDspDidInfoPtr->DDDIDMaxElements) && (SourceElementsNum > DcmDspDidInfoPtr->DDDIDMaxElements))
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
    }

    /* iterate over source elements and set to the Dcm_DDDID */
    for (uint8 index = 0u; (E_NOT_OK != result) && (index < (sourceLen / 4uL)); index++)
    {
        uint16 sourceDidIndex;
        uint16 sourceDid         = DCM_U8N_TO_U16(&reqData[3u + (4u * index)]);
        uint8  sourceDidPosition = reqData[5u + (4u * index)] - 1u;
        uint8  sourceDidSize     = reqData[6u + (4u * index)];

        if ((reqData[5u + (4u * index)] > 0u) && (sourceDidSize > 0u))
        {
            result = Dcm_UDS_FindDid(sourceDid, NULL_PTR, &sourceDidIndex, ErrorCode);
        }
        else
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }

        if ((E_OK == result)
            && (((uint16)sourceDidPosition + (uint16)sourceDidSize) > Dcm_DspDid[sourceDidIndex].DidSize))
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
        if (E_OK == result)
        {
            result =
                Dcm_UDS_CheckReadWriteDid(connectionId, sourceDid, DCM_DID_INFO(sourceDidIndex).DidRead, ErrorCode);
        }
        if ((E_NOT_OK == result) && (DCM_POS_RESP == *ErrorCode))
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
#if (DCM_DDDID_NUM > 0u)
        if (E_OK == result)
        {
            Dcm_DDDID[DDDIDIndex].DDDIDDefine[index + currentSourceElementsCount].SourceDidPosition = sourceDidPosition;
            Dcm_DDDID[DDDIDIndex].DDDIDDefine[index + currentSourceElementsCount].SourceDidSize     = sourceDidSize;
            Dcm_DDDID[DDDIDIndex].DDDIDDefine[index + currentSourceElementsCount].SourceDidIndex    = sourceDidIndex;
            Dcm_DDDID[DDDIDIndex].DDDIDStatus[index + currentSourceElementsCount] = DCM_DEFINED_SOURCE_DID;
        }
#endif
    }
#if (DCM_DDDID_NUM > 0u)
    if (E_OK == result)
    {
        Dcm_DDDID[DDDIDIndex].SourceElementsNum = SourceElementsNum;
    }
#else
    DCM_UNUSED(DDDIDIndex);
#endif

    return result;
}
#endif
/* PRQA S 6080 -- */

#ifdef DCM_UDS_0X2C_0X2
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_02(
    Dcm_MsgContextType*           pMsgContext,
    uint16                        DDDIDIndex,
    const Dcm_DspDidInfoType*     DcmDspDidInfoPtr,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint8*         reqData = pMsgContext->reqData;
    Dcm_MsgLenType SourceElementsNum;
    Std_ReturnType result = Dcm_UDS0x2C_02_ConditionCheck(pMsgContext, DcmDspDidInfoPtr, &SourceElementsNum, ErrorCode);

#if (DCM_DDDID_NUM > 0u)
    uint16 currentSourceElementsCount = 0u;
    currentSourceElementsCount        = Dcm_DDDID[DDDIDIndex].SourceElementsNum;
#endif
    /* iterate over source elements and set to the Dcm_DDDID */
    for (uint8 index = 0u; (E_NOT_OK != result) && (index < SourceElementsNum); index++)
    {
        uint8  memoryBytesNum   = (reqData[3u] & 0x0Fu) + (reqData[3u] >> 4u);
        uint8  memoryIdentifier = 0u;
        uint32 memoryAddress;
        uint32 memorySize;
        result = Dcm_UDS_CheckMemory(
            &Dcm_DspMemory,
            reqData[3u],
            memoryBytesNum,
            &reqData[4u + (memoryBytesNum * index)],
            &memoryIdentifier,
            &memoryAddress,
            &memorySize,
            DCM_READ_MEMORY,
            ErrorCode);
        if (E_OK == result)
        {
#if (DCM_DDDID_NUM > 0u)
            Dcm_DDDID[DDDIDIndex].DDDIDDefine[index + currentSourceElementsCount].MemoryIdentifier = memoryIdentifier;
            Dcm_DDDID[DDDIDIndex].DDDIDDefine[index + currentSourceElementsCount].MemoryAddress    = memoryAddress;
            Dcm_DDDID[DDDIDIndex].DDDIDDefine[index + currentSourceElementsCount].MemorySize       = memorySize;
            Dcm_DDDID[DDDIDIndex].DDDIDStatus[index + currentSourceElementsCount] = DCM_DEFINED_MEMORY_ADD;
#endif
        }
    }

    if (E_OK == result)
    {
#if (DCM_DDDID_NUM > 0u)
        Dcm_DDDID[DDDIDIndex].SourceElementsNum = SourceElementsNum + currentSourceElementsCount;
#endif
    }
#if (DCM_DDDID_NUM == 0u)
    DCM_UNUSED(DDDIDIndex);
#endif

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_02_ConditionCheck(
    Dcm_MsgContextType*           pMsgContext,
    const Dcm_DspDidInfoType*     DcmDspDidInfoPtr,
    Dcm_MsgLenType*               SourceElementsNum,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_OK;
    uint8*         reqData = pMsgContext->reqData; /* PRQA S 3678 */ /* VL_Dcm_3678 */

    if (0u != Dcm_DspMemory.SupportedAddressAndLengthFormatIdentifierNum)
    {
        result = E_NOT_OK;
        for (uint8 index = 0u; index < Dcm_DspMemory.SupportedAddressAndLengthFormatIdentifierNum; index++)
        {
            if (reqData[3u] == Dcm_DspMemory.SupportedAddressAndLengthFormatIdentifier[index])
            {
                result = E_OK;
                break;
            }
        }
    }

    uint8 memoryBytesNum = (reqData[3u] & 0x0Fu) + (reqData[3u] >> 4u);
    *SourceElementsNum   = 0u;
    if (E_OK == result)
    {
        if (0u == memoryBytesNum)
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
        else
        {
            Dcm_MsgLenType sourceLen = pMsgContext->reqDataLen - 3uL;
            if (((sourceLen - 1uL) % memoryBytesNum) != 0uL)
            {
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                result     = E_NOT_OK;
            }
            else
            {
                *SourceElementsNum = sourceLen / memoryBytesNum;
                if ((0u != DcmDspDidInfoPtr->DDDIDMaxElements)
                    && (*SourceElementsNum > DcmDspDidInfoPtr->DDDIDMaxElements))
                {
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                    result     = E_NOT_OK;
                }
            }
        }
    }
    else
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }

    return result;
}
#endif
/* PRQA S 3673 -- */
#ifdef DCM_UDS_0X2C_0X3
#if ((STD_ON == DCM_UDS_0X2A) && (DCM_DDDID_NUM > 0u))
DCM_LOCAL void Dcm_UDS0x2C_GetDidIndex(uint16 DDDIDIndex, uint16* Didindex)
{
    for (uint16 i = 0u; i < DCM_DID_NUM; i++)
    {
        if (DCM_DID_INFO(i).DDDidIndex == DDDIDIndex)
        {
            *Didindex = i;
            break;
        }
    }
}
#endif
DCM_LOCAL Std_ReturnType Dcm_UDS0x2C_03(Dcm_MsgLenType reqDataLen, uint16 DDDIDIndex)
{
    Std_ReturnType result = E_OK;
#if (DCM_DDDID_NUM > 0u)
#if (STD_ON == DCM_UDS_0X2A)
    uint16 Didindex = 0u;
#endif
    if (3uL == reqDataLen)
    {
        DcmInternal_Memset((uint8*)Dcm_DDDID[DDDIDIndex].DDDIDStatus, DCM_NOT_DEFINED, DCM_DSP_DDDID_MAX_ELEMENTS);
        Dcm_DDDID[DDDIDIndex].SourceElementsNum = 0u;
#if (STD_ON == DCM_UDS_0X2A)
        Dcm_UDS0x2C_GetDidIndex(DDDIDIndex, &Didindex);
        Dcm_UDS0x2A_Remove_Scheduler(Didindex);
#endif
    }
    else
    {
        for (uint8 index = 0u; index < DCM_DDDID_NUM; index++) /* PRQA S 2994,2996 */ /* VL_Dcm_2994,VL_Dcm_2996 */
        {
            DcmInternal_Memset((uint8*)Dcm_DDDID[index].DDDIDStatus, DCM_NOT_DEFINED, DCM_DSP_DDDID_MAX_ELEMENTS);
            Dcm_DDDID[index].SourceElementsNum = 0u;
#if (STD_ON == DCM_UDS_0X2A)
            Dcm_UDS0x2C_GetDidIndex(index, &Didindex);
            Dcm_UDS0x2A_Remove_Scheduler(Didindex);
#endif
        }
    }
#else
    DCM_UNUSED(reqDataLen);
    DCM_UNUSED(DDDIDIndex);
#endif

    return result;
}
#endif
/* PRQA S 3120 -- */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
