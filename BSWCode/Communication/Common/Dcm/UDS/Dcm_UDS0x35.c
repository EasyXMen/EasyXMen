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

/****************************************************************
     UDS: RequestUpload (35 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X35_ENABLED)
typedef struct
{
    uint32 MemoryAddress;
    uint32 Memorysize;
} Dcm_0x35Types;
/******************************/

/*************************************************************************/
/*
 * Brief               <UDS  0x34 service Memory Read Session Check>
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
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE) Dcm_0x35MemorySessionCheck(
    uint8 MemoryIdInfoIndex,
    uint8 MemoryRangeInfoIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspMemoryType* pDcmDspMemory = DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo =
        &(pDcmDspMemory->DcmDspMemoryIdInfo[MemoryIdInfoIndex].DcmDspReadMemoryRangeInfo[MemoryRangeInfoIndex]);
    uint8 RangeSessionLevelRefNum = pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeSessionLevelRefNum;
    const uint8* pSessionLevelRow = pDcmDspReadMemoryRangeInfo->pDcmDspReadMemoryRangeSessionLevelRow;

    if (RangeSessionLevelRefNum != 0u)
    {
        for (index = 0; (index < RangeSessionLevelRefNum); index++)
        {
            if (Dcm_MkCtrl.Dcm_ActiveSes == pSessionLevelRow[index])
            {
                ret = E_OK;
            }
        }
    }
    else
    {
        ret = E_OK;
    }
    if (E_NOT_OK == ret)
    {
        /*security check not ok for requested memory interval,send NRC 0x31*/
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        ret = E_NOT_OK;
    }
    return ret;
}

static FUNC(Std_ReturnType, DCM_CODE) Dcm_0x35MemorySecurityCheck(
    uint8 MemoryIdInfoIndex,
    uint8 MemoryRangeInfoIndex,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 index;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo;
    const Dcm_DspMemoryType* pDcmDspMemory = DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory;
    uint8 SecurityLevelRefNum;
    const uint8* pSecurityLevelRow;

    pDcmDspReadMemoryRangeInfo =
        &(pDcmDspMemory->DcmDspMemoryIdInfo[MemoryIdInfoIndex].DcmDspReadMemoryRangeInfo[MemoryRangeInfoIndex]);
    SecurityLevelRefNum = pDcmDspReadMemoryRangeInfo->DcmDspReadMemorySecurityLevelRefNum;
    pSecurityLevelRow = pDcmDspReadMemoryRangeInfo->pDcmDspReadMemorySecurityLevelRow;
    if (SecurityLevelRefNum != 0u)
    {
        for (index = 0; (index < SecurityLevelRefNum) && (E_NOT_OK == ret); index++)
        {
            if (Dcm_MkCtrl.Dcm_ActiveSec == pSecurityLevelRow[index])
            {
                ret = E_OK;
            }
        }
    }
    else
    {
        ret = E_OK;
    }
    if (E_NOT_OK == ret)
    {
        /*security check not ok for requested memory interval,send NRC 0x33*/
        *ErrorCode = DCM_E_SECURITYACCESSDENIED;
        ret = E_NOT_OK;
    }
    return ret;
}

/******************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_0x35MemoryRangeCheck(
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2CONST(Dcm_0x35Types, AUTOMATIC, DCM_VAR) Dcm_0x35Type,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryIdInfoIndex,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryRangeInfoIndex,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryAddressMatchNum)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 IdInfoIndex;
    uint8 RangeInfoIndex;
    Std_ReturnType ret = E_NOT_OK;
    const Dcm_DspReadMemoryRangeInfoType* pDcmDspReadMemoryRangeInfo;
    const Dcm_DspMemoryIdInfoType* pDcmDspMemoryIdInfo;
    const Dcm_DspMemoryType* pDcmDspMemory = DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory;
    uint8 DcmDspMemoryIdInfoNum = pDcmDspMemory->DcmDspMemoryIdInfoNum;
    uint8 DcmDspReadMemoryRangeInfoNum;

    for (IdInfoIndex = 0u; IdInfoIndex < DcmDspMemoryIdInfoNum; IdInfoIndex++)
    {
        pDcmDspMemoryIdInfo = &(pDcmDspMemory->DcmDspMemoryIdInfo[IdInfoIndex]);
        if (pDcmDspMemoryIdInfo != NULL_PTR)
        {
            DcmDspReadMemoryRangeInfoNum = pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfoNum;
            for (RangeInfoIndex = 0; RangeInfoIndex < DcmDspReadMemoryRangeInfoNum; RangeInfoIndex++)
            {
                pDcmDspReadMemoryRangeInfo = &(pDcmDspMemoryIdInfo->DcmDspReadMemoryRangeInfo[RangeInfoIndex]);
                if ((pDcmDspReadMemoryRangeInfo != NULL_PTR)
                    && (pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeLow <= Dcm_0x35Type->MemoryAddress)
                    && (pDcmDspReadMemoryRangeInfo->DcmDspReadMemoryRangeHigh
                        >= (Dcm_0x35Type->MemoryAddress + Dcm_0x35Type->Memorysize - 1u)))
                {
                    *MemoryIdInfoIndex = IdInfoIndex;
                    *MemoryRangeInfoIndex = RangeInfoIndex;
                    *MemoryAddressMatchNum += 1u;
                    ret = E_OK;
                }
            }
        }
    }
    return ret;
}

/***************************/
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x35ServiceConditionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint8 AddressAndLengthFormatIdentifier;
    uint8 MemoryAddressSize;
    uint8 MemoryLengthSize;
    uint8 DataFormatIdentifier;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;

    /*min-length check*/
    if (DCM_UDS0X35_REQ_DATA_MINLENGTH > pMsgContext->ReqDataLen)
    {
        /*the min length of message is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    /*if the required protocol is configured,get the index of runtime datum*/
    if (E_OK == ret)
    {
        AddressAndLengthFormatIdentifier = pMsgContext->pReqData[2];
        ret = DsdInternal_ServiceAddressAndLengthFormatIdentifierCheck(AddressAndLengthFormatIdentifier);
        if (E_NOT_OK == ret)
        {
            /*the processing is not successful,send NRC 0x31 */
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }
    if (E_OK == ret)
    {
        DataFormatIdentifier = pMsgContext->pReqData[1];
        ret = Dcm_DataFormatIdentifierCheck(DataFormatIdentifier);
        if (E_NOT_OK == ret)
        {
            /*the processing is not successful,send NRC 0x31 */
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
        }
    }
    if (E_OK == ret)
    {
        MemoryAddressSize = AddressAndLengthFormatIdentifier & 0x0Fu;
        MemoryLengthSize = (AddressAndLengthFormatIdentifier & 0xF0u) >> 4u;
        /*total length check*/
        if ((Dcm_MsgLenType)((Dcm_MsgLenType)3u + MemoryAddressSize + MemoryLengthSize) != pMsgContext->ReqDataLen)
        {
            /*the length of message is not correct,send NRC 0x13*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/*******************************/
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x35(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    uint8 MsgCtrlId;
    Std_ReturnType ret;
    uint8 AddressAndLengthFormatIdentifier;
    uint32 MemoryAddress = 0u;
    uint32 Memorysize = 0u;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    uint8 MemoryIdInfoIndex = 0u;
    uint8 MemoryRangeInfoIndex = 0u;
    uint8 MemoryAddressSize;
    uint8 MemoryLengthSize;
    uint8 index;
    uint8 MemoryAddressMatchNum = 0;
    uint8 DataFormatIdentifier;
    uint32 BlockLength = 0;
    Dcm_0x35Types Dcm_0x35Type;
    Dcm_MsgContextType* pMsgContext;
    const Dcm_DslBufferType* pDcmChannelCfg;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    ret = Dcm_Uds0x35ServiceConditionCheck(ProtocolCtrlId, ErrorCode);

    if (E_OK == ret)
    {
        AddressAndLengthFormatIdentifier = pMsgContext->pReqData[2];
        MemoryAddressSize = AddressAndLengthFormatIdentifier & 0x0Fu;
        MemoryLengthSize = (AddressAndLengthFormatIdentifier & 0xF0u) >> 4u;
        /*caculate the MemoryAddress of the request message*/
        for (index = 0u; index < MemoryAddressSize; index++)
        {
            MemoryAddress = MemoryAddress << 8u;
            MemoryAddress = MemoryAddress | (uint32)pMsgContext->pReqData[3u + index];
        }
        for (index = 0u; index < MemoryLengthSize; index++)
        {
            Memorysize = Memorysize << 8u;
            Memorysize = Memorysize | (uint32)pMsgContext->pReqData[3u + MemoryAddressSize + index];
        }
        /*MemoryAddress Range Check*/
        Dcm_0x35Type.MemoryAddress = MemoryAddress;
        Dcm_0x35Type.Memorysize = Memorysize;
        ret =
            Dcm_0x35MemoryRangeCheck(&Dcm_0x35Type, &MemoryIdInfoIndex, &MemoryRangeInfoIndex, &MemoryAddressMatchNum);
        if ((E_NOT_OK == ret) || (MemoryAddressMatchNum > 1u) || (Memorysize == 0u))
        {
            /*different MemoryIdValue compare to the request memoryAddress,send NRC 0x31*/
            /*memoryAddress is not inside the allowed memory ranges,send NRC 0x31 */
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        /*MemoryAddress session Accsess Check*/
        ret = Dcm_0x35MemorySessionCheck(MemoryIdInfoIndex, MemoryRangeInfoIndex, ErrorCode);
    }
    if (E_OK == ret)
    {
        /*MemoryAddress Security Accsess Check*/
        ret = Dcm_0x35MemorySecurityCheck(MemoryIdInfoIndex, MemoryRangeInfoIndex, ErrorCode);
    }

    if ((E_OK == ret) && (Dcm_DspProgram.Status != DCM_UDS0X36_INIT))
    {
        /*check program status*/
        /*send NRC 0x22*/
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
        ret = E_NOT_OK;
    }

    if (E_OK == ret)
    {
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
        Offset = pDcmChannelCfg->offset;
        DataFormatIdentifier = pMsgContext->pReqData[1];
        ret = Dcm_ProcessRequestUpload(
            OpStatus,
            DataFormatIdentifier,
            MemoryAddress,
            Memorysize,
            &BlockLength,
            ErrorCode);
        if (DCM_E_PENDING == ret)
        {
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
        }
        if (BlockLength > pDcmChannelCfg->Dcm_DslBufferSize)
        {
#if (STD_ON == DCM_DEV_ERROR_DETECT)
            /*If the function call Xxx_ProcessRequestUpload returns
             * a requested buffer length larger than the supported
             * buffer length of the current protocol connection,
             * the Dcm shall report the Det error DCM_E_INTERFACE_BUFFER_OVERFLOW.*/
            DCM_DET_REPORTERROR(DCM_MAIN_FUNCTION_ID, DCM_E_INTERFACE_BUFFER_OVERFLOW);
#endif
        }
    }

    /* check tx data length */
    if ((E_OK == ret) && ((6u) > (pDcmChannelCfg->Dcm_DslBufferSize)))
    {
        /*Pdu length is bigger than buffer size */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        SchM_Enter_Dcm_ExclusiveArea();
        /* get address */
        Dcm_DspProgram.address = MemoryAddress;
        /* set index to 0, for service 0x36 */
        Dcm_DspProgram.blockId = 0x00;
        /* set 0x36 service status to DCM_UDS0X36_TO_UPLOAD*/
        Dcm_DspProgram.Status = DCM_UDS0X36_TO_UPLOAD;
        /*memory the MemoryIdInfoIndex*/
        Dcm_DspProgram.MemoryIdInfoIndex = MemoryIdInfoIndex;
        /*memory one block size*/
        Dcm_DspProgram.BlockLength = BlockLength;
        Dcm_DspProgram.MemorySize = Memorysize;
        Dcm_DspProgram.FirstFlag = TRUE;
        SchM_Exit_Dcm_ExclusiveArea();
        /*the processing is successful,assemble positive response*/
        Dcm_Channel[Offset] = 0x75; /*response SID*/
        Dcm_Channel[Offset + 1u] = 0x40;
        /*lengthFormatIdentifier*/
        Dcm_Channel[Offset + 2u] = (uint8)(BlockLength >> (uint8)24);
        /*maxNumberOfBlockLength*/
        Dcm_Channel[Offset + 3u] = (uint8)(BlockLength >> (uint8)16);
        /*maxNumberOfBlockLength*/
        Dcm_Channel[Offset + 4u] = (uint8)(BlockLength >> (uint8)8);
        /*maxNumberOfBlockLength*/
        Dcm_Channel[Offset + 5u] = (uint8)BlockLength;
        /*maxNumberOfBlockLength*/
        pMsgContext->ResMaxDataLen = 6u;
        pMsgContext->ResDataLen = 6u;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        DsdInternal_ProcessingDone(ProtocolCtrlId);
    }
    else if (E_NOT_OK == ret)
    {
        DslInternal_InitDspProgramInfo();
    }
    else
    {
        /*idle*/
    }
    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
