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
 **  @file               : Crypto_62.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V1.0.0      2024-07-13   jie.gu         initial versionR
 *
 ==================================================================================================================== */
/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:BswM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_Crypto_62_1336
      Reason: Defining function pointer types
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Crypto_62_3457
      Reason: Macro defines a braced initializer
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Crypto_62_1271
      Reason: non-init expression for enum definition is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Crypto_62_3412
      Reason: Macro defines an unrecognized code-fragment
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Crypto_62_3672
      Reason: function pointer needs to be assigned depends on configuration
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Crypto_62_General
      Reason: Algorithm-related, not processed for the time being
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STCYC
      Reason: Functionality is not separated into subfunctions because there are higher requirements for minimizing the
    stack and applying it to runtime use of the code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STLIN
      Reason: Some function logics are overly complex. Splitting and implementing them will lead to reduced performance.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STPAR
      Reason: The parameters of the function need.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STST3
      Reason: Some function logics are overly complex. Splitting and implementing them will lead to reduced performance.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STM19
      Reason: In principle, a function is not allowed to have multiple exit points,
    but it can help improve code performance in special circumstances.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STCAL
      Reason: The standard compliance vote was higher than the measurement threshold. In addition, a typical way to
    reduce STCAL is to deepen function nesting, which increases call stack usage and runtime
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STPTH
      Reason: The functions were not separated into sub-functions because there were higher requirements for
    minimizing the stack and applying them to the runtime usage of the code
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STVAR
      Reason: The total number of variables is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_Crypto_62_STTPP
      Reason: The number of lines of code is related to the user configuration, allowing
    dynamic code to violate this under extreme conditions
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Crypto_62_2982
      Reason: The operation is not redundant, because it must be emptied.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Crypto_62_3408
      Reason: There is no declaration before the definition.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_Crypto_62_1502
      Reason: Configuration variables design needs.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

 */

#ifndef CRYPTO_H
#define CRYPTO_H

/* =================================================== inclusions =================================================== */

#include "Csm_Types.h"
#include "CryIf.h"
#include "Crypto_62_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
#define CRYPTO_VENDOR_ID          27U        /**< Vendor ID for the Crypto module @range 27U */
#define CRYPTO_C_AR_MAJOR_VERSION 4U         /**< Major AUTOSAR version for the Crypto module @range 4U */
#define CRYPTO_C_AR_MINOR_VERSION 9U         /**< Minor AUTOSAR version for the Crypto module @range 8U */
#define CRYPTO_C_AR_PATCH_VERSION 0U         /**< Patch AUTOSAR version for the Crypto module @range 0U */
#define CRYPTO_C_SW_MAJOR_VERSION 1U         /**< Major software version for the Crypto module @range 1U */
#define CRYPTO_C_SW_MINOR_VERSION 0U         /**< Minor software version for the Crypto module @range 0U */
#define CRYPTO_C_SW_PATCH_VERSION 0U         /**< Patch software version for the Crypto module @range 0U */
#define CRYPTO_PRODUCT_ID         51U        /**< Product ID for the Crypto module @range 51U */
#define CRYPTO_MODULE_ID          114U       /**< Module ID for the Crypto module @range 114U */
#define CRYPTO_MODULE_INSTANCE_ID ((uint8)0) /**< Module instance ID for the Crypto module @range 0 */

/* ===================================================== macros ===================================================== */
/* Runtime errors */
#define CRYPTO_E_RE_KEY_NOT_AVAILABLE ((uint8)0x00U) /**< Error code for key not available @range 0x00U */
#define CRYPTO_E_RE_KEY_READ_FAIL     ((uint8)0x01U) /**< Error code for key read failure @range 0x01U */
#define CRYPTO_E_RE_ENTROPY_EXHAUSTED ((uint8)0x03U) /**< Error code for entropy exhausted @range 0x03U */
#define CRYPTO_E_RE_NVM_ACCESS_FAILED ((uint8)0x04U) /**< Error code for NVM access failure @range 0x04U */

/* Service IDs */
#define CRYPTO_SID_INIT           ((uint8)0x00U) /**< Service ID for initialization @range 0x00U */
#define CRYPTO_SID_GETVERSIONINFO ((uint8)0x01U) /**< Service ID for getting version information @range 0x01U */
#define CRYPTO_SID_PROCESSJOB     ((uint8)0x03U) /**< Service ID for processing a job @range 0x03U */
#define CRYPTO_SID_KEYELEMENTSET  ((uint8)0x04U) /**< Service ID for setting a key element @range 0x04U */
#define CRYPTO_SID_KEYSETVALID    ((uint8)0x05U) /**< Service ID for setting a key as valid @range 0x05U */
#define CRYPTO_SID_KEYELEMENTGET  ((uint8)0x06U) /**< Service ID for getting a key element @range 0x06U */
#define CRYPTO_SID_KEYGENERATE    ((uint8)0x07U) /**< Service ID for generating a key @range 0x07U */
#define CRYPTO_SID_KEYDERIVE      ((uint8)0x08U) /**< Service ID for deriving a key @range 0x08U */
#define CRYPTO_SID_KEYEXCHANGECALCPUBVAL \
    ((uint8)0x09U) /**< Service ID for calculating the public value in key exchange @range 0x09U */
#define CRYPTO_SID_KEYEXCHANGECALCSECRET \
    ((uint8)0x0AU) /**< Service ID for calculating the secret in key exchange @range 0x0AU */
#define CRYPTO_SID_CERTIFICATE_PARSE  ((uint8)0x0BU) /**< Service ID for parsing a certificate @range 0x0BU */
#define CRYPTO_SID_RANDOMSEED         ((uint8)0x0DU) /**< Service ID for seeding the random number generator @range 0x0DU */
#define CRYPTO_SID_CANCELJOB          ((uint8)0x0EU) /**< Service ID for canceling a job @range 0x0EU */
#define CRYPTO_SID_KEYELEMENTCOPY     ((uint8)0x0FU) /**< Service ID for copying a key element @range 0x0FU */
#define CRYPTO_SID_KEYCOPY            ((uint8)0x10U) /**< Service ID for copying a key @range 0x10U */
#define CRYPTO_SID_KEYELEMEMTIDSGET   ((uint8)0x11U) /**< Service ID for getting key element IDs @range 0x11U */
#define CRYPTO_SID_CERTIFICATE_VERIFY ((uint8)0x12U) /**< Service ID for verifying a certificate @range 0x12U */
#define CRYPTO_SID_KEYELEMENTCOPYPARTIAL \
    ((uint8)0x13U)                              /**< Service ID for partially copying a key element @range 0x13U */
#define CRYPTO_SID_KEYGETSTATUS  ((uint8)0x14U) /**< Service ID for getting the status of a key @range 0x14U */
#define CRYPTO_SID_KEYSETINVALID ((uint8)0x15U) /**< Service ID for setting a key as invalid @range 0x15U */
#define CRYPTO_SID_CustomSync    ((uint8)0x1aU) /**< Service ID for custom synchronization @range 0x1aU */

/* ================================================ type definitions ================================================ */
/**
 * @brief Configuration type
 */
typedef uint8 Crypto_62_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief          Initializes the Crypto_62 module.
 * @param[in]      configPtr : Pointer to the Crypto_62 configuration structure.
 * @return         None
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71637
 */
void Crypto_62_Init(const Crypto_62_ConfigType* configPtr);

/**
 * @brief          Retrieves the version information of the Crypto_62 module.
 * @param[out]     versioninfo : Pointer to the version information structure.
 * @return         None
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace          CPD-71638
 */
#if (CRYPTO_VERSION_INFO_API == STD_ON)
/* @req SWS_Crypto_910011,SRS_BSW_00407 */
void Crypto_62_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief          Processes a cryptographic job.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      job : Pointer to the cryptographic job structure.
 * @return         Std_ReturnType
 * @retval         E_OK: The job was processed successfully.
 * @retval         E_NOT_OK: The job could not be processed due to an error.
 * @retval         CRYPTO_E_BUSY: The cryptographic object is busy.
 * @retval         CRYPTO_E_PARAM_HANDLE: The job's service or key ID is invalid.
 * @retval         CRYPTO_E_PARAM_POINTER: The job pointer is NULL.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71639
 */
