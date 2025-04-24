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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Xcp_GenericType.h                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Generic type define                                         **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef XCP_GENERICTYPES_H
#define XCP_GENERICTYPES_H

/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"

/* Bitmask for Priviledge Level */
#define XCP_PL_CAL       0x01u
#define XCP_PL_DAQ       0x04u
#define XCP_PL_STIM      0x08u
#define XCP_PL_PGM       0x10u

#define XCP_DAQ_STATIC   0u
#define XCP_DAQ_DYNAMIC  1u

#define XCP_TS_NO_TS     0u
#define XCP_TS_ONE_BYTE  1u
#define XCP_TS_TWO_BYTE  2u
#define XCP_TS_FOUR_BYTE 4u

#define XCP_AG_BYTE      0u
#define XCP_AG_WORD      1u
#define XCP_AG_DWORD     2u
/* OVERLOAD */
#define XCP_DAQ_OVL_NO_INDIC          0u
#define XCP_DAQ_OVL_PID               1u
#define XCP_DAQ_OVL_EV                2u

#define XCP_PID_ABSOLUTE              0u
#define XCP_PID_RELATIVE_BYTE         1u
#define XCP_PID_RELATIVE_WORD         2u
#define XCP_PID_RELATIVE_WORD_ALIGNED 3u

typedef uint16 Xcp_CommandStatusType;

#define XCP_CMD_IDLE         0
#define XCP_CMD_BLOCK_RECV   0x01U
#define XCP_CMD_RECV         0x02U
#define XCP_CMD_EXCUTE       0x04U
#define XCP_CMD_BLOCK_TRANSF 0x08U
#define XCP_CMD_TRANSF       0x10U

/* Xcp polling mode by short_upload or upload command to get measurement data*/
#define XCP_POLL_MEA_BY_SHORT_UP 0u
#define XCP_POLL_MEA_BY_UP       1u

typedef enum
{
    DAQ = 1,
    STIM,
    DAQ_STIM
} Xcp_DaqDirectionType;

typedef enum
{
    XCP_EVENT_CONSIST_ODT = 0,
    XCP_EVENT_CONSIST_DAQ,
    XCP_EVENT_CONSIST_EVENT
} Xcp_EvChConsistType;

typedef enum
{
    TIMESTAMP_UNIT_1NS = 0,
    TIMESTAMP_UNIT_10NS,
    TIMESTAMP_UNIT_100NS,
    TIMESTAMP_UNIT_1US,
    TIMESTAMP_UNIT_10US,
    TIMESTAMP_UNIT_100US,
    TIMESTAMP_UNIT_1MS,
    TIMESTAMP_UNIT_10MS,
    TIMESTAMP_UNIT_100MS,
    TIMESTAMP_UNIT_1S,
    TIMESTAMP_UNIT_1PS,
    TIMESTAMP_UNIT_10PS,
    TIMESTAMP_UNIT_100PS
} XcpTimestampUnitType;

typedef enum
{
    XCP_DAQ_CFG_IDLE = 0,
    XCP_DAQ_CFG_FREE,
    XCP_DAQ_CFG_DAQ,
    XCP_DAQ_CFG_ODT,
    XCP_DAQ_CFG_ENTRY,
    XCP_DAQ_CFG_DONE
} Xcp_DaqCfgStatusType;

typedef enum
{
    XCP_UINIT = 0,
    XCP_DISCONNECT,
    XCP_PRE_CONNECT,
    XCP_CONNECT,
    XCP_PRE_USERDEFINE,
    XCP_USERDEFINE,
} Xcp_StatusType;

typedef enum
{
    XCP_PRE_SEED = 0,
    XCP_SEED,
    XCP_PRE_KEY,
    XCP_KEY
} Xcp_SeedandKeyStatusType;

typedef struct
{
    uint8 extensionAddr;
    uint32 transferAddr;
} Xcp_AddressType;

#endif
