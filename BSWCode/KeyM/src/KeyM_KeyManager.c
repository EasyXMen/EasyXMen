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
 ***********************************************************************************************************************
 **
 **  @file               : keyM_KeyManager.c
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : KeyM  KeyManager Interface Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

/* PRQA S 3432 ++ */ /* VL_KeyM_3432 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
/* PRQA S 3206 ++ */ /* VL_KeyM_3206 */

#include "KeyM_Internal.h"
/*SWS_KeyM_00001*/
#if (KEYM_CRYPTO_KEYMANAGER_ENABLED == STD_ON)
#include "KeyM_Externals.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
/**
 * @brief          Searches for a key configuration index by key name.
 * @param[in]      KeyNamePtr : Pointer to the key name to search for.
 * @param[in]      KeyNameLength : Length of the key name.
 * @param[out]     KeyCfgIdx : Pointer to the variable where the key configuration index will be stored.
 * @return         Std_ReturnType
 * @retval         E_OK: The key configuration index is found.
 * @retval         E_NOT_OK: The key configuration index is not found.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71463
 */
KEYM_LOCAL Std_ReturnType KeyM_SearchByKeyName(const uint8* KeyNamePtr, uint16 KeyNameLength, uint16* KeyCfgIdx);

/* ============================================ internal data definitions =========================================== */
#if (KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON)
#define KEYM_START_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"

/**
 * @brief          Array to track key update status.
 * @details        This array is used to track whether each key needs to be updated.
 * @range         KEYM_KEY_NUM
 */
KEYM_LOCAL boolean KeyM_NeedUpdate[KEYM_KEY_NUM];
#define KEYM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
#endif

#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
#define KEYM_START_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
/**
 * @brief          Flag to indicate the status of the SHE key.
 * @details        This flag is used to indicate whether the SHE key has been updated or needs attention.
 * @range 0-1
 */
KEYM_LOCAL boolean KeyM_SheKeyFlag;
#define KEYM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
#endif

/* ============================================ external data definitions =========================================== */
#if (KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON)
#define KEYM_START_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
/**
 * @brief Flag to indicate whether the session is open.
 * @range 0-1
 */
boolean KeyM_SessionOpen; /* PRQA S 1504*/ /* VL_KeyM_1504 */
#define KEYM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
#endif

/* ========================================== external function definitions ========================================= */
#define KEYM_START_SEC_CODE
#include "KeyM_MemMap.h"
#if (KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON)
/*************************************************************************/
/**
 * @brief  This function intents to allow key update operation.
 */
/*************************************************************************/
/* PRQA S 3673,3408 ++*/ /* VL_KeyM_3673,VL_KeyM_3408 */
Std_ReturnType KeyM_Start(
    KeyM_StartType StartType,
    const uint8*   RequestData,
    uint16         RequestDataLength,
    uint8*         ResponseData,
    uint16*        ResponseDataLength)
/* PRQA S 3673,3408 --*/
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_KeyM_2981 */

