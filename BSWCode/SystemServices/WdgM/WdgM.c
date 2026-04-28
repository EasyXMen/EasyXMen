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
/*********************************************************************************
**                                                                            **
**  FILENAME    : WdgM.c                                                      **
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
/*  <VERSION>    <DATE>    <AUTHOR>        <REVISION LOG>
 *  V1.0.0     20180116     xinyu.J        Initial version
 *  V1.0.1     20180713     Xinyu.J        1. Modify the type definition base on the Req356/357/358
 *                                         2. Delete the function of checking SE deactivate or activate,
 *                                         which is realized by the ORIENTAIS.
 *  V1.0.2     20190703     Xinyu.J        Add the Function WdgM_GetSupervisedEntityTabIndex
 *  V1.0.3     20190717     Xinyu.J        Update the note and code after CodeReview
 *  V1.0.4     20221222     peng.wu        Modify the head and version info
 *  V1.0.5     20230424     peng.wu        Fix the bugs during logic supervision
 *  V1.0.6     20230630     Xudong.Guan    1.Fix spelling errors
 *                                         2.Modify and add errors classification
 *                                         3.Add the macros switch about BSWM
 *  V1.0.7     20230712     Xudong.Guan    1.Fix some bugs
 *                                         2.Delete WdgMDeInitFlag
 *                                         3.Modify some process about WDGM is not initialized
 *  V1.0.8     20230817     peng.wu        1.Fix CPT-6093 CPT-6205;
 *  V1.0.9     20231120     haibin.shao    Update the SchM for WdgM's interaction with ShutDownOS
 *  V2.1.0     20230714     Xudong.Guan    CP2.1 Release Version. Delete version info in other files
 *  V2.1.1     20230804     Peng.Wu        1.Add WdgMDeInitFlag; 2.Fix spelling errors;3.Delete no used macros
 *  V2.1.2     20231120     Peng.Wu        1.Add multi-core supervision
 *  V2.1.3     20231226     Peng.Wu        1.Fix CPT-7826,CPT-7834; 2.Update some QAC problems
 *  V2.1.4     20240228     Jian.Jiang     Rectification of QAC based on new rule sets
 *  V2.1.5     20240528     Xudong.Guan    Multi-core branch code regression
 *  V2.1.6     20240730     Xudong.Guan    1.Delete WdgMDeInitFlag; 2.Change the time obtaining method.
 *  V2.1.7     20240801     Xudong.Guan    1.Add external function declarations; 2.Change the time interval calculation
 *                                         method of Deadline Supervision.
 *  V2.1.8     20240808     Xudong.Guan    Modify the initialization logic for multiple devices.
 */
/*============================================================================*/

/* PRQA S 1513 EOF */ /* VL_WdgM_1513 */

/*=======[I N C L U D E S]====================================================*/
#include "WdgIf.h"
#include "WdgM.h"
#if (WDGM_DEADLINE_SUP_NUM > 0 || WDGM_MULTI_PARTITION_ENABLED == STD_ON)
#include "Os.h"
#endif
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON && WDGM_BSWM_ENABLED == STD_ON)
#include "BswM_WdgM.h"
#endif
#include "SchM_WdgM.h"
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
#include "Det.h"
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
#if (STD_ON == WDGM_E_SUPERVISION_ENABLE)
#include "Dem.h"
#endif /*STD_ON == WDGM_E_SUPERVISION_ENABLE */
#if (STD_ON == WDGM_IMMEDIATE_RESET)
#include "Mcu.h"
#endif

/*******************************************************************/

#define WDGM_C_SW_MAJOR_VERSION 2u /*Major Version*/
#define WDGM_C_SW_MINOR_VERSION 0u /*Minor Version*/
#define WDGM_C_SW_PATCH_VERSION 3u /*Patch Version*/
#define WDGM_C_AR_MAJOR_VERSION 4u /*Autosar Major Version*/
#define WDGM_C_AR_MINOR_VERSION 5u /*Autosar Minor Version*/
#define WDGM_C_AR_PATCH_VERSION 0u /*Autosar Patch Version*/

/*=======[V E R S I O N  C H E C K]===========================================*/
/* DD_2_259:(1)*/
#if (WDGM_C_AR_MAJOR_VERSION != WDGM_H_AR_MAJOR_VERSION)
#error "WdgM.c : Mismatch in Specification Major Version"
#endif /*WDGM_C_AR_MAJOR_VERSION != WDGM_H_AR_MAJOR_VERSION*/
#if (WDGM_C_AR_MINOR_VERSION != WDGM_H_AR_MINOR_VERSION)
#error "WdgM.c : Mismatch in Specification Minor Version"
#endif /*WDGM_C_AR_MINOR_VERSION != WDGM_H_AR_MINOR_VERSION*/
#if (WDGM_C_AR_PATCH_VERSION != WDGM_H_AR_PATCH_VERSION)
#error "WdgM.c : Mismatch in Specification Patch Version"
#endif /*WDGM_C_AR_PATCH_VERSION != WDGM_H_AR_PATCH_VERSION*/
#if (WDGM_C_SW_MAJOR_VERSION != WDGM_H_SW_MAJOR_VERSION)
#error "WdgM.c : Mismatch in Specification Major Version"
#endif /*WDGM_C_SW_MAJOR_VERSION != WDGM_H_SW_MAJOR_VERSION*/
#if (WDGM_C_SW_MINOR_VERSION != WDGM_H_SW_MINOR_VERSION)
#error "WdgM.c : Mismatch in Specification Minor Version"
#endif /*WDGM_C_SW_MINOR_VERSION != WDGM_H_SW_MINOR_VERSION*/

/***************************Static variable********************************************/

#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
#if (WDGM_SE_NUM > 0)
/*Save the status of the supervision entities*/
static WdgM_SEInfoType WdgM_SEInfoRelated[WDGM_SE_NUM]; /*DD_2_034, DD_2_205*/
#endif                                                  /*WDGM_SE_NUM > 0*/

#if (WDGM_ALIVE_NUM > 0)
/*Save the status of the alive supervision*/
static WdgM_AliveSupInfoType WdgM_AliveSupInfoTab[WDGM_ALIVE_NUM]; /*DD_2_002, DD_2_207*/
#endif                                                             /*WDGM_ALIVE_NUM > 0*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
/*Save the status of the deadline supervision*/
/*DD_2_020, DD_2_028*/
static WdgM_DeadlineSupInfoType WdgM_DeadlineSupInfoTab[WDGM_DEADLINE_SUP_NUM];
#endif /* WDGM_DEADLINE_SUP_NUM == STD_ON */

#if (WDGM_INTLOG_SUP_NUM > 0)
/*Save the status of the internal logical supervision*/
/*DD_2_021, DD_2_023, DD_2_029*/
static WdgM_IntLogSupInfoType WdgM_IntLogSupInfoTab[WDGM_INTLOG_SUP_NUM];
#endif /*WDGM_INTLOG_SUP_NUM > 0*/

#if (WDGM_EXTLOG_SUP_NUM > 0)
/*Save the status of the external logical supervision*/
/*DD_2_021, DD_2_023, DD_2_026*/
static WdgM_ExtLogSupInfoType WdgM_ExtLogSupInfoRelated[WDGM_EXTLOG_SUP_NUM];
#endif /*WDGM_EXTLOG_SUP_NUM > 0*/

#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

/***********************************************************************/

#define WDGM_START_SEC_VAR_INIT_UNSPECIFIED
#include "WdgM_MemMap.h"
/*Point to the WdgM_ModeCfgType from configuration*/
static const WdgM_ModeCfgType* WdgM_CurModeCfg = NULL_PTR;

/*Point to the WdgM_ConfigType from configuration*/
static const WdgM_ConfigType* WdgM_Config = NULL_PTR;

/*Save the status of the WdgM Mode*/
static WdgM_ModeInfoType WdgM_ModeInfo = {WDGM_INIT_NOT, 0, 0}; /*DD_2_204*/

/*Save the status of the WdgM Global information*/
/*DD_2_035, DD_2_036, DD_2_210*/
static WdgM_GlobalInfoType WdgM_GlobalInfo = {E_NOT_OK, WDGM_GLOBAL_STATUS_DEACTIVATED};

#define WDGM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "WdgM_MemMap.h"
/***********************************************************************/

/***************************Internal  API********************************************/
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
#if (WDGM_SE_NUM > 0)
static FUNC(void, WDGM_CODE) WdgM_InitSEStatus(const WdgM_ModeCfgType* cerModeCfgPtr);
static FUNC(void, WDGM_CODE)
    WdgM_ChangeSEStatus(const WdgM_ModeCfgType* oldModeCfg, const WdgM_ModeCfgType* newModeCfg);
static FUNC(void, WDGM_CODE)
    WdgM_SEDeactivateInternal(const WdgM_ModeCfgType* cerModeCfg, uint16 SEIdx, uint16 SEStatusTabIdx);
static FUNC(void, WDGM_CODE) WdgM_ConfirmSELocalStatus(const WdgM_SECfgType* cerSEInfo);
static FUNC(void, WDGM_CODE) WdgM_DeinitSEStatus(const WdgM_ModeCfgType* cerModeCfgPtr);
static FUNC(void, WDGM_CODE) WdgM_ConfirmGlobalStatus(void);
static FUNC(Std_ReturnType, WDGM_CODE)
    WdgM_GetModeSEIndex(WdgM_SupervisedEntityIdType SEID, uint16* modeSEIdx, const WdgM_ModeCfgType* cerModeCfgPtr);
static FUNC(Std_ReturnType, WDGM_CODE)
    WdgM_GetSECheckpointIndex(WdgM_CheckpointIdType CPID, const WdgM_SECfgType* cerSECfg, uint16* CPTabIdx);
static FUNC(void, WDGM_CODE) WdgM_InitAllTabStatus(void);
#if (STD_ON == WDGM_IMMEDIATE_RESET || STD_ON == WDGM_E_SUPERVISION_ENABLE)
static FUNC(void, WDGM_CODE) WdgM_StopStatusErrorHanding(void);
#endif
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
static FUNC(void, WDGM_CODE) WdgM_RestartOSApplication(const WdgM_SECfgType* cerSEInfo);
#endif
static FUNC(void, WDGM_CODE) WdgM_SetTrigger(void);
static FUNC(uint16, WDGM_CODE) WdgM_GetSupervisedEntityTabIndex(WdgM_SupervisedEntityIdType cerSEID);
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_ModeExistenceConfirmation(WdgM_ModeType modeId);
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_InitModeConfirmation(const WdgM_ConfigType* ConfigPtr);
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_OffModeConfirmation(const WdgM_ConfigType* ConfigPtr);
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_OffModeConfirmationForRequestMode(WdgM_ModeType modeId);
#endif
#endif /*WDGM_SE_NUM > 0*/

#if (WDGM_ALIVE_NUM > 0)
static FUNC(void, WDGM_CODE) WdgM_UpdateAICounter(uint16 CPIdx, const WdgM_SECfgType* cerSECfg);
static FUNC(void, WDGM_CODE) WdgM_ConfirmAliveSupResultEachSE(const WdgM_SECfgType* cerSEInfo);
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_ConfirmAliveSupResult(uint16 supIdx, uint16 refCycle);
#endif /*WDGM_ALIVE_NUM > 0*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
static FUNC(void, WDGM_CODE)
    WdgM_UpdateDeadlineSup(uint16 CPIdx, const WdgM_SECfgType* cerSECfg, WdgM_DeadlineTimeType curTime);
static FUNC(void, WDGM_CODE)
    WdgM_UpdateDeadlineSupResult(WdgM_DeadlineTimeType timeGap, const WdgM_DeadlineSupCfgType* deadlineSupCfg);
static FUNC(void, WDGM_CODE) WdgM_PerformDeadlineTimeoutDetection(const WdgM_SECfgType* cerSEInfo);
static FUNC(void, WDGM_CODE) WdgM_ConfirmDeadlineSupResultEachSE(const WdgM_SECfgType* cerSEInfo);
#endif /*WDGM_DEADLINE_SUP_NUM > 0*/

#if (WDGM_INTLOG_SUP_NUM > 0)
static FUNC(void, WDGM_CODE) WdgM_UpdateIntLogSup(uint16 CPIdx, const WdgM_SECfgType* cerSECfg);
static FUNC(void, WDGM_CODE) WdgM_ConfirmIntLogSupResultEachSE(const WdgM_SECfgType* cerSEInfo);
#endif /*WDGM_INTLOG_SUP_NUM > 0*/

#if (WDGM_EXTLOG_SUP_NUM > 0)
static FUNC(void, WDGM_CODE)
    WdgM_UpdateExtLogSup(uint16 CPIdx, const WdgM_SECfgType* cerSECfg, const WdgM_ModeCfgType* cerModeCfg);
static FUNC(void, WDGM_CODE) WdgM_ConfirmExtLogSupResultEachSE(const WdgM_ModeCfgType* cerModeCfgPtr);
static FUNC(void, WDGM_CODE) WdgM_ClearExtlogStatus(const WdgM_ModeCfgType* oldModeCfg);
#endif /*WDGM_EXTLOG_SUP_NUM > 0*/

