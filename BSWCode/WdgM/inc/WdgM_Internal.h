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
extern const WdgM_ConfigType* WdgM_SelectConfigs[WDGM_MAX_CORES_USED];
/* PRQA S 1500,1752 ++ */ /* VL_WdgM_1500,VL_WdgM_1752 */
extern WdgM_RuntimeType* WdgM_ConfigRuntimeData[WDGM_MAX_CORES_USED];
/* PRQA S 1500,1752 -- */
extern const WdgM_SECfgPCType WdgM_SupervisedEntityPcCfg[WDGM_SE_NUM];

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
extern WdgM_IntLogSupInfoType WdgM_IntLogSupTab[WDGM_MAX_INTERNAL_LOGICAL_SUPERVISION_NUM];
#endif /* WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON */

/* ========================================= external function declarations ========================================= */

/**
 * @brief  Gets the global info tab pointer of current WdgM.
 * @param[out]  globalInfoTab: Pointer to pointer of global status data.
 * @retval      E_OK: success
 * @retval      E_NOT_OK: failure
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68316
 */
/* PRQA S 1501 ++ */ /* VL_WdgM_1501 */
Std_ReturnType WdgM_GetGlobalInfoTab(WdgM_GlobalStatusType** globalInfoTab);
/* PRQA S 1501 -- */

/**
 * @brief  Gets the config pointer of current WdgM.
 * @param[out]  ConfigPtr: Pointer to pointer of WdgM post build config data.
 * @retval      E_OK: success
 * @retval      E_NOT_OK: failure
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68325
 */
Std_ReturnType WdgM_GetConfigPtr(const WdgM_ConfigType** ConfigPtr);

/**
 * @brief  Gets the first expired seid pointer of current WdgM.
 * @param[out]  firstExpiredSEID: Pointer to the pointer of first expired seid.
 * @param[out]  firstExpiredSEIDInv: Pointer to the pointer of first expired inverse seid.
 * @retval      E_OK: success
 * @retval      E_NOT_OK: failure
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68328
 */
Std_ReturnType WdgM_GetFirstExpiredSEIDPtr(
    WdgM_SupervisedEntityIdType** firstExpiredSEID,
    WdgM_SupervisedEntityIdType** firstExpiredSEIDInv);

/**
 * @brief  Gets the first expired seid saved flag pointer of current WdgM.
 * @param[out]  firstExpiredSEIDSaved: Pointer to pointer of current WdgM first expired seid saved flag.
 * @retval      E_OK: success
 * @retval      E_NOT_OK: failure
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68331
 */
Std_ReturnType WdgM_GetFirstExpiredSEIDSaved(uint8** firstExpiredSEIDSaved);

/**
 * @brief  Gets the core index in config.
 * @param[out]  outPutIndex: Pointer to passout core index.
 * @retval      E_OK: success
 * @retval      E_NOT_OK: failed and can not find core id.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-72755
 */
Std_ReturnType WdgM_GetCoreIndex(uint8* outPutIndex);

/* PRQA S 5016 ++ */ /* VL_WdgM_5016 */
/**
 * @brief  Returns the run-time config pointer.
 * @retval      WdgM_RuntimeType*
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-72756
 */
WdgM_RuntimeType* WdgM_GetRunDataPtr(void);
/* PRQA S 5016 -- */

#ifdef __cplusplus
}
#endif

#endif
