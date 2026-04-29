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
 **  @file               : Crypto_62_Sha1.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 0314,0488,0842,1253,1255,1277,1281,1290,1338,1840,1841 EOF */ /* VL_Crypto_62_General */
/* PRQA S 1842,1843,1863,2001,2015,2216,3120,3132,3138,3226,3417 EOF */ /* VL_Crypto_62_General */
/* PRQA S 3440,3446,3472,3473,3493,4542,4559,4558,1252,3387,3397 EOF */ /* VL_Crypto_62_General */
/* PRQA S 3400,3412,3678,4115,4544,0553 EOF */                          /* VL_Crypto_62_General */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SHA1 == STD_ON)
#include "Crypto_62_Sha1.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
/******************************************************************************/
/*
 * Brief               This function starts a SHA-1 checksum calculation.

 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The SHA-1 context to initialize.
 * Return              None
 */
/******************************************************************************/
void Crypto_Sha1_Starts(Crypto_Sha1_Context* ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x67452301U;
    ctx->state[1] = 0xEFCDAB89U;
    ctx->state[2] = 0x98BADCFEU;
    ctx->state[3] = 0x10325476U;
    ctx->state[4] = 0xC3D2E1F0U;
}

/******************************************************************************/
/*
 * Brief               SHA-1 process data block (internal use only).

 * Param-Name[in]      data: The data block being processed. This must be a
 *                           readable buffer of length 64 Bytes.
 * Param-Name[in/out]  ctx: The SHA-1 context to use. This must be initialized.
 * Param-Name[out]     None
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Internal_Sha1_Process(Crypto_Sha1_Context* ctx, const unsigned char data[64])
{
    struct
    {
        uint32 temp, W[16], A, B, C, D, E;
    } local;

    local.W[0]  = CRYPTO_GET_UINT32_BE(data, 0);
    local.W[1]  = CRYPTO_GET_UINT32_BE(data, 4);
    local.W[2]  = CRYPTO_GET_UINT32_BE(data, 8);
    local.W[3]  = CRYPTO_GET_UINT32_BE(data, 12);
    local.W[4]  = CRYPTO_GET_UINT32_BE(data, 16);
    local.W[5]  = CRYPTO_GET_UINT32_BE(data, 20);
    local.W[6]  = CRYPTO_GET_UINT32_BE(data, 24);
    local.W[7]  = CRYPTO_GET_UINT32_BE(data, 28);
    local.W[8]  = CRYPTO_GET_UINT32_BE(data, 32);
    local.W[9]  = CRYPTO_GET_UINT32_BE(data, 36);
    local.W[10] = CRYPTO_GET_UINT32_BE(data, 40);
    local.W[11] = CRYPTO_GET_UINT32_BE(data, 44);
    local.W[12] = CRYPTO_GET_UINT32_BE(data, 48);
    local.W[13] = CRYPTO_GET_UINT32_BE(data, 52);
    local.W[14] = CRYPTO_GET_UINT32_BE(data, 56);
    local.W[15] = CRYPTO_GET_UINT32_BE(data, 60);

#define S(x, n) (((x) << (n)) | (((x) & 0xFFFFFFFF) >> (CRYPTO_CONST_32 - (n))))

#define R(t)                                                                                                 \
    (local.temp = local.W[((t) - CRYPTO_CONST_3) & CRYPTO_CONST_0x0F]                                        \
                  ^ local.W[((t) - CRYPTO_CONST_8) & CRYPTO_CONST_0x0F]                                      \
                  ^ local.W[((t) - CRYPTO_CONST_14) & CRYPTO_CONST_0x0F] ^ local.W[(t) & CRYPTO_CONST_0x0F], \
     (local.W[(t) & CRYPTO_CONST_0x0F] = S(local.temp, CRYPTO_CONST_1)))

#define P(a, b, c, d, e, x)                                         \
    do                                                              \
    {                                                               \
        (e) += S((a), CRYPTO_CONST_5) + F((b), (c), (d)) + K + (x); \
        (b) = S((b), CRYPTO_CONST_30);                              \
    } while (CRYPTO_CONST_0)

    local.A = ctx->state[0];
    local.B = ctx->state[1];
    local.C = ctx->state[2];
    local.D = ctx->state[3];
    local.E = ctx->state[4];

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define K          0x5A827999

    P(local.A, local.B, local.C, local.D, local.E, local.W[0]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[1]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[2]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[3]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[4]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[5]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[6]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[7]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[8]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[9]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[10]);
    P(local.E, local.A, local.B, local.C, local.D, local.W[11]);
    P(local.D, local.E, local.A, local.B, local.C, local.W[12]);
    P(local.C, local.D, local.E, local.A, local.B, local.W[13]);
    P(local.B, local.C, local.D, local.E, local.A, local.W[14]);
    P(local.A, local.B, local.C, local.D, local.E, local.W[15]);
    P(local.E, local.A, local.B, local.C, local.D, R(16));
    P(local.D, local.E, local.A, local.B, local.C, R(17));
    P(local.C, local.D, local.E, local.A, local.B, R(18));
    P(local.B, local.C, local.D, local.E, local.A, R(19));

#undef K
#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define K          0x6ED9EBA1

    P(local.A, local.B, local.C, local.D, local.E, R(20));
    P(local.E, local.A, local.B, local.C, local.D, R(21));
    P(local.D, local.E, local.A, local.B, local.C, R(22));
    P(local.C, local.D, local.E, local.A, local.B, R(23));
    P(local.B, local.C, local.D, local.E, local.A, R(24));
    P(local.A, local.B, local.C, local.D, local.E, R(25));
    P(local.E, local.A, local.B, local.C, local.D, R(26));
    P(local.D, local.E, local.A, local.B, local.C, R(27));
    P(local.C, local.D, local.E, local.A, local.B, R(28));
    P(local.B, local.C, local.D, local.E, local.A, R(29));
    P(local.A, local.B, local.C, local.D, local.E, R(30));
    P(local.E, local.A, local.B, local.C, local.D, R(31));
    P(local.D, local.E, local.A, local.B, local.C, R(32));
    P(local.C, local.D, local.E, local.A, local.B, R(33));
    P(local.B, local.C, local.D, local.E, local.A, R(34));
    P(local.A, local.B, local.C, local.D, local.E, R(35));
    P(local.E, local.A, local.B, local.C, local.D, R(36));
    P(local.D, local.E, local.A, local.B, local.C, R(37));
    P(local.C, local.D, local.E, local.A, local.B, R(38));
    P(local.B, local.C, local.D, local.E, local.A, R(39));

#undef K
#undef F

#define F(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define K          0x8F1BBCDC

    P(local.A, local.B, local.C, local.D, local.E, R(40));
    P(local.E, local.A, local.B, local.C, local.D, R(41));
    P(local.D, local.E, local.A, local.B, local.C, R(42));
    P(local.C, local.D, local.E, local.A, local.B, R(43));
    P(local.B, local.C, local.D, local.E, local.A, R(44));
    P(local.A, local.B, local.C, local.D, local.E, R(45));
    P(local.E, local.A, local.B, local.C, local.D, R(46));
    P(local.D, local.E, local.A, local.B, local.C, R(47));
    P(local.C, local.D, local.E, local.A, local.B, R(48));
    P(local.B, local.C, local.D, local.E, local.A, R(49));
    P(local.A, local.B, local.C, local.D, local.E, R(50));
    P(local.E, local.A, local.B, local.C, local.D, R(51));
    P(local.D, local.E, local.A, local.B, local.C, R(52));
    P(local.C, local.D, local.E, local.A, local.B, R(53));
    P(local.B, local.C, local.D, local.E, local.A, R(54));
    P(local.A, local.B, local.C, local.D, local.E, R(55));
    P(local.E, local.A, local.B, local.C, local.D, R(56));
    P(local.D, local.E, local.A, local.B, local.C, R(57));
    P(local.C, local.D, local.E, local.A, local.B, R(58));
    P(local.B, local.C, local.D, local.E, local.A, R(59));

#undef K
#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))
#define K          0xCA62C1D6

    P(local.A, local.B, local.C, local.D, local.E, R(60));
    P(local.E, local.A, local.B, local.C, local.D, R(61));
    P(local.D, local.E, local.A, local.B, local.C, R(62));
    P(local.C, local.D, local.E, local.A, local.B, R(63));
    P(local.B, local.C, local.D, local.E, local.A, R(64));
    P(local.A, local.B, local.C, local.D, local.E, R(65));
    P(local.E, local.A, local.B, local.C, local.D, R(66));
    P(local.D, local.E, local.A, local.B, local.C, R(67));
    P(local.C, local.D, local.E, local.A, local.B, R(68));
    P(local.B, local.C, local.D, local.E, local.A, R(69));
    P(local.A, local.B, local.C, local.D, local.E, R(70));
    P(local.E, local.A, local.B, local.C, local.D, R(71));
    P(local.D, local.E, local.A, local.B, local.C, R(72));
    P(local.C, local.D, local.E, local.A, local.B, R(73));
    P(local.B, local.C, local.D, local.E, local.A, R(74));
    P(local.A, local.B, local.C, local.D, local.E, R(75));
    P(local.E, local.A, local.B, local.C, local.D, R(76));
    P(local.D, local.E, local.A, local.B, local.C, R(77));
    P(local.C, local.D, local.E, local.A, local.B, R(78));
    P(local.B, local.C, local.D, local.E, local.A, R(79));

#undef K
#undef F

    ctx->state[0] += local.A;
    ctx->state[1] += local.B;
    ctx->state[2] += local.C;
    ctx->state[3] += local.D;
    ctx->state[4] += local.E;

    /* Zeroise buffers and variables to clear sensitive data from memory. */
    return E_OK;
}

