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
 **  @file               : Dcm_UDS0x10.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x10 function implementation
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x10
 */
#define DCM_UDS10_MINREQLEN 1uL
/**
 * @brief service id of UDS service session control
 */
#define DCM_UDS_SESCTRL_SID 0x10u
/* ============================================ external data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/**
 * @brief session control unit
 */
Dcm_SessionCtrlType Dcm_SessionCtrl;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
/**
 * @brief         handle session change related to boot
 * @param[in]     protocolId     : the current protocol Id
 * @param[in]     OpStatus       : Indicates the current operation status
 * @param[in]     session        : the requested session
 * @param[in]     sessionForBoot : the configuration of session related to boot
 * @param[out]    ErrorCode      : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                 shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                 parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Session Type and current session level
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x10_HandleForBoot(
    uint8                         protocolId,
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         session,
    Dcm_DspSessionForBootType     sessionForBoot,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         process setProgCondition
 * @param[in]     protocolId     : the current protocol Id
 * @param[in]     OpStatus       : Indicates the current operation status
 * @param[in]     session        : the requested session
 * @param[in]     sessionForBoot : the configuration of session related to boot
 * @param[out]    ErrorCode      : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                                 shall send a negative response with NRC code equal to the parameter ErrorCode
 *                                 parameter value.
 * @return        Std_ReturnType
 * @retval        E_OK              : Request was successful
 * @retval        E_NOT_OK          : Request was unsuccessful
 * @retval        DCM_E_PENDING     : Request is not yet finished
 * @retval        DCM_E_FORCE_RCRRP : Application request the transmission of a response Response Pending (NRC 0x78)
 * @reentrant     TRUE
 * @synchronous   Depending on Session Type and current session level
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x10_ProcessSetProg(
    uint8                         protocolId,
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         session,
    Dcm_DspSessionForBootType     sessionForBoot,
    Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x10
 */
/* PRQA S 1532++ */ /* VL_QAC_OneFunRef */
Std_ReturnType Dcm_UDS0x10(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result  = E_OK;
    uint8          session = pMsgContext->reqData[0u];

    if (DCM_UDS10_MINREQLEN != pMsgContext->reqDataLen)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }

    uint8                        protocolId;
    const Dcm_DspSessionRowType* sessionRowCfg;
    if (E_OK == result)
    {
        /* find sessionRow configuration */
        uint8 sessionIndex;
        (void)DcmInternal_FindSessionIndex(session, &sessionIndex);
        (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, &protocolId, NULL_PTR);
        sessionRowCfg                            = &Dcm_DspSessionRow[sessionIndex];
        Dcm_DspSessionForBootType sessionForBoot = sessionRowCfg->SessionForBoot;

        /* handle session change related to boot */
        if (DCM_NO_BOOT != sessionForBoot)
        {
            result = Dcm_UDS0x10_HandleForBoot(protocolId, OpStatus, session, sessionForBoot, ErrorCode);
            if ((E_OK == result) || (DCM_E_FORCE_RCRRP == result))
            {
                Dcm_ProgConditions.ResponseRequired =
                    ((DCM_OEM_BOOT == sessionForBoot) || (DCM_SYS_BOOT == sessionForBoot)) ? TRUE : FALSE;
            }
            else
            {
                Dcm_ProgConditions.ResponseRequired = FALSE;
            }
        }
    }

    if (E_OK == result)
    {
        /* handle differently for default and non-default session transition */
        if ((uint8)DCM_DEFAULT_SESSION == session)
        {
            DslInternal_ResetToDefaultSession(FALSE);
        }
        else
        {
            result = DslInternal_SetSesCtrlType(protocolId, session);
        }
    }

    /* report configured P2ServerMax/P2StarServerMax */
    if (E_OK == result)
    {
        uint8* resData = pMsgContext->resData;
        /* PRQA S 3120++ */ /* VL_QAC_MagicNum */
        resData[0u] = session;
        resData[1u] = (uint8)(sessionRowCfg->P2ServerMax >> 8u);
        resData[2u] = (uint8)sessionRowCfg->P2ServerMax;
        resData[3u] = (uint8)((sessionRowCfg->P2StarServerMax / 10uL) >> 8u);
        resData[4u] = (uint8)(sessionRowCfg->P2StarServerMax / 10uL);
        pMsgContext->resDataLen += 5uL;
        /* PRQA S 3120-- */ /* VL_QAC_MagicNum */
    }

    return result;
}
/* PRQA S 1532-- */ /* VL_QAC_OneFunRef */

