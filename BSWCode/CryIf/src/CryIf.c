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
 ***********************************************************************************************************************
 **
 **  @file               : CryIf.c
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : CryIf Interface Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */

/* PRQA S 3432 ++ */ /* VL_CryIf_3432 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */

#include "CryIf_Cbk.h"
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif
#include "Csm_Cbk.h"
#include "Csm_Types.h"
#include "Crypto_62.h"
/* ===================================================== macros ===================================================== */
/*cryif cersion check*/
#define CRYIF_C_AR_MAJOR_VERSION 4U
#define CRYIF_C_AR_MINOR_VERSION 9U
#define CRYIF_C_AR_PATCH_VERSION 0U
#define CRYIF_C_SW_MAJOR_VERSION 2U
#define CRYIF_C_SW_MINOR_VERSION 2U
#define CRYIF_C_SW_PATCH_VERSION 0U

#if (                                                                                                                \
    (CRYIF_C_AR_MAJOR_VERSION != CRYIF_H_AR_MAJOR_VERSION) || (CRYIF_C_AR_MINOR_VERSION != CRYIF_H_AR_MINOR_VERSION) \
    || (CRYIF_C_AR_PATCH_VERSION != CRYIF_H_AR_PATCH_VERSION)                                                        \
    || (CRYIF_C_SW_MAJOR_VERSION != CRYIF_H_SW_MAJOR_VERSION)                                                        \
    || (CRYIF_C_SW_MINOR_VERSION != CRYIF_H_SW_MINOR_VERSION)                                                        \
    || (CRYIF_C_SW_PATCH_VERSION != CRYIF_H_SW_PATCH_VERSION))

#error "CryIf.c version mismatching with CryIf.h"
#endif

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definitions =========================================== */
/**
 * @brief          Reports a development error to the DET module.
 *                 This function is used to report development errors that occur
 *                 within the CryIf module. It takes an API ID and an error ID
 *                 as parameters and passes them to the Det_ReportError function.
 * @param[in]      ApiId : API ID of the function where the error occurred.
 *                 This parameter is a unique identifier for the API function
 *                 that detected the error.
 * @param[in]      ErrorId : Error ID of the detected error.
 *                 This parameter is a unique identifier for the error that
 *                 occurred.
 * @return         None
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71589
 */
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
CRYIF_LOCAL inline void CRYIF_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID, (ApiId), (ErrorId));
}
#endif

/* ========================================= internal function declarations ========================================= */
#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
/*******************************************************************************/
/**
 * @brief          Retrieves the driver ID associated with a given key ID.
 *                 This function searches the CryIf_KeyCfg array for a key ID
 *                 that matches the provided keyid parameter. If a match is
 *                 found, it returns the corresponding CryptoChannelId from the
 *                 CryIf_KeyCfg array, decremented by 1.
 * @param[in]      keyid : Key ID to search for in the CryIf_KeyCfg array.
 *                 This parameter is a unique identifier for a key configuration.
 * @return         uint8
 * @retval         driverid: The driver ID associated with the given key ID,
 *                 or 0xFF if the key ID is not found in the CryIf_KeyCfg array.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71590
 */
/*******************************************************************************/
CRYIF_LOCAL uint8 CryIf_GetDriverId(uint32 keyid);
#endif
/*******************************************************************************/
/**
 * @brief          Retrieves the driver key ID associated with a given key ID.
 *                 This function searches the CryIf_KeyCfg array for a key ID
 *                 that matches the provided keyid parameter. If a match is
 *                 found, it returns the corresponding CryptoKeyId from the
 *                 CryIf_KeyCfg array.
 * @param[in]      keyid : Key ID to search for in the CryIf_KeyCfg array.
 *                 This parameter is a unique identifier for a key configuration.
 * @return         uint32
 * @retval         driverkeyid: The driver key ID associated with the given key ID,
 *                 or 0u if the key ID is not found in the CryIf_KeyCfg array.
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71591
 */
/*******************************************************************************/
CRYIF_LOCAL uint32 CryIf_GetDriverKeyId(uint32 keyid);
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

/* =========================================== internal data definitions =========================================== */

/*Module initialize status, (boolean)TRUE initialized, (boolean)FALSE not initialized*/
#define CRYIF_START_SEC_VAR_CLEARED_BOOLEAN
#include "CryIf_MemMap.h"
/**
 * @brief Initialization status flag
 * @range 0-1
 */
CRYIF_LOCAL boolean CryIf_InitStatus; /* PRQA S 3218,3229 */ /* VL_CryIf_3218,VL_CryIf_3229 */
#define CRYIF_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "CryIf_MemMap.h"

/* ========================================== external function definitions ========================================= */

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief Initializes the CryIf module.
 */
/*******************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void CryIf_Init(const CryIf_ConfigType* configPtr)
{
/* PRQA S 1503 -- */
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
/*******************************************************************************/
/**
 * @brief Returns the version information of this module.
 */
/*******************************************************************************/
void CryIf_GetVersionInfo(Std_VersionInfoType* versioninfo)
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
        versioninfo->moduleID         = CRYIF_MODULE_ID;
        versioninfo->vendorID         = CRYIF_VENDOR_ID;
        versioninfo->sw_major_version = CRYIF_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CRYIF_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CRYIF_H_SW_PATCH_VERSION;
    }
    return;
}
#endif
/*******************************************************************************/
/**
 * @brief This interface dispatches the received jobs to the configured crypto driver object.
 */
/*******************************************************************************/
/* PRQA S 6030 ++ */ /* VL_MTR_CryIf_STMIF */
Std_ReturnType CryIf_ProcessJob(uint32 channelId, Crypto_JobType* job)
/* PRQA S 6030 -- */
{
    Std_ReturnType         ret = E_NOT_OK;
    Crypto_ServiceInfoType Serviceid;

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
        const Crypto_JobPrimitiveInfoType* JobPrimitiveInfo = job->jobPrimitiveInfo;
        Serviceid                                           = JobPrimitiveInfo->primitiveInfo->service;
        if (CRYPTO_HASH == Serviceid)
        {
            ret = E_OK;
        }
        else
        {
            /* PRQA S 1252 ++ */ /* VL_CryIf_1252 */
            if (JobPrimitiveInfo->cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
            /* PRQA S 1252 -- */
            {
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
                CRYIF_DET_REPORT(CRYIF_SID_PROCESSJOB, CRYIF_E_PARAM_HANDLE);
#endif
            }
            else
            {
                job->cryptoKeyId = CryIf_GetDriverKeyId(JobPrimitiveInfo->cryIfKeyId);
                /* @req [SWS_CryIf_00135] */
                if (CRYPTO_KEYDERIVE == Serviceid)
                {
                    /* PRQA S 1252 ++ */ /* VL_CryIf_1252 */
                    if (job->jobPrimitiveInputOutput.targetCryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
                    /* PRQA S 1252 -- */
                    {
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
                        CRYIF_DET_REPORT(CRYIF_SID_PROCESSJOB, CRYIF_E_PARAM_HANDLE);
#endif
                    }
                    else
                    {
                        job->targetCryptoKeyId = CryIf_GetDriverKeyId(job->jobPrimitiveInputOutput.targetCryIfKeyId);
                        ret                    = E_OK;
                    }
                }
                else
                {
                    ret = E_OK;
                }
            }
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

/*******************************************************************************/
/**
 * @brief This interface dispatches the job cancellation function to the configured crypto driver object.
 */
/*******************************************************************************/
Std_ReturnType CryIf_CancelJob(uint32 channelId, Crypto_JobType* job)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
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

/*******************************************************************************/
/**
 * @brief This function shall dispatch the set key element function to the configured crypto driver object.
 */
/*******************************************************************************/
/* PRQA S 6030,6060 ++ */ /* VL_MTR_CryIf_STMIF,VL_MTR_CryIf_STM19 */
Std_ReturnType CryIf_KeyElementSet(uint32 cryIfKeyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength)
/* PRQA S 6030,6060 -- */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
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
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_SET, CRYIF_E_PARAM_HANDLE);
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

/*******************************************************************************/
/**
 * @brief This function shall dispatch the set key valid function to the configured crypto driver object.
 */
/*******************************************************************************/

Std_ReturnType CryIf_KeySetValid(uint32 cryIfKeyId)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
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
/*******************************************************************************/
/**
 * @brief This function shall dispatch the set key valid function to the configured crypto driver object.
 */
/*******************************************************************************/

Std_ReturnType CryIf_KeySetInValid(uint32 cryIfKeyId)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00056] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYSETINVALID, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00057] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYSETINVALID, CRYIF_E_PARAM_HANDLE);
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        cryDriverId = CryIf_GetDriverId(cryIfKeyId);
        if (CryIf_DriverApi[cryDriverId].Crypto_KeySetInValid_Name != NULL_PTR)
        {
            /* @req [SWS_CryIf_00058] */
            ret = CryIf_DriverApi[cryDriverId].Crypto_KeySetInValid_Name(CryIf_GetDriverKeyId(cryIfKeyId));
        }
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeySetInValid_Name(CryIf_GetDriverKeyId(cryIfKeyId));
#endif
    }
    return ret;
}
/*******************************************************************************/
/**
 * @brief This function shall dispatch the get key element function to the configured crypto driver object.
 */
