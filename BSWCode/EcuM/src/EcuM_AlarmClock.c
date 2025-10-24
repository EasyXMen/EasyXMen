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
/*
 ************************************************************************************************************************
 **
 **  @file               : EcuM_AlarmClock.c
 **  @author             : qinchun.yang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : ECU state management
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "EcuM_Internal.h"
/* PRQA S 0553 EOF */ /* VL_EcuM_0553 */
#if (ECUM_ALARM_CLOCK_PRESENT == STD_ON)
/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ========================================== internal function declarations ======================================== */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * @brief Finds the alarm ID associated with a given user.
 * @details This function searches through the alarm clock configurations to find the index of the alarm associated with
 * the specified user.
 * @param[in] user : The user for which the alarm ID is to be found.
 * @return uint8
 * @retval alarmId : The index of the alarm associated with the user, or ECUM_MAX_ALARM_CLOCK_NUM if no matching alarm
 * is found.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64776
 */
ECUM_LOCAL uint8 EcuM_FindAlarmIdByUser(EcuM_UserType user);

/**
 * @brief Finds the next earliest alarm time for a given runtime context.
 * @details This function iterates through the user alarms in the runtime context to find the earliest non-zero alarm
 * time.
 * @param[in] pRt : Pointer to the runtime context structure containing user and master alarm information.
 * @return EcuM_TimeType
 * @retval lTime : The earliest non-zero alarm time, or 0 if no valid alarm is found.
 * @reentrant FALSE
 * @synchronous TRUE
 * @trace       CPD-64777
 */
ECUM_LOCAL EcuM_TimeType EcuM_FindNextEarliestAlarm(const EcuM_RunTimeLcType* pRt);
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* ============================================ internal data definitions =========================================== */
#define ECUM_START_SEC_VAR_CLEARED_16
#include "EcuM_MemMap.h"
/* time record for the current alarm */
ECUM_LOCAL uint16 EcuM_TimeRecord; /* PRQA S 3218 */ /* VL_EcuM_3218 */
#define ECUM_STOP_SEC_VAR_CLEARED_16
#include "EcuM_MemMap.h"
/* ============================================ external data definitions =========================================== */

/* ========================================== external function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

/* PRQA S 1503,2814,4603,2844 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_MultiReturn,VL_EcuM_4603,VL_EcuM_DerefInvalidPtr */
/**
 * Sets a relative wakeup alarm for a specified user. This function checks if the ECU Manager
 * is initialized and finds the alarm ID associated with the user. If the alarm ID is valid,
 * it sets the alarm time and updates the master alarm if the new time is earlier. If the
 * system is configured for development error detection, it reports any errors encountered.
 */
