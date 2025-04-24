/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*********************************************************************************
**                                                                            **
**  FILENAME    : Crypto.c                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuhao.ge                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for Crypto                                   **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform 4.4.0                       **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>    <AUTHOR>        <REVISION LOG>
 *  V1.0        20200701    yuhao.ge        Initial version
 *  V2.0        20210101    yuhao.ge        second version
 *  V2.1        20210601    qinchun.yang    Modify RSA algorithm,Add TLSF Library.
 *  V2.2        20210601    qinchun.yang    Modify asynchronous task processing logic.
 *  V2.1.0      2023-07-17   jie.gu         CP2.1 Release Version
 *  V2.1.1      2024-06-24   jie.gu         Replace tlsf library with ilib .
 *  V2.1.2      2024-11-29  shaoqiang.Liang Modify the interface call of ILib  .
 */

/* PRQA S 3432 ++ */                /* MISRA Rule 20.7 */
/* PRQA S 3332 ++ */                /* MISRA Rule 20.9 */
/* PRQA S 1051 ++ */                /* MISRA Rule 18.8 */
/* PRQA S 3200 ++ */                /* MISRA Rule 17.7 */
/* PRQA S 2016,2002 ++ */           /* MISRA Rule 16.4 */
/* PRQA S 3397 ++ */                /* MISRA Rule 12.1 */
/* PRQA S 1881,1842 ++ */           /* MISRA Rule 10.4 */
/* PRQA S 4461,4464,0570,4424 ++ */ /* MISRA Rule 10.3 */
/* PRQA S 0686 ++ */                /* MISRA Rule 9.3 */
/* PRQA S 1532,1505 ++ */           /* MISRA Rule 8.7 */
/* PRQA S 3206 ++ */                /* MISRA Rule 2.7 */
/* PRQA S 1514 ++ */                /* MISRA Rule 8.9 */
/* PRQA S 3451,3449 ++ */           /* MISRA Rule 8.5 */
/* PRQA S 3408 ++ */                /* MISRA Rule 8.4 */
/* PRQA S 1335,3001,3007 ++ */      /* MISRA Rule 8.2 */
/* PRQA S 1503 ++ */                /* MISRA Rule 2.1 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CryIf.h"
#include "CryIf_Cbk.h"
#include "Crypto.h"
#include "Crypto_Types.h"
#include "Std_Types.h"
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#include "aes.h"
#include "sha256.h"
#include "cmac.h"
#include "ctr_drbg.h"
#include "rsa.h"
#include "dh.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"
#if (CRYPTO_JOB_QUEUING == STD_ON)
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_QueueJob(P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_CancelQueuedJob(P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job);
#endif
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_ProcessSYNCJob(P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_ProcessASYNCJob_NONQUEUE(P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job);
FUNC(void, CRYPTO_CODE) Crypto_Start(P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_Update(void);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_Finish(void);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_RandomSeedInternal(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) seedPtr,
    VAR(uint32, AUTOMATIC) seedLength);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_KeySetValid_internal(VAR(uint32, AUTOMATIC) cryptoKeyId);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyExchangeCalcPubVal_internal(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) publicValueLengthPtr);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyExchangeCalcSecret_internal(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, AUTOMATIC) partnerPublicValueLength);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_sha256_Process();
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_GernerateMAC_Process();
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_VerifyMAC_Process();
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AESDecryptProcess();
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AESEncryptProcess();
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_CtrDrbgProcess();
FUNC(Std_ReturnType, CRYPTO_CODE) rsa_encryptProcess();
FUNC(Std_ReturnType, CRYPTO_CODE) rsa_decryptProcess();
FUNC(Std_ReturnType, CRYPTO_CODE) rsa_signProcess();
FUNC(Std_ReturnType, CRYPTO_CODE) rsa_verifyProcess();
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AlgorithmGetInput(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) buf);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AlgorithmGetSecondInput(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) buf);
FUNC(void, CRYPTO_CODE)
Crypto_AlgorithmOutput(P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) buf, VAR(uint32, AUTOMATIC) outputLength);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_ProcessAlgorithm(void);

#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"
static VAR(Crypto_JobType, AUTOMATIC) Crypto_StoredJob;
#if (CRYPTO_JOB_QUEUING == STD_ON)
static P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) Crypto_JobInQueue[CRYPTO_MAX_QUEUE_SIZE];

Crypto_JobType Crypto_JobTempBuf; /* PRQA S 1514  */ /* MISRA Rule 8.9 */
#endif
#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

#define CRYPTO_START_SEC_VAR_CLEARED_BOOLEAN
#include "Crypto_MemMap.h"
static VAR(boolean, Crypto_VAR_CLEARED_BOOLEAN) ASYNFlag = FALSE;
#define CRYPTO_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Crypto_MemMap.h"
#define CRYPTO_START_SEC_VAR_CLEARED_8
#include "Crypto_MemMap.h"
static VAR(Crypto_DriverStatusType, CRYPTO_VAR) Crypto_DriverStatus = CRYPTO_DRIVER_UNINIT;
VAR(Crypto_KeyStateType, CRYPTO_VAR) CryptoKeyStatus[CRYPTO_MAXKEY_CONFIGURED];
#define CRYPTO_STOP_SEC_VAR_CLEARED_8
#include "Crypto_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"
extern CONST(Crypto_KeyCfgType, CRYPTO_CONST) Crypto_Key[CRYPTO_MAXKEY_CONFIGURED];
#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"
static FUNC(void, CRYPTO_CODE) Crypto_ClearStoredJob(void)
{
    Crypto_StoredJob.jobId = CRYPTO_JOB_NOT_VALID;
    Crypto_StoredJob.jobInfo = NULL_PTR;
    Crypto_StoredJob.jobPrimitiveInfo = NULL_PTR;
    Crypto_StoredJob.jobRedirectionInfoRef = NULL_PTR;
}
/**
 * Initializes the Crypto Driver
 * Service ID: 0x00
 * Sync/Async: 0x00
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *    @configPtr, Pointer to a selected configuration structure.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    void
 */

/* @req SRS_BSW_00101,SRS_BSW_00358,SRS_BSW_00414*/

FUNC(void, CRYPTO_CODE) Crypto_Init(P2CONST(Crypto_ConfigType, AUTOMATIC, CRYPTO_APPL_DATA) configPtr)
{
    uint32 index;
    uint32 Keyindex;

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR != configPtr)
    {
        Crypto_ReportDetErr(CRYPTO_SID_INIT, CRYPTO_E_INIT_FAILED);
    }
    else
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
    {
#if (CRYPTO_DEV_ERROR_DETECT == STD_OFF)
        (void)configPtr;
#endif
        /*@req SWS_Crypto_00215 @req [SWS_Crypto_00019] */ /* @req SWS_Crypto_00198 */
        /*init job*/
        Crypto_ClearStoredJob();

#if (CRYPTO_JOB_QUEUING == STD_ON)
        for (index = 0; index < CRYPTO_MAX_QUEUE_SIZE; index++)
        {
            /* Initializes the queue */
            Crypto_JobInQueue[index] = NULL_PTR;
        }
#endif
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
        for (Keyindex = 0; Keyindex < CRYPTO_MAXKEY_CONFIGURED; Keyindex++)
        {
            /* Point to storage address */
            CryptoKeyStatus[Keyindex] = CRYPTO_KEYSTATE_INVALID;
        }
#endif
        Crypto_DriverStatus = CRYPTO_DRIVER_IDLE;
    }
}

/**
 * Returns the version information of this module.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @versioninfo,Pointer to a selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */

#if (CRYPTO_VERSION_INFO_API == STD_ON)
/* @req SWS_Crypto_910011,SRS_BSW_00407 */
FUNC(void, CRYPTO_CODE) Crypto_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_APPL_DATA) versioninfo)
{
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00047*/
    if (NULL_PTR == versioninfo)
    {
        Crypto_ReportDetErr(CRYPTO_SID_GETVERSIONINFO, CRYPTO_E_PARAM_POINTER);
    }
    else
#endif
    {
        (versioninfo)->vendorID = (uint16)CRYPTO_VENDOR_ID;
        (versioninfo)->moduleID = (uint16)CRYPTO_MODULE_ID;
        (versioninfo)->sw_major_version = (uint8)CRYPTO_SW_MAJOR_VERSION;
        (versioninfo)->sw_minor_version = (uint8)CRYPTO_SW_MINOR_VERSION;
        (versioninfo)->sw_patch_version = (uint8)CRYPTO_SW_PATCH_VERSION;
    }
}
#endif

