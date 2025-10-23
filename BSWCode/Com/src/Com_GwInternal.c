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
 **  @file               : Com_GwInternal.c
 **  @author             : shengnan.sun
 **  @date               : 2024/09/01
 **  @vendor             : isoft
 **  @description        : internal implementation for COM gateway signal
 **
 ***********************************************************************************************************************/
/* PRQA S 3120,3132 ++ */ /* VL_QAC_MagicNum */
/* PRQA S 2814,2824 ++ */ /* VL_QAC_DerefNullPtr */
/* PRQA S 0316,2844 ++ */ /* VL_QAC_0316,VL_Com_NullPtrValue */
/* =================================================== inclusions =================================================== */
#include "Com_Internal.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

#if (0u < COM_MAX_GWMAPPING_NUMBER)
/* ============================================ internal data definition ============================================ */
/* Com signal gateway flag */
#define COM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Com_MemMap.h"
COM_LOCAL boolean Com_GwDestinationRequest[COM_MAX_GW_DESTINATION_NUM];
#define COM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Com_MemMap.h"

/* ========================================== internal function declarations ======================================== */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 5015 ++ */ /* VL_Com_UntrustedMethod */
/**
 * @brief         Get the GW Signal's New value and Old value
 * @param[in]     SignalType:SignalType of signal/groupSignal
 * @param[in]     GWSignalBufferId:Gateway signal index in gateway buffer
 * @param[in]     TxSignalValueId:TxSignal index in signal value
 * @param[in]     SourceSignalValue:Source signal value in signal gateway
 * @param[in]     PartitionId:Partition index
 * @retval        destSignalValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66533
 */
COM_LOCAL uint64 Com_GwSignalNewOldValueGet(
    Com_SignalType SignalType,
    uint16         GWSignalBufferId,
    uint16         TxSignalValueId,
    uint64*        SourceSignalValue,
    uint16         PartitionId);
/**
 * @brief         Gateway pdu(which include gateway signal and group signal) handle
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     SignalTxProperty:Property of TxSignal
 * @param[in]     RptNum:Repetition number
 * @param[in]     ValueChanged:Value change or not
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66534
 */
#if (                                                                                    \
    ((0u < COM_MAX_TXSIGNAL_NUMBER) || (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)) \
    || ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER)))
COM_LOCAL void Com_GwSignalOfPduHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    Com_TransferPropertyType    SignalTxProperty,
    uint16                      RptNum,
    boolean                     ValueChanged);
#endif
/**
 * @brief         Called by Com_GwUint8NDYNSignal.
 *                gateway signal which signal type is COM_UINT8_DYN,UINT8_N
 * @param[in]     SignalType:SignalType of signal/groupSignal/destSignal
 * @param[in]     SignalBufferId:Signal buffer index
 * @param[in]     ComSignalLength:Length of signal/groupSignal/destSignal
 * @param[in]     DestSignalId:Signal gateway destination signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_TXSIGNAL_NUMBER)                                         \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_GwUint8NDYNTxSigHandle(
    Com_SignalType   SignalType,
    uint16           SignalBufferId,
    uint16           ComSignalLength,
    Com_SignalIdType DestSignalId);
#endif
/**
 * @brief         Called by Com_GwUint8NDYNSignal.
 *                gateway group signal which signal type is COM_UINT8_DYN,UINT8_N
 * @param[in]     SignalType:SignalType of signal/groupSignal/destSignal
 * @param[in]     SignalBufferId:Signal buffer index
 * @param[in]     ComSignalLength:Length of signal/groupSignal/destSignal
 * @param[in]     DestSignalId:Signal gateway destination signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)                                                                           \
    && ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))                                 \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_GwUint8NDYNTxGrpSigHandle(
    Com_SignalType   SignalType,
    uint16           SignalBufferId,
    uint16           ComSignalLength,
    Com_SignalIdType DestSignalId);
#endif
/**
 * @brief         Called by Com_GwUint8NDYNSignal.
 *                gateway description which signal type is COM_UINT8_DYN,UINT8_N
 * @param[in]     SignalType:SignalType of signal/groupSignal/destSignal
 * @param[in]     SignalBufferId:Signal buffer index
 * @param[in]     ComSignalLength:Length of signal/groupSignal/destSignal
 * @param[in]     DestSignalId:Signal gateway destination signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)                       \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_GwUint8NDYNDestSigHandle(
    Com_SignalType   SignalType,
    uint16           SignalBufferId,
    uint16           ComSignalLength,
    Com_SignalIdType DestSignalId);
#endif
/**
 * @brief         Gateway signal/group signal/description signal which signal type is COM_UINT8_DYN
 * @param[in]     SignalType:SignalType of signal/groupSignal
 * @param[in]     SignalBufferId:Signal buffer index
 * @param[in]     ComSignalLength:Length of signal/groupSignal
 * @param[in]     DestSignalType:Signal gateway destination signal type
 * @param[in]     DestSignalId:Signal gateway destination signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66535
 */
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)                                                                           \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_GwUint8NDYNSignal(
    Com_SignalType       SignalType,
    uint16               SignalBufferId,
    uint16               ComSignalLength,
    Com_SignalConfigType DestSignalType,
    Com_SignalIdType     DestSignalId);
