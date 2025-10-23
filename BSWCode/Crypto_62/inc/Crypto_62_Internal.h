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
 **  @file               : Crypto_62_Internal.h
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

#ifndef CRYPTO_INTERNAL_H
#define CRYPTO_INTERNAL_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Cfg.h"
#include "Crypto_62.h"
#include "SchM_Crypto_62.h"
#include "IStdLib.h"

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
#include "NvM.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#if !defined(CRYPTO_62_LOCAL)
#define CRYPTO_62_LOCAL static /* PRQA S 3414 */ /* VL_QAC_3414 */
#endif

#define CRYPTO_JOB_NOT_VALID (0xffffU) /**< Value indicating that a cryptographic job is not valid */

#define CRYPTO_CT_SIZE \
    (sizeof(uint64)    \
     * 8U) /**< Size of the cryptographic context in bits, calculated as the size of a uint64 multiplied by 8 */

#define CRYPTO_ERROR_ALGO_NOT_SUPPORTED (0x03U)
#define CRYPTO_ECP_DP_SECP192R1_ENABLED
#define CRYPTO_ECP_DP_SECP224R1_ENABLED
#define CRYPTO_ECP_DP_SECP256R1_ENABLED
#define CRYPTO_ECP_DP_SECP384R1_ENABLED
#define CRYPTO_ECP_DP_SECP521R1_ENABLED
#define CRYPTO_ECP_DP_SECP192K1_ENABLED
#define CRYPTO_ECP_DP_SECP224K1_ENABLED
#define CRYPTO_ECP_DP_SECP256K1_ENABLED
#define CRYPTO_ECP_DP_BP256R1_ENABLED
#define CRYPTO_ECP_DP_BP384R1_ENABLED
#define CRYPTO_ECP_DP_BP512R1_ENABLED

#define CRYPTO_RSA1024_PRIVATE_KEY_SIZE (409U)
#define CRYPTO_RSA2048_PRIVATE_KEY_SIZE (797U)
#define CRYPTO_RSA3072_PRIVATE_KEY_SIZE (1181U)

#define CRYPTO_CONST_0   (0U)   /**< Constant 0 */
#define CRYPTO_CONST_1   (1U)   /**< Constant 1 */
#define CRYPTO_CONST_2   (2U)   /**< Constant 2 */
#define CRYPTO_CONST_3   (3U)   /**< Constant 3 */
#define CRYPTO_CONST_4   (4U)   /**< Constant 4 */
#define CRYPTO_CONST_5   (5U)   /**< Constant 5 */
#define CRYPTO_CONST_6   (6U)   /**< Constant 6 */
#define CRYPTO_CONST_7   (7U)   /**< Constant 7 */
#define CRYPTO_CONST_8   (8U)   /**< Constant 8 */
#define CRYPTO_CONST_9   (9U)   /**< Constant 9 */
#define CRYPTO_CONST_10  (10U)  /**< Constant 10 */
#define CRYPTO_CONST_11  (11U)  /**< Constant 11 */
#define CRYPTO_CONST_12  (12U)  /**< Constant 12 */
#define CRYPTO_CONST_13  (13U)  /**< Constant 13 */
#define CRYPTO_CONST_14  (14U)  /**< Constant 14 */
#define CRYPTO_CONST_15  (15U)  /**< Constant 15 */
#define CRYPTO_CONST_16  (16U)  /**< Constant 16 */
#define CRYPTO_CONST_17  (17U)  /**< Constant 17 */
#define CRYPTO_CONST_18  (18U)  /**< Constant 18 */
#define CRYPTO_CONST_19  (19U)  /**< Constant 19 */
#define CRYPTO_CONST_20  (20U)  /**< Constant 20 */
#define CRYPTO_CONST_21  (21U)  /**< Constant 21 */
#define CRYPTO_CONST_22  (22U)  /**< Constant 22 */
#define CRYPTO_CONST_24  (24U)  /**< Constant 24 */
#define CRYPTO_CONST_25  (25U)  /**< Constant 25 */
#define CRYPTO_CONST_26  (26U)  /**< Constant 26 */
#define CRYPTO_CONST_27  (27U)  /**< Constant 27 */
#define CRYPTO_CONST_28  (28U)  /**< Constant 28 */
#define CRYPTO_CONST_29  (29U)  /**< Constant 29 */
#define CRYPTO_CONST_30  (30U)  /**< Constant 30 */
#define CRYPTO_CONST_31  (31U)  /**< Constant 31 */
#define CRYPTO_CONST_32  (32U)  /**< Constant 32 */
#define CRYPTO_CONST_33  (33U)  /**< Constant 33 */
#define CRYPTO_CONST_34  (34U)  /**< Constant 34 */
#define CRYPTO_CONST_39  (39U)  /**< Constant 39 */
#define CRYPTO_CONST_41  (41U)  /**< Constant 41 */
#define CRYPTO_CONST_56  (56U)  /**< Constant 56 */
#define CRYPTO_CONST_60  (60U)  /**< Constant 60 */
#define CRYPTO_CONST_61  (61U)  /**< Constant 61 */
#define CRYPTO_CONST_62  (62U)  /**< Constant 62 */
#define CRYPTO_CONST_63  (63U)  /**< Constant 63 */
#define CRYPTO_CONST_64  (64U)  /**< Constant 64 */
#define CRYPTO_CONST_65  (65U)  /**< Constant 65 */
#define CRYPTO_CONST_68  (68U)  /**< Constant 68 */
#define CRYPTO_CONST_80  (80U)  /**< Constant 80 */
#define CRYPTO_CONST_94  (94U)  /**< Constant 94 */
#define CRYPTO_CONST_95  (95U)  /**< Constant 95 */
#define CRYPTO_CONST_96  (96U)  /**< Constant 96 */
#define CRYPTO_CONST_128 (128U) /**< Constant 128 */
#define CRYPTO_CONST_192 (192U) /**< Constant 192 */
#define CRYPTO_CONST_256 (256U) /**< Constant 256 */

#define CRYPTO_CONST_0x0F       (0x0FU)       /**< Constant 0x0F */
#define CRYPTO_CONST_0x80       (0x80U)       /**< Constant 0x1F */
#define CRYPTO_CONST_0x3F       (0x3FU)       /**< Constant 0x3F */
#define CRYPTO_CONST_0xFF       (0xFFU)       /**< Constant 0xFF */
#define CRYPTO_CONST_0x0FFFFFFF (0x0FFFFFFFU) /**< Constant 0x0FFFFFFF */
/*report DET error*/
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
#define Crypto_62_ReportDetErr(serviceId, error) \
    (void)Det_ReportError(                       \
        CRYPTO_MODULE_ID,                        \
        CRYPTO_MODULE_INSTANCE_ID,               \
        (serviceId),                             \
        (error)); /**< Reports a development error using the Det_ReportError function.*/
#endif

#define CRYPTO_IS_BIG_ENDIAN 1U

#define CRYPTO_MODE_ENCRYPT 1U /**< AES encryption. */
#define CRYPTO_MODE_DECRYPT 0U /**< AES decryption. */

/* PRQA S 3472 ++ */ /* VL_Crypto_62_General */
#define CRYPTO_GET_UINT32_BE(data, offset)                                   \
    ((CRYPTO_IS_BIG_ENDIAN) ? Crypto_Get_Unaligned_Uint32((data) + (offset)) \
                            : Crypto_Bswap32(Crypto_Get_Unaligned_Uint32((data) + (offset))))
/* PRQA S 3472 -- */ /* VL_Crypto_62_General */

#define CRYPTO_GET_UINT64_BE(data, offset)                                   \
    ((CRYPTO_IS_BIG_ENDIAN) ? Crypto_get_unaligned_uint64((data) + (offset)) \
                            : Crypto_BSWAP64(Crypto_get_unaligned_uint64((data) + (offset))))
#define CRYPTO_GET_UINT64_LE(data, offset)                                                   \
    ((CRYPTO_IS_BIG_ENDIAN) ? Crypto_BSWAP64(Crypto_get_unaligned_uint64((data) + (offset))) \
                            : Crypto_get_unaligned_uint64((data) + (offset)))

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#if (CRYPTO_JOB_QUEUING == STD_ON)
extern Crypto_JobType Crypto_62_JobInQueue[CRYPTO_MAX_DRIVER_OBJECT][CRYPTO_MAX_QUEUE_SIZE];
extern uint32         Crypto_62_QueueFilledSize[CRYPTO_MAX_DRIVER_OBJECT];
#endif
#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
extern uint8  Crypto_62_Key_Store0[CRYPTO_NVBLOCK_SIZE_0];
extern uint16 Crypto_62_Key_RetryCount[CRYPTO_MAXNVBLOCK_CONFIGURED];
#endif

#if (CRYPTO_MAXKEY_CONFIGURED > 0)
/*Enumeration of the current Key state*/

extern Crypto_62_KeyCfgType Crypto_62_Key[CRYPTO_MAXKEY_CONFIGURED];

extern Crypto_KeyStatusType CryptoKeyStatus[CRYPTO_MAXKEY_CONFIGURED];

#endif /* CRYPTO_MAXKEY_CONFIGURED > 0 */

extern Crypto_62_DriverStatusType    Crypto_62_DriverStatus;
extern Crypto_JobType                Crypto_62_StoredJob[CRYPTO_MAX_DRIVER_OBJECT];
extern Crypto_62_ObjectStatusType    Crypto_ObjectStatus[CRYPTO_MAX_DRIVER_OBJECT];
extern Crypto_62_DriverObjectCfgType Crypto_DriverObjects[CRYPTO_MAX_DRIVER_OBJECT];
extern uint8                         Crypto_62_MemPool[CRYPTO_MEMPOOL_SIZE];

/* ========================================= external function declarations ========================================= */
/**
 * @brief          Processes an encryption operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the encryption algorithm.
 * @param[in]      mode : The mode of the encryption algorithm.
 * @param[in]      operateMode : The operation mode of the encryption process (e.g., start, update, finish).
 * @return         Std_ReturnType
 * @retval         E_OK: The encryption process was successful.
 * @retval         E_NOT_OK: The encryption process failed.
 * @retval         CRYPTO_ERROR_ALGO_NOT_SUPPORTED: The specified algorithm family is not supported.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71656
 */
