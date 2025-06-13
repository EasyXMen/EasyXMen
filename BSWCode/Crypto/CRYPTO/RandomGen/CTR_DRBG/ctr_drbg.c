/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : ctr_drbg.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for ctr_drbg                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/* PRQA S 3432 ++ */                               /* MISRA Rule 20.7 */
/* PRQA S 0488 ++ */                               /* MISRA Rule 18.4 */
/* PRQA S 2212 ++ */                               /* MISRA Rule 15.6 */
/* PRQA S 3415 ++ */                               /* MISRA Rule 13.5 */
/* PRQA S 3326 ++ */                               /* MISRA Rule 13.4 */
/* PRQA S 3440 ++ */                               /* MISRA Rule 13.3 */
/* PRQA S 3397,3396 ++ */                          /* MISRA Rule 12.1 */
/* PRQA S 3304,3004 ++ */                          /* MISRA Rule 11.9 */
/* PRQA S 0311 ++ */                               /* MISRA Rule 11.8 */
/* PRQA S 1863,1844,1843,1842,1841,1840,1823 ++ */ /* MISRA Rule 10.4 */
/* PRQA S 4461,4443,4434,4558,4542,4538 ++ */      /* MISRA Rule 10.3 */
/* PRQA S 5209 ++  */                              /* MISRA Dir 4.6 */
/* PRQA S 1505,1532 ++  */                         /* MISRA Rule 8.7 */
/* PRQA S 2982,2983,2985,2986  ++ */               /* MISRA Rule 2.2 */
/* PRQA S 1503  ++ */                              /* MISRA Rule 2.1 */
#include "ctr_drbg.h"
#include "Crypto_internal.h"
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"
#if (CRYPTO_ALGORITHM_CTRG == STD_ON)
FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_reseed(Crypto_CtrDrbgData* ctx, P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) additional, uint32 len);

FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_update_ret(
    Crypto_CtrDrbgData* ctx,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) additional,
    uint32 add_len);

FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_random_with_add(
    void* p_rng,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output,
    uint32 output_len,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) additional,
    uint32 add_len);

FUNC(void, CRYPTO_CODE)
mbedtls_ctr_drbg_update(
    Crypto_CtrDrbgData* ctx,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) additional,
    uint32 add_len);

static FUNC(Std_ReturnType, CRYPTO_CODE) block_cipher_df(
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) data,
    uint32 data_len)
{
    uint8 buf[CTR_DRBG_MAX_SEED_INPUT + CTR_DRBG_BLOCKSIZE + 16];
    uint8 tmp[CTR_DRBG_SEEDLEN];
    uint8 key[CTR_DRBG_KEYSIZE];
    uint8 chain[CTR_DRBG_BLOCKSIZE];
    uint8 *p, *iv;
    Crypto_AESData aes_ctx;
    Std_ReturnType ret;
    int i, j;
    uint32 buf_len, use_len;

    Crypto_memset(buf, (CTR_DRBG_MAX_SEED_INPUT + CTR_DRBG_BLOCKSIZE + 16));
    Crypto_memset(&aes_ctx, sizeof(Crypto_AESData));

    p = buf + CTR_DRBG_BLOCKSIZE;
    *p++ = (data_len >> 24) & 0xff;
    *p++ = (data_len >> 16) & 0xff;
    *p++ = (data_len >> 8) & 0xff;
    *p++ = (data_len) & 0xff;
    p += 3;
    *p++ = CTR_DRBG_SEEDLEN;
    Crypto_memcpy(p, (uint8*)data, data_len);
    p[data_len] = 0x80;

    buf_len = CTR_DRBG_BLOCKSIZE + 8 + data_len + 1;

    for (i = 0; i < CTR_DRBG_KEYSIZE; i++)
        key[i] = i;

    ret = Crypto_aes_setkey_enc(&aes_ctx, key, CTR_DRBG_KEYBITS);
    /*
     * Reduce data to CTR_DRBG_SEEDLEN bytes of data
     */
    for (j = 0; j < CTR_DRBG_SEEDLEN; j += CTR_DRBG_BLOCKSIZE)
    {
        p = buf;
        Crypto_memset(chain, CTR_DRBG_BLOCKSIZE);
        use_len = buf_len;

        while (use_len > 0)
        {
            for (i = 0; i < CTR_DRBG_BLOCKSIZE; i++)
                chain[i] ^= p[i];
            p += CTR_DRBG_BLOCKSIZE;
            use_len -= (use_len >= CTR_DRBG_BLOCKSIZE) ? CTR_DRBG_BLOCKSIZE : use_len;

            ret = Crypto_aes_crypt_ecb(&aes_ctx, AES_ENCRYPT, chain, chain);
        }

        Crypto_memcpy(tmp + j, chain, CTR_DRBG_BLOCKSIZE);

        /*
         * Update IV
         */
        buf[3]++;
    }

    /*
     * Do final encryption with reduced data
     */
    ret = Crypto_aes_setkey_enc(&aes_ctx, tmp, CTR_DRBG_KEYBITS);
    iv = tmp + CTR_DRBG_KEYSIZE;
    p = output;

    for (j = 0; j < CTR_DRBG_SEEDLEN; j += CTR_DRBG_BLOCKSIZE)
    {
        ret = Crypto_aes_crypt_ecb(&aes_ctx, AES_ENCRYPT, iv, iv);
        Crypto_memcpy(p, iv, CTR_DRBG_BLOCKSIZE);
        p += CTR_DRBG_BLOCKSIZE;
    }
    if (E_OK != ret)
    {
        /*
         * wipe partial seed from memory
         */
        Crypto_memset(output, CTR_DRBG_SEEDLEN);
    }

    return ret;
}

