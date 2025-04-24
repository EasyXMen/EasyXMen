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
**  FILENAME    : CryIf.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Implementation for CryIf                                    **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>     <AUTHOR>        <REVISION LOG>
 *  V1.0.0      2020-05-29  yuzhe.zhang      Initial version
 *  V2.0.0      2023-02-06  jie.gu           R19-11
 *  V2.1.0      2023-07-17  jie.gu           CP2.1 Release Version
 *  V2.1.1      2023-11-22  jie.gu           CPD-33590 Code execution efficiency optimization
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Cryif<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code

    \li PRQA S 1532 MISRA Rule 8.7 .<br>
    Reason:Functions may be used in other projects.

    \li PRQA S 1532 MISRA Rule 8.9 .<br>
    Reason:Static value can be used in other function

    \li PRQA S 1532 MISRA Rule 2.2 .<br>
    Reason:Return value needs initialized under certain configuration

 */
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "CryIf_Cbk.h"
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#include "Csm_Cbk.h"
#include "Csm_Types.h"
#include "Crypto.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define CRYIF_C_AR_MAJOR_VERSION 4U
#define CRYIF_C_AR_MINOR_VERSION 5U
#define CRYIF_C_AR_PATCH_VERSION 0U
#define CRYIF_C_SW_MAJOR_VERSION 2U
#define CRYIF_C_SW_MINOR_VERSION 1U
#define CRYIF_C_SW_PATCH_VERSION 1U

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (                                                                                                                \
    (CRYIF_C_AR_MAJOR_VERSION != CRYIF_H_AR_MAJOR_VERSION) || (CRYIF_C_AR_MINOR_VERSION != CRYIF_H_AR_MINOR_VERSION) \
    || (CRYIF_C_AR_PATCH_VERSION != CRYIF_H_AR_PATCH_VERSION)                                                        \
    || (CRYIF_C_SW_MAJOR_VERSION != CRYIF_H_SW_MAJOR_VERSION)                                                        \
    || (CRYIF_C_SW_MINOR_VERSION != CRYIF_H_SW_MINOR_VERSION)                                                        \
    || (CRYIF_C_SW_PATCH_VERSION != CRYIF_H_SW_PATCH_VERSION))

#error "CryIf.c version mismatching with CryIf.h"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
static inline void CRYIF_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
static FUNC(uint8, CRYIF_CODE) CryIf_GetDriverId(VAR(uint32, AUTOMATIC) keyid);
#endif
static FUNC(uint32, CRYIF_CODE) CryIf_GetDriverKeyId(VAR(uint32, AUTOMATIC) keyid);
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*Module initialize status, (boolean)TRUE initialized, (boolean)FALSE not initialized*/
#define CRYIF_START_SEC_VAR_CLEARED_BOOLEAN
#include "CryIf_MemMap.h"
static VAR(boolean, CRYIF_VAR_CLEARED_BOOLEAN) CryIf_InitStatus; /*PRQA S 3218*/
#define CRYIF_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "CryIf_MemMap.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/**
 * Initializes the CryIf module.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @configPtr, Pointer to a selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
/* PRQA S 1503 ++ */ /* MISRA Rule 2.1 */
FUNC(void, CRYIF_CODE)
CryIf_Init(P2CONST(CryIf_ConfigType, AUTOMATIC, CRYIF_APPL_DATA) configPtr)
{
/* PRQA S 1503 -- */ /* MISRA Rule 2.1 */
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_91019] */
    if (configPtr != NULL_PTR)
    {
        /* @req [SWS_CryIf_00014] */
        CRYIF_DET_REPORT(CRYIF_SID_INIT, CRYIF_E_INIT_FAILED);
    }
    else
#else
    (void)configPtr;