Std_ReturnType Crypto_62_Encrypt_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief          Processes a decryption operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the decryption algorithm.
 * @param[in]      mode : The mode of the decryption algorithm.
 * @param[in]      operateMode : The operation mode of the decryption process (e.g., start, update, finish).
 * @return         Std_ReturnType
 * @retval         E_OK: The decryption process was successful.
 * @retval         E_NOT_OK: The decryption process failed.
 * @retval         CRYPTO_ERROR_ALGO_NOT_SUPPORTED: The specified algorithm family is not supported.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71657
 */
Std_ReturnType Crypto_62_Decrypt_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief          Processes an AEAD (Authenticated Encryption with Associated Data) encryption operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the AEAD encryption algorithm.
 * @param[in]      mode : The mode of the AEAD encryption algorithm.
 * @param[in]      operateMode : The operation mode of the AEAD encryption process (e.g., start, update, finish).
 * @return         Std_ReturnType
 * @retval         E_OK: The AEAD encryption process was successful.
 * @retval         E_NOT_OK: The AEAD encryption process failed.
 * @retval         CRYPTO_ERROR_ALGO_NOT_SUPPORTED: The specified algorithm family is not supported.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71658
 */
Std_ReturnType Crypto_62_AeadEncrypt_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief          Processes an AEAD (Authenticated Encryption with Associated Data) decryption operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the AEAD decryption algorithm.
 * @param[in]      mode : The mode of the AEAD decryption algorithm.
 * @param[in]      operateMode : The operation mode of the AEAD decryption process (e.g., start, update, finish).
 * @return         Std_ReturnType
 * @retval         E_OK: The AEAD decryption process was successful.
 * @retval         E_NOT_OK: The AEAD decryption process failed.
 * @retval         CRYPTO_ERROR_ALGO_NOT_SUPPORTED: The specified algorithm family is not supported.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71659
 */
Std_ReturnType Crypto_62_AeadDecrypt_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief          Processes a signature generation operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the signature generation algorithm.
 * @param[in]      mode : The mode of the signature generation algorithm.
 * @param[in]      operateMode : The operation mode of the signature generation process (e.g., start, update, finish).
 * @return         Std_ReturnType
 * @retval         E_OK: The signature generation process was successful.
 * @retval         E_NOT_OK: The signature generation process failed.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71660
 */
Std_ReturnType Crypto_62_SignatureGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief          Processes a signature verification operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the signature verification algorithm.
 * @param[in]      mode : The mode of the signature verification algorithm.
 * @param[in]      operateMode : The operation mode of the signature verification process (e.g., start, update, finish).
 * @return         Std_ReturnType
 * @retval         E_OK: The signature verification process was successful.
 * @retval         E_NOT_OK: The signature verification process failed.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71661
 */
Std_ReturnType Crypto_62_SignatureVerify_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief          Processes a random number generation operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the random number generation algorithm.
 * @param[in]      mode : The mode of the random number generation algorithm.
 * @param[in]      operateMode : The operation mode of the random number generation process (e.g., start, update,
 * finish).
 * @return         Std_ReturnType
 * @retval         E_OK: The random number generation process was successful.
 * @retval         E_NOT_OK: The random number generation process failed.
 * @retval         CRYPTO_ERROR_ALGO_NOT_SUPPORTED: The specified algorithm mode is not supported.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71662
 */
Std_ReturnType Crypto_62_RandomGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode);

/**
 * @brief          Processes a random number generator (RNG) seeding operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the RNG algorithm.
 * @param[in]      mode : The mode of the RNG algorithm (not used in this function).
 * @return         Std_ReturnType
 * @retval         E_OK: The RNG seeding process was successful.
 * @retval         E_NOT_OK: The RNG seeding process failed.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71663
 */
Std_ReturnType Crypto_62_RandomSeed_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode);

/**
 * @brief          Processes a key generation operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the key generation algorithm.
 * @param[in]      mode : The mode of the key generation algorithm (not used in this function).
 * @return         Std_ReturnType
 * @retval         E_OK: The key generation process was successful.
 * @retval         E_NOT_OK: The key generation process failed.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71664
 */
Std_ReturnType Crypto_62_KeyGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode);

/**
 * @brief          Processes a key derivation operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the key derivation algorithm.
 * @param[in]      mode : The mode of the key derivation algorithm (not used in this function).
 * @return         Std_ReturnType
 * @retval         E_OK: The key derivation process was successful.
 * @retval         E_NOT_OK: The key derivation process failed.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71665
 */
Std_ReturnType Crypto_62_KeyDerive_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode);

/**
 * @brief          Processes a key exchange operation to calculate the public value.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the key exchange algorithm.
 * @param[in]      mode : The mode of the key exchange algorithm (not used in this function).
 * @return         Std_ReturnType
 * @retval         E_OK: The public value calculation was successful.
 * @retval         E_NOT_OK: The public value calculation failed.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71666
 */
Std_ReturnType Crypto_62_KeyExchangeCalcPubval_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode);

/**
 * @brief          Processes a key exchange operation to calculate the shared secret.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the key exchange algorithm.
 * @param[in]      mode : The mode of the key exchange algorithm (not used in this function).
 * @return         Std_ReturnType
 * @retval         E_OK: The shared secret calculation was successful.
 * @retval         E_NOT_OK: The shared secret calculation failed.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71667
 */
Std_ReturnType Crypto_62_KeyExchangeCalcSecret_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode);

#if (CRYPTO_SERVICE_CUSTOM == STD_ON)
/**
 * @brief          Processes a custom cryptographic service.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      algorithmfamily : The family of the custom cryptographic algorithm.
 * @param[in]      mode : The mode of the custom cryptographic algorithm (not used in this function).
 * @return         Std_ReturnType
 * @retval         E_OK: The custom service process was successful.
 * @retval         E_NOT_OK: The custom service process failed.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71668
 */
Std_ReturnType
    Custom_Service_Process(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily, Crypto_AlgorithmModeType mode);

#endif
#if (CRYPTO_SERVICE_ENCRYPT == STD_ON)
/**
 * @brief          Processes an AES decryption operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      mode : The mode of the AES decryption algorithm.
 * @return         Std_ReturnType
 * @retval         E_OK: The AES decryption process was successful.
 * @retval         E_NOT_OK: The AES decryption process failed.
 * @retval         CRYPTO_ERROR_ALGO_NOT_SUPPORTED: The specified algorithm mode is not supported.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71669
 */
Std_ReturnType Crypto_AesDecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode);

/**
 * @brief          Processes an AES encryption operation.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      mode : The mode of the AES encryption algorithm.
 * @return         Std_ReturnType
 * @retval         E_OK: The AES encryption process was successful.
 * @retval         E_NOT_OK: The AES encryption process failed.
 * @retval         CRYPTO_ERROR_ALGO_NOT_SUPPORTED: The specified algorithm mode is not supported.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71670
 */
Std_ReturnType Crypto_AesEncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode);
#endif
/**
 * @brief          Processes an asynchronous cryptographic job without queuing.
 * @param[in]      objectId : The ID of the cryptographic object.
 * @param[in]      job : Pointer to the cryptographic job structure.
 * @return         Std_ReturnType
 * @retval         E_OK: The job was processed successfully.
 * @retval         E_NOT_OK: The job could not be processed due to an error.
 * @reentrant      Not reentrant for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71671
 */
Std_ReturnType Crypto_62_ProcessAsyncJob_NonQueue(uint32 objectId, Crypto_JobType* job);

/**
 * @brief          Processes the specified cryptographic algorithm based on the given object ID and operation mode.
 * @param[in]      objectId : Unique identifier of the cryptographic job.
 * @param[in]      operateMode : Mode of operation for the cryptographic process (e.g., initialization, processing,
 * finalization).
 * @return         Std_ReturnType
 * @retval         E_OK: The cryptographic operation was successfully processed.
 * @retval         E_NOT_OK: The cryptographic operation failed due to an underlying error or unsupported
 * algorithm/service.
 * @reentrant      Reentrant but not for the same object ID
 * @synchronous    TRUE
 * @trace       CPD-71672
 */
Std_ReturnType Crypto_ProcessAlgorithm(uint32 objectId, Crypto_OperationModeType operateMode);

/**
 * @brief          Processes a cryptographic job internally based on the specified operation mode.
 * @param[in]      objectId : Unique identifier of the cryptographic job.
 * @param[in,out]  job : Pointer to the cryptographic job structure containing input and output data.
 * @param[in]      operationMode : Mode of operation for the cryptographic job (e.g., start, update, finish).
 * @return         Std_ReturnType
 * @retval         E_OK: The cryptographic job was successfully processed according to the specified operation mode.
 * @retval         E_NOT_OK: The cryptographic job failed due to an underlying error, invalid operation mode, or job
 * state mismatch.
 * @reentrant      Reentrant but not for the same object ID and job
 * @synchronous    TRUE
 * @trace       CPD-71673
 */
Std_ReturnType
    Crypto_62_ProcessJob_Internal(uint32 objectId, Crypto_JobType* job, Crypto_OperationModeType operationMode);

#if (CRYPTO_JOB_QUEUING == STD_ON)
/**
 * @brief          Enqueues a cryptographic job into the job queue based on its priority.
 * @param[in]      job : Pointer to the cryptographic job structure to be enqueued.
 * @param[in]      objectId : Unique identifier of the cryptographic job
 * @return         Std_ReturnType
 * @retval         E_OK: The cryptographic job was successfully enqueued.
 * @retval         E_NOT_OK: The cryptographic job failed to be enqueued due to an underlying error.
 * @retval         CRYPTO_E_BUSY: The job queue is full and the job cannot be enqueued at this time.
 * @reentrant      Reentrant but not for the same object ID and job
 * @synchronous    TRUE
 * @trace       CPD-71674
 */
Std_ReturnType Crypto_62_QueueInJob(uint32 objectId, Crypto_JobType* job);

Std_ReturnType Crypto_QueueOutJob(uint32 objectId, Crypto_JobType* job);

/**
 * @brief          Cancels a queued job for a given object ID.
 * @param[in]      objectId : The ID of the object whose queued job is to be canceled.
 * @param[in]      job : Pointer to the job structure that needs to be canceled.
 * @return         Std_ReturnType
 * @retval         E_OK: The specified job was successfully canceled from the queue.
 * @retval         E_NOT_OK: The job was not found in the queue or could not be canceled.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71675
 */
Std_ReturnType Crypto_62_CancelQueuedJob(uint32 objectId, Crypto_JobType* job);
void           Crypto_SetQueueBuf(Crypto_JobType* destJob, Crypto_JobType* srcJob);
#endif

