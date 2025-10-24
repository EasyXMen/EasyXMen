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
 **  @file               : NvM_Inter.c
 **  @author             : peng.wu
 **  @date               : 2024/08/27
 **  @vendor             : isoft
 **  @description        : NVRAM manager internal code
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "MemIf.h"
#include "SchM_NvM.h"
#include "NvM_Inter.h"
#if ((STD_ON == NVM_BSWM_MULTIBLOCK_JOBSTATUS_INFORMATION) || (STD_ON == NVM_BSWM_SINGLEBLOCK_JOBSTATUS_INFORMATION))
#include "BswM_NvM.h"
#endif
#if (STD_ON == NVM_DEM_PRODUCTION_ERROR_DETECT)
#include "Dem.h"
#endif
#if (STD_ON == NVM_INCLUDE_CRC)
#include "Crc.h"
#endif
#if ((STD_ON == NVM_COMPRESSION_ENABLE) || (STD_ON == NVM_CIPHERING_ENABLE))
#include "Csm.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
/**
 * @brief       Save the repaired data
 * @param[in]   DataSourceAddress: The source address of data
 * @param[in]   Length: The length of data
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76709
 */
NVM_LOCAL void NvM_RepireDataSave(const uint8* DataSourceAddress, uint16 Length);

/**
 * @brief       The requested task enters the replication data phase
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76710
 */
NVM_LOCAL void NvM_MemCpyStep(void);

#if (STD_ON == NVM_WRITE_VERIFY)
/**
 * @brief       Phrase one during internal write verify
 * @param[in]   buffPtr: Pointer to data buffer
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76711
 */
NVM_LOCAL void NvM_SuboneInterWriteVerification(const uint8* buffPtr);

/**
 * @brief       Phrase two during internal write verify
 * @param[in]   buffPtr: Pointer to data buffer
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76712
 */
NVM_LOCAL void NvM_SubtwoInterWriteVerification(const uint8* buffPtr);
#endif

/**
 * @brief       Process error when read redundant block
 * @param[in]   CRCResult: The crc result flag
 * @param[in]   StaticIDResult: The staticID result flag
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76713
 */
NVM_LOCAL void
    NvM_ReadErrorHandleRedundant(NvM_CrcStaticIdStatusType CRCResult, NvM_CrcStaticIdStatusType StaticIDResult);

/**
 * @brief       Process error when read block
 * @param[in]   CRCResult: The crc result flag
 * @param[in]   StaticIDResult: The staticID result flag
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76714
 */
NVM_LOCAL void NvM_ReadErrorHandle(NvM_CrcStaticIdStatusType CRCResult, NvM_CrcStaticIdStatusType StaticIDResult);

/**
 * @brief          Internal read block process
 * @param[in]      tempCrc: The crc value
 * @param[out]     tempStaticIdResultPtr: The staticID result flag
 * @param[inout]   tempCrcResultPtr: The crc result flag
 * @reentrant      Non Reentrant
 * @synchronous    TRUE
 * @trace       CPD-76715
 */
NVM_LOCAL void NvM_InterReadBlockSub(
    NvM_CrcStaticIdStatusType* tempCrcResultPtr,
    NvM_CrcStaticIdStatusType* tempStaticIdResultPtr,
    uint32                     tempCrc);

/**
 * @brief       Subfunction of internal WriteAll process
 * @param[in]   intBlockId: The internal block ID
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76716
 */
NVM_LOCAL void NvM_InterWriteAllSub(NvM_BlockIdType intBlockId);

/**
 * @brief       Subfunction of internal write block process
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76717
 */
NVM_LOCAL void NvM_InterWriteBlockSub(void);

/**
 * @brief       Phrase two during internal reading block process
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76718
 */
NVM_LOCAL void NvM_InterReadBlockSubTwo(void);

/**
 * @brief       Phrase one during internal reading block process
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76719
 */
NVM_LOCAL void NvM_InterReadBlockSubOne(void);

/**
 * @brief       Subfunction of internal read rom process
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76720
 */
NVM_LOCAL void NvM_InterReadRomSubDeal(void);

/**
 * @brief       Subfunction of internal read rom process
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76721
 */
NVM_LOCAL void NvM_InterReadRomSubTwo(void);

/**
 * @brief           Subfunction of internal write block process
 * @param[inout]    processOn: The process flag
 * @reentrant       Non Reentrant
 * @synchronous     TRUE
 * @trace       CPD-76722
 */
NVM_LOCAL void NvM_InterWriteBlockProcessSub(boolean* processOn);

/**
 * @brief Internal  read rom process
 * @param[in]       BlockId: The block ID
 * @reentrant       Non Reentrant
 * @synchronous     TRUE
 * @trace       CPD-76723
 */
NVM_LOCAL void NvM_InterReadRomSub(NvM_BlockIdType BlockId);

/**
 * @brief       Internal write block process
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76724
 */
NVM_LOCAL void NvM_InterWriteBlockProcess(void);

/**
 * @brief       Copy data in NvM
 * @param[in]   SrcAddrPtr: Pointer to start address
 * @param[out]  DstAddrPtr: Pointer to end address
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76725
 */
NVM_LOCAL Std_ReturnType NvM_MemCpy(const uint8* SrcAddrPtr, uint8* DstAddrPtr);

/**
 * @brief       Process and record the current operating status
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76726
 */
NVM_LOCAL void NvM_RunTimeHandle(void);

/**
 * @brief       Internal memory copy process
 * @param[in]   SrcAddrPtr: Pointer to start address
 * @param[out]  DstAddrPtr: Pointer to end address
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76727
 */
NVM_LOCAL Std_ReturnType NvM_MemCpySub(const uint8* SrcAddrPtr, uint8* DstAddrPtr);

/**
 * @brief       Internal ReadAll process
 * @param[in]   intBlockId: The internal block ID
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76728
 */
NVM_LOCAL void NvM_InterReadAllSub(NvM_BlockIdType intBlockId);

#if ((STD_ON == NVM_COMPRESSION_ENABLE) || (STD_ON == NVM_CIPHERING_ENABLE))
/**
 * @brief       Perform compression and encryption
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76729
 */
NVM_LOCAL Std_ReturnType NvM_CompressionAndCiphering(void);

/**
 * @brief       Perform decompression and decryption
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76730
 */
NVM_LOCAL Std_ReturnType NvM_DecompressionAndDeciphering(void);
#endif
#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* ============================================ internal data definition ============================================ */

/* ============================================ external data definition ============================================ */
#define NVM_START_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"
#if (STD_ON == NVM_CIPHERING_ENABLE)
/**
 * @brief Data buffer for ciphering
 */
uint8 NvM_CipheringBuffer[NVM_BLOCKMAXLENGTH];
#endif

/**
 * @brief Record the schema information in NvM module
 */
NvM_ModuleType NvM_Module = {
    FALSE,
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
    0u,
#endif
    NVM_JOB_TYPE_NONE,
    NVM_RUNTIME_NONE,
    NVM_MEMIF_JOB_IDLE,
    NVM_JOB_STEP_IDLE};

/**
 * @brief Record the multi-block job information in NvM module
 */
NvM_MultiJobType NvM_MultiJob = {
    FALSE,
    FALSE,
    FALSE,
    NVM_REQ_OK,
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NVM_NONE_SERV_ID,
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    1u,
#endif
#endif
};

/**
 * @brief Record the automated job information in NvM Module
 */
NvM_AtomJobType NvM_AtomJob = {
    0u,
    {NVM_ATOMJOB_MAX_NONE,
     NVM_ATOMJOB_MAX_NONE,
     NVM_ATOMJOB_MAX_NONE,
     NVM_ATOMJOB_MAX_NONE,
     NVM_ATOMJOB_MAX_NONE,
     NVM_ATOMJOB_MAX_NONE}};

/**
 * @brief Record the block-related information currently processed
 */
NvM_CurRunningType NvM_CurRunning = {
    0u,               /*DeviceId*/
    NVM_NONE_SERV_ID, /*ServiceId*/
    0u,               /*Index*/
    0u,               /*FailedNum*/
    0u,               /*NvNum*/
    0u,               /*RomNum*/
    0u,               /*ReadRetryCounter*/
    0u,               /*WriteTimesCounter*/
    0u,               /*RepeatMirrorCounter*/
#if (STD_ON == NVM_CIPHERING_ENABLE)
    0u, /*NvMCsmRetryCounter*/
#endif
    0u,               /*CrcFlag*/
    NVM_REQ_OK,       /*ESingleReqResult*/
    NVM_REQ_OK,       /*EMultiReqResult*/
    NVM_BLOCK_NATIVE, /*ManagementType*/
    NVM_CRC16,        /*CRCType*/
    0u,               /*BlockId*/
    0u,               /*StaticId*/
    0u,               /*BlockIdIndex*/
    0u,               /*AdminFlagGroup*/
    0u,               /*BaseNumber*/
    0u,               /*Length*/
    0u,               /*VerificationSize*/
    0u,               /*VerificationCounter*/
    0u,               /*CrcLength*/
    NULL_PTR,         /*RamAddr*/
    NULL_PTR,         /*RomAddr*/
    NULL_PTR,         /*CrcAddr*/
    0u,               /*Crc*/
    0u,               /*TempCrc*/
    NULL_PTR,         /*NvM_ReadRamBlockFromNvm*/
    NULL_PTR,         /*NvM_WriteRamBlockToNvm*/
    NULL_PTR,         /*InitCallback*/
    NULL_PTR          /*SingleCallback*/
};

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief Management message information of the queue with immediately data
 */
NvM_RoundRobinQueueManageType NvM_ImmedQueueManage = {0x00u, 0x00u, 0x00u};
#else
/**
 * @brief Management message information of the queue without immediately data
 */
NvM_RoundRobinQueueManageType NvM_StandQueueManage = {0x00u, 0x00u, 0x00u};
#endif
#endif
#define NVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief Record the priority list information
 */