void Dcm_UDS0x10_CheckTimer(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint8 Dcm_ProtocolId = Dcm_SessionCtrl.CurrentSessionProtocolId;
    if ((DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
        && (Dcm_ProtocolCtrl[Dcm_ProtocolId].State == DCM_MSG_IDLE))
    {
        boolean timeout = DcmInternal_DecreaseTimer(&Dcm_SessionCtrl.SessionTimer);
        if (timeout)
        {
            DslInternal_ResetToDefaultSession(TRUE);
            DcmInternal_SetActiveDiagnostic(FALSE);
        }
    }
}

Std_ReturnType
    Dcm_UDS0x10_CheckSession(const Dcm_SesCtrlType* session, uint8 sessionNum, Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_NOT_OK;
    if ((0u == sessionNum) || (NULL_PTR == session))
    {
        result = E_OK;
    }
    else
    {
        /* iterate over required session to match with current session */
        for (uint8 index = 0u; index < sessionNum; index++)
        {
            if (Dcm_SessionCtrl.CurrentSession == session[index])
            {
                result = E_OK;
                break;
            }
        }
    }

    if ((E_NOT_OK == result) && (NULL_PTR != ErrorCode))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    }

    return result;
}

/* deal with session change, notify Dsp for session change */
/* PRQA S 6070++ */ /* VL_MTR_Dcm_STCAL */
void Dcm_UDS0x10_SessionChange(void)
{
    SchM_Enter_Dcm_ExclusiveArea();
    if (DCM_INVALID_UINT8 != Dcm_SessionCtrl.NewSession)
    {
        uint8           sessionIndex;
        Dcm_SesCtrlType newSession = Dcm_SessionCtrl.NewSession;
        (void)DcmInternal_FindSessionIndex(Dcm_SessionCtrl.NewSession, &sessionIndex);
        if (DCM_NO_BOOT != Dcm_DspSessionRow[sessionIndex].SessionForBoot)
        {
            Std_ReturnType result = DcmInternal_SetProgConditions(DCM_INITIAL);
            if ((DCM_E_PENDING == result) || (DCM_E_FORCE_RCRRP == result))
            {
                Dcm_PendingSetProg = TRUE;
            }
        }
        Dcm_SessionCtrl.CurrentSession = Dcm_SessionCtrl.NewSession;
        if (DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
        {
            Dcm_SessionCtrl.CurrentSessionProtocolId = Dcm_SessionCtrl.NewSessionProtocolId;
        }
        Dcm_SessionCtrl.NewSession           = DCM_INVALID_UINT8;
        Dcm_SessionCtrl.NewSessionProtocolId = DCM_INVALID_UINT8;
        SchM_Exit_Dcm_ExclusiveArea();
        (void)Rte_Switch_DiagnosticSessionControlModeSwitchInterface_diagnosticSession(newSession);
        (void)SchM_Switch_Dcm_DcmDiagnosticSessionControl(newSession);

        DslInternal_SetSecurityLevel(DCM_SEC_LEV_LOCKED);
        if (DCM_DEFAULT_SESSION != Dcm_SessionCtrl.CurrentSession)
        {
            DcmInternal_SetActiveDiagnostic(TRUE);
            Dcm_SessionCtrl.SessionTimer = DCM_S3_TIMEOUT;
#if (STD_ON == DCM_UDS_0X85)
            const Dcm_DslProtocolRowType* protocolRow = &Dcm_DslProtocolRow[Dcm_SessionCtrl.CurrentSessionProtocolId];
            if (NULL_PTR != protocolRow->UDSControlDTCSessionRef)
            {
                if (E_NOT_OK
                    == Dcm_UDS0x10_CheckSession(
                        protocolRow->UDSControlDTCSessionRef,
                        protocolRow->UDSControlDTCSessionRefNum,
                        NULL_PTR))
                {
                    Dcm_UDS0x85_EnableDTCSetting();
                }
            }
#endif
        }
        else
        {
#if (STD_ON == DCM_UDS_0X85)
            Dcm_UDS0x85_EnableDTCSetting();
#endif
#if (STD_ON == DCM_UDS_0X29)
            for (uint8 index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
            {
                Dcm_UDS0x29_SetDeAuthenticate(index);
            }
#endif
#if (STD_ON == DCM_UDS_0X36)
            Dcm_UDS_InitTransferData(DCM_TRANSFER_IDLE, 0u, 0u, 0u, 0u);
#endif
        }
#if (                        \
    (STD_ON == DCM_UDS_0X28) \
    && ((DCM_COMCONTROL_ALL_NUM > 0) || (DCM_COMCONTROL_SPE_NUM > 0) || (DCM_COMCONTROL_SUB_NUM > 0)))
        Dcm_UDS0x28_SessionChange();
#endif
#if ((STD_ON == DCM_UDS_0X2A) && (STD_ON == DCM_DYN_DID) && (STD_ON == DCM_DDDID_CHECK_SOURCE))
        Dcm_UDS0x2A_StatusChangeHandle();
#endif
#if ((STD_ON == DCM_UDS_0X2F) && (DCM_DID_NUM > 0))
        Dcm_UDS0x2F_StatusChangeHandle(Dcm_SessionCtrl.NewSession);
#endif
    }
    else
    {
        SchM_Exit_Dcm_ExclusiveArea();
    }
}
/* PRQA S 6070-- */ /* VL_MTR_Dcm_STCAL */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
/* ========================================== internal function definitions ========================================= */
/* notify via mode switch interface, calls SetProgConditions for programming session */
DCM_LOCAL Std_ReturnType Dcm_UDS0x10_HandleForBoot(
    uint8                         protocolId,
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         session,
    Dcm_DspSessionForBootType     sessionForBoot,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;
    uint8          resetMode;

    if (DCM_INITIAL == OpStatus)
    {
        if ((DCM_OEM_BOOT == sessionForBoot) || (DCM_OEM_BOOT_RESPAPP == sessionForBoot))
        {
            resetMode = RTE_MODE_DcmEcuReset_DCM_JUMPTOBOOTLOADER;
        }
        else
        {
            resetMode = RTE_MODE_DcmEcuReset_DCM_JUMPTOSYSSUPPLIERBOOTLOADER;
        }
        result = Rte_Switch_EcuResetModeSwitchInterface_ecuReset(resetMode);
        (void)SchM_Switch_Dcm_DcmEcuReset(resetMode);
    }

    if (E_OK == result)
    {
        if (((DCM_INITIAL == OpStatus) && (!Dcm_ProcessingSetProg)) || (DCM_INITIAL != OpStatus))
        {
            result = Dcm_UDS0x10_ProcessSetProg(protocolId, OpStatus, session, sessionForBoot, ErrorCode);
        }
        else
        {
            result = DCM_E_PENDING;
        }
    }
    else
    {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
    }

    return result;
}

DCM_LOCAL Std_ReturnType Dcm_UDS0x10_ProcessSetProg(
    uint8                         protocolId,
    Dcm_ExtendedOpStatusType      OpStatus,
    uint8                         session,
    Dcm_DspSessionForBootType     sessionForBoot,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = E_OK;

    Dcm_ProgConditions.ConnectionId = Dcm_DslMainConnection[Dcm_ProtocolCtrl[protocolId].ConnectionId].RxConnectionId;
    DcmInternal_GetMetaDataSATA(protocolId, &Dcm_ProgConditions.TesterAddress, NULL_PTR);
    Dcm_ProgConditions.Sid      = DCM_UDS_SESCTRL_SID;
    Dcm_ProgConditions.SubFncId = session;
    if ((uint8)DCM_PROGRAMMING_SESSION == session)
    {
        Dcm_ProgConditions.ReprogramingRequest = TRUE;
    }
    else
    {
        Dcm_ProgConditions.ReprogramingRequest = FALSE;
    }

    if (Dcm_DslProtocolRow[protocolId].SendRespPendOnRestart)
    {
        if (DCM_INITIAL == OpStatus)
        {
            result = DCM_E_FORCE_RCRRP;
        }
        else if ((DCM_OEM_BOOT == sessionForBoot) || (DCM_SYS_BOOT == sessionForBoot))
        {
            result = DcmInternal_SetProgConditions(OpStatus);
            if (E_NOT_OK == result)
            {
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            }
        }
        else
        {
            /* idle */
        }
    }
    else
    {
        if ((DCM_OEM_BOOT == sessionForBoot) || (DCM_SYS_BOOT == sessionForBoot))
        {
            result = DcmInternal_SetProgConditions(OpStatus);
            if (E_NOT_OK == result)
            {
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            }
        }
    }

    return result;
}