/**
 * Performs the crypto primitive,that is configured in the job parameter.
 * Service ID: 0x03
 * Sync/Async: Sync or Async,depends on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN): @objectId,Holds the identifier of the Crypto Driver Object.
 * Parameters(INOUT): @job,Pointer to the configuration of the job.
 * Contains structures with job and primitive relevant information but also pointer to result buffers.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType
 *      E_OK: Request successful
 *      E_NOT_OK: Request failed
 *      CRYPTO_E_BUSY: Request failed,Crypro Driver Object is busy
 *      CRYPTO_E_KEY_NOT_VALID: Request failed,the key is not valid
 *      CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,a key element has the wrong size
 *      CRYPTO_E_QUEUE_FULL: Request failed,the queue is full
 *      CRYPTO_E_KEY_READ_FAIL: The service request failed,because key element extraction is not allowed
 *      CRYPTO_E_KEY_WRITE_FAIL: The service request failed because the writing access failed
 *      CRYPTO_E_KEY_NOT_AVAILABLE: The service request failed because the key is not available
 *      CRYPTO_E_ENTROPY_EXHAUSTION: Request failed,the entropy is exhausted
 *      CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *      CRYPTO_E_JOB_CANCELED: The service request failed because the synchronous Job has been canceled
 *      CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

/* @req SWS_Crypto_91003 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_ProcessJob(VAR(uint32, AUTOMATIC) objectId, P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job)
{
    Std_ReturnType Status = E_OK;
#if (CRYPTO_RSA_TYPE == 1)
    uint16 rsaLen = 128u;
#elif (CRYPTO_RSA_TYPE == 2)
    uint16 rsaLen = 256u;
#else
    uint16 rsaLen = 384u;
#endif
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00057*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00058*/
    else if (objectId >= CRYPTO_MAX_DRIVER_OBJECT)
    {
        Crypto_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00059*/
    else if (NULL_PTR == job)
    {
        Crypto_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00064,SWS_Crypto_00067*/
    else if (job->jobPrimitiveInfo->primitiveInfo->service > CRYPTO_KEYSETVALID)
    {
        Crypto_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00201*/
    else if (
        ((CRYPTO_KEYSETVALID == job->jobPrimitiveInfo->primitiveInfo->service)
         || (CRYPTO_RANDOMSEED == job->jobPrimitiveInfo->primitiveInfo->service)
         || (CRYPTO_KEYGENERATE == job->jobPrimitiveInfo->primitiveInfo->service)
         || (CRYPTO_KEYDERIVE == job->jobPrimitiveInfo->primitiveInfo->service)
         || (CRYPTO_KEYEXCHANGECALCPUBVAL == job->jobPrimitiveInfo->primitiveInfo->service)
         || (CRYPTO_KEYEXCHANGECALCSECRET == job->jobPrimitiveInfo->primitiveInfo->service))
        && (job->cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_ReportDetErr(CRYPTO_SID_PROCESSJOB, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        if (((CRYPTO_ENCRYPT == job->jobPrimitiveInfo->primitiveInfo->service)
             || (CRYPTO_SIGNATUREGENERATE == job->jobPrimitiveInfo->primitiveInfo->service))
            && (CRYPTO_ALGOFAM_RSA == job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
            && (rsaLen != *(job->jobPrimitiveInputOutput.outputLengthPtr)))
        {
            return E_NOT_OK;
        }
        /*@req SWS_Crypto_00072*/
        /* PRQA S 2995,2991 ++ */ /* MISRA Rule 2.2 Rule 14.3*/
        if ((CRYPTO_HASH != job->jobPrimitiveInfo->primitiveInfo->service)
            || (CRYPTO_RANDOMGENERATE != job->jobPrimitiveInfo->primitiveInfo->service)
            || (job->cryptoKeyId < CRYPTO_MAXKEY_CONFIGURED))
        /* PRQA S 2995,2991 -- */ /* MISRA Rule 2.2 Rule 14.3*/
        {
            if (CRYPTO_PROCESSING_SYNC == job->jobPrimitiveInfo->processingType)
            {
                if (Crypto_StoredJob.jobId == job->jobId)
                {
                    /* The job request is a new request about job currently being processed. */
                    if ((CRYPTO_OPERATIONMODE_START == job->jobPrimitiveInputOutput.mode)
                        || (CRYPTO_OPERATIONMODE_STREAMSTART == job->jobPrimitiveInputOutput.mode)
                        || (CRYPTO_OPERATIONMODE_SINGLECALL == job->jobPrimitiveInputOutput.mode)
                        || (CRYPTO_OPERATIONMODE_UPDATE == job->jobPrimitiveInputOutput.mode)
                        || (CRYPTO_OPERATIONMODE_FINISH == job->jobPrimitiveInputOutput.mode))
                    {
                        Status = E_OK;
                    }
                }
                else
                {
                    if (CRYPTO_DRIVER_BUSY == Crypto_DriverStatus)
                    {
                        /* @req SWS_Crypto_00118 */
                        Status = (Std_ReturnType)CRYPTO_E_BUSY;
                    }
                    else
                    {
                        if ((CRYPTO_OPERATIONMODE_START == job->jobPrimitiveInputOutput.mode)
                            || (CRYPTO_OPERATIONMODE_STREAMSTART == job->jobPrimitiveInputOutput.mode)
                            || (CRYPTO_OPERATIONMODE_SINGLECALL == job->jobPrimitiveInputOutput.mode))
                        {
                            Status = E_OK;
                        }
                        else
                        {
                            Status = E_NOT_OK;
                        }
                    }
                }
                if (E_OK == Status)
                {
                    Status = Crypto_ProcessSYNCJob(job);
                }
            }
            else
            {
                if (Crypto_StoredJob.jobId == job->jobId)
                {

                    if ((CRYPTO_OPERATIONMODE_START == job->jobPrimitiveInputOutput.mode)
                        || (CRYPTO_OPERATIONMODE_STREAMSTART == job->jobPrimitiveInputOutput.mode)
                        || (CRYPTO_OPERATIONMODE_SINGLECALL == job->jobPrimitiveInputOutput.mode))
                    {
#if (STD_ON == CRYPTO_JOB_QUEUING)
                        Crypto_QueueJob(job);
#else
                        Status = E_NOT_OK;
#endif
                    }
                    else if (
                        (CRYPTO_OPERATIONMODE_UPDATE == job->jobPrimitiveInputOutput.mode)
                        || (CRYPTO_OPERATIONMODE_FINISH == job->jobPrimitiveInputOutput.mode))
                    {
                        Status = Crypto_ProcessASYNCJob_NONQUEUE(job);
                    }
                    else
                    {
                        Status = E_NOT_OK;
                    }
                }
                else
                {
                    Status = Crypto_ProcessASYNCJob_NONQUEUE(job);
                }
            }
        }
    }
    return Status;
}

/**
 * This interface cancels the processing of the job if possible.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant,but not for same Crypto Driver Object
 * Parameters(IN): @objectId,Holds the identifier of the Crypto Driver Object.
 * Parameters(INOUT): @job,Pointer to the configuration of the job.
 * Contains structures with job and primitive relevant information but also pointer to result buffers.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType
 *      E_OK: Request successful
 *      E_NOT_OK: Request failed
 *      CRYPTO_E_JOB_CANCELED: The job has been cancelled but is still processed.
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_CancelJob(VAR(uint32, AUTOMATIC) objectId, P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job)
{
    Std_ReturnType Status = E_OK; /* PRQA S 2981  */ /* MISRA Rule 2.2 */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00123*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_CANCELJOB, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00124*/
    else if (objectId >= CRYPTO_MAX_DRIVER_OBJECT)
    {
        Crypto_ReportDetErr(CRYPTO_SID_CANCELJOB, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00125*/
    else if (NULL_PTR == job)
    {
        Crypto_ReportDetErr(CRYPTO_SID_CANCELJOB, CRYPTO_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* @req [SWS_Crypto_00144]
         * If a job is cancelled,it shall return CRYPTO_E_JOB_CANCELED either with the callback,
         * when the job is an asynchronous job or as the return value of the function Crypto_CancelJob(),in case the job
         * is synchronous. */
        if (CRYPTO_PROCESSING_ASYNC == job->jobPrimitiveInfo->processingType)
        {
#if (STD_ON == CRYPTO_JOB_QUEUING)
            Status = Crypto_CancelQueuedJob(job);
            if (E_OK == Status)
            {
                Status = (Std_ReturnType)CRYPTO_E_JOB_CANCELED;
                CryIf_CallbackNotification(job, (Std_ReturnType)CRYPTO_E_JOB_CANCELED);
            }
            else
            {
                if (&Crypto_StoredJob == job)
                {
                    if (NULL_PTR != Crypto_JobInQueue[0])
                    {
                        Crypto_StoredJob = *(Crypto_JobInQueue[0]);
                        Crypto_CancelQueuedJob(Crypto_JobInQueue[0]);
                        Crypto_DriverStatus = CRYPTO_DRIVER_BUSY;
                        ASYNFlag = TRUE;
                    }
                    else
                    {
                        Crypto_DriverStatus = CRYPTO_DRIVER_IDLE;
                        ASYNFlag = TRUE;
                    }
                    Status = (Std_ReturnType)CRYPTO_E_JOB_CANCELED;
                    CryIf_CallbackNotification(job, (Std_ReturnType)CRYPTO_E_JOB_CANCELED);
                }
                else
                {
                    Status = E_NOT_OK;
                }
            }
#else
            if ((Crypto_StoredJob.jobId == job->jobId)
                && (Crypto_StoredJob.jobPrimitiveInputOutput.mode == job->jobPrimitiveInputOutput.mode))
            {
                ASYNFlag = FALSE;
                Crypto_StoredJob.jobId = 0xff;
                Crypto_DriverStatus = CRYPTO_DRIVER_IDLE;
            }
#endif
        }
        else
        {
            Status = (Std_ReturnType)E_OK;
            Crypto_DriverStatus = CRYPTO_DRIVER_IDLE;
        }
    }
    return Status;
}

/**
 * Sets the given key element bytes to the key identified by keyId.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *    @keyId,Holds the identifier of the key for which a new material shall be set.
 *    @keyElementId,Holds the identifier of the key element to be written.
 *    @keyPtr,Holds the pointer to the key element bytes to be processed.
 *    @keyLength,Contains the number of key element bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed,service is still busy
 *    CRYPTO_E_KEY_WRITE_FAIL:Request failed because write access was denied
 *    CRYPTO_E_KEY_NOT_AVAILABLE: Request failed because the key is not available.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element size does not match size of provided data.
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementSet(
    uint32 cryptokeyId,
    uint32 keyElementId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) keyPtr,
    uint32 keyLength)
{

    Std_ReturnType Status = E_NOT_OK; /* PRQA S 2981  */ /* MISRA Rule 2.2 */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00075*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00078*/
    else if (NULL_PTR == keyPtr)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00079,SWS_Crypto_00146*/
    else if (0u == keyLength)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        Status = Crypto_KeyElementSetInternal(cryptokeyId, keyElementId, keyPtr, keyLength);
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key validation interface.
 * @details  This interface sets the key state of the key identified by cryptoKeyId to valid.
 *
 * @param[in]       cryptoKeyId   Holds the identifier of the key which shall be set to valid.
 * @param[in,out]   None.
 * @param[out]      None.
 ** @return          Std_ReturnType
 * @retval          E_OK:          Request successful
 * @retval          E_NOT_OK:      Request Failed
 * @retval          CRYPTO_E_BUSY: Request Failed,Crypto Driver Object is Busy
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_KeySetValid(VAR(uint32, AUTOMATIC) cryptoKeyId)
{
    Std_ReturnType Status = E_NOT_OK; /* PRQA S 2981  */ /* MISRA Rule 2.2 */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00196*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYSETVALID, CRYPTO_E_UNINIT);
    }
    else
#endif
    {
        Status = Crypto_KeySetValid_internal(cryptoKeyId);
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key element extraction interface.
 * @details  This interface shall be used to get a key element of the key identified by the cryptoKeyId
 *           and store the key element in the memory location pointed by the result pointer.
 *
 * @param[in]       cryptoKeyId     Holds the identifier of the key whose key element shall be returned.
 * @param[in]       keyElementId    Holds the identifier of the key element which shall be returned.
 * @param[in,out]   resultLengthPtr Holds a pointer to a memory location in which the length information is stored.
 *                                  On calling this function this parameter shall contain the size of the buffer
 * provided by resultPtr. If the key element is configured to allow partial access,this parameter contains the amount of
 * data which should be read from the key element. The size may not be equal to the size of the provided buffer anymore.
 *                                  When the request has finished,the amount of data that has been stored shall be
 * stored.
 * @param[out]      resultPtr       Holds the pointer of the buffer for the returned key element
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed because read access was denied
 * @retval          E_NOT_OK:      The provided buffer is too small to store the result
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementGet(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    VAR(uint32, AUTOMATIC) keyElementId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) resultLengthPtr)
{

    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981  */ /* MISRA Rule 2.2 */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00085*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00089*/
    else if ((NULL_PTR == resultPtr) || (NULL_PTR == resultLengthPtr))
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00090*/
    else if (0u == *resultLengthPtr)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        /* SWS_Crypto_00087: KeyElement not found */
        uint32 KeyElementIndex = 0;
        uint32 Index;
        /*Find target Key Element index*/
        Status = Crypto_KeyElementIndexFind(cryptoKeyId, keyElementId, &KeyElementIndex);
        if (E_OK == Status)
        {
            if (*resultLengthPtr
                < Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex].CryptoKeyElementSize)
            {
                /* SWS_Crypto_00195: resultPtr too small */
                Status = (Std_ReturnType)E_NOT_OK;
            }
            /* SWS_Crypto_00139: Check access rights */
            else if (
                Crypto_Key[cryptoKeyId]
                    .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                    .CryptoKeyElementReadAccess
                == CRYPTO_RA_DENIED)
            {
                Status = (Std_ReturnType)CRYPTO_E_KEY_READ_FAIL;
            }
            else
            {
                for (Index = 0; Index < *resultLengthPtr; Index++)
                {
                    resultPtr[Index] = Crypto_Key[cryptoKeyId]
                                           .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                                           .CryptoKeyElementInitValue[Index];
                }
                *resultLengthPtr =
                    Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex].CryptoKeyElementSize;
            }
        }
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key element copy interface.
 * @details  This interface copies a key element to another key element in the same crypto driver.
 *
 * @param[in]       cryptoKeyId        Holds the identifier of the key whose key element shall be the source element.
 * @param[in]       keyElementId       Holds the identifier of the key element which shall be the source for the copy
 * operation.
 * @param[in]       targetCryptoKeyId  Holds the identifier of the key whose key element shall be the destination
 * element.
 * @param[in]       targetKeyElementId Holds the identifier of the key element which shall be the destination for the
 * copy operation.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementCopy(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    VAR(uint32, AUTOMATIC) keyElementId,
    VAR(uint32, AUTOMATIC) targetCryptoKeyId,
    VAR(uint32, AUTOMATIC) targetKeyElementId)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981  */ /* MISRA Rule 2.2 */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00149*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPY, CRYPTO_E_UNINIT);
    }
    else
#endif
    {
        /*set global VAR as null*/
        uint32 KeyElementIndex = 0;
        uint32 KeyElementIndexS = 0;
        uint8 i;

        /*Find target / Source Key Element Buffer Info*/
        Status = Crypto_KeyElementIndexFind(targetCryptoKeyId, targetKeyElementId, &KeyElementIndex);
        if (E_OK == Status)
        {
            Status = Crypto_KeyElementIndexFind(cryptoKeyId, keyElementId, &KeyElementIndexS);
            if (E_OK == Status)
            {
                /*Check Read and Write Access before started*/
                if (Crypto_Key[cryptoKeyId]
                        .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                        .CryptoKeyElementReadAccess
                    == CRYPTO_RA_DENIED)
                {
                    Status = CRYPTO_E_KEY_READ_FAIL;
                }
                else if (
                    Crypto_Key[cryptoKeyId]
                        .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                        .CryptoKeyElementWriteAccess
                    == CRYPTO_WA_DENIED)
                {
                    Status = CRYPTO_E_KEY_WRITE_FAIL;
                }
                else if (
                    Crypto_Key[cryptoKeyId]
                        .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                        .CryptoKeyElementFormat
                    != Crypto_Key[cryptoKeyId]
                           .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                           .CryptoKeyElementFormat)
                {
                    Status = CRYPTO_E_KEY_SIZE_MISMATCH;
                }
                else
                {
                    for (i = 0; i < Crypto_Key[cryptoKeyId]
                                        .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                                        .CryptoKeyElementSize;
                         i++)
                    {
                        Crypto_Key[cryptoKeyId]
                            .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                            .CryptoKeyElementInitValue[i] = Crypto_Key[targetCryptoKeyId]
                                                                .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                                                                .CryptoKeyElementInitValue[i];
                    }
                    *(Crypto_Key[cryptoKeyId]
                          .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                          .CryptoKeyElementLength) = *(Crypto_Key[cryptoKeyId]
                                                           .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                                                           .CryptoKeyElementLength);
                }
            }
        }
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key element copy interface.
 * @details  Copies a key element to another key element in the same crypto driver.
 *           The keyElementSourceOffset and keyElementCopyLength allows to copy just a part of the source key element
 * into the destination The offset of the target key is also specified with this function. If the actual key element is
 * directly mapped to flash memory, there could be a bigger delay when calling this function(synchronous operation).
 *
 * @param[in]       cryptoKeyId                   Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       keyElementId                  Holds the identifier of the key element which shall be the source for
 * the copy operation.
 * @param[in]       keyElementSourceOffset        This is the offset of the of the source key element indicating the
 * start index of the copy operation.
 * @param[in]       keyElementTargetOffset        This is the offset of the of the target key element indicating the
 * start index of the copy operation.
 * @param[in]       keyElementCopyLength          Specifies the number of bytes that shall be copied.
 * @param[in]       targetCryptoKeyId             Holds the identifier of the key whose key element shall be the
 * destination element.
 * @param[in]       targetKeyElementId            Holds the identifier of the key element which shall be the destination
 * for the copy operation.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementCopyPartial(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    VAR(uint32, AUTOMATIC) keyElementId,
    VAR(uint32, AUTOMATIC) keyElementSourceOffset,
    VAR(uint32, AUTOMATIC) keyElementTargetOffset,
    VAR(uint32, AUTOMATIC) keyElementCopyLength,
    VAR(uint32, AUTOMATIC) targetCryptoKeyId,
    VAR(uint32, AUTOMATIC) targetKeyElementId)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981  */ /* MISRA Rule 2.2 */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00205*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPYPARTIAL, CRYPTO_E_UNINIT);
    }
    else
#endif
    {
        uint32 KeyElementIndex = 0;
        uint32 KeyElementIndexS = 0;
        uint8 i;
        /*Find target / Source Key Element Buffer Info*/
        Status = Crypto_KeyElementIndexFind(targetCryptoKeyId, targetKeyElementId, &KeyElementIndex);
        if (E_OK == Status)
        {
            Status = Crypto_KeyElementIndexFind(cryptoKeyId, keyElementId, &KeyElementIndexS);
            if (E_OK == Status)
            {
                /*Check Read and Write Access before started*/
                if (Crypto_Key[cryptoKeyId]
                        .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                        .CryptoKeyElementReadAccess
                    == CRYPTO_RA_DENIED)
                {
                    Status = E_NOT_OK;
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
                    Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPYPARTIAL, CRYPTO_E_KEY_READ_FAIL);
#endif
                }
                else if (
                    Crypto_Key[cryptoKeyId]
                        .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                        .CryptoKeyElementWriteAccess
                    == CRYPTO_WA_DENIED)
                {
                    Status = E_NOT_OK;
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
                    Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPYPARTIAL, CRYPTO_E_KEY_WRITE_FAIL);
#endif
                }
                else if (
                    Crypto_Key[cryptoKeyId]
                        .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                        .CryptoKeyElementFormat
                    != Crypto_Key[cryptoKeyId]
                           .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                           .CryptoKeyElementFormat)
                {
                    Status = CRYPTO_E_KEY_SIZE_MISMATCH;
                }
                else if (
                    FALSE
                        == Crypto_Key[cryptoKeyId]
                               .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                               .CryptoKeyElementAllowPartialAccess
                    || FALSE
                           == Crypto_Key[cryptoKeyId]
                                  .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                                  .CryptoKeyElementAllowPartialAccess)
                {
                    Status = CRYPTO_E_KEY_SIZE_MISMATCH;
                }
                else
                {
                    for (i = 0; i < keyElementCopyLength; i++)
                    {
                        Crypto_Key[cryptoKeyId]
                            .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                            .CryptoKeyElementInitValue[i + keyElementTargetOffset] =
                            Crypto_Key[cryptoKeyId]
                                .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                                .CryptoKeyElementInitValue[i + keyElementSourceOffset];
                    }
                    if ((keyElementCopyLength + keyElementSourceOffset)
                        > Crypto_Key[cryptoKeyId]
                              .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                              .CryptoKeyElementSize)
                    {
                        *(Crypto_Key[cryptoKeyId]
                              .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexS]
                              .CryptoKeyElementLength) = keyElementCopyLength + keyElementSourceOffset;
                    }
                }
            }
        }
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key copy interface.
 * @details  Copies a key with all its elements to another key in the same crypto driver.
 *           Note: If the actual key element is directly mapped to flash memory,
 *           there could be a bigger delay when calling this function(synchronous operation)
 *
 * @param[in]       cryptoKeyId                   Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       targetCryptoKeyId             Holds the identifier of the key whose key element shall be the
 * destination element.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyCopy(VAR(uint32, AUTOMATIC) cryptoKeyId, VAR(uint32, AUTOMATIC) targetCryptoKeyId)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK;
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00156*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYCOPY, CRYPTO_E_UNINIT);
    }
    else
