/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*********************************************************************************
**                                                                            **
**  FILENAME    : Csm.H                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : implementation header for CSM                               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.4.0                       **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/

#ifndef CSM_H
#define CSM_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Csm_Types.h"
#include "Csm_Cfg.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define CSM_MODULE_ID          ((uint16)(110U))
#define CSM_VENDOR_ID          ((uint16)(62U))
#define CSM_MODULE_INSTANCE_ID ((uint8)(0U))

#define CSM_H_AR_MAJOR_VERSION 4U
#define CSM_H_AR_MINOR_VERSION 5U
#define CSM_H_AR_PATCH_VERSION 0U
#define CSM_H_SW_MAJOR_VERSION 2U
#define CSM_H_SW_MINOR_VERSION 1U
#define CSM_H_SW_PATCH_VERSION 3U

#if (STD_ON == CSM_DEV_ERROR_DETECT)
/*Development errors used by the CSM module*/
#define CSM_E_PARAM_POINTER   ((uint8)0x01)
#define CSM_E_PARAM_HANDLE    ((uint8)0x04)
#define CSM_E_UNINIT          ((uint8)0x05)
#define CSM_E_INIT_FAILED     ((uint8)0x07)
#define CSM_E_PROCESSING_MODE ((uint8)0x08)
#define CSM_E_SERVICE_TYPE    ((uint8)0x09)

/*service id*/
#define CSM_SID_INIT                 ((uint8)0x00)
#define CSM_SID_GETVERSIONINFO       ((uint8)0x3B)
#define CSM_SID_HASH                 ((uint8)0x5D)
#define CSM_SID_MAC_GEN              ((uint8)0x60)
#define CSM_SID_MAC_VERIFY           ((uint8)0x61)
#define CSM_SID_ENCRYPT              ((uint8)0x5E)
#define CSM_SID_DECRYPT              ((uint8)0x5F)
#define CSM_SID_AEADENCRYPT          ((uint8)0x62)
#define CSM_SID_AEADDECRYPT          ((uint8)0x63)
#define CSM_SID_SIG_GEN              ((uint8)0x76)
#define CSM_SID_SIG_VERIFY           ((uint8)0x64)
#define CSM_SID_RANDOM_GEN           ((uint8)0x72)
#define CSM_SID_KEYELEMENTSET        ((uint8)0x78)
#define CSM_SID_KEYSET_VALID         ((uint8)0x67)
#define CSM_SID_KEYELEMENTGET        ((uint8)0x68)
#define CSM_SID_KEYELEMENTCPY        ((uint8)0x71)
#define CSM_SID_KEYCPY               ((uint8)0x73)
#define CSM_SID_KEYELEMENTCPYPARTIAL ((uint8)0x79)
#define CSM_SID_RANDOMSEED           ((uint8)0x69)
#define CSM_SID_KEY_GENERATE         ((uint8)0x6A)
#define CSM_SID_KEY_DERIVE           ((uint8)0x6B)
#define CSM_SID_KEY_EX_PUB           ((uint8)0x6C)
#define CSM_SID_KEY_EX_SEC           ((uint8)0x6D)
#define CSM_SID_JOBKEYSETVALID       ((uint8)0x7A)
#define CSM_SID_JOBRANDOMSEED        ((uint8)0x7B)
#define CSM_SID_JOBKEYGENERATE       ((uint8)0x7C)
#define CSM_SID_JOBKEYDERIVE         ((uint8)0x7D)
#define CSM_SID_JOBKEY_EX_PUB        ((uint8)0x7E)
#define CSM_SID_JOBKEY_EX_SEC        ((uint8)0x7F)
#define CSM_SID_JOBCERTIFICATEPARSE  ((uint8)0x80)
#define CSM_SID_JOBCERTIFICATEVERIFY ((uint8)0x81)
#define CSM_SID_CANCELJOB            ((uint8)0x6F)
#define CSM_SID_CBK_NOTIFY           ((uint8)0x70)
#define CSM_SID_MAINFUNCTION         ((uint8)0x01)
#endif /*STD_ON == CSM_DEV_ERROR_DETECT*/
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
typedef uint8 Csm_ConfigType;
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

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
Csm_Init(P2CONST(Csm_ConfigType, AUTOMATIC, CSM_APPL_DATA) configPtr);
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
Csm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CSM_APPL_DATA) versioninfo);
#endif

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
FUNC(Std_ReturnType, CSM_CODE)
Csm_Hash(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr);

#endif

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
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size��
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element��
 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_MacGenerate(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) macPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) macLengthPtr);

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
 *    CRYPTO_E_KEY_NOT_VALID: request failed, the key's state is "invalid"��
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size��
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
    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_DATA) verifyPtr);

#endif

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
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size��
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element��
 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_Encrypt(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr);

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
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size��
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element��
 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_Decrypt(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) dataPtr,
    VAR(uint32, CSM_APPL_DATA) dataLength,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr);
#endif

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
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) tagLengthPtr);

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
    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_DATA) verifyPtr);
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
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr);

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
    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_DATA) verifyPtr);
#endif

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
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr);
#endif

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
    VAR(uint32, CSM_APPL_DATA) keyLength);

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
Csm_KeySetValid(VAR(uint32, CSM_APPL_DATA) keyId);

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
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) keyLengthPtr);

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
    CONST(uint32, CSM_APPL_DATA) targetKeyElementId);

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
Csm_KeyCopy(CONST(uint32, CSM_APPL_DATA) keyId, CONST(uint32, CSM_APPL_DATA) targetKeyId);

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
    VAR(uint32, CSM_APPL_DATA) targetKeyElementId);

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
    VAR(uint32, CSM_APPL_DATA) seedLength);

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
Csm_KeyGenerate(VAR(uint32, CSM_APPL_DATA) keyId);

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
Csm_KeyDerive(VAR(uint32, CSM_APPL_DATA) keyId, VAR(uint32, CSM_APPL_DATA) targetKeyId);

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
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) publicValueLengthPtr);

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
    VAR(uint32, CSM_APPL_DATA) partnerPublicValueLength);

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
Csm_JobKeySetValid(VAR(uint32, CSM_APPL_DATA) jobId, VAR(uint32, CSM_APPL_DATA) keyId);

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
    VAR(uint32, CSM_APPL_DATA) seedLength);

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
Csm_JobKeyGenerate(VAR(uint32, CSM_APPL_DATA) jobId, VAR(uint32, CSM_APPL_DATA) keyId);

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
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
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
    VAR(uint32, CSM_APPL_DATA) targetKeyId);

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
 *    CRYPTO_E_BUSY:  Request failed, service is still busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid"
 *    CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_JobKeyExchangeCalcPubVal(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(uint32, CSM_APPL_DATA) keyId,
    P2VAR(uint8, AUTOMATIC, CSM_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) publicValueLengthPtr);

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
 *    E_BUSY:Request failed, Crypto Driver Object is busy
 *    CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *    CRYPTO_E_BUSY: Request failed, service is still busy
 *    CRYPTO_E_KEY_NOT_VALID: Request failed, the key's state is "invalid".
 *    CRYPTO_E_KEY_EMPTY:Request failed because of uninitialized source key element.
 */
FUNC(Std_ReturnType, CSM_CODE)
Csm_JobKeyExchangeCalcSecret(
    VAR(uint32, CSM_APPL_DATA) jobId,
    VAR(uint32, CSM_APPL_DATA) keyId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, CSM_APPL_DATA) partnerPublicValueLength);

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
Csm_CancelJob(VAR(uint32, CSM_APPL_DATA) job, VAR(Crypto_OperationModeType, CSM_APPL_DATA) mode);

#endif /* CSM_H */