static FUNC(Std_ReturnType, CRYPTO_CODE)
    ctr_drbg_update_internal(Crypto_CtrDrbgData* ctx, const uint8 data[CTR_DRBG_SEEDLEN])
{
    uint8 tmp[CTR_DRBG_SEEDLEN];
    uint8* p = tmp;
    int i, j;
    Std_ReturnType ret;

    Crypto_memset(tmp, CTR_DRBG_SEEDLEN);

    for (j = 0; j < CTR_DRBG_SEEDLEN; j += CTR_DRBG_BLOCKSIZE)
    {
        /*
         * Increase counter
         */
        for (i = CTR_DRBG_BLOCKSIZE; i > 0; i--)
            if (++ctx->counter[i - 1] != 0)
                break;

        /*
         * Crypt counter block
         */
        ret = Crypto_aes_crypt_ecb(&ctx->aes_ctx, AES_ENCRYPT, ctx->counter, p);

        p += CTR_DRBG_BLOCKSIZE;
    }

    for (i = 0; i < CTR_DRBG_SEEDLEN; i++)
        tmp[i] ^= data[i];

    /*
     * Update key and counter
     */
    ret = Crypto_aes_setkey_enc(&ctx->aes_ctx, tmp, CTR_DRBG_KEYBITS);
    Crypto_memcpy(ctx->counter, tmp + CTR_DRBG_KEYSIZE, CTR_DRBG_BLOCKSIZE);

    return ret;
}

/* CTR_DRBG_Instantiate with derivation function (SP 800-90A &sect;10.2.1.3.2)
 * mbedtls_ctr_drbg_update(ctx,additional,add_len)
 * implements
 * CTR_DRBG_Instantiate(entropy_input,nonce,personalization_string,
 *                      security_strength)-> initial_working_state
 * with inputs
 *   ctx->counter = all-bits-0
 *   ctx->aes_ctx = context from all-bits-0 key
 *   additional[:add_len] = entropy_input || nonce || personalization_string
 * and with outputs
 *   ctx = initial_working_state
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_update_ret(
    Crypto_CtrDrbgData* ctx,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) additional,
    uint32 add_len)
{
    uint8 add_input[CTR_DRBG_SEEDLEN];
    Std_ReturnType ret;

    if (add_len == 0)
        return (0);

    ret = block_cipher_df(add_input, additional, add_len);
    if (E_OK == ret)
    {
        ret = ctr_drbg_update_internal(ctx, add_input);
    }

    return ret;
}

void mbedtls_ctr_drbg_update(
    Crypto_CtrDrbgData* ctx,
    P2CONST(uint8, AUTOMATIC, CRYPTO_APPL_DATA) additional,
    uint32 add_len)
{
    /* MAX_INPUT would be more logical here,but we have to match
     * block_cipher_df()'s limits since can't propagate errors */
    if (add_len > CTR_DRBG_MAX_SEED_INPUT)
        add_len = CTR_DRBG_MAX_SEED_INPUT;
    (void)mbedtls_ctr_drbg_update_ret(ctx, additional, add_len);
}

/* CTR_DRBG_Reseed with derivation function (SP 800-90A &sect;10.2.1.4.2)
 * mbedtls_ctr_drbg_reseed(ctx,additional,len)
 * implements
 * CTR_DRBG_Reseed(working_state,entropy_input,additional_input)
 *                -> new_working_state
 * with inputs
 *   ctx contains working_state
 *   additional[:len] = additional_input
 * and entropy_input comes from calling ctx->f_entropy
 * and with output
 *   ctx contains new_working_state
 */
FUNC(Std_ReturnType, CRYPTO_CODE) mbedtls_ctr_drbg_reseed(Crypto_CtrDrbgData* ctx, const uint8* additional, uint32 len)
{
    uint8 seed[CTR_DRBG_MAX_SEED_INPUT] = {0x00}; /* PRQA S 0686  */ /* MISRA Rule 9.3 */
    uint32 seedlen = 0;
    Std_ReturnType ret;

    Crypto_memset(seed, CTR_DRBG_MAX_SEED_INPUT);
    /*
     * Gather entropy_len bytes of entropy to seed state
     */
    ret = ctx->f_entropy(ctx->p_entropy, seed, ctx->entropy_len);
    if (0 == ret)
    {
        seedlen += ctx->entropy_len;
    }

    /*
     * Add additional data
     */
    if (additional != 0 && len != 0)
    {
        Crypto_memcpy(seed + seedlen, (uint8*)additional, len);
        seedlen += len;
    }

    /*
     * Reduce to 256 bits
     */
    ret = block_cipher_df(seed, seed, seedlen);

    /*
     * Update state
     */
    ret = ctr_drbg_update_internal(ctx, seed);

    ctx->reseed_counter = 1;

    return ret;
}
/* Return a "good" nonce length for CTR_DRBG. The chosen nonce length
 * is sufficient to achieve the maximum security strength given the key
 * size and entropy length. If there is enough entropy in the initial
 * call to the entropy function to serve as both the entropy input and
 * the nonce, don't make a second call to get a nonce. */
static uint32 good_nonce_len(uint32 entropy_len)
{
    if (entropy_len >= CTR_DRBG_KEYSIZE * 3 / 2)
        return (0);
    else
        return ((entropy_len + 1) / 2);
}

FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_seed(
    P2VAR(Crypto_CtrDrbgData, AUTOMATIC, CRYPTO_APPL_DATA) ctx,
    /* PRQA S 1335 ++ */ /* MISRA Rule 8.2 */
    Std_ReturnType (*f_entropy)(void*, uint8*, uint32),
    /* PRQA S 1335 -- */ /* MISRA Rule 8.2 */

    P2VAR(Crypto_entropy_context, AUTOMATIC, CRYPTO_APPL_DATA) p_entropy,
    const uint8* custom,
    uint32 len)
{
    Std_ReturnType ret;
    uint8 key[CTR_DRBG_KEYSIZE];
    uint32 nonce_len;

    Crypto_memset(&ctx->aes_ctx, sizeof(Crypto_CtrDrbgData));
    Crypto_memset(key, CTR_DRBG_KEYSIZE);

    ctx->f_entropy = f_entropy;
    ctx->p_entropy = p_entropy;

    if (ctx->entropy_len == 0)
    {
        ctx->entropy_len = CTR_DRBG_ENTROPY_LEN;
    }
    nonce_len = (ctx->reseed_counter >= 0 ? (uint32)ctx->reseed_counter : good_nonce_len(ctx->entropy_len));

    /*
     * Initialize with an empty key
     */
    ret = Crypto_aes_setkey_enc(&ctx->aes_ctx, key, CTR_DRBG_KEYBITS);
    if (ret == E_NOT_OK)
    {
        return ret;
    }
    ret = mbedtls_ctr_drbg_reseed(ctx, custom, len);

    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_random_with_add(
    void* p_rng,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output,
    uint32 output_len,
    const uint8* additional,
    uint32 add_len)
{
    Std_ReturnType ret = E_NOT_OK;
    Crypto_CtrDrbgData* ctx = (Crypto_CtrDrbgData*)p_rng;
    uint8 add_input[CTR_DRBG_SEEDLEN];
    uint8* p = output;
    uint8 tmp[CTR_DRBG_BLOCKSIZE];
    int i;
    uint32 use_len;

    if (output_len > CTR_DRBG_MAX_REQUEST)
    {
        return ret;
    }
    if (add_len > CTR_DRBG_MAX_INPUT)
    {
        return ret;
    }
    Crypto_memset(add_input, CTR_DRBG_SEEDLEN);

    if (ctx->reseed_counter > ctx->reseed_interval || ctx->prediction_resistance)
    {
        ret = mbedtls_ctr_drbg_reseed(ctx, additional, add_len);
        if (ret != E_OK)
        {
            return ret;
        }
        add_len = 0;
    }

    if (add_len > 0)
    {
        ret = block_cipher_df(add_input, additional, add_len);
        if (ret != E_OK)
        {
            return ret;
        }
        ret = ctr_drbg_update_internal(ctx, add_input);
        if (ret != E_OK)
        {
            return ret;
        }
    }

    while (output_len > 0)
    {
        /*
         * Increase counter
         */
        for (i = CTR_DRBG_BLOCKSIZE; i > 0; i--)
            if (++ctx->counter[i - 1] != 0)
                break;

        /*
         * Crypt counter block
         */
        ret = Crypto_aes_crypt_ecb(&ctx->aes_ctx, AES_ENCRYPT, ctx->counter, tmp);
        if (ret != E_OK)
        {
            return ret;
        }

        use_len = (output_len > CTR_DRBG_BLOCKSIZE) ? CTR_DRBG_BLOCKSIZE : output_len;
        /*
         * Copy random block to destination
         */
        Crypto_memcpy(p, tmp, use_len);
        p += use_len;
        output_len -= use_len;
    }

    ret = ctr_drbg_update_internal(ctx, add_input);
    if (ret != E_OK)
    {
        return ret;
    }

    ctx->reseed_counter++;

    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_random(void* p_rng, P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output, uint32 output_len)
{
    Std_ReturnType ret;
    Crypto_CtrDrbgData* ctx = (Crypto_CtrDrbgData*)p_rng;
#if defined(INTERNALRAND)
    {
        uint8 key[16];
        srand(time(0));
        for (int a = 0; a < 16; a++)
        {

            key[a] = rand() % 255;
        }
        ret = mbedtls_ctr_drbg_random_with_add(ctx, output, output_len, key, 16);
    }
#else

    ret = mbedtls_ctr_drbg_random_with_add(ctx, output, output_len, NULL_PTR, 0);
#endif
    return ret;
}

static int entropy_update(
    Crypto_entropy_context* ctx,
    unsigned char source_id,
    unsigned char* data,
    uint32 len) /* PRQA S 3408  */ /* MISRA Rule 8.4 */
{
    unsigned char header[2];
    unsigned char tmp[64];
    uint32 use_len = len;
    unsigned char* p = data;
    int ret;

    if (use_len > 64)
    {
        if ((ret = Crypto_sha256(data, len, tmp)) != E_OK)
        {
            Crypto_memset(tmp, sizeof(tmp));
        }
        p = tmp;
        use_len = 64;
    }

    header[0] = source_id;
    header[1] = use_len & 0xFF;

    /*
     * Start the accumulator if this has not already happened. Note that
     * it is sufficient to start the accumulator here only because all calls to
     * gather entropy eventually execute this code.
     */

    if (ctx->accumulator_started == 0 && (ret = Crypto_sha256_starts_ret(&ctx->accumulator)) != E_OK)
    {
        Crypto_memset(tmp, sizeof(tmp));
    }
    else
        ctx->accumulator_started = 1;

    if ((ret = Crypto_sha256_update_ret(&ctx->accumulator, header, 2)) != E_OK)
    {
        Crypto_memset(tmp, sizeof(tmp));
    }

    ret = Crypto_sha256_update_ret(&ctx->accumulator, p, use_len);

    return ret;
}

Std_ReturnType entropy_gather_internal(Crypto_entropy_context* ctx) /* PRQA S 3408  */ /* MISRA Rule 8.4 */
{
    Std_ReturnType ret = MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    int i;
    int have_one_strong;
    unsigned char buf[128];
    uint32 olen;

    if (ctx->source_count == 0)
    {
        ret = MBEDTLS_ERR_ENTROPY_NO_SOURCES_DEFINED;
        goto exit;
    }

    /*
     * Run through our entropy sources
     */
    for (i = 0; i < ctx->source_count; i++)
    {
        if (ctx->source[i].strong == 1)
        {
            have_one_strong = 1;
        }
        olen = 0;
        if ((ret = ctx->source[i].f_source(ctx->source[i].p_source, buf, 128, &olen)) != 0)
        {
            Crypto_memset(buf, sizeof(buf));
            olen = 0;
        }
        /*
         * Add if we actually gathered something
         */
        if (olen > 0)
        {
            if ((ret = entropy_update(ctx, (unsigned char)i, buf, olen)) != 0)
            {
                goto exit;
            }
            ctx->source[i].size += olen;
        }
    }

    if (have_one_strong == 0)
    {
        ret = MBEDTLS_ERR_ENTROPY_NO_STRONG_SOURCE;
    }

exit:
    return ret;
}

Std_ReturnType mbedtls_entropy_func(void* data, uint8* output, uint32 len)
{
    Std_ReturnType ret;
    int count = 0;
    int i;
    int thresholds_reached;
    uint32 strong_size;
    Crypto_entropy_context* ctx = (Crypto_entropy_context*)data;
    uint8 buf[32];

    if (len > 32u)
    {
        ret = MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
        goto exit;
    }
    do
    {
        if (count++ > 256)
        {
            ret = MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
            goto exit;
        }

        if ((ret = entropy_gather_internal(ctx)) != 0)
        {
            goto exit;
        }

        thresholds_reached = 1;
        strong_size = 0;

        for (i = 0; i < ctx->source_count; i++)
        {
            if (ctx->source[i].size < ctx->source[i].threshold)
            {
                thresholds_reached = 0;
            }
            if (ctx->source[i].strong == 1u)
            {
                strong_size += ctx->source[i].size;
            }
        }
    } while (!thresholds_reached || strong_size < 32u);

    Crypto_memset(buf, sizeof(buf));

    if ((ret = Crypto_sha256_finish_ret(&ctx->accumulator, buf)) != E_OK)
    {
        goto exit;
    }

    Crypto_memset(&ctx->accumulator, sizeof(Crypto_Sha256Data));

    if ((ret = Crypto_sha256_starts_ret(&ctx->accumulator)) != E_OK)
    {
        goto exit;
    }
    if ((ret = Crypto_sha256_update_ret(&ctx->accumulator, buf, 32)) != 0)
    {
        goto exit;
    }

    if ((ret = Crypto_sha256(buf, 32, buf)) != 0)
    {
        goto exit;
    }

    for (i = 0; i < ctx->source_count; i++)
    {
        ctx->source[i].size = 0;
    }

    Crypto_memcpy(output, buf, len);

    ret = E_OK;
exit:
    Crypto_memset(buf, sizeof(buf));
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_CtrDrbg(uint8 key[16], uint8 buf[16])
{
    Crypto_CtrDrbgData ctx;
    Crypto_entropy_context entropy;
    Std_ReturnType ret;
    Crypto_memset(&ctx, sizeof(Crypto_CtrDrbgData));
    Crypto_memset(&entropy, sizeof(Crypto_entropy_context));
    ctx.entropy_len = 16;

#if defined(INTERNALRAND)
    {
        for (int a = 0; a < 16; a++)
        {
            key[a] = rand() % 255;
        }
        mbedtls_ctr_drbg_seed(&ctx, mbedtls_entropy_func, &entropy, key, 16);
    }
#else
    {
        ret = mbedtls_ctr_drbg_seed(&ctx, mbedtls_entropy_func, &entropy, key, 16);
    }
#endif

    ret = mbedtls_ctr_drbg_random(&ctx, buf, 16);

    Crypto_memset(&ctx, sizeof(Crypto_CtrDrbgData));

    return ret;
}

/*
 * CTR_DRBG context initialization
 */
void mbedtls_ctr_drbg_init(Crypto_CtrDrbgData* ctx)
{
    Crypto_memset(ctx, sizeof(Crypto_CtrDrbgData));
    /* Indicate that the entropy nonce length is not set explicitly.
     * See mbedtls_ctr_drbg_set_nonce_len(). */
    ctx->reseed_counter = -1;

    ctx->reseed_interval = 10000;
}
#endif /* CRYPTO_ALGORITHM_CTRG == STD_ON */
#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

/* PRQA S 3432 -- */                               /* MISRA Rule 20.7 */
/* PRQA S 0488 -- */                               /* MISRA Rule 18.4 */
/* PRQA S 2212 -- */                               /* MISRA Rule 15.6 */
/* PRQA S 3415 -- */                               /* MISRA Rule 13.5 */
/* PRQA S 3326 -- */                               /* MISRA Rule 13.4 */
/* PRQA S 3440 -- */                               /* MISRA Rule 13.3 */
/* PRQA S 3397,3396 -- */                          /* MISRA Rule 12.1 */
/* PRQA S 3304,3004 -- */                          /* MISRA Rule 11.9 */
/* PRQA S 0311 -- */                               /* MISRA Rule 11.8 */
/* PRQA S 1863,1844,1843,1842,1841,1840,1823 -- */ /* MISRA Rule 10.4 */
/* PRQA S 4461,4443,4434,4558,4542,4538 -- */      /* MISRA Rule 10.3 */
/* PRQA S 5209 --  */                              /* MISRA Dir 4.6 */
/* PRQA S 1505,1532 --  */                         /* MISRA Rule 8.7 */
/* PRQA S 2982,2983,2985,2986  --*/                /* MISRA Rule 2.2 */
/* PRQA S 1503  --*/                               /* MISRA Rule 2.1 */
