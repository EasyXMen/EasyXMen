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
 **  @file               : Crypto_62_Sm2.c
 **  @author             : Lanyan.wang
 **  @date               : 2025/07/16
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/
/* PRQA S 2781,2856,2871,2889,2911,2982,2985,2986,2992,2996,5209,3387 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1505,4544,4543,4542,4538,4534,4434,4436,4443,2995,4394,4403 ++ */ /* VL_QAC_Crypto */
/* PRQA S 4533,4532,4461,1277,0288,0311,0316,0317,0488,0489,0597,0686 ++ */ /* VL_QAC_Crypto */
/* PRQA S 0780,1258,1259,1277,1290,1291,1338,1339,1487,1820,1821,1840 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1841,1842,1843,1851,1860,1862,1863,2001,2011,2015,2016,2020 ++ */ /* VL_QAC_Crypto */
/* PRQA S 2052,2144,2205,2487,2740,2890,3101,3120,3122,3132,3138,3139 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3200,3208,3218,3223,3334,3344,3395,3397,3400,3408,3410,3412 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3415,3432,3440,3450,3625,3677,3678,3762,4115 ++ */                /* VL_QAC_Crypto */
/* PRQA S 6010 EOF */                                                       /* VL_MTR_Crypto_62_STCYC */
/* PRQA S 6080 EOF */                                                       /* VL_MTR_Crypto_62_STPTH */
/* PRQA S 6070 EOF */                                                       /* VL_MTR_Crypto_62_STCAL */
/* PRQA S 6060 EOF */                                                       /* VL_MTR_Crypto_62_STM19 */
/* PRQA S 6050 EOF */                                                       /* VL_MTR_Crypto_62_STST3 */
/* PRQA S 6040 EOF */                                                       /* VL_MTR_Crypto_62_STPAR */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"

#if (CRYPTO_ALGORITHMFAM_SM2 == STD_ON)
#include "Crypto_62_Hash.h"
#include "Crypto_62_Sm2.h"
#include "Crypto_62_Asn1.h"

#include "stdlib.h"
#include "string.h"

/*******************************************************************************
**                Definition of the basic arithmetic function                 **
*******************************************************************************/
#define add_iterat(u, r, a, b) \
    u = a + u + b;             \
    r = u;                     \
    u >>= 32;
#define sub_iterat(u, r, a, b) \
    u = a - u - b;             \
    r = u;                     \
    u >>= 63;
#define mul_iterat(u, r, a, b) \
    u += r + a * b;            \
    r = u;                     \
    u >>= 32;

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static void add(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    register uint64 u = 0;
    if (r == NULL || a == NULL || b == NULL)
    {
        return;
    }
    add_iterat(u, r[0], a[0], b[0]) add_iterat(u, r[1], a[1], b[1]) add_iterat(u, r[2], a[2], b[2])
        add_iterat(u, r[3], a[3], b[3]) add_iterat(u, r[4], a[4], b[4]) add_iterat(u, r[5], a[5], b[5])
            add_iterat(u, r[6], a[6], b[6]) u = a[7] + u + b[7];
    r[7]                                      = u;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static void sub(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    register uint64 u = 0;
    if (r == NULL || a == NULL || b == NULL)
    {
        return;
    }
    sub_iterat(u, r[0], a[0], b[0]) sub_iterat(u, r[1], a[1], b[1]) sub_iterat(u, r[2], a[2], b[2])
        sub_iterat(u, r[3], a[3], b[3]) sub_iterat(u, r[4], a[4], b[4]) sub_iterat(u, r[5], a[5], b[5])
            sub_iterat(u, r[6], a[6], b[6]) u = a[7] - u - b[7];
    r[7]                                      = u;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static void mul(sm2_dfp r, const sm2_fp a, const sm2_fp b)
{
    if (r == NULL || a == NULL || b == NULL)
    {
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        register uint64 u = 0;
        register uint64 v = a[i];
        mul_iterat(u, r[i], v, b[0]) mul_iterat(u, r[i + 1], v, b[1]) mul_iterat(u, r[i + 2], v, b[2])
            mul_iterat(u, r[i + 3], v, b[3]) mul_iterat(u, r[i + 4], v, b[4]) mul_iterat(u, r[i + 5], v, b[5])
                mul_iterat(u, r[i + 6], v, b[6]) mul_iterat(u, r[i + 7], v, b[7]) r[i + 8] = u;
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static void sqr(sm2_dfp r, const sm2_fp a)
{
    if (r == NULL || a == NULL)
    {
        return;
    }
    register uint64       u;
    register const uint64 a0 = a[0];
    register const uint64 a1 = a[1];
    register const uint64 a2 = a[2];
    register const uint64 a3 = a[3];
    register const uint64 a4 = a[4];
    register const uint64 a5 = a[5];
    register const uint64 a6 = a[6];
    register const uint64 a7 = a[7];
    u                        = 0;
    mul_iterat(u, r[0], a0, a0) mul_iterat(u, r[1], a0, a1) mul_iterat(u, r[2], a0, a2) mul_iterat(u, r[3], a0, a3)
        mul_iterat(u, r[4], a0, a4) mul_iterat(u, r[5], a0, a5) mul_iterat(u, r[6], a0, a6) mul_iterat(u, r[7], a0, a7)
            r[8] = u;

    u = 0;
    mul_iterat(u, r[1], a1, a0) mul_iterat(u, r[1 + 1], a1, a1) mul_iterat(u, r[1 + 2], a1, a2)
        mul_iterat(u, r[1 + 3], a1, a3) mul_iterat(u, r[1 + 4], a1, a4) mul_iterat(u, r[1 + 5], a1, a5)
            mul_iterat(u, r[1 + 6], a1, a6) mul_iterat(u, r[1 + 7], a1, a7) r[1 + 8] = u;

    u = 0;
    mul_iterat(u, r[2], a2, a0) mul_iterat(u, r[2 + 1], a2, a1) mul_iterat(u, r[2 + 2], a2, a2)
        mul_iterat(u, r[2 + 3], a2, a3) mul_iterat(u, r[2 + 4], a2, a4) mul_iterat(u, r[2 + 5], a2, a5)
            mul_iterat(u, r[2 + 6], a2, a6) mul_iterat(u, r[2 + 7], a2, a7) r[2 + 8] = u;

    u = 0;
    mul_iterat(u, r[3], a3, a0) mul_iterat(u, r[3 + 1], a3, a1) mul_iterat(u, r[3 + 2], a3, a2)
        mul_iterat(u, r[3 + 3], a3, a3) mul_iterat(u, r[3 + 4], a3, a4) mul_iterat(u, r[3 + 5], a3, a5)
            mul_iterat(u, r[3 + 6], a3, a6) mul_iterat(u, r[3 + 7], a3, a7) r[3 + 8] = u;

    u = 0;
    mul_iterat(u, r[4], a4, a0) mul_iterat(u, r[4 + 1], a4, a1) mul_iterat(u, r[4 + 2], a4, a2)
        mul_iterat(u, r[4 + 3], a4, a3) mul_iterat(u, r[4 + 4], a4, a4) mul_iterat(u, r[4 + 5], a4, a5)
            mul_iterat(u, r[4 + 6], a4, a6) mul_iterat(u, r[4 + 7], a4, a7) r[4 + 8] = u;

    u = 0;
    mul_iterat(u, r[5], a5, a0) mul_iterat(u, r[5 + 1], a5, a1) mul_iterat(u, r[5 + 2], a5, a2)
        mul_iterat(u, r[5 + 3], a5, a3) mul_iterat(u, r[5 + 4], a5, a4) mul_iterat(u, r[5 + 5], a5, a5)
            mul_iterat(u, r[5 + 6], a5, a6) mul_iterat(u, r[5 + 7], a5, a7) r[5 + 8] = u;

    u = 0;
    mul_iterat(u, r[6], a6, a0) mul_iterat(u, r[6 + 1], a6, a1) mul_iterat(u, r[6 + 2], a6, a2)
        mul_iterat(u, r[6 + 3], a6, a3) mul_iterat(u, r[6 + 4], a6, a4) mul_iterat(u, r[6 + 5], a6, a5)
            mul_iterat(u, r[6 + 6], a6, a6) mul_iterat(u, r[6 + 7], a6, a7) r[6 + 8] = u;

    u = 0;
    mul_iterat(u, r[7], a7, a0) mul_iterat(u, r[7 + 1], a7, a1) mul_iterat(u, r[7 + 2], a7, a2)
        mul_iterat(u, r[7 + 3], a7, a3) mul_iterat(u, r[7 + 4], a7, a4) mul_iterat(u, r[7 + 5], a7, a5)
            mul_iterat(u, r[7 + 6], a7, a6) mul_iterat(u, r[7 + 7], a7, a7) r[7 + 8] = u;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static void haf(sm2_fp r, const sm2_fp a)
{
    if (r == NULL || a == NULL)
    {
        return;
    }
    r[0] = a[0] >> 1 | a[1] << 31;
    r[1] = a[1] >> 1 | a[2] << 31;
    r[2] = a[2] >> 1 | a[3] << 31;
    r[3] = a[3] >> 1 | a[4] << 31;
    r[4] = a[4] >> 1 | a[5] << 31;
    r[5] = a[5] >> 1 | a[6] << 31;
    r[6] = a[6] >> 1 | a[7] << 31;
    r[7] = a[7] >> 1;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static void dbl(sm2_fp r, const sm2_fp a)
{
    if (r == NULL || a == NULL)
    {
        return;
    }
    r[7] = a[7] << 1 | a[6] >> 31;
    r[6] = a[6] << 1 | a[5] >> 31;
    r[5] = a[5] << 1 | a[4] >> 31;
    r[4] = a[4] << 1 | a[3] >> 31;
    r[3] = a[3] << 1 | a[2] >> 31;
    r[2] = a[2] << 1 | a[1] >> 31;
    r[1] = a[1] << 1 | a[0] >> 31;
    r[0] = a[0] << 1;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 adds(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    register uint64 u = 0;
    add_iterat(u, r[0], a[0], b[0]) add_iterat(u, r[1], a[1], b[1]) add_iterat(u, r[2], a[2], b[2])
        add_iterat(u, r[3], a[3], b[3]) add_iterat(u, r[4], a[4], b[4]) add_iterat(u, r[5], a[5], b[5])
            add_iterat(u, r[6], a[6], b[6]) add_iterat(u, r[7], a[7], b[7]) return u;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 subs(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    register uint64 u = 0;
    sub_iterat(u, r[0], a[0], b[0]) sub_iterat(u, r[1], a[1], b[1]) sub_iterat(u, r[2], a[2], b[2])
        sub_iterat(u, r[3], a[3], b[3]) sub_iterat(u, r[4], a[4], b[4]) sub_iterat(u, r[5], a[5], b[5])
            sub_iterat(u, r[6], a[6], b[6]) sub_iterat(u, r[7], a[7], b[7]) return u;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definitions =========================================== */
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
static const uint8 hstr_to_byte[] = {
    0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
    0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
/* ========================================== internal function declarations ======================================== */
/*******************************************************************************
**            Definition of the modulo sm2_p arithmetic function              **
*******************************************************************************/
/**
 * @brief Sets the value of one number to another.
 * @param [out] r The result number.
 * @param [in] a The number to copy from.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_set(sm2_fp r, const sm2_fp a)
{
    (void)IStdLib_MemCpy(r, a, sizeof(sm2_fp));
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Compares two numbers.
 * @param [in] a The first number.
 * @param [in] b The second number.
 * @return 1 if a >= b, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_fp_cmp(const sm2_fp a, const sm2_fp b)
{
    if (a[7] > b[7])
    {
        return 1;
    }
    if (a[7] < b[7])
    {
        return 0;
    }
    if (a[6] > b[6])
    {
        return 1;
    }
    if (a[6] < b[6])
    {
        return 0;
    }
    if (a[5] > b[5])
    {
        return 1;
    }
    if (a[5] < b[5])
    {
        return 0;
    }
    if (a[4] > b[4])
    {
        return 1;
    }
    if (a[4] < b[4])
    {
        return 0;
    }
    if (a[3] > b[3])
    {
        return 1;
    }
    if (a[3] < b[3])
    {
        return 0;
    }
    if (a[2] > b[2])
    {
        return 1;
    }
    if (a[2] < b[2])
    {
        return 0;
    }
    if (a[1] > b[1])
    {
        return 1;
    }
    if (a[1] < b[1])
    {
        return 0;
    }
    if (a[0] > b[0])
    {
        return 1;
    }
    if (a[0] < b[0])
    {
        return 0;
    }
    return 1;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Checks whether the number is odd.
 * @param [in] a The number to check.
 * @return 1 if odd, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_fp_is_odd(const sm2_fp a)
{
    return (int)a[0] & 1;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Checks whether the number is even.
 * @param [in] a The number to check.
 * @return 1 if even, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_fp_is_even(const sm2_fp a)
{
    if (a == NULL)
    {
        return 0;
    }
    return ((int)(a[0]) & 1) ^ 1;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief  Checks whether the number is non-zero.
 * @param [in] a The number to check.
 * @return 1 if not zero, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_fp_non_zero(const sm2_fp a)
{
    return a[0] > 0 || a[1] > 0 || a[2] > 0 || a[3] > 0 || a[4] > 0 || a[5] > 0 || a[6] > 0 || a[7] > 0;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Checks whether the number is zero.
 * @param [in] a The number to check.
 * @return 1 if zero, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_fp_is_zero(const sm2_fp a)
{
    return a[0] == 0 && a[1] == 0 && a[2] == 0 && a[3] == 0 && a[4] == 0 && a[5] == 0 && a[6] == 0 && a[7] == 0;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes halving modulo sm2_p, i.e., r = a/2 mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to halve.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_haf(sm2_fp r, const sm2_fp a)
{
    if (sm2_fp_is_even(a))
    {
        haf(r, a);
    }
    else
    {
        haf(r, a);
        add(r, r, sm2_s);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes multiplicative inverse modulo sm2_p, i.e., r = a^-1 mod sm2_p.
 * @param [out] r The result number.
 * @param [in] q The number to invert.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_inv(sm2_fp r, const sm2_fp q)
{
    sm2_fp u;
    sm2_fp v;
    sm2_fp a = {1};
    sm2_fp c = {0};
    sm2_fp_set(u, q);
    sm2_fp_set(v, sm2_p);
    while (sm2_fp_non_zero(u))
    {
        while (u[0])
        {
            while (sm2_fp_is_even(u))
            {
                haf(u, u);
                sm2_fp_haf(a, a);
            }
            while (sm2_fp_is_even(v))
            {
                haf(v, v);
                sm2_fp_haf(c, c);
            }
            if (sm2_fp_cmp(u, v))
            {
                sub(u, u, v);
                if (subs(a, a, c))
                {
                    add(a, a, sm2_p);
                }
            }
            else
            {
                sub(v, v, u);
                if (subs(c, c, a))
                {
                    add(c, c, sm2_p);
                }
            }
        }
    }
    sm2_fp_set(r, c);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Checks whether two numbers are equal.
 * @param [in] a The first number.
 * @param [in] b The second number.
 * @return 1 if equal, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_fp_equ(const sm2_fp a, const sm2_fp b)
{
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3] && a[4] == b[4] && a[5] == b[5] && a[6] == b[6]
           && a[7] == b[7];
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes doubling modulo sm2_p, i.e., r = 2a mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to double.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_dbl(sm2_fp r, const sm2_fp a)
{
    if (a[7] >> 31)
    {
        dbl(r, a);
        sub(r, r, sm2_p);
    }
    else
    {
        dbl(r, a);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes additive inverse modulo sm2_p, i.e., r = -a mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to negate.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_neg(sm2_fp r, const sm2_fp a)
{
    sub(r, sm2_p, a);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes the reduction modulo sm2_p, i.e., r = c mod sm2_p.
 * @param [out] r The result number.
 * @param [in] c The dnumber to recover from.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_rec(sm2_fp r, const sm2_dfp c)
{
    if (c == NULL)
    {
        return;
    }
    // the middle value
    uint64 w0 = 0;
    uint64 w1 = 0;
    uint64 w2 = 0;
    uint64 w3 = 0;
    uint64 w4 = 0;
    uint64 w5 = 0;
    w0        = w0 + c[8] + c[9] + c[10] + c[11];
    w1        = w1 + c[8] + c[13];
    w2        = w2 + c[9] + c[14];
    w3        = w3 + c[14] + c[15];
    w4        = w4 + w3 + c[13];
    w5        = w5 + w4 + c[12];
    w0        = w0 + w5;

    // t = c mod sm2_p
    uint64 t0;
    uint64 t1;
    uint64 t2;
    uint64 t3;
    uint64 t4;
    uint64 t5;
    uint64 t6;
    uint64 t7;
    t0 = c[0] + w0 + w4;
    t1 = c[1] + w0 + w4 - w1;
    t2 = c[2] - w1 - w2;
    t3 = c[3] + w5 + w1 + c[11];
    t4 = c[4] + w5 + w2;
    t5 = c[5] + w4 + c[10] + c[15];
    t6 = c[6] + w3 + c[11];
    t7 = c[7] + w0 + w5 + c[15];
    t1 += (int)(t0 >> 32);
    t0 &= 0xFFFFFFFFU;
    t2 += (int)(t1 >> 32);
    t1 &= 0xFFFFFFFFU;
    t3 += (int)(t2 >> 32);
    t2 &= 0xFFFFFFFFU;
    t4 += (int)(t3 >> 32);
    t3 &= 0xFFFFFFFFU;
    t5 += (int)(t4 >> 32);
    t4 &= 0xFFFFFFFFU;
    t6 += (int)(t5 >> 32);
    t5 &= 0xFFFFFFFFU;
    t7 += (int)(t6 >> 32);
    t6 &= 0xFFFFFFFFU;
    t0 += (int)(t7 >> 32);
    t2 -= (int)(t7 >> 32);
    t3 += (int)(t7 >> 32);
    t7 += (int)(t7 >> 32);
    t1 += (int)(t0 >> 32);
    t2 += (int)(t1 >> 32);
    t3 += (int)(t2 >> 32);
    t4 += (int)(t3 >> 32);
    t5 += (int)(t4 >> 32);
    t6 += (int)(t5 >> 32);
    t7 += (int)(t6 >> 32);
    r[0] = t0;
    r[1] = t1;
    r[2] = t2;
    r[3] = t3;
    r[4] = t4;
    r[5] = t5;
    r[6] = t6;
    r[7] = t7;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes squaring modulo sm2_p, i.e., r = a^2 mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to square.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_sqr(sm2_fp r, const sm2_fp a)
{
    sm2_dfp c = {0};
    sqr(c, a);
    sm2_fp_rec(r, c);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes multiplication modulo sm2_p, i.e., r = a * b mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The first number.
 * @param [in] b The second number.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_mul(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    sm2_dfp c = {0};
    mul(c, a, b);
    sm2_fp_rec(r, c);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes cubing modulo sm2_p, i.e., r = a^3 mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to cube.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_cub(sm2_fp r, const sm2_fp a)
{
    sm2_fp_sqr(r, a);
    sm2_fp_mul(r, r, a);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Converts a number to its byte array.
 * @param [in] n The number to convert.
 * @param [out] b The byte array to store the result.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_to_bn(const sm2_fp n, sm2_bn b)
{
    const uint8* t = (const uint8*)n;
    for (int i = 0; i < 32; i++)
    {
        b[i] = t[31 - i];
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Fills a number from a byte array.
 * @param [in] b The byte array to convert from.
 * @param [out] n The result number.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_bn_to_fp(const sm2_bn b, sm2_fp n)
{
    uint8* t = (uint8*)n;
    for (int i = 0; i < 32; i++)
    {
        t[31 - i] = b[i];
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes subtraction modulo sm2_p, i.e., r = a - b mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The minuend.
 * @param [in] b The subtrahend.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_sub(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    if (subs(r, a, b))
    {
        add(r, r, sm2_p);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes addition modulo sm2_p, i.e., r = a + b mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The first number.
 * @param [in] b The second number.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_add(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    if (adds(r, a, b))
    {
        sub(r, r, sm2_p);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Generates a random number in the range (0, p) for give p.
 * @param [out] r The output number.
 * @param [in] p The upper bound.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_rand(sm2_fp r, const sm2_fp p)
{
    sm2_fp_sub(r, p, sm2_one);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Fills a number from a hex string representation.
 * @param [out] n The result number.
 * @param [in] s The string to convert from.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_from_str(sm2_fp n, const char* s)
{
    uint32 t[64];
    for (int i = 0; i < 64; i++)
    {
        t[63 - i] = hstr_to_byte[(uint8)s[i]];
    }
    for (int i = 0; i < 8; i++)
    {
        n[i] = 0;
        for (int j = 0; j < 8; j++)
        {
            n[i] |= t[(i << 3) + j] << (j << 2);
        }
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes the Non-Adjacent Form (NAF) representation of a number.
 * @param [out] r The result NAF array.
 * @param [in] w The width of NAF.
 * @param [in] n The number to compute the NAF for.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_naf(sm2_naf r, const uint8 w, const sm2_fp n)
{
    if (w > 7)
    {
        return; // w > 7 is not supported unless the sm2_naf type definition is expanded from i8[257] to int[257]
    }

    int    i = 256;
    sm2_fp k;
    sm2_fp t;
    sm2_fp_set(k, n);
    while (sm2_fp_cmp(k, sm2_one))
    {
        if (sm2_fp_is_odd(k))
        {
            sm2_fp_set(t, sm2_zero);
            t[0] = k[0] & ((1 << w) - 1); // t[0] = k[0] mod 2^w
            if (t[0] >> (w - 1))
            {
                t[0] = (1 << w) - t[0];
                r[i] = (sint8)-t[0];
            }
            else
            {
                r[i] = (sint8)t[0];
            }
            if (r[i] > 0)
            {
                sm2_fp_sub(k, k, t);
            }
            else
            {
                sm2_fp_add(k, k, t);
            }
        }
        else
        {
            r[i] = 0;
        }
        sm2_fp_haf(k, k);
        i--;
    }
    while (i >= 0)
    {
        r[i--] = 0;
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes the Non-Adjacent Form (NAF) representation of a number.
 * @param [out] K The result NAF array.
 * @param [in] k The number to compute the NAF for.
 * @note it only used in sm2_jpoint_gen
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fp_nafp(sint8 K[52], const sm2_fp k)
{
    sm2_naf kn;
    sm2_fp_naf(kn, 2, k);
    int i = 0;
    int j = 0;
    for (i = 256; i > 2; i = i - 5)
    {
        K[j++] = (sint8)(kn[i] + (kn[i - 1] << 1) + (kn[i - 2] << 2) + (kn[i - 3] << 3) + (kn[i - 4] << 4));
    }
    K[j] = (sint8)(kn[1] + (kn[0] << 1));
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Sets the coordinates of a Jacobian point.
 * @param [out] p Pointer to the destination SM2JacobianPoint.
 * @param [in] x X coordinate to set.
 * @param [in] y Y coordinate to set.
 * @param [in] z Z coordinate to set.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_set(sm2_jpoint* p, const sm2_fp x, const sm2_fp y, const sm2_fp z)
{
    sm2_fp_set(p->x, x);
    sm2_fp_set(p->y, y);
    sm2_fp_set(p->z, z);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Sets a Jacobian point to the point at infinity.
 * @param [out] r Pointer to the SM2JacobianPoint to be set.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_set_infinity(sm2_jpoint* r)
{
    sm2_jpoint_set(r, sm2_one, sm2_one, sm2_zero);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Copies the content of one Jacobian point to another.
 * @param [out] p Pointer to the destination SM2JacobianPoint.
 * @param [in] q Pointer to the source SM2JacobianPoint.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_copy(sm2_jpoint* p, const sm2_jpoint* q)
{
    (void)IStdLib_MemCpy(p, q, 96);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Checks if a jpoint is at infinity.
 * @param [in] r The jpoint to check.
 * @return 1 if the jpoint is at infinity, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_jpoint_at_infinity(const sm2_jpoint* r)
{
    return sm2_fp_is_zero(r->z);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Doubles a Jacobian point.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] a Pointer to the SM2JacobianPoint to be doubled.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_dou(sm2_jpoint* r, const sm2_jpoint* a)
{
    if (sm2_jpoint_at_infinity(a))
    {
        sm2_jpoint_copy(r, a);
        return;
    }
    // A = 3(x1 - z1^2) * (x1 + z1^2)
    // B = 2Y1, z3 = B * z1, C = B^2
    // D = C * x1, x3 = A^2 - 2D, (D - x3) * A - C^2/2
    const uint32* x1 = a->x;
    const uint32* y1 = a->y;
    const uint32* z1 = a->z;

    sm2_fp t1;
    sm2_fp t2;
    sm2_fp t3;
    sm2_fp x3;
    sm2_fp y3;
    sm2_fp z3;
    sm2_fp_sqr(t1, z1);     // t1 = z1^2
    sm2_fp_sub(t2, x1, t1); // t2 = x1 - z1^2
    sm2_fp_add(t1, x1, t1); // t1 = x1 + z1^2
    sm2_fp_mul(t2, t2, t1); // t2 = x1^2 - z1^4
    sm2_fp_dbl(t3, t2);     // t3 = 2(x1^2 - z1^4)
    sm2_fp_add(t2, t2, t3); // t2 = A = 3t2 = 3(x1^2 - z1^4)
    sm2_fp_dbl(y3, y1);     // y3 = B = 2y1
    sm2_fp_mul(z3, y3, z1); // z3 = B * z1
    sm2_fp_sqr(y3, y3);     // y3 = C = B^2
    sm2_fp_mul(t3, y3, x1); // t3 = D = C * x1
    sm2_fp_sqr(y3, y3);     // y3 = C^2
    sm2_fp_haf(y3, y3);     // y3 = C^2/2
    sm2_fp_sqr(x3, t2);     // x3 = A^2
    sm2_fp_dbl(t1, t3);     // t1 = 2D
    sm2_fp_sub(x3, x3, t1); // x3 = A^2 - 2D
    sm2_fp_sub(t1, t3, x3); // t1 = D - x3
    sm2_fp_mul(t1, t1, t2); // t1 = (D - x3) * A
    sm2_fp_sub(y3, t1, y3); // y3 = (D - x3) * A - C^2/2
    sm2_jpoint_set(r, x3, y3, z3);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Performs scalar multiplication on a Jacobian point.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] m Scalar for multiplication.
 * @param [in] p Pointer to the SM2JacobianPoint to be multiplied.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_mdo(sm2_jpoint* r, uint32 m, const sm2_jpoint* p)
{
    if (sm2_jpoint_at_infinity(p))
    {
        sm2_jpoint_set(r, p->x, p->y, p->z);
        return;
    }

    sm2_fp x;
    sm2_fp y;
    sm2_fp z;
    sm2_fp w;
    sm2_fp a;
    sm2_fp b;
    sm2_fp t;
    sm2_fp_set(x, p->x);
    sm2_fp_set(y, p->y);
    sm2_fp_set(z, p->z);
    sm2_fp_dbl(y, y); // y = 2y
    sm2_fp_sqr(w, z);
    sm2_fp_sqr(w, w); // w = z^4
    while (m--)
    {
        sm2_fp_sqr(a, x);
        sm2_fp_sub(a, a, w);
        sm2_fp_dbl(t, a);
        sm2_fp_add(a, a, t); // a = 3(x^2 - w)
        sm2_fp_sqr(b, y);
        sm2_fp_mul(b, x, b); // b = x * y^2
        sm2_fp_sqr(x, a);
        sm2_fp_sub(x, x, b);
        sm2_fp_sub(x, x, b); // x = a^2 - 2b
        sm2_fp_mul(z, z, y); // z = z * y
        sm2_fp_sqr(t, y);
        sm2_fp_sqr(t, t); // t = y^4
        if (m)
        {
            sm2_fp_mul(w, w, t); // w = w * y^4
        }
        sm2_fp_sub(y, b, x);
        sm2_fp_mul(y, y, a);
        sm2_fp_dbl(y, y);
        sm2_fp_sub(y, y, t); // y = 2(b-x) - y^4
    }
    sm2_fp_haf(y, y);
    sm2_jpoint_set(r, x, y, z);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Adds two Jacobian points.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] p Pointer to the first SM2JacobianPoint.
 * @param [in] q Pointer to the second SM2JacobianPoint.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_add(sm2_jpoint* r, const sm2_jpoint* p, const sm2_jpoint* q)
{
    // Check if one of the points is the point at infinity
    if (sm2_jpoint_at_infinity(p))
    {
        sm2_jpoint_copy(r, q);
        return;
    }
    if (sm2_jpoint_at_infinity(q))
    {
        sm2_jpoint_copy(r, p);
        return;
    }

    const uint32* x1 = p->x;
    const uint32* y1 = p->y;
    const uint32* z1 = p->z;
    const uint32* x2 = q->x;
    const uint32* y2 = q->y;
    const uint32* z2 = q->z;
    sm2_fp        x3;
    sm2_fp        y3;
    sm2_fp        z3;
    sm2_fp        u1;
    sm2_fp        u2;
    sm2_fp        s1;
    sm2_fp        s2;
    sm2_fp        h;
    sm2_fp        n;
    sm2_fp        h2;
    sm2_fp        h3;
    sm2_fp        u1h2;
    sm2_fp        t1;
    sm2_fp        t2;

    sm2_fp_sqr(t1, z1);     // t1 = z1^2
    sm2_fp_sqr(t2, z2);     // t2 = z2^2
    sm2_fp_mul(u1, x1, t2); // u1 = x1 * z2^2
    sm2_fp_mul(u2, x2, t1); // u2 = x2 * z1^2
    sm2_fp_mul(t1, t1, z1); // t1 = z1^3
    sm2_fp_mul(t2, t2, z2); // t2 = z2^3
    sm2_fp_mul(s1, y1, t2); // s1 = y1 * z2^3
    sm2_fp_mul(s2, y2, t1); // s2 = y2 * z1^3
    if (sm2_fp_equ(u1, u2))
    {
        if (sm2_fp_equ(s1, s2))
        {
            sm2_jpoint_dou(r, p);
        }
        else
        {
            sm2_jpoint_set_infinity(r);
        }
        return;
    }
    sm2_fp_sub(h, u2, u1);    // h = u2 - u1
    sm2_fp_sub(n, s2, s1);    // n = s2 - s1
    sm2_fp_sqr(h2, h);        // h2 = h^2
    sm2_fp_mul(h3, h2, h);    // h3 = h^3
    sm2_fp_mul(u1h2, u1, h2); // u1h2 = u1 * h^2
    sm2_fp_dbl(t1, u1h2);     // t1 = 2u1h2
    sm2_fp_sqr(x3, n);        // x3 = n^2
    sm2_fp_sub(x3, x3, h3);   // x3 = n^2 - h3
    sm2_fp_sub(x3, x3, t1);   // x3 = n^2 - h3 - 2u1h2
    sm2_fp_mul(t1, s1, h3);   // t1 = s1 * h3
    sm2_fp_sub(y3, u1h2, x3); // y3 = u1h2 - x3
    sm2_fp_mul(y3, y3, n);    // y3 = n * (u1h2 - x3)
    sm2_fp_sub(y3, y3, t1);   // y3 = n * (u1h2 - x3) - s1 * h3
    sm2_fp_mul(z3, z1, z2);   // z3 = z1 * z2
    sm2_fp_mul(z3, z3, h);    // z3 = h * z1 * z2
    sm2_jpoint_set(r, x3, y3, z3);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Subtracts one Jacobian point from another.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] p Pointer to the minuend SM2JacobianPoint.
 * @param [in] q Pointer to the subtrahend SM2JacobianPoint.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_sub(sm2_jpoint* r, const sm2_jpoint* p, const sm2_jpoint* q)
{
    // Check if one of the points is the point at infinity
    if (sm2_jpoint_at_infinity(p))
    {
        sm2_jpoint_copy(r, q);
        return;
    }
    if (sm2_jpoint_at_infinity(q))
    {
        sm2_jpoint_copy(r, p);
        return;
    }

    const uint32* x1 = p->x;
    const uint32* y1 = p->y;
    const uint32* z1 = p->z;
    const uint32* x2 = q->x;
    const uint32* z2 = q->z;

    sm2_fp y2;
    sm2_fp x3;
    sm2_fp y3;
    sm2_fp z3;
    sm2_fp u1;
    sm2_fp u2;
    sm2_fp s1;
    sm2_fp s2;
    sm2_fp h;
    sm2_fp n;
    sm2_fp h2;
    sm2_fp h3;
    sm2_fp u1h2;
    sm2_fp t1;
    sm2_fp t2;

    sm2_fp_neg(y2, q->y);
    sm2_fp_sqr(t1, z1);     // t1 = z1^2
    sm2_fp_sqr(t2, z2);     // t2 = z2^2
    sm2_fp_mul(u1, x1, t2); // u1 = x1 * z2^2
    sm2_fp_mul(u2, x2, t1); // u2 = x2 * z1^2
    sm2_fp_mul(t1, t1, z1); // t1 = z1^3
    sm2_fp_mul(t2, t2, z2); // t2 = z2^3
    sm2_fp_mul(s1, y1, t2); // s1 = y1 * z2^3
    sm2_fp_mul(s2, y2, t1); // s2 = y2 * z1^3
    if (sm2_fp_equ(u1, u2))
    {
        if (sm2_fp_equ(s1, s2))
        {
            sm2_jpoint_dou(r, p);
        }
        else
        {
            sm2_jpoint_set_infinity(r);
        }
        return;
    }
    sm2_fp_sub(h, u2, u1);    // h = u2 - u1
    sm2_fp_sub(n, s2, s1);    // n = s2 - s1
    sm2_fp_sqr(h2, h);        // h2 = h^2
    sm2_fp_mul(h3, h2, h);    // h3 = h^3
    sm2_fp_mul(u1h2, u1, h2); // u1h2 = u1 * h^2
    sm2_fp_dbl(t1, u1h2);     // t1 = 2u1h2
    sm2_fp_sqr(x3, n);        // x3 = n^2
    sm2_fp_sub(x3, x3, h3);   // x3 = n^2 - h3
    sm2_fp_sub(x3, x3, t1);   // x3 = n^2 - h3 - 2u1h2
    sm2_fp_mul(t1, s1, h3);   // t1 = s1 * h3
    sm2_fp_sub(y3, u1h2, x3); // y3 = u1h2 - x3
    sm2_fp_mul(y3, y3, n);    // y3 = n * (u1h2 - x3)
    sm2_fp_sub(y3, y3, t1);   // y3 = n * (u1h2 - x3) - s1 * h3
    sm2_fp_mul(z3, z1, z2);   // z3 = z1 * z2
    sm2_fp_mul(z3, z3, h);    // z3 = h * z1 * z2
    sm2_jpoint_set(r, x3, y3, z3);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Multiplies a scalar with a given Jacobian point.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] k Scalar for multiplication.
 * @param [in] g Pointer to the SM2JacobianPoint to be multiplied.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_mul(sm2_jpoint* r, const sm2_fp k, const sm2_jpoint* g)
{
    static sm2_naf    K;
    static sm2_jpoint upt[8];

    // compute the sm2_naf of k
    sm2_fp_naf(K, 4, k);

    // compute the table of point g: {g, 3g, 5g, 7g, ...}
    sm2_jpoint_copy(&upt[0], g);
    sm2_jpoint_dou(r, g);
    for (uint32 i = 1; i < 8; i++)
    {
        sm2_jpoint_add(&upt[i], &upt[i - 1], r);
    }

    // compute the result
    uint32 i = 0;
    uint32 j = 1;
    sm2_jpoint_set_infinity(r);
    do
    {
        if (K[i] == 0)
        {
            j++;
        }
        else
        {
            sm2_jpoint_mdo(r, j, r);
            if (K[i] > 0)
            {
                sm2_jpoint_add(r, r, &upt[K[i] >> 1]);
            }
            else
            {
                sm2_jpoint_sub(r, r, &upt[(-K[i]) >> 1]);
            }
            j = 1;
        }
    } while (++i <= 256);
    sm2_jpoint_mdo(r, j - 1, r);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Converts an affine point to a Jacobian point.
 * @param [in] p Pointer to the source SM2AffinePoint.
 * @param [out] r Pointer to the destination SM2JacobianPoint.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_apoint_to_jpoint(const sm2_apoint* p, sm2_jpoint* r)
{
    (void)IStdLib_MemCpy(r, p, sizeof(sm2_apoint));
    (void)IStdLib_MemCpy(r->z, sm2_one, sizeof(sm2_fp));
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Checks if an affine point represents the point at infinity.
 * @param [in] r Pointer to the SM2AffinePoint to be checked.
 * @return Returns 1 if the point is at infinity, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_apoint_at_infinity(const sm2_apoint* r)
{
    return sm2_fp_equ(r->x, sm2_zero) && sm2_fp_equ(r->y, sm2_zero);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Point addition, affine-Jacobian coordinates
 * @param [out] r The result of the addition.
 * @param [in] p The first jpoint.
 * @param [in] q The second point.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_japoint_add(sm2_jpoint* r, const sm2_jpoint* p, const sm2_apoint* q)
{
    if (sm2_jpoint_at_infinity(p))
    {
        sm2_apoint_to_jpoint(q, r);
        return;
    }
    if (sm2_apoint_at_infinity(q))
    {
        sm2_jpoint_copy(r, p);
        return;
    }

    const uint32* x1 = p->x;
    const uint32* y1 = p->y;
    const uint32* z1 = p->z;
    const uint32* x2 = q->x;
    const uint32* y2 = q->y;

    sm2_fp x3;
    sm2_fp y3;
    sm2_fp z3;
    sm2_fp t1;
    sm2_fp t2;
    sm2_fp t3;
    sm2_fp t4;
    sm2_fp_sqr(t1, z1);     // t1 = A = z1^2
    sm2_fp_mul(t2, t1, z1); // t2 = B = z1 * A
    sm2_fp_mul(t1, t1, x2); // t1 = C = x2 * A
    sm2_fp_mul(t2, t2, y2); // t2 = D = y2 * B
    sm2_fp_sub(t1, t1, x1); // t1 = E = C - x1
    sm2_fp_sub(t2, t2, y1); // t2 = F = D - y1
    if (sm2_fp_equ(t1, sm2_zero))
    {
        if (sm2_fp_equ(t2, sm2_zero))
        {
            sm2_jpoint t;
            sm2_jpoint_set(&t, x2, y2, sm2_one);
            sm2_jpoint_dou(r, &t);
        }
        else
        {
            sm2_jpoint_set_infinity(r);
        }
        return;
    }
    sm2_fp_mul(z3, z1, t1); // z3 = z1 * E
    sm2_fp_sqr(t3, t1);     // t3 = G = E^2
    sm2_fp_mul(t4, t3, t1); // t4 = H = E^3
    sm2_fp_mul(t3, t3, x1); // t3 = I = x1 * G
    sm2_fp_dbl(t1, t3);     // t1 = 2I
    sm2_fp_sqr(x3, t2);     // x3 = F^2
    sm2_fp_sub(x3, x3, t1); // x3 = F^2 - 2I
    sm2_fp_sub(x3, x3, t4); // x3 = F^2 - 2I - H
    sm2_fp_sub(t3, t3, x3); // t3 = I - x3
    sm2_fp_mul(t3, t3, t2); // t3 = (I - x3) * F
    sm2_fp_mul(t4, t4, y1); // t4 = y1 * H
    sm2_fp_sub(y3, t3, t4); // y3 = (I - x3) * F - y1 * H
    sm2_jpoint_set(r, x3, y3, z3);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Point subtraction, affine-Jacobian coordinates
 * @param [out] r The result of the subtraction.
 * @param [in] p The first jpoint.
 * @param [in] q The second point.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_japoint_sub(sm2_jpoint* r, const sm2_jpoint* p, const sm2_apoint* q)
{
    const uint32* x1 = p->x;
    const uint32* y1 = p->y;
    const uint32* z1 = p->z;
    const uint32* x2 = q->x;

    sm2_fp y2;
    sm2_fp x3;
    sm2_fp y3;
    sm2_fp z3;
    sm2_fp t1;
    sm2_fp t2;
    sm2_fp t3;
    sm2_fp t4;
    sm2_fp_neg(y2, q->y);

    if (sm2_jpoint_at_infinity(p))
    {
        sm2_jpoint_set(r, x2, y2, sm2_one);
        return;
    }
    if (sm2_apoint_at_infinity(q))
    {
        sm2_jpoint_copy(r, p);
        return;
    }

    sm2_fp_sqr(t1, z1);     // t1 = A = z1^2
    sm2_fp_mul(t2, t1, z1); // t2 = B = z1 * A
    sm2_fp_mul(t1, t1, x2); // t1 = C = x2 * A
    sm2_fp_mul(t2, t2, y2); // t2 = D = y2 * B
    sm2_fp_sub(t1, t1, x1); // t1 = E = C - x1
    sm2_fp_sub(t2, t2, y1); // t2 = F = D - y1
    if (sm2_fp_equ(t1, sm2_zero))
    {
        if (sm2_fp_equ(t2, sm2_zero))
        {
            sm2_jpoint t;
            sm2_jpoint_set(&t, x2, y2, sm2_one);
            sm2_jpoint_dou(r, &t);
        }
        else
        {
            sm2_jpoint_set_infinity(r);
        }
        return;
    }
    sm2_fp_mul(z3, z1, t1); // z3 = z1 * E
    sm2_fp_sqr(t3, t1);     // t3 = G = E^2
    sm2_fp_mul(t4, t3, t1); // t4 = H = E^3
    sm2_fp_mul(t3, t3, x1); // t3 = I = x1 * G
    sm2_fp_dbl(t1, t3);     // t1 = 2I
    sm2_fp_sqr(x3, t2);     // x3 = F^2
    sm2_fp_sub(x3, x3, t1); // x3 = F^2 - 2I
    sm2_fp_sub(x3, x3, t4); // x3 = F^2 - 2I - H
    sm2_fp_sub(t3, t3, x3); // t3 = I - x3
    sm2_fp_mul(t3, t3, t2); // t3 = (I - x3) * F
    sm2_fp_mul(t4, t4, y1); // t4 = y1 * H
    sm2_fp_sub(y3, t3, t4); // y3 = (I - x3) * F - y1 * H
    sm2_jpoint_set(r, x3, y3, z3);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#if Large_lookup_table_allowed == 1
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_gen(sm2_jpoint* r, const sm2_fp k)
{
    uint8 K[32];
    (void)IStdLib_MemCpy(K, k, 32);

    sm2_apoint_to_jpoint(&sm2_pct[0][K[0]], r);
    for (int i = 1; i < 32; i++)
    {
        sm2_japoint_add(r, r, &sm2_pct[i][K[i]]);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#else
/**
 * @brief Multiplies a scalar with a base Jacobian point.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] k Scalar used for the generation.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_gen(sm2_jpoint* r, const sm2_fp k)
{
    static sm2_jpoint a;
    static sm2_jpoint b;
    static sint8      K[52];

    // set infinity jpoint(1, 1, 0)
    sm2_jpoint_set_infinity(&a);
    sm2_jpoint_set_infinity(&b);

    // compute the sm2_naf of k
    sm2_fp_nafp(K, k);

    // compute the result
    for (int j = 21; j > 0; j--)
    {
        for (int i = 0; i < 52; i++)
        {
            if (K[i] == j)
            {
                sm2_japoint_add(&b, &b, &fpt[i]);
            }
            if (K[i] == -j)
            {
                sm2_japoint_sub(&b, &b, &fpt[i]);
            }
        }
        sm2_jpoint_add(&a, &a, &b);
    }
    sm2_jpoint_copy(r, &a);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

/**
 * @brief Converts a Jacobian point to an affine point.
 * @param [in] a Pointer to the source SM2JacobianPoint.
 * @param [out] r Pointer to the destination SM2AffinePoint.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_jpoint_to_apoint(const sm2_jpoint* a, sm2_apoint* r)
{
    sm2_fp t1;
    sm2_fp t2;
    sm2_fp_inv(t1, a->z);
    sm2_fp_sqr(t2, t1);
    sm2_fp_mul(r->x, t2, a->x);
    sm2_fp_mul(t2, t2, t1);
    sm2_fp_mul(r->y, t2, a->y);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Convert an SM2 ciphertext to DER-encoded format.
 * @param ct The ciphertext to be converted.
 * @param dst The output buffer to store the DER-encoded ciphertext.
 * @param dstl The length of the output buffer.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_ciphertext_to_der(const sm2_ciphertext* ct, uint8** dst, uint32* dstl)
{
    uint8* buf  = IStdLib_MemHeapMalloc(Crypto_62_MemPool, 110 + ct->clen);
    uint8* _buf = buf;
    uint32 blen = 0;
    asn1_integer_to_der(ct->C1.x, 32ULL, &_buf, &blen);
    asn1_integer_to_der(ct->C1.y, 32ULL, &_buf, &blen); // finish： blen too large
    asn1_octets_to_der(ct->C3, 32ULL, &_buf, &blen);
    asn1_octets_to_der(ct->C2, (uint32)ct->clen, &_buf, &blen);
    asn1_sequence_to_der(buf, blen, dst, dstl);
    IStdLib_MemHeapFree(Crypto_62_MemPool, buf);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Convert a DER-encoded SM2 ciphertext to its original format.
 * @param ct The output buffer to store the converted ciphertext.
 * @param src The DER-encoded ciphertext.
 * @param srcl The length of the DER-encoded ciphertext.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_ciphertext_from_der(sm2_ciphertext* ct, const uint8** src, uint32* srcl)
{
    uint8*       buf    = IStdLib_MemHeapMalloc(Crypto_62_MemPool, *srcl);
    const uint8* _buf   = buf;
    uint32       seqlen = 0;
    uint32       genlen = 0;
    uint32       clen;
    asn1_sequence_from_der(buf, &seqlen, src, srcl);
    asn1_integer_from_der(ct->C1.x, &genlen, &_buf, &seqlen);
    asn1_integer_from_der(ct->C1.y, &genlen, &_buf, &seqlen);
    asn1_octets_from_der(ct->C3, &genlen, &_buf, &seqlen);
    asn1_octets_from_der(ct->C2, &clen, &_buf, &seqlen);
    ct->clen = (uint32)clen;
    IStdLib_MemHeapFree(Crypto_62_MemPool, buf);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Converts a point from SM2Point to SM2AffinePoint.
 * @param [in] src Pointer to the source SM2Point.
 * @param [out] p Pointer to the destination SM2AffinePoint.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_point_to_apoint(const sm2_point* src, sm2_apoint* p)
{
    sm2_bn_to_fp(src->x, p->x);
    sm2_bn_to_fp(src->y, p->y);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Converts a point from SM2AffinePoint to SM2Point.
 * @param [in] p Pointer to the source SM2AffinePoint.
 * @param [out] dst Pointer to the destination SM2Point.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_apoint_to_point(const sm2_apoint* p, sm2_point* dst)
{
    sm2_fp_to_bn(p->x, dst->x);
    sm2_fp_to_bn(p->y, dst->y);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Multiplies a scalar with a base affine point G, i.e., r = [k]G.
 * @param [out] r Pointer to the resulting SM2AffinePoint.
 * @param [in] k Scalar used for the generation.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_apoint_gen(sm2_apoint* r, const sm2_fp k)
{
    sm2_jpoint g;
    sm2_jpoint_gen(&g, k);
    sm2_jpoint_to_apoint(&g, r);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Multiplies a scalar with a given affine point, i.e., r = [k]p.
 * @param [out] r Pointer to the resulting SM2AffinePoint.
 * @param [in] k Scalar for multiplication.
 * @param [in] p Pointer to the SM2AffinePoint to be multiplied.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_apoint_mul(sm2_apoint* r, const sm2_fp k, const sm2_apoint* p)
{
    sm2_jpoint t;
    sm2_apoint_to_jpoint(p, &t);
    sm2_jpoint_mul(&t, k, &t);
    sm2_jpoint_to_apoint(&t, r);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Multiplies two affine points together, i.e., r = [s]G + [t]q.
 * @param [out] r Pointer to the resulting SM2AffinePoint.
 * @param [in] s First scalar.
 * @param [in] t Second scalar.
 * @param [in] q Pointer to the SM2AffinePoint to be multiplied.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_apoint_mutmul(sm2_apoint* r, sm2_fp s, sm2_fp t, const sm2_apoint* q)
{
    sm2_jpoint g;
    sm2_jpoint t1;
    sm2_jpoint t2;
    sm2_jpoint_gen(&t1, s);
    sm2_jpoint_set(&g, q->x, q->y, sm2_one);
    sm2_jpoint_mul(&t2, t, &g);
    sm2_jpoint_add(&g, &t1, &t2);
    sm2_jpoint_to_apoint(&g, r);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Multiplies a scalar(number) with the base point on the sm2 curve, i.e., r = [k]G.
 * @param [out] r Pointer to the resulting SM2Point.
 * @param [in] k Scalar for multiplication.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_point_gen(sm2_point* r, const sm2_fp k)
{
    sm2_apoint p;
    sm2_apoint_gen(&p, k);
    sm2_apoint_to_point(&p, r);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Multiplies a scalar(number) with the given point on the sm2 curve, i.e., r = [k]p.
 * @param [out] r Pointer to the resulting SM2Point.
 * @param [in] k Scalar for multiplication.
 * @param [in] p Pointer to the SM2Point to be multiplied.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_point_mul(sm2_point* r, const sm2_fp k, const sm2_point* p)
{
    sm2_apoint t;
    sm2_point_to_apoint(p, &t);
    sm2_apoint_mul(&t, k, &t);
    sm2_apoint_to_point(&t, r);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Checks if a point is on the elliptic curve.
 * @param [in] r Pointer to the SM2Point to be checked.
 * @return Returns 1 if the point is on the curve, 0 otherwise.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_point_is_on_curve(const sm2_point* r)
{
    sm2_fp x;
    sm2_fp y;
    sm2_fp t1;
    sm2_fp t2;
    sm2_bn_to_fp(r->x, x);
    sm2_bn_to_fp(r->y, y);
    sm2_fp_cub(t1, x);
    sm2_fp_mul(t2, x, sm2_a);
    sm2_fp_add(t1, t1, t2);
    sm2_fp_add(t1, t1, sm2_b);
    sm2_fp_sqr(t2, y);
    return sm2_fp_cmp(t1, t2);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes addition modulo sm2_n, i.e., r = a + b mod sm2_n.
 * @param [out] r The result number.
 * @param [in] a The first number.
 * @param [in] b The second number.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fn_add(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    if (adds(r, a, b))
    {
        sub(r, r, sm2_n);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes subtraction modulo sm2_n, i.e., r = a - b mod sm2_n.
 * @param [out] r The result number.
 * @param [in] a The minuend.
 * @param [in] b The subtrahend.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fn_sub(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    if (subs(r, a, b))
    {
        add(r, r, sm2_n);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes the reduction modulo sm2_n, i.e., r 鈮� a mod sm2_n.
 * @param [out] r The result number.
 * @param [in] a The dnumber to recover from.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fn_rec(sm2_fp r, const sm2_dfp a)
{
    if (a == NULL || r == NULL)
    {
        return;
    }
    // nu = 2^512 / sm2_n = 0x1000000010000000100000001000000018DFC2096FA323C0112AC6361F15149A0
    static const sm2_fp u =
        {0xF15149A0U, 0x12AC6361U, 0xFA323C01U, 0x8DFC2096U, 0x00000001U, 0x00000001U, 0x00000001U, 0x00000001U};
    // b = (a * u) >> 256
    sm2_dfp b;
    (void)IStdLib_MemSet(b, 0, sizeof(sm2_dfp));
    sm2_dfp c;
    (void)IStdLib_MemSet(c, 0, sizeof(sm2_dfp));
    mul(b, a + 8, u);
    const uint32 k = adds(b + 8, b + 8, a + 8);

    // c = b * n
    mul(c, b + 8, sm2_n);
    if (k)
    {
        add(c + 8, c + 8, sm2_n);
    }

    // r = a - c
    if (a[8] - c[8] - subs(r, a, c))
    {
        sub(r, r, sm2_n);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes multiplication modulo sm2_n, i.e., r = a * b mod sm2_n.
 * @param [out] r The result number.
 * @param [in] a The first number.
 * @param [in] b The second number.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fn_mul(sm2_fp r, const sm2_fp a, const sm2_fp b)
{
    sm2_dfp c = {0};
    mul(c, a, b);
    sm2_fn_rec(r, c);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void fn_haf(sm2_fp r, const sm2_fp a)
{
    static const sm2_fp t =
        {0x9CEAA092U, 0xA9DDFA04U, 0x90E30295U, 0xB901EFB5U, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x7FFFFFFFU, 0x7FFFFFFFU};
    if (a[0] & 1)
    {
        haf(r, a);
        add(r, r, t);
    }
    else
    {
        haf(r, a);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Computes multiplicative inverse modulo sm2_n, i.e., r = a^-1 mod sm2_n.
 * @param [out] r The result number.
 * @param [in] q The number to invert.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_fn_inv(sm2_fp r, const sm2_fp q)
{
    sm2_fp u;
    sm2_fp v;
    sm2_fp a = {1};
    sm2_fp c = {0};
    sm2_fp_set(u, q);
    sm2_fp_set(v, sm2_n);
    while (sm2_fp_non_zero(u))
    {
        while (u[0])
        {
            while (sm2_fp_is_even(u))
            {
                haf(u, u);
                fn_haf(a, a);
            }
            while (sm2_fp_is_even(v))
            {
                haf(v, v);
                fn_haf(c, c);
            }
            if (sm2_fp_cmp(u, v))
            {
                sub(u, u, v);
                if (subs(a, a, c))
                {
                    add(a, a, sm2_n);
                }
            }
            else
            {
                sub(v, v, u);
                if (subs(c, c, a))
                {
                    add(c, c, sm2_n);
                }
            }
        }
    }
    sm2_fp_set(r, c);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Checks the validity of a private key.
 * @param prikey Pointer to the private key structure to check.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_prikey_check(sm2_prikey* prikey)
{
    if (sm2_fp_equ(prikey->d, sm2_zero))
    {
        sm2_bn_to_fp(prikey->prikey, prikey->d);
    }
    if (memcmp(prikey->pubkey.x, sm2_zero, 32) == 0 && memcmp(prikey->pubkey.y, sm2_zero, 32) == 0)
    {
        sm2_apoint p;
        sm2_apoint_gen(&p, prikey->d);
        sm2_apoint_to_point(&p, &prikey->pubkey);
    }
    if (sm2_fp_equ(prikey->d_inv, sm2_zero))
    {
        sm2_fn_add(prikey->d_inv, prikey->d, sm2_one);
        sm2_fn_inv(prikey->d_inv, prikey->d_inv);
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Converts a byte array to an SM2 public key.
 * @param pubkey Pointer to the structure where the public key will be stored.
 * @param b Byte array representation of the public key.
 * @param blen Length of the byte array.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_pubkey_from_bytes(sm2_pubkey* pubkey, const uint8* b, const uint32 blen)
{
    switch (blen)
    {
    case 64:
        (void)IStdLib_MemCpy(pubkey, b, 64);
        break;
    case 65:
        (void)IStdLib_MemCpy(pubkey, b + 1, 64);
        break;
    case 33:
        // not support for compressed pubkey
    default:;
    }
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Converts a byte array to an SM2 private key.
 * @param prikey Pointer to the structure where the private key will be stored.
 * @param b Byte array representation of the private key.
 * @param blen Length of the byte array.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_prikey_from_bytes(sm2_prikey* prikey, const uint8* b, const uint32 blen)
{
    (void)IStdLib_MemSet(prikey, 0, sizeof(sm2_prikey));
    switch (blen)
    {
    case 32:
        (void)IStdLib_MemCpy(prikey->prikey, b, 32);
        break;
    case 192:
        (void)IStdLib_MemCpy(prikey->prikey, b, 192);
        break;
    default:;
    }
    sm2_prikey_check(prikey);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Compute a hash.
 *
 * Param-Name[in]      md_alg: Algorithm to use.
 *                      input: Input message to hash.
 *                     ilen: Input length.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The output buffer - must be large enough for md_alg.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
    compute_hash(Crypto_AlgorithmFamilyType md_alg, const unsigned char* input, uint32 ilen, unsigned char* output)
{
    const Crypto_Md_Info_t* md_info;

    md_info = Crypto_Md_info_from_type(md_alg);
    if (md_info == NULL_PTR)
    {
        return E_NOT_OK;
    }

    return Crypto_md(md_info, input, ilen, output);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Derive the SM2 key pair from a given point.
 *
 * Param-Name[in]      p    : The point used for key derivation.
 *                     klen : Length of the key to be derived.
 *
 * Param-Name[in/out]  output: Pointer to store the derived output.
 * Return              Returns 0 on success, or an error code on failure.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_keypair_derive(uint8* output, const sm2_point* p, const uint32 klen)
{
    int          ret = E_NOT_OK;
    uint8        reg[68];
    uint32       ct = 1;
    const uint32 n  = klen >> 5;
    (void)IStdLib_MemCpy(reg, p, 64);

    for (; ct < n + 1; ct++)
    {
        PUT32(reg + 64, ct);
        // sm3(output, reg, 68);//compute_hash, Computes the SM3 digest of a given message.
        ret = compute_hash(CRYPTO_ALGOFAM_SM3, reg, 68, output);
        if (ret != 0)
        {
            //
        }
        output += 32;
    }
    PUT32(reg + 64, ct);
    // sm3(reg, reg, 68);//compute_hash
    ret = compute_hash(CRYPTO_ALGOFAM_SM3, reg, 68, reg);
    if (ret != 0)
    {
        //
    }
    (void)IStdLib_MemCpy(output, reg, klen & 0x1f);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Core function for encrypting a message using SM2.
 *
 * Param-Name[in]      pubkey: Pointer to the public key used for encryption.
 *                     msg    : Pointer to the plaintext message to be encrypted.
 *                     msglen : Length of the plaintext message.
 *
 * Param-Name[in/out]  ciphertext: Pointer to the structure where the ciphertext will be stored.
 *                     ctlen : Pointer to the variable that stores the length of the ciphertext.
 * Return              Returns 0 on success, or an error code on failure.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    sm2_encrypt_core(sm2_ciphertext* ciphertext, const sm2_pubkey* pubkey, const uint8* msg, const uint32 msglen)
{
    Std_ReturnType ret = E_NOT_OK;

    // (1) generate rand k in [1, n - 1]
    sm2_fp k;
    sm2_fp_rand(k, sm2_n);

    // (2) compute C1 = [k]G
    sm2_point_gen(&ciphertext->C1, k);

    // compute c2 = [k]pubkey = (x2, y2)
    sm2_point keypoint;
    sm2_point_mul(&keypoint, k, pubkey);
    sm2_keypair_derive(ciphertext->C2, &keypoint, msglen);
    for (uint32 i = 0; i < msglen; i++)
    {
        ciphertext->C2[i] ^= msg[i];
    }

    // output C3 = Hash(x2 || m || y2)
    // pt2 = keypoint.x||msg||keypoint.y
    uint32 pt2_len = 64 + msglen;
    uint8* pt2     = IStdLib_MemHeapMalloc(Crypto_62_MemPool, pt2_len);
    IStdLib_MemCpy(pt2, keypoint.x, 32);
    IStdLib_MemCpy(pt2 + 32, msg, msglen);
    IStdLib_MemCpy(pt2 + 32 + msglen, keypoint.y, 32);

    // call crypto_sm3 to compute output C3 = Hash(x2 || m || y2)
    ret = compute_hash(CRYPTO_ALGOFAM_SM3, pt2, pt2_len, ciphertext->C3);
    if (ret != 0)
    {
        return ret;
    }

    ciphertext->clen = msglen;
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, pt2);
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Core function for decrypting a ciphertext using SM2.
 * @param dec_pt Pointer to the buffer where the decrypted plaintext will be stored.
 * @param dec_ptlen Length of the plaintext buffer.
 * @param prikey Pointer to the private key used for decryption.
 * @param ciphertext Pointer to the ciphertext to be decrypted.
 * @return Returns 0 on success, or an error code on failure.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    sm2_decrypt_core(uint8* dec_pt, uint32* dec_ptlen, const sm2_prikey* prikey, const sm2_ciphertext* ciphertext)
{
    Std_ReturnType ret = E_NOT_OK;
    // check point on curve
    if (sm2_point_is_on_curve(&ciphertext->C1) == 0)
    {
        return -1;
    }

    // compute c2 = [d] c1
    sm2_point keypoint;
    sm2_point_mul(&keypoint, prikey->d, &ciphertext->C1); //&ciphertext->C1 x y : 0 c1 is point
    // compute plaintext
    sm2_keypair_derive(dec_pt, &keypoint, ciphertext->clen);
    for (uint32 i = 0; i < ciphertext->clen; i++)
    {
        dec_pt[i] ^= ciphertext->C2[i];
    }

    // pt2 = keypoint.x||msg||keypoint.y
    uint32 pt2_len = ciphertext->clen + 64;
    uint8* pt2     = IStdLib_MemHeapMalloc(Crypto_62_MemPool, pt2_len);
    IStdLib_MemCpy(pt2, keypoint.x, 32);
    IStdLib_MemCpy(pt2 + 32, dec_pt, ciphertext->clen);
    IStdLib_MemCpy(pt2 + 32 + ciphertext->clen, keypoint.y, 32);

    // call crypto_sm3 to compute output C3 = Hash(x2 || m || y2)
    sm3_digest compute_C3;
    ret = compute_hash(CRYPTO_ALGOFAM_SM3, pt2, pt2_len, compute_C3);

    // check c3?
    for (int i = 0; i <= 31; i++)
    {
        if (compute_C3[i] != ciphertext->C3[i])
        {
            return E_NOT_OK;
        }
    }
    *dec_ptlen = ciphertext->clen;
    ret        = E_OK;
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function Encrypt a plaintext message using
 *                     the SM2 algorithm.
 *
 * Param-Name[in]      pubkey: Pointer to the public key used for encryption.
 *                     pt    : Pointer to the plaintext message to be encrypted.
 *                     ptlen : Length of the plaintext message.
 *
 * Param-Name[in/out]  ct    : Pointer to the buffer where the ciphertext will be stored.
 *                     ctlen : Pointer to the variable that stores the length of the ciphertext.
 * Return              Returns 0 on success, or an error code on failure.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType sm2_encrypt(const sm2_pubkey* pubkey, uint8* ct, uint32* ctlen, const uint8* pt, const uint32 ptlen)
{
    Std_ReturnType ret = E_NOT_OK;
    sm2_ciphertext ciphertext;
    ciphertext.C2 = IStdLib_MemHeapMalloc(Crypto_62_MemPool, ptlen);
    ret           = sm2_encrypt_core(&ciphertext, pubkey, pt, ptlen);
    sm2_ciphertext_to_der(&ciphertext, &ct, ctlen);
    IStdLib_MemHeapFree(Crypto_62_MemPool, ciphertext.C2);
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Decrypt a ciphertext using the SM2 algorithm.
 * @param prikey Pointer to the private key used for decryption.
 * @param pt Pointer to the buffer where the decrypted plaintext will be stored.
 * @param ptlen Pointer to the variable that stores the length of the plaintext.
 * @param ct Pointer to the ciphertext to be decrypted.
 * @param ctlen Length of the ciphertext.
 * @return Returns 0 on success, or an error code on failure.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_decrypt(const sm2_prikey* prikey, uint8* pt, uint32* ptlen, const void* ct, uint32 ctlen)
{
    int            ret = 1;
    sm2_ciphertext ciphertext;
    const uint8*   _ct = ct;
    ciphertext.C2      = IStdLib_MemHeapMalloc(Crypto_62_MemPool, ctlen - 96);
    sm2_ciphertext_from_der(&ciphertext, &_ct, &ctlen);
    ret &= sm2_decrypt_core(pt, ptlen, prikey, &ciphertext);
    IStdLib_MemHeapFree(Crypto_62_MemPool, ciphertext.C2);
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function do the SM2 Encrypt process and output cipher text.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *                     secondaryFam: familly for the hash
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_SM2_EncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret = E_NOT_OK;

    (void)mode;
    (void)secondaryFam;

    // input
    uint8* sm2_plaintext    = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint32 sm2_plaintextLen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;

    // output
    uint32 sm2_ctLen      = 0;
    uint8* sm2_ciphertext = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr);

    // publickey
    sm2_pubkey pubkey;
    uint8      keybuf[SM2_KEY_LEN];
    uint32     Keylen = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    (void)IStdLib_MemSet(keybuf, 0, SM2_KEY_LEN);
    // get public key
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, keybuf, &Keylen);

    if (ret == E_NOT_OK)
    {
        return ret;
    }
    sm2_pubkey_from_bytes(&pubkey, keybuf, Keylen);

    // do encrypt
    ret = sm2_encrypt(&pubkey, sm2_ciphertext, &sm2_ctLen, sm2_plaintext, sm2_plaintextLen);

    if (E_OK == ret)
    {
        *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr) = sm2_ctLen;
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function do the SM2 Decrypt process and output cipher text.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *                     secondaryFam: familly for the hash
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_SM2_DecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret = E_NOT_OK;
    sm2_prikey     prikey;
    // Crypto_sm2_context sm2;
    (void)secondaryFam;
    (void)mode;

    // input
    uint8* ciphertext_der    = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint32 ciphertext_derLen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;

    uint8*  dec_plaintext = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr);
    uint32* dec_outLen    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    if (*dec_outLen < ciphertext_derLen - 97)
    {
        goto exit; // return ct initialized length not enough
    }

    // key
    uint32 keylength = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint8* key;
    key = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, keylength);
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &keylength);

    if (ret == E_OK)
    {
        sm2_prikey_from_bytes(&prikey, key, 32);
    }
    if (ret == E_OK)
    {
        ret = sm2_decrypt(&prikey, dec_plaintext, (uint32*)dec_outLen, ciphertext_der, ciphertext_derLen);
    }

    (void)IStdLib_MemSet(key, 0, keylength);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
exit:
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Convert an SM2 signature to DER-encoded format.
 * @param sig The signature to be converted.
 * @param dst The output buffer where the DER-encoded signature will be stored.
 * @param dstl The length of the output buffer.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_sig_to_der(const sm2_signature* sig, uint8** dst, uint32* dstl)
{
    uint8  buf[Sign_sequence_max_size];
    uint8* _buf = buf;
    uint32 blen = 0;
    asn1_integer_to_der(sig->r, (uint32)32, &_buf, &blen);
    asn1_integer_to_der(sig->s, (uint32)32, &_buf, &blen);
    asn1_sequence_to_der(buf, blen, dst, dstl);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/**
 * @brief Convert a DER-encoded SM2 signature to its original format.
 * @param sig Pointer to store the original signature.
 * @param src Pointer to the DER-encoded signature.
 * @param srcl Length of the DER-encoded signature.
 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_sig_from_der(sm2_signature* sig, const uint8** src, uint32* srcl)
{
    uint8        buf[Sign_sequence_max_size];
    const uint8* _buf = buf;
    uint32       blen = 0;
    uint32       rlen;
    uint32       slen;
    asn1_sequence_from_der(buf, &blen, src, srcl);
    asn1_integer_from_der(sig->r, &rlen, &_buf, &blen);
    asn1_integer_from_der(sig->s, &slen, &_buf, &blen);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static void sm2_compute_z(sm3_digest z, const sm2_pubkey* pubkey, const char* id, const uint32 idlen)
{
    static uint8 data[128] = {// a || b || Gx || Gy
                              0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                              0xff, 0xfc, 0x28, 0xe9, 0xfa, 0x9e, 0x9d, 0x9f, 0x5e, 0x34, 0x4d, 0x5a, 0x9e, 0x4b, 0xcf,
                              0x65, 0x09, 0xa7, 0xf3, 0x97, 0x89, 0xf5, 0x15, 0xab, 0x8f, 0x92, 0xdd, 0xbc, 0xbd, 0x41,
                              0x4d, 0x94, 0x0e, 0x93, 0x32, 0xc4, 0xae, 0x2c, 0x1f, 0x19, 0x81, 0x19, 0x5f, 0x99, 0x04,
                              0x46, 0x6a, 0x39, 0xc9, 0x94, 0x8f, 0xe3, 0x0b, 0xbf, 0xf2, 0x66, 0x0b, 0xe1, 0x71, 0x5a,
                              0x45, 0x89, 0x33, 0x4c, 0x74, 0xc7, 0xbc, 0x37, 0x36, 0xa2, 0xf4, 0xf6, 0x77, 0x9c, 0x59,
                              0xbd, 0xce, 0xe3, 0x6b, 0x69, 0x21, 0x53, 0xd0, 0xa9, 0x87, 0x7c, 0xc6, 0x2a, 0x47, 0x40,
                              0x02, 0xdf, 0x32, 0xe5, 0x21, 0x39, 0xf0, 0xa0};

    // compute Hash(entl || id || a || b || Gx || Gy || Px || Py)
    uint8  msg0      = (uint8)idlen >> 5;
    uint8  entl      = (uint8)idlen << 3;
    uint32 input_len = 210;
    uint8  input[210];
    IStdLib_MemCpy(input, &msg0, 1);
    IStdLib_MemCpy(input + 1, &entl, 1);
    IStdLib_MemCpy(input + 2, id, idlen);
    IStdLib_MemCpy(input + 2 + idlen, data, 128);
    IStdLib_MemCpy(input + 130 + idlen, pubkey->x, 32);
    IStdLib_MemCpy(input + 162 + idlen, pubkey->y, 32);
    compute_hash(CRYPTO_ALGOFAM_SM3, input, input_len, z);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static void sm2_compute_e(sm3_digest e, const sm3_digest z, const uint8* msg, const uint32 msglen)
{
    // e = Hash(Za || M)
    uint32 input_len = Sm3_Digest_Len + msglen;
    uint8* input     = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, input_len);
    IStdLib_MemCpy(input, z, Sm3_Digest_Len);
    IStdLib_MemCpy(input + Sm3_Digest_Len, msg, msglen);
    compute_hash(CRYPTO_ALGOFAM_SM3, input, input_len, e);
    IStdLib_MemHeapFree(Crypto_62_MemPool, input);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Core function for generate an SM2 signature.
 *
 * Param-Name[in]       sig: Pointer to the signature to be verified.
 *                      prikey: Pointer to the private key used for signing.
 *                      dgst: Pointer to the message digest that was signed.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Returns 0 if the signature is valid, or an error code if invalid.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_sign_core(sm2_signature* sig, const sm2_prikey* prikey, sm3_digest dgst)
{
    sm2_fp     r;
    sm2_fp     s;
    sm2_fp     k;
    sm2_apoint kg;
    // compute r, s
    while (1)
    {
        while (1)
        {
            // rand k in [1, n - 1]
            sm2_fp_rand(k, sm2_n);
            // use for test
            sm2_fp_from_str(k, "59276E27D506861A16680F3AD9C02DCCEF3CC1FA3CDBE4CE6D54B80DEAC1BC21");

            // compute [k]g
            sm2_apoint_gen(&kg, k);

            // r = e + x_1 (mod n)
            sm2_bn_to_fp(dgst, r);
            sm2_fn_add(r, r, kg.x);

            // if r == 0 or r + k == n re-generate k
            sm2_fn_add(s, r, k);
            if (sm2_fp_is_zero(r) || sm2_fp_equ(s, k) == 0)
            {
                break;
            }
        }
        // s = ((1 + d)^(-1) * (k - r * d)) mod n  = (k + r)*(1 + d)^-1 - r
        sm2_fn_add(s, k, r);
        sm2_fn_mul(s, s, prikey->d_inv);
        sm2_fn_sub(s, s, r);
        // check s != 0
        if (sm2_fp_non_zero(s))
        {
            break;
        }
    }
    // return r, s
    sm2_fp_to_bn(r, sig->r);
    sm2_fp_to_bn(s, sig->s);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Core function for verifying an SM2 signature.
 *
 * Param-Name[in]       sig: Pointer to the signature to be verified.
 *                      publickey: Pointer to the public key used for verification.
 *                      dgst: Pointer to the message digest that was signed.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Returns 0 if the signature is valid, or an error code if invalid.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_verify_core(const sm2_signature* sig, const sm2_pubkey* publickey, sm3_digest dgst)
{
    sm2_fp     sigr;
    sm2_fp     sigs;
    sm2_fp     t;
    sm2_fp     r;
    sm2_apoint pubkey;
    sm2_apoint p;

    // retrieve public key
    sm2_point_to_apoint(publickey, &pubkey);

    // check r′, s′ ∈[1,n-1]
    sm2_bn_to_fp(sig->r, sigr);
    sm2_bn_to_fp(sig->s, sigs);
    if (sm2_fp_equ(sigr, sm2_zero) || sm2_fp_cmp(sigr, sm2_n) || sm2_fp_equ(sigs, sm2_zero) || sm2_fp_cmp(sigs, sm2_n))
    {
        return 0;
    }

    // compute t = r' + s' mod n
    sm2_fn_add(t, sigr, sigs);
    if (sm2_fp_equ(t, sm2_zero))
    {
        return 0;
    }

    // compute s′G + t'p
    sm2_apoint_mutmul(&p, sigs, t, &pubkey);

    // compute r = (e′ + x_1′) mod n
    sm2_bn_to_fp(dgst, r);
    sm2_fn_add(r, r, p.x);

    // check r = r'?
    if (sm2_fp_equ(r, sigr))
    {
        return 1;
    }
    return 0;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function Sign a message using SM2 and produce a signature.
 *
 * Param-Name[in]       prikey: Pointer to the private key used for signing.
 *                      sig: Pointer to the signature to be verified.
 *                      siglen: Length of the signature.
 *                      id: Identifier used for verification.
 *                      idlen: Length of the identifier.
 *                      msg: Pointer to the signed message.
 *                      msglen: Length of the message.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Returns 0 if the signature is valid, or an error code if invalid.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void sm2_sign(
    const sm2_prikey* prikey,
    uint8*            sig,
    uint32*           siglen,
    const char*       id,
    const uint32      idlen,
    const void*       msg,
    const uint32      msglen)
{
    sm3_digest dgst;
    // compute Za = H ( entl || id || a ||b || Gx || Gy || Px || Py)
    sm2_compute_z(dgst, &prikey->pubkey, id, idlen);
    // compute e = H ( Za || M)
    sm2_compute_e(dgst, dgst, msg, msglen);
    // sign
    sm2_signature signature;
    sm2_sign_core(&signature, prikey, dgst);
    // signature to der
    sm2_sig_to_der(&signature, &sig, siglen);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function Verify a signature using the SM2 algorithm.
 *
 * Param-Name[in]       pubkey: Pointer to the public key used for verification.
 *                      sig: Pointer to the signature to be verified.
 *                      siglen: Length of the signature.
 *                      id: Identifier used for verification.
 *                      idlen: Length of the identifier.
 *                      msg: Pointer to the signed message.
 *                      msglen: Length of the message.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Returns 0 if the signature is valid, or an error code if invalid.
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int sm2_verify(
    const sm2_pubkey* pubkey,
    const uint8*      sig,
    uint32            siglen,
    const char*       id,
    const uint32      idlen,
    const void*       msg,
    const uint32      msglen)
{
    sm3_digest dgst;
    // compute Za = H ( entl || id || a ||b || Gx || Gy || Px || Py)
    sm2_compute_z(dgst, pubkey, id, idlen);
    // compute e = H ( Za || M)
    sm2_compute_e(dgst, dgst, msg, msglen);

    // verify
    sm2_signature signature;
    sm2_sig_from_der(&signature, &sig, &siglen);
    return sm2_verify_core(&signature, pubkey, dgst);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function do the SM2 signature generate process and output signature.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *                     secondaryFam: familly for the hash
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Sm2_SignatureGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret;
    // Declare output signature and len
    uint8  sig[75];
    uint32 siglen = 0;
    // Declare input msg and len
    uint8* msg;
    uint32 msglen = 0;
    (void)secondaryFam;
    (void)mode;

    msglen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    msg    = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    // Declare id
    char   id[]  = "1234567812345678";
    uint32 idlen = strlen(id);
    // Declare private key
    sm2_prikey prikey;
    uint8      kbuf[32];
    uint32     Keylen = 32;
    (void)IStdLib_MemSet(kbuf, 0, 32u);

    // Get private key
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_SIGNATURE_KEY, kbuf, &Keylen);
    if (ret == E_NOT_OK)
    {
        return ret;
    }
    sm2_prikey_from_bytes(&prikey, kbuf, 32);

    // sm2 sign process
    sm2_sign(&prikey, sig, &siglen, id, idlen, msg, msglen);

    if (E_OK == ret)
    {
        for (uint32 i = 0; i < siglen; i++)
        {
            Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr[i] = sig[i];
        }
        *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr) = siglen;
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function do the SM2 signature verify process and output the result.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *                     secondaryFam: familly for the hash
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Sm2_SignatureVerifyProcess(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret          = E_NOT_OK;
    int            VerifyResult = 1;
    // Declare input signature and len
    uint8* sig;
    uint32 siglen;
    (void)secondaryFam;
    (void)mode;

    siglen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.secondaryInputLength;
    sig    = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.secondaryInputPtr);
    // Declare input msg and len
    uint8* msg;
    uint32 msglen;

    msglen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    msg    = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    // Declare id
    char   id[]  = "1234567812345678";
    uint32 idlen = strlen(id);
    // Declare public key
    sm2_pubkey pubkey;
    uint8      keybuf[SM2_KEY_LEN];
    uint32     Keylen = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    (void)IStdLib_MemSet(keybuf, 0, SM2_KEY_LEN);
    // get public key
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_SIGNATURE_KEY, keybuf, &Keylen);
    if (ret == E_NOT_OK)
    {
        return ret;
    }
    sm2_pubkey_from_bytes(&pubkey, keybuf, Keylen);

    // sm2 verify process
    VerifyResult = sm2_verify(&pubkey, sig, siglen, id, idlen, msg, msglen);
    if (VerifyResult)
    {
        *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = E_OK;
    }
    else
    {
        *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = E_NOT_OK;
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif
/* PRQA S 2781,2856,2871,2889,2911,2982,2985,2986,2992,2996,5209,3387 -- */
/* PRQA S 1505,4544,4543,4542,4538,4534,4434,4436,4443,2995,4394,4403 -- */
/* PRQA S 4533,4532,4461,1277,0288,0311,0316,0317,0488,0489,0597,0686 -- */
/* PRQA S 0780,1258,1259,1277,1290,1291,1338,1339,1487,1820,1821,1840 -- */
/* PRQA S 1841,1842,1843,1851,1860,1862,1863,2001,2011,2015,2016,2020 -- */
/* PRQA S 2052,2144,2205,2487,2740,2890,3101,3120,3122,3132,3138,3139 -- */
/* PRQA S 3200,3208,3218,3223,3334,3344,3395,3397,3400,3408,3410,3412 -- */
/* PRQA S 3415,3432,3440,3450,3625,3677,3678,3762,4115 -- */
