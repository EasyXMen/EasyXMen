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
 **  @file               : Com_Internal.h
 **  @author             : shengnan.sun
 **  @date               : 2024/09/01
 **  @vendor             : isoft
 **  @description        : Com internal header for internal API declarations
 **
 ***********************************************************************************************************************/
#ifndef COM_INTERNAL_H_
#define COM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "Com.h"
#include "PduR_Com.h"
#if (STD_ON == COM_MULTIPLE_PARTITION_USED) && (STD_ON == COM_DEV_ERROR_DETECT)
#include "Os.h"
#endif
#include "IStdLib.h"
#include "SchM_Com.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/* the Private Macro Definition */
#define COM_UNUSED_UINT8     0xFFu
#define COM_UNUSED_UINT16    0xFFFFu
#define COM_UNUSED_UINT32    0xFFFFFFFFu
#define COM_UNUSED_UINT64    0xFFFFFFFFFFFFFFFFULL
#define COM_NOUSED(paramter) (void)(paramter)
/* Com unused Type */
#define COM_UNUSED_RXIPDUCOUNTERID (Com_RxIPduCounterIdType)(-1)
#define COM_UNUSED_RXIPDUGROUPID   (Com_RxIpduGroupIdType)(-1)
#define COM_UNUSED_TXIPDUGROUPID   (Com_TxIpduGroupIdType)(-1)
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
#define COM_UNUSED_RXSIGTIMEOUTID (Com_RxSigTimeoutIdType)(-1)
#endif
#define COM_UNUSED_RXSIGGRPTIMEOUTID          (Com_RxSigGrpTimeoutIdType)(-1)
#define COM_UNUSED_GWSOURCEDESCRIPTIONID      (Com_GwSourceDescriptionIdType)(-1)
#define COM_UNUSED_RXSIGNALID                 (Com_RxSignalIdType)(-1)
#define COM_UNUSED_RXSIGNALGROUPID            (Com_RxSignalGroupIdType)(-1)
#define COM_UNUSED_GWDESTINATIONDESCRIPTIONID (Com_GwDestinationDescriptionIdType)(-1)
#define COM_UNUSED_TXSIGNALID                 (Com_TxSignalIdType)(-1)
#define COM_UNUSED_TXSIGNALGROUPID            (Com_TxSignalGroupIdType)(-1)
#define COM_UNUSED_TXIPDUCOUNTERID            (Com_TxIPduCounterIdType)(-1)
#define COM_UNUSED_RXSIGNALINVALIDID          (Com_RxSignalInvalidIdType)(-1)
#define COM_UNUSED_RXSIGNALFILTERID           (Com_RxSignalFilterIdType)(-1)
#define COM_UNUSED_SIGNALPOSITION             (Com_SignalPositionType)(-1)
#define COM_UNUSED_TXSIGNALINVALIDID          (Com_TxSignalInvalidIdType)(-1)
#define COM_UNUSED_TXSIGNALFILTERID           (Com_TxSignalFilterIdType)(-1)
#define COM_UNUSED_TXSIGERRANDNOTIFYID        (Com_TxSigErrAndNotifyIdType)(-1)
#define COM_UNUSED_TXSIGNALTIMEOUTID          (Com_TxSignalTimeoutIdType)(-1)
#define COM_UNUSED_RXGROUPSIGNALID            (Com_RxGroupSignalIdType)(-1)
#define COM_UNUSED_TXGROUPSIGNALID            (Com_TxGroupSignalIdType)(-1)
#define COM_UNUSED_TXSIGGRPERRANDNOTIFYID     (Com_TxSigGrpErrAndNotifyIdType)(-1)
#define COM_UNUSED_TXSIGGRPTIMEOUTID          (Com_TxSigGrpTimeoutIdType)(-1)
#define COM_UNUSED_RXGRPSIGFILTERID           (Com_RxGrpSigFilterIdType)(-1)
#define COM_UNUSED_TXGRPSIGFILTERID           (Com_TxGrpSigFilterIdType)(-1)
#define COM_UNUSED_DESTSIGNALFILTERID         (Com_DestSignalFilterIdType)(-1)
#define COM_UNUSED_COM_RXIPDUBUFID            (Com_RxIpduBufIdType)(-1)
#define COM_UNUSED_COM_TXIPDUBUFID            (Com_TxIpduBufIdType)(-1)
#define COM_UNUSED_COM_SIGNALGROUPARRYID      (Com_SignalGroupArryIdType)(-1)

/* RxIpduTimeout status */
#define COM_RX_SIGNAL_DM       1u
#define COM_RX_SIGNAL_GROUP_DM 2u
#define COM_RX_IPDU_DM         4u
#define COM_RX_IPDU_DM_ONLY    8u

/* the initState for RxIPdu */
#define COM_RX_ACTIVE_EN     0x1u
#define COM_RX_DM_EN         0x2u
#define COM_RX_RECEIVING_EN  0x4u
#define COM_RX_INDICATION_EN 0x8u

#define COM_RX_ACTIVE_DIS     0xFEu
#define COM_RX_DM_DIS         0xFDu
#define COM_RX_RECEIVING_DIS  0xFBu
#define COM_RX_INDICATION_DIS 0xF7u

