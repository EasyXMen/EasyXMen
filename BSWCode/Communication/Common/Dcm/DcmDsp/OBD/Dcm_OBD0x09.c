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
 *
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
/* PRQA S 3108-- */

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "OBD.h"
#include "Dcm_OBDInternal.h"
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
    P2CONST(Dcm_DspVehInfoDataType, TYPEDEF, DCM_CONST) DcmDspVehInfoData;
    const Dcm_DspVehInfoType* pVehInfo;
    uint8 VehInfoData_Num;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;

    /* check the massage length and format */
    if (Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen != DCM_UDS0X09_REQ_DATA_LENGTH_MIN)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = OBD_FindCfgIndexOfVehInfo(InfoType, &vehInfoCtrlIndex);
        pVehInfo = &(Dcm_DspCfg.pDcmDspVehInfo[vehInfoCtrlIndex]);
        VehInfoData_Num = pVehInfo->DcmDspVehInfoDataNum;
    }
    /* find VehInfo config index number */
    if ((E_OK == ret)
        && (((pVehInfo->DcmDspVehInfoNODIProvResp == TRUE) && (VehInfoData_Num == 1u))
            || (pVehInfo->DcmDspVehInfoNODIProvResp == FALSE)))
    {
        SchM_Enter_Dcm(Dcm_Channel);
        Dcm_Channel[*TxOffset] = InfoType;
        (*TxOffset) += 1u;
        Dcm_Channel[*TxOffset] = VehInfoData_Num;
        SchM_Exit_Dcm(Dcm_Channel);
        (*TxOffset) += 1u;
        DcmDspVehInfoData = pVehInfo->DcmDspVehInfoData;
        for (index = 0; (index < VehInfoData_Num) && (E_OK == ret); index++)
        {
            OBD_MemSet(txBuffer, 0x00, DCM_TEMP_BUFFER_LENGTH);
            if (*DcmDspVehInfoData->DcmGetInfoTypeValueFnc != NULL_PTR)
            {
                ret = (*DcmDspVehInfoData->DcmGetInfoTypeValueFnc)(OpStatus, txBuffer);
                if (E_OK == ret)
                {
                    /*idle*/
                }
                else if (DCM_E_PENDING == ret)
                {
                    SchM_Enter_Dcm(Dcm_MsgCtrl);
                    Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
                    SchM_Exit_Dcm(Dcm_MsgCtrl);
                }
                else
                {
                    /*send NRC = 0x12*/
                    *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
                    ret = E_NOT_OK;
                }
            }
            if (E_OK == ret)
            {
                SchM_Enter_Dcm(Dcm_Channel);
                Dcm_MemoryCopy(txBuffer, &Dcm_Channel[*TxOffset], DcmDspVehInfoData->DcmDspVehInfoDataSize);
                SchM_Exit_Dcm(Dcm_Channel);
                (*TxOffset) += (uint32)DcmDspVehInfoData->DcmDspVehInfoDataSize;
                DcmDspVehInfoData++;
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
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

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
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(Std_ReturnType, DCM_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
Dcm_OBD0x09(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
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
    Dcm_MsgContextType* pMsgContextt;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (Dcm_DslCfg.pDcmChannelCfg)[TxChannelCfgIndex].offset;
    pMsgContextt = &(Dcm_MsgCtrl[MsgCtrlId].MsgContext);
    /* check the massage length and format */
    if ((pMsgContextt->ReqDataLen < DCM_UDS0X09_REQ_DATA_LENGTH_MIN)
        || (pMsgContextt->ReqDataLen > DCM_UDS0X09_REQ_DATA_LENGTH_MAX))
    {
        ret = E_NOT_OK;
    }
    else
    {
        SchM_Enter_Dcm(Dcm_Channel);
        Dcm_Channel[Offset] = 0x49;
        SchM_Exit_Dcm(Dcm_Channel);
        TxOffset = Offset + 1u;
        InfoTypeNum = (uint8)pMsgContextt->ReqDataLen - 1u;
        for (iloop = 0; (iloop < InfoTypeNum) && (E_OK == ret); iloop++)
        {
            InfoType = pMsgContextt->pReqData[iloop + 1u];
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
                        SchM_Enter_Dcm(Dcm_Channel);
                        Dcm_Channel[TxOffset] = InfoType;
                        TxOffset += 1u;
                        Dcm_FillTo4bytes(&(Dcm_Channel[TxOffset]), &SupportBuffer);
                        SchM_Exit_Dcm(Dcm_Channel);
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
    if ((E_OK == ret) && ((TxOffset - Offset) > Dcm_DslCfg.pDcmChannelCfg[TxChannelCfgIndex].Dcm_DslBufferSize))
    {
        /*Pdu length is bigger than buffer size,ignore the request message */
        DslInternal_ResetResource(ProtocolCtrlId);
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        SchM_Enter_Dcm(Dcm_MsgCtrl);
        pMsgContextt->ResMaxDataLen = (Dcm_MsgLenType)TxOffset - Offset;
        pMsgContextt->ResDataLen = (Dcm_MsgLenType)TxOffset - Offset;
        pMsgContextt->pResData = &Dcm_Channel[Offset];
        SchM_Exit_Dcm(Dcm_MsgCtrl);
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X09_ENABLED*/
