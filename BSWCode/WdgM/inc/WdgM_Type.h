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
 **  @file               : WdgM_Type.h
 **  @author             : xudong.guan
 **  @date               : 2024/08/20
 **  @vendor             : isoft
 **  @description        : Type definition for WdgM
 **
 ***********************************************************************************************************************/

#ifndef WDGM_TYPE_H_
#define WDGM_TYPE_H_

/* =================================================== inclusions =================================================== */
#include "Rte_WdgM_Type.h"

/* ===================================================== macros ===================================================== */
#define WDGM_TIME_RESOLUTION         1000UL  /**< Watchdog Manager time resolution in milliseconds */
#define WDGM_NO_OS_APPLICAITON_REF   0xFFFFu /**< Invalid value indicate no os application */
#define WDGM_NO_CP_REACH             0xFFFFu /**< Invalid value indicate no checkpoint reached */
#define WDGM_NO_SE_REACH             0u      /**< Invalid value indicate no supervised entity reached */
#define WDGM_TRIGGER_CONDITION_STOP  0u      /**< Trigger condition for Stop*/
#define WDGM_INIT_FIRST_EXPIRED_SEID 0xFFFFu /**< Invalid value for first expired supervised entity value*/
#define WDGM_VALIED_FIRST_EXPIRED_SEID_SUM \
    0xFFFFu                                      /**< Invalid value for first expired supervised entity inverse value*/
#define WDGM_INVALIED_DEM_ID             0xFFFFu /**< Invalid DEM id to indicate without Dem*/
#define WDGM_NO_COUNTER_CONFIG           0xFFFFu /**< No counter is configured in SE */
#define WDGM_INVALIED_COUNTER_VALUE      0u      /**< Maxvalue when no counter is configured in SE */
#define WDGM_INVALIED_COUNTER_RESOLUTION 0u      /**< Resolution when no counter is configured in SE */

/* ================================================ type definitions ================================================ */
typedef uint16 WdgM_AliveSupIDType; /**< WdgM Alive Supervision ID Type. @range 0..65535 */

typedef uint16 WdgM_DeadlineSupIDType; /**< WdgM Deadline Supervision ID Type. @range 0..65535 */

typedef uint16 WdgM_LogSupIDType; /**< WdgM Logical Supervision ID Type. @range 0..65535 */

typedef uint16 WdgM_WatchdogNameType; /**< WdgM Watchdog name mapping Type. @range 0.65535 */

typedef uint8 WdgM_WatchdogDeviceType; /**< WdgM Watchdog device Type. @range 0..255 */

typedef uint16 WdgM_ExpiredSupTolCounterType; /**< WdgM Expired supervision tolerance counter Type. @range 0..65535 */

typedef uint32 WdgM_DeadlineTimeType; /**< WdgM Deadline time Type. @range 0..4294967295 */

typedef float32 WdgM_DeadlineTimeFloatType; /**< WdgM Deadline float time Type. @range -3.4028235e+38..3.4028235e+38 */

/**
 * @brief WdgM Combination status Type.
 */
typedef uint8 WdgM_CombinationStatusType;
#define WDGM_COMBINATION_OK     0x00u /**< Combinaiton status ok. @range 0u */
#define WDGM_COMBINATION_NOT_OK 0x01u /**< Combinaiton status not ok. @range 1u */

/**
 * @brief Type of supervision kind that the checkpoint supporting.
 */
typedef uint8 WdgM_SupSupportGroupType;
#define ALIVE_SUPPORT         0u /**< Checkpoint supports alive supervision flag. @range 0u */
#define DEADLINE_SUPPORT      1u /**< Checkpoint supports deadline supervision flag. @range 1u */
#define INT_LOG_SUPPORT       2u /**< Checkpoint supports internal logical supervision flag. @range 2u */
#define EXT_LOG_SUPPORT       3u /**< Checkpoint supports external logical supervision flag. @range 3u */
#define LOG_SUP_SUPPORT_INIT  4u /**< Checkpoint is start point of logical supervision flag. @range 4u */
#define LOG_SUP_SUPPORT_FINAL 5u /**< Checkpoint is end point of logical supervision flag. @range 5u */

/**
 * @brief Type of Dem status of WdgM.
 */
typedef uint8 WdgM_DemReportStatustype;
#define WDGM_DEM_REPORT_NONE   0u /**< Init status which has no dem reports. @range 0u */
#define WDGM_DEM_REPORT_FAILED 1u /**< Last reset cause is reaching STOPPED. @range 1u */
#define WDGM_DEM_REPORT_PASSED 2u /**< Last reset cause is reaching STOPPED and has been reset. @range 2u */

/**
 * @brief Type of start or stop deadline checkpoint kind that the checkpoint supports.
 */
typedef uint8 WdgM_CPSupportDeadlineGroupType;
#define CP_DEADLINE_SUPPORT_START 0u /**< Checkpoint is start point of deadline supervision. @range 0u */
#define CP_DEADLINE_SUPPORT_STOP  1u /**< Checkpoint is end point of deadline supervision. @range 1u */

/**
 * @brief WdgM combination master slave type.
 */
typedef uint8 WdgM_CombinationMasterSlaveType;
#define WDGM_COMBINATION_MASTER 0u /**< Current WdgM is master in combination. @range 0u */
#define WDGM_COMBINATION_SLAVE  1u /**< Current WdgM is slave in combination. @range 0u */

/**
 * @brief Enumeration type of supervision machnism.
 */
typedef enum
{
    WDGM_ALIVE_SUP,               /**< Alive Supervision. @range 0u */
    WDGM_DEADLINE_SUP,            /**< Deadline Supervision. @range 1u */
    WDGM_INTLOG_SUP,              /**< Internal logical Supervision. @range 2u */
    WDGM_EXTLOG_SUP,              /**< External logical Supervision. @range 3u */
    WDGM_SUPERVISION_MACHNISM_MAX /**< Invalid number. @range 4u */
} WDGM_SUPERVISION_MACHNISM_TYPE;

/**
 * @brief Enumeration type of logical supervision activated status.
 */
typedef enum
{
    WDGM_AF_FALSE, /**< Logical supervision deactivated. @range 0u */
    WDGM_AF_TRUE   /**< Logical supervision activated. @range 1u */
} WdgM_ActiveStatusType;

/**
 * @brief Enumeration type of result for each kind of Supervision.
 */
typedef enum
{
    WDGM_CORRECT,  /**< Supervision result is correct. @range 0u */
    WDGM_INCORRECT /**< Supervision result is incorrect. @range 1u */
} WdgM_SupResultType;

/**
 * @brief Struct type of external supervision.
 */
typedef struct
{
    WdgM_ActiveStatusType WdgMExtLogActiveFlag;   /**< External logical activity flag. @range 0u */
    WdgM_CheckpointIdType WdgMExtLastReachedCPID; /**< Last reached checkpoint id in external supervision. @range 1u */
    WdgM_SupervisedEntityIdType
        WdgMExtLastReachedSEID;             /**< Last reached supervised entity id in external supervision. @range 2u */
    WdgM_SupResultType WdgMExtLogSupResult; /**< External supervision result. @range 3u */
} WdgM_ExtLogSupInfoType;

/**
 * @brief Struct type of Alive supervision
 */
typedef struct
{
    uint16             WdgMAliveCycleCounter;      /**< Alive supervision cycle counter. @range 0u */
    uint16             WdgMAliveIndicationCounter; /**< The time that Checkpoint has been reached. @range 1u */
    WdgM_SupResultType WdgMAliveSupResult;         /**< Alive supervision result. @range 2u */
} WdgM_AliveSupInfoType;

/**
 * @brief Struct type of deadline supervision.
 */
typedef struct
{
    WdgM_DeadlineTimeType WdgMTimeStampStart;    /**< The os time tick when checkpoint has been reached. @range 0u */
    WdgM_SupResultType    WdgMDeadlineSupResult; /**< Deadline supervision result. @range 1u */
} WdgM_DeadlineSupInfoType;

/**
 * @brief Struct type of internal supervision.
 */
typedef struct
{
    WdgM_ActiveStatusType WdgMIntLogActiveFlag;   /**< Internal logical activity flag. @range 0u */
    WdgM_CheckpointIdType WdgMIntLastReachedCPID; /**< Last reached checkpoint id in internal supervision. @range 1u */
    WdgM_SupResultType    WdgMIntLogSupResult;    /**< Internal supervision result. @range 2u */
} WdgM_IntLogSupInfoType;

/**
 * @brief Struct type of Supervised Entity.
 */
typedef struct
{
    WdgM_LocalStatusType WdgMLocalStatus;           /**< Supervised Entity local status. @range 0..4 */
    uint8                WdgMFailedAliveSupCounter; /**< Failed alive supervision counter @range 0..255 */
    WdgM_SupResultType   WdgMEachSupResult[WDGM_SUPERVISION_MACHNISM_MAX]; /**< Each supervision result @range NA */
} WdgM_SEInfoType;

/**
 * @brief Struct type of Mode information.
 */
typedef struct
{
    WdgM_ModeType WdgMModeCurrentID; /**< Current mode id. @range 0..255 */
    WdgM_ExpiredSupTolCounterType
          WdgMExpiredSupTolCounter; /**< Expired supervsion tolerance counter. @range 0..65535 */
    uint8 WdgMMainFunctionRunTick;  /**< WdgM MainFunction rate. @range 0..255*/
    WdgM_ActiveStatusType
        WdgMMainFunctionActivityFlag; /**< MainFunction activity flag. @range WDGM_AF_FALSE/WDGM_AF_TRUE */
} WdgM_ModeInfoType;

/**
 * @brief Struct type of global information.
 */
typedef struct
{
    WdgM_GlobalStatusType WdgMGlobalResult; /**< WdgM Global status. @range 0..4 */
} WdgM_GlobalInfoType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#endif /* WDGM_TYPE_H_ */
