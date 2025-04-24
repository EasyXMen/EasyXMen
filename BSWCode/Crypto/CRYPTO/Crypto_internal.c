/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Crypto_internal.c                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for Crypto                                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>    <AUTHOR>        <REVISION LOG>
 *  V1.0        20200701  yuzhe.zhang      Initial version
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
/* PRQA S 3344 ++ */ /* MISRA Rule 14.4 */
/* PRQA S 3440 ++ */ /* MISRA Rule 13.3 */
/* PRQA S 1842 ++ */ /* MISRA Rule 10.4 */
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
/* PRQA S 1503 ++ */ /* MISRA Rule 2.1 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Crypto_internal.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
/*Find Key Element index*/

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementIndexFind(
    VAR(uint32, AUTOMATIC) cryptokeyId,
    VAR(uint32, AUTOMATIC) keyElementId,
    P2VAR(uint32, AUTOMATIC, CRYPTO_APPL_DATA) getkeyElementId)
{
    uint32 i;
    Std_ReturnType ret = E_NOT_OK;
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00076*/
    if (cryptokeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
    {
        /*Find Key space */
        for (i = 0; i < Crypto_Key[cryptokeyId].CryptoKeyTypeRef->KeyElementNum; i++)
        {
            if (Crypto_Key[cryptokeyId].CryptoKeyTypeRef->CryptoKeyElementRef[i].CryptoKeyElementId == keyElementId)
            {
                *getkeyElementId = i;
                ret = E_OK;
                break;
            }
        }
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
        /*@req SWS_Crypto_00076*/
        if (E_OK != ret)
        {
            Crypto_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_PARAM_HANDLE);
        }
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
Get_Key(
    VAR(uint32, AUTOMATIC) cryptoKeyId,
    P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) key,
    VAR(uint8, AUTOMATIC) keyelementid)
{
    Std_ReturnType ret;
    uint16 i;
    uint32 KeyElementIndex;
    ret = Crypto_KeyElementIndexFind(cryptoKeyId, keyelementid, &KeyElementIndex);
    if (E_OK == ret)
    {
        if ((CryptoKeyStatus[cryptoKeyId] == CRYPTO_KEYSTATE_VALID))
        {
            for (i = 0;
                 i
                 < Crypto_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex].CryptoKeyElementSize;
                 i++)
            {
                key[i] = Crypto_Key[cryptoKeyId]
                             .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                             .CryptoKeyElementInitValue[i];
            }
        }
        else
        {
            ret = CRYPTO_E_KEY_READ_FAIL;
        }
    }
    return ret;
}

FUNC(Std_ReturnType, CRYPTO_CODE)
Crypto_KeyElementSetInternal(
    uint32 cryptokeyId,
    uint32 keyElementId,
    P2CONST(uint8, AUTOMATIC, CSM_APPL_DATA) keyPtr,
    uint32 keyLength)
{
    Std_ReturnType Status;
    uint32 KeyElementIndex;
    uint16 Index;

    Status = Crypto_KeyElementIndexFind(cryptokeyId, keyElementId, &KeyElementIndex);

    if (E_OK == Status)
    {
        Status = E_NOT_OK;
        if (keyLength
            <= Crypto_Key[cryptokeyId].CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex].CryptoKeyElementSize)
        {
            for (Index = 0; Index < keyLength; Index++)
            {
                Crypto_Key[cryptokeyId]
                    .CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex]
                    .CryptoKeyElementInitValue[Index] = keyPtr[Index];
            }
            *(Crypto_Key[cryptokeyId].CryptoKeyTypeRef->CryptoKeyElementRef[KeyElementIndex].CryptoKeyElementLength) =
                keyLength;
            Status = E_OK;
        }
    }
    return Status;
}
#endif
/* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
/* PRQA S 3387 ++ */ /* MISRA Rule 13.3 */
/* PRQA S 1338 ++ */ /* MISRA Rule 17.8 */
FUNC(void, CRYPTO_CODE) Crypto_memset(void* buf, uint32 len)
{
    uint8* tmpsrc = (uint8*)buf;

    if ((len > 0) && (buf != NULL_PTR))
    {
        while (len--)
        {
            *tmpsrc++ = (uint8)0;
        }
    }
}

FUNC(void, CRYPTO_CODE) Crypto_memsetVal(void* buf, uint8 val, uint32 len)
{
    uint8* tmpsrc = (uint8*)buf;

    if ((len > 0) && (buf != NULL_PTR))
    {
        while (len--)
        {
            *tmpsrc++ = val;
        }
    }
}

FUNC(void, CRYPTO_CODE) Crypto_memcpy(void* buf, const void* buf1, uint32 len)
{
    uint32 i;
    if ((len > 0) && (buf != NULL_PTR) && (buf1 != NULL_PTR))
    {
        const char* psrc = (const char*)buf1;
        char* pdest = (char*)buf;
        for (i = 0; i < len; i++)
        {
            pdest[i] = psrc[i];
        }
    }
}
/* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
/* PRQA S 3387 -- */ /* MISRA Rule 13.3 */
/* PRQA S 1338 -- */ /* MISRA Rule 17.8 */
#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/* PRQA S 3344 -- */ /* MISRA Rule 14.4 */
/* PRQA S 3440 -- */ /* MISRA Rule 13.3 */
/* PRQA S 1842 -- */ /* MISRA Rule 10.4 */
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
/* PRQA S 1503 -- */ /* MISRA Rule 2.1 */
