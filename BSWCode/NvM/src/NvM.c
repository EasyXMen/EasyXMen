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
 **  @file               : NvM.c
 **  @author             : peng.wu
 **  @date               : 2024/08/27
 **  @vendor             : isoft
 **  @description        : NVRAM manager code
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "MemIf.h"
#include "NvM_Inter.h"
#include "NvM_MemIf.h"
#include "SchM_NvM.h"
#if (STD_ON == NVM_DEM_PRODUCTION_ERROR_DETECT)
#include "Dem.h"
#endif
#if (NVM_ECUC_PARTITION_NUM > 1U)
#include "Os.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
/**
 * @brief       Cancel the job in queue
 * @param[in]   BlockId: index of block
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76662
 */
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
NVM_LOCAL Std_ReturnType NvM_CancelJobInQueue(NvM_BlockIdType BlockId);

#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief       Sort the queue in normal priority
 * @param[in]   BlockId: index of block
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76663
 */
NVM_LOCAL Std_ReturnType NvM_NormalQueueSort(NvM_BlockIdType BlockId);
#else
/**
 * @brief       Sort the queue without priority
 * @param[in]   BlockId: index of block
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-78480
 */
NVM_LOCAL Std_ReturnType NvM_QueueSortWithoutPriority(NvM_BlockIdType BlockId);
#endif

/**
 * @brief       Sort the queue in high priority
 * @param[in]   BlockId: index of block
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76664
 */
#if (STD_ON == NVM_JOB_PRIORITIZATION)
NVM_LOCAL Std_ReturnType NvM_ImmedQueueSort(NvM_BlockIdType BlockId);

/**
 * @brief       Find the normal block in the queue
 * @param[in]   blockIdIndex: index of block
 * @param[in]   curActPriTable: currently priority table
 * @param[in]   jobPri: proirity of the job
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76665
 */
NVM_LOCAL void NvM_NormalQueueFindBLockId(const uint8 blockIdIndex, const uint8 curActPriTable, const uint8 jobPri);

/**
 * @brief       Subfunction of sorting the queue in normal priority
 * @param[in]   blockIdIndex: index of block
 * @param[in]   curActPriTable: currently priority table
 * @param[in]   jobPri: proirity of the job
 * @param[in]   preIndex: last table index
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76666
 */
NVM_LOCAL void NvM_NormalQueueSubSort(
    const uint8 blockIdIndex,
    const uint8 curActPriTable,
    const uint8 jobPri,
    const uint8 preIndex);

/**
 * @brief       Internal process of subfunction that sorting the queue in high priority
 * @param[in]   QueueCounter: counter of the queue
 * @param[in]   CurIndex: currently index
 * @param[in]   PreIndex: last table index
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76667
 */
NVM_LOCAL void NvM_ImmedQueueSortSubInternal(uint8 QueueCounter, uint8 CurIndex, uint8 PreIndex);

/**
 * @brief       Subfunction of sorting the queue in high priority
 * @param[in]   blockIdIndex: index of block
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76668
 */
NVM_LOCAL void NvM_ImmedQueueSortSub(uint8 blockIdIndex);
#endif
#endif

#if (STD_OFF == NVM_POLLING_MODE)
/**
 * @brief       Get notifacation job ID
 * @param[in]   JobSuccess: Flag of job result
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76669
 */
NVM_LOCAL uint8 NvM_JobNotiGetId(const boolean JobSuccess);
#endif

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/**
 * @brief       Subfunction of setting block protection
 * @param[in]   BlockId: index of block
 * @param[in]   ProtectionEnabled: Enable protection flag
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76670
 */
NVM_LOCAL Std_ReturnType NvM_SetBlockProtectionSub(NvM_BlockIdType BlockId, boolean ProtectionEnabled);

/**
 * @brief       Subfunction of invalidating block
 * @param[in]   BlockId: index of block
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76671
 */
NVM_LOCAL Std_ReturnType NvM_InvalidateNvBlockSub(NvM_BlockIdType BlockId);

/**
 * @brief       Subfunction of erasing block
 * @param[in]   BlockId: index of block
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76672
 */
NVM_LOCAL Std_ReturnType NvM_EraseNvBlockSub(NvM_BlockIdType BlockId);
#if (STD_ON == NVM_DEV_ERROR_DETECT)
/**
 * @brief       Check develop error in NvM_InvalidateNvBlock
 * @param[in]   intBlockId: index of block
 * @param[out]  errorId: error code
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76673
 */
NVM_LOCAL boolean NvM_InvalidateNvBlockCheck(NvM_BlockIdType intBlockId, uint8* errorId);

/**
 * @brief       Check develop error in NvM_EraseNvBlock
 * @param[in]   intBlockId: index of block
 * @param[out]  errorId: error code
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76674
 */
NVM_LOCAL boolean NvM_EraseNvBlockCheck(NvM_BlockIdType intBlockId, uint8* errorId);
#endif
#endif

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/**
 * @brief       Subfunction of setting Ram block status
 * @param[in]   BlockId: index of block
 * @param[in]   BlockChanged: Block changed flag
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76675
 */
NVM_LOCAL Std_ReturnType NvM_SetRamBlockStatusSub(NvM_BlockIdType BlockId, boolean BlockChanged);
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief       Subfunction of reading block
 * @param[in]   BlockId: index of block
 * @param[in]   readPRAMflag: Read PRAM flag
 * @param[out]  NvM_DstPtr: pointer to end address
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76676
 */
NVM_LOCAL Std_ReturnType NvM_ReadBlockSub(NvM_BlockIdType BlockId, void* NvM_DstPtr, const boolean readPRAMflag);

/**
 * @brief       Subfunction of writing block
 * @param[in]   BlockId: index of block
 * @param[in]   writePRAMflag: Write PRAM flag
 * @param[out]  NvM_SrcPtr: pointer to source address
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76677
 */
NVM_LOCAL Std_ReturnType
    NvM_WriteBlockSub(NvM_BlockIdType BlockId, const uint8* NvM_SrcPtr, const boolean writePRAMflag);

/**
 * @brief       Subfunction of restoring block default
 * @param[in]   BlockId: index of block
 * @param[in]   restorePRAMflag: Restore PRAM flag
 * @param[out]  NvM_DestPtr: pointer to source address
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76678
 */
NVM_LOCAL Std_ReturnType
    NvM_RestoreBlockDefaultsSub(NvM_BlockIdType BlockId, void* NvM_DestPtr, const boolean restorePRAMflag);

#if (STD_ON == NVM_DEV_ERROR_DETECT)
/**
 * @brief       Check develop error in NvM_WriteBlock
 * @param[in]   intBlockId: index of block
 * @param[in]   NvM_SrcPtr: pointer to source address
 * @param[out]  errorId: error code
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-76679
 */
NVM_LOCAL boolean NvM_WriteBlockCheck(NvM_BlockIdType intBlockId, const uint8* NvM_SrcPtr, uint8* errorId);
#endif
#endif
#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* ============================================ internal data definition ============================================ */
#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"
/* PRQA S 3218 ++ */ /* VL_NvM_3218 */
/**
 * @brief Function pointer to internal services @range NA
 */
NVM_LOCAL const NvM_VoidFuncVoidPtr NvM_InterServFuncMatrix[] = {
    NULL_PTR, /** NVM_INIT_SERV_ID */
    NULL_PTR, /** NVM_SET_DATA_INDEX_SERV_ID */
    NULL_PTR, /** NVM_GET_DATA_INDEX_SERV_ID */
    NULL_PTR, /** NVM_SET_BLOCK_PROTECTION_SERV_ID */
    NULL_PTR, /** NVM_GET_ERROR_STATUS_SERV_ID */
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
    &NvM_InterSetRamStatus, /** NVM_SET_RAM_BLOCK_STATUS_SERV_ID */
#else
    NULL_PTR, /** NVM_SET_RAM_BLOCK_STATUS_SERV_ID */
#endif
    &NvM_InterReadBlock,  /** NVM_READ_BLOCK_SERV_ID */
    &NvM_InterWriteBlock, /** NVM_WRITE_BLOCK_SERV_ID */
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    &NvM_InterRestoreBlockDefaults, /** NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID */
#else
    NULL_PTR, /** NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID */
#endif
#if ((NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
    &NvM_InterEraseNvBlock, /** NVM_ERASE_NV_BLOCK_SERV_ID */
#else
    NULL_PTR, /** NVM_ERASE_NV_BLOCK_SERV_ID */
#endif
    NULL_PTR, /** NVM_CANCEL_WRITE_ALL_SERV_ID */
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
    &NvM_InterInvalidateNvBlock, /** NVM_INVALIDATE_NV_BLOCK_SERV_ID */
#else
    NULL_PTR, /** NVM_INVALIDATE_NV_BLOCK_SERV_ID */
#endif
    &NvM_InterReadAll,  /** NVM_READ_ALL_SERV_ID */
    &NvM_InterWriteAll, /** NVM_WRITE_ALL_SERV_ID */
    NULL_PTR,           /** IDLE */
    NULL_PTR,           /** NVM_GET_VERSIONINFO_ID */
    NULL_PTR,           /** NVM_CANCEL_JOBS_SERV_ID */
    NULL_PTR,           /** IDLE */
    NULL_PTR,           /** IDLE */
    NULL_PTR,           /** IDLE */
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
    &NvM_InterFirstInitAll, /** NVM_FIRSTINIT_ALL_SERV_ID */
#else
    NULL_PTR, /** NVM_FIRSTINIT_ALL_SERV_ID */
#endif
    NULL_PTR,             /** IDLE */
    &NvM_InterReadBlock,  /** NVM_READ_PRAM_BLOCK_SERV_ID */
    &NvM_InterWriteBlock, /** NVM_WRITE_PRAM_BLOCK_SERV_ID */
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    &NvM_InterRestoreBlockDefaults, /** NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID */
    &NvM_InterValidateAll,          /** NVM_VALIDATE_ALL_SERV_ID */
#else
    NULL_PTR, /** NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID */
    NULL_PTR  /** NVM_VALIDATE_ALL_SERV_ID */
#endif
};

/**
 * @brief Function pointer to automated task @range NA
 */
NVM_LOCAL const NvM_VoidFuncVoidPtr NvM_AtomJobFuncMatrix[] = {
    &NvM_InterReadBlock,
    &NvM_InterWriteBlock,
    &NvM_InterReadRom,
    &NvM_InterCalcCrc,
#if (STD_ON == NVM_WRITE_VERIFY)
    &NvM_InterWriteVerification,
#else
    NULL_PTR,
#endif
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
    &NvM_InterInvalidateNvBlock
#endif
};
/* PRQA S 3218 -- */
#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definition ========================================== */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
/* PRQA S 1503,1532 ++ */ /* VL_NvM_1503,VL_QAC_OneFunRef */
/**
 * @brief Initialize global variables and all state machines, but reading the block data is not here
 */
void NvM_Init(const NvM_ConfigType* ConfigPtr)
{
#if (NVM_ECUC_PARTITION_NUM > 1U)
    if (TRUE == NvM_CheckMasterEcucPartition())
#endif
    {
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
        /* PRQA S 3120 ++ */                                         /* VL_QAC_MagicNum */
        uint8 queueCount = (NVM_SIZE_STANDARD_JOB_QUEUE - 1U) >> 4U; /** the data used for array,so minus 1 */
        uint8 modCount   = (NVM_SIZE_STANDARD_JOB_QUEUE - 1U) % 16U;
        /* PRQA S 3120 -- */
        NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] = 0x0000u;
        NvM_QueueSpaceTalbe[queueCount]                    = 0x0000u;
        NvM_AtomJob.Count                                  = 0u;

        for (uint16 iLoop = 0u; iLoop <= queueCount; ++iLoop)
        {
#if (NVM_SIZE_STANDARD_JOB_QUEUE > 0x10u)
            if (iLoop < queueCount)
            {
                NvM_QueueSpaceTalbe[iLoop] = 0xFFFFu;
                /** Each bit stand for one element of the NvM_QueueSpaceTalbe[] */
            }
            else
#endif
            {
                for (uint8 index = 0u; index <= modCount; ++index)
                {
                    NvM_QueueSpaceTalbe[queueCount] |= (uint16)((uint16)0x0001u << index);
                }
            }
            NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] |= (uint16)((uint16)0x0001u << iLoop);
            /** The last element is the summary for previous 16 elements, one bit stands for one element */
        }
#endif
        for (uint16 iLoop = 0u; iLoop < NVM_BLOCK_NUM_ALL; ++iLoop)
        {
            NvM_AdminBlock[iLoop].FlagGroup       = 0u;
            NvM_AdminBlock[iLoop].CurrentIndex    = 0u;
            NvM_AdminBlock[iLoop].ServiceID       = NVM_NONE_SERV_ID;
            NvM_AdminBlock[iLoop].SingleReqResult = NVM_REQ_NOT_OK;
#if (NVM_ECUC_PARTITION_NUM > 1U)
            NvM_AdminBlock[iLoop].SatelliteFeedBack = FALSE;
#endif
            NVM_SETFLAGOFF(&NvM_AdminBlock[iLoop].FlagGroup, NVM_ADMIN_READ_REQUEST);

            if (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[iLoop].FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEPROT))
            {
                NVM_SETFLAGON(&NvM_AdminBlock[iLoop].FlagGroup, NVM_ADMIN_NV_WRPRO);
            }
            else
            {
                NVM_SETFLAGOFF(&NvM_AdminBlock[iLoop].FlagGroup, NVM_ADMIN_NV_WRPRO);
            }
        }
        NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
        NvM_Module.Init    = TRUE;

        /** Avoid warning */
        (void)ConfigPtr;
    }
}

#if (STD_ON == NVM_VERSION_INFO_API)
/**
 * @brief Return the version information of this module
 */
void NvM_GetVersionInfo(Std_VersionInfoType* VersionInfo) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        NvM_Det_ReportError((uint8)NVM_GET_VERSIONINFO_ID, NVM_E_PARAM_POINTER);
    }
    else
#endif
    {
        VersionInfo->vendorID         = NVM_VENDOR_ID;
        VersionInfo->moduleID         = NVM_MODULE_ID;
        VersionInfo->sw_major_version = NVM_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = NVM_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = NVM_SW_PATCH_VERSION;
    }
}
#endif

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/**
 * @brief Set/Reset the protection bit of the NV block to implement the write protection service
 */
/* PRQA S 6030 ++ */ /* VL_MTR_NvM_STMIF */
Std_ReturnType NvM_SetBlockProtection(NvM_BlockIdType BlockId, boolean ProtectionEnabled)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1U)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[BlockId - 1U].SingleReqResult == NVM_REQ_PENDING))
    {
        errorId = NVM_E_BLOCK_PENDING;
    }
    else if (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[BlockId - 1U].FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEBLOCKONCE))
    {
        errorId = NVM_E_BLOCK_CONFIG;
    }
    else if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_BLOCK_LOCKED))
    {
        errorId = NVM_E_BLOCK_LOCKED;
    }
    else
#endif
    {
        retVal = NvM_SetBlockProtectionSub(BlockId, ProtectionEnabled);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_SET_BLOCK_PROTECTION_SERV_ID, errorId);
    }
#endif

    return retVal;
}
/* PRQA S 6030 -- */

/**
 * @brief Queue the invalid service requested by the upper layer
 */
Std_ReturnType NvM_InvalidateNvBlock(NvM_BlockIdType BlockId)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1U)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (NvM_InvalidateNvBlockCheck((BlockId - 1U), &errorId) == TRUE)
    {
        /** do nothing and report the recorded error */
    }
    else
#endif
#if ((STD_ON == NVM_DEV_ERROR_DETECT) || (STD_ON == NVM_DEM_E_WRITE_PROTECTED))
        if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_NV_WRPRO))
    {
#if (STD_ON == NVM_DEV_ERROR_DETECT)
        errorId = NVM_E_WRITE_PROTECTED_DET;
#endif
#if (STD_ON == NVM_DEM_E_WRITE_PROTECTED)
        Dem_SetEventStatus(NVM_E_WRITE_PROTECTED, DEM_EVENT_STATUS_FAILED);
#endif
    }
    else
#endif
    {
        retVal = NvM_InvalidateNvBlockSub(BlockId);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_INVALIDATE_NV_BLOCK_SERV_ID, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Queue the erase service requested by the upper layer
 */
Std_ReturnType NvM_EraseNvBlock(NvM_BlockIdType BlockId)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1U)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (NvM_EraseNvBlockCheck((BlockId - 1U), &errorId) == TRUE)
    {
        /** do nothing and report the recorded error */
    }
    else
#endif
#if ((STD_ON == NVM_DEV_ERROR_DETECT) || (STD_ON == NVM_DEM_E_WRITE_PROTECTED))
        if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_NV_WRPRO))
    {
#if (STD_ON == NVM_DEV_ERROR_DETECT)
        errorId = NVM_E_WRITE_PROTECTED_DET;
#endif
#if (STD_ON == NVM_DEM_E_WRITE_PROTECTED)
        Dem_SetEventStatus(NVM_E_WRITE_PROTECTED, DEM_EVENT_STATUS_FAILED);
#endif
    }
    else
#endif
    {
        retVal = NvM_EraseNvBlockSub(BlockId);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_ERASE_NV_BLOCK_SERV_ID, errorId);
    }
#endif

    return retVal;
}
#endif

/**
 * @brief Get the result of a single/multiple request by the BlockId
 */
Std_ReturnType NvM_GetErrorStatus(NvM_BlockIdType BlockId, NvM_RequestResultType* RequestResultPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if (NVM_BLOCK_NUM_ALL < BlockId)
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
    else if (NULL_PTR == RequestResultPtr)
    {
        errorId = NVM_E_PARAM_DATA;
    }
#if (NVM_ECUC_PARTITION_NUM > 1U)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        if (0U == BlockId)
        {
            /** multi job is running but not execute over,return the latest single job result */
            *RequestResultPtr = NvM_MultiJob.ReqResult;
        }
        else
        {
            /** single type job is running */
            *RequestResultPtr = NvM_AdminBlock[BlockId - 1U].SingleReqResult;
        }
        retVal = E_OK;
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_GET_ERROR_STATUS_SERV_ID, errorId);
    }
