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
/* PRQA S 6520, 3415 EOF */ /* VL_MTR_WdgM_STVAR, VL_WdgM_3415 */
/* =================================================== inclusions =================================================== */
#include "WdgIf.h" /* PRQA S 0380 */ /* VL_WdgM_0380 */
#include "WdgM_Internal.h"
#include "SchM_WdgM.h"
#include "Det.h"
#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
#include "Dem.h"
#endif /*WDGM_E_SUPERVISION_ENABLE == STD_ON*/
#if (WDGM_RTE_ENABLED == STD_ON)
#include "Rte_WdgM.h"
#endif /*WDGM_RTE_ENABLED == STD_ON*/
#if (WDGM_IMMEDIATE_RESET == STD_ON)
#include "Mcu.h"
#endif /*WDGM_IMMEDIATE_RESET == STD_ON*/

/* ===================================================== macros ===================================================== */

/**
 * @name WDGM_C_VERSION_MACROS
 * @{
 * @brief group contains all version info macros in WdgM.c
 */
#define WDGM_C_SW_MAJOR_VERSION 2u /**< Major Version*/
#define WDGM_C_SW_MINOR_VERSION 2u /**<Minor Version*/
#define WDGM_C_SW_PATCH_VERSION 7u /**<Patch Version*/
#define WDGM_C_AR_MAJOR_VERSION 4u /**<Autosar Major Version*/
#define WDGM_C_AR_MINOR_VERSION 9u /**<Autosar Minor Version*/
#define WDGM_C_AR_PATCH_VERSION 0u /**<Autosar Patch Version*/
/**@} */

/* ------------------------------------------------- Version  Check ------------------------------------------------- */
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

/* ================================================ type definitions ================================================ */

/* ============================================ internal data definitions =========================================== */
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

WDGM_LOCAL WdgM_SEInfoType WdgM_SEInfoRelated[WDGM_SE_NUM];

#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "WdgM_MemMap.h"

#if (WDGM_COMBINATION_USED == STD_ON)
#define WDGM_START_SEC_VAR_INIT_PTR
#include "WdgM_MemMap.h"

WDGM_LOCAL const WdgM_MultipleCoresCombinationType* WdgM_CombinationCfgPtr = WdgM_CombinationCfg;

#define WDGM_STOP_SEC_VAR_INIT_PTR
#include "WdgM_MemMap.h"
#endif /*WDGM_COMBINATION_USED == STD_ON*/

/* ========================================== internal function declarations ======================================== */
#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"

/**
 * @brief The function returns the result if init is OK.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @retval      TRUE: Initialize WdgM is FAILED.
 * @retval      FALSE: Initialize WdgM is OK.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-67744
 */
WDGM_LOCAL Std_ReturnType WdgM_DoInit(const WdgM_RuntimeType* runDataPtr, const WdgM_ConfigType* ConfigPtr);

/**
 * @brief Init the releated Supervised Entity Status of current Mode.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63749
 */
WDGM_LOCAL void WdgM_InitSEStatus(const WdgM_ModeCfgType* modeCfg);

/**
 * @brief Init the releated Supervised Entity Tab ram data of current Config.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68338
 */
WDGM_LOCAL void WdgM_InitRelatedSETab(const WdgM_ConfigType* ConfigPtr);

/**
 * @brief Init the releated Supervision Status of current mode.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-68344
 */
WDGM_LOCAL void WdgM_InitLocalSupStatus(const WdgM_RuntimeType* runDataPtr, const WdgM_ModeCfgType* modeCfg);

/**
 * @brief Init WDG trigger driver.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @retval      TRUE: Initialize Wdg Trigger within the current Config is SUCCESS.
 * @retval      FALSE: Initialize Wdg Trigger within the current Config is FAILED.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63771
 */
WDGM_LOCAL Std_ReturnType WdgM_InitWdgTrigger(const WdgM_RuntimeType* runDataPtr, const WdgM_ConfigType* ConfigPtr);

/**
 * @brief Deactivate the SE internal variables.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @param[in]   seIndex: SE index within current mode.
 * @param[in]   seStatusTabIndex: SE status index in SE status global variable.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63756
 */
WDGM_LOCAL void WdgM_SEDeactivateInternal(
    const WdgM_RuntimeType* runDataPtr,
    const WdgM_ModeCfgType* modeCfg,
    uint16                  seIndex,
    uint16                  seStatusTabIndex);

/**
 * @brief Change SE status between old mode and new mode.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   oldModeCfg: Pointer to old mode config data.
 * @param[in]   newModeCfg: Pointer to ne mode config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63732
 */
WDGM_LOCAL void WdgM_ChangeSEStatus(
    const WdgM_RuntimeType* runDataPtr,
    const WdgM_ModeCfgType* oldModeCfg,
    const WdgM_ModeCfgType* newModeCfg);

/**
 * @brief Deinit the SE status.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63742
 */
WDGM_LOCAL void WdgM_DeinitSEStatus(const WdgM_ModeCfgType* modeCfg);

/**
 * @brief Call kinds of supervision functions to confirm all supervisions results.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67745
 */
WDGM_LOCAL void WdgM_ConfirmSupervisionResult(const WdgM_RuntimeType* runDataPtr, const WdgM_SECfgType* seInfo);

#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Confirm the alive supervision result of current SE.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63736
 */
WDGM_LOCAL void WdgM_ConfirmAliveSupResultEachSE(const WdgM_RuntimeType* runDataPtr, const WdgM_SECfgType* seInfo);

/**
 * @brief Confirm the result of alive temp supervision.
 * @param[in]   currentAliveTab: Alive supervision runtime data pointer.
 * @param[in]   refCycle: Current reference mainfunction recycle.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63737
 */
WDGM_LOCAL Std_ReturnType WdgM_ConfirmAliveSupResult(WdgM_AliveSupInfoType* currentAliveTab, uint16 refCycle);
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Confirm the deadline supervision result of current SE.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63738
 */
WDGM_LOCAL void WdgM_ConfirmDeadlineSupResultEachSE(const WdgM_RuntimeType* runDataPtr, const WdgM_SECfgType* seInfo);
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Confirm the internal logical supervision result of current SE.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63740
 */
WDGM_LOCAL void WdgM_ConfirmIntLogSupResultEachSE(WdgM_SupervisedEntityIdType SEID);
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

/**
 * @brief Confirm the external logical supervision result of current mode.
 * @param[in]   SEID: Supervised Entity ID.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63739
 */
WDGM_LOCAL void WdgM_ConfirmExtLogSupResultEachSE(const WdgM_RuntimeType* runDataPtr, const WdgM_ModeCfgType* modeCfg);

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Perform the deadline timeout detection during mainfuncion.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63767
 */
WDGM_LOCAL void WdgM_PerformDeadlineTimeoutDetection(const WdgM_RuntimeType* runDataPtr, const WdgM_SECfgType* seInfo);
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

/**
 * @brief Confirm the SE local status of current SE during mainfunction.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63766
 */
WDGM_LOCAL void WdgM_ConfirmSELocalStatus(const WdgM_SECfgType* seInfo);

/**
 * @brief Perform handle the alive failed tolerance comupute during confirmation when the status is FAILED .
 * @param[in]   SEID: Supervised Entity Id.
 * @param[in]   aliveFailedTol: Alive failed tolerance time of current SE.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67748
 */
WDGM_LOCAL Std_ReturnType WdgM_ConfirmSELocalStatusHandleFAILED(uint16 SEID, const uint8 aliveFailedTol);

/**
 * @brief Perform handle the alive failed tolerance comupute during confirmation when the status is OK.
 * @param[in]   SEID: Supervised Entity Id.
 * @param[in]   aliveFailedTol: Alive failed tolerance time of current SE.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67747
 */
WDGM_LOCAL Std_ReturnType WdgM_ConfirmSELocalStatusHandleOK(uint16 SEID, const uint8 aliveFailedTol);

/**
 * @brief Confirm global status during mainfunction.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63765
 */
WDGM_LOCAL void WdgM_ConfirmGlobalStatus(void);

/**
 * @brief Write to ram data of expired SEID when the first SE is expired.
 * @param[in]   SEID: Supervised Entity Id.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-67749
 */
WDGM_LOCAL void WdgM_WriteFirstExpiredSEID(uint16 SEID);

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
WDGM_LOCAL Std_ReturnType WdgM_ModeExistenceConfirmation(WdgM_ModeType modeID);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/

/**
 * @brief Get the index of the SE in the array of the configured SE Tab.
 * @param[in]   SEID: Supervised Entity Id.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @param[out]  modeSETabIndex: Index of in current mode related SE list.
 * @retval      TRUE: SE index is in current mode.
 * @retval      FALSE: SE index is not in current mode.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63744
 */
WDGM_LOCAL Std_ReturnType
    WdgM_GetModeSEIndex(WdgM_SupervisedEntityIdType SEID, uint16* modeSETabIndex, const WdgM_ModeCfgType* modeCfg);

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
WDGM_LOCAL Std_ReturnType
    WdgM_GetSECheckpointIndex(WdgM_CheckpointIdType CPID, const WdgM_SECfgType* seInfo, uint16* cpTabIndex);

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
WDGM_LOCAL Std_ReturnType WdgM_OffModeConfirmation(const WdgM_ConfigType* ConfigPtr);

/**
 * @brief Check if the requested mode would disable the watchdog .
 * @param[in]   modeID: Mode Id.
 * @retval      TRUE: Requested mode will not disable the watchdog.
 * @retval      FALSE: Requested mode will disable the watchdog.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63754
 */
WDGM_LOCAL Std_ReturnType WdgM_OffModeConfirmationForRequestMode(WdgM_ModeType modeID);
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/

/**
 * @brief If global status is ok/failed WdgM will set the trigger condition.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @param[in]   currentGlobalResult: Pointer to current global status in WdgM.
 * @param[in]   modeID: Mode Id.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63757
 */
WDGM_LOCAL void WdgM_SetTrigger(
    const WdgM_ConfigType*       ConfigPtr,
    const WdgM_GlobalStatusType* currentGlobalResult,
    WdgM_ModeType                modeId);

#if (WDGM_IMMEDIATE_RESET == STD_ON)
/**
 * @brief Error operation when the global status is stop.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63758
 */
WDGM_LOCAL void WdgM_StopStatusErrorHanding(void);
#endif /*WDGM_IMMEDIATE_RESET == STD_ON*/

/**
 * @brief Update the alive indication counter if current CP has been reached.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63762
 */
WDGM_LOCAL void WdgM_UpdateAICounter(const WdgM_RuntimeType* runDataPtr, uint16 cpIndex, const WdgM_SECfgType* seInfo);

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Update the deadline supervision result if current CP has been reached.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @param[in]   curWdgMTime WdgM_DeadlineTimeType.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63768
 */
WDGM_LOCAL void WdgM_UpdateDeadlineSup(
    const WdgM_RuntimeType* runDataPtr,
    uint16                  cpIndex,
    const WdgM_SECfgType*   seInfo,
    WdgM_DeadlineTimeType   curWdgMTime);

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
WDGM_LOCAL void WdgM_DoUpdateDeadlineSup(
    const WdgM_CPSupportDeadLineInfoType* cpSupportDeadlineInfo,
    WdgM_DeadlineSupInfoType*             currentDeadlineTab,
    const WdgM_SECfgType*                 seInfo,
    WdgM_DeadlineTimeType                 curWdgMTime);

/**
 * @brief Update the deadline supervision result of current deadline config.
 * @param[in]   timeGap: Time gap between start checkpoint and stop checkpoint in deadline supervision.
 * @param[in]   deadlineSupCfg: Pointer to deadline supervision config data.
 * @param[in]   currentDeadlineTab: Deadline supervision runtime data pointer.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63764
 */
WDGM_LOCAL void WdgM_UpdateDeadlineSupResult(
    WdgM_DeadlineTimeType          timeGap,
    const WdgM_DeadlineSupCfgType* deadlineSupCfg,
    WdgM_DeadlineSupInfoType*      currentDeadlineTab);
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Update the internal logical supervision result of current internal logical config.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @param[in]   seInfo: Pointer to current SE data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63770
 */
WDGM_LOCAL void WdgM_UpdateIntLogSup(uint16 cpIndex, const WdgM_SECfgType* seInfo);
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
WDGM_LOCAL void WdgM_UpdateExtLogSup(
    const WdgM_RuntimeType* runDataPtr,
    uint16                  cpIndex,
    const WdgM_SECfgType*   seInfo,
    const WdgM_ModeCfgType* modeCfg);

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
WDGM_LOCAL void WdgM_UpdateExtLogSupResult(
    WdgM_ExtLogSupInfoType*           currentExtLogSupTab,
    const WdgM_ExtLogSupCfgType*      currentExtLogSup,
    const WdgM_CheckpointIdType       CheckpointID,
    const WdgM_SupervisedEntityIdType SEID);
#endif /*WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

/**
 * @brief Call the kinds of supervision update functions to check the supervision status.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   seInfo: Pointer to current SE data.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @param[in]   cpIndex: Checkpoint index in SE.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67756
 */
WDGM_LOCAL void WdgM_UpdateSupervision(
    const WdgM_RuntimeType* runDataPtr,
    const WdgM_SECfgType*   seInfo,
    const WdgM_ModeCfgType* modeCfg,
    uint16                  cpIndex);

/**
 * @brief Clear external logical status during mode change in the old mode.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   oldModeCfg: Pointer to old mode config data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63733
 */
WDGM_LOCAL void WdgM_ClearExtlogStatus(const WdgM_RuntimeType* runDataPtr, const WdgM_ModeCfgType* oldModeCfg);

/**
 * @brief Clear alive and deadline supervision status if not use anymore in old mode SE.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   oldSE: Pointer to old SE config data.
 * @param[in]   newSE: Pointer to new SE config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-63734
 */
WDGM_LOCAL void WdgM_ClearNoUseAliveDeadlineStatus(
    const WdgM_RuntimeType* runDataPtr,
    const WdgM_SECfgType*   oldSE,
    const WdgM_SECfgType*   newSE);

/**
 * @brief Get the bit value in the word.
 * @param[in]   wordNum: Data to get bit.
 * @param[in]   getBit: The bit which to get.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63748
 */
WDGM_LOCAL uint8 WdgM_GetWordBitState(uint16 wordNum, uint8 getBit);

#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
/**
 * @brief Report status of current componet to Dem.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-67757
 */
