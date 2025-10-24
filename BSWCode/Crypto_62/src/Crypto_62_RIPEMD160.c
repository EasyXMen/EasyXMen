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
 **  @file               : Crypto_62_Ripemd160.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/
/* PRQA S 0553 EOF */ /* VL_BswM_0553 */
/* =================================================== inclusions =================================================== */

/* PRQA S 0488,4559,4544,3493,3446,3432,3226,3138,3120,2741,2216,1840,1841 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1842,1252,0842,0314,1253,1290,1338 ++ */                               /* VL_QAC_Crypto */

#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_RIPEMD160 == STD_ON)
#include "Crypto_62_Ripemd160.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
/* ===================================================== macros ===================================================== */
#define STATE_0 0x67452301U
#define STATE_1 0xEFCDAB89U
#define STATE_2 0x98BADCFEU
#define STATE_3 0x10325476U
#define STATE_4 0xC3D2E1F0U

#define BIT_MASK_32 0xFFFFFFFFU
#define ROTATE_MASK 0x3FU

#define KP_1 0x50A28BE6U
#define KP_2 0x5C4DD124U
#define KP_3 0x6D703EF3U
#define KP_4 0x7A6D76E9U
#define KP_5 0x00000000U

#define K_1 0x00000000U
#define K_2 0x5A827999U
#define K_3 0x6ED9EBA1U
#define K_4 0x8F1BBCDCU
#define K_5 0xA953FD4EU

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
CRYPTO_62_LOCAL void
    Crypto_Internal_Ripemd160_process(Crypto_Ripemd160_Context* ctx, const uint8 data[CRYPTO_CONST_64]);
/* ============================================ internal data definitions =========================================== */
/* PRQA S 3218 ++ */ /* VL_QAC_Crypto */
CRYPTO_62_LOCAL const uint8 Ripemd160_padding[CRYPTO_CONST_64] = {
    0x80U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
/* PRQA S 3218 -- */
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               RIPEMD-160 process data block (internal use only).

 * Param-Name[in]      data: buffer holding one block of data.
 * Param-Name[in/out]  ctx: RIPEMD-160 context.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_Internal_Ripemd160_process(Crypto_Ripemd160_Context* ctx, const uint8 data[CRYPTO_CONST_64])
{
    struct
    {
        uint32 A, B, C, D, E, Ap, Bp, Cp, Dp, Ep, X[CRYPTO_CONST_16];
    } local;

    local.X[0]  = CRYPTO_GET_UINT32_LE(data, 0);
    local.X[1]  = CRYPTO_GET_UINT32_LE(data, 4);
    local.X[2]  = CRYPTO_GET_UINT32_LE(data, 8);
    local.X[3]  = CRYPTO_GET_UINT32_LE(data, 12);
    local.X[4]  = CRYPTO_GET_UINT32_LE(data, 16);
    local.X[5]  = CRYPTO_GET_UINT32_LE(data, 20);
    local.X[6]  = CRYPTO_GET_UINT32_LE(data, 24);
    local.X[7]  = CRYPTO_GET_UINT32_LE(data, 28);
    local.X[8]  = CRYPTO_GET_UINT32_LE(data, 32);
    local.X[9]  = CRYPTO_GET_UINT32_LE(data, 36);
    local.X[10] = CRYPTO_GET_UINT32_LE(data, 40);
    local.X[11] = CRYPTO_GET_UINT32_LE(data, 44);
    local.X[12] = CRYPTO_GET_UINT32_LE(data, 48);
    local.X[13] = CRYPTO_GET_UINT32_LE(data, 52);
    local.X[14] = CRYPTO_GET_UINT32_LE(data, 56);
    local.X[15] = CRYPTO_GET_UINT32_LE(data, 60);

    local.E  = ctx->state[4];
    local.Ep = ctx->state[4];
    local.D  = ctx->state[3];
    local.Dp = ctx->state[3];
    local.C  = ctx->state[2];
    local.Cp = ctx->state[2];
    local.B  = ctx->state[1];
    local.Bp = ctx->state[1];
    local.A  = ctx->state[0];
    local.Ap = ctx->state[0];

#define Fp F5
#define Kp KP_1
#define F  F1
#define K  K_1
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 0, 11, 5, 8);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 1, 14, 14, 9);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 2, 15, 7, 9);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 3, 12, 0, 11);

    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 4, 5, 9, 13);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 5, 8, 2, 15);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 6, 7, 11, 15);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 7, 9, 4, 5);

    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 8, 11, 13, 7);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 9, 13, 6, 7);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 10, 14, 15, 8);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 11, 15, 8, 11);

    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 12, 6, 1, 14);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 13, 7, 10, 14);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 14, 9, 3, 12);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 15, 8, 12, 6);
#undef Fp
#undef K
#undef Kp
#undef F

#define Fp F4
#define Kp KP_2
#define F  F2
#define K  K_2
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 7, 7, 6, 9);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 4, 6, 11, 13);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 13, 8, 3, 15);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 1, 13, 7, 7);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 10, 11, 0, 12);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 6, 9, 13, 8);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 15, 7, 5, 9);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 3, 15, 10, 11);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 12, 7, 14, 7);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 0, 12, 15, 7);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 9, 15, 8, 12);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 5, 9, 12, 7);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 2, 11, 4, 6);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 14, 7, 9, 15);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 11, 13, 1, 13);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 8, 12, 2, 11);
#undef F
#undef Kp
#undef K
#undef Fp

#define F  F3
#define K  K_3
#define Fp F3
#define Kp KP_3
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 3, 11, 15, 9);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 10, 13, 5, 7);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 14, 6, 1, 15);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 4, 7, 3, 11);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 9, 14, 7, 8);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 15, 9, 14, 6);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 8, 13, 6, 6);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 1, 15, 9, 14);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 2, 14, 11, 12);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 7, 8, 8, 13);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 0, 13, 12, 5);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 6, 6, 2, 14);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 13, 5, 10, 13);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 11, 12, 0, 13);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 5, 7, 4, 7);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 12, 5, 13, 5);
#undef F
#undef Kp
#undef K
#undef Fp

#define F  F4
#define K  K_4
#define Fp F2
#define Kp KP_4
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 1, 11, 8, 15);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 9, 12, 6, 5);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 11, 14, 4, 8);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 10, 15, 1, 11);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 0, 14, 3, 14);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 8, 15, 11, 14);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 12, 9, 15, 6);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 4, 8, 0, 14);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 13, 9, 5, 6);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 3, 14, 12, 9);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 7, 5, 2, 12);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 15, 6, 13, 9);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 14, 8, 9, 12);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 5, 6, 7, 5);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 6, 5, 10, 15);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 2, 12, 14, 8);
#undef F
#undef Kp
#undef K
#undef Fp

#define F  F5
#define K  K_5
#define Fp F1
#define Kp KP_5
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 4, 9, 12, 8);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 0, 15, 15, 5);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 5, 5, 10, 12);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 9, 11, 4, 9);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 7, 6, 1, 12);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 12, 8, 5, 5);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 2, 13, 8, 14);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 10, 12, 7, 6);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 14, 5, 6, 8);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 1, 12, 2, 13);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 3, 13, 13, 6);
    ROUND2_STEP(local.A, local.B, local.C, local.D, local.E, 8, 14, 14, 5);
    ROUND2_STEP(local.E, local.A, local.B, local.C, local.D, 11, 11, 0, 15);
    ROUND2_STEP(local.D, local.E, local.A, local.B, local.C, 6, 8, 3, 13);
    ROUND2_STEP(local.C, local.D, local.E, local.A, local.B, 15, 5, 9, 11);
    ROUND2_STEP(local.B, local.C, local.D, local.E, local.A, 13, 6, 11, 11);
#undef K
#undef F
#undef Kp
#undef Fp

    local.C       = ctx->state[1] + local.Dp + local.C;
    ctx->state[4] = ctx->state[0] + local.Cp + local.B;
    ctx->state[3] = ctx->state[4] + local.Bp + local.A;
    ctx->state[2] = ctx->state[3] + local.Ap + local.E;
    ctx->state[1] = ctx->state[2] + local.Ep + local.D;
    ctx->state[0] = local.C;
}

/******************************************************************************/
/*
 * Brief               RIPEMD-160 process buffer.

 * Param-Name[in]      input: buffer holding the data.
                       ilen: length of the input data.
 * Param-Name[in/out]  ctx: RIPEMD-160 context.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_Ripemd160_Update(Crypto_Ripemd160_Context* ctx, const uint8* input, uint32 ilen)
{
    uint32 pad;
    uint32 leave;

    if (ilen > 0)
    {
        leave = ctx->total[0] & ROTATE_MASK;
        pad   = 64 - leave;

        ctx->total[0] = ctx->total[0] + (uint32)ilen;
        ctx->total[0] = ctx->total[0] & BIT_MASK_32;

        if (ctx->total[0] < (uint32)ilen)
        {
            ctx->total[1]++;
        }

        /* PRQA S 4112,4115,4505,4558,3397,3400 ++ */ /* VL_QAC_Crypto */
        if (leave && ilen >= pad)
        /* PRQA S 4112,4115,4505,4558,3397,3400 -- */
        {
            (void)IStdLib_MemCpy((void*)(ctx->buffer + leave), input, pad);

            Crypto_Internal_Ripemd160_process(ctx, ctx->buffer);

            input += pad;
            ilen -= pad;
            leave = 0;
        }

        for (; ilen >= 64; ilen -= 64, input += 64)
        {
            Crypto_Internal_Ripemd160_process(ctx, input);
        }

        if (ilen > 0)
        {
            (void)IStdLib_MemCpy((void*)(ctx->buffer + leave), input, ilen);
        }
    }
}

/******************************************************************************/
/*
 * Brief               Output = RIPEMD-160( input buffer ).

 * Param-Name[in]      input: buffer holding the data.
                       ilen: length of the input data.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: RIPEMD-160 checksum result.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Ripemd160(const uint8* input, uint32 ilen, uint8 output[CRYPTO_CONST_20])
{
    Crypto_Ripemd160_Context ctx;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Ripemd160_Context));

    Crypto_Ripemd160_Starts(&ctx);

    Crypto_Ripemd160_Update(&ctx, input, ilen);

    Crypto_Ripemd160_Finish(&ctx, output);

    return E_OK;
}

/******************************************************************************/
/*
 * Brief               This function process the Ripemd160 and ouput the result.

 * Param-Name[in]      objectId: objectId configured in crypto.

 * Param-Name[in/out]  None
 * Param-Name[out]     None.
 * Return              Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Ripemd160_Process(uint32 objectId)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          output[CRYPTO_CONST_20];

    uint32 ilen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    if (*(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr) >= 20U)
    {
        /* PRQA S 0311,3678 ++ */ /*VL_QAC_0311 */
        uint8* input = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
        /* PRQA S 0311,3678 -- */

        ret = Crypto_Ripemd160(input, ilen, output);
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, 20u);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               RIPEMD-160 final digest.

 * Param-Name[in]      ctx: RIPEMD-160 context.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: RIPEMD-160 checksum result.
 * Return              None
 */
/******************************************************************************/
void Crypto_Ripemd160_Finish(Crypto_Ripemd160_Context* ctx, uint8 output[CRYPTO_CONST_20])
{
    uint32 last;
    uint32 padNum;
    uint32 high;
    uint32 total0_high;
    uint32 total1_low;
    uint32 low;
    uint8  msglen[CRYPTO_CONST_8];

    total0_high = ctx->total[0] >> 29;
    total1_low  = ctx->total[1] << 3;
    high        = total0_high | total1_low;
    low         = (ctx->total[0] << 3);

    CRYPTO_PUT_UINT32_LE(low, msglen, 0);
    CRYPTO_PUT_UINT32_LE(high, msglen, 4);

    last = ctx->total[0] & ROTATE_MASK;
    if (last < 56)
    {
        padNum = 56 - last;
    }
    else
    {
        padNum = 120 - last;
    }

    Crypto_Ripemd160_Update(ctx, Ripemd160_padding, padNum);
    Crypto_Ripemd160_Update(ctx, msglen, 8);

    CRYPTO_PUT_UINT32_LE(ctx->state[0], output, 0);
    CRYPTO_PUT_UINT32_LE(ctx->state[1], output, 4);
    CRYPTO_PUT_UINT32_LE(ctx->state[2], output, 8);
    CRYPTO_PUT_UINT32_LE(ctx->state[3], output, 12);
    CRYPTO_PUT_UINT32_LE(ctx->state[4], output, 16);
}

/******************************************************************************/
/*
 * Brief               RIPEMD-160 context setup.

 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: RIPEMD-160 context.
 * Return              None
 */
/******************************************************************************/
void Crypto_Ripemd160_Starts(Crypto_Ripemd160_Context* ctx)
{
    ctx->state[0] = STATE_0;
    ctx->state[1] = STATE_1;
    ctx->state[2] = STATE_2;
    ctx->state[3] = STATE_3;
    ctx->state[4] = STATE_4;
    ctx->total[0] = 0;
    ctx->total[1] = 0;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif
/* PRQA S 0488,4559,4544,3493,3446,3432,3226,3138,3120,2741,2216,1840,1841 -- */
/* PRQA S 1842,1252,0842,0314,1253,1290,1338 -- */
