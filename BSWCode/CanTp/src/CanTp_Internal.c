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
/*
 ************************************************************************************************************************
 **
 **  @file               : CanTp_Internal.h
 **  @author             : xue.han
 **  @date               : 2024/08/15
 **  @vendor             : isoft
 **  @description        : CAN Transport Layer internal header
 **
 ***********************************************************************************************************************/
/* PRQA S 1503,1532,1505,3120 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_OneFunRef,VL_CanTp_1505,VL_QAC_MagicNum */
/* =================================================== inclusions =================================================== */
#include "PduR_CanTp.h"
#include "CanIf.h"
#include "SchM_CanTp.h"
#include "CanTp_Internal.h"
#include "CanTp_Lcfg.h"
#if (CANTP_DEM_PRODUCTION_ERROR_DETECT == STD_ON)
#include "Dem.h"
#endif
/* ======================================== internal function declarations ========================================*/

#if (CANTP_DYN_ID_SUPPORT == STD_ON)

#endif
/* ========================================== internal function definitions ========================================= */
#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
/**
 * @brief Get Frame PCI Type
 */
CanTp_FramePCIType CanTp_GetFramePCIType(const PduIdType rxpduId, const PduInfoType* pduInfoPtr)
{
    uint8 index = rxpduId; /* PRQA S 4461 */ /* VL_CanTp_4461 */
#if (CANTP_SUPPORT_VARIANT == STD_ON)
    index = CanTp_InternalRxPduIndex(rxpduId);
#endif
    CanTp_AddressingFormatType AddressingFormat = CanTp_ConfigPtr->CanTp_RxPduMap[index].AddressingFormat;
    CanTp_FramePCIType         framePCIType     = CANTP_FTYPE_RESEVED;
    switch (AddressingFormat)
    {
    case CANTP_STANDARD:
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
#endif
        framePCIType = pduInfoPtr->SduDataPtr[CANTP_PCI_OFFSET_STD_NF] & CANTP_FTYPE_MASK;
        break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_EXTENDED:
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
        if (pduInfoPtr->SduLength > CANTP_PCI_OFFSET_EX_MIX)
        {
            /**check this RX-SDU, which is configured as mix11 addressing
             * format, and the CAN data at least includes AE and PCIType*/
            /*check its TA and frame type*/
            framePCIType = pduInfoPtr->SduDataPtr[CANTP_PCI_OFFSET_EX_MIX] & CANTP_FTYPE_MASK;
        }
        break;
#endif
    default:
        /*idle*/
        break;
    }
    return framePCIType;
}

/**
 * @brief Get Frame PCI Offset
 */
uint8 CanTp_GetPCIOffset(CanTp_AddressingFormatType addressingFormat)
{
    uint8 offset = 0x00u;
    switch (addressingFormat)
    {
    case CANTP_STANDARD:
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
#endif
        offset = CANTP_PCI_OFFSET_STD_NF;
        break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_EXTENDED:
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
        offset = CANTP_PCI_OFFSET_EX_MIX;
        break;
#endif
    default:
        /*idle*/
        break;
    }
    return offset;
}

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
/**
 * @brief  Match frame length
 */
PduLengthType CanTp_MatchDLC(PduLengthType frameDL)
{
    uint8         i;
    PduLengthType len = frameDL;
    /**ISO 11898-1:2014 define the DLC table*/
    const uint8 canDL[CANTP_MATCH_DLC_NUMBER] = {8u, 12u, 16u, 20u, 24u, 32u, 48u, 64u};
    if (frameDL > (PduLengthType)CANTP_CAN20_FRAME_LEN_MAX)
    {
        for (i = 0u; i < CANTP_MATCH_DLC_NUMBER; i++)
        {
            if (frameDL <= canDL[i])
            {
                len = canDL[i];
                break;
            }
        }
    }
    return len;
}
/* PRQA S 3120 -- */

/**
 * @brief Check SF datalength
 */
