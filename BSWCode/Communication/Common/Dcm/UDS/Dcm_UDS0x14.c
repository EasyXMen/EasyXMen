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
#if (STD_ON == DCM_UDS_SERVICE0X14_ENABLED)
#include "Dem_Dcm.h"

/****************************************************************
         UDS:ClearDiagnosticInformation (14 hex) service
 ***************************************************************/

/********************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x14_ConditionCheck(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 MsgCtrlId;
    uint32 groupOfDTC;
    Dem_ReturnClearDTCType returnClearDTC;
    Std_ReturnType ret = E_OK;
    const Dcm_DspClearDTCType* pDcmDspClearDTC = DcmPbCfgPtr->pDcmDspCfg->pDcmDspClearDTC;
    const Dcm_MsgContextType* pMsgContext;

    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /*check the massage length*/
    if (DCM_UDS0X14_REQ_DATA_LENGTH != pMsgContext->ReqDataLen)
    {
        /*the length of massage is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    if ((E_OK == ret) && (OpStatus == DCM_INITIAL))
    {
        /*get and assemble DTC, high byte first*/
        groupOfDTC = (((uint32)(pMsgContext->pReqData[1])) << 16u) | (((uint32)(pMsgContext->pReqData[2])) << 8u)
                     | ((uint32)(pMsgContext->pReqData[3]));
        returnClearDTC = Dem_DcmCheckClearParameter(groupOfDTC, DEM_DTC_FORMAT_UDS, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        switch (returnClearDTC)
        {
        case DEM_CLEAR_PENDING:
            ret = DCM_E_PENDING;
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            break;
        case DEM_CLEAR_WRONG_DTC:
            /*Server does not support the required groupOfDTC,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
            break;
        case DEM_CLEAR_FAILED:
            /*internal conditions within the server prevent the
                clearing of DTC related information stored in the server,sent NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
            break;
        case DEM_CLEAR_OK:
            if ((pDcmDspClearDTC != NULL_PTR) && (pDcmDspClearDTC->DcmDsp_ClearDTCCheckFnc != NULL_PTR))
            {
                ret = pDcmDspClearDTC->DcmDsp_ClearDTCCheckFnc(groupOfDTC, ErrorCode);
            }
            break;
        default:
            /*idle*/
            break;
        }
    }
    return ret;
}

/********************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x14(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    uint32 groupOfDTC;
    Dem_ReturnClearDTCType returnClearDTC;
    Std_ReturnType ret;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;

    /*if the required protocol is configuted,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /*get and assemble DTC, high byte first*/
    groupOfDTC =
        ((((uint32)pMsgContext->pReqData[1]) << 16u) | ((uint32)(pMsgContext->pReqData[2]) << 8u)
         | (uint32)(pMsgContext->pReqData[3]));
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
    ret = Dcm_UDS0x14_ConditionCheck(OpStatus, ProtocolCtrlId, ErrorCode);
    if (E_OK == ret)
    {
        /*invoke the corresponding API provided by DEM*/
        returnClearDTC = Dem_DcmClearDTC(groupOfDTC, DEM_DTC_FORMAT_UDS, DEM_DTC_ORIGIN_PRIMARY_MEMORY);
        switch (returnClearDTC)
        {
        case DEM_CLEAR_OK:
            /*clear is successful,assemble and send the positive response*/
            /* check tx data length */
            if ((0x01u) > pDcmChannelCfg->Dcm_DslBufferSize)
            {
                /*Pdu length is bigger than buffer size,ignore the request message */
                *ErrorCode = DCM_E_RESPONSETOOLONG;
                ret = E_NOT_OK;
            }
            if (E_OK == ret)
            {
                Dcm_Channel[Offset] = 0x54; /*response SID*/
                pMsgContext->ResMaxDataLen = 0x01;
                pMsgContext->ResDataLen = 0x01;
                pMsgContext->pResData = &Dcm_Channel[Offset];
                DsdInternal_ProcessingDone(ProtocolCtrlId);
            }
            break;
        case DEM_CLEAR_WRONG_DTC:
            /*Server does not support the required groupOfDTC,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
            break;
        case DEM_CLEAR_PENDING:
            ret = DCM_E_PENDING;
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            break;
        case DEM_CLEAR_MEMORY_ERROR:
            *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;
            ret = E_NOT_OK;
            break;
        case DEM_CLEAR_BUSY:
        case DEM_CLEAR_FAILED:
            /*internal conditions within the server prevent the
                clearing of DTC related information stored in the server,sent NRC 0x22*/
        default:
            /*the unexpected return value is present,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
            break;
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /* STD_ON == DCM_UDS_SERVICE0X14_ENABLED */