WDGM_LOCAL void WdgM_DemSetEvent(const WdgM_RuntimeType* runDataPtr, const WdgM_ConfigType* ConfigPtr);
#endif /*WDGM_E_SUPERVISION_ENABLE == STD_ON*/

/**
 * @brief Perform the mode change.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   Mode: Mode id.
 * @retval      TRUE: Set Mode is OK.
 * @retval      FALSE: Set Mode is FAILED.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-67758
 */
WDGM_LOCAL Std_ReturnType
    WdgM_DoSetMode(const WdgM_ConfigType* ConfigPtr, const WdgM_RuntimeType* runDataPtr, WdgM_ModeType Mode);

/**
 * @brief Perform the checkpoint reached.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   seInfo: Pointer to current SE data.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @param[in]   CheckpointID: Checkpoint Id.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67759
 */
WDGM_LOCAL void WdgM_DoCheckpointReached(
    WdgM_RuntimeType*       runDataPtr,
    const WdgM_SECfgType*   seInfo,
    const WdgM_ModeCfgType* modeCfg,
    WdgM_CheckpointIdType   CheckpointID);

/**
 * @brief Perform the params check during checkpoint reached.
 * @param[in]   currentGlobalResult: Pointer to current global status in WdgM.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @param[in]   SEID: Supervised Entity Id.
 * @param[in]   CheckpointID: Checkpoint Id .
 * @retval      TRUE: Params check is OK.
 * @retval      FALSE: Params check is FAILED.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67761
 */
WDGM_LOCAL Std_ReturnType WdgM_DoParamsCheckforCheckpointReached(
    const WdgM_GlobalStatusType* currentGlobalResult,
    const WdgM_ModeCfgType*      modeCfg,
    WdgM_SupervisedEntityIdType  SEID,
    WdgM_CheckpointIdType        CheckpointID);

#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
/**
 * @brief Perform the limits check during checkpoint reached.
 * @param[in]   SEID: Supervised Entity Id.
 * @retval      TRUE:    Limits check is OK.
 * @retval      FALSE:   Limits check is FAILED.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-68369
 */
WDGM_LOCAL Std_ReturnType WdgM_CheckpointReachedLimitCheck(const WdgM_SupervisedEntityIdType SEID);
#endif /*WDGM_MULTI_PARTITION_ENABLED == STD_ON*/

#if (WDGM_COMBINATION_USED == STD_ON)
/**
 * @brief Compute the combination status of current WdgM.
 * @param[in]   currentCombinationPtr: Pointer to global variable CombinationCfg.
 * @retval      WDGM_COMBINATION_OK       Combination result is OK.
 * @retval      WDGM_COMBINATION_NOT_OK   Combination result is NOT OK.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-63773
 */
WDGM_LOCAL WdgM_CombinationStatusType
    WdgM_ComputeCombinationStatus(const WdgM_MultipleCoresCombinationType* currentCombinationPtr);

/**
 * @brief Get the index of slave in current combination slaves array.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @param[in]   slaveCoreIndex: Slave core index in combination.
 * @retval      TRUE: Get the index successfully.
 * @retval      FALSE: Get the index unsuccessfully.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67763
 */
WDGM_LOCAL Std_ReturnType WdgM_GetCombinationSlaveStatusIndex(const WdgM_ConfigType* ConfigPtr, uint8* slaveCoreIndex);

/**
 * @brief Perform combination handle according to the result of combination during mainfuncton.
 * @param[in]   runDataPtr: Pointer to WdgM runtime data.
 * @param[in]   ConfigPtr: Pointer to WdgM post build config data.
 * @param[in]   currentGlobalResult: Pointer to current global status in WdgM.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67764
 */
WDGM_LOCAL void WdgM_CombinationResultHandle(const WdgM_RuntimeType* runDataPtr, const WdgM_ConfigType* ConfigPtr);

/**
 * @brief Combination master handle according to the result of combination during mainfuncton.
 * @param[in]   currentCombinationPtr: Pointer to global variable CombinationCfg.
 * @param[in]   modeCfg: Pointer to current mode config data.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67765
 */
WDGM_LOCAL void WdgM_CombinationMasterDo(
    const WdgM_MultipleCoresCombinationType* currentCombinationPtr,
    const WdgM_GlobalStatusType*             currentGlobalResult,
    const WdgM_ModeCfgType*                  modeCfg);

/**
 * @brief Combination slaves handle according to the result of combination during mainfuncton.
 * @param[in]   currentCombinationPtr: Pointer to global variable CombinationCfg.
 * @param[in]   currentGlobalResult: Pointer to current global status in WdgM.
 * @param[in]   slaveIndex uint8.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-67766
 */
WDGM_LOCAL void WdgM_CombinationSlaveDo(
    const WdgM_MultipleCoresCombinationType* currentCombinationPtr,
    const WdgM_GlobalStatusType*             currentGlobalResult,
    uint8                                    slaveIndex);
#endif /*WDGM_COMBINATION_USED == STD_ON*/

/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
/**
 * This function initializes the WdgM module when config param is not nullptr
 * and global status is not in Deactivated.
 */
/* PRQA S 1503 ++*/ /*VL_QAC_NoUsedApi*/
void WdgM_Init(const WdgM_ConfigType* ConfigPtr)
/* PRQA S 1503 --*/
{
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    if (NULL_PTR == ConfigPtr)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_ID, WDGM_E_INV_POINTER);
    }
    else
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    {
#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
        if ((Std_ReturnType)E_NOT_OK == WdgM_OffModeConfirmation(ConfigPtr))
        {
            ((void)Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_INIT_ID, WDGM_E_DISABLE_NOT_ALLOWED));
        }
        else
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/
        {
            const WdgM_RuntimeType* runDataPtr = WdgM_GetRunDataPtr();

            if ((runDataPtr != NULL_PTR) && ((*runDataPtr->WdgMGlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)))
            {
                uint8 tabIndex;
                if ((WdgM_DoInit(runDataPtr, ConfigPtr) == E_OK) && (WdgM_GetCoreIndex(&tabIndex) == E_OK))
                {
                    WdgM_SelectConfigs[tabIndex]  = ConfigPtr;
                    *runDataPtr->WdgMGlobalStatus = WDGM_GLOBAL_STATUS_OK;
                }
            }
        }
    }
}

WDGM_LOCAL Std_ReturnType WdgM_DoInit(const WdgM_RuntimeType* runDataPtr, const WdgM_ConfigType* ConfigPtr)
{
    WdgM_SupervisedEntityIdType* firstExpiredSEID;
    WdgM_SupervisedEntityIdType* firstExpiredSEIDInv;
    const WdgM_ModeCfgType*      modeCfg;
    Std_ReturnType               retVal;
    WdgM_ModeType                initModeId = 0u;

    /* Get the pointer of current config*/
    if (WdgM_GetFirstExpiredSEIDPtr(&firstExpiredSEID, &firstExpiredSEIDInv) != E_OK)
    {
        retVal = E_NOT_OK;
    }
    else
    {
        /* Initialize the FirstExpired */
        *firstExpiredSEID    = WDGM_INIT_FIRST_EXPIRED_SEID;
        *firstExpiredSEIDInv = WDGM_INIT_FIRST_EXPIRED_SEID;
        /* Initialize the related SE ram data */
        WdgM_InitRelatedSETab(ConfigPtr);

        /* Initialize Alive Deadline Logical Supervision ram data within the current mode */
        initModeId                                  = ConfigPtr->WdgMInitialModeId;
        runDataPtr->WdgMModeInfo->WdgMModeCurrentID = initModeId;
        modeCfg                                     = &ConfigPtr->WdgMMode[initModeId];
        WdgM_InitLocalSupStatus(runDataPtr, modeCfg);

        /* Initialize Wdg Trigger within the current Config */
        retVal = WdgM_InitWdgTrigger(runDataPtr, ConfigPtr);

        if (retVal == E_OK)
        {
            /* If above is ok then Set SE local status to OK*/
            WdgM_InitSEStatus(modeCfg);
        }

        /*Initialize the mainfunction tick and expired supervision tolerance*/
        runDataPtr->WdgMModeInfo->WdgMMainFunctionRunTick  = 0u;
        runDataPtr->WdgMModeInfo->WdgMExpiredSupTolCounter = 0u;
    }

    return retVal;
}

/**
 * This function deinitializes the WdgM module when global status is not in Deactivated.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
void WdgM_DeInit(void)
{
    const WdgM_RuntimeType* runDataPtr = WdgM_GetRunDataPtr();

    if (runDataPtr != NULL_PTR)
    {
        if (*runDataPtr->WdgMGlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
        {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
            (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_DEINIT_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        }
        else
        {
            *runDataPtr->WdgMGlobalStatus = WDGM_GLOBAL_STATUS_DEACTIVATED;
#if (WDGM_RTE_ENABLED == STD_ON)
            uint8 coreIndex;
            if (WdgM_GetCoreIndex(&coreIndex) == E_OK)
            {
                Rte_PortHandle_WdgM_GlobalMode_P phGlobal = Rte_Ports_WdgM_GlobalMode_P();
                phGlobal[coreIndex].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_DEACTIVATED);
            }
#endif /*WDGM_RTE_ENABLED == STD_ON*/
        }
    }
} /* PRQA S 1503,1505 --*/

/**
 * This function gets the WdgM version and can be invoked only when switch Version info api turns on.
 */
#if (WDGM_VERSION_INFO_API == STD_ON)
void WdgM_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
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
        VersionInfo->sw_major_version = WDGM_C_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = WDGM_C_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = WDGM_C_SW_PATCH_VERSION;
    }
}
#endif /*WDGM_VERSION_INFO_API == STD_ON*/

/* ========================================== internal function definitions ========================================= */
WDGM_LOCAL Std_ReturnType
    WdgM_DoSetMode(const WdgM_ConfigType* ConfigPtr, const WdgM_RuntimeType* runDataPtr, WdgM_ModeType Mode)
{
    uint16                  triggerIndex;
    uint8                   deviceID;
    Std_ReturnType          lRet         = E_OK;
    Std_ReturnType          lReturnValue = E_NOT_OK;
    WdgIf_ModeType          mappingMode;
    const WdgM_ModeCfgType* currentModeCfg;
    WdgM_ModeType           currentModeId;
    WdgM_GlobalStatusType*  currentGlobalResult = runDataPtr->WdgMGlobalStatus;

    /* Only global status is OK or FAILED then execute mode change */
    if ((WDGM_GLOBAL_STATUS_OK == *currentGlobalResult) || (WDGM_GLOBAL_STATUS_FAILED == *currentGlobalResult))
    {
        for (triggerIndex = 0u; triggerIndex < ConfigPtr->WdgMMode[Mode].ModeTriggerCnt; triggerIndex++)
        {
            deviceID    = ConfigPtr->WdgMMode[Mode].WdgMTriggerRelated[triggerIndex].WdgMWatchdogDevice;
            mappingMode = ConfigPtr->WdgMMode[Mode].WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode;
            lRet        = WdgIf_SetMode(deviceID, mappingMode);
            if ((Std_ReturnType)E_NOT_OK == lRet)
            {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
                ((void)Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_SET_MODE));
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
                *currentGlobalResult = WDGM_GLOBAL_STATUS_STOPPED;
#if (WDGM_RTE_ENABLED == STD_ON)
                uint8 coreIndex;
                if (WdgM_GetCoreIndex(&coreIndex) == E_OK)
                {
                    Rte_PortHandle_WdgM_GlobalMode_P phGlobal = Rte_Ports_WdgM_GlobalMode_P();
                    phGlobal[coreIndex].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_STOPPED);
                }
#endif /*WDGM_RTE_ENABLED == STD_ON*/
                lReturnValue = E_NOT_OK;
                break;
            }
        }
        /* If mode trigger mode change is ok then Change the SE ram data */
        if ((Std_ReturnType)E_OK == lRet)
        {
            currentModeId  = runDataPtr->WdgMModeInfo->WdgMModeCurrentID;
            currentModeCfg = &ConfigPtr->WdgMMode[currentModeId];
            WdgM_ChangeSEStatus(runDataPtr, currentModeCfg, (const WdgM_ModeCfgType*)&ConfigPtr->WdgMMode[Mode]);
            runDataPtr->WdgMModeInfo->WdgMModeCurrentID        = Mode;
            runDataPtr->WdgMModeInfo->WdgMExpiredSupTolCounter = 0u;
            runDataPtr->WdgMModeInfo->WdgMMainFunctionRunTick  = 0u;
            lReturnValue                                       = E_OK;
#if (WDGM_COMBINATION_USED == STD_ON)
            uint8 combinationID;
            combinationID = ConfigPtr->WdgMSupportCombinationID;
            if ((combinationID != WDGM_COMBINATITON_INVALIED_ID)
                && (ConfigPtr->WdgMMasterOrSlave == WDGM_COMBINATION_MASTER))
            {
                if (combinationID > WDGM_MAX_COMBINATION_NUM)
                {
                    lReturnValue = E_NOT_OK;
                }
                else
                {
                    (*WdgM_CombinationCfgPtr[combinationID].WdgMCombinationSyncCycleTab) = 0u;
                    lReturnValue                                                         = E_OK;
                }
            }
#endif /*WDGM_COMBINATION_USED == STD_ON*/
        }
    }
    return lReturnValue;
}

/**
 * This function sets WdgM mode and it will change Wdg driver to corresponding mode.
 * It should be noted that only can set to be off mode with OFFMODE Enabled.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
Std_ReturnType WdgM_SetMode(WdgM_ModeType Mode)
/* PRQA S 1503,1505 --*/
{
    Std_ReturnType          lReturnValue;
    const WdgM_ConfigType*  ConfigPtr;
    const WdgM_RuntimeType* runDataPtr = WdgM_GetRunDataPtr();

    if (runDataPtr == NULL_PTR)
    {
        lReturnValue = E_NOT_OK;
    }
    else if (*runDataPtr->WdgMGlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        lReturnValue = E_NOT_OK;
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if ((Std_ReturnType)E_NOT_OK == WdgM_ModeExistenceConfirmation(Mode))
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_PARAM_MODE);
        lReturnValue = E_NOT_OK;
    }
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
    else if ((Std_ReturnType)E_NOT_OK == WdgM_OffModeConfirmationForRequestMode(Mode))
    {
        Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_SET_MODE_ID, WDGM_E_DISABLE_NOT_ALLOWED);
        lReturnValue = E_NOT_OK;
    }
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/
    else
    {
        if (Mode == runDataPtr->WdgMModeInfo->WdgMModeCurrentID)
        {
            lReturnValue = E_OK;
        }
        else
        {
            if ((WdgM_GetConfigPtr(&ConfigPtr) == E_NOT_OK))
            {
                lReturnValue = E_NOT_OK;
            }
            else
            {
                lReturnValue = WdgM_DoSetMode(ConfigPtr, runDataPtr, Mode);
            }
        }
    }
    return lReturnValue;
}

/**
 * This function gets current WdgM mode only when WdgM module is initialized.
 */
/* PRQA S 1503 ++*/ /*VL_QAC_NoUsedApi*/
Std_ReturnType WdgM_GetMode(WdgM_ModeType* Mode)
/* PRQA S 1503 --*/
{
    Std_ReturnType          lReturnValue = E_NOT_OK;
    const WdgM_RuntimeType* runDataPtr   = WdgM_GetRunDataPtr();

    if (runDataPtr == NULL_PTR)
    {
        lReturnValue = E_NOT_OK;
    }
    else if (*runDataPtr->WdgMGlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_MODE_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        lReturnValue = E_NOT_OK;
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if (NULL_PTR == Mode)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_MODE_ID, WDGM_E_INV_POINTER);
        lReturnValue = E_NOT_OK;
    }
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    else
    {
        *Mode        = runDataPtr->WdgMModeInfo->WdgMModeCurrentID;
        lReturnValue = E_OK;
    }
    return lReturnValue;
}

WDGM_LOCAL void WdgM_UpdateSupervision(
    const WdgM_RuntimeType* runDataPtr,
    const WdgM_SECfgType*   seInfo,
    const WdgM_ModeCfgType* modeCfg,
    uint16                  cpIndex)
{
    /* If current Supervised Entity has deadline supervision */
#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
    if (seInfo->SEDealineCnt > 0u)
    {
        WdgM_SupervisedEntityIdType SEID;
        WdgM_DeadlineTimeType       curWdgMTime;

        SEID = seInfo->WdgMSupervisedEntityID;

        (void)GetCounterValue(WdgM_SupervisedEntityPcCfg[SEID].WdgMOsCounter, &curWdgMTime);
        curWdgMTime = curWdgMTime * WdgM_SupervisedEntityPcCfg[SEID].WdgMOsCounterResolution;
        /*Update Deadline supervision during checkpoint reached*/
        SchM_Enter_WdgM_Exclusive();
        WdgM_UpdateDeadlineSup(runDataPtr, cpIndex, seInfo, curWdgMTime);
        SchM_Exit_WdgM_Exclusive();
    }
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (                                                                                            \
    (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON) || (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON) \
    || (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON))

    SchM_Enter_WdgM_Exclusive();

#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
    /* If current Supervised Entity has alive supervision */
    if (seInfo->SEAliveCnt > 0u)
    {
        /*Update Alive indication counter during checkpoint reached*/
        WdgM_UpdateAICounter(runDataPtr, cpIndex, seInfo);
    }
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    /* If current Supervised Entity has internal logical supervision */
    if (seInfo->SEIntLogCnt > 0u)
    {
        /*Update Internal Logical supervision during checkpoint reached*/
        WdgM_UpdateIntLogSup(cpIndex, seInfo);
    }
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    /*Update External Logical supervision during checkpoint reached*/
    if (modeCfg->ModeExtLogCnt > 0u)
    {
        WdgM_UpdateExtLogSup(runDataPtr, cpIndex, seInfo, modeCfg);
    }
#endif /*WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

    SchM_Exit_WdgM_Exclusive();
#endif
}

/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
WDGM_LOCAL void WdgM_DoCheckpointReached(
    WdgM_RuntimeType*       runDataPtr,
    const WdgM_SECfgType*   seInfo,
    const WdgM_ModeCfgType* modeCfg,
    WdgM_CheckpointIdType   CheckpointID)
{
    Std_ReturnType lReturnValue = E_NOT_OK;
    uint16         seCpIndex    = 0u;

    if ((WDGM_GLOBAL_STATUS_OK == *runDataPtr->WdgMGlobalStatus)
        || (WDGM_GLOBAL_STATUS_FAILED == *runDataPtr->WdgMGlobalStatus))
    {
#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
        Std_ReturnType ret = WdgM_CheckpointReachedLimitCheck(seInfo->WdgMSupervisedEntityID);
        if (ret == E_OK)
#endif /*WDGM_MULTI_PARTITION_ENABLED == STD_ON*/
        {
            lReturnValue = WdgM_GetSECheckpointIndex(CheckpointID, seInfo, &seCpIndex);
            if (lReturnValue == E_OK)
            {
                WdgM_UpdateSupervision(runDataPtr, seInfo, modeCfg, seCpIndex);
            }
        }
    }
}
/* PRQA S 3673 -- */

WDGM_LOCAL Std_ReturnType WdgM_DoParamsCheckforCheckpointReached(
    const WdgM_GlobalStatusType* currentGlobalResult,
    const WdgM_ModeCfgType*      modeCfg,
    WdgM_SupervisedEntityIdType  SEID,
    WdgM_CheckpointIdType        CheckpointID)
{
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    uint16 modeSETabIndex = 0u;
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    Std_ReturnType lReturnValue;

    if ((WDGM_GLOBAL_STATUS_DEACTIVATED == *currentGlobalResult))
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        lReturnValue = E_NOT_OK;
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if ((Std_ReturnType)E_NOT_OK == WdgM_GetModeSEIndex(SEID, &modeSETabIndex, modeCfg))
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_ID, WDGM_E_PARAM_SEID);
        lReturnValue = E_NOT_OK;
    }
    else if ((CheckpointID + (WdgM_CheckpointIdType)1) > WdgM_SupervisedEntityPcCfg[SEID].SECPCnt)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_ID, WDGM_E_CPID);
        lReturnValue = E_NOT_OK;
    }
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    else if (WDGM_LOCAL_STATUS_DEACTIVATED == WdgM_SEInfoRelated[SEID].WdgMLocalStatus)
    {
        (void)
            Det_ReportRuntimeError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_CHECKPOINT_REACHED_ID, WDGM_E_SEDEACTIVATED);
        lReturnValue = E_NOT_OK;
    }
    else
    {
        lReturnValue = E_OK;
    }

    (void)modeCfg;
    (void)CheckpointID;
    return lReturnValue;
}

/**
 * This function indicates the WdgM that the Checkpoint has been reported, then it will perform the supervision
 * algorithm to decide the result of Alive, Deadline,Internal Logical and External Logical supervision.
 * It will not confirm the local or global status and only compute the algorithm supervison result.
 */
/* PRQA S 1503 ++*/ /*VL_QAC_NoUsedApi*/
Std_ReturnType WdgM_CheckpointReached(WdgM_SupervisedEntityIdType SEID, WdgM_CheckpointIdType CheckpointID)
/* PRQA S 1503 --*/
{
    uint16                  modeSETabIndex = 0u;
    Std_ReturnType          lReturnValue   = E_NOT_OK;
    const WdgM_SECfgType*   seInfo;
    const WdgM_ConfigType*  ConfigPtr;
    const WdgM_ModeCfgType* modeCfg;
    WdgM_RuntimeType*       runDataPtr = WdgM_GetRunDataPtr();

    if (runDataPtr == NULL_PTR)
    {
        lReturnValue = E_NOT_OK;
    }
    else if (WdgM_GetConfigPtr(&ConfigPtr) == E_NOT_OK)
    {
        lReturnValue = E_NOT_OK;
    }
    else
    {
        modeCfg = &ConfigPtr->WdgMMode[runDataPtr->WdgMModeInfo->WdgMModeCurrentID];
        lReturnValue =
            WdgM_DoParamsCheckforCheckpointReached(runDataPtr->WdgMGlobalStatus, modeCfg, SEID, CheckpointID);
        if (lReturnValue == E_OK)
        {
            if (WdgM_GetModeSEIndex(SEID, &modeSETabIndex, modeCfg) == E_OK)
            {
                seInfo = (const WdgM_SECfgType*)&modeCfg->WdgMSERelated[modeSETabIndex];
                WdgM_DoCheckpointReached(runDataPtr, seInfo, modeCfg, CheckpointID);
            }
        }
    }
    return lReturnValue;
}

#if (WDGM_MULTI_PARTITION_ENABLED == STD_ON)
WDGM_LOCAL Std_ReturnType WdgM_CheckpointReachedLimitCheck(const WdgM_SupervisedEntityIdType SEID)
{
    Std_ReturnType  lRet    = E_OK;
    ApplicationType partIdx = GetApplicationID();

    if (WdgM_SupervisedEntityPcCfg[SEID].WdgMEcucPartitionRef != partIdx)
    {
        lRet = E_NOT_OK;
    }
    return lRet;
}
#endif /*WDGM_MULTI_PARTITION_ENABLED == STD_ON*/

/**
 * This function gets the local status of current Supervised Entity. The state indicates the supervision algorithm
 * result and it will As a reference for calculating the global state.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
Std_ReturnType WdgM_GetLocalStatus(WdgM_SupervisedEntityIdType SEID, WdgM_LocalStatusType* Status)
/* PRQA S 1503,1505 --*/
{
    Std_ReturnType          lReturnValue = E_NOT_OK;
    const WdgM_RuntimeType* runDataPtr   = WdgM_GetRunDataPtr();

    if (runDataPtr == NULL_PTR)
    {
        lReturnValue = E_NOT_OK;
    }
    else if (*runDataPtr->WdgMGlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        lReturnValue = E_NOT_OK;
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if (WDGM_SE_NUM <= SEID) /*Check SEID if in range*/
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_ID, WDGM_E_PARAM_SEID);
        lReturnValue = E_NOT_OK;
    }
    else if (NULL_PTR == Status)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_LOCAL_STATUS_ID, WDGM_E_INV_POINTER);
        lReturnValue = E_NOT_OK;
    }
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    else
    {
        *Status      = WdgM_SEInfoRelated[SEID].WdgMLocalStatus;
        lReturnValue = E_OK;
    }
    return lReturnValue;
}

/**
 * This function gets the global status of WdgM and it can be invoked only when WdgM is initliazed. The global state is
 * calculated from all local states within current running mode and it has five states, respectively are DEACTIVATED,
 * OK, FAILED, EXPIRED and STOPPED.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
Std_ReturnType WdgM_GetGlobalStatus(WdgM_GlobalStatusType* Status)
/* PRQA S 1503,1505 --*/
{
    Std_ReturnType          lReturnValue = E_NOT_OK;
    const WdgM_RuntimeType* runDataPtr   = WdgM_GetRunDataPtr();

    if (runDataPtr == NULL_PTR)
    {
        lReturnValue = E_NOT_OK;
    }
    else if (WDGM_GLOBAL_STATUS_DEACTIVATED == *runDataPtr->WdgMGlobalStatus)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_GLOBAL_STATUS_ID, WDGM_E_UNINIT);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        lReturnValue = E_NOT_OK;
    }
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    else if (NULL_PTR == Status)
    {
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_GLOBAL_STATUS_ID, WDGM_E_INV_POINTER);
        lReturnValue = E_NOT_OK;
    }
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
    else
    {
        *Status      = *runDataPtr->WdgMGlobalStatus;
        lReturnValue = E_OK;
    }
    return lReturnValue;
}

/**
 * This function will call Mcu_PerfomReset to immediately reset. It should be noted that only if Mcu supports above
 * capability and open the switch ImmediatelyReset can use it.
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
void WdgM_PerformReset(void)
/* PRQA S 1503,1505 --*/
{
    uint8                   triggerIndex;
    const WdgM_ConfigType*  ConfigPtr;
    const WdgM_ModeCfgType* modeCfg;
    const WdgM_RuntimeType* runDataPtr = WdgM_GetRunDataPtr();

    if ((runDataPtr != NULL_PTR) && (WdgM_GetConfigPtr(&ConfigPtr) == E_OK))
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        if (*runDataPtr->WdgMGlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
        {
            (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_PERFORM_RESET_ID, WDGM_E_UNINIT);
        }
        else
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        {
            SchM_Enter_WdgM_Exclusive();
            modeCfg = &ConfigPtr->WdgMMode[runDataPtr->WdgMModeInfo->WdgMModeCurrentID];
            for (triggerIndex = 0u; triggerIndex < modeCfg->ModeTriggerCnt; triggerIndex++)
            {
                if (WDGIF_OFF_MODE != modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
                {
                    WdgIf_SetTriggerCondition(
                        modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogDevice,
                        WDGM_TRIGGER_CONDITION_STOP);
                }
            }
            SchM_Exit_WdgM_Exclusive();
            /* PRQA S 2740 ++*/ /*VL_WdgM_EndLessLoop*/
            while (1)
            {
                break;
            }
            /* PRQA S 2740 --*/
        }
    }
}

/**
 * This function gets the first expired SE's ID of WdgM. The double inverse value will not be clear after Wdg reset and
 * will be initliazed by api WdgM_Init().
 */
/* PRQA S 1503,1505 ++*/ /*VL_QAC_NoUsedApi,VL_WdgM_1505*/
Std_ReturnType WdgM_GetFirstExpiredSEID(WdgM_SupervisedEntityIdType* SEID)
/* PRQA S 1503,1505 --*/
{
    Std_ReturnType               lReturnValue;
    WdgM_SupervisedEntityIdType* firstExpiredSEID;
    WdgM_SupervisedEntityIdType* firstExpiredSEIDInv;

    if (NULL_PTR == SEID)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_GET_FIRST_EXPIRED_SEID_ID, WDGM_E_INV_POINTER);
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/
        lReturnValue = E_NOT_OK;
    }
    else if (WdgM_GetFirstExpiredSEIDPtr(&firstExpiredSEID, &firstExpiredSEIDInv) == E_NOT_OK)
    {
        lReturnValue = E_NOT_OK;
    }
    else
    {
        if (WDGM_VALIED_FIRST_EXPIRED_SEID_SUM == (*firstExpiredSEID + *firstExpiredSEIDInv))
        {
            *SEID        = *firstExpiredSEID;
            lReturnValue = E_OK;
        }
        else
        {
            *firstExpiredSEID    = 0u;
            *firstExpiredSEIDInv = 0u;
            *SEID                = 0u;
            lReturnValue         = E_NOT_OK;
        }
    }
    return lReturnValue;
}