boolean CanTp_CheckReceivedSFDL(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    PduLengthType*          datalength,
    uint8*                  dataOffset
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
    ,
    CanTp_RxStatusType* rxStatus
#endif
)
{
    boolean checkResult = FALSE;
    uint8   offset      = CanTp_GetPCIOffset(rxNSduCfgPtr->AddressingFormat);
    uint8   CAN_DL      = pduInfoPtr->SduLength;
    uint8   SF_DL       = (pduInfoPtr->SduDataPtr[offset] & CANTP_SF_DL_MASK);

    /**< If the network layer receives a SF where SF_DL is equal to 0, then the network layer shall ignore the
     * received SF N_PDU;*/
    if (SF_DL != 0u)
    {
        /**< see 9.6.2.2 SF_DL error handling */
        /* PRQA S 2985 ++ */ /* VL_CanTp_2985 */
        if (((SF_DL + offset + CANTP_PCI_LENGTH_SF) <= CAN_DL) && (CAN_DL <= rxNSduCfgPtr->RxNPduDLC)
            && (((CAN_DL <= CANTP_CAN20_FRAME_LEN_MAX) && (rxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_OFF))
                || ((CAN_DL == CANTP_CAN20_FRAME_LEN_MAX) && (rxNSduCfgPtr->RxPaddingActivation == CANTP_PADDING_ON))))
        /* PRQA S 2985 -- */
        {
            checkResult = TRUE;
            *datalength = SF_DL;
            *dataOffset = offset + CANTP_PCI_LENGTH_SF;
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
            if ((CANTP_FLAGISSET(rxNSduCfgPtr->CanTpPduFlag, CANTP_CAN20_CANFD_MASK))
                && (pduInfoPtr->MetaDataPtr != NULL_PTR) && (rxNSduCfgPtr->RxNPduDLC > CANTP_CAN20_FRAME_LEN_MAX)
                && (CANTP_FLAG_ISNSET(pduInfoPtr->MetaDataPtr[CANTP_ID_OFFSET_META_IF], CANTP_CANID_CANFD_MASK)))
            {
                /* PRQA S 3473 ++ */ /* VL_CanTp_3473 */
                CANTP_SETFLAG(rxStatus->EventFlags, CANTP_EVENT_CAN20_WITH_CANFD);
                /* PRQA S 3473 -- */
            }
#endif
        }
    }
#if (STD_ON == CANTP_FD)
    /**< SF_DL is encoded in the second N_PCI byte (Byte #2) value. This is only allowed for CAN FD type frames.*/
    else
    {
        SF_DL = pduInfoPtr->SduDataPtr[offset + 1u];
        /**< In the case of CAN frame data optimization (see 10.4.2.2): If the network layer receives an SF where
         * the value of SF_DL does not match the valid values shown in Table 12, then the network layer shall
         * ignore the received SF N_PDU.*/
        uint8   matchSF      = CanTp_MatchDLC(SF_DL + offset + CANTP_PCI_LENGTH_SF_FD);
        boolean isValidSFDL  = (SF_DL != 0u) && ((SF_DL + offset) >= CANTP_CAN20_FRAME_LEN_MAX);
        boolean isValidCANDL = (CAN_DL <= rxNSduCfgPtr->RxNPduDLC) && (matchSF == CAN_DL);

        if (isValidSFDL && isValidCANDL)
        {
            checkResult = TRUE;
            *datalength = SF_DL;
            *dataOffset = offset + CANTP_PCI_LENGTH_SF_FD;
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
            if (CANTP_FLAGISSET(rxNSduCfgPtr->CanTpPduFlag, CANTP_CAN20_CANFD_MASK))
            {
                /* PRQA S 3473 ++ */ /* VL_CanTp_3473 */
                CANTP_CLEARFLAG(rxStatus->EventFlags, CANTP_EVENT_CAN20_WITH_CANFD);
                /* PRQA S 3473 -- */
            }
#endif
        }
    }
#endif
    return checkResult;
}

/**
 * @brief Check FF datalength
 */