#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_START_ID, KEYM_E_UNINIT);
    }
    /*SWS_KeyM_00087:check startType*/
    else if ((StartType > KEYM_START_WORKSHOPMODE) && (StartType < KEYM_START_USER_BEGIN))
    {
        KEYM_DET_REPORT(KEYM_START_ID, KEYM_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
    {
#if (STD_ON == KEYM_CRYPTO_KEYHANDLER_START_FINALIZE_ENABLED)
        /*SWS_KeyM_00088:OEM or security specific checks
         * SWS_KeyM_00006
         * */
        ret = KeyM_KH_Start(StartType, RequestData, RequestDataLength, ResponseData, ResponseDataLength);
#else
        /*SWS_KeyM_00005*/
        (void)StartType;
        (void)RequestData;
        (void)RequestDataLength;
        (void)ResponseData;
        (void)ResponseDataLength;
        ret = E_OK;
#endif /* STD_ON == KEYM_CRYPTO_KEYHANDLER_START_FINALIZE_ENABLED */
        /*SWS_KeyM_00085:initiate a key update session
         * SWS_KeyM_00004*/
        SchM_Enter_KeyM_Area_KeyM();
        KeyM_SessionOpen = TRUE;
        SchM_Exit_KeyM_Area_KeyM();
    }
    return ret;
}
/*************************************************************************/
/**
 * @brief  This function is used to finalize key update operations.
 */
/*************************************************************************/
/* PRQA S 3673,3408 ++ */ /* VL_KeyM_3673,VL_KeyM_3408 */
Std_ReturnType KeyM_Finalize(
    const uint8* RequestDataPtr,
    uint16       RequestDataLength,
    uint8*       ResponseDataPtr,
    uint16       ResponseMaxDataLength)
/* PRQA S 3673,3408 --*/
{
    uint16          keyIdx;
    uint16          length = ResponseMaxDataLength;
    Std_ReturnType  ret    = E_OK;
    KeyM_ResultType keyRes = KEYM_RT_OK;
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_FINAL_ID, KEYM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if ((NULL_PTR == RequestDataPtr) || (NULL_PTR == ResponseDataPtr))
    {
        KEYM_DET_REPORT(KEYM_FINAL_ID, KEYM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if ((0u == RequestDataLength) || (0u == ResponseMaxDataLength))
    {
        KEYM_DET_REPORT(KEYM_FINAL_ID, KEYM_E_SMALL_BUFFER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
#if (KEYM_CRYPTO_KEYHANDLER_START_FINALIZE_ENABLED == STD_ON)
        ret = KeyM_KH_Finalize(RequestDataPtr, RequestDataLength, ResponseDataPtr, &length);
        /*SWS_KeyM_00104*/
        if ((Std_ReturnType)E_OK == ret)
#else
        (void)RequestDataPtr;
        (void)RequestDataLength;
#endif /*KEYM_CRYPTO_KEYHANDLER_START_FINALIZE_ENABLED == STD_ON*/
        {
            for (keyIdx = 0u; keyIdx < KEYM_KEY_NUM; keyIdx++) /* PRQA S 2877 */ /* VL_KeyM_2877 */
            {
                /*SWS_KeyM_00019*/
                /*SWS_KeyM_00103*/
                SchM_Enter_KeyM_Area_KeyM();
                if ((boolean)TRUE == KeyM_NeedUpdate[keyIdx]) /* PRQA S 2109 */ /* VL_KeyM_2109 */
                {
                    if (E_OK == Csm_KeySetValid(*(KeyM_CryptoKeyCfg[keyIdx].keyTargetRef)))
                    {
#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
                        /* PRQA S 2109 ++*/ /* VL_KeyM_2109 */
                        if ((boolean)TRUE == KeyM_SheKeyFlag)
                        /* PRQA S 2109 --*/
                        {
                            /* PRQA S 0311 ++ */ /* VL_KeyM_0331 */
                            (void)KeyM_GetSHEKey_M4M5(
                                *(uint32*)(KeyM_CryptoKeyCfg[keyIdx].keyTargetRef),
                                ResponseDataPtr,
                                &length);
                            /* PRQA S 0311 -- */
                        }
#endif /*STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED*/
                        ret = E_OK;
                    }
                    else
                    {
                        keyRes = KEYM_RT_NOT_OK;
                        ret    = E_NOT_OK;
                    }
                    KeyM_NeedUpdate[keyIdx] = FALSE;
                }
                SchM_Exit_KeyM_Area_KeyM();
            }
            KeyM_CryptoKeyFinalizeCallbackNotification(keyRes, length, ResponseDataPtr);
        }
        /*SWS_KeyM_00106*/
        SchM_Enter_KeyM_Area_KeyM();
        KeyM_SessionOpen = FALSE;
        SchM_Exit_KeyM_Area_KeyM();
    }
    return ret;
}
#endif /*KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON*/

/*SWS_KeyM_00010*/
#if (STD_ON == KEYM_CRYPTO_KEY_PREPARE_FUNCTION_ENABLED)
/*************************************************************************/
/**
 * @brief  This function is used to prepare a key update operation.
 */
/*************************************************************************/
/* PRQA S 3673,3408 ++ */ /* VL_KeyM_3673,VL_KeyM_3408 */
Std_ReturnType
    KeyM_Prepare(const uint8* RequestData, uint16 RequestDataLength, uint8* ResponseData, uint16* ResponseDataLength)
/* PRQA S 3673,3408 -- */
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_PREPARE_ID, KEYM_E_UNINIT);
    }
    else if ((NULL_PTR == RequestData) || (NULL_PTR == ResponseData))
    {
        KEYM_DET_REPORT(KEYM_PREPARE_ID, KEYM_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
    {
#if (STD_ON == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED)
        if (KeyM_SessionOpen) /* PRQA S 2109 */ /* VL_KeyM_2109 */
#endif                                          /* STD_ON == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED */
        {
/*SWS_KeyM_00090*/
#if (STD_ON == KEYM_CRYPTO_KEYHANDLER_PREPARE_ENABLED)
            /*SWS_KeyM_00011*/
            ret = KeyM_KH_Prepare(RequestData, RequestDataLength, ResponseData, ResponseDataLength);
#else
            (void)RequestData;
            (void)RequestDataLength;
            (void)ResponseData;
            (void)ResponseDataLength;

#endif /* STD_ON == KEYM_CRYPTO_KEYHANDLER_PREPARE_ENABLED */
        }
    }
    return ret;
}
#endif /*STD_ON == KEYM_CRYPTO_KEY_PREPARE_FUNCTION_ENABLED*/
/*************************************************************************/
/**
 * @brief   This function is used to initiate the key generation or update process.
 */
/*************************************************************************/
/* PRQA S 3673,6040 ++*/ /* VL_KeyM_3673,VL_MTR_KeyM_STPAR */
Std_ReturnType KeyM_Update(
    const uint8* KeyNamePtr,
    uint16       KeyNameLength,
    const uint8* RequestDataPtr,
    uint16       RequestDataLength,
    uint8*       ResultDataPtr,
    uint16       ResultDataMaxLength)
/* PRQA S 3673,6040 --*/
{
#if (STD_ON == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
    KeyM_CryptoKeyIdType        keymId;
    KeyM_KH_UpdateOperationType khUpdate;
#endif
    uint16         cfgKeyIdx;
    Std_ReturnType ret = E_NOT_OK;
    uint8          slotId;
    boolean        sheKey = FALSE;

#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_UPDATE_ID, KEYM_E_UNINIT);
    }
    else if ((NULL_PTR == RequestDataPtr) || (NULL_PTR == ResultDataPtr))
    {
        KEYM_DET_REPORT(KEYM_UPDATE_ID, KEYM_E_PARAM_POINTER);
    }
    else
#endif /*STD_ON == KEYM_DEVERROR_DETECT*/
    {
#if (STD_ON == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED)
        if (KeyM_SessionOpen) /* PRQA S 2109 */ /* VL_KeyM_2109 */
#endif                                          /* STD_ON == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED */
        {
            /*Determine whether it is a SHE-Key*/
            /*SWS_KeyM_00154*/
            if (((NULL_PTR == KeyNamePtr) || (KeyNameLength == 0U)) && (RequestDataLength >= KEYM_SHEKEY_INPUT_LEN))
            {
                /* Parameter    Direction   width(bits)
                 * M1           IN          128         -> M1 = UID | SlotID | AuthID
                 * M2           IN          256
                 * M3           IN          128
                 * */
                slotId = (RequestDataPtr[KEYM_SHEKEY_SLOTID_OFFSET] & KEYM_SHEKEY_SLOTID_MASK) >> KEYM_CONST_4;
                for (cfgKeyIdx = 0u; cfgKeyIdx < KEYM_KEY_NUM; cfgKeyIdx++) /* PRQA S 2877 */ /* VL_KeyM_2877 */
                {
                    if ((KeyM_CryptoKeyCfg[cfgKeyIdx].keyProps != NULL_PTR)
                        && (slotId == *(KeyM_CryptoKeyCfg[cfgKeyIdx].keyProps)))
                    {
                        ret = E_OK;
                        break;
                    }
                }
                sheKey = TRUE;
#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
                SchM_Enter_KeyM_Area_KeyM();
                KeyM_SheKeyFlag = TRUE;
                SchM_Exit_KeyM_Area_KeyM();
#endif
            }
            else /*Normal key*/
            {
                /*SWS_KeyM_00091*/
                ret = KeyM_SearchByKeyName(KeyNamePtr, KeyNameLength, &cfgKeyIdx);
#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
                if (ret == E_NOT_OK)
                {
                    /*SWS_KeyM_00155*/
                    ret = KEYM_E_PARAMETER_MISMATCH;
                }
#endif
#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
                SchM_Enter_KeyM_Area_KeyM();
                KeyM_SheKeyFlag = FALSE;
                SchM_Exit_KeyM_Area_KeyM();
#endif
            }
#if (STD_ON == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
            keymId = (ret == E_OK) ? KeyM_CryptoKeyCfg[cfgKeyIdx].keyId : 0xFFFF;
            do
            {
                ret = KeyM_KH_Update(
                    KeyNamePtr,
                    KeyNameLength,
                    RequestDataPtr,
                    RequestDataLength,
                    ResultDataPtr,
                    &ResultDataMaxLength,
                    &keymId,
                    &khUpdate);
                if (ret == E_OK)
                {
                    ret = KeyM_HandleUpdate(ResultDataPtr, ResultDataMaxLength, cfgKeyIdx, sheKey);
                }
            } while (khUpdate == KEYM_KH_UPDATE_KEY_UPDATE_REPEAT);
#else  /*No key handler is configured*/
            if (ret == E_OK)
            {
                ret = KeyM_HandleUpdate(ResultDataPtr, ResultDataMaxLength, cfgKeyIdx, sheKey);
            }
#endif /*STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED*/
        }
    }
    return ret;
}
/* PRQA S 1505 ++*/ /* VL_KeyM_1505 */
Std_ReturnType KeyM_HandleUpdate(
    /* PRQA S 1505 --*/
    const uint8* ResultDataPtr,
    uint16       ResultDataLength,
    uint16       KeyIdx,
    boolean      sheKey)
{
#if (KEYM_NVM_BLOCK_NUM > 0u)
    uint16 blkIdx;
#endif
    boolean         updateCsmFlag = FALSE;
    Std_ReturnType  ret;
    KeyM_ResultType keyMRes;

    /*Store or derive Key according to the configuration*/
    if (KEYM_STORED_KEY == KeyM_CryptoKeyCfg[KeyIdx].keyGenType)
    {
        /*SWS_KeyM_00098*/
        switch (KeyM_CryptoKeyCfg[KeyIdx].keyStorage)
        {
        case KEYM_STORAGE_IN_CSM:
        case KEYM_STORAGE_IN_RAM:
            /* SWS_KeyM_00016
             * SWS_KeyM_00099
             * */
            ret = Csm_KeyElementSet(
                *(KeyM_CryptoKeyCfg[KeyIdx].keyTargetRef),
                KEYM_CONST_1,
                ResultDataPtr,
                (uint32)ResultDataLength);
            updateCsmFlag = TRUE;
            break;
#if (KEYM_NVM_BLOCK_NUM > 0u)
        case KEYM_STORAGE_IN_NVM:
            blkIdx = *(KeyM_CryptoKeyCfg[KeyIdx].nvmBlkRef);
            if (KeyM_NvmBlockPCfg[blkIdx].blkWriDelay == 0u)
            {
                /*Write immediately*/
                ret = NvM_WriteBlock(KeyM_NvmBlockPCfg[blkIdx].blkId, ResultDataPtr);
            }
            break;
#endif
        default:
            ret = KEYM_E_PARAMETER_MISMATCH;
            break;
        }
    }
    else /*KEYM_DERIVED_KEY*/
    {
        /*SWS_KeyM_00100*/
        /*SWS_KeyM_00017*/
        /* PRQA S 1258 ++ */ /* VL_KeyM_1258 */
        ret = Csm_KeyElementSet(
            *(KeyM_CryptoKeyCfg[KeyIdx].keySrcDervRef),
            CRYPTO_KE_KEYDERIVATION_PASSWORD,
            ResultDataPtr,
            (uint32)ResultDataLength);
        if ((ret == E_OK) && (NULL_PTR != KeyM_CryptoKeyCfg[KeyIdx].keyGenInfo))
        {
            ret = Csm_KeyElementSet(
                *(KeyM_CryptoKeyCfg[KeyIdx].keyTargetRef),
                CRYPTO_KE_KEYDERIVATION_SALT,
                KeyM_CryptoKeyCfg[KeyIdx].keyGenInfo,
                KeyM_CryptoKeyCfg[KeyIdx].keyGenInfoLen);
        }
        /* PRQA S 1258 -- */
        updateCsmFlag = TRUE;
        /*Derivation Key*/
        if (ret == E_OK)
        {
            ret = Csm_KeyDerive(*(KeyM_CryptoKeyCfg[KeyIdx].keySrcDervRef), *(KeyM_CryptoKeyCfg[KeyIdx].keyTargetRef));
        }
    }
    if (updateCsmFlag) /* PRQA S 2109 */ /* VL_KeyM_2109 */
    {
#if (STD_ON == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED)
        /*SWS_KeyM_00102:Internal marker will be set for this key*/
        SchM_Enter_KeyM_Area_KeyM();
        KeyM_NeedUpdate[KeyIdx] = TRUE;
        SchM_Exit_KeyM_Area_KeyM();
        (void)sheKey;
#else
        /*SWS_KeyM_00101*/
        /*SWS_KeyM_00018*/
        (void)Csm_KeySetValid(*(KeyM_CryptoKeyCfg[KeyIdx].keyTargetRef));
#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
        if (TRUE == sheKey)
        {
            KeyM_GetSHEKey_M4M5(
                *(uint32*)(KeyM_CryptoKeyCfg[KeyIdx].keyTargetRef),
                (uint8*)ResultDataPtr,
                &ResultDataLength);
        }
#endif
#endif /*STD_OFF == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED*/
    }
    keyMRes = (ret != E_OK) ? KEYM_RT_NOT_OK : KEYM_RT_OK;
    KeyM_CryptoKeyUpdateCallbackNotification(keyMRes, ResultDataLength, ResultDataPtr);
    return ret;
}

#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
/* PRQA S 1505 ++ */ /* VL_KeyM_1505 */
Std_ReturnType KeyM_GetSHEKey_M4M5(
    /* PRQA S 1505 -- */
    uint32  keyId,
    uint8*  ResponseDataPtr,
    uint16* ResponseMaxDataLength)
{
    Std_ReturnType ret;
    uint32         keyEleLen = *ResponseMaxDataLength;

    /* M4 = M1 | M4*,M4 * = Encrypt_ECB(K3(CID))
     * */
    /* M5 = CMACK4 (M4)
     * K4 = AES128(Knew | KEY_UPDATE_MAC_C ) ,
     * Where KEY_UPDATE_MAC_C is Constant
     * Knew is the Value of the new key used for the Key Update.
     * */
    /* PRQA S 1258 ++ */ /* VL_KeyM_1258 */
    ret = Csm_KeyElementGet(keyId, CRYPTO_KE_MAC_PROOF, ResponseDataPtr, &keyEleLen);
    /* PRQA S 1258 -- */
    *ResponseMaxDataLength = (uint16)keyEleLen;

    return ret;
}
#endif /*STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED*/

/*SWS_KeyM_00107*/
#if (KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED == STD_ON)
/*************************************************************************/
/**
 * @brief  This function is used to initiate the key generation or update process.
 */
/*************************************************************************/
/* PRQA S 3408,0310,3305,6040 ++ */ /* VL_KeyM_3408,VL_KeyM_0310,VL_KeyM_3305,VL_MTR_KeyM_STPAR */
Std_ReturnType KeyM_Verify(
    const uint8* KeyNamePtr,
    uint16       KeyNameLength,
    const uint8* RequestData,
    uint16       RequestDataLength,
    uint8*       ResponseData,
    uint16*      ResponseDataLength)
{
    uint32         verJob;
    uint16         cfgKeyIdx;
    Std_ReturnType ret;
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_VERIFY_ID, KEYM_E_UNINIT);
        ret = E_NOT_OK;
    }
    else if ((NULL_PTR == KeyNamePtr) || (NULL_PTR == RequestData) || (NULL_PTR == ResponseData))
    {
        KEYM_DET_REPORT(KEYM_VERIFY_ID, KEYM_E_PARAM_POINTER);
        ret = E_NOT_OK;
    }
    else if ((0u == RequestDataLength) || (0u == *ResponseDataLength))
    {
        KEYM_DET_REPORT(KEYM_VERIFY_ID, KEYM_E_SMALL_BUFFER);
        ret = E_NOT_OK;
    }
    else
#endif
    {
        /*SWS_KeyM_00108:Syn-mode*/
        ret = KeyM_SearchByKeyName(KeyNamePtr, KeyNameLength, &cfgKeyIdx);
        if ((Std_ReturnType)E_OK == ret)
        {
            uint32 responseDataLength = (uint32)(*ResponseDataLength);
            verJob                    = KeyM_CryptoKeyCfg[cfgKeyIdx].verifyJobRef;
            switch (KeyM_CryptoKeyCfg[cfgKeyIdx].verJobType)
            {
            case KEYM_VERIFY_ENCRYPT:
                ret = Csm_Encrypt(
                    verJob,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    RequestData,
                    (uint32)RequestDataLength,
                    ResponseData,
                    &responseDataLength); /* Note:Ignore the warning*/
                *ResponseDataLength = (uint16)responseDataLength;
                break;
            case KEYM_VERIFY_DECRYPT:
                ret = Csm_Decrypt(
                    verJob,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    RequestData,
                    (uint32)RequestDataLength,
                    ResponseData,
                    &responseDataLength); /* Note:Ignore the warning*/
                *ResponseDataLength = (uint16)responseDataLength;
                break;
            case KEYM_VERIFY_MACGENERATE:
                ret = Csm_MacGenerate(
                    verJob,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    RequestData,
                    (uint32)RequestDataLength,
                    ResponseData,
                    &responseDataLength); /* Note:Ignore the warning*/
                *ResponseDataLength = (uint16)responseDataLength;
                break;
            case KEYM_VERIFY_AEADENCRYPT:
            case KEYM_VERIFY_AEADDECRYPT:
            case KEYM_VERIFY_MACVERIFY:
#if (KEYM_CRYPTO_KEYHANDLER_VERIFY_ENABLED == STD_ON)
                /*Handling multiple input parameters*/
                ret = KeyM_KH_Verify(
                    KeyNamePtr,
                    KeyNameLength,
                    RequestData,
                    RequestDataLength,
                    ResponseData,
                    ResponseDataLength);
#endif /*KEYM_CRYPTO_KEYHANDLER_VERIFY_ENABLED == STD_ON*/
                break;
            default:
                /* Intentionally Empty */
                break;
            }
#if (KEYM_CRYPTO_KEY_VERIFY_ASYNCMODE == STD_ON)
            if (ret != E_OK)
            {
                ret = KEYM_E_PENDING;
            }
#endif
        }
    }
    return ret;
}
/* PRQA S 3408,0310,3305,6040 -- */
#endif /*KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED == STD_ON*/
#define KEYM_STOP_SEC_CODE
#include "KeyM_MemMap.h"

