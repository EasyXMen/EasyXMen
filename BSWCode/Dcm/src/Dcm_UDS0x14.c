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
 **  @file               : Dcm_UDS0x14.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x14 function implementation
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X14)
#ifdef SEV_UDS_CLEAR_DTC_SUCCESSFUL
#include "IdsM.h"
#endif
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x14
 */
#define DCM_UDS14_MINREQLEN 3uL
/* ============================================ internal data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief Tracks the current step of the Diagnostic Trouble Code (DTC) clearing process @range 0..255.
 */
DCM_LOCAL uint8 Dcm_ClearDTCProcessStep;
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/* ============================================ external data definitions =========================================== */
#define DCM_START_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
/**
 * @brief Indicates whether the diagnostic information clearing process is in progress @range 0..1.
 */
boolean Dcm_ProcessingClearingDiagInfo = FALSE; /* PRQA S 1514 */ /* VL_QAC_OneRefSymbol */
#define DCM_STOP_SEC_VAR_INIT_BOOLEAN
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         prepare clear DTC by select and check clear conditions
 * @param[in]     OpStatus  : Indicates the current operation status
 * @param[in]     clientId  : the configured dem client id
 * @param[in]     reqData   : the requested data
 * @param[out]    ErrorCode : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                            shall send a negative response with NRC code equal to the parameter ErrorCode
 *                            parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         -
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x14_PrepareClear(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         clientId,
    const uint8*                  reqData,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         process clearDTC
 * @param[in]     clientId  : the configured dem client id
 * @param[out]    ErrorCode : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                            shall send a negative response with NRC code equal to the parameter ErrorCode
 *                            parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   Depending on Dem
 * @trace         -
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x14_ProcessClear(uint8 clientId, Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x14
 */
/* PRQA S 1532++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x14(
    Dcm_ExtendedOpStatusType OpStatus,
    /* PRQA S 3673++ */ /* VL_QAC_3673 */
    Dcm_MsgContextType* pMsgContext,
    /* PRQA S 3673-- */ /* VL_QAC_3673 */
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result          = E_OK;
    Dcm_ProcessingClearingDiagInfo = TRUE;

    if (DCM_UDS14_MINREQLEN != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    if (E_OK == result)
    {
        uint8 protocolId;
        (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
        uint8 clientId = Dcm_DslProtocolRow[protocolId].DemClientRef;

        result = Dcm_UDS0x14_PrepareClear(OpStatus, clientId, pMsgContext->reqData, ErrorCode);

        if ((E_OK == result) && (DCM_CANCEL != OpStatus))
        {
            result = Dcm_UDS0x14_ProcessClear(clientId, ErrorCode);
        }
    }

    if (DCM_E_PENDING != result)
    {
        Dcm_ProcessingClearingDiagInfo = FALSE;
    }

    return result;
}
/* PRQA S 1532-- */ /* VL_QAC_OneFunRef */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* prepare condition by select DTC and check conditions */
DCM_LOCAL Std_ReturnType Dcm_UDS0x14_PrepareClear(
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         clientId,
    const uint8*                  reqData,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_OK;
    uint32         groupOfDTC = DCM_U8N_TO_U24(reqData);

    if (DCM_INITIAL == OpStatus)
    {
        Dcm_ClearDTCProcessStep = 0u;
        result = Dem_SelectDTC(clientId, groupOfDTC, DEM_DTC_FORMAT_UDS, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        switch (result)
        {
        case E_OK:
        {
            break;
        }
        case DEM_BUSY:
        {
            result = DCM_E_PENDING;
            break;
        }
        default:
        {
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            result     = E_NOT_OK;
            break;
        }
        }
    }

    if ((E_OK == result)
        && ((DCM_INITIAL == OpStatus) || ((DCM_PENDING == OpStatus) && (0u == Dcm_ClearDTCProcessStep))))
    {
        result = Dem_GetDTCSelectionResultForClearDTC(clientId);
        switch (result)
        {
        case E_OK:
        {
#if (STD_ON == DCM_DSP_CLEAR_DTC)
            if (NULL_PTR != Dcm_DspClearDTC.CheckFnc)
            {
                result = Dcm_DspClearDTC.CheckFnc(groupOfDTC, ErrorCode);
            }

            if ((E_OK == result) && (NULL_PTR != Dcm_DspClearDTC.ClearDTCModeRuleRef))
            {
                result = Dcm_DspClearDTC.ClearDTCModeRuleRef(ErrorCode);
            }
#endif
            break;
        }
        case DEM_WRONG_DTC:
        case DEM_WRONG_DTCORIGIN:
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
            break;
        }
        case DEM_PENDING:
        {
            result = DCM_E_PENDING;
            break;
        }
        default:
        {
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            result     = E_NOT_OK;
            break;
        }
        }
    }

    return result;
}

/* actual process of clearDTC */
DCM_LOCAL Std_ReturnType Dcm_UDS0x14_ProcessClear(uint8 clientId, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Dcm_ClearDTCProcessStep = 1u;
    Std_ReturnType result   = Dem_ClearDTC(clientId);
    switch (result)
    {
    case E_OK:
    {
#ifdef SEV_UDS_CLEAR_DTC_SUCCESSFUL
        IdsM_SetSecurityEvent(SEV_UDS_CLEAR_DTC_SUCCESSFUL);
#endif
        break;
    }
    case DEM_WRONG_DTC:
    case DEM_WRONG_DTCORIGIN:
    {
        result     = E_NOT_OK;
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        break;
    }
    case DEM_CLEAR_MEMORY_ERROR:
    {
        result     = E_NOT_OK;
        *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;
        break;
    }
    case DEM_PENDING:
    {
        result = DCM_E_PENDING;
        break;
    }
    default:
    {
        result     = E_NOT_OK;
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        break;
    }
    }

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
