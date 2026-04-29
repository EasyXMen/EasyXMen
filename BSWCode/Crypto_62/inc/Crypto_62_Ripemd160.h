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
 **  @file               : Crypto_62_Ripemd160.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_RIPEMD160_H
#define CRYPTO_RIPEMD160_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define S(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
/* PRQA S 3410 ++ */ /* VL_Crypto_62_General */
#define P(a, b, c, d, e, r, s, f, k)                \
    do                                              \
    {                                               \
        (a) += f((b), (c), (d)) + local.X[r] + (k); \
        (a) = S((a), (s)) + (e);                    \
        (c) = S((c), 10);                           \
    } while (0)
/* PRQA S 3410 -- */
#define ROUND2_STEP(a, b, c, d, e, r, s, rp, sp)             \
    do                                                       \
    {                                                        \
        P((a), (b), (c), (d), (e), (r), (s), F, K);          \
        P(a##p, b##p, c##p, d##p, e##p, (rp), (sp), Fp, Kp); \
    } while (0)

#define F1(x, y, z) ((x) ^ (y) ^ (z))
#define F2(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define F3(x, y, z) (((x) | ~(y)) ^ (z))
#define F4(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define F5(x, y, z) ((x) ^ ((y) | ~(z)))
/* ================================================ type definitions ================================================ */
/** @brief RIPEMD-160 Context*/
typedef struct
{
    uint32 total[CRYPTO_CONST_2];   /*!< number of bytes processed  */
    uint32 state[CRYPTO_CONST_5];   /*!< intermediate digest state  */
    uint8  buffer[CRYPTO_CONST_64]; /*!< data block being processed */
} Crypto_Ripemd160_Context;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Initializes the RIPEMD-160 context for a new hash operation.
 *
 * @param[in,out] ctx  Pointer to the RIPEMD-160 context structure.
 *
 * @return  None.
 */
void Crypto_Ripemd160_Starts(Crypto_Ripemd160_Context* ctx);

/**
 * @brief   Updates the RIPEMD-160 context with new input data.
 *
 * @param[in,out] ctx    Pointer to the RIPEMD-160 context structure.
 * @param[in]     input  Pointer to the input data.
 * @param[in]     ilen   Length of the input data in bytes.
 *
 * @return  None.
 */
void Crypto_Ripemd160_Update(Crypto_Ripemd160_Context* ctx, const uint8* input, uint32 ilen);

/**
 * @brief   Finalizes the RIPEMD-160 context and produces the hash output.
 *
 * @param[in,out] ctx    Pointer to the RIPEMD-160 context structure.
 * @param[out]    output Pointer to the buffer where the hash output will be stored (20 bytes).
 *
 * @return  None.
 */
void Crypto_Ripemd160_Finish(Crypto_Ripemd160_Context* ctx, uint8 output[CRYPTO_CONST_20]);

/**
 * @brief   Computes the RIPEMD-160 hash of the input data.
 *
 * @param[in]  input   Pointer to the input data.
 * @param[in]  ilen    Length of the input data in bytes.
 * @param[out] output  Pointer to the buffer where the hash output will be stored (20 bytes).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Ripemd160(const uint8* input, uint32 ilen, uint8 output[CRYPTO_CONST_20]);

/**
 * @brief   Processes the RIPEMD-160 hash operation for a given object ID.
 *
 * @param[in] objectId  The object ID for the RIPEMD-160 process.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Ripemd160_Process(uint32 objectId);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON */

#endif /* CRYPTO_RIPEMD160_H */
