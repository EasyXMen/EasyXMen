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
**  FILENAME    : dh.c                                                        **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for dh                                       **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#include "dh.h"
#include "Crypto_internal.h"
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"
#if (CRYPTO_ALGORITHM_DH == STD_ON)
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_DHPublic_KeyElement(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) publicValueLengthPtr)
{

    Std_ReturnType ret = E_NOT_OK;
    dh_context ctx;
    mbedtls_mpi p, g, A, RR;
    Crypto_CtrDrbgData ctr_drbg;
    Crypto_entropy_context entropy;
    uint8 keyp[128];
    uint8 keyg[128];
    uint8 keyA[128];
    uint32 length = 0;

    mbedtls_mpi_init(&A);
    mbedtls_mpi_init(&p);
    mbedtls_mpi_init(&g);
    mbedtls_mpi_init(&RR);
    Crypto_memset(&ctx, sizeof(dh_context));
    Crypto_memset(&ctr_drbg, sizeof(Crypto_CtrDrbgData));
    Crypto_memset(&entropy, sizeof(Crypto_entropy_context));
    Crypto_memset(keyp, 128);
    Crypto_memset(keyg, 3);
    Crypto_memset(keyA, 128);

    /*store a in key element*/
    ret = Crypto_KeyElementSetInternal(cryptoKeyId, 9, publicValuePtr, *publicValueLengthPtr);
    if (E_OK == ret)
    {
        mbedtls_mpi_read_binary(&ctx.a, publicValuePtr, *publicValueLengthPtr);

        /*get p and g*/
        ret = Get_Key(cryptoKeyId, keyp, 8);
        mbedtls_mpi_read_binary(&ctx.p, keyp, 128);
        ret = Get_Key(cryptoKeyId, keyg, 12);
        mbedtls_mpi_read_binary(&ctx.g, keyg, 128);

        /*
         * A = p^a mod g
         */
        ret = mbedtls_mpi_exp_mod(&A, &ctx.p, &ctx.a, &ctx.g, &RR);
        if (E_OK == ret)
        {
            mbedtls_mpi_write_binary(&A, keyA, 128);
        }
        publicValuePtr = keyA;
        length = 128;
        publicValueLengthPtr = &length;
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_DHPublic_Generate(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) publicValueLengthPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    dh_context ctx;
    mbedtls_mpi p, g, A, RR;
    Crypto_CtrDrbgData ctr_drbg;
    Crypto_entropy_context entropy;
    const uint8 pers[16] = {0x00};
    uint8 keyp[128];
    uint8 keyg[128];
    uint8 keyA[128];
    uint32 length = 0;
    uint16 KeySize = 1024;
    int prime_quality = MPI_GEN_PRIME_FLAG_LOW_ERR; /* PRQA S 5209  */ /* MISRA Dir 4.6 */

    mbedtls_mpi_init(&A);
    mbedtls_mpi_init(&p);
    mbedtls_mpi_init(&g);
    mbedtls_mpi_init(&RR);
    Crypto_memset(&ctx, sizeof(dh_context));
    Crypto_memset(&ctr_drbg, sizeof(Crypto_CtrDrbgData));
    Crypto_memset(&entropy, sizeof(Crypto_entropy_context));
    Crypto_memset(keyp, 128);
    Crypto_memset(keyg, 128);
    Crypto_memset(keyA, 128);

    /*store a in key element*/
    ret = Crypto_KeyElementSetInternal(cryptoKeyId, 9, publicValuePtr, *publicValueLengthPtr);
    if (E_OK == ret)
    {
        /*general p and g*/
        ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, pers, 16);
        mbedtls_mpi_read_binary(&ctx.a, publicValuePtr, *publicValueLengthPtr);
        if (E_OK == ret)
        {
            ret = mbedtls_mpi_gen_prime(&ctx.p, KeySize >> 1, prime_quality, mbedtls_ctr_drbg_random, &ctr_drbg);
            ret = mbedtls_mpi_gen_prime(&ctx.g, KeySize >> 1, prime_quality, mbedtls_ctr_drbg_random, &ctr_drbg);
        }
        /*
         * A = p^a mod g
         */
        ret = mbedtls_mpi_exp_mod(&A, &ctx.p, &ctx.a, &ctx.g, &RR);
        if (E_OK == ret)
        {
            mbedtls_mpi_write_binary(&A, keyA, 128);
        }
        publicValuePtr = keyA;
        length = 128;
        publicValueLengthPtr = &length;
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_DHSecret(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, AUTOMATIC) partnerPublicValueLength)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 keyp[128];
    uint8 keya[128];
    uint8 keyK[128];
    dh_context ctx;
    mbedtls_mpi B, RR;

    Crypto_memset(keyp, 128);
    Crypto_memset(keya, 128);
    Crypto_memset(keyK, 128);
    Crypto_memset(&ctx, sizeof(dh_context));
    mbedtls_mpi_init(&B);
    mbedtls_mpi_init(&RR);

    /*get p and a*/
    ret = Get_Key(cryptoKeyId, keyp, 8);
    mbedtls_mpi_read_binary(&ctx.p, keyp, 128);
    ret = Get_Key(cryptoKeyId, keya, 9);
    mbedtls_mpi_read_binary(&ctx.a, keya, 128);
    mbedtls_mpi_read_binary(&B, partnerPublicValuePtr, partnerPublicValueLength);

    /*
     * K = B^a mod p
     */
    ret = mbedtls_mpi_exp_mod(&ctx.K, &B, &ctx.a, &ctx.p, &RR);
    if (E_OK == ret)
    {
        mbedtls_mpi_write_binary(&ctx.K, keyK, 128);
    }
    ret = Crypto_KeyElementSetInternal(cryptoKeyId, 1, keyK, 128);

    return ret;
}
#endif /* CRYPTO_ALGORITHM_DH == STD_ON */
#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"
