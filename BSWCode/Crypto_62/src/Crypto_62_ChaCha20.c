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
 **  @file               : Crypto_62_ChaCha20.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 0311,1252,1253,0488,3446,4549,4559,0553 EOF */ /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_CHACHA == STD_ON)
#include "Crypto_62_ChaCha.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ===================================================== macros ===================================================== */
/* PRQA S 3472 ++ */ /* VL_QAC_Crypto */
#define ROTL32(value, amount) ((uint32)((value) << (amount)) | ((value) >> (32 - (amount))))
/* PRQA S 3472 -- */
#define CHACHA20_CTR_INDEX (12U)

#define CHACHA20_BLOCK_SIZE_BYTES (4U * 16U)
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
CRYPTO_62_LOCAL void chacha20_inner_block(uint32 state[CRYPTO_CONST_16]);
CRYPTO_62_LOCAL void chacha20_block(const uint32 initial_state[CRYPTO_CONST_16], uint8 keystream[CRYPTO_CONST_64]);

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               ChaCha20 quarter round operation. The quarter round is
 *                     defined as follows (from RFC 7539): 1. a += b; d ^= a;
 *                     d <<<= 16; 2. c += d; b ^= c; b <<<= 12; 3. a += b; d ^= a;
 *                     d <<<= 8; 4. c += d; b ^= c; b <<<= 7;
 *
 * Param-Name[in]      a: The index of 'a' in the state.
 *                     b: The index of 'b' in the state.
 *                     c: The index of 'c' in the state.
 *                     d: The index of 'd' in the state.
 * Param-Name[in/out]  state: ChaCha20 state to modify.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL inline void
    chacha20_quarter_round(uint32 state[CRYPTO_CONST_16], uint32 a, uint32 b, uint32 c, uint32 d)
{
    /* PRQA S 1840 ++ */ /* VL_QAC_Crypto */
    /* a += b; d ^= a; d <<<= 16; */
    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = ROTL32(state[d], CRYPTO_CONST_16);

    /* c += d; b ^= c; b <<<= 12 */
    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = ROTL32(state[b], CRYPTO_CONST_12);

    /* a += b; d ^= a; d <<<= 8; */
    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = ROTL32(state[d], CRYPTO_CONST_8);

    /* c += d; b ^= c; b <<<= 7; */
    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = ROTL32(state[b], CRYPTO_CONST_7);
    /* PRQA S 1840 -- */
}

/******************************************************************************/
/*
 * Brief               Perform the ChaCha20 inner block operation.This function
 *                     performs two rounds: the column round and the diagonal round.
 *
 * Param-Name[in]      None
 * Param-Name[in/out]  state: The ChaCha20 state to update.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void chacha20_inner_block(uint32 state[CRYPTO_CONST_16])
{
    chacha20_quarter_round(state, CRYPTO_CONST_0, CRYPTO_CONST_4, CRYPTO_CONST_8, CRYPTO_CONST_12);
    chacha20_quarter_round(state, CRYPTO_CONST_1, CRYPTO_CONST_5, CRYPTO_CONST_9, CRYPTO_CONST_13);
    chacha20_quarter_round(state, CRYPTO_CONST_2, CRYPTO_CONST_6, CRYPTO_CONST_10, CRYPTO_CONST_14);
    chacha20_quarter_round(state, CRYPTO_CONST_3, CRYPTO_CONST_7, CRYPTO_CONST_11, CRYPTO_CONST_15);

    chacha20_quarter_round(state, CRYPTO_CONST_0, CRYPTO_CONST_5, CRYPTO_CONST_10, CRYPTO_CONST_15);
    chacha20_quarter_round(state, CRYPTO_CONST_1, CRYPTO_CONST_6, CRYPTO_CONST_11, CRYPTO_CONST_12);
    chacha20_quarter_round(state, CRYPTO_CONST_2, CRYPTO_CONST_7, CRYPTO_CONST_8, CRYPTO_CONST_13);
    chacha20_quarter_round(state, CRYPTO_CONST_3, CRYPTO_CONST_4, CRYPTO_CONST_9, CRYPTO_CONST_14);
}

/******************************************************************************/
/*
 * Brief               Generates a keystream block.
 *
 * Param-Name[in]      initial_state: The initial ChaCha20 state (key, nonce, counter).
 * Param-Name[in/out]  None
 * Param-Name[out]     keystream: Generated keystream bytes are written to this buffer.
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void chacha20_block(const uint32 initial_state[CRYPTO_CONST_16], uint8 keystream[CRYPTO_CONST_64])
{
    uint32 working_state[CRYPTO_CONST_16];
    uint32 i;

    (void)IStdLib_MemCpy(working_state, initial_state, CHACHA20_BLOCK_SIZE_BYTES);

    for (i = 0U; i < CRYPTO_CONST_10; i++)
    {
        chacha20_inner_block(working_state);
    }

    working_state[0] += initial_state[0];
    working_state[1] += initial_state[1];
    working_state[CRYPTO_CONST_2] += initial_state[CRYPTO_CONST_2];
    working_state[CRYPTO_CONST_3] += initial_state[CRYPTO_CONST_3];
    working_state[CRYPTO_CONST_4] += initial_state[CRYPTO_CONST_4];
    working_state[CRYPTO_CONST_5] += initial_state[CRYPTO_CONST_5];
    working_state[CRYPTO_CONST_6] += initial_state[CRYPTO_CONST_6];
    working_state[CRYPTO_CONST_7] += initial_state[CRYPTO_CONST_7];
    working_state[CRYPTO_CONST_8] += initial_state[CRYPTO_CONST_8];
    working_state[CRYPTO_CONST_9] += initial_state[CRYPTO_CONST_9];
    working_state[CRYPTO_CONST_10] += initial_state[CRYPTO_CONST_10];
    working_state[CRYPTO_CONST_11] += initial_state[CRYPTO_CONST_11];
    working_state[CRYPTO_CONST_12] += initial_state[CRYPTO_CONST_12];
    working_state[CRYPTO_CONST_13] += initial_state[CRYPTO_CONST_13];
    working_state[CRYPTO_CONST_14] += initial_state[CRYPTO_CONST_14];
    working_state[CRYPTO_CONST_15] += initial_state[CRYPTO_CONST_15];

    for (i = 0U; i < CRYPTO_CONST_16; i++)
    {
        uint32 offset = i * CRYPTO_CONST_4;
        /* PRQA S 2741,3138 ++ */ /* VL_QAC_Crypto */
        CRYPTO_PUT_UINT32_LE(working_state[i], keystream, offset);
        /* PRQA S 2741,3138 -- */
    }
}

