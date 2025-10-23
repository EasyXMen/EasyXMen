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
 **  @file               : Crypto_62_KeyManagement.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 0314,0316,0570,0597,1258,1290,1291,2109,3120,3206,3625,3673,3762,4434 ++ */   /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,1751,1533,1532,1503,2889,3556,1505   ++ */ /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
#include "Crypto_62_Rsa.h"
#endif
#include "Crypto_62_Random.h"
#include "Crypto_62_CtrDrbg.h"
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */
#define CRYPTO_62_START_SEC_VAR_CLEARED_8
#include "Crypto_62_MemMap.h"
Crypto_KeyStatusType CryptoKeyStatus[CRYPTO_MAXKEY_CONFIGURED];
#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
uint8  Crypto_62_Key_Store0[CRYPTO_NVBLOCK_SIZE_0];
uint16 Crypto_62_Key_RetryCount[CRYPTO_MAXNVBLOCK_CONFIGURED];
#endif
#define CRYPTO_62_STOP_SEC_VAR_CLEARED_8
#include "Crypto_62_MemMap.h"

/* ========================================== external function definitions ========================================= */
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
/***************************************************************************************/
/**
 * @brief CRYPTO driver key element set interface
 * @details Sets the given key element bytes to the key identified by keyId.
 *
 * @param[in]     cryptokeyId   Holds the identifier of the key for which a new material shall be set.
 * @param[in]     keyElementId  Holds the identifier of the key element to be written
 * @param[in]     keyPtr        Holds the pointer to the key element bytes to be Processed.
 * @param[in]     keyLength     Contains the number of key element bytes.
 * @param[in,out] None
 * @param[out]    None
 * @return          Std_ReturnType
 * @retval   E_OK: request successful
 * @retval   E_NOT_OK: request failed
 * @retval   CRYPTO_E_BUSY: request failed,service is still busy
 * @retval   CRYPTO_E_KEY_WRITE_FAIL:Request failed because write access was denied
 * @retval   CRYPTO_E_KEY_NOT_AVAILABLE: Request failed because the key is not available.
 * @retval   CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element size does not match size of provided data.
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeyElementSet(uint32 cryptokeyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength)
{
    Std_ReturnType status = E_NOT_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00075*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00078*/
    else if (NULL_PTR == keyPtr)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_PARAM_POINTER);
    }
    /*SWS_Crypto_00076*/
    else if (cryptokeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00079,SWS_Crypto_00146*/
    else if (0u == keyLength)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        status = Crypto_62_KeyElementSetInternal(cryptokeyId, keyElementId, keyPtr, keyLength);
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key validation interface.
 * @details  This interface sets the key state of the key identified by cryptoKeyId to valid.
 *
 * @param[in]       cryptoKeyId   Holds the identifier of the key which shall be set to valid.
 * @param[in,out]   None.
 * @param[out]      None.
 ** @return          Std_ReturnType
 * @retval          E_OK:          Request successful
 * @retval          E_NOT_OK:      Request Failed
 * @retval          CRYPTO_E_BUSY: Request Failed,Crypto Driver Object is Busy
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeySetValid(uint32 cryptoKeyId)
{
    Std_ReturnType status = E_NOT_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00196*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYSETVALID, CRYPTO_E_UNINIT);
    }
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYSETVALID, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
#if (CRYPTO_MAXNVBLOCK_CONFIGURED > 0)
        if (NULL_PTR != Crypto_62_Key[cryptoKeyId].CryptoKeyNvBlockRef)
        {
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
            status = NvM_SetRamBlockStatus(Crypto_62_Key[cryptoKeyId].CryptoKeyNvBlockRef->Crypto_62_NvmBlockId, TRUE);
#endif
            if ((CRYPTO_NV_BLOCK_IMMEDIATE == Crypto_62_Key[cryptoKeyId].CryptoKeyNvBlockRef->CryptoNvBlockProcessing)
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
                && (E_OK == status)
#endif
            )
            {
                status = NvM_WriteBlock(
                    Crypto_62_Key[cryptoKeyId].CryptoKeyNvBlockRef->Crypto_62_NvmBlockId,
                    (void*)&Crypto_62_Key_Store0[0]);
                if (E_NOT_OK == status)
                {
                    /*SWS_Crypto_00244*/
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
                    Crypto_62_ReportDetErr(CRYPTO_SID_KEYSETVALID, CRYPTO_E_RE_NVM_ACCESS_FAILED);
#endif
                    CryptoKeyStatus[cryptoKeyId] = CRYPTO_KEYSTATUS_UPDATE_IN_PROGRESS;
                }
                else
                {
                    CryptoKeyStatus[cryptoKeyId] = CRYPTO_KEYSTATUS_VALID;
                }
            }
            else
            {
                CryptoKeyStatus[cryptoKeyId] = CRYPTO_KEYSTATUS_INVALID;
            }
        }
        else
