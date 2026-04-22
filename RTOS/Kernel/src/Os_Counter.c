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
 *
 ************************************************************************************************************************
 **
 **  @file               : Os_Counter.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Counter API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Counter.h"
#include "Os_Alarm.h"
#include "Os_Sprot.h"
#include "Os_ScheduleTable.h"
#include "Os_Rpc.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Err.h"
#include "Os_Arti.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (CFG_COUNTER_MAX > 0U)
/**
 * @brief           Internally increments a counter and processes related alarms and schedule tables
 * @param[in]       counterId: ID of the counter to increment
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_IncrementCounterInternal(CounterType counterId);

/**
 * @brief           Retrieves the current value of a counter
 * @param[in]       counterId: ID of the counter to query
 * @param[out]      value: Pointer where to store the counter value
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetCounterValue(CounterType counterId, TickRefType value);

/**
 * @brief           Calculates elapsed ticks since a previous counter value
 * @param[in]       counterId: ID of the counter to query
 * @param[inout]    value: Previous counter value (updated with current value)
 * @param[out]      elapsedValue: Pointer where to store the elapsed ticks
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_GetElapsedValue(CounterType counterId, TickRefType value, TickRefType elapsedValue);
#endif

/* ========================================== external function definitions ========================================= */
#if (CFG_COUNTER_MAX > 0U)

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Init the counter control block
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitCounter(void)
/* PRQA S 1532 -- */
{
    Os_CoreIdType coreId = Os_GetCoreIdLocal();
    Os_CounterType idStartRange = Os_CounterRange[coreId].start;
    Os_CounterType idEndRange = Os_CounterRange[coreId].end;

    if (idStartRange > idEndRange)
    {
        Os_Panic();
    }

    Os_CCBType *pCCB = NULL_PTR;
    for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
    {
        pCCB = Os_CCB[i];
        pCCB->CounterCurVal = 0u;
        pCCB->CounterLastVal = 0u;
#if (CFG_ALARM_MAX > 0)
        pCCB->CounterAlmQue = INVALID_ALARM;
#endif

#if (CFG_SCHEDTBL_MAX > 0U)
        pCCB->CounterStListHead = NULL_PTR;
#endif
    }
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Calculate the absolute ticks that baseTick add the
 *                       offset
 */
Os_TickType Os_CalcAbsTicks(
    Os_TickType baseTick,
    Os_TickType offset,
    Os_CounterType counterId)
{
    Os_TickType counterAbsTicks;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Os_TickType counterTwiceMaxTicks = (Os_CounterCfg[counterId].CounterMaxAllowedValue * 2u) + 1u;
    /* PRQA S 3120 -- */

    if ((baseTick + offset) <= counterTwiceMaxTicks)
    {
        counterAbsTicks = baseTick + offset;
    }
    else
    {
        counterAbsTicks = (baseTick + offset) - (counterTwiceMaxTicks + 1u);
    }

    return counterAbsTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get the distance from baseTick to destTick
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
Os_TickType Os_GetDistance(Os_TickType baseTick, Os_TickType destTick, Os_CounterType counterId)
/* PRQA S 1532 -- */
{
    Os_TickType counterDistanceTicks;
    /* PRQA S 3120 ++ */ /* VL_QAC_MagicNum */
    Os_TickType counterTwiceMaxTicks = (Os_CounterCfg[counterId].CounterMaxAllowedValue * 2u) + 1u;
    /* PRQA S 3120 -- */

    if (baseTick <= destTick)
    {
        counterDistanceTicks = destTick - baseTick;
    }
    else
    {
        /* PRQA S 2911 ++ */ /* VL_Os_2911 */
        counterDistanceTicks = (destTick - baseTick) + (counterTwiceMaxTicks + 1u);
        /* PRQA S 2911 -- */
    }

    return counterDistanceTicks;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Increment counter internal
 */
OS_LOCAL void Os_IncrementCounterInternal(CounterType counterId)
{
    Os_CCBType *pCcb = Os_CCB[counterId];
    OS_HAL_DECLARE_CRITICAL();

    /* update the Counter value. */
    OS_HAL_ENTRY_CRITICAL();
    pCcb->CounterLastVal = pCcb->CounterCurVal;
    pCcb->CounterCurVal = Os_CalcAbsTicks(pCcb->CounterCurVal, 1U, counterId);
    OS_HAL_EXIT_CRITICAL();

/* Alarm. */
#if (CFG_ALARM_MAX > 0)
    Os_WorkAlarm(counterId);
#endif

/* Schedule table. */
#if (CFG_SCHEDTBL_MAX > 0U)
    Os_WorkSchedTbl(counterId);
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Increment Hardware counter
 */
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_IncrementHardCounter(CounterType counterId)
/* PRQA S 1532 -- */
{
    Os_IncrementCounterInternal(counterId);
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get current counter value
 */
OS_LOCAL void Os_GetCounterValue(CounterType counterId, TickRefType value)
{
    OS_HAL_DECLARE_CRITICAL();

    /* Timer type. */
    OS_HAL_ENTRY_CRITICAL();
    /* PRQA S 2834 ++ */ /* VL_Os_2834 */
    *value = Os_CCB[counterId]->CounterCurVal % (Os_CounterCfg[counterId].CounterMaxAllowedValue + 1u);
    /* PRQA S 2834 -- */
    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get elapsed counter value
 */
OS_LOCAL void Os_GetElapsedValue(
    CounterType counterId,
    TickRefType value,
    TickRefType elapsedValue)
{
    OS_HAL_DECLARE_CRITICAL();
    Os_TickType counterMaxAllowedValue = Os_CounterCfg[counterId].CounterMaxAllowedValue;

    OS_HAL_ENTRY_CRITICAL();
    /* PRQA S 2834 ++ */ /* VL_Os_2834 */
    Os_TickType counterCurval = Os_CCB[counterId]->CounterCurVal %
                                (counterMaxAllowedValue + 1u);

    *elapsedValue = ((counterCurval + counterMaxAllowedValue) -
                     (*value)) % counterMaxAllowedValue;
    /* PRQA S 2834 -- */

    *value = counterCurval;
    OS_HAL_EXIT_CRITICAL();
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Increment counter
 */
/* PRQA S 3006,1532,3408,6070,1503,1512 ++ */ /* VL_Os_3006, VL_QAC_OneFunRef, VL_Os_3408, VL_MTR_Os_STCAL, VL_QAC_NoUsedApi, VL_Os_1512 */
StatusType IncrementCounter(CounterType CounterID)
/* PRQA S 3006,1532,3408,6070,1503,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    Os_CoreIdType curCoreId = Os_GetCoreIdLocal();
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetSystemContext(curCoreId);
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_IncrementCounter);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_IncrementCounter_Start, CounterID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */
    StatusType err = E_OK;

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_INCREMENT_COUNTER,
        .ObjectType = OS_OBJECT_COUNTER,
        .ObjectID = (Os_AppObjectId)CounterID,
        /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
        .Address = NULL_PARA,
        /* PRQA S 1258 -- */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    /* SWS_Os_00285:If the input parameter <CounterID> in a call of
     * IncrementCounter() is not valid OR the counter is a hardware
     * counter,IncrementCounter() shall return E_OS_ID.  */
    if (Os_ObjectIDCheck((ObjectType)CounterID, (uint8)OS_OBJECT_COUNTER) != TRUE)
    {
        err = E_OS_ID;
    }
    else if (OS_COUNTER_HARDWARE == Os_CounterCfg[CounterID].CounterProperty)
    {
        err = E_OS_ID;
    }
    else
#endif
        /* SWS_Os_00629:A COUNTER belonging to an OS-Application shall be
         * incremented by the core on which the OS-Application resides. */
        if (curCoreId != OS_COUNTER_GET_COREID(CounterID))
        {
            err = E_OS_CORE;
        }
        else
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        /* PRQA S 3326 ++ */ /* VL_Os_3326 */
        if ((err = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 2004 */ /* VL_Os_2004 */
        /* PRQA S 3326 -- */
#endif
        {
            Os_IncrementCounterInternal(CounterID);
        }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_IncrementCounter(CounterID),
                          OSServiceId_IncrementCounter,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_IncrementCounter);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_IncrementCounter_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get current counter value
 */
/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi,VL_Os_3006,VL_Os_3408,VL_MTR_Os_STCAL,VL_Os_1512 */
StatusType GetCounterValue(CounterType CounterID, TickRefType Value)
/* PRQA S 1503,3006,3408,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    /* PRQA S 2983, 3678 ++ */ /* VL_Os_2983, VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 2983, 3678 -- */

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetCounterValue);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetCounterValue_Start, CounterID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */
    StatusType err = E_OK;

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_COUNTER_VALUE,
        .ObjectType = OS_OBJECT_COUNTER,
        .ObjectID = (Os_AppObjectId)CounterID,
        /* PRQA S 0306 ++ */ /* VL_Os_0306 */
        .Address = (uint32)Value,
        /* PRQA S 0306 -- */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    Os_CoreIdType curCoreId = Os_GetCoreIdLocal();
    if (Os_ObjectIDCheck((ObjectType)CounterID, (uint8)OS_OBJECT_COUNTER) != TRUE)
    {
        err = E_OS_ID;
    }
    else if (curCoreId != OS_COUNTER_GET_COREID(CounterID))
    {
        err = E_OS_CORE;
    }
    else
#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    /* PRQA S 3326 ++ */ /* VL_Os_3326 */
    if ((err = Os_ServiceProtCheck(pScb, &SprotParam)) == E_OK) /* PRQA S 2004 */ /* VL_Os_2004 */
    /* PRQA S 3326 -- */
#endif
    {
        Os_GetCounterValue(CounterID, Value);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_GetCounterValue(CounterID, Value), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_GetCounterValue,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1258, 2997, 2995 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1258, VL_Os_2997, VL_Os_2995 */
    OSRtiExitApi(pScb, OSApiId_GetCounterValue);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetCounterValue_Return, (err == E_OK ) ? *Value : OS_TICK_INVALID);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/  
    /* PRQA S 3432, 4544, 1258, 2997, 2995 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Get elapsed counter value
 */
/* PRQA S 1503,6030,3408,3006,6070,1512 ++ */ /* VL_QAC_NoUsedApi, VL_MTR_Os_STMIF, VL_Os_3408, VL_Os_3006, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType GetElapsedValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue)
/* PRQA S 1503,6030,3408,3006,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006 ++ */ /* VL_Os_1006 */
    OS_HAL_ENTER_KERNEL(); /* PRQA S 1021 */ /* VL_Os_1021 */
    /* PRQA S 1006 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */

    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetElapsedValue);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetElapsedValue_Start, CounterID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141-- */
    StatusType err = E_OK;

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ServicePortParamType SprotParam = {
        .AllowedContext = OS_SERVICEPORT_CHECK_GET_ELAPSED_COUNTER_VALUE,
        .ObjectType = OS_OBJECT_COUNTER,
        .ObjectID = (Os_AppObjectId)CounterID,
        /* PRQA S 0306 ++ */ /* VL_Os_0306 */
        .Address = (uint32)ElapsedValue,
        /* PRQA S 0306 -- */
    };
#endif

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    Os_CoreIdType curCoreId = Os_GetCoreIdLocal();
    if (Os_ObjectIDCheck((ObjectType)CounterID, (uint8)OS_OBJECT_COUNTER) != TRUE)
    {
        err = E_OS_ID;
    }
    else if (curCoreId != OS_COUNTER_GET_COREID(CounterID))
    {
        err = E_OS_CORE;
    }
    else
#endif
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    /* PRQA S 3326 ++ */ /* VL_Os_3326 */
    if ((err = Os_ServiceProtCheck(pScb, &SprotParam)) != E_OK)
    /* PRQA S 3326 -- */
    {
        /* do nothing */
    }
    /* PRQA S 0306 ++ */ /* VL_Os_0306 */
    else if (Os_AddressWritable(pScb, (uint32)Value) != TRUE)
    /* PRQA S 0306 -- */
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else
#endif
    if (*Value > Os_CounterCfg[CounterID].CounterMaxAllowedValue)
    {
        err = E_OS_VALUE;
    }
    else
    {
        Os_GetElapsedValue(CounterID, Value, ElapsedValue);
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetElapsedValue(CounterID, Value, ElapsedValue),
                          OSServiceId_GetElapsedValue,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544, 1258 ++ */ /* VL_Os_3432, VL_Os_4544, VL_Os_1258 */
    OSRtiExitApi(pScb, OSApiId_GetElapsedValue);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetElapsedValue_Return, (err == E_OK ) ? *ElapsedValue : OS_TICK_INVALID);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/  
    /* PRQA S 3432, 4544, 1258 -- */
    /* PRQA S 3138, 3141 -- */
    UNUSED_PARAMETER(pScb);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

/*=======[E N D   O F   F I L E]==============================================*/
