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
 **  @file               : Crypto_GeneralTypes.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : Type definitions of Crypto stack
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_GENERALTYPES_H
#define CRYPTO_GENERALTYPES_H

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Rte_Csm_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
/*
 * Enumeration of the algorithm family.
 * */
/*Development errors used by the CRYPTO module*/
#define CRYPTO_E_UNINIT        ((uint8)0x00)
#define CRYPTO_E_INIT_FAILED   ((uint8)0x01)
#define CRYPTO_E_PARAM_POINTER ((uint8)0x02)
#define CRYPTO_E_PARAM_HANDLE  ((uint8)0x04)
#define CRYPTO_E_PARAM_VALUE   ((uint8)0x05)
#define CRYPTO_E_SMALL_BUFFER  ((uint8)0x06)
#define CRYPTO_E_QUEUE_FULL    ((Std_ReturnType)0x05u)
/*[SWS_Csm_01022]key element id*/
/*MAC*/
#define CRYPTO_KE_MAC_KEY   ((uint32)1U)
#define CRYPTO_KE_MAC_PROOF ((uint32)2U)

/*Signature*/
#define CRYPTO_KE_SIGNATURE_KEY ((uint32)1U)
/*Random*/
#define CRYPTO_KE_RANDOM_SEED_STATE ((uint32)3U)
#define CRYPTO_KE_RANDOM_ALGORITHM  ((uint32)4U)
/*Cipher/AEAD*/
#define CRYPTO_KE_CIPHER_KEY    ((uint32)1U)
#define CRYPTO_KE_CIPHER_IV     ((uint32)5U)
#define CRYPTO_KE_CIPHER_PROOF  ((uint32)6U)
#define CRYPTO_KE_CIPHER_2NDKEY ((uint32)7U)
/*Key Exchange*/
#define CRYPTO_KE_KEYEXCHANGE_BASE        ((uint32)8U)
#define CRYPTO_KE_KEYEXCHANGE_PRIVKEY     ((uint32)9U)
#define CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY   ((uint32)10U)
#define CYRPTO_KE_KEYEXCHANGE_SHAREDVALUE ((uint32)1U)
#define CRYPTO_KE_KEYEXCHANGE_ALGORITHM   ((uint32)12U)
#define CRYPTO_KE_KEYEXCHANGE_CURVETYPE   ((uint32)29U)
/*Key Derivation*/
#define CRYPTO_KE_KEYDERIVATION_PASSWORD   ((uint32)1U)
#define CRYPTO_KE_KEYDERIVATION_SALT       ((uint32)13U)
#define CRYPTO_KE_KEYDERIVATION_ITERATIONS ((uint32)14U)
#define CRYPTO_KE_KEYDERIVATION_ALGORITHM  ((uint32)15U)
#define CRYPTO_KE_KEYDERIVATION_CURVETYPE  ((uint32)29U)
/*Key Generate*/
#define CRYPTO_KE_KEYGENERATE_KEY       ((uint32)1U)
#define CRYPTO_KE_KEYGENERATE_SEED      ((uint32)16U)
#define CRYPTO_KE_KEYGENERATE_ALGORITHM ((uint32)17U)
#define CRYPTO_KE_KEYGENERATE_CURVETYPE ((uint32)29U)
/*Certificate Parsing*/
#define CRYPTO_KE_CERTIFICATE_DATA                ((uint32)0)
#define CRYPTO_KE_CERTIFICATE_PARSING_FORMAT      ((uint32)18U)
#define CRYPTO_KE_CERTIFICATE_CURRENT_TIME        ((uint32)19U)
#define CRYPTO_KE_CERTIFICATE_VERSION             ((uint32)20U)
#define CRYPTO_KE_CERTIFICATE_SERIALNUMBER        ((uint32)21U)
#define CRYPTO_KE_CERTIFICATE_SIGNATURE_ALGORITHM ((uint32)22U)
#define CRYPTO_KE_CERTIFICATE_ISSUER              ((uint32)23U)
#define CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_BEFORE ((uint32)24U)
#define CRYPTO_KE_CERTIFICATE_VALIDITY_NOT_AFTER  ((uint32)25U)
#define CRYPTO_KE_CERTIFICATE_SUBJECT             ((uint32)26U)
#define CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY  ((uint32)1U)
#define CRYPTO_KE_CERTIFICATE_EXTENSIONS          ((uint32)27U)
#define CRYPTO_KE_CERTIFICATE_SIGNATURE           ((uint32)28U)

/* ================================================ type definitions ================================================ */
/** @brief Enumeration of the algorithm family.*/
/* PRQA S 1271++ */ /* VL_Csm_1271 */
typedef enum
{
    CRYPTO_ALGOFAM_NOT_SET              = 0x00u,
    CRYPTO_ALGOFAM_SHA1                 = 0x01u,
    CRYPTO_ALGOFAM_SHA2_224             = 0x02u,
    CRYPTO_ALGOFAM_SHA2_256             = 0x03u,
    CRYPTO_ALGOFAM_SHA2_384             = 0x04u,
    CRYPTO_ALGOFAM_SHA2_512             = 0x05u,
    CRYPTO_ALGOFAM_SHA2_512_224         = 0x06u,
    CRYPTO_ALGOFAM_SHA2_512_256         = 0x07u,
    CRYPTO_ALGOFAM_SHA3_224             = 0x08u,
    CRYPTO_ALGOFAM_SHA3_256             = 0x09u,
    CRYPTO_ALGOFAM_SHA3_384             = 0x0Au,
    CRYPTO_ALGOFAM_SHA3_512             = 0x0Bu,
    CRYPTO_ALGOFAM_SHAKE128             = 0x0Cu,
    CRYPTO_ALGOFAM_SHAKE256             = 0x0Du,
    CRYPTO_ALGOFAM_RIPEMD160            = 0x0Eu,
    CRYPTO_ALGOFAM_BLAKE_1_256          = 0x0Fu,
    CRYPTO_ALGOFAM_BLAKE_1_512          = 0x10u,
    CRYPTO_ALGOFAM_BLAKE_2s_256         = 0x11u,
    CRYPTO_ALGOFAM_BLAKE_2s_512         = 0x12u,
    CRYPTO_ALGOFAM_3DES                 = 0x13u,
    CRYPTO_ALGOFAM_AES                  = 0x14u,
    CRYPTO_ALGOFAM_CHACHA               = 0x15u,
    CRYPTO_ALGOFAM_RSA                  = 0x16u,
    CRYPTO_ALGOFAM_ED25519              = 0x17u,
    CRYPTO_ALGOFAM_BRAINPOOL            = 0x18u,
    CRYPTO_ALGOFAM_ECCNIST              = 0x19u,
    CRYPTO_ALGOFAM_RNG                  = 0x1bu,
    CRYPTO_ALGOFAM_SIPHASH              = 0x1cu,
    CRYPTO_ALGOFAM_ECCANSI              = 0x1eu,
    CRYPTO_ALGOFAM_ECCSEC               = 0x1fu,
    CRYPTO_ALGOFAM_DRBG                 = 0x20u,
    CRYPTO_ALGOFAM_FIPS186              = 0x21u,
    CRYPTO_ALGOFAM_PADDING_PKCS7        = 0x22u,
    CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS = 0x23u,
    CRYPTO_ALGOFAM_PBKDF2               = 0x24u,
    CRYPTO_ALGOFAM_KDFX963              = 0x25u,
    CRYPTO_ALGOFAM_DH                   = 0x26u,
    CRYPTO_ALGOFAM_SM2                  = 0x27u,
    CRYPTO_ALGOFAM_EEA3                 = 0x28u,
    CRYPTO_ALGOFAM_SM3                  = 0x29u,
    CRYPTO_ALGOFAM_EIA3                 = 0x2Au,
    CRYPTO_ALGOFAM_HKDF                 = 0x2Bu,
    CRYPTO_ALGOFAM_ECDSA                = 0x2Cu,
    CRYPTO_ALGOFAM_POLY1305             = 0x2Du,
    CRYPTO_ALGOFAM_X25519               = 0x2Eu,
    CRYPTO_ALGOFAM_ECDH                 = 0x2Fu,
    CRYPTO_ALGOFAM_CUSTOM               = 0xfFu

} Crypto_AlgorithmFamilyType;
/* PRQA S 1271-- */

/** @brief  Enumeration of the algorithm mode */
/* PRQA S 0722 ++ */ /* VL_Csm_0722*/
/* PRQA S 1271++ */  /* VL_Csm_1271 */
typedef enum
{
    CRYPTO_ALGOMODE_NOT_SET           = 0x00u,
    CRYPTO_ALGOMODE_ECB               = 0x01u,
    CRYPTO_ALGOMODE_CBC               = 0x02u,
    CRYPTO_ALGOMODE_CFB               = 0x03u,
    CRYPTO_ALGOMODE_OFB               = 0x04u,
    CRYPTO_ALGOMODE_CTR               = 0x05u,
    CRYPTO_ALGOMODE_GCM               = 0x06u,
    CRYPTO_ALGOMODE_XTS               = 0x07u,
    CRYPTO_ALGOMODE_RSAES_OAEP        = 0x08u,
    CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5  = 0x09u,
    CRYPTO_ALGOMODE_RSASSA_PSS        = 0x0au,
    CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5 = 0x0bu,
    CRYPTO_ALGOMODE_8ROUNDS           = 0x0cu,
    CRYPTO_ALGOMODE_12ROUNDS          = 0x0du,
    CRYPTO_ALGOMODE_20ROUNDS          = 0x0eu,
    CRYPTO_ALGOMODE_HMAC              = 0x0fu,
    CRYPTO_ALGOMODE_CMAC              = 0x10u,
    CRYPTO_ALGOMODE_GMAC              = 0x11u,
    CRYPTO_ALGOMODE_CTRDRBG           = 0x12u,
    CRYPTO_ALGOMODE_SIPHASH_2_4       = 0x13u,
    CRYPTO_ALGOMODE_SIPHASH_4_8       = 0x14u,
    CRYPTO_ALGOMODE_PXXXR1            = 0x15u,
    CRYPTO_ALGOMODE_AESKEYWRAP        = 0x16u,
    CRYPTO_ALGOMODE_CUSTOM            = 0xfFu
} Crypto_AlgorithmModeType;
/* PRQA S 1271-- */
/* PRQA S 0722 -- */
/** @brief  Defines which of the input/output parameters are re-directed to a key element.
 *  The values can be combined to define a bit field.
 * */

/* PRQA S 1271++ */ /* VL_Csm_1271 */
typedef enum
{
    CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT    = 0x01u,
    CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT  = 0x02u,
    CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT   = 0x04u,
    CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT   = 0x10u,
    CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT = 0x20u
} Crypto_InputOutputRedirectionConfigType;
/* PRQA S 1271-- */

/** @brief Enumeration of the current job state*/
/* PRQA S 1271++ */ /* VL_Csm_1271 */
typedef enum
{
    CRYPTO_JOBSTATE_IDLE   = 0x00u, /*Job is in the state "idle".*/
    CRYPTO_JOBSTATE_ACTIVE = 0x01u  /*Job is in the state "active".*/
} Crypto_JobStateType;
/* PRQA S 1271-- */

/** @brief Structure which contains input and output information depending on the job and
 * the crypto primitive.,ref@01009*/
typedef struct
{
    const uint8*             inputPtr;
    uint32                   inputLength;
    const uint8*             secondaryInputPtr;
    uint32                   secondaryInputLength;
    const uint8*             tertiaryInputPtr;
    uint32                   tertiaryInputLength;
    uint8*                   outputPtr;
    uint32*                  outputLengthPtr;
    uint8*                   secondaryOutputPtr;
    uint32*                  secondaryOutputLengthPtr;
    uint64                   input64;
    Crypto_VerifyResultType* verifyPtr;
    uint64*                  output64Ptr;
    Crypto_OperationModeType mode;
    uint32                   cryIfKeyId;
    uint32                   targetCryIfKeyId;
} Crypto_JobPrimitiveInputOutputType;