#endif
/**
 * @brief         Gateway signal which signal type is COM_UINT8_DYN,
 *                Called by Com_GwNotArraySignal.
 * @param[in]     SignalType:SignalType of signal/groupSignal/destSignal
 * @param[in]     GWSignalBufferId:Gateway signal index in gateway buffer
 * @param[in]     DestSignalId:Signal gateway destination signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
COM_LOCAL void
    Com_GwNotArrayTxSignalHandle(Com_SignalType SignalType, uint16 GWSignalBufferId, Com_SignalIdType DestSignalId);
#endif
/**
 * @brief         Gateway group signal which signal type is COM_UINT8_DYN,
 *                Called by Com_GwNotArraySignal.
 * @param[in]     SignalType:SignalType of signal/groupSignal/destSignal
 * @param[in]     GWSignalBufferId:Gateway signal index in gateway buffer
 * @param[in]     DestSignalId:Signal gateway destination signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))
COM_LOCAL void
    Com_GwNotArrayTxGrpSigHandle(Com_SignalType SignalType, uint16 GWSignalBufferId, Com_SignalIdType DestSignalId);
#endif
/**
 * @brief         Gateway description signal which signal type is COM_UINT8_DYN,
 *                Called by Com_GwNotArraySignal.
 * @param[in]     SignalType:SignalType of signal/groupSignal/destSignal
 * @param[in]     GWSignalBufferId:Gateway signal index in gateway buffer
 * @param[in]     DestSignalId:Signal gateway destination signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)
COM_LOCAL void
    Com_GwNotArrayDestSigHandle(Com_SignalType SignalType, uint16 GWSignalBufferId, Com_SignalIdType DestSignalId);
#endif
/**
 * @brief         Gateway signal/group signal/description signal which signal type is COM_UINT8_DYN
 * @param[in]     SignalType:SignalType of signal/groupSignal
 * @param[in]     GWSignalBufferId:Gateway signal index in gateway buffer
 * @param[in]     DestSignalType:Signal gateway destination signal type
 * @param[in]     DestSignalId:Signal gateway destination signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66537
 */
COM_LOCAL void Com_GwNotArraySignal(
    Com_SignalType       SignalType,
    uint16               GWSignalBufferId,
    Com_SignalConfigType DestSignalType,
    Com_SignalIdType     DestSignalId);
/**
 * @brief         Gateway signal/group signal/signal group/description signal
 * @param[in]     GwDestPtr:Pointer of signal gateway destination
 * @param[in]     GWSourceSignalType:Signal gateway source signal type
 * @param[in]     GWSourceSignalId:Signal gateway source signal index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66538
 */
COM_LOCAL void Com_GwSignal(
    const Com_GwDestType* GwDestPtr,
    Com_SignalConfigType  GWSourceSignalType,
    Com_SignalIdType      GWSourceSignalId);
/* PRQA S 5015 -- */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#if COM_MAX_GWMAPPING_NUMBER > 0u
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/**
 * @brief Signal gateway handle.
 *
 */
