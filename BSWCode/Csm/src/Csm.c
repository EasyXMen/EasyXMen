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
 ***********************************************************************************************************************
 **
 **  @file               : Csm.c
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : Csm Interface Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

#include "Csm_Internal.h"

/* ===================================================== macros ===================================================== */
#define CSM_C_AR_MAJOR_VERSION 4U
#define CSM_C_AR_MINOR_VERSION 9U
#define CSM_C_AR_PATCH_VERSION 0U
#define CSM_C_SW_MAJOR_VERSION 2U
#define CSM_C_SW_MINOR_VERSION 1U
#define CSM_C_SW_PATCH_VERSION 3U

#if (                                                                                                           \
    (CSM_C_AR_MAJOR_VERSION != CSM_H_AR_MAJOR_VERSION) || (CSM_C_AR_MINOR_VERSION != CSM_H_AR_MINOR_VERSION)    \
    || (CSM_C_AR_PATCH_VERSION != CSM_H_AR_PATCH_VERSION) || (CSM_C_SW_MAJOR_VERSION != CSM_H_SW_MAJOR_VERSION) \
    || (CSM_C_SW_MINOR_VERSION != CSM_H_SW_MINOR_VERSION) || (CSM_C_SW_PATCH_VERSION != CSM_H_SW_PATCH_VERSION))

#error "Csm.c version mismatching with Csm.h"
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
/* PRQA S 3432 ++ */ /* VL_Csm_3432 */
/* PRQA S 1503 ++ */ /* VL_Csm_1503 */
/* PRQA S 1532 ++ */ /* VL_Csm_1532 */
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/**
 * @brief          Processes a job and forwards it to the Cryptographic Interface (CryIf).
 * @param[in]      jobPrimInOut : Pointer to the job's primitive input/output structure.
 * @param[in]      jobId : Identifier of the job to be processed.
 * @return         Std_ReturnType
 * @retval         E_OK: The job was successfully processed and forwarded to CryIf.
 * @retval         E_NOT_OK: The job could not be processed due to an error or the underlying CryIf service returned
 * E_NOT_OK.
 * @reentrant      Reentrant but not for the same job instance
 * @synchronous    TRUE
 * @trace       CPD-71229
 */
static Std_ReturnType Csm_ProcessJobToCryIf(Crypto_JobPrimitiveInputOutputType* jobPrimInOut, uint32 jobId);

/**
 * @brief          Enqueues a job into the specified queue.
 * @param[in]      jobId : Identifier of the job to be enqueued.
 * @param[in]      queId : Identifier of the queue where the job will be enqueued.
 * @param[in]      jobPriInOutInfo : Job's primitive input/output information.
 * @return         Std_ReturnType
 * @retval         E_OK: The job was successfully enqueued.
 * @retval         CRYPTO_E_BUSY: The queue is full and the job could not be enqueued.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71230
 */
static Std_ReturnType
    Csm_EnqueueJobToQue(uint32 jobId, uint32 queId, Crypto_JobPrimitiveInputOutputType jobPriInOutInfo);

/**
 * @brief          Dequeues a job from the specified queue.
 * @param[in]      jobId : Identifier of the job to be dequeued.
 * @param[in]      queId : Identifier of the queue from which the job will be dequeued.
 * @return         Std_ReturnType
 * @retval         E_OK: The job was successfully dequeued.
 * @retval         E_NOT_OK: The job could not be dequeued (e.g., not found in the queue).
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71231
 */
static Std_ReturnType Csm_DequeueJobInQue(uint32 jobId, uint32 queId);

/* PRQA S 5016 ++ */ /* VL_Csm_5016 */
/**
 * @brief          Finds an idle queue buffer and marks it as used.
 * @param[out]     bufPos : Pointer to store the position of the found idle buffer.
 * @return         Csm_JobInQueuePtrType
 * @retval         Pointer to the found idle queue buffer, or NULL_PTR if no idle buffer is found.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71232
 */
static Csm_JobInQueuePtrType Csm_FindIdleQueue(uint32* bufPos);
/* PRQA S 5016 -- */

/**
 * @brief          Checks if the given operation mode is valid.
 * @param[in]      mode : Operation mode to be checked.
 * @return         Std_ReturnType
 * @retval         E_OK: The operation mode is valid.
 * @retval         E_NOT_OK: The operation mode is invalid.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71233
 */
static Std_ReturnType Csm_CheckOperaMode(Crypto_OperationModeType mode);
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

/* ============================================ internal data definitions =========================================== */
#define CSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"
/**
 * @brief Buffer to store jobs in the job queue
 * @range CSM_QUEUE_TOTAL_SIZE
 */
static Csm_JobInQueueType Csm_JobQueBuf[CSM_QUEUE_TOTAL_SIZE]; /* PRQA S 3218 */ /* VL_Csm_3218 */
/**
 * @brief Status array for queue channels
 * @range CSM_QUEUE_NUM
 */
static Csm_QueueChStatusType Csm_QueueChRunStatus[CSM_QUEUE_NUM];

#define CSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"

#define CSM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"

/**
 * @brief Array to record the usage status of Csm_JobQueBuf
 * @range CSM_QUEUE_TOTAL_SIZE
 */
static boolean Csm_JobQueBufUsed[CSM_QUEUE_TOTAL_SIZE]; /* PRQA S 1514 */ /* VL_Csm_1514 */
#define CSM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"

/* ============================================ external data definitions =========================================== */
/*Module initialize status, TRUE initialized, FALSE not initialized*/
#define CSM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"
/**
 * @brief Indicates whether the CSM module has been initialized.
 * @range 0-1
 */
boolean Csm_InitStatus; /* PRQA S 1514 */ /* VL_Csm_1514 */

/**
 * @brief Flags to indicate whether a job can be postponed.
 *        Each element corresponds to a job, and is used to manage job postponement.
 * @range 0-1
 */
boolean Csm_JobPostponeFlag[CSM_JOB_NUM];
#define CSM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"

#define CSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"
/**
 * @brief Array to store the states of jobs.
 *        Each element represents the state of a corresponding job,
 *        used for managing and tracking job execution.
 * @range 0-1
 */
Crypto_JobStateType Csm_JobStates[CSM_JOB_NUM];
#define CSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"

/* ========================================== external function definitions ========================================= */
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief the CSM module.
 */
/*************************************************************************/
void Csm_Init(const Csm_ConfigType* configPtr)
{
    uint32 jobIdx;
    uint32 queIdx;
#if (CSM_DEV_ERROR_DETECT == STD_ON)
    boolean chkRes;
    chkRes = Csm_ChkInitParam(configPtr);
    if (chkRes)
#else
    /*Avoid warning.*/
    (void)configPtr;
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*Initial job states*/
        for (jobIdx = 0u; jobIdx < CSM_JOB_NUM; jobIdx++)
        {
            Csm_JobStates[jobIdx]       = CRYPTO_JOBSTATE_IDLE;
            Csm_JobPostponeFlag[jobIdx] = (boolean)FALSE;
        }
        /*Init job-queue*/
        for (queIdx = 0u; queIdx < CSM_QUEUE_NUM; queIdx++) /* PRQA S 2877 */ /* VL_Csm_2877 */
        {
            Csm_QueueChRunStatus[queIdx].pendingJobQue = NULL_PTR;
            Csm_QueueChRunStatus[queIdx].jobCnt        = 0u;
            Csm_QueueChRunStatus[queIdx].maxPriority   = CSM_INVALID_PRIORITY;
            Csm_QueueChRunStatus[queIdx].dealSynJobIng = (boolean)FALSE;
        }
        SchM_Enter_Csm_Area_Csm();
        for (queIdx = 0u; queIdx < CSM_QUEUE_TOTAL_SIZE; queIdx++)
        {
            Csm_JobQueBufUsed[queIdx] = (boolean)FALSE;
        }
        SchM_Exit_Csm_Area_Csm();
        Csm_InitStatus = (boolean)TRUE;
    }
}

#if (CSM_GET_VERSION_INFO == STD_ON)
/*************************************************************************/
/**
 * @brief Returns the version information of this module.
 */
/*************************************************************************/
void Csm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    Std_ReturnType ret = E_OK;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkGetVersionInfo(versioninfo, CSM_SID_GETVERSIONINFO);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        versioninfo->moduleID         = CSM_MODULE_ID;
        versioninfo->vendorID         = CSM_VENDOR_ID;
        versioninfo->sw_major_version = CSM_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CSM_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CSM_H_SW_PATCH_VERSION;
    }
}
#endif /*CSM_GET_VERSION_INFO == STD_ON*/

#if (CSM_HASH_SUPPORT == STD_ON)
/*************************************************************************/
/**
 * @brief  Uses the given data to perform the hash calculation and stores the hash.
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_Hash(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   resultPtr,
    uint32*                  resultLengthPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType hashInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_HASH);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            hashInfo.mode            = mode;
            hashInfo.inputPtr        = dataPtr;
            hashInfo.inputLength     = dataLength;
            hashInfo.outputPtr       = resultPtr;
            hashInfo.outputLengthPtr = resultLengthPtr;
            ret                      = Csm_ProcessJobToCryIf(&hashInfo, jobId);
        }
    }
    return ret;
}
#endif /*CSM_HASH_SUPPORT == STD_ON*/

#if (CSM_MAC_SUPPORT == STD_ON)
/*************************************************************************/
/**
 * @brief  Uses the given data to perform a MAC generation and stores the MAC in the memory
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_MacGenerate(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   macPtr,
    uint32*                  macLengthPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType macInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_MAC_GEN);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            macInfo.mode            = mode;
            macInfo.inputPtr        = dataPtr;
            macInfo.inputLength     = dataLength;
            macInfo.outputPtr       = macPtr;
            macInfo.outputLengthPtr = macLengthPtr;
            ret                     = Csm_ProcessJobToCryIf(&macInfo, jobId);
        }
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Verifies the given MAC by comparing if the MAC is generated with the given data.
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_MacVerify(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    const uint8*             macPtr,
    uint32                   macLength,
    Crypto_VerifyResultType* verifyPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType macInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_MAC_VERIFY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            macInfo.mode                 = mode;
            macInfo.inputPtr             = dataPtr;
            macInfo.inputLength          = dataLength;
            macInfo.secondaryInputPtr    = macPtr;
            macInfo.secondaryInputLength = macLength / CSM_CONST_8L;
            macInfo.verifyPtr            = verifyPtr;
            ret                          = Csm_ProcessJobToCryIf(&macInfo, jobId);
        }
    }
    return ret;
}
#endif /*CSM_MAC_SUPPORT == STD_ON*/

