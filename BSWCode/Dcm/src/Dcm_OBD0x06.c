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
 **  @file               : Dcm_OBD0x06.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm OBD service 0x06 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_OBD_0X6)
#include "Dem_Dcm.h"
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief the request length of OBD 0x06
 */
#define DCM_OBD0x06_REQLEN 6u
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for OBD 0x06
 */
Std_ReturnType Dcm_OBD0x06(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_OK;
    Dcm_MsgLenType offset  = 0u;
    Dcm_MsgLenType bufSize = pMsgContext->resMaxDataLen - 1u;
    DCM_UNUSED(ErrorCode);
    DCM_UNUSED(OpStatus);

    for (uint8 index = 0u; (index < pMsgContext->reqDataLen) && (E_OK == result); index++)
    {
        uint8 mixMid = 0u;
        result = Dcm_OBD0x06_ReadMid(pMsgContext->reqData[index], &(pMsgContext->resData[offset]), &bufSize, &mixMid);
        if (E_OK == result)
        {
            offset += bufSize;
            bufSize = pMsgContext->resMaxDataLen - offset;
        }

        if ((pMsgContext->reqDataLen > DCM_OBD0x06_REQLEN) || (DCM_OBD_MIXED_AVAILABLE_BIT == mixMid))
        {
            result = E_NOT_OK;
        }
    }

    if (E_OK == result)
    {
        if (0u == offset)
        {
            result = E_NOT_OK;
        }
        else
        {
            pMsgContext->resDataLen = offset;
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