#if ((WDGM_ALIVE_NUM > 0) || (WDGM_DEADLINE_SUP_NUM > 0))
static FUNC(void, WDGM_CODE)
    WdgM_ClearNoUseAliveDeadlineStatus(const WdgM_SECfgType* oldSE, const WdgM_SECfgType* NewSE);
#endif /*WDGM_ALIVE_NUM > 0 || WDGM_DEADLINE_SUP_NUM > 0*/

static FUNC(uint8, WDGM_CODE) WdgM_GetWordBitState(uint16 wordNum, uint8 getBit);

/* PRQA S 3449, 3451 ++ */ /* VL_WdgM_3449, VL_WdgM_3451 */
#if (WDGM_BSWM_ENABLED == STD_ON)
extern FUNC(void, BSWM_WDGM_CODE) BswM_WdgM_RequestPartitionReset(ApplicationType Application);
#endif
/* PRQA S 3449, 3451 -- */

/***************************************************************/
/*
 * Brief               Initializes the Watchdog Manager.
 * ServiceId           0x00
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      WdgMConfigPtr
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_061,DD_2_088,DD_2_089,DD_2_144~DD_2_149
 */
/**************************************************************/
FUNC(void, WDGM_CODE) WdgM_Init(const WdgM_ConfigType* ConfigPtr)
{
    uint8          devIdx; /*Rule-2.2     This initialization is redundant.*/
    Std_ReturnType retVal = E_OK;
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
    uint8 partIdx = 0u;
#endif

#if (STD_ON == WDGM_DEV_ERROR_DETECT)
    if (NULL_PTR == ConfigPtr)
    {
        /*req WDGM-FUNR-091[WDGM048]*/
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_ID, WDGM_E_INV_POINTER);
    }
    else if ((Std_ReturnType)E_NOT_OK == WdgM_InitModeConfirmation(ConfigPtr))
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_ID, WDGM_E_PARAM_CONFIG); /*req WdgM 010*/
    }
#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
    else if ((Std_ReturnType)E_NOT_OK == WdgM_OffModeConfirmation(ConfigPtr))
    {
        /*req WdgM 030*/
        ((void)Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_ID, WDGM_E_DISABLE_NOT_ALLOWED));
    }
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/
    else
#endif /*STD_OFF == WDGM_DEV_ERROR_DETECT*/
    {
        WdgM_Config = ConfigPtr;

        /* PRQA S 0303 ++*/ /* VL_WdgM_0303 */
        *(volatile uint16*)WDGM_FIRST_EXPIRED_SEID         = 0;
        *(volatile uint16*)WDGM_FIRST_EXPIRED_INVERSE_SEID = 0;
        /* PRQA S 0303 --*/ /* VL_WdgM_0303 */

        WdgM_GlobalInfo.WdgMFirstExpiredSEIDSaved = E_NOT_OK;

        WdgM_InitAllTabStatus();

        WdgM_CurModeCfg = (const WdgM_ModeCfgType*)&ConfigPtr->WdgMModeRef[ConfigPtr->WdgMInitialModeId];

        for (devIdx = 0U; devIdx < WdgM_Config->WdgMModeRef[WdgM_CurModeCfg->WdgMModeID].WdgMModeTriggerCnt; devIdx++)
        {
            retVal =
                WdgIf_SetMode(devIdx, (WdgIf_ModeType)(WdgM_CurModeCfg->WdgMTriggerRelated[devIdx].WdgMWatchdogMode));
            if ((Std_ReturnType)E_NOT_OK == retVal)
            {
                WdgM_DeinitSEStatus(WdgM_CurModeCfg);

                WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_STOPPED;

                WdgM_ModeInfo.WdgMModeInitStatus = WDGM_INIT_NOT;

                break;
            }
        }
        if ((Std_ReturnType)E_OK == retVal)
        {
            WdgM_InitSEStatus(WdgM_CurModeCfg);

            WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_OK;

            WdgM_ModeInfo.WdgMModeCurrentID = WdgM_CurModeCfg->WdgMModeID;

            WdgM_ModeInfo.WdgMModeInitStatus = WDGM_INIT_OK;

            WdgM_ModeInfo.WdgMExpiredSupTolCounter = 0u;
        }
    }
}

/*************************************************************************/
/*
 * Brief               De-initializes the Watchdog Manager.
 * ServiceId           0x01
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_062,DD_2_150,DD_2_151
 */
/*************************************************************************/
FUNC(void, WDGM_CODE)
WdgM_DeInit(void)
{
    if ((WDGM_INIT_NOT == WdgM_ModeInfo.WdgMModeInitStatus)
        || (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalInfo.WdgMGlobalResult))
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_DEINIT_ID, WDGM_E_UNINIT); /*req WdgM 288*/
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        /*req WdgM 388*/
    }
    else
    {
        if ((Std_ReturnType)E_OK == WdgM_SetMode((WdgM_ModeType)WDGM_SLEEP_MODE_ID))
        {
            /*req WdgM 286 : Fig 4-14*/
            WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_DEACTIVATED;
            WdgM_ModeInfo.WdgMModeInitStatus = WDGM_INIT_NOT;
        }
        else
        {
            /*MISRA-C*/
        }
    }
}

/*************************************************************************/
/*
 * Brief               Gets the version of Watchdog Manager.
 * ServiceId           0x02
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_250,DD_2_128,DD_2_129
 */
/*************************************************************************/
#if (STD_ON == WDGM_VERSION_INFO_API)
/*DD_2_250,DD_2_128,DD_2_129*/
FUNC(void, WDGM_CODE)
WdgM_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        /*req WDGM-FUNR-091[WDGM048]*/
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_VERSION_INFO_ID, WDGM_E_INV_POINTER);
    }
    else
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
    {
        /*req WDGM-APIR-008[WDGM110]*/
        VersionInfo->vendorID = WDGM_VENDOR_ID;
        VersionInfo->moduleID = WDGM_MODULE_ID;

        VersionInfo->sw_major_version = WDGM_C_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = WDGM_C_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = WDGM_C_SW_PATCH_VERSION;
    }
}
#endif /*STD_ON == WDGM_VERSION_INFO_API*/

/*************************************************************************/
/*
 * Brief               Sets the current mode of Watchdog Manager.
 * ServiceId           0x03
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Mode: One of the configured Watchdog Manager modes.
 * Param-Name[in/out]  None
 * Return              E_OK: Successfully changed to the new mode
 *                     E_NOT_OK: Changing to the new mode failed
 * CallByAPI           <>
 * REQ ID              DD_2_046,DD_2_074,DD_2_077,DD_2_078,DD_2_121,DD_2_122,DD_2_123
 * REQ ID              DD_2_124,DD_2_125,DD_2_126
 */
/*************************************************************************/
/* PRQA S 1505 ++*/ /*VL_WdgM_1505*/
FUNC(Std_ReturnType, WDGM_CODE)
WdgM_SetMode(WdgM_ModeType Mode)
{
    Std_ReturnType retValue = E_NOT_OK;
    Std_ReturnType ret      = E_NOT_OK;
    uint8          triggerIdx;
    /*device index*/
    WdgM_WatchdogDeviceType deviceId;
    /*wdg mode*/
    WdgIf_ModeType WdgIfMode;

    if ((WDGM_INIT_NOT == WdgM_ModeInfo.WdgMModeInitStatus)
        || (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalInfo.WdgMGlobalResult))
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_UNINIT); /*req WdgM 021*/
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
    else if ((Std_ReturnType)E_NOT_OK == WdgM_ModeExistenceConfirmation(Mode))
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_PARAM_MODE); /*req WdgM 020*/
        retValue = E_NOT_OK;
    }
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
    else if ((Std_ReturnType)E_NOT_OK == WdgM_OffModeConfirmationForRequestMode(Mode))
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_SET_MODE_ID,
            WDGM_E_DISABLE_NOT_ALLOWED); /*req WdgM 031*/
#endif                                   /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/
    else if (Mode == WdgM_ModeInfo.WdgMModeCurrentID)
    {
        retValue = E_OK;
    }
    else
    {
        if ((WDGM_GLOBAL_STATUS_OK == WdgM_GlobalInfo.WdgMGlobalResult)
            || (WDGM_GLOBAL_STATUS_FAILED == WdgM_GlobalInfo.WdgMGlobalResult))
        {
            for (triggerIdx = 0; triggerIdx < WdgM_Config->WdgMModeRef[Mode].WdgMModeTriggerCnt; triggerIdx++)
            {
                deviceId  = WdgM_Config->WdgMModeRef[Mode].WdgMTriggerRelated[triggerIdx].WdgM_WatchdogDevice;
                WdgIfMode = WdgM_Config->WdgMModeRef[Mode].WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode;
                ret       = WdgIf_SetMode(deviceId, WdgIfMode); /*req WdgM 186*/
                if ((Std_ReturnType)E_NOT_OK == ret)
                {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
                    /*req WdgM 142*/
                    ((void)Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_SET_MODE));
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
                    /*req WdgM 139*/
                    WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_STOPPED;
                    retValue                         = E_NOT_OK;
                    break;
                }
            }
            if ((Std_ReturnType)E_OK == ret)
            {
                WdgM_ChangeSEStatus(WdgM_CurModeCfg, (const WdgM_ModeCfgType*)&WdgM_Config->WdgMModeRef[Mode]);
                WdgM_ModeInfo.WdgMModeCurrentID = Mode;
                WdgM_CurModeCfg                 = (const WdgM_ModeCfgType*)&WdgM_Config->WdgMModeRef[Mode];
                retValue                        = E_OK;
            }
        }
        else
        {
            retValue = E_NOT_OK; /*req WdgM 316*/
        }
    }
    return retValue;
}
/* PRQA S 1505 --*/ /*VL_WdgM_1505*/

/*************************************************************************/
/*
 * Brief               Returns the current mode of the Watchdog Manager
 * ServiceId           0x0b
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Mode: One of the configured Watchdog Manager modes.
 * Param-Name[in/out]  None
 * Return              E_OK: Success
 *                     E_NOT_OK: failure
 * CallByAPI           <>
 * REQ ID              DD_2_080,DD_2_130,DD_2_131,DD_2_132
 */
/*************************************************************************/
FUNC(Std_ReturnType, WDGM_CODE)
WdgM_GetMode(WdgM_ModeType* Mode)
{
    Std_ReturnType retValue = E_NOT_OK;
    if ((WDGM_INIT_NOT == WdgM_ModeInfo.WdgMModeInitStatus)
        || (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalInfo.WdgMGlobalResult))
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_MODE_ID, WDGM_E_UNINIT); /*req WdgM 253*/
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
    else if (NULL_PTR == Mode)
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_MODE_ID, WDGM_E_INV_POINTER); /*req WdgM 254*/
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
    else
    {
        *Mode    = WdgM_ModeInfo.WdgMModeCurrentID;
        retValue = E_OK;
    }
    return retValue;
}

/*************************************************************************/
/*
 * Brief                Indicates to the Watchdog Manager that a Checkpoint
 *                      within a Supervised Entity has been reached
 * ServiceId            0x0e
 * Sync/Async           synchronous
 * Reentrancy           Reentrant
 * Param-Name[in]       SEID: Identifier of the Supervised Entity that reports a Checkpoint
 * Param-Name[in]       CheckpointID: Identifier of the Checkpoint within a Supervised Entity
 *                      that has been reached
 * Param-Name[in/out]   None
 * Return               E_OK: Successfully updated alive counter
 *                      E_NOT_OK: updated failed
 * CallByAPI            <>
 * REQ ID               DD_2_058,DD_2_113,DD_2_117,DD_2_118,DD_2_119
 */
/*************************************************************************/
FUNC(Std_ReturnType, WDGM_CODE)
WdgM_CheckpointReached(WdgM_SupervisedEntityIdType SEID, WdgM_CheckpointIdType CheckpointID)
{
    uint16                SECPIdx   = 0;
    uint16                modeSEIdx = 0;
    uint16                SEStatusTabIdx;
    Std_ReturnType        retValue = E_NOT_OK;
    const WdgM_SECfgType* curSECfg; /*Point to the WdgM_SECfgType from configuration*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
    WdgM_DeadlineTimeType curTime;
#endif

#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
    ApplicationType partIdx = GetApplicationID();
#endif /* WDGM_MULTI_PARTITION_ENABLED == STD_ON*/
    SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(SEID);

#if (STD_ON == WDGM_DEV_ERROR_DETECT)
    /*req WdgM 319 282 realized by the ORIENTAIS*/
    if ((WDGM_INIT_NOT == WdgM_ModeInfo.WdgMModeInitStatus)
        || (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalInfo.WdgMGlobalResult))
    {
        (void)Det_ReportError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_CHECKPOINT_REACHED_ID,
            WDGM_E_UNINIT); /*req WdgM 279*/
        retValue = E_NOT_OK;
    }
    else if (WDGM_SE_NUM <= SEStatusTabIdx)
    {
        (void)Det_ReportError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_CHECKPOINT_REACHED_ID,
            WDGM_E_PARAM_SEID); /*req WdgM 278*/
        retValue = E_NOT_OK;
    }
    else if ((Std_ReturnType)E_NOT_OK == WdgM_GetModeSEIndex(SEID, &modeSEIdx, WdgM_CurModeCfg))
    {
        /*req WdgM 319*/
        ((void)Det_ReportRuntimeError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_CHECKPOINT_REACHED_ID,
            WDGM_E_SEDEACTIVATED));
        retValue = E_NOT_OK;
    }
    else if ((CheckpointID + (WdgM_CheckpointIdType)1) > WdgM_CheckpointQuantityInSE[modeSEIdx])
    {
        (void)
            Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_ID, WDGM_E_CPID); /*req WdgM 284*/
        retValue = E_NOT_OK;
    }
#else
    if (((Std_ReturnType)E_NOT_OK == WdgM_GetModeSEIndex(SEID, &modeSEIdx, WdgM_CurModeCfg))
        || (WDGM_LOCAL_STATUS_DEACTIVATED == WdgM_SEInfoRelated[modeSEIdx].WdgMLocalStatus)
        || (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalInfo.WdgMGlobalResult))
    {
        retValue = E_NOT_OK;
    }
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
    else
    {
        curSECfg = (const WdgM_SECfgType*)&WdgM_CurModeCfg->WdgMSERelated[modeSEIdx];
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
        if (partIdx == curSECfg->WdgMEcucPartitionRef)
#endif
        {
#if (WDGM_DEADLINE_SUP_NUM > 0)
            /*PRQA S 4342 ++*/ /* VL_WdgM_4342*/
            (void)GetCounterValue((CounterType)curSECfg->WdgMOsCounter, &curTime);
            /*PRQA S 4342 --*/ /* VL_WdgM_4342*/
            curTime = curTime * curSECfg->WdgMOsCounterResolution;
#endif
            /*The Checkpoint index in the SE*/
            retValue = WdgM_GetSECheckpointIndex(CheckpointID, curSECfg, &SECPIdx);
            if ((Std_ReturnType)E_OK == retValue)
            {
                SchM_Enter_WdgM_Exclusive();
#if (WDGM_ALIVE_NUM > 0)
                /*Confirm whether has AliveSup,then update AI Cnt*/
                WdgM_UpdateAICounter(SECPIdx, curSECfg);
#endif /*WDGM_ALIVE_NUM > 0*/

                if ((WDGM_GLOBAL_STATUS_OK == WdgM_GlobalInfo.WdgMGlobalResult)
                    || (WDGM_GLOBAL_STATUS_FAILED == WdgM_GlobalInfo.WdgMGlobalResult))
                {
#if (WDGM_EXTLOG_SUP_NUM > 0)
                    WdgM_UpdateExtLogSup(SECPIdx, curSECfg, WdgM_CurModeCfg);
#endif /*WDGM_EXTLOG_SUP_NUM > 0*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
                    WdgM_UpdateDeadlineSup(SECPIdx, curSECfg, curTime);
#endif /*WDGM_DEADLINE_SUP_NUM > 0*/

#if (WDGM_INTLOG_SUP_NUM > 0)
                    WdgM_UpdateIntLogSup(SECPIdx, curSECfg);
#endif /*WDGM_INTLOG_SUP_NUM > 0*/
                }
                SchM_Exit_WdgM_Exclusive();
            }
            else
            {
                /*MISRAC*/
            }
        }
    }
    return retValue;
}

/*************************************************************************/
/*
 * Brief                Returns the supervision status of an individual Supervised Entity
 * ServiceId            0x0C
 * Sync/Async           synchronous
 * Reentrancy           Reentrant
 * Param-Name[in]       SEId: Identifier of the supervised entity
 *                      whose supervision status shall be returned.
 * Param-Name[out]      Status :Supervision status of the given supervised entity.
 * Param-Name[in/out]   None
 * Return               E_OK: Current supervision status successfully returned
 *                      E_NOT_OK: Returning current supervision status failed
 * CallByAPI            <>
 * REQ ID               DD_2_081,DD_2_137,DD_2_138,DD_2_139,DD_2_140
 */
/*************************************************************************/
Std_ReturnType WdgM_GetLocalStatus(WdgM_SupervisedEntityIdType SEID, WdgM_LocalStatusType* Status)
{
    Std_ReturnType retValue = E_NOT_OK;
    uint16         SEStatusTabIdx;
    /*The SEID index in the Mode is the same as in the WdgM_ModeInfo*/
    uint16 modeSETabIdx = 0;

    SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(SEID);

    if ((WDGM_INIT_NOT == WdgM_ModeInfo.WdgMModeInitStatus)
        || (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalInfo.WdgMGlobalResult))
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)
            Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_ID, WDGM_E_UNINIT); /*req WdgM 173*/
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
    else if (WDGM_SE_NUM <= SEStatusTabIdx)
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_GET_LOCAL_STATUS_ID,
            WDGM_E_PARAM_SEID); /*req WdgM 172*/
#endif                          /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
    else if (NULL_PTR == Status)
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_GET_LOCAL_STATUS_ID,
            WDGM_E_INV_POINTER); /*req WdgM 257*/
#endif                           /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
    else if ((Std_ReturnType)E_OK == WdgM_GetModeSEIndex(SEID, &modeSETabIdx, WdgM_CurModeCfg))
    {
        *Status  = WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus;
        retValue = E_OK;
    }
    else
    {
        *Status  = WDGM_LOCAL_STATUS_DEACTIVATED;
        retValue = E_OK;
    }

    return retValue;
}

/*************************************************************************/
/*
 * Brief               Returns the global supervision status of the Watchdog Manager
 * ServiceId           0x0d
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Status :Global supervision status of the Watchdog Manager
 * Param-Name[in/out]  None
 * Return              E_OK: Current supervision status successfully returned
 *                     E_NOT_OK: Returning current supervision status failed
 * CallByAPI           <>
 * REQ ID              DD_2_082,DD_2_133,DD_2_134,DD_2_136
 */
/*************************************************************************/
Std_ReturnType WdgM_GetGlobalStatus(WdgM_GlobalStatusType* Status)
{
    Std_ReturnType retValue = E_NOT_OK;

    if ((WDGM_INIT_NOT == WdgM_ModeInfo.WdgMModeInitStatus)
        || (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalInfo.WdgMGlobalResult))
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_GET_GLOBAL_STATUS_ID,
            WDGM_E_UNINIT); /*req WdgM 176*/
#endif                      /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
    else if (NULL_PTR == Status)
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_GET_GLOBAL_STATUS_ID,
            WDGM_E_INV_POINTER); /*req WdgM 334*/
#endif                           /*STD_ON == WDGM_DEV_ERROR_DETECT*/
        retValue = E_NOT_OK;
    }
    else
    {
        *Status  = WdgM_GlobalInfo.WdgMGlobalResult;
        retValue = E_OK;
    }
    return retValue;
}

/*************************************************************************/
/*
 * Brief               Instructs the Watchdog Manager to cause a watchdog reset.
 * ServiceId           0x0F
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_254,DD_2_255,DD_2_256,DD_2_257
 */
/*************************************************************************/
FUNC(void, WDGM_CODE)
WdgM_PerformReset(void)
{
    uint8 triggerIdx;
    if ((WDGM_INIT_NOT == WdgM_ModeInfo.WdgMModeInitStatus)
        || (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalInfo.WdgMGlobalResult))
    {
#if (STD_ON == WDGM_DEV_ERROR_DETECT)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_PERFORM_RESET_ID, WDGM_E_UNINIT); /*req WdgM 270*/
                                                                                                       /*req WdgM 401*/
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
    }
    else
    {
        /*req WdgM 223-1*/
        for (triggerIdx = 0; triggerIdx < WdgM_CurModeCfg->WdgMModeTriggerCnt; triggerIdx++)
        {
            if (WDGIF_OFF_MODE != WdgM_CurModeCfg->WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode)
            {
                /*req WdgM 232 : Trigger value is 0*/
                WdgIf_SetTriggerCondition(
                    WdgM_CurModeCfg->WdgMTriggerRelated[triggerIdx].WdgM_WatchdogDevice,
                    WDGM_TRIGGER_CONDITION_STOP);
            }
        }
        SchM_Exit_WdgM_Exclusive();
        while (1)
        {
            /*Nothing to do*/
            /*req WdgM 233 : Never update the trigger after WdgM_PerformReset*/
        }
    }
}

/*************************************************************************/
/*
 * Brief               Returns SEID that first reached the state WDGM_LOCAL_STATUS_EXPIRED
 * ServiceId           0x10
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     SEID : Identifier of the supervised entity
 *                     that first reached the state WDGM_LOCAL_STATUS_EXPIRED
 * Param-Name[in/out]  None
 * Return              E_OK: Success
 *                     E_NOT_OK: failure
 * CallByAPI           <>
 * REQ ID              DD_2_083,DD_2_141,DD_2_142,DD_2_143
 */
/*************************************************************************/
FUNC(Std_ReturnType, WDGM_CODE)
WdgM_GetFirstExpiredSEID(WdgM_SupervisedEntityIdType* SEID)
{
    Std_ReturnType retValue;

    /* PRQA S 0303 ++*/ /* VL_WdgM_0303 */
    uint16 FirstExpiredSEID        = *(volatile uint16*)WDGM_FIRST_EXPIRED_SEID;
    uint16 FirstExpiredInverseSEID = *(volatile uint16*)WDGM_FIRST_EXPIRED_INVERSE_SEID;
    /* PRQA S 0303 --*/ /* VL_WdgM_0303 */

#if (STD_ON == WDGM_DEV_ERROR_DETECT)
    if (NULL_PTR == SEID)
    {
        (void)Det_ReportError(
            WDGM_MODULE_ID,
            WDGM_INSTANCE_ID,
            WDGM_GET_FIRST_EXPIRED_SEID_ID,
            WDGM_E_INV_POINTER); /*req WdgM 347*/
        retValue = E_NOT_OK;
    }
    else
#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/
    {
        /*req WdgM 349*/
        if ((uint16)0xFFFF == (FirstExpiredSEID + FirstExpiredInverseSEID))
        {
            *SEID    = FirstExpiredSEID;
            retValue = E_OK;
        }
        else
        {
            /* PRQA S 0303 ++*/ /* VL_WdgM_0303 */
            *(volatile uint16*)WDGM_FIRST_EXPIRED_SEID         = 0;
            *(volatile uint16*)WDGM_FIRST_EXPIRED_INVERSE_SEID = 0;
            /* PRQA S 0303 --*/ /* VL_WdgM_0303 */
            *SEID    = 0;
            retValue = E_NOT_OK;
        }
    }
    return retValue;
}

#if (WDGM_ALIVE_NUM > 0)
/***************************************************************/
/*
 * Brief               Update the alive supervision counter
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CPIdx : the array index of the CP in the configuration of the certain SE
 *                     cerSECfg: the configuration of the certain SE
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_114,DD_2_170
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_UpdateAICounter(uint16 CPIdx, const WdgM_SECfgType* cerSECfg)
{
    if ((uint8)STD_ON
        == WdgM_GetWordBitState(cerSECfg->WdgMCheckpointRelated[CPIdx].WdgMSupSupportGroup, (uint8)WDGM_ALIVE_SUPPORT))
    {
        WdgM_AliveSupInfoTab[cerSECfg->WdgMCheckpointRelated[CPIdx].WdgMAliveSupID].WdgMAliveIndicationCounter++;
    }
}

/***************************************************************/
/*
 * Brief               Confirm the results of alive supervision  in the SE.
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerSEInfo: the configuration of the certain SE
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_003, DD_2_004, DD_2_005, DD_2_006, DD_2_007,DD_2_104,DD_2_157
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_ConfirmAliveSupResultEachSE(const WdgM_SECfgType* cerSEInfo)
{
    uint16 tabIdx;
    uint16 idx;
    uint16 aliveRefCycle;
    uint16 SEStatusTabIdx;
    sint32 aliveSupTempCnt;
    sint32 aliveSupTempResult;
    sint32 aliveSupTempCfgCnt;
    if ((uint16)0 < cerSEInfo->WdgMSEAliveCnt)
    {
        /*update certain AliveSup result*/
        for (idx = 0; idx < cerSEInfo->WdgMSEAliveCnt; idx++)
        {
            aliveRefCycle = cerSEInfo->WdgMAliveSupCfg[idx].WdgMSupRefCycle;
            tabIdx        = cerSEInfo->WdgMAliveSupCfg[idx].WdgMAliveSupID;
            /*req WdgM 098 : Reach to the refCycle, determine the related Alive Sup result*/
            if ((Std_ReturnType)E_OK == WdgM_ConfirmAliveSupResult(tabIdx, aliveRefCycle))
            {
                /*f(EAI,SRC)= SRC - EAI */
                aliveSupTempCfgCnt = (sint32)(cerSEInfo->WdgMAliveSupCfg[idx].WdgMSupRefCycle)
                                     - (sint32)(cerSEInfo->WdgMAliveSupCfg[idx].WdgMExpectedAliveSupIndication);
                /*n(AI)-n(SC)*/
                aliveSupTempCnt = (sint32)(WdgM_AliveSupInfoTab[tabIdx].WdgMAliveIndicationCounter)
                                  - (sint32)(WdgM_AliveSupInfoTab[tabIdx].WdgMAliveCycleCounter);
                aliveSupTempResult = aliveSupTempCfgCnt + aliveSupTempCnt;

                WdgM_AliveSupInfoTab[tabIdx].WdgMAliveCycleCounter      = 0;
                WdgM_AliveSupInfoTab[tabIdx].WdgMAliveIndicationCounter = 0;

                if (((aliveSupTempResult + (sint32)(cerSEInfo->WdgMAliveSupCfg[idx].WdgMMinMargin)) >= 0)
                    && (aliveSupTempResult <= (sint32)(cerSEInfo->WdgMAliveSupCfg[idx].WdgMMaxMargin)))
                {
                    WdgM_AliveSupInfoTab[tabIdx].WdgMAliveSupResult = WDGM_CORRECT;
                }
                else
                {
                    WdgM_AliveSupInfoTab[tabIdx].WdgMAliveSupResult = WDGM_INCORRECT;
                }
            }
        }

        SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(cerSEInfo->WdgMSEId);

        WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_ALIVE_SUP] = WDGM_CORRECT;

        /*update the AliveSup result of Certain SE including several AliveSup */
        for (idx = 0; idx < cerSEInfo->WdgMSEAliveCnt; idx++)
        {
            tabIdx = cerSEInfo->WdgMAliveSupCfg[idx].WdgMAliveSupID;
            if (WDGM_INCORRECT == WdgM_AliveSupInfoTab[tabIdx].WdgMAliveSupResult)
            {
                WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_ALIVE_SUP] = WDGM_INCORRECT;
                break;
            }
        }
    }
}

