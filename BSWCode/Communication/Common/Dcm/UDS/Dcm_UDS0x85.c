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
#if (STD_ON == DCM_UDS_SERVICE0X85_ENABLED)
#include "Dem_Dcm.h"

/****************************************************************
                 UDS:ControlDTCSetting (85 hex) service
 ***************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0X85ServiceConditionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 MsgCtrlId;
    Std_ReturnType ret = E_OK;
#if (STD_ON == DCM_DTC_SETTING_CONTROL_OPTION_RECORD_ENABLED)
    const Dcm_DspControlDTCSettingType* pDcmDspControlDTCSetting = DcmPbCfgPtr->pDcmDspCfg->pDcmDspControlDTCSetting;
#endif
    Dcm_MsgLenType ReqDataLen;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    ReqDataLen = Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen;

    if ((DCM_UDS0X85_ON != Dcm_MsgCtrl[MsgCtrlId].Subfunction)
        && (DCM_UDS0X85_OFF != Dcm_MsgCtrl[MsgCtrlId].Subfunction))
    {
        /*the required sub-function is not supported,send NRC 0x12*/
        *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        ret = E_NOT_OK;
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
        /*session check,check whether the current session supports the request service*/
        ret = DsdInternal_SubSesCheck(ProtocolCtrlId, SID_CONTROL_DTC_SETTING, ErrorCode);
    }
#endif
#if (STD_ON == DCM_SECURITY_FUNC_ENABLED)
    if (E_OK == ret)
    {
        /*security check,check whether the current security supports the request service*/
        ret = DsdInternal_SubSecurityCheck(ProtocolCtrlId, SID_CONTROL_DTC_SETTING, ErrorCode);
    }
#endif
    if (E_OK == ret)
    {
#if (STD_ON == DCM_DTC_SETTING_CONTROL_OPTION_RECORD_ENABLED)
        if ((pDcmDspControlDTCSetting != NULL_PTR)
            && (TRUE == pDcmDspControlDTCSetting->DcmSupportDTCSettingControlOptionRecord))
        {
            if ((0u != ((ReqDataLen - DCM_UDS0X85_REQ_DATA_MINLENGTH) % 3u))
                || (ReqDataLen < DCM_UDS0X85_REQ_DATA_DTC_MINLENGTH))
            {
                /*the length of massage is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
        }
        else
#endif
        {
            if ((ReqDataLen) != DCM_UDS0X85_REQ_DATA_MINLENGTH)
            {
                /*the length of massage is not correct,send NRC 0x13*/
                *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                ret = E_NOT_OK;
            }
        }
    }
    return ret;
}

