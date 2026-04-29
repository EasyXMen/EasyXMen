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
 **  @file               : Com_RxInternal.c
 **  @author             : shengnan.sun
 **  @date               : 2024/09/01
 **  @vendor             : isoft
 **  @description        : internal implementation for COM receive
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Com_Internal.h"
#if (STD_ON == COM_DEV_ERROR_DETECT)
#include "Det.h"
#endif
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
/* PRQA S 3120,3132 ++ */ /* VL_QAC_MagicNum */
/* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr */
/* PRQA S 0316,2844 ++ */ /* VL_QAC_0316,VL_Com_NullPtrValue */
/* PRQA S 6520 EOF */     /* VL_MTR_Com_STVAR */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 5015 ++ */ /* VL_Com_UntrustedMethod */
/**
 * @brief         Receive Rx Pdu handle
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu:state pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxIpduId:RxIpdu index
 * @param[in]     PduInfoPtr:Pointer of Pdu info
 * @param[in]     RxMainfunctionId:MainfunctionId of Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66482
 */
#if (0u < COM_MAX_RXIPDU_NUMBER)
COM_LOCAL void Com_RxPduHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
    PduIdType RxIpduId,
#endif
    const PduInfoType*     PduInfoPtr,
    Com_MainFunctionIdType RxMainfunctionId);
#endif
/**
 * @brief         RxSignalGroup receive timeout handle
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxMainfunctionId:MainfunctionId of Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66483
 */
#if ((0u < COM_MAX_RXIPDU_NUMBER) && (COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
COM_LOCAL void Com_RxSignalGroupTimeOutHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId);
#endif
/**
 * @brief         Unpack the rx source description signal to gateway buffer.
 * @param[in]     GwSourceSignalPtr:Pointer of gateway source Signal
 * @param[in]     RxIpduRuntimeBuffPtr:Pointer of RxIpdu runtime buffer
 * @param[in]     RxDynSignalLength:Length of RxDynSignal
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66484
 */
#if (0u < COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER) && (COM_MAX_GWMAPPING_NUMBER > 0u)
COM_LOCAL void Com_SourceSignalUnPack(
    const Com_GwSourceSignalType* GwSourceSignalPtr,
    uint8*                        RxIpduRuntimeBuffPtr,
    uint16                        RxDynSignalLength,
    Com_RxIpduBufIdType           IpduBufferId);
#endif
/**
 * @brief         Unpack the rx signal and notification up layer.
 * @param[in]     RxSignalPtr:Pointer of RxSignal
 * @param[in]     RxIpduRuntimeBuffPtr:Pointer of RxIpdu runtime buffer
 * @param[in]     RxDynSignalLength:Length of RxDynSignal
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66485
 */
#if (0u < COM_MAX_RXSIGNAL_NUMBER)
COM_LOCAL void Com_SignalRxIndication(
    const Com_RxSignalType* RxSignalPtr,
    uint8*                  RxIpduRuntimeBuffPtr,
    uint16                  RxDynSignalLength,
    Com_RxIpduBufIdType     IpduBufferId);
#endif
/**
 * @brief         Unpack the rx signal(signal type is COM_UINT8_N) and notification up layer.
 * @param[in]     RxSignalPtr:Pointer of RxSignal
 * @param[in]     ComSignalLength:Length of signal/groupSignal
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66486
 */
#if ((0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDUBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE))
COM_LOCAL void
    Com_Rx8NSignalHandle(const Com_RxSignalType* RxSignalPtr, uint16 ComSignalLength, Com_RxIpduBufIdType IpduBufferId);
#endif
/**
 * @brief         Unpack the rx signal(signal type is COM_UINT8_DYN) invalidate value.
 * @param[in]     RxSignalPtr:Pointer of RxSignal
 * @param[in]     ComSignalLength:Length of signal/groupSignal
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @retval        ret:TRUE/FALSE
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if (                                                                \
    (0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (0u < COM_MAX_RX_SIGNAL_8BIT_INVALIDVALUE_NUMBER))
COM_LOCAL boolean Com_Rx8DYNSignalInvalidHandle(
    const Com_RxSignalType* RxSignalPtr,
    uint16                  ComSignalLength,
    Com_RxIpduBufIdType     IpduBufferId);
#endif
/**
 * @brief         Unpack the rx signal(signal type is COM_UINT8_DYN) and notification up layer.
 * @param[in]     RxSignalPtr:Pointer of RxSignal
 * @param[in]     ComSignalLength:Length of signal/groupSignal
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66487
 */
#if (                                                                \
    (0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_Rx8DYNSignalHandle(
    const Com_RxSignalType* RxSignalPtr,
    uint16                  ComSignalLength,
    Com_RxIpduBufIdType     IpduBufferId);
#endif
/**
 * @brief         called by Com_RxSignalHandle.
 *                rx signal(signal type isn't COM_UINT8_N or COM_UINT8_DYN) receive invalid value.
 * @param[in]     RxSignalPtr:Pointer of RxSignal
 * @param[in]     SignalType:SignalType
 * @param[in]     UnPackValue:UnPackValue
 * @param[in]     PartitionId:PartitionId
 * @retval        needFilter:needFilter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 *
 */
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
COM_LOCAL boolean Com_RxSignalInvalidHandle(
    const Com_RxSignalType* RxSignalPtr,
    Com_SignalType          SignalType,
    uint64*                 UnPackValue,
    uint16                  PartitionId);
#endif
/**
 * @brief         Unpack the rx signal(signal type isn't COM_UINT8_N or COM_UINT8_DYN) and notification up layer.
 * @param[in]     RxSignalPtr:Pointer of RxSignal
 * @param[in]     RxIpduRuntimeBuffPtr:Pointer of RxIpdu runtime buffer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66488
 */
#if ((0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDUBUFF_SIZE))
COM_LOCAL void Com_RxSignalHandle(const Com_RxSignalType* RxSignalPtr, uint8* RxIpduRuntimeBuffPtr);
#endif
/**
 * @brief         Judge the rx (group) signal value is invalid value or not.
 * @param[in]     SignalType:SignalType of signal/groupSignal
 * @param[in]     InvalidValueId:InvalidValue index
 * @param[in]     Value:Value
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66489
 */
#if ((STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE))
COM_LOCAL boolean Com_SignalInvalidateHandle(Com_SignalType SignalType, uint16 InvalidValueId, uint64 Value);
#endif
/**
 * @brief         Get the rx (group) signal's init value.
 * @param[in]     SignalType:SignalType of signal/groupSignal
 * @param[in]     InitValueId:InitValue index
 * @param[in]     PartitionId:Partition index
 * @retval        unPackValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66490
 */
#if (                                                                                          \
    ((0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)) \
    || ((0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)))

COM_LOCAL uint64 Com_RxSignalReplaceHanlde(Com_SignalType SignalType, uint16 InitValueId, uint16 PartitionId);
#endif
/**
 * @brief         Calculate the rx signal value is filter out or not.
 * @param[in]     SignalType:SignalType of signal/groupSignal
 * @param[in]     FilterType:FilterType
 * @param[in]     FilterIndex:Index from filter type
 * @param[in]     SignalBufferId:SignalBuffer index
 * @param[in]     PartitionId:Partition index
 * @param[in]     NewSignalValue:New signal value
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66491
 */
#if (                                                                                 \
    ((0u < COM_MAX_RXSIGNAL_NUMBER) && (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)) \
    || ((0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)))
COM_LOCAL boolean Com_RxSignalFilter(
    Com_SignalType          SignalType,
    Com_FilterAlgorithmType FilterType,
    Com_SignalIdType        FilterIndex,
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint16 SignalBufferId,
    uint16 PartitionId,
#endif
    uint64 NewSignalValue);
#endif
/**
 * @brief         Update the rx signal value buffer.
 * @param[in]     Value:Value
 * @param[in]     SignalType:SignalType of signal/groupSignal
 * @param[in]     InitValueId:InitValue index
 * @param[in]     PartitionId:Partition index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66492
 */
#if (0u < COM_MAX_RXSIGNAL_NUMBER) || (0u < COM_MAX_RXGROUPSIGNAL_NUMBER)
COM_LOCAL void Com_RxSignalBuffHandle(uint64 Value, Com_SignalType SignalType, uint16 InitValueId, uint16 PartitionId);
#endif
/**
 * @brief         Update the rx signal value buffer.
 * @param[in]     Value:Value
 * @param[in]     GWSignalBufferId:Gateway signal buffer index
 * @param[in]     SignalType:SignalType of signal/groupSignal
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66493
 */
#if (((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER)) || (0u < COM_MAX_RXSIGNAL_NUMBER)) \
    && (COM_MAX_GWMAPPING_NUMBER > 0u)
COM_LOCAL void Com_UpdateGWSignalBuffer(uint64 Value, uint16 GWSignalBufferId, Com_SignalType SignalType);
#endif
/**
 * @brief         Rx signal value replace,and notification.
 * @param[in]     RxSignalPtr:Pointer of RxSignal
 * @param[in]     PartitionId:Partition index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66494
 */
#if ((STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE))
COM_LOCAL void Com_InvalidSignalReplaceAndNotification(const Com_RxSignalType* RxSignalPtr, uint16 PartitionId);
#endif
/**
 * @brief         Handle signal group RxIndication of one rx pdu.
 * @param[in]     RxSignalGroupPtr:Pointer of RxSignalGroup
 * @param[in]     RxDynSignalLength:Length of RxDynSignal
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @param[in]     PartitionId:Partition index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66495
 */
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER))
COM_LOCAL void Com_SignalGroupRxIndication(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    uint16                       RxDynSignalLength,
    Com_RxIpduBufIdType          IpduBufferId,
    uint16                       PartitionId);
#endif
/**
 * @brief         Update the rx group signal value buffer.
 * @param[in]     InvalidSignalGroup:SignalGroup invalid or not
 * @param[in]     RxGroupId:RxGroup index
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @param[in]     RxDynSignalLength:Length of RxDynSignal
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66496
 */
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER))
COM_LOCAL void Com_RxGroupSignalBuffHanlde(
    boolean                 InvalidSignalGroup,
    Com_RxGroupSignalIdType RxGroupId,
    Com_RxIpduBufIdType     IpduBufferId,
    uint16                  RxDynSignalLength);
#endif
/**
 * @brief         Uint8_Dyn/Uint8_N Group Signal Invalid Calculate.
 * @param[in]     ComSignalLength:Length of signal/groupSignal
 * @param[in]     RxGroupSignalPtr:Pointer of RxGroupSignal
 * @param[in]     SignalGroupInvalid:SignalGroup invalid or not
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @param[in]     RxMainfunctionId:MainfunctionId of Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66497
 */
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER))
#if ((STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_Uint8NDynGroupSignalInvalidCalculate(
    uint16                       ComSignalLength,
    const Com_RxGroupSignalType* RxGroupSignalPtr,
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    boolean* SignalGroupInvalid,
#endif
    Com_RxIpduBufIdType    IpduBufferId,
    Com_MainFunctionIdType RxMainfunctionId);
#endif
/**
 * @brief         Signal Group Invalid Calculate.
 * @param[in]     RxSignalGroupPtr:Pointer of RxSignalGroup
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @param[in]     RxDynSignalLength:Length of RxDynSignal
 * @retval
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66498
 */
COM_LOCAL boolean Com_SignalGroupInvalidCalculate(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    Com_RxIpduBufIdType          IpduBufferId,
    uint16                       RxDynSignalLength);
#endif
/**
 * @brief         RxSignal receive timeout handle
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66499
 */
#if ((0u < COM_MAX_RXIPDU_NUMBER) && (COM_MAX_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE))
COM_LOCAL void Com_RxSignalTimeOutHandle(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
#endif
/**
 * @brief         Rx ipdu based receive DM timeout handle
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxMainfunctionId:MainfunctionId of Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66500
 */
#if (                                                                            \
    ((0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)) \
    || ((0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)))
COM_LOCAL void Com_RxIpduBaseDMTimeOutHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId);
#endif
/**
 * @brief         Rx ipdu based receive DM timeout handle signal
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66501
 */
#if (0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
COM_LOCAL void Com_RxDMTimeOutHandleSignal(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
#endif
/**
 * @brief         Rx ipdu based receive DM timeout handle signal group
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxMainfunctionId:MainfunctionId of Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66502
 */
#if (0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
COM_LOCAL void Com_RxDMTimeOutHandleSignalGroup(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId);
#endif
/**
 * @brief         Reset receive MD timer for all signals/signal groups (which is update) of one rx pdu
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     PduInfo
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66503
 */
#if (COM_MAX_RXSIGNAL_NUMBER + COM_MAX_RXSIGNALGROUP_NUMBER > 0u) \
    && ((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
COM_LOCAL void Com_ResetUpdateDMTime(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    const PduInfoType*          PduInfo);
#endif
/**
 * @brief         Copy the Rx Pdu data to RxPdu Buffer
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     PduInfoPtr:Pointer of Pdu info
 * @param[in]     RxMainfunctionId:MainfunctionId of Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66504
 */
COM_LOCAL void Com_CopyPduDataToRxBuffer(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    const PduInfoType*          PduInfoPtr,
    Com_MainFunctionIdType      RxMainfunctionId);
/**
 * @brief         Unpack the rx pdu data and invoke notification
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxMainfunctionId:MainfunctionId of Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66505
 */
#if (0u < COM_MAX_RXIPDU_NUMBER)
COM_LOCAL void Com_IndicationProcess(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId);
#endif
/**
 * @brief         Execute all configured ComDataInvalidActions for all included signals and signal groups.
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66506
 */
#if (STD_ON == COM_RXTPPDU_SUPPORT) \
    && ((STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE))
COM_LOCAL void
    Com_TpPduInvalidAction(const Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr);
#endif
/**
 * @brief         Rx data timeout action handle
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxTimeoutAction:Timeout action of Rx
 * @param[in]     RxArgPtr:Rx argument pointer
 * @param[in]     IsRxSignal:Is RxSignal or not
 * @param[in]     SubstituteBufId:Substitute buffer index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66507
 */
#if (                                                              \
    (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE)        \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)) \
    || ((STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE) \
        || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE))
COM_LOCAL void Com_RxDataTimeOutActionHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_RxDataTimeoutActionType RxTimeoutAction,
    const void*                 RxArgPtr,
    boolean                     IsRxSignal,
    Com_SignalIdType            SubstituteBufId);
#endif
/**
 * @brief         Judge the rx signal group value is filter out or not.
 * @param[in]     RxSignalGroupPtr:Pointer of RxSignalGroup
 * @param[in]     InvalidSignalGroup:SignalGroup invalid or not
 * @param[in]     IpduBufferId:Ipdu buffer index
 * @param[in]     PartitionId:Partition index
 * @retval        signalGroupFilter
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66508
 */
#if (COM_MAX_RXIPDU_NUMBER > 0u) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
COM_LOCAL boolean Com_RxSignalGroupFilter(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    boolean                      InvalidSignalGroup,
    Com_RxIpduBufIdType          IpduBufferId,
    uint16                       PartitionId);
#endif
/* PRQA S 5015 -- */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/**
 * @brief TP RxPdu RxIndication handle
 *
 */
#if (STD_ON == COM_RXTPPDU_SUPPORT)
/* PRQA S 1532 ++ */ /* VL_Com_ReferencedOnlyOne */
void Com_TpRxIndicationHandle(
    PduIdType                   RxIpduId,
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId,
    Std_ReturnType              result)