/**
 * @brief          Starts a cryptographic operation for a given object ID with the specified job.
 * @param[in]      objectId : The ID of the cryptographic object to start the operation on.
 * @param[in]      job : Pointer to the job structure that defines the cryptographic operation to be started.
 * @return         Std_ReturnType
 * @retval         E_OK: The cryptographic operation has been successfully started.
 * @retval         E_NOT_OK: The cryptographic operation could not be started, or an error has been detected.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71676
 */
Std_ReturnType Crypto_Start(uint32 objectId, Crypto_JobType* job);

/**
 * @brief          Completes a cryptographic operation for a given object ID.
 * @param[in]      objectId : The ID of the cryptographic object to complete the operation on.
 * @return         Std_ReturnType
 * @retval         E_OK: The cryptographic operation has been successfully completed.
 * @retval         E_NOT_OK: The cryptographic operation could not be completed, or an error has been detected.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71677
 */
Std_ReturnType Crypto_Finish(uint32 objectId);

/**
 * @brief          Seeds the internal random number generator with a provided seed.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key associated with the random number generator.
 * @param[in]      seedPtr : Pointer to the seed data used to initialize the random number generator.
 * @param[in]      seedLength : The length of the seed data in bytes.
 * @return         Std_ReturnType
 * @retval         E_OK: The random number generator was successfully seeded.
 * @retval         E_NOT_OK: The seeding process failed, possibly due to an invalid key element ID.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71678
 */
Std_ReturnType Crypto_RandomSeedInternal(uint32 cryptoKeyId, const uint8* seedPtr, uint32 seedLength);
/**
 * @brief          Finds the index of a key element within a cryptographic key.
 * @param[in]      cryptokeyId : The ID of the cryptographic key to search within.
 * @param[in]      keyElementId : The ID of the key element to find.
 * @param[out]     getkeyElementId : Pointer to a variable where the index of the found key element will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element was found, and its index is stored in `getkeyElementId`.
 * @retval         E_NOT_OK: The key element was not found.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71679
 */
Std_ReturnType Crypto_62_KeyElementIndexFind(uint32 cryptokeyId, uint32 keyElementId, uint32* getkeyElementId);

/**
 * @brief          Sets the value of a key element for a given cryptographic key.
 * @param[in]      cryptokeyId : The ID of the cryptographic key to modify.
 * @param[in]      keyElementId : The ID of the key element to set.
 * @param[in]      keyPtr : Pointer to the buffer containing the new value for the key element.
 * @param[in]      keyLength : The length of the new key element value in bytes.
 * @return         Std_ReturnType
 * @retval         E_OK: The key element was successfully set.
 * @retval         E_NOT_OK: The key element could not be set due to an error (e.g., invalid key element ID, write
 * access denied, or key size mismatch).
 * @retval         CRYPTO_E_KEY_SIZE_MISMATCH: The provided key length does not match the expected size for the key
 * element.
 * @retval         CRYPTO_E_KEY_WRITE_FAIL: Writing to the key element is not allowed.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71680
 */
Std_ReturnType
    Crypto_62_KeyElementSetInternal(uint32 cryptokeyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength);

/**
 * @brief          Calculates the public value for a key exchange operation.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key used for the key exchange.
 * @param[out]     publicValuePtr : Pointer to the buffer where the calculated public value will be stored.
 * @param[out]     publicValueLengthPtr : Pointer to the variable where the length of the public value will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The public value was successfully calculated.
 * @retval         E_NOT_OK: The public value could not be calculated, possibly due to an invalid key ID or other
 * errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71681
 */
Std_ReturnType
    Crypto_62_KeyExchangeCalcPubVal_internal(uint32 cryptoKeyId, uint8* publicValuePtr, uint32* publicValueLengthPtr);

/**
 * @brief          Calculates the shared secret for a key exchange operation.
 * @param[in]      cryptoKeyId : The ID of the cryptographic key used for the key exchange.
 * @param[in]      partnerPublicValuePtr : Pointer to the buffer containing the partner's public value.
 * @param[in]      partnerPublicValueLength : The length of the partner's public value in bytes.
 * @return         Std_ReturnType
 * @retval         E_OK: The shared secret was successfully calculated.
 * @retval         E_NOT_OK: The shared secret could not be calculated, possibly due to an invalid key ID, invalid
 * partner public value, or other errors.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71682
 */
Std_ReturnType Crypto_62_KeyExchangeCalcSecret_internal(
    uint32       cryptoKeyId,
    const uint8* partnerPublicValuePtr,
    uint32       partnerPublicValueLength);

/**
 * @brief          Clears the stored job for a given object ID.
 * @param[in]      objectId : The ID of the object whose stored job is to be cleared.
 * @return         void
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71683
 */
void Crypto_62_ClearStoredJob(uint32 objectId);

/**
 * @brief          Sets the queue buffer by copying the contents of the source job to the destination job.
 * @param[out]     destJob : Pointer to the destination job where the contents will be copied.
 * @param[in]      srcJob : Pointer to the source job from which the contents will be copied.
 * @return         void
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71684
 */
void Crypto_62_SetQueueBuf(Crypto_JobType* destJob, Crypto_JobType* srcJob);

/**
 * @brief          Removes a job from the queue for a given object ID and returns it.
 * @param[in]      objectId : The ID of the object whose queue is to be accessed.
 * @param[out]     job : Pointer to the job structure where the dequeued job will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: A job was successfully dequeued.
 * @retval         E_NOT_OK: The queue is empty, and no job could be dequeued.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71685
 */
Std_ReturnType                Crypto_62_QueueOutJob(uint32 objectId, Crypto_JobType* job);
CRYPTO_62_LOCAL inline uint32 Crypto_Get_Unaligned_Uint32(const void* p)
{
    uint32 r;
    (void)IStdLib_MemCpy(&r, p, sizeof(r));
    return r;
}
CRYPTO_62_LOCAL inline void Crypto_put_unaligned_uint32(void* p, uint32 x)
{
    /* PRQA S 1340 ++ */ /* VL_Crypto_62_General */
    (void)IStdLib_MemCpy(p, &x, sizeof(x));
    /* PRQA S 1340 -- */ /* VL_Crypto_62_General */
}
#if !defined(Crypto_Bswap32)
CRYPTO_62_LOCAL inline uint32 Crypto_Bswap32(uint32 x)
{
    return ((x & 0x000000ffU) << 24) | ((x & 0x0000ff00U) << 8) | ((x & 0x00ff0000U) >> 8) | ((x & 0xff000000U) >> 24);
}
#define CRYPTO_BSWAP32 Crypto_Bswap32
#if (CRYPTO_IS_BIG_ENDIAN)
/* PRQA S 3412 ++ */ /* VL_Crypto_62_General */
#define CRYPTO_PUT_UINT32_BE(n, data, offset) Crypto_put_unaligned_uint32((data) + (offset), (uint32)(n));
/* PRQA S 3412 -- */ /* VL_Crypto_62_General */
#else
#define CRYPTO_PUT_UINT32_BE(n, data, offset) \
    Crypto_put_unaligned_uint32((data) + (offset), CRYPTO_BSWAP32((uint32)(n)));
#endif

#endif
/* PRQA S 3412 ++ */ /* VL_Crypto_62_General */
#define CRYPTO_PUT_UINT32_LE(n, data, offset)                                        \
    if (CRYPTO_IS_BIG_ENDIAN)                                                        \
    {                                                                                \
        Crypto_put_unaligned_uint32((data) + (offset), CRYPTO_BSWAP32((uint32)(n))); \
    }                                                                                \
    else                                                                             \
    {                                                                                \
        Crypto_put_unaligned_uint32((data) + (offset), ((uint32)(n)));               \
    }
/* PRQA S 3412 -- */ /* VL_Crypto_62_General */
#define CRYPTO_GET_UINT32_LE(data, offset)                                                   \
    ((CRYPTO_IS_BIG_ENDIAN) ? CRYPTO_BSWAP32(Crypto_Get_Unaligned_Uint32((data) + (offset))) \
                            : Crypto_Get_Unaligned_Uint32((data) + (offset)))
#if !defined(Crypto_BSWAP64)
CRYPTO_62_LOCAL inline uint64 Crypto_bswap64(uint64 x)
{
    return ((x & 0x00000000000000ffULL) << 56) | ((x & 0x000000000000ff00ULL) << 40)
           | ((x & 0x0000000000ff0000ULL) << 24) | ((x & 0x00000000ff000000ULL) << 8)
           | ((x & 0x000000ff00000000ULL) >> 8) | ((x & 0x0000ff0000000000ULL) >> 24)
           | ((x & 0x00ff000000000000ULL) >> 40) | ((x & 0xff00000000000000ULL) >> 56);
}
#define Crypto_BSWAP64 Crypto_bswap64

/* PRQA S 3412 ++ */ /* VL_Crypto_62_General */
#define CRYPTO_PUT_UINT64_BE(n, data, offset)                                        \
    if (CRYPTO_IS_BIG_ENDIAN)                                                        \
    {                                                                                \
        Crypto_put_unaligned_uint64((data) + (offset), (uint64)(n));                 \
    }                                                                                \
    else                                                                             \
    {                                                                                \
        Crypto_put_unaligned_uint64((data) + (offset), Crypto_BSWAP64((uint64)(n))); \
    }
#define CRYPTO_PUT_UINT64_LE(n, data, offset)                                        \
    if (CRYPTO_IS_BIG_ENDIAN)                                                        \
    {                                                                                \
        Crypto_put_unaligned_uint64((data) + (offset), Crypto_BSWAP64((uint64)(n))); \
    }                                                                                \
    else                                                                             \
    {                                                                                \
        Crypto_put_unaligned_uint64((data) + (offset), (uint64)(n));                 \
    }
/* PRQA S 3412 -- */ /* VL_Crypto_62_General */

CRYPTO_62_LOCAL inline void Crypto_put_unaligned_uint64(void* p, uint64 x)
{
    /* PRQA S 1340 ++ */ /* VL_Crypto_62_General */
    (void)IStdLib_MemCpy(p, &x, sizeof(x));
    /* PRQA S 1340 -- */ /* VL_Crypto_62_General */
}
CRYPTO_62_LOCAL inline uint64 Crypto_get_unaligned_uint64(const void* p)
{
    uint64 r;

    (void)IStdLib_MemCpy(&r, p, sizeof(r));

    return r;
}
#endif
#if (CRYPTO_IS_BIG_ENDIAN)