/******************************************************************************/
/*
 * Brief               This function initializes the specified ChaCha20 context.
 *                     It must be the first API called before using the context.
 *
 * Param-Name[in]      None
 * Param-Name[in/out]  ctx: The ChaCha20 context to initialize. This must not be NULL.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_chacha20_init(Crypto_Chacha20_Context* ctx)
{
    /* Initially, there's no keystream bytes available */
    ctx->keystream_bytes_used = CHACHA20_BLOCK_SIZE_BYTES;
}

/******************************************************************************/
/*
 * Brief               This function sets the encryption/decryption key.
 *
 * Param-Name[in]      key: The encryption/decryption key. This must be 32 Bytes
 *                         in length.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The ChaCha20 context to which the key should be bound.
 *                          It must be initialized.
 * Return              None
 */
/******************************************************************************/
void Crypto_chacha20_setkey(Crypto_Chacha20_Context* ctx, const uint8 key[CRYPTO_CONST_32])
{
    /* ChaCha20 constants - the string "expand 32-byte k" */
    /* PRQA S 3120 ++ */ /* VL_QAC_Crypto */
    ctx->state[0]              = 0x61707865U;
    ctx->state[1]              = 0x3320646eU;
    ctx->state[CRYPTO_CONST_2] = 0x79622d32U;
    ctx->state[CRYPTO_CONST_3] = 0x6b206574U;
    /* PRQA S 3120 -- */

    /* Set key */
    /* PRQA S 3493 ++ */ /* VL_QAC_Crypto */
    ctx->state[CRYPTO_CONST_4]  = CRYPTO_GET_UINT32_LE(key, CRYPTO_CONST_0);
    ctx->state[CRYPTO_CONST_5]  = CRYPTO_GET_UINT32_LE(key, CRYPTO_CONST_4);
    ctx->state[CRYPTO_CONST_6]  = CRYPTO_GET_UINT32_LE(key, CRYPTO_CONST_8);
    ctx->state[CRYPTO_CONST_7]  = CRYPTO_GET_UINT32_LE(key, CRYPTO_CONST_12);
    ctx->state[CRYPTO_CONST_8]  = CRYPTO_GET_UINT32_LE(key, CRYPTO_CONST_16);
    ctx->state[CRYPTO_CONST_9]  = CRYPTO_GET_UINT32_LE(key, CRYPTO_CONST_20);
    ctx->state[CRYPTO_CONST_10] = CRYPTO_GET_UINT32_LE(key, CRYPTO_CONST_24);
    ctx->state[CRYPTO_CONST_11] = CRYPTO_GET_UINT32_LE(key, CRYPTO_CONST_28);
    /* PRQA S 3493 -- */
}

/******************************************************************************/
/*
 * Brief               This function sets the nonce and initial counter value.
 *
 * Param-Name[in]      nonce: The nonce. This must be 12 Bytes in size.
 *                     counter: The initial counter value. This is usually 0.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The ChaCha20 context to which the nonce should be bound.
 *                           It must be initialized and bound to a key.
 * Return              None
 */
