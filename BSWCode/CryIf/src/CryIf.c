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
#include "CryIf_Arti.h"
/* ===================================================== macros ===================================================== */
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
#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
CRYIF_LOCAL inline void CRYIF_DET_REPORT(uint8 ApiId, uint8 ErrorId)
{
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID, (ApiId), (ErrorId));
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"
#endif

/* ========================================= internal function declarations ========================================= */
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief          Maps a CryIf key ID to a CryIf_DriverApi[] index.
 * @details        Intended behavior: resolve keyid to the target crypto driver /
 *                 channel index used for key operations under multi-driver support.
 *                 Current status (CPT-19718): CryptoChannelId was removed from
 *                 CryIf_KeyCfgType; key-to-driver mapping (e.g. CryIf_KeyChannelId[]
 *                 or equivalent) is not implemented yet. Do not extend
 *                 CryIf_KeyCfgType for this purpose. Until the mapping is restored,
 *                 this function returns 0u (CryIf_DriverApi[0]) as a temporary stub
 *                 so single-driver / channel-0 configurations remain usable.
 * @param[in]      keyid : CryIf key ID to resolve.
 * @return         uint8
 * @retval         0u: temporary stub index into CryIf_DriverApi[] (pending mapping).
 * @reentrant      Reentrant
 * @synchronous    TRUE
 * @trace       CPD-71590
 */
/*******************************************************************************/
CRYIF_LOCAL uint8 CryIf_GetDriverId(uint32 keyid);
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"
#endif

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
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

#define CRYIF_START_SEC_VAR_INIT_32
#include "CryIf_MemMap.h"
CRYIF_LOCAL uint32 CryIf_CurrentLockedKeyId = CRYIF_KEY_INVALID_ID; /* PRQA S 3218 */ /* VL_CryIf_3218 */
#define CRYIF_STOP_SEC_VAR_INIT_32
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
    CryIf_ArtiApiStart(2, CRYIF_SID_INIT, configPtr);

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

    CryIf_ArtiApiStop(1, CRYIF_SID_INIT);

    return;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#if (STD_ON == CRYIF_VERSION_INFO_API)
#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief Returns the version information of this module.
 */
/*******************************************************************************/
void CryIf_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    CryIf_ArtiApiStart(2, CRYIF_SID_GETVERSIONINFO, versioninfo);

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
        versioninfo->sw_major_version = CRYIF_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = CRYIF_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = CRYIF_SW_PATCH_VERSION;
    }

    CryIf_ArtiApiStop(1, CRYIF_SID_GETVERSIONINFO);

    return;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"
#endif

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This interface dispatches the received jobs to the configured crypto driver object.
 */
