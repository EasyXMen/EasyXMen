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
**  FILENAME    : dh.h                                                        **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for dh                                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef DH_H
#define DH_H
#if (CRYPTO_ALGORITHM_DH == STD_ON)
#include "Crypto_Types.h"
#include "ctr_drbg.h"
#include "rsa.h"

typedef struct
{
    uint32 plen; /*!<  The size of \p N in Bytes. */
    uint32 glen; /*!<  The size of \p N in Bytes. */

    mbedtls_mpi p; /*!<  The public modulus. */
    mbedtls_mpi g; /*!<  The public exponent. */
    mbedtls_mpi a; /*!<  The public exponent. */
    mbedtls_mpi K; /*!<  The public exponent. */

    mbedtls_mpi A; /*!<  The private exponent. */
} dh_context;

// Regular implementation
//
typedef enum
{
    MPI_GEN_PRIME_FLAG_DH = 0x0001,      /**< (X-1)/2 is prime too */
    MPI_GEN_PRIME_FLAG_LOW_ERR = 0x0002, /**< lower error rate from 2<sup>-80</sup> to 2<sup>-128</sup> */
} mpi_gen_prime_flag_t;

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_DHPublic_KeyElement(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) publicValueLengthPtr);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_DHPublic_Generate(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) publicValueLengthPtr);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_DHSecret(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, AUTOMATIC) partnerPublicValueLength);
#endif /* CRYPTO_ALGORITHM_DH == STD_ON */
#endif /* dh.h */
