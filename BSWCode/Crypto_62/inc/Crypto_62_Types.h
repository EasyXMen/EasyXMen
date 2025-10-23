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
 **  @file               : Crypto_62_Types.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_TYPES_H
#define CRYPTO_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Cfg.h"
#include "Crypto_GeneralTypes.h"
#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
#include "NvM.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/**
 * @brief current driver status
 */
typedef enum
{
    CRYPTO_DRIVER_UNINIT = 0,
    CRYPTO_DRIVER_IDLE   = 1,
} Crypto_62_DriverStatusType;

/**
 * @brief crypto key element type
 */
typedef enum
{
    CRYPTO_OBJECT_IDLE = 0,
    CRYPTO_OBJECT_BUSY = 1,
} Crypto_62_ObjectStatusType;
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
/*crypto key element format*/
/**
 * @brief crypto key element format
 */
typedef enum
{
    CRYPTO_KE_FORMAT_BIN_OCTET                  = 1,
    CRYPTO_KE_FORMAT_BIN_SHEKEYS                = 2,
    CRYPTO_KE_FORMAT_BIN_IDENT_PRIVATEKEY_PKCS8 = 3,
    CRYPTO_KE_FORMAT_BIN_IDENT_PUBLICKEY        = 4,
    CRYPTO_KE_FORMAT_BIN_RSA_PRIVATEKEY         = 5,
    CRYPTO_KE_FORMAT_BIN_RSA_PUBLICKEY          = 6,
    CRYPTO_KE_FORMAT_BIN_CERT_X509_V3           = 7,
    CRYPTO_KE_FORMAT_BIN_CERT_CVC               = 8,
} CryptoKeyElementFormat;

/**
 * @brief crypto key element access type
 */
typedef enum
{
    /* key element can be read as plaintext */
    CRYPTO_RA_ALLOWED = 0,
    /* key element can be read encrypted. E.g. SHE Ram-Key export.*/
    CRYPTO_RA_ENCRYPTED = 1,
    /* key element can be copied to another key element in the same crypto driver.*/
    CRYPTO_RA_INTERNAL_COPY = 2,
    /* key element cannot be read from outside the Crypto */
    CRYPTO_RA_DENIED = 3
} Crypto_62_KeyElemReadAccessType;

/**
 * @brief key element write access type
 */
typedef enum
{
    /* key element can be written as plaintext */
    CRYPTO_WA_ALLOWED = 0,
    /* key element can be written encrypted. E.g. SHE load key.*/
    CRYPTO_WA_ENCRYPTED = 1,
    /* key element can be filled with another key element in the same crypto driver.*/
    CRYPTO_WA_INTERNAL_COPY = 2,
    /* key element cannot be written from outside the crypto driver */
    CRYPTO_WA_DENIED = 3
} Crypto_62_KeyElemWriteAccessType;

#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
/**
 * @brief  nvm process type
 */
typedef enum
{
    CRYPTO_NV_BLOCK_DEFERRED  = 1,
    CRYPTO_NV_BLOCK_IMMEDIATE = 2,
} Crypto_62_NvBlockProcessingType;
#endif
/**
 * @brief key element config type
 */
typedef struct Crypto_62_KeyElementCfgTag
{
    /*  Enable or disable writing and reading the key element with data smaller than the size of the element */
    boolean CryptoKeyElementAllowPartialAccess;
    /*  Defines the format for the key element  */
    CryptoKeyElementFormat CryptoKeyElementFormat;
    /*  Holds the Identifier of the CRYPTO Key element */
    uint32 CryptoKeyElementId;
    /*  Holds the Value which will be used to fill the key element during startup */
    uint8* CryptoKeyElementInitValue;
    /*  Enable or disable persisting of the key element in non-volatile storage  */
    boolean CryptoKeyElementPersist;
    /*  Define the reading access rights of the key element.  */
    Crypto_62_KeyElemReadAccessType CryptoKeyElementReadAccess;
    /*  Holds the Maximum Size size of a CRYPTO key element in bytes */
    uint32 CryptoKeyElementSize;
    /*  Define the writing access rights of the key element  */
    Crypto_62_KeyElemWriteAccessType CryptoKeyElementWriteAccess;
    /*  Define the actual Size size of a CRYPTO key element in bytes  */
    uint32* CryptoKeyElementLength;
} Crypto_62_KeyElementCfgType;

/**
 *@brief  Structure of a key type.
 */
typedef struct Crypto_62_KeyTypeCfgTag
{
    /* the Number of KeyElement  */
    uint8 KeyElementNum;
    /*  Refers to a pointer in the CRYPTOCrypto Key Element  */
    const Crypto_62_KeyElementCfgType* CryptoKeyElementRef;
} Crypto_62_KeyTypeCfgType;

#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
/**
 * @brief Structure of a non-volatile block.
 */
typedef struct Crypto_62_NvStorageCfgTag
{
    uint16                          Crypto_62_NvmBlockId;
    uint16                          CryptoNvBlockFailedRetries;
    Crypto_62_NvBlockProcessingType CryptoNvBlockProcessing;
} Crypto_62_NvStorageCfgType;
#endif
/**
 * @brief Structure of a CRYPTO Key.
 */
typedef struct Crypto_62_KeyCfgTag
{
    /*  Holds the Identifier of the CRYPTO Key  */
    uint32 CryptoKeyId;
#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
    /*  Holds the nvblock which will be used to fill the key */
    const Crypto_62_NvStorageCfgType* CryptoKeyNvBlockRef;
#endif
    /*  Refers to a pointer in the CRYPTOCrypto Key Element  */
    const Crypto_62_KeyTypeCfgType* CryptoKeyTypeRef;
} Crypto_62_KeyCfgType;

#endif /* CRYPTO_MAXKEY_CONFIGURED > 0 */

/**
 *@brief  structure of a driver object
 */
typedef struct Crypto_62_DriverObjectCfgTag
{
    uint32 CryptoDriverObjectId;
    uint32 CryptoQueueSize;
/* This is a reference to the CryptoKey that is used by the CryptoDefaultRandomPrimitiveRef */
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
    const Crypto_62_KeyCfgType* CryptoDefaultRandomKeyRef;
#endif
    /* This is a reference to a primitive that configures a default random number generator */
    uint8 CryptoDefaultRandomPrimitiveRef;
    /* Maps a crypto driver object to zero or multiple ECUC partitions.  */
    uint8 CryptoDriverObjectEcucPartitionRef;
    uint8 CryptoPrimitiveRef;
} Crypto_62_DriverObjectCfgType;

/**
 * @brief structure of a primitive
 */
typedef struct Crypto_62_PrimitiveCfgTag
{
    Crypto_ServiceInfoType     CryptoPrimitiveService;
    Crypto_AlgorithmFamilyType CryptoPrimitiveAlgorithmFamily;
    Crypto_AlgorithmModeType   CryptoPrimitiveAlgorithmMode;
    boolean                    CryptoPrimitiveSupportContext;
    Crypto_AlgorithmFamilyType CryptoPrimitiveAlgorithmSecondaryFamily;
} Crypto_62_PrimitiveCfgType;

/* structure of nvstorage */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_TYPES_H */
