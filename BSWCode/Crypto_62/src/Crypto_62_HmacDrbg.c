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
 **  @file               : Crypto_62_HmacDrbg.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 0488,0316,0772,1253,1258,1291,1290,1331,1336,1823,1840,1842,1843 ++ */ /* VL_QAC_Crypto */
/* PRQA S 2001,2015,4434,1252,3219,3396,3397,3672,3715,4443 ++ */                /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_SERVICE_RANDOM == STD_ON)
#if (CRYPTO_ALGORITHMMODE_HMAC == STD_ON)

#include "Crypto_62_HmacDrbg.h"
#include "Crypto_62_Mac.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ===================================================== macros ===================================================== */
/**
 * \name SECTION: Module settings
 *
 * The configuration options you can set for this module are in this section.
 * Either change them in Crypto_config.h or define them on the compiler command line.
 * \{
 */

#define CRYPTO_HMAC_DRBG_RESEED_INTERVAL 10000U /**< Interval before reseed is performed by default */

#define CRYPTO_HMAC_DRBG_MAX_INPUT 256U /**< Maximum number of additional input bytes */

#define CRYPTO_HMAC_DRBG_MAX_REQUEST 1024U /**< Maximum number of requested bytes per call */

#define CRYPTO_HMAC_DRBG_MAX_SEED_INPUT 384U /**< Maximum size of (re)seed buffer */

/** \} name SECTION: Module settings */

#define CRYPTO_HMAC_DRBG_PR_OFF 0U /**< No prediction resistance       */
#define CRYPTO_HMAC_DRBG_PR_ON  1U /**< Prediction resistance enabled  */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
CRYPTO_62_LOCAL void Crypto_hmac_drbg_init(Crypto_hmac_drbg_context* ctx);
CRYPTO_62_LOCAL int
    Crypto_hmac_drbg_update(Crypto_hmac_drbg_context* ctx, const unsigned char* additional, uint32 add_len);
CRYPTO_62_LOCAL int Crypto_hmac_drbg_reseed(Crypto_hmac_drbg_context* ctx, const unsigned char* additional, uint32 len);
CRYPTO_62_LOCAL Std_ReturnType Crypto_hmac_drbg_seed(
    Crypto_hmac_drbg_context* ctx,
    const Crypto_Md_Info_t*   md_info,
    Std_ReturnType (*f_entropy)(void*, unsigned char*, uint32),
    void*                p_entropy,
    const unsigned char* custom,
    uint32               len);
CRYPTO_62_LOCAL int Crypto_hmac_drbg_random_with_add(
    void*                p_rng,
    unsigned char*       output,
    uint32               out_len,
    const unsigned char* additional,
    uint32               add_len);
CRYPTO_62_LOCAL int            Crypto_hmac_drbg_random(void* p_rng, unsigned char* output, uint32 out_len);
CRYPTO_62_LOCAL void           Crypto_hmac_drbg_free(Crypto_hmac_drbg_context* ctx);
CRYPTO_62_LOCAL Std_ReturnType Crypto_hmac_drbg_seed_buf(
    Crypto_hmac_drbg_context* ctx,
    const Crypto_Md_Info_t*   md_info,
    const unsigned char*      data,
    uint32                    data_len);
CRYPTO_62_LOCAL int
    hmac_drbg_reseed_core(Crypto_hmac_drbg_context* ctx, const unsigned char* additional, uint32 len, int use_nonce);