#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
WDGM_LOCAL void WdgM_UpdateAICounter(const WdgM_RuntimeType* runDataPtr, uint16 cpIndex, const WdgM_SECfgType* seInfo)
{
    if ((uint8)STD_ON
        == WdgM_GetWordBitState(seInfo->WdgMCheckpointRelated[cpIndex].WdgMSupSupportGroup, (uint8)ALIVE_SUPPORT))
    {
        for (uint8 index = 0u; index < seInfo->SEAliveCnt; index++)
        {
            WdgM_AliveSupIDType seAliveId = seInfo->WdgMAliveSupCfg[index].WdgMAliveSupID;
            WdgM_AliveSupIDType cpAliveId = seInfo->WdgMCheckpointRelated[cpIndex].WdgM_AliveSupID;
            if (seAliveId == cpAliveId)
            {
                runDataPtr->WdgMAliveSupervisionInfo[seAliveId].WdgMAliveIndicationCounter++;
                break;
            }
        }
    }
}

WDGM_LOCAL void WdgM_ConfirmAliveSupResultEachSE(const WdgM_RuntimeType* runDataPtr, const WdgM_SECfgType* seInfo)
{
    uint16                 index;
    uint16                 aliveRefCycle;
    sint32                 aliveSupTempCnt;
    sint32                 aliveSupTempResult;
    sint32                 aliveSupTempConfigCnt;
    WdgM_AliveSupIDType    currentAliveId;
    uint16                 exceptIndication;
    uint8                  minMargin;
    uint8                  maxMargin;
    WdgM_AliveSupInfoType* currentAliveTab;

    if ((uint16)0u < seInfo->SEAliveCnt)
    {
        /*update certain AliveSup result*/
        for (index = 0u; index < seInfo->SEAliveCnt; index++)
        {
            aliveRefCycle    = seInfo->WdgMAliveSupCfg[index].WdgMSupRefCycle;
            exceptIndication = seInfo->WdgMAliveSupCfg[index].WdgMExpectedAliveSupIndication;
            minMargin        = seInfo->WdgMAliveSupCfg[index].WdgMMinMargin;
            maxMargin        = seInfo->WdgMAliveSupCfg[index].WdgMMaxMargin;
            /*Reach to the RefCycle, determine the related Alive Sup result*/
            currentAliveId  = seInfo->WdgMAliveSupCfg[index].WdgMAliveSupID;
            currentAliveTab = &runDataPtr->WdgMAliveSupervisionInfo[currentAliveId];

            if ((Std_ReturnType)E_OK == WdgM_ConfirmAliveSupResult(currentAliveTab, aliveRefCycle))
            {
                /*f(EAI,SRC)= SRC - EAI */
                aliveSupTempConfigCnt = (sint32)(aliveRefCycle) - (sint32)(exceptIndication);
                /*n(AI)-n(SC)*/
                aliveSupTempCnt = (sint32)(currentAliveTab->WdgMAliveIndicationCounter)
                                  - (sint32)(currentAliveTab->WdgMAliveCycleCounter);
                aliveSupTempResult = aliveSupTempConfigCnt + aliveSupTempCnt;

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
        for (index = 0u; index < seInfo->SEAliveCnt; index++)
        {
            currentAliveId  = seInfo->WdgMAliveSupCfg[index].WdgMAliveSupID;
            currentAliveTab = &runDataPtr->WdgMAliveSupervisionInfo[currentAliveId];
            if (WDGM_INCORRECT == currentAliveTab->WdgMAliveSupResult)
            {
                WdgM_SEInfoRelated[seInfo->WdgMSupervisedEntityID].WdgMEachSupResult[WDGM_ALIVE_SUP] = WDGM_INCORRECT;
                break;
            }
            else
            {
                WdgM_SEInfoRelated[seInfo->WdgMSupervisedEntityID].WdgMEachSupResult[WDGM_ALIVE_SUP] = WDGM_CORRECT;
            }
        }
    }
}

WDGM_LOCAL Std_ReturnType WdgM_ConfirmAliveSupResult(WdgM_AliveSupInfoType* currentAliveTab, uint16 refCycle)
{
    Std_ReturnType lReturnValue;

    currentAliveTab->WdgMAliveCycleCounter++;
    if (currentAliveTab->WdgMAliveCycleCounter >= refCycle)
    {
        /*The AliveSup Result base on the attachment*/
        lReturnValue = E_OK;
    }
    else
    {
        lReturnValue = E_NOT_OK;
    }
    return lReturnValue;
}
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
WDGM_LOCAL void WdgM_DoUpdateDeadlineSup(
    const WdgM_CPSupportDeadLineInfoType* cpSupportDeadlineInfo,
    WdgM_DeadlineSupInfoType*             currentDeadlineTab,
    const WdgM_SECfgType*                 seInfo,
    WdgM_DeadlineTimeType                 curWdgMTime)
{
    uint16                         index;
    WdgM_DeadlineTimeType          deadlineTimeGap = 0u;
    const WdgM_DeadlineSupCfgType* deadlineSupCfgInfo;

    if ((uint8)STD_ON
        == WdgM_GetWordBitState(cpSupportDeadlineInfo->WdgMCPSupportDeadlineGroup, (uint8)CP_DEADLINE_SUPPORT_START))
    {
        currentDeadlineTab->WdgMTimeStampStart = curWdgMTime;
    }

    /**Deadline Sup Stop**/
    if ((uint8)STD_ON
        == WdgM_GetWordBitState(cpSupportDeadlineInfo->WdgMCPSupportDeadlineGroup, (uint8)CP_DEADLINE_SUPPORT_STOP))
    {
        if ((WdgM_DeadlineTimeType)0uL != currentDeadlineTab->WdgMTimeStampStart)
        {
            for (index = 0u; index < seInfo->SEDealineCnt; index++)
            {
                if (seInfo->WdgMDeadlineSupCfg[index].WdgMDeadlineSupID == cpSupportDeadlineInfo->WdgM_DeadlineSupID)
                {
                    break;
                }
            }

            WdgM_SupervisedEntityIdType SEID       = seInfo->WdgMSupervisedEntityID;
            uint32                      resolution = WdgM_SupervisedEntityPcCfg[SEID].WdgMOsCounterResolution;

            if (curWdgMTime >= currentDeadlineTab->WdgMTimeStampStart)
            {
                deadlineTimeGap = (curWdgMTime - currentDeadlineTab->WdgMTimeStampStart) * resolution;
            }
            else
            {
                uint32 maxTickValue = WdgM_SupervisedEntityPcCfg[SEID].WdgMOsCounterMaxValue;
                deadlineTimeGap =
                    1uL + curWdgMTime + ((maxTickValue * resolution) - currentDeadlineTab->WdgMTimeStampStart);
            }
            deadlineSupCfgInfo = (const WdgM_DeadlineSupCfgType*)&seInfo->WdgMDeadlineSupCfg[index];
            WdgM_UpdateDeadlineSupResult(deadlineTimeGap, deadlineSupCfgInfo, currentDeadlineTab);
            currentDeadlineTab->WdgMTimeStampStart = 0u;
        }
        else
        {
            /* No action, if TimeStamp is 0*/
        }
    }
}

WDGM_LOCAL void WdgM_UpdateDeadlineSup(
    const WdgM_RuntimeType* runDataPtr,
    uint16                  cpIndex,
    const WdgM_SECfgType*   seInfo,
    WdgM_DeadlineTimeType   curWdgMTime)
{
    uint16                        cpSupportDeadlineIndex;
    const WdgM_CheckpointCfgType* checkpointCfg =
        (const WdgM_CheckpointCfgType*)&seInfo->WdgMCheckpointRelated[cpIndex];
    const WdgM_CPSupportDeadLineInfoType* cpSupportDeadlineInfo;
    WdgM_DeadlineSupInfoType*             currentDeadlineTab;

    if ((uint8)WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, (uint8)DEADLINE_SUPPORT) == STD_ON)
    {
        /******One Checkpoint may map several Deadline Sup********/
        for (cpSupportDeadlineIndex = 0u; cpSupportDeadlineIndex < checkpointCfg->WdgM_CPSupportDeadLineCnt;
             cpSupportDeadlineIndex++)
        {
            cpSupportDeadlineInfo = (const WdgM_CPSupportDeadLineInfoType*)&checkpointCfg
                                        ->WdgM_CPSupportDeadLineInfo[cpSupportDeadlineIndex];
            WdgM_DeadlineSupIDType currentDeadlineSupID = cpSupportDeadlineInfo->WdgM_DeadlineSupID;
            currentDeadlineTab = &runDataPtr->WdgMDeadlineSupervisionInfo[currentDeadlineSupID];

            if (WDGM_INCORRECT == currentDeadlineTab->WdgMDeadlineSupResult)
            {
                break;
            }
            else
            {
                /**Deadline Sup Start**/
                WdgM_DoUpdateDeadlineSup(cpSupportDeadlineInfo, currentDeadlineTab, seInfo, curWdgMTime);
            }
        }
    }
}

WDGM_LOCAL void WdgM_UpdateDeadlineSupResult(
    WdgM_DeadlineTimeType          timeGap,
    const WdgM_DeadlineSupCfgType* deadlineSupCfg,
    WdgM_DeadlineSupInfoType*      currentDeadlineTab)
{
    if ((((deadlineSupCfg->WdgMDeadlineMin) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
         <= (WdgM_DeadlineTimeFloatType)timeGap)
        && (((deadlineSupCfg->WdgMDeadlineMax) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
            >= (WdgM_DeadlineTimeFloatType)timeGap))
    {
        currentDeadlineTab->WdgMDeadlineSupResult = WDGM_CORRECT;
    }
    else
    {
        currentDeadlineTab->WdgMDeadlineSupResult = WDGM_INCORRECT;
    }
}

WDGM_LOCAL void WdgM_PerformDeadlineTimeoutDetection(const WdgM_RuntimeType* runDataPtr, const WdgM_SECfgType* seInfo)
{
    uint16                         index;
    uint32                         tempStartTime;
    WdgM_DeadlineTimeType          curWdgMTime;
    WdgM_DeadlineTimeType          deadlineTimeGap = 0u;
    const WdgM_DeadlineSupCfgType* deadlineSupCfgInfo;

    for (index = 0u; index < seInfo->SEDealineCnt; index++)
    {
        if (seInfo->WdgMDeadlineSupCfg[index].WdgMEnableTimeoutDetection)
        {
            deadlineSupCfgInfo = (const WdgM_DeadlineSupCfgType*)&seInfo->WdgMDeadlineSupCfg[index];
            WdgM_DeadlineSupIDType    currentDeadlineId  = seInfo->WdgMDeadlineSupCfg[index].WdgMDeadlineSupID;
            WdgM_DeadlineSupInfoType* currentDeadlineTab = &runDataPtr->WdgMDeadlineSupervisionInfo[currentDeadlineId];
            tempStartTime                                = currentDeadlineTab->WdgMTimeStampStart;
            if (0uL != tempStartTime)
            {
                curWdgMTime                      = tempStartTime;
                WdgM_SupervisedEntityIdType SEID = seInfo->WdgMSupervisedEntityID;
                (void)GetElapsedValue(WdgM_SupervisedEntityPcCfg[SEID].WdgMOsCounter, &curWdgMTime, &deadlineTimeGap);
                deadlineTimeGap = deadlineTimeGap * WdgM_SupervisedEntityPcCfg[SEID].WdgMOsCounterResolution;
                if (((deadlineSupCfgInfo->WdgMDeadlineMax) * (WdgM_DeadlineTimeFloatType)WDGM_TIME_RESOLUTION)
                    < ((WdgM_DeadlineTimeFloatType)deadlineTimeGap))
                {
                    currentDeadlineTab->WdgMDeadlineSupResult = WDGM_INCORRECT;
                    currentDeadlineTab->WdgMTimeStampStart    = 0u;
                }
                else
                {
                    /*MISRA C*/
                }
            }
        }
    }
}

WDGM_LOCAL void WdgM_ConfirmDeadlineSupResultEachSE(const WdgM_RuntimeType* runDataPtr, const WdgM_SECfgType* seInfo)
{
    uint16                          index;
    const WdgM_DeadlineSupCfgType*  deadlineSupCfgInfo;
    const WdgM_DeadlineSupInfoType* deadlineSupTab;
    WdgM_SupervisedEntityIdType     SEID;

    /*If Alive Sup is false, may jump the determination of the Deadline Sup*/
    if ((uint16)0u < seInfo->SEDealineCnt)
    {
        SEID = seInfo->WdgMSupervisedEntityID;

        for (index = 0u; index < seInfo->SEDealineCnt; index++)
        {
            deadlineSupCfgInfo = &seInfo->WdgMDeadlineSupCfg[index];
            deadlineSupTab     = &runDataPtr->WdgMDeadlineSupervisionInfo[deadlineSupCfgInfo->WdgMDeadlineSupID];
            if (deadlineSupTab->WdgMDeadlineSupResult == WDGM_INCORRECT)
            {
                /*one Deadline Sup incorrect*/
                WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_INCORRECT;
                break;
            }
        }
    }
}
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
WDGM_LOCAL void WdgM_UpdateIntLogSup(uint16 cpIndex, const WdgM_SECfgType* seInfo)
{
    uint16                        transitionIndex;
    uint16                        transitionMax;
    const WdgM_CheckpointCfgType* checkpointCfg =
        (const WdgM_CheckpointCfgType*)&seInfo->WdgMCheckpointRelated[cpIndex];
    WdgM_CheckpointIdType   intLastReachedCPID;
    WdgM_CheckpointIdType   intCurReachedCPID;
    WdgM_CheckpointIdType   intDestReachedCPID;
    WdgM_LogSupIDType       intLogId     = WdgM_SupervisedEntityPcCfg[seInfo->WdgMSupervisedEntityID].WdgMIntLogSupId;
    WdgM_IntLogSupInfoType* intLogSupTab = &WdgM_IntLogSupTab[intLogId];

    if (((uint8)WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, (uint8)INT_LOG_SUPPORT) == STD_ON)
        && (WDGM_CORRECT == intLogSupTab->WdgMIntLogSupResult)) /*Interlog Sup*/
    {
        if (WDGM_AF_FALSE == intLogSupTab->WdgMIntLogActiveFlag)
        {
            if ((uint8)WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, (uint8)LOG_SUP_SUPPORT_INIT) == STD_ON)
            {
                intLogSupTab->WdgMIntLastReachedCPID = checkpointCfg->WdgMCheckpointID;
                intLogSupTab->WdgMIntLogActiveFlag   = WDGM_AF_TRUE;
                intLogSupTab->WdgMIntLogSupResult    = WDGM_CORRECT;
            }
            else
            {
                intLogSupTab->WdgMIntLogSupResult = WDGM_INCORRECT;
            }
        }
        else
        {
            transitionMax = WdgM_SupervisedEntityPcCfg[seInfo->WdgMSupervisedEntityID].WdgMIntLogTransitionCnt;

            for (transitionIndex = 0u; transitionIndex < transitionMax; transitionIndex++)
            {
                intLastReachedCPID = intLogSupTab->WdgMIntLastReachedCPID;
                intCurReachedCPID  = WdgM_SupervisedEntityPcCfg[seInfo->WdgMSupervisedEntityID]
                                        .WdgMIntLogTransition[transitionIndex]
                                        .WdgMTransitionSourCPID;
                intDestReachedCPID = WdgM_SupervisedEntityPcCfg[seInfo->WdgMSupervisedEntityID]
                                         .WdgMIntLogTransition[transitionIndex]
                                         .WdgMTransitionDestCPID;
                if ((intLastReachedCPID == intCurReachedCPID)
                    && (checkpointCfg->WdgMCheckpointID == intDestReachedCPID))
                {
                    intLogSupTab->WdgMIntLogSupResult = WDGM_CORRECT;
                    break;
                }
                else
                {
                    intLogSupTab->WdgMIntLogSupResult = WDGM_INCORRECT;
                }
            }
            if (WDGM_CORRECT == intLogSupTab->WdgMIntLogSupResult)
            {
                intLogSupTab->WdgMIntLastReachedCPID = checkpointCfg->WdgMCheckpointID;
                if ((uint8)STD_ON
                    == WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, (uint8)LOG_SUP_SUPPORT_FINAL))
                {
                    intLogSupTab->WdgMIntLogActiveFlag   = WDGM_AF_FALSE;
                    intLogSupTab->WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
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
        /*Nothing*/
    }
}

/*Determine the Alive Sup Result*/
WDGM_LOCAL void WdgM_ConfirmIntLogSupResultEachSE(WdgM_SupervisedEntityIdType SEID)
{
    const WdgM_IntLogSupInfoType* intLogSupTab;
    WdgM_LogSupIDType             intLogId = WdgM_SupervisedEntityPcCfg[SEID].WdgMIntLogSupId;
    /*If Sup above is false, may jump the determination of the Internal log Sup*/

    intLogSupTab                                                = &WdgM_IntLogSupTab[intLogId];
    WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_INTLOG_SUP] = intLogSupTab->WdgMIntLogSupResult;
}
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
WDGM_LOCAL void WdgM_UpdateExtLogSupResult(
    WdgM_ExtLogSupInfoType*           currentExtLogSupTab,
    const WdgM_ExtLogSupCfgType*      currentExtLogSup,
    const WdgM_CheckpointIdType       CheckpointID,
    const WdgM_SupervisedEntityIdType SEID)
{
    uint16 transitionIndex;

    for (transitionIndex = 0u; transitionIndex < currentExtLogSup->WdgExtTransitionCnt; transitionIndex++)
    {
        if ((currentExtLogSupTab->WdgMExtLastReachedCPID
             == currentExtLogSup->WdgMExtTransition[transitionIndex].WdgMTransitionSourCPID)
            && (currentExtLogSupTab->WdgMExtLastReachedSEID
                == currentExtLogSup->WdgMExtTransition[transitionIndex].WdgMTransitionSourSEID))
        {
            if ((CheckpointID == currentExtLogSup->WdgMExtTransition[transitionIndex].WdgMTransitionDestCPID)
                && (SEID == currentExtLogSup->WdgMExtTransition[transitionIndex].WdgMTransitionDestSEID))
            {
                currentExtLogSupTab->WdgMExtLogSupResult = WDGM_CORRECT;
                break;
            }
            else
            {
                currentExtLogSupTab->WdgMExtLogSupResult = WDGM_INCORRECT;
            }
        }
    }
}

/* PRQA S 6030 ++ */ /* VL_MTR_WdgM_STMIF */
WDGM_LOCAL void WdgM_UpdateExtLogSup(
    const WdgM_RuntimeType* runDataPtr,
    uint16                  cpIndex,
    const WdgM_SECfgType*   seInfo,
    const WdgM_ModeCfgType* modeCfg)
/* PRQA S 6030 -- */
{
    uint16                        extLogSupId;
    const WdgM_CheckpointCfgType* checkpointCfg =
        (const WdgM_CheckpointCfgType*)&seInfo->WdgMCheckpointRelated[cpIndex];
    const WdgM_ExtLogSupCfgType* currentExtLogSup;
    WdgM_ExtLogSupInfoType*      currentExtLogSupTab;

    if ((uint8)WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, (uint8)EXT_LOG_SUPPORT) == STD_ON)
    {
        extLogSupId         = checkpointCfg->WdgMExtLogSupID;
        currentExtLogSup    = &modeCfg->WdgMExtLogSupRelated[extLogSupId];
        currentExtLogSupTab = &runDataPtr->WdgMExtLogicalSupervisionInfo[extLogSupId];
        if (currentExtLogSupTab->WdgMExtLogSupResult == WDGM_CORRECT)
        {
            if (currentExtLogSupTab->WdgMExtLogActiveFlag == WDGM_AF_FALSE)
            {
                if ((uint8)STD_ON
                    == WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, (uint8)LOG_SUP_SUPPORT_INIT))
                {
                    currentExtLogSupTab->WdgMExtLastReachedCPID = checkpointCfg->WdgMCheckpointID;
                    currentExtLogSupTab->WdgMExtLastReachedSEID = seInfo->WdgMSupervisedEntityID;
                    currentExtLogSupTab->WdgMExtLogActiveFlag   = WDGM_AF_TRUE;
                    currentExtLogSupTab->WdgMExtLogSupResult    = WDGM_CORRECT;
                }
                else
                {
                    currentExtLogSupTab->WdgMExtLogSupResult = WDGM_INCORRECT;
                }
            }
            else
            {
                WdgM_UpdateExtLogSupResult(
                    currentExtLogSupTab,
                    currentExtLogSup,
                    checkpointCfg->WdgMCheckpointID,
                    seInfo->WdgMSupervisedEntityID);

                if (WDGM_CORRECT == currentExtLogSupTab->WdgMExtLogSupResult)
                {
                    currentExtLogSupTab->WdgMExtLastReachedCPID = checkpointCfg->WdgMCheckpointID;
                    currentExtLogSupTab->WdgMExtLastReachedSEID = seInfo->WdgMSupervisedEntityID;

                    if ((uint8)STD_ON
                        == WdgM_GetWordBitState(checkpointCfg->WdgMSupSupportGroup, (uint8)LOG_SUP_SUPPORT_FINAL))
                    {
                        currentExtLogSupTab->WdgMExtLogActiveFlag   = WDGM_AF_FALSE;
                        currentExtLogSupTab->WdgMExtLastReachedSEID = WDGM_NO_SE_REACH;
                        currentExtLogSupTab->WdgMExtLastReachedCPID = WDGM_NO_CP_REACH;
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
#endif /*WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

/* PRQA S 2982,2983 ++ */ /* VL_WdgM_UsedBeforModify,VL_WdgM_2983 */
WDGM_LOCAL void WdgM_ClearExtlogStatus(const WdgM_RuntimeType* runDataPtr, const WdgM_ModeCfgType* oldModeCfg)
{
    uint16 oldIndex;
    uint16 oldIndexMax = oldModeCfg->ModeExtLogCnt;

    for (oldIndex = 0u; oldIndex < oldIndexMax; oldIndex++)
    {
        WdgM_LogSupIDType       extLogId     = oldModeCfg->WdgMExtLogSupRelated[oldIndex].WdgMExtLogSupID;
        WdgM_ExtLogSupInfoType* extLogSupTab = &runDataPtr->WdgMExtLogicalSupervisionInfo[extLogId];
        extLogSupTab->WdgMExtLogActiveFlag   = WDGM_AF_FALSE;
        extLogSupTab->WdgMExtLastReachedCPID = WDGM_NO_CP_REACH;
        extLogSupTab->WdgMExtLastReachedSEID = WDGM_NO_SE_REACH;
        extLogSupTab->WdgMExtLogSupResult    = WDGM_CORRECT;
    }
}
/* PRQA S 2982,2983 -- */

WDGM_LOCAL void WdgM_ConfirmExtLogSupResultEachSE(const WdgM_RuntimeType* runDataPtr, const WdgM_ModeCfgType* modeCfg)
{
    uint16                      index;
    uint16                      extLogIndex;
    const uint16                extLogIndexMax = modeCfg->ModeExtLogCnt;
    uint16                      seRelatedIndex;
    const uint16                indexMax = modeCfg->ModeSECnt;
    uint16                      seRelatedIndexMax;
    WdgM_SupervisedEntityIdType SEId;

    for (index = 0u; index < indexMax; index++) /*SE quantity*/
    {
        SEId = modeCfg->WdgMSERelated[index].WdgMSupervisedEntityID;
        for (extLogIndex = 0u; extLogIndex < extLogIndexMax; extLogIndex++) /*ExtLogSup quantity*/
        {
            seRelatedIndexMax = modeCfg->WdgMExtLogSupRelated[extLogIndex].SEIdRelatedCnt;
            /*Related SE quantity of each ExtLog Sup*/
            for (seRelatedIndex = 0u; seRelatedIndex < seRelatedIndexMax; seRelatedIndex++)
            {
                WdgM_LogSupIDType extLogId = modeCfg->WdgMExtLogSupRelated[extLogIndex].WdgMExtLogSupID;
                /*Find the SE related to the ExtLog Sup via SEID*/
                if ((SEId == modeCfg->WdgMExtLogSupRelated[extLogIndex].WdgMSEIdImpacted[seRelatedIndex])
                    && (WDGM_INCORRECT == runDataPtr->WdgMExtLogicalSupervisionInfo[extLogId].WdgMExtLogSupResult))
                {
                    /*One ExtLog Sup Error in this SE*/
                    break;
                }
            }
            if (seRelatedIndexMax > seRelatedIndex)
            {
                break;
            }
        }
        if (extLogIndexMax > extLogIndex)
        {
            WdgM_SEInfoRelated[SEId].WdgMEachSupResult[WDGM_EXTLOG_SUP] = WDGM_INCORRECT;
        }
        else
        {
            WdgM_SEInfoRelated[SEId].WdgMEachSupResult[WDGM_EXTLOG_SUP] = WDGM_CORRECT;
        }
    }
}

WDGM_LOCAL void WdgM_ClearNoUseAliveDeadlineStatus(
    const WdgM_RuntimeType* runDataPtr,
    const WdgM_SECfgType*   oldSE,
    const WdgM_SECfgType*   newSE)
{
    uint16 oldIndex;
    uint16 newIndex;
    uint16 innerCycleIndex = 0u;
    uint16 oldIndexMax     = 0u;
    uint16 newIndexMax     = 0u;

    oldIndexMax = oldSE->SEAliveCnt;
    newIndexMax = newSE->SEAliveCnt;
    for (oldIndex = 0u; oldIndex < oldIndexMax; oldIndex++)
    {
        /*AliveID sorts from same to big*/
        for (newIndex = innerCycleIndex; newIndex < newIndexMax; newIndex++)
        {
            if (oldSE->WdgMAliveSupCfg[oldIndex].WdgMAliveSupID == newSE->WdgMAliveSupCfg[newIndex].WdgMAliveSupID)
            {
                innerCycleIndex = newIndex + (uint16)1u;
                break;
            }
        }
        /* PRQA S 2982,2983 ++ */ /* VL_WdgM_UsedBeforModify,VL_WdgM_2983 */
        if (newIndex >= newIndexMax)
        {
            WdgM_AliveSupIDType    aliveId          = oldSE->WdgMAliveSupCfg[oldIndex].WdgMAliveSupID;
            WdgM_AliveSupInfoType* aliveSupTab      = &runDataPtr->WdgMAliveSupervisionInfo[aliveId];
            aliveSupTab->WdgMAliveCycleCounter      = 0u;
            aliveSupTab->WdgMAliveIndicationCounter = 0u;
            aliveSupTab->WdgMAliveSupResult         = WDGM_CORRECT;
        }
        /* PRQA S 2982,2983 -- */
    }

    oldIndexMax     = oldSE->SEDealineCnt;
    newIndexMax     = newSE->SEDealineCnt;
    innerCycleIndex = 0u;
    for (oldIndex = 0u; oldIndex < oldIndexMax; oldIndex++)
    {
        for (newIndex = innerCycleIndex; newIndex < newIndexMax; newIndex++)
        {
            if (oldSE->WdgMDeadlineSupCfg[oldIndex].WdgMDeadlineSupID
                == newSE->WdgMDeadlineSupCfg[newIndex].WdgMDeadlineSupID)
            {
                innerCycleIndex = newIndex + (uint16)1u;
                break;
            }
        }
        if (newIndex >= newIndexMax)
        {
            WdgM_DeadlineSupIDType    deadlineId     = oldSE->WdgMDeadlineSupCfg[oldIndex].WdgMDeadlineSupID;
            WdgM_DeadlineSupInfoType* deadlineSupTab = &runDataPtr->WdgMDeadlineSupervisionInfo[deadlineId];
            deadlineSupTab->WdgMTimeStampStart       = 0u;
            deadlineSupTab->WdgMDeadlineSupResult    = WDGM_CORRECT;
        }
    }
}

/* PRQA S 1503,1505,1532 ++ */ /* VL_QAC_NoUsedApi,VL_WdgM_1505,VL_QAC_OneFunRef */
void WdgM_MainFunction(void)
{
    uint16                  seIndex;
    const WdgM_SECfgType*   seInfo;
    const WdgM_ConfigType*  ConfigPtr;
    const WdgM_RuntimeType* runDataPtr = WdgM_GetRunDataPtr();

    if ((runDataPtr != NULL_PTR) && (WdgM_GetConfigPtr(&ConfigPtr) == E_OK))
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        if (*runDataPtr->WdgMGlobalStatus == WDGM_GLOBAL_STATUS_DEACTIVATED)
        {
            (void)Det_ReportError(WDGM_MODULE_ID, WDGM_INSTANCE_ID, WDGM_MAINFUNCTION_ID, WDGM_E_UNINIT);
        }
        else
#endif
        {
            WdgM_ModeType           modeId  = runDataPtr->WdgMModeInfo->WdgMModeCurrentID;
            const WdgM_ModeCfgType* modeCfg = &ConfigPtr->WdgMMode[modeId];
            runDataPtr->WdgMModeInfo->WdgMMainFunctionRunTick++;
            /* If the execution cycle of the main function is reached */
            if (runDataPtr->WdgMModeInfo->WdgMMainFunctionRunTick == modeCfg->WdgMMainFunctionExceptTick)
            {
                if (runDataPtr->WdgMModeInfo->WdgMMainFunctionActivityFlag == WDGM_AF_FALSE)
                {
#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
                    WdgM_DemSetEvent(runDataPtr, ConfigPtr);
#endif
                    runDataPtr->WdgMModeInfo->WdgMMainFunctionActivityFlag = WDGM_AF_TRUE;
                }

                /*Determine the External Log Sup status of each SE*/
                WdgM_ConfirmExtLogSupResultEachSE(runDataPtr, modeCfg);

                for (seIndex = 0u; seIndex < modeCfg->ModeSECnt; seIndex++)
                {
                    seInfo = (const WdgM_SECfgType*)&modeCfg->WdgMSERelated[seIndex];

                    if ((WDGM_LOCAL_STATUS_OK == WdgM_SEInfoRelated[seInfo->WdgMSupervisedEntityID].WdgMLocalStatus)
                        || (WDGM_LOCAL_STATUS_FAILED
                            == WdgM_SEInfoRelated[seInfo->WdgMSupervisedEntityID].WdgMLocalStatus))
                    {
                        WdgM_ConfirmSupervisionResult(runDataPtr, seInfo);
                    }
                }
                /*Determine the Global Sup status*/
                WdgM_ConfirmGlobalStatus();

#if (WDGM_COMBINATION_USED == STD_ON)
                if (ConfigPtr->WdgMSupportCombinationID != WDGM_COMBINATITON_INVALIED_ID)
                {
                    WdgM_CombinationResultHandle(runDataPtr, ConfigPtr);
                }
                else
                {
                    WdgM_SetTrigger(
                        ConfigPtr,
                        runDataPtr->WdgMGlobalStatus,
                        runDataPtr->WdgMModeInfo->WdgMModeCurrentID);
                }
#else
                WdgM_SetTrigger(ConfigPtr, runDataPtr->WdgMGlobalStatus, runDataPtr->WdgMModeInfo->WdgMModeCurrentID);
#endif
                runDataPtr->WdgMModeInfo->WdgMMainFunctionRunTick = 0u;
            }
        }
    }
}
/* PRQA S 1503,1505,1532 -- */

WDGM_LOCAL void WdgM_ConfirmSupervisionResult(const WdgM_RuntimeType* runDataPtr, const WdgM_SECfgType* seInfo)
{
#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
    /* Determine the Alive Sup Result*/
    WdgM_ConfirmAliveSupResultEachSE(runDataPtr, seInfo);
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
    /* Deadline Timeout Detection */
    WdgM_PerformDeadlineTimeoutDetection(runDataPtr, seInfo);
    /* Determine the Alive Sup Result*/
    WdgM_ConfirmDeadlineSupResultEachSE(runDataPtr, seInfo);
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    /* Determine the Internal Logical Sup Result*/
    WdgM_ConfirmIntLogSupResultEachSE(seInfo->WdgMSupervisedEntityID);
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/

    /* Determine the Local Sup status of each SE*/
    WdgM_ConfirmSELocalStatus(seInfo);
}

#if (WDGM_COMBINATION_USED == STD_ON)
WDGM_LOCAL void WdgM_CombinationMasterDo(
    const WdgM_MultipleCoresCombinationType* currentCombinationPtr,
    const WdgM_GlobalStatusType*             currentGlobalResult,
    const WdgM_ModeCfgType*                  modeCfg)
{
    uint16 triggerIndex;
    uint8  ret = 0u;
    (*currentCombinationPtr->WdgMCombinationSyncCycleTab)++;
    if (currentCombinationPtr->WdgMCombinationSyncCycle == *currentCombinationPtr->WdgMCombinationSyncCycleTab)
    {
        if (*currentGlobalResult == WDGM_GLOBAL_STATUS_STOPPED)
        {
            ret = WDGM_COMBINATION_NOT_OK;
        }
        else
        {
            ret = WdgM_ComputeCombinationStatus(currentCombinationPtr);
        }
        (*currentCombinationPtr->WdgMCombinationSyncCycleTab) = 0u;
    }
    switch (ret)
    {
    case WDGM_COMBINATION_OK:
        for (triggerIndex = 0u; triggerIndex < modeCfg->ModeTriggerCnt; triggerIndex++)
        {
            if (WDGIF_OFF_MODE != modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
            {
                /*Trigger value is 0*/
                WdgIf_SetTriggerCondition(
                    modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogDevice,
                    modeCfg->WdgMTriggerRelated[triggerIndex].WdgMTriggerConditionValue);
            }
        }
        break;
    case WDGM_COMBINATION_NOT_OK:
        /* code */
        for (triggerIndex = 0u; triggerIndex < modeCfg->ModeTriggerCnt; triggerIndex++)
        {
            if (WDGIF_OFF_MODE != modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
            {
                /*Trigger value is 0*/
                WdgIf_SetTriggerCondition(
                    modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogDevice,
                    WDGM_TRIGGER_CONDITION_STOP);
            }
        }
        break;
    default:
        break;
    }
}

WDGM_LOCAL void WdgM_CombinationSlaveDo(
    const WdgM_MultipleCoresCombinationType* currentCombinationPtr,
    const WdgM_GlobalStatusType*             currentGlobalResult,
    uint8                                    slaveIndex)
{
    if (*currentGlobalResult == WDGM_GLOBAL_STATUS_STOPPED)
    {
        currentCombinationPtr->WdgMCombinationSlaveStatus[slaveIndex] = WDGM_COMBINATION_NOT_OK;
    }
}

WDGM_LOCAL void WdgM_CombinationResultHandle(const WdgM_RuntimeType* runDataPtr, const WdgM_ConfigType* ConfigPtr)
{
    uint8          combinationID;
    Std_ReturnType lRet = E_NOT_OK;

    const WdgM_MultipleCoresCombinationType* currentCombinationPtr;
    const WdgM_ModeCfgType*                  modeCfg;

    modeCfg       = &ConfigPtr->WdgMMode[runDataPtr->WdgMModeInfo->WdgMModeCurrentID];
    combinationID = ConfigPtr->WdgMSupportCombinationID;
    if ((combinationID != WDGM_COMBINATITON_INVALIED_ID) && (combinationID <= WDGM_MAX_COMBINATION_NUM))
    {
        currentCombinationPtr = &WdgM_CombinationCfgPtr[combinationID];

        /*Master Core*/
        if (ConfigPtr->WdgMMasterOrSlave == WDGM_COMBINATION_MASTER)
        {
            WdgM_CombinationMasterDo(currentCombinationPtr, runDataPtr->WdgMGlobalStatus, modeCfg);
        }
        /*Slave Core*/
        else if (ConfigPtr->WdgMMasterOrSlave == WDGM_COMBINATION_SLAVE)
        {
            uint8 slaveIndex;
            lRet = WdgM_GetCombinationSlaveStatusIndex(ConfigPtr, &slaveIndex);
            if (lRet == E_OK)
            {
                WdgM_CombinationSlaveDo(currentCombinationPtr, runDataPtr->WdgMGlobalStatus, slaveIndex);
            }
            /* found the index*/
        }
        else
        {
            /*MISRAC*/
        }
    }
}
#endif

WDGM_LOCAL void WdgM_WriteFirstExpiredSEID(uint16 SEID)
{
    uint8*                       firstExpiredSEIDSaved;
    WdgM_SupervisedEntityIdType* firstExpiredSEID;
    WdgM_SupervisedEntityIdType* firstExpiredSEIDInv;

    if ((E_OK == WdgM_GetFirstExpiredSEIDSaved(&firstExpiredSEIDSaved))
        && (E_OK == WdgM_GetFirstExpiredSEIDPtr(&firstExpiredSEID, &firstExpiredSEIDInv)))
    {
        if (E_NOT_OK == *firstExpiredSEIDSaved)
        {
            *firstExpiredSEID      = SEID;
            *firstExpiredSEIDInv   = ~(SEID);
            *firstExpiredSEIDSaved = E_OK;
        }
    }
}

WDGM_LOCAL Std_ReturnType WdgM_ConfirmSELocalStatusHandleOK(uint16 SEID, const uint8 aliveFailedTol)
{
    Std_ReturnType lRet = E_OK;

    if ((WDGM_CORRECT == WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_INTLOG_SUP])
        && (WDGM_CORRECT == WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_EXTLOG_SUP])
        && (WDGM_CORRECT == WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_DEADLINE_SUP]))
    {
        if (WDGM_INCORRECT == WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_ALIVE_SUP])
        {
            if ((uint8)0u < aliveFailedTol)
            {
                WdgM_SEInfoRelated[SEID].WdgMFailedAliveSupCounter++;
                WdgM_SEInfoRelated[SEID].WdgMLocalStatus = WDGM_LOCAL_STATUS_FAILED;
#if (WDGM_RTE_ENABLED == STD_ON)
                Rte_PortHandle_WdgM_LocalMode_P ph = Rte_Ports_WdgM_LocalMode_P();
                ph[SEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_FAILED);
#endif
                /*DD_2_038*/
            }
            else
            {
                lRet = E_NOT_OK;
            }
        }
    }
    else
    {
        lRet = E_NOT_OK;
    }

    return lRet;
}

WDGM_LOCAL Std_ReturnType WdgM_ConfirmSELocalStatusHandleFAILED(uint16 SEID, const uint8 aliveFailedTol)
{
    Std_ReturnType lRet = E_OK;

    if ((WDGM_INCORRECT == WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_INTLOG_SUP])
        || (WDGM_INCORRECT == WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_EXTLOG_SUP])
        || (WDGM_INCORRECT == WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_DEADLINE_SUP]))
    {
        /*Failed  to Expired*/
        lRet = E_NOT_OK;
    }
    else if (WDGM_INCORRECT == WdgM_SEInfoRelated[SEID].WdgMEachSupResult[WDGM_ALIVE_SUP])
    {
        WdgM_SEInfoRelated[SEID].WdgMFailedAliveSupCounter++;
        if (aliveFailedTol < WdgM_SEInfoRelated[SEID].WdgMFailedAliveSupCounter)
        {
            lRet = E_NOT_OK;
        }
    }
    else
    {
        if ((uint8)1u >= WdgM_SEInfoRelated[SEID].WdgMFailedAliveSupCounter)
        {
            WdgM_SEInfoRelated[SEID].WdgMFailedAliveSupCounter = 0u;
            WdgM_SEInfoRelated[SEID].WdgMLocalStatus           = WDGM_LOCAL_STATUS_OK;
#if (WDGM_RTE_ENABLED == STD_ON)
            Rte_PortHandle_WdgM_LocalMode_P ph = Rte_Ports_WdgM_LocalMode_P();
            ph[SEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_OK);
#endif
        }
        else
        {
            /* Reamin the Failed, ErrorCnt--*/
            WdgM_SEInfoRelated[SEID].WdgMFailedAliveSupCounter--;
        }
    }
    return lRet;
}

