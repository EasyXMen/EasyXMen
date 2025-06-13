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
********************************************************************************
**                                                                            **
**  FILENAME    : Dcm_OBD0x08.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x08 API definitions                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Dcm_Internal.h"

/******************************************************************************/
/*
 * Brief               <OBD service (0x08): Request Control of On-Board System, Test or Component>
 * ServiceId           <none>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Reentrant>
 * Param-Name[in]      <ProtocolCtrlId:current protocol control identifier>
 * Param-Name[out]     <none>
 * Param-Name[in/out]  <none>
 * Return              <Std_ReturnType>
 * PreCondition        <none>
 * CallByAPI           <APIName>
 */
/******************************************************************************/
#if (STD_ON == DCM_OBD_SERVICE0X08_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_OBD0x08(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    uint8 iloop;
    uint8 TidNum;
    uint8 Tid;
    uint8 MixTid = 0;
    uint32 TxOffset;
    uint32 SupportBuffer;
    uint8 noFindTidNum = 0;
    uint8 CtrlIndex;
    uint8 inputbuffer[7] = {0};
    uint8 outputbuffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint8 counter = 0;
    Std_ReturnType ret = E_OK;
    boolean ProcessOn = TRUE;
    const Dcm_DspRequestControlType* pDcmDspRequestControl = DcmPbCfgPtr->pDcmDspCfg->pDcmDspRequestControl;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslCfgType* pDcmDslCfg;
    const Dcm_DslBufferType* pDcmChannelCfg;

    DCM_UNUSED(OpStatus);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    pDcmChannelCfg = &pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /*check the massage length*/
    if ((pMsgContext->ReqDataLen < DCM_UDS0X08_REQ_DATA_LENGTH_MIN)
        || (pMsgContext->ReqDataLen > DCM_UDS0X08_REQ_DATA_LENGTH_MAX))
    {
        /* requested message format is wrong,ignore the request message */
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        TidNum = (uint8)pMsgContext->ReqDataLen - 1u;
        Dcm_Channel[Offset] = 0x48;
        TxOffset = Offset + 1u;
        for (iloop = 0; (iloop < TidNum) && (E_OK == ret) && (ProcessOn == TRUE); iloop++)
        {
            Tid = pMsgContext->pReqData[counter + 1u];
            switch (Tid)
            {
            case 0x00u:
            case 0x20u:
            case 0x40u:
            case 0x60u:
            case 0x80u:
            case 0xA0u:
            case 0xC0u:
            case 0xE0u:
                if (MixTid == NORMAL_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    OBD_CheckSupportedTIDs(Tid, &SupportBuffer);
                    if (SupportBuffer != 0u)
                    {
                        Dcm_Channel[TxOffset] = Tid;
                        TxOffset += 1u;
                        Dcm_FillTo4bytes(&(Dcm_Channel[TxOffset]), &SupportBuffer);
                        TxOffset += 4u;
                    }
                    else
                    {
                        noFindTidNum++;
                    }
                    MixTid = SUPPORT_REQUEST;
                    counter++;
                }
                break;
            default:
                if (MixTid == SUPPORT_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    if (E_OK == OBD_FindCfgIndexOfTestID(Tid, &CtrlIndex))
                    {
                        Dcm_MemoryCopy(
                            &(pMsgContext->pReqData[1u]),
                            inputbuffer,
                            pDcmDspRequestControl[CtrlIndex].DcmDspRequestControlInBufferSize);
                        if (pDcmDspRequestControl[CtrlIndex].DcmDspRequestControlInBufferSize
                            == (pMsgContext->ReqDataLen - 2u))
                        {
                            if (E_OK == Rte_RequestControl(Tid, inputbuffer, outputbuffer))
                            {
                                Dcm_Channel[TxOffset] = Tid;
                                TxOffset += 1u;
                                Dcm_MemoryCopy(
                                    outputbuffer,
                                    &Dcm_Channel[TxOffset],
                                    pDcmDspRequestControl[CtrlIndex].DcmDspRequestControlOutBufferSize);
                                TxOffset += pDcmDspRequestControl[CtrlIndex].DcmDspRequestControlOutBufferSize;
                                ProcessOn = FALSE;
                            }
                            else
                            {
                                /* the conditions of the system are not proper to run,send NRC
                                 * 0x22*/
                                *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
                                ret = E_NOT_OK;
                            }
                        }
                        else
                        {
                            /* there is only one TID can be done,ignore the request message with
                             * more TIDs*/
                            ret = E_NOT_OK;
                        }
                    }
                    else
                    {
                        /* there is no supported VehInfotype,ignore the request message */
                        ret = E_NOT_OK;
                    }
                    MixTid = NORMAL_REQUEST;
                }
                break;
            }
        }
        if (noFindTidNum == TidNum)
        {
            /* ignore the request message */
            ret = E_NOT_OK;
        }
    }

    /* check tx data length */
    if ((E_OK == ret) && ((TxOffset - Offset) > (pDcmChannelCfg->Dcm_DslBufferSize)))
    {
        /*Pdu length is bigger than buffer size,ignore the request message */
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        pMsgContext->ResMaxDataLen = TxOffset - Offset;
        pMsgContext->ResDataLen = TxOffset - Offset;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X08_ENABLED*/
