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
 **  @file               : Dcm_UDS0x2F.c
 **  @author             : xinrun.wang
 **  @date               : 2024/02/23
 **  @vendor             : isoft
 **  @description        : Dcm UDS service 0x2F function implementation
 **
 ***********************************************************************************************************************/
/* PRQA S 3132, 3120, 1532 EOF */ /* VL_QAC_MagicNum, VL_QAC_MagicNum, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "Dcm_Cfg.h"
#if (STD_ON == DCM_UDS_0X2F)
#include "Dcm_Internal.h"
/* ===================================================== macros ===================================================== */
/**
 * @brief The minimum request length of UDS 0x2F
 */
#define DCM_UDS2F_MINREQLEN 3uL
/**
 * @brief The I/O Control Parameter ReturnControlToEcu
 */
#define DCM_UDS2F_RETURNCONTROLTOECU 0u
/**
 * @brief The I/O Control Parameter ResetToDefault
 */
#define DCM_UDS2F_RESETTODEFAULT 1u
/**
 * @brief The I/O Control Parameter FreezeCurrentState
 */
#define DCM_UDS2F_FREEZECURRENTSTATE 2u
/**
 * @brief The I/O Control Parameter ShortTermAdjustment
 */
#define DCM_UDS2F_SHORTTERMADJUSTMENT 3u
/* ============================================ internal data definitions =========================================== */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
/**
 * @brief set to TRUE for non-ReturnControlToEcu request, used to indicate active IO control when session changes @range
 * NA
 */
DCM_LOCAL boolean Dcm_activeIoControl[DCM_DID_NUM];
/**
 * @brief whether it is conctrolled currently @range NA
 */
DCM_LOCAL boolean Dcm_ControlHandle; /* PRQA S 3218 */ /* VL_Dcm_3218 */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/**
 * @brief Holds active I/O control data for CMER in DCM @range NA.
 */
