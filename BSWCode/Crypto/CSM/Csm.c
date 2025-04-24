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
**  FILENAME    : Csm.c                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for CSM                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>     <DATE>      <AUTHOR>        <REVISION LOG>
 *    V1.0.0     2019-10-18   qinchun.yang     Initial version
 *    V1.1.0     2020-06-20   yuzhe.zhang      R19-11
 *    V2.0.0     2023-02-06   jie.gu           R19_11
 *    V2.1.0     2023-07-17   jie.gu           CP2.1 Release Version
 *    V2.1.1     2023-11-22   jie.gu           CPD-33590 Code execution efficiency optimization
 *    V2.1.2     2024-01-31   jie.gu           CPD-33716 update code for misra c rules check
 *    V2.1.3     2024-05-27   jie.gu           CPD-33710 Add exclusive area
 */

/**
 \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
   ModeName:Csm<br>
 RuleSorce:puhua-rule.rcf 2.3.1

   \li PRQA S 3432 MISRA Rule 20.7 .<br>
   Reason:Function-like macros are used to allow more efficient code

   \li PRQA S 3432 MISRA Rule 10.4 .<br>
   Reason:Macros are used to judge the parameter value

   \li PRQA S 3432 MISRA Rule 10.1 .<br>
   Reason:Macros are used to judge the parameter value

   \li PRQA S 3432 MISRA Rule 8.9 .<br>
   Reason:Object may be refernced by other module or Function

   \li PRQA S 1532 MISRA Rule 8.7,Rule 2.1 .<br>
   Reason:Functions may be used in other projects.

   \li PRQA S 3432 MISRA Dir 4.1 .<br>
   Reason:Loop can be executed more than once in actual projects

   \li PRQA S 3432 MISRA Rule 5.2,Rule 1.3 .<br>
   Reason:Macros have the same string don't influence function
*/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Csm_Internal.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define CSM_C_AR_MAJOR_VERSION 4U
#define CSM_C_AR_MINOR_VERSION 5U
#define CSM_C_AR_PATCH_VERSION 0U
#define CSM_C_SW_MAJOR_VERSION 2U
#define CSM_C_SW_MINOR_VERSION 1U
#define CSM_C_SW_PATCH_VERSION 3U

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (                                                                                                           \
    (CSM_C_AR_MAJOR_VERSION != CSM_H_AR_MAJOR_VERSION) || (CSM_C_AR_MINOR_VERSION != CSM_H_AR_MINOR_VERSION)    \
    || (CSM_C_AR_PATCH_VERSION != CSM_H_AR_PATCH_VERSION) || (CSM_C_SW_MAJOR_VERSION != CSM_H_SW_MAJOR_VERSION) \
    || (CSM_C_SW_MINOR_VERSION != CSM_H_SW_MINOR_VERSION) || (CSM_C_SW_PATCH_VERSION != CSM_H_SW_PATCH_VERSION))

#error "Csm.c version mismatching with Csm.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
/* PRQA S 1503 ++ */ /* MISRA Rule 2.1 */
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*Call CryIf_ProcessJob,Transmit (Crypto_JobType) job to CryIf*/
static FUNC(Std_ReturnType, CSM_CODE) Csm_ProcessJobToCryIf(
    P2VAR(Crypto_JobPrimitiveInputOutputType, AUTOMATIC, AUTOMATIC) jobPrimInOut,
    VAR(uint32, AUTOMATIC) jobId);

/*Enqueue job to CSM queue*/
static FUNC(Std_ReturnType, CSM_CODE) Csm_EnqueueJobToQue(
    VAR(uint32, AUTOMATIC) jobId,
    VAR(uint32, AUTOMATIC) queId,
    VAR(Crypto_JobPrimitiveInputOutputType, AUTOMATIC) jobPriInOutInfo);

/*Dequeue job to CSM queue*/
static FUNC(Std_ReturnType, CSM_CODE) Csm_DequeueJobInQue(VAR(uint32, AUTOMATIC) jobId, VAR(uint32, AUTOMATIC) queId);

/*Find idle buffer in CSM queue*/
static FUNC(Csm_JobInQueuePtrType, CSM_CODE) Csm_FindIdleQueue(P2VAR(uint32, AUTOMATIC, AUTOMATIC) bufPos);

/*Check Crypto_OperationModeType*/
static FUNC(Std_ReturnType, CSM_CODE) Csm_CheckOperaMode(VAR(Crypto_OperationModeType, AUTOMATIC) mode);
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define CSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"
/*Store the buffer of JOB in QUEUE*/
static VAR(Csm_JobInQueueType, CSM_VAR_CLEARED_UNSPECIFIED) Csm_JobQueBuf[CSM_QUEUE_TOTAL_SIZE]; /*PRQA S 3218*/
static VAR(Csm_QueueChStatusType, CSM_VAR_CLEARED_UNSPECIFIED) Csm_QueueChRunStatus[CSM_QUEUE_NUM];

#define CSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"

#define CSM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"
/*Record whether Csm_JobQueBuf is used*/
static VAR(boolean, CSM_VAR_CLEARED_BOOLEAN) Csm_JobQueBufUsed[CSM_QUEUE_TOTAL_SIZE];
/* PRQA S 1514 */ /* MISRA Rule 8.9 */
#define CSM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
/*Module initialize status, TRUE initialized, FALSE not initialized*/
#define CSM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"
VAR(boolean, CSM_VAR_CLEARED_BOOLEAN) Csm_InitStatus; /* PRQA S 1514 */ /* MISRA Rule 8.9 */
/*In order to meet [SWS_Csm_01087]*/
VAR(boolean, CSM_VAR_CLEARED_BOOLEAN) Csm_JobPostponeFlag[CSM_JOB_NUM];
#define CSM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Csm_MemMap.h"

#define CSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"
/*Jobs states*/
VAR(Crypto_JobStateType, CSM_VAR_CLEARED_UNSPECIFIED) Csm_JobStates[CSM_JOB_NUM];
#define CSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Csm_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/**
 * Initializes the CSM module.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @configPtr, Pointer to a selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */

FUNC(void, CSM_CODE)
Csm_Init(P2CONST(Csm_ConfigType, AUTOMATIC, CSM_APPL_DATA) configPtr)
{
    uint32 jobIdx;
    uint32 queIdx;
#if (CSM_DEV_ERROR_DETECT == STD_ON)
    boolean chkRes;
    chkRes = Csm_ChkInitParam(configPtr);
    if ((boolean)TRUE == chkRes)
#else
    /*Avoid warning.*/
    (void)configPtr;
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*Initial job states*/
        for (jobIdx = 0u; jobIdx < CSM_JOB_NUM; jobIdx++)
        {
            Csm_JobStates[jobIdx] = CRYPTO_JOBSTATE_IDLE;
            Csm_JobPostponeFlag[jobIdx] = (boolean)FALSE;
        }
        /*Init job-queue*/
        for (queIdx = 0u; queIdx < CSM_QUEUE_NUM; queIdx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            Csm_QueueChRunStatus[queIdx].pendingJobQue = NULL_PTR;
            Csm_QueueChRunStatus[queIdx].jobCnt = 0u;
            Csm_QueueChRunStatus[queIdx].maxPriority = CSM_INVALID_PRIORITY;
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
/**
 * Returns the version information of this module.
 * Service ID: 0x3b
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): @versioninfo,Pointer to where to store the version information of this module.
 * Return value: NA
 */
FUNC(void, CSM_CODE)
Csm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CSM_APPL_DATA) versioninfo)
{
    Std_ReturnType ret = E_OK;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkGetVersionInfo(versioninfo, CSM_SID_GETVERSIONINFO);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        versioninfo->moduleID = CSM_MODULE_ID;
        versioninfo->vendorID = CSM_VENDOR_ID;
        versioninfo->sw_major_version = CSM_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CSM_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CSM_H_SW_PATCH_VERSION;
    }
}
#endif /*CSM_GET_VERSION_INFO == STD_ON*/

#if (CSM_HASH_SUPPORT == STD_ON)
/**
 * Uses the given data to perform the hash calculation and stores the hash.
 * Service ID: 0x5d
 * Sync/Async: Sync or Async, depending on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @dataPtr, Contains the pointer to the data for which the hash shall be computed.
 *    @dataLength, Contains the number of bytes to be hashed.
 * Parameters(INOUT):
 *    @resultLengthPtr, Holds a pointer to the memory location in which the output
 *       length in bytes is stored. On calling this function, this parameter shall
 *       contain the size of the buffer provided by resultPtr. When the request
 *       has finished, the actual length of the returned value shall be stored.
 * Parameters(OUT):
 *     @resultPtr, Contains the pointer to the data where the hash value shall be stored.
 * Return value:
 *     E_OK: request successful
 *     E_NOT_OK: request failed
 *     CRYPTO_E_BUSY: request failed, service is still busy
 *     CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 */
/* PRQA S 1503  */ /* MISRA Rule 2.1 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_Hash(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr)
{
    Crypto_JobPrimitiveInputOutputType hashInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_HASH);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            hashInfo.mode = mode;
            hashInfo.inputPtr = dataPtr;
            hashInfo.inputLength = dataLength;
            hashInfo.outputPtr = resultPtr;
            hashInfo.outputLengthPtr = resultLengthPtr;
            ret = Csm_ProcessJobToCryIf(&hashInfo, jobId);
        }
    }
    return ret;
}
#endif /*CSM_HASH_SUPPORT == STD_ON*/

#if (CSM_MAC_SUPPORT == STD_ON)
/**
 * Uses the given data to perform a MAC generation and stores the MAC in the memory
 * location pointed to by the MAC pointer
 * Service ID: 0x60
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @dataPtr, Contains the pointer to the data for which the MAC shall be computed.
 *    @dataLength, Contains the number of bytes to be hashed.
 * Parameters(INOUT):
 *    @macLengthPtr, Holds a pointer to the memory location in which the output
 *       length in bytes is stored. On calling this function, this parameter shall
 *       contain the size of the buffer provided by macPtr. When the request
 *       has finished, the actual length of the returned MAC shall be stored.
 * Parameters(OUT):
 *    @macPtr, Contains the pointer to the data where the MAC shall be stored.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size.
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_MacGenerate(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) macPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) macLengthPtr)
{
    Crypto_JobPrimitiveInputOutputType macInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_MAC_GEN);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            macInfo.mode = mode;
            macInfo.inputPtr = dataPtr;
            macInfo.inputLength = dataLength;
            macInfo.outputPtr = macPtr;
            macInfo.outputLengthPtr = macLengthPtr;
            ret = Csm_ProcessJobToCryIf(&macInfo, jobId);
        }
    }
    return ret;
}

/**
 * Verifies the given MAC by comparing if the MAC is generated with the given data.
 * Service ID: 0x61
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @dataPtr, Holds a pointer to the data for which the MAC shall be verified
 *    @dataLength, Contains the number of data bytes for which the MAC shall be verified.
 *    @macPtr, Holds a pointer to the MAC to be verified.
 *    @macLength, Contains the MAC length in BITS to be verified
 * Parameters(INOUT): NA
 * Parameters(OUT):
 *    @verifyPtr, Holds a pointer to the memory location, which will hold the result of the MAC verification.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER:The provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size.
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_MacVerify(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) macPtr,
    VAR(uint32, CSM_APPL_DATA) macLength,
    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_DATA) verifyPtr)
{
    Crypto_JobPrimitiveInputOutputType macInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_MAC_VERIFY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            macInfo.mode = mode;
            macInfo.inputPtr = dataPtr;
            macInfo.inputLength = dataLength;
            macInfo.secondaryInputPtr = macPtr;
            macInfo.secondaryInputLength = macLength / CSM_CONST_8;
            macInfo.verifyPtr = verifyPtr;
            ret = Csm_ProcessJobToCryIf(&macInfo, jobId);
        }
    }
    return ret;
}
#endif /*CSM_MAC_SUPPORT == STD_ON*/