/* PRQA S 1532 -- */
{
    uint8 activeEnable = RxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
    RxIpduStatePtr->RxIpduRTStFlag &= COM_RX_RECEIVING_DIS;

    if (COM_RX_ACTIVE_EN == activeEnable)
    {
        Com_RxIpduBufIdType rxPduBufferId        = RxIpduPtr->RxIPduBufIndex;
        uint8*              rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
        /*TP PDU successful receipt*/
        if (E_OK == result)
        {
            RxIpduStatePtr->RxIpduLength = (uint16)RxIpduStatePtr->RxOffset;
#if (                                                                                             \
    (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)                                                        \
    || (((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)) \
        && (COM_MAX_RXSIGNAL_NUMBER + COM_MAX_RXSIGNALGROUP_NUMBER > 0u)))
            PduInfoType info;
            info.SduDataPtr = &rxIpduRuntimeBuffPtr[rxPduBufferId];
            info.SduLength  = RxIpduStatePtr->RxIpduLength;
#if (STD_ON == COM_METADATA_SUPPORT)
            uint8 metaDataLength = RxIpduPtr->MetaDataLength;
            if (0u < metaDataLength)
            {
                uint16 metaDataPos = (uint16)RxIpduPtr->IPduNoDynSignalLength
                                     + (uint16)RxIpduPtr->IPduMaxDynSignalLength + rxPduBufferId;
                info.MetaDataPtr = &rxIpduRuntimeBuffPtr[metaDataPos];
            }
            else
#endif
            {
                info.MetaDataPtr = NULL_PTR;
            }
#endif
#if ((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
            uint8 DMEnable = RxIpduStatePtr->RxIpduRTStFlag & COM_RX_DM_EN;
            if (COM_RX_DM_EN == DMEnable)
            {
/*reset all signals and signal groups DM time(except the signal/signal group have update bit and the update bit is not
 * 1)*/
#if (COM_MAX_RXSIGNAL_NUMBER + COM_MAX_RXSIGNALGROUP_NUMBER > 0u)
                Com_ResetUpdateDMTime(RxIpduStatePtr, RxIpduPtr, &info);
#endif
            }
#endif

#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
            boolean ret = TRUE;
            if (NULL_PTR != RxIpduPtr->ComIPduCallout)
            {
                ret = RxIpduPtr->ComIPduCallout(RxIpduId, &info);
            }
            if (ret)
#endif
            {
                /*signal/signal group RxIndication is made immediate*/
#if (STD_ON == COM_RX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE)
                if (COM_IMMEDIATE == RxIpduPtr->ComIPduSignalProcessing)
                {
                    RxIpduStatePtr->RxIpduRTStFlag &= COM_RX_INDICATION_DIS;
                    /*rx indication handle*/
                    Com_IndicationProcess(RxIpduStatePtr, RxIpduPtr, RxMainfunctionId);
                }
                /*signal/signal group RxIndication is made in next MainFunctionRx*/
                else
#endif
                {
                    RxIpduStatePtr->RxIpduRTStFlag |= COM_RX_INDICATION_EN;
                }
            }
        }
        /*TP PDU false receipt:the AUTOSAR COM module shall treat all signals of the
         * received large I-PDU exactly like invalidated signals.*/
        else
        {
            const Com_RxIPduRunTimeStateType* rxIpduInitStatePtr = Com_ConfigStd->RxIpduInitStatePtr[RxMainfunctionId];
            const uint8*                      rxIpduInitValuePtr = Com_ConfigStd->RxIpduInitValuePtr[RxMainfunctionId];
            uint16 ipduIdPerMainfunction = RxIpduId - Com_GetStartOfMainFunctionRx(RxMainfunctionId);
            RxIpduStatePtr->RxIpduLength = rxIpduInitStatePtr[ipduIdPerMainfunction].RxIpduLength;
            rxPduBufferId                = RxIpduPtr->RxIPduBufIndex;
            SchM_Enter_Com_Context();
            (void)IStdLib_MemCpy(
                &rxIpduRuntimeBuffPtr[rxPduBufferId],
                &rxIpduInitValuePtr[rxPduBufferId],
                RxIpduStatePtr->RxIpduLength);
            SchM_Exit_Com_Context();
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
            /*execute all configured ComDataInvalidActions for all included signals and signal groups*/
            Com_TpPduInvalidAction(RxIpduStatePtr, RxIpduPtr);
#endif
        }
    }
}
/**
 * @brief TP Pdu copy receive data handle
 *
 */
/* PRQA S 1532 ++ */ /* VL_Com_ReferencedOnlyOne */
BufReq_ReturnType Com_CopyRxDataHandle(
    PduIdType              RxIpduId,
    Com_MainFunctionIdType RxMainfunctionId,
    const PduInfoType*     info,
    PduLengthType*         bufferSizePtr)
/* PRQA S 1532 -- */
{
    uint16                      ipduIdPerMainfunction = RxIpduId - Com_GetStartOfMainFunctionRx(RxMainfunctionId);
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[RxMainfunctionId][ipduIdPerMainfunction];
    BufReq_ReturnType           bufReq         = BUFREQ_E_NOT_OK;
    uint8                       activeEnable   = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
    uint8                       Receiving      = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_RECEIVING_EN;

    if ((COM_RX_ACTIVE_EN == activeEnable) && (COM_RX_RECEIVING_EN == Receiving)
        && ((rxIpduStatePtr->RxOffset + info->SduLength) <= rxIpduStatePtr->RxIpduLength))
    {
        const Com_RxIPduType* rxIpduPtr    = &Com_CfgRxIPduPtr[RxIpduId];
        Com_RxIpduBufIdType   copyStartPos = rxIpduPtr->RxIPduBufIndex + (Com_RxIpduBufIdType)rxIpduStatePtr->RxOffset;
        /*copy the RX data to RX PDU buffer*/
        uint8* rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
        SchM_Enter_Com_Context();
        (void)IStdLib_MemCpy(&rxIpduRuntimeBuffPtr[copyStartPos], info->SduDataPtr, info->SduLength);
        SchM_Exit_Com_Context();
        rxIpduStatePtr->RxOffset += info->SduLength;
        PduLengthType rxPduMaxLength = rxIpduPtr->IPduNoDynSignalLength + rxIpduPtr->IPduMaxDynSignalLength;
        *bufferSizePtr               = rxPduMaxLength - rxIpduStatePtr->RxOffset;
        bufReq                       = BUFREQ_OK;
    }
    return bufReq;
}
/**
 * @brief TP Pdu Start receive handle
 *
 */
/* PRQA S 1532,6030 ++ */ /* VL_Com_ReferencedOnlyOne,VL_MTR_Com_STMIF */
BufReq_ReturnType Com_StartOfReceptionHandle(
    PduIdType              RxIpduId,
    Com_MainFunctionIdType RxMainfunctionId,
    const PduInfoType*     info,
    PduLengthType          TpSduLength,
    PduLengthType*         bufferSizePtr)
/* PRQA S 1532,6030 -- */
{
    BufReq_ReturnType           bufReq                = BUFREQ_E_NOT_OK;
    uint16                      ipduIdPerMainfunction = RxIpduId - Com_GetStartOfMainFunctionRx(RxMainfunctionId);
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[RxMainfunctionId][ipduIdPerMainfunction];
    uint8                       activeEnable   = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;

    if (COM_RX_ACTIVE_EN == activeEnable)
    {
        const Com_RxIPduType* rxIpduPtr      = &Com_CfgRxIPduPtr[RxIpduId];
        PduLengthType         rxPduMaxLength = rxIpduPtr->IPduNoDynSignalLength + rxIpduPtr->IPduMaxDynSignalLength;
        /*the TP PDU length is too large than the RX PDU buffer*/
        if (TpSduLength > rxPduMaxLength)
        {
            bufReq = BUFREQ_E_OVFL;
        }
        else
        {
            /*the tp pdu is not receiving and the rx pdu length >= min length of the pdu*/
            uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_RECEIVING_EN;
            *bufferSizePtr  = rxPduMaxLength;
            if ((COM_RX_RECEIVING_EN != Receiving) && (TpSduLength >= rxIpduPtr->IPduNoDynSignalLength))
            {
                rxIpduStatePtr->RxIpduLength = (uint16)TpSduLength;
                rxIpduStatePtr->RxIpduRTStFlag |= COM_RX_RECEIVING_EN;
                rxIpduStatePtr->RxOffset = (NULL_PTR == info) ? 0u : info->SduLength;
                if (NULL_PTR != info)
                {
                    Com_RxIpduBufIdType rxIPduBufIndex       = rxIpduPtr->RxIPduBufIndex;
                    uint8*              rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
                    if (NULL_PTR != info->SduDataPtr)
                    {
                        SchM_Enter_Com_Context();
                        (void)IStdLib_MemCpy(&rxIpduRuntimeBuffPtr[rxIPduBufIndex], info->SduDataPtr, info->SduLength);
                        SchM_Exit_Com_Context();
                        *bufferSizePtr = rxPduMaxLength - info->SduLength;
                    }
#if (STD_ON == COM_METADATA_SUPPORT)
                    if (NULL_PTR != info->MetaDataPtr)
                    {
                        uint8  metadataLength = rxIpduPtr->MetaDataLength;
                        uint16 metaDataPos    = (uint16)rxPduMaxLength + (uint16)rxIPduBufIndex;
                        /*copy the metadata to RX PDU buffer*/
                        SchM_Enter_Com_Context();
                        (void)IStdLib_MemCpy(&rxIpduRuntimeBuffPtr[metaDataPos], info->MetaDataPtr, metadataLength);
                        SchM_Exit_Com_Context();
                    }
#endif
                }
                bufReq = BUFREQ_OK;
            }
        }
    }
    return bufReq;
}
#endif
/* PRQA S 2895 ++ */ /* VL_Com_NegtiveValueType */
/**
 * @brief receive SignalGroup(Array) data handle
 *
 */
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) \
    && ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER))
/* PRQA S 1532 ++ */ /* VL_Com_ReferencedOnlyOne */
uint8 Com_ReceiveSignalGroupArrayHandle(Com_SignalGroupIdType SignalGroupId, uint8* SignalGroupArrayPtr)
/* PRQA S 1532 -- */
{
    uint8                        ret              = COM_SERVICE_NOT_AVAILABLE;
    const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[SignalGroupId];
    const Com_MainFunctionIdType rxMainfunctionId = rxSignalGroupPtr->RxMainfunctionId;
    /*the signal group is array*/
    if (COM_UNUSED_COM_SIGNALGROUPARRYID != rxSignalGroupPtr->SignalGroupArrayId)
    {
        PduIdType ipduRef               = rxSignalGroupPtr->IpduRefIndex;
        PduIdType ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
        const Com_RxIPduRunTimeStateType* rxIpduStatePtr =
            &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
        /*the I-Pdu is receiving,cann't copy data*/
        uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_RECEIVING_EN;
        /*IPDU is receiving data*/
        if (COM_RX_RECEIVING_EN == Receiving)
        {
            ret = COM_BUSY;
        }
        else
        {
            const uint8*                    rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[rxMainfunctionId];
            const Com_SignalGroupArrayType* signalGroupArray =
                &Com_ConfigStd->SignalGroupArrayPtr[rxSignalGroupPtr->SignalGroupArrayId];
            SchM_Enter_Com_Context();
            (void)IStdLib_MemCpy(
                SignalGroupArrayPtr,
                &rxIpduRuntimeBuffPtr[signalGroupArray->IPduStartBufIndex],
                signalGroupArray->SignalGroupArrayLength);
            SchM_Exit_Com_Context();
            uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
            if (COM_RX_ACTIVE_EN == activeEnable)
            {
                ret = E_OK;
            }
        }
    }
    return ret;
}
#endif
/**
 * @brief If RxPdu RxIndication handle
 *
 */
#if (0u < COM_MAX_RXIPDU_NUMBER)
/* PRQA S 1532 ++ */ /* VL_Com_ReferencedOnlyOne */
void Com_RxIndicationHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
    PduIdType RxIpduId,
#endif
    const PduInfoType*     PduInfoPtr,
    Com_MainFunctionIdType RxMainfunctionId)
/* PRQA S 1532 -- */
{
    uint16 minPduLength = (uint16)RxIpduPtr->IPduNoDynSignalLength;
    uint16 maxPduLength = minPduLength + (uint16)RxIpduPtr->IPduMaxDynSignalLength;
    uint8  activeEnable = RxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;

    /*the rx pdu is active,the receive length is suited to pdu configuration length*/
    if ((COM_RX_ACTIVE_EN == activeEnable) && (PduInfoPtr->SduLength >= minPduLength)
        && (PduInfoPtr->SduLength <= maxPduLength))
    {
#if ((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
        uint8 DMEnable = RxIpduStatePtr->RxIpduRTStFlag & COM_RX_DM_EN;
        if (COM_RX_DM_EN == DMEnable)
        {
            /*reset all signals and signal groups DM time(except the signal/signal group have update bit and the
             * update bit is not 1)*/
            Com_ResetUpdateDMTime(RxIpduStatePtr, RxIpduPtr, PduInfoPtr);
        }
#endif

        Com_CopyPduDataToRxBuffer(RxIpduStatePtr, RxIpduPtr, PduInfoPtr, RxMainfunctionId);
        Com_RxPduHandle(
            RxIpduStatePtr,
            RxIpduPtr,
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
            RxIpduId,
#endif
            PduInfoPtr,
            RxMainfunctionId);
    }
}
#endif

/**
 * @brief RxPdu MainFunction handle
 *
 */
#if (0u < COM_MAX_RXIPDU_NUMBER)
void Com_MainFunctionRxHandle(Com_MainFunctionIdType RxMainfunctionId) /* PRQA S 1532 */ /* VL_Com_ReferencedOnlyOne */
{
    PduIdType                   rxIpduStartId  = Com_GetStartOfMainFunctionRx(RxMainfunctionId);
    PduIdType                   rxIpduEndId    = Com_GetEndOfMainFunctionRx(RxMainfunctionId);
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[RxMainfunctionId][0u];

    for (PduIdType rxIpduId = rxIpduStartId; rxIpduId < rxIpduEndId; ++rxIpduId)
    {
        uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
        if (COM_RX_ACTIVE_EN == activeEnable)
        {
            const Com_RxIPduType* rxIpduPtr    = &Com_CfgRxIPduPtr[rxIpduId];
            uint8                 RxIndication = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_INDICATION_EN;
            /*signal/signal group RxIndication is DEFERRED in the MainFunctionRx*/
            if (COM_RX_INDICATION_EN == RxIndication)
            {
                /*rx indication handle*/
                Com_IndicationProcess(rxIpduStatePtr, rxIpduPtr, RxMainfunctionId);
                rxIpduStatePtr->RxIpduRTStFlag &= COM_RX_INDICATION_DIS;
                /*the rx pdu include signal/signal group which need gateway*/
            }
            /*if the rx DM is enabled*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
            uint8 DMEnable = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_DM_EN;
            if (COM_RX_DM_EN == DMEnable)
            {
#if (COM_MAX_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
                if (COM_RX_SIGNAL_DM == (rxIpduStatePtr->RxIpduDMFlag & COM_RX_SIGNAL_DM))
                {
                    Com_RxSignalTimeOutHandle(rxIpduStatePtr, rxIpduPtr);
                }
#endif

#if (COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
                if (COM_RX_SIGNAL_GROUP_DM == (rxIpduStatePtr->RxIpduDMFlag & COM_RX_SIGNAL_GROUP_DM))
                {
                    Com_RxSignalGroupTimeOutHandle(rxIpduStatePtr, rxIpduPtr, RxMainfunctionId);
                }
#endif

#if ((0u < COM_MAX_RXSIGNAL_NUMBER) || (0u < COM_MAX_RXGROUPSIGNAL_NUMBER))
                if ((COM_RX_IPDU_DM_ONLY == (rxIpduStatePtr->RxIpduDMFlag & COM_RX_IPDU_DM_ONLY))
                    || (COM_RX_IPDU_DM == (rxIpduStatePtr->RxIpduDMFlag & COM_RX_IPDU_DM)))
                {
                    Com_RxIpduBaseDMTimeOutHandle(rxIpduStatePtr, rxIpduPtr, RxMainfunctionId);
                }
#endif
            }
#endif
        }
        rxIpduStatePtr++;
    }
}
#endif

/**
 * @brief Receive Dynamic Group Signal handle
 *
 */
#if (                                                                            \
    ((COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (COM_MAX_RXGROUPSIGNAL_NUMBER > 0u)) \
    && (0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE))                               \
    && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (0u < COM_MAX_RXIPDU_NUMBER)
uint8 Com_ReceiveDynGroupSignalHandle(Com_SignalIdType SignalId, void* SignalDataPtr, uint16* Length)
{
    const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[SignalId - Com_CfgRxSignalNum];
    Com_RxSignalGroupIdType      signalGroupId    = rxGroupSignalPtr->ComSignalGroupRef;
    const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[signalGroupId];
    PduIdType                    ipduRef          = rxSignalGroupPtr->IpduRefIndex;
    Com_MainFunctionIdType       rxMainfunctionId = rxSignalGroupPtr->RxMainfunctionId;
    uint16                       signalLength;
    uint16                       signalInitValueId;
    uint8                        ret = COM_SERVICE_NOT_AVAILABLE;

    /* Check IPdu which this group signal belonged is Active or not,the group signal type must be
     * COM_UINT8_DYN*/
    if (COM_UINT8_DYN == rxGroupSignalPtr->ComSignalType)
    {
        uint16                      ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
        Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
        uint8                       Receiving      = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_RECEIVING_EN;
        /*IPDU is not receiving data*/
        if (COM_RX_RECEIVING_EN != Receiving)
        {
            Com_RxIPduType* rxIpduPtr   = &Com_CfgRxIPduPtr[rxMainfunctionId];
            uint16          partitionId = rxIpduPtr->IpduPartitionId;
            signalLength                = (rxIpduStatePtr->RxIpduLength - rxIpduPtr->IPduNoDynSignalLength);
            if (signalLength > (*Length))
            {
#if (STD_ON == COM_DEV_ERROR_DETECT)
                (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, COM_E_PARAM);
#endif
                ret = E_NOT_OK;
            }
            else
            {
                signalInitValueId               = rxGroupSignalPtr->SignalInitValueId;
                uint8* rxGroupSignal8BitBuffPtr = Com_RxGroupSignal8BitShadowBuff[partitionId];
                /* Copy Uint8DYN signal value one byte by one */
                (void)IStdLib_MemCpy((uint8*)SignalDataPtr, &rxGroupSignal8BitBuffPtr[signalInitValueId], signalLength);
                *Length = signalLength;
                /* Check IPdu which this group signal belonged is Active or not*/
                uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
                if (COM_RX_ACTIVE_EN == activeEnable)
                {
                    ret = E_OK;
                }
            }
        }
        else
        {
            ret = COM_BUSY;
        }
    }
    return ret;
}
#endif
/**
 * @brief Receive Dynamic Signal handle
 *
 */
#if (                                                                     \
    (0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE) \
    && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE))                   \
    && (0u < COM_MAX_RXIPDU_NUMBER)
/* PRQA S 3673,1532 ++ */ /* VL_QAC_3673,VL_Com_ReferencedOnlyOne */
uint8 Com_ReceiveDynSignalHandle(Com_SignalIdType SignalId, void* SignalDataPtr, uint16* Length)
/* PRQA S 3673,1532 -- */
{
    uint8                   ret              = COM_SERVICE_NOT_AVAILABLE;
    const Com_RxSignalType* rxSignalPtr      = &Com_CfgRxSignalPtr[SignalId];
    Com_MainFunctionIdType  rxMainfunctionId = rxSignalPtr->RxMainfunctionId;
    PduIdType               ipduRef          = rxSignalPtr->IpduRefIndex;
    const Com_RxIPduType*   rxIpduPtr        = &Com_CfgRxIPduPtr[ipduRef];
    uint16                  partitionId      = rxIpduPtr->IpduPartitionId;

    /*the signal type must be COM_UINT8_DYN */
    if (COM_UINT8_DYN == rxSignalPtr->ComSignalType)
    {
        /*IPDU is not receiving data*/
        PduIdType ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
        const Com_RxIPduRunTimeStateType* rxIpduStatePtr =
            &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
        uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_RECEIVING_EN;
        if (COM_RX_RECEIVING_EN != Receiving)
        {
            uint16 signalLength = (rxIpduStatePtr->RxIpduLength - (uint16)rxIpduPtr->IPduNoDynSignalLength);
            if (signalLength > (*Length))
            {
#if (STD_ON == COM_DEV_ERROR_DETECT)
                (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, COM_E_PARAM);
#endif
                ret = E_NOT_OK;
            }
            else
            {
                uint16       signalInitValueId        = rxSignalPtr->SignalInitValueId;
                const uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[partitionId];
                /* Copy Uint8DYN signal value one byte by one */
                SchM_Enter_Com_Context();
                (void)IStdLib_MemCpy((uint8*)SignalDataPtr, &signal8BitRuntimeBuffPtr[signalInitValueId], signalLength);
                SchM_Exit_Com_Context();
                *Length = signalLength;
                /* Check IPdu which this group signal belonged is Active or not*/
                uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
                if (COM_RX_ACTIVE_EN == activeEnable)
                {
                    ret = E_OK;
                }
            }
        }
        else
        {
            ret = COM_BUSY;
        }
    }
    return ret;
}
#endif

/**
 * @brief Receive all group signal of the SignalGroup
 *
 */
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER))
/* PRQA S 1532,3673 ++ */ /* VL_Com_ReferencedOnlyOne,VL_QAC_3673 */
void Com_ReceiveSignalGroupHandle(
    const Com_RxIPduType*       RxIpduPtr,
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    uint16                      PartitionId,
    Com_SignalGroupIdType       SignalGroupId)
/* PRQA S 1532,3673 -- */
{
    const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[SignalGroupId];

    Com_RxGroupSignalIdType groupSignalEndId   = rxSignalGroupPtr->GroupSignalRefEndId;
    Com_RxGroupSignalIdType groupSignalStartId = rxSignalGroupPtr->GroupSignalRefStartId;
#if ((STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE))
    uint16 signalLength;
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_BOOLBUFF_SIZE) && (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    boolean*       rxGroupSignalBoolRuntimeBuffPtr = Com_RxGroupSignalBoolShadowBuff[PartitionId];
    const boolean* signalBoolRuntimeBuffPtr        = Com_SignalBoolRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    uint8*       rxGroupSignal8BitRuntimeBuffPtr = Com_RxGroupSignal8BitShadowBuff[PartitionId];
    const uint8* signal8BitRuntimeBuffPtr        = Com_Signal8BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_16BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    uint16*       rxGroupSignalBit16RuntimeBuffPtr = Com_RxGroupSignalBit16ShadowBuff[PartitionId];
    const uint16* signalBit16RuntimeBuffPtr        = Com_SignalBit16RuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_32BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    uint32*       rxGroupSignal32BitRuntimeBuffPtr = Com_RxGroupSignal32BitShadowBuff[PartitionId];
    const uint32* signal32BitRuntimeBuffPtr        = Com_Signal32BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_64BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    uint64*       rxGroupSignal64BitRuntimeBuffPtr = Com_RxGroupSignal64BitShadowBuff[PartitionId];
    const uint64* signal64BitRuntimeBuffPtr        = Com_Signal64BitRuntimeBuff[PartitionId];
#endif
    SchM_Enter_Com_Context();
    for (; groupSignalStartId < groupSignalEndId; ++groupSignalStartId)
    {
        const Com_RxGroupSignalType* rxGroupSignalPtr  = &Com_CfgRxGroupSignalPtr[groupSignalStartId];
        uint16                       signalInitValueId = rxGroupSignalPtr->SignalInitValueId;
        Com_SignalType               signalType        = rxGroupSignalPtr->ComSignalType;
        switch (signalType)
        {
#if ((0u < COM_MAX_RXGROUPSIGNAL_BOOLBUFF_SIZE) && (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE))
        case COM_BOOLEAN:
            rxGroupSignalBoolRuntimeBuffPtr[signalInitValueId] = signalBoolRuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if ((0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE))
        case COM_SINT8:
        case COM_UINT8:
            rxGroupSignal8BitRuntimeBuffPtr[signalInitValueId] = signal8BitRuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if ((0u < COM_MAX_RXGROUPSIGNAL_16BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE))
        case COM_SINT16:
        case COM_UINT16:
            rxGroupSignalBit16RuntimeBuffPtr[signalInitValueId] = signalBit16RuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if ((0u < COM_MAX_RXGROUPSIGNAL_32BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE))
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            rxGroupSignal32BitRuntimeBuffPtr[signalInitValueId] = signal32BitRuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if ((0u < COM_MAX_RXGROUPSIGNAL_64BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE))
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            rxGroupSignal64BitRuntimeBuffPtr[signalInitValueId] = signal64BitRuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if ((0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE))
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        case COM_UINT8_N:
            signalLength = rxGroupSignalPtr->ComSignalLength;
            (void)IStdLib_MemCpy(
                &rxGroupSignal8BitRuntimeBuffPtr[signalInitValueId],
                &signal8BitRuntimeBuffPtr[signalInitValueId],
                signalLength);
            break;
#endif
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        case COM_UINT8_DYN:
            signalLength = RxIpduStatePtr->RxIpduLength - (uint16)RxIpduPtr->IPduNoDynSignalLength;
            (void)IStdLib_MemCpy(
                &rxGroupSignal8BitRuntimeBuffPtr[signalInitValueId],
                &signal8BitRuntimeBuffPtr[signalInitValueId],
                signalLength);
            break;
#endif
#endif
        default:
            /*do nothing*/
            break;
        }
    }
    SchM_Exit_Com_Context();
    COM_NOUSED(RxIpduPtr);
    COM_NOUSED(RxIpduStatePtr);

    return;
}
#endif

/**
 * @brief Receive group signal handle
 *
 */
#if (0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER)
/* PRQA S 3673,1532 ++ */ /*  VL_QAC_3673,VL_Com_ReferencedOnlyOne */
uint8 Com_ReceiveGroupSignalHandle(Com_SignalIdType SignalId, void* SignalDataPtr)
/* PRQA S 3673,1532 -- */
{
    uint8                        ret = COM_SERVICE_NOT_AVAILABLE;
    const Com_RxGroupSignalType* rxGroupSignalPtr =
        &Com_ConfigStd->RxGroupSignalPtr[SignalId - Com_ConfigStd->RxSignalNum];
    Com_RxSignalGroupIdType           signalGroupId         = rxGroupSignalPtr->ComSignalGroupRef;
    const Com_RxSignalGroupType*      rxSignalGroupPtr      = &Com_ConfigStd->RxSignalGroupPtr[signalGroupId];
    PduIdType                         ipduRef               = rxSignalGroupPtr->IpduRefIndex;
    Com_MainFunctionIdType            rxMainfunctionId      = rxSignalGroupPtr->RxMainfunctionId;
    PduIdType                         ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
    const Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
    const Com_RxIPduType*             rxIpduPtr      = &Com_ConfigStd->RxIPduPtr[rxMainfunctionId];
    uint16                            partitionId    = rxIpduPtr->IpduPartitionId;
    /*TP IPDU is not receiving data*/
    uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_RECEIVING_EN;

#if (0u < COM_MAX_RXGROUPSIGNAL_BOOLBUFF_SIZE)
    const boolean* rxGroupSignalBoolRuntimeBuffPtr = Com_RxGroupSignalBoolShadowBuff[partitionId];
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE)
    const uint8* rxGroupSignal8BitRuntimeBuffPtr = Com_RxGroupSignal8BitShadowBuff[partitionId];
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_16BITBUFF_SIZE)
    const uint16* rxGroupSignalBit16RuntimeBuffPtr = Com_RxGroupSignalBit16ShadowBuff[partitionId];
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_32BITBUFF_SIZE)
    const uint32* rxGroupSignal32BitRuntimeBuffPtr = Com_RxGroupSignal32BitShadowBuff[partitionId];
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_64BITBUFF_SIZE)
    const uint64* rxGroupSignal64BitRuntimeBuffPtr = Com_RxGroupSignal64BitShadowBuff[partitionId];
#endif
    if (COM_RX_RECEIVING_EN != Receiving)
    {
        uint16 signalInitValueId = rxGroupSignalPtr->SignalInitValueId;
        /* Get signal type, decide signal value is in which Runtime buffer */
#if (0u < COM_MAX_RXGROUPSIGNAL_BOOLBUFF_SIZE) || (0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE)            \
    || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (0u < COM_MAX_RXGROUPSIGNAL_16BITBUFF_SIZE) \
    || (0u < COM_MAX_RXGROUPSIGNAL_32BITBUFF_SIZE) || (0u < COM_MAX_RXGROUPSIGNAL_64BITBUFF_SIZE)
        switch (rxGroupSignalPtr->ComSignalType)
        {
#if (0u < COM_MAX_RXGROUPSIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            *((boolean*)SignalDataPtr) = rxGroupSignalBoolRuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            *((uint8*)SignalDataPtr) = rxGroupSignal8BitRuntimeBuffPtr[signalInitValueId];
            break;
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        case COM_UINT8_N:
            /* Copy Uint8N signal value one byte by one */
            (void)IStdLib_MemCpy(
                (uint8*)SignalDataPtr,
                &rxGroupSignal8BitRuntimeBuffPtr[signalInitValueId],
                rxGroupSignalPtr->ComSignalLength);
            break;
#endif
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            *((uint16*)SignalDataPtr) = rxGroupSignalBit16RuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
            *((uint32*)SignalDataPtr) = rxGroupSignal32BitRuntimeBuffPtr[signalInitValueId];
            break;
        case COM_FLOAT32:
            /* PRQA S 0310, 0311 ++ */ /* VL_Com_PointerObjectType, VL_Com_PointerObjectType */
            *((float32*)SignalDataPtr) = *(float32*)(&rxGroupSignal32BitRuntimeBuffPtr[signalInitValueId]);
            /* PRQA S 0310, 0311 -- */
            break;
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
            *((uint64*)SignalDataPtr) = rxGroupSignal64BitRuntimeBuffPtr[signalInitValueId];
            break;
        case COM_FLOAT64:
            /* PRQA S 0310 ++ */ /* VL_Com_PointerObjectType */
            *((float64*)SignalDataPtr) = *(float64*)(&rxGroupSignal64BitRuntimeBuffPtr[signalInitValueId]);
            /* PRQA S 0310 -- */
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
#endif
        /* Check IPdu which this group signal belonged is Active or not */
        uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
        if (COM_RX_ACTIVE_EN == activeEnable)
        {
            ret = E_OK;
        }
    }
    else
    {
        ret = COM_BUSY;
    }
    COM_NOUSED(SignalDataPtr);
    return ret;
}
#endif

/**
 * @brief Receive signal handle
 *
 */
#if (0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER)
/* PRQA S 3673, 1532, 6010, 5017 ++ */ /* VL_QAC_3673, VL_Com_ReferencedOnlyOne, VL_MTR_Com_STCYC, VL_Com_5017 */
uint8 Com_ReceiveSignalHandle(Com_SignalIdType SignalId, void* SignalDataPtr)
/* PRQA S 3673, 1532, 6010, 5017 -- */
{
    uint8                   ret              = COM_SERVICE_NOT_AVAILABLE;
    const Com_RxSignalType* rxSignalPtr      = &Com_ConfigStd->RxSignalPtr[SignalId];
    Com_MainFunctionIdType  rxMainfunctionId = rxSignalPtr->RxMainfunctionId;
    PduIdType               ipduRef          = rxSignalPtr->IpduRefIndex;
    const Com_RxIPduType*   rxIpduPtr        = &Com_ConfigStd->RxIPduPtr[ipduRef];
    uint16                  partitionId      = rxIpduPtr->IpduPartitionId;

#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    const boolean* signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[partitionId];
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    const uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[partitionId];
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    const uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[partitionId];
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    const uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[partitionId];
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    const uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[partitionId];
#endif
    PduIdType                         ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
    const Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
    /*IPDU is not receiving data*/
    uint8 Receiving = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_RECEIVING_EN;

    if (COM_RX_RECEIVING_EN != Receiving)
    {
        uint16 signalInitValueId = rxSignalPtr->SignalInitValueId;
        /* Get signal type, decide signal value is in which Runtime buffer */
        SchM_Enter_Com_Context();
        switch (rxSignalPtr->ComSignalType)
        {
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            *((boolean*)SignalDataPtr) = signalBoolRuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            *((uint8*)SignalDataPtr) = signal8BitRuntimeBuffPtr[signalInitValueId];
            break;
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE)
        case COM_UINT8_N:
            /* Copy Uint8N signal value one byte by one */
            (void)IStdLib_MemCpy(
                (uint8*)SignalDataPtr,
                &signal8BitRuntimeBuffPtr[signalInitValueId],
                rxSignalPtr->ComSignalLength);
            break;
#endif
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            *((uint16*)SignalDataPtr) = signalBit16RuntimeBuffPtr[signalInitValueId];
            break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
            *((uint32*)SignalDataPtr) = signal32BitRuntimeBuffPtr[signalInitValueId];
            break;
        case COM_FLOAT32:
            /* PRQA S 0310 ++ */ /* VL_Com_PointerObjectType */
            *((float32*)SignalDataPtr) = *(const float32*)(&signal32BitRuntimeBuffPtr[signalInitValueId]);
            /* PRQA S 0310 -- */
            break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
            *((uint64*)SignalDataPtr) = signal64BitRuntimeBuffPtr[signalInitValueId];
            break;
        case COM_FLOAT64:
            /* PRQA S 0310 ++ */ /* VL_Com_PointerObjectType */
            *((float64*)SignalDataPtr) = *(const float64*)(&signal64BitRuntimeBuffPtr[signalInitValueId]);
            /* PRQA S 0310 -- */
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
        SchM_Exit_Com_Context();
        /* Check IPdu which this Signal belonged is Active or not */
        uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
        if (COM_RX_ACTIVE_EN == activeEnable)
        {
            ret = E_OK;
        }
    }
    else
    {
        ret = COM_BUSY;
    }
    COM_NOUSED(SignalDataPtr);
    return ret;
}
#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/**
 * @brief Receive Rx Pdu handle
 *
 */
#if (0u < COM_MAX_RXIPDU_NUMBER)
COM_LOCAL void Com_RxPduHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
    PduIdType RxIpduId,
#endif
    const PduInfoType*     PduInfoPtr,
    Com_MainFunctionIdType RxMainfunctionId)
{
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
    boolean ret = TRUE;
    if (NULL_PTR != RxIpduPtr->ComIPduCallout)
    {
        ret = RxIpduPtr->ComIPduCallout(RxIpduId, PduInfoPtr);
    }
    if (ret)
#endif
    {
        /*signal/signal group RxIndication is made immediate*/
#if (STD_ON == COM_RX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE)
        if (COM_IMMEDIATE == RxIpduPtr->ComIPduSignalProcessing)
        {
            RxIpduStatePtr->RxIpduRTStFlag &= COM_RX_INDICATION_DIS;
            /*rx indication handle*/
            Com_IndicationProcess(RxIpduStatePtr, RxIpduPtr, RxMainfunctionId);
        }
        /*signal/signal group RxIndication is made in next MainFunctionRx*/
        else
#endif
        {
            RxIpduStatePtr->RxIpduRTStFlag |= COM_RX_INDICATION_EN;
        }
    }
    COM_NOUSED(RxIpduPtr);
    COM_NOUSED(PduInfoPtr);
    COM_NOUSED(RxMainfunctionId);
}
#endif

