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
     UDS: RequestFileTransfer (38 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X38_ENABLED)
typedef struct
{
    uint32* BlockLength;
    uint16* fileSizeParameterLength;
    uint8* fileSizeUncompressedOrDirInfoLength;
    uint8* fileSizeCompressed;
    uint8* dataFormatIdentifier;
} Dcm_0x38Types;
/***************************/
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
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x38ServiceConditionCheck(
    Dcm_OpStatusType OpStatus,
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_0x38Types, AUTOMATIC, DCM_VAR) Dcm_0x38Type,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    uint16 filePathAndNameLength;
    uint8 modeOfOperation;
    uint8* filePathAndName;
    uint16 ReqOffset;
    const Dcm_DspRequestFileTransferType* DcmDspRequestFileTransfer =
        DcmPbCfgPtr->pDcmDspCfg->DcmDspRequestFileTransfer;
    Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;

    /*min-length check*/
    if (DCM_UDS0X38_REQ_DATA_MINLENGTH > pMsgContext->ReqDataLen)
    {
        /*the min length of message is not correct,send NRC 0x13*/
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        ret = E_NOT_OK;
    }
    if (E_OK == ret)
    {
        modeOfOperation = pMsgContext->pReqData[1];
        if ((modeOfOperation < 0x01u) || (modeOfOperation > 0x05u))
        { /*the modeOfOperation is not correct,send NRC 0x31*/
            *ErrorCode = DCM_E_REQUESTOUTOFRANGE;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        filePathAndNameLength =
            (uint16)((uint16)pMsgContext->pReqData[3u] | (uint16)((uint16)pMsgContext->pReqData[2u] << 8u));
        filePathAndName = &(pMsgContext->pReqData[4u]);
        ReqOffset = 4u + filePathAndNameLength;
        if ((modeOfOperation != 0x02u) && (modeOfOperation != 0x05u))
        {
            *Dcm_0x38Type->dataFormatIdentifier = pMsgContext->pReqData[ReqOffset];
            ReqOffset += 1u;
            if (modeOfOperation != 0x04u)
            {
                if (DcmDspRequestFileTransfer != NULL_PTR)
                {
                    *Dcm_0x38Type->fileSizeParameterLength =
                        (uint16)DcmDspRequestFileTransfer->DcmRequestFileTransferFileSizeParameterLength;
                }
                else
                {
                    *Dcm_0x38Type->fileSizeParameterLength = (uint16)pMsgContext->pReqData[ReqOffset];
                }
                ReqOffset += 1u;
                Dcm_0x38Type->fileSizeUncompressedOrDirInfoLength = &(pMsgContext->pReqData[ReqOffset]);
                ReqOffset += *Dcm_0x38Type->fileSizeParameterLength;
                Dcm_0x38Type->fileSizeCompressed = &(pMsgContext->pReqData[ReqOffset]);
                ReqOffset += *Dcm_0x38Type->fileSizeParameterLength;
            }
            else
            {
                if (DcmDspRequestFileTransfer != NULL_PTR)
                {
                    *Dcm_0x38Type->fileSizeParameterLength =
                        (uint16)DcmDspRequestFileTransfer->DcmRequestFileTransferFileSizeParameterLength;
                }
            }
        }
        else
        {
            if (DcmDspRequestFileTransfer != NULL_PTR)
            {
                *Dcm_0x38Type->fileSizeParameterLength =
                    (uint16)DcmDspRequestFileTransfer->DcmRequestFileTransferFileSizeParameterLength;
            }
        }
        if (ReqOffset != pMsgContext->ReqDataLen)
        { /*the length of message is not correct,send NRC 0x13*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            ret = E_NOT_OK;
        }
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
        ret = Dcm_ProcessRequestFileTransfer(
            OpStatus,
            modeOfOperation,
            *Dcm_0x38Type->fileSizeParameterLength,
            filePathAndName,
            *Dcm_0x38Type->dataFormatIdentifier,
            Dcm_0x38Type->fileSizeUncompressedOrDirInfoLength,
            Dcm_0x38Type->fileSizeCompressed,
            Dcm_0x38Type->BlockLength,
            ErrorCode);
        if (DCM_E_PENDING == ret)
        {
            Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
        }
    }
    return ret;
}

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
static FUNC(void, DCM_CODE) Dcm_UDS0x38_Tx(uint8 ProtocolCtrlId, uint32 ResOffset, uint32 BlockLength)
{
    uint8 MsgCtrlId;
    uint8 modeOfOperation;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    Dcm_MsgContextType* pMsgContext;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
    modeOfOperation = pMsgContext->pReqData[1];

    SchM_Enter_Dcm_ExclusiveArea(); /* get address */
    Dcm_DspProgram.address = 0UL;   /* set index to 0, for service 0x36 */
    Dcm_DspProgram.blockId = 0x00;  /* set 0x36 service status to DCM_UDS0X36_TO_UPLOAD*/
    switch (modeOfOperation)
    {
    case 0x01u:
        Dcm_DspProgram.Status = DCM_UDS0X38_0x01;
        break;
    case 0x02u:
        Dcm_DspProgram.Status = DCM_UDS0X38_0x02;
        break;
    case 0x03u:
        Dcm_DspProgram.Status = DCM_UDS0X38_0x03;
        break;
    case 0x04u:
        Dcm_DspProgram.Status = DCM_UDS0X38_0x04;
        break;
    case 0x05u:
        Dcm_DspProgram.Status = DCM_UDS0X38_0x05;
        break;
    default:
        /*idle*/
        break;
    }
    Dcm_DspProgram.MemoryIdInfoIndex = 0xFF;  /*memory the MemoryIdInfoIndex*/
    Dcm_DspProgram.BlockLength = BlockLength; /*memory one block size*/
    SchM_Exit_Dcm_ExclusiveArea();
    pMsgContext->ResMaxDataLen = (Dcm_MsgLenType)ResOffset - Offset;
    pMsgContext->ResDataLen = (Dcm_MsgLenType)ResOffset - Offset;
    pMsgContext->pResData = &Dcm_Channel[Offset];
    DsdInternal_ProcessingDone(ProtocolCtrlId);
}

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
Dcm_UDS0x38(
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
    uint16 fileSizeParameterLength = 0;
    uint8 index;
    uint8 modeOfOperation;
    uint8 dataFormatIdentifier = 0u;
    uint32 ResOffset;
    uint8 fileSizeUncompressedOrDirInfoLength = 0;
    uint8 fileSizeCompressed;
    uint32 BlockLength = 0;
    uint8 lengthFormatIdentifier;
    Dcm_0x38Types Dcm_0x38Type;
    const Dcm_DspRequestFileTransferType* DcmDspRequestFileTransfer =
        DcmPbCfgPtr->pDcmDspCfg->DcmDspRequestFileTransfer;
    const Dcm_DslBufferType* pDcmChannelCfg;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_0x38Type.BlockLength = &BlockLength;
    Dcm_0x38Type.fileSizeParameterLength = &fileSizeParameterLength;
    Dcm_0x38Type.fileSizeUncompressedOrDirInfoLength = &fileSizeUncompressedOrDirInfoLength;
    Dcm_0x38Type.fileSizeCompressed = &fileSizeCompressed;
    Dcm_0x38Type.dataFormatIdentifier = &dataFormatIdentifier;
    ret = Dcm_Uds0x38ServiceConditionCheck(OpStatus, ProtocolCtrlId, &Dcm_0x38Type, ErrorCode);
    if (E_OK == ret)
    {
        modeOfOperation = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1];
        TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
        TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
        pDcmChannelCfg = &(DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex];
        Offset = pDcmChannelCfg->offset;
        /*the processing is successful,assemble positive response*/
        Dcm_Channel[Offset] = 0x78;                 /*response SID*/
        Dcm_Channel[Offset + 1u] = modeOfOperation; /*modeOfOperation*/
        ResOffset = Offset + 2u;
        if (modeOfOperation != 0x02u)
        {
            if (DcmDspRequestFileTransfer != NULL_PTR)
            {
                lengthFormatIdentifier = DcmDspRequestFileTransfer->DcmRequestFileTransferLengthFormatIdentifier;
            }
            else
            {
                lengthFormatIdentifier = 4u;
            }
            Dcm_Channel[ResOffset] = lengthFormatIdentifier;
            ResOffset += 1u;
            for (index = 0; index < lengthFormatIdentifier; index++)
            {
                Dcm_Channel[ResOffset] = (uint8)(BlockLength >> (uint8)((lengthFormatIdentifier - index - 1u) * 8u));
                ResOffset += 1u;
            }
            if (modeOfOperation == 0x05u)
            {
                Dcm_Channel[ResOffset] = 0x00u;
            }
            else
            {
                Dcm_Channel[ResOffset] = dataFormatIdentifier;
            }
            ResOffset += 1u;
            if ((modeOfOperation != 0x01u) && (modeOfOperation != 0x03u))
            {
                Dcm_Channel[ResOffset] = (uint8)fileSizeParameterLength;
                ResOffset += 1u;
                for (index = 0; index < fileSizeParameterLength; index++)
                {
                    Dcm_Channel[ResOffset] = *Dcm_0x38Type.fileSizeUncompressedOrDirInfoLength;
                    (*Dcm_0x38Type.fileSizeUncompressedOrDirInfoLength)++;
                    ResOffset += 1u;
                }
                if (modeOfOperation != 0x05u)
                {
                    for (index = 0; index < fileSizeParameterLength; index++)
                    {
                        Dcm_Channel[ResOffset] = *Dcm_0x38Type.fileSizeCompressed;
                        (*Dcm_0x38Type.fileSizeCompressed)++;
                        ResOffset += 1u;
                    }
                }
            }
        } /* check tx data length */
        if ((ResOffset - Offset) > (pDcmChannelCfg->Dcm_DslBufferSize))
        { /*Pdu length is bigger than buffer size */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        Dcm_UDS0x38_Tx(ProtocolCtrlId, ResOffset, BlockLength);
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