#endif

    return retVal;
}

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/**
 * @brief Queue the set ram block status service requested by the upper layer
 */
Std_ReturnType NvM_SetRamBlockStatus(NvM_BlockIdType BlockId, boolean BlockChanged)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[BlockId - 1U].SingleReqResult == NVM_REQ_PENDING))
    {
        errorId = NVM_E_BLOCK_PENDING;
    }
    else if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_BLOCK_LOCKED))
    {
        errorId = NVM_E_BLOCK_LOCKED;
    }
    else
#endif
    {
        retVal = NvM_SetRamBlockStatusSub(BlockId, BlockChanged);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_SET_RAM_BLOCK_STATUS_SERV_ID, errorId);
    }
#endif

    return retVal;
}
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief If the DataIndex is valid, then set the DataIndex of a dataset NVRAM block
 */
/* PRQA S 6030 ++ */ /* VL_MTR_NvM_STMIF */
Std_ReturnType NvM_SetDataIndex(NvM_BlockIdType BlockId, uint8 DataIndex)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[BlockId - 1U].SingleReqResult == NVM_REQ_PENDING))
    {
        errorId = NVM_E_BLOCK_PENDING;
    }
    else if (
        (NvM_BlockDescriptor[BlockId - 1U].NvmNvBlockNum + NvM_BlockDescriptor[BlockId - 1U].NvmRomBlockNum)
        <= DataIndex)
    {
        errorId = NVM_E_PARAM_BLOCK_DATA_IDX;
    }
    else
#endif
    {
        if (NVM_BLOCK_DATASET == NvM_BlockDescriptor[BlockId - 1U].NvmBlockManagementType)
        {
            NvM_AdminBlock[BlockId - 1U].CurrentIndex = DataIndex;
            retVal                                    = E_OK;
        }
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_SET_DATA_INDEX_SERV_ID, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief If the NVRAM block type is dataset get the currently DataIndex, otherwise set the DataIndex to 0
 */
Std_ReturnType NvM_GetDataIndex(NvM_BlockIdType BlockId, uint8* DataIndexPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
    else if (NULL_PTR == DataIndexPtr)
    {
        errorId = NVM_E_PARAM_DATA;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        NvM_BlockIdType intBlockId = (NvM_BlockIdType)(BlockId - 1U);
        if (NVM_BLOCK_DATASET == NvM_BlockDescriptor[intBlockId].NvmBlockManagementType)
        {
            *DataIndexPtr = NvM_AdminBlock[intBlockId].CurrentIndex;
            if ((NvM_BlockDescriptor[intBlockId].NvmNvBlockNum + NvM_BlockDescriptor[intBlockId].NvmRomBlockNum)
                > *DataIndexPtr)
            {
                retVal = E_OK;
            }
        }
        else
        {
            *DataIndexPtr = 0u;
        }
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_GET_DATA_INDEX_SERV_ID, errorId);
    }
#endif

    return retVal;
}
/* PRQA S 6030 -- */

/**
 * @brief Queue the read block service requested by the upper layer, and set the valid/changed bit
 */
Std_ReturnType NvM_ReadBlock(NvM_BlockIdType BlockId, void* NvM_DstPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (NULL_PTR == NvM_DstPtr) && (NULL_PTR == NvM_BlockDescriptor[BlockId - 1U].NvmRamBlockDataAddress)
        && (TRUE == NVM_ISFLAGOFF(NvM_BlockDescriptor[BlockId - 1U].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM)))
    {
        errorId = NVM_E_PARAM_ADDRESS;
    }
    else if (
        (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[BlockId - 1U].SingleReqResult == NVM_REQ_PENDING))
    {
        errorId = NVM_E_BLOCK_PENDING;
    }
    else
#endif
    {
        retVal = NvM_ReadBlockSub(BlockId, NvM_DstPtr, FALSE);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_READ_BLOCK_SERV_ID, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Queue the read permanent RAM block service requested by the upper layer, and set the valid/changed bit
 */
Std_ReturnType NvM_ReadPRAMBlock(NvM_BlockIdType BlockId)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (NULL_PTR == NvM_BlockDescriptor[BlockId - 1U].NvmRamBlockDataAddress)
        && (TRUE == NVM_ISFLAGOFF(NvM_BlockDescriptor[BlockId - 1U].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM)))
    {
        errorId = NVM_E_PARAM_ADDRESS;
    }
    else if (
        (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[BlockId - 1U].SingleReqResult == NVM_REQ_PENDING))
    {
        errorId = NVM_E_BLOCK_PENDING;
    }
    else
#endif
    {
        retVal = NvM_ReadBlockSub(BlockId, NULL_PTR, TRUE);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_READ_PRAM_BLOCK_SERV_ID, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Queue the write block service requested by the upper layer, and set the valid/changed bit
 */
Std_ReturnType NvM_WriteBlock(NvM_BlockIdType BlockId, const void* NvM_SrcPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (NvM_WriteBlockCheck(BlockId - 1U, NvM_SrcPtr, &errorId) == TRUE) /* PRQA S 0317 */ /* VL_NvM_0317 */
    {
        /** do nothing and report the recorded error */
    }
    else
#endif
#if ((STD_ON == NVM_DEV_ERROR_DETECT) || (STD_ON == NVM_DEM_E_WRITE_PROTECTED))
        if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_NV_WRPRO))
    {
#if (STD_ON == NVM_DEV_ERROR_DETECT)
        errorId = NVM_E_WRITE_PROTECTED_DET;
#endif
#if (STD_ON == NVM_DEM_E_WRITE_PROTECTED)
        Dem_SetEventStatus(NVM_E_WRITE_PROTECTED, DEM_EVENT_STATUS_FAILED);
#endif
    }
    else
#endif
    {
        retVal = NvM_WriteBlockSub(BlockId, NvM_SrcPtr, FALSE); /* PRQA S 0317 */ /* VL_NvM_0317 */
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_WRITE_BLOCK_SERV_ID, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Queue the write permanent RAM block service requested by the upper layer, and set the valid/changed bit
 */
Std_ReturnType NvM_WritePRAMBlock(NvM_BlockIdType BlockId)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (NvM_WriteBlockCheck(BlockId - 1U, NULL_PTR, &errorId) == TRUE)
    {
        /** do nothing and report the recorded error */
    }
    else
#endif
#if ((STD_ON == NVM_DEV_ERROR_DETECT) || (STD_ON == NVM_DEM_E_WRITE_PROTECTED))
        if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_NV_WRPRO))
    {
#if (STD_ON == NVM_DEV_ERROR_DETECT)
        errorId = NVM_E_WRITE_PROTECTED_DET;
#endif
#if (STD_ON == NVM_DEM_E_WRITE_PROTECTED)
        Dem_SetEventStatus(NVM_E_WRITE_PROTECTED, DEM_EVENT_STATUS_FAILED);
#endif
    }
    else
#endif
    {
        retVal = NvM_WriteBlockSub(BlockId, NULL_PTR, TRUE);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_WRITE_PRAM_BLOCK_SERV_ID, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Queue the restore default data service requested by the upper layer, and set the valid/changed bit
 */
/* PRQA S 6030 ++ */ /* VL_MTR_NvM_STMIF */
Std_ReturnType NvM_RestoreBlockDefaults(NvM_BlockIdType BlockId, void* NvM_DestPtr)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (((NULL_PTR == NvM_DestPtr)
              && ((NULL_PTR == NvM_BlockDescriptor[BlockId - 1U].NvmRamBlockDataAddress)
                  && (TRUE
                      == NVM_ISFLAGOFF(
                          NvM_BlockDescriptor[BlockId - 1U].FlagGroup,
                          (uint8)NVM_BLOCK_DESC_SYNCMECHANISM)))))
    {
        errorId = NVM_E_PARAM_ADDRESS;
    }
    else if (
        (NULL_PTR == NvM_BlockDescriptor[BlockId - 1U].NvmRomBlockDataAddress)
        && (NULL_PTR == NvM_BlockDescriptor[BlockId - 1U].NvmInitBlockCallback))
    {
        errorId = NVM_E_BLOCK_WITHOUT_DEFAULTS;
    }
    else if (
        (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[BlockId - 1U].SingleReqResult == NVM_REQ_PENDING))
    {
        errorId = NVM_E_BLOCK_PENDING;
    }
    else
#endif
    {
        retVal = NvM_RestoreBlockDefaultsSub(BlockId, NvM_DestPtr, FALSE);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID, errorId);
    }
#endif

    return retVal;
}

/**
 * @brief Queue the restore default data to its corresponding Penmanent RAM block service requested by the upper layer,
 * and set the valid/changed bit
 */
Std_ReturnType NvM_RestorePRAMBlockDefaults(NvM_BlockIdType BlockId)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else if (
        (NULL_PTR == NvM_BlockDescriptor[BlockId - 1U].NvmRamBlockDataAddress)
        && (TRUE == NVM_ISFLAGOFF(NvM_BlockDescriptor[BlockId - 1U].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM)))
    {
        errorId = NVM_E_PARAM_ADDRESS;
    }
    else if (
        (NULL_PTR == NvM_BlockDescriptor[BlockId - 1U].NvmRomBlockDataAddress)
        && (NULL_PTR == NvM_BlockDescriptor[BlockId - 1U].NvmInitBlockCallback))
    {
        errorId = NVM_E_BLOCK_WITHOUT_DEFAULTS;
    }
    else if (
        (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[BlockId - 1U].SingleReqResult == NVM_REQ_PENDING))
    {
        errorId = NVM_E_BLOCK_PENDING;
    }
    else
#endif
    {
        retVal = NvM_RestoreBlockDefaultsSub(BlockId, NULL_PTR, TRUE);
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID, errorId);
    }
#endif

    return retVal;
}
/* PRQA S 6030 -- */

/**
 * @brief Queue the validation service requested by the upper layer
 */
void NvM_ValidateAll(void)
{
#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (TRUE != NvM_Module.Init)
    {
        NvM_Det_ReportError((uint8)NVM_VALIDATE_ALL_SERV_ID, NVM_E_UNINIT);
    }
    else
#endif
    {
        if (TRUE != NvM_MultiJob.Enqueue)
        {
            if (E_NOT_OK == NvM_JobEnqueue(0u, NVM_VALIDATE_ALL_SERV_ID, NULL_PTR))
            {
                /** This will not happen */
            }
        }
        else
        {
#if (STD_ON == NVM_DEV_ERROR_DETECT)
            NvM_Det_ReportRunTimeError((uint8)NVM_VALIDATE_ALL_SERV_ID, NVM_E_QUEUE_FULL);
#endif
        }
    }
}

/**
 * @brief If the requested block is in queue, then cancel it
 */
Std_ReturnType NvM_CancelJobs(NvM_BlockIdType BlockId)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else if ((NVM_BLOCK_NUM_ALL < BlockId) || (0U == BlockId))
    {
        errorId = NVM_E_PARAM_BLOCK_ID;
    }
#if (NVM_ECUC_PARTITION_NUM > 1u)
    else if (FALSE == NvM_CheckBlockEcucPartition(BlockId))
    {
        errorId = NVM_E_WRONG_CONDITION;
    }
#endif
    else
#endif
    {
        if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE))
        {
            if (E_OK == NvM_CancelJobInQueue(BlockId))
            {
                retVal = E_OK;
            }
        }
    }

#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_CANCEL_JOBS_SERV_ID, errorId);
    }
#endif

    return retVal;
}
#endif

/**
 * @brief Set the cancelwrite flag if a running Write All request is in queue
 */
void NvM_CancelWriteAll(void)
{
#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (TRUE != NvM_Module.Init)
    {
        NvM_Det_ReportError((uint8)NVM_CANCEL_WRITE_ALL_SERV_ID, NVM_E_UNINIT);
    }
    else
#endif
    {
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
        if ((TRUE == NvM_MultiJob.Enqueue) && (NVM_WRITE_ALL_SERV_ID == NvM_MultiJob.ServiceId))
#else
        if ((TRUE == NvM_MultiJob.Enqueue) && (NVM_WRITE_ALL_SERV_ID == NvM_CurRunning.ServiceId))
#endif
        {
            NvM_MultiJob.CancelWriteAll = TRUE;
        }
    }
}

/**
 * @brief Queue the Read All service if a multi block read is not requested
 */
void NvM_ReadAll(void)
{
#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (TRUE != NvM_Module.Init)
    {
        NvM_Det_ReportError((uint8)NVM_READ_ALL_SERV_ID, NVM_E_UNINIT);
    }
    else
#endif
    {
        if (TRUE != NvM_MultiJob.Enqueue)
        {
            if (E_NOT_OK == NvM_JobEnqueue(0u, NVM_READ_ALL_SERV_ID, NULL_PTR))
            {
                /** This will not happen */
            }
        }
        else
        {
#if (STD_ON == NVM_DEV_ERROR_DETECT)
            NvM_Det_ReportRunTimeError((uint8)NVM_READ_ALL_SERV_ID, NVM_E_QUEUE_FULL);
#endif
        }
    }
}

/**
 * @brief Queue the Write All service if a multi write read is not requested
 */
void NvM_WriteAll(void)
{
#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else
#endif
    {
        boolean errorFlag = FALSE;
        for (uint16 iLoop = 0u; ((iLoop < NVM_BLOCK_NUM_ALL) && (errorFlag == FALSE)); ++iLoop)
        {
            if ((TRUE
                 == NVM_ISFLAGON(NvM_BlockDescriptor[iLoop].FlagGroup, (uint16)NVM_BLOCK_DESC_SELECTBLOCKFORWRITEALL))
                && (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[iLoop].FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEBLOCKONCE))
                && (TRUE == NVM_ISFLAGOFF(NvM_AdminBlock[iLoop].FlagGroup, NVM_ADMIN_READ_REQUEST)))
            {
                errorFlag = TRUE;
            }
        }
        if (errorFlag == FALSE)
        {
            if (TRUE != NvM_MultiJob.Enqueue)
            {
                if (E_OK == NvM_JobEnqueue(0u, NVM_WRITE_ALL_SERV_ID, NULL_PTR))
                {
                    /** Nothing */
                }
            }
            else
            {
#if (STD_ON == NVM_DEV_ERROR_DETECT)
                NvM_Det_ReportRunTimeError((uint8)NVM_WRITE_ALL_SERV_ID, NVM_E_QUEUE_FULL);
#endif
            }
        }
#if (STD_ON == NVM_DEV_ERROR_DETECT)
        else
        {
            errorId = NVM_E_WRITE_ONCE_STATUS_UNKNOWN;
        }
#endif
    }
#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_WRITE_ALL_SERV_ID, errorId);
    }
#endif
}

/**
 * @brief       Service for performing the processing of the NvM jobs
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76680
 */
void NvM_MainFunction(void)
{
    if (TRUE == NvM_Module.Init)
    {
#if (NVM_ECUC_PARTITION_NUM > 1U)
        if (TRUE == NvM_CheckMasterEcucPartition())
#endif
        {
            /** Check whether finished the previous task */
            if (NVM_JOB_STEP_IDLE == NvM_Module.JobStep)
            {
                if (E_OK == NvM_JobDequeue())
                {
                    NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                    /** ServiceID overflow, return to the NVM_JOB_STEP_IDLE */
                    if (NVM_NONE_SERV_ID <= NvM_CurRunning.ServiceId)
                    {
                        NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
                    }
                }
            }

            if ((NVM_JOB_STEP_IDLE != NvM_Module.JobStep) && (0U == NvM_AtomJob.Count))
            {
                /* PRQA S 2814, 2844 ++ */ /* VL_NvM_2814, VL_NvM_2844 */
                if (NvM_InterServFuncMatrix[NvM_CurRunning.ServiceId] != NULL_PTR)
                {
                    (*NvM_InterServFuncMatrix[NvM_CurRunning.ServiceId])();
                }
                /* PRQA S 2814, 2844 -- */
            }

            if (0U != NvM_AtomJob.Count)
            {
                /* PRQA S 2814, 2842,2844 ++ */ /* VL_NvM_2814, VL_NvM_2842,VL_NvM_2844 */
                if (NvM_InterServFuncMatrix[NvM_CurRunning.ServiceId] != NULL_PTR)
                {
                    (*NvM_AtomJobFuncMatrix[NvM_AtomJob.ReqId[NvM_AtomJob.Count]])();
                }
                /* PRQA S 2814, 2842,2844 -- */
            }
            NvM_EMultiReqResultCheck();
        }
#if (NVM_ECUC_PARTITION_NUM > 1U)
        else
        {
            /** Satellite single job feed back process */
            NvM_SatelliteMainFunction();
        }
#endif
    }
}

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/**
 * @brief Queue the first initialization service if no multi jobs are in the queue
 */
void NvM_FirstInitAll(void)
{
#if (STD_ON == NVM_DEV_ERROR_DETECT)
    uint8 errorId = NVM_E_NO_ERROR;

    if (TRUE != NvM_Module.Init)
    {
        errorId = NVM_E_UNINIT;
    }
    else
#endif
    {
        if (TRUE != NvM_MultiJob.Enqueue)
        {
            if (E_OK == NvM_JobEnqueue(0u, NVM_FIRSTINIT_ALL_SERV_ID, NULL_PTR))
            {
                /** Nothing */
            }
        }
#if (STD_ON == NVM_DEV_ERROR_DETECT)
        else
        {
            errorId = NVM_E_BLOCK_PENDING;
        }
#endif
    }
#if (STD_ON == NVM_DEV_ERROR_DETECT)
    if (errorId != NVM_E_NO_ERROR)
    {
        NvM_Det_ReportError((uint8)NVM_FIRSTINIT_ALL_SERV_ID, errorId);
    }
#endif
}
#endif

#if (STD_OFF == NVM_POLLING_MODE)
/**
 * @brief Used by the underlying memory abstraction to signal end of job without error if the service ID is valid
 */
void NvM_JobEndNotification(void)
{
    uint8 serviceId = NvM_JobNotiGetId(TRUE);

    if ((uint8)NVM_NONE_SERV_ID != serviceId)
    {
        /* PRQA S 2814, 2842, 2844 ++ */ /* VL_NvM_2814, VL_NvM_2842, VL_NvM_2844 */
        (*NvM_MemIfAsyncFuncTable[MEMIF_JOB_OK][serviceId])();
        /* PRQA S 2814, 2842, 2844 -- */
        NvM_EMultiReqResultCheck();
    }
}

/**
 * @brief Used by the underlying memory abstraction to signal end of job with error if the service ID is valid
 */
void NvM_JobErrorNotification(void)
{
    uint8 serviceId = NvM_JobNotiGetId(FALSE);
    if ((uint8)NVM_NONE_SERV_ID != serviceId)
    {
        MemIf_JobResultType result = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        /* PRQA S 2814, 2842, 2844 ++ */ /* VL_NvM_2814, VL_NvM_2842, VL_NvM_2844 */
        (*NvM_MemIfAsyncFuncTable[result][serviceId])();
        /* PRQA S 2814, 2842, 2844 -- */
        NvM_EMultiReqResultCheck();
    }
}
#endif

/* ========================================== internal function definition ========================================== */
/**
 * @brief @brief     Depending on whether the block is in the current priority queue or the normal queue, set the
 * corresponding request result to cancel
 */
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
NVM_LOCAL Std_ReturnType NvM_CancelJobInQueue(NvM_BlockIdType BlockId)
{
    Std_ReturnType  retVal = E_NOT_OK;
    Std_ReturnType  normalRet;
    NvM_BlockIdType intBlockId = BlockId - 1U;
#if (STD_ON == NVM_JOB_PRIORITIZATION)
    Std_ReturnType  immedRet;
    NvM_JobTypeType jobType;
    uint8           jobPri = NvM_BlockDescriptor[intBlockId].NvmBlockJobPriority; /* PRQA S 2844 */ /* VL_NvM_2844 */

    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((0U == jobPri)
        && ((NVM_WRITE_PRAM_BLOCK_SERV_ID == NvM_AdminBlock[intBlockId].ServiceID)
            || (NVM_WRITE_BLOCK_SERV_ID == NvM_AdminBlock[intBlockId].ServiceID)))
    /* PRQA S 2844 -- */
    {
        jobType  = NVM_JOB_TYPE_SINGLE_IMMED;
        immedRet = NvM_ImmedQueueSort(BlockId);
    }
    else
    {
        jobType   = NVM_JOB_TYPE_SINGLE_NORMAL;
        normalRet = NvM_NormalQueueSort(BlockId);
    }
    if (((jobType == NVM_JOB_TYPE_SINGLE_IMMED) && (E_OK == immedRet))
        || ((jobType == NVM_JOB_TYPE_SINGLE_NORMAL) && (E_OK == normalRet)))
#else
    normalRet = NvM_QueueSortWithoutPriority(BlockId);
    if (E_OK == normalRet)
#endif
    {
        NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_ENQUEUE); /* PRQA S 2844 */ /* VL_NvM_2844 */
        /* PRQA S 2995 ++ */                                                                        /* VL_NvM_2995 */
        if ((BlockId != NvM_CurRunning.BlockId)
            || ((BlockId == NvM_CurRunning.BlockId) && (NvM_Module.JobStep == NVM_JOB_STEP_IDLE)))
        /* PRQA S 2995 -- */
        {
            NvM_AdminBlock[intBlockId].SingleReqResult = NVM_REQ_CANCELED; /* PRQA S 2844 */ /* VL_NvM_2844 */
            NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_VALID);
            NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED);
        }
        retVal = E_OK;
    }
    return retVal;
}

#if (STD_OFF == NVM_JOB_PRIORITIZATION)
/**
 * @brief     Check whether the current block is in the queue. If it is not, the queue flag is clear. Otherwise, check
 * the position of the current task in the queue and update the corresponding index
 */
NVM_LOCAL Std_ReturnType NvM_QueueSortWithoutPriority(NvM_BlockIdType BlockId)
{
    uint8 curIndex     = 0;
    uint8 blockIdIndex = 0;

    for (; curIndex < NVM_SIZE_STANDARD_JOB_QUEUE; ++curIndex)
    {
        if (NvM_StandQueue[curIndex].BlockId == BlockId)
        {
            blockIdIndex = curIndex;
            break;
        }
    }

    Std_ReturnType retVal = E_OK;
    /** This block takes the last position in the NvM_StandQueueManage */
    if (((0u != NvM_StandQueueManage.TailIndex) && (blockIdIndex == (NvM_StandQueueManage.TailIndex - 1u)))
        || ((0u == NvM_StandQueueManage.TailIndex) && (blockIdIndex == (NVM_SIZE_STANDARD_JOB_QUEUE - 1u))))
    {
        --NvM_StandQueueManage.Count;
        if (0U < blockIdIndex)
        {
            NvM_StandQueueManage.TailIndex = blockIdIndex;
        }
        else
        {
            NvM_StandQueueManage.TailIndex = 0U;
        }
    }
    /** This block takes the middle or fist position of the NvM_StandQueueManage */
    else
    {
        boolean blockFind    = FALSE;
        uint8   queueCounter = 0;
        uint8   preIndex     = 0;
        /** Find the position of the block among the NvM_StandQueueManage.Count */
        curIndex = NvM_StandQueueManage.HeadIndex;
        for (; queueCounter < NvM_StandQueueManage.Count; ++queueCounter)
        {
            if (NvM_StandQueue[curIndex].BlockId == BlockId)
            {
                preIndex = curIndex;
                --NvM_StandQueueManage.Count;
                blockFind = TRUE;
                break;
            }
            ++curIndex;
            if (NVM_SIZE_STANDARD_JOB_QUEUE <= curIndex)
            {
                curIndex = 0;
            }
        }
        if (blockFind == TRUE)
        {
            /** Move forward */
            for (; queueCounter < NvM_StandQueueManage.Count; ++queueCounter)
            {
                ++curIndex;
                if (NVM_SIZE_STANDARD_JOB_QUEUE <= curIndex)
                {
                    curIndex = 0;
                }
                NvM_StandQueue[preIndex].BlockId    = NvM_StandQueue[curIndex].BlockId;
                NvM_StandQueue[preIndex].ServiceId  = NvM_StandQueue[curIndex].ServiceId;
                NvM_StandQueue[preIndex].DestSrcPtr = NvM_StandQueue[curIndex].DestSrcPtr;
                preIndex                            = curIndex;
            }
            if (0u == NvM_StandQueueManage.TailIndex)
            {
                NvM_StandQueueManage.TailIndex = NVM_SIZE_STANDARD_JOB_QUEUE - 1;
            }
            else
            {
                --NvM_StandQueueManage.TailIndex;
            }
        }
        else
        {
            retVal = E_NOT_OK;
        }
    }

    return retVal;
}
#endif

#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief     Find the blockID before this block
 */
NVM_LOCAL void NvM_NormalQueueFindBLockId(const uint8 blockIdIndex, const uint8 curActPriTable, const uint8 jobPri)
{
    /** Only task for this priority */ /* PRQA S 3120 ++ */                       /* VL_QAC_MagicNum */
    if (NvM_StandQueue[blockIdIndex].NextIndex == blockIdIndex) /* PRQA S 2844 */ /* VL_NvM_2844 */
    {
        /** This is the last element, clear priority table flag */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        NvM_PriorityTable[curActPriTable][((jobPri & 0xF0U) >> 4u)] &=
            (uint16)(~(uint16)((uint16)0x01u << (jobPri & 0x0FU)));
        /* PRQA S 2844 -- */
        if (0U == NvM_PriorityTable[curActPriTable][((jobPri & 0xF0U) >> 4u)]) /* PRQA S 2844 */ /* VL_NvM_2844 */
        {
            NvM_PriorityTable[curActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] &=
                (uint16)(~(uint16)((uint16)0x01u << (jobPri & 0xF0U)));
        }
    }
    /* PRQA S 3120 -- */
    else
    {
        /** Shift the first position to the next task */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        NvM_PriTable2Queue[curActPriTable][jobPri].HeadIndex = NvM_StandQueue[blockIdIndex].NextIndex;
        NvM_StandQueue[blockIdIndex].NextIndex               = blockIdIndex;
        /* PRQA S 2844 -- */
    }
}

/**
 * @brief     Sort the queue based on the blocks found
 */
NVM_LOCAL void NvM_NormalQueueSubSort(
    const uint8 blockIdIndex,
    const uint8 curActPriTable,
    const uint8 jobPri,
    const uint8 preIndex)
{
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if (NvM_StandQueue[blockIdIndex].NextIndex == blockIdIndex)
    {
        /** This block is the last task for this priority */
        NvM_StandQueue[preIndex].NextIndex                   = preIndex;
        NvM_PriTable2Queue[curActPriTable][jobPri].TailIndex = preIndex;
    }
    else if (blockIdIndex == preIndex)
    {
        if (blockIdIndex == NvM_PriTable2Queue[curActPriTable][jobPri].TailIndex)
        {
            NvM_StandQueue[NvM_PriTable2Queue[curActPriTable][jobPri].HeadIndex].NextIndex =
                NvM_PriTable2Queue[curActPriTable][jobPri].HeadIndex;
            NvM_PriTable2Queue[curActPriTable][jobPri].TailIndex = NvM_PriTable2Queue[curActPriTable][jobPri].HeadIndex;
        }
        else
        {
            NvM_StandQueue[NvM_PriTable2Queue[curActPriTable][jobPri].HeadIndex].NextIndex =
                NvM_StandQueue[blockIdIndex].NextIndex;
        }
        NvM_StandQueue[blockIdIndex].NextIndex = blockIdIndex;
    }
    else
    {
        NvM_StandQueue[preIndex].NextIndex     = NvM_StandQueue[blockIdIndex].NextIndex;
        NvM_StandQueue[blockIdIndex].NextIndex = blockIdIndex;
    }
    /* PRQA S 2844 -- */
}

/**
 * @brief     Check whether the current block is in the queue. If it is not, the queue flag is clear. Otherwise, check
 * whether the current priority supports multiple tasks or a single task
 */
NVM_LOCAL Std_ReturnType NvM_NormalQueueSort(NvM_BlockIdType BlockId)
{
    uint8 blockIdIndex;
    uint8 curIndex = 0u;
    for (; curIndex < NVM_SIZE_STANDARD_JOB_QUEUE; ++curIndex)
    {
        if (NvM_StandQueue[curIndex].BlockId == BlockId)
        {
            blockIdIndex = curIndex;
            break;
        }
    }

    Std_ReturnType  retVal     = E_OK;
    NvM_BlockIdType intBlockId = BlockId - 1U;

    uint8 curActPriTable = NvM_Module.CurActPriTable;
    uint8 jobPri         = NvM_BlockDescriptor[intBlockId].NvmBlockJobPriority; /* PRQA S 2844 */ /* VL_NvM_2844 */
    /* PRQA S 2844,2962 ++ */ /* VL_NvM_2844,VL_NvM_2962 */
    if (blockIdIndex == NvM_PriTable2Queue[curActPriTable][jobPri].HeadIndex)
    /* PRQA S 2844,2962 -- */
    {
        NvM_NormalQueueFindBLockId(blockIdIndex, curActPriTable, jobPri);
    }
    /** More than one task for this priority or curActPriTable is Another */
    else
    {
        uint8 queueCounter = 0u;

        /** More than one task for this priority */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        curIndex = NvM_PriTable2Queue[curActPriTable][jobPri].HeadIndex;
        /* PRQA S 2844 -- */
        uint8 preIndex = curIndex;
        while ((curIndex != blockIdIndex) && (retVal == E_OK))
        {
            ++queueCounter;
            if (queueCounter < (NVM_SIZE_STANDARD_JOB_QUEUE - 1u))
            {
                preIndex = curIndex;
                curIndex = NvM_StandQueue[preIndex].NextIndex;
            }
            else
            {
                retVal = E_NOT_OK;
            }
        }
        if (retVal == E_OK)
        {
            NvM_NormalQueueSubSort(blockIdIndex, curActPriTable, jobPri, preIndex);
        }
    }
    if (retVal == E_OK)
    {
        /** Clear the bit in NvM_QueueSpaceTalbe */ /* PRQA S 2985, 3120 ++ */ /* VL_NvM_2985, VL_QAC_MagicNum */
        NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] |= (uint16)((uint16)0x01U << ((blockIdIndex & 0xF0U) >> 4U));
        NvM_QueueSpaceTalbe[((blockIdIndex & 0xF0U) >> 4u)] |= (uint16)((uint16)0x01U << (blockIdIndex & 0x0FU));
        /* PRQA S 2985, 3120 -- */
    }

    return retVal;
}

/**
 * @brief     Calculate the current tailindex
 */
NVM_LOCAL void NvM_ImmedQueueSortSubInternal(uint8 QueueCounter, uint8 CurIndex, uint8 PreIndex)
{
    uint8 queueCnt = QueueCounter;
    uint8 preIndex = PreIndex;
    uint8 index    = CurIndex;
    for (; queueCnt < (NvM_ImmedQueueManage.Count - 1U); ++queueCnt)
    {
        ++index;
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= index)
        {
            index = 0u;
        }
        NvM_ImmedQueue[preIndex].BlockId    = NvM_ImmedQueue[index].BlockId; /* PRQA S 2844 */    /* VL_NvM_2844 */
        NvM_ImmedQueue[preIndex].DestSrcPtr = NvM_ImmedQueue[index].DestSrcPtr; /* PRQA S 2844 */ /* VL_NvM_2844 */
        preIndex                            = index;
        NvM_ImmedQueue[index].BlockId       = 0u;
    }
    if (NvM_ImmedQueueManage.TailIndex == 0U)
    {
        NvM_ImmedQueueManage.TailIndex = NvM_ImmedQueueManage.Count - 1U; /* PRQA S 2903 */ /* VL_NvM_2903 */
    }
    else
    {
        --NvM_ImmedQueueManage.TailIndex;
    }
    --NvM_ImmedQueueManage.Count;
}

/**
 * @brief     Calculate the current tailindex
 */
NVM_LOCAL void NvM_ImmedQueueSortSub(uint8 blockIdIndex)
{
    uint8 preIndex     = 0u;
    uint8 queueCounter = 0u;
    /** This block takes the last position in the NvM_ImmedQueueManage */
    uint8 curIndex = NvM_ImmedQueueManage.HeadIndex;
    for (; queueCounter < NVM_SIZE_IMMEDIATE_JOB_QUEUE; ++queueCounter)
    {
        if (curIndex == blockIdIndex)
        {
            preIndex = curIndex;
            break;
        }
        ++curIndex;
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= curIndex)
        {
            curIndex = 0u;
        }
    }
    if (blockIdIndex == NvM_ImmedQueueManage.HeadIndex) /** First element */
    {
        NvM_ImmedQueue[blockIdIndex].BlockId = 0u;
        --NvM_ImmedQueueManage.Count;
        if ((NVM_SIZE_IMMEDIATE_JOB_QUEUE - 1U) <= NvM_ImmedQueueManage.HeadIndex)
        {
            NvM_ImmedQueueManage.HeadIndex = 0u;
        }
        else
        {
            ++NvM_ImmedQueueManage.HeadIndex;
        }
    }
    else if (queueCounter == (NvM_ImmedQueueManage.Count - 1U)) /** Last element */
    {
        NvM_ImmedQueue[blockIdIndex].BlockId = 0u;
        if (NvM_ImmedQueueManage.TailIndex == 0U)
        {
            NvM_ImmedQueueManage.TailIndex = NvM_ImmedQueueManage.Count - 1u;
        }
        else
        {
            --NvM_ImmedQueueManage.TailIndex;
        }
        --NvM_ImmedQueueManage.Count;
    }
    else
    {
        NvM_ImmedQueueSortSubInternal(queueCounter, curIndex, preIndex);
    }
}

/**
 * @brief     Check whether the current block is in the queue. If it is not, the queue flag is clear. Otherwise, check
 * the position of the current task in the queue and update the corresponding index
 */
NVM_LOCAL Std_ReturnType NvM_ImmedQueueSort(NvM_BlockIdType BlockId)
{
    uint8 queueIndex = NvM_ImmedQueueManage.TailIndex;
    uint8 blockIdIndex;
    for (uint8 curIndex = 0u; curIndex < NVM_SIZE_IMMEDIATE_JOB_QUEUE; ++curIndex)
    {
        if (NvM_ImmedQueue[queueIndex].BlockId == BlockId)
        {
            blockIdIndex = queueIndex;
            break;
        }
        ++queueIndex;
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= queueIndex)
        {
            queueIndex = 0u;
        }
    }

    NvM_ImmedQueueSortSub(blockIdIndex); /* PRQA S 2962 */ /* VL_NvM_2962 */

    return E_OK;
}
#endif
#endif

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
/**
 * @brief     Set the corresponding flag based on the passed protection flag
 */
NVM_LOCAL Std_ReturnType NvM_SetBlockProtectionSub(NvM_BlockIdType BlockId, boolean ProtectionEnabled)
{
    NvM_BlockIdType intBlockId = BlockId - 1U;
    if (TRUE == ProtectionEnabled)
    {
        NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_WRPRO); /* PRQA S 2844 */ /* VL_NvM_2844 */
    }
    else
    {
        NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_WRPRO);
    }
    return E_OK;
}

/**
 * @brief     Queue the invalidate request when block is a dataset type and index is in a valid range, or block is not a
 * dataset type
 */
NVM_LOCAL Std_ReturnType NvM_InvalidateNvBlockSub(NvM_BlockIdType BlockId)
{
    Std_ReturnType  retVal     = E_NOT_OK;
    NvM_BlockIdType intBlockId = BlockId - 1U;

    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((NVM_BLOCK_DATASET != NvM_BlockDescriptor[intBlockId].NvmBlockManagementType)
        || ((NvM_BlockDescriptor[intBlockId].NvmNvBlockNum > NvM_AdminBlock[intBlockId].CurrentIndex)
            && (NVM_BLOCK_DATASET == NvM_BlockDescriptor[intBlockId].NvmBlockManagementType)))
    /* PRQA S 2844 -- */
    {
        retVal = NvM_JobEnqueue(BlockId, NVM_INVALIDATE_NV_BLOCK_SERV_ID, NULL_PTR);
    }
    return retVal;
}

/**
 * @brief     Queue the erase request when block is a dataset type and index is in a valid range, or block is not a
 * dataset type
 */
NVM_LOCAL Std_ReturnType NvM_EraseNvBlockSub(NvM_BlockIdType BlockId)
{
    Std_ReturnType  retVal     = E_NOT_OK;
    NvM_BlockIdType intBlockId = BlockId - 1U;

    /* PRQA S 2844++  */ /* VL_NvM_2844 */
    if ((NVM_BLOCK_DATASET != NvM_BlockDescriptor[intBlockId].NvmBlockManagementType)
        || ((NvM_BlockDescriptor[intBlockId].NvmNvBlockNum > NvM_AdminBlock[intBlockId].CurrentIndex)
            && (NVM_BLOCK_DATASET == NvM_BlockDescriptor[intBlockId].NvmBlockManagementType)))
    /* PRQA S 2844 -- */
    {
        retVal = NvM_JobEnqueue(BlockId, NVM_ERASE_NV_BLOCK_SERV_ID, NULL_PTR);
    }
    return retVal;
}

/**
 * @brief     Check that the administrative flag bit for the current block is valid and record the error value for
 * invalid request
 */
#if (STD_ON == NVM_DEV_ERROR_DETECT)
NVM_LOCAL boolean NvM_InvalidateNvBlockCheck(NvM_BlockIdType intBlockId, uint8* errorId)
{
    boolean errorflag = FALSE;

    if ((TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[intBlockId].SingleReqResult == NVM_REQ_PENDING))
    {
        *errorId  = NVM_E_BLOCK_PENDING; /* PRQA S 2814 */ /* VL_NvM_2814 */
        errorflag = TRUE;
    }
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    else if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_BLOCK_LOCKED))
    /* PRQA S 2844 -- */
    {
        *errorId  = NVM_E_BLOCK_LOCKED;
        errorflag = TRUE;
    }
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    else if (
        (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEBLOCKONCE))
        && (TRUE == NVM_ISFLAGOFF(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_READ_REQUEST)))
    /* PRQA S 2844 -- */
    {
        *errorId  = NVM_E_WRITE_ONCE_STATUS_UNKNOWN;
        errorflag = TRUE;
    }
    else
    {
        /** idle */
    }

    return errorflag;
}

/**
 * @brief     Check that the administrative flag bit for the current block is valid and record the error value for erase
 * request
 */
NVM_LOCAL boolean NvM_EraseNvBlockCheck(NvM_BlockIdType intBlockId, uint8* errorId)
{
    boolean errorflag = FALSE;

    if ((TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[intBlockId].SingleReqResult == NVM_REQ_PENDING))
    {
        *errorId  = NVM_E_BLOCK_PENDING; /* PRQA S 2814 */ /* VL_NvM_2814 */
        errorflag = TRUE;
    }
    else if
#if (STD_ON == NVM_JOB_PRIORITIZATION)
        (0U != NvM_BlockDescriptor[intBlockId].NvmBlockJobPriority) /* PRQA S 2844 */ /* VL_NvM_2844 */
#else
        (STD_OFF == NVM_JOB_PRIORITIZATION)
#endif
    {
        *errorId  = NVM_E_BLOCK_CONFIG;
        errorflag = TRUE;
    }
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    else if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_BLOCK_LOCKED))
    {
        *errorId  = NVM_E_BLOCK_LOCKED;
        errorflag = TRUE;
    }
    else if (
        (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEBLOCKONCE))
        && (TRUE == NVM_ISFLAGOFF(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_READ_REQUEST)))
    /* PRQA S 2844 -- */
    {
        *errorId  = NVM_E_WRITE_ONCE_STATUS_UNKNOWN;
        errorflag = TRUE;
    }
    else
    {
        /** idle */
    }

    return errorflag;
}
#endif
#endif

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/**
 * @brief     Check whether the management flag bit of the current block is valid, and set the corresponding flag
 * according to the blockchange flag bit passed in
 */
NVM_LOCAL Std_ReturnType NvM_SetRamBlockStatusSub(NvM_BlockIdType BlockId, boolean BlockChanged)
{
    Std_ReturnType  retVal     = E_NOT_OK;
    NvM_BlockIdType intBlockId = BlockId - 1U;

    /* PRQA S 2844 ++*/ /* VL_NvM_2844 */
    if ((TRUE == NVM_ISFLAGOFF(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_WRPRO))
        && ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress)
            || (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_SYNCMECHANISM)))
        && (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_SETRAMSTATUS)))
    /* PRQA S 2844 -- */
    {
        if (TRUE == BlockChanged)
        {
            /* PRQA S 2844 ++*/ /* VL_NvM_2844 */
            NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_VALID);
            NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED);
            /* PRQA S 2844 -- */
            if (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_CALCRAMBLOCKCRC))
            {
                /** The Req is useless, no further clarification about the use of Crc */
                if (E_NOT_OK == NvM_JobEnqueue(BlockId, NVM_SET_RAM_BLOCK_STATUS_SERV_ID, NULL_PTR))
                {
                    /** Reserved */
                }
            }
            /* PRQA S 2844 ++*/ /* VL_NvM_2844 */
            if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_REPAIR))
            {
                NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_NV_REPAIR);
            }
            /* PRQA S 2844 -- */
        }
        else
        {
            /* PRQA S 2844 ++*/ /* VL_NvM_2844 */
            NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_VALID);
            NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED);
            /* PRQA S 2844 -- */
        }
        retVal = E_OK;
    }
    return retVal;
}
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
/**
 * @brief     Set the corresponding administrative flag bit when the read request is queued and the correct value is
 * returned
 */
NVM_LOCAL Std_ReturnType NvM_ReadBlockSub(NvM_BlockIdType BlockId, void* NvM_DstPtr, const boolean readPRAMflag)
{
    Std_ReturnType  retVal     = E_NOT_OK;
    NvM_BlockIdType intBlockId = BlockId - 1U;

    if (readPRAMflag == FALSE)
    {
        /** read NVRAM block */ /* PRQA S 0316 ++ */ /* VL_NvM_0316 */
        retVal = NvM_JobEnqueue(BlockId, NVM_READ_BLOCK_SERV_ID, (uint8*)NvM_DstPtr);
        /* PRQA S 0316 -- */
    }
    else
    {
        /** read PRAM block */
        retVal = NvM_JobEnqueue(BlockId, NVM_READ_PRAM_BLOCK_SERV_ID, NULL_PTR);
    }

    if (E_OK == retVal)
    {
        if (readPRAMflag == TRUE)
        {
            /** execute for read PRAM block service */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
            if (TRUE == NVM_ISFLAGOFF(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM))
            {
                NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, (uint16)(NVM_ADMIN_RAM_VALID));
                NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, (uint16)(NVM_ADMIN_RAM_CHANGED));
            }
            /* PRQA S 2844 -- */
        }
        else
        {
            /** execute for read block service */
            if (((NULL_PTR != NvM_DstPtr) && (NvM_DstPtr == NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress))
                || ((NULL_PTR == NvM_DstPtr) && (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress)))
            {
                NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, (uint16)(NVM_ADMIN_RAM_VALID));
                NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, (uint16)(NVM_ADMIN_RAM_CHANGED));
            }
        }
        /* PRQA S 2844 ++*/ /* VL_NvM_2844 */
        NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_READ_REQUEST);
        /* PRQA S 2844 -- */
    }
    return retVal;
}

#if (STD_ON == NVM_DEV_ERROR_DETECT)
/**
 * @brief     Check that the administrative flag bit for the current block is valid and record the error value for write
 * request
 */
