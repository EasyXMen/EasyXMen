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
 **  @file               : WdgM.c
 **  @author             : xudong.guan
 **  @date               : 2024/08/20
 **  @vendor             : isoft

 **  @description        : Implementation for WdgM
 **
 ***********************************************************************************************************************/
/* PRQA S 6520 EOF */ /* VL_MTR_WdgM_STVAR */
/* =================================================== inclusions =================================================== */
#include "WdgM_Internal.h"
#include "WdgIf.h" /* PRQA S 0380 */ /* VL_WdgM_0380 */
#include "SchM_WdgM.h"
#include "Det.h"
#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
#include "Dem.h"
#endif /*WDGM_E_SUPERVISION_ENABLE == STD_ON*/
#if (WDGM_IMMEDIATE_RESET == STD_ON)
#include "Mcu.h"
#endif /*WDGM_IMMEDIATE_RESET == STD_ON*/

/* ============================================ internal data definitions =========================================== */

#define WDGM_START_SEC_VAR_INIT_8
#include "WdgM_MemMap.h"
WDGM_LOCAL WdgM_GlobalStatusType WdgM_GlobalStatusInfo = WDGM_GLOBAL_STATUS_DEACTIVATED;
#define WDGM_STOP_SEC_VAR_INIT_8
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_CLEARED_BOOLEAN
#include "WdgM_MemMap.h"
WDGM_LOCAL boolean WdgM_ResetRequest;
#define WDGM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"
WDGM_LOCAL WdgM_LocalStatusType WdgM_LocalStatusInfo[WDGM_SE_NUM];
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_FAST_CLEARED_GLOBAL_PTR
#include "WdgM_MemMap.h"
WDGM_LOCAL const WdgM_ConfigType* WdgM_ConfigPtr;
#define WDGM_STOP_SEC_VAR_FAST_CLEARED_GLOBAL_PTR
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_VAR_FAST_CLEARED_GLOBAL_PTR
#include "WdgM_MemMap.h"
WDGM_LOCAL const WdgM_ModeCfgType* WdgM_ModeCfgPtr;
#define WDGM_STOP_SEC_VAR_FAST_CLEARED_GLOBAL_PTR
#include "WdgM_MemMap.h"

/* ========================================== internal function declarations ======================================== */
/**
 * @brief The function returns the result if init is OK.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @retval      TRUE: Initialize WdgM is FAILED.
 * @retval      FALSE: Initialize WdgM is OK.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-67744
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_DoInit(const WdgM_ConfigType* ConfigPtr);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Init the releated Supervised Entity Status of current Mode.
 * @param[in]   modeCfgPtr: Pointer to current mode config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63749
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_InitSEStatus(const WdgM_ModeCfgType* modeCfgPtr);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief DeInit the releated Supervised Entity Status of current Mode.
 * @param[in]   seInfo: Pointer to current SE config data in mode.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ClearModeSEStatus(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Init the releated Supervision Status of current mode.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68344
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_InitLocalSupRunTimeStatus(const WdgM_ModeCfgType* modeCfgPtr);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Init WDG trigger driver.
 * @param[in]   modeCfgPtr: Pointer to WdgM mode configuration.
 * @retval      TRUE: Initialize Wdg Trigger within the current Config is SUCCESS.
 * @retval      FALSE: Initialize Wdg Trigger within the current Config is FAILED.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63771
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_InitWdgTrigger(const WdgM_ModeCfgType* modeCfgPtr);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Init variable of mainFunctions.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_InitModeMainFunctionState(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Deinit the SE status.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63742
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_DeinitSEStatus(const WdgM_ModeCfgType* modeCfgPtr);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Call kinds of supervision functions to confirm all supervisions results.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67745
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmSupervisionResult(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Update the alive indication counter if current CP has been reached.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63762
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateAICounter(uint16 cpIndex, const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Confirm the alive supervision result of current SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63736
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmAliveSupResultEachSE(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Confirm the result of alive temp supervision.
 * @param[in]   aliveCycleCounter: Alive supervision cycle counter.
 * @param[in]   refCycle: Current reference mainfunction recycle.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63737
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_ConfirmAliveSupResult(uint16* aliveCycleCounter, uint16 refCycle);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Perform handle the alive failed tolerance comupute during confirmation when the status is FAILED .
 * @param[in]   SEID: Supervised Entity Id.
 * @param[in]   aliveFailedTol: Alive failed tolerance time of current SE.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67748
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_ConfirmSELocalStatusHandleFAILED(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

/**
 * @brief Confirm the SE local status of current SE during mainfunction.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63766
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmSELocalStatus(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Perform handle the alive failed tolerance comupute during confirmation when the status is OK.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67747
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_ConfirmSELocalStatusHandleOK(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Confirm global status during mainfunction.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63765
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmGlobalStatus(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief Confirm whether the mode id is in the configuration.
 * @param[in]   modeID: Mode Id.
 * @retval      TRUE: Mode is in current configuration.
 * @retval      FALSE: Mode is not in current configuration.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63752
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_ModeExistenceConfirmation(WdgM_ModeType modeID);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/

/**
 * @brief Get the index of the SE in the array of the configured SE Tab.
 * @param[in]   SEID: Supervised Entity Id.
 * @param[out]  modeSETabIndex: Index of in current mode related SE list.
 * @retval      TRUE: SE index is in current mode.
 * @retval      FALSE: SE index is not in current mode.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63744
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_GetModeSEIndex(WdgM_SupervisedEntityIdType SEID, uint16* modeSETabIndex);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Get the array index of the CP in the SE.
 * @param[in]   CPID: Checkpoint Id.
 * @param[in]   seInfo: Pointer to current SE data.
 * @param[out]  cpTabIndex: Checkpoint index in CP config data.
 * @retval      TRUE: CP index is in current SE.
 * @retval      FALSE: CP index is not in current SE.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63746
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType
    WdgM_GetSECheckpointIndex(WdgM_CheckpointIdType CPID, const WdgM_ModeSECfgType* seInfo, uint16* cpTabIndex);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
/**
 * @brief Confirm whether contains the off mode if OFF_MODE is disabled.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @retval      TRUE:    Off mode exists.
 * @retval      FALSE:   Off mode doesn't exist.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63753
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_OffModeConfirmation(const WdgM_ConfigType* ConfigPtr);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Check if the requested mode would disable the watchdog .
 * @param[in]   modeID: Mode Id.
 * @retval      TRUE: Requested mode will not disable the watchdog.
 * @retval      FALSE: Requested mode will disable the watchdog.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63754
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_OffModeConfirmationForRequestMode(WdgM_ModeType modeID);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/

/**
 * @brief If global status is ok/failed WdgM will set the trigger condition.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63757
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_SetTrigger(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_IMMEDIATE_RESET == STD_ON)
/**
 * @brief Error operation when the global status is stop.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63758
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_StopStatusErrorHanding(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_IMMEDIATE_RESET == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Update the deadline supervision result if current CP has been reached.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @param[in]   curWdgMTime WdgM_DeadlineTimeType.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63768
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void
    WdgM_UpdateDeadlineSup(uint16 cpIndex, const WdgM_ModeSECfgType* seInfo, WdgM_DeadlineTimeType curWdgMTime);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Perform the deadline supervision algorithm.
 * @param[in]   cpSupportDeadlineInfo: Deadline supervision info data which supported by Checkpoint.
 * @param[in]   currentDeadlineTab: Deadline supervision runtime data pointer.
 * @param[in]   seInfo: Pointer to current SE data.
 * @param[in]   curWdgMTime WdgM_DeadlineTimeType.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68257
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_DoUpdateDeadlineSup(
    const WdgM_CPSupportDeadLineInfoType* cpSupportDeadlineInfo,
    const WdgM_ModeSECfgType*             seInfo,
    WdgM_DeadlineTimeType                 curWdgMTime);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Update the deadline supervision result of current deadline config.
 * @param[in]   timeGap: Time gap between start checkpoint and stop checkpoint in deadline supervision.
 * @param[in]   deadlineSupCfg: Pointer to deadline supervision config data.
 * @param[in]   currentDeadlineTab: Deadline supervision runtime data pointer.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63764
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void
    WdgM_UpdateDeadlineSupResult(WdgM_DeadlineTimeType timeGap, const WdgM_DeadlineSupCfgType* deadlineSupCfg);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_DEADLINE_TIMEOUT_DETECT_ENABLE == STD_ON)
/**
 * @brief Perform the deadline timeout detection during mainfuncion.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63767
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_PerformDeadlineTimeoutDetection(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif

/**
 * @brief Confirm the deadline supervision result of current SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63738
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmDeadlineSupResultEachSE(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Update the internal logical supervision result of current internal logical config.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63770
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateIntLogSup(uint16 cpIndex, const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Confirm the internal logical supervision result of current SE.
 * @param[in]   SEID: Supervised Entity Id.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63740
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmIntLogSupResultEachSE(const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Perform the external logical supervision algorithm.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63769
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateExtLogSup(uint16 cpIndex, const WdgM_ModeSECfgType* seInfo);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Update the external logical supervision result of current external logical config.
 * @param[in]   currentExtLogSupTab: External logical supervision runtime data pointer.
 * @param[in]   currentExtLogSup: External logical supervision config data pointer.
 * @param[in]   CheckpointID: Checkpoint Id .
 * @param[in]   SEID: Supervised Entity Id.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68368
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateExtLogSupResult(
    const WdgM_ExtLogSupCfgType*      currentExtLogSup,
    const WdgM_CheckpointIdType       CheckpointID,
    const WdgM_SupervisedEntityIdType SEID);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Confirm the external logical supervision result of current mode.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63739
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmExtLogSupResultEachSE(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

/**
 * @brief Call the kinds of supervision update functions to check the supervision status.
 * @param[in]   seInfo: Pointer to current SE data.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67756
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateSupervision(const WdgM_ModeSECfgType* seInfo, uint16 cpIndex);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Get the bit value in the word.
 * @param[in]   wordNum: Data to get bit.
 * @param[in]   getBit: The bit which to get.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63748
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL uint8 WdgM_GetWordBitState(uint16 wordNum, uint8 getBit);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Perform the mode change.
 * @param[in]   Mode: Mode id.
 * @retval      TRUE: Set Mode is OK.
 * @retval      FALSE: Set Mode is FAILED.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-67758
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_DoSetMode(WdgM_ModeType Mode);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Perform the mode change.
 * @param[in]   oldModeCfg: Pointer to Old mode config data.
 * @param[in]   newModeCfg: Pointer to New mode config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_LocalStatusChangeWithSetMode(const WdgM_ModeCfgType* newModeCfg);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Perform the checkpoint reached.
 * @param[in]   seInfo: Pointer to current SE data.
 * @param[in]   CheckpointID: Checkpoint Id.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67759
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_DoCheckpointReached(const WdgM_ModeSECfgType* seInfo, WdgM_CheckpointIdType CheckpointID);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief Perform the params check during checkpoint reached.
 * @param[in]   SEID: Supervised Entity Id.
 * @param[in]   CheckpointID: Checkpoint Id .
 * @retval      TRUE: Params check is OK.
 * @retval      FALSE: Params check is FAILED.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67761
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType
    WdgM_DoParamsCheckforCheckpointReached(WdgM_SupervisedEntityIdType SEID, WdgM_CheckpointIdType CheckpointID);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif

#if (WDGM_PARTITION_ENABLED == STD_ON)
/**
 * @brief Perform the limits check during checkpoint reached.
 * @param[in]   SEID: Supervised Entity Id.
 * @retval      TRUE:    Limits check is OK.
 * @retval      FALSE:   Limits check is FAILED.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68369
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_CheckpointReachedLimitCheck(const uint16 ecucPartition);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_PARTITION_ENABLED == STD_ON*/

#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Report to Dem when global status reached STOPPED and cause reset.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_SetDemEventStatus(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif

/**
 * @brief Compute the tick value of current WdgM_MainFunction.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-86497
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_MainFunctionTickValueCompu(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Perform the alive supervision of master-satellite mainfunction.
 * @param[in]   appl: Application Id.
 * @param[in]   stateVar: Mainfunction alive state value.
 * @param[in]   configVar: Pointer to current config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-86498
 */
#if (WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_PerformSatelliteMainFunctionAliveSupervision(
    WdgM_ApplicationType                                appl,
    WdgM_MasterSatelliteAliveSupInfoType*               stateVar,
    const WdgM_MasterSatelliteMainFunctionAliveCfgType* configVar);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Confirm satellite mainfunction alive supervision result.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-86499
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmSatelliteMainFunctionAliveResult(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Update satellite mainfunction alive supervision indication.
 * @param[in]   mainFStateIdx: Index of mainfunction in state value.
 * @param[in]   modeMainFCfgIdx: Index of mainfunction in configuration.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-86500
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateSatelliteMainFunctionAliveIndication(uint16 mainFStateIdx, uint16 modeMainFCfgIdx);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief Set Mode of mainfunction.
 * @param[in]   modeId: Index of mode which needs to switch.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-86501
 */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_MainFunctionSetMode(WdgM_ModeType modeId);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u*/

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function initializes the WdgM module when config param is not nullptr
 * and global status is not in Deactivated.
 */
/* PRQA S 1503 ++*/ /*VL_QAC_NoUsedApi*/
void WdgM_Init(const WdgM_ConfigType* ConfigPtr)
/* PRQA S 1503 --*/
{
    WdgM_ArtiApiStart(2, WDGM_INIT_ID, ConfigPtr);

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == ConfigPtr)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_ID, WDGM_E_INV_POINTER);
    }
    else
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    {
#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
        if (E_NOT_OK == WdgM_OffModeConfirmation(ConfigPtr))
        {
            (void)Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_ID, WDGM_E_DISABLE_NOT_ALLOWED);
        }
        else
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/
        {
            if (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_DEACTIVATED)
            {
                if (WdgM_DoInit(ConfigPtr) == E_OK)
                {
                    WdgM_GlobalStatusInfo = WDGM_GLOBAL_STATUS_OK;
                    WdgM_ResetRequest     = FALSE;
                }
            }
        }
    }

    WdgM_ArtiApiStop(1, WDGM_INIT_ID);
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_DoInit(const WdgM_ConfigType* ConfigPtr)
{
    Std_ReturnType          retVal     = E_NOT_OK;
    WdgM_ModeType           initModeId = ConfigPtr->WdgMInitialModeId;
    const WdgM_ModeCfgType* modeCfgPtr = &ConfigPtr->WdgMMode[initModeId];

    /* Initialize the FirstExpired */
    WdgM_FirstExpiredSEID          = WDGM_INIT_FIRST_EXPIRED_SEID;
    WdgM_FirstExpiredSEIDInverse   = WDGM_INIT_FIRST_EXPIRED_SEID;
    WdgM_FirstExpiredSEIDSavedFlag = E_NOT_OK;

    /* Initialize the related SE ram data */
    for (uint16 idx = 0u; idx < WDGM_SE_NUM; idx++)
    {
        WdgM_LocalStatusInfo[idx] = WDGM_LOCAL_STATUS_DEACTIVATED;
    }

    /* Initialize Alive Deadline Logical Supervision ram data within the current mode */
    WdgM_InitLocalSupRunTimeStatus(modeCfgPtr);

    /* Initialize Wdg Trigger within the current Config */
    retVal = WdgM_InitWdgTrigger(modeCfgPtr);

    if (retVal == E_OK)
    {
        /**/
        WdgM_ConfigPtr                                          = ConfigPtr;
        WdgM_ModeCfgPtr                                         = modeCfgPtr;
        WdgM_ModeCfgPtr->WdgMModeInfo->WdgMModeCurrentID        = initModeId;
        WdgM_ModeCfgPtr->WdgMModeInfo->WdgMExpiredSupTolCounter = 0u;
        /*Initialize expired supervision tolerance*/
        /* If above is ok then Set SE local status to OK*/
        WdgM_InitSEStatus(modeCfgPtr);
        WdgM_InitModeMainFunctionState();
    }

    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_InitModeMainFunctionState(void)
{
#if (WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u)

    for (uint8 idx = 0u; idx < WDGM_MAINFUNCTION_NUM; idx++)
    {
        WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter = WDGM_INVALIED_ID;
        WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveIndicationCounter =
            WDGM_INVALIED_ID;
        WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupResult = WDGM_CORRECT;
        WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMFailedAliveCycleRefCounter =
            WDGM_INVALIED_ID;
        WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupState =
            WDGM_LOCAL_STATUS_DEACTIVATED;
    }

    for (uint8 idx = 0u; idx < WdgM_ModeCfgPtr->WdgMMainFunctionCnt; idx++)
    {
        uint16 modeMainfMapIdx = WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[idx].WdgMMainFunctionApplMapIdx;
        if (WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[idx].WdgMMasterSatelliteMainFunctionAliveCfg != NULL_PTR)
        {
            /*Set master mainfunction alive to invalid*/
            WdgM_MainFunctionStateVar[modeMainfMapIdx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter =
                WDGM_INVALIED_ID;
            WdgM_MainFunctionStateVar[modeMainfMapIdx]
                .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveIndicationCounter = WDGM_INVALIED_ID;
            WdgM_MainFunctionStateVar[modeMainfMapIdx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupResult =
                WDGM_CORRECT;
            WdgM_MainFunctionStateVar[modeMainfMapIdx]
                .WdgMMasterSatelliteAliveSupervisionInfo.WdgMFailedAliveCycleRefCounter = 0u;
            WdgM_MainFunctionStateVar[modeMainfMapIdx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupState =
                WDGM_LOCAL_STATUS_OK;
        }
        else
        {
            /*Just set satellite mainfunction alive state to valid*/

            WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter      = 1u;
            WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveIndicationCounter = 0u;
            WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupResult = WDGM_CORRECT;
            WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMFailedAliveCycleRefCounter = 0u;
            WdgM_MainFunctionStateVar[idx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupState =
                WDGM_LOCAL_STATUS_OK;
        }
        /*Init all mainfunction tick*/
        WdgM_MainFunctionStateVar[idx].WdgMMainFunctionTickCounter = 1u;
    }
    WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionExpiredAliveCyclCounter = 0u;
    WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus             = WDGM_GLOBAL_STATUS_OK;
#else
    WdgM_MainFunctionStateVarTick = 1u;
#endif
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function deinitializes the WdgM module when global status is not in Deactivated.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
void WdgM_DeInit(void)
{
    WdgM_ArtiApiStart(1, WDGM_DEINIT_ID);

    if (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_DEINIT_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    }
    else
    {
        WdgM_GlobalStatusInfo = WDGM_GLOBAL_STATUS_DEACTIVATED;
#if (WDGM_RTE_ENABLED == STD_ON)
        (void)WdgM_ConfigPtr->WdgMGlobalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_DEACTIVATED);
#endif /*WDGM_RTE_ENABLED == STD_ON*/
    }

    WdgM_ArtiApiStop(1, WDGM_DEINIT_ID);
} /* PRQA S 1503,1505 --*/

#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * This function gets the WdgM version and can be invoked only when switch Version info api turns on.
 */
#if (WDGM_VERSION_INFO_API == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
void WdgM_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
    WdgM_ArtiApiStart(2, WDGM_GET_VERSION_INFO_ID, VersionInfo);

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == VersionInfo)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_VERSION_INFO_ID, WDGM_E_INV_POINTER);
    }
    else
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    {
        VersionInfo->vendorID         = WDGM_VENDOR_ID;
        VersionInfo->moduleID         = WDGM_MODULE_ID;
        VersionInfo->sw_major_version = WDGM_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = WDGM_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = WDGM_SW_PATCH_VERSION;
    }

    WdgM_ArtiApiStop(1, WDGM_GET_VERSION_INFO_ID);
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_VERSION_INFO_API == STD_ON*/

/* ========================================== internal function definitions ========================================= */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_DoSetMode(WdgM_ModeType Mode)
{
    uint16         triggerIdx;
    Std_ReturnType ret    = E_OK;
    Std_ReturnType retVal = E_NOT_OK;

    /* Only global status is OK or FAILED then execute mode change */
    if ((WDGM_GLOBAL_STATUS_OK == WdgM_GlobalStatusInfo) || (WDGM_GLOBAL_STATUS_FAILED == WdgM_GlobalStatusInfo))
    {
        for (triggerIdx = 0u; triggerIdx < WdgM_ConfigPtr->WdgMMode[Mode].WdgMModeTriggerCnt; triggerIdx++)
        {
            uint8          deviceID = WdgM_ConfigPtr->WdgMMode[Mode].WdgMTriggerRelated[triggerIdx].WdgMWatchdogDevice;
            WdgIf_ModeType deviceMode = WdgM_ConfigPtr->WdgMMode[Mode].WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode;
            ret                       = WdgIf_SetMode(deviceID, deviceMode);
            if (E_NOT_OK == ret)
            {
                ((void)Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_SET_MODE));
                WdgM_GlobalStatusInfo = WDGM_GLOBAL_STATUS_STOPPED;
#if (WDGM_RTE_ENABLED == STD_ON)
                (void)WdgM_ConfigPtr->WdgMGlobalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_STOPPED);
#endif /*WDGM_RTE_ENABLED == STD_ON*/
                retVal = E_NOT_OK;
                break;
            }
        }
        /* If mode trigger mode change is ok then Change the SE ram data */
        if ((Std_ReturnType)E_OK == ret)
        {
            WdgM_LocalStatusChangeWithSetMode((const WdgM_ModeCfgType*)&WdgM_ConfigPtr->WdgMMode[Mode]);
#if (WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u)
            WdgM_MainFunctionSetMode(Mode);
#endif
            WdgM_ModeCfgPtr->WdgMModeInfo->WdgMModeCurrentID        = Mode;
            WdgM_ModeCfgPtr->WdgMModeInfo->WdgMExpiredSupTolCounter = 0u;
            retVal                                                  = E_OK;
        }
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function sets WdgM mode and it will change Wdg driver to corresponding mode.
 * It should be noted that only can set to be off mode with OFFMODE Enabled.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
Std_ReturnType WdgM_SetMode(WdgM_ModeType Mode)
/* PRQA S 1503,1505 --*/
{
    WdgM_ArtiApiStart(2, WDGM_SET_MODE_ID, Mode);

    Std_ReturnType retVal = E_NOT_OK;

    if (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        retVal = E_NOT_OK;
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if ((Std_ReturnType)E_NOT_OK == WdgM_ModeExistenceConfirmation(Mode))
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_PARAM_MODE);
        retVal = E_NOT_OK;
    }
#endif
#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
    else if ((Std_ReturnType)E_NOT_OK == WdgM_OffModeConfirmationForRequestMode(Mode))
    {
        Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_DISABLE_NOT_ALLOWED);
        retVal = E_NOT_OK;
    }
#endif
    else
    {
        if (Mode == WdgM_ModeCfgPtr->WdgMModeInfo->WdgMModeCurrentID)
        {
            retVal = E_OK;
        }
        else
        {
            retVal = WdgM_DoSetMode(Mode);
#if (WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u)
            if (retVal == E_OK)
            {
                for (uint8 idx = 0u; idx < WDGM_MAINFUNCTION_NUM; idx++)
                {
                    WdgM_MainFunctionStateVar[idx].WdgMMainFunctionTickCounter = 1u;
                }
            }
#else
            WdgM_MainFunctionStateVarTick = 1u;
#endif
        }
    }

    WdgM_ArtiApiStop(2, WDGM_SET_MODE_ID, retVal);

    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function gets current WdgM mode only when WdgM module is initialized.
 */
/* PRQA S 1503 ++*/ /*VL_QAC_NoUsedApi*/
Std_ReturnType WdgM_GetMode(WdgM_ModeType* Mode)
/* PRQA S 1503 --*/
{
    WdgM_ArtiApiStart(2, WDGM_GET_MODE_ID, Mode);

    Std_ReturnType retVal = E_NOT_OK;

    if (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_MODE_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        retVal = E_NOT_OK;
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if (NULL_PTR == Mode)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_MODE_ID, WDGM_E_INV_POINTER);
        retVal = E_NOT_OK;
    }
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    else
    {
        *Mode  = WdgM_ModeCfgPtr->WdgMModeInfo->WdgMModeCurrentID;
        retVal = E_OK;
    }

    WdgM_ArtiApiStop(2, WDGM_GET_MODE_ID, retVal);

    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateSupervision(const WdgM_ModeSECfgType* seInfo, uint16 cpIndex)
{
    /* If current Supervised Entity has deadline supervision */
#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
    if (seInfo->WdgMSEDealineCnt > 0u)
    {
        WdgM_DeadlineTimeType curWdgMTime;

        (void)GetCounterValue((CounterType)seInfo->WdgMSEGeneralCfg->WdgMOsCounter, &curWdgMTime);
        curWdgMTime = curWdgMTime * seInfo->WdgMSEGeneralCfg->WdgMOsCounterResolution;
        /*Update Deadline supervision during checkpoint reached*/
        SchM_Enter_WdgM_Exclusive();
        WdgM_UpdateDeadlineSup(cpIndex, seInfo, curWdgMTime);
        SchM_Exit_WdgM_Exclusive();
    }
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (                                                                                            \
    (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON) || (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON) \
    || (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON))

    SchM_Enter_WdgM_Exclusive();
#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
    /* If current Supervised Entity has alive supervision */
    if (seInfo->WdgMSEAliveCnt > 0u)
    {
        /*Update Alive indication counter during checkpoint reached*/
        WdgM_UpdateAICounter(cpIndex, seInfo);
    }
#endif
#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    /* If current Supervised Entity has internal logical supervision */
    if (seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo != NULL_PTR)
    {
        /*Update Internal Logical supervision during checkpoint reached*/
        WdgM_UpdateIntLogSup(cpIndex, seInfo);
    }
#endif
#if (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    /*Update External Logical supervision during checkpoint reached*/
    if (WdgM_ModeCfgPtr->WdgMModeExtLogCnt > 0u)
    {
        WdgM_UpdateExtLogSup(cpIndex, seInfo);
    }
#endif
    SchM_Exit_WdgM_Exclusive();
#endif
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
WDGM_LOCAL void WdgM_DoCheckpointReached(const WdgM_ModeSECfgType* seInfo, WdgM_CheckpointIdType CheckpointID)
{
    uint16 seCpIndex = 0u;

    if ((WDGM_GLOBAL_STATUS_OK == WdgM_GlobalStatusInfo) || (WDGM_GLOBAL_STATUS_FAILED == WdgM_GlobalStatusInfo))
    {
#if (WDGM_PARTITION_ENABLED == STD_ON)
        if (E_OK == WdgM_CheckpointReachedLimitCheck(seInfo->WdgMSEGeneralCfg->WdgMEcucPartitionRef))
#endif
        {
            if (E_OK == WdgM_GetSECheckpointIndex(CheckpointID, seInfo, &seCpIndex))
            {
                WdgM_UpdateSupervision(seInfo, seCpIndex);
            }
        }
    }
}
/* PRQA S 3673 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType
    WdgM_DoParamsCheckforCheckpointReached(WdgM_SupervisedEntityIdType SEID, WdgM_CheckpointIdType CheckpointID)
{
    Std_ReturnType retVal = E_OK;

    if ((CheckpointID + (WdgM_CheckpointIdType)1) > WdgM_SEGeneralCfg[SEID].WdgMSECPCnt)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_ID, WDGM_E_CPID);
        retVal = E_NOT_OK;
    }
    else
    {
        Std_ReturnType retSEActive = E_NOT_OK;
        for (uint8 idx = 0; idx < WdgM_ModeCfgPtr->WdgMModeSECnt; idx++)
        {
            if (WdgM_ModeCfgPtr->WdgMSERelated[idx].WdgMSEGeneralCfg->WdgMSEID == SEID)
            {
                retSEActive = E_OK;
                break;
            }
        }
        if (retSEActive == E_NOT_OK)
        {
            (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_ID, WDGM_E_PARAM_SEID);
            retVal = E_NOT_OK;
        }
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function indicates the WdgM that the Checkpoint has been reported, then it will perform the supervision
 * algorithm to decide the result of Alive, Deadline,Internal Logical and External Logical supervision.
 * It will not confirm the local or global status and only compute the algorithm supervison result.
 */
/* PRQA S 1503 ++*/ /*VL_QAC_NoUsedApi*/
Std_ReturnType WdgM_CheckpointReached(WdgM_SupervisedEntityIdType SEID, WdgM_CheckpointIdType CheckpointID)
/* PRQA S 1503 --*/
{
    WdgM_ArtiApiStart(3, WDGM_CHECKPOINT_REACHED_ID, SEID, CheckpointID);

    uint16         modeSETabIndex = 0u;
    Std_ReturnType retVal         = E_NOT_OK;

    if (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    }
    else
    {
        if (WDGM_LOCAL_STATUS_DEACTIVATED == WdgM_LocalStatusInfo[SEID])
        {
            (void)Det_ReportRuntimeError(
                WDGM_MODULE_ID,
                WDGM_INSTANCE_ID,
                WDGM_CHECKPOINT_REACHED_ID,
                WDGM_E_SEDEACTIVATED);
        }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        retVal = WdgM_DoParamsCheckforCheckpointReached(SEID, CheckpointID);
        if (retVal == E_OK)
#endif
        {
            if (WdgM_GetModeSEIndex(SEID, &modeSETabIndex) == E_OK)
            {
                const WdgM_ModeSECfgType* seInfo =
                    (const WdgM_ModeSECfgType*)&WdgM_ModeCfgPtr->WdgMSERelated[modeSETabIndex];
                WdgM_DoCheckpointReached(seInfo, CheckpointID);
                retVal = E_OK;
            }
        }
    }

    WdgM_ArtiApiStop(2, WDGM_CHECKPOINT_REACHED_ID, retVal);

    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_PARTITION_ENABLED == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_CheckpointReachedLimitCheck(const uint16 ecucPartition)
{
    Std_ReturnType       ret     = E_OK;
    WdgM_ApplicationType partIdx = GetApplicationID();

    if (ecucPartition != partIdx)
    {
        ret = E_NOT_OK;
    }
    return ret;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_PARTITION_ENABLED == STD_ON*/

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function gets the local status of current Supervised Entity. The state indicates the supervision algorithm
 * result and it will As a reference for calculating the global state.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
Std_ReturnType WdgM_GetLocalStatus(WdgM_SupervisedEntityIdType SEID, WdgM_LocalStatusType* Status)
/* PRQA S 1503,1505 --*/
{
    WdgM_ArtiApiStart(3, WDGM_GET_LOCAL_STATUS_ID, SEID, Status);

    Std_ReturnType retVal = E_NOT_OK;

    if (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if (WDGM_SE_NUM <= SEID) /*Check SEID if in range*/
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_ID, WDGM_E_PARAM_SEID);
    }
    else if (NULL_PTR == Status)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_ID, WDGM_E_INV_POINTER);
    }
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    else
    {
        *Status = WdgM_LocalStatusInfo[SEID];
        retVal  = E_OK;
    }

    WdgM_ArtiApiStop(2, WDGM_GET_LOCAL_STATUS_ID, retVal);

    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function gets the global status of WdgM and it can be invoked only when WdgM is initliazed. The global state is
 * calculated from all local states within current running mode and it has five states, respectively are DEACTIVATED,
 * OK, FAILED, EXPIRED and STOPPED.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
Std_ReturnType WdgM_GetGlobalStatus(WdgM_GlobalStatusType* Status)
/* PRQA S 1503,1505 --*/
{
    WdgM_ArtiApiStart(2, WDGM_GET_GLOBAL_STATUS_ID, Status);

    Std_ReturnType retVal = E_NOT_OK;

    if (WDGM_GLOBAL_STATUS_DEACTIVATED == WdgM_GlobalStatusInfo)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_GLOBAL_STATUS_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if (NULL_PTR == Status)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_GLOBAL_STATUS_ID, WDGM_E_INV_POINTER);
    }
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    else
    {
        *Status = WdgM_GlobalStatusInfo;
        retVal  = E_OK;
    }

    WdgM_ArtiApiStop(2, WDGM_GET_GLOBAL_STATUS_ID, retVal);

    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function will call Mcu_PerfomReset to immediately reset. It should be noted that only if Mcu supports above
 * capability and open the switch ImmediatelyReset can use it.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
void WdgM_PerformReset(void)
/* PRQA S 1503,1505 --*/
{
    WdgM_ArtiApiStart(1, WDGM_PERFORM_RESET_ID);

    uint8 triggerIndex;
    if (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_PERFORM_RESET_ID, WDGM_E_UNINIT);
#endif
    }
    else
    {
        SchM_Enter_WdgM_Exclusive();
        for (triggerIndex = 0u; triggerIndex < WdgM_ModeCfgPtr->WdgMModeTriggerCnt; triggerIndex++)
        {
            if (WDGIF_OFF_MODE != WdgM_ModeCfgPtr->WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
            {
                WdgIf_SetTriggerCondition(
                    WdgM_ModeCfgPtr->WdgMTriggerRelated[triggerIndex].WdgMWatchdogDevice,
                    WDGM_TRIGGER_CONDITION_STOP);
            }
        }
        WdgM_ResetRequest = TRUE;
        SchM_Exit_WdgM_Exclusive();
    }

    WdgM_ArtiApiStop(1, WDGM_PERFORM_RESET_ID);
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/**
 * This function gets the first expired SE's ID of WdgM. The double inverse value will not be clear after Wdg reset and
 * will be initliazed by api WdgM_Init().
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
Std_ReturnType WdgM_GetFirstExpiredSEID(WdgM_SupervisedEntityIdType* SEID)
/* PRQA S 1503,1505 --*/
{
    WdgM_ArtiApiStart(2, WDGM_GET_FIRST_EXPIRED_SEID_ID, SEID);

    Std_ReturnType retVal = E_NOT_OK;

    if (NULL_PTR == SEID)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_FIRST_EXPIRED_SEID_ID, WDGM_E_INV_POINTER);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    }
    else
    {
        {
            uint16 firstExpiredSEID    = WdgM_FirstExpiredSEID;
            uint16 firstExpiredSEIDInv = WdgM_FirstExpiredSEIDInverse;
            if (WDGM_VALIED_FIRST_EXPIRED_SEID_SUM == (firstExpiredSEID + firstExpiredSEIDInv))
            {
                *SEID  = firstExpiredSEID;
                retVal = E_OK;
            }
            else
            {
                *SEID  = 0u;
                retVal = E_NOT_OK;
            }
        }
    }

    WdgM_ArtiApiStop(2, WDGM_GET_FIRST_EXPIRED_SEID_ID, retVal);

    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateAICounter(uint16 cpIndex, const WdgM_ModeSECfgType* seInfo)
{
    if (STD_ON == WdgM_GetWordBitState(seInfo->WdgMCheckpointRelated[cpIndex].WdgMSupSupportGroup, ALIVE_SUPPORT))
    {
        for (uint8 index = 0u; index < seInfo->WdgMSEAliveCnt; index++)
        {
            if (seInfo->WdgMAliveSupCfg[index].WdgMAliveSupID
                == seInfo->WdgMCheckpointRelated[cpIndex].WdgMCPAliveSupID)
            {
                seInfo->WdgMAliveSupCfg[index].WdgMAliveSupervisionInfo->WdgMAliveIndicationCounter++;
            }
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmAliveSupResultEachSE(const WdgM_ModeSECfgType* seInfo)
{
    if ((uint16)0u < seInfo->WdgMSEAliveCnt)
    {
        /*update certain AliveSup result*/
        for (uint8 idx = 0u; idx < seInfo->WdgMSEAliveCnt; idx++)
        {
            uint16                 aliveRefCycle   = seInfo->WdgMAliveSupCfg[idx].WdgMSupRefCycle;
            WdgM_AliveSupInfoType* currentAliveTab = seInfo->WdgMAliveSupCfg[idx].WdgMAliveSupervisionInfo;

            if (E_OK == WdgM_ConfirmAliveSupResult(&currentAliveTab->WdgMAliveCycleCounter, aliveRefCycle))
            {
                /*Reach to the RefCycle, determine the related Alive Sup result*/
                uint8  minMargin        = seInfo->WdgMAliveSupCfg[idx].WdgMMinMargin;
                uint8  maxMargin        = seInfo->WdgMAliveSupCfg[idx].WdgMMaxMargin;
                uint16 exceptIndication = seInfo->WdgMAliveSupCfg[idx].WdgMExpectedAliveSupIndication;
                /*f(EAI,SRC)= SRC - EAI */
                sint32 aliveSupTempConfigCnt = (sint32)(aliveRefCycle) - (sint32)(exceptIndication);
                /*n(AI)-n(SC)*/
                sint32 aliveSupTempCnt = (sint32)(currentAliveTab->WdgMAliveIndicationCounter)
                                         - (sint32)(currentAliveTab->WdgMAliveCycleCounter);
                sint32 aliveSupTempResult = aliveSupTempConfigCnt + aliveSupTempCnt;

                currentAliveTab->WdgMAliveCycleCounter      = 0u;
                currentAliveTab->WdgMAliveIndicationCounter = 0u;

                if (((aliveSupTempResult + (sint32)minMargin) >= 0L) && (aliveSupTempResult <= (sint32)maxMargin))
                {
                    currentAliveTab->WdgMAliveSupResult = WDGM_CORRECT;
                }
                else
                {
                    currentAliveTab->WdgMAliveSupResult = WDGM_INCORRECT;
                }
            }
        }

        /*update the AliveSup result of Certain SE including several AliveSup */
        for (uint8 idx = 0u; idx < seInfo->WdgMSEAliveCnt; idx++)
        {
            const WdgM_AliveSupInfoType* currentAliveTab = seInfo->WdgMAliveSupCfg[idx].WdgMAliveSupervisionInfo;
            if (WDGM_INCORRECT == currentAliveTab->WdgMAliveSupResult)
            {
                seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_ALIVE_SUP] = WDGM_INCORRECT;
                break;
            }
            else
            {
                seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_ALIVE_SUP] = WDGM_CORRECT;
            }
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_ConfirmAliveSupResult(uint16* aliveCycleCounter, uint16 refCycle)
{
    Std_ReturnType retVal;

    (*aliveCycleCounter)++;
    if (*aliveCycleCounter >= refCycle)
    {
        /*The AliveSup Result base on the attachment*/
        retVal = E_OK;
    }
    else
    {
        retVal = E_NOT_OK;
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_WdgM_STMIF */
WDGM_LOCAL void WdgM_DoUpdateDeadlineSup(
    const WdgM_CPSupportDeadLineInfoType* cpSupportDeadlineInfo,
    const WdgM_ModeSECfgType*             seInfo,
    WdgM_DeadlineTimeType                 curWdgMTime)
{
    WdgM_DeadlineTimeType deadlineTimeGap = 0u;

    if (STD_ON == WdgM_GetWordBitState(cpSupportDeadlineInfo->WdgMCPSupportDeadlineGroup, CP_DEADLINE_SUPPORT_START))
    {
        for (uint16 idx = 0u; idx < seInfo->WdgMSEDealineCnt; idx++)
        {
            if (seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupID == cpSupportDeadlineInfo->WdgMDeadlineSupID)
            {
                seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart = curWdgMTime;
            }
        }
    }

    /**Deadline Sup Stop**/
    if (STD_ON == WdgM_GetWordBitState(cpSupportDeadlineInfo->WdgMCPSupportDeadlineGroup, CP_DEADLINE_SUPPORT_STOP))
    {
        for (uint16 idx = 0u; idx < seInfo->WdgMSEDealineCnt; idx++)
        {
            if (seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupID == cpSupportDeadlineInfo->WdgMDeadlineSupID)
            {
                if ((WdgM_DeadlineTimeType)0uL
                    != seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart)
                {
                    uint32 resolution = seInfo->WdgMSEGeneralCfg->WdgMOsCounterResolution;
                    if (curWdgMTime >= seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart)
                    {
                        deadlineTimeGap =
                            (curWdgMTime
                             - seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart)
                            * resolution;
                    }
                    else
                    {
                        uint32 maxTickValue = seInfo->WdgMSEGeneralCfg->WdgMOsCounterMaxValue;
                        deadlineTimeGap =
                            1uL + curWdgMTime
                            + ((maxTickValue * resolution)
                               - seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart);
                    }
                    WdgM_UpdateDeadlineSupResult(deadlineTimeGap, &seInfo->WdgMDeadlineSupCfg[idx]);
                    seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart = 0u;
                }
                else
                {
                    /* No action, if TimeStamp is 0*/
                }
            }
        }
    }
}
/* PRQA S 6030 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_WdgM_STMIF */
WDGM_LOCAL void
    WdgM_UpdateDeadlineSup(uint16 cpIndex, const WdgM_ModeSECfgType* seInfo, WdgM_DeadlineTimeType curWdgMTime)
{
    uint16                        cpSupportDeadlineIndex;
    const WdgM_CheckpointCfgType* checkpointCfg =
        (const WdgM_CheckpointCfgType*)&seInfo->WdgMCheckpointRelated[cpIndex];

    if (WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, DEADLINE_SUPPORT) == STD_ON)
    {
        /******One Checkpoint may map several Deadline Sup********/
        for (cpSupportDeadlineIndex = 0u; cpSupportDeadlineIndex < checkpointCfg->WdgMCPSupportDeadLineCnt;
             cpSupportDeadlineIndex++)
        {
            const WdgM_CPSupportDeadLineInfoType* cpSupportDeadlineInfo =
                (const WdgM_CPSupportDeadLineInfoType*)&checkpointCfg
                    ->WdgMCPSupportDeadLineInfo[cpSupportDeadlineIndex];
            WdgM_DeadlineSupIDType currentDeadlineSupID = cpSupportDeadlineInfo->WdgMDeadlineSupID;
            for (uint16 idx = 0u; idx < seInfo->WdgMSEDealineCnt; idx++)
            {
                if (seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupID == currentDeadlineSupID)
                {
                    if (WDGM_INCORRECT
                        == seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMDeadlineSupResult)
                    {
                        break;
                    }
                    else
                    {
                        /* Deadline Sup Start */
                        WdgM_DoUpdateDeadlineSup(cpSupportDeadlineInfo, seInfo, curWdgMTime);
                    }
                }
            }
        }
    }
}
/* PRQA S 6030 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void
    WdgM_UpdateDeadlineSupResult(WdgM_DeadlineTimeType timeGap, const WdgM_DeadlineSupCfgType* deadlineSupCfg)
{
    if ((((deadlineSupCfg->WdgMDeadlineMin) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
         <= (WdgM_DeadlineTimeFloatType)timeGap)
        && (((deadlineSupCfg->WdgMDeadlineMax) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
            >= (WdgM_DeadlineTimeFloatType)timeGap))
    {
        deadlineSupCfg->WdgMDeadlineSupervisionInfo->WdgMDeadlineSupResult = WDGM_CORRECT;
    }
    else
    {
        deadlineSupCfg->WdgMDeadlineSupervisionInfo->WdgMDeadlineSupResult = WDGM_INCORRECT;
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_DEADLINE_TIMEOUT_DETECT_ENABLE == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_PerformDeadlineTimeoutDetection(const WdgM_ModeSECfgType* seInfo)
{
    for (uint16 idx = 0u; idx < seInfo->WdgMSEDealineCnt; idx++)
    {
        uint32 tempStartTime = seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart;
        if (0uL != tempStartTime)
        {
            WdgM_DeadlineTimeType curWdgMTime     = tempStartTime;
            WdgM_DeadlineTimeType deadlineTimeGap = 0u;
            (void)GetElapsedValue(seInfo->WdgMSEGeneralCfg->WdgMOsCounter, &curWdgMTime, &deadlineTimeGap);
            deadlineTimeGap = deadlineTimeGap * seInfo->WdgMSEGeneralCfg->WdgMOsCounterResolution;
            if (((seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineMax) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
                < ((WdgM_DeadlineTimeFloatType)deadlineTimeGap))
            {
                seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMDeadlineSupResult = WDGM_INCORRECT;
                seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart    = 0u;
            }
            else
            {
                /*MISRA C*/
            }
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmDeadlineSupResultEachSE(const WdgM_ModeSECfgType* seInfo)
{
    /*If Alive Sup is false, may jump the determination of the Deadline Sup*/
    if (0u < seInfo->WdgMSEDealineCnt)
    {
        for (uint16 idx = 0u; idx < seInfo->WdgMSEDealineCnt; idx++)
        {
            if (seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMDeadlineSupResult == WDGM_INCORRECT)
            {
                /*one Deadline Sup incorrect*/
                seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_INCORRECT;
                break;
            }
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateIntLogSup(uint16 cpIndex, const WdgM_ModeSECfgType* seInfo)
{
    const WdgM_CheckpointCfgType* checkpointCfg =
        (const WdgM_CheckpointCfgType*)&seInfo->WdgMCheckpointRelated[cpIndex];

    if ((WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, INT_LOG_SUPPORT) == STD_ON)
        && (WDGM_CORRECT == seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogSupResult)) /*Interlog Sup*/
    {
        if (WDGM_AF_FALSE == seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogActiveFlag)
        {
            if (WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, LOG_SUP_SUPPORT_INIT) == STD_ON)
            {
                seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLastReachedCPID = checkpointCfg->WdgMCheckpointID;
                seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogActiveFlag   = WDGM_AF_TRUE;
                seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogSupResult    = WDGM_CORRECT;
            }
            else
            {
                seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogSupResult = WDGM_INCORRECT;
            }
        }
        else
        {
            WdgM_SupResultType tempTransitionResult = WDGM_INCORRECT;
            uint16             transitionMax        = seInfo->WdgMSEGeneralCfg->WdgMIntLogTransitionCnt;

            for (uint16 transitionIndex = 0u; transitionIndex < transitionMax; transitionIndex++)
            {
                WdgM_CheckpointIdType intLastReachedCPID =
                    seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLastReachedCPID;
                WdgM_CheckpointIdType intCurReachedCPID =
                    seInfo->WdgMSEGeneralCfg->WdgMIntLogTransition[transitionIndex].WdgMTransitionSourCPID;
                WdgM_CheckpointIdType intDestReachedCPID =
                    seInfo->WdgMSEGeneralCfg->WdgMIntLogTransition[transitionIndex].WdgMTransitionDestCPID;
                if ((intLastReachedCPID == intCurReachedCPID)
                    && (checkpointCfg->WdgMCheckpointID == intDestReachedCPID))
                {
                    tempTransitionResult = WDGM_CORRECT;
                    break;
                }
                else
                {
                    tempTransitionResult = WDGM_INCORRECT;
                }
            }
            if (WDGM_CORRECT == tempTransitionResult)
            {
                seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogSupResult    = WDGM_CORRECT;
                seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLastReachedCPID = checkpointCfg->WdgMCheckpointID;
                if (STD_ON == WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, LOG_SUP_SUPPORT_FINAL))
                {
                    seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogActiveFlag   = WDGM_AF_FALSE;
                    seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
                }
            }
            else
            {
                seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogSupResult = WDGM_INCORRECT;
            }
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

/*Determine the Alive Sup Result*/
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmIntLogSupResultEachSE(const WdgM_ModeSECfgType* seInfo)
{
    if (seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo != NULL_PTR)
    {
        /*If Sup above is false, may jump the determination of the Internal log Sup*/
        seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_INTLOG_SUP] =
            seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogSupResult;
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateExtLogSupResult(
    const WdgM_ExtLogSupCfgType*      currentExtLogSup,
    const WdgM_CheckpointIdType       CheckpointID,
    const WdgM_SupervisedEntityIdType SEID)
{
    uint16 transitionIndex;

    for (transitionIndex = 0u; transitionIndex < currentExtLogSup->WdgMExtTransitionCnt; transitionIndex++)
    {
        if ((currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLastReachedCPID
             == currentExtLogSup->WdgMExtTransition[transitionIndex].WdgMTransitionSourCPID)
            && (currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLastReachedSEID
                == currentExtLogSup->WdgMExtTransition[transitionIndex].WdgMTransitionSourSEID))
        {
            if ((CheckpointID == currentExtLogSup->WdgMExtTransition[transitionIndex].WdgMTransitionDestCPID)
                && (SEID == currentExtLogSup->WdgMExtTransition[transitionIndex].WdgMTransitionDestSEID))
            {
                currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogSupResult = WDGM_CORRECT;
                break;
            }
            else
            {
                currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogSupResult = WDGM_INCORRECT;
            }
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_WdgM_STMIF */
WDGM_LOCAL void WdgM_UpdateExtLogSup(uint16 cpIndex, const WdgM_ModeSECfgType* seInfo)
/* PRQA S 6030 -- */
{
    const WdgM_CheckpointCfgType* checkpointCfg =
        (const WdgM_CheckpointCfgType*)&seInfo->WdgMCheckpointRelated[cpIndex];

    if (WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, EXT_LOG_SUPPORT) == STD_ON)
    {
        uint16                       extLogSupId      = checkpointCfg->WdgMExtLogSupID;
        const WdgM_ExtLogSupCfgType* currentExtLogSup = &WdgM_ModeCfgPtr->WdgMExtLogSupRelated[extLogSupId];
        if (currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogSupResult == WDGM_CORRECT)
        {
            if (currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogActiveFlag == WDGM_AF_FALSE)
            {
                if (STD_ON == WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, LOG_SUP_SUPPORT_INIT))
                {
                    currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLastReachedCPID =
                        checkpointCfg->WdgMCheckpointID;
                    currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLastReachedSEID =
                        seInfo->WdgMSEGeneralCfg->WdgMSEID;
                    currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogActiveFlag = WDGM_AF_TRUE;
                    currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogSupResult  = WDGM_CORRECT;
                }
                else
                {
                    currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogSupResult = WDGM_INCORRECT;
                }
            }
            else
            {
                WdgM_UpdateExtLogSupResult(
                    currentExtLogSup,
                    checkpointCfg->WdgMCheckpointID,
                    seInfo->WdgMSEGeneralCfg->WdgMSEID);

                if (WDGM_CORRECT == currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogSupResult)
                {
                    currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLastReachedCPID =
                        checkpointCfg->WdgMCheckpointID;
                    currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLastReachedSEID =
                        seInfo->WdgMSEGeneralCfg->WdgMSEID;

                    if (STD_ON == WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, LOG_SUP_SUPPORT_FINAL))
                    {
                        currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLogActiveFlag   = WDGM_AF_FALSE;
                        currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLastReachedSEID = WDGM_NO_SE_REACH;
                        currentExtLogSup->WdgMExtLogSupervisionInfo->WdgMExtLastReachedCPID = WDGM_NO_CP_REACH;
                    }
                }
            }
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmExtLogSupResultEachSE(void)
{
    for (uint16 idx = 0u; idx < WdgM_ModeCfgPtr->WdgMModeSECnt; idx++) /*SE quantity*/
    {
        WdgM_SupervisedEntityIdType SEID      = WdgM_ModeCfgPtr->WdgMSERelated[idx].WdgMSEGeneralCfg->WdgMSEID;
        uint16                      extLogIdx = 0u;
        for (extLogIdx = 0u; extLogIdx < WdgM_ModeCfgPtr->WdgMModeExtLogCnt; extLogIdx++) /*ExtLogSup quantity*/
        {
            uint16 seRelatedIdxMax = WdgM_ModeCfgPtr->WdgMExtLogSupRelated[extLogIdx].WdgMSEIdRelatedCnt;
            /*Related SE quantity of each ExtLog Sup*/
            uint16 seRelatedIdx = 0u;
            for (seRelatedIdx = 0u; seRelatedIdx < seRelatedIdxMax; seRelatedIdx++)
            {
                WdgM_LogSupIDType extLogId = WdgM_ModeCfgPtr->WdgMExtLogSupRelated[extLogIdx].WdgMExtLogSupID;
                /*Find the SE related to the ExtLog Sup via SEID*/
                if ((SEID == WdgM_ModeCfgPtr->WdgMExtLogSupRelated[extLogIdx].WdgMSEIdImpacted[seRelatedIdx])
                    && (WDGM_INCORRECT
                        == WdgM_ModeCfgPtr->WdgMExtLogSupRelated[extLogId]
                               .WdgMExtLogSupervisionInfo->WdgMExtLogSupResult))
                {
                    /*One ExtLog Sup Error in this SE*/
                    break;
                }
            }
            if (seRelatedIdxMax > seRelatedIdx)
            {
                break;
            }
        }
        if (WdgM_ModeCfgPtr->WdgMModeExtLogCnt > extLogIdx)
        {
            WdgM_ModeCfgPtr->WdgMSERelated[idx].WdgMSEInfo->WdgMEachSupResult[WDGM_EXTLOG_SUP] = WDGM_INCORRECT;
        }
        else
        {
            WdgM_ModeCfgPtr->WdgMSERelated[idx].WdgMSEInfo->WdgMEachSupResult[WDGM_EXTLOG_SUP] = WDGM_CORRECT;
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 1503,1505,1532,6030 ++ */ /* VL_QAC_NoUsedApi,VL_WdgM_1505,VL_QAC_OneFunRef,VL_MTR_WdgM_STMIF */
void WdgM_MainFunction(void)
{
    WdgM_ArtiApiStart(1, WDGM_MAINFUNCTION_ID);

    if (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_MAINFUNCTION_ID, WDGM_E_UNINIT);
#endif
    }
    else
    {
#if (WDGM_PARTITION_ENABLED == STD_ON)
        WdgM_ApplicationType partIdx = GetApplicationID();
#endif
#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
        WdgM_SetDemEventStatus();
#endif
        if (WdgM_MainFunctionTickValueCompu() == E_OK)
        {
#if (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
            /*Confirm the External Log Sup status of each SE*/
            WdgM_ConfirmExtLogSupResultEachSE();
#endif
            for (uint16 seIdx = 0u; seIdx < WdgM_ModeCfgPtr->WdgMModeSECnt; seIdx++)
            {
                const WdgM_ModeSECfgType* seInfo = &WdgM_ModeCfgPtr->WdgMSERelated[seIdx];
                uint16                    SEID   = seInfo->WdgMSEGeneralCfg->WdgMSEID;
#if (WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u)
                if (partIdx == seInfo->WdgMSEGeneralCfg->WdgMEcucPartitionRef)
#endif
                {
                    if ((WDGM_LOCAL_STATUS_OK == WdgM_LocalStatusInfo[SEID])
                        || (WDGM_LOCAL_STATUS_FAILED == WdgM_LocalStatusInfo[SEID]))
                    {
                        /* Confirm the supervision result of current se include alive, deadline, internal */
                        WdgM_ConfirmSupervisionResult(seInfo);
                    }
                }
            }
#if (WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u)
            if (partIdx == WdgM_ConfigPtr->WdgMModeGlobalMainFunctionPartitionRef)
            {
                WdgM_ConfirmSatelliteMainFunctionAliveResult();
#else
            {
#endif
                /*Determine the Global Sup status*/
                WdgM_ConfirmGlobalStatus();
                if (!WdgM_ResetRequest)
                {
                    /*Set watchdog driver according to global status*/
                    WdgM_SetTrigger();
                }
            }
        }
    }

    WdgM_ArtiApiStop(1, WDGM_MAINFUNCTION_ID);
}
/* PRQA S 1503,1505,1532,6030 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmSupervisionResult(const WdgM_ModeSECfgType* seInfo)
{
#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
    /* Determine the Alive Sup Result*/
    WdgM_ConfirmAliveSupResultEachSE(seInfo);
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
#if (WDGM_DEADLINE_TIMEOUT_DETECT_ENABLE == STD_ON)
    /* Deadline Timeout Detection */
    WdgM_PerformDeadlineTimeoutDetection(seInfo);
#endif
    /* Determine the Alive Sup Result*/
    WdgM_ConfirmDeadlineSupResultEachSE(seInfo);
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    /* Determine the Internal Logical Sup Result*/
    WdgM_ConfirmIntLogSupResultEachSE(seInfo);
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

    /* Determine the Local Sup status of each SE*/
    WdgM_ConfirmSELocalStatus(seInfo);
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_ConfirmSELocalStatusHandleOK(const WdgM_ModeSECfgType* seInfo)
{
    Std_ReturnType ret = E_OK;

    if ((WDGM_CORRECT == seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_INTLOG_SUP])
        && (WDGM_CORRECT == seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_EXTLOG_SUP])
        && (WDGM_CORRECT == seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_DEADLINE_SUP]))
    {
#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
        if (WDGM_INCORRECT == seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_ALIVE_SUP])
        {
            if (0u < seInfo->WdgMFailedAliveSupervisionRefCycleTol)
            {
                WdgM_SupervisedEntityIdType SEID = seInfo->WdgMSEGeneralCfg->WdgMSEID;
                seInfo->WdgMSEInfo->WdgMFailedAliveSupCounter++;
                WdgM_LocalStatusInfo[SEID] = WDGM_LOCAL_STATUS_FAILED;
#if (WDGM_RTE_ENABLED == STD_ON)
                (void)seInfo->WdgMSEGeneralCfg->WdgMLocalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_FAILED);
#endif
                /*DD_2_038*/
            }
            else
            {
                ret = E_NOT_OK;
            }
        }
#endif
    }
    else
    {
        ret = E_NOT_OK;
    }

    return ret;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_ConfirmSELocalStatusHandleFAILED(const WdgM_ModeSECfgType* seInfo)
{
    Std_ReturnType ret = E_OK;

    if ((WDGM_INCORRECT == seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_INTLOG_SUP])
        || (WDGM_INCORRECT == seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_EXTLOG_SUP])
        || (WDGM_INCORRECT == seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_DEADLINE_SUP]))
    {
        /*Failed  to Expired*/
        ret = E_NOT_OK;
    }
    else if (WDGM_INCORRECT == seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_ALIVE_SUP])
    {
        seInfo->WdgMSEInfo->WdgMFailedAliveSupCounter++;
        if (seInfo->WdgMFailedAliveSupervisionRefCycleTol < seInfo->WdgMSEInfo->WdgMFailedAliveSupCounter)
        {
            ret = E_NOT_OK;
        }
    }
    else
    {
        if (1u >= seInfo->WdgMSEInfo->WdgMFailedAliveSupCounter)
        {
            seInfo->WdgMSEInfo->WdgMFailedAliveSupCounter            = 0u;
            WdgM_LocalStatusInfo[seInfo->WdgMSEGeneralCfg->WdgMSEID] = WDGM_LOCAL_STATUS_OK;
#if (WDGM_RTE_ENABLED == STD_ON)
            (void)seInfo->WdgMSEGeneralCfg->WdgMLocalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_OK);
#endif
        }
        else
        {
            /* Reamin the Failed, ErrorCnt--*/
            seInfo->WdgMSEInfo->WdgMFailedAliveSupCounter--;
        }
    }
    return ret;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ConfirmSELocalStatus(const WdgM_ModeSECfgType* seInfo)
{
    Std_ReturnType              ret = E_OK;
    WdgM_SupervisedEntityIdType SEID;

    SEID = seInfo->WdgMSEGeneralCfg->WdgMSEID;
    if (WDGM_LOCAL_STATUS_OK == WdgM_LocalStatusInfo[SEID])
    {
        ret = WdgM_ConfirmSELocalStatusHandleOK(seInfo);
    }
#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
    else if (WDGM_LOCAL_STATUS_FAILED == WdgM_LocalStatusInfo[SEID])
    {
        ret = WdgM_ConfirmSELocalStatusHandleFAILED(seInfo);
    }
#endif
    else
    {
        /*MISRA-C*/
    }
    if ((Std_ReturnType)E_NOT_OK == ret)
    {
        WdgM_LocalStatusInfo[SEID] = WDGM_LOCAL_STATUS_EXPIRED;
#if (WDGM_RTE_ENABLED == STD_ON)
        (void)seInfo->WdgMSEGeneralCfg->WdgMLocalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_EXPIRED);
#endif

        /* Write First Expired SEID to non-initialized RAM  */
        if (WdgM_FirstExpiredSEIDSavedFlag == E_NOT_OK)
        {
            SchM_Enter_WdgM_Exclusive();
            /* Set first expired SEID and inv SEID*/
            WdgM_FirstExpiredSEID          = SEID;
            WdgM_FirstExpiredSEIDInverse   = ~SEID;
            WdgM_FirstExpiredSEIDSavedFlag = E_OK;
            SchM_Exit_WdgM_Exclusive();
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_WdgM_STMIF */
WDGM_LOCAL void WdgM_ConfirmGlobalStatus(void)
/* PRQA S 6030 -- */
{
    WdgM_SupervisedEntityIdType SEID;
    uint16                      idx;
    Std_ReturnType              seExpiredFlag = E_NOT_OK;
    Std_ReturnType              seFailedFlag  = E_NOT_OK;

    if ((WdgM_GlobalStatusInfo != WDGM_GLOBAL_STATUS_STOPPED)
        && (WdgM_GlobalStatusInfo != WDGM_GLOBAL_STATUS_DEACTIVATED))
    {
        for (idx = 0u; idx < WdgM_ModeCfgPtr->WdgMModeSECnt; idx++)
        {
            SEID = WdgM_ModeCfgPtr->WdgMSERelated[idx].WdgMSEGeneralCfg->WdgMSEID;
            if (WDGM_LOCAL_STATUS_FAILED == WdgM_LocalStatusInfo[SEID])
            {
                seFailedFlag = E_OK;
            }
            if (WDGM_LOCAL_STATUS_EXPIRED == WdgM_LocalStatusInfo[SEID])
            {
                seExpiredFlag = E_OK;
                break;
            }
        }
        if ((E_OK == seExpiredFlag) || (WdgM_GlobalStatusInfo == WDGM_GLOBAL_STATUS_EXPIRED))
        {
            WdgM_ModeCfgPtr->WdgMModeInfo->WdgMExpiredSupTolCounter++;
            if ((WdgM_ModeCfgPtr->WdgMModeInfo->WdgMExpiredSupTolCounter
                 > WdgM_ModeCfgPtr->WdgMExpiredSupervisionCycleTol))
            {
                WdgM_GlobalStatusInfo                                   = WDGM_GLOBAL_STATUS_STOPPED;
                WdgM_ModeCfgPtr->WdgMModeInfo->WdgMExpiredSupTolCounter = 0u;
#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
                WdgM_DemSetFlag = WDGM_DEM_REPORT_FAILED;
#endif
#if (WDGM_RTE_ENABLED == STD_ON)
                (void)WdgM_ConfigPtr->WdgMGlobalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_STOPPED);
#endif
#if (WDGM_IMMEDIATE_RESET == STD_ON)
                WdgM_StopStatusErrorHanding();
#endif
            }
            else
            {
                WdgM_GlobalStatusInfo = WDGM_GLOBAL_STATUS_EXPIRED;
#if (WDGM_RTE_ENABLED == STD_ON)
                (void)WdgM_ConfigPtr->WdgMGlobalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_EXPIRED);
#endif
            }
        }
        else if ((Std_ReturnType)E_OK == seFailedFlag)
        {
            WdgM_GlobalStatusInfo = WDGM_GLOBAL_STATUS_FAILED;
#if (WDGM_RTE_ENABLED == STD_ON)
            (void)WdgM_ConfigPtr->WdgMGlobalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_FAILED);
#endif
        }
        else
        {
#if (WDGM_RTE_ENABLED == STD_ON)
            if (WdgM_GlobalStatusInfo != WDGM_GLOBAL_STATUS_OK)
            {
                (void)WdgM_ConfigPtr->WdgMGlobalStatusNotification(RTE_MODE_WdgM_Mode_SUPERVISION_OK);
            }
#endif
            WdgM_GlobalStatusInfo = WDGM_GLOBAL_STATUS_OK;
        }
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_IMMEDIATE_RESET == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 2987 ++ */ /* VL_WdgM_2987 */
WDGM_LOCAL void WdgM_StopStatusErrorHanding(void)
{
    /*ShutdownOS shall Deinit the RAM for OS, this shall forbid the fail of startOS*/
    SchM_Enter_WdgM_Exclusive();
    (void)Mcu_PerformReset();
    SchM_Exit_WdgM_Exclusive();
    /* no notification to the application via the RTE */
}
/* PRQA S 2987 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_IMMEDIATE_RESET == STD_ON*/

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 3219,2962 ++ */ /* VL_WdgM_3219,VL_WdgM_2962 */
WDGM_LOCAL void WdgM_SetTrigger(void)
{
    uint8 triggerIdx;

    if ((WDGM_GLOBAL_STATUS_OK == WdgM_GlobalStatusInfo) || (WDGM_GLOBAL_STATUS_FAILED == WdgM_GlobalStatusInfo)
        || (WDGM_GLOBAL_STATUS_EXPIRED == WdgM_GlobalStatusInfo))
    {
        for (triggerIdx = 0u; triggerIdx < WdgM_ModeCfgPtr->WdgMModeTriggerCnt; triggerIdx++)
        {
            if (WDGIF_OFF_MODE != WdgM_ModeCfgPtr->WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode)
            {
                WdgIf_SetTriggerCondition(
                    WdgM_ModeCfgPtr->WdgMTriggerRelated[triggerIdx].WdgMWatchdogDevice,
                    WdgM_ModeCfgPtr->WdgMTriggerRelated[triggerIdx].WdgMTriggerConditionValue);
            }
        }
    }
    else if (WDGM_GLOBAL_STATUS_STOPPED == WdgM_GlobalStatusInfo)
    {
        for (triggerIdx = 0u; triggerIdx < WdgM_ModeCfgPtr->WdgMModeTriggerCnt; triggerIdx++)
        {
            if (WDGIF_OFF_MODE != WdgM_ModeCfgPtr->WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode)
            {
                WdgIf_SetTriggerCondition(
                    WdgM_ModeCfgPtr->WdgMTriggerRelated[triggerIdx].WdgMWatchdogDevice,
                    WDGM_TRIGGER_CONDITION_STOP);
            }
        }
    }
    else
    {
        /* Do nothing*/
    }
}
/* PRQA S 3219,2962 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType
    WdgM_GetSECheckpointIndex(WdgM_CheckpointIdType CPID, const WdgM_ModeSECfgType* seInfo, uint16* cpTabIndex)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint16         cpIndex;

    for (cpIndex = 0u; cpIndex < seInfo->WdgMSEGeneralCfg->WdgMSECPCnt; cpIndex++)
    {
        if (seInfo->WdgMCheckpointRelated[cpIndex].WdgMCheckpointID == CPID)
        {
            *cpTabIndex = cpIndex;
            retVal      = E_OK;
            break;
        }
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_InitLocalSupRunTimeStatus(const WdgM_ModeCfgType* modeCfgPtr)
{
    for (uint16 seIdx = 0u; seIdx < modeCfgPtr->WdgMModeSECnt; seIdx++)
    {
        const WdgM_ModeSECfgType* seInfo = &modeCfgPtr->WdgMSERelated[seIdx];
        WdgM_ClearModeSEStatus(seInfo);
        seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_ALIVE_SUP]    = WDGM_CORRECT;
        seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_CORRECT;
        seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_INTLOG_SUP]   = WDGM_CORRECT;
        seInfo->WdgMSEInfo->WdgMEachSupResult[WDGM_EXTLOG_SUP]   = WDGM_CORRECT;
        seInfo->WdgMSEInfo->WdgMFailedAliveSupCounter            = 0u;
    }

#if (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON)

    for (uint8 extIdx = 0u; extIdx < modeCfgPtr->WdgMModeExtLogCnt; extIdx++)
    {
        modeCfgPtr->WdgMExtLogSupRelated[extIdx].WdgMExtLogSupervisionInfo->WdgMExtLastReachedCPID = WDGM_NO_CP_REACH;
        modeCfgPtr->WdgMExtLogSupRelated[extIdx].WdgMExtLogSupervisionInfo->WdgMExtLastReachedSEID = WDGM_NO_SE_REACH;
        modeCfgPtr->WdgMExtLogSupRelated[extIdx].WdgMExtLogSupervisionInfo->WdgMExtLogActiveFlag   = WDGM_AF_FALSE;
        modeCfgPtr->WdgMExtLogSupRelated[extIdx].WdgMExtLogSupervisionInfo->WdgMExtLogSupResult    = WDGM_CORRECT;
    }
#endif /*WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_InitSEStatus(const WdgM_ModeCfgType* modeCfgPtr)
{
    for (uint16 idx = 0u; idx < WDGM_SE_NUM; idx++)
    {
        WdgM_LocalStatusInfo[idx] = WDGM_LOCAL_STATUS_DEACTIVATED;
    }

    for (uint16 seIdx = 0u; seIdx < modeCfgPtr->WdgMModeSECnt; seIdx++)
    {
        WdgM_SupervisedEntityIdType seId = modeCfgPtr->WdgMSERelated[seIdx].WdgMSEGeneralCfg->WdgMSEID;
        WdgM_LocalStatusInfo[seId]       = WDGM_LOCAL_STATUS_OK;
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_DeinitSEStatus(const WdgM_ModeCfgType* modeCfgPtr)
{
    for (uint16 seIdx = 0u; seIdx < modeCfgPtr->WdgMModeSECnt; seIdx++)
    {
        WdgM_SupervisedEntityIdType seId = modeCfgPtr->WdgMSERelated[seIdx].WdgMSEGeneralCfg->WdgMSEID;
        WdgM_LocalStatusInfo[seId]       = WDGM_LOCAL_STATUS_DEACTIVATED;
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_GetModeSEIndex(WdgM_SupervisedEntityIdType SEID, uint16* modeSETabIndex)
{
    uint16         seIdx;
    Std_ReturnType retVal = E_NOT_OK;

    for (seIdx = 0u; seIdx < WdgM_ModeCfgPtr->WdgMModeSECnt; seIdx++)
    {
        if (WdgM_ModeCfgPtr->WdgMSERelated[seIdx].WdgMSEGeneralCfg->WdgMSEID == SEID)
        {
            retVal          = E_OK;
            *modeSETabIndex = seIdx;
            break;
        }
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 5017, 6010, 6080 ++ */ /*VL_WdgM_5017, VL_MTR_WdgM_STCYC, VL_MTR_WdgM_STPTH */
WDGM_LOCAL void WdgM_LocalStatusChangeWithSetMode(const WdgM_ModeCfgType* newModeCfg)
{
    /* Set new SE status to the same as old SE */
    for (uint16 newIdx = 0u; newIdx < newModeCfg->WdgMModeSECnt; newIdx++)
    {
        Std_ReturnType setFlag = E_NOT_OK;
        for (uint16 oldIdx = 0u; oldIdx < WdgM_ModeCfgPtr->WdgMModeSECnt; oldIdx++)
        {
            if (newModeCfg->WdgMSERelated[newIdx].WdgMSEGeneralCfg->WdgMSEID
                == WdgM_ModeCfgPtr->WdgMSERelated[oldIdx].WdgMSEGeneralCfg->WdgMSEID)
            {
                setFlag = E_OK;
                break;
            }
        }
        if (setFlag != E_OK)
        {
            /* Unfind current se in old mode */
            uint16 unFindSEID = newModeCfg->WdgMSERelated[newIdx].WdgMSEGeneralCfg->WdgMSEID;
            /* Set SE supervision information to CORRECT */
            WdgM_ClearModeSEStatus(&newModeCfg->WdgMSERelated[newIdx]);
            /* And clear the alive failedAliveSupCounter */
            newModeCfg->WdgMSERelated[newIdx].WdgMSEInfo->WdgMFailedAliveSupCounter = 0u;
            WdgM_LocalStatusInfo[unFindSEID]                                        = WDGM_LOCAL_STATUS_OK;
#if (WDGM_RTE_ENABLED == STD_ON)
            (void)newModeCfg->WdgMSERelated[newIdx].WdgMSEGeneralCfg->WdgMLocalStatusNotification(
                RTE_MODE_WdgM_Mode_SUPERVISION_OK);
#endif
        }
        newModeCfg->WdgMSERelated[newIdx].WdgMSEInfo->WdgMEachSupResult[WDGM_ALIVE_SUP] = WDGM_CORRECT;
    }
    /* Set old mode SE which is not included in new mode to deactivated */
    for (uint16 oldIdx = 0u; oldIdx < WdgM_ModeCfgPtr->WdgMModeSECnt; oldIdx++)
    {
        Std_ReturnType findFlag = E_NOT_OK;
        for (uint16 newIdx = 0u; newIdx < newModeCfg->WdgMModeSECnt; newIdx++)
        {
            if (WdgM_ModeCfgPtr->WdgMSERelated[oldIdx].WdgMSEGeneralCfg->WdgMSEID
                == newModeCfg->WdgMSERelated[newIdx].WdgMSEGeneralCfg->WdgMSEID)
            {
                findFlag = E_OK;
                break;
            }
        }
        if (findFlag != E_OK)
        {
            WdgM_SupervisedEntityIdType seId = WdgM_ModeCfgPtr->WdgMSERelated[oldIdx].WdgMSEGeneralCfg->WdgMSEID;
            if ((WdgM_LocalStatusInfo[seId] == WDGM_LOCAL_STATUS_OK)
                || (WdgM_LocalStatusInfo[seId] == WDGM_LOCAL_STATUS_FAILED))
            {
                /*SWS_WdgM_00207, SWS_WdgM_00291, SWS_WdgM_00315*/
                WdgM_LocalStatusInfo[seId] = WDGM_LOCAL_STATUS_DEACTIVATED;
                WdgM_ModeCfgPtr->WdgMSERelated[oldIdx].WdgMSEInfo->WdgMEachSupResult[WDGM_ALIVE_SUP]    = WDGM_CORRECT;
                WdgM_ModeCfgPtr->WdgMSERelated[oldIdx].WdgMSEInfo->WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_CORRECT;
                WdgM_ModeCfgPtr->WdgMSERelated[oldIdx].WdgMSEInfo->WdgMEachSupResult[WDGM_INTLOG_SUP]   = WDGM_CORRECT;
                WdgM_ModeCfgPtr->WdgMSERelated[oldIdx].WdgMSEInfo->WdgMEachSupResult[WDGM_EXTLOG_SUP]   = WDGM_CORRECT;
#if (WDGM_RTE_ENABLED == STD_ON)
                (void)WdgM_ModeCfgPtr->WdgMSERelated[oldIdx].WdgMSEGeneralCfg->WdgMLocalStatusNotification(
                    RTE_MODE_WdgM_Mode_SUPERVISION_DEACTIVATED);
#endif
            }
        }
    }
    WdgM_ModeCfgPtr = newModeCfg;
}
/* PRQA S 5017, 6010, 6080 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_ClearModeSEStatus(const WdgM_ModeSECfgType* seInfo)
{
#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
    for (uint16 idx = 0u; idx < seInfo->WdgMSEAliveCnt; idx++)
    {
        seInfo->WdgMAliveSupCfg[idx].WdgMAliveSupervisionInfo->WdgMAliveCycleCounter      = 0u;
        seInfo->WdgMAliveSupCfg[idx].WdgMAliveSupervisionInfo->WdgMAliveIndicationCounter = 0u;
        seInfo->WdgMAliveSupCfg[idx].WdgMAliveSupervisionInfo->WdgMAliveSupResult         = WDGM_CORRECT;
    }
#endif

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
    for (uint16 idx = 0u; idx < seInfo->WdgMSEDealineCnt; idx++)
    {
        seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMTimeStampStart    = 0u;
        seInfo->WdgMDeadlineSupCfg[idx].WdgMDeadlineSupervisionInfo->WdgMDeadlineSupResult = WDGM_CORRECT;
    }
#endif

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    if (seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo != NULL_PTR)
    {
        seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogActiveFlag   = WDGM_AF_FALSE;
        seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
        seInfo->WdgMSEGeneralCfg->WdgMIntLogSupInfo->WdgMIntLogSupResult    = WDGM_CORRECT;
    }
#endif
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_ModeExistenceConfirmation(WdgM_ModeType modeID)
{
    Std_ReturnType retVal = E_OK;
    if (modeID >= WdgM_ConfigPtr->WdgMModeCnt)
    {
        retVal = E_NOT_OK;
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/

#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_OffModeConfirmationForRequestMode(WdgM_ModeType modeId)
{
    Std_ReturnType retVal = E_OK;

    /* Judging whether there is an OFF MODE in the current mode. */
    for (uint8 triggerIdx = 0u; triggerIdx < WdgM_ConfigPtr->WdgMMode[modeId].WdgMModeTriggerCnt; triggerIdx++)
    {
        if (WDGIF_OFF_MODE == WdgM_ConfigPtr->WdgMMode[modeId].WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode)
        {
            retVal = E_NOT_OK;
            break;
        }
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/

#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_OffModeConfirmation(const WdgM_ConfigType* ConfigPtr)
{
    Std_ReturnType retVal = E_OK;

    /* Judging whether there is an OFF MODE in the current initialization mode. */
    for (uint8 triggerIdx = 0; triggerIdx < ConfigPtr->WdgMMode[ConfigPtr->WdgMInitialModeId].WdgMModeTriggerCnt;
         triggerIdx++)
    {
        if (WDGIF_OFF_MODE
            == ConfigPtr->WdgMMode[ConfigPtr->WdgMInitialModeId].WdgMTriggerRelated[triggerIdx].WdgMWatchdogMode)
        {
            retVal = E_NOT_OK;
            break;
        }
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL uint8 WdgM_GetWordBitState(uint16 wordNum, uint8 getBit)
{
    uint8 bitvalue;

    if (0u != (wordNum & ((uint16)0x0001u << getBit))) /* PRQA S 4397 */ /* VL_WdgM_4397 */
    {
        bitvalue = 1u;
    }
    else
    {
        bitvalue = 0u;
    }
    return bitvalue;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_InitWdgTrigger(const WdgM_ModeCfgType* modeCfgPtr)
{
    Std_ReturnType retVal = E_OK;

    for (uint8 devIndex = 0u; devIndex < modeCfgPtr->WdgMModeTriggerCnt; devIndex++)
    {
        const WdgM_TriggerType* devicePtr = &modeCfgPtr->WdgMTriggerRelated[devIndex];
        if (devicePtr != NULL_PTR)
        {
            retVal = WdgIf_SetMode(devicePtr->WdgMWatchdogDevice, devicePtr->WdgMWatchdogMode);
            if ((Std_ReturnType)E_NOT_OK == retVal)
            {
                WdgM_DeinitSEStatus(modeCfgPtr);
                WdgM_GlobalStatusInfo = WDGM_GLOBAL_STATUS_STOPPED;
                break;
            }
        }
    }
    return retVal;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_SetDemEventStatus()
{
    if (WdgM_DemSetFlag == WDGM_DEM_REPORT_FAILED)
    {
        (void)Dem_SetEventStatus(WdgM_ConfigPtr->WdgMDemEventID, DEM_EVENT_STATUS_PASSED);
        WdgM_DemSetFlag = WDGM_DEM_REPORT_PASSED;
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL Std_ReturnType WdgM_MainFunctionTickValueCompu(void)
{
    Std_ReturnType retActive = E_NOT_OK;

#if (WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u)
    WdgM_ApplicationType partIdx             = GetApplicationID();
    uint16               mainFunctionIdx     = WdgM_MainFunctionApplMapIdx(partIdx);
    uint16               modeMainFunctionIdx = 0u;

    for (modeMainFunctionIdx = 0u; modeMainFunctionIdx < WdgM_ModeCfgPtr->WdgMMainFunctionCnt; modeMainFunctionIdx++)
    {
        if (WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[modeMainFunctionIdx].WdgMMainFunctionApplMapIdx
            == mainFunctionIdx)
        {
            break;
        }
    }
    if ((mainFunctionIdx != WDGM_INVALIED_ID) && (modeMainFunctionIdx != WdgM_ModeCfgPtr->WdgMMainFunctionCnt))
    {
        {
            if (WdgM_MainFunctionStateVar[mainFunctionIdx].WdgMMainFunctionTickCounter
                != WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[modeMainFunctionIdx].WdgMainFunctionTickRate)
            {
                WdgM_MainFunctionStateVar[mainFunctionIdx].WdgMMainFunctionTickCounter++;
            }
            else
            {
                retActive                                                              = E_OK;
                WdgM_MainFunctionStateVar[mainFunctionIdx].WdgMMainFunctionTickCounter = 1u;
            }
        }
    }
    if ((retActive == E_OK)
        && (WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[modeMainFunctionIdx].WdgMMasterSatelliteMainFunctionAliveCfg
            != NULL_PTR))
    {
        WdgM_UpdateSatelliteMainFunctionAliveIndication(mainFunctionIdx, modeMainFunctionIdx);
    }
#else
    if (WdgM_MainFunctionStateVarTick != WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef)
    {
        WdgM_MainFunctionStateVarTick++;
    }
    else
    {
        retActive                     = E_OK;
        WdgM_MainFunctionStateVarTick = 1u;
    }
#endif
    return retActive;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#if (WDGM_PARTITION_ENABLED == STD_ON && WDGM_MAINFUNCTION_NUM > 1u)
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_UpdateSatelliteMainFunctionAliveIndication(uint16 mainFStateIdx, uint16 modeMainFCfgIdx)
{
    if (WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[modeMainFCfgIdx].WdgMMasterSatelliteMainFunctionAliveCfg
        != NULL_PTR)
    {
        WdgM_MainFunctionStateVar[mainFStateIdx].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveIndicationCounter++;
    }
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 6030, 6010 ++ */ /* VL_MTR_WdgM_STMIF, VL_MTR_WdgM_STCYC */
WDGM_LOCAL void WdgM_ConfirmSatelliteMainFunctionAliveResult(void)
{
    if ((WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus == WDGM_GLOBAL_STATUS_OK)
        || (WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus == WDGM_GLOBAL_STATUS_FAILED))
    {
        Std_ReturnType satelliteFailed = E_NOT_OK;
        for (uint8 idx = 0u; idx < WdgM_ModeCfgPtr->WdgMMainFunctionCnt; idx++)
        {
            if (WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[idx].WdgMMasterSatelliteMainFunctionAliveCfg != NULL_PTR)
            {
                /*Judge if need to perform alive detection for satellite mainfunction*/
                uint16 mainfuncVarIdx = WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[idx].WdgMMainFunctionApplMapIdx;
                if (WdgM_MainFunctionStateVar[mainfuncVarIdx]
                        .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter
                    == WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[idx]
                           .WdgMMasterSatelliteMainFunctionAliveCfg->WdgMMasterMainFunctionRefCycle)
                {
                    WdgM_PerformSatelliteMainFunctionAliveSupervision(
                        WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[idx].WdgMMainFunctionApplMapIdx,
                        &WdgM_MainFunctionStateVar[mainfuncVarIdx].WdgMMasterSatelliteAliveSupervisionInfo,
                        WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[idx].WdgMMasterSatelliteMainFunctionAliveCfg);
                    if (WdgM_MainFunctionStateVar[mainfuncVarIdx]
                            .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupState
                        == WDGM_LOCAL_STATUS_EXPIRED)
                    {
                        if (WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionExpiredAliveCyclCounter
                            < WdgM_ModeCfgPtr->WdgMMainFunctionAliveExpiredSupervisionCycleTol)
                        {
                            WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionExpiredAliveCyclCounter++;
                            WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus =
                                WDGM_GLOBAL_STATUS_EXPIRED;
                            WdgM_MainFunctionGlobalAliveSupervisionErrorNotification(WDGM_GLOBAL_STATUS_EXPIRED);
                        }
                        else
                        {
                            WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus =
                                WDGM_GLOBAL_STATUS_STOPPED;
                            WdgM_MainFunctionGlobalAliveSupervisionErrorNotification(WDGM_GLOBAL_STATUS_STOPPED);
                        }
                        break;
                    }
                    else if (
                        WdgM_MainFunctionStateVar[mainfuncVarIdx]
                            .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupState
                        == WDGM_LOCAL_STATUS_FAILED)
                    {
                        satelliteFailed = E_OK;
                    }
                    else
                    {
                        /*Do nothing*/
                    }

                    WdgM_MainFunctionStateVar[mainfuncVarIdx]
                        .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter = 1u;
                }
                else
                {
                    WdgM_MainFunctionStateVar[mainfuncVarIdx]
                        .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter++;
                }
            }
        }
        if (satelliteFailed == E_OK)
        {
            if (WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus == WDGM_GLOBAL_STATUS_OK)
            {
                WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus = WDGM_GLOBAL_STATUS_FAILED;
                WdgM_MainFunctionGlobalAliveSupervisionErrorNotification(WDGM_GLOBAL_STATUS_FAILED);
            }
        }
    }
    else if (WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus == WDGM_GLOBAL_STATUS_EXPIRED)
    {
        if (WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionExpiredAliveCyclCounter
            < WdgM_ModeCfgPtr->WdgMMainFunctionAliveExpiredSupervisionCycleTol)
        {
            WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionExpiredAliveCyclCounter++;
        }
        else
        {
            WdgM_ModeCfgPtr->WdgMModeInfo->WdgMMasterMainFunctionAliveStatus = WDGM_GLOBAL_STATUS_STOPPED;
            WdgM_MainFunctionGlobalAliveSupervisionErrorNotification(WDGM_GLOBAL_STATUS_STOPPED);
        }
    }
    else
    {
        /* MainFunction alive supervision state reached to stopped */
        WdgM_MainFunctionGlobalAliveSupervisionErrorNotification(WDGM_GLOBAL_STATUS_STOPPED);
    }
}
/* PRQA S 6030, 6010 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
WDGM_LOCAL void WdgM_PerformSatelliteMainFunctionAliveSupervision(
    WdgM_ApplicationType                                appl,
    WdgM_MasterSatelliteAliveSupInfoType*               stateVar,
    const WdgM_MasterSatelliteMainFunctionAliveCfgType* configVar)
{
    if ((stateVar->WdgMAliveSupState == WDGM_LOCAL_STATUS_OK)
        || (stateVar->WdgMAliveSupState == WDGM_LOCAL_STATUS_FAILED))
    {
        uint8 minMargin = configVar->WdgMMinMargin;
        uint8 maxMargin = configVar->WdgMMaxMargin;

        sint32 aliveSupTempResult =
            ((sint32)(stateVar->WdgMAliveIndicationCounter)) - ((sint32)(configVar->WdgMExpectedAliveSupIndication));

        if (((aliveSupTempResult + (sint32)minMargin) >= 0L) && (aliveSupTempResult <= (sint32)maxMargin))
        {
            stateVar->WdgMAliveSupResult = WDGM_CORRECT;
            if ((stateVar->WdgMAliveSupState == WDGM_LOCAL_STATUS_FAILED)
                && ((configVar->WdgMFailedAliveSupervisionRefCycleTol - stateVar->WdgMFailedAliveCycleRefCounter)
                    == 0u))
            {
                stateVar->WdgMAliveSupState              = WDGM_LOCAL_STATUS_OK;
                stateVar->WdgMFailedAliveCycleRefCounter = 0u;
                WdgM_MainFunctionLocalAliveSupervisionErrorNotification(appl, WDGM_LOCAL_STATUS_OK);
            }
        }
        else
        {
            stateVar->WdgMAliveSupResult = WDGM_INCORRECT;
            if (stateVar->WdgMFailedAliveCycleRefCounter < configVar->WdgMFailedAliveSupervisionRefCycleTol)
            {
                stateVar->WdgMFailedAliveCycleRefCounter++;
                stateVar->WdgMAliveSupState = WDGM_LOCAL_STATUS_FAILED;
                WdgM_MainFunctionLocalAliveSupervisionErrorNotification(appl, WDGM_LOCAL_STATUS_FAILED);
            }
            else
            {
                stateVar->WdgMAliveSupState = WDGM_LOCAL_STATUS_EXPIRED;
                WdgM_MainFunctionLocalAliveSupervisionErrorNotification(appl, WDGM_LOCAL_STATUS_EXPIRED);
            }
        }
    }
    stateVar->WdgMAliveIndicationCounter = 0u;
}
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
/* PRQA S 6030 ++ */ /* VL_MTR_WdgM_STMIF */
WDGM_LOCAL void WdgM_MainFunctionSetMode(WdgM_ModeType modeId)
{
    /*New Mode*/
    uint16 WdgM_MainFunctionModeChangeSign[WDGM_MAINFUNCTION_NUM] = {0};
    for (uint8 idx = 0u; idx < WdgM_ConfigPtr->WdgMMode[modeId].WdgMMainFunctionCnt; idx++)
    {
        const WdgM_MainFunctionCfgType* newMainFunctionCfgPtr =
            &WdgM_ConfigPtr->WdgMMode[modeId].WdgMModeMainFunctionCfgRef[idx];
        /*Old Mode*/
        for (uint8 oldIdx = 0u; oldIdx < WdgM_ModeCfgPtr->WdgMMainFunctionCnt; oldIdx++)
        {
            /*Init the change sign for old mode mainfunction*/
            WdgM_MainFunctionModeChangeSign[oldIdx] = 0u;
            const WdgM_MainFunctionCfgType* oldMainFunctionCfgPtr =
                &WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[oldIdx];

            if (newMainFunctionCfgPtr->WdgMMainFunctionApplMapIdx == oldMainFunctionCfgPtr->WdgMMainFunctionApplMapIdx)
            {
                /*The same mainfunction, then judge the local status of current mainfunction*/
                uint16 idxMap = WdgM_MainFunctionApplMapIdx(oldMainFunctionCfgPtr->WdgMMainFunctionApplMapIdx);
                if (WdgM_MainFunctionStateVar[idxMap]
                        .WdgMMasterSatelliteAliveSupervisionInfo.WdgMFailedAliveCycleRefCounter
                    == WDGM_LOCAL_STATUS_FAILED)
                {
                    {
                        if (newMainFunctionCfgPtr->WdgMMasterSatelliteMainFunctionAliveCfg != NULL_PTR)
                        {
                            WdgM_MainFunctionStateVar[idxMap]
                                .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter = 1u;
                            WdgM_MainFunctionStateVar[idxMap]
                                .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveIndicationCounter = 0u;
                            WdgM_MainFunctionStateVar[idxMap]
                                .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupResult = WDGM_CORRECT;
                        }
                        else
                        {
                            WdgM_MainFunctionStateVar[idxMap]
                                .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter = WDGM_INVALIED_ID;
                            WdgM_MainFunctionStateVar[idxMap]
                                .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveIndicationCounter = WDGM_INVALIED_ID;
                            WdgM_MainFunctionStateVar[idxMap]
                                .WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupResult = WDGM_CORRECT;
                        }
                    }
                }
                WdgM_MainFunctionModeChangeSign[oldIdx] = 1u;
                break;
            }
        }
    }
    /*New mode mainfunction has been deal*/

    /*Deal those mainfunction which not include in new mode but in old mode, reset them and set to DEACTIVED*/
    for (uint8 oldIdx = 0u; oldIdx < WdgM_ModeCfgPtr->WdgMMainFunctionCnt; oldIdx++)
    {
        if (WdgM_MainFunctionModeChangeSign[oldIdx] == 0u)
        {
            uint16 idxMap = WdgM_ModeCfgPtr->WdgMModeMainFunctionCfgRef[oldIdx].WdgMMainFunctionApplMapIdx;
            WdgM_MainFunctionStateVar[idxMap].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveCycleCounter =
                WDGM_INVALIED_ID;
            WdgM_MainFunctionStateVar[idxMap].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveIndicationCounter =
                WDGM_INVALIED_ID;
            WdgM_MainFunctionStateVar[idxMap].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupResult = WDGM_CORRECT;
            WdgM_MainFunctionStateVar[idxMap].WdgMMasterSatelliteAliveSupervisionInfo.WdgMFailedAliveCycleRefCounter =
                WDGM_INVALIED_ID;
            WdgM_MainFunctionStateVar[idxMap].WdgMMasterSatelliteAliveSupervisionInfo.WdgMAliveSupState =
                WDGM_LOCAL_STATUS_DEACTIVATED;
        }
    }
}
/* PRQA S 6030 -- */
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
#endif /*WDGM_PARTITION_ENABLED == STD_ON*/
