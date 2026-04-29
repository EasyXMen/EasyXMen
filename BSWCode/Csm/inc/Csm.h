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
 **  @file               : Csm.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of Csm
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.2.1    20250331    jie.gu       inittial version for R23-11
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:BswM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Csm_1271
      Reason: non-init expression for enum definition is necessary
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_0722
      Reason: Enum constant not explicitly initialized
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_3432
      Reason: Function-like macros are used to allow more efficient code.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_3430
      Reason: The code requires the use of struct nesting.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_1503
      Reason: Allow some external functions not to be called.Since we're providing an API set, will it be called or not
  depends on how the project is integrated.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_1532
      Reason: autosar requires functions to be defined and declared in specific units.Allow these functions to be
  called, possibly once, in the integration code.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_3218
      Reason: Used to record global information, such as the timestamp of the first entry into the main function, the
  last access to the main function.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_1514
      Reason: Static global variables need to be used
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_2877
      Reason: The usage of this file depends on the config
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_Csm_STPAR
      Reason: The parameters of the function need.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_3408
      Reason: There is no declaration before the definition.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_Csm_STCYC
      Reason: Functionality is not separated into subfunctions because there are higher requirements for minimizing the
  stack and applying it to runtime use of the code.
      Risk: No risk.
      Prevention: None.

    \li VL_MTR_Csm_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_3673
      Reason: Pointers without const need to be used.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_1317
      Reason: Definitions from other modules are used.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_4522
      Reason: The enumeration type needs to be enumerated for bitwise and operations.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_1823
      Reason: It is necessary to compare signed numbers with unsigned ones.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_2109
      Reason: Boolean types need to be used as operands
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_5016
      Reason: Parameter passing requires
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_UsedBeforModify
      Reason: The operation is not redundant, because it must be emptied.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_3678
      Reason: non-const type declaration is necessary depends on the config
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_0770
      Reason: Ability to quickly jump of next loops in different situations with different configurations.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_5017
      Reason: High cyclomatic complexity is reserved for better readability.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_0553
      Reason: The usage of this file depends on the config.
      Risk: No risk.
      Prevention: None.

    \li VL_Csm_3207
      Reason: It depends on the configuration.
      Risk: No risk.
      Prevention: None.

 */

#ifndef CSM_H
#define CSM_H

/* =================================================== inclusions =================================================== */
#include "Csm_Types.h"
#include "Csm_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/*Module ID and version*/
#define CSM_MODULE_ID          ((uint16)(110U)) /** @brief  Module ID of the CSM module */
#define CSM_VENDOR_ID          ((uint16)(62U))  /** @brief  Vendor ID of the CSM module */
#define CSM_MODULE_INSTANCE_ID ((0U))           /** @brief  Instance ID of the CSM module */

#define CSM_H_AR_MAJOR_VERSION 4U /** @brief  Major version of the CSM module's AUTOSAR version */
#define CSM_H_AR_MINOR_VERSION 9U /** @brief  Minor version of the CSM module's AUTOSAR version */
#define CSM_H_AR_PATCH_VERSION 0U /** @brief  Patch version of the CSM module's AUTOSAR version */
#define CSM_H_SW_MAJOR_VERSION 2U /** @brief  Major version of the CSM module's software version */
#define CSM_H_SW_MINOR_VERSION 1U /** @brief  Minor version of the CSM module's software version */
#define CSM_H_SW_PATCH_VERSION 3U /** @brief  Patch version of the CSM module's software version */

/* ===================================================== macros ===================================================== */
#if (STD_ON == CSM_DEV_ERROR_DETECT)
/*Development errors used by the CSM module*/
#define CSM_E_PARAM_POINTER   (0x01u) /** @brief  Error code for invalid pointer parameter */
#define CSM_E_PARAM_HANDLE    (0x04u) /** @brief  Error code for invalid handle parameter */
#define CSM_E_UNINIT          (0x05u) /** @brief  Error code for uninitialized module */
#define CSM_E_INIT_FAILED     (0x07u) /** @brief  Error code for initialization failure */
#define CSM_E_PROCESSING_MODE (0x08u) /** @brief  Error code for invalid processing mode */
#define CSM_E_SERVICE_TYPE    (0x09u) /** @brief  Error code for invalid service type */