/**
 * @brief RxSignalGroup receive timeout handle
 *
 */
#if ((0u < COM_MAX_RXIPDU_NUMBER) && (COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
/* PRQA S 3673 ++*/ /* VL_QAC_3673 */
COM_LOCAL void Com_RxSignalGroupTimeOutHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId)
/* PRQA S 3673 -- */
{
    uint16 substituteBufId = COM_UNUSED_UINT16;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    /* substituteStartId is continuous */
    Com_RxGroupSignalIdType substituteStartId = RxIpduPtr->GrpSigSubstituteStartId;
#endif
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;
    ipduSignalGroupTimeoutNumber                                = RxIpduPtr->IPduSignalGroupsTimeoutRefEndId;
    counter                                                     = RxIpduPtr->IPduSignalGroupsTimeoutRefStartId;
    const uint16*                  rxSignalGroupTimeoutValuePtr = Com_CfgSignalTimeoutPtr->RxSigGrpTimeoutVal;
    const Com_RxSignalGroupIdType* rxSignalGroupId              = Com_CfgSignalTimeoutPtr->TimeoutRxSigGrpId;

    for (; counter < ipduSignalGroupTimeoutNumber; ++counter)
    {
        const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[rxSignalGroupId[counter]];
        if (Com_RxSignalGroupTimeOutCnt[counter] > 0u)
        {
            Com_RxSignalGroupTimeOutCnt[counter] -= 1u;
            if (0u == Com_RxSignalGroupTimeOutCnt[counter])
            {
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE)       \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE) \
    || ((COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u))
                Com_RxGroupSignalIdType groupSignalEndId, groupSignalStartId;
                groupSignalEndId = rxSignalGroupPtr->GroupSignalRefEndId;
                const Com_RxGroupSignalType* rxGroupSignalPtr;
#endif
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
                groupSignalStartId = rxSignalGroupPtr->GroupSignalRefStartId;
                for (; groupSignalStartId < groupSignalEndId; ++groupSignalStartId)
                {
                    rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[groupSignalStartId];
                    if ((COM_UNUSED_RXGRPSIGFILTERID != rxGroupSignalPtr->FilterIndex)
                        && (COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxGroupSignalPtr->FilterAlgorithm))
                    {
                        Com_MaskNewDifferMaskOldTimeOut[rxGroupSignalPtr->FilterIndex] = TRUE;
                    }
                }
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                Com_RxDataTimeoutActionType timeoutAction =
                    Com_CfgSignalTimeoutPtr->RxSigGrpDataTimeoutActionCfg[counter];
                if (COM_TIMEOUT_ACTION_NONE != timeoutAction)
                {
/*replace the rx pdu buffer for signal group array*/
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
                    Com_SignalGroupArryIdType rxSignalGroupArrayId = rxSignalGroupPtr->SignalGroupArrayId;
                    if (COM_UNUSED_COM_SIGNALGROUPARRYID != rxSignalGroupArrayId)
                    {
                        uint8*       rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
                        const uint8* rxIpduInitValuePtr   = Com_ConfigStd->RxIpduInitValuePtr[RxMainfunctionId];
                        const Com_SignalGroupArrayType* signalGroupArrayPtr =
                            &Com_ConfigStd->SignalGroupArrayPtr[rxSignalGroupArrayId];
                        uint16 iPduStartBufferId = signalGroupArrayPtr->IPduStartBufIndex;
                        SchM_Enter_Com_Context();
                        (void)IStdLib_MemCpy(
                            &rxIpduRuntimeBuffPtr[iPduStartBufferId],
                            &rxIpduInitValuePtr[iPduStartBufferId],
                            signalGroupArrayPtr->SignalGroupArrayLength);
                        SchM_Exit_Com_Context();
                    }
#endif
                    /*replace the signal group(include all group signals) value with init value*/
                    groupSignalStartId = rxSignalGroupPtr->GroupSignalRefStartId;
                    for (; groupSignalStartId < groupSignalEndId; ++groupSignalStartId)
                    {
                        rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[groupSignalStartId];
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                        substituteBufId =
                            Com_ConfigStd->SignalSubstitueteValuePtr->RxGrpSigSubstituteValueId[substituteStartId];
                        substituteStartId++;
#endif
                        Com_RxDataTimeOutActionHandle(
                            RxIpduStatePtr,
                            RxIpduPtr,
                            timeoutAction,
                            rxGroupSignalPtr,
                            FALSE,
                            substituteBufId);
                    }
                }
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
                const Com_RxTimeoutNotificationType* rxSigGrpDataTimeoutNotificationCfg =
                    Com_CfgSignalTimeoutPtr->RxSigGrpTimeoutNotificationCfg;
                if (NULL_PTR != rxSigGrpDataTimeoutNotificationCfg[counter])
                {
                    rxSigGrpDataTimeoutNotificationCfg[counter]();
                }
#endif
                Com_RxSignalGroupTimeOutCnt[counter] = rxSignalGroupTimeoutValuePtr[counter];
            }
        }
    }
    COM_NOUSED(RxMainfunctionId);
    COM_NOUSED(RxIpduStatePtr);
}
#endif
/**
 * @brief called by Com_IndicationProcess.
 *        unpack the rx source description signal to gateway buffer.
 *
 */
#if (0u < COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER) && (COM_MAX_GWMAPPING_NUMBER > 0u)
/* PRQA S 3673 ++*/ /* VL_QAC_3673 */
COM_LOCAL void Com_SourceSignalUnPack(
    const Com_GwSourceSignalType* GwSourceSignalPtr,
    uint8*                        RxIpduRuntimeBuffPtr,
    uint16                        RxDynSignalLength,
    Com_RxIpduBufIdType           IpduBufferId)
/* PRQA S 3673 -- */
{
#if (0u < COM_MAX_RXIPDUBUFF_SIZE)
    uint16         signalBufferId = GwSourceSignalPtr->GWSignalBufferId;
    Com_SignalType signalType     = GwSourceSignalPtr->ComSignalType;

#if (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE)
    if ((signalType != COM_UINT8_N) && (signalType != COM_UINT8_DYN))
#endif
    {
        const uint8* rxIpduRuntimeBuff = &RxIpduRuntimeBuffPtr[IpduBufferId];
        uint64       sourceSignalValue = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
            signalType,
#endif
            COM_RX_DESCRIPTION,
            GwSourceSignalPtr,
            rxIpduRuntimeBuff);
        SchM_Enter_Com_MultiCore_Lock();
        Com_UpdateGWSignalBuffer(sourceSignalValue, signalBufferId, signalType);
        SchM_Exit_Com_MultiCore_Lock();
    }
#if ((STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE)) \
    && (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)
    else
    {
        Com_SignalPositionType sigLsbBytePos = GwSourceSignalPtr->SigLsbBytePos;
        Com_RxIpduBufIdType    ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)sigLsbBytePos;
        /* COM_UINT8_DYN or COM_UINT8_N */
        uint16 signalLength = (COM_UINT8_N == signalType) ? GwSourceSignalPtr->ComSignalLength : RxDynSignalLength;
        SchM_Enter_Com_MultiCore_Lock();
        SchM_Enter_Com_Context();
#if (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
            /* PRQA S 1340 ++ */ /* VL_Com_ConstPointer */
            (void)IStdLib_MemCpy(
                &Com_GWSignal8BitBuff[signalBufferId],
                (const uint16*)RxDynSignalLength,
                sizeof(RxDynSignalLength));
            /* PRQA S 1340 -- */
            signalBufferId += (uint16)(sizeof(RxDynSignalLength));
        }
#endif
        /* PRQA S 2934 ++ */ /* VL_Com_NullPtrValue */
        (void)IStdLib_MemCpy(&Com_GWSignal8BitBuff[signalBufferId], &RxIpduRuntimeBuffPtr[ipduBufferPos], signalLength);
        /* PRQA S 2934 -- */
        SchM_Exit_Com_Context();
        SchM_Exit_Com_MultiCore_Lock();
    }
#endif
#endif

#if COM_MAX_GWMAPPING_NUMBER > 0u
    Com_GwMappingIdType gwMappingId = GwSourceSignalPtr->GwMappingId;
    if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
    {
        Com_GwUpdateDestinationRequest(gwMappingId, TRUE);
    }
#endif

    COM_NOUSED(RxDynSignalLength);
    COM_NOUSED(IpduBufferId);
    return;
}
#endif

/**
 * @brief called by Com_IndicationProcess.
 *        unpack the rx signal and notification up layer.
 *
 */
#if (0u < COM_MAX_RXSIGNAL_NUMBER)
COM_LOCAL void Com_SignalRxIndication(
    const Com_RxSignalType* RxSignalPtr,
    uint8*                  RxIpduRuntimeBuffPtr,
    uint16                  RxDynSignalLength,
    Com_RxIpduBufIdType     IpduBufferId)
{
#if (0u < COM_MAX_RXIPDUBUFF_SIZE)
    Com_SignalType signalType = RxSignalPtr->ComSignalType;
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    if ((signalType != COM_UINT8_N) && (signalType != COM_UINT8_DYN))
#endif
    {
        Com_RxSignalHandle(RxSignalPtr, RxIpduRuntimeBuffPtr);
    }
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    else
    {
        /* COM_UINT8_DYN or COM_UINT8_N */
        uint16 signalLength = (COM_UINT8_N == signalType) ? RxSignalPtr->ComSignalLength : RxDynSignalLength;
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE) && (COM_MAX_GWMAPPING_NUMBER > 0u)
        uint16              gwSignalBufferId = RxSignalPtr->GWSignalBufferId;
        Com_RxIpduBufIdType ipduBufferPos    = IpduBufferId + (Com_RxIpduBufIdType)(RxSignalPtr->SigLsbBytePos);
        /*the rx signal need to be gateway,then store the value to gateway buffer*/
        if (COM_UNUSED_UINT16 != gwSignalBufferId)
        {
            SchM_Enter_Com_MultiCore_Lock();
            SchM_Enter_Com_Context();
            if (COM_UINT8_DYN == signalType)
            {
                /* PRQA S 1340 ++ */ /* VL_Com_ConstPointer */
                (void)IStdLib_MemCpy(
                    &Com_GWSignal8BitBuff[gwSignalBufferId],
                    &RxDynSignalLength,
                    sizeof(RxDynSignalLength));
                /* PRQA S 1340 -- */
                gwSignalBufferId += (uint16)(sizeof(RxDynSignalLength));
            }
            (void)IStdLib_MemCpy(
                &Com_GWSignal8BitBuff[gwSignalBufferId],
                &RxIpduRuntimeBuffPtr[ipduBufferPos],
                signalLength);
            SchM_Exit_Com_Context();
            SchM_Exit_Com_MultiCore_Lock();
        }
#endif
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == signalType)
        {
            Com_Rx8NSignalHandle(RxSignalPtr, signalLength, IpduBufferId);
        }
#endif
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
            Com_Rx8DYNSignalHandle(RxSignalPtr, signalLength, IpduBufferId);
        }
#endif
    }
#endif
#endif

#if COM_MAX_GWMAPPING_NUMBER > 0u
    Com_GwMappingIdType gwMappingId = RxSignalPtr->GwMappingId;
    if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
    {
        Com_GwUpdateDestinationRequest(gwMappingId, TRUE);
    }
#endif
    COM_NOUSED(IpduBufferId);
    COM_NOUSED(RxDynSignalLength);
}
#endif

/**
 * @brief called by Com_SignalRxIndication.
 *        unpack the rx signal(signal type is COM_UINT8_N) and notification up layer.
 *
 */
#if ((0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDUBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE))
COM_LOCAL void
    Com_Rx8NSignalHandle(const Com_RxSignalType* RxSignalPtr, uint16 ComSignalLength, Com_RxIpduBufIdType IpduBufferId)
{
    Com_RxIpduBufIdType    ipduBufferPos        = IpduBufferId + (Com_RxIpduBufIdType)(RxSignalPtr->SigLsbBytePos);
    Com_MainFunctionIdType rxMainfunctionId     = RxSignalPtr->RxMainfunctionId;
    const uint8*           rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[rxMainfunctionId];
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    uint16    initValueId              = RxSignalPtr->SignalInitValueId;
    PduIdType rxIpduId                 = RxSignalPtr->IpduRefIndex;
    uint16    partitionId              = Com_CfgRxIPduPtr[rxIpduId].IpduPartitionId;
    uint8*    signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[partitionId];
#endif
    /*the rx signal configuration invalid value*/
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    boolean                   ret             = TRUE;
    Com_RxSignalInvalidIdType signalInvalidId = RxSignalPtr->SignalInvalidRefId;
    if (COM_UNUSED_RXSIGNALINVALIDID != signalInvalidId)
    {
        const Com_SignalInvalidValueType* signalInvalidValue = Com_ConfigStd->SignalInvalidValuePtr;
        const uint16 invalidValueId            = signalInvalidValue->RxSignalDataInvalidValueId[signalInvalidId];
        const uint8* signal8BitInvalidValuePtr = signalInvalidValue->Signal8BitInvalidValue;
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
        if (0
            == IStdLib_MemCmp(
                &signal8BitInvalidValuePtr[invalidValueId],
                &rxIpduRuntimeBuffPtr[ipduBufferPos],
                ComSignalLength))
#endif
        {
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
            if (COM_INVALID_ACTION_NOTIFY == signalInvalidValue->RxSignalDataInvalidAction[signalInvalidId])
            {
                if (NULL_PTR != signalInvalidValue->RxInvalidNotificationCfg[signalInvalidId])
                {
                    signalInvalidValue->RxInvalidNotificationCfg[signalInvalidId]();
                }
            }
            else
#endif
            {
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
                if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->FilterIndex)
                    || (COM_ALWAYS == RxSignalPtr->FilterAlgorithm))
#endif
                {
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
                    const uint8* signal8BitInitValuePtr = Com_CfgSignalInitValuePtr->Signal8BitInitValue[partitionId];
                    SchM_Enter_Com_Context();
                    (void)IStdLib_MemCpy(
                        &signal8BitRuntimeBuffPtr[initValueId],
                        &signal8BitInitValuePtr[initValueId],
                        ComSignalLength);
                    SchM_Exit_Com_Context();
#endif
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
                    if (NULL_PTR != RxSignalPtr->ComNotification)
                    {
                        RxSignalPtr->ComNotification();
                    }
#endif
                }
            }
            ret = FALSE;
        }
    }
    if (ret)
#endif
    {
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->FilterIndex) || (COM_ALWAYS == RxSignalPtr->FilterAlgorithm))
#endif
        {
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
            SchM_Enter_Com_Context();
            (void)IStdLib_MemCpy(
                &signal8BitRuntimeBuffPtr[initValueId],
                &rxIpduRuntimeBuffPtr[ipduBufferPos],
                ComSignalLength);
            SchM_Exit_Com_Context();
#endif
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif
        }
    }
    COM_NOUSED(ComSignalLength);
    return;
}
#endif
/**
 * @brief Unpack the rx signal(signal type is COM_UINT8_DYN) invalidate value.
 *
 */
#if (                                                                \
    (0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (0u < COM_MAX_RX_SIGNAL_8BIT_INVALIDVALUE_NUMBER))
COM_LOCAL boolean Com_Rx8DYNSignalInvalidHandle(
    const Com_RxSignalType* RxSignalPtr,
    uint16                  ComSignalLength,
    Com_RxIpduBufIdType     IpduBufferId)
{
    Com_MainFunctionIdType rxMainfunctionId = RxSignalPtr->RxMainfunctionId;
    Com_RxIpduBufIdType    ipduBufferPos    = IpduBufferId + (Com_RxIpduBufIdType)(RxSignalPtr->SigLsbBytePos);
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    uint16                      initValueId           = RxSignalPtr->SignalInitValueId;
    const uint8*                rxIpduRuntimeBuffPtr  = Com_RxIPduRuntimeBuff[rxMainfunctionId];
    PduIdType                   rxIpduId              = RxSignalPtr->IpduRefIndex;
    uint16                      ipduIdPerMainfunction = rxIpduId - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
    Com_RxIPduRunTimeStateType* rxIpduStatePtr = &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
    uint16                      partitionId    = Com_CfgRxIPduPtr[rxIpduId].IpduPartitionId;
    uint8*                      signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[partitionId];
#endif

    /*the rx signal configuration invalid value*/
    boolean                   ret             = TRUE;
    Com_RxSignalInvalidIdType signalInvalidId = RxSignalPtr->SignalInvalidRefId;
    if (COM_UNUSED_RXSIGNALINVALIDID != signalInvalidId)
    {
        const Com_SignalInvalidValueType* signalInvalidValue = Com_ConfigStd->SignalInvalidValuePtr;
        if (ComSignalLength == signalInvalidValue->RxSignalDataInvalidValueLength[signalInvalidId])
        {
            const uint16 invalidValueId            = signalInvalidValue->RxSignalDataInvalidValueId[signalInvalidId];
            const uint8* signal8BitInvalidValuePtr = signalInvalidValue->Signal8BitInvalidValue;
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
            if (0
                == IStdLib_MemCmp(
                    &signal8BitInvalidValuePtr[invalidValueId],
                    &rxIpduRuntimeBuffPtr[ipduBufferPos],
                    ComSignalLength))
#endif
            {
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
                if (COM_INVALID_ACTION_NOTIFY == signalInvalidValue->RxSignalDataInvalidAction[signalInvalidId])
                {
                    if (NULL_PTR != signalInvalidValue->RxInvalidNotificationCfg[signalInvalidId])
                    {
                        signalInvalidValue->RxInvalidNotificationCfg[signalInvalidId]();
                    }
                }
                else
#endif
                {
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
                    if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->FilterIndex)
                        || (COM_ALWAYS == RxSignalPtr->FilterAlgorithm))
#endif
                    {
                        uint16 initValueLength = RxSignalPtr->SignalDataInitValueLength;
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
                        const uint8* signal8BitInitValuePtr =
                            Com_CfgSignalInitValuePtr->Signal8BitInitValue[partitionId];
                        SchM_Enter_Com_Context();
                        (void)IStdLib_MemCpy(
                            &signal8BitRuntimeBuffPtr[initValueId],
                            &signal8BitInitValuePtr[initValueId],
                            initValueLength);
                        SchM_Exit_Com_Context();
#endif

                        /*change the rx Ipdu length to Init rx Ipdu length*/
                        uint16 rxIpduLength          = rxIpduStatePtr->RxIpduLength - ComSignalLength + initValueLength;
                        rxIpduStatePtr->RxIpduLength = rxIpduLength;
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
                        if (NULL_PTR != RxSignalPtr->ComNotification)
                        {
                            RxSignalPtr->ComNotification();
                        }
#endif
                    }
                }
                ret = FALSE;
            }
        }
    }

    return ret;
}
#endif
/**
 * @brief called by Com_SignalRxIndication.
 *        unpack the rx signal(signal type is COM_UINT8_DYN) and notification up layer.
 *
 */
#if (                                                                \
    (0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_Rx8DYNSignalHandle(
    const Com_RxSignalType* RxSignalPtr,
    uint16                  ComSignalLength,
    Com_RxIpduBufIdType     IpduBufferId)
{
    Com_RxIpduBufIdType ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)(RxSignalPtr->SigLsbBytePos);
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    uint16       initValueId              = RxSignalPtr->SignalInitValueId;
    const uint8* rxIpduRuntimeBuffPtr     = Com_RxIPduRuntimeBuff[RxSignalPtr->RxMainfunctionId];
    uint16       partitionId              = Com_CfgRxIPduPtr[RxSignalPtr->IpduRefIndex].IpduPartitionId;
    uint8*       signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[partitionId];
#endif

    /*the rx signal configuration invalid value*/
#if (0u < COM_MAX_RX_SIGNAL_8BIT_INVALIDVALUE_NUMBER)
    if (Com_Rx8DYNSignalInvalidHandle(RxSignalPtr, ComSignalLength, IpduBufferId))
#endif
    {
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->FilterIndex) || (COM_ALWAYS == RxSignalPtr->FilterAlgorithm))
#endif
        {
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
            SchM_Enter_Com_Context();
            (void)IStdLib_MemCpy(
                &signal8BitRuntimeBuffPtr[initValueId],
                &rxIpduRuntimeBuffPtr[ipduBufferPos],
                ComSignalLength);
            SchM_Exit_Com_Context();
#endif
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif
        }
    }
    COM_NOUSED(ComSignalLength);
    return;
}
#endif
/**
 * @brief called by Com_RxSignalHandle.
 *        rx signal(signal type isn't COM_UINT8_N or COM_UINT8_DYN) receive invalid value.
 *
 */
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
COM_LOCAL boolean Com_RxSignalInvalidHandle(
    const Com_RxSignalType* RxSignalPtr,
    Com_SignalType          SignalType,
    uint64*                 UnPackValue,
    uint16                  PartitionId)
{
    Com_RxSignalInvalidIdType signalInvalidId = RxSignalPtr->SignalInvalidRefId;
    boolean                   needFilter      = TRUE;
    if (COM_UNUSED_RXSIGNALINVALIDID != signalInvalidId)
    {
        const Com_SignalInvalidValueType* signalInvalidValue = Com_ConfigStd->SignalInvalidValuePtr;
        uint16 invalidValueId = signalInvalidValue->RxSignalDataInvalidValueId[signalInvalidId];
        if (Com_SignalInvalidateHandle(SignalType, invalidValueId, *UnPackValue))
        {
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
            if (COM_INVALID_ACTION_NOTIFY == signalInvalidValue->RxSignalDataInvalidAction[signalInvalidId])
            {
                if (NULL_PTR != signalInvalidValue->RxInvalidNotificationCfg[signalInvalidId])
                {
                    signalInvalidValue->RxInvalidNotificationCfg[signalInvalidId]();
                }
                needFilter = FALSE;
            }
            else
#endif
            {
                /*replace signal runtime buffer, with init value. */
                *UnPackValue = Com_RxSignalReplaceHanlde(SignalType, RxSignalPtr->SignalInitValueId, PartitionId);
            }
        }
    }
    return needFilter;
}
#endif
/**
 * @brief called by Com_SignalRxIndication.
 *        unpack the rx signal(signal type isn't COM_UINT8_N or COM_UINT8_DYN) and notification up layer.
 *
 */
#if ((0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_RXIPDUBUFF_SIZE))
/* PRQA S 3673 ++*/ /* VL_QAC_3673 */
COM_LOCAL void Com_RxSignalHandle(const Com_RxSignalType* RxSignalPtr, uint8* RxIpduRuntimeBuffPtr)
/* PRQA S 3673 -- */
{
    uint16                initValueId = RxSignalPtr->SignalInitValueId;
    Com_SignalType        signalType  = RxSignalPtr->ComSignalType;
    const Com_RxIPduType* rxIpduPtr   = &Com_CfgRxIPduPtr[RxSignalPtr->IpduRefIndex];
    /* unpack the signal value from rx ipdu buffer, and do B-End and L-End exchanging,Flag Extension
       and return the result as an uint64 value to variable 'unPackValue'. */
    uint16              partitionId       = rxIpduPtr->IpduPartitionId;
    Com_RxIpduBufIdType ipduBufferIndex   = rxIpduPtr->RxIPduBufIndex;
    const uint8*        rxIpduRuntimeBuff = &RxIpduRuntimeBuffPtr[ipduBufferIndex];
    uint64              unPackValue       = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
        signalType,
#endif
        COM_RX_SIGNAL,
        RxSignalPtr,
        rxIpduRuntimeBuff);
/* update signal value into GW signal buffer */
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
    uint16 gwSignalBufferId = RxSignalPtr->GWSignalBufferId;
    SchM_Enter_Com_MultiCore_Lock();
    Com_UpdateGWSignalBuffer(unPackValue, gwSignalBufferId, signalType);
    SchM_Exit_Com_MultiCore_Lock();
#endif

/*the rx signal configuration invalid value*/
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    if (Com_RxSignalInvalidHandle(
            RxSignalPtr,
            signalType,
            &unPackValue,
            partitionId)) /* PRQA S 2991 */ /* VL_Com_ResultAlwaysTrue */
#endif
    {
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        boolean filterPass = Com_RxSignalFilter(
            signalType,
            RxSignalPtr->FilterAlgorithm,
            (Com_SignalIdType)RxSignalPtr->FilterIndex,
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
            initValueId,
            partitionId,
#endif
            unPackValue);

        if (filterPass)
#endif
        {
            /* update signal value into signal runtime buffer */
            Com_RxSignalBuffHandle(unPackValue, signalType, initValueId, partitionId);
/* if this signal has normal rx notification function configured,
   invoke this function. */
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif
        }
    }
    return;
}
#endif

/**
 * @brief called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 *        judge the rx (group) signal value is invalid value or not.
 *
 */
#if ((STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE))
COM_LOCAL boolean Com_SignalInvalidateHandle(Com_SignalType SignalType, uint16 InvalidValueId, uint64 Value)
{
    boolean                           ret                         = FALSE;
    const Com_SignalInvalidValueType* rxSignalDataInvalidValuePtr = Com_ConfigStd->SignalInvalidValuePtr;
#if (0u < COM_MAX_SIGNAL_BOOL_INVALID_SIZE)
    const boolean* signalBoolInvalidValuePtr = rxSignalDataInvalidValuePtr->SignalBoolInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
    const uint8* signal8BitInvalidValuePtr = rxSignalDataInvalidValuePtr->Signal8BitInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_16BIT_INVALID_SIZE)
    const uint16* signalBit16InvalidValuePtr = rxSignalDataInvalidValuePtr->SignalBit16InvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_32BIT_INVALID_SIZE)
    const uint32* signal32BitInvalidValuePtr = rxSignalDataInvalidValuePtr->Signal32BitInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_64BIT_INVALID_SIZE)
    const uint64* signal64BitInvalidValuePtr = rxSignalDataInvalidValuePtr->Signal64BitInvalidValue;
#endif
    /* It have judged the InvalidValueId is not null, so the ValuePtr may be not to judge*/
    switch (SignalType)
    {
#if (0u < COM_MAX_SIGNAL_BOOL_INVALID_SIZE)
    case COM_BOOLEAN:
        if (((boolean)Value) == signalBoolInvalidValuePtr[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
    case COM_SINT8:
    case COM_UINT8:
        if (((uint8)Value) == signal8BitInvalidValuePtr[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_MAX_SIGNAL_16BIT_INVALID_SIZE)
    case COM_SINT16:
    case COM_UINT16:
        if (((uint16)Value) == signalBit16InvalidValuePtr[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_MAX_SIGNAL_32BIT_INVALID_SIZE)
    case COM_SINT32:
    case COM_UINT32:
    case COM_FLOAT32:
        if (((uint32)Value) == signal32BitInvalidValuePtr[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
#if (0u < COM_MAX_SIGNAL_64BIT_INVALID_SIZE)
    case COM_SINT64:
    case COM_UINT64:
    case COM_FLOAT64:
        if (Value == signal64BitInvalidValuePtr[InvalidValueId])
        {
            ret = TRUE;
        }
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    COM_NOUSED(InvalidValueId);
    COM_NOUSED(Value);
    return ret;
}
#endif

/**
 * @brief called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification, Com_RxSignalGroupFilter.
 *        get the rx (group) signal's init value.
 *
 */
#if (                                                                                          \
    ((0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)) \
    || ((0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)))
COM_LOCAL uint64 Com_RxSignalReplaceHanlde(Com_SignalType SignalType, uint16 InitValueId, uint16 PartitionId)
{
    uint64                         unPackValue        = COM_UNUSED_UINT64;
    const Com_SignalInitValueType* signalInitValuePtr = Com_CfgSignalInitValuePtr;
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE) || (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE) || (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE) \
    || (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE) || (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    switch (SignalType)
    {
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        /* PRQA S 4304 ++ */ /* VL_Com_BooleanType */
        unPackValue = (uint64)signalInitValuePtr->SignalBoolInitValue[PartitionId][InitValueId];
        /* PRQA S 4304 -- */
        break;
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    case COM_SINT8:
    case COM_UINT8:
        unPackValue = (uint64)signalInitValuePtr->Signal8BitInitValue[PartitionId][InitValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    case COM_SINT16:
    case COM_UINT16:
        unPackValue = (uint64)signalInitValuePtr->SignalBit16InitValue[PartitionId][InitValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    case COM_SINT32:
    case COM_UINT32:
    case COM_FLOAT32:
        unPackValue = (uint64)signalInitValuePtr->Signal32BitInitValue[PartitionId][InitValueId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    case COM_SINT64:
    case COM_UINT64:
    case COM_FLOAT64:
        unPackValue = signalInitValuePtr->Signal64BitInitValue[PartitionId][InitValueId];
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
#endif
    COM_NOUSED(InitValueId);
    return unPackValue;
}
#endif

/**
 * @brief called by Com_RxSignalHandle,
 *        Com_InvalidSignalReplaceAndNotification.
 *        update the rx signal value buffer.
 *
 */
#if (0u < COM_MAX_RXSIGNAL_NUMBER) || (0u < COM_MAX_RXGROUPSIGNAL_NUMBER)
COM_LOCAL void Com_RxSignalBuffHandle(uint64 Value, Com_SignalType SignalType, uint16 InitValueId, uint16 PartitionId)
{
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    boolean* signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[PartitionId];
#endif
    switch (SignalType)
    {
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        signalBoolRuntimeBuffPtr[InitValueId] = (Value == 1ULL) ? TRUE : FALSE;
        break;
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    case COM_SINT8:
    case COM_UINT8:
        signal8BitRuntimeBuffPtr[InitValueId] = (uint8)Value;
        break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    case COM_SINT16:
    case COM_UINT16:
        signalBit16RuntimeBuffPtr[InitValueId] = (uint16)Value;
        break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    case COM_SINT32:
    case COM_UINT32:
    case COM_FLOAT32:
        signal32BitRuntimeBuffPtr[InitValueId] = (uint32)Value;
        break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    case COM_SINT64:
    case COM_UINT64:
    case COM_FLOAT64:
        signal64BitRuntimeBuffPtr[InitValueId] = (uint64)Value;
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    COM_NOUSED(Value);
    COM_NOUSED(InitValueId);
    return;
}
#endif
/**
 * @brief called by Com_RxSignalHandle,Com_SignalGroupRxIndication.
 *        update the gateway signal value buffer of the rx signal.
 *
 */
#if (((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER)) || (0u < COM_MAX_RXSIGNAL_NUMBER)) \
    && (COM_MAX_GWMAPPING_NUMBER > 0u)
COM_LOCAL void Com_UpdateGWSignalBuffer(uint64 Value, uint16 GWSignalBufferId, Com_SignalType SignalType)
{
    /*the rx signal need gateway,store signal value in gateway buffer*/
    if (COM_UNUSED_UINT16 != GWSignalBufferId)
    {
        switch (SignalType)
        {
#if (0u < COM_MAX_GWSIGNAL_BOOLBUFF_SIZE)
        case COM_BOOLEAN:
            Com_GWSignalBoolBuff[GWSignalBufferId] = (boolean)(Value != 0ULL);
            break;
#endif
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)
        case COM_SINT8:
        case COM_UINT8:
            Com_GWSignal8BitBuff[GWSignalBufferId] = (uint8)Value;
            break;
#endif
#if (0u < COM_MAX_GWSIGNAL_16BITBUFF_SIZE)
        case COM_SINT16:
        case COM_UINT16:
            Com_GWSignalBit16Buff[GWSignalBufferId] = (uint16)Value;
            break;
#endif
#if (0u < COM_MAX_GWSIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            Com_GWSignal32BitBuff[GWSignalBufferId] = (uint32)Value;
            break;
#endif
#if (0u < COM_MAX_GWSIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            Com_GWSignal64BitBuff[GWSignalBufferId] = (uint64)Value;
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
    }
    COM_NOUSED(Value);
    return;
}
#endif

/**
 * @brief called by Com_IndicationProcess.
 *        handle signal group RxIndication of one rx pdu.
 *
 */
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER))
COM_LOCAL void Com_SignalGroupRxIndication(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    uint16                       RxDynSignalLength,
    Com_RxIpduBufIdType          IpduBufferId,
    uint16                       PartitionId)
{
    boolean normalRxIndication = FALSE;
    /*if the signal group have invalid signal or not, and update gwGroupSignal buffer*/
    boolean signalGroupInvalid = Com_SignalGroupInvalidCalculate(RxSignalGroupPtr, IpduBufferId, RxDynSignalLength);
    /*the signal group is invalid if at least one group signal is invalid*/
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    if (signalGroupInvalid)
    {
#if (STD_ON == COM_RX_SIG_GROUP_INVALID_DATA_ACTION_NOTIFY_ENABLE)
        if (COM_INVALID_ACTION_NOTIFY == RxSignalGroupPtr->ComDataInvalidAction)
        {
            if (NULL_PTR != RxSignalGroupPtr->ComInvalidNotification)
            {
                RxSignalGroupPtr->ComInvalidNotification();
            }
        }
#endif
        if (COM_INVALID_ACTION_REPLACE == RxSignalGroupPtr->ComDataInvalidAction)
        {
            normalRxIndication = TRUE;
/*replace the rx pdu buffer for signal group array*/
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (0u < COM_MAX_RXIPDUBUFF_SIZE))
            Com_SignalGroupArryIdType signalGroupArrayId = RxSignalGroupPtr->SignalGroupArrayId;
            if (COM_UNUSED_COM_SIGNALGROUPARRYID != signalGroupArrayId)
            {
                const Com_SignalGroupArrayType* signalGroupArrayPtr =
                    &Com_ConfigStd->SignalGroupArrayPtr[signalGroupArrayId];
                Com_MainFunctionIdType rxMainfunctionId     = RxSignalGroupPtr->RxMainfunctionId;
                uint8*                 rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[rxMainfunctionId];
                const uint8*           rxIpduInitValuePtr   = Com_ConfigStd->RxIpduInitValuePtr[rxMainfunctionId];
                SchM_Enter_Com_Context();
                (void)IStdLib_MemCpy(
                    &rxIpduRuntimeBuffPtr[signalGroupArrayPtr->IPduStartBufIndex],
                    &rxIpduInitValuePtr[signalGroupArrayPtr->IPduStartBufIndex],
                    signalGroupArrayPtr->SignalGroupArrayLength);
                SchM_Exit_Com_Context();
            }
#endif
        }
    }
#endif
    if (!signalGroupInvalid || normalRxIndication)
    {
#if (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
        if (Com_RxSignalGroupFilter(RxSignalGroupPtr, signalGroupInvalid, IpduBufferId, PartitionId))
#endif
        {
            Com_RxGroupSignalIdType groupSignalEndId   = RxSignalGroupPtr->GroupSignalRefEndId;
            Com_RxGroupSignalIdType groupSignalStartId = RxSignalGroupPtr->GroupSignalRefStartId;
            /* update all group signal init value into group signal runtime buffer */
            for (; groupSignalStartId < groupSignalEndId; ++groupSignalStartId)
            {
                Com_RxGroupSignalBuffHanlde(signalGroupInvalid, groupSignalStartId, IpduBufferId, RxDynSignalLength);
            }
            /* if this group signal has normal rx notification function configured,
            invoke this function. */
#if (STD_ON == COM_RX_SIG_GROUP_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalGroupPtr->ComNotification)
            {
                RxSignalGroupPtr->ComNotification();
            }
#endif
        }
    }
    COM_NOUSED(PartitionId);

    return;
}
#endif

/**
 * @brief called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 *        update the rx group signal value buffer.
 *
 */
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER))
/******************************************************************************/
COM_LOCAL void Com_RxGroupSignalBuffHanlde(
    boolean                 InvalidSignalGroup,
    Com_RxGroupSignalIdType RxGroupId,
    Com_RxIpduBufIdType     IpduBufferId,
    uint16                  RxDynSignalLength)
{
    const Com_RxGroupSignalType* rxGroupSignalPtr     = &Com_CfgRxGroupSignalPtr[RxGroupId];
    uint16                       initValueId          = rxGroupSignalPtr->SignalInitValueId;
    Com_SignalType               signalType           = rxGroupSignalPtr->ComSignalType;
    Com_RxSignalGroupIdType      signalGroupId        = rxGroupSignalPtr->ComSignalGroupRef;
    const Com_RxSignalGroupType* rxSignalGroupPtr     = &Com_CfgRxSignalGroupPtr[signalGroupId];
    Com_MainFunctionIdType       rxMainfunctionId     = rxSignalGroupPtr->RxMainfunctionId;
    PduIdType                    rxIpduId             = rxSignalGroupPtr->IpduRefIndex;
    const Com_RxIPduType*        rxIpduPtr            = &Com_CfgRxIPduPtr[rxIpduId];
    const uint8*                 rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[rxMainfunctionId];
    uint16                       partitionId          = rxIpduPtr->IpduPartitionId;

    /*rx signal group is invalid,use the init value update the rx group signal buffer*/
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    if (InvalidSignalGroup)
    {
        uint16 grpSigLen = 0u;
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == signalType)
        {
            grpSigLen = rxGroupSignalPtr->ComSignalLength;
        }
#endif
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
            uint16 ipduIdPerMainfunction = rxIpduId - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
            Com_RxIPduRunTimeStateType* rxIpduStatePtr =
                &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
            grpSigLen = rxGroupSignalPtr->SignalDataInitValueLength;
            /*change the rx Ipdu length to Init rx Ipdu length*/
            uint16 rxIpduMinLength       = (uint16)rxIpduPtr->IPduNoDynSignalLength;
            rxIpduStatePtr->RxIpduLength = rxIpduMinLength + grpSigLen;
        }
#endif
#endif
        Com_InitSignalBuffer(signalType, initValueId, grpSigLen, partitionId);
    }
    /*rx signal group is valid,use the really rx value update the rx group signal buffer*/
    else
#endif
    {
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if ((COM_UINT8_N != signalType) && (COM_UINT8_DYN != signalType))
#endif
        {
            uint64 value = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
                signalType,
#endif
                COM_RX_GROUP_SIGNAL,
                rxGroupSignalPtr,
                &rxIpduRuntimeBuffPtr[IpduBufferId]);
            Com_RxSignalBuffHandle(value, signalType, initValueId, partitionId);
        }
#if ((0u < COM_MAX_SIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_RXIPDUBUFF_SIZE))
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        else
        {
            Com_RxIpduBufIdType ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)(rxGroupSignalPtr->SigLsbBytePos);
            uint16 grpSigLength = (COM_UINT8_N == signalType) ? rxGroupSignalPtr->ComSignalLength : RxDynSignalLength;
            uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[partitionId];
            SchM_Enter_Com_Context();
            (void)IStdLib_MemCpy(
                &signal8BitRuntimeBuffPtr[initValueId],
                &rxIpduRuntimeBuffPtr[ipduBufferPos],
                grpSigLength);
            SchM_Exit_Com_Context();
        }
#endif
#endif
    }
    COM_NOUSED(RxDynSignalLength);
    COM_NOUSED(InvalidSignalGroup);
    return;
}
#endif

/**
 * @brief called by Com_TpPduInvalidAction.
 *        rx signal value replace,and notification.
 *
 */
#if ((STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE))
COM_LOCAL void Com_InvalidSignalReplaceAndNotification(const Com_RxSignalType* RxSignalPtr, uint16 PartitionId)
{
    uint16         initValueId = RxSignalPtr->SignalInitValueId;
    Com_SignalType signalType  = RxSignalPtr->ComSignalType;

#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    if ((COM_UINT8_N != signalType) && (COM_UINT8_DYN != signalType))
#endif
    {
        /*replace signal runtime buffer, with init value. */
        uint64 initValue = Com_RxSignalReplaceHanlde(signalType, initValueId, PartitionId);
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        if (Com_RxSignalFilter(
                signalType,
                RxSignalPtr->FilterAlgorithm,
                (Com_SignalIdType)RxSignalPtr->FilterIndex,
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                initValueId,
                PartitionId,
#endif
                initValue))
#endif
        {
            /* update signal value into signal runtime buffer */
            Com_RxSignalBuffHandle(initValue, signalType, initValueId, PartitionId);
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif
        }
    }
    /*COM_UINT8_N or COM_UINT8_DYN*/
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    else
    {
#if (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
        if ((COM_UNUSED_RXSIGNALFILTERID == RxSignalPtr->FilterIndex) || (COM_ALWAYS == RxSignalPtr->FilterAlgorithm))
#endif
        {
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
            uint16 signalLength =
                (COM_UINT8_N == signalType) ? RxSignalPtr->ComSignalLength : RxSignalPtr->SignalDataInitValueLength;
            uint8*       signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
            const uint8* signal8BitInitValuePtr   = Com_CfgSignalInitValuePtr->Signal8BitInitValue[PartitionId];
            SchM_Enter_Com_Context();
            (void)IStdLib_MemCpy(
                &signal8BitRuntimeBuffPtr[initValueId],
                &signal8BitInitValuePtr[initValueId],
                signalLength);
            SchM_Exit_Com_Context();
#endif
#if (STD_ON == COM_RX_SIGNAL_NOTIFICATION_ENABLE)
            if (NULL_PTR != RxSignalPtr->ComNotification)
            {
                RxSignalPtr->ComNotification();
            }
#endif
        }
    }
#endif

    return;
}
#endif

/**
 * @brief called by Com_RxSignalHandle,Com_InvalidSignalReplaceAndNotification,Com_RxSignalGroupFilter.
 *        calculate the rx signal value is filter out or not.
 *
 */
#if (                                                                                 \
    ((0u < COM_MAX_RXSIGNAL_NUMBER) && (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)) \
    || ((0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)))
COM_LOCAL boolean Com_RxSignalFilter(
    Com_SignalType          SignalType,
    Com_FilterAlgorithmType FilterType,
    Com_SignalIdType        FilterIndex,
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint16 SignalBufferId,
    uint16 PartitionId,
#endif
    uint64 NewSignalValue)
{
    boolean ret = TRUE;
    if (COM_FILTER_NONE != FilterType)
    {
        const Com_TxModeAndSignalFilterPtrType* rxFilter = Com_CfgTxModeAndSignalFilterPtr;
        Com_FilterMaskType                      filterMask;
        Com_FilterXType                         filterX;
        Com_FilterMaxType                       filterMax;
        Com_FilterMinType                       filterMin;
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
        Com_SignalIdType filterOffset;
        Com_SignalIdType filterPeriod;
#endif
        ret = FALSE;
        switch (FilterType)
        {
        case COM_ALWAYS:
            ret = TRUE;
            break;
#if (COM_MAX_RXMASKNEWDIFFERX_NUMBER > 0u)
        case COM_MASKED_NEW_DIFFERS_X:
            filterMask = rxFilter->RxMaskedNewDiffersX[FilterIndex].FilterMask;
            filterX    = rxFilter->RxMaskedNewDiffersX[FilterIndex].FilterX;
            if ((NewSignalValue & filterMask) != filterX)
            {
                ret = TRUE;
            }
            break;
#endif
#if (COM_MAX_RXMASKNEWEQUALSX_NUMBER > 0u)
        case COM_MASKED_NEW_EQUALS_X:
            filterMask = rxFilter->RxMaskedNewEqualsX[FilterIndex].FilterMask;
            filterX    = rxFilter->RxMaskedNewEqualsX[FilterIndex].FilterX;
            if ((NewSignalValue & filterMask) == filterX)
            {
                ret = TRUE;
            }
            break;
#endif
            /*COM_BOOLEAN not support WITHIN and WITHOUT*/
#if (COM_MAX_RXNEWISWITHIN_NUMBER > 0u)
        case COM_NEW_IS_WITHIN:
            filterMax = rxFilter->RxNewIsWithin[FilterIndex].FilterMax;
            filterMin = rxFilter->RxNewIsWithin[FilterIndex].FilterMin;
            ret       = Com_NewIsWInWOtFilterCalculate(SignalType, NewSignalValue, filterMax, filterMin, TRUE);
            break;
#endif
            /*COM_BOOLEAN not support WITHIN and WITHOUT*/
#if (COM_MAX_RXNEWISOUTSIDE_NUMBER > 0u)
        case COM_NEW_IS_OUTSIDE:
            filterMax = rxFilter->RxNewIsOutside[FilterIndex].FilterMax;
            filterMin = rxFilter->RxNewIsOutside[FilterIndex].FilterMin;
            ret       = Com_NewIsWInWOtFilterCalculate(SignalType, NewSignalValue, filterMax, filterMin, FALSE);
            break;
#endif
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
        case COM_ONE_EVERY_N:
            filterOffset = rxFilter->FilterOneEveryN[FilterIndex].FilterOffset;
            filterPeriod = rxFilter->FilterOneEveryN[FilterIndex].FilterPeriod;
            if (filterPeriod == Com_OneEveryNcnt[FilterIndex])
            {
                Com_OneEveryNcnt[FilterIndex] = 0u;
            }
            if (filterOffset == Com_OneEveryNcnt[FilterIndex])
            {
                ret = TRUE;
            }
            Com_OneEveryNcnt[FilterIndex]++;
            break;
#endif
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
        case COM_MASKED_NEW_DIFFERS_MASKED_OLD:
            filterMask = rxFilter->RxMaskedNewDiffersMaskedOld[FilterIndex];
            if (Com_MaskNewDifferMaskOldTimeOut[FilterIndex])
            {
                ret                                          = TRUE;
                Com_MaskNewDifferMaskOldTimeOut[FilterIndex] = FALSE;
            }
            else
            {
                ret = Com_MaskedNewDiffersMaskedOldFilterCalculate(
                    SignalType,
                    NewSignalValue,
                    SignalBufferId,
                    filterMask,
                    PartitionId);
            }
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
    }
    return ret;
}
#endif

/**
 * @brief called by Com_SignalGroupInvalidCalculate.
 *        Uint8_Dyn/Uint8_N Group Signal Invalid Calculate.
 *
 */
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER))
#if ((STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_Uint8NDynGroupSignalInvalidCalculate(
    uint16                       ComSignalLength,
    const Com_RxGroupSignalType* RxGroupSignalPtr,
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    boolean* SignalGroupInvalid,
#endif
    Com_RxIpduBufIdType    IpduBufferId,
    Com_MainFunctionIdType RxMainfunctionId)
{
    Com_RxIpduBufIdType ipduBufferPos = IpduBufferId + (Com_RxIpduBufIdType)RxGroupSignalPtr->SigLsbBytePos;
    Com_SignalType      signalType    = RxGroupSignalPtr->ComSignalType;
    /*the group signal need gateway,store group signal value in gateway buffer*/
    const uint8* rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
    uint16 gwSignalBufferId = RxGroupSignalPtr->GWSignalBufferId;
    if (COM_UNUSED_UINT16 != gwSignalBufferId)
    {
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)
        SchM_Enter_Com_MultiCore_Lock();
        SchM_Enter_Com_Context();
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
            (void)IStdLib_MemCpy(&Com_GWSignal8BitBuff[gwSignalBufferId], &ComSignalLength, sizeof(ComSignalLength));
            gwSignalBufferId += sizeof(ComSignalLength);
        }
#endif
        (void)IStdLib_MemCpy(
            &Com_GWSignal8BitBuff[gwSignalBufferId],
            &rxIpduRuntimeBuffPtr[ipduBufferPos],
            ComSignalLength);
        SchM_Exit_Com_Context();
        SchM_Exit_Com_MultiCore_Lock();
#endif
    }
#endif

#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
    if (((COM_UINT8_DYN == signalType) && (ComSignalLength == RxGroupSignalPtr->ComSignalDataInvalidValueLength))
        || (COM_UINT8_N == signalType))
#endif
    {
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
        uint16 invalidValueId = RxGroupSignalPtr->ComSignalInvalidBuffRefId;
        if ((COM_UNUSED_UINT16 != invalidValueId) && (!(*SignalGroupInvalid)))
        {
            const uint8* signalInvalidValue = Com_ConfigStd->SignalInvalidValuePtr->Signal8BitInvalidValue;
            if (0
                == IStdLib_MemCmp(
                    &signalInvalidValue[invalidValueId],
                    &rxIpduRuntimeBuffPtr[ipduBufferPos],
                    ComSignalLength))
            {
                *SignalGroupInvalid = TRUE;
            }
        }
#endif
    }
#endif
    return;
}
#endif

/**
 * @brief called by Com_SignalGroupRxIndication.
 *        Signal Group Invalid Calculate.
 *
 */
COM_LOCAL boolean Com_SignalGroupInvalidCalculate(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    Com_RxIpduBufIdType          IpduBufferId,
    uint16                       RxDynSignalLength)
{
    boolean                 signalGroupInvalid = FALSE;
    Com_RxGroupSignalIdType groupSignalEndId   = RxSignalGroupPtr->GroupSignalRefEndId;
    Com_RxGroupSignalIdType groupSignalStartId = RxSignalGroupPtr->GroupSignalRefStartId;

    for (; groupSignalStartId < groupSignalEndId; ++groupSignalStartId)
    {
        const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[groupSignalStartId];
        Com_SignalType               signalType       = rxGroupSignalPtr->ComSignalType;
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if ((COM_UINT8_N == signalType) || (COM_UINT8_DYN == signalType))
        {
#if (                              \
    (0u < COM_MAX_RXIPDUBUFF_SIZE) \
    && ((0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE) || (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)))
            uint16 signalLength = (COM_UINT8_N == signalType) ? rxGroupSignalPtr->ComSignalLength : RxDynSignalLength;
            Com_Uint8NDynGroupSignalInvalidCalculate(
                signalLength,
                rxGroupSignalPtr,
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
                &signalGroupInvalid,
#endif
                IpduBufferId,
                RxSignalGroupPtr->RxMainfunctionId);
#endif
        }
        else
#endif
        {
            const uint8* rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxSignalGroupPtr->RxMainfunctionId];
            uint64       unPackValue          = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
                signalType,
#endif
                COM_RX_GROUP_SIGNAL,
                rxGroupSignalPtr,
                &rxIpduRuntimeBuffPtr[IpduBufferId]);
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
            SchM_Enter_Com_MultiCore_Lock();
            uint16 gwSignalBufferId = rxGroupSignalPtr->GWSignalBufferId;
            Com_UpdateGWSignalBuffer(unPackValue, gwSignalBufferId, signalType);
            SchM_Exit_Com_MultiCore_Lock();
#endif

#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
            uint16 invalidValueId = rxGroupSignalPtr->ComSignalInvalidBuffRefId;
            if ((COM_UNUSED_UINT16 != invalidValueId) && (!signalGroupInvalid))
            {
                if (Com_SignalInvalidateHandle(signalType, invalidValueId, unPackValue))
                {
                    signalGroupInvalid = TRUE;
                }
            }
#endif
        }
#if COM_MAX_GWMAPPING_NUMBER > 0u
        Com_GwMappingIdType gwMappingId = rxGroupSignalPtr->GwMappingId;
        if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
        {
            Com_GwUpdateDestinationRequest(gwMappingId, TRUE);
        }
#endif
    }
    COM_NOUSED(RxDynSignalLength);
    return signalGroupInvalid;
}
#endif

/**
 * @brief RxSignal receive timeout handle
 *
 */
#if (COM_MAX_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
/* PRQA S 3673 ++*/ /* VL_QAC_3673 */
COM_LOCAL void Com_RxSignalTimeOutHandle(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
/* PRQA S 3673 -- */
{
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    ipduSignalTimeoutRefNumber            = RxIpduPtr->IPduSignalsTimeoutRefEndId;
    cnt                                   = RxIpduPtr->IPduSignalsTimeoutRefStartId;
    const uint16* rxSignalTimeoutValuePtr = Com_CfgSignalTimeoutPtr->RxSignalTimeoutVal;
    const uint16* timeoutRxSignalId       = Com_CfgSignalTimeoutPtr->TimeoutRxSignalId;

    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
        const Com_RxSignalType* rxSignalPtr = &Com_CfgRxSignalPtr[timeoutRxSignalId[cnt]];
        if (Com_RxSignalTimeOutCnt[cnt] > 0u)
        {
            Com_RxSignalTimeOutCnt[cnt] -= 1u;
            if (0u == Com_RxSignalTimeOutCnt[cnt])
            {
/*let pass any value for the filter MASKED_NEW_DIFFERS_MASKED_OLD after an RX deadline timeout*/
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u) && (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
                if ((COM_UNUSED_RXSIGNALFILTERID != rxSignalPtr->FilterIndex)
                    && (COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxSignalPtr->FilterAlgorithm))
                {
                    Com_MaskNewDifferMaskOldTimeOut[rxSignalPtr->FilterIndex] = TRUE;
                }
#endif
                /*handle the timeout replace action*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                uint16 substituteBufId = COM_UNUSED_UINT16;
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                substituteBufId = Com_ConfigStd->SignalSubstitueteValuePtr->RxSignalSubstituteValueId[cnt];
#endif
                Com_RxDataTimeoutActionType rxTimeoutAction =
                    Com_CfgSignalTimeoutPtr->RxSignalDataTimeoutActionCfg[cnt];
                if (COM_TIMEOUT_ACTION_NONE != rxTimeoutAction)
                {
                    Com_RxDataTimeOutActionHandle(
                        RxIpduStatePtr,
                        RxIpduPtr,
                        rxTimeoutAction,
                        rxSignalPtr,
                        TRUE,
                        substituteBufId);
                }
#endif
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
                const Com_RxTimeoutNotificationType* rxSignalDataTimeoutNotificationCfg =
                    Com_CfgSignalTimeoutPtr->RxSignalTimeoutNotificationCfg;
                if (NULL_PTR != rxSignalDataTimeoutNotificationCfg[cnt])
                {
                    rxSignalDataTimeoutNotificationCfg[cnt]();
                }
#endif
                Com_RxSignalTimeOutCnt[cnt] = rxSignalTimeoutValuePtr[cnt];
            }
        }
    }
    COM_NOUSED(RxIpduStatePtr);
    return;
}
#endif

/**
 * @brief Rx ipdu based receive DM timeout handle
 *
 */
#if (                                                                            \
    ((COM_MAX_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)) \
    || ((0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)))
COM_LOCAL void Com_RxIpduBaseDMTimeOutHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId)
{
    if (RxIpduStatePtr->RxIpduBaseDMTimeout > 0u)
    {
        RxIpduStatePtr->RxIpduBaseDMTimeout -= 1u;
        if (0u == RxIpduStatePtr->RxIpduBaseDMTimeout)
        {
#if (0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
            Com_RxDMTimeOutHandleSignal(RxIpduStatePtr, RxIpduPtr);
#endif

#if (0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
            Com_RxDMTimeOutHandleSignalGroup(RxIpduStatePtr, RxIpduPtr, RxMainfunctionId);
#endif
            RxIpduStatePtr->RxIpduBaseDMTimeout = RxIpduPtr->RxIPduDMTimeout;
        }
    }
    COM_NOUSED(RxMainfunctionId);
}
#endif

/**
 * @brief Rx ipdu based receive DM timeout handle signal
 *
 */
#if (0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
/* PRQA S 3673 ++*/ /* VL_QAC_3673 */
COM_LOCAL void Com_RxDMTimeOutHandleSignal(Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
/* PRQA S 3673 -- */
{
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    uint16 substituteBufId = COM_UNUSED_UINT16;
#endif
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    ipduSignalTimeoutRefNumber = RxIpduPtr->IPduSignalsTimeoutRefEndId;
    cnt                        = RxIpduPtr->IPduSignalsTimeoutRefStartId;
    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
        const uint16*           timeoutRxSignalId = Com_CfgSignalTimeoutPtr->TimeoutRxSignalId;
        const Com_RxSignalType* rxSignalPtr       = &Com_CfgRxSignalPtr[timeoutRxSignalId[cnt]];
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
        /*rx signal ComTimeout > 0 and have not update-bit*/
        if (COM_UNUSED_SIGNALPOSITION == rxSignalPtr->UpdateLsbBytePos)
#endif
        {
            Com_RxSigTimeoutIdType counter               = cnt - RxIpduPtr->IPduSignalsTimeoutRefStartId;
            const uint16*          rxSignaltimeoutValPtr = Com_CfgSignalTimeoutPtr->RxSignalTimeoutVal;
            if (rxSignaltimeoutValPtr[counter] > 0u)
            {
/*let pass any value for the filter MASKED_NEW_DIFFERS_MASKED_OLD after an RX deadline timeout*/
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                Com_SignalIdType maskNewDifferMaskOldId = rxSignalPtr->FilterIndex;
                if ((COM_UNUSED_RXSIGNALFILTERID == maskNewDifferMaskOldId)
                    && (COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxSignalPtr->FilterAlgorithm))
                {
                    Com_MaskNewDifferMaskOldTimeOut[maskNewDifferMaskOldId] = TRUE;
                }
#endif

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                Com_RxDataTimeoutActionType rxTimeoutAction =
                    Com_CfgSignalTimeoutPtr->RxSignalDataTimeoutActionCfg[cnt];
                if (COM_TIMEOUT_ACTION_NONE != rxTimeoutAction)
                {
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                    substituteBufId = Com_ConfigStd->SignalSubstitueteValuePtr->RxSignalSubstituteValueId[cnt];
#endif
                    Com_RxDataTimeOutActionHandle(
                        RxIpduStatePtr,
                        RxIpduPtr,
                        rxTimeoutAction,
                        rxSignalPtr,
                        TRUE,
                        substituteBufId);
                }
#endif
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
                const Com_RxTimeoutNotificationType* rxSignalTimeoutNotificationPtr =
                    Com_CfgSignalTimeoutPtr->RxSignalTimeoutNotificationCfg;
                if (NULL_PTR != rxSignalTimeoutNotificationPtr[cnt])
                {
                    rxSignalTimeoutNotificationPtr[cnt]();
                }
#endif
            }
        }
    }
    COM_NOUSED(RxIpduStatePtr);
}
#endif

/**
 * @brief Rx ipdu based receive DM timeout handle signal group
 *
 */
#if (0u < COM_MAX_RXGROUPSIGNAL_NUMBER) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
/* PRQA S 3673 ++*/ /* VL_QAC_3673 */
COM_LOCAL void Com_RxDMTimeOutHandleSignalGroup(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId)
/* PRQA S 3673 -- */
{
    uint16 substituteBufId = COM_UNUSED_UINT16;
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    /* substituteStartId is continuous */
    Com_RxGroupSignalIdType substituteStartId = RxIpduPtr->GrpSigSubstituteStartId;
#endif
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutEndId, ipduSignalGroupTimeoutStartId;
    ipduSignalGroupTimeoutEndId                    = RxIpduPtr->IPduSignalGroupsTimeoutRefEndId;
    ipduSignalGroupTimeoutStartId                  = RxIpduPtr->IPduSignalGroupsTimeoutRefStartId;
    const Com_RxSignalGroupIdType* rxSignalGroupId = Com_CfgSignalTimeoutPtr->TimeoutRxSigGrpId;
    for (; ipduSignalGroupTimeoutStartId < ipduSignalGroupTimeoutEndId; ++ipduSignalGroupTimeoutStartId)
    {
        Com_RxSignalGroupIdType      rxsigGrpId       = rxSignalGroupId[ipduSignalGroupTimeoutStartId];
        const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[rxsigGrpId];
/*rx signal group ComTimeout > 0 and have not update-bit*/
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        if (COM_UNUSED_SIGNALPOSITION == rxSignalGroupPtr->UpdateLsbBytePos)
#endif
        {
            const uint16* rxSignalGroupTimeoutValuePtr = Com_CfgSignalTimeoutPtr->RxSigGrpTimeoutVal;
            if (rxSignalGroupTimeoutValuePtr[ipduSignalGroupTimeoutStartId] > 0u)
            {
                Com_RxGroupSignalIdType      groupSignalNumber = rxSignalGroupPtr->GroupSignalRefEndId;
                Com_RxGroupSignalIdType      groupSignalId     = rxSignalGroupPtr->GroupSignalRefStartId;
                const Com_RxGroupSignalType* rxGroupSignalPtr;
                /* update all group signal init value into group signal runtime buffer */
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                for (; groupSignalId < groupSignalNumber; ++groupSignalId)
                {
                    rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[groupSignalId];
                    if ((COM_UNUSED_RXGRPSIGFILTERID != rxGroupSignalPtr->FilterIndex)
                        && (COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxGroupSignalPtr->FilterAlgorithm))
                    {
                        Com_MaskNewDifferMaskOldTimeOut[rxGroupSignalPtr->FilterIndex] = TRUE;
                    }
                }
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                Com_RxDataTimeoutActionType timeoutAction =
                    Com_CfgSignalTimeoutPtr->RxSigGrpDataTimeoutActionCfg[ipduSignalGroupTimeoutStartId];
                if (COM_TIMEOUT_ACTION_NONE != timeoutAction)
                {
/*replace the rx pdu buffer for signal group array*/
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
                    if (COM_UNUSED_COM_SIGNALGROUPARRYID != rxSignalGroupPtr->SignalGroupArrayId)
                    {
                        uint8*       rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
                        const uint8* rxIpduInitValuePtr   = Com_ConfigStd->RxIpduInitValuePtr[RxMainfunctionId];
                        const Com_SignalGroupArrayType* signalGroupArrayPtr =
                            &Com_ConfigStd->SignalGroupArrayPtr[rxSignalGroupPtr->SignalGroupArrayId];
                        uint16 iPduSignalGroupArrayStartBufId = signalGroupArrayPtr->IPduStartBufIndex;
                        SchM_Enter_Com_Context();
                        (void)IStdLib_MemCpy(
                            &rxIpduRuntimeBuffPtr[iPduSignalGroupArrayStartBufId],
                            &rxIpduInitValuePtr[iPduSignalGroupArrayStartBufId],
                            signalGroupArrayPtr->SignalGroupArrayLength);
                        SchM_Exit_Com_Context();
                    }
#endif
                    /*replace the signal group(include all group signals) value with init value*/
                    groupSignalId = rxSignalGroupPtr->GroupSignalRefStartId;
                    for (; groupSignalId < groupSignalNumber; ++groupSignalId)
                    {
                        rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[groupSignalId];
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
                        if (COM_TIMEOUT_ACTION_SUBSTITUTE == timeoutAction)
                        {
                            substituteBufId =
                                Com_ConfigStd->SignalSubstitueteValuePtr->RxGrpSigSubstituteValueId[substituteStartId];
                            substituteStartId++;
                        }
#endif
                        Com_RxDataTimeOutActionHandle(
                            RxIpduStatePtr,
                            RxIpduPtr,
                            timeoutAction,
                            rxGroupSignalPtr,
                            FALSE,
                            substituteBufId);
                    }
                }
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
                const Com_RxTimeoutNotificationType* rxSigGrpTimeoutNotificationPtr =
                    Com_CfgSignalTimeoutPtr->RxSigGrpTimeoutNotificationCfg;
                if (NULL_PTR != rxSigGrpTimeoutNotificationPtr[ipduSignalGroupTimeoutStartId])
                {
                    rxSigGrpTimeoutNotificationPtr[ipduSignalGroupTimeoutStartId]();
                }
#endif
            }
        }
    }
    COM_NOUSED(RxMainfunctionId);
    COM_NOUSED(RxIpduStatePtr);
}
#endif
/**
 * @brief reset receive MD timer for all signals/signal groups (which is update) of one rx pdu
 *
 */
#if (COM_MAX_RXSIGNAL_NUMBER + COM_MAX_RXSIGNALGROUP_NUMBER > 0u) \
    && ((STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE))
COM_LOCAL void Com_ResetUpdateDMTime(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    const PduInfoType*          PduInfo)
{
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition;
    uint8                  updateBitOffset;
#endif
    uint8 rxDMFlag = 0u;

#if (COM_MAX_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    ipduSignalTimeoutRefNumber = RxIpduPtr->IPduSignalsTimeoutRefEndId;
    cnt                        = RxIpduPtr->IPduSignalsTimeoutRefStartId;
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
    const uint16* timeoutRxSignalId       = Com_CfgSignalTimeoutPtr->TimeoutRxSignalId;
    const uint16* rxSignalTimeoutValuePtr = Com_CfgSignalTimeoutPtr->RxSignalTimeoutVal;
#endif
    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
        uint16                  rxSignalId  = timeoutRxSignalId[cnt];
        const Com_RxSignalType* rxSignalPtr = &Com_CfgRxSignalPtr[rxSignalId];
        updateBitPosition                   = rxSignalPtr->UpdateLsbBytePos;
        /*the rx signal have update bit*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            updateBitOffset = rxSignalPtr->UpdateBitMask;
            /*update bit is 1*/
            if (updateBitOffset == ((PduInfo->SduDataPtr[updateBitPosition]) & updateBitOffset))
            {
                Com_RxSignalTimeOutCnt[cnt] = rxSignalTimeoutValuePtr[cnt];
                rxDMFlag |= COM_RX_SIGNAL_DM;
            }
        }
        /*the rx signal not have update bit*/
        else
#endif
        {
            rxDMFlag |= COM_RX_IPDU_DM;
#if (STD_OFF == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
            break;
#endif
        }
    }
#endif
#if (COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;
    ipduSignalGroupTimeoutNumber = RxIpduPtr->IPduSignalGroupsTimeoutRefEndId;
    counter                      = RxIpduPtr->IPduSignalGroupsTimeoutRefStartId;
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    const Com_RxSignalGroupIdType* rxSignalGroupId              = Com_CfgSignalTimeoutPtr->TimeoutRxSigGrpId;
    const uint16*                  rxSignalGroupTimeoutValuePtr = Com_CfgSignalTimeoutPtr->RxSigGrpTimeoutVal;
#endif
    for (; counter < ipduSignalGroupTimeoutNumber; ++counter)
    {
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[rxSignalGroupId[counter]];
        updateBitPosition                             = rxSignalGroupPtr->UpdateLsbBytePos;
        /*the rx signal group not have update bit*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            updateBitOffset = rxSignalGroupPtr->UpdateBitMask;
            /*update bit is 1*/
            if (updateBitOffset == ((PduInfo->SduDataPtr[updateBitPosition]) & updateBitOffset))
            {
                Com_RxSignalGroupTimeOutCnt[counter] = rxSignalGroupTimeoutValuePtr[counter];
                rxDMFlag |= COM_RX_SIGNAL_GROUP_DM;
            }
        }
        /*the rx signal group not have update bit*/
        else
#endif
        {
            rxDMFlag |= COM_RX_IPDU_DM;
#if (STD_OFF == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
            break;
#endif
        }
    }
#endif

#if (                                                                            \
    ((COM_MAX_RXSIGNAL_NUMBER > 0u) && (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)) \
    || ((COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)))
    if (rxDMFlag > 0u)
    {
        if (COM_RX_IPDU_DM == (rxDMFlag & COM_RX_IPDU_DM)) /* PRQA S 2991,2995 */ /* VL_Com_ResultAlwaysTrue */
        {
            RxIpduStatePtr->RxIpduBaseDMTimeout = RxIpduPtr->RxIPduDMTimeout;
        }
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        if ((COM_RX_SIGNAL_DM == (rxDMFlag & COM_RX_SIGNAL_DM))
            || (COM_RX_SIGNAL_GROUP_DM == (rxDMFlag & COM_RX_SIGNAL_GROUP_DM)))
        {
            RxIpduStatePtr->RxIpduDMFlag = Com_SetRxDMFlag(RxIpduStatePtr->RxIpduDMFlag, rxDMFlag);
        }
        /* rx ipdu base DM */
        else
#endif
        {
            RxIpduStatePtr->RxIpduDMFlag = 0u;
            RxIpduStatePtr->RxIpduDMFlag = Com_SetRxDMFlag(RxIpduStatePtr->RxIpduDMFlag, COM_RX_IPDU_DM_ONLY);
        }
    }
