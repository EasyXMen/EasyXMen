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
**  FILENAME    : CanTp_Internal.c                                           **
**                                                                           **
**  Created on  : 2021/7/30 14:29:43                                         **
**  Author      : tao.yu                                                     **
**  Vendor      :                                                            **
**  DESCRIPTION : Internal Source file of CanTp module.                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                     **
**                                                                           **
**************************************************************************** */

/*=======[I N C L U D E S]====================================================*/
#include "CanTp_Internal.h"

#define CANTP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "CanTp_MemMap.h"
/*CANTP global variable for runtime channel controlling information*/
VAR(CanTp_ConnectionChannelType, CANTP_VAR_NOINIT) CanTp_Channels[CANTP_CHANNEL_NUMBER];
#define CANTP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "CanTp_MemMap.h"

/*=============Module Internal Function Implementation====================*/

#define CANTP_START_SEC_CODE
#include "CanTp_MemMap.h"
/*
 * Brief               Get RX-SDU configuration by received NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
CanTp_GetRxSduCfgByNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2VAR(CanTp_FramePCIType, AUTOMATIC, AUTOMATIC) SduFrameType,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) ChannelId)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7, 20.7 */
{
    const CanTp_RxNSduType* result = NULL_PTR;
    const CanTp_ChannelType* channelPtr;
    const CanTp_RxNSduType* RxNSdus;
    uint8 rxNSduIdx;
    uint8 chnIdx;
    CanTp_FramePCIType frameType;

    for (chnIdx = 0; (NULL_PTR == result) && (chnIdx < CanTp_ConfigPtr->ChannelNum); chnIdx++)
    {
        channelPtr = &CanTp_ConfigPtr->CanTpChannel[chnIdx];
        for (rxNSduIdx = 0; rxNSduIdx < channelPtr->RxNSduNum; rxNSduIdx++)
        {
            RxNSdus = &channelPtr->RxNSdus[rxNSduIdx];
            switch (RxNSdus->AddressingFormat)
            {
            case CANTP_STANDARD:
            case CANTP_NORMALFIXED:
                frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_STD] & CANTP_FTYPE_MASK;
                if ((RxNSdus->RxNPduId == CanTpRxPduId)
                    && ((CANTP_FTYPE_CF == frameType) || (CANTP_FTYPE_SF == frameType)
                        || (CANTP_FTYPE_FF == frameType)))
                {
                    result = RxNSdus;
                    *SduFrameType = frameType;
                    *ChannelId = chnIdx;
                }
                break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
            case CANTP_EXTENDED:
            case CANTP_MIXED:
            case CANTP_MIXED29BIT:
                if (CanTpRxPduPtr->SduLength > CANTP_PCI_OFFSET_EX)
                {
                    /*check this RX-SDU, which is configured as mix11 addressing
                     * format, and the CAN data at least includes AE and PCIType*/
                    /*check its TA and frame type*/
                    frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_EX] & CANTP_FTYPE_MASK;
                    if ((RxNSdus->RxNPduId == CanTpRxPduId)
                        && ((CANTP_FTYPE_CF == frameType) || (CANTP_FTYPE_SF == frameType)
                            || (CANTP_FTYPE_FF == frameType)))
                    {
                        result = RxNSdus;
                        *SduFrameType = frameType;
                        *ChannelId = chnIdx;
                    }
                }
                break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
 || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
            default:
                /*idle*/
                break;
            }
        }
    }
    return result;
}

/*
 * Brief               Get SFF_DL information from the data content of SF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(PduLengthType, CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
CanTp_RxGetSFDl(
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
#if (STD_ON == CANTP_FD)
    P2VAR(uint8, AUTOMATIC, CANTP_CONST) dataoffset,
#endif /* STD_ON == CANTP_FD */
    P2VAR(uint8, AUTOMATIC, CANTP_CONST) totaloffset)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
{
    uint8 sfDl;
    uint8 offset = 0xFF;

    /*check SF DL*/
    switch (RxNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        /*PCI is byte[0]*/
        offset = CANTP_PCI_OFFSET_STD;
        *totaloffset = 1;
        break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_EXTENDED:
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
        /*PCI is byte[1]*/
        offset = CANTP_PCI_OFFSET_EX;
        *totaloffset = 2;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
 || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*No further deal*/
        break;
    }
    sfDl = PduInfoPtr->SduDataPtr[offset] & CANTP_SF_DL_MASK;
#if (STD_ON == CANTP_FD)
    if ((0x0u == sfDl) && (8u < PduInfoPtr->SduLength))
    {
        /*CanFD only*/
        if (CANTP_CAN_FD == RxNSduCfgPtr->CanFrameType)
        {
            sfDl = PduInfoPtr->SduDataPtr[offset + 1u];
            *totaloffset += 1u;
        }
    }
    else if ((0x0u != sfDl) && (8u < PduInfoPtr->SduLength))
    {
        sfDl = 0; /*invalid*/
    }
    else
    {
        /*idle*/
    }
    *dataoffset = offset;
#endif /* STD_ON == CANTP_FD */

    return sfDl;
}

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               store Rx meta data info.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(Std_ReturnType, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_SaveRxMetaDataInfo(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    CanTp_ConnectionType* RxchannelPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr)
{
    Std_ReturnType ret = E_OK;
    SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
#if (                                                                                             \
    (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
    RxchannelPtr->MetaDataLength = (RxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
#else
    CANTP_UNUSED(RxchannelPtr);
    CANTP_UNUSED(PduInfoPtr);
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
    || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON   \
    || CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON      \
    || CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
    switch (RxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        RxchannelPtr->MetaDataN_TA = PduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX];
        if ((RxNSduCfgPtr->RxTaType == CANTP_PHYSICAL_RX) || (RxNSduCfgPtr->RxTaType == CANTP_CANFD_PHYSICAL))
        {
            if (RxNSduCfgPtr->NSa == NULL_PTR)
            {
                ret = E_NOT_OK;
            }
            else if (RxchannelPtr->MetaDataN_TA != *(RxNSduCfgPtr->NSa))
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
        RxchannelPtr->MetaDataN_AE = PduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX];
        if ((RxNSduCfgPtr->NAe == NULL_PTR) || (RxchannelPtr->MetaDataN_AE != *(RxNSduCfgPtr->NAe)))
        {
            ret = E_NOT_OK;
        }
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        if (PduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            RxchannelPtr->MetaDataN_SA = PduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF];
            RxchannelPtr->MetaDataN_TA = PduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF];
        }
#if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
        if ((RxNSduCfgPtr->NSa == NULL_PTR) || (RxchannelPtr->MetaDataN_TA != *(RxNSduCfgPtr->NSa)))
        {
            ret = E_NOT_OK;
        }
        if ((RxNSduCfgPtr->NTa == NULL_PTR) || (RxchannelPtr->MetaDataN_SA != *(RxNSduCfgPtr->NTa)))
        {
            ret = E_NOT_OK;
        }
#endif
        break;
#endif /* CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        if (PduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            RxchannelPtr->MetaDataN_SA = PduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF];
            RxchannelPtr->MetaDataN_TA = PduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF];
            RxchannelPtr->MetaDataN_AE = PduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX];
        }

#if (CANTP_GENERIC_CONNECTION_SUPPORT != STD_ON)
        if ((*(RxNSduCfgPtr->NSa) == NULL_PTR) || (RxchannelPtr->MetaDataN_TA != *(RxNSduCfgPtr->NSa)))
        {
            ret = E_NOT_OK;
        }
        else if ((RxNSduCfgPtr->NTa == NULL_PTR) || (RxchannelPtr->MetaDataN_SA != *(RxNSduCfgPtr->NTa)))
        {
            ret = E_NOT_OK;
        }
        else if ((RxNSduCfgPtr->NAe == NULL_PTR) || (RxchannelPtr->MetaDataN_AE != *(RxNSduCfgPtr->NAe)))
        {
            ret = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
#endif
        break;
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
    SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
    return ret;
}
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Get FF_DL information from the data content of FF.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(PduLengthType, CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7, 20.7 */
CanTp_RxGetFFDl(
    P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_CONST) offset,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7, 20.7 */
{
    /*because PduLengthType is uint16, so FFDL can not bigger than 65535*/
    uint32 ffDl;
    uint8 pcioffest = 0xFF;

    switch (RxNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        pcioffest = CANTP_PCI_OFFSET_STD;
        *offset = CANTP_FF_DATA_OFFSET_LT4095_STD;
        break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_EXTENDED:
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
        pcioffest = CANTP_PCI_OFFSET_EX;
        *offset = CANTP_FF_DATA_OFFSET_LT4095_EX;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
|| CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
    ffDl = (uint32)PduInfoPtr[pcioffest] & (uint32)CANTP_FF_DL_MASK;
    ffDl <<= CANTP_OFFSET_BYTE_PART;
    ffDl += PduInfoPtr[pcioffest + 1u];
#ifndef CANTP_MATCH_ISO_2004
    /*FFDL > 4095*/
    if (0x0u == ffDl)
    {

        ffDl = PduInfoPtr[pcioffest + 2u];
        ffDl <<= CANTP_OFFSET_BYTE_PART;
        ffDl += PduInfoPtr[pcioffest + 3u];
        ffDl <<= CANTP_OFFSET_BYTE_PART;
        ffDl += PduInfoPtr[pcioffest + 4u];
        ffDl <<= CANTP_OFFSET_BYTE_PART;
        ffDl += PduInfoPtr[pcioffest + 5u];
        if (ffDl <= CANTP_FF_DL_12bit_LENGTH)
        {
            ffDl = 0;
        }
        else
        {
            *offset += 4u;
        }
    }
#endif /* CANTP_MATCH_ISO_2004 */
    return (PduLengthType)ffDl;
}

/*
 * Brief               Calculate the BS value according to
 *  the buffer length and remain data length.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_CalcBS(P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr, CanTp_ConnectionType* RxchannelPtr)
/* PRQA S 1532-- */ /* MISRA Rule 8.7 */
{
#ifndef CANTP_FIX_BS
    PduLengthType blockDataLen;
    uint8 length = 0xFF;
#if (STD_ON == CANTP_FD)
    uint8 offset = 0xFF;
#endif /* STD_ON == CANTP_FD */
#endif
    uint8 bs;

    /*check changParameter of BS is done or not*/
    /* PRQA S 3432++ */ /* MISRA Rule 20.7 */
    bs = CANTP_FLAGISSET(RxchannelPtr->EventFlags, CANTP_EVENT_RXBSCHANGED)
             ? RxchannelPtr->ChangedBs
             : ((RxNSduCfgPtr->Bs != NULL_PTR) ? *(RxNSduCfgPtr->Bs) : 0x0u);
    /* PRQA S 3432-- */ /* MISRA Rule 20.7 */

    if (0u == bs)
    {
        /*BS is 0, no more FC need from now on*/
        RxchannelPtr->CurrentBs = 0;
    }
    else
    {
#ifndef CANTP_FIX_BS
        /*get the data length of this block using the minimum value between PDUR buffer length and SDU remain
        data length. If the minimum value is zero, I.E. the PDUR remain buffer is 0, use 1 as the BS*/
        blockDataLen = RxchannelPtr->SduDataRemaining;

        if (0u == blockDataLen)
        {
            /*no more upper buffer space left */
            RxchannelPtr->CurrentBs = 1U;
        }
        else
        {
            switch (RxNSduCfgPtr->AddressingFormat)
            {
            case CANTP_STANDARD:
            case CANTP_NORMALFIXED:
                length = CANTP_CF_DATALEN_STD;
#if (STD_ON == CANTP_FD)
                offset = CANTP_CF_DATA_OFFSET_STD;
#endif /* STD_ON == CANTP_FD */
                break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
            case CANTP_EXTENDED:
            case CANTP_MIXED:
            case CANTP_MIXED29BIT:
                length = CANTP_CF_DATALEN_EX;
#if (STD_ON == CANTP_FD)
                offset = CANTP_CF_DATA_OFFSET_EX;
#endif /* STD_ON == CANTP_FD */
                break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
|| CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
            default:
                /*idle*/
                break;
            }

            /*calculate CF block,CAN2.0 and CANFD is the same format,just different with length*/
            SchM_Enter_CanTp_ExclusiveArea(); /*lock channel*/
            if (CANTP_CAN_20 == RxNSduCfgPtr->CanFrameType)
            {
                RxchannelPtr->CurrentBs = (uint8)((blockDataLen + length - 1U) / length);
            }
#if (STD_ON == CANTP_FD)
            else
            {
                /*CANTP_CAN_FD*/
                RxchannelPtr->CurrentBs =
                    (uint8)((blockDataLen + RxchannelPtr->FDDlc - offset - 1U) / (RxchannelPtr->FDDlc - offset));
            }
#endif /* STD_ON == CANTP_FD */
            /*use the minimum value between calculated and the configured maximum value*/
            if (RxchannelPtr->CurrentBs > bs)
            {
                RxchannelPtr->CurrentBs = bs;
            }
            SchM_Exit_CanTp_ExclusiveArea(); /*unlock channel*/
        }
#else
        RxchannelPtr->CurrentBs = ((RxNSduCfgPtr->Bs != NULL_PTR) ? *(RxNSduCfgPtr->Bs) : 0x0u);
#endif /* CANTP_FIX_BS */
    }  /*END OF BS needed*/
}

/*
 * Brief               Compare Buffer With Bs
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(Std_ReturnType, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_CompareBufferWithBs(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    const CanTp_RxNSduType* RxNSduCfgPtr,
    const CanTp_ConnectionType* RxchannelPtr,
    PduLengthType bufferSize)
{
    PduLengthType blockDataLen;
    PduLengthType offset = 0xFF;
    Std_ReturnType ret = E_NOT_OK;

    switch (RxNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        offset = CANTP_CF_DATA_OFFSET_STD;
        break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_EXTENDED:
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
        offset = CANTP_CF_DATA_OFFSET_EX;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
|| CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }

    /*CF composed of this block,CAN2.0 and CANFD is the same format,just different with length*/
    if (CANTP_CAN_20 == RxNSduCfgPtr->CanFrameType)
    {
        blockDataLen = (PduLengthType)RxchannelPtr->CurrentBs * (CANTP_CAN20_FRAME_LEN_MAX - offset);
    }
#if (STD_ON == CANTP_FD)
    else
    {
        /*CANTP_CAN_FD*/
        blockDataLen = (PduLengthType)RxchannelPtr->CurrentBs * (RxchannelPtr->FDDlc - offset);
    }
#else  /* STD_ON == CANTP_FD */
    else
    {
        blockDataLen = 0xFFFFu;
    }
#endif /* STD_ON == CANTP_FD */
    if (bufferSize > blockDataLen)
    {
        ret = E_OK;
    }
    else
    {
        if (bufferSize >= RxchannelPtr->SduDataRemaining)
        {
            ret = E_OK;
        }
    }

    return ret;
}

/*
 * Brief               send FC
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(Std_ReturnType, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_SendFC(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    const CanTp_RxNSduType* RxNSduCfgPtr,
    const CanTp_ConnectionType* RxchannelPtr,
    CanTp_FCInfoType CanTp_FC)

{
    PduInfoType pduInfo;
    PduLengthType offset = 0xFF;
    uint16 fcFrameLen = 0u;
    Std_ReturnType ret;
#if (STD_ON == CANTP_FD)
    uint8 FcBuf[CANTP_CANFD_FRAME_LEN_MAX];
#else  /* STD_ON == CANTP_FD */
    uint8 FcBuf[CANTP_CAN20_FRAME_LEN_MAX];
#endif /* STD_ON == CANTP_FD */

    SchM_Enter_CanTp_ExclusiveArea();
#if (                                                                                                       \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF)            \
    && (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF) \
    && (CANTP_DYN_ID_SUPPORT == STD_OFF))
    CANTP_UNUSED(RxNSduCfgPtr);
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF && CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF       \
     && CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF && CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF \
    && CANTP_DYN_ID_SUPPORT == STD_OFF */
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
    uint8 Metadata[4] = {0, 0, 0, 0};

    pduInfo.MetaDataPtr = Metadata;
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
    switch (RxNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        offset = CANTP_PCI_OFFSET_STD;
        fcFrameLen = (uint16)CANTP_FC_LEN_STD;
        break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_EXTENDED:
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
        offset = CANTP_PCI_OFFSET_EX;
        fcFrameLen = (uint16)CANTP_FC_LEN_EX;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
 || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
    if (offset != (PduLengthType)0xFF)
    {
        FcBuf[offset] = CANTP_FTYPE_FC | CanTp_FC.FcFs;
        FcBuf[offset + 1u] = CanTp_FC.FcBS;
        FcBuf[offset + 2u] = (uint8)CanTp_FC.FcSTMin;
    }
#if (CANTP_DYN_ID_SUPPORT == STD_ON)
    switch (RxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        FcBuf[CANTP_TA_OFFSET_META_EX] = RxchannelPtr->MetaDataN_SA;
        break;
#endif /* CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        FcBuf[CANTP_AE_OFFSET_META_MIX] = RxchannelPtr->MetaDataN_AE;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        Metadata[CANTP_SA_OFFSET_META_IF] = RxchannelPtr->MetaDataN_TA;
        Metadata[CANTP_TA_OFFSET_META_IF] = RxchannelPtr->MetaDataN_SA;
        break;
#endif /* CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        Metadata[CANTP_SA_OFFSET_META_IF] = RxchannelPtr->MetaDataN_TA;
        Metadata[CANTP_TA_OFFSET_META_IF] = RxchannelPtr->MetaDataN_SA;
        FcBuf[CANTP_AE_OFFSET_META_MIX29] = RxchannelPtr->MetaDataN_AE;
        break;
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */
    if (CANTP_PADDING_ON == RxNSduCfgPtr->RxPaddingActivation)
    {
        CanTp_MemorySet(&FcBuf[fcFrameLen], CANTP_PADDING_BYTE, ((uint16)CANTP_CAN20_FRAME_LEN_MAX - fcFrameLen));
        fcFrameLen = (uint16)CANTP_CAN20_FRAME_LEN_MAX;
    }
    pduInfo.SduLength = (PduLengthType)fcFrameLen;
    pduInfo.SduDataPtr = FcBuf;
    SchM_Exit_CanTp_ExclusiveArea();

    ret = CanIf_Transmit(RxNSduCfgPtr->TxFcLPduId, &pduInfo);
#if (                                                                                           \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT != STD_ON) && (CANTP_MIXED_ADDRESSING_SUPPORT != STD_ON) \
    && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT != STD_ON) && (CANTP_MIXED29_ADDRESSING_SUPPORT != STD_ON))
    CANTP_UNUSED(RxchannelPtr);