NvM_PriTable2QueueType NvM_PriTable2Queue[NVM_PRI_TAB_MAX_NUM][NVM_TOTAL_NUM_DIFF_PRI];
/**
 * @brief Immediate Queue
 */
NvM_ImmedQueueType NvM_ImmedQueue[NVM_SIZE_IMMEDIATE_JOB_QUEUE];
#endif
/**
 * @brief Standard Queue
 */
NvM_StandQueueType NvM_StandQueue[NVM_SIZE_STANDARD_JOB_QUEUE];
#endif
/**
 * @brief Record the management block information
 */
NvM_AdminBlockType NvM_AdminBlock[NVM_BLOCK_NUM_ALL];
#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
/**
 * @brief BUffer of NVRAM block
 */
uint8 NvM_NvDataBuffer[NVM_MAX_LENGTH_NV_BLOCK + NVM_NV_CRC_MAX_LENGTH];

/**
 * @brief Buffer for temporary to store the repair data
 */
uint8 NVM_TemporaryRAMForRepaire[NVM_REDUNDANT_ALL][NVM_MAX_LENGTH_REDUNDANT_BLOCK];
#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
#define NVM_START_SEC_VAR_CLEARED_16
#include "NvM_MemMap.h"
/**
 * @brief BUffer for queue with priority
 */
uint16 NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE];
#define NVM_STOP_SEC_VAR_CLEARED_16
#include "NvM_MemMap.h"
#endif

/* ========================================== external function definition ========================================== */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
/**
 * @brief If the current job step is to calculate crc, set the corresponding flag. Otherwise, restore data from the rom
 * area. Otherwise, set the current job as a failure
 */
void NvM_InterReadRom(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        NvM_InterReadRomSub(NvM_CurRunning.BlockId);
    }
    else
    {
        NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
        if ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRomBlockDataAddress) /* PRQA S 2844 */ /* VL_NvM_2844 */
            && (NvM_CurRunning.RepeatMirrorCounter > 0U))
        {
            if ((Std_ReturnType)E_OK == NvM_MemCpy(NvM_CurRunning.RomAddr, NvM_CurRunning.RamAddr))
            {
                NvM_InterReadRomSubDeal();
            }
        }
        else if (NULL_PTR != NvM_CurRunning.InitCallback)
        {
            NvM_InterReadRomSubTwo();
        }
        else
        {
            /* PRQA S 2824, 2844 ++ */ /* VL_NvM_2824, VL_NvM_2844 */
            NvM_AdminBlock[intBlockId].SingleReqResult = (NvM_CurRunning.ESingleReqResult == NVM_REQ_INTEGRITY_FAILED)
                                                             ? NVM_REQ_INTEGRITY_FAILED
                                                             : NvM_AdminBlock[intBlockId].SingleReqResult;
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NvM_AdminBlock[intBlockId].SingleReqResult, NVM_CRC_DELETE);
            /* PRQA S 2824, 2844 -- */
        }
    }
}

/**
 * @brief The length to be processed by the crc is obtained based on the number of bytes to be processed, and the
 * corresponding interface is called based on the type of computation of the crc
 */
void NvM_InterCalcCrc(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    uint16 crcLength;
#if (STD_ON == NVM_INCLUDE_CRC)
    boolean crcIsFirstCall;
#endif

#if (0xFFFFU != NVM_CRC_NUM_OF_BYTES)
    if (0U == NvM_CurRunning.CrcFlag)
    {
        NvM_CurRunning.CrcFlag = 1U;
#endif
#if (STD_ON == NVM_CIPHERING_ENABLE)
        /* PRQA S 2814, 2844 ++ */ /* VL_NvM_2814, VL_NvM_2844 */
        NvM_CurRunning.Length =
            (NULL_PTR != NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1u].NvMBlockCipheringRef)
                ? (NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1u].NvMBlockCipheringRef->NvMNvBlockNVRAMDataLength)
                : NvM_CurRunning.Length;
        /* PRQA S 2814, 2844 -- */
#endif
        NvM_CurRunning.CrcLength = NvM_CurRunning.Length;
#if (STD_ON == NVM_INCLUDE_CRC)
        crcIsFirstCall = TRUE;
#endif
#if (0xFFFFU != NVM_CRC_NUM_OF_BYTES)
    }
#if (STD_ON == NVM_INCLUDE_CRC)
    else
    {
        crcIsFirstCall = FALSE;
    }
#endif
    if (NVM_CRC_NUM_OF_BYTES < NvM_CurRunning.CrcLength)
    {
        crcLength = NVM_CRC_NUM_OF_BYTES;
    }
    else
#endif
    {
        crcLength = NvM_CurRunning.CrcLength;
#if (0xFFFFU != NVM_CRC_NUM_OF_BYTES)
        NvM_CurRunning.CrcFlag = 0U;
#endif
        if (0U != NvM_AtomJob.Count)
        {
            --NvM_AtomJob.Count;
        }
    }
#if (STD_ON == NVM_INCLUDE_CRC)
    switch (NvM_CurRunning.CRCType)
    {
#if (STD_ON == NVM_CRC_TYPE_8)
    case NVM_CRC8:
        NvM_CurRunning.Crc =
            Crc_CalculateCRC8(NvM_CurRunning.CrcAddr, crcLength, (uint8)NvM_CurRunning.Crc, crcIsFirstCall);
        break;
#endif
#if (STD_ON == NVM_CRC_TYPE_16)
    case NVM_CRC16:
        NvM_CurRunning.Crc =
            Crc_CalculateCRC16(NvM_CurRunning.CrcAddr, crcLength, (uint16)NvM_CurRunning.Crc, crcIsFirstCall);
        break;
#endif
#if (STD_ON == NVM_CRC_TYPE_32)
    case NVM_CRC32:
        NvM_CurRunning.Crc = Crc_CalculateCRC32(NvM_CurRunning.CrcAddr, crcLength, NvM_CurRunning.Crc, crcIsFirstCall);
        break;
#endif
    default:
        /** idle */
        break;
    }
#endif
    NvM_CurRunning.CrcLength -= crcLength;
    NvM_CurRunning.CrcAddr = &NvM_CurRunning.CrcAddr[crcLength]; /* PRQA S 2824 */ /* VL_NvM_2824 */
}

/**
 * @brief If the current job step is copy, the corresponding data is copied. Otherwise, it sends a read request to the
 * underlying memif, invokes the corresponding processing interface, and sets the job flag
 */
void NvM_InterReadBlock(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if (NVM_JOB_STEP_COPY == NvM_Module.JobStep)
    {
#if ((STD_ON == NVM_COMPRESSION_ENABLE) || (STD_ON == NVM_CIPHERING_ENABLE))
        Std_ReturnType retVal;
        retVal = NvM_DecompressionAndDeciphering();
        if (CRYPTO_E_BUSY == retVal)
        {
            /** Repeat again */
        }
        else if (E_OK != retVal)
        {
            /** Decompression or(and) Deciphering failed */
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
        }
        else
#endif
        {
            NvM_MemCpyStep();
        }
    }
    else
    {
        NvM_InterReadBlockSubOne();
        NvM_MemIfReq(NVM_MEMIF_REQ_READ);
        NvM_InterReadBlockSubTwo();
    }
}

/**
 * @brief Initiate a read request to the underlying memif, and invoke the corresponding processing interface; If the
 * current job step is pending and the repeat count is not 0, the corresponding data is copied.
 */
void NvM_InterWriteBlock(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    NvM_MemIfReq(NVM_MEMIF_REQ_WRITE);

#if (STD_ON == NVM_POLLING_MODE)
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
        MemIf_JobResultType retValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        /* PRQA S 2814, 2844, 3120 ++ */ /* VL_NvM_2814, VL_NvM_2844, VL_QAC_MagicNum */
        (*NvM_MemIfAsyncFuncTable[retValue][(uint8)NVM_WRITE_BLOCK_SERV_ID - 6U])();
        /* PRQA S 2814, 2844, 3120 -- */
    }
#endif

    NvM_InterWriteBlockSub();
    NvM_InterWriteBlockProcess();
}

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/**
 * @brief If the current job phase is computing crc, set the corresponding flag bit. If the repeat count is not 0, the
 * corresponding data is copied to the current ram address, otherwise the current job is set to fail
 */
void NvM_InterSetRamStatus(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
    }
    else
    {
        if (NvM_CurRunning.RepeatMirrorCounter > 0U)
        {
            if (E_OK == NvM_MemCpy(NvM_CurRunning.RamAddr, NvM_NvDataBuffer))
            {
                NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
                NvM_Module.JobStep     = NVM_JOB_STEP_CALC_CRC;
                NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
            }
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        }
    }
}
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief If the current job status is suspended and the current block has a default value, set the job result status to
 * suspended and update the corresponding job steps and job requests
 */
void NvM_InterRestoreBlockDefaults(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmInitBlockCallback))
        {
            NvM_AdminBlock[intBlockId].SingleReqResult = NVM_REQ_PENDING;
            /* PRQA S 2844 -- */
            NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM);
        }
    }
}
#endif

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief If the current job is in the pending state, set the corresponding job step and the job state of memif, then
 * send an erase request to the underlying memif and invoke the corresponding job interface
 */
void NvM_InterEraseNvBlock(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    /** whether this func ia active when priority strategy not used */
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        NvM_Module.JobStep       = NVM_JOB_STEP_ERASE_1ST_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
    NvM_MemIfReq(NVM_MEMIF_REQ_ERASE);
#if (STD_ON == NVM_POLLING_MODE)
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
        MemIf_JobResultType retVal = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        /* PRQA S 2814, 2844, 3120 ++ */ /* VL_NvM_2814, VL_NvM_2844, VL_QAC_MagicNum */
        (*NvM_MemIfAsyncFuncTable[retVal][(uint8)NVM_ERASE_NV_BLOCK_SERV_ID - 6U])();
        /* PRQA S 2814, 2844, 3120 -- */
    }
#endif
}
#endif

/**
 * @brief If the current job is in the suspended state, set the corresponding job step and the job state of the memif,
 * and then send the invalidation request to the underlying memif and invoke the corresponding job interface
 */
