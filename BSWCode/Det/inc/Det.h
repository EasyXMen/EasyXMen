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
 **  @file               : Det.h
 **  @author             : haibin.shao
 **  @date               : 2024/09/25
 **  @vendor             : isoft
 **  @description        : Default Error Tracer
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.00.00 2020-07-23  haibin.shao  1. Update code according AUTOSAR CP23 Specification
 *                                     2. Modify code with the new coding rule
 *  V02.00.01 2025-04-14  haibin.shao  Optimize code implementation and template engine generation rule updates
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanNm<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Det_3206
      Reason: Variables are used in different configurations due to complex precompilation logic.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_0791
      Reason: Configuration data design is required.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_3408
      Reason: Configuration structure design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_1334
      Reason: Only parameter name is different.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_Det_1502
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_2740
      Reason:Necessary logical design for while(1)
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_2870
      Reason: Special scenario use
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_1504
      Reason: Variables are used in different configurations due to complex precompilation logic.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_3218
      Reason: Use the macro definition symbol instead of the static keyword
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_3432
      Reason: Function-like macros are used to allow more efficient code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_2986
      Reason: Allow some conditional statements to always equal the result on the left or right.
      Risk: It may cause incomplete coverage of the test branch.
      Prevention: Use different test data to cover.

    \li VL_Det_1290
      Reason: Other Module Macro Definition References
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_5004
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Det_STMIF
      Reason: Functions have different scenarios that need to be covered, which depends on the configuration of
  different precompiled macros
      Risk: The code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_Det_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_1513
      Reason: Identifier '${name}' with external linkage has separate non-defining declarations in more than one
  location. Risk: No risk. Prevention: Functional reliability guaranteed by design.

    \li VL_Det_3440
      Reason: Floating-point numbers are used in the for loop
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Det_1707
      Reason: Function '${name}' is not using the same aliases.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */
#ifndef DET_H
#define DET_H

/* =================================================== inclusions =================================================== */
#include "Det_Types.h"
#include "Det_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
#define DET_AR_RELEASE_MAJOR_VERSION    (4u)
#define DET_AR_RELEASE_MINOR_VERSION    (9u)
#define DET_AR_RELEASE_REVISION_VERSION (0u)

#define DET_SW_MAJOR_VERSION (2u)
#define DET_SW_MINOR_VERSION (0u)
#define DET_SW_PATCH_VERSION (1u)

/* ===================================================== macros ===================================================== */
#define DET_VENDOR_ID   (62u)
#define DET_MODULE_ID   (15u)
#define DET_INSTANCE_ID (0u)

/* Development Errors */
#define DET_E_PARAM_POINTER (1u)

/* Define API service IDs */
#define DET_SID_GETVERSIONINFO (0x03u)

#define DET_MAX_BSW_MODULE_ID (4095u)

#ifndef DET_REPORT_ERROR_RECURSIONLIMIT
/* Used to report error recursion limit of Det module. */
#define DET_REPORT_ERROR_RECURSIONLIMIT 3u
#endif /* DET_REPORT_ERROR_RECURSIONLIMIT */

#ifndef DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT
/* Used to report error recursion limit of Det module. */
#define DET_REPORT_RUNTIMEERROR_RECURSIONLIMIT 3u
#endif /* DET_REPORT_ERROR_RECURSIONLIMIT */

#ifndef DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT
/* Used to report error recursion limit of Det module. */
#define DET_REPORT_TRANSIENTFAULT_RECURSIONLIMIT 3u
#endif /* DET_REPORT_ERROR_RECURSIONLIMIT */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
/* PRQA S 1512 ++ */ /* VL_Det_1512 */
extern const Det_ConfigType Det_Config;
/* PRQA S 1512 -- */

/* ========================================= external function declarations ========================================= */

/**
 * @brief          Service is used to initializes the Det module.
 * @param[in]      ConfigPtr: Pointer to the selected configuration structure.
 * @param[inout]   -
 * @param[out]     -
 * @return         -
 * @serviceID      0x00
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          CPD-64024
 */
extern void Det_Init(const Det_ConfigType* configPtr);

#if (STD_ON == DET_VERSION_INFO_API)
/**
 * @brief          Service is used to return the version information of this module.
 * @param[in]      -
 * @param[inout]   -
 * @param[out]     versioninfo: Pointer to where to store the version information of this module.
 * @return         -
 * @serviceID      0x03
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          CPD-64074
 */
extern void Det_GetVersionInfo(Std_VersionInfoType* versionInfoPtr);
#endif /* TD_ON == DET_VERSION_INFO_API */

/**
 * @brief          Service is used to start the Default Error Tracer.
 * @param[in]      -
 * @param[inout]   -
 * @param[out]     -
 * @return         -
 * @serviceID      0x02
 * @reentrant      FALSE
 * @synchronous    TRUE
 * @trace          CPD-64087
 */
extern void Det_Start(void);

/* PRQA S 1512,1513,1707 ++ */ /* VL_Det_1512,VL_Det_1513,VL_Det_1707 */
/**
 * @brief          Service is used to report development errors in Det module
 * @param[in]      moduleId: Module ID of calling module
 * @param[in]      instanceId: The identifier of the index based instance of a module
 * @param[in]      apiId: ID of API service in which error is detected
 * @param[in]      errorId: ID of detected development error
 * @param[inout]   -
 * @param[out]     -
 * @return         Std_ReturnType
 * @retval         never returns a value, but has a return type for compatibility with services and hooks
 * @serviceID      0x01
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace          CPD-64088
 */
extern Std_ReturnType Det_ReportError(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId);

/**
 * @brief          Service is used to report runtime errors in Det module
 * @param[in]      moduleId: Module ID of calling module
 * @param[in]      instanceId: The identifier of the index based instance of a module
 * @param[in]      apiId: ID of API service in which error is detected
 * @param[in]      errorId: ID of detected development error
 * @param[inout]   -
 * @param[out]     -
 * @return         Std_ReturnType
 * @retval         returns always E_OK (is required for services)
 * @serviceID      0x04
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace          CPD-64089
 */
extern Std_ReturnType Det_ReportRuntimeError(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 errorId);
/* PRQA S 1512,1513,1707 -- */

/**
 * @brief          Service is used to report runtime errors in Det module
 * @param[in]      moduleId: Module ID of calling module
 * @param[in]      instanceId: The identifier of the index based instance of a module
 * @param[in]      apiId: ID of API service in which error is detected
 * @param[in]      faultId: ID of detected transient fault.
 * @param[inout]   -
 * @param[out]     -
 * @return         Std_ReturnType
 * @retval         If no callout exists it shall return E_OK, otherwise it shall return the value of the configured
 * callout.
 * @serviceID      0x05
 * @reentrant      TRUE
 * @synchronous    TRUE
 * @trace          CPD-64090
 */
extern Std_ReturnType Det_ReportTransientFault(uint16 moduleId, uint8 instanceId, uint8 apiId, uint8 faultId);

#ifdef __cplusplus
}
#endif

#endif /* DET_H */
