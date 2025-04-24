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
**  FILENAME    : Os_Alarm.c                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : alarm manager                                               **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os_Internal.h"
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/
#if (CFG_ALARM_MAX > 0)
/* PRQA S 0791++ */ /* MISRA Rule 5.4 */ /*OS_ALARM_SEGMENTNAME_SIMILAR_004*/
#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
static P2CONST(Os_AlarmCfgType, AUTOMATIC, OS_VAR) Os_AlarmCfg;
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"

#define OS_START_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_ALARM_COMPILER_001 */
static P2VAR(Os_ACBType, AUTOMATIC, OS_VAR) Os_ACB;
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#define OS_STOP_SEC_VAR_CLONE_PTR
#include "Os_MemMap.h"
/* PRQA S 0791-- */ /* MISRA Rule 5.4 */

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_InsertAlarm(Os_AlarmType alarmid, Os_AlarmRefType almque, Os_TickType curTick);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
static FUNC(void, OS_CODE) Os_DeleteAlarm(Os_AlarmType alarmid, Os_AlarmRefType almque);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/*
 * Brief                <Init the alarm control block>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InitSystem>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitAlarm(void)
{
    VAR(Os_AlarmType, OS_VAR) i;
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_ALARM_COMPILER_001 */
    P2VAR(Os_ACBType, AUTOMATIC, OS_VAR) pACB;
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

#if (CFG_AUTO_ALARM_MAX > 0U)
    P2CONST(Os_AlarmCfgType, AUTOMATIC, OS_VAR) pAlarmCfg = NULL_PTR;
    VAR(Os_CounterType, OS_VAR) alarmCounter;
    VAR(Os_AppModeType, OS_VAR) alarmAppModeMask;
    VAR(Os_AlarmType, OS_VAR) AlarmQue;
