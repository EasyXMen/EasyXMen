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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Tm.c                                                        **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun.Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public functions implementation                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* <VERSION>   <DATE>     <AUTHOR>     <REVISION LOG>
 * V2.0.0      20200824   Xinrun.Wang  Initial version
 * V2.1.0      2023-07-18   Jian.jiang      CP2.1 Release Version.
 */

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Tm<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 3432 MISRA Rule 20.7 .<br>
    Reason:Function-like macros are used to allow more efficient code.

    \li PRQA S 1503 MISRA Dir 2.1 .<br>
    Reason:No impact was assessed and the status quo was maintained.

    \li PRQA S 2981 MISRA Rule 2.2
    .<br> Reason:Init may be necessary for different macro definition.
 */
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tm.h"
#include "Gpt.h"
#include "Det.h"

/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/
#define TM_C_AR_MAJOR_VERSION 4U
#define TM_C_AR_MINOR_VERSION 5U
#define TM_C_AR_PATCH_VERSION 0U
#define TM_C_SW_MAJOR_VERSION 2U
#define TM_C_SW_MINOR_VERSION 1U
#define TM_C_SW_PATCH_VERSION 0U

/*******************************************************************************
**                       Version  Check                                       **
*******************************************************************************/
#if (                                                                                                       \
    (TM_C_AR_MAJOR_VERSION != TM_H_AR_MAJOR_VERSION) || (TM_C_AR_MINOR_VERSION != TM_H_AR_MINOR_VERSION)    \
    || (TM_C_AR_PATCH_VERSION != TM_H_AR_PATCH_VERSION) || (TM_C_SW_MAJOR_VERSION != TM_H_SW_MAJOR_VERSION) \
    || (TM_C_SW_MINOR_VERSION != TM_H_SW_MINOR_VERSION) || (TM_C_SW_PATCH_VERSION != TM_H_SW_PATCH_VERSION))

#error "Tm.c : Mismatch in Specification Major Version"
#endif

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#if (STD_ON == TM_DEV_ERROR_DETECT)
#define TM_DET_REPORT(ApiId, ErrorId) ((void)Det_ReportError(TM_MODULE_ID, TM_INSTANCE_ID, (ApiId), (ErrorId)))
#endif /* STD_ON == TM_DEV_ERROR_DETECT */

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
FUNC(void, TM_CODE) Tm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, TM_APPL_DATA) versioninfo)
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (NULL_PTR == versioninfo)
    {
        TM_DET_REPORT(TM_GETVERSIONINFO_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* update versioninfo */
        versioninfo->vendorID = TM_VENDOR_ID;
        versioninfo->moduleID = TM_MODULE_ID;
        versioninfo->sw_major_version = TM_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = TM_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = TM_H_SW_PATCH_VERSION;
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

#endif /* STD_ON == TM_VERSIONINFO_API */

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us16bit(P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981*/ /* MISRA Rule 2.2 */
    uint32 CurrentTime = 0;
    uint32* CurrentTimePtr = &CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (NULL_PTR == TimerPtr)
    {

        TM_DET_REPORT(TM_RESETTIMER1US16BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt real time and assign to reference time of given clock */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, CurrentTimePtr);
        if ((Std_ReturnType)E_OK == ret)
        {
            TimerPtr->ui16RefTime = (uint16)CurrentTime;
        }
        else
        {
            /* gpt clock error */
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_RESETTIMER1US16BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE)
Tm_GetTimeSpan1us16bit(
    P2CONST(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint16, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981*/ /* MISRA Rule 2.2 */
    uint32 ui16CurrentTime = 0;
    uint32* ui16CurrentTimePtr = &ui16CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
    {
        TM_DET_REPORT(TM_GETTIMESPAN1US16BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt time, subtract it from reference time */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, ui16CurrentTimePtr);
        if ((Std_ReturnType)E_OK == ret)
        {
            *TimeSpanPtr = (uint16)(ui16CurrentTime - TimerPtr->ui16RefTime);
        }
        else
        {
            /* gpt clock error, timespan=0 */
            *TimeSpanPtr = 0U;
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_GETTIMESPAN1US16BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(void, TM_CODE)
Tm_ShiftTimer1us16bit(
    P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr,
    VAR(uint16, AUTOMATIC) TimeValue)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_SHIFTTIMER1US16BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* simply add TimeValue to refernce time */
        TimerPtr->ui16RefTime = (uint16)(TimerPtr->ui16RefTime + TimeValue);
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(void, TM_CODE)
Tm_SyncTimer1us16bit(
    P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA) TimerDstPtr,
    P2CONST(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_CONST) TimerSrcPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
    {
        TM_DET_REPORT(TM_SYNCTIMER1US16BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* Sync two clocks */
        TimerDstPtr->ui16RefTime = TimerSrcPtr->ui16RefTime;
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us16bit(VAR(uint8, AUTOMATIC) WaitingTimeMin)
{
    Std_ReturnType ret;
    uint32 RefTime = 0;
    uint32* RefTimePtr = &RefTime;
    uint32 CurrentTime = 0;
    uint32* CurrentTimePtr = &CurrentTime;

    /* get the start time */
    ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, RefTimePtr);

    /* keep getting the current time and subtract it from start time until over timemin */
    do
    {
        if ((Std_ReturnType)E_OK == ret)
        {
            ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, CurrentTimePtr);
        }
        else
        {
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_BUSYWAIT1US16BIT_ID, TM_E_HARDWARE_TIMER);
            break;
        }
    } while ((uint16)(CurrentTime - RefTime) <= WaitingTimeMin);

    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us24bit(P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981*/ /* MISRA Rule 2.2 */
    uint32 CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect null_ptr */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_RESETTIMER1US24BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt real time and assign to reference time of given clock */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            TimerPtr->ui24RefTime = CurrentTime & (uint32)0x00FFFFFFu;
        }
        else
        {
            /* gpt clock error */
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_RESETTIMER1US24BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE)
Tm_GetTimeSpan1us24bit(
    P2CONST(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint32, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981*/ /* MISRA Rule 2.2 */
    uint32 ui32CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
    {
        TM_DET_REPORT(TM_GETTIMESPAN1US24BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt time, subtract it from reference time */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &ui32CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            *TimeSpanPtr = (uint32)(ui32CurrentTime - TimerPtr->ui24RefTime) & (uint32)0x00FFFFFFu;
        }
        else
        {
            /* gpt error, TimeSpan = 0 */
            *TimeSpanPtr = 0U;
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_GETTIMESPAN1US24BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * This service shifts the reference time of the timer instance.
 * Service ID: 0x9
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimeValue: Time value in us, the reference time has to be shifted. Range: 0-0xFFFFFF
 * Parameters(INOUT): TimerPtr: Pointer to a timer instance defined by the user.
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(void, TM_CODE)
Tm_ShiftTimer1us24bit(
    P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr,
    VAR(uint32, AUTOMATIC) TimeValue)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_SHIFTTIMER1US24BIT_ID, TM_E_PARAM_POINTER);
    }
    else if (TimeValue > 0x00FFFFFFu)
    {
        TM_DET_REPORT(TM_SHIFTTIMER1US24BIT_ID, TM_E_PARAM_VALUE);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* simply add TimeValue to refernce time */
        TimerPtr->ui24RefTime = (uint32)(TimerPtr->ui24RefTime + TimeValue) & (uint32)0x00FFFFFFu;
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(void, TM_CODE)
Tm_SyncTimer1us24bit(
    P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA) TimerDstPtr,
    P2CONST(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_CONST) TimerSrcPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
    {
        TM_DET_REPORT(TM_SYNCTIMER1US24BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* Sync two clocks */
        TimerDstPtr->ui24RefTime = TimerSrcPtr->ui24RefTime;
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us24bit(VAR(uint8, AUTOMATIC) WaitingTimeMin)
{
    Std_ReturnType ret;
    uint32 RefTime;
    uint32 CurrentTime;

    /* get the start time */
    ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &RefTime);

    /* keep getting the current time and subtract it from start time until over timemin */
    do
    {
        if ((Std_ReturnType)E_OK == ret)
        {
            ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &CurrentTime);
        }
        else
        {
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_BUSYWAIT1US24BIT_ID, TM_E_HARDWARE_TIMER);
            break;
        }
    } while (((uint32)(CurrentTime - RefTime) & (uint32)0x00FFFFFFu) <= WaitingTimeMin);

    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us32bit(P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981*/ /* MISRA Rule 2.2 */
    uint32 CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_RESETTIMER1US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt real time and assign to reference time of given clock */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            TimerPtr->ui32RefTime = CurrentTime;
        }
        else
        {
            /* gpt clock error */
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_RESETTIMER1US32BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE)
Tm_GetTimeSpan1us32bit(
    P2CONST(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint32, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981*/ /* MISRA Rule 2.2 */
    uint32 ui32CurrentTime;

#if (TM_DEV_ERROR_DETECT == STD_ON)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
    {
        TM_DET_REPORT(TM_GETTIMESPAN1US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt time, subtract it from reference time */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &ui32CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            *TimeSpanPtr = (uint32)(ui32CurrentTime - TimerPtr->ui32RefTime);
        }
        else
        {
            /* gpt error, TimeSpan = 0 */
            *TimeSpanPtr = 0U;
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_GETTIMESPAN1US32BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * This service shifts the reference time of the timer instance.
 * Service ID: 0xe
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimeValue: Time value in us, the reference time has to be shifted. Range: 0-0xFFFFFF
 * Parameters(INOUT): TimerPtr: Pointer to a timer instance defined by the user.
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(void, TM_CODE)
Tm_ShiftTimer1us32bit(
    P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr,
    VAR(uint32, AUTOMATIC) TimeValue)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_SHIFTTIMER1US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* simply add TimeValue to refernce time */
        TimerPtr->ui32RefTime = (uint32)(TimerPtr->ui32RefTime + TimeValue);
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(void, TM_CODE)
Tm_SyncTimer1us32bit(
    P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA) TimerDstPtr,
    P2CONST(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_CONST) TimerSrcPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
/* detect NULL_PTR */
#if (STD_ON == TM_DEV_ERROR_DETECT)
    if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
    {
        TM_DET_REPORT(TM_SYNCTIMER1US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* Sync two clocks */
        TimerDstPtr->ui32RefTime = TimerSrcPtr->ui32RefTime;
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us32bit(VAR(uint8, AUTOMATIC) WaitingTimeMin)
{
    Std_ReturnType ret;
    uint32 RefTime;
    uint32 CurrentTime;

    /* get the start time */
    ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &RefTime);

    /* keep getting the current time and subtract it from start time until over timemin */
    do
    {
        if ((Std_ReturnType)E_OK == ret)
        {
            ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &CurrentTime);
        }
        else
        {
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_BUSYWAIT1US32BIT_ID, TM_E_HARDWARE_TIMER);
            break;
        }
    } while (((uint32)(CurrentTime - RefTime)) <= WaitingTimeMin);

    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"
#endif /* STD_ON == TM_1US32BIT */

#if (STD_ON == TM_100US32BIT)
/**
 * This service performs busy waiting by polling with a guaranteed minimum waiting time.
 * Service ID: 0x11
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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE)
Tm_ResetTimer100us32bit(P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981*/ /* MISRA Rule 2.2 */
    uint32 CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_RESETTIMER100US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt real time and assign to reference time of given clock */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_100US_32BIT, &CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            TimerPtr->ui32RefTime = CurrentTime;
        }
        else
        {
            /* gpt clock error */
            (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_RESETTIMER100US32BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * This service delivers the time difference (current time - reference time).
 * Service ID: 0x12
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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE)
Tm_GetTimeSpan100us32bit(
    P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint32, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType ret = E_NOT_OK; /* PRQA S 2981*/ /* MISRA Rule 2.2 */
    uint32 ui32CurrentTime;

#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
    {
        TM_DET_REPORT(TM_GETTIMESPAN100US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* get current gpt time, subtract it from reference time */
        ret = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_100US_32BIT, &ui32CurrentTime);
        if ((Std_ReturnType)E_OK == ret)
        {
            *TimeSpanPtr = (uint32)(ui32CurrentTime - TimerPtr->ui32RefTime);
        }
        else
        {
            /* gpt error, timeSpan = 0 */
            *TimeSpanPtr = 0U;
            (void)
                Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, TM_GETTIMESPAN100US32BIT_ID, TM_E_HARDWARE_TIMER);
        }
    }
    return ret;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * This service shifts the reference time of the timer instance.
 * Service ID: 0x13
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimeValue: Time value in us, the reference time has to be shifted. Range: 0-0xFFFFFF
 * Parameters(INOUT): TimerPtr: Pointer to a timer instance defined by the user.
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(void, TM_CODE)
Tm_ShiftTimer100us32bit(
    P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA) TimerPtr,
    VAR(uint32, AUTOMATIC) TimeValue)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if (NULL_PTR == TimerPtr)
    {
        TM_DET_REPORT(TM_SHIFTTIMER100US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* simply add TimeValue to refernce time */
        TimerPtr->ui32RefTime = (uint32)(TimerPtr->ui32RefTime + TimeValue);
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"

/**
 * This service synchronizes two timer instances.
 * Service ID: 0x14
 * Sync/Async: synchronous
 * Reentrancy: Reentrant but not for the same timer instance
 * Parameters(IN): TimerSrcPtr: Pointer to the source timer instance defined by the user.
 * Parameters(INOUT): NA
 * Parameters(OUT): TimerDstPtr: Pointer to the destination timer instance defined by the user.
 * Return value: NA
 *
 */
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(void, TM_CODE)
Tm_SyncTimer100us32bit(
    P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA) TimerDstPtr,
    P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_CONST) TimerSrcPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
#if (STD_ON == TM_DEV_ERROR_DETECT)
    /* detect NULL_PTR */
    if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
    {
        TM_DET_REPORT(TM_SYNCTIMER100US32BIT_ID, TM_E_PARAM_POINTER);
    }
    else
#endif /* STD_ON == TM_DEV_ERROR_DETECT */
    {
        /* sync two clocks */
        TimerDstPtr->ui32RefTime = TimerSrcPtr->ui32RefTime;
    }
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"
#endif /* STD_ON == TM_100US32BIT */

#if (STD_ON == TM_DEMO)
/**
 * This service delivers the time difference (current time - reference time).
 * Service ID: 0x15
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
#define TM_START_SEC_CODE
#include "Tm_MemMap.h"
/* PRQA S 3432, 1503 ++ */ /* MISRA Rule 20.7, 2.1 */
FUNC(Std_ReturnType, TM_CODE)
Tm_GetTimeSpan1ms32bit(
    P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_CONST) TimerPtr,
    P2VAR(uint32, AUTOMATIC, TM_APPL_DATA) TimeSpanPtr)
/* PRQA S 3432, 1503 -- */ /* MISRA Rule 20.7, 2.1 */
{
    Std_ReturnType result;

    /* get current timeSpan using 100us tick */
    result = Tm_GetTimeSpan100us32bit(TimerPtr, TimeSpanPtr);
    if (E_OK == result)
    {
        /* divide the result by 10, you get 1ms tick clock timeSpan */
        *TimeSpanPtr = *TimeSpanPtr / (uint32)10;
    }
    return result;
}
#define TM_STOP_SEC_CODE
#include "Tm_MemMap.h"
#endif /* STD_ON == TM_DEMO */
