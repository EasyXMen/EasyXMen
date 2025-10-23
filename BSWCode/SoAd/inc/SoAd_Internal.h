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
 **  @file               : SoAd_Internal.h
 **  @author             : zhengfei.li
 **  @date               : 2024/07/03
 **  @vendor             : isoft
 **  @description        : Socket Adaptor internal header
 **
 ***********************************************************************************************************************/
#ifndef SOAD_INTERNAL_H_
#define SOAD_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "TcpIp_Types.h"
#include "SoAd_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/**
 * Unused function parameters
 */
#define SOAD_NOUSED(paramter) (void)(paramter)

/**
 * SoAd invalid PduId definition
 */
#define SOAD_PDUID_INVALID COMSTACK_PDUID_INVALID

/**
 * Unused uint8 value
 */
#define SOAD_UNUSED_UINT8 (0xFFu)
/**
 * Unused uint32 value
 */
#define SOAD_UNUSED_UINT32 (0xFFFFFFFFUL)
/**
 * Unused uint16 value
 */
#define SOAD_UNUSED_UINT16 (0xFFFFu)

/**
 * @name SOAD_BIT_POSITION_GROUP
 * @{
 * @brief Defines the bit position
 */
#define SOAD_BIT_24 24u /**< Defines the bit position for 24 bits*/
#define SOAD_BIT_16 16u /**< Defines the bit position for 16 bits*/
#define SOAD_BIT_8  8u  /**< Defines the bit position for 8 bits*/
#define SOAD_BIT_4  4u  /**< Defines the bit position for 4 bits*/
/** @} */

/**
 * @name SOAD_DATA_OFFSET_GROUP
 * @{
 * @brief Defines the data offset
 */
#define SOAD_DATA_OFFSET_0 0UL /**< Defines the data offset for position 0*/
#define SOAD_DATA_OFFSET_1 1UL /**< Defines the data offset for position 1*/
#define SOAD_DATA_OFFSET_2 2UL /**< Defines the data offset for position 2*/
#define SOAD_DATA_OFFSET_3 3UL /**< Defines the data offset for position 3*/
#define SOAD_DATA_OFFSET_4 4UL /**< Defines the data offset for position 4*/
#define SOAD_DATA_OFFSET_5 5UL /**< Defines the data offset for position 5*/
#define SOAD_DATA_OFFSET_6 6UL /**< Defines the data offset for position 6*/
#define SOAD_DATA_OFFSET_7 7UL /**< Defines the data offset for position 7*/
/** @} */

/**
 * @name SOAD_BEST_ALGORITHM_PRIORITY_GROUP
 * @{
 * @brief  Defines the priority for the best match algorithm
 */
#define SOAD_BEST_MATCH_ALG_PRIORITY_4 4U /**< Defines the priority for the best match algorithm as 4*/
#define SOAD_BEST_MATCH_ALG_PRIORITY_3 3U /**< Defines the priority for the best match algorithm as 3*/
#define SOAD_BEST_MATCH_ALG_PRIORITY_2 2U /**< Defines the priority for the best match algorithm as 2*/
#define SOAD_BEST_MATCH_ALG_PRIORITY_1 1U /**< Defines the priority for the best match algorithm as 1*/
/** @} */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if (SOAD_SUPPORT_UPPERLAYMODULE_MAX > 0u)
/**
 * @brief Array of pointers to the initialization status of each partition.
 */
extern const SoAd_PcCfgType SoAd_PcCfg;
#endif

/**
 * @brief Array of pointers to the initialization status of each partition.
 */
extern SoAd_Init_StateType* const SoAd_MultiPartitionInitStatusPtr[SOAD_PARTITION_NUM];

#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
/**
 * @brief Array of partition information structures.
 */
extern const SoAd_PartitionInfoType SoAd_Partition[SOAD_PARTITION_MAX];
#endif

/**
 * @brief Array of multi-partition information structures.
 */
extern const SoAd_MultiPartitionInfoType SoAd_MultiPartitionInfo[SOAD_PARTITION_NUM];

/* ========================================= external function declarations ========================================= */
#if (STD_ON == SOAD_SUPPORT_MULTIPLE_PARTITION)
extern void SoAd_MainFunction(uint16 PartitionId);
#endif
/**
 * @brief         dummy function for IfRxIndication
 * @param[in]     RxPduId: received PduId
 * @param[in]     PduInfoPtr: pdu information pointer
 * @return        None
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-78712
 */
