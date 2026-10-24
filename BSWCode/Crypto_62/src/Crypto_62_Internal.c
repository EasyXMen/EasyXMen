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
 **  @file               : Crypto_62_Internal.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 1252,1290,3335,3673,4434,1533,1505,1532,1503 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040   ++ */         /* VL_QAC_Crypto */

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#include "Crypto_62_Hash.h"
#include "CryIf_Cbk.h"
#if (CRYPTO_SERVICE_MAC == STD_ON)
#include "Crypto_62_Mac.h"
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/**
 * @brief          Updates the cryptographic operation for a given object.
 * @param[in]      objectId : The ID of the cryptographic object to be updated.
 * @return         Std_ReturnType
 * @retval         E_OK: The underlying cryptographic operation has been successfully updated.
 * @retval         E_NOT_OK: The underlying cryptographic operation has failed, or an error has been detected.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71687
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
CRYPTO_62_LOCAL Std_ReturnType Crypto_Update(uint32 objectId);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ========================================== external data definition ============================================= */
/**
 * @brief Array to store the cryptographic jobs
 * @details This array is used to store cryptographic jobs that are being processed or waiting to be processed.
 * @range CRYPTO_MAX_DRIVER_OBJECT
 */
#define CRYPTO_62_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_62_MemMap.h"
Crypto_JobType Crypto_62_StoredJob[CRYPTO_MAX_DRIVER_OBJECT];
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_62_MemMap.h"

/**
 * @brief Memory pool for cryptographic operations
 * @details This memory pool is used to allocate memory for cryptographic operations dynamically.
 * @range CRYPTO_MEMPOOL_SIZE/4 elements (CRYPTO_MEMPOOL_SIZE bytes total)
 */
#define CRYPTO_62_START_SEC_VAR_CLEARED_32
#include "Crypto_62_MemMap.h"
uint32 Crypto_62_MemPool[CRYPTO_MEMPOOL_SIZE / CRYPTO_CONST_4];
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_32
#include "Crypto_62_MemMap.h"

#if (CRYPTO_JOB_QUEUING == STD_ON)
/**
 * @brief 2D array to store the queued cryptographic jobs
 * @details This 2D array is used to store cryptographic jobs that are queued for processing.
 *          Each row corresponds to a specific cryptographic object, and each column represents a job in the queue.
 * @range CRYPTO_MAX_DRIVER_OBJECT x CRYPTO_MAX_QUEUE_SIZE
 */
#define CRYPTO_62_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_62_MemMap.h"
Crypto_JobType Crypto_62_JobInQueue[CRYPTO_MAX_DRIVER_OBJECT][CRYPTO_MAX_QUEUE_SIZE];
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_62_MemMap.h"
#endif

#if (CRYPTO_MEMPOOL_SIZE % 4 != 0)
#error "ERROR: CRYPTO_MEMPOOL_SIZE must be divisible by 4 for 32-bit alignment."
#endif

/**
 * @brief Status of the cryptographic driver
 * @range 0-1
 */
#define CRYPTO_62_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_62_MemMap.h"
Crypto_62_DriverStatusType Crypto_62_DriverStatus; /* PRQA S 1531  */ /* VL_QAC_NoReferenced */
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_62_MemMap.h"

/**
 * @brief Array to store the status of cryptographic objects
 * @details This array is used to store the status of each cryptographic object.
 * @range CRYPTO_MAX_DRIVER_OBJECT
 */
#define CRYPTO_62_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_62_MemMap.h"
Crypto_62_ObjectStatusType Crypto_ObjectStatus[CRYPTO_MAX_DRIVER_OBJECT];
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_62_MemMap.h"

/**
 * @brief Array to store the status of stored jobs
 * @details This array is used to store the status of cryptographic jobs that are being processed or waiting to be
 * processed.
 * @range CRYPTO_MAX_DRIVER_OBJECT
 */
#define CRYPTO_62_START_SEC_VAR_CLEARED_8
#include "Crypto_62_MemMap.h"
uint8 Crypto_62_StoredJobBusy[CRYPTO_MAX_DRIVER_OBJECT];
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_8
#include "Crypto_62_MemMap.h"

#if (CRYPTO_JOB_QUEUING == STD_ON)
/**
 * @brief Array to store the filled size of each job queue
 * @details This array keeps track of the number of jobs currently in each queue.
 * @range CRYPTO_MAX_DRIVER_OBJECT
 */
#define CRYPTO_62_START_SEC_VAR_CLEARED_32
#include "Crypto_62_MemMap.h"
uint32 Crypto_62_QueueFilledSize[CRYPTO_MAX_DRIVER_OBJECT];
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_32
#include "Crypto_62_MemMap.h"

/**
 * @brief Skip dequeue once in MainFunction after high-priority sync (SWS_Crypto_00199)
 * @range CRYPTO_MAX_DRIVER_OBJECT
 */
#define CRYPTO_62_START_SEC_VAR_CLEARED_8
#include "Crypto_62_MemMap.h"
boolean Crypto_62_QueueSkipDequeue[CRYPTO_MAX_DRIVER_OBJECT]; /* PRQA S 1531 */ /* VL_QAC_NoReferenced */
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_8
#include "Crypto_62_MemMap.h"
#endif

/* ========================================== external function definitions ========================================= */
/******************************************************************************/
/*
 * @Brief               This function clears the stored job in the crypto.
 *
 *
 * @param[in]      objectId : The object ID of the crypto driver.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void Crypto_62_ClearStoredJob(uint32 objectId)
{
    Crypto_62_StoredJob[objectId].jobState              = CRYPTO_JOBSTATE_IDLE;
    Crypto_62_StoredJob[objectId].jobId                 = CRYPTO_JOB_NOT_VALID;
    Crypto_62_StoredJob[objectId].jobPrimitiveInfo      = NULL_PTR;
    Crypto_62_StoredJob[objectId].jobRedirectionInfoRef = NULL_PTR;
    Crypto_62_StoredJobBusy[objectId]                   = CRYPTO_STOREDJOB_IDLE;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#if (CRYPTO_JOB_QUEUING == STD_ON)
/******************************************************************************/
/*
 * @Brief               This function copy the job to the crypto queue.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void Crypto_62_SetQueueBuf(Crypto_JobType* destJob, Crypto_JobType* srcJob)
{
    destJob->jobId                   = srcJob->jobId;
    destJob->jobState                = srcJob->jobState;
    destJob->jobPrimitiveInputOutput = srcJob->jobPrimitiveInputOutput;
    destJob->jobPrimitiveInfo        = srcJob->jobPrimitiveInfo;
    destJob->jobPriority             = srcJob->jobPriority;
    destJob->cryptoKeyId             = srcJob->cryptoKeyId;
    destJob->jobRedirectionInfoRef   = srcJob->jobRedirectionInfoRef;
    destJob->targetCryptoKeyId       = srcJob->targetCryptoKeyId;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * @Brief               This function pops a job from the crypto queue.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_62_QueueOutJob(uint32 objectId, Crypto_JobType* job)
{
    uint32         offset;
    Std_ReturnType ret = E_NOT_OK;

    if (Crypto_62_QueueFilledSize[objectId] > 0U)
    {
        Crypto_62_QueueFilledSize[objectId]--;
        *job = Crypto_62_JobInQueue[objectId][0UL];
        for (offset = 0; offset < Crypto_62_QueueFilledSize[objectId]; offset++)
        {
            /* job prior to Crypto_62_JobInQueue[offset] - shift the item */
            Crypto_62_JobInQueue[objectId][offset] = Crypto_62_JobInQueue[objectId][offset + 1U];
        }
        ret = E_OK;
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * @Brief               Return the highest priority among queued jobs by reading
 *                      queue head (index 0). Head is highest because
 *                      Crypto_62_QueueInJob inserts in descending priority order;
 *                      this function does not scan the queue.
 * @req                 SWS_Crypto_00030
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
uint32 Crypto_62_GetQueueHighestPriority(uint32 objectId)
{
    uint32 highestPriority = 0u;

    /* SWS_Crypto_00058, SWS_Crypto_00030 */
    if ((objectId < CRYPTO_MAX_DRIVER_OBJECT) && (Crypto_62_QueueFilledSize[objectId] > 0u))
    {
        highestPriority = Crypto_62_JobInQueue[objectId][0u].jobPriority;
    }
    return highestPriority;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * @Brief               This function push a job to the crypto queue.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_62_QueueInJob(uint32 objectId, Crypto_JobType* job)
{
    uint32         offset;
    Std_ReturnType status = E_NOT_OK;
    Crypto_JobType Crypto_JobTempBuf;
    uint32         queueSize = Crypto_DriverObjects[objectId].CryptoQueueSize;
    Crypto_62_SetQueueBuf(&Crypto_JobTempBuf, job);
    if (0u == Crypto_62_QueueFilledSize[objectId])
    {
        /* JobQueue is empty */
        /* @req [SWS_Crypto_00033] */
        Crypto_62_JobInQueue[objectId][0]   = Crypto_JobTempBuf;
        Crypto_62_QueueFilledSize[objectId] = 1u;
        status                              = E_OK;
    }
    else if (queueSize == Crypto_62_QueueFilledSize[objectId])
    {
        /* JobQueue is full */
        /* @req SWS_Crypto_00032 */
        status = CRYPTO_E_BUSY;
    }
    else
    {
        /* JobQueue is not empty */
        Crypto_62_QueueFilledSize[objectId] += 1u;
        /* Rank the items by priority */
        /* @req [SWS_Crypto_00030] */
        for (offset = Crypto_62_QueueFilledSize[objectId] - 1u; offset > 0UL; offset--)
        {
            /* The higher the job priority value,the higher the job priority. */
            if (job->jobPriority > Crypto_62_JobInQueue[objectId][offset - 1U].jobPriority)
            {
                /* job prior to Crypto_62_JobInQueue[offset] - shift the item */
                Crypto_62_JobInQueue[objectId][offset]      = Crypto_62_JobInQueue[objectId][offset - 1U];
                Crypto_62_JobInQueue[objectId][offset - 1U] = Crypto_JobTempBuf;
            }
            else
            {
                /* shifted all lower priority elements now,we can insert the new request here. */
                /* @req [SWS_Crypto_00033] */
                Crypto_62_JobInQueue[objectId][offset] = Crypto_JobTempBuf;
                break;
            }
        }
        status = E_OK;
    }
    return status;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * @Brief               This function cancels a job from the queue.
 *                     state.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_62_CancelQueuedJob(uint32 objectId, Crypto_JobType* job)
{
    uint32         offset;
    uint32         shift;
    uint32         shiftNum;
    Std_ReturnType status    = E_NOT_OK;
    uint32         queueSize = Crypto_62_QueueFilledSize[objectId];

    for (offset = 0UL; offset < queueSize; offset++)
    {
        /* Don't touch the items in queue before finding the job to cancel */
        if (job->jobId == Crypto_62_JobInQueue[objectId][offset].jobId)
        {
            shiftNum = queueSize - offset - 1U;
            for (shift = offset; shift < shiftNum; shift++)
            {
                Crypto_62_JobInQueue[objectId][shift] = Crypto_62_JobInQueue[objectId][shift + 1U];
            }
            Crypto_62_QueueFilledSize[objectId] -= 1U;
            status = E_OK;
            if (0u == Crypto_62_QueueFilledSize[objectId])
            {
                Crypto_ObjectStatus[objectId] = CRYPTO_OBJECT_IDLE;
            }
            break;
        }
    }
    return status;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif
/******************************************************************************/
/*
 * @Brief               This function implements the Crypto driver.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_62_ProcessJob_Internal(uint32 objectId, Crypto_JobType* job, Crypto_OperationModeType operationMode)
{
    Std_ReturnType status = E_NOT_OK;
    if (job->jobPrimitiveInfo != NULL_PTR)
    {
        switch (operationMode)
        {
        case CRYPTO_OPERATIONMODE_START:
            status = Crypto_Start(objectId, job);
            break;
        case CRYPTO_OPERATIONMODE_UPDATE:
            if ((CRYPTO_OPERATIONMODE_FINISH != Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.mode)
                && (CRYPTO_JOBSTATE_ACTIVE == Crypto_62_StoredJob[objectId].jobState))
            {
                if ((Crypto_62_StoredJobBusy[objectId] == CRYPTO_STOREDJOB_INUSE)
                    && (Crypto_62_StoredJob[objectId].jobId == job->jobId))
                {
                    Crypto_62_StoredJob[objectId] = *job;
                    status                        = Crypto_Update(objectId);
                }
            }
            break;
        case CRYPTO_OPERATIONMODE_FINISH:
            if ((CRYPTO_OPERATIONMODE_FINISH != Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.mode)
                && (CRYPTO_JOBSTATE_ACTIVE == Crypto_62_StoredJob[objectId].jobState))
            {
                if ((Crypto_62_StoredJobBusy[objectId] == CRYPTO_STOREDJOB_INUSE)
                    && (Crypto_62_StoredJob[objectId].jobId == job->jobId))
                {
                    Crypto_62_StoredJob[objectId] = *job;
                    status                        = Crypto_Finish(objectId);
                }
            }
            break;
        case CRYPTO_OPERATIONMODE_STREAMSTART:
        case CRYPTO_OPERATIONMODE_SINGLECALL:
            status = Crypto_Start(objectId, job);
            if (status == E_OK)
            {
                status = Crypto_Update(objectId);
            }
            if (status == E_OK)
            {
                status = Crypto_Finish(objectId);
            }
            break;

        default:
            /*do nothing*/
            break;
        }
    }
    return status;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * @Brief               This function process the asynchronous job.without queue.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_62_ProcessAsyncJob_NonQueue(uint32 objectId, Crypto_JobType* job)
{
    Std_ReturnType status = E_OK;
    switch (job->jobPrimitiveInputOutput.mode)
    {
    case CRYPTO_OPERATIONMODE_UPDATE:
        if ((CRYPTO_OPERATIONMODE_START == Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.mode)
            && (CRYPTO_JOBSTATE_ACTIVE == Crypto_62_StoredJob[objectId].jobState))
        {
            if ((Crypto_62_StoredJobBusy[objectId] == CRYPTO_STOREDJOB_INUSE)
                && (Crypto_62_StoredJob[objectId].jobId == job->jobId))
            {
                Crypto_62_StoredJob[objectId] = *job;
            }
            else
            {
                status = E_NOT_OK;
            }
        }
        break;
    case CRYPTO_OPERATIONMODE_FINISH:
        if (((CRYPTO_OPERATIONMODE_UPDATE == Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.mode)
             || (CRYPTO_OPERATIONMODE_STREAMSTART == Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.mode))
            && (CRYPTO_JOBSTATE_ACTIVE == Crypto_62_StoredJob[objectId].jobState))
        {
            if ((Crypto_62_StoredJobBusy[objectId] == CRYPTO_STOREDJOB_INUSE)
                && (Crypto_62_StoredJob[objectId].jobId == job->jobId))
            {
                Crypto_62_StoredJob[objectId] = *job;
            }
            else
            {
                status = E_NOT_OK;
            }
        }
        else
        {
            status = E_NOT_OK;
        }
        break;
    default:
        /*do nothing*/
        break;
    }
    return status;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * @Brief               This function starts the Crypto driver job.
 *
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Start(uint32 objectId, Crypto_JobType* job)
{
    Std_ReturnType ret;
    if ((Crypto_62_StoredJobBusy[objectId] == CRYPTO_STOREDJOB_IDLE)
        || (Crypto_62_StoredJob[objectId].jobId == job->jobId))
    {
        Crypto_62_StoredJobBusy[objectId]      = CRYPTO_STOREDJOB_INUSE;
        Crypto_62_StoredJob[objectId]          = *job;
        Crypto_62_StoredJob[objectId].jobState = CRYPTO_JOBSTATE_ACTIVE;
        Crypto_ObjectStatus[objectId]          = CRYPTO_OBJECT_BUSY;
        ret                                    = Crypto_ProcessAlgorithm(objectId, CRYPTO_OPERATIONMODE_START);
    }
    else
    {
        ret = CRYPTO_E_BUSY;
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * @Brief               This function update the Crypto driver job.
 *
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
CRYPTO_62_LOCAL Std_ReturnType Crypto_Update(uint32 objectId)
{
    Std_ReturnType ret;
    ret = Crypto_ProcessAlgorithm(objectId, CRYPTO_OPERATIONMODE_UPDATE);
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * @Brief               This function finish the Crypto driver job.
 *
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Finish(uint32 objectId)
{
    Std_ReturnType ret;
    ret                           = Crypto_ProcessAlgorithm(objectId, CRYPTO_OPERATIONMODE_FINISH);
    Crypto_ObjectStatus[objectId] = CRYPTO_OBJECT_IDLE;
    if (CRYPTO_PROCESSING_ASYNC == Crypto_62_StoredJob[objectId].jobPrimitiveInfo->processingType)
    {
        CryIf_CallbackNotification(&Crypto_62_StoredJob[objectId], ret);
    }

    Crypto_62_ClearStoredJob(objectId);

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * @Brief               This function process the Crypto driver services.
 *
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_ProcessAlgorithm(uint32 objectId, Crypto_OperationModeType operateMode)
{
    Std_ReturnType result = E_NOT_OK;
    /* Service cases*/

    Crypto_AlgorithmFamilyType algorithmfamily =
        Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.family;
    const Crypto_ServiceInfoType service = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->service;
    Crypto_AlgorithmModeType     mode = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.mode;

    switch (service)
    {
    case CRYPTO_HASH:
#if (CRYPTO_SERVICE_HASH == STD_ON)
        result = Crypto_62_Hash_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_MACGENERATE:
#if (CRYPTO_SERVICE_MAC == STD_ON)
        result = Crypto_62_MacGenerate_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_MACVERIFY:
#if (CRYPTO_SERVICE_MAC == STD_ON)
        result = Crypto_62_MacVerify_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_ENCRYPT:
#if (CRYPTO_SERVICE_ENCRYPT == STD_ON)
        result = Crypto_62_Encrypt_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_DECRYPT:
#if (CRYPTO_SERVICE_ENCRYPT == STD_ON)
        result = Crypto_62_Decrypt_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_AEADENCRYPT:
#if (CRYPTO_SERVICE_AEADENCRYPT == STD_ON)
        result = Crypto_62_AeadEncrypt_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_AEADDECRYPT:
#if (CRYPTO_SERVICE_AEADENCRYPT == STD_ON)
        result = Crypto_62_AeadDecrypt_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_SIGNATUREGENERATE:
#if (CRYPTO_SERVICE_SIGNATURE == STD_ON)
        result = Crypto_62_SignatureGenerate_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_SIGNATUREVERIFY:
#if (CRYPTO_SERVICE_SIGNATURE == STD_ON)
        result = Crypto_62_SignatureVerify_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_RANDOMGENERATE:
#if (CRYPTO_SERVICE_RANDOM == STD_ON)
        result = Crypto_62_RandomGenerate_Process(objectId, algorithmfamily, mode, operateMode);
#endif
        break;
    case CRYPTO_RANDOMSEED:
#if (CRYPTO_SERVICE_RANDOM == STD_ON)
        result = Crypto_62_RandomSeed_Process(objectId, algorithmfamily, mode);
#endif
        break;
    case CRYPTO_KEYGENERATE:
        result = Crypto_62_KeyGenerate_Process(objectId, algorithmfamily, mode);
        break;
    case CRYPTO_KEYDERIVE:
        result = Crypto_62_KeyDerive_Process(objectId, algorithmfamily, mode);
        break;
    case CRYPTO_KEYSETVALID:
        result = Crypto_62_KeySetValid(Crypto_62_StoredJob[objectId].cryptoKeyId);
        break;
    case CRYPTO_KEYSETINVALID:
        result = Crypto_62_KeySetInValid(Crypto_62_StoredJob[objectId].cryptoKeyId);
        break;
    case CRYPTO_CUSTOM_SERVICE:
#if (CRYPTO_SERVICE_CUSTOM == STD_ON)
        result = Custom_Service_Process(objectId, algorithmfamily, mode);
#endif
        break;
    default:
        result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    }

    return result;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * @Brief               This function process the custom service.
 *
 */
/******************************************************************************/
#if (CRYPTO_SERVICE_CUSTOM == STD_ON)
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Custom_Service_Process(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily, Crypto_AlgorithmModeType mode)
{
    Std_ReturnType result = E_NOT_OK;
    (void)objectId;
    (void)mode;
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_CUSTOM:

        break;
    case CRYPTO_ALGOFAM_NOT_SET:
        break;
    default:
        /*do nothing*/
        break;
    }
    return result;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif
/******************************************************************************/
/*
 * @Brief               This function process the custom sync service.
 *
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_62_CustomSync(
    uint32       dispatchId,
    uint32       keyId,
    uint32       keyElementId,
    uint32       targetKeyId,
    uint32       targetKeyElementId,
    const uint8* inputPtr,
    uint32       inputLength,
    uint8*       outputPtr,
    uint32*      outputLengthPtr,
    uint8*       secondaryOutputPtr,
    uint32*      secondaryOutputLengthPtr)
{
    Crypto_62_ArtiApiStart(
        12,
        CRYPTO_SID_CustomSync,
        dispatchId,
        keyId,
        keyElementId,
        targetKeyId,
        targetKeyElementId,
        inputPtr,
        inputLength,
        outputPtr,
        outputLengthPtr,
        secondaryOutputPtr,
        secondaryOutputLengthPtr);

    Std_ReturnType status = E_NOT_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*PRQA S 3138 ++*/ /* VL_Crypto_62_DETReport */
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_CustomSync, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00076*/
    else if (targetKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_CustomSync, CRYPTO_E_PARAM_HANDLE);
    }
    /*SWS_Crypto_00257*/
    else if (keyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_CustomSync, CRYPTO_E_PARAM_HANDLE);
    } /*PRQA S 3138 --*/
    else
#endif
    {
        (void)dispatchId;
        (void)keyId;
        (void)keyElementId;
        (void)targetKeyId;
        (void)targetKeyElementId;
        (void)inputPtr;
        (void)inputLength;
        (void)outputPtr;
        (void)outputLengthPtr;
        (void)secondaryOutputPtr;
        (void)secondaryOutputLengthPtr;
    }

    Crypto_62_ArtiApiStop(2, CRYPTO_SID_CustomSync, status);

    return status;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#if ((CRYPTO_ALGORITHMFAM_AES == STD_ON) || (CRYPTO_ALGORITHMFAM_SM4 == STD_ON))
/******************************************************************************/
/*
 * Brief               Function of CTR mode counter raise.
 *                     default counter len : 8
 * Param-Name[in]      None.
 * Param-Name[in/out]  arr [in]counter to raise [out] raised counter
 * Param-Name[out]     None.
 * Return              void
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void CTR_Counter_Raise(uint8 arr[CRYPTO_CONST_8])
{
    /* PRQA S 2990,5209,1291,2144,1822 ++ */ /* VL_Crypto_62_2990 */ /* VL_Crypto_62_5209 */
    for (uint32 i = CRYPTO_CONST_0; i <= CRYPTO_CONST_7; i++)
    {
        if (arr[CRYPTO_CONST_7 - i] < CRYPTO_CONST_0xFF)
        {
            arr[CRYPTO_CONST_7 - i]++;
            break;
        }
        else
        {
            arr[CRYPTO_CONST_7 - i] = CRYPTO_CONST_0;
        }
    }
    /* PRQA S 2990,5209,1291,2144,1822 -- */
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

/* PRQA S 1252,1290,3335,3673,4434,1533,1505,1532,1503 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040   -- */
