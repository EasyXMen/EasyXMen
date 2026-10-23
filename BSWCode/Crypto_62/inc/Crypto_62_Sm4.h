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
 **  @file               : Crypto_62_SM4.h
 **  @author             : lanyan.wang
 **  @date               : 2025/07/23
 **  @vendor             : isoft
 **  @description        : Internal implementation header for Crypto
 **
 ***********************************************************************************************************************/
/* PRQA S 1255,1281,3120,3132,3207,3218,3232,3678 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6060 EOF */                                   /* VL_MTR_Crypto_62_STM19 */
/* PRQA S 6040 EOF */                                   /* VL_MTR_Crypto_62_STPAR */
#ifndef CRYPTO_SM4_H
#define CRYPTO_SM4_H

#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_SM4 == STD_ON)

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */
#define CRYPTO_62_START_SEC_VAR_INIT_32
#include "Crypto_62_MemMap.h"
static uint32 FK[4] = {0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc};
#define CRYPTO_62_STOP_SEC_VAR_INIT_32
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_VAR_INIT_32
#include "Crypto_62_MemMap.h"
static uint32 CK[32] = {0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269, 0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
                        0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249, 0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
                        0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229, 0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
                        0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209, 0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279};
#define CRYPTO_62_STOP_SEC_VAR_INIT_32
#include "Crypto_62_MemMap.h"

// S盒
#define CRYPTO_62_START_SEC_VAR_INIT_32
#include "Crypto_62_MemMap.h"
static uint8 SBox[256] = {
    0xD6, 0x90, 0xE9, 0xFE, 0xCC, 0xE1, 0x3D, 0xB7, 0x16, 0xB6, 0x14, 0xC2, 0x28, 0xFB, 0x2C, 0x05, 0x2B, 0x67, 0x9A,
    0x76, 0x2A, 0xBE, 0x04, 0xC3, 0xAA, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99, 0x9C, 0x42, 0x50, 0xF4, 0x91, 0xEF,
    0x98, 0x7A, 0x33, 0x54, 0x0B, 0x43, 0xED, 0xCF, 0xAC, 0x62, 0xE4, 0xB3, 0x1C, 0xA9, 0xC9, 0x08, 0xE8, 0x95, 0x80,
    0xDF, 0x94, 0xFA, 0x75, 0x8F, 0x3F, 0xA6, 0x47, 0x07, 0xA7, 0xFC, 0xF3, 0x73, 0x17, 0xBA, 0x83, 0x59, 0x3C, 0x19,
    0xE6, 0x85, 0x4F, 0xA8, 0x68, 0x6B, 0x81, 0xB2, 0x71, 0x64, 0xDA, 0x8B, 0xF8, 0xEB, 0x0F, 0x4B, 0x70, 0x56, 0x9D,
    0x35, 0x1E, 0x24, 0x0E, 0x5E, 0x63, 0x58, 0xD1, 0xA2, 0x25, 0x22, 0x7C, 0x3B, 0x01, 0x21, 0x78, 0x87, 0xD4, 0x00,
    0x46, 0x57, 0x9F, 0xD3, 0x27, 0x52, 0x4C, 0x36, 0x02, 0xE7, 0xA0, 0xC4, 0xC8, 0x9E, 0xEA, 0xBF, 0x8A, 0xD2, 0x40,
    0xC7, 0x38, 0xB5, 0xA3, 0xF7, 0xF2, 0xCE, 0xF9, 0x61, 0x15, 0xA1, 0xE0, 0xAE, 0x5D, 0xA4, 0x9B, 0x34, 0x1A, 0x55,
    0xAD, 0x93, 0x32, 0x30, 0xF5, 0x8C, 0xB1, 0xE3, 0x1D, 0xF6, 0xE2, 0x2E, 0x82, 0x66, 0xCA, 0x60, 0xC0, 0x29, 0x23,
    0xAB, 0x0D, 0x53, 0x4E, 0x6F, 0xD5, 0xDB, 0x37, 0x45, 0xDE, 0xFD, 0x8E, 0x2F, 0x03, 0xFF, 0x6A, 0x72, 0x6D, 0x6C,
    0x5B, 0x51, 0x8D, 0x1B, 0xAF, 0x92, 0xBB, 0xDD, 0xBC, 0x7F, 0x11, 0xD9, 0x5C, 0x41, 0x1F, 0x10, 0x5A, 0xD8, 0x0A,
    0xC1, 0x31, 0x88, 0xA5, 0xCD, 0x7B, 0xBD, 0x2D, 0x74, 0xD0, 0x12, 0xB8, 0xE5, 0xB4, 0xB0, 0x89, 0x69, 0x97, 0x4A,
    0x0C, 0x96, 0x77, 0x7E, 0x65, 0xB9, 0xF1, 0x09, 0xC5, 0x6E, 0xC6, 0x84, 0x18, 0xF0, 0x7D, 0xEC, 0x3A, 0xDC, 0x4D,
    0x20, 0x79, 0xEE, 0x5F, 0x3E, 0xD7, 0xCB, 0x39, 0x48};
#define CRYPTO_62_STOP_SEC_VAR_INIT_32
#include "Crypto_62_MemMap.h"

#define ROL(x, y) ((x) << (y) | (x) >> (32 - (y)))

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/*密钥扩展*/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType SM4_KeyInit(uint8* key, uint32 rk[32]);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with OFB group mode
 *                     on input.
 *
 * Param-Name[in]      sm4key: The SM4_key of 32 rounds.
 *                     length: The length of input, calculated in byte.
 *                     iv_off: The offset in IV.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext corresponded with input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_SM4_Crypt_Ofb(
    uint32       roundKey[32],
    uint32       length,
    uint8*       iv_off,
    uint8        iv[CRYPTO_CONST_16],
    const uint8* input,
    uint8*       output,
    uint32*      outputLength);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with CFB group mode
 *                     on input.
 *
 * Param-Name[in]      sm4key: The SM4_key of 32 rounds.
 *                     mode: The SM4 operation: meaning do encryption or decryption.
 *                     length: The length of input, calculated in byte.
 *                     iv_off: The offset in IV.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_SM4_Crypt_Cfb(
    uint32       roundKey[32],
    uint8        mode,
    uint32       length,
    uint8*       iv_off,
    uint8        iv[CRYPTO_CONST_16],
    const uint8* input,
    uint8*       output,
    uint32*      outputLength);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with CTR group mode
 *                     on input.
 *
 * Param-Name[in]      sm4key: The SM4_key of 32 rounds.
 *                     mode: The AES operation: CRYPTO_MODE_DECRYPT or
 *                           CRYPTO_MODE_ENCRYPT.
 *                     length: The length of input, calculated in byte.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_SM4_Crypt_Ctr(
    uint32       roundKey[32],
    uint8        mode,
    uint32       length,
    uint8        iv[CRYPTO_CONST_16],
    const uint8* input,
    uint8*       output,
    uint32*      outputLength);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with CBC group mode
 *                     on input.
 *
 * Param-Name[in]      sm4key: The SM4_key of 32 rounds.
 *                     mode: The SM4 operation: CRYPTO_MODE_DECRYPT or
 *                           CRYPTO_MODE_ENCRYPT.
 *                     length: The length of input, calculated in byte.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_SM4_Crypt_Cbc(
    uint32       roundKey[32],
    uint8        mode,
    uint32       length,
    uint8        iv[CRYPTO_CONST_16],
    const uint8* input,
    uint8*       output,
    uint32*      outputLength);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with ECB group mode
 *                     on input.
 *
 * Param-Name[in]      sm4key: The SM4_key of 32 rounds.
 *                     mode: The SM4 operation: CRYPTO_MODE_DECRYPT or
 *                           CRYPTO_MODE_ENCRYPT.
 *                     length: The length of input, calculated in byte.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_SM4_Crypt_Ecb(
    uint32       roundKey[32],
    uint8        mode,
    uint32       length,
    const uint8* input,
    uint8*       output,
    uint32*      outputLength);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Inner SM4 encryption/decryption on a 128 bits block, so you
 *                     do not need to care about the group mode.
 *
 * Param-Name[in]      sm4_key: SM4 round key for encryption/decryption
 *                     input: Buffer filled with 128 bits plaintext.
 *                     enc_flag: flag of encrypt(0)/decrypt(1)
 * Param-Name[in/out]  None
 * Param-Name[out]     output: Buffer filled with ciphertext corresponded with
 *                             the input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Internal_SM4_Block_Crypt(uint32 roundKey[32], const uint8 input[16], uint8 output[16], uint8 enc_flag);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_ALGORITHMFAM_SM4 == STD_ON */

#endif /* CRYPTO_SM4_H */

/* PRQA S 1255,1281,3120,3132,3207,3218,3232,3678 -- */