/***************************************************************/
/*
 * Brief               confirm the result of alive supervision
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      supIdx: the array index of the WdgM_AliveSupInfoTab
 *                     refCycle: The threshold cycle defined in the configuration
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Success
 *                     E_NOT_OK: failure
 * CallByAPI           <>
 */
/**************************************************************/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_ConfirmAliveSupResult(uint16 supIdx, uint16 refCycle)
{
    Std_ReturnType retValue;

    WdgM_AliveSupInfoTab[supIdx].WdgMAliveCycleCounter++;
    if (WdgM_AliveSupInfoTab[supIdx].WdgMAliveCycleCounter >= refCycle)
    {
        /*The AliveSup Result base on the attachment*/
        retValue = E_OK;
    }
    else
    {
        retValue = E_NOT_OK;
    }
    return retValue;
}

#endif /*WDGM_ALIVE_NUM > 0*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
/***************************************************************/
/*
 * Brief               Update the status of the deadline supervision
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CPIdx: the array index of the CP in the configuration of the certain SE
 *                     cerSECfg: the configuration of the certain SE
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_009, DD_2_010, DD_2_012, DD_2_013, DD_2_014, DD_2_015, DD_2_016,
 *                     DD_2_019,DD_2_115,DD_2_171
 */
/**************************************************************/

static FUNC(void, WDGM_CODE)
    WdgM_UpdateDeadlineSup(uint16 CPIdx, const WdgM_SECfgType* cerSECfg, WdgM_DeadlineTimeType curTime)
{
    WdgM_DeadlineTimeType         DeadlineTimeGap = 0;
    uint16                        CPSupportDeadlineIndex;
    const WdgM_CheckpointCfgType* CertCheckpoint =
        (const WdgM_CheckpointCfgType*)&cerSECfg->WdgMCheckpointRelated[CPIdx];
    const WdgM_CPSupportDeadLineInfoType* CPSupportDeadlineInfo;
    const WdgM_DeadlineSupCfgType*        DeadlineSupCfgInfo;
    uint16                                DealineTab_Index;

    if ((uint8)STD_ON == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_DEADLINE_SUPPORT))
    {
        /******One Checkpoint may map several Deadline Sup********/
        for (CPSupportDeadlineIndex = 0; CPSupportDeadlineIndex < CertCheckpoint->WdgMCPSupportDeadLineCnt;
             CPSupportDeadlineIndex++)
        {
            CPSupportDeadlineInfo = (const WdgM_CPSupportDeadLineInfoType*)&CertCheckpoint
                                        ->WdgMCPSupportDeadLineInfo[CPSupportDeadlineIndex];
            if (WDGM_INCORRECT
                == WdgM_DeadlineSupInfoTab[CPSupportDeadlineInfo->WdgM_DeadlineSupID].WdgMDeadlineSupResult)
            {
                break;
            }
            else
            {
                /**Deadline Sup Start**/
                if ((uint8)STD_ON
                    == WdgM_GetWordBitState(
                        CPSupportDeadlineInfo->WdgMCPSupportDeadlineGroup,
                        (uint8)CP_DEADLINE_SUPPORT_START))
                {
                    WdgM_DeadlineSupInfoTab[CPSupportDeadlineInfo->WdgM_DeadlineSupID].WdgMTimeStampStart = curTime;
                }

                /**Deadline Sup Stop**/
                if ((uint8)STD_ON
                    == WdgM_GetWordBitState(
                        CPSupportDeadlineInfo->WdgMCPSupportDeadlineGroup,
                        (uint8)CP_DEADLINE_SUPPORT_STOP))
                {
                    if ((WdgM_DeadlineTimeType)0
                        != WdgM_DeadlineSupInfoTab[CPSupportDeadlineInfo->WdgM_DeadlineSupID].WdgMTimeStampStart)
                    {
                        /*req WdgM 229*/

                        for (DealineTab_Index = 0; DealineTab_Index < cerSECfg->WdgMSEDealineCnt; DealineTab_Index++)
                        {
                            if (cerSECfg->WdgMDeadlineSupCfg[DealineTab_Index].WdgMDeadlineSupID
                                == CPSupportDeadlineInfo->WdgM_DeadlineSupID)
                            {
                                break;
                            }
                        }
                        if (curTime
                            >= WdgM_DeadlineSupInfoTab[CPSupportDeadlineInfo->WdgM_DeadlineSupID].WdgMTimeStampStart)
                        {
                            DeadlineTimeGap =
                                curTime
                                - WdgM_DeadlineSupInfoTab[CPSupportDeadlineInfo->WdgM_DeadlineSupID].WdgMTimeStampStart;
                        }
                        else
                        {
                            DeadlineTimeGap = curTime
                                              + ((cerSECfg->WdgMOsCounterMaxValue * cerSECfg->WdgMOsCounterResolution)
                                                 - WdgM_DeadlineSupInfoTab[CPSupportDeadlineInfo->WdgM_DeadlineSupID]
                                                       .WdgMTimeStampStart);
                        }
                        /*req WdgM 294*/
                        DeadlineSupCfgInfo =
                            (const WdgM_DeadlineSupCfgType*)&cerSECfg->WdgMDeadlineSupCfg[DealineTab_Index];
                        WdgM_UpdateDeadlineSupResult(DeadlineTimeGap, DeadlineSupCfgInfo);
                        WdgM_DeadlineSupInfoTab[CPSupportDeadlineInfo->WdgM_DeadlineSupID].WdgMTimeStampStart = 0;
                    }
                    else
                    {
                        /*req WdgM 354: No action, if TimeStamp is 0*/
                    }
                }
            }
        }
    }
}

/***************************************************************/
/*
 * Brief               Update the result of the deadline supervision
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      timeGap: the time span from the start CP to the end CP.
 *                     deadlineSupCfg: the configuration of the certain deadline supervision
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_011, DD_2_017, DD_2_018,DD_2_159
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_UpdateDeadlineSupResult(
    WdgM_DeadlineTimeType          timeGap,
    const WdgM_DeadlineSupCfgType* deadlineSupCfg) /*req WdgM 294*/
{
    if ((((deadlineSupCfg->WdgMDeadlineMin) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
         <= (WdgM_DeadlineTimeFloatType)timeGap)
        && (((deadlineSupCfg->WdgMDeadlineMax) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
            >= (WdgM_DeadlineTimeFloatType)timeGap))
    {
        WdgM_DeadlineSupInfoTab[deadlineSupCfg->WdgMDeadlineSupID].WdgMDeadlineSupResult = WDGM_CORRECT;
    }
    else
    {
        WdgM_DeadlineSupInfoTab[deadlineSupCfg->WdgMDeadlineSupID].WdgMDeadlineSupResult = WDGM_INCORRECT;
    }
}

/***************************************************************/
/*
 * Brief               Perform timeout detection of deadline supervision in SE.
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]       cerSEInfo: the configuration of the certain supervision
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_158
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_PerformDeadlineTimeoutDetection(const WdgM_SECfgType* cerSEInfo)
{
    uint16                         idx;
    uint32                         TempStartTime;
    WdgM_DeadlineTimeType          DeadlineTimeGap = 0u;
    const WdgM_DeadlineSupCfgType* DeadlineSupCfgInfo;

    if (WdgM_GlobalInfo.WdgMGlobalResult != WDGM_GLOBAL_STATUS_STOPPED)
    {
        for (idx = 0u; idx < cerSEInfo->WdgMSEDealineCnt; idx++)
        {
            if ((boolean)TRUE == cerSEInfo->WdgMDeadlineSupCfg[idx].WdgMEnableTimeoutDetection)
            {
                DeadlineSupCfgInfo = (const WdgM_DeadlineSupCfgType*)&cerSEInfo->WdgMDeadlineSupCfg[idx];
                TempStartTime      = WdgM_DeadlineSupInfoTab[DeadlineSupCfgInfo->WdgMDeadlineSupID].WdgMTimeStampStart;
                if ((WdgM_DeadlineTimeType)0 != TempStartTime)
                {
                    /*PRQA S 4342,4442 ++*/ /* VL_WdgM_4342,VL_WdgM_4442*/
                    (void)GetElapsedValue(cerSEInfo->WdgMOsCounter, &TempStartTime, &DeadlineTimeGap);
                    /*PRQA S 4342,4442 --*/ /* VL_WdgM_4342,VL_WdgM_4442*/
                    DeadlineTimeGap = DeadlineTimeGap * cerSEInfo->WdgMOsCounterResolution;
                    if (((DeadlineSupCfgInfo->WdgMDeadlineMax) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
                        < ((WdgM_DeadlineTimeFloatType)DeadlineTimeGap))
                    {
                        WdgM_DeadlineSupInfoTab[DeadlineSupCfgInfo->WdgMDeadlineSupID].WdgMDeadlineSupResult =
                            WDGM_INCORRECT;
                        WdgM_DeadlineSupInfoTab[DeadlineSupCfgInfo->WdgMDeadlineSupID].WdgMTimeStampStart = 0;
                    }
                    else
                    {
                        /*MISRA C*/
                    }
                }
            }
        }
    }
}

/***************************************************************/
/*
 * Brief               Confirm the results of deadline supervision in SE.
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]       cerSEInfo: the configuration of the certain supervision
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_158
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_ConfirmDeadlineSupResultEachSE(const WdgM_SECfgType* cerSEInfo)
{
    uint16 tabIdx;
    uint16 idx;
    uint16 SEStatusTabIdx;
    /*If Alive Sup is false, may jump the determination of the Deadline Sup*/
    if ((uint16)0 < cerSEInfo->WdgMSEDealineCnt)
    {
        SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(cerSEInfo->WdgMSEId);
        WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_CORRECT;

        for (idx = 0; idx < cerSEInfo->WdgMSEDealineCnt; idx++)
        {
            tabIdx = cerSEInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupID;
            if (WDGM_INCORRECT == WdgM_DeadlineSupInfoTab[tabIdx].WdgMDeadlineSupResult)
            {
                /*one Deadline Sup incorrect*/
                WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_INCORRECT;
                break;
            }
        }
    }
}
#endif /*WDGM_DEADLINE_SUP_NUM > 0*/

#if (WDGM_INTLOG_SUP_NUM > 0)