DCM_LOCAL uint8 Dcm_activeIoControlCMER[DCM_DID_NUM][4];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
/* ========================================== internal function declarations ======================================== */
/**
 * @brief         called to check conditions for UDS 0x2F
 * @param[inout]  pMsgContext : Message-related information for one diagnostic protocol identifier. The pointers in
 *                              pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode   : If the operation <Module>_<DiagnosticService> returns value E_NOT_OK, the Dcm module
 *                              shall send a negative response with NRC code equal to the parameter ErrorCode parameter
 *                              value.
 * @param[out]    didIndex    : the target did configuration index if condition check passes
 * @param[out]    isDynamic   : whether the did is dynamic
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_ConditionCheck(
    const Dcm_MsgContextType*     pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode,
    uint16*                       didIndex,
    boolean*                      isDynamic);

/**
 * @brief         find did and did control configuration based on input did
 * @param[in]     did           : the requested did
 * @param[out]    didCfg        : the requested did configuration
 * @param[out]    didControlCfg : the requested did control configuration
 * @param[out]    didIndex      : the configuration index of the did
 * @param[out]    ErrorCode     : the nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_FindDidControl(
    uint16                        did,
    const Dcm_DspDidType**        didCfg,
    const Dcm_DspDidControlType** didControlCfg,
    uint16*                       didIndex,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         check to totalLength of request
 * @param[in]     didCfg        : the requested did configuration
 * @param[in]     didControlCfg : the requested did control configuration
 * @param[inout]  pMsgContext   : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                pMsgContext shall point behind the SID.
 * @param[out]    isDynamic     : is the requested did dynamic
 * @param[out]    ErrorCode     : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK     : Request was successful
 * @retval        E_NOT_OK : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_CheckTotalLength(
    const Dcm_DspDidType*         didCfg,
    const Dcm_DspDidControlType*  didControlCfg,
    const Dcm_MsgContextType*     pMsgContext,
    boolean*                      isDynamic,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         restore io control
 * @param[in]     didIndex      : the requested did configuration index
 * @param[in]     didControlCfg : the requested did control configuration
 * @return        void
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL void Dcm_UDS0x2F_ControlRestore(uint16 didIndex, const Dcm_DspDidControlType* didControlCfg);

/**
 * @brief         process io control including cmer and ioCtrlParam
 * @param[in]     OpStatus      : current operation status
 * @param[in]     didIndex      : the requested did configuration index
 * @param[in]     didControlCfg : the requested did control configuration
 * @param[in]     isDynamic     : is the requested did dynamic
 * @param[inout]  pMsgContext   : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                pMsgContext shall point behind the SID.
 * @param[out]    ErrorCode     : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @retval        DCM_E_PENDING : Request is not yet finished
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_ControlHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_MsgContextType*     pMsgContext,
    uint16                        didIndex,
    const Dcm_DspDidControlType*  didControlCfg,
    boolean                       isDynamic,
    Dcm_NegativeResponseCodeType* ErrorCode);

/**
 * @brief         process CMER
 * @param[in]     signalIndex   : signal index
 * @param[in]     didIndex      : the requested did configuration index
 * @param[in]     didControlCfg : the requested did control configuration
 * @param[in]     isDynamic     : is the requested did dynamic
 * @param[inout]  pMsgContext   : Message-related information for one diagnostic protocol identifier. The pointers in
 *                                pMsgContext shall point behind the SID.
 * @param[inout]  maskOffset    : the current maskOffset
 * @param[out]    stateSize     : the requested state size
 * @return        Std_ReturnType
 * @retval        E_OK          : this signal shall be processed
 * @retval        E_NOT_OK      : this signal shall be skipped
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_HandleCMER(
    uint16                       signalIndex,
    uint16                       didIndex,
    const Dcm_DspDidControlType* didControlCfg,
    boolean                      isDynamic,
    const Dcm_MsgContextType*    pMsgContext,
    Dcm_MsgLenType*              stateSize,
    uint16*                      maskOffset);

/**
 * @brief         process ioCtrlParam
 * @param[in]     reqData        : requested data
 * @param[in]     OpStatus       : current operation status
 * @param[in]     DcmDspDataCfg  : DcmDspData configuration
 * @param[in]     ioCtrlParam    : requested ioCtrl Parameter
 * @param[in]     stateOffset    : requested state offset
 * @param[in]     stateSize      : requested state size
 * @param[in]     controlMaskPtr : requested controlMask pointer
 * @param[out]    ErrorCode      : nrc to send
 * @return        Std_ReturnType
 * @retval        E_OK          : Request was successful
 * @retval        E_NOT_OK      : Request was not successful
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-PLACEHOLDE
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_HandleIoControl(
    const uint8*                  reqData,
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_DspDataCfgType*     DcmDspDataCfg,
    uint8                         ioCtrlParam,
    uint16                        stateOffset,
    Dcm_MsgLenType                stateSize,
    const uint8*                  controlMaskPtr,
    Dcm_NegativeResponseCodeType* ErrorCode);
/* ========================================== external function definitions ========================================= */
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 * The service interpreter for UDS 0x2F
 */
Std_ReturnType Dcm_UDS0x2F(
    Dcm_ExtendedOpStatusType      OpStatus,
    Dcm_MsgContextType*           pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType               result = E_OK;
    uint16                       didIndex;
    const Dcm_DspDidControlType* didControlCfg;
    const uint8*                 reqData     = pMsgContext->reqData;
    uint8                        ioCtrlParam = reqData[2u];
    boolean                      isDynamic;
    uint16                       did = DCM_U8N_TO_U16(&reqData[0u]);

    result = Dcm_UDS0x2F_ConditionCheck(pMsgContext, ErrorCode, &didIndex, &isDynamic);

    if (E_OK == result)
    {
        didControlCfg = DCM_DID_INFO(didIndex).DidControl;
    }

    /* set to TRUE when control part is over to avoid request on control function again when it is finished */
    if (DCM_INITIAL == OpStatus)
    {
        Dcm_ControlHandle = FALSE;
    }

#if (DCM_DID_SIGNAL_NUM > 0)
    if ((E_OK == result) && (!Dcm_ControlHandle))
    {
        result = Dcm_UDS0x2F_ControlHandle(OpStatus, pMsgContext, didIndex, didControlCfg, isDynamic, ErrorCode);
    }
#endif

    uint8*         resData = pMsgContext->resData;
    Dcm_MsgLenType bufSize = pMsgContext->resMaxDataLen - 3uL;
    if (E_OK == result)
    {
        Dcm_ControlHandle = TRUE;
        if (DCM_UDS2F_RETURNCONTROLTOECU != ioCtrlParam)
        {
            Dcm_activeIoControl[didIndex] = TRUE;
        }
        else
        {
            Dcm_activeIoControl[didIndex] = FALSE;
        }

        result = Dcm_UDS_DidReadHandle(OpStatus, didIndex, &resData[3u], &bufSize, ErrorCode);
    }

    Dcm_MsgLenType* resDataLen = &pMsgContext->resDataLen;
    if (E_OK == result)
    {
        DcmInternal_TransformArray_u16(resData, did, DCM_OPAQUE);
        resData[2u] = ioCtrlParam;
        (*resDataLen) += bufSize + 3uL;
    }

    return result;
}

