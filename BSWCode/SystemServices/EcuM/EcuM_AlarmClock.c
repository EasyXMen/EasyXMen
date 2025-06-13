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
**  FILENAME    : EcuM_AlarmClock.c                                           **
**                                                                            **
**  Created on  :                                                             **
**  Author      : qinchun.yang                                                **
**  Vendor      :                                                             **
**  DESCRIPTION : Implement code for alarm clock if present                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>      <AUTHOR>        <REVISION LOG>
 *  V1.0.0       2020-06-28  qinchun.yang    R19_11 EcuM initial version.
 *  V2.0.0       2021-04-08  qinchun.yang    R1911 stable version.
 */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EcuM_Internal.h"

#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
static FUNC(uint8, ECUM_CODE) EcuM_FindAlarmIdByUser(EcuM_UserType user);

/*Find the next earliest user alarm clock value*/
static FUNC(EcuM_TimeType, ECUM_CODE) EcuM_FindNextEarliestAlarm(void);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
#define ECUM_START_SEC_VAR_CLEARED_16
#include "EcuM_MemMap.h"
static uint16 EcuM_TimeRecord;
#define ECUM_STOP_SEC_VAR_CLEARED_16
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Global Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Sets a user's wakeup alarm relative to the current point in time.
 * EcuM_SetRelWakeupAlarm is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x22
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, the user that wants to set the wakeup alarm.
 *                  time, relative time from now in seconds.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service failed
 *               ECUM_E_EARLIER_ACTIVE: An earlier alarm is already set
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SetRelWakeupAlarm(EcuM_UserType user, EcuM_TimeType time)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;
    EcuM_TimeType setWkTime;
    uint8 alarmId;
    Std_ReturnType ret = E_NOT_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if (FALSE == EcuM_IsInit)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETRELWAKEUPALARM, ECUM_E_UNINIT);
    }
    else
#endif
    {
        alarmId = EcuM_FindAlarmIdByUser(user);
        if (alarmId != ECUM_MAX_ALARM_CLOCK_NUM)
        {
            SchM_Enter_EcuM_GlobalClock();
            ret = E_OK;
            setWkTime = pRt->GlobalClock + time;
            pRt->UserAlarm[alarmId] = setWkTime;
            /*The relative time from now is earlier than the current wakeup time,
             * EcuM_SetRelWakeupAlarm shall update the wakeup time.*/
            if (setWkTime < pRt->MasterAlarm)
            {
                pRt->MasterAlarm = setWkTime;
            }
            else
            {
                /*The relative time from now is later than the current wakeup time.*/
                ret = ECUM_E_EARLIER_ACTIVE;
            }
            SchM_Exit_EcuM_GlobalClock();
        }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETRELWAKEUPALARM, ECUM_E_INVALID_PAR);
        }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    }
    return ret;
}

/**
 * Sets the user's wakeup alarm to an absolute point in time.
 * EcuM_SetAbsWakeupAlarm is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x23
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, the user that wants to set the wakeup alarm.
 *                  time, absolute time in seconds. Note that, absolute alarms
 *                        use knowledge of the current time.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service failed
 *               ECUM_E_EARLIER_ACTIVE: An earlier alarm is already set
 *               ECUM_E_PAST: The given point in time has already passed
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SetAbsWakeupAlarm(EcuM_UserType user, EcuM_TimeType time)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;
    uint8 alarmId;
    Std_ReturnType ret = E_NOT_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Init Check */
    if (FALSE == EcuM_IsInit)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETABSWAKEUPALARM, ECUM_E_UNINIT);
    }
    else
#endif
    {
        alarmId = EcuM_FindAlarmIdByUser(user);
        if (alarmId != ECUM_MAX_ALARM_CLOCK_NUM)
        {
            SchM_Enter_EcuM_GlobalClock();
            ret = E_OK;
            if (time > pRt->GlobalClock)
            {
                pRt->UserAlarm[alarmId] = time;
                /*The relative time from now is earlier than the current wakeup time,
                 * EcuM_SetRelWakeupAlarm shall update the wakeup time.*/
                if (time < pRt->MasterAlarm)
                {
                    pRt->MasterAlarm = time;
                }
                else
                {
                    /*The relative time from now is later than the current wakeup time.*/
                    ret = ECUM_E_EARLIER_ACTIVE;
                }
            }
            else
            {
                /*Time parameter is earlier than now.*/
                ret = ECUM_E_PAST;
            }
            SchM_Exit_EcuM_GlobalClock();
        }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETABSWAKEUPALARM, ECUM_E_INVALID_PAR);
        }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    }
    return ret;
}

/**
 * Aborts the wakeup alarm previously set by this user.
 * EcuM_AbortWakeupAlarm is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x24
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, the user that wants to cancel the wakeup alarm
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service failed
 *               ECUM_E_NOT_ACTIVE: No owned alarm found
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_AbortWakeupAlarm(EcuM_UserType user)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;
    Std_ReturnType ret = E_NOT_OK;
    uint8 alarmId;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == EcuM_IsInit)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_ABORTWAKEUPALARM, ECUM_E_UNINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        alarmId = EcuM_FindAlarmIdByUser(user);
        if (alarmId != ECUM_MAX_ALARM_CLOCK_NUM)
        {
            SchM_Enter_EcuM_GlobalClock();
            if (pRt->UserAlarm[alarmId] != 0u)
            {
                if (pRt->MasterAlarm == pRt->UserAlarm[alarmId])
                {
                    pRt->MasterAlarm = EcuM_FindNextEarliestAlarm();
                }
                pRt->UserAlarm[alarmId] = 0u;
                ret = E_OK;
            }
            SchM_Exit_EcuM_GlobalClock();
        }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        else
        {
            Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_ABORTWAKEUPALARM, ECUM_E_INVALID_PAR);
        }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    }
    return ret;
}

/**
 * Returns the current value of the EcuM clock (i.e. the time since battery connect).
 * EcuM_GetCurrentTime is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x25
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  time, absolute time in seconds since battery connect.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: time points to NULL or the module is not initialized
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetCurrentTime(P2VAR(EcuM_TimeType, AUTOMATIC, ECUM_APPL_DATA) time)
{
    Std_ReturnType ret = E_NOT_OK;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == EcuM_IsInit)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETCURRENTTIME, ECUM_E_UNINIT);
    }
    else if (NULL_PTR == time)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETCURRENTTIME, ECUM_E_NULL_POINTER);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        SchM_Enter_EcuM_GlobalClock();
        *time = EcuMRunData.GlobalClock;
        SchM_Exit_EcuM_GlobalClock();
        ret = E_OK;
    }
    return ret;
}

/**
 * Returns the current value of the master alarm clock (the minimum absolute
 * time of all user alarm clocks).
 * EcuM_GetWakeupTime is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x26
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): NA
 * Parameters(INOUT): NA
 * Parameters(OUT):  time, absolute time in seconds for next wakeup.
 *                        0xFFFFFFFF means no active alarm.
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: time points to NULL or the module is not initialized
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GetWakeupTime(P2VAR(EcuM_TimeType, AUTOMATIC, ECUM_APPL_DATA) time)
{
    Std_ReturnType ret = E_NOT_OK;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == EcuM_IsInit)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETWAKEUPTIME, ECUM_E_UNINIT);
    }
    else if (NULL_PTR == time)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETWAKEUPTIME, ECUM_E_NULL_POINTER);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        SchM_Enter_EcuM_GlobalClock();
        *time = EcuMRunData.MasterAlarm;
        SchM_Exit_EcuM_GlobalClock();
        ret = E_OK;
    }
    return ret;
}

/**
 * Sets the EcuM clock time to the provided value. This API is useful for
 * testing the alarm services; Alarms that take days to expire can be tested.
 * EcuM_SetClock is part of the ECU Manager Module port interface.
 * Service ID[hex]: 0x27
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): user, user that wants to set the clock
 *                  time, absolute time in seconds since battery connect.
 * Parameters(INOUT): NA
 * Parameters(OUT):  NA
 * Return value: E_OK: The service has succeeded
 *               E_NOT_OK: The service failed
 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SetClock(EcuM_UserType user, EcuM_TimeType time)
{
    Std_ReturnType ret = E_NOT_OK;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == EcuM_IsInit)
    {
        Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETWAKEUPTIME, ECUM_E_UNINIT);
    }
    else
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        SchM_Enter_EcuM_GlobalClock();
        if (EcuM_UserCfgs[user].setClkAllowed == TRUE)
        {
            EcuMRunData.GlobalClock = time;
            ret = E_OK;
        }
        SchM_Exit_EcuM_GlobalClock();
    }
    return ret;
}

/*Update Ecum global clock.(Use EcuMMainFunctionPeriod)*/
FUNC(void, ECUM_CODE)
EcuM_UpdateEcuMClock(void)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;

    EcuM_TimeRecord += ECUM_MAINFUNC_PERIOD;
    /*Up to 1 second.*/
    if (ECUM_MS_TO_SECOND <= EcuM_TimeRecord)
    {
        SchM_Enter_EcuM_GlobalClock();
        EcuM_TimeRecord = 0;
        pRt->GlobalClock++;
        if (pRt->GlobalClock >= pRt->MasterAlarm)
        {
            (*pRt->Wks.Validated) |= ECUM_WKSOURCE_ALARMCLOCK;
            pRt->MasterAlarm = EcuM_FindNextEarliestAlarm();
        }
        SchM_Exit_EcuM_GlobalClock();
    }
}

FUNC(void, ECUM_CODE)
EcuM_CancellAlarms(void)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;
    uint8 alarmIdx;

    for (alarmIdx = 0; alarmIdx < ECUM_MAX_ALARM_CLOCK_NUM; alarmIdx++)
    {
        pRt->UserAlarm[alarmIdx] = 0u;
    }
    SchM_Enter_EcuM_GlobalClock();
    pRt->MasterAlarm = (uint32)0xFFFFFFFF;
    SchM_Exit_EcuM_GlobalClock();
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
static FUNC(uint8, ECUM_CODE) EcuM_FindAlarmIdByUser(EcuM_UserType user)
{
    uint8 alarmId = ECUM_MAX_ALARM_CLOCK_NUM;
    uint8 alarmIdx;

    for (alarmIdx = 0; alarmIdx < ECUM_MAX_ALARM_CLOCK_NUM; alarmIdx++)
    {
        if (EcuM_AlarmClkCfgs[alarmIdx].alarmClkUser == user)
        {
            alarmId = alarmIdx;
            break;
        }
    }
    return alarmId;
}

/*Find the next earliest user alarm clock value*/
static FUNC(EcuM_TimeType, ECUM_CODE) EcuM_FindNextEarliestAlarm(void)
{
    P2VAR(EcuM_RunTimeType, AUTOMATIC, ECUM_VAR) pRt = &EcuMRunData;
    EcuM_TimeType time = 0u;
    boolean firstFlag = TRUE;
    uint8 alarmIdx;

    for (alarmIdx = 0; (alarmIdx < ECUM_MAX_ALARM_CLOCK_NUM) && (pRt->UserAlarm[alarmIdx] != pRt->MasterAlarm);
         alarmIdx++)
    {
        if (pRt->UserAlarm[alarmIdx] == 0u)
        {
            continue;
        }
        if (TRUE == firstFlag)
        {
            firstFlag = FALSE;
            time = pRt->UserAlarm[alarmIdx];
        }
        else
        {
            if (time > pRt->UserAlarm[alarmIdx])
            {
                /*Update earliest user alarm clock.*/
                time = pRt->UserAlarm[alarmIdx];
            }
        }
    }
    return time;
}
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
