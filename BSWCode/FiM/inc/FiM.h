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
 **  @file               : FiM.h
 **  @author             : tao.yu
 **  @date               : 2024/12/04
 **  @vendor             : isoft
 **  @description        : API declaration of FiM
 **
 ***********************************************************************************************************************/
#ifndef FIM_H_
#define FIM_H_

/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2020-05-09  tao.yu       Create
 *  V02.00.00 2021-04-08  tao.yu       release version
 *  V03.00.00 2024-12-04  tao.yu       ReWork and update to R23-11,support Dem multi-partition
 *  V03.00.01 2025-09-16  li.feng      CPD-84617,To fix the incorrect use of GetApplicationID and the issue with
 *                                     obtaining satelliteId
 *  V03.00.02 2025-11-07  li.feng      Optimize FIM RAM Usage
 *  V03.00.03 2026-04-08  tao.yu       CPT-18443, fix the triggering of the Dem master partition is not handled
 *                                     correctly in the trigger mode.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:FiM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_FiM_1502
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_1505
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_1531
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_1533
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_3218
      Reason: const variable define.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_2991
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_2995
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_FiM_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different FiMFIDs, FiMInhibitionConfigurations, FiMSummaryEvents and other configuration conditions, as well as some
  precompiled macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_FiM_3213
      Reason: The tag '%1s' is not used and could be removed.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_3132
      Reason: This cast is converting a complex floating point expression to an integral type.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_3120
      Reason: In the generated code, there are a lot of devil numbers, don't have to define a macro.
      Risk: The reader can derive the meaning of the number based on the annotation or structure type, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_FiM_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_2992
      Reason: The control statement has no effect, but is reserved for better readability.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_FiM_2996
      Reason: the issue shall be avoided in actual running environment and configs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FiM_1330
      Reason: The parameter identifiers in this function declaration differ from those in a previous declaration.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */

/* =================================================== inclusions =================================================== */
#include "FiM_PBcfg.h"
#include "FiM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/* Published information elements */
#define FIM_MODULE_ID                   (11u)
#define FIM_VENDOR_ID                   (62u)
#define FIM_INSTANCE_ID                 (0u)
#define FIM_AR_RELEASE_MAJOR_VERSION    (4u)
#define FIM_AR_RELEASE_MINOR_VERSION    (9u)
#define FIM_AR_RELEASE_REVISION_VERSION (0u)
#define FIM_SW_MAJOR_VERSION            (3u)
#define FIM_SW_MINOR_VERSION            (0u)
#define FIM_SW_PATCH_VERSION            (3u)

/* Error classification */

/** API function called with no det error */
#define FIM_E_NO_ERROR (0x0u)
/**API function called before the FiM module has been full initialized or after the FiM module has been shut down */
#define FIM_E_UNINIT (0x01u)
/** FiM_GetFunctionPermission called with wrong FID */
#define FIM_E_FID_OUT_OF_RANGE (0x02u)
/** Dem calls FiM with invalid EventId */
#define FIM_E_EVENTID_OUT_OF_RANGE (0x03u)
/** API is invoked with NULL Pointer. */
#define FIM_E_PARAM_POINTER (0x04u)
/** Invalid configuration set selection */
#define FIM_E_INIT_FAILED (0x05u)
/** Dem calls FiM with invalid ComponentId */
#define FIM_E_COMPONENT_OUT_OF_RANGE (0x09u)
/** partition error */
#define FIM_E_PARTITION_ERROR (0x0Au)

/* Service ID */
#define FIM_SID_INIT                        (0x00u) /** FiM_Init */
#define FIM_SID_GETFUNCTIONPERMISSION       (0x01u) /** FiM_GetFunctionPermission */
#define FIM_SID_DEMTRIGGERONMONITORSTATUS   (0x02u) /** FiM_DemTriggerOnMonitorStatus */
#define FIM_SID_DEMINIT                     (0x03u) /** FiM_DemInit */
#define FIM_SID_GETVERSIONINFO              (0x04u) /** FiM_GetVersionInfo */
#define FIM_SID_MAINFUNCTION                (0x05u) /** FiM_MainFunction */
#define FIM_SID_DEMTRIGGERONCOMPONENTSTATUS (0x06u) /** FiM_ DemTriggerOnComponentStatus */
#define FIM_SID_SETFUNCTIONAVAILABLE        (0x07u) /** FiM_SetFunctionAvailable */
#define FIM_SID_INTERNALAPIID               (0xffU) /** Internal Functions */

/**
 * @brief         This service initializes the FIM.Initialization needs to be called separately for each partition.
 * @param[in]     FiMConfigPtr: Pointer to the configuration.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69573
 */
extern void FiM_Init(const FiM_ConfigType* FiMConfigPtr);

/**
 * @brief         This service reports the permission state to the functionality.
 * @param[in]     FID: Identification of a functionality by assigned FID. The FunctionId is configured in the FIM.
 *                     Min.: 1 (0: Indication of no functionality)
 *                     Max.: Result of configuration of FIDs in FIM (Max is either 255 or 65535)
 * @param[out]    Permission: TRUE: FID has permission to run
 *                            FALSE: FID has no permission to run, i.e. shall not be executed
 * @return        Std_ReturnType
 * @retval        E_OK: The request is accepted
 * @retval        E_NOT_OK: The request is not accepted, ie. initialization of FIM not completed
 * @reentrant     Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69574
 */
extern Std_ReturnType FiM_GetFunctionPermission(FiM_FunctionIdType FID, boolean* Permission);

#if (STD_ON == FIM_AVAILABILITY_SUPPORT)
/**
 * @brief         This service sets the availability of a function. The function is only available if FiMAvailability
 *                Support is configured as True.
 * @param[in]     FID: Identification of a functionality by assigned FID.
 * @param[in]     Availability: The permission of the requested FID. TRUE: Function is available. FALSE: Function is
 *                              not available.
 * @return        Std_ReturnType
 * @retval        E_OK: The request is accepted
 * @retval        E_NOT_OK: Request is not accepted (e.g. invalid FID is given)
 * @reentrant     Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69575
 */
extern Std_ReturnType FiM_SetFunctionAvailable(FiM_FunctionIdType FID, boolean Availability);
#endif /* STD_ON == FIM_AVAILABILITY_SUPPORT */

#if (STD_ON == FIM_VERSION_INFO_API)
/**
 * @brief         This service returns the version information of this module.
 * @param[out]    versioninfo: Pointer to where to store the version information of this module.
 * @reentrant     Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69579
 */
extern void FiM_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#if (STD_OFF == FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
/**
 * @brief         This function is used to evaluate permission states cyclically.
 * @reentrant     Non Reentrant
 * @synchronous   Synchronous
 * @trace         CPD-69580
 */
/* PRQA S 1512,1513 ++ */ /* VL_FiM_1512,VL_QAC_MultiDeclaration */
/* PRQA S 3449,3451 ++ */ /* VL_QAC_MultiDeclaration,VL_QAC_MultiDeclaration */
extern void FiM_MainFunction(void);
/* PRQA S 1512,1513 -- */
/* PRQA S 3449,3451 -- */
#endif

#ifdef __cplusplus
}
#endif
#endif
