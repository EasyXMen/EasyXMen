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
 **  @file               : LinSM_Internal.h
 **  @author             : caihong.liu
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Lin statue manager internal header
 **
 ***********************************************************************************************************************/
/* ================================================ misar justifications ============================================ */

#ifndef LINSM_INTERNAL_H_
#define LINSM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "LinIf.h"
#include "LinSM_Cfg.h"
/** SWS_LinSM_00013 */
#include "ComM.h"
/* Multiple partition check. */
#if LINSM_MULTIPLE_PARTITION_USED == STD_ON
#include "Os.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/** Type used to report the current mode to the BswM */
typedef uint8 LinSM_ChannelIndexType;
/**
 * @brief     Sub-status of LINSM_FULL_COM
 */
typedef enum
{
    LINSM_RUN_COMMUNICATION,
    LINSM_GOTO_SLEEP
} LinSM_FullComSubStatusType;

/**
 * @brief     Handling Request
 */
typedef enum
{
    LINSM_EXE_NOTHING,
    LINSM_EXE_GOTO_SLEEP,
    LINSM_EXE_WAKE_UP,
} LinSM_HandlingRequestType;

/**
 * @brief     The timer associated with the current operation
 */
typedef enum
{
    LINSM_NONE_TIMER,
    LINSM_WAKEUP_TIMER,
    LINSM_SILENCEWAKEUP_TIMER
} LinSM_TimerType;

/**
 * @brief     Runtime variables
 */
typedef struct LinSM_RunTimeTag
{
    uint32          TimerCnt;
    LinSM_TimerType TimerType;
    uint8           WakeupRetryCnt;
    uint8           ChannelComMode;
    /** Use only when channelComMode = LINSM_FULL_COM */
    LinSM_FullComSubStatusType FullComSubStatus;
    LinSM_HandlingRequestType  RunningReq;
    ComM_ModeType              ReqComMode;
    boolean                    ReqComModeFlag;
    LinIf_SchHandleType        CurSchedule;
    boolean                    ReportSchedule;
} LinSM_RunTimeType;

/**
 * @brief     Node Type Definition
 */
typedef enum
{
    LINSM_MASTER,
    LINSM_SLAVE
} LinSM_NodeType;

/** ECUC_LinSM_00146 */
/**
 * @brief     Record the schedule table configuration of a channel
 */

typedef uint8 LinSM_ScheduleType;

/** ECUC_LinSM_00142 */
/**
 * @brief     Channel-related configuration parameter structure
 */
typedef struct LinSM_ChannelTag
{
#if LINSM_MULTIPLE_PARTITION_USED == STD_ON
    ApplicationType ApplicationID;
#endif
    /** Timeout in seconds for the goto sleep, wakeup and schedule request calls
     *  to LinIf. The timeout must be longer than a goto-sleep command on the
     *  bus (i.e. it is bit rate dependent). */
    /** ECUC_LinSM_00144 */
    uint32 ConfirmationTimeout;

    /** Specifies the LIN node type of this channel. */
    /** ECUC_LinSM_00211 */
    LinSM_NodeType NodeType;

    /** Timeout in ms after a failed wakeup sequence until a new wakeup
     *  process is started.*/
    /** ECUC_LinSM_00210 */
    uint32 SilenceAfterWakeupTimeout;

#if LINSM_TRANSCEIVER_PASSIVER_MODE_SUPPORT == STD_ON
    /** Indicate parameter [transceiverPassiveMode] is support or not */
    boolean IsTransceiverPassiveModeSupport;

    /** Selects STANDBY (true) or SLEEP (false) transceiver mode when
     *  entering LINSM_NO_COM. */
    /** ECUC_LinSM_00202 */
    boolean TransceiverPassiveMode;
#endif

    /** Unique handle to identify one certain LIN network. Reference to
     *  one of the network handles configured in the ComM. */
    /** ECUC_LinSM_00145 */
    NetworkHandleType ComMNetworkHandleRef;

    uint8              ScheduleIndex;
    uint8              ScheduleNum;
    LinSM_RunTimeType* RuntimePtr;
} LinSM_ChannelType;

/** ECUC_LinSM_00207 */
/**
 * @brief     The root struct configuration parameters of LinSM.
 */
typedef struct LinSM_ConfigTag
{
    /** Specifies the maximal amount of mode request repetitions without a
     *  respective mode indication from the LinIf module until the LinSM module
     *  reports a Default Error to the Det and tries to go back to no
     *  communication. */
    /** ECUC_LinSM_00208 */
    uint8 ModeRequestRepetitionMax;

    /** Describes each LIN channel the LinSM is connected to. */
    /** ECUC_LinSM_00142 */
    const LinSM_ChannelType* ChannelPtr;

    /** Record all schedule table configurations for a variant. */
    const LinSM_ScheduleType* SchedulePtr;
} LinSM_ConfigType;
/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if LINSM_MULTIPLE_PARTITION_USED == STD_ON
extern const ApplicationType LinSM_OsApplicationList[LINSM_PARTITION_NUM];

#endif
/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif
#endif