#endif
        {
            status                       = E_OK;
            CryptoKeyStatus[cryptoKeyId] = CRYPTO_KEYSTATUS_VALID;
        }
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key validation interface.
 * @details  This interface sets the key state of the key identified by cryptoKeyId to Invalid.
 *
 * @param[in]       cryptoKeyId   Holds the identifier of the key which shall be set to Invalid.
 * @param[in,out]   None.
 * @param[out]      None.
 ** @return          Std_ReturnType
 * @retval          E_OK:          Request successful
 * @retval          E_NOT_OK:      Request Failed
 * @retval          CRYPTO_E_BUSY: Request Failed,Crypto Driver Object is Busy
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeySetInValid(uint32 cryptoKeyId)
{
    Std_ReturnType status = E_NOT_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00236*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYSETINVALID, CRYPTO_E_UNINIT);
    }
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYSETINVALID, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        CryptoKeyStatus[cryptoKeyId] = CRYPTO_KEYSTATUS_INVALID;
        status                       = E_OK;
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key element extraction interface.
 * @details  This interface shall be used to get a key element of the key identified by the cryptoKeyId
 *           and store the key element in the memory location pointed by the result pointer.
 *
 * @param[in]       cryptoKeyId     Holds the identifier of the key whose key element shall be returned.
 * @param[in]       keyElementId    Holds the identifier of the key element which shall be returned.
 * @param[in,out]   resultLengthPtr Holds a pointer to a memory location in which the length information is stored.
 *                                  On calling this function this parameter shall contain the size of the buffer
 * provided by resultPtr. If the key element is configured to allow partial access,this parameter contains the amount of
 * data which should be read from the key element. The size may not be equal to the size of the provided buffer anymore.
 *                                  When the request has finished,the amount of data that has been stored shall be
 * stored.
 * @param[out]      resultPtr       Holds the pointer of the buffer for the returned key element
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed because read access was denied
 * @retval          E_NOT_OK:      The provided buffer is too small to store the result
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType
    Crypto_62_KeyElementGet(uint32 cryptoKeyId, uint32 keyElementId, uint8* resultPtr, uint32* resultLengthPtr)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00085*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_UNINIT);
    }
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00089*/
    else if ((NULL_PTR == resultPtr) || (NULL_PTR == resultLengthPtr))
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00090*/
    else if (0u == *resultLengthPtr)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        /* SWS_Crypto_00087: KeyElement not found */
        uint32 keyEleIdx = 0;
        uint32 Index;
        if (CryptoKeyStatus[cryptoKeyId] == CRYPTO_KEYSTATUS_VALID)
        {
            const Crypto_62_KeyElementCfgType* CryptoKeyElement =
                Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef;
            /*Find target Key Element index*/
            status = Crypto_62_KeyElementIndexFind(cryptoKeyId, keyElementId, &keyEleIdx);
            if (E_OK == status)
            {
                if (*resultLengthPtr < CryptoKeyElement[keyEleIdx].CryptoKeyElementSize)
                {
/* SWS_Crypto_00195: resultPtr too small */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
                    Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_SMALL_BUFFER);
#endif
                    status = (Std_ReturnType)E_NOT_OK;
                }
                /* SWS_Crypto_00139: Check access rights */
                else if (
                    (CryptoKeyElement[keyEleIdx].CryptoKeyElementReadAccess == CRYPTO_RA_DENIED)
                    || (CryptoKeyElement[keyEleIdx].CryptoKeyElementReadAccess == CRYPTO_RA_INTERNAL_COPY))
                {
                    status = (Std_ReturnType)CRYPTO_E_KEY_READ_FAIL;
                }
                else
                {
                    if (CryptoKeyElement[keyEleIdx].CryptoKeyElementAllowPartialAccess == (boolean)TRUE)
                    {
                        *resultLengthPtr = *CryptoKeyElement[keyEleIdx].CryptoKeyElementLength;
                    }
                    else
                    {
                        *resultLengthPtr = CryptoKeyElement[keyEleIdx].CryptoKeyElementSize;
                    }

                    for (Index = 0; Index < *resultLengthPtr; Index++)
                    {
                        resultPtr[Index] = CryptoKeyElement[keyEleIdx].CryptoKeyElementInitValue[Index];
                    }
                }
            }
            else
            {
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
                /*@req SWS_Crypto_00087*/
                Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTGET, CRYPTO_E_PARAM_HANDLE);
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
            }
        }
        else
        {
            status = (Std_ReturnType)CRYPTO_E_KEY_NOT_VALID;
        }
    }
    return status;
}

