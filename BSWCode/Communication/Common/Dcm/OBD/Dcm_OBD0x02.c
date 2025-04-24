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
********************************************************************************
**                                                                            **
**  FILENAME    : Dcm_OBD0x02.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x02 API definitions                                 **
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
 * Brief               <OBD service (0x02):  Request Power Train FreezeFrame Data>
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
#if (STD_ON == DCM_OBD_SERVICE0X02_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_OBD0x02(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode) /* PRQA S 3673 */ /* MISRA Rule 8.13 */
/* PRQA S 3432,1532-- */                                                                 /* MISRA Rule 20.7,8.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    uint8 iloop;
    uint8 PidNum;
    uint8 Pid;
    uint8 frame;
    uint8 Sumframe = 0;
    uint8 MixPid = 0;
    uint32 SupportBuffer;
    uint32 TxOffset;
    uint8 noFindPidNum = 0;
    uint32 DTC;
    OBD_ReadPidValuesTypes OBD_ReadPid;
    Dcm_MsgContextType* pMsgContext;

    DCM_UNUSED(OpStatus);
    DCM_UNUSED(ErrorCode);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex].offset;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /* check the massage length and format */
    if ((pMsgContext->ReqDataLen < DCM_UDS0X02_REQ_DATA_LENGTH_MIN)
        || (pMsgContext->ReqDataLen > DCM_UDS0X02_REQ_DATA_LENGTH_MAX) || (0u == (pMsgContext->ReqDataLen % 2u)))
    {
        /*the format of massage is not correct,ignore the request message */
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        PidNum = (uint8)pMsgContext->ReqDataLen - 1u;
        Dcm_Channel[Offset] = 0x42;
        TxOffset = Offset + 1u;
        for (iloop = 0; (E_OK == ret) && (iloop < PidNum) && (Sumframe == 0u); (iloop += 2u))
        {
            Pid = pMsgContext->pReqData[iloop + 1u];
            frame = pMsgContext->pReqData[iloop + 2u];
            Sumframe = frame;
            switch (Pid)
            {
            case 0x00u:
            case 0x20u:
            case 0x40u:
            case 0x60u:
            case 0x80u:
            case 0xA0u:
            case 0xC0u:
            case 0xE0u:
                if (MixPid == NORMAL_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    OBD_CheckSupportedPIDs(Pid, &SupportBuffer, SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA);
                    if (SupportBuffer != 0u)
                    {
                        Dcm_Channel[TxOffset] = Pid;
                        TxOffset += 1u;
                        Dcm_Channel[TxOffset] = frame;
                        TxOffset += 1u;
                        Dcm_FillTo4bytes(&(Dcm_Channel[TxOffset]), &SupportBuffer);
                        TxOffset += 4u;
                    }
                    else
                    {
                        noFindPidNum++;
                    }
                    MixPid = SUPPORT_REQUEST;
                }
                break;
            case 0x02u:
                if (MixPid == SUPPORT_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    if (E_NOT_OK == Dem_DcmGetDTCOfOBDFreezeFrame(0u, &DTC, DEM_DTC_FORMAT_OBD))
                    {
                        DTC = 0x0000UL;
                    }
                    Dcm_Channel[TxOffset] = Pid;
                    TxOffset += 1u;
                    Dcm_Channel[TxOffset] = frame;
                    TxOffset += 1u;
                    /*take every 2 byte of DTC,and fill in Dcm_Channel*/
                    Dcm_Channel[TxOffset] = (uint8)((DTC & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                    TxOffset += 1u;
                    Dcm_Channel[TxOffset] = (uint8)(DTC & OBD_DATA_LSB_MASK);
                    TxOffset += 1u;
                    MixPid = NORMAL_REQUEST;
                }
                break;
            default:
                if (MixPid == SUPPORT_REQUEST)
                {
                    ret = E_NOT_OK;
                }
                else
                {
                    OBD_ReadPid.SID = SID_REQUEST_POWERTRAIN_FREEZE_FRAME_DATA;
                    OBD_ReadPid.pid = Pid;
                    OBD_ReadPid.UDSCall = FALSE;
                    ret = OBD_ReadPidValues(&OBD_ReadPid, &TxOffset, &noFindPidNum);
                    /* Request message is supported but data is currently not available,
                     * ignore the request message */
                    MixPid = NORMAL_REQUEST;
                }
                break;
            }
        }

        if (Sumframe != 0u)
        {
            /*the format of massage is not correct,ignore the request message */
            ret = E_NOT_OK;
        }

        /* can not find supported PIDs,ignose the requested message */
        if (noFindPidNum == (PidNum >> 1u))
        {
            ret = E_NOT_OK;
        }
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
#endif /*STD_ON == DCM_OBD_SERVICE0X02_ENABLED*/
