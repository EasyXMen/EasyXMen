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
 **  @file               : Crypto_62_Blake.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 4544,1252,1253,1290,3432,0553 EOF */ /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"

#if (CRYPTO_ALGORITHMFAM_BLAKE_2s_256 == STD_ON)

#include "Crypto_62_Hash.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/* ===================================================== macros ===================================================== */
#ifndef ROTR32
#define ROTR32(x, y) (((x) >> (y)) ^ ((x) << (32 - (y))))
#endif

/* PRQA S 3472,3410,3458 ++ */ /* VL_QAC_Crypto */

#define B2S_GET32(p)                                                                                     \
    (((uint32)((uint8*)(p))[0]) ^ (((uint32)((uint8*)(p))[1]) << 8) ^ (((uint32)((uint8*)(p))[2]) << 16) \
     ^ (((uint32)((uint8*)(p))[3]) << 24))

/* Mixing function G. */

#define B2S_G(a, b, c, d, x, y)         \
    {                                   \
        v[a] = v[a] + v[b] + x;         \
        v[d] = ROTR32(v[d] ^ v[a], 16); \
        v[c] = v[c] + v[d];             \
        v[b] = ROTR32(v[b] ^ v[c], 12); \
        v[a] = v[a] + v[b] + y;         \
        v[d] = ROTR32(v[d] ^ v[a], 8);  \
        v[c] = v[c] + v[d];             \
        v[b] = ROTR32(v[b] ^ v[c], 7);  \
    }
/* PRQA S 3472,3410,3458 -- */
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* Initialize the hashing context "ctx" with optional key "key".
 *      1 <= outlen <= 32 gives the digest size in bytes.
 *      Secret key (also <= 32 bytes) is optional (keylen = 0).
 *      Returns 0 on success, -1 on parameter error.
 */
CRYPTO_62_LOCAL Std_ReturnType
    Crypto_Blake2s_Start(Crypto_Blake2s_Context* ctx, uint32 outlen, const void* key, uint32 keylen); /*secret key */

/*Add "inlen" bytes from "in" into the hash. */
CRYPTO_62_LOCAL void Crypto_Blake2s_Update(
    Crypto_Blake2s_Context* ctx, /* context*/
    const void*             in,
    uint32                  inlen); /*data to be hashed */

/* Generate the message digest (size given in init).Result placed in "out".*/

CRYPTO_62_LOCAL void Crypto_Blake2s_Final(Crypto_Blake2s_Context* ctx, void* out);

/* All-in-one convenience function.*/
CRYPTO_62_LOCAL Std_ReturnType Crypto_Blake2s(
    void*       out,
    uint32      outlen, /* return buffer for digest */
    const void* key,
    uint32      keylen, /* optional secret key */
    const void* in,
    uint32      inlen); /* data to be hashed*/
/* Compression function. */
CRYPTO_62_LOCAL void Crypto_Blake2s_Compress(Crypto_Blake2s_Context* ctx, int last);
/* Little-endian byte access. */
/* =========================================== Internal data Definitions =========================================== */

/* Initialization Vector.*/
CRYPTO_62_LOCAL const uint32 blake2s_iv[CRYPTO_CONST_8] =
    {0x6A09E667U, 0xBB67AE85U, 0x3C6EF372U, 0xA54FF53AU, 0x510E527FU, 0x9B05688CU, 0x1F83D9ABU, 0x5BE0CD19U};

/* ========================================== external data definition ============================================= */

/* =================================================== Global Function Definitions ================================== */

