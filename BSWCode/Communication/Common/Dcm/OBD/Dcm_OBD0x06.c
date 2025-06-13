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
**  FILENAME    : Dcm_OBD0x06.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x06 API definitions                                 **
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
 * Brief               <OBD service (0x06): Request On-Board Monitoring Test-results
 *                                           for Specific Monitored Systems>
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
#if (STD_ON == DCM_OBD_SERVICE0X06_ENABLED)
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_OBD0x06(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode) /* PRQA S 3673 */ /* MISRA Rule 8.13 */
/* PRQA S 3432,1532-- */                                                                 /* MISRA Rule 20.7,8.7 */
{
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    uint8 MidNum;
    uint8 iloop;
    uint8 Mid;
    uint32 SupportBuffer = 0;
    uint32 TxOffset;
    uint8 noFindMidNum = 0;
#ifdef DEM_OBD_SUPPORT
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
    uint8 index;
    uint8 TidNum;
    uint8 TIDvalue;
    uint8 UaSID;
    uint16 Testvalue;
    uint16 Lowlimvalue;
    uint16 Upplimvalue;
#endif
#endif
    Std_ReturnType ret = E_OK;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslCfgType* pDcmDslCfg;
    const Dcm_DslBufferType* pDcmChannelCfg;

    DCM_UNUSED(OpStatus);
    DCM_UNUSED(ErrorCode);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    pDcmChannelCfg = &pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    if ((pMsgContext->ReqDataLen < DCM_UDS0X06_REQ_DATA_LENGTH_MIN)
        || (pMsgContext->ReqDataLen > DCM_UDS0X06_REQ_DATA_LENGTH_MAX))
    {
        /* requested message format is wrong,ignore the request message */
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        MidNum = (uint8)pMsgContext->ReqDataLen - 1u;
        Dcm_Channel[Offset] = 0x46;
        TxOffset = Offset + 1u;
        for (iloop = 0; iloop < MidNum; iloop++)
        {
            Mid = pMsgContext->pReqData[iloop + 1u];
            switch (Mid)
            {
            case 0x00u:
            case 0x20u:
            case 0x40u:
            case 0x60u:
            case 0x80u:
            case 0xA0u:
            case 0xC0u:
            case 0xE0u:
#ifdef DEM_OBD_SUPPORT
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
                (void)Dem_DcmGetAvailableOBDMIDs(Mid, &SupportBuffer);
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */
#endif /* DEM_OBD_SUPPORT */
                if (SupportBuffer != 0u)
                {
                    Dcm_Channel[TxOffset] = Mid;
                    TxOffset += 1u;
                    Dcm_FillTo4bytes(&(Dcm_Channel[TxOffset]), &SupportBuffer);
                    TxOffset += 4u;
                }
                else
                {
                    noFindMidNum++;
                }
                break;
            default:
                if (pMsgContext->ReqDataLen != DCM_UDS0X06_REQ_DATA_LENGTH_MIN)
                {
                    /* requested message format is wrong,ignore the request message */
                    ret = E_NOT_OK;
                }
                else
                {
#ifdef DEM_OBD_SUPPORT
#if (DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT)
                    (void)Dem_DcmGetNumTIDsOfOBDMID(Mid, &TidNum);
                    if (TidNum != 0u)
                    {
                        for (index = 0; index < TidNum; index++)
                        {
                            (void)Dem_DcmGetDTRData(
                                Mid,
                                index,
                                &TIDvalue,
                                &UaSID,
                                &Testvalue,
                                &Lowlimvalue,
                                &Upplimvalue);
                            Dcm_Channel[TxOffset] = Mid;
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = TIDvalue;
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = UaSID;
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)((Testvalue & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)(Testvalue & OBD_DATA_LSB_MASK);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)((Lowlimvalue & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)(Lowlimvalue & OBD_DATA_LSB_MASK);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)((Upplimvalue & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                            TxOffset += 1u;
                            Dcm_Channel[TxOffset] = (uint8)(Upplimvalue & OBD_DATA_LSB_MASK);
                            TxOffset += 1u;
                        }
                    }
                    else
#endif /* DEM_OBD_SUPPORT != DEM_OBD_NO_OBD_SUPPORT */
#endif /* DEM_OBD_SUPPORT */
                    {
                        /* there is no supported PID,ignore the request message */
                        ret = E_NOT_OK;
                    }
                }
                break;
            }
        }
        /* can not find supported PIDs,ignose the requested message */
        if (noFindMidNum == MidNum)
        {
            /* there is no supported PID,ignore the request message */
            ret = E_NOT_OK;
        }
    }
    /* check tx data length */
    if ((E_OK == ret) && ((TxOffset - Offset) > pDcmChannelCfg->Dcm_DslBufferSize))
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
#endif /*STD_ON == DCM_OBD_SERVICE0X06_ENABLED*/