boolean CanTp_CheckReceivedFFDL(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    PduLengthType*          totalLength,
    PduLengthType*          datalength,
    uint8*                  dataOffset)
{
    boolean       checkResult = FALSE;
    uint8         offset      = CanTp_GetPCIOffset(rxNSduCfgPtr->AddressingFormat);
    PduLengthType RX_DL       = CanTp_CheckVaildLen(pduInfoPtr->SduLength) ? pduInfoPtr->SduLength : 0u;
    PduLengthType FF_DLmin;
    /* PRQA S 3120,4391 ++ */ /* VL_QAC_MagicNum ,VL_CanTp_DataConverteToWider */
    PduLengthType FF_DL =
        (((PduLengthType)(pduInfoPtr->SduDataPtr[offset] & CANTP_FF_DL_MASK)
          << 8u) /* PRQA S 4397 */ /* VL_CanTp_4397 */
         | (pduInfoPtr->SduDataPtr[offset + 1u]));
    /* PRQA S 3120,4391 -- */
    /* 15765-2:2016 defines the received CAN_DL to RX_DL mapping table. vaild RX_DL values:8,12,16,20,24,32,48,64 */

    if ((FF_DL != 0u) && (RX_DL != 0u))
    {
        FF_DLmin = (RX_DL > CANTP_CAN20_FRAME_LEN_MAX) ? (PduLengthType)(RX_DL - offset - CANTP_PCI_LENGTH_FF + 1u)
                                                       : (PduLengthType)(CANTP_CAN20_FRAME_LEN_MAX - offset);
        if ((FF_DL >= FF_DLmin) && (RX_DL <= rxNSduCfgPtr->RxNPduDLC))
        {
            checkResult  = TRUE;
            *totalLength = FF_DL;
            *dataOffset  = offset + CANTP_PCI_LENGTH_FF;
            *datalength  = RX_DL - *dataOffset;
        }
    }
#ifndef CANTP_MATCH_ISO_2004
    else
    {
        /* PRQA S 3120,4461 ++ */ /* VL_QAC_MagicNum,VL_CanTp_4461 */
        FF_DL =
            (((uint32)(pduInfoPtr->SduDataPtr[offset + 2u]) << 24u)
             | ((uint32)(pduInfoPtr->SduDataPtr[offset + 3u]) << 16u)
             | ((uint32)(pduInfoPtr->SduDataPtr[offset + 4u]) << 8u) | ((uint32)(pduInfoPtr->SduDataPtr[offset + 5u])));
        /* PRQA S 3120,4461 -- */
        FF_DLmin = (RX_DL > CANTP_CAN20_FRAME_LEN_MAX)
                       ? (RX_DL - offset - CANTP_PCI_LENGTH_FF_12BIT + 1u)
                       : (CANTP_CAN20_FRAME_LEN_MAX - offset - CANTP_PCI_LENGTH_FF_12BIT + 1u);

        if ((RX_DL != 0u) && (FF_DL > CANTP_FF_DL_12BIT_LENGTH) && (RX_DL <= rxNSduCfgPtr->RxNPduDLC)
            && (FF_DL >= FF_DLmin))
        {
            checkResult  = TRUE;
            *totalLength = FF_DL;
            *dataOffset  = offset + CANTP_PCI_LENGTH_FF_12BIT;
            *datalength  = RX_DL - *dataOffset;
        }
    }
#endif
    return checkResult;
}
/**
 * @brief get Current Bs
 */
uint8 CanTp_GetRxCurrentBs(PduIdType rxNSduId, const CanTp_GPartInfoType* globalInfo)
{
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    uint16 index = rxNSduId - CANTP_GETRXNSDUSTART(globalInfo->index);
#else
    uint16 index = rxNSduId;
#endif
    CanTp_RxStatusType* rxStatus = &(globalInfo->CanTp_RxConnectionStatus[index]);

#if (CANTP_CHANGE_PARAMETER == STD_ON)
    const CanTp_ChangeParameterType* changeRxParameter = &globalInfo->CanTp_ChangeRxParameter[index];
    uint8 bs_cfg = (changeRxParameter->FcBs != CANTP_INVALID_U8) ? changeRxParameter->FcBs : 0u;
#else
    CanTp_RxNSduType* rxNSduCfgPtr = &CanTp_ConfigPtr->CanTp_RxNSduCfg[rxNsduId];
    uint8             bs_cfg       = (rxNSduCfgPtr->Bs != CANTP_INVALID_U8) ? rxNSduCfgPtr->Bs : 0u;
#endif

    if (rxStatus->ChannelState != CANTP_RX_IDLE)
    {
#ifndef CANTP_FIX_BS
        uint32 length = rxStatus->DatalenRemain;
        uint8  offset = (uint8)(rxStatus->InitalDLC) - rxStatus->PCIOffset - CANTP_PCI_LENGTH_CF;
        uint8  bs_cal = (uint8)(((length - 1u) / offset) + 1u); /* PRQA S 2834,1252 */ /* VL_CanTp_2834,VL_QAC_1252 */
        rxStatus->CurrentBs = ((bs_cal > bs_cfg) && (bs_cfg != 0u)) ? bs_cfg : bs_cal;
#else
        rxStatus->CurrentBs = bs_cfg;
#endif
    }
    return bs_cfg;
}

