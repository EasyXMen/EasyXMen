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
 **  @file               : FlsTst.h
 **  @author             : peng.wu
 **  @date               : 2024/10/30
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of FlsTst
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2021-09-16  Huangyin      Initial version
 *  V02.00.00 2022-12-21  Peng.Wu       Update to version R19-11
 *  V02.00.01 2023-10-25  pengwu        CPT-6553, FIx FlsTst_MemCompare function
 *  V02.00.02 2023-10-25  pengwu        1.CPT-8641, Fix compile problem when DET is disable
 *                                      2.Fix compile warning information
 *                                      3.CPT-8747, Fix FLSTST_E_INIT_FAILED report,add duplicated address
 *                                      4.Add Qac
 *  V03.00.00 2024-10-30  peng.wu       Update to R23-11
 *  V03.00.01 2025-02-25  peng.wu       CPT-12936, Fix config variable name
 *                                      CPT-13009,CPT-13008,CPT-13003, Fix error value in generate code
 *                                      CPT-13030,CPT-13031,Fix signature value initialization and obtain
 *                                      CPT-13050, Fix inconsistent algorithm processing under foreground/background
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:FlsTst<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_FlsTst_2669
      Reason: Need to access a certain piece of data in the array through the pointer.
      Risk: The offset value of the array buffer is calculated before it is used, no risk.
      Prevention: Code inspection and test of the different configurations in the component test.

    \li VL_FlsTst_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_1531
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_1505
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_0488
      Reason: Calculates the offset value of the array.
      Risk: The offset value is calculated with a fixed length, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_FlsTst_3451
      Reason: multiple declaration is necessary for RTE
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_3449
      Reason: multiple declaration is necessary for RTE
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_0306
      Reason: Necessary data processing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_5004
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_FlsTst_STCAL
      Reason: The software structure of FlsTst is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_FlsTst_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_3461
      Reason: Macro defines a storage-class specifier/type qualifier sequence.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_3223
      Reason: Bitmap initialization usage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_1502
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_1504
      Reason: Multi-partition variable design requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_3408
      Reason:Configuration parameter, design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_FlsTst_1753
      Reason: The function '${name}' with external linkage is declared but not defined within this project.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.
 */

#ifndef FLSTST_H
#define FLSTST_H

/* =================================================== inclusions =================================================== */
#include "FlsTst_Cfg.h"
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#if (FLSTST_MULTI_PARTITION == STD_ON)
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name FLSTST_VERSION_INFO
 * @{
 * @brief Identification and version information
 */
#define FLSTST_VENDOR_ID                   62u /**< Value of vendor */
#define FLSTST_MODULE_ID                   21u /**< Value of module */
#define FLSTST_INSTANCE_ID                 0u  /**< Value of instance */
#define FLSTST_AR_RELEASE_MAJOR_VERSION    4u  /**< Value of Autosar major version */
#define FLSTST_AR_RELEASE_MINOR_VERSION    9u  /**< Value of Autosar minor version */
#define FLSTST_AR_RELEASE_REVISION_VERSION 0u  /**< Value of Autosar patch version */
#define FLSTST_SW_MAJOR_VERSION            3u  /**< Value of module major version */
#define FLSTST_SW_MINOR_VERSION            0u  /**< Value of module minor version */
#define FLSTST_SW_PATCH_VERSION            0u  /**< Value of module patch version */

/* ===================================================== macros ===================================================== */
/* PRQA S 3414 ++ */ /* VL_FlsTst_3414 */
#if !defined(FLSTST_LOCAL)
#define FLSTST_LOCAL static /**< Defines used as the local keyword for variables or functions */
#endif
/* PRQA S 3414 -- */

#if !defined(FLSTST_LOCAL_INLINE)
#define FLSTST_LOCAL_INLINE static inline /**< Defines used as keyword for the local inline functions */
#endif

/**
 * @name FLSTST_SERVICE_ID
 * @{
 * @brief SERVICE ID Definition
 */
#define FLSTST_INIT_ID                 0x00u /**< FlsTst_Init Api Id */
#define FLSTST_DEINIT_ID               0x01u /**< FlsTst_DeInit Api Id */
#define FLSTST_STARTFGND_ID            0x02u /**< FlsTst_StartFgnd Api Id */
#define FLSTST_ABORT_ID                0x03u /**< FlsTst_Abort Api Id */
#define FLSTST_SUSPEND_ID              0x04u /**< FlsTst_Suspend Api Id */
#define FLSTST_RESUME_ID               0x05u /**< FlsTst_Resume Api Id */
#define FLSTST_GETCURRENTSTATE_ID      0x06u /**< FlsTst_GetCurrentState Api Id */
#define FLSTST_GETTESTRESULTBGND_ID    0x07u /**< FlsTst_GetTestResultBgnd Api Id */
#define FLSTST_GETTESTRESULTFGND_ID    0x0Fu /**< FlsTst_GetTestResultFgnd Api Id */
#define FLSTST_GETVERSIONINFO_ID       0x08u /**< FlsTst_GetVersionInfo Api Id */
#define FLSTST_GETTESTSIGNATUREBGND_ID 0x09u /**< FlsTst_GetTestSignatureBgnd Api Id */
#define FLSTST_GETTESTSIGNATUREFGND_ID 0x0Au /**< FlsTst_GetTestSignatureFgnd Api Id */
#define FLSTST_GETERRORDETAILS_ID      0x0Bu /**< FlsTst_GetErrorDetails Api Id */
#define FLSTST_TESTECC_ID              0x0Cu /**< FlsTst_TestEcc Api Id */
#define FLSTST_MAINFUNCTION_ID         0x0Du /**< FlsTst_Mainfunction Api Id */
#define FLSTST_TESTCOMPLETENOTIFY_ID   0x0Eu /**< FlsTst_TestCompletedNotification Api Id */

/**
 * @name FLSTST_DEVELOP_ERROR_ID
 * @{
 * @brief Develop Error Definition
 */
#define FLSTST_E_NO_ERROR            0x00u /**< API function called with no det error */
#define FLSTST_E_STATE_FAILURE       0x01u /**< Failure within Flash Test execution state */
#define FLSTST_E_PARAM_INVALID       0x02u /**< API parameter out of specified range */
#define FLSTST_E_UNINIT              0x03u /**< API service used without module initialization */
#define FLSTST_E_ALREADY_INITIALIZED 0x04u /**< Flash Test module is already initialized */
#define FLSTST_E_INIT_FAILED                                                                                           \
    0x05u /**< For Variant PB: Configuration pointer is a NULL pointer. For Variant PC: Configuration pointer is NOT a \
             NULL pointer. */
#define FLSTST_E_PARAM_POINTER   0x06u /**< Pointer is a NULL pointer */
#define FLSTST_E_TIMEOUT         0x07u /**< The process is timeout */
#define FLSTST_E_WRONG_CONDITION 0x08u /**< partition info error  */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
/**
 * @brief       Report Develop Error
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76993
 */
FLSTST_LOCAL_INLINE void FlsTst_Det_ReportError(uint8 ApiId, uint8 ErrorId)
{
    ((void)Det_ReportError(FLSTST_MODULE_ID, FLSTST_INSTANCE_ID, ApiId, ErrorId));
}
#endif

#if (STD_ON == FLSTST_MULTI_PARTITION)
/**
 * @brief       Check satellite partition current id
 * @return      boolean
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76994
 */
FLSTST_LOCAL_INLINE boolean FlsTst_CheckPartition(void)
{
    return (boolean)(GetApplicationID() == FLSTST_PARTITION_ID);
}
#endif

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Service for Flash Test initialization
 * @param[in]   ConfigPtr: Pointer to configuration set
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76995
 */
extern void FlsTst_Init(const FlsTst_ConfigType* ConfigPtr);

/**
 * @brief       Service for Flash Test De-Initialization
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76996R
 */
extern void FlsTst_DeInit(void);

/**
 * @brief       Service for executing foreground Flash Test
 * @param[in]   FgndBlockId: Number of the foreground test to be executed
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: Foreground test processed
 * @retval      E_NOT_OK: Foreground test not accepte
 * @trace       CPD-76997
 */
#if ((STD_ON == FLSTST_START_FGND_API) && (0u != FLSTST_FGND_BLOCK_NUM))
extern Std_ReturnType FlsTst_StartFgnd(FlsTst_BlockIdFgndType FgndBlockId);
#endif

/**
 * @brief       Service for aborting the Flash Test
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76998
 */
#if (0u != FLSTST_BGND_BLOCK_NUM)
extern void FlsTst_Abort(void);
#endif

/**
 * @brief       Service for suspending current operation of the Flash Test, until FlsTst_Resume is called
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76999
 */
#if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0u != FLSTST_BGND_BLOCK_NUM))
extern void FlsTst_Suspend(void);
#endif

/**
 * @brief       Service for continuing the Flash Test at the point it was suspended
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77000
 */
#if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0u != FLSTST_BGND_BLOCK_NUM))
extern void FlsTst_Resume(void);
#endif

/**
 * @brief       Service returns the current Flash Test exection state
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @return      FlsTst_StateType
 * @retval      FLSTST_UNINIT: The Flash Test is not initialized or not usable
 * @retval      FLSTST_INIT: The Flash Test is initialized and ready to be started
 * @retval      FLSTST_RUNNING: The Flash Test is currently running
 * @retval      FLSTST_ABORTED: The Flash Test is aborted
 * @retval      FLSTST_SUSPENDED: The Flash Test is waiting to be resumed or is waiting to start forground mode test
 * @trace       CPD-77001
 */
#if (STD_ON == FLSTST_GET_CURRENT_STATE_API)
extern FlsTst_StateType FlsTst_GetCurrentState(void);
#endif

/**
 * @brief       Service returns the Background Flash Test result
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @return      FlsTst_TestResultBgndType
 * @retval      FlsTstIntervalID: current value of FlsTstTestIntervalId, which is incremented by each new start of
 * an test interval
 * @retval      result: Test Result in background flash test
 * @trace       CPD-77002
 */
#if ((STD_ON == FLSTST_GET_TEST_RESULT_BGND_API) && (0u != FLSTST_BGND_BLOCK_NUM))
extern FlsTst_TestResultBgndType FlsTst_GetTestResultBgnd(void);
#endif

/**
 * @brief       Service returns the Foreground Flash Test result
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @return      FlsTst_TestResultFgndType
 * @retval      FLSTST_NOT_TESTED: There is no result availabl
 * @retval      FLSTST_OK: The last Flash Test has been tested with OK result
 * @retval      FLSTST_NOT_OK: The last Flash Test has been tested with NOT_OK result
 * @trace       CPD-77003
 */
#if ((STD_ON == FLSTST_GET_TEST_RESULT_FGND_API) && (0u != FLSTST_FGND_BLOCK_NUM))
extern FlsTst_TestResultFgndType FlsTst_GetTestResultFgnd(void);
#endif

/**
 * @brief       Service returns the version information of this module
 * @param[out]  versionInfo: Pointer to where to store the version information of this module
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-77004
 */
#if (STD_ON == FLSTST_VERSION_INFO_API)
extern void FlsTst_GetVersionInfo(Std_VersionInfoType* versionInfo);
#endif

/**
 * @brief       Service returns the Foreground Flash Test result
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @return      FlsTst_TestSignatureBgndType
 * @retval      FlsTstIntervalID: current value of FlsTstTestIntervalId, which is incremented by each new start of
 * an test interval
 * @retval      BgndSignature: It represents the signature value of the last completed test interval
 * @trace       CPD-77005
 */
#if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_BGND_API) && (0u != FLSTST_BGND_BLOCK_NUM))
extern FlsTst_TestSignatureBgndType FlsTst_GetTestSignatureBgnd(void);
#endif

/**
 * @brief       Service returns the Flash Test result in background
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @return      FlsTst_TestSignatureBgndType
 * @retval      FlsTstIntervalID: current value of FlsTstTestIntervalId, which is incremented by each new start of
 * an test interval
 * @retval      BgndSignature: It represents the signature value of the last completed test interval
 * @trace       CPD-77006
 */
#if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_FGND_API) && (0u != FLSTST_FGND_BLOCK_NUM))
extern FlsTst_TestSignatureFgndType FlsTst_GetTestSignatureFgnd(void);
#endif

/**
 * @brief       Service returns error detais monitored from the Flash module
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @return      FlsTst_ErrorDetailsType
 * @retval      ErrorBlockID: Record the block ID value when the error occurred
 * @retval      Algorithm: The test algorithm used by the current block
 * @retval      SignatureResult: The signature result of the current block
 * @trace       CPD-77007
 */
#if ((STD_ON == FLSTST_GET_ERRORD_ETAILS_API) && ((0u != FLSTST_BGND_BLOCK_NUM) || (0u != FLSTST_FGND_BLOCK_NUM)))
extern FlsTst_ErrorDetailsType FlsTst_GetErrorDetails(void);
#endif

/**
 * @brief       Service executes a test of ECC hardware. This is only applicable in case the hardware provices such
 * functionality
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: ECC test processed
 * @retval      E_NOT_OK: ECC test not accepte
 * @trace       CPD-77008
 */
#if ((STD_ON == FLSTST_TEST_ECC_API) && ((0u != FLSTST_BGND_BLOCK_NUM) || (0u != FLSTST_FGND_BLOCK_NUM)))
extern Std_ReturnType FlsTst_TestEcc(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
