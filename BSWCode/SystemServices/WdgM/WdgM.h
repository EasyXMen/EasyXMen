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
/*********************************************************************************
**                                                                            **
**  FILENAME    : WdgM.h                                                      **
**                                                                            **
**  Created on  :                                                             **
**  Author      : haibin.shao                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S):   AUTOSAR classic Platform R19-11                       **
**                                                                            **
*******************************************************************************/

/*======================[R E V I S I O N   H I S T O R Y]=====================*/
/*============================================================================*/

#ifndef WDGM_H_
#define WDGM_H_

/******************************INCLUDE*********************************/
#include "WdgM_CfgType.h"
#include "WdgM_Cfg.h"
#include "Std_Types.h"

/*********************************WDGM Version Definition*************************************/
#define WDGM_VENDOR_ID          62u /*DD_2_212, Vendor ID*/
#define WDGM_INSTANCE_ID        0u  /*DD_2_213, Instance ID*/

#define WDGM_H_SW_MAJOR_VERSION 2u /*DD_2_214, Major Version*/
#define WDGM_H_SW_MINOR_VERSION 0u /*DD_2_215, Minor Version*/
#define WDGM_H_SW_PATCH_VERSION 3u /*DD_2_216, Patch Version*/
#define WDGM_H_AR_MAJOR_VERSION 4u /*DD_2_217, Autosar Major Version*/
#define WDGM_H_AR_MINOR_VERSION 5u /*DD_2_218, Autosar Minor Version*/
#define WDGM_H_AR_PATCH_VERSION 0u /*DD_2_219, Autosar Patch Version*/

#define WDGM_MODULE_ID          13u /*DD_2_220*/

/******************************API ID Definition *********************************/
#define WDGM_INIT_ID                   (0x00u)  /*DD_2_221, WdgM_Init()*/
#define WDGM_DEINIT_ID                 (0x01u)  /*DD_2_222, WdgM_DeInit()*/
#define WDGM_GET_VERSION_INFO_ID       (0x02u)  /*DD_2_223, WdgM_GetVersionInfo()*/
#define WDGM_SET_MODE_ID               (0x03u)  /*DD_2_224, WdgM_SetMode()*/
#define WDGM_MAINFUNCTION_ID           (0x08u)  /*DD_2_225, WdgM_MainFunction()*/
#define WDGM_GET_MODE_ID               (0x0Bu)  /*DD_2_226, WdgM_GetMode()*/
#define WDGM_GET_LOCAL_STATUS_ID       (0x0Cu)  /*DD_2_227, WdgM_GetLocalStatus()*/
#define WDGM_GET_GLOBAL_STATUS_ID      (0x0Du)  /*DD_2_228, WdgM_GetGlobalStatus()*/
#define WDGM_CHECKPOINT_REACHED_ID     (0x0Eu)  /*DD_2_229, WdgM_CheckpointReached()*/
#define WDGM_PERFORM_RESET_ID          (0x0Fu)  /*DD_2_230, WdgM_PerformReset()*/
#define WDGM_GET_FIRST_EXPIRED_SEID_ID (0x010u) /*DD_2_231, WdgM_GetFirstExpiredSEID()*/
/*************************************************************************************/

/******************************Det Error Definition*********************************/
/*DD_2_183*/
/*Development Errors*/
#define WDGM_E_UNINIT       (0x10u) /*without module initialization*/
#define WDGM_E_PARAM_CONFIG (0x11u) /*API service Wdg_Init was called with an erroneous configuration set*/
#define WDGM_E_PARAM_MODE   (0x12u) /*API service called with wrong mode paramete*/
#define WDGM_E_PARAM_SEID   (0x13u) /*API service called with wrong supervised entity identifier parameter*/
#define WDGM_E_INV_POINTER  (0x14u) /*API service called with invalid pointer*/
#define WDGM_E_CPID         (0x16u) /*API service used with an invalid CheckpointId*/

/*Runtime Errors*/
#define WDGM_E_DISABLE_NOT_ALLOWED (0x15u) /*Disabling of watchdog not allowed (e.g. in safety relevant systems)*/
#define WDGM_E_SEDEACTIVATED                                                                                        \
    (0x19u) /*API service used with a checkpoint of a Supervised Entity that is deactivated in the current Watchdog \
               Manager mode.*/
#define WDGM_E_SET_MODE (0x1D) /*Watchdog drivers' mode switch has failed*/

/*************************************************************************************/

/*******************************The other Definition**********************************/
#define WDGM_TIME_RESOLUTION        1000UL /*DD_2_211*/
#define WDGM_NO_OS_APPLICAITON_REF  0xFFFF
#define WDGM_NO_CP_REACH            0xFFFF
#define WDGM_NO_SE_REACH            0
#define WDGM_TRIGGER_CONDITION_STOP 0u /*Trigger condition for Stop*/

/*************************************************************************************/

/***************************extern variable********************************************/

/* WdgM configuration. Used by WdgMInit() */
extern const WdgM_ConfigType WdgMConfigRoot[1]; /*DD_2_079*/

/*************************************************************************************/

extern FUNC(void, WDGM_CODE) WdgM_Init(const WdgM_ConfigType* WdgMConfigPtr);
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetMode(WdgM_ModeType* Mode);
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_SetMode(WdgM_ModeType Mode);
extern FUNC(Std_ReturnType, WDGM_CODE)
    WdgM_CheckpointReached(WdgM_SupervisedEntityIdType SEID, WdgM_CheckpointIdType CheckpointID);
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetGlobalStatus(WdgM_GlobalStatusType* Status);
extern FUNC(void, WDGM_CODE) WdgM_PerformReset(void);
extern FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetFirstExpiredSEID(WdgM_SupervisedEntityIdType* SEID);
extern FUNC(Std_ReturnType, WDGM_CODE)
    WdgM_GetLocalStatus(WdgM_SupervisedEntityIdType SEID, WdgM_LocalStatusType* Status);
#if (STD_ON == WDGM_OFF_MODE_ENABLED)
extern FUNC(void, WDGM_CODE) WdgM_DeInit(void);
#endif /*STD_ON == WDGM_OFF_MODE_ENABLED*/

#if (WDGM_VERSION_INFO_API == STD_ON)
extern FUNC(void, WDGM_CODE) WdgM_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif /*WDGM_VERSION_INFO_API == STD_ON*/

#endif /* WDGM_H_ */