/**
 * @brief Get Current STmin
 */
uint8 CanTp_GetRxCurrentSTmin(
    PduIdType rxNSduId
#if (CANTP_CHANGE_PARAMETER == STD_ON)
    ,
    const CanTp_GPartInfoType* globalInfo
#endif
)
{
#if (CANTP_CHANGE_PARAMETER == STD_ON)
#if (CANTP_MULTIPLE_PARTITION_ENABLED == STD_ON)
    uint8 index = (uint8)rxNSduId - CANTP_GETRXNSDUSTART(globalInfo->index);
#else
    uint8 index = (uint8)rxNSduId;
#endif
    const CanTp_ChangeParameterType* changeRxParameter = &globalInfo->CanTp_ChangeRxParameter[index];
    uint8 stmin = (changeRxParameter->FcSTmin != CANTP_STMIN_INVALID) ? changeRxParameter->FcSTmin : 0u;
#else
    CanTp_RxNSduType* rxNSduCfgPtr = &CanTp_ConfigPtr->CanTp_RxNSduCfg[rxNSduId];
    uint8             stmin        = (rxNSduCfgPtr->STmin != CANTP_STMIN_INVALID) ? rxNSduCfgPtr->STmin : 0u;
#endif
    return stmin;
}
/**
 * @brief Compare remaining buffer with Bs
 */
