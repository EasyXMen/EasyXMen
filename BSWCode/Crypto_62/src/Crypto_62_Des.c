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
 **  @file               : Crypto_62_Des.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"

/* PRQA S 4549,4544,4542,0488,1252,1253,3387,1841,3473,3440,1338 ++ */ /* VL_Crypto_62_General */
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

#include "Crypto_62_Des.h"

/* ===================================================== macros ===================================================== */
/* PRQA S 3472++ */ /* VL_Crypto_62_General */

/*
 * Initial Permutation macro
 */
#define DES_IP(X, Y)                                                              \
    do                                                                            \
    {                                                                             \
        T = (((X) >> CRYPTO_CONST_4) ^ (Y)) & 0x0F0F0F0FU;                        \
        (Y) ^= T;                                                                 \
        (X) ^= (T << CRYPTO_CONST_4);                                             \
        T = (((X) >> CRYPTO_CONST_16) ^ (Y)) & 0x0000FFFFU;                       \
        (Y) ^= T;                                                                 \
        (X) ^= (T << CRYPTO_CONST_16);                                            \
        T = (((Y) >> CRYPTO_CONST_2) ^ (X)) & 0x33333333U;                        \
        (X) ^= T;                                                                 \
        (Y) ^= (T << CRYPTO_CONST_2);                                             \
        T = (((Y) >> CRYPTO_CONST_8) ^ (X)) & 0x00FF00FFU;                        \
        (X) ^= T;                                                                 \
        (Y) ^= (T << CRYPTO_CONST_8);                                             \
        (Y) = (((Y) << CRYPTO_CONST_1) | ((Y) >> CRYPTO_CONST_31)) & 0xFFFFFFFFU; \
        T   = ((X) ^ (Y)) & 0xAAAAAAAAU;                                          \
        (Y) ^= T;                                                                 \
        (X) ^= T;                                                                 \
        (X) = (((X) << CRYPTO_CONST_1) | ((X) >> CRYPTO_CONST_31)) & 0xFFFFFFFFU; \
    } while (0)

/*
 * Final Permutation macro
 */
#define DES_FP(X, Y)                                                              \
    do                                                                            \
    {                                                                             \
        (X) = (((X) << CRYPTO_CONST_31) | ((X) >> CRYPTO_CONST_1)) & 0xFFFFFFFFU; \
        T   = ((X) ^ (Y)) & 0xAAAAAAAAU;                                          \
        (X) ^= T;                                                                 \
        (Y) ^= T;                                                                 \
        (Y) = (((Y) << CRYPTO_CONST_31) | ((Y) >> CRYPTO_CONST_1)) & 0xFFFFFFFFU; \
        T   = (((Y) >> CRYPTO_CONST_8) ^ (X)) & 0x00FF00FFU;                      \
        (X) ^= T;                                                                 \
        (Y) ^= (T << CRYPTO_CONST_8);                                             \
        T = (((Y) >> CRYPTO_CONST_2) ^ (X)) & 0x33333333U;                        \
        (X) ^= T;                                                                 \
        (Y) ^= (T << CRYPTO_CONST_2);                                             \
        T = (((X) >> CRYPTO_CONST_16) ^ (Y)) & 0x0000FFFFU;                       \
        (Y) ^= T;                                                                 \
        (X) ^= (T << CRYPTO_CONST_16);                                            \
        T = (((X) >> CRYPTO_CONST_4) ^ (Y)) & 0x0F0F0F0FU;                        \
        (Y) ^= T;                                                                 \
        (X) ^= (T << CRYPTO_CONST_4);                                             \
    } while (0)

/*
 * DES round macro
 */
#define DES_ROUND(X, Y)                                                                                             \
    do                                                                                                              \
    {                                                                                                               \
        T = *SK++ ^ (X);                                                                                            \
        (Y) ^= SB8[(T) & CRYPTO_CONST_0x3F] ^ SB6[(T >> CRYPTO_CONST_8) & CRYPTO_CONST_0x3F]                        \
               ^ SB4[(T >> CRYPTO_CONST_16) & CRYPTO_CONST_0x3F] ^ SB2[(T >> CRYPTO_CONST_24) & CRYPTO_CONST_0x3F]; \
                                                                                                                    \
        T = *SK++ ^ (((X) << CRYPTO_CONST_28) | ((X) >> CRYPTO_CONST_4));                                           \
        (Y) ^= SB7[(T) & CRYPTO_CONST_0x3F] ^ SB5[(T >> CRYPTO_CONST_8) & CRYPTO_CONST_0x3F]                        \
               ^ SB3[(T >> CRYPTO_CONST_16) & CRYPTO_CONST_0x3F] ^ SB1[(T >> CRYPTO_CONST_24) & CRYPTO_CONST_0x3F]; \
    } while (0)

#define SWAP(a, b)      \
    do                  \
    {                   \
        uint32 t = (a); \
        (a)      = (b); \
        (b)      = t;   \
        t        = 0;   \
    } while (0)
/* PRQA S 3472-- */
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* PRQA S 3218 ++ */ /* VL_Crypto_62_General */
CRYPTO_62_LOCAL const uint32 SB1[CRYPTO_CONST_64] = {
    0x01010400U, 0x00000000U, 0x00010000U, 0x01010404U, 0x01010004U, 0x00010404U, 0x00000004U, 0x00010000U,
    0x00000400U, 0x01010400U, 0x01010404U, 0x00000400U, 0x01000404U, 0x01010004U, 0x01000000U, 0x00000004U,
    0x00000404U, 0x01000400U, 0x01000400U, 0x00010400U, 0x00010400U, 0x01010000U, 0x01010000U, 0x01000404U,
    0x00010004U, 0x01000004U, 0x01000004U, 0x00010004U, 0x00000000U, 0x00000404U, 0x00010404U, 0x01000000U,
    0x00010000U, 0x01010404U, 0x00000004U, 0x01010000U, 0x01010400U, 0x01000000U, 0x01000000U, 0x00000400U,
    0x01010004U, 0x00010000U, 0x00010400U, 0x01000004U, 0x00000400U, 0x00000004U, 0x01000404U, 0x00010404U,
    0x01010404U, 0x00010004U, 0x01010000U, 0x01000404U, 0x01000004U, 0x00000404U, 0x00010404U, 0x01010400U,
    0x00000404U, 0x01000400U, 0x01000400U, 0x00000000U, 0x00010004U, 0x00010400U, 0x00000000U, 0x01010004U};
CRYPTO_62_LOCAL const uint32 SB2[CRYPTO_CONST_64] = {
    0x80108020U, 0x80008000U, 0x00008000U, 0x00108020U, 0x00100000U, 0x00000020U, 0x80100020U, 0x80008020U,
    0x80000020U, 0x80108020U, 0x80108000U, 0x80000000U, 0x80008000U, 0x00100000U, 0x00000020U, 0x80100020U,
    0x00108000U, 0x00100020U, 0x80008020U, 0x00000000U, 0x80000000U, 0x00008000U, 0x00108020U, 0x80100000U,
    0x00100020U, 0x80000020U, 0x00000000U, 0x00108000U, 0x00008020U, 0x80108000U, 0x80100000U, 0x00008020U,
    0x00000000U, 0x00108020U, 0x80100020U, 0x00100000U, 0x80008020U, 0x80100000U, 0x80108000U, 0x00008000U,
    0x80100000U, 0x80008000U, 0x00000020U, 0x80108020U, 0x00108020U, 0x00000020U, 0x00008000U, 0x80000000U,
    0x00008020U, 0x80108000U, 0x00100000U, 0x80000020U, 0x00100020U, 0x80008020U, 0x80000020U, 0x00100020U,
    0x00108000U, 0x00000000U, 0x80008000U, 0x00008020U, 0x80000000U, 0x80100020U, 0x80108020U, 0x00108000U};

CRYPTO_62_LOCAL const uint32 SB3[CRYPTO_CONST_64] = {
    0x00000208U, 0x08020200U, 0x00000000U, 0x08020008U, 0x08000200U, 0x00000000U, 0x00020208U, 0x08000200U,
    0x00020008U, 0x08000008U, 0x08000008U, 0x00020000U, 0x08020208U, 0x00020008U, 0x08020000U, 0x00000208U,
    0x08000000U, 0x00000008U, 0x08020200U, 0x00000200U, 0x00020200U, 0x08020000U, 0x08020008U, 0x00020208U,
    0x08000208U, 0x00020200U, 0x00020000U, 0x08000208U, 0x00000008U, 0x08020208U, 0x00000200U, 0x08000000U,
    0x08020200U, 0x08000000U, 0x00020008U, 0x00000208U, 0x00020000U, 0x08020200U, 0x08000200U, 0x00000000U,
    0x00000200U, 0x00020008U, 0x08020208U, 0x08000200U, 0x08000008U, 0x00000200U, 0x00000000U, 0x08020008U,
    0x08000208U, 0x00020000U, 0x08000000U, 0x08020208U, 0x00000008U, 0x00020208U, 0x00020200U, 0x08000008U,
    0x08020000U, 0x08000208U, 0x00000208U, 0x08020000U, 0x00020208U, 0x00000008U, 0x08020008U, 0x00020200U};

CRYPTO_62_LOCAL const uint32 SB4[CRYPTO_CONST_64] = {
    0x00802001U, 0x00002081U, 0x00002081U, 0x00000080U, 0x00802080U, 0x00800081U, 0x00800001U, 0x00002001U,
    0x00000000U, 0x00802000U, 0x00802000U, 0x00802081U, 0x00000081U, 0x00000000U, 0x00800080U, 0x00800001U,
    0x00000001U, 0x00002000U, 0x00800000U, 0x00802001U, 0x00000080U, 0x00800000U, 0x00002001U, 0x00002080U,
    0x00800081U, 0x00000001U, 0x00002080U, 0x00800080U, 0x00002000U, 0x00802080U, 0x00802081U, 0x00000081U,
    0x00800080U, 0x00800001U, 0x00802000U, 0x00802081U, 0x00000081U, 0x00000000U, 0x00000000U, 0x00802000U,
    0x00002080U, 0x00800080U, 0x00800081U, 0x00000001U, 0x00802001U, 0x00002081U, 0x00002081U, 0x00000080U,
    0x00802081U, 0x00000081U, 0x00000001U, 0x00002000U, 0x00800001U, 0x00002001U, 0x00802080U, 0x00800081U,
    0x00002001U, 0x00002080U, 0x00800000U, 0x00802001U, 0x00000080U, 0x00800000U, 0x00002000U, 0x00802080U};
CRYPTO_62_LOCAL const uint32 SB5[CRYPTO_CONST_64] = {
    0x00000100U, 0x02080100U, 0x02080000U, 0x42000100U, 0x00080000U, 0x00000100U, 0x40000000U, 0x02080000U,
    0x40080100U, 0x00080000U, 0x02000100U, 0x40080100U, 0x42000100U, 0x42080000U, 0x00080100U, 0x40000000U,
    0x02000000U, 0x40080000U, 0x40080000U, 0x00000000U, 0x40000100U, 0x42080100U, 0x42080100U, 0x02000100U,
    0x42080000U, 0x40000100U, 0x00000000U, 0x42000000U, 0x02080100U, 0x02000000U, 0x42000000U, 0x00080100U,
    0x00080000U, 0x42000100U, 0x00000100U, 0x02000000U, 0x40000000U, 0x02080000U, 0x42000100U, 0x40080100U,
    0x02000100U, 0x40000000U, 0x42080000U, 0x02080100U, 0x40080100U, 0x00000100U, 0x02000000U, 0x42080000U,
    0x42080100U, 0x00080100U, 0x42000000U, 0x42080100U, 0x02080000U, 0x00000000U, 0x40080000U, 0x42000000U,
    0x00080100U, 0x02000100U, 0x40000100U, 0x00080000U, 0x00000000U, 0x40080000U, 0x02080100U, 0x40000100U};
CRYPTO_62_LOCAL const uint32 SB6[CRYPTO_CONST_64] = {
    0x20000010U, 0x20400000U, 0x00004000U, 0x20404010U, 0x20400000U, 0x00000010U, 0x20404010U, 0x00400000U,
    0x20004000U, 0x00404010U, 0x00400000U, 0x20000010U, 0x00400010U, 0x20004000U, 0x20000000U, 0x00004010U,
    0x00000000U, 0x00400010U, 0x20004010U, 0x00004000U, 0x00404000U, 0x20004010U, 0x00000010U, 0x20400010U,
    0x20400010U, 0x00000000U, 0x00404010U, 0x20404000U, 0x00004010U, 0x00404000U, 0x20404000U, 0x20000000U,
    0x20004000U, 0x00000010U, 0x20400010U, 0x00404000U, 0x20404010U, 0x00400000U, 0x00004010U, 0x20000010U,
    0x00400000U, 0x20004000U, 0x20000000U, 0x00004010U, 0x20000010U, 0x20404010U, 0x00404000U, 0x20400000U,
    0x00404010U, 0x20404000U, 0x00000000U, 0x20400010U, 0x00000010U, 0x00004000U, 0x20400000U, 0x00404010U,
    0x00004000U, 0x00400010U, 0x20004010U, 0x00000000U, 0x20404000U, 0x20000000U, 0x00400010U, 0x20004010U};

CRYPTO_62_LOCAL const uint32 SB7[CRYPTO_CONST_64] = {
    0x00200000U, 0x04200002U, 0x04000802U, 0x00000000U, 0x00000800U, 0x04000802U, 0x00200802U, 0x04200800U,
    0x04200802U, 0x00200000U, 0x00000000U, 0x04000002U, 0x00000002U, 0x04000000U, 0x04200002U, 0x00000802U,
    0x04000800U, 0x00200802U, 0x00200002U, 0x04000800U, 0x04000002U, 0x04200000U, 0x04200800U, 0x00200002U,
    0x04200000U, 0x00000800U, 0x00000802U, 0x04200802U, 0x00200800U, 0x00000002U, 0x04000000U, 0x00200800U,
    0x04000000U, 0x00200800U, 0x00200000U, 0x04000802U, 0x04000802U, 0x04200002U, 0x04200002U, 0x00000002U,
    0x00200002U, 0x04000000U, 0x04000800U, 0x00200000U, 0x04200800U, 0x00000802U, 0x00200802U, 0x04200800U,
    0x00000802U, 0x04000002U, 0x04200802U, 0x04200000U, 0x00200800U, 0x00000000U, 0x00000002U, 0x04200802U,
    0x00000000U, 0x00200802U, 0x04200000U, 0x00000800U, 0x04000002U, 0x04000800U, 0x00000800U, 0x00200002U};

CRYPTO_62_LOCAL const uint32 SB8[CRYPTO_CONST_64] = {
    0x10001040U, 0x00001000U, 0x00040000U, 0x10041040U, 0x10000000U, 0x10001040U, 0x00000040U, 0x10000000U,
    0x00040040U, 0x10040000U, 0x10041040U, 0x00041000U, 0x10041000U, 0x00041040U, 0x00001000U, 0x00000040U,
    0x10040000U, 0x10000040U, 0x10001000U, 0x00001040U, 0x00041000U, 0x00040040U, 0x10040040U, 0x10041000U,
    0x00001040U, 0x00000000U, 0x00000000U, 0x10040040U, 0x10000040U, 0x10001000U, 0x00041040U, 0x00040000U,
    0x00041040U, 0x00040000U, 0x10041000U, 0x00001000U, 0x00000040U, 0x10040040U, 0x00001000U, 0x00041040U,
    0x10001000U, 0x00000040U, 0x10000040U, 0x10040000U, 0x10040040U, 0x10000000U, 0x00040000U, 0x10001040U,
    0x00000000U, 0x10041040U, 0x00040040U, 0x10000040U, 0x10040000U, 0x10001000U, 0x10001040U, 0x00000000U,
    0x10041040U, 0x00041000U, 0x00041000U, 0x00001040U, 0x00001040U, 0x00040040U, 0x10000000U, 0x10041000U};
/*
 * PC1: left and right halves bit-swap
 */
CRYPTO_62_LOCAL const uint32 LHs[CRYPTO_CONST_16] = {
    0x00000000U,
    0x00000001U,
    0x00000100U,
    0x00000101U,
    0x00010000U,
    0x00010001U,
    0x00010100U,
    0x00010101U,
    0x01000000U,
    0x01000001U,
    0x01000100U,
    0x01000101U,
    0x01010000U,
    0x01010001U,
    0x01010100U,
    0x01010101U};

CRYPTO_62_LOCAL const uint32 RHs[CRYPTO_CONST_16] = {
    0x00000000U,
    0x01000000U,
    0x00010000U,
    0x01010000U,
    0x00000100U,
    0x01000100U,
    0x00010100U,
    0x01010100U,
    0x00000001U,
    0x01000001U,
    0x00010001U,
    0x01010001U,
    0x00000101U,
    0x01000101U,
    0x00010101U,
    0x01010101U};
/* PRQA S 3218 -- */
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               Perform 3Des in ECB mode.
 *
 * Param-Name[in]      ctx: The context for 3Des.
 *                     input: The input message.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: Buffer stored the output tag.
 * Return              None
 */
/******************************************************************************/
/* PRQA S 3673++ */ /* VL_Crypto_62_General */
void Crypto_3Des_Crypt_Ecb(Crypto_Des3Data* ctx, const uint8 input[CRYPTO_CONST_8], uint8 output[CRYPTO_CONST_8])
/* PRQA S 3673-- */
{
    Std_ReturnType i;
    uint32         X, Y, T, *SK; /* PRQA S 3678 */ /* VL_Crypto_62_General */

    SK = ctx->sk;
    /* PRQA S 3493,3446,4559 ++ */ /* VL_Crypto_62_General */
    X = CRYPTO_GET_UINT32_BE(input, CRYPTO_CONST_0);
    Y = CRYPTO_GET_UINT32_BE(input, CRYPTO_CONST_4);
    /* PRQA S 3493,3446,4559 -- */

    DES_IP(X, Y);

    for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_8; i++)
    {
        DES_ROUND(Y, X);
        DES_ROUND(X, Y);
    }

    for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_8; i++)
    {
        DES_ROUND(X, Y);
        DES_ROUND(Y, X);
    }

    for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_8; i++)
    {
        DES_ROUND(Y, X);
        DES_ROUND(X, Y);
    }

    DES_FP(Y, X);
    /* PRQA S 3138 ++ */ /* VL_Crypto_62_General */
    CRYPTO_PUT_UINT32_BE(Y, output, CRYPTO_CONST_0);
    CRYPTO_PUT_UINT32_BE(X, output, CRYPTO_CONST_4);
    /* PRQA S 3138 -- */
}

/******************************************************************************/
/*
 * Brief               Internal function for key expansion.
 *
 * Param-Name[in]      key: Base key.
 * Param-Name[in/out]  None
 * Param-Name[out]     SK: Round keys.
 * Return              None
 */
