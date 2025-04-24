/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
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
         UDS:ReadDataByPeriodicIdentifier  (2A hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X2A_ENABLED)
typedef struct
{
    uint8* DidSupportNum;
    uint8* DidSessionSupportNum;
} Dcm_0x2ATypes;

typedef struct
{
    uint16 Did;
    uint16 Length;
    boolean DDDid;
} DidRxQueueTypes;

#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"
static VAR(SchedulerQueueTypes, DCM_VAR_NOINIT) SchedulerQueue[DCM_DSP_MAX_PERIODIC_DID_SCHEDULER];

static VAR(DidRxQueueTypes, DCM_VAR_NOINIT) DidRxQueue[DCM_DSP_MAX_PERIODIC_DID_TO_READ];
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_POWER_ON_INIT_16
#include "Dcm_MemMap.h"
static VAR(uint16, DCM_VAR_POWER_ON_INIT) SchedulerNum = 0U;

static VAR(uint16, DCM_VAR_POWER_ON_INIT) SchedulerIndex = 0U;

static VAR(uint16, DCM_VAR_POWER_ON_INIT) SchedulerPengingDid = 0u;
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_16
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_POWER_ON_INIT_8 /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#include "Dcm_MemMap.h"
static VAR(boolean, DCM_VAR_POWER_ON_INIT) SchedulerPengingDDDid = FALSE;
#define DCM_STOP_SEC_VAR_POWER_ON_INIT_8 /* PRQA S 0791 */ /* MISRA Rule 5.4 */
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
/*************************************************************************/
/*
 * Brief               <UDS  0x2A service DDDid Read Data Did>
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
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x2A_DDDid_Read_Data_Did(
    Dcm_OpStatusType OpStatus,
    P2CONST(Dcm_DDDidElementsDataTypes, TYPEDEF, DCM_CONST) DDDid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) Data)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 DidSignalIndex;
    uint16 DidSignalNum;
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_OK;
    uint8 NewData[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint16 Did;
    uint8 Pos;
    uint16 Index;
    uint16 pDidCfgIndex = 0u;
    uint16 DidSignalPos = 0u;
    uint16 Size = 0;
    Dcm_NegativeResponseCodeType ErrorCode;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidSignalType* pDcmDspDidSignal;

    Did = (uint16)(DDDid->Data & 0xFFFFUL);
    Pos = (uint8)((DDDid->Data & 0xFF0000UL) >> 16u);
    for (Index = 0; (Index < pDcmDspCfg->DcmDspDidNum) && (ret == E_OK); Index++)
    {
        /*single did check*/
        if ((Did == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
        {
            pDidCfgIndex = Index;
            break;
        }
    }
    pDcmDspDidSignal = pDcmDspDid[pDidCfgIndex].pDcmDspDidSignal;
    DidSignalNum = pDcmDspDid[pDidCfgIndex].DcmDspDidSignalNum;
    for (DidSignalIndex = 0; (DidSignalIndex < DidSignalNum) && (ret == E_OK); DidSignalIndex++)
    {
        pDspDidData = pDcmDspDidSignal[DidSignalIndex].pDcmDspDidData;
#if (STD_ON == DCM_DATA_TYPE_UINT8_DYN_ENABLED)
        if (DCM_UINT8_DYN == pDspDidData->DcmDspDataType)
        {
            /*DidDataSize is dynamic*/
            if (NULL_PTR == pDspDidData->DcmDspDataReadDataLengthFnc)
            {
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
            DidSignalPos += pDcmDspDidSignal[DidSignalIndex].DcmDspDidDataPos;
            if ((NULL_PTR == pDspDidData->DcmDspDataReadFnc) || (Size > DCM_TEMP_BUFFER_LENGTH))
            {
                ret = E_NOT_OK;
            }
            else
            {
                ret = pDspDidData->DcmDspDataReadFnc(OpStatus, &NewData[DidSignalPos], &ErrorCode);
            }
            DidSignalPos += Size;
        }
    }
    Dcm_MemoryCopy(&(NewData[Pos]), Data, DDDid->Size);
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x2A service DDDid Read Data Did>
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
    DspInternalUDS0x2A_DDDid_Read_Data(Dcm_OpStatusType OpStatus, uint16 SchedulerQueueIndex)
{
    Std_ReturnType ret = E_OK;
    const Dcm_DDDidElementsDataTypes* DDDid;
    uint8 DDDidNum;
    uint8 index;
    uint16 iloop;
    uint16 DDDidIndex;
    uint8 IdInfoIndex;
    uint8 RangeInfoIndex;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo;
    const Dcm_DspMemoryIdInfoType* pDcmDspMemoryIdInfo;
    uint8 MemoryIdentifier = 0u;
    boolean Find = FALSE;
    Dcm_NegativeResponseCodeType ErrorCode;
    uint16 RecDid = SchedulerQueue[SchedulerQueueIndex].Did;
    uint8* Data;
    const Dcm_DspMemoryType* pDcmDspMemory = DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory;

    for (iloop = 0; (iloop < DCM_DSP_DDDID_MAX_NUMBER) && (Find == FALSE); iloop++)
    {
        if (RecDid == Dcm_DDDid[iloop].DDDid)
        {
            Find = TRUE;
            DDDidIndex = iloop;
        }
    }
    if (Find == TRUE)
    {
        SchedulerQueue[SchedulerQueueIndex].Length = 0;
        DDDidNum = Dcm_DDDid[DDDidIndex].DDDIDNumOfElements;
        for (index = 0; index < DDDidNum; index++)
        {
            DDDid = &Dcm_DDDid[DDDidIndex].DcmDspAlternativeArgumentData[index];
            Data = &(SchedulerQueue[SchedulerQueueIndex].Data[SchedulerQueue[SchedulerQueueIndex].Length + 1u]);
            if (DDDid->Subfunction == DCM_UDS0X2C_01_DDBYDID)
            {
                ret = DspInternalUDS0x2A_DDDid_Read_Data_Did(OpStatus, DDDid, Data);
            }
            else if (DDDid->Subfunction == DCM_UDS0X2C_02_DDBYMEMORY)
            {
                for (IdInfoIndex = 0u; IdInfoIndex < pDcmDspMemory->DcmDspMemoryIdInfoNum; IdInfoIndex++)
                {
                    pDcmDspMemoryIdInfo = &(pDcmDspMemory->DcmDspMemoryIdInfo[IdInfoIndex]);
                    if (pDcmDspMemoryIdInfo != NULL_PTR)
                    {
                        for (RangeInfoIndex = 0; RangeInfoIndex < pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfoNum;
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
                }
                ret = Dcm_ReadMemory(OpStatus, MemoryIdentifier, DDDid->Data, (uint32)DDDid->Size, Data, &ErrorCode);
                if (DCM_READ_FAILED == ret)
                {
                    ret = E_NOT_OK;
                }
                else if ((DCM_READ_FORCE_RCRRP == ret) || (DCM_READ_PENDING == ret))
                {
                    ret = DCM_E_PENDING;
                }
                else
                {
                    /*idle*/
                }
            }
            else
            {
                ret = E_NOT_OK;
            }
            SchedulerQueue[SchedulerQueueIndex].Length += DDDid->Size;
        }
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */

/*************************************************************************/
/*
 * Brief               <UDS  0x2A service DDDid Read Data Did>
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
static FUNC(Std_ReturnType, DCM_CODE) DspInternalUDS0x2A_Did_Read_Data(uint16 SchedulerQueueIndex)
{
    uint16 DidSignalPos = 0;
    uint16 DidSignalIndex;
    uint16 DidSignalNum;
    const Dcm_DspDataType* pDspDidData;
    Std_ReturnType ret = E_NOT_OK;
    uint16 Size;
    uint16 Index;
    boolean Flag = FALSE;
    uint16 DidCfgIndex = 0;
    Dcm_NegativeResponseCodeType ErrorCode;
    uint16 RecDid = SchedulerQueue[SchedulerQueueIndex].Did;
    uint8* Data = &(SchedulerQueue[SchedulerQueueIndex].Data[1]);
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidSignalType* pDcmDspDidSignal;

    for (Index = 0; (Index < pDcmDspCfg->DcmDspDidNum) && (FALSE == Flag); Index++)
    {
        /*single did check*/
        if ((RecDid == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
        {
            Flag = TRUE;
            DidCfgIndex = Index;
            ret = E_OK;
        }
    }

    if (ret == E_OK)
    {
        SchedulerQueue[SchedulerQueueIndex].Length = 0;
        DidSignalNum = pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum;
        for (DidSignalIndex = 0; DidSignalIndex < DidSignalNum; DidSignalIndex++)
        {
            pDcmDspDidSignal = &pDcmDspDid[DidCfgIndex].pDcmDspDidSignal[DidSignalIndex];
            pDspDidData = pDcmDspDidSignal->pDcmDspDidData;
            Size = 0;
#if (STD_ON == DCM_DATA_TYPE_UINT8_DYN_ENABLED)
            if (DCM_UINT8_DYN == pDspDidData->DcmDspDataType)
            {
                /*DidDataSize is dynamic*/
                if (NULL_PTR == pDspDidData->DcmDspDataReadDataLengthFnc)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    ret = pDspDidData->DcmDspDataReadDataLengthFnc(DCM_INITIAL, &Size);
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
                DidSignalPos += pDcmDspDidSignal->DcmDspDidDataPos;
                if ((NULL_PTR == pDspDidData->DcmDspDataReadFnc) || (Size > DCM_TEMP_BUFFER_LENGTH))
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    ret = pDspDidData->DcmDspDataReadFnc(DCM_INITIAL, &Data[DidSignalPos], &ErrorCode);
                }
                /*ResOffset equal to last position add last size of the signal of did*/
                DidSignalPos += Size;
                SchedulerQueue[SchedulerQueueIndex].Length += Size;
            }
        }
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x2A service Init>
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
FUNC(void, DCM_CODE) Dcm_Scheduler_0x2A_Init(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    /* DcmDspMaxPeriodicDidScheduler range ：1-255 */
    uint8 Index;
    SchedulerQueueTypes* pSchedulerQueue;
    for (Index = 0; Index < DCM_DSP_MAX_PERIODIC_DID_SCHEDULER; Index++)
    {
        pSchedulerQueue = &SchedulerQueue[Index];
        pSchedulerQueue->Did = 0;
        pSchedulerQueue->Counter = 0;
        pSchedulerQueue->SubFunction = 0;
        pSchedulerQueue->Length = 0;
        pSchedulerQueue->ProtocolCtrlId = 0xFF;
        pSchedulerQueue->Data[0] = 0;
        pSchedulerQueue->Data[1] = 0;
        pSchedulerQueue->Data[2] = 0;
        pSchedulerQueue->Data[3] = 0;
        pSchedulerQueue->Data[4] = 0;
        pSchedulerQueue->Data[5] = 0;
        pSchedulerQueue->Data[6] = 0;
        pSchedulerQueue->Data[7] = 0;
        pSchedulerQueue->DDDid = FALSE;
    }
    SchM_Enter_Dcm_ExclusiveArea();
    for (Index = 0; Index < DCM_PERIODICCONNECTION_NUM; Index++)
    {
        Scheduler_0x2A_Transmit[Index].PduId = 0xFF;
        Scheduler_0x2A_Transmit[Index].Transmit = FALSE;
    }
    SchM_Exit_Dcm_ExclusiveArea();
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
static FUNC(void, DCM_CODE) Dcm_0x2A_SetCounter(uint16 iloop)
{
    const Dcm_DspPeriodicTransmissionTypes* Periodic = DcmPbCfgPtr->pDcmDspCfg->Dcm_DspPeriodicTransmission;
    switch (SchedulerQueue[iloop].SubFunction)
    {
    case DCM_UDS0X2A_01_SLOW:
        SchedulerQueue[iloop].Counter = (uint8)Periodic->DcmDspPeriodicTransmissionSlowRate;
        break;
    case DCM_UDS0X2A_02_MEDIUM:
        SchedulerQueue[iloop].Counter = (uint8)Periodic->DcmDspPeriodicTransmissionMediumRate;
        break;
    case DCM_UDS0X2A_03_FAST:
        SchedulerQueue[iloop].Counter = (uint8)Periodic->DcmDspPeriodicTransmissionFastRate;
        break;
    default:
        /*idle*/
        break;
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
static FUNC(boolean, DCM_CODE) Dcm_MainFunction_0x2A_SubDeal(uint16 iloop, uint16 index, uint8 ProtocolCtrlId)
{
    boolean Find = FALSE;
    uint16 cnt;
    PduInfoType PduInfo;
    Std_ReturnType ret;
    PduIdType TxPduID;
    Std_ReturnType result;
    const Dcm_DslMainConnectionType* pDcmDslMainConnection =
        DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCtrlId]
            .pDcmDslConnection->pDcmDslMainConnection;
    const Dcm_DslPeriodicConnectionType* pPeriodicConnection =
        &pDcmDslMainConnection->pDcmDslPeriodicTranmissionConRef->DcmDslPeriodicConnection[index];

    if (SchedulerQueue[iloop].Counter == 0u)
    {
        if (SchedulerQueue[iloop].DDDid == TRUE)
        {
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
            ret = DspInternalUDS0x2A_DDDid_Read_Data(DCM_INITIAL, iloop);
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
        }
        else
        {
            ret = DspInternalUDS0x2A_Did_Read_Data(iloop);
        }
        if (ret == E_OK)
        {
            SchedulerQueue[iloop].Data[0] = (uint8)(SchedulerQueue[iloop].Did);
            PduInfo.SduDataPtr = &(SchedulerQueue[iloop].Data[0]);
            PduInfo.SduLength = SchedulerQueue[iloop].Length + 1u;
            TxPduID = pPeriodicConnection->DcmDslTxPduRPduId;
#if (STD_ON == DCM_GENERIC_CONNECTION)
            if ((pPeriodicConnection->DcmDslMetaDataFlag & DCM_METADATA_LENGTH_MASK) > 0u)
            {
                PduInfo.MetaDataPtr = &(Dcm_Metadata[ProtocolCtrlId][0]);
#if (DCM_CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
                PduInfo.MetaDataPtr[0u] = (uint8)((SchedulerQueue[iloop].TA & 0xFF00u) >> 8u);
                PduInfo.MetaDataPtr[1u] = (uint8)(SchedulerQueue[iloop].TA & 0xFFu);
                PduInfo.MetaDataPtr[2u] = (uint8)((SchedulerQueue[iloop].SA & 0xFF00u) >> 8u);
                PduInfo.MetaDataPtr[3u] = (uint8)(SchedulerQueue[iloop].SA & 0xFFu);
#else
                PduInfo.MetaDataPtr[0u] = (uint8)(SchedulerQueue[iloop].TA & 0xFFu);
                PduInfo.MetaDataPtr[1u] = (uint8)((SchedulerQueue[iloop].TA & 0xFF00u) >> 8u);
                PduInfo.MetaDataPtr[2u] = (uint8)(SchedulerQueue[iloop].SA & 0xFFu);
                PduInfo.MetaDataPtr[3u] = (uint8)((SchedulerQueue[iloop].SA & 0xFF00u) >> 8u);
#endif
            }
#else  /* STD_ON == DCM_GENERIC_CONNECTION */
            PduInfo.MetaDataPtr = NULL_PTR;
#endif /* STD_ON == DCM_GENERIC_CONNECTION */
            for (cnt = 0u; cnt < DCM_PERIODICCONNECTION_NUM; cnt++)
            {
                if (Scheduler_0x2A_Transmit[cnt].PduId == pPeriodicConnection->DcmDslPeriodicTxConfirmationPduId)
                {
                    Scheduler_0x2A_Transmit[cnt].Transmit = TRUE;
                }
            }
            result = PduR_DcmTransmit(TxPduID, &PduInfo);
            if (result != E_OK)
            {
                for (cnt = 0; cnt < DCM_PERIODICCONNECTION_NUM; cnt++)
                {
                    if (Scheduler_0x2A_Transmit[cnt].PduId == pPeriodicConnection->DcmDslPeriodicTxConfirmationPduId)
                    {
                        Scheduler_0x2A_Transmit[cnt].Transmit = FALSE;
                    }
                }
            }
            Dcm_0x2A_SetCounter(iloop);
            Find = TRUE;
            SchedulerIndex = iloop;
            SchedulerPengingDid = 0;
            SchedulerPengingDDDid = FALSE;
        }
        else if (ret == E_NOT_OK)
        {
            SchedulerIndex = iloop;
            SchedulerPengingDDDid = FALSE;
            SchedulerPengingDid = 0;
        }
        else if (ret == DCM_E_PENDING)
        {
            SchedulerPengingDid = SchedulerQueue[iloop].Did;
            SchedulerPengingDDDid = SchedulerQueue[iloop].DDDid;
            Find = TRUE;
        }
        else
        {
            /*idle*/
        }
    }
    return Find;
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
FUNC(void, DCM_CODE) Dcm_MainFunction_Scheduler_0x2A(uint8 ProtocolCtrlId) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    boolean Find;
    boolean ProcessContinue = TRUE;
    uint16 iloop;
    uint16 index;
    uint16 cnt;
    uint8 TxPduIdNum;
    const Dcm_DslMainConnectionType* pDcmDslMainConnection;
    const Dcm_DslPeriodicTransmissionType* pPeriodicTranmissionConRef;

    for (iloop = 0u; iloop < SchedulerNum; iloop++)
    {
        if ((SchedulerQueue[iloop].Counter != 0u) && (ProtocolCtrlId == SchedulerQueue[iloop].ProtocolCtrlId))
        {
            SchedulerQueue[iloop].Counter--;
        }
    }
    if ((SchedulerNum == 0U) && (SchedulerPengingDid != 0u) && (SchedulerPengingDDDid == TRUE))
    {
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
        (void)DspInternalUDS0x2A_DDDid_Read_Data(DCM_CANCEL, iloop);
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
        SchedulerPengingDid = 0;
        SchedulerPengingDDDid = FALSE;
    }
    pDcmDslMainConnection = DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCtrlId]
                                .pDcmDslConnection->pDcmDslMainConnection;
    if ((pDcmDslMainConnection == NULL_PTR) || (pDcmDslMainConnection->pDcmDslPeriodicTranmissionConRef == NULL_PTR))
    {
        ProcessContinue = FALSE;
    }
    if (ProcessContinue == TRUE)
    {
        for (index = 0u; index < DCM_MAINCONNECTION_NUM; index++)
        {
            if ((Dcm_CommCtrl[index].DcmDslProtocolComMChannelId == pDcmDslMainConnection->DcmDslProtocolComMChannelId)
                && (Dcm_CommCtrl[index].Dcm_CommState != DCM_COMM_FULL_COMMUNICATION))
            {
                ProcessContinue = FALSE;
            }
        }
    }
    if (ProcessContinue == TRUE)
    {
        pPeriodicTranmissionConRef = pDcmDslMainConnection->pDcmDslPeriodicTranmissionConRef;
        TxPduIdNum = pPeriodicTranmissionConRef->DcmPeriodicTxPduIdNum;
        for (index = 0; index < TxPduIdNum; index++)
        {
            for (cnt = 0u; cnt < DCM_PERIODICCONNECTION_NUM; cnt++)
            {
                if ((Scheduler_0x2A_Transmit[cnt].PduId
                     == pPeriodicTranmissionConRef->DcmDslPeriodicConnection[index].DcmDslPeriodicTxConfirmationPduId)
                    && (Scheduler_0x2A_Transmit[cnt].Transmit == TRUE))
                {
                    ProcessContinue = FALSE;
                }
            }
            if (ProcessContinue == TRUE)
            {
                Find = FALSE;
                for (iloop = SchedulerIndex; (iloop < SchedulerNum) && (Find == FALSE); iloop++)
                {
                    Find = Dcm_MainFunction_0x2A_SubDeal(iloop, index, ProtocolCtrlId);
                }
                if (SchedulerIndex >= (SchedulerNum - 1u))
                {
                    SchedulerIndex = 0u;
                }
            }
        }
    }
}

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
static Std_ReturnType DspInternalUDS0x2A_DidCheck(
    uint16 RecDid,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, DCM_VAR) pDidCfgIndex
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
    ,
    P2VAR(boolean, AUTOMATIC, DCM_VAR) DDDid,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) DDDidIndex
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
)
{
    uint16 Index;
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
    uint16 iloop;
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
    Std_ReturnType ret = E_NOT_OK;
    uint16 DidInfoIndex;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;

    if ((RecDid >= 0xF200u) && (RecDid <= 0xF2FFu))
    {
        /*find the corresponding DID in configuration*/
        for (Index = 0; (Index < pDcmDspCfg->DcmDspDidNum) && (E_NOT_OK == ret); Index++)
        {
            DidInfoIndex = pDcmDspDid[Index].DcmDspDidInfoIndex;
            /*single did check*/
            if ((RecDid == pDcmDspDid[Index].DcmDspDidId) && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
            {
                if ((pDcmDspDidInfo[DidInfoIndex].DcmDspDidDynamicallyDefined == TRUE)
                    && (pDcmDspDidInfo[DidInfoIndex].DcmDspDDDIDMaxElements > 0u))
                {

#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
                    for (iloop = 0; (iloop < DCM_DSP_DDDID_MAX_NUMBER) && (E_NOT_OK == ret); iloop++)
                    {
                        if (RecDid == Dcm_DDDid[iloop].DDDid)
                        {
                            *DDDid = TRUE;
                            *DDDidIndex = iloop;
                            *pDidCfgIndex = Index;
                            ret = E_OK;
                        }
                    }
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
                }
                else
                {
                    *pDidCfgIndex = Index;
                    ret = E_OK;
                }
            }
        }
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
static FUNC(void, DCM_CODE) DspInternalUDS0x2A_Clear_SchedulerQueue(uint16 Index)
{
    SchedulerQueueTypes* pSchedulerQueue = &SchedulerQueue[Index];
    pSchedulerQueue->Did = 0;
    pSchedulerQueue->Counter = 0;
    pSchedulerQueue->SubFunction = 0;
    pSchedulerQueue->Length = 0;
    pSchedulerQueue->ProtocolCtrlId = 0xFF;
    pSchedulerQueue->Data[0] = 0;
    pSchedulerQueue->Data[1] = 0;
    pSchedulerQueue->Data[2] = 0;
    pSchedulerQueue->Data[3] = 0;
    pSchedulerQueue->Data[4] = 0;
    pSchedulerQueue->Data[5] = 0;
    pSchedulerQueue->Data[6] = 0;
    pSchedulerQueue->Data[7] = 0;
    pSchedulerQueue->DDDid = FALSE;
#if (STD_ON == DCM_GENERIC_CONNECTION)
    SchedulerQueue[Index].TA = 0;
    SchedulerQueue[Index].SA = 0;
#endif /* STD_ON == DCM_GENERIC_CONNECTION */
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
static FUNC(void, DCM_CODE) DspInternalUDS0x2A_Move_SchedulerQueue(uint16 Index)
{
    uint16 Cnt;
    const SchedulerQueueTypes* pSchedulerQueue;
    for (Cnt = Index; (Cnt < (SchedulerNum - 1u)) && (SchedulerNum <= DCM_DSP_MAX_PERIODIC_DID_SCHEDULER); Cnt++)
    {
        pSchedulerQueue = &SchedulerQueue[Cnt + 1u];
        SchedulerQueue[Cnt].Did = pSchedulerQueue->Did;
        SchedulerQueue[Cnt].Counter = pSchedulerQueue->Counter;
        SchedulerQueue[Cnt].SubFunction = pSchedulerQueue->SubFunction;
        SchedulerQueue[Cnt].Length = pSchedulerQueue->Length;
        SchedulerQueue[Cnt].ProtocolCtrlId = pSchedulerQueue->ProtocolCtrlId;
        SchedulerQueue[Cnt].DDDid = pSchedulerQueue->DDDid;
#if (STD_ON == DCM_GENERIC_CONNECTION)
        SchedulerQueue[Cnt].TA = pSchedulerQueue->TA;
        SchedulerQueue[Cnt].SA = pSchedulerQueue->SA;
#endif /* STD_ON == DCM_GENERIC_CONNECTION */
    }
    DspInternalUDS0x2A_Clear_SchedulerQueue(Cnt);
    if (SchedulerIndex >= Index)
    {
        SchedulerIndex--;
    }
    SchedulerNum -= 1u;
}

#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
/*************************************************************************/
/*
 * Brief               <UDS  0x2A service DDDid check brfore clear used by 0x2C>
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
FUNC(void, DCM_CODE) Dcm_UDS0x2A_DDDidStop(uint16 iloop)
{
    uint16 index;

    /* DDDID reported periodically shall firstly be stopped and then cleared.*/
    for (index = 0u; index < SchedulerNum; index++)
    {
        if (SchedulerQueue[index].Did == Dcm_DDDid[iloop].DDDid)
        {
            DspInternalUDS0x2A_Move_SchedulerQueue(index);
            break;
        }
    }
}
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
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
FUNC(void, DCM_CODE) Dcm_UDS0x2ACheckNewSecurity(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    boolean SecSupport;
    boolean DDDidSupport = TRUE;
    uint16 Index;
    uint16 iloop;
    uint16 DidInfoCfgIndex;
    uint8 Idx;
    uint8 Num;
    const Dcm_DspDidReadType* pDspDidRead;
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
    Dcm_NegativeResponseCodeType ErrorCode;
    Std_ReturnType ret;
#endif /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED && DCM_DSP_DDDID_MAX_NUMBER > 0u */
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    uint16 DidNum = pDcmDspCfg->DcmDspDidNum;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;

    for (Index = 0; Index < SchedulerNum; Index++)
    {
        SecSupport = FALSE;
        for (iloop = 0; iloop < DidNum; iloop++)
        {
            if ((SchedulerQueue[Index].Did == pDcmDspDid[Index].DcmDspDidId)
                && (TRUE == pDcmDspDid[Index].DcmDspDidUsed))
            {
                DidInfoCfgIndex = pDcmDspDid[Index].DcmDspDidInfoIndex;
                pDspDidRead = pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidRead;
                if (NULL_PTR != pDspDidRead)
                {
                    Num = pDspDidRead->DcmDspDidReadSecurityLevelRefNum;
                    if (Num != 0u)
                    {
                        for (Idx = 0; (Idx < Num) && (FALSE == SecSupport); Idx++)
                        {
                            if (Dcm_MkCtrl.Dcm_ActiveSec == pDspDidRead->pDcmDspDidReadSecurityLevelRow[Idx])
                            {
                                SecSupport = TRUE;
                            }
                        }
                    }
                    else
                    {
                        SecSupport = TRUE;
                    }
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
                    if ((SchedulerQueue[Index].DDDid == TRUE) && (pDcmDspCfg->DcmDspDDDIDcheckPerSourceDID != NULL_PTR)
                        && (TRUE == (*(pDcmDspCfg->DcmDspDDDIDcheckPerSourceDID))))
                    {
                        for (Idx = 0; (Idx < DCM_DSP_DDDID_MAX_NUMBER); Idx++)
                        {
                            ret = DsdInternal_DDDIDcheckPerSourceDID(Idx, &ErrorCode);
                            if ((SchedulerQueue[Index].Did == Dcm_DDDid[Idx].DDDid) && (E_NOT_OK == ret))
                            {
                                DDDidSupport = FALSE;
                            }
                        }
                    }
#endif /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED && DCM_DSP_DDDID_MAX_NUMBER > 0u */
                }
            }
        }

        if ((SecSupport == FALSE) || (DDDidSupport == FALSE))
        {
            DspInternalUDS0x2A_Clear_SchedulerQueue(Index);
            DspInternalUDS0x2A_Move_SchedulerQueue(Index);
            Index--; /* PRQA S 2469 */ /* MISRA Rule 14.2 */
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
static FUNC(void, DCM_CODE) Dcm_UDS0x2ACheckNewSession_SubDeal(
    uint16 Index,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(boolean, AUTOMATIC, DCM_VAR) SesSupport,
    P2VAR(boolean, AUTOMATIC, DCM_VAR) SecSupport
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
    ,
    P2VAR(boolean, AUTOMATIC, DCM_VAR) DDDidSupport
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
#endif              /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED && DCM_DSP_DDDID_MAX_NUMBER > 0u */
)
{
    uint16 DidInfoCfgIndex;
    uint8 Idx;
    uint8 SesNum;
    uint8 SecNum;
    const Dcm_DspDidReadType* pDspDidRead;
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
    Dcm_NegativeResponseCodeType ErrorCode;
    Std_ReturnType ret;
#endif /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED && DCM_DSP_DDDID_MAX_NUMBER > 0u */
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = &DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid[Index];
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
    const boolean* pDcmDspDDDIDcheckPerSourceDID = pDcmDspCfg->DcmDspDDDIDcheckPerSourceDID;

    DidInfoCfgIndex = pDcmDspDid->DcmDspDidInfoIndex;
    pDspDidRead = pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidRead;
    SesNum = pDspDidRead->DcmDspDidReadSessionRefNum;
    if ((SchedulerQueue[Index].Did == pDcmDspDid->DcmDspDidId) && (TRUE == pDcmDspDid->DcmDspDidUsed)
        && (NULL_PTR != pDspDidRead))
    {
        if (SesNum != 0u)
        {
            for (Idx = 0; (Idx < SesNum) && (FALSE == *SesSupport); Idx++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSes == pDspDidRead->pDcmDspDidReadSessionRow[Idx])
                {
                    *SesSupport = TRUE;
                }
            }
        }
        else
        {
            *SesSupport = TRUE;
        }
        SecNum = pDspDidRead->DcmDspDidReadSecurityLevelRefNum;
        if (SecNum != 0u)
        {
            for (Idx = 0; (Idx < SecNum) && (FALSE == *SecSupport); Idx++)
            {
                if (Dcm_MkCtrl.Dcm_ActiveSec == pDspDidRead->pDcmDspDidReadSecurityLevelRow[Idx])
                {
                    *SecSupport = TRUE;
                }
            }
        }
        else
        {
            *SecSupport = TRUE;
        }
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
        if ((SchedulerQueue[Index].DDDid == TRUE) && (pDcmDspDDDIDcheckPerSourceDID != NULL_PTR)
            && (TRUE == (*(pDcmDspDDDIDcheckPerSourceDID))))
        {
            for (Idx = 0; (Idx < DCM_DSP_DDDID_MAX_NUMBER); Idx++)
            {
                ret = DsdInternal_DDDIDcheckPerSourceDID(Idx, &ErrorCode);
                if ((SchedulerQueue[Index].Did == Dcm_DDDid[Idx].DDDid) && (E_NOT_OK == ret))
                {
                    *DDDidSupport = FALSE;
                }
            }
        }
#endif /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED && DCM_DSP_DDDID_MAX_NUMBER > 0u */
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
FUNC(void, DCM_CODE) Dcm_UDS0x2ACheckNewSession(Dcm_SesCtrlType NewSes) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
    boolean SesSupport;
    boolean SecSupport;
    boolean DDDidSupport = TRUE;
    uint16 Index;
    uint16 iloop;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;

    if (DCM_DEFAULT_SESSION == NewSes)
    {
        for (Index = 0; Index < SchedulerNum; Index++)
        {
            if ((SchedulerPengingDid == SchedulerQueue[Index].Did) && (SchedulerPengingDDDid == TRUE))
            {
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
                (void)DspInternalUDS0x2A_DDDid_Read_Data(DCM_CANCEL, Index);
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
                SchedulerPengingDid = 0;
                SchedulerPengingDDDid = FALSE;
            }
            DspInternalUDS0x2A_Clear_SchedulerQueue(Index);
        }
    }
    else
    {
        for (Index = 0; Index < SchedulerNum; Index++)
        {
            SesSupport = FALSE;
            SecSupport = FALSE;
            for (iloop = 0; iloop < pDcmDspCfg->DcmDspDidNum; iloop++)
            {
                Dcm_UDS0x2ACheckNewSession_SubDeal(
                    Index,
                    &SesSupport,
                    &SecSupport
#if ((STD_ON == DCM_UDS_SERVICE0X2C_ENABLED) && (DCM_DSP_DDDID_MAX_NUMBER > 0u))
                    ,
                    &DDDidSupport
#endif /* STD_ON == DCM_UDS_SERVICE0X2C_ENABLED && DCM_DSP_DDDID_MAX_NUMBER > 0u */
                );
            }

            if ((SesSupport == FALSE) || (SecSupport == FALSE) || (DDDidSupport == FALSE))
            {
                if ((SchedulerPengingDid == SchedulerQueue[Index].Did) && (SchedulerPengingDDDid == TRUE))
                {
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
                    (void)DspInternalUDS0x2A_DDDid_Read_Data(DCM_CANCEL, iloop);
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
                    SchedulerPengingDid = 0;
                    SchedulerPengingDDDid = FALSE;
                }
                DspInternalUDS0x2A_Clear_SchedulerQueue(Index);
                DspInternalUDS0x2A_Move_SchedulerQueue(Index);
                Index--; /* PRQA S 2469 */ /* MISRA Rule 14.2 */
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2A_SubFunSubDeal(
    Dcm_OpStatusType OpStatus,
    uint8 DidSupportNum,
    uint16 DidCfgIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    const Dcm_DspDataType* pDspDidData;
    uint16 index;
#if (STD_ON == DCM_DATA_TYPE_UINT8_DYN_ENABLED)
    uint16 Size = 0;
#endif
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;

    /*call the ConditionCheckRead function*/
    for (index = 0; index < pDcmDspDid[DidCfgIndex].DcmDspDidSignalNum; index++)
    {
        pDspDidData = pDcmDspDid[DidCfgIndex].pDcmDspDidSignal[index].pDcmDspDidData;
        if ((pDspDidData->DcmConditionCheckReadFncUsed == TRUE)
            && (pDspDidData->DcmDspDataConditionCheckReadFnc != NULL_PTR))
        {
            ret = pDspDidData->DcmDspDataConditionCheckReadFnc(OpStatus, ErrorCode);
        }
        if (E_OK == ret)
        {
#if (STD_ON == DCM_DATA_TYPE_UINT8_DYN_ENABLED)
            if (pDspDidData->DcmDspDataType == DCM_UINT8_DYN)
            {
                /*DidDataSize is dynamic*/
                if (NULL_PTR == pDspDidData->DcmDspDataReadDataLengthFnc)
                {
                    /*DcmDspDidReadDataLengthFnc is NULL,send NRC 0x22*/
                    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                    ret = E_NOT_OK;
                }
                else
                {
                    ret = pDspDidData->DcmDspDataReadDataLengthFnc(OpStatus, &Size);
                }
                DidRxQueue[DidSupportNum].Length += Size;
            }
            else
#endif
            {
                DidRxQueue[DidSupportNum].Length = pDspDidData->DcmDspDataSize;
            }
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2A_SubFunDeal_0x01_0x03(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x2ATypes, AUTOMATIC, DCM_VAR) Dcm_0x2AType,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint16 iloop;
    uint8 index;
    uint8 cnt;
    uint16 DidNum = (uint16)(Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - 2u);
    uint16 RecDid;
    boolean Find;
    uint16 DidInfoCfgIndex;
    boolean DDDid = FALSE;
    uint16 DDDidIndex = 0;
    uint16 DidCfgIndex = 0;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;
    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;

    for (iloop = 0; iloop < DidNum; iloop++)
    {
        /*get the required DID from request message*/
        RecDid = (uint16)((uint16)(0xF2u) << 8u) | ((uint16)Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2u + iloop]);
        /*Repetition of the same periodicDataIdentifier in a single request message is not
         * allowed and the server shall ignore them all except one periodicDataIdentifer if
         * the client breaks this rule.*/
        Find = FALSE;
        for (index = 0; (index < DCM_DSP_MAX_PERIODIC_DID_TO_READ) && (Find == FALSE); index++)
        {
            if (DidRxQueue[index].Did == RecDid)
            {
                Find = TRUE;
                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                ret = E_NOT_OK;
            }
        }
        /* PRQA S 3415++ */ /* MISRA Rule 13.5 */
        if ((Find == FALSE)
            && (E_OK
                == DspInternalUDS0x2A_DidCheck(
                    RecDid,
                    &DidCfgIndex
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
                    ,
                    &DDDid,
                    &DDDidIndex
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
                    )))
        /* PRQA S 3415-- */ /* MISRA Rule 13.5 */
        {
            DidInfoCfgIndex = pDcmDspDid[DidCfgIndex].DcmDspDidInfoIndex;
            if (pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidRead != NULL_PTR)
            {
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
                ret = DspInternal_DidAuthenticationCheck(ProtocolCtrlId, RecDid, DidInfoCfgIndex, ErrorCode);
                if (E_NOT_OK == ret)
                {
                    break;
                }
#endif
#if ((STD_ON == DCM_SESSION_FUNC_ENABLED) && (STD_ON == DCM_DSP_DID_FUNC_ENABLED))
                ret = DsdInternal_DidSessionCheck(DidInfoCfgIndex, ErrorCode);
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED && STD_ON == DCM_DSP_DID_FUNC_ENABLED */
                if (E_OK == ret)
                {
                    (*Dcm_0x2AType->DidSessionSupportNum)++;
/*check the current security level*/
#if ((STD_ON == DCM_SECURITY_FUNC_ENABLED) && (STD_ON == DCM_DSP_DID_FUNC_ENABLED))
                    ret = DsdInternal_DidSecurityCheck(DidInfoCfgIndex, ErrorCode);
#endif /* STD_ON == DCM_SECURITY_FUNC_ENABLED && STD_ON == DCM_DSP_DID_FUNC_ENABLED */
                    if (E_OK == ret)
                    {
#if ((STD_ON == DCM_DSP_DID_FUNC_ENABLED) && (STD_ON == DCM_UDS_SERVICE0X2C_ENABLED))
                        if ((DDDid == TRUE) && (*(pDcmDspCfg->DcmDspDDDIDcheckPerSourceDID) == TRUE))
                        {
                            ret = DsdInternal_DDDIDcheckPerSourceDID(DDDidIndex, ErrorCode);
                        }
                        if ((DDDid == TRUE) && (E_OK == ret))
                        {
                            for (cnt = 0; cnt < Dcm_DDDid[DDDidIndex].DDDIDNumOfElements; cnt++)
                            {
                                DidRxQueue[*Dcm_0x2AType->DidSupportNum].Length +=
                                    Dcm_DDDid[DDDidIndex].DcmDspAlternativeArgumentData[cnt].Size;
                            }
                            DidRxQueue[*Dcm_0x2AType->DidSupportNum].DDDid = TRUE;
                        }
                        else
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED && STD_ON == DCM_UDS_SERVICE0X2C_ENABLED */
                            if (DDDid == FALSE)
                            {
                                ret = Dcm_UDS0x2A_SubFunSubDeal(
                                    OpStatus,
                                    *Dcm_0x2AType->DidSupportNum,
                                    DidCfgIndex,
                                    ErrorCode);
                            }
                            else
                            {
                                /*idle*/
                            }
                        if (DidRxQueue[*Dcm_0x2AType->DidSupportNum].Length > 7u)
                        {
                            /*can only transfer IF PDU*/
                            DidRxQueue[*Dcm_0x2AType->DidSupportNum].Length = 0;
                            DidRxQueue[*Dcm_0x2AType->DidSupportNum].DDDid = FALSE;
                        }
                        else
                        {
                            DidRxQueue[*Dcm_0x2AType->DidSupportNum].Did = RecDid;
                            (*Dcm_0x2AType->DidSupportNum) += 1u;
                        }
                    }
                }
            }
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x2A_SubFunDeal(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) DidSupportNum,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 DidSessionSupportNum = 0;
    uint16 DidInfoCfgIndex;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    uint8 Subfunction = pMsgContext->pReqData[1u];
    uint16 iloop;
    uint16 DidNum = (uint16)(pMsgContext->ReqDataLen - 2u);
    uint16 RecDid;
    uint8 index;
    Dcm_0x2ATypes Dcm_0x2AType;
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
    boolean DDDid = FALSE;
    uint16 DDDidIndex = 0;
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
    uint16 DidCfgIndex = 0;
    const Dcm_DspDidType* pDcmDspDid = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDid;

    switch (Subfunction)
    {
    case DCM_UDS0X2A_01_SLOW:
    case DCM_UDS0X2A_02_MEDIUM:
    case DCM_UDS0X2A_03_FAST:
        Dcm_0x2AType.DidSupportNum = DidSupportNum;
        Dcm_0x2AType.DidSessionSupportNum = &DidSessionSupportNum;
        ret = Dcm_UDS0x2A_SubFunDeal_0x01_0x03(OpStatus, ProtocolCtrlId, &Dcm_0x2AType, ErrorCode);
        break;
    case DCM_UDS0X2A_04_STOP:
        if (DidNum == 0u)
        {
            for (iloop = 0; iloop < DCM_DSP_MAX_PERIODIC_DID_SCHEDULER; iloop++)
            {
                DspInternalUDS0x2A_Clear_SchedulerQueue(iloop);
            }
            SchedulerNum = 0;
            SchedulerIndex = 0;
            DidSessionSupportNum++;
        }
        else
        {
            for (iloop = 0; iloop < DidNum; iloop++)
            {
                RecDid = (uint16)(((uint16)0xF2) << 8u) | (pMsgContext->pReqData[2u + iloop]);
                if (E_OK
                    == DspInternalUDS0x2A_DidCheck(
                        RecDid,
                        &DidCfgIndex
#if (DCM_DSP_DDDID_MAX_NUMBER > 0u)
                        ,
                        &DDDid,
                        &DDDidIndex
#endif /* DCM_DSP_DDDID_MAX_NUMBER > 0u */
                        ))
                {
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
                    const Dcm_DspDidInfoType* pDcmDspDidInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspDidInfo;
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
                    DidInfoCfgIndex = pDcmDspDid[DidCfgIndex].DcmDspDidInfoIndex;
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
                    if (pDcmDspDidInfo[DidInfoCfgIndex].pDcmDspDidRead != NULL_PTR)
                    {
                        ret = DspInternal_DidAuthenticationCheck(ProtocolCtrlId, RecDid, DidInfoCfgIndex, ErrorCode);
                        if (E_NOT_OK == ret)
                        {
                            break;
                        }
                    }
#endif /* STD_ON == DCM_UDS_SERVICE0X29_ENABLED */
#if ((STD_ON == DCM_SESSION_FUNC_ENABLED) && (STD_ON == DCM_DSP_DID_FUNC_ENABLED))
                    ret = DsdInternal_DidSessionCheck(DidInfoCfgIndex, ErrorCode);
#endif /* STD_ON == DCM_SESSION_FUNC_ENABLED && STD_ON == DCM_DSP_DID_FUNC_ENABLED */
                    if (E_OK == ret)
                    {
                        for (index = 0; index < SchedulerNum; index++)
                        {
                            if (SchedulerQueue[index].Did == RecDid)
                            {
                                DspInternalUDS0x2A_Move_SchedulerQueue(index);
                                DidSessionSupportNum++;
                            }
                        }
                    }
                }
            }
        }
        break;
    default:
        /*idle*/
        break;
    }

    if (DidSessionSupportNum == 0u)
    {
        /*at least one DID is supported in the active session,not then send NRC 0x31*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x2AServiceConditionCheck(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) DidSupportNum,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint8 iloop;
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    uint8 MsgCtrlId;
    uint8 Subfunction;
    const Dcm_DspCfgType* pDcmDspCfg = DcmPbCfgPtr->pDcmDspCfg;
    const Dcm_MsgContextType* pMsgContext;
    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    Subfunction = pMsgContext->pReqData[1u];

    if (((pMsgContext->ReqDataLen < DCM_UDS0X2A_SUBFUNC0X04_REQ_DATA_MINLENGTH)
         || ((pMsgContext->ReqDataLen < DCM_UDS0X2A_REQ_DATA_MINLENGTH)
             && ((DCM_UDS0X2A_01_SLOW == Subfunction) || (DCM_UDS0X2A_02_MEDIUM == Subfunction)
                 || (DCM_UDS0X2A_03_FAST == Subfunction)))
         || ((uint32)((DCM_DSP_MAX_PERIODIC_DID_TO_READ + 2u)) < pMsgContext->ReqDataLen)))
    {
        /*check the massage length*/ /* max PDid number : 0xFF */
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    if ((E_OK == ret) && ((Subfunction < DCM_UDS0X2A_01_SLOW) || (Subfunction > DCM_UDS0X2A_04_STOP)))
    {
        /*check transmissionMode*/
        /*if transmissionMode not supported,send NRC 0x31*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#if (STD_OFF == DCM_DSP_DID_FUNC_ENABLED)
    if (E_OK == ret)
    { /*NRC 0x31:request out of range*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
#else  /* STD_OFF == DCM_DSP_DID_FUNC_ENABLED */
    if (E_OK == ret)
    {
        for (iloop = 0; iloop < DCM_DSP_MAX_PERIODIC_DID_TO_READ; iloop++)
        {
            DidRxQueueTypes* pDidRxQueue = &DidRxQueue[iloop];
            pDidRxQueue->Did = 0;
            pDidRxQueue->Length = 0;
            pDidRxQueue->DDDid = FALSE;
        }
        ret = Dcm_UDS0x2A_SubFunDeal(OpStatus, ProtocolCtrlId, DidSupportNum, ErrorCode);
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
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x2A(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    uint32 Offset;
    uint8 iloop;
    uint8 index;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 DidSupportNum = 0;
    uint8 DidSupportNumStored = 0;
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 Subfunction;
    boolean Find;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;
    static VAR(boolean, DCM_VAR_POWER_ON_INIT) SchedulerIndexChanged = FALSE;
    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    Subfunction = pMsgContext->pReqData[1u];
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;

    ret = Dcm_Uds0x2AServiceConditionCheck(OpStatus, ProtocolCtrlId, &DidSupportNum, ErrorCode);
#if (STD_ON == DCM_DSP_DID_FUNC_ENABLED)
    if ((E_OK == ret) && (DCM_UDS0X2A_04_STOP != Subfunction))
    {
        for (index = 0; index < DidSupportNum; index++)
        {
            Find = FALSE;
            for (iloop = 0; (iloop < DCM_DSP_MAX_PERIODIC_DID_SCHEDULER) && (Find == FALSE); iloop++)
            {
                if (SchedulerQueue[iloop].Did == DidRxQueue[index].Did)
                {
                    Find = TRUE;
                    DidSupportNumStored += 1u;
                }
            }
        }
        if (((uint16)SchedulerNum + (uint16)DidSupportNum - (uint16)DidSupportNumStored)
            > (uint16)DCM_DSP_MAX_PERIODIC_DID_SCHEDULER)
        {
            /*scheduler free to store all supported pDIDs requested,not then send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
        for (index = 0; (index < DidSupportNum) && (E_OK == ret); index++)
        {
            Find = FALSE;
            for (iloop = 0; (iloop < DCM_DSP_MAX_PERIODIC_DID_SCHEDULER) && (Find == FALSE); iloop++)
            {
                if (SchedulerQueue[iloop].Did == DidRxQueue[index].Did)
                {
                    Find = TRUE;
                    SchedulerQueue[iloop].SubFunction = Subfunction;
                    SchedulerQueue[iloop].Length = DidRxQueue[index].Length + 1u;
                    Dcm_0x2A_SetCounter(iloop);
                    SchedulerQueue[iloop].ProtocolCtrlId = ProtocolCtrlId;
                    SchedulerQueue[iloop].DDDid = DidRxQueue[index].DDDid;
                }
            }
            if (Find == FALSE)
            {
                for (iloop = 0; (iloop < DCM_DSP_MAX_PERIODIC_DID_SCHEDULER) && (Find == FALSE); iloop++)
                {
                    if (SchedulerQueue[iloop].Did == 0u)
                    {
                        Find = TRUE;
                        SchedulerQueue[iloop].Did = DidRxQueue[index].Did;
                        SchedulerQueue[iloop].SubFunction = Subfunction;
                        SchedulerQueue[iloop].Length = DidRxQueue[index].Length + 1u;
                        Dcm_0x2A_SetCounter(iloop);
                        SchedulerQueue[iloop].ProtocolCtrlId = ProtocolCtrlId;
                        SchedulerQueue[iloop].DDDid = DidRxQueue[index].DDDid;
#if (STD_ON == DCM_GENERIC_CONNECTION)
                        SchedulerQueue[iloop].TA = Dcm_MsgCtrl[MsgCtrlId].Dcm_MetaData_TA;
                        SchedulerQueue[iloop].SA = Dcm_MsgCtrl[MsgCtrlId].Dcm_MetaData_SA;
#endif /* STD_ON == DCM_GENERIC_CONNECTION */
                        if (SchedulerIndexChanged == FALSE)
                        {
                            SchedulerIndexChanged = TRUE;
                            SchedulerIndex = iloop;
                        }
                        SchedulerNum += 1u;
                    }
                }
            }
        }
    }
    if ((E_OK == ret) && ((1u) > pDcmChannelCfg->Dcm_DslBufferSize))
    {
        /*assemble and send positive response*/
        /* check tx data length */
        /*Pdu length is bigger than buffer size,ignore the request message */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        SchedulerIndexChanged = FALSE;
        Dcm_Channel[Offset] = 0x6A;
        pMsgContext->ResMaxDataLen = 1u;
        pMsgContext->ResDataLen = 1u;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
#endif /* STD_ON == DCM_DSP_DID_FUNC_ENABLED */
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_UDS_SERVICE0X2A_ENABLED */