#if (DCM_DID_NUM > 0)
/**
 * deal with session/security change for 0x2F to return control
 */
void Dcm_UDS0x2F_StatusChangeHandle(void)
{
    for (uint16 index = 0u; index < DCM_DID_NUM; index++)
    {
        if (Dcm_activeIoControl[index])
        {
            const Dcm_DspDidControlType* didControlCfg = DCM_DID_INFO(index).DidControl;
            Std_ReturnType               result        = Dcm_UDS0x10_CheckSession(
                didControlCfg->DidControlSessionRef,
                didControlCfg->DidControlSessionRefNum,
                NULL_PTR);
#if (STD_ON == DCM_UDS_0X27)
            if (E_OK == result)
            {
                result = Dcm_UDS0x27_CheckSecurity(
                    didControlCfg->DidControlSecurityLevelRef,
                    didControlCfg->DidControlSecurityLevelRefNum,
                    NULL_PTR);
            }
#endif

            if (E_NOT_OK == result)
            {
                Dcm_UDS0x2F_ControlRestore(index, didControlCfg);
            }
        }
    }
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */
/**
 * condition check including minimun length, session, security and didInfo/didControl, etc.
 */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_ConditionCheck(
    const Dcm_MsgContextType*     pMsgContext,
    Dcm_NegativeResponseCodeType* ErrorCode,
    uint16*                       didIndex,
    boolean*                      isDynamic)
{
    Std_ReturnType               result;
    Dcm_MsgLenType               reqDataLen = pMsgContext->reqDataLen;
    const uint8*                 reqData    = pMsgContext->reqData;
    uint16                       did;
    const Dcm_DspDidControlType* didControlCfg;
    const Dcm_DspDidType*        didCfg;

    /* Minimum length check */
    if (reqDataLen < DCM_UDS2F_MINREQLEN)
    {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        result     = E_NOT_OK;
    }
    else
    {
        did    = DCM_U8N_TO_U16(&reqData[0u]);
        result = Dcm_UDS0x2F_FindDidControl(did, &didCfg, &didControlCfg, didIndex, ErrorCode);
    }

#if (STD_ON == DCM_UDS_0X29)
    /* Check if did is supported for current session */
    uint16 connectionId;
    (void)DslInternal_FindProtocolRowByRxPduId(pMsgContext->dcmRxPduId, NULL_PTR, &connectionId);
    if (E_OK == result)
    {
        result = Dcm_UDS0x29_DidAuthenticationCheck(connectionId, did, NULL_PTR, didControlCfg, ErrorCode);
    }
#endif

    /* Check if did is supported for current session */
    if (E_OK == result)
    {
        result = Dcm_UDS0x10_CheckSession(
            didControlCfg->DidControlSessionRef,
            didControlCfg->DidControlSessionRefNum,
            ErrorCode);
    }

    /* Check if inputOutputControlParameter is supported */
    if (E_OK == result)
    {
        result = Dcm_UDS0x2F_CheckTotalLength(didCfg, didControlCfg, pMsgContext, isDynamic, ErrorCode);
    }

#if (STD_ON == DCM_UDS_0X27)
    /* Security check */
    if (E_OK == result)
    {
        result = Dcm_UDS0x27_CheckSecurity(
            didControlCfg->DidControlSecurityLevelRef,
            didControlCfg->DidControlSecurityLevelRefNum,
            ErrorCode);
    }
#endif

    if ((E_OK == result) && (NULL_PTR != didControlCfg->DidControlModeRuleRef))
    {
        result = didControlCfg->DidControlModeRuleRef(ErrorCode);
    }

    return result;
}

/* find did and didControl configuration based on input did */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_FindDidControl(
    uint16                        did,
    const Dcm_DspDidType**        didCfg,
    const Dcm_DspDidControlType** didControlCfg,
    uint16*                       didIndex,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result = Dcm_UDS_FindDid(did, NULL_PTR, didIndex, ErrorCode);

    /* Check if didControl/didRead is configured */
    if (E_OK == result)
    {
#if (DCM_DID_NUM > 0)
        *didCfg        = &Dcm_DspDid[*didIndex];
        *didControlCfg = DCM_DID_INFO(*didIndex).DidControl;
        if ((NULL_PTR == *didControlCfg) || (NULL_PTR == DCM_DID_INFO(*didIndex).DidRead))
#endif
        {
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            result     = E_NOT_OK;
        }
    }

    return result;
}

/* check total request length based on ioCtrlParam */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_CheckTotalLength(
    const Dcm_DspDidType*         didCfg,
    const Dcm_DspDidControlType*  didControlCfg,
    const Dcm_MsgContextType*     pMsgContext,
    boolean*                      isDynamic,
    Dcm_NegativeResponseCodeType* ErrorCode)
{
    Std_ReturnType result      = E_OK;
    *isDynamic                 = FALSE;
    uint8          ioCtrlParam = pMsgContext->reqData[2u];
    Dcm_MsgLenType reqDataLen  = pMsgContext->reqDataLen;

    if (((DCM_UDS2F_RETURNCONTROLTOECU == ioCtrlParam) && (!didControlCfg->FreezeCurrentState)
         && (!didControlCfg->ResetToDefault) && (!didControlCfg->ShortTermAdjustment))
        || ((DCM_UDS2F_RESETTODEFAULT == ioCtrlParam) && (!didControlCfg->ResetToDefault))
        || ((DCM_UDS2F_FREEZECURRENTSTATE == ioCtrlParam) && (!didControlCfg->FreezeCurrentState))
        || ((DCM_UDS2F_SHORTTERMADJUSTMENT == ioCtrlParam) && (!didControlCfg->ShortTermAdjustment))
        || (ioCtrlParam > DCM_UDS2F_SHORTTERMADJUSTMENT))
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        result     = E_NOT_OK;
    }
    else
    {
#if (STD_ON == DCM_DYN_DATA)
        if (DCM_UINT8_DYN == Dcm_DspDidSignal[didCfg->DidSignalIndexEnd].DidDataRef->DspDataType)
        {
            *isDynamic = TRUE;
        }
#endif
        if (DCM_UDS2F_SHORTTERMADJUSTMENT != ioCtrlParam)
        {
            if (reqDataLen != (DCM_UDS2F_MINREQLEN + didControlCfg->ControlMaskSize))
            {
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                result     = E_NOT_OK;
            }
        }
        else
        {
            Dcm_MsgLenType minReqLen;
            if (DCM_CONTROLMASK_NO == didControlCfg->ControlMask)
            {
                minReqLen = (Dcm_MsgLenType)DCM_UDS2F_MINREQLEN + (Dcm_MsgLenType)didCfg->DidSize;
            }
            else
            {
                minReqLen = DCM_UDS2F_MINREQLEN + (Dcm_MsgLenType)didCfg->DidSize
                            + (Dcm_MsgLenType)didControlCfg->ControlMaskSize;
            }
#if (DCM_DID_SIGNAL_NUM > 0)
#if (STD_ON == DCM_DYN_DATA)
            if (((*isDynamic)
                 && ((reqDataLen <= minReqLen)
                     || ((reqDataLen - minReqLen)
                         > Dcm_DspDidSignal[didCfg->DidSignalIndexEnd].DidDataRef->DspDataByteSize)))
                || (!(*isDynamic) && (reqDataLen != minReqLen)))
#else
            if (reqDataLen != minReqLen)
#endif
            {
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                result     = E_NOT_OK;
            }
#endif
        }
    }

    return result;
}

