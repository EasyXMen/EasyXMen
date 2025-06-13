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
**  FILENAME    : CDD_FVM.c                                                   **
**                                                                            **
**  Created on  : 2023-10-20                                                  **
**  Author      : Jian.Jiang                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2023-10-20  Jian.Jiang      R19-11 FVM initial version.
 *  V1.0.1       2023-12-15  Jian.Jiang      Modify the receiving mechanism to require its upward growth and add frame
 * loss check
 *  V1.0.2       2024-04-22  Jian.Jiang      Solve the problem that FVM multicounters can be changed only after secoc
 * changes, as well as the problem of NVM usage
 *  V1.0.3       2024-06-11  Jian.Jiang      Solve the problem that FVM sends and receives when TripConter and
 * ResetCounter are not full bytes, resulting in a shift error.
 *  V1.0.4       2024-06-14  Jian.Jiang      Solving the problem of reading and writing NVM exception handling for
 * master and slave machines
 *  V1.0.5       2024-07-11   Jian.Jiang     Add judgement PDUR to send return value
 *  V1.0.6       2024-08-06   Jian.Jiang     Solve the problem that the first frame master is delayed due to failure to
 * send
 */
/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:FVM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

 */

#define CDD_FVM_C_AR_MAJOR_VERSION 4u
#define CDD_FVM_C_AR_MINOR_VERSION 5u
#define CDD_FVM_C_AR_PATCH_VERSION 0u
#define CDD_FVM_C_SW_MAJOR_VERSION 1u
#define CDD_FVM_C_SW_MINOR_VERSION 0u
#define CDD_FVM_C_SW_PATCH_VERSION 6u

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CDD_FVM.h"
#include "CDD_FVM_Internal.h"

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
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CDD_FVM_LOCAL static

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define CDD_FVM_START_SEC_CODE
#include "CDD_FVM_MemMap.h"

#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_TxSingleCounterInit(P2CONST(CDD_FVM_TxSingleFreshnessCounterConfigType, AUTOMATIC, CDD_FVM_APPL_CONST)
                                    TxFreshnessSingleConfig);
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_GetTxSingleCounterFreshnessValue(
    uint16 freshnessIndex,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) freshnessValue,
    uint8 FreshnessLenfth);
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_TxUpdateSingleCounterFreshnessValue(uint16 freshnessIndex);
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_RxSingleCounterInit(P2CONST(CDD_FVM_RxSingleFreshnessCounterConfigType, AUTOMATIC, CDD_FVM_APPL_CONST)
                                    RxFreshnessSingleConfig);

static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_RxUpdateSingleCounterFreshnessValue(
    uint16 freshnessIndex,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) freshnessValue);

static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_GetRxSingleCounterFreshnessValue(
    uint16 freshnessIndex,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) freshnessValue);

static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_RxSingleCounterFreshnessHandle(
    uint16 freshnessIndex,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    uint32 FVMTruncatedFreshnessValueLength,
    uint16 FVMAuthVerifyAttempts,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
);

static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_JudgmentUpwardGrowth(
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) buff1,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) buff2,
    uint8 nbit,
    uint16 DeltaCounter);

#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON)
static FUNC(void, CDD_FVM_CODE) CDD_FVM_SlaveCopyMasterSyncMsg(uint16 masterIndex, uint16 slaveIndex);

#endif /* CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON */

#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
static FUNC(void, CDD_FVM_CODE) CDD_FVM_MasterEcuInit(P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config);

#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
static FUNC(uint8, CDD_FVM_CODE) CDD_FVM_FindMasterCtrlSlaveIdleData(uint16 index, uint16 syncMsgId);
static FUNC(uint8, CDD_FVM_CODE) CDD_FVM_FindMasterCtrlCurrentData(uint16 index, uint16 syncMsgId);
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_MasterRxCtrlPduFrameHandle(uint16 index, P2CONST(PduInfoType, AUTOMATIC, CDD_FVM_APPL_CONST) PduInfoPtr);
#endif /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
static FUNC(void, CDD_FVM_CODE) CDD_FVM_SlaveEcuInit(P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config);
static FUNC(void, CDD_FVM_CODE) CDD_FVM_SlaveRxSyncMessagePduFrameHandle(
    uint16 index,
    P2CONST(PduInfoType, AUTOMATIC, CDD_FVM_APPL_CONST) PduInfoPtr);
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_SlaveTripCounterMaxValueHandle(
    uint16 index,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) rxtripCntRef,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) localtripCntRef);
#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_SlaveRxCtrlPduFrameHandle(uint16 index, P2CONST(PduInfoType, AUTOMATIC, CDD_FVM_APPL_CONST) PduInfoPtr);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_SLaveSendAskMessageToMaster(uint16 index);
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
static FUNC(void, CDD_FVM_CODE) CDD_FVM_TxMultipleTruncatedCounterInit(
    P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config,
    uint16 slaveSyncMsgIndex);

static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_GetTxMultipleTruncatedCounterFreshnessValue(
    uint16 freshnessIndex,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) freshnessValue,
    uint8 FreshnessLength,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMTruncatedFreshnessValueLength);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_MultipleFreshnessBufferAddN(
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) buffer,
    sint8 value,
    uint8 bitLength);

#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
static FUNC(void, CDD_FVM_CODE) CDD_FVM_RxMultipleTruncatedCounterInit(
    P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config,
    uint16 slaveSyncMsgIndex);

static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_RxMultipleCounterTruncatedHandle(
    uint16 freshnessIndex,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    uint32 FVMTruncatedFreshnessValueLength,
    uint16 FVMAuthVerifyAttempts,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
);

static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_RxResetFlagHandles(
    P2CONST(CDD_FVM_SlaveECUSyncMsgType, AUTOMATIC, CDD_FVM_APPL_CONST) SlaveEcuHandle,
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST) multipleFeshnessConfig,
    P2VAR(sint8, AUTOMATIC, CDD_FVM_APPL_DATA) resetFlagSubtraction,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) truncatedFreshnessValue);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_RxMsgLowerCntHandle(
    uint16 freshnessIndex,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    P2VAR(uint64, AUTOMATIC, CDD_FVM_APPL_DATA) latestLowerMsgCntValue,
    P2VAR(uint64, AUTOMATIC, CDD_FVM_APPL_DATA) previouslyLowerMsgCntValue);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_RxSyncMessageHandle(
    uint16 freshnessIndex,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) SyncMsgLatestBuffer,
    P2VAR(uint64, AUTOMATIC, CDD_FVM_APPL_DATA) latestSyncMsgValue,
    P2VAR(uint64, AUTOMATIC, CDD_FVM_APPL_DATA) previouslySyncMsgValue);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_RxMultipleCounterIsSecOCPass(uint16 freshnessIndex);
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

static FUNC(void, CDD_FVM_CODE) CDD_FVM_Memcpy(
    P2VAR(void, AUTOMATIC, CDD_FVM_APPL_DATA) dest,
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    uint8 dataNumOfBit);

static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_BitCompare(
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) ptr1,
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) ptr2,
    uint8 bitLength,
    uint16 maxDeltaCounter);

static FUNC(uint16, CDD_FVM_CODE) CDD_FVM_GetTxFreshnessConfigIndex(
    uint16 FVMFreshnessValueID,
    P2VAR(CDD_FVM_ModeType, AUTOMATIC, CDD_FVM_APPL_DATA) mode);

static FUNC(uint16, CDD_FVM_CODE) CDD_FVM_GetRxFreshnessConfigIndex(
    uint16 FVMFreshnessValueID,
    P2VAR(CDD_FVM_ModeType, AUTOMATIC, CDD_FVM_APPL_DATA) mode);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_CoverageOfLowNBits(
    P2VAR(void, AUTOMATIC, CDD_FVM_APPL_DATA) dest,
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    uint8 destBitLength,
    uint8 srcBitLength);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_CopySrcFromMbitToDstBits(
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) dst,
    uint8 firstNBit,
    uint8 bitLength);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_GetLowBitFromSrc(
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    P2VAR(void, AUTOMATIC, CDD_FVM_APPL_DATA) dst,
    uint8 allBitLength,
    uint8 lowbitLength);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_BigEndianIncreaseMessageCounter(
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) freshnessCountBuff,
    uint8 freshnessBit);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_CopyBufferToDstBits(
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) dst,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    uint8 firstNBit,
    uint8 bitLength);

static FUNC(uint64, CDD_FVM_CODE)
    CDD_FVM_BufferToValue(P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) src, uint8 firstBit, uint8 bitlength);

static FUNC(void, CDD_FVM_CODE) CDD_FVM_GetNbitBufferFromSrc(
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) dst,
    uint8 srcIndex,
    uint8 bitlength);

static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_MemsetOfBit(P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) dst, uint8 Value, uint8 bitLength);

#define CDD_FVM_STOP_SEC_CODE
#include "CDD_FVM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define CDD_FVM_START_SEC_VAR_INIT_UNSPECIFIED
#include "CDD_FVM_MemMap.h"
/* Variable store FVM initialization status */
static VAR(CDD_FVM_StateType, CDD_FVM_VAR_POWER_ON_INIT) CDD_FVM_InitStatus = CDD_FVM_UNINIT;
#define CDD_FVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CDD_FVM_MemMap.h"

#define CDD_FVM_START_SEC_VAR_INIT_PTR
#include "CDD_FVM_MemMap.h"
/* Pointer to configuration data */
static P2CONST(CDD_FVM_ConfigType, CDD_FVM__VAR, CDD_FVM__CONST) CDD_FVM_ConfigPtr = NULL_PTR;
#define CDD_FVM_STOP_SEC_VAR_INIT_PTR
#include "CDD_FVM_MemMap.h"

#define CDD_FVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CDD_FVM_MemMap.h"

#if (CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
static CDD_FVM_TxFreshnessCntType FVM_TxSingleCntHandle[CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM];
#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
static CDD_FVM_RxFreshnessCntType FVM_RxSingleCntHandle[CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM];
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
static CDD_FVM_MultipleFreshnessCntType
    FVM_TxMultipleTruncatedCntHandle[CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM];
#endif /* CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)
static CDD_FVM_MultipleFreshnessCntType
    FVM_RxMultipleTruncatedCntHandle[CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM];
#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
static CDD_FVM_MasterECUSyncMsgType FVM_MasterEcuHandle[CDD_FVM_MASTER_ECU_CONFIG_NUM];
#endif /* CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u */

#if (CDD_FVM_SLAVE_CONFIG_NUM > 0u)
static CDD_FVM_SlaveECUSyncMsgType FVM_SlaveEcuHandle[CDD_FVM_SLAVE_CONFIG_NUM];
#endif /* CDD_FVM_SLAVE_CONFIG_NUM > 0U */

#define CDD_FVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CDD_FVM_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CDD_FVM_START_SEC_CODE
#include "CDD_FVM_MemMap.h"

/******************************************************************************/
/*
 * Brief               Initializes the the FVM module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      config: Pointer to a selected configuration structure
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, CDD_FVM_CODE) CDD_FVM_Init(P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config)
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
        CDD_FVM_TxSingleCounterInit(CDD_FVM_ConfigPtr->FvmTxSingleFreshnessConfig);
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

        CDD_FVM_InitStatus = CDD_FVM_INIT;
    }
}

/******************************************************************************/
/*
 * Brief               This service stops the fvm.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, CDD_FVM_CODE) CDD_FVM_DeInit(void)
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
            CDD_FVM_MemsetOfBit(
                FVM_TxSingleCntHandle[i].FVMmsgCnt,
                0u,
                CDD_FVM_ConfigPtr->FvmTxSingleFreshnessConfig[i].FvmFreshnessLength);
        }

#endif /* CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u)
        for (uint16 i = 0u; i < CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM; i++)
        {
            CDD_FVM_MemsetOfBit(
                FVM_RxSingleCntHandle[i].FVMmsgCnt,
                0u,
                CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_MemsetOfBit(
                FVM_RxSingleCntHandle[i].FVMmsgRxFreshnessValue,
                0u,
                CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[i].FvmFreshnessLength);
        }
#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

        CDD_FVM_InitStatus = CDD_FVM_UNINIT;
    }
}

#if (CDD_FVM_VERSIONINFO_API == STD_ON)
/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, CDD_FVM_CODE)
CDD_FVM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CDD_FVM_APPL_DATA) versioninfo)
{
#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (versioninfo == NULL_PTR)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_GETVERSIONINFO, CDD_FVM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
        versioninfo->vendorID = CDD_FVM_VENDOR_ID;
        versioninfo->moduleID = CDD_FVM_MODULE_ID;
        versioninfo->sw_major_version = CDD_FVM_C_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CDD_FVM_C_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CDD_FVM_C_SW_PATCH_VERSION;
    }
}
#endif /* CDD_FVM_VERSIONINFO_API == STD_ON */

/******************************************************************************/
/*
 * Brief               This interface is used by the FVM to obtain the current
 *                     freshness value.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      FVMFreshnessValueID: Holds the identifier of the
 *                                            freshness value.
 *                     FVMTruncatedFreshnessValue: Holds the truncated
 *                                  freshness value that was contained in the
 *                                  Secured I-PDU.
 *                     FVMTruncatedFreshnessValueLength: Holds the length in
 *                                  bits of the truncated freshness value.
 *                     FVMAuthVerifyAttempts:Hold the number of authentication
 *                                  verify attempts of this PDU since the last
 *                                  reception.
 * Param-Name[out]     FVMFreshnessValue: Holds the freshness value to be used
 *                                  for the calculation of the authenticator.
 * Param-Name[in/out]  FVMFreshnessValueLength: Holds the length in bits of
 *                                                the freshness value.
 * Return              E_OK: request successful.
 *                     E_NOT_OK: request failed, a freshness value cannot be
 *                               provided due to general issues for freshness orx
 *                               this FreshnessValueId.
 *                     E_BUSY: The freshness information can temporarily not be
 *                             provided.
 */
/******************************************************************************/
FUNC(Std_ReturnType, CDD_FVM_CODE)
CDD_FVM_GetRxFreshness(
    uint16 FVMFreshnessValueID,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    uint32 FVMTruncatedFreshnessValueLength,
    uint16 FVMAuthVerifyAttempts,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
)
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16 index;
    Std_ReturnType result = E_NOT_OK;
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

/******************************************************************************/
/*
 * Brief               This interface is used by the FVM to obtain the current
 *                     freshness value.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      FVMFreshnessValueID: Holds the identifier of the
 *                                            freshness value.
 *                     FVMTruncatedFreshnessValue: Holds the truncated
 *                                  freshness value that was contained in the
 *                                  Secured I-PDU.
 *                     FVMTruncatedFreshnessValueLength: Holds the length in
 *                                  bits of the truncated freshness value.
 *                     FVMAuthDataFreshnessValue: The parameter holds a part
 *                                  of the received, not yet authenticated PDU.
 *                     FVMAuthDataFreshnessValueLength: This is the length
 *                                  value in bits that holds the freshness from
 *                                  the authentic PDU.
 *                     FVMAuthVerifyAttempts: Holds the number of
 *                                  authentication verify attempts of this PDU
 *                                  since the last reception.
 * Param-Name[out]     FVMFreshnessValue: Holds the freshness value to be used
 *                                  for the calculation of the authenticator.
 * Param-Name[in/out]  FVMFreshnessValueLength: Holds the length in bits of
 *                                                the freshness value.
 * Return              E_OK: request successful.
 *                     E_NOT_OK: request failed, a freshness value cannot be
 *                               provided due to general issues for freshness orx
 *                               this FreshnessValueId.
 *                     E_BUSY: The freshness information can temporarily not be
 *                             provided.
 */
/******************************************************************************/
FUNC(Std_ReturnType, CDD_FVM_CODE)
CDD_FVM_GetRxFreshnessAuthData(
    uint16 FVMFreshnessValueID,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    uint32 FVMTruncatedFreshnessValueLength,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMAuthDataFreshnessValue,
    uint16 FVMAuthDataFreshnessValueLength,
    uint16 FVMAuthVerifyAttempts,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
)
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16 index;
    Std_ReturnType result = E_NOT_OK;
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
    return result;
}

/******************************************************************************/
/*
 * Brief               This API returns the freshness value from the Most
 *                     Significant Bits in the first byte in the array
 *                     (FVMFreshnessValue), in big endian format.
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      FVMFreshnessValueID: Holds the identifier of the
 *                                            freshness value.
 * Param-Name[out]     FVMFreshnessValue: Holds the freshness value to be used
 *                                     for the calculation of the authenticator.
 * Param-Name[in/out]  FVMFreshnessValueLength: Holds the length in bits of
 *                                                the freshness value.
 * Return              E_OK: request successful.
 *                     E_NOT_OK: request failed, a freshness value cannot be
 *                               provided due to general issues for freshness orx
 *                               this FreshnessValueId.
 *                     E_BUSY: The freshness information can temporarily not be
 *                             provided.
 */
/******************************************************************************/
FUNC(Std_ReturnType, CDD_FVM_CODE)
CDD_FVM_GetTxFreshness(
    uint16 FVMFreshnessValueID,
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
)
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16 index;
    Std_ReturnType result = E_NOT_OK;
    uint8 tempLength;
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
            result = CDD_FVM_GetTxSingleCounterFreshnessValue(index, FVMFreshnessValue, tempLength);
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

/******************************************************************************/
/*
 * Brief               This interface is used by the FVM to obtain the current
 *                     freshness value. The interface function provides also the
 *                     truncated freshness transmitted in the secured I-PDU.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      FVMFreshnessValueID: Holds the identifier of the
 *                                            freshness value.
 * Param-Name[out]     FVMFreshnessValue: Holds the current freshness value.
 *                     FVMTruncatedFreshnessValue: Holds the truncated
 *                              freshness to be included into the Secured I-PDU.
 * Param-Name[in/out]  FVMFreshnessValueLength: Holds the length in bits of
 *                                                the freshness value.
 *                     FVMTruncatedFreshnessValueLength: Provides the
 *                              truncated freshness length configured for this
 *                              freshness.
 * Return              E_OK: request successful.
 *                     E_NOT_OK: request failed, a freshness value cannot be
 *                               provided due to general issues for freshness orx
 *                               this FreshnessValueId.
 *                     E_BUSY: The freshness information can temporarily not be
 *                             provided.
 */
/******************************************************************************/
FUNC(Std_ReturnType, CDD_FVM_CODE)
CDD_FVM_GetTxFreshnessTruncData(
    uint16 FVMFreshnessValueID,
    /* PRQA S 3432 ++*/ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMTruncatedFreshnessValueLength
    /* PRQA S 3432 --*/ /* MISRA Rule 20.7 */
)
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16 index;
    Std_ReturnType result = E_NOT_OK;
    uint8 tempLength;
    uint8 tempallLength;
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
            result = CDD_FVM_GetTxMultipleTruncatedCounterFreshnessValue(
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

/******************************************************************************/
/*
 * Brief               This interface is used by the FVM to indicate that the
 *                     Secured I-PDU has been initiated for transmission.
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      FVMFreshnessValueID: Holds the identifier of the
 *                                            freshness value.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_SPduTxConfirmation(uint16 FVMFreshnessValueID)
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16 index;
    Std_ReturnType result = E_NOT_OK;

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

/******************************************************************************/
/*
 * Description         This function is used to notify the reception of SecOC verification status.
 * ServiceId           0x0B
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      verificationStatus: SecOC verification status to notify.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, CDD_FVM_CODE) CDD_FVM_RxSecOCVerificationNotify(SecOC_VerificationStatusType verificationStatus)
{
    CDD_FVM_ModeType mode = CDD_FVM_MODE_UNDEFINED;
    uint16 index;

#if (STD_ON == CDD_FVM_DEV_ERROR_DETECT)
    if (CDD_FVM_UNINIT == CDD_FVM_InitStatus)
    {
        CDD_FVM_DET_REPORTERROR(CDD_FVM_RXSECOCVERIFICATIONNOTIFY, CDD_FVM_E_UNINIT);
    }
    else
#endif /* STD_ON == CDD_FVM_DEV_ERROR_DETECT */
    {
        index = CDD_FVM_GetRxFreshnessConfigIndex(verificationStatus.freshnessValueID, &mode);
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
    }
}