/* the initState for TxIPdu */
#define COM_TX_ACTIVE_EN      0x1u
#define COM_TX_CONFIRM_EN     0x2u
#define COM_TX_TMS_EN         0x4u
#define COM_TX_TRANSMITING_EN 0x8u
#define COM_TX_DELAY_EN       0x10u

#define COM_TX_ACTIVE_DIS      0xFEu
#define COM_TX_CONFIRM_DIS     0xFDu
#define COM_TX_TMS_DIS         0xFBu
#define COM_TX_TRANSMITING_DIS 0xF7u
#define COM_TX_DELAY_DIS       0xEFu

#define COM_NTIMESTRANSMITNEED_EN 0x1u
#define COM_WITHOUTREPETITION_EN  0x2u

#define COM_NTIMESTRANSMITNEED_DIS 0xFEu
#define COM_WITHOUTREPETITION_DIS  0xFDu

#define COM_TX_SIGNAL_NOTIFICATION_NONE      0x0u
#define COM_TX_SIGNAL_NOTIFICATION_EN        0x1u
#define COM_TX_SIGNAL_ERROR_NOTIFICATION_EN  0x2u
#define COM_TX_SIGNAL_ERROR_AND_NOTIFICATION 0x3u

#if !defined(COM_LOCAL)
#define COM_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

#define COM_INLINE static inline
/* ============================================ external data definitions =========================================== */
extern const Com_ConfigType* Com_ConfigStd;

#if (COM_MAX_RXIPDU_NUMBER > 0u)
extern const Com_RxIPduType* Com_CfgRxIPduPtr;
#endif
#if (COM_MAX_TXIPDU_NUMBER > 0u)
extern const Com_TxIPduType* Com_CfgTxIPduPtr;
#endif
extern const Com_RxSignalType*      Com_CfgRxSignalPtr;
extern const Com_TxSignalType*      Com_CfgTxSignalPtr;
extern const Com_RxSignalGroupType* Com_CfgRxSignalGroupPtr;
extern const Com_TxSignalGroupType* Com_CfgTxSignalGroupPtr;
extern const Com_RxGroupSignalType* Com_CfgRxGroupSignalPtr;
extern const Com_TxGroupSignalType* Com_CfgTxGroupSignalPtr;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
extern const uint16* Com_CfgMainFuncRxPartitionRangePtr;
extern const uint16* Com_CfgMainFuncTxPartitionRangePtr;
#endif
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
extern const Com_GwMappingConfigType* Com_CfgGwMappingConfigPtr;
#endif
#if (STD_ON == COM_SIGNAL_INITVALUE_SUPPORT)
extern const Com_SignalInitValueType* Com_CfgSignalInitValuePtr;
#endif
#if (STD_ON == COM_TXMODE_AND_FILTER_SUPPORT)
extern const Com_TxModeAndSignalFilterPtrType* Com_CfgTxModeAndSignalFilterPtr;
#endif
#if (STD_ON == COM_SIGNAL_TIMEOUT_SUPPORT)
extern const Com_SignalTimeoutType* Com_CfgSignalTimeoutPtr;
#endif
extern Com_SignalIdType Com_CfgTxSignalNum;
extern Com_SignalIdType Com_CfgRxSignalNum;
extern PduIdType        Com_CfgTxPduNum;
extern PduIdType        Com_CfgRxPduNum;
/* ========================================== external function definitions ========================================= */
/**
 * @brief         set RxIpdu deadline monitor flag
 * @param[in]     Flag:RxIpdu deadline monitor flag
 * @param[in]     Val:value
 * @retval        var
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69031
 */
COM_INLINE uint8 Com_SetRxDMFlag(uint8 Flag, uint8 Val)
{
    uint8 var = Flag | Val;
    return var;
}
/**
 * @brief         get start Ipdu range from mainfunctionTx
 * @param[in]     MainFunctionId:MainfunctionId from mainfunctionTx
 * @retval        ipduId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69032
 */
COM_INLINE PduIdType Com_GetStartOfMainFunctionTx(Com_MainFunctionIdType MainFunctionId)
{
    PduIdType ipduId = ((MainFunctionId) == 0u) ? 0u : Com_ConfigStd->MainFunctionTxRangePtr[(MainFunctionId)-1u];
    return ipduId;
}
/**
 * @brief         get end Ipdu range from mainfunctionTx
 * @param[in]     MainFunctionId:MainfunctionId from mainfunctionTx
 * @retval        ipduId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69034
 */
COM_INLINE PduIdType Com_GetEndOfMainFunctionTx(Com_MainFunctionIdType MainFunctionId)
{
    return Com_ConfigStd->MainFunctionTxRangePtr[(MainFunctionId)];
}
/**
 * @brief         get start Ipdu range from mainfunctionRx
 * @param[in]     MainFunctionId:MainfunctionId from mainfunctionRx
 * @retval        ipduId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69035
 */
COM_INLINE PduIdType Com_GetStartOfMainFunctionRx(Com_MainFunctionIdType MainFunctionId)
{
    PduIdType ipduId = ((MainFunctionId) == 0u) ? 0u : Com_ConfigStd->MainFunctionRxRangePtr[(MainFunctionId)-1u];
    return ipduId;
}
/**
 * @brief         get end Ipdu range from mainfunctionRx
 * @param[in]     MainFunctionId:MainfunctionId from mainfunctionRx
 * @retval        ipduId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69036
 */
COM_INLINE PduIdType Com_GetEndOfMainFunctionRx(Com_MainFunctionIdType MainFunctionId)
{
    return Com_ConfigStd->MainFunctionRxRangePtr[(MainFunctionId)];
}
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
/**
 * @brief         get partitionId from mainfunctionRx
 * @param[in]     MainFunctionId:MainfunctionId from mainfunctionRx
 * @retval        partitionId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69037
 */
COM_INLINE uint16 Com_GetRxMFPartitionId(Com_MainFunctionIdType MainFunctionId)
{
    return (Com_CfgMainFuncRxPartitionRangePtr[(MainFunctionId)]);
}
/**
 * @brief         get partitionId from mainfunctionTx
 * @param[in]     MainFunctionId:MainfunctionId from mainfunctionRx
 * @retval        partitionId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69038
 */
COM_INLINE uint16 Com_GetTxMFPartitionId(Com_MainFunctionIdType MainFunctionId)
{
    return (Com_CfgMainFuncTxPartitionRangePtr[(MainFunctionId)]);
}
#endif
/**
 * @brief         get mainfunctionId from RxSignal
 * @param[in]     SignalId:RxSignal index
 * @retval        mainfunctionId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69039
 */
#if (COM_MAX_RXSIGNAL_NUMBER > 0u) || (COM_MAX_RXSIGNALGROUP_NUMBER > 0u)
COM_INLINE Com_MainFunctionIdType Com_GetRxSigMainfunctionId(Com_RxSignalIdType SignalId)
{
    return (Com_CfgRxSignalPtr[(SignalId)].RxMainfunctionId);
}
#endif
/**
 * @brief         get mainfunctionId from TxSignal
 * @param[in]     SignalId:TxSignal index
 * @retval        mainfunctionId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69040
 */
#if (COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)
COM_INLINE Com_MainFunctionIdType Com_GetTxSigMainfunctionId(Com_TxSignalIdType SignalId)
{
    return (Com_CfgTxSignalPtr[(SignalId)].TxMainfunctionId);
}
#endif
/**
 * @brief         get mainfunctionId from RxSignalGroup
 * @param[in]     SignaGroupId:SignaGroupId from RxSignalGroup
 * @retval        mainfunctionId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69042
 */
#if (COM_MAX_RXSIGNALGROUP_NUMBER > 0u)
COM_INLINE Com_MainFunctionIdType Com_GetRxSigGrpMainfunctionId(Com_RxSignalGroupIdType SignaGroupId)
{
    return (Com_CfgRxSignalGroupPtr[(SignaGroupId)].RxMainfunctionId);
}
#endif
/**
 * @brief         get mainfunctionId from TxSignalGroup
 * @param[in]     SignaGroupId:SignaGroupId from TxSignalGroup
 * @retval        mainfunctionId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69043
 */
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
COM_INLINE Com_MainFunctionIdType Com_GetTxSigGrpMainfunctionId(Com_TxSignalGroupIdType SignaGroupId)
{
    return (Com_CfgTxSignalGroupPtr[(SignaGroupId)].TxMainfunctionId);
}
#endif
#if (COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
/**
 * @brief         get TxSignalGroupId from TxGroupSignal
 * @param[in]     GroupSignalId:GroupSignalId from TxGroupSignal
 * @retval        TxSignalGroupId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69045
 */
COM_INLINE Com_RxSignalGroupIdType Com_GetTxSignalGroupId(Com_TxGroupSignalIdType GroupSignalId)
{
    return Com_CfgTxGroupSignalPtr[(GroupSignalId)].ComSignalGroupRef;
}
#endif
/**
 * @brief         get mainfunctionId from TxGroupSignal
 * @param[in]     GroupSignalId:GroupSignalId from TxGroupSignal
 * @retval        mainfunctionId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69050
 */
COM_INLINE Com_MainFunctionIdType Com_GetTxGrpSigMainfunctionId(Com_TxGroupSignalIdType GroupSignalId)
{
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
    return (Com_GetTxSigGrpMainfunctionId(Com_GetTxSignalGroupId(GroupSignalId)));
#else
    return COM_UNUSED_UINT8;
#endif
}
#endif
#if (COM_MAX_RXSIGNAL_NUMBER > 0u) || (COM_MAX_RXGROUPSIGNAL_NUMBER > 0u)
/**
 * @brief         get RxSignalGroupId from RxGroupSignal
 * @param[in]     GroupSignalId:GroupSignalId from RxGroupSignal
 * @retval        RxSignalGroupId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69054
 */
COM_INLINE Com_RxSignalGroupIdType Com_GetRxSignalGroupId(Com_RxGroupSignalIdType GroupSignalId)
{
    return Com_CfgRxGroupSignalPtr[(GroupSignalId)].ComSignalGroupRef;
}
/**
 * @brief         get mainfunctionId from RxGroupSignal
 * @param[in]     GroupSignalId:GroupSignalId from RxGroupSignal
 * @retval        mainfunctionId
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-69057
 */
COM_INLINE Com_MainFunctionIdType Com_GetRxGrpSigMainfunctionId(Com_RxGroupSignalIdType GroupSignalId)
{
#if (COM_MAX_RXSIGNALGROUP_NUMBER > 0u)
    return (Com_GetRxSigGrpMainfunctionId(Com_GetRxSignalGroupId(GroupSignalId)));
#else
    return COM_UNUSED_UINT8;
#endif
}
#endif
#if (COM_MAX_RXGROUPSIGNAL_NUMBER > 0u)
/**
 * @brief         the rx signal/group Signal/source signal have update bit
 * @param[in]     UpdateBitPosition: Update BitPosition
 * @param[in]     SigUbOffset: signal/group Signal/source signal UpdateBit Offset
 * @param[in]     UpdateBitOffset: signal/group Signal/source signal UpdateBit Offset configured
 * @retval        ubIsAccess
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace
 */
COM_INLINE boolean
    Com_RxUpdateBitIsTure(Com_SignalPositionType UpdateBitPosition, uint8 SigUbOffset, uint8 UpdateBitOffset)
{
    boolean ubIsAccess = TRUE;
    if ((COM_UNUSED_SIGNALPOSITION != UpdateBitPosition) && (UpdateBitOffset != (SigUbOffset & UpdateBitOffset)))
    {
        ubIsAccess = FALSE;
    }
    return ubIsAccess;
}
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/*define in Com_PBcfg.c*/
#if (COM_MAX_RXIPDU_NUMBER > 0u)
extern Com_RxIPduRunTimeStateType* const Com_RxIPduRunTimeState[COM_MAX_NUMBER_OF_MAIN_FUNCTION_RX];
#endif

#if (COM_MAX_TXIPDU_NUMBER > 0u)
extern Com_TxIPduRunTimeStateType* const Com_TxIPduRunTimeState[COM_MAX_NUMBER_OF_MAIN_FUNCTION_TX];
#endif

#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
extern Com_TxSignalGroupRunTimeStateType Com_TxSignalGroupState[COM_MAX_TXSIGNALGROUP_NUMBER];
#endif

#if (COM_MAX_TXIPDUBUFF_SIZE > 0u)
extern uint8* Com_TxIPduRuntimeBuff[COM_MAX_NUMBER_OF_MAIN_FUNCTION_TX];
#endif

#if (COM_MAX_RXIPDUBUFF_SIZE > 0u)
extern uint8* Com_RxIPduRuntimeBuff[COM_MAX_NUMBER_OF_MAIN_FUNCTION_RX];
#endif

#if (COM_MAX_SIGNAL_BOOLBUFF_SIZE > 0u)
extern boolean* Com_SignalBoolRuntimeBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_SIGNAL_8BITBUFF_SIZE > 0u)
extern uint8* Com_Signal8BitRuntimeBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_SIGNAL_16BITBUFF_SIZE > 0u)
extern uint16* Com_SignalBit16RuntimeBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_SIGNAL_32BITBUFF_SIZE > 0u)
extern uint32* Com_Signal32BitRuntimeBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_SIGNAL_64BITBUFF_SIZE > 0u)
extern uint64* Com_Signal64BitRuntimeBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_RXGROUPSIGNAL_BOOLBUFF_SIZE > 0u)
extern boolean* Com_RxGroupSignalBoolShadowBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE > 0u)
extern uint8* Com_RxGroupSignal8BitShadowBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_RXGROUPSIGNAL_16BITBUFF_SIZE > 0u)
extern uint16* Com_RxGroupSignalBit16ShadowBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_RXGROUPSIGNAL_32BITBUFF_SIZE > 0u)
extern uint32* Com_RxGroupSignal32BitShadowBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_RXGROUPSIGNAL_64BITBUFF_SIZE > 0u)
extern uint64* Com_RxGroupSignal64BitShadowBuff[COM_MAX_PARTITION_NUMBER];
#endif

#if (COM_MAX_GWSIGNAL_BOOLBUFF_SIZE > 0u)
extern boolean Com_GWSignalBoolBuff[COM_MAX_GWSIGNAL_BOOLBUFF_SIZE];
#endif

#if (COM_MAX_GWSIGNAL_8BITBUFF_SIZE > 0u)
extern uint8 Com_GWSignal8BitBuff[COM_MAX_GWSIGNAL_8BITBUFF_SIZE];
#endif

#if (COM_MAX_GWSIGNAL_16BITBUFF_SIZE > 0u)
extern uint16 Com_GWSignalBit16Buff[COM_MAX_GWSIGNAL_16BITBUFF_SIZE];
#endif

#if (COM_MAX_GWSIGNAL_32BITBUFF_SIZE > 0u)
extern uint32 Com_GWSignal32BitBuff[COM_MAX_GWSIGNAL_32BITBUFF_SIZE];
#endif

#if (COM_MAX_GWSIGNAL_64BITBUFF_SIZE > 0u)
extern uint64 Com_GWSignal64BitBuff[COM_MAX_GWSIGNAL_64BITBUFF_SIZE];
#endif

#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER)
extern boolean* Com_TxSignalTMCRunTime[COM_MAX_NUMBER_OF_MAIN_FUNCTION_TX];
#endif

#if (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
extern boolean* Com_TxGroupSignalTMCBuffer[COM_MAX_NUMBER_OF_MAIN_FUNCTION_TX];
#endif

#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
extern uint32 Com_OneEveryNcnt[COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER];
#endif

#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
extern boolean Com_MaskNewDifferMaskOldTimeOut[COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER];
#endif

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
extern uint16 Com_RxSignalTimeOutCnt[COM_MAX_RXSIG_TIMEOUT_NUMBER];
#endif

#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
extern uint16 Com_RxSignalGroupTimeOutCnt[COM_MAX_RXSIGGRP_TIMEOUT_NUMBER];
#endif
/* ========================================= external function declarations ========================================= */
/* PRQA S 5015 ++ */ /* VL_Com_UntrustedMethod */
/**
 * @brief         Get partitionId from partitioIdList
 * @param[in]     osApplicationId:Os application index.
 * @retval        partitionId:Partition index.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66520
 */
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
extern uint16 Com_PartitonIdFromList(ApplicationType osApplicationId);
#endif
/**
 * @brief         Init signal/group signal buffer
 * @param[in]     SignalType:SignalType from signal/groupSignal
 * @param[in]     SignalInitValueId:SiganlInitValue from signal/groupSignal
 * @param[in]     ComSignalLength:Length of signal/groupSignal
 * @param[in]     PartitionId:Partition index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66524
 */
#if ((0u < COM_MAX_IPDUGROUP_NUMBER) || (0u < COM_MAX_RXIPDU_NUMBER))
extern void Com_InitSignalBuffer(
    Com_SignalType SignalType,
    uint16         SignalInitValueId,
    uint16         ComSignalLength,
    uint16         PartitionId);
#endif
/**
 * @brief         Unpack all byte value(group signal/signal cover,except MSB byte).
 * @param[in]     SignalType:SignalType from signal/groupSignal
 * @param[in]     SignalConfigType:Signal config type
 * @param[in]     SignalPtr:Signal pointer
 * @param[in]     IPduBufferPtr:Ipdu buffer pointer
 * @retval        signalValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66525
 */
#if (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    || ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0u))
extern uint64 Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
    Com_SignalType SignalType,
#endif
    Com_SignalConfigType SignalConfigType,
    const void*          SignalPtr,
    const uint8*         IPduBufferPtr);
#endif
/**
 * @brief         Called by Com_TxSignalPack,Com_TxGroupSignalPack
 *                pack signal which signal type is not boolean,COM_UINT8_N,
 *                COM_UINT8_DYN,and the signal value bits cover 2-9 bytes
 * @param[in]     SignalConfigType:Signal config type
 * @param[in]     TxSignalPtr:TxSignal pointer
 * @param[in]     TxValue:Tx value
 * @param[in]     IPduBufferPtr:Ipdu buffer pointer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66526
 */
#if ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) || (0u < COM_MAX_TXSIGNAL_NUMBER) || (0u < COM_MAX_GWMAPPING_NUMBER))
extern void Com_TxSignalPackHandle(
    Com_SignalConfigType SignalConfigType,
    const void*          TxSignalPtr,
    uint64               TxValue,
    uint8*               IPduBufferPtr);
#endif
/**
 * @brief         Called by Com_TxSignalTMCCalculate,
 *                Com_DestSignalTMCCalculate,Com_RxSignalFilter.
 *                when FilterAlgorithm is NEW_IS_WITHIN or NEW_IS_WITHOUT,calculate the signal's TMC.
 * @param[in]     SignalType:SignalType from signal/groupSignal
 * @param[in]     SignalValue:Signal value
 * @param[in]     ComFilterMax:Filter max value
 * @param[in]     ComFilterMin:Filter min value
 * @param[in]     Within:NEW_IS_WITHIN or NEW_IS_WITHOUT
 * @retval        result
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66527
 */
#if (COM_MAX_TXNEWISWITHIN_NUMBER > 0u) || (COM_MAX_RXNEWISWITHIN_NUMBER > 0u) || (COM_MAX_RXNEWISOUTSIDE_NUMBER > 0u) \
    || (COM_MAX_TXNEWISOUTSIDE_NUMBER > 0U)
extern boolean Com_NewIsWInWOtFilterCalculate(
    Com_SignalType    SignalType,
    uint64            SignalValue,
    Com_FilterMaxType ComFilterMax,
    Com_FilterMinType ComFilterMin,
    boolean           Within);
#endif
/**
 * @brief         Called by Com_TxSignalTMHandle,Com_TxIpduTMSHandle.
 *                calculate the tx ipdu TMS(signal/Group signal/Dest signal)
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66528
 */
#if (                                                                                                       \
    (0u < COM_MAX_TXSIGNAL_NUMBER) || ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (0u < COM_MAX_TXIPDU_NUMBER)) \
    || (0u < COM_MAX_GWMAPPING_NUMBER))                                                                     \
    && (STD_ON == COM_TMS_ENABLE)
extern boolean Com_TxIpduTMSCalculate(const Com_TxIPduType* TxIpduPtr, Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         Called by Com_SendSignalGroupArray,Com_SendSignalGroup,
 *                Com_GwUint8NDYNSignal,Com_GwNotArraySignal.
 *                calculate the TMS,store new Tx mode,get rptNum value for
 *                DIRECT/MIXED tx mode,if TMS changed will reset periodic mode counter
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     RptNum:Repetition number
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66529
 */
#if (0u < COM_MAX_TXIPDU_NUMBER) || (0u < COM_MAX_GWMAPPING_NUMBER)
extern void
    Com_TxIpduTMSHandle(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, uint16* RptNum);
#endif
/**
 * @brief         Set tx signal update bit to 1
 * @param[in]     IPduBufferPtr:Ipdu buffer pointer
 * @param[in]     UpdateBitPosition:UpdateBitPosition
 * @param[in]     UpdateBitMask:UpdateBitMask
 * @param[in]     TxIpduRuntimeBuffPtr:TxIpdu runtime buffer pointer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66530
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)                                                                      \
    && ((STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) \
        || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE))
extern void Com_SetSignalUpdateBit(
    Com_TxIpduBufIdType    IpduBufferIndex,
    Com_SignalPositionType UpdateBitPosition,
    uint8                  UpdateBitMask,
    uint8*                 TxIpduRuntimeBuffPtr);
#endif
/**
 * @brief         When FilterAlgorithm is MASKED_NEW_DIFFERS_MASKED_OLD,calculate the signal's TMC
 * @param[in]     SignalType:SignalType from signal/groupSignal
 * @param[in]     SignalValue:Signal value
 * @param[in]     SignalBufferId:Signal buffer index
 * @param[in]     ComFilterMask:ComFilterMask
 * @param[in]     PartitionId:Partition index
 * @retval        filterOk
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66531
 */
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
extern boolean Com_MaskedNewDiffersMaskedOldFilterCalculate(
    Com_SignalType     SignalType,
    uint64             SignalValue,
    uint16             SignalBufferId,
    Com_FilterMaskType ComFilterMask,
    uint16             PartitionId);
#endif
/**
 * @brief         Substitute signal/group signal buffer
 * @param[in]     SignalType:SignalType from signal/groupSignal
 * @param[in]     SignalInitValueId:SiganlInitValue from signal/groupSignal
 * @param[in]     ComSignalLength:Length of signal/groupSignal
 * @param[in]     SignalSubstituteValueId:Signal index from substituteValue buffer
 * @param[in]     PartitionId:Partition index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66532
 */
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
extern void Com_SubstituteSignalBuffer(
    Com_SignalType SignalType,
    uint16         SignalInitValueId,
    uint16         ComSignalLength,
    uint16         SignalSubstituteValueId,
    uint16         PartitionId);
#endif
/**
 * @brief         Init Runtime TMC of all tx signals and tx group siganls
 *                and GW dest signals,which the comfilter is not NULL
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66521
 */
extern void Com_TxSignalTMCStateInit(Com_MainFunctionIdType TxMainfunctionId);
/**
 * @brief         Set all tx signal group state to not need to trigger transmit
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66522
 */
#if (0u < COM_MAX_TXSIGNALGROUP_NUMBER)
extern void Com_TxSignalGroupTriggerFlagInit(void);
#endif
/**
 * @brief         Used to handle all Tx Ipdu state change or not.
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @param[in]     IpduGroupId:IpduGroupId
 * @param[in]     Initialize:Initialize
 * @param[in]     IsEnable:Enable or not
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66531
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_TXIPDUGROUP_NUMBER > 0u)
extern void Com_TxIpduControl(
    Com_MainFunctionIdType TxMainfunctionId,
    Com_IpduGroupIdType    IpduGroupId,
    boolean                Initialize,
    boolean                IsEnable);
#endif
/**
 * @brief         Send dynamic Tx group signal handle.
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxGroupSignalId:TxGroupSignal index
 * @param[in]     SignalDataPtr:Pointer from signal data
 * @param[in]     DynLength:Length of dynamic signal/groupSignal
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66448
 */
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
extern uint8 Com_SendTxDynGroupSignalHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    Com_SignalIdType            TxGroupSignalId,
    const void*                 SignalDataPtr,
    uint16                      DynLength);
#endif
/**
 * @brief         Tx notification for all signals/signal groups of the Tx Pdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66449
 */
#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
extern void Com_ConfirmationProcess(const Com_TxIPduType* TxIpduPtr);
#endif
/**
 * @brief         Reset the counter of tx Ipdu MDT.
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66479
 */
#if (STD_ON == COM_MDT_ENABLE)
extern void Com_ResetTxIpduMDT(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr);
#endif
/**
 * @brief         Receive Dynamic Group Signal handle
 * @param[in]     SignalId:Signal index
 * @param[in]     SignalDataPtr:Pointer from signal data
 * @param[in]     Length:Length:Length
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66513
 */
#if (                                                                            \
    ((COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (COM_MAX_RXGROUPSIGNAL_NUMBER > 0u)) \
    && (0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE))                               \
    && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (0u < COM_MAX_RXIPDU_NUMBER)
