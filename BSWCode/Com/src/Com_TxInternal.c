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
 **  @file               : Com_TxInternal.c
 **  @author             : shengnan.sun
 **  @date               : 2024/09/01
 **  @vendor             : isoft
 **  @description        : internal implementation for COM transmit
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Com_Internal.h"
#include "Det.h"
/* ===================================================== macros ===================================================== */

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
 * @brief         Reset OCCURRENCE of filter with FilterAlgorithm ONE_EVERY_N.
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66404
 */
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
COM_LOCAL void Com_ResetTxOccurrenceOfPduFilter(const Com_TxIPduType* TxIpduPtr);
#endif
/**
 * @brief         Called by Com_TxIpduControl.init the tx ipdu buffer,all signal buffer (included in the ipdu) and
 *                init the TMC.
 * @param[in]     IpduId:Ipdu index
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduInitStatePtr:TxIpdu initValue state pointer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66405
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_TXIPDUGROUP_NUMBER > 0u)
COM_LOCAL void Com_ResetTxPduBufferAndSignalBuffer(
    PduIdType                         IpduId,
    Com_MainFunctionIdType            TxMainfunctionId,
    Com_TxIPduRunTimeStateType*       TxIpduStatePtr,
    const Com_TxIPduRunTimeStateType* TxIpduInitStatePtr);
#endif
/**
 * @brief         Pack tx group signal(not dynamic).
 * @param[in]     TxGroupSignalPtr:TxGroupSignal pointer
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66406
 */
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
COM_LOCAL void Com_TxGroupSignalPack(
    const Com_TxGroupSignalType* TxGroupSignalPtr,
    const Com_TxIPduType*        TxIpduPtr,
    Com_MainFunctionIdType       TxMainfunctionId);
#endif
/**
 * @brief         The group signal value changed,judge it will trigger calculate the new TMC of the signal new value.
 * @param[in]     DestSignalType:Dest signal type
 * @param[in]     SignalValuePtr:Signal value pinter:signalValue pointer
 * @param[in]     TxSignalTMCPtr:TxSignal TMC pointer
 * @param[in]     SignalOldValue:Signal old value
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66407
 */
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
COM_LOCAL boolean Com_TxSignalTMCCalculate(
    Com_SignalConfigType DestSignalType,
    const void*          SignalValuePtr,
    const void*          TxSignalTMCPtr,
    uint64               SignalOldValue);
#endif
/**
 * @brief         Change the signal value to uint64 type.
 * @param[in]     SignalType:Signaltype of signal/groupSignal
 * @param[in]     SignalValuePtr:Signal value pinter
 * @param[in]     ComBitSize:ComBitSize
 * @retval        signalNewValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66410
 */
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
COM_LOCAL uint64 Com_GetTxSignalValue(Com_SignalType SignalType, const void* SignalValuePtr, uint8 ComBitSize);
#endif
/**
 * @brief         Update the uint8_n signal value.
 * @param[in]     TxIPduBufferId:TxIpdu buffer index
 * @param[in]     GroupSignalId:GroupSignal index
 * @param[in]     GroupSignalInitValueId:GroupSignal initvalue index
 * @param[in]     PartitionId:Partition index
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @retval        triggerOnChange
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-69027
 */
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0u))
#if (                                                                                   \
    (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) && (0u < COM_MAX_TXIPDUBUFF_SIZE) \
    && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE))
COM_LOCAL boolean Com_TxGroupSignalUpdateUint8N(
    Com_TxIpduBufIdType    TxIPduBufferId,
    Com_SignalIdType       GroupSignalId,
    uint16                 GroupSignalInitValueId,
    uint16                 PartitionId,
    Com_MainFunctionIdType TxMainfunctionId);
#endif
#endif
/**
 * @brief         Send Signal Group(Array) handle
 * @param[in]     TxSignalGroupPtr:TxSignalGroup pointer
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TriggerOnChange:TriggerOnChange or not
 * @param[in]     RptNum:Repetition number
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66411
 */
#if ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API))
COM_LOCAL void Com_SendSignalGroupArrayInternalHandle(
    const Com_TxSignalGroupType* TxSignalGroupPtr,
    Com_TxIPduRunTimeStateType*  TxIpduStatePtr,
    const Com_TxIPduType*        TxIpduPtr,
    boolean                      TriggerOnChange,
    uint16                       RptNum);
#endif
/**
 * @brief         Trigger IPdu Send handle
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     IpduId:Ipdu index
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66412
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
COM_LOCAL Std_ReturnType Com_TriggerIPDUSendInternalHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    PduIdType IpduId,
#endif
    Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         Get TxModeTimeOffset of True/False TxMode
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TMS:TMS
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66413
 */
COM_LOCAL void
    Com_GetTxModeOffset(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean TMS);
/**
 * @brief         Tx pdu DM timeout notification.
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66414
 */
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
COM_LOCAL void Com_TxDMTimeOutNotification(const Com_TxIPduType* TxIpduPtr);
#endif
/**
 * @brief         Calculate DM and MDT
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @retval        ipduMDTRun:ipduMDTRun
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
COM_LOCAL boolean
    Com_MainTxDMTimeoutMdtCal(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr);
#endif
/**
 * @brief         When the tx mode is direct.return the boolean value for the tx pdu need to transmit or not.
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     MDTRun:Monitor delay timer
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66431
 */
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)   \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER)                                 \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)                                \
    || ((0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
        || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)                              \
        || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
COM_LOCAL boolean
    Com_MainFuncTxDirect(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun);
#endif
/**
 * @brief         When the tx mode is period.
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     MDTRun:Monitor delay timer
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66433
 */
#if (0u < COM_MAX_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER)  \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
COM_LOCAL boolean
    Com_MainFuncTxPeriod(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun);
#endif
/**
 * @brief         When the tx mode is mixed.return the boolean value for the tx pdu need to transmit or not.
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     MDTRun:Monitor delay timer
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66435
 */
#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
COM_LOCAL boolean
    Com_MainFuncTxMixed(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun);
#endif
/**
 * @brief         It is called in case the transmission is not possible because the corresponding I-PDU group is
 * stopped.
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66436
 */
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
COM_LOCAL void Com_InvokeErrorNotification(const Com_TxIPduType* TxIpduPtr);
#endif
/**
 * @brief         Send the tx pdu in Com_MainFunctionTx
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxIpduId:TxIpdu index
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66437
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
COM_LOCAL void Com_MainFunction_SendPdu(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    PduIdType TxIpduId,
#endif
    Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         Get signal old value
 * @param[in]     SignalType:Signaltype of signal/groupSignal
 * @param[in]     SignalBufferId:Signal buffer index
 * @param[in]     PartitionId:Partition index
 * @retval        oldValue
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66438
 */
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
COM_LOCAL uint64 Com_GetOldSignalValue(Com_SignalType SignalType, uint16 SignalBufferId, uint16 PartitionId);
#endif
/**
 * @brief         The tx signal value change,calculate the new TMC and TMS.
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxSignalId:TxSignal index
 * @param[in]     TxSignalDataPtr:TxSignal data pointer
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66439
 */
#if (0u < COM_MAX_TXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
COM_LOCAL void Com_TxSignalTMHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
    Com_SignalIdType            TxSignalId,
    const void*                 TxSignalDataPtr,
    Com_MainFunctionIdType      TxMainfunctionId);
#endif
/**
 * @brief         Set Tx signal(not dynamic signal) to signalbuffer,return the buffer value is changed or not.
 * @param[in]     SignalType:Signaltype of signal/groupSignal
 * @param[in]     ComSignalLength:Length of signal/groupSignal
 * @param[in]     SignalBufferId:Signal buffer index
 * @param[in]     SignalNewDataPtr:Signal new data pointer
 * @param[in]     ComBitSize:ComBitSize
 * @param[in]     PartitionId:Partition index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-68737
 */
#if ((COM_MAX_TXSIGNALGROUP_NUMBER > 0u) || (0u < COM_MAX_TXSIGNAL_NUMBER))
COM_LOCAL boolean Com_SetTxSignalBuff(
    Com_SignalType SignalType,
    uint16         ComSignalLength,
    uint16         SignalBufferId,
    const void*    SignalNewDataPtr,
    uint8          ComBitSize,
    uint16         PartitionId);
#endif
/**
 * @brief         Pack the Tx signal into ipdu Tx buffer.
 * @param[in]     TxSignalId:TxSignal index
 * @param[in]     PartitionId:Partition index
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66440
 */
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
COM_LOCAL void
    Com_TxSignalPack(const Com_TxSignalType* txSignalPtr, uint16 PartitionId, Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         Called by Com_SendSignal,Com_SendDynSignal.send Tx signal handle.
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     SignalTxProperty:Property of TxSignal
 * @param[in]     DMSignal:Deadline monitor signal
 * @param[in]     ValueChanged:Value is changed or not
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66441
 */
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
COM_LOCAL uint8 Com_SendTxSignalHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    Com_TransferPropertyType    SignalTxProperty,
    boolean                     DMSignal,
    boolean                     ValueChanged);
#endif
/**
 * @brief         Called by Com_SendSignal,Com_InvalidateSignalGroup.send Tx group signal handle.
 * @param[in]     TxGroupSignalId:TxGroupSignal index
 * @param[in]     SignalDataPtr:Signal data pointer
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66442
 */
#if (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)
COM_LOCAL uint8 Com_SendTxGroupSignalHandle(Com_SignalIdType TxGroupSignalId, const void* SignalDataPtr);
#endif
/**
 * @brief         Set Tx dynamic signal to signalbuffer,return the buffer value is changed or not.
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxRuntimeStatePtr:TxIpdu runtime state pointer
 * @param[in]     Signal8BitRuntimeBuffPtr:Signal runtime 8 bit buffer index
 * @param[in]     SignalNewDataPtr:Signal new data pointer
 * @param[in]     Length:Length
 * @retval        ret
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66443
 */
#if (                                                                                            \
    ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)) \
    || ((0u < COM_MAX_TXSIGNAL_NUMBER) && (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)))
COM_LOCAL boolean Com_SetDynTxSignalBuff(
    const Com_TxIPduType*             TxIpduPtr,
    const Com_TxIPduRunTimeStateType* TxRuntimeStatePtr,
    uint8*                            Signal8BitRuntimeBuffPtr,
    const void*                       SignalNewDataPtr,
    uint16                            Length);
#endif
/**
 * @brief         Pack the Tx signal group into ipdu Tx buffer.
 * @param[in]     SignalGroupId:SignalGroup index
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66444
 */
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
COM_LOCAL void Com_TxSignalGroupPack(Com_SignalGroupIdType SignalGroupId, Com_MainFunctionIdType TxMainfunctionId);
#endif
/**
 * @brief         Update every group signal buffer,calculate every TMC,return the signal group is changed or not
 *                for trigger transmit.
 * @param[in]     TxSignalGroupId:TxSignalGroup index
 * @param[in]     GroupSignalNumber:GroupSignal number
 * @param[in]     TxIPduBufferId:TxIpdu buffer index
 * @retval        triggerOnChange
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66445
 */
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0u))
COM_LOCAL boolean Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal(
    Com_SignalGroupIdType   TxSignalGroupId,
    Com_TxGroupSignalIdType GroupSignalNumber,
    Com_TxIpduBufIdType     TxIPduBufferId);
#endif
/* PRQA S 5015 -- */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 1532,2895 ++ */ /* VL_Com_ReferencedOnlyOne,VL_Com_NegtiveValueType */
/**
 * @brief Send SignalGroup(Array) handle
 *
 */
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (0u < COM_MAX_TXIPDU_NUMBER)
uint8 Com_SendSignalGroupArrayHandle(Com_SignalGroupIdType SignalGroupId, const uint8* SignalGroupArrayPtr)
{
    uint8                        ret              = COM_SERVICE_NOT_AVAILABLE;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[SignalGroupId];
    Com_MainFunctionIdType       txMainfunctionId = txSignalGroupPtr->TxMainfunctionId;
    boolean                      triggerOnChange;
    uint16                       rptNum = 1u;

    /*the signal group isn't InitialValueOnly,the signal group is array*/
    if ((!txSignalGroupPtr->ComInitialValueOnly)
        && (COM_UNUSED_COM_SIGNALGROUPARRYID != txSignalGroupPtr->SignalGroupArrayId))
    {
        PduIdType                   ipduRef               = txSignalGroupPtr->IpduRefIndex;
        const Com_TxIPduType*       txIpduPtr             = &Com_CfgTxIPduPtr[ipduRef];
        PduIdType                   ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionTx(txMainfunctionId);
        Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
        /*the IPDU(TP) is sending*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
        if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (COM_TX_TRANSMITING_EN == Transmiting))
        {
            ret = COM_BUSY;
        }
        else
#endif
        {
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
            const Com_SignalGroupArrayType* signalGroupArray =
                &Com_ConfigStd->SignalGroupArrayPtr[txSignalGroupPtr->SignalGroupArrayId];
            uint16 iPduSignalGroupArrayStartBufId = signalGroupArray->IPduStartBufIndex;
            uint8* txIpduRuntimeBuffPtr           = Com_TxIPduRuntimeBuff[txMainfunctionId];
            /*pack the Tx signal group(array) into ipdu Tx buffer*/
            SchM_Enter_Com_Context();
            (void)IStdLib_MemCpy(
                &txIpduRuntimeBuffPtr[iPduSignalGroupArrayStartBufId],
                SignalGroupArrayPtr,
                signalGroupArray->SignalGroupArrayLength);
            SchM_Exit_Com_Context();
#endif
            Com_TxIpduBufIdType     txIPduBufferId   = txIpduPtr->TxIPduBufIndex;
            Com_TxGroupSignalIdType groupSignalEndId = txSignalGroupPtr->GroupSignalRefEndId;
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
            /*set the update bit if the signal group configuration */
            Com_SetSignalUpdateBit(
                txIPduBufferId,
                txSignalGroupPtr->UpdateLsbBytePos,
                txSignalGroupPtr->UpdateBitMask,
                txIpduRuntimeBuffPtr);
#endif
            /*update every group signal buffer,calculate every TMC,return the signal group is changed or not for
             * trigger transmit*/
            triggerOnChange =
                Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal(SignalGroupId, groupSignalEndId, txIPduBufferId);
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will
             * reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
            if ((txIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN) == COM_TX_ACTIVE_EN)
            {
                Com_SendSignalGroupArrayInternalHandle(
                    txSignalGroupPtr,
                    txIpduStatePtr,
                    txIpduPtr,
                    triggerOnChange,
                    rptNum);
                ret = E_OK;
            }
#if (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
            else
            {
                /*call Tx Err Notification*/
                if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
                    == (txSignalGroupPtr->TxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
                {
                    Com_ConfigStd->TxErrorAndNotificationPtr
                        ->TxSignalGroupErrorAndNotification[txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId]();
                }
            }
#endif
        }
    }
    COM_NOUSED(SignalGroupId);
    COM_NOUSED(SignalGroupArrayPtr);
    return ret;
}
#endif

/**
 * @brief Called by Com_SendDynSignal,Com_InvalidateSignalGroup.
 *        send dynamic Tx group signal handle.
 *
 */
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
uint8 Com_SendTxDynGroupSignalHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    Com_SignalIdType            TxGroupSignalId,
    const void*                 SignalDataPtr,
    uint16                      DynLength)
{
    uint8                        ret;
    const Com_TxGroupSignalType* txGroupSignalPtr = &Com_CfgTxGroupSignalPtr[TxGroupSignalId];
    Com_TxSignalGroupIdType      signalGroupId    = txGroupSignalPtr->ComSignalGroupRef;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
    Com_MainFunctionIdType       txMainfunctionId = txSignalGroupPtr->TxMainfunctionId;
    /*group signal(not dynamic) isn't send by the API,InitialValueOnly signal group don't expect to be called by any
     * API*/
    if ((COM_UINT8_DYN != txGroupSignalPtr->ComSignalType) || txSignalGroupPtr->ComInitialValueOnly)
    {
        ret = COM_SERVICE_NOT_AVAILABLE;
    }
    else
    {
        if (DynLength > txGroupSignalPtr->ComSignalLength)
        {
#if (STD_ON == COM_DEV_ERROR_DETECT)
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_PARAM);
#endif
            ret = E_NOT_OK;
        }
        else
        {
            Com_TxSignalGroupRunTimeStateType* txSigGrpStPtr            = &Com_TxSignalGroupState[signalGroupId];
            PduIdType                          ipduRef                  = txSignalGroupPtr->IpduRefIndex;
            uint16                             groupSignalBufferId      = txGroupSignalPtr->SignalInitValueId;
            const Com_TxIPduType*              txIpduPtr                = &Com_CfgTxIPduPtr[ipduRef];
            uint16                             partitionId              = txIpduPtr->IpduPartitionId;
            uint8*                             signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[partitionId];
            /*update Dynamic tx group signal buffer and check if the sending signal value changed*/
            boolean valueChanged = Com_SetDynTxSignalBuff(
                txIpduPtr,
                (const Com_TxIPduRunTimeStateType*)TxIpduStatePtr,
                signal8BitRuntimeBuffPtr,
                SignalDataPtr,
                DynLength);
            txSigGrpStPtr->DynamicSignalLength = DynLength;
            Com_TransferPropertyType groupSignalTxProperty =
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
                (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty)
                    ? txGroupSignalPtr->ComTransferProperty
                    :
#endif
                    txSignalGroupPtr->ComTransferProperty;
            switch (groupSignalTxProperty)
            {
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE)
            case COM_TRIGGERED:
                txSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                txSigGrpStPtr->TxSigGrpRTStFlag &= COM_WITHOUTREPETITION_DIS;
                break;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
            case COM_TRIGGERED_ON_CHANGE:
                if (valueChanged)
                {
                    txSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                    txSigGrpStPtr->TxSigGrpRTStFlag &= COM_WITHOUTREPETITION_DIS;
                }
                break;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)
            case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
                if (valueChanged)
                {
                    txSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                    txSigGrpStPtr->TxSigGrpRTStFlag |= COM_WITHOUTREPETITION_EN;
                }
                break;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE)
            case COM_TRIGGERED_WITHOUT_REPETITION:
                txSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                txSigGrpStPtr->TxSigGrpRTStFlag |= COM_WITHOUTREPETITION_EN;
                break;
#endif
            default:
                /*do nothing*/
                break;
            }
            ret = E_OK;
        }
    }
    return ret;
}
#endif

/**
 * @brief Called by Com_TxConfirmation,Com_TpTxConfirmation,Com_MainFunctionTx.
 *        Tx notification for all signals/signal groups of the Tx Pdu
 *
 */
#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
/* PRQA S 1505 ++ */ /* VL_Com_1505 */
void Com_ConfirmationProcess(const Com_TxIPduType* TxIpduPtr)
/* PRQA S 1505 -- */ /* VL_Com_1505 */
{
#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalIdType txSignalNumber, cnt;
    txSignalNumber = TxIpduPtr->IPduSignalsRefEndId;
    cnt            = TxIpduPtr->IpduSignalRefStartId;
    for (; cnt < txSignalNumber; ++cnt)
    {
        const Com_TxSignalType* txSignalPtr = &Com_CfgTxSignalPtr[cnt];
        /*call signal group Tx Notification*/
        if (COM_TX_SIGNAL_NOTIFICATION_EN == (txSignalPtr->TxSignalNotificationType & COM_TX_SIGNAL_NOTIFICATION_EN))
        {
            Com_TxSigErrAndNotifyIdType txSignalErrAndNotifyFncId =
                (COM_TX_SIGNAL_ERROR_AND_NOTIFICATION == txSignalPtr->TxSignalNotificationType)
                    ? (txSignalPtr->TxSignalErrAndNotifyFncId + 1u)
                    : txSignalPtr->TxSignalErrAndNotifyFncId;
            Com_ConfigStd->TxErrorAndNotificationPtr->TxSignalErrorAndNotification[txSignalErrAndNotifyFncId]();
        }
    }
#endif
#if (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    signalGroupNumber = TxIpduPtr->IPduSignalGroupsRefEndId;
    signalGroupId     = TxIpduPtr->IPduSignalGroupsRefStartId;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
        /*call signal group Tx Notification*/
        if (COM_TX_SIGNAL_NOTIFICATION_EN
            == (txSignalGroupPtr->TxSignalNotificationType & COM_TX_SIGNAL_NOTIFICATION_EN))
        {
            Com_TxSigGrpErrAndNotifyIdType txSigGrpErrAndNotifyFncId =
                (COM_TX_SIGNAL_ERROR_AND_NOTIFICATION == txSignalGroupPtr->TxSignalNotificationType)
                    ? (txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId + 1u)
                    : txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId;
            Com_ConfigStd->TxErrorAndNotificationPtr->TxSignalGroupErrorAndNotification[txSigGrpErrAndNotifyFncId]();
        }
    }
#endif
    COM_NOUSED(TxIpduPtr);
    return;
}
#endif

/**
 * @brief Send Signal handle
 *
 */
#if (((COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_MAX_TXIPDU_NUMBER))
uint8 Com_SendSignalHandle(
    Com_SignalIdType       SignalId,
    const void*            SignalDataPtr,
    Com_MainFunctionIdType TxMainfunctionId)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
    /*the signal is Tx Signal*/
    if (SignalId < Com_CfgTxSignalNum)
    {
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
        const Com_TxSignalType* txSignalPtr = &Com_CfgTxSignalPtr[SignalId];
        Com_SignalType          signalType  = txSignalPtr->ComSignalType;
        /*dynamic signal isn't send by the API,InitialValueOnly signal don't expect to be called by any API*/
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIG_INITIAL_VALUE_ONLY_ENABLE)
        if ((COM_UINT8_DYN != signalType) && (!txSignalPtr->ComInitialValueOnly))
#endif
        {
            PduIdType             ipduRef               = txSignalPtr->IpduRefIndex;
            const Com_TxIPduType* txIpduPtr             = &Com_CfgTxIPduPtr[ipduRef];
            PduIdType             ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionTx(TxMainfunctionId);
            Com_TxIPduRunTimeStateType* txIpduStatePtr =
                &Com_TxIPduRunTimeState[TxMainfunctionId][ipduIdPerMainfunction];
            /*the IPDU(TP) is sending,return busy*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
            uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
            if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (COM_TX_TRANSMITING_EN == Transmiting))
            {
                ret = COM_BUSY;
            }
            else
#endif
            {
#if (STD_ON == COM_TMS_ENABLE)
                Com_TxSignalTMHandle(txIpduStatePtr, txIpduPtr, SignalId, SignalDataPtr, TxMainfunctionId);
#endif
                /*signalLength is used when the signal type is COM_UINT8_N*/
                uint16 signalLength   = txSignalPtr->ComSignalLength;
                uint16 signalBufferId = txSignalPtr->SignalInitValueId;
                /*update tx signal buffer and check if the sending signal value changed*/
                boolean valueChanged = Com_SetTxSignalBuff(
                    signalType,
                    signalLength,
                    signalBufferId,
                    SignalDataPtr,
                    txSignalPtr->ComBitSize,
                    txIpduPtr->IpduPartitionId);
                if (valueChanged)
                {
                    /*pack the Tx signal into ipdu Tx buffer*/
                    Com_TxSignalPack(txSignalPtr, txIpduPtr->IpduPartitionId, TxMainfunctionId);
                }
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
                /*handle update bit function. */
                Com_SignalPositionType updateBitPosition    = txSignalPtr->UpdateLsbBytePos;
                uint8                  updateBitMask        = txSignalPtr->UpdateBitMask;
                uint8*                 txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
                Com_SetSignalUpdateBit(
                    txIpduPtr->TxIPduBufIndex,
                    updateBitPosition,
                    updateBitMask,
                    txIpduRuntimeBuffPtr);
#endif
                /*the Tx Pdu is enabled*/
                if ((txIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN) == COM_TX_ACTIVE_EN)
                {
                    boolean signalIsDM =
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
                        (COM_UNUSED_TXSIGNALTIMEOUTID != txSignalPtr->TimeoutIndex) ? TRUE :
#endif
                                                                                    FALSE;
                    Com_TransferPropertyType signalTxProperty = txSignalPtr->ComTransferProperty;
                    ret = Com_SendTxSignalHandle(txIpduPtr, txIpduStatePtr, signalTxProperty, signalIsDM, valueChanged);
                }
                /*the Tx Pdu is disabled*/
#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
                else
                {
                    /*call Tx Err Notification*/
                    if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
                        == (txSignalPtr->TxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
                    {
                        const Com_ErrorAndNotificationType* txSignalErrorAndNotificationPtr =
                            Com_ConfigStd->TxErrorAndNotificationPtr->TxSignalErrorAndNotification;
                        txSignalErrorAndNotificationPtr[txSignalPtr->TxSignalErrAndNotifyFncId]();
                    }
                }
#endif
            }
        }
#endif
    }
#if (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)
    /*the signal is Tx Group Signal*/
    else
    {
        Com_SignalIdType groupSignalId = SignalId - Com_CfgTxSignalNum;
        ret                            = Com_SendTxGroupSignalHandle(groupSignalId, SignalDataPtr);
    }
#endif
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    return ret;
}

/**
 * @brief Send Dynamic Signal handle
 *
 */
uint8 Com_SendDynSignalHandle(
    Com_SignalIdType       SignalId,
    const void*            SignalDataPtr,
    uint16                 Length,
    Com_MainFunctionIdType TxMainfunctionId)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    const Com_TxSignalType* txSignalPtr = &Com_CfgTxSignalPtr[SignalId];
#endif
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    PduIdType ipduRef               = txSignalPtr->IpduRefIndex;
    PduIdType ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionTx(TxMainfunctionId);
#endif

    /*the signal is Tx Signal*/
    if (SignalId < COM_MAX_TXSIGNAL_NUMBER)
    {
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[TxMainfunctionId][ipduIdPerMainfunction];
        /*signal(not dynamic) isn't send by the API,InitialValueOnly signal don't expect to be called by any API*/
        if ((COM_UINT8_DYN == txSignalPtr->ComSignalType) && (!txSignalPtr->ComInitialValueOnly))
        {
            const Com_TxIPduType* txIpduPtr = &Com_CfgTxIPduPtr[ipduRef];
            /*the IPDU(TP) is sending,return busy*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
            uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
            if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (COM_TX_TRANSMITING_EN == Transmiting))
            {
                ret = COM_BUSY;
            }
            else
#endif
            {
                if (Length > txSignalPtr->ComSignalLength)
                {
#if (STD_ON == COM_DEV_ERROR_DETECT)
                    (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, COM_E_PARAM);
#endif
                    ret = E_NOT_OK;
                }
                else
                {
                    uint16 signalBufferId           = txSignalPtr->SignalInitValueId;
                    uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[txIpduPtr->IpduPartitionId];
                    /*update tx Dyn signal buffer and check if the sending signal value changed*/
                    boolean valueChanged = Com_SetDynTxSignalBuff(
                        txIpduPtr,
                        (const Com_TxIPduRunTimeStateType*)txIpduStatePtr,
                        &signal8BitRuntimeBuffPtr[signalBufferId],
                        SignalDataPtr,
                        Length);
                    uint8* txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
                    if (valueChanged)
                    {
                        /*pack the Tx Dynamic signal into ipdu Tx buffer*/
#if ((0u < COM_MAX_TXIPDUBUFF_SIZE) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE))
                        /* Com_TxDynSignalPack */
                        Com_TxIpduBufIdType ipduBufferIndex =
                            txIpduPtr->TxIPduBufIndex + (Com_TxIpduBufIdType)txSignalPtr->SigLsbBytePos;
                        SchM_Enter_Com_Context();
                        (void)IStdLib_MemCpy(
                            &txIpduRuntimeBuffPtr[ipduBufferIndex],
                            &signal8BitRuntimeBuffPtr[signalBufferId],
                            Length);
                        SchM_Exit_Com_Context();
#endif
                        txIpduStatePtr->TxIpduLength = (uint16)txIpduPtr->IPduNoDynSignalLength + Length;
                    }
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
                    /*handle update bit function. */
                    Com_SignalPositionType updateBitPosition = txSignalPtr->UpdateLsbBytePos;
                    uint8                  updateBitMask     = txSignalPtr->UpdateBitMask;
                    Com_SetSignalUpdateBit(
                        txIpduPtr->TxIPduBufIndex,
                        updateBitPosition,
                        updateBitMask,
                        txIpduRuntimeBuffPtr);
#endif
                    /*the Tx Pdu is enabled*/
                    if ((txIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN) == COM_TX_ACTIVE_EN)
                    {
                        boolean signalIsDM =
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
                            (COM_UNUSED_TXSIGNALTIMEOUTID != txSignalPtr->TimeoutIndex) ? TRUE :
#endif
                                                                                        FALSE;
                        Com_TransferPropertyType signalTxProperty = txSignalPtr->ComTransferProperty;
                        ret                                       = Com_SendTxSignalHandle(
                            txIpduPtr,
                            txIpduStatePtr,
                            signalTxProperty,
                            signalIsDM,
                            valueChanged);
                    }
                    /*the Tx Pdu is disabled*/
#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
                    else
                    {
                        /*call Tx Err Notification*/
                        if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
                            == (txSignalPtr->TxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
                        {
                            const Com_Tx_ErrorAndNotificationType* txErrorAndNotificationPtr =
                                Com_ConfigStd->TxErrorAndNotificationPtr;
                            txErrorAndNotificationPtr
                                ->TxSignalErrorAndNotification[txSignalPtr->TxSignalErrAndNotifyFncId]();
                        }
                    }
#endif
                }
            }
        }
#endif
    }
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
    /*the signal is Tx Group Signal*/
    else
    {
        Com_SignalIdType groupSignalId = SignalId - Com_CfgTxSignalNum;
        ret                            = Com_SendTxDynGroupSignalHandle(
            &Com_TxIPduRunTimeState[TxMainfunctionId][ipduIdPerMainfunction],
            groupSignalId,
            SignalDataPtr,
            Length);
    }
#endif
    COM_NOUSED(Length);
    COM_NOUSED(TxMainfunctionId);
    COM_NOUSED(SignalDataPtr);
    return ret;
}
#endif

#if ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (0u < COM_MAX_TXIPDU_NUMBER))
/******************************************************************************/
/*
 * Brief               Send SignalGroup handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      SignalGroupId
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              uint8
 */
/******************************************************************************/
uint8 Com_SendSignalGroupHandle(Com_SignalGroupIdType SignalGroupId, Com_MainFunctionIdType TxMainfunctionId)
{
    uint8  ret    = COM_SERVICE_NOT_AVAILABLE;
    uint16 rptNum = 1u;
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    boolean initDMCnt = FALSE;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition;
    uint8                  updateBitMask;
#endif
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[SignalGroupId];

    /*the signal group is configuration ComInitialValueOnly,don't expect to be called by any API*/
#if (STD_ON == COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE)
    if (!txSignalGroupPtr->ComInitialValueOnly)
#endif
    {
        PduIdType                   ipduRef               = txSignalGroupPtr->IpduRefIndex;
        PduIdType                   ipduStartId           = Com_GetStartOfMainFunctionTx(TxMainfunctionId);
        PduIdType                   ipduIdPerMainfunction = ipduRef - ipduStartId;
        Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[TxMainfunctionId][ipduIdPerMainfunction];
        const Com_TxIPduType*       txIpduPtr      = &Com_CfgTxIPduPtr[ipduRef];
        /*the IPDU(TP) is sending*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
        if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (COM_TX_TRANSMITING_EN == Transmiting))
        {
            ret = COM_BUSY;
        }
        else
#endif
        {
            /*pack the Tx signal group into ipdu Tx buffer,copy Com_TxGroupSignalTMCBuffer to
             * Com_TxSignalTMCRunTime*/
            Com_TxSignalGroupPack(SignalGroupId, TxMainfunctionId);
            /*update the Tx Pdu length(if the signal group include dynamic group signal)*/
            /* PRQA S 2934 ++ */ /* VL_Com_NullPtrValue */
            Com_TxSignalGroupRunTimeStateType* txSigGrpStPtr = &Com_TxSignalGroupState[SignalGroupId];
            /* PRQA S 2934 -- */
            txIpduStatePtr->TxIpduLength =
                ((uint16)txIpduPtr->IPduNoDynSignalLength + txSigGrpStPtr->DynamicSignalLength);
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
            /*handle update bit function. */
            updateBitPosition           = txSignalGroupPtr->UpdateLsbBytePos;
            updateBitMask               = txSignalGroupPtr->UpdateBitMask;
            uint8* txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
            Com_SetSignalUpdateBit(txIpduPtr->TxIPduBufIndex, updateBitPosition, updateBitMask, txIpduRuntimeBuffPtr);
#endif
            /*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,if TMS changed will
             * reset periodic mode counter.*/
            Com_TxIpduTMSHandle(txIpduStatePtr, txIpduPtr, &rptNum);
            /*the Tx Pdu is enabled*/
            if ((txIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN) == COM_TX_ACTIVE_EN)
            {
                /*the signal group configuration timeout,the DM timer isn't start by other signals/signal groups,Tx
                 * Mode isn't NONE*/
                Com_TxModeModeType ipduTxMode = txIpduStatePtr->IpduTxMode;
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
                if ((COM_UNUSED_TXSIGGRPTIMEOUTID != txSignalGroupPtr->TimeoutIndex) && (COM_TX_MODE_NONE != ipduTxMode)
                    && (0u == txIpduStatePtr->DMCnt))
                {
                    /*restart the tx DM timer*/
                    txIpduStatePtr->DMCnt = txIpduPtr->TxIpduDM;
                    initDMCnt             = TRUE;
                }
#endif

#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)                              \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)                         \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)                                                         \
    || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
                if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode)
                    || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode)
                    || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
                {
                    uint8 NTimesTransmitNeed = txSigGrpStPtr->TxSigGrpRTStFlag & COM_NTIMESTRANSMITNEED_EN;
                    if (COM_NTIMESTRANSMITNEED_EN == NTimesTransmitNeed)
                    {
                        uint8 WithoutRepetition  = txSigGrpStPtr->TxSigGrpRTStFlag & COM_WITHOUTREPETITION_EN;
                        txIpduStatePtr->NTimeCnt = 0u;
                        txIpduStatePtr->RptNum   = (COM_WITHOUTREPETITION_EN == WithoutRepetition) ? 1u : rptNum;
                        /*For an I-PDU with ComTxModeMode DIRECT and TxModeNumberOfRepetitions > 0, the AUTOSAR
                          COM module shall reset an already running transmission deadline monitoring timer in case
                          another send request for this I-PDU is initiated*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
                        if ((1u < txIpduStatePtr->RptNum) && (!initDMCnt)
                            && (COM_UNUSED_TXSIGGRPTIMEOUTID != txSignalGroupPtr->TimeoutIndex))
                        {
                            txIpduStatePtr->DMCnt = txIpduPtr->TxIpduDM;
                        }
#endif
                        txSigGrpStPtr->TxSigGrpRTStFlag &= COM_NTIMESTRANSMITNEED_DIS;
                    }
                }
                else
#endif
                {
                    txIpduStatePtr->NTimeCnt = 0u;
                    txIpduStatePtr->RptNum   = 0u;
                }
                ret = E_OK;
            }
#if (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
            else
            {
                /*call Tx Err Notification*/
                Com_ConfigStd->TxErrorAndNotificationPtr
                    ->TxSignalGroupErrorAndNotification[txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId]();
            }
#endif
        }
    }
    return ret;
}
#endif

/**
 * @brief Send Invalidate Signal handle
 *
 */
#if ((COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_MAX_TXIPDU_NUMBER)
uint8 Com_InvalidateSignalHandle(Com_SignalIdType SignalId)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;
#if (STD_ON == COM_TX_SIGNAL_INVALID_DATA_ENABLE)
    const Com_TxSignalType*   txSignalPtr     = &Com_CfgTxSignalPtr[SignalId];
    Com_SignalType            signalType      = txSignalPtr->ComSignalType;
    Com_TxSignalInvalidIdType signalInvalidId = txSignalPtr->SignalInvalidRefId;

    if ((COM_UNUSED_TXSIGNALINVALIDID != signalInvalidId) && (!txSignalPtr->ComInitialValueOnly))
    {
        const Com_SignalInvalidValueType* txSignalDataInvalidValuePtr = Com_ConfigStd->SignalInvalidValuePtr;
        uint16 invalidValueId = txSignalDataInvalidValuePtr->TxSignalDataInvalidValueId[signalInvalidId];
#if (0u < COM_MAX_SIGNAL_BOOL_INVALID_SIZE)
        const boolean* signalBoolInvalidValuePtr = txSignalDataInvalidValuePtr->SignalBoolInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
        const uint8* signal8BitInvalidValuePtr = txSignalDataInvalidValuePtr->Signal8BitInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_16BIT_INVALID_SIZE)
        const uint16* signalBit16InvalidValuePtr = txSignalDataInvalidValuePtr->SignalBit16InvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_32BIT_INVALID_SIZE)
        const uint32* signal32BitInvalidValuePtr = txSignalDataInvalidValuePtr->Signal32BitInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_64BIT_INVALID_SIZE)
        const uint64* signal64BitInvalidValuePtr = txSignalDataInvalidValuePtr->Signal64BitInvalidValue;
#endif
        /* It have judged the InvalidValueId is not null, so the ValuePtr may be not to judge*/
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == signalType)
        {
#if (0u < COM_MAX_TX_SIGNAL_8BIT_INVALIDVALUE_NUMBER) && (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
            /*send signal use configuration invalidate value */
            uint16 invalidValueLength = txSignalDataInvalidValuePtr->TxSignalDataInvalidValueLength[signalInvalidId];
            ret                       = Com_SendDynSignalHandle(
                SignalId,
                &signal8BitInvalidValuePtr[invalidValueId],
                invalidValueLength,
                txSignalPtr->TxMainfunctionId);
#endif
        }
        else
#endif
        {
#if (0u < COM_MAX_SIGNAL_BOOL_INVALID_SIZE) || (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)      \
    || (0u < COM_MAX_SIGNAL_16BIT_INVALID_SIZE) || (0u < COM_MAX_SIGNAL_32BIT_INVALID_SIZE) \
    || (0u < COM_MAX_SIGNAL_64BIT_INVALID_SIZE)
            switch (signalType)
            {
/*send signal use configuration invalidate value */
#if (0u < COM_MAX_SIGNAL_BOOL_INVALID_SIZE)
            case COM_BOOLEAN:
                ret = Com_SendSignal(SignalId, &(signalBoolInvalidValuePtr[invalidValueId]));
                break;
#endif
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
            case COM_SINT8:
            case COM_UINT8:
            case COM_UINT8_N:
                ret = Com_SendSignal(SignalId, &(signal8BitInvalidValuePtr[invalidValueId]));
                break;
#endif
#if (0u < COM_MAX_SIGNAL_16BIT_INVALID_SIZE)
            case COM_SINT16:
            case COM_UINT16:
                ret = Com_SendSignal(SignalId, &(signalBit16InvalidValuePtr[invalidValueId]));
                break;
#endif
#if (0u < COM_MAX_SIGNAL_32BIT_INVALID_SIZE)
            case COM_SINT32:
            case COM_UINT32:
            case COM_FLOAT32:
                ret = Com_SendSignal(SignalId, &(signal32BitInvalidValuePtr[invalidValueId]));
                break;
#endif
#if (0u < COM_MAX_SIGNAL_64BIT_INVALID_SIZE)
            case COM_SINT64:
            case COM_UINT64:
            case COM_FLOAT64:
                ret = Com_SendSignal(SignalId, &(signal64BitInvalidValuePtr[invalidValueId]));
                break;
#endif
            default:
                /*do nothing*/
                break;
            }
#endif
        }
    }
#endif
    COM_NOUSED(SignalId);
    return ret;
}
#endif
/**
 * @brief Send Invalidate SignalGroup handle
 *
 */
#if ((COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_MAX_TXIPDU_NUMBER)
uint8 Com_InvalidateSignalGroupHandle(Com_SignalGroupIdType SignalGroupId)
{
    uint8                        ret              = COM_SERVICE_NOT_AVAILABLE;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[SignalGroupId];

    /*the signal group is InitialValueOnly,don't expect to be called by any API*/
#if (STD_ON == COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE)
    if (TRUE != txSignalGroupPtr->ComInitialValueOnly)
#endif
    {
        PduIdType              ipduRef               = txSignalGroupPtr->IpduRefIndex;
        Com_MainFunctionIdType txMainfunctionId      = txSignalGroupPtr->TxMainfunctionId;
        PduIdType              ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionTx(txMainfunctionId);
        /* PRQA S 3678 ++ */ /* VL_Com_NeverModify */
        Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
        /* PRQA S 3678 -- */
        /*the IPDU(TP) is sending,return busy*/
        /* It have judged the InvalidValueId is not null, so the ValuePtr may be not to judge*/
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        const Com_TxIPduType* txIPduPtr   = &Com_CfgTxIPduPtr[ipduRef];
        uint8                 Transmiting = txIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
        if ((COM_PDU_TP == txIPduPtr->ComIPduType) && (COM_TX_TRANSMITING_EN == Transmiting))
        {
            ret = COM_BUSY;
        }
        else
#endif
        {
#if (STD_ON == COM_TX_GRP_SIGNAL_INVALID_DATA_ENABLE)
            Com_TxGroupSignalIdType groupSignalEndId, groupSignalStartId;
            groupSignalEndId                                              = txSignalGroupPtr->GroupSignalRefEndId;
            groupSignalStartId                                            = txSignalGroupPtr->GroupSignalRefStartId;
            const Com_SignalInvalidValueType* txSignalDataInvalidValuePtr = Com_ConfigStd->SignalInvalidValuePtr;
#if (0u < COM_MAX_SIGNAL_BOOL_INVALID_SIZE)
            const boolean* signalBoolInvalidValuePtr = txSignalDataInvalidValuePtr->SignalBoolInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
            const uint8* signal8BitInvalidValuePtr = txSignalDataInvalidValuePtr->Signal8BitInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_16BIT_INVALID_SIZE)
            const uint16* signalBit16InvalidValuePtr = txSignalDataInvalidValuePtr->SignalBit16InvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_32BIT_INVALID_SIZE)
            const uint32* signal32BitInvalidValuePtr = txSignalDataInvalidValuePtr->Signal32BitInvalidValue;
#endif
#if (0u < COM_MAX_SIGNAL_64BIT_INVALID_SIZE)
            const uint64* signal64BitInvalidValuePtr = txSignalDataInvalidValuePtr->Signal64BitInvalidValue;
#endif
            for (; groupSignalStartId < groupSignalEndId; ++groupSignalStartId)
            {
                const Com_TxGroupSignalType* txGroupSignalPtr = &Com_CfgTxGroupSignalPtr[groupSignalStartId];
                Com_SignalType               signalType       = txGroupSignalPtr->ComSignalType;
                uint16                       invalidValueId   = txGroupSignalPtr->ComSignalInvalidBuffRefId;
                if (COM_UNUSED_UINT16 != invalidValueId)
                {
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
                    if (COM_UINT8_DYN == signalType)
                    {
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
                        uint16 invalidValueLength = txGroupSignalPtr->ComSignalDataInvalidValueLength;
                        /*send dynamic group signal use configuration invalidate value */
                        (void)Com_SendTxDynGroupSignalHandle(
                            txIpduStatePtr,
                            groupSignalStartId,
                            &(signal8BitInvalidValuePtr[invalidValueId]),
                            invalidValueLength);
#endif
                    }
                    else
#endif
                    {
#if (0u < COM_MAX_SIGNAL_BOOL_INVALID_SIZE) || (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)      \
    || (0u < COM_MAX_SIGNAL_16BIT_INVALID_SIZE) || (0u < COM_MAX_SIGNAL_32BIT_INVALID_SIZE) \
    || (0u < COM_MAX_SIGNAL_64BIT_INVALID_SIZE)
                        switch (signalType)
                        {
/*send group signal use configuration invalidate value */
#if (0u < COM_MAX_SIGNAL_BOOL_INVALID_SIZE)
                        case COM_BOOLEAN:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalStartId,
                                &(signalBoolInvalidValuePtr[invalidValueId]));
                            break;
#endif
#if (0u < COM_MAX_SIGNAL_8BIT_INVALID_SIZE)
                        case COM_SINT8:
                        case COM_UINT8:
                        case COM_UINT8_N:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalStartId,
                                &(signal8BitInvalidValuePtr[invalidValueId]));
                            break;
#endif
#if (0u < COM_MAX_SIGNAL_16BIT_INVALID_SIZE)
                        case COM_SINT16:
                        case COM_UINT16:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalStartId,
                                &(signalBit16InvalidValuePtr[invalidValueId]));
                            break;
#endif
#if (0u < COM_MAX_SIGNAL_32BIT_INVALID_SIZE)
                        case COM_SINT32:
                        case COM_UINT32:
                        case COM_FLOAT32:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalStartId,
                                &(signal32BitInvalidValuePtr[invalidValueId]));
                            break;
#endif
#if (0u < COM_MAX_SIGNAL_64BIT_INVALID_SIZE)
                        case COM_SINT64:
                        case COM_UINT64:
                        case COM_FLOAT64:
                            (void)Com_SendTxGroupSignalHandle(
                                groupSignalStartId,
                                &(signal64BitInvalidValuePtr[invalidValueId]));
                            break;
#endif
                        default:
                            /*do nothing*/
                            break;
                        }
#endif
                    }
                }
            }
#endif
            ret = Com_SendSignalGroup(SignalGroupId);
        }
        COM_NOUSED(txIpduStatePtr);
    }
    return ret;
}
#endif

/**
 * @brief Trigger IPDU send handle
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
Std_ReturnType Com_TriggerIPDUSendHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    PduIdType IpduId,
#endif
    Com_MainFunctionIdType TxMainfunctionId)
{
    Std_ReturnType returnValue = E_NOT_OK;
    /*In case a stopped I-PDU is triggered for transmission, just E_NOT_OK is returned.*/
    uint8 activeEnable = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN;
    if (COM_TX_ACTIVE_EN == activeEnable)
    {
        uint8 Transmiting = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
        /*the transmit is delay by MDT or the pdu is transmitting*/
        if (
#if (STD_ON == COM_MDT_ENABLE)
            (0u < TxIpduStatePtr->MDTCnt) ||
#endif
            (COM_TX_TRANSMITING_EN == Transmiting))
        {
            TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_DELAY_EN;
            returnValue = E_OK;
        }
        else
        {
            returnValue = Com_TriggerIPDUSendInternalHandle(
                TxIpduPtr,
                TxIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
                IpduId,
#endif
                TxMainfunctionId);
        }
    }
    else
    {
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
        Com_InvokeErrorNotification(TxIpduPtr);
#endif
    }
    return returnValue;
}
#endif

#if (STD_ON == COM_METADATA_SUPPORT) && (0u < COM_MAX_TXIPDU_NUMBER)
/******************************************************************************/
/*
 * Brief               Trigger IPDU(with metadata) send handle
 * ServiceId
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      PduId,MetaData
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              Std_ReturnType
 */
/******************************************************************************/
Std_ReturnType Com_TriggerIPDUSendWithMetaDataHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    PduIdType IpduId,
#endif
    Com_MainFunctionIdType TxMainfunctionId,
    const uint8*           MetaData)
{
    Std_ReturnType returnValue  = E_NOT_OK;
    uint8          activeEnable = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN;

    /*In case a stopped I-PDU is triggered for transmission, just E_NOT_OK is returned.*/
    if (COM_TX_ACTIVE_EN == activeEnable)
    {
        Com_TxIpduBufIdType ipduBufferId   = TxIpduPtr->TxIPduBufIndex;
        uint8               metaDataLength = TxIpduPtr->MetaDataLength;
        uint16              metaDataStartId =
            (uint16)TxIpduPtr->IPduNoDynSignalLength + (uint16)TxIpduPtr->IPduMaxDynSignalLength + (uint16)ipduBufferId;
        SchM_Enter_Com_Context();
        /*update the metadata of the tx ipdu*/
        uint8* txIpduRunrimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
        (void)IStdLib_MemCpy(&txIpduRunrimeBuffPtr[metaDataStartId], MetaData, metaDataLength);
        SchM_Exit_Com_Context();
        /*the transmit is delay by MDT or the pdu is transmitting*/
        uint8 Transmiting = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
        if (
#if (STD_ON == COM_MDT_ENABLE)
            (0u < TxIpduStatePtr->MDTCnt) ||
#endif
            (COM_TX_TRANSMITING_EN == Transmiting))
        {
            TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_DELAY_EN;
            returnValue = E_OK;
        }
        else
        {
            returnValue = Com_TriggerIPDUSendInternalHandle(
                TxIpduPtr,
                TxIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
                IpduId,
#endif
                TxMainfunctionId);
        }
    }
    else
    {
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
        Com_InvokeErrorNotification(TxIpduPtr);
#endif
    }
    return returnValue;
}
#endif
/**
 * @brief  Switch Ipdu Transmit Mode Handle
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER) && (STD_ON == COM_TMS_ENABLE)
void Com_SwitchIpduTxModeHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    boolean                     Mode)
{
    uint8 TMS = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_TMS_EN;
    if (Mode)
    {
        if (COM_TX_TMS_EN != TMS)
        {
            TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_TMS_EN;
            TxIpduStatePtr->IpduTxMode = TxIpduPtr->TxTrueModeMode;
            TxIpduStatePtr->NTimeCnt   = 0u;
            TxIpduStatePtr->RptNum     = 0u;
        }
    }
    else
    {
        if (COM_TX_TMS_EN == TMS)
        {
            TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TMS_DIS;
            TxIpduStatePtr->IpduTxMode = TxIpduPtr->TxFalseModeMode;
            TxIpduStatePtr->NTimeCnt   = 0u;
            TxIpduStatePtr->RptNum     = 0u;
        }
    }

    if ((TxIpduPtr->TxFalseModeMode != COM_TX_MODE_NULL)
        && ((COM_TX_MODE_MIXED == TxIpduStatePtr->IpduTxMode) || (COM_TX_MODE_PERIODIC == TxIpduStatePtr->IpduTxMode)
            || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == TxIpduStatePtr->IpduTxMode)))
    {
        if ((Mode && (COM_TX_TMS_EN != TMS)) || (!Mode && (COM_TX_TMS_EN == TMS)))
        {
            Com_GetTxModeOffset(TxIpduStatePtr, TxIpduPtr, Mode);
        }
    }
}
#endif
/**
 * @brief TxPdu MainFunction handle
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
/* PRQA S 6070 ++ */ /* VL_MTR_Com_STCAL */
void Com_MainFunctionTxHandle(Com_MainFunctionIdType TxMainfunctionId)
/* PRQA S 6070 -- */
{
    PduIdType txIpduStartId = Com_GetStartOfMainFunctionTx(TxMainfunctionId);

    for (PduIdType txIpduId = txIpduStartId; txIpduId < Com_GetEndOfMainFunctionTx(TxMainfunctionId); ++txIpduId)
    {
        PduIdType                   ipduIdPerMainfunction = txIpduId - txIpduStartId;
        Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[TxMainfunctionId][ipduIdPerMainfunction];
        const Com_TxIPduType*       txIpduPtr      = &Com_CfgTxIPduPtr[txIpduId];
        uint8                       activeEnable   = txIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN;
        if (COM_TX_ACTIVE_EN == activeEnable)
        {
            boolean txSendFlag = FALSE;
            /*Tx notification for all signals/signal groups of the Tx Pdu is DEFERRED in the MainFunctionTx*/
#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
            uint8 TxConfirm = txIpduStatePtr->TxIpduRTStFlag & COM_TX_CONFIRM_EN;
            if (COM_TX_CONFIRM_EN == TxConfirm)
            {
                /*Tx notification for all signals/signal groups of the Tx Pdu*/
                Com_ConfirmationProcess(txIpduPtr);
                txIpduStatePtr->TxIpduRTStFlag &= COM_TX_CONFIRM_DIS;
            }
#endif
            /* Calculate DM, if DM > 0u, DM-- every mainfunction */
            boolean ipduMDTRun = Com_MainTxDMTimeoutMdtCal(txIpduStatePtr, txIpduPtr);
            switch (txIpduStatePtr->IpduTxMode)
            {
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER)                               \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)
            case COM_TX_MODE_DIRECT:
            case COM_TX_MODE_DIRECT_WITHOUT_REPETITION:
                txSendFlag = Com_MainFuncTxDirect(txIpduStatePtr, txIpduPtr, ipduMDTRun);
                break;
#endif
#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
            case COM_TX_MODE_MIXED:
            case COM_TX_MODE_MIXED_WITHOUT_REPETITION:
                txSendFlag = Com_MainFuncTxMixed(txIpduStatePtr, txIpduPtr, ipduMDTRun);
                break;
#endif
#if (0u < COM_MAX_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER)  \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
            case COM_TX_MODE_PERIODIC:
                txSendFlag = Com_MainFuncTxPeriod(txIpduStatePtr, txIpduPtr, ipduMDTRun);
                break;
#endif
            default:
                /*do nothing*/
                break;
            }
            if (txSendFlag)
            {
#if (STD_ON == COM_TXTPPDU_SUPPORT)
                uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
                if ((COM_PDU_TP == txIpduPtr->ComIPduType) && (COM_TX_TRANSMITING_EN == Transmiting))
                {
                    /* in case a large I-PDU is currently transmitted and the same I-PDU is triggered for
                     * transmission again*/
                    (void)Det_ReportRuntimeError(
                        COM_MODULE_ID,
                        COM_INSTANCE_ID,
                        COM_MAINFUNCTIONTX_ID,
                        COM_E_SKIPPED_TRANSMISSION);
                }
                /*this MainFunctionTx need send the tx ipdu */
                else
#endif
                {
                    Com_MainFunction_SendPdu(
                        txIpduStatePtr,
                        txIpduPtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
                        txIpduId,
#endif
                        TxMainfunctionId);
                }
            }
        }
    }
}
#endif
/**
 * @brief TxPdu Trigger Transmit handle
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
Std_ReturnType Com_TriggerTransmitHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
    PduIdType                   TxIpduId,
    PduInfoType*                PduInfoPtr,
    Com_MainFunctionIdType      TxMainfunctionId)
{
    Std_ReturnType returnValue  = E_NOT_OK;
    uint8          activeEnable = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN;

    if (COM_TX_ACTIVE_EN == activeEnable)
    {
        uint16              txIPduLength = TxIpduStatePtr->TxIpduLength;
        Com_TxIpduBufIdType ipduBufferId = TxIpduPtr->TxIPduBufIndex;
        if (txIPduLength <= PduInfoPtr->SduLength)
        {
            uint8*      txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
            PduInfoType pduInfo;
            pduInfo.SduLength  = txIPduLength;
            pduInfo.SduDataPtr = &txIpduRuntimeBuffPtr[ipduBufferId];
#if (STD_ON == COM_METADATA_SUPPORT)
            if (TxIpduPtr->MetaDataLength > 0u)
            {
                uint16 metaDataStartId = (uint16)TxIpduPtr->IPduNoDynSignalLength
                                         + (uint16)TxIpduPtr->IPduMaxDynSignalLength + (uint16)ipduBufferId;
                pduInfo.MetaDataPtr = &txIpduRuntimeBuffPtr[metaDataStartId];
            }
#endif

            SchM_Enter_Com_Context();
#if (COM_TXIPDU_TIGGERTRANSMIT_CALLOUT_ENABLE > 0u)
            if (NULL_PTR != TxIpduPtr->ComIPduTriggerTransmitCallout)
            {
                (void)(TxIpduPtr->ComIPduTriggerTransmitCallout(TxIpduId, &pduInfo));
            }
#endif
            /* copy the pdu buffer data to the provided pointer */
            (void)IStdLib_MemCpy(PduInfoPtr->SduDataPtr, pduInfo.SduDataPtr, txIPduLength);
            /* copy the pdu length to the provided pointer */
            PduInfoPtr->SduLength = pduInfo.SduLength;
            /* copy the metadata to the provided pointer */
#if (STD_ON == COM_METADATA_SUPPORT)
            if (TxIpduPtr->MetaDataLength > 0u)
            {
                (void)IStdLib_MemCpy(PduInfoPtr->MetaDataPtr, pduInfo.MetaDataPtr, TxIpduPtr->MetaDataLength);
            }
#endif
            SchM_Exit_Com_Context();

#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
            if (COM_TRIGGER_TRANSMIT == TxIpduPtr->ComTxIPduClearUpdateBit)
            {
                Com_ClearTxIPduUpdates(TxIpduPtr, TxMainfunctionId);
            }
#endif
            TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_TRANSMITING_EN;
            returnValue = E_OK;
        }
    }
    else
    {
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
        Com_InvokeErrorNotification(TxIpduPtr);
#endif
    }
    COM_NOUSED(TxIpduId);

    return returnValue;
}
#endif

/**
 * @brief TP TxPdu TxConfirmation handle
 *
 */
#if ((0u < COM_MAX_TXIPDU_NUMBER) && (STD_ON == COM_TXTPPDU_SUPPORT))
void Com_TpTxConfirmationHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
    Com_MainFunctionIdType      TxMainfunctionId,
    Std_ReturnType              result)
{
    uint8 activeEnable = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN;
    TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TRANSMITING_DIS;
    if (COM_TX_ACTIVE_EN == activeEnable)
    {
        TxIpduStatePtr->TxOffset = 0u;
        /*TP PDU successful transmit*/
        if (E_OK == result)
        {
            /*repetitions of transmission */
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)   \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER)                                 \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)                                \
    || ((0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
        || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)                              \
        || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
            if (TxIpduStatePtr->RptNum > 0U)
            {
                (TxIpduStatePtr->RptNum) -= 1u;
                if (0u == TxIpduStatePtr->RptNum)
                {
                    TxIpduStatePtr->DMCnt    = 0u;
                    TxIpduStatePtr->NTimeCnt = 0u;
                }
            }
            /*no repetitions of transmission */
            else
#endif
            {
                TxIpduStatePtr->DMCnt = 0u;
            }
#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
            if (0u == TxIpduStatePtr->RptNum)
            {
#if (STD_ON == COM_TX_IPDU_SIGNAL_PROCESS_IMMEDIATE_ENABLE)
                /*signal/signal group TxConfirmation handle is made immediate*/
                if (COM_IMMEDIATE == TxIpduPtr->ComIPduSignalProcessing)
                {
                    /*Tx notification for all signals/signal groups of the Tx Pdu*/
                    Com_ConfirmationProcess(TxIpduPtr);
                    TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_CONFIRM_DIS;
                }
                else
#endif
                {
                    /*Tx notification for all signals/signal groups of the Tx Pdu in next MainFunctionTx*/
                    TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_CONFIRM_EN;
                }
            }
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
            if (COM_CONFIRMATION == TxIpduPtr->ComTxIPduClearUpdateBit)
            {
                /*clear all update bit of the Tx Pdu*/
                Com_ClearTxIPduUpdates(TxIpduPtr, TxMainfunctionId);
            }
#endif
        }
    }
    COM_NOUSED(TxMainfunctionId);
    COM_NOUSED(TxIpduPtr);

    return;
}
/**
 * @brief TP TxPdu copy pdu data handle
 *
 */
BufReq_ReturnType Com_CopyTxDataHandle(
    PduIdType              TxIpduId,
    Com_MainFunctionIdType TxMainfunctionId,
    const PduInfoType*     info,
    const RetryInfoType*   retry,
    PduLengthType*         availableDataPtr)
{
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK;
    uint16            copyStartPos;
    uint8             activeEnable, Transmiting;
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8  metaDataLength;
    uint16 metaDataStartId;
#endif
    const Com_TxIPduType*       txIpduPtr             = &Com_CfgTxIPduPtr[TxIpduId];
    PduIdType                   ipduIdPerMainfunction = TxIpduId - Com_GetStartOfMainFunctionTx(TxMainfunctionId);
    Com_TxIPduRunTimeStateType* txIpduStatePtr = &Com_TxIPduRunTimeState[TxMainfunctionId][ipduIdPerMainfunction];
    activeEnable                               = txIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN;
    Transmiting                                = txIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;

    if ((COM_TX_ACTIVE_EN == activeEnable) && (COM_TX_TRANSMITING_EN == Transmiting))
    {
        /*data retry offset*/
        if ((NULL_PTR != retry) && (TP_DATARETRY == retry->TpDataState))
        {
            txIpduStatePtr->TxOffset = (txIpduStatePtr->TxOffset >= retry->TxTpDataCnt)
                                           ? (txIpduStatePtr->TxOffset - (uint16)retry->TxTpDataCnt)
                                           : 0u;
            /*will not happen general,TP module guarantee*/
        }
        if (((uint16)txIpduStatePtr->TxOffset + (uint16)info->SduLength) <= txIpduStatePtr->TxIpduLength)
        {
            copyStartPos                      = (uint16)(txIpduPtr->TxIPduBufIndex + txIpduStatePtr->TxOffset);
            const uint8* txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
            SchM_Enter_Com_Context();
            /*copy the TX data from TX PDU buffer*/
            (void)IStdLib_MemCpy(info->SduDataPtr, &txIpduRuntimeBuffPtr[copyStartPos], info->SduLength);
            SchM_Exit_Com_Context();
/*copy the meta data from Tx Pdu Buffer*/
#if (STD_ON == COM_METADATA_SUPPORT)
            metaDataLength = txIpduPtr->MetaDataLength;
            if ((0u < metaDataLength) && (NULL_PTR != info->MetaDataPtr))
            {
                metaDataStartId = (uint16)txIpduPtr->IPduNoDynSignalLength + (uint16)txIpduPtr->IPduMaxDynSignalLength
                                  + txIpduPtr->TxIPduBufIndex;
                SchM_Enter_Com_Context();
                (void)IStdLib_MemCpy(info->MetaDataPtr, &txIpduRuntimeBuffPtr[metaDataStartId], metaDataLength);
                SchM_Exit_Com_Context();
            }
#endif
            txIpduStatePtr->TxOffset += (uint16)info->SduLength;
            *availableDataPtr = (PduLengthType)txIpduStatePtr->TxIpduLength - (PduLengthType)txIpduStatePtr->TxOffset;
            bufReq            = BUFREQ_OK;
        }
    }
    return bufReq;
}
#endif
/**
 * @brief Reset the counter of tx Ipdu MDT.
 *
 */
#if (STD_ON == COM_MDT_ENABLE)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
void Com_ResetTxIpduMDT(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr)
/* PRQA S 3673 -- */
{
#if (STD_ON == COM_ENABLE_MDT_FOR_CYCLIC_TRANSMISSION)
    TxIpduStatePtr->MDTCnt = TxIpduPtr->ComMinimumDelayTime;
#elif (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER)                                 \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)
    if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode))
    {
        if (TxIpduStatePtr->RptNum <= 1u)
        {
            TxIpduStatePtr->MDTCnt = TxIpduPtr->ComMinimumDelayTime;
        }
    }
#endif
    COM_NOUSED(TxIpduPtr);
    COM_NOUSED(TxIpduStatePtr);
}
#endif
/**
 * @brief Called by Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,
 *        Com_TriggerTransmit,Com_TxConfirmation,Com_TpTxConfirmation,
 *        Com_MainFunctionTx,Com_TxIpduControl.
 *        clear all signal group/signal/dest description signal update bit of the Tx Pdu
 *
 */
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
void Com_ClearTxIPduUpdates(const Com_TxIPduType* TxIpduPtr, Com_MainFunctionIdType TxMainfunctionId)
{
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_TxIpduBufIdType    ipduBufferIndex = TxIpduPtr->TxIPduBufIndex;
    Com_SignalPositionType updateBitPosition;
#endif

    uint8* txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    signalGroupNumber = TxIpduPtr->IPduSignalGroupsRefEndId;
    signalGroupId     = TxIpduPtr->IPduSignalGroupsRefStartId;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
        updateBitPosition                             = txSignalGroupPtr->UpdateLsbBytePos;
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            txIpduRuntimeBuffPtr[ipduBufferIndex + (Com_TxIpduBufIdType)updateBitPosition] &=
                (uint8)(~(txSignalGroupPtr->UpdateBitMask));
        }
    }
#endif
#if (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE)
    Com_TxSignalIdType txSignalNumber, cnt;
    txSignalNumber = TxIpduPtr->IPduSignalsRefEndId;
    cnt            = TxIpduPtr->IpduSignalRefStartId;
    for (; cnt < txSignalNumber; ++cnt)
    {
        const Com_TxSignalType* txSignalPtr = &Com_CfgTxSignalPtr[cnt];
        updateBitPosition                   = txSignalPtr->UpdateLsbBytePos;
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            txIpduRuntimeBuffPtr[ipduBufferIndex + (Com_TxIpduBufIdType)updateBitPosition] &=
                (uint8)(~(txSignalPtr->UpdateBitMask));
        }
    }
#endif
#if (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
    Com_GwDestinationDescriptionIdType destSignalNumber, destSignalId;
    destSignalNumber = TxIpduPtr->IPduDespSignalsRefNumber;
    destSignalId     = TxIpduPtr->IPduDespSignalsRefStartId;
    for (; destSignalId < destSignalNumber; ++destSignalId)
    {
        const Com_GwDestSignalType* gwDestSignalPtr = &Com_CfgGwMappingConfigPtr->DestSignal[destSignalId];
        updateBitPosition                           = gwDestSignalPtr->UpdateLsbBytePos;
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            txIpduRuntimeBuffPtr[ipduBufferIndex + (Com_TxIpduBufIdType)updateBitPosition] &=
                (uint8)(~(gwDestSignalPtr->UpdateBitMask));
        }
    }
#endif
#endif
    COM_NOUSED(TxIpduPtr);
    COM_NOUSED(TxMainfunctionId);
    return;
}
#endif
/* PRQA S 1532,2895 -- */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 1532,2895 ++ */ /* VL_Com_ReferencedOnlyOne,VL_Com_NegtiveValueType */
/**
 * @brief get TxModeTimeOffset of True/False TxMode
 *
 */
COM_LOCAL void
    Com_GetTxModeOffset(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean TMS)
{
    Com_TxModeTimeOffsetType txModeOffset = 1u;
    Com_TxModeModeType       ipduTxMode   = TxIpduStatePtr->IpduTxMode;
#if (STD_ON == COM_TXMODE_AND_FILTER_SUPPORT)
    const Com_TxModeAndSignalFilterPtrType* txModeAndSignalFilterPtr = Com_CfgTxModeAndSignalFilterPtr;
#endif
#if (STD_ON == COM_TMS_ENABLE)
    if (!TMS)
    {
        if (COM_TX_MODE_MIXED == ipduTxMode)
        {
#if (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)
            txModeOffset =
                txModeAndSignalFilterPtr->TxModeFalseMixedRepetitions[TxIpduPtr->TxModeFalseRefId].TxModeTimeOffset;
#endif
        }
        else
        {
#if (0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
            txModeOffset = txModeAndSignalFilterPtr->TxModeFalsePeriod[TxIpduPtr->TxModeFalseRefId].TxModeTimeOffset;
#endif
        }
    }
    else
#endif
    {
        if (COM_TX_MODE_MIXED == ipduTxMode)
        {
#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)
            txModeOffset =
                txModeAndSignalFilterPtr->TxModeTrueMixedRepetitions[TxIpduPtr->TxModeTrueRefId].TxModeTimeOffset;
#endif
        }
        else
        {
#if (0u < COM_MAX_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)
            txModeOffset = txModeAndSignalFilterPtr->TxModeTruePeriod[TxIpduPtr->TxModeTrueRefId].TxModeTimeOffset;
#endif
        }
    }
    TxIpduStatePtr->PeriodCnt = txModeOffset;
    COM_NOUSED(TMS);
}

/**
 * @brief reset OCCURRENCE of filter with FilterAlgorithm ONE_EVERY_N.
 *
 */
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
COM_LOCAL void Com_ResetTxOccurrenceOfPduFilter(const Com_TxIPduType* TxIpduPtr)
{
/*reset signal/group signal/dest description signal of tx pdu which FilterAlgorithm is ONE_EVERY_N*/
#if (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_TxSignalIdType txSignalEndId, txSignalStartId;
    txSignalEndId   = TxIpduPtr->IPduSignalsRefEndId;
    txSignalStartId = TxIpduPtr->IpduSignalRefStartId;
    for (; txSignalStartId < txSignalEndId; ++txSignalStartId)
    {
        const Com_TxSignalType* txSignalPtr = &Com_CfgTxSignalPtr[txSignalStartId];
        if ((txSignalPtr->FilterAlgorithm == COM_ONE_EVERY_N)
            && (COM_UNUSED_TXSIGNALFILTERID != txSignalPtr->FilterIndex))
        {
            Com_OneEveryNcnt[txSignalPtr->FilterIndex] = 0u;
        }
    }
#endif
#if (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_GwDestinationDescriptionIdType ipduDestSignalNumber, counter;
    ipduDestSignalNumber = TxIpduPtr->IPduDespSignalsRefNumber;
    counter              = TxIpduPtr->IPduDespSignalsRefStartId;
    for (; counter < ipduDestSignalNumber; ++counter)
    {
        const Com_GwDestSignalType* ComDestSignalPtr = &Com_CfgGwMappingConfigPtr->DestSignal[counter];
        if ((ComDestSignalPtr->FilterAlgorithm == COM_ONE_EVERY_N)
            && (COM_UNUSED_DESTSIGNALFILTERID != ComDestSignalPtr->FilterIndex))
        {
            Com_OneEveryNcnt[ComDestSignalPtr->FilterIndex] = 0u;
        }
    }
#endif
#if (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    Com_TxGroupSignalIdType groupSignalNumber, index;
    signalGroupNumber = TxIpduPtr->IPduSignalGroupsRefEndId;
    signalGroupId     = TxIpduPtr->IPduSignalGroupsRefStartId;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
        groupSignalNumber                             = txSignalGroupPtr->GroupSignalRefEndId;
        index                                         = txSignalGroupPtr->GroupSignalRefStartId;
        for (; index < groupSignalNumber; ++index)
        {
            const Com_TxGroupSignalType* txGroupSignalPtr = &Com_CfgTxGroupSignalPtr[index];
            if ((txGroupSignalPtr->FilterAlgorithm == COM_ONE_EVERY_N)
                && (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->FilterIndex))
            {
                Com_OneEveryNcnt[txGroupSignalPtr->FilterIndex] = 0u;
            }
        }
    }
#endif
    return;
}
#endif

/**
 * @brief called by Com_TxIpduControl.init the tx ipdu buffer,all signal buffer (included in the ipdu) and init the
 * TMC.
 *
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_TXIPDUGROUP_NUMBER > 0u)
COM_LOCAL void Com_ResetTxPduBufferAndSignalBuffer(
    PduIdType                         IpduId,
    Com_MainFunctionIdType            TxMainfunctionId,
    Com_TxIPduRunTimeStateType*       TxIpduStatePtr,
    const Com_TxIPduRunTimeStateType* TxIpduInitStatePtr)
{
    const Com_TxIPduType* txIpduPtr            = &Com_CfgTxIPduPtr[IpduId];
    const uint8*          txIpduInitValuePtr   = Com_ConfigStd->TxIpduInitValuePtr[TxMainfunctionId];
    uint8*                txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
    uint16                signalInitValueId;
    uint16                signalLength = 0u;
    Com_SignalType        signalType;
    Com_SignalIdType      txSignalTMCId;

/*init the tx ipdu buffer,all signal buffer(included in the ipdu) and init the TMC*/
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
    Com_TxIpduBufIdType pduBufferId   = txIpduPtr->TxIPduBufIndex;
    uint16              initPduLength = TxIpduInitStatePtr->TxIpduLength;
    SchM_Enter_Com_Context();
    (void)IStdLib_MemCpy(&txIpduRuntimeBuffPtr[pduBufferId], &txIpduInitValuePtr[pduBufferId], initPduLength);
    SchM_Exit_Com_Context();
#endif
#if (0u < COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER) || (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u) \
    || (0u < COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER)
    boolean*       txSignalTMCRuntimePtr = Com_TxSignalTMCRunTime[TxMainfunctionId];
    const boolean* txSignalInitTMCPtr    = Com_CfgTxModeAndSignalFilterPtr->TxSignalTMCInitVale[TxMainfunctionId];
#endif

#if (COM_MAX_TXSIGNAL_NUMBER > 0u)
    Com_TxSignalIdType txSignalEndId, txSignalStartId;
    /*init signal buffer and TMC*/
    txSignalEndId   = txIpduPtr->IPduSignalsRefEndId;
    txSignalStartId = txIpduPtr->IpduSignalRefStartId;

    for (; txSignalStartId < txSignalEndId; ++txSignalStartId)
    {
        const Com_TxSignalType* txSignalPtr = &Com_CfgTxSignalPtr[txSignalStartId];
        signalInitValueId                   = txSignalPtr->SignalInitValueId;
        signalType                          = txSignalPtr->ComSignalType;
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE)
        signalLength = (COM_UINT8_DYN == signalType) ? (txSignalPtr->SignalDataInitValueLength) : signalLength;
        signalLength = (COM_UINT8_N == signalType) ? (txSignalPtr->ComSignalLength) : signalLength;
#endif
        Com_InitSignalBuffer(signalType, signalInitValueId, signalLength, txIpduPtr->IpduPartitionId);
#if (0u < COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER)
        if (COM_UNUSED_TXSIGNALFILTERID != txSignalPtr->FilterIndex)
        {
            txSignalTMCId                        = txSignalPtr->TMCBufferId;
            txSignalTMCRuntimePtr[txSignalTMCId] = txSignalInitTMCPtr[txSignalTMCId];
        }
#endif
    }
#endif
#if (COM_MAX_GW_DESTINATION_DESCRIPTION_NUMBER > 0u)
    Com_GwDestinationDescriptionIdType destSignalNumber, destSignalId;
    /*init dest description signal buffer and TMC*/
    destSignalNumber = txIpduPtr->IPduDespSignalsRefNumber;
    destSignalId     = txIpduPtr->IPduDespSignalsRefStartId;
    for (; destSignalId < destSignalNumber; ++destSignalId)
    {
        const Com_GwDestSignalType* gwDestSignalPtr = &Com_CfgGwMappingConfigPtr->DestSignal[destSignalId];
        signalInitValueId                           = gwDestSignalPtr->SignalInitValueId;
        signalType                                  = gwDestSignalPtr->ComSignalType;
        signalLength = (COM_UINT8_DYN == signalType) ? (gwDestSignalPtr->SignalDataInitValueLength) : signalLength;
        signalLength = (COM_UINT8_N == signalType) ? (gwDestSignalPtr->ComSignalLength) : signalLength;
        Com_InitSignalBuffer(signalType, signalInitValueId, signalLength, txIpduPtr->IpduPartitionId);
#if (COM_MAX_DESTSIG_FILTERTYPE_MAX_NUMBER > 0u)
        if (COM_UNUSED_DESTSIGNALFILTERID != gwDestSignalPtr->FilterIndex)
        {
            txSignalTMCId                        = gwDestSignalPtr->TMCBufferId;
            txSignalTMCRuntimePtr[txSignalTMCId] = txSignalInitTMCPtr[txSignalTMCId];
        }
#endif
    }
#endif
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    Com_TxGroupSignalIdType groupSignalNumber, counter;
    /*init group signal buffer and TMC*/
    signalGroupNumber = txIpduPtr->IPduSignalGroupsRefEndId;
    signalGroupId     = txIpduPtr->IPduSignalGroupsRefStartId;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
        groupSignalNumber                             = txSignalGroupPtr->GroupSignalRefEndId;
        counter                                       = txSignalGroupPtr->GroupSignalRefStartId;
        for (; counter < groupSignalNumber; ++counter)
        {
            const Com_TxGroupSignalType* txGroupSignalPtr = &Com_CfgTxGroupSignalPtr[counter];
            signalInitValueId                             = txGroupSignalPtr->SignalInitValueId;
            signalType                                    = txGroupSignalPtr->ComSignalType;
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
            signalLength = (COM_UINT8_DYN == signalType) ? (txGroupSignalPtr->SignalDataInitValueLength) : signalLength;
            signalLength = (COM_UINT8_N == signalType) ? (txGroupSignalPtr->ComSignalLength) : signalLength;
#endif
            Com_InitSignalBuffer(signalType, signalInitValueId, signalLength, txIpduPtr->IpduPartitionId);
#if ((0u < COM_MAX_TMCTXSIGNAL_NUMBER) && (0u < COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER))
            if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->FilterIndex)
            {
                boolean* TxGroupSignalTMCBuffPtr       = Com_TxGroupSignalTMCBuffer[TxMainfunctionId];
                txSignalTMCId                          = txGroupSignalPtr->TMCBufferId;
                TxGroupSignalTMCBuffPtr[txSignalTMCId] = txSignalInitTMCPtr[txSignalTMCId];
                txSignalTMCRuntimePtr[txSignalTMCId]   = txSignalInitTMCPtr[txSignalTMCId];
            }
#endif
        }
    }
#endif
    TxIpduStatePtr->TxIpduRTStFlag |= (TxIpduInitStatePtr->TxIpduRTStFlag & COM_TX_TMS_EN);
    TxIpduStatePtr->IpduTxMode   = TxIpduInitStatePtr->IpduTxMode;
    TxIpduStatePtr->TxIpduLength = TxIpduInitStatePtr->TxIpduLength;
    TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_CONFIRM_DIS;
    TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TRANSMITING_DIS;
    TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_DELAY_DIS;
    TxIpduStatePtr->NTimeCnt = 0u;
    TxIpduStatePtr->RptNum   = 0u;
    return;
}
#endif

/**
 * @brief pack tx group signal(not dynamic).
 *
 */
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
COM_LOCAL void Com_TxGroupSignalPack(
    const Com_TxGroupSignalType* TxGroupSignalPtr,
    const Com_TxIPduType*        TxIpduPtr,
    Com_MainFunctionIdType       TxMainfunctionId)
{
    Com_SignalType      signalType      = TxGroupSignalPtr->ComSignalType;
    Com_TxIpduBufIdType ipduBufferIndex = TxIpduPtr->TxIPduBufIndex;
    uint16              signalBufferId  = TxGroupSignalPtr->SignalInitValueId;
    uint64              txData;
    Com_TxIpduBufIdType ipduBuffPos   = ipduBufferIndex + (Com_TxIpduBufIdType)(TxGroupSignalPtr->SigLsbBytePos);
    uint8*              iPduBufferPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
    uint16              partitionId   = TxIpduPtr->IpduPartitionId;

#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
    if (COM_UINT8_N == signalType)
    {
        /*pack COM_UINT8_N type group signal*/
        uint16 signalLength = TxGroupSignalPtr->ComSignalLength;
        SchM_Enter_Com_Context();
        (void)IStdLib_MemCpy(
            &iPduBufferPtr[ipduBuffPos],
            &Com_Signal8BitRuntimeBuff[partitionId][signalBufferId],
            signalLength);
        SchM_Exit_Com_Context();
    }
    else
#endif
#if ((0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_MAX_TXIPDUBUFF_SIZE))
        if (COM_BOOLEAN == signalType)
    {
        uint8 value = (uint8)(1u << TxGroupSignalPtr->SignalPosInByte);
        SchM_Enter_Com_Context();
        if (Com_SignalBoolRuntimeBuff[partitionId][signalBufferId])
        {
            iPduBufferPtr[ipduBuffPos] |= value;
        }
        else
        {
            iPduBufferPtr[ipduBuffPos] &= (uint8)(~value);
        }
        SchM_Exit_Com_Context();
    }
    else
#endif
    {
        /*pack group signal expect signal type(boolean,COM_UINT8_N,COM_UINT8_DYN)*/
        switch (signalType)
        {
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
        case COM_UINT8:
        case COM_SINT8:
            txData = (uint64)(Com_Signal8BitRuntimeBuff[partitionId][signalBufferId]);
            break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
        case COM_UINT16:
        case COM_SINT16:
            txData = (uint64)(Com_SignalBit16RuntimeBuff[partitionId][signalBufferId]);
            break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
        case COM_SINT32:
        case COM_UINT32:
        case COM_FLOAT32:
            txData = (uint64)(Com_Signal32BitRuntimeBuff[partitionId][signalBufferId]);
            break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
        case COM_SINT64:
        case COM_UINT64:
        case COM_FLOAT64:
            txData = Com_Signal64BitRuntimeBuff[partitionId][signalBufferId];
            break;
#endif
        default:
            /*reach here,the signal type is error*/
            txData = 0u;
            break;
        }
        Com_TxSignalPackHandle(COM_TX_GROUP_SIGNAL, TxGroupSignalPtr, txData, &iPduBufferPtr[ipduBufferIndex]);
    }
}
#endif

/**
 * @brief called by Com_TxSignalTMHandle,Com_SendTxGroupSignalHandle,
 *        Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal.
 *        the group signal value changed,judge it will trigger calculate the new TMC of the signal new value.
 *
 */
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
COM_LOCAL boolean Com_TxSignalTMCCalculate(
    Com_SignalConfigType DestSignalType,
    const void*          SignalValuePtr,
    const void*          TxSignalTMCPtr,
    uint64               SignalOldValue)
{
    boolean ret = FALSE;
#if (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u) || (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    const Com_TxSignalType* txSignalPtr;
#if (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    const Com_TxGroupSignalType* txGroupSignalPtr;
#endif
    Com_SignalType          signalType;
    Com_FilterAlgorithmType filterType;
    Com_SignalIdType        filterIndex;
    uint8                   bitsize;
    boolean                 isTrue = FALSE;

    switch (DestSignalType)
    {
#if (COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    case COM_TX_SIGNAL:
        txSignalPtr = (const Com_TxSignalType*)TxSignalTMCPtr;
        signalType  = txSignalPtr->ComSignalType;
        filterType  = txSignalPtr->FilterAlgorithm;
        filterIndex = (Com_SignalIdType)(txSignalPtr->FilterIndex);
        bitsize     = txSignalPtr->ComBitSize;
        isTrue      = TRUE;
        break;
#endif
#if (COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
    case COM_TX_GROUP_SIGNAL:
        txGroupSignalPtr = (const Com_TxGroupSignalType*)TxSignalTMCPtr;
        signalType       = txGroupSignalPtr->ComSignalType;
        filterType       = txGroupSignalPtr->FilterAlgorithm;
        filterIndex      = (Com_SignalIdType)(txGroupSignalPtr->FilterIndex);
        bitsize          = txGroupSignalPtr->ComBitSize;
        isTrue           = TRUE;
        break;
#endif
    default:
        /* do nothing */
        break;
    }

    /*COM_UINT8_N just support ALWAYS and NEVER FilterAlgorithm*/
    if (isTrue)
    {
#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == signalType)
        {
            if (COM_ALWAYS == filterType)
            {
                ret = TRUE;
            }
        }
        else
#endif
        {
            /*tx signal changed to uint64 type*/
            uint64 signalNewValue = Com_GetTxSignalValue(signalType, SignalValuePtr, bitsize);
            ret                   = Com_DestSignalTMCCalculate(
                signalType,
                filterType,
                filterIndex,
#if (COM_MAX_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
                SignalOldValue,
#endif
                signalNewValue);
        }
    }
#endif
    COM_NOUSED(DestSignalType);
    COM_NOUSED(SignalValuePtr);
    COM_NOUSED(TxSignalTMCPtr);
    COM_NOUSED(SignalOldValue);
    return ret;
}
#endif

/**
 * @brief change the signal value to uint64 type.
 *
 */
#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
COM_LOCAL uint64 Com_GetTxSignalValue(Com_SignalType SignalType, const void* SignalValuePtr, uint8 ComBitSize)
{
    uint64 signalNewValue = 0u;
    /* PRQA S 4391++ */ /* VL_Com_DataConverteToWider */
    switch (SignalType)
    {
    case COM_BOOLEAN:
        /* PRQA S 4304 ++ */ /* VL_Com_BooleanType */
        signalNewValue = (uint64)(*((const boolean*)SignalValuePtr));
        /* PRQA S 4304 -- */
        break;
    case COM_UINT8:
    case COM_SINT8:
        signalNewValue = (uint64)((*((const uint8*)SignalValuePtr)) & (COM_UNUSED_UINT8 >> (8u - ComBitSize)));
        break;
    case COM_SINT16:
    case COM_UINT16:
        signalNewValue = (uint64)((*((const uint16*)SignalValuePtr)) & (COM_UNUSED_UINT16 >> (16u - ComBitSize)));
        break;
    case COM_SINT32:
    case COM_UINT32:
        signalNewValue = (uint64)((*((const uint32*)SignalValuePtr)) & (COM_UNUSED_UINT32 >> (32u - ComBitSize)));
        break;
    case COM_SINT64:
    case COM_UINT64:
        signalNewValue = (*((const uint64*)SignalValuePtr));
        break;
    default:
        /*do nothing*/
        break;
    }
    /* PRQA S 4391 -- */
    return signalNewValue;
}
#endif

/**
 * @brief update the uint8_n signal value.
 *
 */
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0u))
#if (                                                                                   \
    (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) && (0u < COM_MAX_TXIPDUBUFF_SIZE) \
    && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE))
COM_LOCAL boolean Com_TxGroupSignalUpdateUint8N(
    Com_TxIpduBufIdType    TxIPduBufferId,
    Com_SignalIdType       GroupSignalId,
    uint16                 GroupSignalInitValueId,
    uint16                 PartitionId,
    Com_MainFunctionIdType TxMainfunctionId)
{
    const Com_TxGroupSignalType* txGroupSignalPtr  = &Com_CfgTxGroupSignalPtr[GroupSignalId];
    boolean                      triggerOnChange   = FALSE;
    uint16                       groupSignalLength = txGroupSignalPtr->ComSignalLength;
    Com_TxIpduBufIdType sigBufPosBase = TxIPduBufferId + (Com_TxIpduBufIdType)(txGroupSignalPtr->SigLsbBytePos);
    uint8*              signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
    const uint8*        txIpduRuntimeBuffPtr     = Com_TxIPduRuntimeBuff[TxMainfunctionId];

    /*if the group signal changed,then update the signal buffer*/
    if (0
        != IStdLib_MemCmp(
            &signal8BitRuntimeBuffPtr[GroupSignalInitValueId],
            &txIpduRuntimeBuffPtr[sigBufPosBase],
            groupSignalLength))
    {
        SchM_Enter_Com_Context();
        (void)IStdLib_MemCpy(
            &signal8BitRuntimeBuffPtr[GroupSignalInitValueId],
            &txIpduRuntimeBuffPtr[sigBufPosBase],
            groupSignalLength);
        SchM_Exit_Com_Context();
        triggerOnChange = TRUE;
    }
    return triggerOnChange;
}
#endif
#endif

/**
 * @brief Send Signal Group(Array) handle
 *
 */
#if ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API))
COM_LOCAL void Com_SendSignalGroupArrayInternalHandle(
    const Com_TxSignalGroupType* TxSignalGroupPtr,
    Com_TxIPduRunTimeStateType*  TxIpduStatePtr,
    const Com_TxIPduType*        TxIpduPtr,
    boolean                      TriggerOnChange,
    uint16                       RptNum)
{
    Com_TxModeModeType ipduTxMode = TxIpduStatePtr->IpduTxMode;
    boolean            initDMCnt  = FALSE;
/*the signal group configuration timeout,the DM timer isn't start by other signals/signal groups,Tx Mode isn't
 * NONE*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
    if ((COM_UNUSED_TXSIGGRPTIMEOUTID != TxSignalGroupPtr->TimeoutIndex) && (COM_TX_MODE_NONE != ipduTxMode)
        && (0u == TxIpduStatePtr->DMCnt))
    {
        /*restart the tx DM timer*/
        TxIpduStatePtr->DMCnt = TxIpduPtr->TxIpduDM;
        initDMCnt             = TRUE;
    }
#endif
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)                              \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)                         \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)                                                         \
    || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode)
        || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode)
        || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
    {
        Com_TransferPropertyType signalTxProperty = TxSignalGroupPtr->ComTransferProperty;
        /* PRQA S 2109 ++  */ /* VL_Com_BoolPromoted */
        boolean onChangeIsTrue =
            (signalTxProperty == COM_TRIGGERED_ON_CHANGE)
                ? TriggerOnChange
                : ((signalTxProperty == COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION) ? TriggerOnChange : TRUE);
/* PRQA S 2109 --  */ /* VL_Com_BoolPromoted */
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
        if (((signalTxProperty == COM_TRIGGERED_ON_CHANGE) || (signalTxProperty == COM_TRIGGERED)) && onChangeIsTrue)
        {
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum   = RptNum;
            /*For an I-PDU with ComTxModeMode DIRECT and TxModeNumberOfRepetitions > 0, the AUTOSAR COM module
              shall reset an already running transmission deadline monitoring timer in case another send request for
              this I-PDU is initiated*/
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
            if ((1u < RptNum) && (!initDMCnt) && (COM_UNUSED_TXSIGGRPTIMEOUTID != TxSignalGroupPtr->TimeoutIndex))
            {
                TxIpduStatePtr->DMCnt = TxIpduPtr->TxIpduDM;
            }
#endif
        }
#endif
#if (STD_ON == COM_TX_SIG_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE) || (STD_ON == COM_TX_SIG_PROP_TRIG_NO_REPETITION_ENABLE)
        if (((signalTxProperty == COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION)
             || (signalTxProperty == COM_TRIGGERED_WITHOUT_REPETITION))
            && onChangeIsTrue)
        {
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum   = 1u;
        }
#endif
    }
    else
#endif
    {
        TxIpduStatePtr->NTimeCnt = 0u;
        TxIpduStatePtr->RptNum   = 0u;
    }
    COM_NOUSED(RptNum);
    COM_NOUSED(TriggerOnChange);
    COM_NOUSED(initDMCnt);
    COM_NOUSED(TxIpduPtr);
    return;
}
#endif

/**
 * @brief Trigger IPdu Send handle
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
COM_LOCAL Std_ReturnType Com_TriggerIPDUSendInternalHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    PduIdType IpduId,
#endif
    Com_MainFunctionIdType TxMainfunctionId)
{
    uint8*              txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
    Com_TxIpduBufIdType ipduBufferId         = TxIpduPtr->TxIPduBufIndex;
    uint16              ipduLength           = TxIpduStatePtr->TxIpduLength;
    PduInfoType         pduInfo;
    Std_ReturnType      returnValue = E_NOT_OK;

    pduInfo.SduDataPtr = &txIpduRuntimeBuffPtr[ipduBufferId];
    pduInfo.SduLength  = (PduLengthType)ipduLength;
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8 metaDataLength = TxIpduPtr->MetaDataLength;
    if (0u < metaDataLength)
    {
        PduLengthType metaDataStartId =
            TxIpduPtr->IPduNoDynSignalLength + TxIpduPtr->IPduMaxDynSignalLength + (PduLengthType)ipduBufferId;
        pduInfo.MetaDataPtr = &txIpduRuntimeBuffPtr[metaDataStartId];
    }
    else
#endif
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    boolean ret = TRUE;
    /*invoke the callout API*/
    if (NULL_PTR != TxIpduPtr->ComIPduCallout)
    {
        ret = TxIpduPtr->ComIPduCallout(IpduId, &pduInfo);
    }
    if (ret)
#endif
    {
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        if (COM_PDU_TP == TxIpduPtr->ComIPduType)
        {
            pduInfo.SduDataPtr = NULL_PTR;
        }
#endif
        TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_TRANSMITING_EN;
        TxIpduStatePtr->TxOffset = 0u;
        if (E_OK == PduR_ComTransmit(TxIpduPtr->ComPduIdRef, &pduInfo))
        {
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
            Com_ResetTxIpduMDT(TxIpduStatePtr, TxIpduPtr);
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
            if (COM_TRANSMIT == TxIpduPtr->ComTxIPduClearUpdateBit)
            {
                /*clear all signal group/signal/dest description signal update bit of the Tx Pdu*/
                Com_ClearTxIPduUpdates(TxIpduPtr, TxMainfunctionId);
            }
#endif
            returnValue = E_OK;
        }
        else
        {
            TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TRANSMITING_DIS;
/*ComRetryFailedTransmitRequests*/
#if (STD_ON == COM_RETRY_FAILED_TRANSMIT_REQUESTS)
            TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_DELAY_EN;
            returnValue = E_OK;
#else
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
            Com_ResetTxIpduMDT(TxIpduStatePtr, TxIpduPtr);
#endif
#endif
        }
    }
    return returnValue;
}
#endif
/**
 * @brief Called by Com_MainFunctionTx.
 *        tx pdu DM timeout notification.
 *
 */
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
COM_LOCAL void Com_TxDMTimeOutNotification(const Com_TxIPduType* TxIpduPtr)
{
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
    Com_TxSignalIdType txSignalNumber, cnt;
    txSignalNumber = TxIpduPtr->IPduSignalsRefEndId;
    cnt            = TxIpduPtr->IpduSignalRefStartId;
    for (; cnt < txSignalNumber; ++cnt)
    {
        const Com_TxSignalType*              txSignalPtr       = &Com_CfgTxSignalPtr[cnt];
        Com_TxSignalTimeoutIdType            txSigTimeoutIndex = txSignalPtr->TimeoutIndex;
        const Com_TxTimeoutNotificationType* txSignalDataTimeoutNotificationCfg =
            Com_CfgSignalTimeoutPtr->TxSignalDataTimeoutNotificationCfg;
        if ((txSigTimeoutIndex != COM_UNUSED_TXSIGNALTIMEOUTID)
            && (NULL_PTR != txSignalDataTimeoutNotificationCfg[txSigTimeoutIndex]))
        {
            txSignalDataTimeoutNotificationCfg[txSigTimeoutIndex]();
        }
    }
#endif
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    signalGroupNumber = TxIpduPtr->IPduSignalGroupsRefEndId;
    signalGroupId     = TxIpduPtr->IPduSignalGroupsRefStartId;
    Com_TxSigGrpTimeoutIdType timeoutIndex;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
        timeoutIndex                                  = txSignalGroupPtr->TimeoutIndex;
        const Com_TxTimeoutNotificationType* txSigGrpDataTimeoutNotificationCfg =
            Com_CfgSignalTimeoutPtr->TxSigGrpTimeoutNotificationCfg;
        if ((timeoutIndex != COM_UNUSED_TXSIGGRPTIMEOUTID)
            && (NULL_PTR != txSigGrpDataTimeoutNotificationCfg[timeoutIndex]))
        {
            txSigGrpDataTimeoutNotificationCfg[timeoutIndex]();
        }
    }
#endif
    return;
}
#endif
/**
 * @brief Calculate DM and MDT
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
COM_LOCAL boolean Com_MainTxDMTimeoutMdtCal(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr)
/* PRQA S 3673 -- */
{
    boolean ipduMDTRun = FALSE;
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)

    if (TxIpduStatePtr->DMCnt > 0u)
    {
        (TxIpduStatePtr->DMCnt) -= 1u;
        /* DM_TimeOut */
        if (0u == TxIpduStatePtr->DMCnt)
        {
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
            /* ToutNotification*/
            Com_TxDMTimeOutNotification(TxIpduPtr);
#endif
            /* CancelTx(Direct/N component */
            TxIpduStatePtr->RptNum   = 0u;
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TRANSMITING_DIS;
/*cancel the transmitting tx pdu*/
#if (STD_ON == COM_CANCELLATION_SUPPORT)
            if (TxIpduPtr->ComIPduCancellationSupport)
            {
                (void)PduR_ComCancelTransmit(TxIpduPtr->ComPduIdRef);
            }
#endif
        }
    }
#endif
#if (STD_ON == COM_MDT_ENABLE)
    /* Calculate MDT, if MDT > 0u, MDT-- every mainfunction  */
    if (TxIpduStatePtr->MDTCnt > 0u)
    {
        (TxIpduStatePtr->MDTCnt) -= 1u;
        /* MDT Timer > 0u, set MDT running flag */
        if (TxIpduStatePtr->MDTCnt != 0u)
        {
            ipduMDTRun = TRUE;
        }
    }
#endif
    COM_NOUSED(TxIpduStatePtr);
    COM_NOUSED(TxIpduPtr);
    return ipduMDTRun;
}
#endif
/**
 * @brief Called by Com_MainFunctionTx.
 *        the handle of Com_MainFunctionTx,when the tx mode is direct.
 *        return the boolean value for the tx pdu need to transmit or not.
 *
 */
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)   \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER)                                 \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)                                \
    || ((0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
        || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)                              \
        || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
COM_LOCAL boolean
    Com_MainFuncTxDirect(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun)
{
    boolean            ret = FALSE;
    Com_TxModeModeType txMode;
    uint8              TxDelay = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_DELAY_EN;
#if (STD_ON == COM_TXMODE_AND_FILTER_SUPPORT)
    const Com_TxModeAndSignalFilterPtrType* txFilterPtr = Com_CfgTxModeAndSignalFilterPtr;
#endif

#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)
    if ((!MDTRun) && (COM_TX_DELAY_EN == TxDelay))
    {
        TxIpduStatePtr->NTimeCnt = 0u;
    }
    TxIpduStatePtr->NTimeCnt =
        (TxIpduStatePtr->NTimeCnt > 0u) ? (TxIpduStatePtr->NTimeCnt - 1u) : TxIpduStatePtr->NTimeCnt;
#endif
    if ((0u == TxIpduStatePtr->NTimeCnt) && (TxIpduStatePtr->RptNum > 0u))
    {
/*delay transmit of the MDT */
#if (STD_ON == COM_MDT_ENABLE)
        TxIpduStatePtr->TxIpduRTStFlag =
            MDTRun ? (TxIpduStatePtr->TxIpduRTStFlag | COM_TX_DELAY_EN) : TxIpduStatePtr->TxIpduRTStFlag;
#endif
        ret = (!MDTRun) ? TRUE : FALSE;
#if (STD_ON == COM_TMS_ENABLE)
        uint8 TMS = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_TMS_EN;
        if (TMS != COM_TX_TMS_EN)
        {
            txMode = TxIpduPtr->TxFalseModeMode;
#if (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)
            TxIpduStatePtr->NTimeCnt =
                (txMode == COM_TX_MODE_DIRECT)
                    ? txFilterPtr->TxModeFalseDirectRepetitions[TxIpduPtr->TxModeFalseRefId].TxModeRepetitionPeriod
                    : TxIpduStatePtr->NTimeCnt;
#endif
#if (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)
            TxIpduStatePtr->NTimeCnt =
                (txMode == COM_TX_MODE_MIXED)
                    ? txFilterPtr->TxModeFalseMixedRepetitions[TxIpduPtr->TxModeFalseRefId].TxModeRepetitionPeriod
                    : TxIpduStatePtr->NTimeCnt;
#endif
        }
        else
#endif
        {
            txMode = TxIpduPtr->TxTrueModeMode;
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER)
            TxIpduStatePtr->NTimeCnt =
                (txMode == COM_TX_MODE_DIRECT)
                    ? txFilterPtr->TxModeTrueDirectRepetitions[TxIpduPtr->TxModeTrueRefId].TxModeRepetitionPeriod
                    : TxIpduStatePtr->NTimeCnt;
#endif
#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)
            TxIpduStatePtr->NTimeCnt =
                (txMode == COM_TX_MODE_MIXED)
                    ? txFilterPtr->TxModeTrueMixedRepetitions[TxIpduPtr->TxModeTrueRefId].TxModeRepetitionPeriod
                    : TxIpduStatePtr->NTimeCnt;
#endif
        }
    }
    if ((!MDTRun) && (COM_TX_DELAY_EN == TxDelay))
    {
        ret = TRUE;
    }
    return ret;
}
#endif

/**
 * @brief Called by Com_MainFunctionTx.the handle of Com_MainFunctionTx,when the tx mode is period.
 *
 */
#if (0u < COM_MAX_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER)  \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
COM_LOCAL boolean
    Com_MainFuncTxPeriod(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun)
{
    boolean                                 ret         = FALSE;
    const Com_TxModeAndSignalFilterPtrType* txFilterPtr = Com_CfgTxModeAndSignalFilterPtr;
    if (TxIpduStatePtr->PeriodCnt > 0u)
    {
        (TxIpduStatePtr->PeriodCnt) -= 1u;
    }
    if (0u == TxIpduStatePtr->PeriodCnt)
    {
/*delay transmit of the MDT */
#if (STD_ON == COM_MDT_ENABLE)
        if (MDTRun)
        {
            TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_DELAY_EN;
        }
        else
#endif
        {
            ret = TRUE;
        }

#if (STD_ON == COM_TMS_ENABLE)
        uint8 TMS = (TxIpduStatePtr->TxIpduRTStFlag & COM_TX_TMS_EN);
        if (TMS != COM_TX_TMS_EN)
        {
            if (COM_TX_MODE_MIXED == TxIpduStatePtr->IpduTxMode)
            {
#if (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)
                TxIpduStatePtr->PeriodCnt =
                    txFilterPtr->TxModeFalseMixedRepetitions[TxIpduPtr->TxModeFalseRefId].TxModeTimePeriod;
#endif
            }
            else
            {
#if ((0u < COM_MAX_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
                TxIpduStatePtr->PeriodCnt =
                    txFilterPtr->TxModeFalsePeriod[TxIpduPtr->TxModeFalseRefId].TxModeTimePeriod;
#endif
            }
        }
        else
#endif
        {
            if (COM_TX_MODE_MIXED == TxIpduStatePtr->IpduTxMode)
            {
#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)
                TxIpduStatePtr->PeriodCnt =
                    txFilterPtr->TxModeTrueMixedRepetitions[TxIpduPtr->TxModeTrueRefId].TxModeTimePeriod;
#endif
            }
            else
            {
#if (0u < COM_MAX_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)
                TxIpduStatePtr->PeriodCnt = txFilterPtr->TxModeTruePeriod[TxIpduPtr->TxModeTrueRefId].TxModeTimePeriod;
#endif
            }
        }
    }
    uint8 TxDelay = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_DELAY_EN;
    if ((!MDTRun) && (COM_TX_DELAY_EN == TxDelay))
    {
        ret = TRUE;
    }
    if (ret && (0u == TxIpduStatePtr->DMCnt))
    {
        /*reset the tx MD counter*/
        TxIpduStatePtr->DMCnt = TxIpduPtr->TxIpduDM;
    }
    return ret;
}
#endif

/**
 * @brief Called by Com_MainFunctionTx.
 *        the handle of Com_MainFunctionTx,when the tx mode is mixed.
 *        return the boolean value for the tx pdu need to transmit or not
 *
 */
#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
    || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
COM_LOCAL boolean
    Com_MainFuncTxMixed(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, boolean MDTRun)
{
    boolean transmitRequest       = FALSE;
    boolean directTransmitRequest = Com_MainFuncTxDirect(TxIpduStatePtr, TxIpduPtr, MDTRun);
    boolean periodTransmitRequest = Com_MainFuncTxPeriod(TxIpduStatePtr, TxIpduPtr, MDTRun);

    if (directTransmitRequest || periodTransmitRequest)
    {
        transmitRequest = TRUE;
    }
    return transmitRequest;
}
#endif

/**
 * @brief Called by Com_TriggerTransmit,Com_TriggerIPDUSend,
 *        Com_TriggerIPDUSendWithMetaData,Com_TxIpduControl.
 *        It is called in case the transmission is not possible because the corresponding I-PDU group is stopped.
 *
 */
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
COM_LOCAL void Com_InvokeErrorNotification(const Com_TxIPduType* TxIpduPtr)
{
    const Com_Tx_ErrorAndNotificationType* txErrorAndNotificationPtr = Com_ConfigStd->TxErrorAndNotificationPtr;
#if (COM_MAX_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalIdType txSignalNumber, cnt;
    txSignalNumber = TxIpduPtr->IPduSignalsRefEndId;
    cnt            = TxIpduPtr->IpduSignalRefStartId;
    for (; cnt < txSignalNumber; ++cnt)
    {
        const Com_TxSignalType* txSignalPtr = &Com_CfgTxSignalPtr[cnt];
        /*call signal Tx Err Notification*/
        if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
            == (txSignalPtr->TxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
        {
            /* only ErrorNotify or both ErrorNotify and Notify*/
            txErrorAndNotificationPtr->TxSignalErrorAndNotification[txSignalPtr->TxSignalErrAndNotifyFncId]();
        }
    }
#endif
#if (COM_MAX_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
    Com_TxSignalGroupIdType signalGroupNumber, signalGroupId;
    signalGroupNumber = TxIpduPtr->IPduSignalGroupsRefEndId;
    signalGroupId     = TxIpduPtr->IPduSignalGroupsRefStartId;
    for (; signalGroupId < signalGroupNumber; ++signalGroupId)
    {
        const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
        /*call signal group Tx Err Notification*/
        if (COM_TX_SIGNAL_ERROR_NOTIFICATION_EN
            == (txSignalGroupPtr->TxSignalNotificationType & COM_TX_SIGNAL_ERROR_NOTIFICATION_EN))
        {
            txErrorAndNotificationPtr
                ->TxSignalGroupErrorAndNotification[txSignalGroupPtr->ComTxSigGrpErrAndNotifyFncId]();
        }
    }
#endif
    return;
}
#endif

/**
 * @brief Send the tx pdu in Com_MainFunctionTx
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
COM_LOCAL void Com_MainFunction_SendPdu(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    PduIdType TxIpduId,
#endif
    Com_MainFunctionIdType TxMainfunctionId)
{
    PduInfoType         pduInfo;
    uint8*              txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
    Com_TxIpduBufIdType ipduBufferId         = TxIpduPtr->TxIPduBufIndex;
    uint16              ipduLength           = TxIpduStatePtr->TxIpduLength;
    pduInfo.SduDataPtr                       = &txIpduRuntimeBuffPtr[ipduBufferId];
    pduInfo.SduLength                        = ipduLength;
#if (STD_ON == COM_METADATA_SUPPORT)
    uint8 metaDataLength = TxIpduPtr->MetaDataLength;
    if (0u < metaDataLength)
    {
        PduLengthType metaDataPos =
            TxIpduPtr->IPduNoDynSignalLength + TxIpduPtr->IPduMaxDynSignalLength + (PduLengthType)ipduBufferId;
        pduInfo.MetaDataPtr = &txIpduRuntimeBuffPtr[metaDataPos];
    }
    else
#endif
    {
        pduInfo.MetaDataPtr = NULL_PTR;
    }
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
    boolean ret = TRUE;
    /*invoke the callout API*/
    if (NULL_PTR != TxIpduPtr->ComIPduCallout)
    {
        ret = TxIpduPtr->ComIPduCallout((TxIpduId + Com_CfgRxPduNum), &pduInfo);
    }
    if (ret)
#endif
    {
#if (STD_ON == COM_TXTPPDU_SUPPORT)
        if (COM_PDU_TP == TxIpduPtr->ComIPduType)
        {
            pduInfo.SduDataPtr = NULL_PTR;
        }
#endif
        /*Set the Transmiting flag before call PduR_Transmit,
         *avoid the TxConfirmation come before set the Transmiting flag*/
        TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_TRANSMITING_EN;
        if (E_OK == PduR_ComTransmit(TxIpduPtr->ComPduIdRef, &pduInfo))
        {
            uint8 TxDelay = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_DELAY_EN;
            if (COM_TX_DELAY_EN == TxDelay)
            {
                TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_DELAY_DIS;
            }
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
            Com_ResetTxIpduMDT(TxIpduStatePtr, TxIpduPtr);
#endif
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
            if (COM_TRANSMIT == TxIpduPtr->ComTxIPduClearUpdateBit)
            {
                /*clear all signal group/signal/dest description signal update bit of the Tx Pdu*/
                Com_ClearTxIPduUpdates(TxIpduPtr, TxMainfunctionId);
            }
#endif
            TxIpduStatePtr->TxOffset = 0u;
        }
        else
        {
            TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TRANSMITING_DIS;
/*ComRetryFailedTransmitRequests*/
#if (STD_ON == COM_RETRY_FAILED_TRANSMIT_REQUESTS)
            TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_DELAY_EN;
#else
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
            Com_ResetTxIpduMDT(TxIpduStatePtr, TxIpduPtr);
#endif
#endif
        }
    }
    return;
}
#endif

#if (0u < COM_MAX_TMCTXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
COM_LOCAL uint64 Com_GetOldSignalValue(Com_SignalType SignalType, uint16 SignalBufferId, uint16 PartitionId)
{
    uint64 oldValue = COM_UNUSED_UINT64;
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    const boolean* signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    const uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    const uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    const uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[PartitionId];
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    const uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[PartitionId];
#endif
    switch (SignalType)
    {
/*According to the signal type,
 *Calculated the signal value is filtered or not?*/
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
        /* PRQA S 4304 ++ */ /* VL_Com_BooleanType */
        oldValue = (uint64)signalBoolRuntimeBuffPtr[SignalBufferId];
        /* PRQA S 4304 -- */
        break;
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    case COM_UINT8:
    case COM_SINT8:
        oldValue = (uint64)signal8BitRuntimeBuffPtr[SignalBufferId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
        oldValue = (uint64)signalBit16RuntimeBuffPtr[SignalBufferId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    case COM_UINT32:
    case COM_SINT32:
        oldValue = (uint64)signal32BitRuntimeBuffPtr[SignalBufferId];
        break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    case COM_UINT64:
    case COM_SINT64:
        oldValue = (uint64)signal64BitRuntimeBuffPtr[SignalBufferId];
        break;
#endif
    default:
        /*do nothing*/
        break;
    }
    return oldValue;
}
#endif
/**
 * @brief the tx signal value change,calculate the new TMC and TMS.
 *
 */
#if (0u < COM_MAX_TXSIGNAL_NUMBER) && (STD_ON == COM_TMS_ENABLE)
COM_LOCAL void Com_TxSignalTMHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
    Com_SignalIdType            TxSignalId,
    const void*                 TxSignalDataPtr,
    Com_MainFunctionIdType      TxMainfunctionId)
{
    boolean* txSignalTMCRuntimePtr = Com_TxSignalTMCRunTime[TxMainfunctionId];
#if (0u < COM_MAX_TXSIGNAL_FILTERTYPE_MAX_NUMBER)
    const Com_TxSignalType* txSignalPtr = &Com_CfgTxSignalPtr[TxSignalId];
    if (COM_UNUSED_TXSIGNALFILTERID != txSignalPtr->FilterIndex)
    {
        uint64 oldValue = Com_GetOldSignalValue(
            txSignalPtr->ComSignalType,
            txSignalPtr->SignalInitValueId,
            TxIpduPtr->IpduPartitionId);
        /*calculate the TMC of the signal*/
        txSignalTMCRuntimePtr[txSignalPtr->TMCBufferId] =
            Com_TxSignalTMCCalculate(COM_TX_SIGNAL, TxSignalDataPtr, txSignalPtr, oldValue);
    }
#endif
    uint8   oldTMS = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_TMS_EN;
    boolean TMS    = Com_TxIpduTMSCalculate(TxIpduPtr, txSignalPtr->TxMainfunctionId);
    if (!TMS && (COM_TX_TMS_EN == oldTMS))
    {
        TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TMS_DIS;
        TxIpduStatePtr->IpduTxMode = TxIpduPtr->TxFalseModeMode;
        if ((COM_TX_MODE_MIXED == TxIpduStatePtr->IpduTxMode)
            || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == TxIpduStatePtr->IpduTxMode)
            || (COM_TX_MODE_PERIODIC == TxIpduStatePtr->IpduTxMode))
        {
            Com_GetTxModeOffset(TxIpduStatePtr, TxIpduPtr, FALSE);
        }
    }
    else if (TMS && (COM_TX_TMS_EN != oldTMS))
    {
        TxIpduStatePtr->TxIpduRTStFlag |= COM_TX_TMS_EN;
        TxIpduStatePtr->IpduTxMode = TxIpduPtr->TxTrueModeMode;
        if ((COM_TX_MODE_MIXED == TxIpduStatePtr->IpduTxMode)
            || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == TxIpduStatePtr->IpduTxMode)
            || (COM_TX_MODE_PERIODIC == TxIpduStatePtr->IpduTxMode))
        {
            Com_GetTxModeOffset(TxIpduStatePtr, TxIpduPtr, TRUE);
        }
    }
    else
    {
        /* do nothing */
    }
    COM_NOUSED(TxSignalId);
    COM_NOUSED(TxSignalDataPtr);
    return;
}
#endif

/**
 * @brief Called by Com_SendSignal,Com_SendTxGroupSignalHandle.
 * set Tx signal(not dynamic signal) to signalbuffer,return the buffer value is changed or not.
 *
 */
#if ((COM_MAX_TXSIGNALGROUP_NUMBER > 0u) || (0u < COM_MAX_TXSIGNAL_NUMBER))
/* PRQA S 6040 ++ */ /* VL_MTR_Com_STPAR */
COM_LOCAL boolean Com_SetTxSignalBuff(
    Com_SignalType SignalType,
    uint16         ComSignalLength,
    uint16         SignalBufferId,
    const void*    SignalNewDataPtr,
    uint8          ComBitSize,
    uint16         PartitionId)
/* PRQA S 6040 -- */
{
    boolean ret = FALSE;

    SchM_Enter_Com_Context();
    switch (SignalType)
    {
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    case COM_BOOLEAN:
    {
        boolean* signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[PartitionId];
        /* PRQA S 2109 ++  */ /* VL_Com_BoolPromoted */
        ret = (*((const boolean*)(SignalNewDataPtr)) != signalBoolRuntimeBuffPtr[SignalBufferId]) ? TRUE : FALSE;
        /* PRQA S 2109 -- */ /* VL_Com_BoolPromoted */
        signalBoolRuntimeBuffPtr[SignalBufferId] = *((const boolean*)SignalNewDataPtr);
    }
    break;
#endif

#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    case COM_UINT8:
    case COM_SINT8:
    {
        uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
        uint8  signalValueU8            = *((const uint8*)SignalNewDataPtr) & (COM_UNUSED_UINT8 >> (8u - ComBitSize));
        ret                             = (signalValueU8 != signal8BitRuntimeBuffPtr[SignalBufferId]) ? TRUE : FALSE;
        signal8BitRuntimeBuffPtr[SignalBufferId] = signalValueU8;
    }
    break;

#if ((STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE))
    case COM_UINT8_N:
    {
        uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
        /* Copy Uint8N signal value one byte by one */
        if (0
            != IStdLib_MemCmp(
                &signal8BitRuntimeBuffPtr[SignalBufferId],
                (const uint8*)SignalNewDataPtr,
                ComSignalLength))
        {
            (void)IStdLib_MemCpy(
                &signal8BitRuntimeBuffPtr[SignalBufferId],
                (const uint8*)SignalNewDataPtr,
                ComSignalLength);
            ret = TRUE;
        }
    }
    break;
#endif
#endif

#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    case COM_UINT16:
    case COM_SINT16:
    {
        uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[PartitionId];
        uint16  signalValueU16 = *((const uint16*)SignalNewDataPtr) & (COM_UNUSED_UINT16 >> (16u - ComBitSize));
        ret                    = (signalValueU16 != signalBit16RuntimeBuffPtr[SignalBufferId]) ? TRUE : FALSE;
        signalBit16RuntimeBuffPtr[SignalBufferId] = signalValueU16;
    }
    break;
#endif

#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    case COM_SINT32:
    case COM_UINT32:
    case COM_FLOAT32:
    {
        uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[PartitionId];
        uint32  signalValueU32            = *((const uint32*)SignalNewDataPtr);
        signalValueU32 =
            (COM_FLOAT32 != SignalType) ? (signalValueU32 & (COM_UNUSED_UINT32 >> (32u - ComBitSize))) : signalValueU32;
        ret = (signalValueU32 != signal32BitRuntimeBuffPtr[SignalBufferId]) ? TRUE : FALSE;
        signal32BitRuntimeBuffPtr[SignalBufferId] = signalValueU32;
    }
    break;
#endif

#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    case COM_SINT64:
    case COM_UINT64:
    case COM_FLOAT64:
    {
        uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[PartitionId];
        uint64  signalNewValue            = *((const uint64*)SignalNewDataPtr);
        signalNewValue =
            (COM_FLOAT64 != SignalType) ? (signalNewValue & (COM_UNUSED_UINT64 >> (64u - ComBitSize))) : signalNewValue;
        ret = (signalNewValue != signal64BitRuntimeBuffPtr[SignalBufferId]) ? TRUE : FALSE;
        signal64BitRuntimeBuffPtr[SignalBufferId] = signalNewValue;
    }
    break;
#endif

    default:
        /*do nothing*/
        break;
    }
    SchM_Exit_Com_Context();
    COM_NOUSED(ComSignalLength);
    COM_NOUSED(SignalBufferId);
    COM_NOUSED(SignalNewDataPtr);
    return ret;
}
#endif
/**
 * @brief Called by Com_SendSignal.
 *        pack the Tx signal into ipdu Tx buffer.
 *
 */
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
COM_LOCAL void
    Com_TxSignalPack(const Com_TxSignalType* txSignalPtr, uint16 PartitionId, Com_MainFunctionIdType TxMainfunctionId)
{
    uint8*                txIpduRuntimeBuffPtr = Com_TxIPduRuntimeBuff[TxMainfunctionId];
    PduIdType             txIpduId             = txSignalPtr->IpduRefIndex;
    const Com_TxIPduType* txIpduPtr            = &Com_CfgTxIPduPtr[txIpduId];
    Com_SignalType        signalType           = txSignalPtr->ComSignalType;
    Com_TxIpduBufIdType   ipduBufferIndex      = txIpduPtr->TxIPduBufIndex;
    uint16                signalBufferId       = txSignalPtr->SignalInitValueId;
    uint64                txData;
#if (                                                                                          \
    (0u < COM_MAX_TXIPDUBUFF_SIZE)                                                             \
    && (((0u < COM_MAX_SIGNAL_8BITBUFF_SIZE) && (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE)) \
        || (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)))
    Com_TxIpduBufIdType ipduBuffPos   = ipduBufferIndex + (Com_TxIpduBufIdType)(txSignalPtr->SigLsbBytePos);
    uint8*              iPduBufferPtr = &txIpduRuntimeBuffPtr[ipduBuffPos];
#endif

#if (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_N_ENABLE)
    if (COM_UINT8_N == signalType)
    {
        /*pack COM_UINT8_N type group signal*/
        uint16 signalLength = txSignalPtr->ComSignalLength;
        SchM_Enter_Com_Context();
        (void)IStdLib_MemCpy(iPduBufferPtr, &Com_Signal8BitRuntimeBuff[PartitionId][signalBufferId], signalLength);
        SchM_Exit_Com_Context();
    }
    else
#endif
#if ((0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE) && (0u < COM_MAX_TXIPDUBUFF_SIZE))
        /*pack boolean type signal*/
        if (COM_BOOLEAN == signalType)
        {
            uint8 value = (uint8)(1u << ((uint8)txSignalPtr->SignalPosInByte));
            if (Com_SignalBoolRuntimeBuff[PartitionId][signalBufferId])
            {
                *iPduBufferPtr |= value;
            }
            else
            {
                *iPduBufferPtr &= (uint8)(~value);
            }
        }
        else
#endif
        {
            /*pack signal expect signal type(boolean,COM_UINT8_N,COM_UINT8_DYN)*/
            switch (signalType)
            {
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
            case COM_UINT8:
            case COM_SINT8:
                txData = (uint64)(Com_Signal8BitRuntimeBuff[PartitionId][signalBufferId]);
                break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
            case COM_UINT16:
            case COM_SINT16:
                txData = (uint64)(Com_SignalBit16RuntimeBuff[PartitionId][signalBufferId]);
                break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
            case COM_SINT32:
            case COM_UINT32:
            case COM_FLOAT32:
                txData = (uint64)(Com_Signal32BitRuntimeBuff[PartitionId][signalBufferId]);
                break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
            case COM_SINT64:
            case COM_UINT64:
            case COM_FLOAT64:
                txData = Com_Signal64BitRuntimeBuff[PartitionId][signalBufferId];
                break;
#endif
            default:
                txData = 0u;
                break;
            }
            Com_TxSignalPackHandle(COM_TX_SIGNAL, txSignalPtr, txData, &txIpduRuntimeBuffPtr[ipduBufferIndex]);
        }
}
#endif

/**
 * @brief Called by Com_SendSignal,Com_SendDynSignal.send Tx signal handle.
 *
 */
#if (0u < COM_MAX_TXSIGNAL_NUMBER)
COM_LOCAL uint8 Com_SendTxSignalHandle(
    const Com_TxIPduType*       TxIpduPtr,
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    Com_TransferPropertyType    SignalTxProperty,
    boolean                     DMSignal,
    boolean                     ValueChanged)
{
    uint16                            txIpduDM     = TxIpduPtr->TxIpduDM;
    Com_TxModeNumberOfRepetitionsType txModeRptNum = 1u;
    Com_TxModeModeType                ipduTxMode   = TxIpduStatePtr->IpduTxMode;
#if ((STD_ON == COM_TX_SIG_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ON_CHANGE_ENABLE))
    boolean initDMCnt = FALSE;
#endif
    /*the signal config timeout,the DM timer isn't start by other signals,the Tx mode is not NONE.then start the DM
     * timer here*/
    if ((0u == TxIpduStatePtr->DMCnt) && DMSignal && (COM_TX_MODE_NONE != TxIpduStatePtr->IpduTxMode))
    {
        TxIpduStatePtr->DMCnt = txIpduDM;
#if ((STD_ON == COM_TX_SIG_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ON_CHANGE_ENABLE))
        initDMCnt = TRUE;
#endif
    }
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)                              \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)                         \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER) || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER) \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)                                                         \
    || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
    if ((COM_TX_MODE_DIRECT == ipduTxMode) || (COM_TX_MODE_MIXED == ipduTxMode)
        || (COM_TX_MODE_DIRECT_WITHOUT_REPETITION == ipduTxMode)
        || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
    {
#if (STD_ON == COM_TXMODE_AND_FILTER_SUPPORT)
        const Com_TxModeAndSignalFilterPtrType* txFilterPtr = Com_CfgTxModeAndSignalFilterPtr;
#endif
#if (STD_ON == COM_TMS_ENABLE)
        uint8 TMS = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_TMS_EN;
        if (TMS != COM_TX_TMS_EN)
        {
#if (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)
            txModeRptNum =
                (COM_TX_MODE_DIRECT == ipduTxMode)
                    ? txFilterPtr->TxModeFalseDirectRepetitions[TxIpduPtr->TxModeFalseRefId].TxModeNumberOfRepetitions
                    : txModeRptNum;
#endif

#if (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER)
            txModeRptNum =
                (COM_TX_MODE_MIXED == ipduTxMode)
                    ? txFilterPtr->TxModeFalseMixedRepetitions[TxIpduPtr->TxModeFalseRefId].TxModeNumberOfRepetitions
                    : txModeRptNum;
#endif
        }
        else
#endif
        {
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER)
            txModeRptNum =
                (COM_TX_MODE_DIRECT == ipduTxMode)
                    ? txFilterPtr->TxModeTrueDirectRepetitions[TxIpduPtr->TxModeTrueRefId].TxModeNumberOfRepetitions
                    : txModeRptNum;
#endif

#if (0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER)
            txModeRptNum =
                (COM_TX_MODE_MIXED == ipduTxMode)
                    ? txFilterPtr->TxModeTrueMixedRepetitions[TxIpduPtr->TxModeTrueRefId].TxModeNumberOfRepetitions
                    : txModeRptNum;
#endif
        }
#if (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ON_CHANGE_ENABLE) \
    || (STD_ON == COM_TX_SIG_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)                                         \
    || (STD_ON == COM_TX_SIG_PROP_TRIG_NO_REPETITION_ENABLE)
        /* PRQA S 2109 ++ */ /* VL_Com_BoolPromoted */
        boolean onChangeIsTrue =
            (SignalTxProperty == COM_TRIGGERED_ON_CHANGE)
                ? ValueChanged
                : ((SignalTxProperty == COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION) ? ValueChanged : TRUE);
/* PRQA S 2109 -- */ /* VL_Com_BoolPromoted */
#if (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_PROP_TRIGGERED_ON_CHANGE_ENABLE)
        if (((SignalTxProperty == COM_TRIGGERED_ON_CHANGE) || (SignalTxProperty == COM_TRIGGERED)) && onChangeIsTrue)
        {
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum   = txModeRptNum;
            /*For an I-PDU with ComTxModeMode DIRECT and TxModeNumberOfRepetitions > 0, the AUTOSAR COM
              module shall reset an already running transmission deadline monitoring timer in case another send
              request for this I-PDU is initiated*/
            if ((1u < txModeRptNum) && (!initDMCnt) && DMSignal)
            {
                TxIpduStatePtr->DMCnt = txIpduDM;
            }
        }
#endif
#if (STD_ON == COM_TX_SIG_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE) || (STD_ON == COM_TX_SIG_PROP_TRIG_NO_REPETITION_ENABLE)
        if (((SignalTxProperty == COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION)
             || (SignalTxProperty == COM_TRIGGERED_WITHOUT_REPETITION))
            && onChangeIsTrue)
        {
            TxIpduStatePtr->NTimeCnt = 0u;
            TxIpduStatePtr->RptNum   = 1u;
        }
#endif
#endif
    }
    else
#endif
    {
        TxIpduStatePtr->NTimeCnt = 0u;
        TxIpduStatePtr->RptNum   = 0u;
    }
    COM_NOUSED(ValueChanged);
    COM_NOUSED(SignalTxProperty);
    return E_OK;
}
#endif

/**
 * @brief Called by Com_SendSignal,Com_InvalidateSignalGroup.send Tx group signal handle.
 *
 */
#if (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)
COM_LOCAL uint8 Com_SendTxGroupSignalHandle(Com_SignalIdType TxGroupSignalId, const void* SignalDataPtr)
{
    const Com_TxGroupSignalType* txGroupSignalPtr = &Com_CfgTxGroupSignalPtr[TxGroupSignalId];
    Com_TxSignalGroupIdType      signalGroupId    = txGroupSignalPtr->ComSignalGroupRef;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[signalGroupId];
    Com_SignalType               groupSignalType  = txGroupSignalPtr->ComSignalType;
    uint8                        ret;

/*dynamic signal isn't send by the API,InitialValueOnly signal group don't expect to be called by any API*/
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_INITIAL_VALUE_ONLY_ENABLE)
    if ((COM_UINT8_DYN == groupSignalType) || txSignalGroupPtr->ComInitialValueOnly)
    {
        ret = COM_SERVICE_NOT_AVAILABLE;
    }
    else
#endif
    {
        const PduIdType       txIpduId  = txSignalGroupPtr->IpduRefIndex;
        const Com_TxIPduType* txIpduPtr = &Com_CfgTxIPduPtr[txIpduId];
#if (0u < COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER) && (STD_ON == COM_TMS_ENABLE)
        if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->FilterIndex)
        {
            const Com_MainFunctionIdType txMainfunctionId        = txSignalGroupPtr->TxMainfunctionId;
            boolean*                     txGroupSignalTMCBuffPtr = Com_TxGroupSignalTMCBuffer[txMainfunctionId];
            Com_SignalIdType             txSignalTMCId           = txGroupSignalPtr->TMCBufferId;
            uint64                       oldValue                = Com_GetOldSignalValue(
                txGroupSignalPtr->ComSignalType,
                txGroupSignalPtr->SignalInitValueId,
                txIpduPtr->IpduPartitionId);
            /*calculate the TMC of the signal*/
            txGroupSignalTMCBuffPtr[txSignalTMCId] =
                Com_TxSignalTMCCalculate(COM_TX_GROUP_SIGNAL, SignalDataPtr, txGroupSignalPtr, oldValue);
        }
#endif
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE) \
    || (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)
        boolean valueChanged =
#endif
            Com_SetTxSignalBuff(
                groupSignalType,
                txGroupSignalPtr->ComSignalLength,
                txGroupSignalPtr->SignalInitValueId,
                SignalDataPtr,
                txGroupSignalPtr->ComBitSize,
                txIpduPtr->IpduPartitionId);
        Com_TransferPropertyType signalTxProperty =
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
            (COM_GROUPSIGNAL_TMC_NONE != txGroupSignalPtr->ComTransferProperty) ? txGroupSignalPtr->ComTransferProperty
                                                                                :
#endif
                                                                                txSignalGroupPtr->ComTransferProperty;
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE) \
    || (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)                                               \
    || (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE)
        Com_TxSignalGroupRunTimeStateType* txSigGrpStPtr = &Com_TxSignalGroupState[signalGroupId];
        switch (signalTxProperty)
        {
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ENABLE)
        case COM_TRIGGERED:
            txSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
            txSigGrpStPtr->TxSigGrpRTStFlag &= COM_WITHOUTREPETITION_DIS;
            break;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
        case COM_TRIGGERED_ON_CHANGE:
            if (valueChanged)
            {
                txSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                txSigGrpStPtr->TxSigGrpRTStFlag &= COM_WITHOUTREPETITION_DIS;
            }
            break;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_ON_CH_NO_REPETITION_ENABLE)
        case COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION:
            if (valueChanged)
            {
                txSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
                txSigGrpStPtr->TxSigGrpRTStFlag |= COM_WITHOUTREPETITION_EN;
            }
            break;
#endif
#if (STD_ON == COM_TX_SIG_GROUP_PROP_TRIG_NO_REPETITION_ENABLE)
        case COM_TRIGGERED_WITHOUT_REPETITION:
            txSigGrpStPtr->TxSigGrpRTStFlag |= COM_NTIMESTRANSMITNEED_EN;
            txSigGrpStPtr->TxSigGrpRTStFlag |= COM_WITHOUTREPETITION_EN;
            break;
#endif
        default:
            /*do nothing*/
            break;
        }
#endif
        ret = E_OK;
    }
    COM_NOUSED(SignalDataPtr);

    return ret;
}
#endif

/**
 * @brief used to handle all Tx Ipdu state change or not.
 *
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_TXIPDUGROUP_NUMBER > 0u)
/* PRQA S 6030,6070 ++ */ /* VL_MTR_Com_STMIF,VL_MTR_Com_STCAL */
void Com_TxIpduControl(
    Com_MainFunctionIdType TxMainfunctionId,
    Com_IpduGroupIdType    IpduGroupId,
    boolean                Initialize,
    boolean                IsEnable)
/* PRQA S 6030,6070 -- */
{
    PduIdType startTxIpduId = Com_GetStartOfMainFunctionTx(TxMainfunctionId);
    PduIdType endTxIpduId   = Com_GetEndOfMainFunctionTx(TxMainfunctionId);
    for (PduIdType cirCnt = startTxIpduId; cirCnt < endTxIpduId; ++cirCnt)
    {
        const Com_TxIPduType* txIpduPtr = &Com_CfgTxIPduPtr[cirCnt];
        for (Com_RxIpduGroupIdType counter = txIpduPtr->IPduGroupsRefStartId; counter < txIpduPtr->IPduGroupsRefEndId;
             ++counter)
        {
            if (IpduGroupId == Com_ConfigStd->IpduGroupRefPtr->TxIpduGroupRef[counter])
            {
                uint16                      idpduIdPerMainfunction = cirCnt - startTxIpduId;
                Com_TxIPduRunTimeStateType* txIpduStatePtr =
                    &Com_TxIPduRunTimeState[TxMainfunctionId][idpduIdPerMainfunction];
                uint8 activeEnable = txIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN;
                if (IsEnable && (COM_TX_ACTIVE_EN != activeEnable))
                {
                    SchM_Enter_Com_MultiCore_Lock();
                    txIpduStatePtr->TxIpduRTStFlag |= COM_TX_ACTIVE_EN;
                    SchM_Exit_Com_MultiCore_Lock();
#if (STD_ON == COM_MDT_ENABLE)
                    /*reset ComMinimumDelayTime of I-PDUs in transmission*/
                    txIpduStatePtr->MDTCnt = 0u;
                    /*reset Tx time out counter*/
                    txIpduStatePtr->DMCnt =
                        (txIpduPtr->TxFalseModeMode != COM_TX_MODE_NULL)
                            ? 0u
                            : ((COM_TX_MODE_NONE != txIpduPtr->TxTrueModeMode) ? 0u : txIpduPtr->TxIpduDM);
#endif
                    /*clear all signal/signal group/dest description signal's update bit of the tx Ipdu*/
#if (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) \
    || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE)
                    Com_ClearTxIPduUpdates(txIpduPtr, TxMainfunctionId);
#endif
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
                    /*reset OCCURRENCE of filter with FilterAlgorithm ONE_EVERY_N*/
                    Com_ResetTxOccurrenceOfPduFilter(txIpduPtr);
#endif
                    if (Initialize)
                    {
                        /*init the tx ipdu buffer,all signal buffer(included in the ipdu) and init the TMC,TMS*/
                        Com_TxModeModeType                ipduTxMode = txIpduStatePtr->IpduTxMode;
                        const Com_TxIPduRunTimeStateType* txIpduInitStateMainfunctionPtr =
                            Com_ConfigStd->TxIpduInitStatePtr[TxMainfunctionId];
                        const Com_TxIPduRunTimeStateType* txIpduInitStatePtr =
                            &txIpduInitStateMainfunctionPtr[idpduIdPerMainfunction];
                        Com_ResetTxPduBufferAndSignalBuffer(
                            cirCnt,
                            TxMainfunctionId,
                            txIpduStatePtr,
                            txIpduInitStatePtr);
                        if ((COM_TX_MODE_MIXED == ipduTxMode) || (COM_TX_MODE_PERIODIC == ipduTxMode)
                            || (COM_TX_MODE_MIXED_WITHOUT_REPETITION == ipduTxMode))
                        {
                            boolean TMS = (COM_TX_TMS_EN == (txIpduStatePtr->TxIpduRTStFlag & COM_TX_TMS_EN));
                            Com_GetTxModeOffset(txIpduStatePtr, txIpduPtr, TMS);
                        }
                    }
                }
                else if (!IsEnable && (COM_TX_ACTIVE_EN == activeEnable))
                {
                    SchM_Enter_Com_MultiCore_Lock();
                    txIpduStatePtr->TxIpduRTStFlag &= COM_TX_ACTIVE_DIS;
                    SchM_Exit_Com_MultiCore_Lock();
                    uint8 Transmiting = txIpduStatePtr->TxIpduRTStFlag & COM_TX_TRANSMITING_EN;
                    if (COM_TX_TRANSMITING_EN == Transmiting)
                    {
#if (STD_ON == COM_TX_SIGNAL_ERROR_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_ERROR_NOTIFICATION_ENABLE)
                        Com_InvokeErrorNotification(txIpduPtr);
#endif
#if (STD_ON == COM_CANCELLATION_SUPPORT)
                        if (txIpduPtr->ComIPduCancellationSupport)
                        {
                            (void)PduR_ComCancelTransmit(txIpduPtr->ComPduIdRef);
                        }
#endif
                        txIpduStatePtr->TxIpduRTStFlag &= COM_TX_TRANSMITING_DIS;
                    }
                    txIpduStatePtr->DMCnt = 0u;
                }
                else
                {
                    /* do nothing */
                }
                break;
            }
        }
    }
}
#endif

/**
 * @brief Called by Com_SendDynSignal.
 *        set Tx dynamic signal to signalbuffer,return the buffer value is changed or not.
 *
 */
#if (                                                                                            \
    ((0u < COM_MAX_TXSIGNALGROUP_NUMBER) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)) \
    || ((0u < COM_MAX_TXSIGNAL_NUMBER) && (STD_ON == COM_TX_SIGNAL_TYPE_UINT8_DYN_ENABLE)))
COM_LOCAL boolean Com_SetDynTxSignalBuff(
    const Com_TxIPduType*             TxIpduPtr,
    const Com_TxIPduRunTimeStateType* TxRuntimeStatePtr,
    uint8*                            Signal8BitRuntimeBuffPtr,
    const void*                       SignalNewDataPtr,
    uint16                            Length)
{
    boolean ret = FALSE;

/* Copy Uint8N signal value one byte by one */
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    SchM_Enter_Com_Context();
    if (0 != IStdLib_MemCmp(Signal8BitRuntimeBuffPtr, (const uint8*)SignalNewDataPtr, Length))
    {
        (void)IStdLib_MemCpy(Signal8BitRuntimeBuffPtr, (const uint8*)SignalNewDataPtr, Length);
        ret = TRUE;
    }
    SchM_Exit_Com_Context();
#endif
    if (!ret)
    {
        uint16 oldSignalLength = TxRuntimeStatePtr->TxIpduLength - (uint16)TxIpduPtr->IPduNoDynSignalLength;
        if (oldSignalLength != Length)
        {
            ret = TRUE;
        }
    }
    COM_NOUSED(SignalNewDataPtr);
    return ret;
}
#endif
/**
 * @brief Called by Com_SendSignalGroup.
 *        pack the Tx signal group into ipdu Tx buffer.
 *
 */
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
COM_LOCAL void Com_TxSignalGroupPack(Com_SignalGroupIdType SignalGroupId, Com_MainFunctionIdType TxMainfunctionId)
{
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[SignalGroupId];

    Com_TxGroupSignalIdType groupSignalEndId   = txSignalGroupPtr->GroupSignalRefEndId;
    Com_TxGroupSignalIdType groupSignalStartId = txSignalGroupPtr->GroupSignalRefStartId;
    PduIdType               txIpduId           = txSignalGroupPtr->IpduRefIndex;
    const Com_TxIPduType*   txIpduPtr          = &Com_CfgTxIPduPtr[txIpduId];

    for (; groupSignalStartId < groupSignalEndId; ++groupSignalStartId)
    {
        const Com_TxGroupSignalType* txGroupSignalPtr = &Com_CfgTxGroupSignalPtr[groupSignalStartId];
#if (0u < COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER)
        if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->FilterIndex)
        {
            Com_SignalIdType groupSignalTMCId      = txGroupSignalPtr->TMCBufferId;
            boolean*         txSignalTMCRuntimePtr = Com_TxSignalTMCRunTime[TxMainfunctionId];
            const boolean* txSignalInitTMCPtr = Com_CfgTxModeAndSignalFilterPtr->TxSignalTMCInitVale[TxMainfunctionId];
            txSignalTMCRuntimePtr[groupSignalTMCId] = txSignalInitTMCPtr[groupSignalTMCId];
        }
#endif
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (COM_UINT8_DYN == txGroupSignalPtr->ComSignalType)
        {
            Com_TxSignalGroupRunTimeStateType* txSigGrpStPtr  = &Com_TxSignalGroupState[SignalGroupId];
            Com_SignalPositionType             sigLsbBytePos  = txGroupSignalPtr->SigLsbBytePos;
            uint16                             signalBufferId = txGroupSignalPtr->SignalInitValueId;
            uint16                             length         = txSigGrpStPtr[SignalGroupId].DynamicSignalLength;
#if ((0u < COM_MAX_TXIPDUBUFF_SIZE) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE))
            /* Com_TxDynSignalPack */
            uint8*              signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[txIpduPtr->IpduPartitionId];
            uint8*              txIpduRuntimeBuffPtr     = Com_TxIPduRuntimeBuff[TxMainfunctionId];
            Com_TxIpduBufIdType ipduBufferIndex = txIpduPtr->TxIPduBufIndex + (Com_TxIpduBufIdType)sigLsbBytePos;
            SchM_Enter_Com_Context();
            (void)IStdLib_MemCpy(
                &txIpduRuntimeBuffPtr[ipduBufferIndex],
                &signal8BitRuntimeBuffPtr[signalBufferId],
                length);
            SchM_Exit_Com_Context();
#endif
        }
        else
#endif
        {
            Com_TxGroupSignalPack(txGroupSignalPtr, txIpduPtr, TxMainfunctionId);
        }
    }
    return;
}
#endif
/**
 * @brief Called by Com_SendSignalGroupArray.
 *        update every group signal buffer,calculate every TMC,
 *        return the signal group is changed or not for trigger transmit.
 *
 */
#if ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_MAX_TXSIGNALGROUP_NUMBER > 0u))
COM_LOCAL boolean Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal(
    Com_SignalGroupIdType   TxSignalGroupId,
    Com_TxGroupSignalIdType GroupSignalNumber,
    Com_TxIpduBufIdType     TxIPduBufferId)
{
    boolean        triggerOnChange = FALSE;
    Com_SignalType groupSignalType;
    uint16         groupSignalInitValueId;
    uint64         groupSignalValue;
#if (0u < COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER) && (STD_ON == COM_TMS_ENABLE)
    Com_SignalIdType txSignalTMCId;
    uint64           oldValue;
#endif
    const Com_TxGroupSignalType* txGroupSignalPtr;
    const Com_TxSignalGroupType* txSignalGroupPtr = &Com_CfgTxSignalGroupPtr[TxSignalGroupId];
    Com_MainFunctionIdType       txMainfunctionId = txSignalGroupPtr->TxMainfunctionId;
    PduIdType                    txIpduId         = txSignalGroupPtr->IpduRefIndex;
    const Com_TxIPduType*        txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
    uint16                       partitionId      = txIpduPtr->IpduPartitionId;
    Com_TxGroupSignalIdType      cnt              = txSignalGroupPtr->GroupSignalRefStartId;
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    boolean* signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[partitionId];
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    uint8* signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[partitionId];
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[partitionId];
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[partitionId];
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[partitionId];
#endif

    for (; cnt < GroupSignalNumber; ++cnt)
    {
        txGroupSignalPtr       = &Com_CfgTxGroupSignalPtr[cnt];
        groupSignalType        = txGroupSignalPtr->ComSignalType;
        groupSignalInitValueId = txGroupSignalPtr->SignalInitValueId;
#if (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE)
        if (COM_UINT8_N == groupSignalType)
        {
#if ((0u < COM_MAX_TXIPDUBUFF_SIZE) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE))
            triggerOnChange = Com_TxGroupSignalUpdateUint8N(
                TxIPduBufferId,
                cnt,
                groupSignalInitValueId,
                partitionId,
                txMainfunctionId);
#endif
        }
        else
#endif
        {
            const uint8* txRuntimeBuffPtr = Com_TxIPduRuntimeBuff[txMainfunctionId];
            /*UnPack tx group signal from signal group array,used to calculate the TMC of the signal and judge the
             * signal value if changed or not,if changed update the signal value*/
            groupSignalValue = Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
                groupSignalType,
#endif
                COM_TX_GROUP_SIGNAL,
                txGroupSignalPtr,
                &txRuntimeBuffPtr[TxIPduBufferId]);
#if (0u < COM_MAX_TXGRPSIG_FILTERTYPE_MAX_NUMBER) && (STD_ON == COM_TMS_ENABLE)
            if (COM_UNUSED_TXGRPSIGFILTERID != txGroupSignalPtr->FilterIndex)
            {
                boolean* txSignalTMCRuntimePtr = Com_TxSignalTMCRunTime[txMainfunctionId];
                txSignalTMCId                  = txGroupSignalPtr->TMCBufferId;
                oldValue                       = Com_GetOldSignalValue(
                    txGroupSignalPtr->ComSignalType,
                    txGroupSignalPtr->SignalInitValueId,
                    partitionId);
                txSignalTMCRuntimePtr[txSignalTMCId] =
                    Com_TxSignalTMCCalculate(COM_TX_GROUP_SIGNAL, &groupSignalValue, txGroupSignalPtr, oldValue);
            }
#endif
            SchM_Enter_Com_Context();
            switch (groupSignalType)
            {
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
            case COM_BOOLEAN:
                /* PRQA S 2109,4340 ++ */ /* VL_Com_BoolPromoted */
                triggerOnChange =
                    (signalBoolRuntimeBuffPtr[groupSignalInitValueId] != (boolean)groupSignalValue) ? TRUE : FALSE;
                signalBoolRuntimeBuffPtr[groupSignalInitValueId] = (boolean)groupSignalValue;
                /* PRQA S 2109,4340 -- */ /* VL_Com_BoolPromoted */
                break;
#endif
#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
            case COM_UINT8:
            case COM_SINT8:
                triggerOnChange =
                    (signal8BitRuntimeBuffPtr[groupSignalInitValueId] != (uint8)groupSignalValue) ? TRUE : FALSE;
                signal8BitRuntimeBuffPtr[groupSignalInitValueId] = (uint8)groupSignalValue;
                break;
#endif
#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
            case COM_UINT16:
            case COM_SINT16:
                triggerOnChange =
                    (signalBit16RuntimeBuffPtr[groupSignalInitValueId] != (uint16)groupSignalValue) ? TRUE : FALSE;
                signalBit16RuntimeBuffPtr[groupSignalInitValueId] = (uint16)groupSignalValue;
                break;
#endif
#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
            case COM_UINT32:
            case COM_SINT32:
            case COM_FLOAT32:
                triggerOnChange =
                    (signal32BitRuntimeBuffPtr[groupSignalInitValueId] != (uint32)groupSignalValue) ? TRUE : FALSE;
                signal32BitRuntimeBuffPtr[groupSignalInitValueId] = (uint32)groupSignalValue;
                break;
#endif
#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
            case COM_UINT64:
            case COM_SINT64:
            case COM_FLOAT64:
                triggerOnChange =
                    (signal64BitRuntimeBuffPtr[groupSignalInitValueId] != (uint64)groupSignalValue) ? TRUE : FALSE;
                signal64BitRuntimeBuffPtr[groupSignalInitValueId] = (uint64)groupSignalValue;
                break;
#endif
            default:
                /*do nothing*/
                break;
            }
            SchM_Exit_Com_Context();
        }
#if (0u < COM_MAX_TXIPDUBUFF_SIZE) && (STD_ON == COM_TX_SIG_GROUP_PROP_TRIGGERED_ON_CHANGE_ENABLE)
        /* PRQA S 2109 ++  */ /* VL_Com_BoolPromoted */
        triggerOnChange = (txGroupSignalPtr->ComTransferProperty == COM_TRIGGERED_ON_CHANGE) ? triggerOnChange : FALSE;
/* PRQA S 2109 --  */ /* VL_Com_BoolPromoted */
#endif
    }
    return triggerOnChange;
}
#endif
/* PRQA S 1532,2895 -- */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 0316,2844 -- */
/* PRQA S 2814,2824 -- */
/* PRQA S 3120,3132 -- */