/*******************************************************************************/
/* PRQA S 6030 ++ */ /* VL_MTR_CryIf_STMIF */
Std_ReturnType CryIf_ProcessJob(uint32 channelId, Crypto_JobType* job)
/* PRQA S 6030 -- */
{
    CryIf_ArtiApiStart(3, CRYIF_SID_PROCESSJOB, channelId, job);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_PROCESSJOB, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This interface dispatches the job cancellation function to the configured crypto driver object.
 */
/*******************************************************************************/
Std_ReturnType CryIf_CancelJob(uint32 channelId, Crypto_JobType* job)
{
    CryIf_ArtiApiStart(3, CRYIF_SID_CANCELJOB, channelId, job);

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
    CryIf_ArtiApiStop(2, CRYIF_SID_CANCELJOB, ret);
    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall dispatch the set key element function to the configured crypto driver object.
 */
/*******************************************************************************/
/* PRQA S 6030,6060 ++ */ /* VL_MTR_CryIf_STMIF,VL_MTR_CryIf_STM19 */
Std_ReturnType CryIf_KeyElementSet(uint32 cryIfKeyId, uint32 keyElementId, const uint8* keyPtr, uint32 keyLength)
/* PRQA S 6030,6060 -- */
{
    CryIf_ArtiApiStart(5, CRYIF_SID_KEYELEMENT_SET, cryIfKeyId, keyElementId, keyPtr, keyLength);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYELEMENT_SET, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

/*******************************************************************************/
/**
 * @brief This function shall dispatch the set key valid function to the configured crypto driver object.
 */
/*******************************************************************************/

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
Std_ReturnType CryIf_KeySetValid(uint32 cryIfKeyId)
{
    CryIf_ArtiApiStart(2, CRYIF_SID_KEYSETVALID, cryIfKeyId);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYSETVALID, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall dispatch the set key valid function to the configured crypto driver object.
 */
/*******************************************************************************/

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
Std_ReturnType CryIf_KeySetInValid(uint32 cryIfKeyId)
{
    CryIf_ArtiApiStart(2, CRYIF_SID_KEYSETINVALID, cryIfKeyId);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYSETINVALID, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall dispatch the get key element function to the configured crypto driver object.
 */
/*******************************************************************************/
/* PRQA S 6030,6060 ++ */ /* VL_MTR_CryIf_STMIF,VL_MTR_CryIf_STM19 */
Std_ReturnType CryIf_KeyElementGet(uint32 cryIfKeyId, uint32 keyElementId, uint8* resultPtr, uint32* resultLengthPtr)
/* PRQA S 6030,6060 -- */
{
    CryIf_ArtiApiStart(5, CRYIF_SID_KEYELEMENT_GET, cryIfKeyId, keyElementId, resultPtr, resultLengthPtr);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYELEMENT_GET, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
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
    CryIf_ArtiApiStart(5, CRYIF_SID_KEYELEMENT_COPY, cryIfKeyId, keyElementId, targetCryIfKeyId, targetKeyElementId);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYELEMENT_COPY, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
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
    CryIf_ArtiApiStart(
        8,
        CRYIF_SID_KEYELEMENT_COPYPARTIAL,
        cryIfKeyId,
        keyElementId,
        keyElementSourceOffset,
        keyElementTargetOffset,
        keyElementCopyLength,
        targetCryIfKeyId,
        targetKeyElementId);

    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
    uint8 sourceDriverId;
    uint8 targetDriverId;
#else /*STD_OFF == CRYIF_MUL_DRIVER_SUPPORT*/
#endif

    /* SWS_CryIf_91018 : Reentrant but not for the same cryIfKeyId*/
    if (CryIf_CurrentLockedKeyId == cryIfKeyId)
    {
        return ret;
    }
    CryIf_CurrentLockedKeyId = cryIfKeyId;

#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00137] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_COPYPARTIAL, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00138] */
    else if ((cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED) || (targetCryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED))
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYELEMENT_COPYPARTIAL, CRYIF_E_PARAM_HANDLE);
    }
    else
#endif
    {
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        sourceDriverId = CryIf_GetDriverId(cryIfKeyId);
        targetDriverId = CryIf_GetDriverId(targetCryIfKeyId);

        /* @req [SWS_CryIf_00139] */
        if (sourceDriverId == targetDriverId)
        {
#endif
            /* PRQA S 1258,1252 ++ */ /* VL_CryIf_1258,VL_CryIf_1252 */
            if (cryIfKeyId != targetCryIfKeyId)
            /* PRQA S 1258,1252 -- */
            {
                ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyElementCopyPartial_Name(
                    CryIf_GetDriverKeyId(cryIfKeyId),
                    keyElementId,
                    keyElementSourceOffset,
                    keyElementTargetOffset,
                    keyElementCopyLength,
                    CryIf_GetDriverKeyId(targetCryIfKeyId),
                    targetKeyElementId);
            }
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
        }
        else
        {
            /* @req [SWS_CryIf_00140] sourceDriverId != targetDriverId */
        }
#endif
    }
    CryIf_CurrentLockedKeyId = CRYIF_KEY_INVALID_ID;

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYELEMENT_COPYPARTIAL, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall copy all key elements from the source key to a target key.
 */
/*******************************************************************************/
/* PRQA S 6060, 2889 ++ */ /* VL_MTR_CryIf_STM19, VL_QAC_MultiReturn */
Std_ReturnType CryIf_KeyCopy(uint32 cryIfKeyId, uint32 targetCryIfKeyId)
/* PRQA S 6060, 2889 -- */
{
    CryIf_ArtiApiStart(3, CRYIF_SID_KEYCOPY, cryIfKeyId, targetCryIfKeyId);

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
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
        if (ret == CRYPTO_E_KEY_SIZE_MISMATCH)
        {
            CRYIF_DET_REPORT(CRYIF_SID_KEYCOPY, CRYIF_E_KEY_SIZE_MISMATCH);
        }
#endif
    }

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYCOPY, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall dispatch the random seed function to the configured cryptodriver object.
 */
/*******************************************************************************/
Std_ReturnType CryIf_RandomSeed(uint32 cryIfKeyId, const uint8* seedPtr, uint32 seedLength)
{
    CryIf_ArtiApiStart(4, CRYIF_SID_RANDOM_SEED, cryIfKeyId, seedPtr, seedLength);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_RANDOM_SEED, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall dispatch the key generate function to the configured crypto driver object.
 */
/*******************************************************************************/
Std_ReturnType CryIf_KeyGenerate(uint32 cryIfKeyId)
{
    CryIf_ArtiApiStart(2, CRYIF_SID_KEYGENERATE, cryIfKeyId);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYGENERATE, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall dispatch the key derive function to the configured crypto driver object.
 */
/*******************************************************************************/
/* PRQA S 6060, 2889 ++ */ /* VL_MTR_CryIf_STM19, VL_QAC_MultiReturn */
Std_ReturnType CryIf_KeyDerive(uint32 cryIfKeyId, uint32 targetCryIfKeyId)
/* PRQA S 6060, 2889 -- */
{
    CryIf_ArtiApiStart(3, CRYIF_SID_KEYDERIVE, cryIfKeyId, targetCryIfKeyId);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYDERIVE, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall dispatch the key exchange public value calculation
 * function to the configured crypto driver object.
 */
/*******************************************************************************/
Std_ReturnType CryIf_KeyExchangeCalcPubVal(uint32 cryIfKeyId, uint8* publicValuePtr, uint32* publicValueLengthPtr)
{
    CryIf_ArtiApiStart(4, CRYIF_SID_KEYEXCHANGECALCPUBVAL, cryIfKeyId, publicValuePtr, publicValueLengthPtr);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYEXCHANGECALCPUBVAL, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief This function shall dispatch the key exchange common shared secret
 * calculation function to the configured crypto driver object.
 */
/*******************************************************************************/
Std_ReturnType
    CryIf_KeyExchangeCalcSecret(uint32 cryIfKeyId, const uint8* partnerPublicValuePtr, uint32 partnerPublicValueLength)
{
    CryIf_ArtiApiStart(4, CRYIF_SID_KEYEXCHANGECALCSECRET, cryIfKeyId, partnerPublicValuePtr, partnerPublicValueLength);

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

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYEXCHANGECALCSECRET, ret);

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
    CryIf_ArtiApiStart(3, CRYIF_SID_CBNOTIFICATION, job, result);

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

    CryIf_ArtiApiStop(1, CRYIF_SID_CBNOTIFICATION);
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
/*******************************************************************************/
/**
 * @brief get the status of a key .
 */
/*******************************************************************************/
Std_ReturnType CryIf_KeyGetStatus(uint32 cryIfKeyId, Crypto_KeyStatusType* keyStatusPtr)
{
    CryIf_ArtiApiStart(3, CRYIF_SID_KEYGETSTATUS, cryIfKeyId, keyStatusPtr);

    Std_ReturnType ret = E_NOT_OK;
#if (STD_ON == CRYIF_DEV_ERROR_DETECT)
    /* @req [SWS_CryIf_00146] */
    if (CryIf_InitStatus != (boolean)TRUE)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYGETSTATUS, CRYIF_E_UNINIT);
    }
    /* @req [SWS_CryIf_00147] */
    else if (cryIfKeyId >= CRYIF_KEY_MAX_CONFIGURED)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYGETSTATUS, CRYIF_E_PARAM_HANDLE);
    }
    /* @req [SWS_CryIf_00148] */
    else if (NULL_PTR == keyStatusPtr)
    {
        CRYIF_DET_REPORT(CRYIF_SID_KEYGETSTATUS, CRYIF_E_PARAM_POINTER);
    }
    else
#endif
    {
#if (CRYPTO_MAXKEY_CONFIGURED > 0)
        /* @req [SWS_CryIf_00149] */
        ret = CryIf_DriverApi[CRYIF_SINGLE_DRIVER].Crypto_KeyGetStatus_Name(
            CryIf_GetDriverKeyId(cryIfKeyId),
            keyStatusPtr);
#endif
    }

    CryIf_ArtiApiStop(2, CRYIF_SID_KEYGETSTATUS, ret);

    return ret;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
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
#if (STD_ON == CRYIF_MUL_DRIVER_SUPPORT)
/*******************************************************************************/
/**
 * @brief          Maps a CryIf key ID to a CryIf_DriverApi[] index.
 * @details        Temporary stub until multi-core/multi-driver key routing is restored.
 *                 Currently always returns 0u so key APIs remain callable when
 *                 CRYIF_MUL_DRIVER_SUPPORT is enabled before the mapping lands.
 * @param[in]      keyid : CryIf key ID to resolve.
 * @return         uint8
 * @retval         0u: temporary stub index into CryIf_DriverApi[].
 * @trace       CPD-71590
 */
/*******************************************************************************/
#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
CRYIF_LOCAL uint8 CryIf_GetDriverId(uint32 keyid)
{
    /* TODO(multi-core): Implement keyid -> CryIf_DriverApi index mapping
     * (e.g. CryIf_KeyChannelId[] or equivalent). Do not extend CryIf_KeyCfgType. */
    (void)keyid;

    return 0u;
}
#define CRYIF_STOP_SEC_CODE
#include "CryIf_MemMap.h"
#endif

#define CRYIF_START_SEC_CODE
#include "CryIf_MemMap.h"
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
