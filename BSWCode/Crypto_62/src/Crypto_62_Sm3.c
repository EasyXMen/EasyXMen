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
 **  @file               : Crypto_62_Sm3.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 0317,0488,0551,1252,1253,1255,1277,1281,1290,1330 EOF */ /* VL_QAC_Crypto */
/* PRQA S 1338,1840,3387,3397,3400,3410,3417,3426,3430,3432 EOF */ /* VL_QAC_Crypto */
/* PRQA S 3440,3472,3473,3673,4150,4542,4544,1841,1842,1843 EOF */ /* VL_QAC_Crypto */
/* PRQA S 2011,2741,3120,3132,3344,0553 EOF */                     /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"

#if (CRYPTO_ALGORITHMFAM_SM3 == STD_ON)

#include "Crypto_62_Sm3.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ===================================================== macros ===================================================== */
#define SM3_CBLOCK 64

#define SM3_DIGEST_LENGTH 32

#define HASH_MAKE_STRING(c, s)   \
    do                           \
    {                            \
        unsigned long ll;        \
        ll = (c)->A;             \
        (void)HOST_l2c(ll, (s)); \
        ll = (c)->B;             \
        (void)HOST_l2c(ll, (s)); \
        ll = (c)->C;             \
        (void)HOST_l2c(ll, (s)); \
        ll = (c)->D;             \
        (void)HOST_l2c(ll, (s)); \
        ll = (c)->E;             \
        (void)HOST_l2c(ll, (s)); \
        ll = (c)->F;             \
        (void)HOST_l2c(ll, (s)); \
        ll = (c)->G;             \
        (void)HOST_l2c(ll, (s)); \
        ll = (c)->H;             \
        (void)HOST_l2c(ll, (s)); \
    } while (0)
#define ROTATE(a, n) (((a) << (n)) | (((a) & 0xffffffff) >> (32 - (n))))
#ifndef P0
#define P0(X) (X ^ ROTATE(X, 9) ^ ROTATE(X, 17))
#endif
#ifndef P1
#define P1(X) (X ^ ROTATE(X, 15) ^ ROTATE(X, 23))
#endif

#define FF0(X, Y, Z) (X ^ Y ^ Z)
#define GG0(X, Y, Z) (X ^ Y ^ Z)

#define FF1(X, Y, Z) ((X & Y) | ((X | Y) & Z))
#define GG1(X, Y, Z) ((Z ^ (X & (Y ^ Z))))

#define EXPAND(W0, W7, W13, W3, W10) (P1(W0 ^ W7 ^ ROTATE(W13, 15)) ^ ROTATE(W3, 7) ^ W10)

#define RND(A, B, C, D, E, F, G, H, TJ, Wi, Wj, FF, GG)             \
    do                                                              \
    {                                                               \
        const uint32 A12    = ROTATE(A, 12);                        \
        const uint32 A12_SM = A12 + E + TJ;                         \
        const uint32 SS1    = ROTATE(A12_SM, 7);                    \
        const uint32 TT1    = FF(A, B, C) + D + (SS1 ^ A12) + (Wj); \
        const uint32 TT2    = GG(E, F, G) + H + SS1 + Wi;           \
        B                   = ROTATE(B, 9);                         \
        D                   = TT1;                                  \
        F                   = ROTATE(F, 19);                        \
        H                   = P0(TT2);                              \
    } while (0)

#define R1(A, B, C, D, E, F, G, H, TJ, Wi, Wj) RND(A, B, C, D, E, F, G, H, TJ, Wi, Wj, FF0, GG0)

#define R2(A, B, C, D, E, F, G, H, TJ, Wi, Wj) RND(A, B, C, D, E, F, G, H, TJ, Wi, Wj, FF1, GG1)

#define SM3_A 0x7380166fUL
#define SM3_B 0x4914b2b9UL
#define SM3_C 0x172442d7UL
#define SM3_D 0xda8a0600UL
#define SM3_E 0xa96f30bcUL
#define SM3_F 0x163138aaUL
#define SM3_G 0xe38dee4dUL
#define SM3_H 0xb0fb0e4eUL
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
CRYPTO_62_LOCAL void Crypto_Sm3_Block_Data_Order(Crypto_SM3_Context* c, const void* p, uint32 num);

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               Initialize the context of SM3, fill the context arguments
                       with constants.

 * Param-Name[in]      None
 * Param-Name[in/out]  ctx: The context of SM3.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_Sm3_Start(Crypto_SM3_Context* ctx)
{
    (void)IStdLib_MemSet(ctx, 0, sizeof(Crypto_SM3_Context));
    ctx->A = SM3_A;
    ctx->B = SM3_B;
    ctx->C = SM3_C;
    ctx->D = SM3_D;
    ctx->E = SM3_E;
    ctx->F = SM3_F;
    ctx->G = SM3_G;
    ctx->H = SM3_H;
}

/******************************************************************************/
/*
 * Brief               Do expansion and compression on groups iteratively,
                       without padding. Each group are 64 byte divided from input.

 * Param-Name[in]      p: The pointer to input data.
                       n: The number of 64 byte groups divided from input.
 * Param-Name[in/out]  ctx: The context of SM3.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_Sm3_Block_Data_Order(Crypto_SM3_Context* ctx, const void* p, uint32 num)
{
    const uint8*    data = p;
    register uint32 A, B, C, D, E, F, G, H;

    uint32 W00, W01, W02, W03, W04, W05, W06, W07, W08, W09, W10, W11, W12, W13, W14, W15;

    for (; num--;)
    {
        A = ctx->A;
        B = ctx->B;
        C = ctx->C;
        D = ctx->D;
        E = ctx->E;
        F = ctx->F;
        G = ctx->G;
        H = ctx->H;

        /*
         * We have to load all message bytes immediately since SM3 reads
         * them slightly out of order.
         */
        (void)HOST_c2l(data, W00);
        (void)HOST_c2l(data, W01);
        (void)HOST_c2l(data, W02);
        (void)HOST_c2l(data, W03);
        (void)HOST_c2l(data, W04);
        (void)HOST_c2l(data, W05);
        (void)HOST_c2l(data, W06);
        (void)HOST_c2l(data, W07);
        (void)HOST_c2l(data, W08);
        (void)HOST_c2l(data, W09);
        (void)HOST_c2l(data, W10);
        (void)HOST_c2l(data, W11);
        (void)HOST_c2l(data, W12);
        (void)HOST_c2l(data, W13);
        (void)HOST_c2l(data, W14);
        (void)HOST_c2l(data, W15);

        R1(A, B, C, D, E, F, G, H, 0x79CC4519, W00, W00 ^ W04);
        W00 = EXPAND(W00, W07, W13, W03, W10);
        R1(D, A, B, C, H, E, F, G, 0xF3988A32, W01, W01 ^ W05);
        W01 = EXPAND(W01, W08, W14, W04, W11);
        R1(C, D, A, B, G, H, E, F, 0xE7311465, W02, W02 ^ W06);
        W02 = EXPAND(W02, W09, W15, W05, W12);
        R1(B, C, D, A, F, G, H, E, 0xCE6228CB, W03, W03 ^ W07);
        W03 = EXPAND(W03, W10, W00, W06, W13);
        R1(A, B, C, D, E, F, G, H, 0x9CC45197, W04, W04 ^ W08);
        W04 = EXPAND(W04, W11, W01, W07, W14);
        R1(D, A, B, C, H, E, F, G, 0x3988A32F, W05, W05 ^ W09);
        W05 = EXPAND(W05, W12, W02, W08, W15);
        R1(C, D, A, B, G, H, E, F, 0x7311465E, W06, W06 ^ W10);
        W06 = EXPAND(W06, W13, W03, W09, W00);
        R1(B, C, D, A, F, G, H, E, 0xE6228CBC, W07, W07 ^ W11);
        W07 = EXPAND(W07, W14, W04, W10, W01);
        R1(A, B, C, D, E, F, G, H, 0xCC451979, W08, W08 ^ W12);
        W08 = EXPAND(W08, W15, W05, W11, W02);
        R1(D, A, B, C, H, E, F, G, 0x988A32F3, W09, W09 ^ W13);
        W09 = EXPAND(W09, W00, W06, W12, W03);
        R1(C, D, A, B, G, H, E, F, 0x311465E7, W10, W10 ^ W14);
        W10 = EXPAND(W10, W01, W07, W13, W04);
        R1(B, C, D, A, F, G, H, E, 0x6228CBCE, W11, W11 ^ W15);
        W11 = EXPAND(W11, W02, W08, W14, W05);
        R1(A, B, C, D, E, F, G, H, 0xC451979C, W12, W12 ^ W00);
        W12 = EXPAND(W12, W03, W09, W15, W06);
        R1(D, A, B, C, H, E, F, G, 0x88A32F39, W13, W13 ^ W01);
        W13 = EXPAND(W13, W04, W10, W00, W07);
        R1(C, D, A, B, G, H, E, F, 0x11465E73, W14, W14 ^ W02);
        W14 = EXPAND(W14, W05, W11, W01, W08);
        R1(B, C, D, A, F, G, H, E, 0x228CBCE6, W15, W15 ^ W03);
        W15 = EXPAND(W15, W06, W12, W02, W09);
        R2(A, B, C, D, E, F, G, H, 0x9D8A7A87, W00, W00 ^ W04);
        W00 = EXPAND(W00, W07, W13, W03, W10);
        R2(D, A, B, C, H, E, F, G, 0x3B14F50F, W01, W01 ^ W05);
        W01 = EXPAND(W01, W08, W14, W04, W11);
        R2(C, D, A, B, G, H, E, F, 0x7629EA1E, W02, W02 ^ W06);
        W02 = EXPAND(W02, W09, W15, W05, W12);
        R2(B, C, D, A, F, G, H, E, 0xEC53D43C, W03, W03 ^ W07);
        W03 = EXPAND(W03, W10, W00, W06, W13);
        R2(A, B, C, D, E, F, G, H, 0xD8A7A879, W04, W04 ^ W08);
        W04 = EXPAND(W04, W11, W01, W07, W14);
        R2(D, A, B, C, H, E, F, G, 0xB14F50F3, W05, W05 ^ W09);
        W05 = EXPAND(W05, W12, W02, W08, W15);
        R2(C, D, A, B, G, H, E, F, 0x629EA1E7, W06, W06 ^ W10);
        W06 = EXPAND(W06, W13, W03, W09, W00);
        R2(B, C, D, A, F, G, H, E, 0xC53D43CE, W07, W07 ^ W11);
        W07 = EXPAND(W07, W14, W04, W10, W01);
        R2(A, B, C, D, E, F, G, H, 0x8A7A879D, W08, W08 ^ W12);
        W08 = EXPAND(W08, W15, W05, W11, W02);
        R2(D, A, B, C, H, E, F, G, 0x14F50F3B, W09, W09 ^ W13);
        W09 = EXPAND(W09, W00, W06, W12, W03);
        R2(C, D, A, B, G, H, E, F, 0x29EA1E76, W10, W10 ^ W14);
        W10 = EXPAND(W10, W01, W07, W13, W04);
        R2(B, C, D, A, F, G, H, E, 0x53D43CEC, W11, W11 ^ W15);
        W11 = EXPAND(W11, W02, W08, W14, W05);
        R2(A, B, C, D, E, F, G, H, 0xA7A879D8, W12, W12 ^ W00);
        W12 = EXPAND(W12, W03, W09, W15, W06);
        R2(D, A, B, C, H, E, F, G, 0x4F50F3B1, W13, W13 ^ W01);
        W13 = EXPAND(W13, W04, W10, W00, W07);
        R2(C, D, A, B, G, H, E, F, 0x9EA1E762, W14, W14 ^ W02);
        W14 = EXPAND(W14, W05, W11, W01, W08);
        R2(B, C, D, A, F, G, H, E, 0x3D43CEC5, W15, W15 ^ W03);
        W15 = EXPAND(W15, W06, W12, W02, W09);
        R2(A, B, C, D, E, F, G, H, 0x7A879D8A, W00, W00 ^ W04);
        W00 = EXPAND(W00, W07, W13, W03, W10);
        R2(D, A, B, C, H, E, F, G, 0xF50F3B14, W01, W01 ^ W05);
        W01 = EXPAND(W01, W08, W14, W04, W11);
        R2(C, D, A, B, G, H, E, F, 0xEA1E7629, W02, W02 ^ W06);
        W02 = EXPAND(W02, W09, W15, W05, W12);
        R2(B, C, D, A, F, G, H, E, 0xD43CEC53, W03, W03 ^ W07);
        W03 = EXPAND(W03, W10, W00, W06, W13);
        R2(A, B, C, D, E, F, G, H, 0xA879D8A7, W04, W04 ^ W08);
        W04 = EXPAND(W04, W11, W01, W07, W14);
        R2(D, A, B, C, H, E, F, G, 0x50F3B14F, W05, W05 ^ W09);
        W05 = EXPAND(W05, W12, W02, W08, W15);
        R2(C, D, A, B, G, H, E, F, 0xA1E7629E, W06, W06 ^ W10);
        W06 = EXPAND(W06, W13, W03, W09, W00);
        R2(B, C, D, A, F, G, H, E, 0x43CEC53D, W07, W07 ^ W11);
        W07 = EXPAND(W07, W14, W04, W10, W01);
        R2(A, B, C, D, E, F, G, H, 0x879D8A7A, W08, W08 ^ W12);
        W08 = EXPAND(W08, W15, W05, W11, W02);
        R2(D, A, B, C, H, E, F, G, 0x0F3B14F5, W09, W09 ^ W13);
        W09 = EXPAND(W09, W00, W06, W12, W03);
        R2(C, D, A, B, G, H, E, F, 0x1E7629EA, W10, W10 ^ W14);
        W10 = EXPAND(W10, W01, W07, W13, W04);
        R2(B, C, D, A, F, G, H, E, 0x3CEC53D4, W11, W11 ^ W15);
        W11 = EXPAND(W11, W02, W08, W14, W05);
        R2(A, B, C, D, E, F, G, H, 0x79D8A7A8, W12, W12 ^ W00);
        W12 = EXPAND(W12, W03, W09, W15, W06);
        R2(D, A, B, C, H, E, F, G, 0xF3B14F50, W13, W13 ^ W01);
        W13 = EXPAND(W13, W04, W10, W00, W07);
        R2(C, D, A, B, G, H, E, F, 0xE7629EA1, W14, W14 ^ W02);
        W14 = EXPAND(W14, W05, W11, W01, W08);
        R2(B, C, D, A, F, G, H, E, 0xCEC53D43, W15, W15 ^ W03);
        W15 = EXPAND(W15, W06, W12, W02, W09);
        R2(A, B, C, D, E, F, G, H, 0x9D8A7A87, W00, W00 ^ W04);
        W00 = EXPAND(W00, W07, W13, W03, W10);
        R2(D, A, B, C, H, E, F, G, 0x3B14F50F, W01, W01 ^ W05);
        W01 = EXPAND(W01, W08, W14, W04, W11);
        R2(C, D, A, B, G, H, E, F, 0x7629EA1E, W02, W02 ^ W06);
        W02 = EXPAND(W02, W09, W15, W05, W12);
        R2(B, C, D, A, F, G, H, E, 0xEC53D43C, W03, W03 ^ W07);
        W03 = EXPAND(W03, W10, W00, W06, W13);
        R2(A, B, C, D, E, F, G, H, 0xD8A7A879, W04, W04 ^ W08);
        R2(D, A, B, C, H, E, F, G, 0xB14F50F3, W05, W05 ^ W09);
        R2(C, D, A, B, G, H, E, F, 0x629EA1E7, W06, W06 ^ W10);
        R2(B, C, D, A, F, G, H, E, 0xC53D43CE, W07, W07 ^ W11);
        R2(A, B, C, D, E, F, G, H, 0x8A7A879D, W08, W08 ^ W12);
        R2(D, A, B, C, H, E, F, G, 0x14F50F3B, W09, W09 ^ W13);
        R2(C, D, A, B, G, H, E, F, 0x29EA1E76, W10, W10 ^ W14);
        R2(B, C, D, A, F, G, H, E, 0x53D43CEC, W11, W11 ^ W15);
        R2(A, B, C, D, E, F, G, H, 0xA7A879D8, W12, W12 ^ W00);
        R2(D, A, B, C, H, E, F, G, 0x4F50F3B1, W13, W13 ^ W01);
        R2(C, D, A, B, G, H, E, F, 0x9EA1E762, W14, W14 ^ W02);
        R2(B, C, D, A, F, G, H, E, 0x3D43CEC5, W15, W15 ^ W03);

        ctx->A ^= A;
        ctx->B ^= B;
        ctx->C ^= C;
        ctx->D ^= D;
        ctx->E ^= E;
        ctx->F ^= F;
        ctx->G ^= G;
        ctx->H ^= H;
    }
}

/******************************************************************************/
/*
 * Brief               Take in data, do SM3 but do not output. This function can
                       be called any times.

 * Param-Name[in]      data_: Pointer to the input message.
                       len: The length of input in byte.
 * Param-Name[in/out]  c: The context of SM3.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sm3_Update(Crypto_SM3_Context* c, const void* data_, uint32 len)
{
    const uint8*   data = data_;
    uint8*         p;
    uint32         l;
    uint32         n;
    Std_ReturnType ret = E_OK;

    l = (c->Nl + (((uint32)len) << 3)) & 0xffffffffUL;
    if (l < c->Nl) /* overflow */
    {
        c->Nh++;
    }
    c->Nh += (uint32)(len >> 29); /* might cause compiler warning on
                                   * 16-bit */
    c->Nl = l;

    n = c->num;
    if (n != 0)
    {
        p = (uint8*)c->data;

        if (len >= SM3_CBLOCK || len + n >= SM3_CBLOCK)
        {
            (void)IStdLib_MemCpy(p + n, data, SM3_CBLOCK - n);
            Crypto_Sm3_Block_Data_Order(c, p, 1);
            n = SM3_CBLOCK - n;
            data += n;
            len -= n;
            c->num = 0;
            /*
             * We use memset rather than OPENSSL_cleanse() here deliberately.
             * Using OPENSSL_cleanse() here could be a performance issue. It
             * will get properly cleansed on finalisation so this isn't a
             * security problem.
             */
            (void)IStdLib_MemSet(p, 0, SM3_CBLOCK); /* keep it zeroed */
        }
        else
        {
            (void)IStdLib_MemCpy(p + n, data, len);
            c->num += (uint32)len;
            return E_OK;
        }
    }

    n = len / SM3_CBLOCK;
    if (n > 0)
    {
        Crypto_Sm3_Block_Data_Order(c, data, n);
        n *= SM3_CBLOCK;
        data += n;
        len -= n;
    }

    if (len != 0)
    {
        p      = (uint8*)c->data;
        c->num = (uint8)len;
        (void)IStdLib_MemCpy(p, data, len);
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Finish SM3 calculation, and output the digest. The function
                       can only be called once.

 * Param-Name[in]      SdConsumedEventGroupHandleId: ID to identify the Consumed
 *                                                   Eventgroup.
 *                     ConsumedEventGroupState: The state the EventGroup shall be
 *                                              set to.
 * Param-Name[in/out]  c: The context of SM3.
 * Param-Name[out]     md: The output buffer stored the digest of input message.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
void Crypto_Sm3_Finish(Crypto_SM3_Context* c, uint8* md)
{
    uint8* p = (uint8*)c->data;
    uint32 n = c->num;

    p[n] = 0x80; /* there is always room for one */
    n++;

    if (n > (SM3_CBLOCK - 8))
    {
        (void)IStdLib_MemSet(p + n, 0, SM3_CBLOCK - n);
        n = 0;
        Crypto_Sm3_Block_Data_Order(c, p, 1);
    }
    (void)IStdLib_MemSet(p + n, 0, SM3_CBLOCK - 8 - n);

    p += SM3_CBLOCK - 8;
    if (CRYPTO_IS_BIG_ENDIAN == 1)
    {
        (void)HOST_l2c(c->Nh, p);
        (void)HOST_l2c(c->Nl, p);
    }
    else
    {
        (void)HOST_l2c(c->Nl, p);
        (void)HOST_l2c(c->Nh, p);
    }

    p -= SM3_CBLOCK;
    Crypto_Sm3_Block_Data_Order(c, p, 1);
    c->num = 0;
    HASH_MAKE_STRING(c, md);
}

/******************************************************************************/
/*
 * Brief               The upper interface to perform the whole SM3 algorithm on
                       input buffer.

 * Param-Name[in]      input: Message need to be digested.
 *                     ilen: The length of input message in byte.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The pointer to the buffer filled with the digest
 * 							   of message using SM3.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Sm3(uint8* input, uint32 ilen, uint8* output)
{
    Std_ReturnType     ret;
    Crypto_SM3_Context ctx;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_SM3_Context));
    Crypto_Sm3_Start(&ctx);

    ret = Crypto_Sm3_Update(&ctx, input, ilen);
    if (E_OK == ret)
    {
        Crypto_Sm3_Finish(&ctx, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               This function process process the sm3.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_Sm3_Process(uint32 objectId)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          output[32];

    uint32 ilen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    if (*(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr) >= 32u)
    {
        /* PRQA S 0311 ++ */ /*VL_QAC_0311 */
        uint8* input = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
        /* PRQA S 0311 -- */

        ret = Crypto_Sm3(input, ilen, output);
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, output, 32u);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif
