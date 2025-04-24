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
**  FILENAME    : KeyM.c                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implement KeyM general functions                            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R21-11                      **
**                                                                            **
*******************************************************************************/

/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0     2022-02-15   qinchun.yang     initial version.
 *  V1.0.1     2023-03-21   qinchun.yang     CPD-3718,CPD-3719,CPD-3720,CPD-3721,CPD-3724,CPD-3725
 *                                           CPD-3726,CPD-3727,CPD-3728,CPD-3730,CPD-3769.
 *  V1.0.2     2023-03-21   qinchun.yang     Modify initialization processing persistent storage certificate logic.
 *  V1.0.3     2023-03-24   qinchun.yang     Versions that have undergone functional testing.
 *  V1.0.4     2023-04-26     jie.gu         modify for QAC MISRAC Test
 *  V1.0.5     2023-05-05     jie.gu         CPD-32052,CPD-32053,CPD-32054,CPD-32055,CPD-32056,CPD-3792,
 *                                           CPD-3769,CPD-3748,CPD-3747,CPD-3726,CPD-3725,CPD-3767,CPD-3793
 *  V1.0.6     2023-05-11     jie.gu         CPD-32065,modify for UnitTest
 *  V2.1.0     2023-07-17     jie.gu         CP2.1 Release Version
 *  V2.1.1     2023-08-16     jie.gu         CPT-6554 change certificate verify in Asymc mode
 *  V2.1.2     2023-08-21     jie.gu         CPD-32986 reserve return value of Csm_KeyElementSet, KeyM_HandleUpdate
 *  V2.1.3     2023-08-23     jie.gu         CPT-6588 add micro for nvmblock and cert array
 *  V2.1.4     2023-09-04     jie.gu         CPT-6775 notificate all the verify fault to dcm
 *  V2.1.5     2023-09-05     jie.gu         CPT-6810,modify data element judge condition
 *  V2.1.6     2023-11-20     jie.gu         CPD-33582 add nvmblock length check，fix return value check
 *  V2.1.7     2024-01-31     jie.gu         CPD-33716 update code for misra c rules check
 *  V2.1.8     2024-05-27     jie.gu         CPD-33710 add exclusive areas for keym
 *  V2.1.9     2024-06-21     jie.gu         Add const memory section
 *  V2.1.10    2024-08-20     jie.gu         Clear Compile warning
 *  V2.1.11    2024-11-23     jian.jiang     modify for QAC MISRAC Test
 */

/**
 \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
   ModeName:KeyM<br>
 RuleSorce:puhua-rule.rcf 2.3.1

   \li PRQA S 3432 MISRA Rule 20.7 .<br>
   Reason:Function-like macros are used to allow more efficient code

   \li PRQA S 1532 MISRA Rule 8.7,Rule 2.1 .<br>
   Reason:Functions may be used in other projects.

   \li PRQA S 1532 MISRA Rule 8.4 .<br>
   Reason:Mainfunction declared in Schm

   \li PRQA S 3432 MISRA Dir 4.1 .<br>
   Reason:Loop can be executed more than once in actual projects

   \li PRQA S 3673 MISRA Rule 8.13 .<br>
   Reason: Parameters not used because configuration is not turned on
*/

/*=======[V E R S I O N  I N F O R M A T I O N]===============================*/
#define KEYM_C_AR_MAJOR_VERSION 4u
#define KEYM_C_AR_MINOR_VERSION 7u
#define KEYM_C_AR_PATCH_VERSION 0u
#define KEYM_C_SW_MAJOR_VERSION 2u
#define KEYM_C_SW_MINOR_VERSION 1u
#define KEYM_C_SW_PATCH_VERSION 11u
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "KeyM_Internal.h"
/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
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

/*******************************************************************************
**                      Private Macro Definitions                             **
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
#define KEYM_START_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
VAR(boolean, KEYM_VAR)
KeyM_InitStatus;
#define KEYM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "KeyM_MemMap.h"
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define KEYM_START_SEC_CODE
#include "KeyM_MemMap.h"
/*************************************************************************/
/*
 * Brief               This function initializes the key management module.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to the configuration set in VARIANT-POST-BUILD.
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 1532,1503 ++ */ /* MISRA Rule 8.7,Rule 2.1 */
FUNC(void, KEYM_CODE)
KeyM_Init(P2CONST(KeyM_ConfigType, AUTOMATIC, KEYM_APPL_DATA) ConfigPtr)
/* PRQA S 1532,1503 -- */ /* MISRA Rule 8.7,Rule 2.1 */
{
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certCfgPtr;
    uint16 certId;
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
        for (certId = 0u; certId < KEYM_CERT_NUM; certId++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            certCfgPtr = &KeyM_CertPCfg[certId];
            (void)KeyM_CertSetStatus(certId, KEYM_CERTIFICATE_NOT_AVAILABLE);
            if (certCfgPtr->certStorage == KEYM_STORAGE_IN_CSM)
            {
                if ((certCfgPtr->selfSignedCert != TRUE) && (certCfgPtr->certKeyTarRef != NULL_PTR))
                {
                    KeyM_HandleCsmKeyStorage(*(certCfgPtr->certKeyTarRef), certId, FALSE);
                    (void)KeyM_CertSetStatus(certId, KEYM_CERTIFICATE_PARSED_NOT_VALIDATED);
                }
            }
#if (KEYM_NVM_BLOCK_NUM > 0u)
            else if (certCfgPtr->certStorage == KEYM_STORAGE_IN_NVM)
            {
                (void)NvM_ReadBlock(
                    KeyM_NvmBlockPCfg[(*KeyM_CertPCfg[certId].certNvmBlkRef)].blkId,
                    KeyM_CertPCfg[certId].tbsCert->dataEle);
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
 * Brief               This function resets the key management module to
 *                     the uninitialized state.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, KEYM_CODE)
KeyM_Deinit(void) /* PRQA S 1503*/ /* MISRA Rule 2.1 */
{
#if (STD_ON == KEYM_DEVERROR_DETECT)
    if ((boolean)FALSE == KeyM_InitStatus)
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
 * Brief               This function provides the version of this module.
 * ServiceId           0x03
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     VersionInfo: Pointer to the version information of this module.
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
/* PRQA S 3432 ,1503 ++ */ /* MISRA Rule 20.7 Rule 2.1 */
FUNC(void, KEYM_CODE)
KeyM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, KEYM_APPL_DATA) VersionInfo)
/* PRQA S 3432 ,1503 -- */ /* MISRA Rule 20.7 Rule 2.1 */
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
 * Brief               Function is called periodically according the specified time interval.
 * ServiceId           0x19
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None.
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, KEYM_CODE)
KeyM_MainFunction(void) /* PRQA S 3408,1503 */ /* MISRA Rule 8.4,Rule 2.1 */
{
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
    KeyM_CertStoreNvmHandle();
#endif /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/
}
/*************************************************************************/
/*
 * Brief               Function is called from a pre-emptive operating system
 *                     when no other task operation is needed
 * ServiceId           0x1a
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None.
 * Param-Name[in/out]  None
 * Return              None
 */
/*************************************************************************/
FUNC(void, KEYM_CODE)
KeyM_MainBackgroundFunction(void) /* PRQA S 3408,1503 */ /* MISRA Rule 8.4,Rule 2.1 */
{
#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
    P2CONST(KeyM_CertPCfgType, AUTOMATIC, KEYM_CONST)
    certCfgPtr;
    P2CONST(KeyM_CertPCfgType, TYPEDEF, KEYM_CONST)
    certUpperHierRef;
    uint16 certId;
    KeyM_CertificateStatusType certStatu;
#endif /*STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED*/

#if (STD_ON == KEYM_CERTIFICATE_MANAGER_ENABLED)
    for (certId = 0u; certId < KEYM_CERT_NUM; certId++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
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
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(void, KEYM_CODE)
KeyM_CopyData(
    /* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
    /* PRQA S 3673 ++ */ /* MISRA Rule 8.13 */
    P2VAR(void, AUTOMATIC, KEYM_APPL_DATA) dest,
    /* PRQA S 3673 -- */ /* MISRA Rule 8.13 */
    P2CONST(void, AUTOMATIC, KEYM_CONST) src,
    uint32 size)
{
    uint32 idx;
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) /* PRQA S 3432 */ /* MISRA Rule 20.7 */
    destPtr = (uint8*)dest;
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA)
    srcPtr = src;

    if (dest == src)
    {
        return;
    }
    if ((src != NULL_PTR) && (dest != NULL_PTR) && (size != 0u))
    {
        for (idx = 0; idx < size; idx++)
        {
            destPtr[idx] = srcPtr[idx];
        }
    }
}
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
FUNC(Std_ReturnType, KEYM_CODE)
KeyM_strcmp(

    P2CONST(uint8, AUTOMATIC, KEYM_CONST) str1,
    P2CONST(uint8, AUTOMATIC, KEYM_CONST) str2,
    uint16 size)
{
    /* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
    uint16 len;
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
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
