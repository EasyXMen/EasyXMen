/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : cmac.c                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for cmac                                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/* PRQA S 5209 ++  */               /* MISRA Dir 4.6 */
/* PRQA S 0686 ++  */               /* MISRA Dir 9.3 */
/* PRQA S 3432 ++ */                /* MISRA Rule 20.7 */
/* PRQA S 3200 ++ */                /* MISRA Rule 17.7 */
/* PRQA S 2784 ++ */                /* MISRA Rule 17.5 */
/* PRQA S 3397,3396,3395 ++ */      /* MISRA Rule 12.1 */
/* PRQA S 1863,1862,1860,1852 ++ */ /* MISRA Rule 10.4 */
/* PRQA S 4443,4434 ++ */           /* MISRA Rule 10.3 */
/* PRQA S 1505,1532 ++ */           /* MISRA Rule 8.7 */
/* PRQA S 3108 ++ */                /* MISRA Rule 3.1 */

#include "cmac.h"
#include "Crypto_internal.h"
#include "aes.h"
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"
#if (CRYPTO_ALGORITHM_CMAC == STD_ON)

FUNC(void, CMAC_CODE) leftshift(int len, uint8* add, uint8* des);
FUNC(void, CMAC_CODE) ArrayXor(int len, uint8* a1, uint8* a2, uint8* des);
FUNC(Std_ReturnType, CMAC_CODE)
LoadMacKey(
    P2VAR(Crypto_AESData, AUTOMATIC, CRYPTO_APPL_DATA) aes,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) IVkey,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) k1,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) k2);
FUNC(Std_ReturnType, CMAC_CODE)
internal_GenerateMAC(
    P2VAR(Crypto_AESData, AUTOMATIC, CRYPTO_APPL_DATA) aes,
    VAR(uint8, AUTOMATIC) len,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) add,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) macvalue,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) Key,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) IVKey,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) k1,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) k2);

FUNC(void, CMAC_CODE) leftshift(int len, uint8* add, uint8* des)
{
    int i;
    for (i = 0; i < len - 1; i++)
    {
        des[i] = (add[i] << 1) + (add[i + 1] >= 0x80 ? 1 : 0);
    }
    des[len - 1] = add[len - 1] << 1;
}

FUNC(void, CMAC_CODE) ArrayXor(int len, uint8* a1, uint8* a2, uint8* des)
{
    int i;
    for (i = 0; i < len; i++)
    {
        des[i] = a1[i] ^ a2[i];
    }
}

