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
 **  @file               : Crypto_62_Random.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 0311,0316,0772,0780,1252,1253,1290,1842,1863,2001,2015,3120 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3326,3440,3672,3673,4116,4434,4461,4558,1843,1505,5022,5045 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040   ++ */                        /* VL_QAC_Crypto */
/* PRQA S 5014,5143,3332,2985,2889,5031,1532 ++ */                          /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"

#if (CRYPTO_SERVICE_RANDOM == STD_ON)
#include "stdlib.h"
#include "Crypto_62_Random.h"
#include "Crypto_62_CtrDrbg.h"
#include "Crypto_62_HmacDrbg.h"
/* ===================================================== macros ===================================================== */

#define CRYPTO_ENTROPY_BLOCK_SIZE 64 /**< Block size of entropy accumulator (SHA-512) */
#define CRYPTO_ENTROPY_MAX_LOOP   256

#define CRYPTO_ENTROPY_SOURCE_STRONG 1   /**< Entropy source is strong   */
#define CRYPTO_ENTROPY_SOURCE_WEAK   0   /**< Entropy source is weak     */
#define CRYPTO_ENTROPY_MAX_GATHER    128 /**< Maximum amount requested from entropy sources */
/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

CRYPTO_62_LOCAL Std_ReturnType Crypto_entropy_add_source(
    Crypto_entropy_context*     ctx,
    crypto_entropy_f_source_ptr f_source,
    void*                       p_source,
    uint32                      threshold,
    uint32                      strong);
CRYPTO_62_LOCAL Std_ReturnType Crypto_Entropy_Gather_Internal(Crypto_entropy_context* ctx);
CRYPTO_62_LOCAL Std_ReturnType
    Crypto_Entropy_Update(Crypto_entropy_context* ctx, uint8 source_id, const uint8* data, uint32 len);

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               Update the entropy in the accumulator.

 * Param-Name[in]      source_id: ID of entropy source.
 *                     data: entropy input.
 *                     len: The length of data.
 * Param-Name[in/out]  ctx: The context of entropy.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType
    Crypto_Entropy_Update(Crypto_entropy_context* ctx, uint8 source_id, const uint8* data, uint32 len)
{
    uint8          header[CRYPTO_CONST_2];
    uint8          tmp[CRYPTO_ENTROPY_BLOCK_SIZE];
    uint32         use_len = len;
    uint8*         p       = (uint8*)data;
    Std_ReturnType ret     = E_NOT_OK;
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
    if (use_len > CRYPTO_ENTROPY_BLOCK_SIZE)
    {
        (void)IStdLib_MemSet(&ctx->accumulator, 0, sizeof(Crypto_Sha512_Context));

        Crypto_Sha512_Starts(&ctx->accumulator, FALSE);

        if ((ret = Crypto_Sha512_Update(&ctx->accumulator, p, len)) != 0)
        {
            goto cleanup;
        }

        if ((ret = Crypto_Sha512_Finish(&ctx->accumulator, p)) != 0)
        {
            goto cleanup;
        }

        p       = tmp;
        use_len = CRYPTO_ENTROPY_BLOCK_SIZE;
    }

    header[0] = source_id;
    header[1] = use_len & 0xFFu;

    /*
     * Start the accumulator if this has not already happened. Note that
     * it is sufficient to start the accumulator here only because all calls to
     * gather entropy eventually execute this code.
     */
    if (ctx->accumulator_started == 0)
    {
        Crypto_Sha512_Starts(&ctx->accumulator, FALSE);

        ctx->accumulator_started = 1;
    }
    ret = Crypto_Sha512_Update(&ctx->accumulator, header, 2);
    if (E_OK != ret)
    {
        goto cleanup;
    }
    ret = Crypto_Sha512_Update(&ctx->accumulator, p, use_len);

cleanup:
#endif
    return ret;
}

/******************************************************************************/
/*
 * Brief               Gather entropy from different sources into accumulator.

 * Param-Name[in]      None
 * Param-Name[in/out]  ctx: The context of entropy.
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                           E_NOT_OK: State not accepted
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType Crypto_Entropy_Gather_Internal(Crypto_entropy_context* ctx)
{
    Std_ReturnType ret = E_NOT_OK;
    uint32         i;
    uint32         have_one_strong = 0;
    uint8          buf[CRYPTO_ENTROPY_MAX_GATHER];
    uint32         olen;

    if (ctx->source_count == 0)
    {
        return E_NOT_OK;
    }

    /* Run through our entropy sources */

    for (i = 0; i < ctx->source_count; i++)
    {
        if (ctx->source[i].strong == CRYPTO_ENTROPY_SOURCE_STRONG)
        {
            have_one_strong = 1;
        }

        olen = 0;
        if ((ret = ctx->source[i].f_source(ctx->source[i].p_source, buf, CRYPTO_ENTROPY_MAX_GATHER, &olen)) != 0)
        {
            goto cleanup;
        }

        /* Add if we actually gathered something */

        if (olen > 0)
        {
            if ((ret = Crypto_Entropy_Update(ctx, (uint8)i, buf, olen)) != 0)
            {
                return ret;
            }
            ctx->source[i].size += olen;
        }
    }

    if (have_one_strong == 0)
    {
        ret = E_NOT_OK;
    }

cleanup:

    return ret;
}

/******************************************************************************/
/*
 * Brief               Retrieve entropy from the accumulator.

 * Param-Name[in]      data: Entropy context.
                       len: Number of bytes desired.
 * Param-Name[in/out]  ctx: The context of entropy.
 * Param-Name[out]     output: Buffer to fill.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_entropy_func(void* data, uint8* output, uint32 len)
{
    Std_ReturnType          ret   = E_NOT_OK;
    uint32                  count = 0u, i, thresholds_reached;
    uint32                  strong_size;
    Crypto_entropy_context* ctx = (Crypto_entropy_context*)data;
    uint8                   buf[CRYPTO_ENTROPY_BLOCK_SIZE];
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
    if (len > CRYPTO_ENTROPY_BLOCK_SIZE)
    {
        return E_NOT_OK;
    }
    /* Always gather extra entropy before a call */
    do
    {
        if (count++ > CRYPTO_ENTROPY_MAX_LOOP)
        {
            ret = E_NOT_OK;
            goto exit;
        }

        if ((ret = Crypto_Entropy_Gather_Internal(ctx)) != 0)
        {
            goto exit;
        }

        thresholds_reached = 1;
        strong_size        = 0;
        for (i = 0; i < ctx->source_count; i++)
        {
            if (ctx->source[i].size < ctx->source[i].threshold)
            {
                thresholds_reached = 0;
            }
            if (ctx->source[i].strong == CRYPTO_ENTROPY_SOURCE_STRONG)
            {
                strong_size += ctx->source[i].size;
            }
        }
    } while ((!thresholds_reached) || (strong_size < CRYPTO_ENTROPY_BLOCK_SIZE));

    (void)IStdLib_MemSet(buf, 0, CRYPTO_ENTROPY_BLOCK_SIZE);

    /*
     * Note that at this stage it is assumed that the accumulator was started
     * in a previous call to Crypto_Entropy_Update(). If this is not guaranteed, the
     * code below will fail.
     */
    if ((ret = Crypto_Sha512_Finish(&ctx->accumulator, buf)) != 0)
    {
        goto exit;
    }

    /* Reset accumulator and counters and recycle existing entropy */

    (void)IStdLib_MemSet(&ctx->accumulator, 0, sizeof(Crypto_Sha512_Context));

    Crypto_Sha512_Starts(&ctx->accumulator, FALSE);

    if ((ret = Crypto_Sha512_Update(&ctx->accumulator, buf, len)) != 0)
    {
        goto exit;
    }

    /* Perform second hashing on entropy */
    (void)IStdLib_MemSet(&ctx->accumulator, 0, sizeof(Crypto_Sha512_Context));

    Crypto_Sha512_Starts(&ctx->accumulator, FALSE);

    if ((ret = Crypto_Sha512_Update(&ctx->accumulator, buf, len)) != 0)
    {
        goto exit;
    }

    if ((ret = Crypto_Sha512_Finish(&ctx->accumulator, buf)) != 0)
    {
        goto exit;
    }

    for (i = 0; i < ctx->source_count; i++)
    {
        ctx->source[i].size = 0;
    }

    (void)IStdLib_MemCpy(output, buf, len);

    ret = 0;

exit:
#endif
    return ret;
}
/******************************************************************************/
/*
 * Brief               This function initializes the entropy .

 * Param-Name[in]      None.
 * Param-Name[in/out]  ctx: The context of CTR_DRBG.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_entropy_init(Crypto_entropy_context* ctx)
{
#if (CRYPTO_ALGORITHMFAM_SHA2_512 == STD_ON)
    ctx->source_count = 0;
    (void)IStdLib_MemSet(ctx->source, 0, sizeof(ctx->source));

    ctx->accumulator_started = 0;

    (void)IStdLib_MemSet(&ctx->accumulator, 0, sizeof(ctx->accumulator));
    /* Reminder: Update ENTROPY_HAVE_STRONG in the test files
     *           when adding more strong entropy sources here. */

    (void)Crypto_entropy_add_source(ctx, Crypto_platform_entropy_poll, NULL_PTR, 32u, 1u);
#endif
}
/******************************************************************************/
/*
 * Brief               This function initializes the entropy poll .

 * Param-Name[in]      len. The length of entropy.
 * Param-Name[in/out]  data: The context of entropy.
 * Param-Name[out]     output: output of entropy.
 * Param-Name[out]     olen: output length.
 *
 * Return              None
 */
/******************************************************************************/
int Crypto_platform_entropy_poll(void* data, unsigned char* output, uint32 len, uint32* olen)
{
    ((void)data);
    uint8 i;

    /*
     * BCryptGenRandom takes ULONG for size, which is smaller than size_t on
     * 64-bit Windows platforms. Extract entropy in chunks of len (dependent
     * on ULONG_MAX) size.
     */
    for (i = 0; i < len; i++)
    {
        output[i] = rand();
        *olen += 1U;
    }

    return 0;
}
/******************************************************************************/
/*
 * Brief               This function add the entropy source .
 *
 * Param-Name[in]      f_source :Entropy function.
 *                      threshold: Minimum required from source before entropy is released
 *                       ( with mbedtls_entropy_func() ) (in bytes)
 *                     strong:   At least one strong source needs to be added.
 *                            Weaker sources (such as the cycle counter) can be used as
 *                             a complement.
 * Param-Name[in/out]  ctx: Entropy context.
 *                     p_source:  Function data
 * Param-Name[out]     output: output of entropy.
 * Param-Name[out]     olen: output length.
 *
 * Return              None
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType Crypto_entropy_add_source(
    Crypto_entropy_context*     ctx,
    crypto_entropy_f_source_ptr f_source,
    void*                       p_source,
    uint32                      threshold,
    uint32                      strong)
{
    uint32 idx, ret = E_OK;

    idx = ctx->source_count;
    if (idx >= 20U)
    {
        ret = E_NOT_OK;
        goto exit;
    }

    ctx->source[idx].f_source  = f_source;
    ctx->source[idx].p_source  = p_source;
    ctx->source[idx].threshold = threshold;
    ctx->source[idx].strong    = strong;

    ctx->source_count++;

exit:

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function process randomgenerate service with supported modes.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 *                     mode: The algorithm mode.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_62_RandomGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;
    if ((CRYPTO_OPERATIONMODE_START == operateMode) || (CRYPTO_OPERATIONMODE_UPDATE == operateMode))
    {
        result = E_OK;
    }
    else
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_CTRDRBG:
#if (CRYPTO_ALGORITHMMODE_CTRDRBG == STD_ON)
            result = Crypto_CtrDrbgProcess(objectId, algorithmfamily);
#endif
            break;
        case CRYPTO_ALGOMODE_GMAC:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_HMAC:
#if (CRYPTO_ALGORITHMMODE_HMAC == STD_ON)
            result = Crypto_HmacDrbgProcess(objectId, algorithmfamily);
#endif
            break;
        case CRYPTO_ALGOMODE_SIPHASH_2_4:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_SIPHASH_4_8:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_NOT_SET:
        case CRYPTO_ALGOMODE_CUSTOM:
            break;

        default:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }
    return result;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

/* PRQA S 0311,0316,0772,0780,1252,1253,1290,1842,1863,2001,2015,3120 -- */
/* PRQA S 3326,3440,3672,3673,4116,4434,4461,4558,1843,1505,5022,5045 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040 -- */
/* PRQA S 5014,5143,3332,2985,2889,5031,1532 -- */
