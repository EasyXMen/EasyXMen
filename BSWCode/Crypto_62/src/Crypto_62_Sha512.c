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
 **  @file               : Crypto_62_Sha512.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/
/* PRQA S 0488,0842,1252,1253,1290,1840,1842,1860,1862,2741 ++ */ /* VL_QAC_Crypto */
/* PRQA S 0314,0684,1338,1863,1881,2001,2015,2109,2216,3120 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3138,3218,3326,3387,3397,3400,3440,3446,3472,3473 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3493,3678,4115,4116,4393,4397,4461,4544,4558,4559 ++ */ /* VL_QAC_Crypto */
/* PRQA S 0342,1505,2755,2743,2880,2889,2784,2995,2991,1532 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040   ++ */              /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)

#include "Crypto_62_Sha512.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ===================================================== macros ===================================================== */
#define SHA512_BLOCK_SIZE 128
#define UL64(x)           x##ULL
#if defined(CRYPTO_SHA512_SMALLER)
CRYPTO_62_LOCAL void CRYPTO_PUT_UINT64_BE(uint64 n, unsigned char* b, uint8_t i)
{
    CRYPTO_PUT_UINT64_BE(n, b, i);
}
#else
#endif

/* ========================================== internal function declarations ======================================== */
CRYPTO_62_LOCAL Std_ReturnType
                       Crypto_Internal_Sha512_Process(Crypto_Sha512_Context* ctx, const uint8 data[SHA512_BLOCK_SIZE]);
CRYPTO_62_LOCAL uint32 Crypto_Internal_Sha512_Process_Many(Crypto_Sha512_Context* ctx, const uint8* data, uint32 len);

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               This function starts a Sha512_224 checksum calculation.

 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The SHA-512 context to use. This must be initialized.
 * Return              None
 */
/******************************************************************************/
void Crypto_Sha512_224_Starts(Crypto_Sha512_Context* ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = UL64(0x8c3d37c819544da2);
    ctx->state[1] = UL64(0x73e1996689dcd4d6);
    ctx->state[2] = UL64(0x1dfab7ae32ff9c82);
    ctx->state[3] = UL64(0x679dd514582f9fcf);
    ctx->state[4] = UL64(0x0f6d2b697bd44da8);
    ctx->state[5] = UL64(0x77e36f7304c48942);
    ctx->state[6] = UL64(0x3f9d85a86a1d36c8);
    ctx->state[7] = UL64(0x1112e6ad91d692a1);
}

/******************************************************************************/
/*
 * Brief               This function starts a Sha512_256 checksum calculation.

 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The SHA-512 context to use. This must be initialized.
 * Return              None
 */
/******************************************************************************/
void Crypto_Sha512_256_Starts(Crypto_Sha512_Context* ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = UL64(0x22312194fc2bf72c);
    ctx->state[1] = UL64(0x9f555fa3c84c64c2);
    ctx->state[2] = UL64(0x2393b86b6f53b151);
    ctx->state[3] = UL64(0x963877195940eabd);
    ctx->state[4] = UL64(0x96283ee2a88effe3);
    ctx->state[5] = UL64(0xbe5e1e2553863992);
    ctx->state[6] = UL64(0x2b0199fc2c85b8aa);
    ctx->state[7] = UL64(0x0eb72ddc81c52ca2);
}

/******************************************************************************/
/*
 * Brief               This function starts a SHA-384 or SHA-512 checksum calculation.

 * Param-Name[in]      is384: Determines which function to use. This must be either
                              0 for SHA-512, or 1 for SHA-384.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The SHA-512 context to use. This must be initialized.
 * Return              None
 */
/******************************************************************************/
void Crypto_Sha512_Starts(Crypto_Sha512_Context* ctx, boolean is384)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    if (is384 != 0)
    {
        ctx->state[0] = UL64(0xCBBB9D5DC1059ED8);
        ctx->state[1] = UL64(0x629A292A367CD507);
        ctx->state[2] = UL64(0x9159015A3070DD17);
        ctx->state[3] = UL64(0x152FECD8F70E5939);
        ctx->state[4] = UL64(0x67332667FFC00B31);
        ctx->state[5] = UL64(0x8EB44A8768581511);
        ctx->state[6] = UL64(0xDB0C2E0D64F98FA7);
        ctx->state[7] = UL64(0x47B5481DBEFA4FA4);
    }
    else
    {
        ctx->state[0] = UL64(0x6A09E667F3BCC908);
        ctx->state[1] = UL64(0xBB67AE8584CAA73B);
        ctx->state[2] = UL64(0x3C6EF372FE94F82B);
        ctx->state[3] = UL64(0xA54FF53A5F1D36F1);
        ctx->state[4] = UL64(0x510E527FADE682D1);
        ctx->state[5] = UL64(0x9B05688C2B3E6C1F);
        ctx->state[6] = UL64(0x1F83D9ABFB41BD6B);
        ctx->state[7] = UL64(0x5BE0CD19137E2179);
    }
}

/******************************************************************************/
/*
 * Brief               This function processes a single data block within the
                       ongoing SHA-512 computation. This function is for internal
                       use only.

 * Param-Name[in]      data: The buffer holding one block of data. This must be a
                             readable buffer of length 128 Bytes.
 * Param-Name[in/out]  ctx: The SHA-512 context. This must be initialized.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType
    Crypto_Internal_Sha512_Process(Crypto_Sha512_Context* ctx, const uint8 data[SHA512_BLOCK_SIZE])
{
    uint8 i;
    struct
    {
        uint64 temp1, temp2, W[CRYPTO_CONST_80];
        uint64 A[CRYPTO_CONST_8];
    } local;

#define SHR(x, n)  ((x) >> (n))
#define ROTR(x, n) (SHR((x), (n)) | ((x) << (CRYPTO_CONST_64 - (n))))

#define S0(x) (ROTR(x, CRYPTO_CONST_1) ^ ROTR(x, CRYPTO_CONST_8) ^ SHR(x, CRYPTO_CONST_7))
#define S1(x) (ROTR(x, CRYPTO_CONST_19) ^ ROTR(x, CRYPTO_CONST_61) ^ SHR(x, CRYPTO_CONST_6))

#define S2(x) (ROTR(x, CRYPTO_CONST_28) ^ ROTR(x, CRYPTO_CONST_34) ^ ROTR(x, CRYPTO_CONST_39))
#define S3(x) (ROTR(x, CRYPTO_CONST_14) ^ ROTR(x, CRYPTO_CONST_18) ^ ROTR(x, CRYPTO_CONST_41))

#define F0(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

#define P(a, b, c, d, e, f, g, h, x, K)                            \
    do                                                             \
    {                                                              \
        local.temp1 = (h) + S3(e) + F1((e), (f), (g)) + (K) + (x); \
        local.temp2 = S2(a) + F0((a), (b), (c));                   \
        (d) += local.temp1;                                        \
        (h) = local.temp1 + local.temp2;                           \
    } while (0)

    for (i = 0; i < 8; i++)
    {
        local.A[i] = ctx->state[i];
    }

    for (i = 0; i < 16; i++)
    {
        local.W[i] = CRYPTO_GET_UINT64_BE(data, i << 3);
    }

    for (; i < 80; i++)
    {
        local.W[i] = S1(local.W[i - 2]) + local.W[i - 7] + S0(local.W[i - 15]) + local.W[i - 16];
    }

    i = 0;
    do
    {
        P(local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.W[i],
          K[i]);
        i++;
        P(local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.W[i],
          K[i]);
        i++;
        P(local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.W[i],
          K[i]);
        i++;
        P(local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.W[i],
          K[i]);
        i++;
        P(local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.W[i],
          K[i]);
        i++;
        P(local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.W[i],
          K[i]);
        i++;
        P(local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.W[i],
          K[i]);
        i++;
        P(local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.W[i],
          K[i]);
        i++;
    } while (i < 80);

    for (i = 0; i < 8; i++)
    {
        ctx->state[i] += local.A[i];
    }

    return E_OK;
}

/******************************************************************************/
/*
 * Brief               This function feeds an input buffer into an ongoing SHA-512
 *                     checksum calculation.
 *
 * Param-Name[in]      input: The buffer holding the input data. This must be a
 *                            readable buffer of length ilen Bytes.
 *                     ilen: The length of the input data in Bytes.
 * Param-Name[in/out]  ctx: The SHA-512 context. This must be initialized.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha512_Update(Crypto_Sha512_Context* ctx, const unsigned char* input, uint32 ilen)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32         fill;
    Std_ReturnType left;

    if (ilen == 0U)
    {
        return 0;
    }

    left = (Std_ReturnType)(ctx->total[0] & 0x7FU);
    fill = (uint32)(SHA512_BLOCK_SIZE - left);

    ctx->total[0] += (uint64)ilen;

    if (ctx->total[0] < (uint64)ilen)
    {
        ctx->total[1]++;
    }

    if (left && ilen >= fill)
    {
        (void)IStdLib_MemCpy((void*)(ctx->buffer + left), input, fill);

        if ((ret = Crypto_Internal_Sha512_Process(ctx, ctx->buffer)) != E_OK)
        {
            return ret;
        }

        input += fill;
        ilen -= fill;
        left = 0;
    }

    while (ilen >= SHA512_BLOCK_SIZE)
    {
        uint32 processed = Crypto_Internal_Sha512_Process_Many(ctx, input, ilen);
        if (processed < SHA512_BLOCK_SIZE)
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

    return E_OK;
}

/******************************************************************************/
/*
 * Brief               This function finishes the SHA-512 operation, and writes
 *                     the result to the output buffer.
 *
 * Param-Name[in]      ctx: The SHA-512 context. This must be initialized and
 *                          have a hash operation started.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-384 or SHA-512 checksum result. This must
 *                             be a writable buffer of length 64 bytes for SHA-512,
 *                             48 bytes for SHA-384.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha512_Finish(Crypto_Sha512_Context* ctx, unsigned char* output)
{
    Std_ReturnType ret = E_NOT_OK;
    unsigned       used;
    uint64         high, low;
    Std_ReturnType truncated = 0;

    used = ctx->total[0] & 0x7FU;

    ctx->buffer[used++] = 0x80U;

    if (used <= 112)
    {
        (void)IStdLib_MemSet(ctx->buffer + used, 0, 112 - used);
    }
    else
    {
        (void)IStdLib_MemSet(ctx->buffer + used, 0, SHA512_BLOCK_SIZE - used);

        if ((ret = Crypto_Internal_Sha512_Process(ctx, ctx->buffer)) != 0)
        {
            goto exit;
        }

        (void)IStdLib_MemSet(ctx->buffer, 0, 112u);
    }

    high = (ctx->total[0] >> 61) | (ctx->total[1] << 3);
    low  = (ctx->total[0] << 3);

    CRYPTO_PUT_UINT64_BE(high, ctx->buffer, 112);
    CRYPTO_PUT_UINT64_BE(low, ctx->buffer, 120);

    if ((ret = Crypto_Internal_Sha512_Process(ctx, ctx->buffer)) != 0)
    {
        goto exit;
    }

    CRYPTO_PUT_UINT64_BE(ctx->state[0], output, 0);
    CRYPTO_PUT_UINT64_BE(ctx->state[1], output, 8);
    CRYPTO_PUT_UINT64_BE(ctx->state[2], output, 16);
    CRYPTO_PUT_UINT64_BE(ctx->state[3], output, 24);
    CRYPTO_PUT_UINT64_BE(ctx->state[4], output, 32);
    CRYPTO_PUT_UINT64_BE(ctx->state[5], output, 40);

#if defined(CRYPTO_SHA384_C)
    truncated = ctx->is384;
#endif
    if (!truncated)
    {
        CRYPTO_PUT_UINT64_BE(ctx->state[6], output, 48);
        CRYPTO_PUT_UINT64_BE(ctx->state[7], output, 56);
    }

    ret = E_OK;

exit:
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function divides data into groups and processes one
 *                     group each time until all groups are processed.
 *
 * Param-Name[in]      data: Data to be processed.
 *                     len: The length of data.
 * Param-Name[in/out]  ctx: The SHA-512 context. This must be initialized.
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
CRYPTO_62_LOCAL uint32 Crypto_Internal_Sha512_Process_Many(Crypto_Sha512_Context* ctx, const uint8* data, uint32 len)
{
    uint32 processed = 0;

    while (len >= SHA512_BLOCK_SIZE)
    {
        if (Crypto_Internal_Sha512_Process(ctx, data) != 0)
        {
            return 0;
        }

        data += SHA512_BLOCK_SIZE;
        len -= SHA512_BLOCK_SIZE;

        processed += SHA512_BLOCK_SIZE;
    }

    return processed;
}

/******************************************************************************/
/*
 * Brief               This function calculates the SHA-512 or SHA-384 checksum
 *                     of a buffer.
 *
 * Param-Name[in]      input: The buffer holding the input data. This must be
 *                            a readable buffer of length \p ilen Bytes.
 *                     ilen: The length of the input data in Bytes.
 *                     is384: Determines which function to use. This must be either
 *                            0 for SHA-512, or 1 for SHA-384.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-384 or SHA-512 checksum result. This must
 *                             be a writable buffer of length 64 bytes for SHA-512,
 *                             48 bytes for SHA-384.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha512(const unsigned char* input, uint32 ilen, unsigned char* output, boolean is384)
{
    Std_ReturnType        ret = E_NOT_OK;
    Crypto_Sha512_Context ctx;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Sha512_Context));
    Crypto_Sha512_Starts(&ctx, is384);
    ret = Crypto_Sha512_Update(&ctx, input, ilen);
    if (ret == E_OK)
    {
        ret = Crypto_Sha512_Finish(&ctx, output);
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function calculates the SHA-3 checksum and output result.
 *
 * Param-Name[in]      objectId: The object ID of the job.
 *                     is384: the length of sha512 is 384 or not .
 * Param-Name[in/out]  None
 * Param-Name[out]     None.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha512_Process(uint32 objectId, boolean is384)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          output[CRYPTO_CONST_64];

    uint32 ilen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    if (*(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr) >= 64u)
    {
        /* PRQA S 0311 ++ */ /*VL_QAC_0311 */
        uint8* input = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
        /* PRQA S 0311 -- */

        ret = Crypto_Sha512(input, ilen, output, is384);
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, 64u);
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function calculates the SHA-512_224.
 *
 * Param-Name[in]      input: The buffer holding the input data. This must be
 *                            a readable buffer of length ilen Bytes.
 *                     ilen: The length of the input data in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-512_224 checksum result.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_sha512_224(const unsigned char* input, uint32 ilen, unsigned char* output)
{
    Std_ReturnType        ret = E_NOT_OK;
    Crypto_Sha512_Context ctx;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Sha512_Context));
    Crypto_Sha512_224_Starts(&ctx);
    ret = Crypto_Sha512_Update(&ctx, input, ilen);
    if (ret == E_OK)
    {
        ret = Crypto_Sha512_Finish(&ctx, output);
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function calculates the SHA-512_224.
 *
 * Param-Name[in]      objectId: The object ID of the job.
 *                     Id: The id of the SHA-3 family.
 * Param-Name[in/out]  None
 * Param-Name[out]     None.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha512_224_Process(uint32 objectId)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          output[CRYPTO_CONST_64];

    uint32 ilen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    if (*(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr) >= 64u)
    {
        /* PRQA S 0311 ++ */ /*VL_QAC_0311 */
        uint8* input = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
        /* PRQA S 0311 -- */

        ret = Crypto_sha512_224(input, ilen, output);
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, 64u);
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function calculates the SHA-512_256.
 *
 * Param-Name[in]      input: The buffer holding the input data. This must be
 *                            a readable buffer of length ilen Bytes.
 *                     ilen: The length of the input data in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-512_224 checksum result.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_sha512_256(const unsigned char* input, uint32 ilen, unsigned char* output)
{
    Std_ReturnType        ret = E_NOT_OK;
    Crypto_Sha512_Context ctx;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Sha512_Context));
    Crypto_Sha512_256_Starts(&ctx);
    ret = Crypto_Sha512_Update(&ctx, input, ilen);
    if (ret == E_OK)
    {
        ret = Crypto_Sha512_Finish(&ctx, output);
    }

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function calculates the SHA512-256 checksum and output result.
 *
 * Param-Name[in]      objectId: The object ID of the job.
 *                     Id: The id of the SHA-3 family.
 * Param-Name[in/out]  None
 * Param-Name[out]     None.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha512_256_Process(uint32 objectId)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          output[CRYPTO_CONST_64];

    uint32 ilen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32 olen = *Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    if (olen <= 64u)
    {
        /* PRQA S 0311 ++ */ /*VL_QAC_0311 */
        uint8* input = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
        /* PRQA S 0311 -- */

        ret = Crypto_sha512_256(input, ilen, output);
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, olen);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif /* CRYPTO_SHA512_C */

/* PRQA S 0488,0842,1252,1253,1290,1840,1842,1860,1862,2741 -- */
/* PRQA S 0314,0684,1338,1863,1881,2001,2015,2109,2216,3120 -- */
/* PRQA S 3138,3218,3326,3387,3397,3400,3440,3446,3472,3473 -- */
/* PRQA S 3493,3678,4115,4116,4393,4397,4461,4544,4558,4559 -- */
/* PRQA S 0342,1505,2755,2743,2880,2889,2784,2995,2991,1532 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040   -- */
