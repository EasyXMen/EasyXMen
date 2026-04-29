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
 **  @file               : Crypto_62_CtrDrbg.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 6010 EOF */ /* VL_MTR_Crypto_62_STCYC */
/* PRQA S 6020 EOF */ /* VL_MTR_Crypto_62_STLIN */
/* PRQA S 6030 EOF */ /* VL_MTR_Crypto_62_STMIF */
/* PRQA S 6040 EOF */ /* VL_MTR_Crypto_62_STPAR */
/* PRQA S 6050 EOF */ /* VL_MTR_Crypto_62_STST3 */
/* PRQA S 6060 EOF */ /* VL_MTR_Crypto_62_STM19 */
/* PRQA S 6070 EOF */ /* VL_MTR_Crypto_62_STCAL */
/* PRQA S 6080 EOF */ /* VL_MTR_Crypto_62_STPTH */

/* PRQA S 0488,0316,1252,1253,1290,1840,1842,2001,2015,3397,4436,2889,2784,2986,2995 ++ */ /* VL_Crypto_62_General */
/* PRQA S 2997,2889,2972,1503,1505,1532 ++ */                                              /* VL_Crypto_62_General */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMMODE_CTRDRBG == STD_ON)

#include "Crypto_62_Random.h"
#include "Crypto_62_CtrDrbg.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

CRYPTO_62_LOCAL Std_ReturnType Crypto_Block_Cipher_Df(
    uint8*                     output,
    const uint8*               data,
    uint32                     data_len,
    Crypto_AlgorithmFamilyType algorithmfamily);
CRYPTO_62_LOCAL Std_ReturnType Crypto_Ctr_Drbg_Update_Internal(
    Crypto_Ctr_Drbg_Context*   ctx,
    const uint8                data[CRYPTO_CTR_DRBG_SEEDLEN],
    Crypto_AlgorithmFamilyType algorithmfamily);
CRYPTO_62_LOCAL Std_ReturnType Crypto_ctr_drbg_reseed_internal(
    Crypto_Ctr_Drbg_Context* ctx,
    const uint8*             additional,
    uint32                   len,
    uint32                   nonce_len);

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               This function initializes the CTR_DRBG context.

 * Param-Name[in]      algorithmfamily: Define the inner algorithm, it can be
 *                                      CRYPTO_ALGOFAM_AES.
 * Param-Name[in/out]  ctx: The context of CTR_DRBG.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_Ctr_Drbg_Init(Crypto_Ctr_Drbg_Context* ctx, Crypto_AlgorithmFamilyType algorithmfamily)
{
    (void)IStdLib_MemSet(ctx, 0, sizeof(Crypto_Ctr_Drbg_Context));

    if (algorithmfamily == CRYPTO_ALGOFAM_AES)
    {
    }
    else if (algorithmfamily == CRYPTO_ALGOFAM_3DES)
    {
    }
    else
    {
        /* do nothing */
    }
    /* Indicate that the entropy nonce length is not set explicitly.
     * See Crypto_ctr_drbg_set_nonce_len(). */
    ctx->reseed_counter = 0;

    ctx->reseed_interval = CRYPTO_CTR_DRBG_RESEED_INTERVAL;
    ctx->algorithmfamily = algorithmfamily;
}

/******************************************************************************/
/*
 * Brief               This function sets prediction resistance.

 * Param-Name[in]      resistance: resistance to be set.
 * Param-Name[in/out]  ctx: The context of CTR_DRBG.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_Ctr_Drbg_Set_Prediction_Resistance(Crypto_Ctr_Drbg_Context* ctx, uint32 resistance)
{
    ctx->prediction_resistance = resistance;
}

/******************************************************************************/
/*
 * Brief               This function sets entropy length.

 * Param-Name[in]      len: entropy length to be set.
 * Param-Name[in/out]  ctx: The context of CTR_DRBG.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_Ctr_Drbg_Set_Entropy_Len(Crypto_Ctr_Drbg_Context* ctx, uint32 len)
{
    ctx->entropy_len = len;
}

/******************************************************************************/
/*
 * Brief               This function sets nonce length.

 * Param-Name[in]      len: nonce length to be set.
 * Param-Name[in/out]  ctx: The context of CTR_DRBG.
 * Param-Name[out]     None
 * Return              Std_ReturnType: E_OK: State accepted
 *                          E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Ctr_Drbg_Set_Nonce_Len(Crypto_Ctr_Drbg_Context* ctx, uint32 len)
{
    Std_ReturnType ret = E_NOT_OK;
    /* If Crypto_ctr_drbg_seed() has already been called, it's
     * too late. Return the error code that's closest to making sense. */
    if (ctx->f_entropy != NULL_PTR)
    {
        ret = E_NOT_OK;
    }

    else if (len > CRYPTO_CTR_DRBG_MAX_SEED_INPUT)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* For backward compatibility with Mbed TLS <= 2.19, store the
         * entropy nonce length in a field that already exists, but isn't
         * used until after the initial seeding. */
        /* Due to the capping of len above, the value fits in an int. */
        ctx->reseed_counter = len;
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function sets reseed interval.

 * Param-Name[in]      interval: reseed interval to be set.
 * Param-Name[in/out]  ctx: The context of CTR_DRBG.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_Ctr_Drbg_Set_reseed_interval(Crypto_Ctr_Drbg_Context* ctx, uint32 interval)
{
    ctx->reseed_interval = interval;
}

/******************************************************************************/
/*
 * Brief               Derivation function.

 * Param-Name[in]      data: Buffer stored seed material.
 *                     data_len: The length of data in byte.
 *                     algorithmfamily: Define the inner algorithm, it can be
 *                                      CRYPTO_ALGOFAM_AES.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: Buffer stored the seed.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType Crypto_Block_Cipher_Df(
    uint8*                     output,
    const uint8*               data,
    uint32                     data_len,
    Crypto_AlgorithmFamilyType algorithmfamily)
{
    uint8          buf[CRYPTO_CTR_DRBG_MAX_SEED_INPUT + CRYPTO_CTR_DRBG_BLOCKSIZE + CRYPTO_CONST_16];
    uint8          tmp[CRYPTO_CTR_DRBG_SEEDLEN];
    uint8          key[CRYPTO_CTR_DRBG_KEYSIZE];
    uint8          chain[CRYPTO_CTR_DRBG_BLOCKSIZE];
    uint8 *        p, *iv;
    Std_ReturnType ret = E_OK;

#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
    Crypto_AESData aes_ctx;
#endif

    uint8  i, j;
    uint32 buf_len, use_len;
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
    Crypto_Des3Data des3_ctx;
#endif

    if (data_len > CRYPTO_CTR_DRBG_MAX_SEED_INPUT)
    {
        return E_NOT_OK;
    }

    (void)IStdLib_MemSet(buf, 0, CRYPTO_CTR_DRBG_MAX_SEED_INPUT + CRYPTO_CTR_DRBG_BLOCKSIZE + CRYPTO_CONST_16);

    /*
     * Construct IV (16 bytes) and S in buffer
     * IV = Counter (in 32-bits) padded to 16 with zeroes
     * S = Length input string (in 32-bits) || Length of output (in 32-bits) ||
     *     data || 0x80
     *     (Total is padded to a multiple of 16-bytes with zeroes)
     */
    p = buf + CRYPTO_CTR_DRBG_BLOCKSIZE;
    CRYPTO_PUT_UINT32_BE(data_len, p, 0); /* PRQA S 3138 */ /* VL_Crypto_62_General */
    p += CRYPTO_CONST_4 + CRYPTO_CONST_3;
    *p++ = CRYPTO_CTR_DRBG_SEEDLEN; /* PRQA S 3387,3440 */ /* VL_Crypto_62_General */
    (void)IStdLib_MemCpy(p, data, data_len);
    p[data_len] = CRYPTO_CONST_0x80;

    buf_len = CRYPTO_CTR_DRBG_BLOCKSIZE + CRYPTO_CONST_8 + data_len + 1;

    for (i = 0; i < CRYPTO_CTR_DRBG_KEYSIZE; i++)
    {
        key[i] = i;
    }

    if (algorithmfamily == CRYPTO_ALGOFAM_AES)
    {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
        (void)IStdLib_MemSet(&aes_ctx, 0, sizeof(Crypto_AESData));

        Crypto_Aes_Setkey_Enc(&aes_ctx, key, CRYPTO_CTR_DRBG_KEYBITS);
#endif
    }
    else
    {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
        Crypto_Des_Setkey(&des3_ctx, key);
#endif
    }

    /*
     * Reduce data to CRYPTO_CTR_DRBG_SEEDLEN bytes of data
     */
    for (j = 0; j < CRYPTO_CTR_DRBG_SEEDLEN; j += CRYPTO_CTR_DRBG_BLOCKSIZE)
    {
        p = buf;
        (void)IStdLib_MemSet(chain, 0, CRYPTO_CTR_DRBG_BLOCKSIZE);
        use_len = buf_len;

        while (use_len > 0)
        {
            Crypto_Xor(CRYPTO_CTR_DRBG_BLOCKSIZE, chain, p, chain);
            p += CRYPTO_CTR_DRBG_BLOCKSIZE;
            use_len -= (use_len >= CRYPTO_CTR_DRBG_BLOCKSIZE) ? CRYPTO_CTR_DRBG_BLOCKSIZE : use_len;

            if (algorithmfamily == CRYPTO_ALGOFAM_AES)
            {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
                ret = Internal_Aes_Encrypt(&aes_ctx, chain, chain);
                if (ret != E_OK)
                {
                    goto exit;
                }
#endif
            }
            else
            {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
                Crypto_3Des_Crypt_Ecb(&des3_ctx, chain, chain);
#endif
            }
        }

        (void)IStdLib_MemCpy(tmp + j, chain, CRYPTO_CTR_DRBG_BLOCKSIZE);

        /*
         * Update IV
         */
        buf[CRYPTO_CONST_3]++;
    }

    /*
     * Do final encryption with reduced data
     */
    if (algorithmfamily == CRYPTO_ALGOFAM_AES)
    {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
        Crypto_Aes_Setkey_Enc(&aes_ctx, tmp, CRYPTO_CTR_DRBG_KEYBITS);
#endif
    }
    else
    {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
        Crypto_Des_Setkey(&des3_ctx, tmp);
#endif
    }

    iv = tmp + CRYPTO_CTR_DRBG_KEYSIZE;
    p  = output;

    for (j = 0; j < CRYPTO_CTR_DRBG_SEEDLEN; j += CRYPTO_CTR_DRBG_BLOCKSIZE)
    {
        if (algorithmfamily == CRYPTO_ALGOFAM_AES)
        {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            ret = Internal_Aes_Encrypt(&aes_ctx, iv, iv);
            if (ret != CRYPTO_CONST_0)
            {
                goto exit;
            }
#endif
        }
        else
        {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
            Crypto_3Des_Crypt_Ecb(&des3_ctx, iv, iv);

#endif
        }

        (void)IStdLib_MemCpy(p, iv, CRYPTO_CTR_DRBG_BLOCKSIZE);
        p += CRYPTO_CTR_DRBG_BLOCKSIZE;
    }
exit:

    /*
     * tidy up the stack
     */

    return ret;
}

/******************************************************************************/
/*
 * Brief               Inner function to update context of CTR_DRBG.
 *
 * Param-Name[in]      data: Input data.
 *                     algorithmfamily: Define the inner algorithm, it can be
 *                                      CRYPTO_ALGOFAM_AES.
 * Param-Name[in/out]  ctx: Context of CTR_DRBG.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType Crypto_Ctr_Drbg_Update_Internal(
    Crypto_Ctr_Drbg_Context*   ctx,
    const uint8                data[CRYPTO_CTR_DRBG_SEEDLEN],
    Crypto_AlgorithmFamilyType algorithmfamily)
{
    uint8          tmp[CRYPTO_CTR_DRBG_SEEDLEN];
    uint8*         p = tmp;
    uint32         j;
    Std_ReturnType ret = E_OK;

    (void)IStdLib_MemSet(tmp, 0, CRYPTO_CTR_DRBG_SEEDLEN);

    for (j = 0; j < CRYPTO_CTR_DRBG_SEEDLEN; j += CRYPTO_CTR_DRBG_BLOCKSIZE)
    {
        /*
         * Increase counter
         */
        Crypto_Ctr_Increment_Counter(ctx->counter);

        /*
         * Crypt counter block
         */
        if (algorithmfamily == CRYPTO_ALGOFAM_AES)
        {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            ret = Internal_Aes_Encrypt(&ctx->aes_ctx, ctx->counter, p);
            if (ret != CRYPTO_CONST_0)
            {
                goto exit;
            }
#endif
        }
        else
        {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
            Crypto_3Des_Crypt_Ecb(&ctx->des3_ctx, ctx->counter, p);

#endif
        }

        p += CRYPTO_CTR_DRBG_BLOCKSIZE;
    }

    Crypto_Xor(CRYPTO_CTR_DRBG_SEEDLEN, tmp, (const uint8*)data, tmp);

    /*
     * Update key and counter
     */
    if (algorithmfamily == CRYPTO_ALGOFAM_AES)
    {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
        Crypto_Aes_Setkey_Enc(&ctx->aes_ctx, tmp, CRYPTO_CTR_DRBG_KEYBITS);
#endif
    }
    else
    {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
        Crypto_Des_Setkey(&ctx->des3_ctx, tmp);
#endif
    }

    (void)IStdLib_MemCpy(ctx->counter, tmp + CRYPTO_CTR_DRBG_KEYSIZE, CRYPTO_CTR_DRBG_BLOCKSIZE);

exit:

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function updates the state of the CTR_DRBG context.
 *
 * Param-Name[in]      additional: The data to update the state with.
 *                     add_len: Length of additional in bytes.
 *                     algorithmfamily: Define the inner algorithm, it can be
 *                                      CRYPTO_ALGOFAM_AES.
 * Param-Name[in/out]  ctx: The CTR_DRBG context.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_ctr_drbg_update(
    Crypto_Ctr_Drbg_Context*   ctx,
    const uint8*               additional,
    uint32                     add_len,
    Crypto_AlgorithmFamilyType algorithmfamily)
{
    uint8          add_input[CRYPTO_CTR_DRBG_SEEDLEN];
    Std_ReturnType ret = E_NOT_OK;

    if (add_len == CRYPTO_CONST_0)
    {
        return E_OK;
    }
    ret = Crypto_Block_Cipher_Df(add_input, additional, add_len, algorithmfamily);
    if (ret != CRYPTO_CONST_0)
    {
        goto exit;
    }
    ret = Crypto_Ctr_Drbg_Update_Internal(ctx, add_input, algorithmfamily);
    if (ret != CRYPTO_CONST_0)
    {
        goto exit;
    }

exit:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Inner function of Crypto_ctr_drbg_reseed.
 *
 * Param-Name[in]      additional: Additional data to add to the state. Can be NULL.
 *                     len: The length of the additional data in byte.
 *                     nonce_len: The length of nonce in byte.
 *
 *
 * Param-Name[in/out]  ctx: The CTR_DRBG context.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType
    Crypto_ctr_drbg_reseed_internal(Crypto_Ctr_Drbg_Context* ctx, const uint8* additional, uint32 len, uint32 nonce_len)
{
    uint8          seed[CRYPTO_CTR_DRBG_MAX_SEED_INPUT];
    uint32         seedlen = 0;
    Std_ReturnType ret     = E_NOT_OK;

    if (ctx->entropy_len > CRYPTO_CTR_DRBG_MAX_SEED_INPUT)
    {
        return E_NOT_OK;
    }
    if (nonce_len > CRYPTO_CTR_DRBG_MAX_SEED_INPUT - ctx->entropy_len)
    {
        return E_NOT_OK;
    }
    if (len > CRYPTO_CTR_DRBG_MAX_SEED_INPUT - ctx->entropy_len - nonce_len)
    {
        return E_NOT_OK;
    }

    (void)IStdLib_MemSet(seed, 0, CRYPTO_CTR_DRBG_MAX_SEED_INPUT);

    /* Gather entropy_len bytes of entropy to seed state. */
    if (E_OK != ctx->f_entropy(ctx->p_entropy, seed, ctx->entropy_len))
    {
        return E_NOT_OK;
    }
    seedlen += ctx->entropy_len;

    /* Gather entropy for a nonce if requested. */
    if (nonce_len != CRYPTO_CONST_0)
    {
        if (E_OK != ctx->f_entropy(ctx->p_entropy, seed + seedlen, nonce_len))
        {
            return E_NOT_OK;
        }
        seedlen += nonce_len;
    }

    /* Add additional data if provided. */
    if ((additional != NULL_PTR) && (len != CRYPTO_CONST_0))
    {
        (void)IStdLib_MemCpy(seed + seedlen, additional, len);
        seedlen += len;
    }
    ret = Crypto_Block_Cipher_Df(seed, seed, seedlen, ctx->algorithmfamily);
    /* Reduce to 384 bits. */
    if (ret != E_OK)
    {
        goto exit;
    }
    ret = Crypto_Ctr_Drbg_Update_Internal(ctx, seed, ctx->algorithmfamily);
    /* Update state. */
    if (ret != E_OK)
    {
        goto exit;
    }
    ctx->reseed_counter = 1;

exit:

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function reseeds the CTR_DRBG context, that is
 *                     extracts data from the entropy source.
 *
 * Param-Name[in]      additional: Additional data to add to the state. Can be NULL.
 *                     len: The length of the additional data.
 *                     algorithmfamily: Define the inner algorithm, it can be
 *                                      CRYPTO_ALGOFAM_AES.
 * Param-Name[in/out]  ctx: The CTR_DRBG context.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_ctr_drbg_reseed(Crypto_Ctr_Drbg_Context* ctx, const uint8* additional, uint32 len)
{
    return Crypto_ctr_drbg_reseed_internal(ctx, additional, len, 0u);
}

/******************************************************************************/
/*
 * Brief               This function seeds and sets up the CTR_DRBG entropy
 *                     source for future reseeds.
 *
 * Param-Name[in]      f_entropy: The function that can get entropy from accumulator.
 *                     p_entropy: The entropy context to pass to f_entropy.
 *                     custom:  The personalization string. This can be NULL, in
 *                              which case the personalization string is empty
 *                              regardless of the value of len.
 *                     len: The length of the personalization string.
 *                     algorithmfamily: Define the inner algorithm, it can be
 *                                      CRYPTO_ALGOFAM_AES.
 * Param-Name[in/out]  ctx: The context for CTR_DRBG.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_ctr_drbg_seed(
    Crypto_Ctr_Drbg_Context* ctx,
    /* PRQA S 1336,3672++ */ /* VL_Crypto_62_General */
    Std_ReturnType (*f_entropy)(void*, uint8*, uint32),
    /* PRQA S 1336,3672-- */
    void*        p_entropy,
    const uint8* custom,
    uint32       len)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          key[CRYPTO_CTR_DRBG_KEYSIZE];
    uint32         nonce_len;

    (void)IStdLib_MemSet(key, 0, CRYPTO_CTR_DRBG_KEYSIZE);

    ctx->f_entropy = f_entropy;
    ctx->p_entropy = p_entropy;

    if (ctx->entropy_len == CRYPTO_CONST_0)
    {
        ctx->entropy_len = CRYPTO_CTR_DRBG_ENTROPY_LEN;
    }
    /* ctx->reseed_counter contains the desired amount of entropy to
     * grab for a nonce (see Crypto_ctr_drbg_set_nonce_len()).
     * If it's -1, indicating that the entropy nonce length was not set
     * explicitly, use a sufficiently large nonce for security. */
    /* PRQA S 3396++ */ /* VL_Crypto_62_General */
    nonce_len = (uint32)ctx->reseed_counter;
    /* PRQA S 3396-- */
    /* Initialize with an empty key. */
    if (ctx->algorithmfamily == CRYPTO_ALGOFAM_AES)
    {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
        Crypto_Aes_Setkey_Enc(&ctx->aes_ctx, key, CRYPTO_CTR_DRBG_KEYBITS);
#endif
    }
    else
    {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
        Crypto_Des_Setkey(&ctx->des3_ctx, key);
#endif
    }
    ret = Crypto_ctr_drbg_reseed_internal(ctx, custom, len, nonce_len);
    /* Do the initial seeding. */
    if (ret != E_OK)
    {
        return ret;
    }
    return 0;
}

/******************************************************************************/
/*
 * Brief               This function updates a CTR_DRBG context with additional
 *                     data and uses it to generate random data.
 *
 * Param-Name[in]      p_rng: The CTR_DRBG context.
 *                     output_len: The length of the requested random bits in
 *                                 bytes.
 *                     additional: Additional data to update. Can be NULL, in
 *                                 which case the additional data is empty
 *                                 regardless of the value of add_len.
 *                     add_len: The length of the additional data.
 *                     algorithmfamily: Define the inner algorithm, it can be
 *                                      CRYPTO_ALGOFAM_AES.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer to fill.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_ctr_drbg_random_with_add(
    void*        p_rng,
    uint8*       output,
    uint32       output_len,
    const uint8* additional,
    uint32       add_len)
{
    Std_ReturnType           ret            = E_OK;
    Crypto_Ctr_Drbg_Context* ctx            = (Crypto_Ctr_Drbg_Context*)p_rng;
    uint8*                   p              = output;
    uint32                   tmp_add_len    = add_len;
    uint32                   tmp_output_len = output_len;
    struct
    {
        uint8 add_input[CRYPTO_CTR_DRBG_SEEDLEN];
        uint8 tmp[CRYPTO_CTR_DRBG_BLOCKSIZE];
    } locals;
    uint32 use_len;

    if (tmp_output_len > CRYPTO_CTR_DRBG_MAX_REQUEST)
    {
        return E_NOT_OK;
    }

    if (tmp_add_len > CRYPTO_CTR_DRBG_MAX_INPUT)
    {
        return E_NOT_OK;
    }

    (void)IStdLib_MemSet(locals.add_input, 0, CRYPTO_CTR_DRBG_SEEDLEN);

    if ((ctx->reseed_counter > ctx->reseed_interval) || (ctx->prediction_resistance > 0U))
    {
        ret = Crypto_ctr_drbg_reseed(ctx, additional, tmp_add_len);
        if (ret != E_OK)
        {
            return ret;
        }
        tmp_add_len = 0;
    }

    if (tmp_add_len > 0)
    {
        ret = Crypto_Block_Cipher_Df(locals.add_input, additional, tmp_add_len, ctx->algorithmfamily);
        if (ret != E_OK)
        {
            goto exit;
        }
        ret = Crypto_Ctr_Drbg_Update_Internal(ctx, locals.add_input, ctx->algorithmfamily);
        if (ret != E_OK)
        {
            goto exit;
        }
    }

    while (tmp_output_len > 0)
    {
        /*
         * Increase counter (treat it as a 128-bit big-endian integer).
         */
        Crypto_Ctr_Increment_Counter(ctx->counter);

        /*
         * Crypt counter block
         */
        if (ctx->algorithmfamily == CRYPTO_ALGOFAM_AES)
        {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            ret = Internal_Aes_Encrypt(&ctx->aes_ctx, ctx->counter, locals.tmp);
            if (ret != CRYPTO_CONST_0)
            {
                goto exit;
            }
#endif
        }
        else
        {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
            Crypto_3Des_Crypt_Ecb(&ctx->des3_ctx, ctx->counter, locals.tmp);

#endif
        }

        use_len = (tmp_output_len > CRYPTO_CTR_DRBG_BLOCKSIZE) ? CRYPTO_CTR_DRBG_BLOCKSIZE : tmp_output_len;
        /*
         * Copy random block to destination
         */
        (void)IStdLib_MemCpy(p, locals.tmp, use_len);
        p += use_len;
        tmp_output_len -= use_len;
    }
    ret = Crypto_Ctr_Drbg_Update_Internal(ctx, locals.add_input, ctx->algorithmfamily);
    if (ret != E_OK)
    {
        goto exit;
    }

    ctx->reseed_counter++;

exit:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Upper interface to do CTR_DRBG and output random bits.
 *
 * Param-Name[in]      p_rng: The CTR_DRBG context.
 *                     output_len: The length of the requested random bits in bytes.
 *                     algorithmfamily: Define the inner algorithm, it can be
 *                                      CRYPTO_ALGOFAM_AES.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The buffer to fill.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_ctr_drbg_random(void* p_rng, uint8* output, uint32 output_len)
{
    Std_ReturnType           ret = E_NOT_OK;
    Crypto_Ctr_Drbg_Context* ctx = (Crypto_Ctr_Drbg_Context*)p_rng;

    ret = Crypto_ctr_drbg_random_with_add(ctx, output, output_len, NULL_PTR, 0u);

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function do the counter random process .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     algorithmfamily: familly for the encrypt
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
Std_ReturnType Crypto_CtrDrbgProcess(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily)
{
    Std_ReturnType          ret = E_NOT_OK;
    Crypto_Ctr_Drbg_Context ctx;
    Crypto_entropy_context  entropy;
    uint8*                  buf = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint8                   key[CRYPTO_CONST_16];
    uint32                  keylength = CRYPTO_CONST_16;
    uint32                  outLen    = *Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    ret                               = Crypto_62_KeyElementGet(
        Crypto_62_StoredJob[objectId].cryptoKeyId,
        /* PRQA S 1258++ */ /* VL_Crypto_62_General */
        CRYPTO_KE_RANDOM_SEED_STATE,
        /* PRQA S 1258-- */
        &key[0],
        &keylength);
    Crypto_entropy_init(&entropy);
    Crypto_Ctr_Drbg_Init(&ctx, algorithmfamily);
    /*
     * Based on a NIST CTR_DRBG test vector (PR = True)
     */

    ctx.entropy_len    = CRYPTO_CONST_32;
    ctx.reseed_counter = CRYPTO_CONST_16;
    if (E_OK == ret)
    {
        ret = Crypto_ctr_drbg_seed(&ctx, Crypto_entropy_func, &entropy, key, keylength);
    }

    if (E_OK == ret)
    {
        ctx.prediction_resistance = 1U;
        ret                       = Crypto_ctr_drbg_random(&ctx, buf, outLen);
    }

    return ret;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

/* PRQA S 0488,0316,1252,1253,1290,1840,1842,2001,2015,3397,4436,2889,2784,2986,2995 -- */
/* PRQA S 2997,2889,2972,1503,1505,1532 -- */
