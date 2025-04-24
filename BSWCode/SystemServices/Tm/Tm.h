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
**  FILENAME    : Tm.h                                                        **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public interfaces declared by Tm module                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
#ifndef TM_H_
#define TM_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
#include "Tm_Cfg.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define TM_VENDOR_ID          62U
#define TM_MODULE_ID          14U
#define TM_INSTANCE_ID        0U
#define TM_H_AR_MAJOR_VERSION 4U
#define TM_H_AR_MINOR_VERSION 5U
#define TM_H_AR_PATCH_VERSION 0U
#define TM_H_SW_MAJOR_VERSION 2U
#define TM_H_SW_MINOR_VERSION 1U
#define TM_H_SW_PATCH_VERSION 0U

#define TM_DEMO               STD_ON

#if (STD_ON == TM_DEV_ERROR_DETECT)
#define TM_E_PARAM_POINTER ((uint8)0x01)
#define TM_E_PARAM_VALUE   ((uint8)0x02)
#endif

#define TM_E_HARDWARE_TIMER         ((uint8)0x03)

#define TM_GETVERSIONINFO_ID        ((uint8)0x01)
#define TM_RESETTIMER1US16BIT_ID    ((uint8)0x02)
#define TM_GETTIMESPAN1US16BIT_ID   ((uint8)0x03)
#define TM_SHIFTTIMER1US16BIT_ID    ((uint8)0x04)
#define TM_SYNCTIMER1US16BIT_ID     ((uint8)0x05)
#define TM_BUSYWAIT1US16BIT_ID      ((uint8)0x06)
#define TM_RESETTIMER1US24BIT_ID    ((uint8)0x07)
#define TM_GETTIMESPAN1US24BIT_ID   ((uint8)0x08)
#define TM_SHIFTTIMER1US24BIT_ID    ((uint8)0x09)
#define TM_SYNCTIMER1US24BIT_ID     ((uint8)0x0a)
#define TM_BUSYWAIT1US24BIT_ID      ((uint8)0x0b)
#define TM_RESETTIMER1US32BIT_ID    ((uint8)0x0c)
#define TM_GETTIMESPAN1US32BIT_ID   ((uint8)0x0d)
#define TM_SHIFTTIMER1US32BIT_ID    ((uint8)0x0e)
#define TM_SYNCTIMER1US32BIT_ID     ((uint8)0x0f)
#define TM_BUSYWAIT1US32BIT_ID      ((uint8)0x10)
#define TM_RESETTIMER100US32BIT_ID  ((uint8)0x11)
#define TM_GETTIMESPAN100US32BIT_ID ((uint8)0x12)
#define TM_SHIFTTIMER100US32BIT_ID  ((uint8)0x13)
#define TM_SYNCTIMER100US32BIT_ID   ((uint8)0x14)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/* Type definition for 1us16bit type clock */
typedef struct
{
    uint16 ui16RefTime;
} Tm_PredefTimer1us16bitType;

/* Type definition for 1us24bit type clock */
typedef struct
{
    uint32 ui24RefTime;

} Tm_PredefTimer1us24bitType;

/* Type definition for 1us32bit type clock */
typedef struct
{
    uint32 ui32RefTime;

} Tm_PredefTimer1us32bitType;

/* Type definition for 100us32bit type clock */
typedef struct
{
    uint32 ui32RefTime;

} Tm_PredefTimer100us32bitType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/
#if (STD_ON == TM_VERSIONINFO_API)

/**
 * This service returns the version information of this module.
 * Service ID: 0x1
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): versioninfo: Pointer to where to store the version information of this module
 * Return value: NA
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(void, TM_CODE) Tm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, TM_APPL_DATA) versioninfo);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/
#endif               /* STD_ON == TM_VERSIONINFO_API */

#if (STD_ON == TM_1US16BIT)

/**
 * This service resets a timer instance (user point of view).
 * Service ID: 0x2
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): TimerPtr: Pointer to a timer instance defined by the user.
 * Return value: Std_ReturnType: E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE)
    Tm_ResetTimer1us16bit(P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service delivers the time difference (current time - reference time).
 * Service ID: 0x3
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimerPtr: Pointer to a timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimeSpanPtr: Pointer to time span destination data in RAM.
 * Return value: Std_ReturnType: E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us16bit(
    P2CONST(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint16, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service shifts the reference time of the timer instance.
 * Service ID: 0x4
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimeValue: Time value in us, the reference time has to be shifted
 * Parameters(INOUT): TimerPtr: Pointer to a timer instance defined by the user.
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(void, TM_CODE) Tm_ShiftTimer1us16bit(
    P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr,
    VAR(uint16, AUTOMATIC) TimeValue);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service synchronizes two timer instances.
 * Service ID: 0x5
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimerSrcPtr: Pointer to the source timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimerDstPtr: Pointer to the destination timer instance defined by the user.
 * Return value: NA
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(void, TM_CODE) Tm_SyncTimer1us16bit(
    P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA) TimerDstPtr,
    P2CONST(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_CONST) TimerSrcPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service performs busy waiting by polling with a guaranteed minimum waiting time.
 * Service ID: 0x6
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): WaitingTimeMin: Minimum waiting time in microseconds.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType: E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us16bit(VAR(uint8, AUTOMATIC) WaitingTimeMin);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

#endif /* STD_ON == TM_1US16BIT */

#if (STD_ON == TM_1US24BIT)
/**
 * This service resets a timer instance (user point of view).
 * Service ID: 0x7
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): TimerPtr: Pointer to a timer instance defined by the user.
 * Return value: Std_ReturnType: E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE)
    Tm_ResetTimer1us24bit(P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service delivers the time difference (current time - reference time).
 * Service ID: 0x8
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): TimerPtr: Pointer to a timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimeSpanPtr: Pointer to time span destination data in RAM
 * Return value: Std_ReturnType: E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us24bit(
    P2CONST(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint32, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service shifts the reference time of the timer instance.
 * Service ID: 0x9
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimeValue: Time value in μs, the reference time has to be shifted. Range: 0-0xFFFFFF
 * Parameters(INOUT): TimerPtr: Pointer to a timer instance defined by the user.
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(void, TM_CODE) Tm_ShiftTimer1us24bit(
    P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr,
    VAR(uint32, AUTOMATIC) TimeValue);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service synchronizes two timer instances.
 * Service ID: 0xa
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimerSrcPtr: Pointer to the source timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimerDstPtr: Pointer to the destination timer instance defined by the user.
 * Return value: NA
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(void, TM_CODE) Tm_SyncTimer1us24bit(
    P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA) TimerDstPtr,
    P2CONST(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_CONST) TimerSrcPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service performs busy waiting by polling with a guaranteed minimum waiting time.
 * Service ID: 0xb
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): WaitingTimeMin,Minimum waiting time in microseconds.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us24bit(VAR(uint8, AUTOMATIC) WaitingTimeMin);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

#endif /* STD_ON == TM_1US24BIT */

#if (STD_ON == TM_1US32BIT)

/**
 * This service performs busy waiting by polling with a guaranteed minimum waiting time.
 * Service ID: 0xc
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): WaitingTimeMin: Minimum waiting time in microseconds.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType: E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE)
    Tm_ResetTimer1us32bit(P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service delivers the time difference (current time - reference time).
 * Service ID: 0xd
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): TimerPtr,Pointer to a timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimeSpanPtr: Pointer to time span destination data in RAM
 * Return value: Std_ReturnType: E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us32bit(
    P2CONST(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint32, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service shifts the reference time of the timer instance.
 * Service ID: 0xe
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimeValue: Time value in μs, the reference time has to be shifted. Range: 0-0xFFFFFF
 * Parameters(INOUT): TimerPtr: Pointer to a timer instance defined by the user.
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(void, TM_CODE) Tm_ShiftTimer1us32bit(
    P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr,
    VAR(uint32, AUTOMATIC) TimeValue);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service synchronizes two timer instances.
 * Service ID: 0xf
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimerSrcPtr: Pointer to the source timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimerDstPtr: Pointer to the destination timer instance defined by the user.
 * Return value: NA
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(void, TM_CODE) Tm_SyncTimer1us32bit(
    P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA) TimerDstPtr,
    P2CONST(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_CONST) TimerSrcPtr);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

/**
 * This service performs busy waiting by polling with a guaranteed minimum waiting time.
 * Service ID: 0x10
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): WaitingTimeMin: Minimum waiting time in microseconds.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType: E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

/* PRQA S 3432 ++ */ /* MISRA Rule 20.7*/
extern FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us32bit(VAR(uint8, AUTOMATIC) WaitingTimeMin);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7*/

#endif /* STD_ON == TM_1US32BIT */

#if (STD_ON == TM_100US32BIT)

/**
 * This service performs busy waiting by polling with a guaranteed minimum waiting time.
 * Service ID: 0x11
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): WaitingTimeMin,Minimum waiting time in microseconds.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType,E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

extern FUNC(Std_ReturnType, TM_CODE)
    Tm_ResetTimer100us32bit(P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr);

/**
 * This service delivers the time difference (current time - reference time).
 * Service ID: 0x12
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): TimerPtr,Pointer to a timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimeSpanPtr,Pointer to time span destination data in RAM
 * Return value: Std_ReturnType,E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan100us32bit(
    P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint32, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr);

/**
 * This service shifts the reference time of the timer instance.
 * Service ID: 0x13
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimeValue,Time value in μs, the reference time has to be shifted. Range: 0-0xFFFFFF
 * Parameters(INOUT): TimerPtr,Pointer to a timer instance defined by the user.
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

extern FUNC(void, TM_CODE) Tm_ShiftTimer100us32bit(
    P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr,
    VAR(uint32, AUTOMATIC) TimeValue);

/**
 * This service synchronizes two timer instances.
 * Service ID: 0x14
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimerSrcPtr,Pointer to the source timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimerDstPtr,Pointer to the destination timer instance defined by the user.
 * Return value: NA
 *
 */

extern FUNC(void, TM_CODE) Tm_SyncTimer100us32bit(
    P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA) TimerDstPtr,
    P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_CONST) TimerSrcPtr);

#endif /* STD_ON == TM_100US32BIT */

#if (STD_ON == TM_DEMO)

/**
 * This service delivers the time difference (current time - reference time).
 * Service ID: 0x15
 * Sync/Async: synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): TimerPtr,Pointer to a timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimeSpanPtr,Pointer to time span destination data in RAM
 * Return value: Std_ReturnType,E_OK: The underlying GPT driver service has returned E_OK and no development error has
 * been detected E_NOT_OK: The underlying GPT driver service has returned E_NOT_OK, or a development error has been
 * detected
 *
 */

extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1ms32bit(
    P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint32, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr);

#endif /* STD_ON == TM_DEMO */

#endif /* TM_H_ */
