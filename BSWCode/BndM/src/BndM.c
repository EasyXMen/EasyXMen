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
 **  @file               : BndM.c
 **  @author             : haibin.shao
 **  @date               : 2025/05/06
 **  @vendor             : isoft
 **  @description        : Implementation for bulk non volatile Data Manager
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "BndM.h"
#include "MemAcc.h"
#include "SchM_BndM.h"
#include "BndM_Internal.h"
#include "BndM_Arti.h"

/* ===================================================== macros ===================================================== */

/* PRQA S 3472 ++ */ /* VL_BndM_3472 */
#define BndM_EnterCritical() SchM_Enter_BndM_BNDM_EXCLUSIVE_AREA_0()
#define BndM_LeaveCritical() SchM_Exit_BndM_BNDM_EXCLUSIVE_AREA_0()
/* PRQA S 3472 -- */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Get the index of given block identifier
 * @param[in]   BlockId: The identifier of block
 * @return      uint16
 * @retval      value of block index
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL uint16 BndM_GetBlockIndex(BndM_BlockIdType BlockId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Get the header block size check result of the bank
 * @param[in]   firstBankDataInfo: The memory information of first bank
 * @param[in]   secondBankDataInfo: The memory information of second bank
 * @param[out]  firstCheckResult: The block size check result of first bank
 * @param[out]  secondCheckResult: The block size check result of second bank
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_SwapHeaderBlockSizeCheck(
    BndM_BankDataInfoType firstBankDataInfo,
    BndM_BankDataInfoType secondBankDataInfo,
    boolean*              firstCheckResult,
    boolean*              secondCheckResult);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Get the information of the bank and update the bank parameter to the specified global variable
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_SwapParamUpdate(void);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       If the underlying state of the request is pengding and the underlying module is idle,
 *              set the corresponding state based on the requested task and call the finish API
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @param[in]   RequestJob: The current request job of the given block
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_MemAccReq(MemAcc_AddressAreaIdType AddressAreaId, MemAcc_JobType RequestJob);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       If the underlying state of the request is pengding and the underlying module is idle,
 *              set the corresponding state based on the requested task and call the finish API
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @return      void
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_CancelReq(MemAcc_AddressAreaIdType AddressAreaId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       According to the job result, set runtime variables and the end callback notification API is invoked
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @param[in]   JobResult: The current job result of the given block
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_JobFinish(MemAcc_JobResultType JobResult);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Release sub write-start resource.
 * @param[in]   BlockId: The identifier of block
 * @param[in]   result: The job result of given block
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteStartRelease(BndM_BlockIdType BlockId, BndM_ResultType result);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Release sub write-finalize resource.
 * @param[in]   BlockId: The identifier of block
 * @param[in]   result: The job result of given block
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteFinalizeRelease(BndM_BlockIdType BlockId, BndM_ResultType result);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Release sub write-block resource.
 * @param[in]   BlockId: The identifier of block
 * @param[in]   result: The job result of given block
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteBlockRelease(BndM_BlockIdType BlockId, BndM_ResultType result);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       According to the job result, set runtime variables and the end callback notification API is invoked
 * @param[in]   BlockId: The identifier of the given block
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteCancelFinish(BndM_BlockIdType BlockId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Request sub write-finalize task and the Request to copy data.
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteFinalizeCopyDataHandle(MemAcc_AddressAreaIdType AddressAreaId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Request sub write-finalize task and the Request to program data.
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteFinalizeProgramDataHandle(MemAcc_AddressAreaIdType AddressAreaId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Request sub write-cancel task and the request is sent to the bottom layer.
 *              If the bottom layer processing is complete, the corresponding status is set and the finish API is called
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteCancelHandle(MemAcc_AddressAreaIdType AddressAreaId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Request sub write-start task and the request is sent to the bottom layer.
 *              If the bottom layer processing is complete, the corresponding status is set and the finish API is called
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteStartHandle(MemAcc_AddressAreaIdType AddressAreaId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Request sub write-block task and the request is sent to the bottom layer.
 *              If the bottom layer processing is complete, the corresponding status is set and the finish API is called
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteBlockHandle(MemAcc_AddressAreaIdType AddressAreaId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief       Request sub write-finalize task and the request is sent to the bottom layer.
 *              If the bottom layer processing is complete, the corresponding status is set and the finish API is called
 * @param[in]   AddressAreaId: Numeric identifier of address area
 * @return      void
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
BNDM_LOCAL void BndM_WriteFinalizeHandle(MemAcc_AddressAreaIdType AddressAreaId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define BNDM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "BndM_MemMap.h"
/**
 * @brief The block Information of BndM module
 * @range NA
 */
BNDM_LOCAL BndM_BlockInfoType BndM_BlockInfo[BNDM_NUMBER_OF_USER_BLOCKS];
#define BNDM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "BndM_MemMap.h"

#define BNDM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "BndM_MemMap.h"
/**
 * @brief The bank Parameter information of BndM module
 * @range NA
 */
BNDM_LOCAL BndM_BankParamInfoType BndM_BankParamInfo;
#define BNDM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "BndM_MemMap.h"

#define BNDM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "BndM_MemMap.h"
/**
 * @brief The run-time data of BndM module
 * @range NA
 */
BNDM_LOCAL BndM_RunTimeDataType BndM_RunTimeData;
#define BNDM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "BndM_MemMap.h"

#define BNDM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "BndM_MemMap.h"
/**
 * @brief The bank data information of BndM module
 * @range NA
 */
BndM_BankDataInfoType BndM_BankDataInfo; /* PRQA S 0759 */ /* VL_BndM_0759 */
#define BNDM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "BndM_MemMap.h"

#define BNDM_START_SEC_CONST_UNSPECIFIED
#include "BndM_MemMap.h"
/**
 * @brief Function Pointer for internal jobs
 * @range NA
 */
/* PRQA S 3218,3132 ++ */ /* VL_BndM_3218,VL_QAC_MagicNum */
BNDM_LOCAL const BndM_WriteHandleFuncPtr BndM_WriteHandleFunc[5] = {
    NULL_PTR,
    BndM_WriteStartHandle,
    BndM_WriteBlockHandle,
    BndM_WriteFinalizeHandle,
    BndM_WriteCancelHandle,
};
/* PRQA S 3218,3132 -- */
#define BNDM_STOP_SEC_CONST_UNSPECIFIED
#include "BndM_MemMap.h"

/* ============================================ external data definitions =========================================== */
#define BNDM_START_SEC_VAR_CLEARED_BOOLEAN
#include "BndM_MemMap.h"
/**
 * @brief initialize status of BndM module
 * @range 0..1
 */
boolean BndM_ModuleInitStatus;
#define BNDM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "BndM_MemMap.h"

#define BNDM_START_SEC_VAR_CLEARED_PTR
#include "BndM_MemMap.h"
/**
 * @brief PB configuration pointer for the bulk non volatile Data Manager
 * @range N/A
 */
const BndM_ConfigType* BndM_ConfigPtr;
#define BNDM_STOP_SEC_VAR_CLEARED_PTR
#include "BndM_MemMap.h"

/* ========================================== external function definitions ========================================= */
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Service is used to initializes the bulk non volatile Data Manager.
 */
void BndM_Init(const BndM_ConfigType* ConfigPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Start, 2, 0x01, ConfigPtr);
#if (STD_ON == BNDM_DEV_ERROR_DETECT)
    /* Check if the incoming parameter of the function is a null pointer */
    if (NULL_PTR == ConfigPtr)
    {
        /* Used to indicate that the error report type is 'PARAM_POINTER' */
        /* Report development error to the DET module */
        BNDM_DET_REPORT((uint8)BNDM_SERVICE_ID_INIT, BNDM_E_PARAM);
    }
    else
#endif
    {
        /* Store the PB configuration */
        BndM_ConfigPtr = ConfigPtr;

        BndM_BlockInfoType*     BlockInfoPtr     = NULL_PTR;
        BndM_RunTimeDataType*   RunTimeDataPtr   = &BndM_RunTimeData;
        BndM_BankParamInfoType* BankParamInfoPtr = &BndM_BankParamInfo;

        BndM_EnterCritical();
        /* Clear the 'isFirstProgram' of the given bank param information */
        BankParamInfoPtr->isFirstProgram = FALSE;
        /* Clear the 'versionNumber' of the given bank param information */
        BankParamInfoPtr->versionNumber = 0u;
        /* Clear the 'activatedStartAddress' of the given bank param information */
        BankParamInfoPtr->activatedStartAddress = 0u;
        /* Clear the 'activatedLength' of the given bank param information */
        BankParamInfoPtr->activatedLength = 0u;
        /* Clear the 'operateStartAddress' of the given bank param information */
        BankParamInfoPtr->operateStartAddress = 0u;
        /* Clear the 'operateLength' of the given bank param information */
        BankParamInfoPtr->operateLength = 0u;
        /* Clear the "CancelDone" of the runtime variable data */
        RunTimeDataPtr->CancelDone = FALSE;
        /* Clear the "RequestJob" of the runtime variable data */
        RunTimeDataPtr->RequestJob = MEMACC_NO_JOB;
        /* Set the "JobStatus" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->JobStatus = MEMACC_JOB_IDLE;
        /* Set the "SubJobStatus" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_IDLE;
        /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
        /* Set the "CancelState" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->CancelState = BNDM_CANCEL_JOB_WRITE_IDLE;
        /* Set the "WriteJobSequence" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_IDLE;
        /* Clear the "BlockId" of the runtime variable data */
        RunTimeDataPtr->BlockId = 0u;
        /* Clear the "OperateAddress" of the runtime variable data */
        RunTimeDataPtr->OperateAddress = 0u;
        /* Clear the "OperateSize" of the runtime variable data */
        RunTimeDataPtr->OperateSize = 0u;
        /* Clear the "OperateBufferPtr" of the runtime variable data */
        RunTimeDataPtr->OperateBufferPtr = NULL_PTR;
        for (uint16 iloop = 0u; iloop < (uint16)BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
        {
            BlockInfoPtr = &(BndM_BlockInfo[iloop]);
            /* Clear the "isUpdated" of the block information */
            BlockInfoPtr->isUpdated = FALSE;
            /* Clear the "IsOperated" of the block information */
            BlockInfoPtr->IsOperated = FALSE;
            /* Set the "BlockVaild" of the block information to 'INVALID' */
            BlockInfoPtr->BlockVaild = BNDM_BLOCK_INVALID;
            /* Clear the "BlockId" of the block information */
            BlockInfoPtr->BlockId = iloop;
            /* Clear the "BlockAddress" of the block information */
            BlockInfoPtr->BlockAddress = 0u;
            /* Clear the "BlockLength" of the block information */
            BlockInfoPtr->BlockLength = 0u;
        }
        /* Clear the "buffer" of the bank data information */
        BndM_MemSet(BndM_BankDataInfo.Data, 0x00u, BNDM_BANK_HEAD_INFO_SIZE);
        /* Set the status of the BndM module to initialized */
        BndM_ModuleInitStatus = TRUE;
        BndM_LeaveCritical();
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Return, 1, 0x01);
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#if (BNDM_VERSION_INFO_API == STD_ON)
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Service is used to get the version information of BndM module.
 */
void BndM_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Start, 2, 0x02, versioninfo);
#if (STD_ON == BNDM_DEV_ERROR_DETECT)
    /** Check if the incoming parameter of the function is a null pointer */
    if (NULL_PTR == versioninfo)
    {
        /** Report development error to the DET module */
        BNDM_DET_REPORT(BNDM_SERVICE_ID_GETVERSIONINFO, BNDM_E_PARAM);
    }
    else
#endif
    {
        versioninfo->vendorID         = BNDM_VENDOR_ID;
        versioninfo->moduleID         = BNDM_MODULE_ID;
        versioninfo->sw_major_version = BNDM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = BNDM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = BNDM_SW_PATCH_VERSION;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Return, 1, 0x02);
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#endif
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Service is used to will trigger the start of writing phase.
 *        The finish of asynchronous processing will trigger the callback xxx_BndMWriteStartFinish
 *        including the result of this operation.
 */
Std_ReturnType BndM_WriteStart(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Start, 1, 0x04);
    Std_ReturnType retVal = E_NOT_OK;
    /* A call of BndM_WriteStart shall trigger the preparation of the 2nd partition. */
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
#if (STD_ON == BNDM_DEV_ERROR_DETECT)
    /** Check if the status of the BndM module is de-initialized */
    if ((boolean)FALSE == BndM_ModuleInitStatus)
    {
        /** Used to indicate that the error report type is 'WRONG_SEQUENCE' */
        /* Report development error to the DET module */
        BNDM_DET_REPORT((uint8)BNDM_SERVICE_ID_WRITESTART, BNDM_E_WRONG_SEQUENCE);
    }
    /* Check if the written job sequence is not in the IDLE stage */
    else if (
        ((MEMACC_JOB_IDLE == RunTimeDataPtr->JobStatus)
         && (BNDM_JOB_SEQ_WRITE_IDLE != RunTimeDataPtr->WriteJobSequence))
        || ((MEMACC_JOB_PENDING == RunTimeDataPtr->JobStatus)
            && ((BNDM_JOB_SEQ_WRITE_BLOCK == RunTimeDataPtr->WriteJobSequence)
                || (BNDM_JOB_SEQ_WRITE_FINALIZE == RunTimeDataPtr->WriteJobSequence))))
    {
        /* A call of BndM_WriteStart shall be rejected with the errorCode E_NOT_OK,
         * if the call is done within an active writing phase. */
        /** Used to indicate that the error report type is 'WRONG_SEQUENCE' */
        /* Report development error to the DET module */
        BNDM_DET_REPORT((uint8)BNDM_SERVICE_ID_WRITESTART, BNDM_E_WRONG_SEQUENCE);
    }
    else
#endif
    {
        const BndM_BankParamInfoType* BankParamInfoPtr = &BndM_BankParamInfo;
        /* Check if the status of the write job is 'PENDING' */
        if (((boolean)TRUE == RunTimeDataPtr->CancelDone)
            || ((MEMACC_JOB_PENDING == RunTimeDataPtr->JobStatus)
                && (BNDM_JOB_SEQ_WRITE_START == RunTimeDataPtr->WriteJobSequence)))
        {
            /* Indicate that the current job processing is in the 'BUSY' state */
        }
        else
        {
            BndM_BlockInfoType* BlockInfoPtr = NULL_PTR;
            for (uint16 iloop = 0u; iloop < (uint16)BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
            {
                BlockInfoPtr = &(BndM_BlockInfo[iloop]);
                /* Clear the "isUpdated" of the block information */
                BlockInfoPtr->isUpdated = FALSE;
                /* Clear the "IsOperated" of the block information */
                BlockInfoPtr->IsOperated = FALSE;
                /* Set the "BlockVaild" of the block information to 'INVALID' */
                BlockInfoPtr->BlockVaild = BNDM_BLOCK_INVALID;
                /* Clear the "BlockId" of the block information */
                BlockInfoPtr->BlockId = iloop;
                /* Clear the "BlockAddress" of the block information */
                BlockInfoPtr->BlockAddress = 0u;
                /* Clear the "BlockLength" of the block information */
                BlockInfoPtr->BlockLength = 0u;
            }
            /* Update the current activation status information of bank A/B */
            BndM_SwapParamUpdate();
            BndM_EnterCritical();
            /* Set the "RequestJob" of the runtime variable data to 'ERASE' */
            RunTimeDataPtr->RequestJob = MEMACC_ERASE_JOB;
            /* Set the "JobStatus" of the runtime variable data to 'PENDING' */
            RunTimeDataPtr->JobStatus = MEMACC_JOB_PENDING;
            /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
            RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
            /* Set the "WriteJobSequence" of the runtime variable data to 'START' */
            RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_START;
            /* Set the "OperateAddress" of the runtime variable data */
            RunTimeDataPtr->OperateAddress = BankParamInfoPtr->operateStartAddress;
            /* Set the "OperateSize" of the runtime variable data */
            RunTimeDataPtr->OperateSize = BankParamInfoPtr->operateLength;
            BndM_LeaveCritical();
            retVal = E_OK;
        }
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Return, 2, 0x04, retVal);
    return retVal;
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Service is used to will trigger the finalization of writing phase.
 *        The finish of asynchronous processing will trigger the callback xxx_BndMWriteFinalizeFinish
 *        including the result of this operation.
 */
Std_ReturnType BndM_WriteFinalize(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Start, 1, 0x06);
    Std_ReturnType        retVal         = E_NOT_OK;
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
#if (STD_ON == BNDM_DEV_ERROR_DETECT)
    /** Check if the status of the BndM module is de-initialized */
    if ((boolean)FALSE == BndM_ModuleInitStatus)
    {
        /** Used to indicate that the error report type is 'WRONG_SEQUENCE' */
        /* Report development error to the DET module */
        BNDM_DET_REPORT((uint8)BNDM_SERVICE_ID_WRITEFINALIZE, BNDM_E_WRONG_SEQUENCE);
    }
    /** Check if the sequence of the write job is not 'WRITE_BLOCK' */
    else if (
        ((MEMACC_JOB_IDLE == RunTimeDataPtr->JobStatus)
         && (BNDM_JOB_SEQ_WRITE_BLOCK != RunTimeDataPtr->WriteJobSequence))
        || ((MEMACC_JOB_PENDING == RunTimeDataPtr->JobStatus)
            && ((BNDM_JOB_SEQ_WRITE_START == RunTimeDataPtr->WriteJobSequence)
                || (BNDM_JOB_SEQ_WRITE_BLOCK == RunTimeDataPtr->WriteJobSequence))))
    {
        /* A call of BndM_WriteFinalize without a previously called BndM_WriteStart or
         * within the finalization mode of the BndM the DET BNDM_E_WRONG_SEQUENCE error shall be thrown. */
        /** Used to indicate that the error report type is 'WRONG_SEQUENCE' */
        /* Report development error to the DET module */
        BNDM_DET_REPORT((uint8)BNDM_SERVICE_ID_WRITEFINALIZE, BNDM_E_WRONG_SEQUENCE);
    }
    else
#endif
    {
        if (((boolean)TRUE == RunTimeDataPtr->CancelDone)
            || ((MEMACC_JOB_PENDING == RunTimeDataPtr->JobStatus)
                && (BNDM_JOB_SEQ_WRITE_FINALIZE == RunTimeDataPtr->WriteJobSequence)))
        {
            /* Indicate that the current job processing is in the 'BUSY' state */
        }
        else
        {
            /** If the finalization is NOT successful the BndM shall keep the current active partition
             * as the active partition and trigger the callback Xxx_BndMWriteFinalizeFinish
             * with the result set to E_NOT_OK. */
            BndM_EnterCritical();
            /* Set the "RequestJob" of the runtime variable data to 'WRITE' */
            RunTimeDataPtr->RequestJob = MEMACC_WRITE_JOB;
            /* Set the "JobStatus" of the runtime variable data to 'PENDING' */
            RunTimeDataPtr->JobStatus = MEMACC_JOB_PENDING;
            /* Set the "SubJobStatus" of the runtime variable data to 'IDLE' */
            RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_IDLE;
            /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
            RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
            /* Set the "WriteJobSequence" of the runtime variable data to 'FINALIZE' */
            RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_FINALIZE;
            BndM_LeaveCritical();
            retVal = E_OK;
        }
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Return, 2, 0x06, retVal);
    return retVal;
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to will trigger the cancel of writing phase.
 */
void BndM_WriteCancel(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Start, 1, 0x07);
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
#if (STD_ON == BNDM_DEV_ERROR_DETECT)
    /* Check if the status of the BndM module is initialized */
    if ((boolean)FALSE == BndM_ModuleInitStatus)
    {
        /** Used to indicate that the error report type is 'WRONG_SEQUENCE' */
        /* Report development error to the DET module */
        BNDM_DET_REPORT((uint8)BNDM_SERVICE_ID_WRITECANCEL, BNDM_E_WRONG_SEQUENCE);
    }
    else if (
        (MEMACC_JOB_IDLE == RunTimeDataPtr->JobStatus) && (BNDM_JOB_SEQ_WRITE_IDLE == RunTimeDataPtr->WriteJobSequence))
    {
        /** Used to indicate that the error report type is 'WRONG_SEQUENCE' */
        /* Report development error to the DET module */
        BNDM_DET_REPORT((uint8)BNDM_SERVICE_ID_WRITECANCEL, BNDM_E_WRONG_SEQUENCE);
    }
    else
#endif
    {
        BndM_EnterCritical();
        /* Set the "CancelDone" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->CancelDone = TRUE;
        /* Set the "WriteJobSequence" of the runtime variable data to 'CANCEL' */
        RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_CANCEL;
        /* Set the "CancelState" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->CancelState = BNDM_CANCEL_JOB_WRITE_IDLE;
        /* Set the "JobStatus" of the runtime variable data to 'PENDING' */
        RunTimeDataPtr->JobStatus = MEMACC_JOB_PENDING;
        BndM_LeaveCritical();
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Return, 1, 0x07);
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        This callback function is called when a MemAcc job has been finished with positive result.
 */
void BndM_JobEndNotification(MemAcc_AddressAreaIdType addressAreaId, MemAcc_JobResultType jobResult)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Start, 3, 0x08, addressAreaId, jobResult);
#if (STD_OFF == BNDM_POLLING_MODE)
    /* Check if the status of the BndM module is initialized */
    if ((boolean)TRUE == BndM_ModuleInitStatus)
    {
        /* Check if the status of the write job is 'PENDING' */
        if (MEMACC_JOB_PENDING == jobResult)
        {
            BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
            /* The transaction handle given write job sequence */
            (*BndM_WriteHandleFunc[RunTimeDataPtr->WriteJobSequence])(addressAreaId);
        }
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Return, 1, 0x08);
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Schedule function for the background processing.
 */
void BndM_MainFunction(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Start, 1, 0x55);
    /* Check if the status of the BndM module is initialized */
    if ((boolean)TRUE == BndM_ModuleInitStatus)
    {
#if (STD_ON == BNDM_POLLING_MODE)
        const BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
        /* Check if the status of the write job is 'PENDING' */
        if (MEMACC_JOB_PENDING == RunTimeDataPtr->JobStatus)
        {
            MemAcc_AddressAreaIdType AddressAreaId = BndM_ConfigPtr->BankParamListPtr->AddressAreaId;
            /* The transaction handle given write job sequence */
            (*BndM_WriteHandleFunc[RunTimeDataPtr->WriteJobSequence])(AddressAreaId);
        }
#endif
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, BndM, 0, Bsw_Return, 1, 0x55);
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to will persist the data in flash,
 *               that it can later directly accessed via BndM_GetBlockPtr API.
 *               The writing take a while and is finished after
 *               the successful callback xxx_BndMWriteBlockFinish.
 */
Std_ReturnType BndM_WriteBlockRequest(const BndM_BlockIdType BlockId, const MemAcc_DataType* WriteBufferPtr)
{
    Std_ReturnType        retVal         = E_NOT_OK;
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
#if (STD_ON == BNDM_DEV_ERROR_DETECT)
    /* Check if the sequence of the given write job is 'WRITE_IDLE' or 'WRITE_FINALIZE' */
    if (((MEMACC_JOB_IDLE == RunTimeDataPtr->JobStatus)
         && ((BNDM_JOB_SEQ_WRITE_IDLE == RunTimeDataPtr->WriteJobSequence)
             || (BNDM_JOB_SEQ_WRITE_FINALIZE == RunTimeDataPtr->WriteJobSequence)))
        || ((MEMACC_JOB_PENDING == RunTimeDataPtr->JobStatus)
            && ((BNDM_JOB_SEQ_WRITE_START == RunTimeDataPtr->WriteJobSequence)
                || (BNDM_JOB_SEQ_WRITE_FINALIZE == RunTimeDataPtr->WriteJobSequence))))
    {
        /* A call of BndM_WriteBlock shall be rejected with the errorCode E_NOT_OK, if the call is done without a
         * previous call of BndM_WriteStart. or while another writing of the same or another block is ongoing or the
         * call is done within the finalization mode of the BndM. */
        /* A call of BndM_WriteBlock shall be rejected with the errorCode E_NOT_OK, if the call is done while another
         * writing of the same or another block is ongoing. */
        /* A call of BndM_WriteBlock shall be rejected with the errorCode E_NOT_OK, if the call is done within or after
         * the finalization mode of the BndM. */
        BNDM_DET_REPORT(BNDM_SERVICE_ID_WRITEBLOCK, BNDM_E_WRONG_SEQUENCE);
    }
    else
#endif
    {
        uint16 blockIndex = BndM_GetBlockIndex(BlockId);
        if ((RunTimeDataPtr->JobStatus == MEMACC_JOB_PENDING)
            && (BNDM_JOB_SEQ_WRITE_BLOCK == RunTimeDataPtr->WriteJobSequence))
        {
            /* Indicate that the current job processing is in the 'BUSY' state */
        }
        else
        {
            if ((boolean)TRUE == BndM_BlockInfo[blockIndex].IsOperated)
            {
                BndM_EnterCritical();
                const BndM_BlockDescriptorType* BlockDescriptorPtr =
                    &(BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[BlockId]);
                MemAcc_AddressType OffsetAddress = BlockDescriptorPtr->BlockOffsetAddress;
                MemAcc_LengthType  WriteLength   = BlockDescriptorPtr->BlockSize;
                /* Get the 'operateStartAddress' and 'OffsetAddress' of the given bank param information */
                MemAcc_AddressType WriteAddress = BndM_BankParamInfo.operateStartAddress + OffsetAddress;
                /* Set the "BlockId" of the runtime variable data */
                RunTimeDataPtr->BlockId = BlockId;
                /* Set the "JobStatus" of the runtime variable data to 'PENDING' */
                RunTimeDataPtr->JobStatus = MEMACC_JOB_PENDING;
                /* Set the "WriteJobSequence" of the runtime variable data to 'BLOCK' */
                RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_BLOCK;
                /* Set the "RequestJob" of the runtime variable data to 'WRITE' */
                RunTimeDataPtr->RequestJob = MEMACC_WRITE_JOB;
                /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
                RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
                /* Set the "OperateAddress" of the runtime variable data */
                RunTimeDataPtr->OperateAddress = WriteAddress;
                /* Set the "OperateSize" of the runtime variable data */
                RunTimeDataPtr->OperateSize = WriteLength;
                /* Set the "OperateBufferPtr" of the runtime variable data */
                /* PRQA S 0311 ++ */ /* VL_BndM_0311 */
                RunTimeDataPtr->OperateBufferPtr = (MemAcc_DataType*)WriteBufferPtr;
                /* PRQA S 0311 -- */
                BndM_LeaveCritical();
                retVal = E_OK;
            }
        }
    }
    return retVal;
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to get the parameter information pointer for the bank.
 */
BndM_BankParamInfoType* BndM_GetBankParamInfoPtr(void)
{
    BndM_BankParamInfoType* BankParamInfoPtr = &BndM_BankParamInfo;
    return BankParamInfoPtr;
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to get the parameter information pointer for the block.
 */
BndM_BlockInfoType* BndM_GetBlockInfoPtr(const BndM_BlockIdType BlockId)
{
    BndM_BlockInfoType* BlockInfoPtr = &BndM_BlockInfo[BlockId];
    return BlockInfoPtr;
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

/* ========================================== internal function definitions ========================================= */
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Get the index of given block identifier
 */
BNDM_LOCAL uint16 BndM_GetBlockIndex(BndM_BlockIdType BlockId)
{
    uint16 retVal = BNDM_BLOCK_NUM_INVALID;
    if (BlockId == BNDM_NUMBER_OF_USER_BLOCKS)
    {
        retVal = BNDM_NUMBER_OF_USER_BLOCKS;
    }
    else
    {
        for (uint16 iloop = 0u; iloop < BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorNbr; iloop++)
        {
            if (BlockId == BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[iloop].BlockId)
            {
                retVal = iloop;
                break;
            }
        }
    }
    return retVal;
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Get the header block size check result of the bank
 */
BNDM_LOCAL void BndM_SwapHeaderBlockSizeCheck(
    BndM_BankDataInfoType firstBankDataInfo,
    BndM_BankDataInfoType secondBankDataInfo,
    boolean*              firstCheckResult,
    boolean*              secondCheckResult)
{
    MemAcc_LengthType               BlockSize[BNDM_NUMBER_OF_USER_BLOCKS];
    const BndM_BlockDescriptorType* BlockDescriptorPtr = NULL_PTR;
    *firstCheckResult                                  = TRUE;
    *secondCheckResult                                 = TRUE;
    for (uint16 iloop = 0u; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
    {
        BlockDescriptorPtr = &(BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[iloop]);
        BlockSize[iloop]   = BlockDescriptorPtr->BlockSize;
    }
    for (uint16 iloop = 0u; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
    {
        if (BlockSize[iloop] != firstBankDataInfo.BankDataList.BlockSize[iloop])
        {
            *firstCheckResult = FALSE;
            break;
        }
    }
    for (uint16 iloop = 0u; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
    {
        if (BlockSize[iloop] != secondBankDataInfo.BankDataList.BlockSize[iloop])
        {
            *secondCheckResult = FALSE;
            break;
        }
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Get the information of the bank and update the bank parameter to the specified global variable
 */
/* PRQA S 0488,0309,0306 ++ */ /* VL_BndM_0488,VL_BndM_0309,VL_BndM_0306 */
BNDM_LOCAL void BndM_SwapParamUpdate(void)
{
    boolean                  firstBankCheckResult;
    boolean                  secondBankCheckResult;
    BndM_BankDataInfoType    firstBankDataInfo;
    BndM_BankDataInfoType    secondBankDataInfo;
    BndM_MemEraseInfoType    memEraseInfo;
    MemAcc_MemoryInfoType    firstBankMemoryInfo;
    MemAcc_MemoryInfoType    secondBankMemoryInfo;
    MemAcc_AddressAreaIdType AddressAreaId         = BndM_ConfigPtr->BankParamListPtr->AddressAreaId;
    MemAcc_LengthType        BankTotalSize         = BndM_ConfigPtr->BankParamListPtr->TotalSizeOfPerBank;
    MemAcc_AddressType       bankFirstBaseAddress  = BndM_ConfigPtr->BankParamListPtr->BankFirstBaseAddress;
    MemAcc_AddressType       bankSecondBaseAddress = BndM_ConfigPtr->BankParamListPtr->BankSecondBaseAddress;
    (void)MemAcc_GetMemoryInfo(AddressAreaId, bankFirstBaseAddress, &firstBankMemoryInfo);
    (void)MemAcc_GetMemoryInfo(AddressAreaId, bankSecondBaseAddress, &secondBankMemoryInfo);
    BndM_MemCpy(
        (MemAcc_DataType*)(&firstBankDataInfo.Data[0]),
        (const MemAcc_DataType*)(firstBankMemoryInfo.PhysicalStartAddress),
        BNDM_BANK_HEAD_INFO_SIZE);
    BndM_MemCpy(
        (MemAcc_DataType*)(&secondBankDataInfo.Data[0]),
        (const MemAcc_DataType*)(secondBankMemoryInfo.PhysicalStartAddress),
        BNDM_BANK_HEAD_INFO_SIZE);
    BndM_MemSet(
        (MemAcc_DataType*)(&memEraseInfo.Data[0]),
        (MemAcc_DataType)BNDM_ERASE_VALUE,
        BNDM_MEM_ERASE_VALUE_INFO);
    BndM_BankParamInfoType*         BankParamInfoPtr   = &BndM_BankParamInfo;
    const BndM_BlockHeaderInfoType* BlockHeaderInfoPtr = BndM_ConfigPtr->BlockConfigurationPtr->BlockHeaderInfoPtr;
    BndM_SwapHeaderBlockSizeCheck(firstBankDataInfo, secondBankDataInfo, &firstBankCheckResult, &secondBankCheckResult);
    /* Check if both swap banks are invalid */
    if (((firstBankDataInfo.BankDataList.ActivateCode != BlockHeaderInfoPtr->ActivateCode)
         && (secondBankDataInfo.BankDataList.ActivateCode != BlockHeaderInfoPtr->ActivateCode))
        || ((firstBankDataInfo.BankDataList.ActivateCode == memEraseInfo.EraseValue)
            && (secondBankDataInfo.BankDataList.ActivateCode == memEraseInfo.EraseValue)))
    {
        BndM_EnterCritical();
        /* Set the 'isFirstProgram' of the given bank param information */
        BankParamInfoPtr->isFirstProgram = TRUE;
        /* Set the 'versionNumber' of the given bank param information */
        BankParamInfoPtr->versionNumber = 0x01u;
        /* Set the 'operateStartAddress' of the given bank param information */
        BankParamInfoPtr->operateStartAddress = (MemAcc_AddressType)firstBankMemoryInfo.LogiStartAddr;
        /* Set the 'operateLength' of the given bank param information */
        BankParamInfoPtr->operateLength = BankTotalSize;
        BndM_LeaveCritical();
    }
    /* Check if both swap banks are valid */
    else if (
        (firstBankDataInfo.BankDataList.ActivateCode == BlockHeaderInfoPtr->ActivateCode)
        && (secondBankDataInfo.BankDataList.ActivateCode == BlockHeaderInfoPtr->ActivateCode))
    {
        if ((BNDM_NUMBER_OF_USER_BLOCKS == firstBankDataInfo.BankDataList.BlockTotalNumber)
            && (BNDM_NUMBER_OF_USER_BLOCKS == secondBankDataInfo.BankDataList.BlockTotalNumber)
            && (TRUE == firstBankCheckResult) && (TRUE == secondBankCheckResult))
        {
            /* Check if the version number of the first swap bank is the highest */
            if (firstBankDataInfo.BankDataList.VersionNumber > secondBankDataInfo.BankDataList.VersionNumber)
            {
                BndM_EnterCritical();
                /* Clear the 'isFirstProgram' of the given bank param information */
                BankParamInfoPtr->isFirstProgram = FALSE;
                /* Set the 'versionNumber' of the given bank param information */
                BankParamInfoPtr->versionNumber = firstBankDataInfo.BankDataList.VersionNumber;
                /* Set the 'activatedStartAddress' of the given bank param information */
                BankParamInfoPtr->activatedStartAddress = firstBankMemoryInfo.LogiStartAddr;
                /* Set the 'operateStartAddress' of the given bank param information */
                BankParamInfoPtr->operateStartAddress = (MemAcc_AddressType)secondBankMemoryInfo.LogiStartAddr;
                /* Set the 'operateLength' of the given bank param information */
                BankParamInfoPtr->operateLength = BankTotalSize;
                BndM_LeaveCritical();
            }
            else
            {
                BndM_EnterCritical();
                /* Clear the 'isFirstProgram' of the given bank param information */
                BankParamInfoPtr->isFirstProgram = FALSE;
                /* Set the 'versionNumber' of the given bank param information */
                BankParamInfoPtr->versionNumber = secondBankDataInfo.BankDataList.VersionNumber;
                /* Set the 'activatedStartAddress' of the given bank param information */
                BankParamInfoPtr->activatedStartAddress = secondBankMemoryInfo.LogiStartAddr;
                /* Set the 'operateStartAddress' of the given bank param information */
                BankParamInfoPtr->operateStartAddress = (MemAcc_AddressType)firstBankMemoryInfo.LogiStartAddr;
                /* Set the 'operateLength' of the given bank param information */
                BankParamInfoPtr->operateLength = BankTotalSize;
                BndM_LeaveCritical();
            }
        }
        else
        {
            BndM_EnterCritical();
            /* Set the 'isFirstProgram' of the given bank param information */
            BankParamInfoPtr->isFirstProgram = TRUE;
            /* Set the 'versionNumber' of the given bank param information */
            BankParamInfoPtr->versionNumber = 0x01u;
            /* Set the 'operateStartAddress' of the given bank param information */
            BankParamInfoPtr->operateStartAddress = (MemAcc_AddressType)firstBankMemoryInfo.LogiStartAddr;
            /* Set the 'operateLength' of the given bank param information */
            BankParamInfoPtr->operateLength = BankTotalSize;
            BndM_LeaveCritical();
        }
    }
    /* Check if one of the swap banks is valid */
    else
    {
        if ((firstBankDataInfo.BankDataList.ActivateCode == BlockHeaderInfoPtr->ActivateCode)
            && (secondBankDataInfo.BankDataList.ActivateCode == memEraseInfo.EraseValue)
            && (BNDM_NUMBER_OF_USER_BLOCKS == firstBankDataInfo.BankDataList.BlockTotalNumber)
            && (TRUE == firstBankCheckResult))
        {
            BndM_EnterCritical();
            /* Clear the 'isFirstProgram' of the given bank param information */
            BankParamInfoPtr->isFirstProgram = FALSE;
            /* Set the 'versionNumber' of the given bank param information */
            BankParamInfoPtr->versionNumber = firstBankDataInfo.BankDataList.VersionNumber;
            /* Set the 'activatedStartAddress' of the given bank param information */
            BankParamInfoPtr->activatedStartAddress = firstBankMemoryInfo.LogiStartAddr;
            /* Set the 'operateStartAddress' of the given bank param information */
            BankParamInfoPtr->operateStartAddress = (MemAcc_AddressType)secondBankMemoryInfo.LogiStartAddr;
            /* Set the 'operateLength' of the given bank param information */
            BankParamInfoPtr->operateLength = BankTotalSize;
            BndM_LeaveCritical();
        }
        else if (
            (firstBankDataInfo.BankDataList.ActivateCode == memEraseInfo.EraseValue)
            && (secondBankDataInfo.BankDataList.ActivateCode == BlockHeaderInfoPtr->ActivateCode)
            && (BNDM_NUMBER_OF_USER_BLOCKS == secondBankDataInfo.BankDataList.BlockTotalNumber)
            && (TRUE == secondBankCheckResult))
        {
            BndM_EnterCritical();
            /* Clear the 'isFirstProgram' of the given bank param information */
            BankParamInfoPtr->isFirstProgram = FALSE;
            /* Set the 'versionNumber' of the given bank param information */
            BankParamInfoPtr->versionNumber = secondBankDataInfo.BankDataList.VersionNumber;
            /* Set the 'activatedStartAddress' of the given bank param information */
            BankParamInfoPtr->activatedStartAddress = secondBankMemoryInfo.LogiStartAddr;
            /* Set the 'operateStartAddress' of the given bank param information */
            BankParamInfoPtr->operateStartAddress = (MemAcc_AddressType)firstBankMemoryInfo.LogiStartAddr;
            /* Set the 'operateLength' of the given bank param information */
            BankParamInfoPtr->operateLength = BankTotalSize;
            BndM_LeaveCritical();
        }
        else
        {
            if ((BNDM_NUMBER_OF_USER_BLOCKS == firstBankDataInfo.BankDataList.BlockTotalNumber)
                && (TRUE == firstBankCheckResult))
            {
                BndM_EnterCritical();
                /* Clear the 'isFirstProgram' of the given bank param information */
                BankParamInfoPtr->isFirstProgram = FALSE;
                /* Set the 'versionNumber' of the given bank param information */
                BankParamInfoPtr->versionNumber = firstBankDataInfo.BankDataList.VersionNumber;
                /* Set the 'activatedStartAddress' of the given bank param information */
                BankParamInfoPtr->activatedStartAddress = firstBankMemoryInfo.LogiStartAddr;
                /* Set the 'operateStartAddress' of the given bank param information */
                BankParamInfoPtr->operateStartAddress = (MemAcc_AddressType)secondBankMemoryInfo.LogiStartAddr;
                /* Set the 'operateLength' of the given bank param information */
                BankParamInfoPtr->operateLength = BankTotalSize;
                BndM_LeaveCritical();
            }
            else
            {
                BndM_EnterCritical();
                /* Set the 'isFirstProgram' of the given bank param information */
                BankParamInfoPtr->isFirstProgram = TRUE;
                /* Set the 'versionNumber' of the given bank param information */
                BankParamInfoPtr->versionNumber = 0x01u;
                /* Set the 'operateStartAddress' of the given bank param information */
                BankParamInfoPtr->operateStartAddress = (MemAcc_AddressType)firstBankMemoryInfo.LogiStartAddr;
                /* Set the 'operateLength' of the given bank param information */
                BankParamInfoPtr->operateLength = BankTotalSize;
                BndM_LeaveCritical();
            }
        }
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/* PRQA S 0488,0309,0306 -- */

/**
 * @brief If the underlying state of the request is pengding and the underlying module is idle,
 *        set the corresponding state based on the requested task and call the finish API
 */
BNDM_LOCAL void BndM_MemAccReq(MemAcc_AddressAreaIdType AddressAreaId, MemAcc_JobType RequestJob)
{
    Std_ReturnType         retVal         = E_OK;
    const MemAcc_DataType* dataBufferPtr  = NULL_PTR;
    BndM_RunTimeDataType*  RunTimeDataPtr = &BndM_RunTimeData;
    MemAcc_LengthType      dealLength     = RunTimeDataPtr->OperateSize;
    MemAcc_AddressType     dealAddress    = RunTimeDataPtr->OperateAddress;
    /* Check if state of the 'MemAcc' is write ready and the job status is 'IDLE' */
    if ((BNDM_MEMACC_JOB_ASYNC_WRITE_READY == RunTimeDataPtr->MemAccState)
        && (MEMACC_JOB_IDLE == MemAcc_GetJobStatus(AddressAreaId)))
    {
        switch (RequestJob)
        {
        case MEMACC_WRITE_JOB:
            dataBufferPtr = RunTimeDataPtr->OperateBufferPtr;
            /* Invoke the "Write" function of the underlying memory access module
             * selected by the parameter 'AddressAreaId'. */
            retVal = MemAcc_Write(AddressAreaId, dealAddress, dataBufferPtr, dealLength);
            /* Check if the function call is successful */
            if ((Std_ReturnType)E_OK == retVal)
            {
                BndM_EnterCritical();
                /* Set the "MemAccState" of the runtime variable data to 'PENDING' */
                RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_ASYNC_WRITE_PENDING;
                BndM_LeaveCritical();
            }
            else
            {
                /* Finish the job assignment and release the resources of the job */
                BndM_JobFinish(MEMACC_FAILED);
            }
            break;
        case MEMACC_ERASE_JOB:
            /* Invoke the "Erase" function of the underlying memory access module
             * selected by the parameter 'AddressAreaId'. */
            retVal = MemAcc_Erase(AddressAreaId, dealAddress, dealLength);
            /* Check if the function call is successful */
            if ((Std_ReturnType)E_OK == retVal)
            {
                BndM_EnterCritical();
                /* Set the "MemAccState" of the runtime variable data to 'PENDING' */
                RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_ASYNC_WRITE_PENDING;
                BndM_LeaveCritical();
            }
            else
            {
                /* Finish the job assignment and release the resources of the job */
                BndM_JobFinish(MEMACC_FAILED);
            }
            break;
        default:
            /** idle */
            break;
        }
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief If the underlying state of the request is pengding and the underlying module is idle,
 *        set the corresponding state based on the requested task and call the finish API
 */
BNDM_LOCAL void BndM_CancelReq(MemAcc_AddressAreaIdType AddressAreaId)
{
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Check if state of the 'MemAcc' is write ready and the job status is 'IDLE' */
    if (BNDM_CANCEL_JOB_ASYNC_WRITE_READY == RunTimeDataPtr->CancelState)
    {
        /* Invoke the "Cancel" function of the underlying memory access module
         * selected by the parameter 'AddressAreaId'. */
        MemAcc_Cancel(AddressAreaId);
        BndM_EnterCritical();
        /* Set the "CancelState" of the runtime variable data to 'PENDING' */
        RunTimeDataPtr->CancelState = BNDM_CANCEL_JOB_ASYNC_WRITE_PENDING;
        BndM_LeaveCritical();
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief According to the job result, set runtime variables and the end callback notification API is invoked
 */
BNDM_LOCAL void BndM_JobFinish(MemAcc_JobResultType JobResult)
{
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    BndM_BlockIdType      BlockId        = RunTimeDataPtr->BlockId;
    if (MEMACC_OK == JobResult)
    {
        /* Check if the sequence of the given write job is 'WRITE_START' */
        if (BNDM_JOB_SEQ_WRITE_START == RunTimeDataPtr->WriteJobSequence)
        {
            for (BndM_BlockIdType iloop = 0u; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
            {
                /* Report the finish result of the write start to the upper layer */
                BndM_ConfigPtr->WriteStartFinish(iloop, E_OK);
                /* After the write start is finished, the request resource will be release */
                BndM_WriteStartRelease(iloop, E_OK);
            }
        }
        /* Check if the sequence of the given write job is 'WRITE_BLOCK' */
        else if (BNDM_JOB_SEQ_WRITE_BLOCK == RunTimeDataPtr->WriteJobSequence)
        {
            const BndM_BlockDescriptorType* BlockDescriptorPtr =
                &(BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[BlockId]);
            /* Report the finish result of the write block to the upper layer */
            BlockDescriptorPtr->WriteBlockFinish(RunTimeDataPtr->BlockId, E_OK);
            /* After the write block is finished, the request resource will be release */
            BndM_WriteBlockRelease(RunTimeDataPtr->BlockId, E_OK);
        }
        /* Check if the sequence of the given write job is 'WRITE_FINALIZE' */
        else if (BNDM_JOB_SEQ_WRITE_FINALIZE == RunTimeDataPtr->WriteJobSequence)
        {
            for (BndM_BlockIdType iloop = 0u; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
            {
                /* Report the finish result of the write finalize to the upper layer */
                BndM_ConfigPtr->WriteFinalizeFinish(iloop, E_OK);
                /* After the write finalize is finished, the request resource will be release */
                BndM_WriteFinalizeRelease(iloop, E_OK);
            }
        }
        else
        {
            /** do nothing */
        }
    }
    else if (
        (MEMACC_FAILED == JobResult) || (MEMACC_INCONSISTENT == JobResult) || (MEMACC_ECC_UNCORRECTED == JobResult)
        || (MEMACC_ECC_CORRECTED == JobResult))
    {
        /* Check if the sequence of the given write job is 'WRITE_START' */
        if (BNDM_JOB_SEQ_WRITE_START == RunTimeDataPtr->WriteJobSequence)
        {
            for (BndM_BlockIdType iloop = 0u; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
            {
                /* Report the finish result of the write start to the upper layer */
                BndM_ConfigPtr->WriteStartFinish(iloop, E_NOT_OK);
                /* After the write start is failed, the request resource will be release */
                BndM_WriteStartRelease(iloop, E_NOT_OK);
            }
        }
        /* Check if the sequence of the given write job is 'WRITE_BLOCK' */
        else if (BNDM_JOB_SEQ_WRITE_BLOCK == RunTimeDataPtr->WriteJobSequence)
        {
            const BndM_BlockDescriptorType* BlockDescriptorPtr =
                &(BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[BlockId]);
            /* Report the finish result of the write block to the upper layer */
            BlockDescriptorPtr->WriteBlockFinish(RunTimeDataPtr->BlockId, E_NOT_OK);
            /* After the write block is failed, the request resource will be release */
            BndM_WriteBlockRelease(RunTimeDataPtr->BlockId, E_NOT_OK);
        }
        /* Check if the sequence of the given write job is 'WRITE_FINALIZE' */
        else if (BNDM_JOB_SEQ_WRITE_FINALIZE == RunTimeDataPtr->WriteJobSequence)
        {
            for (BndM_BlockIdType iloop = 0u; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
            {
                /* Report the finish result of the write finalize to the upper layer */
                BndM_ConfigPtr->WriteFinalizeFinish(iloop, E_NOT_OK);
                /* After the write finalize is failed, the request resource will be release */
                BndM_WriteFinalizeRelease(iloop, E_NOT_OK);
            }
        }
        else
        {
            /** do nothing */
        }
    }
    /* MEMACC_CANCELED == JobResult */
    else
    {
        /* Check if the request of the given write job is 'WRITE' or 'ERASE' */
        /* After the write block is canceled, the request resource will be release */
        BndM_WriteCancelFinish(RunTimeDataPtr->BlockId);
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Release sub write-start resource.
 */
BNDM_LOCAL void BndM_WriteStartRelease(BndM_BlockIdType BlockId, BndM_ResultType result)
{
    BndM_BlockInfoType*   BlockInfoPtr   = &(BndM_BlockInfo[BlockId]);
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Set the "BlockId" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->BlockId = BlockId;
    /* Clear the "RequestJob" of the runtime variable data */
    RunTimeDataPtr->RequestJob = MEMACC_NO_JOB;
    /* Set the "JobStatus" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->JobStatus = MEMACC_JOB_IDLE;
    /* Set the "SubJobStatus" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_IDLE;
    /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
    /* Set the "OperateAddress" of the runtime variable data */
    RunTimeDataPtr->OperateAddress = 0u;
    /* Set the "OperateSize" of the runtime variable data */
    RunTimeDataPtr->OperateSize = 0u;
    /* Set the "OperateBufferPtr" of the runtime variable data */
    RunTimeDataPtr->OperateBufferPtr = NULL_PTR;
    if (result == E_NOT_OK)
    {
        /* Set the "WriteJobSequence" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_IDLE;
        /* Set the "IsOperated" of the block information to 'TRUE' */
        BlockInfoPtr->IsOperated = TRUE;
        /* Set the "BlockVaild" of the block information to 'FAILED' */
        BlockInfoPtr->BlockVaild = BNDM_JOB_FAILED;
    }
    else
    {
        /* Set the "IsOperated" of the block information to 'TRUE' */
        BlockInfoPtr->IsOperated = TRUE;
        /* Set the "BlockVaild" of the block information to 'OK' */
        BlockInfoPtr->BlockVaild = BNDM_JOB_OK;
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Release sub write-finalize resource.
 */
BNDM_LOCAL void BndM_WriteFinalizeRelease(BndM_BlockIdType BlockId, BndM_ResultType result)
{
    BndM_BlockInfoType*   BlockInfoPtr   = &(BndM_BlockInfo[BlockId]);
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Set the "BlockId" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->BlockId = BlockId;
    /* Clear the "RequestJob" of the runtime variable data */
    RunTimeDataPtr->RequestJob = MEMACC_NO_JOB;
    /* Set the "JobStatus" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->JobStatus = MEMACC_JOB_IDLE;
    /* Set the "SubJobStatus" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_IDLE;
    /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
    /* Set the "OperateAddress" of the runtime variable data */
    RunTimeDataPtr->OperateAddress = 0u;
    /* Set the "OperateSize" of the runtime variable data */
    RunTimeDataPtr->OperateSize = 0u;
    /* Set the "OperateBufferPtr" of the runtime variable data */
    RunTimeDataPtr->OperateBufferPtr = NULL_PTR;
    /* Set the "WriteJobSequence" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_IDLE;
    if (result == E_NOT_OK)
    {
        /* Set the "IsOperated" of the block information to 'TRUE' */
        BlockInfoPtr->IsOperated = TRUE;
        /* Set the "BlockVaild" of the block information to 'FAILED' */
        BlockInfoPtr->BlockVaild = BNDM_JOB_FAILED;
    }
    else
    {
        /* Set the "IsOperated" of the block information to 'TRUE' */
        BlockInfoPtr->IsOperated = TRUE;
        /* Set the "BlockVaild" of the block information to 'OK' */
        BlockInfoPtr->BlockVaild = BNDM_JOB_OK;
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Release sub write-block resource.
 */
BNDM_LOCAL void BndM_WriteBlockRelease(BndM_BlockIdType BlockId, BndM_ResultType result)
{
    BndM_BlockInfoType*   BlockInfoPtr   = &(BndM_BlockInfo[BlockId]);
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Set the "BlockId" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->BlockId = BlockId;
    /* Clear the "RequestJob" of the runtime variable data */
    RunTimeDataPtr->RequestJob = MEMACC_NO_JOB;
    /* Set the "JobStatus" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->JobStatus = MEMACC_JOB_IDLE;
    /* Set the "SubJobStatus" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_IDLE;
    /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
    /* Set the "OperateAddress" of the runtime variable data */
    RunTimeDataPtr->OperateAddress = 0u;
    /* Set the "OperateSize" of the runtime variable data */
    RunTimeDataPtr->OperateSize = 0u;
    /* Set the "OperateBufferPtr" of the runtime variable data */
    RunTimeDataPtr->OperateBufferPtr = NULL_PTR;
    if (result == E_NOT_OK)
    {
        /* Set the "WriteJobSequence" of the runtime variable data to 'IDLE' */
        RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_IDLE;
        /* Set the "IsOperated" of the block information to 'TRUE' */
        BlockInfoPtr->IsOperated = TRUE;
        /* Set the "BlockVaild" of the block information to 'FAILED' */
        BlockInfoPtr->BlockVaild = BNDM_JOB_FAILED;
    }
    else
    {
        /* Set the "IsOperated" of the block information to 'TRUE' */
        BlockInfoPtr->IsOperated = TRUE;
        /* Set the "BlockVaild" of the block information to 'OK' */
        BlockInfoPtr->BlockVaild = BNDM_JOB_OK;
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief According to the job result, set runtime variables and the end callback notification API is invoked
 */
BNDM_LOCAL void BndM_WriteCancelFinish(BndM_BlockIdType BlockId)
{
    BndM_BlockInfoType*   BlockInfoPtr   = &(BndM_BlockInfo[BlockId]);
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    BndM_EnterCritical();
    /* Clear the "CancelDone" of the runtime variable data */
    RunTimeDataPtr->CancelDone = FALSE;
    /* Set the "BlockId" of the runtime variable data */
    RunTimeDataPtr->BlockId = BlockId;
    /* Clear the "RequestJob" of the runtime variable data */
    RunTimeDataPtr->RequestJob = MEMACC_NO_JOB;
    /* Set the "JobStatus" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->JobStatus = MEMACC_JOB_IDLE;
    /* Set the "SubJobStatus" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_IDLE;
    /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
    /* Set the "OperateAddress" of the runtime variable data */
    RunTimeDataPtr->OperateAddress = 0u;
    /* Set the "OperateSize" of the runtime variable data */
    RunTimeDataPtr->OperateSize = 0u;
    /* Set the "OperateBufferPtr" of the runtime variable data */
    RunTimeDataPtr->OperateBufferPtr = NULL_PTR;
    /* Set the "WriteJobSequence" of the runtime variable data to 'IDLE' */
    RunTimeDataPtr->WriteJobSequence = BNDM_JOB_SEQ_WRITE_IDLE;
    /* Clear the "IsOperated" of the block information */
    BlockInfoPtr->IsOperated = FALSE;
    /* Set the "BlockVaild" of the block information to 'CANCELED' */
    BlockInfoPtr->BlockVaild = BNDM_JOB_CANCELED;
    BndM_LeaveCritical();
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Request sub write-finalize task and the Request to copy data.
 */
BNDM_LOCAL void BndM_WriteFinalizeCopyDataHandle(MemAcc_AddressAreaIdType AddressAreaId)
{
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Check if the job status of the sub write finalize is 'COPY' */
    if (BNDM_JOB_SUB_WRITE_FINALIZE_COPY == RunTimeDataPtr->SubJobStatus)
    {
        boolean                       isProgramFinished = TRUE;
        const BndM_BlockInfoType*     BlockInfoPtr      = NULL_PTR;
        const BndM_BankParamInfoType* BankParamInfoPtr  = &BndM_BankParamInfo;
        for (uint16 iloop = 0u; iloop < (uint16)BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
        {
            BlockInfoPtr = &(BndM_BlockInfo[iloop]);
            /* Check if the updated flag of the given block information has been set */
            if ((boolean)FALSE == BlockInfoPtr->isUpdated)
            {
                MemAcc_MemoryInfoType MemoryInfo;
                MemAcc_GetMemoryInfo(AddressAreaId, BankParamInfoPtr->activatedStartAddress, &MemoryInfo);
                const BndM_BlockDescriptorType* BlockDescriptorPtr =
                    &(BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[iloop]);
                MemAcc_AddressType CopySourceAddress =
                    MemoryInfo.PhysicalStartAddress + BlockDescriptorPtr->BlockOffsetAddress;
                MemAcc_AddressType CopyDestAddress =
                    BankParamInfoPtr->operateStartAddress + BlockDescriptorPtr->BlockOffsetAddress;
                MemAcc_LengthType CopyBufferLength = BlockDescriptorPtr->BlockSize;
                BndM_EnterCritical();
                /* Set the "BlockId" of the runtime variable data */
                RunTimeDataPtr->BlockId = BlockInfoPtr->BlockId;
                /* Set the "OperateAddress" of the runtime variable data */
                RunTimeDataPtr->OperateAddress = CopyDestAddress;
                /* Set the "OperateSize" of the runtime variable data */
                RunTimeDataPtr->OperateSize = CopyBufferLength;
                /* Set the "OperateBufferPtr" of the runtime variable data */
                RunTimeDataPtr->OperateBufferPtr = (MemAcc_DataType*)CopySourceAddress;
                BndM_LeaveCritical();
                isProgramFinished = FALSE;
                break;
            }
        }
        /* Check if the data programming of the given partition is completed */
        if ((boolean)FALSE == isProgramFinished)
        {
            BndM_EnterCritical();
            /* Set the "SubJobStatus" of the runtime variable data to 'DATA' */
            RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_FINALIZE_DATA;
            /* Set the "MemAccState" of the runtime variable data to 'READY' */
            RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_ASYNC_WRITE_READY;
            BndM_LeaveCritical();
        }
        else
        {
            const BndM_BlockDescriptorType* BlockDescriptorPtr = NULL_PTR;
            const BndM_BlockHeaderInfoType* BlockHeaderInfoPtr =
                BndM_ConfigPtr->BlockConfigurationPtr->BlockHeaderInfoPtr;
            /* Store the 'ActivateCode' of the given bank param information */
            BndM_BankDataInfo.BankDataList.ActivateCode = BlockHeaderInfoPtr->ActivateCode;
            /* Store the 'versionNumber' of the given bank param information */
            BndM_BankDataInfo.BankDataList.VersionNumber = BankParamInfoPtr->versionNumber + 1u;
            /* Store the 'BlockTBlockTotalNumberotalSize' of the given bank param information */
            BndM_BankDataInfo.BankDataList.BlockTotalNumber = (uint32)BNDM_NUMBER_OF_USER_BLOCKS;
            /* Store the 'VerificationCode' of the given bank param information */
            for (uint16 iloop = 0; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
            {
                BlockDescriptorPtr = &(BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[iloop]);
                BndM_BankDataInfo.BankDataList.BlockSize[iloop] = BlockDescriptorPtr->BlockSize;
            }
            BndM_EnterCritical();
            /* Set the "SubJobStatus" of the runtime variable data to 'MARK' */
            RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_FINALIZE_MARK;
            /* Set the "MemAccState" of the runtime variable data to 'READY' */
            RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_ASYNC_WRITE_READY;
            /* Set the "OperateAddress" of the runtime variable data */
            RunTimeDataPtr->OperateAddress =
                BankParamInfoPtr->operateStartAddress + BlockHeaderInfoPtr->HeaderOffsetAddress;
            /* Set the "OperateSize" of the runtime variable data */
            RunTimeDataPtr->OperateSize = BlockHeaderInfoPtr->HeaderSize;
            /* Set the "OperateBufferPtr" of the runtime variable data */
            RunTimeDataPtr->OperateBufferPtr = (MemAcc_DataType*)(&BndM_BankDataInfo.Data[0]);
            BndM_LeaveCritical();
        }
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Request sub write-finalize task and the Request to program data.
 */
BNDM_LOCAL void BndM_WriteFinalizeProgramDataHandle(MemAcc_AddressAreaIdType AddressAreaId)
{
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Check if the job status of the sub write finalize is 'DATA' or
     * the job status of the sub write finalize is 'MARK' */
    if ((BNDM_JOB_SUB_WRITE_FINALIZE_DATA == RunTimeDataPtr->SubJobStatus)
        || (BNDM_JOB_SUB_WRITE_FINALIZE_MARK == RunTimeDataPtr->SubJobStatus))
    {
        /* Call the service API of MemAcc to request the memory access job */
        BndM_MemAccReq(AddressAreaId, MEMACC_WRITE_JOB);
        /* Check if the operating status of the 'MemAcc' is 'PENDING' and job status is 'IDLE' */
        if ((BNDM_MEMACC_JOB_ASYNC_WRITE_PENDING == RunTimeDataPtr->MemAccState)
            && (MEMACC_JOB_IDLE == MemAcc_GetJobStatus(AddressAreaId)))
        {
            MemAcc_JobResultType retVal = MemAcc_GetJobResult(AddressAreaId);
            if (MEMACC_OK == retVal)
            {
                /* Check if the status of the sub write finalize job is 'DATA' */
                if (BNDM_JOB_SUB_WRITE_FINALIZE_DATA == RunTimeDataPtr->SubJobStatus)
                {
                    BndM_BlockIdType    BlockIndex   = BndM_GetBlockIndex(RunTimeDataPtr->BlockId);
                    BndM_BlockInfoType* BlockInfoPtr = &(BndM_BlockInfo[BlockIndex]);
                    BndM_EnterCritical();
                    /* Set the "isUpdated" of the block information to 'TRUE' */
                    BlockInfoPtr->isUpdated = TRUE;
                    /* Set the "BlockId" of the block information */
                    BlockInfoPtr->BlockId = RunTimeDataPtr->BlockId;
                    /* Set the "BlockAddress" of the block information */
                    BlockInfoPtr->BlockAddress = RunTimeDataPtr->OperateAddress;
                    /* Set the "BlockLength" of the block information */
                    BlockInfoPtr->BlockLength = RunTimeDataPtr->OperateSize;
                    /* Set the "SubJobStatus" of the runtime variable data to 'COPY' */
                    RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_FINALIZE_COPY;
                    /* Set the "MemAccState" of the runtime variable data to 'IDLE' */
                    RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_WRITE_IDLE;
                    BndM_LeaveCritical();
                }
                /* Check if the status of the sub write finalize job is 'MARK' */
                else if (BNDM_JOB_SUB_WRITE_FINALIZE_MARK == RunTimeDataPtr->SubJobStatus)
                {
                    BndM_BankParamInfoType* BankParamInfoPtr = &BndM_BankParamInfo;
                    BndM_EnterCritical();
                    /* Set the 'activatedStartAddress' of the given bank param information */
                    BankParamInfoPtr->activatedStartAddress = BankParamInfoPtr->operateStartAddress;
                    /* Set the 'activatedLength' of the given bank param information */
                    BankParamInfoPtr->activatedLength = BankParamInfoPtr->operateLength;
                    BndM_LeaveCritical();
                    /* Finish the job assignment and release the resources of the job */
                    BndM_JobFinish(MEMACC_OK);
                }
                else
                {
                    /* do nothing */
                }
            }
            else
            {
                /* Finish the job assignment and release the resources of the job */
                BndM_JobFinish(retVal);
            }
        }
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Request sub write-cancel task and the request is sent to the bottom layer.
 *        If the bottom layer processing is complete, the corresponding status is set and the finish API is called
 */
BNDM_LOCAL void BndM_WriteCancelHandle(MemAcc_AddressAreaIdType AddressAreaId)
{
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Check if the 'cancel' asynchronous job has been completed */
    if ((boolean)TRUE == RunTimeDataPtr->CancelDone)
    {
        /* Check if the cancel state of the given write job is IDLE */
        if (BndM_RunTimeData.CancelState == BNDM_CANCEL_JOB_WRITE_IDLE)
        {
            BndM_EnterCritical();
            /* Set the "CancelState" of the runtime variable data to 'READY' */
            RunTimeDataPtr->CancelState = BNDM_CANCEL_JOB_ASYNC_WRITE_READY;
            BndM_LeaveCritical();
        }
        /* Call the service API of MemAcc to request the memory access job */
        BndM_CancelReq(AddressAreaId);
        /* Check if the operating status of the 'MemAcc' is ASYNC_PENDING and
         * the job status of MemAcc has been restored to IDLE */
        if ((BNDM_CANCEL_JOB_ASYNC_WRITE_PENDING == RunTimeDataPtr->CancelState)
            && (MEMACC_JOB_IDLE == MemAcc_GetJobStatus(AddressAreaId)))
        {
            /* get the current job result of MemAcc */
            MemAcc_JobResultType retVal = MemAcc_GetJobResult(AddressAreaId);
            /* Finish the job assignment and release the resources of the job */
            BndM_JobFinish(retVal);
        }
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Request sub write-start task and the request is sent to the bottom layer.
 *        If the bottom layer processing is complete, the corresponding status is set and the finish API is called
 */
BNDM_LOCAL void BndM_WriteStartHandle(MemAcc_AddressAreaIdType AddressAreaId)
{
    /* After preparation of the 2nd partition is successfully finished the callback Xxx_BndMWriteStartFinish
     * with the result set to E_OK shall be triggered in the context of the BndM_MainFunction. */
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Check if the 'cancel' asynchronous job has been completed */
    if ((boolean)FALSE == RunTimeDataPtr->CancelDone)
    {
        /* Check if the operating status of the 'MemAcc' is 'IDLE' */
        if (BNDM_MEMACC_JOB_WRITE_IDLE == RunTimeDataPtr->MemAccState)
        {
            BndM_EnterCritical();
            /* Set the "MemAccState" of the runtime variable data to 'READY' */
            RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_ASYNC_WRITE_READY;
            BndM_LeaveCritical();
        }
        /* Call the service API of MemAcc to request the memory access job */
        BndM_MemAccReq(AddressAreaId, MEMACC_ERASE_JOB);
        /* Check if the operating status of the 'MemAcc' is ASYNC_PENDING and
         * the job status of MemAcc has been restored to IDLE */
        if ((BNDM_MEMACC_JOB_ASYNC_WRITE_PENDING == RunTimeDataPtr->MemAccState)
            && (MEMACC_JOB_IDLE == MemAcc_GetJobStatus(AddressAreaId)))
        {
            /* get the current job result of MemAcc */
            MemAcc_JobResultType retVal = MemAcc_GetJobResult(AddressAreaId);
            /* [SWS_BndM_00007]: After preparation of the 2nd (free) partition [SWS_BndM_00003] is successfully
             * finished (writing to the 2nd partition is possible) the callback Xxx_BndMWriteStartFinish with the
             * result set to E_OK shall be triggered in the context of the BndM_MainFunction. */
            /* Finish the job assignment and release the resources of the job */
            BndM_JobFinish(retVal);
        }
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Request sub write-block task and the request is sent to the bottom layer.
 *        If the bottom layer processing is complete, the corresponding status is set and the finish API is called
 */
BNDM_LOCAL void BndM_WriteBlockHandle(MemAcc_AddressAreaIdType AddressAreaId)
{
    /* After writing of the 2nd partition is finished the callback Xxx_BndMWriteBlockFinish
     * with the result set to E_OK shall be triggered in the context of the BndM_MainFunction. */
    /* A call of BndM_WriteBlock shall trigger the writing of the data to the 2nd partition.
     * The data (ImplementationDataType) shall be not modified to allow a pointer access. */
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Check if the 'cancel' asynchronous job has been completed */
    if ((boolean)FALSE == RunTimeDataPtr->CancelDone)
    {
        /* Check if the operating status of the 'MemAcc' is 'IDLE' */
        if (BNDM_MEMACC_JOB_WRITE_IDLE == RunTimeDataPtr->MemAccState)
        {
            BndM_EnterCritical();
            /* Set the "MemAccState" of the runtime variable data to 'READY' */
            RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_ASYNC_WRITE_READY;
            BndM_LeaveCritical();
        }
        /* Call the service API of MemAcc to request the memory access job */
        BndM_MemAccReq(AddressAreaId, MEMACC_WRITE_JOB);
        /* Check if the operating status of the 'MemAcc' is 'ASYNC_PENDING' and
         * the job status of MemAcc has been restored to 'IDLE' */
        if ((BNDM_MEMACC_JOB_ASYNC_WRITE_PENDING == RunTimeDataPtr->MemAccState)
            && (MEMACC_JOB_IDLE == MemAcc_GetJobStatus(AddressAreaId)))
        {
            MemAcc_JobResultType retVal       = MemAcc_GetJobResult(AddressAreaId);
            BndM_BlockIdType     BlockIndex   = BndM_GetBlockIndex(RunTimeDataPtr->BlockId);
            BndM_BlockInfoType*  BlockInfoPtr = &(BndM_BlockInfo[BlockIndex]);
            if (MEMACC_OK == retVal)
            {
                BndM_EnterCritical();
                /* Set the "isUpdated" of the block information to 'TRUE' */
                BlockInfoPtr->isUpdated = TRUE;
                /* Set the "IsOperated" of the block information to 'TRUE' */
                BlockInfoPtr->IsOperated = TRUE;
                /* Set the "BlockVaild" of the block information to 'OK' */
                BlockInfoPtr->BlockVaild = BNDM_JOB_OK;
                /* Set the "BlockAddress" of the block information */
                BlockInfoPtr->BlockId = RunTimeDataPtr->BlockId;
                /* Set the "BlockAddress" of the block information */
                BlockInfoPtr->BlockAddress = RunTimeDataPtr->OperateAddress;
                /* Set the "BlockLength" of the block information */
                BlockInfoPtr->BlockLength = RunTimeDataPtr->OperateSize;
                BndM_LeaveCritical();
                /* Finish the job assignment and release the resources of the job */
                BndM_JobFinish(retVal);
            }
            else
            {
                /* Finish the job assignment and release the resources of the job */
                BndM_JobFinish(retVal);
            }
        }
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief Request sub write-finalize task and the request is sent to the bottom layer.
 *        If the bottom layer processing is complete, the corresponding status is set and the finish API is called
 */
BNDM_LOCAL void BndM_WriteFinalizeHandle(MemAcc_AddressAreaIdType AddressAreaId)
{
    /* A call of BndM_WriteFinalize shall trigger the finalization of the 2nd partition.
     * In background the BndM shall make the 2nd partition consistent by coping all unchanged BndMBlockDescriptor to the
     * 2nd partition. If the finalization is successful the BndM shall make the 2nd partition to the active partition
     * and trigger the callback Xxx_BndMWriteFinalizeFinish with the result set to E_OK. */
    BndM_RunTimeDataType* RunTimeDataPtr = &BndM_RunTimeData;
    /* Check if the 'cancel' asynchronous job has been completed */
    if ((boolean)FALSE == RunTimeDataPtr->CancelDone)
    {
        /* Check if the operating status of the 'MemAcc' is 'IDLE' */
        if (BNDM_MEMACC_JOB_WRITE_IDLE == RunTimeDataPtr->MemAccState)
        {
            const BndM_BankParamInfoType* BankParamInfoPtr = &BndM_BankParamInfo;
            /* Check if it is the first time to perform program job */
            if ((boolean)FALSE == BankParamInfoPtr->isFirstProgram)
            {
                BndM_EnterCritical();
                /* Set the "SubJobStatus" of the runtime variable data to 'COPY' */
                RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_FINALIZE_COPY;
                BndM_LeaveCritical();
            }
            else
            {
                const BndM_BlockDescriptorType* BlockDescriptorPtr = NULL_PTR;
                const BndM_BlockHeaderInfoType* BlockHeaderInfoPtr =
                    BndM_ConfigPtr->BlockConfigurationPtr->BlockHeaderInfoPtr;
                BndM_EnterCritical();
                /* Store the 'ActivateCode' of the given bank param information */
                BndM_BankDataInfo.BankDataList.ActivateCode = BlockHeaderInfoPtr->ActivateCode;
                /* Store the 'versionNumber' of the given bank param information */
                BndM_BankDataInfo.BankDataList.VersionNumber = BankParamInfoPtr->versionNumber + 1u;
                /* Store the 'BlockTotalNumber' of the given bank param information */
                BndM_BankDataInfo.BankDataList.BlockTotalNumber = (uint32)BNDM_NUMBER_OF_USER_BLOCKS;
                /* Store the 'BlockSize' of the given bank param information */
                for (uint16 iloop = 0; iloop < BNDM_NUMBER_OF_USER_BLOCKS; ++iloop)
                {
                    BlockDescriptorPtr = &(BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[iloop]);
                    BndM_BankDataInfo.BankDataList.BlockSize[iloop] = BlockDescriptorPtr->BlockSize;
                }
                /* Set the "SubJobStatus" of the runtime variable data to 'MARK' */
                RunTimeDataPtr->SubJobStatus = BNDM_JOB_SUB_WRITE_FINALIZE_MARK;
                /* Set the "MemAccState" of the runtime variable data to 'READY' */
                RunTimeDataPtr->MemAccState = BNDM_MEMACC_JOB_ASYNC_WRITE_READY;
                /* Set the "OperateAddress" of the runtime variable data */
                RunTimeDataPtr->OperateAddress =
                    BankParamInfoPtr->operateStartAddress + BlockHeaderInfoPtr->HeaderOffsetAddress;
                /* Set the "OperateSize" of the runtime variable data */
                RunTimeDataPtr->OperateSize = BlockHeaderInfoPtr->HeaderSize;
                /* Set the "OperateBufferPtr" of the runtime variable data */
                RunTimeDataPtr->OperateBufferPtr = (MemAcc_DataType*)(&BndM_BankDataInfo.Data[0]);
                BndM_LeaveCritical();
            }
        }
        /* Request sub write-finalize task and the Request to copy data. */
        BndM_WriteFinalizeCopyDataHandle(AddressAreaId);
        /* Request sub write-finalize task and the Request to program data. */
        BndM_WriteFinalizeProgramDataHandle(AddressAreaId);
    }
}
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
