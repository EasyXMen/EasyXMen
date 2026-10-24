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
**  @file               : Crypto_62_Rsa.c
**  @author             : Jie.gU
**  @date               : 2024/01/01
**  @vendor             : isoft
**  @description        : Rebuid for Crypto
**
***********************************************************************************************************************/

/* PRQA S 0311,0314,0316,0488,1339,2016,2023,2024,2052,2105,3102,3103,3208,3326,3345 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1253,0288,0317,0489,0490,0693,0770,0772,0780,0842,1252,1258,1259,1277,4825 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1290,1291,1336,1338,1840,1820,1821,1823,1841,1842,1843,1844,1851,1853,1860 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1861,1863,1880,1881,1890,1891,2001,2015,2205,2106,2140,2462,2463,2472,2487 ++ */ /* VL_QAC_Crypto */
/* PRQA S 2740,3101,3120,3122,3123,3140,3200,3206,3209,3218,3219,3226,3332,3344,3387 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3395,3396,3397,3400,3408,3410,3418,3450,3440,3455,3610,3625,3672,3678,3715 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3717,3762,3769,3772,3779,4115,4116,4393,4394,4403,4404,4413,4414,4423,4432 ++ */ /* VL_QAC_Crypto */
/* PRQA S 4434,4443,4446,4447,4460,4461,4464,4470,4480,4491,4499,4501,4538,4542,4544 ++ */ /* VL_QAC_Crypto */
/* PRQA S 4532,4533,4543,4558,3673,3795,4150,4522,3391,3432,3442,2743,1505,2889,2834 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,2996,2992,2911,1503,2822,2896,2986,2990 ++ */ /* VL_QAC_Crypto */
/* PRQA S 2834,2982,2983,2955,1532,2986,2982,2983,5022,5143,5014,5031,5045,2839,2998 ++ */ /* VL_QAC_Crypto */
/* PRQA S 2995,2997,2871 ++ */                                                             /* VL_QAC_Crypto */

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
#define CRYPTO_62_START_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_62_MemMap.h"

/* ========================================== internal function declarations ======================================== */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
CRYPTO_62_LOCAL Std_ReturnType myrand(void* rng_state, unsigned char* output, uint32 len);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
CRYPTO_62_LOCAL int rsa_check_context(const Crypto_rsa_context* ctx, int is_priv, int blinding_needed);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
CRYPTO_62_LOCAL int Crypto_oid_get_oid_by_md(Crypto_AlgorithmFamilyType md_alg, const char** oid, uint32* olen);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#if !defined(CRYPTO_RSA_NO_CRT)
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_validate_crt(
    const Crypto_mpi* P,
    const Crypto_mpi* Q,
    const Crypto_mpi* D,
    const Crypto_mpi* DP,
    const Crypto_mpi* DQ,
    const Crypto_mpi* QP);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_ct_memcmp(const void* a, const void* b, uint32 n)
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function frees the components of an RSA key.
 *
 * Param-Name[in]      ctx: The RSA context to free.
 * Param-Name[out]     None
 * Return              int:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_free(Crypto_rsa_context* ctx)
{
    if (ctx == NULL)
    {
        return E_OK;
    }

    Crypto_mpi_free(&ctx->Vi);

    Crypto_mpi_free(&ctx->Vf);

    Crypto_mpi_free(&ctx->RN);

    Crypto_mpi_free(&ctx->D);

    Crypto_mpi_free(&ctx->Q);

    Crypto_mpi_free(&ctx->P);

    Crypto_mpi_free(&ctx->E);

    Crypto_mpi_free(&ctx->N);

#if !defined(CRYPTO_RSA_NO_CRT)
    Crypto_mpi_free(&ctx->RQ);
    Crypto_mpi_free(&ctx->RP);
    Crypto_mpi_free(&ctx->QP);
    Crypto_mpi_free(&ctx->DQ);
    Crypto_mpi_free(&ctx->DP);
#endif /* CRYPTO_RSA_NO_CRT */
    return E_OK;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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

#if 0
    if (p != end)
    {
        ret = E_NOT_OK;
    }
#endif

cleanup:
    Crypto_mpi_free(&T);

    if (ret != 0)
    {
        Crypto_rsa_free(rsa);
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#if 0
    if (p != end)
    {
        return E_NOT_OK;
    }
#endif
    return E_OK;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#if !defined(CRYPTO_RSA_NO_CRT)
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
    plaintext_size = Crypto_ct_uint_if(bad, (uint32)plaintext_max_size, (uint32)(ilen - pad_count - 3));

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
    plaintext_size = Crypto_ct_uint_if(output_too_large, (uint32)plaintext_max_size, (uint32)plaintext_size);

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
        (void)IStdLib_MemCpy(output, input + ilen - plaintext_max_size, plaintext_size);
    }

    /* Report the amount of data we copied to the output buffer. In case
     * of errors (bad padding or output too large), the value of *olen
     * when this function returns is not specified. Making it equivalent
     * to the good case limits the risks of leaking the padding validity. */
    *olen = plaintext_size;

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function initializes an RSA context.
 *
 * Param-Name[in]      mode: Indicate padding mode. It can be CRYPTO_RSA_PKCS_V15
 *                           for 1.5 padding and CRYPTO_RSA_PKCS_V21 for OAEP or
 *                           PSS.
 *                     secondaryFam: Hash identifier of Crypto_AlgorithmFamilyType type.
 *
 * Param-Name[in/out]  ctx: The context of RSA.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
void Crypto_rsa_init(Crypto_rsa_context* ctx, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    (void)IStdLib_MemSet(ctx, 0, sizeof(Crypto_rsa_context));

    ctx->padding = mode;
    ctx->hash_id = secondaryFam;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_set_padding(Crypto_rsa_context* ctx, int padding, Crypto_AlgorithmFamilyType hash_id)
{
    ctx->padding = padding;
    ctx->hash_id = hash_id;

    return 0;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_get_padding_mode(const Crypto_rsa_context* ctx)
{
    return ctx->padding;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_get_md_alg(const Crypto_rsa_context* ctx)
{
    return ctx->hash_id;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
uint32 Crypto_rsa_get_bitlen(const Crypto_rsa_context* ctx)
{
    return Crypto_mpi_bitlen(&ctx->N);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
uint32 Crypto_rsa_get_len(const Crypto_rsa_context* ctx)
{
    return ctx->len;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
CRYPTO_62_LOCAL int rsa_unblind(Crypto_mpi* T, Crypto_mpi* Vf, const Crypto_mpi* N)
{
    int                   ret    = E_NOT_OK;
    const Crypto_mpi_uint mm     = Crypto_mpi_core_montmul_init(N->p);
    const uint32          nlimbs = N->n;
    const uint32          tlimbs = Crypto_mpi_core_montmul_working_limbs(nlimbs);
    Crypto_mpi            RR, M_T;

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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
    if ((ret = Crypto_md_setup(&md_ctx, md_info, 0)) != 0)
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
    Crypto_md_free(&md_ctx);
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
    if ((ret = Crypto_md_setup(&md_ctx, md_info, 0)) != 0)
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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

    if (label_len > 0)
    {
        /* Construct DB */
        ret = compute_hash((Crypto_AlgorithmFamilyType)ctx->hash_id, label, label_len, p);
        if (ret != 0)
        {
            return ret;
        }
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_pkcs1_encrypt(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    uint32               ilen,
    const unsigned char* input,
    unsigned char*       output)
{
    Std_ReturnType          ret = E_NOT_OK;
    const Crypto_Md_Info_t* md_info;
    uint32                  hlen;
    md_info = Crypto_Md_info_from_type(ctx->hash_id);

    hlen = Crypto_md_get_size(md_info);
    switch (ctx->padding)
    {
    case CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5:
        if (ilen <= ctx->len - 11U)
        {
            ret = Crypto_rsa_rsaes_pkcs1_v15_encrypt(ctx, f_rng, p_rng, ilen, input, output);
        }
        else
        {
            ret = E_NOT_OK; // input message too long
        }
        break;
    case CRYPTO_ALGOMODE_RSAES_OAEP:
        if (ilen <= ctx->len - 2u * hlen - 2)
        {
            ret = Crypto_rsa_rsaes_oaep_encrypt(ctx, f_rng, p_rng, NULL_PTR, 0u, ilen, input, output);
        }
        else
        {
            ret = E_NOT_OK; // input message too long
        }
        break;

    default:
        break;
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
    unsigned char  lhash[CRYPTO_MD_MAX_SIZE] = {0x00};
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

    if (label_len > 0)
    {
        /* Generate lHash */
        ret = compute_hash((Crypto_AlgorithmFamilyType)ctx->hash_id, label, label_len, lhash);
        if (ret != 0)
        {
            goto cleanup;
        }
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
int Crypto_rsa_pkcs1_decrypt(
    Crypto_rsa_context* ctx,
    Std_ReturnType (*f_rng)(void*, unsigned char*, uint32),
    void*                p_rng,
    uint32*              olen,
    const unsigned char* input,
    unsigned char*       output,
    uint32               output_max_len)
{
    Std_ReturnType ret = E_NOT_OK;
    *olen              = 0u;

    switch (ctx->padding)
    {
    case CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5:
        ret = Crypto_rsa_rsaes_pkcs1_v15_decrypt(ctx, f_rng, p_rng, olen, input, output, output_max_len);

        break;
    case CRYPTO_ALGOMODE_RSAES_OAEP:
        ret = Crypto_rsa_rsaes_oaep_decrypt(ctx, f_rng, p_rng, NULL_PTR, 0u, olen, input, output, output_max_len);
        break;
    default:
        return E_NOT_OK;
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
    if (ret != 0)
    {
        Crypto_rsa_free(dst);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_Rsa_EncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType     ret = E_NOT_OK;
    Crypto_rsa_context rsa;

    uint8*  rsa_plaintext  = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint8*  rsa_ciphertext = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr);
    uint32  inLen          = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32  keylength      = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint32* ciphertextLen  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint8*  key;
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
        ret            = Crypto_rsa_pkcs1_encrypt(&rsa, myrand, NULL_PTR, inLen, rsa_plaintext, rsa_ciphertext);
        *ciphertextLen = rsa.len;
    }
    (void)IStdLib_MemSet(key, 0, keylength);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    Crypto_rsa_free(&rsa);
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_Rsa_DecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret = E_NOT_OK;

    Crypto_rsa_context rsa_ctx;
    uint32             keylength = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint8*             key;
    key                    = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, keylength);
    uint32* outLen         = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint8*  rsa_ciphertext = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint8*  rsa_plaintext  = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr);

    Crypto_rsa_init(&rsa_ctx, mode, secondaryFam);

    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &keylength);
    if (ret == E_OK)
    {
        ret         = Crypto_rsa_parse_key(&rsa_ctx, key, keylength);
        rsa_ctx.len = Crypto_mpi_size(&rsa_ctx.N);
    }
    if (ret == E_OK)
    {
        ret = Crypto_rsa_pkcs1_decrypt(&rsa_ctx, myrand, NULL_PTR, outLen, rsa_ciphertext, rsa_plaintext, keylength);
    }
    (void)IStdLib_MemSet(key, 0, keylength);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);

    Crypto_rsa_free(&rsa_ctx);

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Rsa_SignatureGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret = E_NOT_OK;

    /**0. variable definition */
    Crypto_rsa_context      rsa_ctx;
    Crypto_Md_Context_t     md_ctx;
    const Crypto_Md_Info_t* mdInfo       = NULL_PTR;
    uint8*                  rsaSignature = NULL_PTR;
    uint32                  sigLen;
    uint8*                  key      = NULL_PTR;
    uint8*                  mdOutput = NULL_PTR;
    uint32                  mdSize;
    const uint8*            inputPtr  = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint8*                  outputPtr = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr);
    uint32                  inputLen  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32                  outBufLen = *Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32 keyLength     = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint32 rsa_init_flag = CRYPTO_CONST_0;

    if (inputPtr == NULL_PTR || outputPtr == NULL_PTR || outBufLen <= 0 || inputLen <= 0 || keyLength <= 0)
    {
        ret = E_NOT_OK;
        goto exit;
    }

    /**1. compute digest */
    (void)IStdLib_MemSet(&md_ctx, 0, sizeof(Crypto_Md_Context_t));
    mdInfo = Crypto_Md_info_from_type(secondaryFam);
    ret    = Crypto_md_setup(&md_ctx, mdInfo, 0);
    if (ret != E_OK)
    {
        goto exit;
    }
    mdSize   = md_ctx.md_info->size;
    mdOutput = IStdLib_MemHeapMalloc(Crypto_62_MemPool, mdSize);
    ret      = Crypto_Md_Start(&md_ctx);
    if (ret != E_OK)
    {
        goto exit;
    }
    ret = Crypto_Md_Update(&md_ctx, inputPtr, inputLen);
    if (ret != E_OK)
    {
        goto exit;
    }
    ret = Crypto_Md_Finish(&md_ctx, mdOutput);
    if (ret != E_OK)
    {
        goto exit;
    }

    /**2. get key */
    key = (uint8*)IStdLib_MemHeapCalloc(
        Crypto_62_MemPool,
        1u,
        keyLength); // should assign according to the selected length passed in
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_SIGNATURE_KEY, key, &keyLength);
    if (ret != E_OK)
    {
        goto exit;
    }

    /**3. compute signature */
    Crypto_rsa_init(&rsa_ctx, mode, secondaryFam);
    rsa_init_flag = CRYPTO_CONST_1;
    ret           = Crypto_rsa_parse_key(&rsa_ctx, key, keyLength);
    if (ret != E_OK)
    {
        goto exit;
    }

    sigLen       = rsa_ctx.len;
    rsaSignature = IStdLib_MemHeapMalloc(Crypto_62_MemPool, sigLen);
    ret          = Crypto_rsa_pkcs1_sign(&rsa_ctx, myrand, NULL_PTR, secondaryFam, mdSize, mdOutput, rsaSignature);
    if (ret != E_OK)
    {
        goto exit;
    }

    /**4. return result */
    if (ret == E_OK && outBufLen >= sigLen)
    {
        outBufLen = sigLen;
        (void)IStdLib_MemCpy(outputPtr, rsaSignature, outBufLen);
    }
    else
    {
        ret = E_NOT_OK; // buffer len not enough
        (void)IStdLib_MemCpy(outputPtr, rsaSignature, outBufLen);
    }

exit:
    if (rsaSignature != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, rsaSignature);
    }
    if (mdOutput != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, mdOutput);
    }
    if (key != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    }
    if (md_ctx.md_ctx != NULL_PTR)
    {
        Crypto_md_free(&md_ctx);
    }
    if (rsa_init_flag == CRYPTO_CONST_1)
    {
        Crypto_rsa_free(&rsa_ctx);
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
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
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_Rsa_SignatureVerifyProcess(
    uint32                     objectId,
    Crypto_AlgorithmModeType   mode,
    Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret = E_NOT_OK;
    /**0. variable definition */
    Crypto_Md_Context_t     md_ctx;
    Crypto_rsa_context      rsa_ctx;
    const Crypto_Md_Info_t* md_info;
    uint32                  mdSize;
    uint8*                  key      = NULL_PTR;
    uint8*                  mdOutput = NULL_PTR;
    uint8*       inputRsaSigPtr      = (uint8*)Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.secondaryInputPtr;
    const uint8* inputPtr            = (uint8*)(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr);
    uint32       inputLen            = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32       keyLength = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;

    if (inputPtr == NULL_PTR || inputRsaSigPtr == NULL_PTR || inputLen <= 0 || keyLength <= 0)
    {
        ret = E_NOT_OK;
        goto exit;
    }

    /**1. compute digest */
    md_info  = Crypto_Md_info_from_type(secondaryFam);
    mdSize   = md_info->size;
    mdOutput = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, mdSize);

    ret = Crypto_md_setup(&md_ctx, md_info, 0);
    if (ret != E_OK)
    {
        goto exit;
    }
    ret = Crypto_Md_Start(&md_ctx);
    if (ret != E_OK)
    {
        goto exit;
    }
    ret = Crypto_Md_Update(&md_ctx, inputPtr, inputLen);
    if (ret != E_OK)
    {
        goto exit;
    }
    ret = Crypto_Md_Finish(&md_ctx, mdOutput);
    if (ret != E_OK)
    {
        goto exit;
    }

    /**2. get key */
    key = (uint8*)IStdLib_MemHeapCalloc(
        Crypto_62_MemPool,
        1u,
        keyLength); // should assign according to the selected length passed in
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_SIGNATURE_KEY, key, &keyLength);
    if (ret != E_OK)
    {
        goto exit;
    }

    /**3. rsa signature verify*/
    Crypto_rsa_init(&rsa_ctx, mode, secondaryFam);
    ret = Crypto_rsa_parse_pubkey(&rsa_ctx, key, keyLength);
    if (ret != E_OK)
    {
        goto exit;
    }
    ret = Crypto_rsa_pkcs1_verify(&rsa_ctx, secondaryFam, mdSize, mdOutput, inputRsaSigPtr);
    if (ret != E_OK)
    {
        goto exit;
    }

    /**4. return result */
    if (E_OK == ret)
    {
        *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_OK;
    }
    else
    {
        *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_NOT_OK;
    }

exit:
    if (mdOutput != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, mdOutput);
    }
    if (key != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    }
    if (md_ctx.md_ctx != NULL_PTR)
    {
        Crypto_md_free(&md_ctx);
    }
    Crypto_rsa_free(&rsa_ctx);

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif
/* PRQA S 0311,0314,0316,0488,1339,2016,2023,2024,2052,2105,3102,3103,3208,3326,3345 -- */
/* PRQA S 1253,0288,0317,0489,0490,0693,0770,0772,0780,0842,1252,1258,1259,1277,4825 -- */
/* PRQA S 1290,1291,1336,1338,1840,1820,1821,1823,1841,1842,1843,1844,1851,1853,1860 -- */
/* PRQA S 1861,1863,1880,1881,1890,1891,2001,2015,2205,2106,2140,2462,2463,2472,2487 -- */
/* PRQA S 2740,3101,3120,3122,3123,3140,3200,3206,3209,3218,3219,3226,3332,3344,3387 -- */
/* PRQA S 3395,3396,3397,3400,3408,3410,3418,3450,3440,3455,3610,3625,3672,3678,3715 -- */
/* PRQA S 3717,3762,3769,3772,3779,4115,4116,4393,4394,4403,4404,4413,4414,4423,4432 -- */
/* PRQA S 4434,4443,4446,4447,4460,4461,4464,4470,4480,4491,4499,4501,4538,4542,4544 -- */
/* PRQA S 4532,4533,4543,4558,3673,3795,4150,4522,3391,3432,3442,2743,1505,2889,2834 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,2996,2992,2911,1503,2822,2896,2986,2990 -- */
/* PRQA S 2834,2982,2983,2955,1532,2986,2982,2983,5022,5143,5014,5031,5045,2839,2998 -- */
/* PRQA S 2995,2997,2871 -- */
