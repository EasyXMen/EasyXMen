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
 **  @file               : EthTSyn.c
 **  @author             : yuzhe.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Implementation for EthTSyn
 **
 ***********************************************************************************************************************/
/* PRQA S 2889, 6060, 6520 EOF */ /* VL_QAC_MultiReturn, VL_MTR_EthTSyn_STM19, VL_MTR_EthTSyn_STVAR */
/* PRQA S 1503, 1532 ++ */        /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
/* =================================================== inclusions =================================================== */
#include "EthTSyn.h"
#include "SchM_EthTSyn.h"
#include "EthTSyn_Types.h"
#include "IStdLib.h"
#include "StbM.h"

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) && (ETHTSYN_CRC_SUPPORT == STD_ON)
#include "Crc.h"
#endif
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON) || (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
#include "Det.h"
#endif
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
#include "Os.h"
#endif
#if (ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
#include "IdsM.h"
#endif
#include "EthTSyn_Arti.h"
/* ===================================================== macros ===================================================== */
#ifndef ETHTSYN_LOCAL
#define ETHTSYN_LOCAL static
#endif

#ifndef ETHTSYN_LOCAL_INLINE
#define ETHTSYN_LOCAL_INLINE ETHTSYN_LOCAL inline
#endif

#define TIMEDOMIN_CFG(timedomainid)    EthTSyn_ConfigData->GlobalTimeDomain[timedomainid]
#define PORT_CFG(timedomainid, PortId) EthTSyn_ConfigData->GlobalTimeDomain[timedomainid].PortConfig[PortId]
#define PORT_TOCFGDOMIN(PortId) \
    EthTSyn_ConfigData->GlobalTimeDomain[EthTSyn_ConfigData->RuntimeVariables[PortId]->DomainIndex]
#define PORT_TOCFGPORT(PortId) \
    EthTSyn_ConfigData->GlobalTimeDomain[EthTSyn_ConfigData->RuntimeVariables[PortId]->DomainIndex].PortConfig[0u]
#define ETHTSYN_UNUSED(var) (void)(var)
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define ICV_FRAG_NUM(totalLen)                                                         \
    ((totalLen) <= ETHTSYN_TIME_AUTHENTICATED_FIRST_FRAGMENT_MAX_ICV_LENGTH            \
         ? 1U                                                                          \
         : (1U                                                                         \
            + (((totalLen) - ETHTSYN_TIME_AUTHENTICATED_FIRST_FRAGMENT_MAX_ICV_LENGTH) \
               + (ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH - 1U))     \
                  / ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH))
#endif

#define ETHTSYN_MSG_TYPE_BYTE ((uint8)0u)
#define ETHTSYN_MSG_TYPE_MASK ((uint8)0x0Fu)

#if ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON
#define EthTSyn_SecurityEventReporting(securityEventId, contextData) \
    IdsM_SetSecurityEventWithContextData(securityEventId, &contextData, sizeof(contextData))
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_GetEgressTV(uint8 ctrlIdx, uint8 portId, EthTSyn_TimeStampType* tv, Eth_BufIdxType bufIdx);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_DataToTime(const uint8* data);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON) || (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL StbM_VirtualLocalTimeType EthTSyn_GTToVT(EthTSyn_TimeStampType globalTime);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_VTToGT(StbM_VirtualLocalTimeType virtualLocalTime);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_TimeToData(EthTSyn_TimeStampType rawTime, uint8* data);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_TransmitMessage(uint8 ethTSynMessage, uint16 portId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ExpectedStatusGet(uint16 portId, uint8 expectedStatus);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_CreatCorrectionField(uint64 ns, uint8* data);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_GetCorrectionField(const uint8* data);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_CrcTimeFlagsTxSecuredCfgType EthTSyn_GetcrcTimeFlags(uint16 portId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void
    EthTSyn_CalculateTimeSecuredCrc(uint16 portId, const uint8* data, uint16 offset, uint8* crc0, uint8* crc1);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint16 EthTSyn_TlvTimeSecured(const EthTSyn_PortType* port, uint8* data, uint16 Lenth);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL
Std_ReturnType EthTSyn_VerifyTimeSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyStatusNotSecuredSubTlv(
    const uint8* data,
    uint16       length,
    uint16       offset,
    uint16       portId,
    boolean*     isSyncToGatewayBitSet);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyStatusSecuredSubTlv(
    const uint8* data,
    uint16       length,
    uint16       offset,
    uint16       portId,
    boolean*     isSyncToGatewayBitSet);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyUserDataNotSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL
Std_ReturnType EthTSyn_VerifyUserDataSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyTLVHeader(const uint8* data);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType EthTSyn_TlvScanAndCRCValidation(
    uint16       portId,
    const uint8* data,
    uint16       frameLength,
    boolean*     isSyncToGatewayBitSet);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE boolean EthTSyn_StbMEthHWUsed(uint16 portId);
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL sint32 EthTSyn_TimeStampCmp(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_timeoptimal(EthTSyn_TimeStampType* timestamp);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_timeadd(EthTSyn_TimeStampType time1, EthTSyn_TimeStampType time2);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_timeless(EthTSyn_TimeStampType time1, uint64 time2);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_TimeStampSub(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2, EthTSyn_TimeStampType* td);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_IsPortIdentityEqual
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_IsPortIdentityEqual(const uint8* portIdentity1, const uint8* portIdentity2);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

/* PRQA S 5016 ++ */ /* VL_EthTSyn_5016 */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindPort(uint8 ctrlIdx, uint8* portIdx);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 5016 -- */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint8 EthTSyn_MainFunctionMaster(uint8 portIndex);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_GetHardIngressTime(uint8 ctrlIdx, const uint8* dataPtr, EthTSyn_TimeStampType* tVnow);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#else
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_GetSoftIngressTime(const EthTSyn_PortType* port, EthTSyn_TimeStampType* tVnow);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean
    EthTSyn_GetSyncT2vlt(const EthTSyn_PortType* port, EthTSyn_TimeStampType t1vlt, EthTSyn_TimeStampType* t2vlt);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_UpdateHysteresisInTimeout(
    EthTSyn_PortType* port,
    const uint16      sequenceId,
    const uint16      sequenceCounterJump,
    const uint16      sequenceCounterJumpWidth);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL
void EthTSyn_RxHandleSyncMessage(EthTSyn_PortType* port, const uint8* dataPtr, EthTSyn_TimeStampType tVnow);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_RxHandleFollowUpMessage(EthTSyn_PortType* port, const uint8* dataPtr, uint16 lenByte);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void
    EthTSyn_RxHandlePdelayReqMessage(EthTSyn_PortType* port, const uint8* dataPtr, EthTSyn_TimeStampType tVnow);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void
    EthTSyn_RxHandlePdelayResponseMessage(EthTSyn_PortType* port, const uint8* dataPtr, EthTSyn_TimeStampType tVnow);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void
    EthTSyn_RxHandlePdelayResponseFUMessage(EthTSyn_PortType* port, const uint8* dataPtr, EthTSyn_TimeStampType tVnow);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_OriginalTimeStampCaculation(
    EthTSyn_PortType*     port,
    boolean               isValidEgressTime,
    EthTSyn_TimeStampType egressTimeStamp);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint16 EthTSyn_SyncMessagePack(EthTSyn_PortType* port, uint8* data, boolean* hasCapturedEgress);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint16 EthTSyn_FollowUpMessagePack(const EthTSyn_PortType* port, uint8* data);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint16 EthTSyn_ReqMessagePack(EthTSyn_PortType* port, uint8* data);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_PublicInformationPack(const EthTSyn_PortType* port, uint8* data, uint8 ethTSynMessage);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#ifdef QAC_ANALYZE
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidatePointer
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidateCtrlIdx
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidateInitStatus
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidateInit
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidateRxindication
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidatePartitionContext
#endif
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_GetPartitionIndex
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidateInit
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidateTransmissionMode
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidateSetOrGetProtocolParam
#pragma PRQA_NO_SIDE_EFFECTS EthTSyn_ValidateICVIndication
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidatePointer(uint8 apiId, const void* pointer);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidateCtrlIdx(uint8 apiId, uint8 ctrlIdx);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidateInitStatus(uint8 apiId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidateInit(const EthTSyn_ConfigType* configPtr);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidateRxindication(uint8 ctrlIdx, const void* pointer);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidateTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType mode);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint8 EthTSyn_GetPartitionIndex(void);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidatePartitionContext(uint8 apiId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidateSetOrGetProtocolParam(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_ProtocolParamType* protocolParam,
    uint8                         apiId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL boolean EthTSyn_ValidateICVIndication(uint32 jobId, uint8 apiId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_InitPortStatus(EthTSyn_PortType* portPtr);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_Init_DataAndICV(
    EthTSyn_GlobalTimeDomainCfgType domainConfig,
    EthTSyn_PortConfigCfgType       portConifg,
    uint8                           domainIndex);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_InitClockIdentity(EthTSyn_PortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_HandleTxConfirmationByType(
    EthTSyn_PortType*     port,
    uint8                 portIndex,
    boolean               isValidEgressTime,
    EthTSyn_TimeStampType egressTimeStamp);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint8 EthTSyn_HandlePortMainFunctionByRole(uint8 portIndex, EthTSyn_PortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_ParseAndVerifySubTlvs(const uint8* data, uint16 msgLength, uint16 portId, boolean* isSyncToGatewayBitSet);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint16 EthTSyn_FillFollowUpExtendedTlv(const EthTSyn_PortType* port, uint8* data, uint16 Lenth);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_DispatchMessageByType(
    EthTSyn_PortType*     port,
    const uint8*          msg,
    EthTSyn_TimeStampType tVnow,
    uint16                lenByte,
    boolean               isValidIngressTime);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindPortAcrossTimeBase(uint8 timeBaseId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindTxPortAcrossJobId(uint32 jobId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindRxPortAcrossJobId(uint32 jobId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
ETHTSYN_LOCAL inline uint16 EthTSyn_BitToBYTE(uint16 nbits);
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType EthTSyn_TxFreshnessObtain(
    const EthTSyn_GlobalTimeTxIcvGenerationType* txIcvGeneration,
    uint8*                                       freshnessValue,
    uint8*                                       truncatedFreshnessValue);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_ICVGeneration(EthTSyn_PortType* port, const uint8* generationBuffer, uint16 dataLength);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint16 EthTSyn_ICVTxMainfunction(EthTSyn_PortType* port, uint8* data, uint16 dataLenth);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_StartICVGeneration(EthTSyn_PortType* port, uint8* data, uint16 dataLenth);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_SubTlvAuthenticatedBuild(EthTSyn_PortType* port, uint8* data, uint16 dataLenth);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

ETHTSYN_LOCAL_INLINE void    EthTSyn_ICVStatusChange(EthTSyn_PortType* port, uint8 toState);
ETHTSYN_LOCAL_INLINE boolean EthTSyn_ICVStatusComparison(const EthTSyn_PortType* port, uint8 State);
ETHTSYN_LOCAL_INLINE void    EthTSyn_ICVGenerationBuildFailBehavior(EthTSyn_PortType* port, uint16 eventId);
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyTimeAuthenticatedSubTlv(const uint8* data, uint8 dataTap, uint16 length, uint16 portId);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_ICVRxMainfunction(EthTSyn_PortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_StartICVVerification(EthTSyn_PortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_RxFreshnessObtain(const EthTSyn_PortType* port, uint8* fvValue, uint8 timeAuthenticatedFieldOffset);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_ICVVerification(
    EthTSyn_PortType* port,
    const uint8*      generationBuffer,
    uint8             timeAuthenticatedFieldOffset,
    const uint8*      recRawDataPtr);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_InitPortICVRuntimeVariables(EthTSyn_PortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

ETHTSYN_LOCAL_INLINE boolean EthTSyn_IsBufferValid(const EthTSyn_CircularBufferType* cb);
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint16 EthTSyn_CircularBufferWrite(EthTSyn_CircularBufferType* cb, const uint8* data, uint16 size);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint16 EthTSyn_CircularBufferRead(EthTSyn_CircularBufferType* cb, uint8* data, uint16 size);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint8 EthTSyn_ValidateMultipleFragment(
    const EthTSyn_GlobalTimeRxIcvVerificationType* icvVerification,
    const uint8*                                   data,
    uint8                                          dataTap);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_ObtainIcvInMultipleFragment(EthTSyn_PortType* port, uint8* receivedIcv, uint8 dataTap);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_ValidFollowUpMessageSetGlobalTime(
    const EthTSyn_PortType* port,
    const uint8*            rawData,
    const boolean*          isSyncToGatewayBitSet);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL uint64 EthTSyn_TimestampToLocalTime(const EthTSyn_TimeStampType* timestamp);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void
    EthTSyn_CalculateNeighborRateRatio(const EthTSyn_PdelayConfigCfgType* pdelayConfig, EthTSyn_PortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_DynamicPortRole(EthTSyn_PortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
ETHTSYN_LOCAL void EthTSyn_HoldOverTimerHandle(EthTSyn_PortType* port);
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

ETHTSYN_LOCAL_INLINE uint8   EthTSyn_UnpackType(const uint8* msg);
ETHTSYN_LOCAL_INLINE boolean EthTSyn_CheckMsgSequence(uint8 type, EthTSyn_PortType* port);

/* ============================================ internal data definitions =========================================== */
ETHTSYN_LOCAL_INLINE uint8 EthTSyn_UnpackType(const uint8* msg)
{
    return msg[ETHTSYN_MSG_TYPE_BYTE] & ETHTSYN_MSG_TYPE_MASK;
}
ETHTSYN_LOCAL_INLINE boolean EthTSyn_CheckMsgSequence(uint8 type, EthTSyn_PortType* port)
{
    boolean success = TRUE;

    SchM_Enter_EthTSyn_Context();
    if (((type == SYNC) || (type == FOLLOW_UP)) && (port->RxDebounceTimeTicks > 0u))
    {
        /* SWS_EthTSyn_00232, PRS_TS_00242 */
        port->FollowUpTimeoutTicks = 0u;
        success                    = FALSE;
    }
    if ((type == SYNC) && (port->FollowUpTimeoutTicks > 0u))
    {
        port->FollowUpTimeoutTicks = 0u;
        success                    = FALSE;
    }
    SchM_Exit_EthTSyn_Context();

#ifdef ETHTSYN_SEV_TSYN_ETH_MSG_SEQUENCE_ERROR
    if (!success)
    {
        EthTSyn_SecurityEventReporting(ETHTSYN_SEV_TSYN_ETH_MSG_SEQUENCE_ERROR, port->TimeDomainId);
    }
#endif

    SchM_Enter_EthTSyn_Context();
    if ((type == FOLLOW_UP) && (port->FollowUpTimeoutTicks == 0u))
    {
        success = FALSE;
    }
    SchM_Exit_EthTSyn_Context();

    return success;
}

#define ETHTSYN_START_SEC_VAR_CLEARED_BOOLEAN
#include "EthTSyn_MemMap.h"
/**
 * @brief Initialization status array for EthTSyn module partitions
 * @range 0..1
 */
ETHTSYN_LOCAL boolean EthTSyn_Initialized[ETHTSYN_PARTITION_NUMBER];
#define ETHTSYN_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthTSyn_MemMap.h"
/**
 * @brief       Pointer to EthTSyn module configuration data structure
 * @range N/A
 */
ETHTSYN_LOCAL const EthTSyn_ConfigType* EthTSyn_ConfigData;
#define ETHTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthTSyn_MemMap.h"
/**
 * @brief       Storage for received Pdelay_Req message source port identities
 * @range 0..255
 */
ETHTSYN_LOCAL uint8 EthTSyn_ReceivedPdelayReqSourcePortId[ETHTSYN_GLOBALTIME_DOMAIN_NUM][ETHTSYN_PORT_IDENTITY_LENGTH];
#define ETHTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthTSyn_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief           Initialize EthTSyn runtime data and ICVvariables
 * @param[in]       domainConfig Global time domain configuration parameters
 * @param[in]       portConfig   Port configuration parameters
 * @param[in]       domainIndex  Time domain index, identifies different time synchronization domains
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-86484
 */
ETHTSYN_LOCAL void EthTSyn_Init_DataAndICV(
    EthTSyn_GlobalTimeDomainCfgType domainConfig,
    EthTSyn_PortConfigCfgType       portConifg,
    uint8                           domainIndex)
{
    EthTSyn_PortType* port   = EthTSyn_ConfigData->RuntimeVariables[domainIndex];
    port->TimeDomainId       = domainConfig.GlobalTimeDomainId;
    port->DomainIndex        = domainIndex;
    port->CtrlId             = domainConfig.GlobalTimeEthIfRef;
    port->SCHysteresisCount  = ETHTSYN_MAX_HYSTERESIS_COUNTER;
    port->PortRoleType       = portConifg.GlobalTimePortRole;
    port->FramePrio          = domainConfig.FramePrio;
    uint64 delay             = portConifg.PdelayConfig->GlobalTimePropagationDelay;
    port->Pdelay.Nanoseconds = (uint32)((uint32)delay % ETHTSYN_NANOSECOND_TO_SECOND);
    delay /= ETHTSYN_NANOSECOND_TO_SECOND;
    port->Pdelay.Seconds   = (uint32)delay;
    port->Pdelay.SecondsHi = (uint16)(delay >> ETHTSYN_BIT_SHIFT_32);

    port->TransmissionMode = ETHTSYN_TX_OFF;

    /* Initialize data */
    (void)IStdLib_MemSet(EthTSyn_ReceivedPdelayReqSourcePortId[domainIndex], 0u, ETHTSYN_PORT_IDENTITY_LENGTH);

    /* ThisPortIdentity.ClockIdentity see 802.1as 2011 8.5.2.2, delay initialized in EthTSyn_TrcvLinkStateChg */

    /* ThisPortIdentity.PortNumber  802.1as 8.5.2.3 */
    uint16 PortNumber = 0u;
    if (EthTSyn_ExpectedStatusGet(port->DomainIndex, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
    {
        PortNumber = (uint16)(domainIndex);
    }

    port->ThisPortIdentity.PortNumber = PortNumber;

    (void)IStdLib_MemSet(&port->PortIdentity, 0u, ETHTSYN_PORT_IDENTITY_LENGTH);
    port->UserData.userByte0                 = 0u;
    port->UserData.userByte1                 = 0u;
    port->UserData.userByte2                 = 0u;
    port->UserData.userDataLength            = 0u;
    port->TimeBaseUpdateCounter              = 0u;
    port->PreciseOriginTimestamp.Nanoseconds = 0u;
    port->PreciseOriginTimestamp.Seconds     = 0u;
    port->PreciseOriginTimestamp.SecondsHi   = 0u;
    port->T1vlt                              = port->PreciseOriginTimestamp;
    port->T1                                 = port->T1vlt;
    port->T2                                 = port->T1;
    port->T3                                 = port->T1;
    port->T4                                 = port->T1;

    port->EthtrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
    EthTSyn_InitPortStatus(port);
/* Init ICV Generation And Verification */
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    EthTSyn_InitPortICVRuntimeVariables(port);
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    port->IsSyncToGatewayBitSet = FALSE;
#endif
#endif

#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON)
    (void*)IStdLib_MemSet(&port->TrefPDResponder, 0, sizeof(StbM_TimeTupleType));
#endif
#if (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
    (void*)IStdLib_MemSet(&port->TrefPDInitiator, 0, sizeof(StbM_TimeTupleType));
#endif

    port->RateRatio.NeighborRateRatio        = 1.0;
    port->RateRatio.RateRatioPdelayResponder = 1.0;
#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
    port->RateRatio.HasCaculatedNeighborRateRatio = FALSE;
#endif

    port->HoldOverTimer = 0u;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_Init(
    const EthTSyn_ConfigType*
        configPtr) /* PRQA S 6030 */ /* VL_MTR_EthTSyn_STMIF */ /* PRQA S 6070 */ /* VL_MTR_EthTSyn_STCAL*/
{
    EthTSyn_ArtiApiStart(2, ETHTSYN_SID_INIT, configPtr);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_INIT) &&
#endif
        EthTSyn_ValidateInit(configPtr))
#endif
    {
        EthTSyn_ConfigData = configPtr;
        for (uint8 domainIndex = 0u; domainIndex < EthTSyn_ConfigData->DomainNum; domainIndex++)
        {
            EthTSyn_GlobalTimeDomainCfgType domainConfig = TIMEDOMIN_CFG(domainIndex);
            EthTSyn_PortConfigCfgType       portConifg   = PORT_CFG(domainIndex, 0u);
#if ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON
            if (domainConfig.PortReferrencedPartitionId == GetApplicationID())
#endif
            {
                EthTSyn_Init_DataAndICV(domainConfig, portConifg, domainIndex);
            }
        }
        EthTSyn_Initialized[EthTSyn_GetPartitionIndex()] = TRUE;
    }
    EthTSyn_ArtiApiReturn(1, ETHTSYN_SID_INIT);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_VERSION_INFO_API == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    EthTSyn_ArtiApiStart(2, ETHTSYN_SID_GETVERSIONINFO, versioninfo);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidatePointer(ETHTSYN_SID_GETVERSIONINFO, versioninfo))
#endif
    {
        versioninfo->vendorID         = ETHTSYN_VENDOR_ID;
        versioninfo->moduleID         = ETHTSYN_MODULE_ID;
        versioninfo->sw_major_version = ETHTSYN_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = ETHTSYN_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = ETHTSYN_SW_PATCH_VERSION;
    }
    EthTSyn_ArtiApiReturn(2, ETHTSYN_SID_GETVERSIONINFO, versioninfo);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
/* PRQA S 1505 ++*/ /* VL_QAC_OneFunRef */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_SetTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType mode)
/* PRQA S 1505 --*/
{
    EthTSyn_ArtiApiStart(3, ETHTSYN_SID_SETTRANSMISSIONMODE, ctrlIdx, mode);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_SETTRANSMISSIONMODE) &&
#endif
        EthTSyn_ValidateInitStatus(ETHTSYN_SID_SETTRANSMISSIONMODE) && EthTSyn_ValidateTransmissionMode(ctrlIdx, mode))
#endif
    {
        EthTSyn_PortType* port = EthTSyn_FindPort(ctrlIdx, NULL_PTR);
        port->TransmissionMode = mode;
    }
    EthTSyn_ArtiApiReturn(1, ETHTSYN_SID_SETTRANSMISSIONMODE);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief           Dispatch received message to appropriate handler based on message type
 * @param[inout]    port Pointer to EthTSyn port structure
 * @param[in]       msg  Pointer to received message data
 * @param[in]       tVnow Current timestamp value
 * @param[in]       lenByte Length of received message in bytes
 * @param[in]       isValidIngressTime Flag indicating if ingress timestamp is valid
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-86486
 */
ETHTSYN_LOCAL void EthTSyn_DispatchMessageByType(
    EthTSyn_PortType*     port,
    const uint8*          msg,
    EthTSyn_TimeStampType tVnow,
    uint16                lenByte,
    boolean               isValidIngressTime)
{
    uint8 type = EthTSyn_UnpackType(msg);

    if (!isValidIngressTime && (type == PDELAY_RESP_FOLLOW_UP))
    {
        port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
        return;
    }

    if (!EthTSyn_CheckMsgSequence(type, port))
    {
        return;
    }

    switch (type)
    {
    case SYNC:
        EthTSyn_RxHandleSyncMessage(port, msg, tVnow);
        break;
    case FOLLOW_UP:
        EthTSyn_RxHandleFollowUpMessage(port, msg, lenByte);
        break;
    case PDELAY_REQ:
        EthTSyn_RxHandlePdelayReqMessage(port, msg, tVnow);
        break;
    case PDELAY_RESP:
        EthTSyn_RxHandlePdelayResponseMessage(port, msg, tVnow);
        break;
    case PDELAY_RESP_FOLLOW_UP:
        EthTSyn_RxHandlePdelayResponseFUMessage(port, msg, tVnow);
        break;
    default:
        /* do nothing */
        break;
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 6040 ++ */ /* VL_MTR_EthTSyn_STPAR */
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_RxIndication(
    uint8         ctrlIdx,
    Eth_FrameType frameType,
    boolean       isBroadcast,
    const uint8*  physAddrPtr,
    const uint8*  dataPtr,
    uint16        lenByte)
/* PRQA S 6070 -- */
/* PRQA S 6040 -- */
{
    EthTSyn_ArtiApiStart(7, ETHTSYN_SID_RXINDICATION, ctrlIdx, frameType, isBroadcast, physAddrPtr, dataPtr, lenByte);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_RXINDICATION) &&
#endif
        EthTSyn_ValidateInitStatus(ETHTSYN_SID_RXINDICATION) && EthTSyn_ValidateRxindication(ctrlIdx, dataPtr))
#endif
    {
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
        /* SWS_EthTSyn_00180 */
        SchM_Enter_EthTSyn_Context();
#endif
        ETHTSYN_UNUSED(isBroadcast);
        ETHTSYN_UNUSED(physAddrPtr);
        uint8             portIndex = 0u;
        EthTSyn_PortType* port      = EthTSyn_FindPort(ctrlIdx, &portIndex);
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON)
        ETHTSYN_UNUSED(lenByte);
#endif

        if (((port != NULL_PTR) && (frameType == ETHTSYN_TIME_SYNCHRONIZATION_FRAMETYPE)
             && (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
             && (port->TimeDomainId == dataPtr[ETHTSYN_TIMEDOMAINID_BYTE_OFFSET])))
        {
            EthTSyn_TimeStampType tVnow;
            boolean               isValidIngressTime = FALSE;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
            isValidIngressTime = EthTSyn_GetHardIngressTime(ctrlIdx, dataPtr, &tVnow);
#else
            isValidIngressTime = EthTSyn_GetSoftIngressTime(port, &tVnow);
            SchM_Exit_EthTSyn_Context();
#endif
            EthTSyn_DispatchMessageByType(port, dataPtr, tVnow, lenByte, isValidIngressTime);
        }
        else
        {
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
            SchM_Exit_EthTSyn_Context();
#endif
        }
    }
    EthTSyn_ArtiApiReturn(1, ETHTSYN_SID_RXINDICATION);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief           Handle transmission confirmation based on PDU type
 * @param[inout]    port Pointer to the port structure
 * @param[in]       portIndex Port index identifier
 * @param[in]       isValidEgressTime Flag indicating if egress timestamp is valid
 * @param[in]       egressTimeStamp Egress timestamp from hardware
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-86491
 */
ETHTSYN_LOCAL void EthTSyn_HandleTxConfirmationByType(
    EthTSyn_PortType*     port,
    uint8                 portIndex,
    boolean               isValidEgressTime,
    EthTSyn_TimeStampType egressTimeStamp)
{
    boolean isSyncPduSend = TRUE;
    switch (port->TranssmitMessageType)
    {
    case SYNC:
    {
        EthTSyn_OriginalTimeStampCaculation(port, isValidEgressTime, egressTimeStamp);
#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON)
        /* SWS_EthTSyn_00214 SWS_EthTSyn_00215 */
        if (PORT_TOCFGPORT(portIndex).EnableTimeValidation)
        {
            StbM_EthTimeMasterMeasurementType validationMeasureDataPtr;
            IStdLib_MemSet(&validationMeasureDataPtr, 0u, sizeof(StbM_EthTimeMasterMeasurementType));
            validationMeasureDataPtr.sequenceId                         = port->SyncSequenceId;
            validationMeasureDataPtr.sourcePortId.clockIdentity         = port->ThisPortIdentity.ClockIdentity;
            validationMeasureDataPtr.sourcePortId.portNumber            = port->ThisPortIdentity.PortNumber;
            validationMeasureDataPtr.syncEgressTimestamp                = EthTSyn_GTToVT(egressTimeStamp);
            validationMeasureDataPtr.preciseOriginTimestamp.seconds     = port->PreciseOriginTimestamp.Seconds;
            validationMeasureDataPtr.preciseOriginTimestamp.nanoseconds = port->PreciseOriginTimestamp.Nanoseconds;
            validationMeasureDataPtr.correctionField =
                (sint64)PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster->CorrectionField;
            StbM_EthSetMasterTimingData(PORT_TOCFGDOMIN(portIndex).SynchronizedTimeBaseRef, &validationMeasureDataPtr);
        }
#endif
    }
    break;
    case FOLLOW_UP:
    {
        port->SyncStatusType = ETHTSYN_SEND_SYNC;
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        /* SWS_EthTSyn_00258 */
        const EthTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
            PORT_TOCFGPORT(portIndex).GlobalTimeTxIcvGeneration;
        if (icvGeneration != NULL_PTR)
        {
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_BUILDING);
            if (icvGeneration->IcvTxFreshnessValueTruncLength != 0u)
            {
                StbM_SPduTxConfirmation(icvGeneration->IcvGenerationFvIdRef);
            }
        }

#endif
        break;
    }
    case PDELAY_REQ:
    {
        if (isValidEgressTime)
        {
            port->PdelayTXStatusType = ETHTSYN_WAITFOR_PDELAY_RESP;
            port->T1                 = egressTimeStamp;
        }
        else
        {
            port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
        }
    }
    break;
    case PDELAY_RESP:
    {
        if (isValidEgressTime)
        {
            port->PdelayResponderTimestamp.PdelayRespEventEgressTimestamp = egressTimeStamp;
            port->PdelayRXStatusType                                      = ETHTSYN_SEND_PDELAY_RESP_FOLLOWUP;
        }
        else
        {
            port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
        }
    }
    break;
    case PDELAY_RESP_FOLLOW_UP:
    {
        port->PdelayRXStatusType = ETHTSYN_WAITFOR_PDELAY_REQ;
#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON)
        /* SWS_EthTSyn_00219 SWS_EthTSyn_00220 */
        if (PORT_TOCFGPORT(portIndex).EnableTimeValidation)
        {
            StbM_PdelayResponderMeasurementType validationMeasureDataPtr;
            IStdLib_MemSet(&validationMeasureDataPtr, 0u, sizeof(StbM_PdelayResponderMeasurementType));
            validationMeasureDataPtr.sequenceId = port->ReceivedPdelayReqSequenceId;
            const uint64* ClockIdentity         = (const uint64*)&EthTSyn_ReceivedPdelayReqSourcePortId[portIndex][0];
            validationMeasureDataPtr.requestPortId.clockIdentity  = *ClockIdentity;
            validationMeasureDataPtr.requestPortId.portNumber     = port->DomainIndex;
            validationMeasureDataPtr.responsePortId.clockIdentity = *ClockIdentity;
            validationMeasureDataPtr.responsePortId.portNumber    = port->DomainIndex;
            /* T2 ingress timestamp of REQ */
            validationMeasureDataPtr.requestReceiptTimestamp =
                EthTSyn_GTToVT(port->PdelayResponderTimestamp.PdelayReqEventIngressTimestamp);
            /* T3 egress timestamp of RESP */
            validationMeasureDataPtr.responseOriginTimestamp =
                EthTSyn_GTToVT(port->PdelayResponderTimestamp.PdelayRespEventEgressTimestamp);
            validationMeasureDataPtr.referenceLocalTimestamp          = port->TrefPDResponder.virtualLocalTime;
            validationMeasureDataPtr.referenceGlobalTimestamp.seconds = port->TrefPDResponder.globalTime.seconds;
            validationMeasureDataPtr.referenceGlobalTimestamp.nanoseconds =
                port->TrefPDResponder.globalTime.nanoseconds;
            StbM_EthSetPdelayResponderData(
                PORT_TOCFGDOMIN(portIndex).SynchronizedTimeBaseRef,
                &validationMeasureDataPtr);
        }
#endif
    }
    break;
    default:
        isSyncPduSend = FALSE;
        break;
    }
    if (isSyncPduSend)
    {
        uint64 globalTimeDebounceTime = PORT_TOCFGDOMIN(portIndex).GlobalTimeDebounceTime;
        port->SyncdebounceCounter     = globalTimeDebounceTime;
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* clang-format off */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_TxConfirmation(uint8 ctrlIdx, Eth_BufIdxType bufIdx, Std_ReturnType result)
/* clang-format on */
{
    EthTSyn_ArtiApiStart(4, ETHTSYN_SID_TXCONFIRMATION, ctrlIdx, bufIdx, result);
    uint8             portIndex = 0u;
    EthTSyn_PortType* port      = EthTSyn_FindPort(ctrlIdx, &portIndex);
    if (
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_TXCONFIRMATION) &&
#endif
        EthTSyn_ValidateInitStatus(ETHTSYN_SID_TXCONFIRMATION)
        && EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_TXCONFIRMATION, ctrlIdx) &&
#endif
        (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) && (result == E_OK))
    {
        EthTSyn_TimeStampType egressTimeStamp   = {0u, 0u, 0u};
        boolean               isValidEgressTime = FALSE;
        if (EthTSyn_GetEgressTV(ctrlIdx, portIndex, &egressTimeStamp, bufIdx) == E_OK)
        {
            isValidEgressTime = TRUE;
        }
        EthTSyn_HandleTxConfirmationByType(port, portIndex, isValidEgressTime, egressTimeStamp);
    }
    else if ((result != E_OK))
    {
        EthTSyn_InitPortStatus(port);
    }
    else
    {
        /* do nothing */
    }
    EthTSyn_ArtiApiReturn(1, ETHTSYN_SID_TXCONFIRMATION);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_TrcvLinkStateChg(uint8 ctrlIdx, EthTrcv_LinkStateType trcvLinkState)
{
    EthTSyn_ArtiApiStart(3, ETHTSYN_SID_TRCVLINKSTATECHG, ctrlIdx, trcvLinkState);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_TRCVLINKSTATECHG) &&
#endif
        EthTSyn_ValidateInitStatus(ETHTSYN_SID_TRCVLINKSTATECHG)
        && EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_TRCVLINKSTATECHG, ctrlIdx))
#endif
    {
        EthTSyn_PortType* port = EthTSyn_FindPort(ctrlIdx, NULL_PTR);
        if (port != NULL_PTR)
        {
            if ((port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) && (trcvLinkState == ETHTRCV_LINK_STATE_DOWN))
            {
                /* SWS_EthTSyn_00019 */
                port->EthtrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
                EthTSyn_InitPortStatus(port);
            }
            if ((port->EthtrcvLinkState == ETHTRCV_LINK_STATE_DOWN) && (trcvLinkState == ETHTRCV_LINK_STATE_ACTIVE))
            {
                /* In certain implementations, the EthDriver can only retrieve the MAC address once the controller mode
                 * transitions to active. */
                EthTSyn_InitClockIdentity(port);
                /* SWS_EthTSyn_00020 */
                port->EthtrcvLinkState = ETHTRCV_LINK_STATE_ACTIVE;
            }
#if (ETHTSYN_DEST_PHY_ADDR_MULTICAST == STD_ON)
            Eth_FilterActionType filterAction =
                (trcvLinkState == ETHTRCV_LINK_STATE_ACTIVE) ? ETH_ADD_TO_FILTER : ETH_REMOVE_FROM_FILTER;
            (void)EthIf_UpdatePhysAddrFilter(ctrlIdx, EthTSyn_DestPhyAddr, filterAction);
#endif
        }
    }
    EthTSyn_ArtiApiReturn(1, ETHTSYN_SID_TRCVLINKSTATECHG);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief           Handle port main function processing based on port role
 * @param[in]       portIndex Index of the port
 * @param[inout]    port Pointer to EthTSyn port structure
 * @param[in]       txPdelayReqPeriod Transmission period for Pdelay_Req messages
 * @return          uint8 Type of message to transmit (if any)
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-86493
 */
ETHTSYN_LOCAL uint8 EthTSyn_HandlePortMainFunctionByRole(uint8 portIndex, EthTSyn_PortType* port)
{
    uint8                            transsmitMsgType  = MSG_NONE;
    const EthTSyn_PortConfigCfgType* portCfg           = &PORT_TOCFGPORT(portIndex);
    uint64                           txPdelayReqPeriod = portCfg->PdelayConfig->GlobalTimeTxPdelayReqPeriod;
    if (EthTSyn_ExpectedStatusGet(portIndex, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
    {
        EthTSyn_HoldOverTimerHandle(port);
    }
    if (ETHTSYN_MASTER == port->PortRoleType)
    {
        transsmitMsgType = EthTSyn_MainFunctionMaster(portIndex);
    }
    else
    {
        const EthTSyn_GlobalTimeSlaveCfgType* slaveCfgPtr = PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeSlave;

        SchM_Enter_EthTSyn_Context();
        if (port->RxDebounceTimeTicks > 0u)
        {
            port->RxDebounceTimeTicks--;
        }
        if ((slaveCfgPtr->FollowUpTimeoutTicks > 0u) && (port->FollowUpTimeoutTicks > 0u))
        {
            port->FollowUpTimeoutTicks--;
        }
        SchM_Exit_EthTSyn_Context();

#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        EthTSyn_ICVRxMainfunction(port);
#endif

#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
        EthTSyn_CalculateNeighborRateRatio(portCfg->PdelayConfig, port);
#endif
        if (txPdelayReqPeriod != 0ULL)
        {
            const EthTSyn_PortType* portPtr = EthTSyn_ConfigData->RuntimeVariables[portIndex];
            if ((port->PdelayReqPeriod <= ETHTSYN_MAIN_FUNCTION_PERIOD) && (portPtr->SyncdebounceCounter == 0u))
            {
                transsmitMsgType      = PDELAY_REQ;
                port->PdelayReqPeriod = txPdelayReqPeriod;
            }
            else
            {
                port->PdelayReqPeriod -= ETHTSYN_MAIN_FUNCTION_PERIOD;
            }
        }
    }
    return transsmitMsgType;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_MainFunction(uint8 portIndex) /* PRQA S 6030 */ /* VL_MTR_EthTSyn_STMIF */
{
    EthTSyn_ArtiApiStart(2, ETHTSYN_SID_MAINFUNCTION, portIndex);
    if (
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
        EthTSyn_ValidatePartitionContext(ETHTSYN_SID_MAINFUNCTION) &&
#endif
        EthTSyn_ValidateInitStatus(ETHTSYN_SID_MAINFUNCTION) &&
#endif
        EthTSyn_Initialized[EthTSyn_GetPartitionIndex()])
    {
        EthTSyn_PortType* port = EthTSyn_ConfigData->RuntimeVariables[portIndex];
        if (port->EthtrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
        {
            const EthTSyn_PortConfigCfgType* portCfg                = &PORT_TOCFGPORT(portIndex);
            EthTSyn_PortType*                portPtr                = EthTSyn_ConfigData->RuntimeVariables[portIndex];
            uint64                           globalTimeDebounceTime = PORT_TOCFGDOMIN(portIndex).GlobalTimeDebounceTime;
            if ((globalTimeDebounceTime > 0ULL) && (portPtr->SyncdebounceCounter > 0ULL))
            {
                --portPtr->SyncdebounceCounter;
            }

            /* if only one port in domain, do not need to handle HoldOverTimer,Time GateWay is required */
            uint8 transsmitMsgType = EthTSyn_HandlePortMainFunctionByRole(portIndex, port);
            if (portPtr->SyncdebounceCounter == 0ULL)
            {
                if ((port->PdelayRXStatusType == ETHTSYN_SEND_PDELAY_RESP)
                    && (portCfg->PdelayConfig->GlobalTimePdelayRespEnable) && (transsmitMsgType == MSG_NONE))
                {
                    transsmitMsgType = PDELAY_RESP;
                }
                else if (port->PdelayRXStatusType == ETHTSYN_SEND_PDELAY_RESP_FOLLOWUP)
                {
                    if (transsmitMsgType == MSG_NONE)
                    {
                        transsmitMsgType = PDELAY_RESP_FOLLOW_UP;
                    }
                }
                else
                {
                    /* do nothing */
                }

                if (transsmitMsgType != MSG_NONE)
                {
                    EthTSyn_TransmitMessage(transsmitMsgType, portIndex);
                }
            }
        }
    }
    EthTSyn_ArtiApiReturn(1, ETHTSYN_SID_MAINFUNCTION);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
Std_ReturnType
    EthTSyn_SetProtocolParam(StbM_SynchronizedTimeBaseType timeBaseId, const StbM_ProtocolParamType* protocolParam)
{
    Std_ReturnType ret = E_NOT_OK;
    EthTSyn_ArtiApiStart(3, ETHTSYN_SID_SETPROTOCOLPARAM, timeBaseId, protocolParam);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateSetOrGetProtocolParam(timeBaseId, protocolParam, ETHTSYN_SID_SETPROTOCOLPARAM))
#endif
    {
        for (uint8 portIndex = 0u; portIndex < ETHTSYN_GLOBALTIME_DOMAIN_NUM; ++portIndex)
        {
            if (EthTSyn_ConfigData->GlobalTimeDomain[portIndex].SynchronizedTimeBaseRef == timeBaseId)
            {
                SchM_Enter_EthTSyn_Context();
                *EthTSyn_ConfigData->GlobalTimeDomain[portIndex].ProtocolParam = *protocolParam;
                SchM_Exit_EthTSyn_Context();
                ret = E_OK;
                break;
            }
        }
    }
    EthTSyn_ArtiApiReturn(2, ETHTSYN_SID_SETPROTOCOLPARAM, ret);
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
Std_ReturnType EthTSyn_GetProtocolParam(StbM_SynchronizedTimeBaseType timeBaseId, StbM_ProtocolParamType* protocolParam)
{
    Std_ReturnType ret = E_NOT_OK;
    EthTSyn_ArtiApiStart(3, ETHTSYN_SID_GETPROTOCOLPARAM, timeBaseId, protocolParam);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateSetOrGetProtocolParam(
            timeBaseId,
            (const StbM_ProtocolParamType*)protocolParam,
            ETHTSYN_SID_GETPROTOCOLPARAM))
#endif
    {
        SchM_Enter_EthTSyn_Context();
        const EthTSyn_PortType* port = EthTSyn_FindPortAcrossTimeBase((uint8)timeBaseId);
        if (port != NULL_PTR)
        {
            const StbM_ProtocolParamType* protocolParamRuntime =
                EthTSyn_ConfigData->GlobalTimeDomain[port->DomainIndex].ProtocolParam;
            protocolParam->protocolType               = STBM_TIMESYNC_ETHERNET;
            protocolParam->cumulativeScaledRateOffset = protocolParamRuntime->cumulativeScaledRateOffset;
            protocolParam->gmTimeBaseIndicator        = protocolParamRuntime->gmTimeBaseIndicator;
            protocolParam->lastGmPhaseChange          = protocolParamRuntime->lastGmPhaseChange;
            protocolParam->scaledLastGmFreqChange     = protocolParamRuntime->scaledLastGmFreqChange;
            ret                                       = E_OK;
        }
        SchM_Exit_EthTSyn_Context();
    }
    EthTSyn_ArtiApiReturn(2, ETHTSYN_SID_GETPROTOCOLPARAM, ret);
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_IcvGenerationIndication(Crypto_JobType* job, Crypto_ResultType result)
{
    EthTSyn_ArtiApiStart(3, ETHTSYN_SID_ICVGENERATIONINDICATION, job, result);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateICVIndication(job->jobId, ETHTSYN_SID_ICVGENERATIONINDICATION))
#endif
    {
        EthTSyn_PortType* port = EthTSyn_FindTxPortAcrossJobId(job->jobId);
        if (port != NULL_PTR)
        {
            if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION))
            {
                if (result == E_OK)
                {
                    EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION);
                    port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter = 0u;
                }
                else if (result == E_NOT_OK)
                {
                    /* SWS_EthTSyn_00256 */
                    EthTSyn_ICVGenerationBuildFailBehavior(port, ETHTSYN_SEV_TSYN_ETH_ICV_GENERATION_FAILED);
                }
                else
                {
                    EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_RETRY);
                }
            }
        }
    }
    EthTSyn_ArtiApiReturn(1, ETHTSYN_SID_ICVGENERATIONINDICATION);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
void EthTSyn_IcvVerificationIndication(Crypto_JobType* job, Crypto_ResultType result)
{
    EthTSyn_ArtiApiStart(3, ETHTSYN_SID_ICVVERIFICATIONINDICATION, job, result);
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    if (EthTSyn_ValidateICVIndication(job->jobId, ETHTSYN_SID_ICVVERIFICATIONINDICATION))
#endif
    {
        EthTSyn_PortType* port = EthTSyn_FindRxPortAcrossJobId(job->jobId);
        if (port != NULL_PTR)
        {
            if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION))
            {
                if (result == E_OK)
                {
                    EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL);
                    /* SWS_EthTSyn_00244 */
                    EthTSyn_ValidFollowUpMessageSetGlobalTime(port, port->RecRawDataPtr, NULL_PTR);
                    port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter = 0u;
                    port->ICVGenarationAndVerification.IcvVerificationAttemptsCounter     = 0u;
                }
                else if (result == E_NOT_OK)
                {
                    EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
                    IdsM_SetSecurityEventWithContextData(
                        ETHTSYN_SEV_TSYN_ETH_ICV_VERIFICATION_FAILED,
                        &port->TimeDomainId,
                        1u);
#endif
                }
                else
                {
                    /* SWS_EthTSyn_00411 */
                    EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY);
                    port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter = 0u;
                    port->ICVGenarationAndVerification.IcvVerificationAttemptsCounter++;
                }
            }
        }
    }
    EthTSyn_ArtiApiReturn(1, ETHTSYN_SID_ICVVERIFICATIONINDICATION);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif

/* ========================================== internal function definitions ========================================= */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         Init the clock identity of the port with the MAC address retrieved from EthIf.
 * @param[in]     port Pointer to the EthTSyn port structure for which the clock identity needs to be initialized.
 * @synchronous   TRUE
 * @reentrant     TRUE (only for different port instances)
 * @trace         CPD-85911
 */
ETHTSYN_LOCAL void EthTSyn_InitClockIdentity(EthTSyn_PortType* port)
{
    uint8 phyAddr[ETHTSYN_PHYS_ADDR_LENGTH];
    EthIf_GetPhysAddr(port->CtrlId, phyAddr);

    uint8* clockIdentity = (uint8*)&port->ThisPortIdentity.ClockIdentity;
    /* IEEE 802.1AS-2011 8.5.2.2 NOLINTBEGIN */ /* PRQA S 3120 ++ */ /* VL_EthTSyn_clockIdentity */
    clockIdentity[0] = phyAddr[0];
    clockIdentity[1] = phyAddr[1];
    clockIdentity[2] = phyAddr[2];
    clockIdentity[3] = 0xFFU;
    clockIdentity[4] = 0xFEU;
    clockIdentity[5] = phyAddr[3];
    clockIdentity[6] = phyAddr[4];
    clockIdentity[7] = phyAddr[5];
    /* NOLINTEND */ /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of getting egress time.
 * @param[in]     ctrlIdx: Index of the Ethernet controller within the context of the Ethernet Interface.
 * @param[in]     portId: Index of the configured Port.
 * @param[in]     bufIdx: Index of the buffer provided by EthIf_ProvideTxBuffer.
 * @param[out]    tv: specifying the engress timestamp of transmitting data,which is determinant by bufIdx when
 * HARDWARE_TIMESTAMP enabled, or the virtual egress time captured by current virtual local time when HARDWARE_TIMESTAMP
 * disabled.
 * @synchronous   TRUE
 * @reentrant    FALSE
 * @trace         CPD-74326
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_GetEgressTV(uint8 ctrlIdx, uint8 portId, EthTSyn_TimeStampType* tv, Eth_BufIdxType bufIdx)
{
    Std_ReturnType ret;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
    StbM_VirtualLocalTimeType VirtualLocalTime;
    ETHTSYN_UNUSED(bufIdx);
    ETHTSYN_UNUSED(ctrlIdx);
    SchM_Enter_EthTSyn_Context();
    ret = StbM_GetCurrentVirtualLocalTime(PORT_TOCFGDOMIN(portId).SynchronizedTimeBaseRef, &VirtualLocalTime);
    if (ret == E_OK)
    {
        *tv = EthTSyn_VTToGT(VirtualLocalTime);
    }
    SchM_Exit_EthTSyn_Context();
#else
    ETHTSYN_UNUSED(portId);
    Eth_TimeStampQualType timeQualPtr = ETH_INVALID;
    Eth_TimeStampType     timeStampPtr;
    ret = EthIf_GetEgressTimeStamp(ctrlIdx, bufIdx, &timeQualPtr, &timeStampPtr);
    if ((ret == E_OK) && (timeQualPtr == ETH_VALID))
    {
        tv->Nanoseconds = timeStampPtr.nanoseconds;
        tv->Seconds     = timeStampPtr.seconds;
        tv->SecondsHi   = timeStampPtr.secondsHi;
    }
    else
    {
        ret = E_NOT_OK;
    }
#endif
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of unpacking the time data in the raw data.
 * @param[in]     data: raw received data.
 * @return        parsed timestamp data.
 * @synchronous   TRUE
 * @reentrant    FALSE
 * @trace         CPD-70673
 */
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_DataToTime(const uint8* data)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    EthTSyn_TimeStampType t;
    t.Nanoseconds = ((uint32)data[6] << ETHTSYN_BIT_SHIFT_24) + ((uint32)data[7] << ETHTSYN_BIT_SHIFT_16)
                    + ((uint32)data[8] << ETHTSYN_BIT_SHIFT_8) + (uint32)data[9];
    t.Seconds = ((uint32)data[2] << ETHTSYN_BIT_SHIFT_24) + ((uint32)data[3] << ETHTSYN_BIT_SHIFT_16)
                + ((uint32)data[4] << ETHTSYN_BIT_SHIFT_8) + (uint32)data[5];
    t.SecondsHi = (uint16)((uint16)(((uint16)data[0]) << ETHTSYN_BIT_SHIFT_8) + (uint16)data[1]);
    /* PRQA S 3120 -- */
    return t;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON) || (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of translating global time to local time.
 * @param[in]     globalTime: global time to be translated to local time.
 * @return        The local time after conversion.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83285
 */
ETHTSYN_LOCAL StbM_VirtualLocalTimeType EthTSyn_GTToVT(EthTSyn_TimeStampType globalTime)
{
    StbM_VirtualLocalTimeType vt;

    uint64 totalSeconds = ((uint64)globalTime.SecondsHi << ETHTSYN_BIT_SHIFT_32) + globalTime.Seconds;

    uint64 totalNanoseconds = (totalSeconds * ETHTSYN_NANOSECOND_TO_SECOND) + globalTime.Nanoseconds;

    vt.nanosecondsLo = (uint32)(totalNanoseconds & ETHTSYN_MAX_MASK_32BIT);
    vt.nanosecondsHi = (uint32)(totalNanoseconds >> ETHTSYN_BIT_SHIFT_32);
    return vt;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of translating local time to global time.
 * @param[in]     virtualLocalTime: virtual local time to be translated to global time.
 * @return        The global time after conversion.
 * @synchronous   TRUE
 * @reentrant    FALSE
 * @trace         CPD-70674
 */
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_VTToGT(StbM_VirtualLocalTimeType virtualLocalTime)
{
    EthTSyn_TimeStampType t;
    uint64 Vt     = (((uint64)virtualLocalTime.nanosecondsHi) << ETHTSYN_BIT_SHIFT_32) + virtualLocalTime.nanosecondsLo;
    t.Nanoseconds = (uint32)(Vt % ETHTSYN_NANOSECOND_TO_SECOND);
    Vt /= ETHTSYN_NANOSECOND_TO_SECOND;
    t.Seconds   = (uint32)Vt;
    t.SecondsHi = (uint16)(Vt >> ETHTSYN_BIT_SHIFT_32);
    return t;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of packing the time data to the data buffer.
 * @param[in]     rawTime: time data to be packed in to buffer.
 * @param[out]    data: data buffer after packing time data.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70676
 */
ETHTSYN_LOCAL void EthTSyn_TimeToData(EthTSyn_TimeStampType rawTime, uint8* data)
{
    uint64 second = (((uint64)rawTime.SecondsHi) << ETHTSYN_BIT_SHIFT_32) + rawTime.Seconds;
    uint8  i;
    for (i = 0u; i < ETHTSYN_SECONDS_SIZE; i++)
    {
        data[i] = (uint8)(((second << (i * ETHTSYN_BIT_SHIFT_PER_BYTE))) >> ETHTSYN_BIT_SHIFT_40);
    }
    for (i = ETHTSYN_NANOSECONDS_BYTE_INDEX_START; i < ETHTSYN_NANOSECONDS_BYTE_INDEX_END; i++)
    {
        data[i] = (uint8)(((rawTime.Nanoseconds << ((i - ETHTSYN_SECONDS_SIZE) * ETHTSYN_BIT_SHIFT_PER_BYTE)))
                          >> ETHTSYN_BIT_SHIFT_24);
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of packing the data into messages and transmitting them.
 * @param[in]     ethTSynMessage: time data to be packed in to buffer.
 * @param[in]     portId: the index of configured port.
 * @synchronous   TRUE
 * @reentrant    FALSE
 * @trace         CPD-85830
 */
ETHTSYN_LOCAL void EthTSyn_TransmitMessage(uint8 ethTSynMessage, uint16 portId)
/* PRQA S 6070 -- */ /* VL_MTR_EthTSyn_STCAL */
{
    Eth_BufIdxType    bufId   = 0u;
    uint8*            dataBuf = NULL_PTR;
    uint8             data[ETHTSYN_BUFFER_TO_TRANSSMIT_LENGTH];
    uint16            dataLenth         = 0u;
    EthTSyn_PortType* port              = EthTSyn_ConfigData->RuntimeVariables[portId];
    boolean           hasCapturedEgress = TRUE;
    uint8             priority          = port->FramePrio;
    uint8             ctrlIdx           = port->CtrlId;
#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON) || (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
    StbM_UserDataType userData;
#endif
    if (port->TransmissionMode == ETHTSYN_TX_ON)
    {
        EthTSyn_PublicInformationPack(port, data, ethTSynMessage);
        switch (ethTSynMessage)
        {
        case SYNC:
            dataLenth = EthTSyn_SyncMessagePack(port, data, &hasCapturedEgress);
            break;
        case FOLLOW_UP:
            dataLenth = EthTSyn_FollowUpMessagePack(port, data);
            break;
        case PDELAY_REQ:
            dataLenth = EthTSyn_ReqMessagePack(port, data);
#if (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
            /* SWS_EthTSyn_00223 */
            if (PORT_TOCFGPORT(portId).EnableTimeValidation)
            {
                StbM_TimeTupleType timeTuple;
                Std_ReturnType     ret = StbM_GetCurrentTime(
                    PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef,
                    &timeTuple,
                    &userData);
                if (ret == E_OK)
                {
                    port->TrefPDInitiator = timeTuple;
                }
            }
#endif
            break;
        case PDELAY_RESP:
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            /*messageLength */
            data[2] = 0u;
            data[3] = 54u;
            /*flags Octet 0*/
            data[6] = 0x02u;
            data[7] = 0x08u;
            /*sequenceId*/
            data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] =
                (uint8)(port->ReceivedPdelayReqSequenceId >> ETHTSYN_BIT_SHIFT_8);
            data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW] = (uint8)port->ReceivedPdelayReqSequenceId;
            /* control */
            data[32] = 0x5u;
            /*logMessageInterval*/
            data[33] = 0x7Fu;
            /*requestReceiptTimestamp*/
            EthTSyn_TimeToData(port->PdelayResponderTimestamp.PdelayReqEventIngressTimestamp, &data[34]);
            /*requestingPortIdentity*/
            (void)IStdLib_MemCpy(&data[44], &EthTSyn_ReceivedPdelayReqSourcePortId[portId][0], 10u);
            dataLenth = 54u;
#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON)
            /* SWS_EthTSyn_00218 */
            if (PORT_TOCFGPORT(portId).EnableTimeValidation)
            {
                StbM_TimeTupleType timeTuple;
                Std_ReturnType     ret = StbM_GetCurrentTime(
                    PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef,
                    &timeTuple,
                    &userData);
                if (ret == E_OK)
                {
                    port->TrefPDResponder = timeTuple;
                }
            }
#endif
            break;
        case PDELAY_RESP_FOLLOW_UP:
            /*messageLength */
            data[2] = 0u;
            data[3] = 54u;
            /*flags Octet 0*/
            data[6] = 0x00u;
            data[7] = 0x08u;
            /*sequenceId*/
            data[30] = (uint8)(port->ReceivedPdelayReqSequenceId >> ETHTSYN_BIT_SHIFT_8);
            data[31] = (uint8)port->ReceivedPdelayReqSequenceId;
            /*control*/
            data[32] = 0x5u;
            /*logMessageInterval*/
            data[33] = 0x7Fu;
            /*responseOriginTimestamp*/
            EthTSyn_TimeToData(port->PdelayResponderTimestamp.PdelayRespEventEgressTimestamp, &data[34]);
            /*requestingPortIdentity*/
            (void)IStdLib_MemCpy(&data[44], &EthTSyn_ReceivedPdelayReqSourcePortId[portId][0], 10u);
            dataLenth = 54u;
            /* PRQA S 3120 -- */
            break;
        default:
            /* do nothing */
            break;
        }
        if (hasCapturedEgress
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
            && (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION)
                || (PORT_TOCFGPORT(portId).GlobalTimeTxIcvGeneration == NULL_PTR) || (ethTSynMessage != FOLLOW_UP))
#endif
        )
        {
            uint16        length    = dataLenth;
            Eth_FrameType frameType = ETHTSYN_TIME_SYNCHRONIZATION_FRAMETYPE;
            /* SWS_EthTSyn_00162 */
            BufReq_ReturnType ret = EthIf_ProvideTxBuffer(ctrlIdx, frameType, priority, &bufId, &dataBuf, &length);
            if ((ret == BUFREQ_OK) && (dataBuf != NULL_PTR) && (length >= dataLenth))
            {
                /* SWS_EthTSyn_00201 SWS_EthTSyn_00202 SWS_EthTSyn_00203 */
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
                if ((ethTSynMessage == PDELAY_REQ) || (ethTSynMessage == PDELAY_RESP) || (ethTSynMessage == SYNC))
                {
                    EthIf_EnableEgressTimeStamp(ctrlIdx, bufId);
                }
#endif
                (void)IStdLib_MemCpy(dataBuf, data, dataLenth);
                port->TranssmitMessageType = ethTSynMessage;
                (void)EthIf_Transmit(ctrlIdx, bufId, frameType, TRUE, dataLenth, &EthTSyn_DestPhyAddr[0]);
            }
            else
            {
                port->SyncSequenceId--;
            }
        }
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of getting the status of timebase,which is related by portID,and returning if the
 *                expected status has been got.
 * @param[in]     portId: the index of configured port.
 * @param[in]     expectedStatus: the expected status of timebase.
 * @synchronous   TRUE
 * @reentrant    FALSE
 * @trace         CPD-70695
 */
ETHTSYN_LOCAL boolean EthTSyn_ExpectedStatusGet(uint16 portId, uint8 expectedStatus)
{
    boolean                 ret;
    StbM_TimeBaseStatusType syncBase;
    uint8                   timebaseid = PORT_TOCFGDOMIN(portId).SynchronizedTimeBaseRef;
    Std_ReturnType          stdRet     = StbM_GetTimeBaseStatus(timebaseid, &syncBase);
    ret                                = (stdRet == E_OK) && ((syncBase & expectedStatus) > 0u);
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of packing the correctionfield into data buffer.
 * @param[in]     ns: correctionfield data to be packed.
 * @param[in]     data: data buffer to be sended.
 * @synchronous   TRUE
 * @reentrant    FALSE
 * @trace         CPD-70696
 */
ETHTSYN_LOCAL void EthTSyn_CreatCorrectionField(uint64 ns, uint8* data)
{
    if (data != NULL_PTR)
    {
        for (uint8 iloop = 0u; iloop < ETHTSYN_NS_BYTES_TO_EXTRACT; iloop++)
        {
            data[iloop] =
                (uint8)(ns >> ((ETHTSTN_NS_BITS_TO_EXTRACT - ETHTSYN_BITS_PER_BYTE) - (iloop * ETHTSYN_BITS_PER_BYTE)));
        }
        data[ETHTSYN_CORRECTIONFIELD_DEFAULT_VALUE_INDEX0] = 0u;
        data[ETHTSYN_CORRECTIONFIELD_DEFAULT_VALUE_INDEX1] = 0u;
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of unpacking the correctionfield into data buffer.
 * @param[in]     data: data buffer to be parsed.
 * @return        the correctionfield data.
 * @synchronous   TRUE
 * @reentrant    FALSE
 * @trace         CPD-70697
 */
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_GetCorrectionField(const uint8* data)
{
    EthTSyn_TimeStampType timeStamp;
    uint64                tempTime = 0ULL;

    for (uint8 iloop = 0u; iloop < ETHTSYN_NS_BYTES_TO_EXTRACT; iloop++)
    {
        tempTime += ((uint64)data[iloop])
                    << ((ETHTSTN_NS_BITS_TO_EXTRACT - ETHTSYN_BITS_PER_BYTE) - (uint8)(iloop * ETHTSYN_BITS_PER_BYTE));
    }
    timeStamp.SecondsHi   = 0U;
    timeStamp.Seconds     = (uint32)(tempTime / ETHTSYN_NANOSECOND_TO_SECOND);
    timeStamp.Nanoseconds = (uint32)(tempTime % ETHTSYN_NANOSECOND_TO_SECOND);

    return timeStamp;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief           Get CRC time flags configuration based on port role
 * @param[in]       portId Port identifier
 * @return          EthTSyn_CrcTimeFlagsTxSecuredCfgType CRC time flags configuration
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-86487
 */
ETHTSYN_LOCAL EthTSyn_CrcTimeFlagsTxSecuredCfgType EthTSyn_GetcrcTimeFlags(uint16 portId)
{
    const EthTSyn_PortType*              port = EthTSyn_ConfigData->RuntimeVariables[portId];
    EthTSyn_CrcTimeFlagsTxSecuredCfgType crcTimeFlags;
    if (port->PortRoleType == ETHTSYN_MASTER)
    {
        crcTimeFlags = *PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeMaster->CrcTimeFlagsTxSecured;
    }
    else
    {
        crcTimeFlags = *PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave->CrcFlagsRxValidated;
    }
    return crcTimeFlags;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of caculating CRC of the sub-tlv in the FU message,which contains the Secured Time.
 * @param[in]     portId: the index of configured port.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[out]    crc0: the CRC_Time_0.
 * @param[out]    crc1: the CRC_Time_1.
 * @synchronous   TRUE
 * @reentrant    TRUE
 * @trace         CPD-86492
 */
ETHTSYN_LOCAL void
    EthTSyn_CalculateTimeSecuredCrc(uint16 portId, const uint8* data, uint16 offset, uint8* crc0, uint8* crc1)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint8 dataID = PORT_TOCFGDOMIN(portId)
                       .GlobalTimeFollowUpDataIDList->GlobalTimeFollowUpDataIDListElement[data[31] % 16u]
                       .GlobalTimeFollowUpDataIDListValue;
    EthTSyn_CrcTimeFlagsTxSecuredCfgType crcTimeFlags = EthTSyn_GetcrcTimeFlags(portId);
    /* CRC_Time_Flags */
    uint8 crcData0[ETHTSYN_CRC_DATA_0_LENGTH];
    uint8 crcData1[ETHTSYN_CRC_DATA_1_LENGTH];
    (void)IStdLib_MemSet(&crcData0[0u], 0u, ETHTSYN_CRC_DATA_0_LENGTH);
    (void)IStdLib_MemSet(&crcData1[0u], 0u, ETHTSYN_CRC_DATA_1_LENGTH);
    uint8 crcData0Length = 1u;
    uint8 crcData1Length = 1u;
    crcData0[0u]         = data[offset + 2u];
    crcData1[0u]         = data[offset + 2u];
    /* CRCMessageLength */
    if (crcTimeFlags.CrcMessageLength)
    {
        crcData1[crcData1Length]      = data[2];
        crcData1[crcData1Length + 1u] = data[3];
        crcData1Length += 2u;
    }
    /* CRCDomainNumber */
    if (crcTimeFlags.CrcDomainNumber)
    {
        crcData0[crcData0Length] = data[4];
        crcData0Length++;
    }
    /* CrcCorrectionField */
    if (crcTimeFlags.CrcCorrectionField)
    {
        (void)IStdLib_MemCpy(&crcData1[crcData1Length], &data[8], 8u);
        crcData1Length += 8u;
    }
    /* CRCSequenceIdentity */
    if (crcTimeFlags.CrcSequenceId)
    {
        crcData1[crcData1Length]      = data[30];
        crcData1[crcData1Length + 1u] = data[31];
        crcData1Length += 2u;
    }
    /* sourcePortIdentity */
    if (crcTimeFlags.CrcSourcePortIdentity)
    {
        (void)IStdLib_MemCpy(&crcData0[crcData0Length], &data[20u], 10u);
        crcData0Length += 10u;
    }
    /* preciseOriginTimestamp */
    if (crcTimeFlags.CrcPreciseOriginTimestamp)
    {
        (void)IStdLib_MemCpy(&crcData0[crcData0Length], &data[34], 10u);
        crcData0Length += 10u;
    }
    crcData0[crcData0Length] = dataID;
    crcData1[crcData1Length] = dataID;
    /* CRC_Time_0 */
    *crc0 = Crc_CalculateCRC8H2F(&crcData0[0], (uint32)crcData0Length + 1u, 0xFFu, TRUE);
    /* CRC_Time_1 */
    *crc1 = Crc_CalculateCRC8H2F(&crcData1[0], (uint32)crcData1Length + 1u, 0xFFu, TRUE);
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Secured Time.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrant    TRUE
 * @trace         CPD-77968
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyTimeSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId)
{
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave;
    Std_ReturnType                        ret                   = E_NOT_OK;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_TIME_LENGTH + 1u) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_TIME_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVTime)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        case ETHTSYN_CRC_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            uint8 crcTime0 = 0u;
            uint8 crcTime1 = 0u;
            EthTSyn_CalculateTimeSecuredCrc(portId, data, offset + 1u, &crcTime0, &crcTime1);
            if ((crcTime0 != data[offset + ETHTSYN_CRC_TIME0_OFFSET + 1u])
                || (crcTime1 != data[offset + ETHTSYN_CRC_TIME1_OFFSET + 1u]))
            {
                return E_NOT_OK;
            }
            ret = E_OK;
            break;
        }
#endif
        case ETHTSYN_CRC_IGNORED:
            /* PRS_TS_00108*/
            ret = E_OK;
            break;
        default:
            /* ETHTSYN_CRC_NOT_VALIDATED PRS_TS_00107 do nothing */
            break;
        }
    }
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Not Secured status.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @param[inout]  isSyncToGatewayBitSet: the flag to indicate if the syncToGateway bit is set.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-85831
 */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyStatusNotSecuredSubTlv(
    const uint8* data,
    uint16       length,
    uint16       offset,
    uint16       portId,
    boolean*     isSyncToGatewayBitSet)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_STATUS_LENGTH + 1u) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_STATUS_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVStatus)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
        /* PRS_TS_00107 PRS_TS_00108 PRS_TS_00109 */
        case ETHTSYN_CRC_IGNORED:
        case ETHTSYN_CRC_NOT_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            /* PRS_TS_00156*/
            if ((data[offset + ETHTSYN_SUBTLV_STATUS_STATUS + 1u] & ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV) > 0U)
            {
                /* PRS_TS_00156*/
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
                EthTSyn_PortType* port      = EthTSyn_ConfigData->RuntimeVariables[portId];
                port->IsSyncToGatewayBitSet = TRUE;
                ETHTSYN_UNUSED(isSyncToGatewayBitSet);
#else
                *isSyncToGatewayBitSet = TRUE;
#endif
            }
            ret = E_OK;
            break;
        }
        default:
            /* ETHTSYN_CRC_VALIDATED do nothing */
            break;
        }
    }
    /* PRQA S 3120 -- */
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Secured status.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @param[inout]  isSyncToGatewayBitSet: the flag to indicate if the syncToGateway bit is set.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-85832
 */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyStatusSecuredSubTlv(
    const uint8* data,
    uint16       length,
    uint16       offset,
    uint16       portId,
    boolean*     isSyncToGatewayBitSet)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_STATUS_LENGTH + 1u) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_STATUS_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVStatus)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        /* PRS_TS_00107 PRS_TS_00109*/
        case ETHTSYN_CRC_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            uint8 crcData[ETHTSYN_CRC_DATA_LENGTH];
            crcData[0u] = data[offset + 3u];
            crcData[1u] = PORT_TOCFGDOMIN(portId)
                              .GlobalTimeFollowUpDataIDList->GlobalTimeFollowUpDataIDListElement[data[31] % 16u]
                              .GlobalTimeFollowUpDataIDListValue;
            uint8 crcDataResult = Crc_CalculateCRC8H2F(&crcData[0u], 2u, 0xFFu, TRUE);
            if (crcDataResult == data[offset + 4u])
            {
                /*PRS_TS_00156*/
                if ((data[offset + ETHTSYN_SUBTLV_STATUS_STATUS + 1u] & ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV) > 0U)
                {
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
                    EthTSyn_PortType* port      = EthTSyn_ConfigData->RuntimeVariables[portId];
                    port->IsSyncToGatewayBitSet = TRUE;
                    ETHTSYN_UNUSED(isSyncToGatewayBitSet);
#else
                    *isSyncToGatewayBitSet = TRUE;
#endif
                }
                ret = E_OK;
            }
            break;
        }
#endif
        /* PRS_TS_00108*/
        case ETHTSYN_CRC_IGNORED:
            /*PRS_TS_00156*/
            if ((data[offset + ETHTSYN_SUBTLV_STATUS_STATUS + 1u] & ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV) > 0U)
            {
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
                EthTSyn_PortType* port      = EthTSyn_ConfigData->RuntimeVariables[portId];
                port->IsSyncToGatewayBitSet = TRUE;
                ETHTSYN_UNUSED(isSyncToGatewayBitSet);
#else
                *isSyncToGatewayBitSet = TRUE;
#endif
            }
            ret = E_OK;
            break;
        default: /* ETHTSYN_CRC_NOT_VALIDATED */ /* PRQA S 3120 */ /* VL_QAC_EmptyClause*/
            break;
        }
    }

    return ret;
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Not Secured userdata.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrant    TRUE
 * @trace         CPD-77965
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyUserDataNotSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId)
{
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVUserData)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
        /* PRS_TS_00107 PRS_TS_00108 PRS_TS_00109*/
        case ETHTSYN_CRC_IGNORED:
        case ETHTSYN_CRC_NOT_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            EthTSyn_PortType* port        = EthTSyn_ConfigData->RuntimeVariables[portId];
            port->UserData.userDataLength = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_DATA_LENGTH + 1u];
            port->UserData.userByte0      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_0 + 1u];
            port->UserData.userByte1      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_1 + 1u];
            port->UserData.userByte2      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_2 + 1u];
            ret                           = E_OK;
            break;
        }
        default: /* ETHTSYN_CRC_VALIDATED */ /* PRQA S 3120 */ /* VL_QAC_EmptyClause*/
            /* do nothing */
            break;
        }
    }

    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Secured userdata.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     length: the length of FU message.
 * @param[in]     offset: the start offset byte index of Sub-tlv field.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrant    TRUE
 * @trace         CPD-77964
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyUserDataSecuredSubTlv(const uint8* data, uint16 length, uint16 offset, uint16 portId)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave;
    if (((offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH) <= length)
        && (data[offset + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] == ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH)
        && globalTimeSlaveCfgPtr->RxSubTLVUserData)
    {
        switch (globalTimeSlaveCfgPtr->RxCrcValidated)
        {
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        /* PRS_TS_00107 PRS_TS_00109 PRS_TS_00116*/
        case ETHTSYN_CRC_VALIDATED:
        case ETHTSYN_CRC_OPTIONAL:
        {
            uint8 crcData[ETHTSYN_CRC_DATA_LENGTH];
            (void)IStdLib_MemCpy(&crcData[0u], &data[offset + 3u], 4u);
            crcData[4u] = PORT_TOCFGDOMIN(portId)
                              .GlobalTimeFollowUpDataIDList->GlobalTimeFollowUpDataIDListElement[data[31] % 16u]
                              .GlobalTimeFollowUpDataIDListValue;
            uint8 crcDataResult = Crc_CalculateCRC8H2F(&crcData[0u], 5u, 0xFFu, TRUE);
            if (crcDataResult == data[offset + 7u])
            {
                EthTSyn_PortType* port        = EthTSyn_ConfigData->RuntimeVariables[portId];
                port->UserData.userDataLength = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_DATA_LENGTH + 1u];
                port->UserData.userByte0      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_0 + 1u];
                port->UserData.userByte1      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_1 + 1u];
                port->UserData.userByte2      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_2 + 1u];
                ret                           = E_OK;
            }
            break;
        }
#endif
        case ETHTSYN_CRC_IGNORED:
        {
            EthTSyn_PortType* port        = EthTSyn_ConfigData->RuntimeVariables[portId];
            port->UserData.userDataLength = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_DATA_LENGTH + 1u];
            port->UserData.userByte0      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_0 + 1u];
            port->UserData.userByte1      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_1 + 1u];
            port->UserData.userByte2      = data[offset + ETHTSYN_SUBTLV_USERDATA_USER_BYTE_2 + 1u];
            ret                           = E_OK;
            break;
        }
        default: /* ETHTSYN_CRC_NOT_VALIDATED */
            break;
        }
    }
    return ret;
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of verifying the header of sub-tlv in the FU message.
 * @param[in]     data: raw data buffer to be validated.
 * @retval        E_OK: header validation passed.
 * @retval        E_NOT_OK: header validation failed.
 * @synchronous   TRUE
 * @reentrant    TRUE
 * @trace         CPD-77963
 */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_VerifyTLVHeader(const uint8* data)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16         Tlv_Length;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Tlv_Length = (((uint16)(((uint16)(((data))[(2u)])) << 8u)) | ((uint16)((uint8)(((data))[3u]))));

    /* Verify header: */
    if (
        /* TLV type */
        (data[ETHTSYN_FUP_TLV_TYPE_1] == 0U) && (data[ETHTSYN_FUP_TLV_TYPE_0] == 3U) &&
        /* length field */
        (Tlv_Length > ETHTSYN_FUP_EXT_TLV_LENGTH_0) &&

        /* organizationId */
        (data[ETHTSYN_FUP_TLV_ORGID_2] == ETHTSYN_FUP_EXT_TLV_ORGID_2)
        && (data[ETHTSYN_FUP_TLV_ORGID_1] == ETHTSYN_FUP_EXT_TLV_ORGID_1)
        && (data[ETHTSYN_FUP_TLV_ORGID_0] == ETHTSYN_FUP_EXT_TLV_ORGID_0) &&
        /* organizationSubType */
        (data[ETHTSYN_FUP_TLV_ORG_SUBTYPE_2] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2)
        && (data[ETHTSYN_FUP_TLV_ORG_SUBTYPE_1] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1)
        && (data[ETHTSYN_FUP_TLV_ORG_SUBTYPE_0] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0))
    {
        ret = E_OK;
    }

    return ret;
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief           Parse and verify sub-TLVs in Follow_Up message
 * @param[in]       data Pointer to received message data
 * @param[in]       msgLength Total length of received message
 * @param[in]       portId Port identifier for configuration lookup
 * @param[out]      isSyncToGatewayBitSet Pointer to store Sync to Gateway bit status
 * @return          Std_ReturnType E_OK if parsing successful, E_NOT_OK otherwise
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-86488
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_ParseAndVerifySubTlvs(const uint8* data, uint16 msgLength, uint16 portId, boolean* isSyncToGatewayBitSet)
{
    Std_ReturnType                        ret                   = E_OK;
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave;
    uint16                                datap                 = ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR - 1u;
    boolean                               rxSubTLVStatus        = FALSE;
    boolean                               rxSubTLVTime          = FALSE;
    boolean                               rxSubTLVUserData      = FALSE;
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    boolean rxSubTLVTimeAuthenticated = FALSE;
    boolean configRxSubTLVTimeAuthenticated =
        globalTimeSlaveCfgPtr->RxIcvVerification == ETHTSYN_ICV_NOT_VERIFIED ? FALSE : TRUE;
#endif
    while (datap < (msgLength - 1u))
    {
        if ((datap + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET) >= msgLength)
        {
            return E_NOT_OK;
        }
        switch ((uint8)data[datap + 1u])
        {
        case ETHTSYN_SUB_TLV_TYPE_TIME_SECURED:
            ret          = EthTSyn_VerifyTimeSecuredSubTlv(&data[0u], msgLength, datap, portId);
            rxSubTLVTime = TRUE;
            break;
        case ETHTSYN_SUB_TLV_TYPE_STATUS_NOT_SECURED:
            ret = EthTSyn_VerifyStatusNotSecuredSubTlv(&data[0u], msgLength, datap, portId, isSyncToGatewayBitSet);
            rxSubTLVStatus = TRUE;
            break;
        case ETHTSYN_SUB_TLV_TYPE_STATUS_SECURED:
            ret = EthTSyn_VerifyStatusSecuredSubTlv(&data[0u], msgLength, datap, portId, isSyncToGatewayBitSet);
            rxSubTLVStatus = TRUE;
            break;
        case ETHTSYN_SUB_TLV_TYPE_USERDATA_NOT_SECURED:
            ret              = EthTSyn_VerifyUserDataNotSecuredSubTlv(&data[0u], msgLength, datap, portId);
            rxSubTLVUserData = TRUE;
            break;
        case ETHTSYN_SUB_TLV_TYPE_USERDATA_SECURED:
            ret              = EthTSyn_VerifyUserDataSecuredSubTlv(&data[0u], msgLength, datap, portId);
            rxSubTLVUserData = TRUE;
            break;
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        case ETHTSYN_SUB_TLV_TYPE_TIME_AUTHENTICATED:
            if (!rxSubTLVTimeAuthenticated)
            {
                ret                       = EthTSyn_VerifyTimeAuthenticatedSubTlv(&data[0u], datap, msgLength, portId);
                rxSubTLVTimeAuthenticated = TRUE;
            }
            break;
#endif
        default:
            ret = ETHTSYN_E_UNRECOGNIZED_TLV;
            break;
        }
        if (ret == E_NOT_OK)
        {
            break;
        }
        datap += (uint16)data[datap + ETHTSYN_FUP_SUBTLV_LENGTH_OFFSET] + ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
    }
    if ((globalTimeSlaveCfgPtr->RxSubTLVStatus != rxSubTLVStatus)
        || (globalTimeSlaveCfgPtr->RxSubTLVTime != rxSubTLVTime)
        || (globalTimeSlaveCfgPtr->RxSubTLVUserData != rxSubTLVUserData)
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        /* PRS_TS_00246 PRS_TS_00247*/
        || (((globalTimeSlaveCfgPtr->RxIcvVerification != ETHTSYN_ICV_IGNORED)
             && (globalTimeSlaveCfgPtr->RxIcvVerification != ETHTSYN_ICV_OPTIONAL))
            && rxSubTLVTimeAuthenticated != configRxSubTLVTimeAuthenticated)
#endif
    )
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of validating the crc of sub-tlv in the FU message.
 * @param[in]     portId: the index of configured port.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     frameLength: the length of FU message.
 * @param[inout]  isSyncToGatewayBitSet: the flag to indicate if the syncToGateway bit is set.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrant    TRUE
 * @trace         CPD-85833
 */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_TlvScanAndCRCValidation(
    uint16       portId,
    const uint8* data,
    uint16       frameLength,
    boolean*     isSyncToGatewayBitSet)
{
    Std_ReturnType                        ret                   = E_OK;
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave;
    uint16 msgLength = (uint16)((uint16)data[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET] << ETHTSYN_BIT_SHIFT_8)
                       | data[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET + 1U];
    if ((msgLength != frameLength)
        || ((EthTSyn_VerifyTLVHeader(&data[ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR]) != E_OK)
            && (globalTimeSlaveCfgPtr->RxSubTLVStatus || globalTimeSlaveCfgPtr->RxSubTLVTime
                || globalTimeSlaveCfgPtr->RxSubTLVUserData)))
    {
        return E_NOT_OK;
    }
    ret = EthTSyn_ParseAndVerifySubTlvs(data, msgLength, portId, isSyncToGatewayBitSet);
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    if (ret == E_NOT_OK)
    {
        EthTSyn_PortType* port = EthTSyn_ConfigData->RuntimeVariables[portId];
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL);
    }
#endif
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif

#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
/**
 * @brief         specifying dose the timebase associated with this port relate to the Ethernet hardware timestamp.
 * @param[in]     portId: the index of configured port.
 * @retval        TRUE:  the timebase associated with this port relate to the Ethernet hardware timestamp.
 * @retval        FALSE:  the timebase dose not associated with this port relate to the Ethernet hardware timestamp.
 * @synchronous   TRUE
 * @reentrant    TRUE
 * @trace         CPD-70699
 */
ETHTSYN_LOCAL_INLINE boolean EthTSyn_StbMEthHWUsed(uint16 portId)
{
    return PORT_TOCFGPORT(portId).SynchronizedTimeBaseUseEthFreerunning;
}
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of compasion between two global timestamps.
 * @param[in]     t1: timestamp to be compared.
 * @param[in]     t2: timestamp to be compared.
 * @return        The positive or negative result of subtracting two timestamps.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70703
 */
ETHTSYN_LOCAL sint32 EthTSyn_TimeStampCmp(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2)
{
    uint64 t1Seconds = ((uint64)t1.SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)t1.Seconds;
    uint64 t2Seconds = ((uint64)t2.SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)t2.Seconds;
    sint32 sign;
    if (t1Seconds < t2Seconds)
    {
        sign = -1;
    }
    else if (t1Seconds == t2Seconds)
    {
        sint32 tdNanoseconds = (sint32)t1.Nanoseconds - (sint32)t2.Nanoseconds;
        if (tdNanoseconds < 0L)
        {
            sign = -1;
        }
        else if (tdNanoseconds == 0L)
        {
            sign = 0;
        }
        else
        {
            sign = 1;
        }
    }
    else
    {
        sign = 1;
    }
    return sign;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of optimizing the timestamp.
 * @param[inout]  timestamp: timestamp to be optimize.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70700
 */
ETHTSYN_LOCAL void EthTSyn_timeoptimal(EthTSyn_TimeStampType* timestamp)
{
    while (timestamp->Nanoseconds >= ETHTSYN_NANOSECOND_TO_SECOND)
    {
        timestamp->Seconds++;
        if (timestamp->Seconds == 0ULL)
        {
            timestamp->SecondsHi++;
        }
        else
        {
            /* do nothing */
        }
        timestamp->Nanoseconds -= ETHTSYN_NANOSECOND_TO_SECOND;
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of addition between two timestamps.
 * @param[in]     time1: timestamp to be added.
 * @param[in]     time2: timestamp to be added.
 * @return        the timestamp after addition between two timestamp.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70701
 */
ETHTSYN_LOCAL EthTSyn_TimeStampType EthTSyn_timeadd(EthTSyn_TimeStampType time1, EthTSyn_TimeStampType time2)
{
    uint64 second1 = (((uint64)time1.SecondsHi) << ETHTSYN_BIT_SHIFT_32) + time1.Seconds;
    uint64 second2 = (((uint64)time2.SecondsHi) << ETHTSYN_BIT_SHIFT_32) + time2.Seconds;
    uint64 second  = second1 + second2;

    EthTSyn_TimeStampType t;
    t.Nanoseconds = time1.Nanoseconds + time2.Nanoseconds;
    t.Seconds     = (uint32)second;
    t.SecondsHi   = (uint16)(second >> ETHTSYN_BIT_SHIFT_32);
    EthTSyn_timeoptimal(&t);
    return t;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         specifying if time1 is less than time2.
 * @param[in]     time1: global timestamp to be compared.
 * @param[in]     time2: locall time to be compared.
 * @retval        TRUE: time1 is less than time2.
 * @retval        FALSE: time1 is not less than time2.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70702
 */
ETHTSYN_LOCAL boolean EthTSyn_timeless(EthTSyn_TimeStampType time1, uint64 time2)
{
    boolean ret = TRUE;
    if (time2 <= ETHTSYN_NANOSECOND_TO_SECOND)
    {
        if ((time1.Nanoseconds > time2) || (time1.Seconds > 0ULL) || (time1.SecondsHi > 0UL))
        {
            ret = FALSE;
        }
    }
    else
    {
        uint64 second = (((uint64)time1.SecondsHi) << ETHTSYN_BIT_SHIFT_32) + time1.Seconds;
        if (second > (time2 / ETHTSYN_NANOSECOND_TO_SECOND))
        {
            ret = FALSE;
        }
        if (second == (time2 / ETHTSYN_NANOSECOND_TO_SECOND))
        {
            if (time1.Nanoseconds > (time2 % ETHTSYN_NANOSECOND_TO_SECOND))
            {
                ret = FALSE;
            }
        }
    }
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of subtraction  between two timestamps.
 * @param[in]     t1: timestamp to be subtracted.
 * @param[in]     t2: timestamp to be subtracted.
 * @param[out]    td: the timestamp after subtraction between two timestamp.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70704
 */
ETHTSYN_LOCAL void EthTSyn_TimeStampSub(EthTSyn_TimeStampType t1, EthTSyn_TimeStampType t2, EthTSyn_TimeStampType* td)
{
    EthTSyn_TimeStampType t1Copy = t1;
    EthTSyn_TimeStampType t2Copy = t2;
    EthTSyn_timeoptimal(&t1Copy);
    EthTSyn_timeoptimal(&t2Copy);
    uint64 tdSeconds = (((uint64)t1.SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)t1.Seconds)
                       - (((uint64)t2.SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)t2.Seconds);
    uint32 tdUnsignedNanoseconds = 0U;
    if ((sint32)((sint32)t1.Nanoseconds - (sint32)t2.Nanoseconds) < 0LL)
    {
        tdUnsignedNanoseconds = ETHTSYN_NANOSECOND_TO_SECOND - (t2.Nanoseconds - t1.Nanoseconds);
        tdSeconds--;
    }
    else
    {
        tdUnsignedNanoseconds = t1.Nanoseconds - t2.Nanoseconds;
    }

    td->SecondsHi   = (uint16)((uint64)tdSeconds >> ETHTSYN_BIT_SHIFT_32);
    td->Seconds     = (uint32)tdSeconds;
    td->Nanoseconds = tdUnsignedNanoseconds;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         specifying if two portIdentity are the same.
 * @param[in]     portIdentity1: portIdentity to be compared.
 * @param[in]     portIdentity2: portIdentity to be compared.
 * @retval        TRUE: two portIdentity are the same.
 * @retval        FALSE: two portIdentity are not the same.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70708
 */
ETHTSYN_LOCAL boolean EthTSyn_IsPortIdentityEqual(const uint8* portIdentity1, const uint8* portIdentity2)
{
    return IStdLib_MemCmp(portIdentity1, portIdentity2, ETHTSYN_PORT_IDENTITY_LENGTH) == 0u;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 6080 ++ */ /* VL_MTR_EthTSyn_STPTH */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of handling the port of master.
 * @param[in]     portIndex: Index of the configured Port.
 * @return        the type of message to be sended.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70849
 */
ETHTSYN_LOCAL uint8 EthTSyn_MainFunctionMaster(uint8 portIndex)
/* PRQA S 6080 -- */
{
    uint8             syncMessageType = MSG_NONE;
    EthTSyn_PortType* portPtr         = EthTSyn_ConfigData->RuntimeVariables[portIndex];

    uint64 globalTimeTxPeriod = PORT_TOCFGDOMIN(portIndex).PortRole.GlobalTimeMaster->GlobalTimeTxPeriod;
    if ((globalTimeTxPeriod > 0ULL) && (portPtr->SyncReqPeriod > 0ULL))
    {
        --portPtr->SyncReqPeriod;
    }
    if (portPtr->SyncdebounceCounter == 0u)
    {
        if (portPtr->SyncStatusType == ETHTSYN_SEND_SYNC)
        {
            boolean isImmediate = FALSE;
            if (PORT_TOCFGDOMIN(portIndex).PortRole.GlobalTimeMaster->ImmediateTimeSync)
            {
                uint8 timeBaseUpdateCounter =
                    StbM_GetTimeBaseUpdateCounter(PORT_TOCFGDOMIN(portIndex).SynchronizedTimeBaseRef);
                if (EthTSyn_ExpectedStatusGet(portIndex, STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE)
                    && (timeBaseUpdateCounter != portPtr->TimeBaseUpdateCounter))
                {
                    syncMessageType                = SYNC;
                    isImmediate                    = TRUE;
                    portPtr->TimeBaseUpdateCounter = timeBaseUpdateCounter;
                }
            }

            if (EthTSyn_ExpectedStatusGet(portIndex, STBM_TIMEBASE_STATUS_GLOBAL_TIME_BASE)
                && (globalTimeTxPeriod > 0ULL) && (portPtr->SyncReqPeriod == 0ULL))
            {
                syncMessageType = SYNC;
            }

            if (syncMessageType == SYNC)
            {
                portPtr->SyncReqPeriod = isImmediate
                                             ? PORT_TOCFGDOMIN(portIndex).PortRole.GlobalTimeMaster->CyclicMsgResumeTime
                                             : globalTimeTxPeriod;
            }
        }
        else
        {
            syncMessageType = FOLLOW_UP;
        }
    }
    return syncMessageType;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of initializing the port status.
 * @param[inout]  portPtr: port pointer.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70850
 */
ETHTSYN_LOCAL void EthTSyn_InitPortStatus(EthTSyn_PortType* portPtr)
{
    SchM_Enter_EthTSyn_Context();
    portPtr->PdelayReqPeriod             = 0u;
    portPtr->SyncReqPeriod               = 0u;
    portPtr->PdelayTXStatusType          = ETHTSYN_SEND_PDELAY_REQ;
    portPtr->PdelayRXStatusType          = ETHTSYN_WAITFOR_PDELAY_REQ;
    portPtr->SyncStatusType              = ETHTSYN_SEND_SYNC;
    portPtr->FollowUpTimeoutTicks        = 0u;
    portPtr->RxDebounceTimeTicks         = 0u;
    portPtr->FirstlyReceivedSyncReq      = TRUE;
    portPtr->SyncSequenceId              = ETHTSYN_MAX_SEQUENCEID;
    portPtr->PdelayReqSequenceId         = ETHTSYN_MAX_SEQUENCEID;
    portPtr->ReceivedPdelayReqSequenceId = 0u;
    portPtr->SyncdebounceCounter         = 0u;
    SchM_Exit_EthTSyn_Context();
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of validating pointer,if the pointer is NULL_PTR.
 * @param[in]     apiId: the id of application.
 * @param[in]     pointer: the pointer to be validated.
 * @retval        TRUE: the pointer is not NULL_PTR.
 * @retval        FALSE: the pointer is NULL_PTR.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70851
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean valid = pointer != NULL_PTR;
    if (!valid)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_PARAM_POINTER);
    }
    return valid;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of validating EthTSyn_Init.
 * @param[in]     configPtr: the pointer of configuration.
 * @retval        TRUE: validation passed.
 * @retval        FALSE: validation failed.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70852
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateInit(const EthTSyn_ConfigType* configPtr)
{
    boolean valid = FALSE;
    if (EthTSyn_Initialized[EthTSyn_GetPartitionIndex()] == TRUE)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_INIT, ETHTSYN_E_ALREADY_INITIALIZED);
    }
    else if (configPtr == NULL_PTR)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_INIT, ETHTSYN_E_INIT_FAILED);
    }
    else
    {
        valid = TRUE;
    }
    return valid;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of validation whether it has been initialized.
 * @param[in]     apiId: the ID of appication.
 * @retval        TRUE: validation passed.
 * @retval        FALSE: validation failed.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-70988
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateInitStatus(uint8 apiId)
{
    boolean valid = EthTSyn_Initialized[EthTSyn_GetPartitionIndex()];
    if (!valid)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_UNINIT);
    }
    return valid;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        find the index of configured port related to ctrlIdx.
 * @param[in]    ctrlIdx   Identification of the controllor.
 * @param[inout] portIdx   the index of configured port related to ctrlIdx.
 * @return       the runtime variable(EthTSyn_Port) which is related to the ctrlIdx.
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-85834
 */
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindPort(uint8 ctrlIdx, uint8* portIdx)
{
    EthTSyn_PortType* port = NULL_PTR;

    uint8 portIndex = 0u;
    for (portIndex = 0u; portIndex < EthTSyn_ConfigData->DomainNum; ++portIndex)
    {
        if (EthTSyn_ConfigData->RuntimeVariables[portIndex]->CtrlId == ctrlIdx)
        {
            port = EthTSyn_ConfigData->RuntimeVariables[portIndex];
            break;
        }
    }
    if (portIdx != NULL_PTR)
    {
        *portIdx = portIndex;
    }
    else
    {
        ETHTSYN_UNUSED(portIdx);
    }
    return port;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of checking,if the ctrlIdx is valid.
 * @param[in]    apiId   the index of application.
 * @param[in]    ctrlIdx   the index of controller to be validated.
 * @retval       TRUE: validation passed.
 * @retval       FALSE: validation failed.
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-85835
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateCtrlIdx(uint8 apiId, uint8 ctrlIdx)
{
    boolean valid = FALSE;
    if (EthTSyn_ValidateInitStatus(apiId))
    {
        uint8 portIdx = 0u;
        (void)EthTSyn_FindPort(ctrlIdx, &portIdx);
        if (portIdx >= EthTSyn_ConfigData->DomainNum)
        {
            (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_CTRL_IDX);
        }
#if ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON
        else if (PORT_TOCFGDOMIN(portIdx).PortReferrencedPartitionId != GetApplicationID())
        {
            (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_INVALID_PARTITION_CONTEXT);
        }
#endif
        else
        {
            valid = TRUE;
        }
    }
    return valid;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of validating EthTSyn_Rxindication.
 * @param[in]    ctrlIdx   the index of controller to be validated.
 * @param[in]    pointer   the pointer to be validated.
 * @retval       TRUE: validation passed.
 * @retval       FALSE: validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71031
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateRxindication(uint8 ctrlIdx, const void* pointer)
{
    return EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_RXINDICATION, ctrlIdx)
           && EthTSyn_ValidatePointer(ETHTSYN_SID_RXINDICATION, pointer);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of validating TransmissionMode.
 * @param[in]    ctrlIdx   the index of controller to be validated.
 * @param[in]    mode   the Mode to be validated.
 * @retval       TRUE: validation passed.
 * @retval       FALSE: validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71032
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateTransmissionMode(uint8 ctrlIdx, EthTSyn_TransmissionModeType mode)
{
    boolean valid = EthTSyn_ValidateCtrlIdx(ETHTSYN_SID_SETTRANSMISSIONMODE, ctrlIdx);
    if (valid && ((uint8)mode > ETHTSYN_TX_ON_MAX_VALUE))
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_SETTRANSMISSIONMODE, ETHTSYN_E_PARAM);
        valid = FALSE;
    }
    return valid;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of validating of invalid partition context
 * @param[in]    apiId  ID of API service in which error is detected
 * @retval       TRUE   validation passed.
 * @retval       FALSE  validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71033
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidatePartitionContext(uint8 apiId)
{
    if (EthTSyn_GetPartitionIndex() >= ETHTSYN_PARTITION_NUMBER)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_INVALID_PARTITION_CONTEXT);
        return FALSE;
    }
    return TRUE;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of validating of EthTSyn_SetGetProtocolParam and EthTSyn_GetProtocolParam.
 * @param[in]    timeBaseId  ID of the synchronized time base.
 * @param[in]    protocolParam  structure with Follow_Up information TLV parameters.
 * @param[in]    apiId  ID of API service in which error is detected.
 * @retval       TRUE   validation passed.
 * @retval       FALSE  validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-83024
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateSetOrGetProtocolParam(
    StbM_SynchronizedTimeBaseType timeBaseId,
    const StbM_ProtocolParamType* protocolParam,
    uint8                         apiId)
{
    const EthTSyn_PortType* port = EthTSyn_FindPortAcrossTimeBase((uint8)timeBaseId);
    if (port == NULL_PTR)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_PARAM);
        return FALSE;
    }
    else if (protocolParam == NULL_PTR)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_PARAM_POINTER);
        return FALSE;
    }
    else
    {
        /* do nothing */
    }

    return TRUE;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of validating of EthTSyn_IcvGenerationIndication and
 * EthTSyn_IcvVerificationIndication.
 * @param[in]    jobId  JobID of the operation that caused the callback.
 * @param[in]    apiId  ID of API service in which error is detected.
 * @retval       TRUE   validation passed.
 * @retval       FALSE  validation failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85836
 */
ETHTSYN_LOCAL boolean EthTSyn_ValidateICVIndication(uint32 jobId, uint8 apiId)
{
    if (!EthTSyn_ValidateInitStatus(apiId))
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_UNINIT);
        return FALSE;
    }
    else if (EthTSyn_GetPartitionIndex() >= ETHTSYN_PARTITION_NUMBER)
    {
        (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_INVALID_PARTITION_CONTEXT);
        return FALSE;
    }
    else
    {
        EthTSyn_PortType* port = EthTSyn_FindTxPortAcrossJobId(jobId);
        if (port == NULL_PTR)
        {
            (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, apiId, ETHTSYN_E_PARAM);
            return FALSE;
        }
    }
    return TRUE;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of getting the index of partition.
 * @return       the index of partition.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-71034
 */