/*******************************************************************************/
/* PRQA S 6030,6060 ++ */ /* VL_MTR_CryIf_STMIF,VL_MTR_CryIf_STM19 */
Std_ReturnType CryIf_KeyElementGet(uint32 cryIfKeyId, uint32 keyElementId, uint8* resultPtr, uint32* resultLengthPtr)
/* PRQA S 6030,6060 -- */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
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
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_SET, CRYIF_E_PARAM_HANDLE);
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

/*******************************************************************************/
/**
 * @brief This function shall copy a key elements from one key to a target key.
 */
/*******************************************************************************/

/* PRQA S 6030,6060, 2889 ++ */ /* VL_MTR_CryIf_STMIF, VL_MTR_CryIf_STM19, VL_QAC_MultiReturn */
Std_ReturnType
    CryIf_KeyElementCopy(uint32 cryIfKeyId, uint32 keyElementId, uint32 targetCryIfKeyId, uint32 targetKeyElementId)
/* PRQA S 6030,6060, 2889 -- */
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
        /* PRQA S 1258,1252 ++ */ /* VL_CryIf_1258,VL_CryIf_1252 */
        if (cryIfKeyId == targetCryIfKeyId)
        /* PRQA S 1258,1252 -- */
        {
            return ret;
        }
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        uint8 cryTargetDriverId;
        cryDriverId       = CryIf_GetDriverId(cryIfKeyId);
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
                uint8  resultPtr;
                uint8  targetresultPtr;
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

/*******************************************************************************/
/**
 * @brief Copies a key element to another key element. The keyElementOffsets and
 * keyElementCopyLength allows to copy just parts of the source key element into the destination key element.
 */
/*******************************************************************************/

/* PRQA S 6060, 6040, 2889 ++ */ /* VL_MTR_CryIf_STM19, VL_MTR_CryIf_STPAR, VL_QAC_MultiReturn */
Std_ReturnType CryIf_KeyElementCopyPartial(
    uint32 cryIfKeyId,
    uint32 keyElementId,
    uint32 keyElementSourceOffset,
    uint32 keyElementTargetOffset,
    uint32 keyElementCopyLength,
    uint32 targetCryIfKeyId,
    uint32 targetKeyElementId)
/* PRQA S 6060, 6040, 2889 -- */
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
        /* PRQA S 1258,1252 ++ */ /* VL_CryIf_1258,VL_CryIf_1252 */
        if (cryIfKeyId == targetCryIfKeyId)
        /* PRQA S 1258,1252 -- */
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

/*******************************************************************************/
/**
 * @brief This function shall copy all key elements from the source key to a target key.
 */
/*******************************************************************************/

/* PRQA S 6060, 2889 ++ */ /* VL_MTR_CryIf_STM19, VL_QAC_MultiReturn */
Std_ReturnType CryIf_KeyCopy(uint32 cryIfKeyId, uint32 targetCryIfKeyId)
/* PRQA S 6060, 2889 -- */
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
        /* PRQA S 1258,1252 ++ */ /* VL_CryIf_1258,VL_CryIf_1252 */
        if (cryIfKeyId == targetCryIfKeyId)
        /* PRQA S 1258,1252 -- */
        {
            return ret;
        }
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        uint8 cryDriverId;
        uint8 cryTargetDriverId;
        cryDriverId       = CryIf_GetDriverId(cryIfKeyId);
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

/*******************************************************************************/
/**
 * @brief This function shall dispatch the random seed function to the configured cryptodriver object.
 */
/*******************************************************************************/

Std_ReturnType CryIf_RandomSeed(uint32 cryIfKeyId, const uint8* seedPtr, uint32 seedLength)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
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

/*******************************************************************************/
/**
 * @brief This function shall dispatch the key generate function to the configured crypto driver object.
 */
/*******************************************************************************/

Std_ReturnType CryIf_KeyGenerate(uint32 cryIfKeyId)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
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

/*******************************************************************************/
/**
 * @brief This function shall dispatch the key derive function to the configured crypto driver object.
 */
/*******************************************************************************/

/* PRQA S 6060, 2889 ++ */ /* VL_MTR_CryIf_STM19, VL_QAC_MultiReturn */
Std_ReturnType CryIf_KeyDerive(uint32 cryIfKeyId, uint32 targetCryIfKeyId)
/* PRQA S 6060, 2889 -- */
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
        /* PRQA S 1258,1252 ++ */ /* VL_CryIf_1258,VL_CryIf_1252 */
        if (cryIfKeyId == targetCryIfKeyId)
        /* PRQA S 1258,1252 -- */
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

/*******************************************************************************/
/**
 * @brief This function shall dispatch the key exchange public value calculation
 * function to the configured crypto driver object.
 */
/*******************************************************************************/

Std_ReturnType CryIf_KeyExchangeCalcPubVal(uint32 cryIfKeyId, uint8* publicValuePtr, uint32* publicValueLengthPtr)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
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

/*******************************************************************************/
/**
 * @brief This function shall dispatch the key exchange common shared secret
 * calculation function to the configured crypto driver object.
 */
/*******************************************************************************/

Std_ReturnType
    CryIf_KeyExchangeCalcSecret(uint32 cryIfKeyId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength)
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981 */ /* VL_CryIf_2981 */
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

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief the CRYIF about the completion of the request with the result of the cryptographic operation.
 */
/*******************************************************************************/

void CryIf_CallbackNotification(Crypto_JobType* job, Crypto_ResultType result)
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
/*******************************************************************************/
/**
 * @brief get the status of a key .
 */
/*******************************************************************************/
Std_ReturnType CryIf_KeyGetStatus(uint32 cryIfKeyId, Crypto_KeyStatusType* keyStatusPtr)
{
    Std_ReturnType ret = E_NOT_OK;
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
    ret = Crypto_62_KeyGetStatus(cryIfKeyId, keyStatusPtr);
#endif
    return ret;
}

/*******************************************************************************/
/**
 * @brief Custom synchronous operation for the CryIf module.
 */
/*******************************************************************************/
/* PRQA S 6040 ++ */ /* VL_MTR_CryIf_STPAR */
Std_ReturnType CryIf_CustomSync(
    uint32       dispatchId,
    uint32       keyId,
    uint32       keyElementId,
    uint32       targetKeyId,
    uint32       targetKeyElementId,
    const uint8* inputPtr,
    uint32       inputLength,
    uint8*       outputPtr,
    uint32*      outputLengthPtr,
    uint8*       secondaryOutputPtr,
    uint32*      secondaryOutputLengthPtr)
/* PRQA S 6040 -- */
{
    Std_ReturnType ret;
    ret = Crypto_62_CustomSync(
        dispatchId,
        keyId,
        keyElementId,
        targetKeyId,
        targetKeyElementId,
        inputPtr,
        inputLength,
        outputPtr,
        outputLengthPtr,
        secondaryOutputPtr,
        secondaryOutputLengthPtr);
    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
/*******************************************************************************/
/**
 * @brief Function to get driverId from keyId.
 */
/*******************************************************************************/

CRYIF_LOCAL uint8 CryIf_GetDriverId(uint32 keyid)
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
/*******************************************************************************/
/**
 * @brief Function to get keyid in driver from Cryif keyId.
 */
/*******************************************************************************/
CRYIF_LOCAL uint32 CryIf_GetDriverKeyId(uint32 keyid)
{
    uint32 driverkeyid = 0u;
    uint32 i;
    /*  search driverId form CryIf_KeyCfg   */
    /* PRQA S 1258 ++ */ /* VL_CryIf_1258 */
    for (i = 0u; i < (uint32)CRYIF_KEY_MAX_CONFIGURED; i++)
    /* PRQA S 1258 -- */
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
/* PRQA S 1532 -- */
/* PRQA S 3432 -- */