Std_ReturnType CanTp_CompareRxBufferWithBs(const CanTp_RxStatusType* rxStatus, PduLengthType bufferSize)
{
    Std_ReturnType ret;
    uint8          offset = (uint8)(rxStatus->InitalDLC) - rxStatus->PCIOffset - CANTP_PCI_LENGTH_CF;
    uint32 blockLength    = (uint32)(rxStatus->CurrentBs * offset); /* PRQA S 4391 */ /* VL_CanTp_DataConverteToWider */
    if (bufferSize >= blockLength)
    {
        ret = E_OK;
    }
    else if (bufferSize >= rxStatus->DatalenRemain)
    {
        ret = E_OK;
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/**
 * @brief Assemble PCI information of the Nsdu
 */
void CanTp_TxConstructPCI(CanTp_TxStatusType* txStatus, uint8* dataOffset)
{
    PduLengthType len;
    switch (txStatus->ChannelState)
    {
    case CANTP_TX_TRANSMIT_SF:

        len = txStatus->DatalenTotal + txStatus->PCIOffset + CANTP_PCI_LENGTH_SF;
        if (len > CANTP_CAN20_FRAME_LEN_MAX)
        {
            *dataOffset                                  = CANTP_PCI_LENGTH_SF_FD;
            txStatus->LocalBuf[txStatus->PCIOffset]      = CANTP_FTYPE_SF | 0x00u;
            txStatus->LocalBuf[txStatus->PCIOffset + 1u] = (uint8)(txStatus->DatalenTotal);
        }
        else
        {
            *dataOffset                             = CANTP_PCI_LENGTH_SF;
            txStatus->LocalBuf[txStatus->PCIOffset] = (uint8)(txStatus->DatalenTotal & CANTP_SF_DL_MASK);
        }
        break;
    case CANTP_TX_TRANSMIT_FF:
        txStatus->DatalenRemain -= txStatus->FFDataMax;
        if (txStatus->DatalenTotal <= CANTP_FF_DL_12BIT_LENGTH)
        {
            *dataOffset = CANTP_PCI_LENGTH_FF;
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            txStatus->LocalBuf[txStatus->PCIOffset]      = CANTP_FTYPE_FF | (uint8)(txStatus->DatalenTotal >> 8u);
            txStatus->LocalBuf[txStatus->PCIOffset + 1u] = (uint8)(txStatus->DatalenTotal & 0xFFu);
            /* PRQA S 3120 -- */
        }
        else
        {
            *dataOffset                             = CANTP_PCI_LENGTH_FF_12BIT;
            txStatus->LocalBuf[txStatus->PCIOffset] = CANTP_FTYPE_FF | 0x00u;
            /* PRQA S 3120,0499,2985 ++ */ /* VL_QAC_MagicNum,VL_CanTp_0499,VL_CanTp_2985 */
            txStatus->LocalBuf[txStatus->PCIOffset + 1u] = 0x00u;
            txStatus->LocalBuf[txStatus->PCIOffset + 2u] = (uint8)(txStatus->DatalenTotal >> 24U);
            txStatus->LocalBuf[txStatus->PCIOffset + 3u] = (uint8)((txStatus->DatalenTotal >> 16u) & 0xFFu);
            txStatus->LocalBuf[txStatus->PCIOffset + 4u] = (uint8)((txStatus->DatalenTotal >> 8u) & 0xFFu);
            txStatus->LocalBuf[txStatus->PCIOffset + 5u] = (uint8)(txStatus->DatalenTotal & 0xFFu);
            /* PRQA S 3120,0499,2985 -- */
        }
        break;
    case CANTP_TX_TRANSMIT_CF:
        *dataOffset = CANTP_PCI_LENGTH_CF;
        if (txStatus->ExpectdCfSn > CANTP_MAX_SEQUENC_NUMBER)
        {
            txStatus->ExpectdCfSn = 0u;
        }
        /* PRQA S 3120 ++*/ /* VL_QAC_MagicNum */
        txStatus->LocalBuf[txStatus->PCIOffset] = CANTP_FTYPE_CF | txStatus->ExpectdCfSn;
        /* PRQA S 3120 --*/
        if (txStatus->DatalenRemain >= txStatus->CFDataMax)
        {
            txStatus->DatalenRemain -= txStatus->CFDataMax;
        }
        else
        {
            txStatus->DatalenRemain = 0u;
        }
        break;
    default:
        /* idle */
        break;
    }
}

/**
 * @brief Assemble Padding data information of the NSdu
 */
void CanTp_AssemblePaddingData(CanTp_TxStatusType* txStatus, PduInfoType* txpduInfo, uint8 assembledLength)
{
    if ((txStatus->TxDLC <= CANTP_CAN20_FRAME_LEN_MAX) || (assembledLength <= CANTP_CAN20_FRAME_LEN_MAX))
    {
        /* CAN2.0 or 8 byte CAN-FD */ /* PRQA S 4491 ++ */ /* VL_CanTp_4491 */
        CanTp_MemSet(
            &txStatus->LocalBuf[assembledLength],
            CANTP_PADDING_BYTE,
            (CANTP_CAN20_FRAME_LEN_MAX - assembledLength));
        /* PRQA S 4491 -- */
        txpduInfo->SduLength = CANTP_CAN20_FRAME_LEN_MAX;
    }
#if (CANTP_FD == STD_ON)
    else
    {
        uint8 paddingLength = 0u;
        /** @see SWS_CanTp_00351 If the data length which shall be transmitted via CanIf_Transmit() does not match
         *possible DLC values (0..8, 12, 16, 20, 24, 32, 48, or 64), CanTp shall use the next higher valid DLC for
         *transmission with initialization of unused bytes to the value of CANTP_PADDING_BYTE (see CanTpPaddingByte).c
         *(SRS_Can_01073)
         *  */
        /* Current Length |  Padded Length   | Delta   */
        /* ---------------+------------------+-------- */
        /*   9 .... 12    | 12 = 0b0000 1100 |  0x04  */
        /*   13 ... 16    | 16 = 0b0001 0000 |  0x04   */
        /*   17 ... 20    | 20 = 0b0001 0100 |  0x04   */
        /*   21 ... 24    | 24 = 0b0001 1000 |  0x04   */
        /*   25 ... 32    | 32 = 0b0010 0000 |  0x10   */
        /*   33 ... 48    | 48 = 0b0011 0000 |  0x10   */
        /*   49 ... 64    | 64 = 0b0100 0000 |  --     */
        if (assembledLength <= 24u)
        {
            /** The 0xFC mask is used to mask out the lowest 2 bits, which means the value of CurrentLength will be
             * rounded down to the nearest multiple of 4. */
            paddingLength = (uint8)(((uint8)(assembledLength - 1u) & 0xFCu) + 0x04u);
        }
        else
        {
            /** The 0xF0 mask is used to mask out the lowest 4 bits, which means the value of CurrentLength will be
             * rounded down to the nearest multiple of 16. */
            paddingLength = (uint8)(((uint8)(assembledLength - 1u) & 0xF0u) + 0x10u);
        }
        /** CAN-FD with more than 8 byte */
        CanTp_MemSet(&txStatus->LocalBuf[assembledLength], CANTP_PADDING_BYTE, (paddingLength - assembledLength));
        txpduInfo->SduLength = paddingLength;
    }
#endif
}

#if ((CANTP_DYN_ID_SUPPORT == STD_ON))
/**
 * @brief Save MetaData information when receiving FF,SF
 */
Std_ReturnType CanTp_SaveRxMetaDataInfo(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    const PduInfoType*      pduInfoPtr)
{
    Std_ReturnType ret = E_OK;
#if (                                                                                             \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF) \
    && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF))
    CANTP_PARAM_UNUSED(rxStatus);
#endif
    SchM_Enter_CanTp_ExclusiveArea();
    switch (rxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        rxStatus->MetaDataNTa = pduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX];
        if (rxNSduCfgPtr->RxTaType == CANTP_PHYSICAL)
        {
            if (rxNSduCfgPtr->NSa == CANTP_INVALID_U8)
            {
                ret = E_NOT_OK;
            }
            else if (rxStatus->MetaDataNTa != rxNSduCfgPtr->NSa)
            {
                ret = E_NOT_OK;
            }
            else
            {
                /* idle */
            }
        }
        break;
#endif
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        rxStatus->MetaDataNAe = pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX];
        if ((rxNSduCfgPtr->NAe == CANTP_INVALID_U8) || (rxStatus->MetaDataNAe != rxNSduCfgPtr->NAe))
        {
            ret = E_NOT_OK;
        }
        break;
#endif
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            rxStatus->MetaDataNSa = pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF];
            rxStatus->MetaDataNTa = pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF];
        }
        else
        {
            ret = E_NOT_OK;
        }
#if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
        if ((rxNSduCfgPtr->NSa == CANTP_INVALID_U8) || (rxStatus->MetaDataNTa != rxNSduCfgPtr->NSa))
        {
            ret = E_NOT_OK;
        }
        if ((rxNSduCfgPtr->NTa == CANTP_INVALID_U8) || (rxStatus->MetaDataNSa != rxNSduCfgPtr->NTa))
        {
            ret = E_NOT_OK;
        }
#endif
        break;
#endif
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            rxStatus->MetaDataNSa = pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF];
            rxStatus->MetaDataNTa = pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF];
            rxStatus->MetaDataNAe = pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX];
        }
        else
        {
            ret = E_NOT_OK;
        }
        if ((E_OK == ret) && ((rxNSduCfgPtr->NAe == CANTP_INVALID_U8) || (rxStatus->MetaDataNAe != rxNSduCfgPtr->NAe)))
        {
            ret = E_NOT_OK;
        }
#if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
        else if ((rxNSduCfgPtr->NSa == CANTP_INVALID_U8) || (rxStatus->MetaDataNTa != rxNSduCfgPtr->NSa))
        {
            ret = E_NOT_OK;
        }
        else if ((rxNSduCfgPtr->NTa == CANTP_INVALID_U8) || (rxStatus->MetaDataNSa != rxNSduCfgPtr->NTa))
        {
            ret = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
#endif
        break;
#endif
    default:
        /*idle*/
        break;
    }
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
    if (CANTP_FLAGISSET(rxNSduCfgPtr->CanTpPduFlag, CANTP_CAN20_CANFD_MASK))
    {
        CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].MetaData =
            ((CanTp_CanIf_IdType)pduInfoPtr->MetaDataPtr[0]) | ((CanTp_CanIf_IdType)pduInfoPtr->MetaDataPtr[1] << 8u)
            | ((CanTp_CanIf_IdType)pduInfoPtr->MetaDataPtr[2] << 16u)
            | ((CanTp_CanIf_IdType)pduInfoPtr->MetaDataPtr[3] << 24u);
    }
#endif
    SchM_Exit_CanTp_ExclusiveArea();
    return ret;
}
/**
 * @brief Save MetaData information when setup tx session
 */
void CanTp_SaveTxMetaDataInfo(
    CanTp_TxStatusType*     txStatus,
    const CanTp_TxNSduType* txNSduCfgPtr,
    const PduInfoType*      pduInfoPtr)
{
#if (                                                                                             \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF) \
    && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF))
    CANTP_PARAM_UNUSED(pduInfoPtr);
#endif
    txStatus->MetaDataLength = (txNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
    switch (txNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        txStatus->MetaDataNTa = txNSduCfgPtr->NTa;
        txStatus->MetaDataNSa = txNSduCfgPtr->NSa;
        break;
#endif
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        txStatus->MetaDataNAe = txNSduCfgPtr->NAe;
        break;
#endif
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        if (pduInfoPtr->MetaDataPtr == NULL_PTR)
        {
            txStatus->MetaDataNTa = txNSduCfgPtr->NTa;
            txStatus->MetaDataNSa = txNSduCfgPtr->NSa;
        }
        else
        {
            txStatus->MetaDataNSa = pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_UPPER];
            txStatus->MetaDataNTa = pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_UPPER];
        }
        break;