/* restore io control */
DCM_LOCAL void Dcm_UDS0x2F_ControlRestore(uint16 didIndex, const Dcm_DspDidControlType* didControlCfg)
{
    for (uint16 subIndex = Dcm_DspDid[didIndex].DidSignalIndexStart; subIndex <= Dcm_DspDid[didIndex].DidSignalIndexEnd;
         subIndex++)
    {
        const uint8*              controlMaskPtr = NULL_PTR;
        const Dcm_DspDataCfgType* DcmDspDataCfg  = Dcm_DspDidSignal[subIndex].DidDataRef;
        uint8                     controlMask    = 0u;
        if (DCM_CONTROLMASK_INTERNAL == didControlCfg->ControlMask)
        {
            controlMask = Dcm_activeIoControlCMER[didIndex][subIndex / 8u] & (uint8)(1u << (didIndex % 8u));
        }
        else if (DCM_CONTROLMASK_EXTERNAL == didControlCfg->ControlMask)
        {
            controlMaskPtr = Dcm_activeIoControlCMER[didIndex];
        }
        else
        {
                }

        if ((DCM_CONTROLMASK_INTERNAL != didControlCfg->ControlMask) || (0u != controlMask))
        {
            if (DcmDspDataCfg->DspDataUsePort == DCM_USE_ECU_SIGNAL)
            {
                (void)DcmDspDataCfg->EcuSignal(DCM_UDS2F_RETURNCONTROLTOECU, NULL_PTR);
            }
            else
            {
                Dcm_NegativeResponseCodeType ErrorCode;
                (void)DcmDspDataCfg->ReturnControlToEcuFnc(controlMaskPtr, &ErrorCode);
            }
        }
    }
    Dcm_activeIoControl[didIndex] = FALSE;
}

