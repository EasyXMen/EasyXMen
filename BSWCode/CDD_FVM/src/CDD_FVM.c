/**
 * Copyright (C) 2008-2026 isoft Infrastructure Software Co., Ltd.
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
 **  @file               : CDD_FVM.c
 **  @author             : Jian.Jiang
 **  @date               : 2023/10/20
 **  @vendor             : isoft
 **  @description        : Implementation for FVM
 **
 ***********************************************************************************************************************/
/* PRQA S 6540, 6520 EOF */ /* VL_MTR_CDD_FVM_STTPP, VL_MTR_CDD_FVM_STVAR */
/* ===================================================== macros ===================================================== */
#define CDD_FVM_C_AR_MAJOR_VERSION 4u
#define CDD_FVM_C_AR_MINOR_VERSION 9u
#define CDD_FVM_C_AR_PATCH_VERSION 0u
#define CDD_FVM_C_SW_MAJOR_VERSION 1u
#define CDD_FVM_C_SW_MINOR_VERSION 1u
#define CDD_FVM_C_SW_PATCH_VERSION 1u

#define CDD_FVM_UNUSED(v) (void)(v)
/* =================================================== inclusions =================================================== */
#include "CDD_FVM.h"
#include "CDD_FVM_Internal.h"

/* ------------------------------------------------- Version  Check ------------------------------------------------- */
#if (CDD_FVM_C_AR_MAJOR_VERSION != CDD_FVM_AR_MAJOR_VERSION)
#error "CDD_FVM.c : Mismatch in Specification Major Version"
#endif
#if (CDD_FVM_C_AR_MINOR_VERSION != CDD_FVM_AR_MINOR_VERSION)
#error "CDD_FVM.c : Mismatch in Specification Minor Version"
#endif
#if (CDD_FVM_C_AR_PATCH_VERSION != CDD_FVM_AR_PATCH_VERSION)
#error "CDD_FVM.c : Mismatch in Specification Patch Version"
#endif
#if (CDD_FVM_C_SW_MAJOR_VERSION != CDD_FVM_SW_MAJOR_VERSION)
#error "CDD_FVM.c : Mismatch in Specification Major Version"
#endif
#if (CDD_FVM_C_SW_MINOR_VERSION != CDD_FVM_SW_MINOR_VERSION)
#error "CDD_FVM.c : Mismatch in Specification MinorVersion"
#endif
#if (CDD_FVM_C_SW_PATCH_VERSION != CDD_FVM_SW_PATCH_VERSION)
#error "CDD_FVM.c : Mismatch in Specification Patch Version"
#endif

/* ========================================== internal function declarations ======================================== */
#define CDD_FVM_START_SEC_CODE
#include "CDD_FVM_MemMap.h"

#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
/**
 * @brief       Initializes sending a single counter.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70875
 */
CDD_FVM_LOCAL void CDD_FVM_TxSingleCounterInit(void);

/**
 * @brief       Gets the freshness value corresponding to the specified freshness index.
 * @param[in]   freshnessIndex : Freshness index.
 * @param[in]   FreshnessLength : Freshness value length.
 * @param[inout] freshnessValue : Buffer used to store freshness values.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70876
 */
CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_GetTxSingleCounterFreshnessValue(uint16 freshnessIndex, uint8* freshnessValue, uint8 FreshnessLength);

/**
 * @brief       Updates the send counter.
 * @param[in]   freshnessIndex : Freshness index.
 * @param[out]  None
 * @param[inout] None
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70877
 */
CDD_FVM_LOCAL Std_ReturnType CDD_FVM_TxUpdateSingleCounterFreshnessValue(uint16 freshnessIndex);
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
/**
 * @brief       Initializes receiving a single counter.
 * @param[in]   RxFreshnessSingleConfig : Receive counter configuration.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70878
 */
CDD_FVM_LOCAL void
    CDD_FVM_RxSingleCounterInit(const CDD_FVM_RxSingleFreshnessCounterConfigType* RxFreshnessSingleConfig);

/**
 * @brief       Updates the receive counter.
 * @param[in]   freshnessIndex : Freshness index.
 * @param[in]   freshnessValue : Freshness value.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70879
 */
CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_RxUpdateSingleCounterFreshnessValue(uint16 freshnessIndex, const uint8* freshnessValue);

/**
 * @brief       Gets the freshness value corresponding to the specified freshness index.
 * @param[in]   freshnessIndex : Freshness index.
 * @param[out]  None
 * @param[inout] freshnessValue : Buffer used to store freshness values.
 * @return      None
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70880
 */
CDD_FVM_LOCAL Std_ReturnType CDD_FVM_GetRxSingleCounterFreshnessValue(uint16 freshnessIndex, uint8* freshnessValue);

/**
 * @brief       Obtains the current freshness value.
 * @param[in]   freshnessIndex : Holds the identifier of the freshness value.
 * @param[in]   FVMTruncatedFreshnessValue : Holds the truncated freshness value that was contained in the Secured
 * I-PDU.
 * @param[in]   FVMTruncatedFreshnessValueLength : Holds the length in bits of the truncated freshness value.
 * @param[in]   FVMAuthVerifyAttempts : Holds the number of authentication verify attempts of this PDU since the last
 * reception.
 * @param[out]  FVMFreshnessValue : Holds the freshness value to be used for the calculation of the authenticator.
 * @param[inout] FVMFreshnessValueLength : Holds the length in bits of the freshness value.
 * @return      Std_ReturnType
 * @retval      E_OK : Request successful.
 * @retval      E_NOT_OK : Request failed, a freshness value cannot be provided due to general issues for freshness or
 * this FreshnessValueId.
 * @retval      E_BUSY : The freshness information can temporarily not be provided.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70881
 */
CDD_FVM_LOCAL Std_ReturnType CDD_FVM_RxSingleCounterFreshnessHandle(
    uint16       freshnessIndex,
    const uint8* FVMTruncatedFreshnessValue,
    uint32       FVMTruncatedFreshnessValueLength,
    uint16       FVMAuthVerifyAttempts,
    uint8*       FVMFreshnessValue,
    uint32*      FVMFreshnessValueLength);

/**
 * @brief       Judges the upward growth of two buffers based on the specified parameters.
 * @param[in]   buff1 : Pointer to the first buffer.
 * @param[in]   buff2 : Pointer to the second buffer.
 * @param[in]   nbit : Number of bits to compare.
 * @param[in]   DeltaCounter : Counter value for comparison.
 * @return      Std_ReturnType
 * @retval      E_OK : Upward growth judgment successful.
 * @retval      E_NOT_OK : Upward growth judgment failed.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70882
 */
CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_JudgmentUpwardGrowth(const uint8* buff1, const uint8* buff2, uint8 nbit, uint16 DeltaCounter);

#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON)
/**
 * @brief       Copies the synchronous message from the master buffer to the slave buffer.
 * @param[in]   masterIndex : Index of the master buffer.
 * @param[in]   slaveIndex : Index of the slave buffer.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70883
 */
CDD_FVM_LOCAL void CDD_FVM_SlaveCopyMasterSyncMsg(uint16 masterIndex, uint16 slaveIndex);

#endif /* CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON */

#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
/**
 * @brief       Initializes the master ECU with the provided configuration.
 * @param[in]   config : Pointer to the configuration data for the master ECU.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70884
 */
CDD_FVM_LOCAL void CDD_FVM_MasterEcuInit(const CDD_FVM_ConfigType* config);

#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
/**
 * @brief       Finds the idle data of the master control slave.
 * @param[in]   index : Index of the master control slave.
 * @param[in]   syncMsgId : Sync message ID.
 * @return      uint8 : Idle data value.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70885
 */
CDD_FVM_LOCAL uint8 CDD_FVM_FindMasterCtrlSlaveIdleData(uint16 index, uint16 syncMsgId);

/**
 * @brief       Finds the current data of the master control.
 * @param[in]   index : Index of the master control.
 * @param[in]   syncMsgId : Sync message ID.
 * @return      uint8 : Current data value.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70886
 */
CDD_FVM_LOCAL uint8 CDD_FVM_FindMasterCtrlCurrentData(uint16 index, uint16 syncMsgId);

/**
 * @brief       Finds the current data of the master control.
 * @param[in]   index : Index of the master control.
 * @param[in]   PduInfoPtr : Pointer to the PDU information.
 * @return      uint8 : Current data value.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70887
 */
CDD_FVM_LOCAL void CDD_FVM_MasterRxCtrlPduFrameHandle(uint16 index, const PduInfoType* PduInfoPtr);
#endif /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
/**
 * @brief       Initializes the slave ECU with the provided configuration.
 * @param[in]   config : Pointer to the configuration data for the slave ECU.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70888
 */
CDD_FVM_LOCAL void CDD_FVM_SlaveEcuInit(const CDD_FVM_ConfigType* config);

/**
 * @brief       Handles the reception of a sync message PDU frame by the slave.
 * @param[in]   index : Index of the sync message PDU frame.
 * @param[in]   PduInfoPtr : Pointer to the PDU information.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70889
 */
CDD_FVM_LOCAL void CDD_FVM_SlaveRxSyncMessagePduFrameHandle(uint16 index, const PduInfoType* PduInfoPtr);

/**
 * @brief       Handles the maximum value of the trip counter by the slave.
 * @param[in]   index : Index of the trip counter.
 * @param[in]   rxtripCntRef : Pointer to the received trip counter reference value.
 * @param[in]   localtripCntRef : Pointer to the local trip counter reference value.
 * @return      Std_ReturnType
 * @retval      E_OK : Function executed successfully.
 * @retval      E_NOT_OK : Function execution failed.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70890
 */
CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_SlaveTripCounterMaxValueHandle(uint16 index, const uint8* rxtripCntRef, const uint8* localtripCntRef);
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)

/**
 * @brief       Handles the reception of a control PDU frame by the slave.
 * @param[in]   index : Index of the control PDU frame.
 * @param[in]   PduInfoPtr : Pointer to the PDU information.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70891
 */
CDD_FVM_LOCAL void CDD_FVM_SlaveRxCtrlPduFrameHandle(uint16 index, const PduInfoType* PduInfoPtr);

/**
 * @brief       Sends an ask message to the master by the slave.
 * @param[in]   index : Index of the ask message.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70892
 */
CDD_FVM_LOCAL void CDD_FVM_SLaveSendAskMessageToMaster(uint16 index);
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
/**
 * @brief       Initializes the transmission of multiple truncated counters based on the provided configuration and
 * slave sync message index.
 * @param[in]   config : Pointer to the configuration for initializing the transmission of multiple truncated counters.
 * @param[in]   slaveSyncMsgIndex : Index representing the slave sync message.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70893
 */
CDD_FVM_LOCAL void CDD_FVM_TxMultipleTruncatedCounterInit(const CDD_FVM_ConfigType* config, uint16 slaveSyncMsgIndex);

/**
 * @brief       Gets the freshness value of a specific index from the multiple truncated counters.
 * @param[in]   freshnessIndex : Index of the freshness value to retrieve.
 * @param[in]   FreshnessLength : Length of the freshness value.
 * @param[out]  None
 * @param[inout] freshnessValue : Retrieved freshness value.
 * @param[inout] FVMTruncatedFreshnessValue : Pointer to the truncated freshness value.
 * @param[inout] FVMTruncatedFreshnessValueLength : Length of the truncated freshness value.
 * @return      Std_ReturnType
 * @retval      E_OK : Freshness value retrieved successfully.
 * @retval      E_NOT_OK : Failed to retrieve freshness value.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70894
 */
CDD_FVM_LOCAL Std_ReturnType CDD_FVM_GetTxMultipleTruncatedCounterFreshnessValue(
    uint16  freshnessIndex,
    uint8*  freshnessValue,
    uint8   FreshnessLength,
    uint8*  FVMTruncatedFreshnessValue,
    uint32* FVMTruncatedFreshnessValueLength);

/**
 * @brief       Adds a signed value to a multiple freshness buffer.
 * @param[in]   buffer     : Pointer to the buffer where the value will be added.
 * @param[in]   value      : The signed value to be added to the buffer.
 * @param[in]   bitLength  : The bit length of the buffer.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70895
 */
CDD_FVM_LOCAL void CDD_FVM_MultipleFreshnessBufferAddN(uint8* buffer, sint8 value, uint8 bitLength);

#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
/**
 * @brief       Initializes the reception of multiple truncated counters based on the provided configuration.
 * @param[in]   config : Pointer to the configuration for initializing the reception of multiple truncated counters.
 * @param[in]   slaveSyncMsgIndex : Index representing the slave sync message.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70896
 */
CDD_FVM_LOCAL void CDD_FVM_RxMultipleTruncatedCounterInit(const CDD_FVM_ConfigType* config, uint16 slaveSyncMsgIndex);

/**
 * @brief       Handles the reception of multiple truncated counters freshness values.
 * @param[in]   freshnessIndex : The index of the freshness value to be updated.
 * @param[in]   FVMTruncatedFreshnessValue : Pointer to the buffer containing the received freshness value.
 * @param[in]   FVMTruncatedFreshnessValueLength : The length of the received freshness value in bytes.
 * @param[in]   FVMAuthVerifyAttempts : The number of attempts to authenticate the received freshness value.
 * @param[out]  None
 * @param[inout] FVMFreshnessValueLength : Pointer to the variable where the length of the updated freshness value will
 * be stored.
 * @param[inout] FVMFreshnessValue : Pointer to the buffer where the updated freshness value will be stored.
 * @return      Std_ReturnType
 * @retval      E_OK : Function executed successfully.
 * @retval      E_NOT_OK : Function execution failed.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70897
 */
CDD_FVM_LOCAL Std_ReturnType CDD_FVM_RxMultipleCounterTruncatedHandle(
    uint16       freshnessIndex,
    const uint8* FVMTruncatedFreshnessValue,
    uint32       FVMTruncatedFreshnessValueLength,
    uint16       FVMAuthVerifyAttempts,

    uint8*  FVMFreshnessValue,
    uint32* FVMFreshnessValueLength

);

/**
 * @brief       Resets the flag handles for a specific slave ECU.
 * @param[in]   SlaveEcuHandle : Pointer to the slave ECU handle.
 * @param[in]  multipleFeshnessConfig : Pointer to the multiple freshness configuration.
 * @param[in]   truncatedFreshnessValue : Pointer to the truncated freshness value.
 * @param[inout] resetFlagSubtraction : Pointer to the subtraction value for the reset flag.
 * @return      Std_ReturnType
 * @retval      E_OK : Flag handles reset successfully.
 * @retval      E_NOT_OK : Failed to reset flag handles.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70898
 */
CDD_FVM_LOCAL Std_ReturnType CDD_FVM_RxResetFlagHandles(
    const CDD_FVM_SlaveECUSyncMsgType*                 SlaveEcuHandle,
    const CDD_FVM_MultipleFreshnessValueStructureType* multipleFeshnessConfig,
    sint8*                                             resetFlagSubtraction,
    const uint8*                                       truncatedFreshnessValue);

/**
 * @brief       Handles the lower message counter for a specific freshness index.
 * @param[in]   freshnessIndex : Index of the freshness value.
 * @param[in]   FVMTruncatedFreshnessValue : Pointer to the truncated freshness value.
 * @param[out]  None
 * @param[inout] latestLowerMsgCntValue : Pointer to the latest lower message counter value.
 * @param[inout] previouslyLowerMsgCntValue : Pointer to the previously lower message counter value.
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70899
 */
CDD_FVM_LOCAL void CDD_FVM_RxMsgLowerCounterHandle(
    uint16       freshnessIndex,
    const uint8* FVMTruncatedFreshnessValue,
    uint64*      latestLowerMsgCntValue,
    uint64*      previouslyLowerMsgCntValue);

