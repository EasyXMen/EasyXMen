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
**  FILENAME    : Crypto_Types.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuhao.ge                                                    **
**  Vendor      :                                                             **
**  DESCRIPTION : Type header for CSM                                         **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef CRYPTO_TYPES_H
#define CRYPTO_TYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Std_Types.h"
#include "Crypto_GeneralTypes.h"
#include "Crypto_ISoft_Cfg.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

typedef enum
{
    /**@details Indicate that crypto driver have not been yet initialized */
    CRYPTO_DRIVER_UNINIT = 0x00,
    /**@details Indicates that crypto driver is initialized, and no job registered.*/
    CRYPTO_DRIVER_IDLE = 0x01,
    /**@details Indicate there is a request ongoing in HSM */
    CRYPTO_DRIVER_BUSY = 0x02,
} Crypto_DriverStatusType;

#if (CRYPTO_MAXKEY_CONFIGURED > 0)

typedef enum
{
    CRYPTO_KE_FORMAT_BIN_OCTET = 0x01,
    CRYPTO_KE_FORMAT_BIN_SHEKEYS = 0x02,
    CRYPTO_KE_FORMAT_BIN_IDENT_PRIVATEKEY_PKCS8 = 0x03,
    CRYPTO_KE_FORMAT_BIN_IDENT_PUBLICKEY = 0x04,
    CRYPTO_KE_FORMAT_BIN_RSA_PRIVATEKEY = 0x05,
    CRYPTO_KE_FORMAT_BIN_RSA_PUBLICKEY = 0x06,
    CRYPTO_KE_FORMAT_BIN_CERT_X509_V3 = 0x07,
    CRYPTO_KE_FORMAT_BIN_CERT_CVC = 0x08,
} CryptoKeyElementFormat;

/**
 * @brief   Enumeration that defines the reading access rights of the key element.
 */
typedef enum
{

    /**@details key element can be read as plaintext */
    CRYPTO_RA_ALLOWED = 0x00u,
    /**@details key element can be read encrypted. E.g. SHE Ram-Key export.*/
    CRYPTO_RA_ENCRYPTED = 0x01u,
    /**@details key element cannot be read from outside the Crypto */
    CRYPTO_RA_DENIED = 0x02,
    /**@details key element can be copied to another key element in the same crypto driver.*/
    CRYPTO_RA_INTERNAL_COPY = 0x03
} Crypto_KeyElemReadAccessType;

/**
 * @brief   Enumeration that defines the writing access rights of the key element.
 */
typedef enum
{

    /**@details key element can be written as plaintext */
    CRYPTO_WA_ALLOWED = 0x00,
    /**@details key element can be written encrypted. E.g. SHE load key.*/
    CRYPTO_WA_ENCRYPTED = 0x01,
    /**@details key element can be filled with another key element in the same crypto driver.*/
    CRYPTO_WA_INTERNAL_COPY = 0x02,
    /**@details key element cannot be written from outside the crypto driver */
    CRYPTO_WA_DENIED = 0x03
} Crypto_KeyElemWriteAccessType;

/**
 * @brief   Structure of a key element.
 */

typedef struct
{
    /** @brief   Enable or disable writing and reading the key element with data smaller than the size of the element */
    VAR(boolean, CRYPTO_VAR) CryptoKeyElementAllowPartialAccess;
    /** @brief   Defines the format for the key element  */
    VAR(CryptoKeyElementFormat, CRYPTO_VAR) CryptoKeyElementFormat;
    /** @brief   Holds the Identifier of the CRYPTO Key element */
    VAR(uint32, CRYPTO_VAR) CryptoKeyElementId;
    /** @brief   Holds the Value which will be used to fill the key element during startup */
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) CryptoKeyElementInitValue;
    /** @brief   Enable or disable persisting of the key element in non-volatile storage  */
    VAR(boolean, CRYPTO_VAR) CryptoKeyElementPersist;
    /** @brief   Define the reading access rights of the key element.  */
    VAR(Crypto_KeyElemReadAccessType, CRYPTO_VAR) CryptoKeyElementReadAccess;
    /** @brief   Holds the Maximum Size size of a CRYPTO key element in bytes */
    VAR(uint32, CRYPTO_VAR) CryptoKeyElementSize;
    /** @brief   Define the writing access rights of the key element  */
    VAR(Crypto_KeyElemWriteAccessType, CRYPTO_VAR) CryptoKeyElementWriteAccess;
    /** @brief   Define the writing access rights of the key element  */
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) CryptoKeyElementLength;
} Crypto_KeyElementCfgType;

/**
 * @brief   Structure of a key type.
 */
typedef struct
{
    /** @brief  the Number of KeyElement  */
    VAR(uint8, CRYPTO_VAR) KeyElementNum;
    /** @brief   Refers to a pointer in the CRYPTOCrypto Key Element  */
    P2CONST(Crypto_KeyElementCfgType, AUTOMATIC, CRYPTO_APPL_DATA) CryptoKeyElementRef;
} Crypto_KeyTypeCfgType;

typedef struct
{
    /** @brief   Holds the Identifier of the CRYPTO Key  */
    VAR(uint32, CRYPTO_VAR) CryptoKeyId;
    /** @brief   Refers to a pointer in the CRYPTOCrypto Key Element  */
    P2CONST(Crypto_KeyTypeCfgType, AUTOMATIC, CRYPTO_APPL_DATA) CryptoKeyTypeRef;
} Crypto_KeyCfgType;

#endif /* CRYPTO_MAXKEY_CONFIGURED > 0 */
/* Refers to primitive in the CRYPTO.
The CryptoPrimitive is a pre-configured container of the crypto service that
shall be used. */
typedef struct
{
    /* Identifier of the Crypto Driver Object. The Crypto Driver Object offers
different crypto primitives.  */
    uint32 CryptoDriverObjectId;
    /* Size of the queue in the Crypto Driver. Defines the maximum number of
jobs in the Crypto Driver Object queue. If it is set to 0, queueing is disabled
in the Crypto Driver Object. */
    uint32 CryptoQueueSize;
    /* This is a reference to the CryptoKey that is used by the
CryptoDefaultRandomPrimitiveRef. The key contains key elements that
are necessary to seed the random number generator.
This element shall only be set if the primitive referenced by
CryptoDefaultRandomPrimitiveRef requires a seed value. */
    uint8 CryptoDefaultRandomKeyRef;
    /* This is a reference to a primitive that configures a default random number
generator. If a crypto driver object needs to perform a crypto primitive that
requires a random number generator, but the configuration of this primitive
does not provide parameter for a random number generator, then this default
random number generator shall be used (i.e. the elements of algorithm family,
secondary algorithm family and algorithm mode do not provide this information).
Example: The crypto driver shall generate a signature based on elliptic
curve but the primitive for signature generation lacks information about a
random number generator. */
    uint8 CryptoDefaultRandomPrimitiveRef;
    /* Maps a crypto driver object to zero or multiple ECUC partitions. The ECUC
partition referenced is a subset of the ECUC partitions where the Crypto
driver is mapped to.
Note: CryptoDriverObjects such as a HSM shall be mapped to one
partition only. */
    uint8 CryptoDriverObjectEcucPartitionRef;
    /* Refers to primitive in the CRYPTO.
The CryptoPrimitive is a pre-configured container of the crypto service that
shall be used. */
    uint8 CryptoPrimitiveRef;
} Crypto_DriverObjectCfgType;

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#endif /* CSM_TYPES_H */
