/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*********************************************************************************
**                                                                            **
**  FILENAME    : WdgM_Type.h                                                 **
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

#ifndef WDGM_TYPE_H_
#define WDGM_TYPE_H_

/******************************INCLUDE*********************************/
#include "Std_Types.h"
#include "Rte_WdgM_Type.h"

#ifndef _DEFINED_TYPEDEF_FOR_WdgM_SupervisedEntityIdType_
typedef uint16 WdgM_SupervisedEntityIdType; /*DD_2_181*/
#endif
typedef uint16 WdgM_CheckpointIdType; /*DD_2_181*/
typedef uint8 WdgM_ModeType;          /*DD_2_181*/
typedef uint16 WdgM_AliveSupIDType;
typedef uint16 WdgM_DeadlineSupIDType;
typedef uint16 WdgM_LogSupIDType;
typedef uint16 WdgM_WatchdogNameType;
typedef uint8 WdgM_WatchdogDeviceType;
typedef uint16 WdgM_ExpiredSupTolCounterType;
typedef uint32 WdgM_DeadlineTimeType;
typedef float WdgM_DeadlineTimeFloatType;

/* Type of WdgM Supervision */
/*DD_2_189*/
typedef enum
{
    /* Alive Supervision */
    WDGM_ALIVE_SUP,
    /* Deadline Supervision */
    WDGM_DEADLINE_SUP,
    /* Internal logical Supervision */
    WDGM_INTLOG_SUP,
    /* External logical Supervision */
    WDGM_EXTLOG_SUP,
    WDGM_SUPERVISION_MACHNISM_MAX
} WDGM_SUPERVISION_MACHNISM_TYPE;

/* Type of WdgM initialization status */
typedef enum
{
    /*WdgM Uninit*/
    WDGM_INIT_NOT,
    /*WdgM init*/
    WDGM_INIT_OK
} WdgM_ModeInitStatusType;

/* Type of logical supervision activated status */
/*DD_2_190*/
typedef enum
{
    /* logical supervision activated */
    WDGM_AF_TRUE,
    /* logical supervision deactivated */
    WDGM_AF_FALSE
} WdgM_ActiveStatusType;

/* Type of result for each kind of Supervision*/
/*DD_2_191*/
typedef enum
{
    WDGM_CORRECT,
    WDGM_INCORRECT
} WdgM_SupResultType;

/* Type of supervision kind that the checkpoint supporting*/
/*DD_2_192*/
typedef enum
{
    /* Checkpoint support alive supervision*/
    ALIVE_SUPPORT,
    /* Checkpoint support deadline supervision*/
    DEADLINE_SUPPORT,
    /* Checkpoint support logical supervision*/
    LOG_SUPPORT,
    /* Checkpoint support internal logical supervision*/
    LOG_SUPPORT_INT_OR_EXT,
    /* Checkpoint support initial checkpoint for logical supervision*/
    LOG_SUP_SUPPORT_INIT,
    /* Checkpoint support final checkpoint for logical supervision*/
    LOG_SUP_SUPPORT_FINAL
} WdgM_SupSupportGroupType;

/* Type of start or stop deadline checkpoint kind that the checkpoint supporting*/
/*DD_2_193*/
typedef enum
{
    /* Checkpoint support start checkpoint for deadline supervision*/
    CP_DEADLINE_SUPPORT_START,
    /* Checkpoint support stop checkpoint for deadline supervision*/
    CP_DEADLINE_SUPPORT_STOP
} WdgM_CPSupportDeadlineGroupType;

/* Type of external supervision*/
/*DD_2_194*/
typedef struct
{
    WdgM_ActiveStatusType WdgMExtLogActiveFlag;
    WdgM_CheckpointIdType WdgMExtLastReachedCPID;
    WdgM_SupervisedEntityIdType WdgMExtLastReachedSEID;
    WdgM_SupResultType WdgMExtLogSupResult;
} WdgM_ExtLogSupInfoType;

/* Type of Alive supervision*/
/*DD_2_195*/
typedef struct
{
    /*storing the time when that Checkpoint has been reached*/
    uint16 WdgMAliveCycleCounter;
    uint16 WdgMAliveIndicationCounter;
    WdgM_SupResultType WdgMAliveSupResult;
} WdgM_AliveSupInfoType;

/* Type of deadline supervision*/
/*DD_2_196*/
typedef struct
{
    /*storing the time when that Checkpoint has been reached*/
    WdgM_DeadlineTimeType WdgMTimeStampStart;
    WdgM_SupResultType WdgMDeadlineSupResult;
} WdgM_DeadlineSupInfoType;

/* Type of internal supervision*/
/*DD_2_197*/
typedef struct
{
    WdgM_ActiveStatusType WdgMIntLogActiveFlag;
    WdgM_CheckpointIdType WdgMIntLastReachedCPID;
    WdgM_SupResultType WdgMIntLogSupResult;
} WdgM_IntLogSupInfoType;

/* Type of supervision*/
/*DD_2_198*/
typedef struct
{
    WdgM_LocalStatusType WdgMLocalStatus;
    uint8 WdgMFailedAliveSupCounter;
    WdgM_SupResultType WdgMEachSupResult[WDGM_SUPERVISION_MACHNISM_MAX];
} WdgM_SEInfoType;

/* Type of Mode information*/
/*DD_2_199*/
typedef struct
{
    WdgM_ModeInitStatusType WdgMModeInitStatus;
    WdgM_ModeType WdgMModeCurrentID;
    WdgM_ExpiredSupTolCounterType WdgMExpiredSupTolCounter;
} WdgM_ModeInfoType;

/* Type of global information*/
/*DD_2_200*/
typedef struct
{
    Std_ReturnType WdgM_FirstExpiredSEIDSaved;
    WdgM_GlobalStatusType WdgMGlobalResult;
} WdgM_GlobalInfoType;

#endif /* WDGM_TYPE_H_ */