/*Runtime error codes*/
#define CSM_E_QUEUE_FULL (0x01u) /** @brief  Runtime error for queue full */
/*service id*/
#define CSM_SID_INIT                 (0x00u) /** @brief  Service ID for initialization */
#define CSM_SID_GETVERSIONINFO       (0x3Bu) /** @brief  Service ID for getting version information */
#define CSM_SID_HASH                 (0x5Du) /** @brief  Service ID for hash operation */
#define CSM_SID_MAC_GEN              (0x60u) /** @brief  Service ID for MAC generation */
#define CSM_SID_MAC_VERIFY           (0x61u) /** @brief  Service ID for MAC verification */
#define CSM_SID_ENCRYPT              (0x5Eu) /** @brief  Service ID for encryption */
#define CSM_SID_DECRYPT              (0x5Fu) /** @brief  Service ID for decryption */
#define CSM_SID_AEADENCRYPT          (0x62u) /** @brief  Service ID for AEAD encryption */
#define CSM_SID_AEADDECRYPT          (0x63u) /** @brief  Service ID for AEAD decryption */
#define CSM_SID_SIG_GEN              (0x76u) /** @brief  Service ID for signature generation */
#define CSM_SID_SIG_VERIFY           (0x64u) /** @brief  Service ID for signature verification */
#define CSM_SID_RANDOM_GEN           (0x72u) /** @brief  Service ID for random number generation */
#define CSM_SID_KEYELEMENTSET        (0x78u) /** @brief  Service ID for setting key element */
#define CSM_SID_KEYSET_VALID         (0x67u) /** @brief  Service ID for setting key valid */
#define CSM_SID_KEYELEMENTGET        (0x68u) /** @brief  Service ID for getting key element */
#define CSM_SID_KEYELEMENTCPY        (0x71u) /** @brief  Service ID for copying key element */
#define CSM_SID_KEYCPY               (0x73u) /** @brief  Service ID for copying key */
#define CSM_SID_KEYELEMENTCPYPARTIAL (0x79u) /** @brief  Service ID for partially copying key element */
#define CSM_SID_RANDOMSEED           (0x69u) /** @brief  Service ID for setting random seed */
#define CSM_SID_KEY_GENERATE         (0x6Au) /** @brief  Service ID for key generation */
#define CSM_SID_KEY_DERIVE           (0x6Bu) /** @brief  Service ID for key derivation */
#define CSM_SID_KEY_EX_PUB           (0x6Cu) /** @brief  Service ID for exporting public key */
#define CSM_SID_KEY_EX_SEC           (0x6Du) /** @brief  Service ID for exporting secret key */
#define CSM_SID_JOBKEYSETVALID       (0x7Au) /** @brief  Service ID for setting job key valid */
#define CSM_SID_JOBRANDOMSEED        (0x7Bu) /** @brief  Service ID for setting job random seed */
#define CSM_SID_JOBKEYGENERATE       (0x7Cu) /** @brief  Service ID for job key generation */
#define CSM_SID_JOBKEYDERIVE         (0x7Du) /** @brief  Service ID for job key derivation */
#define CSM_SID_JOBKEY_EX_PUB        (0x7Eu) /** @brief  Service ID for exporting job public key */
#define CSM_SID_JOBKEY_EX_SEC        (0x7Fu) /** @brief  Service ID for exporting job secret key */
#define CSM_SID_JOBCERTIFICATEPARSE  (0x80u) /** @brief  Service ID for parsing job certificate */
#define CSM_SID_JOBCERTIFICATEVERIFY (0x81u) /** @brief  Service ID for verifying job certificate */
#define CSM_SID_CANCELJOB            (0x6Fu) /** @brief  Service ID for canceling job */
#define CSM_SID_CBK_NOTIFY           (0x70u) /** @brief  Service ID for callback notification */
#define CSM_SID_KEY_GETSTATUS        (0x83u) /** @brief  Service ID for getting key status */
#define CSM_SID_MAINFUNCTION         (0x01u) /** @brief  Service ID for main function */
#define CSM_SID_JOBKEYSETINVALID     (0x84u) /** @brief  Service ID for setting job keyinvalid */
#define CSM_SID_KEYSET_INVALID       (0x85u) /** @brief  Service ID for invalid key */
#endif

/* ================================================ type definitions ================================================ */
/**
 * @brief     CSM configuration type
 */
typedef uint8 Csm_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief Initializes the CSM module.
 * @synchronous  TRUE
 * @reentrant Reentrant
 * @param[in] configPtr: Pointer to a selected configuration structure
 * @trace       CPD-71169
 */
void Csm_Init(const Csm_ConfigType* configPtr);
#if (CSM_GET_VERSION_INFO == STD_ON)

/**
 * @brief Returns the version information of this module.
 * @synchronous  TRUE
 * @reentrant Reentrant.
 * @param[out]  versioninfo: Pointer to where to store the version information of this module.
 * @trace       CPD-71170
 */
void Csm_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#if (CSM_HASH_SUPPORT == STD_ON)

/**
 * @brief Uses the given data to perform the hash calculation and stores the hash.
 * @synchronous TRUE or FALSE, depending on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]  mode: Indicates which operation mode(s) to perfom
 * @param[in] dataPtr: Contains the pointer to the data for which the hash shall be computed.
 * @param[in] dataLength: Contains the number of bytes to be hashed.
 * @param[inout]
 *       resultLengthPtr: Holds a pointer to the memory location in which the output
 *       length in bytes is stored. On calling this function, this parameter shall
 *       contain the size of the buffer provided by resultPtr. When the request
 *       has finished, the actual length of the returned value shall be stored.
 * @param[out]
 *      resultPtr: Contains the pointer to the data where the hash value shall be stored.
 * @retval
 *     E_OK: request successful
 *     E_NOT_OK: request failed
 *     CRYPTO_E_BUSY: request failed, service is still busy
 *     CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 * @trace       CPD-71171
 * @implements CPD-49581
 */
