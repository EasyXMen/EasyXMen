/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
**************************************************************************** **
**                                                                           **
**  FILENAME    :                                                            **
**                                                                           **
**  Created on  : 2020/5/6 14:29:43                                          **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION :                                                            **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#include "Dcm_Internal.h"

/****************************************************************
     UDS:InputOutputControlByIdentifier (2F hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X2F_ENABLED)
typedef struct
{
    uint32 Mask;
    uint16 InputDidSize;
    uint16 DidCfgIndex;
    const Dcm_DspDidControlType* pDspDidControl;
    uint16* ControlStatusRecordSize;
    uint32 Offset;
    const Dcm_DspDidSignalType* pDcmDspDidSignal;
    uint8* pControlEnableMaskRecord;
    uint8* pControlStatusRecord;
    uint16 InputDataOffset;
} Dcm_0x2FTypes;

/********************************/
#if (DCM_DSP_DID_FOR_2F_NUM > 0)
#define DCM_START_SEC_VAR_POWER_ON_INIT_16
#include "Dcm_MemMap.h"
static VAR(uint16, DCM_VAR_POWER_ON_INIT) DcmOnControlDidfor2F[DCM_DSP_DID_FOR_2F_NUM] = {0};
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_16
#include "Dcm_MemMap.h"
#endif /* DCM_DSP_DID_FOR_2F_NUM > 0 */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#if (DCM_DSP_DID_FOR_2F_NUM > 0)
/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
FUNC(void, DCM_CODE) Dcm_UDS0x2FCheckNewSession(Dcm_SesCtrlType NewSes) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    boolean Support = FALSE;
    uint8 ProtocolCtrlIdIndex;
    uint8 SidTabId;
    uint8 Service_Num;
    uint16 SidTabIndex;
    uint16 ServiceIndex;
    uint8 SesIndex;
    uint8 iloop;
    uint16 DidIndex;
    uint16 SingalIndex;
    uint16 DcmDspDidSignalNum;
    uint16 DcmDspDidId;
    Dcm_NegativeResponseCodeType ErrorCode;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid;
    const Dcm_DsdCfgType* pDcmDsdCfg = DcmPbCfgPtr->pDcmDsdCfg;
    uint16 ServiceTable_Num = pDcmDsdCfg->DcmDsdServiceTable_Num;
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow;
    const Dcm_DsdServiceTableCfgType* pServiceTable = DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable;
    const Dcm_DsdServiceCfgType* pService;
    const Dcm_DspDidSignalType* pDcmDspDidSignal;
    const Dcm_DspDataType* pDcmDspDidData;
    Dcm_DspDataUsePortEnumType DcmDspDataUsePort;

    for (ProtocolCtrlIdIndex = 0; (ProtocolCtrlIdIndex < ServiceTable_Num) && (FALSE == Support); ProtocolCtrlIdIndex++)
    {
        SidTabId = (pDcmDslProtocolRow)[ProtocolCtrlIdIndex].DcmDslServiceTableID;
        /*find SidTabId configuration position in the service configuration table*/
        for (SidTabIndex = 0; (SidTabIndex < ServiceTable_Num) && (FALSE == Support); SidTabIndex++)
        {
            if (SidTabId == (pServiceTable[SidTabIndex].DcmDsdSidTabId))
            {
                Service_Num = pServiceTable[SidTabIndex].DcmDsdSidTab_ServiceNum;
                for (ServiceIndex = 0; (ServiceIndex < Service_Num) && (FALSE == Support); ServiceIndex++)
                {
                    pService = &pServiceTable[SidTabIndex].pDcmDsdService[ServiceIndex];
                    if (0x2Fu == (pService->DcmDsdServiceId))
                    {
                        for (SesIndex = 0; (SesIndex < (pService->DcmDsdSessionLevel_Num)) && (Support == FALSE);
                             SesIndex++)
                        {
                            if (NewSes == pService->pDcmDsdSessionLevelRef[SesIndex])
                            {
                                Support = TRUE;
                            }
                        }
                    }
                }
            }
        }
    }
    if ((DCM_DEFAULT_SESSION == NewSes) || (Support == FALSE))
    {
        iloop = 0;
#if DCM_DSP_DID_FOR_2F_NUM > 1
        for (iloop = 0; iloop < DCM_DSP_DID_FOR_2F_NUM; iloop++)
#endif
        {
            if (DcmOnControlDidfor2F[iloop] != 0u)
            {
                for (DidIndex = 0; DidIndex < pDcmDspCfg->DcmDspDidNum; DidIndex++)
                {
                    pDcmDspDid = &pDcmDspCfg->pDcmDspDid[DidIndex];
                    DcmDspDidSignalNum = pDcmDspDid->DcmDspDidSignalNum;
                    DcmDspDidId = pDcmDspDid->DcmDspDidId;
                    pDcmDspDidSignal = pDcmDspDid->pDcmDspDidSignal;
                    for (SingalIndex = 0;
                         (SingalIndex < DcmDspDidSignalNum) && (DcmOnControlDidfor2F[iloop] == DcmDspDidId);
                         SingalIndex++)
                    {
                        pDcmDspDidData = pDcmDspDidSignal[SingalIndex].pDcmDspDidData;
                        DcmDspDataUsePort = pDcmDspDidData->DcmDspDataUsePort;
#if (DCM_DATA_ECU_SIGNAL_ENABLED == STD_ON)
                        if ((DcmDspDataUsePort == USE_ECU_SIGNAL)
                            && (pDcmDspDidData->DcmDspDataEcuSignalFnc != NULL_PTR))
                        {
                            pDcmDspDidData->DcmDspDataEcuSignalFnc(DCM_UDS0X2F_RETURNCONTROLTOECU, 0xFF);
                        }
                        else
#endif
                            if (((DcmDspDataUsePort == USE_DATA_SYNCH_FNC) || (DcmDspDataUsePort == USE_DATA_ASYNCH_FNC)
                                 || (DcmDspDataUsePort == USE_DATA_ASYNCH_FNC_ERROR)
                                 || (DcmDspDataUsePort == USE_DATA_SYNCH_CLIENT_SERVER)
                                 || (DcmDspDataUsePort == USE_DATA_ASYNCH_CLIENT_SERVER)
                                 || (DcmDspDataUsePort == USE_DATA_ASYNCH_CLIENT_SERVER_ERROR))
                                && (pDcmDspDidData->DcmDspDataReturnControlToECUFnc != NULL_PTR))
                        {

                            pDcmDspDidData->DcmDspDataReturnControlToECUFnc(NULL_PTR, &ErrorCode);
                        }
                        else
                        {
                            /*idle*/
                        }
                    }
                }
                DcmOnControlDidfor2F[iloop] = 0;
            }
        }
    }
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
FUNC(void, DCM_CODE) Dcm_UDS0x2FCheckNewSecurity(Dcm_SecLevelType NewSec) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    boolean Support = FALSE;
    uint8 ProtocolCtrlIdIndex;
    uint8 SidTabId;
    uint8 Service_Num;
    uint16 SidTabIndex;
    uint16 ServiceIndex;
    uint8 SesIndex;
    uint8 iloop;
    uint16 DidIndex;
    uint16 SingalIndex;
    Dcm_NegativeResponseCodeType ErrorCode;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DsdCfgType* pDcmDsdCfg = DcmPbCfgPtr->pDcmDsdCfg;
    uint16 DidSignalNum = pDcmDspDid->DcmDspDidSignalNum;
    uint16 ServiceTable_Num = pDcmDsdCfg->DcmDsdServiceTable_Num;
    uint16 DcmDspDidId;
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow;
    const Dcm_DsdServiceTableCfgType* pServiceTable = DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable;
    const Dcm_DsdServiceCfgType* pService;
    const Dcm_DspDidSignalType* pDcmDspDidSignal;
    const Dcm_DspDataType* pDcmDspDidData;
    Dcm_DspDataUsePortEnumType DcmDspDataUsePort;

    for (ProtocolCtrlIdIndex = 0; (ProtocolCtrlIdIndex < ServiceTable_Num) && (FALSE == Support); ProtocolCtrlIdIndex++)
    {
        SidTabId = pDcmDslProtocolRow[ProtocolCtrlIdIndex].DcmDslServiceTableID;
        /*find SidTabId configuration position in the service configuration table*/
        for (SidTabIndex = 0; (SidTabIndex < ServiceTable_Num) && (FALSE == Support); SidTabIndex++)
        {
            if (SidTabId == (pServiceTable[SidTabIndex].DcmDsdSidTabId))
            {
                Service_Num = pServiceTable[SidTabIndex].DcmDsdSidTab_ServiceNum;
                for (ServiceIndex = 0; (ServiceIndex < Service_Num) && (FALSE == Support); ServiceIndex++)
                {
                    pService = &pServiceTable[SidTabIndex].pDcmDsdService[ServiceIndex];
                    if (0x2Fu == (pService->DcmDsdServiceId))
                    {
                        for (SesIndex = 0; (SesIndex < (pService->DcmDsdSecurityLevel_Num)) && (Support == FALSE);
                             SesIndex++)
                        {
                            if (NewSec
                                == (DcmPbCfgPtr->pDcmDsdCfg->pDcmDsdServiceTable)[SidTabIndex]
                                       .pDcmDsdService[ServiceIndex]
                                       .pDcmDsdSecurityLevelRef[SesIndex])
                            {
                                Support = TRUE;
                            }
                        }
                    }
                }
            }
        }
    }
    if (Support == FALSE)
    {
        iloop = 0;
#if (DCM_DSP_DID_FOR_2F_NUM > 1)
        for (iloop = 0; iloop < DCM_DSP_DID_FOR_2F_NUM; iloop++)
#endif
        {
            if (DcmOnControlDidfor2F[iloop] != 0u)
            {
                for (DidIndex = 0; DidIndex < pDcmDspCfg->DcmDspDidNum; DidIndex++)
                {
                    pDcmDspDidSignal = pDcmDspDid[DidIndex].pDcmDspDidSignal;
                    DcmDspDidId = pDcmDspDid[DidIndex].DcmDspDidId;
                    for (SingalIndex = 0; (SingalIndex < DidSignalNum) && (DcmOnControlDidfor2F[iloop] == DcmDspDidId);
                         SingalIndex++)
                    {
                        pDcmDspDidData = pDcmDspDidSignal->pDcmDspDidData;
                        DcmDspDataUsePort = pDcmDspDidData->DcmDspDataUsePort;
#if (DCM_DATA_ECU_SIGNAL_ENABLED == STD_ON)
                        if ((DcmDspDataUsePort == USE_ECU_SIGNAL)
                            && (pDcmDspDidData->DcmDspDataEcuSignalFnc != NULL_PTR))
                        {
                            pDcmDspDidData->DcmDspDataEcuSignalFnc(DCM_UDS0X2F_RETURNCONTROLTOECU, 0xFF);
                        }
                        else
#endif
                            if (((DcmDspDataUsePort == USE_DATA_SYNCH_FNC) || (DcmDspDataUsePort == USE_DATA_ASYNCH_FNC)
                                 || (DcmDspDataUsePort == USE_DATA_ASYNCH_FNC_ERROR)
                                 || (DcmDspDataUsePort == USE_DATA_SYNCH_CLIENT_SERVER)
                                 || (DcmDspDataUsePort == USE_DATA_ASYNCH_CLIENT_SERVER)
                                 || (DcmDspDataUsePort == USE_DATA_ASYNCH_CLIENT_SERVER_ERROR))
                                && (pDcmDspDidData->DcmDspDataReturnControlToECUFnc != NULL_PTR))
                        {
                            pDcmDspDidData->DcmDspDataReturnControlToECUFnc(NULL_PTR, &ErrorCode);
                        }
                        else
                        {
                            /*idle*/
                        }
                    }
                }
                DcmOnControlDidfor2F[iloop] = 0;
            }
        }
    }
}
#endif /* DCM_DSP_DID_FOR_2F_NUM > 0 */