/* ========================================== internal function definitions ========================================= */
KEYM_LOCAL Std_ReturnType KeyM_SearchByKeyName(const uint8* KeyNamePtr, uint16 KeyNameLength, uint16* KeyCfgIdx)
{
    uint16         keyIdx;
    uint16         keyPos;
    Std_ReturnType ret = E_NOT_OK;

    *KeyCfgIdx = KEYM_KEY_NUM;
    for (keyIdx = 0u; keyIdx < KEYM_KEY_NUM; keyIdx++) /* PRQA S 2877 */ /* VL_KeyM_2877 */
    {
        if (KeyM_CryptoKeyCfg[keyIdx].KeyNameLen != KeyNameLength)
        {
            continue; /* PRQA S 0770 */ /* VL_KeyM_0770 */
        }

        for (keyPos = 0u; keyPos < KeyNameLength; keyPos++)
        {
            if (KeyM_CryptoKeyCfg[keyIdx].keyName[keyPos] != KeyNamePtr[keyPos])
            {
                break;
            }
        }

        if (keyPos == KeyNameLength)
        {
            *KeyCfgIdx = keyIdx;
            ret        = E_OK;
            break;
        }
    }

    return ret;
}

#endif /*KEYM_CCRYPTO_KEYMANAGER_ENABLED == STD_ON*/
/* PRQA S 3432 -- */
/* PRQA S 1532 -- */
/* PRQA S 1503 -- */
/* PRQA S 3206 -- */
