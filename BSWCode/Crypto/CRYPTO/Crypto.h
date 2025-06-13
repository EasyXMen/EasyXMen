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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Crypto.h                                                    **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuhao.ge                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : implementation header for CRYPTO                            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.4.0                       **
**                                                                            **
*******************************************************************************/
#ifndef CRYPTO_H
#define CRYPTO_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Crypto_ISoft_Cfg.h"
#include "CSM_Types.h"
#include "CryIf.h"
#include "Std_Types.h"
#include "Crypto_Types.h"
#include "SchM_Crypto_ISoft.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID                   27
#define CRYPTO_AR_RELEASE_MAJOR_VERSION    4
#define CRYPTO_AR_RELEASE_MINOR_VERSION    0
#define CRYPTO_AR_RELEASE_REVISION_VERSION 3
#define CRYPTO_SW_MAJOR_VERSION            2
#define CRYPTO_SW_MINOR_VERSION            1
#define CRYPTO_SW_PATCH_VERSION            2
#define CRYPTO_PRODUCT_ID                  51
#define CRYPTO_MODULE_ID                   114
#define CRYPTO_MODULE_INSTANCE_ID          ((uint8)0)

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*Development errors used by the CRYPTO module*/
#define CRYPTO_E_UNINIT        ((uint8)0x00)
#define CRYPTO_E_INIT_FAILED   ((uint8)0x01)
#define CRYPTO_E_PARAM_POINTER ((uint8)0x02)
#define CRYPTO_E_PARAM_HANDLE  ((uint8)0x04)
#define CRYPTO_E_PARAM_VALUE   ((uint8)0x05)

/*runtime errors*/
#define CRYPTO_E_RE_KEY_NOT_AVAILABLE ((uint8)0x00)
#define CRYPTO_E_RE_KEY_READ_FAIL     ((uint8)0x01)

/*service id*/
#define CRYPTO_SID_INIT                  ((uint8)0x00)
#define CRYPTO_SID_GETVERSIONINFO        ((uint8)0x01)
#define CRYPTO_SID_PROCESSJOB            ((uint8)0x03)
#define CRYPTO_SID_CANCELJOB             ((uint8)0x0E)
#define CRYPTO_SID_KEYELEMENTSET         ((uint8)0x04)
#define CRYPTO_SID_KEYSETVALID           ((uint8)0x05)
#define CRYPTO_SID_KEYELEMENTGET         ((uint8)0x06)
#define CRYPTO_SID_KEYELEMENTCOPY        ((uint8)0x0F)
#define CRYPTO_SID_KEYELEMENTCOPYPARTIAL ((uint8)0x13)
#define CRYPTO_SID_KEYCOPY               ((uint8)0x10)
#define CRYPTO_SID_KEYELEMEMTIDSGET      ((uint8)0x11)
#define CRYPTO_SID_RANDOMSEED            ((uint8)0x0D)
#define CRYPTO_SID_KEYGENERATE           ((uint8)0x07)
#define CRYPTO_SID_KEYDERIVE             ((uint8)0x08)
#define CRYPTO_SID_KEYEXCHANGECALCPUBVAL ((uint8)0x09)
#define CRYPTO_SID_KEYEXCHANGECALCSECRET ((uint8)0x0A)
#define CRYPTO_SID_CERTIFICATE_PARSE     ((uint8)0x0B)
#define CRYPTO_SID_CERTIFICATE_VERIFY    ((uint8)0x12)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

typedef uint8 Crypto_ConfigType;

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * Initializes the Crypto Driver
 * Service ID: 0x00
 * Sync/Async: 0x00
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *    @configPtr, Pointer to a selected configuration structure.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    void
 */

/* @req SRS_BSW_00101,SRS_BSW_00358,SRS_BSW_00414*/
FUNC(void, CRYPTO_CODE) Crypto_Init(P2CONST(Crypto_ConfigType, AUTOMATIC, CRYPTO_APPL_DATA) configPtr);
/**
 * Returns the version information of this module.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @versioninfo,Pointer to a selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */

#if (CRYPTO_VERSION_INFO_API == STD_ON)
/* @req SWS_Crypto_910011,SRS_BSW_00407 */
FUNC(void, CRYPTO_CODE) Crypto_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_APPL_DATA) versioninfo);
#endif

/**
 * Performs the crypto primitive,that is configured in the job parameter.
 * Service ID: 0x03
 * Sync/Async: Sync or Async,depends on the job configuration
 * Reentrancy: Reentrant
 * Parameters(IN): @objectId,Holds the identifier of the Crypto Driver Object.
 * Parameters(INOUT): @job,Pointer to the configuration of the job.
 * Contains structures with job and primitive relevant information but also pointer to result buffers.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType
 *      E_OK: Request successful
 *      E_NOT_OK: Request failed
 *      CRYPTO_E_BUSY: Request failed,Crypro Driver Object is busy
 *      CRYPTO_E_KEY_NOT_VALID: Request failed,the key is not valid
 *      CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,a key element has the wrong size
 *      CRYPTO_E_QUEUE_FULL: Request failed,the queue is full
 *      CRYPTO_E_KEY_READ_FAIL: The service request failed,because key element extraction is not allowed
 *      CRYPTO_E_KEY_WRITE_FAIL: The service request failed because the writing access failed
 *      CRYPTO_E_KEY_NOT_AVAILABLE: The service request failed because the key is not available
 *      CRYPTO_E_ENTROPY_EXHAUSTION: Request failed,the entropy is exhausted
 *      CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *      CRYPTO_E_JOB_CANCELED: The service request failed because the synchronous Job has been canceled
 *      CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

/* @req SWS_Crypto_91003 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_ProcessJob(VAR(uint32, AUTOMATIC) objectId, P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job);

/**
 * This interface cancels the processing of the job if possible.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant,but not for same Crypto Driver Object
 * Parameters(IN): @objectId,Holds the identifier of the Crypto Driver Object.
 * Parameters(INOUT): @job,Pointer to the configuration of the job.
 * Contains structures with job and primitive relevant information but also pointer to result buffers.
 * Parameters(OUT): NA
 * Return value: Std_ReturnType
 *      E_OK: Request successful
 *      E_NOT_OK: Request failed
 *      CRYPTO_E_JOB_CANCELED: The job has been cancelled but is still processed.
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_CancelJob(VAR(uint32, AUTOMATIC) objectId, P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_APPL_DATA) job);

/**
 * Sets the given key element bytes to the key identified by keyId.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *    @keyId,Holds the identifier of the key for which a new material shall be set.
 *    @keyElementId,Holds the identifier of the key element to be written.
 *    @keyPtr,Holds the pointer to the key element bytes to be processed.
 *    @keyLength,Contains the number of key element bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *    E_OK: request successful
 *    E_NOT_OK: request failed
 *    CRYPTO_E_BUSY: request failed,service is still busy
 *    CRYPTO_E_KEY_WRITE_FAIL:Request failed because write access was denied
 *    CRYPTO_E_KEY_NOT_AVAILABLE: Request failed because the key is not available.
 *    CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element size does not match size of provided data.
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementSet(
    uint32 cryptokeyId,
    uint32 keyElementId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) keyPtr,
    uint32 keyLength);

/**
 * @brief    CRYPTO driver key validation interface.
 * @details  This interface sets the key state of the key identified by cryptoKeyId to valid.
 *
 * @param[in]       cryptoKeyId   Holds the identifier of the key which shall be set to valid.
 * @param[in,out]   None.
 * @param[out]      None.
 ** @return          Std_ReturnType
 * @retval          E_OK:          Request successful
 * @retval          E_NOT_OK:      Request Failed
 * @retval          CRYPTO_E_BUSY: Request Failed,Crypto Driver Object is Busy
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_KeySetValid(VAR(uint32, AUTOMATIC) cryptoKeyId);

/**
 * @brief    CRYPTO driver key element extraction interface.
 * @details  This interface shall be used to get a key element of the key identified by the cryptoKeyId
 *           and store the key element in the memory location pointed by the result pointer.
 *
 * @param[in]       cryptoKeyId     Holds the identifier of the key whose key element shall be returned.
 * @param[in]       keyElementId    Holds the identifier of the key element which shall be returned.
 * @param[in,out]   resultLengthPtr Holds a pointer to a memory location in which the length information is stored.
 *                                  On calling this function this parameter shall contain the size of the buffer
 * provided by resultPtr. If the key element is configured to allow partial access,this parameter contains the amount of
 * data which should be read from the key element. The size may not be equal to the size of the provided buffer anymore.
 *                                  When the request has finished,the amount of data that has been stored shall be
 * stored.
 * @param[out]      resultPtr       Holds the pointer of the buffer for the returned key element
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed because read access was denied
 * @retval          E_NOT_OK:      The provided buffer is too small to store the result
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementGet(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    VAR(uint32, AUTOMATIC) keyElementId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) resultLengthPtr);

/**
 * @brief    CRYPTO driver key element copy interface.
 * @details  This interface copies a key element to another key element in the same crypto driver.
 *
 * @param[in]       cryptoKeyId        Holds the identifier of the key whose key element shall be the source element.
 * @param[in]       keyElementId       Holds the identifier of the key element which shall be the source for the copy
 * operation.
 * @param[in]       targetCryptoKeyId  Holds the identifier of the key whose key element shall be the destination
 * element.
 * @param[in]       targetKeyElementId Holds the identifier of the key element which shall be the destination for the
 * copy operation.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementCopy(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    VAR(uint32, AUTOMATIC) keyElementId,
    VAR(uint32, AUTOMATIC) targetCryptoKeyId,
    VAR(uint32, AUTOMATIC) targetKeyElementId);

/**
 * @brief    CRYPTO driver key element copy interface.
 * @details  Copies a key element to another key element in the same crypto driver.
 *           The keyElementSourceOffset and keyElementCopyLength allows to copy just a part of the source key element
 * into the destination The offset of the target key is also specified with this function. If the actual key element is
 * directly mapped to flash memory, there could be a bigger delay when calling this function(synchronous operation).
 *
 * @param[in]       cryptoKeyId                   Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       keyElementId                  Holds the identifier of the key element which shall be the source for
 * the copy operation.
 * @param[in]       keyElementSourceOffset        This is the offset of the of the source key element indicating the
 * start index of the copy operation.
 * @param[in]       keyElementTargetOffset        This is the offset of the of the target key element indicating the
 * start index of the copy operation.
 * @param[in]       keyElementCopyLength          Specifies the number of bytes that shall be copied.
 * @param[in]       targetCryptoKeyId             Holds the identifier of the key whose key element shall be the
 * destination element.
 * @param[in]       targetKeyElementId            Holds the identifier of the key element which shall be the destination
 * for the copy operation.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementCopyPartial(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    VAR(uint32, AUTOMATIC) keyElementId,
    VAR(uint32, AUTOMATIC) keyElementSourceOffset,
    VAR(uint32, AUTOMATIC) keyElementTargetOffset,
    VAR(uint32, AUTOMATIC) keyElementCopyLength,
    VAR(uint32, AUTOMATIC) targetCryptoKeyId,
    VAR(uint32, AUTOMATIC) targetKeyElementId);

/**
 * @brief    CRYPTO driver key copy interface.
 * @details  Copies a key with all its elements to another key in the same crypto driver.
 *           Note: If the actual key element is directly mapped to flash memory,
 *           there could be a bigger delay when calling this function(synchronous operation)
 *
 * @param[in]       cryptoKeyId                   Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       targetCryptoKeyId             Holds the identifier of the key whose key element shall be the
 * destination element.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyCopy(VAR(uint32, AUTOMATIC) cryptoKeyId, VAR(uint32, AUTOMATIC) targetCryptoKeyId);

/**
 * @brief    CRYPTO driver key get interface.
 * @details  Used to retrieve information which key elements are available in a given key.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       keyElementIdsLengthPtr       Holds a pointer to the memory location in which the number of key
 * elements in the given key is stored. On calling this function, this parameter shall contain the size of the buffer
 * provided by keyElementIdsPtr. When the request has finished,the actual number of key elements shall be stored.
 * @param[in,out]   None.
 * @param[out]      keyElementIdsPtr             Contains the pointer to the array where the ids of the key elements
 * shall be stored.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementIdsGet(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) keyElementIdsPtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) keyElementIdsLengthPtr);

/**
 * @brief    CRYPTO driver key generate interface.
 * @details  This function generates the internal seed state using the provided entropy source.
 *           Furthermore,this function can be used to update the seed state with new entropy
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       seedPtr                      Holds a pointer to the memory location which contains the data to feed
 * the seed.
 * @param[in]       seedLength                   Contains the length of the seed in bytes.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_RandomSeed(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) seedPtr,
    VAR(uint32, AUTOMATIC) seedLength);

/**
 * @brief    CRYPTO driver key generate interface.
 * @details  Generates new key material store it in the key identified by cryptoKeyId.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_KeyGenerate(VAR(uint32, AUTOMATIC) cryptoKeyId);
/**
 * @brief    CRYPTO driver key Derivation interface.
 * @details  Derives a new key by using the key elements in the given key identified by the cryptoKeyId.
 *           The given key contains the key elements for the password,salt.
 *           The derived key is stored in the key element with the id 1 of the key identified by targetCryptoKeyId.
 *           The number of iterations is given in the key element CRYPTO_KE_KEYDERIVATION_ITERATIONS.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       targetCryptoKeyId            Holds the identifier of the key whose key element shall be the
 * destination element.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyDerive(VAR(uint32, AUTOMATIC) cryptoKeyId, VAR(uint32, AUTOMATIC) targetCryptoKeyId);

/**
 * @brief    CRYPTO driver key Exchange interface.
 * @details  Calculates the public value for the key exchange and
 *           stores the public key in the memory location pointed by the public value pointer.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 *
 * @param[in,out]   publicValueLengthPtr         Holds a pointer to the memory location in which the public value length
 * information is stored.
 * @param[out]      publicValuePtr               Contains the pointer to the data where the public value shall be
 * stored.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyExchangeCalcPubVal(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) publicValueLengthPtr);

/**
 * @brief    CRYPTO driver key Exchange interface.
 * @details  Calculates the shared secret key for the key exchange with the key material of the key identified by the
 * cryptoKeyId and the partner public key. The shared secret key is stored as a key element in the same key.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       partnerPublicValuePtr        Holds the pointer to the memory location which contains the partner's
 * public value.
 * @param[in]       partnerPublicValueLength     Contains the length of the partner's public value in bytes.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyExchangeCalcSecret(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, AUTOMATIC) partnerPublicValueLength);

/**
 * @brief    CRYPTO driver key Certificate interface.
 * @details  Parses the certificate data stored in the key element CRYPTO_KE_CERT_DATA and fills the key elements
 *           CRYPTO_KE_CERT_SIGNEDDATA,CRYPTO_KE_CERT_PARSEDPUBLICKEY and CRYPTO_KE_CERT_SIGNATURE.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_CertificateParse(VAR(uint32, AUTOMATIC) cryptoKeyId);

/**
 * @brief    CRYPTO driver key Certificate interface.
 * @details  Verifies the certificate stored in the key referenced by cryptoValidateKeyId with the certificate stored in
 * the key referenced by cryptoKeyId.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       verifyCryptoKeyId            Holds the identifier of the key contain
 *
 * @param[in,out]   None.
 * @param[out]      verifyPtr                    Holds a pointer to the memory location which will contain the result of
 * the certificate verification.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_CertificateVerify(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    VAR(uint32, AUTOMATIC) verifyCryptoKeyId,
    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_APPL_DATA) verifyPtr);

#endif
