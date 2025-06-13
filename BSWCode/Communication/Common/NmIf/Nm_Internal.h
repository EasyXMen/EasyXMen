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
**  FILENAME    : Nm_Internal.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Wanglili                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef NM_INTERNAL_H_
#define NM_INTERNAL_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "NmStack_Types.h"
#include "Nm_Cfg.h"

#if NM_MULTIPLE_PARTITION_USED == STD_ON
#include "Os.h"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define NM_MODULE_ID   ((uint16)(29u))
#define NM_VENDOR_ID   ((uint16)(62u))
#define NM_INSTANCE_ID ((uint8)(0u))

/* claclulate array element count */
#define NM_UBOND(x) (sizeof(x) / sizeof((x)[0]))

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* used cycle find */
typedef uint32 uintx;

#if (STD_ON == NM_STATE_CHANGE_IND_ENABLED)
/* nm state and report signal value */
typedef struct
{
    /* nmPreviousState */
    Nm_StateType nmPreSta;
    /* nmCurrentState */
    Nm_StateType nmCurSta;
    /* report singnal value */
    uint8 signalVal;
} Nm_StateReportKindType;
#endif /* STD_ON == NM_STATE_CHANGE_IND_ENABLED */
#if (STD_ON == NM_COORDINATOR_SUPPORT_ENABLED)

/* shutdown timer type */
typedef uint16 shutdownTimerType;

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
    /* remote sleep indication */
    boolean remoteSleepIndFlg;
    /* bus awake flage */
    boolean busAwakeFlg;
    /* notify lower layer network request */
    boolean lowLayNetRequest;
    /* ComM request network */
    boolean upLayNetRequest;
    /* allow shutdown */
    boolean allowShutdown;
#if (STD_ON == NM_COORDINATOR_SYNC_SUPPORT)
    /* coordinator ready sleep bit */
    boolean coorRSB;
#endif /* STD_ON ==  NM_COORDINATOR_SYNC_SUPPORT */
    boolean lowLayBusSyncPoint;
    /* shutdown timer */
    shutdownTimerType shutdownTimer;
} Nm_InnerChannelType;

typedef struct
{
    Nm_ShutdownStatusType shutdownStatus;
} Nm_InnerClusterType;

#endif /* STD_ON == NM_COORDINATOR_SUPPORT_ENABLED */

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/
#if NM_MULTIPLE_PARTITION_USED == STD_ON
extern const Os_ApplicationType Nm_ApplicationID;
#endif /* NM_MULTIPLE_PARTITION_USED == STD_ON */

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#endif /* NM_INTERNAL_H_ */