/** @brief Enumeration of the kind of the service.*/
/* PRQA S 1271++ */ /* VL_Csm_1271 */
typedef enum
{
    CRYPTO_HASH                  = 0x00u, /* Hash Service */
    CRYPTO_MACGENERATE           = 0x01u, /* MacGenerate Service */
    CRYPTO_MACVERIFY             = 0x02u, /* MacVerify Service*/
    CRYPTO_ENCRYPT               = 0x03u, /* Encrypt Service */
    CRYPTO_DECRYPT               = 0x04u, /* Decrypt Service */
    CRYPTO_AEADENCRYPT           = 0x05u, /* AEADEncrypt Service */
    CRYPTO_AEADDECRYPT           = 0x06u, /* AEADDecrypt Service */
    CRYPTO_SIGNATUREGENERATE     = 0x07u, /* SignatureGenerate Service*/
    CRYPTO_SIGNATUREVERIFY       = 0x08u, /* SignatureVerify Service */
    CRYPTO_SECCOUNTERINCREMENT   = 0x09u, /* SECCOUNTERINCREMENT Service */
    CRYPTO_SECCOUNTERREAD        = 0x0Au, /* SECCOUNTERREAD Service */
    CRYPTO_RANDOMGENERATE        = 0x0Bu, /* RandomGenerate Service */
    CRYPTO_RANDOMSEED            = 0x0Cu, /* RandomSeed Service */
    CRYPTO_KEYGENERATE           = 0x0Du, /* KeyGenerate Service */
    CRYPTO_KEYDERIVE             = 0x0Eu, /* KeyDerive Service */
    CRYPTO_KEYEXCHANGECALCPUBVAL = 0x0Fu, /* KeyExchangeCalcPubVal Service */
    CRYPTO_KEYEXCHANGECALCSECRET = 0x10u, /* KeyExchangeCalcSecret Service */
    CRYPTO_KEYSETVALID           = 0x13u, /* KeySetValid Service */
    CRYPTO_KEYSETINVALID         = 0x14u, /* KeySetinValid Service */
    CUSTOM_SERVICE               = 0x15u, /* Custom Service */
    CRYPTO_SERVICEMAX            = 0x16u
} Crypto_ServiceInfoType;
/* PRQA S 1271-- */

/** @brief Structure which determines the exact algorithm.*/
typedef struct
{
    Crypto_AlgorithmFamilyType family;
    Crypto_AlgorithmFamilyType secondaryFamily;
    uint32                     keyLength;
    Crypto_AlgorithmModeType   mode;
} Crypto_AlgorithmInfoType;

/** @brief Structure which contains basic information about the crypto primitive.*/
typedef struct
{
    const Crypto_ServiceInfoType   service;
    const Crypto_AlgorithmInfoType algorithm;
} Crypto_PrimitiveInfoType;

/** @brief Enumeration of the processing type.*/
/* PRQA S 1271++ */ /* VL_Csm_1271 */
typedef enum
{
    CRYPTO_PROCESSING_ASYNC = 0x00u, /* Asynchronous job processing*/
    CRYPTO_PROCESSING_SYNC  = 0x01u  /* Synchronous job processing*/
} Crypto_ProcessingType;
/* PRQA S 1271-- */

/** @brief Structure which contains further information, which depends on the job and
 * the crypto primitive.*/
typedef struct
{
    uint32                          callbackId;
    const Crypto_PrimitiveInfoType* primitiveInfo;
    uint32                          cryIfKeyId;
    Crypto_ProcessingType           processingType;
} Crypto_JobPrimitiveInfoType;

/** @brief Structure which holds the identifiers of the keys and key elements which shall be used
 * as input and output for a job and a bit structure which indicates which buffers shall
 * be redirected to those key elements.*/
typedef struct
{
    uint8  redirectionConfig;
    uint32 inputKeyId;
    uint32 inputKeyElementId;
    uint32 secondaryInputKeyId;
    uint32 secondaryInputKeyElementId;
    uint32 tertiaryInputKeyId;
    uint32 tertiaryInputKeyElementId;
    uint32 outputKeyId;
    uint32 outputKeyElementId;
    uint32 secondaryOutputKeyId;
    uint32 secondaryOutputKeyElementId;
} Crypto_JobRedirectionInfoType;

/** @brief Structure which contains further information, which depends on the job and the
 * crypto primitive.*/
typedef struct
{
    uint32                             jobId;
    Crypto_JobStateType                jobState;
    Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput;
    const Crypto_JobPrimitiveInfoType* jobPrimitiveInfo;
    uint32                             cryptoKeyId;
    Crypto_JobRedirectionInfoType*     jobRedirectionInfoRef;
    uint32                             targetCryptoKeyId;
    uint32                             jobPriority;
} Crypto_JobType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_GENERALTYPES_H */
