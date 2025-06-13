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
/*********************************************************************************
**                                                                            **
**  FILENAME    : WdgM_CfgType.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : haibin.shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/
/*======================[R E V I S I O N   H I S T O R Y]=====================*/
/*============================================================================*/

#ifndef WDGM_CFGTYPE_H_
#define WDGM_CFGTYPE_H_

/**********************************************/

#include "WdgM_Type.h"
#include "WdgIf_Types.h"
#include "Os.h"

/****************************** references *********************************/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /***************************************************************/

    /*configuration Type of internal logical transition */
    typedef struct
    {
        /*CP_ID for Final E_CP*/
        WdgM_CheckpointIdType WdgMTransitionSourCPID;
        /*CP_ID for Initial E_CP*/
        WdgM_CheckpointIdType WdgMTransitionDestCPID;
    } WdgM_IntTransitionType;

    /*configuration Type of external logical transition */
    typedef struct
    {
        WdgM_SupervisedEntityIdType WdgMTransitionSourSEID;
        /*CP_ID for Final E_CP*/
        WdgM_CheckpointIdType WdgMTransitionSourCPID;
        WdgM_SupervisedEntityIdType WdgMTransitionDestSEID;
        /*CP_ID for Initial E_CP*/
        WdgM_CheckpointIdType WdgMTransitionDestCPID;
    } WdgM_ExtTransitionType;

    /*configuration Type of trigger condition */
    typedef struct
    {
        WdgIf_ModeType WdgMWatchdogMode;
        /*Parameter passed to the WdgIf_SetTriggerCondition*/
        uint16 WdgMTriggerConditionValue;
        WdgM_WatchdogDeviceType WdgM_WatchdogDevice;
    } WdgM_TriggerType;

    /*configuration Type of alive supervision */
    typedef struct
    {
        WdgM_AliveSupIDType WdgMAliveSupID;
        /* Expected AI */
        WdgM_AliveSupIDType WdgMExpectedAliveSupIndication;
        /* CP period */
        uint16 WdgMSupRefCycle;
        uint8 WdgMMaxMargin;
        uint8 WdgMMinMargin;
    } WdgM_AliveSupCfgType;

    /*configuration Type of deadline supervision */
    typedef struct
    {
        WdgM_DeadlineSupIDType WdgMDeadlineSupID;
        /*CP_ID for start Deadline*/
        WdgM_CheckpointIdType WdgMDeadlineStartRef;
        /*CP_ID for stop Deadline*/
        WdgM_CheckpointIdType WdgMDeadlineStopRef;
        /*Max time span. Uint:s*/
        WdgM_DeadlineTimeFloatType WdgMDeadlineMax;
        /*Min time span. Uint:s*/
        WdgM_DeadlineTimeFloatType WdgMDeadlineMin;
        boolean WdgMEnableTimeoutDetection;
    } WdgM_DeadlineSupCfgType;

    /*configuration Type of internal logical supervision */
    typedef struct
    {
        WdgM_LogSupIDType WdgMIntLogSupID;
        uint16 WdgMIntTransitionCnt;
        P2CONST(WdgM_IntTransitionType, TYPEDEF, WDGM_CONST) WdgMIntTransition;
    } WdgM_IntLogSupCfgType;

    /*configuration Type of external logical supervision */
    typedef struct
    {
        WdgM_LogSupIDType WdgMExtLogSupID;
        uint16 SEIdRelatedCnt;
        P2CONST(WdgM_SupervisedEntityIdType, TYPEDEF, WDGM_CONST) WdgMSEIdImpacted;
        uint16 WdgExtTransitionCnt;
        P2CONST(WdgM_ExtTransitionType, TYPEDEF, WDGM_CONST) WdgMExtTransition;
    } WdgM_ExtLogSupCfgType;

    /*configuration Type of checkpoint supporting deadline logical supervision */
    typedef struct
    {
        WdgM_DeadlineSupIDType WdgM_DeadlineSupID;
        uint16 WdgMCPSupportDeadlineGroup;
    } WdgM_CPSupportDeadLineInfoType;

    /*configuration Type of checkpoint */
    typedef struct
    {
        WdgM_CheckpointIdType WdgMCheckpointID;
        /* AliveSupport,DeadLineSupport,LogSupCpSupport,IntExtLogSupCP,LogSupInitCP,LogSupFinalCP */
        uint16 WdgMSupSupportGroup;
        /*One CP maps to one AliveSup*/
        WdgM_AliveSupIDType WdgM_AliveSupID;
        /*One CP maps to several DeadlineSups*/
        uint16 WdgM_CPSupportDeadLineCnt;
        P2CONST(WdgM_CPSupportDeadLineInfoType, TYPEDEF, WDGM_CONST) WdgM_CPSupportDeadLineInfo;
        /*One CP maps to one LogSup*/
        WdgM_LogSupIDType WdgMLogSupID;
    } WdgM_CheckpointCfgType;

    /*configuration Type of supervision entity */
    typedef struct
    {
        /*Id for Supervised Entity req371*/
        WdgM_SupervisedEntityIdType WdgMSEId;
        uint16 SECPCnt;
        P2CONST(WdgM_CheckpointCfgType, TYPEDEF, WDGM_CONST) WdgMCheckpointRelated;
        uint16 SEAliveCnt;
        P2CONST(WdgM_AliveSupCfgType, TYPEDEF, WDGM_CONST) WdgMAliveSupCfg;
        uint16 SEDealineCnt;
        P2CONST(WdgM_DeadlineSupCfgType, TYPEDEF, WDGM_CONST) WdgMDeadlineSupCfg;
        uint16 SEIntLogCnt;
        P2CONST(WdgM_IntLogSupCfgType, TYPEDEF, WDGM_CONST) WdgMIntLogSupCfg;
        /*Acceptable quantity of the SE cycle in incorrect/failed status*/
        uint8 WdgMFailedAliveSupervisionRefCycleTol;
        /*EcucPartition Symboic name , Invalue 0xFF*/
        uint16 WdgMEcucPartitionRef;
        /*The Wdgm reset partition reference*/
        boolean WdgMResetPartitionRef;
        /*The WdgM Os Counter reference*/
        uint16 WdgMOsCounter;
        /*The WdgM Os Counter Max Value*/
        uint32 WdgMOsCounterMaxValue;
        /*The WdgM Os Counter resolution*/
        uint32 WdgMOsCounterResolution;
    } WdgM_SECfgType;

    /*configuration Type of WdgM Mode */
    typedef struct
    {
        WdgM_ModeType WdgMModeID;
        /*Expired hold tolerance*/
        uint16 WdgMExpiredSupervisionCycleTol;
        /*Quantity of WdgMLocalStatusParams*/
        uint16 ModeSECnt;
        /*Total of the WdgMLocalStatusSupervisedEntityRef*/
        P2CONST(WdgM_SECfgType, TYPEDEF, WDGM_CONST) WdgMSERelated;
        uint16 ModeExtLogCnt;
        P2CONST(WdgM_ExtLogSupCfgType, TYPEDEF, WDGM_CONST) WdgMExtLogSupRelated;
        uint8 ModeTriggerCnt;
        P2CONST(WdgM_TriggerType, TYPEDEF, WDGM_CONST) WdgMTriggerRelated;
    } WdgM_ModeCfgType;

    /*configuration Type of WdgM */
    /*DD_2_181*/
    typedef struct
    {
        WdgM_ModeType WdgMInitialModeId;
        uint8 WdgM_ModeCnt;
        P2CONST(WdgM_ModeCfgType, TYPEDEF, WDGM_CONST) WdgMMode;
    } WdgM_ConfigType;

    /***************************************************************/

    /****************************** definitions *********************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WDGM_CFGTYPE_H_ */
