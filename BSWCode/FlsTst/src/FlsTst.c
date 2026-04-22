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
 **  @file               : FlsTst.c
 **  @author             : peng.wu
 **  @date               : 2024/10/30
 **  @vendor             : isoft
 **  @description        : Implementation for FlsTst
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "FlsTst.h"
#include "SchM_FlsTst.h"
#if (FLSTST_DEM_FLSTST_E_FLSTST_FAILURE_ENABLE == STD_ON)
#include "Dem.h"
#endif
#include "Crc.h"
#if (FLSTST_TIMEOUT == STD_ON)
#include "FreeRTimer.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
#if (0u != FLSTST_BGND_BLOCK_NUM)
/**
 * @brief       Perform atomic calculation
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77013
 */
FLSTST_LOCAL void FlsTst_CalcAtomic(void);

/**
 * @brief       Perform internal test in background test
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77014
 */
FLSTST_LOCAL void FlsTst_InterTest(void);

/**
 * @brief       Update information when a block test completed
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77015
 */
FLSTST_LOCAL void FlsTst_BlockTestFinish(void);

/**
 * @brief       Copy block information
 * @param[in]   FlsTstBlockID: Used block ID
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77016
 */
FLSTST_LOCAL void FlsTst_CopyBlockInfo(uint32 FlsTstBlockID);
#endif

#if (0u != FLSTST_FGND_BLOCK_NUM)
/**
 * @brief       Perform internal test in foreground test
 * @param[in]   BlockInfoPtr: Pointer to block data information
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77017
 */
FLSTST_LOCAL Std_ReturnType FlsTst_StartFgndInterTest(FlsTst_BlockInfoType* BlockInfoPtr);

/**
 * @brief       Calculate CRC32 in foreground test
 * @param[in]   BlockInfoPtr: Pointer to block data information
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77018
 */
FLSTST_LOCAL Std_ReturnType FlsTst_CRC32Fgnd(FlsTst_BlockInfoType* BlockInfoPtr);

/**
 * @brief       Calculate CRC16 in foreground test
 * @param[in]   BlockInfoPtr: Pointer to block data information
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77019
 */
FLSTST_LOCAL Std_ReturnType FlsTst_CRC16Fgnd(FlsTst_BlockInfoType* BlockInfoPtr);

/**
 * @brief       Calculate CRC8 in foreground test
 * @param[in]   BlockInfoPtr: Pointer to block data information
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77020
 */
FLSTST_LOCAL Std_ReturnType FlsTst_CRC8Fgnd(FlsTst_BlockInfoType* BlockInfoPtr);

/**
 * @brief       Calculate checksum in foreground test
 * @param[in]   BlockInfoPtr: Pointer to block data information
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77021
 */
FLSTST_LOCAL Std_ReturnType FlsTst_ChecksumFgnd(FlsTst_BlockInfoType* BlockInfoPtr);

#if (STD_ON == FLSTST_TEST_ECC_API)
/**
 * @brief       Perform ECC test in foreground test
 * @param[in]   BlockInfoPtr: Pointer to block data information
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77022
 */
FLSTST_LOCAL Std_ReturnType FlsTst_ECCTestFgnd(FlsTst_BlockInfoType* BlockInfoPtr);
#endif

/**
 * @brief       Performe duplicated test in foreground test
 * @param[in]   BlockInfoPtr: Pointer to block data information
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77023
 */
FLSTST_LOCAL Std_ReturnType FlsTst_DuplicatedMemoryFgnd(FlsTst_BlockInfoType* BlockInfoPtr);
#endif

#if ((0u != FLSTST_BGND_BLOCK_NUM) || (0u != FLSTST_FGND_BLOCK_NUM))
/**
 * @brief       Compare memory in duplicated test
 * @param[in]   SourceDataPtr: Pointer to source data address
 * @param[in]   DstDataPtr: Pointer to destination data address
 * @param[in]   Length: Test length
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77024
 */
FLSTST_LOCAL Std_ReturnType FlsTst_MemCompare(const uint8* SourceDataPtr, const uint8* DstDataPtr, uint32 Length);

/**
 * @brief       Internal calculation when performeing checksum
 * @param[in]   Checksum_DataPtr: Pointer to data buffer
 * @param[in]   Checksum_Length: Test length
 * @param[in]   Checksum_StartValue: Start value
 * @param[in]   Checksum_IsFirstCall: First called flag
 * @return      uint32
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77025
 */
FLSTST_LOCAL uint32 FlsTst_CheckSum(
    const uint8* Checksum_DataPtr,
    uint32       Checksum_Length,
    uint32       Checksum_StartValue,
    boolean      Checksum_IsFirstCall);
#endif

#if (STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT)
/**
 * @brief       Notifation when a test is completed
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-77026
 */
FLSTST_LOCAL void FlsTst_TestCompletedNotification(void);
#endif

#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define FLSTST_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "FlsTst_MemMap.h"
#if (0u != FLSTST_BGND_BLOCK_NUM)
/**
 * @brief Used to record the Background test block information @range NA
 */
FLSTST_LOCAL FlsTst_BlockInfoType FlsTst_BlockBgndInfo[FLSTST_BGND_BLOCK_NUM];
/**
 * @brief Used to record the current running information @range NA
 */
FLSTST_LOCAL FlsTst_CurRunningType FlsTst_CurRunning;
#endif

#if (0u != FLSTST_FGND_BLOCK_NUM)
/**
 * @brief Used to record the Foreground test block information @range NA
 */
FLSTST_LOCAL FlsTst_BlockInfoType FlsTst_BlockFgndInfo[FLSTST_FGND_BLOCK_NUM];
#endif

/**
 * @brief Used to record the module information @range NA
 */
FLSTST_LOCAL FlsTst_ModuleType FlsTst_Module;

#if (STD_ON == FLSTST_INIT_ECC)
/**
 * @brief Used to record the ECC initial information @range NA
 */
FLSTST_LOCAL FlsTst_ECCInitType FlsTst_ECCInitInfo;
#endif
#define FLSTST_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "FlsTst_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define FLSTST_START_SEC_CODE
#include "FlsTst_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_FlsTst_1503 */
/**
 * @brief Check initialization status and input parameter, then initialize global variables and all state machines
 */
