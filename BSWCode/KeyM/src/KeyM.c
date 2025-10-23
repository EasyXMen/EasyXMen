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
 **  @file               : KeyM.c
 **  @author             : jie.gu
 **  @date               : 2025/03/31
 **  @vendor             : isoft
 **  @description        : KeyM Interface Implementation source
 **
 **********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "KeyM_Internal.h"

/* ===================================================== macros ===================================================== */
#define KEYM_C_AR_MAJOR_VERSION 4u
#define KEYM_C_AR_MINOR_VERSION 9u
#define KEYM_C_AR_PATCH_VERSION 0u
#define KEYM_C_SW_MAJOR_VERSION 2u
#define KEYM_C_SW_MINOR_VERSION 2u
#define KEYM_C_SW_PATCH_VERSION 0u

#if (KEYM_C_AR_MAJOR_VERSION != KEYM_AR_MAJOR_VERSION)
#error "KeyM.c : Mismatch in Specification Major Version"
#endif
#if (KEYM_C_AR_MINOR_VERSION != KEYM_AR_MINOR_VERSION)
#error "KeyM.c : Mismatch in Specification Major Version"
#endif
#if (KEYM_C_AR_PATCH_VERSION != KEYM_AR_PATCH_VERSION)
#error "KeyM.c : Mismatch in Specification Major Version"
#endif
#if (KEYM_C_SW_MAJOR_VERSION != KEYM_SW_MAJOR_VERSION)
#error "KeyM.c : Mismatch in Specification Major Version"
#endif
#if (KEYM_C_SW_MINOR_VERSION != KEYM_SW_MINOR_VERSION)
#error "KeyM.c : Mismatch in Specification Major Version"
#endif
#if (KEYM_C_SW_PATCH_VERSION != KEYM_SW_PATCH_VERSION)
#error "KeyM.c : Mismatch in Specification Major Version"
#endif

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */
#define KEYM_START_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
/**
 * @brief Initialization status of the KeyM module.
 * @trace
 * @implement CPD-56265
 */
boolean KeyM_InitStatus;
#define KEYM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"

/* ========================================== external function definitions ========================================= */
#define KEYM_START_SEC_CODE
#include "KeyM_MemMap.h"
/*************************************************************************/
/*
 * Brief  This function initializes the key management module.
 */
/*************************************************************************/
/* PRQA S 1532,1503 ++ */ /* VL_QAC_OneFunRef,VL_QAC_NoUsedApi */
void KeyM_Init(const KeyM_ConfigType* ConfigPtr)
/* PRQA S 1532,1503 -- */
{
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
    const KeyM_CertPCfgType* certCfgPtr;
    uint16                   certId;
#endif /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/

    if (ConfigPtr != NULL_PTR)
    {
#if (KEYM_DEVERROR_DETECT == STD_ON)
        KEYM_DET_REPORT(KEYM_INIT_ID, KEYM_E_INIT_FAILED);
#endif
    }
    else
    {
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
        /*SWS_KeyM_00167*/
        /* PRQA S 2877 ++ */ /* VL_KeyM_2877 */
        for (certId = 0u; certId < KEYM_CERT_NUM; certId++)
        /* PRQA S 2877 -- */
        {
            certCfgPtr = &KeyM_CertPCfg[certId];
            (void)KeyM_CertSetStatus(certId, KEYM_CERTIFICATE_NOT_AVAILABLE);
            if (certCfgPtr->certStorage == KEYM_STORAGE_IN_CSM)
            {
                /* PRQA S 2109 ++ */ /* VL_KeyM_2109 */
                if ((!certCfgPtr->selfSignedCert) && (certCfgPtr->certKeyTarRef != NULL_PTR))
                /* PRQA S 2109 -- */
                {
                    KeyM_HandleCsmKeyStorage(*(certCfgPtr->certKeyTarRef), certId, FALSE);
                    (void)KeyM_CertSetStatus(certId, KEYM_CERTIFICATE_PARSED_NOT_VALIDATED);
                }
            }
#if (KEYM_NVM_BLOCK_NUM > 0u)
            else if (certCfgPtr->certStorage == KEYM_STORAGE_IN_NVM)
            {
                (void)NvM_ReadBlock((*KeyM_CertPCfg[certId].certNvmBlkRef), KeyM_CertPCfg[certId].tbsCert->dataEle);
                (void)KeyM_CertSetStatus(certId, KEYM_CERTIFICATE_NOT_PARSED);
            }
#endif /*KEYM_NVM_BLOCK_NUM > 0*/
            else
            {
                /*Do nothing.*/
            }
        }
#endif /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/
        KeyM_InitStatus = TRUE;
    }
}

/*************************************************************************/
/*
 * Brief  This function resets the key management module to
 */
/*************************************************************************/
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
void KeyM_Deinit(void)
/* PRQA S 1503 -- */
{
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if (!KeyM_InitStatus)
    {
        KEYM_DET_REPORT(KEYM_DEINIT_ID, KEYM_E_UNINIT);
    }
    else
#endif
    {
        KeyM_InitStatus = (boolean)FALSE;
    }
}

/*************************************************************************/
/*
 * Brief  This function provides the version of this module.
 */
/*************************************************************************/
/* PRQA S 3432,1503 ++ */ /* VL_KeyM_3432,VL_QAC_NoUsedApi */
void KeyM_GetVersionInfo(Std_VersionInfoType* VersionInfo)
/* PRQA S 3432,1503 -- */
{
#if (KEYM_DEVERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfo)
    {
        KEYM_DET_REPORT(KEYM_GETVERSIONINFO_ID, KEYM_E_PARAM_POINTER);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        VersionInfo->moduleID = KEYM_MODULE_ID;
        VersionInfo->vendorID = KEYM_H_VENDOR_ID;

        VersionInfo->sw_major_version = KEYM_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = KEYM_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = KEYM_SW_PATCH_VERSION;
    }
}

/*************************************************************************/
/*
 * Brief  Function is called periodically according the specified time interval.
 */
/*************************************************************************/
/* PRQA S 3408,1503,1532 ++ */ /* VL_KeyM_3408,VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
void KeyM_MainFunction(void)
/* PRQA S 3408,1503,1532 -- */
{
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
    KeyM_CertStoreNvmHandle();
#endif /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/
}
/*************************************************************************/
/*
 * Brief  Function is called from a pre-emptive operating system
 */
/*************************************************************************/
/* PRQA S 3408,1503,1532 ++ */ /* VL_KeyM_3408,VL_QAC_NoUsedApi,VL_QAC_OneFunRef */
void KeyM_MainBackgroundFunction(void)
/* PRQA S 3408,1503,1532 -- */
{
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
    const KeyM_CertPCfgType*   certCfgPtr;
    const KeyM_CertPCfgType*   certUpperHierRef;
    uint16                     certId;
    KeyM_CertificateStatusType certStatu;
#endif /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/

#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
    /* PRQA S 2877 ++ */ /* VL_KeyM_2877 */
    for (certId = 0u; certId < KEYM_CERT_NUM; certId++)
    /* PRQA S 2877 -- */
    {
        certCfgPtr = &KeyM_CertPCfg[certId];
        (void)KeyM_CertGetStatus(certId, &certStatu);
        if ((certCfgPtr->certStorage == KEYM_STORAGE_IN_NVM) && (certStatu == KEYM_CERTIFICATE_NOT_PARSED))
        {
            (void)KeyM_HandleParseCert(certId, certCfgPtr->tbsCert->dataEle, certCfgPtr->tbsCert->len);
        }
        if (certStatu == KEYM_CERTIFICATE_PARSED_NOT_VALIDATED)
        {
            certUpperHierRef = certCfgPtr->certUpperHierRef;
            (void)KeyM_HandleCertcVerify(certCfgPtr, certUpperHierRef);
        }
    }
#endif /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/
}
/* PRQA S 1532,3673,2889 ++ */ /* VL_QAC_OneFunRef,VL_QAC_3673,VL_QAC_MultiReturn */
void KeyM_CopyData(void* dest, const void* src, uint32 size)
/* PRQA S 1532,3673,2889 -- */
{
    uint32 idx;
    /* PRQA S 0316,0311 ++ */ /* VL_QAC_0316,VL_KeyM_0311 */
    uint8*       destPtr = (uint8*)dest;
    const uint8* srcPtr  = (uint8*)src;
    /* PRQA S 0316,0311 -- */
    if (dest == src)
    {
        return;
    }
    /* PRQA S 1252 ++ */ /* VL_KeyM_1252 */
    if ((src != NULL_PTR) && (dest != NULL_PTR) && (size != 0u))
    /* PRQA S 1252 -- */
    {
        for (idx = 0u; idx < size; idx++)
        {
            destPtr[idx] = srcPtr[idx];
        }
    }
}
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Std_ReturnType KeyM_strcmp(const uint8* str1, const uint8* str2, uint16 size)
/* PRQA S 1532 -- */
{
    uint16         len;
    Std_ReturnType ret = E_OK;

    for (len = 0u; len < size; len++)
    {
        if (str1[len] != str2[len])
        {
            break;
        }
    }
    if (len != size)
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#define KEYM_STOP_SEC_CODE
#include "KeyM_MemMap.h"

/* ========================================== internal function definitions ========================================= */
