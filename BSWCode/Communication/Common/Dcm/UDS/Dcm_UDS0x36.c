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

/****************************************************************
     UDS: TransferData (36 hex) service
 ***************************************************************/
#if (STD_ON == DCM_UDS_SERVICE0X36_ENABLED)

/*length of 0x36 header sid and bolckid*/
#define DCM_UDS_0X36_HEADER_LENGTH 0x2u
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
static FUNC(Std_ReturnType, DCM_CODE)
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    Dcm_Uds0x36SubCheck(uint8 ProtocolCtrlId, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret = E_OK;
    uint8 MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    const Dcm_MsgContextType* pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;

    /*min-length check*/
    if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x04)
        || (Dcm_DspProgram.Status == DCM_UDS0X38_0x05))
    {
        if (DCM_UDS0X36_REQ_DATA_UPLOAD_MINLENGTH > pMsgContext->ReqDataLen)
        {
            /*the min length of message is not correct,send NRC 0x13*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            ret = E_NOT_OK;
        }
    }
    else if (
        (Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x01)
        || (Dcm_DspProgram.Status == DCM_UDS0X38_0x03))
    {
        if (DCM_UDS0X36_REQ_DATA_DOWNLOAD_MINLENGTH > pMsgContext->ReqDataLen)
        {
            /*the min length of message is not correct,send NRC 0x13*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            ret = E_NOT_OK;
        }
    }
    else
    {
        if (DCM_UDS0X36_REQ_DATA_UPLOAD_MINLENGTH > pMsgContext->ReqDataLen)
        {
            /*the min length of message is not correct,send NRC 0x13*/
            *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            ret = E_NOT_OK;
        }
        else
        {
            /*the request message is not sequence,send NRC 0x24*/
            *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
            ret = E_NOT_OK;
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
 * CallByAPI           <Dcm_UDS0x36_CheckSameBlock>
 */
/***********************************************************************/
static FUNC(boolean, DCM_CODE) Dcm_UDS0x36_CheckSameBlock(uint32* Size)
{
    boolean IsSameBlock;
    if ((Dcm_DspProgram.blockId != Dcm_DspProgram.reqBlockId) || (Dcm_DspProgram.FirstFlag == TRUE))
    {
        *Size = Dcm_DspProgram.MemorySize;
        IsSameBlock = FALSE;
    }
    else
    {
        *Size = Dcm_DspProgram.MemorySize + Dcm_DspProgram.DcmLastloadSize;
        IsSameBlock = TRUE;
    }
    return IsSameBlock;
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_Uds0x36ServiceConditionCheck(
    uint8 ProtocolCtrlId,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryIdentifier,
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    Std_ReturnType ret;
    uint8 MsgCtrlId;
    Dcm_MsgLenType ReqDataLen;
    uint32 MemorySizeRemain = 0;
    boolean IsSameBlockId;

    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    Dcm_DspProgram.reqBlockId = Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[1];
    ReqDataLen = Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - DCM_UDS_0X36_HEADER_LENGTH;
    IsSameBlockId = Dcm_UDS0x36_CheckSameBlock(&MemorySizeRemain);

    ret = Dcm_Uds0x36SubCheck(ProtocolCtrlId, ErrorCode);
    if ((E_OK == ret)
        && ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD)))
    {
        if (Dcm_DspProgram.MemoryIdInfoIndex != 0xFFu)
        {
            *MemoryIdentifier =
                DcmPbCfgPtr->pDcmDspCfg->pDcmDspMemory->DcmDspMemoryIdInfo[Dcm_DspProgram.MemoryIdInfoIndex]
                    .DcmDspMemoryIdValue;
        }
        else
        {
            /*the MemoryIdInfoIndex is not correct,send NRC 0x22*/
            *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        if ((Dcm_DspProgram.Status == DCM_UDS0X36_INIT) || (Dcm_DspProgram.Status > DCM_UDS0X38_0x05))
        {
            *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
            ret = E_NOT_OK;
        }
        else if (
            (MemorySizeRemain == 0u)
            && ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD)))
        {
            /* Download/Upload Process is over, send NRC 0x24 */
            *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;
            ret = E_NOT_OK;
        }
        else if (
            (((Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x01)
              || (Dcm_DspProgram.Status == DCM_UDS0X38_0x03))
             && ((ReqDataLen > (Dcm_DspProgram.BlockLength - DCM_UDS_0X36_HEADER_LENGTH))
                 || ((IsSameBlockId == TRUE) && (ReqDataLen != Dcm_DspProgram.DcmLastloadSize))))
            || ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD) && (MemorySizeRemain < ReqDataLen)))
        {
            /* Downloaed Data Size goes over Required Length, send NRC 0x71 */
            *ErrorCode = DCM_E_TRANSFERDATASUSPENDED;
            ret = E_NOT_OK;
        }
        else if ((IsSameBlockId == FALSE) && (Dcm_DspProgram.reqBlockId != (uint8)(Dcm_DspProgram.blockId + 1u)))
        {
            /* then calculate block index and check block index ,send NRC 0x73 */
            *ErrorCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;
            ret = E_NOT_OK;
        }
        else
        {
            /*idle*/
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
static FUNC(Std_ReturnType, DCM_CODE) Dcm_UDS0x36_TxCheck(
    uint8 ProtocolCtrlId,
    uint32 MemorySize,
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint8 MsgCtrlId;
    Std_ReturnType ret = E_OK;
    uint8 TxChannelCtrlIndex;
    uint8 TxChannelCfgIndex;
    uint32 Offset;
    const Dcm_DslBufferType* pDcmChannelCfg;
    Dcm_MsgContextType* pMsgContext;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    pMsgContext = &Dcm_MsgCtrl[MsgCtrlId].MsgContext;
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    pDcmChannelCfg = &DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg[TxChannelCfgIndex];
    Offset = pDcmChannelCfg->offset;

    /* check tx data length */
    if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD)
        && ((MemorySize + DCM_UDS_0X36_HEADER_LENGTH) > pDcmChannelCfg->Dcm_DslBufferSize))
    {
        /*Pdu length is bigger than buffer size */
        *ErrorCode = DCM_E_RESPONSETOOLONG;
        ret = E_NOT_OK;
    }
    else
    {
        if (DCM_UDS_0X36_HEADER_LENGTH > pDcmChannelCfg->Dcm_DslBufferSize)
        {
            /*Pdu length is bigger than buffer size */
            *ErrorCode = DCM_E_RESPONSETOOLONG;
            ret = E_NOT_OK;
        }
    }
    if (E_OK == ret)
    {
        if (((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD))
            && ((Dcm_DspProgram.blockId != Dcm_DspProgram.reqBlockId) || (Dcm_DspProgram.FirstFlag == TRUE)))
        {
            /* calculate address */
            Dcm_DspProgram.address += MemorySize;
            Dcm_DspProgram.DcmLastloadSize = MemorySize;
        }
        /*the processing is successful,assemble positive response*/
        Dcm_Channel[Offset] = 0x76; /*response SID*/
        Dcm_Channel[Offset + 1u] = Dcm_DspProgram.reqBlockId;
        /*blockSequenceCounter*/

        if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x04)
            || (Dcm_DspProgram.Status == DCM_UDS0X38_0x05))
        {
            pMsgContext->ResMaxDataLen = MemorySize + DCM_UDS_0X36_HEADER_LENGTH;
            pMsgContext->ResDataLen = MemorySize + DCM_UDS_0X36_HEADER_LENGTH;
            if (((Dcm_DspProgram.blockId != Dcm_DspProgram.reqBlockId) || (Dcm_DspProgram.FirstFlag == TRUE))
                && (Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD))
            {
                Dcm_DspProgram.MemorySize -= MemorySize;
            }
        }
        else if (
            (Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x01)
            || (Dcm_DspProgram.Status == DCM_UDS0X38_0x03))
        {
            pMsgContext->ResMaxDataLen = DCM_UDS_0X36_HEADER_LENGTH;
            pMsgContext->ResDataLen = DCM_UDS_0X36_HEADER_LENGTH;
            if (((Dcm_DspProgram.blockId != Dcm_DspProgram.reqBlockId) || (Dcm_DspProgram.FirstFlag == TRUE))
                && (Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD))
            {
                Dcm_DspProgram.MemorySize -= MemorySize;
            }
        }
        else
        {
            /*idle*/
        }
        SchM_Enter_Dcm_ExclusiveArea();
        if (TRUE == Dcm_DspProgram.FirstFlag)
        {
            Dcm_DspProgram.FirstFlag = FALSE;
        }
        Dcm_DspProgram.blockId = Dcm_DspProgram.reqBlockId;
        pMsgContext->pResData = &Dcm_Channel[Offset];
        SchM_Exit_Dcm_ExclusiveArea();
        DsdInternal_ProcessingDone(ProtocolCtrlId);
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
/*******************************/
/* PRQA S 3432,1532++ */ /* MISRA Rule 20.7,8.7 */
FUNC(Std_ReturnType, DCM_CODE)
Dcm_UDS0x36(
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
    uint8 MemoryIdentifier = 0;
    uint32 MemorySize = 0;
    uint32 Address;
    boolean IsSameBlockId;
    uint32 MemorySizeRemain = 0;

    /*if the required protocol is configured,get the index of runtime datum*/
    MsgCtrlId = Dcm_ProtocolCtrl[ProtocolCtrlId].MsgCtrlIndex;
    ret = Dcm_Uds0x36ServiceConditionCheck(ProtocolCtrlId, &MemoryIdentifier, ErrorCode);
    TxChannelCtrlIndex = Dcm_MsgCtrl[MsgCtrlId].Dcm_TxCtrlChannelIndex;
    TxChannelCfgIndex = Dcm_ChannelCtrl[TxChannelCtrlIndex].Dcm_ChannelCfgIndex;
    Offset = (DcmPbCfgPtr->pDcmDslCfg->pDcmChannelCfg)[TxChannelCfgIndex].offset;
    if (E_OK == ret)
    {
        IsSameBlockId = Dcm_UDS0x36_CheckSameBlock(&MemorySizeRemain);
        /*process is running and the request format is correct,to call the callout Following NRCs
         * shall be the responsibility of the callout function 0x23,0x31,0x71,0x72,0x92,0x93*/
        if ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x04)
            || (Dcm_DspProgram.Status == DCM_UDS0X38_0x05))
        {
            MemorySize = (Dcm_DspProgram.Status == DCM_UDS0X36_TO_UPLOAD)
                             ? ((MemorySizeRemain > (Dcm_DspProgram.BlockLength - DCM_UDS_0X36_HEADER_LENGTH))
                                    ? (Dcm_DspProgram.BlockLength - DCM_UDS_0X36_HEADER_LENGTH)
                                    : MemorySizeRemain)
                             : Dcm_DspProgram.BlockLength;
            Address = (IsSameBlockId == TRUE) ? (Dcm_DspProgram.address - Dcm_DspProgram.DcmLastloadSize)
                                              : Dcm_DspProgram.address;
            ret = Dcm_ReadMemory(
                OpStatus,
                MemoryIdentifier,
                Address,
                MemorySize,
                (&Dcm_Channel[Offset + DCM_UDS_0X36_HEADER_LENGTH]),
                ErrorCode);
            if (DCM_READ_FAILED == ret)
            {
                ret = E_NOT_OK;
            }
#if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
            else if (DCM_READ_FORCE_RCRRP == ret)
            {
                /*the processing is pending,send NRC 0x78*/
                ret = DCM_E_FORCE_RCRRP;
                DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_RESPONSE_PENDING);
                DsdInternal_ProcessingDone(ProtocolCtrlId);
                Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = DCM_E_FORCE_RCRRP;
            }