#if (CSM_ENCRYPT_SUPPORT == STD_ON)
/**
 * Encrypts the given data and store the ciphertext in the memory location pointed by
 * the result pointer.
 * Service ID: 0x5e
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @dataPtr, Contains the pointer to the data to be encrypted.
 *    @dataLength,Contains the number of bytes to encrypt
 * Parameters(INOUT):
 *    @resultLengthPtr, Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by resultPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * Parameters(OUT):
 *    @resultPtr, Contains the pointer to the data where the encrypted data shall be stored.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_Encrypt(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr)
{
    Crypto_JobPrimitiveInputOutputType encryptInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_ENCRYPT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            encryptInfo.mode = mode;
            encryptInfo.inputPtr = dataPtr;
            encryptInfo.inputLength = dataLength;
            encryptInfo.outputPtr = resultPtr;
            encryptInfo.outputLengthPtr = resultLengthPtr;
            ret = Csm_ProcessJobToCryIf(&encryptInfo, jobId);
        }
    }

    return ret;
}

/**
 * Decrypts the given encrypted data and store the decrypted plaintext in the memory
 * location pointed by the result pointer.
 * Service ID: 0x5F
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @dataPtr, Contains the pointer to the data to be decrypted..
 *    @dataLength,Contains the number of bytes to decrypt.
 * Parameters(INOUT):
 *    @resultLengthPtr, Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by resultPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * Parameters(OUT):
 *    @resultPtr, Contains the pointer to the data where the decrypted data shall be stored.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_Decrypt(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr)
{
    Crypto_JobPrimitiveInputOutputType decryptInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_DECRYPT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            decryptInfo.mode = mode;
            decryptInfo.inputPtr = dataPtr;
            decryptInfo.inputLength = dataLength;
            decryptInfo.outputPtr = resultPtr;
            decryptInfo.outputLengthPtr = resultLengthPtr;
            ret = Csm_ProcessJobToCryIf(&decryptInfo, jobId);
        }
    }

    return ret;
}
#endif /*CSM_ENCRYPT_SUPPORT == STD_ON*/
#if (CSM_AEADENCRYPT_SUPPORT == STD_ON)
/**
 * Uses the given input data to perform a AEAD encryption and stores the ciphertext and the MAC
 *  in the memory locations pointed by the ciphertext
 * pointer and Tag pointer
 * Service ID: 0x62
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @plaintextPtr, Contains the pointer to the data to be encrypted.
 *    @plaintextLength,Contains the number of bytes to encrypt
 *    @associatedDataPtr, Contains the pointer to the associated data.
 *    @associatedDataLength, Contains the number of bytes of the associated data
 * Parameters(INOUT):
 *    @ciphertextLengthPtr, Holds a pointer to the memory location in which the output length
 *       information is stored in bytes. On calling this function, this  parameter shall contain
 *       the size of the buffer provided by ciphertextPtr.When the request has finished, the actual
 *       length of the returned value shall be stored.
 *    @tagLengthPtr, Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by tagPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * Parameters(OUT):
 *    @ciphertextPtr, Contains the pointer to the data where the encrypted data shall be stored.
 *    @tagPtr, Contains the pointer to the data where the Tag shall be stored.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_AEADEncrypt(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) plaintextPtr,
    VAR(uint32, CSM_APPL_DATA) plaintextLength,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) associatedDataPtr,
    VAR(uint32, CSM_APPL_DATA) associatedDataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) ciphertextPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) ciphertextLengthPtr,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) tagPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) tagLengthPtr)
{
    Crypto_JobPrimitiveInputOutputType AEADEncryptInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_AEADENCRYPT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            AEADEncryptInfo.mode = mode;
            AEADEncryptInfo.inputPtr = plaintextPtr;
            AEADEncryptInfo.inputLength = plaintextLength;
            AEADEncryptInfo.secondaryInputPtr = associatedDataPtr;
            AEADEncryptInfo.secondaryInputLength = associatedDataLength;
            AEADEncryptInfo.outputPtr = ciphertextPtr;
            AEADEncryptInfo.outputLengthPtr = ciphertextLengthPtr;
            AEADEncryptInfo.secondaryOutputPtr = tagPtr;
            AEADEncryptInfo.secondaryOutputLengthPtr = tagLengthPtr;
            ret = Csm_ProcessJobToCryIf(&AEADEncryptInfo, jobId);
        }
    }
    return ret;
}

/**
 * Uses the given data to perform an AEAD Decryption and stores the ciphertext
 * and the MAC in the memory locations pointed by the ciphertext pointer and
 * Tag pointer.
 * Service ID: 0x63
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @ciphertextPtr, Contains the pointer to the data to be decrypted..
 *    @ciphertextLength,Contains the number of bytes to decrypt.
 *    @associatedDataPtr, Contains the pointer to the associated data.
 *    @associatedDataLength, Contains the length in bytes of the associated data
 *    @tagPtr, Contains the pointer to the Tag to be verified.
 *    @tagLength,Contains the length in bytes of the Tag to be verified.
 * Parameters(INOUT):
 *    @plaintextLengthPtr, Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by plaintextPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * Parameters(OUT):
 *    @plaintextPtr, Contains the pointer to the data where the decrypted data shall be stored.
 *    @verifyPtr, Contains the pointer to the result of the verification.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_AEADDecrypt(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) ciphertextPtr,
    VAR(uint32, CSM_APPL_DATA) ciphertextLength,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) associatedDataPtr,
    VAR(uint32, CSM_APPL_DATA) associatedDataLength,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) tagPtr,
    VAR(uint32, CSM_APPL_DATA) tagLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) plaintextPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) plaintextLengthPtr,
    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_DATA) verifyPtr)
{
    Crypto_JobPrimitiveInputOutputType AEADDecryptInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_AEADDECRYPT);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            AEADDecryptInfo.mode = mode;
            AEADDecryptInfo.inputPtr = ciphertextPtr;
            AEADDecryptInfo.inputLength = ciphertextLength;
            AEADDecryptInfo.secondaryInputPtr = associatedDataPtr;
            AEADDecryptInfo.secondaryInputLength = associatedDataLength;
            AEADDecryptInfo.tertiaryInputPtr = tagPtr;
            AEADDecryptInfo.tertiaryInputLength = tagLength;
            AEADDecryptInfo.outputPtr = plaintextPtr;
            AEADDecryptInfo.outputLengthPtr = plaintextLengthPtr;
            AEADDecryptInfo.verifyPtr = verifyPtr;
            ret = Csm_ProcessJobToCryIf(&AEADDecryptInfo, jobId);
        }
    }
    return ret;
}
#endif
#if (CSM_SIGNATURE_SUPPORT == STD_ON)
/**
 * Uses the given data to perform the signature calculation and stores the
 * signature in the memory location pointed by the result pointer.
 * Service ID: 0x76
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @dataPtr, Contains the pointer to the data to be signed...
 *    @dataLength, Contains the number of bytes to sign.
 * Parameters(INOUT):
 *    @resultLengthPtr, Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by resultPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * Parameters(OUT):
 *    @resultPtr, Contains the pointer to the data where the signature shall be stored.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_SignatureGenerate(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr)
{
    Crypto_JobPrimitiveInputOutputType signatureInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_SIG_GEN);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            signatureInfo.mode = mode;
            signatureInfo.inputPtr = dataPtr;
            signatureInfo.inputLength = dataLength;
            signatureInfo.outputPtr = resultPtr;
            signatureInfo.outputLengthPtr = resultLengthPtr;
            ret = Csm_ProcessJobToCryIf(&signatureInfo, jobId);
        }
    }
    return ret;
}

/**
 * Verifies the given MAC by comparing if the signature is generated with the given data.
 * Service ID: 0x64
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @mode, Indicates which operation mode(s) to perfom
 *    @dataPtr, Contains the pointer to the data to be verified.
 *    @dataLength, Contains the number of data bytes.
 *    @signaturePtr, Holds a pointer to the signature to be verified
 *    @signatureLength, Contains the signature length in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT):
 *    @verifyPtr, Holds a pointer to the memory location, which will hold the result of the signature verification.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_SignatureVerify(

    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) signaturePtr,
    VAR(uint32, CSM_APPL_DATA) signatureLength,
    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_DATA) verifyPtr)
{
    Crypto_JobPrimitiveInputOutputType signatureInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_SIG_VERIFY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        ret = Csm_CheckOperaMode(mode);
        if (E_OK == ret)
        {
            signatureInfo.mode = mode;
            signatureInfo.inputPtr = dataPtr;
            signatureInfo.inputLength = dataLength;
            signatureInfo.secondaryInputPtr = signaturePtr;
            signatureInfo.secondaryInputLength = signatureLength;
            signatureInfo.verifyPtr = verifyPtr;
            ret = Csm_ProcessJobToCryIf(&signatureInfo, jobId);
        }
    }
    return ret;
}
#endif /*CSM_SIGNATURE_SUPPORT == STD_ON*/

