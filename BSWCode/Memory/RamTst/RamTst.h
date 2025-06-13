/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*********************************************************************************
**                                                                            **
**  FILENAME    : RamTst.h                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : peng.wu                                                     **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

/*======================[R E V I S I O N   H I S T O R Y]=====================*/
/*  <VERSION>    <DATE>    <AUTHOR>   <REVISION LOG>
 *  V1.0.0     20210914     Xin.Liu   Initial version
 *  V2.0.0     20221221     Peng.Wu   Modified to version R19-11
 *  V2.0.1     20240314     Peng.Wu   1.Update Schm and some global variables
 *             20240326     Peng.Wu   2.Fix header file including,CPT-8545
 *             20240415     Peng.Wu   CPT-8649, Fix Deinit, Add QAC
 *             20240515     peng.wu   1.CPT-9007, Update MemMap file
 *                                    2.CPT-9006, Fix mapping type when defined variables
 *                                    3.Optimize test algorithm code, add QAC
 *             20240603     peng.wu   Update multiple subfunction for Abraham
 */
/*============================================================================*/
#ifndef RAMTST_H_
#define RAMTST_H_

/******************************INCLUDE*********************************/
#include "RamTst_Types.h"

/*********************************Version Definition*************************************/
#define RAMTST_VENDOR_ID   62u /* Vendor ID*/
#define RAMTST_INSTANCE_ID 0u  /* Instance ID*/
#define RAMTST_MODULE_ID   93u /* MODULE ID*/

/******************************API ID Definition *********************************/
#define RAMTST_INIT_ID                          (0x00u) /* RamTst_Init*/
#define RAMTST_DEINIT_ID                        (0x0Cu) /* RamTst_DeInit*/
#define RAMTST_STOP_ID                          (0x02u) /* RamTst_Stop*/
#define RAMTST_ALLOW_ID                         (0x03u) /* RamTst_Allow*/
#define RAMTST_SUSPEND_ID                       (0x0Du) /* RamTst_Suspend()*/
#define RAMTST_RESUME_ID                        (0x0Eu) /* RamTst_Suspend()*/
#define RAMTST_GET_EXECUTION_STATUS_ID          (0x04u) /* RamTst_GetExecutionStatus */
#define RAMTST_GET_TEST_RESULT_ID               (0x05u) /* RamTst_GetTestResult */
#define RAMTST_GET_TEST_RESULT_PER_BLOCK_ID     (0x06u) /* RamTst_GetTestResultPerBlock */
#define RAMTST_GET_VERSION_INFO_ID              (0x0Au) /* RamTst_GetVersionInfo */
#define RAMTST_GET_ALG_PARAMS_ID                (0x12u) /* RamTst_GetAlgParams */
#define RAMTST_GET_TEST_ALGORITHM_ID            (0x07u) /* RamTst_GetTestAlgorithm */
#define RAMTST_GET_NUMBER_OF_TESTED_CELLS_ID    (0x09u) /* RamTst_GetNumberOfTestedCells */
#define RAMTST_SELECT_ALGPARAMS_ID              (0x0Bu) /* RamTst_SelectAlgParams */
#define RAMTST_CHANGE_NUMBER_OF_TESTED_CELLS_ID (0x08u) /* RamTst_ChangeNumberOfTestedCells */
#define RAMTST_RUN_FULL_TEST_ID                 (0x10u) /* RamTst_RunFullTest */
#define RAMTST_RUN_PARTIAL_TEST_ID              (0x11u) /* RamTst_RunPartialTest */
#define RAMTST_GET_MAIN_FUNCTION_ID             (0x01u) /* RamTst_MainFunction */

/******************************Det Error Definition*********************************/
#define RAMTST_E_STATUS_FAILURE (0x01u) /* A particular API is called in an unexpected state */
#define RAMTST_E_OUT_OF_RANGE   (0x02u) /* API parameter out of specified range */
#define RAMTST_E_UNINIT         (0x03u) /* API service used without module initialization */
#define RAMTST_E_PARAM_POINTER  (0x04u) /* API service called with a NULL pointer */
#define RAMTST_E_SELF_CHECK_ERR (0x05u) /* RAMTst Self Check Err */
#define RAMTST_E_TIMEOUT        (0x06u) /* RAMTst timeout */