void NvM_InterInvalidateNvBlock(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        NvM_Module.JobStep       = NVM_JOB_STEP_INVALID_1ST_NV;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
    NvM_MemIfReq(NVM_MEMIF_REQ_INVALID);
#if (STD_ON == NVM_POLLING_MODE)
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
        MemIf_JobResultType retVal = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        /* PRQA S 2814, 2844, 3120 ++ */ /* VL_NvM_2814, VL_NvM_2844, VL_QAC_MagicNum */
        (*NvM_MemIfAsyncFuncTable[retVal][(uint8)NVM_INVALIDATE_NV_BLOCK_SERV_ID - 6U])();
        /* PRQA S 2814, 2844, 3120 -- */
    }
#endif
}
#endif

/**
 * @brief If the current block is the first block, the read job request is directly switched. Otherwise, readall the
 * blocks configured with readall in sequence, and update the corresponding status and job steps
 */
void NvM_InterReadAll(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_SELECTBLOCKFORREADALL))
        && (TRUE == NVM_ISFLAGOFF(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED))
        && (NVM_BLOCK_DATASET != NvM_CurRunning.ManagementType))
#else
    if ((TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, NVM_BLOCK_DESC_SELECTBLOCKFORREADALL))
        && (TRUE == NVM_ISFLAGOFF(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED)))
#endif
    /* PRQA S 2844 -- */
    {
        if (NVM_READALL_FIRST_BLOCKID == NvM_CurRunning.BlockId) /*For MultiJob,start from first block*/
        {
            NvM_AtomJobReq(NVM_ATOMJOB_READBLOCK);
        }
        else if (NVM_READALL_FIRST_BLOCKID < NvM_CurRunning.BlockId)
        {
            if ((NVM_READALL_FIRST_BLOCKID + 1U) == NvM_CurRunning.BlockId)
            {
                NvM_RunTimeHandle();
            }
            else
            {
                /** error block id, do nothing */
            }
            NvM_InterReadAllSub(intBlockId);
        }
        else
        {
            // for qac
        }
    }
    else
    {
        NvM_JobOverSetFlag(NVM_REQ_BLOCK_SKIPPED, NVM_CRC_REMAIN);
    }
}

/**
 * @brief If canceling a write job has been requested, the remaining unwritten blocks are set to cancel, and the
 * corresponding job results and status are updated.
 */
void NvM_InterWriteAll(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
    if (TRUE == NvM_MultiJob.CancelWriteAll)
    {
        NvM_BlockIdType blockId = NvM_CurRunning.BlockId;
        while (NVM_BLOCK_NUM_ALL >= blockId)
        {
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_ENQUEUE);
            NvM_AdminBlock[intBlockId].SingleReqResult = NVM_REQ_CANCELED;
            NvM_AdminBlock[intBlockId].ServiceID       = NVM_NONE_SERV_ID;
            ++blockId;
            ++NvM_CurRunning.BlockId;
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
            NvM_MultiJob.BlockId = NvM_CurRunning.BlockId;
#endif
        }
        if (TRUE == NvM_MultiJob.ID1WriteMark)
        {
            NvM_AdminBlock[0].SingleReqResult = NVM_REQ_CANCELED;
        }
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        NvM_MultiJob.CancelWriteAll = FALSE;
        NvM_MultiJob.Enqueue        = FALSE;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        NvM_MultiJobResultFeedBack(NVM_CANCEL_WRITE_ALL_SERV_ID, NVM_REQ_CANCELED);
        NvM_MultiJob.ServiceId = NVM_NONE_SERV_ID;
#else
        NvM_MultiJobResultFeedBack(NVM_CANCEL_WRITE_ALL_SERV_ID, NVM_REQ_CANCELED);
        NvM_CurRunning.ServiceId = NVM_NONE_SERV_ID;
#endif
        NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;
        NvM_Module.MemIfJobState  = NVM_MEMIF_JOB_IDLE;
        NvM_Module.JobStep        = NVM_JOB_STEP_IDLE;
    }
    else
    {
        NvM_InterWriteAllSub(intBlockId);
    }
}

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/**
 * @brief Check whether firstinitall is configured for all blocks. If FirstinitAll is configured, set and update the
 * corresponding flag bits and job steps according to different block types
 */
void NvM_InterFirstInitAll(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    boolean         isProcessed = FALSE;
    NvM_BlockIdType intBlockId  = NvM_CurRunning.BlockId - 1U;
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if (TRUE
        == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_SELECTBLOCKFORFIRSTINITALL))
    /* PRQA S 2844 -- */
    {
        switch (NvM_CurRunning.ManagementType)
        {
        case NVM_BLOCK_NATIVE:
            if ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRomBlockDataAddress)
                || (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmInitBlockCallback))
            {
                NVM_GetRepeatMirrorOperation();
                NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
                NvM_AtomJobReq(NVM_ATOMJOB_READROM);
            }
            else
            {
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NvM_AtomJobReq(NVM_ATOMJOB_INVALIDATE);
            }
            isProcessed = TRUE;
            break;
        case NVM_BLOCK_REDUNDANT:
            isProcessed = TRUE;
#if (STD_OFF == NVM_DYNAMIC_CONFIGURATION)
            if (NVM_READALL_FIRST_BLOCKID == NvM_CurRunning.BlockId)
            {
                isProcessed = FALSE;
            }
            if (isProcessed == TRUE)
#endif
            {
                if ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRomBlockDataAddress)
                    || (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmInitBlockCallback))
                {
                    NVM_GetRepeatMirrorOperation();
                    NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
                    NvM_AtomJobReq(NVM_ATOMJOB_READROM);
                }
                else
                {
                    NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                    NvM_AtomJobReq(NVM_ATOMJOB_INVALIDATE);
                }
            }
            break;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        case NVM_BLOCK_DATASET:
            NvM_CurRunning.Index     = 0u;
            NvM_CurRunning.FailedNum = 0u;
            NvM_Module.JobStep       = NVM_JOB_STEP_PENDING;
            NvM_AtomJobReq(NVM_ATOMJOB_INVALIDATE);
            isProcessed = TRUE;
            break;
#endif
        default:
            /** idle,never happen */
            break;
        }
    }
    if (isProcessed == FALSE)
    {
        NvM_JobOverSetFlag(NVM_REQ_BLOCK_SKIPPED, NVM_CRC_REMAIN);
    }
}
#endif

#if (STD_ON == NVM_WRITE_VERIFY)
/**
 * @brief If the current job step is to read the first nv or the second nv, the read request is sent to the underlying
 * memif, and the corresponding verification interface is invoked according to the memif job result
 */
void NvM_InterWriteVerification(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    if ((NvM_Module.JobStep == NVM_JOB_STEP_READ_1ST_NV) || (NvM_Module.JobStep == NVM_JOB_STEP_READ_2ND_NV))
    {
        NvM_MemIfReq(NVM_MEMIF_REQ_READ);
        if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
        {
#if (STD_ON == NVM_POLLING_MODE)
            MemIf_JobResultType retVal = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
            if (MEMIF_JOB_OK == retVal)
            {
                NvM_InterWriteVerificationOK();
            }
            else if (MEMIF_JOB_PENDING != retVal)
            {
                /** Update the valid and changed flag bits to off, and update the job request result to not ok */
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_VERIFY_FAILED)
                Dem_SetEventStatus(NVM_E_VERIFY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
            }
            else
            {
                /** idle */
            }
#else
            NvM_InterWriteVerificationOK();
#endif
        }
    }
}

/**
 * @brief If the verified size is smaller than the length of the current block, the check operation is performed.
 * Otherwise, the verification is considered to be complete, and the corresponding status and result are updated
 * according to different block types
 */
void NvM_InterWriteVerificationOK(void)
{
    if ((NvM_CurRunning.VerificationCounter <= NvM_CurRunning.Length) && (NvM_CurRunning.VerificationSize != 0u))
    {
        uint8* buffPtr = NULL_PTR;

#if (STD_ON == NVM_CIPHERING_ENABLE)
        if (NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1u].NvMBlockCipheringRef != NULL_PTR)
        {
            buffPtr = NvM_CipheringBuffer;
        }
        else
#endif
        {
            buffPtr = NvM_CurRunning.RamAddr;
        }

        if ((NvM_CurRunning.Length - NvM_CurRunning.VerificationCounter) >= NvM_CurRunning.VerificationSize)
        {
            NvM_SuboneInterWriteVerification(buffPtr);
        }
        else
        {
            NvM_SubtwoInterWriteVerification(buffPtr);
        }
    }
    else
    {
        if (NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
        {
            if (NVM_JOB_STEP_READ_2ND_NV != NvM_Module.JobStep)
            {
                NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
            }
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
        }

#if ((STD_ON == NVM_DEM_E_HARDWARE) || (STD_ON == NVM_DEM_E_VERIFY_FAILED))
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_PASSED);
#endif
#if (STD_ON == NVM_DEM_E_VERIFY_FAILED)
        Dem_SetEventStatus(NVM_E_VERIFY_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
#endif
    }
}
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief If the current job is to calculate crc, the corresponding job result and tag bit are updated. Otherwise, set
 * the corresponding flag bit, job request, and result based on the block configuration
 */
/* PRQA S 6030 ++ */                              /* VL_MTR_NvM_STMIF */
void NvM_InterValidateAll(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_REPAIR))
        {
            NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_REPAIR);
        }
        /* PRQA S 2844 -- */
        NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
    }
    else
    {
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if ((TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_AUTOVALIDATION))
            && (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID))
            && ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress)
                || (TRUE
                    == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_SYNCMECHANISM))))
        {
            if (TRUE
                == NVM_ISFLAGOFF(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_CALCRAMBLOCKCRC))
            {
                if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_REPAIR))
                {
                    NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_REPAIR);
                }
                /* PRQA S 2844 -- */
                NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_REMAIN);
            }
            else if (NvM_CurRunning.RepeatMirrorCounter > 0U)
            {
                if (E_OK == NvM_MemCpy(NvM_CurRunning.RamAddr, NvM_NvDataBuffer))
                {
                    NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
                    /** After copy nvmbuffer to ram or mirror */
                    NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
                    NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
                }
            }
            else
            {
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_REMAIN);
            }
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_REMAIN);
        }
    }
}
/* PRQA S 6030 -- */
#endif