ETHTSYN_LOCAL uint8 EthTSyn_GetPartitionIndex(void)
{
    uint8 partitionIndex = 0u;
#if ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType application = GetApplicationID();
    for (; partitionIndex < ETHTSYN_PARTITION_NUMBER; ++partitionIndex)
    {
        if (EthTSyn_OsApplicationList[partitionIndex] == application)
        {
            break;
        }
    }
#endif
    return partitionIndex;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of getting the hard ingress timestamp if EthTSynHardwareTimestampSupport is TRUE.
 * @param[in]    ctrlIdx index of controllor.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @retval       TRUE   ingress timestamp has been captured successfully.
 * @retval       FALSE  ingress timestamp has been captured failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73917
 */
ETHTSYN_LOCAL boolean EthTSyn_GetHardIngressTime(uint8 ctrlIdx, const uint8* dataPtr, EthTSyn_TimeStampType* tVnow)
{
    boolean               isValidIngressTime = TRUE;
    Eth_TimeStampQualType timeQualPtr        = ETH_INVALID;
    Eth_TimeStampType     timeStampPtr;
    Std_ReturnType ret = EthIf_GetIngressTimeStamp(ctrlIdx, (const Eth_DataType*)dataPtr, &timeQualPtr, &timeStampPtr);
    if ((ret == E_OK) && (timeQualPtr == ETH_VALID))
    {
        tVnow->Nanoseconds = timeStampPtr.nanoseconds;
        tVnow->Seconds     = timeStampPtr.seconds;
        tVnow->SecondsHi   = timeStampPtr.secondsHi;
    }
    else
    {
        isValidIngressTime = FALSE;
    }
    return isValidIngressTime;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#else
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of getting the soft ingress timestamp if EthTSynHardwareTimestampSupport is FALSE.
 * @param[in]    port port pointer.
 * @param[in]    tVnow ingress time.
 * @retval       TRUE   ingress timestamp has been captured successfully.
 * @retval       FALSE  ingress timestamp has been captured failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-74327
 */
ETHTSYN_LOCAL boolean EthTSyn_GetSoftIngressTime(const EthTSyn_PortType* port, EthTSyn_TimeStampType* tVnow)
{
    boolean                   isValidIngressTime = FALSE;
    StbM_VirtualLocalTimeType VirtualLocalTime;
    if (StbM_GetCurrentVirtualLocalTime(PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef, &VirtualLocalTime)
        == E_OK)
    {
        *tVnow             = EthTSyn_VTToGT(VirtualLocalTime);
        isValidIngressTime = TRUE;
    }
    return isValidIngressTime;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of getting the T2vlt.
 * @param[in]    port port pointer.
 * @param[in]    t1vlt Ingress timestamp of SYNC message as captured by HW in the Ethernet controller or by SW in
 * EthTSyn_RxIndication.
 * @param[out]   t2vlt Virtual Local Time component of the Rx Time Tuple (equivalent to TV_Rxin the StbM)
 * @retval       TRUE   ingress timestamp has been captured successfully.
 * @retval       FALSE  ingress timestamp has been captured failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85837
 */
ETHTSYN_LOCAL boolean
    EthTSyn_GetSyncT2vlt(const EthTSyn_PortType* port, EthTSyn_TimeStampType t1vlt, EthTSyn_TimeStampType* t2vlt)
/* PRQA S 6070 -- */
{
    boolean isValidT2vlt = TRUE;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_ON)
    /* SWS_EthTSyn_00190 */
    if (!EthTSyn_StbMEthHWUsed(port->DomainIndex))
    {
        EthTSyn_TimeStampType     t3vlt;
        EthTSyn_TimeStampType     t4vlt;
        StbM_VirtualLocalTimeType virtualLocalTime;
        TimeTupleType             timeTuple;
        timeTuple.timeQuality = TSQ_INVALID;
        SchM_Enter_EthTSyn_Context();
        Std_ReturnType ret =
            EthIf_GetCurrentTimeTuple(port->CtrlId, PORT_TOCFGDOMIN(port->DomainIndex).ClkUnitRef, &timeTuple);
        if ((ret == E_OK) && (timeTuple.timeQuality == TSQ_VALID))
        {
            if (StbM_GetCurrentVirtualLocalTime(
                    PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef,
                    &virtualLocalTime)
                == E_OK)
            {
                t3vlt.Nanoseconds = timeTuple.timestampClockValue.nanoseconds;
                t3vlt.Seconds     = timeTuple.timestampClockValue.seconds;
                t3vlt.SecondsHi   = timeTuple.timestampClockValue.secondsHi;
                t4vlt             = EthTSyn_VTToGT(virtualLocalTime);
            }
            else
            {
                isValidT2vlt = FALSE;
            }
        }
        else
        {
            isValidT2vlt = FALSE;
        }
        SchM_Exit_EthTSyn_Context();
        if (isValidT2vlt)
        {
            EthTSyn_TimeStampType tSrd;
            /* SWS_EthTSyn_00190    tsrd = t3vlt-t1vlt*/
            EthTSyn_TimeStampSub(t3vlt, t1vlt, &tSrd);
            if ((sint32)EthTSyn_TimeStampCmp(t4vlt, tSrd) >= 0)
            {
                /* SWS_EthTSyn_00190    t2vlt = t4vlt-tsrd*/
                EthTSyn_TimeStampSub(t4vlt, tSrd, t2vlt);
            }
            /* t4vlt < tSrd,The probability of this scenario occurring is very low, as it can only happen when a
            Sync message is received immediately after power-on or following STBM module initialization. If such
            a situation occurs, the current packet should be discarded, which will not affect subsequent synchronization
            processes */
            else
            {
                isValidT2vlt = FALSE;
            }
        }
    }
    else
#endif
    {
        /* SWS_EthTSyn_00138 SWS_EthTSyn_00180 */
        *t2vlt = t1vlt;
        ETHTSYN_UNUSED(port);
    }

    return isValidT2vlt;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of updating the HysteresisCount.
 * @param[in]    port port pointer.
 * @param[in]    sequenceId sequence id of SYNC message
 * @param[in]    sequenceCounterJump jump of sequenceId of SYNC message
 * @param[in]    sequenceCounterJumpWidth configuration of GlobalTimeSequenceCounterJumpWidth
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace
 */
ETHTSYN_LOCAL void EthTSyn_UpdateHysteresisInTimeout(
    EthTSyn_PortType* port,
    const uint16      sequenceId,
    const uint16      sequenceCounterJump,
    const uint16      sequenceCounterJumpWidth)
/* PRQA S 6070 -- */
{
    if (sequenceId == port->ReceivedSyncSequenceId)
    {
        port->SCHysteresisCount = 0u;
    }
    else
    {
        if (ETHTSYN_MAX_HYSTERESIS_COUNTER == port->SCHysteresisCount)
        {
            port->SCHysteresisCount = 1u;
        }
        else
        {
            if ((sequenceId > port->ReceivedSyncSequenceId) && (sequenceCounterJump <= sequenceCounterJumpWidth))
            {
                port->SCHysteresisCount++;
            }
            else
            {
                port->SCHysteresisCount = 0u;
            }
        }
    }
    if (port->SCHysteresisCount > 0u)
    {
        port->ReceivedSyncSequenceId = sequenceId;
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of handling the received Sync message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73918
 */
ETHTSYN_LOCAL
void EthTSyn_RxHandleSyncMessage(EthTSyn_PortType* port, const uint8* dataPtr, EthTSyn_TimeStampType tVnow)
/* PRQA S 6070 -- */
{
    /* if only one port in domain, there will be no DYNAMIC port. Time GateWay would be required */
    if (EthTSyn_ExpectedStatusGet(port->DomainIndex, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY)
        && (port->PortRoleType == ETHTSYN_DYNAMIC))
    {
        EthTSyn_DynamicPortRole(port);
    }

    if (port->PortRoleType == ETHTSYN_SLAVE)
    {
        const uint8* masterPortIdentity = &dataPtr[ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET];
#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
        if (!EthTSyn_IsPortIdentityEqual((const uint8*)&port->PortIdentity, masterPortIdentity)
            && !port->FirstlyReceivedSyncReq)
        {
            (void)Det_ReportRuntimeError(
                ETHTSYN_MODULE_ID,
                ETHTSYN_INSTANCE_ID,
                ETHTSYN_SID_RXINDICATION,
                ETHTSYN_E_TSCONFLICT);
        }
        else
#endif
        {
            const EthTSyn_GlobalTimeDomainCfgType* domainCfgPtr = &PORT_TOCFGDOMIN(port->DomainIndex);
            StbM_TimeBaseStatusType                timeBaseStatus;
            (void)StbM_GetTimeBaseStatus(domainCfgPtr->SynchronizedTimeBaseRef, &timeBaseStatus);

            const uint16 sequenceCounterJumpWidth =
                domainCfgPtr->PortRole.GlobalTimeSlave->GlobalTimeSequenceCounterJumpWidth;
            uint16 sequenceId =
                (uint16)(((uint16)((uint16)(dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH]) << ETHTSYN_BIT_SHIFT_8))
                         + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
            uint16  sequenceCounterJump = sequenceId - port->ReceivedSyncSequenceId;
            boolean isValidSequenceInNormalState =
                ((sequenceId > port->ReceivedSyncSequenceId) && (sequenceCounterJump <= sequenceCounterJumpWidth)
                 && ((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) == 0u));
            boolean isValidSequenceInTimeoutState = FALSE;
            if (((timeBaseStatus & STBM_TIMEBASE_STATUS_TIMEOUT) != 0u) && (sequenceCounterJumpWidth > 0u))
            {
                EthTSyn_UpdateHysteresisInTimeout(port, sequenceId, sequenceCounterJump, sequenceCounterJumpWidth);
                if (port->SCHysteresisCount
                    > domainCfgPtr->PortRole.GlobalTimeSlave->GlobalTimeSequenceCounterHysteresis)
                {
                    isValidSequenceInTimeoutState = TRUE;
                }
            }

            if ((sequenceCounterJumpWidth == 0u) || port->FirstlyReceivedSyncReq || isValidSequenceInNormalState
                || isValidSequenceInTimeoutState)
            {
                EthTSyn_TimeStampType t2vlt;
                if (EthTSyn_GetSyncT2vlt(port, tVnow, &t2vlt))
                {
                    const EthTSyn_GlobalTimeSlaveCfgType* slaveCfgPtr = domainCfgPtr->PortRole.GlobalTimeSlave;

                    SchM_Enter_EthTSyn_Context();
                    port->RxDebounceTimeTicks = slaveCfgPtr->RxDebounceTimeTicks;
                    port->FollowUpTimeoutTicks =
                        (slaveCfgPtr->FollowUpTimeoutTicks > 0u) ? slaveCfgPtr->FollowUpTimeoutTicks : 1u;
                    SchM_Exit_EthTSyn_Context();
                    port->FirstlyReceivedSyncReq = FALSE;
                    /* tVnow represents the local time after transmission of sync*/
                    port->T1vlt = t2vlt;
                    (void)IStdLib_MemCpy(&port->PortIdentity, masterPortIdentity, ETHTSYN_PORT_IDENTITY_LENGTH);
                    port->ReceivedSyncSequenceId = sequenceId;
                    port->SCHysteresisCount      = ETHTSYN_MAX_HYSTERESIS_COUNTER;
                }
            }
        }
    }
#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
    else
    {
        (void)Det_ReportRuntimeError(
            ETHTSYN_MODULE_ID,
            ETHTSYN_INSTANCE_ID,
            ETHTSYN_SID_RXINDICATION,
            ETHTSYN_E_TMCONFLICT);
    }
#endif
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 6080 ++ */ /* VL_MTR_EthTSyn_STPTH */
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of handling the received Follow Up message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @param[in]    lenByte the length of received data.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85838
 */
ETHTSYN_LOCAL void EthTSyn_RxHandleFollowUpMessage(EthTSyn_PortType* port, const uint8* dataPtr, uint16 lenByte)
/* PRQA S 6070 -- */
/* PRQA S 6080 -- */
{
    const EthTSyn_GlobalTimeSlaveCfgType* slaveCfgPtr = PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeSlave;
    const uint8*                          masterPortIdentity = &dataPtr[ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET];
    uint16                                sequenceId =
        (uint16)(((uint16)((uint16)(dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH]) << ETHTSYN_BIT_SHIFT_8))
                 + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
    if (EthTSyn_IsPortIdentityEqual((const uint8*)&port->PortIdentity, masterPortIdentity)
        && (sequenceId == port->ReceivedSyncSequenceId) && (slaveCfgPtr != NULL_PTR))
    {
        boolean isSyncToGatewayBitSet = FALSE;
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
        Std_ReturnType ret = 0u;
#endif
        StbM_ProtocolParamType* protocolParamRuntime = NULL_PTR;
        uint32                  temp_u32             = 0u;
        EthTSyn_TimeStampType   tg = EthTSyn_DataToTime(&(dataPtr[ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX]));
        if (tg.Nanoseconds > ETHTSYN_NANOSECOND_TO_SECOND)
        {
            return;
        }

        SchM_Enter_EthTSyn_Context();
        port->RxDebounceTimeTicks  = slaveCfgPtr->RxDebounceTimeTicks;
        port->FollowUpTimeoutTicks = 0u;
        SchM_Exit_EthTSyn_Context();

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
        ret = EthTSyn_TlvScanAndCRCValidation(port->DomainIndex, &dataPtr[0u], lenByte, &isSyncToGatewayBitSet);
        if (ret != E_OK)

        {
            return;
        }
#else
        ETHTSYN_UNUSED(lenByte);
#endif
        /* cumulativeScaledRateOffset */
        protocolParamRuntime = EthTSyn_ConfigData->GlobalTimeDomain[port->DomainIndex].ProtocolParam;
        temp_u32 =
            (((uint32)dataPtr[ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE0] << ETHTSYN_BIT_SHIFT_24)
             | ((uint32)dataPtr[ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE1] << ETHTSYN_BIT_SHIFT_16)
             | ((uint32)dataPtr[ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE2] << ETHTSYN_BIT_SHIFT_8)
             | (uint32)dataPtr[ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE3]);
        protocolParamRuntime->cumulativeScaledRateOffset = (sint32)temp_u32;
        EthTSyn_ValidFollowUpMessageSetGlobalTime(port, &dataPtr[0u], &isSyncToGatewayBitSet);
    }
    else
    {
        /* Intentionally Empty */
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of handling the received Pdelay Req message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-83286
 */
ETHTSYN_LOCAL void
    EthTSyn_RxHandlePdelayReqMessage(EthTSyn_PortType* port, const uint8* dataPtr, EthTSyn_TimeStampType tVnow)
{
    port->PdelayResponderTimestamp.PdelayReqEventIngressTimestamp = tVnow;
    port->PdelayRXStatusType                                      = ETHTSYN_SEND_PDELAY_RESP;
    uint16 sequenceId = (uint16)(((uint16)((uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] << ETHTSYN_BIT_SHIFT_8))
                                 + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
    port->ReceivedPdelayReqSequenceId = sequenceId;
    (void)IStdLib_MemCpy(
        &EthTSyn_ReceivedPdelayReqSourcePortId[port->DomainIndex][0],
        &dataPtr[ETHTSYN_PDELAY_REQ_SOURCE_PORTID_BYTE_OFFSET],
        ETHTSYN_PDELAY_REQ_SOURCE_PORTID_BYTE_LENGTH);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of handling the received Pdelay Response message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-73920
 */
ETHTSYN_LOCAL void
    EthTSyn_RxHandlePdelayResponseMessage(EthTSyn_PortType* port, const uint8* dataPtr, EthTSyn_TimeStampType tVnow)
{
    const EthTSyn_PortConfigCfgType* portCfg = &PORT_TOCFGPORT(port->DomainIndex);
    if (port->PdelayTXStatusType != ETHTSYN_WAITFOR_PDELAY_RESP)
    {
        return;
    }

    /* CODE_EthTSyn_00001 */
    uint16 sequenceId =
        (uint16)(((uint16)((uint16)(dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH]) << ETHTSYN_BIT_SHIFT_8))
                 + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
    const uint8* requestingPortIdentity = &dataPtr[ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET];
    if ((sequenceId != port->PdelayReqSequenceId)
        || !EthTSyn_IsPortIdentityEqual(requestingPortIdentity, (const uint8*)&port->ThisPortIdentity))
    {
        port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
        return;
    }

    uint64                timeout = portCfg->PdelayConfig->PdelayRespAndRespFollowUpTimeout;
    EthTSyn_TimeStampType td;
    EthTSyn_TimeStampSub(tVnow, port->T1, &td);
    if ((timeout != 0ULL) && !EthTSyn_timeless(td, timeout))
    {
        port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
        return;
    }

    port->T2                 = EthTSyn_DataToTime(&(dataPtr[ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX]));
    port->T4                 = tVnow;
    port->PdelayTXStatusType = ETHTSYN_WAITFOR_PDELAY_RESP_FOLLOWUP;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of handling the received Pdelay Response Follow Up message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    dataPtr pointer of received data.
 * @param[in]    tVnow ingress time.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85839
 */
ETHTSYN_LOCAL void
    EthTSyn_RxHandlePdelayResponseFUMessage(EthTSyn_PortType* port, const uint8* dataPtr, EthTSyn_TimeStampType tVnow)
{
    const EthTSyn_PortConfigCfgType* portCfg = &PORT_TOCFGPORT(port->DomainIndex);
    if (port->PdelayTXStatusType != ETHTSYN_WAITFOR_PDELAY_RESP_FOLLOWUP)
    {
        return;
    }
    port->PdelayTXStatusType = ETHTSYN_SEND_PDELAY_REQ;
    uint16 sequenceId =
        (uint16)(((uint16)((uint16)(dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH]) << ETHTSYN_BIT_SHIFT_8))
                 + (uint16)dataPtr[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]);
    /* CODE_EthTSyn_00002 */
    const uint8* requestingPortIdentity = &dataPtr[ETHTSYN_PDELAY_RESP_REQUESTING_PORT_IDENTITY_OFFSET];
    if ((sequenceId != port->PdelayReqSequenceId)
        || !EthTSyn_IsPortIdentityEqual(requestingPortIdentity, (const uint8*)&port->ThisPortIdentity))
    {
        return;
    }

    uint64                timeout = portCfg->PdelayConfig->PdelayRespAndRespFollowUpTimeout;
    EthTSyn_TimeStampType td;
    EthTSyn_TimeStampSub(tVnow, port->T4, &td);
    if ((timeout != 0ULL) && !EthTSyn_timeless(td, timeout))
    {
        return;
    }
    port->T3 = EthTSyn_DataToTime(&(dataPtr[ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX]));
    /* pdelay = ((t4-t1)-(t3-t2))/2 */
    uint64 t3 = EthTSyn_TimestampToLocalTime(&port->T3);
    uint64 t4 = EthTSyn_TimestampToLocalTime(&port->T4);

    uint64 t1   = EthTSyn_TimestampToLocalTime(&port->T1);
    uint64 td41 = (t4 > t1) ? (t4 - t1) : 0LLu;

    uint64 t2   = EthTSyn_TimestampToLocalTime(&port->T2);
    uint64 td32 = (t3 > t2) ? (t3 - t2) : 0LLu;

    uint64 pDelay = 0LLu;
    if ((td41 > 0LLu) && (td32 > 0LLu))
    {
        float64 neighborRateratio        = port->RateRatio.NeighborRateRatio;
        float64 rateRatioPdelayResponder = port->RateRatio.RateRatioPdelayResponder;
        /* PRQA S 3121 ++ */ /* VL_QAC_MagicNum */
        float64 pDelay_tem = rateRatioPdelayResponder * (neighborRateratio * ((float64)td41 - (float64)td32) / 2.0);
        /* PRQA S 3121 -- */
        pDelay                        = (uint64)pDelay_tem;
        uint64 pdelayLatencyThreshold = portCfg->PdelayConfig->PdelayLatencyThreshold;
        if ((pdelayLatencyThreshold == 0u) || (pDelay < pdelayLatencyThreshold))
        {
            uint64 pdelaySeconds     = pDelay / ETHTSYN_NANOSECOND_TO_SECOND;
            port->Pdelay.Seconds     = (uint32)pdelaySeconds;
            port->Pdelay.SecondsHi   = (uint16)(pdelaySeconds >> ETHTSYN_BIT_SHIFT_32);
            port->Pdelay.Nanoseconds = (uint32)(pDelay % ETHTSYN_NANOSECOND_TO_SECOND);
        }
#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
        /* SWS_EthTSyn_00263 */
        port->RateRatio.RateRatioMesuredCounter =
            (port->RateRatio.RateRatioMesuredCounter % portCfg->PdelayConfig->RateRatioMeasurementCount) + 1u;
#endif
    }
#if (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
    /* SWS_EthTSyn_00221 SWS_EthTSyn_00222 */
    if ((PORT_TOCFGPORT(port->DomainIndex).EnableTimeValidation) && (port->PortRoleType == ETHTSYN_SLAVE))
    {
        StbM_PdelayInitiatorMeasurementType validationMeasureDataPtr;
        (void)IStdLib_MemSet(&validationMeasureDataPtr, 0u, sizeof(StbM_PdelayInitiatorMeasurementType));
        validationMeasureDataPtr.sequenceId = port->PdelayReqSequenceId;

        validationMeasureDataPtr.requestPortId.clockIdentity = port->ThisPortIdentity.ClockIdentity;
        validationMeasureDataPtr.requestPortId.portNumber    = port->ThisPortIdentity.PortNumber;

        validationMeasureDataPtr.responsePortId.clockIdentity = port->ThisPortIdentity.ClockIdentity;
        validationMeasureDataPtr.responsePortId.portNumber    = port->ThisPortIdentity.PortNumber;
        /* RequestOriginTimestamp */
        validationMeasureDataPtr.requestOriginTimestamp = EthTSyn_GTToVT(port->T1);
        /* ResponseReceiptTimestamp */
        validationMeasureDataPtr.responseReceiptTimestamp = EthTSyn_GTToVT(port->T4);
        /* requestReceiptTimestamp */
        validationMeasureDataPtr.requestReceiptTimestamp.seconds      = port->T2.Seconds;
        validationMeasureDataPtr.requestReceiptTimestamp.nanoseconds  = port->T2.Nanoseconds;
        validationMeasureDataPtr.responseOriginTimestamp.seconds      = port->T3.Seconds;
        validationMeasureDataPtr.responseOriginTimestamp.nanoseconds  = port->T3.Nanoseconds;
        validationMeasureDataPtr.referenceLocalTimestamp              = port->TrefPDInitiator.virtualLocalTime;
        validationMeasureDataPtr.referenceGlobalTimestamp.seconds     = port->TrefPDInitiator.globalTime.seconds;
        validationMeasureDataPtr.referenceGlobalTimestamp.nanoseconds = port->TrefPDInitiator.globalTime.nanoseconds;
        validationMeasureDataPtr.pdelay                               = (uint32)pDelay;
        StbM_EthSetPdelayInitiatorData(
            PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef,
            &validationMeasureDataPtr);
    }
#endif
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 6070 ++ */ /* VL_MTR_EthTSyn_STCAL */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of caculating the original timestamp to be sended in the Follow Up message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    isValidEgressTime  specifying if ingress timestamp has been captured successfully.
 * @param[in]    egressTimeStamp the ingress timestamp of SYNC message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85840
 */
ETHTSYN_LOCAL void EthTSyn_OriginalTimeStampCaculation(
    EthTSyn_PortType*     port,
    boolean               isValidEgressTime,
    EthTSyn_TimeStampType egressTimeStamp)
/* PRQA S 6070 -- */
{
    if (!isValidEgressTime)
    {
        port->SyncStatusType = ETHTSYN_SEND_SYNC;
        return;
    }
    EthTSyn_TimeStampType t0    = port->SyncEgressTuple.T0;
    EthTSyn_TimeStampType t0vlt = port->SyncEgressTuple.T0vlt;
#if (ETHTSYN_HARDWARE_TIMESTAMP_SUPPORT == STD_OFF)
    /* SWS_EthTSyn_00189 */
    EthTSyn_TimeStampType t4vlt = egressTimeStamp;
    EthTSyn_TimeStampType td;
    EthTSyn_TimeStampSub(t4vlt, t0vlt, &td); /* preciseOriginTimestamp = T0 + t4vlt - t0vlt */
    port->PreciseOriginTimestamp = EthTSyn_timeadd(td, t0);

#else
    if (EthTSyn_StbMEthHWUsed(port->DomainIndex))
    {
        /* SWS_EthTSyn_00188 */
        EthTSyn_TimeStampType td;
        EthTSyn_TimeStampSub(egressTimeStamp, t0vlt, &td);
        port->PreciseOriginTimestamp = EthTSyn_timeadd(td, t0); /* preciseOriginTimestamp = T0 + t2vlt - t0vlt */
    }
    else
    {
        /* SWS_EthTSyn_00017 */
        EthTSyn_TimeStampType t3vlt = {0u, 0u, 0u};
        EthTSyn_TimeStampType t4vlt;
        TimeTupleType         timeTuple;
        timeTuple.timeQuality = TSQ_INVALID;
        SchM_Enter_EthTSyn_Context();
        Std_ReturnType ret =
            EthIf_GetCurrentTimeTuple(port->CtrlId, PORT_TOCFGDOMIN(port->DomainIndex).ClkUnitRef, &timeTuple);
        if (!((ret == E_OK) && (timeTuple.timeQuality == TSQ_VALID)))
        {
            SchM_Exit_EthTSyn_Context();
            port->SyncStatusType = ETHTSYN_SEND_SYNC;
            return;
        }

        t3vlt.Nanoseconds = timeTuple.timestampClockValue.nanoseconds;
        t3vlt.Seconds     = timeTuple.timestampClockValue.seconds;
        t3vlt.SecondsHi   = timeTuple.timestampClockValue.secondsHi;
        StbM_VirtualLocalTimeType VirtualLocalTime;
        if (StbM_GetCurrentVirtualLocalTime(
                PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef,
                &VirtualLocalTime)
            != E_OK)
        {
            SchM_Exit_EthTSyn_Context();
            port->SyncStatusType = ETHTSYN_SEND_SYNC;
            return;
        }
        SchM_Exit_EthTSyn_Context();
        t4vlt = EthTSyn_VTToGT(VirtualLocalTime);
        EthTSyn_TimeStampType td32vlt;
        EthTSyn_TimeStampSub(t3vlt, egressTimeStamp, &td32vlt); /* td32vlt = t3vlt - t2vlt */
        EthTSyn_TimeStampType td40vlt;
        EthTSyn_TimeStampSub(t4vlt, t0vlt, &td40vlt); /* td40vlt = t4vlt - t0vlt */
        if ((sint32)EthTSyn_TimeStampCmp(td40vlt, td32vlt) < 0)
        {
            port->SyncStatusType = ETHTSYN_SEND_SYNC;
            return;
        }

        EthTSyn_TimeStampType tdTemp;
        EthTSyn_TimeStampSub(td40vlt, td32vlt, &tdTemp);
        /** [SWS_EthTSyn_00017]
         *  preciseOriginTimestamp = T0 - (T3VLT - T2VLT) + (T4VLT - T0VLT)
         */
        port->PreciseOriginTimestamp = EthTSyn_timeadd(tdTemp, t0);
    }
#endif
    port->SyncStatusType = ETHTSYN_SEND_FOLLOWUP;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of packing the Sync message to be sended.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    data the databuffer to be sended.
 * @param[in]    hasCapturedEgress  specifying if T0 and t0vlt have been captured successfully.
 * @return       the length of Sync message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85841
 */
ETHTSYN_LOCAL uint16 EthTSyn_SyncMessagePack(EthTSyn_PortType* port, uint8* data, boolean* hasCapturedEgress)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    /* SWS_EthTSyn_00202 */
    StbM_TimeStampType        globalTime;
    StbM_VirtualLocalTimeType localTime;
    StbM_UserDataType         userData;
    uint16                    dataLenth = 0U;
    EthTSyn_TimeStampType     t0        = {0u, 0u, 0u};
    StbM_TimeTupleType        timeTuple;
    Std_ReturnType            ret =
        StbM_GetCurrentTime(PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef, &timeTuple, &userData);
    globalTime = timeTuple.globalTime;
    localTime  = timeTuple.virtualLocalTime;
    if (ret == E_OK)
    {
        t0.Nanoseconds              = globalTime.nanoseconds;
        t0.Seconds                  = globalTime.seconds;
        t0.SecondsHi                = globalTime.secondsHi;
        EthTSyn_TimeStampType t0vlt = {0u, 0u, 0u};
        t0vlt                       = EthTSyn_VTToGT(localTime);
        port->SyncEgressTuple.T0vlt = t0vlt;
        port->SyncEgressTuple.T0    = t0;
        port->UserData              = userData;
        /*messageLength */
        data[2] = 0u;
        data[3] = 44U;
        /*flags Octet 0*/
        data[6] = 0x02u;
        data[7] = 0x08u;
        /*sequenceId*/
        if (port->SyncSequenceId < ETHTSYN_MAX_SEQUENCEID)
        {
            port->SyncSequenceId++;
        }
        else
        {
            port->SyncSequenceId = 0u;
        }
        data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] = (uint8)(port->SyncSequenceId >> ETHTSYN_BIT_SHIFT_8);
        data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]  = (uint8)port->SyncSequenceId;
        /*control*/
        data[32] = 0x0u;
        /*logMessageInterval in Seconds*/
        data[33] = (uint8)(PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster->LogMessageInterval);
        /*reserved */
        for (uint8 i = 0u; i < 10u; i++)
        {
            data[34u + i] = 0x00u;
        }
        dataLenth = 44U;
    }
    else
    {
        *hasCapturedEgress = FALSE;
    }
    return dataLenth;
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief           Construct Time Secured sub-TLV for Follow_Up messages
 * @param[in]       port Pointer to port structure
 * @param[inout]    data Buffer for constructing the TLV
 * @param[in]       Lenth Current length of data buffer
 * @return          uint16 Updated length after adding Time Secured sub-TLV
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-86489
 */
ETHTSYN_LOCAL uint16 EthTSyn_TlvTimeSecured(const EthTSyn_PortType* port, uint8* data, uint16 Lenth)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint16 dataLenth = Lenth;
    if ((PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->TLVFollowUpTimeSubTLV
        && ((PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->CrcTimeFlagsTxSecured != NULL_PTR)
        && ((PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->GlobalTimeTxCrcSecured
            == ETHTSYN_CRC_SUPPORTED))
    {
        data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u] += 5U;
        /* Type */
        data[dataLenth] = ETHTSYN_SUB_TLV_TYPE_TIME_SECURED;
        /* Length */
        data[dataLenth + 1u] = ETHTSYN_FUP_SUBTLV_TIME_LENGTH;

        data[3] += 5u;

        /* CRC_Time_Flags */
        data[dataLenth + 2u] = (PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->CRCTimeFlags;
        uint8 crcTime0       = 0u;
        uint8 crcTime1       = 0u;
        EthTSyn_CalculateTimeSecuredCrc(port->DomainIndex, data, dataLenth, &crcTime0, &crcTime1);
        /* CRC_Time_0 */
        data[dataLenth + ETHTSYN_CRC_TIME0_OFFSET] = crcTime0;
        /* CRC_Time_1 */
        data[dataLenth + ETHTSYN_CRC_TIME1_OFFSET] = crcTime1;

        dataLenth += 5U;
    }
    return dataLenth;
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief           Fill extended TLV field for FollowUp message
 * @param[in]       port Pointer to EthTSyn port structure
 * @param[inout]    data Buffer for TLV data construction
 * @param[in]       Lenth Current buffer length
 * @return          uint16 Updated buffer length after extended TLV addition
 * @synchronous     TRUE
 * @reentrant       FALSE
 * @trace           CPD-86490
 */
ETHTSYN_LOCAL uint16 EthTSyn_FillFollowUpExtendedTlv(const EthTSyn_PortType* port, uint8* data, uint16 Lenth)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint16 dataLenth = Lenth;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
    uint8 crcdata[ETHTSYN_CRC_DATA_TO_BE_CACULATED_LENGTH];
#endif
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    if (!EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION))
#endif
    {
        /* Autosar Follow_Up information TLV */
        /*tlvType */
        data[76] = 0x00u;
        data[77] = 0x03u;
        /*lengthField*/
        data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET]      = 0x00u;
        data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u] = ETHTSYN_FUP_EXT_TLV_LENGTH_0;
        /*organizationId*/
        data[80] = ETHTSYN_FUP_EXT_TLV_ORGID_2;
        data[81] = ETHTSYN_FUP_EXT_TLV_ORGID_1;
        data[82] = ETHTSYN_FUP_EXT_TLV_ORGID_0;
        /*organizationSubType*/
        data[83] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2;
        data[84] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1;
        data[85] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0;
        data[3] += 10u;
        dataLenth += 10U;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        uint8 dataID =
            PORT_TOCFGDOMIN(port->DomainIndex)
                .GlobalTimeFollowUpDataIDList->GlobalTimeFollowUpDataIDListElement[(port->SyncSequenceId) % 16u]
                .GlobalTimeFollowUpDataIDListValue;
#endif
        /* Sub-TLV: Status */
        if ((PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->TLVFollowUpStatusSubTLV)
        {
            data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u] += 4u;
            /* Length */
            data[dataLenth + 1u] = 0x02u;
            /* Status */
            if (EthTSyn_ExpectedStatusGet(port->DomainIndex, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
            {
                data[dataLenth + 2u] = 0x01u;
            }
            else
            {
                data[dataLenth + 2u] = 0x00u;
            }
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
            if ((PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->GlobalTimeTxCrcSecured
                == ETHTSYN_CRC_SUPPORTED)
            {
                /* Type */
                data[dataLenth]      = 0x50u;
                crcdata[0u]          = data[dataLenth + 2u];
                crcdata[1u]          = dataID;
                data[dataLenth + 3u] = Crc_CalculateCRC8H2F(&crcdata[0u], 2u, 0xFFu, TRUE);
            }
            else
#endif
            {
                /* Type */
                data[dataLenth]      = 0x51u;
                data[dataLenth + 3u] = 0x00u;
            }
            data[3] += 4u;
            dataLenth += 4U;
        }
        /* Sub-TLV: UserData Secured */
        StbM_UserDataType userData = port->UserData;
        if ((PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->TLVFollowUpUserDataSubTLV
            && (userData.userDataLength > 0u))
        {
            data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u] += 7u;
            /* Length */
            data[dataLenth + 1u] = 0x05u;
            data[dataLenth + 2u] = userData.userDataLength;
            data[dataLenth + 3u] = userData.userByte0;
            data[dataLenth + 4u] = userData.userByte1;
            data[dataLenth + 5u] = userData.userByte2;
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
            if ((PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->GlobalTimeTxCrcSecured
                == ETHTSYN_CRC_SUPPORTED)
            {
                /* Type */
                data[dataLenth] = 0x60u;
                crcdata[4]      = dataID;
                (void)IStdLib_MemCpy(&crcdata[0u], &data[dataLenth + 2u], 4u);
                data[dataLenth + 6u] = Crc_CalculateCRC8H2F(&crcdata[0u], 5u, 0xFFu, TRUE);
            }
            else
#endif
            {
                /* Type */
                data[dataLenth]      = 0x61u;
                data[dataLenth + 6u] = 0x00u;
            }
            data[3] += 7u;
            dataLenth += 7U;
        }
#if (ETHTSYN_CRC_SUPPORT == STD_ON)
        /* Sub-TLV: Time Secured */
        dataLenth = EthTSyn_TlvTimeSecured(port, data, dataLenth);
#endif
    }
    return dataLenth;
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif

/* PRQA S 6020, 6050 ++ */ /* VL_MTR_EthTSyn_STLIN, VL_MTR_EthTSyn_STST3 */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of packing the Follow Up message to be sended.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    data the databuffer to be sended.
 * @return       the length of Follow Up message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85842
 */
ETHTSYN_LOCAL uint16 EthTSyn_FollowUpMessagePack(const EthTSyn_PortType* port, uint8* data)
/* PRQA S 6020, 6050 -- */
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint16 dataLenth = 0U;
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    if (!EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION))
#endif
    {
        /*messageLength */
        data[2] = 0u;
        data[3] = 76U;
        /*flags Octet 0*/
        data[6] = 0x00u;
        data[7] = 0x08u;
        /*sequenceId*/
        data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] = (uint8)(port->SyncSequenceId >> ETHTSYN_BIT_SHIFT_8);
        data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]  = (uint8)port->SyncSequenceId;
        /*control*/
        data[32] = 0x02u;
        /*logMessageInterval in Seconds*/
        data[33] = (uint8)(PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster->LogMessageInterval);
        /*preciseOriginTimestamp */
        EthTSyn_TimeToData(port->PreciseOriginTimestamp, &data[34u]);
        /*IEEE Follow_Up information TLV */
        /*tlvType */
        data[44] = 0x00u;
        data[45] = 0x03u;
        /*lengthField*/
        data[46] = 0x00u;
        data[47] = 28u;
        /*organizationId*/
        data[48] = 0x00u;
        data[49] = 0x80u;
        data[50] = 0xC2u;
        /*organizationSubType*/
        data[51] = 0x00u;
        data[52] = 0x00u;
        data[53] = 0x01u;
        /* cumulativeScaledRateOffset*/

        /* PRS_TS_00263 */
#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
        StbM_ProtocolParamType* protocolParamRuntime =
            EthTSyn_ConfigData->GlobalTimeDomain[port->DomainIndex].ProtocolParam;
        if (EthTSyn_ExpectedStatusGet(port->DomainIndex, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
        {
            StbM_ProtocolParamType protocolParam;
            (void)EthTSyn_GetProtocolParam(PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef, &protocolParam);
            protocolParamRuntime->cumulativeScaledRateOffset = protocolParam.cumulativeScaledRateOffset;
        }
#else
        const StbM_ProtocolParamType* protocolParamRuntime =
            EthTSyn_ConfigData->GlobalTimeDomain[port->DomainIndex].ProtocolParam;
#endif
        data[ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE0] =
            (uint8)((uint32)protocolParamRuntime->cumulativeScaledRateOffset >> ETHTSYN_BIT_SHIFT_24);
        data[ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE1] =
            (uint8)((uint32)protocolParamRuntime->cumulativeScaledRateOffset >> ETHTSYN_BIT_SHIFT_16);
        data[ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE2] =
            (uint8)((uint32)protocolParamRuntime->cumulativeScaledRateOffset >> ETHTSYN_BIT_SHIFT_8);
        data[ETHTSYN_CUMULATIVE_SCALED_RATE_OFFSET_BTE3] = (uint8)(protocolParamRuntime->cumulativeScaledRateOffset);
        /* gmTimeBaseIndicator*/
        /* lastGmPhaseChange */
        /* scaledLastGmFreqChange*/
        for (uint8 i = 0u; i < 18u; i++)
        {
            data[58u + i] = 0x00u;
        }
        dataLenth = 76U;
    }
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    dataLenth = EthTSyn_FillFollowUpExtendedTlv(port, data, dataLenth);
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    if ((PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster)->TLVFollowUpICVSubTLV)
    {
        dataLenth += EthTSyn_ICVTxMainfunction((EthTSyn_PortType*)port, data, dataLenth);
    }
#endif

    if ((data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u] == 6u)
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
        && (!EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION))
#endif
    )
    {
        for (uint8 i = 0u; i < 10u; i++)
        {
            data[76u + i] = 0x00u;
        }
        data[3] -= 10u;
        dataLenth -= 10u;
    }
#endif
    return dataLenth;
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of packing the Req message to be sended.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    data the databuffer to be sended.
 * @return       the length of Req message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85843
 */
ETHTSYN_LOCAL uint16 EthTSyn_ReqMessagePack(EthTSyn_PortType* port, uint8* data)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    uint16 dataLenth = 0U;
    /*messageLength */
    data[2] = 0u;
    data[3] = 54u;
    /*flags Octet 0*/
    data[6] = 0x00u;
    data[7] = 0x08u;
    /*sequenceId*/
    if (port->PdelayReqSequenceId < 0xFFFFu)
    {
        port->PdelayReqSequenceId++;
    }
    else
    {
        port->PdelayReqSequenceId = 0u;
    }
    data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_HIGH] = (uint8)(port->PdelayReqSequenceId >> ETHTSYN_BIT_SHIFT_8);
    data[ETHTSYN_SEQUENCEID_BYTE_OFFSET_LOW]  = (uint8)port->PdelayReqSequenceId;
    /*control*/
    data[32] = 0x5u;
    /*logMessageInterval in Seconds*/
    data[33] = (uint8)(uint8)(PORT_TOCFGPORT(port->DomainIndex).PdelayConfig->LogMessageInterval);
    /*reserved */
    for (uint8 i = 0u; i < 20u; i++)
    {
        data[34u + i] = 0x00u;
    }
    dataLenth = 54u;
    /* PRQA S 3120 -- */
    return dataLenth;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of packing the public information in a  gptp message.
 * @param[in]    port the runtime variable pointer of a controllor port.
 * @param[in]    data the databuffer to be sended.
 * @param[in]    ethTSynMessage the type of gptp message.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85844
 */
ETHTSYN_LOCAL void EthTSyn_PublicInformationPack(const EthTSyn_PortType* port, uint8* data, uint8 ethTSynMessage)
{
    uint64 CorrectionField = 0u;
    if (port->PortRoleType == ETHTSYN_MASTER)
    {
        CorrectionField = PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster->CorrectionField;
    }
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    /*transportSpecific and messageType */
    data[0] = (uint8)((uint8)(0x01u << ETHTSYN_BIT_SHIFT_4) | ((uint8)ethTSynMessage));
    /*versionPTP */
    data[1] = ETHTSYN_PTP_VERSION;
    /*domainNumber */
    data[ETHTSYN_TIMEDOMAINID_BYTE_OFFSET] = port->TimeDomainId;
    /*reserved */
    data[5] = 0x00u;
    /*CorrectionField*/
    EthTSyn_CreatCorrectionField(CorrectionField, &data[8u]);
    /*reserved */
    data[16] = 0x00u;
    data[17] = 0x00u;
    data[18] = 0x00u;
    data[19] = 0x00u;

    /* sourcePortIdentity */
    (void)IStdLib_MemCpy(
        &data[ETHTSYN_HEADER_SOURCE_PORT_IDENTITY_OFFSET],
        &port->ThisPortIdentity,
        ETHTSYN_PORT_IDENTITY_LENGTH);

    /*reserved */
    for (uint8 i = 0u; i < 8u; i++)
    {
        data[46u + i] = 0x00u;
    }
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/* PRQA S 1503, 1532 -- */
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        find the configured port related to the timeBaseId.
 * @param[in]    timeBaseId   Identification of the controllor.
 * @return       the runtime variable(EthTSyn_Port),which is related to the timeBaseId.
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-85845
 */
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindPortAcrossTimeBase(uint8 timeBaseId)
{
    EthTSyn_PortType* port      = NULL_PTR;
    uint8             portIndex = 0u;
    for (portIndex = 0u; portIndex < EthTSyn_ConfigData->DomainNum; ++portIndex)
    {
        if (EthTSyn_ConfigData->GlobalTimeDomain[portIndex].SynchronizedTimeBaseRef == timeBaseId)
        {
            port = EthTSyn_ConfigData->RuntimeVariables[portIndex];
            break;
        }
    }
    return port;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        find the configured Master port related to the CSM jobId.
 * @param[in]    jobId Holds the identifier of the job using the CSM service.
 * @return       the runtime variable(EthTSyn_Port),which is related to the ctrlIdx.
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-85846
 */
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindTxPortAcrossJobId(uint32 jobId)
{
    EthTSyn_PortType* port      = NULL_PTR;
    uint8             portIndex = 0u;
    for (portIndex = 0u; portIndex < EthTSyn_ConfigData->DomainNum; ++portIndex)
    {
        if (PORT_TOCFGPORT(portIndex).GlobalTimeTxIcvGeneration == NULL_PTR)
        {
            continue;
        }
        if (PORT_TOCFGPORT(portIndex).GlobalTimeTxIcvGeneration->TxICVGenerationConfig.IcvGenerationJobRef == jobId)
        {
            port = EthTSyn_ConfigData->RuntimeVariables[portIndex];
            break;
        }
    }
    return port;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        find the configured Slave port related to the CSM jobId.
 * @param[in]    jobId Holds the identifier of the job using the CSM service.
 * @return       the runtime variable(EthTSyn_Port),which is related to the ctrlIdx.
 * @reentrant    FALSE
 * @synchronous  TRUE
 * @trace        CPD-85847
 */
ETHTSYN_LOCAL EthTSyn_PortType* EthTSyn_FindRxPortAcrossJobId(uint32 jobId)
{
    EthTSyn_PortType* port      = NULL_PTR;
    uint8             portIndex = 0u;
    for (portIndex = 0u; portIndex < EthTSyn_ConfigData->DomainNum; ++portIndex)
    {
        if (PORT_TOCFGPORT(portIndex).GlobalTimeRxIcvVerification == NULL_PTR)
        {
            continue;
        }
        if (PORT_TOCFGPORT(portIndex).GlobalTimeRxIcvVerification->RxICVVerificationConfig.IcvVerificationJobRef
            == jobId)
        {
            port = EthTSyn_ConfigData->RuntimeVariables[portIndex];
            break;
        }
    }
    return port;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
/**
 * @brief       Converts the number of bits to the corresponding number of bytes.
 * @param[in]   nbits   : The number of bits to be converted.
 * @retval      uint16 : The number of bytes equivalent to the given number of bits.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-83030
 */
ETHTSYN_LOCAL_INLINE uint16 EthTSyn_BitToBYTE(uint16 nbits)
{
    return (((nbits) + ETHTSYN_CONST_7U)) >> (ETHTSYN_CONST_3U);
}
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        Obtaining the freshness value of TX.
 * @param[out]   freshnessValue          Freshness value.
 * @param[inout] txIcvGeneration         ICV generation configuration
 * @param[inout] truncatedFreshnessValue Pointer to store the truncated freshness value.
 * @retval       E_OK: obtaining the freshness value of TX successfully.
 * @retval       E_NOT_OK: obtaining the freshness value of TX failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-85848
 */
ETHTSYN_LOCAL Std_ReturnType EthTSyn_TxFreshnessObtain(
    const EthTSyn_GlobalTimeTxIcvGenerationType* txIcvGeneration,
    uint8*                                       freshnessValue,
    uint8*                                       truncatedFreshnessValue)
{
    uint32         freshnessValueTruncLength = txIcvGeneration->IcvTxFreshnessValueTruncLength;
    uint32         freshnessValueLength      = txIcvGeneration->IcvTxFreshnessValueLength;
    Std_ReturnType stdReturn;
    /* SWS_EthTSyn_00246 */
    if (freshnessValueTruncLength == freshnessValueLength)
    {
        stdReturn = StbM_GetTxFreshness(txIcvGeneration->IcvGenerationFvIdRef, freshnessValue, &freshnessValueLength);
    }
    /* SWS_EthTSyn_00247 */
    else /* freshnessValueTruncLength < freshnessValueLength */
    {
        stdReturn = StbM_GetTxFreshnessTruncData(
            txIcvGeneration->IcvGenerationFvIdRef,
            freshnessValue,
            &freshnessValueLength,
            truncatedFreshnessValue,
            &freshnessValueTruncLength);
    }

    if (stdReturn == E_OK)
    {
        if ((freshnessValueTruncLength == txIcvGeneration->IcvTxFreshnessValueTruncLength)
            && (freshnessValueLength == txIcvGeneration->IcvTxFreshnessValueLength))
        {
            if (freshnessValueTruncLength == freshnessValueLength)
            {
                (void)IStdLib_MemCpy(
                    truncatedFreshnessValue,
                    freshnessValue,
                    EthTSyn_BitToBYTE(freshnessValueTruncLength));
            }
        }
        else
        {
            stdReturn = E_NOT_OK;
        }
    }
    return stdReturn;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of ICV generation at Master port.
 * @param[inout] port : port pointer.
 * @param[in]    generationBuffer : Contains the pointer to the data for which the MAC shall be computed.
 * @param[in]    dataLength : the length of generationBuffer.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-83032
 */
ETHTSYN_LOCAL void EthTSyn_ICVGeneration(EthTSyn_PortType* port, const uint8* generationBuffer, uint16 dataLength)
{
    Std_ReturnType                               ret = E_NOT_OK;
    const EthTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        EthTSyn_ConfigData->GlobalTimeDomain->PortConfig[port->DomainIndex].GlobalTimeTxIcvGeneration;
    EthTSyn_ICVGenerationAndVerificationType* icvGenarationAndVerification = &port->ICVGenarationAndVerification;
    uint32                                    icvLength                    = icvGeneration->IcvTxLength;
    switch (icvGeneration->TxICVGenerationConfig.IcvGenerationBase)
    {
    case ETHTSYN_ICV_MAC:
        ret = Csm_MacGenerate(
            icvGeneration->TxICVGenerationConfig.IcvGenerationJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            &generationBuffer[0u],
            dataLength,
            &icvGenarationAndVerification->AuthenticatedFieldInfo.ICVBuffer[0u],
            &icvLength);
        break;
    case ETHTSYN_ICV_SIGNATURE:
        ret = Csm_SignatureGenerate(
            icvGeneration->TxICVGenerationConfig.IcvGenerationJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            &generationBuffer[0u],
            dataLength,
            &icvGenarationAndVerification->AuthenticatedFieldInfo.ICVBuffer[0u],
            &icvLength);
        break;
    default:
        ret = E_NOT_OK;
        break;
    }

    if (ret == E_OK)
    {
        if (icvGeneration->TxICVGenerationConfig.IcvProcessingType == ETHTSYN_CRYPTO_PROCESSING_SYNC)
        {
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION);
            icvGenarationAndVerification->AuthenticationBuildAttemptsCounter = 0u;
        }
        else if (icvGeneration->IcvGenerationTimeout != 0u)
        {
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION);
            StbM_VirtualLocalTimeType VirtualLocalTime;
            if (StbM_GetCurrentVirtualLocalTime(
                    PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef,
                    &VirtualLocalTime)
                == E_OK)
            {
                icvGenarationAndVerification->IcvIndicationStartTime =
                    (uint64)((uint64)VirtualLocalTime.nanosecondsHi << ETHTSYN_BIT_SHIFT_32
                             | (uint64)VirtualLocalTime.nanosecondsLo);
            }
        }
        else
        {
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION);
        }
    }
    else if (ret == E_NOT_OK)
    {
        /* SWS_EthTSyn_00256 */
        EthTSyn_ICVGenerationBuildFailBehavior(port, ETHTSYN_SEV_TSYN_ETH_ICV_GENERATION_FAILED);
    }
    else if (ret == CRYPTO_E_BUSY)
    {
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_RETRY);
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the mainfunction of ICV generation at master port.
 * @param[inout] port : port pointer.
 * @param[in]    data : the pointer of data to be sended.
 * @param[in]    dataLenth : the length of data to be sended.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85849
 */
ETHTSYN_LOCAL uint16 EthTSyn_ICVTxMainfunction(EthTSyn_PortType* port, uint8* data, uint16 dataLenth)
{
    const EthTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        (PORT_TOCFGPORT(port->DomainIndex).GlobalTimeTxIcvGeneration);

    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_BUILDING)
        || EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_RETRY))
    {
        EthTSyn_StartICVGeneration(port, data, dataLenth);
    }

    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION))
    {
        if (icvGeneration->IcvGenerationTimeout != 0u)
        {
            StbM_VirtualLocalTimeType     virtualLocalTime;
            StbM_SynchronizedTimeBaseType timeBaseId = PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef;
            if (StbM_GetCurrentVirtualLocalTime(timeBaseId, &virtualLocalTime) == E_OK)
            {
                if (((uint64)((uint64)virtualLocalTime.nanosecondsHi << ETHTSYN_BIT_SHIFT_32)
                     | (uint64)virtualLocalTime.nanosecondsLo)
                        - port->ICVGenarationAndVerification.IcvIndicationStartTime
                    > icvGeneration->IcvGenerationTimeout)
                {
                    /* SWS_EthTSyn_00256 */
                    EthTSyn_ICVGenerationBuildFailBehavior(port, ETHTSYN_SEV_TSYN_ETH_ICV_GENERATION_FAILED);
                }
            }
        }
    }

    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION))
    {
        EthTSyn_SubTlvAuthenticatedBuild(port, data, dataLenth);
    }

    return port->ICVGenarationAndVerification.AuthenticatedFieldInfo.TimeAuthenticatedLength;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the action of ICV generation when status is ETHTSYN_ICV_TX_PROGRESS_STATUS_BUILDING
 *               or ETHTSYN_ICV_TX_PROGRESS_STATUS_RETRY.
 * @param[inout] port : port pointer.
 * @param[in]    data : the ptr of data to be sended.
 * @param[in]    dataLenth : the length of data to be sended.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85850
 */
ETHTSYN_LOCAL void EthTSyn_StartICVGeneration(EthTSyn_PortType* port, uint8* data, uint16 dataLenth)
{
    Std_ReturnType                               ret = E_OK;
    uint8                                        freshnessValue[ETHTSYN_MAX_FRESHNESS_VALUE_SIZE];
    const EthTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        (PORT_TOCFGPORT(port->DomainIndex).GlobalTimeTxIcvGeneration);
    EthTSyn_AuthenticatedFieldInfoType* authenticatedFieldInfo =
        &port->ICVGenarationAndVerification.AuthenticatedFieldInfo;
    EthTSyn_ICVGenerationAndVerificationType* icvGenarationAndVerification = &port->ICVGenarationAndVerification;
    icvGenarationAndVerification->AuthenticationBuildAttemptsCounter++;

    uint8 trunctedFvLength = EthTSyn_BitToBYTE(icvGeneration->IcvTxFreshnessValueTruncLength);
    if (trunctedFvLength > 0u)
    {
        ret = EthTSyn_TxFreshnessObtain(icvGeneration, freshnessValue, authenticatedFieldInfo->TruncatedFreshnessValue);
        if (ret == E_OK)
        {
            ; /* ICV generation attempt the following implementation. */
        }
        else if (ret == STBM_E_BUSY)
        {
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_RETRY);
        }
        else
        {
            /* SWS_EthTSyn_00250 */
            EthTSyn_ICVGenerationBuildFailBehavior(port, ETHTSYN_SEV_TSYN_ETH_FRESHNESS_NOT_AVAILABLE);
            (void)Det_ReportRuntimeError(
                ETHTSYN_MODULE_ID,
                ETHTSYN_INSTANCE_ID,
                ETHTSYN_SID_MAINFUNCTION,
                ETHTSYN_E_FRESHNESSFAILURE);
        }
    }

    if (ret == E_OK)
    {
        if (trunctedFvLength > 0u)
        {
            authenticatedFieldInfo->TimeAuthenticatedLength =
                ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET + trunctedFvLength + icvGeneration->IcvTxLength;
            authenticatedFieldInfo->ICVFlags = ETHTSYN_ICV_WITH_FV;
        }
        else
        {
            authenticatedFieldInfo->TimeAuthenticatedLength =
                ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET + icvGeneration->IcvTxLength;
            authenticatedFieldInfo->ICVFlags = 0u;
        }

        /* FV obtain successfully then build input data buffer for generation of ICV */
        uint8 icvGenerationBuffer[ETHTSYN_ICV_CIRCULAR_BUFFER_SIZE] = {0u};
        /* copy all of the data except Time Authenticated */
        IStdLib_MemCpy(&icvGenerationBuffer[0u], &data[0u], dataLenth);
        uint16 totalFollowUpLength =
            (uint16)(icvGenerationBuffer[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET] << ETHTSYN_BIT_SHIFT_8
                     | icvGenerationBuffer[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET + 1u]);
        uint16 totalTlvLength = (uint16)(icvGenerationBuffer[ETHTSYN_TLV_LENGTH_FIELD_OFFSET] << ETHTSYN_BIT_SHIFT_8
                                         | icvGenerationBuffer[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u]);
        uint8  fragmentNumber = ICV_FRAG_NUM(icvGeneration->IcvTxLength);
        if (fragmentNumber > 1u)
        {
            authenticatedFieldInfo->ICVFlags |= ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV;
        }
        totalFollowUpLength += authenticatedFieldInfo->TimeAuthenticatedLength;
        totalTlvLength += authenticatedFieldInfo->TimeAuthenticatedLength;
        totalFollowUpLength += ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET * (fragmentNumber - 1u);
        totalTlvLength += ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET * (fragmentNumber - 1u);
        /* FU Totol Length*/
        icvGenerationBuffer[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET] =
            (uint8)(totalFollowUpLength >> ETHTSYN_BIT_SHIFT_8);
        icvGenerationBuffer[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET + 1u] = (uint8)(totalFollowUpLength);
        /* TLV lengthField */
        icvGenerationBuffer[ETHTSYN_TLV_LENGTH_FIELD_OFFSET]      = (uint8)(totalTlvLength >> ETHTSYN_BIT_SHIFT_8);
        icvGenerationBuffer[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u] = (uint8)(totalTlvLength);
        /* type */
        icvGenerationBuffer[dataLenth] = ETHTSYN_SUB_TLV_TYPE_TIME_AUTHENTICATED;
        /* length */
        icvGenerationBuffer[dataLenth + ETHTSYN_ICV_LENGTH_OFFSET] =
            authenticatedFieldInfo->TimeAuthenticatedLength - ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
        /* ICV_Flags */
        icvGenerationBuffer[dataLenth + ETHTSYN_ICV_FLAG_OFFSET] = authenticatedFieldInfo->ICVFlags;
        /* SequenceNumber */
        icvGenerationBuffer[dataLenth + ETHTSYN_ICV_SEQUENCE_NUMBER_OFFSET] =
            icvGenarationAndVerification->TimeAuthencatedSequenceCounter;

        if (trunctedFvLength > 0u)
        {
            /* FreshnessValueLength in bits */
            icvGenerationBuffer[dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] =
                icvGeneration->IcvTxFreshnessValueTruncLength;
            /* include Full FV in ICV generationBuffer*/
            IStdLib_MemCpy(
                &icvGenerationBuffer[dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET],
                &freshnessValue[0u],
                (uint8)EthTSyn_BitToBYTE(icvGeneration->IcvTxFreshnessValueLength));
            dataLenth += ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET
                         + (uint16)EthTSyn_BitToBYTE(icvGeneration->IcvTxFreshnessValueLength);
            EthTSyn_ICVGeneration(port, (const uint8*)icvGenerationBuffer, dataLenth);
        }
        else
        {
            EthTSyn_ICVGeneration(
                port,
                (const uint8*)icvGenerationBuffer,
                dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET);
        }
    }

    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_RETRY)
        && (icvGenarationAndVerification->AuthenticationBuildAttemptsCounter
            >= icvGeneration->TxAuthenticationBuildAttempts))
    {
        /* SWS_EthTSyn_00256 */
        EthTSyn_ICVGenerationBuildFailBehavior(port, ETHTSYN_SEV_TSYN_ETH_ICV_GENERATION_FAILED);
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of packing the Sub-Tlv Time Authenticated.
 * @param[inout] port : port pointer.
 * @param[in]    data : the ptr of data to be sended.
 * @param[in]    dataLenth : the length of data to be sended.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85851
 */
ETHTSYN_LOCAL void EthTSyn_SubTlvAuthenticatedBuild(EthTSyn_PortType* port, uint8* data, uint16 dataLenth)
{
    const EthTSyn_GlobalTimeTxIcvGenerationType* icvGeneration =
        (PORT_TOCFGPORT(port->DomainIndex).GlobalTimeTxIcvGeneration);
    EthTSyn_AuthenticatedFieldInfoType* authenticatedFieldInfo =
        &port->ICVGenarationAndVerification.AuthenticatedFieldInfo;
    const EthTSyn_ICVGenerationAndVerificationType* icvGenarationAndVerification = &port->ICVGenarationAndVerification;

    uint8 firstFragmentIcvLength = icvGeneration->IcvTxLength > ETHTSYN_TIME_AUTHENTICATED_FIRST_FRAGMENT_MAX_ICV_LENGTH
                                       ? ETHTSYN_TIME_AUTHENTICATED_FIRST_FRAGMENT_MAX_ICV_LENGTH
                                       : icvGeneration->IcvTxLength;
    uint8 firstFragmentSubTlvLength = 0u;
    uint16 remainIcvLength          = icvGeneration->IcvTxLength - firstFragmentIcvLength;
    uint8  lastFragmentIcvLength    = remainIcvLength % ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH;
    lastFragmentIcvLength = (lastFragmentIcvLength == 0u) ? ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH
                                                          : lastFragmentIcvLength;

    uint8 fragmentNumber = ICV_FRAG_NUM(icvGeneration->IcvTxLength);

    if ((authenticatedFieldInfo->ICVFlags & ETHTSYN_ICV_FV_GENERATION_FAILED) != 0u)
    {
        fragmentNumber                                  = 1u;
        authenticatedFieldInfo->TimeAuthenticatedLength = ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET;
    }
    else if ((authenticatedFieldInfo->ICVFlags & ETHTSYN_ICV_WITH_FV) != 0u)
    {
        uint8 trunctedFvLength = EthTSyn_BitToBYTE(icvGeneration->IcvTxFreshnessValueTruncLength);
        /* FreshnessValueLength in bits */
        data[dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET] = icvGeneration->IcvTxFreshnessValueTruncLength;
        /* FV */
        IStdLib_MemCpy(
            &data[dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET],
            &authenticatedFieldInfo->TruncatedFreshnessValue[0u],
            trunctedFvLength);
        /* ICV with FV*/

        IStdLib_MemCpy(
            &data[dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET + trunctedFvLength + 1u],
            icvGenarationAndVerification->AuthenticatedFieldInfo.ICVBuffer,
            firstFragmentIcvLength);
    }
    else if (icvGeneration->IcvTxFreshnessValueTruncLength == 0u)
    {
        /* ICV without FV */

        IStdLib_MemCpy(
            &data[dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET],
            icvGenarationAndVerification->AuthenticatedFieldInfo.ICVBuffer,
            firstFragmentIcvLength);
    }
    /* add infomation except ICV */
    /* Type */
    data[dataLenth] = ETHTSYN_SUB_TLV_TYPE_TIME_AUTHENTICATED;
    /* ICV_Flags */
    data[dataLenth + ETHTSYN_ICV_FLAG_OFFSET] = authenticatedFieldInfo->ICVFlags;
    /* SequenceNumber */
    data[dataLenth + ETHTSYN_ICV_SEQUENCE_NUMBER_OFFSET] = 0u;
    if (fragmentNumber == 1u)
    {
        /* Length */
        data[dataLenth + ETHTSYN_ICV_LENGTH_OFFSET] =
            authenticatedFieldInfo->TimeAuthenticatedLength - ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
    }
    else
    {
        firstFragmentSubTlvLength = authenticatedFieldInfo->TimeAuthenticatedLength - icvGeneration->IcvTxLength
                                    + ETHTSYN_TIME_AUTHENTICATED_FIRST_FRAGMENT_MAX_ICV_LENGTH;
        /* Length */
        data[dataLenth + ETHTSYN_ICV_LENGTH_OFFSET] = firstFragmentSubTlvLength - ETHTSYN_FUP_SUBTLV_HEADER_SIZE;

        authenticatedFieldInfo->ICVFlags |= ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV;
        /* ICV_Flags */
        data[dataLenth + ETHTSYN_ICV_FLAG_OFFSET] = authenticatedFieldInfo->ICVFlags;
        dataLenth += firstFragmentSubTlvLength;
    }

    uint8 remainFragmentSubTlvLength = authenticatedFieldInfo->TimeAuthenticatedLength - icvGeneration->IcvTxLength
                                       + ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH;
    /* when ICV without FV, the length of Insert or final fragment does not need to be minus */
    /* IcvTxFreshnessValueTruncLength */
    uint8 lastFragmentSubTlvLength =
        authenticatedFieldInfo->TimeAuthenticatedLength - icvGeneration->IcvTxLength + lastFragmentIcvLength;
    /* when ICV with FV, the length of Insert or final fragment with FV needs to be minus */
    /* IcvTxFreshnessValueTruncLength */
    if ((authenticatedFieldInfo->ICVFlags & ETHTSYN_ICV_WITH_FV) != 0u)
    {
        remainFragmentSubTlvLength -= (EthTSyn_BitToBYTE(icvGeneration->IcvTxFreshnessValueTruncLength) + 1u);
        lastFragmentSubTlvLength -= (EthTSyn_BitToBYTE(icvGeneration->IcvTxFreshnessValueTruncLength) + 1u);
    }

    for (uint8 fragmentIndex = 1u; fragmentIndex < fragmentNumber; fragmentIndex++)
    {
        /* Type */
        data[dataLenth] = ETHTSYN_SUB_TLV_TYPE_TIME_AUTHENTICATED;
        /* ICV_Flags */
        data[dataLenth + ETHTSYN_ICV_FLAG_OFFSET] = authenticatedFieldInfo->ICVFlags;
        /* SequenceNumber */
        data[dataLenth + ETHTSYN_ICV_SEQUENCE_NUMBER_OFFSET] = fragmentIndex;
        /* remaining fragment exclude last one */
        if (fragmentIndex < fragmentNumber - 1u)
        {
            /* Length */
            data[dataLenth + ETHTSYN_ICV_LENGTH_OFFSET] = remainFragmentSubTlvLength - ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
            /* ICV */
            IStdLib_MemCpy(
                &data[dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET],
                icvGenarationAndVerification->AuthenticatedFieldInfo.ICVBuffer + firstFragmentIcvLength
                    + ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH * (fragmentIndex - 1u),
                ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH);
            dataLenth += remainFragmentSubTlvLength;
        }
        /* last fragment */
        else
        {
            /* Length */
            data[dataLenth + ETHTSYN_ICV_LENGTH_OFFSET] = lastFragmentSubTlvLength - ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
            /* ICV */
            IStdLib_MemCpy(
                &data[dataLenth + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET],
                icvGenarationAndVerification->AuthenticatedFieldInfo.ICVBuffer + firstFragmentIcvLength
                    + ETHTSYN_TIME_AUTHENTICATED_REMAIN_FRAGMENT_MAX_ICV_LENGTH * (fragmentIndex - 1u),
                lastFragmentIcvLength);
            /* ICV_Flags */
            data[dataLenth + ETHTSYN_ICV_FLAG_OFFSET] &= (uint8)(~(uint8)(ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV));
        }
    }

    uint16 totalFollowUpLength = (uint16)(data[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET] << ETHTSYN_BIT_SHIFT_8
                                          | data[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET + 1u]);
    uint16 totalTlvLength      = (uint16)(data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET] << ETHTSYN_BIT_SHIFT_8
                                     | data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u]);
    if (fragmentNumber > 1u)
    {
        authenticatedFieldInfo->TimeAuthenticatedLength =
            firstFragmentSubTlvLength + (fragmentNumber - 2) * remainFragmentSubTlvLength + lastFragmentSubTlvLength;
    }
    totalFollowUpLength += authenticatedFieldInfo->TimeAuthenticatedLength;
    totalTlvLength += authenticatedFieldInfo->TimeAuthenticatedLength;
    /* FU Totol Length*/
    data[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET]      = (uint8)(totalFollowUpLength >> ETHTSYN_BIT_SHIFT_8);
    data[ETHTSYN_FUP_MESSAGE_TOTAL_LENGTH_OFFSET + 1u] = (uint8)(totalFollowUpLength);
    /* TLV lengthField */
    data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET]      = (uint8)(totalTlvLength >> ETHTSYN_BIT_SHIFT_8);
    data[ETHTSYN_TLV_LENGTH_FIELD_OFFSET + 1u] = (uint8)(totalTlvLength);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of changing the status of ICV generation or verification.
 * @param[inout] port : port pointer.
 * @param[in]    toState : the new status of ICV generation or verification.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-83037
 */
ETHTSYN_LOCAL_INLINE void EthTSyn_ICVStatusChange(EthTSyn_PortType* port, uint8 toState)
{
    port->ICVGenarationAndVerification.IcvProgressStatus = toState;
}
/**
 * @brief        the implementation of comparing the status of ICV generation or verification.
 * @param[in]    port : port pointer.
 * @param[in]    State : the state to be compared with the current state of ICV generation or verification.
 * @retval       TRUE: the current state is State.
 * @retval       FALSE: the current state is not State.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-83039
 */
ETHTSYN_LOCAL_INLINE boolean EthTSyn_ICVStatusComparison(const EthTSyn_PortType* port, uint8 State)
{
    return port->ICVGenarationAndVerification.IcvProgressStatus == State;
}
/**
 * @brief        the behavior when ICV generation failed.
 * @param[inout] port : port pointer.
 * @param[in]    eventId : the event id of ICV generation failed.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-83040
 */
ETHTSYN_LOCAL_INLINE void EthTSyn_ICVGenerationBuildFailBehavior(EthTSyn_PortType* port, uint16 eventId)
{
    port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter = 0u;
    /* there will be no FV and ICV*/
    port->ICVGenarationAndVerification.AuthenticatedFieldInfo.TimeAuthenticatedLength =
        ETHTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH;
    port->ICVGenarationAndVerification.AuthenticatedFieldInfo.ICVFlags = ETHTSYN_ICV_FV_GENERATION_FAILED;
    EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION);

#if (ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
    IdsM_SetSecurityEventWithContextData(eventId, &port->TimeDomainId, 1u);
#else
    ETHTSYN_UNUSED(eventId);
#endif
}

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of verifying the sub-tlv in the FU message,which contains the Time Authenticated.
 *                But not do the ICV verifying.
 * @param[in]     data: raw data buffer to be validated.
 * @param[in]     dataTap : Time Authenticated Sub-TLV offset in data buffer.
 * @param[in]     length: the length of FU message.
 * @param[in]     portId: the index of configured port.
 * @retval        E_OK: crc validation passed.
 * @retval        E_NOT_OK: crc validation failed.
 * @synchronous   TRUE
 * @reentrant     TRUE
 * @trace         CPD-85852
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_VerifyTimeAuthenticatedSubTlv(const uint8* data, uint8 dataTap, uint16 length, uint16 portId)
{
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Std_ReturnType                        ret                   = E_NOT_OK;
    const EthTSyn_GlobalTimeSlaveCfgType* globalTimeSlaveCfgPtr = PORT_TOCFGDOMIN(portId).PortRole.GlobalTimeSlave;

    switch (globalTimeSlaveCfgPtr->RxIcvVerification)
    {
    /* PRS_TS_00243 PRS_TS_00244 PRS_TS_00245 PRS_TS_00246 PRS_TS_00247 */
    case ETHTSYN_ICV_VERIFIED:
    case ETHTSYN_ICV_OPTIONAL:
    {
        EthTSyn_PortType*                              port            = EthTSyn_ConfigData->RuntimeVariables[portId];
        uint8                                          receivedICVFlag = data[dataTap + 1u + ETHTSYN_ICV_FLAG_OFFSET];
        const EthTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
            (PORT_TOCFGPORT(port->DomainIndex).GlobalTimeRxIcvVerification);
        if (((receivedICVFlag & ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV) != 0u)
            && (EthTSyn_ValidateMultipleFragment(icvVerification, data, dataTap) != E_OK))
        {
            break;
        }

        uint16 receivedTimeAuthenticatedlengthFiled =
            data[dataTap + 1u + ETHTSYN_ICV_LENGTH_OFFSET] + ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
        uint16 expectedTimeAuthenticatedlengthFiled;
        uint8  receivedFvLength = 0u;

        if ((receivedICVFlag & ETHTSYN_ICV_WITH_FV) == 0u)
        {
            expectedTimeAuthenticatedlengthFiled =
                icvVerification->IcvRxLength + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET;
        }
        else
        {
            receivedFvLength                     = data[dataTap + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET];
            expectedTimeAuthenticatedlengthFiled = icvVerification->IcvRxLength
                                                   + EthTSyn_BitToBYTE(icvVerification->IcvRxFreshnessValueTruncLength)
                                                   + ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET;
        }

        if (((receivedICVFlag & ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV) == 0u)
            && (receivedTimeAuthenticatedlengthFiled != expectedTimeAuthenticatedlengthFiled))
        {
            break;
        }

        if (((icvVerification->IcvRxFreshnessValueTruncLength != 0u) && (receivedFvLength == 0u))
            || ((icvVerification->IcvRxFreshnessValueTruncLength == 0u) && (receivedFvLength != 0u)))
        {
            break;
        }
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START);

        uint8 dataLengthHigh               = (uint8)(length >> ETHTSYN_BIT_SHIFT_8);
        uint8 dataLengthLow                = (uint8)(length & 0xFFu);
        uint8 timeAuthenticatedFieldOffset = dataTap;
        (void)EthTSyn_CircularBufferWrite(&port->ICVGenarationAndVerification.RxICVBufferToken, &dataLengthHigh, 1u);
        (void)EthTSyn_CircularBufferWrite(&port->ICVGenarationAndVerification.RxICVBufferToken, &dataLengthLow, 1u);
        (void)EthTSyn_CircularBufferWrite(
            &port->ICVGenarationAndVerification.RxICVBufferToken,
            &timeAuthenticatedFieldOffset,
            1u);
        (void)EthTSyn_CircularBufferWrite(&port->ICVGenarationAndVerification.RxICVBufferToken, &data[0u], length);

        ret = ETHTSYN_E_VALIDATE_ICV;
        break;
    }
    case ETHTSYN_ICV_IGNORED:
    {
        ret = E_OK;
        break;
    }
    case ETHTSYN_ICV_NOT_VERIFIED:
    {
        ret = E_NOT_OK;
        break;
    }
    default:
        ret = E_NOT_OK;
        break;
    }

    return ret;
    /* PRQA S 3120 -- */
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the mainfunction of ICV verification at slave port.
 * @param[inout] port : port pointer.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85853
 */
ETHTSYN_LOCAL void EthTSyn_ICVRxMainfunction(EthTSyn_PortType* port)
{
    const EthTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        (PORT_TOCFGPORT(port->DomainIndex).GlobalTimeRxIcvVerification);

    /* Rxindication validates the Sub-Tlv Time Authenticated successfully --->
     * (ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL ---> ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START) */
    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START)
        || EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY))
    {
        EthTSyn_StartICVVerification(port);
    }
    /* csm job is configured to synchronou and obtain the FV succesfully --->
     * (ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START--->ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION) */
    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION))
    {
        if (icvVerification->IcvVerificationTimeout != 0u)
        {
            StbM_VirtualLocalTimeType     virtualLocalTime;
            StbM_SynchronizedTimeBaseType timeBaseId = PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef;
            if (StbM_GetCurrentVirtualLocalTime(timeBaseId, &virtualLocalTime) == E_OK)
            {
                if (((uint64)((uint64)((uint64)virtualLocalTime.nanosecondsHi << ETHTSYN_BIT_SHIFT_32)
                              | (uint64)virtualLocalTime.nanosecondsLo)
                     - port->ICVGenarationAndVerification.IcvIndicationStartTime)
                    > icvVerification->IcvVerificationTimeout)
                {
                    /* SWS_EthTSyn_00243 */
                    EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
                    IdsM_SetSecurityEventWithContextData(
                        ETHTSYN_SEV_TSYN_ETH_ICV_VERIFICATION_FAILED,
                        &port->TimeDomainId,
                        1u);
#endif
                }
            }
        }
    }

    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED))
    {
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL);
    }

    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_COMFIRMATION))
    {
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL);
        if (port->ICVGenarationAndVerification.RxVerifyResult == CRYPTO_E_VER_OK)
        {
            EthTSyn_ValidFollowUpMessageSetGlobalTime(port, port->RecRawDataPtr, NULL_PTR);
        }
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the action of ICV generation when status is ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START
 *               or ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY.
 * @param[inout] port : port pointer.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85854
 */
ETHTSYN_LOCAL void EthTSyn_StartICVVerification(EthTSyn_PortType* port)
{
    if (EthTSyn_ICVStatusComparison(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START))
    {
        uint8 dataLengthHigh = 0u;
        uint8 dataLengthLow  = 0u;
        (void)EthTSyn_CircularBufferRead(&port->ICVGenarationAndVerification.RxICVBufferToken, &dataLengthHigh, 1u);
        (void)EthTSyn_CircularBufferRead(&port->ICVGenarationAndVerification.RxICVBufferToken, &dataLengthLow, 1u);
        uint16 datalength = (uint16)(dataLengthHigh << ETHTSYN_BIT_SHIFT_8) | dataLengthLow;
        (void)EthTSyn_CircularBufferRead(
            &port->ICVGenarationAndVerification.RxICVBufferToken,
            &port->TimeAuthenticatedFieldOffset,
            1u);
        (void)EthTSyn_CircularBufferRead(
            &port->ICVGenarationAndVerification.RxICVBufferToken,
            &port->RecRawDataPtr[0u],
            datalength);
    }

    Std_ReturnType                                 ret = E_OK;
    uint8                                          receivedFvValue[ETHTSYN_MAX_FRESHNESS_VALUE_SIZE];
    const EthTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        (PORT_TOCFGPORT(port->DomainIndex).GlobalTimeRxIcvVerification);

    uint8 authenticatedBuildCounter   = port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter;
    uint8 verificationAttemptsCounter = port->ICVGenarationAndVerification.IcvVerificationAttemptsCounter;

    uint8 receivedFvLength =
        port->RecRawDataPtr[port->TimeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET];

    uint8 receivedICVFlag = port->RecRawDataPtr[port->TimeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FLAG_OFFSET];
    if ((port->RecRawDataPtr[port->TimeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_LENGTH_OFFSET]
         + ETHTSYN_FUP_SUBTLV_HEADER_SIZE)
        == (ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET + icvVerification->IcvRxLength))
    {
        receivedFvLength = 0u;
    }
    boolean isAuthBuildComplete = (authenticatedBuildCounter == icvVerification->RxAuthenticationBuildAttempts)
                                  && (icvVerification->RxAuthenticationBuildAttempts != 0U);

    boolean isVerificationComplete = (verificationAttemptsCounter == icvVerification->IcvVerificationAttempts)
                                     && (icvVerification->IcvVerificationAttempts != 0U);
    if (isAuthBuildComplete || isVerificationComplete)
    {
        port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter = 0u;
        port->ICVGenarationAndVerification.IcvVerificationAttemptsCounter     = 0u;
        /* SWS_EthTSyn_00243 */
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
        IdsM_SetSecurityEventWithContextData(ETHTSYN_SEV_TSYN_ETH_ICV_VERIFICATION_FAILED, &port->TimeDomainId, 1u);
#endif
    }
    else if ((receivedFvLength == 0u) && ((receivedICVFlag & ETHTSYN_ICV_WITH_FV) != 0u))
    {
        /* SWS_EthTSyn_00236 */
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
        IdsM_SetSecurityEventWithContextData(ETHTSYN_SEV_TSYN_ETH_FRESHNESS_NOT_AVAILABLE, &port->TimeDomainId, 1u);
#endif
        (void)Det_ReportRuntimeError(
            ETHTSYN_MODULE_ID,
            ETHTSYN_INSTANCE_ID,
            ETHTSYN_SID_MAINFUNCTION,
            ETHTSYN_E_FRESHNESSFAILURE);
    }
    else if (icvVerification->IcvRxFreshnessValueTruncLength != 0u)
    {
        ret = EthTSyn_RxFreshnessObtain(port, receivedFvValue, port->TimeAuthenticatedFieldOffset);
        if (ret == E_OK)
        {
            /* FV obtain successfully*/
            uint8 icvGenerationBuffer[ETHTSYN_ICV_CIRCULAR_BUFFER_SIZE] = {0u};
            /* copy all of the data except FV */
            IStdLib_MemCpy(
                &icvGenerationBuffer[0u],
                &port->RecRawDataPtr[0u],
                port->TimeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET);
            if (ICV_FRAG_NUM(icvVerification->IcvRxLength) > 1u)
            {
                icvGenerationBuffer[port->TimeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_LENGTH_OFFSET] =
                    ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET + EthTSyn_BitToBYTE(icvVerification->IcvRxFreshnessValueLength)
                    + icvVerification->IcvRxLength - ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
            }

            /* include Full FV in ICV generationBuffer*/
            IStdLib_MemCpy(
                &icvGenerationBuffer[port->TimeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET],
                &receivedFvValue[0u],
                EthTSyn_BitToBYTE(icvVerification->IcvRxFreshnessValueLength));
            EthTSyn_ICVVerification(
                port,
                (const uint8*)icvGenerationBuffer,
                port->TimeAuthenticatedFieldOffset,
                port->RecRawDataPtr);
        }
        else if (ret == E_NOT_OK)
        {
            /* SWS_EthTSyn_00236 */
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
            IdsM_SetSecurityEventWithContextData(ETHTSYN_SEV_TSYN_ETH_FRESHNESS_NOT_AVAILABLE, &port->TimeDomainId, 1u);
#endif
            (void)Det_ReportRuntimeError(
                ETHTSYN_MODULE_ID,
                ETHTSYN_INSTANCE_ID,
                ETHTSYN_SID_MAINFUNCTION,
                ETHTSYN_E_FRESHNESSFAILURE);
        }
        else if (ret == E_BUSY)
        {
            /* retry build*/
            port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter++;
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY);
        }
    }
    else
    {
        uint8  icvGenerationBuffer[ETHTSYN_ICV_CIRCULAR_BUFFER_SIZE] = {0u};
        uint16 timeAuthenticatedSubtlvLength =
            port->TimeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET;
        IStdLib_MemCpy(&icvGenerationBuffer[0u], &port->RecRawDataPtr[0u], timeAuthenticatedSubtlvLength);
        if (ICV_FRAG_NUM(icvVerification->IcvRxLength) > 1u)
        {
            icvGenerationBuffer[port->TimeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_LENGTH_OFFSET] =
                ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET + EthTSyn_BitToBYTE(icvVerification->IcvRxFreshnessValueLength)
                + icvVerification->IcvRxLength - ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
        }
        /* no FV in Time Authenticated */
        EthTSyn_ICVVerification(port, port->RecRawDataPtr, port->TimeAuthenticatedFieldOffset, port->RecRawDataPtr);
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        Obtaining the freshness value of RX.
 * @param[out]   fvValue : the ptr of FULL freshness value.
 * @param[inout] port : port pointer.
 * @param[in]    timeAuthenticatedFieldOffset : Offset to the start of time-authenticated field in received data
 * @retval       E_OK: obtaining the freshness value of TX successfully.
 * @retval       E_NOT_OK: obtaining the freshness value of TX failed.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-83044
 */
ETHTSYN_LOCAL Std_ReturnType
    EthTSyn_RxFreshnessObtain(const EthTSyn_PortType* port, uint8* fvValue, uint8 timeAuthenticatedFieldOffset)
{
    Std_ReturnType                                 ret = E_NOT_OK;
    const EthTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        EthTSyn_ConfigData->GlobalTimeDomain->PortConfig[port->DomainIndex].GlobalTimeRxIcvVerification;

    const uint8* dataPtr = port->RecRawDataPtr;

    uint32 truncatedFreshnessValueLength =
        (uint32)dataPtr[timeAuthenticatedFieldOffset + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET + 1u];
    const uint8* receivedFvValuePtr = &dataPtr[timeAuthenticatedFieldOffset + ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET + 1u];

    uint8 receivedICVFlag = dataPtr[timeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FLAG_OFFSET];

    if ((receivedICVFlag & ETHTSYN_ICV_FV_GENERATION_FAILED) == 0u)
    {
        uint32 configuredFreshnessValueLen = icvVerification->IcvRxFreshnessValueLength;
        /* SWS_EthTSyn_00233 */
        ret = StbM_GetRxFreshness(
            icvVerification->IcvVerificationFvIdRef,
            receivedFvValuePtr,
            truncatedFreshnessValueLength,
            port->ICVGenarationAndVerification.IcvVerificationAttemptsCounter,
            fvValue,
            &configuredFreshnessValueLen);
    }

    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief        the implementation of ICV verification at Slave port.
 * @param[inout] port : port pointer.
 * @param[in]    generationBuffer : Contains the pointer to the data for which the MAC shall be computed.
 * @param[in]    timeAuthenticatedFieldOffset : the byte offset of TimeAuthenticatedField in recRawData.
 * @param[in]    recRawDataPtr: the pointer of received raw data.
 * @reentrant    TRUE
 * @synchronous  FALSE
 * @trace        CPD-85855
 */
ETHTSYN_LOCAL void EthTSyn_ICVVerification(
    EthTSyn_PortType* port,
    const uint8*      generationBuffer,
    uint8             timeAuthenticatedFieldOffset,
    const uint8*      recRawDataPtr)
{
    Std_ReturnType                                 ret = E_NOT_OK;
    const EthTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        EthTSyn_ConfigData->GlobalTimeDomain->PortConfig[port->DomainIndex].GlobalTimeRxIcvVerification;
    uint8 icvOffset;
    uint8 dataLength;

    if ((recRawDataPtr[timeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FLAG_OFFSET] & ETHTSYN_ICV_WITH_FV) == 0u)
    {
        icvOffset  = timeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET;
        dataLength = icvOffset;
    }
    else
    {
        icvOffset = timeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET
                    + (uint8)EthTSyn_BitToBYTE(icvVerification->IcvRxFreshnessValueTruncLength);
        dataLength = timeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_OFFSET
                     + (uint8)EthTSyn_BitToBYTE(icvVerification->IcvRxFreshnessValueLength);
    }

    const uint8* receivedICV = NULL_PTR;

    if ((recRawDataPtr[timeAuthenticatedFieldOffset + 1u + ETHTSYN_ICV_FLAG_OFFSET] & ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV)
        == 0u)
    {
        receivedICV = &recRawDataPtr[icvOffset];
    }
    else
    {
        uint8 aggregateIcvBuffer[ETHTSYN_MAX_ICV_SIZE];
        EthTSyn_ObtainIcvInMultipleFragment(port, aggregateIcvBuffer, timeAuthenticatedFieldOffset);
        receivedICV = aggregateIcvBuffer;
    }

    switch (icvVerification->RxICVVerificationConfig.IcvVerificationBase)
    {
    case ETHTSYN_ICV_MAC:
        ret = Csm_MacVerify(
            icvVerification->RxICVVerificationConfig.IcvVerificationJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            &generationBuffer[0u],
            dataLength,
            receivedICV,
            icvVerification->IcvRxLength,
            &port->ICVGenarationAndVerification.RxVerifyResult);
        break;
    case ETHTSYN_ICV_SIGNATURE:
        ret = Csm_SignatureVerify(
            icvVerification->RxICVVerificationConfig.IcvVerificationJobRef,
            CRYPTO_OPERATIONMODE_SINGLECALL,
            &generationBuffer[0u],
            dataLength,
            receivedICV,
            EthTSyn_BitToBYTE(icvVerification->IcvRxLength),
            &port->ICVGenarationAndVerification.RxVerifyResult);
        break;
    default:
        ret = E_NOT_OK;
        break;
    }

    if (ret == E_OK)
    {
        if (icvVerification->RxICVVerificationConfig.IcvProcessingType == ETHTSYN_CRYPTO_PROCESSING_SYNC)
        {
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_COMFIRMATION);
        }
        else if (icvVerification->IcvVerificationTimeout != 0u)
        {
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION);
            StbM_VirtualLocalTimeType VirtualLocalTime;
            if (StbM_GetCurrentVirtualLocalTime(
                    PORT_TOCFGDOMIN(port->DomainIndex).SynchronizedTimeBaseRef,
                    &VirtualLocalTime)
                == E_OK)
            {
                port->ICVGenarationAndVerification.IcvIndicationStartTime =
                    (uint64)(((uint64)VirtualLocalTime.nanosecondsHi << ETHTSYN_BIT_SHIFT_32)
                             | (uint64)VirtualLocalTime.nanosecondsLo);
            }
        }
        else
        {
            EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION);
        }
    }
    else if (ret == E_NOT_OK)
    {
        /* SWS_EthTSyn_00243 */
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED);
#if (ETHTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
        IdsM_SetSecurityEventWithContextData(ETHTSYN_SEV_TSYN_ETH_ICV_VERIFICATION_FAILED, &port->TimeDomainId, 1u);
#endif
    }
    else if (ret == CRYPTO_E_BUSY)
    {
        /* SWS_EthTSyn_00241 */
        port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter++;
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY);
    }
    else
    {
        /* SWS_EthTSyn_00411 */
        port->ICVGenarationAndVerification.IcvVerificationAttemptsCounter++;
        port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter = 0u;
        EthTSyn_ICVStatusChange(port, ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY);
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of initializing the ICV runtime variables.
 * @param[inout]  port: port pointer.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83046
 */
ETHTSYN_LOCAL void EthTSyn_InitPortICVRuntimeVariables(EthTSyn_PortType* port)
{
    EthTSyn_PortConfigCfgType portConifg = PORT_TOCFGPORT(port->DomainIndex);

    (void)IStdLib_MemSet(&port->RecRawDataPtr[0u], 0, ETHTSYN_ICV_CIRCULAR_BUFFER_SIZE);
    port->ICVGenarationAndVerification.IcvIndicationStartTime             = 0u;
    port->ICVGenarationAndVerification.AuthenticationBuildAttemptsCounter = 0u;
    if ((port->PortRoleType == ETHTSYN_MASTER) && (portConifg.GlobalTimeTxIcvGeneration != NULL_PTR))
    {
        (void)IStdLib_MemSet(
            &port->ICVGenarationAndVerification.AuthenticatedFieldInfo.ICVBuffer[0u],
            0,
            portConifg.GlobalTimeTxIcvGeneration->IcvTxLength);
        (void)IStdLib_MemSet(
            &port->ICVGenarationAndVerification.AuthenticatedFieldInfo,
            0u,
            sizeof(EthTSyn_AuthenticatedFieldInfoType));
        port->ICVGenarationAndVerification.TimeAuthencatedSequenceCounter = 0u;
        port->ICVGenarationAndVerification.IcvProgressStatus              = ETHTSYN_ICV_TX_PROGRESS_STATUS_BUILDING;
    }
    else if ((port->PortRoleType == ETHTSYN_SLAVE) && (portConifg.GlobalTimeRxIcvVerification != NULL_PTR))
    {
        port->ICVGenarationAndVerification.TimeAuthenticatedFieldOffset   = 0u;
        port->ICVGenarationAndVerification.IcvVerificationAttemptsCounter = 0u;
        port->ICVGenarationAndVerification.IcvProgressStatus = ETHTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL;
        /* init circular buffer */
        port->ICVGenarationAndVerification.RxICVBufferToken.Buffer =
            portConifg.GlobalTimeRxIcvVerification->RxIcvCircularBuffer;
        port->ICVGenarationAndVerification.RxICVBufferToken.Capacity = ETHTSYN_ICV_CIRCULAR_BUFFER_SIZE;
        port->ICVGenarationAndVerification.RxICVBufferToken.Head     = 0u;
        port->ICVGenarationAndVerification.RxICVBufferToken.Tail     = 0u;
        port->ICVGenarationAndVerification.RxICVBufferToken.Count    = 0u;
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of checking the circular buffer is valid.
 * @param[in]     cb: circular buffer table pointer.
 * @retval        TRUE:  validation passed.
 * @retval        FALSE: validation failed.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83174
 */
ETHTSYN_LOCAL_INLINE boolean EthTSyn_IsBufferValid(const EthTSyn_CircularBufferType* cb)
{
    return (cb != NULL_PTR) && (cb->Buffer != NULL_PTR) && (cb->Capacity > 0);
}
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of writing data to the circular buffer.
 * @param[inout]  cb: circular buffer table pointer.
 * @param[in]     data: data pointer.
 * @param[in]     size: data size.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83171
 */
ETHTSYN_LOCAL uint16 EthTSyn_CircularBufferWrite(EthTSyn_CircularBufferType* cb, const uint8* data, uint16 size)
{
    if (!EthTSyn_IsBufferValid(cb) || (data == NULL_PTR) || (size == 0))
    {
        return 0;
    }

    SchM_Enter_EthTSyn_Context();

    uint16 freeSpace = cb->Capacity - cb->Count;

    if (freeSpace == 0)
    {
        SchM_Exit_EthTSyn_Context();
        return 0;
    }

    if (size > freeSpace)
    {
        size = freeSpace;
    }

    uint16 firstChunk = cb->Capacity - cb->Head;

    if (firstChunk >= size)
    {
        /* no need to wrap around */
        IStdLib_MemCpy(cb->Buffer + cb->Head, data, size);
        cb->Head += size;

        if (cb->Head == cb->Capacity)
        {
            cb->Head = 0;
        }
    }
    else
    {
        /* need to wrap around */
        IStdLib_MemCpy(cb->Buffer + cb->Head, data, firstChunk);
        IStdLib_MemCpy(cb->Buffer, data + firstChunk, size - firstChunk);
        cb->Head = size - firstChunk;
    }

    cb->Count += size;

    SchM_Exit_EthTSyn_Context();
    return size;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of reading data from the circular buffer.
 * @param[inout]  cb: circular buffer table pointer.
 * @param[in]     data: data pointer.
 * @param[in]     size: data size.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83172
 */
ETHTSYN_LOCAL uint16 EthTSyn_CircularBufferRead(EthTSyn_CircularBufferType* cb, uint8* data, uint16 size)
{
    SchM_Enter_EthTSyn_Context();
    if (!EthTSyn_IsBufferValid(cb) || (data == NULL_PTR) || (size == 0))
    {
        return 0;
    }

    uint16 usedSpace = cb->Count;
    if (size > usedSpace)
    {
        size = usedSpace;
    }

    uint16 firstChunk = cb->Capacity - cb->Tail;

    if (firstChunk >= size)
    {
        /* no need to wrap around */
        IStdLib_MemCpy(data, cb->Buffer + cb->Tail, size);
        cb->Tail += size;

        if (cb->Tail == cb->Capacity)
        {
            cb->Tail = 0;
        }
    }
    else
    {
        /* need to wrap around */
        IStdLib_MemCpy(data, cb->Buffer + cb->Tail, firstChunk);
        IStdLib_MemCpy(data + firstChunk, cb->Buffer, size - firstChunk);
        cb->Tail = size - firstChunk;
    }
    cb->Count -= size;
    SchM_Exit_EthTSyn_Context();
    return size;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of validating multiple fragment ICV.
 * @param[in]     icvVerification: ICV verification table pointer.
 * @param[in]     data: data pointer.
 * @param[in]     dataTap: data offset.
 * @retval        E_OK:  validation passed.
 * @retval        E_NOT_OK: validation failed.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-85757
 */
ETHTSYN_LOCAL uint8 EthTSyn_ValidateMultipleFragment(
    const EthTSyn_GlobalTimeRxIcvVerificationType* icvVerification,
    const uint8*                                   data,
    uint8                                          dataTap)
{
    uint8  ret                              = E_OK;
    uint16 rxConfigIcvLength                = icvVerification->IcvRxLength;
    uint16 remainReceivedIcvLength          = 0u;
    uint8  dataOffset                       = 0u;
    uint8  timeAuthenticatedLengthExludeIcv = 0u;
    if ((data[dataTap + 1u + ETHTSYN_ICV_FLAG_OFFSET] & ETHTSYN_ICV_WITH_FV) == 0u)
    {
        timeAuthenticatedLengthExludeIcv = ETHTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH;
    }
    else
    {
        timeAuthenticatedLengthExludeIcv = ETHTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH + 1u
                                           + EthTSyn_BitToBYTE(icvVerification->IcvRxFreshnessValueTruncLength);
    }
    uint8 firstFragmentIcvLength = data[dataTap + 1u + ETHTSYN_ICV_LENGTH_OFFSET] + ETHTSYN_FUP_SUBTLV_HEADER_SIZE
                                   - timeAuthenticatedLengthExludeIcv;
    dataOffset += (data[dataTap + 1u + ETHTSYN_ICV_LENGTH_OFFSET] + ETHTSYN_FUP_SUBTLV_HEADER_SIZE);

    do
    {
        uint8 authenticatedSubTlvLength = data[dataOffset + dataTap + 1u + ETHTSYN_ICV_LENGTH_OFFSET];
        remainReceivedIcvLength +=
            (authenticatedSubTlvLength + ETHTSYN_FUP_SUBTLV_HEADER_SIZE
             - ETHTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH);
        dataOffset += (authenticatedSubTlvLength + ETHTSYN_FUP_SUBTLV_HEADER_SIZE);
    } while ((data[dataTap + 1u + ETHTSYN_ICV_FLAG_OFFSET + dataOffset] & ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV) != 0u);

    if ((firstFragmentIcvLength + remainReceivedIcvLength) != rxConfigIcvLength)
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         The implementation of obtaining ICV in multiple fragment.
 * @param[inout]  port: port table pointer.
 * @param[inout]  receivedIcv: received ICV pointer.
 * @param[in]     dataTap: data offset.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-85758
 */
ETHTSYN_LOCAL void EthTSyn_ObtainIcvInMultipleFragment(EthTSyn_PortType* port, uint8* receivedIcv, uint8 dataTap)
{
    uint16                                         dataOffset                       = 0u;
    uint16                                         icvOffset                        = 0u;
    uint8                                          timeAuthenticatedLengthExludeIcv = 0u;
    const EthTSyn_GlobalTimeRxIcvVerificationType* icvVerification =
        EthTSyn_ConfigData->GlobalTimeDomain->PortConfig[port->DomainIndex].GlobalTimeRxIcvVerification;
    const uint8* data = port->RecRawDataPtr;
    if ((data[dataTap + 1u + ETHTSYN_ICV_FLAG_OFFSET] & ETHTSYN_ICV_WITH_FV) == 0u)
    {
        timeAuthenticatedLengthExludeIcv = ETHTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH;
    }
    else
    {
        timeAuthenticatedLengthExludeIcv = ETHTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH + 1u
                                           + EthTSyn_BitToBYTE(icvVerification->IcvRxFreshnessValueTruncLength);
    }
    uint8 firstFragmentIcvLength = data[dataTap + 1u + ETHTSYN_ICV_LENGTH_OFFSET] + ETHTSYN_FUP_SUBTLV_HEADER_SIZE
                                   - timeAuthenticatedLengthExludeIcv;
    uint8 remainFragmentIcvLength = 0;
    dataOffset += (data[dataTap + 1u + ETHTSYN_ICV_LENGTH_OFFSET] + ETHTSYN_FUP_SUBTLV_HEADER_SIZE);
    /* first fragment */
    IStdLib_MemCpy(receivedIcv, data + dataTap + 1u + timeAuthenticatedLengthExludeIcv, firstFragmentIcvLength);
    icvOffset += firstFragmentIcvLength;
    do
    {
        uint8 authenticatedSubTlvLength = data[dataOffset + dataTap + 1u + ETHTSYN_ICV_LENGTH_OFFSET];
        remainFragmentIcvLength =
            (authenticatedSubTlvLength + ETHTSYN_FUP_SUBTLV_HEADER_SIZE
             - ETHTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH);
        IStdLib_MemCpy(
            receivedIcv + icvOffset,
            data + dataOffset + dataTap + 1u + ETHTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET,
            remainFragmentIcvLength);
        dataOffset += (authenticatedSubTlvLength + ETHTSYN_FUP_SUBTLV_HEADER_SIZE);
        icvOffset += remainFragmentIcvLength;
    } while ((data[dataTap + 1u + ETHTSYN_ICV_FLAG_OFFSET + dataOffset] & ETHTSYN_ICV_IN_MULTIPLE_SUB_TLV) != 0u);
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of setting global time after revceiving a valid Follow Up message.
 * @param[inout]  port: port pointer.
 * @param[in]     rawData: raw data pointer.
 * @param[in]     isSyncToGatewayBitSet: specifying if the synctogateway bit is set by the sub-tlv message.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-85860
 */
ETHTSYN_LOCAL void EthTSyn_ValidFollowUpMessageSetGlobalTime(
    const EthTSyn_PortType* port,
    const uint8*            rawData,
    const boolean*          isSyncToGatewayBitSet)
{
    StbM_TimeBaseStatusType timeBaseStatus = 0u;

#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    ETHTSYN_UNUSED(isSyncToGatewayBitSet);
    if (port->IsSyncToGatewayBitSet)
    {
        EthTSyn_PortType portTemp;
        portTemp                       = *port;
        portTemp.IsSyncToGatewayBitSet = FALSE;
        (void)IStdLib_MemCpy((void*)port, &portTemp, sizeof(EthTSyn_PortType));
        timeBaseStatus = STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
    }
#else
    if (*isSyncToGatewayBitSet)
    {
        timeBaseStatus = STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY;
    }
#endif
#endif
    StbM_TimeTupleType syncTimetuple;
    syncTimetuple.timeBaseStatus                 = timeBaseStatus;
    EthTSyn_GlobalTimeDomainCfgType domainConfig = PORT_TOCFGDOMIN(port->DomainIndex);
    if (domainConfig.SynchronizedTimeBaseRef < ETHTSYN_SYNCHRONIZE_TIMEBASE_MAX_INDEX)
    {
#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON) || (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON) \
    || (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
        EthTSyn_PortConfigCfgType portConfig = PORT_TOCFGPORT(port->DomainIndex);
#endif
        const uint8* rawDataPtr = rawData;

        /* PRS_TS_00261 PRS_TS_00262 */
        /* SWS_EthTSyn_268*/
        float64                 rateRatio           = 1.0;
        RateDeviationStatusType rateDeviationStatus = RATE_NOT_AVAILABLE;
#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
        if (portConfig.PdelayConfig->RateRatioEnable && port->RateRatio.HasCaculatedNeighborRateRatio)
        {
            EthTSyn_PortType* portTemp                        = (EthTSyn_PortType*)port;
            portTemp->RateRatio.HasCaculatedNeighborRateRatio = FALSE;
            /* SWS_EthTSyn_00266 */
            const StbM_ProtocolParamType* protocolParamRuntime =
                EthTSyn_ConfigData->GlobalTimeDomain[port->DomainIndex].ProtocolParam;
            float64 rateRatioPdelayResponder =
                ((float64)protocolParamRuntime->cumulativeScaledRateOffset / (float64)ETHTSYN_RATE_RATIO_MULTIPLIER)
                + 1.0;
            float64 neighborRateratio                    = port->RateRatio.NeighborRateRatio;
            rateRatio                                    = rateRatioPdelayResponder + neighborRateratio - 1.0;
            rateDeviationStatus                          = RATE_OK;
            portTemp->RateRatio.RateRatioPdelayResponder = rateRatioPdelayResponder;
            /* PRS_TS_00263 */
            if (EthTSyn_ExpectedStatusGet(port->DomainIndex, STBM_TIMEBASE_STATUS_SYNC_TO_GATEWAY))
            {
                StbM_ProtocolParamType protocolParam;
                float64 cumulativeScaledRateOffset       = (rateRatio - 1.0) * (float64)ETHTSYN_RATE_RATIO_MULTIPLIER;
                protocolParam.cumulativeScaledRateOffset = (sint32)cumulativeScaledRateOffset;
                (void)StbM_SetBusProtocolParam(domainConfig.SynchronizedTimeBaseRef, &protocolParam);
            }
        }
#endif
        const StbM_UserDataType* userDataPtr = NULL_PTR;
        EthTSyn_TimeStampType    correction =
            EthTSyn_GetCorrectionField(&(rawDataPtr[ETHTSYN_CORRECTIONFIELD_BYTE_INDEX]));
        EthTSyn_TimeStampType tg = EthTSyn_timeadd(
            EthTSyn_timeadd(EthTSyn_DataToTime(&rawDataPtr[ETHTSYN_ORIGINAL_TIMESTAMP_BYTE_INDEX]), port->Pdelay),
            correction);
        EthTSyn_timeoptimal(&tg);

        syncTimetuple.globalTime.nanoseconds = tg.Nanoseconds;
        syncTimetuple.globalTime.seconds     = tg.Seconds;
        syncTimetuple.globalTime.secondsHi   = tg.SecondsHi;
        StbM_MeasurementType measureDataPtr;
        measureDataPtr.pathDelay = port->Pdelay.Nanoseconds;
        uint64 tv                = port->T1vlt.Nanoseconds
                    + (((uint64)port->T1vlt.Seconds + (((uint64)port->T1vlt.SecondsHi) << ETHTSYN_BIT_SHIFT_32))
                       * ETHTSYN_NANOSECOND_TO_SECOND);
        syncTimetuple.virtualLocalTime.nanosecondsHi = (uint32)(tv >> ETHTSYN_BIT_SHIFT_32);
        syncTimetuple.virtualLocalTime.nanosecondsLo = (uint32)tv;
#if ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF
        if (PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeSlave->RxSubTLVUserData)
        {
            userDataPtr = &port->UserData;
        }
#endif
        /* SWS_EthTSyn_00267*/
        float64 rateDeviationValueTemp = ((rateRatio - 1.0) * (float64)ETHTSYN_RATE_RATIO_MULTIPLIER);
        sint32  rateDeviationValue     = (sint32)rateDeviationValueTemp;
#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
        if (rateDeviationValueTemp <= ETHTSYN_MINIMAL_LIMIT_RATE_RATIO_VALUE_FLOAT)
        {
            rateDeviationValue  = ETHTSYN_MINIMAL_LIMIT_RATE_RATIO_VALUE_INT;
            rateDeviationStatus = RATE_EXCEEDED;
        }
        else if (rateDeviationValueTemp >= (float64)ETHTSYN_MAX_LIMIT_RATE_RATIO_VALUE)
        {
            rateDeviationValue  = ETHTSYN_MAX_LIMIT_RATE_RATIO_VALUE;
            rateDeviationStatus = RATE_EXCEEDED;
        }
        else
        {
            /* do nothing */
        }
#endif
        measureDataPtr.rateDeviation.rateDeviationValue  = rateDeviationValue;
        measureDataPtr.rateDeviation.rateDeviationStatus = rateDeviationStatus;

        (void)StbM_BusSetGlobalTime(domainConfig.SynchronizedTimeBaseRef, &syncTimetuple, userDataPtr, &measureDataPtr);
#if (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
        /* SWS_EthTSyn_00216 SWS_EthTSyn_00217 */
        if (portConfig.EnableTimeValidation)
        {
            StbM_EthTimeSlaveMeasurementType validationMeasureDataPtr;
            (void)IStdLib_MemSet(&validationMeasureDataPtr, 0, sizeof(StbM_EthTimeSlaveMeasurementType));
            validationMeasureDataPtr.sequenceId                         = port->ReceivedSyncSequenceId;
            validationMeasureDataPtr.sourcePortId.clockIdentity         = port->PortIdentity.ClockIdentity;
            validationMeasureDataPtr.sourcePortId.portNumber            = port->PortIdentity.PortNumber;
            validationMeasureDataPtr.syncIngressTimestamp               = EthTSyn_GTToVT(port->T1vlt);
            validationMeasureDataPtr.preciseOriginTimestamp.seconds     = tg.Seconds;
            validationMeasureDataPtr.preciseOriginTimestamp.nanoseconds = tg.Nanoseconds;
            validationMeasureDataPtr.correctionField =
                (sint64)((sint64)EthTSyn_GTToVT(correction).nanosecondsHi << ETHTSYN_BIT_SHIFT_32
                         | EthTSyn_GTToVT(correction).nanosecondsLo);
            validationMeasureDataPtr.pDelay = (sint64)port->Pdelay.Nanoseconds;
            StbM_EthSetSlaveTimingData(domainConfig.SynchronizedTimeBaseRef, &validationMeasureDataPtr);
        }
#endif
    }
    else
    {
        /* Intentionally Empty */
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of converting the global timestamp to local timestamp.
 * @param[in]     timestamp: global timestamp to be converted.
 * @return        the timestamp after conversion.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83048
 */
ETHTSYN_LOCAL uint64 EthTSyn_TimestampToLocalTime(const EthTSyn_TimeStampType* timestamp)
{
    uint64 timestampSeconds = ((uint64)timestamp->SecondsHi << ETHTSYN_BIT_SHIFT_32) | (uint64)timestamp->Seconds;
    if (timestampSeconds > ETHTSYN_LOCAL_TIME_MAX_SECONDS)
    {
        timestampSeconds = ETHTSYN_LOCAL_TIME_MAX_SECONDS;
    }
    return (timestampSeconds * ETHTSYN_NANOSECOND_TO_SECOND) + (uint64)timestamp->Nanoseconds;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of caculating the neighbor rate ratio.
 * @param[in]     pdelayConfig: the configuration of pdelay.
 * @param[inout]  port: the port pointer.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-85861
 */
ETHTSYN_LOCAL void
    EthTSyn_CalculateNeighborRateRatio(const EthTSyn_PdelayConfigCfgType* pdelayConfig, EthTSyn_PortType* port)
{
    if (pdelayConfig->RateRatioEnable)
    {
        if (port->RateRatio.RateRatioMesuredCounter == pdelayConfig->RateRatioMeasurementCount)
        {
            uint64 t3n = EthTSyn_TimestampToLocalTime(&port->T3);
            uint64 t4n = EthTSyn_TimestampToLocalTime(&port->T4);
            uint64 t30 = EthTSyn_TimestampToLocalTime(&port->RateRatio.T30);
            uint64 t40 = EthTSyn_TimestampToLocalTime(&port->RateRatio.T40);

            uint64 t3d = (t3n > t30) ? (t3n - t30) : 0LLu;
            uint64 t4d = (t4n > t40) ? (t4n - t40) : 0LLu;

            if ((t3d > 0LLu) && (t4d > 0LLu))
            {
                port->RateRatio.NeighborRateRatio             = (float64)((float64)t3d / (float64)t4d);
                port->RateRatio.HasCaculatedNeighborRateRatio = TRUE;
            }
        }

        if ((port->RateRatio.RateRatioMesuredCounter == 0u)
            || (port->RateRatio.RateRatioMesuredCounter == pdelayConfig->RateRatioMeasurementCount))
        {
            port->RateRatio.T30 = port->T3;
            port->RateRatio.T40 = port->T4;
        }
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
#endif
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of handling the dynamic port role upon receiving a SYNC message.
 * @param[inout]  port: the port pointer.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-85862
 */
ETHTSYN_LOCAL void EthTSyn_DynamicPortRole(EthTSyn_PortType* port)
{
    EthTSyn_PortType* const* portRuntime = EthTSyn_ConfigData->RuntimeVariables;
    /* SWS_EthTSyn_00414 */
    port->PortRoleType = ETHTSYN_SLAVE;
    EthTSyn_InitPortStatus(port);
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    EthTSyn_InitPortICVRuntimeVariables(port);
#endif

    for (uint8 portIndex = 0u; portIndex < EthTSyn_ConfigData->DomainNum; portIndex++)
    {
        if (portIndex != port->DomainIndex)
        {
            if (portRuntime[portIndex]->PortRoleType == ETHTSYN_DYNAMIC)
            {
                /* all remaining ports which are set as DYNAMIC shall turn as Master */
                portRuntime[portIndex]->PortRoleType = ETHTSYN_MASTER;
                EthTSyn_InitPortStatus(portRuntime[portIndex]);
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
                EthTSyn_InitPortICVRuntimeVariables(port);
#endif
            }
            uint64 configHoldOverTime = PORT_TOCFGDOMIN(port->DomainIndex).PortRole.GlobalTimeMaster->HoldOverTime;

            if (configHoldOverTime != 0u)
            {
                /* SWS_EthTSyn_00415 SWS_EthTSyn_00416 */
                /* master port load the HoldOver timer */
                portRuntime[portIndex]->HoldOverTimer = configHoldOverTime;
                /* enable static or dynamic master port SYNC and FU messages sending */
                EthTSyn_SetTransmissionMode(port->CtrlId, ETHTSYN_TX_ON);
            }
        }
    }
    /* slave port load the max holdover time of all other masters*/
    port->HoldOverTimer = ETHTSYN_MAX_HOLDOVER_TIME;
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"

#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h"
/**
 * @brief         the implementation of handling the holdover timer which determins the maxmum backup time upon
 *                the loss of SYNC message.
 * @param[inout]  port: the port pointer.
 * @synchronous   TRUE
 * @reentrant     FALSE
 * @trace         CPD-83051
 */
ETHTSYN_LOCAL void EthTSyn_HoldOverTimerHandle(EthTSyn_PortType* port)
{
    const EthTSyn_GlobalTimeDomainCfgType* domainCfg  = &PORT_TOCFGDOMIN(port->DomainIndex);
    EthTSyn_PortConfigCfgType              portConfig = PORT_TOCFGPORT(port->DomainIndex);

    uint64 configHoldOverTime = 0u;

    if (port->PortRoleType == ETHTSYN_MASTER)
    {
        configHoldOverTime = domainCfg->PortRole.GlobalTimeMaster->HoldOverTime;
    }

    if (port->HoldOverTimer != 0u)
    {
        port->HoldOverTimer--;
    }

    /* SWS_EthTSyn_00416 */
    /* HoldOverTimer expired,ports which are set to TIME_MASTER (dynamically or statically) shall stop transmission
       of Sync and Follow_Up messages.*/
    if ((port->HoldOverTimer == 0u) && (port->PortRoleType == ETHTSYN_MASTER) && (configHoldOverTime != 0u))
    {
        EthTSyn_SetTransmissionMode(port->CtrlId, ETHTSYN_TX_OFF);
    }
    /* SWS_EthTSyn_00415 */
    /* HoldOverTimer expired,ports which are dynamically set to either Master or Slave shall turn back to DYNAMIC. */
    if (((port->HoldOverTimer == 0u) && (portConfig.GlobalTimePortRole == ETHTSYN_DYNAMIC))
        && (((port->PortRoleType == ETHTSYN_MASTER) && (configHoldOverTime != 0u))
            || ((port->PortRoleType == ETHTSYN_SLAVE))))
    {
        port->PortRoleType = ETHTSYN_DYNAMIC;
    }
}
#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"