/* PRQA S 3410 ++ */ /* VL_Crypto_62_General */
#define HOST_c2l(c, l)                         \
    (l = (((unsigned long)(*((c)++))) << 24),  \
     l |= (((unsigned long)(*((c)++))) << 16), \
     l |= (((unsigned long)(*((c)++))) << 8),  \
     l |= (((unsigned long)(*((c)++)))))
/* PRQA S 3410 -- */ /* VL_Crypto_62_General */

#define HOST_l2c(l, c)                               \
    (*((c)++) = (unsigned char)(((l) >> 24) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 16) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 8) & 0xff),  \
     *((c)++) = (unsigned char)(((l)) & 0xff),       \
     l)

#else

#define HOST_c2l(c, l)                         \
    (l = (((unsigned long)(*((c)++)))),        \
     l |= (((unsigned long)(*((c)++))) << 8),  \
     l |= (((unsigned long)(*((c)++))) << 16), \
     l |= (((unsigned long)(*((c)++))) << 24))
#define HOST_l2c(l, c)                               \
    (*((c)++) = (unsigned char)(((l)) & 0xff),       \
     *((c)++) = (unsigned char)(((l) >> 8) & 0xff),  \
     *((c)++) = (unsigned char)(((l) >> 16) & 0xff), \
     *((c)++) = (unsigned char)(((l) >> 24) & 0xff), \
     l)

#endif
/******************************************************************************/
/*
 * Brief               This function do xor for a1 and a2 .
 *
 * Param-Name[in]      len: lenth of the data input.
 *                     a1: the data to be xor.
 *                     a2: the data to be xor.
 *
 * Param-Name[in/out]  None.
 * Param-Name[out]     des:the data after xor..
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL inline void Crypto_Xor(uint32 len, const uint8* a1, const uint8* a2, uint8* des)
{
    uint32 i;
    for (i = 0; i < len; i++)
    {
        des[i] = a1[i] ^ a2[i];
    }
}
/* ============================================================================
 * Everything below here is trivial wrapper functions
 */
CRYPTO_62_LOCAL inline uint64 Crypto_ct_compiler_opaque(uint64 x)
{
    /* PRQA S 4542 ++ */ /* VL_Crypto_62_General */
    return x ^ 0;
    /* PRQA S 4542 -- */ /* VL_Crypto_62_General */
}
/* Convert a number into a condition in constant time. */
CRYPTO_62_LOCAL inline uint64 Crypto_ct_bool(uint64 x)
{
    /*
     * Define mask-generation code that, as far as possible, will not use branches or conditional instructions.
     *
     * For some platforms / type sizes, we define assembly to assure this.
     *
     * Otherwise, we define a plain C fallback which (in May 2023) does not get optimised into
     * conditional instructions or branches by trunk clang, gcc, or MSVC v19.
     */

    const uint64 xo = Crypto_ct_compiler_opaque(x);

    // y is negative (i.e., top bit set) iff x is non-zero
    /* PRQA S 3101 ++ */ /* VL_Crypto_62_General */
    uint64 y = (-xo) | -(xo >> 1);
    /* PRQA S 3101 -- */ /* VL_Crypto_62_General */

    // extract only the sign bit of y so that y == 1 (if x is non-zero) or 0 (if x is zero)
    y = (((uint64)y) >> (CRYPTO_CT_SIZE - 1));

    // -y has all bits set (if x is non-zero), or all bits clear (if x is zero)
    /* PRQA S 3101 ++ */ /* VL_Crypto_62_General */
    return (uint64)(-y);
    /* PRQA S 3101 -- */ /* VL_Crypto_62_General */
}
CRYPTO_62_LOCAL inline uint64 Crypto_ct_if(uint64 condition, uint64 if1, uint64 if0)
{
    uint64 not_cond = (uint64)(~Crypto_ct_compiler_opaque(condition));
    return (uint64)((condition & if1) | (not_cond & if0));
}
CRYPTO_62_LOCAL inline uint64 Crypto_ct_size_if(uint64 condition, uint64 if1, uint64 if0)
{
    return (uint64)Crypto_ct_if(condition, (uint64)if1, (uint64)if0);
}

