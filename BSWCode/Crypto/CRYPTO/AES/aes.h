#ifndef AES_H
#define AES_H
#if (CRYPTO_ALGORITHM_AES == STD_ON)
#include "Crypto_Types.h"

typedef struct
{
    int nr;         /*!< The number of rounds. */
    uint32* rk;     /*!< AES round keys. */
    uint32 buf[68]; /*!< Unaligned data buffer. This buffer can
                           hold 32 extra Bytes,which can be used for
                           one of the following purposes:
                           <ul><li>Alignment if VIA padlock is
                                   used.</li>
                           <li>Simplifying key expansion in the 256-bit
                               case by generating an extra round key.
                               </li></ul> */
} Crypto_AESData;

#define AES_ENCRYPT 1 /**< AES encryption. */
#define AES_DECRYPT 0 /**< AES decryption. */

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_aes_setkey_enc(Crypto_AESData* ctx, P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key, uint16 keybits);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_aes_setkey_dec(Crypto_AESData* ctx, P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key, uint16 keybits);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_aes_crypt_ecb(Crypto_AESData* ctx, uint8 mode, const uint8 input[16], uint8 output[16]);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_aes_crypt_cbc(
    Crypto_AESData* ctx,
    uint8 mode,
    uint8 length,
    uint8 iv[16],
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) input,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AESEncryptECB(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key, uint8 buf[64]);
FUNC(Std_ReturnType, CRYPTO_CODE) Crypto_AESDecryptECB(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key, uint8 buf[64]);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_AESEncryptCBC(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key, uint8 buf[64], uint8 iv[16], uint8 inputlength);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_AESDecryptCBC(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key, uint8 buf[16], uint8 iv[16], uint8 inputlength);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_AESEncryptCTR(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key, uint8 buf[64], uint8 iv[16]);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_AESDecryptCTR(P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key, uint8 buf[64], uint8 iv[16]);
FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_aes_crypt_cbc_CMAC(
    Crypto_AESData* ctx,
    uint8 mode,
    uint8 length,
    uint8 iv[16],
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) input,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) output);
#endif /* CRYPTO_ALGORITHM_AES == STD_ON */
#endif /* aes.h */
