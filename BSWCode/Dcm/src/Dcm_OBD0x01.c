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
 **  @file               : Dcm_OBD0x01.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm OBD service 0x01 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_OBD_0X1)
#include "Dcm_Internal.h"
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for OBD 0x01
 */
Std_ReturnType Dcm_OBD0x01(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_OK;
    uint8          mixPid  = 0u;
    Dcm_MsgLenType bufSize = pMsgContext->resMaxDataLen - 1u;
    uint8          offset  = 0u;
    DCM_UNUSED(ErrorCode);

    for (uint8 index = 0u; (index < pMsgContext->reqDataLen) && (E_OK == result); index++)
    {
        /* iterate over requested pid to read pid data */
        result = Dcm_OBD0x01_ReadPid(
            OpStatus,
            pMsgContext->reqData[index],
            &pMsgContext->resData[offset],
            &bufSize,
            &mixPid);

        /* The result is successful, add to offset */
        if (E_OK == result)
        {
            offset += (uint8)bufSize;
            bufSize = pMsgContext->resMaxDataLen - offset;
        }
    }

    if (E_OK == result)
    {
        /* no data is read */
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
