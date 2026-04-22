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
 **  @file               : Crypto_Des.h
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/

#ifndef CRYPTO_DES_H
#define CRYPTO_DES_H

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */
#define CRYPTO_DES_KEY_SIZE (0x08U)

/* ================================================ type definitions ================================================ */
/** @brief 3DES context structure. */
typedef struct
{
    uint32 sk[CRYPTO_CONST_96]; /*!<  3DES subkeys      */
} Crypto_Des3Data;
/** @brief DES context structure. */
typedef struct
{
    uint32 sk[CRYPTO_CONST_32]; /*!<  3DES subkeys      */
} Crypto_DesData;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
/**
 * @brief   Encrypts or decrypts a single block of data using 3DES in ECB mode.
 *
 * @param[in]     ctx     Pointer to the 3DES context structure.
 * @param[in]     input   Pointer to the input data block (8 bytes).
 * @param[out]    output  Pointer to the output data block (8 bytes).
 *
 * @return  None.
 */
void Crypto_3Des_Crypt_Ecb(Crypto_Des3Data* ctx, const uint8 input[CRYPTO_CONST_8], uint8 output[CRYPTO_CONST_8]);

/**
 * @brief   Sets the encryption key for DES.
 *
 * @param[in,out] ctx     Pointer to the DES context structure.
 * @param[in]     key     Pointer to the encryption key (8 bytes).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Des_Setkey_Enc(Crypto_DesData* ctx, const unsigned char key[CRYPTO_DES_KEY_SIZE]);

/**
 * @brief   Sets the decryption key for DES.
 *
 * @param[in,out] ctx     Pointer to the DES context structure.
 * @param[in]     key     Pointer to the decryption key (8 bytes).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Des_Setkey_Dec(Crypto_DesData* ctx, const unsigned char key[CRYPTO_CONST_8]);

/**
 * @brief   Sets the encryption and decryption keys for 3DES.
 *
 * @param[out] esk     Pointer to the encryption subkeys (96 words).
 * @param[out] dsk     Pointer to the decryption subkeys (96 words).
 * @param[in]  key     Pointer to the 3DES key (16 bytes).
 *
 * @return  None.
 */
void Crypto_Des3_Set2key(
    uint32              esk[CRYPTO_CONST_96],
    uint32              dsk[CRYPTO_CONST_96],
    const unsigned char key[CRYPTO_CONST_16]);

/**
 * @brief   Sets the decryption key for DES.
 *
 * @param[in,out] ctx     Pointer to the DES context structure.
 * @param[in]     key     Pointer to the decryption key (16 bytes).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Des3_Set2key_Dec(Crypto_Des3Data* ctx, const unsigned char key[CRYPTO_CONST_16]);

/**
 * @brief   Sets the encryption key for DES.
 *
 * @param[in,out] ctx     Pointer to the DES context structure.
 * @param[in]     key     Pointer to the encryption key (16 bytes).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Des3_Set2key_Enc(Crypto_Des3Data* ctx, const unsigned char key[CRYPTO_CONST_16]);

/**
 * @brief   Sets the encryption and decryption keys for 3DES.
 *
 * @param[out] esk     Pointer to the encryption subkeys (96 words).
 * @param[out] dsk     Pointer to the decryption subkeys (96 words).
 * @param[in]  key     Pointer to the 3DES key (24 bytes).
 *
 * @return  None.
 */
void Crypto_Des3_Set3key(
    uint32              esk[CRYPTO_CONST_96],
    uint32              dsk[CRYPTO_CONST_96],
    const unsigned char key[CRYPTO_CONST_24]);

/**
 * @brief   Sets the encryption keys for 3DES.
 *
 * @param[in,out] ctx     Pointer to the 3DES context structure.
 * @param[in]     key     Pointer to the 3DES key (24 bytes).
 *
 * @return  None.
 */
void Crypto_Des3_Set3key_Enc(Crypto_Des3Data* ctx, const unsigned char key[CRYPTO_CONST_8 * CRYPTO_CONST_3]);

/**
 * @brief   Sets the decryption keys for 3DES.
 *
 * @param[in,out] ctx     Pointer to the 3DES context structure.
 * @param[in]     key     Pointer to the 3DES key (24 bytes).
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_Des3_Set3key_Dec(Crypto_Des3Data* ctx, const unsigned char key[CRYPTO_CONST_8 * CRYPTO_CONST_3]);

/**
 * @brief   Encrypts or decrypts data using 3DES in CBC mode.
 *
 * @param[in]     ctx     Pointer to the 3DES context structure.
 * @param[in]     mode    Operation mode (encrypt/decrypt).
 * @param[in]     length  Length of the input data.
 * @param[in,out] iv      Initialization vector (8 bytes).
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output data.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType Crypto_3Des_Crypt_Cbc(
    Crypto_Des3Data* ctx,
    uint8            mode,
    uint32           length,
    uint8            iv[CRYPTO_CONST_8],
    const uint8*     input,
    uint8*           output,
    uint32*          outputLength);

/**
 * @brief   Computes the CMAC using 3DES.
 *
 * @param[in]     ctx     Pointer to the 3DES context structure.
 * @param[in]     mode    Operation mode (generate/verify).
 * @param[in]     length  Length of the input data.
 * @param[in]     input   Pointer to the input data.
 * @param[out]    output  Pointer to the output CMAC.
 *
 * @return  Status of the operation.
 *          - E_OK: Operation successful.
 *          - E_NOT_OK: Operation failed.
 */
Std_ReturnType
    Crypto_3Des_Crypt_Cmac(Crypto_Des3Data* ctx, uint8 mode, uint32 length, const uint8* input, uint8* output);

/**
 * @brief   Sets the encryption key for DES.
 *
 * @param[out] SK     Pointer to the DES subkeys (32 words).
 * @param[in]  key    Pointer to the encryption key (8 bytes).
 *
 * @return  None.
 */
void Crypto_Des_Setkey(uint32 SK[CRYPTO_CONST_32], const uint8 key[CRYPTO_DES_KEY_SIZE]);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_3DES == STD_ON */

#endif /* CRYPTO_DES_H */
