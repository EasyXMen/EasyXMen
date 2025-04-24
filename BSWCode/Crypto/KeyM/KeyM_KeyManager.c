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
**  FILENAME    : KeyM_KeyManager.c                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for key sub module                           **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R21-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0     2022-02-15   qinchun.yang     initial version.
 *  V1.0.1     2023-03-15   qinchun.yang     Key_Update change to synchronization.
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:KeyM<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code

    \li PRQA S 3206 MISRA Rule 2.7 .<br>
    Reason:Parameter is necessary in AUTOSAR specification

    \li PRQA S 1331 MISRA Rule 2.2 .<br>
    Reason: sometimes the retuen value needs init value

    \li PRQA S 4461 MISRA Dir 4.1 .<br>
    Reason:Loop can be executed more than once with different configuration

    \li PRQA S 1532 MISRA Rule 8.7,Rule 2.1 .<br>
   Reason:Functions may be used in other projects.
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
/* PRQA S 1503 ++ */ /* MISRA Rule 2.1 */
/* PRQA S 3206 ++ */ /* MISRA Rule 2.7 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "KeyM_Internal.h"
/*SWS_KeyM_00001*/
#if (KEYM_CRYPTO_KEYMANAGER_ENABLED == STD_ON)
#include "KeyM_Externals.h"
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_SearchByKeyName(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) KeyNamePtr,
    VAR(uint16, AUTOMATIC) KeyNameLength,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_CONST) KeyCfgIdx);
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#if (KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON)
#define KEYM_START_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"

KEYM_LOCAL VAR(boolean, KEYM_VAR) KeyM_NeedUpate[KEYM_KEY_NUM];
#define KEYM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
#endif

#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
#define KEYM_START_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
KEYM_LOCAL boolean KeyM_SheKeyFlag;
#define KEYM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
#endif

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/
#if (KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON)
#define KEYM_START_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
VAR(boolean, KEYM_VAR) KeyM_SessionOpen; /* PRQA S 1504*/ /* MISRA Rule 8.7 */
#define KEYM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
#endif
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define KEYM_START_SEC_CODE
#include "KeyM_MemMap.h"
#if (KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED == STD_ON)
/*
 * Brief               This function intents to allow key update operation.
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      StartType: Defines in which mode the key operation shall be executed.
 *                     RequestData: Information that comes along with the request.
 *                     RequestDataLength: length of request data.
 * Param-Name[out]     ResponseData: Data returned by the function.
 * Param-Name[in/out]  ResponseDataLength:
 *                            In: Max number of bytes available in ResponseData
 *                            Out: Actual number
 * Return              None
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_Start(
    VAR(KeyM_StartType, AUTOMATIC) StartType,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestData,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    /* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength)
/* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* MISRA Rule 2.2 */

#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
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

/*
 * Brief               This function is used to finalize key update operations.
 * ServiceId           0x07
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RequestData: Information that comes along with the request.
 *                     RequestDataLength: length of request data.
 * Param-Name[out]     ResponseData: Data returned by the function.
 * Param-Name[in/out]  ResponseDataLength:
 *                            In: Max number of bytes available in ResponseData
 *                            Out: Actual number  of bytes in ResponseData or left untouched if
 *                                 service runs in asynchronous mode and function returns KEYM_E_OK.
 * Return              Std_ReturnType
 *                     E_OK: Operation has been accepted and will be processed internally.
 *                           Results will be provided through a callback
 *                     E_NOT_OK: Operation not accepted due to an internal error.
 *                     KEYM_E_BUSY: Validation cannot be performed yet. KeyM is currently busy with
 *                           other jobs.
 *                     KEYM_E_PARAMETER_MISMATCH: Parameter do not match with expected value.
 *                     KEYM_E_KEY_CERT_SIZE_MISMATCH: Parameter size doesn't match.
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_Finalize(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestDataPtr,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) ResponseDataPtr,
    VAR(uint16, AUTOMATIC) ResponseMaxDataLength)
{
    uint16 keyIdx;
    uint16 length = ResponseMaxDataLength;
    Std_ReturnType ret = E_OK;
    KeyM_ResultType keyRes = KEYM_RT_OK;
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
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
            for (keyIdx = 0u; keyIdx < KEYM_KEY_NUM; keyIdx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
            {
                /*SWS_KeyM_00019*/
                /*SWS_KeyM_00103*/
                SchM_Enter_KeyM_Area_KeyM();
                if ((boolean)TRUE == KeyM_NeedUpate[keyIdx])
                {
                    if (E_OK == Csm_KeySetValid(*(KeyM_CryptoKeyCfg[keyIdx].keyTargetRef)))
                    {
#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
                        if (TRUE == KeyM_SheKeyFlag)
                        {

                            (void)KeyM_GetSHEKey_M4M5(
                                *(const uint32*)(KeyM_CryptoKeyCfg[keyIdx].keyTargetRef),
                                ResponseDataPtr,
                                &length);
                        }
#endif /*STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED*/
                        ret = E_OK;
                    }
                    else
                    {
                        keyRes = KEYM_RT_NOT_OK;
                        ret = E_NOT_OK;
                    }
                    KeyM_NeedUpate[keyIdx] = FALSE;
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
/*
 * Brief               This function is used to prepare a key update operation.
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      RequestData: Information that comes along with the request.
 *                     RequestDataLength: length of request data.
 * Param-Name[out]     ResponseData: Data returned by the function.0
 * Param-Name[in/out]  ResponseDataLength:
 *                            In: Max number of bytes available in ResponseData
 *                            Out: Actual number
 * Return              Std_ReturnType
 *                     E_OK: Service has been accepted and will be processed internally.
 *                           Results will be provided through a callback.
 *                     E_NOT_OK: Service not accepted due to an internal error.
 *                     KEYM_E_PARAMETER_MISMATCH: Parameter do not match with expected value.
 *                     KEYM_E_KEY_CERT_SIZE_MISMATCH: Parameter size doesn't match
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_Prepare(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestData,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    /* PRQA S 3673 ++*/ /* MISRA Rule 8.13 */
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    /* PRQA S 3673 --*/                                                            /* MISRA Rule 8.13 */
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength) /* PRQA S 3673 */ /* MISRA Rule 8.13 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
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
        if ((boolean)TRUE == KeyM_SessionOpen)
#endif /* STD_ON == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED */
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
/*
 * Brief                This function is used to initiate the key generation or update process.
 * ServiceId            0x06
 * Sync/Async           Asynchronous
 * Reentrancy           Non Reentrant
 * Param-Name[in]       KeyNamePtr:Pointer to an array that defines the name of the key to be updated
 *                      KeyNameLength:Specifies the number of bytes in keyName. The value 0 indicates
 *                      that no keyName is provided within this function.
 *                      RequestDataPtr:Information that comes along with the request
 *                      RequestData:Length Length of data in the RequestData array
 *                      ResultDataMax:Length Max number of bytes available in ResultDataPtr.
 * Param-Name[out]      ResultDataPtr:Pointer to a data buffer used by the function to store results.
 * Param-Name[in/out]   None
 * Return               Std_ReturnType
 *                      E_OK: Service has been accepted and will be processed internally. Results will be provided
 * through a callback E_NOT_OK: Service not accepted due to an internal error. E_BUSY: Service could not be accepted
 * because another operation is already ongoing. Try next time. KEYM_E_PARAMETER_MISMATCH: Parameter do not match with
 * expected value. KEYM_E_KEY_CERT_SIZE_MISMATCH: Parameter size doesn't match
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_Update(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) KeyNamePtr,
    VAR(uint16, AUTOMATIC) KeyNameLength,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestDataPtr,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    /* PRQA S 3673 ++*/ /* MISRA Rule 8.13 */
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr,
    /* PRQA S 3673 --*/ /* MISRA Rule 8.13 */
    VAR(uint16, AUTOMATIC) ResultDataMaxLength)
{
#if (STD_ON == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
    KeyM_CryptoKeyIdType keymId;
    KeyM_KH_UpdateOperationType khUpdate;
#endif
    uint16 cfgKeyIdx;
    Std_ReturnType ret = E_NOT_OK;
    uint8 slotId;
    boolean sheKey = FALSE;

#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
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
        if ((boolean)TRUE == KeyM_SessionOpen)
#endif /* STD_ON == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED */
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
                for (cfgKeyIdx = 0u; cfgKeyIdx < KEYM_KEY_NUM; cfgKeyIdx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
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
/* PRQA S 1505 ++*/ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_HandleUpdate(
    /* PRQA S 1505 --*/ /* MISRA Rule 8.7 */
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr,
    uint16 ResultDataLength,
    uint16 KeyIdx,
    boolean sheKey)
{
#if (KEYM_NVM_BLOCK_NUM > 0u)
    uint16 blkIdx;
#endif
    boolean updateCsmFlag = FALSE;
    Std_ReturnType ret;
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
        updateCsmFlag = TRUE;
        /*Derivation Key*/
        if (ret == E_OK)
        {
            ret = Csm_KeyDerive(*(KeyM_CryptoKeyCfg[KeyIdx].keySrcDervRef), *(KeyM_CryptoKeyCfg[KeyIdx].keyTargetRef));
        }
    }
    if ((boolean)TRUE == updateCsmFlag)
    {
#if (STD_ON == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED)
        /*SWS_KeyM_00102:Internal marker will be set for this key*/
        SchM_Enter_KeyM_Area_KeyM();
        KeyM_NeedUpate[KeyIdx] = TRUE;
        SchM_Exit_KeyM_Area_KeyM();
        (void)sheKey;
#else
        /*SWS_KeyM_00101*/
        /*SWS_KeyM_00018*/
        (void)Csm_KeySetValid(*(KeyM_CryptoKeyCfg[KeyIdx].keyTargetRef));
        if (TRUE == sheKey)
        {
            KeyM_GetSHEKey_M4M5(
                *(uint32*)(KeyM_CryptoKeyCfg[KeyIdx].keyTargetRef),
                (uint8*)ResultDataPtr,
                &ResultDataLength);
        }
#endif /*STD_OFF == KEYM_CRYPTO_KEY_START_FINALIZE_FUNCTION_ENABLED*/
    }
    keyMRes = (ret != E_OK) ? KEYM_RT_NOT_OK : KEYM_RT_OK;
    KeyM_CryptoKeyUpdateCallbackNotification(keyMRes, ResultDataLength, ResultDataPtr);
    return ret;
}

#if (STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED)
/* PRQA S 1505 ++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_GetSHEKey_M4M5(
    /* PRQA S 1505 -- */ /* MISRA Rule 8.7 */
    uint32 keyId,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) ResponseDataPtr,
    P2VAR(uint16, AUTOMATIC, AUTOMATIC) ResponseMaxDataLength)
{
    Std_ReturnType ret;
    uint32 keyEleLen = *ResponseMaxDataLength;

    /* M4 = M1 | M4*,M4 * = Encrypt_ECB(K3(CID))
     * */
    /* M5 = CMACK4 (M4)
     * K4 = AES128(Knew | KEY_UPDATE_MAC_C ) ,
     * Where KEY_UPDATE_MAC_C is Constant
     * Knew is the Value of the new key used for the Key Update.
     * */
    ret = Csm_KeyElementGet(keyId, CRYPTO_KE_MAC_PROOF, ResponseDataPtr, &keyEleLen);
    *ResponseMaxDataLength = (uint16)keyEleLen;

    return ret;
}
#endif /*STD_OFF == KEYM_CRYPTO_KEYHANDLER_UPDATE_ENABLED*/

/*SWS_KeyM_00107*/
#if (KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED == STD_ON)
/*
 * Brief               This function is used to initiate the key generation or update process.
 * ServiceId           0x08
 * Sync/Async          Synchronous/Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      KeyNamePtr: Pointer to an array that defines the name of the key to
 *                                 be updated.
 *                     KeyNameLength: Specifies the number of bytes in keyName. The value 0 indicates
 *                                    that no keyName is provided within this function.
 *                     RequestDataPtr: Information that comes along with the request
 *                     RequestDataLength: Length of data in the RequestData array
 * Param-Name[out]     ResponseData: Data returned by the function.
 * Param-Name[in/out]  ResponseDataLength:
 *                      in: Max number of bytes available in ResponseData
 *                      out: Actual number of bytes in ResponseData or left untouched if service
 *                           runs in asynchronous mode and function returns KEYM_E_PENDING
 * Return              Std_ReturnType
 *                     KEYM_E_PENDING: Operation runs in asynchronous mode, has been accepted and will be processed
 * internally. Results will be provided through callback E_OK: Operation was successfully performed. Result information
 * are available. E_NOT_OK: Operation not accepted due to an internal error. KEYM_E_BUSY: Validation cannot be performed
 * yet. KeyM is currently busy with other jobs (for asynchronous mode). KEYM_E_PARAMETER_MISMATCH: Parameter do not
 * match with expected value. KEYM_E_KEY_CERT_SIZE_MISMATCH: Parameter size doesn't match KEYM_E_KEY_CERT_INVALID: Key
 * operation cannot be performed because the key name is invalid. KEYM_E_KEY_CERT_EMPTY: The key for this slot has not
 * been set.
 *
 */
