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
 ***********************************************************************************************************************
 **
 **  @file               : ComM_InternalSM.c
 **  @author             : xiaojian.liang
 **  @date               : 2024/08/21
 **  @vendor             : isoft
 **  @description        : ComM Internal SM Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "ComM_InternalSM.h"
#include "ComM_Channel.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definition ============================================ */

/* ========================================= internal function declarations ========================================= */

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
/* ========================================== external function definitions ========================================= */
Std_ReturnType ComM_InternalSM_RequestComMode(NetworkHandleType Channel, ComM_ModeType ComMode)
{
    ComM_ChannelBusSMModeIndication(Channel, ComMode);
    return E_OK;
}

/* ========================================== internal function definitions ========================================= */
/* PRQA S 1503 -- */ /* VL_QAC_NoUsedApi */

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