#if (CSM_RANDOM_SUPPORT == STD_ON)
/**
 * Generate a random number and stores it in the memory location pointed by the result pointer
 * Service ID: 0x72
 * Sync/Async: Sync or Async, dependend on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 * Parameters(INOUT):
 *    @resultLengthPtr, Holds a pointer to the memory location in which the result
 *       length in bytes is stored. On calling this function, this parameter shall
 *       contain the number of random bytes, which shall be stored to the buffer
 *       provided by resultPtr. When the request has finished, the actual length
 *       of the returned value shall be stored.
 * Parameters(OUT):
 *    @resultPtr, Holds a pointer to the memory location which will hold the result
 *        of the random number generation.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_ENTROPY_EXHAUSTION: request failed, entropy of random number generator is exhausted.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_RandomGenerate(
    VAR(uint32, CSM_APPL_DATA) jobId,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr)
{
    Crypto_JobPrimitiveInputOutputType randomInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkServiceParam(jobId, CSM_SID_RANDOM_GEN);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        randomInfo.outputPtr = resultPtr;
        randomInfo.outputLengthPtr = resultLengthPtr;
        /*In order to meet the ST-Crypto-Drv implementation
         * 2019/11/12
         * */
        randomInfo.mode = CRYPTO_OPERATIONMODE_SINGLECALL;
        ret = Csm_ProcessJobToCryIf(&randomInfo, jobId);
    }
    return ret;
}
#endif /*CSM_RANDOM_SUPPORT == STD_ON*/

/**
 * Sets the given key element bytes to the key identified by keyId.
 * Service ID: 0x78
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key for which a new material shall be set.
 *    @keyElementId, Holds the identifier of the key element to be written.
 *    @keyPtr, Holds the pointer to the key element bytes to be processed.
 *    @keyLength, Contains the number of key element bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_WRITE_FAIL:Request failed because write access was denied
 *    CRYPTO_E_KEY_NOT_AVAILABLE: Request failed because the key is not available.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element size does not match size of provided data.
 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyElementSet(
    VAR(uint32, CSM_APPL_DATA) keyId,
    VAR(uint32, CSM_APPL_DATA) keyElementId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) keyPtr,
    VAR(uint32, CSM_APPL_DATA) keyLength)
{
    uint32 cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyEleSetParam(keyId, keyPtr, keyLength, CSM_SID_KEYELEMENTSET);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = CryIf_KeyElementSet(cryIfKeyId, keyElementId, keyPtr, keyLength);
    }
    return ret;
}

/**
 * Sets the key state of the key identified by keyId to valid.
 * Service ID: 0x67
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *   @ keyId, Holds the identifier of the key for which a new material shall be validated.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_KeySetValid(VAR(uint32, CSM_APPL_DATA) keyId)
{
    uint32 cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeySetValid(keyId, CSM_SID_KEYSET_VALID);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = CryIf_KeySetValid(cryIfKeyId);
    }
    return ret;
}

/**
 * Retrieves the key element bytes from a specific key element of the key
 * identified by the keyId and stores the key element in the memory location
 * pointed by the key pointer.
 * Service ID: 0x68
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key from which a key element shall be extracted.
 *    @keyElementId, Holds the identifier of the key element to be extracted
 * Parameters(INOUT):
 *    @keyLengthPtr, Holds a pointer to the memory location in which the output
 *       buffer length in bytes is stored. On calling this function, this parameter shall
 *       contain the buffer length in bytes of the keyPtr. When the request has finished,
 *       the actual size of the written input bytes shall be stored.
 * Parameters(OUT):
 *    @keyPtr, Holds the pointer to the memory location where the key shall be copied to.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_AVAILABLE: request failed, the requested key element is not available
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element
 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyElementGet(
    VAR(uint32, CSM_APPL_DATA) keyId,
    VAR(uint32, CSM_APPL_DATA) keyElementId,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) keyPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) keyLengthPtr)
{
    uint32 cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyEleGet(keyId, keyPtr, keyLengthPtr, CSM_SID_KEYELEMENTGET);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = CryIf_KeyElementGet(cryIfKeyId, keyElementId, keyPtr, keyLengthPtr);
    }
    return ret;
}

/**
 * This function shall copy a key elements from one key to a target key.
 * Service ID: 0x71
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same keyId
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key whose key element shall be the source element.
 *    @keyElementId, Holds the identifier of the key element which shall be the source for the copy operation.
 *    @targetKeyId, Holds the identifier of the key whose key element shall be the destination element.
 *    @targetKeyElementId, Holds the identifier of the key element which shall be the destination for the copy
 * operation. Parameters(INOUT): NA Parameters(OUT): NA Return value: E_OK: request successful E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_AVAILABLE: request failed, the requested key element is not available
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyElementCopy(
    CONST(uint32, CSM_APPL_DATA) keyId,
    CONST(uint32, CSM_APPL_DATA) keyElementId,
    CONST(uint32, CSM_APPL_DATA) targetKeyId,
    CONST(uint32, CSM_APPL_DATA) targetKeyElementId)
{
    uint32 cryIfSrcKeyId;
    uint32 cryIfDestKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyEleCopy(keyId, targetKeyId, CSM_SID_KEYELEMENTCPY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfSrcKeyId = Csm_KeyCfg[keyId].cryIfKey;
        cryIfDestKeyId = Csm_KeyCfg[targetKeyId].cryIfKey;
        ret = CryIf_KeyElementCopy(cryIfSrcKeyId, keyElementId, cryIfDestKeyId, targetKeyElementId);
    }
    return ret;
}

/**
 * This function shall copy all key elements from the source key to a target key.
 * Service ID: 0x73
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same keyId
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key whose key element shall be the source element.
 *    @targetKeyId, Holds the identifier of the key whose key element shall be the destination element.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_AVAILABLE: request failed, the requested key element is not available
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyCopy(CONST(uint32, CSM_APPL_DATA) keyId, CONST(uint32, CSM_APPL_DATA) targetKeyId)
{
    uint32 cryIfSrcKeyId;
    uint32 cryIfDestKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyCopy(keyId, targetKeyId, CSM_SID_KEYCPY);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfSrcKeyId = Csm_KeyCfg[keyId].cryIfKey;
        cryIfDestKeyId = Csm_KeyCfg[targetKeyId].cryIfKey;
        ret = CryIf_KeyCopy(cryIfSrcKeyId, cryIfDestKeyId);
    }
    return ret;
}

/**
 * Copies a key element to another key element in the same crypto driver. The keyElementSourceOffset
 * and keyElementCopyLength allows to copy just a part of the source key element into the destination.
 * The offset into the target key is also specified with this function.
 * Service ID: 0x79
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same keyId
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key whose key element shall be the source element for copy operation.
 *    @keyElementId, Holds the identifier of the key element which shall be the source for the copy operation.
 *    @keyElementSourceOffset:This is the offset of the source key element indicating the start index of the copy
 * operation.
 *    @keyElementTargetOffset:This is the offset of the destination key element indicating the start index of the copy
 * operation.
 *    @keyElementCopyLength:Specifies the number of bytes that shall be copied.
 *    @targetKeyId:Holds the identifier of the key whose key element shall be the destination element.
 *    targetKeyElementId:Holds the identifier of the key element which shall be the destination for the copy operation.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_AVAILABLE: request failed, the requested key element is not available
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyElementCopyPartial(
    VAR(uint32, CSM_APPL_DATA) keyId,
    VAR(uint32, CSM_APPL_DATA) keyElementId,
    VAR(uint32, CSM_APPL_DATA) keyElementSourceOffset,
    VAR(uint32, CSM_APPL_DATA) keyElementTargetOffset,
    VAR(uint32, CSM_APPL_DATA) keyElementCopyLength,
    VAR(uint32, CSM_APPL_DATA) targetKeyId,
    VAR(uint32, CSM_APPL_DATA) targetKeyElementId)
{
    uint32 cryIfSrcKeyId;
    uint32 cryIfDestKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyEleCopyPart(keyId, targetKeyId, CSM_SID_KEYELEMENTCPYPARTIAL);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfSrcKeyId = Csm_KeyCfg[keyId].cryIfKey;
        cryIfDestKeyId = Csm_KeyCfg[targetKeyId].cryIfKey;
        ret = CryIf_KeyElementCopyPartial(
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

/**
 * Feeds the key element CRYPTO_KE_RANDOM_SEED with a random seed.
 * Service ID: 0x69
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same keyId
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key for which a new seed shall be generated.
 *    @seedPtr, Holds a pointer to the memory location which contains the data to feed the seed.
 *    @seedLength, Contains the length of the seed in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_RandomSeed(
    VAR(uint32, CSM_APPL_DATA) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) seedPtr,
    VAR(uint32, CSM_APPL_DATA) seedLength)
{
    uint32 cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkRandomSeed(keyId, seedPtr, seedLength, CSM_SID_RANDOMSEED);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = CryIf_RandomSeed(cryIfKeyId, seedPtr, seedLength);
    }
    return ret;
}

/**
 * Generates new key material and store it in the key identified by keyId.
 * Service ID: 0x6A
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same keyId
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key for which a new material shall be generated.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyGenerate(VAR(uint32, CSM_APPL_DATA) keyId)
{
    uint32 cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyGenerate(keyId, CSM_SID_KEY_GENERATE);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = CryIf_KeyGenerate(cryIfKeyId);
    }
    return ret;
}

/**
 * Derives a new key by using the key elements in the given key identified by the
 * keyId. The given key contains the key elements for the password and salt. The
 * derived key is stored in the key element with the id 1 of the key identified
 * by targetCryptoKeyId.
 * Service ID: 0x6B
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same keyId
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key which is used for key derivation.
 *    @targetKeyId, Holds the identifier of the key which is used to store the derived key.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element.
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyDerive(VAR(uint32, CSM_APPL_DATA) keyId, VAR(uint32, CSM_APPL_DATA) targetKeyId)
{
    uint32 cryIfSrcKeyId;
    uint32 cryIfDestKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkKeyDerive(keyId, targetKeyId, CSM_SID_KEY_DERIVE);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfSrcKeyId = Csm_KeyCfg[keyId].cryIfKey;
        cryIfDestKeyId = Csm_KeyCfg[targetKeyId].cryIfKey;
        ret = CryIf_KeyDerive(cryIfSrcKeyId, cryIfDestKeyId);
    }
    return ret;
}

/**
 * Calculates the public value of the current user for the key exchange and stores
 * the public key in the memory location pointed by the public value pointer.
 * Service ID: 0x6C
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same keyId
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key which shall be used for the key exchange protocol.
 * Parameters(INOUT):
 *    @publicValueLengthPtr, Holds a pointer to the memory location in which the
 *      public value length information is stored. On calling this function,
 *      this parameter shall contain the size of the buffer provided by publicValuePtr.
 *      When the request has finished, the actual length of the returned value shall be stored.
 * Parameters(OUT):
 *    @publicValuePtr, Contains the pointer to the data where the public value shall be stored.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyExchangeCalcPubVal(
    VAR(uint32, CSM_APPL_DATA) keyId,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) publicValueLengthPtr)
{
    uint32 cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkExCalcPubVal(keyId, publicValuePtr, publicValueLengthPtr, CSM_SID_KEY_EX_PUB);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = CryIf_KeyExchangeCalcPubVal(cryIfKeyId, publicValuePtr, publicValueLengthPtr);
    }
    return ret;
}

/**
 * Calculates the shared secret key for the key exchange with the key material of
 * the key identified by the keyId and the partner public key. The shared secret
 * key is stored as a key element in the same key.
 * Service ID: 0x6D
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same keyId
 * Parameters(IN):
 *    @keyId, Holds the identifier of the key which shall be used for the key exchange protocol.
 *    @partnerPublicValuePtr, Holds the pointer to the memory location which contains the partner's public value.
 *    @partnerPublicValueLength, Contains the length of the partner's public value in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_KeyExchangeCalcSecret(
    VAR(uint32, CSM_APPL_DATA) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, CSM_APPL_DATA) partnerPublicValueLength)
{
    uint32 cryIfKeyId;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkExCalcSecVal(keyId, partnerPublicValuePtr, partnerPublicValueLength, CSM_SID_KEY_EX_SEC);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = CryIf_KeyExchangeCalcSecret(cryIfKeyId, partnerPublicValuePtr, partnerPublicValueLength);
    }
    return ret;
}

/**
 * Stores the key if necessary and sets the key state of the key identified by keyId to valid.
 * Service ID: 0x7A
 * Sync/Async: Sync or Async, depending on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @keyId, Holds the identifier of the key for which a new material shall be validated.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_JobKeySetValid(VAR(uint32, CSM_APPL_DATA) jobId, VAR(uint32, CSM_APPL_DATA) keyId)
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeySetValid(jobId, keyId, CSM_SID_JOBKEYSETVALID);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}

/**
 * This function shall dispatch the random seed function to the configured crypto driver object.
 * Service ID: 0x7B
 * Sync/Async: Sync or Async, depending on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @keyId, Holds the identifier of the key for which a new seed shall be generated.
 *    @seedPtr:Holds a pointer to the memory location which contains the data to feed the seed.
 *    @seedLength:Contains the length of the seed in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_JobRandomSeed(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(uint32, CSM_APPL_DATA) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) seedPtr,
    VAR(uint32, CSM_APPL_DATA) seedLength)
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobRandomSeed(jobId, keyId, seedPtr, seedLength, CSM_SID_JOBRANDOMSEED);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        jobInfo.inputPtr = seedPtr;
        jobInfo.inputLength = seedLength;
        ret = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}

/**
 * Generates new key material and stores it in the key identified by keyId.
 * Service ID: 0x7C
 * Sync/Async: Sync or Async, depending on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @keyId, Holds the identifier of the key for which a new material shall be generated.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_JobKeyGenerate(VAR(uint32, CSM_APPL_DATA) jobId, VAR(uint32, CSM_APPL_DATA) keyId)
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeyGenerate(jobId, keyId, CSM_SID_JOBKEYGENERATE);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        ret = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}

/**
 * Derives a new key by using the key elements in the given key identified by the keyId.
 * The given key contains the key elements for the password and salt. The derived key is
 * stored in the key element with the id 1 of the key identified by targetCryptoKeyId.
 * Service ID: 0x7C
 * Sync/Async: Sync or Async, depending on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @keyId, Holds the identifier of the key which is used for key derivation.
 *    @targetKeyId: Holds the identifier of the key which is used to store the derived key.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_READ_FAIL: Request failed, not allowed to extract key element
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_JobKeyDerive(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(uint32, CSM_APPL_DATA) keyId,
    VAR(uint32, CSM_APPL_DATA) targetKeyId)
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobKeyDerive(jobId, keyId, targetKeyId, CSM_SID_JOBKEYDERIVE);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        jobInfo.targetCryIfKeyId = targetKeyId;
        ret = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}

/**
 * Calculates the public value of the current user for the key exchange and stores the public key
 * in the memory location pointed by the public value pointer.
 * Service ID: 0x7E
 * Sync/Async: Sync or Async, depending on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @keyId, Holds the identifier of the key which shall be used for the key exchange protocol.
 *    @publicValuePtr: Contains the pointer to the data where the public value shall be stored.
 * Parameters(INOUT): NA
 * Parameters(OUT):
 *    @publicValueLengthPtr:Holds a pointer to the memory location in which the public value
 *      length information is stored. On calling this function, this parameter shall contain the
 *      size of the buffer provided by publicValuePtr. When the request has finished, the actual
 *      length of the returned value shall be stored.
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:  Request failed, Crypro Driver Object is busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_JobKeyExchangeCalcPubVal(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(uint32, CSM_APPL_DATA) keyId,
    /* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) publicValuePtr,
    /* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) publicValueLengthPtr)
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobExCalcPubVal(jobId, keyId, publicValuePtr, publicValueLengthPtr, CSM_SID_JOBKEY_EX_PUB);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        jobInfo.secondaryInputPtr = publicValuePtr;
        jobInfo.secondaryInputLength = *publicValuePtr;
        jobInfo.secondaryOutputLengthPtr = publicValueLengthPtr;
        ret = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}

/**
 * Calculates the shared secret key for the key exchange with the key material of the key
 * identified by the keyId and the partner public key. The shared secret key is stored as
 * a key element in the same key.
 * Service ID: 0x7F
 * Sync/Async: Sync or Async, depending on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *    @jobId, Holds the identifier of the job using the CSM service.
 *    @keyId, Holds the identifier of the key which shall be used for the key exchange protocol.
 *    @partnerPublicValuePtr, Holds the pointer to the memory location which contains the partner's public value.
 *    @partnerPublicValueLength, Contains the length of the partner's public value in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:Request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_JobKeyExchangeCalcSecret(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(uint32, CSM_APPL_DATA) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, CSM_APPL_DATA) partnerPublicValueLength)
{
    Crypto_JobPrimitiveInputOutputType jobInfo;
    Std_ReturnType ret;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkJobExCalcSec(jobId, keyId, partnerPublicValuePtr, partnerPublicValueLength, CSM_SID_JOBKEY_EX_SEC);
    if (E_OK == ret)
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        jobInfo.cryIfKeyId = Csm_KeyCfg[keyId].cryIfKey;
        jobInfo.inputPtr = partnerPublicValuePtr;
        jobInfo.inputLength = partnerPublicValueLength;
        ret = Csm_ProcessJobToCryIf(&jobInfo, jobId);
    }
    return ret;
}

/**
 * Cancels the job processing from asynchronous or streaming jobs.
 * Service ID: 0x6F
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *    @job, Holds the identifier of the job to be canceled
 *    @mode, Not used, just for interface compatibility provided.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 * E_OK: Request successful. Job removed from any queue and potentially from crypto driver hardware.
 * E_NOT_OK: Request failed
 * CRYPTO_E_JOB_CANCELED: Immediate cancelation not possible.The cancelation will be done at next
 * suitable processing step and notified via a negative finish callback.
 */

FUNC(Std_ReturnType, CSM_CODE)
Csm_CancelJob(VAR(uint32, CSM_APPL_DATA) job, VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode)
{
    P2CONST(Csm_JobsCfgType, AUTOMATIC, AUTOMATIC) jobCfgPtr;
    Crypto_JobType jobInfo;
    Std_ReturnType ret;
    uint32 cryIfChId;
    uint32 jobQueId;
    uint32 cbkId;

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    ret = Csm_ChkCancelJob(job, CSM_SID_CANCELJOB);
    if (E_OK == ret)
#else
    ret = E_NOT_OK;
#endif /*CSM_DEV_ERROR_DETECT == STD_ON*/
    {
        /*Not used, just for interface compatibility provided.
         * Avoid warning
         * */
        CSM_AVOID_WARNING(mode);
        SchM_Enter_Csm_Area_Csm();
        if (CRYPTO_JOBSTATE_ACTIVE == Csm_JobStates[job])
        {
            jobCfgPtr = &(Csm_JobCfg[job]);
            jobQueId = jobCfgPtr->queRef;
            cryIfChId = Csm_QueCfg[jobQueId].cryIfChRef;
            cbkId = jobCfgPtr->jobPrimitiveInfo->callbackId;
            jobInfo.jobInfo = jobCfgPtr->jobInfo;
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
                CSM_AVOID_WARNING(cbkId);
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
        }
        SchM_Exit_Csm_Area_Csm();
    }
    return ret;
}

/**
 * API to be called cyclically to process the requested jobs. The Csm_MainFunction
 * shall check the queues for jobs to pass to the underlying CRYIF.
 * Service ID: 0x01
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */

FUNC(void, CSM_CODE)
Csm_MainFunction(void) /* PRQA S 3408 */ /* MISRA Rule 8.4 */
{
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */
    P2VAR(Csm_QueueChStatusType, AUTOMATIC, AUTOMATIC) queChRunPtr;
    /* PRQA S 3678 -- */ /* MISRA Rule 8.13 */
    P2CONST(Csm_JobsCfgType, AUTOMATIC, AUTOMATIC) jobCfgPtr;
    Crypto_JobType jobInfo;
    uint32 queIdx;
    uint32 cryIfChId;
    uint32 jobInQueId;
    Std_ReturnType ret;

    /*Deal asynchronous job in queue*/
    for (queIdx = 0u; queIdx < CSM_QUEUE_NUM; queIdx++) /* PRQA S 2877 ++*/ /* MISRA Dir 4.1 */
    {
        queChRunPtr = &(Csm_QueueChRunStatus[queIdx]);
        if (((boolean)TRUE == queChRunPtr->dealSynJobIng) || (0u == queChRunPtr->jobCnt))
        {
            /*There is a job being executed in queue, the CSM shall disable
             * processing new jobs
             * Ref:[SWS_Csm_00037]*/
            continue;
        }
        cryIfChId = Csm_QueCfg[queIdx].cryIfChRef;
        jobInQueId = queChRunPtr->pendingJobQue->jobId;
        jobCfgPtr = &(Csm_JobCfg[jobInQueId]);
        jobInfo.jobRedirectionInfoRef = jobCfgPtr->inOutRedirtRef;
        jobInfo.jobInfo = jobCfgPtr->jobInfo;
        jobInfo.jobPrimitiveInfo = jobCfgPtr->jobPrimitiveInfo;
        jobInfo.jobId = jobInQueId;
        SchM_Enter_Csm_Area_Csm();
        jobInfo.jobState = Csm_JobStates[jobInQueId];
        SchM_Exit_Csm_Area_Csm();
        jobInfo.jobPrimitiveInputOutput = queChRunPtr->pendingJobQue->jobPriInOut;
        ret = CryIf_ProcessJob(cryIfChId, &jobInfo);
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
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define CSM_START_SEC_CODE
#include "Csm_MemMap.h"
/*Call CryIf_ProcessJob,Transmit (Crypto_JobType) job to CryIf*/
static FUNC(Std_ReturnType, CSM_CODE) Csm_ProcessJobToCryIf(
    /* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
    P2VAR(Crypto_JobPrimitiveInputOutputType, AUTOMATIC, AUTOMATIC) jobPrimInOut,
    /* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
    VAR(uint32, AUTOMATIC) jobId)
{
    Crypto_JobType job;
    P2CONST(Csm_JobsCfgType, AUTOMATIC, AUTOMATIC) jobCfgPtr;
    P2VAR(Csm_QueueChStatusType, AUTOMATIC, AUTOMATIC) queChRunPtr;
    uint32 queueRef;
    uint32 channelId;
    Crypto_JobStateType jobState;
    Crypto_ProcessingType processMode;
    Std_ReturnType ret = E_OK;

    jobCfgPtr = &(Csm_JobCfg[jobId]);
    SchM_Enter_Csm_Area_Csm();
    jobState = Csm_JobStates[jobId];
    SchM_Exit_Csm_Area_Csm();
    if ((jobCfgPtr->inOutRedirtRef != NULL_PTR)
        && ((jobPrimInOut->inputLength != 0u) || (*(jobPrimInOut->outputLengthPtr) != 0u)))
    {
        /*[SWS_Csm_91014]*/
        ret = E_NOT_OK;
    }
    else
    {

        job.jobRedirectionInfoRef = jobCfgPtr->inOutRedirtRef;
        job.jobInfo = jobCfgPtr->jobInfo;
        job.jobPrimitiveInfo = jobCfgPtr->jobPrimitiveInfo;
        job.jobId = jobId;
        job.jobState = jobState;
        job.jobPrimitiveInputOutput = *jobPrimInOut;
        /*Job belong to queue*/
        queueRef = jobCfgPtr->queRef;
        channelId = Csm_QueCfg[queueRef].cryIfChRef;
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
                else if ((jobPrimInOut->mode & CRYPTO_OPERATIONMODE_FINISH) != 0u) /*PRQA S 4522,1823*/
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
                if (((jobCfgPtr->jobInfo->jobPriority > queChRunPtr->maxPriority)) || (0u == queChRunPtr->jobCnt))
                {
                    queChRunPtr->dealSynJobIng = (boolean)TRUE;
                    ret = CryIf_ProcessJob(channelId, &job);
                    if ((E_OK == ret) && (0u == (jobPrimInOut->mode & CRYPTO_OPERATIONMODE_FINISH)) /*PRQA S 4522,1823*/
                    )
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
                if (0u == queChRunPtr->jobCnt)
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
                    }
                }
            }
        }
    }
    return ret;
}

/*Enqueue job to CSM queue*/
static FUNC(Std_ReturnType, CSM_CODE) Csm_EnqueueJobToQue(
    VAR(uint32, AUTOMATIC) jobId,
    VAR(uint32, AUTOMATIC) queId,
    VAR(Crypto_JobPrimitiveInputOutputType, AUTOMATIC) jobPriInOutInfo)
{
    P2VAR(Csm_QueueChStatusType, AUTOMATIC, AUTOMATIC) queChRunPtr;
    P2CONST(Csm_JobsCfgType, AUTOMATIC, AUTOMATIC) jobCfgPtr;
    P2VAR(Csm_JobInQueueType, AUTOMATIC, AUTOMATIC) queBuf;
    P2VAR(Csm_JobInQueueType, AUTOMATIC, AUTOMATIC) swapqueBuf;
    P2VAR(Csm_JobInQueueType, AUTOMATIC, AUTOMATIC) preNode = NULL_PTR;
    P2VAR(Csm_JobInQueuePtrType, AUTOMATIC, AUTOMATIC) curNode;
    P2VAR(Csm_JobInQueueType, AUTOMATIC, AUTOMATIC) pendingNode;
    uint32 bufPos;
    uint32 itemPriority;
    uint32 idx;
    Std_ReturnType ret = E_OK;
    boolean break_Flag = FALSE;

    queChRunPtr = &(Csm_QueueChRunStatus[queId]);
    jobCfgPtr = &(Csm_JobCfg[jobId]);
    itemPriority = jobCfgPtr->jobInfo->jobPriority;
    curNode = &(queChRunPtr->pendingJobQue);
    pendingNode = queChRunPtr->pendingJobQue;
    /*Update the max priority in queue*/
    if ((queChRunPtr->maxPriority == CSM_INVALID_PRIORITY) || (itemPriority > queChRunPtr->maxPriority))
    {
        queChRunPtr->maxPriority = itemPriority;
    }
    if (0u == queChRunPtr->jobCnt)
    {
        /*Queue is empty*/
        (*curNode) = Csm_FindIdleQueue(&bufPos);
        if ((*curNode) != NULL_PTR)
        {
            (*curNode)->jobId = jobId;
            (*curNode)->jobPriority = itemPriority;
            (*curNode)->next = NULL_PTR;
            (*curNode)->bufPos = bufPos;
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
                queBuf->bufPos = bufPos;
                queBuf->jobId = jobId;
                queBuf->jobPriority = itemPriority;
                queBuf->jobPriInOut = jobPriInOutInfo;
                for (idx = 0u; idx < queChRunPtr->jobCnt; idx++)
                {
                    /*[SWS_Csm_00944]*/
                    if ((itemPriority > (*curNode)->jobPriority))
                    {
                        swapqueBuf = (*curNode);
                        (*curNode) = queBuf;
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
                            queBuf->next = (*curNode)->next;
                            (*curNode)->next = queBuf;
                            break_Flag = TRUE;
                        }
                        else
                        {
                            preNode = (*curNode);
                            (*curNode) = (*curNode)->next;
                        }
                    }
                    if (break_Flag == TRUE)
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

/*Dequeue job to CSM queue*/
static FUNC(Std_ReturnType, CSM_CODE) Csm_DequeueJobInQue(VAR(uint32, AUTOMATIC) jobId, VAR(uint32, AUTOMATIC) queId)
{
    P2VAR(Csm_QueueChStatusType, AUTOMATIC, AUTOMATIC) queChRunPtr;
    P2VAR(Csm_JobInQueuePtrType, AUTOMATIC, AUTOMATIC) curNode;
    /* PRQA S 3678 ++ */ /* MISRA Rule 8.13 */
    P2VAR(Csm_JobInQueuePtrType, AUTOMATIC, AUTOMATIC) preNode;
    /* PRQA S 3678 -- */ /* MISRA Rule 8.13 */
    P2VAR(Csm_JobInQueueType, AUTOMATIC, AUTOMATIC) temNode;
    uint32 idx;
    Std_ReturnType ret = E_NOT_OK;

    queChRunPtr = &(Csm_QueueChRunStatus[queId]);
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

/*Find idle buffer in CSM queue*/
static FUNC(Csm_JobInQueuePtrType, CSM_CODE) Csm_FindIdleQueue(P2VAR(uint32, AUTOMATIC, AUTOMATIC) bufPos)
{
    P2VAR(Csm_JobInQueueType, AUTOMATIC, AUTOMATIC) queBuf;
    uint32 queIdx;

    queBuf = NULL_PTR;
    SchM_Enter_Csm_Area_Csm();
    for (queIdx = 0u; queIdx < CSM_QUEUE_TOTAL_SIZE; queIdx++)
    {
        if ((boolean)FALSE == Csm_JobQueBufUsed[queIdx])
        {
            break;
        }
    }
    SchM_Exit_Csm_Area_Csm();
    if (queIdx != CSM_QUEUE_TOTAL_SIZE)
    {
        queBuf = &(Csm_JobQueBuf[queIdx]);
        *bufPos = queIdx;
        SchM_Enter_Csm_Area_Csm();
        Csm_JobQueBufUsed[queIdx] = (boolean)TRUE;
        SchM_Exit_Csm_Area_Csm();
    }
    return queBuf;
}

/*Check Crypto_OperationModeType*/
static FUNC(Std_ReturnType, CSM_CODE) Csm_CheckOperaMode(VAR(Crypto_OperationModeType, AUTOMATIC) mode)
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
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/* PRQA S 1503 -- */ /* MISRA Rule 2.1 */
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
#define CSM_STOP_SEC_CODE
#include "Csm_MemMap.h"
