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
 **  @file               : Dcm_UDS0x85.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x85 function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 1532 EOF */ /* VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X85)
#ifdef SEV_UDS_DTC_SETTING_SWITCHED_OFF
#include "IdsM.h"
#endif
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x85
 */
#define DCM_UDS85_MINREQLEN 1uL
#ifdef DCM_UDS_0X85_0X1
/**
 * @brief subFunction dtcSettingOn
 */
#define DCM_UDS85_ON 1u
#endif
#ifdef DCM_UDS_0X85_0X2
/**
 * @brief subFunction dtcSettingOff
 */
#define DCM_UDS85_OFF 2u
#endif
/* ============================================ internal data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
DCM_LOCAL boolean Dcm_DTCSettingDisabled[DCM_PROTOCOLROW_NUM];
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         check general conditions for UDS 0x85
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The
 *                              pointers in pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm
 *                              module shall send a negative response with NRC code equal to the parameter
 *                              ErrorCode parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x85_ConditionCheck(const Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x85
 */
Std_ReturnType Dcm_UDS0x85(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_OK;
    const uint8*   reqData = pMsgContext->reqData;
    uint8          protocolId;
    DCM_UNUSED(OpStatus);
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
    uint8 clientId = Dcm_DslProtocolRow[protocolId].DemClientRef;

    result = Dcm_UDS0x85_ConditionCheck(pMsgContext, ErrorCode);

    uint8 subfunction       = reqData[0u];
    uint8 controlDTCSetting = 0u;
    if (E_OK == result)
    {
        switch (subfunction)
        {
#ifdef DCM_UDS_0X85_0X1
        case DCM_UDS85_ON:
        {
            result = Dem_EnableDTCSetting(clientId);
            if (E_OK == result)
            {
                Dcm_DTCSettingDisabled[protocolId] = FALSE;
                controlDTCSetting                  = RTE_MODE_DcmControlDTCSetting_DCM_ENABLEDTCSETTING;
            }
            break;
        }
#endif
#ifdef DCM_UDS_0X85_0X2
        case DCM_UDS85_OFF:
        {
            result = Dem_DisableDTCSetting(clientId);
            if (E_OK == result)
            {
#ifdef SEV_UDS_DTC_SETTING_SWITCHED_OFF
                IdsM_SetSecurityEvent(SEV_UDS_DTC_SETTING_SWITCHED_OFF);
#endif
                Dcm_DTCSettingDisabled[protocolId] = TRUE;
                controlDTCSetting                  = RTE_MODE_DcmControlDTCSetting_DCM_DISABLEDTCSETTING;
            }
            break;
        }
#endif
        default:
        {
            /* idle */
            break;
        }
        }
        if (DEM_PENDING == result)
        {
            result = DCM_E_PENDING;
        }
    }

    if (E_OK == result)
    {
        pMsgContext->resData[0u] = subfunction;
        pMsgContext->resDataLen++;
        (void)Rte_Switch_ControlDTCSettingModeSwitchInterface_controlDTCSetting(controlDTCSetting);
    }

    return result;
}

/**
 * enable dtc setting for disabled protocol
 */
void Dcm_UDS0x85_EnableDTCSetting(void)
{
    for (uint8 index = 0u; index < DCM_PROTOCOLROW_NUM; index++)
    {
        if (Dcm_DTCSettingDisabled[index])
        {
            (void)Dem_EnableDTCSetting(Dcm_DslProtocolRow[index].DemClientRef);
            Dcm_DTCSettingDisabled[index] = FALSE;
            (void)Rte_Switch_ControlDTCSettingModeSwitchInterface_controlDTCSetting(
                RTE_MODE_DcmControlDTCSetting_DCM_ENABLEDTCSETTING);
        }
    }
}

#if (STD_ON == DCM_UDS_0X85_MODERULE)
/**
 * periodically check mode Rule, enable dtc setting if failed
 */
void Dcm_UDS0x85_MainFunction(void)
{
    if (NULL_PTR != Dcm_DspControlDTCSetting.ControlDTCSettingReEnableModeRuleRef)
    {
        Dcm_NegativeResponseCodeType ErrorCode;
        if (E_NOT_OK == Dcm_DspControlDTCSetting.ControlDTCSettingReEnableModeRuleRef(&ErrorCode))
        {
            Dcm_UDS0x85_EnableDTCSetting();
        }
    }
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
DCM_LOCAL Std_ReturnType
    Dcm_UDS0x85_ConditionCheck(const Dcm_MsgContextType* pMsgContext, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result     = E_OK;
    Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;

    if ((reqDataLen < DCM_UDS85_MINREQLEN) ||
#if (STD_ON == DCM_SUPPORT_DTC_SETTING_CONTROL_OPTION_RECORD)
        (reqDataLen != (DCM_UDS85_MINREQLEN + 3u))
#else
        (reqDataLen != DCM_UDS85_MINREQLEN)
#endif
    )
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

#if (STD_ON == DCM_SUPPORT_DTC_SETTING_CONTROL_OPTION_RECORD)
    if ((E_OK == result) && (0xFFFFFFu != DCM_U8N_TO_U24(&pMsgContext->reqData[1u])))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
    }
#endif

    return result;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