/*************************************************************************/
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_Verify(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) KeyNamePtr,
    VAR(uint16, AUTOMATIC) KeyNameLength,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) RequestData,
    VAR(uint16, AUTOMATIC) RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength)
{
    uint32 verJob;
    uint16 cfgKeyIdx;
    Std_ReturnType ret;
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
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
            verJob = KeyM_CryptoKeyCfg[cfgKeyIdx].verifyJobRef;
            switch (KeyM_CryptoKeyCfg[cfgKeyIdx].verJobType)
            {
            case KEYM_VERIFY_ENCRYPT:
                ret = Csm_Encrypt(
                    verJob,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    RequestData,
                    (uint32)RequestDataLength,
                    ResponseData,
                    (uint32*)ResponseDataLength);
                break;
            case KEYM_VERIFY_DECRYPT:
                ret = Csm_Decrypt(
                    verJob,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    RequestData,
                    (uint32)RequestDataLength,
                    ResponseData,
                    (uint32*)ResponseDataLength);
                break;
            case KEYM_VERIFY_MACGENERATE:
                ret = Csm_MacGenerate(
                    verJob,
                    CRYPTO_OPERATIONMODE_SINGLECALL,
                    RequestData,
                    (uint32)RequestDataLength,
                    ResponseData,
                    (uint32*)ResponseDataLength);
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
#endif /*KEYM_CRYPTO_KEY_VERIFY_FUNCTION_ENABLED == STD_ON*/
#define KEYM_STOP_SEC_CODE
#include "KeyM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
KEYM_LOCAL FUNC(Std_ReturnType, KEYM_CODE) KeyM_SearchByKeyName(
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_CONST) KeyNamePtr,
    VAR(uint16, AUTOMATIC) KeyNameLength,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_CONST) KeyCfgIdx)
{
    uint16 keyIdx;
    uint16 keyPos;
    Std_ReturnType ret = E_NOT_OK;

    *KeyCfgIdx = KEYM_KEY_NUM;
    for (keyIdx = 0u; keyIdx < KEYM_KEY_NUM; keyIdx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        if (KeyM_CryptoKeyCfg[keyIdx].KeyNameLen != KeyNameLength)
        {
            continue;
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
            ret = E_OK;
            break;
        }
    }

    return ret;
}

#endif               /*KEYM_CRYPTO_KEYMANAGER_ENABLED == STD_ON*/
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
/* PRQA S 1503 -- */ /* MISRA Rule 2.1 */
/* PRQA S 3206 -- */ /* MISRA Rule 2.7 */
