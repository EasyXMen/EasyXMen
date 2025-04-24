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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : CryIf.h                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : implementation header for CryIf                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/

#ifndef CRYIF_H
#define CRYIF_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "CryIf_Cfg.h"
#include "CSM_Types.h"
#include "Std_Types.h"
#include "Rte_Csm_Type.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define CRYIF_MODULE_ID          112U
#define CRYIF_VENDOR_ID          62U
#define CRYIF_INSTANCE_ID        0U

#define CRYIF_H_AR_MAJOR_VERSION 4U
#define CRYIF_H_AR_MINOR_VERSION 5U
#define CRYIF_H_AR_PATCH_VERSION 0U
#define CRYIF_H_SW_MAJOR_VERSION 2U
#define CRYIF_H_SW_MINOR_VERSION 1U
#define CRYIF_H_SW_PATCH_VERSION 1U

#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
/*Development errors used by the CRYIF module*/
#define CRYIF_E_UNINIT            ((uint8)0x00)
#define CRYIF_E_INIT_FAILED       ((uint8)0x01)
#define CRYIF_E_PARAM_POINTER     ((uint8)0x02)
#define CRYIF_E_PARAM_HANDLE      ((uint8)0x03)
#define CRYIF_E_PARAM_VALUE       ((uint8)0x04)
#define CRYIF_E_KEY_SIZE_MISMATCH ((uint8)0x05)

/*service id*/
#define CRYIF_SID_INIT                   ((uint8)0x00)
#define CRYIF_SID_GETVERSIONINFO         ((uint8)0x01)
#define CRYIF_SID_PROCESSJOB             ((uint8)0x03)
#define CRYIF_SID_CANCELJOB              ((uint8)0x0E)
#define CRYIF_SID_KEYELEMENT_SET         ((uint8)0x04)
#define CRYIF_SID_KEYSETVALID            ((uint8)0x05)
#define CRYIF_SID_KEYELEMENT_GET         ((uint8)0x06)
#define CRYIF_SID_KEYELEMENT_COPY        ((uint8)0x0F)
#define CRYIF_SID_KEYELEMENT_COPYPARTIAL ((uint8)0x12)
#define CRYIF_SID_KEYCOPY                ((uint8)0x10)
#define CRYIF_SID_RANDOM_SEED            ((uint8)0x07)
#define CRYIF_SID_KEYGENERATE            ((uint8)0x08)
#define CRYIF_SID_KEYDERIVE              ((uint8)0x09)
#define CRYIF_SID_KEYEXCHANGECALCPUBVAL  ((uint8)0x0A)
#define CRYIF_SID_KEYEXCHANGECALCSECRET  ((uint8)0x0B)
#define CRYIF_SID_CBNOTIFICATION         ((uint8)0x0D)
#endif

#define CRYIF_SINGLE_DRIVER (0U)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

typedef uint8 CryIf_ConfigType;

typedef struct
{
    /* This parameter defines the name of the Crypto_<vi>_<ai>_ProcessJob */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_ProcessJob_Name)
    (VAR(uint32, CRYIF_APPL_DATA) channelId, P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_CancelJob */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_CancelJob_Name)
    (VAR(uint32, CRYIF_APPL_DATA) channelId, P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyElementSet */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyElementSet_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
     VAR(uint32, CRYIF_APPL_DATA) keyElementId,
     P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) keyPtr,
     VAR(uint32, CRYIF_APPL_DATA) keyLength);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeySetValid */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeySetValid_Name)(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyElementGet */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyElementGet_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
     VAR(uint32, CRYIF_APPL_DATA) keyElementId,
     P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) resultPtr,
     P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) resultLengthPtr);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyElementCopy */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyElementCopy_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryptoKeyId,
     VAR(uint32, CRYIF_APPL_DATA) keyElementId,
     VAR(uint32, CRYIF_APPL_DATA) targetCryptoKeyId,
     VAR(uint32, CRYIF_APPL_DATA) targetKeyElementId);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyElementCopyPartial */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyElementCopyPartial_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
     VAR(uint32, CRYIF_APPL_DATA) keyElementId,
     VAR(uint32, CRYIF_APPL_DATA) keyElementSourceOffset,
     VAR(uint32, CRYIF_APPL_DATA) keyElementTargetOffset,
     VAR(uint32, CRYIF_APPL_DATA) keyElementCopyLength,
     VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId,
     VAR(uint32, CRYIF_APPL_DATA) targetKeyElementId);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyCopy */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyCopy_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId, VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_RandomSeed */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_RandomSeed_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
     P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) seedPtr,
     VAR(uint32, CRYIF_APPL_DATA) seedLength);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyGenerate */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyGenerate_Name)(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyDerive */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyDerive_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId, VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyExchangeCalcPubVal */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyExchangeCalcPubVal_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
     P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) publicValuePtr,
     P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) publicValueLengthPtr);
    /* This parameter defines the name of the Crypto_<vi>_<ai>_KeyExchangeCalcSecret */
    P2FUNC(Std_ReturnType, CRYIF_APPL_CODE, Crypto_KeyExchangeCalcSecret_Name)
    (VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
     P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) partnerPublicValuePtr,
     VAR(uint32, CRYIF_APPL_DATA) partnerPublicValueLength);
} CryIf_FuncNameConfigType;

typedef struct
{
    uint32 KeyId;
    uint32 CryptoKeyId;
} CryIf_KeyCfgType;

typedef struct
{
    uint32 ChannelId;
    uint32 CryptoDriverObjectId;
} CryIf_ChannelCfgType;
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
extern CONST(CryIf_KeyCfgType, CRYIF_APPL_DATA) CryIf_KeyCfg[CRYIF_KEY_MAX_CONFIGURED];
extern CONST(CryIf_FuncNameConfigType, CRYIF_APPL_DATA) CryIf_DriverApi[CRYIF_CHANNELS_MAX_CONFIGURED];

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

/**
 * Initializes the CryIf module.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @configPtr, Pointer to a selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, CRYIF_CODE)
CryIf_Init(P2CONST(CryIf_ConfigType, AUTOMATIC, CRYIF_APPL_DATA) configPtr);

/**
 * Returns the version information of this module.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @versioninfo, Pointer to where to store the version information of this module
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
#if (CRYIF_VERSION_INFO_API == STD_ON)
FUNC(void, CRYIF_CODE)
CryIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CRYIF_APPL_DATA) versioninfo);
#endif
/**
 * This interface dispatches the received jobs to the configured crypto driver object.
 * Service ID: 0x03
 * Sync/Async: Sync or Async, depends on the configuration
 * Reentrancy: Reentrant
 * Parameters(IN): @channelId, Holds the identifier of the crypto channel.
 * Parameters(INOUT): @job, Pointer to the configuration of the job. Contains structures with user and primitive
 * relevant information. Parameters(OUT): NA Return value: E_OK: Request successful E_NOT_OK: Request failed
 *               CRYPTO_E_BUSY: Request failed, Crypro Driver Object is busy
 *               CRYPTO_E_KEY_NOT_VALID: Request failed, the key is not valid
 *               CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *               CRYPTO_E_KEY_READ_FAIL: The service request failed,because key element extraction is not allowed
 *               CRYPTO_E_KEY_WRITE_FAIL: The service request failed because the writing access failed
 *               CRYPTO_E_KEY_NOT_AVAILABLE: The service request failed because the key is not available
 *               CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *               CRYPTO_E_JOB_CANCELED: The service request failed because the synchronous Job has been canceled
 *               CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */
FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_ProcessJob(VAR(uint32, CRYIF_APPL_DATA) channelId, P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job);

/**
 * This interface dispatches the job cancellation function to the configured crypto driver object.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @channelId, Holds the identifier of the crypto channel.
 * Parameters(INOUT): @job, Pointer to the configuration of the job. Contains structures with
 user and primitive relevant information.
 * Parameters(OUT): NA
 * Return value: E_OK: Request successful, job has been removed
 *               E_NOT_OK: Request failed, job couldn't be removed
 */
FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_CancelJob(VAR(uint32, CRYIF_APPL_DATA) channelId, P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job);

/**
 * This function shall dispatch the set key element function to the configured crypto driver object.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be set.
 *          @keyElementId, Holds the identifier of the key element which shall be set.
 *          @keyPtr, Holds the pointer to the key data which shall be set as key element.
 *          @keyLength Contains the length of the key element in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_WRITE_FAIL:Request failed because write access was denied
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed because the key is not available
 *          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element size does not match size of provided data
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyElementSet(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementId,
    P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) keyPtr,
    VAR(uint32, CRYIF_APPL_DATA) keyLength);

/**
 * This function shall dispatch the set key valid function to the configured crypto driver object.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @cryIfKeyId, Holds the identifier of the key whose key elements shall be set to valid.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeySetValid(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId);

/**
 * This function shall dispatch the get key element function to the configured crypto driver object.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be returned.
 *          @keyElementId, Holds the identifier of the key element which shall be returned.
 * Parameters(INOUT): @resultLengthPtr, Holds a pointer to a memory location in which the length
 *                  information is stored. On calling this function this parameter shall
 *                  contain the size of the buffer provided by resultPtr. If the key
 *                  element is configured to allow partial access, this parameter
 *                  contains the amount of data which should be read from the key
 *                  element. The size may not be equal to the size of the provided
 *                  buffer anymore. When the request has finished, the amount of
 *                  data that has been stored shall be stored.
 * Parameters(OUT): @resultPtr,Holds the pointer of the buffer for the returned key element.
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed, the requested key element is not available
 *          CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *          CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyElementGet(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementId,
    P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) resultPtr,
    P2VAR(VAR(uint32, CRYIF_APPL_DATA), AUTOMATIC, CRYIF_APPL_DATA) resultLengthPtr);

/**
 * This function shall copy a key elements from one key to a target key.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same cryIfKeyId
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be the source element.
 *          @keyElementId, Holds the identifier of the key element which shall be the source for the copy operation.
 *          @targetCryIfKeyId, Holds the identifier of the key whose key element shall be the destination element.
 *          @targetKeyElementId, Holds the identifier of the key element which shall be the destination for the copy
 * operation. Parameters(INOUT):NA Parameters(OUT): NA Return value: E_OK: Request successful E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed, the requested key element is not available
 *          CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *          CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element
 *          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyElementCopy(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementId,
    VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) targetKeyElementId);

/**
 * Copies a key element to another key element. The keyElementOffsets and
 * keyElementCopyLength allows to copy just parts of the source key element into the destination key element.
 * Service ID: 0x12
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same cryIfKeyId
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be the source element.
 *          @keyElementId, Holds the identifier of the key element which shall be the source for the copy operation.
 *          @keyElementSourceOffset, This is the offset of the source key element indicating the start index of the copy
 * operation.
 *          @keyElementTargetOffset, This is the offset of the target key element indicating the start index of the copy
 * operation.
 *          @keyElementCopyLength, Specifies the number of bytes that shall be copied.
 *          @targetCryIfKeyId, Holds the identifier of the key whose key element shall be the destination element.
 *          @targetKeyElementId, Holds the identifier of the key element which shall be the destination for the copy
 * operation. Parameters(INOUT):NA Parameters(OUT): NA Return value: E_OK: Request successful E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed, the requested key element is not available
 *          CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *          CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element
 *          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyElementCopyPartial(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementSourceOffset,
    VAR(uint32, CRYIF_APPL_DATA) keyElementTargetOffset,
    VAR(uint32, CRYIF_APPL_DATA) keyElementCopyLength,
    VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) targetKeyElementId);

/**
 * This function shall copy all key elements from the source key to a target key.
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same cryIfKeyId
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be the source element.
 *          @targetCryIfKeyId, Holds the identifier of the key whose key element shall be the destination element.
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed, the requested key element is not available
 *          CRYPTO_E_KEY_READ_FAIL: Request failed, not allowed to extract key element
 *          CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element
 *          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyCopy(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId, VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId);

/**
 * This function shall dispatch the random seed function to the configured cryptodriver object.
 * Service ID: 0x07
 * Sync/Async: Sync or Async, depends on the configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key for which a new seed shall be generated.
 *          @seedPtr, Holds a pointer to the memory location which contains the data to feed the seed.
 *          @seedLength, Contains the length of the seed in bytes.
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_RandomSeed(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) seedPtr,
    VAR(uint32, CRYIF_APPL_DATA) seedLength);

/**
 * This function shall dispatch the key generate function to the configured crypto driver object.
 * Service ID: 0x08
 * Sync/Async: Sync or Async, depends on the configuration
 * Reentrancy: Reentrant
 * Parameters(IN):  @cryIfKeyId, Holds the identifier of the key which is to be updated with the generated value.
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyGenerate(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId);

/**
 * This function shall dispatch the key derive function to the configured crypto driver object.
 * Service ID: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key which is used for key derivation.
 *          @targetCryIfKeyId, Holds the identifier of the key which is used to store the derived key.
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyDerive(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId, VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId);

/**
 * This function shall dispatch the key exchange public value calculation function to the configured crypto driver
 * object. Service ID: 0x0a Sync/Async: Synchronous Reentrancy: Reentrant Parameters(IN): @cryIfKeyId, Holds the
 * identifier of the key which shall be used for the key exchange protocol. Parameters(INOUT): @publicValueLengthPtr,
 * Holds a pointer to the memory location in which the public value length information is stored. On calling this
 * function, this parameter shall contain the size of the buffer provided by publicValuePtr. When the request has
 * finished, the actual length of the returned value shall be stored. Parameters(OUT): @publicValuePtr, Contains the
 * pointer to the data where the public value shall be stored. Return value: E_OK: Request successful E_NOT_OK: Request
 * failed CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy CRYPTO_E_SMALL_BUFFER: The provided buffer is too
 * small to store the result CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyExchangeCalcPubVal(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) publicValueLengthPtr);

/**
 * This function shall dispatch the key exchange common shared secret calculation function to the configured crypto
 * driver object. Service ID: 0x0b Sync/Async: Synchronous Reentrancy: Reentrant Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key which shall be used for the key exchange protocol.
 *          @partnerPublicValuePtr, Holds the pointer to the memory location which contains the partner's public value.
 *          @partnerPublicValueLength, Contains the length of the partner's public value in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyExchangeCalcSecret(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, CRYIF_APPL_DATA) partnerPublicValueLength);

#endif /* CRYIF_H */