#endif

    VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;
    Os_AlarmCfg = Os_AlarmCfg_Inf[coreId];
    Os_ACB = Os_ACB_Inf[coreId];
    Os_SCB.sysAlarmMax = Os_CfgAlarmMax_Inf[coreId];

    if (Os_AlarmCfg != NULL_PTR)
    {
#if (CFG_ALARM_MAX > 0)
        for (i = 0U; i < Os_SCB.sysAlarmMax; i++)
        {
            pACB = &Os_ACB[i];
            pACB->alarmStart = 0U;
            pACB->alarmCycle = 0U;
            pACB->alarmPre = i;
            pACB->alarmNext = i;

#if (CFG_AUTO_ALARM_MAX > 0U)
            pAlarmCfg = &Os_AlarmCfg[i];
            if (NULL_PTR != pAlarmCfg->osAlarmAutostartRef)
            {
                alarmAppModeMask = pAlarmCfg->osAlarmAutostartRef->osAlarmAppMode;
                if (0U != (alarmAppModeMask & Os_SCB.sysActiveAppMode))
                {
                    alarmCounter = pAlarmCfg->osAlarmCounter;
                    pACB->alarmStart = pAlarmCfg->osAlarmAutostartRef->osAlarmStartTime;
                    pACB->alarmCycle = pAlarmCfg->osAlarmAutostartRef->osAlarmCycleTime;
                    AlarmQue = Os_CCB[alarmCounter].counterAlmQue;
                    Os_InsertAlarm(i, &AlarmQue, Os_CCB[alarmCounter].counterCurVal);
                    Os_CCB[alarmCounter].counterAlmQue = AlarmQue;
                }
            }
#endif /* CFG_AUTO_ALARM_MAX > 0U */
        }
#endif /* CFG_ALARM_MAX > 0 */
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service GetAlarm returns the relative value
 *                       in ticksbefore the alarm <AlarmID> expires>
 * Service ID           <>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Tick[out]      <Relative value in ticks before the alarm <AlarmID>
 *                       expires>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_GetAlarm(AlarmType AlarmID, TickRefType Tick)
{
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_CounterType, OS_VAR) osCounterId;
    VAR(Os_TickType, OS_VAR) counterCurval;

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA  Dir-4.9,Rule-17.8*/ /* OS_ALARM_MACRO_004 */
    AlarmID = Os_GetObjLocalId(AlarmID);
    /* PRQA S 3469, 1338 -- */ /* MISRA  Dir-4.9,Rule-17.8*/
#endif

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
    if (Os_ACB[AlarmID].alarmNext == AlarmID)
    {
        err = E_OS_NOFUNC;
    }
    else
    {
        osCounterId = Os_AlarmCfg[AlarmID].osAlarmCounter;

        /* Driven by hardware counter or sofrware counter. */
        counterCurval = Os_CCB[osCounterId].counterCurVal;
        *Tick = Os_GetDistance(counterCurval, Os_ACB[AlarmID].alarmStart, osCounterId);
    }
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Os_WorkAlarm>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-CounterID[in]  <reference to counter>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_WorkAlarm(Os_CounterType CounterID)
{
    VAR(Os_TickType, OS_VAR) counterCurval;
    VAR(Os_TickType, OS_VAR) counterMaxAllowedValue;
    VAR(boolean, OS_VAR) done = FALSE;
    VAR(Os_AlarmType, OS_VAR) AlarmQue;
    VAR(Os_AlarmType, OS_VAR) alarmCurVal;
    VAR(Os_AlarmType, OS_VAR) alarmNextVal;

    OS_ARCH_DECLARE_CRITICAL();

#if (CFG_OSAPPLICATION_MAX > 0U)
    VAR(Os_ApplicationType, OS_VAR) bakappID = Os_SCB.sysRunningAppID;
#endif

    counterMaxAllowedValue = Os_CounterCfg[CounterID].osCounterMaxAllowedValue;

    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
    counterCurval = Os_CCB[CounterID].counterCurVal;
    alarmNextVal = Os_CCB[CounterID].counterAlmQue;

    while (FALSE == done)
    {
        if (OS_ALARM_INVALID == alarmNextVal)
        {
            done = TRUE;
        }
        else if (Os_GetDistance(Os_ACB[alarmNextVal].alarmStart, counterCurval, CounterID) <= counterMaxAllowedValue)
        {
            alarmCurVal = alarmNextVal;
            alarmNextVal = Os_ACB[alarmNextVal].alarmNext;

#if (CFG_OSAPPLICATION_MAX > 0U)
            Os_SCB.sysRunningAppID = Os_ObjectAppCfg[OBJECT_ALARM][alarmCurVal].hostApp;
#endif

            Os_CCB[CounterID].counterAlmQue = alarmNextVal;

            if (OS_ALARM_INVALID != alarmNextVal)
            {
                Os_ACB[alarmNextVal].alarmPre = OS_ALARM_INVALID;
            }

            Os_ACB[alarmCurVal].alarmPre = alarmCurVal;
            Os_ACB[alarmCurVal].alarmNext = alarmCurVal;

            if (NULL_PTR != Os_AlarmCfg[alarmCurVal].osAlarmCallback)
            {
                /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
                OS_ARCH_EXIT_CRITICAL();
                /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
                Os_AlarmCfg[alarmCurVal].osAlarmCallback();
                /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
                OS_ARCH_ENTRY_CRITICAL();
                /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
            }

            if (Os_ACB[alarmCurVal].alarmCycle > 0U)
            {
                Os_ACB[alarmCurVal].alarmStart =
                    Os_CalcAbsTicks(Os_ACB[alarmCurVal].alarmStart, Os_ACB[alarmCurVal].alarmCycle, CounterID);
                AlarmQue = Os_CCB[CounterID].counterAlmQue;
                Os_InsertAlarm(alarmCurVal, &AlarmQue, Os_CCB[CounterID].counterLastVal);
                Os_CCB[CounterID].counterAlmQue = AlarmQue;
            }
        }
        else
        {
            done = TRUE;
        }
    }
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */

#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_SCB.sysRunningAppID = bakappID;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Insert an alarm in alarm queue>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-alarmid[in]    <reference to alarm>
 * param-curTick[in]    <current tick value>
 * Param-Name[out]      <None>
 * Param-almque[in/out] <reference to alarm queue>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_InsertAlarm(Os_AlarmType alarmid, Os_AlarmRefType almque, Os_TickType curTick)
{
    VAR(Os_TickType, OS_VAR) alarmTick;
    VAR(Os_AlarmType, OS_VAR) pre;
    VAR(Os_AlarmType, OS_VAR) next;
    VAR(boolean, OS_VAR) done = FALSE;

    if (alarmid < Os_SCB.sysAlarmMax)
    {
        alarmTick = Os_ACB[alarmid].alarmStart;

        pre = OS_ALARM_INVALID;
        next = *almque;

        if (curTick < alarmTick)
        {
            while (done != TRUE)
            {
                if (OS_ALARM_INVALID == next)
                {
                    done = TRUE;
                }
                else if ((Os_ACB[next].alarmStart >= curTick) && (Os_ACB[next].alarmStart <= alarmTick))
                {
                    pre = next;
                    next = Os_ACB[next].alarmNext;
                }
                else
                {
                    done = TRUE;
                }
            }
        }
        else
        {
            while (done != TRUE)
            {
                if (OS_ALARM_INVALID == next)
                {
                    done = TRUE;
                }
                else if ((Os_ACB[next].alarmStart >= curTick) || (Os_ACB[next].alarmStart <= alarmTick))
                {
                    pre = next;
                    next = Os_ACB[next].alarmNext;
                }
                else
                {
                    done = TRUE;
                }
            }
        }

        Os_ACB[alarmid].alarmPre = pre;
        Os_ACB[alarmid].alarmNext = next;

        if (pre != OS_ALARM_INVALID)
        {
            Os_ACB[pre].alarmNext = alarmid;
        }
        else
        {
            *almque = alarmid;
        }

        if (next != OS_ALARM_INVALID)
        {
            Os_ACB[next].alarmPre = alarmid;
        }
    }

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceAlarmStart(alarmid);
#endif /* TRUE == CFG_TRACE_ENABLE */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Delete an alarm in alarm queue>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-alarmid[in]    <reference to alarm>
 * Param-Name[out]      <None>
 * Param-almque[in/out] <reference to alarm queue>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
static FUNC(void, OS_CODE) Os_DeleteAlarm(Os_AlarmType alarmid, Os_AlarmRefType almque)
{
    VAR(Os_AlarmType, OS_VAR) pre;
    VAR(Os_AlarmType, OS_VAR) next;

    if (alarmid >= Os_SCB.sysAlarmMax)
    {
        /*nothing to do*/
    }
    else
    {
        pre = Os_ACB[alarmid].alarmPre;
        next = Os_ACB[alarmid].alarmNext;

        if (pre != OS_ALARM_INVALID)
        {
            Os_ACB[pre].alarmNext = next;
        }
        else
        {
            *almque = next;
        }

        if (next != OS_ALARM_INVALID)
        {
            Os_ACB[next].alarmPre = pre;
        }

        Os_ACB[alarmid].alarmPre = alarmid;
        Os_ACB[alarmid].alarmNext = alarmid;
    }

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceAlarmStop(alarmid);
#endif /* TRUE == CFG_TRACE_ENABLE */

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Internal implementation of OS service: CancelAlarm>
 * Service ID           <OSServiceId_CancelAlarm>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_CancelAlarm(AlarmType AlarmID)
{
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_CounterType, OS_VAR) alarmCounter;
    VAR(Os_AlarmType, OS_VAR) AlarmQue;

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA  Dir-4.9,Rule-17.8*/ /* OS_ALARM_MACRO_004 */
    AlarmID = Os_GetObjLocalId(AlarmID);
    /* PRQA S 3469, 1338 -- */ /* MISRA  Dir-4.9,Rule-17.8*/
#endif

    /* Standard Status */
    OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
    if (Os_ACB[AlarmID].alarmNext == AlarmID)
    {
        err = E_OS_NOFUNC;
    }
    else
    {
        alarmCounter = Os_AlarmCfg[AlarmID].osAlarmCounter;
        AlarmQue = Os_CCB[alarmCounter].counterAlmQue;
        Os_DeleteAlarm(AlarmID, &AlarmQue);
        Os_CCB[alarmCounter].counterAlmQue = AlarmQue;
    }
    OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service occupies the alarm <AlarmID> element.
 *                       When <start> ticks are reached, the task assigned to
 *                       the alarm <AlarmID> is activated or the assigned event
 *                       (only for extendedtasks) is set or the alarm-callback
 *                       routine is called.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * param-start[in]      <Absolute value in ticks>
 * param-cycle[in]      <Cycle value in case of cyclic alarm. In case of single
 *                       alarms,cycle has to be zero.>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
{
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_AlarmType, OS_VAR) AlarmQue;
    VAR(Os_TickType, OS_VAR) osCounterCurVal;
    VAR(Os_CounterType, OS_VAR) alarmCounter;
    VAR(Os_TickType, OS_VAR) counterMaxAllowedValue;

    OS_ARCH_DECLARE_CRITICAL();

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA  Dir-4.9,Rule-17.8*/ /* OS_ALARM_MACRO_004 */
    AlarmID = Os_GetObjLocalId(AlarmID);
    /* PRQA S 3469, 1338 -- */ /* MISRA  Dir-4.9,Rule-17.8*/
#endif

    alarmCounter = Os_AlarmCfg[AlarmID].osAlarmCounter;
    counterMaxAllowedValue = Os_CounterCfg[alarmCounter].osCounterMaxAllowedValue;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    Os_TickType counterMinCycle = Os_CounterCfg[alarmCounter].osCounterMinCycle;
    /*start is unsigned long integer, so not check smaller than 0*/
    if ((0U == start) || (start > counterMaxAllowedValue))
    {
        err = E_OS_VALUE;
    }
    else if ((cycle != 0U) && ((cycle > counterMaxAllowedValue) || (cycle < counterMinCycle)))
    {
        err = E_OS_VALUE;
    }
    else
    {
        /*nothing to do*/
    }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */

    if ((StatusType)E_OK == err)
    {
        OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        if (Os_ACB[AlarmID].alarmNext != AlarmID)
        {
            OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
            err = E_OS_STATE;
        }
        else
        {
            osCounterCurVal = Os_CCB[alarmCounter].counterCurVal;

            if (Os_GetDistance(osCounterCurVal, start, alarmCounter) <= counterMaxAllowedValue)
            {
                Os_ACB[AlarmID].alarmStart = start;
            }
            else
            {
                Os_ACB[AlarmID].alarmStart = start + counterMaxAllowedValue + 1U;
            }

            Os_ACB[AlarmID].alarmCycle = cycle;
            AlarmQue = Os_CCB[alarmCounter].counterAlmQue;
            Os_InsertAlarm(AlarmID, &AlarmQue, Os_CCB[alarmCounter].counterCurVal);
            Os_CCB[alarmCounter].counterAlmQue = AlarmQue;
            OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        }
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service GetAlarmBase reads the alarm base
 *                       characteristics. The return value <Info> is a structure in which
 *                       the information of data type Os_AlarmBaseType is stored.>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Info[out]      <Reference to structure with constants of the alarm base>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
{
    VAR(Os_CounterType, OS_VAR) alarmCounter;

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA  Dir-4.9,Rule-17.8*/ /* OS_ALARM_MACRO_004 */
    AlarmID = Os_GetObjLocalId(AlarmID);
    /* PRQA S 3469, 1338 -- */ /* MISRA  Dir-4.9,Rule-17.8*/
#endif
    alarmCounter = Os_AlarmCfg[AlarmID].osAlarmCounter;

    Info->maxallowedvalue = Os_CounterCfg[alarmCounter].osCounterMaxAllowedValue;
    Info->mincycle = Os_CounterCfg[alarmCounter].osCounterMinCycle;
    Info->ticksperbase = Os_CounterCfg[alarmCounter].osCounterTicksPerBase;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service occupies the alarm <AlarmID> element.
 *                       After <increment> ticks have elapsed, the task assigned
 *                       to the alarm <AlarmID> is activated or the assigned
 *                       event (only for extended tasks) is set or the
 *                       alarm-callback routine is called>
 * Service ID           <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * param-increment[in]  <Relative value in ticks>
 * param-cycle[in]      <Cycle value in case of cyclic alarm. In case of single
 *                       alarms,cycle has to be zero.>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) Os_SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
{
    VAR(StatusType, OS_VAR) err = E_OK;
    VAR(Os_AlarmType, OS_VAR) AlarmQue;
    VAR(Os_CounterType, OS_VAR) alarmCounter;

#if (OS_AUTOSAR_CORES > 1)
    /* PRQA S 3469, 1338 ++ */ /* MISRA  Dir-4.9,Rule-17.8*/ /* OS_ALARM_MACRO_004 */
    AlarmID = Os_GetObjLocalId(AlarmID);
    /* PRQA S 3469, 1338 -- */ /* MISRA  Dir-4.9,Rule-17.8*/
#endif

    /*
     * OSEKOS223: the behaviour of this case is up to the implementation
     * AutoSarOS, @SWS_Os_00304: If in a call to SetRelAlarm() the
     * parameter increment is set to zero, the service shall
     * return E_OS_VALUE in standard and extended status
     *
     * we meet SWS_Os_00304 requirement in here.
     */
    if (0U == increment)
    {
        err = E_OS_VALUE;
    }
    else
    {
        alarmCounter = Os_AlarmCfg[AlarmID].osAlarmCounter;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
        Os_TickType counterMinCycle = Os_CounterCfg[alarmCounter].osCounterMinCycle;
        Os_TickType counterMaxAllowedValue = Os_CounterCfg[alarmCounter].osCounterMaxAllowedValue;

        if (increment > counterMaxAllowedValue)
        {
            err = E_OS_VALUE;
        }
        else if ((cycle != 0U) && ((cycle > counterMaxAllowedValue) || (cycle < counterMinCycle)))
        {
            err = E_OS_VALUE;
        }
        else
        {
            /*nothing to do*/
        }
#endif /* OS_STATUS_EXTENDED == CFG_STATUS */
    }

    if ((StatusType)E_OK == err)
    {
        OS_ARCH_DECLARE_CRITICAL();
        OS_ARCH_ENTRY_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        if (Os_ACB[AlarmID].alarmNext != AlarmID)
        {
            OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
            err = E_OS_STATE;
        }
        else
        {
            /* Timer type. */
            Os_ACB[AlarmID].alarmStart = Os_CalcAbsTicks(Os_CCB[alarmCounter].counterCurVal, increment, alarmCounter);
            Os_ACB[AlarmID].alarmCycle = cycle;
            AlarmQue = Os_CCB[alarmCounter].counterAlmQue;
            Os_InsertAlarm(AlarmID, &AlarmQue, Os_CCB[alarmCounter].counterCurVal);
            Os_CCB[alarmCounter].counterAlmQue = AlarmQue;
            OS_ARCH_EXIT_CRITICAL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        }
    }
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service GetAlarm returns the relative value
 *                       in ticksbefore the alarm <AlarmID> expires>
 * Service ID           <0xf1>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Tick[out]      <Relative value in ticks before the alarm <AlarmID>
 *                       expires>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) GetAlarm(AlarmType AlarmID, TickRefType Tick)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetAlarm);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Tick)
    {
        err = E_OS_PARAM_POINTER;
    }
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_ALARM_MACRO_005 */
    else if (CHECK_ID_INVALID(AlarmID, Os_CfgAlarmMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED != CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_ALARM) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_ALARM_TYPE_CAST_002 */
    else if (Os_AddressWritable((uint32)Tick) != TRUE)
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_ALARM, AlarmID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00640:GetAlarm shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_GetObjCoreId(AlarmID);
        /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_GetAlarm,
                .srvPara0 = (uint32)AlarmID,
                /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_ALARM_TYPE_CAST_003 */
                .srvPara1 = (uint32)Tick,
                /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
                .srvPara2 = (uint32)NULL_PARA,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_GetAlarm(AlarmID, Tick);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetAlarm(AlarmID, Tick), OSServiceId_GetAlarm, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetAlarm);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service GetAlarmBase reads the alarm base
 *                       characteristics. The return value <Info> is a structure in which
 *                       the information of data type Os_AlarmBaseType is stored.>
 * Service ID           <0xf0>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Info[out]      <Reference to structure with constants of the alarm base>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_GetAlarmBase);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Info)
    {
        err = E_OS_PARAM_POINTER;
    }
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_ALARM_MACRO_005 */
    else if (CHECK_ID_INVALID(AlarmID, Os_CfgAlarmMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED != CFG_STATUS */

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_GET_ALARM_BASE) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_ALARM_TYPE_CAST_002 */
    else if (Os_AddressWritable((uint32)Info) != TRUE)
    /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_ALARM, AlarmID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00639:GetAlarmBase shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        Os_CoreIdType coreId = Os_GetObjCoreId(AlarmID);
        /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_GetAlarmBase,
                .srvPara0 = (uint32)AlarmID,
                /* PRQA S 0306 ++ */ /* MISRA Rule 11.4 */ /* OS_ALARM_TYPE_CAST_003 */
                .srvPara1 = (uint32)Info,
                /* PRQA S 0306 -- */ /* MISRA Rule 11.4 */
                .srvPara2 = (uint32)NULL_PARA,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            Os_GetAlarmBase(AlarmID, Info);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetAlarmBase(AlarmID, Info), OSServiceId_GetAlarmBase, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_GetAlarmBase);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service cancels the alarm <AlarmID>>
 * Service ID           <0xf4>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) CancelAlarm(AlarmType AlarmID)
{
    OS_ENTER_KERNEL();
    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_CancelAlarm);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_ALARM_MACRO_005 */
    if (CHECK_ID_INVALID(AlarmID, Os_CfgAlarmMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED != CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_CANCEL_ALARM) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_ALARM, AlarmID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00638:CancelAlarm shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        Os_CoreIdType coreId = Os_GetObjCoreId(AlarmID);
        /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_CancelAlarm,
                .srvPara0 = (uint32)AlarmID,
                .srvPara1 = (uint32)NULL_PARA,
                .srvPara2 = (uint32)NULL_PARA,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_CancelAlarm(AlarmID);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_CancelAlarm(AlarmID), OSServiceId_CancelAlarm, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_CancelAlarm);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service occupies the alarm <AlarmID> element.
 *                       After <increment> ticks have elapsed, the task assigned
 *                       to the alarm <AlarmID> is activated or the assigned
 *                       event (only for extended tasks) is set or the
 *                       alarm-callback routine is called>
 * Service ID           <0xf2>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * param-increment[in]  <Relative value in ticks>
 * param-cycle[in]      <Cycle value in case of cyclic alarm. In case of single
 *                       alarms,cycle has to be zero.>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_SetRelAlarm);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_ALARM_MACRO_005 */
    if (CHECK_ID_INVALID(AlarmID, Os_CfgAlarmMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED != CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_SET_REL_ALARM) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_ALARM, AlarmID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00636:SetRelAlarm shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        Os_CoreIdType coreId = Os_GetObjCoreId(AlarmID);
        /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_SetRelAlarm,
                .srvPara0 = (uint32)AlarmID,
                .srvPara1 = (uint32)increment,
                .srvPara2 = (uint32)cycle,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_SetRelAlarm(AlarmID, increment, cycle);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetRelAlarm(AlarmID, increment, cycle), OSServiceId_SetRelAlarm, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_SetRelAlarm);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <The system service occupies the alarm <AlarmID> element.
 *                       When <start> ticks are reached, the task assigned to
 *                       the alarm <AlarmID> is activated or the assigned event
 *                       (only for extendedtasks) is set or the alarm-callback
 *                       routine is called.>
 * Service ID           <0xf3>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Reentrant>
 * param-AlarmID[in]    <reference to alarm>
 * param-start[in]      <Absolute value in ticks>
 * param-cycle[in]      <Cycle value in case of cyclic alarm. In case of single
 *                       alarms,cycle has to be zero.>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * return               <StatusType>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(StatusType, OS_CODE) SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
{
    OS_ENTER_KERNEL();

    VAR(StatusType, OS_VAR) err = E_OK;

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceEnter(OSServiceId_SetAbsAlarm);
#endif /* TRUE == CFG_TRACE_ENABLE */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_ALARM_MACRO_005 */
    if (CHECK_ID_INVALID(AlarmID, Os_CfgAlarmMax_Inf))
    /* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
    {
        err = E_OS_ID;
    }
    else
#endif /* OS_STATUS_EXTENDED != CFG_STATUS */
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        if (Os_WrongContext(OS_CONTEXT_SET_ABS_ALARM) != TRUE)
    {
        err = E_OS_CALLEVEL;
    }
    else if (Os_IgnoreService() != TRUE)
    {
        err = E_OS_DISABLEDINT;
    }
    else if (Os_CheckObjAcs(OBJECT_ALARM, AlarmID) != TRUE)
    {
        err = E_OS_ACCESS;
    }
    else
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
    {
/* SWS_Os_00637:SetAbsAlarm shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
        /* PRQA S 3469 ++ */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
        Os_CoreIdType coreId = Os_GetObjCoreId(AlarmID);
        /* PRQA S 3469 -- */ /* MISRA  Dir-4.9*/
        if (coreId != Os_SCB.sysCore)
        {
            RpcInputType rpcData = {
                .sync = RPC_SYNC,
                .remoteCoreId = coreId,
                .serviceId = OSServiceId_SetAbsAlarm,
                .srvPara0 = (uint32)AlarmID,
                .srvPara1 = (uint32)start,
                .srvPara2 = (uint32)cycle,
            };
            err = Os_RpcCallService(&rpcData);
        }
        else
#endif /* OS_AUTOSAR_CORES > 1 */
        {
            err = Os_SetAbsAlarm(AlarmID, start, cycle);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetAbsAlarm(AlarmID, start, cycle), OSServiceId_SetAbsAlarm, err);
    }
#endif

#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceServiceExit(OSServiceId_SetAbsAlarm);
#endif /* TRUE == CFG_TRACE_ENABLE */

    OS_EXIT_KERNEL(); /* PRQA S 3469 */ /* MISRA  Dir-4.9*/ /* OS_ALARM_MACRO_004 */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* CFG_ALARM_MAX > 0 */

/*=======[E N D   O F   F I L E]==============================================*/