Std_ReturnType Crypto_62_KeyGetStatus(uint32 cryptoKeyId, Crypto_KeyStatusType* keyStatusPtr)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00085*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYGETSTATUS, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00089*/
    else if (NULL_PTR == keyStatusPtr)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYGETSTATUS, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00090*/
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYGETSTATUS, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        *keyStatusPtr = CryptoKeyStatus[cryptoKeyId];
        status        = (Std_ReturnType)E_OK;
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key element copy interface.
 * @details  This interface copies a key element to another key element in the same crypto driver.
 *
 * @param[in]       cryptoKeyId        Holds the identifier of the key whose key element shall be the source element.
 * @param[in]       keyElementId       Holds the identifier of the key element which shall be the source for the copy
 * operation.
 * @param[in]       targetCryptoKeyId  Holds the identifier of the key whose key element shall be the destination
 * element.
 * @param[in]       targetKeyElementId Holds the identifier of the key element which shall be the destination for the
 * copy operation.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeyElementCopy(
    uint32 cryptoKeyId,
    uint32 keyElementId,
    uint32 targetCryptoKeyId,
    uint32 targetKeyElementId)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00149*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPY, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00150*/ /*SWS_Crypto_00151*/
    else if ((cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED) || (targetCryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPY, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        /*set global VAR as null*/
        uint32                             keyEleIdx  = 0;
        uint32                             keyEleIdxS = 0;
        uint8                              i;
        const Crypto_62_KeyElementCfgType* CryptoKeyElement =
            Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef;

        const Crypto_62_KeyElementCfgType* CryptoKeyElement_Target =
            Crypto_62_Key[targetCryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef;
        /*Find target / Source Key Element Buffer Info*/
        status = Crypto_62_KeyElementIndexFind(targetCryptoKeyId, targetKeyElementId, &keyEleIdx);
        if (E_OK == status)
        {
            status = Crypto_62_KeyElementIndexFind(cryptoKeyId, keyElementId, &keyEleIdxS);
            if (E_OK == status)
            {
                /*Check Read and Write Access before started*/
                if (CryptoKeyElement[keyEleIdxS].CryptoKeyElementReadAccess == CRYPTO_RA_DENIED)
                {
                    status = CRYPTO_E_KEY_READ_FAIL;
                }
                else if (CryptoKeyElement_Target[keyEleIdx].CryptoKeyElementWriteAccess == CRYPTO_WA_DENIED)
                {
                    status = CRYPTO_E_KEY_WRITE_FAIL;
                }
                else if (
                    CryptoKeyElement_Target[keyEleIdx].CryptoKeyElementFormat
                    != CryptoKeyElement[keyEleIdxS].CryptoKeyElementFormat)
                {
                    status = CRYPTO_E_KEY_SIZE_MISMATCH;
                }
                else
                {
                    for (i = 0; i < CryptoKeyElement[keyEleIdxS].CryptoKeyElementSize; i++)
                    {
                        CryptoKeyElement_Target[keyEleIdx].CryptoKeyElementInitValue[i] =
                            CryptoKeyElement[keyEleIdxS].CryptoKeyElementInitValue[i];
                    }
                    *(CryptoKeyElement_Target[keyEleIdx].CryptoKeyElementLength) =
                        *(CryptoKeyElement[keyEleIdxS].CryptoKeyElementLength);
                }
            }
        }
        if (E_NOT_OK == status)
        {
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
            /*@req SWS_Crypto_00152*/ /*SWS_Crypto_00153*/
            Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPY, CRYPTO_E_PARAM_HANDLE);
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
        }
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key element copy interface.
 * @details  Copies a key element to another key element in the same crypto driver.
 *           The keyElementSourceOffset and keyElementCopyLength allows to copy just a part of the source key element
 * into the destination The offset of the target key is also specified with this function. If the actual key element is
 * directly mapped to flash memory, there could be a bigger delay when calling this function(synchronous operation).
 *
 * @param[in]       cryptoKeyId                   Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       keyElementId                  Holds the identifier of the key element which shall be the source for
 * the copy operation.
 * @param[in]       keyElementSourceOffset        This is the offset of the of the source key element indicating the
 * start index of the copy operation.
 * @param[in]       keyElementTargetOffset        This is the offset of the of the target key element indicating the
 * start index of the copy operation.
 * @param[in]       keyElementCopyLength          Specifies the number of bytes that shall be copied.
 * @param[in]       targetCryptoKeyId             Holds the identifier of the key whose key element shall be the
 * destination element.
 * @param[in]       targetKeyElementId            Holds the identifier of the key element which shall be the destination
 * for the copy operation.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeyElementCopyPartial(
    uint32 cryptoKeyId,
    uint32 keyElementId,
    uint32 keyElementSourceOffset,
    uint32 keyElementTargetOffset,
    uint32 keyElementCopyLength,
    uint32 targetCryptoKeyId,
    uint32 targetKeyElementId)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981  */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00205*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPYPARTIAL, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00206*/
    else if ((cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED) || (targetCryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPYPARTIAL, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        uint32                             sourcekeyEleIdx = 0;
        uint32                             targetkeyEleIdx = 0;
        uint8                              i;
        const Crypto_62_KeyElementCfgType* CryptoKeyElement =
            Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef;
        const Crypto_62_KeyElementCfgType* CryptoKeyElement_Target =
            Crypto_62_Key[targetCryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef;
        /*Find target / Source Key Element Buffer Info*/
        status = Crypto_62_KeyElementIndexFind(targetCryptoKeyId, targetKeyElementId, &targetkeyEleIdx);
        if (E_OK == status)
        {
            status = Crypto_62_KeyElementIndexFind(cryptoKeyId, keyElementId, &sourcekeyEleIdx);
            if (E_OK == status)
            {
                /*Check Read and Write Access before started*/
                if (CryptoKeyElement[sourcekeyEleIdx].CryptoKeyElementReadAccess == CRYPTO_RA_DENIED)
                {
                    status = CRYPTO_E_KEY_READ_FAIL;
                }
                else if (CryptoKeyElement_Target[targetkeyEleIdx].CryptoKeyElementWriteAccess == CRYPTO_WA_DENIED)
                {
                    status = CRYPTO_E_KEY_WRITE_FAIL;
                }
                else if (
                    CryptoKeyElement[sourcekeyEleIdx].CryptoKeyElementFormat
                    != CryptoKeyElement_Target[targetkeyEleIdx].CryptoKeyElementFormat)
                {
                    status = CRYPTO_E_KEY_SIZE_MISMATCH;
                }
                else if (
                    ((boolean)FALSE == CryptoKeyElement[sourcekeyEleIdx].CryptoKeyElementAllowPartialAccess)
                    || ((boolean)FALSE == CryptoKeyElement_Target[targetkeyEleIdx].CryptoKeyElementAllowPartialAccess))
                {
                    status = CRYPTO_E_KEY_SIZE_MISMATCH;
                }
                else if (
                    ((keyElementCopyLength + keyElementSourceOffset)
                     > CryptoKeyElement[sourcekeyEleIdx].CryptoKeyElementSize)
                    || ((keyElementCopyLength + keyElementTargetOffset)
                        > CryptoKeyElement_Target[targetkeyEleIdx].CryptoKeyElementSize))
                {
                    status = CRYPTO_E_KEY_SIZE_MISMATCH;
                }
                else
                {
                    for (i = 0; i < keyElementCopyLength; i++)
                    {
                        CryptoKeyElement_Target[targetkeyEleIdx].CryptoKeyElementInitValue[i + keyElementTargetOffset] =
                            CryptoKeyElement[sourcekeyEleIdx].CryptoKeyElementInitValue[i + keyElementSourceOffset];
                    }
                    /*SWS_Crypto_00211*/
                    if ((keyElementTargetOffset + keyElementCopyLength)
                        > *CryptoKeyElement_Target[targetkeyEleIdx].CryptoKeyElementLength)
                    {
                        *CryptoKeyElement_Target[targetkeyEleIdx].CryptoKeyElementLength =
                            keyElementTargetOffset + keyElementCopyLength;
                    }
                }
            }
        }
        if (E_NOT_OK == status)
        {
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
            /*@req SWS_Crypto_00206*/
            Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTCOPYPARTIAL, CRYPTO_E_PARAM_HANDLE);
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
        }
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key copy interface.
 * @details  Copies a key with all its elements to another key in the same crypto driver.
 *           Note: If the actual key element is directly mapped to flash memory,
 *           there could be a bigger delay when calling this function(synchronous operation)
 *
 * @param[in]       cryptoKeyId                   Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       targetCryptoKeyId             Holds the identifier of the key whose key element shall be the
 * destination element.
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed,the requested key element is not available
 * @retval          CRYPTO_E_KEY_READ_FAIL:     Request failed,not allowed to extract key element
 * @retval          CRYPTO_E_KEY_WRITE_FAIL:    Request failed,not allowed to write key element.
 * @retval          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed,key element sizes are not compatible.
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeyCopy(uint32 cryptoKeyId, uint32 targetCryptoKeyId)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK;
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00156*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYCOPY, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00157*/ /*SWS_Crypto_00158*/
    else if ((cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED) || (targetCryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYCOPY, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        uint8  i;
        uint32 keyElementId;

        /*Find target / source Key Buffer in DescType*/
        if (Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->KeyElementNum
            == Crypto_62_Key[targetCryptoKeyId].CryptoKeyTypeRef->KeyElementNum)
        {
            for (i = 0; i < Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->KeyElementNum; i++)
            {
                keyElementId = Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[i].CryptoKeyElementId;
                status       = Crypto_62_KeyElementCopy(cryptoKeyId, keyElementId, targetCryptoKeyId, keyElementId);
                if (E_OK != status)
                {
                    break;
                }
            }
        }
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key get interface.
 * @details  Used to retrieve information which key elements are available in a given key.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       keyElementIdsLengthPtr       Holds a pointer to the memory location in which the number of key
 * elements in the given key is stored. On calling this function, this parameter shall contain the size of the buffer
 * provided by keyElementIdsPtr. When the request has finished,the actual number of key elements shall be stored.
 * @param[in,out]   None.
 * @param[out]      keyElementIdsPtr             Contains the pointer to the array where the ids of the key elements
 * shall be stored.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeyElementIdsGet(
    uint32 cryptoKeyId,
    /* PRQA S 2980 ++ */ /* VL_QAC_Crypto */
    uint32* keyElementIdsPtr,

    uint32* keyElementIdsLengthPtr)
/* PRQA S 2980 -- */
{
    /* PRQA S 2981 ++ */ /* VL_QAC_Crypto */
    Std_ReturnType status = E_NOT_OK;
    /* PRQA S 2981 -- */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00161*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMEMTIDSGET, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00162*/
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMEMTIDSGET, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        uint32 Count;
        uint8  eleIndex;

        eleIndex = Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->KeyElementNum;

        for (Count = 0; Count < eleIndex; Count++)
        {
            keyElementIdsPtr[Count] =
                Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[Count].CryptoKeyElementId;
            keyElementIdsLengthPtr[Count] =
                *Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[Count].CryptoKeyElementLength;
        }

        status = (Std_ReturnType)E_OK;
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key generate interface.
 * @details  This function generates the internal seed state using the provided entropy source.
 *           Furthermore,this function can be used to update the seed state with new entropy
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       seedPtr                      Holds a pointer to the memory location which contains the data to feed
 * the seed.
 * @param[in]       seedLength                   Contains the length of the seed in bytes.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_RandomSeed(uint32 cryptoKeyId, const uint8* seedPtr, uint32 seedLength)
{
    Std_ReturnType status = E_NOT_OK; /* PRQA S 2981 */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00128*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_RANDOMSEED, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00129*/
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_RANDOMSEED, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00130*/
    else if (NULL_PTR == seedPtr)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_RANDOMSEED, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00131*/
    else if (0u == seedLength)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_RANDOMSEED, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        if (CRYPTO_KEYSTATUS_INVALID == CryptoKeyStatus[cryptoKeyId])
        {
            status = CRYPTO_E_KEY_NOT_VALID;
        }
        else
        {
#if (CRYPTO_SERVICE_RANDOM == STD_ON)
            status = Crypto_RandomSeedInternal(cryptoKeyId, seedPtr, seedLength);
#endif
        }
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key generate interface.
 * @details  Generates new key material store it in the key identified by cryptoKeyId.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeyGenerate(uint32 cryptoKeyId)
{
    Std_ReturnType status    = (Std_ReturnType)E_NOT_OK;
    uint32         keyLength = 1u;
    uint8          type;
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
    uint32 private_keylen = 0u;
    uint32 keyEleIdx;
    uint32 rsa_keybits;

    Crypto_rsa_context rsa;
    const char*        pers = "gen_key";
    uint8*             p;
    uint8*             end;
#endif
#if (CRYPTO_ALGORITHMMODE_CTRDRBG == STD_ON)
    Crypto_entropy_context  entropy;
    Crypto_Ctr_Drbg_Context ctr_drbg;

    Crypto_entropy_init(&entropy);
#endif

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00094*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYGENERATE, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00095*/
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYGENERATE, CRYPTO_E_PARAM_HANDLE);
    }
    else
#endif
    {
        const Crypto_62_KeyElementCfgType* CryptoKeyElement =
            Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef;
        status = Crypto_62_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYGENERATE_ALGORITHM, &type, &keyLength);

        if (E_OK == status)
        {
            switch (type)
            {
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)
#if (CRYPTO_ALGORITHMMODE_CTRDRBG == STD_ON)
            case CRYPTO_ALGOFAM_RSA:

                Crypto_Ctr_Drbg_Init(&ctr_drbg, CRYPTO_ALGOFAM_AES);
                Crypto_rsa_init(&rsa, CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5, CRYPTO_ALGOFAM_SHA2_256);

                ctr_drbg.entropy_len           = CRYPTO_CONST_32;
                ctr_drbg.reseed_counter        = CRYPTO_CONST_16;
                ctr_drbg.prediction_resistance = 1U;
                status                         = Crypto_ctr_drbg_seed(
                    &ctr_drbg,
                    Crypto_entropy_func,
                    &entropy,
                    (const unsigned char*)pers,
                    CRYPTO_CONST_7);
                /* PRQA S 1252++ */ /* VL_QAC_Crypto */
                if (E_OK == status)
                {
                    status = Crypto_62_KeyElementIndexFind(cryptoKeyId, CRYPTO_KE_KEYGENERATE_KEY, &keyEleIdx);
                    if (E_OK == status)
                    {
                        rsa_keybits = CryptoKeyElement[keyEleIdx].CryptoKeyElementSize;
                        if ((rsa_keybits >= CRYPTO_RSA1024_PRIVATE_KEY_SIZE)
                            && (rsa_keybits < CRYPTO_RSA2048_PRIVATE_KEY_SIZE))
                        {
                            rsa_keybits = 1024U;
                        }
                        else if (
                            (rsa_keybits >= CRYPTO_RSA2048_PRIVATE_KEY_SIZE)
                            && (rsa_keybits < CRYPTO_RSA3072_PRIVATE_KEY_SIZE))
                        {
                            rsa_keybits = 2048U;
                        }
                        else if (rsa_keybits >= CRYPTO_RSA3072_PRIVATE_KEY_SIZE)
                        {
                            rsa_keybits = 3072U;
                        }
                        else
                        {
                            return E_NOT_OK;
                        }
                        status = Crypto_rsa_gen_key(&rsa, Crypto_ctr_drbg_random, &ctr_drbg, rsa_keybits, 65537u);
                    }
                }
                if (E_OK == status)
                {
                    if (rsa_keybits == 1024U)
                    {
                        private_keylen = CRYPTO_RSA1024_PRIVATE_KEY_SIZE;
                    }
                    if (rsa_keybits == 2048U)
                    {
                        private_keylen = CRYPTO_RSA2048_PRIVATE_KEY_SIZE;
                    }
                    if (rsa_keybits == 3072U)
                    {
                        private_keylen = CRYPTO_RSA3072_PRIVATE_KEY_SIZE;
                    }
                    p              = (uint8*)IStdLib_MemHeapCalloc(Crypto_62_MemPool, 1, private_keylen);
                    end            = p + private_keylen; /* PRQA S 0488 */ /* VL_QAC_Crypto */
                    private_keylen = Crypto_rsa_write_key(&rsa, p, &end);
                    status         = Crypto_62_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYGENERATE_KEY, p, private_keylen);
                    (void)IStdLib_MemSet(p, 0, private_keylen);
                    (void)IStdLib_MemHeapFree(Crypto_62_MemPool, p);

                    Crypto_mpi_free(&rsa.Vi);
                    Crypto_mpi_free(&rsa.Vf);
                    Crypto_mpi_free(&rsa.RN);
                    Crypto_mpi_free(&rsa.D);
                    Crypto_mpi_free(&rsa.Q);
                    Crypto_mpi_free(&rsa.P);
                    Crypto_mpi_free(&rsa.E);
                    Crypto_mpi_free(&rsa.N);
#if !defined(CRYPTO_RSA_NO_CRT)
                    Crypto_mpi_free(&rsa.RQ);
                    Crypto_mpi_free(&rsa.RP);
                    Crypto_mpi_free(&rsa.QP);
                    Crypto_mpi_free(&rsa.DQ);
                    Crypto_mpi_free(&rsa.DP);
#endif /* CRYPTO_RSA_NO_CRT */
                }
                break;
#endif
#endif
            default:
                status = (Std_ReturnType)E_NOT_OK;
                break;
            }
        }
    }
    /* PRQA S 1252-- */
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key Derivation interface.
 * @details  Derives a new key by using the key elements in the given key identified by the cryptoKeyId.
 *           The given key contains the key elements for the password,salt.
 *           The derived key is stored in the key element with the id 1 of the key identified by targetCryptoKeyId.
 *           The number of iterations is given in the key element CRYPTO_KE_KEYDERIVATION_ITERATIONS.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       targetCryptoKeyId            Holds the identifier of the key whose key element shall be the
 * destination element.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeyDerive(uint32 cryptoKeyId, uint32 targetCryptoKeyId)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981 */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00097*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYDERIVE, CRYPTO_E_UNINIT);
    }
    /*@req SWS_Crypto_00098*/ /*@req SWS_Crypto_00180*/
    else if ((cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED) || (targetCryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED))
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYGENERATE, CRYPTO_E_PARAM_HANDLE);
    }
    else
#else
    (void)cryptoKeyId;
    (void)targetCryptoKeyId;
#endif
    {
        status = (Std_ReturnType)E_NOT_OK;
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key Exchange interface.
 * @details  Calculates the public value for the key exchange and
 *           stores the public key in the memory location pointed by the public value pointer.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 *
 * @param[in,out]   publicValueLengthPtr         Holds a pointer to the memory location in which the public value length
 * information is stored.
 * @param[out]      publicValuePtr               Contains the pointer to the data where the public value shall be
 * stored.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
/***************************************************************************************/
Std_ReturnType Crypto_62_KeyExchangeCalcPubVal(uint32 cryptoKeyId, uint8* publicValuePtr, uint32* publicValueLengthPtr)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981 */ /* VL_QAC_Crypto */

#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00103*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCPUBVAL, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00104*/
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCPUBVAL, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00105*/ /*@req SWS_Crypto_00106*/
    else if ((NULL_PTR == publicValuePtr) || (NULL_PTR == publicValueLengthPtr))
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCPUBVAL, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00107*/
    else if (0u == *publicValueLengthPtr)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCPUBVAL, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        status = Crypto_62_KeyExchangeCalcPubVal_internal(cryptoKeyId, publicValuePtr, publicValueLengthPtr);
    }
    return status;
}
/***************************************************************************************/
/**
 * @brief    CRYPTO driver key Exchange interface.
 * @details  Calculates the shared secret key for the key exchange with the key material of the key identified by the
 * cryptoKeyId and the partner public key. The shared secret key is stored as a key element in the same key.
 *
 * @param[in]       cryptoKeyId                  Holds the identifier of the key whose key element shall be the source
 * element.
 * @param[in]       partnerPublicValuePtr        Holds the pointer to the memory location which contains the partner's
 * public value.
 * @param[in]       partnerPublicValueLength     Contains the length of the partner's public value in bytes.
 *
 * @param[in,out]   None.
 * @param[out]      None.
 *
 * @return          Std_ReturnType
 * @retval          E_OK:                       Request successful
 * @retval          E_NOT_OK:                   Request Failed
 * @retval          CRYPTO_E_BUSY:              Request Failed,Crypto Driver Object is Busy
 * @retval          E_NOT_OK:       The provided buffer is too small to store the result
 * @retval          CRYPTO_E_KEY_EMPTY:         Request failed because of uninitialized source key element
 *
 * @api
 *
 */
