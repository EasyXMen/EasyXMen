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
 **  @file               : Dcm_UDS0x37.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x37 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X37)
#include "Dcm_Internal.h"
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x37
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x37(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 1532 -- */
{
    Std_ReturnType result                              = E_OK;
    uint32         transferResponseParameterRecordSize = pMsgContext->resMaxDataLen - 1uL;

#if (STD_ON == DCM_UDS_0X36)
    if (DCM_TRANSFER_IDLE == Dcm_TransferData.TransferStatus)
    {
        *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
        result     = E_NOT_OK;
    }
#endif

    if (E_OK == result)
    {
        /* process tranfer exit callout */
        result = Dcm_ProcessRequestTransferExit(
            OpStatus,
            pMsgContext->reqData,
            pMsgContext->reqDataLen,
            pMsgContext->resData,
            &transferResponseParameterRecordSize,
            ErrorCode);
    }

#if (STD_ON == DCM_DEV_ERROR_DETECT)
    /* check callout return values */
    if ((E_OK != result) && (E_NOT_OK != result) && (DCM_E_PENDING != result))
    {
        DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
        *ErrorCode = DCM_E_GENERALREJECT;
        result     = E_NOT_OK;
    }
#endif

    if (E_OK == result)
    {
        /* assign resDataLen, check parameter length if development error is enabled */
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((transferResponseParameterRecordSize + 1uL) <= pMsgContext->resMaxDataLen)
        {
#endif
            pMsgContext->resDataLen += transferResponseParameterRecordSize;
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        }
        else
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
#endif
    }

#if (STD_ON == DCM_UDS_0X36)
    if (E_OK == result)
    {
        /* initialize global var to initial status */
        Dcm_UDS_InitTransferData(DCM_TRANSFER_IDLE, 0u, 0u, 0u, 0u);
    }
#endif

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