/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x85(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432,1532-- */ /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint8 subFunc;
    uint32 Offset;
    Std_ReturnType ret;
    Dem_ReturnControlDTCSettingType returnCtrlDtcSetting = DEM_CONTROL_DTC_SETTING_OK;
#if (STD_ON == DCM_DTC_SETTING_CONTROL_OPTION_RECORD_ENABLED)
    uint32 numOfDTC;
    uint32 groupOfDTC;
    uint32 index;
#endif
    const Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;

    DCM_UNUSED(OpStatus);
    ret = Dcm_Uds0X85ServiceConditionCheck(ProtocolCtrlId, ErrorCode);

    if (E_OK == ret)
    {
        /*get the sub-function*/
        MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
        pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
        subFunc = pMsgContext->pReqData[1];

        /*the request message does not contain DTCSettingControlOptionRecord*/
        if (DCM_UDS0X85_REQ_DATA_MINLENGTH == pMsgContext->ReqDataLen)
        {
            /*check the sub-function*/
            if (DCM_UDS0X85_ON == subFunc)
            {
                /*invoke the corresponding API provided by DEM*/
                returnCtrlDtcSetting = Dem_DcmEnableDTCSetting(DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_KIND_ALL_DTCS);
                if (DEM_CONTROL_DTC_SETTING_OK == returnCtrlDtcSetting)
                {
                    (void)SchM_Switch_DcmControlDTCSetting(RTE_MODE_DcmControlDTCSetting_ENABLEDTCSETTING);
                }
            }
            if (DCM_UDS0X85_OFF == subFunc)
            {
                /*invoke the corresponding API provided by DEM*/
                returnCtrlDtcSetting = Dem_DcmDisableDTCSetting(DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_KIND_ALL_DTCS);
            }
        }
#if (STD_ON == DCM_DTC_SETTING_CONTROL_OPTION_RECORD_ENABLED)
        else if (0u == ((pMsgContext->ReqDataLen - DCM_UDS0X85_REQ_DATA_MINLENGTH) % 3u))
        {
            /*the request message contains DTCSettingControlOptionRecord*/
            numOfDTC = (pMsgContext->ReqDataLen - 2UL) / 3UL;
            /*check the sub-function*/
            if (DCM_UDS0X85_ON == subFunc)
            {
                /*get and assemble DTC, high byte first*/
                for (index = 0; (index < numOfDTC) && (DEM_CONTROL_DTC_SETTING_OK == returnCtrlDtcSetting); index++)
                {
                    groupOfDTC = (((uint32)(pMsgContext->pReqData[2u + (3u * index)])) << 16u)
                                 | (((uint32)(pMsgContext->pReqData[3u + (3u * index)])) << 8u)
                                 | ((uint32)(pMsgContext->pReqData[4u + (3u * index)]));
                    /*invoke the corresponding API provided by DEM*/
                    returnCtrlDtcSetting = Dem_DcmEnableDTCSetting(groupOfDTC, DEM_DTC_KIND_ALL_DTCS);
                }
            }
            /*get and assemble DTC, high byte first*/
            for (index = 0; (index < numOfDTC) && (DCM_UDS0X85_OFF == subFunc)
                            && (DEM_CONTROL_DTC_SETTING_OK == returnCtrlDtcSetting);
                 index++)
            {
                groupOfDTC = (((uint32)(pMsgContext->pReqData[2u + (3u * index)])) << 16u)
                             | (((uint32)(pMsgContext->pReqData[3u + (3u * index)])) << 8u)
                             | ((uint32)(pMsgContext->pReqData[4u + (3u * index)]));
                /*invoke the corresponding API provided by DEM*/
                returnCtrlDtcSetting = Dem_DcmDisableDTCSetting(groupOfDTC, DEM_DTC_KIND_ALL_DTCS);
                if (DEM_CONTROL_DTC_SETTING_OK == returnCtrlDtcSetting)
                {
                    (void)SchM_Switch_DcmControlDTCSetting(RTE_MODE_DcmControlDTCSetting_DISABLEDTCSETTING);
                }
            }
        }
#endif /* STD_ON == DCM_DTC_SETTING_CONTROL_OPTION_RECORD_ENABLED */
        else
        {
            /*Nothing to do ,shall never in*/
        }
        switch (returnCtrlDtcSetting)
        {
        case DEM_CONTROL_DTC_SETTING_OK:
            /*the processing is successful,assemble positive response*/
            TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
            TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
            pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
            Offset = pDcmChannelCfg->offset;
            /* check tx data length */
            if ((0x02u) > (pDcmChannelCfg->Dcm_DslBufferSize))
            {
                /*Pdu length is bigger than buffer size,ignore the request message */
                *ErrorCode = DCM_E_RESPONSETOOLONG;
                ret = E_NOT_OK;
            }
            else
            {
                Dcm_Channel[Offset] = 0xC5;         /*response SID*/
                Dcm_Channel[Offset + 1u] = subFunc; /*echo of 0 - 6 bits of sub-function*/
                Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResMaxDataLen = 0x02;
                Dcm_MsgCtrl[MsgCtrlId].MsgContext.ResDataLen = 0x02;
                Dcm_MsgCtrl[MsgCtrlId].MsgContext.pResData = &Dcm_Channel[Offset];
                DsdInternal_ProcessingDone(ProtocolCtrlId);
            }
            break;
        case DEM_CONTROL_DTC_SETTING_N_OK:
            /*the processing is not successful,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
            break;
        case DEM_CONTROL_DTC_WRONG_DTCGROUP:
        default:
            /*DTCSettingControlOptionRecord error,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
            break;
        }
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