/******************************************************************************/
void Crypto_chacha20_starts(Crypto_Chacha20_Context* ctx, const uint8 nonce[CRYPTO_CONST_12], uint32 counter)
{
    /* Counter */
    ctx->state[CRYPTO_CONST_12] = counter;

    /* Nonce */
    /* PRQA S 3493 ++ */ /* VL_QAC_Crypto */
    ctx->state[CRYPTO_CONST_13] = CRYPTO_GET_UINT32_LE(nonce, CRYPTO_CONST_0);
    ctx->state[CRYPTO_CONST_14] = CRYPTO_GET_UINT32_LE(nonce, CRYPTO_CONST_4);
    ctx->state[CRYPTO_CONST_15] = CRYPTO_GET_UINT32_LE(nonce, CRYPTO_CONST_8);
    /* PRQA S 3493 -- */

    /* Initially, there's no keystream bytes available */
    ctx->keystream_bytes_used = CHACHA20_BLOCK_SIZE_BYTES;
}

/******************************************************************************/
/*
 * Brief               This function encrypts or decrypts data. Since ChaCha20
 *                     is a stream cipher, the same operation is used for
 *                     encrypting and decrypting data.
 * Param-Name[in]      size:  The length of the input data in Bytes.
 *                     input: The buffer holding the input data. This pointer
 *                            can be NULL if size == 0.
 * Param-Name[in/out]  ctx: The ChaCha20 context to use for encryption or decryption.
 *                          It must be initialized and bound to a key and nonce.
 * Param-Name[out]     output: The buffer holding the output data. This must be
 *                             able to hold size Bytes. This pointer can be NULL
 *                             if size == 0.
 * Return              None
 */
/******************************************************************************/
void Crypto_chacha20_update(Crypto_Chacha20_Context* ctx, uint32 size, const uint8* input, uint8* output)
{
    uint32 offset   = 0U;
    uint32 tmp_size = size;
    /* Use leftover keystream bytes, if available */
    while ((tmp_size > 0U) && (ctx->keystream_bytes_used < CHACHA20_BLOCK_SIZE_BYTES))
    {
        output[offset] = input[offset] ^ ctx->keystream8[ctx->keystream_bytes_used];

        ctx->keystream_bytes_used++;
        offset++;
        tmp_size--;
    }

    /* Process full blocks */
    while (tmp_size >= CHACHA20_BLOCK_SIZE_BYTES)
    {
        /* Generate new keystream block and increment counter */
        chacha20_block(ctx->state, ctx->keystream8);
        ctx->state[CHACHA20_CTR_INDEX]++;

        Crypto_Xor(CRYPTO_CONST_64, (uint8*)(input + offset), ctx->keystream8, output + offset);

        offset += CHACHA20_BLOCK_SIZE_BYTES;
        tmp_size -= CHACHA20_BLOCK_SIZE_BYTES;
    }

    /* Last (partial) block */
    if (tmp_size > 0U)
    {
        /* Generate new keystream block and increment counter */
        chacha20_block(ctx->state, ctx->keystream8);
        ctx->state[CHACHA20_CTR_INDEX]++;

        Crypto_Xor(tmp_size, (uint8*)(input + offset), ctx->keystream8, output + offset);

        ctx->keystream_bytes_used = tmp_size;
    }
}

/******************************************************************************/
/*
 * Brief               This function encrypts or decrypts data with ChaCha20 and
 *                     the given key and nonce. Since ChaCha20 is a stream cipher,
 *                     the same operation is used for encrypting and decrypting data.
 *
 * Param-Name[in]      key: The encryption/decryption key. This must be 32 Bytes
 *                         in length.
 *                     nonce: The nonce. This must be 12 Bytes in size.
 *                     counter: The initial counter value. This is usually 0.
 *                     data_len: The length of the input data in Bytes.
 *                     input: The buffer holding the input data. This pointer can
 *                             be NULL if size == 0.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer holding the output data. This must be able
 *                              to hold size Bytes. This pointer can be NULL if
 *                              `size == 0`.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_chacha20_crypt(
    const uint8  key[CRYPTO_CONST_32],
    const uint8  nonce[CRYPTO_CONST_12],
    uint32       counter,
    uint32       data_len,
    const uint8* input,
    uint8*       output)
{
    Crypto_Chacha20_Context ctx;
    Std_ReturnType          ret = E_OK;

    Crypto_chacha20_init(&ctx);

    Crypto_chacha20_setkey(&ctx, key);

    Crypto_chacha20_starts(&ctx, nonce, counter);

    Crypto_chacha20_update(&ctx, data_len, input, output);

    return ret;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif
