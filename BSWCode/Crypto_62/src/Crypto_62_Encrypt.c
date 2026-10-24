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
 **  @file               : Crypto_62_Encrypt.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 1252,1258,1290,2784,2822,2880,1505,1532 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,2995 ++ */ /* VL_QAC_Crypto */
/* PRQA S 3200,0316,2991,2889 ++ */                     /* VL_QAC_Crypto */
/* PRQA S 6020 EOF */                                   /* VL_MTR_Crypto_62_STLIN */

/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#include "Crypto_62_Aes.h"
#include "Crypto_62_Des.h"
#include "Crypto_62_Rsa.h"
#if (CRYPTO_ALGORITHMFAM_SM2 == STD_ON)
#include "Crypto_62_Sm2.h"
#endif
#if (CRYPTO_ALGORITHMFAM_SM4 == STD_ON)
#include "Crypto_62_Sm4.h"
#endif
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

#if (CRYPTO_SERVICE_ENCRYPT == STD_ON)
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    pkcs7_unpad(const uint8* data, uint32 data_len, uint32 block_size, uint32* unpadded_len, uint8* unpadded_data);
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * Brief               Adds PKCS7 padding to the input data
 *
 * Param-Name[in]      data Pointer to the padded data
 *                     data_len Length of the padded data
 *                     block_size The block size for the cipher
 *                     padded_len Output parameter for the length after padding
 *                     padded_data Output parameter after padding
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
/* PRQA S 3408 ++ */ /* VL_Csm_3408 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType pkcs7_pad(const uint8* data, uint32 data_len, uint32 block_size, uint32* padded_len, uint8* padded_data)
/* PRQA S 3408 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    // Calculate padding needed
    if (block_size != CRYPTO_CONST_0)
    {
        uint32 padding = block_size - (data_len % block_size);
        *padded_len    = data_len + padding;
        // Copy original data
        (void)IStdLib_MemCpy(padded_data, data, data_len);

        // Add padding bytes
        for (uint32 i = data_len; i < *padded_len; i++)
        {
            (padded_data)[i] = (uint8)padding;
        }

        ret = E_OK;
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Adds ONEWITHZEROS padding to the input data
 *
 * Param-Name[in]      data Pointer to the padded data
 *                     data_len Length of the padded data
 *                     block_size The block size for the cipher
 *                     padded_len Output parameter for the length after padding
 *                     padded_data Output parameter after padding
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
/* PRQA S 3408 ++ */ /* VL_Csm_3408 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    onewithZeros_pad(const uint8* data, uint32 data_len, uint32 block_size, uint32* padded_len, uint8* padded_data)
/* PRQA S 3408 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    /* Calculate padding needed */
    if (block_size > CRYPTO_CONST_0)
    {
        uint32 padding = block_size - (data_len % block_size);
        *padded_len    = data_len + padding;
        /* Copy original data */
        (void)IStdLib_MemCpy(padded_data, data, data_len);
        padded_data[data_len] = CRYPTO_CONST_80;
        /* Add padding bytes */
        for (uint32 i = data_len + CRYPTO_CONST_1; i < *padded_len; i++)
        {
            padded_data[i] = CRYPTO_CONST_0;
        }
        ret = E_OK;
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Removes PKCS7 padding from the input data.
 *
 * Param-Name[in]      data Pointer to the padded data
 *                     data_len Length of the padded data
 *                     block_size The block size for the cipher
 *                     unpadded_len Output parameter for the length after removing padding
 *                     unpadded_data Output parameter after unpadding
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    pkcs7_unpad(const uint8* data, uint32 data_len, uint32 block_size, uint32* unpadded_len, uint8* unpadded_data)
{
    Std_ReturnType ret = E_OK;
    /* Check if data length is multiple of block size */
    if ((block_size == CRYPTO_CONST_0) || ((data_len % block_size) != CRYPTO_CONST_0) || (data_len == CRYPTO_CONST_0))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    /* Get the padding value (last byte) */
    /* PRQA S 2852,2952 ++ */ /* VL_QAC_Crypto */
    uint32 padding = data[(data_len - CRYPTO_CONST_1)];
    /* PRQA S 2852,2952 -- */
    /* Validate padding */
    if ((padding == CRYPTO_CONST_0) || (padding > block_size))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    // Verify all padding bytes
    for (uint32 i = data_len - padding; i < data_len; i++)
    {
        if (data[i] != padding)
        {
            ret = E_NOT_OK;
            goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
        }
    }

    // Calculate unpadded length
    *unpadded_len = data_len - padding;
    (void)IStdLib_MemCpy(unpadded_data, data, *unpadded_len);

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               Removes ONEWITHZEROS padding from the input data.
 *
 * Param-Name[in]      data Pointer to the padded data
 *                     data_len Length of the padded data
 *                     block_size The block size for the cipher
 *                     padded_len Output parameter for the length after padding
 *                     padded_data Output parameter after padding
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
/* PRQA S 3408 ++ */ /* VL_Csm_3408 */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType onewithZeros_unpad(
    const uint8* data,
    uint32       data_len,
    uint32       block_size,
    uint32*      unpadded_len,
    uint8*       unpadded_data)
/* PRQA S 3408 -- */
{
    Std_ReturnType ret = E_OK;
    // Check if data length is multiple of block size
    if ((block_size == CRYPTO_CONST_0) || ((data_len % block_size) != CRYPTO_CONST_0) || (data_len == CRYPTO_CONST_0))
    {
        ret = E_NOT_OK;
    }

    if (ret == E_OK)
    {
        uint32 padded_len = CRYPTO_CONST_0;

        /* PRQA S 2990,1840 ++ */ /* VL_Crypto_62_2990 */
        for (uint32 i = CRYPTO_CONST_0; i < data_len; i++)
        {
            /* PRQA S 2004 ++ */ /* VL_Crypto_62_2004 */
            if (data[data_len - 1 - i] == CRYPTO_CONST_80)
            {
                padded_len += CRYPTO_CONST_1;
                break;
            }
            else if (data[data_len - 1 - i] == CRYPTO_CONST_0)
            {
                padded_len += CRYPTO_CONST_1;
            }
            /* PRQA S 2004 --*/
        }
        /* PRQA S 2990,1840 -- */

        *unpadded_len = data_len - padded_len;
        (void)IStdLib_MemCpy(unpadded_data, data, *unpadded_len);
    }

    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

#if (CRYPTO_SERVICE_ENCRYPT == STD_ON)
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
/******************************************************************************/
/*
 * Brief               This function do the AES Encrypt process .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                      mode: AlgorithmMode configured in crypto,
 *
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_AesEncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret          = E_NOT_OK;
    uint8          offset       = CRYPTO_CONST_0;
    const uint8*   inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint8*         outputPtr    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32         KeyLength    = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint32*        outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32         inputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint16         keyBit       = (uint16)(KeyLength * CRYPTO_CONST_8);
    uint32         cryptoKeyId  = Crypto_62_StoredJob[objectId].cryptoKeyId;
    Crypto_AESData ctx;
    uint32         ivLength     = CRYPTO_CONST_16;
    uint32         padded_len   = CRYPTO_CONST_0;
    uint32         tmpOutLen    = CRYPTO_CONST_0;
    uint32         maxCipherLen = inputLength + CRYPTO_CONST_16;
    uint8*         padded_data  = NULL_PTR;
    uint8*         tmpCipherPtr = NULL_PTR;
    uint8*         iv           = NULL_PTR;
    uint8*         key          = NULL_PTR;
    if ((inputPtr == NULL_PTR) || (outputPtr == NULL_PTR) || (outputLength == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    iv           = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_16);
    key          = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_32);
    padded_data  = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, maxCipherLen);
    tmpCipherPtr = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, maxCipherLen);
    if ((padded_data == NULL_PTR) || (tmpCipherPtr == NULL_PTR) || (iv == NULL_PTR) || (key == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_AESData));
    ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (ret == E_OK)
    {
        if ((mode == CRYPTO_ALGOMODE_CBC) || (mode == CRYPTO_ALGOMODE_CTR) || (mode == CRYPTO_ALGOMODE_CFB)
            || (mode == CRYPTO_ALGOMODE_OFB))
        {
            ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &ivLength);
        }
    }
    else
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if ((KeyLength == CRYPTO_CONST_16) && (ivLength == CRYPTO_CONST_16) && (ret == E_OK))
    {
        Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
    }
    else
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    /** padding */
    if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
    {
        switch (secondaryFam)
        {
        case CRYPTO_ALGOFAM_PADDING_PKCS7:
            ret = pkcs7_pad(inputPtr, inputLength, CRYPTO_CONST_16, &padded_len, padded_data);
            break;
        case CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS:
            ret = onewithZeros_pad(inputPtr, inputLength, CRYPTO_CONST_16, &padded_len, padded_data);
            break;
        case CRYPTO_ALGOFAM_NOT_SET:
            if ((inputLength % CRYPTO_CONST_16) == CRYPTO_CONST_0)
            {
                padded_len = inputLength;
                (void)IStdLib_MemCpy(padded_data, inputPtr, inputLength);
            }
            else
            {
                ret = E_NOT_OK;              // input length error
                goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
            }
            break;
        default:
            ret = E_NOT_OK;
            break; /* PRQA S 2001 */ /* VL_QAC_Crypto */
        }
    }

    /** ciphertext calculate */
    if (ret == E_OK)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            ret = Crypto_Aes_Crypt_Ecb(&ctx, CRYPTO_MODE_ENCRYPT, padded_len, padded_data, tmpCipherPtr);
            break;

        case CRYPTO_ALGOMODE_CBC:
            ret = Crypto_Aes_Crypt_Cbc(&ctx, CRYPTO_MODE_ENCRYPT, padded_len, iv, padded_data, tmpCipherPtr);
            break;

        case CRYPTO_ALGOMODE_CTR:
            ret = Crypto_Aes_Crypt_Ctr(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, iv, inputPtr, tmpCipherPtr);
            break;
        case CRYPTO_ALGOMODE_CFB:
            ret = Crypto_Aes_Crypt_Cfb128(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, &offset, iv, inputPtr, tmpCipherPtr);
            break;
        case CRYPTO_ALGOMODE_OFB:
            ret = Crypto_Aes_Crypt_Ofb(&ctx, inputLength, &offset, iv, inputPtr, tmpCipherPtr);
            break;
        case CRYPTO_ALGOMODE_XTS:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        default:
            /*do nothing*/
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }

    if (ret != E_OK)
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
    {
        if ((secondaryFam == CRYPTO_ALGOFAM_PADDING_PKCS7) || (secondaryFam == CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS))
        {
            tmpOutLen = padded_len;
        }
        else if (secondaryFam == CRYPTO_ALGOFAM_NOT_SET)
        {
            tmpOutLen = inputLength;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    else
    {
        tmpOutLen = inputLength;
    }

    if ((*outputLength >= tmpOutLen) && (E_OK == ret))
    {
        *outputLength = tmpOutLen;
    }
    else
    {
        /** buffer length not satisfied */
        ret = E_NOT_OK;
    }

    if (E_OK == ret) /* PRQA S 2982,2995 */ /* VL_QAC_Crypto */
    {
        (void)IStdLib_MemCpy(outputPtr, tmpCipherPtr, *outputLength);
    }

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    if ((ret == E_NOT_OK) && (outputLength != NULL_PTR) && (*outputLength > CRYPTO_CONST_0))
    {
        (void)IStdLib_MemSet(outputPtr, CRYPTO_CONST_0, *outputLength);
    }
    if (iv != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, iv);
    }
    if (key != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    }
    if (padded_data != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, padded_data);
    }
    if (tmpCipherPtr != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, tmpCipherPtr);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function do the AES Decrypt process .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_AesDecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret = E_NOT_OK;
    Crypto_AESData ctx;
    uint8          offset        = CRYPTO_CONST_0;
    uint32         cryptoKeyId   = Crypto_62_StoredJob[objectId].cryptoKeyId;
    uint32         KeyLength     = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint16         keyBit        = (uint16)(KeyLength * CRYPTO_CONST_8);
    const uint8*   inputPtr      = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint32         inputLength   = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint8*         outputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32*        outputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32         ivLength      = CRYPTO_CONST_16;
    uint32         unpadded_len  = CRYPTO_CONST_0;
    uint8*         tmpDecPtr     = NULL_PTR;
    uint8*         unpadded_data = NULL_PTR;
    uint8*         iv            = NULL_PTR;
    uint8*         key           = NULL_PTR;
    if ((inputPtr == NULL_PTR) || (outputPtr == NULL_PTR) || (outputLength == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    tmpDecPtr     = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, *outputLength);
    unpadded_data = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, inputLength);
    iv            = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_16);
    key           = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_32);
    if ((unpadded_data == NULL_PTR) || (tmpDecPtr == NULL_PTR) || (iv == NULL_PTR) || (key == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_AESData));
    ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (ret == E_OK)
    {
        if ((mode == CRYPTO_ALGOMODE_CBC) || (mode == CRYPTO_ALGOMODE_CTR) || (mode == CRYPTO_ALGOMODE_CFB)
            || (mode == CRYPTO_ALGOMODE_OFB))
        {
            ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &ivLength);
        }
    }
    else
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if ((KeyLength == CRYPTO_CONST_16) && (ivLength == CRYPTO_CONST_16) && (ret == E_OK))
    {
        if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
        {
            Crypto_Aes_Setkey_Dec(&ctx, key, keyBit);
        }
        else if ((mode == CRYPTO_ALGOMODE_CTR) || (mode == CRYPTO_ALGOMODE_CFB) || (mode == CRYPTO_ALGOMODE_OFB))
        {
            Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
        }
        else
        {
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
        }
    }
    else
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if (E_OK == ret)
    {
        /* PRQA S 2881 ++ */ /* VL_QAC_Crypto */
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            ret = Crypto_Aes_Crypt_Ecb(&ctx, CRYPTO_MODE_DECRYPT, inputLength, inputPtr, tmpDecPtr);
            break;
        case CRYPTO_ALGOMODE_CBC:
            ret = Crypto_Aes_Crypt_Cbc(&ctx, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, tmpDecPtr);
            break;
        case CRYPTO_ALGOMODE_CTR:
            ret = Crypto_Aes_Crypt_Ctr(&ctx, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, tmpDecPtr);
            break;
        case CRYPTO_ALGOMODE_CFB:
            ret = Crypto_Aes_Crypt_Cfb128(&ctx, CRYPTO_MODE_DECRYPT, inputLength, &offset, iv, inputPtr, tmpDecPtr);
            break;
        case CRYPTO_ALGOMODE_OFB:
            ret = Crypto_Aes_Crypt_Ofb(&ctx, inputLength, &offset, iv, inputPtr, tmpDecPtr);
            break;
        case CRYPTO_ALGOMODE_XTS:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        default:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
        /* PRQA S 2881 -- */
    }

    if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
    {
        if ((tmpDecPtr != NULL_PTR) && (E_OK == ret))
        {
            switch (secondaryFam)
            {
            case CRYPTO_ALGOFAM_PADDING_PKCS7:
                ret = pkcs7_unpad(tmpDecPtr, inputLength, CRYPTO_CONST_16, &unpadded_len, unpadded_data);
                break;
            case CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS:
                ret = onewithZeros_unpad(tmpDecPtr, inputLength, CRYPTO_CONST_16, &unpadded_len, unpadded_data);
                break;
            case CRYPTO_ALGOFAM_NOT_SET:
                unpadded_len = inputLength;
                (void)IStdLib_MemCpy(unpadded_data, tmpDecPtr, inputLength);
                break;
            default:
                ret = E_NOT_OK;
                break;
            }

            if ((*outputLength >= unpadded_len) && (ret == E_OK))
            {
                *outputLength = unpadded_len;
                (void)IStdLib_MemCpy(outputPtr, unpadded_data, *outputLength);
            }
            else
            {
                ret = E_NOT_OK;
            }
        }
    }
    else if ((mode == CRYPTO_ALGOMODE_CFB) || (mode == CRYPTO_ALGOMODE_OFB) || (mode == CRYPTO_ALGOMODE_CTR))
    {
        if ((*outputLength >= inputLength) && (tmpDecPtr != NULL_PTR) && (E_OK == ret))
        {
            *outputLength = inputLength;
            (void)IStdLib_MemCpy(outputPtr, tmpDecPtr, *outputLength);
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    else
    {
        ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
    }

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    if ((ret == E_NOT_OK) && (outputLength != NULL_PTR) && (*outputLength > CRYPTO_CONST_0))
    {
        (void)IStdLib_MemSet(outputPtr, CRYPTO_CONST_0, *outputLength);
    }
    if (iv != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, iv);
    }
    if (key != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    }
    if (unpadded_data != NULL_PTR) /* PRQA S 2961 */ /* VL_Crypto_62_2961 */
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, unpadded_data);
    }
    if (tmpDecPtr != NULL_PTR) /* PRQA S 2961 */ /* VL_Crypto_62_2961 */
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, tmpDecPtr);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif
/******************************************************************************/
/*
 * Brief               This function do the 3DES Encrypt process .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_3DesEncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType  ret = E_NOT_OK;
    Crypto_Des3Data ctx;
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Des3Data));
    const uint8* inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint8*       outputPtr    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32       inputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32*      outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32       KeyLength    = CRYPTO_CONST_24;
    uint32       ivLength     = CRYPTO_CONST_8;
    uint32       maxCipherLen = inputLength + CRYPTO_CONST_16;
    uint32       padded_len   = CRYPTO_CONST_0;
    uint8*       tmpCipherPtr = NULL_PTR;
    uint8*       key          = NULL_PTR;
    uint8*       padded_data  = NULL_PTR;
    uint8*       iv           = NULL_PTR;

    if ((inputPtr == NULL_PTR) || (outputPtr == NULL_PTR) || (outputLength == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    tmpCipherPtr = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, maxCipherLen);
    key          = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, KeyLength);
    padded_data  = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, maxCipherLen);
    iv           = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_16);
    if ((padded_data == NULL_PTR) || (tmpCipherPtr == NULL_PTR) || (key == NULL_PTR) || (iv == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (ret == E_OK)
    {
        if (mode == CRYPTO_ALGOMODE_CBC)
        {
            ret =
                Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &ivLength);
        }
    }
    else
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if (ret == E_OK)
    {
        switch (KeyLength)
        {
        case CRYPTO_CONST_16:
            ret = Crypto_Des3_Set2key_Enc(&ctx, key);
            break;
        case CRYPTO_CONST_24:
            Crypto_Des3_Set3key_Enc(&ctx, key);
            break;
        default:
            ret = E_NOT_OK;
            break; /* PRQA S 2001 */ /* VL_QAC_Crypto */
        }
    }

    if (ret == E_OK)
    {
        if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
        {
            switch (secondaryFam)
            {
            case CRYPTO_ALGOFAM_PADDING_PKCS7:
                ret = pkcs7_pad(inputPtr, inputLength, CRYPTO_CONST_8, &padded_len, padded_data);
                break;
            case CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS:
                ret = onewithZeros_pad(inputPtr, inputLength, CRYPTO_CONST_8, &padded_len, padded_data);
                break;
            case CRYPTO_ALGOFAM_NOT_SET:
                if ((inputLength % CRYPTO_CONST_8) == CRYPTO_CONST_0)
                {
                    padded_len = inputLength;
                    (void)IStdLib_MemCpy(padded_data, inputPtr, inputLength);
                }
                else
                {
                    ret = E_NOT_OK;              // input length error
                    goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
                }
                break;
            default:
                ret = E_NOT_OK;
                break;
            }
        }
    }
    else
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if (ret == E_OK)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            ret = Crypto_3Des_Crypt_Ecb(&ctx, CRYPTO_MODE_ENCRYPT, padded_len, padded_data, tmpCipherPtr);
            break;
        case CRYPTO_ALGOMODE_CBC:
            ret = Crypto_3Des_Crypt_Cbc(&ctx, CRYPTO_MODE_ENCRYPT, padded_len, iv, padded_data, tmpCipherPtr);
            break;
        default:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }

    if ((*outputLength >= padded_len) && (E_OK == ret))
    {
        *outputLength = padded_len;
    }
    else
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if ((tmpCipherPtr != NULL_PTR) && (E_OK == ret)) /* PRQA S 2982,2995 */ /* VL_QAC_Crypto */
    {
        (void)IStdLib_MemCpy(outputPtr, tmpCipherPtr, *outputLength);
    }

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    if ((ret == E_NOT_OK) && (outputLength != NULL_PTR) && (*outputLength > CRYPTO_CONST_0))
    {
        (void)IStdLib_MemSet(outputPtr, CRYPTO_CONST_0, *outputLength);
    }
    if (iv != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, iv);
    }
    if (key != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    }
    if (padded_data != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, padded_data);
    }
    if (tmpCipherPtr != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, tmpCipherPtr);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function do the 3DES Decrypt process .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 *
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_3DesDecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType  ret = E_NOT_OK;
    Crypto_Des3Data ctx;
    const uint8*    inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint8*          outputPtr    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32          inputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32*         outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32          KeyLength    = CRYPTO_CONST_24;
    uint32          ivLength     = CRYPTO_CONST_8;
    uint32          unpadded_len;
    uint8*          unpadded_data = NULL_PTR;
    uint8*          tmpDecPtr     = NULL_PTR;
    uint8*          key           = NULL_PTR;
    uint8*          iv            = NULL_PTR;

    if ((inputPtr == NULL_PTR) || (outputPtr == NULL_PTR) || (outputLength == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Des3Data));
    unpadded_data = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, inputLength);
    tmpDecPtr     = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, *outputLength);
    key           = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, KeyLength);
    iv            = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_16);
    if ((unpadded_data == NULL_PTR) || (tmpDecPtr == NULL_PTR) || (key == NULL_PTR) || (iv == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (ret == E_OK)
    {
        if (mode == CRYPTO_ALGOMODE_CBC)
        {
            ret =
                Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &ivLength);
        }
    }
    else
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    // key generation and set key
    if (ret == E_OK)
    {
        switch (KeyLength)
        {
        case CRYPTO_CONST_16:
            ret = Crypto_Des3_Set2key_Dec(&ctx, key);
            break;
        case CRYPTO_CONST_24:
            ret = Crypto_Des3_Set3key_Dec(&ctx, key);
            break;
        default:
            ret = E_NOT_OK;
            break;
        }
    }

    if (ret == E_OK)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            ret = Crypto_3Des_Crypt_Ecb(&ctx, CRYPTO_MODE_DECRYPT, inputLength, inputPtr, tmpDecPtr);
            break;
        case CRYPTO_ALGOMODE_CBC:
            ret = Crypto_3Des_Crypt_Cbc(&ctx, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, tmpDecPtr);
            break;
        default:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }

    if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
    {
        if ((tmpDecPtr != NULL_PTR) && (E_OK == ret))
        {
            switch (secondaryFam)
            {
            case CRYPTO_ALGOFAM_PADDING_PKCS7:
                ret = pkcs7_unpad(tmpDecPtr, inputLength, CRYPTO_CONST_8, &unpadded_len, unpadded_data);
                break;
            case CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS:
                ret = onewithZeros_unpad(tmpDecPtr, inputLength, CRYPTO_CONST_8, &unpadded_len, unpadded_data);
                break;
            case CRYPTO_ALGOFAM_NOT_SET:
                unpadded_len = inputLength;
                (void)IStdLib_MemCpy(unpadded_data, tmpDecPtr, inputLength);
                break;
            default:
                ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
                break;
            }

            if ((*outputLength >= inputLength) && (ret == E_OK))
            {
                *outputLength = unpadded_len;
                (void)IStdLib_MemCpy(outputPtr, unpadded_data, *outputLength);
            }
            else
            {
                ret = E_NOT_OK;
            }
        }
    }

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    if ((ret == E_NOT_OK) && (outputLength != NULL_PTR) && (*outputLength > CRYPTO_CONST_0))
    {
        (void)IStdLib_MemSet(outputPtr, CRYPTO_CONST_0, *outputLength);
    }
    if (iv != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, iv);
    }
    if (key != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    }
    if (unpadded_data != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, unpadded_data);
    }
    if (tmpDecPtr != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, tmpDecPtr);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

#if (CRYPTO_ALGORITHMFAM_SM4 == STD_ON)
/******************************************************************************/
/*
 * Brief               This function do the SM4 Encrypt process .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                      mode: AlgorithmMode configured in crypto,
 *
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_SM4_EncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret          = E_OK;
    uint8          offset       = CRYPTO_CONST_0;
    uint32         ivLength     = CRYPTO_CONST_16;
    uint8*         outputPtr    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32         KeyLength    = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    const uint8*   inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint32*        outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32         inputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32         cryptoKeyId  = Crypto_62_StoredJob[objectId].cryptoKeyId;
    uint32         roundKey[CRYPTO_CONST_32];
    uint32         padded_len   = CRYPTO_CONST_0;
    uint32         tmpOutLen    = CRYPTO_CONST_0;
    uint32         maxCipherLen = inputLength + CRYPTO_CONST_16;
    uint8*         iv           = NULL_PTR;
    uint8*         key          = NULL_PTR;

    if ((inputPtr == NULL_PTR) || (outputPtr == NULL_PTR) || (outputLength == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    iv                  = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_16);
    key                 = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_32);
    uint8* tmpCipherPtr = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, maxCipherLen);
    uint8* padded_data  = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, maxCipherLen);
    if ((padded_data == NULL_PTR) || (tmpCipherPtr == NULL_PTR) || (iv == NULL_PTR) || (key == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (ret == E_OK)
    {
        if ((mode == CRYPTO_ALGOMODE_CBC) || (mode == CRYPTO_ALGOMODE_CTR) || (mode == CRYPTO_ALGOMODE_CFB)
            || (mode == CRYPTO_ALGOMODE_OFB))
        {
            ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &ivLength);
        }
    }
    else
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if ((KeyLength == CRYPTO_CONST_16) && (ivLength == CRYPTO_CONST_16) && (ret == E_OK))
    {
        ret = SM4_KeyInit(key, roundKey);
    }
    else
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
    {
        switch (secondaryFam)
        {
        case CRYPTO_ALGOFAM_PADDING_PKCS7:
            ret = pkcs7_pad(inputPtr, inputLength, CRYPTO_CONST_16, &padded_len, padded_data);
            break;
        case CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS:
            ret = onewithZeros_pad(inputPtr, inputLength, CRYPTO_CONST_16, &padded_len, padded_data);
            break;
        case CRYPTO_ALGOFAM_NOT_SET:
            if ((inputLength % CRYPTO_CONST_16) == CRYPTO_CONST_0)
            {
                padded_len = inputLength;
                (void)IStdLib_MemCpy(padded_data, inputPtr, inputLength);
            }
            else
            {
                ret = E_NOT_OK;              // input length error
                goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
            }
            break;
        default:
            ret = E_NOT_OK;
            break;
        }
    }

    if (ret == E_OK)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            ret = Crypto_SM4_Crypt_Ecb(
                roundKey,
                CRYPTO_MODE_ENCRYPT,
                padded_len,
                padded_data,
                tmpCipherPtr,
                outputLength);
            break;

        case CRYPTO_ALGOMODE_CBC:
            ret = Crypto_SM4_Crypt_Cbc(
                roundKey,
                CRYPTO_MODE_ENCRYPT,
                padded_len,
                iv,
                padded_data,
                tmpCipherPtr,
                outputLength);
            break;

        case CRYPTO_ALGOMODE_CTR:
            ret = Crypto_SM4_Crypt_Ctr(
                roundKey,
                CRYPTO_MODE_ENCRYPT,
                inputLength,
                iv,
                inputPtr,
                tmpCipherPtr,
                outputLength);
            break;
        case CRYPTO_ALGOMODE_CFB:
            ret = Crypto_SM4_Crypt_Cfb(
                roundKey,
                CRYPTO_MODE_ENCRYPT,
                inputLength,
                &offset,
                iv,
                inputPtr,
                tmpCipherPtr,
                outputLength);
            break;
        case CRYPTO_ALGOMODE_OFB:
            ret = Crypto_SM4_Crypt_Ofb(roundKey, inputLength, &offset, iv, inputPtr, tmpCipherPtr, outputLength);
            break;
        default:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }

    if (ret != E_OK)
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
    {
        if ((secondaryFam == CRYPTO_ALGOFAM_PADDING_PKCS7) || (secondaryFam == CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS))
        {
            tmpOutLen = padded_len;
        }
        else if (secondaryFam == CRYPTO_ALGOFAM_NOT_SET)
        {
            tmpOutLen = inputLength;
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    else
    {
        tmpOutLen = inputLength;
    }

    if ((*outputLength >= tmpOutLen) && (ret == E_OK))
    {
        *outputLength = tmpOutLen;
    }
    else
    {
        /** buffer length not satisfied */
        ret = E_NOT_OK;
    }

    if (E_OK == ret) /* PRQA S 2982,2995 */ /* VL_QAC_Crypto */
    {
        (void)IStdLib_MemCpy(outputPtr, tmpCipherPtr, *outputLength);
    }
exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    if ((ret == E_NOT_OK) && (outputLength != NULL_PTR) && (*outputLength > CRYPTO_CONST_0))
    {
        (void)IStdLib_MemSet(outputPtr, CRYPTO_CONST_0, *outputLength);
    }
    if (iv != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, iv);
    }
    if (key != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    }
    if (padded_data != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, padded_data);
    }
    if (tmpCipherPtr != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, tmpCipherPtr);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/******************************************************************************/
/*
 * Brief               This function do the SM4 Decrypt process .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: AlgorithmMode configured in crypto,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType
    Crypto_SM4_DecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode, Crypto_AlgorithmFamilyType secondaryFam)
{
    Std_ReturnType ret      = E_NOT_OK;
    uint8          offset   = CRYPTO_CONST_0;
    uint32         ivLength = CRYPTO_CONST_16;
    uint32         roundKey[CRYPTO_CONST_32];
    uint32         cryptoKeyId  = Crypto_62_StoredJob[objectId].cryptoKeyId;
    const uint8*   inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint32         inputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint8*         outputPtr    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32*        decLength    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32         KeyLength    = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
    uint32*        outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32         unpadded_len = CRYPTO_CONST_0;
    uint8*         iv           = NULL_PTR;
    uint8*         key          = NULL_PTR;
    if ((inputPtr == NULL_PTR) || (outputPtr == NULL_PTR) || (outputLength == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    iv                   = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_16);
    key                  = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, CRYPTO_CONST_32);
    uint8* tmpDecPtr     = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, *outputLength);
    uint8* unpadded_data = (uint8*)IStdLib_MemHeapMalloc(Crypto_62_MemPool, inputLength);
    if ((unpadded_data == NULL_PTR) || (tmpDecPtr == NULL_PTR) || (iv == NULL_PTR) || (key == NULL_PTR))
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }
    ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (ret == E_OK)
    {
        if ((mode == CRYPTO_ALGOMODE_CBC) || (mode == CRYPTO_ALGOMODE_CTR) || (mode == CRYPTO_ALGOMODE_CFB)
            || (mode == CRYPTO_ALGOMODE_OFB))
        {
            ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &ivLength);
        }
    }
    else
    {
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if ((KeyLength == CRYPTO_CONST_16) && (ivLength == CRYPTO_CONST_16) && (ret == E_OK))
    {
        KeyLength = CRYPTO_CONST_16; /* PRQA S 2983 */ /* VL_QAC_Crypto */
        ret       = SM4_KeyInit(key, roundKey);
    }
    else
    {
        ret = E_NOT_OK;
        goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
    }

    if (E_OK == ret)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            ret = Crypto_SM4_Crypt_Ecb(roundKey, CRYPTO_MODE_DECRYPT, inputLength, inputPtr, tmpDecPtr, decLength);
            break;
        case CRYPTO_ALGOMODE_CBC:
            ret = Crypto_SM4_Crypt_Cbc(roundKey, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, tmpDecPtr, decLength);

            break;
        case CRYPTO_ALGOMODE_CTR:
            ret = Crypto_SM4_Crypt_Ctr(roundKey, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, tmpDecPtr, decLength);
            break;
        case CRYPTO_ALGOMODE_CFB:
            ret = Crypto_SM4_Crypt_Cfb(
                roundKey,
                CRYPTO_MODE_DECRYPT,
                inputLength,
                &offset,
                iv,
                inputPtr,
                tmpDecPtr,
                decLength);

            break;
        case CRYPTO_ALGOMODE_OFB:
            ret = Crypto_SM4_Crypt_Ofb(roundKey, inputLength, &offset, iv, inputPtr, tmpDecPtr, decLength);
            break;
        default:
            ret = E_NOT_OK;
            break;
        }
    }

    if (E_OK == ret)
    {
        if ((mode == CRYPTO_ALGOMODE_ECB) || (mode == CRYPTO_ALGOMODE_CBC))
        {
            if ((tmpDecPtr != NULL_PTR) && (E_OK == ret))
            {
                switch (secondaryFam)
                {
                case CRYPTO_ALGOFAM_PADDING_PKCS7:
                    ret = pkcs7_unpad(tmpDecPtr, inputLength, CRYPTO_CONST_16, &unpadded_len, unpadded_data);
                    break;
                case CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS:
                    ret = onewithZeros_unpad(tmpDecPtr, inputLength, CRYPTO_CONST_16, &unpadded_len, unpadded_data);
                    break;
                case CRYPTO_ALGOFAM_NOT_SET:
                    unpadded_len = inputLength;
                    (void)IStdLib_MemCpy(unpadded_data, tmpDecPtr, inputLength);
                    break;
                default:
                    ret = E_NOT_OK;
                    break;
                }

                if ((*outputLength >= unpadded_len) && (ret == E_OK))
                {
                    *outputLength = unpadded_len;
                    (void)IStdLib_MemCpy(outputPtr, unpadded_data, unpadded_len);
                }
                else
                {
                    ret = E_NOT_OK;
                }
            }
        }
        else if ((mode == CRYPTO_ALGOMODE_CFB) || (mode == CRYPTO_ALGOMODE_OFB) || (mode == CRYPTO_ALGOMODE_CTR))
        {
            if ((*outputLength >= inputLength) && (tmpDecPtr != NULL_PTR) && (E_OK == ret))
            {
                *outputLength = inputLength;
                (void)IStdLib_MemCpy(outputPtr, tmpDecPtr, inputLength);
            }
            else
            {
                ret = E_NOT_OK;
            }
        }
        else
        {
            ret = E_NOT_OK;
        }
    }

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    if ((ret == E_NOT_OK) && (outputLength != NULL_PTR) && (*outputLength > CRYPTO_CONST_0))
    {
        (void)IStdLib_MemSet(outputPtr, CRYPTO_CONST_0, *outputLength);
    }
    if (iv != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, iv);
    }
    if (key != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, key);
    }
    if (unpadded_data != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, unpadded_data);
    }
    if (tmpDecPtr != NULL_PTR)
    {
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, tmpDecPtr);
    }
    return ret;
}
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif

/******************************************************************************/
/*
 * Brief               This function do the ENCRYPT Service for CSM.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: algorithmMode configured in crypto,
 *                     algorithmfamily: familly configured in crypto,
 *                     operateMode: operateMode choose from modetype,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_62_Encrypt_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType             result = E_NOT_OK;
    Crypto_AlgorithmFamilyType secondaryFam =
        Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
    if ((CRYPTO_OPERATIONMODE_START == operateMode) || (CRYPTO_OPERATIONMODE_UPDATE == operateMode))
    {
        result = E_OK;
    }
    else
    {
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_3DES:
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
            result = Crypto_3DesEncryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_AES:
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            result = Crypto_AesEncryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_SM4:
#if (CRYPTO_ALGORITHMFAM_SM4 == STD_ON)
            result = Crypto_SM4_EncryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_CHACHA:

            break;
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
            result = Crypto_Rsa_EncryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_SM2:
#if (CRYPTO_ALGORITHMFAM_SM2 == STD_ON)
            result = Crypto_SM2_EncryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_CUSTOM:

            break;
        case CRYPTO_ALGOFAM_EEA3:

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
/******************************************************************************/
/*
 * Brief               This function do the DECRYPT Service for CSM.
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: algorithmMode configured in crypto,
 *                     algorithmfamily: familly configured in crypto,
 *                     operateMode: operateMode choose from modetype,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
Std_ReturnType Crypto_62_Decrypt_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;

    Crypto_AlgorithmFamilyType secondaryFam =
        Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
    if ((CRYPTO_OPERATIONMODE_START == operateMode) || (CRYPTO_OPERATIONMODE_UPDATE == operateMode))
    {
        result = E_OK;
    }
    else
    {
        switch (algorithmfamily)
        {
        case CRYPTO_ALGOFAM_3DES:
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
            result = Crypto_3DesDecryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_AES:
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            result = Crypto_AesDecryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_SM4:
#if (CRYPTO_ALGORITHMFAM_SM4 == STD_ON)
            result = Crypto_SM4_DecryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_CHACHA:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
            result = Crypto_Rsa_DecryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_SM2:
#if (CRYPTO_ALGORITHMFAM_SM2 == STD_ON)
            result = Crypto_SM2_DecryptProcess(objectId, mode, secondaryFam);
#endif
            break;
        case CRYPTO_ALGOFAM_CUSTOM:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOFAM_EEA3:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
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

/* PRQA S 1252,1258,1290,2784,2822,2880,1505,1532 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,2995 -- */
/* PRQA S 3200,0316,2991,2889 -- */
