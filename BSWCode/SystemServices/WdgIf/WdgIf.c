/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*********************************************************************************
**                                                                            **
**  FILENAME    : WdgIf.c                                                     **
**                                                                            **
**  Created on  :                                                             **
**  Author      : haibin.shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

/*===========================[R E V I S I O N   H I S T O R Y]====================================*/
/*  <VERSION>    <DATE>    <AUTHOR>         <REVISION LOG>
 *  V1.0.0     20180116     xinyu.J       Initial version
 *  V1.0.1     20180713     Xinyu.J       Update the WdgIf_SetMode and WdgIf_SetTriggerCondition.
 *  V1.0.2     20190717     Xinyu.J       Update the note  after CodeReview
 *  V1.0.3     20221222     peng.wu       Modify the head and version info
 *  V2.1.0     20230714     Xudong.Guan   CP2.1 Release Version. Delete version info in other files
 *  V2.1.1     20231226     Peng.Wu       1.Update some QAC problems
 *  V2.1.2     20240528     Xudong.Guan   Multi-core branch code regression
 */
/*================================================================================================*/

/*===========================[V E R S I O N  I N F O R M A T I O N]===============================*/
#define WDGIF_C_AR_MAJOR_VERSION 4U
#define WDGIF_C_AR_MINOR_VERSION 5U
#define WDGIF_C_AR_PATCH_VERSION 0U
#define WDGIF_C_SW_MAJOR_VERSION 2U
#define WDGIF_C_SW_MINOR_VERSION 1U
#define WDGIF_C_SW_PATCH_VERSION 2U

/*===========================[I N C L U D E S]====================================================*/
#include "WdgIf.h"
#if (STD_ON == WDGIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif /* STD_ON == WDGIF_DEV_ERROR_DETECT */

/*===========================[V E R S I O N  C H E C K]===========================================*/
/* DD_2_260:(1)*/
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

/*===========================[M A C R O S]========================================================*/
/*===========================[T Y P E   D E F I N I T I O N S]====================================*/
/*===========================[I N T E R N A L   D A T A]==========================================*/
/*===========================[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/*===========================[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
#define WDGIF_START_SEC_CODE
#include "WdgIf_MemMap.h"
/**************************************************************************************************/
/*
 * Brief               <Service for switches the watchdog into the mode>
 * ServiceId           0x01
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <mode: 1.WDGIFIF_OFF_MODE; 2.WDGIFIF_SLOW_MODE; 3.WDGIFIF_FAST_MODE>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <Std_ReturnType: E_NOT_OK, or E_OK>
 * PreCondition        <The WDGIF driver module must not be initialled>
 * CallByAPI           <None>
 * REQ ID              DD_2_091, DD_2_092, DD_2_094, DD_2_096, DD_2_097,DD_2_152~DD_2_155
 */
/**************************************************************************************************/

FUNC(Std_ReturnType, WDGIF_CODE) WdgIf_SetMode(uint8 DeviceIndex, WdgIf_ModeType WdgMode)
{
    Std_ReturnType ret = E_NOT_OK;
#if (WDGIF_NUMBER_OF_DEVICES > 1)
    P2CONST(Wdg_GeneralType, AUTOMATIC, WDGIF_CONST) gen;
#endif /*WDGIF_NUMBER_OF_DEVICES > 1*/

#if (WDGIF_DEV_ERROR_DETECT == STD_ON)
    if ((DeviceIndex >= (uint8)WDGIF_NUMBER_OF_DEVICES) || (WdgMode > WDGIF_FAST_MODE))
    {
        (void)Det_ReportError(WDGIF_MODULE_ID, WDGIF_INSTANCE, WDGIF_SETMODE_ID, WDGIF_E_PARAM_DEVICE);
        ret = E_NOT_OK;
    }
    else
#endif /*WDGIF_DEV_ERROR_DETECT == STD_ON*/
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
}

/**************************************************************************************************/
/*
 * Brief               <Service for triggers the watchdog hardware>
 * ServiceId           0x02
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <The WDGIF driver module must not be initialled>
 * CallByAPI           <None>
 * REQ ID              DD_2_091,DD_2_093,DD_2_095,DD_2_096,DD_2_097,DD_2_156,DD_2_157
 */
/**************************************************************************************************/
FUNC(void, WDGIF_CODE) WdgIf_SetTriggerCondition(uint8 DeviceIndex, uint16 Timeout)
{
#if (WDGIF_NUMBER_OF_DEVICES > 1)
    P2CONST(Wdg_GeneralType, AUTOMATIC, WDGIF_CONST) gen;
#endif /*WDGIF_NUMBER_OF_DEVICES > 1*/

#if (WDGIF_DEV_ERROR_DETECT == STD_ON)
    if (DeviceIndex >= (uint8)WDGIF_NUMBER_OF_DEVICES)
    {
        (void)Det_ReportError(WDGIF_MODULE_ID, WDGIF_INSTANCE, WDGIF_TRIGGER_ID, WDGIF_E_PARAM_DEVICE);
    }
    else
#endif /*WDGIF_DEV_ERROR_DETECT == STD_ON*/
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

/**************************************************************************************************/
/*
 * Brief               <Returns the version information of this module>
 * ServiceId           <WDGIF_GETVERSIONINFO_ID>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <None>
 * Param-Name[out]     <versionInfo: Pointer to where to store the version information
 *                      of this module>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <None>
 * REQ ID              DD_2_251,DD_2_265,DD_2_266
 */
/**************************************************************************************************/
#if (STD_ON == WDGIF_VERSION_INFO_API)
FUNC(void, WDGIF_CODE) WdgIf_GetVersionInfo(Std_VersionInfoType* versionInfoPtr)
{
#if (STD_ON == WDGIF_DEV_ERROR_DETECT)
    if (NULL_PTR == versionInfoPtr)
    {
        (void)Det_ReportError((uint16)WDGIF_MODULE_ID, WDGIF_INSTANCE, WDGIF_GETVERSIONINFO_ID, WDGIF_E_INV_POINTER);
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

/*===========================[E N D   O F   F I L E]==============================================*/