/**
 * @brief       Handles the reception of synchronous messages and updates the latest and previously received values.
 * @param[in]   freshnessIndex : Index representing the freshness of the message.
 * @param[in]   SyncMsgLatestBuffer : Pointer to the buffer containing the latest synchronous message.
 * @param[in]   latestSyncMsgValue : Pointer to the variable containing the latest synchronous message value.
 * @param[in]   previouslySyncMsgValue : Pointer to the variable containing the previously received synchronous message
 * value.
 * @param[out]  None
 * @param[inout] None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70900
 */
CDD_FVM_LOCAL void CDD_FVM_RxSyncMessageHandle(
    uint16  freshnessIndex,
    uint8*  SyncMsgLatestBuffer,
    uint64* latestSyncMsgValue,
    uint64* previouslySyncMsgValue);

/**
 * @brief       Checks if the received multiple counter is SecOC pass for a specific freshness index.
 * @param[in]   freshnessIndex : Freshness index for which the check is performed.
 * @param[out]  None
 * @param[inout] None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70901
 */
CDD_FVM_LOCAL void CDD_FVM_RxMultipleCounterIsSecOCPass(uint16 freshnessIndex);
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

/**
 * @brief       Copies the information value to the acquisition source.
 * @param[in]   src : Data source.
 * @param[in]   dataNumOfBit : Number of bits to copy.
 * @param[out]  dest : The data location to be copied to.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70902
 */
CDD_FVM_LOCAL void CDD_FVM_Memcpy(uint8* dest, const uint8* src, uint8 dataNumOfBit);

/**
 * @brief       Compares two buffers bit by bit up to a specified length and counts the number of differences within a
 * maximum delta counter.
 * @param[in]   ptr1 : Pointer to the first buffer.
 * @param[in]   ptr2 : Pointer to the second buffer.
 * @param[in]   bitLength : The length of the buffers in bits to be compared.
 * @param[in]   maxDeltaCounter : The maximum number of differences allowed.
 * @return      Std_ReturnType
 * @retval      CDD_FVM_E_EQUAL : ptr1 == ptr2
 * @retval      CDD_FVM_E_MORE_THAN : ptr1 > ptr2
 * @retval      CDD_FVM_E_LESS_THAN : ptr1 < ptr2
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70903
 */
CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_BitCompare(const uint8* ptr1, const uint8* ptr2, uint8 bitLength, uint16 maxDeltaCounter);

/**
 * @brief       Gets the freshness configuration index of the send counter.
 * @param[in]   FVMFreshnessValueID : Counter ID.
 * @param[out]  mode : Pointer to storage counter mode.
 * @param[inout] None
 * @return      uint16 : Return index value.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70904
 */
CDD_FVM_LOCAL uint16 CDD_FVM_GetTxFreshnessConfigIndex(uint16 FVMFreshnessValueID, CDD_FVM_ModeType* mode);

/**
 * @brief       Gets the freshness configuration index of the send counter.
 * @param[in]   FVMFreshnessValueID : Counter ID.
 * @param[out]  mode : Pointer to storage counter mode.
 * @param[inout] None
 * @return      uint16 : Return index value.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-70905
 */
CDD_FVM_LOCAL uint16 CDD_FVM_GetRxFreshnessConfigIndex(uint16 FVMFreshnessValueID, CDD_FVM_ModeType* mode);

/**
 * @brief       Calculates the coverage of the low N bits from the source buffer and stores the result in the
 * destination buffer.
 * @param[in]   dest : Pointer to the destination buffer.
 * @param[in]   destBitLength : The length of the destination buffer in bits.
 * @param[in]   srcBitLength : The length of the source buffer in bits.
 * @param[inout] src : Pointer to the source buffer.
 * @param[out]  None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70906
 */
CDD_FVM_LOCAL void CDD_FVM_CoverageOfLowNBits(uint8* dest, const uint8* src, uint8 srcBitLength, uint8 destBitLength);

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u) \
    || (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
/**
 * @brief       Copies specific bits from the source to the destination buffer.
 * @param[in]   src : Pointer to the source buffer.
 * @param[in]   dst : Pointer to the destination buffer.
 * @param[in]   firstNBit : Position of the first bit to copy.
 * @param[in]   bitLength : Number of bits to copy.
 * @param[out]  None
 * @param[inout] None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70907
 */
CDD_FVM_LOCAL void CDD_FVM_CopySrcFromMbitToDstBits(const uint8* src, uint8* dst, uint8 firstNBit, uint8 bitLength);
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u OR \
          CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

/**
 * @brief       Extracts the low bits from the source buffer and stores them in the destination buffer.
 * @param[in]   src : Pointer to the source buffer.
 * @param[in]   allBitLength : The total length of the source buffer in bits.
 * @param[in]   lowbitLength : The length of the low bits to be extracted and stored in the destination buffer.
 * @param[inout] dst : Pointer to the destination buffer.
 * @param[out]  None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70908
 */
CDD_FVM_LOCAL void CDD_FVM_GetLowBitFromSrc(const uint8* src, uint8* dst, uint8 allBitLength, uint8 lowbitLength);

/**
 * @brief       Increases the message counter in big-endian format.
 * @param[in]   freshnessBit : The bit position of the message counter in the buffer.
 * @param[inout] freshnessCountBuff : Pointer to the buffer containing the message counter.
 * @param[out]  None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70909
 */
CDD_FVM_LOCAL void CDD_FVM_BigEndianIncreaseMessageCounter(uint8* freshnessCountBuff, uint8 freshnessBit);

/**
 * @brief       Copies data from source to destination with a specified length in bits.
 * @param[in]   src : Pointer to the source buffer.
 * @param[in]   firstNBit : The starting bit position in the destination buffer.
 * @param[in]   bitLength : The length of the data to be copied in bits.
 * @param[inout] dst : Pointer to the destination buffer.
 * @param[out]  None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70910
 */
CDD_FVM_LOCAL void CDD_FVM_CopyBufferToDstBits(uint8* dst, const uint8* src, uint8 firstNBit, uint8 bitLength);

/**
 * @brief       Converts a buffer to a value based on the specified parameters.
 * @param[in]   src : Pointer to the source buffer.
 * @param[in]   firstBit : Position of the first bit to convert.
 * @param[in]   bitlength : Number of bits to convert.
 * @param[out]  None
 * @param[inout] None
 * @return      uint64 : Converted value from the buffer.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70911
 */
CDD_FVM_LOCAL uint64 CDD_FVM_ConvertBufferToValueWithOffset(const uint8* src, uint8 firstBit, uint8 bitlength);

/**
 * @brief       Gets a buffer with a specified length in bits from the source buffer.
 * @param[in]   src : Pointer to the source buffer.
 * @param[in]   srcIndex : The starting bit position in the source buffer.
 * @param[in]   bitlength : The length of the buffer to be copied in bits.
 * @param[inout] dst : Pointer to the destination buffer.
 * @param[out]  None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70912
 */
CDD_FVM_LOCAL void
    CDD_FVM_CopyBitsFromSourceToDestination(const uint8* src, uint8* dst, uint8 srcIndex, uint8 bitlength);

/**
 * @brief       Sets the specified number of bits in the destination buffer to the specified value.
 * @param[in]   dst : Pointer to the destination buffer.
 * @param[in]   Value : The value to be set (0 or 1).
 * @param[in]   bitLength : The number of bits to be set.
 * @param[out]  None
 * @param[inout] None
 * @return      None
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-70913
 */
CDD_FVM_LOCAL void CDD_FVM_SetMemoryBytesToValue(uint8* dst, uint8 Value, uint8 bitLength);

#define CDD_FVM_STOP_SEC_CODE
#include "CDD_FVM_MemMap.h"
/* ============================================ internal data definitions =========================================== */
#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u) || (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
#define CDD_FVM_START_SEC_VAR_INIT_UNSPECIFIED
#include "CDD_FVM_MemMap.h"
/** @brief Variable store FVM initialization status. @range 0x00 - 0x01  */
CDD_FVM_LOCAL CDD_FVM_StateType CDD_FVM_InitStatus = CDD_FVM_UNINIT;
#define CDD_FVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CDD_FVM_MemMap.h"
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u || STD_ON == CDD_FVM_DEV_ERROR_DETECT */

#define CDD_FVM_START_SEC_VAR_INIT_PTR
#include "CDD_FVM_MemMap.h"
/** @brief  Pointer to configuration data. @range NA */
CDD_FVM_LOCAL const CDD_FVM_ConfigType* CDD_FVM_ConfigPtr = NULL_PTR;
#define CDD_FVM_STOP_SEC_VAR_INIT_PTR
#include "CDD_FVM_MemMap.h"

#define CDD_FVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CDD_FVM_MemMap.h"

#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
/** @brief  Array to store the freshness counter information for transmission. @range NA */
CDD_FVM_LOCAL CDD_FVM_TxFreshnessCntType FVM_TxSingleCntHandle[CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM];
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
/** @brief  Array to store the freshness counter information for reception. @range NA */
CDD_FVM_LOCAL CDD_FVM_RxFreshnessCntType FVM_RxSingleCntHandle[CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM];
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
/** @brief  Array to store the freshness counter information for transmission. @range NA */
CDD_FVM_LOCAL CDD_FVM_MultipleFreshnessCntType
    FVM_TxMultipleTruncatedCntHandle[CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM];
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
/** @brief  Array to store the freshness counter information for reception. @range NA */
CDD_FVM_LOCAL CDD_FVM_MultipleFreshnessCntType
    FVM_RxMultipleTruncatedCntHandle[CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM];
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
/** @brief  Array to store the freshness counter information for master ECU. @range NA */
CDD_FVM_LOCAL CDD_FVM_MasterECUSyncMsgType FVM_MasterEcuHandle[CDD_FVM_MASTER_ECU_CONFIG_NUM];
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
/** @brief  Array to store the freshness counter information for slave ECU. @range NA */
CDD_FVM_LOCAL CDD_FVM_SlaveECUSyncMsgType FVM_SlaveEcuHandle[CDD_FVM_SLAVE_CONFIG_NUM];
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0U */

#define CDD_FVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CDD_FVM_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define CDD_FVM_START_SEC_CODE
#include "CDD_FVM_MemMap.h"

/**
 * This interface is used to set the initial state of the FVMInitialize the runtime parameters
 * of the FVM module.Initialize trip counter, reset counter, message counter,and single counter
 * according to the specification.clearing all buffers.
 */
/* PRQA S 1503 ++ */ /* VL_CDD_FVM_1503 */
void CDD_FVM_Init(const CDD_FVM_ConfigType* config)
/* PRQA S 1503 -- */
{
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (NULL_PTR == config)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_INIT_ID, CDD_FVM_E_PARAM_POINTER);
    }
    else if (CDD_FVM_UNINIT != CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_INIT_ID, CDD_FVM_E_INIT_FAILED);
    }
    else
#endif
    {
        CDD_FVM_ConfigPtr = config;
#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
        CDD_FVM_MasterEcuInit(CDD_FVM_ConfigPtr);
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
        CDD_FVM_SlaveEcuInit(CDD_FVM_ConfigPtr);
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */

#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        CDD_FVM_TxSingleCounterInit();
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        CDD_FVM_RxSingleCounterInit(CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig);
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
        CDD_FVM_TxMultipleTruncatedCounterInit(CDD_FVM_ConfigPtr, 0xFFFFu);
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
        CDD_FVM_RxMultipleTruncatedCounterInit(CDD_FVM_ConfigPtr, 0xFFFFu);
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u) || (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
        CDD_FVM_InitStatus = CDD_FVM_INIT;
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u || STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    }
}

/**
 * This interface is used to stop the FVM module from running.
 */
/* PRQA S 1503 ++ */ /* VL_CDD_FVM_1503 */
void CDD_FVM_DeInit(void)
/* PRQA S 1503 -- */
{
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_UNINIT == CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_DEINIT_ID, CDD_FVM_E_UNINIT);
    }
    else
#endif
    {
#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        for (uint16 i = 0u; i < CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM; i++)
        {
            CDD_FVM_SetMemoryBytesToValue(
                FVM_TxSingleCntHandle[i].FVMmsgCnt,
                0u,
                CDD_FVM_ConfigPtr->FvmTxSingleFreshnessConfig[i].FvmFreshnessLength);
        }

#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        for (uint16 i = 0u; i < CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM; i++)
        {
            CDD_FVM_SetMemoryBytesToValue(
                FVM_RxSingleCntHandle[i].FVMmsgCnt,
                0u,
                CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_SetMemoryBytesToValue(
                FVM_RxSingleCntHandle[i].FVMmsgRxFreshnessValue,
                0u,
                CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[i].FvmFreshnessLength);
        }
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */
#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u) || (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
        CDD_FVM_InitStatus = CDD_FVM_UNINIT;
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u || STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    }
}

#if (CDD_FVM_VERSIONINFO_API == STD_ON)
/**
 * This interface is used to obtain the version information of the FVM module.
 */
/* PRQA S 1503 ++ */ /* VL_CDD_FVM_1503 */
void CDD_FVM_GetVersionInfo(Std_VersionInfoType* versioninfo)
/* PRQA S 1503 -- */
{
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (versioninfo == NULL_PTR)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETVERSIONINFO, CDD_FVM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
        versioninfo->vendorID         = CDD_FVM_VENDOR_ID;
        versioninfo->moduleID         = CDD_FVM_MODULE_ID;
        versioninfo->sw_major_version = CDD_FVM_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CDD_FVM_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CDD_FVM_C_SW_PATCH_VERSION;
    }
}
#endif /* CDD_FVM_VERSIONINFO_API == STD_ON */

/**
 * This interface is used to get the freshness value from the FVM according to the configured
 * freshness counter mode.
 */
/* PRQA S 1503, 1532, 6040 ++ */ /* VL_CDD_FVM_1503, VL_QAC_OneFunRef, VL_MTR_CDD_FVM_STPAR */
Std_ReturnType CDD_FVM_GetRxFreshness(
    uint16       FVMFreshnessValueID,
    const uint8* FVMTruncatedFreshnessValue,
    uint32       FVMTruncatedFreshnessValueLength,
    uint16       FVMAuthVerifyAttempts,
    uint8*       FVMFreshnessValue,
    uint32*      FVMFreshnessValueLength)
/* PRQA S 1503, 1532, 6040 -- */
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16           index;
    Std_ReturnType   result = E_NOT_OK;
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_UNINIT == CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETRXFRESHNESS, CDD_FVM_E_UNINIT);
    }
    else if (
        (NULL_PTR == FVMFreshnessValue) || (NULL_PTR == FVMFreshnessValueLength)
        || (NULL_PTR == FVMTruncatedFreshnessValue))
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETRXFRESHNESS, CDD_FVM_E_PARAM_POINTER);
    }
    else
#endif
    {
        index = CDD_FVM_GetRxFreshnessConfigIndex(FVMFreshnessValueID, &mode);
        switch (mode)
        {
#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_SINGLE_COUNTER:
            result = CDD_FVM_RxSingleCounterFreshnessHandle(
                index,
                FVMTruncatedFreshnessValue,
                FVMTruncatedFreshnessValueLength,
                FVMAuthVerifyAttempts,
                FVMFreshnessValue,
                FVMFreshnessValueLength);
            break;
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_TRUNCATED_MULTIPLE_COUNTER:
            result = CDD_FVM_RxMultipleCounterTruncatedHandle(
                index,
                FVMTruncatedFreshnessValue,
                FVMTruncatedFreshnessValueLength,
                FVMAuthVerifyAttempts,
                FVMFreshnessValue,
                FVMFreshnessValueLength);
            break;
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
        default:
            result = E_OK;
            (void)(mode);
            break;
        }
    }
    return result;
}