/******************************************************************************/
/*
 * Brief               Do condense computation.Compression function. "last" flag indicates last block.
 *
 * Param-Name[in]      last: The flag of last block.
 * Param-Name[in/out]  ctx: The Blake2s context.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_Blake2s_Compress(Crypto_Blake2s_Context* ctx, int last)
{
    const uint8 sigma[CRYPTO_CONST_10][CRYPTO_CONST_16] = {
        {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U},
        {14U, 10U, 4U, 8U, 9U, 15U, 13U, 6U, 1U, 12U, 0U, 2U, 11U, 7U, 5U, 3U},
        {11U, 8U, 12U, 0U, 5U, 2U, 15U, 13U, 10U, 14U, 3U, 6U, 7U, 1U, 9U, 4U},
        {7U, 9U, 3U, 1U, 13U, 12U, 11U, 14U, 2U, 6U, 5U, 10U, 4U, 0U, 15U, 8U},
        {9U, 0U, 5U, 7U, 2U, 4U, 10U, 15U, 14U, 1U, 11U, 12U, 6U, 8U, 3U, 13U},
        {2U, 12U, 6U, 10U, 0U, 11U, 8U, 3U, 4U, 13U, 7U, 5U, 15U, 14U, 1U, 9U},
        {12U, 5U, 1U, 15U, 14U, 13U, 4U, 10U, 0U, 7U, 6U, 3U, 9U, 2U, 8U, 11U},
        {13U, 11U, 7U, 14U, 12U, 1U, 3U, 9U, 5U, 0U, 15U, 4U, 8U, 6U, 2U, 10U},
        {6U, 15U, 14U, 9U, 11U, 3U, 0U, 8U, 12U, 2U, 13U, 7U, 1U, 4U, 10U, 5U},
        {10U, 2U, 8U, 4U, 7U, 6U, 1U, 5U, 15U, 11U, 9U, 14U, 3U, 12U, 13U, 0U}};
    uint32 i;
    uint32 v[CRYPTO_CONST_16], m[CRYPTO_CONST_16];

    for (i = 0; i < CRYPTO_CONST_8; i++)
    { // init work variables
        v[i]                  = ctx->h[i];
        v[i + CRYPTO_CONST_8] = blake2s_iv[i];
    }

    v[CRYPTO_CONST_12] ^= ctx->t[0]; // low 32 bits of offset
    v[CRYPTO_CONST_13] ^= ctx->t[1]; // high 32 bits
    if (last > 0)                    // last block flag set ?
    {
        v[CRYPTO_CONST_14] = ~v[CRYPTO_CONST_14];
    }

    for (i = 0; i < CRYPTO_CONST_16; i++) // get little-endian words
    {
        m[i] = B2S_GET32(&ctx->b[CRYPTO_CONST_4 * i]);
    }
    /* PRQA S 3422,3138,3141 ++ */ /* VL_QAC_Crypto */
    for (i = 0; i < CRYPTO_CONST_10; i++)
    { // ten rounds
        B2S_G(
            CRYPTO_CONST_0,
            CRYPTO_CONST_4,
            CRYPTO_CONST_8,
            CRYPTO_CONST_12,
            m[sigma[i][CRYPTO_CONST_0]],
            m[sigma[i][CRYPTO_CONST_1]]);
        B2S_G(
            CRYPTO_CONST_1,
            CRYPTO_CONST_5,
            CRYPTO_CONST_9,
            CRYPTO_CONST_13,
            m[sigma[i][CRYPTO_CONST_2]],
            m[sigma[i][CRYPTO_CONST_3]]);
        B2S_G(
            CRYPTO_CONST_2,
            CRYPTO_CONST_6,
            CRYPTO_CONST_10,
            CRYPTO_CONST_14,
            m[sigma[i][CRYPTO_CONST_4]],
            m[sigma[i][CRYPTO_CONST_5]]);
        B2S_G(
            CRYPTO_CONST_3,
            CRYPTO_CONST_7,
            CRYPTO_CONST_11,
            CRYPTO_CONST_15,
            m[sigma[i][CRYPTO_CONST_6]],
            m[sigma[i][CRYPTO_CONST_7]]);
        B2S_G(
            CRYPTO_CONST_0,
            CRYPTO_CONST_5,
            CRYPTO_CONST_10,
            CRYPTO_CONST_15,
            m[sigma[i][CRYPTO_CONST_8]],
            m[sigma[i][CRYPTO_CONST_9]]);
        B2S_G(
            CRYPTO_CONST_1,
            CRYPTO_CONST_6,
            CRYPTO_CONST_11,
            CRYPTO_CONST_12,
            m[sigma[i][CRYPTO_CONST_10]],
            m[sigma[i][CRYPTO_CONST_11]]);
        B2S_G(
            CRYPTO_CONST_2,
            CRYPTO_CONST_7,
            CRYPTO_CONST_8,
            CRYPTO_CONST_13,
            m[sigma[i][CRYPTO_CONST_12]],
            m[sigma[i][CRYPTO_CONST_13]]);
        B2S_G(
            CRYPTO_CONST_3,
            CRYPTO_CONST_4,
            CRYPTO_CONST_9,
            CRYPTO_CONST_14,
            m[sigma[i][CRYPTO_CONST_14]],
            m[sigma[i][CRYPTO_CONST_15]]);
    }
    /* PRQA S 3422,3138,3141 -- */
    for (i = 0; i < CRYPTO_CONST_8; ++i)
    {
        ctx->h[i] ^= v[i] ^ v[i + CRYPTO_CONST_8];
    }
}

/******************************************************************************/
/*
 * Brief               Initialize the hashing context "ctx" with optional key "key".
 *                     1 <= outlen <= 32 gives the digest size in bytes.
 *                     Secret key (also <= 32 bytes) is optional (keylen = 0).
 *
 * Param-Name[in]      outlen: The length of output digest in byte.
 *                     key: The optional secret key.
 *                     keylen: The length of key in byte.
 * Param-Name[in/out]  ctx: The context of Blake2s.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  0: Success
 *                           not 0: Failure
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType Crypto_Blake2s_Start(
    Crypto_Blake2s_Context* ctx,
    uint32                  outlen,
    const void*             key,
    uint32                  keylen) /* (keylen=0: no key) */
{
    uint32 i;

    if ((outlen == CRYPTO_CONST_0) || (outlen > CRYPTO_CONST_32) || (keylen > CRYPTO_CONST_32))
    {
        return E_NOT_OK; // illegal parameters
    }

    for (i = 0; i < CRYPTO_CONST_8; i++) // state, "param block"
    {
        ctx->h[i] = blake2s_iv[i];
    }
    /* PRQA S 3120 ++ */ /* VL_QAC_Crypto */
    ctx->h[0] ^= 0x01010000U ^ (keylen << 8) ^ outlen;
    /* PRQA S 3120 -- */

    ctx->t[0]   = 0; // input count low word
    ctx->t[1]   = 0; // input count high word
    ctx->c      = 0; // pointer within buffer
    ctx->outlen = outlen;

    for (i = keylen; i < CRYPTO_CONST_64; i++) // zero input block
    {
        ctx->b[i] = 0;
    }
    if (keylen > CRYPTO_CONST_0)
    {
        Crypto_Blake2s_Update(ctx, key, keylen);
        ctx->c = CRYPTO_CONST_64; // at the end
    }

    return E_OK;
}

/******************************************************************************/
/*
 * Brief               Take in inlen byte from input and calculate Blake2s. This
 *                     function can be called any times.
 *
 * Param-Name[in]      in: Message to be digested.
 *                     inlen: The length of input message in byte.
 * Param-Name[in/out]  ctx: The context of Blake2s.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_Blake2s_Update(Crypto_Blake2s_Context* ctx, const void* in, uint32 inlen) // data bytes
{
    uint32 i;

    for (i = 0; i < inlen; i++)
    {
        if (ctx->c == CRYPTO_CONST_64)
        {                           // buffer full ?
            ctx->t[0] += ctx->c;    // add counters
            if (ctx->t[0] < ctx->c) // carry overflow ?
            {
                ctx->t[1]++; // high word
            }
            Crypto_Blake2s_Compress(ctx, 0); // compress (not last)
            ctx->c = 0;                      // counter to zero
        }
        /* PRQA S 0316,3440,3387 ++ */ /* VL_QAC_Crypto */
        ctx->b[ctx->c++] = ((const uint8*)in)[i];
        /* PRQA S 0316,3440,3387 -- */
    }
}

/******************************************************************************/
/*
 * Brief               Do padding, complete the last computation, and get the
 *                     digest from the context. This function can only be called
 *                     once.
 *
 * Param-Name[in]      ctx: The context of Blake2s.
 * Param-Name[in/out]  None
 * Param-Name[out]     out: Buffer to store digest.
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_Blake2s_Final(Crypto_Blake2s_Context* ctx, void* out)
{
    uint32 i;

    ctx->t[0] += ctx->c;    // mark last block offset
    if (ctx->t[0] < ctx->c) // carry overflow
    {
        ctx->t[1]++; // high word
    }

    while (ctx->c < CRYPTO_CONST_64) // fill up with zeros
    {
        ctx->b[ctx->c++] = 0; /* PRQA S 3387,3440*/ /* VL_QAC_Crypto */
    }
    Crypto_Blake2s_Compress(ctx, 1); // final block flag = 1

    // little endian convert and store
    for (i = 0; i < ctx->outlen; i++)
    {
        /* PRQA S 0316,4461 ++ */ /* VL_QAC_Crypto */
        ((uint8*)out)[i] = (ctx->h[i >> CRYPTO_CONST_2] >> (CRYPTO_CONST_8 * (i & CRYPTO_CONST_3))) & CRYPTO_CONST_0xFF;
        /* PRQA S 0316,4461 -- */
    }
}

/******************************************************************************/
/*
 * Brief               The upper interface to compute blake2s on input.
 *
 * Param-Name[in]      key: Secret key.
 *                     keylen: The length of key in byte.
 *                     in: The message needed to be digested.
 *                     inlen: The length of input message in byte.
 * Param-Name[in/out]  None
 * Param-Name[out]     out: Buffer to stored blake2s digest value.
 *                     outlen: The length of digest in byte.
 * Return              Std_ReturnType:  0: Success.
 *                           not 0: Failure.
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType
    Crypto_Blake2s(void* out, uint32 outlen, const void* key, uint32 keylen, const void* in, uint32 inlen)
{
    Crypto_Blake2s_Context ctx;
    Std_ReturnType         ret;

    ret = Crypto_Blake2s_Start(&ctx, outlen, key, keylen);

    if (ret == E_OK)
    {
        Crypto_Blake2s_Update(&ctx, in, inlen);
        Crypto_Blake2s_Final(&ctx, out);
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function do the blake2s process .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
Std_ReturnType Crypto_Blake2s_Process(uint32 objectId)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          output[CRYPTO_CONST_32];

    uint32 ilen   = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32 outlen = *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr);
    if (outlen <= CRYPTO_CONST_32)
    {
        /* PRQA S 0311,3678 ++ */ /*VL_QAC_0311 */
        uint8* input = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
        /* PRQA S 0311,3678 -- */

        ret = Crypto_Blake2s(output, outlen, NULL_PTR, 0u, input, ilen);
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, outlen);
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif
