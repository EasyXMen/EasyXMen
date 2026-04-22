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
 **  @file               : Crypto_62_Sha256.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 6010 EOF */ /* VL_MTR_Crypto_62_STCYC */
/* PRQA S 6020 EOF */ /* VL_MTR_Crypto_62_STLIN */
/* PRQA S 6030 EOF */ /* VL_MTR_Crypto_62_STMIF */
/* PRQA S 6040 EOF */ /* VL_MTR_Crypto_62_STPAR */
/* PRQA S 6050 EOF */ /* VL_MTR_Crypto_62_STST3 */
/* PRQA S 6060 EOF */ /* VL_MTR_Crypto_62_STM19 */
/* PRQA S 6070 EOF */ /* VL_MTR_Crypto_62_STCAL */
/* PRQA S 6080 EOF */ /* VL_MTR_Crypto_62_STPTH */

/* PRQA S 0488,0314,1252,1253,1255,1277,1281,1290,1338,1840,1841,1842,1843 ++ */ /* VL_Crypto_62_General */
/* PRQA S 1863,2001,2015,2109,2216,3120,3218,3226,3232,3326,3387,3440,3472 ++ */ /* VL_Crypto_62_General */
/* PRQA S 3473,3678,4115,4542,4544,4558,2889,2985,2743 ++ */                     /* VL_Crypto_62_General */
/* PRQA S 2784,1532 ++ */                                                        /* VL_Crypto_62_General */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SHA2_256 == STD_ON)

#include "Crypto_62_Sha256.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/* ===================================================== macros ===================================================== */

/*
 * 32-bit integer manipulation macros(big endian)
 */
#define SHA256_BLOCK_SIZE (64u)
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n, b, i)                                                                      \
    do                                                                                              \
    {                                                                                               \
        (n) = ((uint32)(b)[(i)] << 24) | ((uint32)(b)[(i) + 1] << 16) | ((uint32)(b)[(i) + 2] << 8) \
              | ((uint32)(b)[(i) + 3]);                                                             \
    } while (0)
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(x, b, i)                        \
    do                                                \
    {                                                 \
        (b)[(i)]     = ((uint8)(((x) >> 24) & 0xff)); \
        (b)[(i) + 1] = ((uint8)(((x) >> 16) & 0xff)); \
        (b)[(i) + 2] = ((uint8)(((x) >> 8) & 0xff));  \
        (b)[(i) + 3] = ((uint8)(((x) >> 0) & 0xff));  \
    } while (0)
#endif

#define SHR(x, n)  (((x) & 0xFFFFFFFF) >> (n))
#define ROTR(x, n) (SHR(x, n) | ((x) << (CRYPTO_CONST_32 - (n))))

#define S0(x) (ROTR(x, CRYPTO_CONST_7) ^ ROTR(x, CRYPTO_CONST_18) ^ SHR(x, CRYPTO_CONST_3))
#define S1(x) (ROTR(x, CRYPTO_CONST_17) ^ ROTR(x, CRYPTO_CONST_19) ^ SHR(x, CRYPTO_CONST_10))

#define S2(x) (ROTR(x, CRYPTO_CONST_2) ^ ROTR(x, CRYPTO_CONST_13) ^ ROTR(x, CRYPTO_CONST_22))
#define S3(x) (ROTR(x, CRYPTO_CONST_6) ^ ROTR(x, CRYPTO_CONST_11) ^ ROTR(x, CRYPTO_CONST_25))

#define F0(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

#define R(t)                                                                        \
    (local.W[t] = S1(local.W[(t) - CRYPTO_CONST_2]) + local.W[(t) - CRYPTO_CONST_7] \
                  + S0(local.W[(t) - CRYPTO_CONST_15]) + local.W[(t) - CRYPTO_CONST_16])

#define P(a, b, c, d, e, f, g, h, x, K)                            \
    do                                                             \
    {                                                              \
        local.temp1 = (h) + S3(e) + F1((e), (f), (g)) + (K) + (x); \
        local.temp2 = S2(a) + F0((a), (b), (c));                   \
        (d) += local.temp1;                                        \
        (h) = local.temp1 + local.temp2;                           \
    } while (0)
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
CRYPTO_62_LOCAL Std_ReturnType
                       Crypto_internal_sha256_process(Crypto_Sha256_Context* ctx, const uint8 data[CRYPTO_CONST_64]);
CRYPTO_62_LOCAL uint32 Crypto_internal_sha256_process_Many(Crypto_Sha256_Context* ctx, const uint8* data, uint32 len);

/* ============================================ internal data definitions =========================================== */

CRYPTO_62_LOCAL uint32 K[] = {
    0x428A2F98U, 0x71374491U, 0xB5C0FBCFU, 0xE9B5DBA5U, 0x3956C25BU, 0x59F111F1U, 0x923F82A4U, 0xAB1C5ED5U,
    0xD807AA98U, 0x12835B01U, 0x243185BEU, 0x550C7DC3U, 0x72BE5D74U, 0x80DEB1FEU, 0x9BDC06A7U, 0xC19BF174U,
    0xE49B69C1U, 0xEFBE4786U, 0x0FC19DC6U, 0x240CA1CCU, 0x2DE92C6FU, 0x4A7484AAU, 0x5CB0A9DCU, 0x76F988DAU,
    0x983E5152U, 0xA831C66DU, 0xB00327C8U, 0xBF597FC7U, 0xC6E00BF3U, 0xD5A79147U, 0x06CA6351U, 0x14292967U,
    0x27B70A85U, 0x2E1B2138U, 0x4D2C6DFCU, 0x53380D13U, 0x650A7354U, 0x766A0ABBU, 0x81C2C92EU, 0x92722C85U,
    0xA2BFE8A1U, 0xA81A664BU, 0xC24B8B70U, 0xC76C51A3U, 0xD192E819U, 0xD6990624U, 0xF40E3585U, 0x106AA070U,
    0x19A4C116U, 0x1E376C08U, 0x2748774CU, 0x34B0BCB5U, 0x391C0CB3U, 0x4ED8AA4AU, 0x5B9CCA4FU, 0x682E6FF3U,
    0x748F82EEU, 0x78A5636FU, 0x84C87814U, 0x8CC70208U, 0x90BEFFFAU, 0xA4506CEBU, 0xBEF9A3F7U, 0xC67178F2U,
};

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               This function starts a SHA-224 or SHA-256 checksum calculation.
 *
 * Param-Name[in]      ctx: The context to use.
 *				       is224: This determines which function to use. This must be
 *                            either 0 for SHA-256, or 1 for SHA-224.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_Sha256_Starts(Crypto_Sha256_Context* ctx, boolean is224)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    if (FALSE == is224)
    {
        /* SHA-256 */
        ctx->state[0]              = 0x6A09E667U;
        ctx->state[1]              = 0xBB67AE85U;
        ctx->state[CRYPTO_CONST_2] = 0x3C6EF372U;
        ctx->state[CRYPTO_CONST_3] = 0xA54FF53AU;
        ctx->state[CRYPTO_CONST_4] = 0x510E527FU;
        ctx->state[CRYPTO_CONST_5] = 0x9B05688CU;
        ctx->state[CRYPTO_CONST_6] = 0x1F83D9ABU;
        ctx->state[CRYPTO_CONST_7] = 0x5BE0CD19U;
    }
    else
    {
        ctx->state[0]              = 0xC1059ED8U;
        ctx->state[1]              = 0x367CD507U;
        ctx->state[CRYPTO_CONST_2] = 0x3070DD17U;
        ctx->state[CRYPTO_CONST_3] = 0xF70E5939U;
        ctx->state[CRYPTO_CONST_4] = 0xFFC00B31U;
        ctx->state[CRYPTO_CONST_5] = 0x68581511U;
        ctx->state[CRYPTO_CONST_6] = 0x64F98FA7U;
        ctx->state[CRYPTO_CONST_7] = 0xBEFA4FA4U;
    }
}

/******************************************************************************/
/*
 * Brief               This function feeds an input buffer into an ongoing SHA-256
 *                     checksum calculation.
 *
 * Param-Name[in]      input: The buffer holding the data. This must be a readable
 *                            buffer of length ilen Bytes.
 *				       ilen: The length of the input data in Bytes.
 * Param-Name[in/out]  ctx: The SHA-256 context. This must be initialized and have
 *                          a hash operation started.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha256_Update(Crypto_Sha256_Context* ctx, const uint8* input, uint32 ilen)
{
    Std_ReturnType ret = E_OK;
    uint32         fill;
    uint32         left;
    uint32         processed;

    if (ilen == 0)
    {
        goto exit;
    }

    left = ctx->total[0] & 0x3F;
    fill = CRYPTO_CONST_64 - left;

    ctx->total[0] += (uint32)ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < ilen)
    {
        ctx->total[1]++;
    }

    if (left && ilen >= fill) /* PRQA S 3397,3400 */ /*VL_Crypto_62_General */
    {
        (void)IStdLib_MemCpy((void*)(ctx->buffer + left), input, fill);

        if ((ret = Crypto_internal_sha256_process(ctx, ctx->buffer)) != 0)
        {
            goto exit;
        }

        input += fill;
        ilen -= fill;
        left = 0;
    }

    while (ilen >= SHA256_BLOCK_SIZE)
    {
        processed = Crypto_internal_sha256_process_Many(ctx, input, ilen);
        if (processed < SHA256_BLOCK_SIZE)
        {
            return E_NOT_OK;
        }

        input += processed;
        ilen -= processed;
    }

    if (ilen > 0)
    {
        (void)IStdLib_MemCpy((void*)(ctx->buffer + left), input, ilen);
    }

exit:
    return (ret);
}

/******************************************************************************/
/*
 * Brief               This function finishes the SHA-256 operation, and writes
 *                     the result to the output buffer.
 *
 * Param-Name[in]      ctx: The SHA-256 context. This must be initialized and
 *                          have a hash operation started.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-224 or SHA-256 checksum result. This must
 *                             be a writable buffer of length 32 bytes for SHA-256,
 *                             28 bytes for SHA-224.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha256_Finish(Crypto_Sha256_Context* ctx, uint8 output[CRYPTO_CONST_32])
{
    uint32         used;
    uint32         high, low;
    Std_ReturnType ret;
    /*
     * Add padding: 0x80 then 0x00 until 8 bytes remain for the length
     */
    used = ctx->total[0] & 0x3F;

    ctx->buffer[used++] = 0x80; /* PRQA S 3440  */ /* VL_Crypto_62_General */

    if (used <= CRYPTO_CONST_56)
    {
        /* Enough room for padding + length in current block */
        (void)IStdLib_MemSet((ctx->buffer + used), 0, (CRYPTO_CONST_56 - used));
    }
    else
    {
        /* We'll need an extra block */
        (void)IStdLib_MemSet((ctx->buffer + used), 0, (CRYPTO_CONST_64 - used));
        if ((ret = Crypto_internal_sha256_process(ctx, ctx->buffer)) != 0)
        {
            goto exit;
        }
        (void)IStdLib_MemSet((ctx->buffer), 0, CRYPTO_CONST_56);
    }

    /*
     * Add message length
     */
    high = (ctx->total[0] >> CRYPTO_CONST_29) | (ctx->total[1] << 3);
    low  = (ctx->total[0] << CRYPTO_CONST_3);

    PUT_UINT32_BE(high, ctx->buffer, CRYPTO_CONST_56);
    PUT_UINT32_BE(low, ctx->buffer, CRYPTO_CONST_60);

    if ((ret = Crypto_internal_sha256_process(ctx, ctx->buffer)) != 0)
    {
        goto exit;
    }
    /*
     * Output final state
     */
    PUT_UINT32_BE(ctx->state[0], output, CRYPTO_CONST_0);
    PUT_UINT32_BE(ctx->state[1], output, CRYPTO_CONST_4);
    PUT_UINT32_BE(ctx->state[2], output, CRYPTO_CONST_8);
    PUT_UINT32_BE(ctx->state[3], output, CRYPTO_CONST_12);
    PUT_UINT32_BE(ctx->state[4], output, CRYPTO_CONST_16);
    PUT_UINT32_BE(ctx->state[5], output, CRYPTO_CONST_20);
    PUT_UINT32_BE(ctx->state[6], output, CRYPTO_CONST_24);
    PUT_UINT32_BE(ctx->state[7], output, CRYPTO_CONST_28);
exit:
    return (ret);
}

/******************************************************************************/
/*
 * Brief               This function divides data into groups and processes one
 *                     group each time until all groups are processed.
 *
 * Param-Name[in]      data: Data to be processed.
 *                     len: The length of data.
 * Param-Name[in/out]  ctx: The SHA-256 context. This must be initialized and
 *                          have a hash operation started.
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
CRYPTO_62_LOCAL uint32 Crypto_internal_sha256_process_Many(Crypto_Sha256_Context* ctx, const uint8* data, uint32 len)
{
    uint32 processed = 0;

    while (len >= SHA256_BLOCK_SIZE)
    {
        if (Crypto_internal_sha256_process(ctx, data) != 0)
        {
            return 0;
        }

        data += SHA256_BLOCK_SIZE;
        len -= SHA256_BLOCK_SIZE;

        processed += SHA256_BLOCK_SIZE;
    }

    return processed;
}

/******************************************************************************/
/*
 * Brief               This function processes one group.
 * Param-Name[in]      data: Data to be processed.
 * Param-Name[in/out]  ctx: The SHA-256 context. This must be initialized and
 *                           have a hash operation started.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType
    Crypto_internal_sha256_process(Crypto_Sha256_Context* ctx, const uint8 data[CRYPTO_CONST_64])
{
    struct
    {
        uint32 temp1, temp2, W[CRYPTO_CONST_64];
        uint32 A[CRYPTO_CONST_8];
    } local;
    uint32         i;
    Std_ReturnType ret = E_OK;

    for (i = 0; i < CRYPTO_CONST_8; i++)
    {
        local.A[i] = ctx->state[i];
    }

    for (i = 0; i < CRYPTO_CONST_16; i++)
    {
        GET_UINT32_BE(local.W[i], data, CRYPTO_CONST_4 * i);
    }

    for (i = 0; i < CRYPTO_CONST_16; i += 8)
    {
        P(local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.W[i + 0],
          K[i + 0]);
        P(local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.W[i + 1],
          K[i + 1]);
        P(local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.W[i + 2],
          K[i + 2]);
        P(local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.W[i + 3],
          K[i + 3]);
        P(local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.W[i + 4],
          K[i + 4]);
        P(local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.W[i + 5],
          K[i + 5]);
        P(local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.W[i + 6],
          K[i + 6]);
        P(local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.W[i + 7],
          K[i + 7]);
    }

    for (i = 16; i < 64; i += 8)
    {
        P(local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          R(i + 0),
          K[i + 0]);
        P(local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          R(i + 1),
          K[i + 1]);
        P(local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          R(i + 2),
          K[i + 2]);
        P(local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          R(i + 3),
          K[i + 3]);
        P(local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          R(i + 4),
          K[i + 4]);
        P(local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          R(i + 5),
          K[i + 5]);
        P(local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          R(i + 6),
          K[i + 6]);
        P(local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          R(i + 7),
          K[i + 7]);
    }

    for (i = 0; i < 8; i++)
    {
        ctx->state[i] += local.A[i];
    }

    (void)IStdLib_MemSet(&local, 0, sizeof(local));
    return (ret);
}
/******************************************************************************/
/*
 * Brief               This function calculates the SHA-224 or SHA-256 checksum
 *                     of a buffer.
 *
 * Param-Name[in]      input: The buffer holding the data. This must be a readable
 *                            buffer of length ilen Bytes.
 *                     ilen: The length of the input data in Bytes.
 *                     is224: Determines which function to use. This must be
 *                            either 0 for SHA-256, or 1 for SHA-224.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-224 or SHA-256 checksum result. This must
 *                             be a writable buffer of length 32 bytes for SHA-256,
 *                             28 bytes for SHA-224.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/

Std_ReturnType Crypto_Sha256(const uint8* input, uint32 ilen, uint8* output, boolean is224)
{
    Std_ReturnType        ret;
    Crypto_Sha256_Context ctx;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Sha256_Context));
    Crypto_Sha256_Starts(&ctx, is224);

    ret = Crypto_Sha256_Update(&ctx, input, ilen);
    if (E_OK == ret)
    {
        ret = Crypto_Sha256_Finish(&ctx, output); /* PRQA S 2784 */ /* VL_Crypto_62_General */
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function calculates the SHA2-256 checksum and output result.
 *
 * Param-Name[in]      objectId: The object ID of the job.
 *                     is224: The length of the SHA2-256 family is 224bit or not.
 * Param-Name[in/out]  None
 * Param-Name[out]     None.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha256_Process(uint32 objectId, boolean is224)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          output[CRYPTO_CONST_32];

    uint32  ilen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32* olen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;

    /* PRQA S 0311 ++ */ /*VL_Crypto_62_General */
    const uint8* input = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    /* PRQA S 0311 -- */

    ret = Crypto_Sha256(input, ilen, output, is224);

    if (ret == E_OK)
    {
        if (*olen >= CRYPTO_CONST_32 && is224 == FALSE)
        {
            (void)IStdLib_MemCpy(
                Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr,
                output,
                CRYPTO_CONST_32);
            *olen = CRYPTO_CONST_32;
        }
        else if (*olen >= CRYPTO_CONST_28 && is224 == TRUE)
        {
            (void)IStdLib_MemCpy(
                Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr,
                output,
                CRYPTO_CONST_28);
            *olen = CRYPTO_CONST_28;
        }
        else
        {
            (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, *olen);
        }
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif /* CRYPTO_SHA256_C */

/* PRQA S 0488,0314,1252,1253,1255,1277,1281,1290,1338,1840,1841,1842,1843 -- */
/* PRQA S 1863,2001,2015,2109,2216,3120,3218,3226,3232,3326,3387,3440,3472 -- */
/* PRQA S 3473,3678,4115,4542,4544,4558,2889,2985,2743 -- */
/* PRQA S 2784,1532   -- */
