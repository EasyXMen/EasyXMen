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
 **  @file               : Csm_Types.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : Type definitions of Csm
 **
 ***********************************************************************************************************************/
#ifndef CSM_TYPE_H
#define CSM_TYPE_H

/* =================================================== inclusions =================================================== */
#include "Crypto_GeneralTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/** @brief Jobs PC-config type*/
typedef struct
{
    const Crypto_JobPrimitiveInfoType* jobPrimitiveInfo;
    uint32                             jobId;
    uint32                             jobPriority;
    Crypto_JobRedirectionInfoType*     inOutRedirtRef; /* PRQA S 3432 */ /* VL_Csm_3432 */
    uint32                             queRef;
    boolean                            usePort;
} Csm_JobsCfgType;

/** @brief configuration of a CSM key*/
typedef struct
{
    uint32  cryIfKey;
    boolean usePort;
} Csm_KeyCfgType;

/** @brief  CSM queue configurations*/
typedef struct
{
    uint32 queueSize;
    uint32 cryIfChRef;
} Csm_QueueCfgType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
