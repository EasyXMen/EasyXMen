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
 **  @file               : WdgM_CfgType.h
 **  @author             : xudong.guan
 **  @date               : 2024/08/20
 **  @vendor             : isoft
 **  @description        : WdgM config type definition header file.
 **
 ***********************************************************************************************************************/

#ifndef WDGM_CFGTYPE_H_
#define WDGM_CFGTYPE_H_

/* =================================================== inclusions =================================================== */
#include "WdgIf_Types.h"
#include "WdgM_Type.h"
#include "Os.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================ type definitions ================================================ */
/*Configuration type of internal logical transition */
typedef struct WdgM_IntTransitionTypeTag
{
    /* Source checkpoint id in internal transition */
    WdgM_CheckpointIdType WdgMTransitionSourCPID;
    /* Destination checkpoint id in internal transition */
    WdgM_CheckpointIdType WdgMTransitionDestCPID;
} WdgM_IntTransitionType;

/*Configuration type of external logical transition */
typedef struct WdgM_ExtTransitionTypeTag
{
    /* Source Supervised Entity id in external transition */
    WdgM_SupervisedEntityIdType WdgMTransitionSourSEID;
    /* Source checkpoint id in external transition */
    WdgM_CheckpointIdType WdgMTransitionSourCPID;
    /* Destination Supervised Entity id in external transition */
    WdgM_SupervisedEntityIdType WdgMTransitionDestSEID;
    /* Destination checkpoint id in external transition */
    WdgM_CheckpointIdType WdgMTransitionDestCPID;
} WdgM_ExtTransitionType;

/*Configuration type of trigger condition */
typedef struct WdgM_TriggerTypeTag
{
    /* WdgM mode mapping to WdgIf mode */
    WdgIf_ModeType WdgMWatchdogMode;
    /* Trigger condition value pass to WdgIf */
    uint16 WdgMTriggerConditionValue;
    /* WdgM device mapping to WdgIf device */
    WdgM_WatchdogDeviceType WdgMWatchdogDevice;
} WdgM_TriggerType;

/*Configuration type of alive supervision */
typedef struct WdgM_AliveSupCfgTypeTag
{
    /* Alive supervision id */
    WdgM_AliveSupIDType WdgMAliveSupID;
    /* Alive supervision excepted indication counter */
    uint16 WdgMExpectedAliveSupIndication;
    /* Alive supervision reference cycle counter */
    uint16 WdgMSupRefCycle;
    /* Alive supervsion max margin */
    uint8 WdgMMaxMargin;
    /* Alive supervsion min margin */
    uint8 WdgMMinMargin;
} WdgM_AliveSupCfgType;

/*configuration type of deadline supervision */
typedef struct WdgM_DeadlineSupCfgTypeTag
{
    /* Deadline supervision id */
    WdgM_DeadlineSupIDType WdgMDeadlineSupID;
    /* Deadline start checkpoint id */
    WdgM_CheckpointIdType WdgMDeadlineStartRef;
    /* Deadline end checkpoint id */
    WdgM_CheckpointIdType WdgMDeadlineStopRef;
    /* Deadline max tolerable time span, Uint:s */
    WdgM_DeadlineTimeFloatType WdgMDeadlineMax;
    /* Deadline min tolerable time span, Uint:s */
    WdgM_DeadlineTimeFloatType WdgMDeadlineMin;
    /* Flag if timeout detection is enabled */
    boolean WdgMEnableTimeoutDetection;
} WdgM_DeadlineSupCfgType;

/*Configuration type of internal logical supervision */
typedef struct WdgM_IntLogSupCfgTypeTag
{
    /* Internal logical supervision id */
    WdgM_LogSupIDType WdgMIntLogSupID;
    /* Internal transition quantity */
    uint16 WdgMIntTransitionCnt;
    /* Internal transition struct address*/
    const WdgM_IntTransitionType* WdgMIntTransition;
} WdgM_IntLogSupCfgType;

/*Configuration type of external logical supervision */
typedef struct WdgM_ExtLogSupCfgTypeTag
{
    /* External logical supervision id */
    WdgM_LogSupIDType WdgMExtLogSupID;
    /* Supervised Entity quantity related to this supervision */
    uint16 SEIdRelatedCnt;
    /* Supervised Entity id impacted by this supervision */
    const WdgM_SupervisedEntityIdType* WdgMSEIdImpacted;
    /* External transition quantity */
    uint16 WdgExtTransitionCnt;
    /* External transition struct address */
    const WdgM_ExtTransitionType* WdgMExtTransition;
} WdgM_ExtLogSupCfgType;

/*Configuration type of checkpoint supporting deadline logical supervision */
typedef struct WdgM_CPSupportDeadLineInfoTypeTag
{
    /* Deadline supervision id */
    WdgM_DeadlineSupIDType WdgM_DeadlineSupID;
    /* Checkpoint group start or stop */
    WdgM_CPSupportDeadlineGroupType WdgMCPSupportDeadlineGroup;
} WdgM_CPSupportDeadLineInfoType;

/*Configuration type of checkpoint */
typedef struct WdgM_CheckpointCfgTypeTag
{
    /* Checkpoint id */
    WdgM_CheckpointIdType WdgMCheckpointID;
    /* While kind of supervision be supported by this checkpoint */
    WdgM_SupSupportGroupType WdgMSupSupportGroup;
    /* Alive supervision id if supports*/
    WdgM_AliveSupIDType WdgM_AliveSupID;
    /* Deadline supervision quantity if supports */
    uint16 WdgM_CPSupportDeadLineCnt;
    /* Deadline supervision info struct pointer */
    const WdgM_CPSupportDeadLineInfoType* WdgM_CPSupportDeadLineInfo;
    /* External logical supervision id if supports */
    WdgM_LogSupIDType WdgMExtLogSupID;
} WdgM_CheckpointCfgType;

/*configuration Type of supervision entity */
typedef struct WdgM_SECfgTypeTag
{
    /*WdgM Supervised Entity id*/
    WdgM_SupervisedEntityIdType WdgMSupervisedEntityID;
    /*Checkpoint config struct pointer associated with current supervised entity*/
    const WdgM_CheckpointCfgType* WdgMCheckpointRelated;
    /*Quantity of alive supervision in current supervised entity*/
    uint16 SEAliveCnt;
    /*Alive supervision config struct pointer associated with current supervised entity*/
    const WdgM_AliveSupCfgType* WdgMAliveSupCfg;
    /*Quantity of deadline supervision in current supervised entity*/
    uint16 SEDealineCnt;
    /*Deadline supervision config struct pointer associated with current supervised entity*/
    const WdgM_DeadlineSupCfgType* WdgMDeadlineSupCfg;
    /*Quantity of internal logical supervision in current supervised entity*/
    uint16 SEIntLogCnt;
    /*Acceptable quantity of the SE cycle in incorrect/failed status*/
    const uint8 WdgMFailedAliveSupervisionRefCycleTol;
} WdgM_SECfgType;

/*Configuration type of WdgM Mode */
typedef struct WdgM_ModeCfgTypeTag
{
    /* Mode id */
    WdgM_ModeType WdgMModeID;
    /*Expired supervision cycle tolerance in mode*/
    uint16 WdgMExpiredSupervisionCycleTol;
    /*Quantity of supervised entity within mode*/
    uint16 ModeSECnt;
    /*Supervised entity reference struct pointer within mode*/
    const WdgM_SECfgType* WdgMSERelated;
    /*Quantity of external logical supervision within mode*/
    uint16 ModeExtLogCnt;
    /*External logical supervision reference struct pointer within mode*/
    const WdgM_ExtLogSupCfgType* WdgMExtLogSupRelated;
    /*Quantity of trigger within mode*/
    uint8 ModeTriggerCnt;
    /*Trigger reference struct pointer within mode*/
    const WdgM_TriggerType* WdgMTriggerRelated;
    /*Mode mainfunction period except tick */
    const uint8 WdgMMainFunctionExceptTick;
} WdgM_ModeCfgType;

typedef struct WdgM_TabCntCfgTypeTag
{
    uint16 WdgM_AliveTabMaxCnt;
    uint16 WdgM_DeadlineTabMaxCnt;
    uint16 WdgM_ExternalTabMaxCnt;
} WdgM_TabCntCfgType;

typedef struct
{
    const WdgM_TabCntCfgType* WdgMTabCfg;
    WdgM_AliveSupInfoType*    WdgMAliveSupervisionInfo;
    WdgM_DeadlineSupInfoType* WdgMDeadlineSupervisionInfo;
    WdgM_ExtLogSupInfoType*   WdgMExtLogicalSupervisionInfo;
    WdgM_ModeInfoType*        WdgMModeInfo;
    WdgM_DemReportStatustype* WdgMDemStatus;
    WdgM_GlobalStatusType*    WdgMGlobalStatus;
} WdgM_RuntimeType;

#ifdef __cplusplus
}
#endif

#endif /* WDGM_CFGTYPE_H_ */
