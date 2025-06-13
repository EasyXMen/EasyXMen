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
**  FILENAME    : Dcm_OBD0x09.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x09 API definitions                                 **
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
 * Brief               <OBD service (0x09): Request Vehicle Information>
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
#if (STD_ON == DCM_OBD_SERVICE0X09_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE) Dcm_OBD0x09_SubDeal(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    uint8 InfoType,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint32, AUTOMATIC, DCM_VAR) TxOffset,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 txBuffer[DCM_TEMP_BUFFER_LENGTH] = {0};
    uint8 vehInfoCtrlIndex;
    uint8 index;
    const Dcm_DspVehInfoDataType* DcmDspVehInfoData;
    const Dcm_DspVehInfoType* pDcmDspVehInfo = DcmPbCfgPtr->pDcmDspCfg->pDcmDspVehInfo;
    uint8 DcmDspVehInfoDataNum;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

    /* check the massage length and format */
    if (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen != DCM_UDS0X09_REQ_DATA_LENGTH_MIN)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = OBD_FindCfgIndexOfVehInfo(InfoType, &vehInfoCtrlIndex);
    }
    /* find VehInfo config index number */
    if ((E_OK == ret)
        && (((pDcmDspVehInfo[vehInfoCtrlIndex].DcmDspVehInfoNODIProvResp == TRUE)
             && (pDcmDspVehInfo[vehInfoCtrlIndex].DcmDspVehInfoDataNum == 1u))
            || (pDcmDspVehInfo[vehInfoCtrlIndex].DcmDspVehInfoNODIProvResp == FALSE)))
    {
        DcmDspVehInfoDataNum = pDcmDspVehInfo[vehInfoCtrlIndex].DcmDspVehInfoDataNum;
        Dcm_Channel[*TxOffset] = InfoType;
        (*TxOffset) += 1u;
        Dcm_Channel[*TxOffset] = DcmDspVehInfoDataNum;
        (*TxOffset) += 1u;
        DcmDspVehInfoData = pDcmDspVehInfo[vehInfoCtrlIndex].DcmDspVehInfoData;
        for (index = 0; (index < DcmDspVehInfoDataNum) && (E_OK == ret); index++)
        {
            Dcm_MemSet(txBuffer, 0x00, DCM_TEMP_BUFFER_LENGTH);
            if (*DcmDspVehInfoData->DcmGetInfoTypeValueFnc != NULL_PTR)
            {
                ret = (*DcmDspVehInfoData->DcmGetInfoTypeValueFnc)(OpStatus, txBuffer);
                if (E_OK == ret)
                {
                    /*idle*/
                }
                else if (DCM_E_PENDING == ret)
                {
                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                }
                else
                {
                    /*send NRC = 0x12*/
                    *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
                    ret = E_NOT_OK;
                }
                if (E_OK == ret)
                {
                    Dcm_MemoryCopy(txBuffer, &Dcm_Channel[*TxOffset], DcmDspVehInfoData->DcmDspVehInfoDataSize);
                    (*TxOffset) += (uint32)DcmDspVehInfoData->DcmDspVehInfoDataSize;
                    DcmDspVehInfoData++;
                }
            }
        }
    }
    else
    {
        /* In case the responsibility is on provider side
            (DcmDspVehInfoNODIProvResp is set to TRUE), only one DcmDspVehInfoData
            container shall be allowed.
            there is no supported VehInfotype,ignore the request message */
        /* there is no supported VehInfotype,ignore the request message */
        ret = E_NOT_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               <OBD service (0x09): Request Vehicle Information>
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
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_OBD0x09(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Std_ReturnType ret = E_OK;
    uint8 InfoTypeNum;
    uint8 InfoType;
    uint8 iloop;
    uint32 SupportBuffer;
    uint32 TxOffset;
    uint8 MixInfoType = NONE_PID;
    uint8 noFindInfoTypeNum = 0;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].offset;
    /* check the massage length and format */
    if ((Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen < DCM_UDS0X09_REQ_DATA_LENGTH_MIN)
        || (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen > DCM_UDS0X09_REQ_DATA_LENGTH_MAX))
    {
        ret = E_NOT_OK;
    }
    else
    {
        Dcm_Channel[Offset] = 0x49;
        TxOffset = Offset + 1u;
        InfoTypeNum = (uint8)Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - 1u;
        for (iloop = 0; (iloop < InfoTypeNum) && (E_OK == ret); iloop++)
        {
            InfoType = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[iloop + 1u];
            switch (InfoType)
            {
            case 0x00u:
            case 0x20u:
            case 0x40u:
            case 0x60u:
            case 0x80u:
            case 0xA0u:
            case 0xC0u:
            case 0xE0u:
                if (MixInfoType == NORMAL_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    OBD_SetAvailabilityInfoTypeValue(InfoType, &SupportBuffer);
                    if (SupportBuffer != 0u)
                    {
                        Dcm_Channel[TxOffset] = InfoType;
                        TxOffset += 1u;
                        Dcm_FillTo4bytes(&(Dcm_Channel[TxOffset]), &SupportBuffer);
                        TxOffset += 4u;
                    }
                    else
                    {
                        noFindInfoTypeNum++;
                    }
                    MixInfoType = SUPPORT_REQUEST;
                }
                break;
            default:
                if (MixInfoType == SUPPORT_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    ret = Dcm_OBD0x09_SubDeal(OpStatus, ProtocolCtrlId, InfoType, &TxOffset, ErrorCode);
                }
                MixInfoType = NORMAL_REQUEST;
                break;
            }
        }
        if (noFindInfoTypeNum == InfoTypeNum)
        {
            /* ignore the request message */
            ret = E_NOT_OK;
        }
    }
    /* check tx data length */
    if ((E_OK == ret)
        && ((TxOffset - Offset) > DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize))
    {
        /*Pdu length is bigger than buffer size,ignore the request message */
        DslInternal_ResetResource(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = TxOffset - Offset;
        Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen = TxOffset - Offset;
        Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X09_ENABLED*/