CRYPTO_62_LOCAL inline unsigned Crypto_ct_uint_if(uint64 condition, unsigned if1, unsigned if0)
{
    return (unsigned)Crypto_ct_if(condition, (uint64)if1, (uint64)if0);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_bool_if(uint64 condition, uint64 if1, uint64 if0)
{
    return (uint64)Crypto_ct_if(condition, (uint64)if1, (uint64)if0);
}

#if defined(Crypto_BIGNUM_C)

CRYPTO_62_LOCAL inline Crypto_mpi_uint Crypto_ct_mpi_uint_if(uint64 condition, Crypto_mpi_uint if1, Crypto_mpi_uint if0)
{
    return (Crypto_mpi_uint)Crypto_ct_if(condition, (uint64)if1, (uint64)if0);
}

#endif

CRYPTO_62_LOCAL inline uint64 Crypto_ct_size_if_else_0(uint64 condition, uint64 if1)
{
    return (uint64)(condition & if1);
}

CRYPTO_62_LOCAL inline unsigned Crypto_ct_uint_if_else_0(uint64 condition, unsigned if1)
{
    return (unsigned)(condition & if1);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_bool_if_else_0(uint64 condition, uint64 if1)
{
    return (uint64)(condition & if1);
}

#if defined(Crypto_BIGNUM_C)

CRYPTO_62_LOCAL inline Crypto_mpi_uint Crypto_ct_mpi_uint_if_else_0(uint64 condition, Crypto_mpi_uint if1)
{
    return (Crypto_mpi_uint)(condition & if1);
}

#endif /* Crypto_BIGNUM_C */
/******************************************************************************/
/*
 * Brief               Increase the counter for CTR mode, not the reseed counter
                       (, that is to say, not the counter for reseeding).
 * Param-Name[in]      None
 * Param-Name[in/out]  n: The counter inside the context of CTR_DRBG.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL inline void Crypto_Ctr_Increment_Counter(uint8 n[CRYPTO_CONST_16])
{
    // The 32-bit version seems to perform about the same as a 64-bit version
    // on 64-bit architectures, so no need to define a 64-bit version.
    /* PRQA S 2471 ++ */ /* VL_Crypto_62_General */
    for (int i = CRYPTO_CONST_3;; i--)
    /* PRQA S 2471 -- */ /* VL_Crypto_62_General */
    {
        /* PRQA S 4533 ++ */ /* VL_Crypto_62_General */
        uint32 x = CRYPTO_GET_UINT32_BE(n, i << CRYPTO_CONST_2);
        x += 1;
        CRYPTO_PUT_UINT32_BE(x, n, i << CRYPTO_CONST_2);
        /* PRQA S 4533 -- */ /* VL_Crypto_62_General */

        /* PRQA S 1843 ++ */ /* VL_Crypto_62_General */
        if ((x != 0) || (i == 0))
        /* PRQA S 1843 -- */ /* VL_Crypto_62_General */
        {
            break;
        }
    }
}
CRYPTO_62_LOCAL inline int Crypto_ct_error_if(uint64 condition, int if1, int if0)
{
    /* Coverting int -> uint -> int here is safe, because we require if1 and if0 to be
     * in the range -32767..0, and we require 32-bit int and uint types.
     *
     * This means that (0 <= -if0 < INT_MAX), so negating if0 is safe, and similarly for
     * converting back to int.
     */
    return -((int)Crypto_ct_if(condition, (uint64)(-if1), (uint64)(-if0)));
}

CRYPTO_62_LOCAL inline int Crypto_ct_error_if_else_0(uint64 condition, int if1)
{
    /* PRQA S 1821,4532,4394 ++ */ /* VL_Crypto_62_General */
    return -((int)(condition & (-if1)));
    /* PRQA S 1821,4532,4394 -- */ /* VL_Crypto_62_General */
}
CRYPTO_62_LOCAL inline uint64 Crypto_ct_uint_lt(uint64 x, uint64 y)
{
    /* Ensure that the compiler cannot optimise the following operations over x and y,
     * even if it knows the value of x and y.
     */
    const uint64 xo = Crypto_ct_compiler_opaque(x);
    const uint64 yo = Crypto_ct_compiler_opaque(y);
    /*
     * Check if the most significant bits (MSB) of the operands are different.
     * cond is true iff the MSBs differ.
     */
    uint64 cond = Crypto_ct_bool((xo ^ yo) >> (CRYPTO_CT_SIZE - 1U));

    /*
     * If the MSB are the same then the difference x-y will be negative (and
     * have its MSB set to 1 during conversion to unsigned) if and only if x<y.
     *
     * If the MSB are different, then the operand with the MSB of 1 is the
     * bigger. (That is if y has MSB of 1, then x<y is true and it is false if
     * the MSB of y is 0.)
     */

    // Select either y, or x - y
    uint64 ret = Crypto_ct_if(cond, yo, (uint64)(xo - yo));

    // Extract only the MSB of ret
    ret = ret >> (CRYPTO_CT_SIZE - 1U);

    // Convert to a condition (i.e., all bits set iff non-zero)
    return Crypto_ct_bool(ret);
}
CRYPTO_62_LOCAL inline uint64 Crypto_ct_uint_ne(uint64 x, uint64 y)
{
    /* diff = 0 if x == y, non-zero otherwise */
    const uint64 diff = Crypto_ct_compiler_opaque(x) ^ Crypto_ct_compiler_opaque(y);

    /* all ones if x != y, 0 otherwise */
    return Crypto_ct_bool(diff);
}
CRYPTO_62_LOCAL inline uint64 Crypto_ct_uint_eq(uint64 x, uint64 y)
{
    return ~Crypto_ct_uint_ne(x, y);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_uint_gt(uint64 x, uint64 y)
{
    return Crypto_ct_uint_lt(y, x);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_uint_ge(uint64 x, uint64 y)
{
    return ~Crypto_ct_uint_lt(x, y);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_uint_le(uint64 x, uint64 y)
{
    return ~Crypto_ct_uint_gt(x, y);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_bool_ne(uint64 x, uint64 y)
{
    return (uint64)(x ^ y);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_bool_and(uint64 x, uint64 y)
{
    return (uint64)(x & y);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_bool_or(uint64 x, uint64 y)
{
    return (uint64)(x | y);
}

CRYPTO_62_LOCAL inline uint64 Crypto_ct_bool_not(uint64 x)
{
    return (uint64)(~x);
}

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_INTERNAL_H */