#endif
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        if (pduInfoPtr->MetaDataPtr == NULL_PTR)
        {
            txStatus->MetaDataNTa = txNSduCfgPtr->NTa;
            txStatus->MetaDataNSa = txNSduCfgPtr->NSa;
        }
        else
        {
            txStatus->MetaDataNSa = pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_UPPER];
            txStatus->MetaDataNTa = pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_UPPER];
        }
        txStatus->MetaDataNAe = txNSduCfgPtr->NAe;
        break;
#endif
    default:
        /*idle*/
        break;
    }
}

/**
 * @brief Check the addressing information contained in the MetaData of the N-PDU when receiving CF
 */
Std_ReturnType CanTp_CheckRxCFMetaData(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    const PduInfoType*      pduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (                                                                                             \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF) \
    && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF))
    CANTP_PARAM_UNUSED(pduInfoPtr);
#endif
    rxStatus->MetaDataLength = (rxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
    switch (rxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
    {
        if (pduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX] == rxStatus->MetaDataNTa)
        {
            result = E_OK;
        }
        break;
    }
#endif
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
    {
        if (rxStatus->MetaDataNAe == pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX])
        {
            result = E_OK;
        }
        break;
    }
#endif
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        if ((pduInfoPtr->MetaDataPtr != NULL_PTR)
            && (rxStatus->MetaDataNTa == pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF])
            && (rxStatus->MetaDataNSa == pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]))
        {
            result = E_OK;
        }
        break;
#endif
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        if ((pduInfoPtr->MetaDataPtr != NULL_PTR)
            && (rxStatus->MetaDataNAe == pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX])
            && (rxStatus->MetaDataNTa == pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF])
            && (rxStatus->MetaDataNSa == pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]))
        {
            result = E_OK;
        }
        break;
#endif
    default:
        result = E_OK;
        break;
    }
    return result;
}

/**
 * @brief Check the addressing information contained in the MetaData of the N-PDU when receiving FC
 */
Std_ReturnType CanTp_CheckRxFCMetaData(
    CanTp_TxStatusType*     txStatus,
    const CanTp_TxNSduType* txNSduCfgPtr,
    const PduInfoType*      pduInfoPtr)
{
    Std_ReturnType result = E_OK;
#if (                                                                                             \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF) \
    && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF))
    CANTP_PARAM_UNUSED(pduInfoPtr);
#endif
    txStatus->MetaDataLength = (txNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
    switch (txNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        if (txStatus->MetaDataNSa != pduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX])
        {
            result = E_NOT_OK;
        }
        break;
#endif
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        if (txStatus->MetaDataNAe != pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX])
        {
            result = E_NOT_OK;
        }
        break;
#endif
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        if (pduInfoPtr->MetaDataPtr == NULL_PTR)
        {
            result = E_NOT_OK;
        }
        else if (
            (txStatus->MetaDataNSa != pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF])
            || (txStatus->MetaDataNTa != pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]))
        {
            result = E_NOT_OK;
        }
        else
        {
            /** idle */
        }
        break;
#endif
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        if (pduInfoPtr->MetaDataPtr == NULL_PTR)
        {
            result = E_NOT_OK;
        }
        else if (
            (txStatus->MetaDataNSa != pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF])
            || (txStatus->MetaDataNTa != pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF])
            || (txStatus->MetaDataNAe != pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX29]))
        {
            result = E_NOT_OK;
        }
        else
        {
            /** idle */
        }
        break;
#endif
    default:
        /** idle*/
        break;
    }
    return result;
}

/**
 * @brief Construct the addressing information contained in the MetaData of the N-PDU when transmit
 */
void CanTp_ConstructTxMetaDataInfo(
    const CanTp_TxStatusType* txStatus,
    const CanTp_TxNSduType*   txNSduCfgPtr,
    PduInfoType*              pduInfoPtr) /* PRQA S 3673 */ /* VL_CanTp_3673 */
{
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
    boolean isProcessed = FALSE;
#endif
#if (                                                                                             \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF) \
    && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF))
    CANTP_PARAM_UNUSED(txStatus);
#endif
    switch (txNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        pduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX] = txStatus->MetaDataNTa;
        break;
#endif
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] = txStatus->MetaDataNAe;
        break;
#endif
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
        isProcessed = TRUE;
#endif
        pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]      = txStatus->MetaDataNSa;
        pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF]      = txStatus->MetaDataNTa;
        pduInfoPtr->MetaDataPtr[CANTP_RESERVE_OFFSET_META_IF] = CANTP_CANID_INIT;
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
        pduInfoPtr->MetaDataPtr[CANTP_ID_OFFSET_META_IF] =
            (uint8)(CanTp_ChannelCanWithCanFD[txNSduCfgPtr->ChannelId].MetaData >> 24u);
#endif
        break;
#endif
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
        isProcessed = TRUE;
#endif
        pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]      = txStatus->MetaDataNSa;
        pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF]      = txStatus->MetaDataNTa;
        pduInfoPtr->MetaDataPtr[CANTP_RESERVE_OFFSET_META_IF] = CANTP_CANID_INIT;
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
        pduInfoPtr->MetaDataPtr[CANTP_ID_OFFSET_META_IF] =
            (uint8)(CanTp_ChannelCanWithCanFD[txNSduCfgPtr->ChannelId].MetaData >> 24u);
#endif
        pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] = txStatus->MetaDataNAe;
        break;
#endif
    default:
        /** idle */
        break;
    }
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
    if ((isProcessed == FALSE) && (CANTP_FLAGISSET(txNSduCfgPtr->CanTpPduFlag, CANTP_CAN20_CANFD_MASK)))
    {
        pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]      = CANTP_CANID_INIT;
        pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF]      = CANTP_CANID_INIT;
        pduInfoPtr->MetaDataPtr[CANTP_RESERVE_OFFSET_META_IF] = CANTP_CANID_INIT;
        pduInfoPtr->MetaDataPtr[CANTP_ID_OFFSET_META_IF] =
            (uint8)(CanTp_ChannelCanWithCanFD[txNSduCfgPtr->ChannelId].MetaData >> 24u);
    }
#endif
}

/**
 * @brief Fill the addressing information contained in the MetaData of the N-PDU when transmitS Fc.
 */
void CanTp_ConstructTxFcMetaDataInfo(
    const CanTp_RxStatusType* rxStatus,
    const CanTp_RxNSduType*   rxNSduCfgPtr,
    PduInfoType*              pduInfoPtr) /* PRQA S 3673 */ /* VL_CanTp_3673 */
{
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
    boolean isProcessed = FALSE;
#endif
#if (                                                                                             \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF) \
    && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF))
    CANTP_PARAM_UNUSED(rxStatus);
#endif
    switch (rxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        pduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX] = rxStatus->MetaDataNTa;
        break;
#endif
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] = rxStatus->MetaDataNAe;
        break;
#endif
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
        isProcessed = TRUE;
#endif
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF] = rxStatus->MetaDataNTa;
            pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF] = rxStatus->MetaDataNSa;
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
            pduInfoPtr->MetaDataPtr[CANTP_ID_OFFSET_META_IF] =
                (uint8)(CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].MetaData >> 24u);
#endif
        }
        break;
#endif
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
        isProcessed = TRUE;
#endif
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF] = rxStatus->MetaDataNTa;
            pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF] = rxStatus->MetaDataNSa;
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
            pduInfoPtr->MetaDataPtr[CANTP_ID_OFFSET_META_IF] =
                (uint8)(CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].MetaData >> 24u);
#endif
        }
        pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] = rxStatus->MetaDataNAe;
        break;
#endif
    default:
        /**idle*/
        break;
    }
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
    if ((isProcessed == FALSE) && (CANTP_FLAGISSET(rxNSduCfgPtr->CanTpPduFlag, CANTP_CAN20_CANFD_MASK)))
    {
        pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]      = CANTP_CANID_INIT;
        pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF]      = CANTP_CANID_INIT;
        pduInfoPtr->MetaDataPtr[CANTP_RESERVE_OFFSET_META_IF] = CANTP_CANID_INIT;
        pduInfoPtr->MetaDataPtr[CANTP_ID_OFFSET_META_IF] =
            (uint8)(CanTp_ChannelCanWithCanFD[rxNSduCfgPtr->ChannelId].MetaData >> 24u);
    }
#endif
}

#endif
#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/* PRQA S 1503,1532,1505,3120 -- */
