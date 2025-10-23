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
 **  @file               : WdgM.h
 **  @author             : xudong.guan
 **  @date               : 2024/12/07
 **  @vendor             : isoft
 **  @description        : Header file for WdgM
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.02.00 2024-10-30  Xudong.Guan  1.Support multiple cores and combination.
 *                                     2.Modify the first expired seid method.
 *  V02.02.01 2024-11-18  Xudong.Guan  1.Fix the verify bug of the SEID in WdgM_GetLocalStatus()
 *  V02.02.02 2024-11-26  Xudong.Guan  1.Fix the modeSwitch interface.
 *  V02.02.03 2024-12-07  Xudong.Guan  1.Modify the cycle processing logic of Mainfunction in modes.
 *  V02.02.04 2024-12-18  Xudong.Guan  1.Update QAC violation inspection instructions.
 *                                     2.Modify the type of Dem report flag of WdgM and update logic of the Dem
 reporting.
 *                                     3.Update some enumeration type to unsigned int and update annotations.
 *                                     4.Conform to the naming convention and fix inconsistent variable names.
 *  V02.02.05 2025-02-18  Xudong.Guan  1.Support post-build selectable compile.
 *                                     2.Adjust the file structure, add header files.
 *                                     3.Update QAC violation inspection instructions.
 *  V02.02.06 2025-03-04  Xudong.Guan  1.Optimize the code and configuration.
 *  V02.02.07 2025-05-20  Xudong.Guan  1.Modify the SE runtime data Initialization method.
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
 \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
   ModeName:WdgM<br>
 RuleSorce:puhua_rules-2024.12.rcf

    \li VL_WdgM_1505
      Reason: External interface.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_0686
      Reason: Array has fewer initializers than its declared size.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_EndLessLoop
      Reason: Necessary logical design for while(1) and have "break" to stop.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_5016
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_5017
      Reason: High cyclomatic complexity indicates inadequate modularization or too much logic in one function.
      Risk: There are risks associated with complex functions.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_WdgM_UsedBeforModify
      Reason:Used before modify.
      Risk: None.
      Prevention:That's how the logic works.

    \li VL_WdgM_0380
      Reason: Macro definitions are necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_WdgM_STVAR
      Reason: The total number of variables is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_WdgM_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_2987
      Reason: Code design requirements.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_3219
      Reason: The static function has been called in other functions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_2962
      Reason: This value is configured in the configuration file.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_WdgM_STCYC
      Reason: Functionality is not separated into subfunctions because there are higher requirements for minimizing the
    stack and applying it to runtime use of the code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_WdgM_STPTH
      Reason: The functions were not separated into sub-functions because there were higher requirements for
    minimizing the stack and applying them to the runtime usage of the code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_3207
      Reason: Depend on the configuration file.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_NameLength
      Reason: Depend on the configuration, The configuration determines the length of the final generated array name.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_1502
      Reason: Configuration variables design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_WdgM_3218
      Reason: file scoped variables only used in one function are necessary for functional usage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

*/
#ifndef WDGM_H_
#define WDGM_H_

/* =================================================== inclusions =================================================== */
#include "WdgM_Cfg.h"
#include "WdgM_PBcfg.h"

/* =============================================== version information ============================================== */
/**
 * @name WDGM_H_VERSION_MACROS
 * @{
 * @brief group contains all version info macros in WdgM.h
 */
#define WDGM_VENDOR_ID          62u /**< vendor ID */
#define WDGM_MODULE_ID          13u /**< module ID */
#define WDGM_INSTANCE_ID        0u  /**< instance ID */
#define WDGM_H_SW_MAJOR_VERSION 2u  /**< WdgM header software ma  jor Version */
#define WDGM_H_SW_MINOR_VERSION 2u  /**< WdgM header software minor Version */
#define WDGM_H_SW_PATCH_VERSION 7u  /**< WdgM header software patch Version */
#define WDGM_H_AR_MAJOR_VERSION 4u  /**< WdgM header autosar major Version */
#define WDGM_H_AR_MINOR_VERSION 9u  /**< WdgM header autosar minor Version */
#define WDGM_H_AR_PATCH_VERSION 0u  /**< WdgM header autosar patch Version */
/** @} */

/* ===================================================== macros ===================================================== */
#define WDGM_INIT_ID                   (0x00u) /**< WdgM_Init API ID */
#define WDGM_DEINIT_ID                 (0x01u) /**< WdgM_DeInit API ID */
#define WDGM_GET_VERSION_INFO_ID       (0x02u) /**< WdgM_GetVersionInfo API ID*/
#define WDGM_SET_MODE_ID               (0x03u) /**< WdgM_SetMode API ID*/
#define WDGM_MAINFUNCTION_ID           (0x08u) /**< WdgM_MainFunction API ID*/
#define WDGM_GET_MODE_ID               (0x0Bu) /**< WdgM_GetMode API ID*/
#define WDGM_GET_LOCAL_STATUS_ID       (0x0Cu) /**< WdgM_GetLocalStatus API ID*/
#define WDGM_GET_GLOBAL_STATUS_ID      (0x0Du) /**< WdgM_GetGlobalStatus API ID*/
#define WDGM_CHECKPOINT_REACHED_ID     (0x0Eu) /**< WdgM_CheckpointReached API ID*/
#define WDGM_PERFORM_RESET_ID          (0x0Fu) /**< WdgM_PerformReset API ID*/
#define WDGM_GET_FIRST_EXPIRED_SEID_ID (0x10u) /**< WdgM_GetFirstExpiredSEID API ID*/

#define WDGM_E_UNINIT       (0x10u) /**< without module initialization*/
#define WDGM_E_PARAM_CONFIG (0x11u) /**< API service Wdg_Init was called with an erroneous configuration set*/
#define WDGM_E_PARAM_SEID   (0x13u) /**< API service called with wrong mode paramete*/
#define WDGM_E_PARAM_MODE   (0x12u) /**< API service called with wrong supervised entity identifier parameter*/
#define WDGM_E_INV_POINTER  (0x14u) /**< API service called with invalid pointer*/
#define WDGM_E_CPID         (0x16u) /**< API service used with an invalid CheckpointId*/
/** API service used in wrong context - WdgM_Init called when module is not deinitialized global status is not
 * WDGM_GLOBAL_STATUS_DEACTIVATED */
#define WDGM_E_NO_DEINIT     (0x1Au)
#define WDGM_E_INIT_FAILED   (0x1Bu) /**< Initialization failed, e.g. selected configuration set doesn't exis*/
#define WDGM_E_PARAM_POINTER (0x1Cu) /**< API service called with a null pointer parameter*/

#define WDGM_E_DISABLE_NOT_ALLOWED (0x15u) /**< Disabling of watchdog not allowed (e.g. in safety relevant systems)*/
/** API service used with a checkpoint of a Supervised Entity that is deactivated in the current Watchdog Manager
 * mode.*/
#define WDGM_E_SEDEACTIVATED (0x19u)
#define WDGM_E_SET_MODE      (0x1Du) /**< Watchdog drivers' mode switch has failed*/

#if (WDGM_COMBINATION_USED == STD_ON)
#define WDGM_COMBINATITON_INVALIED_ID 0xFFu /**< Invalid combination id */
#endif

/* ================================================ type definitions ================================================ */
typedef WdgM_PBConfigType WdgM_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the Watchdog Manager.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63721
 */
void WdgM_Init(const WdgM_ConfigType* ConfigPtr);

/**
 * @brief Returns the current mode of the Watchdog Manager.
 * @param[in]   Mode: Mode id.
 * @retval      E_OK: success
 * @retval      E_NOT_OK: failure
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63726
 */
Std_ReturnType WdgM_GetMode(WdgM_ModeType* Mode);

/**
 * @brief Sets the current mode of Watchdog Manager.
 * @param[in]   Mode: Mode id.
 * @retval      E_OK: Successfully changed to the new mode
 * @retval      E_NOT_OK: Changing to the new mode failed
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63725
 */
Std_ReturnType WdgM_SetMode(WdgM_ModeType Mode);

/**
 * @brief  Indicates to the Watchdog Manager that a Checkpoint within a Supervised Entity has been reached.
 * @param[in]   SEID: Supervised Entity Id.
 * @param[in]   CheckpointID: Checkpoint id.
 * @retval      E_OK: Successfully updated alive counter
 * @retval      E_NOT_OK: updated failed
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63727
 */
Std_ReturnType WdgM_CheckpointReached(WdgM_SupervisedEntityIdType SEID, WdgM_CheckpointIdType CheckpointID);

/**
 * @brief  Returns the global supervision status of the Watchdog Manager.
 * @param[out]  Status: WdgM local status of Supervised Entity.
 * @retval      E_OK: Current supervision status successfully returned
 * @retval      E_NOT_OK: Returning current supervision status failed
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63729
 */
Std_ReturnType WdgM_GetGlobalStatus(WdgM_GlobalStatusType* Status);

/**
 * @brief  Instructs the Watchdog Manager to cause a watchdog reset.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63730
 */
void WdgM_PerformReset(void);

/**
 * @brief  Returns SEID that first reached the state WDGM_LOCAL_STATUS_EXPIRED
 * @param[out]  SEID: Pointer to store Supervised Entity Id.
 * @retval      E_OK: Success
 * @retval      E_NOT_OK: failure
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63731
 */
Std_ReturnType WdgM_GetFirstExpiredSEID(WdgM_SupervisedEntityIdType* SEID);

/**
 * @brief  Returns the supervision status of an individual Supervised Entity.
 * @param[in]   SEID: Pointer to store the Supervised Entity Id.
 * @param[out]  Status: Pointer to store local status of Supervised Entity.
 * @retval      E_OK: Current supervision status successfully returned
 * @retval      E_NOT_OK: Returning current supervision status failed
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63728
 */
Std_ReturnType WdgM_GetLocalStatus(WdgM_SupervisedEntityIdType SEID, WdgM_LocalStatusType* Status);

#if (STD_ON == WDGM_OFF_MODE_ENABLED)
/**
 * @brief  Deinit the local status of all SE in the Mode.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63723
 */
void WdgM_DeInit(void);
#endif /*STD_ON == WDGM_OFF_MODE_ENABLED*/

#if (STD_ON == WDGM_VERSION_INFO_API)
/**
 * @brief  Gets the version of Watchdog Manager.
 * @param[out]  VersionInfo: Pointer to where to store the version information of the module WdgM.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63724
 */
void WdgM_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif /**STD_ON == WDGM_VERSION_INFO_API*/

#ifdef __cplusplus
}
#endif

#endif
