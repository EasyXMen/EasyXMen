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
 **  @file               : NvM_Act.c
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
#if (STD_ON == NVM_DEM_PRODUCTION_ERROR_DETECT)
#include "Dem.h"
#endif
#if (STD_ON == NVM_INCLUDE_CRC)
#include "Crc.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================= internal function declarations ========================================= */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
#if ((NVM_API_CONFIG_CLASS_1 == NVM_API_CONFIG_CLASS) || (STD_OFF == NVM_JOB_PRIORITIZATION))
/**
 * @brief       Multi-job scheduling of NvM
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76694
 */
NVM_LOCAL Std_ReturnType NvM_MultiJobDispatch(void);
#endif

/**
 * @brief       Requests sent by nvm to memif
 * @param[in]   NvM_MemIfRequest: The job request type
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76681
 */
NVM_LOCAL void NvM_MemIfReqSubDeal(NvM_MemIfReqType NvM_MemIfRequest);

/**
 * @brief       Memif read request job is ok
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76682
 */
NVM_LOCAL void NvM_MemIfReadOk(void);

/**
 * @brief       Memif read request job is fail
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76683
 */
NVM_LOCAL void NvM_MemIfReadFailed(void);

/**
 * @brief       Memif request job is pending
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76684
 */
NVM_LOCAL void NvM_MemIfPending(void);

/**
 * @brief       Memif request job is canceled
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76685
 */
NVM_LOCAL void NvM_MemIfCancelled(void);

/**
 * @brief       Memif request data is inconsistent
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76686
 */
NVM_LOCAL void NvM_MemIfInconsistent(void);

/**
 * @brief       Memif request block is invalid
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76687
 */
NVM_LOCAL void NvM_MemIfInvalid(void);

/**
 * @brief       Memif write request job is ok
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76688
 */
NVM_LOCAL void NvM_MemIfWriteOk(void);

/**
 * @brief       Memif write request job is fail
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76689
 */
NVM_LOCAL void NvM_MemIfWriteFailed(void);

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief       Memif erase request job is ok
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76690
 */
NVM_LOCAL void NvM_MemIfEraseOk(void);

/**
 * @brief       Memif erase request job is fail
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76691
 */
NVM_LOCAL void NvM_MemIfEraseFailed(void);
#endif

/**
 * @brief       Memif invalid request job is ok
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76692
 */
NVM_LOCAL void NvM_MemIfInvalidOk(void);

/**
 * @brief       Memif invalid request job is fail
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76693
 */
NVM_LOCAL void NvM_MemIfInvalidFailed(void);
#endif

/**
 * @brief       Gets the ram address of the requested job in the queue
 * @param[in]   QueueIndex: The index value of the queue
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76695
 */
NVM_LOCAL void NvM_GetRamAddress(uint8 QueueIndex);

/**
 * @brief       Copy the block information
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76696
 */
NVM_LOCAL void NvM_CopyCurBlockInfo(void);

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief       The result is ok when the job is dequeue
 * @param[in]   jobQueueIndex: The index value of the queue
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76697
 */
NVM_LOCAL void NvM_JobDequeueSubDealOk(uint8 jobQueueIndex);

/**
 * @brief       Subfunction that exits the queue
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76698
 */
NVM_LOCAL Std_ReturnType NvM_JobDequeueSub(void);

/**
 * @brief       Subfunction that exits the queue
 * @param[in]   Number: The block number
 * @param[out]  SettedBitPtr: Pointer to the bit that need to set
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76699
 */
NVM_LOCAL Std_ReturnType NvM_Get16Bits1stSettedBit(uint16 Number, uint8* SettedBitPtr);

/**
 * @brief       Get the hightest priority job
 * @param[in]   JobQueueIndexPtr: The index value of the queue
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76700
 */
NVM_LOCAL Std_ReturnType NvM_GetHighestPriJob(uint8* JobQueueIndexPtr);

/**
 * @brief       Gets the queue index of the immediate priority of a single task
 * @param[in]   JobQueueIndexPtr: The index value of the queue
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76701
 */
NVM_LOCAL Std_ReturnType NvM_GetQueueSpaceSingleImmed(uint8* JobQueueIndexPtr);

/**
 * @brief       Subfunction of getting the queue space
 * @param[in]   jobQueueGroup: The queue group of request job
 * @param[in]   JobQueueIndexPtr: The index value of the queue
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76702
 */
NVM_LOCAL Std_ReturnType NvM_GetQueueSpaceSub(uint8 jobQueueGroup, uint8* JobQueueIndexPtr);

/**
 * @brief       Get the queue space
 * @param[in]   JobType: The request job type
 * @param[in]   JobQueueIndexPtr: The index value of the queue
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76703
 */
NVM_LOCAL Std_ReturnType NvM_GetQueueSpace(NvM_JobTypeType JobType, uint8* JobQueueIndexPtr);

/**
 * @brief       Release the queue space
 * @param[in]   JobType: The request job type
 * @param[in]   JobQueueIndex: The index value of the queue
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76704
 */
NVM_LOCAL void NvM_RelQueueSpace(NvM_JobTypeType JobType, uint8 JobQueueIndex);

/**
 * @brief       Single job enqueue
 * @param[in]   BlockId: The request block ID
 * @param[in]   ServiceId: The request service ID
 * @param[in]   DestSrcPtr: Pointer to the address
 * @return      Std_ReturnType
 * @retval      E_OK: Single job enqueue successfully
 * @retval      E_NOT_OK: Single job enqueue failed
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76705
 */
NVM_LOCAL Std_ReturnType
    NvM_JobEnqueueSingleJob(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8* DestSrcPtr);

/**
 * @brief       Multi-job enqueue
 * @param[in]   ServiceId: The request service ID
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76706
 */
NVM_LOCAL void NvM_JobEnqueueMultiJob(NvM_ServiceIdType ServiceId);

/**
 * @brief       Assign write job when multi-job is requested
 * @param[in]   JobType: The request job type
 * @return      Std_ReturnType
 * @retval      E_OK: request has been accepted
 * @retval      E_NOT_OK: request has not been accepted
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76707
 */
NVM_LOCAL Std_ReturnType NvM_MultiJobDispatchWrite(void);

/**
 * @brief       Subfunction of getting hightest priority job
 * @param[in]   curActPriTable: The currnetly priority table
 * @param[in]   priGroup: The priority group
 * @param[in]   JobQueueIndexPtr: The index value of the queue
 * @return      Std_ReturnType
 * @retval      E_OK: Get hightest priority job successfully
 * @retval      E_NOT_OK: hightest priority job failed
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-76708
 */
NVM_LOCAL Std_ReturnType NvM_GetHighestPriJobSub(uint8 curActPriTable, uint8 priGroup, uint8* JobQueueIndexPtr);

/**
 * @brief       Place the requested task in the queue with priority
 * @param[in]   BlockId: The block identifier uniquely identifies one NVRAM block descriptor
 * @param[in]   ServiceId: The service identifier uniquely identifies one NvM service
 * @param[in]   DestSrcPtr: A pointer to the data being stored
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @trace       CPD-78584
 */
NVM_LOCAL Std_ReturnType NvM_JobEnqueueWithPri(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8* DestSrcPtr);

/**
 * @brief       Remove a task from the queue for processing with priority
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-78585
 */
NVM_LOCAL Std_ReturnType NvM_JobDequeueWithPri(void);

/**
 * @brief       Multi-job scheduling of NvM with priority
 * @return      Std_ReturnType
 * @retval      E_OK: Processing success
 * @retval      E_NOT_OK: Processing Failure
 * @reentrant   Non Reentrant
 * @synchronous TRUE
 * @trace       CPD-78586
 */
NVM_LOCAL Std_ReturnType NvM_MultiJobDispatchWithPri(void);
#endif
#endif
#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* ============================================ internal data definition ============================================ */

/* ============================================ external data definition ============================================ */
#define NVM_START_SEC_CONST_PTR
#include "NvM_MemMap.h"
/* PRQA S 1531 ++ */ /* VL_QAC_OneRefSymbol */
#if (NVM_API_CONFIG_CLASS_3 != NVM_API_CONFIG_CLASS)
/**
 * @brief API function for class1 and calss2
 */
const NvM_VoidFuncVoidPtr NvM_MemIfAsyncFuncTable[NVM_MEMIF_JOB_MAX_NUM][NVM_MEMIF_CLASS1_AND_CLASS2_NUM] =
#elif (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief API function with pirority mechanism
 */
const NvM_VoidFuncVoidPtr NvM_MemIfAsyncFuncTable[NVM_MEMIF_JOB_MAX_NUM][NVM_MEMIF_JOB_MAX_NUM] =
#else
/**
 * @brief API function for class1 and calss3
 */
const NvM_VoidFuncVoidPtr NvM_MemIfAsyncFuncTable[NVM_MEMIF_JOB_MAX_NUM][NVM_MEMIF_JOB_MAX_NUM] =
#endif
    {
        {
            &NvM_MemIfReadOk,  /** Read */
            &NvM_MemIfWriteOk, /** Write */
            NULL_PTR,          /** Restore default */
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
            &NvM_MemIfEraseOk, /** Erase */
#else
            NULL_PTR, /** Erase */
#endif
            NULL_PTR,            /** Cancel */
            &NvM_MemIfInvalidOk, /** Invalidate */
#endif
        },
        {
            &NvM_MemIfReadFailed,
            &NvM_MemIfWriteFailed,
            NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
            &NvM_MemIfEraseFailed,
#else
            NULL_PTR, /** Erase */
#endif
            NULL_PTR,
            &NvM_MemIfInvalidFailed,
#endif
        },
        {
            &NvM_MemIfPending,
            &NvM_MemIfPending,
            NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
            &NvM_MemIfPending,
#else
            NULL_PTR, /** Erase */
#endif
            NULL_PTR,
            &NvM_MemIfPending,
#endif
        },
        {
            &NvM_MemIfCancelled,
            &NvM_MemIfCancelled,
            NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
            &NvM_MemIfCancelled,
#else
            NULL_PTR, /** Erase */
#endif
            NULL_PTR,
            &NvM_MemIfCancelled,
#endif
        },
        {
            &NvM_MemIfInconsistent,
            &NvM_MemIfInconsistent,
            NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
            &NvM_MemIfInconsistent,
#else
            NULL_PTR, /** Erase */
#endif
            NULL_PTR,
            &NvM_MemIfInconsistent,
#endif
        },
        {
            &NvM_MemIfInvalid,
            &NvM_MemIfInvalid,
            NULL_PTR,
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
            &NvM_MemIfInvalid,
#else
            NULL_PTR, /** Erase */
#endif
            NULL_PTR,
            &NvM_MemIfInvalid,
#endif
        },
};
/* PRQA S 1531 -- */
#define NVM_STOP_SEC_CONST_PTR
#include "NvM_MemMap.h"

#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
/**
 * @brief Mirror buffer for synchronization mechanism
 */
uint8 NVM_RamMirror[NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR]; /* PRQA S 1514 */ /* VL_NvM_1514 */
#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
#endif

/* ========================================== external function definition ========================================== */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
/**
 * @brief If the api class is class 1, put a job to the job queue to be processing. Otherwise, check whether the current
 * queue overflows before adding it to the queue
 */
Std_ReturnType NvM_JobEnqueue(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8* DestSrcPtr)
{
    Std_ReturnType      retVal = E_NOT_OK;
    NvM_AdminBlockType* AdminBlockPtr;

#if (                                                \
    (NVM_API_CONFIG_CLASS_1 == NVM_API_CONFIG_CLASS) \
    || ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_OFF == NVM_JOB_PRIORITIZATION)))
    NvM_MultiJobType* MultiJobPtr = &NvM_MultiJob;
#endif
#if (NVM_API_CONFIG_CLASS_1 == NVM_API_CONFIG_CLASS)
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;

    SchM_Enter_NvM_Queue();
    /** the api class is class 1, put job to global variant directly, no need queue */
    CurRunningPtr->BlockId   = BlockId;
    MultiJobPtr->Enqueue     = TRUE;
    CurRunningPtr->ServiceId = ServiceId;
    SchM_Exit_NvM_Queue();
    NvM_MultiJobResultFeedBack(CurRunningPtr->ServiceId, NVM_REQ_PENDING);
    SchM_Enter_NvM_Queue();
    CurRunningPtr->RamAddr = DestSrcPtr;
    SchM_Exit_NvM_Queue();
    retVal = E_OK;
#else
#if (STD_OFF == NVM_JOB_PRIORITIZATION)
    if (NVM_SIZE_STANDARD_JOB_QUEUE > NvM_StandQueueManage.Count)
    {
        SchM_Enter_NvM_Queue();
        NvM_StandQueue[NvM_StandQueueManage.TailIndex].BlockId    = BlockId;
        NvM_StandQueue[NvM_StandQueueManage.TailIndex].ServiceId  = ServiceId;
        NvM_StandQueue[NvM_StandQueueManage.TailIndex].DestSrcPtr = DestSrcPtr;
        ++NvM_StandQueueManage.TailIndex;
        /** protect the tailindex is overflow, when this happened, reset it */
        if (NVM_SIZE_STANDARD_JOB_QUEUE <= NvM_StandQueueManage.TailIndex)
        {
            NvM_StandQueueManage.TailIndex = 0u;
        }
        ++NvM_StandQueueManage.Count;
        SchM_Exit_NvM_Queue();
        if ((NVM_READ_ALL_SERV_ID == ServiceId) || (NVM_WRITE_ALL_SERV_ID == ServiceId)
            || (NVM_VALIDATE_ALL_SERV_ID == ServiceId) || (NVM_FIRSTINIT_ALL_SERV_ID == ServiceId))
        {
            SchM_Enter_NvM_Queue();
            /** set multiple type job flag */
            MultiJobPtr->Enqueue   = TRUE;
            MultiJobPtr->ServiceId = ServiceId;
            SchM_Exit_NvM_Queue();
            NvM_MultiJobResultFeedBack(ServiceId, NVM_REQ_PENDING);
        }
        else
        {
            AdminBlockPtr = &(NvM_AdminBlock[BlockId - 1U]);

            SchM_Enter_NvM_Queue();
            /** set single type job flag */
            NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_ENQUEUE);
            SchM_Exit_NvM_Queue();
            NvM_SingleJobResultFeedBack(BlockId, NVM_REQ_PENDING);
        }
        retVal = E_OK;
    }
#else
    retVal = NvM_JobEnqueueWithPri(BlockId, ServiceId, DestSrcPtr);
#endif
#endif
#if (NVM_SELECT_BLOCK_FOR_READ_ALL == STD_ON)
    SchM_Enter_NvM_Queue();
    if (NVM_READ_ALL_SERV_ID == ServiceId)
    {
        const NvM_BlockDescriptorType* BlockDescriptorPtr;

        for (uint16 iLoop = 0u; iLoop < NVM_BLOCK_NUM_ALL; ++iLoop)
        {
            AdminBlockPtr      = &(NvM_AdminBlock[iLoop]);
            BlockDescriptorPtr = &(NvM_BlockDescriptor[iLoop]);
            if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_SELECTBLOCKFORREADALL))
            {
                NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_READ_REQUEST);
            }
        }
    }
    SchM_Exit_NvM_Queue();
#endif
    return retVal;
}

/**
 * @brief If the api class is not class 1, determine whether it is a multi job type and schedule it. Otherwise. If the
 * api class is class 1, if the queue is not empty, block information is obtained based on the requested service
 * type.Otherwise, if multiple tasks are scheduled, block information of the requested task is obtained if there are
 * tasks in the queue
 */
/* PRQA S 6070,6020 ++ */ /*VL_MTR_NvM_STCAL,VL_MTR_NvM_STLIN */
Std_ReturnType NvM_JobDequeue(void)
{
    Std_ReturnType retVal = E_NOT_OK;
#if (                                                \
    (NVM_API_CONFIG_CLASS_1 == NVM_API_CONFIG_CLASS) \
    || ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_OFF == NVM_JOB_PRIORITIZATION)))
    NvM_AdminBlockType* AdminBlockPtr;
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;
#endif
#if (NVM_API_CONFIG_CLASS_1 == NVM_API_CONFIG_CLASS)
    NvM_BlockIdType intBlockId;
    if (NVM_JOB_TYPE_MULTI == ModulePtr->CurrentJobType)
    {
        /** multiply type job dispatch */
        retVal = NvM_MultiJobDispatch();
    }
    else if (TRUE == NvM_MultiJob.Enqueue)
    {
        SchM_Enter_NvM_Queue();
        ModulePtr->CurrentJobType = NVM_JOB_TYPE_MULTI;
        if (NVM_WRITE_ALL_SERV_ID == CurRunningPtr->ServiceId)
        {
            CurRunningPtr->BlockId = NVM_WRITEALL_FIRST_BLOCKID;
        }
        else
        {
            CurRunningPtr->BlockId = NVM_READALL_FIRST_BLOCKID;
        }
        /** multiple type job did not have temprary parameter, so using default ram address */
        NvM_GetRamAddress(USELESS_FOR_MULTIJOB);

        intBlockId = NvM_GetIntBlockId(CurRunningPtr->BlockId);

        AdminBlockPtr = &(NvM_AdminBlock[intBlockId]);

        AdminBlockPtr->SingleReqResult = NVM_REQ_PENDING;
        /** Single block from the multi job */
        /** copy current block information to global variant to process */
        NvM_CopyCurBlockInfo();
        SchM_Exit_NvM_Queue();
        retVal = E_OK;
    }
    else
    {
        /** there is no job need process or the flag set error happend */
        retVal = E_NOT_OK;
    }
#else
#if (STD_OFF == NVM_JOB_PRIORITIZATION)
    NvM_BlockIdType                intBlockId;
    uint8                          queueIndex          = 0u;
    NvM_RoundRobinQueueManageType* StandQueueManagePtr = &NvM_StandQueueManage;

    if (NVM_JOB_TYPE_MULTI == ModulePtr->CurrentJobType)
    {
        /** multiple type job dispatch */
        retVal = NvM_MultiJobDispatch();
    }
    else if (0U < StandQueueManagePtr->Count)
    {
        /** get the queue current head index */
        queueIndex               = StandQueueManagePtr->HeadIndex;
        CurRunningPtr->ServiceId = NvM_StandQueue[queueIndex].ServiceId;
        /** The job of the function NvM_FirstInitAll shall not be started whilethere are single block requests that need
         * to be processed by the NvM module, move NvM_FirstInitAll to TailIndex */
        if ((NVM_FIRSTINIT_ALL_SERV_ID == CurRunningPtr->ServiceId) && (1U < StandQueueManagePtr->Count))
        {
            SchM_Enter_NvM_Queue();
            NvM_StandQueue[StandQueueManagePtr->TailIndex].BlockId =
                NvM_StandQueue[StandQueueManagePtr->HeadIndex].BlockId;
            NvM_StandQueue[StandQueueManagePtr->TailIndex].ServiceId =
                NvM_StandQueue[StandQueueManagePtr->HeadIndex].ServiceId;
            NvM_StandQueue[StandQueueManagePtr->TailIndex].DestSrcPtr =
                NvM_StandQueue[StandQueueManagePtr->HeadIndex].DestSrcPtr;
            ++StandQueueManagePtr->TailIndex;
            /** protect the tailindex is overflow, when this happened, reset it */
            if (NVM_SIZE_STANDARD_JOB_QUEUE <= StandQueueManagePtr->TailIndex)
            {
                StandQueueManagePtr->TailIndex = 0u;
            }
            /** set the queue management flag */
            ++StandQueueManagePtr->HeadIndex;
            if (NVM_SIZE_STANDARD_JOB_QUEUE <= StandQueueManagePtr->HeadIndex)
            {
                /** prevent the index overflow, when it happen, need reset it */
                StandQueueManagePtr->HeadIndex = 0u;
            }
            /** get the queue current head index */
            queueIndex               = StandQueueManagePtr->HeadIndex;
            CurRunningPtr->ServiceId = NvM_StandQueue[queueIndex].ServiceId;
            SchM_Exit_NvM_Queue();
        }
        if ((NVM_READ_ALL_SERV_ID == CurRunningPtr->ServiceId) || (NVM_WRITE_ALL_SERV_ID == CurRunningPtr->ServiceId)
            || (NVM_VALIDATE_ALL_SERV_ID == CurRunningPtr->ServiceId)
            || (NVM_FIRSTINIT_ALL_SERV_ID == CurRunningPtr->ServiceId))
        {
            SchM_Enter_NvM_Queue();
            ModulePtr->CurrentJobType = NVM_JOB_TYPE_MULTI;
            if (NVM_WRITE_ALL_SERV_ID == CurRunningPtr->ServiceId)
            {
                CurRunningPtr->BlockId = NVM_WRITEALL_FIRST_BLOCKID;
            }
            else
            {
                CurRunningPtr->BlockId = NVM_READALL_FIRST_BLOCKID;
            }
            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);

            intBlockId = NvM_GetIntBlockId(CurRunningPtr->BlockId);

            AdminBlockPtr = &(NvM_AdminBlock[intBlockId]);

            AdminBlockPtr->SingleReqResult = NVM_REQ_PENDING;
            SchM_Exit_NvM_Queue();
            NvM_SingleJobResultFeedBack(CurRunningPtr->BlockId, NVM_REQ_PENDING); /** Single block from the multi job */
        }
        else
        {
            SchM_Enter_NvM_Queue();
            ModulePtr->CurrentJobType = NVM_JOB_TYPE_SINGLE_NORMAL;
            CurRunningPtr->BlockId    = NvM_StandQueue[queueIndex].BlockId;

            intBlockId = NvM_GetIntBlockId(CurRunningPtr->BlockId);

            AdminBlockPtr = &(NvM_AdminBlock[intBlockId]);

            NvM_GetRamAddress(queueIndex);
            NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_ENQUEUE);
            SchM_Exit_NvM_Queue();
        }
        SchM_Enter_NvM_Queue();
        NvM_CopyCurBlockInfo();
        /** set the queue management flag */
        ++StandQueueManagePtr->HeadIndex;
        if (NVM_SIZE_STANDARD_JOB_QUEUE <= StandQueueManagePtr->HeadIndex)
        {
            /** prevent the index overflow, when it happen, need reset it */
            StandQueueManagePtr->HeadIndex = 0u;
        }
        --StandQueueManagePtr->Count;
        SchM_Exit_NvM_Queue();
        retVal = E_OK;
    }
    else
    {
        /** the job queue is empty */
        retVal = E_NOT_OK;
    }