#if (CSM_ENCRYPT_SUPPORT == STD_ON)
/*************************************************************************/
/**
 * @brief the given data and store the ciphertext in the memory location pointed by
 *        the result pointer.
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_Encrypt(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   resultPtr,
    uint32*                  resultLengthPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType encryptInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_ENCRYPT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            encryptInfo.mode            = mode;
            encryptInfo.inputPtr        = dataPtr;
            encryptInfo.inputLength     = dataLength;
            encryptInfo.outputPtr       = resultPtr;
            encryptInfo.outputLengthPtr = resultLengthPtr;
            ret                         = Csm_ProcessJobToCryIf(&encryptInfo, jobId);
        }
    }

    return ret;
}
/*************************************************************************/
/**
 * @brief the given encrypted data and store the decrypted plaintext in the memory
 *        location pointed by the result pointer.
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_Decrypt(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   resultPtr,
    uint32*                  resultLengthPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType decryptInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_DECRYPT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            decryptInfo.mode            = mode;
            decryptInfo.inputPtr        = dataPtr;
            decryptInfo.inputLength     = dataLength;
            decryptInfo.outputPtr       = resultPtr;
            decryptInfo.outputLengthPtr = resultLengthPtr;
            ret                         = Csm_ProcessJobToCryIf(&decryptInfo, jobId);
        }
    }

    return ret;
}
#endif /*CSM_ENCRYPT_SUPPORT == STD_ON*/
#if (CSM_AEADENCRYPT_SUPPORT == STD_ON)
/*************************************************************************/
/**
 * @brief the given input data to perform a AEAD encryption and stores the ciphertext and the MAC
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_AEADEncrypt(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             plaintextPtr,
    uint32                   plaintextLength,
    const uint8*             associatedDataPtr,
    uint32                   associatedDataLength,
    uint8*                   ciphertextPtr,
    uint32*                  ciphertextLengthPtr,
    uint8*                   tagPtr,
    uint32*                  tagLengthPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType AEADEncryptInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_AEADENCRYPT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            AEADEncryptInfo.mode                     = mode;
            AEADEncryptInfo.inputPtr                 = plaintextPtr;
            AEADEncryptInfo.inputLength              = plaintextLength;
            AEADEncryptInfo.secondaryInputPtr        = associatedDataPtr;
            AEADEncryptInfo.secondaryInputLength     = associatedDataLength;
            AEADEncryptInfo.outputPtr                = ciphertextPtr;
            AEADEncryptInfo.outputLengthPtr          = ciphertextLengthPtr;
            AEADEncryptInfo.secondaryOutputPtr       = tagPtr;
            AEADEncryptInfo.secondaryOutputLengthPtr = tagLengthPtr;
            ret                                      = Csm_ProcessJobToCryIf(&AEADEncryptInfo, jobId);
        }
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Uses the given data to perform an AEAD Decryption and stores the ciphertext
 *        and the MAC in the memory locations pointed by the ciphertext pointer and
 *        Tag pointer.
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_AEADDecrypt(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             ciphertextPtr,
    uint32                   ciphertextLength,
    const uint8*             associatedDataPtr,
    uint32                   associatedDataLength,
    const uint8*             tagPtr,
    uint32                   tagLength,
    uint8*                   plaintextPtr,
    uint32*                  plaintextLengthPtr,
    Crypto_VerifyResultType* verifyPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType AEADDecryptInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_AEADDECRYPT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            AEADDecryptInfo.mode                 = mode;
            AEADDecryptInfo.inputPtr             = ciphertextPtr;
            AEADDecryptInfo.inputLength          = ciphertextLength;
            AEADDecryptInfo.secondaryInputPtr    = associatedDataPtr;
            AEADDecryptInfo.secondaryInputLength = associatedDataLength;
            AEADDecryptInfo.tertiaryInputPtr     = tagPtr;
            AEADDecryptInfo.tertiaryInputLength  = tagLength;
            AEADDecryptInfo.outputPtr            = plaintextPtr;
            AEADDecryptInfo.outputLengthPtr      = plaintextLengthPtr;
            AEADDecryptInfo.verifyPtr            = verifyPtr;
            ret                                  = Csm_ProcessJobToCryIf(&AEADDecryptInfo, jobId);
        }
    }
    return ret;
}
#endif
#if (CSM_SIGNATURE_SUPPORT == STD_ON)
/*************************************************************************/
/**
 * @brief Uses the given data to perform the signature calculation and stores the
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_SignatureGenerate(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   resultPtr,
    uint32*                  resultLengthPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType signatureInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_SIG_GEN);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            signatureInfo.mode            = mode;
            signatureInfo.inputPtr        = dataPtr;
            signatureInfo.inputLength     = dataLength;
            signatureInfo.outputPtr       = resultPtr;
            signatureInfo.outputLengthPtr = resultLengthPtr;
            ret                           = Csm_ProcessJobToCryIf(&signatureInfo, jobId);
        }
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Verifies the given MAC by comparing if the signature is generated with the given data.
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_SignatureVerify(

    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    const uint8*             signaturePtr,
    uint32                   signatureLength,
    Crypto_VerifyResultType* verifyPtr)
/* PRQA S 1503, 6040 -- */
{
    Crypto_JobPrimitiveInputOutputType signatureInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_SIG_VERIFY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            signatureInfo.mode                 = mode;
            signatureInfo.inputPtr             = dataPtr;
            signatureInfo.inputLength          = dataLength;
            signatureInfo.secondaryInputPtr    = signaturePtr;
            signatureInfo.secondaryInputLength = signatureLength;
            signatureInfo.verifyPtr            = verifyPtr;
            ret                                = Csm_ProcessJobToCryIf(&signatureInfo, jobId);
        }
    }
    return ret;
}
#endif /*CSM_SIGNATURE_SUPPORT == STD_ON*/

#if (CSM_RANDOM_SUPPORT == STD_ON)
/*************************************************************************/
/**
 * @brief  Generate a random number and stores it in the memory location pointed by the result pointer
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType
    Csm_RandomGenerate(uint32 jobId, Crypto_OperationModeType mode, uint8* resultPtr, uint32* resultLengthPtr)
/* PRQA S 1503 -- */
{
    Crypto_JobPrimitiveInputOutputType randomInfo;
    Std_ReturnType                     ret;

    (void)mode;
#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_RANDOM_GEN);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        randomInfo.outputPtr       = resultPtr;
        randomInfo.outputLengthPtr = resultLengthPtr;
        /* SWS_Csm_01054 SWS_Csm_01055*/
        randomInfo.mode = CRYPTO_OPERATIONMODE_SINGLECALL;
        ret             = Csm_ProcessJobToCryIf(&randomInfo, jobId);
    }
    return ret;
}
#endif /*CSM_RANDOM_SUPPORT == STD_ON*/
/*************************************************************************/
/**
 * @brief  Sets the given key element bytes to the key identified by keyId.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyElementSet(uint32 keyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength)
/* PRQA S 1503 -- */
{
    uint32         cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyEleSetParam(keyId, keyPtr, keyLength, CSM_SID_KEYELEMENTSET);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret        = CryIf_KeyElementSet(cryIfKeyId, keyElementId, keyPtr, keyLength);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief the key state of the key identified by keyId to valid.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeySetValid(uint32 keyId)
/* PRQA S 1503 -- */
{
    uint32         cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeySetValid(keyId, CSM_SID_KEYSET_VALID);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret        = CryIf_KeySetValid(cryIfKeyId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief the key state of the key identified by keyId to invalid.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeySetInvalid(uint32 keyId)
/* PRQA S 1503 -- */
{
    uint32         cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeySetInValid(keyId, CSM_SID_KEYSET_INVALID);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret        = CryIf_KeySetInValid(cryIfKeyId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Retrieves the key element bytes from a specific key element of the key
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyElementGet(uint32 keyId, uint32 keyElementId, uint8* keyPtr, uint32* keyLengthPtr)
/* PRQA S 1503 -- */
{
    uint32         cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyEleGet(keyId, keyPtr, keyLengthPtr, CSM_SID_KEYELEMENTGET);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret        = CryIf_KeyElementGet(cryIfKeyId, keyElementId, keyPtr, keyLengthPtr);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  function shall copy a key elements from one key to a target key.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyElementCopy(
    const uint32 keyId,
    const uint32 keyElementId,
    const uint32 targetKeyId,
    const uint32 targetKeyElementId)
/* PRQA S 1503 -- */
{
    uint32         cryIfSrcKeyId;
    uint32         cryIfDestKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyEleCopy(keyId, targetKeyId, CSM_SID_KEYELEMENTCPY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfSrcKeyId  = Csm_KeyCfg[keyId].cryIfKey;
        cryIfDestKeyId = Csm_KeyCfg[targetKeyId].cryIfKey;
        ret            = CryIf_KeyElementCopy(cryIfSrcKeyId, keyElementId, cryIfDestKeyId, targetKeyElementId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief This function shall copy all key elements from the source key to a target key.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyCopy(const uint32 keyId, const uint32 targetKeyId)
/* PRQA S 1503 -- */
{
    uint32         cryIfSrcKeyId;
    uint32         cryIfDestKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyCopy(keyId, targetKeyId, CSM_SID_KEYCPY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfSrcKeyId  = Csm_KeyCfg[keyId].cryIfKey;
        cryIfDestKeyId = Csm_KeyCfg[targetKeyId].cryIfKey;
        ret            = CryIf_KeyCopy(cryIfSrcKeyId, cryIfDestKeyId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Copies a key element to another key element in the same crypto driver. The keyElementSourceOffset
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_KeyElementCopyPartial(
    uint32 keyId,
    uint32 keyElementId,
    uint32 keyElementSourceOffset,
    uint32 keyElementTargetOffset,
    uint32 keyElementCopyLength,
    uint32 targetKeyId,
    uint32 targetKeyElementId)
/* PRQA S 1503, 6040 -- */
{
    uint32         cryIfSrcKeyId;
    uint32         cryIfDestKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyEleCopyPart(keyId, targetKeyId, CSM_SID_KEYELEMENTCPYPARTIAL);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfSrcKeyId  = Csm_KeyCfg[keyId].cryIfKey;
        cryIfDestKeyId = Csm_KeyCfg[targetKeyId].cryIfKey;
        ret            = CryIf_KeyElementCopyPartial(
            cryIfSrcKeyId,
            keyElementId,
            keyElementSourceOffset,
            keyElementTargetOffset,
            keyElementCopyLength,
            cryIfDestKeyId,
            targetKeyElementId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Feeds the key element CRYPTO_KE_RANDOM_SEED with a random seed.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_RandomSeed(uint32 keyId, const uint8* seedPtr, uint32 seedLength)
/* PRQA S 1503 -- */
{
    uint32         cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkRandomSeed(keyId, seedPtr, seedLength, CSM_SID_RANDOMSEED);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret        = CryIf_RandomSeed(cryIfKeyId, seedPtr, seedLength);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Generates new key material and store it in the key identified by keyId.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyGenerate(uint32 keyId)
{
    uint32         cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyGenerate(keyId, CSM_SID_KEY_GENERATE);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret        = CryIf_KeyGenerate(cryIfKeyId);
    }
    return ret;
}
/* PRQA S 1503 -- */
/*************************************************************************/
/**
 * @brief  Derives a new key by using the key elements in the given key identified by the
 * keyId. The given key contains the key elements for the password and salt. The
 * derived key is stored in the key element with the id 1 of the key identified
 * by targetCryptoKeyId.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyDerive(uint32 keyId, uint32 targetKeyId)
/* PRQA S 1503 -- */
{
    uint32         cryIfSrcKeyId;
    uint32         cryIfDestKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyDerive(keyId, targetKeyId, CSM_SID_KEY_DERIVE);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfSrcKeyId  = Csm_KeyCfg[keyId].cryIfKey;
        cryIfDestKeyId = Csm_KeyCfg[targetKeyId].cryIfKey;
        ret            = CryIf_KeyDerive(cryIfSrcKeyId, cryIfDestKeyId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Calculates the public value of the current user for the key exchange and stores
 * the public key in the memory location pointed by the public value pointer.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyExchangeCalcPubVal(uint32 keyId, uint8* publicValuePtr, uint32* publicValueLengthPtr)
/* PRQA S 1503 -- */
{
    uint32         cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    if (keyId >= CSM_KEY_NUM)
    {
        ret = E_NOT_OK;
        Csm_ReportDetErr(CSM_SID_KEY_EX_PUB, CSM_E_PARAM_HANDLE);
    }
    else
    {
        ret = Csm_ChkExCalcPubVal(publicValuePtr, publicValueLengthPtr, CSM_SID_KEY_EX_PUB);
    }

    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret        = CryIf_KeyExchangeCalcPubVal(cryIfKeyId, publicValuePtr, publicValueLengthPtr);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Calculates the shared secret key for the key exchange with the key material of
 * the key identified by the keyId and the partner public key. The shared secret
 * key is stored as a key element in the same key.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType
    Csm_KeyExchangeCalcSecret(uint32 keyId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength)
/* PRQA S 1503 -- */
{
    uint32         cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    if (keyId >= CSM_KEY_NUM)
    {
        ret = E_NOT_OK;
        Csm_ReportDetErr(CSM_SID_KEY_EX_SEC, CSM_E_PARAM_HANDLE);
    }
    else
    {
        ret = Csm_ChkExCalcSecVal(partnerPublicValuePtr, partnerPublicValueLength, CSM_SID_KEY_EX_SEC);
    }
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret        = CryIf_KeyExchangeCalcSecret(cryIfKeyId, partnerPublicValuePtr, partnerPublicValueLength);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Stores the key if necessary and sets the key state of the key identified by keyId to valid.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobKeySetValid(uint32 jobId)
/* PRQA S 1503 -- */
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeySetValid(jobId, CSM_SID_JOBKEYSETVALID);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_JobCfg[jobId].jobPrimitiveInfo->cryIfKeyId;
        ret                = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Stores the key if necessary and sets the key state of the key identified by keyId to valid.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobKeySetInValid(uint32 jobId)
/* PRQA S 1503 -- */
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeySetInValid(jobId, CSM_SID_JOBKEYSETINVALID);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_JobCfg[jobId].jobPrimitiveInfo->cryIfKeyId;
        ret                = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  This function shall dispatch the random seed function to the configured crypto driver object.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobRandomSeed(uint32 jobId, uint32 keyId, const uint8* seedPtr, uint32 seedLength)
/* PRQA S 1503 -- */
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobRandomSeed(jobId, keyId, seedPtr, seedLength, CSM_SID_JOBRANDOMSEED);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId  = Csm_KeyCfg[keyId].cryIfKey;
        jobInfo.inputPtr    = seedPtr;
        jobInfo.inputLength = seedLength;
        ret                 = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Generates new key material and stores it in the key identified by keyId.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobKeyGenerate(uint32 jobId, uint32 keyId)
/* PRQA S 1503 -- */
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeyGenerate(jobId, keyId, CSM_SID_JOBKEYGENERATE);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret                = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Derives a new key by using the key elements in the given key identified by the keyId.
 * The given key contains the key elements for the password and salt. The derived key is
 * stored in the key element with the id 1 of the key identified by targetCryptoKeyId.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobKeyDerive(uint32 jobId, uint32 keyId, uint32 targetKeyId)
/* PRQA S 1503 -- */
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeyDerive(jobId, keyId, targetKeyId, CSM_SID_JOBKEYDERIVE);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId       = Csm_KeyCfg[keyId].cryIfKey;
        jobInfo.targetCryIfKeyId = targetKeyId;
        ret                      = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Calculates the public value of the current user for the key exchange and stores the public key
 * in the memory location pointed by the public value pointer.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobKeyExchangeCalcPubVal(
    uint32 jobId,
    /* PRQA S 3673 ++ */ /* VL_QAC_3673 */
    uint8* publicValuePtr,
    /* PRQA S 3673 -- */
    uint32* publicValueLengthPtr)
/* PRQA S 1503 -- */
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobExCalcPubVal(jobId, publicValuePtr, publicValueLengthPtr, CSM_SID_JOBKEY_EX_PUB);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId               = Csm_JobCfg[jobId].jobPrimitiveInfo->cryIfKeyId;
        jobInfo.secondaryInputPtr        = publicValuePtr;
        jobInfo.secondaryInputLength     = *publicValuePtr;
        jobInfo.secondaryOutputLengthPtr = publicValueLengthPtr;
        ret                              = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Calculates the shared secret key for the key exchange with the key material of the key
 * identified by the keyId and the partner public key. The shared secret key is stored as
 * a key element in the same key.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType
    Csm_JobKeyExchangeCalcSecret(uint32 jobId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength)
/* PRQA S 1503 -- */
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobExCalcSec(jobId, partnerPublicValuePtr, partnerPublicValueLength, CSM_SID_JOBKEY_EX_SEC);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId  = Csm_JobCfg[jobId].jobPrimitiveInfo->cryIfKeyId;
        jobInfo.inputPtr    = partnerPublicValuePtr;
        jobInfo.inputLength = partnerPublicValueLength;
        ret                 = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  Cancels the job processing from asynchronous or streaming jobs.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_CancelJob(uint32 job, Crypto_OperationModeType mode)
/* PRQA S 1503 -- */
{
    const Csm_JobsCfgType* jobCfgPtr;
    Crypto_JobType         jobInfo;
    Std_ReturnType         ret;
    uint32                 cryIfChId;
    uint32                 jobQueId;
    uint32                 cbkId;

/* PRQA S 2982 ++ */ /* VL_Csm_UsedBeforModify */
#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkCancelJob(job, CSM_SID_CANCELJOB);
    if (E_OK == ret)
#else
    ret = E_NOT_OK;
/* PRQA S 2982 -- */
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        /* Not used, just for interface compatibility provided.
         * Avoid warning
         */
        (void)(mode);
        SchM_Enter_Csm_Area_Csm();
        jobCfgPtr           = &(Csm_JobCfg[job]);
        jobQueId            = jobCfgPtr->queRef;
        cryIfChId           = Csm_QueCfg[jobQueId].cryIfChRef;
        cbkId               = jobCfgPtr->jobPrimitiveInfo->callbackId;
        jobInfo.jobId       = jobCfgPtr->jobId;
        jobInfo.jobPriority = jobCfgPtr->jobPriority;
        /*[SWS_Csm_01021]*/
        ret = CryIf_CancelJob(cryIfChId, &jobInfo);
        if (E_OK == ret)
        {
#if (CSM_CBKFUNCS_NUM > 0u)
            if (Csm_CbkCfg[cbkId].cbkFnc != NULL_PTR)
            {
                /*[SWS_Csm_01030]*/
                Csm_CbkCfg[cbkId].cbkFnc(&jobInfo, CRYPTO_E_JOB_CANCELED);
            }
#else
            (void)(cbkId);
#endif /*CSM_CBKFUNCS_NUM > 0u*/
        }
        else if (CRYPTO_E_JOB_CANCELED == ret)
        {
            /*[SWS_Csm_01087]*/
            Csm_JobPostponeFlag[job] = (boolean)TRUE;
        }
        else
        {
            /*Do nothing*/
        }
        /*Remove the job from its own queue*/
        (void)Csm_DequeueJobInQue(job, jobQueId);
        SchM_Exit_Csm_Area_Csm();
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Retrieves the status of a key in the CSM module.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyGetStatus(uint32 keyId, Crypto_KeyStatusType* keyStatusPtr)
/* PRQA S 1503 -- */
{
    Std_ReturnType ret = E_OK;
#if (CSM_DEV_ERROR_DETECT == STD_ON)
    if (keyId >= CSM_KEY_NUM)
    {
        ret = E_NOT_OK;
        Csm_ReportDetErr(CSM_SID_KEY_GETSTATUS, CSM_E_PARAM_HANDLE);
    }
    else if (keyStatusPtr == NULL_PTR)
    {
        ret = E_NOT_OK;
        Csm_ReportDetErr(CSM_SID_KEY_GETSTATUS, CSM_E_PARAM_POINTER);
    }
    else
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = CryIf_KeyGetStatus(keyId, keyStatusPtr);
    }

    return ret;
}
/*************************************************************************/
/**
 * @brief Custom service operation for the CSM module.
 */
/*************************************************************************/
/* PRQA S 1503,6040,3673 ++ */ /* VL_QAC_NoUsedApi,VL_MTR_Csm_STPAR,VL_QAC_3673 */
Std_ReturnType Csm_CustomService(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    uint32                   targetKeyId,
    const uint8*             inputPtr,
    uint32                   inputLength,
    const uint8*             secondaryInputPtr,
    uint32                   secondaryInputLength,
    const uint8*             tertiaryInputPtr,
    uint32                   tertiaryInputLength,
    uint8*                   outputPtr,
    uint32*                  outputLengthPtr,
    uint8*                   secondaryOutputPtr,
    uint32*                  secondaryOutputLengthPtr,
    Crypto_VerifyResultType* verifyPtr)
/* PRQA S 1503,6040,3673 -- */
{
    (void)jobId;
    (void)mode;
    (void)targetKeyId;
    (void)inputPtr;
    (void)inputLength;
    (void)secondaryInputPtr;
    (void)secondaryInputLength;
    (void)tertiaryInputPtr;
    (void)tertiaryInputLength;
    (void)outputPtr;
    (void)outputLengthPtr;
    (void)secondaryOutputPtr;
    (void)secondaryOutputLengthPtr;
    (void)verifyPtr;
    return E_OK;
}
/*************************************************************************/
/**
 * @brief Custom synchronous operation for the CSM module.
 */
/*************************************************************************/
/* PRQA S 1503, 6040 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STPAR */
Std_ReturnType Csm_CustomSync(
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
/* PRQA S 1503, 6040 -- */
{
    Std_ReturnType ret;
    ret = CryIf_CustomSync(
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
    return ret;
}
/*************************************************************************/
/**
 * @brief  API to be called cyclically to process the requested jobs. The Csm_MainFunction
 * shall check the queues for jobs to pass to the underlying CRYIF.
 */
/*************************************************************************/
/* PRQA S 3408 ++ */ /* VL_Csm_3408 */
void Csm_MainFunction(void)
/* PRQA S 3408 -- */
{
    /* PRQA S 3678 ++ */ /* VL_Csm_3678 */
    Csm_QueueChStatusType* queChRunPtr;
    /* PRQA S 3678 -- */
    const Csm_JobsCfgType* jobCfgPtr;
    Crypto_JobType         jobInfo;
    uint32                 queIdx;
    uint32                 cryIfChId;
    uint32                 jobInQueId;
    Std_ReturnType         ret;

    /*Deal asynchronous job in queue*/
    for (queIdx = 0u; queIdx < CSM_QUEUE_NUM; queIdx++) /* PRQA S 2877 */ /* VL_Csm_2877 */
    {
        queChRunPtr = &(Csm_QueueChRunStatus[queIdx]);
        if ((queChRunPtr->dealSynJobIng) || (0uL == queChRunPtr->jobCnt))
        {
            /*There is a job being executed in queue, the CSM shall disable
             * processing new jobs
             * Ref:[SWS_Csm_00037]*/
            continue; /* PRQA S 0770 */ /* VL_Csm_0770 */
        }
        cryIfChId                     = Csm_QueCfg[queIdx].cryIfChRef;
        jobInQueId                    = queChRunPtr->pendingJobQue->jobId;
        jobCfgPtr                     = &(Csm_JobCfg[jobInQueId]);
        jobInfo.jobRedirectionInfoRef = jobCfgPtr->inOutRedirtRef;
        jobInfo.jobPriority           = jobCfgPtr->jobPriority;
        jobInfo.jobPrimitiveInfo      = jobCfgPtr->jobPrimitiveInfo;
        jobInfo.jobId                 = jobInQueId;
        SchM_Enter_Csm_Area_Csm();
        jobInfo.jobState = Csm_JobStates[jobInQueId];
        SchM_Exit_Csm_Area_Csm();
        jobInfo.jobPrimitiveInputOutput = queChRunPtr->pendingJobQue->jobPriInOut;
        ret                             = CryIf_ProcessJob(cryIfChId, &jobInfo);
        if (E_OK == ret)
        {
            SchM_Enter_Csm_Area_Csm();
            Csm_JobStates[jobInQueId] = CRYPTO_JOBSTATE_ACTIVE;
            SchM_Exit_Csm_Area_Csm();
            /*Delete the job from queue*/
            (void)Csm_DequeueJobInQue(jobInQueId, queIdx);
        }
    }
}
/*************************************************************************/
/**
 * @brief Saves the context of a job in the CSM module.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_SaveContextJob(uint32 jobId, uint8* contextBufferPtr, uint32* contextBufferLengthPtr)
/* PRQA S 1503 -- */
{
    Std_ReturnType retVal = E_OK;
    (void)jobId;
    *contextBufferPtr       = 0u;
    *contextBufferLengthPtr = 0u;

    return retVal;
}
/*************************************************************************/
/**
 * @brief Restores the context of a job in the CSM module.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_RestoreContextJob(uint32 jobId, const uint8* contextBufferPtr, uint32 contextBufferLength)
/* PRQA S 1503 -- */
{
    Std_ReturnType retVal = E_OK;
    (void)jobId;
    (void)contextBufferPtr;
    (void)contextBufferLength;
    return retVal;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief Call CryIf_ProcessJob,Transmit (Crypto_JobType) job to CryIf
 */
/*************************************************************************/
/* PRQA S 1503, 6010, 6030, 5017 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STCYC, VL_MTR_Csm_STMIF, VL_Csm_5017 */
/* PRQA S 3673 ++ */                   /* VL_Csm_3673 */
static Std_ReturnType Csm_ProcessJobToCryIf(Crypto_JobPrimitiveInputOutputType* jobPrimInOut, uint32 jobId)
/* PRQA S 3673 -- */
/* PRQA S 1503, 6010, 6030, 5017 -- */
{
    const Csm_JobsCfgType* jobCfgPtr;
    Csm_QueueChStatusType* queChRunPtr;
    uint32                 queueRef;
    uint32                 channelId;
    Crypto_JobStateType    jobState;
    Crypto_ProcessingType  processMode;
    Std_ReturnType         ret = E_OK;
    Crypto_JobType         job;

    jobCfgPtr = &(Csm_JobCfg[jobId]);
    SchM_Enter_Csm_Area_Csm();
    jobState = Csm_JobStates[jobId];
    SchM_Exit_Csm_Area_Csm();
    if ((jobCfgPtr->inOutRedirtRef != NULL_PTR)
        && ((jobPrimInOut->inputLength != 0uL) || (*(jobPrimInOut->outputLengthPtr) != 0uL)))
    {
        /*[SWS_Csm_91014]*/
        ret = E_NOT_OK;
    }
    else
    {
        job.jobRedirectionInfoRef   = jobCfgPtr->inOutRedirtRef;
        job.jobPriority             = jobCfgPtr->jobPriority;
        job.jobPrimitiveInfo        = jobCfgPtr->jobPrimitiveInfo;
        job.jobId                   = jobId;
        job.jobState                = jobState;
        job.jobPrimitiveInputOutput = *jobPrimInOut;
        /*Job belong to queue*/
        queueRef    = jobCfgPtr->queRef;
        channelId   = Csm_QueCfg[queueRef].cryIfChRef;
        processMode = jobCfgPtr->jobPrimitiveInfo->processingType;
        if (CRYPTO_JOBSTATE_ACTIVE == jobState)
        {
            ret = CryIf_ProcessJob(channelId, &job);
            if (CRYPTO_PROCESSING_SYNC == processMode)
            {
                /*1.CryIf_ProcessJob return negative response*/
                SchM_Enter_Csm_Area_Csm();
                if (ret != E_OK)
                {
                    Csm_JobStates[jobId] = CRYPTO_JOBSTATE_IDLE;
                }
                /*2.Streaming approach:Start->Update->Finish or single call finish*/
                /* PRQA S 1317,4522,1823 ++ */ /* VL_Csm_1317,VL_Csm_4522,VL_Csm_1823 */
                else if ((jobPrimInOut->mode & CRYPTO_OPERATIONMODE_FINISH) != 0u)
                /* PRQA S 1317,4522,1823 -- */
                {
                    Csm_JobStates[jobId] = CRYPTO_JOBSTATE_IDLE;
                }
                else
                {
                    /*Do nothing*/
                }
                SchM_Exit_Csm_Area_Csm();
            }
        }
        else /*Job in CRYPTO_JOBSTATE_IDLE state*/
        {
            queChRunPtr = &(Csm_QueueChRunStatus[queueRef]);
            if (CRYPTO_PROCESSING_SYNC == processMode)
            {
                /*Is the job priority higher than the highest in the queue?*/
                if (((jobCfgPtr->jobPriority > queChRunPtr->maxPriority)) || (0uL == queChRunPtr->jobCnt))
                {
                    queChRunPtr->dealSynJobIng = (boolean)TRUE;
                    ret                        = CryIf_ProcessJob(channelId, &job);
                    /* PRQA S 1317,4522,1823 ++ */ /* VL_Csm_1317,VL_Csm_4522,VL_Csm_1823 */
                    if ((E_OK == ret) && (0u == (jobPrimInOut->mode & CRYPTO_OPERATIONMODE_FINISH)))
                    /* PRQA S 1317,4522,1823 -- */
                    {
                        /*The job has not been completed yet*/
                        SchM_Enter_Csm_Area_Csm();
                        Csm_JobStates[jobId] = CRYPTO_JOBSTATE_ACTIVE;
                        SchM_Exit_Csm_Area_Csm();
                    }
                    queChRunPtr->dealSynJobIng = (boolean)FALSE;
                }
                else
                {
                    /*Ref:[SWS_Csm_91007]*/
                    ret = CRYPTO_E_BUSY;
                }
            }
            else /*Asny*/
            {
                /*Csm queue empty?*/
                if (0uL == queChRunPtr->jobCnt)
                {
                    /*process job immediately*/
                    ret = CryIf_ProcessJob(channelId, &job);
                    if (E_OK == ret)
                    {
                        SchM_Enter_Csm_Area_Csm();
                        Csm_JobStates[jobId] = CRYPTO_JOBSTATE_ACTIVE;
                        SchM_Exit_Csm_Area_Csm();
                    }
                    else if (CRYPTO_E_BUSY == ret)
                    {
                        /*Enqueue job in CSM queue*/
                        (void)Csm_EnqueueJobToQue(jobId, queueRef, *jobPrimInOut);
                    }
                    else
                    {
                        /*Do nothing*/
                    }
                }
                else
                {
                    /*Judge whether the queue is full*/
                    if (Csm_QueCfg[queueRef].queueSize > queChRunPtr->jobCnt)
                    {
                        /*Have enough space,enqueue job in CSM queue*/
                        (void)Csm_EnqueueJobToQue(jobId, queueRef, *jobPrimInOut);
                    }
                    else
                    {
                        ret = CRYPTO_E_BUSY;
#if (STD_ON == CSM_DEV_ERROR_DETECT)
                        Csm_ReportRuntimeErr(CSM_SID_CANCELJOB, CSM_E_QUEUE_FULL);
#endif
                    }
                }
            }
        }
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Enqueue job to CSM queue
 */
/*************************************************************************/
/* PRQA S 1503, 6010, 6030, 5017 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STCYC, VL_MTR_Csm_STMIF, VL_Csm_5017 */
static Std_ReturnType
    Csm_EnqueueJobToQue(uint32 jobId, uint32 queId, Crypto_JobPrimitiveInputOutputType jobPriInOutInfo)
/* PRQA S 1503, 6010, 6030, 5017 -- */
{
    Csm_QueueChStatusType* queChRunPtr;
    const Csm_JobsCfgType* jobCfgPtr;
    Csm_JobInQueueType*    queBuf;
    Csm_JobInQueueType*    swapqueBuf;
    Csm_JobInQueueType*    preNode = NULL_PTR;
    Csm_JobInQueuePtrType* curNode;
    Csm_JobInQueueType*    pendingNode;
    uint32                 bufPos;
    uint32                 itemPriority;
    uint32                 idx;
    Std_ReturnType         ret        = E_OK;
    boolean                break_Flag = FALSE;

    queChRunPtr  = &(Csm_QueueChRunStatus[queId]);
    jobCfgPtr    = &(Csm_JobCfg[jobId]);
    itemPriority = jobCfgPtr->jobPriority;
    curNode      = &(queChRunPtr->pendingJobQue);
    pendingNode  = queChRunPtr->pendingJobQue;
    /*Update the max priority in queue*/
    if ((queChRunPtr->maxPriority == CSM_INVALID_PRIORITY) || (itemPriority > queChRunPtr->maxPriority))
    {
        queChRunPtr->maxPriority = itemPriority;
    }
    if (0uL == queChRunPtr->jobCnt)
    {
        /*Queue is empty*/
        (*curNode) = Csm_FindIdleQueue(&bufPos);
        if ((*curNode) != NULL_PTR)
        {
            (*curNode)->jobId       = jobId;
            (*curNode)->jobPriority = itemPriority;
            (*curNode)->next        = NULL_PTR;
            (*curNode)->bufPos      = bufPos;
            (*curNode)->jobPriInOut = jobPriInOutInfo;
            queChRunPtr->jobCnt++;
        }
    }
    else
    {
        if (queChRunPtr->jobCnt < Csm_QueCfg[queId].queueSize)
        {
            /*Find idle buffer to save  node*/
            queBuf = Csm_FindIdleQueue(&bufPos);
            if (NULL_PTR == queBuf)
            {
                ret = CRYPTO_E_BUSY;
            }
            else
            {
                /*Insert node to Linklist by priority*/
                queBuf->bufPos      = bufPos;
                queBuf->jobId       = jobId;
                queBuf->jobPriority = itemPriority;
                queBuf->jobPriInOut = jobPriInOutInfo;
                for (idx = 0u; idx < queChRunPtr->jobCnt; idx++)
                {
                    /*[SWS_Csm_00944]*/
                    if ((itemPriority > (*curNode)->jobPriority))
                    {
                        swapqueBuf       = (*curNode);
                        (*curNode)       = queBuf;
                        (*curNode)->next = swapqueBuf;
                        if (itemPriority > pendingNode->jobPriority)
                        {
                            pendingNode = queBuf;
                        }
                        if (preNode != NULL_PTR)
                        {
                            preNode->next = (*curNode);
                        }
                        break_Flag = TRUE;
                    }
                    else
                    {
                        if (NULL_PTR == (*curNode)->next)
                        {
                            queBuf->next     = (*curNode)->next;
                            (*curNode)->next = queBuf;
                            break_Flag       = TRUE;
                        }
                        else
                        {
                            preNode    = (*curNode);
                            (*curNode) = (*curNode)->next;
                        }
                    }
                    if (break_Flag)
                    {
                        break;
                    }
                }
                (*curNode) = pendingNode;
                queChRunPtr->jobCnt++;
            }
        }
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Dequeue job to CSM queue
 */
/*************************************************************************/
static Std_ReturnType Csm_DequeueJobInQue(uint32 jobId, uint32 queId)
{
    Csm_QueueChStatusType* queChRunPtr;
    Csm_JobInQueuePtrType* curNode;
    /* PRQA S 3678 ++ */ /* VL_Csm_3678 */
    Csm_JobInQueuePtrType* preNode;
    Csm_JobInQueueType*    temNode;
    /* PRQA S 3678 -- */
    uint32         idx;
    Std_ReturnType ret = E_NOT_OK;

    queChRunPtr = &(Csm_QueueChRunStatus[queId]);
    curNode     = &(queChRunPtr->pendingJobQue);
    if ((*curNode)->jobId == jobId)
    {
        /*Update the max priority in queue*/
        if ((*curNode)->next != NULL_PTR)
        {
            queChRunPtr->maxPriority = (*curNode)->next->jobPriority;
        }
        else
        {
            queChRunPtr->maxPriority = CSM_INVALID_PRIORITY;
        }
        SchM_Enter_Csm_Area_Csm();
        Csm_JobQueBufUsed[(*curNode)->bufPos] = (boolean)FALSE;
        SchM_Exit_Csm_Area_Csm();
        (*curNode) = (*curNode)->next;
        queChRunPtr->jobCnt--;
        ret = E_OK;
    }
    else
    {
        for (idx = CSM_CONST_1; idx < queChRunPtr->jobCnt; idx++)
        {
            preNode = curNode;
            temNode = (*curNode)->next;
            /*Delete node from link list*/
            if (temNode->jobId == jobId)
            {
                SchM_Enter_Csm_Area_Csm();
                Csm_JobQueBufUsed[temNode->bufPos] = (boolean)FALSE;
                SchM_Exit_Csm_Area_Csm();
                (*preNode)->next = temNode->next;
                queChRunPtr->jobCnt--;
                ret = E_OK;
                break;
            }
        }
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief Find idle buffer in CSM queue
 */
/*************************************************************************/
static Csm_JobInQueuePtrType Csm_FindIdleQueue(uint32* bufPos)
{
    Csm_JobInQueueType* queBuf;
    uint32              queIdx;

    queBuf = NULL_PTR;
    SchM_Enter_Csm_Area_Csm();
    for (queIdx = 0u; queIdx < CSM_QUEUE_TOTAL_SIZE; queIdx++)
    {
        if (!Csm_JobQueBufUsed[queIdx])
        {
            break;
        }
    }
    SchM_Exit_Csm_Area_Csm();
    if (queIdx != CSM_QUEUE_TOTAL_SIZE)
    {
        queBuf  = &(Csm_JobQueBuf[queIdx]);
        *bufPos = queIdx;
        SchM_Enter_Csm_Area_Csm();
        Csm_JobQueBufUsed[queIdx] = (boolean)TRUE;
        SchM_Exit_Csm_Area_Csm();
    }
    return queBuf;
}
/*************************************************************************/
/**
 * @brief Check Crypto_OperationModeType
 */
/*************************************************************************/
static Std_ReturnType Csm_CheckOperaMode(Crypto_OperationModeType mode)
{
    Std_ReturnType ret = E_OK;

    if ((mode != CRYPTO_OPERATIONMODE_START) && (mode != CRYPTO_OPERATIONMODE_UPDATE)
        && (mode != CRYPTO_OPERATIONMODE_STREAMSTART) && (mode != CRYPTO_OPERATIONMODE_FINISH)
        && (mode != CRYPTO_OPERATIONMODE_SINGLECALL))
    {
        ret = E_NOT_OK;
    }
    return ret;
}
/* PRQA S 3432 -- */
/* PRQA S 1503 -- */
/* PRQA S 1532 -- */
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
