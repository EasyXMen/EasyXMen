#ifndef SHA256_H
#define SHA256_H
#if (CRYPTO_ALGORITHM_HASH == STD_ON)
#include "Crypto_Types.h"

typedef struct
{
    uint32 total[2];  /*!< The number of Bytes processed.  */
    uint32 state[8];  /*!< The intermediate digest state.  */
    uint8 buffer[64]; /*!< The data block being processed. */
} Crypto_Sha256Data;
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_sha256_starts_ret(Crypto_Sha256Data* ctx);

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_sha256_update_ret(
    Crypto_Sha256Data* ctx,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) input,
    VAR(uint32, AUTOMATIC) ilen);

FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_sha256_finish_ret(Crypto_Sha256Data* ctx, VAR(uint8, AUTOMATIC) output[32]);

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_sha256(
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) input,
    VAR(uint32, AUTOMATIC) ilen,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output);

#endif /* CRYPTO_ALGORITHM_HASH == STD_ON */
#endif /* Crypto_sha256.h */
