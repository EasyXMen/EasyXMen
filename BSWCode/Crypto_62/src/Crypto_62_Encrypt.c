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
 **  @file               : Crypto_62_Encrypt.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 1252,1258,1290,2784,2822,2880,1505,1532 ++ */ /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040   ++ */    /* VL_QAC_Crypto */

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
    uint32         outputLength = *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr);

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_AESData));
    ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (E_OK == ret)
    {
        Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            ret = Crypto_Aes_Crypt_Ecb(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, inputPtr, outputPtr);
            break;

        case CRYPTO_ALGOMODE_CBC:
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Cbc(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, iv, inputPtr, outputPtr);
            }
            break;

        case CRYPTO_ALGOMODE_CTR:
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Ctr(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, iv, inputPtr, outputPtr);
            }
            break;

        case CRYPTO_ALGOMODE_CFB:
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Cfb128(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, &offset, iv, inputPtr, outputPtr);
            }
            break;

        case CRYPTO_ALGOMODE_OFB:
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Ofb(&ctx, inputLength, &offset, iv, inputPtr, outputPtr);
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
    if ((outputPtr != NULL_PTR) && (E_OK == ret)) /* PRQA S 2982,2995 */ /* VL_QAC_Crypto */
    {
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, outputPtr, outputLength);
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
    uint32         outputLength = *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr);
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_AESData));
    ret = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_KEY, key, &KeyLength);
    if (E_OK == ret)
    {
        switch (mode)
        {
        case CRYPTO_ALGOMODE_ECB:
            Crypto_Aes_Setkey_Dec(&ctx, key, keyBit);
            ret = Crypto_Aes_Crypt_Ecb(&ctx, CRYPTO_MODE_DECRYPT, inputLength, inputPtr, outputPtr);
            break;
        case CRYPTO_ALGOMODE_CBC:
            Crypto_Aes_Setkey_Dec(&ctx, key, keyBit);
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Cbc(&ctx, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, outputPtr);
            }
            break;
        case CRYPTO_ALGOMODE_CTR:
            Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Ctr(&ctx, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, outputPtr);
            }
            break;
        case CRYPTO_ALGOMODE_CFB:
            Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Cfb128(&ctx, CRYPTO_MODE_DECRYPT, inputLength, &offset, iv, inputPtr, outputPtr);
            }
            break;
        case CRYPTO_ALGOMODE_OFB:
            Crypto_Aes_Setkey_Enc(&ctx, key, keyBit);
            KeyLength = CRYPTO_CONST_16;
            ret       = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
            if (ret == E_OK)
            {
                ret = Crypto_Aes_Crypt_Ofb(&ctx, inputLength, &offset, iv, inputPtr, outputPtr);
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
    if ((outputPtr != NULL_PTR) && (E_OK == ret)) /* PRQA S 2982,2995 */ /* VL_QAC_Crypto */
    {
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, outputPtr, outputLength);
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
    uint8           iv[CRYPTO_CONST_16];
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Des3Data));
    const uint8* inputPtr     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint8*       outputPtr    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    uint32       inputLength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32       outputLength = *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr);
    uint32       KeyLength    = CRYPTO_CONST_32;

    switch (mode)
    {
    case CRYPTO_ALGOMODE_ECB:
        Crypto_3Des_Crypt_Ecb(&ctx, inputPtr, outputPtr);
        ret = E_OK;
        break;
    case CRYPTO_ALGOMODE_CBC:
        ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
        if (E_OK == ret)
        {
            ret = Crypto_3Des_Crypt_Cbc(&ctx, CRYPTO_MODE_ENCRYPT, inputLength, iv, inputPtr, outputPtr);
        }

        break;
    default:
        ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    }
    if ((outputPtr != NULL_PTR) && (E_OK == ret)) /* PRQA S 2982,2995 */ /* VL_QAC_Crypto */
    {
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, outputPtr, outputLength);
    }
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
    uint32       outputLength = *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr);
    uint32       KeyLength    = CRYPTO_CONST_32;
    switch (mode)
    {
    case CRYPTO_ALGOMODE_ECB:
        Crypto_3Des_Crypt_Ecb(&ctx, inputPtr, outputPtr);
        ret = E_OK;
        break;
    case CRYPTO_ALGOMODE_CBC:
        ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_CIPHER_IV, iv, &KeyLength);
        if (E_OK == ret)
        {
            ret = Crypto_3Des_Crypt_Cbc(&ctx, CRYPTO_MODE_DECRYPT, inputLength, iv, inputPtr, outputPtr);
        }

        break;
    default:
        ret = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
        break;
    }
    if ((outputPtr != NULL_PTR) && (E_OK == ret)) /* PRQA S 2982,2995 */ /* VL_QAC_Crypto */
    {
        (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, outputPtr, outputLength);
    }
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

/* PRQA S 1252,1258,1290,2784,2822,2880,1505,1532 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040   -- */