#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x2FServiceDidCallReadAPI(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x2FTypes, AUTOMATIC, DCM_VAR) Dcm_0x2FType,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    Dcm_ReadDataFncType DcmDspDataReadFnc = Dcm_0x2FType->pDcmDspDidSignal->pDcmDspDidData->DcmDspDataReadFnc;
    if (NULL_PTR != DcmDspDataReadFnc)
    {
        ret = DcmDspDataReadFnc(OpStatus, Dcm_0x2FType->pControlStatusRecord, pNrc);
    }
    else
    {
        /*DcmDspDataReadFnc is NULL,send NRC 0x22*/
        *pNrc = DCM_E_CONDITIONSNOTCORRECT;
        ret = E_NOT_OK;
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x2F_DidConditionCheck(
    P2CONST(Dcm_0x2FTypes, AUTOMATIC, DCM_VAR) Dcm_0x2FType,
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    uint8 ProtocolCtrlId,
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
{
    Std_ReturnType ret = E_OK;
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    boolean Flag;
    uint8 Index;
    uint8 SecRefNum = Dcm_0x2FType->pDspDidControl->DcmDspDidControlSecurityLevelRefNum;
    const uint8* pSecurityLevelRow = Dcm_0x2FType->pDspDidControl->pDcmDspDidControlSecurityLevelRow;
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_MsgType pReqData = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData;
    uint16 RecDid = (uint16)(((uint16)pReqData[1u]) << 8u) | ((uint16)pReqData[2u]);
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    uint16 DidInfoCfgIndex = pDcmDspDid[Dcm_0x2FType->DidCfgIndex].DcmDspDidInfoIndex;
    ret = DspInternal_DidAuthenticationCheck(ProtocolCtrlId, RecDid, DidInfoCfgIndex, pNrc);
    if (E_OK == ret)
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
    {
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
        /*check the current security level*/
        Flag = FALSE;
        if (SecRefNum != 0u)
        {
            for (Index = 0; (Index < SecRefNum) && (FALSE == Flag); Index++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSec == pSecurityLevelRow[Index])
                {
                    Flag = TRUE;
                }
            }
            if (FALSE == Flag)
            {
                /*if the processing is not supported in current security level,send NRC 0x33*/
                *pNrc = DCM_E_SECURITYACCESSDENIED;
                ret = E_NOT_OK;
            }
        }
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x2FServiceDidCallAPI_SubDeal(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x2FTypes, AUTOMATIC, DCM_VAR) Dcm_0x2FType,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    const uint8* pcontrolState = NULL_PTR;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    uint8 InputOutControlParameter = pMsgContext->pReqData[3u];
    const Dcm_DspDataType* pDcmDspDidData = Dcm_0x2FType->pDcmDspDidSignal->pDcmDspDidData;
    uint8* pControlEnableMaskRecord = Dcm_0x2FType->pControlEnableMaskRecord;

    switch (InputOutControlParameter)
    {
    case DCM_UDS0X2F_RETURNCONTROLTOECU: /*ReturnControlToEcu*/
        if (pDcmDspDidData->DcmDspDataReturnControlToECUFnc != NULL_PTR)
        {
            ret = DspInternalUDS0x2F_DidConditionCheck(
                Dcm_0x2FType,
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
                ProtocolCtrlId,
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
                ErrorCode);
            if (E_OK == ret)
            {
                ret = pDcmDspDidData->DcmDspDataReturnControlToECUFnc(pControlEnableMaskRecord, ErrorCode);
                if (ret == E_OK)
                {
                    ret = Dcm_Uds0x2FServiceDidCallReadAPI(OpStatus, Dcm_0x2FType, ErrorCode);
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /*NRC=0x31*/
        }
        break;
    case DCM_UDS0X2F_RESETTODEFAULT: /*ResetToDefault*/
        if ((Dcm_0x2FType->pDspDidControl->DcmDspDidResetToDefault == TRUE)
            && (pDcmDspDidData->DcmDspDataResetToDefaultFnc != NULL_PTR))
        {
            ret = DspInternalUDS0x2F_DidConditionCheck(
                Dcm_0x2FType,
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
                ProtocolCtrlId,
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
                ErrorCode);
            if (E_OK == ret)
            {
                ret = pDcmDspDidData->DcmDspDataResetToDefaultFnc(OpStatus, pControlEnableMaskRecord, ErrorCode);
                if (ret == E_OK)
                {
                    ret = Dcm_Uds0x2FServiceDidCallReadAPI(OpStatus, Dcm_0x2FType, ErrorCode);
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /*NRC=0x31*/
        }
        break;
    case DCM_UDS0X2F_FREEZECURRENTSTATE: /*FreezeCurrentState*/
        if ((Dcm_0x2FType->pDspDidControl->DcmDspDidFreezeCurrentState == TRUE)
            && (pDcmDspDidData->DcmDspDataFreezeCurrentStateFnc != NULL_PTR))
        {
            ret = DspInternalUDS0x2F_DidConditionCheck(
                Dcm_0x2FType,
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
                ProtocolCtrlId,
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
                ErrorCode);
            if (E_OK == ret)
            {
                ret = pDcmDspDidData->DcmDspDataFreezeCurrentStateFnc(OpStatus, pControlEnableMaskRecord, ErrorCode);
                if (ret == E_OK)
                {
                    ret = Dcm_Uds0x2FServiceDidCallReadAPI(OpStatus, Dcm_0x2FType, ErrorCode);
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /*NRC=0x31*/
        }
        break;
    case DCM_UDS0X2F_SHORTTERMADJUSTMENT: /*ShortTermAdjustment*/
        if ((Dcm_0x2FType->pDspDidControl->DcmDspDidShortTermAdjustement == TRUE)
            && (pDcmDspDidData->DcmDspDataShortTermAdjustmentFnc != NULL_PTR))
        {
            ret = DspInternalUDS0x2F_DidConditionCheck(
                Dcm_0x2FType,
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
                ProtocolCtrlId,
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
                ErrorCode);
            if (E_OK == ret)
            {
                if (0u != Dcm_0x2FType->pDcmDspDidSignal->pDcmDspDidData->DcmDspDataSize)
                {
                    pcontrolState = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[4u + Dcm_0x2FType->InputDataOffset]);
                }
                ret = pDcmDspDidData->DcmDspDataShortTermAdjustmentFnc(
                    pcontrolState,
                    pDcmDspDidData->DcmDspDataSize,
                    OpStatus,
                    pControlEnableMaskRecord,
                    ErrorCode);
                if (ret == E_OK)
                {
                    ret = Dcm_Uds0x2FServiceDidCallReadAPI(OpStatus, Dcm_0x2FType, ErrorCode);
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /*NRC=0x31*/
        }
        break;
    default:
        ret = E_NOT_OK;
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /*NRC=0x31*/
        break;
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x2FServiceDidCallAPI(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x2FTypes, AUTOMATIC, DCM_VAR) Dcm_0x2FType,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8* pControlEnableMaskRecord = NULL_PTR;
    uint8* pControlStatusRecord;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 Dcm_TxCtrlChannelIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    uint32 Offset =
        DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[Dcm_ChannelCtrl[Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex].offset;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
#if (DCM_DATA_ECU_SIGNAL_ENABLED == STD_ON)
    uint8 SignalValue;
    uint8 InputOutControlParameter = pMsgContext->pReqData[3u];
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    uint8 SecRefNum = Dcm_0x2FType->pDspDidControl->DcmDspDidControlSecurityLevelRefNum;
#endif
#endif

    const Dcm_DspDataType* pDcmDspDidData = Dcm_0x2FType->pDcmDspDidSignal->pDcmDspDidData;
    Dcm_DspDataUsePortEnumType DcmDspDataUsePort = pDcmDspDidData->DcmDspDataUsePort;

#if (DCM_DATA_ECU_SIGNAL_ENABLED == STD_ON)
    if ((DcmDspDataUsePort == USE_ECU_SIGNAL) && (pDcmDspDidData->DcmDspDataEcuSignalFnc != NULL_PTR))
    {
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
        boolean Flag;
        uint8 Index;
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED*/
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
        uint16 RecDid =
            (uint16)((uint16)((uint16)pMsgContext->pReqData[1u]) << 8u) | ((uint16)pMsgContext->pReqData[2u]);
        const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
        uint16 DidInfoCfgIndex = pDcmDspDid[Dcm_0x2FType->DidCfgIndex].DcmDspDidInfoIndex;
        ret = DspInternal_DidAuthenticationCheck(ProtocolCtrlId, RecDid, DidInfoCfgIndex, ErrorCode);
        if (E_OK == ret)
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
        {
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
            /*check the current security level*/

            Flag = FALSE;
            if (SecRefNum != 0u)
            {
                for (Index = 0; (Index < SecRefNum) && (FALSE == Flag); Index++)
                {
                    if (Dcm_MkCtrl.Dcm_ActiveSec
                        == (Dcm_0x2FType->pDspDidControl->pDcmDspDidControlSecurityLevelRow)[Index])
                    {
                        Flag = TRUE;
                    }
                }
                if (FALSE == Flag)
                {
                    /*if the processing is not supported in current security level,send NRC 0x33*/
                    *ErrorCode = DCM_E_SECURITYACCESSDENIED;
                    ret = E_NOT_OK;
                }
            }

#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */
        }
        if (E_OK == ret)
        {
            SignalValue = pMsgContext->pReqData[4u + Dcm_0x2FType->InputDataOffset];
            ret = pDcmDspDidData->DcmDspDataEcuSignalFnc(InputOutControlParameter, SignalValue);
        }
    }
    else
#endif
        if ((DcmDspDataUsePort == USE_DATA_SYNCH_FNC) || (DcmDspDataUsePort == USE_DATA_ASYNCH_FNC)
            || (DcmDspDataUsePort == USE_DATA_ASYNCH_FNC_ERROR) || (DcmDspDataUsePort == USE_DATA_SYNCH_CLIENT_SERVER)
            || (DcmDspDataUsePort == USE_DATA_ASYNCH_CLIENT_SERVER)
            || (DcmDspDataUsePort == USE_DATA_ASYNCH_CLIENT_SERVER_ERROR))
    {
        if (0u != Dcm_0x2FType->pDspDidControl->DcmDspDidControlMaskSize)
        {
            pControlEnableMaskRecord =
                &(pMsgContext->pReqData[4u + Dcm_0x2FType->InputDataOffset + pDcmDspDidData->DcmDspDataSize]);
        }
        pControlStatusRecord = &Dcm_Channel[Offset + 4u];
        Dcm_0x2FType->pControlEnableMaskRecord = pControlEnableMaskRecord;
        Dcm_0x2FType->pControlStatusRecord = pControlStatusRecord;
        ret = Dcm_Uds0x2FServiceDidCallAPI_SubDeal(OpStatus, Dcm_0x2FType, ProtocolCtrlId, ErrorCode);
        (*Dcm_0x2FType->ControlStatusRecordSize) += pDcmDspDidData->DcmDspDataSize;
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static Std_ReturnType DspInternalUDS0x2F_DidCheck(
    uint16 RecDid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pDidCfgIndex,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 Index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;

    /*find the corresponding DID in configuration*/
    for (Index = 0; (Index < pDcmDspCfg->DcmDspDidNum) && (E_NOT_OK == ret); Index++)
    {
        /*single did check*/
        if ((RecDid == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
        {
            *pDidCfgIndex = Index;
            ret = E_OK;
        }
    }
    if (E_NOT_OK == ret)
    {
        /*if not found,send NRC 0x31*/
        (*pNrc) = DCM_E_REQUESTOUTOFRANGE;
    }
    return ret;
}
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x2FServiceConditionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) DidCfgIndex,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) InputDidSize,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_DspDidSignalType* pDcmDspDidSignal = NULL_PTR;
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 DidInfoCfgIndex;
    uint16 RecDid;
    uint8 InputOutControlParameter;
    uint16 iloop;
    const Dcm_DspDidControlType* pDspDidControl;
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    boolean Flag = FALSE;
    uint16 Index;
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */

    /*min-length check*/
    if (DCM_UDS0X2F_REQ_DATA_MINLENGTH > pMsgContext->ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
#if (STD_OFF == DCM_DSP_DID_FUNC_ENABLED)
    /*NRC 0x31:request out of range*/
    if (E_OK == ret)
    {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#else /* STD_OFF == DCM_DSP_DID_FUNC_ENABLED */
    if (E_OK == ret)
    {
        /*get the required DID from request message*/
        RecDid = (uint16)((uint16)((uint16)pMsgContext->pReqData[1u]) << 8u) | ((uint16)pMsgContext->pReqData[2u]);
        /*Determine if the DID is configured*/
        ret = DspInternalUDS0x2F_DidCheck(RecDid, DidCfgIndex, ErrorCode);
    }
    if (E_OK == ret)
    {
        DidInfoCfgIndex = pDcmDspDid[*DidCfgIndex].DcmDspDidInfoIndex;
        /*check whether the Did can be configured*/
        pDspDidControl = pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidControl;
        if (NULL_PTR == pDspDidControl)
        {
            /*if the Did can be dynamically defined,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        /*check inputOutputControlParameter is supported*/
        InputOutControlParameter = pMsgContext->pReqData[3u];
        if ((InputOutControlParameter > DCM_UDS0X2F_SHORTTERMADJUSTMENT)
            || ((InputOutControlParameter == DCM_UDS0X2F_SHORTTERMADJUSTMENT)
                && (pDspDidControl->DcmDspDidShortTermAdjustement == FALSE))
            || ((InputOutControlParameter == DCM_UDS0X2F_RESETTODEFAULT)
                && (pDspDidControl->DcmDspDidResetToDefault == FALSE))
            || ((InputOutControlParameter == DCM_UDS0X2F_FREEZECURRENTSTATE)
                && (pDspDidControl->DcmDspDidFreezeCurrentState == FALSE)))
        {
            /*if inputOutputControlParameter is not supported,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    /*session check*/
    if ((E_OK == ret) && (pDspDidControl->DcmDspDidControlSessionRefNum != 0u))
    {
        for (Index = 0; (Index < pDspDidControl->DcmDspDidControlSessionRefNum) && (FALSE == Flag); Index++)
        {
            if (Dcm_MkCtrl.Dcm_ActiveSes == (pDspDidControl->pDcmDspDidControlSessionRow)[Index])
            {
                Flag = TRUE;
            }
        }
        if (FALSE == Flag)
        {
            /*if the processing is not supported in current session,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED */
    if (E_OK == ret)
    {
        if ((InputOutControlParameter == DCM_UDS0X2F_SHORTTERMADJUSTMENT))
        {
            if ((pDcmDspDid[*DidCfgIndex].pDcmDspDidSignal != NULL_PTR))
            {
                pDcmDspDidSignal = pDcmDspDid[*DidCfgIndex].pDcmDspDidSignal;
                for (iloop = 0; ((iloop < pDcmDspDid[*DidCfgIndex].DcmDspDidSignalNum) && (ret == E_OK)); iloop++)
                {
                    (*InputDidSize) += pDcmDspDidSignal->pDcmDspDidData->DcmDspDataSize;
                    /*configuration check*/
                    if ((pDspDidControl->DcmDspDidControlMask != DCM_CONTROLMASK_NO)
                        && ((pDcmDspDidSignal->pDcmDspDidData->DcmDspDataUsePort == USE_DATA_SENDER_RECEIVER)
                            || (pDcmDspDidSignal->pDcmDspDidData->DcmDspDataUsePort
                                == USE_DATA_SENDER_RECEIVER_AS_SERVICE)))
                    {
                        /* send NRC 0x13*/
                        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                        ret = E_NOT_OK;
                    }
                    pDcmDspDidSignal++;
                }
            }
        }
        else
        {
            if ((DCM_UDS0X2F_REQ_DATA_MINLENGTH + pDspDidControl->DcmDspDidControlMaskSize)
                != (uint8)pMsgContext->ReqDataLen)
            {
                /* send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
        }
    }
#endif /* STD_OFF == DCM_DSP_DID_FUNC_ENABLED */
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2FSubDeal(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x2FTypes, AUTOMATIC, DCM_VAR) Dcm_0x2FType,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint16 iloop;
    uint8 Postion;
    uint16 InputDataOffset = 0;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 InputOutControlParameter = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[3u];
    const Dcm_DspDidSignalType* pDcmDspDidSignal;
    const Dcm_DspDidType* pDcmDspDid = &DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid[Dcm_0x2FType->DidCfgIndex];
    const Dcm_DspDidControlType* pDspDidControl = Dcm_0x2FType->pDspDidControl;
    uint8 DcmDspDidControlMaskSize = pDspDidControl->DcmDspDidControlMaskSize;
    uint16 DidSignalNum = pDcmDspDid->DcmDspDidSignalNum;

    /*total length check*/
    if (((Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen
          != (uint32)((uint32)((uint32)4UL + Dcm_0x2FType->InputDidSize + DcmDspDidControlMaskSize)))
         && (InputOutControlParameter == DCM_UDS0X2F_SHORTTERMADJUSTMENT))
        || ((Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen != (uint32)((uint32)((uint32)4UL + DcmDspDidControlMaskSize)))
            && (InputOutControlParameter != DCM_UDS0X2F_SHORTTERMADJUSTMENT)))
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        if (DidSignalNum > 1u)
        {
            if (pDcmDspDid->pDcmDspDidSignal != NULL_PTR)
            {
                pDcmDspDidSignal = pDcmDspDid->pDcmDspDidSignal;
                for (iloop = 0; iloop < DidSignalNum; iloop++)
                {
                    if ((pDspDidControl->DcmDspDidControlMask == DCM_CONTROLMASK_INTERNAL)
                        && (DcmDspDidControlMaskSize >= (DidSignalNum / 8u)))
                    {
                        Postion =
                            (1u == DcmDspDidControlMaskSize)
                                ? 8u
                                : ((2u == DcmDspDidControlMaskSize) ? 16u
                                                                    : ((3u == DcmDspDidControlMaskSize) ? 24u : 32u));
                        if ((Dcm_0x2FType->Mask & (uint32)((uint32)((uint32)0x01u << (Postion - iloop)))) != 0UL)
                        {
                            Dcm_0x2FType->InputDataOffset = InputDataOffset;
                            Dcm_0x2FType->pDcmDspDidSignal = pDcmDspDidSignal;
                            ret = Dcm_Uds0x2FServiceDidCallAPI(OpStatus, Dcm_0x2FType, ProtocolCtrlId, ErrorCode);
                        }
                    }
                    else
                    {
                        Dcm_0x2FType->InputDataOffset = InputDataOffset;
                        Dcm_0x2FType->pDcmDspDidSignal = pDcmDspDidSignal;
                        ret = Dcm_Uds0x2FServiceDidCallAPI(OpStatus, Dcm_0x2FType, ProtocolCtrlId, ErrorCode);
                    }
                    InputDataOffset += pDcmDspDidSignal->pDcmDspDidData->DcmDspDataSize;
                    pDcmDspDidSignal++;
                }
            }
            else
            {
                /*if pDcmDspDidSignal is not supported,send NRC 0x22*/
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
        }
        else if (pDcmDspDid->DcmDspDidSignalNum == 1u)
        {
            pDcmDspDidSignal = pDcmDspDid->pDcmDspDidSignal;
            Dcm_0x2FType->InputDataOffset = InputDataOffset;
            Dcm_0x2FType->pDcmDspDidSignal = pDcmDspDidSignal;
            ret = Dcm_Uds0x2FServiceDidCallAPI(OpStatus, Dcm_0x2FType, ProtocolCtrlId, ErrorCode);
        }
        else
        {
            /*if DcmDspDidSignalNum is 0,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/**********************************************************************/
/*
 * Brief
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/***********************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x2F(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 RecDid;
    uint16 InputDidSize = 0;
    uint32 Offset;
    uint16 DidCfgIndex;
    uint16 DidInfoCfgIndex;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint16 ControlStatusRecordSize = 0;
    uint8 InputOutControlParameter;
    const Dcm_DspDidControlType* pDspDidControl;
    uint8 iloop;
    uint32 Mask = 0;
    Dcm_0x2FTypes Dcm_0x2FType;
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    ret = Dcm_Uds0x2FServiceConditionCheck(ProtocolCtrlId, &DidCfgIndex, &InputDidSize, ErrorCode);
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*check inputOutputControlParameter is supported*/
        InputOutControlParameter = pMsgContext->pReqData[3u];
        /*get the required DID from request message*/
        RecDid = (uint16)((uint16)((uint16)pMsgContext->pReqData[1u]) << 8u) | ((uint16)pMsgContext->pReqData[2u]);
        DidInfoCfgIndex = pDcmDspDid[DidCfgIndex].DcmDspDidInfoIndex;
        /*check whether the Did can be configured*/
        pDspDidControl = pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidControl;
        if ((pDspDidControl->DcmDspDidControlMask == DCM_CONTROLMASK_NO)
            && (pDspDidControl->DcmDspDidControlMaskSize != 0u))
        {
            /*send NRC 0x13*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset = pDcmChannelCfg->offset;
        /*Calculate the Mask*/
        for (iloop = 0; iloop < pDspDidControl->DcmDspDidControlMaskSize; iloop++)
        {
            Mask = Mask << 8u;
            Mask |= (uint32)(pMsgContext->pReqData[4u + InputDidSize + (uint16)iloop]);
        }
        Dcm_0x2FType.Mask = Mask;
        Dcm_0x2FType.InputDidSize = InputDidSize;
        Dcm_0x2FType.DidCfgIndex = DidCfgIndex;
        Dcm_0x2FType.pDspDidControl = pDspDidControl;
        Dcm_0x2FType.ControlStatusRecordSize = &ControlStatusRecordSize;
        ret = Dcm_UDS0x2FSubDeal(OpStatus, ProtocolCtrlId, &Dcm_0x2FType, ErrorCode);
    }
#if (DCM_DSP_DID_FOR_2F_NUM > 0)
    if (E_OK == ret)
    {
        if (InputOutControlParameter != DCM_UDS0X2F_RETURNCONTROLTOECU)
        {
            iloop = 0;
#if (DCM_DSP_DID_FOR_2F_NUM > 1) /*store the control DID*/
            for (iloop = 0; iloop < DCM_DSP_DID_FOR_2F_NUM; iloop++)
#endif
            {
                if (DcmOnControlDidfor2F[iloop] == 0u)
                {
                    DcmOnControlDidfor2F[iloop] = RecDid;
#if (DCM_DSP_DID_FOR_2F_NUM > 1) /*store the control DID*/
                    break;
#endif
                }
            }
        }
        else
        {
            iloop = 0;
#if DCM_DSP_DID_FOR_2F_NUM > 1 /*clear the control DID*/
            for (iloop = 0; iloop < DCM_DSP_DID_FOR_2F_NUM; iloop++)
#endif
            {
                if (DcmOnControlDidfor2F[iloop] == RecDid)
                {
                    DcmOnControlDidfor2F[iloop] = 0;
#if (DCM_DSP_DID_FOR_2F_NUM > 1) /*store the control DID*/
                    break;
#endif
                }
            }
        }
    }
#endif /* DCM_DSP_DID_FOR_2F_NUM > 0 */
    /* check tx data length */
    if ((E_OK == ret) && ((4u + (uint32)ControlStatusRecordSize) > pDcmChannelCfg->Dcm_DslBufferSize))
    {
        /*Pdu length is bigger than buffer size,ignore the request message */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    /* send response message */
    switch (ret)
    {
    case E_OK:
        Dcm_Channel[Offset] = 0x6F;                          /*response SID*/
        Dcm_Channel[Offset + 1u] = (uint8)(RecDid >> 8u);    /*response Did MSB*/
        Dcm_Channel[Offset + 2u] = (uint8)RecDid;            /*response Did LSB*/
        Dcm_Channel[Offset + 3u] = InputOutControlParameter; /*InputOutControlParameter*/
        pMsgContext->ResMaxDataLen = 4u + (uint32)ControlStatusRecordSize;
        pMsgContext->ResDataLen = 4u + (uint32)ControlStatusRecordSize;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
        break;
    case E_NOT_OK:
        break;
    case DCM_E_PENDING:
        Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
        break;
    default:
        /*NRC=0x22*/
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        ret = E_NOT_OK;
        break;
    }
    return ret;
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_UDS_SERVICE0X2F_ENABLED */
