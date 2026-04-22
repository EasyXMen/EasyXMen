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
 **  @file               : Dcm_UDS0x24.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x24 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X24)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x24
 */
#define DCM_UDS24_MINREQLEN 2u
/* ========================================= internal function declarations ========================================= */
/**
 * @brief         This function handles getScailingInfo callout
 * @param[in]     didIndex    : the index of did
 * @param[in]     OpStatus    : current operation status
 * @param[inout]  offset      : The current offset of target buffer
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                              module shall send a negative response with NRC code equal to the parameter
 *                              ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was unsuccessful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x24_GetScalingInfo(
    uint16                        didIndex,
    uint16*                       offset,
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x24
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x24(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532 -- */
{
    Std_ReturnType result = E_OK;
    uint16         connectionId;

    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, NULL_PTR, &connectionId);

    if (DCM_UDS24_MINREQLEN != pMsgContext->reqDataLen) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

#if (DCM_DID_NUM > 0)
    uint16 did;
    uint16 didIndex;
    if (E_OK == result)
    {
        did    = DCM_U8N_TO_U16(pMsgContext->reqData);
        result = Dcm_UDS_FindDid(did, NULL_PTR, &didIndex, ErrorCode);
    }

    if (E_OK == result)
    {
        result = Dcm_UDS_CheckReadWriteDid(connectionId, did, DCM_DID_INFO(didIndex).DidRead, ErrorCode);
    }

    uint16 offset = DCM_DID_LENGTH; /* PRQA S 1257 */ /* VL_Dcm_1257 */
    if (E_OK == result)
    {
        result = Dcm_UDS0x24_GetScalingInfo(didIndex, &offset, OpStatus, pMsgContext, ErrorCode);
    }

    if ((E_OK == result) && (DCM_DID_LENGTH == offset))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
    }

    return result;
#else
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    return E_NOT_OK;
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* This function handles getScailingInfo callout */
DCM_LOCAL Std_ReturnType Dcm_UDS0x24_GetScalingInfo(
    uint16                        didIndex,
    uint16*                       offset,
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType finalResult = E_OK;
    Std_ReturnType result      = E_OK;
    uint8*         resData     = pMsgContext->resData;
    DcmInternal_TransformArray_u16(resData, Dcm_DspDid[didIndex].DidIdentifier, DCM_OPAQUE);
    pMsgContext->resDataLen += DCM_DID_LENGTH;
#if (DCM_DID_SIGNAL_NUM > 0)
    for (uint16 index = Dcm_DspDid[didIndex].DidSignalIndexStart;
         (index <= Dcm_DspDid[didIndex].DidSignalIndexEnd) && (E_NOT_OK != result);
         index++)
    {
        const Dcm_DspDataCfgType* dspData = Dcm_DspDidSignal[index].DidDataRef;
        if ((0uL != dspData->ScalingInfoSize) && (NULL_PTR != dspData->GetScalingInfoFnc))
        {
            if ((dspData->ScalingInfoSize + *offset) > pMsgContext->resMaxDataLen)
            {
                result     = E_NOT_OK;
                *ErrorCode = DCM_E_RESPONSETOOLONG;
            }

            if (E_OK == result)
            {
                result = dspData->GetScalingInfoFnc(OpStatus, &resData[*offset], ErrorCode);
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
                (*offset) += (uint16)dspData->ScalingInfoSize;
                pMsgContext->resDataLen += dspData->ScalingInfoSize;
            }

            if (E_OK != result)
            {
                finalResult = result;
            }
        }
    }
#endif

    return finalResult;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