/******************************************************************************/
/*
 * Description         This function is the Rx indication callback function for the CDD FVM module.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      RxPduId: ID of the received PDU.
 * Param-Name[in]      PduInfoPtr: Pointer to the received PDU data.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, CDD_FVM_CODE)
CDD_FVM_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, CDD_FVM_APPL_CONST) PduInfoPtr)
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
        P2CONST(CDD_FVM_MasterConfigType, AUTOMATIC, CDD_FVM_CONST)
        tempFvmMasterConfig = CDD_FVM_ConfigPtr->FvmMasterEcuConfig;
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
        P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST)
        tempFvmSlaveConfig = CDD_FVM_ConfigPtr->FvmSlaveEcuConfig;
        boolean rxPduTag = FALSE;
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
    }
}

#if (CDD_FVM_MASTER_ECU_ENABLED == STD_ON) && (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
/******************************************************************************/
/*
 * Description         This function is the main function for the master ECU of the CDD FVM module.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
FUNC(void, CDD_FVM_CODE) CDD_FVM_MainFunctionMaster(void)
{
    uint8 tempSyncMsgLength;
    PduInfoType fvmpduInfo;
    PduIdType pduId;
    P2CONST(CDD_FVM_MasterConfigType, AUTOMATIC, CDD_FVM_CONST) tempMasterEcuConfig = NULL_PTR;
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
                        uint64 localResetCntValue = CDD_FVM_BufferToValue(
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
                    fvmpduInfo.SduLength = (PduLengthType)CDD_FVM_BIT_TO_BYTE(tempSyncMsgLength);
                    fvmpduInfo.SduDataPtr = FVM_MasterEcuHandle[i].FvMSynMsgBuff;
                    pduId = tempMasterEcuConfig->fvmSyncMsgPduRef->TxPduHandleId;

                    if (E_OK == PduR_CDD_FVMTransmit(pduId, &fvmpduInfo))
                    {
                        FVM_MasterEcuHandle[i].status = CDD_FVM_CTRL_WAIT;
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
        P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST)
        tempFvmSlaveConfig = CDD_FVM_ConfigPtr->FvmSlaveEcuConfig;
        for (uint16 i = 0; i < CDD_FVM_SLAVE_CONFIG_NUM; i++)
        {
            uint16 masterIndex = tempFvmSlaveConfig[i].FvMCommonMasterIndex;
            if (0xFFFFu != masterIndex)
            {
                if (CDD_FVM_CTRL_WAIT == FVM_MasterEcuHandle[masterIndex].status)
                {
                    CDD_FVM_SlaveCopyMasterSyncMsg(masterIndex, i);
                    FVM_SlaveEcuHandle[i].status = CDD_FVM_CTRL_OK;
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
/******************************************************************************/
/*
 * Description         This function is used to request a synchronization message by the slave.
 * ServiceId           0X0A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      syncMsgId: ID of the synchronization message.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType: Standard return type indicating the success or failure of the function.
 */
/******************************************************************************/
FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_SlaveRequestSyncMsg(uint16 syncMsgId)
{
    P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST) tempSlaveEcuConfig = NULL_PTR;
    Std_ReturnType ret = E_NOT_OK;
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
                P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) tempCtrlBufferRef = FVM_SlaveEcuHandle[index].FvmCtrlBuffer;
                PduInfoType pduInfo;
                PduIdType pduId;

                /* Constructing an acknowledgement message to be sent */
                tempCtrlBufferRef[0u] = CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION << 6u;
                tempCtrlBufferRef[0u] |= CDD_FVM_CTRL_MSG_TYPE_SLAVE_REQUSEST(tempCtrlBufferRef[0u]);
                tempCtrlBufferRef[1u] = CDD_FVM_MULTIPLE_INSTANCE_ID;
                tempCtrlBufferRef[2u] = (uint8)(tempSlaveEcuConfig->FvMSyncMsgId >> 8u);
                tempCtrlBufferRef[3u] = (uint8)(tempSlaveEcuConfig->FvMSyncMsgId & 0xFFu);
                tempCtrlBufferRef[4u] = 0u;
                tempCtrlBufferRef[5u] = 0u;
                tempCtrlBufferRef[6u] = 0u;
                tempCtrlBufferRef[7u] = 0u;
                pduId = tempSlaveEcuConfig->FvMCtrlPduRef->FvMIfTxPduHandleId;
                pduInfo.SduDataPtr = tempCtrlBufferRef;
                pduInfo.SduLength = 8u;
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

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

#define CDD_FVM_START_SEC_CODE
#include "CDD_FVM_MemMap.h"

#if (CDD_FVM_MASTER_ECU_CONFIG_NUM > 0u)
/******************************************************************************/
/*
 * Description         This function is used to initialize the master ECU with the provided configuration.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      config: Pointer to the configuration data for the master ECU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_MasterEcuInit(P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config)
{
    P2CONST(CDD_FVM_MasterConfigType, AUTOMATIC, CDD_FVM_CONST) tempFvmMasterConfig = config->FvmMasterEcuConfig;
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
                CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE * 8u);
            CDD_FVM_BigEndianIncreaseMessageCounter(
                FVM_MasterEcuHandle[i].FvMTripCnt,
                tempFvmMasterConfig[i].FvMTripCntLength);
            CDD_FVM_MemsetOfBit(FVM_MasterEcuHandle[i].FvMResetCnt, 0u, tempFvmMasterConfig[i].FvMResetCntLength);
            CDD_FVM_BigEndianIncreaseMessageCounter(
                FVM_MasterEcuHandle[i].FvMResetCnt,
                tempFvmMasterConfig[i].FvMResetCntLength);
            if (0u
                == CDD_FVM_BufferToValue(
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
            CDD_FVM_MemsetOfBit(FVM_MasterEcuHandle[i].FvMTripCnt, 0u, tempFvmMasterConfig[i].FvMTripCntLength);
            CDD_FVM_BigEndianIncreaseMessageCounter(
                FVM_MasterEcuHandle[i].FvMTripCnt,
                tempFvmMasterConfig[i].FvMTripCntLength);
            CDD_FVM_MemsetOfBit(FVM_MasterEcuHandle[i].FvMResetCnt, 0u, tempFvmMasterConfig[i].FvMResetCntLength);
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
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].FvmRxCounter = 0u;
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].isUse = FALSE;
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].FvmSyncMsgId = 0u;
            FVM_MasterEcuHandle[i].slaveRxCtrlData[j].status = CDD_FVM_CTRL_INIT;
        }
#endif /* CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON */
        /* Host sends synchronization messages as early as possible after power-up */
        FVM_MasterEcuHandle[i].resetCycle = 1u;
        FVM_MasterEcuHandle[i].isRunSend = FALSE;
    }
}

#if (CDD_FVM_MASTER_CTRL_PDU_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Description         This function is used to find the idle data of the master control slave.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      index: Index of the master control slave.
 *                     syncMsgId: Sync message ID.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8: Idle data value.
 */
/******************************************************************************/
static FUNC(uint8, CDD_FVM_CODE) CDD_FVM_FindMasterCtrlSlaveIdleData(uint16 index, uint16 syncMsgId)
{
    uint8 retIndex = 0xFFu;
    uint8 index_i;
    P2CONST(CDD_FVM_MasterECUSyncMsgType, AUTOMATIC, CDD_FVM_CONST) tempMasterEcuHandle = &FVM_MasterEcuHandle[index];
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

/******************************************************************************/
/*
 * Description         This function is used to find the current data of the master control.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      index: Index of the master control.
 *                     syncMsgId: Sync message ID.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8: Current data value.
 */
/******************************************************************************/
static FUNC(uint8, CDD_FVM_CODE) CDD_FVM_FindMasterCtrlCurrentData(uint16 index, uint16 syncMsgId)
{
    uint8 retIndex = 0xFFu;
    uint8 index_i;
    P2CONST(CDD_FVM_MasterECUSyncMsgType, AUTOMATIC, CDD_FVM_CONST) tempMasterEcuHandle = &FVM_MasterEcuHandle[index];

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

/******************************************************************************/
/*
 * Description         This function is used to handle the reception of a control PDU frame by the master.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      index: Index of the control PDU frame.
 *                     PduInfoPtr: Pointer to the PDU information.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_MasterRxCtrlPduFrameHandle(uint16 index, P2CONST(PduInfoType, AUTOMATIC, CDD_FVM_APPL_CONST) PduInfoPtr)
{
    P2CONST(CDD_FVM_MasterConfigType, AUTOMATIC, CDD_FVM_CONST)
    tempFvmMasterConfig = &CDD_FVM_ConfigPtr->FvmMasterEcuConfig[index];
    P2VAR(CDD_FVM_MasterECUSyncMsgType, AUTOMATIC, CDD_FVM_APPL_DATA) tempMasterEcuHandle = &FVM_MasterEcuHandle[index];
    PduInfoType pduInfo;
    PduIdType pduId;
    uint8 slaveIndex;
    /* Determine what the currently received message is dealing with */

    uint8 tempProtocolVersion = PduInfoPtr->SduDataPtr[0u] >> 6u;
    uint8 tempCtrlType = PduInfoPtr->SduDataPtr[0u] & ((1u << 5u) - 1u);
    if (CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION != tempProtocolVersion)
    {
        uint16 rxsyncMsgId = ((uint16)PduInfoPtr->SduDataPtr[2u] << 8u) | PduInfoPtr->SduDataPtr[3u];
        if (CDD_FVM_CTRL_MSG_TYPE_VERIFY == tempCtrlType)
        {

            slaveIndex = CDD_FVM_FindMasterCtrlSlaveIdleData(index, rxsyncMsgId);
            if (0xFFu != slaveIndex)
            {
                P2VAR(CDD_FVM_MasterRxSlveCtrlInfoType, AUTOMATIC, CDD_FVM_APPL_DATA)
                tempRxSlaveCtrlData = &tempMasterEcuHandle->slaveRxCtrlData[slaveIndex];
                P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) tempCtrlBufferRef = tempMasterEcuHandle[index].FvmCtrlBuffer;
                tempRxSlaveCtrlData->isUse = TRUE;
                tempRxSlaveCtrlData->FvmEcuInstanceId = PduInfoPtr->SduDataPtr[1u];
                tempRxSlaveCtrlData->FvmSyncMsgId = rxsyncMsgId;
                tempCtrlBufferRef[0u] = CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION << 6u;
                tempCtrlBufferRef[0u] |= CDD_FVM_CTRL_MSG_TYPE_MASTER_ASK(tempCtrlBufferRef[0u]);
                tempCtrlBufferRef[1u] = CDD_FVM_MULTIPLE_INSTANCE_ID;
                tempCtrlBufferRef[2u] = PduInfoPtr->SduDataPtr[2u];
                tempCtrlBufferRef[3u] = PduInfoPtr->SduDataPtr[3u];
                if ((PduInfoPtr->SduDataPtr[4u] != tempFvmMasterConfig->FvMTripCntLength)
                    || (PduInfoPtr->SduDataPtr[5u] != tempFvmMasterConfig->FvMResetCntLength))
                {
                    tempCtrlBufferRef[4u] = (uint8)CDD_FVM_CTRL_ERR;
                    tempRxSlaveCtrlData->status = CDD_FVM_CTRL_ERR;
                }
                else
                {
                    tempCtrlBufferRef[4u] = (uint8)CDD_FVM_CTRL_OK;
                    tempRxSlaveCtrlData->status = CDD_FVM_CTRL_OK;
                }
                tempRxSlaveCtrlData->FvmRxCounter++;
                pduId = tempFvmMasterConfig->fvmCtrlPduRef->FvMIfTxPduHandleId;
                pduInfo.SduDataPtr = tempCtrlBufferRef;
                pduInfo.SduLength = CDD_FVM_CTRL_FRAME_BYTES_MAX;
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
/******************************************************************************/
/*
 * Description         This function is used to initialize the slave ECU with the provided configuration.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      config: Pointer to the configuration data for the slave ECU.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_SlaveEcuInit(P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config)
{
    P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST) tempFvmSlaveConfig = config->FvmSlaveEcuConfig;
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
                        CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE * 8u);
                    getNvmOk = TRUE;
                }
            }
            if (TRUE != getNvmOk)
#endif /* CDD_FVM_SLAVE_NVM_BLOCK_NUM > 0u */
            {
                CDD_FVM_MemsetOfBit(FVM_SlaveEcuHandle[i].FvMTripCnt, 0u, tempFvmSlaveConfig[i].FvMTripCntLength);
            }
            CDD_FVM_MemsetOfBit(FVM_SlaveEcuHandle[i].FvMResetCnt, 0u, tempFvmSlaveConfig[i].FvMResetCntLength);
        }
    }
}

#if (CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON)
/******************************************************************************/
/*
 * Description         This function is used by the slave to send an ask message to the master.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      index: Index of the ask message.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_SLaveSendAskMessageToMaster(uint16 index)
{
    P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST)
    tempFvmSlaveConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) tempCtrlBufferRef = FVM_SlaveEcuHandle[index].FvmCtrlBuffer;
    PduInfoType pduInfo;
    PduIdType pduId;

    /* Constructing an acknowledgement message to be sent */
    tempCtrlBufferRef[0u] = CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION << 6u;
    tempCtrlBufferRef[0u] |= CDD_FVM_CTRL_MSG_TYPE_SLAVE_VERIFY(tempCtrlBufferRef[0u]);
    tempCtrlBufferRef[1u] = CDD_FVM_MULTIPLE_INSTANCE_ID;
    tempCtrlBufferRef[2u] = (uint8)(tempFvmSlaveConfig->FvMSyncMsgId >> 8u);
    tempCtrlBufferRef[3u] = (uint8)(tempFvmSlaveConfig->FvMSyncMsgId & 0xFFu);
    tempCtrlBufferRef[4u] = tempFvmSlaveConfig->FvMTripCntLength;
    tempCtrlBufferRef[5u] = tempFvmSlaveConfig->FvMResetCntLength;
    FVM_SlaveEcuHandle[index].FvmCounter += 1u;
    tempCtrlBufferRef[6u] = FVM_SlaveEcuHandle[index].FvmCounter;
    tempCtrlBufferRef[7u] = 0u;

    pduInfo.SduDataPtr = tempCtrlBufferRef;
    pduInfo.SduLength = CDD_FVM_CTRL_FRAME_BYTES_MAX;

    pduId = tempFvmSlaveConfig->FvMCtrlPduRef->FvMIfTxPduHandleId;
    (void)PduR_CDD_FVMTransmit(pduId, &pduInfo);
}
#endif /* CDD_FVM_SLAVE_CTRL_PDU_ENABLED == STD_ON */

/******************************************************************************/
/*
 * Description         This function is used to handle the reception of a sync message PDU frame by the slave.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      index: Index of the sync message PDU frame.
 *                     PduInfoPtr: Pointer to the PDU information.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_SlaveRxSyncMessagePduFrameHandle(
    uint16 index,
    P2CONST(PduInfoType, AUTOMATIC, CDD_FVM_APPL_CONST) PduInfoPtr)
{
    P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST)
    tempFvmSlaveConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
    uint8 syncMsgBitLength = tempFvmSlaveConfig->FvMResetCntLength + tempFvmSlaveConfig->FvMTripCntLength;
    uint8 tempsyncMsgBytes = CDD_FVM_BIT_TO_BYTE(syncMsgBitLength);
    uint8 remainingBits = CDD_FVM_MOD_FROM_EIGHT(syncMsgBitLength);
    uint8 tempFvMTripCnt[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE];

    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) tempPduBuff = PduInfoPtr->SduDataPtr;
    if (tempsyncMsgBytes <= PduInfoPtr->SduLength)
    {
        /* Trip counter for synchronization messages from received data */
        CDD_FVM_GetNbitBufferFromSrc(tempPduBuff, tempFvMTripCnt, 0u, tempFvmSlaveConfig->FvMTripCntLength);
        if (CDD_FVM_SlaveTripCounterMaxValueHandle(index, tempFvMTripCnt, FVM_SlaveEcuHandle[index].FvMTripCnt) == E_OK)
        {
            /* Reset counter for synchronization messages from received data */
            CDD_FVM_GetNbitBufferFromSrc(
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

/******************************************************************************/
/*
 * Description         This function is used to handle the maximum value of the trip counter by the slave.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      index: Index of the trip counter.
 *                     rxtripCntRef: Pointer to the received trip counter reference value.
 *                     localtripCntRef: Pointer to the local trip counter reference value.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType: Standard return type indicating the success or failure of the function.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_SlaveTripCounterMaxValueHandle(
    uint16 index,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) rxtripCntRef,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) localtripCntRef)
{
    /* Condition 1: "Maximum value of the trip counter" – "ClearAcceptanceWindow" <= "Latest value of the trip counter
     * maintained by the slave ECU" >= "Maximum value of the trip counter" */
    /* Condition 2: "Initial value of the trip counter" <="Trip counter value in the synchronization message" <=
     * "Initial value of the trip counter" + "ClearAcceptanceWindow"*/
    Std_ReturnType ret = E_NOT_OK;
    P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST)
    tempFvmSlaveConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
    uint64 TripMaxalue = CDD_FVM_GET_N_BITS_MAX_VALUE(tempFvmSlaveConfig->FvMTripCntLength);
    uint16 fvmClearacceptanceWindow = tempFvmSlaveConfig->FvMClearAcceptanceWindow;
    uint64 newTripCntValue = CDD_FVM_BufferToValue(rxtripCntRef, 0u, tempFvmSlaveConfig->FvMTripCntLength);
    uint64 oldTripCntValue = CDD_FVM_BufferToValue(localtripCntRef, 0u, tempFvmSlaveConfig->FvMTripCntLength);
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
/******************************************************************************/
/*
 * Description         This function is used to handle the reception of a control PDU frame by the slave.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      index: Index of the control PDU frame.
 *                     PduInfoPtr: Pointer to the PDU information.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_SlaveRxCtrlPduFrameHandle(uint16 index, P2CONST(PduInfoType, AUTOMATIC, CDD_FVM_APPL_CONST) PduInfoPtr)
{
    P2CONST(CDD_FVM_SlaveECUSyncConfigType, AUTOMATIC, CDD_FVM_CONST)
    tempFvmSlaveConfig = &CDD_FVM_ConfigPtr->FvmSlaveEcuConfig[index];
    boolean tempResult = FALSE;
    if (CDD_FVM_CTRL_WAIT == FVM_SlaveEcuHandle[index].status)
    {
        uint8 tempProtocolVersion = PduInfoPtr->SduDataPtr[0u] >> 6u;
        uint8 tempCtrlType = PduInfoPtr->SduDataPtr[0u] & ((1u << 5u) - 1u);
        if (CDD_FVM_CTRL_FRAME_PROTOCOL_VERSION == tempProtocolVersion)
        {
            if (CDD_FVM_CTRL_MSG_TYPE_ASK == tempCtrlType)
            {
                if ((uint8)CDD_FVM_CTRL_OK == PduInfoPtr->SduDataPtr[4u])
                {
                    FVM_SlaveEcuHandle[index].status = CDD_FVM_CTRL_OK;
                    tempResult = TRUE;
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
/******************************************************************************/
/*
 * Brief               Initialize sending a single counter.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      TxFreshnessSingleConfig: Send counter configuration
 *                     TxFreshnessConfig: Control send counter
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_TxSingleCounterInit(P2CONST(CDD_FVM_TxSingleFreshnessCounterConfigType, AUTOMATIC, CDD_FVM_APPL_CONST)
                                    TxFreshnessSingleConfig)
{
    uint16 index;

    for (index = 0u; index < CDD_FVM_TX_SINGLE_FRESHNESS_COUNTER_NUM; index++)
    {
        /* It needs to be added once during initialization and starts sending from 1. */
        (void)CDD_FVM_TxUpdateSingleCounterFreshnessValue(index);
    }
}

/******************************************************************************/
/*
 * Brief               Get the freshness value corresponding to the specified freshness index
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      freshnessIndex: freshness index
 *                     FreshnessLenfth: Freshness value length
 * Param-Name[out]     None
 * Param-Name[in/out]  freshnessValue: Buffer used to store freshness values
 * Return              None
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_GetTxSingleCounterFreshnessValue(
    uint16 freshnessIndex,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) freshnessValue,
    uint8 FreshnessLenfth)
{
    Std_ReturnType result = E_NOT_OK;
    uint8 fvmFreshnessLength;

    fvmFreshnessLength = CDD_FVM_ConfigPtr->FvmTxSingleFreshnessConfig[freshnessIndex].FvmFreshnessLength;
    if (FreshnessLenfth == fvmFreshnessLength)
    {
        CDD_FVM_Memcpy(freshnessValue, FVM_TxSingleCntHandle[freshnessIndex].FVMmsgCnt, fvmFreshnessLength);
        result = E_OK;
    }
    return result;
}

/******************************************************************************/
/*
 * Brief               Update send counter
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      freshnessIndex: freshness index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_TxUpdateSingleCounterFreshnessValue(uint16 freshnessIndex)
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
/******************************************************************************/
/*
 * Brief               Initialize receive a single counter.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      RxFreshnessSingleConfig: receive counter configuration
 *                     RxFreshnessConfig: Control receive counter
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_RxSingleCounterInit(P2CONST(CDD_FVM_RxSingleFreshnessCounterConfigType, AUTOMATIC, CDD_FVM_APPL_CONST)
                                    RxFreshnessSingleConfig)
{
    uint16 index;

    for (index = 0u; index < CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM; index++)
    {
        CDD_FVM_MemsetOfBit(
            FVM_RxSingleCntHandle[index].FVMmsgCnt,
            0u,
            RxFreshnessSingleConfig[index].FvmFreshnessLength);
    }
}

/******************************************************************************/
/*
 * Brief               Get the freshness value corresponding to the specified freshness index
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      freshnessIndex: freshness index
 * Param-Name[out]     None
 * Param-Name[in/out]  freshnessValue: Buffer used to store freshness values
 * Return              None
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_GetRxSingleCounterFreshnessValue(
    uint16 freshnessIndex,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) freshnessValue)
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

/******************************************************************************/
/*
 * Brief               Update receive counter
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      freshnessIndex: freshness index
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_RxUpdateSingleCounterFreshnessValue(
    uint16 freshnessIndex,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) freshnessValue)
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

/******************************************************************************/
/*
 * Brief               This interface is used by the FVM to obtain the current
 *                     freshness value.
 * ServiceId           0x4f
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      freshnessIndex: Holds the identifier of the
 *                                            freshness value.
 *                     FVMTruncatedFreshnessValue: Holds the truncated
 *                                  freshness value that was contained in the
 *                                  Secured I-PDU.
 *                     FVMTruncatedFreshnessValueLength: Holds the length in
 *                                  bits of the truncated freshness value.
 *                     FVMAuthVerifyAttempts:Hold the number of authentication
 *                                  verify attempts of this PDU since the last
 *                                  reception.
 * Param-Name[out]     FVMFreshnessValue: Holds the freshness value to be used
 *                                  for the calculation of the authenticator.
 * Param-Name[in/out]  FVMFreshnessValueLength: Holds the length in bits of
 *                                                the freshness value.
 * Return              E_OK: request successful.
 *                     E_NOT_OK: request failed, a freshness value cannot be
 *                               provided due to general issues for freshness orx
 *                               this FreshnessValueId.
 *                     E_BUSY: The freshness information can temporarily not be
 *                             provided.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_RxSingleCounterFreshnessHandle(
    uint16 freshnessIndex,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    uint32 FVMTruncatedFreshnessValueLength,
    uint16 FVMAuthVerifyAttempts,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
)
{
    Std_ReturnType result = E_NOT_OK;
    uint8 freshnessTempBuff[CDD_FVM_FRESHNESS_VALUE_MAX_BYTE] = {0};
    uint8* freshnessRxTruncTempBuff = NULL_PTR;
    uint8 RxFreshnessLength = CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[freshnessIndex].FvmFreshnessLength;
    uint16 RxMaxDeltaCounter = CDD_FVM_ConfigPtr->FvmRxSingleFreshnessConfig[freshnessIndex].MaxDeltaCounter;
    if ((freshnessIndex < CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM) && (*FVMFreshnessValueLength <= RxFreshnessLength))
    {
        freshnessRxTruncTempBuff = FVM_RxSingleCntHandle[freshnessIndex].FVMmsgRxFreshnessValue;
        result = CDD_FVM_GetRxSingleCounterFreshnessValue(freshnessIndex, freshnessTempBuff);
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
                    const uint8 tempBuffAllOne[8u] = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
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

                FVMAuthVerifyAttempts = 0u;
            }
        }
    }

    return result;
}

#endif /* CDD_FVM_RX_SINGLE_FRESHNESS_COUNTER_NUM > 0u */

#if (CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u)

/******************************************************************************/
/*
 * Description         This function initializes the transmission of multiple truncated counters
 *                     based on the provided configuration and slave sync message index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      config: Pointer to the configuration for initializing the transmission of multiple truncated
 * counters.
 *                     slaveSyncMsgIndex: Index representing the slave sync message.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_TxMultipleTruncatedCounterInit(
    P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config,
    uint16 slaveSyncMsgIndex)
{
    P2CONST(CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_CONST)
    tempTxFreshnessConfig = config->FvmTxMultipleFreshnessTruncateConfig;
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST) tempFreshnessConfig = NULL_PTR;
    uint16 salveSyncIndex;

    for (uint16 i = 0; i < CDD_FVM_TX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM; i++)
    {
        tempFreshnessConfig = tempTxFreshnessConfig[i].freshnessConfigRef;
        salveSyncIndex = tempTxFreshnessConfig[i].syncMsgIndex;
        if ((0xFFFFu == slaveSyncMsgIndex) || (slaveSyncMsgIndex == salveSyncIndex))
        {
            CDD_FVM_MemsetOfBit(
                FVM_TxMultipleTruncatedCntHandle[i].FVMFreshnessValue,
                0u,
                tempTxFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_MemsetOfBit(
                FVM_TxMultipleTruncatedCntHandle[i].FVMPreviousValue,
                0u,
                tempTxFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_MemsetOfBit(
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

/******************************************************************************/
/*
 * Description         This function is used to get the freshness value of a specific index
 *                     from the multiple truncated counters.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      freshnessIndex: Index of the freshness value to retrieve.
 *                     FreshnessLength: Length of the freshness value.
 * Param-Name[out]     None
 * Param-Name[in/out]  freshnessValue: Retrieved freshness value.
 *                     FVMTruncatedFreshnessValue: Pointer to the truncated freshness value.
 *                     FVMTruncatedFreshnessValueLength: Length of the truncated freshness value.
 * Return              Std_ReturnType: E_OK if the freshness value is retrieved successfully, E_NOT_OK otherwise.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_GetTxMultipleTruncatedCounterFreshnessValue(
    uint16 freshnessIndex,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) freshnessValue,
    uint8 FreshnessLength,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMTruncatedFreshnessValueLength)
{
    Std_ReturnType result;
    uint8 tempSyncMsgLatestBuffer[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE] = {0};
    uint8 tempSyncMsgPreviouslyBuffer[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE] = {0};
    uint64 latestSyncMsgValue;
    uint64 previouslySyncMsgValue;
    P2CONST(CDD_FVM_TxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_APPL_CONST)
    tempTxFreshnessConfig = &CDD_FVM_ConfigPtr->FvmTxMultipleFreshnessTruncateConfig[freshnessIndex];
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST)
    tempFreshnessConfig = tempTxFreshnessConfig->freshnessConfigRef;
    uint16 syncMsgIndex = tempTxFreshnessConfig->syncMsgIndex;
    uint8 syncMsgLength;
    uint8 MsgCntLength;
    uint8 tempTruncatedCounterLength;

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

            MsgCntLength = tempFreshnessConfig->msgCntLower.CounterLen + tempFreshnessConfig->msgCntUpper.CounterLen;
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

                uint8 tempFirstBit = 0u;
                uint8 tempAllSyncMsgAndOffset = tempFreshnessConfig->tirpCnt.CounterPos + syncMsgLength;

                if (0u != tempFreshnessConfig->tirpCnt.CounterPos)
                {
                    tempFirstBit = (CDD_FVM_MOD_FROM_EIGHT(tempAllSyncMsgAndOffset) == 0u)
                                       ? (0u)
                                       : (8u - CDD_FVM_MOD_FROM_EIGHT(tempAllSyncMsgAndOffset));
                }

                latestSyncMsgValue =
                    CDD_FVM_BufferToValue(tempSyncMsgLatestBuffer, tempFirstBit, tempAllSyncMsgAndOffset);
                previouslySyncMsgValue =
                    CDD_FVM_BufferToValue(tempSyncMsgPreviouslyBuffer, tempFirstBit, tempAllSyncMsgAndOffset);
                /* Determine if a synchronization message has changed */
                if (latestSyncMsgValue != previouslySyncMsgValue)
                {
                    CDD_FVM_MemsetOfBit(FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FvmMsgCnt, 0u, MsgCntLength);
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
                if (CDD_FVM_BufferToValue(FVM_TxMultipleTruncatedCntHandle[freshnessIndex].FvmMsgCnt, 0u, MsgCntLength)
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

/******************************************************************************/
/*
 * Description         This function initializes the reception of multiple truncated counters based on the provided
 * configuration.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      config: Pointer to the configuration for initializing the reception of multiple truncated
 *                     slaveSyncMsgIndex: Index representing the slave sync message.
 * counters.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_RxMultipleTruncatedCounterInit(
    P2CONST(CDD_FVM_ConfigType, AUTOMATIC, CDD_FVM_CFG_CONST) config,
    uint16 slaveSyncMsgIndex)
{
    P2CONST(CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_CONST)
    tempRxFreshnessConfig = config->FvmRxMultipleFreshnessTruncateConfig;
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST) tempFreshnessConfig = NULL_PTR;
    uint16 salveSyncIndex;

    for (uint16 i = 0; i < CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM; i++)
    {
        tempFreshnessConfig = tempRxFreshnessConfig[i].freshnessConfigRef;
        salveSyncIndex = tempRxFreshnessConfig[i].syncMsgIndex;
        if ((0xFFFFu == slaveSyncMsgIndex) || (salveSyncIndex == slaveSyncMsgIndex))
        {
            CDD_FVM_MemsetOfBit(
                FVM_RxMultipleTruncatedCntHandle[i].FVMFreshnessValue,
                0u,
                tempRxFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_MemsetOfBit(
                FVM_RxMultipleTruncatedCntHandle[i].FVMPreviousValue,
                0u,
                tempRxFreshnessConfig[i].FvmFreshnessLength);
            CDD_FVM_MemsetOfBit(
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

/******************************************************************************/
/*
 * Description         This function is used to reset the flag handles for a specific slave ECU.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      SlaveEcuHandle: Pointer to the slave ECU handle.
 *                     resetFlagConfig: Pointer to the configuration of the reset flag.
 *                     truncatedFreshnessValue: Pointer to the truncated freshness value.
 *                     truncatedFreshnessValueLength: Length of the truncated freshness value.
 * Param-Name[out]     None
 * Param-Name[in/out]  resetFlagSubtraction: Pointer to the subtraction value for the reset flag.
 * Return              Std_ReturnType: E_OK if the flag handles are reset successfully, E_NOT_OK otherwise.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_RxResetFlagHandles(
    P2CONST(CDD_FVM_SlaveECUSyncMsgType, AUTOMATIC, CDD_FVM_APPL_CONST) SlaveEcuHandle,
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST) multipleFeshnessConfig,
    P2VAR(sint8, AUTOMATIC, CDD_FVM_APPL_DATA) resetFlagSubtraction,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) truncatedFreshnessValue)
{
    Std_ReturnType result = E_OK;
    uint8 latestResetTag;
    uint8 receivedResetTag;
    uint8 tempTruncatedFreshnessLen =
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

/******************************************************************************/
/*
 * Description         This function is used to handle the lower message counter for a specific freshness index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      freshnessIndex: Index of the freshness value.
 *                     FVMTruncatedFreshnessValue: Pointer to the truncated freshness value.
 * Param-Name[out]     None
 * Param-Name[in/out]  latestLowerMsgCntValue: Pointer to the latest lower message counter value.
 *                     previouslyLowerMsgCntValue: Pointer to the previously lower message counter value.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_RxMsgLowerCntHandle(
    uint16 freshnessIndex,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    P2VAR(uint64, AUTOMATIC, CDD_FVM_APPL_DATA) latestLowerMsgCntValue,
    P2VAR(uint64, AUTOMATIC, CDD_FVM_APPL_DATA) previouslyLowerMsgCntValue)
{
    /* Handles the low message counter, received compared to the local placeholder memory */
    uint8 tempFVMTruncatedFreshnessValueLength;
    P2CONST(CDD_FVM_MultipleFreshnessCntType, AUTOMATIC, CDD_FVM_APPL_CONST)
    tempRxFreshnessCnt = &FVM_RxMultipleTruncatedCntHandle[freshnessIndex];
    P2CONST(CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_APPL_CONST)
    tempRxFreshnessConfig = &CDD_FVM_ConfigPtr->FvmRxMultipleFreshnessTruncateConfig[freshnessIndex];
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST)
    tempFreshnessConfig = tempRxFreshnessConfig->freshnessConfigRef;
    uint8 tempMsgLowerCntLatestBuffer[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE] = {0};
    uint8 tempMsgLowerCntPreviouslyBuffer[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE] = {0};
    tempFVMTruncatedFreshnessValueLength =
        tempFreshnessConfig->msgCntLower.CounterLen + tempFreshnessConfig->resetFlag.CounterLen;
    CDD_FVM_GetNbitBufferFromSrc(
        tempRxFreshnessCnt->FVMFreshnessValue,
        tempMsgLowerCntPreviouslyBuffer,
        tempFreshnessConfig->msgCntLower.CounterPos,
        tempFreshnessConfig->msgCntLower.CounterLen);

    uint8 remainingBits = CDD_FVM_MOD_SHIFT(tempFVMTruncatedFreshnessValueLength, 8u);
    uint8 startBit = (remainingBits > 0u) ? (8u - remainingBits) : (0u);
    CDD_FVM_GetNbitBufferFromSrc(
        FVMTruncatedFreshnessValue,
        tempMsgLowerCntLatestBuffer,
        startBit,
        tempFreshnessConfig->msgCntLower.CounterLen);
    *latestLowerMsgCntValue =
        CDD_FVM_BufferToValue(tempMsgLowerCntLatestBuffer, 0u, tempFreshnessConfig->msgCntLower.CounterLen);
    *previouslyLowerMsgCntValue =
        CDD_FVM_BufferToValue(tempMsgLowerCntPreviouslyBuffer, 0u, tempFreshnessConfig->msgCntLower.CounterLen);
}