#endif
    {
        /* @req [SWS_CryIf_00015] */
        CryIf_InitStatus = (boolean)TRUE;
    }
    return;
}
#if (STD_ON == CRYIF_VERSION_INFO_API)
/**
 * Returns the version information of this module.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @versioninfo, Pointer to where to store the version information of this module
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
FUNC(void, CRYIF_CODE)
CryIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CRYIF_APPL_DATA) versioninfo)
{
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00016] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_GETVERSIONINFO, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00017] */
    else if (NULL_PTR == versioninfo)
    {
        CRYIF_DET_REPORT(CRYIF_SID_GETVERSIONINFO, CRYIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        versioninfo->moduleID = CRYIF_MODULE_ID;
        versioninfo->vendorID = CRYIF_VENDOR_ID;
        versioninfo->sw_major_version = CRYIF_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CRYIF_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CRYIF_H_SW_PATCH_VERSION;
    }
    return;
}
#endif
/**
 * This interface dispatches the received jobs to the configured crypto driver object.
 * Service ID: 0x03
 * Sync/Async: Sync or Async, depends on the configuration
 * Reentrancy: Reentrant
 * Parameters(IN): @channelId, Holds the identifier of the crypto channel.
 * Parameters(INOUT): @job, Pointer to the configuration of the job. Contains structures with user and primitive
 * relevant information. Parameters(OUT): NA Return value: E_OK: Request successful E_NOT_OK: Request failed
 *               CRYPTO_E_BUSY: Request failed, Crypro Driver Object is busy
 *               CRYPTO_E_KEY_NOT_VALID: Request failed, the key is not valid
 *               CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, a key element has the wrong size
 *               CRYPTO_E_KEY_READ_FAIL: The service request failed,because key element extraction is not allowed
 *               CRYPTO_E_KEY_WRITE_FAIL: The service request failed because the writing access failed
 *               CRYPTO_E_KEY_NOT_AVAILABLE: The service request failed because the key is not available
 *               CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *               CRYPTO_E_JOB_CANCELED: The service request failed because the synchronous Job has been canceled
 *               CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */
FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_ProcessJob(VAR(uint32, CRYIF_APPL_DATA) channelId, P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job)
{
    Std_ReturnType ret = E_NOT_OK;
    Crypto_ServiceInfoType Serviceid;
    P2CONST(Crypto_JobPrimitiveInfoType, AUTOMATIC, CRYIF_APPL_DATA) JobPrimitiveInfo = job->jobPrimitiveInfo;
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00027] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_PROCESSJOB, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00028] */
    else if (channelId >= CRYIF_CHANNELS_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_PROCESSJOB, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00029] */
    else if (NULL_PTR == job)
    {
        CRYIF_DET_REPORT(CRYIF_SID_PROCESSJOB, CRYIF_E_PARAM_POINTER);
    }
    /* @req [SWS_CryIf_00133] */
    else
#endif
    {

        Serviceid = JobPrimitiveInfo->primitiveInfo->service;
        if (CRYPTO_HASH == Serviceid)
        {
            ret = E_OK;
        }
        else if (
            (CRYPTO_MACGENERATE == Serviceid) || (CRYPTO_MACVERIFY == Serviceid) || (CRYPTO_ENCRYPT == Serviceid)
            || (CRYPTO_DECRYPT == Serviceid) || (CRYPTO_AEADENCRYPT == Serviceid) || (CRYPTO_AEADDECRYPT == Serviceid)
            || (CRYPTO_RANDOMGENERATE == Serviceid) || (CRYPTO_SIGNATUREGENERATE == Serviceid)
            || (CRYPTO_SIGNATUREVERIFY == Serviceid) || (CRYPTO_KEYSETVALID == Serviceid)
            || (CRYPTO_RANDOMSEED == Serviceid) || (CRYPTO_KEYGENERATE == Serviceid) || (CRYPTO_KEYDERIVE == Serviceid)
            || (CRYPTO_KEYEXCHANGECALCPUBVAL == Serviceid) || (CRYPTO_KEYEXCHANGECALCSECRET == Serviceid))
        {
            if (JobPrimitiveInfo->cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
            {
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
                CRYIF_DET_REPORT(CRYIF_SID_PROCESSJOB, CRYIF_E_PARAM_HANDLE);
#endif
            }
            else
            {
                job->cryptoKeyId = CryIf_GetDriverKeyId(JobPrimitiveInfo->cryIfKeyId);
                ret = E_OK;
            }
            /* @req [SWS_CryIf_00135] */
            if (CRYPTO_KEYDERIVE == Serviceid)
            {
                if (job->jobPrimitiveInputOutput.targetCryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
                {
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
                    CRYIF_DET_REPORT(CRYIF_SID_PROCESSJOB, CRYIF_E_PARAM_HANDLE);
#endif
                }
                else
                {
                    job->targetCryptoKeyId = CryIf_GetDriverKeyId(job->targetCryptoKeyId);
                    ret = E_OK;
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
        }
        if (E_OK == ret)
        {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
            if (CryIf_DriverApi[channelId - 1].Crypto_ProcessJob_Name != NULL_PTR)
            {
                ret = CryIf_DriverApi[channelId - 1].Crypto_ProcessJob_Name(channelId, job);
            }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
            ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_ProcessJob_Name(channelId, job);
#endif
        }
    }
    return ret;
}

/**
 * This interface dispatches the job cancellation function to the configured crypto driver object.
 * Service ID: 0x0e
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): @channelId, Holds the identifier of the crypto channel.
 * Parameters(INOUT): @job, Pointer to the configuration of the job. Contains structures with
 user and primitive relevant information.
 * Parameters(OUT): NA
 * Return value: E_OK: Request successful, job has been removed
 *               E_NOT_OK: Request failed, job couldn't be removed
 */
FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_CancelJob(VAR(uint32, CRYIF_APPL_DATA) channelId, P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job)
{
    Std_ReturnType ret = E_NOT_OK; /*PRQA S 2981*/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00129] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_CANCELJOB, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00130] */
    else if (channelId >= CRYIF_CHANNELS_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_CANCELJOB, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00131] */
    else if (NULL_PTR == job)
    {
        CRYIF_DET_REPORT(CRYIF_SID_CANCELJOB, CRYIF_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        if (CryIf_DriverApi[channelId - 1].Crypto_CancelJob_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00132] */
            ret = CryIf_DriverApi[channelId - 1].Crypto_CancelJob_Name(channelId, job);
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_CancelJob_Name(channelId, job);
#endif
    }
    return ret;
}

/**
 * This function shall dispatch the set key element function to the configured crypto driver object.
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be set.
 *          @keyElementId, Holds the identifier of the key element which shall be set.
 *          @keyPtr, Holds the pointer to the key data which shall be set as key element.
 *          @keyLength Contains the length of the key element in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_WRITE_FAIL:Request failed because write access was denied
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed because the key is not available
 *          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element size does not match size of provided data
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyElementSet(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementId,
    P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) keyPtr,
    VAR(uint32, CRYIF_APPL_DATA) keyLength)
{
    Std_ReturnType ret = E_NOT_OK; /*PRQA S 2981*/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00049] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_SET, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00050] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_SET, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00052] */
    else if (NULL_PTR == keyPtr)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_SET, CRYIF_E_PARAM_POINTER);
    }
    /* @req [SWS_CryIf_00053] */
    else if (0u == keyLength)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_SET, CRYIF_E_PARAM_VALUE);
    }
    else if (keyElementId >= CRYIF_KEYELEMENT_MAX_CONFIGURED)
    {
        return ret;
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_KeyElementSet_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00055] */
            ret = CryIf_DriverApi[cryDriverId]
                      .Crypto_KeyElementSet_Name(CryIf_GetDriverKeyId(cryIfKeyId), keyElementId, keyPtr, keyLength);
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER]
                  .Crypto_KeyElementSet_Name(CryIf_GetDriverKeyId(cryIfKeyId), keyElementId, keyPtr, keyLength);
#endif
    }
    return ret;
}

/**
 * This function shall dispatch the set key valid function to the configured crypto driver object.
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): @cryIfKeyId, Holds the identifier of the key whose key elements shall be set to valid.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeySetValid(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId)
{
    Std_ReturnType ret = E_NOT_OK; /*PRQA S 2981*/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00056] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYSETVALID, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00057] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYSETVALID, CRYIF_E_PARAM_HANDLE);
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_KeySetValid_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00058] */
            ret = CryIf_DriverApi[cryDriverId].Crypto_KeySetValid_Name(CryIf_GetDriverKeyId(cryIfKeyId));
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeySetValid_Name(CryIf_GetDriverKeyId(cryIfKeyId));
#endif
    }
    return ret;
}

