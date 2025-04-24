#ifndef CTR_DRBG_H
#define CTR_DRBG_H
#if (CRYPTO_ALGORITHM_CTRG == STD_ON)
#include "Crypto_internal.h"
#include "aes.h"
#include "sha256.h"

#define CTR_DRBG_BLOCKSIZE       16
#define CTR_DRBG_KEYSIZE         16
#define CTR_DRBG_KEYBITS         128
#define CTR_DRBG_SEEDLEN         (CTR_DRBG_KEYSIZE + CTR_DRBG_BLOCKSIZE)
#define CTR_DRBG_RESEED_INTERVAL 10000
#define CTR_DRBG_MAX_INPUT       256
#define CTR_DRBG_MAX_REQUEST     1024
#define CTR_DRBG_MAX_SEED_INPUT  256
#define CTR_DRBG_ENTROPY_LEN     32

/** Critical entropy source failure. */
#define MBEDTLS_ERR_ENTROPY_SOURCE_FAILED 0x11
/** No more sources can be added. */
#define MBEDTLS_ERR_ENTROPY_MAX_SOURCES 0x22
/** No sources have been added to poll. */
#define MBEDTLS_ERR_ENTROPY_NO_SOURCES_DEFINED 0x33
/** No strong sources have been added to poll. */
#define MBEDTLS_ERR_ENTROPY_NO_STRONG_SOURCE 0x44
/** Read/write error in file. */
#define MBEDTLS_ERR_ENTROPY_FILE_IO_ERROR 0x55 /** Critical entropy source failure. */

/*#define INTERNALRAND*/

typedef struct Crypto_CtrDrbgData
{
    uint8 counter[16];         /*!< The counter (V). */
    int reseed_counter;        /*!< The reseed counter. */
    int prediction_resistance; /*!< whether to systematically reseed before each random generation. */
    uint32 entropy_len;        /*!< The amount of entropy grabbed on each seed or reseed operation. */
    int reseed_interval;       /*!< The reseed interval. */
    Crypto_AESData aes_ctx;    /*!< The AES context. */
    void* p_entropy;           /*!< The context for the entropy function. */
    Std_ReturnType (*f_entropy)(void*, P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA), uint32);
} Crypto_CtrDrbgData;

typedef int (*entropy_f_source_ptr)(void* data, unsigned char* output, uint32 len, uint32* olen);
/**
 * \brief           Entropy source state
 */
typedef struct
{
    entropy_f_source_ptr f_source; /**< The entropy source callback */
    void* p_source;                /**< The callback data pointer */
    uint32 size;                   /**< Amount received in bytes */
    uint32 threshold;              /**< Minimum bytes required before release */
    int strong;                    /**< Is the source strong? */
} Crypto_entropy_source_state;

/**
 * \brief           Entropy context structure
 */
typedef struct
{
    int accumulator_started;
    Crypto_Sha256Data accumulator;
    int source_count;
    Crypto_entropy_source_state source[20];

} Crypto_entropy_context;

FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_seed(
    P2VAR(Crypto_CtrDrbgData, AUTOMATIC, CRYPTO_APPL_DATA) ctx,
    Std_ReturnType (*f_entropy)(void*, uint8*, uint32),
    P2VAR(Crypto_entropy_context, AUTOMATIC, CRYPTO_APPL_DATA) p_entropy,
    const uint8* custom,
    uint32 len);
/* PRQA S 1336 */ /* MISRA Rule 8.2 */

FUNC(Std_ReturnType, CRYPTO_CODE)
mbedtls_ctr_drbg_random(void* p_rng, P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output, uint32 output_len);

FUNC(Std_ReturnType, CRYPTO_CODE) mbedtls_entropy_func(void* data, uint8* output, uint32 len);

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_CtrDrbg(uint8 key[16], uint8 buf[16]);

void mbedtls_ctr_drbg_init(Crypto_CtrDrbgData* ctx);
#endif /* CRYPTO_ALGORITHM_CTRG == STD_ON */
#endif /* ctr_drbg.h */