/******************************************************************************/
/*
 * Description         This function handles the reception of synchronous messages and updates the latest and previously
 * received values.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      freshnessIndex: Index representing the freshness of the message.
 *                     SyncMsgLatestBuffer: Pointer to the buffer containing the latest synchronous message.
 *                     latestSyncMsgValue: Pointer to the variable containing the latest synchronous message value.
 *                     previouslySyncMsgValue: Pointer to the variable containing the previously received synchronous
 * message value.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_RxSyncMessageHandle(
    uint16 freshnessIndex,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) SyncMsgLatestBuffer,
    P2VAR(uint64, AUTOMATIC, CDD_FVM_APPL_DATA) latestSyncMsgValue,
    P2VAR(uint64, AUTOMATIC, CDD_FVM_APPL_DATA) previouslySyncMsgValue)
{
    uint8 tempSyncMsgPreviouslyBuffer[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE] = {0};
    P2CONST(CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_APPL_CONST)
    tempRxFreshnessConfig = &CDD_FVM_ConfigPtr->FvmRxMultipleFreshnessTruncateConfig[freshnessIndex];
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST)
    tempFreshnessConfig = tempRxFreshnessConfig->freshnessConfigRef;
    uint16 slaveIndex = tempRxFreshnessConfig->syncMsgIndex;
    P2CONST(CDD_FVM_SlaveECUSyncMsgType, AUTOMATIC, CDD_FVM_APPL_CONST)
    tempFvmSlaveEcuHandle = &FVM_SlaveEcuHandle[slaveIndex];
    P2CONST(CDD_FVM_MultipleFreshnessCntType, AUTOMATIC, CDD_FVM_APPL_CONST)
    tempRxFreshnessCnt = &FVM_RxMultipleTruncatedCntHandle[freshnessIndex];
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

    uint8 tempFirstBit = 0u;
    uint8 tempAllSyncMsgAndOffset = tempFreshnessConfig->tirpCnt.CounterPos + syncMsgLength;

    if (0u != tempFreshnessConfig->tirpCnt.CounterPos)
    {
        tempFirstBit = (CDD_FVM_MOD_FROM_EIGHT(tempAllSyncMsgAndOffset) == 0u)
                           ? (0u)
                           : (8u - CDD_FVM_MOD_FROM_EIGHT(tempAllSyncMsgAndOffset));
    }

    *latestSyncMsgValue = CDD_FVM_BufferToValue(SyncMsgLatestBuffer, tempFirstBit, tempAllSyncMsgAndOffset);
    *previouslySyncMsgValue = CDD_FVM_BufferToValue(tempSyncMsgPreviouslyBuffer, tempFirstBit, tempAllSyncMsgAndOffset);
}

/******************************************************************************/
/*
 * Description         This function is used to add a value to a multiple freshness buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      buffer: Pointer to the buffer.
 *                     value: Value to be added to the buffer.
 *                     bitLength: Length of the value in bits.
 * Param-Name[out]     None
 * Param-Name[in/out]  buffer: Pointer to the updated buffer.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_MultipleFreshnessBufferAddN(P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) buffer, sint8 value, uint8 bitLength)
{
    uint32 tempValue = 0u;
    uint8 index;
    uint8 tempSyncMsgBytes = CDD_FVM_BIT_TO_BYTE(bitLength);
    uint32 tempNValue = (uint32)value;
    for (index = 0; index < tempSyncMsgBytes; index++)
    {
        tempValue |= (uint32)buffer[index] << (8u * (tempSyncMsgBytes - index - 1u));
    }
    tempValue = tempValue + tempNValue;

    for (index = 0; index < tempSyncMsgBytes; index++)
    {
        buffer[index] = (uint8)(tempValue >> (8u * (tempSyncMsgBytes - index - 1u)));
    }
}

/******************************************************************************/
/*
 * Description         This function is used to handle the reception of multiple truncated counters freshness values.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      freshnessIndex: The index of the freshness value to be updated.
 *                     FVMTruncatedFreshnessValue: Pointer to the buffer containing the received freshness value.
 *                     FVMTruncatedFreshnessValueLength: The length of the received freshness value in bytes.
 *                     FVMAuthVerifyAttempts: The number of attempts to authenticate the received freshness value.
 * Param-Name[out]     None
 * Param-Name[in/out]  FVMFreshnessValueLength: Pointer to the variable where the length of the updated freshness
 *                                              value will be stored.
 *                     FVMFreshnessValue: Pointer to the buffer where the updated freshness value will be stored.
 * Return              Std_ReturnType: Standard return type indicating the success or failure of the function.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_RxMultipleCounterTruncatedHandle(
    uint16 freshnessIndex,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) FVMTruncatedFreshnessValue,
    uint32 FVMTruncatedFreshnessValueLength,
    uint16 FVMAuthVerifyAttempts,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValue,
    P2VAR(uint32, AUTOMATIC, CDD_FVM_APPL_DATA) FVMFreshnessValueLength
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
)
{
    Std_ReturnType result;
    uint8 tempSyncMsgLatestBuffer[CDD_FVM_TRIP_COUNTER_VALUE_MAX_BYTE + CDD_FVM_RESET_COUNTER_VALUE_MAX_BYTE] = {0};
    uint64 latestSyncMsgValue;
    uint64 previouslySyncMsgValue;
    uint64 latestLowerMsgCntValue;
    uint64 previouslyLowerMsgCntValue;
    P2CONST(CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_APPL_CONST)
    tempRxFreshnessConfig = &CDD_FVM_ConfigPtr->FvmRxMultipleFreshnessTruncateConfig[freshnessIndex];
    P2CONST(CDD_FVM_MultipleFreshnessValueStructureType, AUTOMATIC, CDD_FVM_APPL_CONST)
    tempFreshnessConfig = tempRxFreshnessConfig->freshnessConfigRef;
    uint16 syncMsgIndex = tempRxFreshnessConfig->syncMsgIndex;
    P2VAR(CDD_FVM_MultipleFreshnessCntType, AUTOMATIC, CDD_FVM_APPL_DATA)
    tempRxFreshnessCnt = &FVM_RxMultipleTruncatedCntHandle[freshnessIndex];
    uint8 syncMsgLength;
    uint8 MsgCntLength;
    uint8 tempTruncatedCounterLength;
    P2CONST(CDD_FVM_SlaveECUSyncMsgType, AUTOMATIC, CDD_FVM_APPL_CONST)
    tempFvmSlaveEcuHandle = &FVM_SlaveEcuHandle[syncMsgIndex];

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
            CDD_FVM_RxMsgLowerCntHandle(
                freshnessIndex,
                FVMTruncatedFreshnessValue,
                &latestLowerMsgCntValue,
                &previouslyLowerMsgCntValue);
            /* Comparison processing from the received reset flag to the local temporarily stored reset flag */
            sint8 tempResetFlagCompareX = 0;
            result = CDD_FVM_RxResetFlagHandles(
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
                uint8 tempMsgCntPreviouslyBuffer[CDD_FVM_MESSAGE_COUNTER_VALUE_MAX_BYTE] = {0};
                if ((latestSyncMsgValue + (uint64)tempResetFlagCompareX) == previouslySyncMsgValue)
                {
                    if (previouslyLowerMsgCntValue >= latestLowerMsgCntValue)
                    {
                        /* Format 2 : The high bit is incremented by 1 and the low
                         *              bit is replaced with the received value.
                         */
                        uint8 tempMsgCntLength =
                            tempFreshnessConfig->msgCntUpper.CounterLen + tempFreshnessConfig->msgCntLower.CounterLen;
                        CDD_FVM_GetNbitBufferFromSrc(
                            tempRxFreshnessCnt->FVMFreshnessValue,
                            tempMsgUpperCntPreviouslyBuffer,
                            tempFreshnessConfig->msgCntUpper.CounterPos,
                            tempFreshnessConfig->msgCntUpper.CounterLen);
                        /* The receiver ECU verifies the authenticator by overwriting the message counter with the
                         * maximum value. */

                        CDD_FVM_GetNbitBufferFromSrc(
                            tempRxFreshnessCnt->FVMFreshnessValue,
                            tempMsgCntPreviouslyBuffer,
                            tempFreshnessConfig->msgCntUpper.CounterPos,
                            tempMsgCntLength);
                        if (CDD_FVM_BufferToValue(tempMsgCntPreviouslyBuffer, 0u, tempMsgCntLength)
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

/******************************************************************************/
/*
 * Description         This function is used to check if the received multiple counter is SecOC pass for a specific
 * freshness index.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      freshnessIndex: Freshness index for which the check is performed.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_RxMultipleCounterIsSecOCPass(uint16 freshnessIndex)
{
    P2CONST(CDD_FVM_RxMultipleFreshnessTruncatedCounterConfigType, TYPEDEF, CDD_FVM_APPL_CONST)
    tempRxFreshnessConfig = &CDD_FVM_ConfigPtr->FvmRxMultipleFreshnessTruncateConfig[freshnessIndex];
    P2VAR(CDD_FVM_MultipleFreshnessCntType, AUTOMATIC, CDD_FVM_APPL_DATA)
    tempRxFreshnessCnt = &FVM_RxMultipleTruncatedCntHandle[freshnessIndex];
    CDD_FVM_Memcpy(
        tempRxFreshnessCnt->FVMPreviousValue,
        tempRxFreshnessCnt->FVMFreshnessValue,
        tempRxFreshnessConfig->FvmFreshnessLength);
}

#endif /* CDD_FVM_RX_MULTIPLE_FRESHNESS_TRUNCATED_COUNTER_NUM > 0u */

#if (CDD_FVM_SUPPORT_MUILT_MASTER_ENABLED == STD_ON)

/******************************************************************************/
/*
 * Description         This function is used to copy the synchronous message from
 *                     the master buffer to the slave buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      masterIndex: Index of the master buffer.
 *                     slaveIndex: Index of the slave buffer.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_SlaveCopyMasterSyncMsg(uint16 masterIndex, uint16 slaveIndex)
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

/******************************************************************************/
/*
 * Brief               Get the freshness configuration index of the send counter.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      FVMFreshnessValueID: Counter ID
 * Param-Name[out]     mode: Pointer to storage counter mode
 * Param-Name[in/out]  None
 * Return              uint16 Return index value
 */
/******************************************************************************/
static FUNC(uint16, CDD_FVM_CODE) CDD_FVM_GetTxFreshnessConfigIndex(
    uint16 FVMFreshnessValueID,
    P2VAR(CDD_FVM_ModeType, AUTOMATIC, CDD_FVM_APPL_DATA) mode)
{
    uint16 index;
    uint16 freshnessIndex = 0u;

    for (index = 0u; index < CDD_FVM_TX_FRESHNESS_CONFIG_NUM; index++)
    {
        if (CDD_FVM_ConfigPtr->FvmTxFreshnessConfig[index].FvmFreshnessId == FVMFreshnessValueID)
        {
            freshnessIndex = CDD_FVM_ConfigPtr->FvmTxFreshnessConfig[index].freshnessIndex;
            *mode = CDD_FVM_ConfigPtr->FvmTxFreshnessConfig[index].mode;
            break;
        }
    }
    return freshnessIndex;
}

/******************************************************************************/
/*
 * Brief               Get the freshness configuration index of the send counter.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      FVMFreshnessValueID: Counter ID
 * Param-Name[out]     mode: Pointer to storage counter mode
 * Param-Name[in/out]  None
 * Return              uint16 Return index value
 */
/******************************************************************************/
static FUNC(uint16, CDD_FVM_CODE) CDD_FVM_GetRxFreshnessConfigIndex(
    uint16 FVMFreshnessValueID,
    P2VAR(CDD_FVM_ModeType, AUTOMATIC, CDD_FVM_APPL_DATA) mode)
{
    uint16 index;
    uint16 freshnessIndex = 0u;

    for (index = 0u; index < CDD_FVM_RX_FRESHNESS_CONFIG_NUM; index++)
    {
        if (CDD_FVM_ConfigPtr->FvmRxFreshnessConfig[index].FvmFreshnessId == FVMFreshnessValueID)
        {
            freshnessIndex = CDD_FVM_ConfigPtr->FvmRxFreshnessConfig[index].freshnessIndex;
            *mode = CDD_FVM_ConfigPtr->FvmRxFreshnessConfig[index].mode;
            break;
        }
    }
    return freshnessIndex;
}

/******************************************************************************/
/*
 * Brief               Copy the information value to the acquisition source.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      src: data source
 *                     dataBitNum：Number of bits copied
 * Param-Name[out]     dest：The data location to be copied to
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_Memcpy(
    P2VAR(void, AUTOMATIC, CDD_FVM_APPL_DATA) dest,
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    uint8 dataNumOfBit)
{
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) destPtr = (P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA))dest;
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) srcPtr = (P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST))src;

    uint8 remainingBits = CDD_FVM_MOD_SHIFT(dataNumOfBit, 8u);
    uint8 tempstartBit = (remainingBits > 0u) ? (8u - remainingBits) : remainingBits;
    CDD_FVM_GetNbitBufferFromSrc(srcPtr, destPtr, tempstartBit, dataNumOfBit);
}

/******************************************************************************/
/*
 * Description         This function is used to extract the low bits from the
 *                     source buffer and store them in the destination buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      src: Pointer to the source buffer.
 *                     allBitLength: The total length of the source buffer in bits.
 *                     lowbitLength: The length of the low bits to be extracted and stored in the destination buffer.
 * Param-Name[out]     None
 * Param-Name[in/out]  dst: Pointer to the destination buffer.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_GetLowBitFromSrc(
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    P2VAR(void, AUTOMATIC, CDD_FVM_APPL_DATA) dst,
    uint8 allBitLength,
    uint8 lowbitLength)
{
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) srcPtr = (P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST))src;
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) destPtr = (P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA))dst;
    uint8 remainingBits = CDD_FVM_MOD_FROM_EIGHT(allBitLength);
    uint8 tempstartBit =
        (remainingBits > 0u) ? (8u - remainingBits + allBitLength - lowbitLength) : (allBitLength - lowbitLength);
    CDD_FVM_GetNbitBufferFromSrc(srcPtr, destPtr, tempstartBit, lowbitLength);
}

/******************************************************************************/
/*
 * Description         This function is used to calculate the coverage of the low N bits
 *                     from the source buffer and store the result in the destination buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      dest: Pointer to the destination buffer.
 *                     destBitLength: The length of the destination buffer in bits.
 *                     srcBitLength: The length of the source buffer in bits.
 * Param-Name[out]     None
 * Param-Name[in/out]  src: Pointer to the source buffer.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_CoverageOfLowNBits(
    P2VAR(void, AUTOMATIC, CDD_FVM_APPL_DATA) dest,
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    uint8 srcBitLength,
    uint8 destBitLength)
{
    if ((srcBitLength >= destBitLength))
    {
        uint8 remainingBits = CDD_FVM_MOD_SHIFT(srcBitLength, 8u);
        uint8 startBit =
            (remainingBits > 0u) ? (8u - remainingBits + srcBitLength - destBitLength) : (srcBitLength - destBitLength);

        CDD_FVM_CopyBufferToDstBits(dest, src, startBit, destBitLength);
    }
}

/******************************************************************************/
/*
 * Description         This function copies specific bits from the source to the destination buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      src: Pointer to the source buffer.
 *                     dst: Pointer to the destination buffer.
 *                     firstNBit: Position of the first bit to copy.
 *                     bitLength: Number of bits to copy.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_CopySrcFromMbitToDstBits(
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) dst,
    uint8 firstNBit,
    uint8 bitLength)
{
    /* Calculate the byte index and bit index of the copy start location */
    uint8 startByteIndex = CDD_FVM_DIV_SHIFT(firstNBit, 3u);
    uint8 startBitIndex = 8u - CDD_FVM_MOD_FROM_EIGHT(firstNBit);

    /* Calculate the number of bytes and bits in the copy */
    uint8 copyBytes = CDD_FVM_DIV_SHIFT(bitLength - startBitIndex, 3u);
    uint8 copyBits = CDD_FVM_MOD_FROM_EIGHT(bitLength - startBitIndex);
    uint8 nextByteIndex = 0u;
    uint8 srcMask = 0u;

    /* Step 1: Process the first byte */
    if ((startBitIndex > 0u) && (startBitIndex != 8u))
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
        dst[nextByteIndex] &= ~(srcMask << (8u - copyBits));
        srcMask = ~((1u << (8u - copyBits)) - 1u);
        dst[nextByteIndex] |= (src[nextByteIndex] & srcMask);
    }
}

/******************************************************************************/
/*
 * Description         This function is used to compare two buffers bit by bit up to
 *                     a specified length and count the number of differences within a maximum delta counter.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      ptr1: Pointer to the first buffer.
 *                     ptr2: Pointer to the second buffer.
 *                     bitLength: The length of the buffers in bits to be compared.
 *                     maxDeltaCounter: The maximum number of differences allowed.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType: CDD_FVM_E_EQUAL CDD_FVM_E_MORE_THAN CDD_FVM_E_LESS_THAN.
 *                     ptr1 == ptr2 : CDD_FVM_E_EQUAL  ptr1 > ptr2 : CDD_FVM_E_MORE_THAN
 *                     ptr1 < ptr2 : CDD_FVM_E_LESS_THAN
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_BitCompare(
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) ptr1,
    P2CONST(void, AUTOMATIC, CDD_FVM_APPL_CONST) ptr2,
    uint8 bitLength,
    uint16 maxDeltaCounter)
{
    Std_ReturnType result = CDD_FVM_E_NOT_OK;
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) bytePtr1 = (P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST))ptr1;
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) bytePtr2 = (P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST))ptr2;

    uint64 ptr1Value = CDD_FVM_BufferToValue(bytePtr1, 0u, bitLength);
    uint64 ptr2Value = CDD_FVM_BufferToValue(bytePtr2, 0u, bitLength);
    uint64 maxValue = CDD_FVM_GET_N_BITS_MAX_VALUE(bitLength);
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
            result = (deltaCount == 0u)
                         ? CDD_FVM_E_EQUAL
                         : ((deltaCount == 1u) ? CDD_FVM_E_LESS_THAN
                                               : ((deltaCount <= (uint64)maxDeltaCounter) ? CDD_FVM_E_LESS_THAN
                                                                                          : CDD_FVM_E_MORE_THAN));
        }
    }

    return result;
}

/******************************************************************************/
/*
 * Description         This function judges the upward growth of two buffers based on the specified parameters.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      buff1: Pointer to the first buffer.
 *                     buff2: Pointer to the second buffer.
 *                     nbit: Number of bits to compare.
 *                     DeltaCounter: Counter value for comparison.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType: Result of the upward growth judgment.
 */
/******************************************************************************/
static FUNC(Std_ReturnType, CDD_FVM_CODE) CDD_FVM_JudgmentUpwardGrowth(
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) buff1,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) buff2,
    uint8 nbit,
    uint16 DeltaCounter)
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

/******************************************************************************/
/*
 * Description         This function is used to increase the message counter in big-endian format.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      freshnessBit: The bit position of the message counter in the buffer.
 * Param-Name[out]     None
 * Param-Name[in/out]  freshnessCountBuff: Pointer to the buffer containing the message counter.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_BigEndianIncreaseMessageCounter(
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) freshnessCountBuff,
    uint8 freshnessBit)
{
    uint8 num_bytes = CDD_FVM_BIT_TO_BYTE(freshnessBit); /* Calculate The Number Of Bytes Required */
    uint8 remaining_bits = CDD_FVM_MOD_SHIFT(freshnessBit, 8u);
    uint8 mask = (1u << (remaining_bits)) - 1u;

    for (sint8 byteIndex = (sint8)num_bytes; byteIndex > 0; byteIndex--)
    {
        freshnessCountBuff[byteIndex - 1]++;
        if (freshnessCountBuff[byteIndex - 1] != 0u)
        {
            break;
        }
    }

    if (0u != remaining_bits)
    {
        if (freshnessCountBuff[0u] > mask)
        {
            freshnessCountBuff[0u] -= 1u;
            freshnessCountBuff[0u] &= ~mask;
        }
    }
}

/******************************************************************************/
/*
 * Description         This function is used to copy data from source to destination with a specified length in bits.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      src: Pointer to the source buffer.
 *                     firstNBit: The starting bit position in the destination buffer.
 *                     bitLength: The length of the data to be copied in bits.
 * Param-Name[out]     None
 * Param-Name[in/out]  dst: Pointer to the destination buffer.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_CopyBufferToDstBits(
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) dst,
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    uint8 firstNBit,
    uint8 bitLength)
{
    /* Calculate the byte index and bit index of the copy start location */
    uint8 startByteIndex = CDD_FVM_DIV_SHIFT(firstNBit, 3u);
    uint8 startBitIndex = CDD_FVM_MOD_FROM_EIGHT(firstNBit);

    /* Calculating the number of bytes and bits in a copy */
    uint8 copyBytes = CDD_FVM_DIV_SHIFT(bitLength, 3u);
    uint8 copyBits = CDD_FVM_MOD_FROM_EIGHT(bitLength);
    uint8 nextByteIndex;
    uint8 startRemainBitIndex = 8u - startBitIndex;
    uint8 srcMask = (1u << startRemainBitIndex) - 1u;
    uint8 maskReminbits;

    if (copyBits < startRemainBitIndex)
    {
        maskReminbits = startRemainBitIndex - copyBits;
        uint8 remainMask = (1u << maskReminbits) - 1u;
        uint8 tempindex;
        if (copyBits > 0u)
        {
            srcMask = (1u << copyBits) - 1u;
            dst[startByteIndex] &= ~(srcMask << maskReminbits);
            dst[startByteIndex] |= ((src[0u] & srcMask) << maskReminbits);
            for (uint8 i = 0u; i < copyBytes; i++)
            {
                tempindex = startByteIndex + i;
                dst[tempindex] &= (~remainMask);
                dst[tempindex] |= (src[i + 1u] >> (8u - maskReminbits));
                tempindex += 1u;
                dst[tempindex] &= remainMask;
                dst[tempindex] |= (src[i + 1u] << maskReminbits);
            }
        }
        else
        {
            for (uint8 i = 0u; i < copyBytes; i++)
            {
                tempindex = startByteIndex + i;
                dst[tempindex] &= (~remainMask);
                dst[tempindex] |= (src[i] >> (8u - maskReminbits));
                tempindex += 1u;
                dst[tempindex] &= remainMask;
                dst[tempindex] |= (src[i] << maskReminbits);
            }
        }
    }
    else
    {
        maskReminbits = copyBits - startRemainBitIndex;
        uint8 dstMask = 8u - maskReminbits;
        dst[startByteIndex] &= (~srcMask);
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
            srcMask = (1u << dstMask) - 1u;
            dst[startByteIndex + 1u] &= (srcMask);
            dst[startByteIndex + 1u] |= (src[0u] << dstMask);

            for (uint8 i = 1u; i <= copyBytes; i++)
            {
                tempindex = startByteIndex + i;
                dst[tempindex] &= (~srcMask);
                dst[tempindex] |= (src[i] >> maskReminbits);
                tempindex += 1u;
                dst[tempindex] &= (srcMask);
                dst[tempindex] |= (src[i] << dstMask);
            }
        }
    }
}

/******************************************************************************/
/*
 * Description         This function converts a buffer to a value based on the specified parameters.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      src: Pointer to the source buffer.
 *                     firstBit: Position of the first bit to convert.
 *                     bitlength: Number of bits to convert.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint64: Converted value from the buffer.
 */
/******************************************************************************/
static FUNC(uint64, CDD_FVM_CODE)
    CDD_FVM_BufferToValue(P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) src, uint8 firstBit, uint8 bitlength)
{
    uint64 retValue = 0u;
    uint8 byteLength = CDD_FVM_BIT_TO_BYTE(bitlength);

    if (bitlength <= 64u)
    {
        for (uint8 i = 0u; i < byteLength; i++)
        {
            retValue |= (uint64)src[i] << (8u * (byteLength - i - 1u));
        }
        retValue = (uint64)(retValue >> firstBit);
    }

    return retValue;
}

/******************************************************************************/
/*
 * Description         This function is used to get a buffer with a specified length in bits from the source buffer.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      src: Pointer to the source buffer.
 *                     srcIndex: The starting bit position in the source buffer.
 *                     bitlenth: The length of the buffer to be copied in bits.
 * Param-Name[out]     None
 * Param-Name[in/out]  dst: Pointer to the destination buffer.
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE) CDD_FVM_GetNbitBufferFromSrc(
    P2CONST(uint8, AUTOMATIC, CDD_FVM_APPL_CONST) src,
    P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) dst,
    uint8 srcIndex,
    uint8 bitlength)
{
    uint8 startByteIndex = CDD_FVM_DIV_SHIFT(srcIndex, 3u);
    uint8 startBitIndex = CDD_FVM_MOD_FROM_EIGHT(srcIndex);
    uint8 copyBytes = CDD_FVM_DIV_SHIFT(bitlength, 3u);
    uint8 copyBits = CDD_FVM_MOD_FROM_EIGHT(bitlength);
    uint8 startRemainBitIndex = 8u - startBitIndex;
    uint8 maskReminbits;
    uint8 tempMaskHighBits;
    uint8 tempindex;

    uint8 mask = (1u << copyBits) - 1u;

    if (startRemainBitIndex > copyBits)
    {
        maskReminbits = startRemainBitIndex - copyBits;
        tempMaskHighBits = 8u - maskReminbits;
        if (copyBits != 0u)
        {
            dst[0u] = (src[startByteIndex] >> (maskReminbits)) & mask;
        }

        for (uint8 i = 0u; i < copyBytes; i++)
        {
            tempindex = startByteIndex + i;
            if (copyBits != 0u)
            {
                dst[i + 1u] = (src[tempindex] << tempMaskHighBits) | (src[tempindex + 1u] >> (maskReminbits));
            }
            else
            {
                if (tempMaskHighBits == 0u)
                {
                    dst[i] = src[tempindex];
                }
                else
                {
                    dst[i] = (src[tempindex] << tempMaskHighBits) | (src[tempindex + 1u] >> (maskReminbits));
                }
            }
        }
    }
    else
    {
        maskReminbits = copyBits - startRemainBitIndex;
        tempMaskHighBits = 8u - maskReminbits;
        /* Process all data less than one byte */
        dst[0u] = (src[startByteIndex] << (maskReminbits)) & mask;
        if (maskReminbits > 0u)
        {
            dst[0u] |= ((src[startByteIndex + 1u]) >> tempMaskHighBits) & ((1u << maskReminbits) - 1u);
        }

        for (uint8 i = 1u; i <= copyBytes; i++)
        {
            tempindex = startByteIndex + i;
            dst[i] = (src[tempindex] << maskReminbits) | (src[tempindex + 1u] >> tempMaskHighBits);
        }
    }
}

/******************************************************************************/
/*
 * Description         This function is used to set the specified number of bits in
 *                      the destination buffer to the specified value.
 * ServiceId           N/A
 * Sync/Async          Synchronous
 * Reentrancy          Non-reentrant
 * Param-Name[in]      dst: Pointer to the destination buffer.
 *                     Value: The value to be set (0 or 1).
 *                     bitLength: The number of bits to be set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/******************************************************************************/
static FUNC(void, CDD_FVM_CODE)
    CDD_FVM_MemsetOfBit(P2VAR(uint8, AUTOMATIC, CDD_FVM_APPL_DATA) dst, uint8 Value, uint8 bitLength)
{
    uint8 writeBytes = CDD_FVM_BIT_TO_BYTE(bitLength);

    for (uint8 i = 0; i < writeBytes; i++)
    {
        dst[i] = Value;
    }
}

#define CDD_FVM_STOP_SEC_CODE
#include "CDD_FVM_MemMap.h"
