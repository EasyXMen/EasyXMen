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
 **  @file               : RamTst.h
 **  @author             : peng.wu
 **  @date               : 2024/09/27
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of RamTst
 **
 ************************************************************************************** */
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V01.00.00 2019-09-14  Xin.Liu     Initial version
 *  V02.00.00 2022-12-21  Peng.Wu     Modified to version R19-11
 *  V02.00.01 2024-03-14  Peng.Wu     1.Update Schm and some global variables
 *                                    2.Fix header file including,CPT-8545
 *                                    3.Optimize test algorithm code, add QAC
 *  V02.00.02 2024-06-03  peng.wu     Update multiple subfunction for Abraham
 *  V03.00.00 2024-09-27  peng.wu     Update to R23-11
 *  V03.00.01 2025-02-25  peng.wu     CPT-12936, Fix config variable name
 *                                    CPT-13117, Fixed overflow when executing the last block
 *                                    CPT-13386,CPT-13388, Fixed coverage and Null pointer problem
 *                                    CPT-17248, Delete develop error report in main function
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:RamTst<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_RamTst_0306
      Reason: Cast between a pointer to object and an integral type.
      Risk: Statements in this 'if' block are not executed.
      Prevention: Code inspection and test of the different configurations in the component test.

    \li VL_RamTst_1503
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_1501
      Reason: The function '${name}' is declared but is not used within this project.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_RamTst_3472
      Reason: Macro definitions for easy reuse.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_5004
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_1532
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_3414
      Reason: specific keyword macro is necessary for unit testing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_1753
      Reason: The function '${name}' with external linkage is declared but not defined within this project.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_RamTst_1505
      Reason: external interface
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_4461
      Reason: Datatype conversion is necessary for functional usage
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_3223
      Reason: Bitmap initialization usage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_0490
      Reason: Need to compare whether the start address is within the range of the end address.
      Risk: The offset calculation of the pointer is fixed and only increases by 1, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_RamTst_0488
      Reason: Calculates the offset value of the array.
      Risk: The offset value is calculated with a fixed length, no risk.
      Prevention: Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_RamTst_0489
      Reason: ConfigId needs to be stored in the next uint address.
      Risk: The address of the operation is within the allowed range, no risk.
      Prevention:Correctness and reliability have been guaranteed through unit and functional testing.

    \li VL_RamTst_1258
      Reason: Necessary data processing
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_RamTst_STPTH
      Reason: Due to the implementation requirements of Abraham Algorithm functions, the functions were not
  separated into sub-functions
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_RamTst_STCAL
      Reason: The software structure of RamTst is defined according to the AUTOSAR standard,resulting in its metric
  being above the threshold
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_RamTst_3461
      Reason: A macro was defined to represent "static const", as during unit testing, this address variable can be
  modified.
      Risk: No risk.It is merely a macro definition, which is convenient for modification during unit testing
  through CMake. Prevention: Functional reliability guaranteed by design.

    \li VL_RamTst_3428
      Reason: A macro was defined to represent "const", as during unit testing, this address variable can be modified.
      Risk: No risk.It is merely a macro definition, which is convenient for modification during unit testing through
  CMake.
      Prevention: Functional reliability guaranteed by design.
 */

#ifndef RAMTST_H_
#define RAMTST_H_

/* =================================================== inclusions =================================================== */
#include "RamTst_Cfg.h"
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
#include "Det.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name RAMTST_VERSION_INFO
 * @{
 * @brief Identification and version information
 */
#define RAMTST_VENDOR_ID                   62u /**< Value of vendor */
#define RAMTST_MODULE_ID                   93u /**< Value of module */
#define RAMTST_INSTANCE_ID                 0u  /**< Value of instance */
#define RAMTST_AR_RELEASE_MAJOR_VERSION    4u  /**< Value of Autosar major version */
#define RAMTST_AR_RELEASE_MINOR_VERSION    9u  /**< Value of Autosar minor version */
#define RAMTST_AR_RELEASE_REVISION_VERSION 0u  /**< Value of Autosar patch version */
#define RAMTST_SW_MAJOR_VERSION            3u  /**< Value of module major version */
#define RAMTST_SW_MINOR_VERSION            0u  /**< Value of module minor version */
#define RAMTST_SW_PATCH_VERSION            0u  /**< Value of module patch version */

/* ===================================================== macros ===================================================== */
/* PRQA S 3414 ++ */ /* VL_QAC_FctLikeMacro */
#if !defined(RAMTST_LOCAL)
#define RAMTST_LOCAL static /**< Defines used as the local keyword for variables or functions */
#endif
/* PRQA S 3414 -- */

#if !defined(RAMTST_LOCAL_INLINE)
#define RAMTST_LOCAL_INLINE static inline /**< Defines used as keyword for the local inline functions */
#endif

#if !defined(RAMTST_PARAM_UNUSED)
#define RAMTST_PARAM_UNUSED(x) (void)(x) /**< Defines used as keyword for the unused variables */
#endif

/**
 * @name RAMTST_SERVICE_ID
 * @{
 * @brief SERVICE ID Definition
 */
#define RAMTST_INIT_ID                          0x00u /**< RamTst_Init API */
#define RAMTST_DEINIT_ID                        0x0Cu /**< RamTst_DeInit API */
#define RAMTST_STOP_ID                          0x02u /**< RamTst_Stop API */
#define RAMTST_ALLOW_ID                         0x03u /**< RamTst_Allow API */
#define RAMTST_SUSPEND_ID                       0x0Du /**< RamTst_Suspend API */
#define RAMTST_RESUME_ID                        0x0Eu /**< RamTst_Resume API */
#define RAMTST_GET_EXECUTION_STATUS_ID          0x04u /**< RamTst_GetExecutionStatus API */
#define RAMTST_GET_TEST_RESULT_ID               0x05u /**< RamTst_GetTestResult API */
#define RAMTST_GET_TEST_RESULT_PER_BLOCK_ID     0x06u /**< RamTst_GetTestResultPerBlock API */
#define RAMTST_GET_VERSION_INFO_ID              0x0Au /**< RamTst_GetVersionInfo API */
#define RAMTST_GET_ALG_PARAMS_ID                0x12u /**< RamTst_GetAlgParams API */
#define RAMTST_GET_TEST_ALGORITHM_ID            0x07u /**< RamTst_GetTestAlgorithm API */
#define RAMTST_GET_NUMBER_OF_TESTED_CELLS_ID    0x09u /**< RamTst_GetNumberOfTestedCells API */
#define RAMTST_SELECT_ALGPARAMS_ID              0x0Bu /**< RamTst_SelectAlgParams API */
#define RAMTST_CHANGE_NUMBER_OF_TESTED_CELLS_ID 0x08u /**< RamTst_ChangeNumberOfTestedCells API */
#define RAMTST_RUN_FULL_TEST_ID                 0x10u /**< RamTst_RunFullTest API */
#define RAMTST_RUN_PARTIAL_TEST_ID              0x11u /**< RamTst_RunPartialTest API */
#define RAMTST_GET_MAIN_FUNCTION_ID             0x01u /**< RamTst_MainFunction API */

/**
 * @name RAMTST_DEVELOP_ERROR_ID
 * @{
 * @brief Develop Error Definition
 */
#define RAMTST_E_NO_ERROR        0x00u /**< API function called with no det error */
#define RAMTST_E_STATUS_FAILURE  0x01u /**< A particular API is called in an unexpected state */
#define RAMTST_E_OUT_OF_RANGE    0x02u /**< API parameter out of specified range */
#define RAMTST_E_UNINIT          0x03u /**< API service used without module initialization */
#define RAMTST_E_PARAM_POINTER   0x04u /**< API service called with a NULL pointer */
#define RAMTST_E_SELF_CHECK_ERR  0x05u /**<  RamTst Self Check Err */
#define RAMTST_E_TIMEOUT         0x06u /**<  RamTst timeout */
#define RAMTST_E_WRONG_CONDITION 0x07u /**<  partition info error  */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
/**
 * @brief       Report Develop Error
 * @param[in]   ApiId: Specifies which API reported the error.
 * @param[in]   ErrorId: Specify which error was reported.
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77201
 */
RAMTST_LOCAL_INLINE void RamTst_Det_ReportError(uint8 ApiId, uint8 ErrorId)
{
    ((void)Det_ReportError(RAMTST_MODULE_ID, RAMTST_INSTANCE_ID, ApiId, ErrorId));
}
#endif

#if (RAMTST_ECUC_PARTITION_NUM > 1U)
/**
 * @brief       Get partition id of the algorithm parameters set
 * @param[in]   AlgParamsId: Identification of the algorithm
 * @return      ApplicationType
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77202
 */
RAMTST_LOCAL_INLINE ApplicationType RamTst_GetPartitionId(RamTst_AlgParamsIdType AlgParamsId)
{
    const RamTst_AlgParams* algotirhmParamsPtr = &RamTst_GetConfigAlgParams(AlgParamsId - 1u);
    return algotirhmParamsPtr->BlockParamsEcucPartitionRef;
}

/**
 * @brief       Check satellite partition current id
 * @param[in]   AlgParamsId: Identification of the algorithm
 * @return      boolean
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77203
 */
RAMTST_LOCAL_INLINE boolean RamTst_CheckPartition(RamTst_AlgParamsIdType AlgParamsId)
{
    return (boolean)(GetApplicationID() == RamTst_GetPartitionId(AlgParamsId));
}
#endif

/* PRQA S 1258 ++ */ /* VL_RamTst_1258 */
/**
 * @brief       Set the bit value to '1'
 * @param[in]   Value: Set value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77204
 */
RAMTST_LOCAL_INLINE uint32 RamTst_SetOne(uint32 Value, uint8 Index)
{
    return (uint32)((RAMTST_ONE << (Index)) | (Value));
}

/**
 * @brief       Set the bit value to '0'
 * @param[in]   Value: Set value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77205
 */
RAMTST_LOCAL_INLINE uint32 RamTst_SetZero(uint32 Value, uint8 Index)
{
    return (uint32)((~(RAMTST_ONE << (Index))) & (Value));
}

/**
 * @brief       Verify the bit in WalkPath algorithm
 * @param[in]   Value: Verify value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77206
 */
RAMTST_LOCAL_INLINE uint32 RamTst_WalkPathVerify(uint32 Value, uint8 Index)
{
    return (uint32)((RAMTST_ONE << (Index)) ^ (Value));
}

/**
 * @brief       Verify the bit value is valid
 * @param[in]   Value: Verify value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77207
 */
RAMTST_LOCAL_INLINE uint32 RamTst_Verify(uint32 Value, uint8 Index)
{
    return (uint32)(((RAMTST_ONE << (Index)) & (Value)) >> (Index));
}

/**
 * @brief       Set the bit to '1' in Galpat algorithm
 * @param[in]   Value: Set value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77208
 */
RAMTST_LOCAL_INLINE uint32 RamTst_GalpatSetPatternOne(uint32 Value, uint8 Index)
{
    return (uint32)((RAMTST_ONE << ((Index)-1u)) | (Value));
}

/**
 * @brief       Set the bit to '0' in Galpat algorithm
 * @param[in]   Value: Set value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77209
 */
RAMTST_LOCAL_INLINE uint32 RamTst_GalpatSetPatternZero(uint32 Value, uint8 Index)
{
    return (uint32)(~(RAMTST_ONE << (RamTst_GetMinNumberOfTestedCells - (Index))) & (Value));
}

/**
 * @brief       Check whether the remaining bit are all '1' in ascending order
 * @param[in]   Value: Check value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77210
 */
RAMTST_LOCAL_INLINE uint32 RamTst_GalpatCheckOnes(uint32 Value, uint8 Index)
{
    return (uint32)((RAMTST_ALL_ONE << (Index)) | (Value));
}

/**
 * @brief       Check whether the remaining bit are all '0' in ascending order
 * @param[in]   Value: Check value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77211
 */
RAMTST_LOCAL_INLINE uint32 RamTst_GalpatCheckZero(uint32 Value, uint8 Index)
{
    return (uint32)((RAMTST_ALL_ONE >> (RamTst_GetMinNumberOfTestedCells - (Index))) ^ (Value));
}

/**
 * @brief       Check whether the remaining bit are all '0' in descending order
 * @param[in]   Value: Check value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77212
 */
RAMTST_LOCAL_INLINE uint32 RamTst_GalpatCheckAllZero(uint32 Value, uint8 Index)
{
    return (uint32)(~(RAMTST_ALL_ONE << (RamTst_GetMinNumberOfTestedCells - (Index))) ^ (Value));
}

/**
 * @brief       Check whether the remaining bit are all '1' in descending order
 * @param[in]   Value: Check value
 * @param[in]   Index: Index value to be offset
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77213
 */
RAMTST_LOCAL_INLINE uint32 RamTst_GalpatCheckAllOne(uint32 Value, uint8 Index)
{
    return (uint32)((RAMTST_ALL_ONE << (RamTst_GetMinNumberOfTestedCells - (Index))) | (Value));
}
/* PRQA S 1258 -- */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief       Service for RAM Test initialization
 * @param[in]   ConfigPtr: Pointer to the selected configuration set
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77214
 */
extern void RamTst_Init(const RamTst_ConfigType* ConfigPtr);

/**
 * @brief       Service for RAM Test deinitialization
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77215
 */
extern void RamTst_DeInit(void);

#if (RAMTST_STOP_API_ENABLE == STD_ON)
/**
 * @brief       Service for stopping the RAM Test
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77216
 */
extern void RamTst_Stop(void);
#endif

#if RAMTST_ALLOW_API_ENABLE
/**
 * @brief       Service for continuing the RAM Test after calling RamTst_Stop
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77217
 */
extern void RamTst_Allow(void);
#endif

#if RAMTST_SUSPEND_API_ENABLE
/**
 * @brief       Service for suspending current operation of background RAM Test, until RESUME is called
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77218
 */
extern void RamTst_Suspend(void);
#endif

#if RAMTST_RESUME_API_ENABLE
/**
 * @brief       Service for allowing to continue the background RAM Test at the point is was suspended
 * @reentrant   Non Reentrant
 * @synchronous FALSE
 * @trace       CPD-77219
 */
extern void RamTst_Resume(void);
#endif

#if RAMTST_GET_EXECUTION_STATUS_API_ENABLE
/**
 * @brief       Service returns the current RAM Test execution status
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-77220
 */
extern RamTst_ExecutionStatusType RamTst_GetExecutionStatus(void);
#endif

#if RAMTST_GET_TEST_RESULT_API_ENABLE
/**
 * @brief       Service returns the current RAM Test result
 * @return      RamTst_TestResultType
 * @retval      RAMTST_RESULT_NOT_TESTED: The RAM Test has been tested with OK result
 * @retval      RAMTST_RESULT_OK: request has not been accepted
 * @retval      RAMTST_RESULT_NOT_OK: request has not been accepted
 * @retval      RAMTST_RESULT_UNDEFINED: request has not been accepted
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-77221
 */
extern RamTst_TestResultType RamTst_GetTestResult(void);
#endif

#if RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE
/**
 * @brief       Service returns the current RAM Test result for the specified block
 * @param[in]   BlockID: Identifies the block
 * @return      RamTst_TestResultType
 * @retval      RAMTST_RESULT_NOT_TESTED: The RAM Test has been tested with OK result
 * @retval      RAMTST_RESULT_OK: request has not been accepted
 * @retval      RAMTST_RESULT_NOT_OK: request has not been accepted
 * @retval      RAMTST_RESULT_UNDEFINED: request has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77222
 */
extern RamTst_TestResultType RamTst_GetTestResultPerBlock(RamTst_NumberOfBlocksType BlockID);
#endif

#if RAMTST_VERSIONINFO_API_ENABLE
/**
 * @brief       Service returns the version information of this module
 * @param[out]  VersionInfo: Pointer to the location / address where to store the version information of this module
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-77223
 */
extern void RamTst_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif

#if RAMTST_GET_ALGPARAMS_API_ENABLE
/**
 * @brief       Service returns the ID of the current RAM Test algorithm parameter set
 * @return      RamTst_AlgParamsIdType
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77224
 */
extern RamTst_AlgParamsIdType RamTst_GetAlgParams(void);
#endif

#if RAMTST_GET_TEST_ALGORITHM_API_ENABLE
/**
 * @brief       Service returns the current RAM Test algorithm
 * @return      RamTst_AlgorithmType
 * @retval      RAMTST_ALGORITHM_UNDEFINED: Undefined algorithm (uninitialized value)
 * @retval      RAMTST_CHECKERBOARD_TEST: Checkerboard test algorithm
 * @retval      RAMTST_MARCH_TEST: March test algorithm
 * @retval      RAMTST_WALK_PATH_TEST: Walk path test algorithm
 * @retval      RAMTST_GALPAT_TEST: Galpat test algorithm
 * @retval      RAMTST_TRANSP_GALPAT_TEST: Transparent Galpat test algorithm
 * @retval      RAMTST_ABRAHAM_TEST: Abraham test algorithm
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77225
 */
extern RamTst_AlgorithmType RamTst_GetTestAlgorithm(void);
#endif

#if RAMTST_GET_NUMBER_OF_TESTED_CELLS_API_ENABLE
/**
 * @brief       Service returns the current number of tested cells per main-function cycle
 * @return      RamTst_NumberOfTestedCellsType
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77226
 */
extern RamTst_NumberOfTestedCellsType RamTst_GetNumberOfTestedCells(void);
#endif

#if RAMTST_SELECT_ALGPARAMS_API_ENABLE
/**
 * @brief       Service used to set the test algorithm and its parameter set
 * @param[in]   NewAlgParamsId: Identifies the parameter set to be used
 * @return      RamTst_NumberOfTestedCellsType
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77227
 */
extern void RamTst_SelectAlgParams(RamTst_AlgParamsIdType NewAlgParamsId);
#endif

#if RAMTST_CHANGE_NUM_OF_TESTED_CELLS_API_ENABLE
/**
 * @brief       Service changes the current number of tested cells
 * @param[in]   NewNumberOfTestedCells: Identifies the number of tested cells
 * @return      RamTst_NumberOfTestedCellsType
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77228
 */
extern void RamTst_ChangeNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells);
#endif

#if RAMTST_RUN_FULL_TEST_API_ENABLE
/**
 * @brief       Service for executing the full RAM Test in the foreground
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77229
 */
extern void RamTst_RunFullTest(void);
#endif

#if RAMTST_RUN_PARTIAL_TEST_API_ENABLE
/**
 * @brief       Service for testing one RAM block in the foreground
 * @param[in]   BlockId: Identifies the single RAM block to be tested in the selected set of RamTstAlgParams
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77230
 */
extern void RamTst_RunPartialTest(RamTst_NumberOfBlocksType BlockId);
#endif

#if (RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON)
/**
 * @brief       The function RamTst_TestCompleted shall be called every time when all RAM blocks of the current test
 * configuration have been tested in the background
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77231
 */
extern void RamTst_TestCompletedNotification(void);
#endif

#if (RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON)
/**
 * @brief       The function RamTst_Error shall be called every time when a RAM failure has been detected by the
 * selected test algorithm in the background
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77232
 */
extern void RamTst_ErrorNotification(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
