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
 **  @file               : RamTst.c
 **  @author             : peng.wu
 **  @date               : 2024/09/27
 **  @vendor             : isoft
 **  @description        : Implementation for RamTst
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "RamTst.h"
#include "SchM_RamTst.h"
#if (RAMTST_DEM_MAIN_RAM_FAILURE_ENABLE == STD_ON) || (RAMTST_DEM_PART_RAM_FAILURE_ENABLE == STD_ON) \
    || (RAMTST_DEM_RUNFL_RAM_FAILURE_ENABLE == STD_ON)
#include "Dem.h"
#endif
#if (RAMTST_TEST_TIMEOUT == STD_ON)
#include "FreeRTimer.h"
#endif
#if (RAMTST_ECUC_PARTITION_NUM > 1U)
#include "Os.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
/**
 * @brief       Subfunction of Checkerboard algotirhm
 * @param[in]   testRam: Pointer to test Ram address
 * @param[in]   testResult: Test result
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77234
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoCheckerboardAlgorithmSub(uint32* testRam, Std_ReturnType testResult);

/**
 * @brief       Execution of March algotirhm
 * @param[in]   CurRamStartAddr: Pointer to current start Ram address
 * @param[in]   CurRamEndAddr: Pointer to current end Ram address
 * @param[in]   FillPattern: Filled value
 * @param[in]   TestPolicy: Test policy
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77235
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoMarchAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy);

/**
 * @brief       Execution of Checkerboard algotirhm
 * @param[in]   CurRamStartAddr: Pointer to current start Ram address
 * @param[in]   CurRamEndAddr: Pointer to current end Ram address
 * @param[in]   FillPattern: Filled value
 * @param[in]   TestPolicy: Test policy
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77236
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoCheckerboardAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy);

/**
 * @brief       Execution of Walk Path algotirhm
 * @param[in]   CurRamStartAddr: Pointer to current start Ram address
 * @param[in]   CurRamEndAddr: Pointer to current end Ram address
 * @param[in]   FillPattern: Filled value
 * @param[in]   TestPolicy: Test policy
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77237
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoWalkPathAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy);

/**
 * @brief       Execution of Golpat algotirhm
 * @param[in]   CurRamStartAddr: Pointer to current start Ram address
 * @param[in]   CurRamEndAddr: Pointer to current end Ram address
 * @param[in]   FillPattern: Filled value
 * @param[in]   TestPolicy: Test policy
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77238
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoGalpatAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy);

/**
 * @brief       Execution of TransGalpat algotirhm
 * @param[in]   CurRamStartAddr: Pointer to current start Ram address
 * @param[in]   CurRamEndAddr: Pointer to current end Ram address
 * @param[in]   FillPattern: Filled value
 * @param[in]   TestPolicy: Test policy
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77239
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoTransGalpatAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy);

/**
 * @brief       Execution of Abraham algotirhm
 * @param[in]   CurRamStartAddr: Pointer to current start Ram address
 * @param[in]   CurRamEndAddr: Pointer to current end Ram address
 * @param[in]   FillPattern: Filled value
 * @param[in]   TestPolicy: Test policy
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77240
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoAbrahamAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy);

/**
 * @brief       Execution the test
 * @param[in]   UsedAlgorithm: The algorithm currently used
 * @param[in]   CurRamStartAddr: Pointer to current start Ram address
 * @param[in]   CurRamEndAddr: Pointer to current end Ram address
 * @param[in]   FillPattern: Filled value
 * @param[in]   TestPolicy: Test policy
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77241
 */
RAMTST_LOCAL Std_ReturnType RamTst_ExecTest(
    const RamTst_AlgorithmType UsedAlgorithm,
    uint32*                    CurRamStartAddr,
    const uint32*              CurRamEndAddr,
    const uint32               FillPattern,
    const RamTst_Policy        TestPolicy);

#if (STD_ON == RAMTST_SELF_CHECK_ENABLE)
/**
 * @brief       Execution the self test
 * @param[in]   u32CStartAddr: Pointer to current start Ram address
 * @param[in]   u32CEndAddr: Pointer to current end Ram address
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77242
 */
RAMTST_LOCAL Std_ReturnType RamTst_DoSelfCheckTestAction(uint32* u32CStartAddr, const uint32* u32CEndAddr);
#endif

/**
 * @brief       Initialization all blocks status
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77243
 */
RAMTST_LOCAL void RamTst_InitAllBlockStatus(void);

#if (                                                            \
    ((STD_ON == RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE)     \
     || (STD_ON == RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE)) \
    && (STD_ON == RAMTST_DEV_ERROR_DETECT))
/**
 * @brief       Check the block ID is out of range
 * @param[in]   BlockID: The currently used block ID
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77244
 */
RAMTST_LOCAL Std_ReturnType RamTst_CheckBlockIDIsOutOfRange(RamTst_NumberOfBlocksType BlockID);
#endif

#if (STD_ON == RAMTST_RUN_FULL_TEST_API_ENABLE)
/**
 * @brief       Subfunction of full test
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77245
 */
RAMTST_LOCAL void RamTst_RunFullTestSub(void);
#endif

#if (STD_ON == RAMTST_RUN_PARTIAL_TEST_API_ENABLE)
/**
 * @brief       Subfunction of partitial test
 * @param[in]   BlockId: The currently used block ID
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77246
 */
RAMTST_LOCAL void RamTst_RunPartialTestSub(RamTst_NumberOfBlocksType BlockId);
#endif

/**
 * @brief       Subfunction of Mainfunction
 * @return      boolean
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77247
 */
RAMTST_LOCAL boolean RamTst_MainFunctionSub(void);

/**
 * @brief       Update the test result
 * @param[in]   testResult: The test result
 * @param[in]   algotirhmParamsPtr: Pointer to the algorithm parameters
 * @param[in]   blockParamsPtr: Pointer to the block parameters
 * @return      boolean
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77248
 */
RAMTST_LOCAL boolean RamTst_UpdateResult(
    Std_ReturnType            testResult,
    const RamTst_AlgParams*   algotirhmParamsPtr,
    const RamTst_BlockParams* blockParamsPtr);

/**
 * @brief       Check background test condition is balid
 * @return      boolean
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77249
 */
RAMTST_LOCAL boolean RamTst_CheckBgndCondIsValid(void);

/**
 * @brief       Check the algorithm parameters set ID
 * @param[in]   NewAlgParamsId: The algorithm parameters set ID
 * @param[out]  foundAlgParams: Pointer to the algorithm parameters
 * @return      boolean
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77250
 */
RAMTST_LOCAL boolean RamTst_CheckAlgParamsId(RamTst_AlgParamsIdType NewAlgParamsId, boolean* foundAlgParams);

/**
 * @brief       Step one of Abraham algorithm
 * @param[in]   testRam: Pointer to the test Ram address
 * @param[out]  testResult: Pointer to the test result
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77251
 */
RAMTST_LOCAL void RamTest_AbrahamAlgorithmStepOne(uint32* testRam, Std_ReturnType* testResult);

/**
 * @brief       Step two of Abraham algorithm
 * @param[in]   testRam: Pointer to the test Ram address
 * @param[out]  testResult: Pointer to the test result
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77252
 */
RAMTST_LOCAL void RamTest_AbrahamAlgorithmStepTwo(uint32* testRam, Std_ReturnType* testResult);

/**
 * @brief       Step three of Abraham algorithm
 * @param[in]   testRam: Pointer to the test Ram address
 * @param[out]  testResult: Pointer to the test result
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77253
 */
RAMTST_LOCAL void RamTest_AbrahamAlgorithmStepThree(uint32* testRam, Std_ReturnType* testResult);

/**
 * @brief       Step four of Abraham algorithm
 * @param[in]   testRam: Pointer to the test Ram address
 * @param[out]  testResult: Pointer to the test result
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77254
 */
RAMTST_LOCAL void RamTest_AbrahamAlgorithmStepFour(uint32* testRam, Std_ReturnType* testResult);
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define RAMTST_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "RamTst_MemMap.h"
/**
 * @brief Used to record the management data of each Block @range NA
 */
RAMTST_LOCAL RamTst_BlockInfoType RamTst_BlockInfo[RAMTST_ALL_BLOCK_NUM + 1u];
#define RAMTST_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "RamTst_MemMap.h"

#define RAMTST_START_SEC_VAR_INIT_32
#include "RamTst_MemMap.h"
/**
 * @brief Record the current block index @range 0..0xFFFFFFFF
 */
RAMTST_LOCAL uint32 RamTst_CurBlockIndex = 1u;

/**
 * @brief Record the tested cell number @range 0..0xFFFFFFFF
 */
RAMTST_LOCAL RamTst_NumberOfTestedCellsType RamTst_NumberOfTestedCell = 1u;
#define RAMTST_STOP_SEC_VAR_INIT_32
#include "RamTst_MemMap.h"

#define RAMTST_START_SEC_VAR_INIT_8
#include "RamTst_MemMap.h"
/**
 * @brief Record the current algorithm params index @range 0..255
 */
RAMTST_LOCAL RamTst_AlgParamsIdType RamTst_CurAlgParamsId = 0u;
#define RAMTST_STOP_SEC_VAR_INIT_8
#include "RamTst_MemMap.h"

#define RAMTST_START_SEC_VAR_CLEARED_8
#include "RamTst_MemMap.h"
/**
 * @brief Record the status of the RamTst module @range NA
 */
RAMTST_LOCAL RamTst_ExecutionStatusType RamTst_CuExecStatus;

/**
 * @brief Record the result of the overall test @range NA
 */
RAMTST_LOCAL RamTst_TestResultType RamTst_OverallResult;
#define RAMTST_STOP_SEC_VAR_CLEARED_8
#include "RamTst_MemMap.h"

#define RAMTST_START_SEC_VAR_CLEARED_32
#include "RamTst_MemMap.h"
/**
 * @brief Record the current tested block number @range 0..0xFFFFFFFF
 */
RAMTST_LOCAL uint32 RamTst_CurTestedBlock;
#define RAMTST_STOP_SEC_VAR_CLEARED_32
#include "RamTst_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define RAMTST_START_SEC_CODE
#include "RamTst_MemMap.h"
/* PRQA S 1252,1258,1503,1532,3120 ++ */ /* VL_QAC_1252,VL_RamTst_1258,VL_RamTst_1503,VL_RamTst_1532,VL_RamTst_3120 */
/**
 * @brief Check initialization status and do self check, then initialize global variables and all state machines
 */
void RamTst_Init(const RamTst_ConfigType* ConfigPtr)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    if (RAMTST_EXECUTION_UNINIT != RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
#if (STD_ON == RAMTST_SELF_CHECK_ENABLE)
    else if (E_OK != RamTst_DoSelfCheckTestAction(RamTst_GetLocalVarStartAddr, RamTst_GetLocalVarEndAddr))
    {
        /** self check */
        errorId = RAMTST_E_SELF_CHECK_ERR;
    }
#endif
    else
#endif
    {
        RamTst_OverallResult      = RAMTST_RESULT_NOT_TESTED;
        RamTst_CurAlgParamsId     = RamTst_GetDefaultAlgParamsId;
        RamTst_CuExecStatus       = RAMTST_EXECUTION_STOPPED;
        RamTst_AlgParams algParam = RamTst_GetConfigAlgParams(RamTst_CurAlgParamsId - 1u);
        RamTst_NumberOfTestedCell = algParam.NumberOfTestedCells;
        RamTst_CurBlockIndex      = algParam.BlockParams[0].BlockId;
        for (uint32 index = 0u; index < RAMTST_ALL_BLOCK_NUM; index++) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            RamTst_BlockInfo[index].BlockTestResult = RAMTST_RESULT_NOT_TESTED;
        }
        RamTst_InitAllBlockStatus();
        RAMTST_PARAM_UNUSED(ConfigPtr);
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_INIT_ID, errorId);
    }
#endif
}

/**
 * @brief If the module has already been initialized, reset all global variables and all state machines
 */
void RamTst_DeInit(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        RamTst_Det_ReportError((uint8)(RAMTST_DEINIT_ID), (RAMTST_E_UNINIT));
    }
    else
#endif
    {
        for (uint32 index = 0u; index < RAMTST_ALL_BLOCK_NUM; index++) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            RamTst_BlockInfo[index].BlockTestResult = RAMTST_RESULT_NOT_TESTED;
        }
        RamTst_CurAlgParamsId = 0u;
        RamTst_OverallResult  = RAMTST_RESULT_NOT_TESTED;
        RamTst_CuExecStatus   = RAMTST_EXECUTION_UNINIT;
        RamTst_InitAllBlockStatus();
        RamTst_CurBlockIndex      = 1u;
        RamTst_NumberOfTestedCell = 1u;
    }
}

#if (STD_ON == RAMTST_STOP_API_ENABLE)
/**
 * @brief Switch the current state to stop, reset the global variables, set the current algorithm id to default value
 */
void RamTst_Stop(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if ((RAMTST_EXECUTION_RUNNING != RamTst_CuExecStatus) && (RAMTST_EXECUTION_SUSPENDED != RamTst_CuExecStatus))
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
    else
#endif
    {
        RamTst_CuExecStatus       = RAMTST_EXECUTION_STOPPED;
        RamTst_AlgParams algParam = RamTst_GetConfigAlgParams(RamTst_CurAlgParamsId - 1u);
        /** The test result is retained, but test parameters and loop data are not.  SWS_RamTst_00014 */
        RamTst_CurBlockIndex = algParam.BlockParams[0].BlockId;
        RamTst_InitAllBlockStatus();
        RamTst_CurAlgParamsId = RamTst_GetDefaultAlgParamsId;
        RamTst_OverallResult  = RAMTST_RESULT_NOT_TESTED;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_STOP_ID, errorId);
    }
#endif
}
#endif

#if (STD_ON == RAMTST_ALLOW_API_ENABLE)
/**
 * @brief Switch the current state to running if the current status is not stoppped
 */
void RamTst_Allow(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if (RAMTST_EXECUTION_STOPPED != RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
    else
#endif
    {
        RamTst_CuExecStatus = RAMTST_EXECUTION_RUNNING;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_ALLOW_ID, errorId);
    }
#endif
}
#endif

#if (STD_ON == RAMTST_SUSPEND_API_ENABLE)
/**
 * @brief Switch the current state to suspended
 */
void RamTst_Suspend(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if (RAMTST_EXECUTION_RUNNING != RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
    else
#endif
    {
        RamTst_CuExecStatus = RAMTST_EXECUTION_SUSPENDED;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_SUSPEND_ID, errorId);
    }
#endif
}
#endif

#if (STD_ON == RAMTST_RESUME_API_ENABLE)
/**
 * @brief Switch the current state to running if the current status is not suspended
 */
void RamTst_Resume(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if (RAMTST_EXECUTION_SUSPENDED != RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
    else
#endif
    {
        RamTst_CuExecStatus = RAMTST_EXECUTION_RUNNING;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_RESUME_ID, errorId);
    }
#endif
}
#endif

#if (STD_ON == RAMTST_GET_EXECUTION_STATUS_API_ENABLE)
/**
 * @brief Return the recorded current status
 */
RamTst_ExecutionStatusType RamTst_GetExecutionStatus(void)
{
    RamTst_ExecutionStatusType retVal = RAMTST_EXECUTION_UNINIT;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        RamTst_Det_ReportError((uint8)(RAMTST_GET_EXECUTION_STATUS_ID), (RAMTST_E_UNINIT));
    }
    else
#endif
    {
        retVal = RamTst_CuExecStatus;
    }

    return retVal;
}
#endif

#if (STD_ON == RAMTST_GET_TEST_RESULT_API_ENABLE)
/**
 * @brief Return the recorded overall test result
 */
RamTst_TestResultType RamTst_GetTestResult(void)
{
    RamTst_TestResultType retVal = RAMTST_RESULT_NOT_TESTED;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        RamTst_Det_ReportError((uint8)RAMTST_GET_TEST_RESULT_ID, RAMTST_E_UNINIT);
    }
    else
#endif
    {
        retVal = RamTst_OverallResult;
    }

    return retVal;
}
#endif

#if (STD_ON == RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE)
/**
 * @brief Return the recorded test result according the passed BlockId
 */
RamTst_TestResultType RamTst_GetTestResultPerBlock(RamTst_NumberOfBlocksType BlockID)
{
    RamTst_TestResultType retVal = RAMTST_RESULT_NOT_TESTED;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    /** SWS_RamTst_00039 */
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if ((Std_ReturnType)E_NOT_OK == RamTst_CheckBlockIDIsOutOfRange(BlockID))
    {
        errorId = RAMTST_E_OUT_OF_RANGE;
        retVal  = RAMTST_RESULT_UNDEFINED;
    }
    else
#endif
    {
        retVal = RamTst_BlockInfo[BlockID].BlockTestResult;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_GET_TEST_RESULT_PER_BLOCK_ID, errorId);
    }
#endif

    return retVal;
}
#endif

#if (STD_ON == RAMTST_VERSIONINFO_API_ENABLE)
/**
 * @brief Return the version information
 */
void RamTst_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        RamTst_Det_ReportError((uint8)(RAMTST_GET_VERSION_INFO_ID), (RAMTST_E_PARAM_POINTER));
    }
    else
#endif
    {
        VersionInfo->vendorID         = (uint16)RAMTST_VENDOR_ID;
        VersionInfo->moduleID         = (uint16)RAMTST_MODULE_ID;
        VersionInfo->sw_major_version = (uint8)RAMTST_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = (uint8)RAMTST_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = (uint8)RAMTST_SW_PATCH_VERSION;
    }
}
#endif

#if (STD_ON == RAMTST_GET_ALGPARAMS_API_ENABLE)
/**
 * @brief Return the recorded current algorithm parameters set ID
 */
RamTst_AlgParamsIdType RamTst_GetAlgParams(void)
{
    RamTst_AlgParamsIdType retVal = 0u;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        RamTst_Det_ReportError((uint8)(RAMTST_GET_ALG_PARAMS_ID), (RAMTST_E_UNINIT));
    }
    else
#endif
    {
        retVal = RamTst_CurAlgParamsId;
    }

    return retVal;
}
#endif

#if (STD_ON == RAMTST_GET_TEST_ALGORITHM_API_ENABLE)
/**
 * @brief Return the current algorithm in its parameters set
 */
RamTst_AlgorithmType RamTst_GetTestAlgorithm(void)
{
    RamTst_AlgorithmType retVal = RAMTST_ALGORITHM_UNDEFINED;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        RamTst_Det_ReportError((uint8)(RAMTST_GET_TEST_ALGORITHM_ID), RAMTST_E_UNINIT);
    }
    else
#endif
    {
        RamTst_AlgParamsIdType algParamsId = RamTst_CurAlgParamsId - 1u;
        retVal                             = RamTst_GetConfigAlgParams(algParamsId).Algorithm;
    }

    return retVal;
}
#endif

#if (STD_ON == RAMTST_GET_NUMBER_OF_TESTED_CELLS_API_ENABLE)
/**
 * @brief Return the currently used number of tested cells
 */
RamTst_NumberOfTestedCellsType RamTst_GetNumberOfTestedCells(void)
{
    RamTst_NumberOfTestedCellsType retVal = 0u;

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        RamTst_Det_ReportError((uint8)(RAMTST_GET_NUMBER_OF_TESTED_CELLS_ID), (RAMTST_E_UNINIT));
    }
    else
#endif
    {
        retVal = RamTst_NumberOfTestedCell;
    }

    return retVal;
}
#endif

#if (STD_ON == RAMTST_SELECT_ALGPARAMS_API_ENABLE)
/**
 * @brief Set new algorithm parameters Id and reset all global varibales
 */
void RamTst_SelectAlgParams(RamTst_AlgParamsIdType NewAlgParamsId)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8   errorId        = RAMTST_E_NO_ERROR;
    boolean foundAlgParams = FALSE;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if (RAMTST_EXECUTION_STOPPED != RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
    else if (FALSE == (RamTst_CheckAlgParamsId(NewAlgParamsId, &foundAlgParams)))
    {
        errorId = RAMTST_E_OUT_OF_RANGE;
    }
    else
#endif
    {
        RamTst_CurAlgParamsId = NewAlgParamsId;
        RamTst_InitAllBlockStatus();
        RamTst_AlgParams algParam = RamTst_GetConfigAlgParams(RamTst_CurAlgParamsId - 1u);
        RamTst_NumberOfTestedCell = algParam.NumberOfTestedCells;
        RamTst_CurBlockIndex      = algParam.BlockParams[0].BlockId;
        RamTst_CurTestedBlock     = 0u;
        RamTst_OverallResult      = RAMTST_RESULT_NOT_TESTED;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_SELECT_ALGPARAMS_ID, errorId);
    }
#endif
}
#endif

#if (STD_ON == RAMTST_CHANGE_NUM_OF_TESTED_CELLS_API_ENABLE)
/**
 * @brief Set new number of tested cells if the number is valid
 */
void RamTst_ChangeNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8                  errorId     = RAMTST_E_NO_ERROR;
    RamTst_AlgParamsIdType algParamsId = RamTst_CurAlgParamsId - 1u;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if (RAMTST_EXECUTION_STOPPED != RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
    else if (
        (RamTst_GetMinNumberOfTestedCells > NewNumberOfTestedCells)
        || (RamTst_GetMaxNumberOfTestedCells(algParamsId) < NewNumberOfTestedCells))
    {
        errorId = RAMTST_E_OUT_OF_RANGE;
    }
    else if (0u != (NewNumberOfTestedCells % 4u)) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        /** NewNumberOfTestedCells Must be Multiple of 4 */
        errorId = RAMTST_E_OUT_OF_RANGE;
    }
    else
#endif
    {
        RamTst_NumberOfTestedCell = NewNumberOfTestedCells;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_CHANGE_NUMBER_OF_TESTED_CELLS_ID, errorId);
    }
#endif
}
#endif

#if (STD_ON == RAMTST_RUN_FULL_TEST_API_ENABLE)
/**
 * @brief Set the current state to running, execute the full test, after the test, set the current state to stop
 */
void RamTst_RunFullTest(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if (RAMTST_EXECUTION_STOPPED != RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
#if (RAMTST_ECUC_PARTITION_NUM > 1U)
    else if (FALSE == (RamTst_CheckPartition(RamTst_CurAlgParamsId)))
    {
        errorId = RAMTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        RamTst_CuExecStatus = RAMTST_EXECUTION_RUNNING;
        RamTst_RunFullTestSub();
        /** set it back to RAMTST_EXECUTION_STOPPED before returning */
        RamTst_CuExecStatus = RAMTST_EXECUTION_STOPPED;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_RUN_FULL_TEST_ID, errorId);
    }
#endif
}
#endif

#if (STD_ON == RAMTST_RUN_PARTIAL_TEST_API_ENABLE)
/**
 * @brief Stores the value of the current state, then sets the state to running, executes the test based on the blockId
 * passed in, logs the test results, and restores the value of the current state when the test is over
 */
void RamTst_RunPartialTest(RamTst_NumberOfBlocksType BlockId)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
    else if ((RAMTST_EXECUTION_STOPPED != RamTst_CuExecStatus) && (RAMTST_EXECUTION_SUSPENDED != RamTst_CuExecStatus))
    {
        errorId = RAMTST_E_STATUS_FAILURE;
    }
    else if ((Std_ReturnType)E_NOT_OK == RamTst_CheckBlockIDIsOutOfRange(BlockId))
    {
        errorId = RAMTST_E_OUT_OF_RANGE;
    }
#if (RAMTST_ECUC_PARTITION_NUM > 1U)
    else if (FALSE == (RamTst_CheckPartition(RamTst_CurAlgParamsId)))
    {
        errorId = RAMTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        /** save the old test exec status */
        RamTst_ExecutionStatusType partialTestStatus = RamTst_CuExecStatus;
        RamTst_CuExecStatus                          = RAMTST_EXECUTION_RUNNING;
        RamTst_RunPartialTestSub(BlockId);
        /** set it back to the previous state (the state when the function was called) */
        RamTst_CuExecStatus = partialTestStatus;
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_RUN_PARTIAL_TEST_ID, errorId);
    }
#endif
}
#endif

/**
 * @brief Check whether the condition is valid, and execute the background test, othersize skip to next block
 */
void RamTst_MainFunction(void)
{
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    uint8 errorId = RAMTST_E_NO_ERROR;
    if (RAMTST_EXECUTION_UNINIT == RamTst_CuExecStatus)
    {
        errorId = RAMTST_E_UNINIT;
    }
#if (RAMTST_ECUC_PARTITION_NUM > 1U)
    else if (FALSE == (RamTst_CheckPartition(RamTst_CurAlgParamsId)))
    {
        errorId = RAMTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        boolean                 allTestdone        = FALSE;
        RamTst_AlgParamsIdType  algParamsId        = RamTst_CurAlgParamsId - 1u;
        const RamTst_AlgParams* algotirhmParamsPtr = &RamTst_GetConfigAlgParams(algParamsId);

        if (RAMTST_EXECUTION_RUNNING == RamTst_CuExecStatus)
        {
            boolean TestCondMatch = RamTst_CheckBgndCondIsValid();
            /** match the test conditions */
            if (TRUE == TestCondMatch)
            {
                allTestdone = RamTst_MainFunctionSub();
            }
            else
            {
                /** this block not match the test conditions or this block has tested, test next block */
                /* PRQA S 1252 ++ */ /* VL_QAC_1252 */
                if ((uint16)(RamTst_CurTestedBlock + 1u) <= algotirhmParamsPtr->NumberOfBlocks)
                /* PRQA S 1252 -- */
                {
                    RamTst_CurTestedBlock++;
                    RamTst_CurBlockIndex++;
#if (STD_ON == RAMTST_DEM_MAIN_RAM_FAILURE_ENABLE)
                    if (RamTst_OverallResult == RAMTST_RESULT_OK)
                    {
                        /** report the production error RAMTST_MAIN_RAM_FAILURE as pass */
                        Dem_SetEventStatus(RAMTST_MAIN_RAM_FAILURE, DEM_EVENT_STATUS_PASSED);
                    }
#endif
                }
                else
                {
                    allTestdone = TRUE;
                }
            }
        }
        /** check if all test has finished */
        if (TRUE == allTestdone)
        {
            /** Once you've tested over, start over from the first block */
            RamTst_CurBlockIndex  = algotirhmParamsPtr->BlockParams[0].BlockId;
            RamTst_CurTestedBlock = 0u;
#if (RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON)
            RamTst_TestCompletedNotification();
#endif
            RamTst_InitAllBlockStatus();
        }
    }

#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
    if (errorId != RAMTST_E_NO_ERROR)
    {
        RamTst_Det_ReportError((uint8)RAMTST_GET_MAIN_FUNCTION_ID, errorId);
    }
#endif
}

#if (RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON)
/**
 * @brief If the callback interface for the end of the test is configured, this interface is called
 */
/* PRQA S 1505 ++ */ /* VL_RamTst_1505 */
void RamTst_TestCompletedNotification(void)
{
    if (NULL_PTR != RamTst_GetConfigParams.TestCompletedNotificationPtr)
    {
        RamTst_GetConfigParams.TestCompletedNotificationPtr();
    }
}
#endif

#if (RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON)
/**
 * @brief If the error callback interface for the test is configured, this interface is called
 */
void RamTst_ErrorNotification(void)
{
    if (NULL_PTR != RamTst_GetConfigParams.ErrorNotificationPtr)
    {
        RamTst_GetConfigParams.ErrorNotificationPtr();
    }
}
/* PRQA S 1505 -- */
#endif

/* ========================================== internal function definitions ========================================= */
/**
 * @brief a) Read the value of each cell in ascending address order and check if it is 0, then write it to 1.
 *        b) Read the value of each cell in descending address order and check if it is 1.
 *        c) Read the value of each cell in ascending address order and check whether it is 1, and then write it to 0;
 *        d) Read the value of each cell in descending address order and check if it is 0.
 */
/* PRQA S 6080 ++ */ /* VL_MTR_RamTst_STPTH */
/* PRQA S 4461 ++ */ /* VL_RamTst_4461 */
RAMTST_LOCAL void RamTest_AbrahamAlgorithmStepOne(uint32* testRam, Std_ReturnType* testResult)
{
    uint32 testCell;

    /** Check whether the value is 0 in ascending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ALL_ZERO == RamTst_Verify(*testRam, testCell))
        {
            /** Write 1 if the data of the cell is right */
            *testRam = RamTst_SetOne(*testRam, testCell);
        }
        else
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 1 in descending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ONE != RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U)))
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 1 in ascending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ONE == RamTst_Verify(*testRam, testCell))
        {
            /** Write 0 if the data of the cell is right */
            *testRam = RamTst_SetZero(*testRam, testCell);
        }
        else
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 0 in descending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ALL_ZERO != RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U)))
        {
            *testResult = E_NOT_OK;
        }
    }
}

/**
 * @brief a) Read the value of each cell in descending address order and check whether it is 0, and then write it to 1;
 *        b) Read the value of each cell in ascending address order and check if it is 1.
 *        c) Read the value of each cell in descending address order and check whether it is 1, and then write it to 0;
 *        d) Read the value of each cell in ascending address order and check if it is 0.
 */
RAMTST_LOCAL void RamTest_AbrahamAlgorithmStepTwo(uint32* testRam, Std_ReturnType* testResult)
{
    uint32 testCell;

    /** Check whether the value is 0 in descending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ALL_ZERO == RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U)))
        {
            /** Write 1 if the data of the cell is right */
            *testRam = RamTst_SetOne(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U));
        }
        else
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 1 in ascending order */
    for (testCell = 0x01U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ONE != RamTst_Verify(*testRam, testCell))
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 1 in descending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ONE == RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U)))
        {
            /** Write 0 if the data of the cell is right */
            *testRam = RamTst_SetZero(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U));
        }
        else
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 0 in ascending order */
    for (testCell = 0x01U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ALL_ZERO != RamTst_Verify(*testRam, testCell))
        {
            *testResult = E_NOT_OK;
        }
    }
}

/**
 * @brief a) Read the value of each cell in ascending address order and check whether it is 0,
 * write it to 1 and then write it back to 0;
 *        b) Read the value of each cell in descending address order and check if it is 0.
 *        c) Read the value of each cell in descending address order and check whether it is 0,
 * write it to 1 and then write it to 0;
 *        d)Read the value of each cell in ascending address order and check if it is 0.
 */
RAMTST_LOCAL void RamTest_AbrahamAlgorithmStepThree(uint32* testRam, Std_ReturnType* testResult)
{
    uint32 testCell;

    /** Check whether the value is 0 in ascending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ALL_ZERO == RamTst_Verify(*testRam, testCell))
        {
            /** Write 1 if the data of the cell is right */
            *testRam = RamTst_SetOne(*testRam, testCell);
            /** Write 0 if the data of the cell is right */
            *testRam = RamTst_SetZero(*testRam, testCell);
        }
        else
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 0 in descending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ALL_ZERO != RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U)))
        {
            *testResult = E_NOT_OK;
        }
    }

    /** Check whether the value is 0 in descending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ALL_ZERO == RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U)))
        {
            /** Write 1 if the data of the cell is right */
            *testRam = RamTst_SetOne(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U));
            /** Write 0 if the data of the cell is right */
            *testRam = RamTst_SetZero(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U));
        }
        else
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 0 in ascending order */
    for (testCell = 0x01U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ALL_ZERO != RamTst_Verify(*testRam, testCell))
        {
            *testResult = E_NOT_OK;
        }
    }
}

/**
 * @brief a) Read the value of each cell in ascending address order and check whether it is 1, and then write it to 0;
 *        b) Read the value of each cell in descending address order and check if it is 1.
 *        c) Read the value of each cell in descending address order and check whether it is 1,
 * write it to 0 and then write it back to 1;
 *        d) Read the value of each cell in ascending address order and check if it is 1.
 */
RAMTST_LOCAL void RamTest_AbrahamAlgorithmStepFour(uint32* testRam, Std_ReturnType* testResult)
{
    uint32 testCell;

    /** Check whether the value is 1 in ascending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ONE == RamTst_Verify(*testRam, testCell))
        {
            /** Write 0 if the data of the cell is right */
            *testRam = RamTst_SetZero(*testRam, testCell);
            /** Write 1 if the data of the cell is right */
            *testRam = RamTst_SetOne(*testRam, testCell);
        }
        else
        {
            *testResult = E_NOT_OK;
        }
    }
    /** Check whether the value is 1 in descending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ONE != RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U)))
        {
            *testResult = E_NOT_OK;
        }
    }

    /** Check whether the value is 1 in descending order */
    for (testCell = 0x00U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ONE == RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U)))
        {
            /** Write 0 if the data of the cell is right */
            *testRam = RamTst_SetZero(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U));
            /** Write 1 if the data of the cell is right */
            *testRam = RamTst_SetOne(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 0x01U));
        }
        else
        {
            *testResult = E_NOT_OK;
        }
    }

    /** Check whether the value is 1 in ascending order */
    for (testCell = 0x01U; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == *testResult)); testCell++)
    {
        if (RAMTST_ONE != RamTst_Verify(*testRam, testCell))
        {
            *testResult = E_NOT_OK;
        }
    }
}
/* PRQA S 6080 -- */

/**
 * @brief a) Write zeros to the entire RAM cell.
 *        b) Read cells in ascending address order, read the value of each cell and check if it is 0,
 * then write 1 to the cell.
 *        c) Read cells in ascending address order, read the value of each cell and check if it is 1.
 */
/* PRQA S 0306,0490 ++ */ /* VL_RamTst_0306,VL_RamTst_0490 */
RAMTST_LOCAL Std_ReturnType RamTest_DoMarchAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy)
{
    Std_ReturnType testResult = E_OK;

    for (uint32* testRam = CurRamStartAddr; ((testRam <= CurRamEndAddr) && (E_OK == testResult)); testRam++)
    {
        SchM_Enter_RamTst_Queue();

        uint32 dataSaved = *testRam;
        uint8  testCell;

        /** Write the pattern 0x00000000 */
        *testRam = RAMTST_ALL_ZERO;

        for (testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == testResult)); testCell++)
        {
            if (RAMTST_ALL_ZERO != RamTst_Verify(*testRam, testCell))
            {
                testResult = E_NOT_OK;
            }
            else
            {
                *testRam = RamTst_SetOne(*testRam, testCell);
            }
        }

        for (testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == testResult)); testCell++)
        {
            if (RAMTST_ALL_ZERO == RamTst_Verify(*testRam, testCell))
            {
                testResult = E_NOT_OK;
            }
        }

        if (RAMTEST_DESTRUCTIVE == TestPolicy)
        {
            *testRam = FillPattern;
        }
        else
        {
            *testRam = dataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }
    return testResult;
}

/**
 * @brief a) Write 1 and 0 to one bit and the next bit in the RAM cell in descending order.
 *        b) Read cells in ascending address order, check if one bit is 1 and the next bit is 0 in RAM cell.
 *        c) Read cells in descending address order, check if the RAM cell is 0 and the next RAM cell is 1.
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoCheckerboardAlgorithmSub(uint32* testRam, Std_ReturnType testResult)
{
    uint32         testCell;
    Std_ReturnType retVal = testResult;

    /** Write checkerboard pattern 0x55555555u */
    for (testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == retVal)); testCell += 2u)
    {
        *testRam = RamTst_SetOne(*testRam, testCell);
        *testRam = RamTst_SetZero(*testRam, (testCell + 1u));
    }

    /** Check from the lower to the higher cell. */
    for (testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == retVal)); testCell += 2u)
    {
        if ((RAMTST_ALL_ZERO == RamTst_Verify(*testRam, testCell)) || (0u != RamTst_Verify(*testRam, (testCell + 1u))))
        {
            retVal = E_NOT_OK;
        }
    }

    /** Check from the higher to the lower cell. */
    for (testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == retVal)); testCell += 2u)
    {
        if ((RAMTST_ALL_ZERO == RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 2u)))
            || (0u != RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 1u))))
        {
            retVal = E_NOT_OK;
        }
    }
    return retVal;
}

/**
 * @brief a) Write 0 and 1 to one bit and the next bit in the RAM cell in ascending order.
 *        b) Read cells in ascending address order, check if one bit is 0 and the next bit is 1 in RAM cell.
 *        c) Read cells in descending address order, check if the RAM cell is 1 and the next RAM cell is 0.
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoCheckerboardAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy)
{
    Std_ReturnType testResult = E_OK;

    for (uint32* testRam = CurRamStartAddr; ((testRam <= CurRamEndAddr) && (E_OK == testResult)); testRam++)
    {
        SchM_Enter_RamTst_Queue();
        uint32 dataSaved = *testRam;

        /** Write checkerboard pattern 0xAAAAAAAAu */
        for (uint8 testCell = 0u; testCell < RamTst_GetMinNumberOfTestedCells; testCell += 2u)
        {
            *testRam = RamTst_SetZero(*testRam, testCell);
            *testRam = RamTst_SetOne(*testRam, (testCell + 1u));
        }

        /** Check the cells from the lower to the higher */
        for (uint8 testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == testResult));
             testCell += 2u)
        {
            if ((RAMTST_ALL_ZERO != RamTst_Verify(*testRam, testCell))
                || (0u == RamTst_Verify(*testRam, (testCell + 1u))))
            {
                testResult = E_NOT_OK;
            }
        }

        /** Check from the higher to the lower cell. */
        for (uint8 testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == testResult));
             testCell += 2u)
        {
            if ((RAMTST_ALL_ZERO != RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 2u)))
                || (0u == RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell - 1u))))
            {
                testResult = E_NOT_OK;
            }
        }

        testResult = RamTest_DoCheckerboardAlgorithmSub(testRam, testResult);
        if (RAMTEST_DESTRUCTIVE == TestPolicy)
        {
            *testRam = FillPattern;
        }
        else
        {
            *testRam = dataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }

    return testResult;
}

/**
 * @brief a) Write zeros to the entire RAM cell.
 *        b) Write 1 to one bit in RAM cell, check if the remaining bits are 0, and then write the bit back to 0.
 *        c) Write ones to the entire RAM cell in ascending address order.
 *        d) Write 0 to one bit in RAM cell, check if the remaining bits are 1, and then write the bit back to 1.
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoWalkPathAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy)
{
    Std_ReturnType testResult = E_OK;

    for (uint32* testRam = CurRamStartAddr; ((testRam <= CurRamEndAddr) && (E_OK == testResult)); testRam++)
    {
        SchM_Enter_RamTst_Queue();

        uint32 dataSaved = *testRam;
        uint8  testCell;

        /** Write the pattern 0x00000000 */
        *testRam = RAMTST_ALL_ZERO;

        for (testCell = 0u; testCell < RamTst_GetMinNumberOfTestedCells; testCell++)
        {
            *testRam = RamTst_SetOne(*testRam, testCell);

            /** Check the remaining memory area. */
            if (RAMTST_ALL_ZERO != RamTst_WalkPathVerify(*testRam, testCell))
            {
                testResult = E_NOT_OK;
                break;
            }

            *testRam = RamTst_SetZero(*testRam, testCell);
        }

        /** Write the pattern 0xFFFFFFFF */
        *testRam = RAMTST_ALL_ONE;

        for (testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == testResult)); testCell++)
        {
            *testRam = RamTst_SetZero(*testRam, testCell);

            /** Check the remaining memory area. */
            if (RAMTST_ALL_ONE != RamTst_WalkPathVerify(*testRam, testCell))
            {
                testResult = E_NOT_OK;
                break;
            }

            *testRam = RamTst_SetOne(*testRam, testCell);
        }

        if (RAMTEST_DESTRUCTIVE == TestPolicy)
        {
            *testRam = FillPattern;
        }
        else
        {
            *testRam = dataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }

    return testResult;
}

/**
 * @brief a) Write zeros to the entire RAM cell.
 *        b) The bit in the RAM cell is written to 1 in ascending address order.
 *        c) Check whether the remaining bits are all zeros or all ones, and check if the written bit is 1.
 *        d) The bit in the RAM cell is written to 0 in descending order.
 *        e) Check whether the remaining bits are all ones or all zeros, and check if the written bit is 0.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_RamTst_STCAL */
RAMTST_LOCAL Std_ReturnType RamTest_DoGalpatAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy)
{
    Std_ReturnType testResult = E_OK;

    for (uint32* testRam = CurRamStartAddr; ((testRam <= CurRamEndAddr) && (E_OK == testResult)); testRam++)
    {
        SchM_Enter_RamTst_Queue();

        uint32 dataSaved = *testRam;

        /** Write the pattern 0x00000000 */
        *testRam = RAMTST_ALL_ZERO;
        uint32 testCell;

        for (testCell = 1u; ((testCell <= RamTst_GetMinNumberOfTestedCells) && (E_OK == testResult)); testCell++)
        {
            /** For example. Write the test cell as follow sequence: 1. 0000; 2. 0001; 3. 0011; ... */
            *testRam = RamTst_GalpatSetPatternOne(*testRam, testCell);

            /** Check the remaining cells. */
            if ((RAMTST_ALL_ZERO != RamTst_GalpatCheckZero(*testRam, testCell))
                || (RAMTST_ALL_ONE != RamTst_GalpatCheckOnes(*testRam, testCell)))
            {
                testResult = E_NOT_OK;
                break;
            }

            /** Check the inverted Cell. */
            if (RAMTST_ALL_ZERO == (RamTst_Verify(*testRam, (testCell - 1u))))
            {
                testResult = E_NOT_OK;
            }
        }

        for (testCell = 1u; ((testCell <= RamTst_GetMinNumberOfTestedCells) && (E_OK == testResult)); testCell++)
        {
            /** For example. Write the test cell as follow sequence: 1. 1111; 2. 0111; 3. 0011 ... */
            *testRam = RamTst_GalpatSetPatternZero(*testRam, testCell);

            /** Check the remaining cells. */
            if ((RAMTST_ALL_ONE != RamTst_GalpatCheckAllOne(*testRam, testCell))
                || (RAMTST_ALL_ZERO != RamTst_GalpatCheckAllZero(*testRam, testCell)))
            {
                testResult = E_NOT_OK;
                break;
            }

            /** Check the inverted Cell. */
            if (RAMTST_ALL_ZERO != (RamTst_Verify(*testRam, (uint8)(RamTst_GetMinNumberOfTestedCells - testCell))))
            {
                testResult = E_NOT_OK;
            }
        }

        if (RAMTEST_DESTRUCTIVE == TestPolicy)
        {
            *testRam = FillPattern;
        }
        else
        {
            *testRam = dataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }

    return testResult;
}
/* PRQA S 6070 -- */
/* PRQA S 4461 -- */
/**
 * @brief a) Calculates the first signature value of the current RAM cell.
 *        b) Invert the value of the RAM cell.
 *        c) Calculates the second signature value of the current RAM cell.
 *        d) Compare the two signatures.
 *        e) Invert the value of the RAM cell again.
 *        f) Calculates the third signature value of the current RAM cell.
 *        g) Check whether the first signature value is equal to the third signature value.
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoTransGalpatAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy)
{
    Std_ReturnType testResult = E_OK;

    for (uint32* testRam = CurRamStartAddr; ((testRam <= CurRamEndAddr) && (E_OK == testResult)); testRam++)
    {
        SchM_Enter_RamTst_Queue();

        uint32 dataSaved = *testRam;
        uint32 testCell;

        for (testCell = 0u; ((testCell < RamTst_GetMinNumberOfTestedCells) && (E_OK == testResult)); testCell++)
        {
            uint32 signatureStepOne =
                (uint32)(((*testRam) ^ RAMTST_TRANSGALPAT_SIGNATURE) & ~(uint32)(1uL << testCell));
            *testRam ^= (uint32)(1uL << testCell);

            uint32 signatureStepTwo =
                (uint32)(((*testRam) ^ RAMTST_TRANSGALPAT_SIGNATURE) & ~(uint32)(1uL << testCell));
            if (signatureStepOne != signatureStepTwo)
            {
                testResult = E_NOT_OK;
                break;
            }
            *testRam ^= (uint32)(1uL << testCell);

            uint32 signatureStepThree =
                (uint32)(((*testRam) ^ RAMTST_TRANSGALPAT_SIGNATURE) & ~(uint32)(1uL << testCell));
            if (signatureStepOne != signatureStepThree)
            {
                testResult = E_NOT_OK;
            }
        }

        if (RAMTEST_DESTRUCTIVE == TestPolicy)
        {
            *testRam = FillPattern;
        }
        else
        {
            *testRam = dataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }

    return testResult;
}

/**
 * @brief Executing Abraham algorithm
 */
RAMTST_LOCAL Std_ReturnType RamTest_DoAbrahamAlgorithm(
    uint32*             CurRamStartAddr,
    const uint32*       CurRamEndAddr,
    const uint32        FillPattern,
    const RamTst_Policy TestPolicy)
{
    Std_ReturnType testResult = E_OK;

    for (uint32* testRam = CurRamStartAddr; ((testRam <= CurRamEndAddr) && (E_OK == testResult)); testRam++)
    {
        SchM_Enter_RamTst_Queue();

        uint32 dataSaved = *testRam;

        /** Write the pattern 0x00000000 */
        *testRam = RAMTST_ALL_ZERO;

        RamTest_AbrahamAlgorithmStepOne(testRam, &testResult);
        RamTest_AbrahamAlgorithmStepTwo(testRam, &testResult);
        RamTest_AbrahamAlgorithmStepThree(testRam, &testResult);

        /** Write the pattern 0xFFFFFFFF */
        *testRam = RAMTST_ALL_ONE;

        RamTest_AbrahamAlgorithmStepFour(testRam, &testResult);

        if (RAMTEST_DESTRUCTIVE == TestPolicy)
        {
            *testRam = FillPattern;
        }
        else
        {
            *testRam = dataSaved;
        }
        SchM_Exit_RamTst_Queue();
    }
    return testResult;
}

/**
 * @brief The algorithm interface is called based on the passed algorithm value
 */
RAMTST_LOCAL Std_ReturnType RamTst_ExecTest(
    const RamTst_AlgorithmType UsedAlgorithm,
    uint32*                    CurRamStartAddr,
    const uint32*              CurRamEndAddr,
    const uint32               FillPattern,
    const RamTst_Policy        TestPolicy)
{
    Std_ReturnType retVal = E_NOT_OK;

    /* PRQA S 3223 ++ */ /* VL_RamTst_3223 */
    RAMTST_LOCAL const RamTst_JobFuncPtr RamTst_JobFuncMatrix[] = {
        NULL_PTR,
#if (RAMTST_CHECKERBOARD_TEST_SELECTED_ENABLE == STD_ON)
        &RamTest_DoCheckerboardAlgorithm, /** Checkerboard */
#endif
#if (RAMTST_MARCH_TEST_SELECTED_ENABLE == STD_ON)
        &RamTest_DoMarchAlgorithm, /** March */
#endif
#if (RAMTST_WALKPATH_TEST_SELECTED_ENABLE == STD_ON)
        &RamTest_DoWalkPathAlgorithm, /** WalkPath */
#endif
#if (RAMTST_GALPAT_TEST_SELECTED_ENABLE == STD_ON)
        &RamTest_DoGalpatAlgorithm, /** Galpat */
#endif
#if (RAMTST_TRANSGALPAT_TEST_SELECTED_ENABLE == STD_ON)
        &RamTest_DoTransGalpatAlgorithm, /** TransGalpat */
#endif
#if (RAMTST_ABRAHAM_TEST_SELECTED_ENABLE == STD_ON)
        &RamTest_DoAbrahamAlgorithm, /** Abraham */
#endif
    };
    /* PRQA S 3223 -- */

    if (UsedAlgorithm != RAMTST_ALGORITHM_UNDEFINED)
    {
        retVal = (*RamTst_JobFuncMatrix[UsedAlgorithm])(CurRamStartAddr, CurRamEndAddr, FillPattern, TestPolicy);
    }
    return retVal;
}

#if (STD_ON == RAMTST_SELF_CHECK_ENABLE)
/**
 * @brief Writes the value to specified address and then reads out to check whether it is the same as the written value
 */
RAMTST_LOCAL Std_ReturnType RamTst_DoSelfCheckTestAction(uint32* u32CStartAddr, const uint32* u32CEndAddr)
{
    const uint32   writeData = 0xA5A5A5A5u;
    Std_ReturnType retVal    = E_OK;

    /** Verify data with addresses increasing */
    for (uint32* pRamPtr = u32CStartAddr; ((pRamPtr < u32CEndAddr) && (E_OK == retVal)); (pRamPtr)++)
    {
        uint32 storeData = *pRamPtr;
        *pRamPtr         = writeData;
        if (*pRamPtr != writeData)
        {
            retVal = E_NOT_OK;
        }
        else
        {
            *pRamPtr = storeData;
        }
    }
    return retVal;
}
#endif
/* PRQA S 0306,0490 -- */

/**
 * @brief Reset the status, result, address and other related information of all blocks
 */
RAMTST_LOCAL void RamTst_InitAllBlockStatus(void)
{
    uint8 aloParamSets = RamTst_GetNumberOfAlgParamSets;
    for (uint32 algParamindex = 0u; algParamindex < aloParamSets; algParamindex++)
    {
        const RamTst_AlgParams*   algParamsPtr = &RamTst_GetConfigAlgParams(algParamindex);
        RamTst_NumberOfBlocksType blockNumber  = algParamsPtr->NumberOfBlocks;
        for (uint32 blockIndex = 0u; blockIndex < blockNumber; blockIndex++)
        {
            /** use block id as the buffer index */
            uint32                    blockId       = algParamsPtr->BlockParams[blockIndex].BlockId;
            const RamTst_BlockParams* blockParamPtr = &algParamsPtr->BlockParams[blockIndex];
            RamTst_BlockInfo[blockId].NowTestedAddr = blockParamPtr->StartAddress;
            /** End address is startaddress, which means the block is not tested */
            RamTst_BlockInfo[blockId].FinalEndAddr   = blockParamPtr->StartAddress;
            RamTst_BlockInfo[blockId].TestPolicyUse  = blockParamPtr->TestPolicy;
            RamTst_BlockInfo[blockId].AlgParamsIdUse = algParamsPtr->AlgParamsId;
        }
    }
}

#if (                                                            \
    ((STD_ON == RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE)     \
     || (STD_ON == RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE)) \
    && (STD_ON == RAMTST_DEV_ERROR_DETECT))
/**
 * @brief Find out if any of the configured blockid matches the passed blockid
 */
RAMTST_LOCAL Std_ReturnType RamTst_CheckBlockIDIsOutOfRange(RamTst_NumberOfBlocksType BlockID)
{
    boolean                findBlock          = FALSE;
    Std_ReturnType         retVal             = E_NOT_OK;
    uint8                  algParamSetsNumber = RamTst_GetNumberOfAlgParamSets;
    RamTst_AlgParamsIdType algParamsId        = RamTst_CurAlgParamsId - 1u;
    for (uint32 algParamindex = 0u; (algParamindex < algParamSetsNumber) && (findBlock == FALSE); algParamindex++)
    {
        const RamTst_AlgParams* algParamsPtr = &RamTst_GetConfigAlgParams(algParamindex);
        for (uint32 blockIndex = 0u; blockIndex < algParamsPtr->NumberOfBlocks; blockIndex++)
        {
            if ((BlockID == algParamsPtr->BlockParams[blockIndex].BlockId) && (algParamindex == algParamsId))
            {
                retVal    = E_OK;
                findBlock = TRUE;
                break;
            }
        }
    }
    return retVal;
}
#endif

#if (STD_ON == RAMTST_RUN_FULL_TEST_API_ENABLE)
/**
 * @brief Test all blocks configured as the foreground test, and record the test results.
 */
RAMTST_LOCAL void RamTst_RunFullTestSub(void)
{
    RamTst_OverallResult                       = RAMTST_RESULT_UNDEFINED;
    RamTst_AlgParamsIdType  algParamsId        = RamTst_CurAlgParamsId - 1u;
    const RamTst_AlgParams* algotirhmParamsPtr = &RamTst_GetConfigAlgParams(algParamsId);
    /** just test RamTst_CurAlgParamsId indicate blocks */
    for (uint32 testIndex = 0u; testIndex < algotirhmParamsPtr->NumberOfBlocks; (testIndex)++)
    {
        const RamTst_BlockParams* blockParamsPtr = &algotirhmParamsPtr->BlockParams[testIndex];
        /** Only matched algorithms can be tested  */
        RamTst_BlockInfo[blockParamsPtr->BlockId].BlockTestResult = RAMTST_RESULT_UNDEFINED;
#if (RAMTST_TEST_TIMEOUT == STD_ON)
        uint32 timeStartFgnd = Frt_ReadOutMS();
#endif
        Std_ReturnType fulltestRet = RamTst_ExecTest(
            algotirhmParamsPtr->Algorithm,
            blockParamsPtr->StartAddress,
            blockParamsPtr->EndAddress,
            blockParamsPtr->FillPattern,
            blockParamsPtr->TestPolicy);
#if (RAMTST_TEST_TIMEOUT == STD_ON)
        uint32 timeGapFgnd = Frt_CalculateElapsedMS(timeStartFgnd);
        if (timeGapFgnd >= RAMTST_TIMEOUTVALUE_FGND)
        {
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
            RamTst_Det_ReportError((uint8)RAMTST_RUN_FULL_TEST_ID, (RAMTST_E_TIMEOUT));
#endif
            RamTst_BlockInfo[blockParamsPtr->BlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
            RamTst_OverallResult                                      = RAMTST_RESULT_NOT_OK;
        }
#endif
        /** check test result */
        if (E_OK == fulltestRet)
        {
            RamTst_BlockInfo[blockParamsPtr->BlockId].BlockTestResult = RAMTST_RESULT_OK;
            if (RamTst_OverallResult != RAMTST_RESULT_NOT_OK)
            {
                RamTst_OverallResult = RAMTST_RESULT_OK;
            }
        }
        else
        {
            RamTst_BlockInfo[blockParamsPtr->BlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
            RamTst_OverallResult                                      = RAMTST_RESULT_NOT_OK;
#if (STD_ON == RAMTST_DEM_RUNFL_RAM_FAILURE_ENABLE)
            /** report the production error RAMTST_RUNFL_RAM_FAILURE as fail */
            Dem_SetEventStatus(RAMTST_RUNFL_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif
        }
    }

#if (STD_ON == RAMTST_DEM_RUNFL_RAM_FAILURE_ENABLE)
    if (RamTst_OverallResult == RAMTST_RESULT_OK)
    {
        /** report the production error RAMTST_RUNFL_RAM_FAILURE as fail */
        Dem_SetEventStatus(RAMTST_RUNFL_RAM_FAILURE, DEM_EVENT_STATUS_PASSED);
    }
#endif
}
#endif

#if (STD_ON == RAMTST_RUN_PARTIAL_TEST_API_ENABLE)
/**
 * @brief Stores the value of the current state, then sets the state to running, executes the test based on the blockId
 * passed in, logs the test results, and restores the value of the current state when the test is over
 */
RAMTST_LOCAL void RamTst_RunPartialTestSub(RamTst_NumberOfBlocksType BlockId)
{
    RamTst_AlgParamsIdType    algParamsId        = RamTst_CurAlgParamsId - 1u;
    const RamTst_AlgParams*   algotirhmParamsPtr = &RamTst_GetConfigAlgParams(algParamsId);
    const RamTst_BlockParams* blockParamsPtr     = NULL_PTR;

    for (RamTst_NumberOfBlocksType iloop = 0u; iloop < algotirhmParamsPtr->NumberOfBlocks; iloop++)
    {
        if (BlockId == algotirhmParamsPtr->BlockParams[iloop].BlockId)
        {
            blockParamsPtr = &algotirhmParamsPtr->BlockParams[iloop];
            break;
        }
    }
    if (blockParamsPtr != NULL_PTR)
    {
        /** Only matched algorithms can be tested  */
        RamTst_BlockInfo[blockParamsPtr->BlockId].BlockTestResult = RAMTST_RESULT_UNDEFINED;
        /** just test RamTst_CurAlgParamsId indicate blocks */
#if (RAMTST_TEST_TIMEOUT == STD_ON)
        uint32 timeStartFgnd = Frt_ReadOutMS();
#endif
        Std_ReturnType PartialTestRet = RamTst_ExecTest(
            algotirhmParamsPtr->Algorithm,
            blockParamsPtr->StartAddress,
            blockParamsPtr->EndAddress,
            blockParamsPtr->FillPattern,
            blockParamsPtr->TestPolicy);

        if (E_OK == PartialTestRet)
        {
            RamTst_BlockInfo[blockParamsPtr->BlockId].BlockTestResult = RAMTST_RESULT_OK;
#if (STD_ON == RAMTST_DEM_PART_RAM_FAILURE_ENABLE)
            /** report the production error RAMTST_PART_RAM_FAILURE as fail */
            Dem_SetEventStatus(RAMTST_PART_RAM_FAILURE, DEM_EVENT_STATUS_PASSED);
#endif
        }
        else
        {
            RamTst_BlockInfo[blockParamsPtr->BlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
            RamTst_OverallResult                                      = RAMTST_RESULT_NOT_OK;
#if (STD_ON == RAMTST_DEM_PART_RAM_FAILURE_ENABLE)
            /** report the production error RAMTST_PART_RAM_FAILURE as fail */
            Dem_SetEventStatus(RAMTST_PART_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif
        }

#if (RAMTST_TEST_TIMEOUT == STD_ON)
        uint32 timeGapFgnd = Frt_CalculateElapsedMS(timeStartFgnd);
        if (timeGapFgnd >= RAMTST_TIMEOUTVALUE_FGND)
        {
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
            RamTst_Det_ReportError((uint8)RAMTST_RUN_PARTIAL_TEST_ID, (RAMTST_E_TIMEOUT));
#endif
            RamTst_BlockInfo[blockParamsPtr->BlockId].BlockTestResult = RAMTST_RESULT_NOT_OK;
        }
#endif
    }
}
#endif

/**
 * @brief Run the test according to the current blockid, start address, end address, then record the test result
 */
RAMTST_LOCAL boolean RamTst_MainFunctionSub(void)
{
    RamTst_AlgParamsIdType    algParamsId        = RamTst_CurAlgParamsId - 1u;
    const RamTst_AlgParams*   algotirhmParamsPtr = &RamTst_GetConfigAlgParams(algParamsId);
    const RamTst_BlockParams* blockParamsPtr     = &algotirhmParamsPtr->BlockParams[(uint16)(RamTst_CurTestedBlock)];
    /** calc now test final addr */ /* PRQA S 0488,0489,0490 ++ */ /* VL_RamTst_0488,VL_RamTst_0489,VL_RamTst_0490 */
    if ((blockParamsPtr->EndAddress)
        < (RamTst_BlockInfo[RamTst_CurBlockIndex].FinalEndAddr + RamTst_NumberOfTestedCell - 1u))
    /* PRQA S 0488,0489,0490 -- */
    {
        RamTst_BlockInfo[RamTst_CurBlockIndex].FinalEndAddr = blockParamsPtr->EndAddress;
    }
    else
    {
        /* PRQA S 0488 ++ */ /* VL_RamTst_0488 */
        RamTst_BlockInfo[RamTst_CurBlockIndex].FinalEndAddr += (RamTst_NumberOfTestedCell - 1u);
        /* PRQA S 0488 -- */
    }
    /** exec test */
    RamTst_BlockInfo[RamTst_CurBlockIndex].BlockTestResult = RAMTST_RESULT_UNDEFINED;

#if (RAMTST_TEST_TIMEOUT == STD_ON)
    uint32 timeStartBgnd = Frt_ReadOutMS();
#endif
    Std_ReturnType testResult = RamTst_ExecTest(
        algotirhmParamsPtr->Algorithm,
        RamTst_BlockInfo[RamTst_CurBlockIndex].NowTestedAddr,
        RamTst_BlockInfo[RamTst_CurBlockIndex].FinalEndAddr,
        blockParamsPtr->FillPattern,
        RamTst_BlockInfo[RamTst_CurBlockIndex].TestPolicyUse);

#if (RAMTST_TEST_TIMEOUT == STD_ON)
    uint32 timeGapBgnd = Frt_CalculateElapsedMS(timeStartBgnd);
    if (timeGapBgnd >= RAMTST_TIMEOUTVALUE_BGND)
    {
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
        RamTst_Det_ReportError((uint8)RAMTST_GET_MAIN_FUNCTION_ID, (RAMTST_E_TIMEOUT));
#endif
        testResult = E_NOT_OK;
    }
#endif

    boolean allTestdone = RamTst_UpdateResult(testResult, algotirhmParamsPtr, blockParamsPtr);
    return allTestdone;
}

/**
 * @brief If all blocks are tested, restart the test from the first block
 */
RAMTST_LOCAL boolean RamTst_UpdateResult(
    Std_ReturnType            testResult,
    const RamTst_AlgParams*   algotirhmParamsPtr,
    const RamTst_BlockParams* blockParamsPtr)
{
    boolean nextBlockStart = FALSE;
    boolean allTestdone    = FALSE;

    if (E_NOT_OK == testResult)
    {
        RamTst_BlockInfo[RamTst_CurBlockIndex].BlockTestResult = RAMTST_RESULT_NOT_OK;
        RamTst_OverallResult                                   = RAMTST_RESULT_NOT_OK;
        RamTst_CurTestedBlock++;
        /** has error, this block not test continue */
        if ((uint16)(RamTst_CurTestedBlock + 1u) <= algotirhmParamsPtr->NumberOfBlocks)
        {
            RamTst_CurBlockIndex++;
        }
        else
        {
            RamTst_AlgParams algParam = RamTst_GetConfigAlgParams(RamTst_CurAlgParamsId - 1u);
            RamTst_CurBlockIndex      = algParam.BlockParams[0].BlockId;
            allTestdone               = TRUE;
        }
        nextBlockStart = TRUE;
        /** report the production error RAMTST_MAIN_RAM_FAILURE as fail */
#if (STD_ON == RAMTST_DEM_MAIN_RAM_FAILURE_ENABLE)
        Dem_SetEventStatus(RAMTST_MAIN_RAM_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif
#if (RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON)
        RamTst_ErrorNotification();
#endif
    }
    /** check if current block test is done */
    if ((RAMTST_RESULT_NOT_OK != RamTst_BlockInfo[RamTst_CurBlockIndex].BlockTestResult) && (nextBlockStart == FALSE))
    {
        /* PRQA S 0490 ++ */ /* VL_RamTst_0490 */
        if (RamTst_BlockInfo[RamTst_CurBlockIndex].FinalEndAddr >= blockParamsPtr->EndAddress)
        /* PRQA S 0490 -- */
        {
            /** now block has test over */
            RamTst_CurTestedBlock++;
            nextBlockStart                                         = TRUE;
            RamTst_OverallResult                                   = RAMTST_RESULT_OK;
            RamTst_BlockInfo[RamTst_CurBlockIndex].BlockTestResult = RAMTST_RESULT_OK;
            if ((uint16)(RamTst_CurTestedBlock + 1u) <= algotirhmParamsPtr->NumberOfBlocks)
            {
                RamTst_CurBlockIndex++;
            }
            else
            {
                RamTst_AlgParams algParam = RamTst_GetConfigAlgParams(RamTst_CurAlgParamsId - 1u);
                RamTst_CurBlockIndex      = algParam.BlockParams[0].BlockId;
                allTestdone               = TRUE;
            }
        }
        else if (RAMTST_RESULT_OK != RamTst_BlockInfo[RamTst_CurBlockIndex].BlockTestResult)
        {
            /** the block is still running test*/
            RamTst_OverallResult = RAMTST_RESULT_UNDEFINED;
        }
        else
        {
            /** do nothing */
        }
    }

    if (TRUE == nextBlockStart)
    {
        /** calc next test start addr */ /* PRQA S 0488,0489 ++ */ /* VL_RamTst_0488,VL_RamTst_0489 */
        RamTst_BlockInfo[RamTst_CurBlockIndex].NowTestedAddr += RamTst_NumberOfTestedCell;
        RamTst_BlockInfo[RamTst_CurBlockIndex].FinalEndAddr = RamTst_BlockInfo[RamTst_CurBlockIndex].FinalEndAddr + 1u;
        /* PRQA S 0488,0489 -- */
    }
    return allTestdone;
}

/**
 * @brief Check whether the current test block meets the conditions of the background test
 */
RAMTST_LOCAL boolean RamTst_CheckBgndCondIsValid(void)
{
    boolean                 retVal             = FALSE;
    RamTst_AlgParamsIdType  algParamsId        = RamTst_CurAlgParamsId - 1u;
    const RamTst_AlgParams* algotirhmParamsPtr = &RamTst_GetConfigAlgParams(algParamsId);

    /** Only matched algorithms can be tested  */
    if ((((uint16)(RamTst_CurTestedBlock + 1u) <= algotirhmParamsPtr->NumberOfBlocks))
        && (RamTst_CurAlgParamsId == RamTst_BlockInfo[RamTst_CurBlockIndex].AlgParamsIdUse))
    {
        /** back test must be none destuctive and need save and recover */
        if (RAMTEST_DESTRUCTIVE == RamTst_BlockInfo[RamTst_CurBlockIndex].TestPolicyUse)
        {
            retVal = FALSE;
#if (STD_ON == RAMTST_DEV_ERROR_DETECT)
            RamTst_Det_ReportError((uint8)RAMTST_GET_MAIN_FUNCTION_ID, (RAMTST_E_OUT_OF_RANGE));
#endif
        }
        else
        {
            retVal = TRUE;
        }
    }

    return retVal;
}

/**
 * @brief Check algorithm parameters id
 */
RAMTST_LOCAL boolean RamTst_CheckAlgParamsId(RamTst_AlgParamsIdType NewAlgParamsId, boolean* foundAlgParams)
{
#if (1u < RAMTST_ALGPARAMS_NUM)
    for (uint32 Index = 0u; Index < RAMTST_ALGPARAMS_NUM; Index++)
#else
    uint32 Index = 0;
#endif
    {
        if (NewAlgParamsId == RamTst_GetConfigAlgParams(Index).AlgParamsId)
        {
            *foundAlgParams = TRUE;
#if (1u < RAMTST_ALGPARAMS_NUM)
            break;
#endif
        }
    }
    return *foundAlgParams;
}

/* PRQA S 1252,1258,1503,1532,3120 -- */
#define RAMTST_STOP_SEC_CODE
#include "RamTst_MemMap.h"