/***************************************************************/
/*
 * Brief               Update the status of the internal logical supervision
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerSECfg: the configuration of the certain supervision
 *                     CPIdx: the array index of the CP in the configuration of the certain SE
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_022, DD_2_024, DD_2_026,DD_2_027, DD_2_028, DD_2_029,
 *                     DD_2_031, DD_2_032, DD_2_033,DD_2_116,DD_2_172
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_UpdateIntLogSup(uint16 CPIdx, const WdgM_SECfgType* cerSECfg)
{
    const WdgM_CheckpointCfgType* CertCheckpoint =
        (const WdgM_CheckpointCfgType*)&cerSECfg->WdgMCheckpointRelated[CPIdx];
    uint16 TransitionIndex;
    uint16 TransitionMax;

    if (((uint8)STD_ON == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_LOG_SUPPORT))
        && ((uint8)STD_ON
            == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_LOG_SUPPORT_INT_OR_EXT))
        && (WDGM_CORRECT == WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogSupResult)) /*Interlog Sup*/
    {
        if (WDGM_AF_FALSE == WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogActiveFlag)
        {
            /*req WdgM 274-1*/
            if ((uint8)STD_ON
                == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_LOG_SUP_SUPPORT_INIT))
            {
                WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLastReachedCPID =
                    CertCheckpoint->WdgMCheckpointID;
                WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogActiveFlag =
                    WDGM_AF_TRUE; /*req WdgM 273,332*/
                WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogSupResult =
                    WDGM_CORRECT; /*req WdgM 274-1*/
            }
            else
            {
                WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogSupResult =
                    WDGM_INCORRECT; /*req WdgM 274-1*/
            }
        }
        else
        {
            TransitionMax = cerSECfg->WdgMIntLogSupCfg[0].WdgMIntTransitionCnt;
            for (TransitionIndex = 0; TransitionIndex < TransitionMax; TransitionIndex++)
            {
                if (WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLastReachedCPID
                    == cerSECfg->WdgMIntLogSupCfg[0].WdgMIntTransition[TransitionIndex].WdgMTransitionSourCPID)
                {
                    if (CertCheckpoint->WdgMCheckpointID
                        == cerSECfg->WdgMIntLogSupCfg[0].WdgMIntTransition[TransitionIndex].WdgMTransitionDestCPID)
                    {
                        WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogSupResult = WDGM_CORRECT;
                        break;
                    }
                    else
                    {
                        WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogSupResult = WDGM_INCORRECT;
                    }
                }
            }
            if (TransitionIndex >= TransitionMax)
            {
                WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogSupResult = WDGM_INCORRECT;
            }
            else if (WDGM_CORRECT == WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogSupResult)
            {
                WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLastReachedCPID =
                    CertCheckpoint->WdgMCheckpointID;
                if ((uint8)STD_ON
                    == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_LOG_SUP_SUPPORT_FINAL))
                {
                    WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLogActiveFlag =
                        WDGM_AF_FALSE; /*req WdgM 331*/
                    WdgM_IntLogSupInfoTab[CertCheckpoint->WdgMLogSupID].WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
                }
            }
            else
            {
                /*MISRAC*/
            }
        }
    }
    else
    {
        /*req WdgM 297 : Nothing*/
    }
}

/***************************************************************/
/*
 * Brief               Confirm the results of internal logical supervision in SE
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerSECfg: the configuration of the certain supervision
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_160
 */
/**************************************************************/
/*req WdgM 324 : Determine the Alive Sup Result*/
static FUNC(void, WDGM_CODE) WdgM_ConfirmIntLogSupResultEachSE(const WdgM_SECfgType* cerSEInfo)
{
    uint16 tabIdx;
    uint16 SEStatusTabIdx;

    /*If Sup above is false, may jump the determination of the Internal log Sup*/
    if ((uint16)0 < cerSEInfo->WdgMSEIntLogCnt)
    {
        tabIdx = cerSEInfo->WdgMIntLogSupCfg[0].WdgMIntLogSupID;

        SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(cerSEInfo->WdgMSEId);

        WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_INTLOG_SUP] =
            WdgM_IntLogSupInfoTab[tabIdx].WdgMIntLogSupResult;
    }
}

#endif /*WDGM_INTLOG_SUP_NUM > 0*/

#if (WDGM_EXTLOG_SUP_NUM > 0)

/***************************************************************/
/*
 * Brief               Confirm the results of External logical supervision in Mode
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerSECfg: the configuration of the certain supervision
 *                     CPIdx: the array index of the CP in the configuration of the certain SE
 *                     cerModeCfg : the configuration of the certain Mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_022, DD_2_025, DD_2_026, DD_2_027, DD_2_028, DD_2_029,
 *                     DD_2_031, DD_2_032, DD_2_033,DD_2_116,DD_2_169
 */
/**************************************************************/
static FUNC(void, WDGM_CODE)
    WdgM_UpdateExtLogSup(uint16 CPIdx, const WdgM_SECfgType* cerSECfg, const WdgM_ModeCfgType* cerModeCfg)
{
    uint16                        TransitionIndex;
    uint16                        ExtlogSupModeConfigIndex;
    const WdgM_CheckpointCfgType* CertCheckpoint =
        (const WdgM_CheckpointCfgType*)&cerSECfg->WdgMCheckpointRelated[CPIdx];

    if (((uint8)STD_ON == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_LOG_SUPPORT))
        && ((uint8)STD_OFF
            == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_LOG_SUPPORT_INT_OR_EXT))
        && (WDGM_CORRECT == WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogSupResult))
    {
        /*req WdgM 252 - 1*/
        if (WDGM_AF_FALSE == WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogActiveFlag)
        {
            if ((uint8)STD_ON
                == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_LOG_SUP_SUPPORT_INIT))
            {
                WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLastReachedCPID =
                    CertCheckpoint->WdgMCheckpointID;
                WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLastReachedSEID = cerSECfg->WdgMSEId;
                WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogActiveFlag   = WDGM_AF_TRUE;
                WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogSupResult =
                    WDGM_CORRECT; /*req WdgM 252-1*/
            }
            else
            {
                WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogSupResult =
                    WDGM_INCORRECT; /*req WdgM 252-1*/
            }
        }
        else
        {
            /****Find the ExtLog Sup index in the Mode Config *****/
            for (ExtlogSupModeConfigIndex = 0; ExtlogSupModeConfigIndex < cerModeCfg->WdgMModeExtLogCnt;
                 ExtlogSupModeConfigIndex++)
            {
                if (cerModeCfg->WdgMExtLogSupRelated[ExtlogSupModeConfigIndex].WdgMExtLogSupID
                    == CertCheckpoint->WdgMLogSupID) /*One CPID only belong to one Logsup*/
                {
                    break;
                }
            }

            if (cerModeCfg->WdgMModeExtLogCnt <= ExtlogSupModeConfigIndex)
            {
                WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogSupResult = WDGM_INCORRECT;
            }
            else
            {
                for (TransitionIndex = 0;
                     TransitionIndex < cerModeCfg->WdgMExtLogSupRelated[ExtlogSupModeConfigIndex].WdgExtTransitionCnt;
                     TransitionIndex++)
                {
                    if ((WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLastReachedCPID
                         == cerModeCfg->WdgMExtLogSupRelated[ExtlogSupModeConfigIndex]
                                .WdgMExtTransition[TransitionIndex]
                                .WdgMTransitionSourCPID)
                        && (WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLastReachedSEID
                            == cerModeCfg->WdgMExtLogSupRelated[ExtlogSupModeConfigIndex]
                                   .WdgMExtTransition[TransitionIndex]
                                   .WdgMTransitionSourSEID))
                    {
                        if ((CertCheckpoint->WdgMCheckpointID
                             == cerModeCfg->WdgMExtLogSupRelated[ExtlogSupModeConfigIndex]
                                    .WdgMExtTransition[TransitionIndex]
                                    .WdgMTransitionDestCPID)
                            && (cerSECfg->WdgMSEId
                                == cerModeCfg->WdgMExtLogSupRelated[ExtlogSupModeConfigIndex]
                                       .WdgMExtTransition[TransitionIndex]
                                       .WdgMTransitionDestSEID))
                        {
                            WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogSupResult = WDGM_CORRECT;
                            break;
                        }
                        else
                        {
                            WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogSupResult =
                                WDGM_INCORRECT;
                        }
                    }
                }
                if (cerModeCfg->WdgMExtLogSupRelated[ExtlogSupModeConfigIndex].WdgExtTransitionCnt <= TransitionIndex)
                {
                    WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogSupResult = WDGM_INCORRECT;
                }
                else if (WDGM_CORRECT == WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogSupResult)
                {
                    WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLastReachedCPID =
                        CertCheckpoint->WdgMCheckpointID;
                    WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLastReachedSEID = cerSECfg->WdgMSEId;

                    if ((uint8)STD_ON
                        == WdgM_GetWordBitState(CertCheckpoint->WdgMSupSupportGroup, (uint8)WDGM_LOG_SUP_SUPPORT_FINAL))
                    {
                        WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLogActiveFlag = WDGM_AF_FALSE;
                        WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLastReachedSEID =
                            WDGM_NO_SE_REACH;
                        WdgM_ExtLogSupInfoRelated[CertCheckpoint->WdgMLogSupID].WdgMExtLastReachedCPID =
                            WDGM_NO_CP_REACH;
                    }
                }
                else
                {
                    /*MISRAC*/
                }
            }
        }
    }
}

/***************************************************************/
/*
 * Brief               Clear the status of the external logical supervision in the old mode.
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      oldModeCfg: the configuration of the old Mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_177
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_ClearExtlogStatus(const WdgM_ModeCfgType* oldModeCfg)
{
    uint16          oldIdx;
    uint16          tabIdx;
    volatile uint16 oldIdxMax = 0;

    oldIdxMax = oldModeCfg->WdgMModeExtLogCnt;

    for (oldIdx = 0; oldIdx < oldIdxMax; oldIdx++)
    {
        tabIdx = oldModeCfg->WdgMExtLogSupRelated[oldIdx].WdgMExtLogSupID;
        WdgM_ExtLogSupInfoRelated[tabIdx].WdgMExtLogActiveFlag   = WDGM_AF_FALSE;
        WdgM_ExtLogSupInfoRelated[tabIdx].WdgMExtLastReachedCPID = WDGM_NO_CP_REACH;
        WdgM_ExtLogSupInfoRelated[tabIdx].WdgMExtLastReachedSEID = WDGM_NO_SE_REACH;
        WdgM_ExtLogSupInfoRelated[tabIdx].WdgMExtLogSupResult    = WDGM_CORRECT;
    }
}

/***************************************************************/
/*
 * Brief               confirm the result of external logical supervision for each SE
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerModeCfg: the configuration of Mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_252
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_ConfirmExtLogSupResultEachSE(const WdgM_ModeCfgType* cerModeCfgPtr)
{
    uint16                      idx;
    uint16                      extLogIdx;
    const uint16                extLogIdxMax = cerModeCfgPtr->WdgMModeExtLogCnt;
    uint16                      SERelatedIndex;
    const uint16                idxMax = cerModeCfgPtr->WdgMModeSECnt;
    uint16                      SERelatedIndexMax;
    uint16                      logsupIdx;
    uint16                      SEStatusTabIdx;
    WdgM_SupervisedEntityIdType SEId;

    for (idx = 0; idx < idxMax; idx++) /*SE quantity*/
    {
        SEId = cerModeCfgPtr->WdgMSERelated[idx].WdgMSEId;

        SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(SEId);

        for (extLogIdx = 0; extLogIdx < extLogIdxMax; extLogIdx++) /*ExtLogSup quantity*/
        {
            SERelatedIndexMax = cerModeCfgPtr->WdgMExtLogSupRelated[extLogIdx].SEIdRelatedCnt;
            /*Related SE quantity of each ExtLog Sup*/
            for (SERelatedIndex = 0; SERelatedIndex < SERelatedIndexMax; SERelatedIndex++)
            {
                /*Find the SE related to the ExtLog Sup via SEID*/
                if (SEId == cerModeCfgPtr->WdgMExtLogSupRelated[extLogIdx].WdgMSEIdImpacted[SERelatedIndex])
                {
                    logsupIdx = cerModeCfgPtr->WdgMExtLogSupRelated[extLogIdx].WdgMExtLogSupID;
                    /*One ExtLog Sup Error in this SE*/
                    if (WDGM_INCORRECT == WdgM_ExtLogSupInfoRelated[logsupIdx].WdgMExtLogSupResult)
                    {
                        break;
                    }
                }
            }
            if (SERelatedIndexMax > SERelatedIndex)
            {
                break;
            }
        }

        if (extLogIdxMax > extLogIdx)
        {
            WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_EXTLOG_SUP] = WDGM_INCORRECT;
        }
        else
        {
            WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_EXTLOG_SUP] = WDGM_CORRECT;
        }
    }
}

#endif /*WDGM_EXTLOG_SUP_NUM > 0*/

#if ((WDGM_ALIVE_NUM > 0) || (WDGM_DEADLINE_SUP_NUM > 0))

/***************************************************************/
/*
 * Brief               change the status of all kinds of supervision in the SE
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      oldModeCfg: the configuration of old Mode
 *                     newModeCfg: the configuration of new Mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_176
 */
