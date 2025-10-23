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
 **  @file               : Crypto_62_Sha3.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 0311,0488,1253,1259,1277,1280,1290,1338,1821,1840,1841,1842,1843 EOF*/ /*VL_QAC_0311 */
/* PRQA S 1852,1860,1862,1863,2001,2015,3120,3132,3218,3226,3332,3344,3397 EOF*/ /*VL_QAC_0311 */
/* PRQA S 3410,3432,3440,3446,3450,3454,3472,3473,3493,4434,4436,4461,4532 EOF*/ /*VL_QAC_0311 */
/* PRQA S 4533,4534,4542,4543,4544,4559,4603,1861,2016,3326,3387,0553 EOF*/      /*VL_QAC_0311 */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"

#if (CRYPTO_ALGORITHMFAM_SHA3 == STD_ON)
#include "Crypto_62_Sha3.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== internal function definitions ========================================= */
CRYPTO_62_LOCAL void keccak_f1600(Crypto_Sha3_Context* ctx);
/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               This function finishes the SHA-3 operation, and writes
 *                     the result to the output buffer.
 *
 * Param-Name[in]      ctx: The SHA-3 context. This must be initialized
 *                          and have a hash operation started.
 *                     olen: Defines the length of output buffer (in bytes). For
 *                           SHA-3 224, SHA-3 256, SHA-3 384 and SHA-3 512 olen
 *                           must equal to 28, 32, 48 and 64, respectively.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-3 checksum result. This must be a writable
 *                             buffer of length olen bytes.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha3_Finish(Crypto_Sha3_Context* ctx, uint8* output, uint32 olen)
{
    Std_ReturnType ret = E_OK;

    if (ctx->olen > 0)
    {
        if (ctx->olen > olen)
        {
            return E_NOT_OK;
        }
        olen = ctx->olen;
    }

    ABSORB(ctx, ctx->index, XOR_BYTE);
    ABSORB(ctx, ctx->max_block_size - 1, 0x80);
    keccak_f1600(ctx);
    ctx->index = 0;

    for (; olen > 0; olen--)
    {
        *output++  = SQUEEZE(ctx, ctx->index);
        ctx->index = (ctx->index + 1) % ctx->max_block_size;
        if (ctx->index == 0)
        {
            keccak_f1600(ctx);
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function feeds an input buffer into an ongoing SHA-3
 *                     checksum calculation.
 *
 * Param-Name[in]      input: The buffer holding the data. This must be a readable
 *                            buffer of length ilen Bytes.
 *                     ilen: The length of the input data in Bytes.
 * Param-Name[in/out]  ctx: The SHA-3 context. This must be initialized
 *                          and have a hash operation started.
 * Param-Name[out]     None
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha3_Update(Crypto_Sha3_Context* ctx, const uint8* input, uint32 ilen)
{
    Std_ReturnType align_bytes;
    if (ilen >= 8)
    {
        align_bytes = 8 - (ctx->index % 8);
        if (align_bytes)
        {
            for (; align_bytes > 0; align_bytes--)
            {
                ABSORB(ctx, ctx->index, *input++);
                ilen--;
                ctx->index++;
            }
            ctx->index = ctx->index % ctx->max_block_size;
            if (ctx->index == 0)
            {
                keccak_f1600(ctx);
            }
        }

        while (ilen >= 8)
        {
            ABSORB(ctx, ctx->index, CRYPTO_GET_UINT64_LE(input, 0));
            input += 8;
            ilen -= 8;
            ctx->index = (ctx->index + 8) % ctx->max_block_size;
            if (ctx->index == 0)
            {
                keccak_f1600(ctx);
            }
        }
    }

    while (ilen-- > 0)
    {
        ABSORB(ctx, ctx->index, *input++);
        ctx->index = (ctx->index + 1) % ctx->max_block_size;
        if (ctx->index == 0)
        {
            keccak_f1600(ctx);
        }
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               This function starts a SHA-3 checksum calculation.
 * Param-Name[in]      id: The id of the SHA-3 family.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The context to use.
 * Return              None
 */
/******************************************************************************/
void Crypto_Sha3_Starts(Crypto_Sha3_Context* ctx, Crypto_Sha3_Id Sha3_Alg_id)
{
    switch (Sha3_Alg_id)
    {
    case CRYPTO_SHA3_512:
        ctx->olen           = 512 / 8;
        ctx->max_block_size = 576 / 8;
        break;
    case CRYPTO_SHA3_384:
        ctx->olen           = 384 / 8;
        ctx->max_block_size = 832 / 8;
        break;
    case CRYPTO_SHA3_256:
        ctx->olen           = 256 / 8;
        ctx->max_block_size = 1088 / 8;
        break;
    case CRYPTO_SHA3_224:
        ctx->olen           = 224 / 8;
        ctx->max_block_size = 1152 / 8;
        break;

    default:
        /*do nothing*/
        break;
    }

    (void)IStdLib_MemSet(ctx->state, 0, sizeof(ctx->state));
    ctx->index = 0;
}

/******************************************************************************/
/*
 * Brief               The permutation function.
 * Param-Name[in]      None
 * Param-Name[in/out]  ctx: Sha3 context.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void keccak_f1600(Crypto_Sha3_Context* ctx)
{
    uint64         lane[5];
    uint64*        s = ctx->state;
    Std_ReturnType i, j;

    for (Std_ReturnType round = 0; round < 24; round++)
    {
        uint64 t;
        uint32 p;
        uint8  r8;
        uint32 r;

#if CRYPTO_SHA3_THETA_UNROLL != 0
        lane[4] = s[4] ^ s[9] ^ s[14] ^ s[19] ^ s[24];
        lane[3] = s[3] ^ s[8] ^ s[13] ^ s[18] ^ s[23];
        lane[2] = s[2] ^ s[7] ^ s[12] ^ s[17] ^ s[22];
        lane[1] = s[1] ^ s[6] ^ s[11] ^ s[16] ^ s[21];
        lane[0] = s[0] ^ s[5] ^ s[10] ^ s[15] ^ s[20];

        t = lane[4] ^ ROTR64(lane[1], 63);
        s[20] ^= t;
        s[15] ^= t;
        s[10] ^= t;
        s[5] ^= t;
        s[0] ^= t;

        t = lane[0] ^ ROTR64(lane[2], 63);
        s[21] ^= t;
        s[16] ^= t;
        s[11] ^= t;
        s[6] ^= t;
        s[1] ^= t;

        t = lane[1] ^ ROTR64(lane[3], 63);
        s[22] ^= t;
        s[17] ^= t;
        s[12] ^= t;
        s[7] ^= t;
        s[2] ^= t;

        t = lane[2] ^ ROTR64(lane[4], 63);
        s[23] ^= t;
        s[18] ^= t;
        s[13] ^= t;
        s[8] ^= t;
        s[3] ^= t;

        t = lane[3] ^ ROTR64(lane[0], 63);
        s[24] ^= t;
        s[19] ^= t;
        s[14] ^= t;
        s[9] ^= t;
        s[4] ^= t;
#else
        for (i = 0; i < 5; i++)
        {
            lane[i] = s[i] ^ s[i + 5] ^ s[i + 10] ^ s[i + 15] ^ s[i + 20];
        }
        for (i = 0; i < 5; i++)
        {
            t = lane[(i + 4) % 5] ^ ROTR64(lane[(i + 1) % 5], 63);
            s[i] ^= t;
            s[i + 5] ^= t;
            s[i + 10] ^= t;
            s[i + 15] ^= t;
            s[i + 20] ^= t;
        }
#endif

        /* Rho */
        for (i = 1; i < 25; i += 4)
        {
            r = rho[(i - 1) >> 2];
#if CRYPTO_SHA3_RHO_UNROLL != 0
            s[i + 0] = ROTR64(s[i + 0], MBEDTLS_BYTE_3(r));
            s[i + 1] = ROTR64(s[i + 1], MBEDTLS_BYTE_2(r));
            s[i + 2] = ROTR64(s[i + 2], MBEDTLS_BYTE_1(r));
            s[i + 3] = ROTR64(s[i + 3], MBEDTLS_BYTE_0(r));
#else
            for (j = i; j < i + 4; j++)
            {
                r8 = (uint8)(r >> 24);
                r <<= 8;
                s[j] = ROTR64(s[j], r8);
            }
#endif
        }

        /* Pi */
        t = s[1];
#if CRYPTO_SHA3_PI_UNROLL != 0
        p = pi[0];
        SWAP(s[MBEDTLS_BYTE_0(p)], t);
        SWAP(s[MBEDTLS_BYTE_1(p)], t);
        SWAP(s[MBEDTLS_BYTE_2(p)], t);
        SWAP(s[MBEDTLS_BYTE_3(p)], t);
        p = pi[1];
        SWAP(s[MBEDTLS_BYTE_0(p)], t);
        SWAP(s[MBEDTLS_BYTE_1(p)], t);
        SWAP(s[MBEDTLS_BYTE_2(p)], t);
        SWAP(s[MBEDTLS_BYTE_3(p)], t);
        p = pi[2];
        SWAP(s[MBEDTLS_BYTE_0(p)], t);
        SWAP(s[MBEDTLS_BYTE_1(p)], t);
        SWAP(s[MBEDTLS_BYTE_2(p)], t);
        SWAP(s[MBEDTLS_BYTE_3(p)], t);
        p = pi[3];
        SWAP(s[MBEDTLS_BYTE_0(p)], t);
        SWAP(s[MBEDTLS_BYTE_1(p)], t);
        SWAP(s[MBEDTLS_BYTE_2(p)], t);
        SWAP(s[MBEDTLS_BYTE_3(p)], t);
        p = pi[4];
        SWAP(s[MBEDTLS_BYTE_0(p)], t);
        SWAP(s[MBEDTLS_BYTE_1(p)], t);
        SWAP(s[MBEDTLS_BYTE_2(p)], t);
        SWAP(s[MBEDTLS_BYTE_3(p)], t);
        p = pi[5];
        SWAP(s[MBEDTLS_BYTE_0(p)], t);
        SWAP(s[MBEDTLS_BYTE_1(p)], t);
        SWAP(s[MBEDTLS_BYTE_2(p)], t);
        SWAP(s[MBEDTLS_BYTE_3(p)], t);

#else
        for (i = 0; i < 24; i += 4)
        {
            p = pi[i >> 2];
            for (unsigned j = 0; j < 4; j++)
            {
                SWAP(s[p & 0xff], t);
                p >>= 8;
            }
        }
#endif

        /* Chi */
#if CRYPTO_SHA3_CHI_UNROLL != 0
        lane[0] = s[0];
        lane[1] = s[1];
        lane[2] = s[2];
        lane[3] = s[3];
        lane[4] = s[4];
        s[4] ^= (~lane[0]) & lane[1];
        s[3] ^= (~lane[4]) & lane[0];
        s[2] ^= (~lane[3]) & lane[4];
        s[1] ^= (~lane[2]) & lane[3];
        s[0] ^= (~lane[1]) & lane[2];

        lane[0] = s[5];
        lane[1] = s[6];
        lane[2] = s[7];
        lane[3] = s[8];
        lane[4] = s[9];
        s[9] ^= (~lane[0]) & lane[1];
        s[8] ^= (~lane[4]) & lane[0];
        s[7] ^= (~lane[3]) & lane[4];
        s[6] ^= (~lane[2]) & lane[3];
        s[5] ^= (~lane[1]) & lane[2];

        lane[0] = s[10];
        lane[1] = s[11];
        lane[2] = s[12];
        lane[3] = s[13];
        lane[4] = s[14];
        s[14] ^= (~lane[0]) & lane[1];
        s[13] ^= (~lane[4]) & lane[0];
        s[12] ^= (~lane[3]) & lane[4];
        s[11] ^= (~lane[2]) & lane[3];
        s[10] ^= (~lane[1]) & lane[2];

        lane[0] = s[15];
        lane[1] = s[16];
        lane[2] = s[17];
        lane[3] = s[18];
        lane[4] = s[19];
        s[19] ^= (~lane[0]) & lane[1];
        s[18] ^= (~lane[4]) & lane[0];
        s[17] ^= (~lane[3]) & lane[4];
        s[16] ^= (~lane[2]) & lane[3];
        s[15] ^= (~lane[1]) & lane[2];

        lane[0] = s[20];
        lane[1] = s[21];
        lane[2] = s[22];
        lane[3] = s[23];
        lane[4] = s[24];
        s[24] ^= (~lane[0]) & lane[1];
        s[23] ^= (~lane[4]) & lane[0];
        s[22] ^= (~lane[3]) & lane[4];
        s[21] ^= (~lane[2]) & lane[3];
        s[20] ^= (~lane[1]) & lane[2];

#else
        for (i = 0; i <= 20; i += 5)
        {
            lane[0] = s[i];
            lane[1] = s[i + 1];
            lane[2] = s[i + 2];
            lane[3] = s[i + 3];
            lane[4] = s[i + 4];
            s[i + 4] ^= (~lane[0]) & lane[1];
            s[i + 3] ^= (~lane[4]) & lane[0];
            s[i + 2] ^= (~lane[3]) & lane[4];
            s[i + 1] ^= (~lane[2]) & lane[3];
            s[i + 0] ^= (~lane[1]) & lane[2];
        }
#endif

        s[0] ^=
            ((iota_r_packed[round] & 0x40ull) << 57 | (iota_r_packed[round] & 0x20ull) << 26
             | (iota_r_packed[round] & 0x10ull) << 11 | (iota_r_packed[round] & 0x8f));
    }
}

/******************************************************************************/
/*
 * Brief               This function calculates the SHA-3 checksum of a buffer.
 *                     The function allocates the context, performs the calculation,
 *                     and frees the context. The SHA-3 result is calculated as
 *                     output = SHA-3(id, input buffer, d).
 *
 * Param-Name[in]      id: The id of the SHA-3 family.
 *                     input: The buffer holding the data. This must be a readable
 *                            buffer of length \p ilen Bytes.
 *                     ilen: The length of the input data in Bytes.
 *                     olen: Defines the length of output buffer (in bytes). For
 *                           SHA-3 224, SHA-3 256, SHA-3 384 and SHA-3 512 olen
 *                           must equal to 28, 32, 48 and 64, respectively.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The SHA-3 checksum result. This must be a writable
 *                             buffer of length \c olen bytes.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha3(Crypto_Sha3_Id id, const uint8* input, uint32 ilen, uint8* output, uint32 olen)
{
    Std_ReturnType      ret = E_NOT_OK;
    Crypto_Sha3_Context ctx;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Sha3_Context));

    Crypto_Sha3_Starts(&ctx, id);

    ret = Crypto_Sha3_Update(&ctx, input, ilen);

    if (ret == 0)
    {
        ret = Crypto_Sha3_Finish(&ctx, output, olen);
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function calculates the SHA-3 checksum and output result.
 *
 * Param-Name[in]      objectId: The object ID of the job.
 *                     Id: The id of the SHA-3 family.
 * Param-Name[in/out]  None
 * Param-Name[out]     None.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sha3_Process(uint32 objectId, Crypto_Sha3_Id Id)
{
    Std_ReturnType ret    = E_NOT_OK;
    const uint8*   input  = (uint8*)Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint8*         output = (uint8*)Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32         ilen   = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32         olen   = *Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;

    ret = Crypto_Sha3(Id, input, ilen, output, olen);
    (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, olen);
    return ret;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif
