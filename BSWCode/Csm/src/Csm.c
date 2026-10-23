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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
#define CSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"

#define CSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"
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
#define CSM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"

#define CSM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"
/**
 * @brief Flags to indicate whether a job can be postponed.
 *        Each element corresponds to a job, and is used to manage job postponement.
 * @range 0-1
 */
boolean Csm_JobPostponeFlag[CSM_JOB_NUM];
#define CSM_STOP_SEC_VAR_CLEARED_BOOLEAN
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
    Csm_ArtiApiStart(2, CSM_SID_INIT, configPtr);

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
            Csm_JobPostponeFlag[jobIdx] = (boolean)FALSE;
        }
        /*Init job-queue*/
        for (queIdx = 0u; queIdx < CSM_QUEUE_NUM; queIdx++) /* PRQA S 2877 */ /* VL_Csm_2877 */
        {
            Csm_QueueChRunStatus[queIdx].pendingJobQue = NULL_PTR;
            Csm_QueueChRunStatus[queIdx].jobCnt        = 0u;
            Csm_QueueChRunStatus[queIdx].maxPriority   = CSM_INVALID_PRIORITY;
        }
        SchM_Enter_Csm_Area_Csm();
        for (queIdx = 0u; queIdx < CSM_QUEUE_TOTAL_SIZE; queIdx++)
        {
            Csm_JobQueBufUsed[queIdx] = (boolean)FALSE;
        }
        SchM_Exit_Csm_Area_Csm();
        Csm_InitStatus = (boolean)TRUE;
    }

    Csm_ArtiApiStop(1, CSM_SID_INIT);
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#if (CSM_GET_VERSION_INFO == STD_ON)
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief Returns the version information of this module.
 */
/*************************************************************************/
void Csm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    Csm_ArtiApiStart(2, CSM_SID_GETVERSIONINFO, versioninfo);

    Std_ReturnType ret = E_OK;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkGetVersionInfo(versioninfo, CSM_SID_GETVERSIONINFO);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        versioninfo->moduleID         = CSM_MODULE_ID;
        versioninfo->vendorID         = CSM_VENDOR_ID;
        versioninfo->sw_major_version = CSM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CSM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CSM_SW_PATCH_VERSION;
    }

    Csm_ArtiApiStop(1, CSM_SID_GETVERSIONINFO);
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
#endif /*CSM_GET_VERSION_INFO == STD_ON*/

#if (CSM_HASH_SUPPORT == STD_ON)
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(7, CSM_SID_HASH, jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_HASH, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
#endif /*CSM_HASH_SUPPORT == STD_ON*/

#if (CSM_MAC_SUPPORT == STD_ON)
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(7, CSM_SID_MAC_GEN, jobId, mode, dataPtr, dataLength, macPtr, macLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_MAC_GEN, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(8, CSM_SID_MAC_VERIFY, jobId, mode, dataPtr, dataLength, macPtr, macLength, verifyPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_MAC_VERIFY, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
#endif /*CSM_MAC_SUPPORT == STD_ON*/

#if (CSM_ENCRYPT_SUPPORT == STD_ON)
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(7, CSM_SID_ENCRYPT, jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_ENCRYPT, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(7, CSM_SID_DECRYPT, jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_DECRYPT, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
#endif /*CSM_ENCRYPT_SUPPORT == STD_ON*/

#if (CSM_AEADENCRYPT_SUPPORT == STD_ON)
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(
        11,
        CSM_SID_AEADENCRYPT,
        jobId,
        mode,
        plaintextPtr,
        plaintextLength,
        associatedDataPtr,
        associatedDataLength,
        ciphertextPtr,
        ciphertextLengthPtr,
        tagPtr,
        tagLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_AEADENCRYPT, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(
        12,
        CSM_SID_AEADDECRYPT,
        jobId,
        mode,
        ciphertextPtr,
        ciphertextLength,
        associatedDataPtr,
        associatedDataLength,
        tagPtr,
        tagLength,
        plaintextPtr,
        plaintextLengthPtr,
        verifyPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_AEADDECRYPT, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
#endif
#if (CSM_SIGNATURE_SUPPORT == STD_ON)
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(7, CSM_SID_SIG_GEN, jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_SIG_GEN, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(8, CSM_SID_SIG_VERIFY, jobId, mode, dataPtr, dataLength, signaturePtr, signatureLength, verifyPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_SIG_VERIFY, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
#endif /*CSM_SIGNATURE_SUPPORT == STD_ON*/

#if (CSM_RANDOM_SUPPORT == STD_ON)
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(5, CSM_SID_RANDOM_GEN, jobId, mode, resultPtr, resultLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_RANDOM_GEN, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
#endif /*CSM_RANDOM_SUPPORT == STD_ON*/
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief  Sets the given key element bytes to the key identified by keyId.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyElementSet(uint32 keyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(5, CSM_SID_KEYELEMENTSET, keyId, keyElementId, keyPtr, keyLength);

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

    Csm_ArtiApiStop(2, CSM_SID_KEYELEMENTSET, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief the key state of the key identified by keyId to valid.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeySetValid(uint32 keyId)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(2, CSM_SID_KEYSET_VALID, keyId);

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

    Csm_ArtiApiStop(2, CSM_SID_KEYSET_VALID, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief the key state of the key identified by keyId to invalid.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeySetInvalid(uint32 keyId)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(2, CSM_SID_KEYSET_INVALID, keyId);

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

    Csm_ArtiApiStop(2, CSM_SID_KEYSET_INVALID, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief Retrieves the key element bytes from a specific key element of the key
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyElementGet(uint32 keyId, uint32 keyElementId, uint8* keyPtr, uint32* keyLengthPtr)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(5, CSM_SID_KEYELEMENTGET, keyId, keyElementId, keyPtr, keyLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_KEYELEMENTGET, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(5, CSM_SID_KEYELEMENTCPY, keyId, keyElementId, targetKeyId, targetKeyElementId);

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

    Csm_ArtiApiStop(2, CSM_SID_KEYELEMENTCPY, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief This function shall copy all key elements from the source key to a target key.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyCopy(const uint32 keyId, const uint32 targetKeyId)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(3, CSM_SID_KEYCPY, keyId, targetKeyId);

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

    Csm_ArtiApiStop(2, CSM_SID_KEYCPY, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(
        8,
        CSM_SID_KEYELEMENTCPYPARTIAL,
        keyId,
        keyElementId,
        keyElementSourceOffset,
        keyElementTargetOffset,
        keyElementCopyLength,
        targetKeyId,
        targetKeyElementId);

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

    Csm_ArtiApiStop(2, CSM_SID_KEYELEMENTCPYPARTIAL, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief Feeds the key element CRYPTO_KE_RANDOM_SEED with a random seed.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_RandomSeed(uint32 keyId, const uint8* seedPtr, uint32 seedLength)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(4, CSM_SID_RANDOMSEED, keyId, seedPtr, seedLength);

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

    Csm_ArtiApiStop(2, CSM_SID_RANDOMSEED, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief  Generates new key material and store it in the key identified by keyId.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyGenerate(uint32 keyId)
{
    Csm_ArtiApiStart(2, CSM_SID_KEY_GENERATE, keyId);

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

    Csm_ArtiApiStop(2, CSM_SID_KEY_GENERATE, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(3, CSM_SID_KEY_DERIVE, keyId, targetKeyId);

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

    Csm_ArtiApiStop(2, CSM_SID_KEY_DERIVE, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(4, CSM_SID_KEY_EX_PUB, keyId, publicValuePtr, publicValueLengthPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_KEY_EX_PUB, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(4, CSM_SID_KEY_EX_SEC, keyId, partnerPublicValuePtr, partnerPublicValueLength);

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

    Csm_ArtiApiStop(2, CSM_SID_KEY_EX_SEC, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief  Stores the key if necessary and sets the key state of the key identified by keyId to valid.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobKeySetValid(uint32 jobId)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(2, CSM_SID_JOBKEYSETVALID, jobId);

    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeySetValid(jobId, CSM_SID_JOBKEYSETVALID);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_JobCfg[jobId].jobPtr->jobPrimitiveInfo->cryIfKeyId;
        ret                = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }

    Csm_ArtiApiStop(2, CSM_SID_JOBKEYSETVALID, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief  Stores the key if necessary and sets the key state of the key identified by keyId to valid.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobKeySetInValid(uint32 jobId)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(2, CSM_SID_JOBKEYSETINVALID, jobId);

    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeySetInValid(jobId, CSM_SID_JOBKEYSETINVALID);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_JobCfg[jobId].jobPtr->jobPrimitiveInfo->cryIfKeyId;
        ret                = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }

    Csm_ArtiApiStop(2, CSM_SID_JOBKEYSETINVALID, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief  This function shall dispatch the random seed function to the configured crypto driver object.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobRandomSeed(uint32 jobId, uint32 keyId, const uint8* seedPtr, uint32 seedLength)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(5, CSM_SID_JOBRANDOMSEED, jobId, keyId, seedPtr, seedLength);

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

    Csm_ArtiApiStop(2, CSM_SID_JOBRANDOMSEED, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief  Generates new key material and stores it in the key identified by keyId.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_JobKeyGenerate(uint32 jobId, uint32 keyId)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(3, CSM_SID_JOBKEYGENERATE, jobId, keyId);

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

    Csm_ArtiApiStop(2, CSM_SID_JOBKEYGENERATE, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(4, CSM_SID_JOBKEYDERIVE, jobId, keyId, targetKeyId);

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

    Csm_ArtiApiStop(2, CSM_SID_JOBKEYDERIVE, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(4, CSM_SID_JOBKEY_EX_PUB, jobId, publicValuePtr, publicValueLengthPtr);

    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobExCalcPubVal(jobId, publicValuePtr, publicValueLengthPtr, CSM_SID_JOBKEY_EX_PUB);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId               = Csm_JobCfg[jobId].jobPtr->jobPrimitiveInfo->cryIfKeyId;
        jobInfo.secondaryInputPtr        = publicValuePtr;
        jobInfo.secondaryInputLength     = *publicValuePtr;
        jobInfo.secondaryOutputLengthPtr = publicValueLengthPtr;
        ret                              = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }

    Csm_ArtiApiStop(2, CSM_SID_JOBKEY_EX_PUB, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(4, CSM_SID_JOBKEY_EX_SEC, jobId, partnerPublicValuePtr, partnerPublicValueLength);

    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobExCalcSec(jobId, partnerPublicValuePtr, partnerPublicValueLength, CSM_SID_JOBKEY_EX_SEC);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId  = Csm_JobCfg[jobId].jobPtr->jobPrimitiveInfo->cryIfKeyId;
        jobInfo.inputPtr    = partnerPublicValuePtr;
        jobInfo.inputLength = partnerPublicValueLength;
        ret                 = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }

    Csm_ArtiApiStop(2, CSM_SID_JOBKEY_EX_SEC, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief  Cancels the job processing from asynchronous or streaming jobs.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_CancelJob(uint32 job, Crypto_OperationModeType mode)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(3, CSM_SID_CANCELJOB, job, mode);

    const Csm_JobsCfgType* jobCfgPtr;
    Crypto_JobType*        jobInfoPtr;
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
        jobCfgPtr  = &(Csm_JobCfg[job]);
        jobQueId   = jobCfgPtr->queRef;
        cryIfChId  = Csm_QueCfg[jobQueId].cryIfChRef;
        jobInfoPtr = jobCfgPtr->jobPtr;
        cbkId      = jobInfoPtr->jobPrimitiveInfo->callbackId;
        /*[SWS_Csm_01021]*/
        ret = CryIf_CancelJob(cryIfChId, jobInfoPtr);
        if (E_OK == ret)
        {
#if (CSM_CBKFUNCS_NUM > 0u)
            if (Csm_CbkCfg[cbkId].cbkFnc != NULL_PTR)
            {
                /*[SWS_Csm_01030]*/
                Csm_CbkCfg[cbkId].cbkFnc(jobInfoPtr->jobId, CRYPTO_E_JOB_CANCELED);
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

    Csm_ArtiApiStop(2, CSM_SID_CANCELJOB, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief Retrieves the status of a key in the CSM module.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_KeyGetStatus(uint32 keyId, Crypto_KeyStatusType* keyStatusPtr)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(3, CSM_SID_KEY_GETSTATUS, keyId, keyStatusPtr);

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

    Csm_ArtiApiStop(2, CSM_SID_KEY_GETSTATUS, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(
        15,
        CSM_SID_CUSTOM_SEV,
        jobId,
        mode,
        targetKeyId,
        inputPtr,
        inputLength,
        secondaryInputPtr,
        secondaryInputLength,
        tertiaryInputPtr,
        tertiaryInputLength,
        outputPtr,
        outputLengthPtr,
        secondaryOutputPtr,
        secondaryOutputLengthPtr,
        verifyPtr);

    Crypto_JobPrimitiveInputOutputType customInfo;
    Std_ReturnType                     ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_CUSTOM_SEV);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        customInfo.mode                     = mode;
        customInfo.targetCryIfKeyId         = targetKeyId;
        customInfo.inputPtr                 = inputPtr;
        customInfo.inputLength              = inputLength;
        customInfo.secondaryInputPtr        = secondaryInputPtr;
        customInfo.secondaryInputLength     = secondaryInputLength;
        customInfo.tertiaryInputPtr         = tertiaryInputPtr;
        customInfo.tertiaryInputLength      = tertiaryInputLength;
        customInfo.outputPtr                = outputPtr;
        customInfo.outputLengthPtr          = outputLengthPtr;
        customInfo.secondaryOutputPtr       = secondaryOutputPtr;
        customInfo.secondaryOutputLengthPtr = secondaryOutputLengthPtr;
        customInfo.verifyPtr                = verifyPtr;
        ret                                 = Csm_ProcessJobToCryIf(&customInfo, jobId);
    }

    Csm_ArtiApiStop(2, CSM_SID_CUSTOM_SEV, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    Csm_ArtiApiStart(1, CSM_SID_MAINFUNCTION);

    /* PRQA S 3678 ++ */ /* VL_Csm_3678 */
    Csm_QueueChStatusType* queChRunPtr;
    /* PRQA S 3678 -- */
    uint32         queIdx;
    uint32         cryIfChId;
    uint32         jobInQueId;
    Std_ReturnType ret;

    /*Deal asynchronous job in queue*/
    for (queIdx = 0u; queIdx < CSM_QUEUE_NUM; queIdx++) /* PRQA S 2877 */ /* VL_Csm_2877 */
    {
        queChRunPtr = &(Csm_QueueChRunStatus[queIdx]);
        if (0uL == queChRunPtr->jobCnt)
        {
            /*There is a job being executed in queue, the CSM shall disable
             * processing new jobs
             * Ref:[SWS_Csm_00037]*/
            continue; /* PRQA S 0770 */ /* VL_Csm_0770 */
        }
        cryIfChId  = Csm_QueCfg[queIdx].cryIfChRef;
        jobInQueId = queChRunPtr->pendingJobQue->jobId;
        ret        = CryIf_ProcessJob(cryIfChId, Csm_JobCfg[jobInQueId].jobPtr);
        if (E_OK == ret)
        {
            /*Delete the job from queue*/
            (void)Csm_DequeueJobInQue(jobInQueId, queIdx);
        }
    }

    Csm_ArtiApiStop(1, CSM_SID_MAINFUNCTION);
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief Saves the context of a job in the CSM module.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_SaveContextJob(uint32 jobId, uint8* contextBufferPtr, uint32* contextBufferLengthPtr)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(4, CSM_SID_SAVE_CONTEXT, jobId, contextBufferPtr, contextBufferLengthPtr);

    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_SAVE_CONTEXT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        if (Csm_JobCfg[jobId].jobPtr->jobState == CRYPTO_JOBSTATE_IDLE)
        {
            ret = E_NOT_OK;
        }
        else
        {
            /*Job is in the state "active".*/
            Crypto_JobPrimitiveInputOutputType contextInfo;
            contextInfo.mode            = CRYPTO_OPERATIONMODE_SAVE_CONTEXT;
            contextInfo.outputPtr       = contextBufferPtr;
            contextInfo.outputLengthPtr = contextBufferLengthPtr;
            ret                         = Csm_ProcessJobToCryIf(&contextInfo, jobId);
        }
    }

    Csm_ArtiApiStop(2, CSM_SID_SAVE_CONTEXT, ret);

    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief Restores the context of a job in the CSM module.
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
Std_ReturnType Csm_RestoreContextJob(uint32 jobId, const uint8* contextBufferPtr, uint32 contextBufferLength)
/* PRQA S 1503 -- */
{
    Csm_ArtiApiStart(4, CSM_SID_RESTORE_CONTEXT, jobId, contextBufferPtr, contextBufferLength);

    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_RESTORE_CONTEXT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        if (Csm_JobCfg[jobId].jobPtr->jobState == CRYPTO_JOBSTATE_IDLE)
        {
            ret = E_NOT_OK;
        }
        else
        {
            /*Job is in the state "active".*/
            Crypto_JobPrimitiveInputOutputType contextInfo;
            contextInfo.mode        = CRYPTO_OPERATIONMODE_RESTORE_CONTEXT;
            contextInfo.inputPtr    = contextBufferPtr;
            contextInfo.inputLength = contextBufferLength;
            ret                     = Csm_ProcessJobToCryIf(&contextInfo, jobId);
        }
    }

    Csm_ArtiApiStop(2, CSM_SID_RESTORE_CONTEXT, ret);

    return ret;
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
    const Csm_JobsCfgType*       jobCfgPtr;
    const Csm_QueueChStatusType* queChRunPtr;
    uint32                       queueRef;
    uint32                       channelId;
    Crypto_ProcessingType        processMode;
    Std_ReturnType               ret = E_OK;

    jobCfgPtr = &(Csm_JobCfg[jobId]);
    if ((jobCfgPtr->jobPtr->jobRedirectionInfoRef != NULL_PTR)
        && ((jobPrimInOut->inputLength != 0uL) || (*(jobPrimInOut->outputLengthPtr) != 0uL)))
    {
        /*[SWS_Csm_91014]*/
        ret = E_NOT_OK;
    }
    else
    {
        Crypto_OperationModeType preMode = jobCfgPtr->jobPtr->jobPrimitiveInputOutput.mode;
        SchM_Enter_Csm_Area_Csm();
        jobCfgPtr->jobPtr->jobPrimitiveInputOutput = *jobPrimInOut;
        jobCfgPtr->jobPtr->jobPrimitiveInputOutput.mode |= preMode;
        SchM_Exit_Csm_Area_Csm();
        /*Job belong to queue*/
        queueRef    = jobCfgPtr->queRef;
        channelId   = Csm_QueCfg[queueRef].cryIfChRef;
        processMode = jobCfgPtr->jobPtr->jobPrimitiveInfo->processingType;

        /*Job in CRYPTO_JOBSTATE_ACTIVE state*/
        if (CRYPTO_JOBSTATE_ACTIVE == jobCfgPtr->jobPtr->jobState)
        {
            ret = CryIf_ProcessJob(channelId, jobCfgPtr->jobPtr);
        }

        /*Job in CRYPTO_JOBSTATE_IDLE state*/
        else
        {
            queChRunPtr = &(Csm_QueueChRunStatus[queueRef]);
            if (CRYPTO_PROCESSING_SYNC == processMode)
            {
                /*Is the job priority higher than the highest in the queue?*/
                if (((jobCfgPtr->jobPtr->jobPriority > queChRunPtr->maxPriority)) || (0uL == queChRunPtr->jobCnt))
                {
                    ret = CryIf_ProcessJob(channelId, jobCfgPtr->jobPtr);
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
                    ret = CryIf_ProcessJob(channelId, jobCfgPtr->jobPtr);
                    if (CRYPTO_E_BUSY == ret)
                    {
                        /*Enqueue job in CSM queue*/
                        (void)Csm_EnqueueJobToQue(jobId, queueRef, *jobPrimInOut);
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
/*************************************************************************/
/**
 * @brief Enqueue job to CSM queue
 */
/*************************************************************************/
/* PRQA S 1503, 6010, 6030, 5017 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Csm_STCYC, VL_MTR_Csm_STMIF, VL_Csm_5017 */
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    itemPriority = jobCfgPtr->jobPtr->jobPriority;
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
    if (queChRunPtr->pendingJobQue != NULL_PTR)
    {
        curNode = &(queChRunPtr->pendingJobQue);
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
    }
    return ret;
}
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"

#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
/* PRQA S 3432 -- */
/* PRQA S 1503 -- */
/* PRQA S 1532 -- */
