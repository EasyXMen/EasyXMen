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
**  FILENAME    : Dcm_OBD0x03.c                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : tao.yu                                                      **
**  Vendor      :                                                             **
**  DESCRIPTION : DCM OBD0x03 API definitions                                 **
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
#if (STD_ON == DCM_OBD_SERVICE0X03_ENABLED)
#include "Dem_Dcm.h"
/******************************************************************************/
/*
 * Brief               <OBD service (0x03):  Request emission-related diagnostic trouble codes>
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
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_OBD0x03(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode) /* PRQA S 3673 */ /* MISRA Rule 8.13 */
/* PRQA S 3432,1532-- */                                                                 /* MISRA Rule 20.7,8.7 */
{
    uint32 dtc = 0;
    Dem_UdsStatusByteType dtcStatus = 0;
    uint32 TxOffset;
    uint16 DTCNumber = 0;
    uint8 MsgCtrlId;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Std_ReturnType ret = E_OK;
    Dem_ReturnSetFilterType DemRet = DEM_WRONG_FILTER;
    const Dcm_DslCfgType* pDcmDslCfg;
    const Dcm_DslBufferType* pDcmChannelCfg;
    Dcm_MsgContextType* pMsgContext;

    DCM_UNUSED(OpStatus);
    DCM_UNUSED(ErrorCode);
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmDslCfg = DcmPbCfgPtr->pDcmDslCfg;
    pDcmChannelCfg = &pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;
    TxOffset = Offset + 2u;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    /*check the massage length*/
    if (pMsgContext->ReqDataLen != DCM_UDS0X03_REQ_DATA_LENGTH)
    {
        ret = E_NOT_OK;
    }
    else
    {
        DemRet = Dem_DcmSetDTCFilter(
            DEM_UDS_STATUS_CDTC,
            DEM_DTC_KIND_EMISSION_REL_DTCS,
            DEM_DTC_FORMAT_OBD,
            DEM_DTC_ORIGIN_PRIMARY_MEMORY,
            DEM_FILTER_WITH_SEVERITY_NO,
            DEM_SEVERITY_NO_SEVERITY,
            DEM_FILTER_FOR_FDC_NO);
    }
    if ((E_OK == ret) && (DEM_FILTER_ACCEPTED == DemRet))
    {
        if (DEM_NUMBER_OK == Dem_DcmGetNumberOfFilteredDTC(&DTCNumber))
        {
            for (uint16 index = 0; index < DTCNumber; index++)
            {
                if (DEM_FILTERED_OK == Dem_DcmGetNextFilteredDTC(&dtc, &dtcStatus))
                {
                    if ((TxOffset - Offset) > (pDcmChannelCfg->Dcm_DslBufferSize))
                    {
                        /*Pdu length is bigger than buffer size,ignore the request message */
                        ret = E_NOT_OK;
                        break;
                    }
                    else
                    {
                        Dcm_Channel[TxOffset] = (uint8)((dtc & (OBD_DATA_LSB_MASK << 8u)) >> 8u);
                        TxOffset++;
                        Dcm_Channel[TxOffset] = (uint8)(dtc & OBD_DATA_LSB_MASK);
                        TxOffset++;
                    }
                }
            }
        }
        if (E_OK == ret)
        {
            Dcm_Channel[Offset] = 0x43;
            Dcm_Channel[Offset + 1u] = (uint8)DTCNumber;
            pMsgContext->ResMaxDataLen = TxOffset - Offset;
            pMsgContext->ResDataLen = TxOffset - Offset;
            pMsgContext->pResData = &Dcm_Channel[Offset];
            DsdInternal_ProcessingDone(ProtocolCtrlId);
        }
    }
    else
    {
        /*the condition is not correct,ignore the request message */
        ret = E_NOT_OK;
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif /*STD_ON == DCM_OBD_SERVICE0X03_ENABLED*/
