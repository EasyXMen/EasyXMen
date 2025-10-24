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
 **  @file               : CryIf.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : CryIf Interface Public Implementation header
 **
 **********************************************************************************************************************/

/** ===================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date       Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.2.1    2025-03-31 jie.gu         inittial version for R23-11
 *
 *
 ==================================================================================================================== */
#ifndef CRYIF_H
#define CRYIF_H

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:BswM<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_CryIf_3432
      Reason: Function-like macros are used to allow more efficient code.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CryIf_3218
      Reason: file scoped variables only used in one function are necessary for functional usage.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CryIf_3229
      Reason: The initialization state should not be masked as it is used to indicate the
    module's initialization status.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CryIf_1252
      Reason: suffixed constant comparison is necessary
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CryIf_2981
      Reason: Init may be necessary for return variant.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CryIf_STM19
      Reason: In principle, a function is not allowed to have multiple exit points,
    but it can help improve code performance in special circumstances.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_CryIf_1258
      Reason: The cast conversion from a constant is safe.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CryIf_STPAR
      Reason: The parameters of the function need.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CryIf_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */

/* =================================================== inclusions =================================================== */
#include "CryIf_Cfg.h"
#include "Csm_Types.h"
#include "Std_Types.h"
#include "Rte_Csm_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/* Module identification constants */
#define CRYIF_MODULE_ID   112U /**< Module ID for CRYIF @range 112U */
#define CRYIF_VENDOR_ID   62U  /**< Vendor ID for CRYIF @range 62U */
#define CRYIF_INSTANCE_ID 0U   /**< Instance ID for CRYIF @range 0U */

/* Module version constants */
#define CRYIF_H_AR_MAJOR_VERSION 4U /**< Major AUTOSAR version for CRYIF @range 4U */
#define CRYIF_H_AR_MINOR_VERSION 9U /**< Minor AUTOSAR version for CRYIF @range 9U */
#define CRYIF_H_AR_PATCH_VERSION 0U /**< Patch AUTOSAR version for CRYIF @range 0U */
#define CRYIF_H_SW_MAJOR_VERSION 2U /**< Major software version for CRYIF @range 2U */
#define CRYIF_H_SW_MINOR_VERSION 2U /**< Minor software version for CRYIF @range 2U */
#define CRYIF_H_SW_PATCH_VERSION 0U /**< Patch software version for CRYIF @range 0U */

/* ===================================================== macros ===================================================== */
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
/* Development errors used by the CRYIF module */
#define CRYIF_E_UNINIT            (0x00u) /**< Error code for uninitialized state @range 0x00 */
#define CRYIF_E_INIT_FAILED       (0x01u) /**< Error code for initialization failure @range 0x01 */
#define CRYIF_E_PARAM_POINTER     (0x02u) /**< Error code for invalid parameter pointer @range 0x02 */
#define CRYIF_E_PARAM_HANDLE      (0x03u) /**< Error code for invalid parameter handle @range 0x03 */
#define CRYIF_E_PARAM_VALUE       (0x04u) /**< Error code for invalid parameter value @range 0x04 */
#define CRYIF_E_KEY_SIZE_MISMATCH (0x05u) /**< Error code for key size mismatch @range 0x05 */

/* Service IDs for CRYIF module */
#define CRYIF_SID_INIT                   (0x00u) /**< Service ID for initialization @range 0x00 */
#define CRYIF_SID_GETVERSIONINFO         (0x01u) /**< Service ID for getting version information @range 0x01 */
#define CRYIF_SID_PROCESSJOB             (0x03u) /**< Service ID for processing a job @range 0x03 */
#define CRYIF_SID_CANCELJOB              (0x0Eu) /**< Service ID for canceling a job @range 0x0E */
#define CRYIF_SID_KEYELEMENT_SET         (0x04u) /**< Service ID for setting a key element @range 0x04 */
#define CRYIF_SID_KEYSETVALID            (0x05u) /**< Service ID for setting a key as valid @range 0x05 */
#define CRYIF_SID_KEYELEMENT_GET         (0x06u) /**< Service ID for getting a key element @range 0x06 */
#define CRYIF_SID_KEYELEMENT_COPY        (0x0Fu) /**< Service ID for copying a key element @range 0x0F */
#define CRYIF_SID_KEYELEMENT_COPYPARTIAL (0x12u) /**< Service ID for partially copying a key element @range 0x12 */
#define CRYIF_SID_KEYCOPY                (0x10u) /**< Service ID for copying a key @range 0x10 */
#define CRYIF_SID_RANDOM_SEED            (0x07u) /**< Service ID for seeding the random number generator @range 0x07 */
#define CRYIF_SID_KEYGENERATE            (0x08u) /**< Service ID for generating a key @range 0x08 */
#define CRYIF_SID_KEYDERIVE              (0x09u) /**< Service ID for deriving a key @range 0x09 */
#define CRYIF_SID_KEYEXCHANGECALCPUBVAL \
    (0x0Au) /**< Service ID for calculating the public value in key exchange @range 0x0A */
#define CRYIF_SID_KEYEXCHANGECALCSECRET \
    (0x0Bu)                              /**< Service ID for calculating the secret in key exchange @range 0x0B */
#define CRYIF_SID_CBNOTIFICATION (0x0Du) /**< Service ID for callback notification @range 0x0D */
#define CRYIF_SID_KEYSETINVALID  (0x0Eu)
#endif

/* Driver support constants */
#define CRYIF_SINGLE_DRIVER (0U) /**< Flag indicating single driver support @range 0U */

/* Constants */
#define CRYIF_CONSTANT_2 ((uint32)(0x2U)) /**< Constant value 2 @range 0x2U */
#if !defined(CRYIF_LOCAL)
#define CRYIF_LOCAL static /* PRQA S 3414 */ /* VL_QAC_3414 */
#endif

/* ================================================ type definitions ================================================ */

/**
 * @brief Type definition for CryIf configuration.
 *
 * This type is used to define the configuration for the CryIf module.
 *
 * @typedef CryIf_ConfigType
 */
typedef uint8 CryIf_ConfigType; /**< Configuration type for CryIf @range any valid uint8 value */

/**
 * @brief Function pointer configuration structure for Crypto operations.
 *
 * This structure contains function pointers to various Crypto operations.
 *
 * @typedef CryIf_FuncNameConfigType
 */
typedef struct CryIf_FuncNameConfigTag
{
    /**
     * @brief Function pointer to the Crypto_ProcessJob function.
     *
     */
    Std_ReturnType (*Crypto_ProcessJob_Name)(uint32 channelId, Crypto_JobType* job);

    /**
     * @brief Function pointer to the Crypto_CancelJob function.
     *
     */
    Std_ReturnType (*Crypto_CancelJob_Name)(uint32 channelId, Crypto_JobType* job);

    /**
     * @brief Function pointer to the Crypto_KeyElementSet function.
     *
     */
    Std_ReturnType (
        *Crypto_KeyElementSet_Name)(uint32 cryIfKeyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength);

    /**
     * @brief Function pointer to the Crypto_KeySetValid function.
     *
     */
    Std_ReturnType (*Crypto_KeySetValid_Name)(uint32 cryIfKeyId);
    /**
     * @brief Function pointer to the Crypto_KeySetInValid function.
     *
     */
    Std_ReturnType (*Crypto_KeySetInValid_Name)(uint32 cryIfKeyId);

    /**
     * @brief Function pointer to the Crypto_KeyElementGet function.
     *
     */
    Std_ReturnType (
        *Crypto_KeyElementGet_Name)(uint32 cryIfKeyId, uint32 keyElementId, uint8* resultPtr, uint32* resultLengthPtr);

    /**
     * @brief Function pointer to the Crypto_KeyElementCopy function.
     *
     */
    Std_ReturnType (*Crypto_KeyElementCopy_Name)(
        uint32 cryptoKeyId,
        uint32 keyElementId,
        uint32 targetCryptoKeyId,
        uint32 targetKeyElementId);

    /**
     * @brief Function pointer to the Crypto_KeyElementCopyPartial function.
     *
     */
    Std_ReturnType (*Crypto_KeyElementCopyPartial_Name)(
        uint32 cryIfKeyId,
        uint32 keyElementId,
        uint32 keyElementSourceOffset,
        uint32 keyElementTargetOffset,
        uint32 keyElementCopyLength,
        uint32 targetCryIfKeyId,
        uint32 targetKeyElementId);

    /**
     * @brief Function pointer to the Crypto_KeyCopy function.
     *
     */
    Std_ReturnType (*Crypto_KeyCopy_Name)(uint32 cryIfKeyId, uint32 targetCryIfKeyId);

    /**
     * @brief Function pointer to the Crypto_RandomSeed function.
     *
     */
    Std_ReturnType (*Crypto_RandomSeed_Name)(uint32 cryIfKeyId, const uint8* seedPtr, uint32 seedLength);

    /**
     * @brief Function pointer to the Crypto_KeyGenerate function.
     *
     */
    Std_ReturnType (*Crypto_KeyGenerate_Name)(uint32 cryIfKeyId);

    /**
     * @brief Function pointer to the Crypto_KeyDerive function.
     *
     */
    Std_ReturnType (*Crypto_KeyDerive_Name)(uint32 cryIfKeyId, uint32 targetCryIfKeyId);

    /**
     * @brief Function pointer to the Crypto_KeyExchangeCalcPubVal function.
     *
     */
    Std_ReturnType (
        *Crypto_KeyExchangeCalcPubVal_Name)(uint32 cryIfKeyId, uint8* publicValuePtr, uint32* publicValueLengthPtr);

    /**
     * @brief Function pointer to the Crypto_KeyExchangeCalcSecret function.
     *
     */
    Std_ReturnType (*Crypto_KeyExchangeCalcSecret_Name)(
        uint32       cryIfKeyId,
        const uint8* partnerPublicValuePtr,
        uint32       partnerPublicValueLength);
} CryIf_FuncNameConfigType;

/**
 * @brief Key configuration structure for CryIf.
 *
 * This structure contains the configuration data for a key in the CryIf module.
 *
 * @typedef CryIf_KeyCfgType
 */
typedef struct CryIf_KeyCfgTag
{
    uint32 KeyId;       /**< Key ID for the configuration @range any valid uint32 value */
    uint32 CryptoKeyId; /**< Crypto Key ID for the configuration @range any valid uint32 value */
} CryIf_KeyCfgType;

/**
 * @brief Channel configuration structure for CryIf.
 *
 * This structure contains the configuration data for a channel in the CryIf module.
 *
 * @typedef CryIf_ChannelCfgType
 */
typedef struct CryIf_ChannelCfgTag
{
    uint32 ChannelId;            /**< Channel ID for the configuration @range any valid uint32 value */
    uint32 CryptoDriverObjectId; /**< Crypto Driver Object ID for the configuration @range any valid uint32 value */
} CryIf_ChannelCfgType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

extern const CryIf_KeyCfgType         CryIf_KeyCfg[CRYIF_KEY_MAX_CONFIGURED];
extern const CryIf_FuncNameConfigType CryIf_DriverApi[CRYIF_CHANNELS_MAX_CONFIGURED];

/* ========================================= external function declarations ========================================= */

/**
 * @brief          Initializes the Cryptographic Interface (CryIf) module.
 * @param[in]      configPtr : Pointer to the configuration structure for the CryIf module.
 * @return         None
 * @reentrant      Not reentrant
 * @synchronous    TRUE
 * @trace       CPD-71571
 *
 */
void CryIf_Init(const CryIf_ConfigType* configPtr);

/**
 * @brief          Retrieves the version information of the Cryptographic Interface (CryIf) module.
 * @param[out]     versioninfo : Pointer to the version information structure.
 * @return         None
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71572
 */
#if (CRYIF_VERSION_INFO_API == STD_ON)
void CryIf_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief          Processes a cryptographic job using the specified channel.
 * @param[in]      channelId : The ID of the channel to use for processing the job.
 * @param[inout]  job : Pointer to the cryptographic job structure.
 * @return         Std_ReturnType
 * @retval         E_OK: The job was successfully processed.
 * @retval         E_NOT_OK: The job could not be processed due to an error.
 * @reentrant      Reentrant but not for the same channel ID
 * @synchronous    TRUE
 * @trace       CPD-71573
 */
Std_ReturnType CryIf_ProcessJob(uint32 channelId, Crypto_JobType* job);

/**
 * @brief          Cancels a cryptographic job using the specified channel.
 * @param[in]      channelId : The ID of the channel to use for canceling the job.
 * @param[inout]  job : Pointer to the cryptographic job structure.
 * @return         Std_ReturnType
 * @retval         E_OK: The job was successfully canceled.
 * @retval         E_NOT_OK: The job could not be canceled due to an error.
 * @reentrant      Reentrant but not for the same channel ID
 * @synchronous    TRUE
 * @trace       CPD-71574
 */
Std_ReturnType CryIf_CancelJob(uint32 channelId, Crypto_JobType* job);

/**
 * @brief          Sets a key element for a cryptographic key.
 * @param[in]      cryIfKeyId : The ID of the cryptographic key.
 * @param[in]      keyElementId : The ID of the key element to set.
 * @param[in]      keyPtr : Pointer to the key data.
 * @param[in]      keyLength : The length of the key data.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element was successfully set.
 * @retval         E_NOT_OK: The key element could not be set due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71575
 */
Std_ReturnType CryIf_KeyElementSet(uint32 cryIfKeyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength);

/**
 * @brief          Sets a cryptographic key as valid.
 * @param[in]      cryIfKeyId : The ID of the cryptographic key to set as valid.
 * @return         Std_ReturnType
 * @retval         E_OK: The key was successfully set as valid.
 * @retval         E_NOT_OK: The key could not be set as valid due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71576
 */
Std_ReturnType CryIf_KeySetValid(uint32 cryIfKeyId);
/**
 * @brief          Sets a cryptographic key as INvalid.
 * @param[in]      cryIfKeyId : The ID of the cryptographic key to set as INvalid.
 * @return         Std_ReturnType
 * @retval         E_OK: The key was successfully set as INvalid.
 * @retval         E_NOT_OK: The key could not be set as valid due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71996
 */
Std_ReturnType CryIf_KeySetInValid(uint32 cryIfKeyId);

/**
 * @brief          Retrieves a key element from a cryptographic key.
 * @param[in]      cryIfKeyId : The ID of the cryptographic key.
 * @param[in]      keyElementId : The ID of the key element to retrieve.
 * @param[out]     resultPtr : Pointer to the buffer to store the key element data.
 * @param[inout]  resultLengthPtr : Pointer to the length of the buffer. On return, it contains the actual length of the
 * key element data.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element was successfully retrieved.
 * @retval         E_NOT_OK: The key element could not be retrieved due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71577
 */
Std_ReturnType CryIf_KeyElementGet(uint32 cryIfKeyId, uint32 keyElementId, uint8* resultPtr, uint32* resultLengthPtr);

/**
 * @brief          Copies a key element from one cryptographic key to another.
 * @param[in]      cryIfKeyId : The ID of the source cryptographic key.
 * @param[in]      keyElementId : The ID of the key element to copy.
 * @param[in]      targetCryIfKeyId : The ID of the target cryptographic key.
 * @param[in]      targetKeyElementId : The ID of the target key element.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element was successfully copied.
 * @retval         E_NOT_OK: The key element could not be copied due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71578
 */
Std_ReturnType
    CryIf_KeyElementCopy(uint32 cryIfKeyId, uint32 keyElementId, uint32 targetCryIfKeyId, uint32 targetKeyElementId);

/**
 * @brief          Copies a partial key element from one cryptographic key to another.
 * @param[in]      cryIfKeyId : The ID of the source cryptographic key.
 * @param[in]      keyElementId : The ID of the key element to copy.
 * @param[in]      keyElementSourceOffset : The offset within the source key element to start copying from.
 * @param[in]      keyElementTargetOffset : The offset within the target key element to start copying to.
 * @param[in]      keyElementCopyLength : The length of the data to copy.
 * @param[in]      targetCryIfKeyId : The ID of the target cryptographic key.
 * @param[in]      targetKeyElementId : The ID of the target key element.
 * @return         Std_ReturnType
 * @retval         E_OK: The partial key element was successfully copied.
 * @retval         E_NOT_OK: The partial key element could not be copied due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71579
 */
Std_ReturnType CryIf_KeyElementCopyPartial(
    uint32 cryIfKeyId,
    uint32 keyElementId,
    uint32 keyElementSourceOffset,
    uint32 keyElementTargetOffset,
    uint32 keyElementCopyLength,
    uint32 targetCryIfKeyId,
    uint32 targetKeyElementId);

/**
 * @brief          Copies a cryptographic key to another key.
 * @param[in]      cryIfKeyId : The ID of the source cryptographic key.
 * @param[in]      targetCryIfKeyId : The ID of the target cryptographic key.
 * @return         Std_ReturnType
 * @retval         E_OK: The key was successfully copied.
 * @retval         E_NOT_OK: The key could not be copied due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71580
 */
Std_ReturnType CryIf_KeyCopy(uint32 cryIfKeyId, uint32 targetCryIfKeyId);

/**
 * @brief          Seeds the random number generator with a cryptographic key.
 * @param[in]      cryIfKeyId : The ID of the cryptographic key to use as a seed.
 * @param[in]      seedPtr : Pointer to the seed data.
 * @param[in]      seedLength : The length of the seed data.
 * @return         Std_ReturnType
 * @retval         E_OK: The random number generator was successfully seeded.
 * @retval         E_NOT_OK: The random number generator could not be seeded due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71581
 */
Std_ReturnType CryIf_RandomSeed(uint32 cryIfKeyId, const uint8* seedPtr, uint32 seedLength);

/**
 * @brief          Generates a key using the Crypto driver.
 *                 This function generates a key based on the provided cryIfKeyId.
 *                 It first checks if the CryIf module is initialized and if the
 *                 provided key ID is valid. If the checks pass, it calls the
 *                 Crypto_KeyGenerate_Name function of the corresponding driver
 *                 API to generate the key.
 * @param[in]      cryIfKeyId : Key ID to generate a key for.
 *                 This parameter is a unique identifier for a key configuration.
 * @return         Std_ReturnType
 * @retval         E_OK: The key generation was successful.
 * @retval         E_NOT_OK: The key generation failed, or a development error
 *                 was detected.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71582
 */
Std_ReturnType CryIf_KeyGenerate(uint32 cryIfKeyId);

/**
 * @brief          Derives a key from an existing key using the Crypto driver.
 *                 This function derives a key based on the provided cryIfKeyId
 *                 and targetCryIfKeyId. It first checks if the CryIf module is
 *                 initialized and if the provided key IDs are valid. If the
 *                 checks pass, it calls the Crypto_KeyDerive_Name function of
 *                 the corresponding driver API to derive the key.
 * @param[in]      cryIfKeyId : Source key ID to derive a key from.
 *                 This parameter is a unique identifier for a source key
 *                 configuration.
 * @param[in]      targetCryIfKeyId : Target key ID to derive a key for.
 *                 This parameter is a unique identifier for a target key
 *                 configuration.
 * @return         Std_ReturnType
 * @retval         E_OK: The key derivation was successful.
 * @retval         E_NOT_OK: The key derivation failed, or a development error
 *                 was detected.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71583
 */
Std_ReturnType CryIf_KeyDerive(uint32 cryIfKeyId, uint32 targetCryIfKeyId);