FUNC(Std_ReturnType, CMAC_CODE)
LoadMacKey(
    P2VAR(Crypto_AESData, AUTOMATIC, CRYPTO_APPL_DATA) aes,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) IVkey,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) k1,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) k2)
{
    Std_ReturnType ret;
    uint8 c0[16];
    uint8 plain[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8 Rb[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    (void)Crypto_aes_setkey_enc(aes, key, 128);

    ret = Crypto_aes_crypt_cbc_CMAC(aes, AES_ENCRYPT, 16, IVkey, plain, c0);
    if (c0[0] < 0x80) // generate k1
    {
        leftshift(16, c0, k1);
    }
    else
    {
        leftshift(16, c0, k1);
        ArrayXor(16, k1, Rb, k1);
    }

    if (k1[0] < 0x80) // generate k2
    {
        leftshift(16, k1, k2);
    }
    else
    {
        leftshift(16, k1, k2);
        ArrayXor(16, k2, Rb, k2);
    }
    return ret;
}

FUNC(Std_ReturnType, CMAC_CODE)
internal_GenerateMAC(
    P2VAR(Crypto_AESData, AUTOMATIC, CRYPTO_APPL_DATA) aes,
    VAR(uint8, AUTOMATIC) len,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) add,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) macvalue,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) Key,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) IVKey,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) k1,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) k2)
{
    Std_ReturnType ret;
    int i, block;
    uint8 IVtemp[128];
    uint8 Blocktemp[128];
    Crypto_memset(IVtemp, 128);
    Crypto_memset(Blocktemp, 128);

    if (len % 16 == 0 && len != 0)
    {
        ret = Crypto_aes_setkey_enc(aes, Key, 128); // load mac key
        block = len / 16;
        for (i = 0; i < block - 1; i++)
        {
            ArrayXor(16, &add[i * 16], IVtemp, Blocktemp);
            Crypto_aes_crypt_cbc_CMAC(aes, AES_ENCRYPT, 16, IVKey, Blocktemp, IVtemp);
        }
        ArrayXor(16, &add[(block - 1) * 16], IVtemp, Blocktemp);
        ArrayXor(16, Blocktemp, k1, Blocktemp);
        Crypto_aes_crypt_cbc_CMAC(aes, AES_ENCRYPT, 16, IVKey, Blocktemp, macvalue);
    }
    else
    {
        if (len == 0)
        {
            ret = Crypto_aes_setkey_enc(aes, Key, 128); // load mac key
            block = 1; /* PRQA S 2983  */               /* MISRA Rule 2.2 */
            Blocktemp[0] = 0x80;                        // padding the first bit with 1
            ArrayXor(16, Blocktemp, k2, Blocktemp);
            Crypto_aes_crypt_cbc_CMAC(aes, AES_ENCRYPT, 16, IVKey, Blocktemp, macvalue);
        }
        else
        {
            ret = Crypto_aes_setkey_enc(aes, Key, 128); // load mac key
            uint8 remain = len % 16;
            block = len / 16 + 1;
            for (i = 0; i < block - 1; i++)
            {
                ArrayXor(16, &add[i * 16], IVtemp, Blocktemp);
                Crypto_aes_crypt_cbc_CMAC(aes, AES_ENCRYPT, 16, IVKey, Blocktemp, IVtemp);
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

            ArrayXor(16, Blocktemp, k2, Blocktemp);
            ArrayXor(16, Blocktemp, IVtemp, Blocktemp);
            Crypto_aes_crypt_cbc_CMAC(aes, AES_ENCRYPT, 16, IVKey, Blocktemp, macvalue);
        }
    }
    return ret;
}

FUNC(Std_ReturnType, CMAC_CODE)
Crypto_GernerateMAC(uint8 MAC[16], uint8 Key[16], uint8 IVKey[16], uint8* add, uint8 inputlength)
{
    Std_ReturnType ret;
    uint8 k1[16] = {0X00};
    uint8 k2[16] = {0X00};
    Crypto_AESData aes;

    ret = LoadMacKey(&aes, Key, IVKey, k1, k2);
    internal_GenerateMAC(&aes, inputlength, add, MAC, Key, IVKey, k1, k2);

    return ret;
}

FUNC(Std_ReturnType, CMAC_CODE)
Crypto_VerifyMAC(uint8* Key, uint8* IVKey, uint8* add, uint8 inputlength, uint8 MAC_Result[16], uint32 Verifylength)
{
    Std_ReturnType ret;
    uint8 i;
    uint8 MAC[16] = {0X00};
    uint8 k1[16] = {0X00};
    uint8 k2[16] = {0X00};
    Crypto_AESData aes;

    ret = LoadMacKey(&aes, Key, IVKey, k1, k2);
    if (ret == E_OK)
    {
        ret = internal_GenerateMAC(&aes, inputlength, add, MAC, Key, IVKey, k1, k2);
    }

    if (ret == E_OK)
    {
        for (i = 0; i < Verifylength; i++)
        {
            if (MAC[i] != MAC_Result[i])
            {
                ret = E_NOT_OK;
                break;
            }
        }
    }
    return ret;
}
#endif /* CRYPTO_ALGORITHM_CMAC == STD_ON */
#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

/* PRQA S 5209 --  */               /* MISRA Dir 4.6 */
/* PRQA S 0686 --  */               /* MISRA Dir 9.3 */
/* PRQA S 3432 -- */                /* MISRA Rule 20.7 */
/* PRQA S 3200 -- */                /* MISRA Rule 17.7 */
/* PRQA S 2784 -- */                /* MISRA Rule 17.5 */
/* PRQA S 3397,3396,3395 -- */      /* MISRA Rule 12.1 */
/* PRQA S 1863,1862,1860,1852 -- */ /* MISRA Rule 10.4 */
/* PRQA S 4443,4434 -- */           /* MISRA Rule 10.3 */
/* PRQA S 1505,1532 -- */           /* MISRA Rule 8.7 */
/* PRQA S 3108 -- */                /* MISRA Rule 3.1 */
