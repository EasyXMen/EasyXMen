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
 **  @file               : Dcm_UDS0x3D.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x3D function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X3D)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x3D
 */
#define DCM_UDS3D_MINREQLEN 4u
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         check general conditions for UDS 0x3D
 * @param[inout]  pMsgContext      : Message-related information for one diagnostic protocol identifier. The
 *                                   pointers in pMsgContext shall point behind the SID.
 * @param[out]    memoryIdentifier : the configured memoryIdentifier
 * @param[out]    memoryAddress    : the requested memoryAddress
 * @param[out]    memorySize       : the requested memorySize
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
DCM_LOCAL Std_ReturnType Dcm_UDS0x3D_CheckCondition(
    Dcm_MsgContextType*           pMsgContext,
    uint8*                        memoryIdentifier,
    uint32*                       memoryAddress,
    uint32*                       memorySize,
    Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x3D
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x3D(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532 -- */
{
    uint8          memoryIdentifier;
    uint32         memoryAddress;
    uint32         memorySize;
    Std_ReturnType result =
        Dcm_UDS0x3D_CheckCondition(pMsgContext, &memoryIdentifier, &memoryAddress, &memorySize, ErrorCode);
    const uint8* reqData = pMsgContext->reqData;
    uint8        addByteNumber;
    uint8        LenByteNumber;

    if (E_OK == result)
    {
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        uint8 AddAndLenFid = reqData[0u];
        addByteNumber      = AddAndLenFid >> 4u;
        LenByteNumber      = AddAndLenFid & 0x0Fu;
        /* PRQA S 3120 -- */
        Dcm_ReturnWriteMemoryType writeMemoryResult = Dcm_WriteMemory(
            OpStatus,
            memoryIdentifier,
            memoryAddress,
            memorySize,
            &reqData[1u + addByteNumber + LenByteNumber],
            ErrorCode);
        switch (writeMemoryResult)
        {
        case DCM_WRITE_OK:
        {
            break;
        }
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
    }

    if (E_OK == result)
    {
        DcmInternal_Memcpy(pMsgContext->resData, reqData, 1uL + (uint32)addByteNumber + (uint32)LenByteNumber);
        pMsgContext->resDataLen += 1uL + (Dcm_MsgLenType)addByteNumber + (Dcm_MsgLenType)LenByteNumber;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x3D_CheckCondition(
    Dcm_MsgContextType*           pMsgContext,
    uint8*                        memoryIdentifier,
    uint32*                       memoryAddress,
    uint32*                       memorySize,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 3673 -- */
{
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
    Std_ReturnType result     = E_OK;

    /* Minimum length check */
    if (reqDataLen < DCM_UDS3D_MINREQLEN) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint8* reqData       = pMsgContext->reqData;
    uint8  AddAndLenFid  = reqData[0u];
    uint8  addByteNumber = AddAndLenFid >> 4u;
    uint8  LenByteNumber = AddAndLenFid & 0x0Fu;
    /* PRQA S 3120 -- */
    if (E_OK == result)
    {
        result = Dcm_UDS_CheckMemory(
            &Dcm_DspMemory,
            AddAndLenFid,
            0u,
            &reqData[1u],
            memoryIdentifier,
            memoryAddress,
            memorySize,
            DCM_WRITE_MEMORY,
            ErrorCode);
    }

    if (E_OK == result)
    {
        if (reqDataLen
            != (1uL + (Dcm_MsgLenType)addByteNumber + (Dcm_MsgLenType)LenByteNumber + (Dcm_MsgLenType)*memorySize))
        {
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            result     = E_NOT_OK;
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