#endif
    {
        uint8 i, j;

        /*Find target / source Key Buffer in DescType*/
        if (Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->KeyElementNum
            == Crypto_Key[targetCryptoKeyId].CryptoKeyTypeRef->KeyElementNum)
        {
            for (i = 0; i < Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->KeyElementNum; i++)
            {
                if (Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[i].CryptoKeyElementReadAccess
                    == CRYPTO_RA_DENIED)
                {
                    Status = CRYPTO_E_KEY_READ_FAIL;
                }
                else if (
                    Crypto_Key[targetCryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[i].CryptoKeyElementWriteAccess
                    == CRYPTO_WA_DENIED)
                {
                    Status = CRYPTO_E_KEY_WRITE_FAIL;
                }
                else if (
                    Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[i].CryptoKeyElementFormat
                    != Crypto_Key[targetCryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[i].CryptoKeyElementFormat)
                {
                    Status = CRYPTO_E_KEY_SIZE_MISMATCH;
                }
                else
                {
                    Status = E_OK;
                }
                if (E_OK != Status)
                {
                    break;
                }
            }
            if (E_OK == Status)
            {
                /*@req SWS_Crypto_00159*/
                for (i = 0; i < Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->KeyElementNum; i++)
                {
                    if (Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[i].CryptoKeyElementFormat
                        != Crypto_Key[targetCryptoKeyId]
                               .CryptoKeyTypeRef->CryptoKeyElementRef[i]
                               .CryptoKeyElementFormat)
                    {
                        Status = CRYPTO_E_KEY_SIZE_MISMATCH;
                        break;
                    }
                    for (j = 0;
                         j < Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[i].CryptoKeyElementSize;
                         j++)
                    {
                        Crypto_Key[targetCryptoKeyId]
                            .CryptoKeyTypeRef->CryptoKeyElementRef[i]
                            .CryptoKeyElementInitValue[j] = Crypto_Key[cryptoKeyId]
                                                                .CryptoKeyTypeRef->CryptoKeyElementRef[i]
                                                                .CryptoKeyElementInitValue[j];
                    }
                }
            }
        }
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key get interface.
 * @details  Used to retrieve information which key elements are available in a given key.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       keyElementIdsLengthPtr       Holds a pointer to the memory location in which the number of key
 * elements in the given key is stored. On calling this function, this parameter shall contain the size of the buffer
 * provided by keyElementIdsPtr. When the request has finished,the actual number of key elements shall be stored.
 * @param[in,out]   None.
 * @param[out]      keyElementIdsPtr             Contains the pointer to the array where the ids of the key elements
 * shall be stored.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementIdsGet(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    /* PRQA S 2980 ++ */ /* MISRA Rule 2.2 */
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) keyElementIdsPtr,

    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) keyElementIdsLengthPtr)
/* PRQA S 2980 -- */ /* MISRA Rule 2.2 */
{
    Std_ReturnType Status = E_OK;
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00161*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMEMTIDSGET, CRYPTO_E_UNINIT);
    }
    else
