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
 ************************************************************************************************************************
 **
 **  @file               : WdgIf.c
 **  @author             : xudong.guan
 **  @date               : 2024/08/20
 **  @vendor             : isoft
 **  @description        : Source file for WdgIf
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "WdgIf.h" /* PRQA S 0380 */ /* VL_WdgIf_0380 */
#if (STD_ON == WDGIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* STD_ON == WDGIF_DEV_ERROR_DETECT */

/* ===================================================== macros ===================================================== */
#define WDGIF_C_AR_MAJOR_VERSION 4U
#define WDGIF_C_AR_MINOR_VERSION 9U
#define WDGIF_C_AR_PATCH_VERSION 0U
#define WDGIF_C_SW_MAJOR_VERSION 2U
#define WDGIF_C_SW_MINOR_VERSION 2U
#define WDGIF_C_SW_PATCH_VERSION 1U

/* ------------------------------------------------- Version  Check ------------------------------------------------- */
#if (WDGIF_C_AR_MAJOR_VERSION != WDGIF_H_AR_MAJOR_VERSION)
#error "WdgIf.c:Mismatch in Specification Major Version"
#endif /*WDGIF_C_AR_MAJOR_VERSION != WDGIF_H_AR_MAJOR_VERSION*/
#if (WDGIF_C_AR_MINOR_VERSION != WDGIF_H_AR_MINOR_VERSION)
#error "WdgIf.c:Mismatch in Specification Minor Version"
#endif /*WDGIF_C_AR_MINOR_VERSION != WDGIF_H_AR_MINOR_VERSION*/
#if (WDGIF_C_AR_PATCH_VERSION != WDGIF_H_AR_PATCH_VERSION)
#error "WdgIf.c:Mismatch in Specification Patch Version"
#endif /*WDGIF_C_AR_PATCH_VERSION != WDGIF_H_AR_PATCH_VERSION*/
#if (WDGIF_C_SW_MAJOR_VERSION != WDGIF_H_SW_MAJOR_VERSION)
#error "WdgIf.c:Mismatch in Specification Major Version"
#endif /* WDGIF_C_SW_MAJOR_VERSION != WDGIF_H_SW_MAJOR_VERSION */
#if (WDGIF_C_SW_MINOR_VERSION != WDGIF_H_SW_MINOR_VERSION)
#error "WdgIf.c:Mismatch in Specification Minor Version"
#endif /* WDGIF_C_SW_MINOR_VERSION != WDGIF_H_SW_MINOR_VERSION */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */

/* ============================================ internal data definitions =========================================== */

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define WDGIF_START_SEC_CODE
#include "WdgIf_MemMap.h"

/*PRQA S 1532 ++*/ /*VL_QAC_OneFunRef*/
Std_ReturnType WdgIf_SetMode(uint8 DeviceIndex, WdgIf_ModeType WdgMode)
{
    Std_ReturnType ret = E_OK;
#if (WDGIF_NUMBER_OF_DEVICES > 1)
    const Wdg_GeneralType* gen;
#endif /*WDGIF_NUMBER_OF_DEVICES > 1*/
    if (DeviceIndex >= (uint8)WDGIF_NUMBER_OF_DEVICES)
    {
#if (WDGIF_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGIF_MODULE_ID, WDGIF_INSTANCE, WDGIF_SETMODE_ID, WDGIF_E_PARAM_DEVICE);
#endif /*WDGIF_DEV_ERROR_DETECT == STD_ON*/
        ret = E_NOT_OK;
    }
    else
    {
#if (WDGIF_NUMBER_OF_DEVICES <= 1)
        {
            ret = WdgIf_SetMode_Map(WdgMode);
        }
#else  /*WDGIF_NUMBER_OF_DEVICES > 1*/
        {
            gen = &WdgIf_DeviceData[DeviceIndex].WdgRef;
            if (gen->Wdg_SetMode != NULL_PTR)
            {
                ret = gen->Wdg_SetMode(WdgMode);
            }
        }
#endif /*WDGIF_NUMBER_OF_DEVICES <= 1*/
    }
    return ret;
} /*PRQA S 1532 --*/

/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void WdgIf_SetTriggerCondition(uint8 DeviceIndex, uint16 Timeout)
{
#if (WDGIF_NUMBER_OF_DEVICES > 1)
    const Wdg_GeneralType* gen;
#endif /*WDGIF_NUMBER_OF_DEVICES > 1*/
    if (DeviceIndex >= (uint8)WDGIF_NUMBER_OF_DEVICES)
    {
#if (WDGIF_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGIF_MODULE_ID, WDGIF_INSTANCE, WDGIF_TRIGGER_ID, WDGIF_E_PARAM_DEVICE);
#endif /*WDGIF_DEV_ERROR_DETECT == STD_ON*/
    }
    else
    {
#if (WDGIF_NUMBER_OF_DEVICES <= 1)
        {
            WdgIf_SetTriggerCondition_Map(Timeout);
        }
#else  /*WDGIF_NUMBER_OF_DEVICES > 1*/
        {
            gen = &WdgIf_DeviceData[DeviceIndex].WdgRef;
            if (gen->Wdg_SetTriggerCondition != NULL_PTR)
            {
                gen->Wdg_SetTriggerCondition(Timeout);
            }
        }
#endif /*WDGIF_NUMBER_OF_DEVICES <= 1*/
    }
}
/* PRQA S 1532 -- */

#if (STD_ON == WDGIF_VERSION_INFO_API)
void WdgIf_GetVersionInfo(Std_VersionInfoType* versionInfoPtr)
{
#if (STD_ON == WDGIF_DEV_ERROR_DETECT)
    if (NULL_PTR == versionInfoPtr)
    {
        (void)Det_ReportError((uint16)WDGIF_MODULE_ID, WDGIF_INSTANCE, WDGIF_GETVERSIONINFO_ID, WDGIF_E_PARAM_POINTER);
    }
    else
#endif /*WDGIF_DEV_ERROR_DETECT == STD_ON*/
    {
        versionInfoPtr->vendorID = WDGIF_VENDOR_ID;
        versionInfoPtr->moduleID = WDGIF_MODULE_ID;

        versionInfoPtr->sw_major_version = WDGIF_C_SW_MAJOR_VERSION;
        versionInfoPtr->sw_minor_version = WDGIF_C_SW_MINOR_VERSION;
        versionInfoPtr->sw_patch_version = WDGIF_C_SW_PATCH_VERSION;
    }
}
#endif

#define WDGIF_STOP_SEC_CODE
#include "WdgIf_MemMap.h"
