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
 **  @file               : Crypto_62_Sm4.c
 **  @author             : Lanyan.wang
 **  @date               : 2025/07/23
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/
/* PRQA S 5209,0288,0488,0489,0499,0686,0780,1277,1290,1291,1330,1338 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1505,4559,4544,4542,4461,4397,3677,3671,3450,3440,3395,3387 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3344,3132,3120,3673,2052,1891,1863,1862,1860,1844,1843,1842 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1841,2986,2985,2889,2784,1840 ++ */                               /* VL_QAC_Crypto */
/* PRQA S 6060 EOF */                                                       /* VL_MTR_Crypto_62_STM19 */
/* PRQA S 6040 EOF */                                                       /* VL_MTR_Crypto_62_STPAR */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"

#if (CRYPTO_ALGORITHMFAM_SM4 == STD_ON)
#include "Crypto_62_Hash.h"
#include "Crypto_62_Sm4.h"

#include "Csm_Cfg.h"
#include "stdlib.h"
#include "string.h"

/* ========================================== internal function declarations ======================================== */
// 循环左移
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 rotate_left(uint32 x, uint32 n)
{
    return (x << n) | (x >> (32 - n));
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

// 非线性变换τ
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 tau(uint32 a)
{
    uint32 b = 0;
    b |= SBox[(a >> 24) & 0xFF] << 24;
    b |= SBox[(a >> 16) & 0xFF] << 16;
    b |= SBox[(a >> 8) & 0xFF] << 8;
    b |= SBox[a & 0xFF];
    return b;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

// 线性变换L
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 L(uint32 b)
{
    return b ^ rotate_left(b, 2) ^ rotate_left(b, 10) ^ rotate_left(b, 18) ^ rotate_left(b, 24);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

// 线性变换L'
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 L_prime(uint32 b)
{
    return b ^ rotate_left(b, 13) ^ rotate_left(b, 23);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

// 合成变换T
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 T(uint32 x)
{
    return L(tau(x));
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

// 合成变换T'
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 T_prime(uint32 x)
{
    return L_prime(tau(x));
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

// 轮函数F
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
static uint32 F(uint32 x0, uint32 x1, uint32 x2, uint32 x3, uint32 rk)
{
    return x0 ^ T(x1 ^ x2 ^ x3 ^ rk);
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType SM4_KeyInit(uint8* key, uint32 rk[32])
{
    uint32 MK[4];
    uint32 K[36];

    // 将密钥转换为4个字
    for (int i = 0; i < 4; i++)
    {
        MK[i] = ((uint32)key[4 * i] << 24) | ((uint32)key[4 * i + 1] << 16) | ((uint32)key[4 * i + 2] << 8)
                | ((uint32)key[4 * i + 3]);
    }

    // 生成中间密钥K
    for (int i = 0; i < 4; i++)
    {
        K[i] = MK[i] ^ FK[i];
    }

    // 生成轮密钥rk
    for (int i = 0; i < 32; i++)
    {
        K[i + 4] = K[i] ^ T_prime(K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ CK[i]);
        rk[i]    = K[i + 4];
    }
    return E_OK;
}
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
/* PRQA S 3673 ++ */ /* VL_QAC_Crypto */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Internal_SM4_Block_Crypt(uint32 roundKey[32], const uint8 input[16], uint8 output[16], uint8 enc_flag)
/* PRQA S 3673 -- */
{
    uint32 X[36];
    uint32 i, rk_index;

    // first 4 X
    for (i = 0; i < 4; i++)
    {
        X[i] = ((uint32)input[4 * i] << 24) | ((uint32)input[4 * i + 1] << 16) | ((uint32)input[4 * i + 2] << 8)
               | ((uint32)input[4 * i + 3]);
    }

    // 32round
    for (i = 0; i < 32; i++)
    {
        rk_index = enc_flag ? i : (31 - i);
        X[i + 4] = F(X[i], X[i + 1], X[i + 2], X[i + 3], roundKey[rk_index]);
    }

    // reverse
    for (i = 0; i < 4; i++)
    {
        output[4 * i]     = (X[35 - i] >> 24) & 0xFF;
        output[4 * i + 1] = (X[35 - i] >> 16) & 0xFF;
        output[4 * i + 2] = (X[35 - i] >> 8) & 0xFF;
        output[4 * i + 3] = X[35 - i] & 0xFF;
    }

    return E_OK;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with ECB group mode
 *                     on input.
 *
 * Param-Name[in]      roundKey: The SM4_key of 32 rounds.
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
    uint32*      outputLength)
{
    uint8          temp[CRYPTO_CONST_16];
    Std_ReturnType ret   = E_NOT_OK;
    uint32         ctLen = 0;

    if ((length % CRYPTO_CONST_16) != 0)
    {
        return ret;
    }

    if (mode == CRYPTO_MODE_DECRYPT)
    {
        while (length > 0)
        {
            (void)IStdLib_MemCpy(temp, input, CRYPTO_CONST_16);
            ret = Internal_SM4_Block_Crypt(roundKey, input, output, CRYPTO_MODE_DECRYPT);
            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
            ctLen += CRYPTO_CONST_16;
        }
    }
    else
    {
        while (length > 0)
        {
            ret = Internal_SM4_Block_Crypt(roundKey, input, output, CRYPTO_MODE_ENCRYPT);
            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
            ctLen += CRYPTO_CONST_16;
        }
    }

    // still modify scm data
    if (*outputLength >= ctLen)
    {
        *outputLength = ctLen;
    }
    else
    {
        ret = E_NOT_OK; // malloc memory not enough
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with CBC group mode
 *                     on input.
 *
 * Param-Name[in]      roundKey: The SM4_key of 32 rounds.
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
    uint32*      outputLength)
{
    uint8          i;
    uint8          temp[CRYPTO_CONST_16];
    Std_ReturnType ret   = E_NOT_OK;
    uint32         ctLen = 0;

    if (mode == CRYPTO_MODE_DECRYPT)
    {
        while (length > 0)
        {
            (void)IStdLib_MemCpy(temp, input, CRYPTO_CONST_16);
            ret = Internal_SM4_Block_Crypt(roundKey, input, output, CRYPTO_MODE_DECRYPT);
            /* PRQA S 2784 */ /* VL_QAC_Crypto */

            for (i = 0; i < CRYPTO_CONST_16; i++)
            {
                output[i] = (uint8)(output[i] ^ iv[i]);
            }

            (void)IStdLib_MemCpy(iv, temp, CRYPTO_CONST_16);

            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
            ctLen += CRYPTO_CONST_16;
        }
    }
    else
    {
        while (length > 0)
        {
            for (i = 0; i < CRYPTO_CONST_16; i++)
            {
                output[i] = (uint8)(input[i] ^ iv[i]);
            }
            ret = Internal_SM4_Block_Crypt(roundKey, output, output, CRYPTO_MODE_ENCRYPT);
            /* PRQA S 2784 */ /* VL_QAC_Crypto */

            (void)IStdLib_MemCpy(iv, output, CRYPTO_CONST_16);

            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
            ctLen += CRYPTO_CONST_16;
        }
    }

    // still modify scm data
    if (*outputLength >= ctLen)
    {
        *outputLength = ctLen;
    }
    else
    {
        ret = E_NOT_OK; // malloc memory not enough
    }
    return ret;
}
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
    uint32*      outputLength)
{
    uint8          i;
    uint8          j;
    Std_ReturnType ret = E_NOT_OK;
    // uint8          iv_Nonce[CRYPTO_CONST_8]   = {0x00};
    uint8  iv_Counter[CRYPTO_CONST_8] = {0x00};
    uint32 ctLen                      = 0;
    (void)mode;

    while (length > 0)
    {
        ret = Internal_SM4_Block_Crypt(roundKey, iv, output, CRYPTO_MODE_ENCRYPT); // dec similar to enc

        for (j = 0; j < CRYPTO_CONST_16; j++)
        {
            output[j] = input[j] ^ output[j];
        }

        // Counter ++
        for (i = 0; i < 8; i++)
        {
            iv_Counter[i] = (uint8)iv[i + 8];
        }
        CTR_Counter_Raise(iv_Counter);
        (void)IStdLib_MemCpy(iv + 8, iv_Counter, CRYPTO_CONST_8);

        input += CRYPTO_CONST_16;
        output += CRYPTO_CONST_16;
        length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        ctLen += CRYPTO_CONST_16;
    }

    // still modify scm data
    if (*outputLength >= ctLen)
    {
        *outputLength = ctLen;
    }
    else
    {
        ret = E_NOT_OK; // malloc memory not enough
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with CFB group mode
 *                     on input.
 *
 * Param-Name[in]      roundKey: The SM4_key of 32 rounds.
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
    uint32*      outputLength)
{
    uint8          c;
    Std_ReturnType ret = E_NOT_OK;
    uint8          n;
    uint32         ctLen = 0;

    n = *iv_off;

    if (n > CRYPTO_CONST_15)
    {
        return CRYPTO_E_KEY_NOT_VALID;
    }

    if (mode == CRYPTO_MODE_DECRYPT)
    {
        while (length--)
        {
            if (n == 0)
            {
                ret = Internal_SM4_Block_Crypt(roundKey, iv, iv, CRYPTO_MODE_ENCRYPT);

                if (ret != 0)
                {
                    goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
                }
            }

            c           = *(input++);
            *(output++) = (uint8)(c ^ iv[n]);
            iv[n]       = (uint8)c;

            n = ((n + CRYPTO_CONST_1) % CRYPTO_CONST_16) & CRYPTO_CONST_0x0F;
            ctLen++;
        }
    }
    else
    {
        while (length--)
        {
            if (n == 0)
            {
                ret = Internal_SM4_Block_Crypt(roundKey, iv, iv, CRYPTO_MODE_ENCRYPT);

                if (ret != 0)
                {
                    goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
                }
            }

            c           = *(input++);
            *(output++) = (uint8)(c ^ iv[n]);
            iv[n]       = (uint8) * (output - 1);

            n = ((n + CRYPTO_CONST_1) % CRYPTO_CONST_16) & CRYPTO_CONST_0x0F;
            ctLen++;
        }
    }

    *iv_off = n;
    // still modify scm data
    if (*outputLength >= ctLen)
    {
        *outputLength = ctLen;
        ret           = E_OK;
    }
    else
    {
        ret = E_NOT_OK; // malloc memory not enough
    }

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function is used to perform SM4 with OFB group mode
 *                     on input.
 *
 * Param-Name[in]      roundKey: The SM4_key of 32 rounds.
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
    uint32*      outputLength)
{
    Std_ReturnType ret = E_OK;
    uint8          n;
    uint32         ctLen = 0;

    n = *iv_off;

    if (n > CRYPTO_CONST_15)
    {
        return CRYPTO_E_KEY_NOT_VALID;
    }

    while (length--)
    {
        if (n == 0U)
        {
            ret = Internal_SM4_Block_Crypt(roundKey, iv, iv, CRYPTO_MODE_ENCRYPT);
            if (ret != E_OK)
            {
                goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
            }
        }
        *output++ = *input++ ^ iv[n];

        n = ((n + CRYPTO_CONST_1) % CRYPTO_CONST_16) & CRYPTO_CONST_0x0F;
        ctLen++;
    }

    *iv_off = n;

    // still modify scm data
    if (*outputLength >= ctLen)
    {
        *outputLength = ctLen;
    }
    else
    {
        ret = E_NOT_OK; // malloc memory not enough
    }

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

#endif

/* PRQA S 5209,0288,0488,0489,0499,0686,0780,1277,1290,1291,1330,1338 -- */
/* PRQA S 1505,4559,4544,4542,4461,4397,3677,3671,3450,3440,3395,3387 -- */
/* PRQA S 3344,3132,3120,3673,2052,1891,1863,1862,1860,1844,1843,1842 -- */
/* PRQA S 1841,2986,2985,2889,2784,1840 -- */
