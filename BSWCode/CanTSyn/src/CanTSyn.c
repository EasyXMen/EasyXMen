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
 **  @file               : CanTSyn.c
 **  @author             : xiongfei.shi
 **  @date               : 2024/12/13
 **  @vendor             : isoft
 **  @description        : Handles the distribution of time information over CAN buses
 **
 ***********************************************************************************************************************/
/* PRQA S 6060, 6520 EOF */      /* VL_MTR_CanTSyn_STVAR */
/* PRQA S 6060 ++ */             /* VL_MTR_CanTSyn_STM19 */
/* PRQA S 1503, 1532, 2889 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef, VL_QAC_MultiReturn */
/* =================================================== inclusions =================================================== */
#include "CanTSyn_Cfg.h"
#include "IStdLib.h"
#include "Os.h"
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "CanIf.h"
#include "SchM_CanTSyn.h"
#include "CanTSyn_Internal.h"
#include "CanTSyn.h"
#if (CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
#include "IdsM.h"
#endif
#if (CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON)
#include "Can_GeneralTypes.h"
#endif
#if (CANTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON) || (CANTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
#include "StbM_CanTSyn.h"
#endif
#include "CanTSyn_Arti.h"
/* ===================================================== macros ===================================================== */
#define CANTSYN_DOMAIN(domainIdx) CanTSyn_configPtr->GlobalTimeDomain[domainIdx]
#define CANTSYN_UNUSED(v)         (void)(v)

#if CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON
#define CanTSyn_SecurityEventReporting(securityEventId, contextData) \
    IdsM_SetSecurityEventWithContextData(securityEventId, &contextData, sizeof(contextData))
#else
#define CanTSyn_SecurityEventReporting(securityEventId, contextData)
#endif

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidateTxMode
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidateCtrlIdx
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidteInitStatus
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidteTxPduId
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidteRxPduId
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_ValidatePointer
#endif
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ValidatePointer(uint8 apiId, const void* pointer);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ValidateTxMode(CanTSyn_TransmissionModeType mode);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ValidateCtrlIdx(uint8 ctrlIdx);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ValidteTxPduId(PduIdType txPduId);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ValidteRxPduId(PduIdType rxPduId);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ValidteInitStatus(uint8 apiId);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

/* slave */
CANTSYN_LOCAL_INLINE uint8   CanTSyn_UnpackType(const uint8* msg);
CANTSYN_LOCAL_INLINE uint8   CanTSyn_UnpackSequenceCounter(const uint8* msg);
CANTSYN_LOCAL_INLINE uint8   CanTSyn_UnpackDomainId(const uint8* msg);
CANTSYN_LOCAL_INLINE boolean CanTSyn_CheckMsgTypeAndLength(const uint8* msg, PduLengthType length);
CANTSYN_LOCAL_INLINE boolean CanTSyn_CheckMsgSequence(uint8 type, uint8 domainIdx);

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL uint16 CanTSyn_GetPartitionIndex(void);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_CheckGlobalTimeBaseBit(uint8 domainIdx);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_PassCRCMode(uint8 domainIdx, const PduInfoType* PduInfoPtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#if (CANTSYN_CRC_SUPPORT == STD_ON)
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL uint8 CanTSyn_CalculateCRC(
    const PduInfoType*            PduInfoPtr,
    CanTSyn_DataIDListElementType element
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    ,
    boolean useExtendedMsgFormat
#endif
);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_PassCRCModeValidate(const PduInfoType* PduInfoPtr, uint8 domainIdx, boolean* result);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ProcessFUPCrcTransmit(uint8 domainIdx, const PduInfoType* pduInfoPtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_TransmitSYNC
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_TransmitFUP
#endif
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL Std_ReturnType CanTSyn_TransmitSYNC(uint8 domainIdx, PduIdType pduId);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL Std_ReturnType CanTSyn_TransmitFUP(uint8 domainIdx, PduIdType pduId);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_RxSYNC(uint8 domainIdx, const PduInfoType* PduInfoPtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_RxFUP(uint8 domainIdx, const uint8* msg);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL uint32 CanTSyn_RxFUPExtractPduInfo(
    uint8               domainIdx,
    const uint8*        dataPtr,
    StbM_TimeTupleType* timeTuplePtr,
    StbM_UserDataType*  userDataPtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void
    CanTSyn_ProcessSyncRxPdu(uint8 domainIdx, const PduInfoType* PduInfoPtr, StbM_VirtualLocalTimeType localTime);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_RxPduInfoProcess(
    uint8                              domainIdx,
    const PduInfoType*                 PduInfoPtr,
    const CanTSyn_GlobalTimeSlaveType* slaveCfgPtr,
    StbM_VirtualLocalTimeType          localTime);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_MasterSyncRequired(uint8 domainIdx);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_MasterFupRequired(uint8 domainIdx);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_MasterSyncNotRequired(uint8 domainIdx, boolean isGlobalTimeBaseBitSet);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_MasterMain(uint8 domainIdx);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_SlaveMain(uint8 domainIdx);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void
    CanTSyn_DecrementCounter(uint32* counter, uint8 domainIdx, const CanTSyn_MasterStatusType* masterStatus);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left8(uint32 x);
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left16(uint32 x);
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left24(uint32 x);
CANTSYN_LOCAL_INLINE uint64 CanTSyn_Shift_Left32(uint64 x);
CANTSYN_LOCAL_INLINE uint64 CanTSyn_Shift_Right32(uint64 x);
CANTSYN_LOCAL_INLINE uint8  CanTSyn_HiHiByte(uint32 x);
CANTSYN_LOCAL_INLINE uint8  CanTSyn_HiLoByte(uint32 x);
CANTSYN_LOCAL_INLINE uint8  CanTSyn_LoHiByte(uint32 x);
CANTSYN_LOCAL_INLINE uint8  CanTSyn_LoLoByte(uint32 x);
CANTSYN_LOCAL_INLINE void   CanTSyn_Det_Report(uint8 apiId, uint8 errorId);

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL Std_ReturnType CanTSyn_SequenceCounterCheck(uint8 domainIdx, uint8 sequenceCounter);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL
boolean CanTSyn_SyncSentTxConfirmation(PduIdType TxPduId, StbM_VirtualLocalTimeType egresstime, uint64 t1canLocalTime);
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_NotSyncSentTxConfirmation(PduIdType TxPduId);
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ConstructFUPFrame(uint8* sduDataPtr, uint8 domainIdx);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_IsSequenceCounterJumpInvalid(
    StbM_TimeBaseStatusType timeBaseStatus,
    uint8                   domainIdx,
    uint8                   sequenceCounter);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL Std_ReturnType CanTSyn_CheckSequenceCounterConditions(
    uint8 domainIdx,
#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
    StbM_TimeBaseStatusType timeBaseStatus,
#endif
    boolean isSequenceCounterJumpInvalid,
    uint8   sequenceCounter);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ProcessFUPSecuredTransmit(uint8 domainIdx, const PduInfoType* pduInfoPtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

CANTSYN_LOCAL_INLINE uint16 CanTSyn_BitToByte(uint16 nbits);

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL
Std_ReturnType CanTSyn_TxFreshnessObtain(uint8 domainIndex, uint8* freshnessValue, uint8* truncatedFreshnessValue);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ICVGeneration(uint8 domainIndex, const uint8* generationBuffer, uint8 dataLength);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ICVTxMainfunction(uint8 domainIndex, PduInfoType* pduInfo);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_StartICVGeneration(uint8 domainIndex, const PduInfoType* pduInfoPtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_SubTlvAuthenticatedBuild(uint8 domainIndex, PduInfoType* pduInfo);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ICVStatusChange(uint8 domainIndex, boolean isTimeMaster, uint8 toState);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ICVStatusComparison(uint8 domainIndex, boolean isTimeMaster, uint8 State);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

CANTSYN_LOCAL_INLINE void CanTSyn_ICVGenerationBuildFailBehavior(uint8 domainIndex, uint16 eventId);

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL uint8 CanTSyn_FindDomainAcrossJobId(uint32 jobId, boolean isTimeMaster);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL boolean CanTSyn_ValidateICVIndication(uint8 domainIndex, uint8 apiId);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS CanTSyn_VerifyTimeAuthenticatedSubTlv
#endif
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL Std_ReturnType CanTSyn_VerifyTimeAuthenticatedSubTlv(uint8 domainIndex, const PduInfoType* pduInfoPtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ICVRxMainfunction(uint8 domainIndex);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_StartICVVerification(uint8 domainIndex);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL Std_ReturnType CanTSyn_RxFreshnessObtain(uint8 domainIndex, uint8* fvValue);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ICVVerification(uint8 domainIndex, const uint8* generationBuffer, uint8 dataLength);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_InitPortICVRuntimeVariables(uint8 domainIndex);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

CANTSYN_LOCAL_INLINE boolean CanTSyn_IsBufferValid(const CanTSyn_CircularBufferType* cb);

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL uint16 CanTSyn_CircularBufferWrite(CanTSyn_CircularBufferType* cb, const uint8* data, uint16 size);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL uint16 CanTSyn_CircularBufferRead(CanTSyn_CircularBufferType* cb, uint8* data, uint16 size);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ProcessFreshnessValue(uint8 domainIndex, const PduInfoType* pduInfoPtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_GenerateIcvWithFreshnessValue(
    uint8              domainIndex,
    const PduInfoType* pduInfoPtr,
    const uint8* const freshnessValuePtr,
    const uint8* const truncatedFreshnessValuePtr);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_VerifyIcvWithFreshnessValue(uint8 domainIndex);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_ConstructAndVerifyIcv(uint8 domainIndex, const uint8* receivedFvValue);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
CANTSYN_LOCAL void CanTSyn_VerifyIcvWithoutFreshnessValue(uint8 domainIndex);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

/* ============================================ internal data definitions =========================================== */
/* CanTSyn init status, at first define it as CANTSYN_UNINIT */
#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
/**
 * @brief CanTSyn init status, at first define it as CANTSYN_UNINIT
 */
CANTSYN_LOCAL CanTSyn_InitStatusType CanTSyn_InitStatus[CANTSYN_PARTITION_NUMBER];
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
/**
 * @brief specifying whether the immediate message has been sent
 */
CANTSYN_LOCAL boolean CanTSyn_ImmediateSent[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
/**
 * @brief specifying the sequence counter
 */
CANTSYN_LOCAL uint8 CanTSyn_SequenceCounters[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
/**
 * @brief specifying the base counter value
 */
CANTSYN_LOCAL uint8 CanTSyn_BaseCounterValues[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
/**
 * @brief specifying the master status
 */
CANTSYN_LOCAL CanTSyn_MasterStatusType CanTSyn_MasterStatus[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
/**
 * @brief specifying the transmission mode
 */
CANTSYN_LOCAL CanTSyn_TransmissionModeType CanTSyn_TransmissionMode[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"

#if (CANTSYN_SYNC_TRANSMISSION_PER_CYCLE > 0u)
#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
/**
 * @brief specifying the sync transmission counter
 */
CANTSYN_LOCAL uint8 CanTSyn_SyncTxCounter;
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_8
#include "CanTSyn_MemMap.h"
#endif

#define CANTSYN_START_SEC_VAR_CLEARED_GLOBAL_PTR
#include "CanTSyn_MemMap.h"
/**
 * @brief specifying the configuration pointer
 */
CANTSYN_LOCAL const CanTSyn_ConfigType* CanTSyn_configPtr;
#define CANTSYN_STOP_SEC_VAR_CLEARED_GLOBAL_PTR
#include "CanTSyn_MemMap.h"

/* ============================================ external data definition ============================================ */

/* ========================================== external function definitions ========================================= */
/* PRQA S 6030 ++ */ /* VL_MTR_CanTSyn_STMIF */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * The implementation of CanTSyn_Init.
 */
void CanTSyn_Init(const CanTSyn_ConfigType* configPtr)
/* PRQA S 6030 -- */
{
    CanTSyn_ArtiApiStart(2, CANTSYN_SID_INIT, configPtr);
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (CanTSyn_GetPartitionIndex() >= CANTSYN_PARTITION_NUMBER)
    {
        CanTSyn_Det_Report(CANTSYN_SID_INIT, CANTSYN_E_INVALID_PARTITION);
    }
    else
#endif
        if (configPtr == NULL_PTR)
    {
        CanTSyn_Det_Report(CANTSYN_SID_INIT, CANTSYN_E_NULL_POINTER);
    }
    else if (CanTSyn_InitStatus[CanTSyn_GetPartitionIndex()] == CANTSYN_INITED)
    {
        CanTSyn_Det_Report(CANTSYN_SID_INIT, CANTSYN_E_ALREADY_INITIALIZED);
    }
    else
#endif
    {
        if (CanTSyn_configPtr == NULL_PTR)
        {
            CanTSyn_configPtr = configPtr;
        }

        /* Initialize base update counter and time counters */
        for (uint8 domainIdx = 0u; domainIdx < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; domainIdx++)
        {
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
            if (GetApplicationID() != CANTSYN_DOMAIN(domainIdx).ApplicationId)
            {
                continue; /* PRQA S 0770 */ /* VL_CanTSyn_0770 */
            }
#endif
            CanTSyn_BaseCounterValues[domainIdx] =
                StbM_GetTimeBaseUpdateCounter(CANTSYN_DOMAIN(domainIdx).SynchronizedTimeBaseId);
            CanTSyn_MasterTimeInfoPtr[domainIdx]->CyclicResumeCounter = 0u;
            CanTSyn_MasterTimeInfoPtr[domainIdx]->DebounceCounter     = 0u;
            CanTSyn_MasterTimeInfoPtr[domainIdx]->PeriodCounter       = 1u;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->FollowUpTimeoutTicks = 0u;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->RxDebounceTimeTicks  = 0u;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->SequenceCounter      = 0u;
            CanTSyn_SlaveTimeInfoPtr[domainIdx]->FirstlyReceivedSync  = TRUE;
            CanTSyn_MasterStatus[domainIdx]                           = CANTSYN_SYNC_NOT_REQUIRED;
            CanTSyn_TransmissionMode[domainIdx]                       = CANTSYN_TX_ON;
#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
            if (CANTSYN_DOMAIN(domainIdx).GlobalTimeSlave != NULL_PTR)
            {
                CanTSyn_SlaveTimeInfoPtr[domainIdx]->SequenceCounterHysteresis =
                    CANTSYN_DOMAIN(domainIdx).GlobalTimeSlave->GlobalTimeSequenceCounterHysteresis + 1u;
            }
#endif
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
            CanTSyn_InitPortICVRuntimeVariables(domainIdx);
#endif
        }
        CanTSyn_InitStatus[CanTSyn_GetPartitionIndex()] = CANTSYN_INITED;
    }
    CanTSyn_ArtiApiReturn(1, CANTSYN_SID_INIT);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#if (CANTSYN_VERSIONINFO_API == STD_ON)
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * The implementation of CanTSyn_GetVersionInfo.
 */
void CanTSyn_GetVersionInfo(Std_VersionInfoType* versioninfoPtr)
{
    CanTSyn_ArtiApiStart(2, CANTSYN_SID_GETVERSIONINFO, versioninfoPtr);
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
    if (CanTSyn_ValidatePointer(CANTSYN_SID_GETVERSIONINFO, versioninfoPtr))
#endif
    {
        versioninfoPtr->vendorID         = CANTSYN_VENDOR_ID;
        versioninfoPtr->moduleID         = CANTSYN_MODULE_ID;
        versioninfoPtr->sw_major_version = CANTSYN_SW_MAJOR_VERSION;
        versioninfoPtr->sw_minor_version = CANTSYN_SW_MINOR_VERSION;
        versioninfoPtr->sw_patch_version = CANTSYN_SW_PATCH_VERSION;
    }
    CanTSyn_ArtiApiReturn(2, CANTSYN_SID_GETVERSIONINFO, versioninfoPtr);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * The implementation of CanTSyn_SetTransmissionMode.
 */
void CanTSyn_SetTransmissionMode(uint8 CtrlIdx, CanTSyn_TransmissionModeType Mode)
{
    CanTSyn_ArtiApiStart(3, CANTSYN_SID_SETTRANSMISSIONMODE, CtrlIdx, Mode);
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
    if (CanTSyn_ValidateTxMode(Mode) && CanTSyn_ValidateCtrlIdx(CtrlIdx)
        && CanTSyn_ValidteInitStatus(CANTSYN_SID_SETTRANSMISSIONMODE))
#endif
    {
        CanTSyn_TransmissionMode[CtrlIdx] = Mode;
    }
    CanTSyn_ArtiApiReturn(1, CANTSYN_SID_SETTRANSMISSIONMODE);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * The implementation of CanTSyn_MainFunction.
 */
void CanTSyn_MainFunction(void)
{
    CanTSyn_ArtiApiStart(1, CANTSYN_SID_MAINFUNCTION);
#if (CANTSYN_SYNC_TRANSMISSION_PER_CYCLE > 0u)
    /* SWS_CanTSyn_00229 */
    CanTSyn_SyncTxCounter = 0u;
#endif
    if (CanTSyn_InitStatus[CanTSyn_GetPartitionIndex()] == CANTSYN_INITED)
    {
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
        ApplicationType applicationID = GetApplicationID();
#endif
        /* look through each time domain */
        for (uint8 domainIdx = 0u; domainIdx < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; domainIdx++)
        {
            const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
            if (applicationID != timeDomainPtr->ApplicationId)
            {
                continue; /* PRQA S 0770 */ /* VL_CanTSyn_0770 */
            }
#endif
            /* check whether configured as a time master */
            if (timeDomainPtr->GlobalTimeMaster != NULL_PTR)
            {
                /* deal with functionality as a time master */
                CanTSyn_MasterMain(domainIdx);
            }
            /* check whether configured as a time slave */
            if (timeDomainPtr->GlobalTimeSlave != NULL_PTR)
            {
                CanTSyn_SlaveMain(domainIdx);
            }
        }
    }
    CanTSyn_ArtiApiReturn(1, CANTSYN_SID_MAINFUNCTION);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * The implementation of CanTSyn_TxConfirmation.
 */
void CanTSyn_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    CanTSyn_ArtiApiStart(3, CANTSYN_SID_TXCONFIRMATION, TxPduId, result);
#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON
    boolean syncSentTxRet = TRUE;
#endif
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
    if (CanTSyn_ValidteTxPduId(TxPduId) && CanTSyn_ValidteInitStatus(CANTSYN_SID_TXCONFIRMATION))
#endif
    {
        StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(TxPduId).SynchronizedTimeBaseId;
        StbM_VirtualLocalTimeType     egresstime;
        /* SWS_CanTSyn_00150 */
#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON
        Can_TimeStampType t1can;
        Std_ReturnType    tvValid        = CanIf_GetEgressTimeStamp(TxPduId, &t1can);
        uint64            t1canLocalTime = (t1can.seconds * CANTSYN_1NS_SECOND) + t1can.nanoseconds;
        egresstime.nanosecondsHi         = (uint32)CanTSyn_Shift_Right32(t1canLocalTime);
        egresstime.nanosecondsLo         = (uint32)t1canLocalTime;
#else
        SchM_Enter_CanTSyn_Context();
        Std_ReturnType tvValid = StbM_GetCurrentVirtualLocalTime(timeBaseId, &egresstime);
        SchM_Exit_CanTSyn_Context();
#endif
        const CanTSyn_GlobalTimeMasterType* timeMasterPtr     = CANTSYN_DOMAIN(TxPduId).GlobalTimeMaster;
        CanTSyn_MasterTimeInfoType*         timeMasterInfoPtr = CanTSyn_MasterTimeInfoPtr[TxPduId];
        if (result == E_OK)
        {
            /* SWS_CanTSyn_00124 */
            if ((CanTSyn_MasterStatus[TxPduId] == CANTSYN_SYNC_SENT) && (tvValid == E_OK))
            {
#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON
                syncSentTxRet = CanTSyn_SyncSentTxConfirmation(TxPduId, egresstime, t1canLocalTime);
#else
                (void)CanTSyn_SyncSentTxConfirmation(TxPduId, egresstime, 0u);
                timeMasterInfoPtr->DebounceCounter = timeMasterPtr->GlobalTimeDebounceTime;
#endif
            }
            else if (CanTSyn_MasterStatus[TxPduId] != CANTSYN_SYNC_SENT)
            {
                tvValid = E_OK;
                CanTSyn_NotSyncSentTxConfirmation(TxPduId);
            }
            else
            {
                /* do nothing */
            }
        }
        if (
#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON
            (syncSentTxRet == TRUE) &&
#endif
            ((result != E_OK) || (tvValid != E_OK)))
        {
            CanTSyn_MasterStatus[TxPduId]    = CANTSYN_SYNC_NOT_REQUIRED;
            timeMasterInfoPtr->PeriodCounter = timeMasterPtr->GlobalTimeTxPeriod;
        }
    }
    CanTSyn_ArtiApiReturn(1, CANTSYN_SID_TXCONFIRMATION);
}
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * @brief        Tx Confirmation callback for the Sync message.
 * @param[in]    TxPduId ID of the PDU that has been transmitted.
 * @param[in]    egresstime Current virtual local time
 * @param[in]    t1canLocalTime Egress timestamp of SYNC message as captured by CAN controller HW
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-86074
 */
CANTSYN_LOCAL
boolean CanTSyn_SyncSentTxConfirmation(PduIdType TxPduId, StbM_VirtualLocalTimeType egresstime, uint64 t1canLocalTime)
{
    uint64                              t4;
    const CanTSyn_GlobalTimeMasterType* timeMasterPtr     = CANTSYN_DOMAIN(TxPduId).GlobalTimeMaster;
    CanTSyn_MasterTimeInfoType*         timeMasterInfoPtr = CanTSyn_MasterTimeInfoPtr[TxPduId];
    (void)t1canLocalTime;
    boolean result = TRUE;
#if CANTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON
    StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(TxPduId).SynchronizedTimeBaseId;
#endif
#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON
    if (CANTSYN_DOMAIN(TxPduId).IsTimeBaseSourceSameAsTransmitter)
#endif
    {
        /* SWS_CanTSyn_00151 SWS_CanTSyn_00152 */
        uint64 t1vlt = egresstime.nanosecondsHi;
        t1vlt        = CanTSyn_Shift_Left32(t1vlt);
        t1vlt += egresstime.nanosecondsLo;
        t4 = timeMasterInfoPtr->T0SYNCns + (t1vlt - timeMasterInfoPtr->T0VLT);
    }
#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON
    else
    {
        /* SWS_CanTSyn_00153 */
        Can_TimeStampType         tCurrentTimeCan;
        StbM_VirtualLocalTimeType t1virtualLocalTime;
        uint64                    t1vlt;
        SchM_Enter_CanTSyn_Context();
        if ((CanIf_GetCurrentTime(TxPduId, &tCurrentTimeCan) != E_OK)
            || (StbM_GetCurrentVirtualLocalTime(CANTSYN_DOMAIN(TxPduId).SynchronizedTimeBaseId, &t1virtualLocalTime)
                != E_OK))
        {
            timeMasterInfoPtr->PeriodCounter = timeMasterPtr->GlobalTimeTxPeriod;
            SchM_Exit_CanTSyn_Context();
            result = FALSE;
        }
        else
        {
            SchM_Exit_CanTSyn_Context();
            t1vlt = t1virtualLocalTime.nanosecondsHi;
            t1vlt = CanTSyn_Shift_Left32(t1vlt);
            t1vlt += t1virtualLocalTime.nanosecondsLo;
            uint64 tLocalCurrentTime = tCurrentTimeCan.seconds * CANTSYN_1NS_SECOND + tCurrentTimeCan.nanoseconds;
            t4 =
                timeMasterInfoPtr->T0SYNCns + (t1vlt - timeMasterInfoPtr->T0VLT) - (tLocalCurrentTime - t1canLocalTime);
        }
    }
    if (result)
#endif
    {
        timeMasterInfoPtr->T4 = t4;
        /* Start period counter */
        timeMasterInfoPtr->PeriodCounter = timeMasterPtr->GlobalTimeTxPeriod;

        /* SWS_CanTSyn_00120 */
        /* check if immediate sent, if yes, start cyclic msg resume counter, stop period counter */
        if (CanTSyn_ImmediateSent[TxPduId])
        {
            timeMasterInfoPtr->CyclicResumeCounter = timeMasterPtr->CyclicMsgResumeTime;
            CanTSyn_ImmediateSent[TxPduId]         = FALSE;
            timeMasterInfoPtr->PeriodCounter       = 0u;
        }

        CanTSyn_MasterStatus[TxPduId] = CANTSYN_FUP_REQUIRED;
#if (CANTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON)
        /* SWS_CanTSyn_00141 SWS_CanTSyn_00142 */
        if (CANTSYN_DOMAIN(TxPduId).EnableTimeValidation)
        {
            StbM_CanTimeMasterMeasurementType validationMeasureDataPtr;
            (void)IStdLib_MemSet(&validationMeasureDataPtr, 0u, sizeof(StbM_CanTimeMasterMeasurementType));
            validationMeasureDataPtr.sequenceCounter = (uint16)CanTSyn_SequenceCounters[TxPduId];
            validationMeasureDataPtr.segmentId       = CANTSYN_DOMAIN(TxPduId).GlobalTimeNetworkSegmentId;
            validationMeasureDataPtr.syncEgressTimestamp.nanosecondsLo  = egresstime.nanosecondsLo;
            validationMeasureDataPtr.syncEgressTimestamp.nanosecondsHi  = egresstime.nanosecondsHi;
            validationMeasureDataPtr.preciseOriginTimestamp.seconds     = (uint32)(t4 / CANTSYN_1NS_SECOND);
            validationMeasureDataPtr.preciseOriginTimestamp.nanoseconds = (uint32)(t4 % CANTSYN_1NS_SECOND);
            (void)StbM_CanSetMasterTimingData(timeBaseId, &validationMeasureDataPtr);
        }
#endif
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * @brief        Tx Confirmation callback for time synchronization messages other than Sync.
 * @param[in]    TxPduId ID of the PDU that has been transmitted.
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-86071
 */
CANTSYN_LOCAL void CanTSyn_NotSyncSentTxConfirmation(PduIdType TxPduId)
{
    /* increment sequence counter */
    CanTSyn_SequenceCounters[TxPduId] = (CanTSyn_SequenceCounters[TxPduId] + 1u) % (CANTSYN_SC_MAX + 1u);
    const CanTSyn_GlobalTimeMasterType* timeMasterPtr = CANTSYN_DOMAIN(TxPduId).GlobalTimeMaster;
    const CanTSyn_MasterTimeInfoType*   timeInfoPtr   = CanTSyn_MasterTimeInfoPtr[TxPduId];
    if (timeMasterPtr->ImmediateTimeSync)
    {
        const boolean result       = CanTSyn_CheckGlobalTimeBaseBit((uint8)TxPduId);
        uint8 baseCounterCurrValue = StbM_GetTimeBaseUpdateCounter(CANTSYN_DOMAIN(TxPduId).SynchronizedTimeBaseId);
        if ((baseCounterCurrValue != CanTSyn_BaseCounterValues[TxPduId]) && result
            && (timeInfoPtr->DebounceCounter < CANTSYN_MAIN_FUNCTION_PERIOD)
            && (CanTSyn_TransmissionMode[TxPduId] == CANTSYN_TX_ON))
        {
            CanTSyn_MasterStatus[TxPduId]      = CANTSYN_SYNC_REQUIRED;
            CanTSyn_ImmediateSent[TxPduId]     = TRUE;
            CanTSyn_BaseCounterValues[TxPduId] = baseCounterCurrValue;
        }
    }
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        CANTSYN_DOMAIN(TxPduId).GlobalTimeMaster->GlobalTimeTxIcvGeneration;
    if (icvGeneration != NULL_PTR)
    {
        CanTSyn_ICVStatusChange((uint8)TxPduId, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_BUILDING);
        /* SWS_CanTSyn_00177 */
        if (icvGeneration->IcvTxFreshnessValueTruncLength != 0u)
        {
            StbM_SPduTxConfirmation(icvGeneration->IcvGenerationFvIdRef);
        }
    }
#endif
}
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

/* PRQA S 6070 ++ */ /* VL_MTR_CanTSyn_STCAL */
#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * The implementation of CanTSyn_RxIndication.
 */
void CanTSyn_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6070 -- */
{
    CanTSyn_ArtiApiStart(3, CANTSYN_SID_RXINDICATION, RxPduId, PduInfoPtr);
#if CANTSYN_DEV_ERROR_DETECT == STD_ON
    if (CanTSyn_ValidatePointer(CANTSYN_SID_RXINDICATION, PduInfoPtr) && CanTSyn_ValidteRxPduId(RxPduId)
        && CanTSyn_ValidteInitStatus(CANTSYN_SID_RXINDICATION))
#endif
    {
        const CanTSyn_GlobalTimeDomainType* domainCfgPtr = &CANTSYN_DOMAIN(RxPduId);
        StbM_VirtualLocalTimeType           ingressTime;
#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON
        /* SWS_CanTSyn_00145 */
        Can_TimeStampType ingressTimeCan;
        (void)IStdLib_MemCpy(&ingressTimeCan, PduInfoPtr->MetaDataPtr, sizeof(Can_TimeStampType));
        uint64 localIngressTimeCan =
            ((uint64)ingressTimeCan.seconds * CANTSYN_1NS_SECOND) + (uint64)ingressTimeCan.nanoseconds;
        ingressTime.nanosecondsHi = (uint32)(localIngressTimeCan >> CANTSYN_SHIFT_BIT32);
        ingressTime.nanosecondsLo = (uint32)localIngressTimeCan;
#else
        /* SWS_CanTSyn_00144 */
        SchM_Enter_CanTSyn_Context();
        Std_ReturnType result = StbM_GetCurrentVirtualLocalTime(domainCfgPtr->SynchronizedTimeBaseId, &ingressTime);
        SchM_Exit_CanTSyn_Context();
#endif

        const uint8*  msg    = PduInfoPtr->SduDataPtr;
        PduLengthType length = PduInfoPtr->SduLength;
        if (!CanTSyn_CheckMsgTypeAndLength(msg, length))
        {
            return;
        }

        uint8 domainId = CanTSyn_UnpackDomainId(msg);
        if (domainId != domainCfgPtr->GlobalTimeDomainId)
        {
            return;
        }

#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_OFF
        /* SYNC requires valid timestamp; FUP does not depend on it */
        uint8 type = CanTSyn_UnpackType(msg);
        if (((type == CANTSYN_SYNC_NOT_CRC_TYPE) || (type == CANTSYN_SYNC_CRC_TYPE)) && (result == E_NOT_OK))
        {
            return;
        }
#endif

        CanTSyn_RxPduInfoProcess((uint8)RxPduId, PduInfoPtr, domainCfgPtr->GlobalTimeSlave, ingressTime);
    }
    CanTSyn_ArtiApiReturn(1, CANTSYN_SID_RXINDICATION);
}
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * By this API service the CanTSyn gets an indication and the result of ICV generation.
 */
void CanTSyn_IcvGenerationIndication(uint32 jobId, Crypto_ResultType result)
{
    CanTSyn_ArtiApiStart(3, CANTSYN_SID_ICVGENERATIONINDICATION, jobId, result);
    uint8 domainIndex = CanTSyn_FindDomainAcrossJobId(jobId, TRUE);
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
    if ((CanTSyn_ValidateICVIndication(domainIndex, CANTSYN_SID_ICVGENERATIONINDICATION)) &&
#else
    if (
#endif
        (domainIndex < CANTSYN_GLOBALTIMEDOMAIN_NUMBER))
    {
        CanTSyn_ICVGenarationType* icvGenarationRuntime = &CanTSyn_MasterTimeInfoPtr[domainIndex]->ICVGenaration;
        if (CanTSyn_ICVStatusComparison(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION))
        {
            if (result == E_OK)
            {
                CanTSyn_ICVStatusChange(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION);
                icvGenarationRuntime->AuthenticationBuildAttemptsCounter                = 0u;
                icvGenarationRuntime->AuthenticatedFieldInfo.IsIcvGeneratedSuccessfully = TRUE;
            }
            else if (result == E_NOT_OK)
            {
                /* SWS_CanTSyn_00256 */
                CanTSyn_ICVGenerationBuildFailBehavior(domainIndex, CANTSYN_SEV_TSYN_CAN_ICV_GENERATION_FAILED);
            }
            else
            {
                icvGenarationRuntime->AuthenticationBuildAttemptsCounter++;
                CanTSyn_ICVStatusChange(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_RETRY);
            }
        }
    }
    CanTSyn_ArtiApiReturn(1, CANTSYN_SID_ICVGENERATIONINDICATION);
}
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
/**
 * By this API service the CanTSyn gets an indication and the result of ICV verification.
 */
void CanTSyn_IcvVerificationIndication(uint32 jobId, Crypto_ResultType result)
{
    CanTSyn_ArtiApiStart(3, CANTSYN_SID_ICVVERIFICATIONINDICATION, jobId, result);
    uint8 domainIndex = CanTSyn_FindDomainAcrossJobId(jobId, FALSE);
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
    if (CanTSyn_ValidateICVIndication(domainIndex, CANTSYN_SID_ICVVERIFICATIONINDICATION))
#else
    if (domainIndex != CANTSYN_GLOBALTIMEDOMAIN_NUMBER)
#endif
    {
        CanTSyn_ICVVerificationType* icvVerificationRuntime = &CanTSyn_SlaveTimeInfoPtr[domainIndex]->ICVVerification;
        if (CanTSyn_ICVStatusComparison(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION))
        {
            if (result == E_OK)
            {
                CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_COMFIRMATION);
                icvVerificationRuntime->AuthenticationBuildAttemptsCounter = 0u;
                icvVerificationRuntime->IcvVerificationAttemptsCounter     = 0u;
                CanTSyn_RxFUP(domainIndex, &icvVerificationRuntime->FollowUpDataField[0u]);
            }
            else if (result == E_NOT_OK)
            {
                CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
                IdsM_SetSecurityEventWithContextData(
                    CANTSYN_SEV_TSYN_CAN_ICV_VERIFICATION_FAILED,
                    &CANTSYN_DOMAIN(domainIndex).GlobalTimeDomainId,
                    1u);
#endif
            }
            else
            {
                /* SWS_CanTSyn_00219 */
                CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY);
                icvVerificationRuntime->AuthenticationBuildAttemptsCounter = 0u;
                icvVerificationRuntime->IcvVerificationAttemptsCounter++;
            }
        }
    }
    CanTSyn_ArtiApiReturn(1, CANTSYN_SID_ICVVERIFICATIONINDICATION);
}
#define CANTSYN_STOP_SEC_CODE_FAST
#include "CanTSyn_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */

#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief Development error validation of null pointer
 * @param[in]    apiId   ID of API service in which error is detected
 * @param[in]    pointer pointer
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-77097
 */
CANTSYN_LOCAL boolean CanTSyn_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean result = TRUE;
    if (pointer == NULL_PTR)
    {
        CanTSyn_Det_Report(apiId, CANTSYN_E_NULL_POINTER);
        result = FALSE;
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief Development error validation of Mode
 * @param[in]    mode    Transmit mode
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-77096
 */
CANTSYN_LOCAL boolean CanTSyn_ValidateTxMode(CanTSyn_TransmissionModeType mode)
{
    boolean result = TRUE;
    if ((mode != CANTSYN_TX_OFF) && (mode != CANTSYN_TX_ON))
    {
        CanTSyn_Det_Report(CANTSYN_SID_SETTRANSMISSIONMODE, CANTSYN_E_PARAM);
        result = FALSE;
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief Development error validation of ctrlIdx
 * @param[in]    ctrlIdx Index of the CAN channel
 * @return       boolean
 * @retval       TRUE    Development error not occurred
 * @retval       FALSE   Development error occurred
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-77095
 */
CANTSYN_LOCAL boolean CanTSyn_ValidateCtrlIdx(uint8 ctrlIdx)
{
    boolean result = TRUE;
    if (ctrlIdx >= CANTSYN_GLOBALTIMEDOMAIN_NUMBER)
    {
        CanTSyn_Det_Report(CANTSYN_SID_SETTRANSMISSIONMODE, CANTSYN_E_INV_CTRL_IDX);
        result = FALSE;
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Check if the txPduId of the CanTSyn_TxConfirmation is valid.
 * @param[in]   txPduId  ID of the PDU that has been transmitted
 * @return      boolean
 * @retval      TRUE   Development error not occurred
 * @retval      FALSE  Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-77094
 */
CANTSYN_LOCAL boolean CanTSyn_ValidteTxPduId(PduIdType txPduId)
{
    boolean result = TRUE;
    if ((txPduId >= CANTSYN_GLOBALTIMEDOMAIN_NUMBER) || (CANTSYN_DOMAIN(txPduId).GlobalTimeMaster == NULL_PTR))
    {
        CanTSyn_Det_Report(CANTSYN_SID_TXCONFIRMATION, CANTSYN_E_INVALID_PDUID);
        result = FALSE;
    }

#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (GetApplicationID() != CANTSYN_DOMAIN(txPduId).ApplicationId)
    {
        CanTSyn_Det_Report(CANTSYN_SID_TXCONFIRMATION, CANTSYN_E_INVALID_PDUID);
        result = FALSE;
    }
#endif

    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Check if the rxPduId of the CanTSyn_RxIndication is valid.
 * @param[in]   rxPduId  ID of the PDU that has been received
 * @return      boolean
 * @retval      TRUE   Development error not occurred
 * @retval      FALSE  Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-77093
 */
CANTSYN_LOCAL boolean CanTSyn_ValidteRxPduId(PduIdType rxPduId)
{
    boolean result = TRUE;
    if ((rxPduId >= CANTSYN_GLOBALTIMEDOMAIN_NUMBER) || (CANTSYN_DOMAIN(rxPduId).GlobalTimeSlave == NULL_PTR))
    {
        CanTSyn_Det_Report(CANTSYN_SID_RXINDICATION, CANTSYN_E_INVALID_PDUID);
        result = FALSE;
    }

#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    if (GetApplicationID() != CANTSYN_DOMAIN(rxPduId).ApplicationId)
    {
        CanTSyn_Det_Report(CANTSYN_SID_RXINDICATION, CANTSYN_E_INVALID_PDUID);
        result = FALSE;
    }
#endif

    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Check whether the CanTSyn of the current partition have been initialized.
 * @param[in]   apiId  ID of API service
 * @return      boolean
 * @retval      TRUE   Development error not occurred
 * @retval      FALSE  Development error occurred
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-77092
 */
CANTSYN_LOCAL boolean CanTSyn_ValidteInitStatus(uint8 apiId)
{
    boolean result = TRUE;
    if (CanTSyn_InitStatus[CanTSyn_GetPartitionIndex()] == CANTSYN_UNINIT)
    {
        CanTSyn_Det_Report(apiId, CANTSYN_E_UNINIT);
        result = FALSE;
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

/* slave */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_UnpackType(const uint8* msg)
{
    return msg[CANTSYN_MSG_TYPE_BYTE];
}
CANTSYN_LOCAL_INLINE uint8 CanTSyn_UnpackSequenceCounter(const uint8* msg)
{
    return msg[CANTSYN_MSG_SC_AND_D_BYTE] & CANTSYN_MSG_SC_BIT_MASK;
}
CANTSYN_LOCAL_INLINE uint8 CanTSyn_UnpackDomainId(const uint8* msg)
{
    return (msg[CANTSYN_MSG_SC_AND_D_BYTE] & CANTSYN_MSG_D_BIT_MASK) >> CANTSYN_MSG_D_BIT_POSITION;
}

CANTSYN_LOCAL_INLINE boolean CanTSyn_CheckMsgTypeAndLength(const uint8* msg, PduLengthType length)
{
    boolean success = FALSE;
    if (length >= CANTSYN_STANDARD_MSG_LEN)
    {
        uint8 type = msg[CANTSYN_MSG_TYPE_BYTE];
        success    = (type == CANTSYN_SYNC_NOT_CRC_TYPE) || (type == CANTSYN_SYNC_CRC_TYPE)
                  || (type == CANTSYN_FUP_NOT_CRC_TYPE) || (type == CANTSYN_FUP_CRC_TYPE)
                  || (type == CANTSYN_FUP_NOT_CRC_ICV_SECURED_TYPE) || (type == CANTSYN_FUP_CRC_ICV_SECURED_TYPE);
    }
    return success;
}

CANTSYN_LOCAL_INLINE boolean CanTSyn_CheckMsgSequence(uint8 type, uint8 domainIdx)
{
    CanTSyn_SlaveTimeInfoType* port    = CanTSyn_SlaveTimeInfoPtr[domainIdx];
    boolean                    success = TRUE;

    SchM_Enter_CanTSyn_Context();
    if (port->RxDebounceTimeTicks > 0u)
    {
        /* SWS_CanTSyn_00207 */
        port->FollowUpTimeoutTicks = 0u;
        success                    = FALSE;
    }
    if (((type == CANTSYN_SYNC_NOT_CRC_TYPE) || ((type == CANTSYN_SYNC_CRC_TYPE))) && (port->FollowUpTimeoutTicks > 0u))
    {
        port->FollowUpTimeoutTicks = 0u;
        success                    = FALSE;
    }
    SchM_Exit_CanTSyn_Context();

#ifdef CANTSYN_SEV_TSYN_CAN_MSG_SEQUENCE_ERROR
    uint8 domainId = CANTSYN_DOMAIN(domainIdx).GlobalTimeDomainId;
    if (!success)
    {
        CanTSyn_SecurityEventReporting(CANTSYN_SEV_TSYN_CAN_MSG_SEQUENCE_ERROR, domainId);
    }
#endif

    SchM_Enter_CanTSyn_Context();
    /* follow up message */
    if (((type != CANTSYN_SYNC_NOT_CRC_TYPE) && (type != CANTSYN_SYNC_CRC_TYPE)) && (port->FollowUpTimeoutTicks == 0u))
    {
        success = FALSE;
    }
    SchM_Exit_CanTSyn_Context();

    return success;
}

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Returns the index of current partition in the CanTSyn configuration.
 * @return      The index of current partition in the CanTSyn configuration
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75630
 */
CANTSYN_LOCAL uint16 CanTSyn_GetPartitionIndex(void)
{
    uint16 index = 0u;
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType applicationId = GetApplicationID();
    for (; index < CANTSYN_PARTITION_NUMBER; ++index)
    {
        if (CanTSyn_OsApplicationList[index] == applicationId)
        {
            break;
        }
    }
#endif
    return index;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Check if the GLOBAL_TIME_BASE bit is set.
 * @param[in]   domainIdx  the index of global time domain
 * @return      TRUE: the GLOBAL_TIME_BASE bit is set
 *              FALSE: the GLOBAL_TIME_BASE bit is not set
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75632
 */
CANTSYN_LOCAL boolean CanTSyn_CheckGlobalTimeBaseBit(uint8 domainIdx)
{
    boolean                 result = TRUE;
    StbM_TimeBaseStatusType timeBaseStatus;

    /* get current time base status */
    Std_ReturnType ret = StbM_GetTimeBaseStatus(CANTSYN_DOMAIN(domainIdx).SynchronizedTimeBaseId, &timeBaseStatus);
    if (ret == E_OK)
    {
        /* check if it is a synchronized time base */
        if (CANTSYN_DOMAIN(domainIdx).GlobalTimeDomainId <= CANTSYN_SYNC_FUP_DOMAIN_ID_MAX)
        {
            if ((timeBaseStatus & CANTSYN_GLOBAL_TIME_BASE_MASK) == 0u)
            {
                result = FALSE;
            }
        }
        else /* it is an offset time base */
        {
            result = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Check whether pass CRC validation mode or not.
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   PduInfoPtr the pdu info pointer
 * @return      boolean
 * @retval      TURE       passed CRC validation mode
 * @retval      FALSE      not passed CRC validation mode
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75633
 */
CANTSYN_LOCAL boolean CanTSyn_PassCRCMode(uint8 domainIdx, const PduInfoType* PduInfoPtr)
{
    const CanTSyn_GlobalTimeDomainType* globalTimeDomain = &CANTSYN_DOMAIN(domainIdx);
    uint8                               type             = PduInfoPtr->SduDataPtr[0];
    CrcValidatedType                    crcMode          = globalTimeDomain->GlobalTimeSlave->RxCrcValidated;
    boolean                             result           = TRUE;

    /* check whether it is a CRC message */
    if ((type == CANTSYN_SYNC_CRC_TYPE) || (type == CANTSYN_FUP_CRC_TYPE)
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        || (type == CANTSYN_FUP_CRC_ICV_SECURED_TYPE)
#endif
    )
    {
        if (crcMode == CRC_NOT_VALIDATED)
        {
            result = FALSE;
        }
#if (CANTSYN_CRC_SUPPORT == STD_ON)
        else if ((crcMode == CRC_OPTIONAL) || (crcMode == CRC_VALIDATED)) /* need to validate CRC */
        {
            CanTSyn_PassCRCModeValidate(PduInfoPtr, domainIdx, &result);
        }
#endif
        else
        {
            /* do nothing */
        }
    }
    else
    {
        if (crcMode == CRC_VALIDATED)
        {
            result = FALSE;
        }
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#if (CANTSYN_CRC_SUPPORT == STD_ON)
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Validates CRC for CAN Time Sync messages.
 * @param[in]   PduInfoPtr   the pdu info
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   result    the return value
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-86083
 */
CANTSYN_LOCAL void CanTSyn_PassCRCModeValidate(const PduInfoType* PduInfoPtr, uint8 domainIdx, boolean* result)
{
    const uint8*                        msg              = PduInfoPtr->SduDataPtr;
    uint8                               type             = CanTSyn_UnpackType(msg);
    uint8                               sequenceCounter  = CanTSyn_UnpackSequenceCounter(msg);
    CanTSyn_DataIDListElementType       element          = {0u, 0u};
    const CanTSyn_GlobalTimeDomainType* globalTimeDomain = &CANTSYN_DOMAIN(domainIdx);
    switch (type)
    {
    case CANTSYN_SYNC_CRC_TYPE:
    {
        element = globalTimeDomain->GlobalTimeSyncDataIDList[sequenceCounter];
        break;
    }
    case CANTSYN_FUP_CRC_TYPE:
    case CANTSYN_FUP_CRC_ICV_SECURED_TYPE:
    {
        element = globalTimeDomain->GlobalTimeFupDataIDList[sequenceCounter];
        break;
    }
    default:
        /* do nothing */
        break;
    }

    uint8 CRC = CanTSyn_CalculateCRC(
        PduInfoPtr,
        element
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        ,
        globalTimeDomain->UseExtendedMsgFormat
#endif
    );
    if (CRC != PduInfoPtr->SduDataPtr[1])
    {
        *result = FALSE;
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

#if (CANTSYN_CRC_SUPPORT == STD_ON)
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       calculate CRC for a specific message if CRC is supported.
 * @param[in]   PduInfoPtr              the pdu info
 * @param[in]   element                 the DataID list element
 * @param[in]   useExtendedMsgFormat    whether extended messages are used
 * @return      uint8
 * @retval      CRC result
 * @reentrant   FALSE(not reentrant for hardware CRC, reentrant for software CRC)
 * @synchronous TRUE
 * @trace       CPD-75634
 */
CANTSYN_LOCAL uint8 CanTSyn_CalculateCRC(
    const PduInfoType*            PduInfoPtr,
    CanTSyn_DataIDListElementType element
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    ,
    boolean useExtendedMsgFormat
#endif
)
{
    const uint8* msg       = PduInfoPtr->SduDataPtr;
    uint32       crcLength = PduInfoPtr->SduLength - CANTSYN_MSG_SC_AND_D_BYTE;
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    /* SWS_CanTSyn_00235 SWS_CanTSyn_00237 */
    if (useExtendedMsgFormat && (msg[0u] == CANTSYN_FUP_CRC_ICV_SECURED_TYPE))
    {
        crcLength = CANTSYN_STANDARD_MSG_LEN - CANTSYN_MSG_SC_AND_D_BYTE;
    }
#endif
    uint8 result = Crc_CalculateCRC8H2F(&msg[CANTSYN_MSG_SC_AND_D_BYTE], crcLength, CANTSYN_CRC_DEFAULT_VALUE, TRUE);

    uint8 dataID = element.GlobalTimeDataIDListValue;
    result       = Crc_CalculateCRC8H2F(&dataID, sizeof(dataID), result, FALSE);
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
/* PRQA S 6070 ++ */ /* VL_MTR_CanTSyn_STCAL */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Transmit a SYNC message as a time master.
 * @param[in]   domainIdx the index of global time domain
 * @param[in]   pduId     the pdu id
 * @return      Std_ReturnType
 * @retval      E_OK      Sent successfully
 * @retval      E_NOT_OK  Send failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75636
 */
CANTSYN_LOCAL Std_ReturnType CanTSyn_TransmitSYNC(uint8 domainIdx, PduIdType pduId)
/* PRQA S 6070 -- */
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    StbM_TimeTupleType                  timeTuple;
    StbM_UserDataType                   userData;
    uint8                               userByte[CANTSYN_USERDATA_LEN_MAX] = {0};
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    uint8 sduData[CANTSYN_EXTENDED_MSG_WITH_ICV_LEN] = {0};
#else
    uint8 sduData[CANTSYN_EXTENDED_MSG_LEN] = {0};
#endif
    PduInfoType pduInfo;
    uint8       seq = CanTSyn_SequenceCounters[domainIdx];

    Std_ReturnType result = StbM_GetCurrentTime(timeDomainPtr->SynchronizedTimeBaseId, &timeTuple, &userData);
    if (result == E_OK)
    {
        /* extract userdata based on given length */
        if (userData.userDataLength >= 1u)
        {
            userByte[0] = userData.userByte0;
        }
        if (userData.userDataLength >= 2u)
        {
            userByte[1] = userData.userByte1;
        }
        if (userData.userDataLength == 3u)
        {
            userByte[2] = userData.userByte2;
        }

        /* construct pdu */
        sduData[0] = CANTSYN_SYNC_NOT_CRC_TYPE;
        sduData[1] = userByte[1];
        sduData[2] = (uint8)(timeDomainPtr->GlobalTimeDomainId << 4u) + seq;
        sduData[3] = userByte[0];

        /* retrieve time info */
        uint32 T0SYNC = timeTuple.globalTime.seconds;

        sduData[4] = CanTSyn_HiHiByte(T0SYNC);
        sduData[5] = CanTSyn_HiLoByte(T0SYNC);
        sduData[6] = CanTSyn_LoHiByte(T0SYNC);
        sduData[7] = CanTSyn_LoLoByte(T0SYNC);

        pduInfo.SduDataPtr = sduData;

        CanTSyn_MasterTimeInfoType* masterTimeInfoPtr = CanTSyn_MasterTimeInfoPtr[domainIdx];
        /* store extra time information */
        masterTimeInfoPtr->T0SYNCns = timeTuple.globalTime.nanoseconds;
        masterTimeInfoPtr->T0VLT    = timeTuple.virtualLocalTime.nanosecondsHi;
        masterTimeInfoPtr->T0VLT    = CanTSyn_Shift_Left32(masterTimeInfoPtr->T0VLT);
        masterTimeInfoPtr->T0VLT += timeTuple.virtualLocalTime.nanosecondsLo;
        masterTimeInfoPtr->UserByte2      = userByte[2];
        masterTimeInfoPtr->TimeBaseStatus = timeTuple.timeBaseStatus;

        /* deal with extented format */
        pduInfo.SduLength = timeDomainPtr->UseExtendedMsgFormat ? CANTSYN_EXTENDED_MSG_LEN : CANTSYN_STANDARD_MSG_LEN;
#if (CANTSYN_CRC_SUPPORT == STD_ON)
        if (timeDomainPtr->GlobalTimeMaster->GlobalTimeTxCrcSecured == CRC_SUPPORTED)
        {
            CanTSyn_DataIDListElementType dataIdElement;
            dataIdElement         = timeDomainPtr->GlobalTimeSyncDataIDList[seq];
            pduInfo.SduDataPtr[0] = CANTSYN_SYNC_CRC_TYPE;
            pduInfo.SduDataPtr[1] = CanTSyn_CalculateCRC(
                &pduInfo,
                dataIdElement
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
                ,
                timeDomainPtr->UseExtendedMsgFormat
#endif
            );
        }
#endif
#if (CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON)
        CanIf_EnableEgressTimeStamp(pduId);
#endif
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        if (CANTSYN_DOMAIN(domainIdx).GlobalTimeMaster->GlobalTimeTxIcvSecured == CANTSYN_ICV_SUPPORTED)
        {
            (void)IStdLib_MemCpy(
                &masterTimeInfoPtr->ICVGenaration.SyncPayload[0],
                &pduInfo.SduDataPtr[0],
                CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE);
        }
#endif
        result = CanIf_Transmit(pduId, &pduInfo);
    }

    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
/* PRQA S 3120 -- */

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Sets the PDU Type Header for a FUP message.
 * @param[in]   domainIdx  the index of global time domain
 * @param[out]  pduInfoPtr the pointer of pdu.
 * @return      boolean
 * @retval      E_OK       is domain support Icv
 * @retval      E_NOT_OK   is not domain support Icv
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-86793
 */
CANTSYN_LOCAL boolean CanTSyn_ProcessFUPSecuredTransmit(uint8 domainIdx, const PduInfoType* pduInfoPtr)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    boolean isDomainSupportIcv = timeDomainPtr->GlobalTimeMaster->GlobalTimeTxIcvSecured == CANTSYN_ICV_SUPPORTED;
    if (isDomainSupportIcv)
    {
        if (timeDomainPtr->GlobalTimeMaster->GlobalTimeTxCrcSecured == CRC_SUPPORTED)
        {
            pduInfoPtr->SduDataPtr[0] = CANTSYN_FUP_CRC_ICV_SECURED_TYPE;
        }
        else
        {
            pduInfoPtr->SduDataPtr[0] = CANTSYN_FUP_NOT_CRC_ICV_SECURED_TYPE;
        }
    }
    return isDomainSupportIcv;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

#if (CANTSYN_CRC_SUPPORT == STD_ON)
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Processes Follow-up message CRC transmission
 * @param[in]   domainIdx   the index of global time domain
 * @param[out]  pduInfoPtr  the pointer of pdu.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-86076
 */
CANTSYN_LOCAL void CanTSyn_ProcessFUPCrcTransmit(uint8 domainIdx, const PduInfoType* pduInfoPtr)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    uint8                               seq           = CanTSyn_SequenceCounters[domainIdx];

    if (timeDomainPtr->GlobalTimeMaster->GlobalTimeTxCrcSecured == CRC_SUPPORTED)
    {
        CanTSyn_DataIDListElementType dataIdElement;
        dataIdElement             = timeDomainPtr->GlobalTimeFupDataIDList[seq];
        pduInfoPtr->SduDataPtr[1] = CanTSyn_CalculateCRC(
            pduInfoPtr,
            dataIdElement
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
            ,
            timeDomainPtr->UseExtendedMsgFormat
#endif
        );
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Constructs and initiates the transmission of a Follow-Up (FUP) message.
 * @param[in]   sduDataPtr   the ptr of Service Data Unit
 * @param[in]   domainIdx the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-86070
 */
CANTSYN_LOCAL void CanTSyn_ConstructFUPFrame(uint8* sduDataPtr, uint8 domainIdx)
{
    uint64                              T4             = CanTSyn_MasterTimeInfoPtr[domainIdx]->T4;
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr  = &CANTSYN_DOMAIN(domainIdx);
    uint8                               timeDomainId   = timeDomainPtr->GlobalTimeDomainId;
    StbM_TimeBaseStatusType             timeBaseStatus = CanTSyn_MasterTimeInfoPtr[domainIdx]->TimeBaseStatus;
    uint8                               seq            = CanTSyn_SequenceCounters[domainIdx];
    /* SWS_CanTSyn_00154 write overflow part to OVS */
    uint8 OVS = (uint8)(T4 / CANTSYN_1NS_SECOND);
    T4 -= OVS * CANTSYN_1NS_SECOND;

    /* Calculate SGW */
    if ((timeBaseStatus & CANTSYN_SYNC_TO_GATEWAY_MASK) == 0u) /* sync to global time master */
    {
        sduDataPtr[3] = 0x00u;
    }
    else /* sync to sub domain */
    {
        sduDataPtr[3] = 0x04u;
    }
    sduDataPtr[0] = CANTSYN_FUP_NOT_CRC_TYPE;
    sduDataPtr[1] = CanTSyn_MasterTimeInfoPtr[domainIdx]->UserByte2;
    sduDataPtr[2] = (uint8)(timeDomainId << 4u) + seq;
    sduDataPtr[3] += OVS;
    sduDataPtr[4] = CanTSyn_HiHiByte((uint32)T4);
    sduDataPtr[5] = CanTSyn_HiLoByte((uint32)T4);
    sduDataPtr[6] = CanTSyn_LoHiByte((uint32)T4);
    sduDataPtr[7] = CanTSyn_LoLoByte((uint32)T4);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
/* PRQA S 3120 -- */

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Transmit a FUP message as a time master.
 * @param[in]   domainIdx the index of global time domain
 * @param[in]   pduId     the pdu id
 * @return      Std_ReturnType
 * @retval      E_OK      Sent successfully
 * @retval      E_NOT_OK  Send failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75637
 */
CANTSYN_LOCAL Std_ReturnType CanTSyn_TransmitFUP(uint8 domainIdx, PduIdType pduId)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    uint8 sduData[CANTSYN_EXTENDED_MSG_WITH_ICV_LEN] = {0};
#else
    uint8 sduData[CANTSYN_EXTENDED_MSG_LEN] = {0};
#endif
    Std_ReturnType ret = E_NOT_OK;
    /* construct pdu */
    CanTSyn_ConstructFUPFrame(sduData, domainIdx);

    PduInfoType pduInfo;
    pduInfo.SduDataPtr = sduData;
    /* deal with extented format */
    pduInfo.SduLength = timeDomainPtr->UseExtendedMsgFormat ? CANTSYN_EXTENDED_MSG_LEN : CANTSYN_STANDARD_MSG_LEN;

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    boolean isDomainSupportIcv = CanTSyn_ProcessFUPSecuredTransmit(domainIdx, (const PduInfoType*)&pduInfo);
#endif
    /* deal with CRC format */
#if (CANTSYN_CRC_SUPPORT == STD_ON)
    if (timeDomainPtr->GlobalTimeMaster->GlobalTimeTxCrcSecured == CRC_SUPPORTED
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        && !isDomainSupportIcv
#endif
    )
    {
        pduInfo.SduDataPtr[0] = CANTSYN_FUP_CRC_TYPE;
    }
    CanTSyn_ProcessFUPCrcTransmit(domainIdx, (const PduInfoType*)&pduInfo);
#endif
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    if (isDomainSupportIcv)
    {
        CanTSyn_ICVTxMainfunction(domainIdx, &pduInfo);
    }
    if (CanTSyn_ICVStatusComparison(domainIdx, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION)
        || !isDomainSupportIcv)
#endif
    {
        ret = CanIf_Transmit(pduId, &pduInfo);
    }
    return ret;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
/* PRQA S 3120 -- */

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
/* PRQA S 6070 ++ */ /* VL_MTR_CanTSyn_STCAL */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Receive a SYNC message as a time slave.
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   PduInfoPtr the pdu info pointer
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75652
 */
CANTSYN_LOCAL void CanTSyn_RxSYNC(uint8 domainIdx, const PduInfoType* PduInfoPtr)
/* PRQA S 6070 -- */
{
    CanTSyn_SlaveTimeInfoType* slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];
    const uint8*               msg              = PduInfoPtr->SduDataPtr;
    uint8                      type             = CanTSyn_UnpackType(msg);
    uint8                      sequenceCounter  = CanTSyn_UnpackSequenceCounter(msg);

    Std_ReturnType result = CanTSyn_SequenceCounterCheck(domainIdx, sequenceCounter);

    if (result == E_OK)
    {
        const CanTSyn_GlobalTimeSlaveType* slaveCfgPtr = CANTSYN_DOMAIN(domainIdx).GlobalTimeSlave;
        SchM_Enter_CanTSyn_Context();
        slaveTimeInfoPtr->FollowUpTimeoutTicks =
            (slaveCfgPtr->FollowUpTimeoutTicks > 0u) ? slaveCfgPtr->FollowUpTimeoutTicks : 1u;
        SchM_Exit_CanTSyn_Context();
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        if ((slaveCfgPtr->RxIcvVerification == CANTSYN_ICV_OPTIONAL)
            || (slaveCfgPtr->RxIcvVerification == CANTSYN_ICV_VERIFIED))
        {
            (void)IStdLib_MemCpy(
                &slaveTimeInfoPtr->ICVVerification.SyncPayload[0u],
                msg,
                CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE);
        }
#endif
        if (slaveTimeInfoPtr->FirstlyReceivedSync)
        {
            slaveTimeInfoPtr->FirstlyReceivedSync = FALSE;
        }
        /* extract T0SYNC from pdu info */
        uint32 T0SYNC = msg[4];
        T0SYNC        = CanTSyn_Shift_Left24(T0SYNC);
        uint32 temp   = msg[5];
        temp          = CanTSyn_Shift_Left16(temp);
        T0SYNC += temp;
        temp = msg[6];
        temp = CanTSyn_Shift_Left8(temp);
        T0SYNC += temp;
        T0SYNC += msg[7];

        /* store extra information for follow up messages */
        slaveTimeInfoPtr->T0SYNC          = T0SYNC;
        slaveTimeInfoPtr->UserByte0       = msg[3];
        slaveTimeInfoPtr->SequenceCounter = sequenceCounter;

        /* deal with CRC format */
        if (type == CANTSYN_SYNC_NOT_CRC_TYPE)
        {
            slaveTimeInfoPtr->UserByte1 = PduInfoPtr->SduDataPtr[1];
            slaveTimeInfoPtr->IsCRC     = FALSE;
        }
        else
        {
            slaveTimeInfoPtr->IsCRC = TRUE;
        }
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
/* PRQA S 3120 -- */

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Processes CAN Time Sync follow-up message to update global time.
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   dataPtr the pointer of received data
 * @param[in]   timeTuplePtr the Synchronized timestamp structure.
 * @param[in]   userDataPtr the user data
 * @return      sync time nano seconds
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-86075
 */
CANTSYN_LOCAL uint32 CanTSyn_RxFUPExtractPduInfo(
    uint8               domainIdx,
    const uint8*        dataPtr,
    StbM_TimeTupleType* timeTuplePtr,
    StbM_UserDataType*  userDataPtr)
{
    const CanTSyn_SlaveTimeInfoType* slaveTimeInfoPtr =
        (const CanTSyn_SlaveTimeInfoType*)CanTSyn_SlaveTimeInfoPtr[domainIdx];
    uint32 SyncTimeNSec = dataPtr[4];
    SyncTimeNSec        = CanTSyn_Shift_Left24(SyncTimeNSec);
    uint32 temp         = dataPtr[5];
    temp                = CanTSyn_Shift_Left16(temp);
    SyncTimeNSec += temp;
    temp = dataPtr[6];
    temp = CanTSyn_Shift_Left8(temp);
    SyncTimeNSec += temp;
    SyncTimeNSec += dataPtr[7];

    if ((dataPtr[3] & CANTSYN_SGW_BIT_MASK) == 0x00u)
    {
        timeTuplePtr->timeBaseStatus &= (uint8)~STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
    }
    else
    {
        timeTuplePtr->timeBaseStatus |= STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
    }

    userDataPtr->userByte0      = slaveTimeInfoPtr->UserByte0;
    userDataPtr->userDataLength = 1u;

    if (!slaveTimeInfoPtr->IsCRC)
    {
        userDataPtr->userByte1 = slaveTimeInfoPtr->UserByte1;
        userDataPtr->userDataLength++;
    }
    return SyncTimeNSec;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
/* PRQA S 3120 -- */

/* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Receive a FUP message as a time slave.
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   msg        the pointer of received data
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75654
 */
CANTSYN_LOCAL void CanTSyn_RxFUP(uint8 domainIdx, const uint8* msg)
{
    StbM_SynchronizedTimeBaseType timeBaseId       = CANTSYN_DOMAIN(domainIdx).SynchronizedTimeBaseId;
    CanTSyn_SlaveTimeInfoType*    slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];

    uint8 sequenceCounter = CanTSyn_UnpackSequenceCounter(msg);
    /* check if SC matches */
    if (sequenceCounter == slaveTimeInfoPtr->SequenceCounter)
    {
        StbM_TimeTupleType timeTuple;
        StbM_UserDataType  userData;
        Std_ReturnType     result = StbM_GetCurrentTime(timeBaseId, &timeTuple, &userData);
        if (result == E_OK)
        {
            /* extract information from pdu info */
            uint32 SyncTimeNSec = CanTSyn_RxFUPExtractPduInfo(domainIdx, msg, &timeTuple, &userData);
            uint8  OVS          = (msg[3] & CANTSYN_OVS_BIT_MASK);
            /* deal with CRC format */
            uint8 type = CanTSyn_UnpackType(msg);
            if (type == CANTSYN_FUP_NOT_CRC_TYPE)
            {
                userData.userByte2 = msg[1];
                userData.userDataLength++;
            }
            StbM_TimeStampType tpd              = {0u, 0u, 0u};
            uint64             propagationDelay = CANTSYN_DOMAIN(domainIdx).GlobalTimeSlave->GlobalTimePropagationDelay;
            if (propagationDelay != 0u)
            {
                tpd.seconds = (uint32)(propagationDelay / CANTSYN_1NS_SECOND);
            }
            /* SWS_CanTSyn_00148 */
            timeTuple.globalTime.seconds             = OVS + slaveTimeInfoPtr->T0SYNC + tpd.seconds;
            timeTuple.globalTime.nanoseconds         = SyncTimeNSec;
            timeTuple.virtualLocalTime.nanosecondsLo = (uint32)slaveTimeInfoPtr->T2VLT;
            timeTuple.virtualLocalTime.nanosecondsHi = (uint32)CanTSyn_Shift_Right32(slaveTimeInfoPtr->T2VLT);

            /* set new global time */
            StbM_MeasurementType measurmentData;
            measurmentData.pathDelay = 0u;
            (void)StbM_BusSetGlobalTime(timeBaseId, &timeTuple, &userData, &measurmentData);

#if (CANTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
            /* SWS_CanTSyn_00139 SWS_CanTSyn_00140 */
            if (CANTSYN_DOMAIN(domainIdx).EnableTimeValidation)
            {
                StbM_CanTimeSlaveMeasurementType validationMeasureDataPtr;
                (void)IStdLib_MemSet(&validationMeasureDataPtr, 0u, sizeof(StbM_CanTimeSlaveMeasurementType));
                validationMeasureDataPtr.sequenceCounter = slaveTimeInfoPtr->SequenceCounter;
                validationMeasureDataPtr.segmentId       = CANTSYN_DOMAIN(domainIdx).GlobalTimeNetworkSegmentId;
                validationMeasureDataPtr.syncIngressTimestamp.nanosecondsLo = timeTuple.virtualLocalTime.nanosecondsLo;
                validationMeasureDataPtr.syncIngressTimestamp.nanosecondsHi = timeTuple.virtualLocalTime.nanosecondsHi;
                validationMeasureDataPtr.preciseOriginTimestamp.seconds     = timeTuple.globalTime.seconds;
                validationMeasureDataPtr.preciseOriginTimestamp.nanoseconds = timeTuple.globalTime.nanoseconds;
                validationMeasureDataPtr.pathDelay                          = (uint32)propagationDelay;
                (void)StbM_CanSetSlaveTimingData(timeBaseId, &validationMeasureDataPtr);
            }
#endif
        }
    }
    else
    {
        if (slaveTimeInfoPtr->SequenceCounter == 0u)
        {
            slaveTimeInfoPtr->SequenceCounter = CANTSYN_SC_MAX;
        }
        else
        {
            slaveTimeInfoPtr->SequenceCounter--;
        }
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
/* PRQA S 3120 -- */

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Processes and validates received Sync message
 * @param[in]    domainIdx    The index of global time domain
 * @param[in]    PduInfoPtr   Contains the length (SduLength) of the received PDU, a pointer to a buffer
 * (SduDataPtr) containing   The PDU, and the MetaData related to this PDU.
 * @param[in]    localTime    The current virtual local time
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-86073
 */
CANTSYN_LOCAL void
    CanTSyn_ProcessSyncRxPdu(uint8 domainIdx, const PduInfoType* PduInfoPtr, StbM_VirtualLocalTimeType localTime)
{
    CanTSyn_SlaveTimeInfoType* slaveVarPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];
    uint64                     t1vlt       = localTime.nanosecondsHi;
    t1vlt                                  = CanTSyn_Shift_Left32(t1vlt);
    t1vlt += localTime.nanosecondsLo;
#if (CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON)
    if (CANTSYN_DOMAIN(domainIdx).IsTimeBaseSourceSameAsTransmitter)
#endif
    {
        /* SWS_CanTSyn_00144 SWS_CanTSyn_00146 */
        slaveVarPtr->T2VLT = t1vlt;
    }
#if (CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON)
    else
    {
        /* SWS_CanTSyn_00147 */
        Can_TimeStampType         tCurrentTimeCan;
        StbM_VirtualLocalTimeType t5virtualLocalTime;

        SchM_Enter_CanTSyn_Context();
        if ((CanIf_GetCurrentTime(domainIdx, &tCurrentTimeCan) != E_OK)
            || (StbM_GetCurrentVirtualLocalTime(CANTSYN_DOMAIN(domainIdx).SynchronizedTimeBaseId, &t5virtualLocalTime)
                != E_OK))
        {
            SchM_Exit_CanTSyn_Context();
            return;
        }
        SchM_Exit_CanTSyn_Context();
        uint64 t3vlt = (tCurrentTimeCan.seconds * CANTSYN_1NS_SECOND) + tCurrentTimeCan.nanoseconds;
        uint64 t5vlt = CanTSyn_Shift_Left32(t5virtualLocalTime.nanosecondsHi) + t5virtualLocalTime.nanosecondsLo;
        t5vlt += t5virtualLocalTime.nanosecondsLo;

        uint64 tsrd        = t3vlt - t1vlt;
        slaveVarPtr->T2VLT = t5vlt - tsrd;
    }
#endif
    CanTSyn_RxSYNC(domainIdx, PduInfoPtr);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Process the received Pdu data packets.
 * @param[in]    domainIdx    The index of global time domain
 * @param[in]    PduInfoPtr   Contains the length (SduLength) of the received PDU, a pointer to a buffer
 * (SduDataPtr) containing   The PDU, and the MetaData related to this PDU.
 * @param[in]    slaveCfgPtr The time salve of the global time domain refered by domainIdx.
 * @param[in]    localTime    The current virtual local time
 * @reentrant    TRUE(Reentrant for different PduIds. Non reentrant for the same PduId.)
 * @synchronous  TRUE
 * @trace        CPD-75655
 */
CANTSYN_LOCAL void CanTSyn_RxPduInfoProcess(
    uint8                              domainIdx,
    const PduInfoType*                 PduInfoPtr,
    const CanTSyn_GlobalTimeSlaveType* slaveCfgPtr,
    StbM_VirtualLocalTimeType          localTime)
{
    const uint8* msg  = PduInfoPtr->SduDataPtr;
    uint8        type = CanTSyn_UnpackType(msg);
    if (!CanTSyn_CheckMsgSequence(type, domainIdx))
    {
        return;
    }

    CanTSyn_SlaveTimeInfoType* slaveVarPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];
    SchM_Enter_CanTSyn_Context();
    slaveVarPtr->RxDebounceTimeTicks = slaveCfgPtr->RxDebounceTimeTicks;
    SchM_Exit_CanTSyn_Context();

    if (CanTSyn_PassCRCMode(domainIdx, PduInfoPtr)
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        && (CanTSyn_VerifyTimeAuthenticatedSubTlv(domainIdx, PduInfoPtr) == E_OK)
#endif
    )
    {
        switch (type)
        {
        case CANTSYN_SYNC_NOT_CRC_TYPE:
        case CANTSYN_SYNC_CRC_TYPE:
            CanTSyn_ProcessSyncRxPdu(domainIdx, PduInfoPtr, localTime);
            break;
        case CANTSYN_FUP_NOT_CRC_TYPE:
        case CANTSYN_FUP_CRC_TYPE:
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        case CANTSYN_FUP_NOT_CRC_ICV_SECURED_TYPE:
        case CANTSYN_FUP_CRC_ICV_SECURED_TYPE:
#endif
            SchM_Enter_CanTSyn_Context();
            slaveVarPtr->FollowUpTimeoutTicks = 0u;
            SchM_Exit_CanTSyn_Context();

            CanTSyn_RxFUP(domainIdx, msg);
            break;
        default:
            /* do nothing */
            break;
        }
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Handle the situation where Master is in CANTSYN_SYNC_REQUIRED state.
 * @param[in]   domainIdx  the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75656
 */
CANTSYN_LOCAL void CanTSyn_MasterSyncRequired(uint8 domainIdx)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    const CanTSyn_GlobalTimeMasterType* timeMasterPtr = timeDomainPtr->GlobalTimeMaster;
    const CanTSyn_MasterTimeInfoType*   timeInfoPtr   = CanTSyn_MasterTimeInfoPtr[domainIdx];

    /* SWS_CanTSyn_00125 */
    if ((CanTSyn_TransmissionMode[domainIdx] == CANTSYN_TX_ON)
        && (timeInfoPtr->DebounceCounter < CANTSYN_MAIN_FUNCTION_PERIOD)
#if (CANTSYN_SYNC_TRANSMISSION_PER_CYCLE > 0u)
        && (CanTSyn_SyncTxCounter < CANTSYN_SYNC_TRANSMISSION_PER_CYCLE)
#endif
    )
    {
        /* Start trasnmitting */
        if ((timeDomainPtr->GlobalTimeDomainId <= CANTSYN_SYNC_FUP_DOMAIN_ID_MAX)
            && (CanTSyn_TransmitSYNC(domainIdx, timeMasterPtr->GlobalTimePduId) == E_OK))
        {
            CanTSyn_MasterStatus[domainIdx] = CANTSYN_SYNC_SENT;
#if (CANTSYN_SYNC_TRANSMISSION_PER_CYCLE > 0u)
            /* SWS_CanTSyn_00230 */
            CanTSyn_SyncTxCounter++;
#endif
        }
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Handle the situation where Master is in CANTSYN_FUP_REQUIRED state.
 * @param[in]   domainIdx  the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75657
 */
CANTSYN_LOCAL void CanTSyn_MasterFupRequired(uint8 domainIdx)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    const CanTSyn_GlobalTimeMasterType* timeMasterPtr = timeDomainPtr->GlobalTimeMaster;
    const CanTSyn_MasterTimeInfoType*   timeInfoPtr   = CanTSyn_MasterTimeInfoPtr[domainIdx];

    /* SWS_CanTSyn_00032 */
    if ((CanTSyn_TransmissionMode[domainIdx] == CANTSYN_TX_ON)
        && (timeInfoPtr->DebounceCounter < CANTSYN_MAIN_FUNCTION_PERIOD))
    {
        /* Start Transimitting */
        if ((timeDomainPtr->GlobalTimeDomainId <= CANTSYN_SYNC_FUP_DOMAIN_ID_MAX)
            && (CanTSyn_TransmitFUP(domainIdx, timeMasterPtr->GlobalTimePduId) == E_OK))
        {
            CanTSyn_MasterStatus[domainIdx] = CANTSYN_SYNC_NOT_REQUIRED;
        }
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Handle the situation where Master is in CANTSYN_SYNC_NOT_REQUIRED state.
 * @param[in]   domainIdx  the index of global time domain
 * @param[in]   isGlobalTimeBaseBitSet  boolean indicating whether the GLOBAL_TIME_BASE bit is set
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75658
 */
CANTSYN_LOCAL void CanTSyn_MasterSyncNotRequired(uint8 domainIdx, boolean isGlobalTimeBaseBitSet)
{
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    const CanTSyn_GlobalTimeMasterType* timeMasterPtr = timeDomainPtr->GlobalTimeMaster;

    if (timeMasterPtr->ImmediateTimeSync)
    {
        /* check whether update counter has changed */
        uint8 baseCounterCurrValue = StbM_GetTimeBaseUpdateCounter(timeDomainPtr->SynchronizedTimeBaseId);
        if (baseCounterCurrValue != CanTSyn_BaseCounterValues[domainIdx])
        {
            CanTSyn_BaseCounterValues[domainIdx] = baseCounterCurrValue;
            /* check whether GLOBAL_TIME_BASE bit is set */
            if (isGlobalTimeBaseBitSet)
            {
                CanTSyn_MasterStatus[domainIdx]  = CANTSYN_SYNC_REQUIRED;
                CanTSyn_ImmediateSent[domainIdx] = TRUE;
            }
        }
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Deal with time master main functionality.
 * @param[in]   domainIdx  the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-75662
 */
CANTSYN_LOCAL void CanTSyn_MasterMain(uint8 domainIdx)
{
    CanTSyn_MasterTimeInfoType* timeInfoPtr = CanTSyn_MasterTimeInfoPtr[domainIdx];
    CanTSyn_MasterStatusType    masterStatus;
    boolean                     result = CanTSyn_CheckGlobalTimeBaseBit(domainIdx);

    if (CanTSyn_MasterStatus[domainIdx] == CANTSYN_SYNC_NOT_REQUIRED)
    {
        CanTSyn_MasterSyncNotRequired(domainIdx, result);
    }

    /* periodic time sync counter operation */
    if (result && (timeInfoPtr->PeriodCounter != 0UL))
    {
        masterStatus = CANTSYN_SYNC_REQUIRED;
        CanTSyn_DecrementCounter(&timeInfoPtr->PeriodCounter, domainIdx, &masterStatus);
    }

    /* cyclic resume counter operation */
    if (timeInfoPtr->CyclicResumeCounter != 0UL)
    {
        masterStatus = CANTSYN_SYNC_REQUIRED;
        CanTSyn_DecrementCounter(&timeInfoPtr->CyclicResumeCounter, domainIdx, &masterStatus);
        if ((timeInfoPtr->CyclicResumeCounter == 0UL) && !result)
        {
            CanTSyn_MasterStatus[domainIdx] = CANTSYN_SYNC_NOT_REQUIRED;
        }
    }
    /* SWS_CanTSyn_00231 */
    if (timeInfoPtr->DebounceCounter >= CANTSYN_MAIN_FUNCTION_PERIOD)
    {
        timeInfoPtr->DebounceCounter -= CANTSYN_MAIN_FUNCTION_PERIOD;
    }

    switch (CanTSyn_MasterStatus[domainIdx])
    {
    case CANTSYN_SYNC_REQUIRED:
    {
        CanTSyn_MasterSyncRequired(domainIdx);
        break;
    }
    case CANTSYN_FUP_REQUIRED:
    {
        CanTSyn_MasterFupRequired(domainIdx);
        break;
    }
    default:
        /* do nothing */
        break;
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Deal with time slave main functionality.
 * @param[in]   domainIdx  the index of global time domain
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-86086
 */
CANTSYN_LOCAL void CanTSyn_SlaveMain(uint8 domainIdx)
{
    const CanTSyn_GlobalTimeSlaveType* slaveCfgPtr = CANTSYN_DOMAIN(domainIdx).GlobalTimeSlave;
    CanTSyn_SlaveTimeInfoType*         slaveVarPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];
    SchM_Enter_CanTSyn_Context();
    if (slaveVarPtr->RxDebounceTimeTicks > 0u)
    {
        slaveVarPtr->RxDebounceTimeTicks--;
    }
    if ((slaveCfgPtr->FollowUpTimeoutTicks > 0u) && (slaveVarPtr->FollowUpTimeoutTicks > 0u))
    {
        slaveVarPtr->FollowUpTimeoutTicks--;
    }
    SchM_Exit_CanTSyn_Context();

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    if (slaveCfgPtr->GlobalTimeRxIcvVerification != NULL_PTR)
    {
        CanTSyn_ICVRxMainfunction(domainIdx);
    }
#endif
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief         Decrement counter value.
 * @param[in,out] counter      the counter to decrement
 * @param[in]     domainIdx    the index of global time domain
 * @param[in]     masterStatus master target status
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-75665
 */
CANTSYN_LOCAL void
    CanTSyn_DecrementCounter(uint32* counter, uint8 domainIdx, const CanTSyn_MasterStatusType* masterStatus)
{
    if (*counter <= CANTSYN_MAIN_FUNCTION_PERIOD)
    {
        if ((CanTSyn_MasterStatus[domainIdx] != CANTSYN_FUP_REQUIRED) && (NULL_PTR != masterStatus))
        {
            CanTSyn_MasterStatus[domainIdx] = *masterStatus;
            *counter                        = 0u;
        }
    }
    else
    {
        *counter -= CANTSYN_MAIN_FUNCTION_PERIOD;
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Shift a 32bit integer to the left by 8 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint32
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75666
 */
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left8(uint32 x)
{
    return x << CANTSYN_SHIFT_BIT8;
}

/**
 * @brief       Shift a 32bit integer to the left by 16 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint32
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75667
 */
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left16(uint32 x)
{
    return x << CANTSYN_SHIFT_BIT16;
}

/**
 * @brief       Shift a 32bit integer to the left by 24 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint32
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75668
 */
CANTSYN_LOCAL_INLINE uint32 CanTSyn_Shift_Left24(uint32 x)
{
    return x << CANTSYN_SHIFT_BIT24;
}

/**
 * @brief       Shift a 64bit integer to the left by 32 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint64
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75669
 */
CANTSYN_LOCAL_INLINE uint64 CanTSyn_Shift_Left32(uint64 x)
{
    return x << CANTSYN_SHIFT_BIT32;
}

/**
 * @brief       Shift a 64bit integer to the right by 32 bits.
 * @param[in]   x  Integer to be operated on
 * @return      uint64
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75670
 */
CANTSYN_LOCAL_INLINE uint64 CanTSyn_Shift_Right32(uint64 x)
{
    return x >> CANTSYN_SHIFT_BIT32;
}

/**
 * @brief       Get the 24-31 bits of a 32bit integer.
 * @param[in]   x  Integer to be operated on
 * @return      uint8
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75671
 */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_HiHiByte(uint32 x)
{
    return (uint8)(x >> CANTSYN_SHIFT_BIT24);
}

/**
 * @brief       Get the 16-23 bits of a 32bit integer.
 * @param[in]   x  Integer to be operated on
 * @return      uint8
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75672
 */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_HiLoByte(uint32 x)
{
    return (uint8)(x >> CANTSYN_SHIFT_BIT16);
}

/**
 * @brief       Get the 8-15 bits of a 32bit integer.
 * @param[in]   x  Integer to be operated on
 * @return      uint8
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75673
 */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_LoHiByte(uint32 x)
{
    return (uint8)(x >> CANTSYN_SHIFT_BIT8);
}

/**
 * @brief       Get the 0-8 bits of a 32bit integer.
 * @param[in]   x  Integer to be operated on
 * @return      uint8
 * @retval      The result after the shift
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75674
 */
CANTSYN_LOCAL_INLINE uint8 CanTSyn_LoLoByte(uint32 x)
{
    return (uint8)x;
}

#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief       Report a CanTSyn diagnostic error.
 * @param[in]   apiId   The application ID
 * @param[in]   errorId The error ID
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-75675
 */
CANTSYN_LOCAL_INLINE void CanTSyn_Det_Report(uint8 apiId, uint8 errorId)
{
    (void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID, apiId, errorId);
}
#endif

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Validates the Sequence Counter jump result on the slave side.
 * @param[in]   timeBaseStatus   The time base status.
 * @param[in]   domainIdx   The domain index.
 * @param[in]   sequenceCounter The sequence counter.
 * @return      boolean
 * @retval      TURE      SequenceCounter is jump valid.
 * @retval      FALSE     SequenceCounter is jump invalid.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-85941
 */
CANTSYN_LOCAL boolean
    CanTSyn_IsSequenceCounterJumpInvalid(StbM_TimeBaseStatusType timeBaseStatus, uint8 domainIdx, uint8 sequenceCounter)
{
    boolean                             result           = FALSE;
    const CanTSyn_SlaveTimeInfoType*    slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr    = &CANTSYN_DOMAIN(domainIdx);
    uint8 maxJumpWidth = timeDomainPtr->GlobalTimeSlave->GlobalTimeSequenceCounterJumpWidth;

#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
    uint8 configSequenceCounterHysteresis = timeDomainPtr->GlobalTimeSlave->GlobalTimeSequenceCounterHysteresis;
#endif

    if (((timeBaseStatus & CANTSYN_TIMEOUT_MASK) == 0u
#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
         || (slaveTimeInfoPtr->SequenceCounterHysteresis != (configSequenceCounterHysteresis + 1u))
#endif
             )
        && (((sequenceCounter > slaveTimeInfoPtr->SequenceCounter)
             && ((sequenceCounter - slaveTimeInfoPtr->SequenceCounter) > maxJumpWidth))
            || ((sequenceCounter < slaveTimeInfoPtr->SequenceCounter)
                && ((CANTSYN_SEQUENCE_COUNTER_MAX_VALUE - slaveTimeInfoPtr->SequenceCounter + sequenceCounter + 1u)
                    > maxJumpWidth))))
    {
        result = TRUE;
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       Evaluates Sequence Counter validity and hysteresis state.
 * @param[in]   domainIdx                       The domain index.
 * @param[in]   timeBaseStatus                  The time base status.
 * @param[in]   isSequenceCounterJumpInvalid    Is sequence counter jump Invalid
 * @param[in]   sequenceCounter                 The sequence counter.
 * @return      boolean
 * @retval      TURE      Check sequence counter conditions valid.
 * @retval      FALSE     Check sequence counter conditions invalid.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-85933
 */
CANTSYN_LOCAL Std_ReturnType CanTSyn_CheckSequenceCounterConditions(
    uint8 domainIdx,
#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
    StbM_TimeBaseStatusType timeBaseStatus,
#endif
    boolean isSequenceCounterJumpInvalid,
    uint8   sequenceCounter)
{
#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
    CanTSyn_SlaveTimeInfoType*          slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr    = &CANTSYN_DOMAIN(domainIdx);
    uint8 configSequenceCounterHysteresis = timeDomainPtr->GlobalTimeSlave->GlobalTimeSequenceCounterHysteresis;
#else
    const CanTSyn_SlaveTimeInfoType* slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];
#endif
    Std_ReturnType ret = E_OK;
    if (isSequenceCounterJumpInvalid == TRUE)
    {
        ret = E_NOT_OK;
#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
        if ((((timeBaseStatus & CANTSYN_TIMEOUT_MASK) != 0u))
            && (slaveTimeInfoPtr->SequenceCounterHysteresis != (configSequenceCounterHysteresis + 1u)))
        {
            slaveTimeInfoPtr->SequenceCounterHysteresis = configSequenceCounterHysteresis + 1u;
        }
#endif
    }
    else if (sequenceCounter == slaveTimeInfoPtr->SequenceCounter)
    {
        ret = E_NOT_OK;
    }
#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
    /* Sequence Counter Hysteresis */
    /* SWS_CanTSyn_00227 */
    else if (((timeBaseStatus & CANTSYN_TIMEOUT_MASK) != 0u))
    {
        slaveTimeInfoPtr->SequenceCounterHysteresis--;
        if (slaveTimeInfoPtr->SequenceCounterHysteresis != 0u)
        {
            ret = E_NOT_OK;
        }
        else
        {
            slaveTimeInfoPtr->SequenceCounterHysteresis = configSequenceCounterHysteresis + 1u;
        }
    }
#endif
    else
    {
        /* do nothing */
    }
    return ret;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief       The implementation of checking sequence counter.
 * @param[in]   domainIdx   The domain index.
 * @param[in]   sequenceCounter The sequence counter.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-84629
 */
CANTSYN_LOCAL Std_ReturnType CanTSyn_SequenceCounterCheck(uint8 domainIdx, uint8 sequenceCounter)
{
    Std_ReturnType                      ret = E_OK;
    StbM_TimeBaseStatusType             timeBaseStatus;
    const CanTSyn_GlobalTimeDomainType* timeDomainPtr = &CANTSYN_DOMAIN(domainIdx);
    StbM_SynchronizedTimeBaseType       timeBaseId    = timeDomainPtr->SynchronizedTimeBaseId;
    uint8 maxJumpWidth = timeDomainPtr->GlobalTimeSlave->GlobalTimeSequenceCounterJumpWidth;

    const CanTSyn_SlaveTimeInfoType* slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIdx];

    if ((maxJumpWidth > 0u) && (!slaveTimeInfoPtr->FirstlyReceivedSync))
    {
        /* Check whether sequence counter diffrence goes over maxJumpWidth if this message is not firstly received
        and Sequence Counter Jump Width Check enabled*/
        if (StbM_GetTimeBaseStatus(timeBaseId, &timeBaseStatus) != E_OK)
        {
            ret = E_NOT_OK;
        }
        else
        {
            boolean isSequenceCounterJumpInvalid =
                CanTSyn_IsSequenceCounterJumpInvalid(timeBaseStatus, domainIdx, sequenceCounter);
            /* SWS_CanTSyn_00223 */
            ret = CanTSyn_CheckSequenceCounterConditions(
                domainIdx,
#if (CANTSYN_SEQUENCE_COUNTER_HYSTERESIS_ENABLED == STD_ON)
                timeBaseStatus,
#endif
                isSequenceCounterJumpInvalid,
                sequenceCounter);
        }
    }
    return ret;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
/**
 * @brief       Converts the number of bits to the corresponding number of bytes.
 * @param[in]   nbits   : The number of bits to be converted.
 * @retval      uint16 : The number of bytes equivalent to the given number of bits.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-84313
 */
CANTSYN_LOCAL_INLINE uint16 CanTSyn_BitToByte(uint16 nbits)
{
    return (((nbits) + CANTSYN_CONST_7U)) >> (CANTSYN_CONST_3U);
}
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Obtaining the freshness value of TX.
 * @param[in]    domainIndex : domain index.
 * @param[out]   freshnessValue : freshness value.
 * @param[out]   truncatedFreshnessValue : truncated freshness value.
 * @retval       E_OK: obtaining the freshness value of TX successfully.
 * @retval       E_NOT_OK: obtaining the freshness value of TX failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-84314
 */
CANTSYN_LOCAL Std_ReturnType
    CanTSyn_TxFreshnessObtain(uint8 domainIndex, uint8* freshnessValue, uint8* truncatedFreshnessValue)
{
    const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration;
    uint32         freshnessValueTruncLength = icvGeneration->IcvTxFreshnessValueTruncLength;
    uint32         freshnessValueLength      = icvGeneration->IcvTxFreshnessValueLength;
    Std_ReturnType ret                       = E_NOT_OK;
    /* SWS_CanTSyn_00163 */
    if (freshnessValueTruncLength == freshnessValueLength)
    {
        ret = StbM_GetTxFreshness(icvGeneration->IcvGenerationFvIdRef, freshnessValue, &freshnessValueTruncLength);
        (void)IStdLib_MemCpy(
            truncatedFreshnessValue,
            freshnessValue,
            CanTSyn_BitToByte((uint16)freshnessValueTruncLength));
    }
    /* SWS_CanTSyn_00164 */
    else if (freshnessValueTruncLength < freshnessValueLength)
    {
        ret = StbM_GetTxFreshnessTruncData(
            icvGeneration->IcvGenerationFvIdRef,
            freshnessValue,
            &freshnessValueLength,
            truncatedFreshnessValue,
            &freshnessValueTruncLength);
    }
    else
    {
        /* do nothing */
    }
    return ret;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the implementation of ICV generation at Master domain.
 * @param[in]    domainIndex : domain index.
 * @param[in]    generationBuffer : Contains the pointer to the data for which the MAC shall be computed.
 * @param[in]    dataLength : the length of generationBuffer.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84315
 */
CANTSYN_LOCAL void CanTSyn_ICVGeneration(uint8 domainIndex, const uint8* generationBuffer, uint8 dataLength)
{
    Std_ReturnType                               ret = E_NOT_OK;
    const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration;
    CanTSyn_MasterTimeInfoType* masterTimeInfoPtr    = CanTSyn_MasterTimeInfoPtr[domainIndex];
    CanTSyn_ICVGenarationType*  icvGenarationRuntime = &masterTimeInfoPtr->ICVGenaration;
    uint32                      icvLength            = icvGeneration->IcvTxLength;
    switch (icvGeneration->TxICVGenerationConfig.IcvGenerationBase)
    {
    case CANTSYN_ICV_MAC:
        ret = Csm_MacGenerate(
            icvGeneration->TxICVGenerationConfig.IcvGenerationJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            &generationBuffer[0u],
            dataLength,
            &icvGenarationRuntime->AuthenticatedFieldInfo.ICVBuffer[0u],
            &icvLength);
        break;
    case CANTSYN_ICV_SIGNATURE:
        ret = Csm_SignatureGenerate(
            icvGeneration->TxICVGenerationConfig.IcvGenerationJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            &generationBuffer[0u],
            dataLength,
            &icvGenarationRuntime->AuthenticatedFieldInfo.ICVBuffer[0u],
            &icvLength);
        break;
    default:
        ret = E_NOT_OK;
        break;
    }

    if (ret == E_OK)
    {
        if (icvGeneration->TxICVGenerationConfig.IcvProcessingType == CANTSYN_CRYPTO_PROCESSING_SYNC)
        {
            CanTSyn_ICVStatusChange(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION);
            icvGenarationRuntime->AuthenticationBuildAttemptsCounter                = 0u;
            icvGenarationRuntime->AuthenticatedFieldInfo.IsIcvGeneratedSuccessfully = TRUE;
        }
        else if (icvGeneration->IcvGenerationTimeout != 0u)
        {
            CanTSyn_ICVStatusChange(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION);
            StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(domainIndex).SynchronizedTimeBaseId;
            StbM_VirtualLocalTimeType     VirtualLocalTime;
            if (StbM_GetCurrentVirtualLocalTime(timeBaseId, &VirtualLocalTime) == E_OK)
            {
                icvGenarationRuntime->IcvIndicationStartTime =
                    (uint64)(((uint64)VirtualLocalTime.nanosecondsHi << CANTSYN_SHIFT_BIT32)
                             | (uint64)VirtualLocalTime.nanosecondsLo);
            }
        }
        else
        {
            CanTSyn_ICVStatusChange(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION);
        }
    }
    else if (ret == E_NOT_OK)
    {
        CanTSyn_ICVGenerationBuildFailBehavior(domainIndex, CANTSYN_SEV_TSYN_CAN_ICV_GENERATION_FAILED);
    }
    else if (ret == CRYPTO_E_BUSY)
    {
        icvGenarationRuntime->AuthenticationBuildAttemptsCounter++;
        CanTSyn_ICVStatusChange(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_RETRY);
    }
    else
    {
        /* do nothing */
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the mainfunction of ICV generation.
 * @param[in]    domainIndex : domain index.
 * @param[out]   pduInfo : the pointer of pdu.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84316
 */
CANTSYN_LOCAL void CanTSyn_ICVTxMainfunction(uint8 domainIndex, PduInfoType* pduInfo)
{
    const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration;

    const CanTSyn_MasterTimeInfoType* masterTimeInfoPtr    = CanTSyn_MasterTimeInfoPtr[domainIndex];
    const CanTSyn_ICVGenarationType*  icvGenarationRuntime = &masterTimeInfoPtr->ICVGenaration;

    if (CanTSyn_ICVStatusComparison(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_BUILDING)
        || CanTSyn_ICVStatusComparison(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_RETRY))
    {
        CanTSyn_StartICVGeneration(domainIndex, pduInfo);
    }

    if (CanTSyn_ICVStatusComparison(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION))
    {
        if (icvGeneration->IcvGenerationTimeout != 0u)
        {
            StbM_VirtualLocalTimeType     virtualLocalTime;
            StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(domainIndex).SynchronizedTimeBaseId;

            if (StbM_GetCurrentVirtualLocalTime(timeBaseId, &virtualLocalTime) == E_OK)
            {
                if (((((uint64)((uint64)virtualLocalTime.nanosecondsHi << CANTSYN_SHIFT_BIT32)
                       | (uint64)virtualLocalTime.nanosecondsLo)
                      - icvGenarationRuntime->IcvIndicationStartTime)
                     > icvGeneration->IcvGenerationTimeout))
                {
                    CanTSyn_ICVGenerationBuildFailBehavior(domainIndex, CANTSYN_SEV_TSYN_CAN_ICV_GENERATION_FAILED);
                }
            }
        }
    }

    if (CanTSyn_ICVStatusComparison(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION))
    {
        CanTSyn_SubTlvAuthenticatedBuild(domainIndex, pduInfo);
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Prepares the data buffer and triggers ICV generation for Secure Time Sync.
 * @param[in]    domainIndex                domain index.
 * @param[in]    pduInfoPtr                 the pointer of pdu.
 * @param[in]    freshnessValuePtr          the pointer of freshness value.
 * @param[in]    truncatedFreshnessValuePtr the pointer of truncated freshness value.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85937
 */
CANTSYN_LOCAL void CanTSyn_GenerateIcvWithFreshnessValue(
    uint8              domainIndex,
    const PduInfoType* pduInfoPtr,
    const uint8* const freshnessValuePtr,
    const uint8* const truncatedFreshnessValuePtr)
{
    uint8                                        trunctedFvLength = 0u;
    const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration;
    trunctedFvLength = (uint8)CanTSyn_BitToByte((uint16)icvGeneration->IcvTxFreshnessValueTruncLength);
    CanTSyn_MasterTimeInfoType*          masterTimeInfoPtr      = CanTSyn_MasterTimeInfoPtr[domainIndex];
    CanTSyn_ICVGenarationType*           icvGenarationRuntime   = &masterTimeInfoPtr->ICVGenaration;
    CanTSyn_AuthenticatedFieldInofoType* authenticatedFieldInfo = &icvGenarationRuntime->AuthenticatedFieldInfo;
    authenticatedFieldInfo->FvLength                            = trunctedFvLength;
    (void)IStdLib_MemCpy(&authenticatedFieldInfo->FreshValue[0u], &truncatedFreshnessValuePtr[0u], trunctedFvLength);
    /* FV obtain successfully then build input data buffer for generation of ICV */
    uint8 icvGenerationBuffer[CANTSYN_ICV_BUILD_BUFFER_MAX];
    (void)IStdLib_MemSet(&icvGenerationBuffer[0u], 0u, CANTSYN_ICV_BUILD_BUFFER_MAX);
    /* copy SYNC paload */
    (void)IStdLib_MemCpy(
        &icvGenerationBuffer[0u],
        &icvGenarationRuntime->SyncPayload[0u],
        CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE);
    /* copy FUP paload except Time Authenticated */
    (void)IStdLib_MemCpy(
        &icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE],
        &pduInfoPtr->SduDataPtr[0u],
        CANTSYN_FUP_EXCEPT_AUTHENTICATED_PAYLOAD_SIZE);
    /* FreshnessValueLength in bits */
    icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] =
        (uint8)icvGeneration->IcvTxFreshnessValueTruncLength;
    /* ICV length in bytes */
    icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_LENGTH_OFFSET] =
        (uint8)icvGeneration->IcvTxLength;
    /* include Full FV in ICV generationBuffer*/
    (void)IStdLib_MemCpy(
        &icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_OFFSET],
        &freshnessValuePtr[0u],
        (uint8)CanTSyn_BitToByte((uint16)icvGeneration->IcvTxFreshnessValueLength));
    uint8 dataLength = CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_OFFSET
                       + (uint8)CanTSyn_BitToByte((uint16)icvGeneration->IcvTxFreshnessValueLength);
    CanTSyn_ICVGeneration(domainIndex, (const uint8*)icvGenerationBuffer, dataLength);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Processes the Freshness Value for Secure Time Synchronization (ICV).
 * @param[in]    domainIndex : domain index.
 * @param[in]    pduInfoPtr : the pointer of pdu.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85938
 */
CANTSYN_LOCAL void CanTSyn_ProcessFreshnessValue(uint8 domainIndex, const PduInfoType* pduInfoPtr)
{
    uint8                       freshnessValue[CANTSYN_MAX_FRESHNESS_VALUE_SIZE];
    uint8                       truncatedFreshnessValue[CANTSYN_MAX_FRESHNESS_VALUE_SIZE];
    CanTSyn_MasterTimeInfoType* masterTimeInfoPtr    = CanTSyn_MasterTimeInfoPtr[domainIndex];
    CanTSyn_ICVGenarationType*  icvGenarationRuntime = &masterTimeInfoPtr->ICVGenaration;
    Std_ReturnType              ret                  = E_OK;

    ret = CanTSyn_TxFreshnessObtain(domainIndex, freshnessValue, truncatedFreshnessValue);

    if (ret == E_OK)
    {
        CanTSyn_GenerateIcvWithFreshnessValue(domainIndex, pduInfoPtr, freshnessValue, truncatedFreshnessValue);
    }
    else if (ret == E_NOT_OK)
    {
        /* SWS_CanTSyn_00168 */
        CanTSyn_ICVGenerationBuildFailBehavior(domainIndex, CANTSYN_SEV_TSYN_CAN_FRESHNESS_NOT_AVAILABLE);
        (void)Det_ReportRuntimeError(
            CANTSYN_MODULE_ID,
            CANTSYN_INSTANCE_ID,
            CANTSYN_SID_MAINFUNCTION,
            CANTSYN_E_FRESHNESSFAILURE);
    }
    else if (ret == E_BUSY)
    {
        /* retry build*/
        icvGenarationRuntime->AuthenticationBuildAttemptsCounter++;
        CanTSyn_ICVStatusChange(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_RETRY);
    }
    else
    {
        /* do nothing */
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the action of ICV generation when status is CANTSYN_ICV_TX_PROGRESS_STATUS_BUILDING
 *               or CANTSYN_ICV_TX_PROGRESS_STATUS_RETRY.
 * @param[in]    domainIndex : domain index.
 * @param[out]   pduInfoPtr : the pointer of pdu.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84317
 */
CANTSYN_LOCAL void CanTSyn_StartICVGeneration(uint8 domainIndex, const PduInfoType* pduInfoPtr)
{
    const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration;

    CanTSyn_MasterTimeInfoType* masterTimeInfoPtr    = CanTSyn_MasterTimeInfoPtr[domainIndex];
    CanTSyn_ICVGenarationType*  icvGenarationRuntime = &masterTimeInfoPtr->ICVGenaration;

    CanTSyn_AuthenticatedFieldInofoType* authenticatedFieldInfo = &icvGenarationRuntime->AuthenticatedFieldInfo;

    uint8 trunctedFvLength           = (uint8)CanTSyn_BitToByte((uint16)icvGeneration->IcvTxFreshnessValueTruncLength);
    authenticatedFieldInfo->FvLength = trunctedFvLength;

    if (icvGenarationRuntime->AuthenticationBuildAttemptsCounter == icvGeneration->TxAuthenticationBuildAttempts)
    {
        icvGenarationRuntime->AuthenticationBuildAttemptsCounter = 0u;
        CanTSyn_ICVGenerationBuildFailBehavior(domainIndex, CANTSYN_SEV_TSYN_CAN_ICV_GENERATION_FAILED);
    }
    else if (trunctedFvLength != 0u)
    {
        CanTSyn_ProcessFreshnessValue(domainIndex, pduInfoPtr);
    }
    /* SWS_CanTSyn_00165 */
    else
    {
        /* build input data buffer for generation of ICV withou FV */
        uint8 icvGenerationBuffer[CANTSYN_ICV_BUILD_BUFFER_MAX];
        (void)IStdLib_MemSet(&icvGenerationBuffer[0u], 0u, CANTSYN_ICV_BUILD_BUFFER_MAX);
        /* copy SYNC payload */
        (void)IStdLib_MemCpy(
            &icvGenerationBuffer[0u],
            &icvGenarationRuntime->SyncPayload[0u],
            CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE);
        /* copy FUP payload except Time Authenticated */
        (void)IStdLib_MemCpy(
            &icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE],
            &pduInfoPtr->SduDataPtr[0u],
            CANTSYN_FUP_EXCEPT_AUTHENTICATED_PAYLOAD_SIZE);
        /* FreshnessValueLength in bits */
        icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] = 0u;
        /* ICV length in bytes */
        icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_LENGTH_OFFSET] =
            (uint8)icvGeneration->IcvTxLength;

        uint8 dataLength = CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_OFFSET;
        CanTSyn_ICVGeneration(domainIndex, (const uint8*)icvGenerationBuffer, dataLength);
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the implementation of packing the Sub-Tlv Time Authenticated.
 * @param[in]    domainIndex : domain index.
 * @param[out]   pduInfo : the pointer of pdu.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84318
 */
CANTSYN_LOCAL void CanTSyn_SubTlvAuthenticatedBuild(uint8 domainIndex, PduInfoType* pduInfo)
{
    const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration;

    CanTSyn_MasterTimeInfoType* masterTimeInfoPtr    = CanTSyn_MasterTimeInfoPtr[domainIndex];
    CanTSyn_ICVGenarationType*  icvGenarationRuntime = &masterTimeInfoPtr->ICVGenaration;

    const CanTSyn_AuthenticatedFieldInofoType* authenticatedFieldInfo = &icvGenarationRuntime->AuthenticatedFieldInfo;
    boolean isIcvGeneratedSuccessfully = icvGenarationRuntime->AuthenticatedFieldInfo.IsIcvGeneratedSuccessfully;
    icvGenarationRuntime->AuthenticatedFieldInfo.IsIcvGeneratedSuccessfully = FALSE;

    uint8* data = pduInfo->SduDataPtr;

    if (authenticatedFieldInfo->FvLength != 0u)
    {
        uint8 trunctedFvLength = (uint8)CanTSyn_BitToByte((uint16)icvGeneration->IcvTxFreshnessValueTruncLength);
        /* FreshnessValueLength in bits */
        data[CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] = (uint8)icvGeneration->IcvTxFreshnessValueTruncLength;
        /* ICV length in bytes */
        data[CANTSYN_ICV_LENGTH_OFFSET] = (uint8)icvGeneration->IcvTxLength;
        /* FV */
        (void)IStdLib_MemCpy(
            &data[CANTSYN_ICV_FRESHNESS_VALUE_OFFSET],
            &authenticatedFieldInfo->FreshValue[0u],
            trunctedFvLength);
        /* ICV with FV*/
        (void)IStdLib_MemCpy(
            &data[CANTSYN_ICV_FRESHNESS_VALUE_OFFSET + trunctedFvLength],
            authenticatedFieldInfo->ICVBuffer,
            icvGeneration->IcvTxLength);
        pduInfo->SduLength =
            (PduLengthType)((uint32)CANTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH
                            + (uint32)CanTSyn_BitToByte((uint16)icvGeneration->IcvTxFreshnessValueLength)
                            + (uint32)icvGeneration->IcvTxLength);
    }
    else if ((icvGeneration->IcvTxFreshnessValueTruncLength == 0u) && isIcvGeneratedSuccessfully)
    {
        /* FreshnessValueLength in bits */
        data[CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] = 0u;
        /* ICV length in bytes */
        data[CANTSYN_ICV_LENGTH_OFFSET] = (uint8)icvGeneration->IcvTxLength;
        /* ICV without FV */
        (void)IStdLib_MemCpy(
            &data[CANTSYN_ICV_LENGTH_OFFSET + 1u],
            authenticatedFieldInfo->ICVBuffer,
            icvGeneration->IcvTxLength);
        pduInfo->SduLength = (uint32)CANTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH + icvGeneration->IcvTxLength;
    }
    else
    {
        /* FreshnessValueLength in bits */
        data[CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] = 0u;
        /* ICV length in bytes */
        data[CANTSYN_ICV_LENGTH_OFFSET] = 0u;
        pduInfo->SduLength              = CANTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH;
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the implementation of changing the status of ICV generation or verification.
 * @param[in]    domainIndex : the index of domain.
 * @param[in]    isTimeMaster : specifying if the domain configured as Time Master.
 * @param[in]    toState : the new status of ICV generation or verification.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84319
 */
CANTSYN_LOCAL void CanTSyn_ICVStatusChange(uint8 domainIndex, boolean isTimeMaster, uint8 toState)
{
    if (isTimeMaster)
    {
        CanTSyn_MasterTimeInfoType* masterTimeInfoPtr      = CanTSyn_MasterTimeInfoPtr[domainIndex];
        masterTimeInfoPtr->ICVGenaration.IcvProgressStatus = toState;
    }
    else
    {
        CanTSyn_SlaveTimeInfoType* slaveTimeInfoPtr         = CanTSyn_SlaveTimeInfoPtr[domainIndex];
        slaveTimeInfoPtr->ICVVerification.IcvProgressStatus = toState;
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the implementation of comparing the status of ICV generation or verification.
 * @param[in]    domainIndex : the index of domain.
 * @param[in]    isTimeMaster : specifying if the domain configured as Time Master.
 * @param[in]    State : the state to be compared with the current state of ICV generation or verification.
 * @retval       TRUE: the current state is State.
 * @retval       FALSE: the current state is not State.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84320
 */
CANTSYN_LOCAL boolean CanTSyn_ICVStatusComparison(uint8 domainIndex, boolean isTimeMaster, uint8 State)
{
    boolean result = FALSE;
    if (isTimeMaster)
    {
        const CanTSyn_MasterTimeInfoType* masterTimeInfoPtr = CanTSyn_MasterTimeInfoPtr[domainIndex];
        if (masterTimeInfoPtr->ICVGenaration.IcvProgressStatus == State)
        {
            result = TRUE;
        }
    }
    else
    {
        const CanTSyn_SlaveTimeInfoType* slaveTimeInfoPtr = CanTSyn_SlaveTimeInfoPtr[domainIndex];
        if (slaveTimeInfoPtr->ICVVerification.IcvProgressStatus == State)
        {
            result = TRUE;
        }
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

/**
 * @brief        the behavior when ICV generation failed.
 * @param[in]    domainIndex : the index of domain.
 * @param[in]    eventId : the event id of ICV generation failed.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84321
 */
CANTSYN_LOCAL_INLINE void CanTSyn_ICVGenerationBuildFailBehavior(uint8 domainIndex, uint16 eventId)
{
    CanTSyn_MasterTimeInfoType* masterTimeInfoPtr    = CanTSyn_MasterTimeInfoPtr[domainIndex];
    CanTSyn_ICVGenarationType*  icvGenarationRuntime = &masterTimeInfoPtr->ICVGenaration;
    /* there will be no FV and ICV*/
    icvGenarationRuntime->AuthenticatedFieldInfo.FvLength                   = 0u;
    icvGenarationRuntime->AuthenticatedFieldInfo.IsIcvGeneratedSuccessfully = FALSE;

    CanTSyn_ICVStatusChange(domainIndex, TRUE, CANTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION);

#if (CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
    IdsM_SetSecurityEventWithContextData(eventId, &CANTSYN_DOMAIN(domainIndex).GlobalTimeDomainId, 1u);
#else
    (void)eventId;
#endif
}

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        find the index of domain which is related to the CSM jobId.
 * @param[in]    jobId Holds the identifier of the job using the CSM service.
 * @param[in]    isTimeMaster : specifying if the domain configured as Time Master.
 * @return       the index of domain,which is related to the jobId.
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-84322
 */
CANTSYN_LOCAL uint8 CanTSyn_FindDomainAcrossJobId(uint32 jobId, boolean isTimeMaster)
{
    uint8 domainIndex = 0u;
    if (isTimeMaster)
    {
        for (domainIndex = 0u; domainIndex < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; ++domainIndex)
        {
            if (CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster != NULL_PTR)
            {
                const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
                    CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration;
                if ((icvGeneration != NULL_PTR) && (icvGeneration->TxICVGenerationConfig.IcvGenerationJobRef == jobId))
                {
                    break;
                }
            }
        }
    }
    else
    {
        for (domainIndex = 0u; domainIndex < CANTSYN_GLOBALTIMEDOMAIN_NUMBER; ++domainIndex)
        {
            if (CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave != NULL_PTR)
            {
                const CanTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
                    CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification;
                if ((icvVerification != NULL_PTR)
                    && (icvVerification->RxICVVerificationConfig.IcvVerificationJobRef == jobId))
                {
                    break;
                }
            }
        }
    }
    return domainIndex;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the implementation of validating of CanTSyn_IcvGenerationIndication and
 *               CanTSyn_IcvVerificationIndication.
 * @param[in]    domainIndex : the index of domain.
 * @param[in]    isTimeMaster : specifying if the domain configured as Time Master.
 * @param[in]    apiId  ID of API service in which error is detected.
 * @retval       TRUE   validation passed.
 * @retval       FALSE  validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-84323
 */
CANTSYN_LOCAL boolean CanTSyn_ValidateICVIndication(uint8 domainIndex, uint8 apiId)
{
    boolean result = TRUE;
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON && CANTSYN_PARTITION_NUMBER != 0
    if (CanTSyn_GetPartitionIndex() >= CANTSYN_PARTITION_NUMBER)
    {
        CanTSyn_Det_Report(CANTSYN_SID_INIT, CANTSYN_E_INVALID_PARTITION);
        result = FALSE;
    }
    else
#endif
        if (domainIndex == CANTSYN_GLOBALTIMEDOMAIN_NUMBER)
    {
        (void)Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID, apiId, CANTSYN_E_PARAM);
        result = FALSE;
    }
    else
    {
        /* do nothing */
    }
    return result;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Time
 * Authenticated. But not do the ICV verifying.
 * @param[in]     domainIndex: the index of domain.
 * @param[in]     pduInfoPtr: the pointer of received pdu.
 * @retval        E_OK:  validation passed.
 * @retval        E_NOT_OK: validation failed.
 * @synchronous   TRUE
 * @reentrant     TRUE
 * @trace        CPD-84324
 */
CANTSYN_LOCAL Std_ReturnType CanTSyn_VerifyTimeAuthenticatedSubTlv(uint8 domainIndex, const PduInfoType* pduInfoPtr)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Std_ReturnType ret = E_OK;

    uint8                         type = pduInfoPtr->SduDataPtr[0u];
    CanTSyn_RxIcvVerificationType rxIcvVerificationType =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->RxIcvVerification;

    if ((type == CANTSYN_FUP_NOT_CRC_ICV_SECURED_TYPE) || (type == CANTSYN_FUP_CRC_ICV_SECURED_TYPE))
    {
        switch (rxIcvVerificationType)
        {
        /* SWS_CanTSyn_00178 */
        case CANTSYN_ICV_VERIFIED:
        case CANTSYN_ICV_OPTIONAL:
        {
            if (CANTSYN_DOMAIN(domainIndex).UseExtendedMsgFormat)
            {
                CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
                CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;
                CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START);

                uint8 pduLength = (uint8)pduInfoPtr->SduLength;
                (void)CanTSyn_CircularBufferWrite(
                    &icvVerificationRuntime->RxICVBufferToken,
                    &icvVerificationRuntime->SyncPayload[0u],
                    CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE);
                (void)CanTSyn_CircularBufferWrite(&icvVerificationRuntime->RxICVBufferToken, &pduLength, 1u);
                (void)CanTSyn_CircularBufferWrite(
                    &icvVerificationRuntime->RxICVBufferToken,
                    &pduInfoPtr->SduDataPtr[0u],
                    (uint16)pduInfoPtr->SduLength);

                ret = CANTSYN_E_VALIDATE_ICV;
            }
            else
            {
                ret = E_NOT_OK;
            }

            break;
        }
        case CANTSYN_ICV_IGNORED:
        {
            ret = E_OK;
            break;
        }
        case CANTSYN_ICV_NOT_VERIFIED:
        {
            ret = E_NOT_OK;
            break;
        }
        default:
            ret = E_NOT_OK;
            break;
        }
    }
    else if (
        ((type == CANTSYN_FUP_NOT_CRC_TYPE) || (type == CANTSYN_FUP_CRC_TYPE))
        && (rxIcvVerificationType == CANTSYN_ICV_VERIFIED))
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* do nothing */
    }

    return ret;
    /* PRQA S 3120 -- */
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the mainfunction of ICV verification at slave domain.
 * @param[in]    domainIndex : the index of domain.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84325
 */
CANTSYN_LOCAL void CanTSyn_ICVRxMainfunction(uint8 domainIndex)
{
    const CanTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification;

    const CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
    const CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;
    /* Rxindication validates the Sub-Tlv Time Authenticated successfully --->
     * (CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL ---> CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START) */
    if (CanTSyn_ICVStatusComparison(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START)
        || CanTSyn_ICVStatusComparison(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY))
    {
        CanTSyn_StartICVVerification(domainIndex);
    }
    /* csm job is configured to synchronou and obtain the FV succesfully --->
     * (CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START--->CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION)
     */
    if (CanTSyn_ICVStatusComparison(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION))
    {
        if (icvVerification->IcvVerificationTimeout != 0u)
        {
            StbM_VirtualLocalTimeType     virtualLocalTime;
            StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(domainIndex).SynchronizedTimeBaseId;
            if (StbM_GetCurrentVirtualLocalTime(timeBaseId, &virtualLocalTime) == E_OK)
            {
                if (((uint64)((uint64)((uint64)virtualLocalTime.nanosecondsHi << CANTSYN_SHIFT_BIT32)
                              | (uint64)virtualLocalTime.nanosecondsLo)
                     - icvVerificationRuntime->IcvIndicationStartTime)
                    > icvVerification->IcvVerificationTimeout)
                {
                    /* SWS_CanTSyn_00200 */
                    CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
                    IdsM_SetSecurityEventWithContextData(
                        CANTSYN_SEV_TSYN_CAN_ICV_VERIFICATION_FAILED,
                        &CANTSYN_DOMAIN(domainIndex).GlobalTimeDomainId,
                        1u);
#endif
                }
            }
        }
    }

    if (CanTSyn_ICVStatusComparison(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED))
    {
        CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL);
    }

    if (CanTSyn_ICVStatusComparison(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_COMFIRMATION))
    {
        CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL);
        if (icvVerificationRuntime->RxVerifyResult == CRYPTO_E_VER_OK)
        {
            CanTSyn_RxFUP(domainIndex, &icvVerificationRuntime->FollowUpDataField[0u]);
        }
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Assembles the verification data block and triggers the ICV check.
 * @param[in]    domainIndex        the index of domain.
 * @param[in]    receivedFvValue    the received fresh value
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85934
 */
CANTSYN_LOCAL void CanTSyn_ConstructAndVerifyIcv(uint8 domainIndex, const uint8* receivedFvValue)
{
    const CanTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification;
    const CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
    const CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;
    /* FV obtain successfully*/
    uint8 icvGenerationBuffer[CANTSYN_ICV_BUILD_BUFFER_MAX];
    (void)IStdLib_MemSet(&icvGenerationBuffer[0u], 0u, CANTSYN_ICV_BUILD_BUFFER_MAX);
    /* copy SYNC paload */
    (void)IStdLib_MemCpy(
        &icvGenerationBuffer[0u],
        &icvVerificationRuntime->SyncPayload[0u],
        CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE);
    /* copy FUP paload except Time Authenticated */
    (void)IStdLib_MemCpy(
        &icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE],
        &icvVerificationRuntime->FollowUpDataField[0u],
        CANTSYN_FUP_EXCEPT_AUTHENTICATED_PAYLOAD_SIZE);
    /* FreshnessValueLength in bits */
    icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] =
        (uint8)icvVerification->IcvRxFreshnessValueTruncLength;
    /* ICV length in bytes */
    icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_LENGTH_OFFSET] =
        (uint8)icvVerification->IcvRxLength;
    /* include Full FV in ICV generationBuffer*/
    (void)IStdLib_MemCpy(
        &icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_OFFSET],
        &receivedFvValue[0u],
        (uint8)CanTSyn_BitToByte((uint16)icvVerification->IcvRxFreshnessValueLength));
    uint8 dataLength = CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_OFFSET
                       + (uint8)CanTSyn_BitToByte((uint16)icvVerification->IcvRxFreshnessValueLength);
    CanTSyn_ICVVerification(domainIndex, (const uint8*)icvGenerationBuffer, dataLength);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Initiates the ICV verification process on the Slave side.
 * @param[in]    domainIndex : the index of domain.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85936
 */
CANTSYN_LOCAL void CanTSyn_VerifyIcvWithFreshnessValue(uint8 domainIndex)
{
    Std_ReturnType               ret = E_OK;
    uint8                        receivedFvValue[CANTSYN_MAX_FRESHNESS_VALUE_SIZE];
    CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
    CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;
    ret                                                 = CanTSyn_RxFreshnessObtain(domainIndex, receivedFvValue);
    if (ret == E_NOT_OK)
    {
        /* SWS_CanTSyn_00193 */
        CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
        IdsM_SetSecurityEventWithContextData(
            CANTSYN_SEV_TSYN_CAN_FRESHNESS_NOT_AVAILABLE,
            &CANTSYN_DOMAIN(domainIndex).GlobalTimeDomainId,
            1u);
#endif
        (void)Det_ReportRuntimeError(
            CANTSYN_MODULE_ID,
            CANTSYN_INSTANCE_ID,
            CANTSYN_SID_MAINFUNCTION,
            CANTSYN_E_FRESHNESSFAILURE);
    }
    else if (ret == E_OK)
    {
        CanTSyn_ConstructAndVerifyIcv(domainIndex, receivedFvValue);
    }
    else if (ret == E_BUSY)
    {
        /* retry build*/
        /* SWS_CanTSyn_00217 */
        icvVerificationRuntime->AuthenticationBuildAttemptsCounter++;
        CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY);
    }
    else
    {
        /* do nothing */
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Triggers ICV verification for sequences not using a Freshness Value.
 * @param[in]    domainIndex : the index of domain.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85935
 */
CANTSYN_LOCAL void CanTSyn_VerifyIcvWithoutFreshnessValue(uint8 domainIndex)
{
    const CanTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification;
    const CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
    const CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;
    /* SWS_CanTSyn_00189 */
    /* build input data buffer for generation of ICV withou FV */
    uint8 icvGenerationBuffer[CANTSYN_ICV_BUILD_BUFFER_MAX];
    (void)IStdLib_MemSet(&icvGenerationBuffer[0u], 0u, CANTSYN_ICV_BUILD_BUFFER_MAX);
    /* copy SYNC payload */
    (void)IStdLib_MemCpy(
        &icvGenerationBuffer[0u],
        &icvVerificationRuntime->SyncPayload[0u],
        CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE);
    /* copy FUP payload except Time Authenticated */
    (void)IStdLib_MemCpy(
        &icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE],
        &icvVerificationRuntime->FollowUpDataField[0u],
        CANTSYN_FUP_EXCEPT_AUTHENTICATED_PAYLOAD_SIZE);
    /* FreshnessValueLength in bits */
    icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] = 0u;
    /* ICV length in bytes */
    icvGenerationBuffer[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_LENGTH_OFFSET] =
        (uint8)icvVerification->IcvRxLength;

    uint8 dataLength = CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE + CANTSYN_ICV_FRESHNESS_VALUE_OFFSET;
    CanTSyn_ICVVerification(domainIndex, (const uint8*)icvGenerationBuffer, dataLength);
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the action of ICV generation when status is CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START
 *               or CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY.
 * @param[in]    domainIndex : the index of domain.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84326
 */
CANTSYN_LOCAL void CanTSyn_StartICVVerification(uint8 domainIndex)
{
    const CanTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification;

    CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
    CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;
    if (CanTSyn_ICVStatusComparison(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START))
    {
        uint8 dataLength = 0u;
        (void)CanTSyn_CircularBufferRead(
            &icvVerificationRuntime->RxICVBufferToken,
            &icvVerificationRuntime->SyncPayload[0u],
            CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE);
        (void)CanTSyn_CircularBufferRead(&icvVerificationRuntime->RxICVBufferToken, &dataLength, 1u);
        (void)CanTSyn_CircularBufferRead(
            &icvVerificationRuntime->RxICVBufferToken,
            &icvVerificationRuntime->FollowUpDataField[0u],
            dataLength);
    }

    uint8 authenticatedBuildCounter   = (uint8)icvVerificationRuntime->AuthenticationBuildAttemptsCounter;
    uint8 verificationAttemptsCounter = (uint8)icvVerificationRuntime->IcvVerificationAttemptsCounter;

    uint8 receivedFvLength = icvVerificationRuntime->FollowUpDataField[CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET];

    if ((authenticatedBuildCounter == icvVerification->RxAuthenticationBuildAttempts)
        || (verificationAttemptsCounter == icvVerification->IcvVerificationAttempts)
        || ((icvVerification->IcvRxFreshnessValueTruncLength != 0u) && (receivedFvLength == 0u))
        || ((icvVerification->IcvRxFreshnessValueTruncLength == 0u) && (receivedFvLength != 0u)))
    {
        icvVerificationRuntime->AuthenticationBuildAttemptsCounter = 0u;
        icvVerificationRuntime->IcvVerificationAttemptsCounter     = 0u;
        /* SWS_CanTSyn_00200 */
        CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
        IdsM_SetSecurityEventWithContextData(
            CANTSYN_SEV_TSYN_CAN_ICV_VERIFICATION_FAILED,
            &CANTSYN_DOMAIN(domainIndex).GlobalTimeDomainId,
            1u);
#endif
    }
    else if (icvVerification->IcvRxFreshnessValueTruncLength != 0u)
    {
        CanTSyn_VerifyIcvWithFreshnessValue(domainIndex);
    }
    else
    {
        CanTSyn_VerifyIcvWithoutFreshnessValue(domainIndex);
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        Obtaining the freshness value of RX.
 * @param[in]    domainIndex : the ptr of FULL freshness value.
 * @param[out]   fvValue : the ptr of FULL freshness value.
 * @retval       E_OK: obtaining the freshness value of TX successfully.
 * @retval       E_NOT_OK: obtaining the freshness value of TX failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-84327
 */
CANTSYN_LOCAL Std_ReturnType CanTSyn_RxFreshnessObtain(uint8 domainIndex, uint8* fvValue)
{
    Std_ReturnType                                 ret = E_NOT_OK;
    const CanTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification;

    const CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
    const CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;

    const uint8* dataPtr = icvVerificationRuntime->FollowUpDataField;

    uint32       truncatedFreshnessValueLength = (uint32)dataPtr[CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET];
    const uint8* receicedFvValuePtr            = &dataPtr[CANTSYN_ICV_FRESHNESS_VALUE_OFFSET];

    uint32 configuredFreshnessValueLen = icvVerification->IcvRxFreshnessValueLength;
    /* SWS_CanTSyn_00188 */
    ret = StbM_GetRxFreshness(
        (uint16)icvVerification->IcvVerificationFvIdRef,
        receicedFvValuePtr,
        truncatedFreshnessValueLength,
        icvVerificationRuntime->IcvVerificationAttemptsCounter,
        fvValue,
        &configuredFreshnessValueLen);

    return ret;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        the implementation of ICV verification at Slave domain.
 * @param[in]    domainIndex : the ptr of FULL freshness value.
 * @param[in]    generationBuffer : Contains the pointer to the data for which the MAC shall be computed.
 * @param[in]    dataLength : the length of raw data to be verified.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-84328
 */
CANTSYN_LOCAL void CanTSyn_ICVVerification(uint8 domainIndex, const uint8* generationBuffer, uint8 dataLength)
{
    Std_ReturnType                                 ret = E_NOT_OK;
    const CanTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification;

    CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
    CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;

    const uint8* receivedICV =
        &icvVerificationRuntime->FollowUpDataField
             [CANTSYN_ICV_FRESHNESS_VALUE_OFFSET
              + (((uint8)CanTSyn_BitToByte(
                     icvVerificationRuntime->FollowUpDataField[CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET]))
                 & CANTSYN_FVL_BIT_MASK)];

    switch (icvVerification->RxICVVerificationConfig.IcvVerificationBase)
    {
    case CANTSYN_ICV_MAC:
        ret = Csm_MacVerify(
            icvVerification->RxICVVerificationConfig.IcvVerificationJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            &generationBuffer[0u],
            dataLength,
            receivedICV,
            icvVerification->IcvRxLength,
            &icvVerificationRuntime->RxVerifyResult);
        break;
    case CANTSYN_ICV_SIGNATURE:
        ret = Csm_SignatureVerify(
            icvVerification->RxICVVerificationConfig.IcvVerificationJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            &generationBuffer[0u],
            dataLength,
            receivedICV,
            (uint32)CanTSyn_BitToByte((uint16)icvVerification->IcvRxLength),
            &icvVerificationRuntime->RxVerifyResult);
        break;
    default:
        ret = E_NOT_OK;
        break;
    }

    if (ret == E_OK)
    {
        if (icvVerification->RxICVVerificationConfig.IcvProcessingType == CANTSYN_CRYPTO_PROCESSING_SYNC)
        {
            CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_COMFIRMATION);
            icvVerificationRuntime->IcvVerificationAttemptsCounter     = 0u;
            icvVerificationRuntime->AuthenticationBuildAttemptsCounter = 0u;
        }
        else if (icvVerification->IcvVerificationTimeout != 0u)
        {
            CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION);
            StbM_VirtualLocalTimeType     VirtualLocalTime;
            StbM_SynchronizedTimeBaseType timeBaseId = CANTSYN_DOMAIN(domainIndex).SynchronizedTimeBaseId;
            if (StbM_GetCurrentVirtualLocalTime(timeBaseId, &VirtualLocalTime) == E_OK)
            {
                /* SWS_CanTSyn_00197 */
                icvVerificationRuntime->IcvIndicationStartTime =
                    (uint64)(((uint64)VirtualLocalTime.nanosecondsHi << CANTSYN_SHIFT_BIT32)
                             | (uint64)VirtualLocalTime.nanosecondsLo);
            }
        }
        else
        {
            CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION);
        }
    }
    else if (ret == E_NOT_OK)
    {
        /* SWS_CanTSyn_00200 */
        CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
        IdsM_SetSecurityEventWithContextData(
            CANTSYN_SEV_TSYN_CAN_ICV_VERIFICATION_FAILED,
            &CANTSYN_DOMAIN(domainIndex).GlobalTimeDomainId,
            1u);
#endif
    }
    else if (ret == CRYPTO_E_BUSY)
    {
        /* SWS_CanTSyn_00199 */
        icvVerificationRuntime->AuthenticationBuildAttemptsCounter++;
        CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY);
    }
    else
    {
        /* SWS_CanTSyn_00219 */
        icvVerificationRuntime->IcvVerificationAttemptsCounter++;
        icvVerificationRuntime->AuthenticationBuildAttemptsCounter = 0u;
        CanTSyn_ICVStatusChange(domainIndex, FALSE, CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY);
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief         The implementation of initializing the ICV runtime variables.
 * @param[in]     domainIndex: the index of domain.
 * @param[in]     isTimeMaster : specifying if the domain configured as Time Master.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace        CPD-84329
 */
CANTSYN_LOCAL void CanTSyn_InitPortICVRuntimeVariables(uint8 domainIndex)
{
    if (CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster != NULL_PTR)
    {
        if (CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration != NULL_PTR)
        {
            CanTSyn_MasterTimeInfoType* masterTimeInfoPtr    = CanTSyn_MasterTimeInfoPtr[domainIndex];
            CanTSyn_ICVGenarationType*  icvGenarationRuntime = &masterTimeInfoPtr->ICVGenaration;

            const CanTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
                CANTSYN_DOMAIN(domainIndex).GlobalTimeMaster->GlobalTimeTxIcvGeneration;

            (void)IStdLib_MemSet(
                &icvGenarationRuntime->AuthenticatedFieldInfo.ICVBuffer[0u],
                0u,
                icvGeneration->IcvTxLength);
            (void)IStdLib_MemSet(
                &icvGenarationRuntime->AuthenticatedFieldInfo,
                0u,
                sizeof(CanTSyn_AuthenticatedFieldInofoType));
            icvGenarationRuntime->TimeAuthencatedSequenceCounter = 0u;
            icvGenarationRuntime->IcvProgressStatus              = CANTSYN_ICV_TX_PROGRESS_STATUS_BUILDING;
            icvGenarationRuntime->AuthenticatedFieldInfo.IsIcvGeneratedSuccessfully = FALSE;
            icvGenarationRuntime->IcvIndicationStartTime                            = 0u;
            icvGenarationRuntime->AuthenticationBuildAttemptsCounter                = 0u;
        }
    }
    if (CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave != NULL_PTR)
    {
        if ((CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification != NULL_PTR))
        {
            CanTSyn_SlaveTimeInfoType*   slaveTimeInfoPtr       = CanTSyn_SlaveTimeInfoPtr[domainIndex];
            CanTSyn_ICVVerificationType* icvVerificationRuntime = &slaveTimeInfoPtr->ICVVerification;

            const CanTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
                CANTSYN_DOMAIN(domainIndex).GlobalTimeSlave->GlobalTimeRxIcvVerification;

            (void)IStdLib_MemSet(&icvVerificationRuntime->FollowUpDataField[0u], 0u, CANTSYN_ICV_BUILD_BUFFER_MAX);
            icvVerificationRuntime->TimeAuthenticatedFieldOffset       = 0u;
            icvVerificationRuntime->IcvVerificationAttemptsCounter     = 0u;
            icvVerificationRuntime->IcvIndicationStartTime             = 0u;
            icvVerificationRuntime->AuthenticationBuildAttemptsCounter = 0u;
            icvVerificationRuntime->IcvProgressStatus = CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL;
            /* init circular buffer */
            icvVerificationRuntime->RxICVBufferToken.Buffer   = icvVerification->RxIcvCircularBuffer;
            icvVerificationRuntime->RxICVBufferToken.Capacity = CANTSYN_ICV_CIRCULAR_BUFFER_SIZE;
            icvVerificationRuntime->RxICVBufferToken.Head     = 0u;
            icvVerificationRuntime->RxICVBufferToken.Tail     = 0u;
            icvVerificationRuntime->RxICVBufferToken.Count    = 0u;
        }
    }
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

/**
 * @brief         The implementation of checking the circular buffer is valid.
 * @param[in]     cb: circular buffer table pointer.
 * @retval        TRUE:  validation passed.
 * @retval        FALSE: validation failed.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace        CPD-84330
 */
CANTSYN_LOCAL_INLINE boolean CanTSyn_IsBufferValid(const CanTSyn_CircularBufferType* cb)
{
    boolean isValid = FALSE;
    if ((cb != NULL_PTR) && (cb->Buffer != NULL_PTR) && (cb->Capacity > 0u))
    {
        isValid = TRUE;
    }
    return isValid;
}

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief         The implementation of writing data to the circular buffer.
 * @param[inout]  cb: circular buffer table pointer.
 * @param[in]     data: data pointer.
 * @param[in]     size: data size.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace        CPD-84331
 */
CANTSYN_LOCAL uint16 CanTSyn_CircularBufferWrite(CanTSyn_CircularBufferType* cb, const uint8* data, uint16 size)
{
    uint16 dataSize = size;

    if (!CanTSyn_IsBufferValid(cb) || (data == NULL_PTR) || (dataSize == 0u))
    {
        dataSize = 0u;
    }
    else
    {
        SchM_Enter_CanTSyn_Context();

        uint16 freeSpace = cb->Capacity - cb->Count;

        if (freeSpace == 0u)
        {
            SchM_Exit_CanTSyn_Context();
            dataSize = 0u;
        }
        else
        {
            if (dataSize > freeSpace)
            {
                dataSize = freeSpace;
            }

            uint16 firstChunk = cb->Capacity - cb->Head;

            if (firstChunk >= dataSize)
            {
                /* no need to wrap around */
                (void)IStdLib_MemCpy(&cb->Buffer[cb->Head], data, dataSize);
                cb->Head += dataSize;

                if (cb->Head == cb->Capacity)
                {
                    cb->Head = 0u;
                }
            }
            else
            {
                /* need to wrap around */
                (void)IStdLib_MemCpy(&cb->Buffer[cb->Head], data, firstChunk);
                (void)IStdLib_MemCpy(cb->Buffer, &data[firstChunk], (uint32)dataSize - firstChunk);
                cb->Head = dataSize - firstChunk;
            }

            cb->Count += dataSize;

            SchM_Exit_CanTSyn_Context();
        }
    }
    return dataSize;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief         The implementation of reading data from the circular buffer.
 * @param[inout]  cb: circular buffer table pointer.
 * @param[in]     data: data pointer.
 * @param[in]     size: data size.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace        CPD-84332
 */
CANTSYN_LOCAL uint16 CanTSyn_CircularBufferRead(CanTSyn_CircularBufferType* cb, uint8* data, uint16 size)
{
    uint16 dataSize = size;

    SchM_Enter_CanTSyn_Context();
    if (!CanTSyn_IsBufferValid(cb) || (data == NULL_PTR) || (dataSize == 0u))
    {
        dataSize = 0u;
    }
    else
    {
        uint16 usedSpace = cb->Count;
        if (dataSize > usedSpace)
        {
            dataSize = usedSpace;
        }

        uint16 firstChunk = cb->Capacity - cb->Tail;

        if (firstChunk >= dataSize)
        {
            /* no need to wrap around */
            (void)IStdLib_MemCpy(data, &cb->Buffer[cb->Tail], dataSize);
            cb->Tail += dataSize;

            if (cb->Tail == cb->Capacity)
            {
                cb->Tail = 0u;
            }
        }
        else
        {
            /* need to wrap around */
            (void)IStdLib_MemCpy(data, &cb->Buffer[cb->Tail], (uint32)firstChunk);
            (void)IStdLib_MemCpy(&data[firstChunk], cb->Buffer, (uint32)dataSize - firstChunk);
            cb->Tail = dataSize - firstChunk;
        }
        cb->Count -= dataSize;
    }
    SchM_Exit_CanTSyn_Context();
    return dataSize;
}
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"
#endif
/* PRQA S 6060 -- */
/* PRQA S 1503, 1532, 2889 -- */