/**
 * This interface is used to get the freshness value from the FVM according to the configured
 * freshness counter mode.
 */
/* PRQA S 1503, 1532, 6040 ++ */ /* VL_CDD_FVM_1503, VL_QAC_OneFunRef, VL_MTR_CDD_FVM_STPAR */
Std_ReturnType CDD_FVM_GetRxFreshnessAuthData(
    uint16       FVMFreshnessValueID,
    const uint8* FVMTruncatedFreshnessValue,
    uint32       FVMTruncatedFreshnessValueLength,
    const uint8* FVMAuthDataFreshnessValue,
    uint16       FVMAuthDataFreshnessValueLength,
    uint16       FVMAuthVerifyAttempts,
    uint8*       FVMFreshnessValue,
    uint32*      FVMFreshnessValueLength)
/* PRQA S 1503, 1532, 6040 -- */
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16           index;
    Std_ReturnType   result = E_NOT_OK;
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_UNINIT == CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETRXFRESHNESSAUTHDATA, CDD_FVM_E_UNINIT);
    }
    else if (
        (NULL_PTR == FVMFreshnessValue) || (NULL_PTR == FVMFreshnessValueLength)
        || (NULL_PTR == FVMAuthDataFreshnessValue) || (NULL_PTR == FVMTruncatedFreshnessValue))
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETRXFRESHNESSAUTHDATA, CDD_FVM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
        index = CDD_FVM_GetRxFreshnessConfigIndex(FVMFreshnessValueID, &mode);
        switch (mode)
        {
#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_SINGLE_COUNTER:
            result = CDD_FVM_RxSingleCounterFreshnessHandle(
                index,
                FVMTruncatedFreshnessValue,
                FVMTruncatedFreshnessValueLength,
                FVMAuthVerifyAttempts,
                FVMFreshnessValue,
                FVMFreshnessValueLength);
            break;
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_TRUNCATED_MULTIPLE_COUNTER:
            result = CDD_FVM_RxMultipleCounterTruncatedHandle(
                index,
                FVMTruncatedFreshnessValue,
                FVMTruncatedFreshnessValueLength,
                FVMAuthVerifyAttempts,
                FVMFreshnessValue,
                FVMFreshnessValueLength);
            break;
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
        default:
            result = E_OK;
            (void)(mode);
            (void)(FVMAuthDataFreshnessValueLength);
            break;
        }
    }
    CDD_FVM_UNUSED(FVMAuthDataFreshnessValue);
    return result;
}

/**
 * This interface returns the freshness value from the Most Significant Bits in the first byte
 * in the array (FVMFreshnessValue), in big endian format.
 */
/* PRQA S 1503, 1532, 3673 ++ */ /* VL_CDD_FVM_1503, VL_CDD_FVM_1532, VL_CDD_FVM_3673 */
Std_ReturnType
    CDD_FVM_GetTxFreshness(uint16 FVMFreshnessValueID, uint8* FVMFreshnessValue, uint32* FVMFreshnessValueLength)
/* PRQA S 1503, 1532, 3673 -- */
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16           index;
    Std_ReturnType   result = E_NOT_OK;
    uint8            tempLength;
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_UNINIT == CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETTXFRESHNESS, CDD_FVM_E_UNINIT);
    }
    else if ((NULL_PTR == FVMFreshnessValue) || (NULL_PTR == FVMFreshnessValueLength))
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETTXFRESHNESS, CDD_FVM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
        index = CDD_FVM_GetTxFreshnessConfigIndex(FVMFreshnessValueID, &mode);
        switch (mode)
        {
#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_SINGLE_COUNTER:
            tempLength = (uint8)(*FVMFreshnessValueLength);
            result     = CDD_FVM_GetTxSingleCounterFreshnessValue(index, FVMFreshnessValue, tempLength);
            break;
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_TRUNCATED_MULTIPLE_COUNTER:
            result = E_OK;
            break;
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
        default:
            result = E_OK;
            (void)(mode);
            break;
        }
    }

    return result;
}

/**
 * This interface is used by the FVM to obtain the current freshness value. The interface function
 * provides also the truncated freshness value transmitted in the secured I-PDU.
 */
/* PRQA S 1503, 1532, 3673 ++ */ /* VL_CDD_FVM_1503, VL_CDD_FVM_1532, VL_CDD_FVM_3673 */
Std_ReturnType CDD_FVM_GetTxFreshnessTruncData(
    uint16  FVMFreshnessValueID,
    uint8*  FVMFreshnessValue,
    uint32* FVMFreshnessValueLength,
    uint8*  FVMTruncatedFreshnessValue,
    uint32* FVMTruncatedFreshnessValueLength)
/* PRQA S 1503, 1532, 3673 -- */
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16           index;
    Std_ReturnType   result = E_NOT_OK;
    uint8            tempLength;
    uint8            tempallLength;
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_UNINIT == CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETTXFRESHNESSTRUNCDATA, CDD_FVM_E_UNINIT);
    }
    else if (
        (NULL_PTR == FVMFreshnessValue) || (NULL_PTR == FVMFreshnessValueLength)
        || (NULL_PTR == FVMTruncatedFreshnessValue) || (NULL_PTR == FVMTruncatedFreshnessValueLength))
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETTXFRESHNESSTRUNCDATA, CDD_FVM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
        index = CDD_FVM_GetTxFreshnessConfigIndex(FVMFreshnessValueID, &mode);
        switch (mode)
        {
#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_SINGLE_COUNTER:
            tempallLength = (uint8)(*FVMFreshnessValueLength);

            /* Get freshness value */
            result = CDD_FVM_GetTxSingleCounterFreshnessValue(index, FVMFreshnessValue, tempallLength);
            /* Get truncated freshness value */
            if ((E_OK == result) && (*FVMTruncatedFreshnessValueLength <= *FVMFreshnessValueLength))
            {
                tempLength = (uint8)(*FVMTruncatedFreshnessValueLength);
                CDD_FVM_GetLowBitFromSrc(FVMFreshnessValue, FVMTruncatedFreshnessValue, tempallLength, tempLength);
            }
            break;
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_TRUNCATED_MULTIPLE_COUNTER:
            tempallLength = (uint8)(*FVMFreshnessValueLength);
            result        = CDD_FVM_GetTxMultipleTruncatedCounterFreshnessValue(
                index,
                FVMFreshnessValue,
                tempallLength,
                FVMTruncatedFreshnessValue,
                FVMTruncatedFreshnessValueLength);
            break;
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
        default:
            result = E_OK;
            (void)(mode);
            break;
        }
    }

    return result;
}

/**
 * This interface is used by the FVM to indicate that the Secured I-PDU has been initiated for
 * transmission.After the Secured I-PDU is successfully transferred, Fvm increments a single
 * counter as the freshness value.
 */
/* PRQA S 1503, 1532 ++ */ /* VL_CDD_FVM_1503 */
Std_ReturnType CDD_FVM_SPduTxConfirmation(uint16 FVMFreshnessValueID)
/* PRQA S 1503, 1532 -- */
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16           index;
    Std_ReturnType   result = E_NOT_OK;

#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_UNINIT == CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_SPDUTXCONFIRMATION, CDD_FVM_E_UNINIT);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
        index = CDD_FVM_GetTxFreshnessConfigIndex(FVMFreshnessValueID, &mode);
        switch (mode)
        {
#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_SINGLE_COUNTER:
            result = CDD_FVM_TxUpdateSingleCounterFreshnessValue(index);
            break;
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_TRUNCATED_MULTIPLE_COUNTER:
            result = E_OK;
            break;
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
        default:
            result = E_OK;
            (void)(mode);
            break;
        }
    }

    return result;
}

/**
 * This function is used to notify the reception of SecOC verification status, and update a variable
 * that holds the previous freshness value.
 */
/* PRQA S 1503, 1532 ++ */ /* VL_CDD_FVM_1503, VL_CDD_FVM_1532 */
void CDD_FVM_RxSecOCVerificationNotify(SecOC_VerificationStatusType verificationStatus)
/* PRQA S 1503, 1532 -- */
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;

#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_UNINIT == CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_RXSECOCVERIFICATIONNOTIFY, CDD_FVM_E_UNINIT);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
        uint16 index = CDD_FVM_GetRxFreshnessConfigIndex(verificationStatus.freshnessValueID, &mode);
        switch (mode)
        {
#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_SINGLE_COUNTER:

            break;
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
        case CDD_FVM_MODE_TRUNCATED_MULTIPLE_COUNTER:
            if (SECOC_VERIFICATIONSUCCESS == verificationStatus.verificationStatus)
            {
                CDD_FVM_RxMultipleCounterIsSecOCPass(index);
            }
            break;
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
        default:
            (void)(mode);
            break;
        }
        CDD_FVM_UNUSED(index);
    }
}

/**
 * This function is the Rx indication callback function for the CDD FVM module.These include the processing
 * of synchronous message control frames by the master ECU, the processing of synchronous message control
 * frames by the slave ECU, and the processing of synchronous message control frames by the slave ECU.
 */
/* PRQA S 1503 ++ */ /* VL_CDD_FVM_1503 */
void CDD_FVM_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 1503 -- */
{
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_InitStatus == CDD_FVM_UNINIT)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_RXINDICATION_ID, CDD_FVM_E_UNINIT);
    }
    else if (RxPduId >= CDD_FVM_RX_IF_PDU_NUM)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_RXINDICATION_ID, CDD_FVM_E_INVALID_PDU_SDU_ID);
    }
    else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_RXINDICATION_ID, CDD_FVM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
        const CDD_FVM_MasterConfigType* tempFvmMasterConfig = CDD_FVM_ConfigPtr->FvmMasterEcuConfig;
        for (uint16 index = 0u; index < CDD_FVM_MASTER_ECU_CONFIG_NUM; index++)
        {
            if (tempFvmMasterConfig[index].fvmCtrlPduRef != NULL_PTR)
            {
                if (RxPduId == tempFvmMasterConfig[index].fvmCtrlPduRef->FvMIfRxPduHandleId)
                {
                    /* Processes received control frames from slaves */
                    CDD_FVM_MasterRxCtrlPduFrameHandle(index, PduInfoPtr);
                    break;
                }
            }
        }
#endif /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
        const CDD_FVM_SlaveECUSyncConfigType* tempFvmSlaveConfig = CDD_FVM_ConfigPtr->FvmSlaveEcuConfig;
        boolean                               rxPduTag           = FALSE;
        for (uint16 index = 0; index < CDD_FVM_SLAVE_CONFIG_NUM; index++)
        {
            if (tempFvmSlaveConfig[index].FvMSyncMsgRef != NULL_PTR)
            {
                if (RxPduId == tempFvmSlaveConfig[index].FvMSyncMsgRef->RxPduHandleId)
                {
                    /* Processes synchronized message frames from the master node */
                    CDD_FVM_SlaveRxSyncMessagePduFrameHandle(index, PduInfoPtr);
                    rxPduTag = TRUE;
                }
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
                else if (NULL_PTR != tempFvmSlaveConfig[index].FvMCtrlPduRef)
                {
                    if (RxPduId == tempFvmSlaveConfig[index].FvMCtrlPduRef->FvMIfRxPduHandleId)
                    {
                        /* Processing control frames from the master node */
                        CDD_FVM_SlaveRxCtrlPduFrameHandle(index, PduInfoPtr);
                        rxPduTag = TRUE;
                    }
                }
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
                else
                {
                }
                if (TRUE == rxPduTag)
                {
                    break;
                }
            }
        }

#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */
        CDD_FVM_UNUSED(RxPduId);
        CDD_FVM_UNUSED(PduInfoPtr);
    }
}

#if (CDD_FVM_MASTER_ECU_ENABLED == STD_ON) && (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
/**
 * This function is the main function for the master ECU of the CDD FVM module.The master ECU sends
 * synchronous message frames to the slave ECU, and the slave ECU updates the local trip counter and
 * reset counter.
 */
void CDD_FVM_MainFunctionMaster(void)
{
    uint8                           tempSyncMsgLength;
    PduInfoType                     fvmpduInfo;
    PduIdType                       pduId;
    const CDD_FVM_MasterConfigType* tempMasterEcuConfig = NULL_PTR;
    if (CDD_FVM_INIT == CDD_FVM_InitStatus)
    {
        for (uint16 i = 0; i < CDD_FVM_MASTER_ECU_CONFIG_NUM; i++)
        {
            tempMasterEcuConfig = &CDD_FVM_ConfigPtr->FvmMasterEcuConfig[i];
            /* Every resetCycle, the reset counter is incremented by 1. */
            if (FVM_MasterEcuHandle[i].resetCycle > 0u)
            {
                FVM_MasterEcuHandle[i].resetCycle -= 1u;
                if (0u == FVM_MasterEcuHandle[i].resetCycle)
                {
                    FVM_MasterEcuHandle[i].isRunSend = TRUE;
                    tempSyncMsgLength = tempMasterEcuConfig->FvMTripCntLength + tempMasterEcuConfig->FvMResetCntLength;

                    if ((CDD_FVM_CTRL_INIT != FVM_MasterEcuHandle[i].status)
#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
                        || (CDD_FVM_CTRL_OK == FVM_SlaveEcuHandle[i].status)
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */
                    )
                    {
                        uint64 ResetCntMaxalue = CDD_FVM_GET_N_BITS_MAX_VALUE(tempMasterEcuConfig->FvMResetCntLength);
                        uint64 localResetCntValue = CDD_FVM_ConvertBufferToValueWithOffset(
                            FVM_MasterEcuHandle[i].FvMResetCnt,
                            0u,
                            tempMasterEcuConfig->FvMResetCntLength);
                        if (localResetCntValue < ResetCntMaxalue)
                        {
                            CDD_FVM_BigEndianIncreaseMessageCounter(
                                FVM_MasterEcuHandle[i].FvMResetCnt,
                                tempMasterEcuConfig->FvMResetCntLength);
                        }
                    }

                    CDD_FVM_CopyBufferToDstBits(
                        FVM_MasterEcuHandle[i].FvMSynMsgBuff,
                        FVM_MasterEcuHandle[i].FvMTripCnt,
                        0u,
                        tempMasterEcuConfig->FvMTripCntLength);

                    CDD_FVM_CoverageOfLowNBits(
                        FVM_MasterEcuHandle[i].FvMSynMsgBuff,
                        FVM_MasterEcuHandle[i].FvMResetCnt,
                        tempSyncMsgLength,
                        tempMasterEcuConfig->FvMResetCntLength);
                    /* Post synchronization messages to the bus */
                    fvmpduInfo.SduLength  = (PduLengthType)CDD_FVM_BIT_TO_BYTE(tempSyncMsgLength);
                    fvmpduInfo.SduDataPtr = FVM_MasterEcuHandle[i].FvMSynMsgBuff;
                    pduId                 = tempMasterEcuConfig->fvmSyncMsgPduRef->TxPduHandleId;

                    if (E_OK == PduR_CDD_FVMTransmit(pduId, &fvmpduInfo))
                    {
                        FVM_MasterEcuHandle[i].status     = CDD_FVM_CTRL_WAIT;
                        FVM_MasterEcuHandle[i].resetCycle = tempMasterEcuConfig->FvMResetCycle;
                    }
                    else
                    {
                        FVM_MasterEcuHandle[i].resetCycle = 1u;
                    }
                }
                else
                {
                    /* Wait one cycle to prevent receiving a Request Synchronization message frame from causing another,
                     * and immediately send a new one */
                    if (TRUE == FVM_MasterEcuHandle[i].isRunSend)
                    {
                        FVM_MasterEcuHandle[i].isRunSend = FALSE;
                    }
                }
            }
        }
#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
#if (CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON)
        const CDD_FVM_SlaveECUSyncConfigType* tempFvmSlaveConfig = CDD_FVM_ConfigPtr->FvmSlaveEcuConfig;
        for (uint16 i = 0; i < CDD_FVM_SLAVE_CONFIG_NUM; i++)
        {
            uint16 masterIndex = tempFvmSlaveConfig[i].FvMCommonMasterIndex;
            if (0xFFFFu != masterIndex)
            {
                if (CDD_FVM_CTRL_WAIT == FVM_MasterEcuHandle[masterIndex].status)
                {
                    CDD_FVM_SlaveCopyMasterSyncMsg(masterIndex, i);
                    FVM_SlaveEcuHandle[i].status            = CDD_FVM_CTRL_OK;
                    FVM_MasterEcuHandle[masterIndex].status = CDD_FVM_CTRL_INIT;
                }
            }
        }

#endif /* CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON */
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */
    }
}
#endif /* CDD_FVM_MASTER_ECU_ENABLED == STD_ON && CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
/**
 * This function is used to request a synchronization message by the slave.
 */
Std_ReturnType CDD_FVM_SlaveRequestSyncMsg(uint16 syncMsgId)
{
    const CDD_FVM_SlaveECUSyncConfigType* tempSlaveEcuConfig = NULL_PTR;
    Std_ReturnType                        ret                = E_NOT_OK;
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_InitStatus == CDD_FVM_UNINIT)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_SLAVEREQUESTSYNCMSG, CDD_FVM_E_UNINIT);
    }
    else if (syncMsgId > CDD_FVM_SLAVE_CONFIG_NUM)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_SLAVEREQUESTSYNCMSG, CDD_FVM_E_PARAM_POINTER);
    }
    else
#endif
    {
        uint16 index;
        uint16 currentIndex = 0xFFFF;
        for (index = 0; index < CDD_FVM_SLAVE_CONFIG_NUM; index++)
        {
            tempSlaveEcuConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
            if (tempSlaveEcuConfig->FvMSyncMsgId == syncMsgId)
            {
                currentIndex = index;
                break;
            }
        }
        /* Sends data to the host requesting a synchronization message frame*/
        if (0xFFFFu != currentIndex)
        {
            if (CDD_FVM_CTRL_OK == FVM_SlaveEcuHandle[index].status)
            {
                uint8*      tempCtrlBufferRef = FVM_SlaveEcuHandle[index].FvmCtrlBuffer;
                PduInfoType pduInfo;
                PduIdType   pduId;

                /* Constructing an acknowledgement message to be sent */
                tempCtrlBufferRef[0u] = CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION << 6u;
                tempCtrlBufferRef[0u] |= CDD_FVM_CTRL_MSG_TYPE_SLAVE_REQUSEST(tempCtrlBufferRef[0u]);
                tempCtrlBufferRef[1u]               = CDD_FVM_MULTIPLE_INSTANCE_ID;
                tempCtrlBufferRef[2u]               = (uint8)(tempSlaveEcuConfig->FvMSyncMsgId >> CDD_FVM_CONST_8U);
                tempCtrlBufferRef[CDD_FVM_CONST_3U] = (uint8)(tempSlaveEcuConfig->FvMSyncMsgId & 0xFFu);
                tempCtrlBufferRef[4u]               = 0u;
                tempCtrlBufferRef[5u]               = 0u;
                tempCtrlBufferRef[6u]               = 0u;
                tempCtrlBufferRef[7u]               = 0u;
                pduId                               = tempSlaveEcuConfig->FvMCtrlPduRef->FvMIfTxPduHandleId;
                pduInfo.SduDataPtr                  = tempCtrlBufferRef;
                pduInfo.SduLength                   = CDD_FVM_CONST_8U;
                (void)PduR_CDD_FVMTransmit(pduId, &pduInfo);
                ret = E_OK;
            }
        }
    }
    return ret;
}
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */

#define CDD_FVM_STOP_SEC_CODE
#include "CDD_FVM_MemMap.h"

/* ========================================== internal function definitions ========================================= */

#define CDD_FVM_START_SEC_CODE
#include "CDD_FVM_MemMap.h"

#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
CDD_FVM_LOCAL void CDD_FVM_MasterEcuInit(const CDD_FVM_ConfigType* config)
{
    const CDD_FVM_MasterConfigType* tempFvmMasterConfig = config->FvmMasterEcuConfig;
    for (uint16 i = 0u; i < CDD_FVM_MASTER_ECU_CONFIG_NUM; i++)
    {
        FVM_MasterEcuHandle[i].status = CDD_FVM_CTRL_INIT;

#if (CDD_FVM_NVM_BLOCK_ENABLED == STD_ON)
        NvM_RequestResultType RequestResultPtr;
        (void)NvM_GetErrorStatus(tempFvmMasterConfig[i].blkId, &RequestResultPtr);
        if (NVM_REQ_OK == RequestResultPtr)
        {
            CDD_FVM_Memcpy(
                FVM_MasterEcuHandle[i].FvMTripCnt,
                (const uint8*)NvM_BlockDescriptor[tempFvmMasterConfig[i].blkId - 1u].NvmRamBlockDataAddress,
                CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE * CDD_FVM_CONST_8U);
            CDD_FVM_BigEndianIncreaseMessageCounter(
                FVM_MasterEcuHandle[i].FvMTripCnt,
                tempFvmMasterConfig[i].FvMTripCntLength);
            CDD_FVM_SetMemoryBytesToValue(
                FVM_MasterEcuHandle[i].FvMResetCnt,
                0u,
                tempFvmMasterConfig[i].FvMResetCntLength);
            CDD_FVM_BigEndianIncreaseMessageCounter(
                FVM_MasterEcuHandle[i].FvMResetCnt,
                tempFvmMasterConfig[i].FvMResetCntLength);
            if (0u
                == CDD_FVM_ConvertBufferToValueWithOffset(
                    FVM_MasterEcuHandle[i].FvMTripCnt,
                    0u,
                    tempFvmMasterConfig[i].FvMTripCntLength))
            {
                CDD_FVM_BigEndianIncreaseMessageCounter(
                    FVM_MasterEcuHandle[i].FvMTripCnt,
                    tempFvmMasterConfig[i].FvMTripCntLength);
            }
            (void)NvM_CancelJobs(tempFvmMasterConfig[i].blkId);
            (void)NvM_WriteBlock(tempFvmMasterConfig[i].blkId, FVM_MasterEcuHandle[i].FvMTripCnt);
        }
        else
#endif /* CDD_FVM_NVM_BLOCK_ENABLED == STD_ON */
        {
            CDD_FVM_SetMemoryBytesToValue(
                FVM_MasterEcuHandle[i].FvMTripCnt,
                0u,
                tempFvmMasterConfig[i].FvMTripCntLength);
            CDD_FVM_BigEndianIncreaseMessageCounter(
                FVM_MasterEcuHandle[i].FvMTripCnt,
                tempFvmMasterConfig[i].FvMTripCntLength);
            CDD_FVM_SetMemoryBytesToValue(
                FVM_MasterEcuHandle[i].FvMResetCnt,
                0u,
                tempFvmMasterConfig[i].FvMResetCntLength);
            CDD_FVM_BigEndianIncreaseMessageCounter(
                FVM_MasterEcuHandle[i].FvMResetCnt,
                tempFvmMasterConfig[i].FvMResetCntLength);
#if (CDD_FVM_NVM_BLOCK_ENABLED == STD_ON)
            (void)NvM_CancelJobs(tempFvmMasterConfig[i].blkId);
            (void)NvM_WriteBlock(tempFvmMasterConfig[i].blkId, FVM_MasterEcuHandle[i].FvMTripCnt);
#endif /* CDD_FVM_NVM_BLOCK_ENABLED == STD_ON */
        }
#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
        for (uint8 j = 0u; j < CDD_FVM_MASTER_RX_SLAVE_DATA_MAX; j++)
        {
            /* code */
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].FvmEcuInstanceId = 0u;
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].FvmRxCounter     = 0u;
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].isUse            = FALSE;
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].FvmSyncMsgId     = 0u;
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].status           = CDD_FVM_CTRL_INIT;
        }
#endif /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */
        /* Host sends synchronization messages as early as possible after power-up */
        FVM_MasterEcuHandle[i].resetCycle = 1u;
        FVM_MasterEcuHandle[i].isRunSend  = FALSE;
    }
}

#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
CDD_FVM_LOCAL uint8 CDD_FVM_FindMasterCtrlSlaveIdleData(uint16 index, uint16 syncMsgId)
{
    uint8                               retIndex = 0xFFu;
    uint8                               index_i;
    const CDD_FVM_MasterECUSyncMsgType* tempMasterEcuHandle = &FVM_MasterEcuHandle[index];
    for (index_i = 0u; index_i < CDD_FVM_MASTER_RX_SLAVE_DATA_MAX; index_i++)
    {
        if (tempMasterEcuHandle->slaveRxCtrlData[index_i].isUse == TRUE)
        {
            if (tempMasterEcuHandle->slaveRxCtrlData[index_i].FvmSyncMsgId == syncMsgId)
            {
                retIndex = index_i;
            }
        }
        else
        {
            retIndex = index_i;
        }
        if (0xFFu != retIndex)
        {
            break;
        }
    }
    return retIndex;
}

CDD_FVM_LOCAL uint8 CDD_FVM_FindMasterCtrlCurrentData(uint16 index, uint16 syncMsgId)
{
    uint8                               retIndex = 0xFFu;
    uint8                               index_i;
    const CDD_FVM_MasterECUSyncMsgType* tempMasterEcuHandle = &FVM_MasterEcuHandle[index];

    for (index_i = 0u; index_i < CDD_FVM_MASTER_RX_SLAVE_DATA_MAX; index_i++)
    {
        if (tempMasterEcuHandle->slaveRxCtrlData[index_i].FvmSyncMsgId == syncMsgId)
        {
            if (CDD_FVM_CTRL_OK == tempMasterEcuHandle->slaveRxCtrlData[index_i].status)
            {
                retIndex = index_i;
            }
            break;
        }
    }
    return retIndex;
}

CDD_FVM_LOCAL void CDD_FVM_MasterRxCtrlPduFrameHandle(uint16 index, const PduInfoType* PduInfoPtr)
{
    const CDD_FVM_MasterConfigType* tempFvmMasterConfig = &CDD_FVM_ConfigPtr->FvmMasterEcuConfig[index];
    CDD_FVM_MasterECUSyncMsgType*   tempMasterEcuHandle = &FVM_MasterEcuHandle[index];
    PduInfoType                     pduInfo;
    PduIdType                       pduId;
    uint8                           slaveIndex;
    /* Determine what the currently received message is dealing with */

    uint8 tempProtocolVersion = PduInfoPtr->SduDataPtr[0u] >> 6u;
    uint8 tempCtrlType        = PduInfoPtr->SduDataPtr[0u] & ((1u << 5u) - 1u);
    if (CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION != tempProtocolVersion)
    {
        uint16 rxsyncMsgId =
            ((uint16)PduInfoPtr->SduDataPtr[2u] << CDD_FVM_CONST_8U) | PduInfoPtr->SduDataPtr[CDD_FVM_CONST_3U];
        if (CDD_FVM_CTRL_MSG_TYPE_VERIFY == tempCtrlType)
        {
            slaveIndex = CDD_FVM_FindMasterCtrlSlaveIdleData(index, rxsyncMsgId);
            if (0xFFu != slaveIndex)
            {
                CDD_FVM_MasterRxSlveCtrlInfoType* tempRxSlaveCtrlData =
                    &tempMasterEcuHandle->slaveRxCtrlData[slaveIndex];
                uint8* tempCtrlBufferRef              = tempMasterEcuHandle[index].FvmCtrlBuffer;
                tempRxSlaveCtrlData->isUse            = TRUE;
                tempRxSlaveCtrlData->FvmEcuInstanceId = PduInfoPtr->SduDataPtr[1u];
                tempRxSlaveCtrlData->FvmSyncMsgId     = rxsyncMsgId;
                tempCtrlBufferRef[0u]                 = CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION << 6u;
                tempCtrlBufferRef[0u] |= CDD_FVM_CTRL_MSG_TYPE_MASTER_ASK(tempCtrlBufferRef[0u]);
                tempCtrlBufferRef[1u]               = CDD_FVM_MULTIPLE_INSTANCE_ID;
                tempCtrlBufferRef[2u]               = PduInfoPtr->SduDataPtr[2u];
                tempCtrlBufferRef[CDD_FVM_CONST_3U] = PduInfoPtr->SduDataPtr[CDD_FVM_CONST_3U];
                if ((PduInfoPtr->SduDataPtr[4u] != tempFvmMasterConfig->FvMTripCntLength)
                    || (PduInfoPtr->SduDataPtr[5u] != tempFvmMasterConfig->FvMResetCntLength))
                {
                    tempCtrlBufferRef[4u]       = (uint8)CDD_FVM_CTRL_ERR;
                    tempRxSlaveCtrlData->status = CDD_FVM_CTRL_ERR;
                }
                else
                {
                    tempCtrlBufferRef[4u]       = (uint8)CDD_FVM_CTRL_OK;
                    tempRxSlaveCtrlData->status = CDD_FVM_CTRL_OK;
                }
                tempRxSlaveCtrlData->FvmRxCounter++;
                pduId              = tempFvmMasterConfig->fvmCtrlPduRef->FvMIfTxPduHandleId;
                pduInfo.SduDataPtr = tempCtrlBufferRef;
                pduInfo.SduLength  = CDD_FVM_CTRL_FRAME_BYTES_MAX;
                (void)PduR_CDD_FVMTransmit(pduId, &pduInfo);
            }
        }
        else if (CDD_FVM_CTRL_MSG_TYPE_REQUEST == tempCtrlType)
        {
            /* When a Request Synchronization message is received */
            slaveIndex = CDD_FVM_FindMasterCtrlCurrentData(index, rxsyncMsgId);
            if (0xFFu != slaveIndex)
            {
                if (TRUE != tempMasterEcuHandle->isRunSend)
                {
                    tempMasterEcuHandle->resetCycle = 0u;
                }
            }
        }
        else
        {
            /* Invalid frames, not processed  */
        }
    }
}
#endif /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */

#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
CDD_FVM_LOCAL void CDD_FVM_SlaveEcuInit(const CDD_FVM_ConfigType* config)
{
    const CDD_FVM_SlaveECUSyncConfigType* tempFvmSlaveConfig = config->FvmSlaveEcuConfig;
    for (uint16 i = 0u; i < CDD_FVM_SLAVE_CONFIG_NUM; i++)
    {
        FVM_SlaveEcuHandle[i].status = CDD_FVM_CTRL_INIT;
#if (CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON)
        if (0xFFFFu != tempFvmSlaveConfig[i].FvMCommonMasterIndex)
        {
            CDD_FVM_SlaveCopyMasterSyncMsg(tempFvmSlaveConfig[i].FvMCommonMasterIndex, i);
        }
        else
#endif /* CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON */
        {
#if (CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0u)
            boolean getNvmOk;
            getNvmOk = FALSE;
            if (NULL_PTR != tempFvmSlaveConfig[i].blkIdRef)
            {
                NvM_RequestResultType RequestResultPtr;
                (void)NvM_GetErrorStatus((*tempFvmSlaveConfig[i].blkIdRef), &RequestResultPtr);
                if (NVM_REQ_OK == RequestResultPtr)
                {
                    CDD_FVM_Memcpy(
                        FVM_SlaveEcuHandle[i].FvMTripCnt,
                        (const uint8*)NvM_BlockDescriptor[(*tempFvmSlaveConfig[i].blkIdRef) - 1u]
                            .NvmRamBlockDataAddress,
                        CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE * CDD_FVM_CONST_8U);
                    getNvmOk = TRUE;
                }
            }
            if (TRUE != getNvmOk)
#endif /* CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0u */
            {
                CDD_FVM_SetMemoryBytesToValue(
                    FVM_SlaveEcuHandle[i].FvMTripCnt,
                    0u,
                    tempFvmSlaveConfig[i].FvMTripCntLength);
            }
            CDD_FVM_SetMemoryBytesToValue(
                FVM_SlaveEcuHandle[i].FvMResetCnt,
                0u,
                tempFvmSlaveConfig[i].FvMResetCntLength);
        }
    }
}

#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
CDD_FVM_LOCAL void CDD_FVM_SLaveSendAskMessageToMaster(uint16 index)
{
    const CDD_FVM_SlaveECUSyncConfigType* tempFvmSlaveConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
    uint8*                                tempCtrlBufferRef  = FVM_SlaveEcuHandle[index].FvmCtrlBuffer;
    PduInfoType                           pduInfo;
    PduIdType                             pduId;

    /* Constructing an acknowledgement message to be sent */
    tempCtrlBufferRef[0u] = CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION << 6u;
    tempCtrlBufferRef[0u] |= CDD_FVM_CTRL_MSG_TYPE_SLAVE_VERIFY(tempCtrlBufferRef[0u]);
    tempCtrlBufferRef[1u]               = CDD_FVM_MULTIPLE_INSTANCE_ID;
    tempCtrlBufferRef[2u]               = (uint8)(tempFvmSlaveConfig->FvMSyncMsgId >> CDD_FVM_CONST_8U);
    tempCtrlBufferRef[CDD_FVM_CONST_3U] = (uint8)(tempFvmSlaveConfig->FvMSyncMsgId & 0xFFu);
    tempCtrlBufferRef[4u]               = tempFvmSlaveConfig->FvMTripCntLength;
    tempCtrlBufferRef[5u]               = tempFvmSlaveConfig->FvMResetCntLength;
    FVM_SlaveEcuHandle[index].FvmCounter += 1u;
    tempCtrlBufferRef[6u] = FVM_SlaveEcuHandle[index].FvmCounter;
    tempCtrlBufferRef[7u] = 0u;

    pduInfo.SduDataPtr = tempCtrlBufferRef;
    pduInfo.SduLength  = CDD_FVM_CTRL_FRAME_BYTES_MAX;

    pduId = tempFvmSlaveConfig->FvMCtrlPduRef->FvMIfTxPduHandleId;
    (void)PduR_CDD_FVMTransmit(pduId, &pduInfo);
}
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */

CDD_FVM_LOCAL void CDD_FVM_SlaveRxSyncMessagePduFrameHandle(uint16 index, const PduInfoType* PduInfoPtr)
{
    const CDD_FVM_SlaveECUSyncConfigType* tempFvmSlaveConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
    uint8 syncMsgBitLength = tempFvmSlaveConfig->FvMResetCntLength + tempFvmSlaveConfig->FvMTripCntLength;
    uint8 tempsyncMsgBytes = CDD_FVM_BIT_TO_BYTE(syncMsgBitLength);
    uint8 remainingBits    = CDD_FVM_MOD_FROM_EIGHT(syncMsgBitLength);
    uint8 tempFvMTripCnt[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE];

    const uint8* tempPduBuff = PduInfoPtr->SduDataPtr;
    if (tempsyncMsgBytes <= PduInfoPtr->SduLength)
    {
        /* Trip counter for synchronization messages from received data */
        CDD_FVM_CopyBitsFromSourceToDestination(tempPduBuff, tempFvMTripCnt, 0u, tempFvmSlaveConfig->FvMTripCntLength);
        if (CDD_FVM_SlaveTripCounterMaxValueHandle(index, tempFvMTripCnt, FVM_SlaveEcuHandle[index].FvMTripCnt) == E_OK)
        {
            /* Reset counter for synchronization messages from received data */
            CDD_FVM_CopyBitsFromSourceToDestination(
                tempPduBuff,
                FVM_SlaveEcuHandle[index].FvMResetCnt,
                tempFvmSlaveConfig->FvMTripCntLength,
                tempFvmSlaveConfig->FvMResetCntLength);
#if (CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0u)
            if (NULL_PTR != tempFvmSlaveConfig->blkIdRef)
            {
                NvM_CancelJobs(*tempFvmSlaveConfig->blkIdRef);
                (void)NvM_WriteBlock(*tempFvmSlaveConfig->blkIdRef, FVM_SlaveEcuHandle[index].FvMTripCnt);
            }
#endif /* CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0u */
        }
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
        if (tempFvmSlaveConfig->FvMCtrlPduRef != NULL_PTR)
        {
            if (CDD_FVM_CTRL_INIT == FVM_SlaveEcuHandle[index].status)
            {
                /* Confirmation to the master ECU */
                FVM_SlaveEcuHandle[index].status = CDD_FVM_CTRL_WAIT;
                CDD_FVM_SLaveSendAskMessageToMaster(index);
            }
        }
        else
        {
            FVM_SlaveEcuHandle[index].status = CDD_FVM_CTRL_OK;
        }

#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
    }
}

CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_SlaveTripCounterMaxValueHandle(uint16 index, const uint8* rxtripCntRef, const uint8* localtripCntRef)
{
    /* Condition 1: "Maximum value of the trip counter" diminish "ClearAcceptanceWindow" <= "Latest value of the trip
     * counter maintained by the slave ECU" >= "Maximum value of the trip counter" */
    /* Condition 2: "Initial value of the trip counter" <="Trip counter value in the synchronization message" <=
     * "Initial value of the trip counter" + "ClearAcceptanceWindow"*/
    Std_ReturnType                        ret                = E_NOT_OK;
    const CDD_FVM_SlaveECUSyncConfigType* tempFvmSlaveConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
    uint64 TripMaxalue              = CDD_FVM_GET_N_BITS_MAX_VALUE(tempFvmSlaveConfig->FvMTripCntLength);
    uint16 fvmClearacceptanceWindow = tempFvmSlaveConfig->FvMClearAcceptanceWindow;
    uint64 newTripCntValue =
        CDD_FVM_ConvertBufferToValueWithOffset(rxtripCntRef, 0u, tempFvmSlaveConfig->FvMTripCntLength);
    uint64 oldTripCntValue =
        CDD_FVM_ConvertBufferToValueWithOffset(localtripCntRef, 0u, tempFvmSlaveConfig->FvMTripCntLength);
    if (newTripCntValue >= oldTripCntValue)
    {
        CDD_FVM_Memcpy(FVM_SlaveEcuHandle[index].FvMTripCnt, rxtripCntRef, tempFvmSlaveConfig->FvMTripCntLength);
        ret = E_OK;
    }
    else
    {
        if (((oldTripCntValue >= (TripMaxalue - fvmClearacceptanceWindow)) && (oldTripCntValue <= TripMaxalue))
            && ((CDD_FVM_TRIP_COUNTER_INIT_VALUE <= newTripCntValue)
                && ((uint64)(CDD_FVM_TRIP_COUNTER_INIT_VALUE + (uint64)fvmClearacceptanceWindow) >= newTripCntValue)))
        {
            CDD_FVM_Memcpy(FVM_SlaveEcuHandle[index].FvMTripCnt, rxtripCntRef, tempFvmSlaveConfig->FvMTripCntLength);
#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
            CDD_FVM_TxMultipleTruncatedCounterInit(CDD_FVM_ConfigPtr, index);
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
            CDD_FVM_RxMultipleTruncatedCounterInit(CDD_FVM_ConfigPtr, index);
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
            ret = E_OK;
        }
    }
    return ret;
}

#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
CDD_FVM_LOCAL void CDD_FVM_SlaveRxCtrlPduFrameHandle(uint16 index, const PduInfoType* PduInfoPtr)
{
    const CDD_FVM_SlaveECUSyncConfigType* tempFvmSlaveConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
    boolean                               tempResult         = FALSE;
    if (CDD_FVM_CTRL_WAIT == FVM_SlaveEcuHandle[index].status)
    {
        uint8 tempProtocolVersion = PduInfoPtr->SduDataPtr[0u] >> 6u;
        uint8 tempCtrlType        = PduInfoPtr->SduDataPtr[0u] & ((1u << 5u) - 1u);
        if (CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION == tempProtocolVersion)
        {
            if (CDD_FVM_CTRL_MSG_TYPE_ASK == tempCtrlType)
            {
                if ((uint8)CDD_FVM_CTRL_OK == PduInfoPtr->SduDataPtr[4u])
                {
                    FVM_SlaveEcuHandle[index].status = CDD_FVM_CTRL_OK;
                    tempResult                       = TRUE;
                }
            }
        }
        if (FALSE == tempResult)
        {
            FVM_SlaveEcuHandle[index].status = CDD_FVM_CTRL_ERR;
        }
    }
}
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */

#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */

#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
CDD_FVM_LOCAL void CDD_FVM_TxSingleCounterInit(void)
{
    uint16 index;

    for (index = 0u; index < CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM; index++)
    {
        /* It needs to be added once during initialization and starts sending from 1. */
        (void)CDD_FVM_TxUpdateSingleCounterFreshnessValue(index);
    }
}

CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_GetTxSingleCounterFreshnessValue(uint16 freshnessIndex, uint8* freshnessValue, uint8 FreshnessLength)
{
    Std_ReturnType result = E_NOT_OK;
    uint8          fvmFreshnessLength;

    fvmFreshnessLength = CDD_FVM_ConfigPtr->FvmTxSingleFreshnessConfig[freshnessIndex].FvmFreshnessLength;
    if (FreshnessLength == fvmFreshnessLength)
    {
        CDD_FVM_Memcpy(freshnessValue, FVM_TxSingleCntHandle[freshnessIndex].FVMmsgCnt, fvmFreshnessLength);
        result = E_OK;
    }
    return result;
}

CDD_FVM_LOCAL Std_ReturnType CDD_FVM_TxUpdateSingleCounterFreshnessValue(uint16 freshnessIndex)
{
    Std_ReturnType result = E_NOT_OK;
    if (freshnessIndex < CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM)
    {
        CDD_FVM_BigEndianIncreaseMessageCounter(
            FVM_TxSingleCntHandle[freshnessIndex].FVMmsgCnt,
            CDD_FVM_ConfigPtr->FvmTxSingleFreshnessConfig[freshnessIndex].FvmFreshnessLength);
        result = E_OK;
    }
    return result;
}

#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)

CDD_FVM_LOCAL void
    CDD_FVM_RxSingleCounterInit(const CDD_FVM_RxSingleFreshnessCounterConfigType* RxFreshnessSingleConfig)
{
    uint16 index;

    for (index = 0u; index < CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM; index++)
    {
        CDD_FVM_SetMemoryBytesToValue(
            FVM_RxSingleCntHandle[index].FVMmsgCnt,
            0u,
            RxFreshnessSingleConfig[index].FvmFreshnessLength);
    }
}

CDD_FVM_LOCAL Std_ReturnType CDD_FVM_GetRxSingleCounterFreshnessValue(uint16 freshnessIndex, uint8* freshnessValue)
{
    Std_ReturnType result = E_NOT_OK;
    if ((freshnessIndex < CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM) && (NULL_PTR != freshnessValue))
    {
        CDD_FVM_Memcpy(
            freshnessValue,
            FVM_RxSingleCntHandle[freshnessIndex].FVMmsgCnt,
            CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[freshnessIndex].FvmFreshnessLength);
        result = E_OK;
    }
    return result;
}

CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_RxUpdateSingleCounterFreshnessValue(uint16 freshnessIndex, const uint8* freshnessValue)
{
    Std_ReturnType result = E_NOT_OK;
    if (freshnessIndex < CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM)
    {
        CDD_FVM_Memcpy(
            FVM_RxSingleCntHandle[freshnessIndex].FVMmsgCnt,
            freshnessValue,
            CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[freshnessIndex].FvmFreshnessLength);
        result = E_OK;
    }
    return result;
}

/* PRQA S 6070, 6030, 6040 ++ */ /* VL_MTR_CDD_FVM_STCAL, VL_MTR_CDD_FVM_STMIF, VL_MTR_CDD_FVM_STPAR */
/* PRQA S 2889 ++ */             /* VL_QAC_MultiReturn */
CDD_FVM_LOCAL Std_ReturnType CDD_FVM_RxSingleCounterFreshnessHandle(
    uint16       freshnessIndex,
    const uint8* FVMTruncatedFreshnessValue,
    uint32       FVMTruncatedFreshnessValueLength,
    uint16       FVMAuthVerifyAttempts,
    uint8*       FVMFreshnessValue,
    uint32*      FVMFreshnessValueLength)
/* PRQA S 2889 -- */
/* PRQA S 6070, 6030, 6040 -- */
{
    Std_ReturnType result                                              = E_NOT_OK;
    uint8          freshnessTempBuff[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE] = {0};
    uint8*         freshnessRxTruncTempBuff                            = NULL_PTR;
    uint8          RxFreshnessLength = CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[freshnessIndex].FvmFreshnessLength;
    uint16         RxMaxDeltaCounter = CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[freshnessIndex].MaxDeltaCounter;
    if ((freshnessIndex < CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM) && (*FVMFreshnessValueLength <= RxFreshnessLength))
    {
        freshnessRxTruncTempBuff = FVM_RxSingleCntHandle[freshnessIndex].FVMmsgRxFreshnessValue;
        result                   = CDD_FVM_GetRxSingleCounterFreshnessValue(freshnessIndex, freshnessTempBuff);
        CDD_FVM_Memcpy(freshnessRxTruncTempBuff, FVMTruncatedFreshnessValue, (uint8)FVMTruncatedFreshnessValueLength);

        if (E_OK == result)
        {
            /* Determine whether the incoming freshness value is complete */
            if (FVMTruncatedFreshnessValueLength == RxFreshnessLength)
            {
                /* Used to handle situations where the stored value is greater than the received value after possible
                 * overflow. */
                if (E_OK
                    == (CDD_FVM_JudgmentUpwardGrowth(
                        freshnessTempBuff,
                        FVMTruncatedFreshnessValue,
                        (uint8)FVMTruncatedFreshnessValueLength,
                        RxMaxDeltaCounter)))
                {
                    CDD_FVM_Memcpy(
                        FVMFreshnessValue,
                        FVMTruncatedFreshnessValue,
                        (uint8)FVMTruncatedFreshnessValueLength);
                    *FVMFreshnessValueLength = FVMTruncatedFreshnessValueLength;
                    (void)CDD_FVM_RxUpdateSingleCounterFreshnessValue(freshnessIndex, FVMFreshnessValue);
                    result = E_OK;
                }
                else
                {
                    result = E_NOT_OK;
                }
            }
            else
            {
                /* Handling truncated freshness values, Build verification freshness value*/
                /* Store the low-order data in the temporary Buffer and compare the size of the low-order values. */
                uint8 freshnessTempLowBitBuff[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE] = {0};
                CDD_FVM_GetLowBitFromSrc(
                    freshnessTempBuff,
                    freshnessTempLowBitBuff,
                    (uint8)RxFreshnessLength,
                    (uint8)FVMTruncatedFreshnessValueLength);

                /* The first value is the value obtained from the freshness manager
                 * and the second value is the received value */
                result = CDD_FVM_BitCompare(
                    freshnessTempLowBitBuff,
                    freshnessRxTruncTempBuff,
                    (uint8)FVMTruncatedFreshnessValueLength,
                    0u);

                if (CDD_FVM_E_MORE_THAN == result)
                {
                    /* When the received intercepted number is less than
                    the locally saved data, the high bit is added 1 */
                    const uint8 tempBuffAllOne[CDD_FVM_CONST_8U] =
                        {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
                    CDD_FVM_CoverageOfLowNBits(
                        freshnessTempBuff,
                        tempBuffAllOne,
                        RxFreshnessLength,
                        (uint8)FVMTruncatedFreshnessValueLength);
                    CDD_FVM_BigEndianIncreaseMessageCounter(freshnessTempBuff, RxFreshnessLength);
                    result = E_OK;
                }
                else if (result == CDD_FVM_E_NOT_OK)
                {
                    result = E_NOT_OK;
                }
                else
                {
                    result = E_OK;
                }

                CDD_FVM_Memcpy(FVMFreshnessValue, freshnessTempBuff, (uint8)RxFreshnessLength);
                /* Updating the local data low */
                CDD_FVM_CoverageOfLowNBits(
                    FVMFreshnessValue,
                    freshnessRxTruncTempBuff,
                    (uint8)RxFreshnessLength,
                    (uint8)FVMTruncatedFreshnessValueLength);

                if (E_OK == result)
                {
                    (void)CDD_FVM_RxUpdateSingleCounterFreshnessValue(freshnessIndex, FVMFreshnessValue);
                }
            }
        }
    }
    (void)FVMAuthVerifyAttempts;

    return result;
}

#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)

CDD_FVM_LOCAL void CDD_FVM_TxMultipleTruncatedCounterInit(const CDD_FVM_ConfigType* config, uint16 slaveSyncMsgIndex)
{
    const CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigType* tempTxFreshnessConfig =
        config->FvmTxMultipleFreshnessTruncateConfig;
    const CDD_FVM_MultipleFreshnessValueStructureType* tempFreshnessConfig = NULL_PTR;
    uint16                                             salveSyncIndex;

    for (uint16 i = 0; i < CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM; i++)
    {
        tempFreshnessConfig = tempTxFreshnessConfig[i].freshnessConfigRef;
        salveSyncIndex      = tempTxFreshnessConfig[i].syncMsgIndex;
        if ((0xFFFFu == slaveSyncMsgIndex) || (slaveSyncMsgIndex == salveSyncIndex))
        {
            CDD_FVM_SetMemoryBytesToValue(
                FVM_TxMultipleTruncatedCntHandle[i].FVMFreshnessValue,
                0u,
                tempTxFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_SetMemoryBytesToValue(
                FVM_TxMultipleTruncatedCntHandle[i].FVMPreviousValue,
                0u,
                tempTxFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_SetMemoryBytesToValue(
                FVM_TxMultipleTruncatedCntHandle[i].FvmMsgCnt,
                0u,
                tempFreshnessConfig->msgCntUpper.CounterLen + tempFreshnessConfig->msgCntLower.CounterLen);

            CDD_FVM_CopyBufferToDstBits(
                FVM_TxMultipleTruncatedCntHandle[i].FVMFreshnessValue,
                FVM_SlaveEcuHandle[salveSyncIndex].FvMTripCnt,
                tempFreshnessConfig->tirpCnt.CounterPos,
                tempFreshnessConfig->tirpCnt.CounterLen);
        }
    }
}

CDD_FVM_LOCAL Std_ReturnType CDD_FVM_GetTxMultipleTruncatedCounterFreshnessValue(
    uint16  freshnessIndex,
    uint8*  freshnessValue,
    uint8   FreshnessLength,
    uint8*  FVMTruncatedFreshnessValue,
    uint32* FVMTruncatedFreshnessValueLength)
{
    Std_ReturnType result;
    uint8 tempSyncMsgLatestBuffer[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE]     = {0};
    uint8 tempSyncMsgPreviouslyBuffer[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE] = {0};
    uint64                                                       latestSyncMsgValue;
    uint64                                                       previouslySyncMsgValue;
    const CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigType* tempTxFreshnessConfig =
        &CDD_FVM_ConfigPtr->FvmTxMultipleFreshnessTruncateConfig[freshnessIndex];
    const CDD_FVM_MultipleFreshnessValueStructureType* tempFreshnessConfig = tempTxFreshnessConfig->freshnessConfigRef;
    uint16                                             syncMsgIndex        = tempTxFreshnessConfig->syncMsgIndex;
    uint8                                              syncMsgLength;
    uint8                                              MsgCntLength;
    uint8                                              tempTruncatedCounterLength;

    do
    {
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
        if ((NULL_PTR != CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[syncMsgIndex].FvMCtrlPduRef)
            && (CDD_FVM_CTRL_ERR == FVM_SlaveEcuHandle[syncMsgIndex].status))
        {
            result = E_NOT_OK;
        }
        else if (
            (NULL_PTR != CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[syncMsgIndex].FvMCtrlPduRef)
            && ((CDD_FVM_CTRL_INIT == FVM_SlaveEcuHandle[syncMsgIndex].status)
                || (CDD_FVM_CTRL_WAIT == FVM_SlaveEcuHandle[syncMsgIndex].status)))
        {
            result = E_BUSY;
        }
        else
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
        {
            MsgCntLength  = tempFreshnessConfig->msgCntLower.CounterLen + tempFreshnessConfig->msgCntUpper.CounterLen;
            syncMsgLength = tempFreshnessConfig->tirpCnt.CounterLen + tempFreshnessConfig->resetCnt.CounterLen;
            tempTruncatedCounterLength =
                tempFreshnessConfig->msgCntLower.CounterLen + tempFreshnessConfig->resetFlag.CounterLen;
            if ((tempTruncatedCounterLength == (*FVMTruncatedFreshnessValueLength))
                && (FreshnessLength == tempTxFreshnessConfig->FvmFreshnessLength))
            {
                CDD_FVM_CopyBufferToDstBits(
                    tempSyncMsgLatestBuffer,
                    FVM_SlaveEcuHandle[syncMsgIndex].FvMTripCnt,
                    tempFreshnessConfig->tirpCnt.CounterPos,
                    tempFreshnessConfig->tirpCnt.CounterLen);
                CDD_FVM_CopyBufferToDstBits(
                    tempSyncMsgLatestBuffer,
                    FVM_SlaveEcuHandle[syncMsgIndex].FvMResetCnt,
                    tempFreshnessConfig->resetCnt.CounterPos,
                    tempFreshnessConfig->resetCnt.CounterLen);

                CDD_FVM_CopySrcFromMbitToDstBits(
                    FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FVMFreshnessValue,
                    tempSyncMsgPreviouslyBuffer,
                    tempFreshnessConfig->tirpCnt.CounterPos,
                    syncMsgLength);

                uint8 tempFirstBit            = 0u;
                uint8 tempAllSyncMsgAndOffset = tempFreshnessConfig->tirpCnt.CounterPos + syncMsgLength;

                if (0u != tempFreshnessConfig->tirpCnt.CounterPos)
                {
                    tempFirstBit = (CDD_FVM_MOD_FROM_EIGHT(tempAllSyncMsgAndOffset) == 0u)
                                       ? (0u)
                                       : (CDD_FVM_CONST_8U - CDD_FVM_MOD_FROM_EIGHT(tempAllSyncMsgAndOffset));
                }

                latestSyncMsgValue = CDD_FVM_ConvertBufferToValueWithOffset(
                    tempSyncMsgLatestBuffer,
                    tempFirstBit,
                    tempAllSyncMsgAndOffset);
                previouslySyncMsgValue = CDD_FVM_ConvertBufferToValueWithOffset(
                    tempSyncMsgPreviouslyBuffer,
                    tempFirstBit,
                    tempAllSyncMsgAndOffset);
                /* Determine if a synchronization message has changed */
                if (latestSyncMsgValue != previouslySyncMsgValue)
                {
                    CDD_FVM_SetMemoryBytesToValue(
                        FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FvmMsgCnt,
                        0u,
                        MsgCntLength);
                    CDD_FVM_CopySrcFromMbitToDstBits(
                        tempSyncMsgLatestBuffer,
                        FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FVMFreshnessValue,
                        tempFreshnessConfig->tirpCnt.CounterPos,
                        syncMsgLength);
                    uint8 tempResetFlagVlaue = 0u;
                    CDD_FVM_GetLowBitFromSrc(
                        FVM_SlaveEcuHandle[syncMsgIndex].FvMResetCnt,
                        &tempResetFlagVlaue,
                        tempFreshnessConfig->resetCnt.CounterLen,
                        tempFreshnessConfig->resetFlag.CounterLen);
                    CDD_FVM_CopyBufferToDstBits(
                        FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FVMFreshnessValue,
                        &tempResetFlagVlaue,
                        tempFreshnessConfig->resetFlag.CounterPos,
                        tempFreshnessConfig->resetFlag.CounterLen);
                }
                /* When the message counter reaches its maximum value, the sender ECU generates a validator by fixing
                 * the message counter to its maximum value.  */
                if (CDD_FVM_ConvertBufferToValueWithOffset(
                        FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FvmMsgCnt,
                        0u,
                        MsgCntLength)
                    != CDD_FVM_GET_N_BITS_MAX_VALUE(MsgCntLength))
                {
                    CDD_FVM_BigEndianIncreaseMessageCounter(
                        FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FvmMsgCnt,
                        MsgCntLength);
                }
                CDD_FVM_CopyBufferToDstBits(
                    FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FVMFreshnessValue,
                    FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FvmMsgCnt,
                    tempFreshnessConfig->msgCntUpper.CounterPos,
                    MsgCntLength);
                CDD_FVM_Memcpy(
                    freshnessValue,
                    FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FVMFreshnessValue,
                    tempTxFreshnessConfig->FvmFreshnessLength);
                /* Get the freshness value of the truncated portion from the freshness value */
                CDD_FVM_GetLowBitFromSrc(
                    FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FVMFreshnessValue,
                    FVMTruncatedFreshnessValue,
                    tempTxFreshnessConfig->FvmFreshnessLength,
                    tempTruncatedCounterLength);
                result = E_OK;
            }
            else
            {
                result = E_NOT_OK;
            }
        }
    } while (0);

    return result;
}

#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)

CDD_FVM_LOCAL void CDD_FVM_RxMultipleTruncatedCounterInit(const CDD_FVM_ConfigType* config, uint16 slaveSyncMsgIndex)
{
    const CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType* tempRxFreshnessConfig =
        config->FvmRxMultipleFreshnessTruncateConfig;
    const CDD_FVM_MultipleFreshnessValueStructureType* tempFreshnessConfig = NULL_PTR;
    uint16                                             salveSyncIndex;

    for (uint16 i = 0; i < CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM; i++)
    {
        tempFreshnessConfig = tempRxFreshnessConfig[i].freshnessConfigRef;
        salveSyncIndex      = tempRxFreshnessConfig[i].syncMsgIndex;
        if ((0xFFFFu == slaveSyncMsgIndex) || (salveSyncIndex == slaveSyncMsgIndex))
        {
            CDD_FVM_SetMemoryBytesToValue(
                FVM_RxMultipleTruncatedCntHandle[i].FVMFreshnessValue,
                0u,
                tempRxFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_SetMemoryBytesToValue(
                FVM_RxMultipleTruncatedCntHandle[i].FVMPreviousValue,
                0u,
                tempRxFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_SetMemoryBytesToValue(
                FVM_RxMultipleTruncatedCntHandle[i].FvmMsgCnt,
                0u,
                tempFreshnessConfig->msgCntUpper.CounterLen + tempFreshnessConfig->msgCntLower.CounterLen);

            /* Assign TripCnt to the constructed freshness value in the */
            CDD_FVM_CopyBufferToDstBits(
                FVM_RxMultipleTruncatedCntHandle[i].FVMPreviousValue,
                FVM_SlaveEcuHandle[salveSyncIndex].FvMTripCnt,
                tempFreshnessConfig->tirpCnt.CounterPos,
                tempFreshnessConfig->tirpCnt.CounterLen);
        }
    }
}

CDD_FVM_LOCAL Std_ReturnType CDD_FVM_RxResetFlagHandles(
    const CDD_FVM_SlaveECUSyncMsgType*                 SlaveEcuHandle,
    const CDD_FVM_MultipleFreshnessValueStructureType* multipleFeshnessConfig,
    sint8*                                             resetFlagSubtraction,
    const uint8*                                       truncatedFreshnessValue)
{
    Std_ReturnType result = E_OK;
    uint8          latestResetTag;
    uint8          receivedResetTag;
    uint8          tempTruncatedFreshnessLen =
        multipleFeshnessConfig->resetFlag.CounterLen + multipleFeshnessConfig->msgCntLower.CounterLen;

    /* Get reset flag for comparison */
    CDD_FVM_GetLowBitFromSrc(
        SlaveEcuHandle->FvMResetCnt,
        &latestResetTag,
        multipleFeshnessConfig->resetCnt.CounterLen,
        multipleFeshnessConfig->resetFlag.CounterLen);
    CDD_FVM_GetLowBitFromSrc(
        truncatedFreshnessValue,
        &receivedResetTag,
        tempTruncatedFreshnessLen,
        multipleFeshnessConfig->resetFlag.CounterLen);

    if (latestResetTag == receivedResetTag)
    {
        *resetFlagSubtraction = 0;
    }
    else if ((latestResetTag - 1u) == receivedResetTag)
    {
        *resetFlagSubtraction = -1;
    }
    else if ((latestResetTag + 1u) == receivedResetTag)
    {
        *resetFlagSubtraction = 1;
    }
    else if ((latestResetTag - 2u) == receivedResetTag)
    {
        *resetFlagSubtraction = -2;
    }
    else if ((latestResetTag + 2u) == receivedResetTag)
    {
        *resetFlagSubtraction = 2;
    }
    else
    {
        result = E_NOT_OK;
    }

    return result;
}

CDD_FVM_LOCAL void CDD_FVM_RxMsgLowerCounterHandle(
    uint16       freshnessIndex,
    const uint8* FVMTruncatedFreshnessValue,
    uint64*      latestLowerMsgCntValue,
    uint64*      previouslyLowerMsgCntValue)
{
    /* Handles the low message counter, received compared to the local placeholder memory */
    uint8                                   tempFVMTruncatedFreshnessValueLength;
    const CDD_FVM_MultipleFreshnessCntType* tempRxFreshnessCnt = &FVM_RxMultipleTruncatedCntHandle[freshnessIndex];
    const CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType* tempRxFreshnessConfig =
        &CDD_FVM_ConfigPtr->FvmRxMultipleFreshnessTruncateConfig[freshnessIndex];
    const CDD_FVM_MultipleFreshnessValueStructureType* tempFreshnessConfig = tempRxFreshnessConfig->freshnessConfigRef;
    uint8 tempMsgLowerCntLatestBuffer[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE]     = {0};
    uint8 tempMsgLowerCntPreviouslyBuffer[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE] = {0};
    tempFVMTruncatedFreshnessValueLength =
        tempFreshnessConfig->msgCntLower.CounterLen + tempFreshnessConfig->resetFlag.CounterLen;
    CDD_FVM_CopyBitsFromSourceToDestination(
        tempRxFreshnessCnt->FVMFreshnessValue,
        tempMsgLowerCntPreviouslyBuffer,
        tempFreshnessConfig->msgCntLower.CounterPos,
        tempFreshnessConfig->msgCntLower.CounterLen);

    uint8 remainingBits = CDD_FVM_MOD_SHIFT(tempFVMTruncatedFreshnessValueLength, CDD_FVM_CONST_8U);
    uint8 startBit      = (remainingBits > 0u) ? (CDD_FVM_CONST_8U - remainingBits) : (0u);
    CDD_FVM_CopyBitsFromSourceToDestination(
        FVMTruncatedFreshnessValue,
        tempMsgLowerCntLatestBuffer,
        startBit,
        tempFreshnessConfig->msgCntLower.CounterLen);
    *latestLowerMsgCntValue = CDD_FVM_ConvertBufferToValueWithOffset(
        tempMsgLowerCntLatestBuffer,
        0u,
        tempFreshnessConfig->msgCntLower.CounterLen);
    *previouslyLowerMsgCntValue = CDD_FVM_ConvertBufferToValueWithOffset(
        tempMsgLowerCntPreviouslyBuffer,
        0u,
        tempFreshnessConfig->msgCntLower.CounterLen);
}

CDD_FVM_LOCAL void CDD_FVM_RxSyncMessageHandle(
    uint16  freshnessIndex,
    uint8*  SyncMsgLatestBuffer,
    uint64* latestSyncMsgValue,
    uint64* previouslySyncMsgValue)
{
    uint8 tempSyncMsgPreviouslyBuffer[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE] = {0};
    const CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType* tempRxFreshnessConfig =
        &CDD_FVM_ConfigPtr->FvmRxMultipleFreshnessTruncateConfig[freshnessIndex];
    const CDD_FVM_MultipleFreshnessValueStructureType* tempFreshnessConfig = tempRxFreshnessConfig->freshnessConfigRef;
    uint16                                             slaveIndex          = tempRxFreshnessConfig->syncMsgIndex;
    const CDD_FVM_SlaveECUSyncMsgType*                 tempFvmSlaveEcuHandle = &FVM_SlaveEcuHandle[slaveIndex];
    const CDD_FVM_MultipleFreshnessCntType* tempRxFreshnessCnt = &FVM_RxMultipleTruncatedCntHandle[freshnessIndex];
    uint8 syncMsgLength = tempFreshnessConfig->tirpCnt.CounterLen + tempFreshnessConfig->resetCnt.CounterLen;
    /* Build the latest received synchronization message */
    CDD_FVM_CopyBufferToDstBits(
        SyncMsgLatestBuffer,
        tempFvmSlaveEcuHandle->FvMTripCnt,
        tempFreshnessConfig->tirpCnt.CounterPos,
        tempFreshnessConfig->tirpCnt.CounterLen);
    CDD_FVM_CopyBufferToDstBits(
        SyncMsgLatestBuffer,
        tempFvmSlaveEcuHandle->FvMResetCnt,
        tempFreshnessConfig->resetCnt.CounterPos,
        tempFreshnessConfig->resetCnt.CounterLen);

    CDD_FVM_CopySrcFromMbitToDstBits(
        tempRxFreshnessCnt->FVMFreshnessValue,
        tempSyncMsgPreviouslyBuffer,
        tempFreshnessConfig->tirpCnt.CounterPos,
        syncMsgLength);

    uint8 tempFirstBit            = 0u;
    uint8 tempAllSyncMsgAndOffset = tempFreshnessConfig->tirpCnt.CounterPos + syncMsgLength;

    if (0u != tempFreshnessConfig->tirpCnt.CounterPos)
    {
        tempFirstBit = (CDD_FVM_MOD_FROM_EIGHT(tempAllSyncMsgAndOffset) == 0u)
                           ? (0u)
                           : (CDD_FVM_CONST_8U - CDD_FVM_MOD_FROM_EIGHT(tempAllSyncMsgAndOffset));
    }

    *latestSyncMsgValue =
        CDD_FVM_ConvertBufferToValueWithOffset(SyncMsgLatestBuffer, tempFirstBit, tempAllSyncMsgAndOffset);
    *previouslySyncMsgValue =
        CDD_FVM_ConvertBufferToValueWithOffset(tempSyncMsgPreviouslyBuffer, tempFirstBit, tempAllSyncMsgAndOffset);
}

CDD_FVM_LOCAL void CDD_FVM_MultipleFreshnessBufferAddN(uint8* buffer, sint8 value, uint8 bitLength)
{
    uint32 tempValue = 0u;
    uint8  index;
    uint8  tempSyncMsgBytes = CDD_FVM_BIT_TO_BYTE(bitLength);
    uint32 tempNValue       = (uint32)value;
    for (index = 0; index < tempSyncMsgBytes; index++)
    {
        tempValue |= (uint32)buffer[index] << (CDD_FVM_CONST_8U * (tempSyncMsgBytes - index - 1u));
    }
    tempValue = tempValue + tempNValue;

    for (index = 0; index < tempSyncMsgBytes; index++)
    {
        buffer[index] = (uint8)(tempValue >> (CDD_FVM_CONST_8U * (tempSyncMsgBytes - index - 1u)));
    }
}

CDD_FVM_LOCAL Std_ReturnType CDD_FVM_RxMultipleCounterTruncatedHandle(
    uint16       freshnessIndex,
    const uint8* FVMTruncatedFreshnessValue,
    uint32       FVMTruncatedFreshnessValueLength,
    uint16       FVMAuthVerifyAttempts,
    uint8*       FVMFreshnessValue,
    uint32*      FVMFreshnessValueLength)
{
    Std_ReturnType result;
    uint8  tempSyncMsgLatestBuffer[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE] = {0};
    uint64 latestSyncMsgValue;
    uint64 previouslySyncMsgValue;
    uint64 latestLowerMsgCntValue;
    uint64 previouslyLowerMsgCntValue;
    const CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType* tempRxFreshnessConfig =
        &CDD_FVM_ConfigPtr->FvmRxMultipleFreshnessTruncateConfig[freshnessIndex];
    const CDD_FVM_MultipleFreshnessValueStructureType* tempFreshnessConfig = tempRxFreshnessConfig->freshnessConfigRef;
    uint16                                             syncMsgIndex        = tempRxFreshnessConfig->syncMsgIndex;
    CDD_FVM_MultipleFreshnessCntType*  tempRxFreshnessCnt = &FVM_RxMultipleTruncatedCntHandle[freshnessIndex];
    uint8                              syncMsgLength;
    uint8                              MsgCntLength;
    uint8                              tempTruncatedCounterLength;
    const CDD_FVM_SlaveECUSyncMsgType* tempFvmSlaveEcuHandle = &FVM_SlaveEcuHandle[syncMsgIndex];

    do
    {
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
        if ((NULL_PTR != CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[syncMsgIndex].FvMCtrlPduRef)
            && (CDD_FVM_CTRL_ERR == tempFvmSlaveEcuHandle->status))
        {
            result = E_NOT_OK;
        }
        else if (
            (NULL_PTR != CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[syncMsgIndex].FvMCtrlPduRef)
            && ((CDD_FVM_CTRL_INIT == tempFvmSlaveEcuHandle->status)
                || (CDD_FVM_CTRL_WAIT == tempFvmSlaveEcuHandle->status)))
        {
            if (FVMAuthVerifyAttempts > 0u)
            {
                result = E_BUSY;
            }
            else
            {
                result = E_NOT_OK;
            }
        }
        else
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
        {
            /* Using locally saved values to generate freshness values for SecOC */
            CDD_FVM_Memcpy(
                tempRxFreshnessCnt->FVMFreshnessValue,
                tempRxFreshnessCnt->FVMPreviousValue,
                tempRxFreshnessConfig->FvmFreshnessLength);
            syncMsgLength = tempFreshnessConfig->tirpCnt.CounterLen + tempFreshnessConfig->resetCnt.CounterLen;
            /* Construct the value of the data obtained by the low message */
            CDD_FVM_RxMsgLowerCounterHandle(
                freshnessIndex,
                FVMTruncatedFreshnessValue,
                &latestLowerMsgCntValue,
                &previouslyLowerMsgCntValue);
            /* Comparison processing from the received reset flag to the local temporarily stored reset flag */
            sint8 tempResetFlagCompareX = 0;
            result                      = CDD_FVM_RxResetFlagHandles(
                tempFvmSlaveEcuHandle,
                tempFreshnessConfig,
                &tempResetFlagCompareX,
                FVMTruncatedFreshnessValue);

            /* Processing of the latest synchronization messages with local staging of synchronization messages */
            CDD_FVM_RxSyncMessageHandle(
                freshnessIndex,
                tempSyncMsgLatestBuffer,
                &latestSyncMsgValue,
                &previouslySyncMsgValue);

            if (E_OK == result)
            {
                uint8 tempMsgUpperCntPreviouslyBuffer[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE] = {0};
                uint8 tempMsgCntPreviouslyBuffer[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE]      = {0};
                if ((latestSyncMsgValue + (uint64)tempResetFlagCompareX) == previouslySyncMsgValue)
                {
                    if (previouslyLowerMsgCntValue >= latestLowerMsgCntValue)
                    {
                        /* Format 2 : The high bit is incremented by 1 and the low
                         *              bit is replaced with the received value.
                         */
                        uint8 tempMsgCntLength =
                            tempFreshnessConfig->msgCntUpper.CounterLen + tempFreshnessConfig->msgCntLower.CounterLen;
                        CDD_FVM_CopyBitsFromSourceToDestination(
                            tempRxFreshnessCnt->FVMFreshnessValue,
                            tempMsgUpperCntPreviouslyBuffer,
                            tempFreshnessConfig->msgCntUpper.CounterPos,
                            tempFreshnessConfig->msgCntUpper.CounterLen);
                        /* The receiver ECU verifies the authenticator by overwriting the message counter with the
                         * maximum value. */

                        CDD_FVM_CopyBitsFromSourceToDestination(
                            tempRxFreshnessCnt->FVMFreshnessValue,
                            tempMsgCntPreviouslyBuffer,
                            tempFreshnessConfig->msgCntUpper.CounterPos,
                            tempMsgCntLength);
                        if (CDD_FVM_ConvertBufferToValueWithOffset(tempMsgCntPreviouslyBuffer, 0u, tempMsgCntLength)
                            != (CDD_FVM_GET_N_BITS_MAX_VALUE(tempMsgCntLength)))
                        {
                            CDD_FVM_BigEndianIncreaseMessageCounter(
                                tempMsgUpperCntPreviouslyBuffer,
                                tempFreshnessConfig->msgCntUpper.CounterLen);
                        }

                        CDD_FVM_CopyBufferToDstBits(
                            tempRxFreshnessCnt->FVMFreshnessValue,
                            tempMsgUpperCntPreviouslyBuffer,
                            tempFreshnessConfig->msgCntUpper.CounterPos,
                            tempFreshnessConfig->msgCntUpper.CounterLen);
                    }
                    /* Format 1 : Simply overwrite the original value with the received value */
                    CDD_FVM_CoverageOfLowNBits(
                        tempRxFreshnessCnt->FVMFreshnessValue,
                        FVMTruncatedFreshnessValue,
                        tempRxFreshnessConfig->FvmFreshnessLength,
                        (uint8)FVMTruncatedFreshnessValueLength);
                }
                else if ((latestSyncMsgValue + (uint64)tempResetFlagCompareX) > previouslySyncMsgValue)
                {
                    /* Format 3 : 1. replace the synchronization message with the freshness value using the latest
                     * synchronization message
                     * 2. set the high position of the message counter to 0.
                     * 3. overwrite the low position of the local freshness value with the received truncated data
                     */
                    uint8 tempResetCntBuffer[CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE] = {0};
                    CDD_FVM_Memcpy(
                        tempResetCntBuffer,
                        tempFvmSlaveEcuHandle->FvMResetCnt,
                        tempFreshnessConfig->resetCnt.CounterLen);

                    CDD_FVM_MultipleFreshnessBufferAddN(
                        tempResetCntBuffer,
                        tempResetFlagCompareX,
                        tempFreshnessConfig->resetCnt.CounterLen);

                    CDD_FVM_CopyBufferToDstBits(
                        tempSyncMsgLatestBuffer,
                        tempResetCntBuffer,
                        tempFreshnessConfig->resetCnt.CounterPos,
                        tempFreshnessConfig->resetCnt.CounterLen);

                    CDD_FVM_CopySrcFromMbitToDstBits(
                        tempSyncMsgLatestBuffer,
                        tempRxFreshnessCnt->FVMFreshnessValue,
                        tempFreshnessConfig->tirpCnt.CounterPos,
                        syncMsgLength);
                    CDD_FVM_CopyBufferToDstBits(
                        tempRxFreshnessCnt->FVMFreshnessValue,
                        tempMsgUpperCntPreviouslyBuffer,
                        tempFreshnessConfig->msgCntUpper.CounterPos,
                        tempFreshnessConfig->msgCntUpper.CounterLen);

                    CDD_FVM_CoverageOfLowNBits(
                        tempRxFreshnessCnt->FVMFreshnessValue,
                        FVMTruncatedFreshnessValue,
                        tempRxFreshnessConfig->FvmFreshnessLength,
                        (uint8)FVMTruncatedFreshnessValueLength);
                }
                else
                {
                    result = E_NOT_OK;
                }

                if (E_OK == result)
                {
                    CDD_FVM_Memcpy(
                        FVMFreshnessValue,
                        tempRxFreshnessCnt->FVMFreshnessValue,
                        tempRxFreshnessConfig->FvmFreshnessLength);
                    *FVMFreshnessValueLength = tempRxFreshnessConfig->FvmFreshnessLength;
                }
            }
        }
    } while (0);

    return result;
}

CDD_FVM_LOCAL void CDD_FVM_RxMultipleCounterIsSecOCPass(uint16 freshnessIndex)
{
    const CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType* tempRxFreshnessConfig =
        &CDD_FVM_ConfigPtr->FvmRxMultipleFreshnessTruncateConfig[freshnessIndex];
    CDD_FVM_MultipleFreshnessCntType* tempRxFreshnessCnt = &FVM_RxMultipleTruncatedCntHandle[freshnessIndex];
    CDD_FVM_Memcpy(
        tempRxFreshnessCnt->FVMPreviousValue,
        tempRxFreshnessCnt->FVMFreshnessValue,
        tempRxFreshnessConfig->FvmFreshnessLength);
}

#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON)

CDD_FVM_LOCAL void CDD_FVM_SlaveCopyMasterSyncMsg(uint16 masterIndex, uint16 slaveIndex)
{
    for (uint8 i = 0; i < CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE; i++)
    {
        FVM_SlaveEcuHandle[slaveIndex].FvMTripCnt[i] = FVM_MasterEcuHandle[masterIndex].FvMTripCnt[i];
    }

    for (uint8 i = 0; i < CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE; i++)
    {
        FVM_SlaveEcuHandle[slaveIndex].FvMResetCnt[i] = FVM_MasterEcuHandle[masterIndex].FvMResetCnt[i];
    }
}

#endif /* CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON */

CDD_FVM_LOCAL uint16 CDD_FVM_GetTxFreshnessConfigIndex(uint16 FVMFreshnessValueID, CDD_FVM_ModeType* mode)
{
    uint16 index;
    uint16 freshnessIndex = 0u;

    for (index = 0u; index < CDD_FVM_TX_FRESHNESS_CONFIG_NUM; index++)
    {
        if (CDD_FVM_ConfigPtr->FvmTxFreshnessConfig[index].FvmFreshnessId == FVMFreshnessValueID)
        {
            freshnessIndex = CDD_FVM_ConfigPtr->FvmTxFreshnessConfig[index].freshnessIndex;
            *mode          = CDD_FVM_ConfigPtr->FvmTxFreshnessConfig[index].mode;
            break;
        }
    }
    return freshnessIndex;
}

CDD_FVM_LOCAL uint16 CDD_FVM_GetRxFreshnessConfigIndex(uint16 FVMFreshnessValueID, CDD_FVM_ModeType* mode)
{
    uint16 index;
    uint16 freshnessIndex = 0u;

    for (index = 0u; index < CDD_FVM_RX_FRESHNESS_CONFIG_NUM; index++)
    {
        if (CDD_FVM_ConfigPtr->FvmRxFreshnessConfig[index].FvmFreshnessId == FVMFreshnessValueID)
        {
            freshnessIndex = CDD_FVM_ConfigPtr->FvmRxFreshnessConfig[index].freshnessIndex;
            *mode          = CDD_FVM_ConfigPtr->FvmRxFreshnessConfig[index].mode;
            break;
        }
    }
    return freshnessIndex;
}

CDD_FVM_LOCAL void CDD_FVM_Memcpy(uint8* dest, const uint8* src, uint8 dataNumOfBit)
{
    uint8*       destPtr = (uint8*)dest;
    const uint8* srcPtr  = (const uint8*)src;

    uint8 remainingBits = CDD_FVM_MOD_SHIFT(dataNumOfBit, CDD_FVM_CONST_8U);
    uint8 tempstartBit  = (remainingBits > 0u) ? (CDD_FVM_CONST_8U - remainingBits) : remainingBits;
    CDD_FVM_CopyBitsFromSourceToDestination(srcPtr, destPtr, tempstartBit, dataNumOfBit);
}

CDD_FVM_LOCAL void CDD_FVM_GetLowBitFromSrc(const uint8* src, uint8* dst, uint8 allBitLength, uint8 lowbitLength)
{
    const uint8* srcPtr        = (const uint8*)src;
    uint8*       destPtr       = (uint8*)dst;
    uint8        remainingBits = CDD_FVM_MOD_FROM_EIGHT(allBitLength);
    uint8 tempstartBit = (remainingBits > 0u) ? ((uint8)CDD_FVM_CONST_8U - remainingBits + allBitLength - lowbitLength)
                                              : (allBitLength - lowbitLength);
    CDD_FVM_CopyBitsFromSourceToDestination(srcPtr, destPtr, tempstartBit, lowbitLength);
}

CDD_FVM_LOCAL void CDD_FVM_CoverageOfLowNBits(uint8* dest, const uint8* src, uint8 srcBitLength, uint8 destBitLength)
{
    if ((srcBitLength >= destBitLength))
    {
        uint8 remainingBits = CDD_FVM_MOD_SHIFT(srcBitLength, CDD_FVM_CONST_8U);
        uint8 startBit = (remainingBits > 0u) ? ((uint8)CDD_FVM_CONST_8U - remainingBits + srcBitLength - destBitLength)
                                              : (srcBitLength - destBitLength);

        CDD_FVM_CopyBufferToDstBits(dest, src, startBit, destBitLength);
    }
}

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u) \
    || (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
CDD_FVM_LOCAL void CDD_FVM_CopySrcFromMbitToDstBits(const uint8* src, uint8* dst, uint8 firstNBit, uint8 bitLength)
{
    /* Calculate the byte index and bit index of the copy start location */
    uint8 startByteIndex = CDD_FVM_DIV_SHIFT(firstNBit, CDD_FVM_CONST_3U);
    uint8 startBitIndex  = CDD_FVM_CONST_8U - CDD_FVM_MOD_FROM_EIGHT(firstNBit);

    /* Calculate the number of bytes and bits in the copy */
    uint8 copyBytes     = CDD_FVM_DIV_SHIFT(bitLength - startBitIndex, CDD_FVM_CONST_3U);
    uint8 copyBits      = CDD_FVM_MOD_FROM_EIGHT(bitLength - startBitIndex);
    uint8 nextByteIndex = 0u;
    uint8 srcMask       = 0u;

    /* Step 1: Process the first byte */
    if ((startBitIndex > 0u) && (startBitIndex != CDD_FVM_CONST_8U))
    {
        srcMask = (1u << (startBitIndex)) - 1u;
        dst[startByteIndex] &= ~(srcMask);
        dst[startByteIndex] |= ((src[startByteIndex] & srcMask));
        startByteIndex += 1u;
    }

    /* Step 2: Handling intermediate full bytes */
    for (nextByteIndex = startByteIndex; nextByteIndex <= copyBytes + startByteIndex; nextByteIndex++)
    {
        dst[nextByteIndex] = src[nextByteIndex];
    }

    /* Step 3: Dispose of the remaining bytes */
    if (copyBits > 0u)
    {
        srcMask = (1u << copyBits) - 1u;
        dst[nextByteIndex] &= ~(srcMask << (CDD_FVM_CONST_8U - copyBits));
        srcMask = ~((1u << (CDD_FVM_CONST_8U - copyBits)) - 1u);
        dst[nextByteIndex] |= (src[nextByteIndex] & srcMask);
    }
}
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u OR \
          CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */
CDD_FVM_LOCAL
Std_ReturnType CDD_FVM_BitCompare(const uint8* ptr1, const uint8* ptr2, uint8 bitLength, uint16 maxDeltaCounter)
{
    Std_ReturnType result   = CDD_FVM_E_NOT_OK;
    const uint8*   bytePtr1 = (const uint8*)ptr1;
    const uint8*   bytePtr2 = (const uint8*)ptr2;

    uint64 ptr1Value = CDD_FVM_ConvertBufferToValueWithOffset(bytePtr1, 0u, bitLength);
    uint64 ptr2Value = CDD_FVM_ConvertBufferToValueWithOffset(bytePtr2, 0u, bitLength);
    uint64 maxValue  = CDD_FVM_GET_N_BITS_MAX_VALUE(bitLength);
    uint64 deltaCount;

    if (ptr2Value > maxValue)
    {
        result = CDD_FVM_E_NOT_OK;
    }
    else
    {
        deltaCount = (ptr2Value >= ptr1Value) ? (ptr2Value - ptr1Value) : (maxValue + ptr2Value - ptr1Value);
        if (maxDeltaCounter == 0u)
        {
            result = (ptr2Value >= ptr1Value) ? CDD_FVM_E_LESS_THAN : CDD_FVM_E_MORE_THAN;
        }
        else
        {
            result = (deltaCount == 0uLL)
                         ? CDD_FVM_E_EQUAL
                         : ((deltaCount == 1uLL) ? CDD_FVM_E_LESS_THAN
                                                 : ((deltaCount <= (uint64)maxDeltaCounter) ? CDD_FVM_E_LESS_THAN
                                                                                            : CDD_FVM_E_MORE_THAN));
        }
    }

    return result;
}

CDD_FVM_LOCAL Std_ReturnType
    CDD_FVM_JudgmentUpwardGrowth(const uint8* buff1, const uint8* buff2, uint8 nbit, uint16 DeltaCounter)
{
    Std_ReturnType ret = E_NOT_OK;

    /* Calculate the number of bytes that need to be set */
    /* buff1 is the local data, buff2 is the received data */

    Std_ReturnType compareRet = CDD_FVM_BitCompare(buff1, buff2, nbit, DeltaCounter);

    if ((compareRet == CDD_FVM_E_EQUAL) || (compareRet == CDD_FVM_E_LESS_THAN))
    {
        ret = E_OK;
    }

    return ret;
}

CDD_FVM_LOCAL void CDD_FVM_BigEndianIncreaseMessageCounter(uint8* freshnessCountBuff, uint8 freshnessBit)
{
    uint8 num_bytes      = CDD_FVM_BIT_TO_BYTE(freshnessBit); /* Calculate The Number Of Bytes Required */
    uint8 remaining_bits = CDD_FVM_MOD_SHIFT(freshnessBit, CDD_FVM_CONST_8U);
    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
    uint8 mask = (1u << (remaining_bits)) - 1u;
    /* PRQA S 4397 -- */

    for (uint8 byteIndex = num_bytes; byteIndex > 0u; byteIndex--)
    {
        freshnessCountBuff[byteIndex - 1u]++;
        if (freshnessCountBuff[byteIndex - 1u] != 0u)
        {
            break;
        }
    }

    if (0u != remaining_bits)
    {
        if (freshnessCountBuff[0u] > mask)
        {
            freshnessCountBuff[0u] -= 1u;
            /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
            freshnessCountBuff[0u] &= ~mask;
            /* PRQA S 4397 -- */
        }
    }
}

CDD_FVM_LOCAL void CDD_FVM_CopyBufferToDstBits(uint8* dst, const uint8* src, uint8 firstNBit, uint8 bitLength)
{
    /* Calculate the byte index and bit index of the copy start location */
    uint8 startByteIndex = CDD_FVM_DIV_SHIFT(firstNBit, CDD_FVM_CONST_3U);
    uint8 startBitIndex  = CDD_FVM_MOD_FROM_EIGHT(firstNBit);

    /* Calculating the number of bytes and bits in a copy */
    uint8 copyBytes           = CDD_FVM_DIV_SHIFT(bitLength, CDD_FVM_CONST_3U);
    uint8 copyBits            = CDD_FVM_MOD_FROM_EIGHT(bitLength);
    uint8 startRemainBitIndex = CDD_FVM_CONST_8U - startBitIndex;
    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
    uint8 srcMask = (1u << startRemainBitIndex) - 1u;
    /* PRQA S 4397 -- */
    uint8 maskReminbits;

    if (copyBits < startRemainBitIndex)
    {
        maskReminbits = startRemainBitIndex - copyBits;
        /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
        uint8 remainMask = (1u << maskReminbits) - 1u;
        /* PRQA S 4397 -- */
        uint8 tempindex;
        if (copyBits > 0u)
        {
            /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
            srcMask = (1u << copyBits) - 1u;
            dst[startByteIndex] &= ~(srcMask << maskReminbits);
            dst[startByteIndex] |= ((src[0u] & srcMask) << maskReminbits);
            /* PRQA S 4397 -- */
            for (uint8 i = 0u; i < copyBytes; i++)
            {
                tempindex = startByteIndex + i;
                /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
                dst[tempindex] &= (~remainMask);
                /* PRQA S 4397 -- */
                dst[tempindex] |= (src[i + 1u] >> (CDD_FVM_CONST_8U - maskReminbits));
                tempindex += 1u;
                dst[tempindex] &= remainMask;
                /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
                dst[tempindex] |= (src[i + 1u] << maskReminbits);
                /* PRQA S 4397 -- */
            }
        }
        else
        {
            for (uint8 i = 0u; i < copyBytes; i++)
            {
                tempindex = startByteIndex + i;
                /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
                dst[tempindex] &= (~remainMask);
                /* PRQA S 4397 -- */
                dst[tempindex] |= (src[i] >> (CDD_FVM_CONST_8U - maskReminbits));
                tempindex += 1u;
                dst[tempindex] &= remainMask;
                /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
                dst[tempindex] |= (src[i] << maskReminbits);
                /* PRQA S 4397 -- */
            }
        }
    }
    else
    {
        maskReminbits = copyBits - startRemainBitIndex;
        uint8 dstMask = CDD_FVM_CONST_8U - maskReminbits;
        /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
        dst[startByteIndex] &= (~srcMask);
        /* PRQA S 4397 -- */
        dst[startByteIndex] |= ((src[0u] >> maskReminbits) & srcMask);

        if (maskReminbits == 0u)
        {
            for (uint8 i = 1u; i <= copyBytes; i++)
            {
                dst[startByteIndex + i] = src[i];
            }
        }
        else
        {
            uint8 tempindex;
            /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
            srcMask = (1u << dstMask) - 1u;
            /* PRQA S 4397 -- */
            dst[startByteIndex + 1u] &= (srcMask);
            /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
            dst[startByteIndex + 1u] |= (src[0u] << dstMask);
            /* PRQA S 4397 -- */

            for (uint8 i = 1u; i <= copyBytes; i++)
            {
                tempindex = startByteIndex + i;
                /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
                dst[tempindex] &= (~srcMask);
                /* PRQA S 4397 -- */
                dst[tempindex] |= (src[i] >> maskReminbits);
                tempindex += 1u;
                dst[tempindex] &= (srcMask);
                /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
                dst[tempindex] |= (src[i] << dstMask);
                /* PRQA S 4397 -- */
            }
        }
    }
}

CDD_FVM_LOCAL uint64 CDD_FVM_ConvertBufferToValueWithOffset(const uint8* src, uint8 firstBit, uint8 bitlength)
{
    uint64 retValue   = 0u;
    uint8  byteLength = CDD_FVM_BIT_TO_BYTE(bitlength);

    if (bitlength <= CDD_FVM_CONST_64U)
    {
        for (uint8 i = 0u; i < byteLength; i++)
        {
            retValue |= (uint64)src[i] << (CDD_FVM_CONST_8U * (byteLength - i - 1u));
        }
        retValue = (uint64)(retValue >> firstBit);
    }

    return retValue;
}

CDD_FVM_LOCAL void
    CDD_FVM_CopyBitsFromSourceToDestination(const uint8* src, uint8* dst, uint8 srcIndex, uint8 bitlength)
{
    uint8 startByteIndex      = CDD_FVM_DIV_SHIFT(srcIndex, CDD_FVM_CONST_3U);
    uint8 startBitIndex       = CDD_FVM_MOD_FROM_EIGHT(srcIndex);
    uint8 copyBytes           = CDD_FVM_DIV_SHIFT(bitlength, CDD_FVM_CONST_3U);
    uint8 copyBits            = CDD_FVM_MOD_FROM_EIGHT(bitlength);
    uint8 startRemainBitIndex = CDD_FVM_CONST_8U - startBitIndex;
    uint8 maskReminbits;
    uint8 tempMaskHighBits;
    uint8 tempindex;

    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
    uint8 mask = (1u << copyBits) - 1u;
    /* PRQA S 4397 -- */

    if (startRemainBitIndex > copyBits)
    {
        maskReminbits    = startRemainBitIndex - copyBits;
        tempMaskHighBits = CDD_FVM_CONST_8U - maskReminbits;
        if (copyBits != 0u)
        {
            dst[0u] = (src[startByteIndex] >> (maskReminbits)) & mask;
        }

        for (uint8 i = 0u; i < copyBytes; i++)
        {
            tempindex = startByteIndex + i;
            if (copyBits != 0u)
            {
                /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
                dst[i + 1u] = (src[tempindex] << tempMaskHighBits) | (src[tempindex + 1u] >> (maskReminbits));
                /* PRQA S 4397 -- */
            }
            else
            {
                if (tempMaskHighBits == 0u)
                {
                    dst[i] = src[tempindex];
                }
                else
                {
                    /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
                    dst[i] = (src[tempindex] << tempMaskHighBits) | (src[tempindex + 1u] >> (maskReminbits));
                    /* PRQA S 4397 -- */
                }
            }
        }
    }
    else
    {
        maskReminbits    = copyBits - startRemainBitIndex;
        tempMaskHighBits = CDD_FVM_CONST_8U - maskReminbits;
        /* Process all data less than one byte */
        /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
        dst[0u] = (src[startByteIndex] << (maskReminbits)) & mask;
        /* PRQA S 4397 -- */
        if (maskReminbits > 0u)
        {
            /* PRQA S 4397, 2985 ++ */ /* VL_CDD_FVM_4397, VL_CDD_FVM_2985 */
            dst[0u] |= ((src[startByteIndex + 1u]) >> tempMaskHighBits) & ((1u << maskReminbits) - 1u);
            /* PRQA S 4397, 2985 -- */
        }

        for (uint8 i = 1u; i <= copyBytes; i++)
        {
            tempindex = startByteIndex + i;
            /* PRQA S 4397 ++ */ /* VL_CDD_FVM_4397 */
            dst[i] = (src[tempindex] << maskReminbits) | (src[tempindex + 1u] >> tempMaskHighBits);
            /* PRQA S 4397 -- */
        }
    }
}

CDD_FVM_LOCAL void CDD_FVM_SetMemoryBytesToValue(uint8* dst, uint8 Value, uint8 bitLength)
{
    uint8 writeBytes = CDD_FVM_BIT_TO_BYTE(bitLength);

    for (uint8 i = 0u; i < writeBytes; i++)
    {
        dst[i] = Value;
    }
}

#define CDD_FVM_STOP_SEC_CODE
#include "CDD_FVM_MemMap.h"
