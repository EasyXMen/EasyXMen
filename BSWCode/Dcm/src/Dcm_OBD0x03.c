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
 **  @file               : Dcm_OBD0x03.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm OBD service 0x03 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_OBD_0X3)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief the confirmed bit of DTC status
 */
#define DCM_DTC_STATUS_CONFIRMED 0x08u
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for OBD 0x03
 */
Std_ReturnType Dcm_OBD0x03(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint8  protocolId;
    uint16 numberOfFilteredDTC;
    DCM_UNUSED(OpStatus);
    DCM_UNUSED(ErrorCode);

    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    Std_ReturnType result = Dcm_OBD_GetFilteredDTC(
        Dcm_DslProtocolRow[protocolId].DemClientRef,
        DCM_DTC_STATUS_CONFIRMED,
        DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY,
        &pMsgContext->resData[1],
        &numberOfFilteredDTC);

    if (E_OK == result)
    {
        pMsgContext->resData[0] = (uint8)(numberOfFilteredDTC & DCM_INVALID_UINT8);
        pMsgContext->resDataLen = ((Dcm_MsgLenType)numberOfFilteredDTC * DCM_OBD_DTC_LENGTH) + 1u;
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
