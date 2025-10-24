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
 **  @file               : Nm_Internal.h
 **  @author             : Wanglili
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : type definitions of Nm
 **
 ***********************************************************************************************************************/
#ifndef NM_INTERNAL_H_
#define NM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "NmStack_Types.h"
#include "Nm_Cfg.h"

#if NM_MULTIPLE_PARTITION_USED == STD_ON
#include "Os.h"
#endif
/* ===================================================== macros ===================================================== */
#define NM_STATE_CHANGE_TYPE_NUM ((uint8)(6u))

/* magic number */
#define NM_SINGLEBYTE_BASENUM              (uint8)(8u)
#define NM_INVALID_CLUSTER_INDEX           (uint8)(0xffu)
#define NM_HANDLE_EIRA_TIMER_CHANNEL_INDEX (uint8)(0x0u)
#define NM_INVALID_COM_SIGNELID            (uint8)(0xffu)
/* claclulate array element count */
#define NM_UBOND(x) (sizeof(x) / sizeof((x)[0]))

/* ================================================ type definitions ================================================ */
/* used cycle find */
typedef uint32 uintx;

typedef enum
{
    NM_UNINIT,
    NM_INIT,
} Nm_InitStatusType;

#if (NM_STATE_CHANGE_IND_ENABLED == STD_ON)
/* nm state and report signal value */
typedef struct
{
    /* nmPreviousState */
    Nm_StateType PreState;
    /* nmCurrentState */
    Nm_StateType CurrentState;
    /* report singnal value */
    uint8 SignalValue;
} Nm_StateReportKindType;
#endif
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)

/* shutdown timer type */
typedef uint16 Nm_ShutdownTimerType;

typedef enum
{
    /*shut down coordinated initial state*/
    NM_SHUTDOWN_STU_INIT = 0x0u,
    /*shut down coordinated start*/
    NM_SHUTDOWN_STU_START,
    /*shut down coordinated cancel*/
    NM_SHUTDOWN_STU_ABORT,
    /*shut down coordinated is going,network not relase*/
    NM_SHUTDOWN_STU_RUN,
    /*shut down coordinated success,network relase*/
    NM_SHUTDOWN_STU_STOP,
} Nm_ShutdownStatusType;

/* nm inner channel struct */
typedef struct
{
    boolean               TopMostCoor;
    Nm_ShutdownStatusType ShutdownStatus;
    uint8                 ReadySleepInChMask;
    boolean               CoorRsbInCluster;
    uint8                 ShutdownTimerLoadOkInChMask;
    uint8                 SleepAllowedInChMask;
    uint8                 ShutdownAbortFinishedInChMask;
    uint8                 PassiveChannelReadySleepInChMask;
} Nm_InnerClusterType;

#endif

/* nm inner channel struct */
#if NM_COORDINATOR_SUPPORT_ENABLED == STD_ON || NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON
typedef struct
{
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
    /* remote sleep indication */
    boolean RemoteSleepIndFlg;
    /* bus awake flage */
    boolean BusAwakeFlg;
    /* notify lower layer network request */
    boolean LowLayNetRequest;
    /* ComM request network */
    boolean UpLayNetRequest;
    /* allow shutdown */
    boolean AllowShutdown;
#if (NM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    /* coordinator ready sleep bit */
    boolean CoorRsb;
#endif
    boolean LowLayBusSyncPoint;
    /* shutdown timer */
    Nm_ShutdownTimerType ShutdownTimer;
#endif
#if (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON)
    boolean UpDateEraFlg;
#endif
} Nm_InnerChannelType;
#endif

#if (NM_PARTIAL_NETWORK_SUPPORT_ENABLED == STD_ON) && (NM_PN_EIRA_CALC_ENABLED == STD_ON)
typedef struct
{
    boolean UpDateEiraFlg;
    uint8   PnEiraInfo[NM_PN_EIRA_INFO_LENGTH];
    uint16  PnEiraResetTimer[NM_PN_EIRA_INFO_LENGTH * NM_SINGLEBYTE_BASENUM];
} Nm_InnerGlobalType;
#endif
/* =========================================== external data declarations =========================================== */
#if NM_MULTIPLE_PARTITION_USED == STD_ON
extern const ApplicationType Nm_OsApplications[NM_PARTITION_NUMBER];
#endif
/* ========================================== internal function definitions ========================================= */
/* ========================================= external function declarations ========================================= */
#endif
