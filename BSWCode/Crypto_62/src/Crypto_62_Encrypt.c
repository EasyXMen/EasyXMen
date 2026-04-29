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

/* PRQA S 6010 EOF */ /* VL_MTR_Crypto_62_STCYC */
/* PRQA S 6020 EOF */ /* VL_MTR_Crypto_62_STLIN */
/* PRQA S 6030 EOF */ /* VL_MTR_Crypto_62_STMIF */
/* PRQA S 6040 EOF */ /* VL_MTR_Crypto_62_STPAR */
/* PRQA S 6050 EOF */ /* VL_MTR_Crypto_62_STST3 */
/* PRQA S 6060 EOF */ /* VL_MTR_Crypto_62_STM19 */
/* PRQA S 6070 EOF */ /* VL_MTR_Crypto_62_STCAL */
/* PRQA S 6080 EOF */ /* VL_MTR_Crypto_62_STPTH */

/* PRQA S 0311,0314,0316,0488,1339,2016,2023,2024,2052,2105,3102,3103,3208,3326,3345 ++ */ /* VL_Crypto_62_General */
/* PRQA S 1253,0288,0317,0432,0489,0490,0693,0770,0772,0780,0842,1252,1258,1259,1277 ++ */ /* VL_Crypto_62_General */
/* PRQA S 1290,1291,1336,1338,1840,1820,1821,1823,1841,1842,1843,1844,1851,1853,1860 ++ */ /* VL_Crypto_62_General */
/* PRQA S 1861,1863,1880,1881,1890,1891,2001,2015,2205,2106,2140,2462,2463,2472,2487 ++ */ /* VL_Crypto_62_General */
/* PRQA S 2740,3101,3120,3122,3123,3140,3200,3206,3209,3218,3219,3226,3332,3344,3387 ++ */ /* VL_Crypto_62_General */
/* PRQA S 3395,3396,3397,3400,3408,3410,3418,3450,3440,3455,3610,3625,3672,3678,3715 ++ */ /* VL_Crypto_62_General */
/* PRQA S 3717,3762,3769,3772,4115,4116,4393,4394,4403,4404,4413,4414,4423,4432 ++ */      /* VL_Crypto_62_General */
/* PRQA S 4434,4443,4446,4447,4460,4461,4464,4470,4480,4491,4499,4501,4538,4542,4544 ++ */ /* VL_Crypto_62_General */
/* PRQA S 4532,4533,4543,4558,3673,3795,4150,4522,3391,3432,3442,2743,1505,2889,2834 ++ */ /* VL_Crypto_62_General */
/* PRQA S 2996,2992,2911,1503,2822,2896,2986 ++ */                                         /* VL_Crypto_62_General */
/* PRQA S 2834,2982,2983,1532,2986,2982,2983,5022,5143,5014,5031,5045,2839,2998 ++ */      /* VL_Crypto_62_General */
/* PRQA S 2995,2997,2871,2990,0597,2784,2880 ++ */                                         /* VL_Crypto_62_General */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#include "Crypto_62_Aes.h"
#include "Crypto_62_Des.h"
#include "Crypto_62_Rsa.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
CRYPTO_62_LOCAL Std_ReturnType Crypto_3DesEncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode);
CRYPTO_62_LOCAL Std_ReturnType Crypto_3DesDecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode);
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
Std_ReturnType Crypto_AesEncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode)
{
    uint8  iv[CRYPTO_CONST_16];
    uint8  key[CRYPTO_CONST_32];
    uint8  offset    = 0U;
    uint8* outputPtr = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32 KeyLength = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;

    uint32         inputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint16         keyBit      = (uint16)(KeyLength * CRYPTO_CONST_8);
    uint32         cryptoKeyId = Crypto_62_StoredJob[objectId].cryptoKeyId;
    Crypto_AESData ctx;

    Std_ReturnType ret          = E_NOT_OK;
    const uint8*   inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint32*        outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_AESData));
    ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (E_OK == ret)
    {
        Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            ret = Crypto_Aes_Crypt_Ecb(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, inputPtr, outputPtr, outputLength);
            break;

        case CRYPTO_ALGOMODE_CBC:
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret =
                    Crypto_Aes_Crypt_Cbc(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, iv, inputPtr, outputPtr, outputLength);
            }
            break;

        case CRYPTO_ALGOMODE_CTR:
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret           = Crypto_Aes_Crypt_Ctr(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, iv, inputPtr, outputPtr);
                *outputLength = inputLength;
            }
            break;

        case CRYPTO_ALGOMODE_CFB:
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Cfb128(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, &offset, iv, inputPtr, outputPtr);
                *outputLength = inputLength;
            }
            break;

        case CRYPTO_ALGOMODE_OFB:
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret           = Crypto_Aes_Crypt_Ofb(&ctx, inputLength, &offset, iv, inputPtr, outputPtr);
                *outputLength = inputLength;
            }
            break;

        case CRYPTO_ALGOMODE_XTS:
            ret = E_NOT_OK;
            break;

        default:
            /*do nothing*/
            break;
        }
    }
    if ((outputPtr != NULL_PTR) && (E_OK == ret)) /* PRQA S 2982,2995 */ /* VL_Crypto_62_General */
    {
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, outputPtr, *outputLength);
    }
    return ret;
}
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
Std_ReturnType Crypto_AesDecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode)
{
    uint8  iv[CRYPTO_CONST_16];
    uint8  key[CRYPTO_CONST_32];
    uint8  offset    = 0;
    uint8* outputPtr = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32 KeyLength = Crypto_62_StoredJob[objectId].jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;

    uint32         inputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint16         keyBit      = (uint16)(KeyLength * CRYPTO_CONST_8);
    uint32         cryptoKeyId = Crypto_62_StoredJob[objectId].cryptoKeyId;
    Crypto_AESData ctx;

    Std_ReturnType ret          = E_NOT_OK;
    const uint8*   inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint32*        outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_AESData));
    ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (E_OK == ret)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            Crypto_Aes_Setkey_Dec(&ctx, key, keyBit);
            ret = Crypto_Aes_Crypt_Ecb(&ctx, CRYPTO_MODE_DECRYPT, inputLength, inputPtr, outputPtr, outputLength);
            break;
        case CRYPTO_ALGOMODE_CBC:
            Crypto_Aes_Setkey_Dec(&ctx, key, keyBit);
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret =
                    Crypto_Aes_Crypt_Cbc(&ctx, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, outputPtr, outputLength);
            }
            break;
        case CRYPTO_ALGOMODE_CTR:
            Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret           = Crypto_Aes_Crypt_Ctr(&ctx, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, outputPtr);
                *outputLength = inputLength;
            }
            break;
        case CRYPTO_ALGOMODE_CFB:
            Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Cfb128(&ctx, CRYPTO_MODE_DECRYPT, inputLength, &offset, iv, inputPtr, outputPtr);
                *outputLength = inputLength;
            }
            break;
        case CRYPTO_ALGOMODE_OFB:
            Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret           = Crypto_Aes_Crypt_Ofb(&ctx, inputLength, &offset, iv, inputPtr, outputPtr);
                *outputLength = inputLength;
            }
            break;
        case CRYPTO_ALGOMODE_XTS:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        default:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }
    if ((outputPtr != NULL_PTR) && (E_OK == ret)) /* PRQA S 2982,2995 */ /* VL_Crypto_62_General */
    {
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, outputPtr, *outputLength);
    }
    return ret;
}
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
CRYPTO_62_LOCAL Std_ReturnType Crypto_3DesEncryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode)
{
    Std_ReturnType  ret = E_NOT_OK;
    Crypto_Des3Data ctx;
    uint8           iv[CRYPTO_CONST_8];
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Des3Data));
    const uint8* inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint8*       outputPtr    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32       inputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32*      outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32       KeyLength    = CRYPTO_CONST_24;
    uint32       ivLength     = CRYPTO_CONST_8;
    uint8*       key;
    key = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, KeyLength);
    uint8 temp[CRYPTO_CONST_8];
    *outputLength = 0;

    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    // key generation and set key
    if (ret == 0)
    {
        switch (KeyLength)
        {
        case 16:
            Crypto_Des3_Set2key_Enc(&ctx, key);
            break;
        case 24:
            Crypto_Des3_Set3key_Enc(&ctx, key);
            break;
        default:
            ret = E_NOT_OK;
            goto exit;
        }
    }

    if (ret == 0)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            while (inputLength > 0)
            {
                (void)IStdLib_MemCpy(temp, inputPtr, CRYPTO_CONST_8);
                Crypto_3Des_Crypt_Ecb(&ctx, temp, outputPtr);
                inputPtr += CRYPTO_CONST_8;
                outputPtr += CRYPTO_CONST_8;
                inputLength = (inputLength >= CRYPTO_CONST_8) ? (inputLength - CRYPTO_CONST_8) : 0;
                *outputLength += 8;
            }

            ret = E_OK;
            break;
        case CRYPTO_ALGOMODE_CBC:
            ret =
                Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &ivLength);
            if (E_OK == ret)
            {
                ret = Crypto_3Des_Crypt_Cbc(
                    &ctx,
                    CRYPTO_MODE_ENCRYPT,
                    inputLength,
                    iv,
                    inputPtr,
                    outputPtr,
                    outputLength);
            }
            break;
        default:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }
exit:
    return ret;
}
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
CRYPTO_62_LOCAL Std_ReturnType Crypto_3DesDecryptProcess(uint32 objectId, Crypto_AlgorithmModeType mode)
{
    Std_ReturnType  ret = E_NOT_OK;
    Crypto_Des3Data ctx;
    uint8           iv[CRYPTO_CONST_16];
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Des3Data));
    const uint8* inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint8*       outputPtr    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32       inputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32*      outputLength = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
    uint32       KeyLength    = CRYPTO_CONST_24;
    uint32       ivLength     = CRYPTO_CONST_8;
    uint8*       key;
    uint8        temp[CRYPTO_CONST_8];
    key           = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1u, KeyLength);
    *outputLength = 0;

    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    // key generation and set key
    if (ret == 0)
    {
        switch (KeyLength)
        {
        case 16:
            Crypto_Des3_Set2key_Dec(&ctx, key);
            break;
        case 24:
            Crypto_Des3_Set3key_Dec(&ctx, key);
            break;
        default:
            ret = E_NOT_OK;
            goto exit;
        }
    }

    if (ret == 0)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            while (inputLength > 0)
            {
                (void)IStdLib_MemCpy(temp, inputPtr, CRYPTO_CONST_8);
                Crypto_3Des_Crypt_Ecb(&ctx, temp, outputPtr);
                inputPtr += CRYPTO_CONST_8;
                outputPtr += CRYPTO_CONST_8;
                inputLength = (inputLength >= CRYPTO_CONST_8) ? (inputLength - CRYPTO_CONST_8) : 0;
                *outputLength += 8;
            }

            ret = E_OK;
            break;
        case CRYPTO_ALGOMODE_CBC:
            ret =
                Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &ivLength);
            if (E_OK == ret)
            {
                ret = Crypto_3Des_Crypt_Cbc(
                    &ctx,
                    CRYPTO_MODE_DECRYPT,
                    inputLength,
                    iv,
                    inputPtr,
                    outputPtr,
                    outputLength);
            }
            break;
        default:
            ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        }
    }

exit:
    return ret;
}
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
            result = Crypto_3DesEncryptProcess(objectId, mode);
#endif
            break;
        case CRYPTO_ALGOFAM_AES:
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            result = Crypto_AesEncryptProcess(objectId, mode);
#endif
            break;
        case CRYPTO_ALGOFAM_CHACHA:

            break;
        case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
            result = Crypto_Rsa_EncryptProcess(objectId, mode, secondaryFam);
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
            result = Crypto_3DesDecryptProcess(objectId, mode);
#endif
            break;
        case CRYPTO_ALGOFAM_AES:
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            result = Crypto_AesDecryptProcess(objectId, mode);
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

#endif

#if (CRYPTO_SERVICE_AEADENCRYPT == STD_ON)
/******************************************************************************/
/*
 * Brief               This function do the AEADENCRYPT Service for CSM.
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
Std_ReturnType Crypto_62_AeadEncrypt_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;
    (void)objectId;
    (void)mode;
    (void)operateMode;
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_3DES:
        result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    case CRYPTO_ALGOFAM_AES:
        result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    case CRYPTO_ALGOFAM_CHACHA:
        result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
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
    return result;
}
/******************************************************************************/
/*
 * Brief               This function do the AEADDECRYPT Service for CSM.
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
Std_ReturnType Crypto_62_AeadDecrypt_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;
    (void)objectId;
    (void)mode;
    (void)operateMode;
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_3DES:
        result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    case CRYPTO_ALGOFAM_AES:
        result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    case CRYPTO_ALGOFAM_CHACHA:
        result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
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
    return result;
}
#endif
#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/* PRQA S 2995,2997,2871,2990,0597,2784,2880 -- */                                         /* VL_Crypto_62_General */
/* PRQA S 2834,2982,2983,1532,2986,2982,2983,5022,5143,5014,5031,5045,2839,2998 -- */      /* VL_Crypto_62_General */
/* PRQA S 2996,2992,2911,1503,2822,2896,2986 -- */                                         /* VL_Crypto_62_General */
/* PRQA S 4532,4533,4543,4558,3673,3795,4150,4522,3391,3432,3442,2743,1505,2889,2834 -- */ /* VL_Crypto_62_General */
/* PRQA S 4434,4443,4446,4447,4460,4461,4464,4470,4480,4491,4499,4501,4538,4542,4544 -- */ /* VL_Crypto_62_General */
/* PRQA S 3717,3762,3769,3772,4115,4116,4393,4394,4403,4404,4413,4414,4423,4432 -- */      /* VL_Crypto_62_General */
/* PRQA S 3395,3396,3397,3400,3408,3410,3418,3450,3440,3455,3610,3625,3672,3678,3715 -- */ /* VL_Crypto_62_General */
/* PRQA S 2740,3101,3120,3122,3123,3140,3200,3206,3209,3218,3219,3226,3332,3344,3387 -- */ /* VL_Crypto_62_General */
/* PRQA S 1861,1863,1880,1881,1890,1891,2001,2015,2205,2106,2140,2462,2463,2472,2487 -- */ /* VL_Crypto_62_General */
/* PRQA S 1290,1291,1336,1338,1840,1820,1821,1823,1841,1842,1843,1844,1851,1853,1860 -- */ /* VL_Crypto_62_General */
/* PRQA S 1253,0288,0317,0432,0489,0490,0693,0770,0772,0780,0842,1252,1258,1259,1277 -- */ /* VL_Crypto_62_General */
/* PRQA S 0311,0314,0316,0488,1339,2016,2023,2024,2052,2105,3102,3103,3208,3326,3345 -- */ /* VL_Crypto_62_General */