void FlsTst_Init(const FlsTst_ConfigType* ConfigPtr)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (NULL_PTR == ConfigPtr)
    {
        errorId = FLSTST_E_INIT_FAILED;
    }
    else if (FLSTST_UNINIT != FlsTst_Module.State)
    {
        errorId = FLSTST_E_ALREADY_INITIALIZED;
    }
#if (STD_ON == FLSTST_MULTI_PARTITION)
    else if (FALSE == FlsTst_CheckPartition())
    {
        errorId = FLSTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        SchM_Enter_FlsTst_Queue();
#if (0u != FLSTST_BGND_BLOCK_NUM)
        const uint32* sigBgndPtr;
#if (1u < FLSTST_BGND_BLOCK_NUM)
        for (uint8 blockIndexBgnd = 0u; blockIndexBgnd < FLSTST_BGND_BLOCK_NUM; blockIndexBgnd++)
#else
        uint8 blockIndexBgnd = 0u;
#endif
        {
            FlsTst_BlockBgndInfo[blockIndexBgnd].MemoryBlockID = ConfigPtr->BgndBlockPtr[blockIndexBgnd].BgndBlockIndex;
            FlsTst_BlockBgndInfo[blockIndexBgnd].TestAlgorithm = ConfigPtr->BgndBlockPtr[blockIndexBgnd].TestAlgorithm;
            FlsTst_BlockBgndInfo[blockIndexBgnd].BlcokStartAddrPtr =
                ConfigPtr->BgndBlockPtr[blockIndexBgnd].BlockBaseAddrPtr;
            FlsTst_BlockBgndInfo[blockIndexBgnd].MemBlcokLength = ConfigPtr->BgndBlockPtr[blockIndexBgnd].BlcokLength;
            /** Duplicated memory start address */
            FlsTst_BlockBgndInfo[blockIndexBgnd].DuplicateAddrPtr =
                ConfigPtr->BgndBlockPtr[blockIndexBgnd].DuplicateAddrPtr;
            sigBgndPtr = ConfigPtr->BgndBlockPtr[blockIndexBgnd].SignatureAddrPtr;
            FlsTst_BlockBgndInfo[blockIndexBgnd].SignatureStored    = *sigBgndPtr;
            FlsTst_BlockBgndInfo[blockIndexBgnd].SignatueCalculated = 0u;
            FlsTst_BlockBgndInfo[blockIndexBgnd].TestResult         = FLSTST_RESULT_NOT_TESTED;
        }
#endif

#if (0u != FLSTST_FGND_BLOCK_NUM)
        const uint32* sigFgndPtr;
#if (1u < FLSTST_FGND_BLOCK_NUM)
        for (uint8 blockIndexFgnd = 0u; blockIndexFgnd < FLSTST_FGND_BLOCK_NUM; blockIndexFgnd++)
#else
        uint8 blockIndexFgnd = 0u;
#endif
        {
            FlsTst_BlockFgndInfo[blockIndexFgnd].MemoryBlockID = ConfigPtr->FgndBlockPtr[blockIndexFgnd].FgndBlockIndex;
            FlsTst_BlockFgndInfo[blockIndexFgnd].TestAlgorithm = ConfigPtr->FgndBlockPtr[blockIndexFgnd].TestAlgorithm;
            FlsTst_BlockFgndInfo[blockIndexFgnd].BlcokStartAddrPtr =
                ConfigPtr->FgndBlockPtr[blockIndexFgnd].BlockBaseAddrPtr;
            FlsTst_BlockFgndInfo[blockIndexFgnd].MemBlcokLength = ConfigPtr->FgndBlockPtr[blockIndexFgnd].BlcokLength;
            /** Duplicated memory start address */
            FlsTst_BlockFgndInfo[blockIndexFgnd].DuplicateAddrPtr =
                ConfigPtr->FgndBlockPtr[blockIndexFgnd].DuplicateAddrPtr;
            sigFgndPtr = ConfigPtr->FgndBlockPtr[blockIndexFgnd].SignatureAddrPtr;
            FlsTst_BlockFgndInfo[blockIndexFgnd].SignatureStored    = *sigFgndPtr;
            FlsTst_BlockFgndInfo[blockIndexFgnd].SignatueCalculated = 0u;
            FlsTst_BlockFgndInfo[blockIndexFgnd].TestResult         = FLSTST_RESULT_NOT_TESTED;
        }
#endif

#if (STD_ON == FLSTST_INIT_ECC)
        FlsTst_ECCInitInfo.ECCInitPtr   = ConfigPtr->ECCApiPtr->ECCInitFunc;
        FlsTst_ECCInitInfo.ECCDeinitPtr = ConfigPtr->ECCApiPtr->ECCDeInitFunc;
        FlsTst_ECCInitInfo.ECCInitPtr();
#endif
        FlsTst_Module.State                     = FLSTST_INIT;
        FlsTst_Module.ErrorInfo.ErrorBlockID    = 0xFFu; /* PRQA S 1252,3120 */ /* VL_QAC_1252,VL_QAC_MagicNum */
        FlsTst_Module.ErrorInfo.Algorithm       = FLSTST_16BIT_CRC;
        FlsTst_Module.ErrorInfo.SignatureResult = 0u;
        FlsTst_Module.CompleteNotificationPtr   = ConfigPtr->ConfigSetPtr->CompleteNotificationFunc;
#if (STD_ON == FLSTST_TEST_ECC_API)
        FlsTst_Module.ECCTestFunction = ConfigPtr->ECCApiPtr->ECCTestFunc;
#endif
        FlsTst_Module.IntervalID = 0u;
        FlsTst_Module.BgndResult = FLSTST_RESULT_NOT_TESTED;
        FlsTst_Module.FgndResult = FLSTST_NOT_TESTED;
        SchM_Exit_FlsTst_Queue();
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError((uint8)FLSTST_INIT_ID, errorId);
    }
#endif
}

/**
 * @brief Check initialization status, then De-initialize global variables and all state machines
 */
void FlsTst_DeInit(void)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        errorId = FLSTST_E_UNINIT;
    }
#if (STD_ON == FLSTST_MULTI_PARTITION)
    else if (FALSE == FlsTst_CheckPartition())
    {
        errorId = FLSTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        SchM_Enter_FlsTst_Queue();
#if (0u != FLSTST_BGND_BLOCK_NUM)
#if (1u < FLSTST_BGND_BLOCK_NUM)
        for (uint8 blockIndexBgnd = 0u; blockIndexBgnd < FLSTST_BGND_BLOCK_NUM; blockIndexBgnd++)
#else
        uint8 blockIndexBgnd = 0u;
#endif
        {
            FlsTst_BlockBgndInfo[blockIndexBgnd].MemoryBlockID      = 0u;
            FlsTst_BlockBgndInfo[blockIndexBgnd].TestAlgorithm      = FLSTST_16BIT_CRC;
            FlsTst_BlockBgndInfo[blockIndexBgnd].BlcokStartAddrPtr  = NULL_PTR;
            FlsTst_BlockBgndInfo[blockIndexBgnd].MemBlcokLength     = 0u;
            FlsTst_BlockBgndInfo[blockIndexBgnd].SignatureStored    = 0u;
            FlsTst_BlockBgndInfo[blockIndexBgnd].SignatueCalculated = 0u;
            FlsTst_BlockBgndInfo[blockIndexBgnd].TestResult         = FLSTST_RESULT_NOT_TESTED;
        }
#endif

#if (0u != FLSTST_FGND_BLOCK_NUM)
#if (1u < FLSTST_FGND_BLOCK_NUM)
        for (uint8 blockIndexFgnd = 0u; blockIndexFgnd < FLSTST_FGND_BLOCK_NUM; blockIndexFgnd++)
#else
        uint8 blockIndexFgnd = 0u;
#endif
        {
            FlsTst_BlockFgndInfo[blockIndexFgnd].MemoryBlockID      = 0u;
            FlsTst_BlockFgndInfo[blockIndexFgnd].TestAlgorithm      = FLSTST_16BIT_CRC;
            FlsTst_BlockFgndInfo[blockIndexFgnd].BlcokStartAddrPtr  = NULL_PTR;
            FlsTst_BlockFgndInfo[blockIndexFgnd].MemBlcokLength     = 0u;
            FlsTst_BlockFgndInfo[blockIndexFgnd].SignatureStored    = 0u;
            FlsTst_BlockFgndInfo[blockIndexFgnd].SignatueCalculated = 0u;
            FlsTst_BlockFgndInfo[blockIndexFgnd].TestResult         = FLSTST_RESULT_NOT_TESTED;
        }
#endif

#if (STD_ON == FLSTST_INIT_ECC)
        FlsTst_ECCInitInfo.ECCDeinitPtr();
#endif
        FlsTst_Module.State = FLSTST_UNINIT;
        SchM_Exit_FlsTst_Queue();
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError(FLSTST_DEINIT_ID, errorId);
    }
#endif
}

/**
 * @brief Check initialization status and input parameter, then start Foreground test
 */
#if ((STD_ON == FLSTST_START_FGND_API) && (0u != FLSTST_FGND_BLOCK_NUM))
Std_ReturnType FlsTst_StartFgnd(FlsTst_BlockIdFgndType FgndBlockId)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        errorId = FLSTST_E_UNINIT;
    }
    else if (FgndBlockId >= FLSTST_FGND_BLOCK_NUM) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        errorId = FLSTST_E_PARAM_INVALID;
    }
#if (STD_ON == FLSTST_MULTI_PARTITION)
    else if (FALSE == FlsTst_CheckPartition())
    {
        errorId = FLSTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        retVal = FlsTst_StartFgndInterTest(&FlsTst_BlockFgndInfo[FgndBlockId]);
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError((uint8)FLSTST_STARTFGND_ID, errorId);
    }
#endif
    return retVal;
}
#endif

/**
 * @brief Check the validity of the module state and set the state to abort
 */
#if (0u != FLSTST_BGND_BLOCK_NUM)
void FlsTst_Abort(void)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        errorId = FLSTST_E_UNINIT;
    }
    else if (FLSTST_ABORTED == FlsTst_Module.State)
    {
        errorId = FLSTST_E_STATE_FAILURE;
    }
#if (STD_ON == FLSTST_MULTI_PARTITION)
    else if (FALSE == FlsTst_CheckPartition())
    {
        errorId = FLSTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        FlsTst_Module.State = FLSTST_ABORTED;
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError((uint8)FLSTST_ABORT_ID, errorId);
    }
#endif
}
#endif

/**
 * @brief Check the validity of the module state and set the state to suspend
 */
#if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0u != FLSTST_BGND_BLOCK_NUM))
void FlsTst_Suspend(void)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        errorId = FLSTST_E_UNINIT;
    }
    else if (FLSTST_ABORTED == FlsTst_Module.State)
    {
        errorId = FLSTST_E_STATE_FAILURE;
    }
#if (STD_ON == FLSTST_MULTI_PARTITION)
    else if (FALSE == FlsTst_CheckPartition())
    {
        errorId = FLSTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        FlsTst_Module.State = FLSTST_SUSPENDED;
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError((uint8)FLSTST_SUSPEND_ID, errorId);
    }
#endif
}
#endif

/**
 * @brief Check the validity of the module state and set the state to running
 */
#if ((STD_ON == FLSTST_SUSPEND_RESUME_API) && (0u != FLSTST_BGND_BLOCK_NUM))
void FlsTst_Resume(void)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        errorId = FLSTST_E_UNINIT;
    }
    else if (FLSTST_SUSPENDED != FlsTst_Module.State)
    {
        errorId = FLSTST_E_STATE_FAILURE;
    }
#if (STD_ON == FLSTST_MULTI_PARTITION)
    else if (FALSE == FlsTst_CheckPartition())
    {
        errorId = FLSTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        FlsTst_Module.State = FLSTST_RUNNING;
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError((uint8)FLSTST_RESUME_ID, errorId);
    }
#endif
}
#endif

/**
 * @brief Return the current state
 */
#if (STD_ON == FLSTST_GET_CURRENT_STATE_API)
FlsTst_StateType FlsTst_GetCurrentState(void)
{
    return FlsTst_Module.State;
}
#endif

/**
 * @brief Check the validity of the module state, then get the internal test ID and the result of Background
 */
#if ((STD_ON == FLSTST_GET_TEST_RESULT_BGND_API) && (0u != FLSTST_BGND_BLOCK_NUM))
FlsTst_TestResultBgndType FlsTst_GetTestResultBgnd(void)
{
    FlsTst_TestResultBgndType testResultBgnd = {0u, FLSTST_RESULT_NOT_TESTED};

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FlsTst_Det_ReportError(FLSTST_GETTESTRESULTBGND_ID, FLSTST_E_UNINIT);
    }
    else
#endif
    {
        testResultBgnd.ResultIntervalID = FlsTst_Module.IntervalID;
        testResultBgnd.result           = FlsTst_Module.BgndResult;
    }

    return testResultBgnd;
}
#endif

/**
 * @brief Check the validity of the module state, then get the result of Foreground
 */
#if ((STD_ON == FLSTST_GET_TEST_RESULT_FGND_API) && (0u != FLSTST_FGND_BLOCK_NUM))
FlsTst_TestResultFgndType FlsTst_GetTestResultFgnd(void)
{
    FlsTst_TestResultFgndType testResultFgnd = FLSTST_NOT_TESTED;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FlsTst_Det_ReportError(FLSTST_GETTESTRESULTFGND_ID, FLSTST_E_UNINIT);
    }
    else
#endif
    {
        testResultFgnd = FlsTst_Module.FgndResult;
    }
    return testResultFgnd;
}
#endif

/**
 * @brief Check initialization status and input parameter, then get the module version information
 */
#if (STD_ON == FLSTST_VERSION_INFO_API)
void FlsTst_GetVersionInfo(Std_VersionInfoType* versionInfo)
{
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        errorId = FLSTST_E_UNINIT;
    }
    else if (NULL_PTR == versionInfo)
    {
        errorId = FLSTST_E_PARAM_POINTER;
    }
    else
#endif
    {
        (versionInfo)->vendorID         = (uint16)FLSTST_VENDOR_ID;
        (versionInfo)->moduleID         = (uint8)FLSTST_MODULE_ID;
        (versionInfo)->sw_major_version = (uint8)FLSTST_SW_MAJOR_VERSION;
        (versionInfo)->sw_minor_version = (uint8)FLSTST_SW_MINOR_VERSION;
        (versionInfo)->sw_patch_version = (uint8)FLSTST_SW_PATCH_VERSION;
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError((uint8)FLSTST_GETVERSIONINFO_ID, errorId);
    }
#endif
}
#endif

/**
 * @brief Check initialization status, calculates the signature values for all blocks, and then gets the current
 * internal test ID
 */
#if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_BGND_API) && (0u != FLSTST_BGND_BLOCK_NUM))
FlsTst_TestSignatureBgndType FlsTst_GetTestSignatureBgnd(void)
{
    FlsTst_TestSignatureBgndType signatureBgnd = {0u, 0u};

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FlsTst_Det_ReportError(FLSTST_MAINFUNCTION_ID, FLSTST_E_UNINIT);
    }
    else
#endif
    {
        for (uint32 bolckId = 0u; bolckId < FLSTST_BGND_BLOCK_NUM; bolckId++) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            if (FLSTST_RESULT_OK == FlsTst_BlockBgndInfo[bolckId].TestResult)
            {
                /* sum of Signature of all blocks */
                signatureBgnd.BgndSignature += FlsTst_BlockBgndInfo[bolckId].SignatueCalculated;
            }
            else
            {
                /** empty */
            }
        }
        signatureBgnd.SignatureIntervalID = FlsTst_Module.IntervalID;
    }
    return signatureBgnd;
}
#endif

/**
 * @brief Check initialization status, calculates the signature values for all blocks, and then gets the current
 * internal test ID
 */
#if ((STD_ON == FLSTST_GET_TEST_SIGNATURE_FGND_API) && (0u != FLSTST_FGND_BLOCK_NUM))
FlsTst_TestSignatureFgndType FlsTst_GetTestSignatureFgnd(void)
{
    FlsTst_TestSignatureFgndType signatureFgnd;
    signatureFgnd.FgndSignature = 0u;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FlsTst_Det_ReportError(FLSTST_GETTESTSIGNATUREFGND_ID, FLSTST_E_UNINIT);
    }
    else
#endif
    {
#if (1u < FLSTST_FGND_BLOCK_NUM)
        for (uint32 bolckId = 0u; bolckId < FLSTST_FGND_BLOCK_NUM; bolckId++)
#else
        uint32 bolckId = 0u;
#endif
        {
            if (FLSTST_RESULT_OK == FlsTst_BlockFgndInfo[bolckId].TestResult)
            {
                /* sum of Signature of all blocks */
#if (1u < FLSTST_FGND_BLOCK_NUM)
                signatureFgnd.FgndSignature += FlsTst_BlockFgndInfo[bolckId].SignatueCalculated;
#else
                signatureFgnd.FgndSignature = FlsTst_BlockFgndInfo[bolckId].SignatueCalculated;
#endif
            }
            else
            {
                /* empty */
            }
        }
    }
    return signatureFgnd;
}
#endif

/**
 * @brief Check initialization status, then get the error information
 */
#if ((STD_ON == FLSTST_GET_ERRORD_ETAILS_API) && ((0u != FLSTST_BGND_BLOCK_NUM) || (0u != FLSTST_FGND_BLOCK_NUM)))
FlsTst_ErrorDetailsType FlsTst_GetErrorDetails(void)
{
    FlsTst_ErrorDetailsType errDetails = {0u, FLSTST_16BIT_CRC, 0u};

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        FlsTst_Det_ReportError(FLSTST_GETERRORDETAILS_ID, FLSTST_E_UNINIT);
    }
    else
#endif
    {
        errDetails = FlsTst_Module.ErrorInfo;
    }
    return errDetails;
}
#endif
/* PRQA S 1503 -- */

/**
 * @brief Check initialization status, calculates the test length, and then invoke the ECC test function
 */
#if ((STD_ON == FLSTST_TEST_ECC_API) && ((0u != FLSTST_BGND_BLOCK_NUM) || (0u != FLSTST_FGND_BLOCK_NUM)))
Std_ReturnType FlsTst_TestEcc(void) /* PRQA S 1505 */ /* VL_FlsTst_1505 */
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (FLSTST_UNINIT == FlsTst_Module.State)
    {
        errorId = FLSTST_E_UNINIT;
    }
#if (STD_ON == FLSTST_MULTI_PARTITION)
    else if (FALSE == FlsTst_CheckPartition())
    {
        errorId = FLSTST_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        FlsTst_LengthType eccLength;
        if (FLSTST_TESTCELL_LENGTH_ATOMIC < FlsTst_CurRunning.TestLength) /* PRQA S 1252 */ /* VL_QAC_1252 */
        {
            eccLength = FLSTST_TESTCELL_LENGTH_ATOMIC;
        }
        else
        {
            eccLength = FlsTst_CurRunning.TestLength;
        }

        if (NULL_PTR != FlsTst_Module.ECCTestFunction)
        {
            const FlsTst_AddressType* startAddrPtr = FlsTst_CurRunning.TestAddrPtr;
            const FlsTst_AddressType* endAddrPtr   = startAddrPtr + eccLength; /* PRQA S 0488 */ /* VL_FlsTst_0488 */
            retVal                                 = FlsTst_Module.ECCTestFunction(startAddrPtr, endAddrPtr);
        }
        else
        {
            /* Do not support Flash ECC test function */
        }
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError(FLSTST_TESTECC_ID, errorId);
    }
#endif

    return retVal;
}
#endif

/**
 * @brief Invoke the corresponding function interface according to the module status
 */
void FlsTst_MainFunction(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if (FLSTST_UNINIT != FlsTst_Module.State)
    {
#if (STD_ON == FLSTST_MULTI_PARTITION)
        if (TRUE == FlsTst_CheckPartition())
#endif
        {
            if (FLSTST_INIT == FlsTst_Module.State)
            {
                /** Auto start */
                FlsTst_Module.State = FLSTST_RUNNING;
                if (FLSTST_RESULT_NOT_TESTED != FlsTst_Module.BgndResult)
                {
                    FlsTst_Module.IntervalID++;
                }
                if (FlsTst_Module.IntervalID > FLSTST_TEST_INTERVALID_END_VALUE) /* PRQA S 1252 */ /* VL_QAC_1252 */
                {
                    FlsTst_Module.IntervalID = 0u;
                }
                FlsTst_CopyBlockInfo(0u);
                FlsTst_InterTest();
            }
            else if (FLSTST_RUNNING == FlsTst_Module.State)
            {
                /** Continue Calc */
                FlsTst_InterTest();
            }
            else
            {
                /** FLSTST_ABORTED, FLSTST_SUSPENDED */
                /** do nothing */
            }
        }
    }
}

/**
 * @brief Check initialization status, and then invoke the callback interface
 */
#if (0u != FLSTST_BGND_BLOCK_NUM)
#if (STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT)
FLSTST_LOCAL void FlsTst_TestCompletedNotification(void)
{
    if (NULL_PTR != FlsTst_Module.CompleteNotificationPtr)
    {
        FlsTst_Module.CompleteNotificationPtr();
    }
}
#endif

/* ========================================== internal function definitions ========================================= */
/**
 * @brief Check the validity of the BlockID and copy Background test block information
 */
FLSTST_LOCAL void FlsTst_CopyBlockInfo(uint32 FlsTstBlockID)
{
    SchM_Enter_FlsTst_Queue();
    FlsTst_CurRunning.FirstFlag     = FALSE;
    FlsTst_CurRunning.CalcResult    = 0u;
    FlsTst_CurRunning.CurBlockID    = FlsTst_BlockBgndInfo[FlsTstBlockID].MemoryBlockID;
    FlsTst_CurRunning.TestAddrPtr   = FlsTst_BlockBgndInfo[FlsTstBlockID].BlcokStartAddrPtr;
    FlsTst_CurRunning.TestLength    = FlsTst_BlockBgndInfo[FlsTstBlockID].MemBlcokLength;
    FlsTst_CurRunning.TestAlgorithm = FlsTst_BlockBgndInfo[FlsTstBlockID].TestAlgorithm;
    /** Duplicated memory start address */
    FlsTst_CurRunning.DupTestAddrPtr = FlsTst_BlockBgndInfo[FlsTstBlockID].DuplicateAddrPtr;
    SchM_Exit_FlsTst_Queue();
}

/**
 * @brief Check the validity of the BlockID, then copy Background test block information. If all blocks are not
 * tested, the test continues, an error message is recorded if an error occurs, and the end notification callback
 * interface is invoked if all blocks are tested
 */
FLSTST_LOCAL void FlsTst_BlockTestFinish(void)
{
    uint32 blockID = FlsTst_CurRunning.CurBlockID;

    FlsTst_BlockBgndInfo[blockID].SignatueCalculated = FlsTst_CurRunning.CalcResult;
    if (FlsTst_BlockBgndInfo[blockID].SignatueCalculated == FlsTst_BlockBgndInfo[blockID].SignatureStored)
    {
        FlsTst_BlockBgndInfo[blockID].TestResult = FLSTST_RESULT_OK;
    }
    else
    {
        FlsTst_Module.BgndResult                 = FLSTST_RESULT_NOT_OK;
        FlsTst_BlockBgndInfo[blockID].TestResult = FLSTST_RESULT_NOT_OK;
        /** at least one block test failed */
#if ((FLSTST_TEST_RESULTSIGNATURE == STD_OFF) && (FLSTST_DEM_FLSTST_E_FLSTST_FAILURE_ENABLE == STD_ON))
        Dem_SetEventStatus(FLSTST_E_FLSTST_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif
    }

    /** Another block need to be tested */
#if (FLSTST_BGND_BLOCK_NUM > 1)
    if (blockID < ((uint32)(FLSTST_BGND_BLOCK_NUM - 1u)))
    {
        blockID++;
        FlsTst_CopyBlockInfo(blockID);
    }
    else /** the last block test finished */
#endif
    {
        boolean errorBlock = FALSE;
        /** no more block need to be tested, current test interval finished. */
#if (1u < FLSTST_BGND_BLOCK_NUM)
        for (uint32 BlockIndex = 0u; BlockIndex < FLSTST_BGND_BLOCK_NUM; BlockIndex++)
#else
        uint32 BlockIndex = 0u;
#endif
        {
            if (FLSTST_RESULT_NOT_OK == FlsTst_BlockBgndInfo[BlockIndex].TestResult)
            {
                errorBlock                              = TRUE;
                FlsTst_Module.ErrorInfo.ErrorBlockID    = BlockIndex;
                FlsTst_Module.ErrorInfo.Algorithm       = FlsTst_BlockBgndInfo[BlockIndex].TestAlgorithm;
                FlsTst_Module.ErrorInfo.SignatureResult = FlsTst_BlockBgndInfo[BlockIndex].SignatueCalculated;
#if (1u < FLSTST_BGND_BLOCK_NUM)
                break;
#endif
            }
        }

        if (FALSE == errorBlock)
        {
            FlsTst_Module.BgndResult = FLSTST_RESULT_OK;
#if ((FLSTST_TEST_RESULTSIGNATURE == STD_OFF) && (FLSTST_DEM_FLSTST_E_FLSTST_FAILURE_ENABLE == STD_ON))
            Dem_SetEventStatus(FLSTST_E_FLSTST_FAILURE, DEM_EVENT_STATUS_PASSED);
#endif
        }
        else
        {
            /** empty */
        }

        FlsTst_Module.State = FLSTST_INIT;
#if (STD_ON == FLSTST_COMPLETED_NOTIFY_SUPPORT)
        FlsTst_TestCompletedNotification();
#endif
    }
}

/**
 * @brief If the length is 0 and the status is running, the test end interface is called, otherwise the test length
 * is calculated and the atomic test is performed
 */
FLSTST_LOCAL void FlsTst_InterTest(void)
{
    Std_ReturnType retVal = E_OK;
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
#endif

    if ((FlsTst_CurRunning.TestLength == 0uL) && (FLSTST_RUNNING == FlsTst_Module.State))
    {
        /** Current Block Calc Finish */
        FlsTst_BlockTestFinish();
    }
    else
    {
        uint32 cycleTimes;
        /** First calculate CRC */
        if ((FLSTST_TESTCELLS_NUM * FLSTST_TESTCELL_LENGTH_ATOMIC) < FlsTst_CurRunning.TestLength)
        {
            cycleTimes = FLSTST_TESTCELLS_NUM;
        }
        else /** Length smaller than FLSTST_NUMOFTESTEDCELLS,but not 0 */
        {
            /* PRQA S 1252 ++ */ /* VL_QAC_1252 */
            cycleTimes = FlsTst_CurRunning.TestLength / FLSTST_TESTCELL_LENGTH_ATOMIC;
            /* PRQA S 1252 -- */
            cycleTimes = cycleTimes + 1uL;
        }

#if (FLSTST_TIMEOUT == STD_ON)
        uint32 timeStartBgnd = Frt_ReadOutMS();
#endif

        for (uint32 cycleCounter = 0u; ((cycleCounter < cycleTimes) && (retVal == E_OK)); cycleCounter++)
        {
            if (FLSTST_RUNNING == FlsTst_Module.State)
            {
                FlsTst_CalcAtomic();
#if (FLSTST_TIMEOUT == STD_ON)
                uint32 timeGapBgnd = Frt_CalculateElapsedMS(timeStartBgnd);
                if (timeGapBgnd >= FLSTST_TIMEOUTVALUE_BGND)
                {
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
                    errorId = FLSTST_E_TIMEOUT;
#endif
                    FlsTst_Abort();
                    FlsTst_Module.BgndResult = FLSTST_RESULT_NOT_OK;
                    retVal                   = E_NOT_OK;
                }
#endif
            }
            else if ((FLSTST_SUSPENDED == FlsTst_Module.State) || (FLSTST_ABORTED == FlsTst_Module.State))
            {
                retVal = E_NOT_OK;
            }
            else
            {
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
                errorId = FLSTST_E_STATE_FAILURE;
#endif
            }

            if (0uL == FlsTst_CurRunning.TestLength)
            {
                break;
            }
        }
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError((uint8)FLSTST_MAINFUNCTION_ID, errorId);
    }
#endif
}

/**
 * @brief Check if the CRC is computed for the first time, check the test length, call the algorithm interface based
 * on the configured algorithm, and then update the current test address and length
 */
/* PRQA S 6070 ++ */ /* VL_MTR_FlsTst_STCAL */
FLSTST_LOCAL void FlsTst_CalcAtomic(void)
{
    FlsTst_LengthType crcLength;
    boolean           firstTimeCalc;

    SchM_Enter_FlsTst_Queue();
    /** if a block was First time to calculate CRC */
    if (FALSE == FlsTst_CurRunning.FirstFlag)
    {
        FlsTst_CurRunning.FirstFlag = TRUE;
        firstTimeCalc               = TRUE;
    }
    else
    {
        firstTimeCalc = FALSE;
    }

    if (FLSTST_TESTCELL_LENGTH_ATOMIC < FlsTst_CurRunning.TestLength) /* PRQA S 1252 */ /* VL_QAC_1252 */
    {
        crcLength = FLSTST_TESTCELL_LENGTH_ATOMIC;
    }
    else
    {
        crcLength = FlsTst_CurRunning.TestLength;
    }

    switch (FlsTst_CurRunning.TestAlgorithm)
    {
    case FLSTST_16BIT_CRC:
        FlsTst_CurRunning.CalcResult = (uint32)Crc_CalculateCRC16(
            (uint8*)FlsTst_CurRunning.TestAddrPtr,
            crcLength,
            (uint16)FlsTst_CurRunning.CalcResult,
            firstTimeCalc);
        break;

    case FLSTST_32BIT_CRC:
        FlsTst_CurRunning.CalcResult = (uint32)Crc_CalculateCRC32(
            (uint8*)FlsTst_CurRunning.TestAddrPtr,
            crcLength,
            (uint32)FlsTst_CurRunning.CalcResult,
            firstTimeCalc);
        break;

    case FLSTST_8BIT_CRC:
        FlsTst_CurRunning.CalcResult = (uint32)Crc_CalculateCRC8(
            (uint8*)FlsTst_CurRunning.TestAddrPtr,
            crcLength,
            (uint8)FlsTst_CurRunning.CalcResult,
            firstTimeCalc);
        break;

    case FLSTST_CHECKSUM:
        FlsTst_CurRunning.CalcResult = (uint32)FlsTst_CheckSum(
            (uint8*)FlsTst_CurRunning.TestAddrPtr,
            crcLength,
            (uint32)FlsTst_CurRunning.CalcResult,
            firstTimeCalc);
        break;

    case FLSTST_DUPLICATED_MEMORY:
        FlsTst_CurRunning.CalcResult = (uint32)FlsTst_MemCompare(
            (uint8*)FlsTst_CurRunning.TestAddrPtr,
            (uint8*)FlsTst_CurRunning.DupTestAddrPtr,
            crcLength);
        break;

    case FLSTST_ECC:
#if ((STD_ON == FLSTST_TEST_ECC_API) && ((0u != FLSTST_BGND_BLOCK_NUM) || (0u != FLSTST_FGND_BLOCK_NUM)))
        FlsTst_CurRunning.CalcResult = FlsTst_TestEcc();
#endif
        break;

    default:
        /** do not support other Algorithm */
        break;
    }
    FlsTst_CurRunning.TestLength -= crcLength;
    FlsTst_CurRunning.TestAddrPtr += crcLength; /* PRQA S 0488 */    /* VL_FlsTst_0488 */
    FlsTst_CurRunning.DupTestAddrPtr += crcLength; /* PRQA S 0488 */ /* VL_FlsTst_0488 */
    SchM_Exit_FlsTst_Queue();
    return;
}
/* PRQA S 6070 -- */
#endif

/**
 * @brief Check the input parameter, store the current state and update the state to running, call the algorithm
 * interface based on the configured algorithm, an error message is recorded if an error occurs, then restore the status
 * of the module before the test
 */
#if (0u != FLSTST_FGND_BLOCK_NUM)
FLSTST_LOCAL Std_ReturnType FlsTst_StartFgndInterTest(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    uint8 errorId = FLSTST_E_NO_ERROR;
    if (NULL_PTR == BlockInfoPtr)
    {
        errorId = FLSTST_E_PARAM_POINTER;
    }
    else
#endif
    {
        boolean          blockError        = FALSE;
        FlsTst_StateType tempState         = FlsTst_Module.State;
        FlsTst_Module.State                = FLSTST_RUNNING;
        FlsTst_AlgorithmType testAlgorithm = BlockInfoPtr->TestAlgorithm;
        /* PRQA S 1252,3223 ++ */ /* VL_QAC_1252,VL_FlsTst_3223 */
        FLSTST_LOCAL const FlsTst_JobFuncPtr FlsTst_JobFuncMatrix[] = {
            NULL_PTR,                     /** Invalid algorithm */
            &FlsTst_CRC16Fgnd,            /** FLSTST_16BIT_CRC */
            &FlsTst_CRC32Fgnd,            /** FLSTST_32BIT_CRC */
            &FlsTst_CRC8Fgnd,             /** FLSTST_8BIT_CRC */
            &FlsTst_ChecksumFgnd,         /** FLSTST_CHECKSUM */
            &FlsTst_DuplicatedMemoryFgnd, /** FLSTST_DUPLICATED_MEMORY */
#if (STD_ON == FLSTST_TEST_ECC_API)
            &FlsTst_ECCTestFgnd, /** FLSTST_ECC */
#endif
        };
        /* PRQA S 1252,3223 -- */

#if (FLSTST_TIMEOUT == STD_ON)
        uint32 timeStartFgnd = Frt_ReadOutMS();
#endif
        if (testAlgorithm != FLSTST_ALGORITHM_UNDEFINED)
        {
            retVal = (*FlsTst_JobFuncMatrix[testAlgorithm])(BlockInfoPtr);
        }
        else
        {
            retVal = E_NOT_OK;
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
            errorId = FLSTST_E_PARAM_INVALID;
#endif
        }
#if (FLSTST_TIMEOUT == STD_ON)
        uint32 timeGapFgnd = Frt_CalculateElapsedMS(timeStartFgnd);
        if (timeGapFgnd >= FLSTST_TIMEOUTVALUE_FGND)
        {
            timeGapFgnd = 0u;
#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
            errorId = FLSTST_E_TIMEOUT;
#endif
            FlsTst_Abort();
            FlsTst_Module.FgndResult = FLSTST_NOT_OK;
            blockError               = TRUE;
        }
#endif
        if ((FLSTST_RESULT_NOT_OK == BlockInfoPtr->TestResult)
#if (FLSTST_TIMEOUT == STD_ON)
            && (blockError == FALSE)
#endif
        )
        {
            /** copy error details info */
            FlsTst_Module.ErrorInfo.ErrorBlockID    = BlockInfoPtr->MemoryBlockID;
            FlsTst_Module.ErrorInfo.Algorithm       = BlockInfoPtr->TestAlgorithm;
            FlsTst_Module.ErrorInfo.SignatureResult = BlockInfoPtr->SignatueCalculated;
            FlsTst_Module.FgndResult                = FLSTST_NOT_OK;
            blockError                              = TRUE;
#if (FLSTST_DEM_FLSTST_E_FLSTST_FAILURE_ENABLE == STD_ON)
            Dem_SetEventStatus(FLSTST_E_FLSTST_FAILURE, DEM_EVENT_STATUS_FAILED);
#endif
        }

        if (FALSE == blockError)
        {
            FlsTst_Module.FgndResult = FLSTST_OK;
        }
        else
        {
            FlsTst_Module.FgndResult = FLSTST_NOT_OK;
        }
        if (FLSTST_RUNNING == FlsTst_Module.State)
        {
            FlsTst_Module.State = tempState;
        }
    }

#if (STD_ON == FLSTST_DEV_ERROR_DETECT)
    if (errorId != FLSTST_E_NO_ERROR)
    {
        FlsTst_Det_ReportError((uint8)FLSTST_MAINFUNCTION_ID, errorId);
    }
#endif
    return retVal;
}

/**
 * @brief Check the input parameter and algorithm, calculate CRC32 and update the Foreground test results based on
 * the calculation results
 */
FLSTST_LOCAL Std_ReturnType FlsTst_CRC32Fgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    const FlsTst_AddressType* startAddrPtr    = BlockInfoPtr->BlcokStartAddrPtr;
    FlsTst_LengthType         testLength      = BlockInfoPtr->MemBlcokLength;
    FlsTst_SignatureType      signatureResult = 0u;

    /** if test method changes,Modify the code below */
    /** Use CRC32 */
    signatureResult = Crc_CalculateCRC32((const uint8*)startAddrPtr, testLength, signatureResult, (boolean)TRUE);

    BlockInfoPtr->SignatueCalculated = signatureResult;
    if (BlockInfoPtr->SignatureStored == BlockInfoPtr->SignatueCalculated)
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        retVal                   = E_OK;
    }
    else
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Check the input parameter and algorithm, calculate CRC16 and update the Foreground test results based on
 * the calculation results
 */