/* PRQA S 1532, 6030 ++ */ /* VL_Com_ReferencedOnlyOne, VL_MTR_Com_STMIF */
void Com_MainFunctionRouteSignalsHandle(Com_MainFunctionIdType RouteSignalsMainFunctionId)
/* PRQA S 1532, 6030 -- */
{
    for (Com_SignalIdType gwMappingId = 0u; gwMappingId < Com_ConfigStd->GwDestNum; ++gwMappingId)
    {
        const Com_GwMappingType* gwMappingPtr = &Com_CfgGwMappingConfigPtr->GwMapping[gwMappingId];
        Com_SignalIdType         gwDestStart  = gwMappingPtr->GwDestSignalStartId;
        Com_SignalIdType         gwDestEnd    = gwMappingPtr->GwDestSignalEndId;
        for (Com_SignalIdType gwDestId = gwDestStart; gwDestId < gwDestEnd; ++gwDestId)
        {
            const Com_GwDestType* gwDestPtr = &Com_CfgGwMappingConfigPtr->GwDest[gwDestId];
            if ((RouteSignalsMainFunctionId == gwDestPtr->MainFunctionId) && Com_GwDestinationRequest[gwDestId])
            {
                Com_GwDestinationRequest[gwDestId] = FALSE;
                Com_GwSignal(gwDestPtr, gwMappingPtr->GwSourceSignalType, gwMappingPtr->GwSourceSignalId);
            }
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif
/* ========================================== internal function definitions ========================================= */
#if ((0u < COM_MAX_GWMAPPING_NUMBER) && (0u < COM_MAX_RXIPDU_NUMBER) && (0u < COM_MAX_TXIPDU_NUMBER))
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/**
 * @brief Gateway signal which signal type is COM_UINT8_DYN,
 *        Called by Com_GwNotArraySignal.
 *
 */
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
COM_LOCAL void
    Com_GwNotArrayTxSignalHandle(Com_SignalType SignalType, uint16 GWSignalBufferId, Com_SignalIdType DestSignalId)
{
    boolean                 valueChanged      = FALSE;
    uint64                  sourceSignalValue = 0u;
    uint16                  rptNum            = 1u;
    const Com_TxSignalType* txSignalPtr       = &Com_CfgTxSignalPtr[DestSignalId];
    PduIdType               txIpduId          = txSignalPtr->IpduRefIndex;
    const Com_TxIPduType*   txIpduPtr         = &Com_CfgTxIPduPtr[txIpduId];
    Com_MainFunctionIdType  txMainfunctionId  = txIpduPtr->TxMainfunctionId;
    Com_TxIpduBufIdType     ipduBufferIndex   = txIpduPtr->TxIPduBufIndex;
    SchM_Enter_Com_MultiCore_Lock();
    uint64 destSignalValue = Com_GwSignalNewOldValueGet(
        SignalType,
        GWSignalBufferId,
        txSignalPtr->SignalInitValueId,
        &sourceSignalValue,
        txIpduPtr->IpduPartitionId);
    uint8* txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[txMainfunctionId];
    SchM_Exit_Com_MultiCore_Lock();
    if (sourceSignalValue != destSignalValue)
    {
        valueChanged = TRUE;
/*pack the new value*/
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
        Com_TxSignalPackHandle(COM_TX_SIGNAL, txSignalPtr, sourceSignalValue, &txIpduRuntimeBuffPtr[ipduBufferIndex]);
#endif
    }
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(
        ipduBufferIndex,
        txSignalPtr->UpdateLsbBytePos,
        txSignalPtr->UpdateBitMask,
        txIpduRuntimeBuffPtr);
#endif
    PduIdType                   ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
#if (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    /* PRQA S 2895 ++ */ /* VL_Com_NegtiveValueType */
    if (COM_UNUSED_TXSIGNALFILTERID != txSignalPtr->FilterIndex)
    /* PRQA S 2895 -- */
    {
        boolean* txSignalTMCRuntimePtr                  = Com_TxSignalTMCRunTime[txMainfunctionId];
        txSignalTMCRuntimePtr[txSignalPtr->TMCBufferId] = Com_DestSignalTMCCalculate(
            SignalType,
            txSignalPtr->FilterAlgorithm,
            (Com_SignalIdType)txSignalPtr->FilterIndex,
#if (COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
            destSignalValue,
#endif
            sourceSignalValue);
    }
#endif
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed
     * will reset periodic mode counter.*/
    Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
    /*handle the tx pdu (which include the gateway signal) transmit parameter*/
    Com_GwSignalOfPduHandle(txIpduStatePtr, txSignalPtr->ComTransferProperty, rptNum, valueChanged);
/*the tx signal configuration timeout*/
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
    /* PRQA S 2895 ++ */ /* VL_Com_NegtiveValueType */
    if ((COM_UNUSED_TXSIGNALTIMEOUTID != txSignalPtr->TimeoutIndex)
        && ((0u == txIpduStatePtr->DMCnt) || (1u < txIpduStatePtr->RptNum)))
    /* PRQA S 2895 -- */
    {
        txIpduStatePtr->DMCnt = txIpduPtr->TxIpduDM;
    }
#endif
}
#endif
/**
 * @brief Gateway group signal which signal type is COM_UINT8_DYN,
 *        Called by Com_GwNotArraySignal.
 *
 */
#if ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))
/* PRQA S 6070 ++ */ /* VL_MTR_Com_STCAL */
COM_LOCAL void
    Com_GwNotArrayTxGrpSigHandle(Com_SignalType SignalType, uint16 GWSignalBufferId, Com_SignalIdType DestSignalId)
/* PRQA S 6070 -- */
{
    uint8*                       txIpduRuntimeBuffPtr;
    boolean                      valueChanged      = FALSE;
    uint64                       sourceSignalValue = 0u;
    uint16                       rptNum            = 1u;
    const Com_TxGroupSignalType* txGroupSignalPtr  = &Com_CfgTxGroupSignalPtr[DestSignalId];
    uint16                       txSignalValueId   = txGroupSignalPtr->SignalInitValueId;
    Com_TxSignalGroupIdType      txSignalGroupId   = txGroupSignalPtr->ComSignalGroupRef;
    const Com_TxSignalGroupType* txSignalGroupPtr  = &Com_CfgTxSignalGroupPtr[txSignalGroupId];
    PduIdType                    txIpduId          = txSignalGroupPtr->IpduRefIndex;
    const Com_TxIPduType*        txIpduPtr         = &Com_CfgTxIPduPtr[txIpduId];
    Com_MainFunctionIdType       txMainfunctionId  = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition = txSignalGroupPtr->UpdateLsbBytePos;
    uint8                  updateBitMask     = txSignalGroupPtr->UpdateBitMask;
#endif

    Com_TxIpduBufIdType      ipduBufferIndex  = txIpduPtr->TxIPduBufIndex;
    Com_TransferPropertyType signalTxProperty = (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty)
                                                    ? txGroupSignalPtr->ComTransferProperty
                                                    : txSignalGroupPtr->ComTransferProperty;
    uint16                   partitionId      = txIpduPtr->IpduPartitionId;
    SchM_Enter_Com_MultiCore_Lock();
    uint64 destSignalValue =
        Com_GwSignalNewOldValueGet(SignalType, GWSignalBufferId, txSignalValueId, &sourceSignalValue, partitionId);
    SchM_Exit_Com_MultiCore_Lock();
    if (sourceSignalValue != destSignalValue)
    {
        valueChanged = TRUE;
/*pack the new value*/
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
        txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[txMainfunctionId];
        Com_TxSignalPackHandle(
            COM_TX_GROUP_SIGNAL,
            txGroupSignalPtr,
            sourceSignalValue,
            &txIpduRuntimeBuffPtr[ipduBufferIndex]);
#endif
    }
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(ipduBufferIndex, updateBitPosition, updateBitMask, txIpduRuntimeBuffPtr);
#endif
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER)
    boolean* txSignalTMCRuntimePtr = Com_TxSignalTMCRunTime[txMainfunctionId];
#endif
    PduIdType                   ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];

#if (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->FilterIndex) /* PRQA S 2895 */ /* VL_Com_NegtiveValueType */
    {
        txSignalTMCRuntimePtr[txGroupSignalPtr->TMCBufferId] = Com_DestSignalTMCCalculate(
            SignalType,
            txGroupSignalPtr->FilterAlgorithm,
            (Com_SignalIdType)txGroupSignalPtr->FilterIndex,
#if (COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
            destSignalValue,
#endif
            sourceSignalValue);
    }
#endif
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed
     * will reset periodic mode counter.*/
    Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
    /*handle the tx pdu (which include the gateway group signal) transmit parameter*/
    Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
/*the tx signal group(include the gateway group signal)configuration timeout*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    if ((COM_UNUSED_TXSIGGRPTIMEOUTID != txSignalGroupPtr->TimeoutIndex) /* PRQA S 2895 */ /* VL_Com_NegtiveValueType */
        && ((0u == txIpduStatePtr->DMCnt) || (1u < txIpduStatePtr->RptNum)))
    {
        txIpduStatePtr->DMCnt = txIpduPtr->TxIpduDM;
    }
#endif
}
#endif
/**
 * @brief Gateway description signal which signal type is COM_UINT8_DYN,
 *        Called by Com_GwNotArraySignal.
 *
 */