/****************************************RamTst define*********************************/
#define RAMTST_ALL_ZERO                            ((uint32)0x00000000uL)
#define RAMTST_ALL_ONE                             ((uint32)0xFFFFFFFFuL)
#define RAMTST_MIN_NUMBER_OF_TESTED_CELLS          (RamTstConfig->RamTstConfigParams.RamTstMinNumberOfTestedCells)
#define RAMTST_ONE                                 (0x00000001uL)
#define RAMTST_SET_ONE(AddValue, Index)            (uint32)((RAMTST_ONE << (Index)) | (AddValue))
#define RAMTST_SET_ZERO(AddValue, Index)           (uint32)((~(RAMTST_ONE << (Index))) & (AddValue))
#define RAMTST_VERIFY_WALKPATH(AddValue, Index)    (uint32)((RAMTST_ONE << (Index)) ^ (AddValue))
#define RAMTST_SIGNATURE_TransGalpat               (0x55555555uL)
#define RAMTST_VERIFY(AddValue, Index)             (uint32)(((RAMTST_ONE << (Index)) & (AddValue)) >> (Index))
#define RAMTST_GALPAT_ONE_PATTERN(AddValue, Index) (uint32)((RAMTST_ONE << ((Index)-1u)) | (AddValue))
#define RAMTST_GALPAT_ZERO_PATTERN(AddValue, Index) \
    (uint32)(~(RAMTST_ONE << (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - (Index))) & (AddValue))
#define RAMTST_GALPAT_CHECK_ONES(AddValue, Index) (uint32)((RAMTST_ALL_ONE << (Index)) | (AddValue))
#define RAMTST_GALPAT_CHECK_ZERO(AddValue, Index) \
    (uint32)((RAMTST_ALL_ONE >> (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - (Index))) ^ (AddValue))
#define RAMTST_GALPAT_CHECK_ALL_ZERO(AddValue, Index) \
    (uint32)(~(RAMTST_ALL_ONE << (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - (Index))) ^ (AddValue))
#define RAMTST_GALPAT_CHECK_ALL_ONE(AddValue, Index) \
    (uint32)((RAMTST_ALL_ONE << (RAMTST_MIN_NUMBER_OF_TESTED_CELLS - (Index))) | (AddValue))
/***************************extern variable********************************************/
extern const RamTst_ConfigType RamTstConfigRoot[1];

/**********************************extern api*************************************/
/*************************************************************************/
/* RamTst
 * Brief               Service for RAM Test initialization
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr:Pointer to the selected configuration set.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_Init(P2CONST(RamTst_ConfigType, AUTOMATIC, RAMTST_APPL_CONST) ConfigPtr);

/*************************************************************************/
/* RamTst
 * Brief               Service for RAM Test deinitialization
 * ServiceId           0x0c
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_DeInit(void);

#if RAMTST_STOP_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for stopping the RAM Test
 * ServiceId           0x02
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_Stop(void);
#endif /* RAMTST_STOP_API_ENABLE */

#if RAMTST_ALLOW_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for continuing the RAM Test after calling RamTst_Stop.
 * ServiceId           0x03
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_Allow(void);
#endif /* RAMTST_ALLOW_API_ENABLE */

#if RAMTST_SUSPEND_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for suspending current operation of background RAM Test, until RESUME is called
 * ServiceId           0x0D
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_Suspend(void);
#endif /* RAMTST_SUSPEND_API_ENABLE */

#if RAMTST_RESUME_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for allowing to continue the background RAM Test at the point is was suspended.
 * ServiceId           0x0E
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_Resume(void);
#endif /* RAMTST_RESUME_API_ENABLE */

#if RAMTST_GET_EXECUTION_STATUS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current RAM Test execution status.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_ExecutionStatusTyp
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(RamTst_ExecutionStatusType, RAMTST_CODE)
RamTst_GetExecutionStatus(void);
#endif /* RAMTST_GET_EXECUTION_STATUS_API_ENABLE */

#if RAMTST_GET_TEST_RESULT_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current RAM Test result.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_TestResultType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(RamTst_TestResultType, RAMTST_CODE)
RamTst_GetTestResult(void);
#endif /* RAMTST_GET_TEST_RESULT_API_ENABLE */