void SoAd_DummyIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief         dummy function for IfTriggerTransmit
 * @param[in]     TxPduId: transmit PduId
 * @param[in]     PduInfoPtr: pdu information pointer
 * @return        Std_ReturnType
 * @retval        E_NOT_OK : The request was not successful.
 * @synchronous   TRUE
 * @trace         CPD-78713
 */
Std_ReturnType SoAd_DummyIfTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);

/**
 * @brief         dummy function for IfTxConfirmation
 * @param[in]     TxPduId: transmit PduId
 * @param[in]     result: result of the transmission
 * @return        None
 * @synchronous   TRUE
 * @trace         CPD-78714
 */
void SoAd_DummyIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);

/**
 * @brief         dummy function for TpStartOfReception
 * @param[in]     RxPduId: received PduId
 * @param[in]     info: pdu information pointer
 * @param[in]     TpSduLength: transmit Tp Pdu length
 * @param[out]    bufferSizePtr: Available receive buffer in the receiving module
 * @return        BufReq_ReturnType
 * @retval        BUFREQ_E_NOT_OK : The request was not successful.
 * @synchronous   TRUE
 * @trace         CPD-78715
 */
BufReq_ReturnType SoAd_DummyTpStartOfReception(
    PduIdType          RxPduId,
    const PduInfoType* info,
    PduLengthType      TpSduLength,
    PduLengthType*     bufferSizePtr);

/**
 * @brief         dummy function for TpCopyRxData
 * @param[in]     id: received PduId
 * @param[in]     info: pdu information pointer
 * @param[out]    bufferSizePtr: Available receive buffer after data has been copied.
 * @return        BufReq_ReturnType
 * @retval        BUFREQ_E_NOT_OK : The request was not successful.
 * @synchronous   TRUE
 * @trace         CPD-78716
 */
BufReq_ReturnType SoAd_DummyTpCopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);

/**
 * @brief         dummy function for TpRxIndication
 * @param[in]     RxPduId: received PduId
 * @param[in]     result: result of the transmission
 * @return        None
 * @synchronous   TRUE
 * @trace         CPD-78717
 */
void SoAd_DummyTpRxIndication(PduIdType RxPduId, Std_ReturnType result);

/**
 * @brief         dummy function for TpCopyTxData
 * @param[in]     id: received PduId
 * @param[in]     info: pdu information pointer
 * @param[in]     retry: Acknowledge transmitted data or to retransmit data after transmission problems.
 * @param[out]    bufferSizePtr: Available in the upper layer module Tx buffer
 * @return        BufReq_ReturnType
 * @retval        BUFREQ_E_NOT_OK : The request was not successful.
 * @synchronous   TRUE
 * @trace         CPD-78718
 */
BufReq_ReturnType SoAd_DummyTpCopyTxData(
    PduIdType            id,
    const PduInfoType*   info,
    const RetryInfoType* retry,
    PduLengthType*       availableDataPtr);

/**
 * @brief         dummy function for TpTxConfirmation
 * @param[in]     TxPduId: transmission PduId
 * @param[in]     result: result of the transmission
 * @return        None
 * @synchronous   TRUE
 * @trace         CPD-78719
 */
void SoAd_DummyTpTxConfirmation(PduIdType TxPduId, Std_ReturnType result);

/**
 * @brief         dummy function for SoConModeChg
 * @param[in]     SoConId: socket connection id
 * @param[in]     Mode: Mode of the socket connection
 * @return        None
 * @return        None
 * @synchronous   TRUE
 * @trace         CPD-78720
 */
void SoAd_DummySoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode);

/**
 * @brief         dummy function for LocalIpAddrAssignmentChg
 * @param[in]     SoConId: socket connection id
 * @param[in]     State: State of the ip address
 * @return        None
 * @return        None
 * @synchronous   TRUE
 * @trace         CPD-78721
 */
void SoAd_DummyLocalIpAddrAssignmentChg(SoAd_SoConIdType SoConId, TcpIp_IpAddrStateType State);

#ifdef __cplusplus
}
#endif

#endif