#endif
    COM_NOUSED(PduInfo);
    return;
}
#endif

/**
 * @brief copy the Rx Pdu data to RxPdu Buffer
 *
 */
COM_LOCAL void Com_CopyPduDataToRxBuffer(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    const PduInfoType*          PduInfoPtr,
    Com_MainFunctionIdType      RxMainfunctionId)
{
    uint8* rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
    SchM_Enter_Com_Context();
#if (0u < COM_MAX_RXIPDUBUFF_SIZE)
    Com_RxIpduBufIdType pduBufferPos = RxIpduPtr->RxIPduBufIndex;
    /*copy receive pdu data to rx pdu buffer*/
    (void)IStdLib_MemCpy(&rxIpduRuntimeBuffPtr[pduBufferPos], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8         metadataLength = RxIpduPtr->MetaDataLength;
    PduLengthType metaDataStartPos =
        RxIpduPtr->IPduNoDynSignalLength + RxIpduPtr->IPduMaxDynSignalLength + (PduLengthType)pduBufferPos;
    if ((0u < metadataLength) && (NULL_PTR != PduInfoPtr->MetaDataPtr))
    {
        /*copy meta data to rx pdu buffer*/
        (void)IStdLib_MemCpy(&rxIpduRuntimeBuffPtr[metaDataStartPos], PduInfoPtr->MetaDataPtr, metadataLength);
    }
#endif
#endif
    SchM_Exit_Com_Context();

    /*update the pdu length*/
    RxIpduStatePtr->RxIpduLength = (uint16)PduInfoPtr->SduLength;
    return;
}
/**
 * @brief  unpack the rx pdu data and invoke notification
 *
 */
/* PRQA S 3673 ++*/ /* VL_QAC_3673 */
#if (0u < COM_MAX_RXIPDU_NUMBER)
COM_LOCAL void Com_IndicationProcess(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_MainFunctionIdType      RxMainfunctionId)
/* PRQA S 3673 -- */
{
    uint8*              rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
    uint16              rxDynSignalLength    = RxIpduStatePtr->RxIpduLength - (uint16)RxIpduPtr->IPduNoDynSignalLength;
    Com_RxIpduBufIdType ipduBufferId         = RxIpduPtr->RxIPduBufIndex;
    uint16              partitionId          = RxIpduPtr->IpduPartitionId;
    boolean             ubIsAccess           = TRUE;
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_SRC_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition;
    uint8                  updateBitOffset;
#endif

/*store the rx dynamic signal length which really receive,not replace by any reason for gateway*/
#if (0u < COM_MAX_RXSIGNAL_NUMBER)
    Com_RxSignalIdType ipduSignalRefNumber = RxIpduPtr->IPduSignalsRefEndId;
    Com_RxSignalIdType cnt                 = RxIpduPtr->IpduSignalRefStartId;
    for (; cnt < ipduSignalRefNumber; ++cnt)
    {
        const Com_RxSignalType* rxSignalPtr = &Com_CfgRxSignalPtr[cnt];
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
        updateBitPosition = rxSignalPtr->UpdateLsbBytePos;
        uint8 sigUbOffset = (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
                                ? rxIpduRuntimeBuffPtr[ipduBufferId + (Com_RxIpduBufIdType)(updateBitPosition)]
                                : COM_UNUSED_UINT8;
        updateBitOffset   = rxSignalPtr->UpdateBitMask;
        /*the rx signal have update bit*/
        ubIsAccess = Com_RxUpdateBitIsTure(updateBitPosition, sigUbOffset, updateBitOffset);
#endif
        if (ubIsAccess) /* PRQA S 2991 */ /* VL_Com_ResultAlwaysTrue */
        {
            Com_SignalRxIndication(rxSignalPtr, rxIpduRuntimeBuffPtr, rxDynSignalLength, ipduBufferId);
        }
    }
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_NUMBER)
    Com_RxSignalGroupIdType ipduSignalGroupNumber = RxIpduPtr->IPduSignalGroupsRefEndId;
    Com_RxSignalGroupIdType counter               = RxIpduPtr->IPduSignalGroupsRefStartId;
    for (; counter < ipduSignalGroupNumber; ++counter)
    {
        const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[counter];
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        updateBitPosition    = rxSignalGroupPtr->UpdateLsbBytePos;
        uint8 sigGrpUbOffset = (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
                                   ? rxIpduRuntimeBuffPtr[ipduBufferId + (Com_RxIpduBufIdType)(updateBitPosition)]
                                   : COM_UNUSED_UINT8;
        updateBitOffset      = rxSignalGroupPtr->UpdateBitMask;
        /*the rx group signal have update bit*/
        ubIsAccess = Com_RxUpdateBitIsTure(updateBitPosition, sigGrpUbOffset, updateBitOffset);
#endif
        if (ubIsAccess) /* PRQA S 2991 */ /* VL_Com_ResultAlwaysTrue */
        {
            Com_SignalGroupRxIndication(rxSignalGroupPtr, rxDynSignalLength, ipduBufferId, partitionId);
        }
    }
#endif
#if (0u < COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER)
    Com_GwSourceDescriptionIdType gwSourceSignalNumber = RxIpduPtr->IPduSourceSignalsRefEndId;
    Com_GwSourceDescriptionIdType gwSourceId           = RxIpduPtr->IPduSourceSignalsRefStartId;
    for (; gwSourceId < gwSourceSignalNumber; ++gwSourceId)
    {
        const Com_GwSourceSignalType* gwSourceSignalPtr = &Com_CfgGwMappingConfigPtr->SourceSignal[gwSourceId];
#if (STD_ON == COM_GW_SRC_SIG_UPDATE_BIT_ENABLE)
        updateBitPosition  = gwSourceSignalPtr->UpdateLsbBytePos;
        uint8 despUbOffset = (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
                                 ? rxIpduRuntimeBuffPtr[ipduBufferId + (Com_RxIpduBufIdType)updateBitPosition]
                                 : COM_UNUSED_UINT8;
        updateBitOffset    = gwSourceSignalPtr->UpdateBitMask;
        /*the source signal have update bit*/
        ubIsAccess = Com_RxUpdateBitIsTure(updateBitPosition, despUbOffset, updateBitOffset);
#endif
        if (ubIsAccess)
        {
            /*unpack the rx source description signal to gateway buffer*/
            Com_SourceSignalUnPack(gwSourceSignalPtr, rxIpduRuntimeBuffPtr, rxDynSignalLength, ipduBufferId);
        }
    }
#endif
    return;
}
#endif
/**
 * @brief execute all configured ComDataInvalidActions for all included signals and signal groups.
 *
 */
#if (STD_ON == COM_RXTPPDU_SUPPORT) \
    && ((STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE))
COM_LOCAL void Com_TpPduInvalidAction(const Com_RxIPduRunTimeStateType* RxIpduStatePtr, const Com_RxIPduType* RxIpduPtr)
{
    uint16 partitionId = RxIpduPtr->IpduPartitionId;
#if (0u < COM_MAX_RXSIGNAL_NUMBER) && (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
    Com_RxSignalIdType ipduSignalEndId, ipduSignalStartId;
    ipduSignalEndId   = RxIpduPtr->IPduSignalsRefEndId;
    ipduSignalStartId = RxIpduPtr->IpduSignalRefStartId;
    for (; ipduSignalStartId < ipduSignalEndId; ++ipduSignalStartId)
    {
        const Com_RxSignalType*   rxSignalPtr     = &Com_CfgRxSignalPtr[ipduSignalStartId];
        Com_RxSignalInvalidIdType signalInvalidId = rxSignalPtr->SignalInvalidRefId;
        if (COM_UNUSED_RXSIGNALINVALIDID != signalInvalidId)
        {
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
            const Com_SignalInvalidValueType* signalInvalidValue = Com_ConfigStd->SignalInvalidValuePtr;
            if ((COM_INVALID_ACTION_NOTIFY == signalInvalidValue->RxSignalDataInvalidAction[signalInvalidId])
                && (NULL_PTR != signalInvalidValue->RxInvalidNotificationCfg[signalInvalidId]))
            {
                signalInvalidValue->RxInvalidNotificationCfg[signalInvalidId]();
            }
#endif
            if (COM_INVALID_ACTION_REPLACE == signalInvalidValue->RxSignalDataInvalidAction[signalInvalidId])
            {
                Com_InvalidSignalReplaceAndNotification(rxSignalPtr, partitionId);
            }
        }
    }
#endif
#if (                                                                          \
    (0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER) \
    && (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE))
    Com_RxSignalGroupIdType ipduSignalGroupNumber, counter;
    ipduSignalGroupNumber = RxIpduPtr->IPduSignalGroupsRefEndId;
    counter               = RxIpduPtr->IPduSignalGroupsRefStartId;
    for (; counter < ipduSignalGroupNumber; ++counter)
    {
        const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[counter];
#if (STD_ON == COM_RX_SIG_GROUP_INVALID_DATA_ACTION_NOTIFY_ENABLE)
        if ((COM_INVALID_ACTION_NOTIFY == rxSignalGroupPtr->ComDataInvalidAction)
            && (NULL_PTR != rxSignalGroupPtr->ComInvalidNotification))
        {
            rxSignalGroupPtr->ComInvalidNotification();
        }
#endif
        Com_RxIpduBufIdType ipduBufferId = RxIpduPtr->RxIPduBufIndex;
        if ((COM_INVALID_ACTION_REPLACE == rxSignalGroupPtr->ComDataInvalidAction)
#if (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
            && (Com_RxSignalGroupFilter(rxSignalGroupPtr, TRUE, ipduBufferId, partitionId))
#endif
        )
        {
            Com_RxGroupSignalIdType groupSignalNumber, groupSignalId;
            uint16 rxDynSignalLength = RxIpduStatePtr->RxIpduLength - (uint16)RxIpduPtr->IPduNoDynSignalLength;
            groupSignalNumber        = rxSignalGroupPtr->GroupSignalRefEndId;
            groupSignalId            = rxSignalGroupPtr->GroupSignalRefStartId;
            /* update all group signal init value into group signal runtime buffer */
            for (; groupSignalId < groupSignalNumber; ++groupSignalId)
            {
                Com_RxGroupSignalBuffHanlde(TRUE, groupSignalId, ipduBufferId, rxDynSignalLength);
            }
#if (STD_ON == COM_RX_SIG_GROUP_NOTIFICATION_ENABLE)
            if (NULL_PTR != rxSignalGroupPtr->ComNotification)
            {
                rxSignalGroupPtr->ComNotification();
            }
#endif
        }
    }
#endif
    COM_NOUSED(RxIpduStatePtr);

    return;
}
#endif

/**
 * @brief Rx data timeout action handle
 *
 */
#if (                                                              \
    (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE)        \
    || (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)) \
    || ((STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE) \
        || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE))
/* PRQA S 3673 ++*/ /* VL_QAC_3673 */
COM_LOCAL void Com_RxDataTimeOutActionHandle(
    Com_RxIPduRunTimeStateType* RxIpduStatePtr,
    const Com_RxIPduType*       RxIpduPtr,
    Com_RxDataTimeoutActionType RxTimeoutAction,
    const void*                 RxArgPtr,
    boolean                     IsRxSignal,
    Com_SignalIdType            SubstituteBufId)
/* PRQA S 3673 -- */
{
    Com_SignalType               signalType;
    uint16                       initValueLength;
    uint16                       initValueId;
    const Com_RxSignalType*      rxSignalPtr;
    const Com_RxGroupSignalType* rxGroupSignalPtr;

    /*rx signal*/
    if (IsRxSignal)
    {
        rxSignalPtr = (const Com_RxSignalType*)RxArgPtr;
        initValueId = rxSignalPtr->SignalInitValueId;
        signalType  = rxSignalPtr->ComSignalType;
    }
    /*rx signal group*/
    else
    {
        rxGroupSignalPtr = (const Com_RxGroupSignalType*)RxArgPtr;
        initValueId      = rxGroupSignalPtr->SignalInitValueId;
        signalType       = rxGroupSignalPtr->ComSignalType;
    }

    /*handle the timeout replace or substitute action*/
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
    || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE)
    if (COM_UINT8_DYN == signalType)
    {
        initValueLength =
            (IsRxSignal) ? (rxSignalPtr->SignalDataInitValueLength) : (rxGroupSignalPtr->SignalDataInitValueLength);
        RxIpduStatePtr->RxIpduLength = (uint16)RxIpduPtr->IPduNoDynSignalLength + initValueLength;
    }
    else
#endif
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) \
    || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == signalType)
    {
        initValueLength = (IsRxSignal) ? (rxSignalPtr->ComSignalLength) : (rxGroupSignalPtr->ComSignalLength);
    }
    else
#endif
    {
        initValueLength = 0u;
    }
    uint16 partitionId = RxIpduPtr->IpduPartitionId;
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_REPLACE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_REPLACE_ENABLE)
    if (COM_TIMEOUT_ACTION_REPLACE == RxTimeoutAction)
    {
        Com_InitSignalBuffer(signalType, initValueId, initValueLength, partitionId);
    }
#endif
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
    if (COM_TIMEOUT_ACTION_SUBSTITUTE == RxTimeoutAction)
    {
        Com_SubstituteSignalBuffer(signalType, initValueId, initValueLength, SubstituteBufId, partitionId);
    }
#endif
    COM_NOUSED(SubstituteBufId);
    COM_NOUSED(RxIpduStatePtr);
}
#endif
/**
 * @brief called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 *        judge the rx signal group value is filter out or not.
 *
 */
#if (COM_MAX_RXIPDU_NUMBER > 0u) && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
COM_LOCAL boolean Com_RxSignalGroupFilter(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    boolean                      InvalidSignalGroup,
    Com_RxIpduBufIdType          IpduBufferId,
    uint16                       PartitionId)
{
    boolean                 signalGroupFilter  = TRUE;
    Com_RxGroupSignalIdType groupSignalEndId   = RxSignalGroupPtr->GroupSignalRefEndId;
    Com_RxGroupSignalIdType groupSignalStartId = RxSignalGroupPtr->GroupSignalRefStartId;

    for (; ((groupSignalStartId < groupSignalEndId) && signalGroupFilter); ++groupSignalStartId)
    {
        const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[groupSignalStartId];
        Com_SignalType               groupSignalType  = rxGroupSignalPtr->ComSignalType;
/*Other Signal types besides  COM_UINT8_DYN or COM_UINT8_DYN*/
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if ((groupSignalType != COM_UINT8_N) && (groupSignalType != COM_UINT8_DYN))
#endif
        {
            uint64 groupSignalValue;
            uint16 groupSignalInitValueId = rxGroupSignalPtr->SignalInitValueId;
#if (STD_ON == COM_RX_GRP_SIGNAL_INVALID_DATA_ENABLE)
            /*the signal group value is invalid,use the init value replace*/
            if (InvalidSignalGroup)
            {
                /*get the init value except COM_UINT8_N and UNIT8_DYN*/
                groupSignalValue = Com_RxSignalReplaceHanlde(groupSignalType, groupSignalInitValueId, PartitionId);
            }
            else
#endif
            {
                const uint8* rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxSignalGroupPtr->RxMainfunctionId];
                /*get the init value except COM_UINT8_N and UNIT8_DYN*/
                groupSignalValue = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
                    groupSignalType,
#endif
                    COM_RX_GROUP_SIGNAL,
                    rxGroupSignalPtr,
                    &rxIpduRuntimeBuffPtr[IpduBufferId]);
            }
#if (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
            signalGroupFilter = Com_RxSignalFilter(
                groupSignalType,
                rxGroupSignalPtr->FilterAlgorithm,
                (Com_SignalIdType)rxGroupSignalPtr->FilterIndex,
#if (COM_MAX_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                groupSignalInitValueId,
                PartitionId,
#endif
                groupSignalValue);
#endif
        }
/*COM_UINT8_N or COM_UINT8_DYN*/
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        else
        {
#if (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
            if ((COM_UNUSED_RXGRPSIGFILTERID != rxGroupSignalPtr->FilterIndex)
                && (COM_NEVER == rxGroupSignalPtr->FilterAlgorithm))
            {
                signalGroupFilter = FALSE;
            }
#endif
        }
#endif
    }

    return signalGroupFilter;
}
#endif
/* PRQA S 2895 -- */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 0316,2844 -- */
/* PRQA S 2814,2824 -- */
/* PRQA S 3120,3132 -- */
