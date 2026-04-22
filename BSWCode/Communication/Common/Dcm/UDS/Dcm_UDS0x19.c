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
#if (STD_ON == DCM_UDS_SERVICE0X19_ENABLED)
#include "Dem_Dcm.h"

/****************************************************************
         UDS:ReadDTCInformation (19 hex) service
 ***************************************************************/
typedef struct
{
    boolean*        PendingFlag;
    boolean*        NotAllowedFlag;
    Dcm_MsgLenType* MsgLen;
    uint16*         Index;
    uint32          Offset;
    uint16*         DtcCount;
} Dcm_UDS0x19Types;

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
#if (STD_ON == DCM_UDS_SERVICE0X19_01_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x01 sub service process>
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
    Dcm_UDS0x19_01(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8                                TxChannelCtrlIndex;
    uint8                                TxChannelCfgIndex;
    uint8                                DtcStatusMask;
    uint8                                MsgCtrlId;
    uint16                               DtcCount = 0;
    uint32                               Offset;
    boolean                              NotAllowedFlag = FALSE;
    boolean                              PendingFlag    = FALSE;
    Std_ReturnType                       ret            = E_OK;
    Dcm_MsgLenType                       MsgLen;
    Dem_ReturnSetFilterType              ReturnSetDTCFilter;
    Dem_ReturnGetNumberOfFilteredDTCType ReturnGetNumberOfFilteredDTC;
    Dcm_MsgContextType*                  pMsgContext;
    const Dcm_DslBufferType*             pDcmChannelCfg;

    MsgCtrlId   = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /**************************************************/
    /*check the massage length*/
    if (DCM_UDS0X19_SUBFUNC0X01_REQ_DATA_LENGTH != pMsgContext->ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret        = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /*find Tx buffer and offset*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg     = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset             = pDcmChannelCfg->offset;
        /*Sub-function:0x01*/
        MsgLen        = 2u;
        DtcStatusMask = pMsgContext->pReqData[2];
        /*obtain DTCStatusAvailabilityMask*/
        ret = Dem_DcmGetDTCStatusAvailabilityMask(&Dcm_Channel[Offset + MsgLen]);
        if (E_OK == ret)
        {
            /*one byte for the storage of DTC status availability mask*/
            MsgLen = MsgLen + 1u;
            /*get and store DTCFormatIdentifier*/
            Dcm_Channel[Offset + MsgLen] = Dem_DcmGetTranslationType();
            MsgLen                       = MsgLen + 1u;
            if (DtcStatusMask == 0u)
            {
                Dcm_Channel[Offset + MsgLen] = (uint8)0;
                MsgLen                       = MsgLen + 1u;
                Dcm_Channel[Offset + MsgLen] = (uint8)0;
                MsgLen                       = MsgLen + 1u;
            }
            else
            {
                /*set DTC filter*/
                ReturnSetDTCFilter = Dem_DcmSetDTCFilter(
                    DtcStatusMask,
                    DEM_DTC_KIND_ALL_DTCS,
                    DEM_DTC_FORMAT_UDS,
                    DEM_DTC_ORIGIN_PRIMARY_MEMORY,
                    FALSE,
                    DEM_SEVERITY_NO_SEVERITY,
                    FALSE);
                if (DEM_FILTER_ACCEPTED == ReturnSetDTCFilter)
                {
                    /*get the number of required DTC*/
                    ReturnGetNumberOfFilteredDTC = Dem_DcmGetNumberOfFilteredDTC(&DtcCount);
                    switch (ReturnGetNumberOfFilteredDTC)
                    {
                    case DEM_NUMBER_OK:
                        Dcm_Channel[Offset + MsgLen] = (uint8)(DtcCount >> 8u);
                        MsgLen                       = MsgLen + 1u;
                        Dcm_Channel[Offset + MsgLen] = (uint8)DtcCount;
                        MsgLen                       = MsgLen + 1u;
                        break;
                    case DEM_NUMBER_FAILED:
                        NotAllowedFlag = TRUE;
                        break;
                    case DEM_NUMBER_PENDING:
                        PendingFlag = TRUE;
                        break;
                    default:
                        NotAllowedFlag = TRUE;
                        break;
                    }
                }
                else
                {
                    /*if fail to set DTC filter*/
                    NotAllowedFlag = TRUE;
                }
            }
        }
        else
        {
            NotAllowedFlag = TRUE;
        }
        if (TRUE == NotAllowedFlag)
        {
            /*if fail to get any request information,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret        = E_NOT_OK;
        }
        else if (TRUE == PendingFlag)
        {
            /*if some request info is not available yet,enter pending and wait for the next cycle*/
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            ret                                 = DCM_E_PENDING;
        }
        else
        {
            /* check tx data length */
            if ((MsgLen) > pDcmChannelCfg->Dcm_DslBufferSize)
            {
                /*Pdu length is bigger than buffer size,ignore the request message */
                *ErrorCode = DCM_E_RESPONSETOOLONG;
                ret        = E_NOT_OK;
            }
            if (E_OK == ret)
            {
                /*assemble positive response*/
                Dcm_Channel[Offset]        = 0x59;
                Dcm_Channel[Offset + 1u]   = 0x01;
                pMsgContext->ResMaxDataLen = MsgLen;
                pMsgContext->ResDataLen    = MsgLen;
                pMsgContext->pResData      = &Dcm_Channel[Offset];
                DsdInternal_ProcessingDone(ProtocolCtrlId);
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X19_01_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X19_02_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x02 sub service process>
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x19_02_SubDealTwo(uint8 ProtocolCtrlId, Dcm_MsgLenType MsgLen)
{
    uint8                    TxChannelCtrlIndex;
    uint8                    TxChannelCfgIndex;
    uint8                    MsgCtrlId;
    uint32                   Offset;
    Std_ReturnType           ret = E_OK;
    const Dcm_DslBufferType* pDcmChannelCfg;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*find Tx buffer and offset*/
    TxChannelCtrlIndex                              = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex                               = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmChannelCfg                                  = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset                                          = pDcmChannelCfg->offset;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
    Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
    DsdInternal_ProcessingDone(ProtocolCtrlId);
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x02 sub service process>
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
static FUNC(void, DCM_CODE) Dcm_UDS0x19_02_SubDealone(
    uint8 ProtocolCtrlId,
    P2CONST(Dcm_UDS0x19Types, AUTOMATIC, DCM_VAR) Dcm_UDS0x19Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    boolean                              Flag;
    Dem_ReturnSetFilterType              ReturnSetDTCFilter;
    Dem_ReturnGetNextFilteredElementType ReturnGetNextFilteredDTC;
    Dem_ReturnGetNumberOfFilteredDTCType ReturnGetNumberOfFilteredDTC;
    uint8                                DtcStatus;
    uint32                               Dtc;
    uint8                                TxChannelCtrlIndex;
    uint8                                TxChannelCfgIndex;
    const Dcm_DslBufferType*             pDcmChannelCfg;
    uint8                                MsgCtrlId;
    uint8                                DtcStatusMask;
    Dcm_MsgLenType*                      MsgLen = Dcm_UDS0x19Type->MsgLen;
    uint32                               Offset = Dcm_UDS0x19Type->Offset;

    MsgCtrlId          = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    DtcStatusMask      = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2];
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmChannelCfg     = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    /*get DTCStatusAvailabilityMask*/
    ReturnSetDTCFilter = Dem_DcmSetDTCFilter(
        DtcStatusMask,
        DEM_DTC_KIND_ALL_DTCS,
        DEM_DTC_FORMAT_UDS,
        DEM_DTC_ORIGIN_PRIMARY_MEMORY,
        FALSE,
        DEM_SEVERITY_NO_SEVERITY,
        FALSE);
    if (DEM_FILTER_ACCEPTED == ReturnSetDTCFilter)
    {
        ReturnGetNumberOfFilteredDTC = Dem_DcmGetNumberOfFilteredDTC(Dcm_UDS0x19Type->DtcCount);

        switch (ReturnGetNumberOfFilteredDTC)
        {
        case DEM_NUMBER_OK:
            Flag             = FALSE;
            uint32 totalsize = (uint32)((uint32)(*Dcm_UDS0x19Type->DtcCount) * 4uL) + 3uL;
            if (totalsize > pDcmChannelCfg->Dcm_DslBufferSize)
            {
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                if (totalsize > DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol[ProtocolCtrlId]
                                    .pDcmDslProtocolRow->DcmDslProtocolMaximumResponseSize)
#endif
                {
                    Flag       = TRUE;
                    *ErrorCode = DCM_E_RESPONSETOOLONG;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                    DslInternal_InitPageBuffer();
#endif
                }
            }
            for (uint16 index = 0;
                 ((index < (*Dcm_UDS0x19Type->DtcCount)) && (FALSE == Flag)
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                  && (Dcm_PageBufferData.IsBufferOccupied == FALSE)
#endif /* TRUE == DCM_PAGEDBUFFER_ENABLED */
                 );
                 ++index)
            {
                ReturnGetNextFilteredDTC = Dem_DcmGetNextFilteredDTC(&Dtc, &DtcStatus);
                switch (ReturnGetNextFilteredDTC)
                {
                case DEM_FILTERED_OK:
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                    if ((index > Dcm_PageBufferData.IloopOne) || (Dcm_PageBufferData.PagedBufferStarted == FALSE))
#endif /* TRUE == DCM_PAGEDBUFFER_ENABLED */
                    {
                        Dcm_Channel[Offset + *(MsgLen)] = (uint8)(Dtc >> 16u);
                        *(MsgLen)                       = *(MsgLen) + 1u;
                        Dcm_Channel[Offset + *(MsgLen)] = (uint8)(Dtc >> 8u);
                        *(MsgLen)                       = *(MsgLen) + 1u;
                        Dcm_Channel[Offset + *(MsgLen)] = (uint8)(Dtc);
                        *(MsgLen)                       = *(MsgLen) + 1u;
                        Dcm_Channel[Offset + *(MsgLen)] = DtcStatus;
                        *(MsgLen)                       = *(MsgLen) + 1u;
                    }
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                    if ((*(MsgLen) + 4u) > (pDcmChannelCfg->Dcm_DslBufferSize))
                    {
                        Flag = TRUE;
                        if (Dcm_PageBufferData.PagedBufferStarted == FALSE)
                        {
                            Dcm_PageBufferData.PagedBufferStarted = TRUE;
                            Dcm_PageBufferData.TotalSize          = totalsize;
                            Dcm_PageBufferData.IloopOne           = index;
                            Dcm_PageBufferData.FilledSize         = (uint32)((uint32)((uint32)index + 1uL) * 4uL) + 3uL;
                            /** active pagedBuffer and the first page is filled */
                            DsdInternal_StartPagedProcessing(ProtocolCtrlId);
                            *Dcm_UDS0x19Type->PendingFlag = TRUE;
                            *Dcm_UDS0x19Type->MsgLen      = 0u;
                        }
                        else
                        {
                            /** fill the next page */
                            Flag                                = TRUE;
                            Dcm_PageBufferData.BufferOverFlowed = TRUE;
                            Dcm_PageBufferData.IsNextFilled     = TRUE;
                            Dcm_PageBufferData.NextFilledSize =
                                (uint32)((uint32)index - (uint32)Dcm_PageBufferData.IloopOne + 1uL) * 4uL;
                            Dcm_PageBufferData.IloopOne         = index;
                            Dcm_PageBufferData.IsBufferOccupied = TRUE;
                            *Dcm_UDS0x19Type->MsgLen            = 0u;
                        }
                    }
                    if (((index + 1u) == *(Dcm_UDS0x19Type->DtcCount))
                        && (Dcm_PageBufferData.PagedBufferStarted == TRUE))
                    {
                        /**< the last page*/
                        Dcm_PageBufferData.BufferOverFlowed = TRUE;
                        Dcm_PageBufferData.IsNextFilled     = TRUE;
                        Dcm_PageBufferData.NextFilledSize =
                            (uint32)((uint32)index - (uint32)Dcm_PageBufferData.IloopOne + 1uL) * 4uL;
                        Dcm_PageBufferData.IloopOne         = index;
                        Dcm_PageBufferData.IsBufferOccupied = TRUE;
                    }
#endif /* TRUE == DCM_PAGEDBUFFER_ENABLED */
                    break;
                case DEM_FILTERED_PENDING:
                    *(Dcm_UDS0x19Type->PendingFlag) = TRUE;
                    Flag                            = TRUE;
                    break;
                case DEM_FILTERED_BUFFER_TOO_SMALL:
                case DEM_FILTERED_NO_MATCHING_ELEMENT:
                default:
                    Flag = TRUE;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                    if (Dcm_PageBufferData.PagedBufferStarted == TRUE)
                    {
                        DspInternal_CancelPagedBufferProcessing();
                    }
                    else
#endif /* TRUE == DCM_PAGEDBUFFER_ENABLED */
                    {
                        *(Dcm_UDS0x19Type->NotAllowedFlag) = TRUE;
                    }
                    break;
                }
            }
            break;
        case DEM_NUMBER_PENDING:
            *(Dcm_UDS0x19Type->PendingFlag) = TRUE;
            break;
        case DEM_NUMBER_FAILED:
        default:
            *(Dcm_UDS0x19Type->NotAllowedFlag) = TRUE;
            break;
        }
    }
    else
    {
        *(Dcm_UDS0x19Type->NotAllowedFlag) = TRUE;
    }
}

/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x02 sub service process>
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
    Dcm_UDS0x19_02(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8                    TxChannelCtrlIndex;
    uint8                    TxChannelCfgIndex;
    uint8                    MsgCtrlId;
    uint32                   Offset;
    uint16                   Index          = 0;
    uint16                   DtcCount       = 0;
    boolean                  NotAllowedFlag = FALSE;
    boolean                  PendingFlag    = FALSE;
    Std_ReturnType           ret            = E_OK;
    Dcm_MsgLenType           MsgLen;
    Dcm_UDS0x19Types         Dcm_UDS0x19Type;
    const Dcm_DslBufferType* pDcmChannelCfg;
    uint8                    mask = 0u;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*check the massage length*/
    if (DCM_UDS0X19_SUBFUNC0X02_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret        = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /*find Tx buffer and offset*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg     = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset             = pDcmChannelCfg->offset;
        MsgLen             = 3u; /*keep two bytes to store response SID and echo of sub-function*/
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
        if (TRUE == Dcm_PageBufferData.PagedBufferStarted)
        {
            MsgLen = 0u;
        }
        else
#endif
        {
            Dcm_Channel[Offset]      = 0x59u;
            Dcm_Channel[Offset + 1u] = 0x02u;
            /*get DTCStatusAvailabilityMask*/
            ret  = Dem_DcmGetDTCStatusAvailabilityMask(&Dcm_Channel[Offset + MsgLen - 1u]);
            mask = Dcm_Channel[Offset + MsgLen - 1u];
        }
        if (E_OK != ret)
        {
            NotAllowedFlag = TRUE;
        }
        else
        {
            /*if DTCStatusAvailabilityMask = 0,no DTC*/
            if (((mask != 0u) && (Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2] != 0u))
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                || (TRUE == Dcm_PageBufferData.PagedBufferStarted)
#endif
            )
            {
                Dcm_UDS0x19Type.PendingFlag    = &PendingFlag;
                Dcm_UDS0x19Type.NotAllowedFlag = &NotAllowedFlag;
                Dcm_UDS0x19Type.MsgLen         = &MsgLen;
                Dcm_UDS0x19Type.Index          = &Index;
                Dcm_UDS0x19Type.Offset         = Offset;
                Dcm_UDS0x19Type.DtcCount       = &DtcCount;
                Dcm_UDS0x19_02_SubDealone(ProtocolCtrlId, &Dcm_UDS0x19Type, ErrorCode);
            }
        }
        if (TRUE == NotAllowedFlag)
        {
            /*if fail to get any request information,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret        = E_NOT_OK;
        }
        else if (TRUE == PendingFlag)
        {
            /*if some request info is not available yet,enter pending and wait for the next cycle*/
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            ret                                 = DCM_E_PENDING;
        }
        else
        {
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
            if (Dcm_PageBufferData.PagedBufferStarted == FALSE)
#endif
            {
                ret = Dcm_UDS0x19_02_SubDealTwo(ProtocolCtrlId, MsgLen);
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X19_02_ENABLED */

/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x03 sub service process>
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
#if (STD_ON == DCM_UDS_SERVICE0X19_03_ENABLED)
static FUNC(Std_ReturnType, DCM_CODE)
    Dcm_UDS0x19_03(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8                                TxChannelCtrlIndex;
    uint8                                TxChannelCfgIndex;
    uint8                                SnapshotRecord;
    uint8                                MsgCtrlId;
    uint32                               Offset;
    uint32                               Dtc;
    Dcm_MsgLenType                       MsgLen;
    Dem_ReturnGetNextFilteredElementType ReturnGetNextFilteredElement = DEM_FILTERED_OK;
    uint16                               NumberOfFilteredRecords      = 0;
    uint16                               iloop;
    Std_ReturnType                       ret = E_OK;
    const Dcm_DslBufferType*             pDcmChannelCfg;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*check the massage length*/
    if (DCM_UDS0X19_SUBFUNC0X03_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret        = E_NOT_OK;
    }

    if (E_OK == ret)
    {
        /*find Tx buffer and offset*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg     = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset             = pDcmChannelCfg->offset;

        if (DEM_WRONG_FILTER == Dem_DcmSetFreezeFrameRecordFilter(DEM_DTC_FORMAT_UDS, &NumberOfFilteredRecords))
        {
            /*if fail to set filter,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret        = E_NOT_OK;
        }
    }

    if (E_OK == ret)
    {
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
        if (Dcm_PageBufferData.PagedBufferStarted == TRUE)
        {
            MsgLen = 0u;
        }
        else
#endif
        {
            Dcm_Channel[Offset]      = 0x59;
            Dcm_Channel[Offset + 1u] = 0x03;
            MsgLen                   = 2u;
        }
        uint32 totalsize = NumberOfFilteredRecords * 4u + 2u;
        if (totalsize > pDcmChannelCfg->Dcm_DslBufferSize)
        {
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
            if (totalsize > (uint32)(DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol[ProtocolCtrlId]
                                         .pDcmDslProtocolRow->DcmDslProtocolMaximumResponseSize))
#endif
            {
                *ErrorCode = DCM_E_RESPONSETOOLONG;
                ret        = E_NOT_OK;
            }
        }
        if (ret == E_OK)
        {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
            boolean Flag = FALSE;
#endif
            for (iloop = 0u;
                 ((iloop < NumberOfFilteredRecords)
                  && (ReturnGetNextFilteredElement != DEM_FILTERED_NO_MATCHING_ELEMENT)
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
                  && (Flag == FALSE) && (Dcm_PageBufferData.IsBufferOccupied == FALSE)
#endif
                 );
                 iloop++)
            {
                /*read-out the corresponding FreezeFrameRecord*/
                ReturnGetNextFilteredElement = Dem_DcmGetNextFilteredRecord(&Dtc, &SnapshotRecord);
                if (ReturnGetNextFilteredElement == DEM_FILTERED_OK)
                {
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
                    if ((iloop >= Dcm_PageBufferData.IloopOne) || (Dcm_PageBufferData.PagedBufferStarted == FALSE))
#endif /* TRUE == DCM_PAGEDBUFFER_ENABLED */
                    {
                        Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 16u);
                        MsgLen                       = MsgLen + 1u;
                        Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 8u);
                        MsgLen                       = MsgLen + 1u;
                        Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc);
                        MsgLen                       = MsgLen + 1u;
                        Dcm_Channel[Offset + MsgLen] = SnapshotRecord;
                        MsgLen                       = MsgLen + 1u;
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
                        if ((MsgLen + 4u) > (pDcmChannelCfg->Dcm_DslBufferSize))
                        {
                            Flag = TRUE;
                            if (Dcm_PageBufferData.PagedBufferStarted == FALSE)
                            {
                                Dcm_PageBufferData.PagedBufferStarted = TRUE;
                                Dcm_PageBufferData.TotalSize          = totalsize;
                                Dcm_PageBufferData.IloopOne           = iloop;
                                Dcm_PageBufferData.FilledSize         = (iloop + 1u) * 4u + 2u;
                                /** active pagedBuffer and the first page is filled */
                                DsdInternal_StartPagedProcessing(ProtocolCtrlId);
                                /*if some request info is not available yet,enter pending and wait for the next cycle*/
                                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                                ret                                 = DCM_E_PENDING;
                            }
                            else
                            {
                                /** fill the next page */
                                Flag                                = TRUE;
                                Dcm_PageBufferData.BufferOverFlowed = TRUE;
                                Dcm_PageBufferData.IsNextFilled     = TRUE;
                                Dcm_PageBufferData.NextFilledSize   = (iloop - Dcm_PageBufferData.IloopOne + 1u) * 4u;
                                Dcm_PageBufferData.IloopOne         = iloop;
                                Dcm_PageBufferData.IsBufferOccupied = TRUE;
                            }
                        }
                        if (((iloop + 1u) == NumberOfFilteredRecords)
                            && (Dcm_PageBufferData.PagedBufferStarted == TRUE))
                        {
                            /**< the last page*/
                            Dcm_PageBufferData.BufferOverFlowed = TRUE;
                            Dcm_PageBufferData.IsNextFilled     = TRUE;
                            Dcm_PageBufferData.NextFilledSize   = (iloop + 1u - Dcm_PageBufferData.IloopOne) * 4u;
                            Dcm_PageBufferData.IloopOne         = iloop;
                            Dcm_PageBufferData.IsBufferOccupied = TRUE;
                        }
#endif /* TRUE == DCM_PAGEDBUFFER_ENABLED */
                    }
                }
            }
        }
    }
    if (E_OK == ret)
    {
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
        if (Dcm_PageBufferData.PagedBufferStarted == FALSE)
#endif
        { /*assemble positive response*/
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X19_03_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X19_04_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x04 sub service process>
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

static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x19_04_SubDeal(
    uint8 ProtocolCtrlId,
    P2CONST(Dcm_UDS0x19Types, AUTOMATIC, DCM_VAR) Dcm_UDS0x19Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8                                 TxChannelCtrlIndex;
    uint8                                 TxChannelCfgIndex;
    uint32                                CurBufferSize;
    uint16                                BufferSize = 0u;
    uint8                                 DtcStatus;
    uint8                                 SnapshotRecord;
    uint8                                 MsgCtrlId;
    uint32                                ChannelSize;
    uint32                                Offset;
    uint32                                Dtc;
    Dem_ReturnGetFreezeFrameDataByDTCType retGetFreezeFrameDataByDTC;
    Dem_ReturnDisableDTCRecordUpdateType  ReturnDisableDTCRecordUpdate;
    Dem_ReturnGetStatusOfDTCType          retGetStatusOfDTC;
    Std_ReturnType                        ret    = E_OK;
    Dcm_MsgLenType*                       MsgLen = Dcm_UDS0x19Type->MsgLen;
    const Dcm_MsgContextType*             pMsgContext;

    MsgCtrlId   = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /*find Tx buffer and offset*/
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    ChannelSize        = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].Dcm_DslBufferSize;
    Offset             = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
    /*get DTC and reoord number from request message*/
    Dtc = (((uint32)pMsgContext->pReqData[2]) << 16u) | (((uint32)pMsgContext->pReqData[3]) << 8u)
          | (((uint32)pMsgContext->pReqData[4]));
    SnapshotRecord                  = pMsgContext->pReqData[5];
    *(MsgLen)                       = 2u;
    Dcm_Channel[Offset + *(MsgLen)] = (uint8)(Dtc >> 16u);
    *(MsgLen)                       = *(MsgLen) + 1u;
    Dcm_Channel[Offset + *(MsgLen)] = (uint8)(Dtc >> 8u);
    *(MsgLen)                       = *(MsgLen) + 1u;
    Dcm_Channel[Offset + *(MsgLen)] = (uint8)(Dtc);
    *(MsgLen)                       = *(MsgLen) + 1u;
    /*read-out the correspoding DtcStatus of required DTC*/
    retGetStatusOfDTC = Dem_DcmGetStatusOfDTC(Dtc, DEM_DTC_ORIGIN_PRIMARY_MEMORY, &DtcStatus);
    if (retGetStatusOfDTC != DEM_STATUS_OK)
    {
        *(Dcm_UDS0x19Type->NotAllowedFlag) = TRUE;
    }
    else
    {
        /*Get DtcStatus Successfully*/
        Dcm_Channel[Offset + *(MsgLen)] = DtcStatus;
        *(MsgLen)                       = *(MsgLen) + 1u;
        /*read-out the correspoding snapshot by SnapshotRecord*/
        ReturnDisableDTCRecordUpdate = Dem_DcmDisableDTCRecordUpdate(Dtc, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        if (DEM_DISABLE_DTCRECUP_OK == ReturnDisableDTCRecordUpdate)
        {
            CurBufferSize = (ChannelSize - *(MsgLen)-1u);
            Dem_ReturnGetSizeOfDataByDTCType getSizeRet =
                Dem_DcmGetSizeOfFreezeFrameByDTC(Dtc, DEM_DTC_ORIGIN_PRIMARY_MEMORY, SnapshotRecord, &BufferSize);
            if ((getSizeRet == DEM_GETSIZEBYDTC_OK) && (CurBufferSize >= BufferSize))
            {
                /*reserve 1byte to store record number*/
                BufferSize                 = (uint16)CurBufferSize;
                retGetFreezeFrameDataByDTC = Dem_DcmGetFreezeFrameDataByDTC(
                    Dtc,
                    DEM_DTC_ORIGIN_PRIMARY_MEMORY,
                    SnapshotRecord,
                    &Dcm_Channel[Offset + *(MsgLen)],
                    &BufferSize);
                if (DEM_GET_FFDATABYDTC_OK == retGetFreezeFrameDataByDTC)
                {
                    *(MsgLen) = *(MsgLen) + (uint32)BufferSize;
                }
                else if (DEM_GET_FFDATABYDTC_PENDING == retGetFreezeFrameDataByDTC)
                {
                    *(Dcm_UDS0x19Type->PendingFlag) = TRUE;
                }
                else if (
                    (DEM_GET_FFDATABYDTC_WRONG_DTC == retGetFreezeFrameDataByDTC)
                    || (DEM_GET_FFDATABYDTC_WRONG_DTCORIGIN == retGetFreezeFrameDataByDTC)
                    || (DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER == retGetFreezeFrameDataByDTC))
                {
                    /*if fail to get any request information,send NRC 0x31*/
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                    (void)Dem_DcmEnableDTCRecordUpdate();
                    ret = E_NOT_OK;
                }
                else
                {
                    /*idle*/
                }
            }
            else if (getSizeRet == DEM_GETSIZEBYDTC_OK)
            {
                *ErrorCode = DCM_E_RESPONSETOOLONG;
                ret        = E_NOT_OK;
            }
            else
            {
                /*if fail to get any request information,send NRC 0x31*/
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret        = E_NOT_OK;
            }
            if (E_OK != Dem_DcmEnableDTCRecordUpdate())
            {
                *(Dcm_UDS0x19Type->NotAllowedFlag) = TRUE;
            }
        }
        else if (DEM_DISABLE_DTCRECUP_WRONG_DTC == ReturnDisableDTCRecordUpdate)
        {
            *(Dcm_UDS0x19Type->NotAllowedFlag) = TRUE;
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
 * Brief               <UDS  0x19 service  0x04 sub service process>
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
    Dcm_UDS0x19_04(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8                    TxChannelCtrlIndex;
    uint8                    TxChannelCfgIndex;
    uint8                    MsgCtrlId;
    uint32                   Offset;
    boolean                  NotAllowedFlag = FALSE;
    boolean                  PendingFlag    = FALSE;
    Dcm_MsgLenType           MsgLen;
    Std_ReturnType           ret = E_OK;
    Dcm_UDS0x19Types         Dcm_UDS0x19Type;
    const Dcm_DslBufferType* pDcmChannelCfg;
    Dcm_MsgContextType*      pMsgContext;

    MsgCtrlId   = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    if (DCM_UDS0X19_SUBFUNC0X04_REQ_DATA_LENGTH != pMsgContext->ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret        = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /*find Tx buffer and offset*/
        TxChannelCtrlIndex             = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex              = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg                 = &(DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex];
        Offset                         = pDcmChannelCfg->offset;
        Dcm_UDS0x19Type.MsgLen         = &MsgLen;
        Dcm_UDS0x19Type.NotAllowedFlag = &NotAllowedFlag;
        Dcm_UDS0x19Type.PendingFlag    = &PendingFlag;
        ret                            = Dcm_UDS0x19_04_SubDeal(ProtocolCtrlId, &Dcm_UDS0x19Type, ErrorCode);
        if (TRUE == NotAllowedFlag)
        {
            /*if fail to get any request information,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret        = E_NOT_OK;
        }
        else if (TRUE == PendingFlag)
        {
            /*if some request info is not available yet,enter pending and wait for the next cycle*/
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            ret                                 = DCM_E_PENDING;
        }
        else
        {
            if (E_OK == ret)
            {
                /* check tx data length */
                if ((MsgLen) > (pDcmChannelCfg->Dcm_DslBufferSize))
                {
                    /*Pdu length is bigger than buffer size,ignore the request message */
                    *ErrorCode = DCM_E_RESPONSETOOLONG;
                    ret        = E_NOT_OK;
                }
                if (E_OK == ret)
                {
                    /*assemble positive response*/
                    Dcm_Channel[Offset]        = 0x59;
                    Dcm_Channel[Offset + 1u]   = 0x04;
                    pMsgContext->ResMaxDataLen = MsgLen;
                    pMsgContext->ResDataLen    = MsgLen;
                    pMsgContext->pResData      = &Dcm_Channel[Offset];
                    DsdInternal_ProcessingDone(ProtocolCtrlId);
                }
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X19_04_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X19_06_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x06 sub service process>
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
    Dcm_UDS0x19_06(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8                                    TxChannelCtrlIndex;
    uint8                                    TxChannelCfgIndex;
    uint32                                   CurChannelSize;
    uint16                                   Size = 0u;
    uint8                                    DtcStatus;
    uint8                                    DtcExtendedDataRecordNumber;
    uint8                                    MsgCtrlId;
    uint32                                   ChannelSize;
    uint32                                   Offset;
    uint32                                   Dtc;
    boolean                                  NotAllowedFlag = FALSE;
    boolean                                  PendingFlag    = FALSE;
    Dcm_MsgLenType                           MsgLen;
    Dem_ReturnGetExtendedDataRecordByDTCType ReturnGetExtendedDataRecord;
    Dem_ReturnDisableDTCRecordUpdateType     ReturnDisableDTCRecordUpdate;
    Dem_ReturnGetStatusOfDTCType             retGetStatusOfDTC;
    Std_ReturnType                           ret = E_OK;
    Dcm_MsgContextType*                      pMsgContext;
    const uint8*                             pReqData;
    const Dcm_DslBufferType*                 pDcmChannelCfg;

    MsgCtrlId   = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex; /*check the massage length*/
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    pReqData    = pMsgContext->pReqData;
    if (DCM_UDS0X19_SUBFUNC0X06_REQ_DATA_LENGTH != pMsgContext->ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret        = E_NOT_OK;
    }
    else
    {
        /*find Tx buffer and offset*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg     = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        ChannelSize        = pDcmChannelCfg->Dcm_DslBufferSize;
        Offset             = pDcmChannelCfg->offset;
        /*get DTC and record number from request message*/
        Dtc = (((uint32)pReqData[2]) << 16u) | (((uint32)pReqData[3]) << 8u) | (((uint32)pReqData[4]));
        DtcExtendedDataRecordNumber  = pReqData[5];
        MsgLen                       = 2u;
        Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 16u);
        MsgLen                       = MsgLen + 1u;
        Dcm_Channel[Offset + MsgLen] = (uint8)(Dtc >> 8u);
        MsgLen                       = MsgLen + 1u;
        Dcm_Channel[Offset + MsgLen] = (uint8)Dtc;
        MsgLen                       = MsgLen + 1u;
        /*read-out the corresponding DtcStatus of required DTC*/
        retGetStatusOfDTC = Dem_DcmGetStatusOfDTC(Dtc, DEM_DTC_ORIGIN_PRIMARY_MEMORY, &DtcStatus);
        if (retGetStatusOfDTC != DEM_STATUS_OK)
        {
            NotAllowedFlag = TRUE;
        }
        else
        {
            /*Get DtcStatus Successfully*/
            Dcm_Channel[Offset + MsgLen] = DtcStatus;
            MsgLen                       = MsgLen + 1u;
            /*read-out the corresponding extended data of DtcExtendedDataRecordNumber*/
            /*disable DTC record update*/
            ReturnDisableDTCRecordUpdate = Dem_DcmDisableDTCRecordUpdate(Dtc, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
            if (DEM_DISABLE_DTCRECUP_OK == ReturnDisableDTCRecordUpdate)
            {
                CurChannelSize                              = ChannelSize - MsgLen - 1u;
                Dem_ReturnGetSizeOfDataByDTCType getSizeRet = Dem_DcmGetSizeOfExtendedDataRecordByDTC(
                    Dtc,
                    DEM_DTC_ORIGIN_PRIMARY_MEMORY,
                    DtcExtendedDataRecordNumber,
                    &Size);
                if ((getSizeRet == DEM_GETSIZEBYDTC_OK) && (CurChannelSize >= Size))
                {
                    /*calculate the remained size of Tx buffer,keep one byte to store record number*/
                    Size                        = (uint16)CurChannelSize;
                    ReturnGetExtendedDataRecord = Dem_DcmGetExtendedDataRecordByDTC(
                        Dtc,
                        DEM_DTC_ORIGIN_PRIMARY_MEMORY,
                        DtcExtendedDataRecordNumber,
                        &Dcm_Channel[Offset + MsgLen],
                        &Size);
                    if ((DEM_RECORD_OK == ReturnGetExtendedDataRecord) && (Size != 0u))
                    {
                        /*successfully store extended data,increase MsgLen*/
                        MsgLen = MsgLen + (uint32)Size;
                    }
                    else if (DEM_RECORD_PENDING == ReturnGetExtendedDataRecord)
                    {
                        PendingFlag = TRUE;
                    }
                    else if (
                        (DEM_RECORD_WRONG_DTC == ReturnGetExtendedDataRecord)
                        || (DEM_RECORD_WRONG_DTCORIGIN == ReturnGetExtendedDataRecord)
                        || (DEM_RECORD_WRONG_NUMBER == ReturnGetExtendedDataRecord))
                    { /*if fail to get any request information,send NRC 0x31*/
                        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                        (void)Dem_DcmEnableDTCRecordUpdate();
                        ret = E_NOT_OK;
                    }
                    else
                    {
                        /*idle*/
                    }
                }
                else if (getSizeRet == DEM_GETSIZEBYDTC_OK)
                {
                    *ErrorCode = DCM_E_RESPONSETOOLONG;
                    ret        = E_NOT_OK;
                }
                else
                {
                    /*if fail to get any request information,send NRC 0x31*/
                    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                    ret        = E_NOT_OK;
                }
                /*enable DTC record update*/
                if (E_NOT_OK == Dem_DcmEnableDTCRecordUpdate())
                {
                    NotAllowedFlag = TRUE;
                }
            }
            else if (DEM_DISABLE_DTCRECUP_WRONG_DTC == ReturnDisableDTCRecordUpdate)
            {
                NotAllowedFlag = TRUE;
            }
            else
            {
                /*idle*/
            }
        }
        if (E_OK == ret)
        {
            if (TRUE == NotAllowedFlag)
            {
                /*if fail to get any request information,send NRC 0x31*/
                *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
                ret        = E_NOT_OK;
            }
            else if (TRUE == PendingFlag)
            {
                /*if some request info is not available yet,
                enter pending and wait for the next cycle*/
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                ret                                 = DCM_E_PENDING;
            }
            else
            {
                /* check tx data length */
                if ((MsgLen) > pDcmChannelCfg->Dcm_DslBufferSize)
                {
                    /*Pdu length is bigger than buffer size,ignore the request message */
                    *ErrorCode = DCM_E_RESPONSETOOLONG;
                    ret        = E_NOT_OK;
                }
                else
                {
                    /*assemble positive response*/
                    Dcm_Channel[Offset]        = 0x59;
                    Dcm_Channel[Offset + 1u]   = 0x06;
                    pMsgContext->ResMaxDataLen = MsgLen;
                    pMsgContext->ResDataLen    = MsgLen;
                    pMsgContext->pResData      = &Dcm_Channel[Offset];
                    DsdInternal_ProcessingDone(ProtocolCtrlId);
                }
            }
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X19_06_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X19_0A_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x0A sub service process>
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x19_0ADeal(
    uint8 ProtocolCtrlId,
    P2VAR(uint16, AUTOMATIC, DCM_VAR) DtcCount,
    P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) MsgLen,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    Std_ReturnType                       ret = E_OK;
    boolean                              Flag;
    uint16                               Index;
    uint8                                DtcStatus;
    uint32                               Dtc;
    uint8                                TxChannelCtrlIndex;
    uint8                                TxChannelCfgIndex;
    uint8                                MsgCtrlId;
    uint32                               Offset;
    Dem_ReturnSetFilterType              ReturnSetDTCFilter;
    Dem_ReturnGetNextFilteredElementType ReturnGetNextFilteredDTC;
    Dem_ReturnGetNumberOfFilteredDTCType ReturnGetNumberOfFilteredDTC;
    const Dcm_DslBufferType*             pDcmChannelCfg;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
    const Dcm_DslProtocolRowType* pDcmDslProtocolRow =
        &DcmPbCfgPtr->pDcmDslCfg->pDcmDslProtocol->pDcmDslProtocolRow[ProtocolCtrlId];
#endif

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    /*find Tx buffer and offset*/
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmChannelCfg     = &(DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex];
    Offset             = pDcmChannelCfg->offset;

    ReturnSetDTCFilter = Dem_DcmSetDTCFilter(
        0x00,
        DEM_DTC_KIND_ALL_DTCS,
        DEM_DTC_FORMAT_UDS,
        DEM_DTC_ORIGIN_PRIMARY_MEMORY,
        FALSE,
        DEM_SEVERITY_NO_SEVERITY,
        FALSE);
    if (DEM_FILTER_ACCEPTED == ReturnSetDTCFilter)
    {
        ReturnGetNumberOfFilteredDTC = Dem_DcmGetNumberOfFilteredDTC(DtcCount);

        switch (ReturnGetNumberOfFilteredDTC)
        {
        case DEM_NUMBER_OK:
            Flag = FALSE;
            for (Index = 0; (Index < *DtcCount) && (FALSE == Flag)
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                            && ((Dcm_PageBufferData.BufferOverFlowed == FALSE) || (Dcm_PageBufferData.TotalSize == 0UL))
#endif /* TRUE == DCM_PAGEDBUFFER_ENABLED */
                     ;
                 Index++)
            {
                ReturnGetNextFilteredDTC = Dem_DcmGetNextFilteredDTC(&Dtc, &DtcStatus);
                switch (ReturnGetNextFilteredDTC)
                {
                case DEM_FILTERED_OK:
                    if ((*MsgLen + 4u) > pDcmChannelCfg->Dcm_DslBufferSize)
                    {
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                        uint32 totalSize = 3uL + ((uint32)(*DtcCount) << 2u);
                        if (totalSize <= pDcmDslProtocolRow->DcmDslProtocolMaximumResponseSize)
                        {
                            /**< jump loop */
                            Dcm_PageBufferData.BufferOverFlowed = TRUE;
                            Dcm_PageBufferData.IloopOne         = Index;
                            Dcm_PageBufferData.FilledSize       = *MsgLen;
                            Dcm_PageBufferData.TotalSize        = totalSize;
                            Dcm_MemoryCopy(
                                &(Dcm_Channel[Offset]),
                                Dcm_PageBufferData.PagedBuffer,
                                Dcm_PageBufferData.FilledSize);
                            DsdInternal_ProcessPage(ProtocolCtrlId);
                            DspInternal_DcmUpdatePage(ProtocolCtrlId, FALSE);
                            Dcm_PageBufferData.ReqOffset = 0u;
                            /**< ready to fill the next page */
                            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                            ret                                 = DCM_E_PENDING;
                        }
                        else
#endif
                        {
                            *ErrorCode = DCM_E_RESPONSETOOLONG;
                            ret        = E_NOT_OK;
                            Flag       = TRUE;
                        }
                    }
                    else
                    {
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                        if (Index >= Dcm_PageBufferData.IloopOne)
#endif
                        {
                            Dcm_Channel[Offset + (*MsgLen)] = (uint8)(Dtc >> 16u);
                            (*MsgLen)                       = (*MsgLen) + 1u;
                            Dcm_Channel[Offset + (*MsgLen)] = (uint8)(Dtc >> 8u);
                            (*MsgLen)                       = (*MsgLen) + 1u;
                            Dcm_Channel[Offset + (*MsgLen)] = (uint8)(Dtc);
                            (*MsgLen)                       = (*MsgLen) + 1u;
                            Dcm_Channel[Offset + (*MsgLen)] = DtcStatus;
                            *MsgLen                         = *MsgLen + 1u;
                        }
                    }
                    break;
                case DEM_FILTERED_NO_MATCHING_ELEMENT:
/*if fail to get any request information,send NRC 0x22*/
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                    if (Dcm_PageBufferData.PagedBufferStarted == TRUE)
                    {
                        DspInternal_CancelPagedBufferProcessing();
                    }
                    else
#endif
                    {
                        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                        ret        = E_NOT_OK;
                        Flag       = TRUE;
                    }

                    break;
                case DEM_FILTERED_PENDING:
                    /*if some request info is not available yet,
                     * enter pending and wait for the next cycle*/
                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
                    Dcm_PageBufferData.ReqOffset =
                        ((Index - Dcm_PageBufferData.IloopOne) > 0u)
                            ? (uint32)(((uint32)Index - (uint32)Dcm_PageBufferData.IloopOne) * 4uL)
                            : 0uL;
#endif
                    ret  = DCM_E_PENDING;
                    Flag = TRUE;
                    break;
                case DEM_FILTERED_BUFFER_TOO_SMALL:
                default:
                    /*if fail to get any request information,send NRC 0x22*/
                    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                    ret        = E_NOT_OK;
                    Flag       = TRUE;
                    break;
                }
            }
            break;
        case DEM_NUMBER_PENDING:
            /*if some request info is not available yet,
             * enter pending and wait for the next cycle*/
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            ret                                 = DCM_E_PENDING;
            break;
        case DEM_NUMBER_FAILED:
        default:
            /*if fail to get any request information,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret        = E_NOT_OK;
            break;
        }
    }
    return ret;
}
/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x0A sub service process>
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
    Dcm_UDS0x19_0A(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8                    TxChannelCtrlIndex;
    uint8                    TxChannelCfgIndex;
    uint8                    MsgCtrlId;
    uint32                   Offset;
    uint16                   DtcCount = 0;
    Std_ReturnType           ret      = E_OK;
    Dcm_MsgLenType           MsgLen   = 3u;
    const Dcm_DslBufferType* pDcmChannelCfg;
    static uint8             mask = 0u;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if (DCM_UDS0X19_SUBFUNC0X0A_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret        = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /*find Tx buffer and offset*/
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex  = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg     = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset             = pDcmChannelCfg->offset;
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
        if (TRUE == Dcm_PageBufferData.PagedBufferStarted)
        {
            /**< current page offset */
            MsgLen = Dcm_PageBufferData.ReqOffset;
        }
        else
#endif
        {
            Dcm_Channel[Offset]      = 0x59u;
            Dcm_Channel[Offset + 1u] = 0x0Au;
            /*get DTCStatusAvailabilityMask*/
            ret  = Dem_DcmGetDTCStatusAvailabilityMask(&Dcm_Channel[Offset + MsgLen - 1u]);
            mask = Dcm_Channel[Offset + MsgLen - 1u];
        }
        if ((E_OK == ret) && (mask != 0u))
        {
            ret = Dcm_UDS0x19_0ADeal(ProtocolCtrlId, &DtcCount, &MsgLen, ErrorCode);
        }
#if (TRUE == DCM_PAGEDBUFFER_ENABLED)
        if (Dcm_PageBufferData.BufferOverFlowed == TRUE)
        {
            Dcm_PageBufferData.BufferOverFlowed = FALSE;
        }
#endif
        if (E_OK == ret)
        {
            /*assemble positive response*/
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X19_0A_ENABLED */

#if (STD_ON == DCM_UDS_SERVICE0X19_0E_ENABLED)
/*************************************************************************/
/*
 * Brief               <UDS  0x19 service  0x0E sub service process>
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
    Dcm_UDS0x19_0E(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8                    TxChannelCtrlIndex;
    uint8                    TxChannelCfgIndex;
    uint8                    MsgCtrlId;
    uint32                   Offset;
    Std_ReturnType           ret    = E_OK;
    Dcm_MsgLenType           MsgLen = 2u; /*store response SID and echo of sub-function*/
    uint32                   DTC;
    uint8                    DTCstatus;
    const Dcm_DslBufferType* pDcmChannelCfg;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    if (DCM_UDS0X19_SUBFUNC0X0E_REQ_DATA_LENGTH != Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret        = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        /*find Tx buffer and offset*/
        TxChannelCtrlIndex       = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex        = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg           = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset                   = pDcmChannelCfg->offset;
        Dcm_Channel[Offset]      = 0x59u;
        Dcm_Channel[Offset + 1u] = 0x0Eu;
        /*get DTCStatusAvailabilityMask*/
        ret = Dem_DcmGetDTCStatusAvailabilityMask(&Dcm_Channel[Offset + MsgLen]);
        MsgLen++;
        if (E_OK == ret)
        {
            ret = Dem_DcmGetDTCByOccurrenceTime(DEM_MOST_REC_DET_CONFIRMED_DTC, &DTC);
            if (ret == DEM_OCCURR_OK)
            {
                (void)Dem_DcmGetStatusOfDTC(DTC, DEM_DTC_ORIGIN_PRIMARY_MEMORY, &DTCstatus);
                Dcm_Channel[Offset + MsgLen] = (uint8)(DTC >> 16u);
                MsgLen++;
                Dcm_Channel[Offset + MsgLen] = (uint8)(DTC >> 8u);
                MsgLen++;
                Dcm_Channel[Offset + MsgLen] = (uint8)(DTC);
                MsgLen++;
                Dcm_Channel[Offset + MsgLen] = DTCstatus;
                MsgLen++;
            }
            else
            {
                ret = E_OK; /* response with a positive and empty response*/
            }
        }
        if ((E_OK == ret) && (MsgLen > pDcmChannelCfg->Dcm_DslBufferSize))
        {
            /*Pdu length is bigger than buffer size,ignore the request message */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            ret        = E_NOT_OK;
        }
        if (E_OK == ret)
        {
            /*assemble positive response*/
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = MsgLen;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen    = MsgLen;
            Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData      = &Dcm_Channel[Offset];
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    return ret;
}
#endif /* STD_ON == DCM_UDS_SERVICE0X19_0E_ENABLED */

/*************************************************************************/
/*
 * Brief               <UDS  0x19 service sub service process>
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x19SubfunctionAnalyse(
    uint8 ProtocolCtrlId,
    uint8 Subfunction,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    Std_ReturnType ret;

    /*check sub-function entrance*/
    switch (Subfunction)
    {
#if (STD_ON == DCM_UDS_SERVICE0X19_01_ENABLED)
    case DCM_REPORTNUMBEROFDTCBYSTATUSMASK:
        /*Sub-function:0x01*/
        ret = Dcm_UDS0x19_01(ProtocolCtrlId, ErrorCode);
        break;
#endif
#if (STD_ON == DCM_UDS_SERVICE0X19_02_ENABLED)
    case DCM_REPORTDTCBYSTATUSMASK:
        /*Sub-function:0x02*/
        ret = Dcm_UDS0x19_02(ProtocolCtrlId, ErrorCode);
        break;
#endif
#if (STD_ON == DCM_UDS_SERVICE0X19_03_ENABLED)
    case DCM_REPORTREPORTDTCSNAPSHOTIDENTIFICATION:
        /*Sub-function:0x03*/
        ret = Dcm_UDS0x19_03(ProtocolCtrlId, ErrorCode);
        break;
#endif
#if (STD_ON == DCM_UDS_SERVICE0X19_04_ENABLED)
    case DCM_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER:
        /*Sub-function:0x04*/
        ret = Dcm_UDS0x19_04(ProtocolCtrlId, ErrorCode);
        break;
#endif
#if (STD_ON == DCM_UDS_SERVICE0X19_06_ENABLED)
    case DCM_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER:
        /*Sub-function:0x06*/
        ret = Dcm_UDS0x19_06(ProtocolCtrlId, ErrorCode);
        break;
#endif
#if (STD_ON == DCM_UDS_SERVICE0X19_0A_ENABLED)
    case DCM_REPORTSUPPORTEDDTC:
        /*Sub-function:0x0A*/
        ret = Dcm_UDS0x19_0A(ProtocolCtrlId, ErrorCode);
        break;
#endif
#if (STD_ON == DCM_UDS_SERVICE0X19_0E_ENABLED)
    case DCM_REPORTMOSTRECENTCONFIRMEDDTC:
        /*Sub-function:0x0E*/
        ret = Dcm_UDS0x19_0E(ProtocolCtrlId, ErrorCode);
        break;
#endif
    default:
        /*if the sub-function is not supported,reset 'control resource' and send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        ret        = E_NOT_OK;
        break;
    }
    return ret;
}

/*************************************************************************/
/*
 * Brief               <UDS  0x19 service process>
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
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x19(
    Dcm_OpStatusType OpStatus,
    uint8            ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
    uint8          MsgCtrlId;
    uint8          Subfunction;
    Std_ReturnType ret;

    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

    /*check whether the required sub-function is configured*/
    ret = Dcm_UdsSubServicesCheck(ProtocolCtrlId);
    if (E_NOT_OK == ret)
    {
        /*if the required reset type is not supported,send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }
#if (STD_ON == DCM_UDS_SERVICE0X29_ENABLED)
    if (E_OK == ret)
    {
        ret = DspInternal_SubServiceAuthenticationCheck(ProtocolCtrlId, ErrorCode);
    }
#endif
#if (STD_ON == DCM_SESSION_FUNC_ENABLED)
    if (E_OK == ret)
    {
        ret = DsdInternal_SubSesCheck(ProtocolCtrlId, SID_READ_DTC_INFORMATION, ErrorCode);
    }
#endif
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*security check,check whether the current security supports the request service*/
        ret = DsdInternal_SubSecurityCheck(ProtocolCtrlId, SID_READ_DTC_INFORMATION, ErrorCode);
    }
#endif
    /**************************************************/
    if ((E_OK == ret) && (OpStatus != DCM_CANCEL))
    {
        Subfunction = Dcm_MsgCtrl[MsgCtrlId].Subfunction;
        /*check sub-function entrance*/
        ret = Dcm_Uds0x19SubfunctionAnalyse(ProtocolCtrlId, Subfunction, ErrorCode);
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_UDS_SERVICE0X19_ENABLED */
