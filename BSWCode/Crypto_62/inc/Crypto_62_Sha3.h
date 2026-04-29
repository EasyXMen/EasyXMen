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
 **  @file               : Crypto_62_Sha3.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_SHA3_H
#define CRYPTO_SHA3_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)

#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

#define H(b63, b31, b15) (b63 << 6 | b31 << 5 | b15 << 4)
CRYPTO_62_LOCAL const uint8 iota_r_packed[24] = {
    H(0, 0, 0) | 0x01, H(0, 0, 1) | 0x82, H(1, 0, 1) | 0x8a, H(1, 1, 1) | 0x00, H(0, 0, 1) | 0x8b, H(0, 1, 0) | 0x01,
    H(1, 1, 1) | 0x81, H(1, 0, 1) | 0x09, H(0, 0, 0) | 0x8a, H(0, 0, 0) | 0x88, H(0, 1, 1) | 0x09, H(0, 1, 0) | 0x0a,
    H(0, 1, 1) | 0x8b, H(1, 0, 0) | 0x8b, H(1, 0, 1) | 0x89, H(1, 0, 1) | 0x03, H(1, 0, 1) | 0x02, H(1, 0, 0) | 0x80,
    H(0, 0, 1) | 0x0a, H(1, 1, 0) | 0x0a, H(1, 1, 1) | 0x81, H(1, 0, 1) | 0x80, H(0, 1, 0) | 0x01, H(1, 1, 1) | 0x08,
};
#undef H

#define BYTE_XOR 0x6

CRYPTO_62_LOCAL const uint32 pi[6]  = {0x110b070a, 0x10050312, 0x04181508, 0x0d13170f, 0x0e14020c, 0x01060916};
CRYPTO_62_LOCAL const uint32 rho[6] = {0x3f022425, 0x1c143a09, 0x2c3d3615, 0x27191713, 0x312b382e, 0x3e030832};

#define ABSORB(ctx, idx, v)                                              \
    do                                                                   \
    {                                                                    \
        ctx->state[(idx) >> 3] ^= ((uint64)(v)) << (((idx) & 0x7) << 3); \
    } while (0)

#define ROTR64(x, y) (((x) << (64U - (y))) | ((x) >> (y)))

#define SQUEEZE(ctx, idx) ((uint8)(ctx->state[(idx) >> 3] >> (((idx) & 0x7) << 3)))

#define SWAP(x, y)        \
    do                    \
    {                     \
        uint64 tmp = (x); \
        (x)        = (y); \
        (y)        = tmp; \
    } while (0)

/* ================================================ type definitions ================================================ */
/** @brief SHA3 Context*/
typedef struct
{
    uint64 state[25];
    uint32 index;
    uint16 olen;
    uint16 max_block_size;
} Crypto_Sha3_Context;
/** @brief  distiguish diffrent sha3 length*/
typedef enum
{
    CRYPTO_SHA3_NONE = 0, /*!< Operation not defined. */
    CRYPTO_SHA3_224,      /*!< SHA3-224 */
    CRYPTO_SHA3_256,      /*!< SHA3-256 */
    CRYPTO_SHA3_384,      /*!< SHA3-384 */
    CRYPTO_SHA3_512,      /*!< SHA3-512 */
} Crypto_Sha3_Id;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Initializes the SHA-3 context.
 *
 * This function initializes the SHA-3 context structure for a specified SHA-3 variant (e.g., SHA3-224, SHA3-256,
 * SHA3-384, SHA3-512).
 *
 * @param[out] ctx  Pointer to the SHA-3 context structure to be initialized.
 * @param[in]  id   The SHA-3 variant identifier (e.g., CRYPTO_SHA3_224, CRYPTO_SHA3_256, CRYPTO_SHA3_384,
 * CRYPTO_SHA3_512).
 */
void Crypto_Sha3_Starts(Crypto_Sha3_Context* ctx, Crypto_Sha3_Id id);

/**
 * @brief   Updates the SHA-3 context with a new block of data.
 *
 * This function updates the SHA-3 context with a new block of data. It can be called multiple times to process
 * large amounts of data in chunks.
 *
 * @param[in,out] ctx   Pointer to the SHA-3 context structure.
 * @param[in]     input Pointer to the input data to be processed.
 * @param[in]     ilen  Length of the input data in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha3_Update(Crypto_Sha3_Context* ctx, const uint8* input, uint32 ilen);

/**
 * @brief   Finalizes the SHA-3 hash computation.
 *
 * This function finalizes the SHA-3 hash computation and writes the resulting hash to the output buffer.
 *
 * @param[in,out] ctx    Pointer to the SHA-3 context structure.
 * @param[out]    output Pointer to the buffer where the resulting hash will be stored.
 * @param[in]     olen   Length of the output buffer in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha3_Finish(Crypto_Sha3_Context* ctx, uint8* output, uint32 olen);

/**
 * @brief   Computes the SHA-3 hash of a given input data.
 *
 * This function computes the SHA-3 hash of a given input data for a specified SHA-3 variant and writes the resulting
 * hash to the output buffer.
 *
 * @param[in]  id     The SHA-3 variant identifier (e.g., CRYPTO_SHA3_224, CRYPTO_SHA3_256, CRYPTO_SHA3_384,
 * CRYPTO_SHA3_512).
 * @param[in]  input  Pointer to the input data to be hashed.
 * @param[in]  ilen   Length of the input data in bytes.
 * @param[out] output Pointer to the buffer where the resulting hash will be stored.
 * @param[in]  olen   Length of the output buffer in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha3(Crypto_Sha3_Id id, const uint8* input, uint32 ilen, uint8* output, uint32* olen);

/**
 * @brief   Processes the SHA-3 hash computation for a given object ID.
 *
 * This function processes the SHA-3 hash computation for a given object ID and SHA-3 variant. It is intended for
 * internal use.
 *
 * @param[in] objectId  The object ID for the SHA-3 hash computation.
 * @param[in] Id        The SHA-3 variant identifier (e.g., CRYPTO_SHA3_224, CRYPTO_SHA3_256, CRYPTO_SHA3_384,
 * CRYPTO_SHA3_512).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha3_Process(uint32 objectId, Crypto_Sha3_Id Id);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_SHA3 == STD_ON */

#endif /* CRYPTO_SHA3_H */