#endif
            else if (DCM_READ_PENDING == ret)
            {
                ret = DCM_E_PENDING;
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            }
            else
            {
                /* idle */
            }
        }
        else if (
            ((Dcm_DspProgram.Status == DCM_UDS0X36_TO_DOWNLOAD) && (IsSameBlockId == FALSE))
            || (Dcm_DspProgram.Status == DCM_UDS0X38_0x01) || (Dcm_DspProgram.Status == DCM_UDS0X38_0x03))
        {
            MemorySize = Dcm_MsgCtrl[MsgCtrlId].MsgContext.ReqDataLen - DCM_UDS_0X36_HEADER_LENGTH;
            ret = Dcm_WriteMemory(
                OpStatus,
                MemoryIdentifier,
                Dcm_DspProgram.address,
                MemorySize,
                &Dcm_MsgCtrl[MsgCtrlId].MsgContext.pReqData[2],
                ErrorCode);
            if (DCM_WRITE_FAILED == ret)
            {
                ret = E_NOT_OK;
            }
#if (STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN)
            else if (DCM_WRITE_FORCE_RCRRP == ret)
            {
                /*the processing is pending,send NRC 0x78*/
                ret = DCM_E_FORCE_RCRRP;
                DsdInternal_SetNrc(ProtocolCtrlId, DCM_E_RESPONSE_PENDING);
                DsdInternal_ProcessingDone(ProtocolCtrlId);
                Dcm_MsgCtrl[MsgCtrlId].Dcm_Ret = DCM_E_FORCE_RCRRP;
            }
#endif /* STD_ON == DCM_DSLDIAGRESP_FORCERESPENDEN */
            else if (DCM_WRITE_PENDING == ret)
            {
                ret = DCM_E_PENDING;
                Dcm_MsgCtrl[MsgCtrlId].Dcm_OpStatus = DCM_PENDING;
            }
            else
            {
                /* idle */
            }
        }
        else
        {
            /*idle*/
        }
    }

    if (E_OK == ret)
    {
        ret = Dcm_UDS0x36_TxCheck(ProtocolCtrlId, MemorySize, ErrorCode);
    }

    return ret;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif
