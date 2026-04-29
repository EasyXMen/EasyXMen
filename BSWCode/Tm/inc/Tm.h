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

*/

#ifndef TM_H_
#define TM_H_

/* =================================================== inclusions =================================================== */
#include "Std_Types.h"
#include "Tm_Cfg.h"

/* =============================================== version information ============================================== */
/**
 * @name TM_VERSION_H_INFO_GROUP
 * @{
 * @brief Tm module version infomation macros in Tm header file.
 */
#define TM_VENDOR_ID          62u /**< Tm vendor id*/
#define TM_MODULE_ID          14u /**< Tm module id*/
#define TM_INSTANCE_ID        0u  /**< Tm instance id*/
#define TM_H_AR_MAJOR_VERSION 4u  /**< Tm header ar major version*/
#define TM_H_AR_MINOR_VERSION 9u  /**< Tm header ar minor version*/
#define TM_H_AR_PATCH_VERSION 0u  /**< Tm header ar patch version*/
#define TM_H_SW_MAJOR_VERSION 2u  /**< Tm header sw major version*/
#define TM_H_SW_MINOR_VERSION 2u  /**< Tm header sw minor version*/
#define TM_H_SW_PATCH_VERSION 0u  /**< Tm header sw patch version*/
/** @} */

/* ===================================================== macros ===================================================== */
#if (STD_ON == TM_DEV_ERROR_DETECT)
/**
 * @name TM_ERROR_PARAM_GROUP
 * @{
 * @brief Tm error param macros in Tm header file.
 */
#define TM_E_PARAM_POINTER  0x01u /**< API parameter checking: invalid pointer*/
#define TM_E_PARAM_VALUE    0x02u /**< API parameter checking: invalid value*/
#define TM_E_HARDWARE_TIMER 0x03u /**< Access to underlying hardware timer failed*/
/** @} */
#endif

#define TM_1US24BIT_MAX_MASK 0x00FFFFFFuL /**< Max value of 1us24bit timer*/

/**
 * @name TM_ERROR_API_ID_GROUP
 * @{
 * @brief Tm api id macros in Tm header file.
 */
#define TM_GETVERSIONINFO_ID        0x01u /**< Api Tm_GetVersionInfo id*/
#define TM_RESETTIMER1US16BIT_ID    0x02u /**< Api Tm_ResetTimer1us16bit id*/
#define TM_GETTIMESPAN1US16BIT_ID   0x03u /**< Api Tm_GetTimeSpan1us16bit id*/
#define TM_SHIFTTIMER1US16BIT_ID    0x04u /**< Api Tm_ShiftTimer1us16bit id*/
#define TM_SYNCTIMER1US16BIT_ID     0x05u /**< Api Tm_SyncTimer1us16bit id*/
#define TM_BUSYWAIT1US16BIT_ID      0x06u /**< Api Tm_BusyWait1us16bit id*/
#define TM_RESETTIMER1US24BIT_ID    0x07u /**< Api Tm_ResetTimer1us24bit id */
#define TM_GETTIMESPAN1US24BIT_ID   0x08u /**< Api Tm_GetTimeSpan1us24bit id*/
#define TM_SHIFTTIMER1US24BIT_ID    0x09u /**< Api Tm_ShiftTimer1us24bit id*/
#define TM_SYNCTIMER1US24BIT_ID     0x0au /**< Api Tm_SyncTimer1us24bit id*/
#define TM_BUSYWAIT1US24BIT_ID      0x0bu /**< Api Tm_BusyWait1us24bit id*/
#define TM_RESETTIMER1US32BIT_ID    0x0cu /**< Api Tm_ResetTimer1us32bit id*/
#define TM_GETTIMESPAN1US32BIT_ID   0x0du /**< Api Tm_GetTimeSpan1us32bit id*/
#define TM_SHIFTTIMER1US32BIT_ID    0x0eu /**< Api Tm_ShiftTimer1us32bit id*/
#define TM_SYNCTIMER1US32BIT_ID     0x0fu /**< Api Tm_SyncTimer1us32bit id*/
#define TM_BUSYWAIT1US32BIT_ID      0x10u /**< Api Tm_BusyWait1us32bit id*/
#define TM_RESETTIMER100US32BIT_ID  0x11u /**< Api Tm_ResetTimer100us32bit id*/
#define TM_GETTIMESPAN100US32BIT_ID 0x12u /**< Api Tm_GetTimeSpan100us32bit id*/
#define TM_SHIFTTIMER100US32BIT_ID  0x13u /**< Api Tm_ShiftTimer100us32bit id*/
#define TM_SYNCTIMER100US32BIT_ID   0x14u /**< Api Tm_SyncTimer100us32bit id*/
/** @} */

/* ================================================ type definitions ================================================ */
/**
 * @brief 1us16bit timer type.
 */
typedef struct
{
    uint16 ui16RefTime; /**< Type definition for 1us16bit type clock @range 0..65535 */
} Tm_PredefTimer1us16bitType;

/**
 * @brief 1us24bit timer type.
 */
typedef struct
{
    uint32 ui24RefTime; /**< Type definition for 1us24bit type clock @range 0..16777215 */

} Tm_PredefTimer1us24bitType;

/**
 * @brief 1us32bit timer type.
 */
typedef struct
{
    uint32 ui32RefTime; /**< Type definition for 1us32bit type clock @range 0..4294967295 */

} Tm_PredefTimer1us32bitType;

/**
 * @brief 100us32bit timer type.
 */
typedef struct
{
    uint32 ui32RefTime; /**< Type definition for 100us32bit type clock @range 0..4294967295*/

} Tm_PredefTimer100us32bitType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#ifdef __cplusplus
extern "C" {
#endif
#if (STD_ON == TM_VERSIONINFO_API)
/**
 * @brief Returns the version information of this module.
 * @param[out]  versioninfo, Pointer to where to store the version information of this module.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-69874
 */
void Tm_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif /* STD_ON == TM_VERSIONINFO_API */

#if (STD_ON == TM_1US16BIT)
/**
 * @brief Resets a timer instance (user point of view).
 * @param[out]  TimerPtr: Pointer to a timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69875
 */
Std_ReturnType Tm_ResetTimer1us16bit(Tm_PredefTimer1us16bitType* TimerPtr);

/**
 * @brief Delivers the time difference (current time - reference time).
 * @param[in]   TimerPtr: Pointer to a timer instance defined by the user.
 * @param[out]  TimeSpanPtr: Pointer to time span destination data in RAM.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69876
 */
Std_ReturnType Tm_GetTimeSpan1us16bit(const Tm_PredefTimer1us16bitType* TimerPtr, uint16* TimeSpanPtr);

/**
 * @brief Shifts the reference time of the timer instance.
 * @param[in]   TimerPtr: Pointer to a timer instance defined by the user.
 * @param[out]  TimeValue: Time value in us, the reference time has to be shifted.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-69877
 */
void Tm_ShiftTimer1us16bit(Tm_PredefTimer1us16bitType* TimerPtr, uint16 TimeValue);

/**
 * @brief Synchronizes two timer instances.
 * @param[in]   TimerSrcPtr: Pointer to the source timer instance defined by the user.
 * @param[out]  TimerDstPtr: Pointer to the destination timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-69878
 */
void Tm_SyncTimer1us16bit(Tm_PredefTimer1us16bitType* TimerDstPtr, const Tm_PredefTimer1us16bitType* TimerSrcPtr);

/**
 * @brief Performs busy waiting by polling with a guaranteed minimum waiting time.
 * @param[in]   WaitingTimeMin: Minimum waiting time in microseconds.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69879
 */
Std_ReturnType Tm_BusyWait1us16bit(uint8 WaitingTimeMin);
#endif /* STD_ON == TM_1US16BIT */

#if (STD_ON == TM_1US24BIT)
/**
 * @brief Resets a timer instance (user point of view).
 * @param[out]  TimerPtr: Pointer to a timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69880
 */
Std_ReturnType Tm_ResetTimer1us24bit(Tm_PredefTimer1us24bitType* TimerPtr);

/**
 * @brief Delivers the time difference (current time - reference time).
 * @param[in]   TimerPtr: Pointer to a timer instance defined by the user.
 * @param[out]  TimeSpanPtr: Pointer to time span destination data in RAM.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69881
 */
Std_ReturnType Tm_GetTimeSpan1us24bit(const Tm_PredefTimer1us24bitType* TimerPtr, uint32* TimeSpanPtr);

/**
 * @brief Shifts the reference time of the timer instance.
 * @param[in]   TimerPtr: Pointer to a timer instance defined by the user.
 * @param[out]  TimeValue: Time value in us, the reference time has to be shifted.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-69882
 */
void Tm_ShiftTimer1us24bit(Tm_PredefTimer1us24bitType* TimerPtr, uint32 TimeValue);

/**
 * @brief Synchronizes two timer instances.
 * @param[in]   TimerSrcPtr: Pointer to the source timer instance defined by the user.
 * @param[out]  TimerDstPtr: Pointer to the destination timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-6988333
 */
void Tm_SyncTimer1us24bit(Tm_PredefTimer1us24bitType* TimerDstPtr, const Tm_PredefTimer1us24bitType* TimerSrcPtr);

/**
 * @brief Performs busy waiting by polling with a guaranteed minimum waiting time.
 * @param[in]   WaitingTimeMin: Minimum waiting time in microseconds.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69884
 */
Std_ReturnType Tm_BusyWait1us24bit(uint8 WaitingTimeMin);
#endif /* STD_ON == TM_1US24BIT */

#if (STD_ON == TM_1US32BIT)
/**
 * @brief Resets a timer instance (user point of view).
 * @param[out]  TimerPtr: Pointer to a timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69885
 */
Std_ReturnType Tm_ResetTimer1us32bit(Tm_PredefTimer1us32bitType* TimerPtr);

/**
 * @brief Delivers the time difference (current time - reference time).
 * @param[in]   TimerPtr: Pointer to a timer instance defined by the user.
 * @param[out]  TimeSpanPtr: Pointer to time span destination data in RAM.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69886
 */
Std_ReturnType Tm_GetTimeSpan1us32bit(const Tm_PredefTimer1us32bitType* TimerPtr, uint32* TimeSpanPtr);

/**
 * @brief Shifts the reference time of the timer instance.
 * @param[in]   TimerPtr: Pointer to a timer instance defined by the user.
 * @param[out]  TimeValue: Time value in us, the reference time has to be shifted.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-69887
 */
void Tm_ShiftTimer1us32bit(Tm_PredefTimer1us32bitType* TimerPtr, uint32 TimeValue);

/**
 * @brief Synchronizes two timer instances.
 * @param[in]   TimerSrcPtr: Pointer to the source timer instance defined by the user.
 * @param[out]  TimerDstPtr: Pointer to the destination timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-69888
 */
void Tm_SyncTimer1us32bit(Tm_PredefTimer1us32bitType* TimerDstPtr, const Tm_PredefTimer1us32bitType* TimerSrcPtr);

/**
 * @brief Performs busy waiting by polling with a guaranteed minimum waiting time.
 * @param[in]   WaitingTimeMin: Minimum waiting time in microseconds.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69889
 */
Std_ReturnType Tm_BusyWait1us32bit(uint8 WaitingTimeMin);
#endif /* STD_ON == TM_1US32BIT */

#if (STD_ON == TM_100US32BIT)
/**
 * @brief Resets a timer instance (user point of view).
 * @param[out]  TimerPtr: Pointer to a timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69890
 */
Std_ReturnType Tm_ResetTimer100us32bit(Tm_PredefTimer100us32bitType* TimerPtr);

/**
 * @brief Delivers the time difference (current time - reference time).
 * @param[in]   TimerPtr: Pointer to a timer instance defined by the user.
 * @param[out]  TimeSpanPtr: Pointer to time span destination data in RAM.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @return      Std_ReturnType
 * @retval      E_OK: The underlying GPT driver service has returned E_OK and
                no development error has been detected
 * @retval      E_NOT_OK: The underlying GPT driver service has returned
                E_NOT_OK, or a development error has been detected
 * @trace       CPD-69891
 */
Std_ReturnType Tm_GetTimeSpan100us32bit(const Tm_PredefTimer100us32bitType* TimerPtr, uint32* TimeSpanPtr);

/**
 * @brief Shifts the reference time of the timer instance.
 * @param[in]   TimerPtr: Pointer to a timer instance defined by the user.
 * @param[out]  TimeValue: Time value in us, the reference time has to be shifted.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-69892
 */
void Tm_ShiftTimer100us32bit(Tm_PredefTimer100us32bitType* TimerPtr, uint32 TimeValue);

/**
 * @brief Synchronizes two timer instances.
 * @param[in]   TimerSrcPtr: Pointer to the source timer instance defined by the user.
 * @param[out]  TimerDstPtr: Pointer to the destination timer instance defined by the user.
 * @reentrant   Reentrant but not for the same timer instance
 * @synchronous TRUE
 * @trace       CPD-69893
 */
void Tm_SyncTimer100us32bit(Tm_PredefTimer100us32bitType* TimerDstPtr, const Tm_PredefTimer100us32bitType* TimerSrcPtr);
#endif /* STD_ON == TM_100US32BIT */

#ifdef __cplusplus
}
#endif
#endif /* TM_H_ */
