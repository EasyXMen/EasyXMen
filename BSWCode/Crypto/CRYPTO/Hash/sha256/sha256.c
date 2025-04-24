/* PRQA S 3432 ++ */                     /* MISRA Rule 20.7 */
/* PRQA S 0488 ++ */                     /* MISRA Rule 18.4 */
/* PRQA S 2212 ++ */                     /* MISRA Rule 15.6 */
/* PRQA S 3326,3226 ++ */                /* MISRA Rule 13.4 */
/* PRQA S 1863,1843,1842,1841,1840 ++ */ /* MISRA Rule 10.4 */
/* PRQA S 4558,4542 ++ */                /* MISRA Rule 10.1 */
/* PRQA S 3218 ++ */                     /* MISRA Dir 8.9 */
/* PRQA S 1281 ++ */                     /* MISRA Rule 7.2 */
/* PRQA S 2985 ++ */                     /* MISRA Rule 2.2 */

#include "sha256.h"
#include "Crypto_internal.h"
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"
#if (CRYPTO_ALGORITHM_HASH == STD_ON)
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_internal_sha256_process(Crypto_Sha256Data* ctx, VAR(uint8, AUTOMATIC) data[64]);

/*
 * 32-bit integer manipulation macros(big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n, b, i)                                                                        \
    do                                                                                                \
    {                                                                                                 \
        (n) = ((VAR(uint32, AUTOMATIC))(b)[(i)] << 24) | ((VAR(uint32, AUTOMATIC))(b)[(i) + 1] << 16) \
              | ((VAR(uint32, AUTOMATIC))(b)[(i) + 2] << 8) | ((VAR(uint32, AUTOMATIC))(b)[(i) + 3]); \
    } while (0)
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(x, b, i)                        \
    do                                                \
    {                                                 \
        (b)[(i)] = ((uint8)(((x) >> 24) & 0xff));     \
        (b)[(i) + 1] = ((uint8)(((x) >> 16) & 0xff)); \
        (b)[(i) + 2] = ((uint8)(((x) >> 8) & 0xff));  \
        (b)[(i) + 3] = ((uint8)(((x) >> 0) & 0xff));  \
    } while (0)
#endif

/*
 * SHA-256 context setup
 */
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_sha256_starts_ret(Crypto_Sha256Data* ctx)
{
    Std_ReturnType ret = E_OK;

    ctx->total[0] = 0;
    ctx->total[1] = 0;

    /* SHA-256 */
    ctx->state[0] = 0x6A09E667;
    ctx->state[1] = 0xBB67AE85;
    ctx->state[2] = 0x3C6EF372;
    ctx->state[3] = 0xA54FF53A;
    ctx->state[4] = 0x510E527F;
    ctx->state[5] = 0x9B05688C;
    ctx->state[6] = 0x1F83D9AB;
    ctx->state[7] = 0x5BE0CD19;

    return (ret);
}

static VAR(uint32, AUTOMATIC) K[] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
};

#define SHR(x, n)   (((x) & 0xFFFFFFFF) >> (n))
#define ROTR(x, n)  (SHR(x, n) | ((x) << (32 - (n))))

#define S0(x)       (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define S1(x)       (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

#define S2(x)       (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S3(x)       (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define F0(x, y, z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

#define R(t)        (local.W[t] = S1(local.W[(t)-2]) + local.W[(t)-7] + S0(local.W[(t)-15]) + local.W[(t)-16])

#define P(a, b, c, d, e, f, g, h, x, K)                            \
    do                                                             \
    {                                                              \
        local.temp1 = (h) + S3(e) + F1((e), (f), (g)) + (K) + (x); \
        local.temp2 = S2(a) + F0((a), (b), (c));                   \
        (d) += local.temp1;                                        \
        (h) = local.temp1 + local.temp2;                           \
    } while (0)

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_internal_sha256_process(Crypto_Sha256Data* ctx, VAR(uint8, AUTOMATIC) data[64])
{
    struct
    {
        uint32 temp1, temp2, W[64];
        uint32 A[8];
    } local;
    uint32 i;
    Std_ReturnType ret = E_OK;

    for (i = 0; i < 8; i++)
    {
        local.A[i] = ctx->state[i];
    }

    for (i = 0; i < 16; i++)
    {
        GET_UINT32_BE(local.W[i], data, 4 * i);
    }

    for (i = 0; i < 16; i += 8)
    {
        P(local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.W[i + 0],
          K[i + 0]);
        P(local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.W[i + 1],
          K[i + 1]);
        P(local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.W[i + 2],
          K[i + 2]);
        P(local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.W[i + 3],
          K[i + 3]);
        P(local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.W[i + 4],
          K[i + 4]);
        P(local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.W[i + 5],
          K[i + 5]);
        P(local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.W[i + 6],
          K[i + 6]);
        P(local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.W[i + 7],
          K[i + 7]);
    }

    for (i = 16; i < 64; i += 8)
    {
        P(local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          R(i + 0),
          K[i + 0]);
        P(local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          R(i + 1),
          K[i + 1]);
        P(local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          R(i + 2),
          K[i + 2]);
        P(local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          R(i + 3),
          K[i + 3]);
        P(local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          local.A[3],
          R(i + 4),
          K[i + 4]);
        P(local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          local.A[2],
          R(i + 5),
          K[i + 5]);
        P(local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          local.A[1],
          R(i + 6),
          K[i + 6]);
        P(local.A[1],
          local.A[2],
          local.A[3],
          local.A[4],
          local.A[5],
          local.A[6],
          local.A[7],
          local.A[0],
          R(i + 7),
          K[i + 7]);
    }

    for (i = 0; i < 8; i++)
        ctx->state[i] += local.A[i];

    Crypto_memset(&local, sizeof(local));
    return (ret);
}

/*
 * SHA-256 process buffer
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_sha256_update_ret(
    Crypto_Sha256Data* ctx,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) input,
    VAR(uint32, AUTOMATIC) ilen)
{
    Std_ReturnType ret = E_OK;
    uint32 fill;
    uint32 left;

    if (ilen == 0)
    {
        goto exit;
    }

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (VAR(uint32, AUTOMATIC))ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < ilen)
    {
        ctx->total[1]++;
    }

    if (left && ilen >= fill) /* PRQA S 3397 */ /* MISRA Rule 12.1 */
    {
        Crypto_memcpy((void*)(ctx->buffer + left), input, fill);

        if ((ret = Crypto_internal_sha256_process(ctx, ctx->buffer)) != 0)
        {
            goto exit;
        }

        input += fill;
        ilen -= fill;
        left = 0;
    }

    while (ilen >= 64)
    {
        if ((ret = Crypto_internal_sha256_process(ctx, input)) != 0) /* PRQA S 2784 */ /* MISRA Rule 17.5 */
        {
            goto exit;
        }

        input += 64;
        ilen -= 64;
    }

    if (ilen > 0)
        Crypto_memcpy((void*)(ctx->buffer + left), input, ilen);

exit:
    return (ret);
}

/*
 * SHA-256 final digest
 */
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_sha256_finish_ret(Crypto_Sha256Data* ctx, VAR(uint8, AUTOMATIC) output[32])
{
    uint32 used;
    uint32 high, low;
    Std_ReturnType ret;
    /*
     * Add padding: 0x80 then 0x00 until 8 bytes remain for the length
     */
    used = ctx->total[0] & 0x3F;

    ctx->buffer[used++] = 0x80; /* PRQA S 3440  */ /* MISRA Rule 13.3 */

    if (used <= 56)
    {
        /* Enough room for padding + length in current block */
        Crypto_memset((ctx->buffer + used), (56 - used));
    }
    else
    {
        /* We'll need an extra block */
        Crypto_memset((ctx->buffer + used), (64 - used));
        if ((ret = Crypto_internal_sha256_process(ctx, ctx->buffer)) != 0)
        {
            goto exit;
        }
        Crypto_memset((ctx->buffer), 56);
    }

    /*
     * Add message length
     */
    high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    low = (ctx->total[0] << 3);

    PUT_UINT32_BE(high, ctx->buffer, 56);
    PUT_UINT32_BE(low, ctx->buffer, 60);

    if ((ret = Crypto_internal_sha256_process(ctx, ctx->buffer)) != 0)
    {
        goto exit;
    }
    /*
     * Output final state
     */
    PUT_UINT32_BE(ctx->state[0], output, 0);
    PUT_UINT32_BE(ctx->state[1], output, 4);
    PUT_UINT32_BE(ctx->state[2], output, 8);
    PUT_UINT32_BE(ctx->state[3], output, 12);
    PUT_UINT32_BE(ctx->state[4], output, 16);
    PUT_UINT32_BE(ctx->state[5], output, 20);
    PUT_UINT32_BE(ctx->state[6], output, 24);
    PUT_UINT32_BE(ctx->state[7], output, 28);
exit:
    return (ret);
}

/*
 * output = SHA-256(input buffer)
 */
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_sha256(
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) input,
    VAR(uint32, AUTOMATIC) ilen,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output)
{
    Std_ReturnType ret;
    Crypto_Sha256Data ctx;

    Crypto_memset(&ctx, sizeof(Crypto_Sha256Data));
    ret = Crypto_sha256_starts_ret(&ctx);

    if (E_NOT_OK != ret)
    {
        ret = Crypto_sha256_update_ret(&ctx, input, ilen);
        if (E_NOT_OK != ret)
        {
            ret = Crypto_sha256_finish_ret(&ctx, output); /* PRQA S 2784 */ /* MISRA Rule 17.5 */
        }
    }
    return ret;
}
#endif /* CRYPTO_ALGORITHM_HASH == STD_ON */
#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

/* PRQA S 3432 -- */                     /* MISRA Rule 20.7 */
/* PRQA S 0488 -- */                     /* MISRA Rule 18.4 */
/* PRQA S 2212 -- */                     /* MISRA Rule 15.6 */
/* PRQA S 3326,3226 -- */                /* MISRA Rule 13.4 */
/* PRQA S 1863,1843,1842,1841,1840 -- */ /* MISRA Rule 10.4 */
/* PRQA S 4558,4542 -- */                /* MISRA Rule 10.1 */
/* PRQA S 3218 -- */                     /* MISRA Dir 8.9 */
/* PRQA S 1281 -- */                     /* MISRA Rule 7.2 */
/* PRQA S 2985 -- */                     /* MISRA Rule 2.2 */