/**
 * @brief     According to the passed SingleReqResult to notify the job result callback interface, and then according to
 * the status of the passed CrcChange to update the Crc and crcvalid bit, and set the job status, result, and so on
 */
void NvM_JobOverSetFlag(NvM_RequestResultType SingleReqResult, NvM_CrcChangeType CrcChange)
{
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;

    NvM_SingleJobResultFeedBack(NvM_CurRunning.BlockId, SingleReqResult);
    NvM_AdminBlock[intBlockId].ServiceID = NVM_NONE_SERV_ID;
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_ENQUEUE);
#endif

    /** calculate the write once flag for each block */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((NvM_CurRunning.ServiceId != NVM_ERASE_NV_BLOCK_SERV_ID)
        && (NvM_CurRunning.ServiceId != NVM_INVALIDATE_NV_BLOCK_SERV_ID)
        && ((TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEBLOCKONCE))
            && (NVM_REQ_OK == NvM_AdminBlock[intBlockId].SingleReqResult)))
    /* PRQA S 2844 -- */
    {
        NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO);
    }

    if (NVM_CRC_UPDATE == CrcChange)
    {
        uint8 index = NvM_AdminBlock[intBlockId].CurrentIndex;
        if (NvM_BlockDescriptor[intBlockId].NvmBlockCrcBuffAddress != NULL_PTR)
        {
            NvM_BlockDescriptor[intBlockId].NvmBlockCrcBuffAddress[index] = NvM_CurRunning.Crc;
        }
        NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID);
    }
    else if (NVM_CRC_DELETE == CrcChange)
    {
        NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID);
    }
    else
    {
        /** idle */
    }
    NvM_AdminBlock[intBlockId].FlagGroup = NvM_CurRunning.AdminFlagGroup; /* PRQA S 2844 */ /* VL_NvM_2844 */
    NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS);
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_Module.JobStep       = NVM_JOB_STEP_IDLE;
    NvM_AtomJob.Count        = 0u;
}

#if (NVM_ECUC_PARTITION_NUM > 1U)
/**
 * @brief     Check feedback flag of all blocks, and call BswM interface to signal
 */
void NvM_SatelliteMainFunction(void)
{
    for (uint8 blockIndex = 0u; blockIndex < NVM_BLOCK_NUM_ALL; ++blockIndex)
    {
        if (TRUE == NvM_AdminBlock[blockIndex].SatelliteFeedBack)
        {
            NvM_AdminBlock[blockIndex].SatelliteFeedBack = FALSE;
            BswM_NvM_CurrentBlockMode(blockIndex + 1u, NvM_AdminBlock[blockIndex].SingleReqResult);
        }
    }
}
#endif

/**
 * @brief     Sends the single job result of the current block to the BswM module, sets the request result of the
 * current block, and invokes the configured callback interface
 */
void NvM_SingleJobResultFeedBack(NvM_BlockIdType BlockId, NvM_RequestResultType Result)
{
    /** Only for single job */
    NvM_BlockIdType intBlockId = BlockId - 1U;

#if (STD_ON == NVM_BSWM_SINGLEBLOCK_JOBSTATUS_INFORMATION)
    if (NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, NVM_BLOCK_DESC_BSWBLOCKSTATUSINFO))
    {
#if (NVM_ECUC_PARTITION_NUM <= 1U)
        BswM_NvM_CurrentBlockMode(BlockId, Result);
#else
        if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
        {
            NvM_AdminBlock[intBlockId].SatelliteFeedBack = TRUE;
        }
        else
        {
            BswM_NvM_CurrentBlockMode(BlockId, Result);
        }
#endif
    }
#endif

    NvM_AdminBlock[intBlockId].SingleReqResult = Result; /* PRQA S 2844 */ /* VL_NvM_2844 */
    /** CallBack function for BlockID */
    if (TRUE == NVM_ISFLAGOFF(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
    {
        if (NULL_PTR != NvM_CurRunning.SingleCallback)
        {
            NvM_BlockRequestType blockRequest;
            switch (NvM_CurRunning.ServiceId)
            {
            case NVM_READ_BLOCK_SERV_ID:
            case NVM_READ_PRAM_BLOCK_SERV_ID:
                blockRequest = NVM_READ_BLOCK;
                break;
            case NVM_WRITE_BLOCK_SERV_ID:
            case NVM_WRITE_PRAM_BLOCK_SERV_ID:
                blockRequest = NVM_WRITE_BLOCK;
                break;
            case NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID:
            case NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID:
                blockRequest = NVM_RESTORE_BLOCK_DEFAULTS;
                break;
            case NVM_ERASE_NV_BLOCK_SERV_ID:
                blockRequest = NVM_ERASE_NV_BLOCK;
                break;
            case NVM_INVALIDATE_NV_BLOCK_SERV_ID:
                blockRequest = NVM_INVALIDATE_NV_BLOCK;
                break;
            case NVM_READ_ALL_SERV_ID:
                blockRequest = NVM_READ_ALL_BLOCK;
                break;
            default:
                blockRequest = NVM_READ_BLOCK;
                break;
            }
            /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
            (*NvM_CurRunning.SingleCallback)(blockRequest, NvM_AdminBlock[intBlockId].SingleReqResult);
            /* PRQA S 2844 -- */
        }
    }
}

/**
 * @brief     Set the current notification job and notify the BswM module of the results of the current service,
 * invoking the configured multijob callback interface
 */
void NvM_MultiJobResultFeedBack(NvM_ServiceIdType ServiceId, NvM_RequestResultType Result)
{
    NvM_MultiBlockRequestType notiServiceId = NVM_READ_ALL;
    NvM_MultiJob.ReqResult                  = Result;
    switch (ServiceId)
    {
    case NVM_WRITE_ALL_SERV_ID:
        notiServiceId = NVM_WRITE_ALL;
        break;
    case NVM_VALIDATE_ALL_SERV_ID:
        notiServiceId = NVM_VALIDATE_ALL;
        break;
    case NVM_CANCEL_WRITE_ALL_SERV_ID:
        notiServiceId = NVM_CANCEL_WRITE_ALL;
        break;
    default:
        /** NVM_READ_ALL_SERV_ID or other */
        break;
    }
#if (STD_ON == NVM_BSWM_MULTIBLOCK_JOBSTATUS_INFORMATION)
    BswM_NvM_CurrentJobMode(notiServiceId, Result);
#else
    if (NULL_PTR != NvmMultiBlockCallback)
    {
        /** call the callback function to notify the up layer after job processing is over */
        (*NvmMultiBlockCallback)(notiServiceId, Result);
    }
#endif
}

/* ========================================== internal function definition ========================================== */
/**
 * @brief     If crc verification is configured for the current block, the update job step is to compute crc, and the
 * job is to compute crc request. Otherwise, if the service is firstiniall, the corresponding job request is updated;
 * otherwise, the valid and changed are updated to on with the request result of the current block, and the
 * corresponding job result is set
 */
NVM_LOCAL void NvM_InterReadRomSubDeal(void)
{
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_CALCRAMBLOCKCRC))
    /* PRQA S 2844 -- */
    {
        NvM_CurRunning.CrcAddr = NvM_CurRunning.RamAddr;
        NvM_Module.JobStep     = NVM_JOB_STEP_CALC_CRC;
        NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
    }
    else
    {
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
        if (NvM_CurRunning.ServiceId == NVM_FIRSTINIT_ALL_SERV_ID)
        {
            NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
            NvM_AtomJobReq(NVM_ATOMJOB_WRITEBLOCK);
        }
        else
#endif
        {
            NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
            /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
            if ((NvM_CurRunning.ESingleReqResult == NVM_REQ_INTEGRITY_FAILED)
                || (NvM_CurRunning.ESingleReqResult == NVM_REQ_NV_INVALIDATED)
                || (NvM_AdminBlock[intBlockId].SingleReqResult == NVM_REQ_NV_INVALIDATED)
                || (NvM_AdminBlock[intBlockId].SingleReqResult == NVM_REQ_NOT_OK))
            /* PRQA S 2844 -- */
            {
                NvM_JobOverSetFlag(NVM_REQ_RESTORED_FROM_ROM, NVM_CRC_DELETE);
            }
            else
            {
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
            }
        }
    }
}

/**
 * @brief     If the job is FirstInitAll, switch to the write request, otherwise set the corresponding flag bit
 */
NVM_LOCAL void NvM_InterReadRomSub(NvM_BlockIdType BlockId)
{
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
    if (NvM_CurRunning.ServiceId == NVM_FIRSTINIT_ALL_SERV_ID)
    {
        NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
        NvM_AtomJobReq(NVM_ATOMJOB_WRITEBLOCK);
    }
    else
#endif
    {
        NvM_BlockIdType intBlockId = BlockId - 1U;
        NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if ((NvM_CurRunning.ESingleReqResult == NVM_REQ_INTEGRITY_FAILED)
            || (NvM_CurRunning.ESingleReqResult == NVM_REQ_NV_INVALIDATED)
            || (NvM_AdminBlock[intBlockId].SingleReqResult == NVM_REQ_NV_INVALIDATED)
            || (NvM_AdminBlock[intBlockId].SingleReqResult == NVM_REQ_NOT_OK))
        /* PRQA S 2844 -- */
        {
            NvM_JobOverSetFlag(NVM_REQ_RESTORED_FROM_ROM, NVM_CRC_DELETE);
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
        }
    }
}

/**
 * @brief     The corresponding callback request interface is invoked based on the current service ID and the
 * corresponding operation is processed based on the returned value
 */
NVM_LOCAL void NvM_InterReadRomSubTwo(void)
{
    NvM_InitBlockRequestType initBlockRequest = NVM_INIT_READ_BLOCK;
    switch (NvM_CurRunning.ServiceId)
    {
    case NVM_READ_BLOCK_SERV_ID:
        initBlockRequest = NVM_INIT_READ_BLOCK;
        break;
    case NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID:
    case NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID:
        initBlockRequest = NVM_INIT_RESTORE_BLOCK_DEFAULTS;
        break;
    case NVM_READ_ALL_SERV_ID:
        initBlockRequest = NVM_INIT_READ_ALL_BLOCK;
        break;
    case NVM_FIRSTINIT_ALL_SERV_ID:
        initBlockRequest = NVM_INIT_FIRST_INIT_ALL;
        break;
    default:
        /** idle, never happen */
        break;
    }
    if ((Std_ReturnType)E_OK == (*NvM_CurRunning.InitCallback)(initBlockRequest)) /* PRQA S 2814 */ /* VL_NvM_2814 */
    {
        NvM_InterReadRomSubDeal();
    }
    else
    {
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
        if (NvM_CurRunning.ServiceId == NVM_FIRSTINIT_ALL_SERV_ID)
        {
            NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
            NvM_AtomJobReq(NVM_ATOMJOB_INVALIDATE);
        }
        else
#endif
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
    }
}

/**
 * @brief     Perform crc checks and staticId
 */
NVM_LOCAL void NvM_InterWriteBlockProcessSub(boolean* processOn)
{
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_CALCRAMBLOCKCRC))
        && (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_CRCCOMPMECHANISM))
        && (TRUE == NVM_ISFLAGOFF(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR))
        && (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID)))
    /* PRQA S 2844 -- */
    {
        if (NvM_CurRunning.Crc == NvM_CurRunning.TempCrc)
        {
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_REMAIN);
            *processOn = FALSE; /* PRQA S 2814 */ /* VL_NvM_2814 */
        }
    }
    if (*processOn == TRUE)
    {
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if (NVM_CRC16 == NvM_CurRunning.CRCType)
        {
            NvM_NvDataBuffer[NvM_CurRunning.Length] = (uint8)NvM_CurRunning.Crc;
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            NvM_NvDataBuffer[NvM_CurRunning.Length + 1U] = (uint8)(NvM_CurRunning.Crc >> 8U);
            /* PRQA S 3120 -- */
        }
        else if (NVM_CRC32 == NvM_CurRunning.CRCType)
        {
            NvM_NvDataBuffer[NvM_CurRunning.Length] = (uint8)NvM_CurRunning.Crc;
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            NvM_NvDataBuffer[NvM_CurRunning.Length + 1U] = (uint8)(NvM_CurRunning.Crc >> 8U);
            NvM_NvDataBuffer[NvM_CurRunning.Length + 2U] = (uint8)(NvM_CurRunning.Crc >> 16U);
            NvM_NvDataBuffer[NvM_CurRunning.Length + 3U] = (uint8)(NvM_CurRunning.Crc >> 24U);
            /* PRQA S 3120 -- */
        }
        else
        {
            NvM_NvDataBuffer[NvM_CurRunning.Length] = (uint8)(NvM_CurRunning.Crc);
        }
        /* PRQA S 2844 -- */
#if (STD_ON == NVM_STATIC_BLOCKID)
        if (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_STATICBLOCKIDCHECK))
        {
            NvM_WriteStaticBlockID(NvM_NvDataBuffer, NvM_CurRunning.CRCType);
        }
#endif
        NvM_WriteSequenceForManagementType();
    }
}

/**
 * @brief     Check whether crc is used and switch job
 */
NVM_LOCAL void NvM_InterWriteBlockProcess(void)
{
    boolean processOn = TRUE;

    if (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_WRITE_COPY_RESULT))
    {
        NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_WRITE_COPY_RESULT);
        /** Whether the Block use the CRC */
        if (NVM_CRC_NOT_USED != NvM_CurRunning.CRCType)
        {
            NvM_CurRunning.TempCrc = NvM_CurRunning.Crc;
            NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
            NvM_Module.JobStep     = NVM_JOB_STEP_CALC_CRC;
            NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC); /** Shift the job to the CALCCRC */
            processOn = FALSE;
        }
        else
        {
            /** If Block no CRC,write nv directly */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
#if (STD_ON == NVM_STATIC_BLOCKID)
            NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
            if (TRUE
                == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_STATICBLOCKIDCHECK))
            /* PRQA S 2844 -- */
            {
                NvM_WriteStaticBlockID(NvM_NvDataBuffer, NvM_CurRunning.CRCType);
            }
#endif
            NvM_WriteSequenceForManagementType();
        }
    }

    if ((processOn == TRUE) && (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep))
    {
        NvM_InterWriteBlockProcessSub(&processOn);
    }
}

/**
 * @brief     Store the static ID or CRC value
 */
/* PRQA S 3673 ++ */ /* VL_NvM_3673 */
NVM_LOCAL void NvM_InterReadBlockSub(
    NvM_CrcStaticIdStatusType* tempCrcResultPtr,
    NvM_CrcStaticIdStatusType* tempStaticIdResultPtr,
    uint32                     tempCrc)
/* PRQA S 3673 -- */
{
    if (NvM_CurRunning.Crc == tempCrc)
    {
        *tempCrcResultPtr = NVM_CRC_STATICID_OK; /* PRQA S 2814 */ /* VL_NvM_2814 */
    }
    else
    {
        *tempCrcResultPtr = NVM_CRC_STATICID_NOT_OK;
    }
#if (STD_ON == NVM_STATIC_BLOCKID)
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_STATICBLOCKIDCHECK))
    /* PRQA S 2844 -- */
    {
        if (NvM_CurRunning.StaticId == NvM_CurRunning.BlockId)
        {
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
            Dem_SetEventStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_PASSED);
#endif
            *tempStaticIdResultPtr = NVM_CRC_STATICID_OK; /* PRQA S 2814 */ /* VL_NvM_2814 */
            if (NVM_CRC_STATICID_OK == *tempCrcResultPtr)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_COPY;
            }
        }
        else
        {
            *tempStaticIdResultPtr = NVM_CRC_STATICID_NOT_OK;
        }
    }
    else
#else
    NVM_PARAM_UNUSED(tempStaticIdResultPtr);
#endif
        if (NVM_CRC_STATICID_OK == *tempCrcResultPtr)
    {
        NvM_Module.JobStep = NVM_JOB_STEP_COPY;
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_PASSED);
#endif
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
        Dem_SetEventStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
    }
    else
    {
        /*idle*/
    }
}

/**
 * @brief The current job is marked as failed. When the copy operation is completed, the job state is changed to the
 * verification. If the job phase is calculated crc, the calculated crc value is saved to the corresponding buffer
 */
NVM_LOCAL void NvM_InterWriteBlockSub(void)
{
    /** First Step */
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        if (NvM_CurRunning.RepeatMirrorCounter > 0U)
        {
            if (E_OK == NvM_MemCpy(NvM_CurRunning.RamAddr, NvM_NvDataBuffer))
            {
#if ((STD_ON == NVM_COMPRESSION_ENABLE) || (STD_ON == NVM_CIPHERING_ENABLE))
                Std_ReturnType retVal = NvM_CompressionAndCiphering();
                if (CRYPTO_E_BUSY == retVal)
                {
                    /** Repeat again */
                }
                else if (E_OK != retVal)
                {
                    /** Compression or(and) Ciphering failed */
                    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
                }
                else
#endif
                {
                    NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_WRITE_COPY_RESULT);
                }
            }
            else
            {
                /** Repeat again */
            }
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        }
    }
}

/**
 * @brief     Check the memif status and set the corresponding NvM status
 */
NVM_LOCAL void NvM_InterReadBlockSubTwo(void)
{
#if (STD_ON == NVM_POLLING_MODE)
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
        MemIf_JobResultType retValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        /* PRQA S 2814, 2844, 3120 ++ */ /* VL_NvM_2814, VL_NvM_2844, VL_QAC_MagicNum */
        (*NvM_MemIfAsyncFuncTable[retValue][(uint8)NVM_READ_BLOCK_SERV_ID - 6U])();
        /* PRQA S 2814, 2844, 3120 -- */
    }
#endif
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        if ((NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType) && (NvM_CurRunning.NvNum <= NvM_CurRunning.Index))
        {
            NVM_GetRepeatMirrorOperation();
            NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM);
        }
        else
#endif
        {
            NvM_Module.JobStep       = NVM_JOB_STEP_READ_1ST_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        }
    }
}

/**
 * @brief     Determine whether the calculated crc is consistent; Then perform static ID check or CRC static ID check;
 */
NVM_LOCAL void NvM_InterReadBlockSubOne(void)
{
    uint32 tempCrc;

    if ((NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep)
        || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
    {
        if (NVM_CRC16 == NvM_CurRunning.CRCType)
        {
            tempCrc = NvM_NvDataBuffer[NvM_CurRunning.Length]; /* PRQA S 2844 */ /* VL_NvM_2844 */
            /* PRQA S 3120 ++ */                                                 /* VL_QAC_MagicNum */
            tempCrc |= ((uint32)NvM_NvDataBuffer[NvM_CurRunning.Length + 1U]) << 8U;
            /* PRQA S 3120 -- */
        }
        else if (NVM_CRC32 == NvM_CurRunning.CRCType)
        {
            tempCrc = NvM_NvDataBuffer[NvM_CurRunning.Length];
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            tempCrc |= ((uint32)NvM_NvDataBuffer[NvM_CurRunning.Length + 1U]) << 8U;
            tempCrc |= ((uint32)NvM_NvDataBuffer[NvM_CurRunning.Length + 2U]) << 16U;
            tempCrc |= ((uint32)NvM_NvDataBuffer[NvM_CurRunning.Length + 3U]) << 24U;
            /* PRQA S 3120 -- */
        }
        else
        {
            tempCrc = NvM_NvDataBuffer[NvM_CurRunning.Length];
        }
    }

    NvM_CrcStaticIdStatusType tempCrcResult      = NVM_CRC_STATICID_NO_USE;
    NvM_CrcStaticIdStatusType tempStaticIdResult = NVM_CRC_STATICID_NO_USE;
    if (NVM_JOB_STEP_CHECK == NvM_Module.JobStep)
    {
        if (NvM_CurRunning.StaticId == NvM_CurRunning.BlockId)
        {
            tempStaticIdResult = NVM_CRC_STATICID_OK;
            NvM_Module.JobStep = NVM_JOB_STEP_COPY;
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
            Dem_SetEventStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_PASSED);
#endif
        }
        else
        {
            tempStaticIdResult = NVM_CRC_STATICID_NOT_OK;
        }
    }
    else if (
        (NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep)
        || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
    {
        NvM_InterReadBlockSub(&tempCrcResult, &tempStaticIdResult, tempCrc);
    }
    else
    {
        /*idle*/
    }

    /** CRC Inconsist || StaticId error */
    NvM_ReadErrorHandle(tempCrcResult, tempStaticIdResult);
}

/**
 * @brief     Set the ram status, if it is currently the first block, it is set to valid. Otherwise, all other blocks
 * check whether writeall is configured, the write protection and lock status of the block, and send a write job request
 * and update the job status and steps
 */
NVM_LOCAL void NvM_InterWriteAllSub(NvM_BlockIdType intBlockId)
{
#if (STD_OFF == NVM_SET_RAM_BLOCK_STATUS_API)
    /** treat the current block as valid and changed */
    NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
#else
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if (TRUE
        == NVM_ISFLAGOFF(
            NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].FlagGroup,
            (uint8)NVM_BLOCK_DESC_SETRAMSTATUS))
    /* PRQA S 2844 -- */
    {
        /*treat the current block as valid and changed.*/
        NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
    }
#endif
    /** if block1 need to be write,treat the block1 as valid and changed */
    if ((1U == NvM_CurRunning.BlockId)
        && (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress))
    {
        NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID);
        NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED);
    }

    if ((TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR))
        || ((TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID))
            && (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED))
            && (TRUE == NVM_ISFLAGOFF(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO))
            && (TRUE == NVM_ISFLAGOFF(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_BLOCK_LOCKED))
            && (TRUE
                == NVM_ISFLAGON(
                    NvM_BlockDescriptor[intBlockId].FlagGroup,
                    (uint16)NVM_BLOCK_DESC_SELECTBLOCKFORWRITEALL))))
    {
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        if ((NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType) && (NvM_CurRunning.NvNum <= NvM_CurRunning.Index))
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
        else
#endif
        {
            NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
            NvM_AtomJobReq(NVM_ATOMJOB_WRITEBLOCK);
        }
    }
    else
    {
        if (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED))
        {
            NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED);
        }
        NvM_JobOverSetFlag(NVM_REQ_BLOCK_SKIPPED, NVM_CRC_REMAIN);
    }
}

/**
 * @brief     If the mirror function is enabled, the corresponding status and result are set after the replication
 * succeeds. Otherwise, the number of mirror retries is 0 and the result is not ok. If the mirror function is disabled,
 * the corresponding status and result are set after the replication succeeds. Otherwise, the result is not ok.
 */
NVM_LOCAL void NvM_MemCpyStep(void)
{
    NvM_BlockIdType blockId = NvM_CurRunning.BlockId;
    /* PRQA S 2842, 2844 ++ */ /* VL_NvM_2842, VL_NvM_2844 */
    if ((TRUE == NVM_ISFLAGON(NvM_AdminBlock[blockId - 1u].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT))
        && (NvM_CurRunning.RepeatMirrorCounter > 0U))
    /* PRQA S 2842, 2844 -- */
    {
        if (E_OK == NvM_MemCpy(NvM_NvDataBuffer, NvM_CurRunning.RamAddr))
        {
            if (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS))
            {
                NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
                NvM_RepireDataSave(NvM_CurRunning.RamAddr, NvM_CurRunning.Length);
#if (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY)
                Dem_SetEventStatus(NVM_E_LOSS_OF_REDUNDANCY, DEM_EVENT_STATUS_FAILED);
#endif
            }
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
        }
        else if (NvM_CurRunning.RepeatMirrorCounter == 0U)
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        }
        else
        {
            /** idle */
        }
    }
    /* PRQA S 2842, 2844 ++ */ /* VL_NvM_2842, VL_NvM_2844 */
    else if (TRUE == NVM_ISFLAGOFF(NvM_AdminBlock[blockId - 1u].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT))
    /* PRQA S 2842, 2844 -- */
    {
        if (E_OK == NvM_MemCpy(NvM_NvDataBuffer, NvM_CurRunning.RamAddr))
        {
            if (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS))
            {
                NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
                NvM_RepireDataSave(NvM_CurRunning.RamAddr, NvM_CurRunning.Length);
#if (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY)
                Dem_SetEventStatus(NVM_E_LOSS_OF_REDUNDANCY, DEM_EVENT_STATUS_FAILED);
#endif
            }
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
        }
    }
    else
    {
        /** MISRA */
    }
}

#if (STD_ON == NVM_WRITE_VERIFY)
/**
 * @brief     If the values in the nv block buffer and ram buffer are inconsistent, retry the write operation or set the
 * result to not ok. If the data is consistent, the verified number is updated
 */
NVM_LOCAL void NvM_SuboneInterWriteVerification(const uint8* buffPtr)
{
    Std_ReturnType tempStatus = E_OK;

    for (uint16 iLoop = 0u; iLoop < NvM_CurRunning.VerificationSize; ++iLoop)
    {
        /* PRQA S 2824, 2844 ++ */ /* VL_NvM_2824, VL_NvM_2844 */
        if (buffPtr[NvM_CurRunning.VerificationCounter + iLoop]
            != NvM_NvDataBuffer[iLoop + NvM_CurRunning.VerificationCounter])
        /* PRQA S 2824, 2844 -- */
        {
            NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID);
            tempStatus                         = E_NOT_OK;
            NvM_CurRunning.VerificationCounter = 0u;
            if (NvM_CurRunning.WriteTimesCounter > 0U)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_WRITE_COPY_RESULT);
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
                --NvM_AtomJob.Count;
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_VERIFY_FAILED)
                Dem_SetEventStatus(NVM_E_VERIFY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
            }
            break;
        }
    }
    if (E_OK == tempStatus)
    {
        NvM_CurRunning.VerificationCounter = NvM_CurRunning.VerificationCounter + NvM_CurRunning.VerificationSize;
    }
}

/**
 * @brief     If the values in the nv block buffer and ram buffer are inconsistent, retry the write operation or set the
 * result to not ok. The last updated checked number was 0
 */
NVM_LOCAL void NvM_SubtwoInterWriteVerification(const uint8* buffPtr)
{
    boolean needtoReset = TRUE;

    for (uint16 iLoop = 0u; iLoop < (NvM_CurRunning.Length - NvM_CurRunning.VerificationCounter); ++iLoop)
    {
        /* PRQA S 2824, 2844 ++ */ /* VL_NvM_2824, VL_NvM_2844 */
        if (buffPtr[NvM_CurRunning.VerificationCounter + iLoop]
            != NvM_NvDataBuffer[iLoop + NvM_CurRunning.VerificationCounter])
        /* PRQA S 2824, 2844 -- */
        {
            NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_CRC_VALID);
            if (NvM_CurRunning.WriteTimesCounter > 0U)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                --NvM_AtomJob.Count;
                NVM_SETFLAGON(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_WRITE_COPY_RESULT);
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_VERIFY_FAILED)
                Dem_SetEventStatus(NVM_E_VERIFY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
            }
            needtoReset = FALSE;
            break;
        }
    }

    NvM_CurRunning.VerificationCounter = 0u;
    if (TRUE == needtoReset)
    {
        /** Verify is ok, reset the counter */
        NvM_CurRunning.VerificationSize = 0u;
    }
}
#endif

/**
 * @brief     If the first nv block fails to be read and the retry times are not 0, the first nv block continues to be
 * read again. Otherwise, reset the number of retries and read the second nv block. Otherwise, continue to re-read the
 * second nv block. If the rom area is configured, the request to read the rom area is sent. Otherwise the result is set
 * to INTEGRITY_FAILED
 */
NVM_LOCAL void
    NvM_ReadErrorHandleRedundant(NvM_CrcStaticIdStatusType CRCResult, NvM_CrcStaticIdStatusType StaticIDResult)
{
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
    if ((NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep) || (NVM_JOB_STEP_CHECK == NvM_Module.JobStep))
    {
        if (NvM_CurRunning.ReadRetryCounter > 0U)
        {
            --NvM_CurRunning.ReadRetryCounter;
            NvM_Module.JobStep       = NVM_JOB_STEP_READ_1ST_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        }
        else
        {
            /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
            NvM_CurRunning.ReadRetryCounter = NvM_BlockDescriptor[intBlockId].NvMMaxNumOfReadRetries;
            /* PRQA S 2844 -- */
            /** Repeat time is N+1 via code structure */
            NvM_Module.JobStep       = NVM_JOB_STEP_READ_2ND_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            NvM_CurRunning.Index     = 1u;
        }
    }
    else
    {
        if (NvM_CurRunning.ReadRetryCounter > 0U)
        {
            --NvM_CurRunning.ReadRetryCounter;
            NvM_Module.JobStep       = NVM_JOB_STEP_READ_2ND_NV;
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        }
        else if (
            (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmInitBlockCallback))
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_CurRunning.ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
#if (                                                                        \
    (STD_ON == NVM_DEM_E_INTEGRITY_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE) \
    || (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY))
            if (NVM_CRC_STATICID_NOT_OK == CRCResult)
            {
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
                Dem_SetEventStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
                Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY)
                /** Recovery fails if the result is fail */
                if (TRUE == NVM_ISFLAGON(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS))
                {
                    Dem_SetEventStatus(NVM_E_LOSS_OF_REDUNDANCY, DEM_EVENT_STATUS_FAILED);
                }
#endif
            }
