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
 **  @file               : Crypto_62_Rsa.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 0311,0314,0316,0488,1339,2016,2023,2024,2052,2105,3102,3103,3208,3326,3345 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1253,0288,0317,0432,0489,0490,0693,0770,0772,0780,0842,1252,1258,1259,1277 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1290,1291,1336,1338,1840,1820,1821,1823,1841,1842,1843,1844,1851,1853,1860 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1861,1863,1880,1881,1890,1891,2001,2015,2205,2106,2140,2462,2463,2472,2487 ++ */ /* VL_QAC_Crypto */
/* PRQA S 2740,3101,3120,3122,3123,3140,3200,3206,3209,3218,3219,3226,3332,3344,3387 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3395,3396,3397,3400,3408,3410,3418,3450,3440,3455,3610,3625,3672,3678,3715 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3717,3762,3769,3772,3779,4115,4116,4393,4394,4403,4404,4413,4414,4423,4432 ++ */ /* VL_QAC_Crypto */
/* PRQA S 4434,4443,4446,4447,4460,4461,4464,4470,4480,4491,4499,4501,4538,4542,4544 ++ */ /* VL_QAC_Crypto */
/* PRQA S 4532,4533,4543,4558,3673,3795,4150,4522,3391,3432,3442,2743,1505,2889,2834 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,2996,2992,2911,1503,2822,2896,2986,2755 ++ */ /* VL_QAC_Crypto */
/* PRQA S 2834,2982,2983,2955,1532,2986,2982,2983,5022,5143,5014,5031,5045,2839,2998 ++ */ /* VL_QAC_Crypto */
/* PRQA S 2995,2997,2871,2990,4825 ++ */                                                   /* VL_QAC_Crypto */

/* PRQA S 6010 EOF */ /* VL_MTR_Crypto_62_STCYC */
/* PRQA S 6020 EOF */ /* VL_MTR_Crypto_62_STLIN */
/* PRQA S 6030 EOF */ /* VL_MTR_Crypto_62_STMIF */
/* PRQA S 6040 EOF */ /* VL_MTR_Crypto_62_STPAR */
/* PRQA S 6050 EOF */ /* VL_MTR_Crypto_62_STST3 */
/* PRQA S 6060 EOF */ /* VL_MTR_Crypto_62_STM19 */
/* PRQA S 6070 EOF */ /* VL_MTR_Crypto_62_STCAL */
/* PRQA S 6080 EOF */ /* VL_MTR_Crypto_62_STPTH */
/* PRQA S 6520 EOF */ /* VL_MTR_Crypto_62_STVAR */
/* PRQA S 6540 EOF */ /* VL_MTR_Crypto_62_STTPP */

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
#include "Crypto_62_Hash.h"
#include "Crypto_62_Rsa.h"

#include "Csm_Cfg.h"
#include "stdlib.h"
#include "string.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definitions =========================================== */
/* Gaps between primes, starting at 3. https://oeis.org/A001223 */
CRYPTO_62_LOCAL const unsigned char small_prime_gaps[] = {
    2U,  2U,  4U, 2U,  4U,  2U,  4U,  6U, 2U,  6U, 4U, 2U,  4U,  6U, 6U, 2U,  6U,  4U,  2U,  6U,  4U,  6U,  8U, 4U,
    2U,  4U,  2U, 4U,  14U, 4U,  6U,  2U, 10U, 2U, 6U, 6U,  4U,  6U, 6U, 2U,  10U, 2U,  4U,  2U,  12U, 12U, 4U, 2U,
    4U,  6U,  2U, 10U, 6U,  6U,  6U,  2U, 6U,  4U, 2U, 10U, 14U, 4U, 2U, 4U,  14U, 6U,  10U, 2U,  4U,  6U,  8U, 6U,
    6U,  4U,  6U, 8U,  4U,  8U,  10U, 2U, 10U, 2U, 6U, 4U,  6U,  8U, 4U, 2U,  4U,  12U, 8U,  4U,  8U,  4U,  6U, 12U,
    2U,  18U, 6U, 10U, 6U,  6U,  2U,  6U, 10U, 6U, 6U, 2U,  6U,  6U, 4U, 2U,  12U, 10U, 2U,  4U,  6U,  6U,  2U, 12U,
    4U,  6U,  8U, 10U, 8U,  10U, 8U,  6U, 6U,  4U, 8U, 6U,  4U,  8U, 4U, 14U, 10U, 12U, 2U,  10U, 2U,  4U,  2U, 10U,
    14U, 4U,  2U, 4U,  14U, 4U,  2U,  4U, 20U, 4U, 8U, 10U, 8U,  4U, 6U, 6U,  14U, 4U,  6U,  6U,  8U,  6U, /*reaches
                                                                                                              997*/
    0U /* the last entry is effectively unused */
};
CRYPTO_62_LOCAL const Crypto_oid_md_alg_t oid_md_alg[] = {

    {
        {CRYPTO_OID_DIGEST_ALG_SHA1, sizeof(CRYPTO_OID_DIGEST_ALG_SHA1) - 1, "id-sha1", "SHA-1"},
        CRYPTO_ALGOFAM_SHA1,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_SHA224, sizeof(CRYPTO_OID_DIGEST_ALG_SHA224) - 1, "id-sha224", "SHA-224"},
        CRYPTO_ALGOFAM_SHA2_224,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_SHA256, sizeof(CRYPTO_OID_DIGEST_ALG_SHA256) - 1, "id-sha256", "SHA-256"},
        CRYPTO_ALGOFAM_SHA2_256,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_SHA384, sizeof(CRYPTO_OID_DIGEST_ALG_SHA384) - 1, "id-sha384", "SHA-384"},
        CRYPTO_ALGOFAM_SHA2_384,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_SHA512, sizeof(CRYPTO_OID_DIGEST_ALG_SHA512) - 1, "id-sha512", "SHA-512"},
        CRYPTO_ALGOFAM_SHA2_512,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_RIPEMD160, sizeof(CRYPTO_OID_DIGEST_ALG_RIPEMD160) - 1, "id-ripemd160", "RIPEMD-160"},
        CRYPTO_ALGOFAM_RIPEMD160,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_SHA3_224, sizeof(CRYPTO_OID_DIGEST_ALG_SHA3_224) - 1, "id-sha3-224", "SHA-3-224"},
        CRYPTO_ALGOFAM_SHA3_224,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_SHA3_256, sizeof(CRYPTO_OID_DIGEST_ALG_SHA3_256) - 1, "id-sha3-256", "SHA-3-256"},
        CRYPTO_ALGOFAM_SHA3_256,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_SHA3_384, sizeof(CRYPTO_OID_DIGEST_ALG_SHA3_384) - 1, "id-sha3-384", "SHA-3-384"},
        CRYPTO_ALGOFAM_SHA3_384,
    },

    {
        {CRYPTO_OID_DIGEST_ALG_SHA3_512, sizeof(CRYPTO_OID_DIGEST_ALG_SHA3_512) - 1, "id-sha3-512", "SHA-3-512"},
        CRYPTO_ALGOFAM_SHA3_512,
    },

    {
        {NULL_PTR, 0u, NULL_PTR, NULL_PTR},
        CRYPTO_ALGOFAM_NOT_SET,
    },
};

/* ========================================== internal function declarations ======================================== */
CRYPTO_62_LOCAL Std_ReturnType myrand(void* rng_state, unsigned char* output, uint32 len);
CRYPTO_62_LOCAL int
    mpi_montmul(Crypto_mpi* A, const Crypto_mpi* B, const Crypto_mpi* N, uint32 mm, const Crypto_mpi* T);

CRYPTO_62_LOCAL void mpi_sub_hlp(uint32 n, uint32* s, uint32* d);

CRYPTO_62_LOCAL int  Crypto_mpi_safe_cond_assign(Crypto_mpi* X, const Crypto_mpi* Y, uint8 assign);
CRYPTO_62_LOCAL int  Crypto_ct_cond_select_sign(uint8 condition, int if1, int if0);
CRYPTO_62_LOCAL void mpi_bigendian_to_host(uint32* const p, uint32 limbs);
CRYPTO_62_LOCAL int  rsa_check_context(const Crypto_rsa_context* ctx, int is_priv, int blinding_needed);
CRYPTO_62_LOCAL int  asn1_write_tagged_int(unsigned char** p, const unsigned char* start, int val, int tag);
CRYPTO_62_LOCAL int
    Crypto_asn1_write_len_and_tag(unsigned char** p, const unsigned char* start, uint32 len, unsigned char tag);
CRYPTO_62_LOCAL int Crypto_oid_get_oid_by_md(Crypto_AlgorithmFamilyType md_alg, const char** oid, uint32* olen);
uint32              Crypto_mpi_core_sub_int(
                 uint32*       X,
                 const uint32* A,
                 uint32        c, /* doubles as carry */
                 uint32        limbs);
uint32 Crypto_mpi_core_sub(uint32* X, const uint32* A, const uint32* B, uint32 limbs);
uint32 Crypto_mpi_core_add(uint32* X, const uint32* A, const uint32* B, uint32 limbs);
uint32 Crypto_mpi_core_mla(uint32* d, uint32 d_len, const uint32* s, uint32 s_len, uint32 b);
uint32 Crypto_mpi_core_check_zero_ct(const uint32* A, uint32 limbs);
uint32 Crypto_mpi_core_montmul_init(const uint32* N);
void Crypto_mpi_core_from_mont_rep(uint32* X, const uint32* A, const uint32* N, uint32 AN_limbs, uint32 mm, uint32* T);
CRYPTO_62_LOCAL void exp_mod_precompute_window(
    const uint32* A,
    const uint32* N,
    uint32        AN_limbs,
    uint32        mm,
    const uint32* RR,
    uint32        welem,
    uint32*       Wtable,
    uint32*       temp);
CRYPTO_62_LOCAL void
    Crypto_mpi_core_ct_uint_table_lookup(uint32* dest, const uint32* table, uint32 limbs, uint32 count, uint32 index);
uint32 Crypto_mpi_core_exp_mod_working_limbs(uint32 AN_limbs, uint32 E_limbs);

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               Write an arbitrary-precision number in ASN.1 format.
 *
 * Param-Name[in]      start: The start of the buffer, for bounds-checking.
 *                     X: The MPI to write. It must be non-negative.
 * Param-Name[in/out]  p: The reference to the current position pointer.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_write_mpi(unsigned char** p, const unsigned char* start, const Crypto_mpi* X)
{
    int    ret = E_NOT_OK;
    uint32 len = 0;

    // Write the MPI
    //
    len = Crypto_mpi_size(X);

    /* DER represents 0 with a sign bit (0=nonnegative) and 7 value bits, not
     * as 0 digits. We need to end up with 020100, not with 0200. */
    if (len == 0)
    {
        len = 1;
    }

    if (*p < start || (uint32)(*p - start) < len)
    {
        return E_NOT_OK;
    }

    (*p) -= len;
    CRYPTO_MPI_CHK(Crypto_mpi_write_binary(X, *p, len));

    // DER format assumes 2s complement for numbers, so the leftmost bit
    // should be 0 for positive numbers and 1 for negative numbers.
    //
    if (X->s == 1 && **p & 0x80U)
    {
        if (*p - start < 1)
        {
            return E_NOT_OK;
        }

        *--(*p) = 0x00U;
        len += 1;
    }

    ret = Crypto_asn1_write_len_and_tag(p, start, len, CRYPTO_ASN1_INTEGER);

cleanup:
    return ret;
}

/******************************************************************************/
/*
 * Brief               Conditionally set a block of memory to zero.
 *
 * Param-Name[in]      condition: Condition to test.
 *                     buf: Pointer to the start of the buffer.
 *                     len: Number of bytes to set to zero.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_ct_zeroize_if(uint64 condition, void* buf, uint32 len)
{
    uint32 mask = (uint32)~condition;
    uint8* p    = (uint8*)buf;
    uint32 i    = 0;

    for (; (i + 4) <= len; i += 4)
    {
        Crypto_put_unaligned_uint32((void*)(p + i), Crypto_Get_Unaligned_Uint32((void*)(p + i)) & mask);
    }

    for (; i < len; i++)
    {
        p[i] = p[i] & mask;
    }
}

/******************************************************************************/
/*
 * Brief               Write a length field in ASN.1 format.
 *
 * Param-Name[in]      start: The start of the buffer, for bounds-checking.
 *                     tag: The length value to write.
 * Param-Name[in/out]  p: The reference to the current position pointer.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_write_len(unsigned char** p, const unsigned char* start, uint32 len)
{
#if SIZE_MAX > 0xFFFFFFFF
    if (len > 0xFFFFFFFF)
    {
        return E_NOT_OK;
    }
#endif

    int required = 1;

    if (len >= 0x80)
    {
        for (uint32 l = len; l != 0; l >>= 8)
        {
            required++;
        }
    }

    if (required > (*p - start))
    {
        return E_NOT_OK;
    }

    do
    {
        *--(*p) = CRYPTO_BYTE_0(len);
        len >>= 8;
    } while (len);

    if (required > 1)
    {
        *--(*p) = (unsigned char)(0x80 + required - 1);
    }

    return required;
}

/******************************************************************************/
/*
 * Brief               Write an ASN.1 tag in ASN.1 format.
 *
 * Param-Name[in]      start: The start of the buffer, for bounds-checking.
 *                     tag: The tag to write.
 * Param-Name[in/out]  p: The reference to the current position pointer.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_write_tag(unsigned char** p, const unsigned char* start, unsigned char tag)
{
    if (*p - start < 1)
    {
        return E_NOT_OK;
    }

    *--(*p) = tag;

    return 1;
}

/******************************************************************************/
/*
 * Brief               Write length and tag in ASN.1 format.
 *
 * Param-Name[in]      start: The beginning to write integer.
 *                     len: The length to be written.
 *                     tag: The tag to be written.
 * Param-Name[in/out]  p: The reference to the current position pointer.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int
    Crypto_asn1_write_len_and_tag(unsigned char** p, const unsigned char* start, uint32 len, unsigned char tag)
{
    int ret = E_NOT_OK;

    CRYPTO_ASN1_CHK_ADD(len, Crypto_asn1_write_len(p, start, len));
    CRYPTO_ASN1_CHK_ADD(len, Crypto_asn1_write_tag(p, start, tag));

    return (int)len;
}

/******************************************************************************/
/*
 * Brief               Write an int tag (#Crypto_ASN1_INTEGER) and value
 *                     in ASN.1 format.
 *
 * Param-Name[in]      start: The start of the buffer, for bounds-checking.
 *                     val: The integer value to write.
 * Param-Name[in/out]  p: The reference to the current position pointer.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_write_int(unsigned char** p, const unsigned char* start, int val)
{
    return asn1_write_tagged_int(p, start, val, CRYPTO_ASN1_INTEGER);
}

/******************************************************************************/
/*
 * Brief               Write integer with tag.
 *
 * Param-Name[in]      start: The start of the buffer, for bounds-checking.
 *                     val: The integer value to write.
 *                     tag: The tag to be written.
 * Param-Name[in/out]  p: The reference to the current position pointer.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int asn1_write_tagged_int(unsigned char** p, const unsigned char* start, int val, int tag)
{
    uint32 len = 0;

    do
    {
        if (*p - start < 1)
        {
            return E_NOT_OK;
        }
        len += 1;
        *--(*p) = val & 0xff;
        val >>= 8;
    } while (val > 0);

    if (**p & 0x80)
    {
        if (*p - start < 1)
        {
            return E_NOT_OK;
        }
        *--(*p) = 0x00;
        len += 1;
    }

    return Crypto_asn1_write_len_and_tag(p, start, len, tag);
}

/******************************************************************************/
/*
 * Brief               Get the length of an ASN.1 element. Updates the pointer
 *                     to immediately behind the length.
 *
 * Param-Name[in]      end: End of data.
 * Param-Name[in/out]  p: On entry, *p points to the first byte of the length,
 *                        i.e. immediately after the tag. On successful completion,
 *                        *p points to the first byte after the length, i.e. the
 *                        first byte of the content. On error, the value of *p is
 *                        undefined.
 * Param-Name[out]     len: On successful completion, *len contains the length
 *                          read from the ASN.1 input.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_get_len(unsigned char** p, const unsigned char* end, uint32* len)
{
    if ((end - *p) < 1)
    {
        return E_NOT_OK;
    }

    if ((**p & 0x80) == 0)
    {
        *len = *(*p)++;
    }
    else
    {
        int n = (**p) & 0x7F;
        if (n == 0 || n > 4)
        {
            return E_NOT_OK;
        }
        if ((end - *p) <= n)
        {
            return E_NOT_OK;
        }
        *len = 0;
        (*p)++;
        while (n--)
        {
            *len = (*len << 8) | **p;
            (*p)++;
        }
    }

    if (*len > (uint32)(end - *p))
    {
        return E_NOT_OK;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Get the tag and length of the element. Check for the
 *                     requested tag. Updates the pointer to immediately behind
 *                     the tag and length.
 *
 * Param-Name[in]      end: End of data.
 *                     tag: The expected tag.
 * Param-Name[in/out]  p: On entry, *p points to the start of the ASN.1 element.
 *                        On successful completion, *p points to the first byte
 *                        after the length, i.e. the first byte of the content.
 *                        On error, the value of *p is undefined.
 * Param-Name[out]     len: On successful completion, *len contains the length
 *                          read from the ASN.1 input.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_get_tag(unsigned char** p, const unsigned char* end, uint32* len, int tag)
{
    if ((end - *p) < 1)
    {
        return E_NOT_OK;
    }

    if (**p != tag)
    {
        return E_NOT_OK;
    }

    (*p)++;

    return Crypto_asn1_get_len(p, end, len);
}

/******************************************************************************/
/*
 * Brief               Retrieve an integer ASN.1 tag and its value. Updates the
 *                     pointer to immediately behind the full tag. And check the
 *                     tag.
 *
 * Param-Name[in]      end: End of data.
 *                     tag: The tag expected.
 * Param-Name[in/out]  p: On entry, *p points to the start of the ASN.1 element.
 *                        On successful completion, *p points to the first byte
 *                        beyond the ASN.1 element. On error, the value of *p is
 *                        undefined.
 * Param-Name[out]     val: On success, the parsed value.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int asn1_get_tagged_int(unsigned char** p, const unsigned char* end, int tag, int* val)
{
    int    ret = E_NOT_OK;
    uint32 len;

    if ((ret = Crypto_asn1_get_tag(p, end, &len, tag)) != 0)
    {
        return ret;
    }

    /*
     * len==0 is malformed (0 must be represented as 020100 for INTEGER,
     * or 0A0100 for ENUMERATED tags
     */
    if (len == 0)
    {
        return E_NOT_OK;
    }
    /* This is a cryptography library. Reject negative integers. */
    if ((**p & 0x80) != 0)
    {
        return E_NOT_OK;
    }

    /* Skip leading zeros. */
    while (len > 0 && **p == 0)
    {
        ++(*p);
        --len;
    }

    /* Reject integers that don't fit in an int. This code assumes that
     * the int type has no padding bit. */
    if (len > sizeof(int))
    {
        return E_NOT_OK;
    }
    if (len == sizeof(int) && (**p & 0x80) != 0)
    {
        return E_NOT_OK;
    }

    *val = 0;
    while (len-- > 0)
    {
        *val = (*val << 8) | **p;
        (*p)++;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Retrieve an integer ASN.1 tag and its value. Updates the
 *                     pointer to immediately behind the full tag.
 *
 * Param-Name[in]      end: End of data.
 * Param-Name[in/out]  p: On entry, *p points to the start of the ASN.1 element.
 *                        On successful completion, *p points to the first byte
 *                        beyond the ASN.1 element. On error, the value of *p is
 *                        undefined.
 * Param-Name[out]     val: On success, the parsed value.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_get_int(unsigned char** p, const unsigned char* end, int* val)
{
    return asn1_get_tagged_int(p, end, CRYPTO_ASN1_INTEGER, val);
}

/******************************************************************************/
/*
 * Brief               Retrieve an enumerated ASN.1 tag and its value. Updates the
 *                     pointer to immediately behind the full tag.
 * Param-Name[in]      end: End of data.
 * Param-Name[in/out]  p: On entry, *p points to the start of the ASN.1 element.
 *                        On successful completion, *p points to the first byte
 *                        beyond the ASN.1 element. On error, the value of *p is
 *                        undefined.
 * Param-Name[out]     val: On success, the parsed value.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_get_enum(unsigned char** p, const unsigned char* end, int* val)
{
    return asn1_get_tagged_int(p, end, CRYPTO_ASN1_ENUMERATED, val);
}

/******************************************************************************/
/*
 * Brief               Retrieve an integer ASN.1 tag and its value. Updates the
 *                     pointer to immediately behind the full tag.
 *
 * Param-Name[in]      end: End of data.
 * Param-Name[in/out]  p: On entry, *p points to the start of the ASN.1 element.
 *                        On successful completion, *p points to the first byte
 *                        beyond the ASN.1 element. On error, the value of *p is
 *                        undefined.
 * Param-Name[out]     X: On success, the parsed value.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_asn1_get_mpi(unsigned char** p, const unsigned char* end, Crypto_mpi* X)
{
    int    ret = E_NOT_OK;
    uint32 len;

    if ((ret = Crypto_asn1_get_tag(p, end, &len, CRYPTO_ASN1_INTEGER)) != 0)
    {
        return ret;
    }

    ret = Crypto_mpi_read_binary(X, *p, len);

    *p += len;

    return ret;
}

/******************************************************************************/
/*
 * Brief               Initialize an MPI context.
 *
 * Param-Name[in]      X: The MPI context to initialize.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_mpi_init(Crypto_mpi* X)
{
    X->s = 1;
    X->n = 0;
    X->p = NULL_PTR;
}

/******************************************************************************/
/*
 * Brief               This function frees the components of an MPI context.
 *
 * Param-Name[in]      X: he MPI context to be cleared.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_mpi_free(Crypto_mpi* X)
{
    if (X == NULL_PTR)
    {
        return;
    }

    if (X->p != NULL_PTR)
    {
        (void)IStdLib_MemSet(X->p, 0, X->n * ciL);
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, X->p);
    }

    X->s = 1;
    X->n = 0;
    X->p = NULL_PTR;
}

/******************************************************************************/
/*
 * Brief               Enlarge an MPI to the specified number of limbs.
 *
 * Param-Name[in]      nblimbs: The target number of limbs.
 * Param-Name[in/out]  X: The MPI to grow. It must be initialized.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_mpi_grow(Crypto_mpi* X, uint32 nblimbs)
{
    uint32* p;

    if (nblimbs > CRYPTO_MPI_MAX_LIMBS)
    {
        return CRYPTO_ERR_MPI_ALLOC_FAILED;
    }

    if (X->n < nblimbs)
    {
        if ((p = (uint32*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, nblimbs, ciL)) == NULL_PTR)
        {
            return CRYPTO_ERR_MPI_ALLOC_FAILED;
        }

        if (X->p != NULL_PTR)
        {
            (void)IStdLib_MemCpy(p, X->p, (X->n) * ciL);
            Crypto_mpi_free(X);
        }

        /* nblimbs fits in n because we ensure that CRYPTO_MPI_MAX_LIMBS
         * fits, and we've checked that nblimbs <= CRYPTO_MPI_MAX_LIMBS. */
        X->n = (unsigned short)nblimbs;
        X->p = p;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               This function resizes an MPI downwards, keeping at least
 *                     the specified number of limbs. If X is smaller than nblimbs,
 *                     it is resized up instead.
 * Param-Name[in]      nblimbs: The minimum number of limbs to keep.
 * Param-Name[in/out]  X: The MPI to shrink. This must point to an initialized MPI.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_mpi_shrink(Crypto_mpi* X, uint32 nblimbs)
{
    uint32* p;
    uint32  i;

    if (nblimbs > CRYPTO_MPI_MAX_LIMBS)
    {
        return CRYPTO_ERR_MPI_ALLOC_FAILED;
    }

    /* Actually resize up if there are currently fewer than nblimbs limbs. */
    if (X->n <= nblimbs)
    {
        return Crypto_mpi_grow(X, nblimbs);
    }
    /* After this point, then X->n > nblimbs and in particular X->n > 0. */

    for (i = X->n - 1; i > 0; i--)
    {
        if (X->p[i] != 0)
        {
            break;
        }
    }
    i++;

    if (i < nblimbs)
    {
        i = nblimbs;
    }
    p = (uint32*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, i, ciL);
    if (p == NULL_PTR)
    {
        return CRYPTO_ERR_MPI_ALLOC_FAILED;
    }

    if (X->p != NULL_PTR)
    {
        (void)IStdLib_MemCpy(p, X->p, i * ciL);
        Crypto_mpi_free(X);
    }

    /* i fits in n because we ensure that CRYPTO_MPI_MAX_LIMBS
     * fits, and we've checked that i <= nblimbs <= CRYPTO_MPI_MAX_LIMBS. */
    X->n = (unsigned short)i;
    X->p = p;

    return 0;
}

/******************************************************************************/
/*
 * Brief               Resize X to have exactly n limbs and set it to 0.
 *
 * Param-Name[in]      limbs: The expected limbs X should be.
 * Param-Name[in/out]  X: The MPI to modify.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int Crypto_mpi_resize_clear(Crypto_mpi* X, uint32 limbs)
{
    if (limbs == 0)
    {
        Crypto_mpi_free(X);
        return 0;
    }
    else if (X->n == limbs)
    {
        (void)IStdLib_MemSet(X->p, 0, limbs * ciL);
        X->s = 1;
        return 0;
    }
    else
    {
        Crypto_mpi_free(X);
        return Crypto_mpi_grow(X, limbs);
    }
}

/******************************************************************************/
/*
 * Brief               Shift some data towards the left inside a buffer.
 *
 * Param-Name[in]      start: Pointer to the start of the buffer.
 *                     total: Total size of the buffer.
 *                     offset: Offset from which to copy total - offset bytes.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_ct_memmove_left(void* start, uint32 total, uint32 offset)
{
    volatile unsigned char* buf = start;
    for (uint32 i = 0; i < total; i++)
    {
        uint64 no_op = Crypto_ct_uint_gt(total - offset, i);
        /* The first `total - offset` passes are a no-op. The last
         * `offset` passes shift the data one byte to the left and
         * zero out the last byte. */
        for (uint32 n = 0; n < total - 1; n++)
        {
            unsigned char current = buf[n];
            unsigned char next    = buf[n + 1];
            buf[n]                = Crypto_ct_uint_if(no_op, current, next);
        }
        buf[total - 1] = Crypto_ct_uint_if_else_0(no_op, buf[total - 1]);
    }
}

/******************************************************************************/
/*
 * Brief               Fast Montgomery initialization.
 *
 * Param-Name[in]      N: The input mpi.
 * Param-Name[in/out]  None
 * Param-Name[out]     mm: The resultant mpi.
 * Return              None
 */
/******************************************************************************/
void mpi_montg_init(uint32* mm, const Crypto_mpi* N)
{
    uint32 x, m0 = N->p[0];
    uint32 i;

    x = m0;
    x += ((m0 + 2) & 4) << 1;

    for (i = biL; i >= 8; i /= 2)
    {
        x *= (2 - (m0 * x));
    }

    *mm = ~x + 1;
}

/******************************************************************************/
/*
 * Brief               Montgomery reduction: A = A * R^-1 mod N.
 *
 * Param-Name[in]      N: The modulus mpi.
 *                    mm: The auxiliary mpi.
 *                     T: The auxiliary mpi.
 * Param-Name[in/out]  A: The resultant mpi.
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int mpi_montred(Crypto_mpi* A, const Crypto_mpi* N, uint32 mm, const Crypto_mpi* T)
{
    uint32     z = 1;
    Crypto_mpi U;

    U.n = U.s = (int)z;
    U.p       = &z;

    return (mpi_montmul(A, &U, N, mm, T));
}

/******************************************************************************/
/*
 * Brief               Helper for Crypto_mpi subtraction.
 *
 * Param-Name[in]
 * Param-Name[in/out]
 * Param-Name[out]
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void mpi_sub_hlp(uint32 n, uint32* s, uint32* d)
{
    uint32 i;
    uint32 c, z;

    for (i = c = 0; i < n; i++, s++, d++)
    {
        z = (*d < c);
        *d -= c;
        c = (*d < *s) + z;
        *d -= *s;
    }

    while (c != 0)
    {
        z = (*d < c);
        *d -= c;
        c = z;
        d++;
    }
}

/******************************************************************************/
/*
 * Brief               Make a copy of an MPI.
 *
 * Param-Name[in]      Y: The source MPI. This must point to an initialized MPI.
 * Param-Name[in/out]  X: The destination MPI. This must point to an initialized MPI.
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_copy(Crypto_mpi* X, const Crypto_mpi* Y)
{
    int    ret = 0;
    uint32 i;

    if (X == Y)
    {
        return 0;
    }

    if (Y->n == 0)
    {
        if (X->n != 0)
        {
            X->s = 1;
            (void)IStdLib_MemSet(X->p, 0, X->n * ciL);
        }
        return 0;
    }

    for (i = Y->n - 1; i > 0; i--)
    {
        if (Y->p[i] != 0)
        {
            break;
        }
    }
    i++;

    X->s = Y->s;

    if (X->n < i)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_grow(X, i));
    }
    else
    {
        (void)IStdLib_MemSet(X->p + i, 0, (X->n - i) * ciL);
    }

    (void)IStdLib_MemCpy(X->p, Y->p, i * ciL);

cleanup:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Swap the contents of two MPIs.
 *
 * Param-Name[in]      None
 * Param-Name[in/out]  X: The first MPI. It must be initialized.
 *                     Y: The second MPI. It must be initialized.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_mpi_swap(Crypto_mpi* X, Crypto_mpi* Y)
{
    Crypto_mpi T;

    (void)IStdLib_MemCpy(&T, X, sizeof(Crypto_mpi));
    (void)IStdLib_MemCpy(X, Y, sizeof(Crypto_mpi));
    (void)IStdLib_MemCpy(Y, &T, sizeof(Crypto_mpi));
}

/******************************************************************************/
/*
 * Brief               Get the absolute value of z.
 *
 * Param-Name[in]      z: The input mpi.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32: The absolute value of z.
 */
/******************************************************************************/
CRYPTO_62_LOCAL inline uint32 mpi_sint_abs(sint32 z)
{
    if (z >= 0)
    {
        return z;
    }
    /* Take care to handle the most negative value (-2^(biL-1)) correctly.
     * A naive -z would have undefined behavior.
     * Write this in a way that makes popular compilers happy (GCC, Clang,
     * MSVC). */
    return (uint32)0 - (uint32)z;
}

/* Convert x to a sign, i.e. to 1, if x is positive, or -1, if x is negative.
 * This looks awkward but generates smaller code than (x < 0 ? -1 : 1) */
#define TO_SIGN(x) ((sint32)(((uint32)x) >> (biL - 1)) * -2 + 1)

/******************************************************************************/
/*
 * Brief               Store integer value in MPI.
 *
 * Param-Name[in]      z: The value to use.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The MPI to set. This must be initialized.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_mpi_lset(Crypto_mpi* X, sint32 z)
{
    int ret = CRYPTO_ERR_ERROR_CORRUPTION_DETECTED;

    CRYPTO_MPI_CHK(Crypto_mpi_grow(X, 1));
    (void)IStdLib_MemSet(X->p, 0, (X->n) * ciL);

    X->p[0] = mpi_sint_abs(z);
    X->s    = TO_SIGN(z);

cleanup:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Get a specific bit from an MPI.
 *
 * Param-Name[in]      X: The MPI to query. This must be initialized.
 *                     pos: Zero-based index of the bit to query.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_get_bit(const Crypto_mpi* X, uint32 pos)
{
    if (X->n * biL <= pos)
    {
        return 0;
    }

    return (X->p[pos / biL] >> (pos % biL)) & 0x01;
}

/******************************************************************************/
/*
 * Brief               Modify a specific bit in an MPI.
 *
 * Param-Name[in]      pos: Zero-based index of the bit to query.
 *                     val: The desired value of bit pos: 0 or 1.
 * Param-Name[in/out]  X: The MPI to modify. This must be initialized.
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_set_bit(Crypto_mpi* X, uint32 pos, unsigned char val)
{
    int    ret = 0;
    uint32 off = pos / biL;
    uint32 idx = pos % biL;

    if (val != 0 && val != 1)
    {
        return CRYPTO_ERR_MPI_BAD_INPUT_DATA;
    }

    if (X->n * biL <= pos)
    {
        if (val == 0)
        {
            return 0;
        }

        CRYPTO_MPI_CHK(Crypto_mpi_grow(X, off + 1));
    }

    X->p[off] &= ~((uint32)0x01 << idx);
    X->p[off] |= (uint32)val << idx;

cleanup:

    return ret;
}

/******************************************************************************/
/**
 * Brief               Return the number of bits of value 0 before the least
 *                     significant bit of value 1.
 * Param-Name[in]      X: The MPI to query.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_mpi_lsb(const Crypto_mpi* X)
{
    uint32 i;

    uint32 count = 0;
    for (i = 0; i < X->n; i++)
    {
        for (uint32 j = 0; j < biL; j++, count++)
        {
            if (((X->p[i] >> j) & 1) != 0)
            {
                return count;
            }
        }
    }
    return 1;
}

/******************************************************************************/
/*
 * Brief               Count leading zero bits in a given integer.
 *
 * Param-Name[in]      a: Integer to count leading zero bits.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_mpi_core_clz(uint32 a)
{
    uint32 j;
    uint32 mask = (uint32)1 << (biL - 1);

    for (j = 0; j < biL; j++)
    {
        if (a & mask)
        {
            break;
        }

        mask >>= 1;
    }

    return j;
}

/******************************************************************************/
/*
 * Brief               Export X into unsigned binary data, big-endian.
 *
 * Param-Name[in]      X: The address of the MPI.
 *                     X_limbs: The number of limbs of X.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The output buffer to export to.
 *                     output_length: The length in bytes of output.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_core_write_be(const uint32* X, uint32 X_limbs, unsigned char* output, uint32 output_length)
{
    uint32         stored_bytes;
    uint32         bytes_to_copy;
    unsigned char* p;

    stored_bytes = X_limbs * ciL;

    if (stored_bytes < output_length)
    {
        /* There is enough space in the output buffer. Write initial
         * null bytes and record the position at which to start
         * writing the significant bytes. In this case, the execution
         * trace of this function does not depend on the value of the
         * number. */
        bytes_to_copy = stored_bytes;
        p             = output + output_length - stored_bytes;
        (void)IStdLib_MemSet(output, 0, output_length - stored_bytes);
    }
    else
    {
        /* The output buffer is smaller than the allocated size of X.
         * However X may fit if its leading bytes are zero. */
        bytes_to_copy = output_length;
        p             = output;
        for (uint32 i = bytes_to_copy; i < stored_bytes; i++)
        {
            if (GET_BYTE(X, i) != 0)
            {
                return E_NOT_OK;
            }
        }
    }

    for (uint32 i = 0; i < bytes_to_copy; i++)
    {
        p[bytes_to_copy - i - 1] = GET_BYTE(X, i);
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Shift an MPI in-place right by a number of bits. Shifting
 *                     by more bits than there are bit positions in X is valid
 *                     and results in setting X to 0. This function's execution
 *                     time depends on the value of count (and of course limbs).
 *
 * Param-Name[in]      limbs: The number of limbs of X. This must be at least 1.
 *                     count: The number of bits to shift by.
 * Param-Name[in/out]  X: The number to shift.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_mpi_core_shift_r(uint32* X, uint32 limbs, uint32 count)
{
    uint32 i, v0, v1;
    uint32 r0 = 0u, r1;

    v0 = count / biL;
    v1 = count & (biL - 1);

    if (v0 > limbs || (v0 == limbs && v1 > 0))
    {
        (void)IStdLib_MemSet(X, 0, limbs * ciL);
        return;
    }

    /*
     * shift by count / limb_size
     */
    if (v0 > 0)
    {
        for (i = 0; i < limbs - v0; i++)
        {
            X[i] = X[i + v0];
        }

        for (; i < limbs; i++)
        {
            X[i] = 0;
        }
    }

    /*
     * shift by count % limb_size
     */
    if (v1 > 0)
    {
        for (i = limbs; i > 0; i--)
        {
            r1 = X[i - 1] << (biL - v1);
            X[i - 1] >>= v1;
            X[i - 1] |= r0;
            r0 = r1;
        }
    }
}

/******************************************************************************/
/*
 * Brief               Shift an MPI in-place left by a number of bits. Shifting
 *                     by more bits than there are bit positions in X will produce
 *                     an unspecified result. This function's execution time
 *                     depends on the value of count (and of course limbs).
 *
 * Param-Name[in]      limbs: The number of limbs of X. This must be at least 1.
 *                     count: The number of bits to shift by.
 * Param-Name[in/out]  X: The number to shift.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_mpi_core_shift_l(uint32* X, uint32 limbs, uint32 count)
{
    uint32 i, v0, v1;
    uint32 r0 = 0u, r1;

    v0 = count / (biL);
    v1 = count & (biL - 1);

    /*
     * shift by count / limb_size
     */
    if (v0 > 0)
    {
        for (i = limbs; i > v0; i--)
        {
            X[i - 1] = X[i - v0 - 1];
        }

        for (; i > 0; i--)
        {
            X[i - 1] = 0;
        }
    }

    /*
     * shift by count % limb_size
     */
    if (v1 > 0)
    {
        for (i = v0; i < limbs; i++)
        {
            r1 = X[i] >> (biL - v1);
            X[i] <<= v1;
            X[i] |= r0;
            r0 = r1;
        }
    }
}
/******************************************************************************/
/*
 * Brief    Multiprecision integer addition
 *
 * Param-Name[in]      A:Addition 1 (multi-precision integer)
 *                     B:Addition 2 (multi-precision integer)
 *                     limbs:Length of the A, B, and X arrays
 * Param-Name[in/out]  None
 * Param-Name[out]     X:Target array for storing the result of the operation
 * Return
 */
/******************************************************************************/
uint32 Crypto_mpi_core_add(uint32* X, const uint32* A, const uint32* B, uint32 limbs)
{
    uint32 c = 0;

    for (uint32 i = 0; i < limbs; i++)
    {
        uint32 t = c + A[i];
        c        = (t < A[i]);
        t += B[i];
        c += (t < B[i]);
        X[i] = t;
    }

    return c;
}
/******************************************************************************/
/*
 * Brief               Return the number of bits up to and including the most
 *                     significant bit of value 1.
 *
 * Param-Name[in]      X: The MPI to query. This must point to an initialized MPI.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_mpi_bitlen(const Crypto_mpi* X)
{
    uint32 i, j;

    if (X->n == 0)
    {
        return (0);
    }

    for (i = X->n - 1; i > 0; i--)
    {
        if (X->p[i] != 0)
        {
            break;
        }
    }

    j = biL - Crypto_mpi_core_clz(X->p[i]);

    return ((i * biL) + j);
}

/******************************************************************************/
/*
 * Brief               This function returns the total size in bytes.
 *
 * Param-Name[in]      X: The mpi.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32: The bit len of X.
 */
/******************************************************************************/
uint32 Crypto_mpi_size(const Crypto_mpi* X)
{
    return (Crypto_mpi_bitlen(X) + 7) >> 3;
}
/*
 * Convert an ASCII character to digit value
 */
CRYPTO_62_LOCAL int mpi_get_digit(uint32* d, int radix, char c)
{
    *d = 255;

    if (c >= 0x30 && c <= 0x39)
    {
        *d = c - 0x30;
    }
    if (c >= 0x41 && c <= 0x46)
    {
        *d = c - 0x37;
    }
    if (c >= 0x61 && c <= 0x66)
    {
        *d = c - 0x57;
    }

    if (*d >= (uint32)radix)
    {
        return CRYPTO_ERR_MPI_INVALID_CHARACTER;
    }

    return 0;
}
/******************************************************************************/
/*
 * Brief               Convert ASCII string to MPI
 *
 * Param-Name[in]      s:Null-terminated string buffer.
 *                     radix: The numeric base of the input string
 * Param-Name[in/out]  None
 * Param-Name[out]     X:The destination MPI. This must point to an initialized MPI.
 * Return              E_NOT_OK or E_OK.
 */
/******************************************************************************/
int Crypto_mpi_read_string(Crypto_mpi* X, int radix, const char* s)
{
    int        ret = CRYPTO_ERR_ERROR_CORRUPTION_DETECTED;
    uint32     i, j, slen, n;
    int        sign = 1;
    uint32     d;
    Crypto_mpi T;

    /*Check whether the value of radix is valid*/
    if (radix < 2 || radix > 16)
    {
        return CRYPTO_ERR_MPI_BAD_INPUT_DATA;
    }

    /*Initialize the temporary variable T*/
    Crypto_mpi_init(&T);

    /*If the input string is empty, release X and return 0.*/
    if (s[0] == 0)
    {
        Crypto_mpi_free(X);
        return 0;
    }

    /*Checks if the first character is a negative sign*/
    if (s[0] == '-')
    {
        ++s;
        sign = -1;
    }

    /*Calculate the length of the input string*/
    slen = strlen(s);

    /*If radix is 16 (hexadecimal), then read the value directly from the string by bit, and store it in X.*/
    if (radix == 16)
    {
        if (slen > (0xffffffffU) >> 2)
        {
            return CRYPTO_ERR_MPI_BAD_INPUT_DATA;
        }

        n = BITS_TO_LIMBS(slen << 2);

        CRYPTO_MPI_CHK(Crypto_mpi_grow(X, n));
        CRYPTO_MPI_CHK(Crypto_mpi_lset(X, 0));

        for (i = slen, j = 0; i > 0; i--, j++)
        {
            CRYPTO_MPI_CHK(mpi_get_digit(&d, radix, s[i - 1]));
            X->p[j / (2 * ciL)] |= d << ((j % (2 * ciL)) << 2);
        }
    }
    /*If radix is not 16, the string is converted to Crypto_mpi by multiplying radix bit by bit and adding the value.*/
    else
    {
        CRYPTO_MPI_CHK(Crypto_mpi_lset(X, 0));

        for (i = 0; i < slen; i++)
        {
            CRYPTO_MPI_CHK(mpi_get_digit(&d, radix, s[i]));
            CRYPTO_MPI_CHK(Crypto_mpi_mul_int(&T, X, radix));
            CRYPTO_MPI_CHK(Crypto_mpi_add_int(X, &T, d));
        }
    }

    /*If the final Crypto_mpi is negative, update X->s to -1.*/
    if (sign < 0 && Crypto_mpi_bitlen(X) != 0)
    {
        X->s = -1;
    }

cleanup:

    Crypto_mpi_free(&T);

    return ret;
}
/******************************************************************************/
/*
 * Brief               Constant-time buffer comparison without branches.
 *
 * Param-Name[in]      a: Pointer to the first buffer.
 *                     b: Pointer to the second buffer.
 *                     n: The number of bytes to compare.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_ct_memcmp(const void* a, const void* b, uint32 n)
{
    uint32 i = 0;
    /*
     * `A` and `B` are cast to volatile to ensure that the compiler
     * generates code that always fully reads both buffers.
     * Otherwise it could generate a test to exit early if `diff` has all
     * bits set early in the loop.
     */
    const volatile unsigned char* A    = (volatile const unsigned char*)a;
    const volatile unsigned char* B    = (volatile const unsigned char*)b;
    uint32                        diff = 0;

    for (; i < n; i++)
    {
        /* Read volatile data in order before computing diff.
         * This avoids IAR compiler warning:
         * 'the order of volatile accesses is undefined ..' */
        unsigned char x = A[i], y = B[i];
        diff |= x ^ y;
    }

    /* The bit-twiddling ensures that when we cast uint32 to int, we are casting
     * a value that is in the range 0..INT_MAX - a value larger than this would
     * result in implementation defined behaviour.
     *
     * This ensures that the value returned by the function is non-zero iff
     * diff is non-zero.
     */

    return (int)((diff & 0xffff) | (diff >> 16));
}

/******************************************************************************/
/*
 * Brief               Import an MPI from unsigned big endian binary data.
 *
 * Param-Name[in]      buf: The input buffer.
 *                     buflen: The length of the input buffer buf in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_read_binary(Crypto_mpi* X, const unsigned char* buf, uint32 buflen)
{
    int    ret = E_NOT_OK;
    uint32 i, j, n;

    for (n = 0; n < buflen; n++)
    {
        if (buf[n] != 0)
        {
            break;
        }
    }

    ret = Crypto_mpi_grow(X, CHARS_TO_LIMBS(buflen - n));
    if (E_OK == ret)
    {
        ret = Crypto_mpi_lset(X, 0);
    }

    if (E_OK == ret)
    {
        for (i = buflen, j = 0; i > n; i--, j++)
        {
            X->p[j / ciL] |= ((uint32)buf[i - 1]) << ((j % ciL) << 3);
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               Export X into unsigned binary data, big endian.
 *                     Always fills the whole buffer, which will start with zeros
 *                     if the number is smaller.
 *
 * Param-Name[in]      X: The source MPI.
 * Param-Name[in/out]  None
 * Param-Name[out]     buf: The output buffer.
 *                     buflen: The size of the output buffer buf in Bytes.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_write_binary(const Crypto_mpi* X, unsigned char* buf, uint32 buflen)
{
    return Crypto_mpi_core_write_be(X->p, X->n, buf, buflen);
}

/******************************************************************************/
/*
 * Brief               Perform a left-shift on an MPI: X <<= count.
 *
 * Param-Name[in]      count: The number of bits to shift by.
 * Param-Name[in/out]  X: The MPI to shift.
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_shift_l(Crypto_mpi* X, uint32 count)
{
    int    ret = CRYPTO_ERR_ERROR_CORRUPTION_DETECTED;
    uint32 i;

    i = Crypto_mpi_bitlen(X) + count;

    if (X->n * biL < i)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_grow(X, BITS_TO_LIMBS(i)));
    }

    ret = 0;

    Crypto_mpi_core_shift_l(X->p, X->n, count);
cleanup:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Perform a right-shift on an MPI: X >>= count.
 *
 * Param-Name[in]      count: The number of bits to shift by.
 * Param-Name[in/out]  X: The MPI to shift.
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_shift_r(Crypto_mpi* X, uint32 count)
{
    if (X->n != 0)
    {
        Crypto_mpi_core_shift_r(X->p, X->n, count);
    }
    return 0;
}

/******************************************************************************/
/*
 * Brief               Compare the absolute values of two MPIs.
 *
 * Param-Name[in]      X: The left-hand MPI.
 *                     Y: The right-hand MPI.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_cmp_abs(const Crypto_mpi* X, const Crypto_mpi* Y)
{
    uint32 i, j;

    for (i = X->n; i > 0; i--)
    {
        if (X->p[i - 1] != 0)
        {
            break;
        }
    }

    for (j = Y->n; j > 0; j--)
    {
        if (Y->p[j - 1] != 0)
        {
            break;
        }
    }

    /* If i == j == 0, i.e. abs(X) == abs(Y),
     * we end up returning 0 at the end of the function. */

    if (i > j)
    {
        return 1;
    }
    if (j > i)
    {
        return -1;
    }

    for (; i > 0; i--)
    {
        if (X->p[i - 1] > Y->p[i - 1])
        {
            return 1;
        }
        if (X->p[i - 1] < Y->p[i - 1])
        {
            return -1;
        }
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Compare two MPIs.
 *
 * Param-Name[in]      X: The left-hand MPI.
 *                     Y: The right-hand MPI.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_cmp_mpi(const Crypto_mpi* X, const Crypto_mpi* Y)
{
    uint32 i, j;

    for (i = X->n; i > 0; i--)
    {
        if (X->p[i - 1] != 0)
        {
            break;
        }
    }

    for (j = Y->n; j > 0; j--)
    {
        if (Y->p[j - 1] != 0)
        {
            break;
        }
    }

    if (i == 0 && j == 0)
    {
        return 0;
    }

    if (i > j)
    {
        return X->s;
    }
    if (j > i)
    {
        return -Y->s;
    }

    if (X->s > 0 && Y->s < 0)
    {
        return 1;
    }
    if (Y->s > 0 && X->s < 0)
    {
        return -1;
    }

    for (; i > 0; i--)
    {
        if (X->p[i - 1] > Y->p[i - 1])
        {
            return X->s;
        }
        if (X->p[i - 1] < Y->p[i - 1])
        {
            return -X->s;
        }
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Helper for Crypto_mpi multiplication.
 *
 * Param-Name[in]
 *
 * Param-Name[in/out]
 * Param-Name[out]
 * Return              None
 */
/******************************************************************************/
void mpi_mul_hlp(uint32 i, uint32* s, uint32* d, uint32 b)
{
    uint32 c = 0u, t = 0;

    for (; i >= 16; i -= 16)
    {
        MULADDC_INIT
        MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE

            MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE
                MULADDC_CORE MULADDC_STOP
    }

    for (; i >= 8; i -= 8)
    {
        MULADDC_INIT
        MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE

            MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_CORE MULADDC_STOP
    }

    for (; i > 0; i--)
    {
        MULADDC_INIT
        MULADDC_CORE
        MULADDC_STOP
    }

    t++;

    do
    {
        *d += c;
        c = (*d < c);
        d++;
    } while (c != 0);
}

/******************************************************************************/
/*
 * Brief               Compare an MPI with an integer.
 *
 * Param-Name[in]      X: The left-hand MPI.
 *                     z: The integer value to compare to.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_cmp_int(const Crypto_mpi* X, sint32 z)
{
    Crypto_mpi Y;
    uint32     p[1];

    *p  = mpi_sint_abs(z);
    Y.s = TO_SIGN(z);
    Y.n = 1;
    Y.p = p;

    return Crypto_mpi_cmp_mpi(X, &Y);
}

/******************************************************************************/
/*
 * Brief               Perform an unsigned addition of MPIs: X = |A| + |B|.
 *
 * Param-Name[in]      A: The first summand.
 *                     B: The second summand.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_add_abs(Crypto_mpi* X, const Crypto_mpi* A, const Crypto_mpi* B)
{
    int ret = CRYPTO_ERR_ERROR_CORRUPTION_DETECTED;

    uint32  j;
    uint32 *p, c;

    if (X == B)
    {
        const Crypto_mpi* T = A;
        A                   = X;
        B                   = T;
    }

    if (X != A)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_copy(X, A));
    }

    /*
     * X should always be positive as a result of unsigned additions.
     */
    X->s = 1;

    for (j = B->n; j > 0; j--)
    {
        if (B->p[j - 1] != 0)
        {
            break;
        }
    }

    /* Exit early to avoid undefined behavior on NULL+0 when X->n == 0
     * and B is 0 (of any size). */
    if (j == 0)
    {
        return 0;
    }

    CRYPTO_MPI_CHK(Crypto_mpi_grow(X, j));

    /* j is the number of non-zero limbs of B. Add those to X. */

    p = X->p;

    c = Crypto_mpi_core_add(p, p, B->p, j);

    p += j;

    /* Now propagate any carry */

    while (c != 0)
    {
        if (j >= X->n)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_grow(X, j + 1));
            p = X->p + j;
        }

        *p += c;
        c = (*p < c);
        j++;
        p++;
    }

cleanup:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Perform an unsigned subtraction of MPIs: X = |A| - |B|.
 *
 * Param-Name[in]      A: The minuend.
 *                     B: The subtrahend.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_sub_abs(Crypto_mpi* X, const Crypto_mpi* A, const Crypto_mpi* B)
{
    int    ret = CRYPTO_ERR_ERROR_CORRUPTION_DETECTED;
    uint32 n;
    uint32 carry;

    for (n = B->n; n > 0; n--)
    {
        if (B->p[n - 1] != 0)
        {
            break;
        }
    }
    if (n > A->n)
    {
        /* B >= (2^ciL)^n > A */
        ret = CRYPTO_ERR_MPI_NEGATIVE_VALUE;
        goto cleanup;
    }

    CRYPTO_MPI_CHK(Crypto_mpi_grow(X, A->n));

    /* Set the high limbs of X to match A. Don't touch the lower limbs
     * because X might be aliased to B, and we must not overwrite the
     * significant digits of B. */
    if (A->n > n && A != X)
    {
        (void)IStdLib_MemCpy(X->p + n, A->p + n, (A->n - n) * ciL);
    }
    if (X->n > A->n)
    {
        (void)IStdLib_MemSet(X->p + A->n, 0, (X->n - A->n) * ciL);
    }

    carry = Crypto_mpi_core_sub(X->p, A->p, B->p, n);
    if (carry != 0)
    {
        /* Propagate the carry through the rest of X. */
        carry = Crypto_mpi_core_sub_int(X->p + n, X->p + n, carry, X->n - n);

        /* If we have further carry/borrow, the result is negative. */
        if (carry != 0)
        {
            ret = CRYPTO_ERR_MPI_NEGATIVE_VALUE;
            goto cleanup;
        }
    }

    /* X should always be positive as a result of unsigned subtractions. */
    X->s = 1;

cleanup:
    return ret;
}

/******************************************************************************/
/*
 * Brief               Common function for signed addition and subtraction.
 *                     Calculate A + B * flip_B where flip_B is 1 or -1.
 *
 * Param-Name[in]      A: The first mpi.
 *                     B: The second mpi.
 *                     flip_B: Can be -1 or 1.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
CRYPTO_62_LOCAL int add_sub_mpi(Crypto_mpi* X, const Crypto_mpi* A, const Crypto_mpi* B, int flip_B)
{
    int ret, s;

    s = A->s;
    if (A->s * B->s * flip_B < 0)
    {
        int cmp = Crypto_mpi_cmp_abs(A, B);
        if (cmp >= 0)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_sub_abs(X, A, B));
            /* If |A| = |B|, the result is 0 and we must set the sign bit
             * to +1 regardless of which of A or B was negative. Otherwise,
             * since |A| > |B|, the sign is the sign of A. */
            X->s = cmp == 0 ? 1 : s;
        }
        else
        {
            CRYPTO_MPI_CHK(Crypto_mpi_sub_abs(X, B, A));
            /* Since |A| < |B|, the sign is the opposite of A. */
            X->s = -s;
        }
    }
    else
    {
        CRYPTO_MPI_CHK(Crypto_mpi_add_abs(X, A, B));
        X->s = s;
    }

cleanup:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Perform a signed addition of MPIs: X = A + B.
 *
 * Param-Name[in]      A: The first summand.
 *                     B: The second summand.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_add_mpi(Crypto_mpi* X, const Crypto_mpi* A, const Crypto_mpi* B)
{
    return add_sub_mpi(X, A, B, 1);
}

/******************************************************************************/
/*
 * Brief               Perform a signed subtraction of MPIs: X = A - B.
 *
 * Param-Name[in]      A: The minuend.
 *                     B: The subtrahend.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_sub_mpi(Crypto_mpi* X, const Crypto_mpi* A, const Crypto_mpi* B)
{
    return add_sub_mpi(X, A, B, -1);
}

/******************************************************************************/
/*
 * Brief               Perform a signed addition of an MPI and an integer: X = A + b.
 *
 * Param-Name[in]      A: The first summand.
 *                     b: The second summand.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_add_int(Crypto_mpi* X, const Crypto_mpi* A, sint32 b)
{
    Crypto_mpi B;
    uint32     p[1];

    p[0] = mpi_sint_abs(b);
    B.s  = TO_SIGN(b);
    B.n  = 1;
    B.p  = p;

    return Crypto_mpi_add_mpi(X, A, &B);
}

/******************************************************************************/
/*
 * Brief               Perform a signed subtraction of an MPI and an integer:
 *                     X = A - b.
 *
 * Param-Name[in]      A: The minuend.
 *                     b: The subtrahend.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_sub_int(Crypto_mpi* X, const Crypto_mpi* A, sint32 b)
{
    Crypto_mpi B;
    uint32     p[1];

    p[0] = mpi_sint_abs(b);
    B.s  = TO_SIGN(b);
    B.n  = 1;
    B.p  = p;

    return Crypto_mpi_sub_mpi(X, A, &B);
}

/******************************************************************************/
/*
 * Brief               Perform a multiplication of two MPIs: X = A * B.
 *
 * Param-Name[in]      A: The first factor.
 *                     B: The second factor.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_mul_mpi(Crypto_mpi* X, const Crypto_mpi* A, const Crypto_mpi* B)
{
    int        ret = CRYPTO_ERR_ERROR_CORRUPTION_DETECTED;
    uint32     i, j;
    Crypto_mpi TA, TB;
    int        result_is_zero = 0;

    Crypto_mpi_init(&TA);
    Crypto_mpi_init(&TB);

    if (X == A)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_copy(&TA, A));
        A = &TA;
    }
    if (X == B)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_copy(&TB, B));
        B = &TB;
    }

    for (i = A->n; i > 0; i--)
    {
        if (A->p[i - 1] != 0)
        {
            break;
        }
    }
    if (i == 0)
    {
        result_is_zero = 1;
    }

    for (j = B->n; j > 0; j--)
    {
        if (B->p[j - 1] != 0)
        {
            break;
        }
    }
    if (j == 0)
    {
        result_is_zero = 1;
    }

    CRYPTO_MPI_CHK(Crypto_mpi_grow(X, i + j));
    CRYPTO_MPI_CHK(Crypto_mpi_lset(X, 0));

    Crypto_mpi_core_mul(X->p, A->p, i, B->p, j);

    /* If the result is 0, we don't shortcut the operation, which reduces
     * but does not eliminate side channels leaking the zero-ness. We do
     * need to take care to set the sign bit properly since the library does
     * not fully support an MPI object with a value of 0 and s == -1. */
    if (result_is_zero)
    {
        X->s = 1;
    }
    else
    {
        X->s = A->s * B->s;
    }

cleanup:

    Crypto_mpi_free(&TB);
    Crypto_mpi_free(&TA);
    return ret;
}

/******************************************************************************/
/*
 * Brief               Perform a multiplication of an MPI with an unsigned integer:
 *                     X = A * b.
 *
 * Param-Name[in]      A: The first factor.
 *                     b: The second factor.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_mul_int(Crypto_mpi* X, const Crypto_mpi* A, uint32 b)
{
    uint32 n = A->n;
    while (n > 0 && A->p[n - 1] == 0)
    {
        --n;
    }

    /* The general method below doesn't work if b==0. */
    if (b == 0 || n == 0)
    {
        return Crypto_mpi_lset(X, 0);
    }

    /* Calculate A*b as A + A*(b-1) to take advantage of Crypto_mpi_core_mla */
    int ret = CRYPTO_ERR_ERROR_CORRUPTION_DETECTED;
    /* In general, A * b requires 1 limb more than b. If
     * A->p[n - 1] * b / b == A->p[n - 1], then A * b fits in the same
     * number of limbs as A and the call to grow() is not required since
     * copy() will take care of the growth if needed. However, experimentally,
     * making the call to grow() unconditional causes slightly fewer
     * calls to calloc() in ECP code, presumably because it reuses the
     * same mpi for a while and this way the mpi is more likely to directly
     * grow to its final size.
     *
     * Note that calculating A*b as 0 + A*b doesn't work as-is because
     * A,X can be the same. */
    CRYPTO_MPI_CHK(Crypto_mpi_grow(X, n + 1));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(X, A));
    Crypto_mpi_core_mla(X->p, X->n, A->p, n, b - 1);

cleanup:
    return ret;
}
/******************************************************************************/
/*
 * Brief     Multiple precision integer multiplication
 *
 * Param-Name[in]   A:Multiplier 1 (multi-precision integer)
 *                  A_limbs:A Length of the array
 *                  B:Multiplier 2 (multi-precision integer)
 *                  B_limbs:B Length of the array
 * Param-Name[in/out]  None
 * Param-Name[out]     X:Target array for storing the result of the operation
 * Return
 */
/******************************************************************************/
void Crypto_mpi_core_mul(uint32* X, const uint32* A, uint32 A_limbs, const uint32* B, uint32 B_limbs)
{
    (void)IStdLib_MemSet(X, 0, (A_limbs + B_limbs) * ciL);

    for (uint32 i = 0; i < B_limbs; i++)
    {
        (void)Crypto_mpi_core_mla(X + i, A_limbs + 1, A, A_limbs, B[i]);
    }
}
/******************************************************************************/
/*
 * Brief               Unsigned integer divide - double  dividend, u1/u0, and
 *                     divisor, d.
 *
 * Param-Name[in]      u1: First dividend.
 *                     u0: Second dividend.
 *                     d: Divisor.
 * Param-Name[in/out]  None
 * Param-Name[out]     r: The destination.
 * Return              uint32
 */
/******************************************************************************/
CRYPTO_62_LOCAL uint32 Crypto_int_div_int(uint32 u1, uint32 u0, uint32 d, uint32* r)
{
#if defined(CRYPTO_HAVE_UDBL)
    Crypto_t_udbl dividend, quotient;
#else
    const uint32 radix              = (uint32)1 << biH;
    const uint32 uint_halfword_mask = ((uint32)1 << biH) - 1;
    uint32       d0, d1, q0, q1, rAX, r0, quotient;
    uint32       u0_msw, u0_lsw;
    uint32       s;
#endif

    /*
     * Check for overflow
     */
    if (0 == d || u1 >= d)
    {
        if (r != NULL_PTR)
        {
            *r = ~(uint32)0u;
        }

        return ~(uint32)0u;
    }

#if defined(CRYPTO_HAVE_UDBL)
    dividend = (Crypto_t_udbl)u1 << biL;
    dividend |= (Crypto_t_udbl)u0;
    quotient = dividend / d;
    if (quotient > ((Crypto_t_udbl)1 << biL) - 1)
    {
        quotient = ((Crypto_t_udbl)1 << biL) - 1;
    }

    if (r != NULL_PTR)
    {
        *r = (uint32)(dividend - (quotient * d));
    }

    return (uint32)quotient;
#else

    /*
     * Algorithm D, Section 4.3.1 - The Art of Computer Programming
     *   Vol. 2 - Seminumerical Algorithms, Knuth
     */

    /*
     * Normalize the divisor, d, and dividend, u0, u1
     */
    s = Crypto_mpi_core_clz(d);
    d = d << s;

    u1 = u1 << s;
    u1 |= (u0 >> (biL - s)) & (-(sint32)s >> (biL - 1));
    u0 = u0 << s;

    d1 = d >> biH;
    d0 = d & uint_halfword_mask;

    u0_msw = u0 >> biH;
    u0_lsw = u0 & uint_halfword_mask;

    /*
     * Find the first quotient and remainder
     */
    q1 = u1 / d1;
    r0 = u1 - d1 * q1;

    while (q1 >= radix || (q1 * d0 > radix * r0 + u0_msw))
    {
        q1 -= 1;
        r0 += d1;

        if (r0 >= radix)
        {
            break;
        }
    }

    rAX = (u1 * radix) + (u0_msw - q1 * d);
    q0  = rAX / d1;
    r0  = rAX - q0 * d1;

    while (q0 >= radix || (q0 * d0 > radix * r0 + u0_lsw))
    {
        q0 -= 1;
        r0 += d1;

        if (r0 >= radix)
        {
            break;
        }
    }

    if (r != NULL_PTR)
    {
        *r = (rAX * radix + u0_lsw - q0 * d) >> s;
    }

    quotient = q1 * radix + q0;

    return quotient;
#endif
}

