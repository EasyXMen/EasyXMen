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
 **  @file               : Com.c
 **  @author             : shengnan.sun
 **  @date               : 2024/09/01
 **  @vendor             : isoft
 **  @description        : Implementation for Com
 **
 ***********************************************************************************************************************/
/* =================================================== inclusions =================================================== */
#include "Com_Internal.h"
#include "Com_Cbk.h"
#include "Det.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
/* PRQA S 3120,3132 ++ */  /* VL_QAC_MagicNum */
/* PRQA S 2814,2824 ++ */  /* VL_QAC_DerefNullPtr */
/* PRQA S 0316,2844 ++ */  /* VL_QAC_0316,VL_Com_NullPtrValue */
/* PRQA S 6520,6530 EOF */ /* VL_MTR_Com_STVAR,VL_MTR_Com_STECT */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 5015 ++ */ /* VL_Com_UntrustedMethod */
/**
 * @brief         Init the RxPdu Runtime Manager and Buffer
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66040
 */
COM_LOCAL void Com_PduBufferRxInit(Com_MainFunctionIdType RxMainfunctionId);
/**
 * @brief         Init the TxPdu Runtime Manager and Buffer
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66042
 */
COM_LOCAL void Com_PduBufferTxInit(Com_MainFunctionIdType TxMainfunctionId);
/**
 * @brief         Init the Runtime RxSignal/RxGroupSignal Buffer
 * @param[in]     PartitionId:Partition index
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66044
 */
COM_LOCAL void Com_SignalBufferInit(uint16 PartitionId);
/**
 * @brief         If TxPdu TxConfirmation handle
 * @param[in]     TxIpduStatePtr:State of TxIpdu
 * @param[in]     TxIpduPtr:Pointer of TxIpdu
 * @param[in]     TxMainfunctionId:MainfunctionId from Tx
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66046
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
COM_LOCAL void Com_TxConfirmationHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
    Com_MainFunctionIdType      TxMainfunctionId);
#endif
/**
 * @brief         Rx Pdu DM timeout enable/disable handle
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @param[in]     IpduGroupId:IpduGroup index
 * @param[in]     IsEnable:Enable is or not
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66047
 */
#if (COM_MAX_RXIPDUGROUP_NUMBER > 0u) && (0u < COM_MAX_RXIPDU_NUMBER)
COM_LOCAL void
    Com_RxPduDMCtrHandle(Com_MainFunctionIdType RxMainfunctionId, Com_IpduGroupIdType IpduGroupId, boolean IsEnable);
#endif
/**
 * @brief         Used to handle all Rx Ipdu state change or not
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @param[in]     IpduGroupId:IpduGroup index
 * @param[in]     Initialize:Initialize
 * @param[in]     IsEnable:Enable is or not
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66048
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_RXIPDUGROUP_NUMBER > 0u)
COM_LOCAL void Com_RxIpduControl(
    Com_MainFunctionIdType RxMainfunctionId,
    Com_IpduGroupIdType    IpduGroupId,
    boolean                Initialize,
    boolean                IsEnable);
#endif
/**
 * @brief         Init the rx ipdu buffer,all signal buffer(included in the ipdu,except source signal)
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @param[in]     RxMainfunctionId:MainfunctionId from Rx
 * @param[in]     RxIpduInitStatePtr:RxIpdu init state pointer
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66051
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
COM_LOCAL void Com_ResetRxPduBufferAndSignalBuffer(
    const Com_RxIPduType*             RxIpduPtr,
    Com_RxIPduRunTimeStateType*       RxIpduStatePtr,
    Com_MainFunctionIdType            RxMainfunctionId,
    const Com_RxIPduRunTimeStateType* RxIpduInitStatePtr);
#endif
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
/**
 * @brief         Set the reception deadline monitoring timer for the included signals and signal groups to
 *                the configured ComFirstTimeout.
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66052
 */
COM_LOCAL void Com_ResetRxPduTimeOut(const Com_RxIPduType* RxIpduPtr, Com_RxIPduRunTimeStateType* RxIpduStatePtr);
#endif
#if ((COM_MAX_IPDUGROUP_NUMBER > 0u) && ((COM_MAX_RXSIGNAL_NUMBER + COM_MAX_RXSIGNALGROUP_NUMBER) > 0u))
/**
 * @brief         Set the reception deadline monitoring timer for the included signals and signal groups to 0
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @param[in]     RxIpduStatePtr:State pointer of RxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66053
 */
COM_LOCAL void Com_DisableRxPduTimeOut(const Com_RxIPduType* RxIpduPtr, Com_RxIPduRunTimeStateType* RxIpduStatePtr);
#endif
/**
 * @brief         Reset OCCURRENCE of filter with FilterAlgorithm ONE_EVERY_N.
 * @param[in]     RxIpduPtr:Pointer of RxIpdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-66078
 */
#if ((COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u))
COM_LOCAL void Com_ResetRxOccurrenceOfPduFilter(const Com_RxIPduType* RxIpduPtr);
#endif
/**
 * @brief         Com_InitHandle is called by Com_Init
 * @param[in]     ApplicationPartitionId: Id of Application
 * @param[in]     PartitionId: Id of Partition
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
COM_LOCAL void Com_InitHandle(
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    ApplicationType ApplicationPartitionId,
#endif
    uint16 PartitionId);
/**
 * @brief         Com_DeInitHandle is called by Com_DeInit
 * @param[in]     ApplicationPartitionId: Id of Application
 * @param[in]     PartitionId: Id of Partition
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
COM_LOCAL void Com_DeInitHandle(
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    ApplicationType ApplicationPartitionId,
#endif
    uint16 PartitionId);
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
/**
 * @brief         get internal IPdu Group Id of the Com module
 * @param[in]     apiId: Id of Application
 * @param[in]     ipduGroupId: Id of IPduGroup
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
COM_LOCAL Com_IpduGroupIdType Com_GetIpduGroupIdFromUserId(uint16 apiId, Com_IpduGroupIdType ipduGroupId);
/**
 * @brief         get internal ISignal or GroupSignal Id of the Com module
 * @param[in]     apiId: Id of Application
 * @param[in]     signalId: Id of ISignal
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
COM_LOCAL Com_SignalIdType Com_GetSignalAndGroupSignalIdFromUserId(uint16 apiId, Com_SignalIdType signalId);
/**
 * @brief         get internal SignalGroup Id of the Com module
 * @param[in]     apiId: Id of Application
 * @param[in]     signalGroupId: Id of ISignalGroup
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
COM_LOCAL Com_SignalGroupIdType Com_GetSignalGroupIdFromUserId(uint16 apiId, Com_SignalGroupIdType signalGroupId);
/**
 * @brief         get internal Pdu Id of the Com module
 * @param[in]     apiId: Id of Application
 * @param[in]     pduId: Id of IPdu
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace
 */
COM_LOCAL PduIdType Com_GetPduIdFromUserId(uint16 apiId, PduIdType pduId);
#endif
/* PRQA S 5015 -- */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* ============================================ internal data definitions =========================================== */
#define COM_START_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"
COM_LOCAL Com_StatusType Com_Status[COM_MAX_PARTITION_NUMBER];
#define COM_STOP_SEC_VAR_CLEARED_8
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
Com_SignalIdType Com_CfgTxSignalNum;
Com_SignalIdType Com_CfgRxSignalNum; /* PRQA S 1504 */ /* VL_Com_1504 */
#define COM_STOP_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
PduIdType Com_CfgTxPduNum; /* PRQA S 1514 */ /* VL_Com_1514 */
PduIdType Com_CfgRxPduNum; /* PRQA S 1504 */ /* VL_Com_1504 */
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"

#define COM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Com_MemMap.h"
COM_LOCAL boolean Com_GlobalInitStatus = FALSE;
#define COM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Com_MemMap.h"

#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
#define COM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Com_MemMap.h"
COM_LOCAL boolean Com_IpduGroupDMEnable[COM_MAX_IPDUGROUP_NUMBER];
COM_LOCAL boolean Com_IpduGroupEnable[COM_MAX_IPDUGROUP_NUMBER];
#define COM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Com_MemMap.h"
#endif
/* ============================================ external data definitions =========================================== */
#define COM_START_SEC_VAR_CLEARED_PTR
#include "Com_MemMap.h"
const Com_ConfigType* Com_ConfigStd = NULL_PTR;

#if (COM_MAX_RXIPDU_NUMBER > 0u)
const Com_RxIPduType* Com_CfgRxIPduPtr = NULL_PTR;
#endif
#if (COM_MAX_TXIPDU_NUMBER > 0u)
const Com_TxIPduType* Com_CfgTxIPduPtr = NULL_PTR;
#endif
const Com_RxSignalType*      Com_CfgRxSignalPtr      = NULL_PTR;
const Com_TxSignalType*      Com_CfgTxSignalPtr      = NULL_PTR;
const Com_RxSignalGroupType* Com_CfgRxSignalGroupPtr = NULL_PTR;
const Com_TxSignalGroupType* Com_CfgTxSignalGroupPtr = NULL_PTR;
const Com_RxGroupSignalType* Com_CfgRxGroupSignalPtr = NULL_PTR;
const Com_TxGroupSignalType* Com_CfgTxGroupSignalPtr = NULL_PTR;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
const uint16* Com_CfgMainFuncRxPartitionRangePtr = NULL_PTR;
const uint16* Com_CfgMainFuncTxPartitionRangePtr = NULL_PTR;
#endif
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
const Com_GwMappingConfigType* Com_CfgGwMappingConfigPtr = NULL_PTR;
#endif
#if (STD_ON == COM_SIGNAL_INITVALUE_SUPPORT)
const Com_SignalInitValueType* Com_CfgSignalInitValuePtr = NULL_PTR;
#endif
#if (STD_ON == COM_TXMODE_AND_FILTER_SUPPORT)
const Com_TxModeAndSignalFilterPtrType* Com_CfgTxModeAndSignalFilterPtr = NULL_PTR;
#endif
#if (STD_ON == COM_SIGNAL_TIMEOUT_SUPPORT)
const Com_SignalTimeoutType* Com_CfgSignalTimeoutPtr = NULL_PTR;
#endif
#define COM_STOP_SEC_VAR_CLEARED_PTR
#include "Com_MemMap.h"
/* ========================================== external function definitions ========================================= */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 1503,1532 ++ */ /* VL_QAC_NoUsedApi,VL_Com_ReferencedOnlyOne */
/**
 * This service initializes internal and external interfaces and
 * variables of the AUTOSAR COM module layer for the further processing.
 * After calling this function the inter-ECU communication is still disabled.
 *
 */
void Com_Init(const Com_ConfigType* config)
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8 errorId = COM_E_NONE;
    if (NULL_PTR == config)
    {
        errorId = COM_E_PARAM_POINTER;
    }
    else if (
        (config->IpduGroupNum > COM_MAX_IPDUGROUP_NUMBER)
        || ((config->RxPduNum + config->TxPduNum) > COM_MAX_RXIPDU_NUMBER + COM_MAX_TXIPDU_NUMBER))
    {
        errorId = COM_E_INIT_FAILED;
    }
    else
#endif
    {
        uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
        ApplicationType applicationPartitionId = GetApplicationID();
        partitionId                            = Com_PartitonIdFromList(applicationPartitionId);
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_UNUSED_UINT16 == partitionId)
        {
            errorId = COM_E_PARTITION;
        }
        else if (COM_UNINIT != Com_Status[partitionId])
        {
            errorId = COM_E_INIT_FAILED;
        }
        else
#endif
        {
            if (NULL_PTR == Com_ConfigStd)
            {
                Com_ConfigStd = config;
            }

/* Load config parameter to ram */
#if (COM_MAX_RXIPDU_NUMBER > 0u)
            Com_CfgRxIPduPtr = Com_ConfigStd->RxIPduPtr;
#endif
#if (COM_MAX_TXIPDU_NUMBER > 0u)
            Com_CfgTxIPduPtr = Com_ConfigStd->TxIPduPtr;
#endif
            Com_CfgRxSignalPtr      = Com_ConfigStd->RxSignalPtr;
            Com_CfgTxSignalPtr      = Com_ConfigStd->TxSignalPtr;
            Com_CfgRxSignalGroupPtr = Com_ConfigStd->RxSignalGroupPtr;
            Com_CfgTxSignalGroupPtr = Com_ConfigStd->TxSignalGroupPtr;
            Com_CfgRxGroupSignalPtr = Com_ConfigStd->RxGroupSignalPtr;
            Com_CfgTxGroupSignalPtr = Com_ConfigStd->TxGroupSignalPtr;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
            Com_CfgMainFuncRxPartitionRangePtr = Com_ConfigStd->MainFuncRxPartitionRangePtr;
            Com_CfgMainFuncTxPartitionRangePtr = Com_ConfigStd->MainFuncTxPartitionRangePtr;
#endif
#if (COM_MAX_GWMAPPING_NUMBER > 0u)
            Com_CfgGwMappingConfigPtr = Com_ConfigStd->GwMappingConfigPtr;
#endif
#if (STD_ON == COM_SIGNAL_INITVALUE_SUPPORT)
            Com_CfgSignalInitValuePtr = Com_ConfigStd->SignalInitValuePtr;
#endif
#if (STD_ON == COM_TXMODE_AND_FILTER_SUPPORT)
            Com_CfgTxModeAndSignalFilterPtr = Com_ConfigStd->TxModeAndSignalFilterPtr;
#endif
#if (STD_ON == COM_SIGNAL_TIMEOUT_SUPPORT)
            Com_CfgSignalTimeoutPtr = Com_ConfigStd->SignalTimeoutPtr;
#endif
            Com_CfgTxSignalNum = Com_ConfigStd->TxSignalNum;
            Com_CfgRxSignalNum = Com_ConfigStd->RxSignalNum;
            Com_CfgTxPduNum    = Com_ConfigStd->TxPduNum;
            Com_CfgRxPduNum    = Com_ConfigStd->RxPduNum;

            Com_InitHandle(
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
                applicationPartitionId,
#endif
                partitionId);
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INIT_ID, errorId);
    }
#endif
    return;
}
/**
 * This service stops the inter-ECU communication. All started I-PDU groups are stopped and have to be started again,
 * if needed, after Com_Init is called. By a call to Com_DeInit.
 * the AUTOSAR COM module is put into an not initialized state.
 *
 */
void Com_DeInit(void)
{
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    ApplicationType applicationPartitionId = GetApplicationID();
    partitionId                            = Com_PartitonIdFromList(applicationPartitionId);
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8 errorId = COM_E_NONE;
    if (COM_UNUSED_UINT16 == partitionId)
    {
        errorId = COM_E_PARTITION;
    }
    else if (COM_INIT != Com_Status[partitionId])
    {
        errorId = COM_E_INIT_FAILED;
    }
    else
#endif
    {
        Com_DeInitHandle(
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
            applicationPartitionId,
#endif
            partitionId);
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_DEINIT_ID, errorId);
    }
#endif

    return;
}

/* PRQA S 2895 ++ */ /* VL_Com_NegtiveValueType */
/**
 * Starts a preconfigured I-PDU group.
 *
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
/* PRQA S 3677 ++ */ /* VL_Com_NeverModify */
void Com_IpduGroupStart(Com_IpduGroupIdType IpduGroupId, boolean initialize)
/* PRQA S 3677 -- */
{
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    partitionId = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
        Com_IpduGroupIdType internalId = Com_GetIpduGroupIdFromUserId(COM_IPDUGROUPSTART_ID, IpduGroupId);
#else
        Com_IpduGroupIdType internalId = IpduGroupId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if ((internalId >= COM_MAX_IPDUGROUP_NUMBER) || (internalId >= Com_ConfigStd->IpduGroupNum))
        {
            errorId = COM_E_PARAM;
        }
        else
#endif
        {
            if (!Com_IpduGroupEnable[internalId])
            {
                Com_MainFunctionIdType mainfunctionRxNum = Com_ConfigStd->MainFunctionRxNum;
                Com_MainFunctionIdType mainfunctionTxNum = Com_ConfigStd->MainFunctionTxNum;
                Com_MainFunctionIdType mainfunctionIdMaxNum =
                    (mainfunctionRxNum > mainfunctionTxNum) ? mainfunctionRxNum : mainfunctionTxNum;
                Com_IpduGroupEnable[internalId] = TRUE;
                for (Com_MainFunctionIdType mainFunctionId = 0u; mainFunctionId < mainfunctionIdMaxNum;
                     ++mainFunctionId)
                {
                    if (mainFunctionId < mainfunctionRxNum)
                    {
                        /*base i-pdu group state,set all rx pdu to enable or disable.
                         * base initialize,select if or not init the rx pdu when state change from disable to
                         * enable*/
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_RXIPDUGROUP_NUMBER > 0u)
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
                        if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[mainFunctionId])
#endif
                        {
                            Com_RxIpduControl(mainFunctionId, internalId, initialize, TRUE);
                        }
#endif
                    }
                    if (mainFunctionId < mainfunctionTxNum)
                    {
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_TXIPDUGROUP_NUMBER > 0u)
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
                        if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[mainFunctionId])
#endif
                        {
                            Com_TxIpduControl(mainFunctionId, internalId, initialize, TRUE);
                        }
#endif
                    }
                }
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_IPDUGROUPSTART_ID, errorId);
    }
#endif

    return;
}
#endif

/**
 * Stops a preconfigured I-PDU group.
 *
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
/* PRQA S 3677 ++ */ /* VL_Com_NeverModify */
void Com_IpduGroupStop(Com_IpduGroupIdType IpduGroupId)
/* PRQA S 3677 -- */
{
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint16 partitionId = 0u;
    uint8  errorId     = COM_E_NONE;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    partitionId = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
        Com_IpduGroupIdType internalId = Com_GetIpduGroupIdFromUserId(COM_IPDUGROUPSTOP_ID, IpduGroupId);
#else
        Com_IpduGroupIdType internalId = IpduGroupId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if ((internalId >= COM_MAX_IPDUGROUP_NUMBER) || (internalId >= Com_ConfigStd->IpduGroupNum))
        {
            errorId = COM_E_PARAM;
        }
        else
#endif
        {
            if (Com_IpduGroupEnable[internalId]) /* PRQA S 2842 */ /* VL_Com_Unreachable */
            {
                Com_MainFunctionIdType mainfunctionRxNum = Com_ConfigStd->MainFunctionRxNum;
                Com_MainFunctionIdType mainfunctionTxNum = Com_ConfigStd->MainFunctionTxNum;
                Com_MainFunctionIdType mainfunctionIdMaxNum =
                    (mainfunctionRxNum > mainfunctionTxNum) ? mainfunctionRxNum : mainfunctionTxNum;
                Com_IpduGroupEnable[internalId] = FALSE; /* PRQA S 2842 */ /* VL_Com_Unreachable */
                for (Com_MainFunctionIdType mainFunctionId = 0u; mainFunctionId < mainfunctionIdMaxNum;
                     ++mainFunctionId)
                {
                    if (mainFunctionId < mainfunctionRxNum)
                    {
                        /*base i-pdu group state,set all rx pdu to enable or disable.
                         * base initialize,select if or not init the rx pdu when state change from disable to
                         * enable*/
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_RXIPDUGROUP_NUMBER > 0u)
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
                        if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[mainFunctionId])
#endif
                        {
                            Com_RxIpduControl(mainFunctionId, internalId, FALSE, FALSE);
                        }
#endif
                    }
                    if (mainFunctionId < mainfunctionTxNum)
                    {
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_TXIPDUGROUP_NUMBER > 0u)
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
                        if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[mainFunctionId])
#endif
                        {
                            Com_TxIpduControl(mainFunctionId, internalId, TRUE, FALSE);
                        }
#endif
                    }
                }
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_IPDUGROUPSTOP_ID, errorId);
    }
#endif
    return;
}
#endif

/**
 * Enables the reception deadline monitoring for the I-PDUs within the given IPDU group.
 *
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
void Com_EnableReceptionDM(Com_IpduGroupIdType IpduGroupId)
{
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint16 partitionId = 0u;
    uint8  errorId     = COM_E_NONE;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    boolean partitionIsTrue = FALSE;
    partitionId             = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
            (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_ENABLERECEPTIONDM_ID, COM_E_UNINIT);
        }
        else
#endif
        { /* SWS_Com_00486 */
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_IpduGroupIdType internalId = Com_GetIpduGroupIdFromUserId(COM_ENABLERECEPTIONDM_ID, IpduGroupId);
#else
            Com_IpduGroupIdType internalId = IpduGroupId;
#endif
            Com_MainFunctionIdType mainfunctionRxNum = Com_ConfigStd->MainFunctionRxNum;
            if ((mainfunctionRxNum > 0u) && (!Com_IpduGroupDMEnable[internalId]))
            {
                Com_IpduGroupDMEnable[internalId] = TRUE;
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_RXIPDUGROUP_NUMBER > 0u)
                Com_MainFunctionIdType mainFunctionId = 0u;
                for (; mainFunctionId < mainfunctionRxNum; ++mainFunctionId)
                {
/*base i-pdu group state,set all rx pdu to enable or disable.
 * base initialize,select if or not init the rx pdu when state change from disable to enable*/
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
                    if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[mainFunctionId])
#endif
                    {
                        Com_RxPduDMCtrHandle(mainFunctionId, internalId, TRUE);
                    }
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_ENABLERECEPTIONDM_ID, errorId);
    }
#endif
    COM_NOUSED(IpduGroupId);
}
#endif

/**
 * Disables the reception deadline monitoring for the I-PDUs within the given IPDU group.
 *
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
void Com_DisableReceptionDM(Com_IpduGroupIdType IpduGroupId)
{
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    boolean partitionIsTrue = FALSE;
    partitionId             = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        { /* SWS_Com_00486 */
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_IpduGroupIdType internalId = Com_GetIpduGroupIdFromUserId(COM_DISABLERECEPTIONDM_ID, IpduGroupId);
#else
            Com_IpduGroupIdType internalId = IpduGroupId;
#endif
            Com_MainFunctionIdType mainfunctionRxNum = Com_ConfigStd->MainFunctionRxNum;
            if ((mainfunctionRxNum > 0u) && (Com_IpduGroupDMEnable[internalId]))
            {
                Com_IpduGroupDMEnable[internalId] = FALSE;
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_RXIPDUGROUP_NUMBER > 0u)
                for (Com_MainFunctionIdType mainFunctionId = 0u; mainFunctionId < mainfunctionRxNum; ++mainFunctionId)
                {
/*base i-pdu group state,set all rx pdu to enable or disable.
 * base initialize,select if or not init the rx pdu when state change from disable to
 * enable*/
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
                    if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[mainFunctionId])
#endif
                    {
                        Com_RxPduDMCtrHandle(mainFunctionId, internalId, FALSE);
                    }
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_DISABLERECEPTIONDM_ID, errorId);
    }
#endif
    COM_NOUSED(IpduGroupId);
}
#endif

/**
 * Returns the status of the AUTOSAR COM module.
 *
 */
Com_StatusType Com_GetStatus(void)
{
    Com_StatusType moduleStatus = COM_UNINIT;
    for (uint16 partitionId = 0u; partitionId < COM_MAX_PARTITION_NUMBER; ++partitionId)
    {
        if (COM_INIT == Com_Status[partitionId])
        {
            moduleStatus = COM_INIT;
            break;
        }
    }
    return moduleStatus;
}
#if (STD_ON == COM_VERSION_INFO_API)
/**
 * Returns the version information of this module.
 *
 */
void Com_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (NULL_PTR == versioninfo)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_GETVERSIONINFO_ID, COM_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->vendorID         = COM_VENDOR_ID;
        versioninfo->moduleID         = COM_MODULE_ID;
        versioninfo->sw_major_version = COM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = COM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = COM_SW_PATCH_VERSION;
    }
}
#endif
/*******************************************************************************
**                      Communication Services                                **
*******************************************************************************/
/**
 * The service Com_SendSignal updates the signal(include group signal) object
 * identified by SignalId with the signal referenced by the SignalDataPtr parameter.
 *
 */
uint8 Com_SendSignal(Com_SignalIdType SignalId, const void* SignalDataPtr)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if (NULL_PTR == SignalDataPtr)
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalAndGroupSignalIdFromUserId(COM_SENDSIGNAL_ID, SignalId);
#else
            Com_SignalIdType internalId = SignalId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= (Com_CfgTxSignalNum + Com_ConfigStd->TxGroupSignalNum))
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)
                Com_MainFunctionIdType txMainfunctionId =
                    (internalId < Com_CfgTxSignalNum)
                        ? Com_GetTxSigMainfunctionId((Com_TxSignalIdType)(internalId))
                        : Com_GetTxGrpSigMainfunctionId((Com_TxGroupSignalIdType)(internalId - Com_CfgTxSignalNum));
/*the signal is Tx Signal*/
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    ret = Com_SendSignalHandle(internalId, SignalDataPtr, txMainfunctionId);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNAL_ID, errorId);
    }
#endif
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    return ret;
}
/**
 * @brief The service Com_SendDynSignal updates the signal object identified by SignalId
 *        with the signal referenced by the SignalDataPtr parameter.
 *
 */
uint8 Com_SendDynSignal(Com_SignalIdType SignalId, const void* SignalDataPtr, uint16 Length)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)
    /* Multiple partition check. */
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if (NULL_PTR == SignalDataPtr)
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalAndGroupSignalIdFromUserId(COM_SENDDYNSIGNAL_ID, SignalId);
#else
            Com_SignalIdType internalId = SignalId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= (Com_CfgTxSignalNum + Com_ConfigStd->TxGroupSignalNum))
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
                Com_MainFunctionIdType txMainfunctionId =
                    (internalId < Com_CfgTxSignalNum)
                        ? Com_GetTxSigMainfunctionId((Com_TxSignalIdType)(internalId))
                        : Com_GetTxGrpSigMainfunctionId((Com_TxGroupSignalIdType)(internalId - Com_CfgTxSignalNum));
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    ret = Com_SendDynSignalHandle(internalId, SignalDataPtr, Length, txMainfunctionId);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDDYNSIGNAL_ID, errorId);
    }
#endif
#endif
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    COM_NOUSED(Length);
    return ret;
}
/**
 * Com_ReceiveSignal copies the data of the signal identified by SignalId to the location specified by
 * SignalDataPtr.
 *
 */
uint8 Com_ReceiveSignal(Com_SignalIdType SignalId, void* SignalDataPtr)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if (NULL_PTR == SignalDataPtr)
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalAndGroupSignalIdFromUserId(COM_RECEIVESIGNAL_ID, SignalId);
#else
            Com_SignalIdType internalId = SignalId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= (Com_CfgRxSignalNum + Com_ConfigStd->RxGroupSignalNum))
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (COM_MAX_RXSIGNAL_NUMBER > 0u) || ((COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (COM_MAX_RXGROUPSIGNAL_NUMBER > 0u))
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                /*the signal is Rx Signal*/
                Com_MainFunctionIdType rxMainfunctionId =
                    (internalId < Com_CfgRxSignalNum)
                        ? Com_GetRxSigMainfunctionId((Com_RxSignalIdType)(internalId))
                        : Com_GetRxGrpSigMainfunctionId((Com_RxGroupSignalIdType)(internalId - Com_CfgRxSignalNum));
                if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[rxMainfunctionId])
#endif
                {
                    if (internalId < Com_CfgRxSignalNum)
                    {
#if (0u < COM_MAX_RXSIGNAL_NUMBER)
                        ret = Com_ReceiveSignalHandle(internalId, SignalDataPtr);
#endif
                    }
                    /*the signal is Rx Group Signal*/
                    else
                    {
#if (COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (COM_MAX_RXGROUPSIGNAL_NUMBER > 0u)
                        ret = Com_ReceiveGroupSignalHandle(internalId, SignalDataPtr);
#endif
                    }
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNAL_ID, errorId);
    }
#endif
    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    return ret;
}
/**
 * Com_ReceiveDynSignal copies the data of the signal identified by SignalId to the
 * location specified by SignalDataPtr and stores the length of the dynamical length
 * signal at the position given by the Length parameter.
 *
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
uint8 Com_ReceiveDynSignal(Com_SignalIdType SignalId, void* SignalDataPtr, uint16* Length)
/* PRQA S 3673 -- */
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if (NULL_PTR == SignalDataPtr)
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalAndGroupSignalIdFromUserId(COM_RECEIVEDYNSIGNAL_ID, SignalId);
#else
            Com_SignalIdType internalId = SignalId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= (Com_CfgRxSignalNum + Com_ConfigStd->RxGroupSignalNum))
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (COM_MAX_RXSIGNAL_NUMBER > 0u) || ((COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (COM_MAX_RXGROUPSIGNAL_NUMBER > 0u))
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                Com_MainFunctionIdType rxMainfunctionId =
                    (internalId < Com_CfgRxSignalNum)
                        ? Com_GetRxSigMainfunctionId((Com_RxSignalIdType)(internalId))
                        : Com_GetRxGrpSigMainfunctionId((Com_RxGroupSignalIdType)(internalId - Com_CfgRxSignalNum));
                if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[rxMainfunctionId])
#endif
                {
                    /*the signal is Rx Signal*/
                    if (internalId < COM_MAX_RXSIGNAL_NUMBER)
                    {
#if (                                                                     \
    (0u < COM_MAX_RXSIGNAL_NUMBER) && (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE) \
    && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE))
                        ret = Com_ReceiveDynSignalHandle(internalId, SignalDataPtr, Length);
#endif
                    }
                    /*the signal is Rx Group Signal*/
                    else
                    {
#if (                                                                            \
    ((COM_MAX_RXSIGNALGROUP_NUMBER > 0u) && (COM_MAX_RXGROUPSIGNAL_NUMBER > 0u)) \
    && (0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE))                               \
    && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
                        ret = Com_ReceiveDynGroupSignalHandle(internalId, SignalDataPtr, Length);
#endif
                    }
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVEDYNSIGNAL_ID, errorId);
    }
#endif

    COM_NOUSED(SignalId);
    COM_NOUSED(SignalDataPtr);
    COM_NOUSED(Length);
    return ret;
}

/**
 * The service Com_SendSignalGroup copies the content of the associated shadow buffer to the associated I-PDU.
 *
 */
uint8 Com_SendSignalGroup(Com_SignalGroupIdType SignalGroupId)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalGroupIdFromUserId(COM_SENDSIGNALGROUP_ID, SignalGroupId);
#else
            Com_SignalIdType internalId = SignalGroupId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_ConfigStd->TxSignalGroupNum)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (0u < COM_MAX_TXSIGNALGROUP_NUMBER)
                Com_MainFunctionIdType txMainfunctionId =
                    Com_GetTxSigGrpMainfunctionId((Com_TxSignalGroupIdType)(internalId));
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    ret = Com_SendSignalGroupHandle(internalId, txMainfunctionId);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUP_ID, errorId);
    }
#endif
    COM_NOUSED(SignalGroupId);
    return ret;
}
/**
 * @brief The service Com_ReceiveSignalGroup copies the received signal group from the I-PDU to the
 *        shadow buffer.
 *
 */
uint8 Com_ReceiveSignalGroup(Com_SignalGroupIdType SignalGroupId)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalGroupIdFromUserId(COM_RECEIVESIGNALGROUP_ID, SignalGroupId);
#else
            Com_SignalIdType internalId = SignalGroupId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_ConfigStd->RxSignalGroupNum)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (0u < COM_MAX_RXSIGNALGROUP_NUMBER)
                Com_MainFunctionIdType rxMainfunctionId =
                    Com_GetRxSigGrpMainfunctionId((Com_RxSignalGroupIdType)(internalId));
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[rxMainfunctionId])
#endif
                {
                    PduIdType ipduRef               = Com_CfgRxSignalGroupPtr[internalId].IpduRefIndex;
                    PduIdType ipduIdPerMainfunction = ipduRef - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
                    Com_RxIPduRunTimeStateType* rxIpduStatePtr =
                        &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
                    uint8                 Receiving = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_RECEIVING_EN;
                    const Com_RxIPduType* rxIpduPtr = &Com_CfgRxIPduPtr[ipduRef];
                    /*IPDU is receiving data*/
                    if (COM_RX_RECEIVING_EN == Receiving)
                    {
                        ret = COM_BUSY;
                    }
                    else
                    {
                        Com_ReceiveSignalGroupHandle(rxIpduPtr, rxIpduStatePtr, rxIpduPtr->IpduPartitionId, internalId);
                        uint8 activeEnable = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
                        if (COM_RX_ACTIVE_EN == activeEnable)
                        {
                            ret = E_OK;
                        }
                    }
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUP_ID, errorId);
    }
#endif
    COM_NOUSED(SignalGroupId);
    return ret;
}
/**
 * The service Com_SendSignalGroupArray copies the content of the provided
 * SignalGroupArrayPtr to the associated I-PDU. The provided data shall
 * correspond to the array representation of the signal group.
 *
 */
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
uint8 Com_SendSignalGroupArray(Com_SignalGroupIdType SignalGroupId, const uint8* SignalGroupArrayPtr)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if (NULL_PTR == SignalGroupArrayPtr)
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalGroupIdFromUserId(COM_SENDSIGNALGROUPARRAY_ID, SignalGroupId);
#else
            Com_SignalIdType internalId = SignalGroupId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_ConfigStd->TxSignalGroupNum)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (0u < COM_MAX_TXSIGNALGROUP_NUMBER)
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                Com_MainFunctionIdType txMainfunctionId =
                    Com_GetTxSigGrpMainfunctionId((Com_TxSignalGroupIdType)(internalId));
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    ret = Com_SendSignalGroupArrayHandle(internalId, SignalGroupArrayPtr);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SENDSIGNALGROUPARRAY_ID, errorId);
    }
#endif

    COM_NOUSED(SignalGroupId);
    COM_NOUSED(SignalGroupArrayPtr);
    return ret;
}
#endif
/**
 * The service Com_ReceiveSignalGroupArray copies the received signal group
 * array representation from the PDU to the location designated by SignalGroupArrayPtr.
 *
 */
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
uint8 Com_ReceiveSignalGroupArray(Com_SignalGroupIdType SignalGroupId, uint8* SignalGroupArrayPtr)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if (NULL_PTR == SignalGroupArrayPtr)
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalGroupIdFromUserId(COM_RECEIVESIGNALGROUPARRAY_ID, SignalGroupId);
#else
            Com_SignalIdType internalId = SignalGroupId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_ConfigStd->RxSignalGroupNum)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (0u < COM_MAX_RXSIGNALGROUP_NUMBER)
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                Com_MainFunctionIdType rxMainfunctionId =
                    Com_GetRxSigGrpMainfunctionId((Com_RxSignalGroupIdType)(internalId));
                if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[rxMainfunctionId])
#endif
                {
                    ret = Com_ReceiveSignalGroupArrayHandle(internalId, SignalGroupArrayPtr);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RECEIVESIGNALGROUPARRAY_ID, errorId);
    }
#endif
    COM_NOUSED(SignalGroupId);
    COM_NOUSED(SignalGroupArrayPtr);
    return ret;
}
#endif
/**
 * The service Com_InvalidateSignal invalidates the signal with the given SignalId
 * by setting its value to its configured ComSignalDataInvalidValue.
 *
 */
uint8 Com_InvalidateSignal(Com_SignalIdType SignalId)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalAndGroupSignalIdFromUserId(COM_INVALIDATESIGNAL_ID, SignalId);
#else
            Com_SignalIdType internalId = SignalId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_CfgTxSignalNum)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (COM_MAX_TXSIGNAL_NUMBER > 0u) || (COM_MAX_TXGROUPSIGNAL_NUMBER > 0u)
/* Multiple partition check. */
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                Com_MainFunctionIdType txMainfunctionId = Com_GetTxSigMainfunctionId((Com_TxSignalIdType)(internalId));
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    ret = Com_InvalidateSignalHandle(internalId);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INVALIDATESIGNAL_ID, errorId);
    }
#endif
    COM_NOUSED(SignalId);
    return ret;
}
/**
 * The service Com_InvalidateSignalGroup invalidates all group signals of the signal
 * group with the given SignalGroupId by setting their values to their configured ComSignalDataInvalidValues.
 *
 */
uint8 Com_InvalidateSignalGroup(Com_SignalGroupIdType SignalGroupId)
{
    uint8 ret = COM_SERVICE_NOT_AVAILABLE; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            Com_SignalIdType internalId = Com_GetSignalGroupIdFromUserId(COM_INVALIDATESIGNALGROUP_ID, SignalGroupId);
#else
            Com_SignalIdType internalId = SignalGroupId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_ConfigStd->TxSignalGroupNum)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (COM_MAX_TXSIGNALGROUP_NUMBER > 0u)
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                Com_MainFunctionIdType txMainfunctionId =
                    Com_GetTxSigGrpMainfunctionId((Com_TxSignalGroupIdType)(internalId));
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    ret = Com_InvalidateSignalGroupHandle(internalId);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_INVALIDATESIGNALGROUP_ID, errorId);
    }
#endif
    COM_NOUSED(SignalGroupId);
    return ret;
}
/**
 * By a call to Com_TriggerIPDUSend the I-PDU with the given ID is triggered for transmission.
 *
 */
Std_ReturnType Com_TriggerIPDUSend(PduIdType PduId)
{
    Std_ReturnType returnValue = E_NOT_OK; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_TRIGGERIPDUSEND_ID, PduId);
#else
            PduIdType internalId = PduId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if ((internalId >= (Com_CfgRxPduNum + Com_CfgTxPduNum)) || (internalId < Com_CfgRxPduNum))
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (0u < COM_MAX_TXIPDU_NUMBER)
                PduIdType              txIpduId         = internalId - Com_CfgRxPduNum;
                const Com_TxIPduType*  txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
                Com_MainFunctionIdType txMainfunctionId = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    PduIdType ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
                    Com_TxIPduRunTimeStateType* txIpduStatePtr =
                        &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
                    returnValue = Com_TriggerIPDUSendHandle(
                        txIpduPtr,
                        txIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
                        internalId,
#endif
                        txMainfunctionId);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSEND_ID, errorId);
    }
#endif
    COM_NOUSED(PduId);
    return returnValue;
}
#if (STD_ON == COM_METADATA_SUPPORT)
/**
 * By a call to Com_TriggerIPDUSendWithMetaData the AUTOSAR COM module
 * updates its internal metadata for the I-PDU with the given ID by copying the
 * metadata from the given position and with respect to the globally configured metadata
 * length of the I-PDU. Then the I-PDU is triggered for transmission.
 *
 */
Std_ReturnType Com_TriggerIPDUSendWithMetaData(PduIdType PduId, const uint8* MetaData)
{
    Std_ReturnType returnValue = E_NOT_OK; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_TRIGGERIPDUSENDWITHMETADATA_ID, PduId);
#else
            PduIdType internalId = PduId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if ((internalId >= (Com_CfgRxPduNum + Com_CfgTxPduNum)) || (internalId < Com_CfgRxPduNum))
            {
                errorId = COM_E_PARAM;
            }
            else if ((0u == Com_CfgTxIPduPtr[internalId - Com_CfgRxPduNum].MetaDataLength) || (NULL_PTR == MetaData))
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (0u < COM_MAX_TXIPDU_NUMBER)
                PduIdType              txIpduId         = internalId - Com_CfgRxPduNum;
                const Com_TxIPduType*  txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
                Com_MainFunctionIdType txMainfunctionId = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    PduIdType ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
                    Com_TxIPduRunTimeStateType* txIpduStatePtr =
                        &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
                    /*Trigger IPDU(with metadata) send handle*/
                    returnValue = Com_TriggerIPDUSendWithMetaDataHandle(
                        txIpduPtr,
                        txIpduStatePtr,
#if (STD_ON == COM_TX_IPDU_CALLOUT_ENABLE)
                        internalId,
#endif
                        txMainfunctionId,
                        MetaData);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERIPDUSENDWITHMETADATA_ID, errorId);
    }
#endif
    COM_NOUSED(PduId);
    COM_NOUSED(MetaData);
    return returnValue;
}
#endif
/**
 * The service Com_SwitchIpduTxMode sets the transmission mode of the I-PDU referenced by PduId
 * to Mode. In case the transmission mode changes, the new mode shall immediately be effective. In case the
 * requested transmission mode was already active for this I-PDU, the call will have no effect.
 *
 */
void Com_SwitchIpduTxMode(PduIdType PduId, boolean Mode)
{
#if (STD_ON == COM_TMS_ENABLE)
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != osApplicationId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_SWITCHIPDUTXMODE_ID, PduId);
#else
            PduIdType internalId = PduId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if ((internalId >= (Com_CfgRxPduNum + Com_CfgTxPduNum)) || (internalId < Com_CfgRxPduNum))
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
                PduIdType              txIpduId         = internalId - Com_CfgRxPduNum;
                const Com_TxIPduType*  txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
                Com_MainFunctionIdType txMainfunctionId = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    PduIdType ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
                    Com_TxIPduRunTimeStateType* txIpduStatePtr =
                        &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
                    Com_SwitchIpduTxModeHandle(txIpduPtr, txIpduStatePtr, Mode);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_SWITCHIPDUTXMODE_ID, errorId);
    }
#endif
#endif
    COM_NOUSED(PduId);
    COM_NOUSED(Mode);
    return;
}
/**
 * This function performs the processing of the AUTOSAR COM module's receive processing that
 * are not directly handled within the COM's functions invoked by the PDU-R, for example *
 *
 */
void Com_MainFunctionRx(Com_MainFunctionIdType RxMainFunctionId)
{
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != partitionId)
#endif
    {
        if ((COM_INIT == Com_Status[partitionId]) && (RxMainFunctionId < Com_ConfigStd->MainFunctionRxNum))
        {
#if (0u < COM_MAX_RXIPDU_NUMBER)
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
            if (osApplicationId == Com_CfgMainFuncRxPartitionRangePtr[RxMainFunctionId])
#endif
            {
                Com_MainFunctionRxHandle(RxMainFunctionId);
            }
#endif
        }
    }
}
/**
 * This function performs the processing of the AUTOSAR COM module's transmission activities
 * that are not directly handled within the COM's function invoked by the RTE, for example Com_SendSignal.
 *
 */
void Com_MainFunctionTx(Com_MainFunctionIdType TxMainFunctionId)
{
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplicationId = GetApplicationID();
    partitionId            = Com_PartitonIdFromList(osApplicationId);
    if (COM_UNUSED_UINT16 != partitionId)
#endif
    {
        if ((COM_INIT == Com_Status[partitionId]) && (TxMainFunctionId < Com_ConfigStd->MainFunctionTxNum))
        {
#if (0u < COM_MAX_TXIPDU_NUMBER)
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
            if (osApplicationId == Com_CfgMainFuncTxPartitionRangePtr[TxMainFunctionId])
#endif
            {
                Com_MainFunctionTxHandle(TxMainFunctionId);
            }
#endif
        }
    }
}

/**
 * Calls the signal gateway part of the AUTOSAR COM module to forward received signals to be routed.
 *
 */
void Com_MainFunctionRouteSignals(Com_MainFunctionIdType RouteSignalsMainFunctionId)
{
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    partitionId = Com_PartitonIdFromList(GetApplicationID());
    if (COM_UNUSED_UINT16 != partitionId)
#endif
    {
        if ((COM_INIT == Com_Status[partitionId])
            && (RouteSignalsMainFunctionId < Com_ConfigStd->MainFunctionRouteSignalsNum))
        {
#if COM_MAX_GWMAPPING_NUMBER > 0u
            Com_MainFunctionRouteSignalsHandle(RouteSignalsMainFunctionId);
#endif
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/**
 * Within this API, the upper layer module (called module) shall check whether the available data fits into the buffer
 * size reported by PduInfoPtr->SduLength.If it fits, it shall copy its data into the buffer provided by
 * PduInfoPtr->SduDataPtr and update the length of the actual copied data in PduInfoPtr->SduLength.
 * If not, it returns E_NOT_OK without changing PduInfoPtr.
 *
 */
#define COM_START_SEC_CODE_FAST
#include "Com_MemMap.h"
Std_ReturnType Com_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
    Std_ReturnType returnValue = E_NOT_OK; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */

#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplication = GetApplicationID();
    partitionId          = Com_PartitonIdFromList(osApplication);
    if (COM_UNUSED_UINT16 != osApplication)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_TRIGGERTRANSMIT_ID, TxPduId);
#else
            PduIdType internalId = TxPduId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if ((internalId >= (Com_CfgRxPduNum + Com_CfgTxPduNum)) || (internalId < Com_CfgRxPduNum))
            {
                errorId = COM_E_PARAM;
            }
            else if (
                (0u < Com_CfgTxIPduPtr[internalId - Com_CfgRxPduNum].MetaDataLength)
                && (NULL_PTR == PduInfoPtr->MetaDataPtr))
            {
                errorId = COM_E_PARAM_POINTER;
            }
            else
#endif
            {
#if (0u < COM_MAX_TXIPDU_NUMBER)
                PduIdType              txIpduId         = internalId - Com_CfgRxPduNum;
                const Com_TxIPduType*  txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
                Com_MainFunctionIdType txMainfunctionId = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (COM_MULTIPLE_PARTITION_USED == STD_ON)
                if (osApplication == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    PduIdType ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
                    Com_TxIPduRunTimeStateType* txIpduStatePtr =
                        &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
                    returnValue =
                        Com_TriggerTransmitHandle(txIpduStatePtr, txIpduPtr, internalId, PduInfoPtr, txMainfunctionId);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (COM_MULTIPLE_PARTITION_USED == STD_ON)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TRIGGERTRANSMIT_ID, errorId);
    }
#endif
    COM_NOUSED(TxPduId);
    COM_NOUSED(PduInfoPtr);
    return returnValue;
}

/**
 * Indication of a received I-PDU from a lower layer communication interface module.
 *
 */
void Com_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplication = GetApplicationID();
    partitionId          = Com_PartitonIdFromList(osApplication);
    if (COM_UNUSED_UINT16 != osApplication)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == PduInfoPtr->SduDataPtr))
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_RXINDICATION_ID, RxPduId);
#else
            PduIdType internalId = RxPduId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_CfgRxPduNum)
            {
                errorId = COM_E_PARAM;
            }
            else if (COM_PDU_NORMAL != Com_CfgRxIPduPtr[internalId].ComIPduType)
            {
                errorId = COM_E_PARAM;
            }
            else if ((0u < Com_CfgRxIPduPtr[internalId].MetaDataLength) && (NULL_PTR == PduInfoPtr->MetaDataPtr))
            {
                errorId = COM_E_PARAM_POINTER;
            }
            else
#endif
            {
#if (0u < COM_MAX_RXIPDU_NUMBER)
                const Com_RxIPduType*  rxIpduPtr        = &Com_CfgRxIPduPtr[internalId];
                Com_MainFunctionIdType rxMainfunctionId = rxIpduPtr->RxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (COM_MULTIPLE_PARTITION_USED == STD_ON)
                if (osApplication == Com_CfgMainFuncRxPartitionRangePtr[rxMainfunctionId])
#endif
                {
                    PduIdType ipduIdPerMainfunction = internalId - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
                    Com_RxIPduRunTimeStateType* rxIpduStatePtr =
                        &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
                    Com_RxIndicationHandle(
                        rxIpduStatePtr,
                        rxIpduPtr,
#if (STD_ON == COM_RX_IPDU_CALLOUT_ENABLE)
                        internalId,
#endif
                        PduInfoPtr,
                        rxMainfunctionId);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (COM_MULTIPLE_PARTITION_USED == STD_ON)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_RXINDICATION_ID, errorId);
    }
#endif
#endif
    COM_NOUSED(RxPduId);
    COM_NOUSED(PduInfoPtr);
    return;
}

/**
 * Called after an I-PDU has been received via the TP API, the result indicates whether the transmission was
 * successful or not.
 *
 */
void Com_TpRxIndication(PduIdType id, Std_ReturnType result)
{
#if (STD_ON == COM_RXTPPDU_SUPPORT)
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplication = GetApplicationID();
    partitionId          = Com_PartitonIdFromList(osApplication);
    if (COM_UNUSED_UINT16 != osApplication)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_TPRXINDICATION_ID, id);
#else
            PduIdType internalId = id;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_CfgRxPduNum)
            {
                errorId = COM_E_PARAM;
            }
            else if (COM_PDU_TP != Com_CfgRxIPduPtr[internalId].ComIPduType)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
                const Com_RxIPduType*  rxIpduPtr        = &Com_CfgRxIPduPtr[internalId];
                Com_MainFunctionIdType rxMainfunctionId = rxIpduPtr->RxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (COM_MULTIPLE_PARTITION_USED == STD_ON)
                if (osApplication == Com_CfgMainFuncRxPartitionRangePtr[rxMainfunctionId])
#endif
                {
                    uint16 ipduIdPerMainfunction = internalId - Com_GetStartOfMainFunctionRx(rxMainfunctionId);
                    Com_RxIPduRunTimeStateType* rxIpduStatePtr =
                        &Com_RxIPduRunTimeState[rxMainfunctionId][ipduIdPerMainfunction];
                    Com_TpRxIndicationHandle(internalId, rxIpduStatePtr, rxIpduPtr, rxMainfunctionId, result);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (COM_MULTIPLE_PARTITION_USED == STD_ON)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TPRXINDICATION_ID, errorId);
    }
#endif
#endif
    COM_NOUSED(id);
    COM_NOUSED(result);
    return;
}

/**
 * The lower layer communication interface module confirms the transmission of an IPDU.
 *
 */
void Com_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplication = GetApplicationID();
    partitionId          = Com_PartitonIdFromList(osApplication);
    if (COM_UNUSED_UINT16 != partitionId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_TXCONFIRMATION_ID, TxPduId);
#else
                        PduIdType internalId = TxPduId;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if ((internalId >= (Com_CfgRxPduNum + Com_CfgTxPduNum)) || (internalId < Com_CfgRxPduNum))
            {
                errorId = COM_E_PARAM;
            }
            else if (COM_PDU_NORMAL != Com_CfgTxIPduPtr[internalId - Com_CfgRxPduNum].ComIPduType)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
#if (0u < COM_MAX_TXIPDU_NUMBER)
                PduIdType              txIpduId         = internalId - Com_CfgRxPduNum;
                const Com_TxIPduType*  txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
                Com_MainFunctionIdType txMainfunctionId = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplication == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    PduIdType ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
                    Com_TxIPduRunTimeStateType* txIpduStatePtr =
                        &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
                    if (E_OK == result)
                    {
                        Com_TxConfirmationHandle(txIpduStatePtr, txIpduPtr, txMainfunctionId);
                    }
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TXCONFIRMATION_ID, errorId);
    }
#endif
    COM_NOUSED(TxPduId);
    return;
}

/**
 * This function is called after the I-PDU has been transmitted on its network, the
 * result indicates whether the transmission was successful or not.
 *
 */
void Com_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
#if (STD_ON == COM_TXTPPDU_SUPPORT)
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplication = GetApplicationID();
    partitionId          = Com_PartitonIdFromList(osApplication);
    if (COM_UNUSED_UINT16 != osApplication)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_TPTXCONFIRMATION_ID, id);
#else
            PduIdType internalId = id;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if ((internalId >= (Com_CfgRxPduNum + Com_CfgTxPduNum)) || (internalId < Com_CfgRxPduNum))
            {
                errorId = COM_E_PARAM;
            }
            else if (COM_PDU_TP != Com_CfgTxIPduPtr[internalId - Com_CfgRxPduNum].ComIPduType)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
                PduIdType              txIpduId         = internalId - Com_CfgRxPduNum;
                const Com_TxIPduType*  txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
                Com_MainFunctionIdType txMainfunctionId = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplication == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    PduIdType ipduIdPerMainfunction = txIpduId - Com_GetStartOfMainFunctionTx(txMainfunctionId);
                    Com_TxIPduRunTimeStateType* txIpduStatePtr =
                        &Com_TxIPduRunTimeState[txMainfunctionId][ipduIdPerMainfunction];
                    Com_TpTxConfirmationHandle(txIpduStatePtr, txIpduPtr, txMainfunctionId, result);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_TPTXCONFIRMATION_ID, errorId);
    }
#endif
#endif
    COM_NOUSED(id);
    COM_NOUSED(result);
    return;
}

/**
 * This function is called at the start of receiving an N-SDU. The N-SDU might be
 * fragmented into multiple N-PDUs (FF with one or more following CFs) or might consist of a single
 * N-PDU (SF).
 *
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
BufReq_ReturnType
    Com_StartOfReception(PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr)
/* PRQA S 3673 -- */
{
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */
#if (STD_ON == COM_RXTPPDU_SUPPORT)
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplication = GetApplicationID();
    partitionId          = Com_PartitonIdFromList(osApplication);
    if (COM_UNUSED_UINT16 != osApplication)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_STARTOFRECEPTION_ID, id);
#else
            PduIdType internalId = id;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if ((internalId >= Com_CfgRxPduNum) || (COM_PDU_TP != Com_CfgRxIPduPtr[internalId].ComIPduType))
            {
                errorId = COM_E_PARAM;
            }
            else if (
                ((0u < Com_CfgRxIPduPtr[internalId].MetaDataLength)
                 && ((NULL_PTR == info) || (NULL_PTR == info->MetaDataPtr)))
                || (((NULL_PTR != info) && (0u < info->SduLength)) && (NULL_PTR == info->SduDataPtr))
                || (NULL_PTR == bufferSizePtr))
            {
                errorId = COM_E_PARAM_POINTER;
            }
            else
#endif
            {
                const Com_RxIPduType*  rxIpduPtr        = &Com_CfgRxIPduPtr[internalId];
                Com_MainFunctionIdType rxMainfunctionId = rxIpduPtr->RxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplication == Com_CfgMainFuncRxPartitionRangePtr[rxMainfunctionId])
#endif
                {
                    bufReq = Com_StartOfReceptionHandle(internalId, rxMainfunctionId, info, TpSduLength, bufferSizePtr);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_STARTOFRECEPTION_ID, errorId);
    }
#endif
#endif
    COM_NOUSED(id);
    COM_NOUSED(info);
    COM_NOUSED(TpSduLength);
    COM_NOUSED(bufferSizePtr);
    return bufReq;
}

/**
 * This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper
 * layer. Each call to this function provides the next part of the I-PDU data. The size of the
 * remaining data is written to the position indicated by bufferSizePtr.
 *
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
BufReq_ReturnType Com_CopyRxData(PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr)
/* PRQA S 3673 -- */
{
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */
#if (STD_ON == COM_RXTPPDU_SUPPORT)
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplication = GetApplicationID();
    partitionId          = Com_PartitonIdFromList(osApplication);
    if (COM_UNUSED_UINT16 != osApplication)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if ((NULL_PTR == info) || (NULL_PTR == bufferSizePtr))
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_COPYRXDATA_ID, id);
#else
            PduIdType internalId = id;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if (internalId >= Com_CfgRxPduNum)
            {
                errorId = COM_E_PARAM;
            }
            else if (COM_PDU_TP != Com_CfgRxIPduPtr[internalId].ComIPduType)
            {
                errorId = COM_E_PARAM;
            }
            else if ((0u < Com_CfgRxIPduPtr[internalId].MetaDataLength) && (NULL_PTR == info->MetaDataPtr))
            {
                errorId = COM_E_PARAM_POINTER;
            }
            else
#endif
            {
                const Com_RxIPduType*  rxIpduPtr        = &Com_CfgRxIPduPtr[internalId];
                Com_MainFunctionIdType rxMainfunctionId = rxIpduPtr->RxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplication == Com_CfgMainFuncRxPartitionRangePtr[rxMainfunctionId])
#endif
                {
                    bufReq = Com_CopyRxDataHandle(internalId, rxMainfunctionId, info, bufferSizePtr);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYRXDATA_ID, errorId);
    }
#endif
#endif
    COM_NOUSED(id);
    COM_NOUSED(info);
    COM_NOUSED(bufferSizePtr);
    return bufReq;
}

/**
 * This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 * Each call to this function provides the next part of the I-PDU data unless
 * retry->TpDataState is TP_DATARETRY. In this case the function restarts to copy the data
 * beginning at the offset from the current position indicated by retry->TxTpDataCnt. The
 * size of the remaining data is written to the position indicated by availableDataPtr.
 *
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
BufReq_ReturnType
    Com_CopyTxData(PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr)
{
    /* PRQA S 3673 -- */
    BufReq_ReturnType bufReq = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* VL_Com_InitReturnVar */
#if (STD_ON == COM_TXTPPDU_SUPPORT)
#if (STD_ON == COM_DEV_ERROR_DETECT)
    uint8  errorId     = COM_E_NONE;
    uint16 partitionId = 0u;
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    uint16 osApplication = GetApplicationID();
    partitionId          = Com_PartitonIdFromList(osApplication);
    if (COM_UNUSED_UINT16 != partitionId)
#endif
#endif
    {
#if (STD_ON == COM_DEV_ERROR_DETECT)
        if (COM_INIT != Com_Status[partitionId])
        {
            errorId = COM_E_UNINIT;
        }
        else if ((NULL_PTR == info) || (NULL_PTR == availableDataPtr))
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else if ((0u < info->SduLength) && (NULL_PTR == info->SduDataPtr))
        {
            errorId = COM_E_PARAM_POINTER;
        }
        else
#endif
        {
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
            PduIdType internalId = Com_GetPduIdFromUserId(COM_COPYTXDATA_ID, id);
#else
            PduIdType internalId = id;
#endif
#if (STD_ON == COM_DEV_ERROR_DETECT)
            if ((internalId >= (Com_CfgRxPduNum + Com_CfgTxPduNum)) || (internalId < Com_CfgRxPduNum))
            {
                errorId = COM_E_PARAM;
            }
            else if (COM_PDU_TP != Com_CfgTxIPduPtr[internalId - Com_CfgRxPduNum].ComIPduType)
            {
                errorId = COM_E_PARAM;
            }
            else
#endif
            {
                PduIdType              txIpduId         = internalId - Com_CfgRxPduNum;
                const Com_TxIPduType*  txIpduPtr        = &Com_CfgTxIPduPtr[txIpduId];
                Com_MainFunctionIdType txMainfunctionId = txIpduPtr->TxMainfunctionId;
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                if (osApplication == Com_CfgMainFuncTxPartitionRangePtr[txMainfunctionId])
#endif
                {
                    bufReq = Com_CopyTxDataHandle(txIpduId, txMainfunctionId, info, retry, availableDataPtr);
                }
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
                else
                {
                    errorId = COM_E_PARTITION;
                }
#endif
            }
        }
    }
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (errorId != COM_E_NONE)
    {
        (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_COPYTXDATA_ID, errorId);
    }
#endif
#endif
    COM_NOUSED(id);
    COM_NOUSED(info);
    COM_NOUSED(retry);
    COM_NOUSED(availableDataPtr);
    return bufReq;
}
/* PRQA S 1503,1532 -- */
#define COM_STOP_SEC_CODE_FAST
#include "Com_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/**
 * @brief Init the RxPdu Runtime Manager and Buffer
 */
COM_LOCAL void Com_PduBufferRxInit(Com_MainFunctionIdType RxMainfunctionId)
{
#if (0u < COM_MAX_RXIPDU_NUMBER)
    Com_RxIPduRunTimeStateType*       rxIpduStatePtr     = Com_RxIPduRunTimeState[RxMainfunctionId];
    const Com_RxIPduRunTimeStateType* rxIpduInitStatePtr = Com_ConfigStd->RxIpduInitStatePtr[RxMainfunctionId];
    /* Rx IPdu's RunTimeState init */
    if (NULL_PTR != rxIpduInitStatePtr)
    {
        PduIdType rxIpduNm =
            Com_GetEndOfMainFunctionRx(RxMainfunctionId) - Com_GetStartOfMainFunctionRx(RxMainfunctionId);
        (void)IStdLib_MemCpy(
            rxIpduStatePtr,
            rxIpduInitStatePtr,
            (uint32)(rxIpduNm * (sizeof(Com_RxIPduRunTimeStateType))));
    }
#endif
#if (0u < COM_MAX_RXIPDUBUFF_SIZE)
    uint8*       rxIpduRuntimeBuff  = Com_RxIPduRuntimeBuff[RxMainfunctionId];
    const uint8* rxIpduInitValuePtr = Com_ConfigStd->RxIpduInitValuePtr[RxMainfunctionId];
    if (NULL_PTR != rxIpduInitValuePtr)
    {
        const Com_RxIpduBufIdType rxIpduBuffSize = Com_ConfigStd->RxIpduBufferSizePerPartition[RxMainfunctionId];
        /* RxIPdu Init value, based on signal or group signal init value */
        (void)IStdLib_MemCpy(rxIpduRuntimeBuff, rxIpduInitValuePtr, rxIpduBuffSize);
    }
#endif
    return;
}

/**
 * @brief Init the TxPdu Runtime Manager and Buffer
 */
COM_LOCAL void Com_PduBufferTxInit(Com_MainFunctionIdType TxMainfunctionId)
{
#if (0u < COM_MAX_TXIPDU_NUMBER)
    Com_TxIPduRunTimeStateType*       txIpduRuntimePtr   = Com_TxIPduRunTimeState[TxMainfunctionId];
    const Com_TxIPduRunTimeStateType* txIpduInitStatePtr = Com_ConfigStd->TxIpduInitStatePtr[TxMainfunctionId];
    if (NULL_PTR != txIpduInitStatePtr)
    { /* Rx IPdu's RunTimeState init */
        PduIdType txIpduNum =
            Com_GetEndOfMainFunctionTx(TxMainfunctionId) - Com_GetStartOfMainFunctionTx(TxMainfunctionId);
        (void)IStdLib_MemCpy(
            txIpduRuntimePtr,
            txIpduInitStatePtr,
            (uint32)(txIpduNum * (sizeof(Com_TxIPduRunTimeStateType))));
    }
#endif
#if (0u < COM_MAX_TXIPDUBUFF_SIZE)
    uint8*       txIpduRuntimeBuff  = Com_TxIPduRuntimeBuff[TxMainfunctionId];
    const uint8* txIpduInitValuePtr = Com_ConfigStd->TxIpduInitValuePtr[TxMainfunctionId];
    if (NULL_PTR != txIpduInitValuePtr)
    {
        const Com_TxIpduBufIdType txIPduBufSize = Com_ConfigStd->TxIpduBufferSizePerPartition[TxMainfunctionId];
        /* RxIPdu Init value, based on signal or group signal init value */
        (void)IStdLib_MemCpy(txIpduRuntimeBuff, txIpduInitValuePtr, txIPduBufSize);
    }
#endif
    return;
}

/**
 * @brief Init the Runtime RxSignal/RxGroupSignal Buffer
 *
 */
/* PRQA S 6080 ++ */ /* VL_MTR_Com_STPTH */
COM_LOCAL void Com_SignalBufferInit(uint16 PartitionId)
/* PRQA S 6080 -- */
{
#if (0u < COM_MAX_SIGNAL_BOOLBUFF_SIZE)
    const boolean* signalBoolInitValuePtr = Com_CfgSignalInitValuePtr->SignalBoolInitValue
                                                ? Com_CfgSignalInitValuePtr->SignalBoolInitValue[PartitionId]
                                                : NULL_PTR;
    if (NULL_PTR != signalBoolInitValuePtr)
    {
        boolean*                         signalBoolRuntimeBuffPtr = Com_SignalBoolRuntimeBuff[PartitionId];
        const Com_InitValueSignalIdType* signalBoolInitValueSizePtr =
            &Com_ConfigStd->SignalInitValueSizePtr->SignalBoolInitValueSize[PartitionId];
        (void)IStdLib_MemCpy(signalBoolRuntimeBuffPtr, signalBoolInitValuePtr, signalBoolInitValueSizePtr->bufferSize);
#if (0u < COM_MAX_RXGROUPSIGNAL_BOOLBUFF_SIZE)
        /* Bool signal Runtime value, based on bool signals or dest description(source
         * description signals are not included) signals init value */
        uint16 rxGrpSigBoolSizeEndId = signalBoolInitValueSizePtr->rxGroupSignalEndId;
        if (rxGrpSigBoolSizeEndId != COM_UNUSED_UINT16)
        {
            boolean* rxGroupSignalBoolRuntimeBuffPtr = Com_RxGroupSignalBoolShadowBuff[PartitionId];
            (void)IStdLib_MemCpy(rxGroupSignalBoolRuntimeBuffPtr, signalBoolInitValuePtr, rxGrpSigBoolSizeEndId);
        }
#endif
    }
#endif

#if (0u < COM_MAX_SIGNAL_8BITBUFF_SIZE)
    const uint8* signal8BitInitValuePtr = Com_CfgSignalInitValuePtr->Signal8BitInitValue
                                              ? Com_CfgSignalInitValuePtr->Signal8BitInitValue[PartitionId]
                                              : NULL_PTR;
    if (NULL_PTR != signal8BitInitValuePtr)
    {
        uint8*                           signal8BitRuntimeBuffPtr = Com_Signal8BitRuntimeBuff[PartitionId];
        const Com_InitValueSignalIdType* signal8BitInitValueSizePtr =
            &Com_ConfigStd->SignalInitValueSizePtr->Signal8BitInitValueSize[PartitionId];
        (void)IStdLib_MemCpy(signal8BitRuntimeBuffPtr, signal8BitInitValuePtr, signal8BitInitValueSizePtr->bufferSize);
        /* Bool signal Runtime value, based on bool signals or dest description(source
         * description signals are not included) signals init value */
#if (0u < COM_MAX_RXGROUPSIGNAL_8BITBUFF_SIZE)
        uint16 rxGrpSig8BitSizeEndId = signal8BitInitValueSizePtr->rxGroupSignalEndId;
        if (rxGrpSig8BitSizeEndId != COM_UNUSED_UINT16)
        {
            uint8* rxGroupSignal8BitRuntimeBuffPtr = Com_RxGroupSignal8BitShadowBuff[PartitionId];
            (void)IStdLib_MemCpy(rxGroupSignal8BitRuntimeBuffPtr, signal8BitInitValuePtr, rxGrpSig8BitSizeEndId);
        }
#endif
    }
#endif

#if (0u < COM_MAX_SIGNAL_16BITBUFF_SIZE)
    const uint16* signalBit16InitValuePtr = Com_CfgSignalInitValuePtr->SignalBit16InitValue
                                                ? Com_CfgSignalInitValuePtr->SignalBit16InitValue[PartitionId]
                                                : NULL_PTR;
    if (NULL_PTR != signalBit16InitValuePtr)
    {
        const Com_InitValueSignalIdType* signalBit16InitValueSizePtr =
            &Com_ConfigStd->SignalInitValueSizePtr->SignalBit16InitValueSize[PartitionId];
        uint16* signalBit16RuntimeBuffPtr = Com_SignalBit16RuntimeBuff[PartitionId];
        (void)
            IStdLib_MemCpy(signalBit16RuntimeBuffPtr, signalBit16InitValuePtr, signalBit16InitValueSizePtr->bufferSize);
        /* Bool signal Runtime value, based on bool signals or dest description(source
         * description signals are not included) signals init value */
#if (0u < COM_MAX_RXGROUPSIGNAL_16BITBUFF_SIZE)
        uint16* rxGroupSignalBit16RuntimeBuffPtr = Com_RxGroupSignalBit16ShadowBuff[PartitionId];
        uint16  rxGrpSig16BitSizeEndId           = signalBit16InitValueSizePtr->rxGroupSignalEndId;
        if (rxGrpSig16BitSizeEndId != COM_UNUSED_UINT16)
        {
            (void)IStdLib_MemCpy(rxGroupSignalBit16RuntimeBuffPtr, signalBit16InitValuePtr, rxGrpSig16BitSizeEndId);
        }
#endif
    }
#endif

#if (0u < COM_MAX_SIGNAL_32BITBUFF_SIZE)
    const uint32* signal32BitInitValuePtr = Com_CfgSignalInitValuePtr->Signal32BitInitValue
                                                ? Com_CfgSignalInitValuePtr->Signal32BitInitValue[PartitionId]
                                                : NULL_PTR;
    if (NULL_PTR != signal32BitInitValuePtr)
    {
        const Com_InitValueSignalIdType* signal32BitInitValueSizePtr =
            &Com_ConfigStd->SignalInitValueSizePtr->Signal32BitInitValueSize[PartitionId];
        uint32* signal32BitRuntimeBuffPtr = Com_Signal32BitRuntimeBuff[PartitionId];
        (void)
            IStdLib_MemCpy(signal32BitRuntimeBuffPtr, signal32BitInitValuePtr, signal32BitInitValueSizePtr->bufferSize);
        /* Bool signal Runtime value, based on bool signals or dest description(source
         * description signals are not included) signals init value */
#if (0u < COM_MAX_RXGROUPSIGNAL_32BITBUFF_SIZE)
        uint16 rxGrpSig32BitSizeEndId = signal32BitInitValueSizePtr->rxGroupSignalEndId;
        if (rxGrpSig32BitSizeEndId != COM_UNUSED_UINT16)
        {
            uint32* rxGroupSignal32BitRuntimeBuffPtr = Com_RxGroupSignal32BitShadowBuff[PartitionId];
            (void)IStdLib_MemCpy(rxGroupSignal32BitRuntimeBuffPtr, signal32BitInitValuePtr, rxGrpSig32BitSizeEndId);
        }
#endif
    }
#endif

#if (0u < COM_MAX_SIGNAL_64BITBUFF_SIZE)
    const uint64* signal64BitInitValuePtr = Com_CfgSignalInitValuePtr->Signal64BitInitValue
                                                ? Com_CfgSignalInitValuePtr->Signal64BitInitValue[PartitionId]
                                                : NULL_PTR;
    if (NULL_PTR != signal64BitInitValuePtr)
    {
        const Com_InitValueSignalIdType* signal64BitInitValueSizePtr =
            &Com_ConfigStd->SignalInitValueSizePtr->Signal64BitInitValueSize[PartitionId];
        uint64* signal64BitRuntimeBuffPtr = Com_Signal64BitRuntimeBuff[PartitionId];
        (void)
            IStdLib_MemCpy(signal64BitRuntimeBuffPtr, signal64BitInitValuePtr, signal64BitInitValueSizePtr->bufferSize);
        /* Bool signal Runtime value, based on bool signals or dest description(source
         * description signals are not included) signals init value */
#if (0u < COM_MAX_RXGROUPSIGNAL_64BITBUFF_SIZE)
        uint16 rxGrpSig64BitSizeEndId = signal64BitInitValueSizePtr->rxGroupSignalEndId;
        if (rxGrpSig64BitSizeEndId != COM_UNUSED_UINT16)
        {
            uint64* rxGroupSignal64BitRuntimeBuffPtr = Com_RxGroupSignal64BitShadowBuff[PartitionId];
            (void)IStdLib_MemCpy(rxGroupSignal64BitRuntimeBuffPtr, signal64BitInitValuePtr, rxGrpSig64BitSizeEndId);
        }
#endif
    }
#endif

    return;
}

/**
 * @brief If TxPdu TxConfirmation handle
 *
 */
#if (0u < COM_MAX_TXIPDU_NUMBER)
COM_LOCAL void Com_TxConfirmationHandle(
    Com_TxIPduRunTimeStateType* TxIpduStatePtr,
    const Com_TxIPduType*       TxIpduPtr,
    Com_MainFunctionIdType      TxMainfunctionId)
{
    uint8 activeEnable = TxIpduStatePtr->TxIpduRTStFlag & COM_TX_ACTIVE_EN;

    if (COM_TX_ACTIVE_EN == activeEnable)
    {
/*Reset Tx_IPduMDT*/
#if (STD_ON == COM_MDT_ENABLE)
        Com_ResetTxIpduMDT(TxIpduStatePtr, TxIpduPtr);
#endif

        /*repetitions of transmission */
#if (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NUMBER)   \
    || (0u < COM_MAX_TX_MODE_TRUE_DIRECT_NOREPETITION_NUMBER)                                 \
    || (0u < COM_MAX_TX_MODE_FALSE_DIRECT_NOREPETITION_NUMBER)                                \
    || ((0u < COM_MAX_TX_MODE_TRUE_MIXED_NUMBER) || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NUMBER) \
        || (0u < COM_MAX_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)                              \
        || (0u < COM_MAX_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER))
        if (TxIpduStatePtr->RptNum > 0u)
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
            Com_IPduSignalProcessingType ipduSignalProcessing = TxIpduPtr->ComIPduSignalProcessing;
            /*signal/signal group TxConfirmation handle is made immediate*/
            if (COM_IMMEDIATE == ipduSignalProcessing)
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
        /*reset Tx time out counter for just NONE mode of Tx Mode*/
        if ((TxIpduPtr->TxFalseModeMode == COM_TX_MODE_NULL) && (COM_TX_MODE_NONE == TxIpduPtr->TxTrueModeMode))
        {
            TxIpduStatePtr->DMCnt = TxIpduPtr->TxIpduDM;
        }
    }
    TxIpduStatePtr->TxIpduRTStFlag &= COM_TX_TRANSMITING_DIS;
    COM_NOUSED(TxMainfunctionId);
}
#endif

/**
 * @brief Rx Pdu DM timeout enable/disable handle
 *
 */
#if (COM_MAX_RXIPDUGROUP_NUMBER > 0u) && (0u < COM_MAX_RXIPDU_NUMBER)
/* PRQA S 6030 ++ */ /* VL_MTR_Com_STMIF */
COM_LOCAL void
    Com_RxPduDMCtrHandle(Com_MainFunctionIdType RxMainfunctionId, Com_IpduGroupIdType IpduGroupId, boolean IsEnable)
/* PRQA S 6030 -- */
{
    Com_RxIPduRunTimeStateType*  rxIpduStateMainfunctionPtr = Com_RxIPduRunTimeState[RxMainfunctionId];
    uint16                       startRxIpduId              = Com_GetStartOfMainFunctionRx(RxMainfunctionId);
    uint16                       endRxIpduId                = Com_GetEndOfMainFunctionRx(RxMainfunctionId);
    const Com_RxIpduGroupIdType* rxIpduGroupRefPtr          = Com_ConfigStd->IpduGroupRefPtr->RxIpduGroupRef;
    for (uint16 cirCnt = startRxIpduId; cirCnt < endRxIpduId; ++cirCnt)
    {
        const Com_RxIPduType* rxIpduPtr = &Com_CfgRxIPduPtr[cirCnt];
        for (Com_RxIpduGroupIdType counter = rxIpduPtr->IPduGroupsRefStartId; counter < rxIpduPtr->IPduGroupsRefEndId;
             ++counter)
        {
            if (IpduGroupId == rxIpduGroupRefPtr[counter])
            {
                uint16                      idpduIdPerMainfunction = cirCnt - startRxIpduId;
                Com_RxIPduRunTimeStateType* rxIpduStatePtr = &rxIpduStateMainfunctionPtr[idpduIdPerMainfunction];
                uint8                       DMEnable       = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_DM_EN;

                /*DM state changed from disabled to enabled*/
                if (IsEnable && (COM_RX_DM_EN != DMEnable))
                {
                    rxIpduStatePtr->RxIpduRTStFlag |= COM_RX_DM_EN;
/*set the reception deadline monitoring timer for the included signals and signal groups to the configured
 * ComFirstTimeout*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
                    Com_ResetRxPduTimeOut(rxIpduPtr, rxIpduStatePtr);
#endif
                }
                else if (!IsEnable && (COM_RX_DM_EN == DMEnable))
                {
                    rxIpduStatePtr->RxIpduRTStFlag &= COM_RX_DM_DIS;
/*set the reception deadline monitoring timer for the included signals and signal groups to 0*/
#if (COM_MAX_RXSIGNAL_NUMBER + COM_MAX_RXSIGNALGROUP_NUMBER > 0u)
                    Com_DisableRxPduTimeOut(rxIpduPtr, rxIpduStatePtr);
#endif
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
 * @brief  used to handle all Rx Ipdu state change or not
 */
#if (COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_RXIPDUGROUP_NUMBER > 0u)
/* PRQA S 6030 ++ */ /* VL_MTR_Com_STMIF */
COM_LOCAL void Com_RxIpduControl(
    Com_MainFunctionIdType RxMainfunctionId,
    Com_IpduGroupIdType    IpduGroupId,
    boolean                Initialize,
    boolean                IsEnable)
/* PRQA S 6030 -- */
{
    Com_RxIPduRunTimeStateType* rxIpduStateMainfunctionPtr = Com_RxIPduRunTimeState[RxMainfunctionId];
    uint16                      startRxIpduId              = Com_GetStartOfMainFunctionRx(RxMainfunctionId);
    uint16                      endRxIpduId                = Com_GetEndOfMainFunctionRx(RxMainfunctionId);
    for (uint16 cirCnt = startRxIpduId; cirCnt < endRxIpduId; ++cirCnt)
    {
        const Com_RxIPduType* rxIpduPtr = &Com_CfgRxIPduPtr[cirCnt];
        for (Com_RxIpduGroupIdType counter = rxIpduPtr->IPduGroupsRefStartId; counter < rxIpduPtr->IPduGroupsRefEndId;
             ++counter)
        {
            if (IpduGroupId == Com_ConfigStd->IpduGroupRefPtr->RxIpduGroupRef[counter])
            {
                uint16                      idpduIdPerMainfunction = cirCnt - startRxIpduId;
                Com_RxIPduRunTimeStateType* rxIpduStatePtr = &rxIpduStateMainfunctionPtr[idpduIdPerMainfunction];
                uint8                       activeEnable   = rxIpduStatePtr->RxIpduRTStFlag & COM_RX_ACTIVE_EN;
                /*the pdu restart*/
                if (IsEnable && (COM_RX_ACTIVE_EN != activeEnable))
                {
                    SchM_Enter_Com_MultiCore_Lock();
                    rxIpduStatePtr->RxIpduRTStFlag |= COM_RX_ACTIVE_EN;
                    SchM_Exit_Com_MultiCore_Lock();
/*reset rx i-pdu(all signal/signal group) DM Counter*/
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
                    Com_ResetRxPduTimeOut(rxIpduPtr, rxIpduStatePtr);
#endif
/*reset OCCURRENCE of filter with FilterAlgorithm ONE_EVERY_N*/
#if (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
                    Com_ResetRxOccurrenceOfPduFilter(rxIpduPtr);
#endif
                    if (Initialize)
                    {
                        /*init the rx ipdu buffer,all signal buffer(included in the ipdu,except source
                         * signal)*/
                        const Com_RxIPduRunTimeStateType* rxIpduInitStateMainfunctionPtr =
                            Com_ConfigStd->RxIpduInitStatePtr[RxMainfunctionId];
                        const Com_RxIPduRunTimeStateType* rxIpduInitStatePtr =
                            &rxIpduInitStateMainfunctionPtr[idpduIdPerMainfunction];
                        Com_ResetRxPduBufferAndSignalBuffer(
                            rxIpduPtr,
                            rxIpduStatePtr,
                            RxMainfunctionId,
                            rxIpduInitStatePtr);
                    }
                }
                else if (!IsEnable && (COM_RX_ACTIVE_EN == activeEnable))
                {
                    SchM_Enter_Com_MultiCore_Lock();
                    rxIpduStatePtr->RxIpduRTStFlag &= COM_RX_ACTIVE_DIS;
                    SchM_Exit_Com_MultiCore_Lock();
/*disable rx i-pdu(all signal/signal group) DM Counter*/
#if ((COM_MAX_RXSIGNAL_NUMBER + COM_MAX_RXSIGNALGROUP_NUMBER) > 0u)
                    Com_DisableRxPduTimeOut(rxIpduPtr, rxIpduStatePtr);
#endif
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
#if (COM_MAX_IPDUGROUP_NUMBER > 0u)
/**
 * @brief init the rx ipdu buffer,all signal buffer(included in the ipdu,except source signal)
 *
 */
COM_LOCAL void Com_ResetRxPduBufferAndSignalBuffer(
    const Com_RxIPduType*             RxIpduPtr,
    Com_RxIPduRunTimeStateType*       RxIpduStatePtr,
    Com_MainFunctionIdType            RxMainfunctionId,
    const Com_RxIPduRunTimeStateType* RxIpduInitStatePtr)
{
    uint8*         rxIpduRuntimeBuffPtr = Com_RxIPduRuntimeBuff[RxMainfunctionId];
    const uint8*   rxIpduInitValuePtr   = Com_ConfigStd->RxIpduInitValuePtr[RxMainfunctionId];
    uint16         signalInitValueId;
    Com_SignalType signalType;
    uint16         signalLength;
    /*init the rx ipdu buffer,all signal buffer(included in the ipdu,except source description signal)*/
#if (0u < COM_MAX_RXIPDUBUFF_SIZE)
    Com_RxIpduBufIdType pduBufferId   = RxIpduPtr->RxIPduBufIndex;
    uint16              initPduLength = RxIpduStatePtr->RxIpduLength;
    SchM_Enter_Com_Context();
    /*init rx pdu buffer*/
    (void)IStdLib_MemCpy(&rxIpduRuntimeBuffPtr[pduBufferId], &rxIpduInitValuePtr[pduBufferId], initPduLength);
    SchM_Exit_Com_Context();
#endif
    /*init signal buffer*/
#if (0u < COM_MAX_RXSIGNAL_NUMBER)
    Com_RxSignalIdType ipduSignalRefNumber, cnt;
    ipduSignalRefNumber = RxIpduPtr->IPduSignalsRefEndId;
    cnt                 = RxIpduPtr->IpduSignalRefStartId;
    for (; cnt < ipduSignalRefNumber; ++cnt)
    {
        const Com_RxSignalType* rxSignalPtr = &Com_CfgRxSignalPtr[cnt];
        signalInitValueId                   = rxSignalPtr->SignalInitValueId;
        signalType                          = rxSignalPtr->ComSignalType;
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        if (signalType < COM_UINT8_DYN)
#endif
        {
            signalLength = 0u;
        }
#if (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE)
        else
        {
            signalLength = (COM_UINT8_DYN == signalType) ? (rxSignalPtr->SignalDataInitValueLength)
                                                         : (rxSignalPtr->ComSignalLength);
        }
#endif
        Com_InitSignalBuffer(signalType, signalInitValueId, signalLength, RxIpduPtr->IpduPartitionId);
#if COM_MAX_GWMAPPING_NUMBER > 0u
        Com_GwMappingIdType gwMappingId = rxSignalPtr->GwMappingId;
        if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
        {
            Com_GwUpdateDestinationRequest(gwMappingId, FALSE);
        }
#endif
    }
#endif
/*init group signal buffer*/
#if ((0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER))
    Com_RxGroupSignalIdType groupSignalEndId, groupSignalStartId;
    Com_RxSignalGroupIdType ipduSignalGroupNumber = RxIpduPtr->IPduSignalGroupsRefEndId;
    Com_RxSignalGroupIdType counter               = RxIpduPtr->IPduSignalGroupsRefStartId;
    for (; counter < ipduSignalGroupNumber; ++counter)
    {
        const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[counter];
        groupSignalEndId                              = rxSignalGroupPtr->GroupSignalRefEndId;
        groupSignalStartId                            = rxSignalGroupPtr->GroupSignalRefStartId;
        for (; groupSignalStartId < groupSignalEndId; ++groupSignalStartId)
        {
            const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[groupSignalStartId];
            signalInitValueId                             = rxGroupSignalPtr->SignalInitValueId;
            signalType                                    = rxGroupSignalPtr->ComSignalType;

#if COM_MAX_GWMAPPING_NUMBER > 0u
            Com_GwMappingIdType gwMappingId = rxGroupSignalPtr->GwMappingId;
            if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
            {
                Com_GwUpdateDestinationRequest(gwMappingId, FALSE);
            }
#endif

#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
            if (signalType < COM_UINT8_DYN)
#endif
            {
                signalLength = 0u;
            }
#if (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_N_ENABLE) || (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
            else
            {
                signalLength = (COM_UINT8_DYN == signalType) ? (rxGroupSignalPtr->SignalDataInitValueLength)
                                                             : (rxGroupSignalPtr->ComSignalLength);
            }
#endif
            Com_InitSignalBuffer(signalType, signalInitValueId, signalLength, RxIpduPtr->IpduPartitionId);
        }
    }
#endif

#if COM_MAX_GW_SOURCE_DESCRIPTION_NUMBER > 0u
    Com_GwSourceDescriptionIdType gwSourceDescriptionEndId = RxIpduPtr->IPduSourceSignalsRefEndId;
    Com_GwSourceDescriptionIdType gwSourceDescriptionId    = RxIpduPtr->IPduSourceSignalsRefStartId;
    for (; gwSourceDescriptionId < gwSourceDescriptionEndId; ++gwSourceDescriptionId)
    {
        const Com_GwSourceSignalType* gwSourceDescriptionPtr =
            &Com_CfgGwMappingConfigPtr->SourceSignal[gwSourceDescriptionId];
        Com_GwMappingIdType gwMappingId = gwSourceDescriptionPtr->GwMappingId;
        if (gwMappingId != COM_NOT_USED_GW_MAPPING_ID)
        {
            Com_GwUpdateDestinationRequest(gwMappingId, FALSE);
        }
    }
#endif

    RxIpduStatePtr->RxIpduRTStFlag &= COM_RX_RECEIVING_DIS;
    RxIpduStatePtr->RxIpduRTStFlag &= COM_RX_INDICATION_DIS;
    RxIpduStatePtr->RxIpduLength = RxIpduInitStatePtr->RxIpduLength;
}
#endif
/**
 * @brief set the reception deadline monitoring timer for the included signals and signal groups to the configured
 * ComFirstTimeout
 *
 */
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
COM_LOCAL void Com_ResetRxPduTimeOut(const Com_RxIPduType* RxIpduPtr, Com_RxIPduRunTimeStateType* RxIpduStatePtr)
{
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
    Com_SignalPositionType updateBitPosition;
    uint8                  updateBitMask;
#endif
    uint8 rxDMFlag = 0u;

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    ipduSignalTimeoutRefNumber = RxIpduPtr->IPduSignalsTimeoutRefEndId;
    cnt                        = RxIpduPtr->IPduSignalsTimeoutRefStartId;
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
    const uint16* rxSignalFirstTimeoutPtr = Com_CfgSignalTimeoutPtr->RxSignalFirstTimeoutVal;
    const uint16* timeoutRxSignalId       = Com_CfgSignalTimeoutPtr->TimeoutRxSignalId;
#endif
    for (; cnt < ipduSignalTimeoutRefNumber; ++cnt)
    {
#if (STD_ON == COM_RX_SIGNAL_UPDATE_BIT_ENABLE)
        const Com_RxSignalType* rxSignalPtr = &Com_CfgRxSignalPtr[timeoutRxSignalId[cnt]];
        updateBitPosition                   = rxSignalPtr->UpdateLsbBytePos;
        /*rx signal with update-bits*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            Com_RxSignalTimeOutCnt[cnt] = rxSignalFirstTimeoutPtr[cnt];
            rxDMFlag |= COM_RX_SIGNAL_DM;
        }
        /* rx signal without update-bits */
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
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;

    ipduSignalGroupTimeoutNumber                                = RxIpduPtr->IPduSignalGroupsTimeoutRefEndId;
    counter                                                     = RxIpduPtr->IPduSignalGroupsTimeoutRefStartId;
    const Com_RxSignalGroupIdType* rxSignalGroupId              = Com_CfgSignalTimeoutPtr->TimeoutRxSigGrpId;
    const uint16*                  rxSignalGroupFirstTimeoutPtr = Com_CfgSignalTimeoutPtr->RxSigGrpFirstTimeoutVal;
    for (; counter < ipduSignalGroupTimeoutNumber; ++counter)
    {
#if (STD_ON == COM_RX_SIG_GROUP_UPDATE_BIT_ENABLE)
        const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[rxSignalGroupId[counter]];
        updateBitPosition                             = rxSignalGroupPtr->UpdateLsbBytePos;
        /*rx signal group with update-bits*/
        if (COM_UNUSED_SIGNALPOSITION != updateBitPosition)
        {
            Com_RxSignalGroupTimeOutCnt[counter] = rxSignalGroupFirstTimeoutPtr[counter];
            rxDMFlag |= COM_RX_SIGNAL_GROUP_DM;
        }
        /* rx signal group without update-bits */
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

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE) || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    if (rxDMFlag > 0u)
    {
        if (COM_RX_IPDU_DM == (rxDMFlag & COM_RX_IPDU_DM)) /* PRQA S 2991,2995 */ /* VL_Com_ResultAlwaysTrue */
        {
            RxIpduStatePtr->RxIpduBaseDMTimeout = RxIpduPtr->RxIPduDMFirstTimeout;
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
}
#endif

/**
 * @brief set the reception deadline monitoring timer for the included signals and signal groups to 0
 *
 */
#if ((COM_MAX_IPDUGROUP_NUMBER > 0u) && ((COM_MAX_RXSIGNAL_NUMBER + COM_MAX_RXSIGNALGROUP_NUMBER) > 0u))
COM_LOCAL void Com_DisableRxPduTimeOut(const Com_RxIPduType* RxIpduPtr, Com_RxIPduRunTimeStateType* RxIpduStatePtr)
{
/* PRQA S 2934 ++ */ /* VL_Com_NullPtrValue */
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
    Com_RxSigTimeoutIdType ipduSignalTimeoutRefNumber, cnt;
    cnt = RxIpduPtr->IPduSignalsTimeoutRefStartId;
    if (cnt != COM_UNUSED_RXSIGTIMEOUTID)
    {
        ipduSignalTimeoutRefNumber = RxIpduPtr->IPduSignalsTimeoutRefEndId - cnt;
        (void)IStdLib_MemSet(&Com_RxSignalTimeOutCnt[cnt], 0, ipduSignalTimeoutRefNumber);
    }
#endif
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
    Com_RxSigGrpTimeoutIdType ipduSignalGroupTimeoutNumber, counter;
    counter = RxIpduPtr->IPduSignalGroupsTimeoutRefStartId;
    if (counter != COM_UNUSED_RXSIGGRPTIMEOUTID)
    {
        ipduSignalGroupTimeoutNumber = RxIpduPtr->IPduSignalGroupsTimeoutRefEndId - counter;
        (void)IStdLib_MemSet(&Com_RxSignalGroupTimeOutCnt[counter], 0, ipduSignalGroupTimeoutNumber);
    }
#endif
    /* PRQA S 2934 -- */

    RxIpduStatePtr->RxIpduDMFlag        = 0u;
    RxIpduStatePtr->RxIpduBaseDMTimeout = 0u;
    COM_NOUSED(RxIpduPtr);
}
#endif
/**
 * @brief reset OCCURRENCE of filter with FilterAlgorithm ONE_EVERY_N.
 *
 */
#if ((COM_MAX_IPDUGROUP_NUMBER > 0u) && (COM_MAX_ONEEVERYNFILTERSIGNAL_NUMBER > 0u))
COM_LOCAL void Com_ResetRxOccurrenceOfPduFilter(const Com_RxIPduType* RxIpduPtr)
{
/*reset signal/group signal of rx pdu which FilterAlgorithm is ONE_EVERY_N*/
#if (0u < COM_MAX_RXSIGNAL_NUMBER) && (COM_MAX_RXSIGNAL_FILTERTYPE_MAX_NUMBER > 0u)
    Com_RxSignalIdType ipduSignalStartId = RxIpduPtr->IPduSignalsRefEndId;
    Com_RxSignalIdType ipduSignalEndId   = RxIpduPtr->IpduSignalRefStartId;
    for (; ipduSignalEndId < ipduSignalStartId; ++ipduSignalEndId)
    {
        const Com_RxSignalType* rxSignalPtr = &Com_CfgRxSignalPtr[ipduSignalEndId];
        if ((rxSignalPtr->FilterAlgorithm == COM_ONE_EVERY_N)
            && (COM_UNUSED_RXSIGNALFILTERID != rxSignalPtr->FilterIndex))
        {
            Com_OneEveryNcnt[rxSignalPtr->FilterIndex] = 0u;
        }
    }
#endif
#if (                                                                          \
    (0u < COM_MAX_RXSIGNALGROUP_NUMBER) && (0u < COM_MAX_RXGROUPSIGNAL_NUMBER) \
    && (COM_MAX_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u))
    Com_RxSignalGroupIdType ipduSignalGroupNumber, counter;
    Com_RxGroupSignalIdType groupSignalNumber, groupSignalId;
    ipduSignalGroupNumber = RxIpduPtr->IPduSignalGroupsRefEndId;
    counter               = RxIpduPtr->IPduSignalGroupsRefStartId;

    for (; counter < ipduSignalGroupNumber; ++counter)
    {
        const Com_RxSignalGroupType* rxSignalGroupPtr = &Com_CfgRxSignalGroupPtr[counter];
        groupSignalNumber                             = rxSignalGroupPtr->GroupSignalRefEndId;
        groupSignalId                                 = rxSignalGroupPtr->GroupSignalRefStartId;
        for (; groupSignalId < groupSignalNumber; ++groupSignalId)
        {
            const Com_RxGroupSignalType* rxGroupSignalPtr = &Com_CfgRxGroupSignalPtr[groupSignalId];
            if ((rxGroupSignalPtr->FilterAlgorithm == COM_ONE_EVERY_N)
                && (COM_UNUSED_RXGRPSIGFILTERID != rxGroupSignalPtr->FilterIndex))
            {
                Com_OneEveryNcnt[rxGroupSignalPtr->FilterIndex] = 0u;
            }
        }
    }
#endif
    return;
}
#endif
/* PRQA S 2895 -- */
/**
 * @brief Com_InitHandle is used by Com_Init
 *
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Com_STCAL */
COM_LOCAL void Com_InitHandle(
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    ApplicationType ApplicationPartitionId,
#endif
    uint16 PartitionId)
/* PRQA S 6070 -- */
{
    Com_Status[PartitionId]                  = COM_INIT;
    Com_MainFunctionIdType mainfunctionRxNum = Com_ConfigStd->MainFunctionRxNum;
    Com_MainFunctionIdType mainfunctionTxNum = Com_ConfigStd->MainFunctionTxNum;
    Com_MainFunctionIdType mainfunctionIdMaxNum =
        (mainfunctionRxNum > mainfunctionTxNum) ? mainfunctionRxNum : mainfunctionTxNum;
    if (!Com_GlobalInitStatus)
    {
        Com_GlobalInitStatus = TRUE;
#if (0u < COM_MAX_TXSIGNALGROUP_NUMBER)
        Com_TxSignalGroupTriggerFlagInit();
#endif
#if COM_MAX_GWMAPPING_NUMBER > 0u
        Com_GwInitDestinationRequest();
#endif
    }
    Com_SignalBufferInit(PartitionId);
    for (Com_MainFunctionIdType mainFunctionId = 0u; mainFunctionId < mainfunctionIdMaxNum; ++mainFunctionId)
    {
        if (mainFunctionId < mainfunctionRxNum)
        {
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
            /* Multiple partition check. */
            if (ApplicationPartitionId == Com_GetRxMFPartitionId(mainFunctionId))
#endif
            {
                Com_PduBufferRxInit(mainFunctionId);
            }
        }
        if (mainFunctionId < mainfunctionTxNum)
        {
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
            if (ApplicationPartitionId == Com_GetTxMFPartitionId(mainFunctionId))
#endif
            {
                Com_PduBufferTxInit(mainFunctionId);
                Com_TxSignalTMCStateInit(mainFunctionId);
            }
        }
    }
}
/**
 * @brief Com_DeInitHandle is used by Com_DeInit
 *
 */
COM_LOCAL void Com_DeInitHandle(
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
    ApplicationType ApplicationPartitionId,
#endif
    uint16 PartitionId)
{
    Com_Status[PartitionId] = COM_UNINIT;
    if (Com_GlobalInitStatus)
    {
        Com_GlobalInitStatus = FALSE;
#if (0u < COM_MAX_IPDUGROUP_NUMBER)
        /* clear All IPduGroup Active Flag */
        (void)IStdLib_MemSet(Com_IpduGroupEnable, 0u, COM_MAX_IPDUGROUP_NUMBER);
#endif
    }
    Com_MainFunctionIdType mainfunctionRxNum = Com_ConfigStd->MainFunctionRxNum;
    Com_MainFunctionIdType mainfunctionTxNum = Com_ConfigStd->MainFunctionTxNum;
    Com_MainFunctionIdType mainfunctionIdMaxNum =
        (mainfunctionRxNum > mainfunctionTxNum) ? mainfunctionRxNum : mainfunctionTxNum;

    for (Com_MainFunctionIdType mainFunctionId = 0u; mainFunctionId < mainfunctionIdMaxNum; ++mainFunctionId)
    {
        if (mainFunctionId < mainfunctionRxNum)
        {
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
            /* Multiple partition check. */
            if (ApplicationPartitionId == Com_GetRxMFPartitionId(mainFunctionId))
#endif
            {
                Com_RxIPduRunTimeStateType* rxIpduStatePtr = Com_RxIPduRunTimeState[mainFunctionId];
                PduIdType                   rxIpduNum =
                    Com_GetEndOfMainFunctionRx(mainFunctionId) - Com_GetStartOfMainFunctionRx(mainFunctionId);
                for (uint16 cirCnt = 0u; cirCnt < rxIpduNum; ++cirCnt)
                {
                    /* clear All Rx IPdu Active Flag */
                    rxIpduStatePtr[cirCnt].RxIpduRTStFlag &= COM_RX_ACTIVE_DIS;
                }
            }
        }
        if (mainFunctionId < mainfunctionTxNum)
        {
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
            if (ApplicationPartitionId == Com_GetTxMFPartitionId(mainFunctionId))
#endif
            {
                /* clear All Rx IPdu Active Flag */
                PduIdType txIpduNum =
                    Com_GetEndOfMainFunctionTx(mainFunctionId) - Com_GetStartOfMainFunctionTx(mainFunctionId);
                for (uint16 cirCnt = 0u; cirCnt < txIpduNum; ++cirCnt)
                {
                    Com_TxIPduRunTimeState[mainFunctionId][cirCnt].TxIpduRTStFlag &= COM_TX_ACTIVE_DIS;
                }
            }
        }
    }
}
#if (COM_MULITIVARIANT_SUPPORT == STD_ON)
/**
 * get internal IPdu Group Id of the Com module
 */
COM_LOCAL Com_IpduGroupIdType Com_GetIpduGroupIdFromUserId(uint16 apiId, Com_IpduGroupIdType ipduGroupId)
{
    Com_IpduGroupIdType groupId = COM_UNUSED_UINT16;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (ipduGroupId >= COM_MAX_IPDUGROUP_NUMBER)
    {
        Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, apiId, COM_E_PARAM);
    }
    else
#endif
    {
        if ((Com_ConfigStd != NULL_PTR) && (Com_ConfigStd->UserIdMapComIPduGrpIdPtr != NULL_PTR))
        {
            groupId = Com_ConfigStd->UserIdMapComIPduGrpIdPtr[ipduGroupId];
        }
    }
    return groupId;
}
/**
 * get internal ISignal or GroupSignal Id of the Com module
 */
COM_LOCAL Com_SignalIdType Com_GetSignalAndGroupSignalIdFromUserId(uint16 apiId, Com_SignalIdType signalId)
{
    Com_SignalIdType sigId = COM_UNUSED_UINT16;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (signalId >= COM_MAX_SIGANDGRPSIG_NUMBER)
    {
        Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, apiId, COM_E_PARAM);
    }
    else
#endif
    {
        if ((Com_ConfigStd != NULL_PTR) && (Com_ConfigStd->UserIdMapComSigAndGrpSigIdPtr != NULL_PTR))
        {
            sigId = Com_ConfigStd->UserIdMapComSigAndGrpSigIdPtr[signalId];
        }
    }
    return sigId;
}
/**
 * get internal SignalGroup Id of the Com module
 */
COM_LOCAL Com_SignalGroupIdType Com_GetSignalGroupIdFromUserId(uint16 apiId, Com_SignalGroupIdType signalGroupId)
{
    Com_SignalGroupIdType sigGrpId = COM_UNUSED_UINT16;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (signalGroupId >= COM_MAX_SIGGROUP_NUMBER)
    {
        Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, apiId, COM_E_PARAM);
    }
    else
#endif
    {
        if ((Com_ConfigStd != NULL_PTR) && (Com_ConfigStd->UserIdMapComSigGrpIdPtr != NULL_PTR))
        {
            sigGrpId = Com_ConfigStd->UserIdMapComSigGrpIdPtr[signalGroupId];
        }
    }
    return sigGrpId;
}
/**
 * get internal Pdu Id of the Com module
 */
COM_LOCAL PduIdType Com_GetPduIdFromUserId(uint16 apiId, PduIdType pduId)
{
    PduIdType pduIdret = COM_UNUSED_UINT16;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if (pduId >= COM_MAX_IPDU_NUMBER)
    {
        Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, apiId, COM_E_PARAM);
    }
    else
#endif
    {
        if (Com_ConfigStd->UserIdMapComIPduIdPtr != NULL_PTR)
        {
            pduIdret = Com_ConfigStd->UserIdMapComIPduIdPtr[pduId];
        }
    }
    return pduIdret;
}

#endif
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* PRQA S 0316,2844 -- */
/* PRQA S 2814,2824 -- */
/* PRQA S 3120,3132 -- */
