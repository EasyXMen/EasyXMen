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
 **  @file               : Ea.c
 **  @author             : peng.wu
 **  @date               : 2024/12/11
 **  @vendor             : isoft
 **  @description        : Implementation for Ea
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Ea.h"
#include "Ea_Cfg.h"
#include "Ea_Arti.h"

/* ===================================================== macros ===================================================== */
/**  EA block header info
 *  byte0 - byte1: BlockNumber (uint16)
 *  byte2 - byte3: BlockSize (uint16)
 *  byte4 - byte7: BlockWriteCycle (uint32)
 *  byte8 - byte9: BlockValid  0xA5 0xA5 means valid; 0x5A 0x5A means invalid;
 *                 others means inconsistent
 *  byte10 - byte13 BlockStartAddress (uint32)
 *
 *  total header size: 14byte
 */
#define EA_TOTAL_HEADER_LENGTH 14u /** Defines used as keyword for the total header length */

/* PRQA S 3412 ++ */ /* VL_Ea_3412 */
#define EA_HEADER_ALIGNMENT_LENGTH                                                   \
    (((EA_TOTAL_HEADER_LENGTH + (EA_VIRTUAL_PAGE_SIZE - 1U)) / EA_VIRTUAL_PAGE_SIZE) \
     * EA_VIRTUAL_PAGE_SIZE) /** Defines used as keyword for the alignment header length */

/**
 * @brief Set the block information
 * Macro parameters:
 *  @param[in] blockIndex Block index
 *  @param[in] isOperated Indicate the block operation status
 *  @param[in] blockVaild Indicate the validity of blcok
 */
#define EA_SET_BLOCK_INFO(blockIndex, isOperated, blockVaild) \
    Ea_BlockInfo[blockIndex].IsOperated = (isOperated);       \
    Ea_BlockInfo[blockIndex].BlockVaild = (blockVaild);

#if (CPU_32_WITH_16_ADR == TRUE)
/**
 * @brief Calculate the aligned values
 * Macro parameters:
 *  @param[in] ParamVal Align based on this value
 *  @param[in] ParamType Used to calculate the size of a type
 *  @param[inout] pSerialPtr Used to record the aligned address
 */
#define EA_SERIALIZE(ParamVal, pSerialPtr, ParamType)            \
    do                                                           \
    {                                                            \
        for (uint8 i = 0u; i < (sizeof(ParamType) * 2u); i++)    \
        {                                                        \
            ((pSerialPtr)[i]) = (uint8)((ParamVal) >> (i * 8u)); \
        }                                                        \
        (pSerialPtr) = &((pSerialPtr)[sizeof(ParamType) * 2u]);  \
    } while (0);

/**
 * @brief Calculate the value before alignment
 * Macro parameters:
 *  @param[inout] pDeserialPtr Used to record addresses before alignment
 *  @param[in] ParamVal Calculate based on this value
 *  @param[in] ParamType Used to calculate the size of a type
 */
#define EA_DESERIALIZE(pDeserialPtr, ParamVal, ParamType)                                    \
    do                                                                                       \
    {                                                                                        \
        ParamVal = 0u;                                                                       \
        for (uint8 i = 0u; i < (sizeof(ParamType) * 2u); i++)                                \
        {                                                                                    \
            (ParamVal) = (ParamVal) | (ParamType)((ParamType)(pDeserialPtr)[i] << (i * 8u)); \
        }                                                                                    \
        (pDeserialPtr) = &((pDeserialPtr)[sizeof(ParamType) * 2u]);                          \
    } while (0);
#else
#define EA_SERIALIZE(ParamVal, pSerialPtr, ParamType)            \
    do                                                           \
    {                                                            \
        for (uint8 i = 0u; i < sizeof(ParamType); i++)           \
        {                                                        \
            ((pSerialPtr)[i]) = (uint8)((ParamVal) >> (i * 8u)); \
        }                                                        \
        (pSerialPtr) = &((pSerialPtr)[sizeof(ParamType)]);       \
    } while (0);

#define EA_DESERIALIZE(pDeserialPtr, ParamVal, ParamType)                                    \
    do                                                                                       \
    {                                                                                        \
        (ParamVal) = 0u;                                                                     \
        for (uint8 i = 0u; i < sizeof(ParamType); i++)                                       \
        {                                                                                    \
            (ParamVal) = (ParamVal) | (ParamType)((ParamType)(pDeserialPtr)[i] << (i * 8u)); \
        }                                                                                    \
        (pDeserialPtr) = &((pDeserialPtr)[sizeof(ParamType)]);                               \
    } while (0);
#endif
/* PRQA S 3412 -- */

#define EA_BLOCKNUM_INVALID (0xFFFFu) /** Invalid BlockNumber */

#define EA_MAX_WRITE_CYCLE (0xFFFFFFFFu) /** Max write cycle */

#define EA_BLOCK_INVALID (0x5A5Au) /** Invalid flag of a block */
#define EA_BLOCK_VALID   (0xA5A5u) /** Valid flag of a block */

/**
 * @name EA_BLOCK_STATUS
 * @{
 * @brief Block status of the Ea module
 */
#define EA_BLOCK_STATUS_INVALID      (0u) /**< Invalid status of a block */
#define EA_BLOCK_STATUS_VALID        (1u) /**< Valid status of a block */
#define EA_BLOCK_STATUS_INCONSISTENT (2u) /**< Inconsistent status of a block */
#define EA_BLOCK_STATUS_CONSISTENT   (3u) /**< Consistent tatus of a block */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief       Get the block BlockNumber index
 * @param[in]   BlockNumber: The block number passed in by the upper module
 * @reentrant   Non Reentrant
 * @return      uint16
 * @synchronous TRUE
 * @trace       CPD-75785
 */
EA_LOCAL uint16 Ea_GetBlockIndex(uint16 BlockNumber);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Job finish, set runtime data
 * @param[in]   JobResult: The current job result
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75786
 */
EA_LOCAL void Ea_JobFinish(MemIf_JobResultType JobResult);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Service to report to this module the successful end of an asynchronous operation
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75787
 */
EA_LOCAL void Ea_CallJobEndNotification(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Service to report to this module the failure of an asynchronous operation
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75788
 */
EA_LOCAL void Ea_CallJobErrorNotification(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Service to read
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75789
 */
EA_LOCAL void Ea_InterRead(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Update runtime state when request read
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75790
 */
EA_LOCAL void Ea_InterReadSub(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Service to write
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75791
 */
EA_LOCAL void Ea_InterWrite(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Update runtime state when request write
 * @param[in]   blockIndex: The current block index
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75792
 */
EA_LOCAL void Ea_InterWriteSub(const uint16 blockIndex);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Service to invalidate
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75793
 */
EA_LOCAL void Ea_InterInvalidate(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Service to erase
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75794
 */
EA_LOCAL void Ea_InterErase(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Service to cancel
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75795
 */
EA_LOCAL void Ea_InterCancel(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Read job finish, copy data to upper buffer
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75796
 */
EA_LOCAL void Ea_InterCopyReadData(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Service to call MemAcc Api
 * @param[in]   Ea_RequestJob: The current request job
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75797
 */
EA_LOCAL void Ea_MemAccReq(Ea_RequestJobType Ea_RequestJob);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Read job finish, check read header
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75798
 */
EA_LOCAL void Ea_InterCheckReadHeader(void);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Write or invalidate job process, prepare write header
 * @param[in]   status: The current block status
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-75799
 */
EA_LOCAL void Ea_InterPrepareWriteHeader(uint8 status);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Handling of internal tasks
 * @param[in]   JobType: The job type
 * @param[in]   blockIndex: The block index
 * @param[in]   deviceIndex: The device index
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       -
 */
EA_LOCAL void Ea_InternalProcess(Ea_RequestJobType JobType, uint16 blockIndex, uint8 deviceIndex);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"

/**
 * @brief       Handling after the successful completion of an internal task
 * @param[in]   JobType: The job type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       -
 */
EA_LOCAL void Ea_InternalProcessOk(Ea_RequestJobType JobType);
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"

/* ============================================ internal data definition ============================================ */
#define EA_START_SEC_VAR_CLEARED_8
#include "Ea_MemMap.h"
/* PRQA S 3408 ++ */ /* VL_Ea_3408 */
/**
 * @brief Record the status of the Ea module @range 0..3
 */
EA_LOCAL MemIf_StatusType Ea_State;
#define EA_STOP_SEC_VAR_CLEARED_8
#include "Ea_MemMap.h"

#define EA_START_SEC_VAR_CLEARED_8
#include "Ea_MemMap.h"
/**
 * @brief Used for the Ea module data Buffer @range NA
 */
EA_LOCAL uint8 Ea_ReadWritBuffer[EA_BLOCK_MAX_SIZE + EA_TOTAL_HEADER_LENGTH];
#define EA_STOP_SEC_VAR_CLEARED_8
#include "Ea_MemMap.h"

#define EA_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Ea_MemMap.h"
/**
 * @brief Record the management data of the current running task @range NA
 */
EA_LOCAL Ea_RunTimeDataType Ea_RunTimeData;
#define EA_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Ea_MemMap.h"

#define EA_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Ea_MemMap.h"
/**
 * @brief Used to record the management data of each Block @range NA
 */
EA_LOCAL Ea_BlockInfoType Ea_BlockInfo[EA_NUMBER_OF_BLOCKS];
#define EA_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Ea_MemMap.h"

#if (EA_POLLING_MODE == STD_OFF)
#define EA_START_SEC_CONST_PTR
#include "Ea_MemMap.h"
/**
 * @brief     Function Pointer for internal jobs @range NA
 */
EA_LOCAL const Ea_VoidFunctionVoidPtr EA_InterServFunc[] = {
    NULL_PTR,           /** EA_JOB_IDLE */
    Ea_InterRead,       /** EA_JOB_READ */
    Ea_InterWrite,      /** EA_JOB_WRITE */
    Ea_InterCancel,     /** EA_JOB_CANCEL */
    Ea_InterInvalidate, /** EA_JOB_INVALIDATE */
    Ea_InterErase,      /** EA_JOB_ERASE */
};
/* PRQA S 3408 -- */
#define EA_STOP_SEC_CONST_PTR
#include "Ea_MemMap.h"
#endif

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definition ========================================== */
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
/**
 * @brief Initialize all variables and states if the input parameters are valid, otherwise an error is reported
 */
void Ea_Init(const Ea_ConfigType* ConfigPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 2, 0x00, ConfigPtr);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR != ConfigPtr)
    {
        Ea_Det_ReportError(EA_SID_INIT, EA_E_PARAM_POINTER);
    }
    else
#endif
    {
        EA_PARAM_UNUSED(ConfigPtr);
        /** init run time data of Ea*/
        Ea_RunTimeData.LastJobResult              = MEMIF_BLOCK_INCONSISTENT;
        Ea_RunTimeData.RequestJob                 = EA_JOB_IDLE;
        Ea_RunTimeData.SubJobStatus               = EA_SUB_JOB_IDLE;
        Ea_RunTimeData.MemHwAbState               = EA_MEMHWAB_JOB_IDLE;
        Ea_RunTimeData.PendReq.BlockNumber        = 0u;
        Ea_RunTimeData.PendReq.BlockOffset        = 0u;
        Ea_RunTimeData.PendReq.BlockIndex         = EA_BLOCKNUM_INVALID;
        Ea_RunTimeData.PendReq.Length             = 0u;
        Ea_RunTimeData.PendReq.DealOffset         = 0u;
        Ea_RunTimeData.PendReq.DealSize           = 0u;
        Ea_RunTimeData.PendReq.DataBufferPtr      = NULL_PTR;
        Ea_RunTimeData.PendReq.DataWriteBufferPtr = NULL_PTR;
        Ea_RunTimeData.PendReq.InterDataBufferPtr = NULL_PTR;

        /** init internal buffer of Ea*/
        for (uint16 iloop = 0u; iloop < (EA_BLOCK_MAX_SIZE + EA_TOTAL_HEADER_LENGTH); iloop++)
        {
            Ea_ReadWritBuffer[iloop] = 0u;
        }

        /** init block information of Ea*/
        for (uint16 iloop = 0u; iloop < Ea_Config.BlockCfg->BlockNum; iloop++)
        {
            Ea_BlockInfo[iloop].IsOperated      = FALSE;
            Ea_BlockInfo[iloop].BlockVaild      = MEMIF_BLOCK_INCONSISTENT;
            Ea_BlockInfo[iloop].BlockWriteCycle = 0u;
        }
        Ea_State = MEMIF_IDLE;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 1, 0x00);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Get the block id value, check the develop error, then put the read request into the queue
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Ea_STMIF */
Std_ReturnType Ea_Read(uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 5, 0x02, BlockNumber, BlockOffset, DataBufferPtr, Length);
    Std_ReturnType ret        = E_NOT_OK;
    uint16         blockIndex = Ea_GetBlockIndex(BlockNumber);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = EA_E_NO_ERROR;
    if (MEMIF_UNINIT == Ea_State)
    {
        errorId = EA_E_UNINIT;
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_Det_ReportRunTimeError(EA_SID_READ, EA_E_BUSY);
    }
    else if (EA_BLOCKNUM_INVALID == blockIndex)
    {
        errorId = EA_E_INVALID_BLOCK_NO;
    }
    else if (Ea_Config.BlockCfg->BlockCfg[blockIndex].BlockSize < (BlockOffset + EA_BLOCK_OVER_HEAD))
    {
        errorId = EA_E_INVALID_BLOCK_OFS;
    }
    else if (
        (Ea_Config.BlockCfg->BlockCfg[blockIndex].BlockSize < (Length + BlockOffset + EA_BLOCK_OVER_HEAD))
#if (EA_MINIMUM_READ_PAGE_SIZE > 0u)
        || (0U != (Length % EA_MINIMUM_READ_PAGE_SIZE))
#endif
    )
    {
        errorId = EA_E_INVALID_BLOCK_LEN;
    }
    else if (NULL_PTR == DataBufferPtr)
    {
        errorId = EA_E_PARAM_POINTER;
    }
    else
#endif
    {
        Ea_State                                  = MEMIF_BUSY;
        Ea_RunTimeData.LastJobResult              = MEMIF_JOB_PENDING;
        Ea_RunTimeData.RequestJob                 = EA_JOB_READ;
        Ea_RunTimeData.PendReq.BlockNumber        = BlockNumber;
        Ea_RunTimeData.PendReq.BlockIndex         = blockIndex;
        Ea_RunTimeData.PendReq.Length             = Length;
        Ea_RunTimeData.PendReq.BlockOffset        = BlockOffset;
        Ea_RunTimeData.PendReq.DataBufferPtr      = DataBufferPtr;
        Ea_RunTimeData.PendReq.InterDataBufferPtr = Ea_ReadWritBuffer;
        ret                                       = E_OK;
    }
#if (STD_ON == EA_DEV_ERROR_DETECT)
    if (errorId != EA_E_NO_ERROR)
    {
        Ea_Det_ReportError((uint8)EA_SID_READ, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 2, 0x02, ret);
    return ret;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Get the block id value, check the develop error, then put the write request into the queue
 */
Std_ReturnType Ea_Write(uint16 BlockNumber, const uint8* DataBufferPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 3, 0x03, BlockNumber, DataBufferPtr);
    Std_ReturnType ret        = E_NOT_OK;
    uint16         blockIndex = Ea_GetBlockIndex(BlockNumber);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = EA_E_NO_ERROR;
    if (MEMIF_UNINIT == Ea_State)
    {
        errorId = EA_E_UNINIT;
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_Det_ReportRunTimeError(EA_SID_WRITE, EA_E_BUSY);
    }
    else if (EA_BLOCKNUM_INVALID == blockIndex)
    {
        errorId = EA_E_INVALID_BLOCK_NO;
    }
    else if (
        (NULL_PTR == DataBufferPtr)
#if (EA_BUFFER_ALIGN_VALUE > 0uL)
        || (0U != ((Ea_AddressType)DataBufferPtr % EA_BUFFER_ALIGN_VALUE))
#endif
    )
    {
        errorId = EA_E_PARAM_POINTER;
    }
    else
#endif
    {
        Ea_BlockInfoType*               blockInfoPtr = &(Ea_BlockInfo[blockIndex]);
        const Ea_BlockConfigurationType blockCfg     = Ea_Config.BlockCfg->BlockCfg[blockIndex];
        if ((0uL == blockCfg.NumberOfWriteCycles) || (blockCfg.NumberOfWriteCycles > blockInfoPtr->BlockWriteCycle))
        {
            Ea_State                                  = MEMIF_BUSY;
            Ea_RunTimeData.LastJobResult              = MEMIF_JOB_PENDING;
            Ea_RunTimeData.RequestJob                 = EA_JOB_WRITE;
            Ea_RunTimeData.PendReq.BlockNumber        = BlockNumber;
            Ea_RunTimeData.PendReq.BlockIndex         = blockIndex;
            Ea_RunTimeData.PendReq.Length             = blockCfg.BlockSize;
            Ea_RunTimeData.PendReq.DataWriteBufferPtr = DataBufferPtr;
            Ea_RunTimeData.PendReq.InterDataBufferPtr = Ea_ReadWritBuffer;
            if (blockInfoPtr->BlockWriteCycle < EA_MAX_WRITE_CYCLE) /* PRQA S 1252 */ /* VL_QAC_1252 */
            {
                blockInfoPtr->BlockWriteCycle++;
            }
            ret = E_OK;
        }
    }

#if (STD_ON == EA_DEV_ERROR_DETECT)
    if (errorId != EA_E_NO_ERROR)
    {
        Ea_Det_ReportError((uint8)EA_SID_WRITE, errorId);
    }
#endif

    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 2, 0x03, ret);
    return ret;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
/* PRQA S 6030 -- */

#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the module has been initialized and the module is not busy, set the request job to cancel status
 */
void Ea_Cancel(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 1, 0x04);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_Det_ReportError(EA_SID_CANCEL, EA_E_UNINIT);
    }
    else if (MEMIF_BUSY != Ea_State)
    {
        Ea_Det_ReportRunTimeError(EA_SID_CANCEL, EA_E_INVALID_CANCEL);
    }
    else
#endif
    {
        Ea_RunTimeData.RequestJob = EA_JOB_CANCEL;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 1, 0x04);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief return the recorded module status
 */
MemIf_StatusType Ea_GetStatus(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 1, 0x05);
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 2, 0x05, Ea_State);
    return Ea_State;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the module has been initialized, return the job result
 */
MemIf_JobResultType Ea_GetJobResult(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 1, 0x06);
    MemIf_JobResultType ret = MEMIF_JOB_FAILED;
#if (EA_DEV_ERROR_DETECT == STD_ON)
    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_Det_ReportError(EA_SID_GETJOBRESULT, EA_E_UNINIT);
    }
    else
#endif
    {
        ret = Ea_RunTimeData.LastJobResult;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 2, 0x06, ret);
    return ret;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Get the block id value, check the develop error, then put the invalidate request into the queue
 */
Std_ReturnType Ea_InvalidateBlock(uint16 BlockNumber)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 2, 0x07, BlockNumber);
    Std_ReturnType ret        = E_NOT_OK;
    uint16         blockIndex = Ea_GetBlockIndex(BlockNumber);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = EA_E_NO_ERROR;
    if (MEMIF_UNINIT == Ea_State)
    {
        errorId = EA_E_UNINIT;
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_Det_ReportRunTimeError(EA_SID_INVALIDATEBLOCK, EA_E_BUSY);
    }
    else if (EA_BLOCKNUM_INVALID == blockIndex)
    {
        errorId = EA_E_INVALID_BLOCK_NO;
    }
    else
#endif
    {
        Ea_State                                  = MEMIF_BUSY;
        Ea_RunTimeData.LastJobResult              = MEMIF_JOB_PENDING;
        Ea_RunTimeData.RequestJob                 = EA_JOB_INVALIDATE;
        Ea_RunTimeData.PendReq.BlockNumber        = BlockNumber;
        Ea_RunTimeData.PendReq.BlockIndex         = blockIndex;
        Ea_RunTimeData.PendReq.InterDataBufferPtr = Ea_ReadWritBuffer;
        ret                                       = E_OK;
    }
#if (STD_ON == EA_DEV_ERROR_DETECT)
    if (errorId != EA_E_NO_ERROR)
    {
        Ea_Det_ReportError((uint8)EA_SID_INVALIDATEBLOCK, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 2, 0x07, ret);
    return ret;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#if (EA_VERSION_INFO_API == STD_ON)
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the module has been initialized, return the version information
 */
void Ea_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 2, 0x08, VersionInfoPtr);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    /** Development Error Checks */
    if (NULL_PTR == VersionInfoPtr)
    {
        Ea_Det_ReportError(EA_SID_GETVERSIONINFO, EA_E_PARAM_POINTER);
    }
    else
#endif
    {
        VersionInfoPtr->vendorID         = EA_VENDOR_ID;
        VersionInfoPtr->moduleID         = EA_MODULE_ID;
        VersionInfoPtr->sw_major_version = EA_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = EA_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = EA_SW_PATCH_VERSION;
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 1, 0x08);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#endif
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Get the block id value, check the develop error, then put the erase request into the queue
 */
Std_ReturnType Ea_EraseImmediateBlock(uint16 BlockNumber)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 2, 0x09, BlockNumber);
    Std_ReturnType ret        = E_NOT_OK;
    uint16         blockIndex = Ea_GetBlockIndex(BlockNumber);
#if (EA_DEV_ERROR_DETECT == STD_ON)
    uint8 errorId = EA_E_NO_ERROR;
    if (MEMIF_UNINIT == Ea_State)
    {
        errorId = EA_E_UNINIT;
    }
    else if (MEMIF_BUSY == Ea_State)
    {
        Ea_Det_ReportRunTimeError(EA_SID_ERASEIMMEDIATE_BLOCK, EA_E_BUSY);
    }
    else if (EA_BLOCKNUM_INVALID == blockIndex)
    {
        errorId = EA_E_INVALID_BLOCK_NO;
    }
    else if (FALSE == Ea_Config.BlockCfg->BlockCfg[blockIndex].ImmediateData)
    {
        errorId = EA_E_INVALID_BLOCK_NO;
    }
    else
#endif
    {
        Ea_State                           = MEMIF_BUSY;
        Ea_RunTimeData.LastJobResult       = MEMIF_JOB_PENDING;
        Ea_RunTimeData.RequestJob          = EA_JOB_ERASE;
        Ea_RunTimeData.PendReq.BlockNumber = BlockNumber;
        Ea_RunTimeData.PendReq.BlockIndex  = blockIndex;
        ret                                = E_OK;
    }
#if (STD_ON == EA_DEV_ERROR_DETECT)
    if (errorId != EA_E_NO_ERROR)
    {
        Ea_Det_ReportError((uint8)EA_SID_ERASEIMMEDIATE_BLOCK, errorId);
    }
#endif
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 2, 0x09, ret);
    return ret;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the module has been initialized, invokes the interface to notifies the upper-layer application that job is
 * completed
 */
void Ea_JobEndNotification(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 1, 0x10);
#if (EA_DEV_ERROR_DETECT == STD_ON)

    if (MEMIF_UNINIT == Ea_State)
    {
        Ea_Det_ReportError(EA_SID_JOBENDNOTIFICATION, EA_E_UNINIT);
    }
    else
#endif
    {
#if (EA_POLLING_MODE == STD_OFF)
        if (MEMIF_JOB_PENDING == Ea_RunTimeData.LastJobResult)
        {
            if (Ea_RunTimeData.RequestJob != EA_JOB_IDLE)
            {
                EA_InterServFunc[Ea_RunTimeData.RequestJob]();
            }
        }
        else
        {
            if (MEMIF_JOB_OK != Ea_RunTimeData.LastJobResult)
            {
                Ea_CallJobErrorNotification();
            }
            else
            {
                Ea_CallJobEndNotification();
            }
        }
#endif
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 1, 0x10);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the status is not IDLE, then the process the job according to the request job
 */
/* PRQA S 3408,1532,1512 ++ */ /* VL_Ea_3408,VL_QAC_OneFunRef,VL_Ea_1512 */
void Ea_MainFunction(void)
{
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Start, 1, 0x12);
    /** MainFunction is only processed if busy */
    if (Ea_State > MEMIF_IDLE)
    {
#if (EA_POLLING_MODE == STD_ON)
        const Ea_VoidFunctionVoidPtr EA_InterServFunc[] = {
            NULL_PTR,           /** EA_JOB_IDLE */
            Ea_InterRead,       /** EA_JOB_READ */
            Ea_InterWrite,      /** EA_JOB_WRITE */
            Ea_InterCancel,     /** EA_JOB_CANCEL */
            Ea_InterInvalidate, /** EA_JOB_INVALIDATE */
            Ea_InterErase,      /** EA_JOB_ERASE */
        };
#endif
        if (Ea_RunTimeData.RequestJob != EA_JOB_IDLE)
        {
            (*EA_InterServFunc[Ea_RunTimeData.RequestJob])();
        }
    }
    ARTI_TRACE_N(USER, AR_CP_BSW_API, Ea, 0, Bsw_Return, 1, 0x12);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
/* PRQA S 3408,1532,1512 -- */

/* ========================================== internal function definition ========================================== */
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Search the block index of all configured blocks
 */
EA_LOCAL uint16 Ea_GetBlockIndex(uint16 BlockNumber)
{
    uint16 ret = EA_BLOCKNUM_INVALID;
    if ((0u != BlockNumber) && (EA_BLOCKNUM_INVALID != BlockNumber))
    {
        const Ea_BlockCfgType* blockCfgPtr = Ea_Config.BlockCfg;
        for (uint16 iloop = 0u; iloop < blockCfgPtr->BlockNum; iloop++)
        {
            if (BlockNumber == blockCfgPtr->BlockCfg[iloop].BlockNumber)
            {
                ret = iloop;
                break;
            }
        }
    }
    return ret;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief According to the job result, set runtime variables and the end callback notification interface is invoked
 */
EA_LOCAL void Ea_JobFinish(MemIf_JobResultType JobResult)
{
    uint16 blockIndex = Ea_RunTimeData.PendReq.BlockIndex;

    Ea_RunTimeData.LastJobResult = JobResult; /** Update the result before notify upper layer */
    switch (JobResult)
    {
    case MEMIF_JOB_OK:
        switch (Ea_RunTimeData.RequestJob)
        {
        case EA_JOB_READ:
        case EA_JOB_WRITE:
            EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_JOB_OK))
            break;
        case EA_JOB_ERASE:
            EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_BLOCK_INCONSISTENT))
            break;
        case EA_JOB_INVALIDATE:
            EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_BLOCK_INVALID))
            break;
        default:
            /*never happen*/
            break;
        }
        Ea_CallJobEndNotification();
        break;
    case MEMIF_JOB_FAILED:
        EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_JOB_FAILED))
        Ea_CallJobErrorNotification();
        break;
    case MEMIF_BLOCK_INCONSISTENT:
        EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_BLOCK_INCONSISTENT))
        Ea_CallJobErrorNotification();
        break;
    case MEMIF_BLOCK_INVALID:
        EA_SET_BLOCK_INFO((blockIndex), (TRUE), (MEMIF_BLOCK_INVALID))
        Ea_CallJobErrorNotification();
        break;
    case MEMIF_JOB_CANCELED:
        if (Ea_RunTimeData.RequestJob != EA_JOB_READ)
        {
            EA_SET_BLOCK_INFO((blockIndex), (FALSE), (MEMIF_BLOCK_INCONSISTENT))
        }
        break;
    default:
        /** never happen */
        break;
    }

    Ea_RunTimeData.RequestJob                 = EA_JOB_IDLE;
    Ea_RunTimeData.SubJobStatus               = EA_SUB_JOB_IDLE;
    Ea_RunTimeData.MemHwAbState               = EA_MEMHWAB_JOB_IDLE;
    Ea_RunTimeData.PendReq.BlockNumber        = 0u;
    Ea_RunTimeData.PendReq.BlockOffset        = 0u;
    Ea_RunTimeData.PendReq.BlockIndex         = EA_BLOCKNUM_INVALID;
    Ea_RunTimeData.PendReq.Length             = 0u;
    Ea_RunTimeData.PendReq.DealOffset         = 0u;
    Ea_RunTimeData.PendReq.DealSize           = 0u;
    Ea_RunTimeData.PendReq.DataBufferPtr      = NULL_PTR;
    Ea_RunTimeData.PendReq.DataWriteBufferPtr = NULL_PTR;
    Ea_RunTimeData.PendReq.InterDataBufferPtr = NULL_PTR;

    Ea_State = MEMIF_IDLE;
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Call the end notification interface if it is configurated
 */
EA_LOCAL void Ea_CallJobEndNotification(void)
{
    if (Ea_Config.NvmJobEndNotification != NULL_PTR)
    {
        Ea_Config.NvmJobEndNotification();
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Call the error notification interface if it is configurated
 */
EA_LOCAL void Ea_CallJobErrorNotification(void)
{
    if (Ea_Config.NvmJobErrorNotification != NULL_PTR)
    {
        Ea_Config.NvmJobErrorNotification();
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Call the finish interface when the task is completed, otherwise request sub read tast, then the request is
 * sent to the bottom level, and if the bottom level processing is complete, the corresponding state is set and the
 * finish interface is called
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Ea_STMIF */
EA_LOCAL void Ea_InterRead(void)
{
    uint16           blockIndex  = Ea_RunTimeData.PendReq.BlockIndex;
    Ea_BlockInfoType blockInfo   = Ea_BlockInfo[blockIndex];
    uint8            deviceIndex = Ea_Config.BlockCfg->BlockCfg[blockIndex].DeviceIndex;
    if ((TRUE == blockInfo.IsOperated) && (MEMIF_JOB_OK != blockInfo.BlockVaild))
    {
        Ea_JobFinish(blockInfo.BlockVaild);
    }
    else
    {
        Ea_InterReadSub();
        Ea_InternalProcess(EA_JOB_READ, blockIndex, deviceIndex);
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
/* PRQA S 6030 -- */

#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Update the corresponding runtime state when a read request is generated
 */
EA_LOCAL void Ea_InterReadSub(void)
{
    if (EA_MEMHWAB_JOB_IDLE == Ea_RunTimeData.MemHwAbState)
    {
        /** first to read header */
        if (EA_SUB_JOB_IDLE == Ea_RunTimeData.SubJobStatus)
        {
            Ea_RunTimeData.PendReq.DealSize = EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.SubJobStatus     = EA_SUB_JOB_READ_HEADER;
            Ea_RunTimeData.MemHwAbState     = EA_MEMHWAB_JOB_ASYNC_READY;
        }
        /** second to read data */
        else if (EA_SUB_JOB_READ_HEADER_OK == Ea_RunTimeData.SubJobStatus)
        {
            uint16 headerAlignLenCal          = EA_HEADER_ALIGNMENT_LENGTH - EA_TOTAL_HEADER_LENGTH;
            Ea_RunTimeData.PendReq.DealOffset = EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.SubJobStatus       = EA_SUB_JOB_READ_DATA;
            if (Ea_RunTimeData.PendReq.Length > headerAlignLenCal)
            {
                Ea_RunTimeData.PendReq.DealSize = Ea_RunTimeData.PendReq.Length - headerAlignLenCal;
                Ea_RunTimeData.MemHwAbState     = EA_MEMHWAB_JOB_ASYNC_READY;
            }
            else
            {
                /** data is already read out */
                Ea_RunTimeData.MemHwAbState = EA_MEMHWAB_JOB_ASYNC_PENDING;
            }
        }
        else
        {
            /** idle */
        }
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Request sub write task, and then the request is sent to the bottom layer. If the bottom layer processing is
 * complete, the corresponding status is set and the finish interface is called
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Ea_STMIF */
EA_LOCAL void Ea_InterWrite(void)
{
    uint16 blockIndex  = Ea_RunTimeData.PendReq.BlockIndex;
    uint8  deviceIndex = Ea_Config.BlockCfg->BlockCfg[blockIndex].DeviceIndex;
    Ea_InterWriteSub(blockIndex);
    Ea_InternalProcess(EA_JOB_WRITE, blockIndex, deviceIndex);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
/* PRQA S 6030 -- */

#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the status of the request bottom layer is idle, Update the corresponding runtime state when a write request
 * is generated, the corresponding value is prepared to write
 */
EA_LOCAL void Ea_InterWriteSub(const uint16 blockIndex)
{
    if (EA_MEMHWAB_JOB_IDLE == Ea_RunTimeData.MemHwAbState)
    {
        /** first to write invalid header */
        if (EA_SUB_JOB_IDLE == Ea_RunTimeData.SubJobStatus)
        {
            Ea_InterPrepareWriteHeader(EA_BLOCK_STATUS_INCONSISTENT);
            Ea_RunTimeData.PendReq.DealSize = EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.MemHwAbState     = EA_MEMHWAB_JOB_ASYNC_READY;
            Ea_RunTimeData.SubJobStatus     = EA_SUB_JOB_INVALID_HEADER;
        }
        /** second to write data */
        else if (EA_SUB_JOB_INVALID_HEADER_OK == Ea_RunTimeData.SubJobStatus)
        {
            Ea_RunTimeData.PendReq.DealOffset = EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.PendReq.DealSize =
                Ea_Config.BlockCfg->BlockCfg[blockIndex].BlockSize - EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.SubJobStatus = EA_SUB_JOB_WRITE_DATA;
            Ea_RunTimeData.MemHwAbState = EA_MEMHWAB_JOB_ASYNC_READY;
            uint16       reqSize        = Ea_RunTimeData.PendReq.DealSize;
            uint8*       destPtr        = &Ea_RunTimeData.PendReq.InterDataBufferPtr[EA_HEADER_ALIGNMENT_LENGTH];
            const uint8* srcPtr =
                &Ea_RunTimeData.PendReq.DataWriteBufferPtr[EA_HEADER_ALIGNMENT_LENGTH - EA_TOTAL_HEADER_LENGTH];
            for (uint16 iloop = 0u; iloop < reqSize; iloop++)
            {
                destPtr[iloop] = srcPtr[iloop];
            }
        }
        /** third to write valid header */
        else if (EA_SUB_JOB_WRITE_DATA_OK == Ea_RunTimeData.SubJobStatus)
        {
            Ea_InterPrepareWriteHeader(EA_BLOCK_STATUS_CONSISTENT);
            Ea_RunTimeData.PendReq.DealOffset = 0u;
            Ea_RunTimeData.PendReq.DealSize   = EA_HEADER_ALIGNMENT_LENGTH;
            Ea_RunTimeData.MemHwAbState       = EA_MEMHWAB_JOB_ASYNC_READY;
            Ea_RunTimeData.SubJobStatus       = EA_SUB_JOB_WRITE_HEADER_VALID;
        }
        else
        {
            /** idle */
        }
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the status of the request bottom layer is idle, the invalid value is ready to write, and then the request
 * is sent to the bottom layer. If the bottom layer processing is complete, the corresponding status is set and the
 * finish interface is called
 */
EA_LOCAL void Ea_InterInvalidate(void)
{
    uint16 blockIndex  = Ea_RunTimeData.PendReq.BlockIndex;
    uint8  deviceIndex = Ea_Config.BlockCfg->BlockCfg[blockIndex].DeviceIndex;
    if (EA_MEMHWAB_JOB_IDLE == Ea_RunTimeData.MemHwAbState)
    {
        Ea_InterPrepareWriteHeader(EA_BLOCK_STATUS_INVALID);
        Ea_RunTimeData.PendReq.DealSize = EA_HEADER_ALIGNMENT_LENGTH;
        Ea_RunTimeData.MemHwAbState     = EA_MEMHWAB_JOB_ASYNC_READY;
    }
    Ea_InternalProcess(EA_JOB_INVALIDATE, blockIndex, deviceIndex);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the status of the request bottom layer is idle, set the size that need to be erased, and then the request
 * is sent to the bottom layer. If the bottom layer processing is complete, the corresponding status is set and the
 * finish interface is called
 */
EA_LOCAL void Ea_InterErase(void)
{
    uint16                          blockIndex  = Ea_RunTimeData.PendReq.BlockIndex;
    const Ea_BlockConfigurationType blockCfg    = Ea_Config.BlockCfg->BlockCfg[blockIndex];
    uint8                           deviceIndex = blockCfg.DeviceIndex;
    if (EA_MEMHWAB_JOB_IDLE == Ea_RunTimeData.MemHwAbState)
    {
        Ea_RunTimeData.PendReq.DealSize = blockCfg.BlockSize;
        Ea_RunTimeData.MemHwAbState     = EA_MEMHWAB_JOB_ASYNC_READY;
    }
    Ea_InternalProcess(EA_JOB_ERASE, blockIndex, deviceIndex);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Read data according to the length of data to be read
 */
EA_LOCAL void Ea_InterCopyReadData(void)
{
    uint16       reqSize = Ea_RunTimeData.PendReq.Length;
    const uint8* srcPtr  = &Ea_RunTimeData.PendReq.InterDataBufferPtr[EA_TOTAL_HEADER_LENGTH];
    uint8*       destPtr = Ea_RunTimeData.PendReq.DataBufferPtr;
    for (uint16 iloop = 0u; iloop < reqSize; iloop++)
    {
        destPtr[iloop] = srcPtr[iloop];
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief After parsing the read alignment data, the interface is invoked based on the corresponding read information
 */
EA_LOCAL void Ea_InterCheckReadHeader(void)
{
    /**  EA block header info
     *  byte0 - byte1: BlockNumber (uint16)
     *  byte2 - byte3: BlockSize (uint16)
     *  byte4 - byte7: BlockWriteCycle (uint32)
     *  byte8 - byte9: BlockValid  0xA5 0xA5 means valid; 0x5A 0x5A means invalid;
     *                   others means inconsistent
     *  byte10 - byte13  BlockStartAddress (uint32)
     *
     *  total header size: 14byte
     */

    uint16                          blockNumber;
    uint16                          blockSize;
    uint32                          blockWriteCycle;
    uint16                          blockValid;
    Ea_AddressType                  blockStartAddress;
    uint16                          blockIndex = Ea_RunTimeData.PendReq.BlockIndex;
    const uint8*                    srcPtr     = Ea_RunTimeData.PendReq.InterDataBufferPtr;
    const Ea_BlockConfigurationType blockCfg   = Ea_Config.BlockCfg->BlockCfg[blockIndex];

    /* PRQA S 2743 ++ */ /* VL_Ea_2743 */
    EA_DESERIALIZE((srcPtr), (blockNumber), uint16)
    EA_DESERIALIZE((srcPtr), (blockSize), uint16)
    EA_DESERIALIZE((srcPtr), (blockWriteCycle), uint32)
    EA_DESERIALIZE((srcPtr), (blockValid), uint16)
    EA_DESERIALIZE((srcPtr), (blockStartAddress), Ea_AddressType) /* PRQA S 2983 */ /* VL_Ea_2983 */
    uint16         blockNumberCfg       = blockCfg.BlockNumber;
    uint16         blockSizeCfg         = blockCfg.BlockSize;
    Ea_AddressType blockStartAddressCfg = blockCfg.BlockStartAddress;
    /* PRQA S 2743 -- */

    if (EA_BLOCK_INVALID == blockValid)
    {
        Ea_JobFinish(MEMIF_BLOCK_INVALID);
    }
    else if (
        (blockNumberCfg != blockNumber) || (blockSizeCfg != blockSize) || (blockStartAddressCfg != blockStartAddress))
    {
        Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
    }
    else if (EA_BLOCK_VALID == blockValid)
    {
        Ea_BlockInfo[blockIndex].BlockWriteCycle = blockWriteCycle;
        Ea_RunTimeData.SubJobStatus              = EA_SUB_JOB_READ_HEADER_OK;
    }
    else
    {
        Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief Prepare the data to be written, align it and place it in the target buffer
 */
EA_LOCAL void Ea_InterPrepareWriteHeader(uint8 status)
{
    /**  EA block header info
     *  byte0 - byte1: BlockNumber (uint16)
     *  byte2 - byte3: BlockSize (uint16)
     *  byte4 - byte7: BlockWriteCycle (uint32)
     *  byte8 - byte9: BlockVaild  0xA5 0xA5 means valid; 0x5A 0x5A means invalid;
     *                  others means inconsistent
     *  byte10 - byte13BlockStartAddress (uint32)
     *
     *  total header size: 14byte
     */
    uint16                          blockIndex = Ea_RunTimeData.PendReq.BlockIndex;
    const Ea_BlockConfigurationType blockCfg   = Ea_Config.BlockCfg->BlockCfg[blockIndex];

    uint16 blockSize  = blockCfg.BlockSize;
    uint16 blockVaild = EA_BLOCK_VALID;

    if (EA_BLOCK_STATUS_INVALID == status)
    {
        blockVaild = EA_BLOCK_INVALID;
    }
    if (EA_BLOCK_STATUS_INCONSISTENT == status)
    {
        blockSize = 0u;
    }

    uint8*         destPtr           = Ea_RunTimeData.PendReq.InterDataBufferPtr;
    uint16         blockNumber       = blockCfg.BlockNumber;
    uint32         blockWriteCycle   = Ea_BlockInfo[blockIndex].BlockWriteCycle;
    Ea_AddressType blockStartAddress = blockCfg.BlockStartAddress;
    /* PRQA S 2743 ++ */ /* VL_Ea_2743 */
    EA_SERIALIZE((blockNumber), (destPtr), uint16)
    EA_SERIALIZE((blockSize), (destPtr), uint16)
    EA_SERIALIZE((blockWriteCycle), (destPtr), uint32)
    EA_SERIALIZE((blockVaild), (destPtr), uint16) /* PRQA S 2906 */ /* VL_Ea_2906 */
    EA_SERIALIZE((blockStartAddress), (destPtr), Ea_AddressType)
    /* PRQA S 2743 -- */
    /** if write job process, copy some write data to buffer*/
    if (NULL_PTR != Ea_RunTimeData.PendReq.DataWriteBufferPtr)
    {
        const uint8* src = Ea_RunTimeData.PendReq.DataWriteBufferPtr;
        for (uint16 iloop = 0u; iloop < (EA_HEADER_ALIGNMENT_LENGTH - EA_TOTAL_HEADER_LENGTH); iloop++)
        {
            destPtr[iloop] = src[iloop];
        }
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the underlying module is in the pengding state, the current task is set to cancelled
 */
EA_LOCAL void Ea_InterCancel(void)
{
    uint16                   blockIndex  = Ea_RunTimeData.PendReq.BlockIndex;
    uint8                    deviceIndex = Ea_Config.BlockCfg->BlockCfg[blockIndex].DeviceIndex;
    uint16                   areaId      = Ea_GetAddrAreaId(blockIndex);
    const Ea_MemHwAbApiType* apiCfgPtr   = &(Ea_Config.MemHwAbApiCfg[deviceIndex]);
    /* Check if the result of the current job is 'PENDING' */
    if (MEMIF_JOB_PENDING == apiCfgPtr->MemHwAbGetJobResultApi(areaId))
    {
        /** Invokes the "Cancel" function of the underlying 'EEPROM' driver module
         * selected by the parameter areaId. */
        apiCfgPtr->MemHwAbCancelApi(areaId);
    }

    Ea_JobFinish(MEMIF_JOB_CANCELED);
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
/**
 * @brief If the underlying state of the request is pengding and the underlying module is idle, set the corresponding
 * state based on the requested task and call the finish interface
 */
EA_LOCAL void Ea_MemAccReq(Ea_RequestJobType Ea_RequestJob)
{
    uint16                          blockIndex    = Ea_RunTimeData.PendReq.BlockIndex;
    const Ea_BlockConfigurationType blockCfg      = Ea_Config.BlockCfg->BlockCfg[blockIndex];
    uint16                          dealOffset    = Ea_RunTimeData.PendReq.DealOffset;
    uint8*                          dataBufferPtr = &(Ea_RunTimeData.PendReq.InterDataBufferPtr[dealOffset]);
    Ea_LengthType                   dealLength    = (Ea_LengthType)Ea_RunTimeData.PendReq.DealSize;
    Ea_AddressType                  dealAddress   = (Ea_AddressType)(dealOffset + blockCfg.BlockStartAddress);
    const uint8                     deviceIndex   = blockCfg.DeviceIndex;

    uint16                   areaId    = Ea_GetAddrAreaId(blockIndex);
    const Ea_MemHwAbApiType* apiCfgPtr = &(Ea_Config.MemHwAbApiCfg[deviceIndex]);

    if (MEMIF_UNINIT == apiCfgPtr->MemHwAbGetStatusApi(areaId))
    {
        Ea_JobFinish(MEMIF_JOB_FAILED); /** Lower layer driver is not initialized */
    }
    else
    {
        if ((EA_MEMHWAB_JOB_ASYNC_READY == Ea_RunTimeData.MemHwAbState)
            && (MEMIF_IDLE == apiCfgPtr->MemHwAbGetStatusApi(areaId))) /* PRQA S 3415 */ /* VL_Ea_3415 */
        {
            Std_ReturnType ret = E_OK;
            switch (Ea_RequestJob)
            {
            case EA_JOB_READ:
                /** Invokes the "Read" function of the underlying 'EEPROM' driver module
                 * selected by the parameter areaId/dealAddress/dataBufferPtr/dealLength. */
                ret = apiCfgPtr->MemHwAbReadApi(areaId, dealAddress, dataBufferPtr, dealLength);
                break;
            case EA_JOB_WRITE:
            case EA_JOB_INVALIDATE:
                /** Invokes the "Write" function of the underlying 'EEPROM' driver module
                 * selected by the parameter areaId/dealAddress/dataBufferPtr/dealLength. */
                ret = apiCfgPtr->MemHwAbWriteApi(areaId, dealAddress, dataBufferPtr, dealLength);
                break;
            case EA_JOB_ERASE:
                /** Invokes the "Erase" function of the underlying 'EEPROM' driver module
                 * selected by the parameter areaId/dealAddress/dealLength. */
                ret = apiCfgPtr->MemHwAbEraseApi(areaId, dealAddress, dealLength);
                break;
            default:
                /** idle */
                break;
            }

            if (E_OK == ret)
            {
                Ea_RunTimeData.MemHwAbState = EA_MEMHWAB_JOB_ASYNC_PENDING;
            }
            else
            {
                Ea_JobFinish(MEMIF_JOB_FAILED);
            }
        }
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
EA_LOCAL void Ea_InternalProcess(Ea_RequestJobType JobType, uint16 blockIndex, uint8 deviceIndex)
{
    Ea_MemAccReq(JobType);
    uint16                  areaId    = Ea_GetAddrAreaId(blockIndex);
    const Ea_MemHwAbApiType apiCfg    = Ea_Config.MemHwAbApiCfg[deviceIndex];
    MemIf_StatusType        jobStatus = apiCfg.MemHwAbGetStatusApi(areaId);

    if ((EA_MEMHWAB_JOB_ASYNC_PENDING == Ea_RunTimeData.MemHwAbState) && (MEMIF_IDLE == jobStatus))
    {
        MemIf_JobResultType retVal = apiCfg.MemHwAbGetJobResultApi(areaId);
        switch (retVal)
        {
        case MEMIF_JOB_OK:
            Ea_InternalProcessOk(JobType);
            break;
        case MEMIF_BLOCK_INCONSISTENT:

            Ea_JobFinish(MEMIF_BLOCK_INCONSISTENT);
            break;
        case MEMIF_JOB_CANCELED:

            Ea_JobFinish(MEMIF_JOB_CANCELED);
            break;
        default:

            Ea_JobFinish(MEMIF_JOB_FAILED);
            break;
        }
        Ea_RunTimeData.MemHwAbState = EA_MEMHWAB_JOB_IDLE;
    }
}
#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
#define EA_START_SEC_CODE
#include "Ea_MemMap.h"
EA_LOCAL void Ea_InternalProcessOk(Ea_RequestJobType JobType)
{
    if (JobType == EA_JOB_WRITE)
    {
        if (EA_SUB_JOB_INVALID_HEADER == Ea_RunTimeData.SubJobStatus)
        {
            /** write invalid header ok */
            Ea_RunTimeData.SubJobStatus = EA_SUB_JOB_INVALID_HEADER_OK;
        }
        else if (EA_SUB_JOB_WRITE_DATA == Ea_RunTimeData.SubJobStatus)
        {
            /** write data ok */
            Ea_RunTimeData.SubJobStatus = EA_SUB_JOB_WRITE_DATA_OK;
        }
        else if (EA_SUB_JOB_WRITE_HEADER_VALID == Ea_RunTimeData.SubJobStatus)
        {
            /** write valid header ok */
            Ea_JobFinish(MEMIF_JOB_OK);
        }
        else
        {
            /** idle,never happen */
        }
    }
    else if (JobType == EA_JOB_READ)
    {
        if (EA_SUB_JOB_READ_HEADER == Ea_RunTimeData.SubJobStatus)
        {
            /** check header */
            Ea_InterCheckReadHeader();
        }
        else if (EA_SUB_JOB_READ_DATA == Ea_RunTimeData.SubJobStatus)
        {
            /** copy data */
            Ea_InterCopyReadData();
            Ea_JobFinish(MEMIF_JOB_OK);
        }
        else
        {
            /** idle,never happen */
        }
    }
    else
    {
        Ea_JobFinish(MEMIF_JOB_OK);
    }
}
/* PRQA S 1503 -- */

#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"