/***************************************************************************************/

Std_ReturnType Crypto_62_KeyExchangeCalcSecret(
    uint32       cryptoKeyId,
    const uint8* partnerPublicValuePtr,
    uint32       partnerPublicValueLength)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK; /* PRQA S 2981 */ /* VL_QAC_Crypto */
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
    /*@req SWS_Crypto_00111*/
    if (CRYPTO_DRIVER_UNINIT == Crypto_62_DriverStatus)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCSECRET, CRYPTO_E_UNINIT);
    }
    /*SWS_Crypto_00112*/
    else if (cryptoKeyId >= CRYPTO_MAXKEY_CONFIGURED)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCSECRET, CRYPTO_E_PARAM_HANDLE);
    }
    /*@req SWS_Crypto_00113*/
    else if (NULL_PTR == partnerPublicValuePtr)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCSECRET, CRYPTO_E_PARAM_POINTER);
    }
    /*@req SWS_Crypto_00115*/
    else if (0u == partnerPublicValueLength)
    {
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYEXCHANGECALCSECRET, CRYPTO_E_PARAM_VALUE);
    }
    else
#endif
    {
        status = Crypto_62_KeyExchangeCalcSecret_internal(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength);
    }
    return status;
}

/*Find Key Element index*/

Std_ReturnType Crypto_62_KeyElementIndexFind(uint32 cryptokeyId, uint32 keyElementId, uint32* getkeyElementId)
{
    uint32                             index;
    Std_ReturnType                     ret = E_NOT_OK;
    uint32                             KeyElementNum;
    const Crypto_62_KeyElementCfgType* CryptoKeyElement =
        Crypto_62_Key[cryptokeyId].CryptoKeyTypeRef->CryptoKeyElementRef;

    KeyElementNum = Crypto_62_Key[cryptokeyId].CryptoKeyTypeRef->KeyElementNum;
    /*Find Key space */
    for (index = 0; index < KeyElementNum; index++)
    {
        if (CryptoKeyElement[index].CryptoKeyElementId == keyElementId)
        {
            *getkeyElementId = index;
            ret              = E_OK;
            break;
        }
    }

    return ret;
}
/* Internal implement for CRYPTO driver key element set interface*/