/******************************************************************************/
/*
 * Brief               Perform a division with remainder of two MPIs: A = Q * B + R.
 * Param-Name[in]      Q: The destination MPI for the quotient.
 *                     R: The destination MPI for the remainder value.
 *                     B: The divisor.
 * Param-Name[in/out]  None
 * Param-Name[out]     A: The dividend.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_div_mpi(Crypto_mpi* Q, Crypto_mpi* R, const Crypto_mpi* A, const Crypto_mpi* B)
{
    int        ret = E_NOT_OK;
    uint32     i, n, t, k;
    Crypto_mpi X, Y, Z, T1, T2;
    uint32     TP2[CRYPTO_CONST_3];

    if (Crypto_mpi_cmp_int(B, 0) == 0)
    {
        return E_NOT_OK;
    }

    Crypto_mpi_init(&X);
    Crypto_mpi_init(&Y);
    Crypto_mpi_init(&Z);
    Crypto_mpi_init(&T1);

    T2.s = 1;
    T2.n = sizeof(TP2) / sizeof(*TP2);
    T2.p = TP2;

    if (Crypto_mpi_cmp_abs(A, B) < 0)
    {
        if (Q != NULL_PTR)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_lset(Q, 0));
        }
        if (R != NULL_PTR)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_copy(R, A));
        }
        return 0;
    }

    CRYPTO_MPI_CHK(Crypto_mpi_copy(&X, A));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&Y, B));
    X.s = Y.s = 1;

    CRYPTO_MPI_CHK(Crypto_mpi_grow(&Z, A->n + 2));
    CRYPTO_MPI_CHK(Crypto_mpi_lset(&Z, 0));
    CRYPTO_MPI_CHK(Crypto_mpi_grow(&T1, A->n + 2));

    k = Crypto_mpi_bitlen(&Y) % biL;
    if (k < biL - 1)
    {
        k = biL - 1 - k;
        CRYPTO_MPI_CHK(Crypto_mpi_shift_l(&X, k));
        CRYPTO_MPI_CHK(Crypto_mpi_shift_l(&Y, k));
    }
    else
    {
        k = 0;
    }

    n = X.n - 1;
    t = Y.n - 1;
    CRYPTO_MPI_CHK(Crypto_mpi_shift_l(&Y, biL * (n - t)));

    while (Crypto_mpi_cmp_mpi(&X, &Y) >= 0)
    {
        Z.p[n - t]++;
        CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&X, &X, &Y));
    }
    CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&Y, biL * (n - t)));

    for (i = n; i > t; i--)
    {
        if (X.p[i] >= Y.p[t])
        {
            Z.p[i - t - 1] = ~(uint32)0u;
        }
        else
        {
            Z.p[i - t - 1] = Crypto_int_div_int(X.p[i], X.p[i - 1], Y.p[t], NULL_PTR);
        }
        T2.p[0] = (i < 2) ? 0 : X.p[i - 2];
        T2.p[1] = (i < 1) ? 0 : X.p[i - 1];
        T2.p[2] = X.p[i];

        Z.p[i - t - 1]++;
        do
        {
            Z.p[i - t - 1]--;

            CRYPTO_MPI_CHK(Crypto_mpi_lset(&T1, 0));
            T1.p[0] = (t < 1) ? 0 : Y.p[t - 1];
            T1.p[1] = Y.p[t];
            CRYPTO_MPI_CHK(Crypto_mpi_mul_int(&T1, &T1, Z.p[i - t - 1]));

        } while (Crypto_mpi_cmp_mpi(&T1, &T2) > 0);

        CRYPTO_MPI_CHK(Crypto_mpi_mul_int(&T1, &Y, Z.p[i - t - 1]));
        CRYPTO_MPI_CHK(Crypto_mpi_shift_l(&T1, biL * (i - t - 1)));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&X, &X, &T1));

        if (Crypto_mpi_cmp_int(&X, 0) < 0)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_copy(&T1, &Y));
            CRYPTO_MPI_CHK(Crypto_mpi_shift_l(&T1, biL * (i - t - 1)));
            CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(&X, &X, &T1));
            Z.p[i - t - 1]--;
        }
    }

    if (Q != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_copy(Q, &Z));
        Q->s = A->s * B->s;
    }

    if (R != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&X, k));
        X.s = A->s;
        CRYPTO_MPI_CHK(Crypto_mpi_copy(R, &X));

        if (Crypto_mpi_cmp_int(R, 0) == 0)
        {
            R->s = 1;
        }
    }

cleanup:

    Crypto_mpi_free(&X);
    Crypto_mpi_free(&Y);
    Crypto_mpi_free(&Z);
    Crypto_mpi_free(&T1);
    (void)IStdLib_MemSet(&TP2, 0, CRYPTO_CONST_3);
    return ret;
}

/******************************************************************************/
/*
 * Brief               Perform a division with remainder of an MPI by an integer:
 *                     A = Q * b + R.
 * Param-Name[in]      Q: The destination MPI for the quotient.
 *                     R: The destination MPI for the remainder value.
 *                     b: The divisor.
 * Param-Name[in/out]  None
 * Param-Name[out]     A: The dividend.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_div_int(Crypto_mpi* Q, Crypto_mpi* R, const Crypto_mpi* A, sint32 b)
{
    Crypto_mpi B;
    uint32     p[1];

    p[0] = mpi_sint_abs(b);
    B.s  = TO_SIGN(b);
    B.n  = 1;
    B.p  = p;

    return Crypto_mpi_div_mpi(Q, R, A, &B);
}

/******************************************************************************/
/*
 * Brief               Perform a modular reduction. R = A mod B.
 * Param-Name[in]      A: The MPI to compute the residue of.
 *                     B: The base of the modular reduction.
 * Param-Name[in/out]  None
 * Param-Name[out]     R: The destination MPI for the residue value.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_mod_mpi(Crypto_mpi* R, const Crypto_mpi* A, const Crypto_mpi* B)
{
    int ret = E_NOT_OK;

    if (Crypto_mpi_cmp_int(B, 0) < 0)
    {
        return E_NOT_OK;
    }

    CRYPTO_MPI_CHK(Crypto_mpi_div_mpi(NULL_PTR, R, A, B));

    while (Crypto_mpi_cmp_int(R, 0) < 0)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(R, R, B));
    }

    while (Crypto_mpi_cmp_mpi(R, B) >= 0)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(R, R, B));
    }

cleanup:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Perform a modular reduction with respect to an integer.
 *                     r = A mod b.
 * Param-Name[in]      A: The MPI to compute the residue of.
 *                     b: The integer base of the modular reduction.
 * Param-Name[in/out]  None
 * Param-Name[out]     r: The address at which to store the residue.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_mod_int(uint32* r, const Crypto_mpi* A, sint32 b)
{
    uint32 i;
    uint32 x, y, z;

    if (b == 0)
    {
        return E_NOT_OK;
    }

    if (b < 0)
    {
        return E_NOT_OK;
    }

    /*
     * handle trivial cases
     */
    if (b == 1 || A->n == 0)
    {
        *r = 0;
        return 0;
    }

    if (b == 2)
    {
        *r = A->p[0] & 1;
        return 0;
    }

    /*
     * general case
     */
    for (i = A->n, y = 0; i > 0; i--)
    {
        x = A->p[i - 1];
        y = (y << biH) | (x >> biH);
        z = y / b;
        y -= z * b;

        x <<= biH;
        y = (y << biH) | (x >> biH);
        z = y / b;
        y -= z * b;
    }

    /*
     * If A is negative, then the current y represents a negative value.
     * Flipping it to the positive side.
     */
    if (A->s < 0 && y != 0)
    {
        y = b - y;
    }

    *r = y;

    return 0;
}

/******************************************************************************/
/*
 * Brief               Perform a sliding-window exponentiation: X = A^E mod N.
 * Param-Name[in]      A: The base of the exponentiation.
 *                     E: The exponent MPI.
 *                     N: The base for the modular reduction.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI. This must point to an initialized MPI.
 *                        This must not alias E or N.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_exp_mod(
    Crypto_mpi*       X,
    const Crypto_mpi* A,
    const Crypto_mpi* E,
    const Crypto_mpi* N,
    Crypto_mpi*       prec_RR)
{
    int ret = E_NOT_OK;

    if (Crypto_mpi_cmp_int(N, 0) <= 0 || (N->p[0] & 1) == 0)
    {
        return E_NOT_OK;
    }

    if (Crypto_mpi_cmp_int(E, 0) < 0)
    {
        return E_NOT_OK;
    }

    if (Crypto_mpi_bitlen(E) > CRYPTO_MPI_MAX_BITS || Crypto_mpi_bitlen(N) > CRYPTO_MPI_MAX_BITS)
    {
        return E_NOT_OK;
    }

    /*
     * Ensure that the exponent that we are passing to the core is not NULL.
     */
    if (E->n == 0)
    {
        ret = Crypto_mpi_lset(X, 1);
        return ret;
    }

    /*
     * Allocate working memory for Crypto_mpi_core_exp_mod()
     */
    uint32 T_limbs = Crypto_mpi_core_exp_mod_working_limbs(N->n, E->n);

    uint32* T = (uint32*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, sizeof(uint32), T_limbs);
    if (T == NULL_PTR)
    {
        return E_NOT_OK;
    }

    Crypto_mpi RR;
    Crypto_mpi_init(&RR);

    /*
     * If 1st call, pre-compute R^2 mod N
     */
    if (prec_RR == NULL_PTR || prec_RR->p == NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_core_get_mont_r2_unsafe(&RR, N));

        if (prec_RR != NULL_PTR)
        {
            *prec_RR = RR;
        }
    }
    else
    {
        CRYPTO_MPI_CHK(Crypto_mpi_grow(prec_RR, N->n));
        RR = *prec_RR;
    }

    /*
     * To preserve constness we need to make a copy of A. Using X for this to
     * save memory.
     */
    CRYPTO_MPI_CHK(Crypto_mpi_copy(X, A));

    /*
     * Compensate for negative A (and correct at the end).
     */
    X->s = 1;

    /*
     * Make sure that X is in a form that is safe for consumption by
     * the core functions.
     *
     * - The core functions will not touch the limbs of X above N->n. The
     *   result will be correct if those limbs are 0, which the mod call
     *   ensures.
     * - Also, X must have at least as many limbs as N for the calls to the
     *   core functions.
     */
    if (Crypto_mpi_cmp_mpi(X, N) >= 0)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(X, X, N));
    }
    CRYPTO_MPI_CHK(Crypto_mpi_grow(X, N->n));

    /*
     * Convert to and from Montgomery around Crypto_mpi_core_exp_mod().
     */
    {
        uint32 mm = Crypto_mpi_core_montmul_init(N->p);
        Crypto_mpi_core_to_mont_rep(X->p, X->p, N->p, N->n, mm, RR.p, T);
        Crypto_mpi_core_exp_mod(X->p, X->p, N->p, N->n, E->p, E->n, RR.p, T);
        Crypto_mpi_core_from_mont_rep(X->p, X->p, N->p, N->n, mm, T);
    }

    /*
     * Correct for negative A.
     */
    if (A->s == -1 && (E->p[0] & 1) != 0)
    {
        uint32 is_x_non_zero = Crypto_mpi_core_check_zero_ct(X->p, X->n);
        X->s                 = Crypto_ct_uint_if(is_x_non_zero, 0, 2) - 1;

        CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(X, N, X));
    }

cleanup:

    (void)IStdLib_MemSet(T, 0, T_limbs * sizeof(*T));
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, T);

    if (prec_RR == NULL_PTR || prec_RR->p == NULL_PTR)
    {
        Crypto_mpi_free(&RR);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Montgomery multiplication: A = A * B * R^-1 mod N.
 * Param-Name[in]      B: The input mpi.
 *                     R: The second input mpi.
 *                     N: The input modulus.
 * Param-Name[in/out]  A: The destination MPI.
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
CRYPTO_62_LOCAL int mpi_montmul(Crypto_mpi* A, const Crypto_mpi* B, const Crypto_mpi* N, uint32 mm, const Crypto_mpi* T)
{
    uint32 i, n, m;
    uint32 u0, u1, *d;

    (void)IStdLib_MemSet(T->p, 0, T->n * ciL);

    d = T->p;
    n = N->n;
    m = (B->n < n) ? B->n : n;

    for (i = 0; i < n; i++)
    {
        /*
         * T = (T + u0*B + u1*N) / 2^biL3
         */
        u0 = A->p[i];
        u1 = (d[0] + u0 * B->p[0]) * mm;

        mpi_mul_hlp(m, B->p, d, u0);
        mpi_mul_hlp(n, N->p, d, u1);

        *d++     = u0;
        d[n + 1] = 0;
    }

    (void)IStdLib_MemCpy(A->p, d, (n + 1) * ciL);

    if (Crypto_mpi_cmp_abs(A, N) >= 0)
    {
        mpi_sub_hlp(n, N->p, A->p);
    }
    else
    {
        /* prevent timing attacks */
        mpi_sub_hlp(n, A->p, T->p);
    }

    return (0);
}

/******************************************************************************/
/*
 * Brief               Testify whether x is equal to y ot not.
 *
 * Param-Name[in]      x: The input mpi.
 *                     y: The second input mpi.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              unsigned
 */
/******************************************************************************/
unsigned Crypto_ct_size_bool_eq(uint32 x, uint32 y)
{
    /* diff = 0 if x == y, non-zero otherwise */
    const uint32 diff = x ^ y;

    /* diff_msb's most significant bit is equal to x != y */
    const uint32 diff_msb = (diff | (uint32)-diff);

    /* diff1 = (x != y) ? 1 : 0 */
    const unsigned diff1 = diff_msb >> (sizeof(diff_msb) * 8 - 1);

    return (1 ^ diff1);
}

/******************************************************************************/
/*
 * Brief  Generate a mask for implementing constant time conditional judgments
 *
 * Param-Name[in]
 * Param-Name[in/out]
 * Param-Name[out]
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_ct_mpi_uint_mask(uint32 value)
{
    return (-((value | -value) >> (sizeof(value) * 8 - 1)));
}

/******************************************************************************/
/*
 * Brief  Conditionally assigns an array from the source array src to the destination array dest
 *
 * Param-Name[in]
 * Param-Name[in/out]
 * Param-Name[out]
 * Return              None
 */
/******************************************************************************/
void Crypto_ct_mpi_uint_cond_assign(uint32 n, uint32* dest, const uint32* src, uint8 condition)
{
    uint32 i;

    /* all-bits 1 if condition is 1, all-bits 0 if condition is 0 */
    const uint32 mask = -condition;

    for (i = 0; i < n; i++)
    {
        dest[i] = (src[i] & mask) | (dest[i] & ~mask);
    }
}

/******************************************************************************/
/*
 * Brief
 *
 * Param-Name[in]
 * Param-Name[in/out]
 * Param-Name[out]
 * Return              int
 */
/******************************************************************************/
CRYPTO_62_LOCAL int Crypto_ct_cond_select_sign(uint8 condition, int if1, int if0)
{
    /* In order to avoid questions about what we can reasonably assume about
     * the representations of signed integers, move everything to unsigned
     * by taking advantage of the fact that if1 and if0 are either +1 or -1. */
    unsigned uif1 = if1 + 1;
    unsigned uif0 = if0 + 1;

    /* condition was 0 or 1, mask is 0 or 2 as are uif1 and uif0 */
    const unsigned mask = condition << 1;

    /* select uif1 or uif0 */
    unsigned ur = (uif0 & ~mask) | (uif1 & mask);

    /* ur is now 0 or 2, convert back to -1 or +1 */
    return ((int)ur - 1);
}

/******************************************************************************/
/*
 * Brief               Perform a safe conditional copy of MPI which doesn't
 *                     reveal whether the condition was true or not.
 * Param-Name[in]      Y: The MPI to be assigned from.
 *                     assign: The condition deciding whether to perform the
 *                             assignment or not. Must be either 0 or 1:
 *                             1: Perform the assignment `X = Y`.
 *                             0: Keep the original value of X.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The MPI to conditionally assign to.
 * Return              int
 */
/******************************************************************************/
CRYPTO_62_LOCAL int Crypto_mpi_safe_cond_assign(Crypto_mpi* X, const Crypto_mpi* Y, uint8 assign)
{
    int    ret = 0;
    uint32 i;
    uint32 limb_mask;

    /* all-bits 1 if assign is 1, all-bits 0 if assign is 0 */
    limb_mask = Crypto_ct_mpi_uint_mask(assign);
    ;

    CRYPTO_MPI_CHK(Crypto_mpi_grow(X, Y->n));

    X->s = Crypto_ct_cond_select_sign(assign, Y->s, X->s);

    Crypto_ct_mpi_uint_cond_assign(Y->n, X->p, Y->p, assign);

    for (i = Y->n; i < X->n; i++)
    {
        X->p[i] &= ~limb_mask;
    }

cleanup:
    return (ret);
}

/******************************************************************************/
/*
 * Brief               Perform a safe conditional copy of MPI which doesn't
 *                     reveal whether the condition was true or not
 *
 * Param-Name[in]      R: The MPI to conditionally assign to.
 *                     T: The MPI to be assigned from.
 *                     size,idx  The condition deciding whether to perform the
 *                             assignment or not. Must be either 0 or 1:
 *                             1: Perform the assignment `X = Y`.
 *                             0: Keep the original value of X.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int mpi_select(Crypto_mpi* R, const Crypto_mpi* T, uint32 T_size, uint32 idx)
{
    int    ret = E_NOT_OK;
    uint32 i;

    for (i = 0; i < T_size; i++)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_safe_cond_assign(R, &T[i], (uint8)Crypto_ct_size_bool_eq(i, idx)));
    }

cleanup:
    return (ret);
}

/******************************************************************************/
/*
 * Brief               Compute the greatest common divisor: G = gcd(A, B).
 * Param-Name[in]      A: The first operand.
 *                     B: The second operand.
 * Param-Name[in/out]  None
 * Param-Name[out]     G: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_gcd(Crypto_mpi* G, const Crypto_mpi* A, const Crypto_mpi* B)
{
    int        ret = E_NOT_OK;
    uint32     lz, lzt;
    Crypto_mpi TA, TB;

    Crypto_mpi_init(&TA);
    Crypto_mpi_init(&TB);

    CRYPTO_MPI_CHK(Crypto_mpi_copy(&TA, A));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&TB, B));

    lz  = Crypto_mpi_lsb(&TA);
    lzt = Crypto_mpi_lsb(&TB);

    /* The loop below gives the correct result when A==0 but not when B==0.
     * So have a special case for B==0. Leverage the fact that we just
     * calculated the lsb and lsb(B)==0 iff B is odd or 0 to make the test
     * slightly more efficient than cmp_int(). */
    if (lzt == 0 && Crypto_mpi_get_bit(&TB, 0) == 0)
    {
        ret = Crypto_mpi_copy(G, A);
        goto cleanup;
    }

    if (lzt < lz)
    {
        lz = lzt;
    }

    TA.s = TB.s = 1;

    /* We mostly follow the procedure described in HAC 14.54, but with some
     * minor differences:
     * - Sequences of multiplications or divisions by 2 are grouped into a
     *   single shift operation.
     * - The procedure in HAC assumes that 0 < TB <= TA.
     *     - The condition TB <= TA is not actually necessary for correctness.
     *       TA and TB have symmetric roles except for the loop termination
     *       condition, and the shifts at the beginning of the loop body
     *       remove any significance from the ordering of TA vs TB before
     *       the shifts.
     *     - If TA = 0, the loop goes through 0 iterations and the result is
     *       correctly TB.
     *     - The case TB = 0 was short-circuited above.
     *
     * For the correctness proof below, decompose the original values of
     * A and B as
     *   A = sa * 2^a * A' with A'=0 or A' odd, and sa = +-1
     *   B = sb * 2^b * B' with B'=0 or B' odd, and sb = +-1
     * Then gcd(A, B) = 2^{min(a,b)} * gcd(A',B'),
     * and gcd(A',B') is odd or 0.
     *
     * At the beginning, we have TA = |A| and TB = |B| so gcd(A,B) = gcd(TA,TB).
     * The code maintains the following invariant:
     *     gcd(A,B) = 2^k * gcd(TA,TB) for some k   (I)
     */

    /* Proof that the loop terminates:
     * At each iteration, either the right-shift by 1 is made on a nonzero
     * value and the nonnegative integer bitlen(TA) + bitlen(TB) decreases
     * by at least 1, or the right-shift by 1 is made on zero and then
     * TA becomes 0 which ends the loop (TB cannot be 0 if it is right-shifted
     * since in that case TB is calculated from TB-TA with the condition TB>TA).
     */
    while (Crypto_mpi_cmp_int(&TA, 0) != 0)
    {
        /* Divisions by 2 preserve the invariant (I). */
        CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&TA, Crypto_mpi_lsb(&TA)));
        CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&TB, Crypto_mpi_lsb(&TB)));

        /* Set either TA or TB to |TA-TB|/2. Since TA and TB are both odd,
         * TA-TB is even so the division by 2 has an integer result.
         * Invariant (I) is preserved since any odd divisor of both TA and TB
         * also divides |TA-TB|/2, and any odd divisor of both TA and |TA-TB|/2
         * also divides TB, and any odd divisor of both TB and |TA-TB|/2 also
         * divides TA.
         */
        if (Crypto_mpi_cmp_mpi(&TA, &TB) >= 0)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_sub_abs(&TA, &TA, &TB));
            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&TA, 1));
        }
        else
        {
            CRYPTO_MPI_CHK(Crypto_mpi_sub_abs(&TB, &TB, &TA));
            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&TB, 1));
        }
        /* Note that one of TA or TB is still odd. */
    }

    /* By invariant (I), gcd(A,B) = 2^k * gcd(TA,TB) for some k.
     * At the loop exit, TA = 0, so gcd(TA,TB) = TB.
     * - If there was at least one loop iteration, then one of TA or TB is odd,
     *   and TA = 0, so TB is odd and gcd(TA,TB) = gcd(A',B'). In this case,
     *   lz = min(a,b) so gcd(A,B) = 2^lz * TB.
     * - If there was no loop iteration, then A was 0, and gcd(A,B) = B.
     *   In this case, lz = 0 and B = TB so gcd(A,B) = B = 2^lz * TB as well.
     */

    CRYPTO_MPI_CHK(Crypto_mpi_shift_l(&TB, lz));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(G, &TB));

cleanup:

    Crypto_mpi_free(&TA);
    Crypto_mpi_free(&TB);

    return ret;
}

/******************************************************************************/
/*
 * Brief               Fill an MPI with a number of random bytes.
 *                     The bytes returned from the RNG are used in a specific order which
 *                     is suitable for deterministic ECDSA (see the specification of
 *                     Crypto_mpi_random() and the implementation in Crypto_mpi_fill_random()).
 *
 * Param-Name[in]      size: The number of random bytes to generate.
 *                     f_rng: The RNG function to use. This must not be NULL.
 *                     p_rng: The RNG parameter to be passed to f_rng. This may
 *                            be NULL if f_rng doesn't need a context argument.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_fill_random(
    Crypto_mpi* X,
    uint32      size,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void* p_rng)
{
    int          ret   = E_NOT_OK;
    const uint32 limbs = CHARS_TO_LIMBS(size);

    const uint32 overhead = (limbs * ciL) - size;

    if ((X != NULL_PTR) && (f_rng != NULL_PTR))
    {
        /* Ensure that target MPI has exactly the necessary number of limbs */
        CRYPTO_MPI_CHK(Crypto_mpi_resize_clear(X, limbs));
        if (size == 0)
        {
            return 0;
        }
        if (X->n < limbs)
        {
            return 1;
        }

        (void)IStdLib_MemSet(X->p, 0, overhead);
        (void)IStdLib_MemSet((uint8*)X->p + limbs * ciL, 0, (X->n - limbs) * ciL);
        CRYPTO_MPI_CHK(f_rng(p_rng, (uint8*)X->p + overhead, size));

        mpi_bigendian_to_host(X->p, limbs);
    }
cleanup:
    return ret;
}

/******************************************************************************/
/*
 * Brief                Convert a big-endian byte array aligned to the size of uint32
 *                      into the storage form used by Crypto_mpi
 *
 * Param-Name[in]      x: a big-endian byte array
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
uint32 mpi_uint_bigendian_to_host_c(uint32 x)
{
    uint8  i;
    uint8* x_ptr;
    uint32 tmp = 0;

    for (i = 0u, x_ptr = (uint8*)&x; i < ciL; i++, x_ptr++)
    {
        tmp <<= 8;
        tmp |= (uint32)*x_ptr;
    }

    return (tmp);
}

/******************************************************************************/
/*
 * Brief               Exchange the order of p to fit bigendian system.
 * Param-Name[in]      limbs: The number of limbs of p.
 * Param-Name[in/out]  p: The number need to exchange order.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void mpi_bigendian_to_host(uint32* const p, uint32 limbs)
{
    uint32* cur_limb_left;
    uint32* cur_limb_right;
    if (limbs == 0)
    {
        return;
    }

    for (cur_limb_left = p, cur_limb_right = p + (limbs - 1); cur_limb_left <= cur_limb_right;
         cur_limb_left++, cur_limb_right--)
    {
        uint32 tmp;

        tmp             = CRYPTO_BSWAP32(*cur_limb_left);
        *cur_limb_left  = CRYPTO_BSWAP32(*cur_limb_right);
        *cur_limb_right = tmp;
    }
}

/******************************************************************************/
/*
 * Brief               Compute the modular inverse: X = A^-1 mod N.
 * Param-Name[in]      A: The MPI to calculate the modular inverse of.
 *                     N: The base of the modular inversion.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI.
 * Return              int
 */
/******************************************************************************/
int Crypto_mpi_inv_mod(Crypto_mpi* X, const Crypto_mpi* A, const Crypto_mpi* N)
{
    int        ret = E_NOT_OK;
    Crypto_mpi G, TA, TU, U1, U2, TB, TV, V1, V2;

    if (Crypto_mpi_cmp_int(N, 1) <= 0)
    {
        return E_NOT_OK;
    }

    Crypto_mpi_init(&TA);
    Crypto_mpi_init(&TU);
    Crypto_mpi_init(&U1);
    Crypto_mpi_init(&U2);
    Crypto_mpi_init(&G);
    Crypto_mpi_init(&TB);
    Crypto_mpi_init(&TV);
    Crypto_mpi_init(&V1);
    Crypto_mpi_init(&V2);

    CRYPTO_MPI_CHK(Crypto_mpi_gcd(&G, A, N));

    if (Crypto_mpi_cmp_int(&G, 1) != 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&TA, A, N));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&TU, &TA));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&TB, N));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&TV, N));

    CRYPTO_MPI_CHK(Crypto_mpi_lset(&U1, 1));
    CRYPTO_MPI_CHK(Crypto_mpi_lset(&U2, 0));
    CRYPTO_MPI_CHK(Crypto_mpi_lset(&V1, 0));
    CRYPTO_MPI_CHK(Crypto_mpi_lset(&V2, 1));

    do
    {
        while ((TU.p[0] & 1) == 0)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&TU, 1));

            if ((U1.p[0] & 1) != 0 || (U2.p[0] & 1) != 0)
            {
                CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(&U1, &U1, &TB));
                CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&U2, &U2, &TA));
            }

            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&U1, 1));
            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&U2, 1));
        }

        while ((TV.p[0] & 1) == 0)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&TV, 1));

            if ((V1.p[0] & 1) != 0 || (V2.p[0] & 1) != 0)
            {
                CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(&V1, &V1, &TB));
                CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&V2, &V2, &TA));
            }

            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&V1, 1));
            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&V2, 1));
        }

        if (Crypto_mpi_cmp_mpi(&TU, &TV) >= 0)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&TU, &TU, &TV));
            CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&U1, &U1, &V1));
            CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&U2, &U2, &V2));
        }
        else
        {
            CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&TV, &TV, &TU));
            CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&V1, &V1, &U1));
            CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&V2, &V2, &U2));
        }
    } while (Crypto_mpi_cmp_int(&TU, 0) != 0);

    while (Crypto_mpi_cmp_int(&V1, 0) < 0)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(&V1, &V1, N));
    }

    while (Crypto_mpi_cmp_mpi(&V1, N) >= 0)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&V1, &V1, N));
    }

    CRYPTO_MPI_CHK(Crypto_mpi_copy(X, &V1));

cleanup:

    Crypto_mpi_free(&TA);
    Crypto_mpi_free(&TU);
    Crypto_mpi_free(&U1);
    Crypto_mpi_free(&U2);
    Crypto_mpi_free(&G);
    Crypto_mpi_free(&TB);
    Crypto_mpi_free(&TV);
    Crypto_mpi_free(&V1);
    Crypto_mpi_free(&V2);

    return ret;
}

/******************************************************************************/
/*
 * Brief               Small divisors test (X must be positive).
 * Param-Name[in]      X: The mpi for test.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int mpi_check_small_factors(const Crypto_mpi* X)
{
    int      ret = 0;
    uint32   i;
    uint32   r;
    unsigned p = 3; /* The first odd prime */

    if ((X->p[0] & 1) == 0)
    {
        return CRYPTO_ERR_MPI_NOT_ACCEPTABLE;
    }

    for (i = 0; i < sizeof(small_prime_gaps); p += small_prime_gaps[i], i++)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_mod_int(&r, X, p));
        if (r == 0)
        {
            if (Crypto_mpi_cmp_int(X, p) == 0)
            {
                return 1;
            }
            else
            {
                return CRYPTO_ERR_MPI_NOT_ACCEPTABLE;
            }
        }
    }

cleanup:
    return ret;
}

/******************************************************************************/
/*
 * Brief               Miller-Rabin pseudo-primality test.
 * Param-Name[in]      X: The mpi for test.
 *                     rounds: The number of test rounds.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int mpi_miller_rabin(
    const Crypto_mpi* X,
    uint32            rounds,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void* p_rng)
{
    int        ret, count;
    uint32     i, j, k, s;
    Crypto_mpi W, R, T, A, RR;

    Crypto_mpi_init(&W);
    Crypto_mpi_init(&R);
    Crypto_mpi_init(&T);
    Crypto_mpi_init(&A);
    Crypto_mpi_init(&RR);

    /*
     * W = |X| - 1
     * R = W >> lsb( W )
     */
    CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&W, X, 1));
    s = Crypto_mpi_lsb(&W);
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&R, &W));
    CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&R, s));

    for (i = 0; i < rounds; i++)
    {
        /*
         * pick a random A, 1 < A < |X| - 1
         */
        count = 0;
        do
        {
            CRYPTO_MPI_CHK(Crypto_mpi_fill_random(&A, X->n * ciL, f_rng, p_rng));

            j = Crypto_mpi_bitlen(&A);
            k = Crypto_mpi_bitlen(&W);
            if (j > k)
            {
                A.p[A.n - 1] &= ((uint32)1 << (k - (A.n - 1) * biL - 1)) - 1;
            }

            if (count++ > 30)
            {
                ret = CRYPTO_ERR_MPI_NOT_ACCEPTABLE;
                goto cleanup;
            }

        } while (Crypto_mpi_cmp_mpi(&A, &W) >= 0 || Crypto_mpi_cmp_int(&A, 1) <= 0);

        /*
         * A = A^R mod |X|
         */
        CRYPTO_MPI_CHK(Crypto_mpi_exp_mod(&A, &A, &R, X, &RR));

        if (Crypto_mpi_cmp_mpi(&A, &W) == 0 || Crypto_mpi_cmp_int(&A, 1) == 0)
        {
            continue;
        }

        j = 1;
        while (j < s && Crypto_mpi_cmp_mpi(&A, &W) != 0)
        {
            /*
             * A = A * A mod |X|
             */
            CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&T, &A, &A));
            CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&A, &T, X));

            if (Crypto_mpi_cmp_int(&A, 1) == 0)
            {
                break;
            }

            j++;
        }

        /*
         * not prime if A != |X| - 1 or A == 1
         */
        if (Crypto_mpi_cmp_mpi(&A, &W) != 0 || Crypto_mpi_cmp_int(&A, 1) == 0)
        {
            ret = CRYPTO_ERR_MPI_NOT_ACCEPTABLE;
            break;
        }
    }

cleanup:
    Crypto_mpi_free(&W);
    Crypto_mpi_free(&R);
    Crypto_mpi_free(&T);
    Crypto_mpi_free(&A);
    Crypto_mpi_free(&RR);

    return ret;
}

/******************************************************************************/
/*
 * Brief               small factors, then Miller-Rabin
 * Param-Name[in]      X: The MPI to check for primality.
 *                     rounds: The number of bases to perform the Miller-Rabin
 *                             primality test for.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_mpi_is_prime_ext(
    const Crypto_mpi* X,
    int               rounds,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void* p_rng)
{
    int        ret = CRYPTO_ERR_ERROR_CORRUPTION_DETECTED;
    Crypto_mpi XX;

    XX.s = 1;
    XX.n = X->n;
    XX.p = X->p;

    if (Crypto_mpi_cmp_int(&XX, 0) == 0 || Crypto_mpi_cmp_int(&XX, 1) == 0)
    {
        return CRYPTO_ERR_MPI_NOT_ACCEPTABLE;
    }

    if (Crypto_mpi_cmp_int(&XX, 2) == 0)
    {
        return 0;
    }

    if ((ret = mpi_check_small_factors(&XX)) != 0)
    {
        if (ret == 1)
        {
            return 0;
        }

        return ret;
    }

    return mpi_miller_rabin(&XX, rounds, f_rng, p_rng);
}

/******************************************************************************/
/*
 * Brief               Generate a prime number.
 * Param-Name[in]      nbits: The required size of the destination MPI in bits.
 *                     flags: A mask of flags.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI to store the generated prime in.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_mpi_gen_prime(
    Crypto_mpi* X,
    uint32      nbits,
    int         flags,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void* p_rng)
{
#ifdef CRYPTO_HAVE_INT64
// ceil(2^63.5)
#define CEIL_MAXUINT_DIV_SQRT2 0xb504f333f9de6485ULL
#else
    // ceil(2^31.5)
#define CEIL_MAXUINT_DIV_SQRT2 0xb504f334U
#endif
    int        ret = CRYPTO_ERR_MPI_NOT_ACCEPTABLE;
    uint32     k, n;
    int        rounds;
    uint32     r;
    Crypto_mpi Y;

    if (nbits < 3 || nbits > CRYPTO_MPI_MAX_BITS)
    {
        return CRYPTO_ERR_MPI_BAD_INPUT_DATA;
    }

    Crypto_mpi_init(&Y);

    n = BITS_TO_LIMBS(nbits);

    if ((flags & CRYPTO_MPI_GEN_PRIME_FLAG_LOW_ERR) == 0)
    {
        /*
         * 2^-80 error probability, number of rounds chosen per HAC, table 4.4
         */
        rounds =
            ((nbits >= 1300)  ? 2
             : (nbits >= 850) ? 3
             : (nbits >= 650) ? 4
             : (nbits >= 350) ? 8
             : (nbits >= 250) ? 12
             : (nbits >= 150) ? 18
                              : 27);
    }
    else
    {
        /*
         * 2^-100 error probability, number of rounds computed based on HAC,
         * fact 4.48
         */
        rounds =
            ((nbits >= 1450)   ? 4
             : (nbits >= 1150) ? 5
             : (nbits >= 1000) ? 6
             : (nbits >= 850)  ? 7
             : (nbits >= 750)  ? 8
             : (nbits >= 500)  ? 13
             : (nbits >= 250)  ? 28
             : (nbits >= 150)  ? 40
                               : 51);
    }

    while (1)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_fill_random(X, n * ciL, f_rng, p_rng));

        if (X->p[n - 1] < CEIL_MAXUINT_DIV_SQRT2)
        {
            continue;
        }

        k = n * biL;
        if (k > nbits)
        {
            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(X, k - nbits));
        }
        X->p[0] |= 1;

        if ((flags & CRYPTO_MPI_GEN_PRIME_FLAG_DH) == 0)
        {
            ret = Crypto_mpi_is_prime_ext(X, rounds, f_rng, p_rng);

            if (ret != CRYPTO_ERR_MPI_NOT_ACCEPTABLE)
            {
                goto cleanup;
            }
        }
        else
        {
            /*
             * A necessary condition for Y and X = 2Y + 1 to be prime
             * is X = 2 mod 3 (which is equivalent to Y = 2 mod 3).
             * Make sure it is satisfied, while keeping X = 3 mod 4
             */

            X->p[0] |= 2;

            CRYPTO_MPI_CHK(Crypto_mpi_mod_int(&r, X, 3));
            if (r == 0)
            {
                CRYPTO_MPI_CHK(Crypto_mpi_add_int(X, X, 8));
            }
            else if (r == 1)
            {
                CRYPTO_MPI_CHK(Crypto_mpi_add_int(X, X, 4));
            }

            /* Set Y = (X-1) / 2, which is X / 2 because X is odd */
            CRYPTO_MPI_CHK(Crypto_mpi_copy(&Y, X));
            CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&Y, 1));

            while (1)
            {
                /*
                 * First, check small factors for X and Y
                 * before doing Miller-Rabin on any of them
                 */
                if ((ret = mpi_check_small_factors(X)) == 0 && (ret = mpi_check_small_factors(&Y)) == 0
                    && (ret = mpi_miller_rabin(X, rounds, f_rng, p_rng)) == 0
                    && (ret = mpi_miller_rabin(&Y, rounds, f_rng, p_rng)) == 0)
                {
                    goto cleanup;
                }

                if (ret != CRYPTO_ERR_MPI_NOT_ACCEPTABLE)
                {
                    goto cleanup;
                }

                /*
                 * Next candidates. We want to preserve Y = (X-1) / 2 and
                 * Y = 1 mod 2 and Y = 2 mod 3 (eq X = 3 mod 4 and X = 2 mod 3)
                 * so up Y by 6 and X by 12.
                 */
                CRYPTO_MPI_CHK(Crypto_mpi_add_int(X, X, 12));
                CRYPTO_MPI_CHK(Crypto_mpi_add_int(&Y, &Y, 6));
            }
        }
    }

cleanup:

    Crypto_mpi_free(&Y);

    return ret;
}

/******************************************************************************/
/*
 * Brief               Wrapper around Crypto_asn1_get_mpi() that rejects zero.
 * Param-Name[in]      p: On entry, *p points to the start of the ASN.1 element.
 *                        On successful completion, *p points to the first byte
 *                        beyond the ASN.1 element. On error, the value of *p is
 *                        undefined.
 *                     end: End of data.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: On success, the parsed value.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int asn1_get_nonzero_mpi(unsigned char** p, const unsigned char* end, Crypto_mpi* X)
{
    int ret;

    ret = Crypto_asn1_get_mpi(p, end, X);
    if (ret != 0)
    {
        return ret;
    }

    if (Crypto_mpi_cmp_int(X, 0) == 0)
    {
        return E_NOT_OK;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Parse a PKCS#1 (ASN.1) encoded private RSA key.
 *
 * Param-Name[in]      key: The buffer that contains the key.
 *                     keylen: The length of the key buffer in bytes.
 * Param-Name[in/out]  rsa: The RSA context where parsed data will be stored.
 * Param-Name[out]     None
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_parse_key(Crypto_rsa_context* rsa, const unsigned char* key, uint32 keylen)
{
    int            ret, version;
    uint32         len;
    unsigned char *p, *end;

    Crypto_mpi T;
    Crypto_mpi_init(&T);

    p   = (unsigned char*)key;
    end = p + keylen;

    /*
     * This function parses the RSAPrivateKey (PKCS#1)
     *
     *  RSAPrivateKey ::= SEQUENCE {
     *      version           Version,
     *      modulus           INTEGER,  -- n
     *      publicExponent    INTEGER,  -- e
     *      privateExponent   INTEGER,  -- d
     *      prime1            INTEGER,  -- p
     *      prime2            INTEGER,  -- q
     *      exponent1         INTEGER,  -- d mod (p-1)
     *      exponent2         INTEGER,  -- d mod (q-1)
     *      coefficient       INTEGER,  -- (inverse of q) mod p
     *      otherPrimeInfos   OtherPrimeInfos OPTIONAL
     *  }
     */
    if ((ret = Crypto_asn1_get_tag(&p, end, &len, CRYPTO_ASN1_CONSTRUCTED | CRYPTO_ASN1_SEQUENCE)) != 0)
    {
        return ret;
    }

    if (end != p + len)
    {
        return E_NOT_OK;
    }

    if ((ret = Crypto_asn1_get_int(&p, end, &version)) != 0)
    {
        return ret;
    }

    if (version != 0)
    {
        return E_NOT_OK;
    }

    /* Import N */
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0
        || (ret = Crypto_rsa_import(rsa, &T, NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR)) != 0)
    {
        goto cleanup;
    }

    /* Import E */
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0
        || (ret = Crypto_rsa_import(rsa, NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR, &T)) != 0)
    {
        goto cleanup;
    }

    /* Import D */
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0
        || (ret = Crypto_rsa_import(rsa, NULL_PTR, NULL_PTR, NULL_PTR, &T, NULL_PTR)) != 0)
    {
        goto cleanup;
    }

    /* Import P */
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0
        || (ret = Crypto_rsa_import(rsa, NULL_PTR, &T, NULL_PTR, NULL_PTR, NULL_PTR)) != 0)
    {
        goto cleanup;
    }

    /* Import Q */
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0
        || (ret = Crypto_rsa_import(rsa, NULL_PTR, NULL_PTR, &T, NULL_PTR, NULL_PTR)) != 0)
    {
        goto cleanup;
    }

#if !defined(CRYPTO_RSA_NO_CRT) && !defined(CRYPTO_RSA_ALT)
    /*
     * The RSA CRT parameters DP, DQ and QP are nominally redundant, in
     * that they can be easily recomputed from D, P and Q. However by
     * parsing them from the PKCS1 structure it is possible to avoid
     * recalculating them which both reduces the overhead of loading
     * RSA private keys into memory and also avoids side channels which
     * can arise when computing those values, since all of D, P, and Q
     * are secret. See https://eprint.iacr.org/2020/055 for a
     * description of one such attack.
     */

    /* Import DP */
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0 || (ret = Crypto_mpi_copy(&rsa->DP, &T)) != 0)
    {
        goto cleanup;
    }

    /* Import DQ */
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0 || (ret = Crypto_mpi_copy(&rsa->DQ, &T)) != 0)
    {
        goto cleanup;
    }

    /* Import QP */
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0 || (ret = Crypto_mpi_copy(&rsa->QP, &T)) != 0)
    {
        goto cleanup;
    }

#else
/* Verify existence of the CRT params */
#if 0
    if ((ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0 || (ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0
        || (ret = asn1_get_nonzero_mpi(&p, end, &T)) != 0)
    {
        goto cleanup;
    }
#endif
#endif

    /* rsa_complete() doesn't complete anything with the default
     * implementation but is still called:
     * - for the benefit of alternative implementation that may want to
     *   pre-compute stuff beyond what's provided (eg Montgomery factors)
     * - as is also sanity-checks the key
     *
     * Furthermore, we also check the public part for consistency with
     * Crypto_pk_parse_pubkey(), as it includes size minima for example.
     */
    if ((ret = Crypto_rsa_complete(rsa)) != 0 || (ret = Crypto_rsa_check_pubkey(rsa)) != 0)
    {
        goto cleanup;
    }

    if (p != end)
    {
        ret = E_NOT_OK;
    }

cleanup:
    Crypto_mpi_free(&T);
    return ret;
}

/******************************************************************************/
/*
 * Brief               Parse a PKCS#1 (ASN.1) encoded public RSA key.
 *
 * Param-Name[in]      key: The buffer that contains the key.
 *                     keylen: The length of the key buffer in bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     rsa; The RSA context where parsed data will be stored.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_parse_pubkey(Crypto_rsa_context* rsa, const unsigned char* key, uint32 keylen)
{
    unsigned char* p   = (unsigned char*)key;
    unsigned char* end = (unsigned char*)(key + keylen);
    int            ret = E_NOT_OK;
    uint32         len;

    /*
     *  RSAPublicKey ::= SEQUENCE {
     *      modulus           INTEGER,  -- n
     *      publicExponent    INTEGER   -- e
     *  }
     */

    if ((ret = Crypto_asn1_get_tag(&p, end, &len, CRYPTO_ASN1_CONSTRUCTED | CRYPTO_ASN1_SEQUENCE)) != 0)
    {
        return ret;
    }

    if (end != p + len)
    {
        return E_NOT_OK;
    }

    /* Import N */
    if ((ret = Crypto_asn1_get_tag(&p, end, &len, CRYPTO_ASN1_INTEGER)) != 0)
    {
        return ret;
    }

    if ((ret = Crypto_rsa_import_raw(rsa, p, len, NULL_PTR, 0u, NULL_PTR, 0u, NULL_PTR, 0u, NULL_PTR, 0)) != 0)
    {
        return E_NOT_OK;
    }

    p += len;

    /* Import E */
    if ((ret = Crypto_asn1_get_tag(&p, end, &len, CRYPTO_ASN1_INTEGER)) != 0)
    {
        return ret;
    }

    if ((ret = Crypto_rsa_import_raw(rsa, NULL_PTR, 0u, NULL_PTR, 0u, NULL_PTR, 0u, NULL_PTR, 0u, p, len)) != 0)
    {
        return E_NOT_OK;
    }

    p += len;

    if (Crypto_rsa_complete(rsa) != 0 || Crypto_rsa_check_pubkey(rsa) != 0)
    {
        return E_NOT_OK;
    }

    if (p != end)
    {
        return E_NOT_OK;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Check a public RSA key.
 * Param-Name[in]      ctx: The context of RSA.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_check_pubkey(const Crypto_rsa_context* ctx)
{
    if (rsa_check_context(ctx, 0 /* public */, 0 /* no blinding */) != 0)
    {
        return E_NOT_OK;
    }

    if (Crypto_mpi_bitlen(&ctx->N) < 128)
    {
        return E_NOT_OK;
    }

    if (Crypto_mpi_get_bit(&ctx->E, 0) == 0 || Crypto_mpi_bitlen(&ctx->E) < 2
        || Crypto_mpi_cmp_mpi(&ctx->E, &ctx->N) >= 0)
    {
        return E_NOT_OK;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Check for the consistency of all fields in an RSA private
                       key context.

 * Param-Name[in]      ctx: The context of RSA.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_check_privkey(const Crypto_rsa_context* ctx)
{
    if (Crypto_rsa_check_pubkey(ctx) != 0 || rsa_check_context(ctx, 1 /* private */, 1 /* blinding */) != 0)
    {
        return E_NOT_OK;
    }

    if (Crypto_rsa_validate_params(&ctx->N, &ctx->P, &ctx->Q, &ctx->D, &ctx->E, NULL_PTR, NULL_PTR) != 0)
    {
        return E_NOT_OK;
    }

#if !defined(CRYPTO_RSA_NO_CRT)
    else if (Crypto_rsa_validate_crt(&ctx->P, &ctx->Q, &ctx->D, &ctx->DP, &ctx->DQ, &ctx->QP) != 0)
    {
        return E_NOT_OK;
    }
#endif

    return 0;
}

/******************************************************************************/
/*
 * Brief               Check that core RSA parameters are sane.
 *
 * Param-Name[in]      N: Parameter N inside context.
 *                     P: Parameter P inside context.
 *                     Q: Parameter Q inside context.
 *                     D: Parameter D inside context.
 *                     E: Parameter E inside context.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_validate_params(
    const Crypto_mpi* N,
    const Crypto_mpi* P,
    const Crypto_mpi* Q,
    const Crypto_mpi* D,
    const Crypto_mpi* E,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void* p_rng)
{
    int        ret = 0;
    Crypto_mpi K, L;

    Crypto_mpi_init(&K);
    Crypto_mpi_init(&L);

    /*
     * Step 1: If PRNG provided, check that P and Q are prime
     */

    /*
     * When generating keys, the strongest security we support aims for an error
     * rate of at most 2^-100 and we are aiming for the same certainty here as
     * well.
     */
    if (f_rng != NULL_PTR && P != NULL_PTR && (ret = Crypto_mpi_is_prime_ext(P, 50, f_rng, p_rng)) != 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    if (f_rng != NULL_PTR && Q != NULL_PTR && (ret = Crypto_mpi_is_prime_ext(Q, 50, f_rng, p_rng)) != 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    /*
     * Step 2: Check that 1 < N = P * Q
     */

    if (P != NULL_PTR && Q != NULL_PTR && N != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&K, P, Q));
        if (Crypto_mpi_cmp_int(N, 1) <= 0 || Crypto_mpi_cmp_mpi(&K, N) != 0)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }
    }

    /*
     * Step 3: Check and 1 < D, E < N if present.
     */

    if (N != NULL_PTR && D != NULL_PTR && E != NULL_PTR)
    {
        if (Crypto_mpi_cmp_int(D, 1) <= 0 || Crypto_mpi_cmp_int(E, 1) <= 0 || Crypto_mpi_cmp_mpi(D, N) >= 0
            || Crypto_mpi_cmp_mpi(E, N) >= 0)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }
    }

    /*
     * Step 4: Check that D, E are inverse modulo P-1 and Q-1
     */

    if (P != NULL_PTR && Q != NULL_PTR && D != NULL_PTR && E != NULL_PTR)
    {
        if (Crypto_mpi_cmp_int(P, 1) <= 0 || Crypto_mpi_cmp_int(Q, 1) <= 0)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }

        /* Compute DE-1 mod P-1 */
        CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&K, D, E));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, &K, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&L, P, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&K, &K, &L));
        if (Crypto_mpi_cmp_int(&K, 0) != 0)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }

        /* Compute DE-1 mod Q-1 */
        CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&K, D, E));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, &K, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&L, Q, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&K, &K, &L));
        if (Crypto_mpi_cmp_int(&K, 0) != 0)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }
    }

cleanup:

    Crypto_mpi_free(&K);
    Crypto_mpi_free(&L);

    /* Wrap MPI error codes by RSA check failure error code */
    if (ret != 0 && ret != E_NOT_OK)
    {
        ret += E_NOT_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Check that RSA CRT parameters are in accordance with core
 *                     parameters.
 * Param-Name[in]      P: Parameter P inside context.
 *                     Q: Parameter Q inside context.
 *                     D: Parameter D inside context.
 *                     DP: Parameter DP inside context.
 *                     DQ: Parameter DQ inside context.
 *                     QP: Parameter QP inside context.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_validate_crt(
    const Crypto_mpi* P,
    const Crypto_mpi* Q,
    const Crypto_mpi* D,
    const Crypto_mpi* DP,
    const Crypto_mpi* DQ,
    const Crypto_mpi* QP)
{
    int ret = 0;

    Crypto_mpi K, L;
    Crypto_mpi_init(&K);
    Crypto_mpi_init(&L);

    /* Check that DP - D == 0 mod P - 1 */
    if (DP != NULL_PTR)
    {
        if (P == NULL_PTR)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }

        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, P, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&L, DP, D));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&L, &L, &K));

        if (Crypto_mpi_cmp_int(&L, 0) != 0)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }
    }

    /* Check that DQ - D == 0 mod Q - 1 */
    if (DQ != NULL_PTR)
    {
        if (Q == NULL_PTR)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }

        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, Q, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&L, DQ, D));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&L, &L, &K));

        if (Crypto_mpi_cmp_int(&L, 0) != 0)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }
    }

    /* Check that QP * Q - 1 == 0 mod P */
    if (QP != NULL_PTR)
    {
        if (P == NULL_PTR || Q == NULL_PTR)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }

        CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&K, QP, Q));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, &K, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&K, &K, P));
        if (Crypto_mpi_cmp_int(&K, 0) != 0)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }
    }

cleanup:

    /* Wrap MPI error codes by RSA check failure error code */
    if (ret != 0 && ret != E_NOT_OK && ret != E_NOT_OK)
    {
        ret += E_NOT_OK;
    }

    Crypto_mpi_free(&K);
    Crypto_mpi_free(&L);

    return ret;
}

/******************************************************************************/
/*
 * Brief               Write a PKCS#1 (ASN.1) encoded private RSA key.
 *
 * Param-Name[in]      rsa: The RSA context which contains the data to be written.
 *                     start: Beginning of the buffer that will be filled with
 *                            the private key.
 * Param-Name[in/out]  p: End of the buffer that will be filled with the private
 *                        key. On successful return, the referenced pointer will
 *                        be updated in order to point to the beginning of written
 *                        data.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_write_key(const Crypto_rsa_context* rsa, unsigned char* start, unsigned char** p)
{
    uint32 len = 0;
    int    ret;

    Crypto_mpi T; /* Temporary holding the exported parameters */

    /*
     * Export the parameters one after another to avoid simultaneous copies.
     */

    Crypto_mpi_init(&T);
#if !defined(CRYPTO_RSA_NO_CRT)
    /* Export QP */
    if ((ret = Crypto_rsa_export_crt(rsa, NULL_PTR, NULL_PTR, &T)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

    /* Export DQ */
    if ((ret = Crypto_rsa_export_crt(rsa, NULL_PTR, &T, NULL_PTR)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

    /* Export DP */
    if ((ret = Crypto_rsa_export_crt(rsa, &T, NULL_PTR, NULL_PTR)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;
#endif
    /* Export Q */
    if ((ret = Crypto_rsa_export(rsa, NULL_PTR, NULL_PTR, &T, NULL_PTR, NULL_PTR)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

    /* Export P */
    if ((ret = Crypto_rsa_export(rsa, NULL_PTR, &T, NULL_PTR, NULL_PTR, NULL_PTR)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

    /* Export D */
    if ((ret = Crypto_rsa_export(rsa, NULL_PTR, NULL_PTR, NULL_PTR, &T, NULL_PTR)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

    /* Export E */
    if ((ret = Crypto_rsa_export(rsa, NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR, &T)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

    /* Export N */
    if ((ret = Crypto_rsa_export(rsa, &T, NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

end_of_export:

    Crypto_mpi_free(&T);
    if (ret < 0)
    {
        return ret;
    }

    CRYPTO_ASN1_CHK_ADD(len, Crypto_asn1_write_int(p, start, 0));
    CRYPTO_ASN1_CHK_ADD(len, Crypto_asn1_write_len(p, start, len));
    CRYPTO_ASN1_CHK_ADD(len, Crypto_asn1_write_tag(p, start, CRYPTO_ASN1_CONSTRUCTED | CRYPTO_ASN1_SEQUENCE));

    return (int)len;
}
/******************************************************************************/
/*
 * Brief               Write a PKCS#1 (ASN.1) encoded public RSA key.
 *
 * Param-Name[in]      rsa: The RSA context which contains the data to be written.
 *                     start: Beginning of the buffer that will be filled with
 *                            the public key.
 * Param-Name[in/out]  p: End of the buffer that will be filled with the public
 *                        key. On successful return, the referenced pointer will
 *                        be updated in order to point to the beginning of written
 *                        data.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_write_publickey(const Crypto_rsa_context* rsa, unsigned char* start, unsigned char** p)
{
    uint32 len = 0;
    int    ret;

    Crypto_mpi T; /* Temporary holding the exported parameters */

    /*
     * Export the parameters one after another to avoid simultaneous copies.
     */

    Crypto_mpi_init(&T);

    /* Export E */
    if ((ret = Crypto_rsa_export(rsa, NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR, &T)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

    /* Export N */
    if ((ret = Crypto_rsa_export(rsa, &T, NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR)) != 0
        || (ret = Crypto_asn1_write_mpi(p, start, &T)) < 0)
    {
        goto end_of_export;
    }
    len += ret;

end_of_export:

    Crypto_mpi_free(&T);
    if (ret < 0)
    {
        return ret;
    }

    CRYPTO_ASN1_CHK_ADD(len, Crypto_asn1_write_int(p, start, 0));
    CRYPTO_ASN1_CHK_ADD(len, Crypto_asn1_write_len(p, start, len));
    CRYPTO_ASN1_CHK_ADD(len, Crypto_asn1_write_tag(p, start, CRYPTO_ASN1_CONSTRUCTED | CRYPTO_ASN1_SEQUENCE));

    return (int)len;
}
/******************************************************************************/
/*
 * Brief               This function performs the unpadding part of a PKCS#1
 *                     v1.5 decryption operation (EME-PKCS1-v1_5 decoding).
 *
 * Param-Name[in]      input: The input buffer which is the payload inside
 *                            PKCS#1v1.5 encryption padding, called the "encoded
 *                            message EM" by the terminology.
 *                     ilen: The length of the payload in the \p input buffer.
 *                     output_max_len: The length in bytes of the output buffer
 *                                     output.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer for the payload, called "message M" by
 *                             the PKCS#1 terminology. This must be a writable
 *                             buffer of length output_max_len bytes.
 *                     olen: The address at which to store the length of
 *                           the payload. This must not be NULL.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int Crypto_ct_rsaes_pkcs1_v15_unpadding(
    unsigned char* input,
    uint32         ilen,
    unsigned char* output,
    uint32         output_max_len,
    uint32*        olen)
{
    int    ret = E_NOT_OK;
    uint32 i, plaintext_max_size;

    /* The following variables take sensitive values: their value must
     * not leak into the observable behavior of the function other than
     * the designated outputs (output, olen, return value). Otherwise
     * this would open the execution of the function to
     * side-channel-based variants of the Bleichenbacher padding oracle
     * attack. Potential side channels include overall timing, memory
     * access patterns (especially visible to an adversary who has access
     * to a shared memory cache), and branches (especially visible to
     * an adversary who has access to a shared code cache or to a shared
     * branch predictor). */
    uint32 pad_count = 0;
    uint32 bad;
    uint32 pad_done;
    uint32 plaintext_size = 0;
    uint32 output_too_large;

    plaintext_max_size = (output_max_len > ilen - 11) ? ilen - 11 : output_max_len;

    /* Check and get padding length in constant time and constant
     * memory trace. The first byte must be 0. */
    bad = Crypto_ct_bool(input[0]);

    /* Decode EME-PKCS1-v1_5 padding: 0x00 || 0x02 || PS || 0x00
     * where PS must be at least 8 nonzero bytes. */
    bad = Crypto_ct_bool_or(bad, Crypto_ct_uint_ne(input[1], CRYPTO_RSA_CRYPT));

    /* Read the whole buffer. Set pad_done to nonzero if we find
     * the 0x00 byte and remember the padding length in pad_count. */
    pad_done = 0U;
    for (i = 2; i < ilen; i++)
    {
        uint32 found = Crypto_ct_uint_eq(input[i], 0);
        pad_done     = Crypto_ct_bool_or(pad_done, found);
        pad_count += Crypto_ct_uint_if_else_0(Crypto_ct_bool_not(pad_done), 1);
    }

    /* If pad_done is still zero, there's no data, only unfinished padding. */
    bad = Crypto_ct_bool_or(bad, Crypto_ct_bool_not(pad_done));

    /* There must be at least 8 bytes of padding. */
    bad = Crypto_ct_bool_or(bad, Crypto_ct_uint_gt(8, pad_count));

    /* If the padding is valid, set plaintext_size to the number of
     * remaining bytes after stripping the padding. If the padding
     * is invalid, avoid leaking this fact through the size of the
     * output: use the maximum message size that fits in the output
     * buffer. Do it without branches to avoid leaking the padding
     * validity through timing. RSA keys are small enough that all the
     * uint32 values involved fit in unsigned int. */
    plaintext_size = Crypto_ct_uint_if(bad, (unsigned)plaintext_max_size, (unsigned)(ilen - pad_count - 3));

    /* Set output_too_large to 0 if the plaintext fits in the output
     * buffer and to 1 otherwise. */
    output_too_large = Crypto_ct_uint_gt(plaintext_size, plaintext_max_size);

    /* Set ret without branches to avoid timing attacks. Return:
     * - INVALID_PADDING if the padding is bad (bad != 0).
     * - OUTPUT_TOO_LARGE if the padding is good but the decrypted
     *   plaintext does not fit in the output buffer.
     * - 0 if the padding is correct. */
    ret = Crypto_ct_error_if(bad, E_NOT_OK, Crypto_ct_error_if_else_0(output_too_large, E_NOT_OK));

    /* If the padding is bad or the plaintext is too large, zero the
     * data that we're about to copy to the output buffer.
     * We need to copy the same amount of data
     * from the same buffer whether the padding is good or not to
     * avoid leaking the padding validity through overall timing or
     * through memory or cache access patterns. */
    Crypto_ct_zeroize_if(Crypto_ct_bool_or(bad, output_too_large), input + 11, ilen - 11);

    /* If the plaintext is too large, truncate it to the buffer size.
     * Copy anyway to avoid revealing the length through timing, because
     * revealing the length is as bad as revealing the padding validity
     * for a Bleichenbacher attack. */
    plaintext_size = Crypto_ct_uint_if(output_too_large, (unsigned)plaintext_max_size, (unsigned)plaintext_size);

    /* Move the plaintext to the leftmost position where it can start in
     * the working buffer, i.e. make it start plaintext_max_size from
     * the end of the buffer. Do this with a memory access trace that
     * does not depend on the plaintext size. After this move, the
     * starting location of the plaintext is no longer sensitive
     * information. */
    Crypto_ct_memmove_left(input + ilen - plaintext_max_size, plaintext_max_size, plaintext_max_size - plaintext_size);

    /* Finally copy the decrypted plaintext plus trailing zeros into the output
     * buffer. If output_max_len is 0, then output may be an invalid pointer
     * and the result of (void)ILib_memcpy() would be undefined; prevent undefined
     * behavior making sure to depend only on output_max_len (the size of the
     * user-provided output buffer), which is independent from plaintext
     * length, validity of padding, success of the decryption, and other
     * secrets. */
    if (output_max_len != 0)
    {
        (void)IStdLib_MemCpy(output, input + ilen - plaintext_max_size, plaintext_max_size);
    }

    /* Report the amount of data we copied to the output buffer. In case
     * of errors (bad padding or output too large), the value of *olen
     * when this function returns is not specified. Making it equivalent
     * to the good case limits the risks of leaking the padding validity. */
    *olen = plaintext_size;

    return ret;
}

#if !defined(CRYPTO_RSA_ALT)

/******************************************************************************/
/*
 * Brief               This function imports a set of core parameters into an
 *                     RSA context.
 *
 * Param-Name[in]      N: The RSA modulus. This may be NULL.
 *                     P: The first prime factor of N. This may be NULL.
 *                     Q: The second prime factor of N. This may be NULL.
 *                     D: The private exponent. This may be NULL.
 *                     E: The public exponent. This may be NULL.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The initialized RSA context to store the parameters in.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_import(
    Crypto_rsa_context* ctx,
    const Crypto_mpi*   N,
    const Crypto_mpi*   P,
    const Crypto_mpi*   Q,
    const Crypto_mpi*   D,
    const Crypto_mpi*   E)
{
    int ret = E_NOT_OK;

    if ((N != NULL_PTR && (ret = Crypto_mpi_copy(&ctx->N, N)) != 0)
        || (P != NULL_PTR && (ret = Crypto_mpi_copy(&ctx->P, P)) != 0)
        || (Q != NULL_PTR && (ret = Crypto_mpi_copy(&ctx->Q, Q)) != 0)
        || (D != NULL_PTR && (ret = Crypto_mpi_copy(&ctx->D, D)) != 0)
        || (E != NULL_PTR && (ret = Crypto_mpi_copy(&ctx->E, E)) != 0))
    {
        return E_NOT_OK;
    }

    if (N != NULL_PTR)
    {
        ctx->len = Crypto_mpi_size(&ctx->N);
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               This function imports core RSA parameters, in raw
 *                     big-endian binary format, into an RSA context.
 *
 * Param-Name[in]      N: The RSA modulus. This may be NULL.
 *                     N_len: The Byte length of N; it is ignored if N == NULL.
 *                     P: The first prime factor of N. This may be NULL.
 *                     P_len: The Byte length of P; it is ignored if P == NULL.
 *                     Q: The second prime factor of N. This may be NULL.
 *                     Q_len: The Byte length of Q; it is ignored if Q == NULL.
 *                     D: The private exponent. This may be NULL.
 *                     D_len: The Byte length of D; it is ignored if D == NULL.
 *                     E: The public exponent. This may be NULL.
 *                     E_len: The Byte length of E; it is ignored if E == NULL.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The initialized RSA context to store the parameters in.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_import_raw(
    Crypto_rsa_context*  ctx,
    const unsigned char* N,
    uint32               N_len,
    const unsigned char* P,
    uint32               P_len,
    const unsigned char* Q,
    uint32               Q_len,
    const unsigned char* D,
    uint32               D_len,
    const unsigned char* E,
    uint32               E_len)
{
    int ret = 0;

    if (N != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_read_binary(&ctx->N, N, N_len));
        ctx->len = Crypto_mpi_size(&ctx->N);
    }

    if (P != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_read_binary(&ctx->P, P, P_len));
    }

    if (Q != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_read_binary(&ctx->Q, Q, Q_len));
    }

    if (D != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_read_binary(&ctx->D, D, D_len));
    }

    if (E != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_read_binary(&ctx->E, E, E_len));
    }

cleanup:

    if (ret != 0)
    {
        return E_NOT_OK;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Checks whether the context fields are set in such a way
 *                     that the RSA primitives will be able to execute without error.
 *                     It does *not* make guarantees for consistency of the parameters.
 *
 * Param-Name[in]      ctx: The RSA context.
 *                     is_priv: To indicate it is public or private.
 *                     blinding_needed: Indication for needing blinding or not.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int rsa_check_context(const Crypto_rsa_context* ctx, int is_priv, int blinding_needed)
{
#if !defined(CRYPTO_RSA_NO_CRT)
    /* blinding_needed is only used for NO_CRT to decide whether
     * P,Q need to be present or not. */
    ((void)blinding_needed);
#endif

    if (ctx->len != Crypto_mpi_size(&ctx->N) || ctx->len > CRYPTO_MPI_MAX_SIZE)
    {
        return E_NOT_OK;
    }

    /*
     * 1. Modular exponentiation needs positive, odd moduli.
     */

    /* Modular exponentiation wrt. N is always used for
     * RSA public key operations. */
    if (Crypto_mpi_cmp_int(&ctx->N, 0) <= 0 || Crypto_mpi_get_bit(&ctx->N, 0) == 0)
    {
        return E_NOT_OK;
    }

#if !defined(CRYPTO_RSA_NO_CRT)
    /* Modular exponentiation for P and Q is only
     * used for private key operations and if CRT
     * is used. */
    if (is_priv
        && (Crypto_mpi_cmp_int(&ctx->P, 0) <= 0 || Crypto_mpi_get_bit(&ctx->P, 0) == 0
            || Crypto_mpi_cmp_int(&ctx->Q, 0) <= 0 || Crypto_mpi_get_bit(&ctx->Q, 0) == 0))
    {
        return E_NOT_OK;
    }
#endif /* !CRYPTO_RSA_NO_CRT */

    /*
     * 2. Exponents must be positive
     */

    /* Always need E for public key operations */
    if (Crypto_mpi_cmp_int(&ctx->E, 0) <= 0)
    {
        return E_NOT_OK;
    }

#if defined(CRYPTO_RSA_NO_CRT)
    /* For private key operations, use D or DP & DQ
     * as (unblinded) exponents. */
    if (is_priv && Crypto_mpi_cmp_int(&ctx->D, 0) <= 0)
    {
        return E_NOT_OK;
    }
#else
    if (is_priv && (Crypto_mpi_cmp_int(&ctx->DP, 0) <= 0 || Crypto_mpi_cmp_int(&ctx->DQ, 0) <= 0))
    {
        return E_NOT_OK;
    }
#endif /* CRYPTO_RSA_NO_CRT */

    /* Blinding shouldn't make exponents negative either,
     * so check that P, Q >= 1 if that hasn't yet been
     * done as part of 1. */
#if defined(CRYPTO_RSA_NO_CRT)
    if (is_priv && blinding_needed && (Crypto_mpi_cmp_int(&ctx->P, 0) <= 0 || Crypto_mpi_cmp_int(&ctx->Q, 0) <= 0))
    {
        return E_NOT_OK;
    }
#endif

    /* It wouldn't lead to an error if it wasn't satisfied,
     * but check for QP >= 1 nonetheless. */
#if !defined(CRYPTO_RSA_NO_CRT)
    if (is_priv && Crypto_mpi_cmp_int(&ctx->QP, 0) <= 0)
    {
        return E_NOT_OK;
    }
#endif

    return 0;
}

/******************************************************************************/
/*
 * Brief               This function completes an RSA context from a set of
 *                     imported core parameters. To setup an RSA public key,
 *                     precisely N and E must have been imported. To setup an
 *                     RSA private key, sufficient information must be present
 *                     for the other parameters to be derivable. The default
 *                     implementation supports the following: Derive P,
 *                     Q from N, D, E.Derive N, D from P, Q, E. Alternative
 *                     implementations need not support these. If this function
 *                     runs successfully, it guarantees that the RSA context can
 *                     be used for RSA operations without the risk of failure or
 *                     crash.
 *
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The initialized RSA context holding imported parameters.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_complete(Crypto_rsa_context* ctx)
{
    int ret = 0;
    int have_N, have_P, have_Q, have_D, have_E;
#if !defined(CRYPTO_RSA_NO_CRT)
    int have_DP, have_DQ, have_QP;
#endif
    int n_missing, pq_missing, d_missing, is_pub, is_priv;

    have_N = (Crypto_mpi_cmp_int(&ctx->N, 0) != 0);
    have_P = (Crypto_mpi_cmp_int(&ctx->P, 0) != 0);
    have_Q = (Crypto_mpi_cmp_int(&ctx->Q, 0) != 0);
    have_D = (Crypto_mpi_cmp_int(&ctx->D, 0) != 0);
    have_E = (Crypto_mpi_cmp_int(&ctx->E, 0) != 0);

#if !defined(CRYPTO_RSA_NO_CRT)
    have_DP = (Crypto_mpi_cmp_int(&ctx->DP, 0) != 0);
    have_DQ = (Crypto_mpi_cmp_int(&ctx->DQ, 0) != 0);
    have_QP = (Crypto_mpi_cmp_int(&ctx->QP, 0) != 0);
#endif

    /*
     * Check whether provided parameters are enough
     * to deduce all others. The following incomplete
     * parameter sets for private keys are supported:
     *
     * (1) P, Q missing.
     * (2) D and potentially N missing.
     *
     */

    n_missing  = have_P && have_Q && have_D && have_E;
    pq_missing = have_N && !have_P && !have_Q && have_D && have_E;
    d_missing  = have_P && have_Q && !have_D && have_E;
    is_pub     = have_N && !have_P && !have_Q && !have_D && have_E;

    /* These three alternatives are mutually exclusive */
    is_priv = n_missing || pq_missing || d_missing;

    if (!is_priv && !is_pub)
    {
        return E_NOT_OK;
    }

    /*
     * Step 1: Deduce N if P, Q are provided.
     */

    if (!have_N && have_P && have_Q)
    {
        if ((ret = Crypto_mpi_mul_mpi(&ctx->N, &ctx->P, &ctx->Q)) != 0)
        {
            return E_NOT_OK;
        }

        ctx->len = Crypto_mpi_size(&ctx->N);
    }

    /*
     * Step 2: Deduce and verify all remaining core parameters.
     */

    if (pq_missing)
    {
        ret = Crypto_rsa_deduce_primes(&ctx->N, &ctx->E, &ctx->D, &ctx->P, &ctx->Q);
        if (ret != 0)
        {
            return E_NOT_OK;
        }
    }
    else if (d_missing)
    {
        if ((ret = Crypto_rsa_deduce_private_exponent(&ctx->P, &ctx->Q, &ctx->E, &ctx->D)) != 0)
        {
            return E_NOT_OK;
        }
    }

    /*
     * Step 3: Deduce all additional parameters specific
     *         to our current RSA implementation.
     */

#if !defined(CRYPTO_RSA_NO_CRT)
    if (is_priv && !(have_DP && have_DQ && have_QP))
    {
        ret = Crypto_rsa_deduce_crt(&ctx->P, &ctx->Q, &ctx->D, &ctx->DP, &ctx->DQ, &ctx->QP);
        if (ret != 0)
        {
            return E_NOT_OK;
        }
    }
#endif /* CRYPTO_RSA_NO_CRT */

    /*
     * Step 3: Basic sanity checks
     */

    return rsa_check_context(ctx, is_priv, 1);
}

/******************************************************************************/
/*
 * Brief               This function exports core parameters of an RSA key in
 *                     raw big-endian binary format. If this function runs
 *                     successfully, the non-NULL buffers pointed to by N,
 *                     P, Q, D, and E are fully written, with additional unused
 *                     space filled leading by zero Bytes.
 *
 * Param-Name[in]      ctx: The initialized RSA context.
 *                     N_len: The size of the buffer for the modulus.
 *                     P_len: The size of the buffer for the first prime factor.
 *                     Q_len: The size of the buffer for the second prime factor.
 *                     D_len: The size of the buffer for the private exponent.
 *                     E_len: The size of the buffer for the public exponent.
 * Param-Name[in/out]  None
 * Param-Name[out]     N: The Byte array to store the RSA modulus, or NULL if
 *                        this field need not be exported.
 *                     P: The Byte array to hold the first prime factor of N,
 *                        or NULL if this field need not be exported.
 *                     Q: The Byte array to hold the second prime factor of N,
 *                        or NULL if this field need not be exported.
 *                     D: The Byte array to hold the private exponent, or NULL
 *                        if this field need not be exported.
 *                     E: The Byte array to hold the public exponent, or NULL
 *                        if this field need not be exported.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_export_raw(
    const Crypto_rsa_context* ctx,
    unsigned char*            N,
    uint32                    N_len,
    unsigned char*            P,
    uint32                    P_len,
    unsigned char*            Q,
    uint32                    Q_len,
    unsigned char*            D,
    uint32                    D_len,
    unsigned char*            E,
    uint32                    E_len)
{
    int ret = 0;
    int is_priv;

    /* Check if key is private or public */
    is_priv = Crypto_mpi_cmp_int(&ctx->N, 0) != 0 && Crypto_mpi_cmp_int(&ctx->P, 0) != 0
              && Crypto_mpi_cmp_int(&ctx->Q, 0) != 0 && Crypto_mpi_cmp_int(&ctx->D, 0) != 0
              && Crypto_mpi_cmp_int(&ctx->E, 0) != 0;

    if (!is_priv)
    {
        /* If we're trying to export private parameters for a public key,
         * something must be wrong. */
        if (P != NULL_PTR || Q != NULL_PTR || D != NULL_PTR)
        {
            return E_NOT_OK;
        }
    }

    if (N != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_write_binary(&ctx->N, N, N_len));
    }

    if (P != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_write_binary(&ctx->P, P, P_len));
    }

    if (Q != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_write_binary(&ctx->Q, Q, Q_len));
    }

    if (D != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_write_binary(&ctx->D, D, D_len));
    }

    if (E != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_write_binary(&ctx->E, E, E_len));
    }

cleanup:

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function exports the core parameters of an RSA key.
 *                     If this function runs successfully, the non-NULL buffers
 *                     pointed to by N, P, Q, D, and E are fully written, with
 *                     additional unused space filled leading by zero Bytes.
 * Param-Name[in]      ctx: The initialized RSA context.
 * Param-Name[in/out]  None
 * Param-Name[out]     N: The MPI to hold the RSA modulus. This may be NULL if
 *                        this field need not be exported.
 *                     P: The MPI to hold the first prime factor of N. This may
 *                        be NULL if this field need not be exported.
 *                     Q: The MPI to hold the second prime factor of N. This may
 *                        be NULL if this field need not be exported.
 *                     D: The MPI to hold the private exponent. This may be NULL
 *                        if this field need not be exported.
 *                     E: The MPI to hold the public exponent. This may be NULL
 *                        if this field need not be exported.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_export(
    const Crypto_rsa_context* ctx,
    Crypto_mpi*               N,
    Crypto_mpi*               P,
    Crypto_mpi*               Q,
    Crypto_mpi*               D,
    Crypto_mpi*               E)
{
    int ret = E_NOT_OK;
    int is_priv;

    /* Check if key is private or public */
    is_priv = Crypto_mpi_cmp_int(&ctx->N, 0) != 0 && Crypto_mpi_cmp_int(&ctx->P, 0) != 0
              && Crypto_mpi_cmp_int(&ctx->Q, 0) != 0 && Crypto_mpi_cmp_int(&ctx->D, 0) != 0
              && Crypto_mpi_cmp_int(&ctx->E, 0) != 0;

    if (!is_priv)
    {
        /* If we're trying to export private parameters for a public key,
         * something must be wrong. */
        if (P != NULL_PTR || Q != NULL_PTR || D != NULL_PTR)
        {
            return E_NOT_OK;
        }
    }

    /* Export all requested core parameters. */

    if ((N != NULL_PTR && (ret = Crypto_mpi_copy(N, &ctx->N)) != 0)
        || (P != NULL_PTR && (ret = Crypto_mpi_copy(P, &ctx->P)) != 0)
        || (Q != NULL_PTR && (ret = Crypto_mpi_copy(Q, &ctx->Q)) != 0)
        || (D != NULL_PTR && (ret = Crypto_mpi_copy(D, &ctx->D)) != 0)
        || (E != NULL_PTR && (ret = Crypto_mpi_copy(E, &ctx->E)) != 0))
    {
        return ret;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Export CRT parameters. This must also be implemented if
 *                     CRT is not used, for being able to write DER encoded RSA
 *                     keys. The helper function Crypto_rsa_deduce_crt can be
 *                     used in this case.
 *
 * Param-Name[in]      ctx: The initialized RSA context.
 * Param-Name[in/out]  None
 * Param-Name[out]     DP: The MPI to hold the CRT parameter DP.
 *                     DQ: The MPI to hold the CRT parameter DQ.
 *                     QP: The MPI to hold the CRT parameter QP.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_export_crt(const Crypto_rsa_context* ctx, Crypto_mpi* DP, Crypto_mpi* DQ, Crypto_mpi* QP)
{
    int ret = E_NOT_OK;
    int is_priv;

    /* Check if key is private or public */
    is_priv = Crypto_mpi_cmp_int(&ctx->N, 0) != 0 && Crypto_mpi_cmp_int(&ctx->P, 0) != 0
              && Crypto_mpi_cmp_int(&ctx->Q, 0) != 0 && Crypto_mpi_cmp_int(&ctx->D, 0) != 0
              && Crypto_mpi_cmp_int(&ctx->E, 0) != 0;

    if (!is_priv)
    {
        return E_NOT_OK;
    }

#if !defined(CRYPTO_RSA_NO_CRT)
    /* Export all requested blinding parameters. */
    if ((DP != NULL_PTR && (ret = Crypto_mpi_copy(DP, &ctx->DP)) != 0)
        || (DQ != NULL_PTR && (ret = Crypto_mpi_copy(DQ, &ctx->DQ)) != 0)
        || (QP != NULL_PTR && (ret = Crypto_mpi_copy(QP, &ctx->QP)) != 0))
    {
        return E_NOT_OK;
    }
#else
    if ((ret = Crypto_rsa_deduce_crt(&ctx->P, &ctx->Q, &ctx->D, DP, DQ, QP)) != 0)
    {
        return E_NOT_OK;
    }
#endif

    return 0;
}

/******************************************************************************/
/*
 * Brief               Compute RSA prime factors from public and private exponents.
 *
 * Param-Name[in]      N: The MPI to hold the RSA modulus.
 *                     E: The MPI to hold the public exponent. This may be NULL
 *                        if this field need not be exported.
 *                     D: The MPI to hold the private exponent. This may be NULL
 *                        if this field need not be exported.
 * Param-Name[in/out]  None
 * Param-Name[out]     P: The MPI to hold the first prime factor of N. This may
 *                        be NULL if this field need not be exported.
 *                     Q: The MPI to hold the second prime factor of N. This may
 *                        be NULL if this field need not be exported.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_deduce_primes(
    const Crypto_mpi* N,
    const Crypto_mpi* E,
    const Crypto_mpi* D,
    Crypto_mpi*       P,
    Crypto_mpi*       Q)
{
    int ret = 0;

    uint16 attempt; /* Number of current attempt  */
    uint16 iter;    /* Number of squares computed in the current attempt */

    uint16 order; /* Order of 2 in DE - 1 */

    Crypto_mpi T; /* Holds largest odd divisor of DE - 1     */
    Crypto_mpi K; /* Temporary holding the current candidate */

    const unsigned char primes[] = {2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,
                                    47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107,
                                    109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                                    191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251};

    const uint32 num_primes = sizeof(primes) / sizeof(*primes);

    if (P == NULL_PTR || Q == NULL_PTR || P->p != NULL_PTR || Q->p != NULL_PTR)
    {
        return E_NOT_OK;
    }

    if (Crypto_mpi_cmp_int(N, 0) <= 0 || Crypto_mpi_cmp_int(D, 1) <= 0 || Crypto_mpi_cmp_mpi(D, N) >= 0
        || Crypto_mpi_cmp_int(E, 1) <= 0 || Crypto_mpi_cmp_mpi(E, N) >= 0)
    {
        return E_NOT_OK;
    }

    /*
     * Initializations and temporary changes
     */

    Crypto_mpi_init(&K);
    Crypto_mpi_init(&T);

    /* T := DE - 1 */
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&T, D, E));
    CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&T, &T, 1));

    if ((order = (uint16)Crypto_mpi_lsb(&T)) == 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    /* After this operation, T holds the largest odd divisor of DE - 1. */
    CRYPTO_MPI_CHK(Crypto_mpi_shift_r(&T, order));

    /*
     * Actual work
     */

    /* Skip trying 2 if N == 1 mod 8 */
    attempt = 0;
    if (N->p[0] % 8 == 1)
    {
        attempt = 1;
    }

    for (; attempt < num_primes; ++attempt)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_lset(&K, primes[attempt]));

        /* Check if gcd(K,N) = 1 */
        CRYPTO_MPI_CHK(Crypto_mpi_gcd(P, &K, N));
        if (Crypto_mpi_cmp_int(P, 1) != 0)
        {
            continue;
        }

        /* Go through K^T + 1, K^(2T) + 1, K^(4T) + 1, ...
         * and check whether they have nontrivial GCD with N. */
        CRYPTO_MPI_CHK(Crypto_mpi_exp_mod(&K, &K, &T, N,
                                            Q /* temporarily use Q for storing Montgomery
                                               * multiplication helper values */));

        for (iter = 1; iter <= order; ++iter)
        {
            /* If we reach 1 prematurely, there's no point
             * in continuing to square K */
            if (Crypto_mpi_cmp_int(&K, 1) == 0)
            {
                break;
            }

            CRYPTO_MPI_CHK(Crypto_mpi_add_int(&K, &K, 1));
            CRYPTO_MPI_CHK(Crypto_mpi_gcd(P, &K, N));

            if (Crypto_mpi_cmp_int(P, 1) == 1 && Crypto_mpi_cmp_mpi(P, N) == -1)
            {
                /*
                 * Have found a nontrivial divisor P of N.
                 * Set Q := N / P.
                 */

                CRYPTO_MPI_CHK(Crypto_mpi_div_mpi(Q, NULL_PTR, N, P));
                goto cleanup;
            }

            CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, &K, 1));
            CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&K, &K, &K));
            CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&K, &K, N));
        }

        /*
         * If we get here, then either we prematurely aborted the loop because
         * we reached 1, or K holds primes[attempt]^(DE - 1) mod N, which must
         * be 1 if D,E,N were consistent.
         * Check if that's the case and abort if not, to avoid very long,
         * yet eventually failing, computations if N,D,E were not sane.
         */
        if (Crypto_mpi_cmp_int(&K, 1) != 0)
        {
            break;
        }
    }

    ret = E_NOT_OK;

cleanup:

    Crypto_mpi_free(&K);
    Crypto_mpi_free(&T);
    return ret;
}

/******************************************************************************/
/*
 * Brief               Given P, Q and the public exponent E, deduce D.
 *                     This is essentially a modular inversion.
 *
 * Param-Name[in]      P: The MPI to hold the first prime factor of N. This may
 *                        be NULL if this field need not be exported.
 *                     Q: The MPI to hold the second prime factor of N. This may
 *                        be NULL if this field need not be exported.
 *                     E: The MPI to hold the public exponent.
 * Param-Name[in/out]  None
 * Param-Name[out]     D: The MPI to hold the private exponent. This may be NULL
 *                        if this field need not be exported.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_deduce_private_exponent(const Crypto_mpi* P, const Crypto_mpi* Q, const Crypto_mpi* E, Crypto_mpi* D)
{
    int        ret = 0;
    Crypto_mpi K, L;

    if (D == NULL_PTR || Crypto_mpi_cmp_int(D, 0) != 0)
    {
        return E_NOT_OK;
    }

    if (Crypto_mpi_cmp_int(P, 1) <= 0 || Crypto_mpi_cmp_int(Q, 1) <= 0 || Crypto_mpi_cmp_int(E, 0) == 0)
    {
        return E_NOT_OK;
    }

    Crypto_mpi_init(&K);
    Crypto_mpi_init(&L);

    /* Temporarily put K := P-1 and L := Q-1 */
    CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, P, 1));
    CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&L, Q, 1));

    /* Temporarily put D := gcd(P-1, Q-1) */
    CRYPTO_MPI_CHK(Crypto_mpi_gcd(D, &K, &L));

    /* K := LCM(P-1, Q-1) */
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&K, &K, &L));
    CRYPTO_MPI_CHK(Crypto_mpi_div_mpi(&K, NULL_PTR, &K, D));

    /* Compute modular inverse of E in LCM(P-1, Q-1) */
    CRYPTO_MPI_CHK(Crypto_mpi_inv_mod(D, E, &K));

cleanup:

    Crypto_mpi_free(&K);
    Crypto_mpi_free(&L);

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function initializes an RSA context.
 *
 * Param-Name[in]      mode: Indicate padding mode. It can be CRYPTO_RSA_PKCS_V15
 *                           for 1.5 padding and CRYPTO_RSA_PKCS_V21 for OAEP or
 *                           PSS.
 *                     secondaryFam: Hash identifier of Crypto_md_type_t type.
 *
 * Param-Name[in/out]  ctx: The context of RSA.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_rsa_init(Crypto_rsa_context* ctx, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    (void)IStdLib_MemSet(ctx, 0, sizeof(Crypto_rsa_context));

    ctx->padding = mode;
    ctx->hash_id = secondaryFam;
}

/******************************************************************************/
/*
 * Brief               This function sets padding for an already initialized RSA
 *                     context.
 *
 * Param-Name[in]      padding: The padding mode to use.
 *                     hash_id: The hash identifier for PSS or OAEP.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The initialized RSA context to be configured.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_set_padding(Crypto_rsa_context* ctx, int padding, Crypto_AlgorithmFamilyType hash_id)
{
    ctx->padding = padding;
    ctx->hash_id = hash_id;

    return 0;
}

/******************************************************************************/
/*
 * Brief               This function retrieves padding mode of initialized
 *                     RSA context.
 *
 * Param-Name[in]      ctx: The initialized RSA context.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_rsa_get_padding_mode(const Crypto_rsa_context* ctx)
{
    return ctx->padding;
}

/******************************************************************************/
/*
 * Brief               This function retrieves hash identifier.
 *
 * Param-Name[in]      ctx: The initialized RSA context.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              int
 */
/******************************************************************************/
int Crypto_rsa_get_md_alg(const Crypto_rsa_context* ctx)
{
    return ctx->hash_id;
}

/******************************************************************************/
/*
 * Brief               This function retrieves the length of the RSA modulus in bits.
 *
 * Param-Name[in]      ctx: The initialized RSA context.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_rsa_get_bitlen(const Crypto_rsa_context* ctx)
{
    return Crypto_mpi_bitlen(&ctx->N);
}

/******************************************************************************/
/*
 * Brief               This function retrieves the length of RSA modulus in Bytes.
 *
 * Param-Name[in]      ctx: The initialized RSA context.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_rsa_get_len(const Crypto_rsa_context* ctx)
{
    return ctx->len;
}

/******************************************************************************/
/*
 * Brief               Generate RSA-CRT parameters.
 *
 * Param-Name[in]      P: First prime factor of N.
 *                     Q: Second prime factor of N.
 *                     D: RSA private exponent.
 * Param-Name[in/out]  None
 * Param-Name[out]     DP: Output variable for D modulo P-1.
 *                     DQ: Output variable for D modulo Q-1.
 *                     QP: Output variable for the modular inverse of Q modulo P.
 * Return              int
 */
/******************************************************************************/
int Crypto_rsa_deduce_crt(
    const Crypto_mpi* P,
    const Crypto_mpi* Q,
    const Crypto_mpi* D,
    Crypto_mpi*       DP,
    Crypto_mpi*       DQ,
    Crypto_mpi*       QP)
{
    int        ret = 0;
    Crypto_mpi K;
    Crypto_mpi_init(&K);

    /* DP = D mod P-1 */
    if (DP != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, P, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(DP, D, &K));
    }

    /* DQ = D mod Q-1 */
    if (DQ != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&K, Q, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(DQ, D, &K));
    }

    /* QP = Q^{-1} mod P */
    if (QP != NULL_PTR)
    {
        CRYPTO_MPI_CHK(Crypto_mpi_inv_mod(QP, Q, P));
    }

cleanup:
    Crypto_mpi_free(&K);

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function generates an RSA keypair.
 *
 * Param-Name[in]      f_rng: The RNG function to be used for key generation.
 *                            This is mandatory and must not be NULL.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                            This may be NULL if f_rng doesn't need a context.
 *                     nbits: The size of the public key in bits.
 *                     exponent: The public exponent to use. For example, 65537.
 *                               This must be odd and greater than \c 1.
 * Param-Name[in/out]  ctx: The initialized RSA context used to hold the key.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_gen_key(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*        p_rng,
    unsigned int nbits,
    int          exponent)
{
    int        ret = E_OK;
    Crypto_mpi H, G, L;
    int        prime_quality = 0;

    /*
     * If the modulus is 1024 bit long or shorter, then the security strength of
     * the RSA algorithm is less than or equal to 80 bits and therefore an error
     * rate of 2^-80 is sufficient.
     */
    if (nbits > 1024)
    {
        prime_quality = CRYPTO_MPI_GEN_PRIME_FLAG_LOW_ERR;
    }

    Crypto_mpi_init(&H);
    Crypto_mpi_init(&G);
    Crypto_mpi_init(&L);

    if (exponent < 3 || nbits % 2 != 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    if (nbits < CRYPTO_RSA_GEN_KEY_MIN_BITS)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    /*
     * find primes P and Q with Q < P so that:
     * 1.  |P-Q| > 2^( nbits / 2 - 100 )
     * 2.  GCD( E, (P-1)*(Q-1) ) == 1
     * 3.  E^-1 mod LCM(P-1, Q-1) > 2^( nbits / 2 )
     */
    CRYPTO_MPI_CHK(Crypto_mpi_lset(&ctx->E, exponent));

    do
    {
        CRYPTO_MPI_CHK(Crypto_mpi_gen_prime(&ctx->P, nbits >> 1, prime_quality, f_rng, p_rng));

        CRYPTO_MPI_CHK(Crypto_mpi_gen_prime(&ctx->Q, nbits >> 1, prime_quality, f_rng, p_rng));

        CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&H, &ctx->P, &ctx->Q));
        if (Crypto_mpi_bitlen(&H) <= ((nbits >= 200) ? ((nbits >> 1) - 99) : 0))
        {
            continue;
        }

        /* not required by any standards, but some users rely on the fact that P > Q */
        if (H.s < 0)
        {
            Crypto_mpi_swap(&ctx->P, &ctx->Q);
        }

        /* Temporarily replace P,Q by P-1, Q-1 */
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&ctx->P, &ctx->P, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&ctx->Q, &ctx->Q, 1));
        CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&H, &ctx->P, &ctx->Q));

        /* check GCD( E, (P-1)*(Q-1) ) == 1 (FIPS 186-4 §B.3.1 criterion 2(a)) */
        CRYPTO_MPI_CHK(Crypto_mpi_gcd(&G, &ctx->E, &H));
        if (Crypto_mpi_cmp_int(&G, 1) != 0)
        {
            continue;
        }

        /* compute smallest possible D = E^-1 mod LCM(P-1, Q-1) (FIPS 186-4 §B.3.1 criterion 3(b)) */
        CRYPTO_MPI_CHK(Crypto_mpi_gcd(&G, &ctx->P, &ctx->Q));
        CRYPTO_MPI_CHK(Crypto_mpi_div_mpi(&L, NULL_PTR, &H, &G));
        CRYPTO_MPI_CHK(Crypto_mpi_inv_mod(&ctx->D, &ctx->E, &L));

        if (Crypto_mpi_bitlen(&ctx->D) <= ((nbits + 1) / 2))
        { // (FIPS 186-4 §B.3.1 criterion 3(a))
            continue;
        }

        break;
    } while (1);

    /* Restore P,Q */
    CRYPTO_MPI_CHK(Crypto_mpi_add_int(&ctx->P, &ctx->P, 1));
    CRYPTO_MPI_CHK(Crypto_mpi_add_int(&ctx->Q, &ctx->Q, 1));

    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&ctx->N, &ctx->P, &ctx->Q));

    ctx->len = Crypto_mpi_size(&ctx->N);

#if !defined(CRYPTO_RSA_NO_CRT)
    /*
     * DP = D mod (P - 1)
     * DQ = D mod (Q - 1)
     * QP = Q^-1 mod P
     */
    CRYPTO_MPI_CHK(Crypto_rsa_deduce_crt(&ctx->P, &ctx->Q, &ctx->D, &ctx->DP, &ctx->DQ, &ctx->QP));
#endif /* CRYPTO_RSA_NO_CRT */

    /* Double-check */
    CRYPTO_MPI_CHK(Crypto_rsa_check_privkey(ctx));

cleanup:

    Crypto_mpi_free(&H);
    Crypto_mpi_free(&G);
    Crypto_mpi_free(&L);

    if (ret != 0)
    {
        Crypto_mpi_free(&ctx->Vi);
        Crypto_mpi_free(&ctx->Vf);
        Crypto_mpi_free(&ctx->RN);
        Crypto_mpi_free(&ctx->D);
        Crypto_mpi_free(&ctx->Q);
        Crypto_mpi_free(&ctx->P);
        Crypto_mpi_free(&ctx->E);
        Crypto_mpi_free(&ctx->N);
        if ((-ret & ~0x7f) == 0)
        {
            ret = E_NOT_OK;
        }
        return ret;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               This function performs an RSA public key operation..
 *
 * Param-Name[in]      ctx: The initialized RSA context to use.
 *                     input: The input buffer.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The output buffer.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_public(Crypto_rsa_context* ctx, const unsigned char* input, unsigned char* output)
{
    int        ret = E_NOT_OK;
    uint32     olen;
    Crypto_mpi T;

    if (rsa_check_context(ctx, 0 /* public */, 0 /* no blinding */))
    {
        return E_NOT_OK;
    }

    Crypto_mpi_init(&T);

    CRYPTO_MPI_CHK(Crypto_mpi_read_binary(&T, input, ctx->len));

    if (Crypto_mpi_cmp_mpi(&T, &ctx->N) >= 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    olen = ctx->len;
    CRYPTO_MPI_CHK(Crypto_mpi_exp_mod(&T, &T, &ctx->E, &ctx->N, &ctx->RN));
    CRYPTO_MPI_CHK(Crypto_mpi_write_binary(&T, output, olen));

cleanup:

    Crypto_mpi_free(&T);

    if (ret != 0)
    {
        return E_NOT_OK;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Generate or update blinding values, see section 10 of:
 *                     KOCHER, Paul C. Timing attacks on implementations of
 *                     Diffie-Hellman, RSA, DSS, and other systems. In : Advances
 *                     in Cryptology-CRYPTO'96. Springer Berlin Heidelberg, 1996.
 *                     p. 104-113.
 *
 * Param-Name[in]      f_rng: The RNG function to be used for key generation.
 *                            This is mandatory and must not be NULL.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                            This may be NULL if f_rng doesn't need a context.
 * Param-Name[in/out]  ctx: The initialized RSA context used to hold the key.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int
    rsa_prepare_blinding(Crypto_rsa_context* ctx, Std_ReturnType (*f_rng)(void*, unsigned char*, uint32), void* p_rng)
{
    int        ret, count = 0;
    Crypto_mpi R;

    Crypto_mpi_init(&R);

    if (ctx->Vf.p != NULL_PTR)
    {
        /* We already have blinding values, just update them by squaring */
        CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&ctx->Vi, &ctx->Vi, &ctx->Vi));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&ctx->Vi, &ctx->Vi, &ctx->N));
        CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&ctx->Vf, &ctx->Vf, &ctx->Vf));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&ctx->Vf, &ctx->Vf, &ctx->N));

        goto cleanup;
    }

    /* Unblinding value: Vf = random number, invertible mod N */
    do
    {
        if (count++ > 10)
        {
            ret = E_NOT_OK;
            goto cleanup;
        }

        CRYPTO_MPI_CHK(Crypto_mpi_fill_random(&ctx->Vf, ctx->len - 1, f_rng, p_rng));

        /* Compute Vf^-1 as R * (R Vf)^-1 to avoid leaks from inv_mod. */
        CRYPTO_MPI_CHK(Crypto_mpi_fill_random(&R, ctx->len - 1, f_rng, p_rng));
        CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&ctx->Vi, &ctx->Vf, &R));
        CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&ctx->Vi, &ctx->Vi, &ctx->N));

        /* At this point, Vi is invertible mod N if and only if both Vf and R
         * are invertible mod N. If one of them isn't, we don't need to know
         * which one, we just loop and choose new values for both of them.
         * (Each iteration succeeds with overwhelming probability.) */
        ret = Crypto_mpi_inv_mod(&ctx->Vi, &ctx->Vi, &ctx->N);
        if (ret != 0 && ret != E_NOT_OK)
        {
            goto cleanup;
        }

    } while (ret == E_NOT_OK);

    /* Finish the computation of Vf^-1 = R * (R Vf)^-1 */
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&ctx->Vi, &ctx->Vi, &R));
    CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&ctx->Vi, &ctx->Vi, &ctx->N));

    /* Blinding value: Vi = Vf^(-e) mod N
     * (Vi already contains Vf^-1 at this point) */
    CRYPTO_MPI_CHK(Crypto_mpi_exp_mod(&ctx->Vi, &ctx->Vi, &ctx->E, &ctx->N, &ctx->RN));

cleanup:
    Crypto_mpi_free(&R);

    return ret;
}

/******************************************************************************/
/*
 * Brief               Fast Montgomery initialization.
 *
 * Param-Name[in]      N: Number input.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_mpi_core_montmul_init(const uint32* N)
{
    uint32 x = N[0];

    x += ((N[0] + 2) & 4) << 1;

    for (unsigned int i = biL; i >= 8; i /= 2)
    {
        x *= (2 - (N[0] * x));
    }

    return ~x + 1;
}

/******************************************************************************/
/*
 * Brief               Calculate the square of the Montgomery constant.
 *                     (Needed for conversion and operations in Montgomery form.)
 * Param-Name[in]      N: Little-endian presentation of the modulus, which must be
 *                          odd.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: A pointer to the result of the calculation of the square
 *                        of the Montgomery constant: 2^{2*n*biL} mod N.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_mpi_core_get_mont_r2_unsafe(Crypto_mpi* X, const Crypto_mpi* N)
{
    int ret = E_NOT_OK;

    CRYPTO_MPI_CHK(Crypto_mpi_lset(X, 1));
    CRYPTO_MPI_CHK(Crypto_mpi_shift_l(X, N->n * 2 * biL));
    CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(X, X, N));
    CRYPTO_MPI_CHK(Crypto_mpi_shrink(X, N->n));

cleanup:
    return ret;
}

/******************************************************************************/
/*
 * Brief               Convert an MPI into Montgomery form. X may be aliased to A,
 *                     but may not otherwise overlap it. X may not alias N (it is
 *                     in canonical form, so must be strictly less than N). Nor
 *                     may it alias or overlap rr (this is unlikely to be required
 *                     in practice.)
 *
 * Param-Name[in]      A: The MPI to convert into Montgomery form. Must have the
 *                        same number of limbs as the modulus.
 *                     N: The address of the modulus, which gives the size of the
 *                        base R = 2^(biL*N->limbs).
 *                     AN_limbs: The number of limbs in X, A, N and rr.
 *                     mm: The Montgomery constant for N: -N^-1 mod 2^biL.
 *                     rr: The residue for 2^{2*n*biL} mod N.
 *                     T: Temporary storage. Its initial content is unused and its
 *                        final content is indeterminate. It must not alias or
 *                        otherwise overlap any of the other parameters.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The result of the conversion. Must have the same number
 *                        of limbs as A.
 * Return              None
 */
/******************************************************************************/
void Crypto_mpi_core_to_mont_rep(
    uint32*       X,
    const uint32* A,
    const uint32* N,
    uint32        AN_limbs,
    uint32        mm,
    const uint32* rr,
    uint32*       T)
{
    Crypto_mpi_core_montmul(X, A, rr, AN_limbs, N, AN_limbs, mm, T);
}
/******************************************************************************/
/*
 * Brief    Conversion from Montgomery to standard representation
 */
/******************************************************************************/
void Crypto_mpi_core_from_mont_rep(uint32* X, const uint32* A, const uint32* N, uint32 AN_limbs, uint32 mm, uint32* T)
{
    const uint32 Rinv = 1; /* 1/R in Mont. rep => 1 */

    Crypto_mpi_core_montmul(X, A, &Rinv, 1, N, AN_limbs, mm, T);
}
/* Exponentiation: X := A^E mod N.
 *
 * A must already be in Montgomery form.
 *
 * As in other bignum functions, assume that AN_limbs and E_limbs are nonzero.
 *
 * RR must contain 2^{2*biL} mod N.
 *
 * The algorithm is a variant of Left-to-right k-ary exponentiation: HAC 14.82
 * (The difference is that the body in our loop processes a single bit instead
 * of a full window.)
 */
void Crypto_mpi_core_exp_mod(
    uint32*       X,
    const uint32* A,
    const uint32* N,
    uint32        AN_limbs,
    const uint32* E,
    uint32        E_limbs,
    const uint32* RR,
    uint32*       T)
{
    const uint32 wsize = ((E_limbs * biL) > 79) ? CRYPTO_MPI_WINDOW_SIZE : 1;
    const uint32 welem = ((uint32)1) << wsize;

    /* This is how we will use the temporary storage T, which must have space
     * for table_limbs, select_limbs and (2 * AN_limbs + 1) for montmul. */
    const uint32 table_limbs  = welem * AN_limbs;
    const uint32 select_limbs = AN_limbs;

    /* Pointers to specific parts of the temporary working memory pool */
    uint32* const Wtable  = T;
    uint32* const Wselect = Wtable + table_limbs;
    uint32* const temp    = Wselect + select_limbs;

    /*
     * Window precomputation
     */

    const uint32 mm = Crypto_mpi_core_montmul_init(N);

    /* Set Wtable[i] = A^(2^i) (in Montgomery representation) */
    exp_mod_precompute_window(A, N, AN_limbs, mm, RR, welem, Wtable, temp);

    /*
     * Fixed window exponentiation
     */

    /* X = 1 (in Montgomery presentation) initially */
    (void)IStdLib_MemCpy(X, Wtable, AN_limbs * ciL);

    /* We'll process the bits of E from most significant
     * (limb_index=E_limbs-1, E_bit_index=biL-1) to least significant
     * (limb_index=0, E_bit_index=0). */
    uint32 E_limb_index = E_limbs;
    uint32 E_bit_index  = 0;
    /* At any given time, window contains window_bits bits from E.
     * window_bits can go up to wsize. */
    uint32 window_bits = 0;
    uint32 window      = 0;

    do
    {
        /* Square */
        Crypto_mpi_core_montmul(X, X, X, AN_limbs, N, AN_limbs, mm, temp);

        /* Move to the next bit of the exponent */
        if (E_bit_index == 0)
        {
            --E_limb_index;
            E_bit_index = biL - 1;
        }
        else
        {
            --E_bit_index;
        }
        /* Insert next exponent bit into window */
        ++window_bits;
        window <<= 1;
        window |= (E[E_limb_index] >> E_bit_index) & 1;

        /* Clear window if it's full. Also clear the window at the end,
         * when we've finished processing the exponent. */
        if (window_bits == wsize || (E_bit_index == 0 && E_limb_index == 0))
        {
            /* Select Wtable[window] without leaking window through
             * memory access patterns. */
            Crypto_mpi_core_ct_uint_table_lookup(Wselect, Wtable, AN_limbs, welem, window);
            /* Multiply X by the selected element. */
            Crypto_mpi_core_montmul(X, X, Wselect, AN_limbs, N, AN_limbs, mm, temp);
            window      = 0;
            window_bits = 0;
        }
    } while (!(E_bit_index == 0 && E_limb_index == 0));
}
/******************************************************************************/
/*
 * Brief               Subtract two fixed-size large unsigned integers, returning
 *                      the borrow. Calculate A - B where A and B have the same
 *                     size. This function operates modulo 2^(biL*limbs) and
 *                     returns the carry (1 if there was a wraparound, i.e. if A < B,
 *                     and 0 otherwise). X may be aliased to A or B, or even both,
 *                     but may not overlap either otherwise.
 *
 * Param-Name[in]      A: Little-endian presentation of left operand.
 *                     B: Little-endian presentation of right operand.
 *                     limbs: Number of limbs of X, A and B.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The result of the subtraction.
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_mpi_core_sub(uint32* X, const uint32* A, const uint32* B, uint32 limbs)
{
    uint32 c = 0;

    for (uint32 i = 0; i < limbs; i++)
    {
        uint32 z = (A[i] < c);
        uint32 t = A[i] - c;
        c        = (t < B[i]) + z;
        X[i]     = t - B[i];
    }

    return c;
}
/******************************************************************************/
/*
 * Brief  Multi-precision integer minus single-precision integer
 *
 * Param-Name[in]   A:Subtracted number (multi-precision integer)
 *                  c:Decrement (single precision integer)
 *                  limbs:Length of the A and X arrays
 * Param-Name[in/out]  None
 * Param-Name[out]   X:Target array for storing the result of the operation
 * Return
 */
/******************************************************************************/
uint32 Crypto_mpi_core_sub_int(
    uint32*       X,
    const uint32* A,
    uint32        c, /* doubles as carry */
    uint32        limbs)
{
    for (uint32 i = 0; i < limbs; i++)
    {
        uint32 s = A[i];
        uint32 t = s - c;
        c        = (t > s);
        X[i]     = t;
    }

    return c;
}
/******************************************************************************/
/*
 * Brief    Check if a multi-precision integer is all 0
 *
 * Param-Name[in] A: Multi-precision integer array to be checked
 *                limbs:Length of array A
 * Param-Name[in/out]  None
 * Param-Name[out] None
 * Return Crypto_ct_bool(bits)
 */
/******************************************************************************/
uint32 Crypto_mpi_core_check_zero_ct(const uint32* A, uint32 limbs)
{
    const volatile uint32* force_read_A = A;
    uint32                 bits         = 0;

    for (uint32 i = 0; i < limbs; i++)
    {
        bits |= force_read_A[i];
    }

    return Crypto_ct_bool(bits);
}
/******************************************************************************/
/*
 * Brief    Pre-calculate a window table to speed up subsequent modulo operations.
 *
 * Param-Name[in]   A:The base to be modulo idempotent
 *                  N:modulus
 *                  AN_limbs:Length of the A and N arrays
 *                  mm:Constants for multiplication
 *                  RR:The RR constant used in multiplication
 *                  welem:Number of window elements to count
 *                  temp:Buffer for temporary storage
 * Param-Name[in/out]  None
 * Param-Name[out]  Wtable: Window table for storing calculation results
 * Return
 */
/******************************************************************************/
CRYPTO_62_LOCAL void exp_mod_precompute_window(
    const uint32* A,
    const uint32* N,
    uint32        AN_limbs,
    uint32        mm,
    const uint32* RR,
    uint32        welem,
    uint32*       Wtable,
    uint32*       temp)
{
    /* W[0] = 1 (in Montgomery presentation) */
    (void)IStdLib_MemSet(Wtable, 0, AN_limbs * ciL);
    Wtable[0] = 1;
    Crypto_mpi_core_montmul(Wtable, Wtable, RR, AN_limbs, N, AN_limbs, mm, temp);

    /* W[1] = A (already in Montgomery presentation) */
    uint32* W1 = Wtable + AN_limbs;
    (void)IStdLib_MemCpy(W1, A, AN_limbs * ciL);

    /* W[i+1] = W[i] * W[1], i >= 2 */
    uint32* Wprev = W1;
    for (uint32 i = 2; i < welem; i++)
    {
        uint32* Wcur = Wprev + AN_limbs;
        Crypto_mpi_core_montmul(Wcur, Wprev, W1, AN_limbs, N, AN_limbs, mm, temp);
        Wprev = Wcur;
    }
}
/******************************************************************************/
/*
 * Brief  Perform a conditional table lookup operation
 *
 * Param-Name[in]  table：Pointer to the input form
 *                 limbs：This is the length of each data item
 *                 count：Total number of data items in the table
 *                 index：Index value to look up
 * Param-Name[in/out]  None
 * Param-Name[out] dest:Storing search results
 * Return
 */
/******************************************************************************/
CRYPTO_62_LOCAL void
    Crypto_mpi_core_ct_uint_table_lookup(uint32* dest, const uint32* table, uint32 limbs, uint32 count, uint32 index)
{
    for (uint32 i = 0; i < count; i++, table += limbs)
    {
        uint32 assign = Crypto_ct_uint_eq(i, index);
        Crypto_mpi_core_cond_assign(dest, table, limbs, assign);
    }
}
/******************************************************************************/
/*
 * Brief   Conditionally copying an array A into an array X
 *
 * Param-Name[in]   A：Pointer to source array
 *                  limbs：Length of the array
 *                  assign：Conditional flag to decide whether to copy A to X.
 * Param-Name[in/out]  None
 * Param-Name[out] X：Pointer to the target array, elements of the array will be conditionally assigned values
 * Return
 */
/******************************************************************************/
void Crypto_mpi_core_cond_assign(uint32* X, const uint32* A, uint32 limbs, uint32 assign)
{
    if (X == A)
    {
        return;
    }

    /* This function is very performance-sensitive for RSA. For this reason
     * we have the loop below, instead of calling Crypto_ct_memcpy_if
     * (this is more optimal since here we don't have to handle the case where
     * we copy awkwardly sized data).
     */
    for (uint32 i = 0; i < limbs; i++)
    {
        X[i] = Crypto_ct_if(assign, A[i], X[i]);
    }
}
/******************************************************************************/
/*
 * Brief  Determine the amount of working memory required to perform modulo power operations
 *
 * Param-Name[in]      AN_limbs：Bit length of base A and modulus N
 *                     E_limbs：The bit length of the index E
 * Param-Name[in/out]  None
 * Param-Name[out] None
 * Return
 */
/******************************************************************************/
uint32 Crypto_mpi_core_exp_mod_working_limbs(uint32 AN_limbs, uint32 E_limbs)
{
    const uint32 wsize = ((E_limbs * biL) > 79) ? CRYPTO_MPI_WINDOW_SIZE : 1;
    const uint32 welem = ((uint32)1) << wsize;

    /* How big does each part of the working memory pool need to be? */
    const uint32 table_limbs  = welem * AN_limbs;
    const uint32 select_limbs = AN_limbs;
    const uint32 temp_limbs   = 2 * AN_limbs + 1;

    return table_limbs + select_limbs + temp_limbs;
}
/******************************************************************************/
/*
 * Brief               Perform a fixed-size multiply accumulate operation:
 *                     d += b * s d may be aliased to s (when d_limbs == s_limbs),
 *                     but may not otherwise overlap. This function operates
 *                     modulo 2^(biL*d_limbs).
 *
 * Param-Name[in]      d_len: The number of limbs of d.
 *                     s: The pointer to the (little-endian) array representing
 *                        the bignum to multiply with.
 *                     s_len: The number of limbs of A.
 *                     b: d scalar to multiply with.
 * Param-Name[in/out]  d: The pointer to the (little-endian) array representing
 *                        the bignum to accumulate onto.
 * Param-Name[out]     None
 * Return              uint32
 */
/******************************************************************************/
uint32 Crypto_mpi_core_mla(uint32* d, uint32 d_len, const uint32* s, uint32 s_len, uint32 b)
{
    uint32 c = 0; /* carry */
    /*
     * It is a documented precondition of this function that d_len >= s_len.
     * If that's not the case, we swap these round: this turns what would be
     * a buffer overflow into an incorrect result.
     */
    if (d_len < s_len)
    {
        s_len = d_len;
    }
    uint32 excess_len = d_len - s_len;
    uint32 steps_x8   = s_len / 8;
    uint32 steps_x1   = s_len & 7;

    while (steps_x8--)
    {
        MULADDC_X8_INIT
        MULADDC_X8_CORE
        MULADDC_X8_STOP
    }

    while (steps_x1--)
    {
        MULADDC_X1_INIT
        MULADDC_X1_CORE
        MULADDC_X1_STOP
    }

    while (excess_len--)
    {
        *d += c;
        c = (*d < c);
        d++;
    }

    return c;
}

/******************************************************************************/
/*
 * Brief               Conditional memcpy. Functionally equivalent to: if
 *                      (condition) { memcpy(dest, src1, len); } else { if
 *                     (src2 != NULL) memcpy(dest, src2, len); } It will always
 *                     read len bytes from src1. If src2 != NULL, it will always
 *                     read len bytes from src2. If src2 == NULL, it will instead
 *                     read len bytes from dest (as if src2 == dest).
 *
 * Param-Name[in]      condition: The condition.
 *                     src1: Pointer to copy from. This may be equal to dest,
 *                            but may not overlap in other ways.
 *                     src2: Pointer to copy from. May be NULL. This may be
 *                           equal to dest, but may not overlap it in other ways.
 *                           It may overlap with src1.
 *                     len: Number of bytes to copy.
 * Param-Name[in/out]  None
 * Param-Name[out]     dest: Destination pointer.
 * Return              None
 */
/******************************************************************************/
void Crypto_ct_memcpy_if(
    uint64               condition,
    unsigned char*       dest,
    const unsigned char* src1,
    const unsigned char* src2,
    uint32               len)
{
#if defined(Crypto_CT_SIZE_64)
    const uint64_t mask     = (uint64_t)condition;
    const uint64_t not_mask = (uint64_t)~Crypto_ct_compiler_opaque(condition);
#else
    const uint32 mask     = (uint32)condition;
    const uint32 not_mask = (uint32)~Crypto_ct_compiler_opaque(condition);
#endif

    /* If src2 is NULL, setup src2 so that we read from the destination address.
     *
     * This means that if src2 == NULL && condition is false, the result will be a
     * no-op because we read from dest and write the same data back into dest.
     */
    if (src2 == NULL_PTR)
    {
        src2 = dest;
    }

    /* dest[i] = c1 == c2 ? src[i] : dest[i] */
    uint32 i = 0;

#if defined(Crypto_CT_SIZE_64)
    for (; (i + 8) <= len; i += 8)
    {
        uint64_t a = Crypto_get_unaligned_uint64(src1 + i) & mask;
        uint64_t b = Crypto_get_unaligned_uint64(src2 + i) & not_mask;
        Crypto_put_unaligned_uint64(dest + i, a | b);
    }
#else
    for (; (i + 4) <= len; i += 4)
    {
        uint32 a = Crypto_Get_Unaligned_Uint32(src1 + i) & mask;
        uint32 b = Crypto_Get_Unaligned_Uint32(src2 + i) & not_mask;
        Crypto_put_unaligned_uint32(dest + i, a | b);
    }
#endif /* defined(Crypto_CT_SIZE_64) */

    for (; i < len; i++)
    {
        dest[i] = (src1[i] & mask) | (src2[i] & not_mask);
    }
}

/******************************************************************************/
/*
 * Brief               Montgomery multiplication: X = A * B * R^-1 mod N (HAC 14.36)
 *                      A and B must be in canonical form. That is, < N. X may be
 *                     aliased to A or N, or even B (if AN_limbs == B_limbs) but may
 *                     not overlap any parameters otherwise. A and B may alias each
 *                     other, if AN_limbs == B_limbs. They may not alias N (since
 *                     they must be in canonical form, they cannot == N).
 * Param-Name[in]      A: Little-endian presentation of first operand. Must have
 *                        the same number of limbs as N.
 *                     B: Little-endian presentation of second operand.
 *                     B_limbs: The number of limbs in B. Must be <= AN_limbs.
 *                     N: Little-endian presentation of the modulus. This must be odd,
 *                        and have exactly the same number of limbs as A. It may alias
 *                        X, but must not alias or otherwise overlap any of the other
 *                        parameters.
 *                     AN_limbs: The number of limbs in X, A and N.
 *                     mm: The Montgomery constant for N: -N^-1 mod 2^biL.
 *                     T: Temporary storage of size at least 2*AN_limbs+1 limbs. Its
 *                        initial content is unused and its final content is
 *                        indeterminate. It must not alias or otherwise overlap any
 *                        of the other parameters.
 * Param-Name[in/out]  None
 * Param-Name[out]     X: The destination MPI, as a little-endian array of length
 *                        AN_limbs. On successful completion, X contains the
 *                        result of the multiplication A * B * R^-1 mod N where
 *                        R = 2^(biL*AN_limbs).
 * Return              None
 */
/******************************************************************************/
void Crypto_mpi_core_montmul(
    uint32*       X,
    const uint32* A,
    const uint32* B,
    uint32        B_limbs,
    const uint32* N,
    uint32        AN_limbs,
    uint32        mm,
    uint32*       T)
{
    (void)IStdLib_MemSet(T, 0, (2 * AN_limbs + 1) * ciL);

    for (uint32 i = 0; i < AN_limbs; i++)
    {
        /* T = (T + u0*B + u1*N) / 2^biL */
        uint32 u0 = A[i];
        uint32 u1 = (T[0] + u0 * B[0]) * mm;

        (void)Crypto_mpi_core_mla(T, AN_limbs + 2, B, B_limbs, u0);
        (void)Crypto_mpi_core_mla(T, AN_limbs + 2, N, AN_limbs, u1);

        T++;
    }

    /*
     * The result we want is (T >= N) ? T - N : T.
     *
     * For better constant-time properties in this function, we always do the
     * subtraction, with the result in X.
     *
     * We also look to see if there was any carry in the final additions in the
     * loop above.
     */

    uint32 carry  = T[AN_limbs];
    uint32 borrow = Crypto_mpi_core_sub(X, T, N, AN_limbs);

    /*
     * Using R as the Montgomery radix (auxiliary modulus) i.e. 2^(biL*AN_limbs):
     *
     * T can be in one of 3 ranges:
     *
     * 1) T < N      : (carry, borrow) = (0, 1): we want T
     * 2) N <= T < R : (carry, borrow) = (0, 0): we want X
     * 3) T >= R     : (carry, borrow) = (1, 1): we want X
     *
     * and (carry, borrow) = (1, 0) can't happen.
     *
     * So the correct return value is already in X if (carry ^ borrow) = 0,
     * but is in (the lower AN_limbs limbs of) T if (carry ^ borrow) = 1.
     */
    Crypto_ct_memcpy_if(
        Crypto_ct_bool(carry ^ borrow),
        (unsigned char*)X,
        (unsigned char*)T,
        NULL_PTR,
        AN_limbs * sizeof(uint32));
}

/******************************************************************************/
/*
 * Brief               Calculate T = T * Vf mod N.
 *
 * Param-Name[in]      Vf: The multiplier mpi.
 *                     N: The modulus mpi.
 * Param-Name[in/out]  T: The result mpi.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int rsa_unblind(Crypto_mpi* T, Crypto_mpi* Vf, const Crypto_mpi* N)
{
    int          ret    = E_NOT_OK;
    const uint32 mm     = Crypto_mpi_core_montmul_init(N->p);
    const uint32 nlimbs = N->n;
    const uint32 tlimbs = Crypto_mpi_core_montmul_working_limbs(nlimbs);
    Crypto_mpi   RR, M_T;

    Crypto_mpi_init(&RR);
    Crypto_mpi_init(&M_T);

    CRYPTO_MPI_CHK(Crypto_mpi_core_get_mont_r2_unsafe(&RR, N));
    CRYPTO_MPI_CHK(Crypto_mpi_grow(&M_T, tlimbs));

    CRYPTO_MPI_CHK(Crypto_mpi_grow(T, nlimbs));
    CRYPTO_MPI_CHK(Crypto_mpi_grow(Vf, nlimbs));

    /* T = T * Vf mod N
     * Reminder: montmul(A, B, N) = A * B * R^-1 mod N
     * Usually both operands are multiplied by R mod N beforehand (by calling
     * `to_mont_rep()` on them), yielding a result that's also * R mod N (aka
     * "in the Montgomery domain"). Here we only multiply one operand by R mod
     * N, so the result is directly what we want - no need to call
     * `from_mont_rep()` on it. */
    Crypto_mpi_core_to_mont_rep(T->p, T->p, N->p, nlimbs, mm, RR.p, M_T.p);
    Crypto_mpi_core_montmul(T->p, T->p, Vf->p, nlimbs, N->p, nlimbs, mm, M_T.p);

cleanup:

    Crypto_mpi_free(&RR);
    Crypto_mpi_free(&M_T);

    return ret;
}

#define RSA_EXPONENT_BLINDING 28

/******************************************************************************/
/*
 * Brief               Do an RSA private key operation.
 *
 * Param-Name[in]      f_rng: The RNG function to be used for key generation.
 *                            This is mandatory and must not be NULL.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                            This may be NULL if f_rng doesn't need a context.
 *                     input: Data input.
 *                     ctx: The RSA context.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: Buffer to store the output.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_private(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    const unsigned char* input,
    unsigned char*       output)
{
    int    ret = E_NOT_OK;
    uint32 olen;

    /* Temporary holding the result */
    Crypto_mpi T;

    /* Temporaries holding P-1, Q-1 and the
     * exponent blinding factor, respectively. */
    Crypto_mpi P1, Q1, R;

#if !defined(CRYPTO_RSA_NO_CRT)
    /* Temporaries holding the results mod p resp. mod q. */
    Crypto_mpi TP, TQ;

    /* Temporaries holding the blinded exponents for
     * the mod p resp. mod q computation (if used). */
    Crypto_mpi DP_blind, DQ_blind;
#else
    /* Temporary holding the blinded exponent (if used). */
    Crypto_mpi D_blind;
#endif /* CRYPTO_RSA_NO_CRT */

    /* Temporaries holding the initial input and the double
     * checked result; should be the same in the end. */
    Crypto_mpi input_blinded, check_result_blinded;

    if (f_rng == NULL_PTR)
    {
        return E_NOT_OK;
    }

    if (rsa_check_context(ctx, 1 /* private key checks */, 1 /* blinding on        */) != 0)
    {
        return E_NOT_OK;
    }

    /* MPI Initialization */
    Crypto_mpi_init(&T);

    Crypto_mpi_init(&P1);
    Crypto_mpi_init(&Q1);
    Crypto_mpi_init(&R);

#if defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_init(&D_blind);
#else
    Crypto_mpi_init(&DP_blind);
    Crypto_mpi_init(&DQ_blind);
#endif

#if !defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_init(&TP);
    Crypto_mpi_init(&TQ);
#endif

    Crypto_mpi_init(&input_blinded);
    Crypto_mpi_init(&check_result_blinded);

    /* End of MPI initialization */

    CRYPTO_MPI_CHK(Crypto_mpi_read_binary(&T, input, ctx->len));
    if (Crypto_mpi_cmp_mpi(&T, &ctx->N) >= 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    /*
     * Blinding
     * T = T * Vi mod N
     */
    CRYPTO_MPI_CHK(rsa_prepare_blinding(ctx, f_rng, p_rng));
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&T, &T, &ctx->Vi));
    CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&T, &T, &ctx->N));

    CRYPTO_MPI_CHK(Crypto_mpi_copy(&input_blinded, &T));

    /*
     * Exponent blinding
     */
    CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&P1, &ctx->P, 1));
    CRYPTO_MPI_CHK(Crypto_mpi_sub_int(&Q1, &ctx->Q, 1));

#if defined(CRYPTO_RSA_NO_CRT)
    /*
     * D_blind = ( P - 1 ) * ( Q - 1 ) * R + D
     */
    CRYPTO_MPI_CHK(Crypto_mpi_fill_random(&R, RSA_EXPONENT_BLINDING, f_rng, p_rng));
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&D_blind, &P1, &Q1));
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&D_blind, &D_blind, &R));
    CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(&D_blind, &D_blind, &ctx->D));
#else
    /*
     * DP_blind = ( P - 1 ) * R + DP
     */
    CRYPTO_MPI_CHK(Crypto_mpi_fill_random(&R, RSA_EXPONENT_BLINDING, f_rng, p_rng));
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&DP_blind, &P1, &R));
    CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(&DP_blind, &DP_blind, &ctx->DP));

    /*
     * DQ_blind = ( Q - 1 ) * R + DQ
     */
    CRYPTO_MPI_CHK(Crypto_mpi_fill_random(&R, RSA_EXPONENT_BLINDING, f_rng, p_rng));
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&DQ_blind, &Q1, &R));
    CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(&DQ_blind, &DQ_blind, &ctx->DQ));
#endif /* CRYPTO_RSA_NO_CRT */

#if defined(CRYPTO_RSA_NO_CRT)
    CRYPTO_MPI_CHK(Crypto_mpi_exp_mod(&T, &T, &D_blind, &ctx->N, &ctx->RN));
#else
    /*
     * Faster decryption using the CRT
     *
     * TP = input ^ dP mod P
     * TQ = input ^ dQ mod Q
     */

    CRYPTO_MPI_CHK(Crypto_mpi_exp_mod(&TP, &T, &DP_blind, &ctx->P, &ctx->RP));
    CRYPTO_MPI_CHK(Crypto_mpi_exp_mod(&TQ, &T, &DQ_blind, &ctx->Q, &ctx->RQ));

    /*
     * T = (TP - TQ) * (Q^-1 mod P) mod P
     */
    CRYPTO_MPI_CHK(Crypto_mpi_sub_mpi(&T, &TP, &TQ));
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&TP, &T, &ctx->QP));
    CRYPTO_MPI_CHK(Crypto_mpi_mod_mpi(&T, &TP, &ctx->P));

    /*
     * T = TQ + T * Q
     */
    CRYPTO_MPI_CHK(Crypto_mpi_mul_mpi(&TP, &T, &ctx->Q));
    CRYPTO_MPI_CHK(Crypto_mpi_add_mpi(&T, &TQ, &TP));
#endif /* CRYPTO_RSA_NO_CRT */

    /* Verify the result to prevent glitching attacks. */
    CRYPTO_MPI_CHK(Crypto_mpi_exp_mod(&check_result_blinded, &T, &ctx->E, &ctx->N, &ctx->RN));
    if (Crypto_mpi_cmp_mpi(&check_result_blinded, &input_blinded) != 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    /*
     * Unblind
     * T = T * Vf mod N
     */
    CRYPTO_MPI_CHK(rsa_unblind(&T, &ctx->Vf, &ctx->N));

    olen = ctx->len;
    CRYPTO_MPI_CHK(Crypto_mpi_write_binary(&T, output, olen));

cleanup:

    Crypto_mpi_free(&P1);
    Crypto_mpi_free(&Q1);
    Crypto_mpi_free(&R);

#if defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_free(&D_blind);
#else
    Crypto_mpi_free(&DP_blind);
    Crypto_mpi_free(&DQ_blind);
#endif

    Crypto_mpi_free(&T);

#if !defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_free(&TP);
    Crypto_mpi_free(&TQ);
#endif

    Crypto_mpi_free(&check_result_blinded);
    Crypto_mpi_free(&input_blinded);

    if (ret != 0 && ret >= -0x007f)
    {
        return E_NOT_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               Generate and apply the MGF1 operation (from PKCS#1 v2.1)
 *                     to a buffer.
 *
 * Param-Name[in]      dlen: Length of destination buffer.
 *                     src: Source of the mask generation.
 *                     slen: Length of the source buffer.
 *                     md_alg: message digest to use.
 * Param-Name[in/out]  None
 * Param-Name[out]     dst: Buffer to mask.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int
    mgf_mask(unsigned char* dst, uint32 dlen, unsigned char* src, uint32 slen, Crypto_AlgorithmFamilyType md_alg)
{
    unsigned char           counter[CRYPTO_CONST_4];
    unsigned char*          p;
    unsigned int            hlen;
    uint32                  i, use_len;
    unsigned char           mask[CRYPTO_MD_MAX_SIZE];
    int                     ret = 0;
    const Crypto_Md_Info_t* md_info;
    Crypto_Md_Context_t     md_ctx;

    (void)IStdLib_MemSet(&md_ctx, 0, sizeof(Crypto_Md_Context_t));
    md_info = Crypto_Md_info_from_type(md_alg);
    if (md_info == NULL_PTR)
    {
        return E_NOT_OK;
    }

    (void)IStdLib_MemSet(&md_ctx, 0, sizeof(Crypto_Md_Context_t));
    if ((ret = Crypto_md_setup(&md_ctx, md_info)) != 0)
    {
        goto exit;
    }

    hlen = Crypto_md_get_size(md_info);

    (void)IStdLib_MemSet(mask, 0, sizeof(mask));
    (void)IStdLib_MemSet(counter, 0, 4);

    /* Generate and apply dbMask */
    p = dst;

    while (dlen > 0)
    {
        use_len = hlen;
        if (dlen < hlen)
        {
            use_len = dlen;
        }

        if ((ret = Crypto_Md_Start(&md_ctx)) != 0)
        {
            goto exit;
        }
        if ((ret = Crypto_Md_Update(&md_ctx, src, slen)) != 0)
        {
            goto exit;
        }
        if ((ret = Crypto_Md_Update(&md_ctx, counter, 4)) != 0)
        {
            goto exit;
        }
        if ((ret = Crypto_Md_Finish(&md_ctx, mask)) != 0)
        {
            goto exit;
        }

        for (i = 0; i < use_len; ++i)
        {
            *p++ ^= mask[i];
        }

        counter[3]++;

        dlen -= use_len;
    }

exit:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Generate Hash(M') as in RFC 8017 page 43 points 5 and 6.
 *
 * Param-Name[in]      hash: The input hash.
 *                     hlen: Length of the input hash.
 *                     salt: The input salt.
 *                     slen: Length of the input salt.
 *                     md_alg: Message digest to use.
 * Param-Name[in/out]  None
 * Param-Name[out]     out: The output buffer - must be large enough for md_alg
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int hash_mprime(
    const unsigned char*       hash,
    uint32                     hlen,
    const unsigned char*       salt,
    uint32                     slen,
    unsigned char*             out,
    Crypto_AlgorithmFamilyType md_alg)
{
    const unsigned char zeros[CRYPTO_CONST_8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0};

    Crypto_Md_Context_t md_ctx;
    int                 ret = E_NOT_OK;

    const Crypto_Md_Info_t* md_info = Crypto_Md_info_from_type(md_alg);
    if (md_info == NULL_PTR)
    {
        return E_NOT_OK;
    }

    (void)IStdLib_MemSet(&md_ctx, 0, sizeof(Crypto_Md_Context_t));
    if ((ret = Crypto_md_setup(&md_ctx, md_info)) != 0)
    {
        goto exit;
    }
    if ((ret = Crypto_Md_Start(&md_ctx)) != 0)
    {
        goto exit;
    }
    if ((ret = Crypto_Md_Update(&md_ctx, zeros, sizeof(zeros))) != 0)
    {
        goto exit;
    }
    if ((ret = Crypto_Md_Update(&md_ctx, hash, hlen)) != 0)
    {
        goto exit;
    }
    if ((ret = Crypto_Md_Update(&md_ctx, salt, slen)) != 0)
    {
        goto exit;
    }
    if ((ret = Crypto_Md_Finish(&md_ctx, out)) != 0)
    {
        goto exit;
    }

exit:

    return ret;
}

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

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v2.1 OAEP encryption
 *                     operation (RSAES-OAEP-ENCRYPT).
 *
 * Param-Name[in]      ctx: The initialized RSA context to use.
 *                     f_rng: The RNG function to use. This is needed for padding
 *                            generation and is mandatory.
 *                     p_rng: The RNG context to be passed to f_rng. This may
 *                            be  NULL if f_rng doesn't need a context argument.
 *                     label: The buffer holding the custom label to use.
 *                            This must be a readable buffer of length label_len.
 *                     label_len: The length of the label in Bytes.
 *                     ilen: The length of the plaintext buffer input in Bytes.
 *                     input: The input data to encrypt. This must be a readable
 *                            buffer of size ilen Bytes. It may be NULL if
 *                            `ilen == 0`.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The output buffer. This must be a writable buffer
 *                             of length ctx->len Bytes. For example, 256 Bytes
 *                             for an 2048-bit RSA modulus.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsaes_oaep_encrypt(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    const unsigned char* label,
    uint32               label_len,
    uint32               ilen,
    const unsigned char* input,
    unsigned char*       output)
{
    uint32         olen;
    int            ret = E_NOT_OK;
    unsigned char* p   = output;
    unsigned int   hlen;

    if (f_rng == NULL_PTR)
    {
        return E_NOT_OK;
    }

    hlen = Crypto_md_get_size_from_type((Crypto_AlgorithmFamilyType)ctx->hash_id);
    if (hlen == 0)
    {
        return E_NOT_OK;
    }

    olen = ctx->len;

    /* first comparison checks for overflow */
    if (ilen + 2 * hlen + 2 < ilen || olen < ilen + 2 * hlen + 2)
    {
        return E_NOT_OK;
    }

    (void)IStdLib_MemSet(output, 0, olen);

    *p++ = 0;

    /* Generate a random octet string seed */
    if ((ret = f_rng(p_rng, p, hlen)) != 0)
    {
        return E_NOT_OK;
    }

    p += hlen;

    /* Construct DB */
    ret = compute_hash((Crypto_AlgorithmFamilyType)ctx->hash_id, label, label_len, p);
    if (ret != 0)
    {
        return ret;
    }
    p += hlen;
    p += olen - 2 * hlen - 2 - ilen;
    *p++ = 1;
    if (ilen != 0)
    {
        (void)IStdLib_MemCpy(p, input, ilen);
    }

    /* maskedDB: Apply dbMask to DB */
    if ((ret = mgf_mask(output + hlen + 1, olen - hlen - 1, output + 1, hlen, (Crypto_AlgorithmFamilyType)ctx->hash_id))
        != 0)
    {
        return ret;
    }

    /* maskedSeed: Apply seedMask to seed */
    if ((ret = mgf_mask(output + 1, hlen, output + hlen + 1, olen - hlen - 1, (Crypto_AlgorithmFamilyType)ctx->hash_id))
        != 0)
    {
        return ret;
    }

    return Crypto_rsa_public(ctx, output, output);
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v1.5 encryption operation
 *                     (RSAES-PKCS1-v1_5-ENCRYPT).
 *
 * Param-Name[in]      ctx: The initialized RSA context to use.
 *                    f_rng: The RNG function to use. It is mandatory and used for
 *                            padding generation.
 *                     p_rng: The RNG context to be passed to f_rng. This may
 *                            be NULL if f_rng doesn't need a context argument.
 *                     ilen: The length of the plaintext in Bytes.
 *                     input: The input data to encrypt. This must be a readable
 *                            buffer of size ilen Bytes. It may be NULL if
 *                            `ilen == 0`.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The output buffer. This must be a writable buffer
 *                             of length ctx->len Bytes. For example, 256 Bytes
 *                             for an 2048-bit RSA modulus.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsaes_pkcs1_v15_encrypt(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    uint32               ilen,
    const unsigned char* input,
    unsigned char*       output)
{
    uint32         nb_pad, olen;
    int            ret = E_NOT_OK;
    unsigned char* p   = output;

    olen = ctx->len;

    /* first comparison checks for overflow */
    if (ilen + 11 < ilen || olen < ilen + 11)
    {
        return E_NOT_OK;
    }

    nb_pad = olen - 3 - ilen;

    *p++ = 0;

    if (f_rng == NULL_PTR)
    {
        return E_NOT_OK;
    }

    *p++ = CRYPTO_RSA_CRYPT;

    while (nb_pad-- > 0)
    {
        int rng_dl = 100;

        do
        {
            ret = f_rng(p_rng, p, 1);
        } while (*p == 0 && --rng_dl && ret == 0);

        /* Check if RNG failed to generate data */
        if (rng_dl == 0 || ret != 0)
        {
            return E_NOT_OK;
        }

        p++;
    }

    *p++ = 0;
    if (ilen != 0)
    {
        (void)IStdLib_MemCpy(p, input, ilen);
    }

    return Crypto_rsa_public(ctx, output, output);
}
#endif /* CRYPTO_PKCS1_V15 */

/******************************************************************************/
/*
 * Brief               This function adds the message padding, then performs an
 *                     RSA operation.
 * Param-Name[in]      ctx: The initialized RSA context to use.
 *                     f_rng: The RNG to use. It is used for padding generation
 *                            and it is mandatory.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                     ilen: The length of the plaintext in Bytes.
 *                     input: The input data to encrypt. This must be a readable
 *                            buffer of size ilen Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The output buffer.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_pkcs1_encrypt(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    uint32               ilen,
    const unsigned char* input,
    unsigned char*       output)
{
    Std_ReturnType          ret = E_NOT_OK;
    uint32                  Plaint_Len;
    const Crypto_Md_Info_t* md_info;
    uint32                  hlen;
    md_info = Crypto_Md_info_from_type(ctx->hash_id);

    hlen = Crypto_md_get_size(md_info);
    switch (ctx->padding)
    {
    case CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5:
        Plaint_Len = ctx->len - 11U;
        if (ilen <= Plaint_Len)
        {
            ret = Crypto_rsa_rsaes_pkcs1_v15_encrypt(ctx, f_rng, p_rng, ilen, input, output);
        }
        else
        {
            while (ilen > Plaint_Len)
            {
                ret = Crypto_rsa_rsaes_pkcs1_v15_encrypt(ctx, f_rng, p_rng, Plaint_Len, input, output);
                ilen -= Plaint_Len;
                input += Plaint_Len;
                output += ctx->len;
            }
        }
        break;
    case CRYPTO_ALGOMODE_RSAES_OAEP:

        Plaint_Len = ctx->len - 2u * hlen - 2;
        if (ilen <= Plaint_Len)
        {
            ret = Crypto_rsa_rsaes_oaep_encrypt(ctx, f_rng, p_rng, NULL_PTR, 0u, Plaint_Len, input, output);
        }
        else
        {
            while (ilen > Plaint_Len)
            {
                ret = Crypto_rsa_rsaes_oaep_encrypt(ctx, f_rng, p_rng, NULL_PTR, 0u, Plaint_Len, input, output);

                ilen -= Plaint_Len;
                input += Plaint_Len;
                output += ctx->len;
            }
        }
        break;

    default:
        break;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v2.1 OAEP decryption
 *                     operation (RSAES-OAEP-DECRYPT).
 *
 * Param-Name[in]      ctx: The initialized RSA context to use.
 *                    f_rng: The RNG function to use. It is mandatory and used for
 *                            padding generation.
 *                     p_rng: The RNG context to be passed to f_rng. This may
 *                            be NULL if f_rng doesn't need a context argument.
 *                     label: The buffer holding the custom label to use.
 *                            This must be a readable buffer of length label_len
 *                            Bytes. It may be NULL if label_len is 0.
 *                     label_len: The length of the label in Bytes.
 *                     input: The ciphertext buffer. This must be a readable buffer
 *                            of length ctx->len Bytes. For example, 256 Bytes
 *                            for an 2048-bit RSA modulus.
 *                     output_max_len: The length in Bytes of the output buffer
 *                                     output.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer used to hold the plaintext. This must
 *                             be a writable buffer of length output_max_len Bytes.
 *                     olen: The address at which to store the length of
 *                           the plaintext. This must not be NULL.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsaes_oaep_decrypt(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    const unsigned char* label,
    uint32               label_len,
    uint32*              olen,
    const unsigned char* input,
    unsigned char*       output,
    uint32               output_max_len)
{
    int            ret = E_NOT_OK;
    uint32         ilen, i, pad_len;
    unsigned char* p;
    uint64         bad, in_padding;
    unsigned char  buf[CRYPTO_MPI_MAX_SIZE];
    unsigned char  lhash[CRYPTO_MD_MAX_SIZE];
    unsigned int   hlen;

    /*
     * Parameters sanity checks
     */
    if (ctx->padding != CRYPTO_ALGOMODE_RSAES_OAEP)
    {
        return E_NOT_OK;
    }

    ilen = ctx->len;

    if (ilen < 16 || ilen > sizeof(buf))
    {
        return E_NOT_OK;
    }

    hlen = Crypto_md_get_size_from_type((Crypto_AlgorithmFamilyType)ctx->hash_id);
    if (hlen == 0)
    {
        return E_NOT_OK;
    }

    // checking for integer underflow
    if (2 * hlen + 2 > ilen)
    {
        return E_NOT_OK;
    }

    /*
     * RSA operation
     */
    ret = Crypto_rsa_private(ctx, f_rng, p_rng, input, buf);

    if (ret != 0)
    {
        goto cleanup;
    }

    /*
     * Unmask data and generate lHash
     */
    /* seed: Apply seedMask to maskedSeed */
    if ((ret = mgf_mask(buf + 1, hlen, buf + hlen + 1, ilen - hlen - 1, (Crypto_AlgorithmFamilyType)ctx->hash_id)) != 0
        ||
        /* DB: Apply dbMask to maskedDB */
        (ret = mgf_mask(buf + hlen + 1, ilen - hlen - 1, buf + 1, hlen, (Crypto_AlgorithmFamilyType)ctx->hash_id)) != 0)
    {
        goto cleanup;
    }

    /* Generate lHash */
    ret = compute_hash((Crypto_AlgorithmFamilyType)ctx->hash_id, label, label_len, lhash);
    if (ret != 0)
    {
        goto cleanup;
    }

    /*
     * Check contents, in "constant-time"
     */
    p = buf;

    bad = Crypto_ct_bool(*p++); /* First byte must be 0 */

    p += hlen; /* Skip seed */

    /* Check lHash */
    bad = Crypto_ct_bool_or(bad, Crypto_ct_bool(Crypto_ct_memcmp(lhash, p, hlen)));
    p += hlen;

    /* Get zero-padding len, but always read till end of buffer
     * (minus one, for the 01 byte) */
    pad_len    = 0;
    in_padding = 0xffffffffffffffffULL;
    for (i = 0; i < ilen - 2 * hlen - 2; i++)
    {
        in_padding = Crypto_ct_bool_and(in_padding, Crypto_ct_uint_eq(p[i], 0));
        pad_len += Crypto_ct_uint_if_else_0(in_padding, 1);
    }

    p += pad_len;
    bad = Crypto_ct_bool_or(bad, Crypto_ct_uint_ne(*p++, 0x01));

    /*
     * The only information "leaked" is whether the padding was correct or not
     * (eg, no data is copied if it was not correct). This meets the
     * recommendations in PKCS#1 v2.2: an opponent cannot distinguish between
     * the different error conditions.
     */
    if (bad != 0u)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    if (ilen - ((uint32)(p - buf)) > output_max_len)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    *olen = ilen - ((uint32)(p - buf));
    if (*olen != 0)
    {
        (void)IStdLib_MemCpy(output, p, *olen);
    }
    ret = 0;

cleanup:

    (void)IStdLib_MemSet(buf, 0, sizeof(buf));
    (void)IStdLib_MemSet(lhash, 0, sizeof(lhash));

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v1.5 decryption
 *                     operation (RSAES-PKCS1-v1_5-DECRYPT).
 *
 * Param-Name[in]      ctx: The initialized RSA context to use.
 *                    f_rng: The RNG function to use. It is mandatory and used for
 *                            padding generation.
 *                     p_rng: The RNG context to be passed to f_rng. This may
 *                            be NULL if f_rng doesn't need a context argument.
 *                     input: The ciphertext buffer. This must be a readable buffer
 *                            of length ctx->len Bytes. For example, 256 Bytes
 *                            for an 2048-bit RSA modulus.
 *                     output_max_len: The length in Bytes of the output buffer
 *                                     output.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer used to hold the plaintext. This must
 *                             be a writable buffer of length output_max_len Bytes.
 *                     olen: The address at which to store the length of
 *                           the plaintext. This must not be NULL.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsaes_pkcs1_v15_decrypt(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    uint32*              olen,
    const unsigned char* input,
    unsigned char*       output,
    uint32               output_max_len)
{
    int           ret = E_NOT_OK;
    uint32        ilen;
    unsigned char buf[CRYPTO_MPI_MAX_SIZE];

    ilen = ctx->len;

    if (ctx->padding != CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5)
    {
        return E_NOT_OK;
    }

    if (ilen < 16 || ilen > sizeof(buf))
    {
        return E_NOT_OK;
    }

    ret = Crypto_rsa_private(ctx, f_rng, p_rng, input, buf);

    if (ret != 0)
    {
        goto cleanup;
    }

    ret = Crypto_ct_rsaes_pkcs1_v15_unpadding(buf, ilen, output, output_max_len, olen);

cleanup:

    (void)IStdLib_MemSet(buf, 0, sizeof(buf));

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function performs an RSA operation, then removes the
 *                     message padding. It is the generic wrapper for performing
 *                     a PKCS#1 decryption operation.
 *
 * Param-Name[in]      cxt: The initialized RSA context to use.
 *                     f_rng: The RNG function.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                     olen: The address at which to store the length of the
 *                           plaintext.
 *                     input: The ciphertext buffer.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer used to hold the plaintext.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_pkcs1_decrypt(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    uint32*              olen,
    const unsigned char* input,
    uint32               ilen,
    unsigned char*       output,
    uint32               output_max_len)
{
    Std_ReturnType          ret = E_NOT_OK;
    const Crypto_Md_Info_t* md_info;
    uint32                  hlen;
    uint32                  mLen = ctx->len;
    *olen                        = 0u;

    md_info = Crypto_Md_info_from_type(ctx->hash_id);
    hlen    = Crypto_md_get_size(md_info);

    switch (ctx->padding)
    {
    case CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5:

        while (ilen >= mLen)
        {
            ret = Crypto_rsa_rsaes_pkcs1_v15_decrypt(ctx, f_rng, p_rng, olen, input, output, output_max_len);
            ilen -= mLen;
            input += mLen;
            output += mLen - 11u;
            *olen += mLen - 11u;
        }

        break;
    case CRYPTO_ALGOMODE_RSAES_OAEP:

        while (ilen >= mLen)
        {
            ret = Crypto_rsa_rsaes_oaep_decrypt(ctx, f_rng, p_rng, NULL_PTR, 0u, olen, input, output, output_max_len);
            ilen -= mLen;
            input += mLen;
            output += mLen - 2u * hlen - 2;
            *olen += mLen - 2u * hlen - 2;
        }
        break;
    default:
        return E_NOT_OK;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v2.1 PSS signature operation
 *                     without mode check.
 * Param-Name[in]      cxt: The initialized RSA context to use.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 *                     hash: The buffer holding the message digest or raw data.
 *                     saltlen: The length of salt.
 * Param-Name[in/out]  None
 * Param-Name[out]     sig: The buffer to hold the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int rsa_rsassa_pss_sign_no_mode_check(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    int                        saltlen,
    unsigned char*             sig)
{
    uint32                     olen;
    unsigned char*             p    = sig;
    unsigned char*             salt = NULL_PTR;
    uint32                     slen, min_slen, hlen, offset = 0;
    int                        ret = E_NOT_OK;
    uint32                     msb;
    Crypto_AlgorithmFamilyType hash_id;

    if ((md_alg != CRYPTO_ALGOFAM_NOT_SET || hashlen != 0) && hash == NULL_PTR)
    {
        return E_NOT_OK;
    }

    if (f_rng == NULL_PTR)
    {
        return E_NOT_OK;
    }

    olen = ctx->len;

    if (md_alg != CRYPTO_ALGOFAM_NOT_SET)
    {
        /* Gather length of hash to sign */
        uint32 exp_hashlen = Crypto_md_get_size_from_type(md_alg);
        if (exp_hashlen == 0)
        {
            return E_NOT_OK;
        }

        if (hashlen != exp_hashlen)
        {
            return E_NOT_OK;
        }
    }

    hash_id = (Crypto_AlgorithmFamilyType)ctx->hash_id;
    if (hash_id == CRYPTO_ALGOFAM_NOT_SET)
    {
        hash_id = md_alg;
    }
    hlen = Crypto_md_get_size_from_type(hash_id);
    if (hlen == 0)
    {
        return E_NOT_OK;
    }

    if (saltlen == CRYPTO_RSA_SALT_LEN_ANY)
    {
        /* Calculate the largest possible salt length, up to the hash size.
         * Normally this is the hash length, which is the maximum salt length
         * according to FIPS 185-4 §5.5 (e) and common practice. If there is not
         * enough room, use the maximum salt length that fits. The constraint is
         * that the hash length plus the salt length plus 2 bytes must be at most
         * the key length. This complies with FIPS 186-4 §5.5 (e) and RFC 8017
         * (PKCS#1 v2.2) §9.1.1 step 3. */
        min_slen = hlen - 2;
        if (olen < hlen + min_slen + 2)
        {
            return E_NOT_OK;
        }
        else if (olen >= hlen + hlen + 2)
        {
            slen = hlen;
        }
        else
        {
            slen = olen - hlen - 2;
        }
    }
    else if ((saltlen < 0) || (saltlen + hlen + 2 > olen))
    {
        return E_NOT_OK;
    }
    else
    {
        slen = (uint32)saltlen;
    }

    (void)IStdLib_MemSet(sig, 0, olen);

    /* Note: EMSA-PSS encoding is over the length of N - 1 bits */
    msb = Crypto_mpi_bitlen(&ctx->N) - 1;
    p += olen - hlen - slen - 2;
    *p++ = 0x01;

    /* Generate salt of length slen in place in the encoded message */
    salt = p;
    if ((ret = f_rng(p_rng, salt, slen)) != 0)
    {
        return E_NOT_OK;
    }

    p += slen;

    /* Generate H = Hash( M' ) */
    ret = hash_mprime(hash, hashlen, salt, slen, p, hash_id);
    if (ret != 0)
    {
        return ret;
    }

    /* Compensate for boundary condition when applying mask */
    if (msb % 8 == 0)
    {
        offset = 1;
    }

    /* maskedDB: Apply dbMask to DB */
    ret = mgf_mask(sig + offset, olen - hlen - 1 - offset, p, hlen, hash_id);
    if (ret != 0)
    {
        return ret;
    }

    msb = Crypto_mpi_bitlen(&ctx->N) - 1;
    sig[0] &= 0xFF >> (olen * 8 - msb);

    p += hlen;
    *p++ = 0xBC;

    return Crypto_rsa_private(ctx, f_rng, p_rng, sig, sig);
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v2.1 PSS signature operation.
 *
 * Param-Name[in]      cxt: The initialized RSA context to use.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 *                     saltlen: The length of salt.
 * Param-Name[in/out]  None
 * Param-Name[out]     hash: The buffer holding the message digest or raw data.
 *                     sig: The buffer to hold the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int rsa_rsassa_pss_sign(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    int                        saltlen,
    unsigned char*             sig)
{
    if (ctx->padding != CRYPTO_ALGOMODE_RSASSA_PSS)
    {
        return E_NOT_OK;
    }
    if ((ctx->hash_id == CRYPTO_ALGOFAM_NOT_SET) && (md_alg == CRYPTO_ALGOFAM_NOT_SET))
    {
        return E_NOT_OK;
    }
    return rsa_rsassa_pss_sign_no_mode_check(ctx, f_rng, p_rng, md_alg, hashlen, hash, saltlen, sig);
}

/******************************************************************************/
/*
 * Brief               Wrapper for rsa_rsassa_pss_sign_no_mode_check.
 *
 * Param-Name[in]      cxt: The initialized RSA context to use.
 *                     f_rng: The RNG function to use. It is mandatory and used for
 *                            padding generation.
 *                     p_rng: The RNG context to be passed to f_rng. This may
 *                            be NULL if f_rng doesn't need a context argument.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 *                     hash: The buffer holding the message digest or raw data.
 * Param-Name[in/out]  None
 * Param-Name[out]     sig: The buffer to hold the signature.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsassa_pss_sign_no_mode_check(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    unsigned char*             sig)
{
    return rsa_rsassa_pss_sign_no_mode_check(ctx, f_rng, p_rng, md_alg, hashlen, hash, CRYPTO_RSA_SALT_LEN_ANY, sig);
}

/******************************************************************************/
/*
 * Brief               Wrapper for rsa_rsassa_pss_sign.
 *
 * Param-Name[in]      cxt: The initialized RSA context to use.
 *                     f_rng: The RNG function to use. It is mandatory and used for
 *                            padding generation.
 *                     p_rng: The RNG context to be passed to f_rng. This may
 *                            be NULL if f_rng doesn't need a context argument.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 *                     hash: The buffer holding the message digest or raw data.
 *                     saltlen: The length of salt.
 * Param-Name[in/out]  None
 * Param-Name[out]     sig: The buffer to hold the signature.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsassa_pss_sign_ext(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    int                        saltlen,
    unsigned char*             sig)
{
    return rsa_rsassa_pss_sign(ctx, f_rng, p_rng, md_alg, hashlen, hash, saltlen, sig);
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v2.1 PSS signature operation.
 *
 * Param-Name[in]      cxt: The initialized RSA context to use.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     hash: The buffer holding the message digest or raw data.
 *                     sig: The buffer to hold the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsassa_pss_sign(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    unsigned char*             sig)
{
    return rsa_rsassa_pss_sign(ctx, f_rng, p_rng, md_alg, hashlen, hash, CRYPTO_RSA_SALT_LEN_ANY, sig);
}

/******************************************************************************/
/*
 * Brief               Construct a PKCS v1.5 encoding of a hashed message. This
 *                     is used both for signature generation and verification.
 *
 * Param-Name[in]      md_alg: Identifies the hash algorithm used to generate
 *                             the given hash; CRYPTO_ALGOFAM_NOT_SET if raw
 *                             data is signed.
 *                     hashlen: Length of hash. Must match md_alg if that's not NONE.
 *                     hash: Buffer containing the hashed message or the raw data.
 *                     dst_len: Length of the encoded message.
 * Param-Name[in/out]  None
 * Param-Name[out]     dst: Buffer to hold the encoded message.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int rsa_rsassa_pkcs1_v15_encode(
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    uint32                     dst_len,
    unsigned char*             dst)
{
    uint32         oid_size = 0;
    uint32         nb_pad   = dst_len;
    unsigned char* p        = dst;
    const char*    oid      = NULL_PTR;

    /* Are we signing hashed or raw data? */
    if (md_alg != CRYPTO_ALGOFAM_NOT_SET)
    {
        unsigned char md_size = Crypto_md_get_size_from_type(md_alg);
        if (md_size == 0)
        {
            return E_NOT_OK;
        }

        if (Crypto_oid_get_oid_by_md(md_alg, &oid, &oid_size) != 0)
        {
            return E_NOT_OK;
        }

        if (hashlen != md_size)
        {
            return E_NOT_OK;
        }

        /* Double-check that 8 + hashlen + oid_size can be used as a
         * 1-byte ASN.1 length encoding and that there's no overflow. */
        if (8 + hashlen + oid_size >= 0x80 || 10 + hashlen < hashlen || 10 + hashlen + oid_size < 10 + hashlen)
        {
            return E_NOT_OK;
        }

        /*
         * CRYPTO_62_LOCAL bounds check:
         * - Need 10 bytes for five tag-length pairs.
         *   (Insist on 1-byte length encodings to protect against variants of
         *    Bleichenbacher's forgery attack against lax PKCS#1v1.5 verification)
         * - Need hashlen bytes for hash
         * - Need oid_size bytes for hash alg OID.
         */
        if (nb_pad < 10 + hashlen + oid_size)
        {
            return E_NOT_OK;
        }
        nb_pad -= 10 + hashlen + oid_size;
    }
    else
    {
        if (nb_pad < hashlen)
        {
            return E_NOT_OK;
        }

        nb_pad -= hashlen;
    }

    /* Need space for signature header and padding delimiter (3 bytes),
     * and 8 bytes for the minimal padding */
    if (nb_pad < 3 + 8)
    {
        return E_NOT_OK;
    }
    nb_pad -= 3;

    /* Now nb_pad is the amount of memory to be filled
     * with padding, and at least 8 bytes long. */

    /* Write signature header and padding */
    *p++ = 0;
    *p++ = CRYPTO_RSA_SIGN;
    (void)IStdLib_MemSet(p, 0xFF, nb_pad);
    p += nb_pad;
    *p++ = 0;

    /* Are we signing raw data? */
    if (md_alg == CRYPTO_ALGOFAM_NOT_SET)
    {
        (void)IStdLib_MemCpy(p, hash, hashlen);
        return 0;
    }

    /* Signing hashed data, add corresponding ASN.1 structure
     *
     * DigestInfo ::= SEQUENCE {
     *   digestAlgorithm DigestAlgorithmIdentifier,
     *   digest Digest }
     * DigestAlgorithmIdentifier ::= AlgorithmIdentifier
     * Digest ::= OCTET STRING
     *
     * Schematic:
     * TAG-SEQ + LEN [ TAG-SEQ + LEN [ TAG-OID  + LEN [ OID  ]
     *                                 TAG-NULL_PTR + LEN [ NULL_PTR ] ]
     *                 TAG-OCTET + LEN [ HASH ] ]
     */
    *p++ = CRYPTO_ASN1_SEQUENCE | CRYPTO_ASN1_CONSTRUCTED;
    *p++ = (unsigned char)(0x08 + oid_size + hashlen);
    *p++ = CRYPTO_ASN1_SEQUENCE | CRYPTO_ASN1_CONSTRUCTED;
    *p++ = (unsigned char)(0x04 + oid_size);
    *p++ = CRYPTO_ASN1_OID;
    *p++ = (unsigned char)oid_size;
    (void)IStdLib_MemCpy(p, oid, oid_size);
    p += oid_size;
    *p++ = CRYPTO_ASN1_NULL;
    *p++ = 0x00;
    *p++ = CRYPTO_ASN1_OCTET_STRING;
    *p++ = (unsigned char)hashlen;
    (void)IStdLib_MemCpy(p, hash, hashlen);
    p += hashlen;

    /* Just a sanity-check, should be automatic
     * after the initial bounds check. */
    if (p != dst + dst_len)
    {
        (void)IStdLib_MemSet(dst, 0, sizeof(dst));
        return E_NOT_OK;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               Translate md_type into hash algorithm OID.
 *
 * Param-Name[in]      md_alg: Message digest algorithm.
 * Param-Name[in/out]  None
 * Param-Name[out]     oid: Place to store ASN.1 OID string pointer.
 *                     olen:  Length of the OID.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int Crypto_oid_get_oid_by_md(Crypto_AlgorithmFamilyType md_alg, const char** oid, uint32* olen)
{
    const Crypto_oid_md_alg_t* cur = (oid_md_alg);
    while (cur->descriptor.asn1 != ((void*)0))
    {
        if (cur->md_alg == (md_alg))
        {
            *oid  = cur->descriptor.asn1;
            *olen = cur->descriptor.asn1_len;
            return 0;
        }
        cur++;
    }
    return E_OK;
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v1.5 signature operation.
 *
 * Param-Name[in]      cxt: The initialized RSA context to use.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     hash: The buffer holding the message digest or raw data.
 *                     sig: The buffer to hold the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsassa_pkcs1_v15_sign(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    unsigned char*             sig)
{
    int            ret     = E_NOT_OK;
    unsigned char *sig_try = NULL_PTR, *verif = NULL_PTR;

    if ((md_alg != CRYPTO_ALGOFAM_NOT_SET || hashlen != 0) && hash == NULL_PTR)
    {
        return E_NOT_OK;
    }
    if (ctx->padding != CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5)
    {
        return E_NOT_OK;
    }

    /*
     * Prepare PKCS1-v1.5 encoding (padding and hash identifier)
     */

    if ((ret = rsa_rsassa_pkcs1_v15_encode(md_alg, hashlen, hash, ctx->len, sig)) != 0)
    {
        return ret;
    }

    /* Private key operation
     *
     * In order to prevent Lenstra's attack, make the signature in a
     * temporary buffer and check it before returning it.
     */

    sig_try = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, ctx->len);
    if (sig_try == NULL_PTR)
    {
        return E_NOT_OK;
    }

    verif = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, ctx->len);
    if (verif == NULL_PTR)
    {
        return E_NOT_OK;
    }

    CRYPTO_MPI_CHK(Crypto_rsa_private(ctx, f_rng, p_rng, sig, sig_try));
    CRYPTO_MPI_CHK(Crypto_rsa_public(ctx, sig_try, verif));

    if (Crypto_ct_memcmp(verif, sig, ctx->len) != 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    (void)IStdLib_MemCpy(sig, sig_try, ctx->len);

cleanup:
    (void)IStdLib_MemSet(sig_try, 0, ctx->len);
    (void)IStdLib_MemSet(verif, 0, ctx->len);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, sig_try);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, verif);

    if (ret != 0)
    {
        (void)IStdLib_MemSet(sig, '!', ctx->len);
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function performs a private RSA operation to sign a
 *                      message digest using PKCS#1.
 *
 * Param-Name[in]      cxt: The initialized RSA context to use.
 *                     f_rng: The RNG function to use.
 *                     p_rng: The RNG context to be passed to f_rng.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     hash: The buffer holding the message digest or raw data.
 *                     sig: The buffer to hold the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_pkcs1_sign(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                      p_rng,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    unsigned char*             sig)
{
    if ((md_alg != CRYPTO_ALGOFAM_NOT_SET || hashlen != 0) && hash == NULL_PTR)
    {
        return E_NOT_OK;
    }

    switch (ctx->padding)
    {
    case CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5:
        return Crypto_rsa_rsassa_pkcs1_v15_sign(ctx, f_rng, p_rng, md_alg, hashlen, hash, sig);

    case CRYPTO_ALGOMODE_RSASSA_PSS:
        return Crypto_rsa_rsassa_pss_sign(ctx, f_rng, p_rng, md_alg, hashlen, hash, sig);

    default:
        return E_NOT_OK;
    }
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v2.1 PSS verification operation.
 *
 * Param-Name[in]      ctx: The initialized RSA public key context to use.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 *                     mgf1_hash_id: The message digest algorithm used for the
 *                                   verification operation and the mask generation
 *                                   function.
 *                     expected_salt_len: Expected length of salt.
 * Param-Name[in/out]  None
 * Param-Name[out]     hash: The buffer holding the message digest or raw data.
 *                     sig: The buffer holding the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsassa_pss_verify_ext(
    Crypto_rsa_context*        ctx,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    Crypto_AlgorithmFamilyType mgf1_hash_id,
    int                        expected_salt_len,
    const unsigned char*       sig)
{
    int            ret = E_NOT_OK;
    uint32         siglen;
    unsigned char* p;
    unsigned char* hash_start;
    unsigned char  result[CRYPTO_MD_MAX_SIZE];
    unsigned int   hlen;
    uint32         observed_salt_len, msb;
    unsigned char  buf[CRYPTO_MPI_MAX_SIZE] = {0};

    if ((md_alg != CRYPTO_ALGOFAM_NOT_SET || hashlen != 0) && hash == NULL_PTR)
    {
        return E_NOT_OK;
    }

    siglen = ctx->len;

    if (siglen < 16 || siglen > sizeof(buf))
    {
        return E_NOT_OK;
    }

    ret = Crypto_rsa_public(ctx, sig, buf);

    if (ret != 0)
    {
        return ret;
    }

    p = buf;

    if (buf[siglen - 1] != 0xBC)
    {
        return E_NOT_OK;
    }

    if (md_alg != CRYPTO_ALGOFAM_NOT_SET)
    {
        /* Gather length of hash to sign */
        uint32 exp_hashlen = Crypto_md_get_size_from_type(md_alg);
        if (exp_hashlen == 0)
        {
            return E_NOT_OK;
        }

        if (hashlen != exp_hashlen)
        {
            return E_NOT_OK;
        }
    }

    hlen = Crypto_md_get_size_from_type(mgf1_hash_id);
    if (hlen == 0)
    {
        return E_NOT_OK;
    }

    /*
     * Note: EMSA-PSS verification is over the length of N - 1 bits
     */
    msb = Crypto_mpi_bitlen(&ctx->N) - 1;

    if (buf[0] >> (8 - siglen * 8 + msb))
    {
        return E_NOT_OK;
    }

    /* Compensate for boundary condition when applying mask */
    if (msb % 8 == 0)
    {
        p++;
        siglen -= 1;
    }

    if (siglen < hlen + 2)
    {
        return E_NOT_OK;
    }
    hash_start = p + siglen - hlen - 1;

    ret = mgf_mask(p, siglen - hlen - 1, hash_start, hlen, mgf1_hash_id);
    if (ret != 0)
    {
        return ret;
    }

    buf[0] &= 0xFF >> (siglen * 8 - msb);

    while (p < hash_start - 1 && *p == 0)
    {
        p++;
    }

    if (*p++ != 0x01)
    {
        return E_NOT_OK;
    }

    observed_salt_len = (uint32)(hash_start - p);

    if (expected_salt_len != CRYPTO_RSA_SALT_LEN_ANY && observed_salt_len != (uint32)expected_salt_len)
    {
        return E_NOT_OK;
    }

    /*
     * Generate H = Hash( M' )
     */
    ret = hash_mprime(hash, hashlen, p, observed_salt_len, result, mgf1_hash_id);
    if (ret != 0)
    {
        return ret;
    }

    if (IStdLib_MemCmp(hash_start, result, hlen) != 0)
    {
        return E_NOT_OK;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v2.1 PSS verification operation.
 *
 * Param-Name[in]      ctx: The initialized RSA public key context to use.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     hash: The buffer holding the message digest or raw data.
 *                     sig: The buffer holding the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsassa_pss_verify(
    Crypto_rsa_context*        ctx,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    const unsigned char*       sig)
{
    Crypto_AlgorithmFamilyType mgf1_hash_id;
    if ((md_alg != CRYPTO_ALGOFAM_NOT_SET || hashlen != 0) && hash == NULL_PTR)
    {
        return E_NOT_OK;
    }

    mgf1_hash_id = (ctx->hash_id != CRYPTO_ALGOFAM_NOT_SET) ? (Crypto_AlgorithmFamilyType)ctx->hash_id : md_alg;

    return Crypto_rsa_rsassa_pss_verify_ext(ctx, md_alg, hashlen, hash, mgf1_hash_id, CRYPTO_RSA_SALT_LEN_ANY, sig);
}

/******************************************************************************/
/*
 * Brief               This function performs a PKCS#1 v1.5 verification operation.
 *
 * Param-Name[in]      ctx: The initialized RSA public key context to use.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     hash: The buffer holding the message digest or raw data.
 *                     sig: The buffer holding the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_rsassa_pkcs1_v15_verify(
    Crypto_rsa_context*        ctx,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    const unsigned char*       sig)
{
    int ret = 0;
    // uint8 data[128];
    uint32         sig_len;
    unsigned char *encoded = NULL_PTR, *encoded_expected = NULL_PTR;

    if ((md_alg != CRYPTO_ALGOFAM_NOT_SET || hashlen != 0) && hash == NULL_PTR)
    {
        return E_NOT_OK;
    }

    sig_len = ctx->len;

    /*
     * Prepare expected PKCS1 v1.5 encoding of hash.
     */

    if ((encoded = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, sig_len)) == NULL_PTR
        || (encoded_expected = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, sig_len)) == NULL_PTR)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

    if ((ret = rsa_rsassa_pkcs1_v15_encode(md_alg, hashlen, hash, sig_len, encoded_expected)) != 0)
    {
        goto cleanup;
    }

    for (uint32 i = 0; i < ctx->len; i++)
    {
        // data[i] = hash[i];
    }
    /*
     * Apply RSA primitive to get what should be PKCS1 encoded hash.
     */

    ret = Crypto_rsa_public(ctx, sig, encoded);
    if (ret != 0)
    {
        goto cleanup;
    }

    /*
     * Compare
     */

    if ((ret = Crypto_ct_memcmp(encoded, encoded_expected, sig_len)) != 0)
    {
        ret = E_NOT_OK;
        goto cleanup;
    }

cleanup:

    if (encoded != NULL_PTR)
    {
        (void)IStdLib_MemSet(encoded, 0, sig_len);
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, encoded);
    }

    if (encoded_expected != NULL_PTR)
    {
        (void)IStdLib_MemSet(encoded_expected, 0, sig_len);
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, encoded_expected);
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function performs a public RSA operation and checks
 *                      the message digest.
 *
 * Param-Name[in]      ctx: The initialized RSA public key context to use.
 *                     md_alg: The message-digest algorithm used to hash the
 *                             original data.
 *                     hashlen: The length of the message digest or raw data in Bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     hash: The buffer holding the message digest or raw data.
 *                     sig: The buffer holding the signature.
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_pkcs1_verify(
    Crypto_rsa_context*        ctx,
    Crypto_AlgorithmFamilyType md_alg,
    unsigned int               hashlen,
    const unsigned char*       hash,
    const unsigned char*       sig)
{
    if ((md_alg != CRYPTO_ALGOFAM_NOT_SET || hashlen != 0) && hash == NULL_PTR)
    {
        return E_NOT_OK;
    }

    switch (ctx->padding)
    {
    case CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5:
        return Crypto_rsa_rsassa_pkcs1_v15_verify(ctx, md_alg, hashlen, hash, sig);

    case CRYPTO_ALGOMODE_RSASSA_PSS:
        return Crypto_rsa_rsassa_pss_verify(ctx, md_alg, hashlen, hash, sig);

    default:
        return E_NOT_OK;
    }
}

/******************************************************************************/
/*
 * Brief               This function copies the components of an RSA context.
 *
 * Param-Name[in]      src: The source context. This must be initialized.
 * Param-Name[in/out]  None
 * Param-Name[out]     dst: The destination context. This must be initialized.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
int Crypto_rsa_copy(Crypto_rsa_context* dst, const Crypto_rsa_context* src)
{
    int ret = E_NOT_OK;

    dst->len = src->len;

    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->N, &src->N));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->E, &src->E));

    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->D, &src->D));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->P, &src->P));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->Q, &src->Q));

#if !defined(CRYPTO_RSA_NO_CRT)
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->DP, &src->DP));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->DQ, &src->DQ));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->QP, &src->QP));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->RP, &src->RP));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->RQ, &src->RQ));
#endif

    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->RN, &src->RN));

    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->Vi, &src->Vi));
    CRYPTO_MPI_CHK(Crypto_mpi_copy(&dst->Vf, &src->Vf));

    dst->padding = src->padding;
    dst->hash_id = src->hash_id;

cleanup:

    return ret;
}

/*
 * Example RSA-1024 keypair, for test purposes
 */

#define RSA_N                          \
    "9292758453063D803DD603D5E777D788" \
    "8ED1D5BF35786190FA2F23EBC0848AEA" \
    "DDA92CA6C3D80B32C4D109BE0F36D6AE" \
    "7130B9CED7ACDF54CFC7555AC14EEBAB" \
    "93A89813FBF3C4F8066D2D800F7C38A8" \
    "1AE31942917403FF4946B0A83D3D3E05" \
    "EE57C6F5F5606FB5D4BC6CD34EE0801A" \
    "5E94BB77B07507233A0BC7BAC8F90F79"

#define RSA_E "10001"

#define RSA_D                          \
    "24BF6185468786FDD303083D25E64EFC" \
    "66CA472BC44D253102F8B4A9D3BFA750" \
    "91386C0077937FE33FA3252D28855837" \
    "AE1B484A8A9A45F7EE8C0C634F99E8CD" \
    "DF79C5CE07EE72C7F123142198164234" \
    "CABB724CF78B8173B9F880FC86322407" \
    "AF1FEDFDDE2BEB674CA15F3E81A1521E" \
    "071513A1E85B5DFA031F21ECAE91A34D"

#define RSA_P                          \
    "C36D0EB7FCD285223CFB5AABA5BDA3D8" \
    "2C01CAD19EA484A87EA4377637E75500" \
    "FCB2005C5C7DD6EC4AC023CDA285D796" \
    "C3D9E75E1EFC42488BB4F1D13AC30A57"

#define RSA_Q                          \
    "C000DF51A7C77AE8D7C7370C1FF55B69" \
    "E211C2B9E5DB1ED0BF61D0D9899620F4" \
    "910E4168387E3C30AA1E00C339A79508" \
    "8452DD96A9A5EA5D9DCA68DA636032AF"

#define PT_LEN 24
#define RSA_PT                                         \
    "\xAA\xBB\xCC\x03\x02\x01\x00\xFF\xFF\xFF\xFF\xFF" \
    "\x11\x22\x33\x0A\x0B\x0C\xCC\xDD\xDD\xDD\xDD\xDD"

/******************************************************************************/
/*
 * Brief               This function generates random bits. Always be the parameter
 *                     f_rng.
 *
 * Param-Name[in]      len: Expected length of random bits.
 * Param-Name[in/out]  rng_state: The state of RNG.
 * Param-Name[out]     output: The buffer to store random bits.
 * Return              Std_ReturnType
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType myrand(void* rng_state, unsigned char* output, uint32 len)
{
    uint32 i;

    if (rng_state != NULL_PTR)
    {
        rng_state = NULL_PTR;
    }

    for (i = 0; i < len; ++i)
    {
        output[i] = rand();
    }

    return E_OK;
}
/******************************************************************************/
/*
 * Brief               This function do the RSA Encrypt process and output cipher text.
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
Std_ReturnType
    Crypto_Rsa_EncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType     ret = E_NOT_OK;
    Crypto_rsa_context rsa;

    uint8* rsa_plaintext  = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint8* rsa_ciphertext = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr);
    uint32 inLen          = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32 keylength      = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint8* key;
    key = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, keylength);

    Crypto_rsa_init(&rsa, mode, secondaryFam);
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &keylength);
    if (ret == E_OK)
    {
        ret     = Crypto_rsa_parse_pubkey(&rsa, key, keylength);
        rsa.len = Crypto_mpi_size(&rsa.N);
    }

    if (ret == E_OK)
    {
        Crypto_rsa_pkcs1_encrypt(&rsa, myrand, NULL_PTR, inLen, rsa_plaintext, rsa_ciphertext);
    }
    (void)IStdLib_MemSet(key, 0, keylength);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    Crypto_mpi_free(&rsa.Vi);
    Crypto_mpi_free(&rsa.Vf);
    Crypto_mpi_free(&rsa.RN);
    Crypto_mpi_free(&rsa.D);
    Crypto_mpi_free(&rsa.Q);
    Crypto_mpi_free(&rsa.P);
    Crypto_mpi_free(&rsa.E);
    Crypto_mpi_free(&rsa.N);
#if !defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_free(&rsa.RQ);
    Crypto_mpi_free(&rsa.RP);
    Crypto_mpi_free(&rsa.QP);
    Crypto_mpi_free(&rsa.DQ);
    Crypto_mpi_free(&rsa.DP);
#endif /* CRYPTO_RSA_NO_CRT */
    return ret;
}
/******************************************************************************/
/*
 * Brief               This function do the RSA DEcrypt process and output cipher text.
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
Std_ReturnType
    Crypto_Rsa_DecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType     ret = E_NOT_OK;
    Crypto_rsa_context rsa;
    uint32             keylength = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint8*             key;
    key                    = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, keylength);
    uint32* outLen         = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint8*  rsa_ciphertext = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint8*  rsa_plaintext  = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr);
    uint32  inLen          = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;

    Crypto_rsa_init(&rsa, mode, secondaryFam);
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &keylength);
    if (ret == E_OK)
    {
        ret     = Crypto_rsa_parse_key(&rsa, key, keylength);
        rsa.len = Crypto_mpi_size(&rsa.N);
    }
    if (ret == E_OK)
    {
        ret = Crypto_rsa_pkcs1_decrypt(&rsa, myrand, NULL_PTR, outLen, rsa_ciphertext, inLen, rsa_plaintext, keylength);
    }
    (void)IStdLib_MemSet(key, 0, keylength);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    Crypto_mpi_free(&rsa.Vi);
    Crypto_mpi_free(&rsa.Vf);
    Crypto_mpi_free(&rsa.RN);
    Crypto_mpi_free(&rsa.D);
    Crypto_mpi_free(&rsa.Q);
    Crypto_mpi_free(&rsa.P);
    Crypto_mpi_free(&rsa.E);
    Crypto_mpi_free(&rsa.N);
#if !defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_free(&rsa.RQ);
    Crypto_mpi_free(&rsa.RP);
    Crypto_mpi_free(&rsa.QP);
    Crypto_mpi_free(&rsa.DQ);
    Crypto_mpi_free(&rsa.DP);
#endif /* CRYPTO_RSA_NO_CRT */
    return ret;
}
/******************************************************************************/
/*
 * Brief               This function do the RSA signature generate process and output signature text.
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
Std_ReturnType Crypto_Rsa_SignatureGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret = E_NOT_OK;

    Crypto_rsa_context rsa;

    Crypto_Md_Context_t     ctx;
    const Crypto_Md_Info_t* md_info;

    const uint8* input    = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint32       inputlen = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;

    uint32 keylength      = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint8* key            = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, keylength);
    uint8* output         = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, keylength);
    uint8* rsa_ciphertext = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, keylength);
    uint32 outputlen      = *Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Md_Context_t));
    Crypto_rsa_init(&rsa, mode, secondaryFam);
    md_info = Crypto_Md_info_from_type(secondaryFam);
    ret     = Crypto_md_setup(&ctx, md_info);
    if (ret == E_OK)
    {
        ret = Crypto_Md_Start(&ctx);
    }
    if (ret == E_OK)
    {
        ret = Crypto_Md_Update(&ctx, input, inputlen);
    }
    if (ret == E_OK)
    {
        ret = Crypto_Md_Finish(&ctx, output);
        (void)IStdLib_MemSet(ctx.hmac_ctx, 0, 2 * (ctx.md_info->size));
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, ctx.hmac_ctx);
        Crypto_Md_Free(&ctx);
    }
    if (ret == E_OK)
    {
        ret = Crypto_62_KeyElementGet(
            Crypto_62_StoredJob[objectId].cryptoKeyId,
            CRYPTO_KE_SIGNATURE_KEY,
            key,
            &keylength);

        /*need to analyze private key*/
        if (ret == E_OK)
        {
            ret = Crypto_rsa_parse_key(&rsa, key, keylength);
        }
        if (ret == E_OK)
        {
            ret =
                Crypto_rsa_pkcs1_sign(&rsa, myrand, NULL_PTR, secondaryFam, ctx.md_info->size, output, rsa_ciphertext);
        }
    }
    (void)IStdLib_MemCpy(
        (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr),
        rsa_ciphertext,
        outputlen);

    (void)IStdLib_MemSet(key, 0, keylength);
    (void)IStdLib_MemSet(output, 0, keylength);
    (void)IStdLib_MemSet(rsa_ciphertext, 0, keylength);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, output);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, rsa_ciphertext);

    Crypto_mpi_free(&rsa.Vi);
    Crypto_mpi_free(&rsa.Vf);
    Crypto_mpi_free(&rsa.RN);
    Crypto_mpi_free(&rsa.D);
    Crypto_mpi_free(&rsa.Q);
    Crypto_mpi_free(&rsa.P);
    Crypto_mpi_free(&rsa.E);
    Crypto_mpi_free(&rsa.N);
#if !defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_free(&rsa.RQ);
    Crypto_mpi_free(&rsa.RP);
    Crypto_mpi_free(&rsa.QP);
    Crypto_mpi_free(&rsa.DQ);
    Crypto_mpi_free(&rsa.DP);
#endif /* CRYPTO_RSA_NO_CRT */
    return ret;
}
/******************************************************************************/
/*
 * Brief               This function do the RSA signature verify process and output result.
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
Std_ReturnType Crypto_Rsa_SignatureVerifyProcess(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret = E_NOT_OK;

    uint32              keylength = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    Crypto_Md_Context_t ctx;
    Crypto_rsa_context  rsa;
    const Crypto_Md_Info_t* md_info;
    uint8*       rsa_Signature = (uint8*)Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.secondaryInputPtr;
    const uint8* input         = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint32       inLen         = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;

    uint8* key  = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, keylength);
    md_info     = Crypto_Md_info_from_type(secondaryFam);
    ret         = Crypto_md_setup(&ctx, md_info);
    uint8* hash = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, ctx.md_info->size);
    if (ret == E_OK)
    {
        ret = Crypto_Md_Start(&ctx);
    }
    if (ret == E_OK)
    {
        ret = Crypto_Md_Update(&ctx, input, inLen);
    }
    if (ret == E_OK)
    {
        ret = Crypto_Md_Finish(&ctx, hash);
        (void)IStdLib_MemSet(ctx.hmac_ctx, 0, 2 * (ctx.md_info->size));
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, ctx.hmac_ctx);
        Crypto_Md_Free(&ctx);
    }

    Crypto_rsa_init(&rsa, mode, secondaryFam);

    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_SIGNATURE_KEY, key, &keylength);
    if (ret == E_OK)
    {
        ret = Crypto_rsa_parse_pubkey(&rsa, key, keylength);
    }
    if (ret == E_OK)
    {
        ret = Crypto_rsa_pkcs1_verify(&rsa, secondaryFam, ctx.md_info->size, hash, rsa_Signature);
    }

    if (E_OK == ret)
    {
        *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_OK;
    }
    else
    {
        *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_NOT_OK;
    }
    (void)IStdLib_MemSet(key, 0, keylength);
    (void)IStdLib_MemSet(hash, 0, ctx.md_info->size);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, hash);

    Crypto_mpi_free(&rsa.Vi);
    Crypto_mpi_free(&rsa.Vf);
    Crypto_mpi_free(&rsa.RN);
    Crypto_mpi_free(&rsa.D);
    Crypto_mpi_free(&rsa.Q);
    Crypto_mpi_free(&rsa.P);
    Crypto_mpi_free(&rsa.E);
    Crypto_mpi_free(&rsa.N);
#if !defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_free(&rsa.RQ);
    Crypto_mpi_free(&rsa.RP);
    Crypto_mpi_free(&rsa.QP);
    Crypto_mpi_free(&rsa.DQ);
    Crypto_mpi_free(&rsa.DP);
#endif /* CRYPTO_RSA_NO_CRT */
    return ret;
}

#endif
/* PRQA S 0311,0314,0316,0488,1339,2016,2023,2024,2052,2105,3102,3103,3208,3326,3345 -- */
/* PRQA S 1253,0288,0317,0432,0489,0490,0693,0770,0772,0780,0842,1252,1258,1259,1277 -- */
/* PRQA S 1290,1291,1336,1338,1840,1820,1821,1823,1841,1842,1843,1844,1851,1853,1860 -- */
/* PRQA S 1861,1863,1880,1881,1890,1891,2001,2015,2205,2106,2140,2462,2463,2472,2487 -- */
/* PRQA S 2740,3101,3120,3122,3123,3140,3200,3206,3209,3218,3219,3226,3332,3344,3387 -- */
/* PRQA S 3395,3396,3397,3400,3408,3410,3418,3450,3440,3455,3610,3625,3672,3678,3715 -- */
/* PRQA S 3717,3762,3769,3772,3779,4115,4116,4393,4394,4403,4404,4413,4414,4423,4432 -- */
/* PRQA S 4434,4443,4446,4447,4460,4461,4464,4470,4480,4491,4499,4501,4538,4542,4544 -- */
/* PRQA S 4532,4533,4543,4558,3673,3795,4150,4522,3391,3432,3442,2743,1505,2889,2834 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,2996,2992,2911,1503,2822,2896,2986,2755 -- */
/* PRQA S 2834,2982,2983,2955,1532,2986,2982,2983,5022,5143,5014,5031,5045,2839,2998 -- */
/* PRQA S 2995,2997,2871,2990,4825 -- */