#if RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current RAM Test result for the specified block
 * ServiceId           0x06
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockID:Identifies the block
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_TestResultType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(RamTst_TestResultType, RAMTST_CODE)
RamTst_GetTestResultPerBlock(RamTst_NumberOfBlocksType BlockID);
#endif /* RAMTST_GET_TEST_RESULT_PER_BLOCK_API_ENABLE */

#if RAMTST_VERSIONINFO_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the version information of this module
 * ServiceId           0x0A
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  The location address where to store the version information of this module.
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, RAMTST_APPL_CONST) VersionInfo);
#endif /* RAMTST_VERSIONINFO_API_ENABLE */

#if RAMTST_GET_ALGPARAMS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the ID of the current RAM Test algorithm parameter set.
 * ServiceId           0x12
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_AlgParamsIdType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(RamTst_AlgParamsIdType, RAMTST_CODE)
RamTst_GetAlgParams(void);
#endif /* RAMTST_GET_ALGPARAMS_API_ENABLE */

#if RAMTST_GET_TEST_ALGORITHM_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current RAM Test algorithm.
 * ServiceId           0x07
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_AlgorithmType
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(RamTst_AlgorithmType, RAMTST_CODE)
RamTst_GetTestAlgorithm(void);
#endif /* RAMTST_GET_TEST_ALGORITHM_API_ENABLE */

#if RAMTST_GET_NUMBER_OF_TESTED_CELLS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service returns the current number of tested cells per main-function cycle.
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              RamTst_NumberOfTestedCellsType:Number of currently tested cells per cycle
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(RamTst_NumberOfTestedCellsType, RAMTST_CODE)
RamTst_GetNumberOfTestedCells(void);
#endif /* RAMTST_GET_NUMBER_OF_TESTED_CELLS_API_ENABLE */

#if RAMTST_SELECT_ALGPARAMS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service used to set the test algorithm and its parameter set.
 * ServiceId           0x0B
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      NewAlgParamsId:Identifies the parameter set to be used.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_SelectAlgParams(RamTst_AlgParamsIdType NewAlgParamsId);
#endif /* RAMTST_SELECT_ALGPARAMS_API_ENABLE */

#if RAMTST_CHANGE_NUM_OF_TESTED_CELLS_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service changes the current number of tested cells
 * ServiceId           0x08
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RamTst_NumberOfTestedCellsType
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_ChangeNumberOfTestedCells(RamTst_NumberOfTestedCellsType NewNumberOfTestedCells);
#endif /* RAMTST_CHANGE_NUM_OF_TESTED_CELLS_API_ENABLE */

#if RAMTST_RUN_FULL_TEST_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for executing the full RAM Test in the foreground.
 * ServiceId           0x10
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_RunFullTest(void);
#endif /* RAMTST_RUN_FULL_TEST_API_ENABLE */

#if RAMTST_RUN_PARTIAL_TEST_API_ENABLE
/*************************************************************************/
/* RamTst
 * Brief               Service for testing one RAM block in the foreground
 * ServiceId           0x11
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockId：Identifies the single RAM block to be tested in the selected set of RamTstAlgParams.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_RunPartialTest(RamTst_NumberOfBlocksType BlockId);
#endif /* RAMTST_RUN_PARTIAL_TEST_API_ENABLE */

/*************************************************************************/
/* RamTst
 * Brief               Scheduled function for executing the RAM Test in the background
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_MainFunction(void);

#if (RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON)
/*************************************************************************/
/* RamTst
 * Brief               The function RamTst_TestCompleted shall be called every time when all RAM
                        blocks of the current test configuration have been tested in the background
 * Sync/Async          --
 * Reentrancy          Dont care
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_TestCompletedNotification(void);
#endif /* RAMTST_TEST_COMPLETE_NOTIFICATION_API_ENABLE == STD_ON */

#if (RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON)
/*************************************************************************/
/* RamTst
 * Brief               The function RamTst_Error shall be called every time when a RAM failure has
                        been detected by the selected test algorithm in the background.
 * Sync/Async          --
 * Reentrancy          Dont care
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI
 */
/*************************************************************************/
FUNC(void, RAMTST_CODE)
RamTst_ErrorNotification(void);
#endif /* RAMTST_TEST_ERROR_NOTIFICATION_API_ENABLE == STD_ON */

#endif /* RAMTST_H_ */