Std_ReturnType
    Crypto_62_KeyElementSetInternal(uint32 cryptokeyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength)
{
    Std_ReturnType                     status;
    uint32                             keyEleIdx;
    uint16                             Index;
    const Crypto_62_KeyElementCfgType* CryptoKeyElement =
        Crypto_62_Key[cryptokeyId].CryptoKeyTypeRef->CryptoKeyElementRef;

    status = Crypto_62_KeyElementIndexFind(cryptokeyId, keyElementId, &keyEleIdx);

    if (E_OK == status)
    {
        /*SWS_Crypto_00223*/
        if ((CryptoKeyElement[keyEleIdx].CryptoKeyElementWriteAccess == CRYPTO_WA_ALLOWED)
            || (CryptoKeyElement[keyEleIdx].CryptoKeyElementWriteAccess == CRYPTO_WA_ENCRYPTED))
        {
            /*SWS_Crypto_00146*/
            if (((keyLength < CryptoKeyElement[keyEleIdx].CryptoKeyElementSize)
                 && (FALSE == CryptoKeyElement[keyEleIdx].CryptoKeyElementAllowPartialAccess))
                || (keyLength > CryptoKeyElement[keyEleIdx].CryptoKeyElementSize))
            {
                status = CRYPTO_E_KEY_SIZE_MISMATCH;
            }
            else
            {
                for (Index = 0; Index < keyLength; Index++)
                {
                    CryptoKeyElement[keyEleIdx].CryptoKeyElementInitValue[Index] = keyPtr[Index];
                }
                *(CryptoKeyElement[keyEleIdx].CryptoKeyElementLength) = keyLength;
                status                                                = E_OK;
                CryptoKeyStatus[cryptokeyId]                          = CRYPTO_KEYSTATUS_INVALID;
            }
        }
        else
        {
            status = CRYPTO_E_KEY_WRITE_FAIL;
        }
    }
    else
    {
#if (CRYPTO_DEV_ERROR_DETECT == STD_ON)
        /*@req SWS_Crypto_00077*/
        Crypto_62_ReportDetErr(CRYPTO_SID_KEYELEMENTSET, CRYPTO_E_PARAM_HANDLE);
#endif /* CRYPTO_DEV_ERROR_DETECT == STD_ON */
    }

    return status;
}
/******************************************************************************/
/*
 * Brief               This function do the Random seed progress .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: algorithmMode configured in crypto,
 *                     algorithmfamily: familly configured in crypto,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
Std_ReturnType Crypto_62_RandomSeed_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode)
{
    Std_ReturnType result = E_NOT_OK;
    (void)mode;
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_3DES:
        break;
    case CRYPTO_ALGOFAM_AES:
        break;
    case CRYPTO_ALGOFAM_BLAKE_1_256:
        break;
    case CRYPTO_ALGOFAM_BLAKE_1_512:
        break;
    case CRYPTO_ALGOFAM_BLAKE_2s_256:
        break;
    case CRYPTO_ALGOFAM_BLAKE_2s_512:
        break;
    case CRYPTO_ALGOFAM_CHACHA:
        break;
    case CRYPTO_ALGOFAM_CUSTOM:
        break;
    case CRYPTO_ALGOFAM_DRBG:
#if (CRYPTO_SERVICE_RANDOM == STD_ON)
        result = Crypto_62_RandomSeed(
            Crypto_62_StoredJob[objectId].cryptoKeyId,
            Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputPtr,
            Crypto_62_StoredJob[objectId].jobPrimitiveInputOutput.inputLength);
#endif
        break;
    case CRYPTO_ALGOFAM_EEA3:
        break;
    case CRYPTO_ALGOFAM_FIPS186:
        break;
    case CRYPTO_ALGOFAM_RIPEMD160:
        break;
    case CRYPTO_ALGOFAM_RNG:
        break;
    case CRYPTO_ALGOFAM_SHA1:
        break;
    case CRYPTO_ALGOFAM_SHA2_224:
        break;
    case CRYPTO_ALGOFAM_SHA2_256:
        break;
    case CRYPTO_ALGOFAM_SHA2_384:
        break;
    case CRYPTO_ALGOFAM_SHA2_512:
        break;
    case CRYPTO_ALGOFAM_SHA2_512_224:
        break;
    case CRYPTO_ALGOFAM_SHA2_512_256:
        break;
    case CRYPTO_ALGOFAM_SHA3_224:
        break;
    case CRYPTO_ALGOFAM_SHA3_256:
        break;
    case CRYPTO_ALGOFAM_SHA3_384:
        break;
    case CRYPTO_ALGOFAM_SHA3_512:
        break;
    case CRYPTO_ALGOFAM_SHAKE128:
        break;
    case CRYPTO_ALGOFAM_SHAKE256:
        break;
    case CRYPTO_ALGOFAM_SM3:
    default:
        /*do nothing*/
        break;
    }
    return result;
}
/******************************************************************************/
/*
 * Brief               This function do the Key generate progress .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: algorithmMode configured in crypto,
 *                     algorithmfamily: familly configured in crypto,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
Std_ReturnType Crypto_62_KeyGenerate_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode)
{
    Std_ReturnType result = E_NOT_OK;
    (void)objectId;
    (void)mode;
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_RSA:
#if (CRYPTO_ALGORITHMFAM_RSA == STD_ON)

#endif /*CRYPTO_ALGORITHMFAM_CTRG == STD_ON*/
        break;
    case CRYPTO_ALGOFAM_CUSTOM:

        break;
    case CRYPTO_ALGOFAM_ECCANSI:
        break;
    case CRYPTO_ALGOFAM_ECCNIST:
        break;
    case CRYPTO_ALGOFAM_ECCSEC:
        break;
    case CRYPTO_ALGOFAM_ECDH:
        break;
    case CRYPTO_ALGOFAM_ED25519:
        break;
    case CRYPTO_ALGOFAM_X25519:
        break;
    default:
        /*do nothing*/
        break;
    }
    return result;
}
/******************************************************************************/
/*
 * Brief               This function do the Key derive progress .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: algorithmMode configured in crypto,
 *                     algorithmfamily: familly configured in crypto,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
Std_ReturnType Crypto_62_KeyDerive_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode)
{
    Std_ReturnType result = E_OK;
    (void)objectId;
    (void)mode;
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_CUSTOM:

        break;
    case CRYPTO_ALGOFAM_HKDF:
        break;
    case CRYPTO_ALGOFAM_KDFX963:
        break;
    case CRYPTO_ALGOFAM_PBKDF2:
        break;
    default:
        /*do nothing*/
        break;
    }
    return result;
}
/******************************************************************************/
/*
 * Brief               This function do the public Key exchange progress .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: algorithmMode configured in crypto,
 *                     algorithmfamily: familly configured in crypto,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
Std_ReturnType Crypto_62_KeyExchangeCalcPubval_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode)
{
    Std_ReturnType result = E_NOT_OK;
    (void)objectId;
    (void)mode;
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_CUSTOM:
        break;
    case CRYPTO_ALGOFAM_DH:

        break;
    case CRYPTO_ALGOFAM_ECDH:
        break;
    default:
        /*do nothing*/
        break;
    }
    return result;
}
/******************************************************************************/
/*
 * Brief               This function do the private Key generate progress .
 *
 * Param-Name[in]      objectId: objectId configured in crypto.
 *                     mode: algorithmMode configured in crypto,
 *                     algorithmfamily: familly configured in crypto,
 * Param-Name[in/out]  None.
 * Param-Name[out]     None.
 * Return              E_OK;
 *                     E_NOT_OK
 */
/******************************************************************************/
Std_ReturnType Crypto_62_KeyExchangeCalcSecret_Process(
    uint32                     objectId,
    Crypto_AlgorithmFamilyType algorithmfamily,
    Crypto_AlgorithmModeType   mode)
{
    Std_ReturnType result = E_NOT_OK;
    (void)objectId;
    (void)mode;
    switch (algorithmfamily)
    {
    case CRYPTO_ALGOFAM_CUSTOM:
        break;
    case CRYPTO_ALGOFAM_DH:
        break;
    case CRYPTO_ALGOFAM_ECDH:
        break;
    case CRYPTO_ALGOFAM_RSA:
        break;
    default:
        /*do nothing*/
        break;
    }
    return result;
}
/* Internal implement for CRYPTO driver public key exchange interface*/
Std_ReturnType
    Crypto_62_KeyExchangeCalcPubVal_internal(uint32 cryptoKeyId, uint8* publicValuePtr, uint32* publicValueLengthPtr)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK;
    (void)cryptoKeyId;
    (void)publicValuePtr;
    (void)publicValueLengthPtr;

    return status;
}
/* Internal implement for CRYPTO driver private key exchange interface*/
Std_ReturnType Crypto_62_KeyExchangeCalcSecret_internal(
    uint32       cryptoKeyId,
    const uint8* partnerPublicValuePtr,
    uint32       partnerPublicValueLength)
{
    Std_ReturnType status = (Std_ReturnType)E_NOT_OK;
    (void)cryptoKeyId;
    (void)partnerPublicValuePtr;
    (void)partnerPublicValueLength;
    return status;
}
#endif
/* Internal implement for CRYPTO driver random seed interface*/
Std_ReturnType Crypto_RandomSeedInternal(uint32 cryptoKeyId, const uint8* seedPtr, uint32 seedLength)
{
    Std_ReturnType status;
    uint8          index;
    uint32         keyEleIdx = 0u;
    status                   = Crypto_62_KeyElementIndexFind(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &keyEleIdx);
    if (E_OK == status)
    {
        for (index = 0; index < seedLength; index++)
        {
            Crypto_62_Key[cryptoKeyId]
                .CryptoKeyTypeRef->CryptoKeyElementRef[keyEleIdx]
                .CryptoKeyElementInitValue[index] = seedPtr[index];
        }
        *(Crypto_62_Key[cryptoKeyId].CryptoKeyTypeRef->CryptoKeyElementRef[keyEleIdx].CryptoKeyElementLength) =
            seedLength;
    }
    return status;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"

/* PRQA S 0314,0316,0570,0597,1258,1290,1291,2109,3120,3206,3625,3673,3762,4434 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,1751,1533,1532,1503,2889,3556,1505   -- */