CRYPTO_62_LOCAL void Crypto_hmac_drbg_set_prediction_resistance(Crypto_hmac_drbg_context* ctx, int resistance);
CRYPTO_62_LOCAL void Crypto_hmac_drbg_set_entropy_len(Crypto_hmac_drbg_context* ctx, uint32 len);
CRYPTO_62_LOCAL void Crypto_hmac_drbg_set_reseed_interval(Crypto_hmac_drbg_context* ctx, int interval);

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== internal function definitions ========================================= */
/******************************************************************************/
/*
 * Brief               HMAC_DRBG context initialization.
 *
 * Param-Name[in]      ctx: HMAC_DRBG context to be initialized.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_hmac_drbg_init(Crypto_hmac_drbg_context* ctx)
{
    (void)IStdLib_MemSet(ctx, 0, sizeof(Crypto_hmac_drbg_context));

    ctx->reseed_interval = CRYPTO_HMAC_DRBG_RESEED_INTERVAL;
}

/******************************************************************************/
/*
 * Brief               This function updates the state of the HMAC_DRBG context.
 *
 * Param-Name[in]      additional: The data to update the state with. If this is
 *                                  NULL, there is no additional data.
 *                     add_len: Length of additional in bytes. Unused if additional
 *                              is NULL.
 * Param-Name[in/out]  ctx: The HMAC_DRBG context.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int
    Crypto_hmac_drbg_update(Crypto_hmac_drbg_context* ctx, const unsigned char* additional, uint32 add_len)
{
    uint32 md_len = Crypto_md_get_size(ctx->md_ctx.md_info);
    /* PRQA S 3400,4461,3844,1844 ++ */ /* VL_QAC_Crypto */
    unsigned char rounds = (additional != NULL_PTR && add_len != 0) ? CRYPTO_CONST_2 : 1;
    /* PRQA S 3400,4461,3844,1844 -- */
    unsigned char sep[1];
    unsigned char K[CRYPTO_MD_MAX_SIZE];
    int           ret = E_NOT_OK;

    for (sep[0] = 0; sep[0] < rounds; sep[0]++)
    {
        /* Step 1 or 4 */
        ret = Crypto_GenerateHmac_Update(&ctx->md_ctx, ctx->V, md_len);
        if (ret != E_OK)
        {
            goto exit;
        }
        ret = Crypto_GenerateHmac_Update(&ctx->md_ctx, sep, 1);
        if (ret != E_OK)
        {
            goto exit;
        }
        if (rounds == CRYPTO_CONST_2)
        {
            ret = Crypto_GenerateHmac_Update(&ctx->md_ctx, additional, add_len);
            if (ret != E_OK)
            {
                goto exit;
            }
        }
        ret = Crypto_GenerateHmac_Finish(&ctx->md_ctx, K);
        if (ret != E_OK)
        {
            goto exit;
        }

        /* Step 2 or 5 */
        ret = Crypto_GenerateHmac_Start(&ctx->md_ctx, K, md_len);
        if (ret != E_OK)
        {
            goto exit;
        }
        ret = Crypto_GenerateHmac_Update(&ctx->md_ctx, ctx->V, md_len);
        if (ret != E_OK)
        {
            goto exit;
        }
        ret = Crypto_GenerateHmac_Finish(&ctx->md_ctx, ctx->V);
        if (ret != E_OK)
        {
            goto exit;
        }
    }

exit:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Simplified HMAC_DRBG initialisation (for use with
 *                     deterministic ECDSA).
 *
 * Param-Name[in]      md_info: MD algorithm to use for HMAC_DRBG.
 *                     data: Concatenation of the initial entropy string and
 *                           the additional data.
 *                     data_len: Length of data in bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: HMAC_DRBG context to be initialised.
 * Return              Std_ReturnType: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType Crypto_hmac_drbg_seed_buf(
    Crypto_hmac_drbg_context* ctx,
    const Crypto_Md_Info_t*   md_info,
    const unsigned char*      data,
    uint32                    data_len)
{
    Std_ReturnType ret = E_NOT_OK;
    ret                = Crypto_md_setup(&ctx->md_ctx, md_info);
    if (ret != E_OK)
    {
        return ret;
    }

    /*
     * Set initial working state.
     * Use the V memory location, which is currently all 0, to initialize the
     * MD context with an all-zero key. Then set V to its initial value.
     */
    ret = Crypto_GenerateHmac_Start(&ctx->md_ctx, ctx->V, Crypto_md_get_size(md_info));
    if (ret != E_OK)
    {
        return ret;
    }
    (void)IStdLib_MemSet(ctx->V, 1, Crypto_md_get_size(md_info));
    ret = Crypto_hmac_drbg_update(ctx, data, data_len);
    if (ret != E_OK)
    {
        return ret;
    }

    return E_OK;
}

/******************************************************************************/
/*
 * Brief               Internal function used both for seeding and reseeding the
 *                     DRBG. Comments starting with arabic numbers refer to section
 *                     10.1.2.4 of SP800-90A, while roman numbers refer to section 9.2.
 *
 * Param-Name[in]      additional: Additional information to reseed.
 *                     len: The length of input data.
 *                     use_nonce: Indication of using nonce or not.
 * Param-Name[in/out]  ctx: The HMAC_DRBG context.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int
    hmac_drbg_reseed_core(Crypto_hmac_drbg_context* ctx, const unsigned char* additional, uint32 len, int use_nonce)
{
    unsigned char seed[CRYPTO_HMAC_DRBG_MAX_SEED_INPUT];
    uint32        seedlen = 0;
    int           ret     = E_NOT_OK;

    {
        uint32 total_entropy_len;

        if (use_nonce == 0)
        {
            total_entropy_len = ctx->entropy_len;
        }
        else
        {
            total_entropy_len = ctx->entropy_len * CRYPTO_CONST_3 / CRYPTO_CONST_2;
        }

        /* III. Check input length */
        if ((len > CRYPTO_HMAC_DRBG_MAX_INPUT) || (total_entropy_len + len > CRYPTO_HMAC_DRBG_MAX_SEED_INPUT))
        {
            return E_NOT_OK;
        }
    }

    (void)IStdLib_MemSet(seed, 0, CRYPTO_HMAC_DRBG_MAX_SEED_INPUT);

    /* IV. Gather entropy_len bytes of entropy for the seed */
    ret = ctx->f_entropy(ctx->p_entropy, seed, ctx->entropy_len);
    if (ret != E_OK)
    {
        return E_NOT_OK;
    }
    seedlen += ctx->entropy_len;

    /* For initial seeding, allow adding of nonce generated
     * from the entropy source. See Sect 8.6.7 in SP800-90A. */
    if (use_nonce) /* PRQA S 3344 */ /* VL_QAC_Crypto */
    {
        /* Note: We don't merge the two calls to f_entropy() in order
         *       to avoid requesting too much entropy from f_entropy()
         *       at once. Specifically, if the underlying digest is not
         *       SHA-1, 3 / 2 * entropy_len is at least 36 Bytes, which
         *       is larger than the maximum of 32 Bytes that our own
         *       entropy source implementation can emit in a single
         *       call in configurations disabling SHA-512. */
        ret = ctx->f_entropy(ctx->p_entropy, seed + seedlen, ctx->entropy_len / CRYPTO_CONST_2);
        if (ret != E_OK)
        {
            return E_NOT_OK;
        }

        seedlen += ctx->entropy_len / CRYPTO_CONST_2;
    }

    /* 1. Concatenate entropy and additional data if any */
    if ((additional != NULL_PTR) && (len != 0))
    {
        (void)IStdLib_MemCpy(seed + seedlen, additional, len);
        seedlen += len;
    }

    /* 2. Update state */
    ret = Crypto_hmac_drbg_update(ctx, seed, seedlen);
    if (ret != E_OK)
    {
        goto exit;
    }

    /* 3. Reset reseed_counter */
    ctx->reseed_counter = 1;

exit:
    /* 4. Done */

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function reseeds the HMAC_DRBG context, that is
 *                     extracts data from the entropy source.
 *
 * Param-Name[in]      additional: Additional data to add to the state. If this
 *                                  is NULL, there is no additional data and len
 *                                 should be 0.
 *                     len: The length of the additional data.
 * Param-Name[in/out]  ctx: The HMAC_DRBG context.
 * Param-Name[out]     None
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int Crypto_hmac_drbg_reseed(Crypto_hmac_drbg_context* ctx, const unsigned char* additional, uint32 len)
{
    return hmac_drbg_reseed_core(ctx, additional, len, 0);
}