#if (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)
COM_LOCAL void
    Com_GwNotArrayDestSigHandle(Com_SignalType SignalType, uint16 GWSignalBufferId, Com_SignalIdType DestSignalId)
{
    uint8*                      txIpduRuntimeBuffPtr;
    boolean                     valueChanged      = FALSE;
    uint64                      sourceSignalValue = 0u;
    uint16                      rptNum            = 1u;
    const Com_GwDestSignalType* gwDestSignalPtr   = &Com_CfgGwMappingConfigPtr->DestSignal[DestSignalId];
    uint16                      txSignalValueId   = gwDestSignalPtr->SignalInitValueId;
    PduIdType                   txIpduId          = gwDestSignalPtr->ComGwIPduRef;
    const Com_TxIPduType*       txIpduPtr         = &Com_CfgTxIPduPtr[txIpduId];
    Com_MainFunctionIdType      txMainfunctionId  = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition = gwDestSignalPtr->UpdateLsbBytePos;
    uint8                  updateBitMask     = gwDestSignalPtr->UpdateBitMask;
#endif
    Com_TransferPropertyType signalTxProperty = gwDestSignalPtr->ComTransferProperty;
    Com_TxIpduBufIdType      ipduBufferIndex  = txIpduPtr->TxIPduBufIndex;
    uint16                   partitionId      = txIpduPtr->IpduPartitionId;
    SchM_Enter_Com_MultiCore_Lock();
    uint64 destSignalValue =
        Com_GwSignalNewOldValueGet(SignalType, GWSignalBufferId, txSignalValueId, &sourceSignalValue, partitionId);
    SchM_Exit_Com_MultiCore_Lock();
    if (sourceSignalValue != destSignalValue)
    {
        valueChanged = TRUE;
/*pack the new value*/
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
        txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[txMainfunctionId];
        Com_TxSignalPackHandle(
            COM_TX_DESCRIPTION,
            gwDestSignalPtr,
            sourceSignalValue,
            &txIpduRuntimeBuffPtr[ipduBufferIndex]);
#endif
    }
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(ipduBufferIndex, updateBitPosition, updateBitMask, txIpduRuntimeBuffPtr);
#endif
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER)
    boolean* txSignalTMCRuntimePtr = Com_TxSignalTMCRunTime[txMainfunctionId];
#endif
    PduIdType                   ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];

#if (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    if (COM_UNUSED_DESTSIGNALFILTERID != gwDestSignalPtr->FilterIndex) /* PRQA S 2895 */ /* VL_Com_NegtiveValueType */
    {
        txSignalTMCRuntimePtr[gwDestSignalPtr->TMCBufferId] = Com_DestSignalTMCCalculate(
            SignalType,
            gwDestSignalPtr->FilterAlgorithm,
            (Com_SignalIdType)gwDestSignalPtr->FilterIndex,
#if (COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
            destSignalValue,
#endif
            sourceSignalValue);
    }
#endif
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed
     * will reset periodic mode counter.*/
    Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
    /*handle the tx pdu (which include the gateway signal) transmit parameter*/
    Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
}
#endif
/**
 * @brief Called by Com_GwSignal.
 *        gateway signal/group signal/description signal which signal type isn't COM_UINT8_N and COM_UINT8_DYN
 *
 */