/* handle CMER and ioCtrlParam */
/* PRQA S 6040++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_ControlHandle(
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_MsgContextType*     pMsgContext,
    uint16                        didIndex,
    const Dcm_DspDidControlType*  didControlCfg,
    boolean                       isDynamic,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 6040-- */ /* VL_MTR_Dcm_STPAR */
{
    Std_ReturnType        finalResult = E_OK;
    uint16                maskOffset  = 3u;
    const Dcm_DspDidType* didCfg      = &Dcm_DspDid[didIndex];
    uint8*                reqData     = pMsgContext->reqData;
    uint8                 ioCtrlParam = reqData[2u];

    for (uint16 index = didCfg->DidSignalIndexStart; (index <= didCfg->DidSignalIndexEnd) && (E_NOT_OK != finalResult);
         index++)
    {
        const Dcm_DspDataCfgType* DcmDspDataCfg  = Dcm_DspDidSignal[index].DidDataRef;
        const uint8*              controlMaskPtr = NULL_PTR;
        Dcm_MsgLenType            stateSize      = 0u;
        Std_ReturnType            result =
            Dcm_UDS0x2F_HandleCMER(index, didIndex, didControlCfg, isDynamic, pMsgContext, &stateSize, &maskOffset);
        if (E_NOT_OK != result)
        {
            if (DCM_CONTROLMASK_EXTERNAL == didControlCfg->ControlMask)
            {
                controlMaskPtr = &reqData[maskOffset];
            }
            else
            {
                /* idle */
            }

            uint16 stateOffset = Dcm_DspDidSignal[index].DidByteOffset + 3u;
            if (DCM_USE_ECU_SIGNAL == DcmDspDataCfg->DspDataUsePort)
            {
                if (DCM_UDS2F_SHORTTERMADJUSTMENT == ioCtrlParam)
                {
                    result = DcmDspDataCfg->EcuSignal(ioCtrlParam, &reqData[stateOffset]);
                }
                else
                {
                    result = DcmDspDataCfg->EcuSignal(ioCtrlParam, NULL_PTR);
                }
            }
            else
            {
                result = Dcm_UDS0x2F_HandleIoControl(
                    reqData,
                    OpStatus,
                    DcmDspDataCfg,
                    ioCtrlParam,
                    stateOffset,
                    stateSize,
                    controlMaskPtr,
                    ErrorCode);
            }

            switch (result)
            {
            case DCM_E_PENDING:
            {
                finalResult = DCM_E_PENDING;
                break;
            }
            case E_NOT_OK:
            {
                finalResult = E_NOT_OK;
                break;
            }
            case E_OK:
            {
                break;
            }
            default:
            {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
                DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
                *ErrorCode  = DCM_E_GENERALREJECT;
                finalResult = E_NOT_OK;
#endif
                break;
            }
            }
        }
    }

    return finalResult;
}

/* handle CMER, decide whether to skip this signal */
/* PRQA S 6040++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_HandleCMER(
    uint16                       signalIndex,
    uint16                       didIndex,
    const Dcm_DspDidControlType* didControlCfg,
    boolean                      isDynamic,
    const Dcm_MsgContextType*    pMsgContext,
    Dcm_MsgLenType*              stateSize,
    uint16*                      maskOffset)
/* PRQA S 6040-- */ /* VL_MTR_Dcm_STPAR */
{
    Std_ReturnType            result        = E_OK;
    const uint8*              reqData       = pMsgContext->reqData;
    uint8                     ioCtrlParam   = reqData[2u];
    const Dcm_DspDataCfgType* DcmDspDataCfg = Dcm_DspDidSignal[signalIndex].DidDataRef;

    if (DCM_UDS2F_SHORTTERMADJUSTMENT == ioCtrlParam)
    {
        if ((isDynamic) && (signalIndex == Dcm_DspDid[didIndex].DidSignalIndexEnd))
        {
            *stateSize = pMsgContext->reqDataLen - *maskOffset - didControlCfg->ControlMaskSize;
        }
        else
        {
            *stateSize = DcmDspDataCfg->DspDataByteSize;
        }
        (*maskOffset) += (uint16)*stateSize;
    }
    uint8 controlMask = reqData[*maskOffset + (signalIndex / 8u)] & (uint8)(1u << (signalIndex % 8u));
    for (uint32 subIndex = 0u; subIndex < didControlCfg->ControlMaskSize; subIndex++)
    {
        Dcm_activeIoControlCMER[didIndex][subIndex] = reqData[*maskOffset + subIndex];
    }

    if (DCM_CONTROLMASK_INTERNAL == didControlCfg->ControlMask)
    {
        if (0u == controlMask)
        {
            result = E_NOT_OK;
        }
    }

    return result;
}

/* handle ioCtrlParam */
/* PRQA S 6040++ */ /* VL_MTR_Dcm_STPAR */
DCM_LOCAL Std_ReturnType Dcm_UDS0x2F_HandleIoControl(
    const uint8*                  reqData,
    Dcm_ExtendedOpStatusType      OpStatus,
    const Dcm_DspDataCfgType*     DcmDspDataCfg,
    uint8                         ioCtrlParam,
    uint16                        stateOffset,
    Dcm_MsgLenType                stateSize,
    const uint8*                  controlMaskPtr,
    Dcm_NegativeResponseCodeType* ErrorCode)
/* PRQA S 6040-- */ /* VL_MTR_Dcm_STPAR */
{
    Std_ReturnType result = E_OK;

    switch (ioCtrlParam)
    {
    case DCM_UDS2F_RETURNCONTROLTOECU:
    {
        result = DcmDspDataCfg->ReturnControlToEcuFnc(controlMaskPtr, ErrorCode);
#if (STD_ON == DCM_DEV_ERROR_DETECT)
        if ((E_OK != result) && (E_NOT_OK != result))
        {
            DCM_DET_REPORT(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_RETURN_VALUE);
            *ErrorCode = DCM_E_GENERALREJECT;
            result     = E_NOT_OK;
        }
#endif
        break;
    }
    case DCM_UDS2F_RESETTODEFAULT:
    {
        result = DcmDspDataCfg->ResetToDefaultFnc(OpStatus, controlMaskPtr, ErrorCode);
        break;
    }
    case DCM_UDS2F_FREEZECURRENTSTATE:
    {
        result = DcmDspDataCfg->FreezeCurrentStateFnc(OpStatus, controlMaskPtr, ErrorCode);
        break;
    }
    case DCM_UDS2F_SHORTTERMADJUSTMENT:
    {
        result =
            DcmDspDataCfg
                ->ShortTermAdjustmentFnc(&reqData[stateOffset], (uint16)stateSize, OpStatus, controlMaskPtr, ErrorCode);
        break;
    }
    default:
    {
        /* idle */
        break;
    }
    }

    return result;
}
#endif
