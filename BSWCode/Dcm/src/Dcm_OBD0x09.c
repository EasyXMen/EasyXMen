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
 **  @file               : Dcm_OBD0x09.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm OBD service 0x09 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_OBD_0X9)
#include "Dcm_Internal.h"
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for OBD 0x09
 */
Std_ReturnType Dcm_OBD0x09(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result      = E_OK;
    Std_ReturnType finalResult = E_OK;
    uint8          offset      = 0u;
    Dcm_MsgLenType BufSize     = pMsgContext->resMaxDataLen - 1u;

    /* iterate over requested vehInfo */
    for (uint8 index = 0u; (index < pMsgContext->reqDataLen) && (E_NOT_OK != result); index++)
    {
        uint8 infoType    = pMsgContext->reqData[index];
        uint8 MixInfoType = 0u;
        result            = Dcm_OBD0x09_ReadVehInfo(
            OpStatus,
            infoType,
            &(pMsgContext->resData[offset]),
            &BufSize,
            &MixInfoType,
            ErrorCode);
        if (E_OK == result)
        {
            offset += (uint8)BufSize;
            BufSize = pMsgContext->resMaxDataLen - offset;
        }
        else if (DCM_E_PENDING == result)
        {
            /* save the pending status, keep going */
            finalResult = DCM_E_PENDING;
        }
        else
        {
            /* idle */
        }
    }

    if (E_NOT_OK == result)
    {
        finalResult = E_NOT_OK;
    }

    if (E_OK == finalResult)
    {
        /* no data is read, return E_NOT_OK */
        if (0u == offset)
        {
            finalResult = E_NOT_OK;
        }
        else
        {
            pMsgContext->resDataLen = offset;
        }
    }

    return finalResult;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
