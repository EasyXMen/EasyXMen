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
 **  @file               : Crypto_62.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "CryIf.h"
#include "CryIf_Cbk.h"

#include "Crypto_62_Internal.h"
/* PRQA S 1503,1532 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6010 EOF */     /* VL_MTR_Crypto_62_STCYC */
/* PRQA S 6030 EOF */     /* VL_MTR_Crypto_62_STMIF */
/* PRQA S 6080 EOF */     /* VL_MTR_Crypto_62_STPTH */

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/***************************************************************************************/
/**
 * @brief  CRYPTO driver init interface
 * @details Initializes the Crypto Driver
 *
 * @param[in]     configPtr, Pointer to a selected configuration structure.
 * @param[in,out] None
 * @param[out]    None
 * @return        None
 *
 */
/***************************************************************************************/
/* @req SRS_BSW_00101,SRS_BSW_00358,SRS_BSW_00414*/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void Crypto_62_Init(const Crypto_62_ConfigType* configPtr)
{
    Crypto_62_ArtiApiStart(2, CRYPTO_SID_INIT, configPtr);

    uint8 objectindex;
#if (CRYPTO_JOB_QUEUING == STD_ON)
    uint32 index;
#endif
    uint32 Keyindex;

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*PRQA S 3138 ++*/ /* VL_Crypto_62_DETReport */
    if (NULL_PTR != configPtr)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_INIT, CRYPTO_E_INIT_FAILED);
    } /*PRQA S 3138 --*/
    else
#else
    (void)configPtr;
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
    {
        /*@req SWS_Crypto_62_00215 @req [SWS_Crypto_62_00019] */ /* @req SWS_Crypto_62_00198 */
                                                                 /*init job*/
        Crypto_62_DriverStatus = CRYPTO_DRIVER_IDLE;
        (void)IStdLib_MemHeapInit((uint8*)Crypto_62_MemPool, CRYPTO_MEMPOOL_SIZE);

        for (objectindex = 0U; objectindex < CRYPTO_MAX_DRIVER_OBJECT; objectindex++)
        {
            Crypto_62_StoredJobBusy[objectindex] = CRYPTO_STOREDJOB_IDLE;
            Crypto_62_ClearStoredJob(objectindex);

#if (CRYPTO_JOB_QUEUING == STD_ON)
            /* PRQA S 1252 ++ */ /* VL_QAC_Crypto */
            for (index = 0U; index < CRYPTO_MAX_QUEUE_SIZE; index++)
            /* PRQA S 1252 -- */
            {
                /* Initializes the queue */
                Crypto_62_JobInQueue[objectindex][index].jobId    = 0u;
                Crypto_62_JobInQueue[objectindex][index].jobState = CRYPTO_JOBSTATE_IDLE;

                Crypto_62_JobInQueue[objectindex][index].jobPrimitiveInfo = NULL_PTR;

                Crypto_62_JobInQueue[objectindex][index].cryptoKeyId           = 0u;
                Crypto_62_JobInQueue[objectindex][index].jobRedirectionInfoRef = NULL_PTR;
                Crypto_62_JobInQueue[objectindex][index].targetCryptoKeyId     = 0u;
            }
            Crypto_62_QueueFilledSize[objectindex]  = 0U;
            Crypto_62_QueueSkipDequeue[objectindex] = FALSE;
#endif
        }
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
        /* PRQA S 1252 ++ */ /* VL_QAC_Crypto */
        for (Keyindex = 0U; Keyindex < CRYPTO_MAXKEY_CONFIGURED; Keyindex++)
        /* PRQA S 1252 -- */
        {
            /* Point to storage address */
            CryptoKeyStatus[Keyindex] = CRYPTO_KEYSTATUS_INVALID;
        }
#endif
    }

    Crypto_62_ArtiApiStop(1, CRYPTO_SID_INIT);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#if (CRYPTO_VERSION_INFO_API == STD_ON)
/* @req SWS_Crypto_910011,SRS_BSW_00407 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/***************************************************************************************/
/**
 * @brief CRYPTO driver get version interface
 * @details Returns the version information of this module.
 *
 * @param[in]     versioninfo  Pointer to a selected configuration structure
 * @param[in,out] None
 * @param[out]    None
 * @return        None
 */
/*****************************************************************************************/
void Crypto_62_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    Crypto_62_ArtiApiStart(2, CRYPTO_SID_GETVERSIONINFO, versioninfo);

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*PRQA S 3138 ++*/ /* VL_Crypto_62_DETReport */
    /*@req SWS_Crypto_00047*/
    if (NULL_PTR == versioninfo)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_GETVERSIONINFO, CRYPTO_E_PARAM_POINTER);
    } /*PRQA S 3138 --*/
    else
#endif
    {
        (versioninfo)->vendorID         = (uint16)CRYPTO_VENDOR_ID;
        (versioninfo)->moduleID         = (uint16)CRYPTO_MODULE_ID;
        (versioninfo)->sw_major_version = (uint8)CRYPTO_C_SW_MAJOR_VERSION;
        (versioninfo)->sw_minor_version = (uint8)CRYPTO_C_SW_MINOR_VERSION;
        (versioninfo)->sw_patch_version = (uint8)CRYPTO_C_SW_PATCH_VERSION;
    }

    Crypto_62_ArtiApiStop(1, CRYPTO_SID_GETVERSIONINFO);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/***************************************************************************************/
/**
 * @brief CRYPTO driver job process interface
 * @details Performs the crypto primitive,that is configured in the job parameter.
 *
 * @param[in]     objectId  Holds the identifier of the Crypto Driver Object.
 * @param[in,out] job    Pointer to the configuration of the job.
 * Contains structures with job and primitive relevant information but also pointer to result buffers.
 * @param[out]    None
 * @return          Std_ReturnType
 * @retval      E_OK: Request successful
 * @retval      E_NOT_OK: Request failed
 * @retval      CRYPTO_E_BUSY: Request failed,Crypro Driver Object is busy
 * @retval      CRYPTO_E_KEY_NOT_VALID: Request failed,the key is not valid
 * @retval      CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,a key element has the wrong size
 * @retval      CRYPTO_E_QUEUE_FULL: Request failed,the queue is full
 * @retval      CRYPTO_E_KEY_READ_FAIL: The service request failed,because key element extraction is not allowed
 * @retval      CRYPTO_E_KEY_WRITE_FAIL: The service request failed because the writing access failed
 * @retval      CRYPTO_E_KEY_NOT_AVAILABLE: The service request failed because the key is not available
 * @retval      CRYPTO_E_ENTROPY_EXHAUSTED: Request failed,the entropy is exhausted
 * @retval      CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 * @retval      CRYPTO_E_JOB_CANCELED: The service request failed because the synchronous Job has been canceled
 * @retval      CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */
/***************************************************************************************/
/* @req SWS_Crypto_91003 */
Std_ReturnType Crypto_62_ProcessJob(uint32 objectId, Crypto_JobType* job)
{
    Crypto_62_ArtiApiStart(3, CRYPTO_SID_PROCESSJOB, objectId, job);

    Std_ReturnType           status = E_NOT_OK;
    Crypto_OperationModeType operationMode;

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*PRQA S 3138 ++*/ /* VL_Crypto_62_DETReport */
    /*@req SWS_Crypto_00057*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00058*/
    else if (objectId >= CRYPTO_MAX_DRIVER_OBJECT)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00059*/
    else if (NULL_PTR == job)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00064,SWS_Crypto_00067*/
    else if (job->jobPrimitiveInfo->primitiveInfo->service >= CRYPTO_SERVICEMAX)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00202*/
    else if (
        (CRYPTO_KEYDERIVE == job->jobPrimitiveInfo->primitiveInfo->service)
        && (job->targetCryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00201*/
    else if (
        ((CRYPTO_RANDOMSEED <= job->jobPrimitiveInfo->primitiveInfo->service)
         && (CRYPTO_KEYSETINVALID >= job->jobPrimitiveInfo->primitiveInfo->service))
        && (job->cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_HANDLE);
    } /*PRQA S 3138 --*/
    else
#endif
    {
        if ((job->jobPrimitiveInfo->primitiveInfo->service <= CRYPTO_SIGNATUREVERIFY))
        {
            operationMode = job->jobPrimitiveInputOutput.mode;
        }
        else
        {
            operationMode = CRYPTO_OPERATIONMODE_SINGLECALL;
        }

        job->jobPrimitiveInputOutput.mode = operationMode;

        if (CRYPTO_PROCESSING_SYNC == job->jobPrimitiveInfo->processingType)
        {
#if (STD_ON == CRYPTO_JOB_QUEUING)
            boolean queueOccupied = (boolean)(Crypto_62_QueueFilledSize[objectId] > 0u);
            uint32  queueHeadPrio = Crypto_62_GetQueueHighestPriority(objectId);
#endif
            if ((CRYPTO_OBJECT_BUSY == Crypto_ObjectStatus[objectId]) && (job->jobState != CRYPTO_JOBSTATE_ACTIVE))
            {
                /* @req SWS_Crypto_00118 */ /* SWS_Crypto_00034 */
                status = (Std_ReturnType)CRYPTO_E_BUSY;
            }
#if (STD_ON == CRYPTO_JOB_QUEUING)
            else if ((queueOccupied == TRUE) && (job->jobState != CRYPTO_JOBSTATE_ACTIVE))
            {
                if (job->jobPriority > queueHeadPrio)
                {
                    /* @req SWS_Crypto_00199 */
                    status = Crypto_62_ProcessJob_Internal(objectId, job, operationMode);
                    if (E_OK == status)
                    {
                        Crypto_62_QueueSkipDequeue[objectId] = TRUE;
                    }
                }
                else
                {
                    /* Sync priority not higher than Crypto queue head */
                    status = (Std_ReturnType)CRYPTO_E_BUSY;
                }
            }
#endif
            else
            {
                status = Crypto_62_ProcessJob_Internal(objectId, job, operationMode);
            }
        }
        else
        {
            if (CRYPTO_OBJECT_BUSY == Crypto_ObjectStatus[objectId])
            {
                if ((CRYPTO_OPERATIONMODE_START == operationMode) || (CRYPTO_OPERATIONMODE_STREAMSTART == operationMode)
                    || (CRYPTO_OPERATIONMODE_SINGLECALL == operationMode))
                {
#if (STD_ON == CRYPTO_JOB_QUEUING)
                    /* PRQA S 1252 ++ */ /* VL_QAC_Crypto */
                    if (Crypto_62_QueueFilledSize[objectId] < CRYPTO_MAX_QUEUE_SIZE)
                    /* PRQA S 1252 -- */
                    {
                        status = Crypto_62_QueueInJob(objectId, job);
                    }
                    else
                    {
                        status = CRYPTO_E_BUSY;
                    }
#else
                    status = CRYPTO_E_BUSY;
#endif
                }
                else if (
                    ((CRYPTO_OPERATIONMODE_UPDATE == operationMode) || (CRYPTO_OPERATIONMODE_FINISH == operationMode))
                    && (job->jobState == CRYPTO_JOBSTATE_ACTIVE))
                {
                    status = Crypto_62_ProcessAsyncJob_NonQueue(objectId, job);
                }
                else
                {
                    status = CRYPTO_E_BUSY;
                }
            }
            else
            {
#if (STD_ON == CRYPTO_JOB_QUEUING)
                job->jobState = CRYPTO_JOBSTATE_ACTIVE;
                status        = Crypto_62_QueueInJob(objectId, job);
#else
                status = CRYPTO_E_BUSY;
#endif
            }
        }
    }

    Crypto_62_ArtiApiStop(2, CRYPTO_SID_PROCESSJOB, status);

    return status;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/***************************************************************************************/
/**
 * @brief CRYPTO driver job cancel interface
 * @details This interface removes the provided job from the queue and cancels the processing of the job if possible
 *
 * @param[in]  objectId   Holds the identifier of the Crypto Driver Object.
 * @param[in,out]  job    Pointer to the configuration of the job.
 * Contains structures with job and primitive relevant information but also pointer to result buffers.
 * @param[out]    None
 * @return          Std_ReturnType
 * @retval      E_OK: Request successful
 * @retval      E_NOT_OK: Request failed
 * @retval      CRYPTO_E_JOB_CANCELED: The job has been cancelled but is still processed.
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_CancelJob(uint32 objectId, Crypto_JobType* job)
{
    Crypto_62_ArtiApiStart(3, CRYPTO_SID_CANCELJOB, objectId, job);

    Std_ReturnType status = E_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*PRQA S 3138 ++*/ /* VL_Crypto_62_DETReport */
    /*@req SWS_Crypto_00123*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_CANCELJOB, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00124*/
    else if (objectId >= CRYPTO_MAX_DRIVER_OBJECT)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_CANCELJOB, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00125*/
    else if (NULL_PTR == job)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_CANCELJOB, CRYPTO_E_PARAM_POINTER);
    } /*PRQA S 3138 --*/
    else
#endif
    {
        /* @req [SWS_Crypto_00144]
         * If a job is cancelled,it shall return CRYPTO_E_JOB_CANCELED either with the callback,
         * when the job is an asynchronous job or as the return value of the function Crypto_CancelJob(),in case the
         * job is synchronous. */
        if (CRYPTO_PROCESSING_ASYNC == job->jobPrimitiveInfo->processingType)
        {
#if (STD_ON == CRYPTO_JOB_QUEUING)
            status = Crypto_62_CancelQueuedJob(objectId, job);
            if (E_OK == status)
            {
                status = (Std_ReturnType)CRYPTO_E_JOB_CANCELED;
                CryIf_CallbackNotification(job, (Std_ReturnType)CRYPTO_E_JOB_CANCELED);
            }
            else
            {
                if ((Crypto_62_StoredJobBusy[objectId] == CRYPTO_STOREDJOB_INUSE)
                    && (Crypto_62_StoredJob[objectId].jobId == job->jobId))
                {
                    if (0U < Crypto_62_QueueFilledSize[objectId]) /* PRQA S 1252  */ /* VL_QAC_Crypto */
                    {
                        Crypto_62_StoredJob[objectId]          = Crypto_62_JobInQueue[objectId][0];
                        Crypto_62_StoredJobBusy[objectId]      = CRYPTO_STOREDJOB_INUSE;
                        Crypto_62_StoredJob[objectId].jobState = CRYPTO_JOBSTATE_ACTIVE;
                        (void)Crypto_62_CancelQueuedJob(objectId, &Crypto_62_JobInQueue[objectId][0]);
                        Crypto_ObjectStatus[objectId] = CRYPTO_OBJECT_BUSY;
                    }
                    else
                    {
                        Crypto_ObjectStatus[objectId] = CRYPTO_OBJECT_IDLE;
                        Crypto_62_ClearStoredJob(objectId);
                    }
                    status = (Std_ReturnType)CRYPTO_E_JOB_CANCELED;
                    CryIf_CallbackNotification(job, (Std_ReturnType)CRYPTO_E_JOB_CANCELED);
                }
                else
                {
                    status = E_NOT_OK;
                }
            }
#else
            if ((Crypto_62_StoredJob[objectId].jobId == job->jobId)
                && (Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.mode == job->jobPrimitiveInputOutput.mode))
            {
                Crypto_62_StoredJob[objectId].jobId = CRYPTO_JOB_NOT_VALID;
                Crypto_ObjectStatus[objectId]       = CRYPTO_OBJECT_BUSY;
            }
#endif
        }
        else
        {
            /* @req SWS_Crypto_00214 / SWS_Crypto_00143 */
            if ((Crypto_62_StoredJobBusy[objectId] == CRYPTO_STOREDJOB_INUSE)
                && (Crypto_62_StoredJob[objectId].jobId == job->jobId))
            {
                Crypto_ObjectStatus[objectId] = CRYPTO_OBJECT_IDLE;
                Crypto_62_ClearStoredJob(objectId);
            }
            status = E_OK;
        }
    }

    Crypto_62_ArtiApiStop(2, CRYPTO_SID_CANCELJOB, status);

    return status;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/***************************************************************************************/
/**
* @brief    Crypto_62_MainFunction.
* @details  If asynchronous job processing is configured and there are job queues,the function
is called cyclically to process queued jobs.
*
* @param[in]       None
*
* @param[in,out]   None
* @param[out]      None
*
* @return          void
* @retval          None
*
* @api
*/
/***************************************************************************************/
void Crypto_62_MainFunction(void)
{
    Crypto_62_ArtiApiStart(1, CRYPTO_SID_MAINFUNCTION);

    Std_ReturnType           status = E_NOT_OK; /* PRQA S 2981 */ /* VL_QAC_Crypto */
    Crypto_OperationModeType operationMode;
    Crypto_JobType           tmpStoreJob;

    if (Crypto_62_DriverStatus != CRYPTO_DRIVER_UNINIT)
    {
#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
        /* PRQA S 1252 ++ */ /* VL_QAC_Crypto */
        for (uint32 cryptokeyId = 0u; cryptokeyId < CRYPTO_MAXKEY_CONFIGURED; cryptokeyId++)
        /* PRQA S 1252 -- */
        {
            if ((NULL_PTR != Crypto_62_Key[cryptokeyId].CryptoKeyNvBlockRef)
                && (CryptoKeyStatus[cryptokeyId] == CRYPTO_KEYSTATUS_UPDATE_IN_PROGRESS)
                && (Crypto_62_Key_RetryCount[Crypto_62_Key[cryptokeyId].CryptoNvBlock]
                    < Crypto_62_Key[cryptokeyId].CryptoKeyNvBlockRef->CryptoNvBlockFailedRetries))
            {
                /* PRQA S 0314 ++ */ /* VL_QAC_Crypto */
                status = Crypto_62_KeyWriteNvm(cryptokeyId);
                /* PRQA S 0314 -- */
                Crypto_62_Key_RetryCount[Crypto_62_Key[cryptokeyId].CryptoNvBlock] += 1u;
                if (E_OK == status)
                {
                    CryptoKeyStatus[cryptokeyId]                                       = CRYPTO_KEYSTATUS_VALID;
                    Crypto_62_Key_RetryCount[Crypto_62_Key[cryptokeyId].CryptoNvBlock] = 0u;
                }
            }
            if ((NULL_PTR != Crypto_62_Key[cryptokeyId].CryptoKeyNvBlockRef)
                && (Crypto_KeyReadfromNVM[cryptokeyId] == TRUE))
            {
                CryptoKeyStatus[cryptokeyId] = CRYPTO_KEYSTATUS_VALID;
            }
        }
#endif
        for (uint8 objectId = 0u; objectId < CRYPTO_MAX_DRIVER_OBJECT; objectId++)
        {
            tmpStoreJob = Crypto_62_StoredJob[objectId];
            if (tmpStoreJob.jobState == CRYPTO_JOBSTATE_ACTIVE)
            {
                if (tmpStoreJob.jobPrimitiveInfo != NULL_PTR)
                {
                    if (CRYPTO_PROCESSING_ASYNC == tmpStoreJob.jobPrimitiveInfo->processingType)
                    {
                        /* operationMode depends on the current StoredJob content */
                        operationMode = tmpStoreJob.jobPrimitiveInputOutput.mode;
                        /* PRQA S 2982 ++ */ /* VL_Crypto_62_2982 */
                        status = Crypto_62_ProcessJob_Internal(objectId, &Crypto_62_StoredJob[objectId], operationMode);
                        /* PRQA S 2982 -- */
                    }
                }
            }

#if (STD_ON == CRYPTO_JOB_QUEUING)
            /* @req SWS_Crypto_00199: skip dequeue for one MainFunction after high-prio sync */
            /* PRQA S 1252 ++ */ /* VL_QAC_Crypto */
            if (Crypto_62_QueueSkipDequeue[objectId] == TRUE)
            {
                Crypto_62_QueueSkipDequeue[objectId] = FALSE;
            }
            else
            {
                while ((Crypto_62_QueueFilledSize[objectId] > 0U)
                       && (Crypto_62_StoredJobBusy[objectId] == CRYPTO_STOREDJOB_IDLE)
                       && (Crypto_62_StoredJob[objectId].jobState == CRYPTO_JOBSTATE_IDLE))
                {
                    /* PRQA S 2982 ++ */ /* VL_QAC_Crypto */
                    status = Crypto_62_QueueOutJob(objectId, &Crypto_62_StoredJob[objectId]);
                    if (E_OK == status)
                    {
                        Crypto_62_StoredJobBusy[objectId]      = CRYPTO_STOREDJOB_INUSE;
                        Crypto_62_StoredJob[objectId].jobState = CRYPTO_JOBSTATE_ACTIVE;
                        Crypto_ObjectStatus[objectId]          = CRYPTO_OBJECT_BUSY;
                    }
                    /* PRQA S 2982 -- */
                    if ((Crypto_62_StoredJob[objectId].jobState == CRYPTO_JOBSTATE_ACTIVE)
                        && (CRYPTO_PROCESSING_ASYNC == Crypto_62_StoredJob[objectId].jobPrimitiveInfo->processingType))
                    {
                        /* QueueOutJob updated StoredJob, so operationMode must be refreshed */
                        operationMode = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.mode;
                        /* PRQA S 2982 ++ */ /* VL_Crypto_62_2982 */
                        status = Crypto_62_ProcessJob_Internal(objectId, &Crypto_62_StoredJob[objectId], operationMode);
                        /* PRQA S 2982 -- */
                    }
                }
            }
            /* PRQA S 1252 -- */
#endif
        }
    }

    Crypto_62_ArtiApiStop(1, CRYPTO_SID_MAINFUNCTION);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/* PRQA S 1503,1532 -- */