/**
 * @brief          Calculates the public value for a key exchange using a cryptographic key.
 * @param[in]      cryIfKeyId : The ID of the cryptographic key to use for the key exchange.
 * @param[out]     publicValuePtr : Pointer to the buffer to store the public value.
 * @param[inout]  publicValueLengthPtr : Pointer to the length of the buffer. On return, it contains the actual length
 * of the public value.
 * @return         Std_ReturnType
 * @retval         E_OK: The public value was successfully calculated.
 * @retval         E_NOT_OK: The public value could not be calculated due to an error.
 * @note           This function calculates the public value for a key exchange using the provided key ID, public value
 * pointer, and public value length pointer. If development error detection is enabled, it checks whether the module has
 *                 been initialized, whether the key ID is valid, whether the public value pointer and public value
 * length pointer are valid, and whether the public value length is non-zero. If any error is detected, a corresponding
 * development error will be reported.
 * @pre            The CryIf module must be initialized.
 * @post           The public value for the key exchange will be calculated and stored in the provided buffer.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71584
 */
Std_ReturnType CryIf_KeyExchangeCalcPubVal(uint32 cryIfKeyId, uint8* publicValuePtr, uint32* publicValueLengthPtr);

/**
 * @brief          Calculates the secret value for a key exchange using a cryptographic key and a partner's public
 * value.
 * @param[in]      cryIfKeyId : The ID of the cryptographic key to use for the key exchange.
 * @param[in]      partnerPublicValuePtr : Pointer to the partner's public value.
 * @param[in]      partnerPublicValueLength : The length of the partner's public value.
 * @return         Std_ReturnType
 * @retval         E_OK: The secret value was successfully calculated.
 * @retval         E_NOT_OK: The secret value could not be calculated due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71585
 */
Std_ReturnType
    CryIf_KeyExchangeCalcSecret(uint32 cryIfKeyId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength);

/**
 * @brief          Performs a custom synchronous cryptographic operation.
 * @param[in]      dispatchId : The dispatch ID for the custom operation.
 * @param[in]      keyId : The ID of the cryptographic key to use.
 * @param[in]      keyElementId : The ID of the key element to use.
 * @param[in]      targetKeyId : The ID of the target cryptographic key.
 * @param[in]      targetKeyElementId : The ID of the target key element.
 * @param[in]      inputPtr : Pointer to the input data.
 * @param[in]      inputLength : The length of the input data.
 * @param[out]     outputPtr : Pointer to the buffer to store the output data.
 * @param[inout]  outputLengthPtr : Pointer to the length of the output buffer. On return, it contains the actual length
 * of the output data.
 * @param[out]     secondaryOutputPtr : Pointer to the buffer to store the secondary output data.
 * @param[inout]  secondaryOutputLengthPtr : Pointer to the length of the secondary output buffer. On return, it
 * contains the actual length of the secondary output data.
 * @return         Std_ReturnType
 * @retval         E_OK: The custom operation was successfully performed.
 * @retval         E_NOT_OK: The custom operation could not be performed due to an error.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71586
 */
Std_ReturnType CryIf_CustomSync(
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
 * @brief          Retrieves the status of a key using the Crypto driver.
 *                 This function retrieves the status of a key based on the
 *                 provided cryIfKeyId. It calls the Crypto_62_KeyGetStatus
 *                 function to get the key status.
 * @param[in]      cryIfKeyId : Key ID to retrieve the status for.
 *                 This parameter is a unique identifier for a key configuration.
 * @param[out]     keyStatusPtr : Pointer to the key status destination data.
 *                 This parameter is a pointer to a variable that will store
 *                 the key status.
 * @return         Std_ReturnType
 * @retval         E_OK: The key status retrieval was successful.
 * @retval         E_NOT_OK: The key status retrieval failed.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71587
 */
Std_ReturnType CryIf_KeyGetStatus(uint32 cryIfKeyId, Crypto_KeyStatusType* keyStatusPtr);

#ifdef __cplusplus
}
#endif

#endif /* CRYIF_H */