#else
            NVM_PARAM_UNUSED(CRCResult);
#endif
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
            if (NVM_CRC_STATICID_NOT_OK == StaticIDResult)
            {
                Dem_SetEventStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_FAILED);
            }
#else
            NVM_PARAM_UNUSED(StaticIDResult);
#endif
            NVM_GetRepeatMirrorOperation();
            NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM);
        }
        else
        {
#if ((STD_ON == NVM_DEM_E_INTEGRITY_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE))
            if (NVM_CRC_STATICID_NOT_OK == CRCResult)
            {
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
                Dem_SetEventStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
                Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
            }
#endif
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
            if (NVM_CRC_STATICID_NOT_OK == StaticIDResult)
            {
                Dem_SetEventStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_FAILED);
            }
#endif
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
        }
    }
}

/**
 * @brief     Continue to retry the corresponding read operation according to different block types. If the configured
 * number of retries is exceeded, the result is INTEGRITY_FAILED
 */
/* PRQA S 6030 ++ */ /* VL_MTR_NvM_STMIF */
NVM_LOCAL void NvM_ReadErrorHandle(NvM_CrcStaticIdStatusType CRCResult, NvM_CrcStaticIdStatusType StaticIDResult)
{
    if ((NVM_JOB_STEP_CHECK == NvM_Module.JobStep) || (NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep)
        || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
    {
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        /** DATASET Block Type */
        if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
        {
            if (NvM_CurRunning.ReadRetryCounter > 0U)
            {
                --NvM_CurRunning.ReadRetryCounter;
                NvM_Module.JobStep       = NVM_JOB_STEP_READ_1ST_NV;
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if ((STD_ON == NVM_DEM_E_INTEGRITY_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE))
                if (NVM_CRC_STATICID_NOT_OK == CRCResult)
                {
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
                    Dem_SetEventStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
                    Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
                }
#endif
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
                if (NVM_CRC_STATICID_NOT_OK == StaticIDResult)
                {
                    Dem_SetEventStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_FAILED);
                }
#endif
                NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
            }
        }
        /** NATIVE Block Type */
        else if (NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
#else
        if (NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
#endif
        {
            NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
            if (NvM_CurRunning.ReadRetryCounter > 0U)
            {
                --NvM_CurRunning.ReadRetryCounter;
                NvM_Module.JobStep       = NVM_JOB_STEP_READ_1ST_NV;
                NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            }
            else
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_CurRunning.ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
#if ((STD_ON == NVM_DEM_E_INTEGRITY_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE))
                if (NVM_CRC_STATICID_NOT_OK == CRCResult)
                {
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
                    Dem_SetEventStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
                    Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
                }
#endif
#if (STD_ON == NVM_DEM_E_WRONG_BLOCK_ID)
                if (NVM_CRC_STATICID_NOT_OK == StaticIDResult)
                {
                    Dem_SetEventStatus(NVM_E_WRONG_BLOCK_ID, DEM_EVENT_STATUS_FAILED);
                }
#endif
                /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                if ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRomBlockDataAddress)
                    || (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmInitBlockCallback))
                /* PRQA S 2844 -- */
                {
                    NVM_GetRepeatMirrorOperation();
                    NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
                    NvM_AtomJobReq(NVM_ATOMJOB_READROM);
                }
                else
                {
                    NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
                }
            }
        }
        /** Redundant Block Type */
        else
        {
            NvM_ReadErrorHandleRedundant(CRCResult, StaticIDResult);
        }
    }
}
/* PRQA S 6030 -- */

/**
 * @brief     Copy the data of the source address to the repair buffer in the Ram area
 */
NVM_LOCAL void NvM_RepireDataSave(const uint8* DataSourceAddress, uint16 Length)
{
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    NvM_BlockIdType repairIndex = NvM_BlockDescriptor[NvM_CurRunning.BlockId - 1U].RepairIndex;
    uint8*          destPtr     = &NVM_TemporaryRAMForRepaire[repairIndex][0u];
    /* PRQA S 2844 -- */
    for (uint16 iLoop = 0u; iLoop < Length; ++iLoop)
    {
        /* PRQA S 2824, 2844 ++ */ /* VL_NvM_2824, VL_NvM_2844 */
        destPtr[iLoop] = DataSourceAddress[iLoop];
        /* PRQA S 2824, 2844 -- */
    }
}

#if ((STD_ON == NVM_COMPRESSION_ENABLE) || (STD_ON == NVM_CIPHERING_ENABLE))
/**
 * @brief     If the compression function is enabled, the corresponding compression callout interface is invoked. If the
 * encryption function is enabled, the data in the current block is encrypted. If the encryption fails, the data is
 * retried according to the number of encryption retries. If the encryption succeeds, the encrypted content is copied to
 * the buffer of the nv block
 */
NVM_LOCAL Std_ReturnType NvM_CompressionAndCiphering(void)
{
    Std_ReturnType  retVal     = E_OK;
    NvM_BlockIdType intBlockId = (NvM_CurRunning.BlockId >= 1u) ? (NvM_CurRunning.BlockId - 1U) : 0u;
#if (STD_ON == NVM_CIPHERING_ENABLE)
    uint32 cipheringOutLength = NVM_BLOCKMAXLENGTH;
#endif
#if (STD_ON == NVM_COMPRESSION_ENABLE)
    uint8  NvM_CompressionBuffer[NVM_BLOCKMAXLENGTH];
    uint16 compressionOutLength = NVM_BLOCKMAXLENGTH;

    /** NvM_CompressionCallout :reserved
    Param1[in]: NvM_NvDataBuffer :Nv data ptr in
    Param2[in]: NvM_CurRunning.Length :Nv data length in
    Param3[out]: NvM_CompressionBuffer :Nv data ptr out
    Param4[out]: compressionOutLength :Nv data length out */
    if (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_BLOCKUSECOMPRESSION))
    {
        retVal = NvM_CompressionCallout(
            NvM_NvDataBuffer,
            NvM_CurRunning.Length,
            NvM_CompressionBuffer,
            &compressionOutLength);
    }
    /** do not deal NvM_CurRunning.Length,because read length is get from configuration not get from calculate, so
     * now Compression not recommended */
    if (E_OK == retVal)
#endif
    {
#if (STD_ON == NVM_CIPHERING_ENABLE)
        if (NvM_BlockDescriptor[intBlockId].NvMBlockCipheringRef != NULL_PTR) /* PRQA S 2844 */ /* VL_NvM_2844 */
        {
            /* PRQA S 2814 ++ */ /* VL_NvM_2814 */
            retVal = Csm_Encrypt(
                NvM_BlockDescriptor[intBlockId].NvMBlockCipheringRef->NvMCsmEncryptJobId,
                CRYPTO_OPERATIONMODE_SINGLECALL,
#if (STD_ON == NVM_COMPRESSION_ENABLE)
                NvM_CompressionBuffer,
                compressionOutLength,
#else
                NvM_NvDataBuffer,
                NvM_CurRunning.Length,
#endif
                NvM_CipheringBuffer,
                &cipheringOutLength);
            /* PRQA S 2814 -- */
            if (retVal == E_OK)
            {
                /** check length */
                if (cipheringOutLength
                    == NvM_BlockDescriptor[intBlockId].NvMBlockCipheringRef->NvMNvBlockNVRAMDataLength)
                {
                    /** copy Ciphering data*/
                    for (uint16 iLoop = 0u; iLoop < cipheringOutLength; ++iLoop)
                    {
                        NvM_NvDataBuffer[iLoop] = NvM_CipheringBuffer[iLoop]; /* PRQA S 2844 */ /* VL_NvM_2844 */
                    }
                    /** update length */
                    NvM_CurRunning.Length = (uint16)cipheringOutLength;
                }
                else
                {
                    retVal = E_NOT_OK;
#if (STD_ON == NVM_DEV_ERROR_DETECT)
                    NvM_Det_ReportError((uint8)NVM_WRITE_BLOCK_SERV_ID, NVM_E_BLOCK_CHIPHER_LENGTH_MISSMATCH);
#endif
                }
            }
            else if (CRYPTO_E_BUSY == retVal)
            {
                if (NvM_CurRunning.NvMCsmRetryCounter > 0u)
                {
                    /** Repeat again */
                    --NvM_CurRunning.NvMCsmRetryCounter;
                }
                else
                {
                    retVal = E_NOT_OK;
                }
            }
            else
            {
                /** deal outside */
            }
        }
        else
        {
            /** This block does not support Encryption */
            retVal = E_OK;
        }

#else
        /** copy Compression data */
        for (uint16 iLoop = 0; iLoop < compressionOutLength; ++iLoop)
        {
            NvM_NvDataBuffer[iLoop] = NvM_CompressionBuffer[iLoop];
        }
#endif
    }
    return retVal;
}

/**
 * @brief     If the decompression function is enabled, the corresponding callout interface is invoked. If decryption is
 * enabled, the data of the current block is decrypted. If decryption fails, the decryption is retried according to the
 * number of decryption retries. If decryption succeeds, the decrypted content is copied to the buffer of the nv block
 */
NVM_LOCAL Std_ReturnType NvM_DecompressionAndDeciphering(void)
{
    Std_ReturnType  retVal     = E_OK;
    NvM_BlockIdType intBlockId = (NvM_CurRunning.BlockId >= 1u) ? (NvM_CurRunning.BlockId - 1U) : 0u;
#if (STD_ON == NVM_CIPHERING_ENABLE)
    uint8  NvM_DecipheringBuffer[NVM_BLOCKMAXLENGTH];
    uint32 decipheringOutLength = NVM_BLOCKMAXLENGTH;
#endif
#if (STD_ON == NVM_COMPRESSION_ENABLE)
    uint8  NvM_DecompressionBuffer[NVM_BLOCKMAXLENGTH];
    uint16 decompressionOutLength = NVM_BLOCKMAXLENGTH;

    /** NvM_DecompressionCallout :reserved
    Param1[in]: NvM_NvDataBuffer :Nv data ptr in
    Param2[in]: NvM_CurRunning.Length :Nv data length in
    Param3[out]: NvM_DecompressionBuffer :Nv data ptr out
    Param4[out]: decompressionOutLength :Nv data length out */
    if (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_BLOCKUSECOMPRESSION))
    {
        retVal = NvM_DecompressionCallout(
            NvM_NvDataBuffer,
            NvM_CurRunning.Length,
            NvM_DecompressionBuffer,
            &decompressionOutLength);
    }
    /** do not deal NvM_CurRunning.Length,because read length is get from configuration not get from calculate, so
     * now decompression not recommended */
    if (E_OK == retVal)