FLSTST_LOCAL Std_ReturnType FlsTst_CRC16Fgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    const FlsTst_AddressType* startAddrPtr    = BlockInfoPtr->BlcokStartAddrPtr;
    FlsTst_LengthType         testLength      = BlockInfoPtr->MemBlcokLength;
    FlsTst_SignatureType      signatureResult = 0u;

    /** if test method changes,Modify the code below */
    /** Use CRC16 */
    signatureResult = (uint32)Crc_CalculateCRC16((const uint8*)startAddrPtr, testLength, (uint16)signatureResult, TRUE);

    BlockInfoPtr->SignatueCalculated = signatureResult;
    if (BlockInfoPtr->SignatureStored == BlockInfoPtr->SignatueCalculated)
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        retVal                   = E_OK;
    }
    else
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Check the input parameter and algorithm, calculate CRC8 and update the Foreground test results based on
 * the calculation results
 */
FLSTST_LOCAL Std_ReturnType FlsTst_CRC8Fgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    const FlsTst_AddressType* startAddrPtr    = BlockInfoPtr->BlcokStartAddrPtr;
    FlsTst_LengthType         testLength      = BlockInfoPtr->MemBlcokLength;
    FlsTst_SignatureType      signatureResult = 0u;

    /** if test method changes,Modify the code below */
    /** Use CRC8 */
    signatureResult = (uint32)Crc_CalculateCRC8((const uint8*)startAddrPtr, testLength, (uint8)signatureResult, TRUE);
    BlockInfoPtr->SignatueCalculated = signatureResult;
    if (BlockInfoPtr->SignatureStored == BlockInfoPtr->SignatueCalculated)
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        retVal                   = E_OK;
    }
    else
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Check the input parameter and algorithm, compare based on duplicated addresse and update the test
 * Foreground results
 */
FLSTST_LOCAL Std_ReturnType FlsTst_DuplicatedMemoryFgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    const FlsTst_AddressType* startAddrPtr = BlockInfoPtr->BlcokStartAddrPtr;
    FlsTst_LengthType         testLength   = BlockInfoPtr->MemBlcokLength;
    /** duplicate memory address */
    const FlsTst_AddressType* dupAddrPtr = BlockInfoPtr->DuplicateAddrPtr;
    /** if test method changes,Modify the code below */
    /** FlsTst Duplicated Memory  Foreground  test */
    retVal = FlsTst_MemCompare((const uint8*)startAddrPtr, (const uint8*)dupAddrPtr, testLength);

    if (BlockInfoPtr->SignatureStored == (FlsTst_SignatureType)retVal)
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        retVal                   = E_OK;
    }
    else
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Check the input parameter and algorithm, calculate checksum and update the Foreground test results based
 * on the calculation results
 */
FLSTST_LOCAL Std_ReturnType FlsTst_ChecksumFgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    const FlsTst_AddressType* startAddrPtr    = BlockInfoPtr->BlcokStartAddrPtr;
    FlsTst_LengthType         testLength      = BlockInfoPtr->MemBlcokLength;
    FlsTst_SignatureType      signatureResult = 0u;

    /** if test method changes,Modify the code below */
    /** Use CRC8 */
    signatureResult = FlsTst_CheckSum((const uint8*)startAddrPtr, testLength, signatureResult, TRUE);

    BlockInfoPtr->SignatueCalculated = signatureResult;
    if (BlockInfoPtr->SignatureStored == BlockInfoPtr->SignatueCalculated)
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        retVal                   = E_OK;
    }
    else
    {
        BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
    }

    return retVal;
}

#if (STD_ON == FLSTST_TEST_ECC_API)
/**
 * @brief Check the input parameter and algorithm, execute ECC test and update the Foreground test results based on
 * the calculation results
 */
FLSTST_LOCAL Std_ReturnType FlsTst_ECCTestFgnd(FlsTst_BlockInfoType* BlockInfoPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    const FlsTst_AddressType* startAddrPtr = BlockInfoPtr->BlcokStartAddrPtr;
    /* PRQA S 0488 ++ */ /* VL_FlsTst_0488 */
    const FlsTst_AddressType* endAddrPtr = startAddrPtr + BlockInfoPtr->MemBlcokLength;
    /* PRQA S 0488 -- */

    if (NULL_PTR != FlsTst_Module.ECCTestFunction)
    {
        retVal = FlsTst_Module.ECCTestFunction(startAddrPtr, endAddrPtr);

        if (BlockInfoPtr->SignatureStored == (FlsTst_SignatureType)retVal)
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_OK;
        }
        else
        {
            BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
        }
    }
    else
    {
        /** Do not support Flash ECC test function */
        BlockInfoPtr->TestResult = FLSTST_RESULT_NOT_OK;
    }

    return retVal;
}
#endif
#endif

#if ((0u != FLSTST_BGND_BLOCK_NUM) || (0u != FLSTST_FGND_BLOCK_NUM))
/**
 * @brief Set the initial value of checksum, perform calculations based on test length
 */
FLSTST_LOCAL uint32 FlsTst_CheckSum(
    const uint8* Checksum_DataPtr,
    uint32       Checksum_Length,
    uint32       Checksum_StartValue,
    boolean      Checksum_IsFirstCall)
{
    uint32 checksumResult;

    if ((boolean)TRUE == Checksum_IsFirstCall)
    {
        checksumResult = 0u;
    }
    else
    {
        checksumResult = Checksum_StartValue;
    }

    for (uint32 index = 0u; index < Checksum_Length; index++)
    {
        checksumResult += Checksum_DataPtr[index];
    }
    return checksumResult;
}
#endif

#if ((0u != FLSTST_BGND_BLOCK_NUM) || (0u != FLSTST_FGND_BLOCK_NUM))
/**
 * @brief Compares the data of the source and destination addresses. If the data is inconsistent, an error is
 * reported
 */
FLSTST_LOCAL Std_ReturnType FlsTst_MemCompare(const uint8* SourceDataPtr, const uint8* DstDataPtr, uint32 Length)
{
    Std_ReturnType retVal = E_OK;

    for (uint32 index = 0u; index < Length; index++)
    {
        if (DstDataPtr[index] != SourceDataPtr[index])
        {
            retVal = E_NOT_OK;
            break;
        }
    }
    return retVal;
}
#define FLSTST_STOP_SEC_CODE
#include "FlsTst_MemMap.h"
#endif