#endif
    {
        uint32 Count;
        uint32 temp;

        temp = Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->KeyElementNum;

        if (E_NOT_OK == Crypto_DriverStatus)
        {
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
            Crypto_ReportDetErr(CRYPTO_SID_KEYELEMEMTIDSGET, CRYPTO_E_BUSY);
#endif
            Status = (Std_ReturnType)E_NOT_OK;
        }
        else
        {
            for (Count = 0; Count < temp; Count++)
            {
                keyElementIdsPtr[Count] =
                    Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[Count].CryptoKeyElementId;
                keyElementIdsLengthPtr[Count] =
                    Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[Count].CryptoKeyElementSize;
            }
        }
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key generate interface.
 * @details  This function generates the internal seed state using the provided entropy source.
 *           Furthermore,this function can be used to update the seed state with new entropy
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       seedPtr                      Holds a pointer to the memory location which contains the data to feed
 * the seed.
 * @param[in]       seedLength                   Contains the length of the seed in bytes.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_RandomSeed(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) seedPtr,
    VAR(uint32, AUTOMATIC) seedLength)
{
    Std_ReturnType Status = E_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2*/
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00128*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_RANDOMSEED, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00130*/
    else if (NULL_PTR == seedPtr)
    {
        Crypto_ReportDetErr(CRYPTO_SID_RANDOMSEED, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00131*/
    else if (0u == seedLength)
    {
        Crypto_ReportDetErr(CRYPTO_SID_RANDOMSEED, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        Status = Crypto_RandomSeedInternal(cryptoKeyId, seedPtr, seedLength);
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key generate interface.
 * @details  Generates new key material store it in the key identified by cryptoKeyId.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_KeyGenerate(VAR(uint32, AUTOMATIC) cryptoKeyId)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK;
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00094*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYGENERATE, CRYPTO_E_UNINIT);
    }
    else
#endif
    {
#if (CRYPTO_ALGORITHM_RSA == STD_ON)
        /*@req SWS_Crypto_00165*/
        Status = rsa_keygenerate(cryptoKeyId);
#endif /*CRYPTO_ALGORITHM_RSA == STD_ON*/
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key Derivation interface.
 * @details  Derives a new key by using the key elements in the given key identified by the cryptoKeyId.
 *           The given key contains the key elements for the password,salt.
 *           The derived key is stored in the key element with the id 1 of the key identified by targetCryptoKeyId.
 *           The number of iterations is given in the key element CRYPTO_KE_KEYDERIVATION_ITERATIONS.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       targetCryptoKeyId            Holds the identifier of the key whose key element shall be the
 * destination element.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyDerive(VAR(uint32, AUTOMATIC) cryptoKeyId, VAR(uint32, AUTOMATIC) targetCryptoKeyId)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2*/
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00097*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYDERIVE, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00098*/ /*@req SWS_Crypto_00180*/
    else if ((cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED) || (targetCryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYGENERATE, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        Status = (Std_ReturnType)E_NOT_OK;
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key Exchange interface.
 * @details  Calculates the public value for the key exchange and
 *           stores the public key in the memory location pointed by the public value pointer.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 *
 * @param[in,out]   publicValueLengthPtr         Holds a pointer to the memory location in which the public value length
 * information is stored.
 * @param[out]      publicValuePtr               Contains the pointer to the data where the public value shall be
 * stored.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyExchangeCalcPubVal(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) publicValueLengthPtr)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2*/

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00103*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCPUBVAL, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00105*/ /*@req SWS_Crypto_00106*/
    else if ((NULL_PTR == publicValuePtr) || (NULL_PTR == publicValueLengthPtr))
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCPUBVAL, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00107*/
    else if (0u == *publicValueLengthPtr)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCPUBVAL, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        Status = Crypto_KeyExchangeCalcPubVal_internal(cryptoKeyId, publicValuePtr, publicValueLengthPtr);
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key Exchange interface.
 * @details  Calculates the shared secret key for the key exchange with the key material of the key identified by the
 * cryptoKeyId and the partner public key. The shared secret key is stored as a key element in the same key.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       partnerPublicValuePtr        Holds the pointer to the memory location which contains the partner's
 * public value.
 * @param[in]       partnerPublicValueLength     Contains the length of the partner's public value in bytes.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyExchangeCalcSecret(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, AUTOMATIC) partnerPublicValueLength)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2*/
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00111*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCSECRET, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00113*/
    else if (NULL_PTR == partnerPublicValuePtr)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCSECRET, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00115*/
    else if (0u == partnerPublicValueLength)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCSECRET, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        Status = Crypto_KeyExchangeCalcSecret_internal(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength);
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key Certificate interface.
 * @details  Parses the certificate data stored in the key element CRYPTO_KE_CERT_DATA and fills the key elements
 *           CRYPTO_KE_CERT_SIGNEDDATA,CRYPTO_KE_CERT_PARSEDPUBLICKEY and CRYPTO_KE_CERT_SIGNATURE.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_CertificateParse(VAR(uint32, AUTOMATIC) cryptoKeyId)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2*/
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00168*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_CERTIFICATE_PARSE, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00169*/
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_ReportDetErr(CRYPTO_SID_CERTIFICATE_PARSE, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        Status = E_OK;
    }
    return Status;
}

/**
 * @brief    CRYPTO driver key Certificate interface.
 * @details  Verifies the certificate stored in the key referenced by cryptoValidateKeyId with the certificate stored in
 * the key referenced by cryptoKeyId.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       verifyCryptoKeyId            Holds the identifier of the key contain
 *
 * @param[in,out]   None.
 * @param[out]      verifyPtr                    Holds a pointer to the memory location which will contain the result of
 * the certificate verification.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_CertificateVerify(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    VAR(uint32, AUTOMATIC) verifyCryptoKeyId,
    /* PRQA S 3673 ++ */ /* MISRA Rule 8.13*/
    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_APPL_DATA) verifyPtr)
/* PRQA S 3673-- */ /* MISRA Rule 8.13*/
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2*/
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00172*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_DriverStatus)
    {
        Crypto_ReportDetErr(CRYPTO_SID_CERTIFICATE_VERIFY, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00173*/ /*@req SWS_Crypto_00174*/
    else if ((cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED) || (verifyCryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_ReportDetErr(CRYPTO_SID_CERTIFICATE_VERIFY, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00175*/
    else if (NULL_PTR == verifyPtr)
    {
        Crypto_ReportDetErr(CRYPTO_SID_CERTIFICATE_VERIFY, CRYPTO_E_PARAM_POINTER);
    }
    else
#endif
    {
        Status = E_OK;
    }
    return Status;
}
/**
* @brief    Crypto_MainFunction.
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

FUNC(void, CSM_CODE) Crypto_MainFunction(void)
{
    Std_ReturnType Status = E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2*/
#if (STD_ON == CRYPTO_JOB_QUEUING)
    boolean clrFlag = FALSE;
#endif

    if ((ASYNFlag == TRUE)
#if (STD_ON == CRYPTO_JOB_QUEUING)
        || (NULL_PTR != Crypto_JobInQueue[0])
#endif /*STD_ON == CRYPTO_JOB_QUEUING*/
    )
    {
#if (STD_ON == CRYPTO_JOB_QUEUING)
        if ((Crypto_StoredJob.jobId == CRYPTO_JOB_NOT_VALID) && (NULL_PTR != Crypto_JobInQueue[0]))
        {
            Crypto_StoredJob = *(Crypto_JobInQueue[0]);
            clrFlag = TRUE;
        }
#endif /*STD_ON == CRYPTO_JOB_QUEUING*/
        ASYNFlag = FALSE;
        switch (Crypto_StoredJob.jobPrimitiveInputOutput.mode)
        {
        case CRYPTO_OPERATIONMODE_START:
            CryIf_CallbackNotification(&Crypto_StoredJob, E_OK);
            break;
        case CRYPTO_OPERATIONMODE_UPDATE:
            Status = Crypto_ProcessAlgorithm();
            CryIf_CallbackNotification(&Crypto_StoredJob, Status);
            break;
        case CRYPTO_OPERATIONMODE_FINISH:
            Status = Crypto_Finish(); /* PRQA S 2982 */ /* MISRA Rule 2.2*/
            break;
        case CRYPTO_OPERATIONMODE_SINGLECALL:
            Crypto_DriverStatus = CRYPTO_DRIVER_BUSY;
            Status = Crypto_Finish(); /* PRQA S 2982 */ /* MISRA Rule 2.2*/
            break;
        case CRYPTO_OPERATIONMODE_STREAMSTART:
            Crypto_DriverStatus = CRYPTO_DRIVER_BUSY;
            Status = Crypto_ProcessAlgorithm();
            CryIf_CallbackNotification(&Crypto_StoredJob, Status);
            break;
        default:
            break;
        }
#if (STD_ON == CRYPTO_JOB_QUEUING)
        if ((CRYPTO_DRIVER_IDLE == Crypto_DriverStatus) && (clrFlag == TRUE))
        {
            if (NULL_PTR != Crypto_JobInQueue[0])
            {
                Crypto_DriverStatus = CRYPTO_DRIVER_BUSY;
                ASYNFlag = TRUE;
                Status = Crypto_CancelQueuedJob(Crypto_JobInQueue[0]); /* PRQA S 2983 */ /* MISRA Rule 2.2*/
            }
        }
#endif /*STD_ON == CRYPTO_JOB_QUEUING*/
    }
}
#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"
/*******************************************************************************
**                      Private Function                                      **
*******************************************************************************/

/**
 * @brief   The function Crypto_QueueJob queues a job in the corresponding job queue.
 * @details The function is responsible of queuing a job at the right place in corresponding job queue.
 *
 * @param[in]      Crypto_JobType *  job.
 * @param[out]     None.
 *
 * @return         Std_ReturnType.
 * @retval         E_NOT_OK                       : The request parameters are inconsistent.
 *                 E_OK                           : Request accepted and sent to the HSM.
 */
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"
#if (CRYPTO_JOB_QUEUING == STD_ON)
static FUNC(void, CRYPTO_CODE) Crypto_SetQueueBuf(
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) destJob,
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) srcJob)
{
    destJob->jobId = srcJob->jobId;
    destJob->jobState = srcJob->jobState;
    destJob->jobPrimitiveInputOutput = srcJob->jobPrimitiveInputOutput;
    destJob->jobPrimitiveInfo = srcJob->jobPrimitiveInfo;
    destJob->jobInfo = srcJob->jobInfo;
    destJob->cryptoKeyId = srcJob->cryptoKeyId;
    destJob->jobRedirectionInfoRef = srcJob->jobRedirectionInfoRef;
    destJob->targetCryptoKeyId = srcJob->targetCryptoKeyId;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_QueueJob(P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job)
{
    uint32 offset;
    Std_ReturnType Status = E_NOT_OK;

    Crypto_SetQueueBuf(&Crypto_JobTempBuf, job);
    if (NULL_PTR == Crypto_JobInQueue[0UL])
    {
        /* JobQueue is empty */
        /* @req [SWS_Crypto_00033] */
        Crypto_JobInQueue[0] = &Crypto_JobTempBuf;
    }
    else if (NULL_PTR != Crypto_JobInQueue[CRYPTO_MAX_QUEUE_SIZE - 1U])
    {
        /* JobQueue is full */
        /* @req SWS_Crypto_00032 */
        Status = CRYPTO_E_BUSY;
    }
    else
    {
        /* JobQueue is not empty */
        /* Rank the items by priority */
        /* @req [SWS_Crypto_00030] */
        for (offset = CRYPTO_MAX_QUEUE_SIZE - 1UL; offset > 0UL; offset--)
        {
            /* Search for registered jobs in the queue */
            if (NULL_PTR != Crypto_JobInQueue[offset - 1U])
            {
                /* The higher the job priority value,the higher the job priority. */
                if (job->jobInfo->jobPriority > Crypto_JobInQueue[offset - 1U]->jobInfo->jobPriority)
                {
                    /* job prior to Crypto_JobInQueue[offset] - shift the item */
                    Crypto_JobInQueue[offset] = Crypto_JobInQueue[offset - 1U];
                    Crypto_JobInQueue[offset - 1U] = &Crypto_JobTempBuf;
                    /* PRQA S 2992,2996 ++ */ /* MISRA Rule 14.3*/
                    if (0UL == offset)
                    /* PRQA S 2992,2996 -- */ /* MISRA Rule 14.3*/
                    {
                        Status = E_OK; /* PRQA S 2880  */ /* MISRA Rule 2.1*/
                    }
                }
                else
                {
                    /* shifted all lower priority elements now,we can insert the new request here. */
                    /* @req [SWS_Crypto_00033] */
                    Crypto_JobInQueue[offset] = &Crypto_JobTempBuf;
                    Status = E_OK;
                    break;
                }
            }
        }
    }
    return Status;
}

/**
 * @brief   The function Crypto_CancelQueuedJob removes the job from the corresponding job queue.
 * @details The function is responsible of searching the requested job and extract it from job queue.
 *
 * @param[in]      Crypto_JobType *  job.
 * @param[out]     None.
 *
 * @return         Std_ReturnType.
 * @retval         E_NOT_OK                       : job has not been found.
 *                 E_OK                           : job has been found and removed from the queue.
 */
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_CancelQueuedJob(P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job)
{
    uint32 offset;
    uint8 shift;
    Std_ReturnType Status = E_NOT_OK;

    for (offset = 0UL; offset < (CRYPTO_MAX_QUEUE_SIZE - 1UL); offset++)
    {
        /* Don't touch the items in queue before finding the job to cancel */
        if (job == Crypto_JobInQueue[offset])
        {
            for (shift = offset; shift < CRYPTO_MAX_QUEUE_SIZE - offset - 1U; shift++)
            {
                Crypto_JobInQueue[shift] = Crypto_JobInQueue[shift + 1U];
                if (NULL_PTR == Crypto_JobInQueue[shift])
                {
                    break;
                }
            }
            Crypto_JobInQueue[shift + 1U] = NULL_PTR;
            Status = E_OK;
            if (NULL_PTR == Crypto_JobInQueue[0])
            {
                Crypto_DriverStatus = CRYPTO_DRIVER_IDLE;
                ASYNFlag = FALSE;
            }
            break;
        }
    }
    return Status;
}
#endif

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_ProcessSYNCJob(P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job)
{
    Std_ReturnType Status = E_NOT_OK;
    switch (job->jobPrimitiveInputOutput.mode)
    {
    case CRYPTO_OPERATIONMODE_START:
        Crypto_Start(job);
        Status = E_OK;
        break;
    case CRYPTO_OPERATIONMODE_UPDATE:
        if (CRYPTO_OPERATIONMODE_FINISH != Crypto_StoredJob.jobPrimitiveInputOutput.mode
            && CRYPTO_JOBSTATE_ACTIVE == Crypto_StoredJob.jobState)
        {
            Crypto_StoredJob = *job;
            Status = Crypto_Update();
        }
        break;
    case CRYPTO_OPERATIONMODE_FINISH:
        if ((CRYPTO_OPERATIONMODE_FINISH != Crypto_StoredJob.jobPrimitiveInputOutput.mode)
            && CRYPTO_JOBSTATE_ACTIVE == Crypto_StoredJob.jobState)
        {
            Crypto_StoredJob = *job;
            Status = Crypto_Finish();
        }
        break;
    case CRYPTO_OPERATIONMODE_STREAMSTART:
        Crypto_Start(job);
        Status = Crypto_Update();
        if (Status == E_OK)
        {
            Status = Crypto_Finish();
        }
        break;
    case CRYPTO_OPERATIONMODE_SINGLECALL:
        Crypto_Start(job);
        Status = Crypto_Finish();
        break;
    default:
        break;
    }
    return Status;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_ProcessASYNCJob_NONQUEUE(P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job)
{
    Std_ReturnType Status = E_OK;
    ASYNFlag = TRUE;
    switch (job->jobPrimitiveInputOutput.mode)
    {
    case CRYPTO_OPERATIONMODE_START:
        Crypto_Start(job);
        Crypto_StoredJob.jobState = CRYPTO_JOBSTATE_ACTIVE;
        break;
    case CRYPTO_OPERATIONMODE_UPDATE:
        if (CRYPTO_OPERATIONMODE_START == Crypto_StoredJob.jobPrimitiveInputOutput.mode
            && CRYPTO_JOBSTATE_ACTIVE == Crypto_StoredJob.jobState)
        {
            Crypto_StoredJob = *job;
        }
        break;
    case CRYPTO_OPERATIONMODE_FINISH:
        if ((CRYPTO_OPERATIONMODE_UPDATE == Crypto_StoredJob.jobPrimitiveInputOutput.mode
             || CRYPTO_OPERATIONMODE_STREAMSTART == Crypto_StoredJob.jobPrimitiveInputOutput.mode)
            && CRYPTO_JOBSTATE_ACTIVE == Crypto_StoredJob.jobState)
        {
            Crypto_StoredJob = *job;
        }
        else
        {
            Status = E_NOT_OK;
        }
        break;
    case CRYPTO_OPERATIONMODE_STREAMSTART:
        Crypto_Start(job);
        break;
    case CRYPTO_OPERATIONMODE_SINGLECALL:
        Crypto_Start(job);
        break;
    default:
        break;
    }
    return Status;
}

FUNC(void, CRYPTO_CODE) Crypto_Start(P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job)
{
    Crypto_StoredJob = *job;
    Crypto_StoredJob.jobState = CRYPTO_JOBSTATE_ACTIVE;
    Crypto_DriverStatus = CRYPTO_DRIVER_BUSY;
    return;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_Update(void)
{
    Std_ReturnType ret;
    ret = Crypto_ProcessAlgorithm();
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_Finish(void)
{
    Std_ReturnType ret;
    ret = Crypto_ProcessAlgorithm();
    Crypto_DriverStatus = CRYPTO_DRIVER_IDLE;
    if (CRYPTO_PROCESSING_ASYNC == Crypto_StoredJob.jobPrimitiveInfo->processingType)
    {
        CryIf_CallbackNotification(&Crypto_StoredJob, ret);
    }
    if (ASYNFlag == FALSE)
    {
        Crypto_ClearStoredJob();
    }

    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_RandomSeedInternal(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) seedPtr,
    VAR(uint32, AUTOMATIC) seedLength)
{
    Std_ReturnType Status;
    uint8 Index;
    uint32 KeyElementIndex = 0;
    Status = Crypto_KeyElementIndexFind(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &KeyElementIndex);
    if (E_OK == Status)
    {
        for (Index = 0; Index < seedLength; Index++)
        {
            Crypto_Key[cryptoKeyId]
                .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                .CryptoKeyElementInitValue[Index] = seedPtr[Index];
        }
        *(Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex].CryptoKeyElementLength) =
            seedLength;
    }
    return Status;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_KeySetValid_internal(VAR(uint32, AUTOMATIC) cryptoKeyId)
{

    Std_ReturnType Status = E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2*/

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00197*/
    if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYSETVALID, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
    {
        CryptoKeyStatus[cryptoKeyId] = CRYPTO_KEYSTATE_VALID;
        Status = E_OK;
    }
    return Status;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyExchangeCalcPubVal_internal(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) publicValueLengthPtr)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK;
#if (CRYPTO_ALGORITHM_DH == STD_ON)
    uint32 KeyElementIndexp = 0;
    uint32 KeyElementIndexg = 0;

    Status = Crypto_KeyElementIndexFind(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_BASE, &KeyElementIndexp);

    if (E_OK == Status)
    {
        Status = Crypto_KeyElementIndexFind(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, &KeyElementIndexg);
        if (E_OK == Status)
        {
            /*@req SWS_Crypto_00167*/
            if ((Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexp].CryptoKeyElementSize
                 != 0)
                || (Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndexg].CryptoKeyElementSize
                    != 0))
            {
                Status = Crypto_DHPublic_KeyElement(cryptoKeyId, publicValuePtr, publicValueLengthPtr);
            }
            else
            {
                Status = Crypto_DHPublic_Generate(cryptoKeyId, publicValuePtr, publicValueLengthPtr);
            }
        }
    }
#endif /*#ifCRYPTO_ALGORITHM_DH == STD_ON*/
    return Status;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyExchangeCalcSecret_internal(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, AUTOMATIC) partnerPublicValueLength)
{
    Std_ReturnType Status = (Std_ReturnType)E_NOT_OK;
#if (CRYPTO_ALGORITHM_DH == STD_ON)
    Status = Crypto_DHSecret(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength);
#endif
    return Status;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_sha256_Process()
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 output[32];

    uint32 ilen = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;
    if (*(Crypto_StoredJob.jobPrimitiveInputOutput.outputLengthPtr) >= 32)
    {
        /* PRQA S 0311 ++ */ /* MISRA Rule 11.8 */
        uint8* input = (uint8*)(Crypto_StoredJob.jobPrimitiveInputOutput.inputPtr);
        /* PRQA S 0311 -- */ /* MISRA Rule 11.8 */

        ret = Crypto_sha256(input, ilen, output);
        Crypto_AlgorithmOutput(output, 32);
    }
    return ret;
}

FUNC(Std_ReturnType, CMAC_CODE) Crypto_GernerateMAC_Process()
{
    Std_ReturnType ret;
    uint8 result[16];
    uint8 Key[16] = {0x00};
    uint8 IVKey[16] = {0x00};
    uint8 input[64];
    uint8 inputlength = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;

    ret = Get_Key(Crypto_StoredJob.cryptoKeyId, Key, CRYPTO_KE_MAC_KEY);
    if (E_OK == ret)
    {
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, IVKey, CRYPTO_KE_CIPHER_IV);
    }
    if (E_OK == ret)
    {
        Crypto_AlgorithmGetInput(input);
        ret = Crypto_GernerateMAC(result, Key, IVKey, input, inputlength);
    }

    if (E_OK == ret)
    {
        Crypto_AlgorithmOutput(result, 16);
        *(Crypto_StoredJob.jobPrimitiveInputOutput.outputLengthPtr) = 16;
    }

    return ret;
}

FUNC(Std_ReturnType, CMAC_CODE) Crypto_VerifyMAC_Process()
{
    Std_ReturnType ret;
    uint8 Key[16] = {0X00};
    uint8 IVKey[16] = {0X00};
    uint8 input[64] = {0X00};
    uint8 MAC_Result[16] = {0X00};
    uint32 Verifylength;
    uint8 i;
    uint8 inputlength = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;

    Verifylength = Crypto_StoredJob.jobPrimitiveInputOutput.secondaryInputLength;
    ret = Get_Key(Crypto_StoredJob.cryptoKeyId, Key, CRYPTO_KE_MAC_KEY);
    if (E_OK == ret)
    {
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, IVKey, CRYPTO_KE_CIPHER_IV);
    }
    for (i = 0; i < Verifylength; i++)
    {
        MAC_Result[i] = Crypto_StoredJob.jobPrimitiveInputOutput.secondaryInputPtr[i];
    }

    if (E_OK == ret)
    {
        Crypto_AlgorithmGetInput(input);
        ret = Crypto_VerifyMAC(Key, IVKey, input, inputlength, &MAC_Result[0], Verifylength);
    }

    if (E_OK == ret)
    {
        *(Crypto_StoredJob.jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_OK;
    }
    else
    {
        *(Crypto_StoredJob.jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_NOT_OK;
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AESEncryptProcess()
{
    uint8 key[16];
    uint8 iv[16];
    uint8 buf[64];
    Std_ReturnType ret;
    uint8 outputLength;
    uint8 inputLength;
    Crypto_memset(key, 16);
    Crypto_memset(iv, 16);
    Crypto_memset(buf, 64);
    ret = Crypto_AlgorithmGetInput(buf);
    if (ret == E_NOT_OK)
    {
        return ret;
    }
    ret = Get_Key(Crypto_StoredJob.cryptoKeyId, key, CRYPTO_KE_CIPHER_KEY);
    if (ret == E_NOT_OK)
    {
        return ret;
    }

    switch (Crypto_StoredJob.jobPrimitiveInfo->primitiveInfo->algorithm.mode)
    {
    case CRYPTO_ALGOMODE_ECB:
        ret = Crypto_AESEncryptECB(key, buf);
        break;
    case CRYPTO_ALGOMODE_CBC:
        inputLength = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, iv, CRYPTO_KE_CIPHER_IV);
        if (ret == E_OK)
        {
            ret = Crypto_AESEncryptCBC(key, buf, iv, inputLength);
        }
        break;
    case CRYPTO_ALGOMODE_CTR:
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, iv, CRYPTO_KE_CIPHER_IV);
        if (ret == E_OK)
        {
            ret = Crypto_AESEncryptCTR(key, buf, iv);
        }
        break;
    }

    if ((buf != NULL_PTR) && (E_OK == ret)) /* PRQA S 2982,2995 */ /* MISRA Rule 2.2*/
    {
        outputLength = *(Crypto_StoredJob.jobPrimitiveInputOutput.outputLengthPtr);
        Crypto_AlgorithmOutput(buf, outputLength);
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AESDecryptProcess()
{
    uint8 key[16];
    uint8 iv[16];
    uint8 buf[64];
    Std_ReturnType ret;
    uint8 outputLength;
    uint8 inputLength;

    Crypto_memset(key, 16);
    Crypto_memset(iv, 16);
    Crypto_memset(buf, 64);
    ret = Crypto_AlgorithmGetInput(buf);
    if (ret == E_NOT_OK)
    {
        return ret;
    }
    ret = Get_Key(Crypto_StoredJob.cryptoKeyId, key, CRYPTO_KE_CIPHER_KEY);
    if (ret == E_NOT_OK)
    {
        return ret;
    }

    switch (Crypto_StoredJob.jobPrimitiveInfo->primitiveInfo->algorithm.mode)
    {
    case CRYPTO_ALGOMODE_ECB:
        ret = Crypto_AESDecryptECB(key, buf);
        break;
    case CRYPTO_ALGOMODE_CBC:
        inputLength = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, iv, CRYPTO_KE_CIPHER_IV);
        if (ret == E_OK)
        {
            ret = Crypto_AESDecryptCBC(key, buf, iv, inputLength);
        }
        break;
    case CRYPTO_ALGOMODE_CTR:
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, iv, CRYPTO_KE_CIPHER_IV);
        if (ret == E_OK)
        {
            ret = Crypto_AESDecryptCTR(key, buf, iv);
        }
        break;
    }

    if (E_OK == ret)
    {
        outputLength = *(Crypto_StoredJob.jobPrimitiveInputOutput.outputLengthPtr);
        Crypto_AlgorithmOutput(buf, outputLength);
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_CtrDrbgProcess()
{
    Std_ReturnType ret;
    uint8 key[16];
    uint8 buf[16];
    Crypto_memset(key, 16);
    Crypto_memset(buf, 16);

    ret = Get_Key(Crypto_StoredJob.cryptoKeyId, key, CRYPTO_KE_RANDOM_SEED_STATE);
    if (E_OK == ret)
    {
        ret = Crypto_CtrDrbg(key, buf);
        if (E_OK == ret)
        {
            Crypto_AlgorithmOutput(buf, 16);
        }
    }
    return ret;
}

#if (CRYPTO_ALGORITHM_RSA == STD_ON)
FUNC(Std_ReturnType, CRYPTO_CODE) rsa_encryptProcess()
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 keyN[128 * CRYPTO_RSA_TYPE];
    uint8 keyE[4 * CRYPTO_RSA_TYPE];
#if (CRYPTO_RSA_TYPE == 1)
    /*RSA 1024*/
    uint8 Plaintext[128u];
    uint8 Output[128u];
    uint32 outLen = 128u;
#elif (CRYPTO_RSA_TYPE == 2)
    /*RSA 2048*/
    uint8 Plaintext[256u];
    uint8 Output[256u];
    uint32 outLen = 256u;
#else
    /*RSA 3072*/
    uint8 Plaintext[384u];
    uint8 Output[384u];
    uint32 outLen = 384u;
#endif
    uint32 outputLength = *(Crypto_StoredJob.jobPrimitiveInputOutput.outputLengthPtr);
    uint32 Length = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;

    if (outputLength <= outLen)
    {
        Crypto_memset(Plaintext, outLen);
        Crypto_memset(Output, outLen);

        ret = Crypto_AlgorithmGetInput(Plaintext);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyN, 255);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyE, 254);
        if (E_OK == ret)
        {
            ret = rsa_encrypt(keyN, keyE, Plaintext, Length, Output);
        }
        if (E_OK == ret)
        {
            Crypto_AlgorithmOutput(Output, outputLength);
        }
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) rsa_decryptProcess()
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 keyN[128 * CRYPTO_RSA_TYPE];
    uint8 keyP[64 * CRYPTO_RSA_TYPE];
    uint8 keyQ[64 * CRYPTO_RSA_TYPE];
    uint8 keyD[128 * CRYPTO_RSA_TYPE];
    uint8 keyE[4 * CRYPTO_RSA_TYPE];
#if (CRYPTO_RSA_TYPE == 1)
    /*RSA 1024*/
    uint8 Ciphertext[128u];
    uint8 Outcome[128u];
    uint32 outLen = 128u;
#elif (CRYPTO_RSA_TYPE == 2)
    /*RSA 2048*/
    uint8 Ciphertext[256u];
    uint8 Outcome[256u];
    uint32 outLen = 256u;
#else
    /*RSA 3072*/
    uint8 Ciphertext[384u];
    uint8 Outcome[384u];
    uint32 outLen = 384u;
#endif
    uint32 Length = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;
    uint16 outputLength = *(Crypto_StoredJob.jobPrimitiveInputOutput.outputLengthPtr);

    if (outputLength <= outLen)
    {
        Crypto_memset(Ciphertext, outLen);
        Crypto_memset(Outcome, outLen);

        ret = Crypto_AlgorithmGetInput(Ciphertext);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyN, 255);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyP, 253);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyQ, 252);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyD, 251);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyE, 254);

        if (E_OK == ret)
        {
            ret = rsa_decrypt(keyN, keyP, keyQ, keyD, keyE, Length, Outcome, Ciphertext);
        }
        if (E_OK == ret)
        {
            Crypto_AlgorithmOutput(Outcome, outputLength);
        }
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) rsa_signProcess()
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 keyN[128 * CRYPTO_RSA_TYPE];
    uint8 keyP[64 * CRYPTO_RSA_TYPE];
    uint8 keyQ[64 * CRYPTO_RSA_TYPE];
    uint8 keyD[128 * CRYPTO_RSA_TYPE];
    uint8 keyE[4 * CRYPTO_RSA_TYPE];
#if (CRYPTO_RSA_TYPE == 1)
    /*RSA 1024*/
    uint8 Plaintext[128u];
    uint8 Output[128u];
    uint32 outLen = 128u;
#elif (CRYPTO_RSA_TYPE == 2)
    /*RSA 2048*/
    uint8 Plaintext[256u];
    uint8 Output[256u];
    uint32 outLen = 256u;
#else
    /*RSA 3072*/
    uint8 Plaintext[384u];
    uint8 Output[384u];
    uint32 outLen = 384u;
#endif
    uint16 outputLength = *(Crypto_StoredJob.jobPrimitiveInputOutput.outputLengthPtr);
    uint8 Length = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;

    if (outputLength <= outLen)
    {
        Crypto_memset(Plaintext, outLen);
        Crypto_memset(Output, outLen);

        ret = Crypto_AlgorithmGetInput(Plaintext);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyN, 255);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyP, 253);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyQ, 252);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyD, 251);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyE, 254);

        if (E_OK == ret)
        {
            ret = rsa_sign(keyN, keyP, keyQ, keyD, keyE, Length, Plaintext, Output);
        }
        if (E_OK == ret)
        {
            Crypto_AlgorithmOutput(Output, outputLength);
        }
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) rsa_verifyProcess()
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 keyN[128 * CRYPTO_RSA_TYPE];
    uint8 keyE[4 * CRYPTO_RSA_TYPE];
    uint32 Length = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;
#if (CRYPTO_RSA_TYPE == 1)
    /*RSA 1024*/
    uint8 Ciphertext[128u];
    uint8 sig[128u];
#elif (CRYPTO_RSA_TYPE == 2)
    /*RSA 2048*/
    uint8 Ciphertext[256u];
    uint8 sig[256u];
#else
    /*RSA 3072*/
    uint8 Ciphertext[384u];
    uint8 sig[384u];
#endif

    Crypto_memset(Ciphertext, sizeof(Ciphertext));
    Crypto_memset(sig, sizeof(sig));
    Crypto_memset(keyN, 128 * CRYPTO_RSA_TYPE);
    Crypto_memset(keyE, 4 * CRYPTO_RSA_TYPE);

    ret = Crypto_AlgorithmGetInput(Ciphertext);
    Crypto_AlgorithmGetSecondInput(sig);
    if (E_OK == ret)
    {
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyN, 255);
        ret = Get_Key(Crypto_StoredJob.cryptoKeyId, keyE, 254);
    }

    if (E_OK == ret)
    {
        ret = rsa_verify(keyN, keyE, Length, Ciphertext, sig);
    }

    if (E_OK == ret)
    {
        *(Crypto_StoredJob.jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_OK;
    }
    else
    {
        *(Crypto_StoredJob.jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_NOT_OK;
    }
    return ret;
}
#endif /*CRYPTO_ALGORITHM_RSA == STD_ON*/
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AlgorithmGetInput(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) buf)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32 i, p;
    p = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;

    if (Crypto_StoredJob.jobPrimitiveInputOutput.inputPtr != NULL_PTR)
    {
        for (i = 0; i < p; i++)
        {
            buf[i] = Crypto_StoredJob.jobPrimitiveInputOutput.inputPtr[i];
        }
        ret = E_OK;
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AlgorithmGetSecondInput(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) buf)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32 i, p;
    p = Crypto_StoredJob.jobPrimitiveInputOutput.secondaryInputLength;

    if (Crypto_StoredJob.jobPrimitiveInputOutput.secondaryInputPtr != NULL_PTR)
    {
        for (i = 0; i < p; i++)
        {
            buf[i] = Crypto_StoredJob.jobPrimitiveInputOutput.secondaryInputPtr[i];
        }
        ret = E_OK;
    }
    return ret;
}
FUNC(void, CRYPTO_CODE)
Crypto_AlgorithmOutput(P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) buf, VAR(uint32, AUTOMATIC) outputLength)
{
    uint32 i;
    for (i = 0; i < outputLength; i++)
    {
        Crypto_StoredJob.jobPrimitiveInputOutput.outputPtr[i] = buf[i];
    }
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_ProcessAlgorithm(void)
{
    Std_ReturnType Result = E_NOT_OK;
    /* Service cases*/

    uint8 algorithmfamily = Crypto_StoredJob.jobPrimitiveInfo->primitiveInfo->algorithm.family;
    switch (Crypto_StoredJob.jobPrimitiveInfo->primitiveInfo->service)
    {
    case CRYPTO_HASH:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_SHA2_256:
#if (CRYPTO_ALGORITHM_HASH == STD_ON)
            Result = Crypto_sha256_Process();
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_MACGENERATE:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_AES:
#if (CRYPTO_ALGORITHM_CMAC == STD_ON)
            Result = Crypto_GernerateMAC_Process();
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_MACVERIFY:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_AES:
#if (CRYPTO_ALGORITHM_CMAC == STD_ON)
            Result = Crypto_VerifyMAC_Process();
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_ENCRYPT:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_AES:
#if (CRYPTO_ALGORITHM_AES == STD_ON)
            Result = Crypto_AESEncryptProcess();
#endif
            break;
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHM_RSA == STD_ON)
            Result = rsa_encryptProcess();
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_DECRYPT:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_AES:
#if (CRYPTO_ALGORITHM_AES == STD_ON)
            Result = Crypto_AESDecryptProcess();
#endif
            break;
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHM_RSA == STD_ON)
            Result = rsa_decryptProcess();
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_AEADENCRYPT:
        break;
    case CRYPTO_AEADDECRYPT:
        break;
    case CRYPTO_SIGNATUREGENERATE:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHM_RSA == STD_ON)
            Result = rsa_signProcess();
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_SIGNATUREVERIFY:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHM_RSA == STD_ON)
            Result = rsa_verifyProcess();
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_RANDOMGENERATE:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RNG:
#if (CRYPTO_ALGORITHM_CTRG == STD_ON)
            Result = Crypto_CtrDrbgProcess();
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_RANDOMSEED:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_DRBG:
#if (CRYPTO_ALGORITHM_CTRG == STD_ON)
            Result = Crypto_RandomSeedInternal(
                Crypto_StoredJob.cryptoKeyId,
                Crypto_StoredJob.jobPrimitiveInputOutput.inputPtr,
                Crypto_StoredJob.jobPrimitiveInputOutput.inputLength);
#endif
            break;
        default:
            break;
        }
        break;
    case CRYPTO_KEYGENERATE:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHM_RSA == STD_ON)
            Result = rsa_keygenerate(Crypto_StoredJob.cryptoKeyId);
#endif /*CRYPTO_ALGORITHM_CTRG == STD_ON*/
            break;
        default:
            break;
        }
        break;
    case CRYPTO_KEYDERIVE:
        break;
    case CRYPTO_KEYEXCHANGECALCPUBVAL:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_DH:
#if (CRYPTO_ALGORITHM_DH == STD_ON)
        {
            uint32 temp;
            uint8 temp1[255] = {0};
            temp = Crypto_StoredJob.jobPrimitiveInputOutput.inputLength;
            Result = Crypto_AlgorithmGetInput(temp1);
            Result = Crypto_KeyExchangeCalcPubVal_internal(Crypto_StoredJob.cryptoKeyId, temp1, &temp);
        }
#endif
        break;
        default:
            break;
        }
        break;
    case CRYPTO_KEYEXCHANGECALCSECRET:
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_DH:
#if (CRYPTO_ALGORITHM_DH == STD_ON)
            Result = Crypto_KeyExchangeCalcSecret_internal(
                Crypto_StoredJob.cryptoKeyId,
                Crypto_StoredJob.jobPrimitiveInputOutput.inputPtr,
                Crypto_StoredJob.jobPrimitiveInputOutput.inputLength);
