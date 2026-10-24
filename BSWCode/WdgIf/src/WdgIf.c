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
#include "WdgIf_Arti.h"

/* ========================================== external function definitions ========================================= */

#define WDGIF_START_SEC_CODE
#include "WdgIf_MemMap.h"
/*PRQA S 1532 ++*/ /*VL_QAC_OneFunRef*/
Std_ReturnType WdgIf_SetMode(uint8 DeviceIndex, WdgIf_ModeType WdgMode)
{
    WdgIf_ArtiApiStart(3, WDGIF_SETMODE_ID, DeviceIndex, WdgMode);

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
            ret = WdgIf_SetModeMap(WdgMode);
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

    WdgIf_ArtiApiStop(2, WDGIF_SETMODE_ID, ret);

    return ret;
}
/*PRQA S 1532 --*/
#define WDGIF_STOP_SEC_CODE
#include "WdgIf_MemMap.h"

#define WDGIF_START_SEC_CODE
#include "WdgIf_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void WdgIf_SetTriggerCondition(uint8 DeviceIndex, uint16 Timeout)
{
    WdgIf_ArtiApiStart(3, WDGIF_TRIGGER_ID, DeviceIndex, Timeout);

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
            WdgIf_SetTriggerConditionMap(Timeout);
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

    WdgIf_ArtiApiStop(1, WDGIF_TRIGGER_ID);
}
/* PRQA S 1532 -- */
#define WDGIF_STOP_SEC_CODE
#include "WdgIf_MemMap.h"

#if (STD_ON == WDGIF_VERSION_INFO_API)
#define WDGIF_START_SEC_CODE
#include "WdgIf_MemMap.h"
void WdgIf_GetVersionInfo(Std_VersionInfoType* versionInfoPtr)
{
    WdgIf_ArtiApiStart(2, WDGIF_GETVERSIONINFO_ID, versionInfoPtr);

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

        versionInfoPtr->sw_major_version = WDGIF_SW_MAJOR_VERSION;
        versionInfoPtr->sw_minor_version = WDGIF_SW_MINOR_VERSION;
        versionInfoPtr->sw_patch_version = WDGIF_SW_PATCH_VERSION;
    }

    WdgIf_ArtiApiStop(1, WDGIF_GETVERSIONINFO_ID);
}
#define WDGIF_STOP_SEC_CODE
#include "WdgIf_MemMap.h"
#endif
