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

#ifndef CANTP_INTERNAL_H_
#define CANTP_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "Det.h"
#include "CanTp.h"
#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
#include "Os.h"
#endif
#if (CANTP_FEATURE_MEMCOPY_USED_ILIB == STD_ON)
#include "IStdLib.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
#if (STD_ON == CANTP_RUNTIME_ERROR_DETECT)
/**
 * @brief       Report runtime error to the Default Error Tracer
 * @param[in]   ApiId    : The identity of the API
 * @param[in]   ErrorId  : The identity of the Runtime error
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73091
 */
CANTP_LOCAL_INLINE void CANTP_DET_REPORTRUNTIMEERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportRuntimeError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif

#if (STD_ON == CANTP_DEV_ERROR_DETECT)
/**
 * @brief       Report development error to the Default Error Tracer
 * @param[in]   ApiId    : The identity of the API
 * @param[in]   ErrorId  : The identity of the development error
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73092
 */
CANTP_LOCAL_INLINE void CANTP_DET_REPORTERROR(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif

#if (CANTP_SUPPORT_VARIANT == STD_ON)
/**
 * @brief       Get corresponding internal rxsdu index
 * @param[in]   rxNSduId  : The identity of the corresponding RxNSdu for external
 * @return      uint16
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73093
 */
CANTP_LOCAL_INLINE uint16 CanTp_InternalRxNSduIndex(const PduIdType rxNSduId)
{
    uint16 index;
    for (index = 0u; index < CanTp_ConfigPtr->CanTp_RxNSduNum; index++)
    {
        if (rxNSduId == CanTp_ConfigPtr->CanTp_RxNSduMapping[index])
        {
            break;
        }
    }
    return index;
}

/**
 * @brief       Get corresponding internal txsdu index
 * @param[in]   txNSduId  : The identity of the corresponding TxNSdu for external
 * @return      uint16
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73094
 */
CANTP_LOCAL_INLINE uint16 CanTp_InternalTxNSduIndex(const PduIdType txNSduId)
{
    uint16 index;
    for (index = 0u; index < CanTp_ConfigPtr->CanTp_TxNSduNum; index++)
    {
        if (txNSduId == CanTp_ConfigPtr->CanTp_TxNSduMapping[index])
        {
            break;
        }
    }
    return index;
}
/**
 * @brief       Get corresponding internal rxpdu index when receiption
 * @param[in]   rxPduId  : The identity of the corresponding RxPdu for external
 * @return      uint16
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73095
 */
CANTP_LOCAL_INLINE uint16 CanTp_InternalRxPduIndex(const PduIdType rxPduId)
{
    uint16 index;
    for (index = 0u; index < CanTp_ConfigPtr->CanTp_RxNPduNum; index++)
    {
        if (rxPduId == CanTp_ConfigPtr->CanTp_RxNPduMapping[index])
        {
            break;
        }
    }
    return index;
}

/**
 * @brief       Get corresponding internal txpdu index
 * @param[in]   txPduId  : The identity of the corresponding TxPdu for external
 * @return      uint16
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73096
 */
CANTP_LOCAL_INLINE uint16 CanTp_InternalTxPduIndex(const PduIdType txPduId)
{
    uint16 index;
    for (index = 0u; index < CanTp_ConfigPtr->CanTp_TxNPduNum; index++)
    {
        if (txPduId == CanTp_ConfigPtr->CanTp_TxNPduMapping[index])
        {
            break;
        }
    }
    return index;
}
#endif
/**
 * @brief       Get corresponding TxNSdu index according to FC RxPduId
 * @param[in]   rxPduId  : The identity of the corresponding RxPdu
 * @return      uint8
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73233
 */
CANTP_LOCAL_INLINE uint8 CanTp_GetTxNSduIndex_FC(const PduIdType rxPduId)
{
    PduIdType index = rxPduId;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
    index = CanTp_InternalRxPduIndex(rxPduId);
#endif
    return CanTp_ConfigPtr->CanTp_RxPduMap[index].TxNSduFc_index;
}
/**
 * @brief       Get corresponding RxNSdu index according to FC TxPduId
 * @param[in]   txPduId  : The identity of the corresponding TxPdu
 * @return      uint8
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73234
 */
/* PRQA S 4461 ++ */ /* VL_CanTp_4461 */
CANTP_LOCAL_INLINE uint8 CanTp_GetRxNSduIndex_FC(const PduIdType txPduId)
{
    uint8 index = txPduId;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
    index = CanTp_InternalTxPduIndex(txPduId);
#endif
    return CanTp_ConfigPtr->CanTp_TxPduMap[index].RxNSduFc_index;
}
/**
 * @brief       Get corresponding TxNSdu index when transmition
 * @param[in]   txPduId  : The identity of the corresponding TxPdu
 * @return      uint8
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73097
 */
CANTP_LOCAL_INLINE uint8 CanTp_GetTxNSduIndex(const PduIdType txPduId)
{
    uint8 index = txPduId;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
    index = CanTp_InternalTxPduIndex(txPduId);
#endif
    return CanTp_ConfigPtr->CanTp_TxPduMap[index].TxNSdu_index;
}

/**
 * @brief       Get corresponding RxNSdu index when receiption
 * @param[in]   rxPduId  : The identity of the corresponding RxPdu
 * @return      uint8
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73098
 */
CANTP_LOCAL_INLINE uint8 CanTp_GetRxNSduIndex(const PduIdType rxPduId)
{
    uint8 index = rxPduId;
#if (CANTP_SUPPORT_VARIANT == STD_ON)
    index = CanTp_InternalRxPduIndex(rxPduId);
#endif
    return CanTp_ConfigPtr->CanTp_RxPduMap[index].RxNSdu_index;
}
/* PRQA S 4461 -- */

/**
 * @brief       Get the corresponding configuration of the RxNSdu
 * @param[in]   rxNSduId  : the corresponding TxNSdu index
 * @return      CanTp_RxNSduType
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73099
 */
/* PRQA S 5016 ++ */ /* VL_CanTp_5016 */
CANTP_LOCAL_INLINE const CanTp_RxNSduType* CanTp_GetRxNSduCfg(const PduIdType rxNSduId)
{
    const CanTp_RxNSduType* rxNSduCfgPtr = (const CanTp_RxNSduType*)NULL_PTR;
    if (rxNSduId <= CanTp_ConfigPtr->CanTp_RxNSduNum)
    {
        rxNSduCfgPtr = &CanTp_ConfigPtr->CanTp_RxNSduCfg[rxNSduId];
    }
    return rxNSduCfgPtr;
}
/**
 * @brief       Get the corresponding configuration of the TxNSdu
 * @param[in]   txNSduInd  : the corresponding TxNSdu index
 * @return      CanTp_TxNSduType
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73100
 */
CANTP_LOCAL_INLINE const CanTp_TxNSduType* CanTp_GetTxNSduCfg(const PduIdType txNSduInd)
{
    const CanTp_TxNSduType* txNSduCfgPtr = (const CanTp_TxNSduType*)NULL_PTR;
    if (txNSduInd <= CanTp_ConfigPtr->CanTp_TxNSduNum)
    {
        txNSduCfgPtr = &CanTp_ConfigPtr->CanTp_TxNSduCfg[txNSduInd];
    }
    return txNSduCfgPtr;
}

#if (STD_ON == CANTP_RX_QUEUE)
/**
 * @brief       Add new request to Rx Queue.
 * @param[in]   buffer   : Rx Queue buffer
 * @param[in]   rxNSduId : The identity of the corresponding RxNsdu
 * @param[in]   accIndex : Indicates the number of current queues
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73101
 */
CANTP_LOCAL_INLINE void CanTp_AddRxQueue(uint8* buffer, uint8 rxNSduId, uint8* accIndex)
{
    boolean find = FALSE;
    for (uint8 index = 0u; index < *accIndex; index++)
    {
        if (buffer[index] == rxNSduId)
        {
            find = TRUE;
            break;
        }
    }
    if (FALSE == find)
    {
        buffer[*accIndex] = rxNSduId;
        (*accIndex)++;
    }
}

/**
 * @brief       Clear resolved request to Rx Queue.
 * @param[in]   buffer   : Rx Queue buffer
 * @param[in]   rxNSduId : The identity of the corresponding RxNsdu
 * @param[in]   accIndex : Indicates the number of current queues
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73102
 */
CANTP_LOCAL_INLINE void CanTp_ClearRxQueue(uint8* buffer, uint8 rxNSduId, uint8* accIndex)
{
    for (uint8 index = 0u; index < *accIndex; index++)
    {
        if (buffer[index] == rxNSduId)
        {
            (*accIndex)--;
            for (uint8 iloop = index; iloop < *accIndex; iloop++)
            {
                buffer[iloop] = buffer[iloop + 1u];
            }
            buffer[*accIndex] = CANTP_INVALID_U8;

            break;
        }
    }
}
#endif

#if (CANTP_ECUC_MULTIPLE_PARTITION_EXIST == STD_ON)
/**
 * @brief       Get partition index according to current application Id.
 * @param[in]   applicationId   : current application id.
 * @return      globalpartitionInfo : partition information
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73103
 */
CANTP_LOCAL_INLINE CanTp_GPartInfoType* CanTp_GetPartitionInfoPtr(ApplicationType applicationId)
{
    CanTp_GPartInfoType* globalpartitionInfo = (CanTp_GPartInfoType*)NULL_PTR;
    if ((applicationId < CANTP_ECUC_PARTITION_NUMBER) && (CanTp_ApplicationMap[applicationId] != CANTP_INVALID_U8))
    {
        globalpartitionInfo = &CanTp_GlobalPartitionInfo[CanTp_ApplicationMap[applicationId]];
    }
    return globalpartitionInfo;
}
/* PRQA S 5016 -- */
#endif

/**
 * @brief       Verifying the length correctness of received CAN frames
 * @param[in]   canDL  : the length of received CAN frames
 * @return      boolean
 * @retval      TRUE : Check OK
 * @retval      FALSE : Check NOT OK
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73104
 */
CANTP_LOCAL_INLINE boolean CanTp_CheckVaildLen(PduLengthType canDL)
{
    boolean result = FALSE;
    switch (canDL)
    {
    case 8u:
    case 12u:
    case 16u:
    case 20u:
    case 24u:
    case 32u:
    case 48u:
    case 64u:
        result = TRUE;
        break;
    default:
        /**< idle*/
        break;
    }
    return result;
}

#if (CANTP_FEATURE_MEMCOPY_USED_ILIB == STD_OFF)
/**
 * @brief       Default implementation of memcopy
 * @param[in]   src    : pointer to the source data that needs to be copied.
 * @param[in]   dst    : pointer to the destination data where the copied data will be stored..
 * @param[in]   len    : number of bytes to copy from `src` to `dst`.
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73104
 */
CANTP_LOCAL_INLINE void CanTp_MemCpy(uint8* dst, const uint8* src, uint32 len)
{
    for (uint32 Index = 0x00u; Index < len; ++Index)
    {
        dst[Index] = src[Index];
    }
}
/**
 * @brief        Default implementation of memory set function.
 * @param[in]    val    the value that will be used to fill dst.
 * @param[in]    len    number of bytes to set to the specified value.
 * @param[inout] dst    pointer to the destination data where the value will be set.
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73105
 */
CANTP_LOCAL_INLINE void CanTp_MemSet(uint8* dst, uint8 val, uint32 len)
{
    for (uint32 Index = 0x00u; Index < len; ++Index)
    {
        dst[Index] = val;
    }
}
#else
/**
 * @brief Wrapper macro for memcopy
 */
#define CanTp_MemCpy(dst, src, len) IStdLib_MemCpy(dst, src, len) /**< implementation of memcpy.*/
/**
 * @brief Wrapper macro for memset
 */
#define CanTp_MemSet(dst, val, len) IStdLib_MemSet(dst, val, len) /**< implementation of memset.*/
#endif
/* ========================================= external function declarations ========================================= */

/**
 * @brief       Get Frame PCI Type
 * @param[in]   rxpduId    : Identification of the received N-PDU
 * @param[in]   pduInfoPtr : pointer to the received N-PDU data
 * @return      CanTp_FramePCIType
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73106
 */
extern CanTp_FramePCIType CanTp_GetFramePCIType(const PduIdType rxpduId, const PduInfoType* pduInfoPtr);

/**
 * @brief       Get Frame PCI Offset
 * @param[in]   addressingFormat   : Frame AddressingFormat
 * @return      uint8
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73107
 */
extern uint8 CanTp_GetPCIOffset(CanTp_AddressingFormatType addressingFormat);

/**
 * @brief       Assemble Padding data information of the Nsdu
 * @param[in]   txpduInfo       : pointer to the lower L-PDU data
 * @param[in]   txStatus        : pointer to the transmitted N-PDU status
 * @param[in]   assembledLength : the length of the data that has been assembled.
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73108
 */
extern void CanTp_AssemblePaddingData(CanTp_TxStatusType* txStatus, PduInfoType* txpduInfo, uint8 assembledLength);

/**
 * @brief       Assemble PCI information of the Nsdu
 * @param[in]   txNSduCfgPtr  : pointer to the transmitted N-PDU configuration
 * @param[in]   txStatus      : pointer to the transmitted N-PDU status
 * @param[in]   dataOffset    : the offset of the data field
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73109
 */
extern void CanTp_TxConstructPCI(CanTp_TxStatusType* txStatus, uint8* dataOffset);

/**
 * @brief       Match frame length
 * @param[in]   frameDL      : frame length
 * @return      PduLengthType
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73110
 */
extern PduLengthType CanTp_MatchDLC(PduLengthType frameDL);

/**
 * @brief       Check SF Frame datalength
 * @param[in]   pduInfoPtr    : pointer to the received N-PDU data
 * @param[in]   rxNSduCfgPtr  : pointer to the received N-PDU configuration
 * @param[out]  datalength    : the received N-PDU data length
 * @param[out]  dataOffset    : the received N-PDU data offset
 * @param[in]   rxStatus      : pointer to the received N-PDU status
 * @return      boolean
 * @retval      TRUE          : check ok
 * @retval      FALSE         : check failed
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73111
 */
extern boolean CanTp_CheckReceivedSFDL(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    PduLengthType*          datalength,
    uint8*                  dataOffset
#if (CANTP_TRANSMIT_CAN20_WITH_CANFD == STD_ON)
    ,
    CanTp_RxStatusType* rxStatus
#endif
);

/**
 * @brief       Check FF Frame datalength
 * @param[in]   pduInfoPtr    : pointer to the received N-PDU data
 * @param[in]   rxNSduCfgPtr  : pointer to the received N-PDU configuration
 * @param[out]  totalLength   : segmented reception total length
 * @param[out]  datalength    : received datalength
 * @param[out]  dataOffset    : data offset
 * @return      boolean
 * @retval      TRUE          : check ok
 * @retval      FALSE         : check failed
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73112
 */
extern boolean CanTp_CheckReceivedFFDL(
    const PduInfoType*      pduInfoPtr,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    PduLengthType*          totalLength,
    PduLengthType*          datalength,
    uint8*                  dataOffset);

/**
 * @brief       Get Current Bs
 * @param[in]   rxNSduId    : the received N-PDU identity.
 * @param[in]   globalInfo  : pointer to the received N-PDU runtime variable.
 * @return      uint8
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73113
 */
extern uint8 CanTp_GetRxCurrentBs(PduIdType rxNSduId, const CanTp_GPartInfoType* globalInfo);

/**
 * @brief       get Current STmin
 * @param[in]   rxNsduId    : the identity of the corresponding RxNsdu
 * @param[in]   globalInfo  : pointer to the received N-PDU runtime variable.
 * @return      uint8
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73114
 */
extern uint8 CanTp_GetRxCurrentSTmin(
    PduIdType rxNSduId
#if (CANTP_CHANGE_PARAMETER == STD_ON)
    ,
    const CanTp_GPartInfoType* globalInfo
#endif
);
/**
 * @brief       Compare remaining buffer with Bs
 * @param[in]   rxStatus      : pointer to the received N-PDU status
 * @param[in]   bufferSize    : the available Rx buffer size provided by the upper layer
 * @return      Std_ReturnType
 * @retval      E_OK          : the remaining buffer is sufficient for the reception of the next block
 * @retval      E_NOT_OK      : the remaining buffer is not sufficient for the reception of the next block
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73115
 */
extern Std_ReturnType CanTp_CompareRxBufferWithBs(const CanTp_RxStatusType* rxStatus, PduLengthType bufferSize);

#if (CANTP_DYN_ID_SUPPORT == STD_ON)
/**
 * @brief       Save MetaData information when setup tx session.
 * @param[in]   txStatus      : pointer to the transimitted N-PDU status.
 * @param[in]   txNSduCfgPtr  : pointer to the transimitted N-PDU configuration.
 * @param[in]   pduInfoPtr    : pointer to the transimitted N-PDU data.
 * @return      Std_ReturnType
 * @retval      E_OK          : Matadata saved successfully
 * @retval      E_NOT_OK      : Matadata saved failed
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73116
 */
extern void CanTp_SaveTxMetaDataInfo(
    CanTp_TxStatusType*     txStatus,
    const CanTp_TxNSduType* txNSduCfgPtr,
    const PduInfoType*      pduInfoPtr);

/**
 * @brief       Construct the addressing information contained in the MetaData of the N-PDU when transmit.
 * @param[in]   txStatus      : pointer to the transimitted N-PDU status.
 * @param[in]   txNSduCfgPtr  : pointer to the transimitted N-PDU configuration.
 * @param[in]   pduInfoPtr    : pointer to the transimitted N-PDU data.
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73117
 */
extern void CanTp_ConstructTxMetaDataInfo(
    const CanTp_TxStatusType* txStatus,
    const CanTp_TxNSduType*   txNSduCfgPtr,
    PduInfoType*              pduInfoPtr);

/**
 * @brief       Check the addressing information contained in the MetaData of the N-PDU when receiving CF.
 * @param[in]   rxNSduCfgPtr  : pointer to the received N-PDU status.
 * @param[in]   rxStatus      : pointer to the received N-PDU configuration.
 * @param[in]   pduInfoPtr    : pointer to the received N-PDU data.
 * @return      Std_ReturnType
 * @retval      E_OK          : Check OK
 * @retval      E_NOT_OK      : Check NOT OK
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73118
 */
extern Std_ReturnType CanTp_CheckRxCFMetaData(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    const PduInfoType*      pduInfoPtr);

/**
 * @brief       Check the addressing information contained in the MetaData of the N-PDU when receiving FC.
 * @param[in]   txNSduCfgPtr  : pointer to the transimitted N-PDU status.
 * @param[in]   txStatus      : pointer to the transimitted N-PDU configuration.
 * @param[in]   pduInfoPtr    : pointer to the transimitted N-PDU data.
 * @return      Std_ReturnType
 * @retval      E_OK          : Check OK
 * @retval      E_NOT_OK      : Check NOT OK
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73119
 */
extern Std_ReturnType CanTp_CheckRxFCMetaData(
    CanTp_TxStatusType*     txStatus,
    const CanTp_TxNSduType* txNSduCfgPtr,
    const PduInfoType*      pduInfoPtr);

/**
 * @brief       Save MetaData information when receiving FF,SF
 * @param[in]   rxStatus      : pointer to the received N-PDU status
 * @param[in]   rxNSduCfgPtr  : pointer to the received N-PDU configuration
 * @param[in]   pduInfoPtr    : pointer to the received N-PDU data
 * @return      Std_ReturnType
 * @retval      E_OK          : metadata saved successfully
 * @retval      E_NOT_OK      : metadata saved failed
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73120
 */
extern Std_ReturnType CanTp_SaveRxMetaDataInfo(
    CanTp_RxStatusType*     rxStatus,
    const CanTp_RxNSduType* rxNSduCfgPtr,
    const PduInfoType*      pduInfoPtr);
/**
 * @brief       Fill the addressing information contained in the MetaData of the N-PDU when transmitS Fc.
 * @param[in]   rxNSduCfgPtr  : The corresponding configuration of the RxNsdu
 * @param[in]   rxStatus      : The corresponding internal status of the RxNsdu
 * @param[in]   pduInfoPtr    : Length of and pointer to the PDU data and pointer to MetaData.
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-73121
 */
extern void CanTp_ConstructTxFcMetaDataInfo(
    const CanTp_RxStatusType* rxStatus,
    const CanTp_RxNSduType*   rxNSduCfgPtr,
    PduInfoType*              pduInfoPtr);
#endif
/* ===================================== external function declarations ========================================= */
#ifdef __cplusplus
}
#endif
#endif