WDGM_LOCAL void WdgM_ConfirmSELocalStatus(const WdgM_SECfgType* seInfo)
{
    Std_ReturnType              lRet = E_OK;
    WdgM_SupervisedEntityIdType SEID;
    const uint8                 aliveFailedTol = seInfo->WdgMFailedAliveSupervisionRefCycleTol;

    SEID = seInfo->WdgMSupervisedEntityID;
    if (WDGM_LOCAL_STATUS_OK == WdgM_SEInfoRelated[SEID].WdgMLocalStatus)
    {
        lRet = WdgM_ConfirmSELocalStatusHandleOK(SEID, aliveFailedTol);
    }
    else if (WDGM_LOCAL_STATUS_FAILED == WdgM_SEInfoRelated[SEID].WdgMLocalStatus)
    {
        lRet = WdgM_ConfirmSELocalStatusHandleFAILED(SEID, aliveFailedTol);
    }
    else
    {
        /*MISRA-C*/
    }
    if ((Std_ReturnType)E_NOT_OK == lRet)
    {
        WdgM_SEInfoRelated[SEID].WdgMLocalStatus = WDGM_LOCAL_STATUS_EXPIRED;
#if (WDGM_RTE_ENABLED == STD_ON)
        Rte_PortHandle_WdgM_LocalMode_P ph = Rte_Ports_WdgM_LocalMode_P();
        ph[SEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_EXPIRED);
#endif
        SchM_Enter_WdgM_Exclusive();
        WdgM_WriteFirstExpiredSEID(SEID);
        SchM_Exit_WdgM_Exclusive();
    }
}

#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
WDGM_LOCAL void WdgM_DemSetEvent(WdgM_RuntimeType const* runDataPtr, const WdgM_ConfigType* ConfigPtr)
{
    if (ConfigPtr->WdgMDemEventID != WDGM_INVALIED_DEM_ID)
    {
        if (*runDataPtr->WdgMDemStatus == WDGM_DEM_REPORT_FAILED)
        {
            (void)Dem_SetEventStatus(ConfigPtr->WdgMDemEventID, DEM_EVENT_STATUS_FAILED);
            *runDataPtr->WdgMDemStatus = WDGM_DEM_REPORT_PASSED;
        }
        else if (*runDataPtr->WdgMDemStatus == WDGM_DEM_REPORT_PASSED)
        {
            (void)Dem_SetEventStatus(ConfigPtr->WdgMDemEventID, DEM_EVENT_STATUS_PASSED);
            *runDataPtr->WdgMDemStatus = WDGM_DEM_REPORT_NONE;
        }
        else
        {
            /*MISRA-C*/
        }
    }
}
#endif /*(WDGM_E_SUPERVISION_ENABLE == STD_ON*/

/* PRQA S 6030 ++ */ /* VL_MTR_WdgM_STMIF */
WDGM_LOCAL void WdgM_ConfirmGlobalStatus(void)
/* PRQA S 6030 -- */
{
    WdgM_SupervisedEntityIdType SEID;
    uint16                      seIndex;
    const WdgM_ConfigType*      ConfigPtr;
    const WdgM_ModeCfgType*     modeCfg;
    Std_ReturnType              seExpiredFlag = E_NOT_OK;
    Std_ReturnType              seFailedFlag  = E_NOT_OK;
    const WdgM_RuntimeType*     runDataPtr    = WdgM_GetRunDataPtr();

#if (WDGM_RTE_ENABLED == STD_ON)
    uint8 coreIndex;
    (void)WdgM_GetCoreIndex(&coreIndex);
    Rte_PortHandle_WdgM_GlobalMode_P phGlobal = Rte_Ports_WdgM_GlobalMode_P();
#endif

    if (runDataPtr != NULL_PTR)
    {
        if ((*runDataPtr->WdgMGlobalStatus != WDGM_GLOBAL_STATUS_STOPPED)
            && (*runDataPtr->WdgMGlobalStatus != WDGM_GLOBAL_STATUS_DEACTIVATED))
        {
            if (WdgM_GetConfigPtr(&ConfigPtr) == E_OK)
            {
                modeCfg = &ConfigPtr->WdgMMode[runDataPtr->WdgMModeInfo->WdgMModeCurrentID];
                for (seIndex = 0u; seIndex < modeCfg->ModeSECnt; seIndex++)
                {
                    SEID = modeCfg->WdgMSERelated[seIndex].WdgMSupervisedEntityID;
                    if (WDGM_LOCAL_STATUS_EXPIRED == WdgM_SEInfoRelated[SEID].WdgMLocalStatus)
                    {
                        seExpiredFlag = E_OK;
                        break;
                    }
                    if (WDGM_LOCAL_STATUS_FAILED == WdgM_SEInfoRelated[SEID].WdgMLocalStatus)
                    {
                        seFailedFlag = E_OK;
                    }
                }
                if ((Std_ReturnType)E_OK == seExpiredFlag)
                {
                    runDataPtr->WdgMModeInfo->WdgMExpiredSupTolCounter++;
                    if ((runDataPtr->WdgMModeInfo->WdgMExpiredSupTolCounter > modeCfg->WdgMExpiredSupervisionCycleTol))
                    {
                        *runDataPtr->WdgMGlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
#if (WDGM_E_SUPERVISION_ENABLE == STD_ON)
                        SchM_Enter_WdgM_Exclusive();
                        *runDataPtr->WdgMDemStatus = WDGM_DEM_REPORT_FAILED;
                        SchM_Exit_WdgM_Exclusive();
#endif
#if (WDGM_RTE_ENABLED == STD_ON)
                        phGlobal[coreIndex].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_STOPPED);
#endif
#if (WDGM_IMMEDIATE_RESET == STD_ON)
                        WdgM_StopStatusErrorHanding();
#endif
                        runDataPtr->WdgMModeInfo->WdgMExpiredSupTolCounter = 0u;
                    }
                    else
                    {
                        *runDataPtr->WdgMGlobalStatus = WDGM_GLOBAL_STATUS_EXPIRED;
#if (WDGM_RTE_ENABLED == STD_ON)
                        phGlobal[coreIndex].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_EXPIRED);
#endif
                    }
                }
                else if ((Std_ReturnType)E_OK == seFailedFlag)
                {
                    *runDataPtr->WdgMGlobalStatus = WDGM_GLOBAL_STATUS_FAILED;
#if (WDGM_RTE_ENABLED == STD_ON)
                    phGlobal[coreIndex].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_FAILED);
#endif
                }
                else
                {
                    *runDataPtr->WdgMGlobalStatus = WDGM_GLOBAL_STATUS_OK;
#if (WDGM_RTE_ENABLED == STD_ON)
                    phGlobal[coreIndex].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_OK);
#endif
                }
            }
        }
    }
}

#if (WDGM_IMMEDIATE_RESET == STD_ON)
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
#endif /*WDGM_IMMEDIATE_RESET == STD_ON*/

/* PRQA S 3219,2962 ++ */ /* VL_WdgM_3219,VL_WdgM_2962 */
WDGM_LOCAL void WdgM_SetTrigger(
    const WdgM_ConfigType*       ConfigPtr,
    const WdgM_GlobalStatusType* currentGlobalResult,
    WdgM_ModeType                modeId)
{
    uint8                   triggerIndex;
    const WdgM_ModeCfgType* modeCfg;

    modeCfg = &ConfigPtr->WdgMMode[modeId];

    if ((WDGM_GLOBAL_STATUS_OK == *currentGlobalResult) || (WDGM_GLOBAL_STATUS_FAILED == *currentGlobalResult)
        || (WDGM_GLOBAL_STATUS_EXPIRED == *currentGlobalResult))
    {
        for (triggerIndex = 0u; triggerIndex < modeCfg->ModeTriggerCnt; triggerIndex++)
        {
            if (WDGIF_OFF_MODE != modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
            {
                WdgIf_SetTriggerCondition(
                    modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogDevice,
                    modeCfg->WdgMTriggerRelated[triggerIndex].WdgMTriggerConditionValue);
            }
        }
    }
    else if (WDGM_GLOBAL_STATUS_STOPPED == *currentGlobalResult)
    {
        for (triggerIndex = 0u; triggerIndex < modeCfg->ModeTriggerCnt; triggerIndex++)
        {
            if (WDGIF_OFF_MODE != modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
            {
                WdgIf_SetTriggerCondition(
                    modeCfg->WdgMTriggerRelated[triggerIndex].WdgMWatchdogDevice,
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

WDGM_LOCAL Std_ReturnType
    WdgM_GetSECheckpointIndex(WdgM_CheckpointIdType CPID, const WdgM_SECfgType* seInfo, uint16* cpTabIndex)
{
    Std_ReturnType              lReturnValue = E_NOT_OK;
    uint16                      cpIndex;
    WdgM_SupervisedEntityIdType SEID = seInfo->WdgMSupervisedEntityID;

    for (cpIndex = 0u; cpIndex < WdgM_SupervisedEntityPcCfg[SEID].SECPCnt; cpIndex++)
    {
        if (seInfo->WdgMCheckpointRelated[cpIndex].WdgMCheckpointID == CPID)
        {
            *cpTabIndex  = cpIndex;
            lReturnValue = E_OK;
            break;
        }
    }
    return lReturnValue;
}

WDGM_LOCAL void WdgM_InitRelatedSETab(const WdgM_ConfigType* ConfigPtr)
{
    uint16 index;
    uint16 seIndex;

    for (index = 0u; index < ConfigPtr->WdgMConfigRelatedSECnt; index++)
    {
        seIndex                                                          = ConfigPtr->WdgMConfigRelatedSEID[index];
        WdgM_SEInfoRelated[seIndex].WdgMLocalStatus                      = WDGM_LOCAL_STATUS_DEACTIVATED;
        WdgM_SEInfoRelated[seIndex].WdgMFailedAliveSupCounter            = 0u;
        WdgM_SEInfoRelated[seIndex].WdgMEachSupResult[WDGM_ALIVE_SUP]    = WDGM_CORRECT;
        WdgM_SEInfoRelated[seIndex].WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_CORRECT;
        WdgM_SEInfoRelated[seIndex].WdgMEachSupResult[WDGM_INTLOG_SUP]   = WDGM_CORRECT;
        WdgM_SEInfoRelated[seIndex].WdgMEachSupResult[WDGM_EXTLOG_SUP]   = WDGM_CORRECT;
    }
}

WDGM_LOCAL void WdgM_InitLocalSupStatus(const WdgM_RuntimeType* runDataPtr, const WdgM_ModeCfgType* modeCfg)
{
    uint16 index = 0u;

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    for (index = 0u; index < modeCfg->ModeSECnt; index++)
    {
        WdgM_SupervisedEntityIdType seid     = modeCfg->WdgMSERelated[index].WdgMSupervisedEntityID;
        WdgM_LogSupIDType           intLogId = WdgM_SupervisedEntityPcCfg[seid].WdgMIntLogSupId;
        /*Do internal logical supervision init*/
        WdgM_IntLogSupTab[intLogId].WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
        WdgM_IntLogSupTab[intLogId].WdgMIntLogActiveFlag   = WDGM_AF_FALSE;
        WdgM_IntLogSupTab[intLogId].WdgMIntLogSupResult    = WDGM_CORRECT;
    }
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE*/

#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
    for (index = 0u; index < runDataPtr->WdgMTabCfg->WdgM_AliveTabMaxCnt; index++)
    {
        runDataPtr->WdgMAliveSupervisionInfo[index].WdgMAliveCycleCounter      = 0u;
        runDataPtr->WdgMAliveSupervisionInfo[index].WdgMAliveIndicationCounter = 0u;
        runDataPtr->WdgMAliveSupervisionInfo[index].WdgMAliveSupResult         = WDGM_CORRECT;
    }
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
    for (index = 0u; index < runDataPtr->WdgMTabCfg->WdgM_DeadlineTabMaxCnt; index++)
    {
        runDataPtr->WdgMDeadlineSupervisionInfo[index].WdgMTimeStampStart    = 0u;
        runDataPtr->WdgMDeadlineSupervisionInfo[index].WdgMDeadlineSupResult = WDGM_CORRECT;
    }
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    for (index = 0u; index < runDataPtr->WdgMTabCfg->WdgM_ExternalTabMaxCnt; index++)
    {
        runDataPtr->WdgMExtLogicalSupervisionInfo[index].WdgMExtLastReachedCPID = WDGM_NO_CP_REACH;
        runDataPtr->WdgMExtLogicalSupervisionInfo[index].WdgMExtLastReachedSEID = WDGM_NO_SE_REACH;
        runDataPtr->WdgMExtLogicalSupervisionInfo[index].WdgMExtLogActiveFlag   = WDGM_AF_FALSE;
        runDataPtr->WdgMExtLogicalSupervisionInfo[index].WdgMExtLogSupResult    = WDGM_CORRECT;
    }
#endif /*WDGM_EXT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/
}

WDGM_LOCAL void WdgM_InitSEStatus(const WdgM_ModeCfgType* modeCfg)
{
    uint16                seIndex;
    const WdgM_SECfgType* seInfo;

    for (seIndex = 0u; seIndex < modeCfg->ModeSECnt; seIndex++)
    {
        seInfo = (const WdgM_SECfgType*)&modeCfg->WdgMSERelated[seIndex];
        WdgM_SEInfoRelated[seInfo->WdgMSupervisedEntityID].WdgMLocalStatus = WDGM_LOCAL_STATUS_OK;
    }
}

WDGM_LOCAL void WdgM_DeinitSEStatus(const WdgM_ModeCfgType* modeCfg)
{
    uint16                seIndex;
    const WdgM_SECfgType* seInfo;

    for (seIndex = 0u; seIndex < modeCfg->ModeSECnt; seIndex++)
    {
        seInfo = (const WdgM_SECfgType*)&modeCfg->WdgMSERelated[seIndex];
        WdgM_SEInfoRelated[seInfo->WdgMSupervisedEntityID].WdgMLocalStatus = WDGM_LOCAL_STATUS_DEACTIVATED;
    }
}

WDGM_LOCAL Std_ReturnType
    WdgM_GetModeSEIndex(WdgM_SupervisedEntityIdType SEID, uint16* modeSETabIndex, const WdgM_ModeCfgType* modeCfg)
{
    uint16         seIndex;
    Std_ReturnType lReturnValue = E_NOT_OK;

    for (seIndex = 0u; seIndex < modeCfg->ModeSECnt; seIndex++)
    {
        if (modeCfg->WdgMSERelated[seIndex].WdgMSupervisedEntityID == SEID)
        {
            lReturnValue    = E_OK;
            *modeSETabIndex = seIndex;
            break;
        }
    }
    return lReturnValue;
}

/* PRQA S 5017, 6010, 6080 ++ */ /*VL_WdgM_5017, VL_MTR_WdgM_STCYC, VL_MTR_WdgM_STPTH */
WDGM_LOCAL void WdgM_ChangeSEStatus(
    const WdgM_RuntimeType* runDataPtr,
    const WdgM_ModeCfgType* oldModeCfg,
    const WdgM_ModeCfgType* newModeCfg)
{
    uint16                      innerCycleIndex = 0u;
    uint16                      newIndexMax     = newModeCfg->ModeSECnt;
    uint16                      oldIndexMax     = oldModeCfg->ModeSECnt;
    Std_ReturnType              alreadyActive   = E_NOT_OK;
    WdgM_SupervisedEntityIdType oldSEID;
    uint16                      oldIndex;
    WdgM_SupervisedEntityIdType newSEID;
    uint16                      newIndex;

    /* temporal variable for previous SE information  */
    /* PRQA S 0686 ++ */ /*VL_WdgM_0686*/
    WdgM_LocalStatusType oldSEStatus[WDGM_SE_NUM] = {WDGM_LOCAL_STATUS_OK};
    Std_ReturnType       newModeDeal[WDGM_SE_NUM] = {E_OK};
    /* PRQA S 0686 -- */

    for (oldIndex = 0u; oldIndex < oldIndexMax; oldIndex++)
    {
        oldSEID = oldModeCfg->WdgMSERelated[oldIndex].WdgMSupervisedEntityID;
        /*Save the old SE in temporary variable*/
        oldSEStatus[oldSEID] = WdgM_SEInfoRelated[oldSEID].WdgMLocalStatus;
    }

    for (newIndex = 0u; newIndex < newIndexMax; newIndex++)
    {
        newSEID = newModeCfg->WdgMSERelated[newIndex].WdgMSupervisedEntityID;
        /* Old Active != New Active, the new SE should be deault OK*/
        WdgM_SEInfoRelated[newSEID].WdgMLocalStatus           = WDGM_LOCAL_STATUS_OK;
        WdgM_SEInfoRelated[newSEID].WdgMFailedAliveSupCounter = 0u; /*SWS_WdgM_00209*/
#if (WDGM_RTE_ENABLED == STD_ON)
        Rte_PortHandle_WdgM_LocalMode_P ph = Rte_Ports_WdgM_LocalMode_P();
        ph[newSEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_OK);
#endif /*WDGM_RTE_ENABLED == STD_ON*/
        newModeDeal[newSEID] = E_NOT_OK;
    }

    /*SE should sort from small to large in config*/
    for (oldIndex = 0u; oldIndex < oldIndexMax; oldIndex++)
    {
        alreadyActive = E_NOT_OK;
        for (newIndex = innerCycleIndex; newIndex < newIndexMax; newIndex++)
        {
            newSEID = newModeCfg->WdgMSERelated[newIndex].WdgMSupervisedEntityID;

            if (oldModeCfg->WdgMSERelated[oldIndex].WdgMSupervisedEntityID
                == newModeCfg->WdgMSERelated[newIndex].WdgMSupervisedEntityID) /*Both Mode Active */
            {
                WdgM_SEInfoRelated[newSEID].WdgMLocalStatus = oldSEStatus[newSEID];
#if (WDGM_RTE_ENABLED == STD_ON)
                Rte_PortHandle_WdgM_LocalMode_P ph = Rte_Ports_WdgM_LocalMode_P();
                ph[newSEID].Switch_currentMode(oldSEStatus[newSEID]);
#endif /*WDGM_RTE_ENABLED == STD_ON*/
                WdgM_ClearNoUseAliveDeadlineStatus(
                    runDataPtr,
                    (const WdgM_SECfgType*)&oldModeCfg->WdgMSERelated[oldIndex],
                    (const WdgM_SECfgType*)&newModeCfg->WdgMSERelated[newIndex]);
                innerCycleIndex = newIndex + (uint16)1;
                /*WdgMLocalStatusSupervisedEntityRef already active in old mode */
                newModeDeal[newSEID] = E_OK;
                alreadyActive        = E_OK;
                break;
            }
        }
        if ((Std_ReturnType)E_NOT_OK == alreadyActive)
        {
            oldSEID                                     = oldModeCfg->WdgMSERelated[oldIndex].WdgMSupervisedEntityID;
            WdgM_SEInfoRelated[oldSEID].WdgMLocalStatus = WDGM_LOCAL_STATUS_DEACTIVATED;
#if (WDGM_RTE_ENABLED == STD_ON)
            Rte_PortHandle_WdgM_LocalMode_P ph = Rte_Ports_WdgM_LocalMode_P();
            ph[oldSEID].Switch_currentMode(RTE_MODE_WdgM_Mode_SUPERVISION_DEACTIVATED);
#endif /*WDGM_RTE_ENABLED == STD_ON*/
            WdgM_SEDeactivateInternal(runDataPtr, oldModeCfg, oldIndex, oldSEID);
        }
    }

    /* Set the status of residual SE without in old mode but in new mode */
    for (newIndex = 0u; newIndex < WDGM_SE_NUM; newIndex++)
    {
        if (newModeDeal[newIndex] == E_NOT_OK)
        {
            uint16 seIndex;
            if (WdgM_GetModeSEIndex(newIndex, &seIndex, newModeCfg) == E_OK)
            {
                /* PRQA S 2982,2983 ++ */ /* VL_WdgM_UsedBeforModify,VL_WdgM_2983 */
#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
                if (newModeCfg->WdgMSERelated[seIndex].SEAliveCnt > 0u)
                {
                    WdgM_AliveSupIDType    aliveId = newModeCfg->WdgMSERelated[seIndex].WdgMAliveSupCfg->WdgMAliveSupID;
                    WdgM_AliveSupInfoType* aliveSupTab      = &runDataPtr->WdgMAliveSupervisionInfo[aliveId];
                    aliveSupTab->WdgMAliveCycleCounter      = 0u;
                    aliveSupTab->WdgMAliveIndicationCounter = 0u;
                    aliveSupTab->WdgMAliveSupResult         = WDGM_CORRECT;
                }
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
                if (newModeCfg->WdgMSERelated[seIndex].SEDealineCnt > 0u)
                {
                    WdgM_DeadlineSupIDType deadlineId =
                        newModeCfg->WdgMSERelated[seIndex].WdgMDeadlineSupCfg->WdgMDeadlineSupID;
                    WdgM_DeadlineSupInfoType* deadlineSupTab = &runDataPtr->WdgMDeadlineSupervisionInfo[deadlineId];
                    deadlineSupTab->WdgMTimeStampStart       = 0u;
                    deadlineSupTab->WdgMDeadlineSupResult    = WDGM_CORRECT;
                }
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
                if (newModeCfg->WdgMSERelated[seIndex].SEIntLogCnt > 0u)
                {
                    WdgM_LogSupIDType intLogId =
                        WdgM_SupervisedEntityPcCfg[newModeCfg->WdgMSERelated[seIndex].WdgMSupervisedEntityID]
                            .WdgMIntLogSupId;
                    WdgM_IntLogSupInfoType* intLogSupTab = &WdgM_IntLogSupTab[intLogId];
                    intLogSupTab->WdgMIntLogActiveFlag   = WDGM_AF_FALSE;
                    intLogSupTab->WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
                    intLogSupTab->WdgMIntLogSupResult    = WDGM_CORRECT;
                }
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/
                /* PRQA S 2982,2983 -- */
            }
        }
    }

    if ((uint16)0 < oldModeCfg->ModeExtLogCnt) /*Mode contains the ExtLogSup*/
    {
        WdgM_ClearExtlogStatus(runDataPtr, oldModeCfg);
    }
}
/* PRQA S 5017, 6010, 6080 -- */

WDGM_LOCAL void WdgM_SEDeactivateInternal(
    const WdgM_RuntimeType* runDataPtr,
    const WdgM_ModeCfgType* modeCfg,
    uint16                  seIndex,
    uint16                  seStatusTabIndex)
{
    uint16                index;
    const WdgM_SECfgType* seInfo;

    WdgM_SEInfoRelated[seStatusTabIndex].WdgMFailedAliveSupCounter            = 0u;
    WdgM_SEInfoRelated[seStatusTabIndex].WdgMEachSupResult[WDGM_ALIVE_SUP]    = WDGM_CORRECT;
    WdgM_SEInfoRelated[seStatusTabIndex].WdgMEachSupResult[WDGM_DEADLINE_SUP] = WDGM_CORRECT;
    WdgM_SEInfoRelated[seStatusTabIndex].WdgMEachSupResult[WDGM_INTLOG_SUP]   = WDGM_CORRECT;
    WdgM_SEInfoRelated[seStatusTabIndex].WdgMEachSupResult[WDGM_EXTLOG_SUP]   = WDGM_CORRECT;
    seInfo                                                                    = &modeCfg->WdgMSERelated[seIndex];

#if (WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON)
    if (seInfo->SEAliveCnt > 0u)
    {
        /* PRQA S 2982,2983 ++ */ /* VL_WdgM_UsedBeforModify,VL_WdgM_2983 */
        for (index = 0u; index < seInfo->SEAliveCnt; index++)
        {
            WdgM_AliveSupIDType    aliveId          = seInfo->WdgMAliveSupCfg[index].WdgMAliveSupID;
            WdgM_AliveSupInfoType* aliveSupTab      = &runDataPtr->WdgMAliveSupervisionInfo[aliveId];
            aliveSupTab->WdgMAliveCycleCounter      = 0u;
            aliveSupTab->WdgMAliveIndicationCounter = 0u;
            aliveSupTab->WdgMAliveSupResult         = WDGM_CORRECT;
        }
        /* PRQA S 2982,2983 -- */
    }
#endif /*WDGM_ALIVE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON)
    if (seInfo->SEDealineCnt > 0u)
    {
        for (index = 0u; index < seInfo->SEDealineCnt; index++)
        {
            WdgM_DeadlineSupIDType    deadlineId     = seInfo->WdgMDeadlineSupCfg[index].WdgMDeadlineSupID;
            WdgM_DeadlineSupInfoType* deadlineSupTab = &runDataPtr->WdgMDeadlineSupervisionInfo[deadlineId];
            deadlineSupTab->WdgMTimeStampStart       = 0u;
            deadlineSupTab->WdgMDeadlineSupResult    = WDGM_CORRECT;
        }
    }
#endif /*WDGM_DEADLINE_SUPERVISION_ENABLE == STD_ON*/

#if (WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON)
    if (seInfo->SEIntLogCnt > 0u)
    {
        WdgM_LogSupIDType       intLogId = WdgM_SupervisedEntityPcCfg[seInfo->WdgMSupervisedEntityID].WdgMIntLogSupId;
        WdgM_IntLogSupInfoType* intLogSupTab = &WdgM_IntLogSupTab[intLogId];
        intLogSupTab->WdgMIntLogActiveFlag   = WDGM_AF_FALSE;
        intLogSupTab->WdgMIntLastReachedCPID = WDGM_NO_CP_REACH;
        intLogSupTab->WdgMIntLogSupResult    = WDGM_CORRECT;
    }
#endif /*WDGM_INT_LOGICAL_SUPERVISION_ENABLE == STD_ON*/
}

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
WDGM_LOCAL Std_ReturnType WdgM_ModeExistenceConfirmation(WdgM_ModeType modeID)
{
    Std_ReturnType         lReturnValue = E_NOT_OK;
    uint8                  index;
    uint8                  indexMax;
    const WdgM_ConfigType* ConfigPtr;
    if (WdgM_GetConfigPtr(&ConfigPtr) == E_NOT_OK)
    {
        lReturnValue = E_NOT_OK;
    }
    else
    {
        indexMax = ConfigPtr->WdgMModeCnt;

        for (index = 0u; index < indexMax; index++)
        {
            if (modeID == ConfigPtr->WdgMMode[index].WdgMModeID)
            {
                lReturnValue = E_OK;
                break;
            }
        }
    }
    return lReturnValue;
}
#endif /*WDGM_DEV_ERROR_DETECT == STD_ON*/

#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
WDGM_LOCAL Std_ReturnType WdgM_OffModeConfirmationForRequestMode(WdgM_ModeType modeID)
{
    Std_ReturnType         lReturnValue = E_OK;
    uint8                  triggerIndex = 0u;
    const WdgM_ConfigType* ConfigPtr;

    if (WdgM_GetConfigPtr(&ConfigPtr) == E_NOT_OK)
    {
        lReturnValue = E_NOT_OK;
    }
    else
    {
        uint8 triggerIndexMax = ConfigPtr->WdgMMode[modeID].ModeTriggerCnt;

        /*Trigger quantities in certain Mode*/
        for (triggerIndex = 0u; triggerIndex < triggerIndexMax; triggerIndex++)
        {
            if (WDGIF_OFF_MODE == ConfigPtr->WdgMMode[modeID].WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
            {
                lReturnValue = E_NOT_OK;
                break;
            }
        }
    }
    return lReturnValue;
}
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/

#if (STD_OFF == WDGM_OFF_MODE_ENABLED)
WDGM_LOCAL Std_ReturnType WdgM_OffModeConfirmation(const WdgM_ConfigType* ConfigPtr)
{
    uint8          triggerIndex = 0u;
    Std_ReturnType lReturnValue;

    if ((uint8)0 == ConfigPtr->WdgMModeCnt)
    {
        lReturnValue = E_OK;
    }
    else
    {
        /*Trigger quantities in each Mode*/
        lReturnValue = E_OK;
        for (triggerIndex = 0; triggerIndex < ConfigPtr->WdgMMode[ConfigPtr->WdgMInitialModeId].ModeTriggerCnt;
             triggerIndex++)
        {
            if (WDGIF_OFF_MODE
                == ConfigPtr->WdgMMode[ConfigPtr->WdgMInitialModeId].WdgMTriggerRelated[triggerIndex].WdgMWatchdogMode)
            {
                lReturnValue = E_NOT_OK;
                break;
            }
        }
    }
    return lReturnValue;
}
#endif /*STD_OFF == WDGM_OFF_MODE_ENABLED*/

WDGM_LOCAL uint8 WdgM_GetWordBitState(uint16 wordNum, uint8 getBit)
{
    uint8 bitvalue;

    if ((uint16)0u != (wordNum & (uint16)((uint16)0x0001u << getBit)))
    {
        bitvalue = 1u;
    }
    else
    {
        bitvalue = 0u;
    }
    return bitvalue;
}

WDGM_LOCAL Std_ReturnType WdgM_InitWdgTrigger(const WdgM_RuntimeType* runDataPtr, const WdgM_ConfigType* ConfigPtr)
{
    uint8                   devIndex;
    Std_ReturnType          retVal = E_OK;
    const WdgM_TriggerType* devicePtr;
    const WdgM_ModeCfgType* modeCfg;

    modeCfg = &ConfigPtr->WdgMMode[ConfigPtr->WdgMInitialModeId];

    for (devIndex = 0u; devIndex < ConfigPtr->WdgMMode[modeCfg->WdgMModeID].ModeTriggerCnt; devIndex++)
    {
        devicePtr = ConfigPtr->WdgMMode[modeCfg->WdgMModeID].WdgMTriggerRelated;
        if (devicePtr != NULL_PTR)
        {
            retVal = WdgIf_SetMode(devicePtr->WdgMWatchdogDevice, devicePtr->WdgMWatchdogMode);
            if ((Std_ReturnType)E_NOT_OK == retVal)
            {
                WdgM_DeinitSEStatus(modeCfg);
                *runDataPtr->WdgMGlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
                break;
            }
        }
    }
    return retVal;
}

#if (WDGM_COMBINATION_USED == STD_ON)
WDGM_LOCAL WdgM_CombinationStatusType
    WdgM_ComputeCombinationStatus(const WdgM_MultipleCoresCombinationType* currentCombinationPtr)
{
    uint8                      slaveCnt;
    uint8                      index;
    WdgM_CombinationStatusType retStatus = WDGM_COMBINATION_OK;

    slaveCnt = currentCombinationPtr->WdgMSlaveCoresCnt;
    for (index = 0u; index < slaveCnt; index++)
    {
        retStatus |= (currentCombinationPtr->WdgMCombinationSlaveStatus[index]);
    }
    return retStatus;
}

WDGM_LOCAL Std_ReturnType WdgM_GetCombinationSlaveStatusIndex(const WdgM_ConfigType* ConfigPtr, uint8* slaveCoreIndex)
{
    uint8          combinationID;
    uint8          index;
    Std_ReturnType ret = E_NOT_OK;
    CoreIdType     coreId;

    combinationID = ConfigPtr->WdgMSupportCombinationID;
    coreId        = GetCoreID();

    for (index = 0u; index < WdgM_CombinationCfgPtr[combinationID].WdgMSlaveCoresCnt; index++)
    {
        if (coreId == WdgM_CombinationCfgPtr[combinationID].WdgMCombinationSlaveCore[index])
        {
            /* found the core id index*/
            *slaveCoreIndex = index;
            ret             = E_OK;
            break;
        }
    }
    return ret;
}
#endif /*WDGM_COMBINATION_USED == STD_ON*/

#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"