/******************************************************************************/
/*
 * Brief               HMAC_DRBG initialisation (10.1.2.3 + 9.1). The nonce is
 *                     not passed as a separate parameter but extracted from the
 *                     entropy source as suggested in 8.6.7.
 *
 * Param-Name[in]      md_info: MD algorithm to use for HMAC_DRBG.
 *                     f_entropy: The entropy callback, taking as arguments the
 *                                p_entropy context, the buffer to fill, and the
 *                                length of the buffer. f_entropy is always called
 *                                with a length that is less than or equal to the
 *                                entropy length.
 *                     p_entropy: The entropy context to pass to \p f_entropy.
 *                     custom: The personalization string. This can be NULL, in
 *                             which case the personalization string is empty
 *                             regardless of the value of len.
 *                     len: The length of the personalization string.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: HMAC_DRBG context to be seeded.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType Crypto_hmac_drbg_seed(
    Crypto_hmac_drbg_context* ctx,
    const Crypto_Md_Info_t*   md_info,
    Std_ReturnType (*f_entropy)(void*, unsigned char*, uint32),
    void*                p_entropy,
    const unsigned char* custom,
    uint32               len)
{
    int    ret = E_NOT_OK;
    uint32 md_size;
    ret = Crypto_md_setup(&ctx->md_ctx, md_info);
    if (ret != E_OK)
    {
        return ret;
    }

    md_size = Crypto_md_get_size(md_info);

    /*
     * Set initial working state.
     * Use the V memory location, which is currently all 0, to initialize the
     * MD context with an all-zero key. Then set V to its initial value.
     */
    ret = Crypto_GenerateHmac_Start(&ctx->md_ctx, ctx->V, md_size);
    if (ret != E_OK)
    {
        return ret;
    }
    (void)IStdLib_MemSet(ctx->V, 1, md_size);

    ctx->f_entropy = f_entropy;
    ctx->p_entropy = p_entropy;

    if (ctx->entropy_len == 0)
    {
        /*
         * See SP800-57 5.6.1 (p. 65-66) for the security strength provided by
         * each hash function, then according to SP800-90A rev1 10.1 table 2,
         * min_entropy_len (in bits) is security_strength.
         *
         * (This also matches the sizes used in the NIST test vectors.)
         */
        /* PRQA S 1824,3391 ++ */                                         /* VL_QAC_Crypto */
        ctx->entropy_len = md_size <= CRYPTO_CONST_20 ? CRYPTO_CONST_16 : /* 160-bits hash -> 128 bits */
                               md_size <= CRYPTO_CONST_28 ? CRYPTO_CONST_24
                                                          : /* 224-bits hash -> 192 bits */
                               CRYPTO_CONST_32;             /* better (256+) -> 256 bits */
        /* PRQA S 1824,3391 -- */
    }
    ret = hmac_drbg_reseed_core(ctx, custom, len, 1 /* add nonce */);
    if (ret != E_OK)
    {
        return ret;
    }

    return 0;
}

/******************************************************************************/
/*
 * Brief               This function turns prediction resistance on or off.
 *                     The default value is off.
 *
 * Param-Name[in]      resistance: Indication of needing prediction resistance
 *                                 or not. It can be CRYPTO_HMAC_DRBG_PR_OFF or
 *                                 CRYPTO_HMAC_DRBG_PR_ON.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The HMAC_DRBG context.
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_hmac_drbg_set_prediction_resistance(Crypto_hmac_drbg_context* ctx, int resistance)
{
    ctx->prediction_resistance = resistance;
}

/******************************************************************************/
/*
 * Brief               This function sets the amount of entropy grabbed on each
 *                     seed or reseed.
 *
 * Param-Name[in]      len: The amount of entropy to grab, in bytes.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The HMAC_DRBG context.
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_hmac_drbg_set_entropy_len(Crypto_hmac_drbg_context* ctx, uint32 len)
{
    ctx->entropy_len = len;
}

/******************************************************************************/
/*
 * Brief               Set the reseed interval.
 *
 * Param-Name[in]      interval: The reseed interval.
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The HMAC_DRBG context.
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_hmac_drbg_set_reseed_interval(Crypto_hmac_drbg_context* ctx, int interval)
{
    ctx->reseed_interval = interval;
}

/******************************************************************************/
/*
 * Brief               This function updates an HMAC_DRBG instance with additional
 *                     data and uses it to generate random data. This function
 *                     automatically reseeds if the reseed counter is exceeded
 *                     or prediction resistance is enabled.
 *
 * Param-Name[in]      p_rng: The HMAC_DRBG context.
 *                     additional: Additional data to update with. If this is NULL,
 *                                 there is no additional data and add_len should
 *                                 be 0.
 *                     add_len: The length of the additional data.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer to fill.
 *                     out_len: The length of the buffer in bytes.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int Crypto_hmac_drbg_random_with_add(
    void*                p_rng,
    unsigned char*       output,
    uint32               out_len,
    const unsigned char* additional,
    uint32               add_len)
{
    int                       ret         = E_NOT_OK;
    Crypto_hmac_drbg_context* ctx         = (Crypto_hmac_drbg_context*)p_rng;
    uint32                    md_len      = Crypto_md_get_size(ctx->md_ctx.md_info);
    uint32                    left        = out_len;
    unsigned char*            out         = output;
    uint32                    tmp_add_len = add_len;

    /* II. Check request length */
    if (out_len > CRYPTO_HMAC_DRBG_MAX_REQUEST)
    {
        return E_NOT_OK;
    }

    /* III. Check input length */
    if (tmp_add_len > CRYPTO_HMAC_DRBG_MAX_INPUT)
    {
        return E_NOT_OK;
    }

    /* 1. (aka VII and IX) Check reseed counter and PR */
    if ((ctx->f_entropy != NULL_PTR) && /* For no-reseeding instances */
        ((ctx->prediction_resistance == CRYPTO_HMAC_DRBG_PR_ON) || (ctx->reseed_counter > ctx->reseed_interval)))
    {
        ret = Crypto_hmac_drbg_reseed(ctx, additional, tmp_add_len);
        if (ret != E_OK)
        {
            return ret;
        }

        tmp_add_len = 0; /* VII.4 */
    }

    /* 2. Use additional data if any */
    if ((additional != NULL_PTR) && (tmp_add_len != 0))
    {
        ret = Crypto_hmac_drbg_update(ctx, additional, tmp_add_len);
        if (ret != E_OK)
        {
            goto exit;
        }
    }

    /* 3, 4, 5. Generate bytes */
    while (left != 0)
    {
        uint32 use_len = left > md_len ? md_len : left;
        ret            = Crypto_GenerateHmac_Update(&ctx->md_ctx, ctx->V, md_len);
        if (ret != E_OK)
        {
            goto exit;
        }
        ret = Crypto_GenerateHmac_Finish(&ctx->md_ctx, ctx->V);
        if (ret != E_OK)
        {
            goto exit;
        }

        (void)IStdLib_MemCpy(out, ctx->V, use_len);
        out += use_len;
        left -= use_len;
    }

    /* 6. Update */
    ret = Crypto_hmac_drbg_update(ctx, additional, tmp_add_len);
    if (ret != E_OK)
    {
        goto exit;
    }

    /* 7. Update reseed counter */
    ctx->reseed_counter++;

exit:
    /* 8. Done */
    return ret;
}

/******************************************************************************/
/*
 * Brief               HMAC_DRBG random function.
 *
 * Param-Name[in]      p_rng: The HMAC_DRBG context.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer to fill.
 *                     out_len: The length of the buffer in bytes.
 * Return              int: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL int Crypto_hmac_drbg_random(void* p_rng, unsigned char* output, uint32 out_len)
{
    int                       ret = E_NOT_OK;
    Crypto_hmac_drbg_context* ctx = (Crypto_hmac_drbg_context*)p_rng;

    ret = Crypto_hmac_drbg_random_with_add(ctx, output, out_len, NULL_PTR, 0);

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function resets HMAC_DRBG context to the initialized
 *                     state.
 *
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     ctx: The HMAC_DRBG context to free.
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL void Crypto_hmac_drbg_free(Crypto_hmac_drbg_context* ctx)
{
    if (ctx == NULL_PTR)
    {
        return;
    }

    ctx->reseed_interval = CRYPTO_HMAC_DRBG_RESEED_INTERVAL;
}

/* ========================================== external function definitions ========================================= */
Std_ReturnType Crypto_HmacDrbgProcess(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily)
{
    Std_ReturnType           ret = E_NOT_OK;
    Crypto_hmac_drbg_context ctx;
    Crypto_entropy_context   entropy;
    uint8*                   buf = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint8                    key[CRYPTO_CONST_16];
    uint32                   keylength = CRYPTO_CONST_16;
    uint32                   outLen    = *Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    ret                                = Crypto_62_KeyElementGet(
        Crypto_62_StoredJob[objectId].cryptoKeyId,
        CRYPTO_KE_RANDOM_SEED_STATE,
        &key[0],
        &keylength);
    Crypto_entropy_init(&entropy);
    Crypto_hmac_drbg_init(&ctx);
    /*
     * Based on a NIST CTR_DRBG test vector (PR = True)
     */

    ctx.entropy_len    = CRYPTO_CONST_32;
    ctx.reseed_counter = CRYPTO_CONST_16;
    if (E_OK == ret)
    {
        ret = Crypto_hmac_drbg_seed(
            &ctx,
            Crypto_Md_info_from_type(algorithmfamily),
            Crypto_entropy_func,
            &entropy,
            key,
            keylength);
    }

    if (E_OK == ret)
    {
        ctx.prediction_resistance = 1U;
        ret                       = Crypto_hmac_drbg_random(&ctx, buf, outLen);
    }
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, ctx.md_ctx.md_ctx);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, ctx.md_ctx.hmac_ctx);
    return ret;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif
#endif

/* PRQA S 0488,0316,0772,1253,1258,1291,1290,1331,1336,1823,1840,1842,1843 -- */
/* PRQA S 2001,2015,4434,1252,3219,3396,3397,3672,3715,4443 -- */
