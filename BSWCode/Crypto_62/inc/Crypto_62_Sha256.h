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
 **  @file               : Crypto_62_Sha256.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_SHA256_H
#define CRYPTO_SHA256_H

#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/** @brief SHA256 Context*/
typedef struct
{
    uint32 total[CRYPTO_CONST_2];   /*!< The number of Bytes processed.  */
    uint32 state[CRYPTO_CONST_8];   /*!< The intermediate digest state.  */
    uint8  buffer[CRYPTO_CONST_64]; /*!< The data block being processed. */
} Crypto_Sha256_Context;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Initializes the SHA-256 context.
 *
 * This function initializes the SHA-256 context structure, preparing it for a new SHA-256 or SHA-224 hash computation.
 *
 * @param[out] ctx    Pointer to the SHA-256 context structure to be initialized.
 * @param[in]  is224  Boolean flag indicating whether to compute SHA-224 (true) or SHA-256 (false).
 */
void Crypto_Sha256_Starts(Crypto_Sha256_Context* ctx, boolean is224);

/**
 * @brief   Updates the SHA-256 context with a new block of data.
 *
 * This function updates the SHA-256 context with a new block of data. It can be called multiple times to process
 * large amounts of data in chunks.
 *
 * @param[in,out] ctx   Pointer to the SHA-256 context structure.
 * @param[in]     input Pointer to the input data to be processed.
 * @param[in]     ilen  Length of the input data in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha256_Update(Crypto_Sha256_Context* ctx, const uint8* input, uint32 ilen);

/**
 * @brief   Finalizes the SHA-256 hash computation.
 *
 * This function finalizes the SHA-256 hash computation and writes the resulting hash to the output buffer.
 *
 * @param[in,out] ctx    Pointer to the SHA-256 context structure.
 * @param[out]    output Pointer to the buffer where the resulting hash will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha256_Finish(Crypto_Sha256_Context* ctx, uint8 output[CRYPTO_CONST_32]);

/**
 * @brief   Computes the SHA-256 or SHA-224 hash of a given input data.
 *
 * This function computes the SHA-256 or SHA-224 hash of a given input data and writes the resulting hash to the output
 * buffer.
 *
 * @param[in]  input  Pointer to the input data to be hashed.
 * @param[in]  ilen   Length of the input data in bytes.
 * @param[out] output Pointer to the buffer where the resulting hash will be stored.
 * @param[in]  is224  Boolean flag indicating whether to compute SHA-224 (true) or SHA-256 (false).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha256(const uint8* input, uint32 ilen, uint8* output, boolean is224);

/**
 * @brief   Processes the SHA-256 or SHA-224 hash computation for a given object ID.
 *
 * This function processes the SHA-256 or SHA-224 hash computation for a given object ID. It is intended for internal
 * use.
 *
 * @param[in] objectId  The object ID for the SHA-256 or SHA-224 hash computation.
 * @param[in] is224     Boolean flag indicating whether to compute SHA-224 (true) or SHA-256 (false).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha256_Process(uint32 objectId, boolean is224);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON */

#endif /* CRYPTO_SHA256_H */
