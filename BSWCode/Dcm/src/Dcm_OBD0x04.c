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
 **  @file               : Dcm_OBD0x04.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm OBD service 0x04 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_Dcm_0553 */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_OBD_0X4)
#include "Dcm_Internal.h"
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for OBD 0x04
 */
Std_ReturnType Dcm_OBD0x04(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    uint8          protocolId;
    Std_ReturnType result = E_OK;
    DCM_UNUSED(OpStatus);

    /* find protocol for DemclientRef */
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);

    if (DCM_INITIAL == OpStatus)
    {
        /* select target DTC */
        result = Dem_SelectDTC(
            Dcm_DslProtocolRow[protocolId].DemClientRef,
            DEM_DTC_GROUP_ALL_DTCS,
            DEM_DTC_FORMAT_OBD,
            DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY);
    }

    if (E_OK == result)
    {
        /* clear selected DTC */
        result = Dem_ClearDTC(Dcm_DslProtocolRow[protocolId].DemClientRef);
        if (DEM_PENDING == result)
        {
            result = DCM_E_PENDING;
        }
        else if ((DEM_CLEAR_FAILED == result) || (DEM_CLEAR_BUSY == result) || (DEM_CLEAR_MEMORY_ERROR == result))
        {
            /* clear failed, return general nrc 0x22 */
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            result     = E_NOT_OK;
        }
        else if (E_OK != result)
        {
            result = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