/**************************************************************/
static FUNC(void, WDGM_CODE)
    WdgM_ClearNoUseAliveDeadlineStatus(const WdgM_SECfgType* oldSE, const WdgM_SECfgType* NewSE)
{
    uint16          oldIdx;
    uint16          tabIdx;
    uint16          newIdx;
    uint16          innerIdx  = 0;
    volatile uint16 oldIdxMax = 0;
    volatile uint16 newIdxMax = 0;

#if (WDGM_ALIVE_NUM > 0)
    oldIdxMax = oldSE->WdgMSEAliveCnt;
    newIdxMax = NewSE->WdgMSEAliveCnt;
    for (oldIdx = 0; oldIdx < oldIdxMax; oldIdx++)
    {
        /*AliveID sorts from same to big*/
        for (newIdx = innerIdx; newIdx < newIdxMax; newIdx++)
        {
            if (oldSE->WdgMAliveSupCfg[oldIdx].WdgMAliveSupID == NewSE->WdgMAliveSupCfg[newIdx].WdgMAliveSupID)
            {
                innerIdx = newIdx + (uint16)1;
                break;
            }
        }
        if (newIdx >= newIdxMax)
        {
            tabIdx                                                  = oldSE->WdgMAliveSupCfg[oldIdx].WdgMAliveSupID;
            WdgM_AliveSupInfoTab[tabIdx].WdgMAliveCycleCounter      = 0;
            WdgM_AliveSupInfoTab[tabIdx].WdgMAliveIndicationCounter = 0;
            WdgM_AliveSupInfoTab[tabIdx].WdgMAliveSupResult         = WDGM_CORRECT;
        }
    }
#endif /*WDGM_ALIVE_NUM > 0*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
    oldIdxMax = oldSE->WdgMSEDealineCnt;
    newIdxMax = NewSE->WdgMSEDealineCnt;
    innerIdx  = 0;
    for (oldIdx = 0; oldIdx < oldIdxMax; oldIdx++)
    {
        for (newIdx = innerIdx; newIdx < newIdxMax; newIdx++)
        {
            if (oldSE->WdgMDeadlineSupCfg[oldIdx].WdgMDeadlineSupID
                == NewSE->WdgMDeadlineSupCfg[newIdx].WdgMDeadlineSupID)
            {
                innerIdx = newIdx + (uint16)1;
                break;
            }
        }
        if (newIdx >= newIdxMax)
        {
            tabIdx                                                = oldSE->WdgMDeadlineSupCfg[oldIdx].WdgMDeadlineSupID;
            WdgM_DeadlineSupInfoTab[tabIdx].WdgMTimeStampStart    = 0;
            WdgM_DeadlineSupInfoTab[tabIdx].WdgMDeadlineSupResult = WDGM_CORRECT;
        }
    }
#endif /*WDGM_DEADLINE_SUP_NUM > 0*/
}
#endif /*WDGM_ALIVE_NUM > 0 || WDGM_DEADLINE_SUP_NUM > 0*/

/***************************************************************/
/*
 * Brief               Performs the processing of the cyclic Watchdog Manager jobs
 * ServiceId           0x08
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_058,DD_2_103,DD_2_107,DD_2_108,DD_2_109,DD_2_112
 */
/**************************************************************/
void WdgM_MainFunction(void)
{
    uint16                SEStatusTabIdx;
    uint16                SEIdx;
    const WdgM_SECfgType* modeSEInfoPtr;
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
    ApplicationType partIdx = GetApplicationID();
#endif

    if (WDGM_INIT_OK == WdgM_ModeInfo.WdgMModeInitStatus)
    {
        if (WDGM_GLOBAL_STATUS_DEACTIVATED != WdgM_GlobalInfo.WdgMGlobalResult) /*req WdgM 0063*/
        {
#if (WDGM_EXTLOG_SUP_NUM > 0)
            /*Determine the External Log Sup status of each SE*/
            WdgM_ConfirmExtLogSupResultEachSE(WdgM_CurModeCfg);
#endif /*WDGM_EXTLOG_SUP_NUM > 0*/

            for (SEIdx = 0; SEIdx < WdgM_CurModeCfg->WdgMModeSECnt; SEIdx++)
            {
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
                if (partIdx == WdgM_CurModeCfg->WdgMSERelated[SEIdx].WdgMEcucPartitionRef)
#endif
                {
                    modeSEInfoPtr  = (const WdgM_SECfgType*)&WdgM_CurModeCfg->WdgMSERelated[SEIdx];
                    SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(modeSEInfoPtr->WdgMSEId);
                    if ((WDGM_LOCAL_STATUS_OK == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus)
                        || (WDGM_LOCAL_STATUS_FAILED == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus))
                    {
#if (WDGM_ALIVE_NUM > 0)
                        /*req WdgM 324 : Determine the Alive Sup Result*/
                        WdgM_ConfirmAliveSupResultEachSE(modeSEInfoPtr);
#endif /*WDGM_ALIVE_NUM > 0*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
                        /* req WdgM 403 : Deadline Timeout Detection */
                        WdgM_PerformDeadlineTimeoutDetection(modeSEInfoPtr);
                        /*req WdgM 324 : Determine the Alive Sup Result*/
                        WdgM_ConfirmDeadlineSupResultEachSE(modeSEInfoPtr);
#endif /*WDGM_DEADLINE_SUP_NUM > 0*/

#if (WDGM_INTLOG_SUP_NUM > 0)
                        /*req WdgM 324 : Determine the Internal Logical Sup Result*/
                        WdgM_ConfirmIntLogSupResultEachSE(modeSEInfoPtr);
#endif /*WDGM_INTLOG_SUP_NUM > 0*/
                        /*req WdgM 325 : Determine the Local Sup status of each SE*/
                        WdgM_ConfirmSELocalStatus(modeSEInfoPtr);
                    }
                }
            }
            /*req WdgM 326 : Determine the Global Sup status*/
            /*req WdgM 214*/
            WdgM_ConfirmGlobalStatus();

            /*req WdgM 328 : Set trigger condition base on the Global Status*/
            WdgM_SetTrigger();
        }
    }
}

/***************************************************************/
/*
 * Brief               confirm the local status of the SE
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerSEInfo: the configuration of SE
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_034,DD_2_253,DD_2_049,DD_2_050,DD_2_051,DD_2_052,
 *                     DD_2_053,DD_2_054,DD_2_055,DD_2_105,DD_2_106,DD_2_161
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_ConfirmSELocalStatus(const WdgM_SECfgType* cerSEInfo)
{
    uint16         SEStatusTabIdx;
    Std_ReturnType ret = E_OK;

    SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(cerSEInfo->WdgMSEId);

    if (WDGM_LOCAL_STATUS_OK == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus)
    {
        if ((WDGM_CORRECT == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_INTLOG_SUP])
            && (WDGM_CORRECT == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_EXTLOG_SUP])
            && (WDGM_CORRECT == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_DEADLINE_SUP]))
        {
            if (WDGM_INCORRECT == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_ALIVE_SUP])
            {
                if ((uint8)0 < cerSEInfo->WdgMFailedAliveSupervisionRefCycleTol)
                {
                    /*req WdgM 203 : Fig3-3*/
                    WdgM_SEInfoRelated[SEStatusTabIdx].WdgMFailedAliveSupCounter++;
                    WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus = WDGM_LOCAL_STATUS_FAILED;
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
                    /*DD_2_038*/
                    WdgM_RestartOSApplication(cerSEInfo);
#endif
                }
                else
                {
                    ret = E_NOT_OK;
                }
            }
        }
        else
        {
            ret = E_NOT_OK;
        }
    }
    else if (WDGM_LOCAL_STATUS_FAILED == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus)
    {
        if ((WDGM_INCORRECT == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_INTLOG_SUP])
            || (WDGM_INCORRECT == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_EXTLOG_SUP])
            || (WDGM_INCORRECT == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_DEADLINE_SUP]))
        {
            /*req WdgM 206 : Failed  to Expired*/
            ret = E_NOT_OK;
        }
        else if (WDGM_INCORRECT == WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_ALIVE_SUP])
        {
            WdgM_SEInfoRelated[SEStatusTabIdx].WdgMFailedAliveSupCounter++;
            if (cerSEInfo->WdgMFailedAliveSupervisionRefCycleTol
                < WdgM_SEInfoRelated[SEStatusTabIdx].WdgMFailedAliveSupCounter)
            {
                ret = E_NOT_OK;
            }
        }
        else
        {
            if ((uint8)1 >= WdgM_SEInfoRelated[SEStatusTabIdx].WdgMFailedAliveSupCounter)
            {
                WdgM_SEInfoRelated[SEStatusTabIdx].WdgMFailedAliveSupCounter = 0;
                /*req WdgM 205 :Fig3-5 */
                WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus = WDGM_LOCAL_STATUS_OK;
            }
            else
            {
                /*req WdgM 300 : Fig3-4 Reamin the Failed, ErrorCnt--*/
                WdgM_SEInfoRelated[SEStatusTabIdx].WdgMFailedAliveSupCounter--;
            }
        }
    }
    else
    {
        /*MISRA-C*/
    }

    if ((Std_ReturnType)E_NOT_OK == ret)
    {
        WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus = WDGM_LOCAL_STATUS_EXPIRED;
        SchM_Enter_WdgM_Exclusive();
        if ((Std_ReturnType)E_NOT_OK == WdgM_GlobalInfo.WdgMFirstExpiredSEIDSaved)
        {
            /* PRQA S 0303 ++*/ /* VL_WdgM_0303 */
            *(volatile uint16*)WDGM_FIRST_EXPIRED_SEID         = cerSEInfo->WdgMSEId;
            *(volatile uint16*)WDGM_FIRST_EXPIRED_INVERSE_SEID = ~(cerSEInfo->WdgMSEId);
            /* PRQA S 0303 --*/ /* VL_WdgM_0303 */

            WdgM_GlobalInfo.WdgMFirstExpiredSEIDSaved = E_OK;
        }
        SchM_Exit_WdgM_Exclusive();
#if (WDGM_MULTI_PARTITION_RESET_CALLBACK == STD_ON)
        WdgM_MultiPartitionReset(cerSEInfo->WdgMEcucPartitionRef);
#endif
    }
}

/***************************************************************/
/*
 * Brief               confirm the Global status
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_035, DD_2_060, DD_2_063, DD_2_064, DD_2_065, DD_2_066
 *                     DD_2_067, DD_2_068, DD_2_069, DD_2_070,DD_2_071,DD_2_072,
 *                     DD_2_073,DD_2_162
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_ConfirmGlobalStatus(void)
{
    uint16  seIdx;
    uint16  seTabIdx;
    boolean oneExipredFlag = FALSE;
    boolean failedFlag     = FALSE;

#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
    ApplicationType partIdx = GetApplicationID();
    if (partIdx == (ApplicationType)WDGM_MASTER_APPLICATION_ID)
#endif
    {
        if ((WDGM_GLOBAL_STATUS_STOPPED != WdgM_GlobalInfo.WdgMGlobalResult)
            && (WDGM_GLOBAL_STATUS_DEACTIVATED != WdgM_GlobalInfo.WdgMGlobalResult))
        {
            for (seIdx = 0; seIdx < WdgM_CurModeCfg->WdgMModeSECnt; seIdx++)
            {
                seTabIdx = WdgM_GetSupervisedEntityTabIndex(WdgM_CurModeCfg->WdgMSERelated[seIdx].WdgMSEId);
                if (WDGM_LOCAL_STATUS_EXPIRED == WdgM_SEInfoRelated[seTabIdx].WdgMLocalStatus)
                {
                    oneExipredFlag = TRUE;
                }
                else if (WDGM_LOCAL_STATUS_FAILED == WdgM_SEInfoRelated[seTabIdx].WdgMLocalStatus)
                {
                    failedFlag = TRUE;
                }
                else
                {
                    /*MISRA-C*/
                }
            }
        }

        if (oneExipredFlag)
        {
            WdgM_ModeInfo.WdgMExpiredSupTolCounter++;
            if ((WdgM_ModeInfo.WdgMExpiredSupTolCounter > WdgM_CurModeCfg->WdgMExpiredSupervisionCycleTol))
            {
                /*req WdgM 216 :Fig4-4 / WdgM 117 :Fig4-8*/
                WdgM_GlobalInfo.WdgMGlobalResult       = WDGM_GLOBAL_STATUS_STOPPED;
                WdgM_ModeInfo.WdgMExpiredSupTolCounter = 0;
#if (STD_ON == WDGM_IMMEDIATE_RESET || STD_ON == WDGM_E_SUPERVISION_ENABLE)
                /*DD_2_037*/
                WdgM_StopStatusErrorHanding();
#endif
            }
            else
            {
                WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_EXPIRED;
            }
        }
        else
        {
            switch (WdgM_GlobalInfo.WdgMGlobalResult)
            {
            case WDGM_GLOBAL_STATUS_OK:
                if (failedFlag)
                {
                    WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_FAILED;
                }
                else
                {
                    WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_OK;
                }
                break;
            case WDGM_GLOBAL_STATUS_FAILED:
                if (failedFlag)
                {
                    /* no expired, then stay global status failed */
                }
                else
                {
                    WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_OK;
                }
                break;
            case WDGM_GLOBAL_STATUS_EXPIRED:
                WdgM_ModeInfo.WdgMExpiredSupTolCounter++;
                if ((WdgM_ModeInfo.WdgMExpiredSupTolCounter > WdgM_CurModeCfg->WdgMExpiredSupervisionCycleTol))
                {
                    /*req WdgM 216 :Fig4-4 / WdgM 117 :Fig4-8*/
                    WdgM_GlobalInfo.WdgMGlobalResult       = WDGM_GLOBAL_STATUS_STOPPED;
                    WdgM_ModeInfo.WdgMExpiredSupTolCounter = 0;
#if (STD_ON == WDGM_IMMEDIATE_RESET || STD_ON == WDGM_E_SUPERVISION_ENABLE)
                    /*DD_2_037*/
                    WdgM_StopStatusErrorHanding();
#endif
                }
                else
                {
                    WdgM_GlobalInfo.WdgMGlobalResult = WDGM_GLOBAL_STATUS_EXPIRED;
                }
                break;
            default:
                /* do noting*/
                break;
            }
        }
    }
}

#if (STD_ON == WDGM_IMMEDIATE_RESET || STD_ON == WDGM_E_SUPERVISION_ENABLE)
/***************************************************************/
/*
 * Brief               Error operation when the global status is stop.
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_163
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_StopStatusErrorHanding(void)
{
#if (STD_ON == WDGM_E_SUPERVISION_ENABLE)
    Dem_ReportErrorStatus(WDGM_E_SUPERVISION, DEM_EVENT_STATUS_FAILED); /* req WdgM 129*/
#endif                                                                  /*STD_ON == WDGM_E_SUPERVISION_ENABLE*/

#if (STD_ON == WDGM_IMMEDIATE_RESET)
    /*DD_2_039:(1)*/
    /*ShutdownOS shall Deinit the RAM for OS, this shall forbid the fail of startOS*/
    SchM_Enter_WdgM_Exclusive();
    Mcu_PerformReset(); /* req WdgM 133*/
    SchM_Exit_WdgM_Exclusive();
/*req WdgM 134: no notification to the application via the RTE */
#endif /*STD_ON == WDGM_IMMEDIATE_RESET*/
}
#endif

/***************************************************************/
/*
 * Brief               OS-application Reset when SE in this OS-application in error.
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerSEInfo : the configuration of the SE
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_258
 */
/**************************************************************/
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
static FUNC(void, WDGM_CODE) WdgM_RestartOSApplication(const WdgM_SECfgType* cerSEInfo)
{
    if ((uint16)WDGM_NO_OS_APPLICAITON_REF != cerSEInfo->WdgMEcucPartitionRef)
    {
#if (WDGM_MULTI_PARTITION_RESET_CALLBACK == STD_ON)
        WdgM_MultiPartitionReset(cerSEInfo->WdgMEcucPartitionRef);
#endif
#if (WDGM_BSWM_ENABLED == STD_ON)
        (void)BswM_WdgM_RequestPartitionReset(cerSEInfo->WdgMEcucPartitionRef);

#endif
    }
}
#endif

/***************************************************************/
/*
 * Brief               Set Trigger condition
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_041, DD_2_042, DD_2_043, DD_2_044, DD_2_045, DD_2_046,DD_2_165
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_SetTrigger(void)
{
    uint8 triggerIndex;
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
    ApplicationType partIdx = GetApplicationID();
    if (partIdx == (ApplicationType)WDGM_MASTER_APPLICATION_ID)
#endif
    {
        if ((WDGM_GLOBAL_STATUS_OK == WdgM_GlobalInfo.WdgMGlobalResult)
            || (WDGM_GLOBAL_STATUS_FAILED == WdgM_GlobalInfo.WdgMGlobalResult)
            || (WDGM_GLOBAL_STATUS_EXPIRED == WdgM_GlobalInfo.WdgMGlobalResult))
        {
            for (triggerIndex = 0; triggerIndex < WdgM_CurModeCfg->WdgMModeTriggerCnt; triggerIndex++)
            {
                if (WDGIF_OFF_MODE != WdgM_CurModeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
                {
                    WdgIf_SetTriggerCondition(
                        WdgM_CurModeCfg->WdgMTriggerRelated[triggerIndex].WdgM_WatchdogDevice,
                        WdgM_CurModeCfg->WdgMTriggerRelated[triggerIndex].WdgMTriggerConditionValue);
                }
            }
        }
        else if (WDGM_GLOBAL_STATUS_STOPPED == WdgM_GlobalInfo.WdgMGlobalResult)
        {
            for (triggerIndex = 0; triggerIndex < WdgM_CurModeCfg->WdgMModeTriggerCnt; triggerIndex++)
            {
                if (WDGIF_OFF_MODE != WdgM_CurModeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
                {
                    WdgIf_SetTriggerCondition(
                        WdgM_CurModeCfg->WdgMTriggerRelated[triggerIndex].WdgM_WatchdogDevice,
                        WDGM_TRIGGER_CONDITION_STOP);
                }
            }
        }
        else
        {
            /*do nothing*/
        }
    }
}

/***************************************************************/
/*
 * Brief               Get the array index of the CP in the SE
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      CPID: the Checkpoint ID
 *                     cerSECfg: the configuration of the SE
 * Param-Name[out]     CPTabIdx: the array index of the CP in the SE
 * Param-Name[in/out]  None
 * Return              E_NOT_OK,E_OK
 * CallByAPI           <>
 * REQ ID              DD_2_168
 */
/**************************************************************/
static FUNC(Std_ReturnType, WDGM_CODE)
    WdgM_GetSECheckpointIndex(WdgM_CheckpointIdType CPID, const WdgM_SECfgType* cerSECfg, uint16* CPTabIdx)
{
    Std_ReturnType retValue = E_NOT_OK;
    uint16         CPIdx;

    for (CPIdx = 0; CPIdx < cerSECfg->WdgMSECPCnt; CPIdx++)
    {
        if (cerSECfg->WdgMCheckpointRelated[CPIdx].WdgMCheckpointID == CPID)
        {
            *CPTabIdx = CPIdx;
            retValue  = E_OK;
            break;
        }
    }

    return retValue;
}

/***************************************************************/
/*
 * Brief               Init the status of all the array ralated to the SE.
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_048,DD_2_084,DD_2_085,DD_2_086,DD_2_087,DD_2_179
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_InitAllTabStatus(void)
{
    uint16 idx;

    for (idx = 0; idx < WDGM_SE_NUM; idx++)
    {
        /*req WdgM 269*/
        WdgM_SEInfoRelated[idx].WdgMLocalStatus                      = WDGM_LOCAL_STATUS_DEACTIVATED;
        WdgM_SEInfoRelated[idx].WdgMFailedAliveSupCounter            = 0;
        WdgM_SEInfoRelated[idx].WdgMEachSupResult[WDGM_ALIVE_SUP]    = WDGM_CORRECT;
        WdgM_SEInfoRelated[idx].WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_CORRECT;
        WdgM_SEInfoRelated[idx].WdgMEachSupResult[WDGM_INTLOG_SUP]   = WDGM_CORRECT;
        WdgM_SEInfoRelated[idx].WdgMEachSupResult[WDGM_EXTLOG_SUP]   = WDGM_CORRECT;
    }

#if (WDGM_ALIVE_NUM > 0)
    for (idx = 0; idx < WDGM_ALIVE_NUM; idx++)
    {
        WdgM_AliveSupInfoTab[idx].WdgMAliveCycleCounter      = 0;
        WdgM_AliveSupInfoTab[idx].WdgMAliveIndicationCounter = 0;
        WdgM_AliveSupInfoTab[idx].WdgMAliveSupResult         = WDGM_CORRECT;
    }
#endif /*WDGM_ALIVE_NUM  > 0*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
    for (idx = 0; idx < WDGM_DEADLINE_SUP_NUM; idx++)
    {
        WdgM_DeadlineSupInfoTab[idx].WdgMTimeStampStart    = 0;
        WdgM_DeadlineSupInfoTab[idx].WdgMDeadlineSupResult = WDGM_CORRECT;
    }
#endif /*WDGM_DEADLINE_SUP_NUM  > 0*/

#if (WDGM_INTLOG_SUP_NUM > 0)
    for (idx = 0; idx < WDGM_INTLOG_SUP_NUM; idx++)
    {
        WdgM_IntLogSupInfoTab[idx].WdgMIntLogActiveFlag   = WDGM_AF_FALSE; /*DD_2_021*/
        WdgM_IntLogSupInfoTab[idx].WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
        WdgM_IntLogSupInfoTab[idx].WdgMIntLogSupResult    = WDGM_CORRECT;
    }
#endif /*WDGM_INTLOG_SUP_NUM  > 0*/

#if (WDGM_EXTLOG_SUP_NUM > 0)
    for (idx = 0; idx < WDGM_EXTLOG_SUP_NUM; idx++)
    {
        WdgM_ExtLogSupInfoRelated[idx].WdgMExtLogActiveFlag   = WDGM_AF_FALSE; /*DD_2_021*/
        WdgM_ExtLogSupInfoRelated[idx].WdgMExtLastReachedCPID = WDGM_NO_CP_REACH;
        WdgM_ExtLogSupInfoRelated[idx].WdgMExtLastReachedSEID = WDGM_NO_SE_REACH;
        WdgM_ExtLogSupInfoRelated[idx].WdgMExtLogSupResult    = WDGM_CORRECT;
    }
#endif /*WDGM_EXTLOG_SUP_NUM  > 0*/
}

/***************************************************************/
/*
 * Brief               Init the local status of all SE in the Mode
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerModeCfgPtr: the configuration of certain Mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_047,DD_2_180
 */
/**************************************************************/

static FUNC(void, WDGM_CODE) WdgM_InitSEStatus(const WdgM_ModeCfgType* cerModeCfgPtr)
{
    uint16                SEIdx;
    uint16                SEStatusTabIdx;
    const WdgM_SECfgType* modeSEInfoPtr;

    for (SEIdx = 0; SEIdx < cerModeCfgPtr->WdgMModeSECnt; SEIdx++)
    {
        modeSEInfoPtr  = (const WdgM_SECfgType*)&cerModeCfgPtr->WdgMSERelated[SEIdx];
        SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(modeSEInfoPtr->WdgMSEId);
        /*req WdgM 268*/
        WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus = WDGM_LOCAL_STATUS_OK;
    }
}

/***************************************************************/
/*
 * Brief               Deinit the local status of all SE in the Mode
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerModeCfgPtr: the configuration of certain Mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_047,DD_2_180
 */
/**************************************************************/

static FUNC(void, WDGM_CODE) WdgM_DeinitSEStatus(const WdgM_ModeCfgType* cerModeCfgPtr)
{
    uint16                SEIdx;
    uint16                SEStatusTabIdx;
    const WdgM_SECfgType* modeSEInfoPtr;
    for (SEIdx = 0; SEIdx < cerModeCfgPtr->WdgMModeSECnt; SEIdx++)
    {
        modeSEInfoPtr  = (const WdgM_SECfgType*)&cerModeCfgPtr->WdgMSERelated[SEIdx];
        SEStatusTabIdx = WdgM_GetSupervisedEntityTabIndex(modeSEInfoPtr->WdgMSEId);
        /*req WdgM 269*/
        WdgM_SEInfoRelated[SEStatusTabIdx].WdgMLocalStatus = WDGM_LOCAL_STATUS_DEACTIVATED;
    }
}

/***************************************************************/
/*
 * Brief               Get the index of the SE in the array of the configured SE Tab
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerModeCfgPtr: the configuration of certain Mode
 *                     SEID: The supervision Entity ID
 * Param-Name[out]     modeSEIdx: the index of the SE in the array of the configured SE Tab
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_166
 */
/**************************************************************/

static FUNC(Std_ReturnType, WDGM_CODE)
    WdgM_GetModeSEIndex(WdgM_SupervisedEntityIdType SEID, uint16* modeSEIdx, const WdgM_ModeCfgType* cerModeCfgPtr)
{
    uint16         SEIdx;
    Std_ReturnType retValue = E_NOT_OK;

    for (SEIdx = 0; SEIdx < cerModeCfgPtr->WdgMModeSECnt; SEIdx++)
    {
        if (cerModeCfgPtr->WdgMSERelated[SEIdx].WdgMSEId == SEID)
        {
            *modeSEIdx = SEIdx;
            retValue   = E_OK;
            break;
        }
    }
    return retValue;
}

/***************************************************************/
/*
 * Brief               change the status of all kinds of supervision in the SE
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      oldModeCfg: the configuration of old Mode
 *                     newModeCfg: the configuration of new Mode
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_056,DD_2_057,DD_2_059,DD_2_075,DD_2_076,DD_2_175
 */