COM_LOCAL void Com_GwNotArraySignal(
    Com_SignalType       SignalType,
    uint16               GWSignalBufferId,
    Com_SignalConfigType DestSignalType,
    Com_SignalIdType     DestSignalId)
{
    switch (DestSignalType)
    {
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
    case COM_TX_SIGNAL:
        Com_GwNotArrayTxSignalHandle(SignalType, GWSignalBufferId, DestSignalId);
        break;
#endif
#if ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        Com_GwNotArrayTxGrpSigHandle(SignalType, GWSignalBufferId, DestSignalId);
        break;
#endif
#if (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)
    case COM_TX_DESCRIPTION:
        Com_GwNotArrayDestSigHandle(SignalType, GWSignalBufferId, DestSignalId);
        break;
#endif
    default:
        /* do nothing */
        break;
    }

    return;
}
/**
 * @brief Called by Com_GwUint8NDYNSignal.
 *        gateway signal which signal type is COM_UINT8_DYN,UINT8_N
 *
 */
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_TXSIGNAL_NUMBER)                                         \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_GwUint8NDYNTxSigHandle(
    Com_SignalType   SignalType,
    uint16           SignalBufferId,
    uint16           ComSignalLength,
    Com_SignalIdType DestSignalId)
{
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
    uint16                      rptNum                = 1u;
    const Com_TxSignalType*     txSignalPtr           = &Com_CfgTxSignalPtr[DestSignalId];
    PduIdType                   txIpduId              = txSignalPtr->IpduRefIndex;
    boolean                     valueChanged          = FALSE;
    const Com_TxIPduType*       txIpduPtr             = &Com_CfgTxIPduPtr[txIpduId];
    Com_MainFunctionIdType      txMainfunctionId      = txIpduPtr->TxMainfunctionId;
    PduIdType                   ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
    Com_TxIPduRunTimeStateType* txIpduStatePtr       = &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
    uint8*                      txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[txMainfunctionId];
    Com_SignalPositionType txIPduBufId = (Com_SignalPositionType)txIpduPtr->TxIPduBufIndex + txSignalPtr->SigLsbBytePos;
    uint16                 len         = ComSignalLength;
    uint16                 SigBufId =
        (COM_UINT8_DYN == SignalType) ? (SignalBufferId + (uint16)sizeof(ComSignalLength)) : SignalBufferId;
    SchM_Enter_Com_MultiCore_Lock();
    SchM_Enter_Com_Context();
    /* PRQA S 2934 ++ */ /* VL_Com_NullPtrValue */
    if (COM_UINT8_DYN == SignalType)
    {
        (void)IStdLib_MemCpy(&len, &Com_GWSignal8BitBuff[SignalBufferId], sizeof(len));
    }
    if (0 != IStdLib_MemCmp(&txIpduRuntimeBuffPtr[txIPduBufId], &Com_GWSignal8BitBuff[SigBufId], len))
    {
        (void)IStdLib_MemCpy(&txIpduRuntimeBuffPtr[txIPduBufId], &Com_GWSignal8BitBuff[SigBufId], len);
        valueChanged = TRUE;
    }
    /* PRQA S 2934 -- */
    SchM_Exit_Com_Context();
    SchM_Exit_Com_MultiCore_Lock();
    if (COM_UINT8_DYN == SignalType)
    {
        uint16 destSignalLength = txIpduStatePtr->TxIpduLength - (uint16)txIpduPtr->IPduNoDynSignalLength;
        if (len != destSignalLength)
        {
            valueChanged                 = TRUE;
            txIpduStatePtr->TxIpduLength = (uint16)txIpduPtr->IPduNoDynSignalLength + len;
        }
    }
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(
        txIPduBufId,
        txSignalPtr->UpdateLsbBytePos,
        txSignalPtr->UpdateBitMask,
        txIpduRuntimeBuffPtr);
#endif
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will
     * reset periodic mode counter.*/
    Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);

    /*handle the tx pdu (which include the gateway signal) transmit parameter*/
    Com_GwSignalOfPduHandle(txIpduStatePtr, txSignalPtr->ComTransferProperty, rptNum, valueChanged);
/*the tx signal configuration timeout*/
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
    if ((COM_UNUSED_TXSIGNALTIMEOUTID != txSignalPtr->TimeoutIndex) /* PRQA S 2895 */ /* VL_Com_NegtiveValueType */
        && ((0u == txIpduStatePtr->DMCnt) || (1u < txIpduStatePtr->RptNum)))
    {
        txIpduStatePtr->DMCnt = txIpduPtr->TxIpduDM;
    }
#endif
#endif
}
#endif
/**
 * @brief Called by Com_GwUint8NDYNSignal.
 *        gateway group signal which signal type is COM_UINT8_DYN,UINT8_N
 *
 */
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)                                                                           \
    && ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))                                 \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_GwUint8NDYNTxGrpSigHandle(
    Com_SignalType   SignalType,
    uint16           SignalBufferId,
    uint16           ComSignalLength,
    Com_SignalIdType DestSignalId)
{
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
    uint16                       rptNum           = 1u;
    const Com_TxGroupSignalType* txGroupSignalPtr = &Com_CfgTxGroupSignalPtr[DestSignalId];
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[txGroupSignalPtr->ComSignalGroupRef];
    PduIdType                    txIpduId         = txSignalGroupPtr->IpduRefIndex;
    Com_TransferPropertyType     signalTxProperty = (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty)
                                                        ? txGroupSignalPtr->ComTransferProperty
                                                        : txSignalGroupPtr->ComTransferProperty;
    boolean                      valueChanged     = FALSE;
    const Com_TxIPduType*        txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
    Com_MainFunctionIdType       txMainfunctionId = txIpduPtr->TxMainfunctionId;
    PduIdType                    ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
    Com_TxIPduRunTimeStateType*  txIpduStatePtr = &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
    uint8*                       txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[txMainfunctionId];
    Com_SignalPositionType       txIPduBufId =
        (Com_SignalPositionType)txIpduPtr->TxIPduBufIndex + txGroupSignalPtr->SigLsbBytePos;
    uint16 len = ComSignalLength;
    uint16 SigBufId =
        (COM_UINT8_DYN == SignalType) ? (SignalBufferId + (uint16)sizeof(ComSignalLength)) : SignalBufferId;
    SchM_Enter_Com_MultiCore_Lock();
    SchM_Enter_Com_Context();
    if (COM_UINT8_DYN == SignalType)
    {
        (void)IStdLib_MemCpy(&len, &Com_GWSignal8BitBuff[SignalBufferId], sizeof(len));
    }
    /* PRQA S 2934 ++ */ /* VL_Com_NullPtrValue */
    if (0 != IStdLib_MemCmp(&txIpduRuntimeBuffPtr[txIPduBufId], &Com_GWSignal8BitBuff[SigBufId], len))
    {
        (void)IStdLib_MemCpy(&txIpduRuntimeBuffPtr[txIPduBufId], &Com_GWSignal8BitBuff[SigBufId], len);
        valueChanged = TRUE;
    }
    /* PRQA S 2934 -- */
    SchM_Exit_Com_Context();
    SchM_Exit_Com_MultiCore_Lock();
    if (COM_UINT8_DYN == SignalType)
    {
        uint16 destSignalLength = txIpduStatePtr->TxIpduLength - (uint16)txIpduPtr->IPduNoDynSignalLength;
        if (len != destSignalLength)
        {
            valueChanged                 = TRUE;
            txIpduStatePtr->TxIpduLength = (uint16)txIpduPtr->IPduNoDynSignalLength + len;
        }
    }
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(
        txIPduBufId,
        txSignalGroupPtr->UpdateLsbBytePos,
        txSignalGroupPtr->UpdateBitMask,
        txIpduRuntimeBuffPtr);
#endif
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will
     * reset periodic mode counter.*/
    Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);

    /*handle the tx pdu (which include the gateway group signal) transmit parameter*/
    Com_GwSignalOfPduHandle(txIpduStatePtr, signalTxProperty, rptNum, valueChanged);
/*the tx signal group(include the gateway group signal)configuration timeout*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    if ((COM_UNUSED_TXSIGGRPTIMEOUTID != txSignalGroupPtr->TimeoutIndex) /* PRQA S 2895 */ /* VL_Com_NegtiveValueType */
        && ((0u == txIpduStatePtr->DMCnt) || (1u < txIpduStatePtr->RptNum)))
    {
        txIpduStatePtr->DMCnt = txIpduPtr->TxIpduDM;
    }
#endif
#endif
}
#endif
/**
 * @brief Called by Com_GwUint8NDYNSignal.
 *        gateway description signal which signal type is COM_UINT8_DYN,UINT8_N
 *
 */
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)                       \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_GwUint8NDYNDestSigHandle(
    Com_SignalType   SignalType,
    uint16           SignalBufferId,
    uint16           ComSignalLength,
    Com_SignalIdType DestSignalId)
{
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
    uint16                      rptNum                = 1u;
    const Com_GwDestSignalType* gwDestSignalPtr       = &Com_CfgGwMappingConfigPtr->DestSignal[DestSignalId];
    PduIdType                   txIpduId              = gwDestSignalPtr->ComGwIPduRef;
    boolean                     valueChanged          = FALSE;
    const Com_TxIPduType*       txIpduPtr             = &Com_CfgTxIPduPtr[txIpduId];
    Com_MainFunctionIdType      txMainfunctionId      = txIpduPtr->TxMainfunctionId;
    PduIdType                   ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
    Com_TxIPduRunTimeStateType* txIpduStatePtr       = &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
    uint8*                      txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[txMainfunctionId];
    Com_SignalPositionType      txIPduBufId =
        (Com_SignalPositionType)txIpduPtr->TxIPduBufIndex + gwDestSignalPtr->SigLsbBytePos;
    uint16 len = ComSignalLength;
    uint16 SigBufId =
        (COM_UINT8_DYN == SignalType) ? (SignalBufferId + (uint16)sizeof(ComSignalLength)) : SignalBufferId;
    SchM_Enter_Com_MultiCore_Lock();
    SchM_Enter_Com_Context();
    if (COM_UINT8_DYN == SignalType)
    {
        (void)IStdLib_MemCpy(&len, &Com_GWSignal8BitBuff[SignalBufferId], sizeof(len));
    }
    /* PRQA S 2934 ++ */ /* VL_Com_NullPtrValue */
    if (0 != IStdLib_MemCmp(&txIpduRuntimeBuffPtr[txIPduBufId], &Com_GWSignal8BitBuff[SigBufId], len))
    {
        (void)IStdLib_MemCpy(&txIpduRuntimeBuffPtr[txIPduBufId], &Com_GWSignal8BitBuff[SigBufId], len);
        valueChanged = TRUE;
    }
    /* PRQA S 2934 -- */
    SchM_Exit_Com_Context();
    SchM_Exit_Com_MultiCore_Lock();
    if (COM_UINT8_DYN == SignalType)
    {
        uint16 destSignalLength = txIpduStatePtr->TxIpduLength - (uint16)txIpduPtr->IPduNoDynSignalLength;
        if (len != destSignalLength)
        {
            valueChanged                 = TRUE;
            txIpduStatePtr->TxIpduLength = (uint16)txIpduPtr->IPduNoDynSignalLength + len;
        }
    }
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    /*if configuration update bit,set update bit to 1*/
    Com_SetSignalUpdateBit(
        txIPduBufId,
        gwDestSignalPtr->UpdateLsbBytePos,
        gwDestSignalPtr->UpdateBitMask,
        txIpduRuntimeBuffPtr);
#endif
    /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will
     * reset periodic mode counter.*/
    Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);

    /*handle the tx pdu (which include the gateway description signal) transmit parameter*/
    Com_GwSignalOfPduHandle(txIpduStatePtr, gwDestSignalPtr->ComTransferProperty, rptNum, valueChanged);
#endif
}
#endif
/**
 * @brief Called by Com_GwSignal.
 *        gateway signal/group signal/description signal which signal type is COM_UINT8_DYN,UINT8_N
 *
 */
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)                                                                           \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
COM_LOCAL void Com_GwUint8NDYNSignal(
    Com_SignalType       SignalType,
    uint16               SignalBufferId,
    uint16               ComSignalLength,
    Com_SignalConfigType DestSignalType,
    Com_SignalIdType     DestSignalId)
{
    switch (DestSignalType)
    {
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
    case COM_TX_SIGNAL:
        Com_GwUint8NDYNTxSigHandle(SignalType, SignalBufferId, ComSignalLength, DestSignalId);
        break;
#endif
#if ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER))
    case COM_TX_GROUP_SIGNAL:
        Com_GwUint8NDYNTxGrpSigHandle(SignalType, SignalBufferId, ComSignalLength, DestSignalId);
        break;
#endif
#if (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)
    case COM_TX_DESCRIPTION:
        Com_GwUint8NDYNDestSigHandle(SignalType, SignalBufferId, ComSignalLength, DestSignalId);
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    return;
}
#endif
/**
 * @brief gateway pdu(which include gateway signal and group signal) handle
 *
 */
#if (                                                                                    \
    ((0u < COM_MAX_TXSIGNAL_NUMBER) || (0u < COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER)) \
    || ((0u < COM_MAX_TXGROUPSIGNAL_NUMBER) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER)))
COM_LOCAL void Com_GwSignalOfPduHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    Com_TransferPropertyType    SignalTxProperty,
    uint16                      RptNum,
    boolean                     ValueChanged)
{
    Com_TxModeModeType ipduTxMode = TxIpduStatePtr->IpduTxMode;
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)                              \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)                         \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)                                                         \
    || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode)
        || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode)
        || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
    {
        switch (SignalTxProperty)
        {
        case COM_TRIGGERED:
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum   = RptNum;
            break;
        case COM_TRIGGERED_ON_CHANGE:
            if (ValueChanged)
            {
                TxIpduStatePtr->NTimeCnt = 0u;
                TxIpduStatePtr->RptNum   = RptNum;
            }
            break;
        case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
            if (ValueChanged)
            {
                TxIpduStatePtr->NTimeCnt = 0u;
                TxIpduStatePtr->RptNum   = 1u;
            }
            break;
        case COM_TRIGGERED_WITHOUT_REPETITION:
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum   = 1u;
            break;
        default:
            /*do nothing*/
            break;
        }
    }
    else
#endif
    {
        TxIpduStatePtr->NTimeCnt = 0u;
        TxIpduStatePtr->RptNum   = 0u;
    }
    COM_NOUSED(SignalTxProperty);
    COM_NOUSED(ValueChanged);
    return;
}
#endif

/**
 * @brief Get the GW Signal's New value and Old value
 *
 */
COM_LOCAL uint64 Com_GwSignalNewOldValueGet(
    Com_SignalType SignalType,
    uint16         GWSignalBufferId,
    uint16         TxSignalValueId,
    uint64*        SourceSignalValue,
    uint16         PartitionId)
{
    uint64 destSignalValue = 0u;
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_BOOLBUFF_SIZE)
    boolean* signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)
    uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_16BITBUFF_SIZE)
    uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_32BITBUFF_SIZE)
    uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_64BITBUFF_SIZE)
    uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[PartitionId];
#endif

    switch (SignalType)
    {
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        *SourceSignalValue                        = Com_GWSignalBoolBuff[GWSignalBufferId] ? 1u : 0u;
        destSignalValue                           = signalBoolRuntimeBuffPtr[TxSignalValueId] ? 1u : 0u;
        signalBoolRuntimeBuffPtr[TxSignalValueId] = Com_GWSignalBoolBuff[GWSignalBufferId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)
    case COM_UINT8:
    case COM_SINT8:
        *SourceSignalValue                        = (uint64)Com_GWSignal8BitBuff[GWSignalBufferId];
        destSignalValue                           = (uint64)signal8BitRuntimeBuffPtr[TxSignalValueId];
        signal8BitRuntimeBuffPtr[TxSignalValueId] = Com_GWSignal8BitBuff[GWSignalBufferId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        *SourceSignalValue                         = (uint64)Com_GWSignalBit16Buff[GWSignalBufferId];
        destSignalValue                            = (uint64)signalBit16RuntimeBuffPtr[TxSignalValueId];
        signalBit16RuntimeBuffPtr[TxSignalValueId] = Com_GWSignalBit16Buff[GWSignalBufferId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_32BITBUFF_SIZE)
    case COM_UINT32:
    case COM_SINT32:
    case COM_FLOAT32:
        *SourceSignalValue                         = (uint64)Com_GWSignal32BitBuff[GWSignalBufferId];
        destSignalValue                            = (uint64)signal32BitRuntimeBuffPtr[TxSignalValueId];
        signal32BitRuntimeBuffPtr[TxSignalValueId] = Com_GWSignal32BitBuff[GWSignalBufferId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE) && (0u < COM_MAX_GWSIGNAL_64BITBUFF_SIZE)
    case COM_UINT64:
    case COM_SINT64:
    case COM_FLOAT64:
        *SourceSignalValue                         = Com_GWSignal64BitBuff[GWSignalBufferId];
        destSignalValue                            = signal64BitRuntimeBuffPtr[TxSignalValueId];
        signal64BitRuntimeBuffPtr[TxSignalValueId] = Com_GWSignal64BitBuff[GWSignalBufferId];
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    COM_NOUSED(GWSignalBufferId);
    COM_NOUSED(TxSignalValueId);
    COM_NOUSED(SourceSignalValue);
    COM_NOUSED(PartitionId);
    return destSignalValue;
}

/**
 * @brief gateway signal/group signal/signal group/description signal
 *
 */
COM_LOCAL void Com_GwSignal(
    const Com_GwDestType* GwDestPtr,
    Com_SignalConfigType  GWSourceSignalType,
    Com_SignalIdType      GWSourceSignalId)
{
    Com_SignalType signalType;
    uint16         gwSignalBufferId;
    boolean        gwSigIsTrue;
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)
    uint16 signalLength = 0u;
#endif

    switch (GWSourceSignalType)
    {
#if (0u < COM_MAX_RXSIGNAL_NUMBER)
    case COM_RX_SIGNAL:
    {
        const Com_RxSignalType* rxSignalPtr = &Com_CfgRxSignalPtr[GWSourceSignalId];
        signalType                          = rxSignalPtr->ComSignalType;
        gwSignalBufferId                    = rxSignalPtr->GWSignalBufferId;
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE)
        signalLength = (COM_UINT8_N == signalType) ? rxSignalPtr->ComSignalLength : signalLength;
#endif
        gwSigIsTrue = TRUE;
        break;
    }
#endif
#if (0u < COM_MAX_RXGROUPSIGNAL_NUMBER)
    case COM_RX_GROUP_SIGNAL:
    {
        const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[GWSourceSignalId];
        signalType                                    = rxGroupSignalPtr->ComSignalType;
        gwSignalBufferId                              = rxGroupSignalPtr->GWSignalBufferId;
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        signalLength = (COM_UINT8_N == signalType) ? rxGroupSignalPtr->ComSignalLength : signalLength;
#endif
        gwSigIsTrue = TRUE;
        break;
    }
#endif
#if (0u < COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER)
    case COM_RX_DESCRIPTION:
    {
        const Com_GwSourceSignalType* gwSourceSignalPtr = &Com_CfgGwMappingConfigPtr->SourceSignal[GWSourceSignalId];
        signalType                                      = gwSourceSignalPtr->ComSignalType;
        gwSignalBufferId                                = gwSourceSignalPtr->GWSignalBufferId;

#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE)
        signalLength = (COM_UINT8_N == signalType) ? gwSourceSignalPtr->ComSignalLength : signalLength;
#endif
        gwSigIsTrue = TRUE;
        break;
    }
#endif
    default:
        gwSigIsTrue = FALSE;
        break;
    }

    if (gwSigIsTrue)
    {
        Com_SignalConfigType gwDestSignalType = GwDestPtr->GwDestSignalType;
        Com_SignalIdType     gwDestSignalId   = GwDestPtr->GwDestSignalId;
/*Except COM_UINT8_N,COM_UINT8_DYN Type Signal*/
#if (0u < COM_MAX_GWSIGNAL_8BITBUFF_SIZE)                                                                           \
    && ((STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)            \
        || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
        || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_GW_SRC_DSP_SIG_TYPE_UINT8_DYN_ENABLE))
        if ((COM_UINT8_N == signalType) || (COM_UINT8_DYN == signalType))
        {
            Com_GwUint8NDYNSignal(signalType, gwSignalBufferId, signalLength, gwDestSignalType, gwDestSignalId);
        }
        else
#endif
        {
            Com_GwNotArraySignal(signalType, gwSignalBufferId, gwDestSignalType, gwDestSignalId);
        }
    }
    COM_NOUSED(GWSourceSignalId);
    return;
}

#if COM_MAX_GWMAPPING_NUMBER > 0u
/**
 * @brief Init every destination request flag in the Gateway
 *
 */
void Com_GwInitDestinationRequest(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    (void)IStdLib_MemSet(&Com_GwDestinationRequest[0], 0, COM_MAX_GW_DESTINATION_NUM);
}
/**
 * @brief Set every destination request flag in the Gateway
 *
 */
void Com_GwUpdateDestinationRequest(Com_GwMappingIdType gwMappingId, boolean request)
{
    const Com_GwMappingType* gwMappingPtr = &Com_CfgGwMappingConfigPtr->GwMapping[gwMappingId];
    Com_SignalIdType         gwDestStart  = gwMappingPtr->GwDestSignalStartId;
    Com_SignalIdType         gwDestEnd    = gwMappingPtr->GwDestSignalEndId;
    for (Com_SignalIdType gwDestId = gwDestStart; gwDestId < gwDestEnd; ++gwDestId)
    {
        Com_GwDestinationRequest[gwDestId] = request;
    }
}
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif
/* PRQA S 0316,2844 -- */
/* PRQA S 2814,2824 -- */
/* PRQA S 3120,3132 -- */
