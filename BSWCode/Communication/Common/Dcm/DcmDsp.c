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

/*=======[I N C L U D E S]====================================================*/
#include "Dcm_Internal.h"

/* PRQA S 1520 EOF */ /* VL_Dcm_1520 */
/******************************* Define ************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/*************************************************************************/
/*
 * Brief               <The Dsp layer response to the confirmation ReProgram>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/

/*************************************************************************/
/*
 * Brief               <The Dsp layer response to the confirmation>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:The corresponding protocol control block ID number>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <APIName>
 */
/*************************************************************************/
FUNC(void, DCM_CODE) DspInternal_DcmConfirmation(uint8 ProtocolCtrlId)
{
    uint8            MsgCtrlId;
    uint8            RxChannelCtrlId;
    uint8            TxChannelCtrlId;
    Dcm_MsgCtrlType* pDcm_MsgCtrl;

    MsgCtrlId       = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    RxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_RxCtrlChannelIndex;
    TxChannelCtrlId = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    pDcm_MsgCtrl    = &Dcm_MsgCtrl[MsgCtrlId];
    /* NRC = 0x78 respond to the confirmation process */
    if (DCM_E_RESPONSE_PENDING == (pDcm_MsgCtrl->NRC))
    {
        /* Nrc=0x78,response to confirmation */
        SchM_Enter_Dcm_ExclusiveArea();
        Dcm_ChannelCtrl[RxChannelCtrlId].Dcm_ChannelRxState        = DCM_CH_OCCUPIED;
        Dcm_ChannelCtrl[TxChannelCtrlId].Dcm_ChannelTxState        = DCM_CH_OCCUPIED;
        Dcm_ChannelCtrl[TxChannelCtrlId].Dcm_BufferCunrentPosition = 0;
        Dcm_ChannelCtrl[TxChannelCtrlId].Dcm_BufferErasePosition   = 0;
        pDcm_MsgCtrl->NRC                                          = 0xFFu;
        pDcm_MsgCtrl->RspStyle                                     = DCM_POS_RSP;
        pDcm_MsgCtrl->Dcm_MsgState                                 = DCM_MSG_PROCESSED;
        /*restart P2*Timer*/
        pDcm_MsgCtrl->Dcm_P2Ctrl.PendingNum++;
        SchM_Exit_Dcm_ExclusiveArea();
        Dcm_ResetTime(&pDcm_MsgCtrl->Dcm_P2Ctrl.Dcm_P2CurTimer);
        SchM_Enter_Dcm_ExclusiveArea();
        pDcm_MsgCtrl->Dcm_P2Ctrl.Dcm_P2ExpiredTimer = Dcm_ProtocolCtrl[ProtocolCtrlId].P2StarServerMax;
        pDcm_MsgCtrl->Dcm_P2Ctrl.Dcm_P2State        = DCM_P2TIMER_ON;
        SchM_Exit_Dcm_ExclusiveArea();
    }
    else
    {
        DspInternal_DcmConfirmation_0x27(ProtocolCtrlId);
        DspInternal_DcmConfirmation_0x10(ProtocolCtrlId);
        DspInternal_DcmConfirmation_0x11(ProtocolCtrlId);
#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
        if (Dcm_PageBufferData.TotalSize != 0UL)
        {
            DslInternal_InitPageBuffer();
        }
#endif
        /*For the realization of the various sub-function, the function code needs to be added
         * here.*/
        DslInternal_ResetResource(ProtocolCtrlId);
    }
}

/*************************************************************************/
/*
 * Brief               <Uds SubServices Check>
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_UdsSubServicesCheck(uint8 ProtocolCtrlId)
{
    uint16         SidTabCfgIndex;
    uint16         SidTabServiceCfgIndex;
    uint8          MsgCtrlIndexx;
    uint16         SubServieCfgIndex;
    uint8          Sid;
    Std_ReturnType ret;

    MsgCtrlIndexx = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Sid           = Dcm_MsgCtrl[MsgCtrlIndexx].SID;
    ret           = DsdInternal_SearchSidTabServiceIndex(Sid, ProtocolCtrlId, &SidTabCfgIndex, &SidTabServiceCfgIndex);
    if (E_OK == ret)
    {
        ret = DsdInternal_SearchSidTabSubServiceIndex(
            ProtocolCtrlId,
            SidTabCfgIndex,
            SidTabServiceCfgIndex,
            &SubServieCfgIndex);
    }
    return ret;
}

#if (STD_ON == DCM_PAGEDBUFFER_ENABLED)
/*************************************************************************/
/*
 * Brief               <Dcm  Update  Page>
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
FUNC(void, DCM_CODE) DspInternal_DcmUpdatePage(uint8 ProtocolCtrlId, boolean isUpdataPage)
{
    uint8 MsgCtrlId         = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 TxChannelCfgIndex = Dcm_ChannelCtrl[Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex].Dcm_ChannelCfgIndex;
    const Dcm_DslBufferType* pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];

    SchM_Enter_Dcm_ExclusiveArea();
    Dcm_PageBufferData.TimerStart   = TRUE;
    Dcm_PageBufferData.ExpiredTimer = DCM_PAGEBUFFER_TIMEOUTVALUE;
    Dcm_ResetTime(&Dcm_PageBufferData.CurTimer);
    if (isUpdataPage == TRUE)
    {
        Dcm_PageBufferData.FilledSize       = Dcm_PageBufferData.NextFilledSize;
        Dcm_PageBufferData.IsBufferOccupied = FALSE;
        Dcm_MemoryCopy(
            &(Dcm_Channel[pDcmChannelCfg->offset]),
            Dcm_PageBufferData.PagedBuffer,
            Dcm_PageBufferData.FilledSize);
    }
    SchM_Exit_Dcm_ExclusiveArea();
}

FUNC(void, DCM_CODE) DspInternal_CancelPagedBufferProcessing(void)
{
    Dcm_PageBufferData.PagedBufferCanceled = TRUE;
}
#endif /* STD_ON == DCM_PAGEDBUFFER_ENABLED */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