#endif

            break;
        default:
            break;
        }
        break;
    case CRYPTO_KEYSETVALID:
        Result = Crypto_KeySetValid_internal(Crypto_StoredJob.cryptoKeyId);
        break;
    default:
        break;
    }
    return Result;
}
#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"
/* PRQA S 3432 -- */                /* MISRA Rule 20.7 */
/* PRQA S 3332 -- */                /* MISRA Rule 20.9 */
/* PRQA S 1051 -- */                /* MISRA Rule 18.8 */
/* PRQA S 3200 -- */                /* MISRA Rule 17.7 */
/* PRQA S 2016,2002 -- */           /* MISRA Rule 16.4 */
/* PRQA S 3397 -- */                /* MISRA Rule 12.1 */
/* PRQA S 1881,1842 -- */           /* MISRA Rule 10.4 */
/* PRQA S 4461,4464,0570,4424 -- */ /* MISRA Rule 10.3 */
/* PRQA S 0686 -- */                /* MISRA Rule 9.3 */
/* PRQA S 1532,1505 -- */           /* MISRA Rule 8.7 */
/* PRQA S 3206 -- */                /* MISRA Rule 2.7 */
/* PRQA S 1514 -- */                /* MISRA Rule 8.9 */
/* PRQA S 3451,3449 -- */           /* MISRA Rule 8.5 */
/* PRQA S 3408 -- */                /* MISRA Rule 8.4 */
/* PRQA S 1335,3001,3007 -- */      /* MISRA Rule 8.2 */
/* PRQA S 1503 -- */                /* MISRA Rule 2.1 */
