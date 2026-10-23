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
 **  @file               : WdgM_Internal.h
 **  @author             : xudong.guan
 **  @date               : 2025/02/13
 **  @vendor             : isoft
 **  @description        : WdgM internal header file
 **
 ***********************************************************************************************************************/

#ifndef WDGM_INTERNAL_H_
#define WDGM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "WdgM.h"
#include "WdgM_Arti.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/* PRQA S 3414 ++ */ /* VL_QAC_FctLikeMacro */
#if !defined(WDGM_LOCAL)
#define WDGM_LOCAL static
#endif

/* PRQA S 3414 -- */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#define WDGM_START_SEC_VAR_POWER_ON_CLEARED_16
#include "WdgM_MemMap.h"
extern WdgM_SupervisedEntityIdType WdgM_FirstExpiredSEID;
#define WDGM_STOP_SEC_VAR_POWER_ON_CLEARED_16
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_POWER_ON_CLEARED_16
#include "WdgM_MemMap.h"
extern WdgM_SupervisedEntityIdType WdgM_FirstExpiredSEIDInverse;
#define WDGM_STOP_SEC_VAR_POWER_ON_CLEARED_16
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_POWER_ON_CLEARED_8
#include "WdgM_MemMap.h"
extern Std_ReturnType WdgM_FirstExpiredSEIDSavedFlag;
#define WDGM_STOP_SEC_VAR_POWER_ON_CLEARED_8
#include "WdgM_MemMap.h"

#if (WDGM_MAINFUNCTION_NUM > 1)
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
extern WdgM_MainFunctionStateType WdgM_MainFunctionStateVar[WDGM_MAINFUNCTION_NUM];
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
extern void WdgM_MainFunctionGlobalAliveSupervisionErrorNotification(WdgM_GlobalStatusType globalStatus);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
extern void WdgM_MainFunctionLocalAliveSupervisionErrorNotification(
    WdgM_ApplicationType appl,
    WdgM_LocalStatusType localStatus);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#else
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
extern WdgM_MainFunctionStateTickType WdgM_MainFunctionStateVarTick;
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
#endif

#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
#define WDGM_START_SEC_VAR_POWER_ON_CLEARED_8
#include "WdgM_MemMap.h"
extern WdgM_DemReportStatusType WdgM_DemSetFlag;
#define WDGM_STOP_SEC_VAR_POWER_ON_CLEARED_8
#include "WdgM_MemMap.h"
#endif
#ifdef __cplusplus
}
#endif

#endif
