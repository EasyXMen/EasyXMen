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
         UDS:ReadDataByIdentifier (22 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X22_ENABLED)

typedef struct
{
    uint32* ResOffset;
    uint16 RecDid;
    uint16 DidSize;
    uint8* pRangeDidCfgIndex;
    uint16* pDidCfgIndex;
    boolean* pRangeDidFlag;
    uint8* NoFindDidReadNum;
    uint8* DidSessionSupportNum;
    uint8* NoFindDidNum;
    uint8* MixPid;
    uint8* noFindPidNum;
    uint16 DidCfgIndex;
    boolean* readDidSignalFlag;
} Dcm_0x22Types;

#define DCM_START_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"
/* PRQA S 3218++ */ /* MISRA Rule 8.9 */
static uint8 Dcm_0x22DidReadNvmFlag = 0xFF;
/* PRQA S 3218-- */ /* MISRA Rule 8.9 */
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  0x2C sub process>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#if (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED)
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UdsAssembleResponse_0x2CSubDeal(
    Dcm_OpStatusType OpStatus,
    P2CONST(Dcm_DDDidElementsDataTypes, AUTOMATIC, DCM_CONST) DDDid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DCM_VAR) ResOffset,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_OK;
    uint16 Did;
    uint8 Pos;
    uint8 Data[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint16 Index;
    uint16 pDidCfgIndex = 0;
    uint16 DidSignalPos = 0;
    uint16 Size;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    uint16 DcmDspDidSignalNum;
    const Dcm_DspDidSignalType* pDcmDspDidSignal;

    Did = (uint16)(DDDid->Data & 0xFFFFUL);
    Pos = (uint8)((DDDid->Data & 0xFF0000UL) >> 16u);
    for (Index = 0; Index < pDcmDspCfg->DcmDspDidNum; Index++)
    {
        /*single did check*/
        if ((Did == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
        {
            pDidCfgIndex = Index;
            break;
        }
    }
    DcmDspDidSignalNum = pDcmDspDid[pDidCfgIndex].DcmDspDidSignalNum;
    pDcmDspDidSignal = pDcmDspDid[pDidCfgIndex].pDcmDspDidSignal;
    for (Index = 0; (Index < DcmDspDidSignalNum) && (E_OK == ret); Index++)
    {
        pDspDidData = pDcmDspDidSignal[Index].pDcmDspDidData;
        Size = 0;
#if (STD_ON == DCM_DATA_TYPE_UINT8_DYN_ENABLED)
        if (DCM_UINT8_DYN == pDspDidData->DcmDspDataType)
        {
            /*DidDataSize is dynamic*/
            if (NULL_PTR == pDspDidData->DcmDspDataReadDataLengthFnc)
            {
                /*DcmDspDidReadDataLengthFnc is NULL,send NRC 0x22*/
                *pNrc = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
            else
            {
                ret = pDspDidData->DcmDspDataReadDataLengthFnc(OpStatus, &Size);
            }
        }
        else
#endif
        {
            /*DidDataSize is fixed*/
            Size = pDspDidData->DcmDspDataSize;
        }
        if (E_OK == ret)
        {
            DidSignalPos += pDcmDspDidSignal[Index].DcmDspDidDataPos;
            if ((NULL_PTR == pDspDidData->DcmDspDataReadFnc) || ((DidSignalPos + DDDid->Size) > DCM_TEMP_BUFFER_LENGTH))
            {
                /*DcmDspDataReadFnc is NULL,send NRC 0x22*/
                *pNrc = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
            else
            {
                ret = pDspDidData->DcmDspDataReadFnc(OpStatus, &Data[DidSignalPos], pNrc);
            }
            DidSignalPos += Size;
        }
    }
    Dcm_MemoryCopy(&(Data[Pos]), &(Dcm_Channel[*ResOffset]), DDDid->Size);
    *ResOffset += DDDid->Size;
    return ret;
}
#endif

#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  0x2C sub process>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UdsAssembleResponse_0x2C(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    const Dcm_DDDidElementsDataTypes* DDDid;
    uint8 DDDidNum;
    uint16 index;
    uint8 RangeInfoIndex;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo;
    const Dcm_DspMemoryIdInfoType* pDcmDspMemoryIdInfo;
    uint8 MemoryIdentifier = 0;
    uint8 Index;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 iloop;
    boolean Find = FALSE;
    const Dcm_DspMemoryType* pDcmDspMemory = DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory;
    uint8 MemoryIdInfoNum = pDcmDspMemory->DcmDspMemoryIdInfoNum;
    uint8 RangeInfoNum;

    for (index = 0; (index < DCM_DSP_DDDID_MAX_NUMBER) && (Find == FALSE); index++)
    {
        if (Dcm_0x22Type->RecDid == Dcm_DDDid[index].DDDid)
        {
            Find = TRUE;
        }
    }
    index--;
    if (Find == TRUE)
    {
        ret = DsdInternal_DDDIDcheckPerSourceDID(index, pNrc);
        /*set the response message*/
        if (E_OK == ret)
        {
            DDDidNum = Dcm_DDDid[index].DDDIDNumOfElements;
            for (iloop = 0; (iloop < DDDidNum) && (E_OK == ret); iloop++)
            {
                DDDid = &Dcm_DDDid[index].DcmDspAlternativeArgumentData[iloop];
                if (DDDid->Subfunction == DCM_UDS0X2C_01_DDBYDID)
                {
                    ret = Dcm_UdsAssembleResponse_0x2CSubDeal(OpStatus, DDDid, Dcm_0x22Type->ResOffset, pNrc);
                }
                else if (DDDid->Subfunction == DCM_UDS0X2C_02_DDBYMEMORY)
                {
                    for (Index = 0u; Index < MemoryIdInfoNum; Index++)
                    {
                        pDcmDspMemoryIdInfo = &(pDcmDspMemory->DcmDspMemoryIdInfo[Index]);
                        RangeInfoNum = pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfoNum;
                        for (RangeInfoIndex = 0; (RangeInfoIndex < RangeInfoNum) && (pDcmDspMemoryIdInfo != NULL_PTR);
                             RangeInfoIndex++)
                        {
                            pDcmDspReadMemoryRangeInfo =
                                &(pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfo[RangeInfoIndex]);
                            if ((pDcmDspReadMemoryRangeInfo != NULL_PTR)
                                && ((pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeLow <= DDDid->Data)
                                    && (pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeHigh
                                        >= (DDDid->Data + DDDid->Size - 1u))))
                            {
                                MemoryIdentifier = pDcmDspMemoryIdInfo->DcmDspMemoryIdValue;
                            }
                        }
                    }
                    ret = Dcm_ReadMemory(
                        OpStatus,
                        MemoryIdentifier,
                        DDDid->Data,
                        (uint32)DDDid->Size,
                        &(Dcm_Channel[*Dcm_0x22Type->ResOffset]),
                        pNrc);
                    if (DCM_READ_FAILED == ret)
                    {
                        /*the processing is not successful,send NRC */
                        ret = E_NOT_OK;
                    }
#if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
                    else if (DCM_READ_FORCE_RCRRP == ret)
                    {
                        /*the processing is pending,send NRC 0x78 */
                        ret = DCM_E_FORCE_RCRRP;
                        DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_RESPONSE_PENDING);
                        DsdInternal_ProcessingDone(ProtocolCtrlId);
                        Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = DCM_E_FORCE_RCRRP;
                        Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                    }
#endif
                    else if (DCM_READ_PENDING == ret)
                    {
                        ret = DCM_E_PENDING;
                        Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                    }
                    else
                    {
                        /*idle*/
                    }
                    (*Dcm_0x22Type->ResOffset) += DDDid->Size;
                }
                else
                {
                    *pNrc = DCM_E_CONDITIONSNOTCORRECT;
                    ret = E_NOT_OK;
                }
            }
        }
    }
    else
    {
        *pNrc = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
    return ret;
}
#endif
#endif
#endif
/************************/
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Did Number check>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_DidNumbercheck(uint16 numberOfDid, Dcm_MsgLenType length)
{
    Std_ReturnType ret = E_OK;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    if ((0u == (length & 1u)) || (length < DCM_UDS0X22_REQ_DATA_MINLENGTH))
    {
        ret = E_NOT_OK;
    }
    /*check the limitation of number of required DID*/
    else if ((0u != pDcmDspCfg->DcmDspMaxDidToRead) && (numberOfDid > pDcmDspCfg->DcmDspMaxDidToRead))
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* idle */
    }
    return ret;
}
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Check Supported>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DspInternalUDS0x22_CheckSupported(uint16 Did, P2VAR(uint32, AUTOMATIC, DCM_VAR) ResOffset)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint32 SupportBuffer = 0;
    uint16 iloop;
    boolean Find = FALSE;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;

    for (iloop = 0; iloop < pDcmDspCfg->DcmDspDidNum; iloop++)
    {
        if ((pDcmDspDid[iloop].DcmDspDidId >= (Did + 0x01u)) && (pDcmDspDid[iloop].DcmDspDidId <= (Did + 0x20u)))
        {
            SupportBuffer |= (uint32)1UL << (0x20u - (pDcmDspDid[iloop].DcmDspDidId - Did));
        }
        if (pDcmDspDid[iloop].DcmDspDidId == Did)
        {
            Find = TRUE;
        }
    }

    if (Find == FALSE)
    {
        SupportBuffer = 0;
    }

    if (SupportBuffer != 0UL)
    {
        SchM_Enter_Dcm_ExclusiveArea();
        Dcm_Channel[*ResOffset] = (uint8)((Did) >> 8u); /*echo of DID MSB*/
        *ResOffset += 1u;
        Dcm_Channel[*ResOffset] = (uint8)(Did);
        *ResOffset += 1u;
        Dcm_FillTo4bytes(&(Dcm_Channel[*ResOffset]), &SupportBuffer);
        *ResOffset += 4u;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        Dcm_PageBufferData.LastFilledSize = 6u;
#endif
        SchM_Exit_Dcm_ExclusiveArea();
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}
#endif
#endif
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Did Check >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DspInternalUDS0x22_DidCheck(Dcm_OpStatusType OpStatus, P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{

    uint16 Index;
    boolean Flag = FALSE;
    Std_ReturnType ret = E_NOT_OK;
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
    uint16 iloop;
    uint16 DcmDspDidInfoIndex;
#endif
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    uint16 DidNum = pDcmDspCfg->DcmDspDidNum;
#if (DCM_DID_RANGE_ENABLED == STD_ON)
    boolean Flagx = FALSE;
    uint8 Idx;
    Dcm_DidSupportedType DidSupported;
    uint8 DidRangeNum = pDcmDspCfg->DcmDspDidRangeNum;
    const Dcm_DspDidRangeType* pDcmDspDidRange = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidRange;
#endif

    (*Dcm_0x22Type->pRangeDidFlag) = FALSE;
    /*first check whether the receiveDid is single did*/
    for (Index = 0; (Index < DidNum) && (FALSE == Flag); Index++)
    {
        /*single did check*/
        if ((Dcm_0x22Type->RecDid == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
        {
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
            DcmDspDidInfoIndex = pDcmDspDid[Index].DcmDspDidInfoIndex;
            const Dcm_DspDidInfoType* pDcmDspDidInfo = &DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo[DcmDspDidInfoIndex];
            if ((Dcm_0x22Type->RecDid >= 0xF200u) && (Dcm_0x22Type->RecDid <= 0xF3FFu)
                && (pDcmDspDidInfo->DcmDspDidDynamicallyDefined == TRUE)
                && (pDcmDspDidInfo->DcmDspDDDIDMaxElements > 0u))
            {
                for (iloop = 0; (iloop < DCM_DSP_DDDID_MAX_NUMBER) && (Flag == FALSE); iloop++)
                {
                    if (pDcmDspDid[Index].DcmDspDidId == Dcm_DDDid[iloop].DDDid)
                    {
                        Flag = TRUE;
                        (*Dcm_0x22Type->pDidCfgIndex) = Index;
                        ret = E_OK;
                    }
                }
            }
            else
#endif
            {
                Flag = TRUE;
                (*Dcm_0x22Type->pDidCfgIndex) = Index;
                ret = E_OK;
            }
        }
    }
#if (DCM_DID_RANGE_ENABLED == STD_ON)
    /*if the receiveDid is not single did,the check whether the receiveDid is range did*/
    if (FALSE == Flag)
    {
        /*range did check*/
        if ((Dcm_0x22Type->RecDid < 0xF200u) || (Dcm_0x22Type->RecDid > 0xF3FFu))
        {
            /*range did can not be DDDid*/
            for (Idx = 0; (Idx < DidRangeNum) && (FALSE == Flagx); Idx++)
            {
                /*this range not have gaps*/
                if ((Dcm_0x22Type->RecDid >= pDcmDspDidRange[Idx].DcmDspDidRangeIdentifierLowerLimit)
                    && (Dcm_0x22Type->RecDid <= pDcmDspDidRange[Idx].DcmDspDidRangeIdentifierUpperLimit))
                {
                    if (TRUE == pDcmDspDidRange[Idx].DcmDspDidRangeHasGaps)
                    {
                        if (pDcmDspDidRange[Idx].DcmDspDidRangeIsDidAvailableFnc != NULL_PTR)
                        {
                            ret = pDcmDspDidRange[Idx].DcmDspDidRangeIsDidAvailableFnc(
                                Dcm_0x22Type->RecDid,
                                OpStatus,
                                &DidSupported);
                            if ((ret == E_OK) && (DCM_DID_SUPPORTED == DidSupported))
                            {
                                *Dcm_0x22Type->pRangeDidCfgIndex = Idx;
                                *Dcm_0x22Type->pRangeDidFlag = TRUE;
                                Flagx = TRUE;
                            }
                        }
                    }
                    else
                    {
                        *Dcm_0x22Type->pRangeDidCfgIndex = Idx;
                        *Dcm_0x22Type->pRangeDidFlag = TRUE;
                        Flagx = TRUE;
                    }
                }
            }
        }
        if (FALSE == Flagx)
        {
            ret = E_NOT_OK;
        }
        else
        {
            (*Dcm_0x22Type->pDidCfgIndex) = Index;
            ret = E_OK;
        }
    }
#else
    DCM_UNUSED(OpStatus);
#endif
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Did  Info Check >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_DidInfoCheck(uint16 DidInfoCfgIdx)
{
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
    if (pDcmDspDidInfo[DidInfoCfgIdx].pDcmDspDidRead != NULL_PTR)
    {
        ret = E_OK;
    }
    return ret;
}
#endif

/************************/
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Did  Session Check >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_DidSessionCheck(
    uint16 DidInfoCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) DidSessionSupportNum,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 Index;
    boolean Flag = FALSE;
    const Dcm_DspDidReadType* pDspDidRead;
    Std_ReturnType ret = E_OK;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
    pDspDidRead = pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidRead;
    uint8 SesRefNum = pDspDidRead->DcmDspDidReadSessionRefNum;
    if (NULL_PTR == pDspDidRead)
    {
        /*if the DcmDspDidRead of required Did is not configured,send NRC 0x31*/
        *pNrc = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
    else
    {
        if (SesRefNum != 0u)
        {
            for (Index = 0; (Index < SesRefNum) && (FALSE == Flag); Index++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSes == pDspDidRead->pDcmDspDidReadSessionRow[Index])
                {
                    Flag = TRUE;
                    (*DidSessionSupportNum) += 1u;
                }
            }
            if (FALSE == Flag)
            {
                ret = E_NOT_OK;
            }
        }
        else
        {
            (*DidSessionSupportNum) += 1u;
        }
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Did  Ecu  Signal Check >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) Dsp_UDS0x22_DidEcuSignalCheck(
#if ((NVM_ENABLE == STD_ON) && (DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON))
    Dcm_OpStatusType OpStatus,
    uint8 MsgCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, AUTOMATIC) pNrc,
#endif
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 DidSignalIndex;
    uint16 DidSignalNum;
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_NOT_OK;
#if ((NVM_ENABLE == STD_ON) && (DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON))
    NvM_RequestResultType NvmRet;
#endif
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    uint16 DidCfgIndex = Dcm_0x22Type->DidCfgIndex;
    const Dcm_DspDidSignalType* pDcmDspDidSignal = pDcmDspDid[DidCfgIndex].pDcmDspDidSignal;

    *Dcm_0x22Type->readDidSignalFlag = FALSE;
    DidSignalNum = pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum;
    for (DidSignalIndex = 0; DidSignalIndex < DidSignalNum; DidSignalIndex++)
    {
        pDspDidData = pDcmDspDidSignal[DidSignalIndex].pDcmDspDidData;
#if (DCM_DATA_ECU_SIGNAL_ENABLED == STD_ON)
        if (USE_ECU_SIGNAL == pDspDidData->DcmDspDataUsePort)
        {
            pDspDidData->DcmDspDataReadEcuSignalFnc(&Dcm_Channel[*Dcm_0x22Type->ResOffset]);
            (*Dcm_0x22Type->ResOffset) += (uint32)pDspDidData->DcmDspDataSize;
            *Dcm_0x22Type->readDidSignalFlag = TRUE;
            ret = E_OK;
        }
        else
#endif
            if (USE_BLOCK_ID == pDspDidData->DcmDspDataUsePort)
        {
#if ((NVM_ENABLE == STD_ON) && (DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON))
            *Dcm_0x22Type->readDidSignalFlag = TRUE;
            if (DCM_INITIAL == OpStatus)
            {
                (void)NvM_ReadBlock(pDspDidData->DcmDspDataBlockId, (void*)&Dcm_Channel[*Dcm_0x22Type->ResOffset]);
            }
            if (DCM_CANCEL != OpStatus)
            {
                (void)NvM_GetErrorStatus(pDspDidData->DcmDspDataBlockId, &NvmRet);
                if ((NVM_REQ_OK == NvmRet) || (NVM_REQ_RESTORED_FROM_ROM == NvmRet))
                {
                    (*Dcm_0x22Type->ResOffset) += (uint32)pDspDidData->DcmDspDataSize;
                    Dcm_0x22DidReadNvmFlag = E_OK;
                    ret = E_OK;
                }
                else if (NVM_REQ_PENDING == NvmRet)
                {
                    (*Dcm_0x22Type->ResOffset) += (uint32)pDspDidData->DcmDspDataSize;
                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                    Dcm_0x22DidReadNvmFlag = DCM_PENDING;
                    ret = DCM_E_PENDING;
                }
                else
                {
                    *pNrc = DCM_E_CONDITIONSNOTCORRECT;
                }
            }
            else
            {
                /* If the Dcm cancels a service with NvM access, it shall call NvM_CancelJobs(). */
                (void)NvM_CancelJobs(pDspDidData->DcmDspDataBlockId);
            }
#endif /*  NVM_ENABLE == STD_ON && DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON*/
        }
        else
        {
            /*idle*/
        }
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Did  Condition Check >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_DidConditionCheck(
    Dcm_OpStatusType OpStatus,
    uint16 DidCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 DidSignalIndex;
    uint16 DidSignalNum;
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_OK;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;

    DidSignalNum = pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum;
    for (DidSignalIndex = 0; DidSignalIndex < DidSignalNum; DidSignalIndex++)
    {
        pDspDidData = pDcmDspDid[DidCfgIndex].pDcmDspDidSignal[DidSignalIndex].pDcmDspDidData;
        boolean DcmConditionCheckReadFncUsed = pDspDidData->DcmConditionCheckReadFncUsed;
        Dcm_ConditionCheckReadFncType DataConditionCheckReadFnc = pDspDidData->DcmDspDataConditionCheckReadFnc;
        if ((TRUE == DcmConditionCheckReadFncUsed) && (NULL_PTR != DataConditionCheckReadFnc))
        {

            ret = DataConditionCheckReadFnc(OpStatus, pNrc);
        }
        if ((TRUE == DcmConditionCheckReadFncUsed) && (NULL_PTR == DataConditionCheckReadFnc))
        {
            *pNrc = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
        }
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Read Range Did DataLength >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if ((STD_ON == DCM_DSP_DID_FUNC_ENABLED) && (STD_ON == DCM_DID_RANGE_ENABLED))
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_ReadRangeDidDataLength(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pDidSize,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    const Dcm_DspDidRangeType* pDcmDspDidRange = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidRange;
    const uint8* pRangeDidCfgIndex = Dcm_0x22Type->pRangeDidCfgIndex;

    if (NULL_PTR == pDcmDspDidRange[*pRangeDidCfgIndex].DcmDspDidRangeReadDataLengthFnc)
    {
        /*DcmDspDidReadDataLengthFnc is NULL,send NRC 0x22*/
        *pNrc = DCM_E_CONDITIONSNOTCORRECT;
        ret = E_NOT_OK;
    }
    else
    {
        ret = pDcmDspDidRange[*pRangeDidCfgIndex].DcmDspDidRangeReadDataLengthFnc(
            Dcm_0x22Type->RecDid,
            OpStatus,
            pDidSize);
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Response >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UdsAssembleResponse(
    Dcm_OpStatusType OpStatus,
    uint16 DidCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DCM_VAR) ResOffset,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint32 DidSignalPos;
    uint16 DidSignalIndex;
    uint16 DidSignalNum;
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_OK;
    uint16 Size;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidSignalType* pDcmDspDidSignal = pDcmDspDid[DidCfgIndex].pDcmDspDidSignal;

    DidSignalNum = pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum;
    for (DidSignalIndex = 0; (DidSignalIndex < DidSignalNum) && (E_OK == ret); DidSignalIndex++)
    {
        pDspDidData = pDcmDspDidSignal[DidSignalIndex].pDcmDspDidData;
        Size = 0;
#if (STD_ON == DCM_DATA_TYPE_UINT8_DYN_ENABLED)
        if (DCM_UINT8_DYN == pDspDidData->DcmDspDataType)
        {
            /*DidDataSize is dynamic*/
            if (NULL_PTR == pDspDidData->DcmDspDataReadDataLengthFnc)
            {
                /*DcmDspDidReadDataLengthFnc is NULL,send NRC 0x22*/
                *pNrc = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
            else
            {
                ret = pDspDidData->DcmDspDataReadDataLengthFnc(OpStatus, &Size);
            }
        }
        else
#endif
        {
            /*DidDataSize is fixed*/
            Size = pDspDidData->DcmDspDataSize;
        }
        if (E_OK == ret)
        {
            uint32 pos = (uint32)pDcmDspDidSignal[DidSignalIndex].DcmDspDidDataPos;
            DidSignalPos = *ResOffset + (uint32)pos;
            if ((NULL_PTR == pDspDidData->DcmDspDataReadFnc) || ((DidSignalPos + (uint32)Size) > DCM_CHANNEL_LENGTH))
            {
                /*DcmDspDataReadFnc is NULL,send NRC 0x22*/
                *pNrc = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
            else
            {
                ret = pDspDidData->DcmDspDataReadFnc(OpStatus, &Dcm_Channel[DidSignalPos], pNrc);
            }
            /*ResOffset equal to last position add last size of the signal of did*/
            *ResOffset = DidSignalPos + (uint32)Size;
        }
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Response >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if ((STD_ON == DCM_DSP_DID_FUNC_ENABLED) && (STD_ON == DCM_DID_RANGE_ENABLED))
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UdsAssembleRangeDidResponse(
    Dcm_OpStatusType OpStatus,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    const Dcm_DspDidRangeType* pDcmDspDidRange = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidRange;
    const uint8* pRangeDidCfgIndex = Dcm_0x22Type->pRangeDidCfgIndex;

    if (NULL_PTR == pDcmDspDidRange[*pRangeDidCfgIndex].DcmDspDidRangeReadDidFnc)
    {
        *pNrc = DCM_E_CONDITIONSNOTCORRECT;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        ret = pDcmDspDidRange[*pRangeDidCfgIndex].DcmDspDidRangeReadDidFnc(
            Dcm_0x22Type->RecDid,
            &Dcm_Channel[*Dcm_0x22Type->ResOffset],
            OpStatus,
            Dcm_0x22Type->DidSize,
            pNrc);
        if (E_OK == ret)
        {
            (*Dcm_0x22Type->ResOffset) = (*Dcm_0x22Type->ResOffset) + Dcm_0x22Type->DidSize;
        }
    }
    return ret;
}
#endif
/***************************/
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Obd Did Deal >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DspInternalUDS0x22_ObdDidDealF4(uint8 ProtocolCtrlId, P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint32 SupportBuffer = 0;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    uint32 Offset;
    const Dcm_DslBufferType* pDcmChannelCfg;
#endif
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 MsgCtrlId;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
#endif

    /* Service 01 */
    switch (Dcm_0x22Type->RecDid)
    {
    case 0xF400u:
    case 0xF420u:
    case 0xF440u:
    case 0xF460u:
    case 0xF480u:
    case 0xF4A0u:
    case 0xF4C0u:
    case 0xF4E0u:
        if ((*Dcm_0x22Type->MixPid == NORMAL_REQUEST) || (*Dcm_0x22Type->MixPid == NEED_CALL_UDS_API))
        {
            ret = E_NOT_OK;
        }
        else
        {
            if (pDcmDspCfg->DcmDspEnableObdMirror == TRUE)
            {
#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
                OBD_CheckSupportedPIDs((uint8)(Dcm_0x22Type->RecDid & 0xFFu), &SupportBuffer, 0x01);
#endif
                if (SupportBuffer != 0UL)
                {
                    Dcm_Channel[*Dcm_0x22Type->ResOffset] = (uint8)((Dcm_0x22Type->RecDid) >> 8u); /*echo of DID MSB*/
                    (*Dcm_0x22Type->ResOffset) += 1u;
                    Dcm_Channel[*Dcm_0x22Type->ResOffset] = (uint8)(Dcm_0x22Type->RecDid);
                    (*Dcm_0x22Type->ResOffset) += 1u;
                    Dcm_FillTo4bytes(&(Dcm_Channel[*Dcm_0x22Type->ResOffset]), &SupportBuffer);
                    (*Dcm_0x22Type->ResOffset) += 4u;
                }
                else
                {
                    *(Dcm_0x22Type->noFindPidNum) += 1u;
                }
            }
            else
            {
                if (E_OK != DspInternalUDS0x22_CheckSupported(Dcm_0x22Type->RecDid, Dcm_0x22Type->ResOffset))
                {
                    *(Dcm_0x22Type->noFindPidNum) += 1u;
                }
            }
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
            if ((*Dcm_0x22Type->ResOffset - Offset) > pDcmChannelCfg->Dcm_DslBufferSize)
            {
                Dcm_PageBufferData.LastFilled = FALSE;
            }
#endif
            *Dcm_0x22Type->MixPid = SUPPORT_REQUEST;
        }
        break;
    default:
        if (*Dcm_0x22Type->MixPid == SUPPORT_REQUEST)
        {
            ret = E_NOT_OK;
        }
        else
        {
            *Dcm_0x22Type->MixPid = NEED_CALL_UDS_API;
        }
        break;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Obd Did Deal >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    DspInternalUDS0x22_ObdDidDealF8(uint8 ProtocolCtrlId, P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint32 SupportBuffer = 0;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    uint32 Offset;
    const Dcm_DslBufferType* pDcmChannelCfg;
#endif
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 MsgCtrlId;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    uint32* ResOffset = Dcm_0x22Type->ResOffset;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
#endif

    /* Service 01 */
    switch (Dcm_0x22Type->RecDid)
    {
    case 0xF800u:
    case 0xF820u:
    case 0xF840u:
    case 0xF860u:
    case 0xF880u:
    case 0xF8A0u:
    case 0xF8C0u:
    case 0xF8E0u:
        if ((*Dcm_0x22Type->MixPid == NORMAL_REQUEST) || (*Dcm_0x22Type->MixPid == NEED_CALL_UDS_API))
        {
            ret = E_NOT_OK;
        }
        else
        {
            if (pDcmDspCfg->DcmDspEnableObdMirror == TRUE)
            {
#if (DCM_DSP_VEHINFO_FUNC_ENABLED == STD_ON)
                OBD_SetAvailabilityInfoTypeValue((uint8)(Dcm_0x22Type->RecDid & 0xFFu), &SupportBuffer);
#endif
                if (SupportBuffer != 0UL)
                {
                    Dcm_Channel[*ResOffset] = (uint8)((Dcm_0x22Type->RecDid) >> 8u); /*echo of DID MSB*/
                    (*ResOffset) += 1u;
                    Dcm_Channel[*ResOffset] = (uint8)(Dcm_0x22Type->RecDid);
                    (*ResOffset) += 1u;
                    Dcm_FillTo4bytes(&(Dcm_Channel[*ResOffset]), &SupportBuffer);
                    (*ResOffset) += 4u;
                }
                else
                {
                    *(Dcm_0x22Type->noFindPidNum) += 1u;
                }
            }
            else
            {
                if (E_OK != DspInternalUDS0x22_CheckSupported(Dcm_0x22Type->RecDid, ResOffset))
                {
                    *(Dcm_0x22Type->noFindPidNum) += 1u;
                }
            }
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
            if ((*ResOffset - Offset) > pDcmChannelCfg->Dcm_DslBufferSize)
            {
                Dcm_PageBufferData.LastFilled = FALSE;
            }
#endif
            *Dcm_0x22Type->MixPid = SUPPORT_REQUEST;
        }
        break;
    default:
        if (*Dcm_0x22Type->MixPid == SUPPORT_REQUEST)
        {
            ret = E_NOT_OK;
        }
        else
        {
            *Dcm_0x22Type->MixPid = NEED_CALL_UDS_API;
        }
        break;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Obd Did Deal >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_ObdDidDealF6(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint32 SupportBuffer = 0;
#ifdef DEM_OBD_SUPPORT
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    uint8 TIDvalue;
    uint8 UaSID;
    uint8 Mid = (uint8)(Dcm_0x22Type->RecDid & 0xFFu);
    uint8 TidNum = 0;
    uint8 iloop = 0;
    uint16 Testvalue;
    uint16 Lowlimvalue;
    uint16 Upplimvalue;
#endif
#endif
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    uint32 Offset;
    const Dcm_DslBufferType* pDcmChannelCfg;
#endif
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 MsgCtrlId;
    uint16 DidNum;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    uint32* ResOffset = Dcm_0x22Type->ResOffset;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
#endif
    /*calculate the number of required DID*/
    DidNum = (uint8)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen >> 1u);

    /* Service 01 */
    switch (Dcm_0x22Type->RecDid)
    {
    case 0xF600u:
    case 0xF620u:
    case 0xF640u:
    case 0xF660u:
    case 0xF680u:
    case 0xF6A0u:
    case 0xF6C0u:
    case 0xF6E0u:
        if ((*Dcm_0x22Type->MixPid == NORMAL_REQUEST) || (*Dcm_0x22Type->MixPid == NEED_CALL_UDS_API))
        {
            ret = E_NOT_OK;
        }
        else
        {
#ifdef DEM_OBD_SUPPORT
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
            (void)Dem_DcmGetAvailableOBDMIDs(Mid, &SupportBuffer);
#endif
#endif
            if (SupportBuffer != 0u)
            {
                Dcm_Channel[*ResOffset] = (uint8)((Dcm_0x22Type->RecDid) >> 8u);
                /*echo of DID MSB*/
                *ResOffset += 1u;
                Dcm_Channel[*ResOffset] = (uint8)(Dcm_0x22Type->RecDid);
                *ResOffset += 1u;
                Dcm_FillTo4bytes(&(Dcm_Channel[*ResOffset]), &SupportBuffer);
                (*ResOffset) += 4u;
            }
            else
            {
                *(Dcm_0x22Type->noFindPidNum) += 1u;
            }
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
            if ((*ResOffset - Offset) > pDcmChannelCfg->Dcm_DslBufferSize)
            {
                Dcm_PageBufferData.LastFilled = FALSE;
            }
#endif
            *Dcm_0x22Type->MixPid = SUPPORT_REQUEST;
        }
        break;
    default:
        if ((*Dcm_0x22Type->MixPid == SUPPORT_REQUEST) || (DidNum > 1u))
        {
            ret = E_NOT_OK;
        }
        else
        {
            if (pDcmDspCfg->DcmDspEnableObdMirror == TRUE)
            {
#ifdef DEM_OBD_SUPPORT
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
                ret = Dem_DcmGetNumTIDsOfOBDMID(Mid, &TidNum);
                if ((TidNum != 0u) && (ret == E_OK))
                {
                    for (; iloop < TidNum; iloop++)
                    {
                        ret = Dem_DcmGetDTRData(Mid, iloop, &TIDvalue, &UaSID, &Testvalue, &Lowlimvalue, &Upplimvalue);
                        if (ret == E_OK)
                        {
                            Dcm_Channel[*ResOffset] = (uint8)((Dcm_0x22Type->RecDid) >> 8u);
                            /*echo of DID MSB*/
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = (uint8)(Dcm_0x22Type->RecDid);
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = TIDvalue;
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = UaSID;
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = (uint8)((Testvalue & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = (uint8)(Testvalue & OBD_DATA_LSB_MASK);
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = (uint8)((Lowlimvalue & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = (uint8)(Lowlimvalue & OBD_DATA_LSB_MASK);
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = (uint8)((Upplimvalue & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            (*ResOffset) += 1u;
                            Dcm_Channel[*ResOffset] = (uint8)(Upplimvalue & OBD_DATA_LSB_MASK);
                            (*ResOffset) += 1u;
                        }
                    }
                }
                else
#endif
#endif
                {
                    /* there is no supported TID,send NRC 0x31 */
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                    ret = E_NOT_OK;
                }
                *Dcm_0x22Type->MixPid = NORMAL_REQUEST;
            }
            else
            {
                *Dcm_0x22Type->MixPid = NEED_CALL_UDS_API;
            }
        }
        break;
    }
    return ret;
}
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Obd Did Deal >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_ObdDidDeal(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;

    if ((Dcm_0x22Type->RecDid >= 0xF400u) && (Dcm_0x22Type->RecDid <= 0xF4FFu))
    {
        ret = DspInternalUDS0x22_ObdDidDealF4(ProtocolCtrlId, Dcm_0x22Type);
    }
    else if ((Dcm_0x22Type->RecDid >= 0xF800u) && (Dcm_0x22Type->RecDid <= 0xF8FFu))
    {
        ret = DspInternalUDS0x22_ObdDidDealF8(ProtocolCtrlId, Dcm_0x22Type);
    }
    else if ((Dcm_0x22Type->RecDid >= 0xF600u) && (Dcm_0x22Type->RecDid <= 0xF6FFu))
    {
        ret = DspInternalUDS0x22_ObdDidDealF6(ProtocolCtrlId, Dcm_0x22Type, ErrorCode);
    }
    else
    {
        /*idle*/
    }
    return ret;
}
#endif
#endif
/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  none Obd Did Deal >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_NonObdDidNOrangeDeal(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    uint8 TxChannelCfgIndex = Dcm_ChannelCtrl[Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex;
    const Dcm_DslBufferType* pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    uint32 Offset = pDcmChannelCfg->offset;
#endif
#if (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED)
#endif

    ret = Dsp_UDS0x22_DidEcuSignalCheck(
#if ((NVM_ENABLE == STD_ON) && (DCM_DATA_USE_BLOCK_ID_ENABLED == STD_ON))
        OpStatus,
        MsgCtrlId,
        ErrorCode,
#endif
        Dcm_0x22Type);
    if (FALSE == *Dcm_0x22Type->readDidSignalFlag)
    {
        /*check the current DID condition is OK*/
        ret = DspInternalUDS0x22_DidConditionCheck(OpStatus, Dcm_0x22Type->DidCfgIndex, ErrorCode);
        if (DCM_E_PENDING == ret)
        {
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
        }
        else if (E_OK == ret)
        {
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
            const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
            const Dcm_DspDidInfoType* pDcmDspDidInfo =
                &pDcmDspCfg->pDcmDspDidInfo[pDcmDspCfg->pDcmDspDid[Dcm_0x22Type->DidCfgIndex].DcmDspDidInfoIndex];
            if ((pDcmDspDidInfo->DcmDspDidDynamicallyDefined == TRUE) && (pDcmDspDidInfo->DcmDspDDDIDMaxElements > 0u)
                && (Dcm_0x22Type->RecDid >= 0xF200u) && (Dcm_0x22Type->RecDid <= 0xF3FFu))
            {
                ret = Dcm_UdsAssembleResponse_0x2C(OpStatus, Dcm_0x22Type, ProtocolCtrlId, ErrorCode);
            }
            else
#endif
            {
                /*set the response message*/
                ret = Dcm_UdsAssembleResponse(OpStatus, Dcm_0x22Type->DidCfgIndex, Dcm_0x22Type->ResOffset, ErrorCode);
                if (DCM_E_PENDING == ret)
                {
                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                }
            }
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
            if ((E_OK == ret) && ((*Dcm_0x22Type->ResOffset - Offset) > pDcmChannelCfg->Dcm_DslBufferSize))
            {
                Dcm_PageBufferData.LastFilled = FALSE;
            }
#endif
        }
        else
        {
            /*idle*/
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  none Obd Did Deal >
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x22_NonObdDidDeal(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x22Types, AUTOMATIC, DCM_VAR) Dcm_0x22Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    uint16 DidInfoCfgIndex;
#if ((STD_ON == DCM_DID_RANGE_ENABLED) || (TRUE == DCM_PAGEDBUFFER_ENABLED))
    uint16 DidSize = 0;
#endif
#if ((DCM_DID_RANGE_ENABLED == STD_ON) || (TRUE == DCM_PAGEDBUFFER_ENABLED))
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
#endif
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    uint8 TxChannelCfgIndex = Dcm_ChannelCtrl[Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex;
    const Dcm_DslBufferType* pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    uint32 Offset = pDcmChannelCfg->offset;
#endif
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidRangeType* pDcmDspDidRange = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidRange;

    /*find the required DID in configuration*/
    ret = DspInternalUDS0x22_DidCheck(OpStatus, Dcm_0x22Type);
    if (E_NOT_OK == ret)
    {
        (*Dcm_0x22Type->NoFindDidNum) += 1u;
        ret = E_OK;
    }
    else
    {
        DidInfoCfgIndex = (TRUE == *Dcm_0x22Type->pRangeDidFlag)
                              ? pDcmDspDidRange[*Dcm_0x22Type->pRangeDidCfgIndex].DcmDspDidRangeInfoIndex
                              : pDcmDspDid[*Dcm_0x22Type->pDidCfgIndex].DcmDspDidInfoIndex;
        /*if the required DID is found,check whether the pDcmDspDidRead container is configured*/
        ret = DspInternalUDS0x22_DidInfoCheck(DidInfoCfgIndex);
        if (E_NOT_OK == ret)
        {
            (*Dcm_0x22Type->NoFindDidReadNum) += 1u;
            ret = E_OK;
        }
        else
        {
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
            /*check the current session*/
            ret = DspInternalUDS0x22_DidSessionCheck(DidInfoCfgIndex, Dcm_0x22Type->DidSessionSupportNum, ErrorCode);
            if (E_NOT_OK == ret)
            {
                ret = E_OK;
            }
            else
#endif
            {
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
                ret = DspInternal_DidAuthenticationCheck(
                    ProtocolCtrlId,
                    Dcm_0x22Type->RecDid,
                    DidInfoCfgIndex,
                    ErrorCode);
                if (E_OK == ret)
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
                {
                    /*check the current security level*/
                    ret = DsdInternal_DidSecurityCheck(DidInfoCfgIndex, ErrorCode);
                    if (E_OK == ret)
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED */
                    {
                        /*set the response message*/
                        Dcm_Channel[*Dcm_0x22Type->ResOffset] = (uint8)((Dcm_0x22Type->RecDid) >> 8u);
                        /*echo of DID MSB*/
                        (*Dcm_0x22Type->ResOffset) += 1u;
                        Dcm_Channel[*Dcm_0x22Type->ResOffset] = (uint8)(Dcm_0x22Type->RecDid);
                        (*Dcm_0x22Type->ResOffset) += 1u;
#if (DCM_DID_RANGE_ENABLED == STD_ON)
                        if (FALSE == *Dcm_0x22Type->pRangeDidFlag)
#endif
                        {
                            Dcm_0x22Type->DidCfgIndex = *Dcm_0x22Type->pDidCfgIndex;
                            ret = DspInternalUDS0x22_NonObdDidNOrangeDeal(
                                OpStatus,
                                ProtocolCtrlId,
                                Dcm_0x22Type,
                                ErrorCode);
                        }
#if (STD_ON == DCM_DID_RANGE_ENABLED)
                        else
                        {
                            /*Read the current length of the data in DID*/
                            ret =
                                DspInternalUDS0x22_ReadRangeDidDataLength(OpStatus, Dcm_0x22Type, &DidSize, ErrorCode);
                            if (DCM_E_PENDING == ret)
                            {
                                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                            }
                            else if (
                                (E_OK == ret)
                                && (DidSize
                                    > pDcmDspDidRange[*Dcm_0x22Type->pRangeDidCfgIndex].DcmDspDidRangeMaxDataLength))
                            {
                                /*DidSize over the DcmDspDidRangeMaxDataLength,Send NRC 0x22*/
                                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                                ret = E_NOT_OK;
                            }
                            else
                            {
                                /*idle*/
                            }
                            if (E_OK == ret)
                            {
                                Dcm_0x22Type->DidSize = DidSize;
                                /*set the response message*/
                                ret = Dcm_UdsAssembleRangeDidResponse(OpStatus, Dcm_0x22Type, ErrorCode);
                                if (DCM_E_PENDING == ret)
                                {
                                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                                }
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                                if (((*Dcm_0x22Type->ResOffset) - Offset) > pDcmChannelCfg->Dcm_DslBufferSize)
                                {
                                    Dcm_PageBufferData.LastFilled = FALSE;
                                }
#endif
                            }
                        }
#endif /* STD_ON == DCM_DID_RANGE_ENABLED */
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                        Dcm_PageBufferData.LastFilledSize = DidSize;
#endif
                    }
                }
            }
        }
    }
    return ret;
}
#endif

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service  Check Tx Length>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x22_CheckTxLength(
    uint8 ProtocolCtrlId,
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    uint16 Index0,
    uint32 ReqOffset,
#endif
    uint32 ResOffset)
{
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint32 Offset;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
#endif
    uint8 MsgCtrlId;
    Std_ReturnType ret = E_OK;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    const Dcm_DslCfgType* pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    const Dcm_DslBufferType* pDcmChannelCfg = &pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow = &pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCtrlId];
#endif
    Offset = pDcmChannelCfg->offset;
    /* check tx data length */
    if
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        (
#endif
            ((ResOffset - Offset) > pDcmChannelCfg->Dcm_DslBufferSize)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            || (0UL != Dcm_PageBufferData.TotalSize))
    {
        if (0UL == Dcm_PageBufferData.TotalSize)
        {
            Dcm_PageBufferData.TotalSize = ResOffset - Offset;
            DsdInternal_StartPagedProcessing(ProtocolCtrlId);
        }
        else
        {
            if (Dcm_PageBufferData.LastFilled == TRUE)
            {
                SchM_Enter_Dcm_ExclusiveArea();
                Dcm_PageBufferData.IloopOne = Index0;
                Dcm_PageBufferData.ReqOffset = ReqOffset;
                Dcm_PageBufferData.ThisPageSize = ResOffset - Offset;
                SchM_Exit_Dcm_ExclusiveArea();
            }
            else
            {
                SchM_Enter_Dcm_ExclusiveArea();
                Dcm_PageBufferData.IloopOne = Index0 - 1u;
                Dcm_PageBufferData.ReqOffset = ReqOffset - 2u;
                Dcm_PageBufferData.ThisPageSize = ResOffset - Offset - Dcm_PageBufferData.LastFilledSize;
                SchM_Exit_Dcm_ExclusiveArea();
            }
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_PageBufferData.PageIndex += 1u;
            Dcm_PageBufferData.Filled = TRUE;
            SchM_Exit_Dcm_ExclusiveArea();

            DsdInternal_ProcessPage(ProtocolCtrlId);
            if (Dcm_PageBufferData.TimeOut == TRUE)
            {
                ret = E_NOT_OK;
            }
            else
            {
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                ret = DCM_E_PENDING;
            }
        }
        if (ret == E_OK)
        {
            if (Dcm_PageBufferData.TotalSize > pDcmDslProtocolRow->DcmDslProtocolMaximumResponseSize)
            {
                DslInternal_InitPageBuffer();
                ret = E_NOT_OK;
            }
            else
            {
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                ret = DCM_E_PENDING;
            }
        }
    }
#else
    {
        /*Pdu length is bigger than buffer size */
        ret = E_NOT_OK;
    }
#endif
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x22 service>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x22(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint16 RecDid;
    uint32 ResOffset;
    uint32 Offset;
    uint32 ReqOffset;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint16 DidNum;
    uint16 Index0;
    uint8 DidSessionSupportNum = 0;
    uint8 NoFindDidNum = 0;
    uint8 NoFindDidReadNum = 0;
    uint8 RangeDidCfgIndex;
    boolean RangeDidFlag;
    uint16 DidCfgIndex;
    boolean readDidSignalFlag;
    Dcm_MsgLenType ReqDataLen;
#endif
    uint8 MsgCtrlId;
    uint8 MixPid = NONE_PID;
    uint8 noFindPidNum = 0;
    Std_ReturnType ret;
    Dcm_0x22Types Dcm_0x22Type;
    Dcm_MsgContextType* pMsgContext;

#if (STD_OFF == DCM_DSP_DID_FUNC_ENABLED)
    /*NRC 0x31:request out of range*/
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
    ret = E_NOT_OK;
#else
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
    ReqDataLen = Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen;
    /*calculate the number of required DID*/
    DidNum = (uint16)(ReqDataLen >> 1u);
    /*check the message min and max length*/
    ret = DspInternalUDS0x22_DidNumbercheck(DidNum, ReqDataLen);
    if (E_NOT_OK == ret)
    {
        /*NRC = 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    else
    {
        /*get the runtime datum index*/
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
        if (0u == Dcm_PageBufferData.PageIndex)
        {
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_PageBufferData.PageTxOK = FALSE;
#endif
            Dcm_Channel[Offset] = 0x62; /*response SID*/
            ResOffset = Offset + 1u;
            ReqOffset = 1;
            Index0 = 0u;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
            Dcm_PageBufferData.LastFilled = TRUE;
            SchM_Exit_Dcm_ExclusiveArea();
        }
        else
        {
            if (Dcm_PageBufferData.PageTxOK != TRUE)
            {
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                ret = DCM_E_PENDING;
            }
            Index0 = Dcm_PageBufferData.IloopOne;
            ReqOffset = Dcm_PageBufferData.ReqOffset;
            ResOffset = Offset;
            SchM_Enter_Dcm_ExclusiveArea();
            Dcm_PageBufferData.PageTxOK = FALSE;
            Dcm_PageBufferData.LastFilled = TRUE;
            SchM_Exit_Dcm_ExclusiveArea();
        }
#endif
    }
    if (E_OK == ret)
    {
        for (; (Index0 < DidNum) && ((E_OK == ret) || (DCM_E_PENDING == ret))
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
               && ((Dcm_PageBufferData.LastFilled == TRUE) || (Dcm_PageBufferData.TotalSize == 0UL))
#endif
                 ;
             Index0++)
        {
            RecDid = (uint16)(((uint16)pMsgContext->pReqData[ReqOffset]) << 8u)
                     | ((uint16)(pMsgContext->pReqData[ReqOffset + 1u]));
            ReqOffset = ReqOffset + 2u;
#if (DCM_DSP_PID_FUNC_ENABLED == STD_ON)
            if ((RecDid >= 0xF400u) && (RecDid <= 0xF8FFu))
            {
                Dcm_0x22Type.RecDid = RecDid;
                Dcm_0x22Type.MixPid = &MixPid;
                Dcm_0x22Type.noFindPidNum = &noFindPidNum;
                Dcm_0x22Type.ResOffset = &ResOffset;
                DidSessionSupportNum++;
                ret = DspInternalUDS0x22_ObdDidDeal(ProtocolCtrlId, &Dcm_0x22Type, ErrorCode);
            }
#endif
            /* PRQA S 2995,2996,2991++ */ /* MISRA Rule 2.2,Rule 2.2,Rule 14.3 */
            if (((MixPid == NEED_CALL_UDS_API) || (MixPid == NONE_PID)) && ((E_OK == ret) || (DCM_E_PENDING == ret)))
            /* PRQA S 2995,2996,2991-- */ /* MISRA Rule 2.2,Rule 2.2,Rule 14.3 */
            {
                Dcm_0x22Type.RecDid = RecDid;
                Dcm_0x22Type.NoFindDidReadNum = &NoFindDidReadNum;
                Dcm_0x22Type.DidSessionSupportNum = &DidSessionSupportNum;
                Dcm_0x22Type.NoFindDidNum = &NoFindDidNum;
                Dcm_0x22Type.ResOffset = &ResOffset;
                Dcm_0x22Type.pRangeDidCfgIndex = &RangeDidCfgIndex;
                Dcm_0x22Type.pDidCfgIndex = &DidCfgIndex;
                Dcm_0x22Type.pRangeDidFlag = &RangeDidFlag;
                Dcm_0x22Type.readDidSignalFlag = &readDidSignalFlag;
                ret = DspInternalUDS0x22_NonObdDidDeal(OpStatus, ProtocolCtrlId, &Dcm_0x22Type, ErrorCode);
            }
            /*if the requested DID reading is not completed, the subsequent DID reading is terminated*/
            if ((OpStatus == DCM_PENDING) && (DCM_E_PENDING == ret))
            {
                break;
            }
        }
        /* PRQA S 2995,2996++ */ /* MISRA Rule 2.2 */
        if (((NoFindDidNum == DidNum) || (NoFindDidReadNum == DidNum) || (noFindPidNum == DidNum)
             || (DidSessionSupportNum == 0u))
            && (E_OK == ret))
        /* PRQA S 2995,2996-- */ /* MISRA Rule 2.2 */
        {
            /*if the required DID is not found in DcmDspDid,send NRC 0x31*/
            /*at least one DID is supported in the active session,not then send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }

    if ((Dcm_0x22DidReadNvmFlag == DCM_PENDING) || ((OpStatus != DCM_PENDING) && (Dcm_0x22DidReadNvmFlag == E_OK)))
    {
        if (ret != E_NOT_OK)
        {
            ret = DCM_E_PENDING;
        }
        Dcm_0x22DidReadNvmFlag = 0xFF;
    }

    if (E_OK == ret)
    {
        /* check tx data length */
        ret = Dcm_UDS0x22_CheckTxLength(
            ProtocolCtrlId,
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
            Index0,
            ReqOffset,
#endif
            ResOffset);
        if (E_NOT_OK == ret)
        {
            /*Pdu length is bigger than Page buffer max size */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
        }
        else
        {
            pMsgContext->ResMaxDataLen = (Dcm_MsgLenType)ResOffset - Offset;
            pMsgContext->ResDataLen = (Dcm_MsgLenType)ResOffset - Offset;
            pMsgContext->pResData = &Dcm_Channel[Offset];
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
#endif
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