extern uint8 Com_ReceiveDynGroupSignalHandle(Com_SignalIdType SignalId, void* SignalDataPtr, uint16* Length);
#endif
/**
 * @brief         Calculate dest signal/group signal/description signal TMC
 * @param[in]     SignalType:SignalType from signal/groupSignal
 * @param[in]     FilterType:FilterType
 * @param[in]     FilterIndex:Index from filter type
 * @param[in]     OldSignalValue:Old signal value
 * @param[in]     NewSignalValue:New signal value
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66539
 */
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER)
extern boolean Com_DestSignalTMCCalculate(
    Com_SignalType          SignalType,
    Com_FilterAlgorithmType FilterType,
    Com_SignalIdType        FilterIndex,
#if (COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint64 OldSignalValue,
#endif
    uint64 NewSignalValue);
#endif
/**
 * @brief         Send SignalGroup(Array) handle
 * @param[in]     SignalGroupId:SignalGroup index
 * @param[in]     SignalGroupArrayPtr:Pointer of signalGroupArray
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66447
 */
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (0u < COM_MAX_TXIPDU_NUMBER)
extern uint8 Com_SendSignalGroupArrayHandle(Com_SignalGroupIdType SignalGroupId, const uint8* SignalGroupArrayPtr);
#endif
/**
 * @brief         Send Signal handle
 * @param[in]     SignalId:Signal index
 * @param[in]     SignalDataPtr:Pointer from signal data
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66450
 */
#if (((COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_MAX_TXIPDU_NUMBER))
extern uint8
    Com_SendSignalHandle(Com_SignalIdType SignalId, const void* SignalDataPtr, Com_MainFunctionIdType TxMainfunctionId);
/**
 * @brief         Send Dynamic Signal handle
 * @param[in]     SignalId:Signal index
 * @param[in]     SignalDataPtr:Pointer from signal data
 * @param[in]     Length:Length
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-67901
 */
extern uint8 Com_SendDynSignalHandle(
    Com_SignalIdType       SignalId,
    const void*            SignalDataPtr,
    uint16                 Length,
    Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         Send SignalGroup handle
 * @param[in]     SignalGroupId:SignalGroup index
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66451
 */
#if ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (0u < COM_MAX_TXIPDU_NUMBER))
extern uint8 Com_SendSignalGroupHandle(Com_SignalGroupIdType SignalGroupId, Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         Send SignalGroup handle
 * @param[in]     SignalId:Signal index
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66453
 */
#if (0u < COM_MAX_TXIPDU_NUMBER) && ((COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u))
extern uint8 Com_InvalidateSignalHandle(Com_SignalIdType SignalId);
#endif
/**
 * @brief         Send Invalidate SignalGroup handle
 * @param[in]     SignalGroupId:SignalGroup index
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66454
 */
#if (0u < COM_MAX_TXIPDU_NUMBER) && ((COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u))
extern uint8 Com_InvalidateSignalGroupHandle(Com_SignalGroupIdType SignalGroupId);
#endif
/**
 * @brief         Trigger IPDU send handle
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     IpduId:Ipdu index
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @retval        returnValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66456
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
extern Std_ReturnType Com_TriggerIPDUSendHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    PduIdType IpduId,
#endif
    Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         Trigger IPDU(with metadata) send handle
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     IpduId:Ipdu index
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @param[in]     MetaData:MetaData
 * @retval        returnValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66467
 */
#if (STD_ON == COM_METADATA_SUPPORT)
extern Std_ReturnType Com_TriggerIPDUSendWithMetaDataHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    PduIdType IpduId,
#endif
    Com_MainFunctionIdType TxMainfunctionId,
    const uint8*           MetaData);
#endif
/**
 * @brief         Switch Ipdu Transmit Mode Handle
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     Mode:TxMode
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66468
 */
#if (0u < COM_MAX_TXIPDU_NUMBER) && (STD_ON == COM_TMS_ENABLE)
extern void Com_SwitchIpduTxModeHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    boolean                     Mode);
#endif
/**
 * @brief         TxPdu MainFunction handle
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66476
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
extern void Com_MainFunctionTxHandle(Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         TxPdu Trigger Transmit handle
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxIpduId:TxIpdu index
 * @param[in]     PduInfoPtr:Pointer of Pdu Info
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @retval        returnValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66477
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
extern Std_ReturnType Com_TriggerTransmitHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
    PduIdType                   TxIpduId,
    PduInfoType*                PduInfoPtr,
    Com_MainFunctionIdType      TxMainfunctionId);
#endif
/**
 * @brief         TP TxPdu TxConfirmation handle
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @param[in]     result:Result
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66478
 */
#if ((0u < COM_MAX_TXIPDU_NUMBER) && (STD_ON == COM_TXTPPDU_SUPPORT))
extern void Com_TpTxConfirmationHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
    Com_MainFunctionIdType      TxMainfunctionId,
    Std_ReturnType              result);
/**
 * @brief         TP TxPdu copy pdu data handle
 * @param[in]     TxIpduId:TxIpdu index
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @param[in]     info:Value info
 * @param[in]     retry:Retry to copy
 * @param[in]     availableDataPtr:Pointer of available data
 * @retval        bufReq
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66864
 */
extern BufReq_ReturnType Com_CopyTxDataHandle(
    PduIdType              TxIpduId,
    Com_MainFunctionIdType TxMainfunctionId,
    const PduInfoType*     info,
    const RetryInfoType*   retry,
    PduLengthType*         availableDataPtr);
#endif
/**
 * @brief         Receive group signal handle
 * @param[in]     SignalId:Signal index
 * @param[in]     SignalDataPtr:Pointer from signal data
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66517
 */