#endif
    {
#if (STD_ON == NVM_CIPHERING_ENABLE)
        if (NvM_BlockDescriptor[intBlockId].NvMBlockCipheringRef != NULL_PTR) /* PRQA S 2844 */ /* VL_NvM_2844 */
        {
            /* PRQA S 2814 ++ */ /* VL_NvM_2814 */
            retVal = Csm_Decrypt(
                NvM_BlockDescriptor[intBlockId].NvMBlockCipheringRef->NvMCsmDecryptJobId,
                CRYPTO_OPERATIONMODE_SINGLECALL,
#if (STD_ON == NVM_COMPRESSION_ENABLE)
                NvM_DecompressionBuffer,
                decompressionOutLength,
#else
                NvM_NvDataBuffer,
                NvM_CurRunning.Length,
#endif
                NvM_DecipheringBuffer,
                &decipheringOutLength);
            /* PRQA S 2814 -- */
            if (retVal == E_OK)
            {
                /** check length */
                if (decipheringOutLength == NvM_BlockDescriptor[intBlockId].NvmNvBlockLength)
                {
                    /** copy Deciphering data */
                    for (uint16 iLoop = 0u; iLoop < decipheringOutLength; ++iLoop)
                    {
                        NvM_NvDataBuffer[iLoop] = NvM_DecipheringBuffer[iLoop]; /* PRQA S 2844 */ /* VL_NvM_2844 */
                    }
                    /** update length */
                    NvM_CurRunning.Length = (uint16)decipheringOutLength;
                }
                else
                {
                    retVal = E_NOT_OK;
#if (STD_ON == NVM_DEV_ERROR_DETECT)
                    NvM_Det_ReportError((uint8)NVM_READ_BLOCK_SERV_ID, NVM_E_BLOCK_CHIPHER_LENGTH_MISSMATCH);
#endif
                }
            }
            else if (CRYPTO_E_BUSY == retVal)
            {
                if (NvM_CurRunning.NvMCsmRetryCounter > 0u)
                {
                    /** Repeat again */
                    --NvM_CurRunning.NvMCsmRetryCounter;
                }
                else
                {
                    retVal = E_NOT_OK;
                }
            }
            else
            {
                /** deal outside */
            }
        }
        else
        {
            /** This block does not support decryption */
            retVal = E_OK;
        }

#else
        /** copy Decompression data */
        for (uint16 iLoop = 0; iLoop < decompressionOutLength; ++iLoop)
        {
            NvM_NvDataBuffer[iLoop] = NvM_DecompressionBuffer[iLoop];
        }
#endif
    }
    return retVal;
}
#endif

/**
 * @brief     Copy specified length data from one memory to another
 */
/* PRQA S 6030 ++ */ /* VL_MTR_NvM_STMIF */
NVM_LOCAL Std_ReturnType NvM_MemCpy(const uint8* SrcAddrPtr, uint8* DstAddrPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

    if ((SrcAddrPtr != NULL_PTR) && (DstAddrPtr != NULL_PTR))
    {
        NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT))
        /* PRQA S 2844 -- */
        {
            --NvM_CurRunning.RepeatMirrorCounter;
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
            if ((NVM_WRITE_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_WRITE_PRAM_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_WRITE_ALL_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_SET_RAM_BLOCK_STATUS_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_VALIDATE_ALL_SERV_ID == NvM_CurRunning.ServiceId))
#else
            if ((NVM_WRITE_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_WRITE_PRAM_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_WRITE_ALL_SERV_ID == NvM_CurRunning.ServiceId))
#endif
            {
                retVal = NvM_MemCpySub(SrcAddrPtr, DstAddrPtr);
            }
            else if (
                (NVM_READ_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_READ_ALL_SERV_ID == NvM_CurRunning.ServiceId)
                || (NVM_READ_PRAM_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                || (NvM_Module.JobStep == NVM_JOB_STEP_READ_ROM))
            {
                /** Parameter is ok, copy data */
                for (uint32 iLoop = 0u; iLoop < NvM_CurRunning.Length; ++iLoop)
                {
                    DstAddrPtr[iLoop] = SrcAddrPtr[iLoop];
                }
                if (NVM_JOB_STEP_READ_ROM != NvM_Module.JobStep)
                {
                    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                    NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_VALID);
                    NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED);
                    /* PRQA S 2844 -- */
                    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                }
                /* PRQA S 2814, 0314 ++ */ /* VL_NvM_2814, VL_NvM_0314 */
                retVal = (*NvM_CurRunning.NvM_ReadRamBlockFromNvm)((void*)DstAddrPtr);
                /* PRQA S 2814, 0314 -- */
            }
            else if (
                (NVM_FIRSTINIT_ALL_SERV_ID == NvM_CurRunning.ServiceId)
                && ((NvM_CurRunning.ManagementType == NVM_BLOCK_REDUNDANT)
                    || (NvM_CurRunning.ManagementType == NVM_BLOCK_NATIVE))
                && (0U != NvM_AtomJob.Count))
            {
                /** Parameter is ok, copy data */
                for (uint32 iLoop = 0u; iLoop < NvM_CurRunning.Length; ++iLoop)
                {
                    DstAddrPtr[iLoop] = SrcAddrPtr[iLoop];
                }
                retVal = E_OK;
            }
            else
            {
                /** idle */
            }
        }
        else
        {
            /** Parameter is ok, copy data */
            for (uint32 iLoop = 0u; iLoop < NvM_CurRunning.Length; ++iLoop)
            {
                DstAddrPtr[iLoop] = SrcAddrPtr[iLoop];
            }
            retVal = E_OK;
        }
    }
    return retVal;
}
/* PRQA S 6030 -- */

/**
 * @brief     Detect and set module runtime type
 */
NVM_LOCAL void NvM_RunTimeHandle(void)
{
#if (STD_OFF == NVM_DYNAMIC_CONFIGURATION)
    NvM_Module.RuntimeType = NVM_RUNTIME_NORMAL;
#else
    if (((NVM_REQ_OK != NvM_AdminBlock[NVM_READALL_FIRST_BLOCKID - 1U].SingleReqResult)
         && (NVM_REQ_PENDING != NvM_AdminBlock[NVM_READALL_FIRST_BLOCKID - 1U].SingleReqResult))
        || (((((uint16)NVM_COMPILED_CONFIG_ID) & 0x00ffU)
             != *(uint8*)(NvM_BlockDescriptor[NVM_READALL_FIRST_BLOCKID - 1U].NvmRamBlockDataAddress))
            || (((((uint16)NVM_COMPILED_CONFIG_ID) >> 8U) & 0x00ffU)
                != *(uint8*)(NvM_BlockDescriptor[NVM_READALL_FIRST_BLOCKID - 1U].NvmRamBlockDataAddress + 1U))))
    {
        /** the runtime need more information to charge */
        NvM_Module.RuntimeType    = NVM_RUNTIME_EXTENDED;
        NvM_MultiJob.ID1WriteMark = TRUE;
    }
    else
    {
        NvM_Module.RuntimeType = NVM_RUNTIME_NORMAL;
    }
#endif
}

/**
 * @brief     Copy data in the mirror buffer
 */
NVM_LOCAL Std_ReturnType NvM_MemCpySub(const uint8* SrcAddrPtr, uint8* DstAddrPtr)
{
    Std_ReturnType  retVal;
    NvM_BlockIdType intBlockId = NvM_CurRunning.BlockId - 1U;
    /* PRQA S 2814, 0311, 0314 ++ */ /* VL_NvM_2814, VL_NvM_0311, VL_NvM_0314 */
    if (E_OK == (*NvM_CurRunning.NvM_WriteRamBlockToNvm)((void*)SrcAddrPtr))
    /* PRQA S 2814, 0311, 0314 -- */
    {
        /** Parameter is ok, copy data */
        for (uint32 iLoop = 0u; iLoop < NvM_CurRunning.Length; ++iLoop)
        {
            DstAddrPtr[iLoop] = SrcAddrPtr[iLoop]; /* PRQA S 2824 */ /* VL_NvM_2824 */
        }
        retVal = E_OK;
        if ((NVM_SET_RAM_BLOCK_STATUS_SERV_ID != NvM_CurRunning.ServiceId)
            && (NVM_VALIDATE_ALL_SERV_ID != NvM_CurRunning.ServiceId))
        {
            /** Confirm that using the RAM mirror, so the RAM status is used */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
            NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_VALID);
            NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED);
            /* PRQA S 2844 -- */
            NvM_UpdateValidandChangeStatus(STD_ON, STD_ON);
        }
    }
    else
    {
        retVal = E_NOT_OK;
    }
    return retVal;
}

/**
 * @brief     Copy data in the mirror buffer
 */
NVM_LOCAL void NvM_InterReadAllSub(NvM_BlockIdType intBlockId)
{
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((NVM_RUNTIME_EXTENDED == NvM_Module.RuntimeType)
        && (TRUE
            == NVM_ISFLAGOFF(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_RESISTANTTOCHANGEDSW)))
    /* PRQA S 2844 -- */
    {
        NVM_SETFLAGOFF(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO);
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);

        if ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmInitBlockCallback))
        {
            /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
            NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult = NVM_REQ_NOT_OK;
            /* PRQA S 2844 -- */
            NVM_GetRepeatMirrorOperation();
            NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM);
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_REMAIN);
        }
    }
    else
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
        NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
        NvM_AtomJobReq(NVM_ATOMJOB_READBLOCK);
    }
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"
