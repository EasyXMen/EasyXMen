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
 **  @file               : NmStack_Types.h
 **  @author             : Wanglili
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : enum type definitions of Nm
 **
 ***********************************************************************************************************************/
#ifndef NMSTACK_TYPES_H_
#define NMSTACK_TYPES_H_
/* =================================================== inclusions =================================================== */
/* ===================================================== macros ===================================================== */
/* ================================================ type definitions ================================================ */
/**
 * @brief     Operational modes of the network management.
 */
typedef enum
{
    NM_MODE_BUS_SLEEP,
    NM_MODE_PREPARE_BUS_SLEEP,
    NM_MODE_SYNCHRONIZE,
    NM_MODE_NETWORK,
} Nm_ModeType;

/**
 * @brief     States of the network management state machine.
 */
typedef enum
{
    NM_STATE_UNINIT            = 0,
    NM_STATE_BUS_SLEEP         = 1,
    NM_STATE_PREPARE_BUS_SLEEP = 2,
    NM_STATE_READY_SLEEP       = 3,
    NM_STATE_NORMAL_OPERATION  = 4,
    NM_STATE_REPEAT_MESSAGE    = 5,
    NM_STATE_SYNCHRONIZE       = 6,
    NM_STATE_OFFLINE           = 7,
} Nm_StateType;

/**
 * @brief     BusNm Type.
 */
typedef enum
{
    NM_BUSNM_CANNM,
    NM_BUSNM_FRNM,
    NM_BUSNM_UDPNM,
    NM_BUSNM_GENERICNM,
    NM_BUSNM_J1939NM,
    NM_BUSNM_LOCALNM,
    NM_BUSNM_OSEKNM,
    NM_BUSNM_UNDEF,
} Nm_BusNmType;

/* ========================================== internal function definitions ========================================= */
/* =========================================== external data declarations =========================================== */
/* ========================================= external function declarations ========================================= */
#endif