#if (0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER)
extern uint8 Com_ReceiveGroupSignalHandle(Com_SignalIdType SignalId, void* SignalDataPtr);
#endif
/**
 * @brief         Receive signal handle
 * @param[in]     SignalId:Signal index
 * @param[in]     SignalDataPtr:Pointer from signal data
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66518
 */
#if (0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER)
extern uint8 Com_ReceiveSignalHandle(Com_SignalIdType SignalId, void* SignalDataPtr);
#endif
/**
 * @brief         Receive all group signal of the SignalGroup
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     PartitionId:Partition index
 * @param[in]     SignalGroupId:SignalGroup index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66516
 */
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER))
extern void Com_ReceiveSignalGroupHandle(
    const Com_RxIPduType*       RxIpduPtr,
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    uint16                      PartitionId,
    Com_SignalGroupIdType       SignalGroupId);
#endif
/**
 * @brief         Receive SignalGroup(Array) data handle
 * @param[in]     SignalGroupId:SignalGroup index
 * @param[in]     SignalGroupArrayPtr:Pointer of signalGroupArray
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66510
 */
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) \
    && ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER))
extern uint8 Com_ReceiveSignalGroupArrayHandle(Com_SignalGroupIdType SignalGroupId, uint8* SignalGroupArrayPtr);
#endif
/**
 * @brief         Receive Dynamic Signal handle
 * @param[in]     SignalId:Signal index
 * @param[in]     SignalDataPtr:Pointer from signal data
 * @param[in]     Length:Length
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66515
 */
#if ((0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)) \
    && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (0u < COM_MAX_RXIPDU_NUMBER)
extern uint8 Com_ReceiveDynSignalHandle(Com_SignalIdType SignalId, void* SignalDataPtr, uint16* Length);
#endif
/**
 * @brief         If RxPdu RxIndication handle
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxIpduId:RxIpdu index
 * @param[in]     PduInfoPtr:Pointer of Pdu Info
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66511
 */
#if (0u < COM_MAX_RXIPDU_NUMBER)
extern void Com_RxIndicationHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
    PduIdType RxIpduId,
#endif
    const PduInfoType*     PduInfoPtr,
    Com_MainFunctionIdType RxMainfunctionId);
#endif
/**
 * @brief         TP RxPdu RxIndication handle
 * @param[in]     RxIpduId:RxIpdu index
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @param[in]     result:Result
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66509
 */
#if (STD_ON == COM_RXTPPDU_SUPPORT)
extern void Com_TpRxIndicationHandle(
    PduIdType                   RxIpduId,
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId,
    Std_ReturnType              result);
/**
 * @brief         TP Pdu copy receive data handle
 * @param[in]     RxIpduId:RxIpdu index
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @param[in]     info:Value info
 * @param[in]     bufferSizePtr:Pointer of buffer size
 * @retval        bufReq
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-67808
 */
extern BufReq_ReturnType Com_CopyRxDataHandle(
    PduIdType              RxIpduId,
    Com_MainFunctionIdType RxMainfunctionId,
    const PduInfoType*     info,
    PduLengthType*         bufferSizePtr);
/**
 * @brief         TP Pdu Start receive handle
 * @param[in]     RxIpduId:RxIpdu index
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @param[in]     info:Value info
 * @param[in]     TpSduLength:Length of TpSdu
 * @param[in]     bufferSizePtr:Pointer of buffer size
 * @retval        bufReq
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-67809
 */
extern BufReq_ReturnType Com_StartOfReceptionHandle(
    PduIdType              RxIpduId,
    Com_MainFunctionIdType RxMainfunctionId,
    const PduInfoType*     info,
    PduLengthType          TpSduLength,
    PduLengthType*         bufferSizePtr);
#endif
/**
 * @brief         RxPdu MainFunction handle
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66512
 */
#if (0u < COM_MAX_RXIPDU_NUMBER)
extern void Com_MainFunctionRxHandle(Com_MainFunctionIdType RxMainfunctionId);
#endif
#if COM_MAX_GWMAPPING_NUMBER > 0u
/**
 * @brief         Signal gateway handle
 * @param[in]     RouteSignalsMainFunctionId:MainfunctionId from RouteSignals
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66541
 */
extern void Com_MainFunctionRouteSignalsHandle(Com_MainFunctionIdType RouteSignalsMainFunctionId);

/**
 * @brief         Init every destination request flag in the Gateway
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66542
 */
extern void Com_GwInitDestinationRequest(void);
/**
 * @brief         Set every destination request flag in the Gateway
 * @param[in]     gwMappingId:Gateway mapping index
 * @param[in]     request:Request flag
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66543
 */
extern void Com_GwUpdateDestinationRequest(Com_GwMappingIdType gwMappingId, boolean request);
#endif
/**
 * @brief         Clear all signal group/signal/dest description signal update bit of the Tx Pdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxMainfunctionId:MainfunctionId of Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66480
 */
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
extern void Com_ClearTxIPduUpdates(const Com_TxIPduType* TxIpduPtr, Com_MainFunctionIdType TxMainfunctionId);
#endif
/* PRQA S 5015 -- */
#ifdef __cplusplus
}
#endif
#endif
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
