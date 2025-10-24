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
 **  @file               : Crypto_62_Aes.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_AES_H
#define CRYPTO_AES_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
#include "Crypto_62.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
/** @brief AES context*/
typedef struct
{
    uint8   nr;                   /*!< The number of rounds. */
    uint32* rk;                   /*!< AES round keys. */
    uint32  buf[CRYPTO_CONST_68]; /*!< Unaligned data buffer. This buffer can hold 32 extra Bytes,which can be used for
                        one of the  following purposes: <ul><li>Alignment if VIA padlock is used.</li> <li>Simplifying
                        key  expansion  in the 256-bit case by generating an extra round key.</li></ul> */
} Crypto_AESData;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Sets the encryption key for AES.
 *
 * @param[in] ctx     Pointer to the AES context structure.
 * @param[in] key     Pointer to the encryption key.
 * @param[in] keybits Length of the encryption key in bits.
 *
 * @return  None.
 */
void Crypto_Aes_Setkey_Enc(Crypto_AESData* ctx, uint8* key, uint16 keybits);

/**
 * @brief   Sets the decryption key for AES.
 *
 * @param[in] ctx     Pointer to the AES context structure.
 * @param[in] key     Pointer to the decryption key.
 * @param[in] keybits Length of the decryption key in bits.
 *
 * @return  None.
 */
void Crypto_Aes_Setkey_Dec(Crypto_AESData* ctx, uint8* key, uint16 keybits);

/**
 * @brief   Encrypts or decrypts data using AES in ECB mode.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     mode    Operation mode (encrypt/decrypt).
 * @param[in]     length  Length of the input data.
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Aes_Crypt_Ecb(Crypto_AESData* ctx, uint8 mode, uint32 length, const uint8* input, uint8* output);

/**
 * @brief   Encrypts or decrypts data using AES in CTR mode.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     mode    Operation mode (encrypt/decrypt).
 * @param[in]     length  Length of the input data.
 * @param[in]     iv      The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes. CTR: Nonce || Counter
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Aes_Crypt_Ctr(
    Crypto_AESData* ctx,
    uint8           mode,
    uint32          length,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output);

/**
 * @brief   Encrypts or decrypts data using AES in CBC mode.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     mode    Operation mode (encrypt/decrypt).
 * @param[in]     length  Length of the input data.
 * @param[in,out] iv      Initialization vector.
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Aes_Crypt_Cbc(
    Crypto_AESData* ctx,
    uint8           mode,
    uint32          length,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output);

/**
 * @brief   Encrypts or decrypts data using AES in CFB128 mode.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     mode    Operation mode (encrypt/decrypt).
 * @param[in]     length  Length of the input data.
 * @param[in,out] iv_off  Offset in the initialization vector.
 * @param[in,out] iv      Initialization vector.
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Aes_Crypt_Cfb128(
    Crypto_AESData* ctx,
    uint8           mode,
    uint32          length,
    uint8*          iv_off,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output);

/**
 * @brief   Encrypts or decrypts data using AES in OFB mode.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     length  Length of the input data.
 * @param[in,out] iv_off  Offset in the initialization vector.
 * @param[in,out] iv      Initialization vector.
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Aes_Crypt_Ofb(
    Crypto_AESData* ctx,
    uint32          length,
    uint8*          iv_off,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output);

/**
 * @brief   Encrypts or decrypts data using AES in CFB8 mode.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     mode    Operation mode (encrypt/decrypt).
 * @param[in]     length  Length of the input data.
 * @param[in,out] iv      Initialization vector.
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Aes_Crypt_Cfb8(
    Crypto_AESData* ctx,
    uint8           mode,
    uint32          length,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output);

/**
 * @brief   Computes the CMAC using AES.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     mode    Operation mode (generate/verify).
 * @param[in]     length  Length of the input data.
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output CMAC.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Aes_Crypt_Cmac(Crypto_AESData* ctx, uint8 mode, uint8 length, uint8* input, uint8* output);

/**
 * @brief   Encrypts a single block of data using AES.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     input   Pointer to the input data block (16 bytes).
 * @param[out]    output  Pointer to the output data block (16 bytes).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType
    Internal_Aes_Encrypt(Crypto_AESData* ctx, const uint8 input[CRYPTO_CONST_16], uint8 output[CRYPTO_CONST_16]);

/**
 * @brief   Decrypts a single block of data using AES.
 *
 * @param[in]     ctx     Pointer to the AES context structure.
 * @param[in]     input   Pointer to the input data block (16 bytes).
 * @param[out]    output  Pointer to the output data block (16 bytes).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType
    Internal_Aes_Decrypt(Crypto_AESData* ctx, const uint8 input[CRYPTO_CONST_16], uint8 output[CRYPTO_CONST_16]);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_AES == STD_ON */

#endif /* CRYPTO_AES_H*/