Std_ReturnType EcuM_SetRelWakeupAlarm(EcuM_UserType user, EcuM_TimeType time)
{
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;
    EcuM_TimeType       setWkTime;
    uint8               alarmId;
    Std_ReturnType      ret = E_NOT_OK;

    /* Init Check */
    if (FALSE == EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else
    {
        alarmId = EcuM_FindAlarmIdByUser(user);
        if (alarmId != ECUM_MAX_ALARM_CLOCK_NUM)
        {
            SchM_Enter_EcuM_GlobalClock();
            ret                     = E_OK;
            setWkTime               = pRt->GlobalClock + time;
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
        else
        {
            detErrorId = ECUM_E_INVALID_PAR;
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETRELWAKEUPALARM, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503,2814,4603,2844 -- */

/* PRQA S 1503,2814,4603,2844 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_MultiReturn,VL_EcuM_4603,VL_EcuM_DerefInvalidPtr */
/**
 * Sets an absolute wakeup alarm for a specified user. This function checks if the ECU Manager
 * is initialized and finds the alarm ID associated with the user. If the alarm ID is valid,
 * it sets the alarm time and updates the master alarm if the new time is earlier. If the
 * system is configured for development error detection, it reports any errors encountered.
 */
Std_ReturnType EcuM_SetAbsWakeupAlarm(EcuM_UserType user, EcuM_TimeType time)
{
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;
    uint8               alarmId;
    Std_ReturnType      ret = E_NOT_OK;

    /* Init Check */
    if (FALSE == EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else
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
        else
        {
            detErrorId = ECUM_E_INVALID_PAR;
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETABSWAKEUPALARM, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503,2814,4603,2844 -- */

/* PRQA S 1503,2814,4603,2844 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_MultiReturn,VL_EcuM_4603,VL_EcuM_DerefInvalidPtr */
/**
 * Aborts a wakeup alarm for a specified user. This function checks if the ECU Manager
 * is initialized and finds the alarm ID associated with the user. If the alarm ID is valid,
 * it aborts the alarm and updates the master alarm if necessary. If the system is configured
 * for development error detection, it reports any errors encountered.
 */
Std_ReturnType EcuM_AbortWakeupAlarm(EcuM_UserType user)
{
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;
    Std_ReturnType      ret        = E_NOT_OK;
    uint8               alarmId;

    /* Init Check */
    if (FALSE == EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else
    {
        alarmId = EcuM_FindAlarmIdByUser(user);
        if (alarmId != ECUM_MAX_ALARM_CLOCK_NUM)
        {
            SchM_Enter_EcuM_GlobalClock();
            if (pRt->UserAlarm[alarmId] != 0u)
            {
                if (pRt->MasterAlarm == pRt->UserAlarm[alarmId])
                {
                    pRt->MasterAlarm = EcuM_FindNextEarliestAlarm(pRt);
                }
                pRt->UserAlarm[alarmId] = 0u;
                ret                     = E_OK;
            }
            SchM_Exit_EcuM_GlobalClock();
        }
        else
        {
            detErrorId = ECUM_E_INVALID_PAR;
        }
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_ABORTWAKEUPALARM, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503,2814,4603,2844 -- */

/* PRQA S 1503,2814,4603,2844 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_MultiReturn,VL_EcuM_4603,VL_EcuM_DerefInvalidPtr */
/**
 * Retrieves the current global clock time. This function checks if the ECU Manager
 * is initialized and if the provided time pointer is valid. If both checks pass,
 * it retrieves the current global clock time and stores it in the provided time pointer.
 * If the system is configured for development error detection, it reports any errors encountered.
 */
Std_ReturnType EcuM_GetCurrentTime(EcuM_TimeType* time)
{
    Std_ReturnType      ret = E_NOT_OK;
    EcuM_RunTimeLcType* pRt;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
    if (FALSE == EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else if (NULL_PTR == time)
    {
        detErrorId = ECUM_E_NULL_POINTER;
    }
    else
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        SchM_Enter_EcuM_GlobalClock();
        *time = pRt->GlobalClock;
        SchM_Exit_EcuM_GlobalClock();
        ret = E_OK;
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETCURRENTTIME, detErrorId);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503,2814,4603,2844 -- */

/* PRQA S 1503,2814,4603,2844 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_MultiReturn,VL_EcuM_4603,VL_EcuM_DerefInvalidPtr */
/**
 * Retrieves the current wakeup time. This function checks if the ECU Manager
 * is initialized and if the provided time pointer is valid. If both checks pass,
 * it retrieves the current master alarm time and stores it in the provided time pointer.
 * If the system is configured for development error detection, it reports any errors encountered.
 */
Std_ReturnType EcuM_GetWakeupTime(EcuM_TimeType* time)
{
    Std_ReturnType      ret = E_NOT_OK;
    EcuM_RunTimeLcType* pRt;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    uint8 detErrorId = ECUM_E_OK;
    if (FALSE == EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else if (NULL_PTR == time)
    {
        detErrorId = ECUM_E_NULL_POINTER;
    }
    else
#else
    if (EcuM_isInitState(&pRt))
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON*/
    {
        SchM_Enter_EcuM_GlobalClock();
        *time = pRt->MasterAlarm;
        SchM_Exit_EcuM_GlobalClock();
        ret = E_OK;
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_GETWAKEUPTIME, detErrorId);
    }
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503,2814,4603,2844 -- */

/* PRQA S 1503,2814,4603,2844 ++ */ /* VL_QAC_NoUsedApi,VL_QAC_MultiReturn,VL_EcuM_4603,VL_EcuM_DerefInvalidPtr */
/**
 * Sets the global clock for the specified user.
 * Checks if the ECU is initialized and if the user has permission to set the clock.
 * If initialization is successful and the user has permission, updates the global clock.
 * Reports errors if development error detection is enabled.
 */
Std_ReturnType EcuM_SetClock(EcuM_UserType user, EcuM_TimeType time)
{
    EcuM_RunTimeLcType* pRt;
    uint8               detErrorId = ECUM_E_OK;
    Std_ReturnType      ret        = E_NOT_OK;
    if (FALSE == EcuM_isInitState(&pRt))
    {
        detErrorId = ECUM_E_UNINIT;
    }
    else
    {
        SchM_Enter_EcuM_GlobalClock();
        if (EcuM_UserCfgs[user].setClkAllowed == TRUE)
        {
            pRt->GlobalClock = time;
            ret              = E_OK;
        }
        SchM_Exit_EcuM_GlobalClock();
    }
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (ECUM_E_OK != detErrorId)
    {
        (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, ECUM_SID_SETCLOCK, detErrorId);
    }
#else
    (void)detErrorId;
#endif /*ECUM_DEV_ERROR_DETECT == STD_ON */
    return ret;
}
/* PRQA S 1503,2814,4603,2844 -- */

/* PRQA S 2814,1532 ++ */ /* VL_QAC_MultiReturn,VL_QAC_OneFunRef */
/**
 * Update the ECU-M clock by adding the main function period.
 */
void EcuM_UpdateEcuMClock(EcuM_RunTimeLcType* pRt)
{
    EcuM_TimeRecord += ECUM_MAINFUNC_PERIOD;
    /*Up to 1 second.*/
    if (ECUM_MS_TO_SECOND <= EcuM_TimeRecord)
    {
        SchM_Enter_EcuM_GlobalClock();
        EcuM_TimeRecord = 0u;
        pRt->GlobalClock++;
        if (pRt->GlobalClock >= pRt->MasterAlarm)
        {
            (*pRt->Wks.Validated) |= ECUM_WKSOURCE_ALARMCLOCK;
            pRt->MasterAlarm = EcuM_FindNextEarliestAlarm(pRt);
        }
        SchM_Exit_EcuM_GlobalClock();
    }
}
/* PRQA S 2814,1532 -- */

/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Cancel all alarms by resetting the user alarms and setting the master alarm to the maximum alarm value.
 */
void EcuM_CancellAlarms(void)
{
    EcuM_RunTimeLcType* pRt = EcuM_GetLcRunDataPtr();

    uint8 alarmIdx;

    for (alarmIdx = 0u; alarmIdx < ECUM_MAX_ALARM_CLOCK_NUM; alarmIdx++)
    {
        pRt->UserAlarm[alarmIdx] = 0u;
    }
    SchM_Enter_EcuM_GlobalClock();
    pRt->MasterAlarm = ECUM_MAX_ALARM_VALUES;
    SchM_Exit_EcuM_GlobalClock();
}
/* PRQA S 2814 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
/* ========================================== internal function definitions ========================================= */
#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"
/**
 * Find the alarm ID associated with a specific user.
 */
ECUM_LOCAL uint8 EcuM_FindAlarmIdByUser(EcuM_UserType user)
{
    uint8 alarmId = ECUM_MAX_ALARM_CLOCK_NUM;
    uint8 alarmIdx;

    for (alarmIdx = 0u; alarmIdx < ECUM_MAX_ALARM_CLOCK_NUM; alarmIdx++)
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
/* PRQA S 2814 ++ */ /* VL_QAC_MultiReturn */
/**
 * Find the next earliest alarm time among user alarms.
 */
ECUM_LOCAL EcuM_TimeType EcuM_FindNextEarliestAlarm(const EcuM_RunTimeLcType* pRt)
{
    EcuM_TimeType lTime     = 0u;
    boolean       firstFlag = TRUE;
    uint8         alarmIdx;

    for (alarmIdx = 0u; (alarmIdx < ECUM_MAX_ALARM_CLOCK_NUM) && (pRt->UserAlarm[alarmIdx] != pRt->MasterAlarm);
         alarmIdx++)
    {
        if (pRt->UserAlarm[alarmIdx] != 0u)
        {
            if (TRUE == firstFlag)
            {
                firstFlag = FALSE;
                lTime     = pRt->UserAlarm[alarmIdx];
            }
            else
            {
                if (lTime > pRt->UserAlarm[alarmIdx])
                {
                    /*Update earliest user alarm clock.*/
                    lTime = pRt->UserAlarm[alarmIdx];
                }
            }
        }
    }
    if (TRUE == firstFlag)
    {
        lTime = ECUM_MAX_ALARM_VALUES;
    }
    return lTime;
}
/* PRQA S 2814 -- */
#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#endif /*ECUM_ALARM_CLOCK_PRESENT == STD_ON*/