Std_ReturnType Csm_Hash(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   resultPtr,
    uint32*                  resultLengthPtr);

#endif

#if (CSM_MAC_SUPPORT == STD_ON)
/**
 * @brief Uses the given data to perform a MAC generation and stores the MAC in the memory
 * location pointed to by the MAC pointer
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in] jobId: Holds the identifier of the job using the CSM service.
 * @param[in] mode: Indicates which operation mode(s) to perfom
 * @param[in] dataPtr: Contains the pointer to the data for which the MAC shall be computed.
 * @param[in] dataLength: Contains the number of bytes to be hashed.
 * @param[inout]
 *    macLengthPtr: Holds a pointer to the memory location in which the output
 *       length in bytes is stored. On calling this function, this parameter shall
 *       contain the size of the buffer provided by macPtr. When the request
 *       has finished, the actual length of the returned MAC shall be stored.
 * @param[out]
 *    macPtr: Contains the pointer to the data where the MAC shall be stored.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71172
 * @implements CPD-49582
 */
Std_ReturnType Csm_MacGenerate(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   macPtr,
    uint32*                  macLengthPtr);

/**
 * @brief Verifies the given MAC by comparing if the MAC is generated with the given data.
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in] mode: Indicates which operation mode(s) to perfom
 * @param[in] dataPtr: Holds a pointer to the data for which the MAC shall be verified
 * @param[in] dataLength: Contains the number of data bytes for which the MAC shall be verified.
 * @param[in]  macPtr: Holds a pointer to the MAC to be verified.
 * @param[in]  macLength: Contains the MAC length in BITS to be verified
 * @param[out]
 *    verifyPtr: Holds a pointer to the memory location, which will hold the result of the MAC verification.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER:The provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71173
 * @implements CPD-49583
 */
Std_ReturnType Csm_MacVerify(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    const uint8*             macPtr,
    uint32                   macLength,
    Crypto_VerifyResultType* verifyPtr);

#endif

#if (CSM_ENCRYPT_SUPPORT == STD_ON)
/**
 * @brief Encrypts the given data and store the ciphertext in the memory location pointed by
 * the result pointer.
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]  mode: Indicates which operation mode(s) to perfom
 * @param[in]  dataPtr: Contains the pointer to the data to be encrypted.
 * @param[in]  dataLength: Contains the number of bytes to encrypt.
 * @param[inout]
 *    resultLengthPtr: Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by resultPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * @param[out]
 *    resultPtr: Contains the pointer to the data where the encrypted data shall be stored.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element
 * @trace       CPD-71174
 * @implements CPD-49584
 */
Std_ReturnType Csm_Encrypt(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   resultPtr,
    uint32*                  resultLengthPtr);

/**
 * @brief Decrypts the given encrypted data and store the decrypted plaintext in the memory
 * location pointed by the result pointer.
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]  mode: Indicates which operation mode(s) to perfom
 * @param[in]  dataPtr: Contains the pointer to the data to be decrypted.
 * @param[in]  dataLength: Contains the number of bytes to decrypt.
 * @param[inout]
 *    resultLengthPtr: Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by resultPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * @param[out]
 *    resultPtr: Contains the pointer to the data where the decrypted data shall be stored.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element
 * @trace       CPD-71175
 * @implements CPD-49585
 */
Std_ReturnType Csm_Decrypt(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   resultPtr,
    uint32*                  resultLengthPtr);
#endif

#if (CSM_AEADENCRYPT_SUPPORT == STD_ON)
/**
 * @brief Uses the given input data to perform a AEAD encryption and stores the ciphertext and the MAC
 *  in the memory locations pointed by the ciphertext
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in] mode: Indicates which operation mode(s) to perfom
 * @param[in]  plaintextPtr: Contains the pointer to the data to be encrypted.
 * @param[in]  plaintextLength: Contains the number of bytes to encrypt
 * @param[in]  associatedDataPtr: Contains the pointer to the associated data.
 * @param[in] associatedDataLength: Contains the number of bytes of the associated data
 * @param[inout]
 *    ciphertextLengthPtr: Holds a pointer to the memory location in which the output length
 *       information is stored in bytes. On calling this function, this  parameter shall contain
 *       the size of the buffer provided by ciphertextPtr.When the request has finished, the actual
 *       length of the returned value shall be stored.
 * @param[inout]    tagLengthPtr: Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by tagPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * @param[out] ciphertextPtr: Contains the pointer to the data where the encrypted data shall be stored.
 * @param[out]   tagPtr: Contains the pointer to the data where the Tag shall be stored.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71176
 * @implements CPD-49586
 */
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
    uint32*                  tagLengthPtr);

/**
 * @brief Uses the given data to perform an AEAD Decryption and stores the ciphertext
 * and the MAC in the memory locations pointed by the ciphertext pointer and
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]  mode: Indicates which operation mode(s) to perfom
 * @param[in]  ciphertextPtr: Contains the pointer to the data to be decrypted..
 * @param[in]  ciphertextLength: Contains the number of bytes to decrypt.
 * @param[in]  associatedDataPtr: Contains the pointer to the associated data.
 * @param[in]  associatedDataLength: Contains the length in bytes of the associated data
 * @param[in]   tagPtr: Contains the pointer to the Tag to be verified.
 * @param[in]   tagLength: Contains the length in bytes of the Tag to be verified.
 * @param[inout]
 *    plaintextLengthPtr: Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by plaintextPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * @param[out]
 *    plaintextPtr: Contains the pointer to the data where the decrypted data shall be stored.
 * @param[out]  verifyPtr: Contains the pointer to the result of the verification.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71177
 * @implements CPD-49587
 */
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
    Crypto_VerifyResultType* verifyPtr);
#endif

#if (CSM_SIGNATURE_SUPPORT == STD_ON)
/**
 * @brief Uses the given data to perform the signature calculation and stores the
 * signature in the memory location pointed by the result pointer.
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]  mode: Indicates which operation mode(s) to perfom
 * @param[in]   dataPtr: Contains the pointer to the data to be signed...
 * @param[in]  dataLength: Contains the number of bytes to sign.
 * @param[inout]
 *    resultLengthPtr: Holds a pointer to the memory location in which the output
 *       length information is stored in bytes. On calling this function, this
 *       parameter shall contain the size of the buffer provided by resultPtr.
 *       When the request has finished, the actual length of the returned value shall be stored.
 * @param[out]
 *    resultPtr: Contains the pointer to the data where the signature shall be stored.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71178
 * @implements CPD-49588
 */
Std_ReturnType Csm_SignatureGenerate(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    uint8*                   resultPtr,
    uint32*                  resultLengthPtr);

/**
 * @brief Verifies the given MAC by comparing if the signature is generated with the given data.
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]   mode: Indicates which operation mode(s) to perfom
 * @param[in]   dataPtr: Contains the pointer to the data to be verified.
 * @param[in]   dataLength: Contains the number of data bytes.
 * @param[in]  signaturePtr: Holds a pointer to the signature to be verified
 * @param[in]   signatureLength: Contains the signature length in bytes.
 * @param[out]
 *    verifyPtr: Holds a pointer to the memory location, which will hold the result of the signature verification.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71179
 * @implements CPD-49589
 */
Std_ReturnType Csm_SignatureVerify(
    uint32                   jobId,
    Crypto_OperationModeType mode,
    const uint8*             dataPtr,
    uint32                   dataLength,
    const uint8*             signaturePtr,
    uint32                   signatureLength,
    Crypto_VerifyResultType* verifyPtr);
#endif

#if (CSM_RANDOM_SUPPORT == STD_ON)
/**
 * @brief Generate a random number and stores it in the memory location pointed by the result pointer
 * @synchronous Sync or Async, dependend on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[inout]
 *    resultLengthPtr: Holds a pointer to the memory location in which the result
 *       length in bytes is stored. On calling this function, this parameter shall
 *       contain the number of random bytes, which shall be stored to the buffer
 *       provided by resultPtr. When the request has finished, the actual length
 *       of the returned value shall be stored.
 * @param[out]
 *    resultPtr: Holds a pointer to the memory location which will hold the result
 *        of the random number generation.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_ENTROPY_EXHAUSTED: request failed, entropy of random number generator is exhausted.
 * @trace       CPD-71180
 * @implements CPD-49590
 */
Std_ReturnType
    Csm_RandomGenerate(uint32 jobId, Crypto_OperationModeType mode, uint8* resultPtr, uint32* resultLengthPtr);
#endif

/**
 * @brief Sets the given key element bytes to the key identified by keyId.
 * @synchronous  TRUE
 * @reentrant Non Reentrant
 * @param[in]
 *    keyId: Holds the identifier of the key for which a new material shall be set.
 * @param[in]   keyElementId: Holds the identifier of the key element to be written.
 * @param[in]   keyPtr: Holds the pointer to the key element bytes to be processed.
 * @param[in]   keyLength: Contains the number of key element bytes.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_WRITE_FAIL:Request failed because write access was denied
 *    CRYPTO_E_KEY_NOT_AVAILABLE: Request failed because the key is not available.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element size does not match size of provided data.
 * @trace       CPD-71181
 * @implements CPD-49593
 */
Std_ReturnType Csm_KeyElementSet(uint32 keyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength);

/**
 * @brief Sets the key state of the key identified by keyId to valid.
 * @synchronous  TRUE
 * @reentrant Non Reentrant
 * @param[in]
 *   keyId: Holds the identifier of the key for which a new material shall be validated.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 * @trace       CPD-71182
 * @implements CPD-49595
 */
Std_ReturnType Csm_KeySetValid(uint32 keyId);
/**
 * @brief Sets the key state of the key identified by keyId to invalid.
 * @synchronous  TRUE
 * @reentrant Non Reentrant
 * @param[in]
 *   keyId: Holds the identifier of the key for which a new material shall be invalidated.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 * @trace      CPD-71994
 * @implements CPD-49597
 */
Std_ReturnType Csm_KeySetInvalid(uint32 keyId);

/**
 * @brief Retrieves the key element bytes from a specific key element of the key
 * identified by the keyId and stores the key element in the memory location
 * pointed by the key pointer.
 * @synchronous  TRUE
 * @reentrant Reentrant
 * @param[in]
 *    keyId: Holds the identifier of the key from which a key element shall be extracted.
 * @param[in]   keyElementId: Holds the identifier of the key element to be extracted
 * @param[inout]
 *    keyLengthPtr: Holds a pointer to the memory location in which the output
 *       buffer length in bytes is stored. On calling this function, this parameter shall
 *       contain the buffer length in bytes of the keyPtr. When the request has finished,
 *       the actual size of the written input bytes shall be stored.
 * @param[out]
 *    keyPtr: Holds the pointer to the memory location where the key shall be copied to.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_AVAILABLE: request failed, the requested key element is not available
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_SMALL_BUFFER: the provided buffer is too small to store the result.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element
 * @trace       CPD-71183
 * @implements CPD-49601
 */
Std_ReturnType Csm_KeyElementGet(uint32 keyId, uint32 keyElementId, uint8* keyPtr, uint32* keyLengthPtr);

/**
 * @brief This function shall copy a key elements from one key to a target key.
 * @synchronous  TRUE
 * @reentrant Reentrant, but not for the same keyId
 * @param[in]
 *    keyId: Holds the identifier of the key whose key element shall be the source element.
 * @param[in]  keyElementId: Holds the identifier of the key element which shall be the source for the copy operation.
 * @param[in]  targetKeyId: Holds the identifier of the key whose key element shall be the destination element.
 * @param[in]    targetKeyElementId: Holds the identifier of the key element which shall be the destination for the copy
 * operation. Parameters(INOUT): NA Parameters(OUT): NA Return value: E_OK: request successful E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_AVAILABLE: request failed, the requested key element is not available
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 * @trace       CPD-71184
 * @implements CPD-49603
 */
Std_ReturnType Csm_KeyElementCopy(
    const uint32 keyId,
    const uint32 keyElementId,
    const uint32 targetKeyId,
    const uint32 targetKeyElementId);

/**
 * @brief  This function shall copy all key elements from the source key to a target key.
 * @synchronous  TRUE
 * @reentrant Reentrant, but not for the same keyId
 * @param[in]
 *    keyId: Holds the identifier of the key whose key element shall be the source element.
 * @param[in]  targetKeyId: Holds the identifier of the key whose key element shall be the destination element.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_AVAILABLE: request failed, the requested key element is not available
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 * @trace       CPD-71185
 * @implements CPD-49605
 */
Std_ReturnType Csm_KeyCopy(const uint32 keyId, const uint32 targetKeyId);

/**
 * @brief  Copies a key element to another key element in the same crypto driver. The keyElementSourceOffset
 * and keyElementCopyLength allows to copy just a part of the source key element into the destination.
 * The offset into the target key is also specified with this function.
 * @synchronous  TRUE
 * @reentrant Reentrant, but not for the same keyId
 * @param[in]
 *    keyId: Holds the identifier of the key whose key element shall be the source element for copy operation.
 * @param[in]  keyElementId: Holds the identifier of the key element which shall be the source for the copy operation..
 * @param[in]  keyElementTargetOffset: This is the offset of the destination key element indicating the start index of
 * the copy operation.
 * @param[in]  keyElementSourceOffset: This is the offset of the source key element indicating the start index of the
 * copy operation..
 * @param[in]  keyElementCopyLength: Specifies the number of bytes that shall be copied.
 * @param[in]  targetKeyId:   target  Key Id.
 * @param[in]  targetKeyElementId: Holds the identifier of the key element which shall be the destination for the copy
 * operation.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_AVAILABLE: request failed, the requested key element is not available
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 * @trace       CPD-71186
 * @implements CPD-49607
 */
Std_ReturnType Csm_KeyElementCopyPartial(
    uint32 keyId,
    uint32 keyElementId,
    uint32 keyElementSourceOffset,
    uint32 keyElementTargetOffset,
    uint32 keyElementCopyLength,
    uint32 targetKeyId,
    uint32 targetKeyElementId);

/**
 * @brief  Feeds the key element CRYPTO_KE_RANDOM_SEED with a random seed.
 * @synchronous  TRUE
 * @reentrant Reentrant, but not for the same keyId
 * @param[in]
 *    keyId: Holds the identifier of the key for which a new seed shall be generated.
 * @param[in]  seedPtr: Holds a pointer to the memory location which contains the data to feed the seed.
 * @param[in] seedLength: Contains the length of the seed in bytes.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 * @trace       CPD-71187
 * @implements CPD-49612
 */
Std_ReturnType Csm_RandomSeed(uint32 keyId, const uint8* seedPtr, uint32 seedLength);

/**
 * @brief  Generates new key material and store it in the key identified by keyId.
 * @synchronous  TRUE
 * @reentrant Reentrant, but not for the same keyId
 * @param[in]
 *    keyId: Holds the identifier of the key for which a new material shall be generated.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71188
 * @implements CPD-49614
 */
Std_ReturnType Csm_KeyGenerate(uint32 keyId);

/**
 * @brief  Derives a new key by using the key elements in the given key identified by the
 * keyId. The given key contains the key elements for the password and salt. The
 * derived key is stored in the key element with the id 1 of the key identified
 * by targetCryptoKeyId.
 * @synchronous  TRUE
 * @reentrant Reentrant, but not for the same keyId
 * @param[in]
 *    keyId: Holds the identifier of the key which is used for key derivation.
 * @param[in]  targetKeyId: Holds the identifier of the key which is used to store the derived key.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element.
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible.
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 * @trace       CPD-71189
 * @implements CPD-49616
 */
Std_ReturnType Csm_KeyDerive(uint32 keyId, uint32 targetKeyId);

/**
 * @brief  Calculates the public value of the current user for the key exchange and stores
 * the public key in the memory location pointed by the public value pointer.
 * @synchronous  TRUE
 * @reentrant Reentrant, but not for the same keyId
 * @param[in]
 *    keyId: Holds the identifier of the key which shall be used for the key exchange protocol.
 * @param[inout]
 *    publicValueLengthPtr: Holds a pointer to the memory location in which the
 *      public value length information is stored. On calling this function,
 *      this parameter shall contain the size of the buffer provided by publicValuePtr.
 *      When the request has finished, the actual length of the returned value shall be stored.
 * @param[out]
 *    publicValuePtr: Contains the pointer to the data where the public value shall be stored.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 * @trace       CPD-71190
 * @implements CPD-49619
 */
Std_ReturnType Csm_KeyExchangeCalcPubVal(uint32 keyId, uint8* publicValuePtr, uint32* publicValueLengthPtr);

/**
 * @brief  Calculates the shared secret key for the key exchange with the key material of
 * the key identified by the keyId and the partner public key. The shared secret
 * key is stored as a key element in the same key.
 * @synchronous  TRUE
 * @reentrant Reentrant, but not for the same keyId
 * @param[in]
 *    keyId: Holds the identifier of the key which shall be used for the key exchange protocol.
 * @param[in]  partnerPublicValuePtr: Holds the pointer to the memory location which contains the partner's public
 * value.
 * @param[in]   partnerPublicValueLength: Contains the length of the partner's public value in bytes.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 * @trace       CPD-71191
 * @implements CPD-49621
 */
Std_ReturnType
    Csm_KeyExchangeCalcSecret(uint32 keyId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength);

/**
 * @brief  Stores the key if necessary and sets the key state of the key identified by keyId to valid.
 * @synchronous TRUE or FALSE, depending on the job configuration
 * @reentrant Reentrant
 * @param[in] jobId: Holds the identifier of the key for which a new material shall be validated.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
 * @trace       CPD-71192
 * @implements CPD-49623
 */
Std_ReturnType Csm_JobKeySetValid(uint32 jobId);

/**
 * @brief  Stores the key if necessary and sets the key state of the key identified by keyId to invalid.
 * @synchronous TRUE or FALSE, depending on the job configuration
 * @reentrant Reentrant
 * @param[in] jobId: Holds the identifier of the key for which a new material shall be invalidated.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
 * @trace       CPD-71998
 * @implements CPD-49624
 */
Std_ReturnType Csm_JobKeySetInValid(uint32 jobId);

/**
 * @brief  This function shall dispatch the random seed function to the configured crypto driver object.
 * @synchronous TRUE or FALSE, depending on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]  keyId: Holds the identifier of the key for which a new seed shall be generated.
 * @param[in]  seedPtr: Holds a pointer to the memory location which contains the data to feed the seed..
 * @param[in]  seedLength: Contains the length of the seed in bytes.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 * @trace       CPD-71193
 * @implements CPD-49625
 */
Std_ReturnType Csm_JobRandomSeed(uint32 jobId, uint32 keyId, const uint8* seedPtr, uint32 seedLength);

/**
 * @brief  Generates new key material and stores it in the key identified by keyId.
 * @synchronous TRUE or FALSE, depending on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]    keyId: Holds the identifier of the key for which a new material shall be generated.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71194
 * @implements CPD-49626
 */
Std_ReturnType Csm_JobKeyGenerate(uint32 jobId, uint32 keyId);

/**
 * @brief  Derives a new key by using the key elements in the given key identified by the keyId.
 * The given key contains the key elements for the password and salt. The derived key is
 * stored in the key element with the id 1 of the key identified by targetCryptoKeyId.
 * @synchronous TRUE or FALSE, depending on the job configuration
 * @reentrant Reentrant
 * @param[in]
 *    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]   keyId: Holds the identifier of the key which is used for key derivation.
 * @param[in]    targetKeyId: Holds the identifier of the key which is used to store the derived key.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
 *    CRYPTO_E_KEY_READ_FAIL: Request failed, not allowed to extract key element
 *    CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71195
 * @implements CPD-49627
 */
Std_ReturnType Csm_JobKeyDerive(uint32 jobId, uint32 keyId, uint32 targetKeyId);

/**
 * @brief  Calculates the public value of the current user for the key exchange and stores the public key
 * in the memory location pointed by the public value pointer.
 * @synchronous TRUE or FALSE, depending on the job configuration
 * @reentrant Reentrant
 * @param[in]    jobId: Holds the identifier of the job using the CSM service.
 * @param[in]    publicValuePtr: Contains the pointer to the data where the public value shall be stored.
 * @param[out]    publicValueLengthPtr: Holds a pointer to the memory location in which the public value
 *      length information is stored.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 * @trace       CPD-71196
 * @implements CPD-49628
 */
Std_ReturnType Csm_JobKeyExchangeCalcPubVal(uint32 jobId, uint8* publicValuePtr, uint32* publicValueLengthPtr);

/**
 * @brief  Calculates the shared secret key for the key exchange with the key material of the key
 * identified by the keyId and the partner public key. The shared secret key is stored as
 * a key element in the same key.
 * @synchronous TRUE or FALSE, depending on the job configuration
 * @reentrant Reentrant
 * @param[in]   jobId: Holds the identifier of the job using the CSM service.
 * @param[in]   partnerPublicValuePtr: Holds the pointer to the memory location which contains the partner's public
 * value.
 * @param[in]   partnerPublicValueLength: Contains the length of the partner's public value in bytes.
 * @retval
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    E_BUSY:Request failed, Crypto Driver Object is busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 * @trace       CPD-71197
 * @implements CPD-49629
 */
Std_ReturnType
    Csm_JobKeyExchangeCalcSecret(uint32 jobId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength);

/**
 * @brief  Cancels the job processing from asynchronous or streaming jobs.
 * @synchronous  TRUE
 * @reentrant Non Reentrant
 * @param[in]
 *    job: Holds the identifier of the job to be canceled
 * @param[in]    mode: Not used, just for interface compatibility provided.
 * @retval
 * E_OK: Request successful. Job removed from any queue and potentially from crypto driver hardware.
 * E_NOT_OK: Request failed
 * CRYPTO_E_JOB_CANCELED: Immediate cancelation not possible.The cancelation will be done at next
 * suitable processing step and notified via a negative finish callback.
 * @trace       CPD-71198
 * @implements CPD-49634
 */
Std_ReturnType Csm_CancelJob(uint32 job, Crypto_OperationModeType mode);

/**
 * @brief Retrieves the status of a key in the CSM module.
 *
 * This function retrieves the status of a specified key in the CSM module. It takes the key ID and a pointer
 * to a variable where the key status will be stored.
 *
 * @synchronous  TRUE
 * @reentrant Non Reentrant
 * @param[in]  keyId  :           Identifier of the key.
 * @param[out] keyStatusPtr:      Pointer to the variable where the key status will be stored.
 *
 * @return Standard return type indicating the result of the operation.
 *         - E_OK: Operation successful.
 *         - E_NOT_OK: Operation failed.
 *
 * @retval E_OK  :        Operation successful.
 * @retval E_NOT_OK:      Operation failed.
 * @trace       CPD-71199
 * @implements CPD-49599
 */
Std_ReturnType Csm_KeyGetStatus(uint32 keyId, Crypto_KeyStatusType* keyStatusPtr);

/**
 * @brief Custom service operation for the CSM module.
 *
 * This function performs a custom service operation in the CSM module. It takes various parameters such as job ID,
 * operation mode, key IDs, input and output pointers, and lengths to perform the specified cryptographic operation.
 * It also handles verification results if required.
 *
 * @synchronous Depends on configuration
 * @reentrant Reentrant for different jobId
 * @param[in]  jobId :                    Identifier of the job.
 * @param[in]  mode   :                    Operation mode (e.g., encryption, decryption, signing, verification).
 * @param[in]  targetKeyId:               Identifier of the target key.
 * @param[in]  inputPtr    :              Pointer to the primary input data.
 * @param[in]  inputLength :              Length of the primary input data.
 * @param[in]  secondaryInputPtr:         Pointer to the secondary input data.
 * @param[in]  secondaryInputLength:      Length of the secondary input data.
 * @param[in]  tertiaryInputPtr  :        Pointer to the tertiary input data.
 * @param[in]  tertiaryInputLength:       Length of the tertiary input data.
 * @param[out] outputPtr    :             Pointer to the output data.
 * @param[out] outputLengthPtr:           Pointer to the length of the output data.
 * @param[out] secondaryOutputPtr:        Pointer to the secondary output data.
 * @param[out] secondaryOutputLengthPtr:  Pointer to the length of the secondary output data.
 * @param[out] verifyPtr  :               Pointer to the verification result.
 *
 * @return Standard return type indicating the result of the operation.
 *         - E_OK: Operation successful.
 *         - E_NOT_OK: Operation failed.
 *
 * @retval E_OK   :       Operation successful.
 * @retval E_NOT_OK:      Operation failed.
 * @trace       CPD-71200
 * @implements CPD-49639
 */
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
    Crypto_VerifyResultType* verifyPtr);

/**
 * @brief Custom synchronous operation for the CSM module.
 * @synchronous  TRUE
 * @reentrant Non Reentrant
 *
 * This function performs a custom synchronous operation in the CSM module. It takes various parameters
 * such as dispatch ID, key IDs, element IDs, input and output pointers, and lengths to perform the operation.
 *
 * @param[in]  dispatchId  :      Dispatch ID for the operation.
 * @param[in]  keyId      :       Identifier of the source key.
 * @param[in]  keyElementId:      Identifier of the source key element.
 * @param[in]  targetKeyId :      Identifier of the target key.
 * @param[in]  targetKeyElementId: Identifier of the target key element.
 * @param[in]  inputPtr :         Pointer to the input data.
 * @param[in]  inputLength:       Length of the input data.
 * @param[out] outputPtr:         Pointer to the output data.
 * @param[out] outputLengthPtr:   Pointer to the length of the output data.
 * @param[out] secondaryOutputPtr: Pointer to the secondary output data.
 * @param[out] secondaryOutputLengthPtr: Pointer to the length of the secondary output data.
 *
 * @return Standard return type indicating the result of the operation.
 *         - E_OK: Operation successful.
 *         - E_NOT_OK: Operation failed.
 *
 * @retval E_OK  :        Operation successful.
 * @retval E_NOT_OK:      Operation failed.
 * @trace       CPD-71201
 * @implements CPD-49640
 */
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
    uint32*      secondaryOutputLengthPtr);

/**
 * @brief Saves the context of a job in the CSM module.
 *
 * @synchronous Depends on configuration
 * @reentrant  Reentrant
 *
 * This function saves the context of a specified job in the CSM module. It takes the job ID, a pointer to the context
 * buffer, and a pointer to the context buffer length. The function writes the context data to the provided buffer and
 * updates the length of the context data.
 *
 * @param[in]  jobId:                  Identifier of the job.
 * @param[out] contextBufferPtr :      Pointer to the buffer where the context data will be stored.
 * @param[out] contextBufferLengthPtr: Pointer to the variable where the length of the context data will be stored.
 *
 * @return Standard return type indicating the result of the operation.
 *         - E_OK: Operation successful.
 *         - E_NOT_OK: Operation failed.
 *
 * @retval E_OK :         Operation successful.
 * @retval E_NOT_OK:      Operation failed.
 * @trace       CPD-71202
 * @implements CPD-49630
 */
Std_ReturnType Csm_SaveContextJob(uint32 jobId, uint8* contextBufferPtr, uint32* contextBufferLengthPtr);

/**
 * @brief Restores the context of a job in the CSM module.
 *
 * @synchronous Depends on configuration
 * @reentrant  Reentrant
 * This function restores the context of a specified job in the CSM module. It takes the job ID, a pointer to the
 * context buffer, and the length of the context buffer. The function reads the context data from the provided buffer
 * and restores it for the job.
 *
 * @param[in] jobId   :               Identifier of the job.
 * @param[in] contextBufferPtr:       Pointer to the buffer containing the context data.
 * @param[in] contextBufferLength:    Length of the context data in the buffer.
 *
 * @return Standard return type indicating the result of the operation.
 *         - E_OK: Operation successful.
 *         - E_NOT_OK: Operation failed.
 *
 * @retval E_OK  :        Operation successful.
 * @retval E_NOT_OK:      Operation failed.
 * @trace       CPD-71203
 * @implements CPD-49632
 */
Std_ReturnType Csm_RestoreContextJob(uint32 jobId, const uint8* contextBufferPtr, uint32 contextBufferLength);

#ifdef __cplusplus
}
#endif

#endif /* CSM_H */