Std_ReturnType Crypto_62_ProcessJob(uint32 objectId, Crypto_JobType* job);

/**
 * @brief          Cancels a cryptographic job.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      job : Pointer to the cryptographic job structure.
 * @return         Std_ReturnType
 * @retval         E_OK: The job was canceled successfully.
 * @retval         E_NOT_OK: The job could not be canceled due to an error.
 * @retval         CRYPTO_E_JOB_CANCELED: The job was canceled and the callback was notified.
 * @retval         CRYPTO_E_PARAM_HANDLE: The object ID is invalid.
 * @retval         CRYPTO_E_PARAM_POINTER: The job pointer is NULL.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71640
 */
Std_ReturnType Crypto_62_CancelJob(uint32 objectId, Crypto_JobType* job);

/**
 * @brief          Sets a key element for a cryptographic key.
 * @param[in]      cryptokeyId : The ID of the cryptographic key.
 * @param[in]      keyElementId : The ID of the key element to set.
 * @param[in]      keyPtr : Pointer to the key data.
 * @param[in]      keyLength : The length of the key data.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element was set successfully.
 * @retval         E_NOT_OK: The key element could not be set due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_PARAM_POINTER: The key pointer is NULL.
 * @retval         CRYPTO_E_PARAM_VALUE: The key length is zero.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71641
 */
Std_ReturnType Crypto_62_KeyElementSet(uint32 cryptokeyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength);

/**
 * @brief          Sets a cryptographic key to a valid state.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key.
 * @return         Std_ReturnType
 * @retval         E_OK: The key was set to a valid state successfully.
 * @retval         E_NOT_OK: The key could not be set to a valid state due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @retval         CRYPTO_E_RE_NVM_ACCESS_FAILED: Access to the NVM block failed.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71642
 */
Std_ReturnType Crypto_62_KeySetValid(uint32 cryptoKeyId);

/**
 * @brief          Sets a cryptographic key to an invalid state.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key.
 * @return         Std_ReturnType
 * @retval         E_OK: The key was set to an invalid state successfully.
 * @retval         E_NOT_OK: The key could not be set to an invalid state due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71643
 */
Std_ReturnType Crypto_62_KeySetInValid(uint32 cryptoKeyId);

/**
 * @brief          Retrieves a key element from a cryptographic key.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key.
 * @param[in]      keyElementId : The ID of the key element to retrieve.
 * @param[out]     resultPtr : Pointer to the buffer where the key element will be stored.
 * @param[in,out]  resultLengthPtr : Pointer to the length of the buffer. On return, it contains the actual length of
 * the key element.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element was retrieved successfully.
 * @retval         E_NOT_OK: The key element could not be retrieved due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid or the key is not valid.
 * @retval         CRYPTO_E_PARAM_POINTER: The result pointer or result length pointer is NULL.
 * @retval         CRYPTO_E_PARAM_VALUE: The result length is zero.
 * @retval         CRYPTO_E_SMALL_BUFFER: The result buffer is too small.
 * @retval         CRYPTO_E_KEY_READ_FAIL: The key element cannot be read due to access restrictions.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71644
 */
Std_ReturnType
    Crypto_62_KeyElementGet(uint32 cryptoKeyId, uint32 keyElementId, uint8* resultPtr, uint32* resultLengthPtr);

/**
 * @brief          Retrieves the status of a cryptographic key.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key.
 * @param[out]     keyStatusPtr : Pointer to the variable where the key status will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The key status was retrieved successfully.
 * @retval         E_NOT_OK: The key status could not be retrieved due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_PARAM_POINTER: The key status pointer is NULL.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71645
 */
Std_ReturnType Crypto_62_KeyGetStatus(uint32 cryptoKeyId, Crypto_KeyStatusType* keyStatusPtr);

/**
 * @brief          Copies a key element from one cryptographic key to another.
 * @param[in]      cryptoKeyId : The ID of the source cryptographic key.
 * @param[in]      keyElementId : The ID of the key element to copy from the source key.
 * @param[in]      targetCryptoKeyId : The ID of the target cryptographic key.
 * @param[in]      targetKeyElementId : The ID of the key element to copy to in the target key.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element was copied successfully.
 * @retval         E_NOT_OK: The key element could not be copied due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The source or target cryptographic key ID is invalid.
 * @retval         CRYPTO_E_KEY_READ_FAIL: The source key element cannot be read due to access restrictions.
 * @retval         CRYPTO_E_KEY_WRITE_FAIL: The target key element cannot be written due to access restrictions.
 * @retval         CRYPTO_E_KEY_SIZE_MISMATCH: The source and target key elements have different sizes.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71646
 */
Std_ReturnType Crypto_62_KeyElementCopy(
    uint32 cryptoKeyId,
    uint32 keyElementId,
    uint32 targetCryptoKeyId,
    uint32 targetKeyElementId);

/**
 * @brief          Copies a partial key element from one cryptographic key to another.
 * @param[in]      cryptoKeyId : The ID of the source cryptographic key.
 * @param[in]      keyElementId : The ID of the key element to copy from the source key.
 * @param[in]      keyElementSourceOffset : The offset in the source key element where the copy starts.
 * @param[in]      keyElementTargetOffset : The offset in the target key element where the copy starts.
 * @param[in]      keyElementCopyLength : The length of the data to be copied.
 * @param[in]      targetCryptoKeyId : The ID of the target cryptographic key.
 * @param[in]      targetKeyElementId : The ID of the key element to copy to in the target key.
 * @return         Std_ReturnType
 * @retval         E_OK: The partial key element was copied successfully.
 * @retval         E_NOT_OK: The partial key element could not be copied due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The source or target cryptographic key ID is invalid.
 * @retval         CRYPTO_E_KEY_READ_FAIL: The source key element cannot be read due to access restrictions.
 * @retval         CRYPTO_E_KEY_WRITE_FAIL: The target key element cannot be written due to access restrictions.
 * @retval         CRYPTO_E_KEY_SIZE_MISMATCH: The source and target key elements have different sizes or the copy
 * length exceeds the element size.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71647
 */
Std_ReturnType Crypto_62_KeyElementCopyPartial(
    uint32 cryptoKeyId,
    uint32 keyElementId,
    uint32 keyElementSourceOffset,
    uint32 keyElementTargetOffset,
    uint32 keyElementCopyLength,
    uint32 targetCryptoKeyId,
    uint32 targetKeyElementId);

/**
 * @brief          Copies all key elements from one cryptographic key to another.
 * @param[in]      cryptoKeyId : The ID of the source cryptographic key.
 * @param[in]      targetCryptoKeyId : The ID of the target cryptographic key.
 * @return         Std_ReturnType
 * @retval         E_OK: All key elements were copied successfully.
 * @retval         E_NOT_OK: The key elements could not be copied due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The source or target cryptographic key ID is invalid.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71648
 */
Std_ReturnType Crypto_62_KeyCopy(uint32 cryptoKeyId, uint32 targetCryptoKeyId);

/**
 * @brief          Retrieves the IDs and lengths of all key elements for a cryptographic key.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key.
 * @param[out]     keyElementIdsPtr : Pointer to the array where the key element IDs will be stored.
 * @param[out]     keyElementIdsLengthPtr : Pointer to the array where the key element lengths will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element IDs and lengths were retrieved successfully.
 * @retval         E_NOT_OK: The key element IDs and lengths could not be retrieved due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71649
 */
Std_ReturnType Crypto_62_KeyElementIdsGet(uint32 cryptoKeyId, uint32* keyElementIdsPtr, uint32* keyElementIdsLengthPtr);

/**
 * @brief          Seeds the random number generator with a given seed.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key associated with the random number generator.
 * @param[in]      seedPtr : Pointer to the seed data.
 * @param[in]      seedLength : The length of the seed data.
 * @return         Std_ReturnType
 * @retval         E_OK: The random number generator was seeded successfully.
 * @retval         E_NOT_OK: The random number generator could not be seeded due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_PARAM_POINTER: The seed pointer is NULL.
 * @retval         CRYPTO_E_PARAM_VALUE: The seed length is zero.
 * @retval         CRYPTO_E_KEY_NOT_VALID: The cryptographic key is not valid.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71650
 */
Std_ReturnType Crypto_62_RandomSeed(uint32 cryptoKeyId, const uint8* seedPtr, uint32 seedLength);

/**
 * @brief          Generates a cryptographic key.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key to generate.
 * @return         Std_ReturnType
 * @retval         E_OK: The key was generated successfully.
 * @retval         E_NOT_OK: The key could not be generated due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71651
 */
Std_ReturnType Crypto_62_KeyGenerate(uint32 cryptoKeyId);

/**
 * @brief          Derives a cryptographic key from another key.
 * @param[in]      cryptoKeyId : The ID of the source cryptographic key.
 * @param[in]      targetCryptoKeyId : The ID of the target cryptographic key to derive.
 * @return         Std_ReturnType
 * @retval         E_OK: The key was derived successfully.
 * @retval         E_NOT_OK: The key could not be derived due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The source or target cryptographic key ID is invalid.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71652
 */
Std_ReturnType Crypto_62_KeyDerive(uint32 cryptoKeyId, uint32 targetCryptoKeyId);

/**
 * @brief          Calculates the public value for a key exchange operation.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key used for the key exchange.
 * @param[out]     publicValuePtr : Pointer to the buffer where the public value will be stored.
 * @param[in,out]  publicValueLengthPtr : Pointer to the length of the public value buffer. On return, it contains the
 * actual length of the public value.
 * @return         Std_ReturnType
 * @retval         E_OK: The public value was calculated successfully.
 * @retval         E_NOT_OK: The public value could not be calculated due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_PARAM_POINTER: The public value pointer or public value length pointer is NULL.
 * @retval         CRYPTO_E_PARAM_VALUE: The public value length is zero.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71653
 */
Std_ReturnType Crypto_62_KeyExchangeCalcPubVal(uint32 cryptoKeyId, uint8* publicValuePtr, uint32* publicValueLengthPtr);

/**
 * @brief          Calculates the shared secret for a key exchange operation.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key used for the key exchange.
 * @param[in]      partnerPublicValuePtr : Pointer to the partner's public value.
 * @param[in]      partnerPublicValueLength : The length of the partner's public value.
 * @return         Std_ReturnType
 * @retval         E_OK: The shared secret was calculated successfully.
 * @retval         E_NOT_OK: The shared secret could not be calculated due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The cryptographic key ID is invalid.
 * @retval         CRYPTO_E_PARAM_POINTER: The partner's public value pointer is NULL.
 * @retval         CRYPTO_E_PARAM_VALUE: The partner's public value length is zero.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71654
 */
Std_ReturnType Crypto_62_KeyExchangeCalcSecret(
    uint32       cryptoKeyId,
    const uint8* partnerPublicValuePtr,
    uint32       partnerPublicValueLength);

/**
 * @brief          Performs a custom synchronous cryptographic operation.
 * @param[in]      dispatchId : The ID of the dispatch table entry.
 * @param[in]      keyId : The ID of the source cryptographic key.
 * @param[in]      keyElementId : The ID of the source key element.
 * @param[in]      targetKeyId : The ID of the target cryptographic key.
 * @param[in]      targetKeyElementId : The ID of the target key element.
 * @param[in]      inputPtr : Pointer to the input data.
 * @param[in]      inputLength : The length of the input data.
 * @param[out]     outputPtr : Pointer to the buffer where the output data will be stored.
 * @param[in,out]  outputLengthPtr : Pointer to the length of the output buffer. On return, it contains the actual
 * length of the output data.
 * @param[out]     secondaryOutputPtr : Pointer to the buffer where the secondary output data will be stored.
 * @param[in,out]  secondaryOutputLengthPtr : Pointer to the length of the secondary output buffer. On return, it
 * contains the actual length of the secondary output data.
 * @return         Std_ReturnType
 * @retval         E_OK: The operation was performed successfully.
 * @retval         E_NOT_OK: The operation could not be performed due to an error.
 * @retval         CRYPTO_E_PARAM_HANDLE: The target cryptographic key ID is invalid.
 * @retval         CRYPTO_E_UNINIT: The Crypto_62 module is not initialized.
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71655
 */
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
    uint32*      secondaryOutputLengthPtr);

#ifdef __cplusplus
}
#endif

#endif