#else
    retVal = NvM_JobDequeueWithPri();
#endif
#endif
    return retVal;
}
/* PRQA S 6070,6020 -- */

/**
 * @brief     If the memif job status is ready, and according to the current underlying state obtained, decide whether
 * to send the request or set the job result to not ok; If the current job step is Cancel, set the job result to cancel
 */
void NvM_MemIfReq(NvM_MemIfReqType NvM_MemIfRequest) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    const NvM_ModuleType* ModulePtr = &NvM_Module;

#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
    if (NVM_JOB_STEP_CANCEL == ModulePtr->JobStep)
    {
        NvM_JobOverSetFlag(NVM_REQ_CANCELED, NVM_CRC_DELETE);
    }
    if (NVM_MEMIF_JOB_ASYNC_READY == ModulePtr->MemIfJobState)
    {
        MemIf_StatusType LowLayerStatus = MemIf_GetStatus(NvM_CurRunning.DeviceId);

        if (MEMIF_IDLE == LowLayerStatus)
        {
            NvM_MemIfReqSubDeal(NvM_MemIfRequest);
        }
        else if (MEMIF_UNINIT == LowLayerStatus)
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
        else
        {
            /** idle */
        }
    }
#else
    /* PRQA S 3415 ++ */ /*VL_NvM_3415 */
    if ((NVM_MEMIF_JOB_ASYNC_READY == ModulePtr->MemIfJobState)
        && (MEMIF_IDLE == MemIf_GetStatus(NvM_CurRunning.DeviceId)))
    {
        /* PRQA S 3415 -- */
        NvM_MemIfReqSubDeal(NvM_MemIfRequest);
    }
#endif
}
/* PRQA S 1532 -- */

/* ========================================== internal function definition ========================================== */
#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
/**
 * @brief The processing is performed depending on whether the requested service is multi job or single job
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
NVM_LOCAL Std_ReturnType NvM_JobEnqueueWithPri(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8* DestSrcPtr)
/* PRQA S 1532 -- */
{
    Std_ReturnType retVal = E_OK;

    /** Multi Job */
    if ((NVM_READ_ALL_SERV_ID == ServiceId) || (NVM_WRITE_ALL_SERV_ID == ServiceId)
        || (NVM_VALIDATE_ALL_SERV_ID == ServiceId) || (NVM_FIRSTINIT_ALL_SERV_ID == ServiceId))
    {
        NvM_JobEnqueueMultiJob(ServiceId);
    }
    /** Single Job */
    else if ((NVM_BLOCK_NUM_ALL >= BlockId) && (0U < BlockId))
    {
        retVal = NvM_JobEnqueueSingleJob(BlockId, ServiceId, DestSrcPtr);
    }
    else
    {
        /** Invalid BlockId or Job has already enqueued */
        retVal = E_NOT_OK;
    }

    return retVal;
}

/**
 * @brief If the current job is an immediate job, obtain the information based on the job type and set the flag.
 * Otherwise, determine whether it is a multi job type and schedule it. Otherwise, the processing is based on the
 * corresponding priority
 */
NVM_LOCAL Std_ReturnType NvM_JobDequeueWithPri(void) /* PRQA S 1532 */ /* VL_QAC_OneFunRef */
{
    Std_ReturnType                       retVal              = E_OK;
    NvM_ModuleType*                      ModulePtr           = &NvM_Module;
    const NvM_RoundRobinQueueManageType* ImmedQueueManagePtr = &NvM_ImmedQueueManage;

    /** ImmedQueue serves first during Dequeue */
    if (NVM_QUEUE_EMPTY_COUNT != ImmedQueueManagePtr->Count)
    {
        SchM_Enter_NvM_Queue();
        if ((NVM_JOB_TYPE_MULTI == ModulePtr->CurrentJobType)
            || (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == ModulePtr->CurrentJobType))
        {
            ModulePtr->CurrentJobType = NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED;
        }
        else
        {
            ModulePtr->CurrentJobType = NVM_JOB_TYPE_SINGLE_IMMED;
        }

        NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;

        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        CurRunningPtr->BlockId   = NvM_ImmedQueue[ImmedQueueManagePtr->HeadIndex].BlockId;
        CurRunningPtr->ServiceId = NVM_WRITE_BLOCK_SERV_ID;
        NvM_GetRamAddress(ImmedQueueManagePtr->HeadIndex);
        SchM_Exit_NvM_Queue();
        NvM_RelQueueSpace(ModulePtr->CurrentJobType, ImmedQueueManagePtr->HeadIndex);
        NvM_CopyCurBlockInfo();
        NVM_SETFLAGOFF(&NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE);
        /* PRQA S 2844 -- */
    }
    else
    {
        retVal = NvM_JobDequeueSub();
    }
    return retVal;
}
#endif

/**
 * @brief     Different memif interfaces are invoked based on different incoming memif requests
 */
NVM_LOCAL void NvM_MemIfReqSubDeal(NvM_MemIfReqType NvM_MemIfRequest)
{
    Std_ReturnType      retVal        = E_NOT_OK;
    boolean             processOn     = FALSE;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;
    uint16 blockNum = (uint16)(CurRunningPtr->BaseNumber << NVM_DATASET_SELECTION_BITS) + (uint16)CurRunningPtr->Index;

    switch (NvM_MemIfRequest)
    {
    case NVM_MEMIF_REQ_READ:
    {
#if ((STD_ON == NVM_CIPHERING_ENABLE) || (NVM_STATIC_BLOCKID_CHK == STD_ON))
        NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
        const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);
#endif
        /* PRQA S 2814, 2844 ++ */ /* VL_QAC_DerefNullPtr, VL_NvM_2844 */
        uint16 reqLength =
#if (STD_ON == NVM_CIPHERING_ENABLE)
            (NULL_PTR != BlockDescriptorPtr->NvMBlockCipheringRef)
                ? (BlockDescriptorPtr->NvMBlockCipheringRef->NvMNvBlockNVRAMDataLength)
                :
#endif
                CurRunningPtr->Length;
        /* PRQA S 2814, 2844 -- */
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        reqLength = (NVM_CRC16 == CurRunningPtr->CRCType)
                        ? (reqLength + 2U)
                        : ((NVM_CRC32 == CurRunningPtr->CRCType)
                               ? (reqLength + 4U)
                               : ((NVM_CRC8 == CurRunningPtr->CRCType) ? (reqLength + 1U) : reqLength));
#if (NVM_STATIC_BLOCKID_CHK == STD_ON)
        reqLength = (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_STATICBLOCKIDCHECK))
                        ? (reqLength + 2U)
                        : reqLength;
#endif
        /* PRQA S 3120 -- */
        retVal    = MemIf_Read(CurRunningPtr->DeviceId, blockNum, 0u, NvM_NvDataBuffer, reqLength);
        processOn = TRUE;
        break;
    }
    case NVM_MEMIF_REQ_WRITE:
        retVal    = MemIf_Write(CurRunningPtr->DeviceId, blockNum, NvM_NvDataBuffer);
        processOn = TRUE;
        if (E_OK == retVal)
        {
            --CurRunningPtr->WriteTimesCounter;
        }
        break;
    case NVM_MEMIF_REQ_ERASE:
        retVal    = MemIf_EraseImmediateBlock(CurRunningPtr->DeviceId, blockNum);
        processOn = TRUE;
        break;
    case NVM_MEMIF_REQ_INVALID:
        retVal    = MemIf_InvalidateBlock(CurRunningPtr->DeviceId, blockNum);
        processOn = TRUE;
        break;
    default:
        /** never happen */
        break;
    }
    if (TRUE == processOn)
    {
        if (E_OK == retVal)
        {
#if (STD_ON == NVM_POLLING_MODE)
            NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING;
#endif
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
    }
}

/**
 * @brief     Determine whether data is lost and needs to be recovered. It then checks the static ID, gets information
 * about the crc, and sends the request calculated by the crc
 */
NVM_LOCAL void NvM_MemIfReadOk(void)
{
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;
    ModulePtr->MemIfJobState          = NVM_MEMIF_JOB_IDLE;

    if (NVM_JOB_STEP_READ_2ND_NV == ModulePtr->JobStep)
    {
        NVM_SETFLAGON(&CurRunningPtr->AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS);
    }
    else
    {
        NVM_SETFLAGOFF(&CurRunningPtr->AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
        /** success read the 1st NV,Clear the repair flag */
#if (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY)
        Dem_SetEventStatus(NVM_E_LOSS_OF_REDUNDANCY, DEM_EVENT_STATUS_PASSED);
#endif
    }

    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
#if (STD_ON == NVM_STATIC_BLOCKID_CHK)
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);
    if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_STATICBLOCKIDCHECK))
    /* PRQA S 2844 -- */
    {
        NvM_ReadStaticBlockID(NvM_NvDataBuffer, CurRunningPtr->CRCType);
    }
#endif
    if (NVM_CRC_NOT_USED != CurRunningPtr->CRCType)
    {
        CurRunningPtr->CrcAddr = NvM_NvDataBuffer;
        if (NVM_JOB_STEP_READ_1ST_NV == ModulePtr->JobStep)
        {
            ModulePtr->JobStep = NVM_JOB_STEP_CALC_CRC_READ_1ST_NV;
        }
        else
        {
            ModulePtr->JobStep = NVM_JOB_STEP_CALC_CRC_READ_2ND_NV;
        }
        NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
    }
#if (STD_ON == NVM_STATIC_BLOCKID_CHK)
    else if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_STATICBLOCKIDCHECK))
    {
        ModulePtr->JobStep = NVM_JOB_STEP_CHECK;
    }
#endif
    else
    {
        ModulePtr->JobStep = NVM_JOB_STEP_COPY;
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_PASSED);
#endif
    }
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
    Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
}

/**
 * @brief     If it is a block of dataset, the result is directly set to not ok. If it is a native or redundant block,
 * the rom area is read. If the rom area also fails to be read, the result is set to not ok
 */
NVM_LOCAL void NvM_MemIfReadFailed(void)
{
    NvM_ModuleType*                ModulePtr          = &NvM_Module;
    NvM_CurRunningType*            CurRunningPtr      = &NvM_CurRunning;
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

    ModulePtr->MemIfJobState = NVM_MEMIF_JOB_IDLE;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == CurRunningPtr->ManagementType)
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if ((STD_ON == NVM_DEM_E_REQ_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE))
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
#endif
        NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
    }
    else
#endif
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if (((NVM_BLOCK_NATIVE == CurRunningPtr->ManagementType) || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
            && ((NULL_PTR != BlockDescriptorPtr->NvmRomBlockDataAddress)
                || (NULL_PTR != BlockDescriptorPtr->NvmInitBlockCallback)))
        /* PRQA S 2844 -- */
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            CurRunningPtr->ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
#if ((STD_ON == NVM_DEM_E_REQ_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE) || (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY))
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
            Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY)
            /** Recovery fails if the result is fail */
            if (TRUE == NVM_ISFLAGON(CurRunningPtr->AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS))
            {
                Dem_SetEventStatus(NVM_E_LOSS_OF_REDUNDANCY, DEM_EVENT_STATUS_FAILED);
            }
