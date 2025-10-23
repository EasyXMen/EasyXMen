/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
 **  @file               : Dcm_UDS0x23.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x23 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X23)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x23
 */
#define DCM_UDS23_MINREQLEN 3u
/* ========================================= internal function declarations ========================================= */
/**
 * @brief         This function handles the result of read memory
 * @param[in]     readMemoryResult : the result of read memory
 * @param[out]    ErrorCode        : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                                   module shall send a negative response with NRC code equal to the parameter
 *                                   ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depends on Application
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x23_PostReadMemory(Dcm_ReturnReadMemoryType readMemoryResult, Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x23
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x23(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532 -- */
{
    Std_ReturnType result           = E_OK;
    Dcm_MsgLenType reqDataLen       = pMsgContext->reqDataLen;
    uint8          memoryIdentifier = 0u;
    uint32         memoryAddress;
    uint32         memorySize;

    if (reqDataLen < DCM_UDS23_MINREQLEN) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        uint8* reqData = pMsgContext->reqData;
        result         = Dcm_UDS_CheckMemory(
            &Dcm_DspMemory,
            reqData[0u],
            reqDataLen - 1uL,
            &reqData[1u],
            &memoryIdentifier,
            &memoryAddress,
            &memorySize,
            DCM_READ_MEMORY,
            ErrorCode);
        if ((E_OK == result) && (memorySize > pMsgContext->resMaxDataLen))
        {
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            result     = E_NOT_OK;
        }
    }

    if (E_OK == result)
    {
        Dcm_ReturnReadMemoryType readMemoryResult =
            Dcm_ReadMemory(OpStatus, memoryIdentifier, memoryAddress, memorySize, pMsgContext->resData, ErrorCode);
        result = Dcm_UDS0x23_PostReadMemory(readMemoryResult, ErrorCode);
    }

    if (E_OK == result)
    {
        pMsgContext->resDataLen += memorySize;
    }
    else if ((E_NOT_OK == result) && (DCM_POS_RESP == *ErrorCode))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {
        /* idle */
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* This function handles the result of read memory */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x23_PostReadMemory(Dcm_ReturnReadMemoryType readMemoryResult, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    switch (readMemoryResult)
    {
    case DCM_READ_OK:
    {
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
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif
