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
 **  @file               : WdgIf.h
 **  @author             : xudong.guan
 **  @date               : 2024/08/20
 **  @vendor             : isoft
 **  @description        : Header file for WdgIf
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.02.00 2024-10-30  Xudong.Guan  1.Modify the annotation format.
 *  V02.02.01 2024-12-18  Xudong.Guan  1.Update QAC violation inspection instructions.
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
 \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
   ModeName:WdgIf<br>
 RuleSorce:puhua_rules-2024.12.rcf

  \li VL_WdgIf_0380
    Reason: Macro definitions are necessary
    Risk: No risk.
    Prevention: None.

  \li VL_WdgIf_0553
    Reason: The usage of this file depends on the config
    Risk: No risk.
    Prevention: None.

  \li VL_WdgIf_3432
    Reason: Function-like macros are used to allow more efficient code.
    Risk: No risk.
    Prevention: None.

  \li VL_MTR_WdgIf_CONF
    Reason: Some special files have less or no code, allowing for the reduction of useless comments.
    Risk: No risk.
    Prevention: None.

  \li VL_WdgIf_3472
    Reason: Macro definitions for easy reuse.
    Risk: No risk.
    Prevention: None.

*/

#ifndef WDGIF_H
#define WDGIF_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "WdgIf_Types.h"
#include "WdgIf_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
#define WDGIF_H_AR_MAJOR_VERSION 4u /*Autosar Major Version*/
#define WDGIF_H_AR_MINOR_VERSION 9u /*Autosar Minor Version*/
#define WDGIF_H_AR_PATCH_VERSION 0u /*Autosar Patch Version*/
#define WDGIF_H_SW_MAJOR_VERSION 2u /*Major Version*/
#define WDGIF_H_SW_MINOR_VERSION 2u /*Minor Version*/
#define WDGIF_H_SW_PATCH_VERSION 1u /*Patch Version*/

#define WDGIF_VENDOR_ID 62U /*Vendor ID*/
#define WDGIF_MODULE_ID 43U /*Module ID*/
#define WDGIF_INSTANCE  0U  /*Instance ID*/

/* ===================================================== macros ===================================================== */
#define WDGIF_SETMODE_ID        (0x01u) /* API SetMode ID*/
#define WDGIF_TRIGGER_ID        (0x02u) /* API Trigger ID*/
#define WDGIF_GETVERSIONINFO_ID (0x03u) /* API Getversioninfo ID*/

#define WDGIF_E_PARAM_DEVICE  (0x01u) /* Invalid device ID   */
#define WDGIF_E_PARAM_POINTER (0x03u) /* Invalid pointer   */

/* ================================================= type definition ================================================ */
/* Type for function macro of wdg driver */
typedef struct
{
    /* PRQA S 3432 ++ */ /* VL_WdgIf_3432 */
    void (*Wdg_SetTriggerCondition)(uint16 timeout);
    Std_ReturnType (*Wdg_SetMode)(WdgIf_ModeType Mode);
    /* PRQA S 3432 -- */
} Wdg_GeneralType;

/* Type for wdg driver */
typedef struct
{
    Wdg_GeneralType WdgRef;
} WdgIf_DeviceType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if (WDGIF_NUMBER_OF_DEVICES > 1)
extern const WdgIf_DeviceType WdgIf_DeviceData[WDGIF_NUMBER_OF_DEVICES];
#endif /*WDGIF_NUMBER_OF_DEVICES > 1*/

/* ========================================= external function declarations ========================================= */

/**
 * @brief Set the current mode of watchdog driver.
 * @param[in]   DeviceIndex uint8.
 * @param[in]   WdgMode see WdgM_ModeType.
 * @retval      E_OK Successfully set watchdog driver to the new mode
 * @retval      E_NOT_OK Changing to the new mode failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68258
 */
extern Std_ReturnType WdgIf_SetMode(uint8 DeviceIndex, WdgIf_ModeType WdgMode);

/**
 * @brief Set the trigger condition value to watchdog driver.
 * @param[in]   DeviceIndex uint8.
 * @param[in]   Timeout uint16.
 * @retval      E_OK Successfully set trigger condition value to watchdog driver
 * @retval      E_NOT_OK Set trigger condition value failed
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68259
 */
extern void WdgIf_SetTriggerCondition(uint8 DeviceIndex, uint16 Timeout);

#if (STD_ON == WDGIF_VERSION_INFO_API)
/**
 * @brief Api for getting version info of WdgIf.
 * @param[out]  versionInfoPtr Std_VersionInfoType*.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68260
 */
extern void WdgIf_GetVersionInfo(Std_VersionInfoType* versionInfoPtr);
#endif /*STD_ON == WDGIF_VERSION_INFO_API*/

#ifdef __cplusplus
}
#endif

#endif /*WDGIF_H */
