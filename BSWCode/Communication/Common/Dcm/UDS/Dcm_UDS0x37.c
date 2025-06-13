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

#if (STD_ON == DCM_UDS_SERVICE0X37_ENABLED)
/****************************************************************
     UDS: RequestTransferExit (37 hex) service
 ***************************************************************/
/*******************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
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
Dcm_UDS0x37(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    uint32 transferRequestParameterRecordSize;
    uint32 transferResponseParameterRecordSize = 0;
    uint8* transferRequestParameterRecord;
    uint8* transferResParameterRecord;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;

    transferRequestParameterRecordSize = pMsgContext->ReqDataLen - 1u;
    transferRequestParameterRecord = &(pMsgContext->pReqData[1]);
    transferResParameterRecord = &Dcm_Channel[Offset + 1u];
    /*Following NRC will be the responsibility of the callout function 0x13,0x24,0x31,0x72*/
    ret = Dcm_ProcessRequestTransferExit(
        OpStatus,
        transferRequestParameterRecord,
        transferRequestParameterRecordSize,
        transferResParameterRecord,
        &transferResponseParameterRecordSize,
        ErrorCode);
    if (DCM_E_PENDING == ret)
    {
        Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
    }

    /* check tx data length */
    if ((E_OK == ret) && ((1u + transferResponseParameterRecordSize) > pDcmChannelCfg->Dcm_DslBufferSize))
    {
        /*Pdu length is bigger than buffer size */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        DslInternal_InitDspProgramInfo();
        /*the processing is successful,assemble positive response*/
        Dcm_Channel[Offset] = 0x77; /*response SID*/
        pMsgContext->ResMaxDataLen = 1u + transferResponseParameterRecordSize;
        pMsgContext->ResDataLen = 1u + transferResponseParameterRecordSize;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#endif
