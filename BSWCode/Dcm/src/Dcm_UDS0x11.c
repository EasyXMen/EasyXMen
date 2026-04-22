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
 **  @file               : Dcm_UDS0x11.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x11 function implementation
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X11)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x11
 */
#define DCM_UDS11_MINREQLEN 1uL
#ifdef DCM_UDS_0X11_0X4
/**
 * @brief The subfunction EnableRepaidPowerShutdown
 */
#define DCM_UDS11_ENABLERAPIDPOWERSHUTDOWN 4u
#endif
#ifdef DCM_UDS_0X11_0X5
/**
 * @brief The subfunction DisableRapidPowerShutdown
 */
#define DCM_UDS11_DISABLERAPIDPOWERSHUTDOWN 5u
#endif
/* ============================================ external data definitions =========================================== */
#if (STD_ON == DCM_UDS_0X11)
/**
 * @brief indicate processing ecuReset
 */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
boolean Dcm_ResetProcessing;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#endif
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         Called by UDS 0x11 to find the configured DcmReset index for the requested subfunction, this function
 *                is guaranteed to be successful as there will be configuration validation rules
 * @param[in]     subFunction : the requested subfunction
 * @param[out]    resetIndex  : the requested subfunction's reset configuration Index
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void DcmInternal_FindResetIndex(uint8 subFunction, uint8* resetIndex);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x11
 */
/* PRQA S 1532,6030++ */ /* VL_QAC_OneFunRef,VL_MTR_Dcm_STMIF */
Std_ReturnType Dcm_UDS0x11(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    if (DCM_UDS11_MINREQLEN != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    uint8* resData     = pMsgContext->resData;
    uint8  subFunction = pMsgContext->reqData[0u];
    if (E_OK == result)
    {
        uint8  protocolId;
        uint16 connectionId;
        (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, &connectionId);
        switch (subFunction) /* PRQA S 3315 */ /* VL_Dcm_3315 */
        {
#ifdef DCM_UDS_0X11_0X4
        case DCM_UDS11_ENABLERAPIDPOWERSHUTDOWN:
        {
            (void)Rte_Switch_ModeRapidPowerShutDownModeSwitchInterface_modeRapidPowerShutDown(
                RTE_MODE_DcmModeRapidPowerShutDown_DCM_ENABLE_RAPIDPOWERSHUTDOWN);
#ifdef DCM_POWER_DOWN_TIME
            resData[1u] = DCM_POWER_DOWN_TIME;
            pMsgContext->resDataLen++;
#else
            resData[1u] = DCM_INVALID_UINT8;
            pMsgContext->resDataLen++;
#endif
            break;
        }
#endif
#ifdef DCM_UDS_0X11_0X5
        case DCM_UDS11_DISABLERAPIDPOWERSHUTDOWN:
        {
            (void)Rte_Switch_ModeRapidPowerShutDownModeSwitchInterface_modeRapidPowerShutDown(
                RTE_MODE_DcmModeRapidPowerShutDown_DCM_DISABLE_RAPIDPOWERSHUTDOWN);
            break;
        }
#endif
        default:
        {
            (void)Rte_Switch_EcuResetModeSwitchInterface_ecuReset(subFunction);
            (void)SchM_Switch_Dcm_DcmEcuReset(subFunction);
            break;
        }
        }

        if ((DCM_INITIAL == OpStatus) && (Dcm_DslProtocolRow[protocolId].SendRespPendOnRestart))
        {
            result = DCM_E_FORCE_RCRRP;
        }
        else
        {
            uint8 resetIndex = 0u;
            DcmInternal_FindResetIndex(subFunction, &resetIndex);
            if (DCM_AFTER_RESET == Dcm_DspEcuResetRow[resetIndex].ResponseToEcuReset)
            {
                if ((DCM_INITIAL != OpStatus) || (!Dcm_ProcessingSetProg))
                {
                    Dcm_ProgConditions.ConnectionId = Dcm_DslMainConnection[connectionId].RxConnectionId;
                    DcmInternal_GetMetaDataSATA(protocolId, &Dcm_ProgConditions.TesterAddress, NULL_PTR);
                    Dcm_ProgConditions.Sid                 = DCM_UDS_ECURESET_SID;
                    Dcm_ProgConditions.SubFncId            = subFunction;
                    Dcm_ProgConditions.ReprogramingRequest = FALSE;
                    Dcm_ProgConditions.ResponseRequired    = TRUE;
                    result                                 = DcmInternal_SetProgConditions(OpStatus);
                    if (E_OK == result)
                    {
                        Dcm_ResRejectedDueToAFTER_RESET = TRUE;
                    }
                }
                else
                {
                    result = DCM_E_PENDING;
                }
            }
        }
    }

    if (E_OK == result)
    {
        resData[0u] = subFunction;
        pMsgContext->resDataLen++;
    }

    return result;
}
/* PRQA S 1532,6030-- */ /* VL_QAC_OneFunRef,VL_MTR_Dcm_STMIF */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * get DcmDspEcuResetRow index for given subFunctionId
 */
DCM_LOCAL void DcmInternal_FindResetIndex(uint8 subFunction, uint8* resetIndex)
{
    uint8 index;

    for (index = 0u; index < DCM_RESET_NUM; index++)
    {
        if (Dcm_DspEcuResetRow[index].EcuResetId == subFunction)
        {
            *resetIndex = index;
            break;
        }
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
