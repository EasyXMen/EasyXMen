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
 **********************************************************************************************************************
 **
 **  @file               : StbM_Freshness.h
 **  @author             : xiaojian.liang
 **  @date               : 2026/01/20
 **  @vendor             : isoft
 **  @description        : Synchronized TimeBase Manager Freshness Header File
 **
 **********************************************************************************************************************/

#ifndef STBM_FRESHNESS_H_
#define STBM_FRESHNESS_H_

/* =================================================== inclusions =================================================== */
#include "StbM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#if STBM_QUERY_FRESHNESS_VALUE != STBM_QUERY_FRESHNESS_VALUE_NONE
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
const StbM_FreshnessValueType* StbM_FreshnessGetConfig(uint16 freshnessValueId);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType StbM_FreshnessGetTxData(uint16 freshnessValueId, uint8* freshnessValue, uint32* freshnessValueLength);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType StbM_FreshnessGetTxTruncData(
    uint16  freshnessValueId,
    uint8*  freshnessValue,
    uint32* freshnessValueLength,
    uint8*  truncatedFreshnessValue,
    uint32* truncatedFreshnessValueLength);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
void StbM_FreshnessSPduTxConfirmation(uint16 freshnessValueId);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"

#define STBM_START_SEC_CODE
#include "StbM_MemMap.h"
Std_ReturnType StbM_FreshnessGetRxData(
    uint16       freshnessValueId,
    const uint8* truncatedFreshnessValue,
    uint32       truncatedFreshnessValueLength,
    uint16       authVerifyAttempts,
    uint8*       freshnessValue,
    uint32*      freshnessValueLength);
#define STBM_STOP_SEC_CODE
#include "StbM_MemMap.h"
#endif /* STBM_QUERY_FRESHNESS_VALUE != STBM_QUERY_FRESHNESS_VALUE_NONE */

#ifdef __cplusplus
}
#endif
#endif
