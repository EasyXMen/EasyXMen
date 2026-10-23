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
 **  @file               : Tm.h
 **  @author             : xudong.guan
 **  @date               : 2024/08/20
 **  @vendor             : isoft
 **  @description        : Header file for Tm
 **
 ***********************************************************************************************************************/
/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.02.00 2024-12-12  Xudong.Guan    1.Update annotations.
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
\page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
  ModeName:WdgM<br>
RuleSorce:puhua_rules-2024.12.rcf

  \li VL_Tm_1503
    Reason: Functions defined out are not used in the project and have no impact on the integration application.
    Risk: No impact on integrated functions.
    Prevention: Provided to the application to use, the application use need to use the corresponding interface.

  \li VL_Tm_3472
    Reason: Macro definitions for easy reuse.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_Tm_0310
    Reason: Compatible with different types.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_Tm_1505
    Reason: external interface
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_Tm_0380
    Reason: Macro definitions are necessary
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_Tm_0883
    Reason: The code design is like this and cannot be changed.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_Tm_0791
    Reason: The code design is like this and cannot be changed.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_Tm_2981
    Reason: Init may be necessary for return variant.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li VL_MTR_Tm_STMIF
      Reason: Functions handle specific tasks in the behavior of the entire component.
    Task has different scenarios that need to be covered, which depends on local conditions - this
    leads to deep nesting of the control structure.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

  \li VL_MTR_Tm_STCYC
      Reason: Because there are higher requirements for minimizing the stack and runtime usage of the applied code.
      Risk: Understandability and testability become overly complex
      Prevention: Design and code review, and have a clear structure and annotated code.

  \li VL_Tm_1502
      Reason: Configuration variables design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

*/

#ifndef TM_
#define TM_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Tm_Cfg.h"
#include "Gpt.h" /* PRQA S 0380 */ /* VL_Tm_0380 */

/* =============================================== version information ============================================== */
/**
 * @name TM_VERSION_H_INFO_GROUP
 * @{
 * @brief Tm module version infomation macros in Tm header file.
 */
#define TM_VENDOR_ID                62u /**< Tm vendor id*/
#define TM_MODULE_ID                14u /**< Tm module id*/
#define TM_INSTANCE_ID              0u  /**< Tm instance id*/
#define TM_AR_RELEASE_MAJOR_VERSION 4u  /**< Tm header ar major version*/
#define TM_AR_RELEASE_MINOR_VERSION 9u  /**< Tm header ar minor version*/
#define TM_AR_RELEASE_PATCH_VERSION 0u  /**< Tm header ar patch version*/
#define TM_SW_MAJOR_VERSION         2u  /**< Tm header sw major version*/
#define TM_SW_MINOR_VERSION         2u  /**< Tm header sw minor version*/
#define TM_SW_PATCH_VERSION         0u  /**< Tm header sw patch version*/
/** @} */

/* ===================================================== macros ===================================================== */
#if (STD_ON == TM_DEV_ERROR_DETECT)
/**
 * @name TM_ERROR_PARAM_GROUP
 * @{
 * @brief Tm error param macros in Tm header file.
 */
#define TM_E_PARAM_POINTER 0x01u /**< API parameter checking: invalid pointer*/
#define TM_E_PARAM_VALUE   0x02u /**< API parameter checking: invalid value*/
#define TM_E_PARAM_SYNC    0x03u /**< API parameter checking: incompatible timer instances for Tm_SyncTimer*/
#define TM_E_STATE         0x04u /**< A provided timer instance was not initialized via Tm_ResetTimer*/
#define TM_E_UNINIT        0x05u /**< Timer instance was not initialized*/
#endif
/** @} */

#define TM_E_HARDWARE_TIMER 0x03u /**< Access to underlying hardware timer failed*/

/**
 * @name TM_ERROR_API_ID_GROUP
 * @{
 * @brief Tm api id macros in Tm header file.
 */
#define TM_INIT_ID           0x01u /**< Api Tm_Init id*/
#define TM_GETVERSIONINFO_ID 0x02u /**< Api Tm_GetVersionInfo id*/
#define TM_RESETTIMER_ID     0x03u /**< Api Tm_ResetTimer id*/
#define TM_GETTIMESPAN_ID    0x04u /**< Api Tm_GetTimeSpan id*/
#define TM_SHIFTTIMER_ID     0x05u /**< Api Tm_ShiftTimer id*/
#define TM_SYNCTIMER_ID      0x06u /**< Api Tm_SyncTimer id*/
#define TM_BUSYWAIT1US_ID    0x07u /**< Api Tm_BusyWait1us id*/
/** @} */

/**
 * @name TM_PREDEF_TIMER
 * @{
 * @brief Tm predef timer macros in Tm header file.
 */
#define TM_PREDEF_TIMER_1US_16BIT   GPT_PREDEF_TIMER_1US_16BIT   /**< Predefined timer 1us 16bit */
#define TM_PREDEF_TIMER_1US_24BIT   GPT_PREDEF_TIMER_1US_24BIT   /**< Predefined timer 1us 24bit */
#define TM_PREDEF_TIMER_1US_32BIT   GPT_PREDEF_TIMER_1US_32BIT   /**< Predefined timer 1us 32bit */
#define TM_PREDEF_TIMER_100US_32BIT GPT_PREDEF_TIMER_100US_32BIT /**< Predefined timer 100us 32bit */
/** @} */

#define TM_1US16BIT_MAX_VALUE   0xFFFFu
#define TM_1US24BIT_MAX_VALUE   0xFFFFFFuL
#define TM_1US32BIT_MAX_VALUE   0xFFFFFFFFuL
#define TM_100US32BIT_MAX_VALUE 0xFFFFFFFFuL

#define TM_INVALID_TIMER_VALUE 0xFFFFFFFFuL

/* ================================================ type definitions ================================================ */

typedef struct
{
    uint32              timerId;
    Gpt_PredefTimerType timerType;
} Tm_PreDefTimerType;

typedef struct
{
    const Tm_PreDefTimerType* preDefTimer;
} Tm_ConfigType;

/* Tm initialization configuration data */
#define TM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Tm_MemMap.h"
extern const Tm_ConfigType Tm_Config;
#define TM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Tm_MemMap.h"

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This service initializes the Time Service module.
 * @param[in]   config: Pointer to Tm  configuration data.
 * @reentrant   FALSE
 * @synchronous TRUE
 * @trace       CPD-PlaceHolder
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
void Tm_Init(const Tm_ConfigType* config);
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

#if (STD_ON == TM_VERSIONINFO_API)
/**
 * @brief Returns the version information of this module.
 * @param[out]  versioninfo, Pointer to where to store the version information of this module.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-69874
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
void Tm_GetVersionInfo(Std_VersionInfoType* versioninfo);
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"
#endif /* STD_ON == TM_VERSIONINFO_API */

/**
 * @brief Resets a timer instance (user point of view).
 * @param[out]  timerId: A timer instance handle.
 * @reentrant   Reentrant but not for the same timerId.
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK.
 * @retval      E_NOT_OK: The underlying GPT driver service has not returned E_NOT_OK.
 * @trace       CPD-PlaceHolder
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
Std_ReturnType Tm_ResetTimer(uint32 timerId);
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * @brief Delivers the time difference (current time - reference time).
 * @param[in]   timerId: A timer instance handle.
 * @param[out]  timeSpan: Pointer to location where the time will be stored.
 * @reentrant   Reentrant
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK.
 * @retval      E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK.
 * @trace       CPD-PlaceHolder
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
Std_ReturnType Tm_GetTimeSpan(uint32 timerId, uint32* timeSpan);
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * @brief Shifts the reference time of the timer instance.
 * @param[in]   timerId: A timer instance handle.
 * @param[out]  timeValue: Time value in in units (e.g. us), the reference time has to be shifted.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-PlaceHolder
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
void Tm_ShiftTimer(uint32 timerId, uint32 timeValue);
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * @brief Synchronizes two timer instances.
 * @param[in]   timerIdSrc: Source timer instance defined by the user.
 * @param[out]  timerIdDst: Destination timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-PlaceHolder
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
void Tm_SyncTimer(uint32 timerIdSrc, uint32 timerIdDst);
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * @brief Performs busy waiting by polling with a guaranteed minimum waiting time.
 * @param[in]   WaitingTimeMin: Minimum waiting time in microseconds.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK.
 * @retval      E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK.
 * @trace       CPD-PlaceHolder
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
Std_ReturnType Tm_BusyWait1us(uint8 WaitingTimeMin);
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

#ifdef __cplusplus
}
#endif
#endif /* TM_ */
