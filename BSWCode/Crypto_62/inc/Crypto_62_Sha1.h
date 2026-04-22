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
 **  @file               : Crypto_62_Sha1.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_SHA1_H
#define CRYPTO_SHA1_H

#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/** @brief SHA1 Context*/
typedef struct
{
    uint32 total[2];   /*!< The number of Bytes processed.  */
    uint32 state[5];   /*!< The intermediate digest state.  */
    uint8  buffer[64]; /*!< The data block being processed. */
} Crypto_Sha1_Context;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Initializes the SHA-1 context.
 *
 * This function initializes the SHA-1 context structure, preparing it for a new SHA-1 hash computation.
 *
 * @param[out] ctx  Pointer to the SHA-1 context structure to be initialized.
 */
void Crypto_Sha1_Starts(Crypto_Sha1_Context* ctx);

/**
 * @brief   Processes a single 512-bit block of data.
 *
 * This function processes a single 512-bit block of data using the SHA-1 algorithm. It is intended for internal use.
 *
 * @param[in,out] ctx   Pointer to the SHA-1 context structure.
 * @param[in]     data  Pointer to the 512-bit block of data to be processed.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Internal_Sha1_Process(Crypto_Sha1_Context* ctx, const unsigned char data[64]);

/**
 * @brief   Updates the SHA-1 context with a new block of data.
 *
 * This function updates the SHA-1 context with a new block of data. It can be called multiple times to process
 * large amounts of data in chunks.
 *
 * @param[in,out] ctx   Pointer to the SHA-1 context structure.
 * @param[in]     input Pointer to the input data to be processed.
 * @param[in]     ilen  Length of the input data in bytes.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha1_Update(Crypto_Sha1_Context* ctx, const unsigned char* input, uint32 ilen);

/**
 * @brief   Finalizes the SHA-1 hash computation.
 *
 * This function finalizes the SHA-1 hash computation and writes the resulting hash to the output buffer.
 *
 * @param[in,out] ctx    Pointer to the SHA-1 context structure.
 * @param[out]    output Pointer to the buffer where the resulting hash will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha1_Finish(Crypto_Sha1_Context* ctx, unsigned char output[20]);

/**
 * @brief   Computes the SHA-1 hash of a given input data.
 *
 * This function computes the SHA-1 hash of a given input data and writes the resulting hash to the output buffer.
 *
 * @param[in]  input  Pointer to the input data to be hashed.
 * @param[in]  ilen   Length of the input data in bytes.
 * @param[out] output Pointer to the buffer where the resulting hash will be stored.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha1(const unsigned char* input, uint32 ilen, unsigned char output[20]);

/**
 * @brief   Processes the SHA-1 hash computation for a given object ID.
 *
 * This function processes the SHA-1 hash computation for a given object ID. It is intended for internal use.
 *
 * @param[in] objectId  The object ID for the SHA-1 hash computation.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Sha1_Process(uint32 objectId);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_SHA1 == STD_ON */

#endif /* CRYPTO_SHA1_H */