#endif
#endif
            NVM_GetRepeatMirrorOperation();
            ModulePtr->JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM);
        }
        else if (
            (NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType) && (NVM_JOB_STEP_READ_1ST_NV == ModulePtr->JobStep))
        {
            /** Only for the Redundant type */
            ModulePtr->JobStep       = NVM_JOB_STEP_READ_2ND_NV;
            ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            CurRunningPtr->Index     = 1u;
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if ((STD_ON == NVM_DEM_E_REQ_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE))
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
            Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
            Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
#endif
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
        }
}

/**
 * @brief     The low layer is processing
 */
NVM_LOCAL void NvM_MemIfPending(void)
{
    /** do nothing, just waiting */
    /** do not remove the function */
}

/**
 * @brief     The low layer job is cancelled
 */
NVM_LOCAL void NvM_MemIfCancelled(void)
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_REMAIN);
}

/**
 * @brief     If it is a block of dataset, the result is directly set to INTEGRITY_FAILED. If it is a native or
 * redundant block, the rom area is read. If the rom area also fails to be read, the result is set to INTEGRITY_FAILED
 */
NVM_LOCAL void NvM_MemIfInconsistent(void)
{
    NvM_ModuleType*                ModulePtr          = &NvM_Module;
    NvM_CurRunningType*            CurRunningPtr      = &NvM_CurRunning;
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

    ModulePtr->MemIfJobState = NVM_MEMIF_JOB_IDLE;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == CurRunningPtr->ManagementType)
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if ((STD_ON == NVM_DEM_E_INTEGRITY_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE))
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
        Dem_SetEventStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
#endif
        NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
    }
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    else if (
        ((NVM_BLOCK_NATIVE == CurRunningPtr->ManagementType) || (NVM_JOB_STEP_READ_2ND_NV == ModulePtr->JobStep))
        && ((NULL_PTR != BlockDescriptorPtr->NvmRomBlockDataAddress)
            || (NULL_PTR != BlockDescriptorPtr->NvmInitBlockCallback)))
    /* PRQA S 2844 -- */
#else
    if (((NVM_BLOCK_NATIVE == CurRunningPtr->ManagementType)
         || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == ModulePtr->JobStep))
        && ((NULL_PTR != BlockDescriptorPtr->NvmRomBlockDataAddress)
            || (NULL_PTR != BlockDescriptorPtr->NvmInitBlockCallback)))
#endif
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
        CurRunningPtr->ESingleReqResult = NVM_REQ_INTEGRITY_FAILED;
#if (                                                                        \
    (STD_ON == NVM_DEM_E_INTEGRITY_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE) \
    || (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY))
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
        Dem_SetEventStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_LOSS_OF_REDUNDANCY)
        /** Recovery fails then this shall be reported to the DEM */
        if (TRUE == NVM_ISFLAGON(CurRunningPtr->AdminFlagGroup, NVM_ADMIN_REDUNDANCY_LOSS))
        {
            Dem_SetEventStatus(NVM_E_LOSS_OF_REDUNDANCY, DEM_EVENT_STATUS_FAILED);
        }
#endif
#endif
        NVM_GetRepeatMirrorOperation();
        ModulePtr->JobStep = NVM_JOB_STEP_READ_ROM;
        NvM_AtomJobReq(NVM_ATOMJOB_READROM);
    }
    else if ((NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType) && (NVM_JOB_STEP_READ_1ST_NV == ModulePtr->JobStep))
    {
        ModulePtr->JobStep       = NVM_JOB_STEP_READ_2ND_NV;
        ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY; /** Only for the Redundant type */
        CurRunningPtr->Index     = 1u;
    }
    else
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if ((STD_ON == NVM_DEM_E_INTEGRITY_FAILED) || (STD_ON == NVM_DEM_E_HARDWARE))
#if (STD_ON == NVM_DEM_E_INTEGRITY_FAILED)
        Dem_SetEventStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_FAILED);
#endif
#endif
        NvM_JobOverSetFlag(NVM_REQ_INTEGRITY_FAILED, NVM_CRC_DELETE);
    }
}

/**
 * @brief     The low layer block is invalid
 */
NVM_LOCAL void NvM_MemIfInvalid(void)
{
    NvM_ModuleType*                ModulePtr          = &NvM_Module;
    NvM_CurRunningType*            CurRunningPtr      = &NvM_CurRunning;
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

    ModulePtr->MemIfJobState = NVM_MEMIF_JOB_IDLE;
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == CurRunningPtr->ManagementType)
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
        NvM_JobOverSetFlag(NVM_REQ_NV_INVALIDATED, NVM_CRC_DELETE);
    }
    else
#endif
        if (((NVM_BLOCK_NATIVE == CurRunningPtr->ManagementType) || (NVM_JOB_STEP_READ_2ND_NV == ModulePtr->JobStep))
            && ((NULL_PTR != BlockDescriptorPtr->NvmRomBlockDataAddress)
                || (NULL_PTR != BlockDescriptorPtr->NvmInitBlockCallback)))
    {
        CurRunningPtr->ESingleReqResult = NVM_REQ_NV_INVALIDATED;
        ModulePtr->JobStep              = NVM_JOB_STEP_READ_ROM;
        NvM_AtomJobReq(NVM_ATOMJOB_READROM);
    }
    else if ((NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType) && (NVM_JOB_STEP_READ_1ST_NV == ModulePtr->JobStep))
    {
        ModulePtr->JobStep       = NVM_JOB_STEP_READ_2ND_NV;
        ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        CurRunningPtr->Index     = 1u;
    }
    else
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
        NvM_JobOverSetFlag(NVM_REQ_NV_INVALIDATED, NVM_CRC_DELETE);
    }
}

/**
 * @brief     If redundant block, and the job step is to read the second nv block, set the result to ok, and the job
 * step is to read the first nv block, etc. Otherwise, if write check is configured, the write check request is switched
 * to the write check request and the result is set to ok
 */
NVM_LOCAL void NvM_MemIfWriteOk(void)
{
    NvM_ModuleType*                ModulePtr          = &NvM_Module;
    NvM_CurRunningType*            CurRunningPtr      = &NvM_CurRunning;
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

    ModulePtr->MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if ((NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType) && (NVM_JOB_STEP_WRITE_2ND_NV == ModulePtr->JobStep))
    {
        CurRunningPtr->ESingleReqResult  = NVM_REQ_OK;
        ModulePtr->JobStep               = NVM_JOB_STEP_WRITE_1ST_NV;
        ModulePtr->MemIfJobState         = NVM_MEMIF_JOB_ASYNC_READY;
        CurRunningPtr->Index             = 0u;
        CurRunningPtr->WriteTimesCounter = BlockDescriptorPtr->NvMMaxNumOfWriteRetries + 1u;
    }
    else
    {
#if (NVM_WRITE_VERIFY == STD_ON)
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEVERIFICATION))
        /* PRQA S 2844 -- */
        {
            ModulePtr->JobStep       = NVM_JOB_STEP_READ_1ST_NV;
            ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            NvM_AtomJobReq(NVM_ATOMJOB_WRITEVERIFICATION);
        }
        else
#endif
        {
            NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
            if (NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType)
            {
                NVM_SETFLAGOFF(&CurRunningPtr->AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
            }
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
#if (STD_ON == NVM_DEM_E_HARDWARE)
            Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_PASSED);
#endif
        }
    }
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
    Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
}

/**
 * @brief     If the number of write retries is not 0, set the memif job status to ready. If the second nv block is read
 * for redundant block, reset the number of write retries and update the corresponding status and result; If the second
 * nv block of the redundant block is successfully read, the write check request is initiated or the result is set to
 * ok; otherwise, the result is set to not ok
 */
NVM_LOCAL void NvM_MemIfWriteFailed(void)
{
    NvM_ModuleType*                ModulePtr          = &NvM_Module;
    NvM_CurRunningType*            CurRunningPtr      = &NvM_CurRunning;
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

    ModulePtr->MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (0U < CurRunningPtr->WriteTimesCounter)
    {
        ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
    else if (
        (NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType) && (NVM_JOB_STEP_WRITE_2ND_NV == ModulePtr->JobStep))
    {
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        CurRunningPtr->WriteTimesCounter = BlockDescriptorPtr->NvMMaxNumOfWriteRetries + 1u;
        /* PRQA S 2844 -- */
        ModulePtr->JobStep       = NVM_JOB_STEP_WRITE_1ST_NV;
        ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        CurRunningPtr->Index     = 0u;
        NVM_GetRepeatMirrorOperation();
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
        Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
    }
    else
    {
        if ((NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType) && (NVM_REQ_OK == CurRunningPtr->ESingleReqResult))
        {
#if (NVM_WRITE_VERIFY == STD_ON)
            if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_WRITEVERIFICATION))
            {
                ModulePtr->JobStep       = NVM_JOB_STEP_READ_2ND_NV; /** Read 2ND firstly, only 2ND is OK */
                CurRunningPtr->Index     = 1u;
                ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
                NvM_AtomJobReq(NVM_ATOMJOB_WRITEVERIFICATION);
            }
            else
#endif
            {
                NvM_UpdateValidandChangeStatus(STD_ON, STD_OFF);
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_UPDATE);
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

#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_JOB_PRIORITIZATION)
/**
 * @brief     If the block is not redundant, set the result to ok. Otherwise, if the current erasing is the first nv
 * block, set erasing the second nv block; otherwise, set the result to ok.
 */
NVM_LOCAL void NvM_MemIfEraseOk(void)
{
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;

    ModulePtr->MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType)
    {
        if (NVM_JOB_STEP_ERASE_1ST_NV == ModulePtr->JobStep)
        {
            ModulePtr->JobStep       = NVM_JOB_STEP_ERASE_2ND_NV;
            ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            CurRunningPtr->Index     = 1u;
        }
        else
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NVM_SETFLAGOFF(&CurRunningPtr->AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
            /** Both NV block is erased,repair is useless */
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_HARDWARE)
            Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_PASSED);
#endif
        }
    }
    else
    {
        NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
        NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_HARDWARE)
        Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_PASSED);
#endif
    }
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
    Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
}

/**
 * @brief     The low layer processing the erase job failed
 */
NVM_LOCAL void NvM_MemIfEraseFailed(void)
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
    Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
}
#endif

/**
 * @brief     Invalidate the first nv block and the second NV block in turn. If it is not the firstinitall service,
 * otherwise, set the result to ok. If it is not a redundant block and not a firstinitall service, set the result to ok.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_NvM_STMIF */
NVM_LOCAL void NvM_MemIfInvalidOk(void)
{
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;

    ModulePtr->MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType)
    {
        if (NVM_JOB_STEP_INVALID_1ST_NV == ModulePtr->JobStep)
        {
            ModulePtr->JobStep       = NVM_JOB_STEP_INVALID_2ND_NV;
            ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            CurRunningPtr->Index     = 1u;
        }
        else if (NVM_JOB_STEP_INVALID_2ND_NV == ModulePtr->JobStep)
        {
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
            if (CurRunningPtr->ServiceId == NVM_FIRSTINIT_ALL_SERV_ID)
            {
                if (0u < CurRunningPtr->FailedNum)
                {
                    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
                }
                else
                {
                    NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
                }
            }
            else
#endif
            {
                NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
                NVM_SETFLAGOFF(&CurRunningPtr->AdminFlagGroup, NVM_ADMIN_NV_REPAIR);
                /** Both NV block is invalid,repair is useless */
                NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_HARDWARE)
                Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_PASSED);
#endif
            }
        }
        else
        {
            /** idle */
        }
    }
    else
    {
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
        if (CurRunningPtr->ServiceId == NVM_FIRSTINIT_ALL_SERV_ID)
        {
            if (CurRunningPtr->Index < (CurRunningPtr->NvNum - 1u))
            {
                /** Process next Nv block */
                ++CurRunningPtr->Index;
                ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            }
            else
            {
                if (0u < CurRunningPtr->FailedNum)
                {
                    NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
                }
                else
                {
                    NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
                }
            }
        }
        else
#endif
        {
            NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
            NvM_JobOverSetFlag(NVM_REQ_OK, NVM_CRC_DELETE);
#if (STD_ON == NVM_DEM_E_HARDWARE)
            Dem_SetEventStatus(NVM_E_HARDWARE, DEM_EVENT_STATUS_PASSED);
#endif
        }
    }
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
    Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_PASSED);
#endif
}
/* PRQA S 6030 -- */

/**
 * @brief     If the service is not firstinitall, set the result to not ok. Otherwise set the corresponding state for
 * the redundant block, or sent an invalid request
 */
NVM_LOCAL void NvM_MemIfInvalidFailed(void)
{
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;

    ModulePtr->MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_UpdateValidandChangeStatus(STD_OFF, STD_OFF);
#if (NVM_API_CONFIG_CLASS_3 == NVM_API_CONFIG_CLASS)
    if (CurRunningPtr->ServiceId == NVM_FIRSTINIT_ALL_SERV_ID)
    {
        if (CurRunningPtr->Index < (CurRunningPtr->NvNum - 1u))
        {
            ++CurRunningPtr->FailedNum;
            ++CurRunningPtr->Index;
        }
        if (NVM_BLOCK_REDUNDANT == CurRunningPtr->ManagementType)
        {
            if (NVM_JOB_STEP_INVALID_1ST_NV == ModulePtr->JobStep)
            {
                ModulePtr->JobStep       = NVM_JOB_STEP_INVALID_2ND_NV;
                ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
            }
            else
            {
                NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
            }
        }
        else if (NVM_BLOCK_DATASET == CurRunningPtr->ManagementType)
        {
            ModulePtr->MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
        }
        else
        {
            NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
        }
    }
    else
#endif
    {
        NvM_JobOverSetFlag(NVM_REQ_NOT_OK, NVM_CRC_DELETE);
    }
#if (STD_ON == NVM_DEM_E_REQ_FAILED)
    Dem_SetEventStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
}
#endif

#if ((NVM_API_CONFIG_CLASS_1 == NVM_API_CONFIG_CLASS) || (STD_OFF == NVM_JOB_PRIORITIZATION))
/**
 * @brief     When dispatching writeall job, we need to determine whether it is firstall, whether it is the first block,
 * whether all blocks have been written, and update the corresponding status, the result, and obtain the ram address.
 * Otherwise, determine whether blockid is greater than the total number of blocks, and set the corresponding job,
 * result, etc
 */
NVM_LOCAL Std_ReturnType NvM_MultiJobDispatch(void) /* PRQA S 6050 */ /* VL_MTR_NvM_STST3 */
{
    Std_ReturnType      retVal        = E_NOT_OK;
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;
    NvM_MultiJobType*   MultiJobPtr   = &NvM_MultiJob;
    NvM_AdminBlockType* AdminBlockPtr = NULL_PTR;

    ++CurRunningPtr->BlockId;

    NvM_BlockIdType intBlockId = NvM_GetIntBlockId(CurRunningPtr->BlockId);

    if (NVM_READ_ALL_SERV_ID == CurRunningPtr->ServiceId)
    {
        if (NVM_BLOCK_NUM_ALL < CurRunningPtr->BlockId)
        {
            SchM_Enter_NvM_Queue();
            MultiJobPtr->Enqueue = FALSE;
            SchM_Exit_NvM_Queue();
            if (NVM_REQ_NOT_OK == CurRunningPtr->EMultiReqResult)
            {
                NvM_MultiJobResultFeedBack(CurRunningPtr->ServiceId, NVM_REQ_NOT_OK);
            }
            else
            {
                NvM_MultiJobResultFeedBack(CurRunningPtr->ServiceId, NVM_REQ_OK);
            }
            SchM_Enter_NvM_Queue();
            CurRunningPtr->EMultiReqResult = NVM_REQ_OK;
            ModulePtr->CurrentJobType      = NVM_JOB_TYPE_NONE;
            SchM_Exit_NvM_Queue();
        }
        else
        {
            AdminBlockPtr = &(NvM_AdminBlock[intBlockId]);

            SchM_Enter_NvM_Queue();
            AdminBlockPtr->ServiceID = CurRunningPtr->ServiceId;
            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            AdminBlockPtr->SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            SchM_Exit_NvM_Queue();
            NvM_SingleJobResultFeedBack(CurRunningPtr->BlockId, NVM_REQ_PENDING);
            retVal = E_OK;
        }
    }
    else
    { /* PRQA S 1881 ++ */ /*VL_NvM_1881 */
        if ((NVM_WRITEALL_FIRST_BLOCKID == CurRunningPtr->BlockId)
            || (NVM_SET_RAM_BLOCK_STATUS_SERV_ID == CurRunningPtr->ServiceId)
            || ((NVM_BLOCK_NUM_ALL < CurRunningPtr->BlockId) && (STD_OFF == MultiJobPtr->ID1WriteMark)))
        {
            /* PRQA S 1881 -- */
            SchM_Enter_NvM_Queue();
            MultiJobPtr->Enqueue = FALSE;
            SchM_Exit_NvM_Queue();
            if (NVM_REQ_NOT_OK == CurRunningPtr->EMultiReqResult)
            {
                NvM_MultiJobResultFeedBack(CurRunningPtr->ServiceId, NVM_REQ_NOT_OK);
            }
            else
            {
                NvM_MultiJobResultFeedBack(CurRunningPtr->ServiceId, NVM_REQ_OK);
            }
            SchM_Enter_NvM_Queue();
            CurRunningPtr->EMultiReqResult = NVM_REQ_OK;
            ModulePtr->CurrentJobType      = NVM_JOB_TYPE_NONE;
            SchM_Exit_NvM_Queue();
        } /* PRQA S 2995 ++ */ /*VL_NvM_2995 */
        else if ((NVM_BLOCK_NUM_ALL < CurRunningPtr->BlockId) && (TRUE == MultiJobPtr->ID1WriteMark))
        {
            /* PRQA S 2995 -- */
            /** pending the job and clear the write flag */
            SchM_Enter_NvM_Queue();
            CurRunningPtr->BlockId    = 1u;
            intBlockId                = 0u;
            MultiJobPtr->ID1WriteMark = FALSE;

            AdminBlockPtr                                     = &(NvM_AdminBlock[intBlockId]);
            const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

            CurRunningPtr->ServiceId = NVM_WRITE_ALL_SERV_ID;
            AdminBlockPtr->ServiceID = NVM_WRITE_ALL_SERV_ID;

            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            /* PRQA S 3120,0489 ++ */ /* VL_QAC_MagicNum,VL_NvM_0489 */
            *(BlockDescriptorPtr->NvmRamBlockDataAddress) = (uint8)(((uint16)(NVM_COMPILED_CONFIG_ID)) & 0x00ffU);
            *((BlockDescriptorPtr->NvmRamBlockDataAddress) + 1U) = (uint8)(((uint16)(NVM_COMPILED_CONFIG_ID)) >> 8U);
            /* PRQA S 3120,0489 -- */
            AdminBlockPtr->SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            SchM_Exit_NvM_Queue();
            NvM_SingleJobResultFeedBack(CurRunningPtr->BlockId, NVM_REQ_PENDING);
            retVal = E_OK;
        }
        else
        {
            AdminBlockPtr = &(NvM_AdminBlock[intBlockId]);

            /** pending the job */
            SchM_Enter_NvM_Queue();
            CurRunningPtr->ServiceId = NVM_WRITE_ALL_SERV_ID;
            AdminBlockPtr->ServiceID = NVM_WRITE_ALL_SERV_ID;
            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            AdminBlockPtr->SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            SchM_Exit_NvM_Queue();
            NvM_SingleJobResultFeedBack(CurRunningPtr->BlockId, NVM_REQ_PENDING);
            retVal = E_OK;
        }
    }
    return retVal;
}
#endif

#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
/**
 * @brief     If the multijob is writeall, the corresponding interface is called.
 */
NVM_LOCAL void NvM_JobDequeueSubDealOk(uint8 jobQueueIndex)
{
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;

    SchM_Enter_NvM_Queue();
    /** transfer the job need processing to global variant */
    ModulePtr->CurrentJobType = NVM_JOB_TYPE_SINGLE_NORMAL;
    CurRunningPtr->BlockId    = NvM_StandQueue[jobQueueIndex].BlockId; /* PRQA S 2844 */ /* VL_NvM_2844 */
    CurRunningPtr->ServiceId  = NvM_StandQueue[jobQueueIndex].ServiceId;
    NvM_GetRamAddress(jobQueueIndex);
    SchM_Exit_NvM_Queue();
    /** after transfer the job to global variant, need release the space */
    NvM_RelQueueSpace(ModulePtr->CurrentJobType, jobQueueIndex);
    SchM_Enter_NvM_Queue();
    NvM_CopyCurBlockInfo();
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    NVM_SETFLAGOFF(&NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].FlagGroup, NVM_ADMIN_ENQUEUE);
    /* PRQA S 2844 -- */
    SchM_Exit_NvM_Queue();
}

/**
 * @brief     If the multijob is writeall, the corresponding interface is called.
 */
NVM_LOCAL Std_ReturnType NvM_JobDequeueSub(void)
{
    Std_ReturnType      retVal;
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;
    NvM_MultiJobType*   MultiJobPtr   = &NvM_MultiJob;

    if ((NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == ModulePtr->CurrentJobType)
        || (NVM_JOB_TYPE_MULTI == ModulePtr->CurrentJobType))
    {
        if (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == ModulePtr->CurrentJobType)
        {
            ModulePtr->CurrentJobType = NVM_JOB_TYPE_MULTI;
            CurRunningPtr->BlockId    = MultiJobPtr->BlockId;
            CurRunningPtr->ServiceId  = MultiJobPtr->ServiceId;
        }
        /** switch low layer mode to slow mode after multiple type job is processed over */
        retVal = NvM_MultiJobDispatchWithPri();
    }
    else
    {
        /** get the highest priority from the priority table */
        uint8 jobQueueIndex;

        retVal = NvM_GetHighestPriJob(&jobQueueIndex);
        if (E_OK == retVal)
        {
            NvM_JobDequeueSubDealOk(jobQueueIndex);
        }
        else if (TRUE == MultiJobPtr->Enqueue)
        {
            SchM_Enter_NvM_Queue();
            ModulePtr->CurrentJobType = NVM_JOB_TYPE_MULTI;
            CurRunningPtr->ServiceId  = MultiJobPtr->ServiceId;
            if (NVM_WRITE_ALL_SERV_ID == CurRunningPtr->ServiceId)
            {
                MultiJobPtr->BlockId = NVM_WRITEALL_FIRST_BLOCKID;
            }
            else
            {
                MultiJobPtr->BlockId = NVM_READALL_FIRST_BLOCKID;
            }
            CurRunningPtr->BlockId = MultiJobPtr->BlockId;
            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            NvM_AdminBlock[CurRunningPtr->BlockId - 1U].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            SchM_Exit_NvM_Queue();
            retVal = E_OK;
        }
        else
        {
            retVal = E_NOT_OK;
        }
    }
    return retVal;
}

/**
 * @brief     If the multijob is writeall, the corresponding interface is called. Otherwise, get the multi-job blockid
 * value, if blockid is invalid, can not join the team and set the status, the result is not ok, otherwise get the
 * corresponding block information and process
 */
NVM_LOCAL Std_ReturnType NvM_MultiJobDispatchWithPri(void)
{
    Std_ReturnType      retVal        = E_NOT_OK;
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;
    NvM_MultiJobType*   MultiJobPtr   = &NvM_MultiJob;

    if (NVM_WRITE_ALL_SERV_ID == MultiJobPtr->ServiceId)
    {
        /** WriteAll Operation */
        retVal = NvM_MultiJobDispatchWrite();
    }
    else
    {
        if ((NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED != ModulePtr->CurrentJobType) || (1u != MultiJobPtr->BlockId))
        {
            ++MultiJobPtr->BlockId;
        }

        if (NVM_BLOCK_NUM_ALL < MultiJobPtr->BlockId)
        {
            if (0u != NvM_PriorityTable[1u - ModulePtr->CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U])
            {
                /** switch the current active priority table */
                ModulePtr->CurActPriTable = 1u - ModulePtr->CurActPriTable;
            }
            MultiJobPtr->Enqueue = FALSE;
            if (NVM_REQ_NOT_OK == CurRunningPtr->EMultiReqResult)
            {
                NvM_MultiJobResultFeedBack(MultiJobPtr->ServiceId, NVM_REQ_NOT_OK);
            }
            else
            {
                NvM_MultiJobResultFeedBack(MultiJobPtr->ServiceId, NVM_REQ_OK);
            }
            CurRunningPtr->EMultiReqResult = NVM_REQ_OK;
            ModulePtr->CurrentJobType      = NVM_JOB_TYPE_NONE; /** Current job been finished */
        }
        else
        {
            /** handle the next blockID for MultiJob */
            ModulePtr->CurrentJobType = NVM_JOB_TYPE_MULTI;
            CurRunningPtr->BlockId    = MultiJobPtr->BlockId;
            CurRunningPtr->ServiceId  = MultiJobPtr->ServiceId;

            NvM_BlockIdType     intBlockId    = NvM_GetIntBlockId(CurRunningPtr->BlockId);
            NvM_AdminBlockType* AdminBlockPtr = &(NvM_AdminBlock[intBlockId]);

            AdminBlockPtr->ServiceID = MultiJobPtr->ServiceId;
            NvM_GetRamAddress(USELESS_FOR_MULTIJOB);
            AdminBlockPtr->SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            NvM_SingleJobResultFeedBack(CurRunningPtr->BlockId, NVM_REQ_PENDING);
            retVal = E_OK;
        }
    }
    return retVal;
}

/**
 * @brief     If the current block is out of the block range and the first block cannot be written, set the queuing flag
 * to off and invoke the notification interface. Otherwise, obtain the information of each block successively to perform
 * write operations. If the first block needs to be written, obtain the information about the first block and perform
 * the write operation
 */
NVM_LOCAL Std_ReturnType NvM_MultiJobDispatchWrite(void)
{
    Std_ReturnType      retVal        = E_NOT_OK;
    NvM_ModuleType*     ModulePtr     = &NvM_Module;
    NvM_CurRunningType* CurRunningPtr = &NvM_CurRunning;
    NvM_MultiJobType*   MultiJobPtr   = &NvM_MultiJob;
    NvM_AdminBlockType* AdminBlockPtr = NULL_PTR;
    NvM_BlockIdType     intBlockId;

    ++MultiJobPtr->BlockId;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    if ((2U == MultiJobPtr->BlockId)
        || ((NVM_BLOCK_NUM_ALL < MultiJobPtr->BlockId) && (FALSE == MultiJobPtr->ID1WriteMark)))
    /* PRQA S 3120 -- */
    {
        if (0U != NvM_PriorityTable[1U - ModulePtr->CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U])
        {
            /** switch the current active priority table */
            ModulePtr->CurActPriTable = 1U - ModulePtr->CurActPriTable;
        }
        MultiJobPtr->Enqueue = FALSE;

        if (NVM_REQ_NOT_OK == CurRunningPtr->EMultiReqResult)
        {
            NvM_MultiJobResultFeedBack(MultiJobPtr->ServiceId, NVM_REQ_NOT_OK);
        }
        else
        {
            NvM_MultiJobResultFeedBack(MultiJobPtr->ServiceId, NVM_REQ_OK);
        }
        CurRunningPtr->EMultiReqResult = NVM_REQ_OK;
        ModulePtr->CurrentJobType      = NVM_JOB_TYPE_NONE;
    }
    else if (NVM_BLOCK_NUM_ALL >= MultiJobPtr->BlockId)
    {
        /** copy the current job information to global variant to process */
        ModulePtr->CurrentJobType = NVM_JOB_TYPE_MULTI;
        CurRunningPtr->BlockId    = MultiJobPtr->BlockId;
        CurRunningPtr->ServiceId  = MultiJobPtr->ServiceId;

        NvM_GetRamAddress(USELESS_FOR_MULTIJOB);

        intBlockId = NvM_GetIntBlockId(CurRunningPtr->BlockId);

        AdminBlockPtr = &(NvM_AdminBlock[intBlockId]);

        AdminBlockPtr->SingleReqResult = NVM_REQ_PENDING; /* PRQA S 2844 */ /* VL_NvM_2844 */
        NvM_CopyCurBlockInfo();
        NvM_SingleJobResultFeedBack(CurRunningPtr->BlockId, NVM_REQ_PENDING);
        retVal = E_OK;
    }
    /** If Block1 should write, then Block1 should the last block to handle */
    else if (TRUE == MultiJobPtr->ID1WriteMark) /* PRQA S 2991,2995 */ /* VL_QAC_2991,VL_NvM_2995 */
    {
        /** copy the current job information to global variant to process */
        MultiJobPtr->BlockId      = 1u;
        MultiJobPtr->ID1WriteMark = FALSE;
        CurRunningPtr->BlockId    = MultiJobPtr->BlockId;
        CurRunningPtr->ServiceId  = MultiJobPtr->ServiceId;

        intBlockId = NvM_GetIntBlockId(CurRunningPtr->BlockId);

        AdminBlockPtr = &(NvM_AdminBlock[intBlockId]);

        AdminBlockPtr->ServiceID = MultiJobPtr->ServiceId;

        const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

        /* PRQA S 2814, 3120 ++ */ /* VL_QAC_DerefNullPtr,VL_QAC_MagicNum */
        *(BlockDescriptorPtr->NvmRamBlockDataAddress) = (uint8)(((uint16)(NVM_COMPILED_CONFIG_ID)) & 0x00ffU);
        /* PRQA S 2814, 3120 -- */
        /* PRQA S 2824, 0489, 3120 ++ */ /* VL_NvM_2824,VL_NvM_0489,VL_QAC_MagicNum */
        *(BlockDescriptorPtr->NvmRamBlockDataAddress + 1U) = (uint8)(((uint16)(NVM_COMPILED_CONFIG_ID)) >> 8U);
        /* PRQA S 2824, 0489, 3120 -- */
        CurRunningPtr->RamAddr         = BlockDescriptorPtr->NvmRamBlockDataAddress;
        AdminBlockPtr->SingleReqResult = NVM_REQ_PENDING;
        NvM_CopyCurBlockInfo();
        NvM_SingleJobResultFeedBack(CurRunningPtr->BlockId, NVM_REQ_PENDING);
        retVal = E_OK;
    }
    else
    {
        /** idle */
    }
    return retVal;
}
#endif

/**
 * @brief     According to whether the current job is immediate data, multi-job, single job, etc., the ram address
 * corresponding to each block is obtained
 */
/* PRQA S 6030 ++ */ /* VL_MTR_NvM_STMIF */
NVM_LOCAL void NvM_GetRamAddress(uint8 QueueIndex)
{
    const NvM_ModuleType*          ModulePtr          = &NvM_Module;
    NvM_CurRunningType*            CurRunningPtr      = &NvM_CurRunning;
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
    NvM_AdminBlockType*            AdminBlockPtr      = &(NvM_AdminBlock[intBlockId]);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

#if (STD_ON == NVM_JOB_PRIORITIZATION)
    if ((ModulePtr->CurrentJobType == NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED)
        || (ModulePtr->CurrentJobType == NVM_JOB_TYPE_SINGLE_IMMED))
    {
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if (NULL_PTR != NvM_ImmedQueue[QueueIndex].DestSrcPtr)
        {
            CurRunningPtr->RamAddr = NvM_ImmedQueue[QueueIndex].DestSrcPtr;
            NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
            NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
        }
#if (STD_ON == NVM_BLOCK_USE_SYNC_MECHANISM)
        else if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_SYNCMECHANISM))
        {
#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
            CurRunningPtr->RamAddr = NVM_RamMirror;
#endif
            NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
            NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
        }
        else
#endif
        {
            CurRunningPtr->RamAddr = BlockDescriptorPtr->NvmRamBlockDataAddress;
            NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
            NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
        }
        /* PRQA S 2844 -- */
    }
    else if (ModulePtr->CurrentJobType == NVM_JOB_TYPE_MULTI)
#else
    if (ModulePtr->CurrentJobType == NVM_JOB_TYPE_MULTI)
#endif
    {
        if ((TRUE == NVM_ISFLAGON(AdminBlockPtr->FlagGroup, NVM_ADMIN_NV_REPAIR))
            && (NVM_WRITE_ALL_SERV_ID == CurRunningPtr->ServiceId))
        {
            /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
            NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
            /* PRQA S 2844 -- */
        }
        else
        {
            NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
#if (STD_ON == NVM_BLOCK_USE_SYNC_MECHANISM)
            if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_SYNCMECHANISM))
            {
#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
                CurRunningPtr->RamAddr = NVM_RamMirror;
#endif
                /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
                /* PRQA S 2844 -- */
            }
            else
#endif
            {
                CurRunningPtr->RamAddr = BlockDescriptorPtr->NvmRamBlockDataAddress;
                NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
            }
        }
    }
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    else if (ModulePtr->CurrentJobType == NVM_JOB_TYPE_SINGLE_NORMAL)
    {
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        if (NULL_PTR != NvM_StandQueue[QueueIndex].DestSrcPtr)
        {
            CurRunningPtr->RamAddr = NvM_StandQueue[QueueIndex].DestSrcPtr;
            NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
            NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
        }
#if (STD_ON == NVM_BLOCK_USE_SYNC_MECHANISM)
        else if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_SYNCMECHANISM))
        {
#if (NVM_MAX_LENGTH_CONFIGED_RAM_MIRROR > 0)
            CurRunningPtr->RamAddr = NVM_RamMirror;
#endif
            NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
            NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
        }
#endif
        else
        {
            CurRunningPtr->RamAddr = BlockDescriptorPtr->NvmRamBlockDataAddress;
            NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAM_VALID_CHANGE_STATUS_USED);
            NVM_SETFLAGOFF(&AdminBlockPtr->FlagGroup, NVM_ADMIN_RAMMIRROR_OR_NOT);
        }
        /* PRQA S 2844 -- */
    }
#endif
    else
    {
        /** idle */
    }
}
/* PRQA S 6030 -- */

/**
 * @brief     Copy current block infomation to global variant
 */
NVM_LOCAL void NvM_CopyCurBlockInfo(void)
{
    NvM_CurRunningType*            CurRunningPtr      = &NvM_CurRunning;
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(CurRunningPtr->BlockId);
    NvM_AdminBlockType*            AdminBlockPtr      = &(NvM_AdminBlock[intBlockId]);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    CurRunningPtr->ManagementType = BlockDescriptorPtr->NvmBlockManagementType;
    /* PRQA S 2844 -- */
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET != CurRunningPtr->ManagementType)
    {
        AdminBlockPtr->CurrentIndex = 0u; /* PRQA S 2844 */ /* VL_NvM_2844 */
    }
#else
    AdminBlockPtr->CurrentIndex = 0u;
#endif

    NVM_GetRepeatMirrorOperation();
    CurRunningPtr->Index          = AdminBlockPtr->CurrentIndex; /* PRQA S 2844 */ /* VL_NvM_2844 */
    CurRunningPtr->FailedNum      = 0u;
    CurRunningPtr->StaticId       = 0xFFFFu; /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    CurRunningPtr->AdminFlagGroup = AdminBlockPtr->FlagGroup;
    if (BlockDescriptorPtr->NvmBlockCrcBuffAddress != NULL_PTR)
    {
        CurRunningPtr->Crc = BlockDescriptorPtr->NvmBlockCrcBuffAddress[CurRunningPtr->Index];
    }
    CurRunningPtr->ESingleReqResult    = AdminBlockPtr->SingleReqResult;
    CurRunningPtr->WriteTimesCounter   = BlockDescriptorPtr->NvMMaxNumOfWriteRetries + 1U;
    CurRunningPtr->ReadRetryCounter    = BlockDescriptorPtr->NvMMaxNumOfReadRetries;
    CurRunningPtr->VerificationSize    = BlockDescriptorPtr->NvMWriteVerificationDataSize;
    CurRunningPtr->VerificationCounter = 0u;
    CurRunningPtr->DeviceId            = BlockDescriptorPtr->NvmNvramDeviceId;
    CurRunningPtr->NvNum               = BlockDescriptorPtr->NvmNvBlockNum;
    CurRunningPtr->RomNum              = BlockDescriptorPtr->NvmRomBlockNum;
    CurRunningPtr->CRCType             = BlockDescriptorPtr->NvmBlockCRCType;
    CurRunningPtr->BaseNumber          = BlockDescriptorPtr->NvmNvBlockBaseNumber;
    CurRunningPtr->Length              = BlockDescriptorPtr->NvmNvBlockLength;
#if (0xFFFFU != NVM_CRC_NUM_OF_BYTES)
    CurRunningPtr->CrcFlag = 0u;
#endif

#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET != CurRunningPtr->ManagementType)
    {
        /* The ROM memory address of the given block */
        CurRunningPtr->RomAddr = BlockDescriptorPtr->NvmRomBlockDataAddress;
    }
    else if (CurRunningPtr->NvNum <= CurRunningPtr->Index)
    {
        /* PRQA S 2824 ++ */ /* VL_NvM_2824 */
        uint8 offset = (CurRunningPtr->Index - CurRunningPtr->NvNum) * (uint8)CurRunningPtr->Length;

        /* The ROM memory address of the given block */
        CurRunningPtr->RomAddr = &(BlockDescriptorPtr->NvmRomBlockDataAddress[offset]);
        /* PRQA S 2824 -- */
    }
    else
    {
        /** None will not happen */
    }
#else
    CurRunningPtr->RomAddr = BlockDescriptorPtr->NvmRomBlockDataAddress;
#endif
#if (STD_ON == NVM_BLOCK_USE_SYNC_MECHANISM)
    if (TRUE == NVM_ISFLAGON(BlockDescriptorPtr->FlagGroup, (uint16)NVM_BLOCK_DESC_SYNCMECHANISM))
    {
        CurRunningPtr->NvM_WriteRamBlockToNvm  = BlockDescriptorPtr->NvM_WriteRamBlockToNvm;
        CurRunningPtr->NvM_ReadRamBlockFromNvm = BlockDescriptorPtr->NvM_ReadRamBlockFromNvm;
    }
#endif
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    CurRunningPtr->InitCallback = BlockDescriptorPtr->NvmInitBlockCallback;
#if (STD_ON == NVM_CIPHERING_ENABLE)
    CurRunningPtr->NvMCsmRetryCounter = NVM_CSM_RETRY_COUNTER;
#endif
#endif
    CurRunningPtr->SingleCallback = BlockDescriptorPtr->NvmSingleBlockCallback;
}

#if ((NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS) && (STD_ON == NVM_JOB_PRIORITIZATION))
/**
 * @brief     Find the first setted bit of a given 16 bits source number
 */
NVM_LOCAL Std_ReturnType NvM_Get16Bits1stSettedBit(uint16 Number, uint8* SettedBitPtr)
{
    Std_ReturnType retVal = E_OK;

    if (0x0000u == Number)
    {
        /** the number equal 0, can not find the setted bit */
        retVal = E_NOT_OK;
    }
    else
    {
        uint8  bitsLoop = 0x00u;
        uint16 baseNum  = 0x0001u;

        do
        {
            if (0u != (baseNum & Number))
            {
                *SettedBitPtr = bitsLoop; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
                break;
            }
            baseNum = (uint16)(baseNum << 1u);
            ++bitsLoop;
        } while (16u > bitsLoop); /* PRQA S 3120 */ /* VL_QAC_MagicNum */
    }
    return retVal;
}

/**
 * @brief     Determine whether the immediate queue overflows and set the header and tail indexes
 */
NVM_LOCAL Std_ReturnType NvM_GetQueueSpaceSingleImmed(uint8* JobQueueIndexPtr)
{
    Std_ReturnType                 retVal              = E_OK;
    NvM_RoundRobinQueueManageType* ImmedQueueManagePtr = &NvM_ImmedQueueManage;

    if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= ImmedQueueManagePtr->Count)
    {
        /** Job queue overflow */
        retVal = E_NOT_OK;
    }
    else
    {
        SchM_Enter_NvM_Queue();
        *JobQueueIndexPtr = ImmedQueueManagePtr->TailIndex; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        ++ImmedQueueManagePtr->TailIndex;
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= ImmedQueueManagePtr->TailIndex)
        {
            /** prevent index overflow, reset it */
            ImmedQueueManagePtr->TailIndex = 0u;
        }
        ++ImmedQueueManagePtr->Count;
        SchM_Exit_NvM_Queue();
    }
    return retVal;
}

/**
 * @brief     Obtain the queue information based on the index value in the queue where the task resides
 */
NVM_LOCAL Std_ReturnType NvM_GetQueueSpaceSub(uint8 jobQueueGroup, uint8* JobQueueIndexPtr)
{
    uint8 jobQueueBit = 0x00u;
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    Std_ReturnType retVal = NvM_Get16Bits1stSettedBit(NvM_QueueSpaceTalbe[jobQueueGroup], &jobQueueBit);
    /* PRQA S 2844 -- */
    uint8 jobQueueIndex = (uint8)(jobQueueGroup << 4u) + jobQueueBit; /* PRQA S 3120 */ /* VL_QAC_MagicNum */

    if (E_NOT_OK == retVal)
    {
        /** Flag error happened, Correct the mistakes */

        /* PRQA S 4397,4399,4391 ++*/ /* VL_NvM_4397,VL_NvM_4399,VL_NvM_4391 */
        NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] &= (uint16)(~(0x01u << jobQueueGroup));
    }
    else if (NVM_SIZE_STANDARD_JOB_QUEUE <= jobQueueIndex)
    {
        /** Initial error happened, Correct the mistakes */
        retVal = E_NOT_OK;
    }
    else
    {
        SchM_Enter_NvM_Queue();
        *JobQueueIndexPtr = jobQueueIndex; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
        /** clear the job queue space table, mark it as not available */
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        NvM_QueueSpaceTalbe[jobQueueGroup] &= (uint16)(~(0x01u << jobQueueBit));

        if (0u == NvM_QueueSpaceTalbe[jobQueueGroup])
        /* PRQA S 2844 -- */
        {
            /** the group's space are all in using, clear the group flag */
            NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] &= (uint16)(~(0x01u << jobQueueGroup));
            /* PRQA S 4397,4399,4391 --*/
        }
        SchM_Exit_NvM_Queue();
    }
    return retVal;
}

/**
 * @brief     Based on the incoming job type, determine whether the corresponding queue overflows and obtain the queue
 * space
 */
NVM_LOCAL Std_ReturnType NvM_GetQueueSpace(NvM_JobTypeType JobType, uint8* JobQueueIndexPtr)
{
    Std_ReturnType retVal = E_OK;

    if (NVM_JOB_TYPE_SINGLE_IMMED == JobType)
    {
        retVal = NvM_GetQueueSpaceSingleImmed(JobQueueIndexPtr);
    }
    else if (NVM_JOB_TYPE_SINGLE_NORMAL == JobType)
    {
        uint8 jobQueueGroup = 0x00u;

        retVal = NvM_Get16Bits1stSettedBit(NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U], &jobQueueGroup);
        /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
        if ((E_NOT_OK == retVal) || (((NVM_SIZE_STANDARD_JOB_QUEUE - 1U) >> 4U) < jobQueueGroup))
        /* PRQA S 3120 -- */
        {
            if (E_NOT_OK != retVal)
            {
                /** Initial error happened, Correct the mistakes */
                /* PRQA S 4397,4399,4391 ++*/ /* VL_NvM_4397,VL_NvM_4399,VL_NvM_4391 */
                NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] &= (uint16)(~(0x01u << jobQueueGroup));
                /* PRQA S 4397,4399,4391 --*/
                retVal = E_NOT_OK;
            }
            /** Job queue overflow */
        }
        else
        {
            retVal = NvM_GetQueueSpaceSub(jobQueueGroup, JobQueueIndexPtr);
        }
    }
    else
    {
        /** None will not happen */
    }
    return retVal;
}

/**
 * @brief     Release the queue space based on the incoming job type and based on the incoming queue index
 */
NVM_LOCAL void NvM_RelQueueSpace(NvM_JobTypeType JobType, uint8 JobQueueIndex)
{
    SchM_Enter_NvM_Queue();
    if ((NVM_JOB_TYPE_SINGLE_IMMED == JobType) || (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == JobType))
    {
        NvM_RoundRobinQueueManageType* ImmedQueueManagePtr = &NvM_ImmedQueueManage;

        NvM_ImmedQueue[ImmedQueueManagePtr->HeadIndex].BlockId = 0u; /** reset the block id when it dequeue */
        ++ImmedQueueManagePtr->HeadIndex;
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= ImmedQueueManagePtr->HeadIndex)
        {
            /** round robin queue, prvent index overflow, reset it */
            ImmedQueueManagePtr->HeadIndex = 0u;
        }
        --ImmedQueueManagePtr->Count;
    }
    else if (NVM_JOB_TYPE_SINGLE_NORMAL == JobType)
    {
        /** set job queue table flag */

        uint8 queueLowBits  = JobQueueIndex & 0x0FU; /* PRQA S 3120 */                  /* VL_QAC_MagicNum */
        uint8 queueHighBits = (uint8)((JobQueueIndex & 0xF0U) >> 4U); /* PRQA S 3120 */ /* VL_QAC_MagicNum */

        NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1U] |= (uint16)((uint16)0x01U << queueHighBits);
        /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
        NvM_QueueSpaceTalbe[queueHighBits] |= (uint16)((uint16)0x01U << queueLowBits);
        /* PRQA S 2844 -- */
    }
    else
    {
        /** None Will not happen */
    }
    SchM_Exit_NvM_Queue();
}

/**
 * @brief     If the value of the high priority is within the valid range, the corresponding priority queue information
 * is obtained
 */
NVM_LOCAL Std_ReturnType NvM_GetHighestPriJobSub(uint8 curActPriTable, uint8 priGroup, uint8* JobQueueIndexPtr)
{
    uint8 priBit = 0u;
    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    Std_ReturnType retVal = NvM_Get16Bits1stSettedBit(NvM_PriorityTable[curActPriTable][priGroup], &priBit);
    /* PRQA S 2844 -- */

    if (E_NOT_OK == retVal)
    {
        /** Flag error happened, Correct the mistakes */
        /* PRQA S 4397,4399,4391 ++*/ /* VL_NvM_4397,VL_NvM_4399,VL_NvM_4391 */
        NvM_PriorityTable[curActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] &= (uint16)(~(0x01u << priGroup));
    }
    else
    {
        uint8 priHighest = (uint8)(priGroup << 4u) + priBit; /* PRQA S 3120 */ /* VL_QAC_MagicNum */

        if (NVM_TOTAL_NUM_DIFF_PRI <= priHighest)
        {
            /** Flag error happened, Correct the mistakes */
            NvM_PriorityTable[curActPriTable][priGroup] &= (uint16)(~(0x01u << priBit));
            retVal = E_NOT_OK;
        }
        else
        {
            NvM_PriTable2QueueType* PriTable2QueuePtr = &(NvM_PriTable2Queue[curActPriTable][priHighest]);

            SchM_Enter_NvM_Queue();

            /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
            uint8 samePriHeadIndex = PriTable2QueuePtr->HeadIndex;

            /* PRQA S 2844 -- */
            *JobQueueIndexPtr = samePriHeadIndex; /* PRQA S 2814 */ /* VL_QAC_DerefNullPtr */
            if (PriTable2QueuePtr->HeadIndex == PriTable2QueuePtr->TailIndex)
            {
                /** This is the last element, clear priority table flag */ /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                NvM_PriorityTable[curActPriTable][priGroup] &= (uint16)(~(0x01u << priBit));
                if (0U == NvM_PriorityTable[curActPriTable][priGroup])
                /* PRQA S 2844 -- */
                {
                    NvM_PriorityTable[curActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] &= (uint16)(~(0x01u << priGroup));
                    /* PRQA S 4397,4399,4391 --*/
                }
            }
            else
            {
                /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                PriTable2QueuePtr->HeadIndex               = NvM_StandQueue[samePriHeadIndex].NextIndex;
                NvM_StandQueue[samePriHeadIndex].NextIndex = samePriHeadIndex;
                /* PRQA S 2844 -- */
            }

            SchM_Exit_NvM_Queue();
        }
    }
    return retVal;
}

/**
 * @brief     If the number of priority groups is greater than the number of priorities, it is considered an error.
 * Otherwise, the value of the high priority is obtained
 */
NVM_LOCAL Std_ReturnType NvM_GetHighestPriJob(uint8* JobQueueIndexPtr)
{
    uint8          priGroup = 0x0u;
    Std_ReturnType retVal   = E_NOT_OK;

    uint8 curActPriTable = NvM_Module.CurActPriTable;

    /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
    retVal = NvM_Get16Bits1stSettedBit(NvM_PriorityTable[curActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U], &priGroup);
    /* PRQA S 2844 -- */ /* VL_NvM_2844 */
    if (E_OK == retVal)
    {
        if (NVM_TABLE_SIZE_PRIORITY < (uint8)(priGroup + 2u)) /* PRQA S 3120 */ /* VL_QAC_MagicNum */
        {
            /** Initial error happened, Correct the mistakes */
            /* PRQA S 4397,4399,4391 ++*/ /* VL_NvM_4397,VL_NvM_4399,VL_NvM_4391 */
            NvM_PriorityTable[curActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] &= (uint16)(~(0x01u << priGroup));
            /* PRQA S 4397,4399,4391 --*/
            retVal = E_NOT_OK;
        }
        else
        {
            retVal = NvM_GetHighestPriJobSub(curActPriTable, priGroup, JobQueueIndexPtr);
        }
    }
    return retVal;
}

/**
 * @brief     If the multijob is writeall,
 */
/* PRQA S 6070 ++ */ /* VL_MTR_NvM_STCAL */
NVM_LOCAL Std_ReturnType
    NvM_JobEnqueueSingleJob(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8* DestSrcPtr)
/* PRQA S 6070 -- */
{
    Std_ReturnType                 retVal             = E_OK;
    NvM_BlockIdType                intBlockId         = NvM_GetIntBlockId(BlockId);
    NvM_AdminBlockType*            AdminBlockPtr      = &(NvM_AdminBlock[intBlockId]);
    const NvM_BlockDescriptorType* BlockDescriptorPtr = &(NvM_BlockDescriptor[intBlockId]);

    if (TRUE == NVM_ISFLAGOFF(AdminBlockPtr->FlagGroup, NVM_ADMIN_ENQUEUE)) /* PRQA S 2844 */ /* VL_NvM_2844 */
    {
        SchM_Enter_NvM_Queue();
        NvM_JobTypeType jobType;
        AdminBlockPtr->ServiceID = ServiceId; /* PRQA S 2844 */                               /* VL_NvM_2844 */
        uint8 jobPri             = BlockDescriptorPtr->NvmBlockJobPriority; /* PRQA S 2844 */ /* VL_NvM_2844 */

        /** Immedi write */
        if ((0u == jobPri) && ((NVM_WRITE_BLOCK_SERV_ID == ServiceId) || (NVM_WRITE_PRAM_BLOCK_SERV_ID == ServiceId)))
        {
            jobType = NVM_JOB_TYPE_SINGLE_IMMED;
        }
        else
        {
            jobType = NVM_JOB_TYPE_SINGLE_NORMAL;
        }
        SchM_Exit_NvM_Queue();

        uint8 addJobQueueIndex = 0U;

        if (E_NOT_OK == NvM_GetQueueSpace(jobType, &addJobQueueIndex))
        {
            /** Job queue overflow */
            retVal = E_NOT_OK;
#if (STD_ON == NVM_DEV_ERROR_DETECT)
            NvM_Det_ReportRunTimeError((uint8)ServiceId, NVM_E_QUEUE_FULL);
#endif
        }
        else
        {
            if (NVM_JOB_TYPE_SINGLE_IMMED == jobType)
            {
                SchM_Enter_NvM_Queue();
                /** store the job information to queue and set the job state as pending */
                NvM_ImmedQueue[addJobQueueIndex].BlockId    = BlockId; /* PRQA S 2844 */    /* VL_NvM_2844 */
                NvM_ImmedQueue[addJobQueueIndex].DestSrcPtr = DestSrcPtr; /* PRQA S 2844 */ /* VL_NvM_2844 */
                NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_ENQUEUE);
                /* PRQA S 2844 */ /* VL_NvM_2844 */
                SchM_Exit_NvM_Queue();
                NvM_SingleJobResultFeedBack(BlockId, NVM_REQ_PENDING);
            }
            else
            {
                SchM_Enter_NvM_Queue();
                uint8 curActPriTable;
                uint8 priLowBits  = jobPri & 0x0FU; /* PRQA S 3120 */                  /* VL_QAC_MagicNum */
                uint8 priHighBits = (uint8)((jobPri & 0xF0U) >> 4u); /* PRQA S 3120 */ /* VL_QAC_MagicNum */

                /** MultiJob & SingleJob each take one queue */
                if (TRUE == NvM_MultiJob.Enqueue)
                {
                    /** if there is a multiple type job in queue,
                     * need change the active priotity table */
                    curActPriTable = 1u - NvM_Module.CurActPriTable;
                }
                else
                {
                    curActPriTable = NvM_Module.CurActPriTable;
                }
                /** set the priority table group flag */
                /* PRQA S 4399,4391 ++*/ /* VL_NvM_4399,VL_NvM_4391 */
                NvM_PriorityTable[curActPriTable][NVM_TABLE_SIZE_PRIORITY - 1U] |= (uint16)(0x01u << priHighBits);

                NvM_PriTable2QueueType* PriTable2QueuePtr = &(NvM_PriTable2Queue[curActPriTable][jobPri]);

                /* PRQA S 2844 ++ */ /* VL_NvM_2844 */
                if (0U != (NvM_PriorityTable[curActPriTable][priHighBits] & (uint16)(0x01u << priLowBits)))
                {
                    /** add the new job to the queue's tail */
                    NvM_StandQueue[PriTable2QueuePtr->TailIndex].NextIndex = addJobQueueIndex;
                }
                else
                {
                    /** set the priority table's corresponding bit */
                    NvM_PriorityTable[curActPriTable][priHighBits] |= (uint16)((uint16)0x01u << priLowBits);
                    PriTable2QueuePtr->HeadIndex = addJobQueueIndex;
                }
                /* PRQA S 4399,4391 --*/
                /* PRQA S 2844 -- */
                /** store the new job information to queue and set the block status as pending */
                PriTable2QueuePtr->TailIndex = addJobQueueIndex;
                /* PRQA S 2844 */ /* VL_NvM_2844 */
                /** The new job takes the last */
                NvM_StandQueue[addJobQueueIndex].BlockId    = BlockId;
                NvM_StandQueue[addJobQueueIndex].ServiceId  = ServiceId;
                NvM_StandQueue[addJobQueueIndex].NextIndex  = addJobQueueIndex;
                NvM_StandQueue[addJobQueueIndex].DestSrcPtr = DestSrcPtr;
                NVM_SETFLAGON(&AdminBlockPtr->FlagGroup, NVM_ADMIN_ENQUEUE);
                /* PRQA S 2844 */ /* VL_NvM_2844 */
                SchM_Exit_NvM_Queue();
                NvM_SingleJobResultFeedBack(BlockId, NVM_REQ_PENDING);
            }
        }
    }
    else
    {
        /** Invalid BlockId or Job has already enqueued */
        retVal = E_NOT_OK;
    }
    return retVal;
}

/**
 * @brief     Invoke the callback interface of the task result and record the corresponding flag and information
 */
NVM_LOCAL void NvM_JobEnqueueMultiJob(NvM_ServiceIdType ServiceId)
{
    NvM_MultiJobType* MultiJobPtr = &NvM_MultiJob;

    SchM_Enter_NvM_Queue();
    MultiJobPtr->Enqueue   = TRUE;
    MultiJobPtr->ServiceId = ServiceId;
    SchM_Exit_NvM_Queue();
    NvM_MultiJobResultFeedBack(MultiJobPtr->ServiceId, NVM_REQ_PENDING);
}
#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"