/******************************************************************************/
void Crypto_Des_Setkey(uint32 SK[CRYPTO_CONST_32], const uint8 key[CRYPTO_DES_KEY_SIZE])
{
    uint32 i;
    uint32 X, Y, T;
    /* PRQA S 3493,3446,4559++ */ /* VL_Crypto_62_General */
    X = CRYPTO_GET_UINT32_BE(key, 0);
    Y = CRYPTO_GET_UINT32_BE(key, CRYPTO_CONST_4);
    /* PRQA S 3493,3446,4559-- */

    /*
     * Permuted Choice 1
     */
    /* PRQA S 3120++ */ /* VL_Crypto_62_General */
    T = ((Y >> CRYPTO_CONST_4) ^ X) & 0x0F0F0F0FU;
    X ^= T;
    Y ^= (T << CRYPTO_CONST_4);
    T = ((Y) ^ X) & 0x10101010U;
    X ^= T;
    Y ^= (T);

    X = (LHs[(X)&CRYPTO_CONST_0x0F] << CRYPTO_CONST_3)
        | (LHs[(X >> CRYPTO_CONST_8) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_2)
        | (LHs[(X >> CRYPTO_CONST_16) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_1)
        | (LHs[(X >> CRYPTO_CONST_24) & CRYPTO_CONST_0x0F])
        | (LHs[(X >> CRYPTO_CONST_5) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_7)
        | (LHs[(X >> CRYPTO_CONST_13) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_6)
        | (LHs[(X >> CRYPTO_CONST_21) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_5)
        | (LHs[(X >> CRYPTO_CONST_29) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_4);

    Y = (RHs[(Y >> CRYPTO_CONST_1) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_3)
        | (RHs[(Y >> CRYPTO_CONST_9) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_2)
        | (RHs[(Y >> CRYPTO_CONST_17) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_1)
        | (RHs[(Y >> CRYPTO_CONST_25) & CRYPTO_CONST_0x0F])
        | (RHs[(Y >> CRYPTO_CONST_4) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_7)
        | (RHs[(Y >> CRYPTO_CONST_12) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_6)
        | (RHs[(Y >> CRYPTO_CONST_20) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_5)
        | (RHs[(Y >> CRYPTO_CONST_28) & CRYPTO_CONST_0x0F] << CRYPTO_CONST_4);

    X &= CRYPTO_CONST_0x0FFFFFFF;
    Y &= CRYPTO_CONST_0x0FFFFFFF;

    /*
     * calculate subkeys
     */
    for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_16; i++)
    {
        if ((i < CRYPTO_CONST_2) || (i == CRYPTO_CONST_8) || (i == CRYPTO_CONST_15))
        {
            X = ((X << 1) | (X >> CRYPTO_CONST_27)) & CRYPTO_CONST_0x0FFFFFFF;
            Y = ((Y << 1) | (Y >> CRYPTO_CONST_27)) & CRYPTO_CONST_0x0FFFFFFF;
        }
        else
        {
            X = ((X << CRYPTO_CONST_2) | (X >> CRYPTO_CONST_26)) & CRYPTO_CONST_0x0FFFFFFF;
            Y = ((Y << CRYPTO_CONST_2) | (Y >> CRYPTO_CONST_26)) & CRYPTO_CONST_0x0FFFFFFF;
        }

        *SK++ = ((X << CRYPTO_CONST_4) & 0x24000000U) | ((X << 28) & 0x10000000U) | ((X << 14) & 0x08000000U)
                | ((X << 18) & 0x02080000U) | ((X << 6) & 0x01000000U) | ((X << 9) & 0x00200000U)
                | ((X >> 1) & 0x00100000U) | ((X << 10) & 0x00040000U) | ((X << 2) & 0x00020000U)
                | ((X >> 10) & 0x00010000U) | ((Y >> 13) & 0x00002000U) | ((Y >> 4) & 0x00001000U)
                | ((Y << 6) & 0x00000800U) | ((Y >> 1) & 0x00000400U) | ((Y >> 14) & 0x00000200U) | ((Y) & 0x00000100U)
                | ((Y >> 5) & 0x00000020U) | ((Y >> 10) & 0x00000010U) | ((Y >> 3) & 0x00000008U)
                | ((Y >> 18) & 0x00000004U) | ((Y >> 26) & 0x00000002U) | ((Y >> 24) & 0x00000001U);

        *SK++ = ((X << CRYPTO_CONST_15) & 0x20000000U) | ((X << 17) & 0x10000000U) | ((X << 10) & 0x08000000U)
                | ((X << 22) & 0x04000000U) | ((X >> 2) & 0x02000000U) | ((X << 1) & 0x01000000U)
                | ((X << 16) & 0x00200000U) | ((X << 11) & 0x00100000U) | ((X << 3) & 0x00080000U)
                | ((X >> 6) & 0x00040000U) | ((X << 15) & 0x00020000U) | ((X >> 4) & 0x00010000U)
                | ((Y >> 2) & 0x00002000U) | ((Y << 8) & 0x00001000U) | ((Y >> 14) & 0x00000808U)
                | ((Y >> 9) & 0x00000400U) | ((Y) & 0x00000200U) | ((Y << 7) & 0x00000100U) | ((Y >> 7) & 0x00000020U)
                | ((Y >> 3) & 0x00000011U) | ((Y << 2) & 0x00000004U) | ((Y >> 21) & 0x00000002U);
    }
}
/* PRQA S 3120-- */
/******************************************************************************/
/*
 * Brief               DES key schedule (56-bit, encryption).
 *
 * Param-Name[in]      key: 8-byte secret key.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The context for 3Des.
 * Return              Std_ReturnType:  0: State accepted
 *                           Not 0: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Des_Setkey_Enc(Crypto_DesData* ctx, const unsigned char key[CRYPTO_DES_KEY_SIZE])
{
    Crypto_Des_Setkey(ctx->sk, key);
    return E_OK;
}

/******************************************************************************/
/*
 * Brief               DES key schedule (56-bit, decryption).
 *
 * Param-Name[in]      key: 8-byte secret key.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The context for 3Des.
 * Return              Std_ReturnType:  0: State accepted
 *                           Not 0: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Des_Setkey_Dec(Crypto_DesData* ctx, const unsigned char key[CRYPTO_DES_KEY_SIZE])
{
    uint32 i;

    Crypto_Des_Setkey(ctx->sk, key);

    for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_16; i += CRYPTO_CONST_2)
    {
        /* PRQA S 1290,1840++ */ /* VL_Crypto_62_General */
        SWAP(ctx->sk[i], ctx->sk[CRYPTO_CONST_30 - i]);
        SWAP(ctx->sk[i + 1], ctx->sk[CRYPTO_CONST_31 - i]);
        /* PRQA S 1290,1840-- */
    }

    return E_OK;
}

void Crypto_Des3_Set2key(
    uint32              esk[CRYPTO_CONST_96],
    uint32              dsk[CRYPTO_CONST_96],
    const unsigned char key[CRYPTO_CONST_16])
{
    int i;

    Crypto_Des_Setkey(esk, key);
    Crypto_Des_Setkey(dsk + 32, key + 8);

    for (i = 0; i < 32; i += 2)
    {
        dsk[i]     = esk[30 - i];
        dsk[i + 1] = esk[31 - i];

        esk[i + 32] = dsk[62 - i];
        esk[i + 33] = dsk[63 - i];

        esk[i + 64] = esk[i];
        esk[i + 65] = esk[i + 1];

        dsk[i + 64] = dsk[i];
        dsk[i + 65] = dsk[i + 1];
    }
}

/*
 * Triple-DES key schedule (112-bit, encryption)
 */
Std_ReturnType Crypto_Des3_Set2key_Enc(Crypto_Des3Data* ctx, const unsigned char key[CRYPTO_CONST_16])
{
    uint32 sk[96];

    Crypto_Des3_Set2key(ctx->sk, sk, key);

    return 0;
}

/*
 * Triple-DES key schedule (112-bit, decryption)
 */
Std_ReturnType Crypto_Des3_Set2key_Dec(Crypto_Des3Data* ctx, const unsigned char key[CRYPTO_CONST_16])
{
    uint32 sk[96];

    Crypto_Des3_Set2key(sk, ctx->sk, key);

    return 0;
}

/******************************************************************************/
/*
 * Brief               Inner function to set 168 bits key.
 *
 * Param-Name[in]      ctx: The context for 3Des.
 *                     dsk: auxiliary buffer.
 *                     key: 24-byte secret key.
 * Param-Name[in/out]  None
 * Param-Name[out]     esk: 3DES subkeys.
 * Return              None
 */
/******************************************************************************/
void Crypto_Des3_Set3key(
    uint32              esk[CRYPTO_CONST_96],
    uint32              dsk[CRYPTO_CONST_96],
    const unsigned char key[CRYPTO_CONST_24])
{
    uint32 i;

    Crypto_Des_Setkey(esk, key);
    Crypto_Des_Setkey(dsk + CRYPTO_CONST_32, key + CRYPTO_CONST_8);
    Crypto_Des_Setkey(esk + CRYPTO_CONST_64, key + CRYPTO_CONST_16);

    for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_32; i += CRYPTO_CONST_2)
    {
        dsk[i]     = esk[CRYPTO_CONST_94 - i];
        dsk[i + 1] = esk[CRYPTO_CONST_95 - i]; /* PRQA S 1840 */ /* VL_Crypto_62_General */

        esk[i + CRYPTO_CONST_32] = dsk[CRYPTO_CONST_62 - i];
        esk[i + CRYPTO_CONST_33] = dsk[CRYPTO_CONST_63 - i];

        dsk[i + CRYPTO_CONST_64] = esk[CRYPTO_CONST_30 - i];
        dsk[i + CRYPTO_CONST_65] = esk[CRYPTO_CONST_31 - i];
    }
}

/******************************************************************************/
/*
 * Brief               Triple-DES key schedule (168-bit, encryption).
 *
 * Param-Name[in]      key: 24-byte secret key.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The context for 3Des.
 * Return              void:
 */
/******************************************************************************/
void Crypto_Des3_Set3key_Enc(Crypto_Des3Data* ctx, const unsigned char key[CRYPTO_DES_KEY_SIZE * CRYPTO_CONST_3])
{
    uint32 sk[CRYPTO_CONST_96];

    Crypto_Des3_Set3key(ctx->sk, sk, key);
}

/******************************************************************************/
/*
 * Brief               Triple-DES key schedule (168-bit, decryption).
 *
 * Param-Name[in]      key: 24-byte secret key.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The context for 3Des.
 * Return              Std_ReturnType:  0: State accepted
 *                           Not 0: State not accepted
 */
/******************************************************************************/
Std_ReturnType
    Crypto_Des3_Set3key_Dec(Crypto_Des3Data* ctx, const unsigned char key[CRYPTO_DES_KEY_SIZE * CRYPTO_CONST_3])
{
    uint32 sk[CRYPTO_CONST_96];

    Crypto_Des3_Set3key(sk, ctx->sk, key);
    return E_OK;
}

/******************************************************************************/
/*
 * Brief               Perform 3Des in CBC mode.
 *
 * Param-Name[in]      ctx: The context for 3Des.
 *                     mode: Indication of doing encryption or decryption.
 *                     length: The length of input.
 *                     iv: The initialization vector.
 *                     input: The input message.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: Buffer stored the output tag.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_3Des_Crypt_Cbc(
    Crypto_Des3Data* ctx,
    uint8            mode,
    uint32           length,
    uint8            iv[CRYPTO_CONST_8],
    const uint8*     input,
    uint8*           output,
    uint32*          outputLength)
{
    Std_ReturnType ret = E_OK;
    uint8          temp[CRYPTO_CONST_8];
    uint8          i;
    if (length % CRYPTO_CONST_8) /* PRQA S 3344*/ /* VL_Crypto_62_General */
    {
        return E_NOT_OK;
    }

    if (mode == CRYPTO_MODE_ENCRYPT)
    {
        while (length > CRYPTO_CONST_0)
        {
            for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_8; i++)
            {
                output[i] = (uint8)(input[i] ^ iv[i]);
            }

            Crypto_3Des_Crypt_Ecb(ctx, output, output);

            (void)IStdLib_MemCpy(iv, output, CRYPTO_CONST_8);

            input += CRYPTO_CONST_8;
            output += CRYPTO_CONST_8;
            length -= CRYPTO_CONST_8;
            *outputLength += CRYPTO_CONST_8;
        }
    }
    else
    { /* DES_DECRYPT */
        while (length > CRYPTO_CONST_0)
        {
            (void)IStdLib_MemCpy(temp, input, CRYPTO_CONST_8);
            Crypto_3Des_Crypt_Ecb(ctx, input, output);

            for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_8; i++)
            {
                output[i] = (uint8)(output[i] ^ iv[i]);
            }

            (void)IStdLib_MemCpy(iv, temp, CRYPTO_CONST_8);

            input += CRYPTO_CONST_8;
            output += CRYPTO_CONST_8;
            length -= CRYPTO_CONST_8;
            *outputLength += CRYPTO_CONST_8;
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Do Cmac with 3Des as inner function.
 *
 * Param-Name[in]      ctx: The context for 3Des.
 *                     mode: Indication of doing encryption or decryption.
 *                     length: The length of input.
 *                     input: The input meaasge.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: Buffer stored the output tag.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType
    Crypto_3Des_Crypt_Cmac(Crypto_Des3Data* ctx, uint8 mode, uint32 length, const uint8* input, uint8* output)
{
    Std_ReturnType ret = E_OK;
    uint8          temp[CRYPTO_CONST_8];
    uint8          i;
    if (length % CRYPTO_CONST_8) /* PRQA S 3344*/ /* VL_Crypto_62_General */
    {
        return E_NOT_OK;
    }

    if (mode == CRYPTO_MODE_ENCRYPT)
    {
        while (length > CRYPTO_CONST_0)
        {
            for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_8; i++)
            {
                output[i] = input[i];
            }

            Crypto_3Des_Crypt_Ecb(ctx, output, output);

            input += CRYPTO_CONST_8;
            output += CRYPTO_CONST_8;
            length -= CRYPTO_CONST_8;
        }
    }
    else
    { /* DES_DECRYPT */
        while (length > CRYPTO_CONST_0)
        {
            (void)IStdLib_MemCpy(temp, input, CRYPTO_CONST_8);
            Crypto_3Des_Crypt_Ecb(ctx, input, output);

            for (i = CRYPTO_CONST_0; i < CRYPTO_CONST_8; i++)
            {
                output[i] = input[i];
            }

            input += CRYPTO_CONST_8;
            output += CRYPTO_CONST_8;
            length -= CRYPTO_CONST_8;
        }
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif
/* PRQA S 4549,4544,4542,0488,1252,1253,3387,1841,3473,3440,1338 -- */
