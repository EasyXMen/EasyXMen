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
 **  @file               : Crypto_62_Mac.c
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

/* PRQA S 0316,0488,0686,1253,1258,1277,1290,1338,1840,1843,1852,1860,1862,1863 ++ */ /* VL_Crypto_62_General */
/* PRQA S 2001,2015,3120,3200,3326,3395,3396,3397,3400,3673,3678,3793,4397,4434 ++ */ /* VL_Crypto_62_General */
/* PRQA S 1505,2982,1505,1503,1532,2889,4446,4461,4544 ++ */                          /* VL_Crypto_62_General */

/* =================================================== inclusions =================================================== */

#include "Crypto_62_Internal.h"
#if (CRYPTO_SERVICE_MAC == STD_ON)
#include "Crypto_62_Aes.h"
#include "Crypto_62_Des.h"
#include "Crypto_62_Hash.h"
#include "Crypto_62_Mac.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
#if (CRYPTO_ALGORITHMMODE_CMAC == STD_ON)
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
CRYPTO_62_LOCAL Std_ReturnType Crypto_Generate_Subkey_Aes(Crypto_AESData* aes, uint8* key, uint8* k1, uint8* k2);
CRYPTO_62_LOCAL void           Crypto_GenerateCmac_Aes(
              Crypto_AESData* aes,
              uint32          len,
              const uint8*    add,
              uint8*          macvalue,
              uint8*          Key,
              uint8*          k1,
              uint8*          k2);
#endif
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
CRYPTO_62_LOCAL void Crypto_GenerateCmac_3Des(
    Crypto_Des3Data* Ctx,
    uint32           len,
    const uint8*     add,
    uint8*           macvalue,
    uint8*           Key,
    uint8*           k1,
    uint8*           k2);
CRYPTO_62_LOCAL Std_ReturnType Crypto_Generate_Subkey_Des3(Crypto_Des3Data* ctx, uint8* key, uint8* k1, uint8* k2);
#endif
#endif
/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */

/******************************************************************************/
/*
 * Brief               This function do left shift for add and des .
 *
 *
 * Param-Name[in]      len: lenth of the data input.
 *                     add: the data to be left shifted.
 * Param-Name[in/out]  None.
 * Param-Name[out]     des:the data after left shift.
 * Return              None
 */
/******************************************************************************/
void Crypto_Leftshift(uint32 len, uint8* add, uint8* des)
{
    uint32 i;
    for (i = 0; i < len - 1; i++)
    {
        des[i] = (add[i] << 1) + (add[i + 1] >= 0x80 ? 1 : 0);
    }
    des[len - 1] = add[len - 1] << 1;
}
#if (CRYPTO_ALGORITHMMODE_CMAC == STD_ON)
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
/******************************************************************************/
/*
 * Brief               This function generate subkey k1,k2 for cmac-aes.
 *
 *
 * Param-Name[in]      aes: aes context .
 *                     key: the key to be used to generate subkey.
 *                     k1: the first subkey.
 *                     k2: the second subkey.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
CRYPTO_62_LOCAL Std_ReturnType Crypto_Generate_Subkey_Aes(Crypto_AESData* aes, uint8* key, uint8* k1, uint8* k2)
{
    Std_ReturnType ret = E_NOT_OK;

    uint8 c0[CRYPTO_CONST_16];
    uint8 plain[CRYPTO_CONST_16] = {
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U};
    uint8 Rb[CRYPTO_CONST_16] = {
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x87U};
    (void)Crypto_Aes_Setkey_Enc(aes, key, 128);

    ret = Crypto_Aes_Crypt_Cmac(aes, CRYPTO_MODE_ENCRYPT, CRYPTO_CONST_16, plain, c0);
    if (c0[0] < 0x80) // generate k1
    {
        Crypto_Leftshift(16, c0, k1);
    }
    else
    {
        Crypto_Leftshift(16, c0, k1);
        Crypto_Xor(16, k1, Rb, k1);
    }

    if (k1[0] < 0x80) // generate k2
    {
        Crypto_Leftshift(16, k1, k2);
    }
    else
    {
        Crypto_Leftshift(16, k1, k2);
        Crypto_Xor(16, k2, Rb, k2);
    }

    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               This function generate subkey k1,k2 for cmac-3des.
 *
 *
 * Param-Name[in]      aes: 3des context .
 *                     key: the key to be used to generate subkey.
 *                     k1: the first subkey.
 *                     k2: the second subkey.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
CRYPTO_62_LOCAL Std_ReturnType Crypto_Generate_Subkey_Des3(Crypto_Des3Data* ctx, uint8* key, uint8* k1, uint8* k2)
{
    Std_ReturnType ret;
    uint8          c0[CRYPTO_CONST_16];
    uint8          plain[CRYPTO_CONST_8] = {
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
    };
    uint8 Rb[CRYPTO_CONST_8] = {

        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x1BU};

    Crypto_Des3_Set3key_Enc(ctx, key);

    ret = Crypto_3Des_Crypt_Cmac(ctx, CRYPTO_MODE_ENCRYPT, 8, plain, c0);
    if (c0[0] < 0x80) // generate k1
    {
        Crypto_Leftshift(8, c0, k1);
    }
    else
    {
        Crypto_Leftshift(8, c0, k1);
        Crypto_Xor(8, k1, Rb, k1);
    }

    if (k1[0] < 0x80) // generate k2
    {
        Crypto_Leftshift(8, k1, k2);
    }
    else
    {
        Crypto_Leftshift(8, k1, k2);
        Crypto_Xor(8, k2, Rb, k2);
    }
    return ret;
}
#endif
/******************************************************************************/
/*
 * Brief               This function generate cmac-aes.
 *
 *
 * Param-Name[in]      aes: aes context.
 *                     len: input data length.
 *                     add: input data.
 *
 *                     key: the key to be used to generate mac value.
 *                     k1: the first subkey.
 *                     k2: the second subkey.
 * Param-Name[in/out]  None.
 * Param-Name[out]     macvalue: output mac value.
 * Return              None
 */
/******************************************************************************/
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
CRYPTO_62_LOCAL void Crypto_GenerateCmac_Aes(
    Crypto_AESData* aes,
    uint32          len,
    const uint8*    add,
    uint8*          macvalue,
    uint8*          Key,
    uint8*          k1,
    uint8*          k2)
{
    int   i, block;
    uint8 Result[CRYPTO_CONST_128];
    uint8 Blocktemp[CRYPTO_CONST_128];
    (void)IStdLib_MemSet(Result, 0, 128);
    (void)IStdLib_MemSet(Blocktemp, 0, 128);

    if (len % 16 == 0 && len != 0)
    {
        Crypto_Aes_Setkey_Enc(aes, Key, 128); // load mac key
        block = len / 16;
        for (i = 0; i < block - 1; i++)
        {
            Crypto_Xor(16, &add[i * 16], (const uint8*)Result, Blocktemp);
            (void)Crypto_Aes_Crypt_Cmac(aes, CRYPTO_MODE_ENCRYPT, 16, Blocktemp, Result);
        }
        Crypto_Xor(16, &add[(block - 1) * 16], (const uint8*)Result, Blocktemp);
        Crypto_Xor(16, Blocktemp, k1, Blocktemp);
        (void)Crypto_Aes_Crypt_Cmac(aes, CRYPTO_MODE_ENCRYPT, 16, Blocktemp, macvalue);
    }
    else
    {
        if (len == 0)
        {
            Crypto_Aes_Setkey_Enc(aes, Key, 128); // load mac key
            block        = 1; /* PRQA S 2983  */  /* VL_Crypto_62_General */
            Blocktemp[0] = 0x80;                  // padding the first bit with 1
            Crypto_Xor(16, Blocktemp, k2, Blocktemp);
            (void)Crypto_Aes_Crypt_Cmac(aes, CRYPTO_MODE_ENCRYPT, 16, Blocktemp, macvalue);
        }
        else
        {
            Crypto_Aes_Setkey_Enc(aes, Key, 128); // load mac key
            uint8 remain = len % 16;
            block        = len / 16 + 1;
            for (i = 0; i < block - 1; i++)
            {
                Crypto_Xor(16, &add[i * 16], (const uint8*)Result, Blocktemp);
                (void)Crypto_Aes_Crypt_Cmac(aes, CRYPTO_MODE_ENCRYPT, 16, Blocktemp, Result);
            }
            // the last block padding
            for (i = 0; i < remain; i++)
            {
                Blocktemp[i] = add[(block - 1) * 16 + i];
            }
            Blocktemp[remain] = 0x80;
            for (i = remain + 1; i < 16; i++)
            {
                Blocktemp[i] = 0;
            }
            // end of the last block padding

            Crypto_Xor(16, Blocktemp, k2, Blocktemp);
            Crypto_Xor(16, Blocktemp, Result, Blocktemp);
            (void)Crypto_Aes_Crypt_Cmac(aes, CRYPTO_MODE_ENCRYPT, 16, Blocktemp, macvalue);
        }
    }
}
#endif
/******************************************************************************/
/*
 * Brief               This function generate cmac-3des.
 *
 *
 * Param-Name[in]      aes: 3des context.
 *                     len: input data length.
 *                     add: input data.
 *
 *                     key: the key to be used to generate mac value.
 *                     k1: the first subkey.
 *                     k2: the second subkey.
 * Param-Name[in/out]  None.
 * Param-Name[out]     macvalue: output mac value.
 * Return              None
 */
/******************************************************************************/
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
CRYPTO_62_LOCAL void Crypto_GenerateCmac_3Des(
    Crypto_Des3Data* Ctx,
    uint32           len,
    const uint8*     add,
    uint8*           macvalue,
    uint8*           Key,
    uint8*           k1,
    uint8*           k2)
{
    int   i, block;
    uint8 Result[CRYPTO_CONST_128];
    uint8 Blocktemp[CRYPTO_CONST_128];
    (void)IStdLib_MemSet(Result, 0, 128);
    (void)IStdLib_MemSet(Blocktemp, 0, 128);

    if (len % 8 == 0 && len != 0)
    {
        Crypto_Des3_Set3key_Enc(Ctx, (const uint8*)Key); // load mac key
        block = len / 8;
        for (i = 0; i < block - 1; i++)
        {
            Crypto_Xor(8, &add[i * 8], (const uint8*)Result, Blocktemp);
            (void)Crypto_3Des_Crypt_Cmac(Ctx, CRYPTO_MODE_ENCRYPT, 8U, Blocktemp, Result);
        }
        Crypto_Xor(8, &add[(block - 1) * 8], (const uint8*)Result, Blocktemp);
        Crypto_Xor(8, Blocktemp, k1, Blocktemp);
        (void)Crypto_3Des_Crypt_Cmac(Ctx, CRYPTO_MODE_ENCRYPT, 8U, Blocktemp, macvalue);
    }
    else
    {
        if (len == 0)
        {
            Crypto_Des3_Set3key_Enc(Ctx, Key);   // load mac key
            block        = 1; /* PRQA S 2983  */ /* VL_Crypto_62_General */
            Blocktemp[0] = 0x80;                 // padding the first bit with 1
            Crypto_Xor(8, Blocktemp, k2, Blocktemp);
            (void)Crypto_3Des_Crypt_Cmac(Ctx, CRYPTO_MODE_ENCRYPT, 8U, Blocktemp, macvalue);
        }
        else
        {
            Crypto_Des3_Set3key_Enc(Ctx, Key); // load mac key
            uint8 remain = len % 8;
            block        = len / 8 + 1;
            for (i = 0; i < block - 1; i++)
            {
                Crypto_Xor(8, &add[i * 8], (const uint8*)Result, Blocktemp);
                (void)Crypto_3Des_Crypt_Cmac(Ctx, CRYPTO_MODE_ENCRYPT, 8U, Blocktemp, Result);
            }
            // the last block padding
            for (i = 0; i < remain; i++)
            {
                Blocktemp[i] = add[(block - 1) * 8 + i];
            }
            Blocktemp[remain] = 0x80;
            for (i = remain + 1; i < 8; i++)
            {
                Blocktemp[i] = 0;
            }
            // end of the last block padding

            Crypto_Xor(8, Blocktemp, k2, Blocktemp);
            Crypto_Xor(8, Blocktemp, Result, Blocktemp);
            (void)Crypto_3Des_Crypt_Cmac(Ctx, CRYPTO_MODE_ENCRYPT, 8U, Blocktemp, macvalue);
        }
    }
}
#endif
/******************************************************************************/
/*
 * Brief               This function verify the macvalue.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_VerifyCmac(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily)
{
    Std_ReturnType ret                       = E_NOT_OK;
    uint8          k1[CRYPTO_CONST_24]       = {0X00};
    uint8          k2[CRYPTO_CONST_24]       = {0X00};
    uint8          key[CRYPTO_CONST_24]      = {0X00};
    uint8          macValue[CRYPTO_CONST_24] = {0X00};
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
    Crypto_AESData aes;
#endif
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
    Crypto_Des3Data ctx;
#endif
    uint32       keylength     = 24u;
    uint32       inputlength   = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    const uint8* add           = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint32       Verifylength  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.secondaryInputLength;
    const uint8* inputMacValue = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.secondaryInputPtr;
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
    (void)IStdLib_MemSet(&aes, 0, sizeof(Crypto_AESData));
#endif
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Des3Data));
#endif
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_MAC_KEY, &key[0], &keylength);

    if (E_OK == ret)
    {
        if (algorithmfamily == CRYPTO_ALGOFAM_AES)
        {
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            ret = Crypto_Generate_Subkey_Aes(&aes, &key[0], k1, k2);
            if (E_OK == ret)
            {
                Crypto_GenerateCmac_Aes(&aes, inputlength, add, macValue, &key[0], k1, k2);
            }
#else
            ret = E_NOT_OK;
#endif
        }
        else if (algorithmfamily == CRYPTO_ALGOFAM_3DES)
        {
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
            ret = Crypto_Generate_Subkey_Des3(&ctx, &key[0], k1, k2);
            if (E_OK == ret)
            {
                Crypto_GenerateCmac_3Des(&ctx, inputlength, add, macValue, key, k1, k2);
            }
#else
            ret = E_NOT_OK;
#endif
        }
        else
        {
            ret = E_NOT_OK;
        }

        ret = IStdLib_MemCmp(macValue, inputMacValue, Verifylength);
        if (E_OK == ret)
        {
            *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_OK;
        }
        else
        {
            *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_NOT_OK;
        }
    }
    return ret;
}
/******************************************************************************/
/*
 * Brief               This function generate the macvalue.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_GenerateCmac_Process(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily)
{
    Std_ReturnType ret                       = E_NOT_OK;
    uint8          k1[CRYPTO_CONST_24]       = {0X00};
    uint8          k2[CRYPTO_CONST_24]       = {0X00};
    uint8          key[CRYPTO_CONST_24]      = {0X00};
    uint8          macValue[CRYPTO_CONST_24] = {0X00};
    uint32         inputlength               = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32*        outputlength              = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
    Crypto_AESData aes;
#endif
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
    Crypto_Des3Data ctx;
#endif
    uint32       keylength = 24u;
    const uint8* add       = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
    (void)IStdLib_MemSet(&aes, 0, sizeof(Crypto_AESData));
#endif
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Des3Data));
#endif
    ret = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_MAC_KEY, &key[0], &keylength);

    if (E_OK == ret)
    {
        if (algorithmfamily == CRYPTO_ALGOFAM_AES)
        {
            if (*outputlength < 16u)
            {
                return E_NOT_OK;
            }
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)
            ret = Crypto_Generate_Subkey_Aes(&aes, &key[0], k1, k2);
            if (E_OK == ret)
            {
                Crypto_GenerateCmac_Aes(&aes, inputlength, add, macValue, &key[0], k1, k2);
                *outputlength = 16u;
                (void)IStdLib_MemCpy(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr, macValue, 16u);
            }
#else
            ret = E_NOT_OK;
#endif
        }
        else if (algorithmfamily == CRYPTO_ALGOFAM_3DES)
        {
            if (*outputlength < 8u)
            {
                return E_NOT_OK;
            }
#if (CRYPTO_ALGORITHMFAM_3DES == STD_ON)
            ret = Crypto_Generate_Subkey_Des3(&ctx, &key[0], k1, k2);
            if (E_OK == ret)
            {
                Crypto_GenerateCmac_3Des(&ctx, inputlength, add, macValue, key, k1, k2);
                *outputlength = 8u;
                (void)IStdLib_MemCpy(
                    Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr,
                    macValue,
                    *outputlength);
            }
#else
            ret = E_NOT_OK;
#endif
        }
        else
        {
            ret = E_NOT_OK;
        }
    }

    return ret;
}
#endif
#if (CRYPTO_ALGORITHMMODE_HMAC == STD_ON)
/******************************************************************************/
/*
 * Brief               This function sets the HMAC key and prepares to authenticate a new message.
 *
 *
 * Param-Name[in]      ctx: message digest context.
 *                     key: The key to be used for hmac.
 *                     keylen: The length of the key.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_GenerateHmac_Start(Crypto_Md_Context_t* ctx, uint8* key, uint32 keylen)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          sum[CRYPTO_MD_MAX_SIZE];
    uint8 *        ipad, *opad;

    if (ctx == NULL_PTR || ctx->md_info == NULL_PTR || ctx->hmac_ctx == NULL_PTR)
    {
        return E_NOT_OK;
    }
    if (keylen > (uint32)ctx->md_info->block_size)
    {
        if ((ret = Crypto_Md_Start(ctx)) != 0)
        {
            goto cleanup;
        }
        if ((ret = Crypto_Md_Update(ctx, key, keylen)) != 0)
        {
            goto cleanup;
        }
        if ((ret = Crypto_Md_Finish(ctx, sum)) != 0)
        {
            goto cleanup;
        }

        keylen = ctx->md_info->size;
        key    = sum;
    }

    ipad = (unsigned char*)ctx->hmac_ctx;
    opad = (unsigned char*)ctx->hmac_ctx + ctx->md_info->block_size;

    (void)IStdLib_MemSet(ipad, 0x36, ctx->md_info->block_size);
    (void)IStdLib_MemSet(opad, 0x5C, ctx->md_info->block_size);

    Crypto_Xor(keylen, ipad, key, ipad);
    Crypto_Xor(keylen, opad, key, opad);

    if ((ret = Crypto_Md_Start(ctx)) != 0)
    {
        goto cleanup;
    }
    if ((ret = Crypto_Md_Update(ctx, ipad, ctx->md_info->block_size)) != 0)
    {
        goto cleanup;
    }

cleanup:

    return ret;
}
/******************************************************************************/
/*
 * Brief               This function feeds an input buffer into an ongoing HMAC computation.
 *
 * Param-Name[in]      ctx: message digest context.
 *                     input: The input data to be hashed.
 *                     ilen: The length of the input data.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_GenerateHmac_Update(Crypto_Md_Context_t* ctx, const uint8* input, uint32 ilen)
{
    if (ctx == NULL_PTR || ctx->md_info == NULL_PTR || ctx->hmac_ctx == NULL_PTR)
    {
        return E_NOT_OK;
    }

    return Crypto_Md_Update(ctx, input, ilen);
}
/******************************************************************************/
/*
 * Brief               This function finishes the HMAC operation, and writes
 *                     the result to the output buffer.
 *
 *
 * Param-Name[in]      ctx: message digest context.
 * Param-Name[in/out]  None.
 * Param-Name[out]     output: output value.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_GenerateHmac_Finish(Crypto_Md_Context_t* ctx, uint8* output)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          tmp[CRYPTO_CONST_64];
    uint8*         opad;

    if (ctx == NULL_PTR || ctx->md_info == NULL_PTR || ctx->hmac_ctx == NULL_PTR)
    {
        return E_NOT_OK;
    }

    opad = (unsigned char*)ctx->hmac_ctx + ctx->md_info->block_size;
    ret  = Crypto_Md_Finish(ctx, tmp);
    if (ret != E_OK)
    {
        return ret;
    }
    ret = Crypto_Md_Start(ctx);
    if (ret != E_OK)
    {
        return ret;
    }
    ret = Crypto_Md_Update(ctx, opad, ctx->md_info->block_size);
    if (ret != E_OK)
    {
        return ret;
    }
    ret = Crypto_Md_Update(ctx, tmp, ctx->md_info->size);
    if (ret != E_OK)
    {
        return ret;
    }
    return Crypto_Md_Finish(ctx, output);
}
/******************************************************************************/
/*
 * Brief               This function prepares to authenticate a new message with
 *                     the same key as the previous HMAC operation.
 *
 * Param-Name[in]      ctx: The message digest context containing an embedded HMAC context.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_md_hmac_reset(Crypto_Md_Context_t* ctx)
{
    Std_ReturnType ret = E_NOT_OK;
    unsigned char* ipad;

    if (ctx == NULL_PTR || ctx->md_info == NULL_PTR || ctx->hmac_ctx == NULL_PTR)
    {
        return E_NOT_OK;
    }

    ipad = (unsigned char*)ctx->hmac_ctx;

    if ((ret = Crypto_Md_Start(ctx)) != 0)
    {
        return ret;
    }
    return Crypto_Md_Update(ctx, ipad, ctx->md_info->block_size);
}
/******************************************************************************/
/*
 * Brief               This function call all the steps for hmac generate.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 *                     operateMode: The operation mode.
 * Param-Name[in/out]  job: The job to be pushed.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_GenerateHmac(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily)
{
    Std_ReturnType          result = E_NOT_OK;
    Crypto_Md_Context_t     ctx;
    const Crypto_Md_Info_t* md_info;
    uint8                   key[CRYPTO_MD_MAX_SIZE];
    uint32                  keyLength = CRYPTO_MD_MAX_SIZE;
    uint8*                  output    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputPtr;
    const uint8*            input     = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint32                  ilen      = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;
    uint32*                 olen      = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.outputLengthPtr;

    (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Md_Context_t));
    md_info       = Crypto_Md_info_from_type(algorithmfamily);
    uint32 mdSize = Crypto_md_get_size(md_info);
    if (*olen < mdSize)
    {
        return result;
    }
    result = Crypto_md_setup(&ctx, md_info);
    if (result == E_OK)
    {
        result = Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_MAC_KEY, key, &keyLength);
    }
    if (result == E_OK)
    {
        result = Crypto_GenerateHmac_Start(&ctx, key, keyLength);
    }
    if (result == E_OK)
    {
        result = Crypto_GenerateHmac_Update(&ctx, input, ilen);
    }
    if (result == E_OK)
    {
        result = Crypto_GenerateHmac_Finish(&ctx, output);
    }
    if (result == E_OK)
    {
        *olen = mdSize;
    }
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, ctx.md_ctx);
    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, ctx.hmac_ctx);

    return result;
}
/******************************************************************************/
/*
 * Brief               This function call all the steps for hmac verify.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 *                     operateMode: The operation mode.
 * Param-Name[in/out]  job: The job to be pushed.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType
    Crypto_VerifyHmac(uint32 objectId, Crypto_AlgorithmFamilyType algorithmfamily, Crypto_OperationModeType operateMode)
{
    Std_ReturnType          result = E_NOT_OK;
    Crypto_Md_Context_t     ctx;
    const Crypto_Md_Info_t* md_info;
    uint8                   key[CRYPTO_MD_MAX_SIZE];
    uint32                  keyLength = CRYPTO_MD_MAX_SIZE;
    const uint8*            inputMac  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.secondaryInputPtr;
    uint32                  macLen    = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.secondaryInputLength;
    uint8                   output[CRYPTO_MD_MAX_SIZE];
    const uint8*            input = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr;
    uint32                  ilen  = Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength;

    switch (operateMode)
    {
    case CRYPTO_OPERATIONMODE_START:
    case CRYPTO_OPERATIONMODE_UPDATE:
        result = E_OK;
        break;

    case CRYPTO_OPERATIONMODE_FINISH:
    case CRYPTO_OPERATIONMODE_SINGLECALL:
    case CRYPTO_OPERATIONMODE_STREAMSTART:
        (void)IStdLib_MemSet(&ctx, 0, sizeof(Crypto_Md_Context_t));
        md_info = Crypto_Md_info_from_type(algorithmfamily);
        result  = Crypto_md_setup(&ctx, md_info);
        if (result == E_OK)
        {
            result =
                Crypto_62_KeyElementGet(Crypto_62_StoredJob[objectId].cryptoKeyId, CRYPTO_KE_MAC_KEY, key, &keyLength);
        }
        if (result == E_OK)
        {
            result = Crypto_GenerateHmac_Start(&ctx, key, keyLength);
        }
        if (result == E_OK)
        {
            result = Crypto_GenerateHmac_Update(&ctx, input, ilen);
        }
        if (result == E_OK)
        {
            result = Crypto_GenerateHmac_Finish(&ctx, output);
        }
        if (result == E_OK)
        {
            if (macLen < ctx.md_info->size)
            {
                result = IStdLib_MemCmp(inputMac, output, macLen);
            }
            else
            {
                result = IStdLib_MemCmp(inputMac, output, macLen);
            }

            if (E_OK == result)
            {
                *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_OK;
            }
            else
            {
                *(Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.verifyPtr) = CRYPTO_E_VER_NOT_OK;
            }
        }
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, ctx.md_ctx);
        (void)IStdLib_MemHeapFree(Crypto_62_MemPool, ctx.hmac_ctx);
        break;
    default:
        /*do nothing*/
        break;
    }
    return result;
}
#endif
/******************************************************************************/
/*
 * Brief               This function process macgenerate service with supported modes.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 *                     mode: The algorithm mode.
 *                     operateMode: The operation mode.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_62_MacGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;
    if ((CRYPTO_OPERATIONMODE_START == operateMode) || (CRYPTO_OPERATIONMODE_UPDATE == operateMode))
    {
        result = E_OK;
    }
    else
    {
        switch (mode)
        {
#if (CRYPTO_ALGORITHMMODE_HMAC == STD_ON)
        case CRYPTO_ALGOMODE_HMAC:
            result = Crypto_GenerateHmac(objectId, algorithmfamily);
            break;
#endif
        case CRYPTO_ALGOMODE_CMAC:
#if (CRYPTO_ALGORITHMMODE_CMAC == STD_ON)
            result = Crypto_GenerateCmac_Process(objectId, algorithmfamily);
#endif
            break;
        case CRYPTO_ALGOMODE_GMAC:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_CTRDRBG:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_SIPHASH_2_4:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_CUSTOM:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_NOT_SET:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
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
 * Brief               This function process macverify service with supported modes.
 *
 *
 * Param-Name[in]      objectId: The object ID of the crypto driver.
 *                     algorithmfamily: The algorithm family.
 *                     mode: The algorithm mode.
 *                     operateMode: The operation mode.
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              Std_ReturnType:
 *                            E_OK: Request successful
 *                            E_NOT_OK: Request failed
 */
/******************************************************************************/
Std_ReturnType Crypto_62_MacVerify_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode,
    Crypto_OperationModeType   operateMode)
{
    Std_ReturnType result = E_NOT_OK;
    if ((CRYPTO_OPERATIONMODE_START == operateMode) || (CRYPTO_OPERATIONMODE_UPDATE == operateMode))
    {
        result = E_OK;
    }
    else
    {
        switch (mode)
        {
#if (CRYPTO_ALGORITHMMODE_HMAC == STD_ON)
        case CRYPTO_ALGOMODE_HMAC:
            result = Crypto_VerifyHmac(objectId, algorithmfamily, operateMode);
            break;
#endif
#if (CRYPTO_ALGORITHMMODE_CMAC == STD_ON)
        case CRYPTO_ALGOMODE_CMAC:
            result = Crypto_VerifyCmac(objectId, algorithmfamily);
            break;
#endif
        case CRYPTO_ALGOMODE_GMAC:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_CTRDRBG:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_SIPHASH_2_4:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_CUSTOM:
            result = CRYPTO_ERROR_ALGO_NOT_SUPPORTED;
            break;
        case CRYPTO_ALGOMODE_NOT_SET:
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

/* PRQA S 0316,0488,0686,1253,1258,1277,1290,1338,1840,1843,1852,1860,1862,1863 -- */
/* PRQA S 2001,2015,3120,3200,3326,3395,3396,3397,3400,3673,3678,3793,4397,4434 -- */
/* PRQA S 1505,2982,1505,1503,1532,2889,4446,4461,4544 -- */