/**************************************************************/
static FUNC(void, WDGM_CODE) WdgM_ChangeSEStatus(const WdgM_ModeCfgType* oldModeCfg, const WdgM_ModeCfgType* newModeCfg)
{
    uint16                  innerIdx = 0;
    uint16                  oldIdx;
    uint16                  oldSEStaTabIdx;
    uint16                  oldIdxMax = oldModeCfg->WdgMModeSECnt; /* Quantities for previous SE */
    uint16                  newIdx;
    uint16                  newSETabStaIdx;
    uint16                  newIdxMax = newModeCfg->WdgMModeSECnt; /* Quantities for new SE */
    volatile Std_ReturnType retActive = E_NOT_OK;
    /* temporal variable for previous SE information  */
    /* PRQA S 0686 ++*/ /* VL_WdgM_0686 */
    WdgM_LocalStatusType oldSEStaArray[WDGM_SE_NUM] = {WDGM_LOCAL_STATUS_OK};
    /* PRQA S 0686 --*/ /* VL_WdgM_0686 */

    for (oldIdx = 0; oldIdx < oldIdxMax; oldIdx++)
    {
        oldSEStaTabIdx = WdgM_GetSupervisedEntityTabIndex(oldModeCfg->WdgMSERelated[oldIdx].WdgMSEId);
        /*Save the old SE in temporary variable*/
        oldSEStaArray[oldSEStaTabIdx] = WdgM_SEInfoRelated[oldSEStaTabIdx].WdgMLocalStatus;
    }

    for (newIdx = 0; newIdx < newIdxMax; newIdx++)
    {
        newSETabStaIdx = WdgM_GetSupervisedEntityTabIndex(newModeCfg->WdgMSERelated[newIdx].WdgMSEId);
        /* req WdgM 209 Old Active != New Active, the new SE should be deault OK*/
        WdgM_SEInfoRelated[newSETabStaIdx].WdgMLocalStatus           = WDGM_LOCAL_STATUS_OK;
        WdgM_SEInfoRelated[newSETabStaIdx].WdgMFailedAliveSupCounter = 0; /*SWS_WdgM_00209*/
    }

    /*SE should sort from small to large in config*/
    for (oldIdx = 0; oldIdx < oldIdxMax; oldIdx++)
    {
        retActive = E_NOT_OK;
        for (newIdx = innerIdx; newIdx < newIdxMax; newIdx++)
        {
            newSETabStaIdx = WdgM_GetSupervisedEntityTabIndex(newModeCfg->WdgMSERelated[newIdx].WdgMSEId);

            if (oldModeCfg->WdgMSERelated[oldIdx].WdgMSEId
                == newModeCfg->WdgMSERelated[newIdx].WdgMSEId) /*Both Mode Active */
            {
                WdgM_SEInfoRelated[newSETabStaIdx].WdgMLocalStatus = oldSEStaArray[newSETabStaIdx];
#if ((WDGM_ALIVE_NUM > 0) || (WDGM_DEADLINE_SUP_NUM > 0))
                WdgM_ClearNoUseAliveDeadlineStatus(
                    (const WdgM_SECfgType*)&oldModeCfg->WdgMSERelated[oldIdx],
                    (const WdgM_SECfgType*)&newModeCfg->WdgMSERelated[newIdx]);
#endif /*WDGM_ALIVE_NUM > 0  ||  WDGM_DEADLINE_SUP_NUM > 0*/
                innerIdx = newIdx + (uint16)1;
                /*WdgMLocalStatusSupervisedEntityRef already active in old mode */
                retActive = E_OK;
                break;
            }
            else
            {
                /*MISAR-C*/
            }
        }
        if ((Std_ReturnType)E_NOT_OK == retActive)
        {
            oldSEStaTabIdx = WdgM_GetSupervisedEntityTabIndex(oldModeCfg->WdgMSERelated[oldIdx].WdgMSEId);
            /*req WdgM 207 208 291*/
            WdgM_SEInfoRelated[oldSEStaTabIdx].WdgMLocalStatus = WDGM_LOCAL_STATUS_DEACTIVATED;
            WdgM_SEDeactivateInternal(oldModeCfg, oldIdx, oldSEStaTabIdx); /*req WdgM 315*/
        }
    }

#if (WDGM_EXTLOG_SUP_NUM > 0)
    if ((uint16)0 < oldModeCfg->WdgMModeExtLogCnt) /*Mode contains the ExtLogSup*/
    {
        WdgM_ClearExtlogStatus(oldModeCfg);
    }
#endif /*WDGM_EXTLOG_SUP_NUM > 0*/
}

/***************************************************************/
/*
 * Brief               Get the index of the SE in the array in the WdgM_SupervisedEntityToTabIndex
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerSEID: the supervision entity ID
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              the index of the SE in the array in the WdgM_SupervisedEntityToTabIndex
 * CallByAPI           <>
 * REQ ID              DD_2_267
 */
/**************************************************************/
static FUNC(uint16, WDGM_CODE) WdgM_GetSupervisedEntityTabIndex(WdgM_SupervisedEntityIdType cerSEID)
{
    uint16 tabIdx;

    for (tabIdx = 0; tabIdx < WDGM_SE_NUM; tabIdx++)
    {
        if (cerSEID == WdgM_SupervisedEntityToTabIndex[tabIdx])
        {
            break;
        }
    }
    return tabIdx;
}

/***************************************************************/
/*
 * Brief               Clear the status of SE that deactivate in the new Mode
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      cerModeCfg: the configuration of new Mode
 *                     SEIdx: The index in the array of the SE configuration
 *                     SEStatusTabIdx : The index in the array of the WdgM_SEInfoRelated
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_127
 */
/**************************************************************/
static FUNC(void, WDGM_CODE)
    WdgM_SEDeactivateInternal(const WdgM_ModeCfgType* cerModeCfg, uint16 SEIdx, uint16 SEStatusTabIdx)
{
#if ((WDGM_ALIVE_NUM > 0) || (WDGM_DEADLINE_SUP_NUM > 0) || (WDGM_INTLOG_SUP_NUM > 0))
    const WdgM_SECfgType* modeSEInfoPtr;
    uint16                tabIdx;
    uint16                idx;

    WdgM_SEInfoRelated[SEStatusTabIdx].WdgMFailedAliveSupCounter            = 0;
    WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_ALIVE_SUP]    = WDGM_CORRECT;
    WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_CORRECT;
    WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_INTLOG_SUP]   = WDGM_CORRECT;
    WdgM_SEInfoRelated[SEStatusTabIdx].WdgMEachSupResult[WDGM_EXTLOG_SUP]   = WDGM_CORRECT;
    modeSEInfoPtr = (const WdgM_SECfgType*)&cerModeCfg->WdgMSERelated[SEIdx];

#if (WDGM_ALIVE_NUM > 0)
    if ((uint16)0 < modeSEInfoPtr->WdgMSEAliveCnt)
    {
        for (idx = 0; idx < modeSEInfoPtr->WdgMSEAliveCnt; idx++)
        {
            tabIdx                                             = modeSEInfoPtr->WdgMAliveSupCfg[idx].WdgMAliveSupID;
            WdgM_AliveSupInfoTab[tabIdx].WdgMAliveCycleCounter = 0;
            WdgM_AliveSupInfoTab[tabIdx].WdgMAliveIndicationCounter = 0;
            WdgM_AliveSupInfoTab[tabIdx].WdgMAliveSupResult         = WDGM_CORRECT;
        }
    }
#endif /*WDGM_ALIVE_NUM > 0*/

#if (WDGM_DEADLINE_SUP_NUM > 0)
    if ((uint16)0 < modeSEInfoPtr->WdgMSEDealineCnt)
    {
        for (idx = 0; idx < modeSEInfoPtr->WdgMSEDealineCnt; idx++)
        {
            tabIdx = modeSEInfoPtr->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupID;
            WdgM_DeadlineSupInfoTab[tabIdx].WdgMTimeStampStart    = 0;
            WdgM_DeadlineSupInfoTab[tabIdx].WdgMDeadlineSupResult = WDGM_CORRECT;
        }
    }
#endif /*WDGM_DEADLINE_SUP_NUM > 0*/

#if (WDGM_INTLOG_SUP_NUM > 0)
    if ((uint16)0 < modeSEInfoPtr->WdgMSEIntLogCnt)
    {
        tabIdx                                               = modeSEInfoPtr->WdgMIntLogSupCfg[0].WdgMIntLogSupID;
        WdgM_IntLogSupInfoTab[tabIdx].WdgMIntLogActiveFlag   = WDGM_AF_FALSE;
        WdgM_IntLogSupInfoTab[tabIdx].WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
        WdgM_IntLogSupInfoTab[tabIdx].WdgMIntLogSupResult    = WDGM_CORRECT;
    }
#endif /*WDGM_INTLOG_SUP_NUM > 0*/
#endif /*WDGM_ALIVE_NUM or WDGM_DEADLINE_SUP_NUM or WDGM_INTLOG_SUP_NUM*/
}

#if (STD_ON == WDGM_DEV_ERROR_DETECT)
/***************************************************************/
/*
 * Brief               confirm whether the Mode ID in the configuration
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      modeId: the Mode ID
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_NOT_OK,E_OK
 * CallByAPI           <>
 * REQ ID              DD_2_173
 */
/**************************************************************/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_ModeExistenceConfirmation(WdgM_ModeType modeId)
{
    Std_ReturnType retValue = E_NOT_OK;
    uint8          idx;
    uint8          idxMax = WdgM_Config->WdgMModeCnt;

    for (idx = 0; idx < idxMax; idx++)
    {
        if (modeId == WdgM_Config->WdgMModeRef[idx].WdgMModeID)
        {
            retValue = E_OK;
            break;
        }
    }
    return retValue;
}

/***************************************************************/
/*
 * Brief               Confirm whether contain the OFF MODE
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ConfigPtr :
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Success
 *                     E_NOT_OK: failure
 * CallByAPI           <>
 * REQ ID              DD_2_147
 */
/**************************************************************/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_InitModeConfirmation(const WdgM_ConfigType* ConfigPtr)
{
    Std_ReturnType retValue;

    /*Whether the address of the  initial Mode */
    if (ConfigPtr == &(WdgMConfigRoot[0]))
    {
        retValue = E_OK;
    }
    else
    {
        retValue = E_NOT_OK;
    }
    return retValue;
}

#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/

#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
/***************************************************************/
/*
 * Brief               Set Trigger condition
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * CallByAPI           <>
 * REQ ID              DD_2_174
 */
/**************************************************************/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_OffModeConfirmationForRequestMode(WdgM_ModeType modeId)
{
    Std_ReturnType retValue        = E_OK;
    uint8          triggerIdx      = 0;
    uint8          triggerIndexMax = WdgM_Config->WdgMModeRef[modeId].WdgMModeTriggerCnt;

    /*Trigger quantities in certain Mode*/
    for (triggerIdx = 0; triggerIdx < triggerIndexMax; triggerIdx++)
    {
        if (WDGIF_OFF_MODE == WdgM_Config->WdgMModeRef[modeId].WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode)
        {
            retValue = E_NOT_OK;
            break;
        }
    }

    return retValue;
}

#if (STD_ON == WDGM_DEV_ERROR_DETECT)
/***************************************************************/
/*
 * Brief               Confirm whether contain the OFF MODE
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ConfigPtr :
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Success
 *                     E_NOT_OK: failure
 * CallByAPI           <>
 * REQ ID              DD_2_178
 */
/**************************************************************/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_OffModeConfirmation(const WdgM_ConfigType* ConfigPtr)
{
    Std_ReturnType retValue;
    uint8          triggerIdx = 0;

    /****************************************/
    if ((uint8)0 == ConfigPtr->WdgMModeCnt)
    {
        retValue = E_OK;
    }
    else
    {
        /*Trigger quantities in each Mode*/
        retValue = E_OK;
        for (triggerIdx = 0; triggerIdx < ConfigPtr->WdgMModeRef[ConfigPtr->WdgMInitialModeId].WdgMModeTriggerCnt;
             triggerIdx++)
        {
            if (WDGIF_OFF_MODE
                == ConfigPtr->WdgMModeRef[ConfigPtr->WdgMInitialModeId].WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode)
            {
                retValue = E_NOT_OK;
                break;
            }
        }
    }
    return retValue;
}

#endif /*STD_ON == WDGM_DEV_ERROR_DETECT*/

#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/

/***************************************************************/
/*
 * Brief               Get the bit value in the word
 * ServiceId           None
 * Sync/Async          synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      wordNum: Word Value
 *                     getBit : Bit Position
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              1 : this bit value is 1; 0: this bit value is 0
 * CallByAPI           <>
 * REQ ID              DD_2_264
 */
/**************************************************************/
static FUNC(uint8, WDGM_CODE) WdgM_GetWordBitState(uint16 wordNum, uint8 getBit)
{
    uint8 bitvalue;

    if ((uint16)0U != (wordNum & (uint16)((uint16)0x0001U << getBit)))
    {
        bitvalue = 1;
    }
    else
    {
        bitvalue = 0;
    }
    return bitvalue;
}

#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/*=======[M I S R A C  R U L E  V I O L A T I O N]============================*/
/*  <MESSAGE ID>    <CODE LINE>    <REASON>
 *  ISOFT_026        2524          The input parameter shall be enumerated by OS,
 *                                 then use the enumeration here.
 *
 */
/*============================================================================*/
