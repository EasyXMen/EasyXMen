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
 **  @file               : Crypto_62__ChaCha.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_CHACHA_H
#define CRYPTO_CHACHA_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_CHACHA == STD_ON)

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/** @brief Chacha20 Context*/
typedef struct
{
    uint32 state[CRYPTO_CONST_16];      /*! The state (before round operations). */
    uint8  keystream8[CRYPTO_CONST_64]; /*! Leftover keystream bytes. */
    uint32 keystream_bytes_used;        /*! Number of keystream bytes already used. */
} Crypto_Chacha20_Context;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Initializes the ChaCha20 context.
 *
 * @param[in] ctx Pointer to the ChaCha20 context structure.
 *
 * @return  None.
 */
void Crypto_chacha20_init(Crypto_Chacha20_Context* ctx);

/**
 * @brief   Encrypts or decrypts data using the ChaCha20 algorithm.
 *
 * @param[in]  key      Pointer to the encryption/decryption key (32 bytes).
 * @param[in]  nonce    Pointer to the nonce (12 bytes).
 * @param[in]  counter  Initial counter value.
 * @param[in]  data_len Length of the input data.
 * @param[in]  input    Pointer to the input data.
 * @param[out] output   Pointer to the output data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_chacha20_crypt(
    const uint8  key[CRYPTO_CONST_32],
    const uint8  nonce[CRYPTO_CONST_12],
    uint32       counter,
    uint32       data_len,
    const uint8* input,
    uint8*       output);

/**
 * @brief   Updates the ChaCha20 context with new data.
 *
 * @param[in,out] ctx  Pointer to the ChaCha20 context structure.
 * @param[in]     size Length of the input data.
 * @param[in]     input Pointer to the input data.
 * @param[out]    output Pointer to the output data.
 *
 * @return  None.
 */
void Crypto_chacha20_update(Crypto_Chacha20_Context* ctx, uint32 size, const uint8* input, uint8* output);

/**
 * @brief   Starts the ChaCha20 encryption/decryption process.
 *
 * @param[in,out] ctx     Pointer to the ChaCha20 context structure.
 * @param[in]     nonce   Pointer to the nonce (12 bytes).
 * @param[in]     counter Initial counter value.
 *
 * @return  None.
 */
void Crypto_chacha20_starts(Crypto_Chacha20_Context* ctx, const uint8 nonce[CRYPTO_CONST_12], uint32 counter);

/**
 * @brief   Sets the encryption/decryption key for ChaCha20.
 *
 * @param[in,out] ctx Pointer to the ChaCha20 context structure.
 * @param[in]     key Pointer to the encryption/decryption key (32 bytes).
 *
 * @return  None.
 */
void Crypto_chacha20_setkey(Crypto_Chacha20_Context* ctx, const uint8 key[CRYPTO_CONST_32]);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_CHACHA == STD_ON */

#endif /* CRYPTO_CHACHA_H */