/**
 * This function shall dispatch the get key element function to the configured crypto driver object.
 * Service ID: 0x06
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be returned.
 *          @keyElementId, Holds the identifier of the key element which shall be returned.
 * Parameters(INOUT): @resultLengthPtr, Holds a pointer to a memory location in which the length
 *                  information is stored. On calling this function this parameter shall
 *                  contain the size of the buffer provided by resultPtr. If the key
 *                  element is configured to allow partial access, this parameter
 *                  contains the amount of data which should be read from the key
 *                  element. The size may not be equal to the size of the provided
 *                  buffer anymore. When the request has finished, the amount of
 *                  data that has been stored shall be stored.
 * Parameters(OUT): @resultPtr,Holds the pointer of the buffer for the returned key element.
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed, the requested key element is not available
 *          CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *          CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyElementGet(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementId,
    P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) resultPtr,
    P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) resultLengthPtr)
{
    Std_ReturnType ret = E_NOT_OK; /*PRQA S 2981*/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00059] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_GET, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00060] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_GET, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00062] */
    else if (NULL_PTR == resultPtr)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_GET, CRYIF_E_PARAM_POINTER);
    }
    /* @req [SWS_CryIf_00063] */
    else if (NULL_PTR == resultLengthPtr)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_GET, CRYIF_E_PARAM_POINTER);
    }
    /* @req [SWS_CryIf_00064] */
    else if (0u == *resultLengthPtr)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_GET, CRYIF_E_PARAM_VALUE);
    }
    else if (keyElementId >= CRYIF_KEYELEMENT_MAX_CONFIGURED)
    {
        return ret;
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_KeyElementGet_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00065] */
            ret = CryIf_DriverApi[cryDriverId].Crypto_KeyElementGet_Name(
                CryIf_GetDriverKeyId(cryIfKeyId),
                keyElementId,
                resultPtr,
                resultLengthPtr);
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyElementGet_Name(
            CryIf_GetDriverKeyId(cryIfKeyId),
            keyElementId,
            resultPtr,
            resultLengthPtr);
#endif
    }
    return ret;
}

/**
 * This function shall copy a key elements from one key to a target key.
 * Service ID: 0x0f
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same cryIfKeyId
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be the source element.
 *          @keyElementId, Holds the identifier of the key element which shall be the source for the copy operation.
 *          @targetCryIfKeyId, Holds the identifier of the key whose key element shall be the destination element.
 *          @targetKeyElementId, Holds the identifier of the key element which shall be the destination for the copy
 * operation. Parameters(INOUT):NA Parameters(OUT): NA Return value: E_OK: Request successful E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed, the requested key element is not available
 *          CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *          CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element
 *          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyElementCopy(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementId,
    VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) targetKeyElementId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00110] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_COPY, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00111] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_COPY, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00112] */
    else if (targetCryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_COPY, CRYIF_E_PARAM_HANDLE);
    }
    else if (keyElementId >= CRYIF_KEYELEMENT_MAX_CONFIGURED)
    {
        return ret;
    }
    else
#endif
    {
        if ((cryIfKeyId == targetCryIfKeyId) || (CRYIF_KEY_MAX_CONFIGURED < 2u))
        {
            return ret;
        }
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        uint8 cryTargetDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        cryTargetDriverId = CryIf_GetDriverId(targetCryIfKeyId);
        /* @req [SWS_CryIf_00113] */
        if (cryDriverId == cryTargetDriverId)
        {
            if (CryIf_DriverApi[cryDriverId].Crypto_KeyElementCopy_Name != NULL_PTR)
            {
                ret = CryIf_DriverApi[cryDriverId].Crypto_KeyElementCopy_Name(
                    CryIf_GetDriverKeyId(cryIfKeyId),
                    keyElementId,
                    CryIf_GetDriverKeyId(targetCryIfKeyId),
                    targetKeyElementId);
            }
        }
        else
        {
            if (CryIf_DriverApi[cryDriverId].Crypto_KeyElementGet_Name != NULL_PTR)
            {
                uint8 resultPtr;
                uint8 targetresultPtr;
                uint32 resultLengthPtr =
                    4294967295; /* max value and different from targetresultLengthPtr ,get true resultLength*/
                uint32 targetresultLengthPtr =
                    4294967294; /* max value and different from resultLengthPtr ,get true targetresultLength*/
                CryIf_DriverApi[cryDriverId].Crypto_KeyElementGet_Name(
                    CryIf_GetDriverKeyId(cryIfKeyId),
                    keyElementId,
                    &resultPtr,
                    &resultLengthPtr);
                CryIf_DriverApi[cryDriverId].Crypto_KeyElementGet_Name(
                    CryIf_GetDriverKeyId(targetCryIfKeyId),
                    targetKeyElementId,
                    &targetresultPtr,
                    &targetresultLengthPtr);
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
                /* @req [SWS_CryIf_00115] */
                if (resultLengthPtr != targetresultLengthPtr)
                {
                    CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_COPY, CRYIF_E_KEY_SIZE_MISMATCH);
                }
#endif
                if (CryIf_DriverApi[cryTargetDriverId].Crypto_KeyElementSet_Name != NULL_PTR)
                {
                    /* @req [SWS_CryIf_00114] */
                    ret = CryIf_DriverApi[cryTargetDriverId].Crypto_KeyElementSet_Name(
                        CryIf_GetDriverKeyId(targetCryIfKeyId),
                        targetKeyElementId,
                        &resultPtr,
                        targetresultLengthPtr);
                }
            }
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyElementCopy_Name(
            CryIf_GetDriverKeyId(cryIfKeyId),
            keyElementId,
            CryIf_GetDriverKeyId(targetCryIfKeyId),
            targetKeyElementId);
#endif
    }
    return ret;
}

/**
 * Copies a key element to another key element. The keyElementOffsets and
 * keyElementCopyLength allows to copy just parts of the source key element into the destination key element.
 * Service ID: 0x12
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same cryIfKeyId
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be the source element.
 *          @keyElementId, Holds the identifier of the key element which shall be the source for the copy operation.
 *          @keyElementSourceOffset, This is the offset of the source key element indicating the start index of the copy
 * operation.
 *          @keyElementTargetOffset, This is the offset of the target key element indicating the start index of the copy
 * operation.
 *          @keyElementCopyLength, Specifies the number of bytes that shall be copied.
 *          @targetCryIfKeyId, Holds the identifier of the key whose key element shall be the destination element.
 *          @targetKeyElementId, Holds the identifier of the key element which shall be the destination for the copy
 * operation. Parameters(INOUT):NA Parameters(OUT): NA Return value: E_OK: Request successful E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed, the requested key element is not available
 *          CRYPTO_E_KEY_READ_FAIL: Request failed because read access was denied
 *          CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element
 *          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyElementCopyPartial(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementId,
    VAR(uint32, CRYIF_APPL_DATA) keyElementSourceOffset,
    VAR(uint32, CRYIF_APPL_DATA) keyElementTargetOffset,
    VAR(uint32, CRYIF_APPL_DATA) keyElementCopyLength,
    VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId,
    VAR(uint32, CRYIF_APPL_DATA) targetKeyElementId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
    uint8 cryDriverId;
    uint8 cryTargetDriverId;
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
#endif
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00137] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_COPYPARTIAL, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00138] */
    else if (
        (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED) || (targetCryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
        || (keyElementId >= CRYIF_KEYELEMENT_MAX_CONFIGURED) || (CRYIF_KEYELEMENT_MAX_CONFIGURED == targetKeyElementId))
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_COPYPARTIAL, CRYIF_E_PARAM_HANDLE);
    }
    else
#endif
    {
        if ((cryIfKeyId == targetCryIfKeyId) || (CRYIF_KEY_MAX_CONFIGURED < 2u))
        {
            return ret;
        }
    }
    (void)keyElementId;
    (void)keyElementSourceOffset;
    (void)keyElementTargetOffset;
    (void)keyElementCopyLength;
    (void)targetKeyElementId;
    return ret;
}

/**
 * This function shall copy all key elements from the source key to a target key.
 * Service ID: 0x10
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant, but not for the same cryIfKeyId
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key whose key element shall be the source element.
 *          @targetCryIfKeyId, Holds the identifier of the key whose key element shall be the destination element.
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_NOT_AVAILABLE: Request failed, the requested key element is not available
 *          CRYPTO_E_KEY_READ_FAIL: Request failed, not allowed to extract key element
 *          CRYPTO_E_KEY_WRITE_FAIL: Request failed, not allowed to write key element
 *          CRYPTO_E_KEY_SIZE_MISMATCH: Request failed, key element sizes are not compatible
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyCopy(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId, VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00016] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYCOPY, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00017] */
    /* @req [SWS_CryIf_00018] */
    else if ((cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED) || (targetCryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED))
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYCOPY, CRYIF_E_PARAM_HANDLE);
    }
    else
#endif
    {
        if ((cryIfKeyId == targetCryIfKeyId) || (CRYIF_KEY_MAX_CONFIGURED < 2u))
        {
            return ret;
        }
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        uint8 cryTargetDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        cryTargetDriverId = CryIf_GetDriverId(targetCryIfKeyId);
        /* @req [SWS_CryIf_00019] */
        if (cryDriverId == cryTargetDriverId)
        {
            if (CryIf_DriverApi[cryDriverId].Crypto_KeyCopy_Name != NULL_PTR)
            {
                ret = CryIf_DriverApi[cryDriverId].Crypto_KeyCopy_Name(
                    CryIf_GetDriverKeyId(cryIfKeyId),
                    CryIf_GetDriverKeyId(targetCryIfKeyId));
            }
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyCopy_Name(
            CryIf_GetDriverKeyId(cryIfKeyId),
            CryIf_GetDriverKeyId(targetCryIfKeyId));
#endif
    }
    return ret;
}

/**
 * This function shall dispatch the random seed function to the configured cryptodriver object.
 * Service ID: 0x07
 * Sync/Async: Sync or Async, depends on the configuration
 * Reentrancy: Reentrant
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key for which a new seed shall be generated.
 *          @seedPtr, Holds a pointer to the memory location which contains the data to feed the seed.
 *          @seedLength, Contains the length of the seed in bytes.
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_RandomSeed(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) seedPtr,
    VAR(uint32, CRYIF_APPL_DATA) seedLength)
{
    Std_ReturnType ret = E_NOT_OK; /*PRQA S 2981*/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00068] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_RANDOM_SEED, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00069] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_RANDOM_SEED, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00070] */
    else if (NULL_PTR == seedPtr)
    {
        CRYIF_DET_REPORT(CRYIF_SID_RANDOM_SEED, CRYIF_E_PARAM_POINTER);
    }
    /* @req [SWS_CryIf_00071] */
    else if (0u == seedLength)
    {
        CRYIF_DET_REPORT(CRYIF_SID_RANDOM_SEED, CRYIF_E_PARAM_VALUE);
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_RandomSeed_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00072] */
            ret = CryIf_DriverApi[cryDriverId].Crypto_RandomSeed_Name(
                CryIf_GetDriverKeyId(cryIfKeyId),
                seedPtr,
                seedLength);
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_RandomSeed_Name(
            CryIf_GetDriverKeyId(cryIfKeyId),
            seedPtr,
            seedLength);
#endif
    }
    return ret;
}

/**
 * This function shall dispatch the key generate function to the configured crypto driver object.
 * Service ID: 0x08
 * Sync/Async: Sync or Async, depends on the configuration
 * Reentrancy: Reentrant
 * Parameters(IN):  @cryIfKeyId, Holds the identifier of the key which is to be updated with the generated value.
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyGenerate(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId)
{
    Std_ReturnType ret = E_NOT_OK; /*PRQA S 2981*/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00073] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYGENERATE, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00074] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYGENERATE, CRYIF_E_PARAM_HANDLE);
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_KeyGenerate_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00075] */
            ret = CryIf_DriverApi[cryDriverId].Crypto_KeyGenerate_Name(CryIf_GetDriverKeyId(cryIfKeyId));
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyGenerate_Name(CryIf_GetDriverKeyId(cryIfKeyId));
#endif
    }
    return ret;
}

/**
 * This function shall dispatch the key derive function to the configured crypto driver object.
 * Service ID: 0x09
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key which is used for key derivation.
 *          @targetCryIfKeyId, Holds the identifier of the key which is used to store the derived key.
 * Parameters(INOUT):NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyDerive(VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId, VAR(uint32, CRYIF_APPL_DATA) targetCryIfKeyId)
{
    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00076] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYDERIVE, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00077] */
    /* @req [SWS_CryIf_00122] */
    else if ((cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED) || (targetCryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED))
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYDERIVE, CRYIF_E_PARAM_HANDLE);
    }
    else
#endif
    {
        if ((cryIfKeyId == targetCryIfKeyId) || (CRYIF_KEY_MAX_CONFIGURED < 2u))
        {
            return ret;
        }
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_KeyDerive_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00081] */
            ret = CryIf_DriverApi[cryDriverId].Crypto_KeyDerive_Name(
                CryIf_GetDriverKeyId(cryIfKeyId),
                CryIf_GetDriverKeyId(targetCryIfKeyId));
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyDerive_Name(
            CryIf_GetDriverKeyId(cryIfKeyId),
            CryIf_GetDriverKeyId(targetCryIfKeyId));
#endif
    }
    return ret;
}

/**
 * This function shall dispatch the key exchange public value calculation function to the configured crypto driver
 * object. Service ID: 0x0a Sync/Async: Synchronous Reentrancy: Reentrant Parameters(IN): @cryIfKeyId, Holds the
 * identifier of the key which shall be used for the key exchange protocol. Parameters(INOUT): @publicValueLengthPtr,
 * Holds a pointer to the memory location in which the public value length information is stored. On calling this
 * function, this parameter shall contain the size of the buffer provided by publicValuePtr. When the request has
 * finished, the actual length of the returned value shall be stored. Parameters(OUT): @publicValuePtr, Contains the
 * pointer to the data where the public value shall be stored. Return value: E_OK: Request successful E_NOT_OK: Request
 * failed CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy CRYPTO_E_SMALL_BUFFER: The provided buffer is too
 * small to store the result CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyExchangeCalcPubVal(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) publicValuePtr,
    P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) publicValueLengthPtr)
{
    Std_ReturnType ret = E_NOT_OK; /*PRQA S 2981*/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00082] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYEXCHANGECALCPUBVAL, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00083] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYEXCHANGECALCPUBVAL, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00084] */
    /* @req [SWS_CryIf_00085] */
    else if ((NULL_PTR == publicValuePtr) || (NULL_PTR == publicValueLengthPtr))
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYEXCHANGECALCPUBVAL, CRYIF_E_PARAM_POINTER);
    }
    /* @req [SWS_CryIf_00086] */
    else if (0u == *publicValueLengthPtr)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYEXCHANGECALCPUBVAL, CRYIF_E_PARAM_VALUE);
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_KeyExchangeCalcPubVal_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00087] */
            ret = CryIf_DriverApi[cryDriverId].Crypto_KeyExchangeCalcPubVal_Name(
                CryIf_GetDriverKeyId(cryIfKeyId),
                publicValuePtr,
                publicValueLengthPtr);
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyExchangeCalcPubVal_Name(
            CryIf_GetDriverKeyId(cryIfKeyId),
            publicValuePtr,
            publicValueLengthPtr);
#endif
    }
    return ret;
}

/**
 * This function shall dispatch the key exchange common shared secret calculation function to the configured crypto
 * driver object. Service ID: 0x0b Sync/Async: Synchronous Reentrancy: Reentrant Parameters(IN):
 *          @cryIfKeyId, Holds the identifier of the key which shall be used for the key exchange protocol.
 *          @partnerPublicValuePtr, Holds the pointer to the memory location which contains the partner's public value.
 *          @partnerPublicValueLength, Contains the length of the partner's public value in bytes.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value:
 *          E_OK: Request successful
 *          E_NOT_OK: Request failed
 *          CRYPTO_E_BUSY: Request failed, Crypto Driver Object is busy
 *          CRYPTO_E_SMALL_BUFFER: The provided buffer is too small to store the result
 *          CRYPTO_E_KEY_EMPTY: Request failed because of uninitialized source key element
 */

FUNC(Std_ReturnType, CRYIF_CODE)
CryIf_KeyExchangeCalcSecret(
    VAR(uint32, CRYIF_APPL_DATA) cryIfKeyId,
    P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) partnerPublicValuePtr,
    VAR(uint32, CRYIF_APPL_DATA) partnerPublicValueLength)
{
    Std_ReturnType ret = E_NOT_OK; /*PRQA S 2981*/
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00090] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYEXCHANGECALCSECRET, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00091] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYEXCHANGECALCSECRET, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00092] */
    else if (NULL_PTR == partnerPublicValuePtr)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYEXCHANGECALCSECRET, CRYIF_E_PARAM_POINTER);
    }
    /* @req [SWS_CryIf_00094] */
    else if (0u == partnerPublicValueLength)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYEXCHANGECALCSECRET, CRYIF_E_PARAM_VALUE);
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_KeyExchangeCalcSecret_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00095] */
            ret = CryIf_DriverApi[cryDriverId].Crypto_KeyExchangeCalcSecret_Name(
                CryIf_GetDriverKeyId(cryIfKeyId),
                partnerPublicValuePtr,
                partnerPublicValueLength);
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyExchangeCalcSecret_Name(
            CryIf_GetDriverKeyId(cryIfKeyId),
            partnerPublicValuePtr,
            partnerPublicValueLength);
#endif
    }
    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CALLBACK_CODE_NOTIFICATION_CODE
#include "CryIf_MemMap.h"
/**
 * Notifies the CRYIF about the completion of the request with the result of the cryptographic operation.
 * Service ID: 0x0d
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN):
 *          @job, Points to the completed job's information structure. It contains a callbackID to identify which job is
 * finished.
 *          @result, Contains the result of the cryptographic operation.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */

FUNC(void, CRYIF_CODE)
CryIf_CallbackNotification(
    P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job,
    VAR(Crypto_ResultType, CSM_APPL_DATA) result)
{
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00107] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_CBNOTIFICATION, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00108] */
    else if (NULL_PTR == job)
    {
        CRYIF_DET_REPORT(CRYIF_SID_CBNOTIFICATION, CRYIF_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* @req [SWS_CryIf_00109] */
        Csm_CallbackNotification(job, result);
    }
}
#define CRYIF_STOP_SEC_CALLBACK_CODE_NOTIFICATION_CODE
#include "CryIf_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
static FUNC(uint8, CRYIF_CODE) CryIf_GetDriverId(VAR(uint32, AUTOMATIC) keyid)
{
    uint8 driverid = 0u;
    uint8 i;
    /*  search driverId form CryIf_KeyCfg   */
    for (i = 0; i < CRYIF_KEY_MAX_CONFIGURED; i++)
    {
        if (CryIf_KeyCfg[i].KeyId == keyid)
        {
            driverid = CryIf_KeyCfg[i].CryptoChannelId;
            break;
        }
    }
    driverid--;
    return driverid;
}
#endif
static FUNC(uint32, CRYIF_CODE) CryIf_GetDriverKeyId(VAR(uint32, AUTOMATIC) keyid)
{
    uint32 driverkeyid = 0u;
    uint8 i;
    /*  search driverId form CryIf_KeyCfg   */
    for (i = 0u; i < CRYIF_KEY_MAX_CONFIGURED; i++)
    {
        if (CryIf_KeyCfg[i].KeyId == keyid)
        {
            driverkeyid = CryIf_KeyCfg[i].CryptoKeyId;
            break;
        }
    }
    return driverkeyid;
}

#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