/******************************************************************************/
/*
 * Brief               This function feeds an input buffer into an ongoing SHA-1
 *                     checksum calculation.
 *
 * Param-Name[in]      input: The buffer holding the input data. This must be a
 *                            readable buffer of length ilen Bytes.
 *                     ilen: The length of the input data input in Bytes.
 * Param-Name[in/out]  ctx: The SHA-1 context. This must be initialized and have
 *                          a hash operation started.
 * Param-Name[out]     None
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha1_Update(Crypto_Sha1_Context* ctx, const unsigned char* input, uint32 ilen)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32         fill;
    uint32         left;

    if (ilen == 0)
    {
        return E_NOT_OK;
    }

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (uint32)ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < (uint32)ilen)
    {
        ctx->total[1]++;
    }

    if (left && ilen >= fill)
    {
        (void)IStdLib_MemCpy((void*)(ctx->buffer + left), input, fill);
        ret = Crypto_Internal_Sha1_Process(ctx, ctx->buffer);
        if (ret != E_OK)
        {
            return ret;
        }

        input += fill;
        ilen -= fill;
        left = 0;
    }

    while (ilen >= 64)
    {
        ret = Crypto_Internal_Sha1_Process(ctx, input);
        if (ret != E_OK)
        {
            return ret;
        }

        input += 64;
        ilen -= 64;
    }

    if (ilen > 0)
    {
        (void)IStdLib_MemCpy((void*)(ctx->buffer + left), input, ilen);
    }

    return E_OK;
}

/******************************************************************************/
/*
 * Brief               This function finishes the SHA-1 operation, and writes
 *                     the result to the output buffer.
 *
 * Param-Name[in]      ctx: The SHA-1 context to use. This must be initialized
 *                          and have a hash operation started.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-1 checksum result. This must be a writable
 *                             buffer of length 20 Bytes.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha1_Finish(Crypto_Sha1_Context* ctx, unsigned char output[20])
{
    Std_ReturnType ret = E_NOT_OK;
    uint32         used;
    uint32         high, low;

    /*
     * Add padding: 0x80 then 0x00 until 8 bytes remain for the length
     */
    used = ctx->total[0] & 0x3F;

    ctx->buffer[used++] = 0x80;

    if (used <= 56)
    {
        /* Enough room for padding + length in current block */
        (void)IStdLib_MemSet(ctx->buffer + used, 0, 56 - used);
    }
    else
    {
        /* We'll need an extra block */
        (void)IStdLib_MemSet(ctx->buffer + used, 0, 64 - used);
        ret = Crypto_Internal_Sha1_Process(ctx, ctx->buffer);
        if (ret != E_OK)
        {
            goto exit;
        }

        (void)IStdLib_MemSet(ctx->buffer, 0, 56);
    }

    /*
     * Add message length
     */
    high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    low  = (ctx->total[0] << 3);

    CRYPTO_PUT_UINT32_BE(high, ctx->buffer, 56);
    CRYPTO_PUT_UINT32_BE(low, ctx->buffer, 60);
    ret = Crypto_Internal_Sha1_Process(ctx, ctx->buffer);
    if (ret != E_OK)
    {
        goto exit;
    }

    /*
     * Output final state
     */
    CRYPTO_PUT_UINT32_BE(ctx->state[0], output, 0);
    CRYPTO_PUT_UINT32_BE(ctx->state[1], output, 4);
    CRYPTO_PUT_UINT32_BE(ctx->state[2], output, 8);
    CRYPTO_PUT_UINT32_BE(ctx->state[3], output, 12);
    CRYPTO_PUT_UINT32_BE(ctx->state[4], output, 16);

    ret = E_OK;

exit:
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function calculates the SHA-1 checksum of a buffer.
 *                     The function allocates the context, performs the calculation,
 *                     and frees the context. The SHA-1 result is calculated as
 *                     output = SHA-1(input buffer).
 *
 * Param-Name[in]      input: The buffer holding the input data. This must be a
 *                            readable buffer of length ilen Bytes.
 *                     ilen: The length of the input data \p input in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-1 checksum result. This must be a writable
 *                             buffer of length 20 Bytes.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha1(const unsigned char* input, uint32 ilen, unsigned char output[20])
{
    Std_ReturnType      ret = E_NOT_OK;
    Crypto_Sha1_Context ctx;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Sha1_Context));
    Crypto_Sha1_Starts(&ctx);

    ret = Crypto_Sha1_Update(&ctx, input, ilen);

    if (ret == E_OK)
    {
        ret = Crypto_Sha1_Finish(&ctx, output);
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function calculates the SHA-1 checksum and output result.
 *
 * Param-Name[in]      objectId: The object ID of the job.
 * Param-Name[in/out]  None
 * Param-Name[out]     None.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha1_Process(uint32 objectId)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          output[CRYPTO_CONST_20];

    uint32  ilen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32* olen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;

    /* PRQA S 0311 ++ */ /*VL_Crypto_62_General */
    uint8* input = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    /* PRQA S 0311 -- */

    ret = Crypto_Sha1(input, ilen, output);
    if (*olen >= CRYPTO_CONST_20 && ret == E_OK)
    {
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, CRYPTO_CONST_20);
        *olen = CRYPTO_CONST_20;
    }
    else
    {
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, *olen);
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif
