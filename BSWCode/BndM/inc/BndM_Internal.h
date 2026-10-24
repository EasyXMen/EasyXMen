
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
 **  @file               : BndM_Internal.h
 **  @author             : haibin.shao
 **  @date               : 2025/06/20
 **  @vendor             : isoft
 **  @description        : Internal declaration for Bulk NvData Manager
 **
 ***********************************************************************************************************************/

#ifndef BNDM_INTERNAL_H_
#define BNDM_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "BndM_Cfg.h"
#include "BndM_Types.h"
#include "BndM_OptMacros.h"
#include "Det.h"
#if (STD_ON == BNDM_FEATURE_MEMCOPY_USED_ILIB)
#include "IStdLib.h"
#endif

/* PRQA S 3472 ++ */ /* VL_BndM_3472 */
#if (STD_ON == BNDM_DEV_ERROR_DETECT)
/**
 * @brief       Report development error to the Default Error Tracer
 * @param[in]   ApiId    : The identity of the API
 * @param[in]   ErrorId  : The identity of the development error
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
#define BNDM_DET_REPORT(ApiId, ErrorId) ((void)Det_ReportError(BNDM_MODULE_ID, BNDM_INSTANCE_ID, (ApiId), (ErrorId)))
#endif

/**
 * @brief       Report runtime error to the Default Error Tracer
 * @param[in]   ApiId    : The identity of the API
 * @param[in]   ErrorId  : The identity of the Runtime error
 * @return      void
 * @reentrant   No-Reentrant.
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDE
 */
#define BNDM_DET_REPORTRUNTIME(ApiId, ErrorId) \
    ((void)Det_ReportRuntimeError(BNDM_MODULE_ID, BNDM_INSTANCE_ID, (ApiId), (ErrorId)))
/* PRQA S 3472 -- */

#define BNDM_BLOCK_NUM_INVALID (0xFFFFu) /**< Used to describe macro definitions with invalid block numbers */

#define BNDM_BANK_ACTIVATE_CODE 0x55555555u /**< Indicate that the activate code of the bank for BndM  */

#if (STD_ON == BNDM_FEATURE_MEMCOPY_USED_ILIB)
#define BndM_MemCpy(dest, src, size)   IStdLib_MemCpy(dest, src, size)   /**< API mapping for memcpy */
#define BndM_MemSet(dest, value, size) IStdLib_MemSet(dest, value, size) /**< API mapping for memset */
#endif

/* ================================================ type definitions ================================================ */
/**
 * @brief This function pointer type is used to describe the link that raise 'WriteHandle' to Upper layer.
 */
typedef void (*BndM_WriteHandleFuncPtr)(MemAcc_AddressAreaIdType AddressAreaId);

/**
 * @brief Enumeration type that is used to describe the state of the write job sequence
 */
typedef enum
{
    BNDM_JOB_SEQ_WRITE_IDLE,     /**< Indicate that the write job sequence is 'IDLE' @range 0x00 */
    BNDM_JOB_SEQ_WRITE_START,    /**< Indicate that the write job sequence is 'START' @range 0x01 */
    BNDM_JOB_SEQ_WRITE_BLOCK,    /**< Indicate that the write job sequence is 'BLOCK' @range 0x02 */
    BNDM_JOB_SEQ_WRITE_FINALIZE, /**< Indicate that the write job sequence is 'FINALIZE' @range 0x03 */
    BNDM_JOB_SEQ_WRITE_CANCEL,   /**< Indicate that the write job sequence is 'CANCEL' @range 0x04 */
} BndM_WriteJobSequenceType;

/**
 * @brief Enumeration type that is used to describe the status of the write sub job
 */
typedef enum
{
    BNDM_JOB_SUB_WRITE_IDLE,          /**< Indicate that the write sub job is 'IDLE' @range 0x00 */
    BNDM_JOB_SUB_WRITE_FINALIZE_COPY, /**< Indicate that the write sub job is 'FINALIZE-COPY' @range 0x01 */
    BNDM_JOB_SUB_WRITE_FINALIZE_DATA, /**< Indicate that the write sub job is 'FINALIZE-DATA' @range 0x02 */
    BNDM_JOB_SUB_WRITE_FINALIZE_MARK  /**< Indicate that the write sub job is 'FINALIZE-MARK' @range 0x03 */
} BndM_SubJobStatusType;

/**
 * @brief Enumeration type that is used to describe the write state of the MemAcc job
 */
typedef enum
{
    BNDM_MEMACC_JOB_WRITE_IDLE,          /**< Indicate that the MemAcc write job state is 'IDLE' @range 0x00 */
    BNDM_MEMACC_JOB_ASYNC_WRITE_READY,   /**< Indicate that the MemAcc write job state is 'READ' @range 0x01 */
    BNDM_MEMACC_JOB_ASYNC_WRITE_PENDING, /**< Indicate that the MemAcc write job state is 'PENDING' @range 0x02 */
} BndM_MemAccStateType;

/**
 * @brief Enumeration type that is used to describe the state of the write cancel
 */
typedef enum
{
    BNDM_CANCEL_JOB_WRITE_IDLE,          /**< Indicate that the write cancel state is 'IDLE' @range 0x00 */
    BNDM_CANCEL_JOB_ASYNC_WRITE_READY,   /**< Indicate that the write cancel state is 'READ' @range 0x01 */
    BNDM_CANCEL_JOB_ASYNC_WRITE_PENDING, /**< Indicate that the write cancel state is 'PENDING' @range 0x02 */
} BndM_CancelStateType;

/**
 * @brief Enumeration type that is used to describe the result of the write job
 */
typedef enum
{
    BNDM_JOB_OK,       /**< Indicate that the write job result is 'OK' @range 0x00 */
    BNDM_JOB_FAILED,   /**< Indicate that the write job result is 'FAILED' @range 0x00 */
    BNDM_JOB_PENDING,  /**< Indicate that the write job result is 'PENDING' @range 0x00 */
    BNDM_JOB_CANCELED, /**< Indicate that the write job result is 'CANCELED' @range 0x00 */
    BNDM_BLOCK_INVALID /**< Indicate that the write job result is 'BLOCK_INVALID' @range 0x00 */
} BndM_JobResultType;

/**
 * @brief Used to describe the type of run time data structure in bulk NV data manager
 */
typedef struct BndM_RunTimeDataTypeTag
{
    boolean                   CancelDone; /**< Indicate that the cancel done of given block. @range 0..1 */
    BndM_BlockIdType          BlockId; /**< Indicate that the unique identification of given block. @range 0..65535 */
    MemAcc_JobType            RequestJob;       /**< Indicate that the request job of given block. @range 0..7 */
    MemAcc_JobStatusType      JobStatus;        /**< Indicate that the job status of given block. @range 0..1 */
    BndM_SubJobStatusType     SubJobStatus;     /**< Indicate that the sub job status of given block. @range 0..3 */
    BndM_MemAccStateType      MemAccState;      /**< Indicate that the 'MemAcc' state of given block. @range 0..2 */
    BndM_CancelStateType      CancelState;      /**< Indicate that the 'Cancel' state of given block. @range 0..2 */
    BndM_WriteJobSequenceType WriteJobSequence; /**< Indicate that the write job sequence of given block. @range 0..4 */
    MemAcc_AddressType OperateAddress;   /**< Indicate that the operate address of given block. @range 0..4294967295 */
    MemAcc_LengthType  OperateSize;      /**< Indicate that the operate size of given block. @range 0..4294967295 */
    MemAcc_DataType*   OperateBufferPtr; /**< Indicate that the operate buffer of given block. @range NA */
} BndM_RunTimeDataType;

/**
 * @brief Used to describe the type of block information structure in bulk NV data manager
 */
typedef struct BndM_BlockInfoTypeTag
{
    boolean            isUpdated;    /**< The flag indicating that has been updated of given block. @range 0..1 */
    boolean            IsOperated;   /**< The flag indicating that has been operated of given block. @range 0..1 */
    BndM_JobResultType BlockVaild;   /**< Indicate that the validity of given block. @range 0..5 */
    BndM_BlockIdType   BlockId;      /**< Indicate that the identifier of given block. @range 0..65535 */
    MemAcc_AddressType BlockAddress; /**< Indicate that the offset address of given block. @range 0..4294967295 */
    MemAcc_LengthType  BlockLength;  /**< Indicate that the length of given block. @range 0..4294967295 */
    MemAcc_DataType*   BlockDataPtr; /**< Indicate that the data pointer of given block. @range NA */
} BndM_BlockInfoType;

/**
 * @brief Used to describe the type of bank parameter information structure in bulk NV data manager
 */
typedef struct BndM_BankParamInfoTypeTag
{
    boolean isFirstProgram; /**< The flag indicating that has been programmed of given bank. @range 0..1 */
    uint32  versionNumber;  /**< Indicate that the version number of given bank. @range 0..4294967295 */
    MemAcc_AddressType
        activatedStartAddress; /**< Indicate that the activated start address of given bank. @range 0..4294967295 */
    MemAcc_LengthType activatedLength; /**< Indicate that the activated length of given bank. @range 0..4294967295 */
    MemAcc_AddressType
        operateStartAddress;         /**< Indicate that the operate start address of given bank. @range 0..4294967295 */
    MemAcc_LengthType operateLength; /**< Indicate that the operate length of given bank. @range 0..4294967295 */
} BndM_BankParamInfoType;

/**
 * @brief Used to describe the type of bank data list structure in bulk NV data manager
 */
typedef struct BndM_BankDataListTypeTag
{
    uint32 ActivateCode;     /**< Indicate that the activate code of given bank. @range 0..4294967295 */
    uint32 VersionNumber;    /**< Indicate that the version number of given bank. @range 0..4294967295 */
    uint32 BlockTotalNumber; /**< Indicate that the block total size of given bank. @range 0..4294967295 */
    uint32 BlockSize[BNDM_NUMBER_OF_USER_BLOCKS]; /**< Indicate that the size of per block. @range 0..4294967295 */
} BndM_BankDataListType;

/* PRQA S 0750 ++ */ /* VL_BndM_0750 */
/**
 * @brief Used to describe the type of bank data information union in bulk NV data manager
 */
typedef union BndM_BankDataInfoTypeTag
{
    uint8 Data[BNDM_BANK_HEAD_INFO_SIZE]; /**< Indicate that the data buffer of given bank. @range 0..255 */
    BndM_BankDataListType BankDataList;   /**< Indicate that the data list of given bank. @range NA */
} BndM_BankDataInfoType;

/**
 * @brief Used to describe the type of erase value information union in bulk NV data manager
 */
typedef union BndM_MemEraseInfoTypeTag
{
    uint8  Data[BNDM_MEM_ERASE_VALUE_INFO]; /**< Indicate that the erase buffer of given memory. @range 0..255 */
    uint32 EraseValue;                      /**< Indicate that the erase value of given memory. @range 0..429496729 */
} BndM_MemEraseInfoType;
/* PRQA S 0750 -- */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */
#define BNDM_START_SEC_VAR_CLEARED_BOOLEAN
#include "BndM_MemMap.h"
/**
 * @brief initialize status of BndM module
 * @range 0..1
 */
extern boolean BndM_ModuleInitStatus;
#define BNDM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "BndM_MemMap.h"
#define BNDM_START_SEC_VAR_CLEARED_PTR
#include "BndM_MemMap.h"
/**
 * @brief PB configuration pointer for the bulk non volatile Data Manager
 * @range N/A
 */
extern const BndM_ConfigType* BndM_ConfigPtr;
#define BNDM_STOP_SEC_VAR_CLEARED_PTR
#include "BndM_MemMap.h"

/* ========================================== external function definitions ========================================= */

#if (STD_OFF == BNDM_FEATURE_MEMCOPY_USED_ILIB)
/**
 * @brief        Service is used to get the pointer of the runtime data.
 * @return       void
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
BNDM_LOCAL_INLINE void BndM_MemCpy(MemAcc_DataType* dest, const MemAcc_DataType* src, MemAcc_LengthType size)
{
    for (MemAcc_LengthType index = 0u; index < size; ++index)
    {
        dest[index] = src[index];
    }
}

/**
 * @brief        Service is used to get the pointer of the runtime data.
 * @return       void
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
BNDM_LOCAL_INLINE void BndM_MemSet(MemAcc_DataType* dest, const MemAcc_DataType value, MemAcc_LengthType size)
{
    for (MemAcc_LengthType index = 0u; index < size; ++index)
    {
        dest[index] = value;
    }
}
#endif

/**
 * @brief        Service is used to get the pointer of the runtime data.
 * @return       BndM_RunTimeDataType
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
BNDM_LOCAL_INLINE MemAcc_LengthType BndM_GetBlockSize(const BndM_BlockIdType BlockId)
{
    return BndM_ConfigPtr->BlockConfigurationPtr->BlockDescriptorPtr[BlockId].BlockSize;
}

#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to the write request for the given block.
 * @param[in]    BlockId: The identifier of the given block
 * @param[in]    WriteBufferPtr: The pointer for writing to the data buffer
 * @return       Std_ReturnType
 * @retval       E_OK: The write request is accepted.
 * @retval       E_NOT_OK: The write request is rejected.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
extern Std_ReturnType BndM_WriteBlockRequest(const BndM_BlockIdType BlockId, const MemAcc_DataType* WriteBufferPtr);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to get the parameter information pointer for the bank.
 * @return       BndM_BankParamInfoType
 * @retval       the parameter information pointer of the bank.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
extern BndM_BankParamInfoType* BndM_GetBankParamInfoPtr(void);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"
#define BNDM_START_SEC_CODE
#include "BndM_MemMap.h"
/**
 * @brief        Service is used to get the parameter information pointer for the block.
 * @param[in]    BlockId: The identifier of the given block
 * @return       BndM_BlockInfoType
 * @retval       the parameter information pointer of the block.
 * @reentrant    TRUE
 * @synchronous  TRUE
 * @trace        CPD-PLACEHOLDE
 */
extern BndM_BlockInfoType* BndM_GetBlockInfoPtr(const BndM_BlockIdType BlockId);
#define BNDM_STOP_SEC_CODE
#include "BndM_MemMap.h"

#endif /* BNDM_INTERNAL_H_ */