NVM_LOCAL boolean NvM_WriteBlockCheck(NvM_BlockIdType intBlockId, const uint8* NvM_SrcPtr, uint8* errorId)
{
    boolean errorflag = FALSE;

    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((NULL_PTR == NvM_SrcPtr) && (NULL_PTR == NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress)
        && (TRUE == NVM_ISFLAGOFF(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM)))
    /* PRQA S 2844 -- */
    {
        *errorId  = NVM_E_PARAM_ADDRESS; /* PRQA S 2814 */ /* VL_NvM_2814 */
        errorflag = TRUE;
    }
    else if (
        (TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
        || (NvM_AdminBlock[intBlockId].SingleReqResult == NVM_REQ_PENDING))
    {
        *errorId  = NVM_E_BLOCK_PENDING;
        errorflag = TRUE;
    }
    /* PRQA S 2844 ++*/ /* VL_NvM_2844 */
    else if (TRUE == NVM_ISFLAGON(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_BLOCK_LOCKED))
    {
        *errorId  = NVM_E_BLOCK_LOCKED;
        errorflag = TRUE;
    }
    else if (
        (TRUE == NVM_ISFLAGON(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEBLOCKONCE))
        && (TRUE == NVM_ISFLAGOFF(NvM_AdminBlock[intBlockId].FlagGroup, NVM_ADMIN_READ_REQUEST)))
    /* PRQA S 2844 -- */
    {
        *errorId  = NVM_E_WRITE_ONCE_STATUS_UNKNOWN;
        errorflag = TRUE;
    }
    else
    {
        /** idle */
    }

    return errorflag;
}
#endif

/**
 * @brief     Set the corresponding administrative flag bit when the write request is queued and the correct value is
 * returned
 */
NVM_LOCAL Std_ReturnType
    NvM_WriteBlockSub(NvM_BlockIdType BlockId, const uint8* NvM_SrcPtr, const boolean writePRAMflag)
{
    Std_ReturnType  retVal     = E_NOT_OK;
    NvM_BlockIdType intBlockId = BlockId - 1U;

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    /** Beyond the Block quantity */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((NVM_BLOCK_DATASET == NvM_BlockDescriptor[intBlockId].NvmBlockManagementType)
        && (NvM_BlockDescriptor[intBlockId].NvmNvBlockNum <= NvM_AdminBlock[intBlockId].CurrentIndex))
    /* PRQA S 2844 -- */
    {
        /** idle */
    }
    else
#endif
    {
        if (writePRAMflag == FALSE)
        {
            /** write NVRAM block */ /* PRQA S 0311 ++ */ /* VL_NvM_0311 */
            retVal = NvM_JobEnqueue(BlockId, NVM_WRITE_BLOCK_SERV_ID, (uint8*)NvM_SrcPtr);
            /* PRQA S 0311 -- */
        }
        else
        {
            /** write PRAM block */
            retVal = NvM_JobEnqueue(BlockId, NVM_WRITE_PRAM_BLOCK_SERV_ID, NULL_PTR);
        }

        if (E_OK == retVal)
        {
#if (STD_ON == NVM_JOB_PRIORITIZATION)
            if ((0U == NvM_BlockDescriptor[intBlockId].NvmBlockJobPriority) && (NVM_JOB_STEP_IDLE != NvM_Module.JobStep)
                && (NVM_JOB_TYPE_SINGLE_IMMED != NvM_Module.CurrentJobType)
                && (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED != NvM_Module.CurrentJobType))
            {
                NvM_Module.JobStep = NVM_JOB_STEP_CANCEL;
                MemIf_Cancel(NvM_CurRunning.DeviceId);
                /** re-enqueue the preempted job */
                (void)NvM_JobEnqueue(NvM_CurRunning.BlockId, NvM_CurRunning.ServiceId, NvM_CurRunning.RamAddr);
            }
#endif
            /** treat the current block as valid */
            if (writePRAMflag == TRUE)
            {
                /** execute for Write PRAM block service */
                if (TRUE
                    == NVM_ISFLAGOFF(NvM_BlockDescriptor[intBlockId].FlagGroup, (uint8)NVM_BLOCK_DESC_SYNCMECHANISM))
                {
                    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                    NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, (uint8)(NVM_ADMIN_RAM_VALID));
                    NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, (uint8)(NVM_ADMIN_RAM_CHANGED));
                    /* PRQA S 2844 -- */
                }
            }
            else
            {
                /** execute for Write block service */
                if (((NULL_PTR != NvM_SrcPtr) && (NvM_SrcPtr == NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress))
                    || ((NULL_PTR == NvM_SrcPtr)
                        && (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress)))
                {
                    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                    NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, (uint8)(NVM_ADMIN_RAM_VALID));
                    NVM_SETFLAGON(&NvM_AdminBlock[intBlockId].FlagGroup, (uint8)(NVM_ADMIN_RAM_CHANGED));
                    /* PRQA S 2844 -- */
                }
            }
        }
    }
    return retVal;
}

/**
 * @brief     Set the corresponding administrative flag bit when the restore default request is queued and the correct
 * value is returned
 */
NVM_LOCAL Std_ReturnType
    NvM_RestoreBlockDefaultsSub(NvM_BlockIdType BlockId, void* NvM_DestPtr, const boolean restorePRAMflag)
{
    Std_ReturnType  retVal     = E_NOT_OK;
    NvM_BlockIdType intBlockId = BlockId - 1U;

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    if ((((NVM_BLOCK_DATASET == NvM_BlockDescriptor[intBlockId].NvmBlockManagementType)
          && (NvM_AdminBlock[intBlockId].CurrentIndex
              < (NvM_BlockDescriptor[intBlockId].NvmNvBlockNum + NvM_BlockDescriptor[intBlockId].NvmRomBlockNum))
          && (NvM_BlockDescriptor[intBlockId].NvmNvBlockNum <= NvM_AdminBlock[intBlockId].CurrentIndex))
         || (NVM_BLOCK_DATASET != NvM_BlockDescriptor[intBlockId].NvmBlockManagementType))
        && ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRomBlockDataAddress)
            || (NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmInitBlockCallback)))
    /* PRQA S 2844 -- */
#endif
    {
        if (restorePRAMflag == FALSE)
        {
            /** Retorre NvRam */ /* PRQA S 0316 ++ */ /* VL_NvM_0316 */
            retVal = NvM_JobEnqueue(BlockId, NVM_RESTORE_BLOCK_DEFAULTS_SERV_ID, (uint8*)NvM_DestPtr);
            /* PRQA S 0316 -- */
        }
        else if (restorePRAMflag == TRUE) /* PRQA S 2991,2995 */ /* VL_QAC_2991,VL_NvM_2995 */
        {
            /** Retore PRAM */
            retVal = NvM_JobEnqueue(BlockId, NVM_RESTORE_PRAM_BLOCK_DEFAULTS_SERV_ID, NULL_PTR);
        }
        else
        {
            /** idle */
        }

        if (E_OK == retVal)
        {
            if (restorePRAMflag == TRUE)
            {
                /** execute for restore PRAM block service */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, (uint16)(NVM_ADMIN_RAM_VALID));
                NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, (uint16)(NVM_ADMIN_RAM_CHANGED));
                /* PRQA S 2844 -- */
            }
            else
            {
                /** execute for restore block service */
                if (((NULL_PTR != NvM_DestPtr)
                     && (NvM_DestPtr == NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress))
                    || ((NULL_PTR == NvM_DestPtr)
                        && ((NULL_PTR != NvM_BlockDescriptor[intBlockId].NvmRamBlockDataAddress)
                            || (TRUE
                                == NVM_ISFLAGON(
                                    NvM_BlockDescriptor[intBlockId].FlagGroup,
                                    (uint16)NVM_BLOCK_DESC_SYNCMECHANISM)))))
                {
                    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                    NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, (uint16)(NVM_ADMIN_RAM_VALID));
                    NVM_SETFLAGOFF(&NvM_AdminBlock[intBlockId].FlagGroup, (uint16)(NVM_ADMIN_RAM_CHANGED));
                    /* PRQA S 2844 -- */
                }
            }
        }
    }
    return retVal;
}
#endif

#if (STD_OFF == NVM_POLLING_MODE)
/**
 * @brief     Check if an internal task still exists and get the service id based on whether the current request task is
 * WRITE VERIFICATION
 */
NVM_LOCAL uint8 NvM_JobNotiGetId(const boolean JobSuccess)
{
    uint8 serviceId = (uint8)NVM_NONE_SERV_ID;
    if (0U != NvM_AtomJob.Count)
    {
#if (STD_ON == NVM_WRITE_VERIFY)
        if (NVM_ATOMJOB_WRITEVERIFICATION == NvM_AtomJob.ReqId[NvM_AtomJob.Count]) /* PRQA S 2844 */ /* VL_NvM_2844 */
        {
            if (TRUE == JobSuccess)
            {
                NvM_InterWriteVerificationOK();
                /** The job is finished and verify data is necessary, change the state to pending */
                if (NvM_Module.MemIfJobState != NVM_MEMIF_JOB_IDLE)
                {
                    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING;
                }
            }
            else
            {
                /** Update the valid and changed flag bits to off, and update the job request result to not ok */
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_VERIFY_FAILED)
                Dem_SetEventStatus(NVM_E_VERIFY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
            }
        }
        else
#else
        NVM_PARAM_UNUSED(JobSuccess);
#endif
        {
            /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
            serviceId =
                (NVM_ATOMJOB_READBLOCK == NvM_AtomJob.ReqId[NvM_AtomJob.Count])
                    ? 0u
                    : ((NVM_ATOMJOB_WRITEBLOCK == NvM_AtomJob.ReqId[NvM_AtomJob.Count])
                           ? 1u
                           : ((NVM_ATOMJOB_INVALIDATE == NvM_AtomJob.ReqId[NvM_AtomJob.Count]) ? 5u : serviceId));
        }
    }
    if ((NVM_JOB_STEP_IDLE != NvM_Module.JobStep) && (0U == NvM_AtomJob.Count)
        && (NvM_Module.MemIfJobState != NVM_MEMIF_JOB_IDLE))
    {
        serviceId = (NVM_READ_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                        ? 0u
                        : ((NVM_READ_PRAM_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                               ? 0u
                               : ((NVM_WRITE_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                                      ? 1u
                                      : ((NVM_WRITE_PRAM_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                                             ? 1u
                                             : ((NVM_INVALIDATE_NV_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                                                    ? 5u
                                                    : ((NVM_ERASE_NV_BLOCK_SERV_ID == NvM_CurRunning.ServiceId)
                                                           ? 3u
                                                           : serviceId)))));
        /* PRQA S 3120 -- */
    }
    return serviceId;
}
#endif
/* PRQA S 1503,1532 -- */
#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"