#endif
    return ret;
}

/*
 * Brief               Utility function: Set memory with specific value.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 3432++ */ /* MISRA Rule 20.7 */
CanTp_MemorySet(P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) Address, uint8 Value, uint16 Length)
/* PRQA S 3432-- */ /* MISRA Rule 20.7 */
{
    uint16 DL;
    for (DL = 0; DL < Length; DL++)
    {
        Address[DL] = Value;
    }
}

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               check RxCF meta data
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(Std_ReturnType, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_CheckRxCFMetaData(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    CanTp_ConnectionType* RxchannelPtr,
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr)
{
    Std_ReturnType result = E_NOT_OK;
#if (                                                                                             \
    (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
    RxchannelPtr->MetaDataLength = (RxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
#else
    CANTP_UNUSED(RxchannelPtr);
    CANTP_UNUSED(PduInfoPtr);
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON \
 || CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
    switch (RxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED: {
        if (PduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX] == RxchannelPtr->MetaDataN_TA)
        {
            result = E_OK;
        }
        break;
    }
#endif /* CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED: {
        if (RxchannelPtr->MetaDataN_AE == PduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX])
        {
            result = E_OK;
        }
        break;
    }
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        if ((PduInfoPtr->MetaDataPtr != NULL_PTR)
            && (RxchannelPtr->MetaDataN_TA == PduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF])
            && (RxchannelPtr->MetaDataN_SA == PduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]))
        {
            result = E_OK;
        }
        break;
#endif /* CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        if ((PduInfoPtr->MetaDataPtr != NULL_PTR)
            && (RxchannelPtr->MetaDataN_AE == PduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX])
            && (RxchannelPtr->MetaDataN_TA == PduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF])
            && (RxchannelPtr->MetaDataN_SA == PduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]))
        {
            result = E_OK;
        }
        break;
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        result = E_OK;
        break;
    }
    return result;
}
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Get CF Offset
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7, 20.7 */
CanTp_RxGetCFOffset(
    P2CONST(CanTp_RxNSduType, AUTOMATIC, CANTP_CONST) RxNSduCfgPtr,
    P2VAR(uint8, AUTOMATIC, CANTP_CONST) dataoffset,
    P2VAR(uint8, AUTOMATIC, CANTP_CONST) pcioffset)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7, 20.7 */
{
    switch (RxNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        *pcioffset = CANTP_PCI_OFFSET_STD;
        *dataoffset = CANTP_CF_DATA_OFFSET_STD;
        break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_EXTENDED:
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
        *pcioffset = CANTP_PCI_OFFSET_EX;
        *dataoffset = CANTP_CF_DATA_OFFSET_EX;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
|| CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
}

/*
 * Brief               Get TX-SDU configuration by received FC NPDU information.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST), CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7, 20.7 */
CanTp_GetTxSduCfgByFCNpdu(
    PduIdType CanTpRxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) CanTpRxPduPtr,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) ChannelId)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7, 20.7 */
{
    const CanTp_TxNSduType* result = NULL_PTR;
    const CanTp_TxNSduType* TxNSdus;
    const CanTp_ChannelType* channelPtr;
    uint8 txNSduIdx;
    uint8 chnIdx;
    CanTp_FramePCIType frameType;

    for (chnIdx = 0; (NULL_PTR == result) && (chnIdx < CanTp_ConfigPtr->ChannelNum); chnIdx++)
    {
        channelPtr = &CanTp_ConfigPtr->CanTpChannel[chnIdx];
        for (txNSduIdx = 0; (NULL_PTR == result) && (txNSduIdx < channelPtr->TxNsduNum); txNSduIdx++)
        {
            /*only physical TX NSDU need configure RX FCNPDUID*/
            TxNSdus = &channelPtr->TxNSdus[txNSduIdx];
            if ((CANTP_PHYSICAL_TX == TxNSdus->TxTaType) && (TxNSdus->RxFcNPduId == CanTpRxPduId))
            {
                switch (TxNSdus->AddressingFormat)
                {
                case CANTP_STANDARD:
                case CANTP_NORMALFIXED:
                    /*check the standard addressing TX-NSDU, only FC accepted*/
                    frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_STD] & CANTP_FTYPE_MASK;
                    if (CANTP_FTYPE_FC == frameType)
                    {
                        result = TxNSdus;
                        *ChannelId = chnIdx;
                    }
                    break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
                case CANTP_MIXED:
                case CANTP_MIXED29BIT:
                case CANTP_EXTENDED:
                    if (CanTpRxPduPtr->SduLength > CANTP_PCI_OFFSET_MIX11)
                    {
                        /*check the mix11 addressing TX-NSDU, only FC accepted*/
                        frameType = CanTpRxPduPtr->SduDataPtr[CANTP_PCI_OFFSET_MIX11] & CANTP_FTYPE_MASK;
                        if (CANTP_FTYPE_FC == frameType)
                        {
                            result = TxNSdus;
                            *ChannelId = chnIdx;
                        }
                    }
                    else
                    {
                        /*configured as extended addressing format, but the received NPDU not has enough data space*/
                    }
                    break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
 || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
                default:
                    /*idle*/
                    break;
                } /*END OF switch*/
            }     /*END OF if (CANTP_PHYSICAL == CanTp_CfgData.TxNSdus[txNSduIdx].TxTaType)*/
        }         /*END OF for loop*/
    }             /*END OF for loop*/
    return result;
}

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               check RxFc meta data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(Std_ReturnType, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_CheckRxFCMetaData(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    CanTp_ConnectionType* TxSubchannelPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr)
{
    Std_ReturnType result = E_OK;
#if (                                                                                             \
    (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
    TxSubchannelPtr->MetaDataLength = (TxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
#else
    CANTP_UNUSED(TxSubchannelPtr);
    CANTP_UNUSED(PduInfoPtr);
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON \
 || CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
    switch (TxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        if (TxSubchannelPtr->MetaDataN_SA != PduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX])
        {
            result = E_NOT_OK;
        }
        break;
#endif /* CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        if (TxSubchannelPtr->MetaDataN_AE != PduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX])
        {
            result = E_NOT_OK;
        }
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        if (PduInfoPtr->MetaDataPtr == NULL_PTR)
        {
            result = E_NOT_OK;
        }
        else if (
            (TxSubchannelPtr->MetaDataN_SA != PduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF])
            || (TxSubchannelPtr->MetaDataN_TA != PduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF]))
        {
            result = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
        break;
#endif /* CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        if (PduInfoPtr->MetaDataPtr == NULL_PTR)
        {
            result = E_NOT_OK;
        }
        else if (
            (TxSubchannelPtr->MetaDataN_SA != PduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF])
            || (TxSubchannelPtr->MetaDataN_TA != PduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF])
            || (TxSubchannelPtr->MetaDataN_AE != PduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX29]))
        {
            result = E_NOT_OK;
        }
        else
        {
            /* idle */
        }
        break;
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
    return result;
}
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Get the information of received FC frame.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7, 20.7 */
CanTp_GetChannelFCInfo(
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr,
    P2VAR(CanTp_FCInfoType, AUTOMATIC, AUTOMATIC) FcInfoPtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7, 20.7 */
{
    uint32 stMinUs;
    PduLengthType offset = 0;
    uint8 stMinRaw;

    switch (TxNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        offset = CANTP_PCI_OFFSET_STD;
        break;
#if (                                                                                          \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_EXTENDED:
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
        offset = CANTP_PCI_OFFSET_EX;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON \
 || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
    FcInfoPtr->FcFs = PduInfoPtr->SduDataPtr[offset] & CANTP_FC_FS_MASK;
    FcInfoPtr->FcBS = PduInfoPtr->SduDataPtr[offset + 1u];
    /*calculate the STMin in tick unit. Round the value to the nearest integer, that is
     * if it is 0.49, then the integer will be 0, 0.50 will be 1.*/
    stMinRaw = PduInfoPtr->SduDataPtr[offset + 2u];
    if (CANTP_FC_STMIN_MS_LIMIT >= stMinRaw)
    {
        /*ms per bit*/
        stMinUs = (uint32)stMinRaw * CANTP_FC_STMIN_MS_UNIT;
    }
    else if ((CANTP_FC_STMIN_US_LOW <= stMinRaw) && (CANTP_FC_STMIN_US_UPPER >= stMinRaw))
    {
        /*F1-F9 100us per step*/
        stMinUs = ((uint32)((uint32)stMinRaw - 0xF0UL)) * CANTP_FC_STMIN_US_UNIT;
    }
    else
    {
        /*invalid value, use 7F*/
        stMinUs = (uint32)CANTP_FC_STMIN_MS_LIMIT * CANTP_FC_STMIN_MS_UNIT;
    }
    /*transit to value in unit tick*/
    FcInfoPtr->FcSTMin = (stMinUs + (uint32)((uint32)1000 >> (uint32)1)) / (uint32)1000;
}

/*
 * Brief               check Tx Length In Address Mode.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(Std_ReturnType, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_CheckTxLengthInAddressMode(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    switch (TxNSduCfgPtr->TxTaType)
    {
        /*functional addressing mode can only use SF*/
    case CANTP_FUNCTIONAL_TX:
        switch (TxNSduCfgPtr->AddressingFormat)
        {
        case CANTP_STANDARD:
        case CANTP_NORMALFIXED:
            /* CAN2.0 or CANFD*/
            if ((CANTP_SF_LEN_MAX_STD >= PduInfoPtr->SduLength)
                || ((CANTP_CAN_FD == TxNSduCfgPtr->CanFrameType)
                    && ((TxNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_STD) > PduInfoPtr->SduLength)))
            {
                /*transmit each sdu length should less than DLC which configuration in Canif module*/
                ret = E_OK;
            }
            break;
#if (                                                                                           \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
        case CANTP_EXTENDED:
        case CANTP_MIXED:
        case CANTP_MIXED29BIT:
            /* CAN2.0 or CANFD*/
            if ((CANTP_SF_LEN_MAX_EX >= PduInfoPtr->SduLength)
                || ((CANTP_CAN_FD == TxNSduCfgPtr->CanFrameType)
                    && ((TxNSduCfgPtr->TxNPduDLC - CANTP_SF_DATA_OFFSET_CANFD_EX) > PduInfoPtr->SduLength)))
            {
                /*transmit each sdu length should less than DLC which configuration in Canif module*/
                ret = E_OK;
            }
            break;
#endif /* CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
 || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
        default:
            /*idle*/
            break;
        }
        break;
    case CANTP_PHYSICAL_TX:
        ret = E_OK;
        break;
    default:
        /*idle*/
        break;
    }

    return ret;
}

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               store Tx meta data info.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_SaveTxMetaDataInfo(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    CanTp_ConnectionType* TxSubchannelPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_CONST) PduInfoPtr)
{
#if (                                                                                             \
    (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON) || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON))
    TxSubchannelPtr->MetaDataLength = (TxNSduCfgPtr->CanTpPduFlag & CANTP_METADATA_LENGTH_MASK);
#else
    CANTP_UNUSED(PduInfoPtr);
#if (CANTP_MIXED_ADDRESSING_SUPPORT != STD_ON)
    CANTP_UNUSED(TxSubchannelPtr);
#endif
#endif
    switch (TxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        TxSubchannelPtr->MetaDataN_TA = *(TxNSduCfgPtr->NTa);
        break;
#endif /* CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        TxSubchannelPtr->MetaDataN_AE = *(TxNSduCfgPtr->NAe);
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
        break;
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
        if (PduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            TxSubchannelPtr->MetaDataN_SA = PduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_UPPER];
            TxSubchannelPtr->MetaDataN_TA = PduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_UPPER];
        }
#else  /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
        TxSubchannelPtr->MetaDataN_TA = *(TxNSduCfgPtr->NTa);
        TxSubchannelPtr->MetaDataN_SA = *(TxNSduCfgPtr->NSa);
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */

        break;
#endif /* CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
        if (PduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            TxSubchannelPtr->MetaDataN_SA = PduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_UPPER];
            TxSubchannelPtr->MetaDataN_TA = PduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_UPPER];
        }
#else /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
        TxSubchannelPtr->MetaDataN_TA = *(TxNSduCfgPtr->NTa);
        TxSubchannelPtr->MetaDataN_SA = *(TxNSduCfgPtr->NSa);

#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
        TxSubchannelPtr->MetaDataN_AE = *(TxNSduCfgPtr->NAe);
        break;
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
}
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Handle transmit request.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_TxHandleTransmitReq(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    CanTp_ConnectionType* TxSubchannelPtr,
    P2CONST(CanTp_TxNSduType, AUTOMATIC, CANTP_CONST) TxNSduCfgPtr
#if (CANTP_SYNCHRONOUS_TRANSMIT == STD_ON)
    ,
    uint8 ChannelId
#endif
)
{
    PduLengthType len;
    uint8 offset = 0xFF;
#if (CANTP_SYNCHRONOUS_TRANSMIT == STD_ON)
    CanTp_ConnectionChannelType* channelPtr = &CanTp_Channels[ChannelId];
#endif

    switch (TxNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        /*Can2.0 or small Can Fd*/
        offset = CANTP_SF_DATA_OFFSET_STD;
#if (STD_ON == CANTP_FD)
        if (CANTP_CAN_FD == TxNSduCfgPtr->CanFrameType)
        {
            /*small CanFD data offset same as Can 2.0,CANTP_SF_DATA_OFFSET_STD,small
            Can FD data field length less than large Can FD, so len = bigger one*/
            if (TxSubchannelPtr->SduDataTotalCount > 7u)
            {
                offset = CANTP_SF_DATA_OFFSET_CANFD_STD;
            }
        }
#endif /* STD_ON == CANTP_FD */
        break;
#if (                                                                                           \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
    case CANTP_EXTENDED:
        offset = CANTP_SF_DATA_OFFSET_EX;
#if (STD_ON == CANTP_FD)
        if (CANTP_CAN_FD == TxNSduCfgPtr->CanFrameType)
        {
            /*small CanFD data offset same as Can 2.0,CANTP_SF_DATA_OFFSET_STD,small
            Can FD data field length less than large Can FD, so len = bigger one*/
            if (TxSubchannelPtr->SduDataTotalCount > 6u)
            {
                offset = CANTP_SF_DATA_OFFSET_CANFD_EX;
            }
        }
#endif /* STD_ON == CANTP_FD */
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON \
|| CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
    if (offset != 0xFFu)
    {
        len = TxNSduCfgPtr->TxNPduDLC - offset;
        if (len < TxSubchannelPtr->SduDataTotalCount)
        {
            /*larger SDU transmit request*/
            TxSubchannelPtr->HandleType = CANTP_FTYPE_FF;
            TxSubchannelPtr->RootState = CANTP_LARGE_TRANSMITTING;
            TxSubchannelPtr->Substate = CANTP_TX_LARGE_START;
#if (CANTP_SYNCHRONOUS_TRANSMIT == STD_ON)
            CanTp_TxHandleLargeStart(channelPtr);
#endif
        }
        else
        {
            /*SF transmit request*/
            TxSubchannelPtr->HandleType = CANTP_FTYPE_SF;
            TxSubchannelPtr->RootState = CANTP_SF_TRANSIMITTING;
            TxSubchannelPtr->Substate = CANTP_TX_SF_START;
#if (CANTP_SYNCHRONOUS_TRANSMIT == STD_ON)
            CanTp_TxHandleSFStart(channelPtr);
#endif
        }
    }
}
/*
 * Brief               construct FF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
CanTp_ConstructFFPci(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* txNSduCfgPtr,
    uint8* dataPtr,
    uint8* totalOffset)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    uint32 pciDl;
    uint8 offset = 0xFF;

    pciDl = TxSubchannelPtr->SduDataTotalCount;
    switch (txNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        /*Can2.0 or small Can Fd*/
        offset = CANTP_PCI_OFFSET_STD;
        break;
#if (                                                                                           \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
    case CANTP_EXTENDED:
        offset = CANTP_PCI_OFFSET_EX;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON \
 || CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }

    if (pciDl <= CANTP_FF_DL_12bit_LENGTH)
    {
        /*FF_DL less than or equal to 4095*/
        /*SDU length high nibble and Frame type*/
        dataPtr[offset] = CANTP_FTYPE_FF | (uint8)(pciDl >> 8u);
        offset++;
        /*SDU length low nibble*/
        dataPtr[offset] = (uint8)pciDl;
        offset++;
        *totalOffset = offset;
    }
    else
    {
        /*FF_DL bigger than 4095*/
        /*SDU length from high to low nibble*/
        dataPtr[offset] = CANTP_FTYPE_FF;
        offset++;
        dataPtr[offset] = 0x00;
        offset++;
        dataPtr[offset] = (uint8)(pciDl >> 24u);
        offset++;
        dataPtr[offset] = (uint8)(pciDl >> 16u);
        offset++;
        dataPtr[offset] = (uint8)(pciDl >> 8u);
        offset++;
        dataPtr[offset] = (uint8)pciDl;
        offset++;
        *totalOffset = offset;
    }
}
/*
 * Brief               construct CF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
CanTp_ConstructCFPci(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* txNSduCfgPtr,
    uint8* dataPtr,
    uint8* totalOffset)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    uint8 offset = 0x00u;

    switch (txNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        /*Can2.0 or small Can Fd*/
        offset = CANTP_PCI_OFFSET_STD;
        break;
#if (                                                                                           \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
    case CANTP_EXTENDED:
        offset = CANTP_PCI_OFFSET_EX;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON \
|| CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
    dataPtr[offset] = CANTP_FTYPE_CF | TxSubchannelPtr->CurrentCfSn;
    offset++;
    *totalOffset = offset;
}

/*
 * Brief               construct SF PCI information in local buffer for channel.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
CanTp_ConstructSFPci(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* txNSduCfgPtr,
    uint8* dataPtr,
    uint8* totalOffset)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    PduLengthType pciDl;
#if (STD_ON == CANTP_FD)
    PduLengthType CANDL;
#endif /* STD_ON == CANTP_FD */
    uint8 offset = 0xFF;

    pciDl = TxSubchannelPtr->SduDataTotalCount;
#if (STD_ON == CANTP_FD)
    if ((pciDl >= 8u) && (pciDl <= 10u))
    {
        CANDL = 12;
    }
    else if ((pciDl >= 11u) && (pciDl <= 14u))
    {
        CANDL = 16;
    }
    else if ((pciDl >= 15u) && (pciDl <= 18u))
    {
        CANDL = 20;
    }
    else if ((pciDl >= 19u) && (pciDl <= 22u))
    {
        CANDL = 24;
    }
    else if ((pciDl >= 23u) && (pciDl <= 30u))
    {
        CANDL = 32;
    }
    else if ((pciDl >= 31u) && (pciDl <= 46u))
    {
        CANDL = 48;
    }
    else if ((pciDl >= 47u) && (pciDl <= 62u))
    {
        CANDL = 64;
    }
    else
    {
        CANDL = 8;
    }
#endif /* STD_ON == CANTP_FD */

    switch (txNSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        /*Can2.0 or small Can Fd*/
        offset = CANTP_PCI_OFFSET_STD;
        break;
#if (                                                                                           \
    (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
    case CANTP_EXTENDED:
        offset = CANTP_PCI_OFFSET_EX;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON || CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON \
|| CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }

#if (STD_ON == CANTP_FD)
    if (CANDL <= 8u)
#endif /* STD_ON == CANTP_FD */
    {
        /*SF_DL less than or equal to 7, Frame type and SF DL*/
        dataPtr[offset] = ((uint8)pciDl & CANTP_SF_DL_MASK); /*CANTP_FTYPE_SF*/
        offset++;
        *totalOffset = offset;
    }
#if (STD_ON == CANTP_FD)
    else
    {
        /*SF_DL bigger than 7, Frame type and SF DL*/
        dataPtr[offset] = CANTP_FTYPE_SF;
        offset++;
        dataPtr[offset] = (uint8)pciDl;
        offset++;
        *totalOffset = offset;
    }
#endif /* STD_ON == CANTP_FD */
}

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/*
 * Brief               Construct Tx meta data info.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_ConstructTxMetaDataInfo(
    /* PRQA S 1532-- */ /* MISRA Rule 8.7 */
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* TxNSduCfgPtr,
    const PduInfoType* pduInfoPtr)
{
#if (                                                                                             \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF) \
    && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF))
    CANTP_UNUSED(TxSubchannelPtr);
    CANTP_UNUSED(pduInfoPtr);
#endif /* CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_OFF && CANTP_MIXED_ADDRESSING_SUPPORT == STD_OFF \
    && CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF && CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF */
    switch (TxNSduCfgPtr->AddressingFormat)
    {
#if (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_EXTENDED:
        pduInfoPtr->SduDataPtr[CANTP_TA_OFFSET_META_EX] = TxSubchannelPtr->MetaDataN_TA;
        break;
#endif /* CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED:
        pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] = TxSubchannelPtr->MetaDataN_AE;
        break;
#endif /* CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_NORMALFIXED:
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF] = TxSubchannelPtr->MetaDataN_SA;
            pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF] = TxSubchannelPtr->MetaDataN_TA;
        }
        break;
#endif /* CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_ON */
#if (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON)
    case CANTP_MIXED29BIT:
        if (pduInfoPtr->MetaDataPtr != NULL_PTR)
        {
            pduInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_IF] = TxSubchannelPtr->MetaDataN_SA;
            pduInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_IF] = TxSubchannelPtr->MetaDataN_TA;
        }
        pduInfoPtr->SduDataPtr[CANTP_AE_OFFSET_META_MIX] = TxSubchannelPtr->MetaDataN_AE;
        break;
#endif /* CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }
}
#endif /* CANTP_DYN_ID_SUPPORT == STD_ON */

/*
 * Brief               Tx SF Padding.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532,3432++ */ /* MISRA Rule 8.7,20.7 */
CanTp_TxSFPadding(
    const CanTp_ConnectionType* TxSubchannelPtr,
    const CanTp_TxNSduType* txSduCfgPtr,
    PduInfoType* pduInfoPtr)
/* PRQA S 1532,3432-- */ /* MISRA Rule 8.7,20.7 */
{
    PduLengthType frameDL;
#if (STD_ON == CANTP_FD)
    PduLengthType can20DataFieldLen = 0;
    PduLengthType canFdDataOffset = 0;

    switch (txSduCfgPtr->AddressingFormat)
    {
    case CANTP_STANDARD:
    case CANTP_NORMALFIXED:
        can20DataFieldLen = CANTP_SF_LEN_MAX_STD;
        canFdDataOffset = CANTP_SF_DATA_OFFSET_CANFD_STD;
        break;
#if (                                                                                           \
    (CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON) || (CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON) \
    || (CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON))
    case CANTP_MIXED:
    case CANTP_MIXED29BIT:
    case CANTP_EXTENDED: {
        can20DataFieldLen = CANTP_SF_LEN_MAX_EX;
        canFdDataOffset = CANTP_SF_DATA_OFFSET_CANFD_EX;
        break;
    }
#endif /* CANTP_EXTENDED_ADDRESSING_SUPPORT == STD_ON || CANTP_MIXED_ADDRESSING_SUPPORT == STD_ON \
|| CANTP_MIXED29_ADDRESSING_SUPPORT == STD_ON */
    default:
        /*idle*/
        break;
    }

    if ((TxSubchannelPtr->SduDataTotalCount > can20DataFieldLen) && (CANTP_CAN_FD == txSduCfgPtr->CanFrameType))
    {
        /*CANFD frame length >8*/
        frameDL = TxSubchannelPtr->SduDataTotalCount + canFdDataOffset;
        /*frameDL here is less than or equal to the DLC which configuration in the CANFI module*/
        frameDL = CanTp_MatchDLC(frameDL);
        CanTp_MemorySet(
            &pduInfoPtr->SduDataPtr[pduInfoPtr->SduLength],
            CANTP_PADDING_BYTE,
            (frameDL - pduInfoPtr->SduLength));
        pduInfoPtr->SduLength = frameDL;
    }
    else
#else
    CANTP_UNUSED(TxSubchannelPtr);
#endif /*ENDOF #if (STD_ON == CANTP_FD)*/
    {
        if (CANTP_PADDING_ON == txSduCfgPtr->TxPaddingActivation)
        {
            /*CAN2.0 or small CAN FD*/
            frameDL = CANTP_CAN20_FRAME_LEN_MAX;
            CanTp_MemorySet(
                &pduInfoPtr->SduDataPtr[pduInfoPtr->SduLength],
                CANTP_PADDING_BYTE,
                (uint16)(frameDL - pduInfoPtr->SduLength));
            pduInfoPtr->SduLength = frameDL;
        }
    }
}

#if (CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
/*
 * Brief               Construct Rx meta data.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      N/A
 * Param-Name[out]     N/A
 * Param-Name[in/out]  N/A
 * Return              N/A
 * PreCondition        N/A
 * CallByAPI
 */
FUNC(void, CANTP_CODE)
/* PRQA S 1532++ */ /* MISRA Rule 8.7 */
CanTp_ConstructForwardRxMetaData(
    /* PRQA S 1532++ */ /* MISRA Rule 8.7 */
    const CanTp_ConnectionType* RxchannelPtr,
    const CanTp_RxNSduType* RxNSduCfgPtr,
    const PduInfoType* CanTpRxInfoPtr)
{
#if ((CANTP_MIXED29_ADDRESSING_SUPPORT == STD_OFF) && (CANTP_NORMAL_FIXED_ADDRESSING_SUPPORT == STD_OFF))
    CANTP_UNUSED(RxchannelPtr);
    CANTP_UNUSED(CanTpRxInfoPtr);
    CANTP_UNUSED(RxNSduCfgPtr);
#else
    if ((RxNSduCfgPtr->AddressingFormat == CANTP_NORMALFIXED)
        || (RxNSduCfgPtr->AddressingFormat == CANTP_MIXED29_ADDRESSING_SUPPORT))
    {
        if (CanTpRxInfoPtr->MetaDataPtr != NULL_PTR)
        {
            CanTpRxInfoPtr->MetaDataPtr[CANTP_SA_OFFSET_META_UPPER] = RxchannelPtr->MetaDataN_SA;
            CanTpRxInfoPtr->MetaDataPtr[CANTP_TA_OFFSET_META_UPPER] = RxchannelPtr->MetaDataN_TA;
        }
    }
#endif
}
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */

#define CANTP_STOP_SEC_CODE
#include "CanTp_MemMap.h"
/*=======[E N D   O F   F I L E]==============================================*/
