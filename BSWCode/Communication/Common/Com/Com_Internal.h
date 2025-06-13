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
**  FILENAME    : Com_Internal.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : zhengfei.li                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : COM internal header for internal API declarations           **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef COM_INTERNAL_H
#define COM_INTERNAL_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Com.h"
#include "PduR_Com.h"
#if (STD_ON == COM_MULTIPLE_PARTITION_USED)
#include "Os.h"
#endif
#include "istd_lib.h"
#include "SchM_Com.h"

/*******************************************************************************
**                      Published information                                 **
*******************************************************************************/
/* Published information */
#if !defined(COM_PUBLISHED_INFORMATION)
#define COM_PUBLISHED_INFORMATION
#define COM_MODULE_ID                   50u
#define COM_VENDOR_ID                   62u
#define COM_AR_RELEASE_MAJOR_VERSION    4u
#define COM_AR_RELEASE_MINOR_VERSION    5u
#define COM_AR_RELEASE_REVISION_VERSION 0u
#define COM_SW_MAJOR_VERSION            2u
#define COM_SW_MINOR_VERSION            1u
#define COM_SW_PATCH_VERSION            25u
#elif ((COM_SW_MAJOR_VERSION != 2u) || (COM_SW_MINOR_VERSION != 1u))
#error "Com: Mismatch in Software Version"
#endif

/*******************************************************************************
**                      Internal Macro Define                                 **
*******************************************************************************/
#define Com_GetStartOfMainFunctionTx(mainFunctionId) \
    ((mainFunctionId) == 0u) ? 0u : Com_ConfigStd->MainFunctionTxRange[(mainFunctionId)-1u]
#define Com_GetEndOfMainFunctionTx(mainFunctionId) Com_ConfigStd->MainFunctionTxRange[(mainFunctionId)]

#define Com_GetStartOfMainFunctionRx(mainFunctionId) \
    ((mainFunctionId) == 0u) ? 0u : Com_ConfigStd->MainFunctionRxRange[(mainFunctionId)-1u]
#define Com_GetEndOfMainFunctionRx(mainFunctionId) Com_ConfigStd->MainFunctionRxRange[(mainFunctionId)]

/* PartitionId used to check multiple partition. */
#if (STD_ON == COM_DEV_ERROR_DETECT) && (STD_ON == COM_MULTIPLE_PARTITION_USED)
#define Com_GetRxMFPartitionId(mainfunctionId) (Com_ConfigStd->ComMainFuncRxPartitionRange[(mainfunctionId)])
#define Com_GetTxMFPartitionId(mainfunctionId) (Com_ConfigStd->ComMainFuncTxPartitionRange[(mainfunctionId)])
#if (COM_RXSIGNAL_NUMBER > 0u)
#define Com_GetRxSigPartitionId(signalId) (Com_ConfigStd->ComRxSignal[(signalId)].ComRxSignalPartitionId)
#endif
#if (COM_TXSIGNAL_NUMBER > 0u)
#define Com_GetTxSigPartitionId(signalId) (Com_ConfigStd->ComTxSignal[(signalId)].ComTxSignalPartitionId)
#endif
#if (COM_RXIPDU_NUMBER > 0u)
#define Com_GetRxIPduPartitionId(ipduId) (Com_ConfigStd->ComRxIPdu[(ipduId)].ComRxIpduPartitionId)
#endif
#if (COM_TXIPDU_NUMBER > 0u)
#define Com_GetTxIPduPartitionId(ipduId) (Com_ConfigStd->ComTxIPdu[(ipduId)].ComTxIpduPartitionId)
#endif
#if (COM_RXSIGNALGROUP_NUMBER > 0u)
#define Com_GetRxSigGrpPartitionId(signaGroupId) \
    (Com_ConfigStd->ComRxSignalGroup[(signaGroupId)].ComRxSigGrpPartitionId)
#endif
#if (COM_TXSIGNALGROUP_NUMBER > 0u)
#define Com_GetTxSigGrpPartitionId(signaGroupId) \
    (Com_ConfigStd->ComTxSignalGroup[(signaGroupId)].ComTxSigGrpPartitionId)
#endif
#if (COM_IPDUGROUP_MAX > 0u)
#define Com_GetIPduGrpPartitionId(ipduGroupId) (Com_ConfigStd->ComIPduGroupPartition[(ipduGroupId)])
#endif
#if (COM_TXGROUPSIGNAL_NUMBER > 0u)
#define Com_GetTxSignalGroupId(groupSignalId)     Com_ConfigStd->ComTxGroupSignal[(groupSignalId)].ComSignalGroupRef
#define Com_GetTxGrpSigPartitionId(groupSignalId) (Com_GetTxSigGrpPartitionId(Com_GetTxSignalGroupId(groupSignalId)))
#endif
#if (COM_RXGROUPSIGNAL_NUMBER > 0u)
#define Com_GetRxSignalGroupId(groupSignalId)     Com_ConfigStd->ComRxGroupSignal[(groupSignalId)].ComSignalGroupRef
#define Com_GetRxGrpSigPartitionId(groupSignalId) (Com_GetRxSigGrpPartitionId(Com_GetRxSignalGroupId(groupSignalId)))
#endif
#endif

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/*define in Com.c*/

#if (COM_IPDUGROUP_MAX > 0u)
extern VAR(boolean, COM_VAR) Com_IpduGroupDMEnable[COM_IPDUGROUP_MAX];
extern VAR(boolean, COM_VAR) Com_IpduGroupEnable[COM_IPDUGROUP_MAX];
#endif

/*define in Com_PBcfg.c*/
#if (COM_RXIPDU_NUMBER > 0u)
extern VAR(Com_RxIPduRunTimeStateType, COM_VAR) Com_RxIPduRunTimeState[COM_RXIPDU_NUMBER];
extern CONST(Com_RxIPduRunTimeStateType, COM_CONST) Com_RxIPduInitState[COM_RXIPDU_NUMBER];
#endif /*COM_RXIPDU_NUMBER > 0u*/

#if (COM_TXIPDU_NUMBER > 0u)
extern VAR(Com_TxIPduRunTimeStateType, COM_VAR) Com_TxIPduRunTimeState[COM_TXIPDU_NUMBER];
extern CONST(Com_TxIPduRunTimeStateType, COM_CONST) Com_TxIPduInitState[COM_TXIPDU_NUMBER];
#endif /*COM_TXIPDU_NUMBER > 0u*/

#if (COM_RXIPDUGROUP_NUMBER > 0u)
extern CONST(Com_RxIpduGroupIdType, COM_CONST) Com_RxIPduGroupsRef[COM_RXIPDUGROUP_NUMBER];
#endif

#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE) && (STD_ON == COM_IPDU_REPLICATION_ENABLE)
extern CONST(Com_IPduReplicationType, COM_CONST) Com_IPduReplication[COM_RXIPDU_COUNTER_NUMBER];
#endif

#if (STD_ON == COM_RX_IPDU_COUNTER_ENABLE)
extern CONST(Com_IPduCounterType, COM_CONST) Com_RxIPduCounter[COM_RXIPDU_COUNTER_NUMBER];
#endif

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
extern CONST(uint16, COM_CONST) Com_RxSignalFirstTimeoutVal[COM_RXSIG_TIMEOUT_NUMBER];
extern CONST(uint16, COM_CONST) Com_RxSignalTimeoutVal[COM_RXSIG_TIMEOUT_NUMBER];
extern CONST(Com_SignalIdType, COM_CONST) Com_TimeoutRxSignalRef[COM_RXSIG_TIMEOUT_NUMBER];
extern CONST(Com_RxDataTimeoutActionType, COM_CONST) Com_RxSignalDataTimeoutActionCfg[COM_RXSIG_TIMEOUT_NUMBER];
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
extern CONST(uint16, COM_CONST) Com_RxSignalSubstituteValueIdRef[COM_RXSIG_TIMEOUT_NUMBER];
#endif
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
extern CONST(Com_RxTimeoutNotificationType, COM_CONST) Com_RxSignalTimeoutNotificationCfg[COM_RXSIG_TIMEOUT_NUMBER];
#endif
#endif

#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
extern CONST(uint16, COM_CONST) Com_RxSigGrpFirstTimeoutVal[COM_RXSIGGRP_TIMEOUT_NUMBER];
extern CONST(uint16, COM_CONST) Com_RxSigGrpTimeoutVal[COM_RXSIGGRP_TIMEOUT_NUMBER];
extern CONST(Com_RxSignalGroupIdType, COM_CONST) Com_TimeoutRxSigGrpRef[COM_RXSIGGRP_TIMEOUT_NUMBER];
extern CONST(Com_RxDataTimeoutActionType, COM_CONST) Com_RxSigGrpDataTimeoutActionCfg[COM_RXSIGGRP_TIMEOUT_NUMBER];
#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
extern CONST(Com_RxTimeoutNotificationType, COM_CONST) Com_RxSigGrpTimeoutNotificationCfg[COM_RXSIGGRP_TIMEOUT_NUMBER];
#endif
#endif

#if (COM_TXIPDUGROUP_NUMBER > 0u)
extern CONST(Com_TxIpduGroupIdType, COM_CONST) Com_TxIPduGroupsRef[COM_TXIPDUGROUP_NUMBER];
#endif

#if (0u < COM_TX_MODE_TRUE_PERIOD_NUMBER) || (0u < COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER)
extern CONST(Com_TxModePeriodType, COM_CONST)
    Com_TxModeTruePeriod[COM_TX_MODE_TRUE_PERIOD_NUMBER + COM_TX_MODE_TRUE_MIXED_NOREPETITION_NUMBER];
#endif
#if (0u < COM_TX_MODE_FALSE_PERIOD_NUMBER) || (0u < COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER)
extern CONST(Com_TxModePeriodType, COM_CONST)
    Com_TxModeFalsePeriod[COM_TX_MODE_FALSE_PERIOD_NUMBER + COM_TX_MODE_FALSE_MIXED_NOREPETITION_NUMBER];
#endif
#if (0u < COM_TX_MODE_TRUE_DIRECT_NUMBER)
extern CONST(Com_TxModeDirectRepetitionsType, COM_CONST)
    Com_TxModeTrueDirectRepetitions[COM_TX_MODE_TRUE_DIRECT_NUMBER];
#endif
#if (0u < COM_TX_MODE_FALSE_DIRECT_NUMBER)
extern CONST(Com_TxModeDirectRepetitionsType, COM_CONST)
    Com_TxModeFalseDirectRepetitions[COM_TX_MODE_FALSE_DIRECT_NUMBER];
#endif
#if (0u < COM_TX_MODE_TRUE_MIXED_NUMBER)
extern CONST(Com_TxModeMixedRepetitionsType, COM_CONST) Com_TxModeTrueMixedRepetitions[COM_TX_MODE_TRUE_MIXED_NUMBER];
#endif
#if (0u < COM_TX_MODE_FALSE_MIXED_NUMBER)
extern CONST(Com_TxModeMixedRepetitionsType, COM_CONST) Com_TxModeFalseMixedRepetitions[COM_TX_MODE_FALSE_MIXED_NUMBER];
#endif

#if (STD_ON == COM_TX_IPDU_COUNTER_ENABLE)
extern CONST(Com_IPduCounterType, COM_CONST) Com_TxIPduCounter[COM_TXIPDU_COUNTER_NUMBER];
#endif

#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ENABLE)
extern CONST(uint16, COM_CONST) Com_RxSignalDataInvalidValueIdRef[COM_RXSIGNAL_INVALID_NUMBER];
#if (0u < COM_RX_SIGNAL_8BIT_INVALIDVALUE_NUMBER)
extern CONST(uint16, COM_CONST) Com_RxSignalDataInvalidValueLengthRef[COM_RXSIGNAL_INVALID_NUMBER];
#endif
extern CONST(Com_DataInvalidActionType, COM_CONST) Com_RxSignalDataInvalidActionRef[COM_RXSIGNAL_INVALID_NUMBER];
#if (STD_ON == COM_RX_SIGNAL_INVALID_DATA_ACTION_NOTIFY_ENABLE)
extern CONST(Com_RxInvalidNotificationType, COM_CONST) Com_RxInvalidNotificationCfg[COM_RXSIGNAL_INVALID_NUMBER];
#endif
#endif

#if (0u < COM_RXMASKNEWDIFFERMASKOLD_NUMBER)
extern CONST(Com_FilterMaskType, COM_CONST) Com_RxMaskedNewDiffersMaskedOld[COM_RXMASKNEWDIFFERMASKOLD_NUMBER];
#endif
#if (0u < COM_RXMASKNEWDIFFERX_NUMBER)
extern CONST(Com_FilterMaskNewDifEquXType, COM_CONST) Com_RxMaskedNewDiffersX[COM_RXMASKNEWDIFFERX_NUMBER];
#endif
#if (0u < COM_RXMASKNEWEQUALSX_NUMBER)
extern CONST(Com_FilterMaskNewDifEquXType, COM_CONST) Com_RxMaskedNewEqualsX[COM_RXMASKNEWEQUALSX_NUMBER];
#endif
#if (0u < COM_RXNEWISOUTSIDE_NUMBER)
extern CONST(Com_FilterNewIsWithinOutsideType, COM_CONST) Com_RxNewIsOutside[COM_RXNEWISOUTSIDE_NUMBER];
#endif
#if (0u < COM_RXNEWISWITHIN_NUMBER)
extern CONST(Com_FilterNewIsWithinOutsideType, COM_CONST) Com_RxNewIsWithin[COM_RXNEWISWITHIN_NUMBER];
#endif
#if (0u < COM_ONEEVERYNFILTERSIGNAL_NUMBER)
extern CONST(Com_FilterONEEVERYNType, COM_CONST) Com_FilterOneEveryN[COM_ONEEVERYNFILTERSIGNAL_NUMBER];
#endif

#if (STD_ON == COM_TX_SIGNAL_INVALID_DATA_ENABLE)
extern CONST(uint16, COM_CONST) Com_TxSignalDataInvalidValueIdRef[COM_TXSIGNAL_INVALID_NUMBER];
#if (0u < COM_TX_SIGNAL_8BIT_INVALIDVALUE_NUMBER)
extern CONST(uint16, COM_CONST) Com_TxSignalDataInvalidValueLengthRef[COM_TXSIGNAL_INVALID_NUMBER];
#endif
extern CONST(Com_DataInvalidActionType, COM_CONST) Com_TxSignalDataInvalidActionRef[COM_TXSIGNAL_INVALID_NUMBER];
#endif
#if (COM_TXSIG_ERROR_AND_NOTIFY_NUMBER > 0u)
extern CONST(Com_ErrorAndNotificationType, COM_CONST)
    Com_TxSignalErrorAndNotification[COM_TXSIG_ERROR_AND_NOTIFY_NUMBER];
#endif

#if (COM_TMCTXSIGNAL_NUMBER > 0u)
#if (0u < COM_TXMASKNEWDIFFERMASKOLD_NUMBER)
extern CONST(Com_FilterMaskType, COM_CONST) Com_TxMaskedNewDiffersMaskedOld[COM_TXMASKNEWDIFFERMASKOLD_NUMBER];
#endif
#if (0u < COM_TXMASKNEWDIFFERX_NUMBER)
extern CONST(Com_FilterMaskNewDifEquXType, COM_CONST) Com_TxMaskedNewDiffersX[COM_TXMASKNEWDIFFERX_NUMBER];
#endif
#if (0u < COM_TXMASKNEWEQUALSX_NUMBER)
extern CONST(Com_FilterMaskNewDifEquXType, COM_CONST) Com_TxMaskedNewEqualsX[COM_TXMASKNEWEQUALSX_NUMBER];
#endif
#if (0u < COM_TXNEWISOUTSIDE_NUMBER)
extern CONST(Com_FilterNewIsWithinOutsideType, COM_CONST) Com_TxNewIsOutside[COM_TXNEWISOUTSIDE_NUMBER];
#endif
#if (0u < COM_TXNEWISWITHIN_NUMBER)
extern CONST(Com_FilterNewIsWithinOutsideType, COM_CONST) Com_TxNewIsWithin[COM_TXNEWISWITHIN_NUMBER];
#endif
#endif

#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_ENABLE)
extern CONST(uint16, COM_CONST) Com_TxSigTimeoutVal[COM_TXSIGNAL_TIMEOUT_NUMBER];
#if (STD_ON == COM_TX_SIGNAL_TIMEOUT_NOTIFICATION_ENABLE)
extern CONST(Com_TxTimeoutNotificationType, COM_CONST) Com_TxSignalTimeoutNotificationCfg[COM_TXSIGNAL_TIMEOUT_NUMBER];
#endif
#endif

#if (0U < COM_ENABLE_SIGNAL_GROUP_ARRAY_API)
extern CONST(Com_SignalGroupArrayType, COM_CONST) Com_SignalGroupArray[COM_SIGNALGROUPARRY_NUMBER];
#endif

#if (COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER > 0u)
extern CONST(Com_ErrorAndNotificationType, COM_CONST)
    Com_TxSignalGroupErrorAndNotification[COM_TXSIGGRP_ERROR_AND_NOTIFY_NUMBER];
#endif

#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_ENABLE)
extern CONST(uint16, COM_CONST) Com_TxSigGrpTimeoutVal[COM_TXSIGGRP_TIMEOUT_NUMBER];
#if (STD_ON == COM_TX_SIG_GROUP_TIMEOUT_NOTIFICATION_ENABLE)
extern CONST(Com_TxTimeoutNotificationType, COM_CONST) Com_TxSigGrpTimeoutNotificationCfg[COM_TXSIGGRP_TIMEOUT_NUMBER];
#endif
#endif

#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
extern CONST(Com_RxGroupSignalIdType, COM_CONST) Com_RxGrpSigSubstituteValueIdRef[COM_RXGRPSIG_SUBSTITUTE_NUMBER];
#endif

#if COM_GWMAPPING_NUMBER > 0u
extern const Com_GwDestType Com_GwDest[COM_GW_DESTINATION_NUM];

#if COM_MULTIPLE_PARTITION_USED == STD_ON
extern const ApplicationType Com_MainFunctionRouteSignalsApplcation[COM_NUMBER_OF_MAIN_FUNCTION_ROUTE_SIGNALS];
#endif
#endif

#if (COM_TXIPDUBUFF_SIZE > 0u)
extern VAR(uint8, COM_VAR) Com_TxIPduRuntimeBuff[COM_TXIPDUBUFF_SIZE];
extern CONST(uint8, COM_CONST) Com_TxIPduInitValue[COM_TXIPDUBUFF_SIZE];
#endif /*COM_TXIPDUBUFF_SIZE > 0u*/

#if (COM_RXIPDUBUFF_SIZE > 0u)
extern VAR(uint8, COM_VAR) Com_RxIPduRuntimeBuff[COM_RXIPDUBUFF_SIZE];
extern CONST(uint8, COM_CONST) Com_RxIPduInitValue[COM_RXIPDUBUFF_SIZE];
#endif /*COM_RXIPDUBUFF_SIZE > 0u*/

#if (COM_SIGNAL_BOOLBUFF_SIZE > 0u)
extern VAR(boolean, COM_VAR) Com_SignalBoolRuntimeBuff[COM_SIGNAL_BOOLBUFF_SIZE];
extern CONST(boolean, COM_CONST) Com_SignalBoolInitValue[COM_SIGNAL_BOOLBUFF_SIZE];
#endif /*COM_SIGNAL_BOOLBUFF_SIZE > 0u*/

#if (COM_SIGNAL_8BITBUFF_SIZE > 0u)
extern VAR(uint8, COM_VAR) Com_Signal8BitRuntimeBuff[COM_SIGNAL_8BITBUFF_SIZE];
extern CONST(uint8, COM_CONST) Com_Signal8BitInitValue[COM_SIGNAL_8BITBUFF_SIZE];
#endif /*COM_SIGNAL_8BITBUFF_SIZE > 0u*/

#if (COM_SIGNAL_16BITBUFF_SIZE > 0u)
extern VAR(uint16, COM_VAR) Com_Signal16BitRuntimeBuff[COM_SIGNAL_16BITBUFF_SIZE];
extern CONST(uint16, COM_CONST) Com_Signal16BitInitValue[COM_SIGNAL_16BITBUFF_SIZE];
#endif /*COM_SIGNAL_16BITBUFF_SIZE > 0u*/

#if (COM_SIGNAL_32BITBUFF_SIZE > 0u)
extern VAR(uint32, COM_VAR) Com_Signal32BitRuntimeBuff[COM_SIGNAL_32BITBUFF_SIZE];
extern CONST(uint32, COM_CONST) Com_Signal32BitInitValue[COM_SIGNAL_32BITBUFF_SIZE];
#endif /*COM_SIGNAL_32BITBUFF_SIZE > 0u*/

#if (COM_SIGNAL_64BITBUFF_SIZE > 0u)
extern VAR(uint64, COM_VAR) Com_Signal64BitRuntimeBuff[COM_SIGNAL_64BITBUFF_SIZE];
extern CONST(uint64, COM_CONST) Com_Signal64BitInitValue[COM_SIGNAL_64BITBUFF_SIZE];
#endif /*COM_SIGNAL_64BITBUFF_SIZE > 0u*/

#if (COM_RXGROUPSIGNAL_BOOLBUFF_SIZE > 0u)
extern VAR(boolean, COM_VAR) Com_RxGroupSignalBoolShadowBuff[COM_RXGROUPSIGNAL_BOOLBUFF_SIZE];
#endif

#if (COM_RXGROUPSIGNAL_8BITBUFF_SIZE > 0u)
extern VAR(uint8, COM_VAR) Com_RxGroupSignal8BitShadowBuff[COM_RXGROUPSIGNAL_8BITBUFF_SIZE];
#endif

#if (COM_RXGROUPSIGNAL_16BITBUFF_SIZE > 0u)
extern VAR(uint16, COM_VAR) Com_RxGroupSignal16BitShadowBuff[COM_RXGROUPSIGNAL_16BITBUFF_SIZE];
#endif

#if (COM_RXGROUPSIGNAL_32BITBUFF_SIZE > 0u)
extern VAR(uint32, COM_VAR) Com_RxGroupSignal32BitShadowBuff[COM_RXGROUPSIGNAL_32BITBUFF_SIZE];
#endif

#if (COM_RXGROUPSIGNAL_64BITBUFF_SIZE > 0u)
extern VAR(uint64, COM_VAR) Com_RxGroupSignal64BitShadowBuff[COM_RXGROUPSIGNAL_64BITBUFF_SIZE];
#endif

#if (COM_GWSIGNAL_BOOLBUFF_SIZE > 0u)
extern VAR(boolean, COM_VAR) Com_GWSignalBoolBuff[COM_GWSIGNAL_BOOLBUFF_SIZE];
#endif

#if (COM_GWSIGNAL_8BITBUFF_SIZE > 0u)
extern VAR(uint8, COM_VAR) Com_GWSignal8BitBuff[COM_GWSIGNAL_8BITBUFF_SIZE];
#endif

#if (COM_GWSIGNAL_16BITBUFF_SIZE > 0u)
extern VAR(uint16, COM_VAR) Com_GWSignal16BitBuff[COM_GWSIGNAL_16BITBUFF_SIZE];
#endif

#if (COM_GWSIGNAL_32BITBUFF_SIZE > 0u)
extern VAR(uint32, COM_VAR) Com_GWSignal32BitBuff[COM_GWSIGNAL_32BITBUFF_SIZE];
#endif

#if (COM_GWSIGNAL_64BITBUFF_SIZE > 0u)
extern VAR(uint64, COM_VAR) Com_GWSignal64BitBuff[COM_GWSIGNAL_64BITBUFF_SIZE];
#endif

#if (0u < COM_TMCTXSIGNAL_NUMBER)
extern VAR(boolean, COM_VAR) Com_TxSignalTMCRunTime[COM_TMCTXSIGNAL_NUMBER];
extern CONST(boolean, COM_CONST) Com_TxSignalInitTMC[COM_TMCTXSIGNAL_NUMBER];
#endif

#if (COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
extern VAR(boolean, COM_VAR) Com_TxGroupSignalTMCBuffer[COM_TXGRPSIG_FILTERTYPE_MAX_NUMBER];
#endif

#if (COM_SIGNAL_BOOL_INVALID_SIZE > 0u)
extern CONST(boolean, COM_CONST) Com_SignalBoolInvalidValue[COM_SIGNAL_BOOL_INVALID_SIZE];
#endif

#if (COM_SIGNAL_8BIT_INVALID_SIZE > 0u)
extern CONST(uint8, COM_CONST) Com_Signal8BitInvalidValue[COM_SIGNAL_8BIT_INVALID_SIZE];
#endif

#if (COM_SIGNAL_16BIT_INVALID_SIZE > 0u)
extern CONST(uint16, COM_CONST) Com_Signal16BitInvalidValue[COM_SIGNAL_16BIT_INVALID_SIZE];
#endif

#if (COM_SIGNAL_32BIT_INVALID_SIZE > 0u)
extern CONST(uint32, COM_CONST) Com_Signal32BitInvalidValue[COM_SIGNAL_32BIT_INVALID_SIZE];
#endif

#if (COM_SIGNAL_64BIT_INVALID_SIZE > 0u)
extern CONST(uint64, COM_CONST) Com_Signal64BitInvalidValue[COM_SIGNAL_64BIT_INVALID_SIZE];
#endif

#if (COM_TXSIGNALGROUP_NUMBER > 0u)
extern VAR(Com_TxSignalGroupRunTimeStateType, COM_VAR) Com_TxSignalGroupState[COM_TXSIGNALGROUP_NUMBER];
#endif

#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ENABLE)
extern VAR(uint16, COM_VAR) Com_RxSignalTimeOutCnt[COM_RXSIG_TIMEOUT_NUMBER];
#endif

#if (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ENABLE)
extern VAR(uint16, COM_VAR) Com_RxSignalGroupTimeOutCnt[COM_RXSIGGRP_TIMEOUT_NUMBER];
#endif

#if (COM_ONEEVERYNFILTERSIGNAL_NUMBER > 0u)
extern VAR(uint32, COM_VAR) Com_OneEveryNcnt[COM_ONEEVERYNFILTERSIGNAL_NUMBER];
#endif

#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
extern VAR(boolean, COM_VAR) Com_MaskNewDifferMaskOldTimeOut[COM_RXMASKNEWDIFFERMASKOLD_NUMBER];
#endif

#if (COM_SIGNAL_BOOL_SUBSTITUTE_SIZE > 0u)
extern CONST(boolean, COM_CONST) Com_SignalBoolSubstituteValue[COM_SIGNAL_BOOL_SUBSTITUTE_SIZE];
#endif /* COM_SIGNAL_BOOL_SUBSTITUTE_SIZE > 0u */

#if (COM_SIGNAL_8BIT_SUBSTITUTE_SIZE > 0u)
extern CONST(uint8, COM_CONST) Com_Signal8BitSubstituteValue[COM_SIGNAL_8BIT_SUBSTITUTE_SIZE];
#endif /* COM_SIGNAL_8BIT_SUBSTITUTE_SIZE > 0u */

#if (COM_SIGNAL_16BIT_SUBSTITUTE_SIZE > 0u)
extern CONST(uint16, COM_CONST) Com_Signal16BitSubstituteValue[COM_SIGNAL_16BIT_SUBSTITUTE_SIZE];
#endif /* COM_SIGNAL_16BIT_SUBSTITUTE_SIZE > 0u */

#if (COM_SIGNAL_32BIT_SUBSTITUTE_SIZE > 0u)
extern CONST(uint32, COM_CONST) Com_Signal32BitSubstituteValue[COM_SIGNAL_32BIT_SUBSTITUTE_SIZE];
#endif /* COM_SIGNAL_32BIT_SUBSTITUTE_SIZE > 0u */

#if (COM_SIGNAL_64BIT_SUBSTITUTE_SIZE > 0u)
extern CONST(uint64, COM_CONST) Com_Signal64BitSubstituteValue[COM_SIGNAL_64BIT_SUBSTITUTE_SIZE];
#endif /* COM_SIGNAL_64BIT_SUBSTITUTE_SIZE > 0u */
/*******************************************************************************
**                      Internal Global Functions                             **
*******************************************************************************/
/*********************************************************************
**Internal public functions definition in Com_Internal.c *************
**for Com.c,Com_GwInternal.c,Com_TxInternal.c,Com_RxInternal.c********
**********************************************************************/

#if ((0u < COM_IPDUGROUP_NUMBER) || (0u < COM_RXIPDU_NUMBER))
/*init signal/group signal buffer*/
extern FUNC(void, COM_CODE)
    Com_InitSignalBuffer(Com_SignalType SignalType, uint16 SignalInitValueId, uint16 SignalLength);
#endif
/* Called by
 * Com_UpdateTxBufferAndCalculateTMCOfEveryGroupSignal,Com_RxSignalUnPack,Com_SourceSignalNotArrayUnPack
 * unpack all byte value(group signal/signal cover,except MSB byte). */
#if (0u < COM_RXIPDUBUFF_SIZE) || ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (COM_TXSIGNALGROUP_NUMBER > 0u))
extern FUNC(uint64, COM_CODE) Com_SignalUnPackHandle(
#if (STD_ON == COM_SIGNAL_SIGNED_TYPE_ENABLE)
    Com_SignalType SignalType,
#endif
    Com_SignalConfigType SignalConfigType,
    const void* SignalPtr,
    const uint8* IPduBufferPtr);
#endif /* 0u < COM_RXIPDUBUFF_SIZE) || ((STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && \
          (COM_TXSIGNALGROUP_NUMBER > 0u */
/* called by Com_TxSignalPack,Com_TxGroupSignalPack.
 * pack signal which signal type is not boolean,UINT8_N,UINT8_DYN,and the signal value bits cover 2-9 bytes*/
#if ((0u < COM_TXSIGNALGROUP_NUMBER) || (0u < COM_TXSIGNAL_NUMBER) || (0u < COM_GWMAPPING_NUMBER))
extern FUNC(void, COM_CODE) Com_TxSignalPackHandle(
    Com_SignalConfigType SignalConfigType,
    const void* TxSignalPtr,
    uint64 TxValue,
    uint8* IPduBufferPtr);
#endif
/* called by Com_TxSignalTMCCalculate,Com_DestSignalTMCCalculate.
 * when FilterAlgorithm is NEW_IS_WITHIN or NEW_IS_WITHOUT,calculate the signal's TMC */
#if (COM_TXNEWISWITHIN_NUMBER > 0u) || (COM_RXNEWISWITHIN_NUMBER > 0u) || (COM_RXNEWISOUTSIDE_NUMBER > 0u) \
    || (COM_TXNEWISOUTSIDE_NUMBER > 0U)
extern FUNC(boolean, COM_CODE) Com_NewIsWInWOtFilterCalculate(
    Com_SignalType SignalType,
    uint64 SignalValue,
    Com_FilterMaxType ComFilterMax,
    Com_FilterMinType ComFilterMin,
    boolean Within);
#endif
/* called by Com_TxSignalTMHandle,Com_TxIpduTMSHandle.
 * calculate the tx ipdu TMS(signal/Group signal/Dest signal)*/
#if (                                                                                           \
    (0u < COM_TXSIGNAL_NUMBER) || ((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER)) \
    || (0u < COM_GWMAPPING_NUMBER))                                                             \
    && (STD_ON == COM_TMS_ENABLE)
extern FUNC(boolean, COM_CODE) Com_TxIpduTMSCalculate(const Com_TxIPduType* TxIpduPtr);
#endif

/*calculate the TMS,store new Tx mode,get rptNum value for DIRECT/MIXED tx mode,
 *if TMS changed will reset periodic mode counter.*/
#if (0u < COM_TXIPDU_NUMBER) || (0u < COM_GWMAPPING_NUMBER)
extern FUNC(void, COM_CODE)
    Com_TxIpduTMSHandle(Com_TxIPduRunTimeStateType* TxIpduStatePtr, const Com_TxIPduType* TxIpduPtr, uint16* RptNum);
#endif
/*called by Com_SendSignal,Com_SendDynSignal,Com_SendSignalGroup,Com_SendSignalGroupArray.
 * set tx signal update bit to 1*/
#if (0u < COM_TXIPDU_NUMBER)                                                                          \
    && ((STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) \
        || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE))
extern FUNC(void, COM_CODE) Com_SetSignalUpdateBit(
    Com_TxIpduBufIdType IpduBufferIndex,
    Com_SignalPositionType UpdateBitPosition,
    uint8 UpdateBitMask);
#endif /* 0u < COM_TXIPDU_NUMBER) && ((STD_ON == COM_TX_SIGNAL_UPDATE_BIT_ENABLE) || (STD_ON == \
          COM_TX_SIG_GROUP_UPDATE_BIT_ENABLE) || (STD_ON == COM_GW_DEST_SIG_UPDATE_BIT_ENABLE */
/* called by Com_MainFunctionTx,Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData,
 * Com_RxPduCounterValid,Com_RxPduReplicationValid.
 * update the expected counter*/
#if ((0u < COM_TXIPDU_NUMBER) || (0u < COM_RXIPDU_NUMBER)) \
    && ((STD_ON == COM_RX_IPDU_COUNTER_ENABLE) || (STD_ON == COM_TX_IPDU_COUNTER_ENABLE))
extern FUNC(void, COM_CODE) Com_PduCounterIncrease(boolean IsTxPdu, PduIdType IpduId);
#endif

/* called by Com_RxSignalFilter.
 * when FilterAlgorithm is MASKED_NEW_DIFFERS_MASKED_OLD,calculate the signal's TMC */
#if (COM_RXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
extern FUNC(boolean, COM_CODE) Com_MaskedNewDiffersMaskedOldFilterCalculate(
    Com_SignalType SignalType,
    uint64 SignalValue,
    uint16 SignalBufferId,
    Com_FilterMaskType ComFilterMask);
#endif

/* substitute signal/group signal buffer */
#if (STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE) \
    || (STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE)
extern FUNC(void, COM_CODE) Com_SubstituteSignalBuffer(
    Com_SignalType SignalType,
    uint16 SignalInitValueId,
    uint16 SignalLength,
    uint16 SignalSubstituteValueId);
#endif /* STD_ON == COM_RX_SIGNAL_TIMEOUT_ACTION_SUBSTITUTE_ENABLE \
    || STD_ON == COM_RX_SIG_GROUP_TIMEOUT_ACTION_SUBSTITUTE_ENABLE */

/*Init Runtime TMC of all tx signals and tx group siganls
 *and GW dest signals,which the comfilter is not NULL*/
extern FUNC(void, COM_CODE) Com_TxSignalTMCStateInit(void);

#if (0u < COM_TXSIGNALGROUP_NUMBER)
/*set all tx signal group state to don't need to
 *trigger transmit*/
extern FUNC(void, COM_CODE) Com_TxSignalGroupTriggerFlagInit(void);
#endif

#if (COM_IPDUGROUP_MAX > 0u) && (COM_TXIPDUGROUP_NUMBER > 0u)
/*used to handle Tx Ipdu state change or not*/
extern FUNC(void, COM_CODE) Com_TxIpduController(boolean initialize);
#endif

/* called by Com_SendDynSignal,Com_InvalidateSignalGroup.
 * send dynamic Tx group signal handle*/
#if (COM_TXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE)
extern FUNC(uint8, COM_CODE) Com_SendTxDynGroupSignalHandle(
    Com_SignalIdType TxGroupSignalId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    uint16 DynLength);
#endif /* COM_TXSIGNALGROUP_NUMBER > 0u) && (STD_ON == COM_TX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE */
/* called by Com_TxConfirmation,Com_TpTxConfirmation,Com_MainFunctionTx.
 * Tx notification for all signals/signal groups of the Tx Pdu*/
#if (STD_ON == COM_TX_SIGNAL_NOTIFICATION_ENABLE) || (STD_ON == COM_TX_SIG_GROUP_NOTIFICATION_ENABLE)
extern FUNC(void, COM_CODE) Com_ConfirmationProcess(const Com_TxIPduType* TxIpduPtr);
#endif
/*Receive Dynamic Group Signal handle*/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
#if (((COM_RXSIGNALGROUP_NUMBER > 0u) && (COM_RXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_RXGROUPSIGNAL_8BITBUFF_SIZE)) \
    && (STD_ON == COM_RX_GRP_SIGNAL_TYPE_UINT8_DYN_ENABLE) && (0u < COM_RXIPDU_NUMBER)
extern FUNC(uint8, COM_CODE) Com_ReceiveDynGroupSignalHandle(
    Com_SignalIdType SignalId,
    P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    P2VAR(uint16, AUTOMATIC, COM_APPL_CONST) Length);
#endif
/*********************************************************************
**Internal public functions definition in Com_RxInternal.c for Com.c**
**********************************************************************/
#if (0u < COM_RXIPDU_NUMBER)
/* called by Com_IpduGroupControl.
 * used to handle Rx Ipdu state change or not*/
#if (COM_IPDUGROUP_MAX > 0u) && (COM_RXIPDUGROUP_NUMBER > 0u)
extern FUNC(void, COM_CODE) Com_RxIpduController(boolean initialize);
#endif
/* called by Com_SignalGroupRxIndication,Com_TpPduInvalidAction.
 * judge the rx signal group value is filter out or not*/
#if (COM_RXGRPSIG_FILTERTYPE_MAX_NUMBER > 0u)
extern FUNC(boolean, COM_CODE) Com_RxSignalGroupFilter(
    const Com_RxSignalGroupType* RxSignalGroupPtr,
    boolean InvalidSignalGroup,
    Com_RxIpduBufIdType IpduBufferId);
#endif
#endif /*0u < COM_RXIPDU_NUMBER*/
/*********************************************************************
**Internal public functions definition in Com_GwInternal.c for Com.c**
**********************************************************************/
#if (0u < COM_TMCTXSIGNAL_NUMBER)
/* calculate dest signal/group signal/description signal TMC*/
extern FUNC(boolean, COM_CODE) Com_DestSignalTMCCalculate(
    Com_SignalType SignalType,
    Com_FilterAlgorithmType FilterType,
    Com_SignalIdType ComFilterIndex,
#if (COM_TXMASKNEWDIFFERMASKOLD_NUMBER > 0u)
    uint64 OldSignalValue,
#endif
    uint64 NewSignalValue);
#endif

/*********************************************************************
**Internal public functions definition in Com_TxInternal.c          **
**********************************************************************/
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && (0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER)
/*Send SignalGroup(Array) handle*/
extern FUNC(uint8, COM_CODE) Com_SendSignalGroupArrayHandle(
    Com_SignalGroupIdType SignalGroupId,
    P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) SignalGroupArrayPtr);
#endif
#if (((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u)) && (0u < COM_TXIPDU_NUMBER))
/*Send Signal handle*/
extern FUNC(uint8, COM_CODE)
    Com_SendSignalHandle(Com_SignalIdType SignalId, P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr);
/*Send Dynamic Signal handle*/
extern FUNC(uint8, COM_CODE) Com_SendDynSignalHandle(
    Com_SignalIdType SignalId,
    P2CONST(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    uint16 Length);
#endif
/*Send SignalGroup handle*/
#if ((0u < COM_TXSIGNALGROUP_NUMBER) && (0u < COM_TXIPDU_NUMBER))
extern FUNC(uint8, COM_CODE) Com_SendSignalGroupHandle(Com_SignalGroupIdType SignalGroupId);
#endif
/*Send Invalidate Signal handle*/
#if (0u < COM_TXIPDU_NUMBER) && ((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u))
extern FUNC(uint8, COM_CODE) Com_InvalidateSignalHandle(Com_SignalIdType SignalId);
#endif
/*Send Invalidate SignalGroup handle*/
#if (0u < COM_TXIPDU_NUMBER) && ((COM_TXSIGNAL_NUMBER > 0u) || (COM_TXGROUPSIGNAL_NUMBER > 0u))
extern FUNC(uint8, COM_CODE) Com_InvalidateSignalGroupHandle(Com_SignalGroupIdType SignalGroupId);
#endif
/*Called by Com_TriggerIPDUSend,Com_TriggerIPDUSendWithMetaData.
 *Trigger IPdu Send handle*/
#if (0u < COM_TXIPDU_NUMBER)
extern FUNC(Std_ReturnType, COM_CODE) Com_TriggerIPDUSendHandle(PduIdType PduId);
#endif
/*Trigger IPDU(with metadata) send handle*/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
#if (STD_ON == COM_METADATA_SUPPORT)
extern FUNC(Std_ReturnType, COM_CODE)
    Com_TriggerIPDUSendWithMetaDataHandle(PduIdType PduId, P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) MetaData);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif
/*Switch Ipdu Transmit Mode Handle*/
#if (0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TMS_ENABLE)
extern FUNC(void, COM_CODE) Com_SwitchIpduTxModeHandle(PduIdType PduId, boolean Mode);
#endif
/*TxPdu MainFunction handle*/
#if (0u < COM_TXIPDU_NUMBER)
extern void Com_MainFunctionTxHandle(Com_MainFunctionType mainFunctionId);
#endif
/*TxPdu Trigger Transmit handle*/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
#if (0u < COM_TXIPDU_NUMBER)
extern FUNC(Std_ReturnType, COM_CODE)
    Com_TriggerTransmitHandle(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
#endif
/*IF TxPdu TxConfirmation handle*/
#if (0u < COM_TXIPDU_NUMBER)
extern FUNC(void, COM_CODE) Com_TxConfirmationHandle(PduIdType TxPduId);
#endif
/*TP TxPdu TxConfirmation handle*/
#if ((0u < COM_TXIPDU_NUMBER) && (STD_ON == COM_TXTPPDU_SUPPORT))
extern FUNC(void, COM_CODE) Com_TpTxConfirmationHandle(PduIdType id, Std_ReturnType result);
/*TP TxPdu copy pdu data handle*/
extern FUNC(BufReq_ReturnType, COM_CODE) Com_CopyTxDataHandle(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    P2CONST(RetryInfoType, AUTOMATIC, COM_APPL_DATA) retry,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) availableDataPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif
/*********************************************************************
**Internal public functions definition in Com_RxInternal.c for Com.c**
**********************************************************************/
/*Called by Com_ReceiveSignal.
 *Receive group signal handle*/
#if (0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXGROUPSIGNAL_NUMBER) && (0u < COM_RXIPDU_NUMBER)
extern FUNC(uint8, COM_CODE)
    Com_ReceiveGroupSignalHandle(Com_SignalIdType SignalId, P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr);
#endif
/*Called by Com_ReceiveSignal.
 *Receive signal handle*/
#if (0u < COM_RXSIGNAL_NUMBER) && (0u < COM_RXIPDU_NUMBER)
extern FUNC(uint8, COM_CODE)
    Com_ReceiveSignalHandle(Com_SignalIdType SignalId, P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr);
#endif
/*Called by Com_ReceiveSignalGroup.
 *Receive all group signal of the SignalGroup*/
#if ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXIPDU_NUMBER))
extern FUNC(void, COM_CODE) Com_ReceiveSignalGroupHandle(PduIdType RxPduId, Com_SignalGroupIdType SignalGroupId);
#endif
#if (STD_ON == COM_ENABLE_SIGNAL_GROUP_ARRAY_API) && ((0u < COM_RXSIGNALGROUP_NUMBER) && (0u < COM_RXIPDU_NUMBER))
/*receive SignalGroup(Array) data handle*/
extern FUNC(uint8, COM_CODE) Com_ReceiveSignalGroupArrayHandle(
    Com_SignalGroupIdType SignalGroupId,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint8, AUTOMATIC, COM_APPL_CONST) SignalGroupArrayPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif
/*Called by Com_ReceiveDynSignal.
 *Receive Dynamic Signal handle*/
#if ((0u < COM_RXSIGNAL_NUMBER) && (0u < COM_SIGNAL_8BITBUFF_SIZE)) && (STD_ON == COM_RX_SIGNAL_TYPE_UINT8_DYN_ENABLE) \
    && (0u < COM_RXIPDU_NUMBER)
extern FUNC(uint8, COM_CODE) Com_ReceiveDynSignalHandle(
    Com_SignalIdType SignalId,
    P2VAR(void, AUTOMATIC, COM_APPL_CONST) SignalDataPtr,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(uint16, AUTOMATIC, COM_APPL_CONST) Length);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif

/*RxPdu RxIndication handle*/
#if (0u < COM_RXIPDU_NUMBER)
extern FUNC(void, COM_CODE)
    Com_RxIndicationHandle(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
#endif
/*TP RxPdu RxIndication handle*/
#if (STD_ON == COM_RXTPPDU_SUPPORT) && (0u < COM_RXIPDU_NUMBER)
extern FUNC(void, COM_CODE) Com_TpRxIndicationHandle(PduIdType id, Std_ReturnType result);
/*TP Pdu copy receive data handle*/
extern FUNC(BufReq_ReturnType, COM_CODE) Com_CopyRxDataHandle(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) bufferSizePtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/*TP Pdu Start receive handle*/
extern FUNC(BufReq_ReturnType, COM_CODE) Com_StartOfReceptionHandle(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) info,
    PduLengthType TpSduLength,
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
    P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) bufferSizePtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif
/*RxPdu MainFunction handle*/
#if (0u < COM_RXIPDU_NUMBER)
extern void Com_MainFunctionRxHandle(Com_MainFunctionType mainFunctionId);
#endif
/*********************************************************************
**Internal public functions definition in Com_GwInternal.c for Com.c**
**********************************************************************/
/*Signal gateway handle*/
#if COM_GWMAPPING_NUMBER > 0u
extern void Com_MainFunctionRouteSignalsHandle(Com_MainFunctionType mainFunctionId);
extern void Com_GwInitDestinationRequest(void);
extern void Com_GwUpdateDestinationRequest(Com_GwMappingIdType gwMappingId, boolean request);
#endif
#if (COM_RXIPDUGROUP_NUMBER > 0u) && (0u < COM_RXIPDU_NUMBER)
/*Rx Pdu DM timeout enable/disable handle*/
extern FUNC(void, COM_CODE) Com_RxPduDMCtrHandle(void);
#endif

#endif /*COM_INTERNAL_H*/
/*******************************************************************************
**                      End of file                                           **
*******************************************************************************/
