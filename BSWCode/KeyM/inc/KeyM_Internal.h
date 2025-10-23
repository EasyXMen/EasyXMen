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
 *
 ************************************************************************************************************************
 **
 **  @file               : KeyM_Internal.h
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : KeyM internal header for internal API declarations
 **
 ***********************************************************************************************************************/

#ifndef KEYM_INTERNAL_H
#define KEYM_INTERNAL_H

/* =================================================== inclusions =================================================== */
#include "KeyM.h"
#include "NvM.h"
#include "Csm.h"
#include "KeyM_Cfg.h"
#include "KeyM_Externals.h"
#include "SchM_KeyM.h"
#if (STD_ON == KEYM_DEVERROR_DETECT)
#include "Det.h"
#endif
#if (STD_ON == KEYM_SECURITY_ENENT_REPORT_ENABLED)
#include "IdsM.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================ macros  ================================================ */

#define KEYM_SHEKEY_INPUT_LEN (64U) /**< Length of the SHE key input. */

#define KEYM_SHEKEY_SLOTID_OFFSET (15U) /**< Offset for the SHE key slot ID. */

#define KEYM_SHEKEY_SLOTID_MASK (0xf0U) /**< Mask for the SHE key slot ID. */

#define KEYM_CONST_1 (uint8)(0x1U) /**< Constant value 1. */

#define KEYM_CONST_2 (uint8)(0x2U) /**< Constant value 2. */

#define KEYM_CONST_3 (uint8)(0x3U) /**< Constant value 3. */

#define KEYM_CONST_4 (uint8)(0x4U) /**< Constant value 4. */

#define KEYM_CONST_5 (uint8)(0x5U) /**< Constant value 5. */

#define KEYM_INVALID_U16 (0xffffU) /**< Invalid 16-bit value. */

#define KEYM_INVALID_U32 (0xffffffffU) /**< Invalid 32-bit value. */

#define KEYM_NVM_TIMER_STOP (0U) /**< Value to stop the NVM timer. */

#define KEYM_NVM_TIMER_START (1U) /**< Value to start the NVM timer. */

#if !defined(KEYM_LOCAL)
#define KEYM_LOCAL static /**< Local storage class specifier. */ /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

#if (STD_ON == KEYM_DEVERROR_DETECT)

/**
 * @brief          Reports an error to the DET (Diagnostic Error Trap) module.
 * @param[in]      ApiId : API identifier of the function that detected the error.
 * @param[in]      ErrorId : Error identifier specifying the type of error.
 * @return         None
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71422
 */
static inline void KEYM_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(KEYM_MODULE_ID, KEYM_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif /* STD_ON == KEYM_DEVERROR_DETECT */

/* ================================================ type definitions ================================================ */

/*******************************Key Sub Module*********************************/
/**
 * @brief Enumeration of crypto CSM verify job types.
 */
typedef enum
{
    /** This job type represents an AEAD (Authenticated Encryption with Associated Data) decryption verification.*/
    KEYM_VERIFY_AEADDECRYPT,

    /** This job type represents an AEAD (Authenticated Encryption with Associated Data) encryption verification.*/
    KEYM_VERIFY_AEADENCRYPT,

    /** This job type represents a decryption verification.*/
    KEYM_VERIFY_DECRYPT,

    /** This job type represents an encryption verification.*/
    KEYM_VERIFY_ENCRYPT,

    /** This job type represents a MAC (Message Authentication Code) generation verification.*/
    KEYM_VERIFY_MACGENERATE,

    /** This job type represents a MAC (Message Authentication Code) verification.*/
    KEYM_VERIFY_MACVERIFY
} KeyM_CryptoCsmVerifyJobType;

/**
 * @brief Enumeration of crypto key generation types.
 */
typedef enum
{
    /** This type represents key generation through derivation.*/
    KEYM_DERIVED_KEY,

    /** This type represents a key that is stored in a secure location.*/
    KEYM_STORED_KEY
} KeyM_CryptoKeyGenerationType;

/**
 * @brief Enumeration of storage types for keys.
 */
typedef enum
{
    /** This type represents storage of keys in the Crypto Service Manager (CSM).*/
    KEYM_STORAGE_IN_CSM,

    /** This type represents storage of keys in Non-Volatile Memory (NVM).*/
    KEYM_STORAGE_IN_NVM,

    /** This type represents storage of keys in Random Access Memory (RAM).*/
    KEYM_STORAGE_IN_RAM
} KeyM_StorageType;

/**
 * @brief Structure to hold configuration data for a crypto key.
 */
typedef struct
{
    /**  This field holds the unique identifier for the key.*/
    uint32 keyId;

    /**  This field holds the maximum length of the key in bytes.*/
    uint32 keyMaxLen;

    /**  This field holds a pointer to the name of the key.*/
    const uint8* keyName;

    /**  This field holds the length of the key name in bytes.*/
    uint16 KeyNameLen;

#if (STD_ON == KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED)
    /**  This field specifies the type of verification job used for the key verification operation.*/
    KeyM_CryptoCsmVerifyJobType verJobType;

    /** This field holds the reference to the verification job.*/
    uint32 verifyJobRef;
#endif

    /**  This field holds a pointer to the properties of the key.*/
    const uint8* keyProps;

    /**  This field holds a pointer to the key generation information.*/
    const uint8* keyGenInfo;

    /**  This field holds the length of the key generation information in bytes.*/
    uint32 keyGenInfoLen;

    /**  This field specifies the type of key generation method used.*/
    KeyM_CryptoKeyGenerationType keyGenType;

    /**  This field specifies the storage type for the key.*/
    KeyM_StorageType keyStorage;

    /**  This field holds a pointer to the reference of the source key used for derivation.*/
    const uint32* keySrcDervRef;

    /** This field holds a pointer to the reference of the target key.*/
    const uint32* keyTargetRef;

    /** This field holds a pointer to the reference of the NVM block.*/
    const uint16* nvmBlkRef;
} KeyM_CryptoKeyPCfgType;

/**
 * @brief Structure to hold configuration data for an NVM block.
 */
typedef struct
{
    /** This field holds the write delay for the NVM block in milliseconds.*/
    uint16 blkWriDelay;

    /** This field holds the unique identifier for the NVM block.*/
    NvM_BlockIdType blkId;
} KeyM_NvmBlockPCfgType;

/***************************Certificate Sub Module*****************************/

/**
 * @brief   Enumeration of certificate algorithms.
 */
typedef enum
{
    /** This type represents the Elliptic Curve Cryptography algorithm.*/
    KEYM_CERT_ALGO_ECC,

    /** This type represents the Rivest-Shamir-Adleman algorithm.*/
    KEYM_CERT_ALGO_RSA
} KeyM_CertAlgorithmType;

/**
 * @brief   Enumeration of certificate formats.
 */
typedef enum
{
    /** This type represents the Certificate Revocation List format.*/
    KEYM_CERT_FORMAT_CRL,

    /** This type represents the Card Verifiable Certificate format.*/
    KEYM_CERT_FORMAT_CVC,

    /** This type represents the X.509 certificate format.*/
    KEYM_CERT_FORMAT_X509
} KeyM_CertFormatType;

/* PRQA S 0779 ++ */ /* VL_KeyM_0779 */
/**
 * @brief   Enumeration of certificate element structures.
 */
typedef enum
{
    /** This type represents a certificate extension.*/
    CERTIFICATE_EXTENSION,

    /** This type represents the issuer name of the certificate.*/
    CERTIFICATE_ISSUER_NAME,

    /** This type represents the unique identifier of the certificate issuer.*/
    CERTIFICATE_ISSUER_UNIQUE_IDENTIFIER,

    /** This type represents the serial number of the certificate.*/
    CERTIFICATE_SERIAL_NUMBER,

    /** This type represents the signature of the certificate.*/
    CERTIFICATE_SIGNATURE,

    /** This type represents the signature algorithm used in the certificate.*/
    CERTIFICATE_SIGNATURE_ALGORITHM,

    /** This type represents the identifier of the signature algorithm used in the certificate.*/
    CERTIFICATE_SIGNATURE_ALGORITHM_ID,

    /** This type represents the authorization of the certificate subject.*/
    CERTIFICATE_SUBJECT_AUTHORIZATION,

    /** This type represents the name of the certificate subject.*/
    CERTIFICATE_SUBJECT_NAME,

    /** This type represents the public key algorithm in the subject public key info of the certificate.*/
    CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_PUBLIC_KEY_ALGORITHM,

    /** This type represents the subject public key in the subject public key info of the certificate.*/
    CERTIFICATE_SUBJECT_PUBLIC_KEY_INFO_SUBJECT_PUBLIC_KEY,

    /** This type represents the unique identifier of the certificate subject.*/
    CERTIFICATE_SUBJECT_UNIQUE_IDENTIFIER,

    /** This type represents the end date of the certificate validity period.*/
    CERTIFICATE_VALIDITY_PERIOD_NOT_AFTER,

    /** This type represents the start date of the certificate validity period.*/
    CERTIFICATE_VALIDITY_PERIOD_NOT_BEFORE,

    /** This type represents the version number of the certificate.*/
    CERTIFICATE_VERSION_NUMBER,

    /** This type represents the revoked status of the certificate.*/
    CERTIFICATE_REVOKED
} KeyM_CertEleStructType;
/* PRQA S 0779 -- */

/**
 * @brief Structure to hold certificate element information.
 */
typedef struct
{
    /** This field is a boolean flag that indicates whether the certificate element has iteration.*/
    boolean certEleHasIteration;

    /** This field holds the unique identifier for the certificate element.*/
    uint16 certEleId;

    /** This field holds the maximum length of the certificate element in bytes.*/
    uint16 certEleMaxLen;

    /** This field holds a pointer to the object type of the certificate element.*/
    const uint8* certEleObjType;

    /** This field specifies the structure type of the certificate element.*/
    KeyM_CertEleStructType certEleStruct;

    /** This field holds the length of the object identifier of the certificate element in bytes.*/
    uint16 certEleObjIdLen;

    /** This field holds a pointer to the object identifier of the certificate element.*/
    uint8* certEleObjId;

    /** This field holds a pointer to the buffer containing the data of the certificate element.*/
    KeyM_Asn1DesType* certEleBuf;
} KeyM_CertEleType;

/**
 * @brief Structure to hold certificate configuration data.
 */
typedef struct KeyM_CertType
{
    /** This field specifies the cryptographic algorithm used in the certificate.*/
    KeyM_CertAlgorithmType certAlgoT;

    /** This field specifies the format of the certificate.*/
    KeyM_CertFormatType certFormatT;

    /** This field holds the unique identifier for the certificate.*/
    uint16 certId;

    /** This field holds a pointer to the name of the certificate.*/
    const uint8* certName;

    /** This field specifies the storage type for the certificate.*/
    KeyM_StorageType certStorage;

    /** This field holds a pointer to the callback function used for certificate verification.*/
    Std_ReturnType (*certVeriCbkFunc)(KeyM_CertificateIdType CertId, KeyM_CertificateStatusType Result);

    /** This field holds a pointer to the service certificate callback function.*/
    void (*servCertCbkFunc)(
        KeyM_CertificateIdType CertId,
        KeyM_ResultType        Result,
        uint16                 ResultDataLength,
        const uint8*           ResultDataPtr);

    /** This field holds a pointer to the rule arbitration function related to the certificate.*/
    boolean (*certRuleArbiFunc)(void);

    /** This field holds a pointer to the reference of the signature generation job.*/
    const uint32* certSigGenJobRef;

    /** This field holds the signature verification job.*/
    uint32 certSigVeriJob;

    /** This field holds the signature verification key.*/
    uint32 certSigVeriKey;

    /** This field holds a pointer to the reference of the target key.*/
    const uint32* certKeyTarRef;

    /** This field holds a pointer to the reference of the NVM block.*/
    const uint16* certNvmBlkRef;

    /** This field holds a pointer to the reference of the private key.*/
    const uint32* certPriKeyRef;

    /** This field holds the stability time of the certificate in milliseconds.*/
    uint16 certStbMTime;

    /** This field holds the number of certificate elements.*/
    uint16 numOfCertEle;

    /** This field holds a pointer to the reference of the certificate elements.*/
    const KeyM_CertEleType* certEleRef;

    /** This field holds the maximum length of the certificate body in bytes.*/
    uint16 certMaxLen;

    /** This field holds a pointer to the buffer containing the certificate body (To Be Signed part).*/
    KeyM_Asn1DesType* tbsCert;

    /** This field holds a pointer to the reference of the upper hierarchy certificate.*/
    const struct KeyM_CertType* certUpperHierRef;

    /** This field is a boolean flag that indicates whether the certificate is self-signed.*/
    boolean selfSignedCert;
} KeyM_CertPCfgType;

/**
 * @brief Structure to hold certificate time information.
 */
typedef struct
{
    /** This field holds the year component of the certificate time.*/
    uint32 certYear;

    /** This field holds the month component of the certificate time (1-12).*/
    uint32 certMon;

    /** This field holds the day component of the certificate time (1-31).*/
    uint32 certDay;

    /** This field holds the hour component of the certificate time (0-23).*/
    uint32 certHour;

    /** This field holds the minute component of the certificate time (0-59).*/
    uint32 certMin;
    /** This field holds the second component of the certificate time (0-59).*/
    uint32 certSec;
} KeyM_CertTimeType;

/**
 * @brief Structure to hold NVM block write information.
 */
typedef struct
{
    /** This field holds a pointer to the certificate element that needs to be written to the NVM block.*/
    KeyM_Asn1DesType* nvmCertEle;

    /** This field holds the delay time in milliseconds before writing to the NVM block.*/
    uint16 delayTime;

    /** This field is a boolean flag that indicates whether the delay time has elapsed and the write operation has
     * started.*/
    boolean timeStart;
} KeyM_BlkWriType;

/* ========================================== internal function definitions ========================================= */

/* ========================================= external data declarations ========================================= */
/**
 * @brief Initialization status of the KeyM module.
 */
extern boolean KeyM_InitStatus;

/**
 * @brief Session open status of the KeyM module.
 */
extern boolean KeyM_SessionOpen;

/**
 * @brief Array of crypto key configuration data.
 */
extern const KeyM_CryptoKeyPCfgType KeyM_CryptoKeyCfg[KEYM_KEY_NUM];

#if (KEYM_NVM_BLOCK_NUM > 0)
extern const KeyM_NvmBlockPCfgType KeyM_NvmBlockPCfg[KEYM_NVM_BLOCK_NUM];
#endif
#if (KEYM_CERT_NUM > 0)
extern const KeyM_CertPCfgType KeyM_CertPCfg[KEYM_CERT_NUM];

extern KeyM_CertTimeType KeyM_CertStartTime[KEYM_CERT_NUM];

extern KeyM_CertTimeType KeyM_CertEndTime[KEYM_CERT_NUM];

extern uint32 KeyM_CertVersion[KEYM_CERT_NUM];
#endif
/* ========================================= external function declarations ========================================= */
/**
 * @brief          Copies data from the source buffer to the destination buffer.
 *                 This function performs a byte-wise copy of the specified number of bytes
 *                 from the source buffer to the destination buffer.
 * @param[in]      dest : Pointer to the destination buffer.
 * @param[in]      src : Pointer to the source buffer.
 * @param[in]      size : Number of bytes to copy.
 * @return         None
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71423
 */
void KeyM_CopyData(void* dest, const void* src, uint32 size);

/**
 * @brief          Compares two strings of a specified length.
 *                 This function compares the specified number of bytes from two strings.
 *                 If the strings are identical up to the specified length, it returns E_OK.
 *                 Otherwise, it returns E_NOT_OK.
 * @param[in]      str1 : Pointer to the first string.
 * @param[in]      str2 : Pointer to the second string.
 * @param[in]      size : Number of bytes to compare.
 * @return         Std_ReturnType
 * @retval         E_OK:The strings are identical up to the specified length.
 * @retval         E_NOT_OK:The strings are not identical up to the specified length.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71424
 */
Std_ReturnType KeyM_strcmp(const uint8* str1, const uint8* str2, uint16 size);

/**
 * @brief          Handles the update of a cryptographic key.
 *                 This function updates a cryptographic key based on the provided result data.
 *                 It can store or derive the key according to the configuration.
 *                 If the key is stored in CSM or RAM, it updates the key directly.
 *                 If the key is stored in NVM, it writes the key to the specified NVM block.
 *                 If the key is derived, it performs key derivation using the provided result data and key generation
 * information.
 * @param[in]      ResultDataPtr : Pointer to the result data used for key update or derivation.
 * @param[in]      ResultDataLength : Length of the result data.
 * @param[in]      KeyIdx : Index of the cryptographic key configuration.
 * @param[in]      sheKey : Flag indicating whether the key is an SHE key.
 * @return         Std_ReturnType
 * @retval         E_OK:The key update or derivation was successful.
 * @retval         E_NOT_OK:The key update or derivation failed.
 * @retval         KEYM_E_PARAMETER_MISMATCH:Invalid parameters were provided.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71425
 */
Std_ReturnType KeyM_HandleUpdate(const uint8* ResultDataPtr, uint16 ResultDataLength, uint16 KeyIdx, boolean sheKey);

#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)

/**
 * @brief          Retrieves the SHE key for M4 and M5 operations.
 *                 This function retrieves the key element for the specified key ID,
 *                 which is used for generating M4 and M5 data in the key update process.
 *                 M4 is generated by encrypting the CID with K3, and M5 is generated by
 *                 computing the CMAC of M4 using K4, where K4 is derived from the new key.
 * @param[in]      keyId : ID of the key to retrieve.
 * @param[out]     ResponseDataPtr : Pointer to the buffer where the key element will be stored.
 * @param[inout]    ResponseMaxDataLength : Pointer to the maximum length of the response data buffer.
 *                                         On return, it contains the actual length of the key element.
 * @return         Std_ReturnType
 * @retval         E_OK:The key element was successfully retrieved.
 * @retval         E_NOT_OK:The key element retrieval failed.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71426
 */
Std_ReturnType KeyM_GetSHEKey_M4M5(uint32 keyId, uint8* ResponseDataPtr, uint16* ResponseMaxDataLength);
#endif

/**
 * @brief          Parses a certificate and extracts relevant information.
 *                 This function processes a certificate according to the X.509 standard.
 *                 It extracts the TBS (To Be Signed) certificate, version, serial number,
 *                 signature algorithm, issuer and subject names, validity period,
 *                 subject public key info, and extensions (if present).
 *                 It also validates the structure of the certificate and stores the parsed data.
 * @param[in]      CertId : ID of the certificate to parse.
 * @param[in]      certDataPtr : Pointer to the certificate data.
 * @param[in]      certDataLength : Length of the certificate data.
 * @return         Std_ReturnType
 * @retval         E_OK:The certificate was successfully parsed.
 * @retval         E_NOT_OK:The certificate parsing failed.
 * @retval         KEYM_E_KEY_CERT_INVALID:The certificate is invalid.
 * @retval         KEYM_E_CERTIFICATE_INVALID_FORMAT:The certificate has an invalid format.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71427
 */
Std_ReturnType KeyM_HandleParseCert(KeyM_CertificateIdType CertId, const uint8* certDataPtr, uint32 certDataLength);

/**
 * @brief          Handles the storage of certificate elements in the CSM.
 *                 This function processes each certificate element defined in the certificate configuration
 *                 and either sets or gets the corresponding key element in the CSM.
 *                 The operation (set or get) is determined by the `keySet` parameter.
 * @param[in]      keyId : ID of the key in the CSM where the certificate elements will be stored or retrieved.
 * @param[in]      certId : ID of the certificate configuration.
 * @param[in]      keySet : Flag indicating whether to set (TRUE) or get (FALSE) the key elements.
 * @return         None
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71428
 */
void KeyM_HandleCsmKeyStorage(uint32 keyId, uint16 certId, boolean keySet);

/**
 * @brief          Verifies a certificate against a higher-level certificate in the chain.
 *                 This function performs a series of checks to validate a certificate:
 *                 1. Subject and issuer name comparison (for self-signed certificates, subject must equal issuer).
 *                 2. Timestamp check to ensure the certificate is within its validity period.
 *                 3. Certificate element check using a custom verification function if provided.
 *                 4. Signature verification using the public key from the higher-level certificate.
 *                 If the certificate is valid, it sets the certificate status and optionally writes the public key to
 * NVM or CSM.
 * @param[in]      certCfgPtr : Pointer to the certificate configuration of the certificate to verify.
 * @param[in]      certUpperHierRef : Pointer to the certificate configuration of the higher-level certificate.
 * @return         Std_ReturnType
 * @retval         E_OK:The certificate is valid.
 * @retval         E_NOT_OK:The certificate verification failed.
 * @retval         KEYM_E_CERT_INVALID_CHAIN_OF_TRUST:The certificate chain of trust is invalid.
 * @retval         KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL:The certificate is outside its validity period.
 * @retval         KEYM_E_CERTIFICATE_INVALID_CONTENT:The certificate content is invalid.
 * @retval         KEYM_E_CERTIFICATE_SIGNATURE_FAIL:The certificate signature is invalid.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71429
 */
Std_ReturnType KeyM_HandleCertcVerify(const KeyM_CertPCfgType* certCfgPtr, const KeyM_CertPCfgType* certUpperHierRef);

/**
 * @brief          Sets the status of a certificate.
 *                 This function updates the status of a certificate in the certificate status array.
 *                 It checks if the provided certificate ID is valid before updating the status.
 * @param[in]      CertId : ID of the certificate to update the status for.
 * @param[in]      Status : New status of the certificate.
 * @return         Std_ReturnType
 * @retval         E_OK:The status was successfully updated.
 * @retval         KEYM_E_PARAMETER_MISMATCH:The provided certificate ID is invalid.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71430
 */
Std_ReturnType KeyM_CertSetStatus(KeyM_CertificateIdType CertId, KeyM_CertificateStatusType Status);

/**
 * @brief          Handles the storage of certificates in NVM with delayed write.
 *                 This function iterates through the NVM blocks and checks if any block has a delayed write pending.
 *                 If a block has a delayed write pending, it decrements the delay counter.
 *                 When the delay counter reaches zero, it writes the certificate data to the NVM block and resets the
 * delay. This ensures that the certificate data is written to NVM after a specified delay, which can be useful for
 *                 optimizing write operations and reducing wear on the NVM.
 * @return         None
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71431
 */
void KeyM_CertStoreNvmHandle(void);

#ifdef __cplusplus
}
#endif

#endif /*KEYM_INTERNAL_H*/
