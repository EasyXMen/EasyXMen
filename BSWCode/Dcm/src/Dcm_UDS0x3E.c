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
 **  @file               : Dcm_UDS0x3E.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x3E function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X3E)
#include "Dcm_Internal.h"
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x3E
 */
Std_ReturnType Dcm_UDS0x3E(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    DCM_UNUSED(OpStatus);
    Std_ReturnType result = E_OK;

    /* Minimum length check */
    if (pMsgContext->reqDataLen != (DCM_UDS3E_MINREQLEN - 1u))
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    if (E_OK == result)
    {
        pMsgContext->resData[0u] = pMsgContext->reqData[0u];
        pMsgContext->resDataLen++;
        if (DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
        {
            Dcm_SessionCtrl.SessionTimer = DCM_S3_TIMEOUT;
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
