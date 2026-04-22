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
 **  @file               : Os_Alarm.c
 **  @author             : i-soft-os
 **  @date               : 2025/02/10
 **  @vendor             : isoft
 **  @description        : Os source file for Alarm API implementations
 **
 ***********************************************************************************************************************/

/* =================================================== inclusions =================================================== */
#include "Os_Arch_Processor.h"
#include "Os_Alarm.h"
#include "Os_Appl.h"
#include "Os_Counter.h"
#include "Os_Sprot.h"
#include "Os_Rpc.h"
#include "Os_Hook.h"
#include "Os_Kernel.h"
#include "Os_Err.h"
#include "Os_Rti.h"
#include "Os_Arti.h"

/* ===================================================== macros ===================================================== */

/* ================================================ type definitions ================================================ */

/* ============================================ external data definitions =========================================== */

/* ============================================ internal data definitions =========================================== */

/* ========================================== internal function declarations ======================================== */
#if (CFG_ALARM_MAX > 0)
/**
 * @brief           Returns the number of ticks until alarm expiration
 * @param[in]       alarmId: ID of the alarm to query
 * @param[out]      tick: Pointer where to store the remaining ticks
 * @return          StatusType
 * @retval          E_OK: Alarm expiration time successfully returned
 * @retval          E_OS_NOFUNC: Alarm is not active
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_GetAlarm(AlarmType alarmId, TickRefType tick);

/**
 * @brief           Processes an alarm that has expired
 * @param[in]       counterId: ID of the counter that triggered the alarm
 * @param[in]       pScb: Pointer to the system control block
 * @param[inout]    alarmNextVal: Pointer to the ID of the next alarm in queue
 * @param[inout]    pCcb: Pointer to the counter control block
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_ProcessAlarm(Os_CounterType counterId, Os_AlarmType* alarmNextVal, Os_SCBType *pScb, Os_CCBType *pCcb, Os_ArchMsrType *msr);

/**
 * @brief           Inserts an alarm into the alarm queue
 * @param[in]       alarmId: ID of the alarm to be inserted
 * @param[in]       curTick: Current tick value of the counter
 * @param[inout]    alarmQueue: Pointer to the alarm queue
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_InsertAlarm(Os_AlarmType alarmId, Os_AlarmRefType alarmQueue, Os_TickType curTick);

/**
 * @brief           Removes an alarm from the alarm queue
 * @param[in]       alarmId: ID of the alarm to be deleted
 * @param[inout]    alarmQueue: Pointer to the alarm queue
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL void Os_DeleteAlarm(Os_AlarmType alarmId, Os_AlarmRefType alarmQueue);

/**
 * @brief           Validates parameters for absolute alarm setting
 * @param[in]       alarmId: ID of the alarm to check
 * @param[in]       start: Absolute start tick value
 * @param[in]       cycle: Cyclic period in ticks (0 means non-cyclic)
 * @return          StatusType
 * @retval          E_OK: Parameters are valid
 * @retval          E_OS_VALUE: Invalid start or cycle parameter
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_AbsAlarmCheckStatus(AlarmType alarmId, TickType start, TickType cycle);

/**
 * @brief           Retrieves alarm base characteristics
 * @param[in]       alarmId: ID of the alarm to query
 * @param[out]      Info: Pointer to store alarm base information
 * @return          void
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_GetAlarmBase(AlarmType alarmId, AlarmBaseRefType Info);

/**
 * @brief           Validates parameters for relative alarm setting
 * @param[in]       alarmId: ID of the alarm to check
 * @param[in]       increment: Relative ticks for first expiration
 * @param[in]       cycle: Cyclic period in ticks (0 means non-cyclic)
 * @return          StatusType
 * @retval          E_OK: Parameters are valid
 * @retval          E_OS_VALUE: Invalid increment or cycle parameter
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RelAlarmCheckStatus(AlarmType alarmId, TickType increment, TickType cycle);

/**
 * @brief           Sets an alarm to expire after relative time
 * @param[in]       alarmId: ID of the alarm to set
 * @param[in]       increment: Relative ticks for first expiration
 * @param[in]       cycle: Cyclic period in ticks (0 means non-cyclic)
 * @return          StatusType
 * @retval          E_OK: Alarm set successfully
 * @retval          E_OS_STATE: Alarm is already active
 * @retval          E_OS_VALUE: Invalid parameter values
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_SetRelAlarm(AlarmType alarmId, TickType increment, TickType cycle);

/**
 * @brief           The system service occupies the alarm <alarmId>
 * @param[in]       alarmId: ID of the alarm to set
 * @param[in]       start: start ticks for first expiration
 * @param[in]       cycle: Cyclic period in ticks (0 means non-cyclic)
 * @return          StatusType
 * @retval          E_OK: Alarm set successfully
 * @retval          E_OS_STATE: Alarm is already active
 * @retval          E_OS_VALUE: Invalid parameter values
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_SetAbsAlarm(AlarmType alarmId, TickType start, TickType cycle);

#if (OS_AUTOSAR_CORES > 1)
/**
 * @brief           RPC action handler for GetAlarm service
 * @param[inout]    inPara: Parameter array containing alarm ID and buffer for result
 * @return          StatusType
 * @retval          E_OK: RPC action completed successfully
 * @retval          E_OS_NOFUNC: Alarm is not active
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_GetAlarm(uint32 *inPara);

/**
 * @brief           Makes RPC call to GetAlarm on remote core
 * @param[in]       ownerCore: Core ID that owns the alarm
 * @param[in]       alarmId: ID of the alarm to query
 * @param[out]      tick: Pointer where to store the remaining ticks
 * @return          StatusType
 * @retval          E_OK: Remote call completed successfully
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_GetAlarm(Os_CoreIdType ownerCore, AlarmType alarmId, TickRefType tick);

/**
 * @brief           RPC action handler for CancelAlarm service
 * @param[in]       inPara: Parameter array containing alarm ID
 * @return          StatusType
 * @retval          E_OK: Alarm successfully canceled
 * @retval          E_OS_NOFUNC: Alarm is not active
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_CancelAlarm(uint32 * inPara);

/**
 * @brief           Makes RPC call to CancelAlarm on remote core
 * @param[in]       ownerCore: Core ID that owns the alarm
 * @param[in]       alarmId: ID of the alarm to cancel
 * @return          StatusType
 * @retval          E_OK: Remote call completed successfully
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_CancelAlarm(Os_CoreIdType ownerCore, AlarmType alarmId);

/**
 * @brief           RPC action handler for SetRelAlarm service
 * @param[in]       inPara: Parameter array containing alarm ID, increment, and cycle values
 * @return          StatusType
 * @retval          E_OK: Alarm set successfully
 * @retval          E_OS_STATE: Alarm is already active
 * @retval          E_OS_VALUE: Invalid parameter values
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_SetRelAlarm(uint32 *inPara);

/**
 * @brief           Makes RPC call to SetRelAlarm on remote core
 * @param[in]       ownerCore: Core ID that owns the alarm
 * @param[in]       alarmId: ID of the alarm to set
 * @param[in]       increment: Relative ticks for first expiration
 * @param[in]       cycle: Cyclic period in ticks (0 means non-cyclic)
 * @return          StatusType
 * @retval          E_OK: Remote call completed successfully
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_SetRelAlarm(Os_CoreIdType ownerCore, AlarmType alarmId, TickType increment, TickType cycle);

/**
 * @brief           RPC action handler for SetAbsAlarm service
 * @param[in]       inPara: Parameter array containing alarm ID, start, and cycle values
 * @return          StatusType
 * @retval          E_OK: Alarm set successfully
 * @retval          E_OS_STATE: Alarm is already active
 * @retval          E_OS_VALUE: Invalid parameter values
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcAction_SetAbsAlarm(uint32 *inPara);

/**
 * @brief           Makes RPC call to SetAbsAlarm on remote core
 * @param[in]       ownerCore: Core ID that owns the alarm
 * @param[in]       alarmId: ID of the alarm to set
 * @param[in]       start: Absolute tick value for first expiration
 * @param[in]       cycle: Cyclic period in ticks (0 means non-cyclic)
 * @return          StatusType
 * @retval          E_OK: Remote call completed successfully
 * @synchronous     TRUE
 * @reentrant       TRUE
 * @trace           -
 */
OS_LOCAL StatusType Os_RpcCall_SetAbsAlarm(Os_CoreIdType ownerCore, AlarmType alarmId, TickType start, TickType cycle);
#endif

/* ========================================== external function definitions ========================================= */
/**
 * Init the alarm control block
 */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/* PRQA S 1532 ++ */ /* VL_QAC_OneFunRef */
void Os_InitAlarm(void)
/* PRQA S 1532 -- */
{
    Os_ACBType *pACB;
    Os_CoreIdType coreId = Os_GetCoreIdLocal();

#if (CFG_AUTO_ALARM_MAX > 0U)
    /* PRQA S 3678 ++ */  /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetSystemContext(coreId);
    /* PRQA S 3678 -- */
    const Os_AlarmCfgType *pAlarmCfg = NULL_PTR;
    Os_CCBType *pCcb = NULL_PTR;
    Os_CounterType alarmCounter;
    Os_AppModeType alarmAppModeMask;
    Os_AlarmType alarmQueue;
#endif

    Os_AlarmType idStartRange = Os_AlarmIdRange[coreId].Start;
    Os_AlarmType idEndRange = Os_AlarmIdRange[coreId].End;

    if (idStartRange > idEndRange)
    {
        Os_Panic();
    }

    for (uint16 i = (uint16)idStartRange; i < (uint16)idEndRange; i++)
    {
        pACB = Os_ACB[i];
        pACB->AlarmStart = 0U;
        pACB->AlarmCycle = 0U;
        /* PRQA S 4342 ++ */ /* VL_Os_4342 */
        pACB->AlarmNext = (Os_AlarmType)i;
        /* PRQA S 4342 -- */

#if (CFG_AUTO_ALARM_MAX > 0U)
        pAlarmCfg = &Os_AlarmCfg[i];
        if (NULL_PTR != pAlarmCfg->AlarmAutostartRef)
        {
            alarmAppModeMask = pAlarmCfg->AlarmAutostartRef->AlarmAppMode;
            if (0U != (alarmAppModeMask & pScb->SysActiveAppMode))
            {
                alarmCounter = pAlarmCfg->AlarmCounter;
                pCcb = Os_CCB[alarmCounter];
                pACB->AlarmStart = pAlarmCfg->AlarmAutostartRef->AlarmStartTime;
                pACB->AlarmCycle = pAlarmCfg->AlarmAutostartRef->AlarmCycleTime;
                alarmQueue = pCcb->CounterAlmQue;
                /* PRQA S 1441, 4442 ++ */ /* VL_Os_1441, VL_Os_4442 */
                Os_InsertAlarm(i, &alarmQueue, pCcb->CounterCurVal);
                /* PRQA S 1441, 4442 -- */
                pCcb->CounterAlmQue = alarmQueue;
            }
        }
#endif
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service GetAlarm returns the relative value
 *                       in ticksbefore the alarm <AlarmID
 */
OS_LOCAL StatusType Os_GetAlarm(AlarmType alarmId, TickRefType tick)
{
    StatusType err = E_OK;

    OS_HAL_DECLARE_CRITICAL();

    OS_HAL_ENTRY_CRITICAL();
    if (Os_ACB[alarmId]->AlarmNext == alarmId)
    {
        err = E_OS_NOFUNC;
    }
    else
    {
        Os_CounterType osCounterId = Os_AlarmCfg[alarmId].AlarmCounter;

        /* Driven by hardware counter or sofrware counter. */
        *tick = Os_GetDistance(Os_CCB[osCounterId]->CounterCurVal,
                               Os_ACB[alarmId]->AlarmStart,
                               osCounterId);
    }
    OS_HAL_EXIT_CRITICAL();

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Process the alarm when it expires.
 */
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL void Os_ProcessAlarm(
    Os_CounterType counterId,
    Os_AlarmType* alarmNextVal,
    Os_SCBType *pScb,
    Os_CCBType *pCcb,
    Os_ArchMsrType *msr)
/* PRQA S 3673 -- */
{
    Os_AlarmType alarmCurVal = *alarmNextVal;
    *alarmNextVal = Os_ACB[*alarmNextVal]->AlarmNext;

#if (CFG_OSAPPLICATION_MAX > 0U)
    pScb->SysRunningAppId = Os_GetObjectAppID((ObjectTypeType)OS_OBJECT_ALARM, (AppObjectId)alarmCurVal);
#endif

    pCcb->CounterAlmQue = *alarmNextVal;
    Os_ACB[alarmCurVal]->AlarmNext = alarmCurVal;

    if (NULL_PTR != Os_AlarmCfg[alarmCurVal].AlarmCallback)
    {
#if (CFG_OSAPPLICATION_MAX > 0U)
        if (pScb->SysOsLevel == OS_LEVEL_TASK)
        {
            Os_TCB[pScb->SysRunningTaskId]->CallBackAppID = pScb->SysRunningAppId;
        }
        else if (pScb->SysOsLevel == OS_LEVEL_ISR2)
        {
            Os_ICB[pScb->SysRunningIsrCat2Id]->CallBackAppID = pScb->SysRunningAppId;
        }
        else
        {
            Os_Panic();
        }
#endif

        Os_Hal_RestoreInt(*msr);
        Os_AlarmCfg[alarmCurVal].AlarmCallback();
        *msr = Os_Hal_SuspendInt();

#if (CFG_OSAPPLICATION_MAX > 0U)
        if (pScb->SysOsLevel == OS_LEVEL_TASK)
        {
            Os_TCB[pScb->SysRunningTaskId]->CallBackAppID = INVALID_OSAPPLICATION;
        }
        else if (pScb->SysOsLevel == OS_LEVEL_ISR2)
        {
            Os_ICB[pScb->SysRunningIsrCat2Id]->CallBackAppID = INVALID_OSAPPLICATION;
        }
        else
        {
            Os_Panic();
        }
#endif
    }

    if (Os_ACB[alarmCurVal]->AlarmCycle > 0U)
    {
        Os_ACB[alarmCurVal]->AlarmStart = Os_CalcAbsTicks(
            Os_ACB[alarmCurVal]->AlarmStart,
            Os_ACB[alarmCurVal]->AlarmCycle,
            counterId);
        Os_AlarmType alarmQueue = pCcb->CounterAlmQue;
        Os_InsertAlarm(alarmCurVal, &alarmQueue, pCcb->CounterLastVal);
        pCcb->CounterAlmQue = alarmQueue;
    }
    UNUSED_PARAMETER(pScb);
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * This function processes the Alarm bound to the incoming counterId.
 */
/* PRQA S 1503, 1532 ++ */ /* VL_QAC_NoUsedApi, VL_QAC_OneFunRef */
void Os_WorkAlarm(Os_CounterType counterId)
/* PRQA S 1503, 1532 -- */
{
    boolean done = FALSE;
    Os_SCBType *pScb = OS_COUNTER_GET_SCB(counterId);
    OS_HAL_DECLARE_CRITICAL();

#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType bakAppId = pScb->SysRunningAppId;
#endif

    Os_TickType counterMaxAllowedValue = Os_CounterCfg[counterId].CounterMaxAllowedValue;

    OS_HAL_ENTRY_CRITICAL();
    Os_CCBType *pCcb = Os_CCB[counterId];
    Os_TickType counterCurval = pCcb->CounterCurVal;
    Os_AlarmType *alarmNextVal = &pCcb->CounterAlmQue;

    while (FALSE == done)
    {
        if (INVALID_ALARM == *alarmNextVal)
        {
            done = TRUE;
        }
        else if (Os_GetDistance(
                     Os_ACB[*alarmNextVal]->AlarmStart,
                     counterCurval, counterId) <= counterMaxAllowedValue)
        {
            Os_ProcessAlarm(counterId, alarmNextVal, pScb, pCcb, &msr);
        }
        else
        {
            done = TRUE;
        }
    }
    OS_HAL_EXIT_CRITICAL();

#if (CFG_OSAPPLICATION_MAX > 0U)
    pScb->SysRunningAppId = bakAppId;
#endif
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Insert an alarm in alarm queue
 */
/* PRQA S 6030, 6010 ++ */ /* VL_MTR_Os_STMIF, VL_MTR_Os_STCYC */
OS_LOCAL void Os_InsertAlarm( Os_AlarmType alarmId, Os_AlarmRefType alarmQueue, Os_TickType curTick)
/* PRQA S 6030, 6010 -- */
{
    if (Os_ObjectIDCheck((ObjectType)alarmId, (uint8)OS_OBJECT_ALARM) == TRUE)
    {
        Os_TickType alarmTick = Os_ACB[alarmId]->AlarmStart;
        Os_AlarmType pre = INVALID_ALARM;
        Os_AlarmType next = *alarmQueue;
        boolean done = FALSE;

        if (curTick < alarmTick)
        {
            while (done != TRUE)
            {
                if (INVALID_ALARM == next)
                {
                    done = TRUE;
                }
                else if ((Os_ACB[next]->AlarmStart >= curTick) && (Os_ACB[next]->AlarmStart <= alarmTick))
                {
                    pre = next;
                    next = Os_ACB[next]->AlarmNext;
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
                if (INVALID_ALARM == next)
                {
                    done = TRUE;
                }
                else if ((Os_ACB[next]->AlarmStart >= curTick) || (Os_ACB[next]->AlarmStart <= alarmTick))
                {
                    pre = next;
                    next = Os_ACB[next]->AlarmNext;
                }
                else
                {
                    done = TRUE;
                }
            }
        }

        Os_ACB[alarmId]->AlarmNext = next;

        if (pre != INVALID_ALARM)
        {
            Os_ACB[pre]->AlarmNext = alarmId;
        }
        else
        {
            *alarmQueue = alarmId;
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Delete an alarm in alarm queue
 */
OS_LOCAL void Os_DeleteAlarm(Os_AlarmType alarmId, Os_AlarmRefType alarmQueue)
{
    if (Os_ObjectIDCheck((ObjectType)alarmId, (uint8)OS_OBJECT_ALARM) != TRUE)
    {
        /*nothing to do*/
    }
    else
    {
        Os_AlarmType pre = INVALID_ALARM;
        Os_AlarmType next = *alarmQueue;

        while (next != INVALID_ALARM)
        {
            if (next == alarmId)
            {
                if (pre == INVALID_ALARM)
                {
                    *alarmQueue = Os_ACB[next]->AlarmNext;
                }
                else
                {
                    Os_ACB[pre]->AlarmNext = Os_ACB[next]->AlarmNext;
                }
                Os_ACB[next]->AlarmNext = next;
                break;
            }
            else
            {
                pre = next;
                next = Os_ACB[next]->AlarmNext;
            }
        }
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * Internal implementation of OS service: CancelAlarm
 */
/* PRQA S 1505 ++ */ /* VL_Os_1505 */
StatusType Os_CancelAlarm(AlarmType alarmId)
/* PRQA S 1505 -- */
{
    StatusType err = E_OK;

    OS_HAL_DECLARE_CRITICAL();

    /* Standard Status */
    OS_HAL_ENTRY_CRITICAL();
    if (Os_ACB[alarmId]->AlarmNext == alarmId)
    {
        err = E_OS_NOFUNC;
    }
    else
    {
        Os_CounterType alarmCounter = Os_AlarmCfg[alarmId].AlarmCounter;
        Os_CCBType *pCcb = Os_CCB[alarmCounter];
        Os_AlarmType alarmQueue = pCcb->CounterAlmQue;
        Os_DeleteAlarm(alarmId, &alarmQueue);
        pCcb->CounterAlmQue = alarmQueue;
    }
    OS_HAL_EXIT_CRITICAL();

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * State check when absolutely starting the Alarm.
 */
OS_LOCAL StatusType Os_AbsAlarmCheckStatus(
    AlarmType alarmId,
    TickType start,
    TickType cycle)
{
    StatusType err = E_OK;

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    Os_CounterType alarmCounter = Os_AlarmCfg[alarmId].AlarmCounter;
    const Os_CounterCfgType *pCounterCfg = &Os_CounterCfg[alarmCounter];
    Os_TickType counterMaxAllowedValue = pCounterCfg->CounterMaxAllowedValue;
    Os_TickType counterMinCycle = pCounterCfg->CounterMinCycle;
    if (start > counterMaxAllowedValue)
    {
        err = E_OS_VALUE; /*start is unsigned long integer, so not check smaller than 0*/
    }
    else if ((cycle != 0U) && ((cycle > counterMaxAllowedValue) || (cycle < counterMinCycle)))
    {
        err = E_OS_VALUE;
    }
    else
    {
        /*nothing to do*/
    }
#endif

    UNUSED_PARAMETER(alarmId);
    UNUSED_PARAMETER(start);
    UNUSED_PARAMETER(cycle);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service occupies the alarm <alarmId
 */
/* PRQA S 1503 ++ */ /* VL_QAC_NoUsedApi */
OS_LOCAL StatusType Os_SetAbsAlarm(AlarmType alarmId, TickType start, TickType cycle)
/* PRQA S 1503 -- */
{
    StatusType err = Os_AbsAlarmCheckStatus(alarmId, start, cycle);

    if ((StatusType)E_OK == err) /* PRQA S 2991, 2995 */ /* VL_Os_2991, VL_Os_2995 */
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        if (Os_ACB[alarmId]->AlarmNext != alarmId)
        {
            OS_HAL_EXIT_CRITICAL();
            err = E_OS_STATE;
        }
        else
        {
            Os_CounterType alarmCounter = Os_AlarmCfg[alarmId].AlarmCounter;
            Os_TickType counterMaxAllowedValue = Os_CounterCfg[alarmCounter].CounterMaxAllowedValue;
            Os_CCBType *pCcb = Os_CCB[alarmCounter];

            if (Os_GetDistance(pCcb->CounterCurVal, start, alarmCounter) <= counterMaxAllowedValue)
            {
                Os_ACB[alarmId]->AlarmStart = start;
            }
            else
            {
                Os_ACB[alarmId]->AlarmStart = start + counterMaxAllowedValue + 1U;
            }

            Os_ACB[alarmId]->AlarmCycle = cycle;
            Os_AlarmType alarmQueue = pCcb->CounterAlmQue;
            Os_InsertAlarm(alarmId, &alarmQueue, pCcb->CounterCurVal);
            pCcb->CounterAlmQue = alarmQueue;
            OS_HAL_EXIT_CRITICAL();
        }
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service GetAlarmBase reads the alarm base
 *                       characteristics. The return value <Info
 */
OS_LOCAL_INLINE OS_ALWAYS_INLINE void Os_GetAlarmBase(AlarmType alarmId, AlarmBaseRefType Info)
{
    Os_CounterType alarmCounter = Os_AlarmCfg[alarmId].AlarmCounter;
    const Os_CounterCfgType *pCounterCfg = &Os_CounterCfg[alarmCounter];

    Info->MaxAllowedValue = pCounterCfg->CounterMaxAllowedValue;
    Info->MinCycle = pCounterCfg->CounterMinCycle;
    Info->TicksPerBase = pCounterCfg->CounterTicksPerBase;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * State check when relatively starting the Alarm.
 */
OS_LOCAL StatusType Os_RelAlarmCheckStatus(
    AlarmType alarmId,
    TickType increment,
    TickType cycle)
{
    StatusType err = E_OK;
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
#if (OS_STATUS_EXTENDED == CFG_STATUS)
        Os_CounterType alarmCounter = Os_AlarmCfg[alarmId].AlarmCounter;
        const Os_CounterCfgType *pCounterCfg = &Os_CounterCfg[alarmCounter];
        Os_TickType counterMinCycle = pCounterCfg->CounterMinCycle;
        Os_TickType counterMaxAllowedValue = pCounterCfg->CounterMaxAllowedValue;

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
#endif
    }

    UNUSED_PARAMETER(alarmId);
    UNUSED_PARAMETER(cycle);
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service occupies the alarm <alarmId
 */
OS_LOCAL StatusType Os_SetRelAlarm(
    AlarmType alarmId,
    TickType increment,
    TickType cycle)
{
    StatusType err = Os_RelAlarmCheckStatus(alarmId, increment, cycle);

    if ((StatusType)E_OK == err)
    {
        OS_HAL_DECLARE_CRITICAL();
        OS_HAL_ENTRY_CRITICAL();
        if (Os_ACB[alarmId]->AlarmNext != alarmId)
        {
            OS_HAL_EXIT_CRITICAL();
            err = E_OS_STATE;
        }
        else
        {
            Os_CounterType alarmCounter = Os_AlarmCfg[alarmId].AlarmCounter;
            Os_CCBType *pCcb = Os_CCB[alarmCounter];
            /* Timer type. */
            Os_ACB[alarmId]->AlarmStart = Os_CalcAbsTicks(pCcb->CounterCurVal,
                                                          increment, alarmCounter);
            Os_ACB[alarmId]->AlarmCycle = cycle;
            Os_AlarmType alarmQueue = pCcb->CounterAlmQue;
            Os_InsertAlarm(alarmId, &alarmQueue, pCcb->CounterCurVal);
            pCcb->CounterAlmQue = alarmQueue;
            OS_HAL_EXIT_CRITICAL();
        }
    }

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service GetAlarm returns the relative value
 *                       in ticksbefore the alarm <alarmId
 */
#if (OS_AUTOSAR_CORES > 1)
OS_LOCAL StatusType Os_RpcAction_GetAlarm(uint32 *inPara)
{
    /* PRQA S 0311, 4342 ++ */ /* VL_Os_0311, VL_Os_4342 */
    return Os_GetAlarm((AlarmType)inPara[0], (Os_TickRefType)&inPara[1]);
    /* PRQA S 0311, 4342 -- */
}

OS_LOCAL StatusType Os_RpcCall_GetAlarm(
    Os_CoreIdType ownerCore,
    AlarmType alarmId,
    TickRefType tick)
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_SYNC,
        .RemoteCoreId = ownerCore,
        /* PRQA S 0674 ++ */ /* VL_Os_0674 */
        .ActionFn = Os_RpcAction_GetAlarm,
        /* PRQA S 0674 -- */
        .InPara[0] = (uint32)alarmId,
        /* PRQA S 0691, 1290 ++ */ /* VL_Os_0691, VL_Os_1290 */
        .InPara[1] = 0,
        /* PRQA S 0691, 1290 -- */
    };/* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    if (err == E_OK)
    {
        *tick = (TickType)rpcData.InPara[1];
    }
    return err;
}
#endif

/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick)
/* PRQA S 1503,3006,3408,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetAlarm);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetAlarm_Start, AlarmID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Tick)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_ObjectIDCheck((ObjectType)AlarmID, (uint8)OS_OBJECT_ALARM) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_GET_ALARM,
            .ObjectType = OS_OBJECT_ALARM,
            .ObjectID = (Os_AppObjectId)AlarmID,
            /* PRQA S 0306 ++ */ /* VL_Os_0306 */
            .Address = (uint32)Tick,
            /* PRQA S 0306 -- */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
/* SWS_Os_00640:GetAlarm shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType coreId = OS_ALARM_GET_COREID(AlarmID);
            if (coreId != pScb->SysCore)
            {
                err = Os_RpcCall_GetAlarm(coreId, AlarmID, Tick);
            }
            else
#endif
            {
                err = Os_GetAlarm(AlarmID, Tick);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_GetAlarm(AlarmID, Tick),
                          OSServiceId_GetAlarm,
                          err, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    UNUSED_PARAMETER(pScb);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 2812, 4544, 1258 ++ */ /* VL_Os_3432, VL_Os_2812, VL_Os_4544, VL_Os_1258 */
    OSRtiExitApi(pScb, OSApiId_GetAlarm);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetAlarm_Return, (err == E_OK ) ? *Tick : OS_TICK_INVALID);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/  
    /* PRQA S 3432, 2812, 4544, 1258 -- */
    /* PRQA S 3138, 3141 -- */

    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service GetAlarmBase reads the alarm base
 *                       characteristics. The return value <Info
 */
/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
/* PRQA S 1503,3006,3408,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_GetAlarmBase);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetAlarmBase_Start, AlarmID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (NULL_PTR == Info)
    {
        err = E_OS_ILLEGAL_ADDRESS;
    }
    else if (Os_ObjectIDCheck((ObjectType)AlarmID, (uint8)OS_OBJECT_ALARM) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_GET_ALARM_BASE,
            .ObjectType = OS_OBJECT_ALARM,
            .ObjectID = (Os_AppObjectId)AlarmID,
            /* PRQA S 0306 ++ */ /* VL_Os_0306 */
            .Address = (uint32)Info,
            /* PRQA S 0306 -- */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
            /* SWS_Os_00639:GetAlarmBase shall also work on an
            * ALARM that is bound to another core. */
            Os_GetAlarmBase(AlarmID, Info);
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK) /* PRQA S 2992, 2996 */ /* VL_Os_2992, VL_Os_2996 */
    {
        Os_TraceErrorHook(OSError_Save_GetAlarmBase(AlarmID, Info), /* PRQA S 2880 */ /* VL_Os_2880 */
                          OSServiceId_GetAlarmBase,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    UNUSED_PARAMETER(pScb);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_GetAlarmBase);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_GetAlarmBase_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service cancels the alarm <alarmId
 */
#if (OS_AUTOSAR_CORES > 1)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL StatusType Os_RpcAction_CancelAlarm(uint32 * inPara)
/* PRQA S 3673 -- */
{
    /* PRQA S 4342 ++ */ /* VL_Os_4342 */
    return Os_CancelAlarm((AlarmType)inPara[0]);
    /* PRQA S 4342 -- */
}

OS_LOCAL StatusType Os_RpcCall_CancelAlarm(
    Os_CoreIdType ownerCore,
    AlarmType alarmId)
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_SYNC,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_CancelAlarm,
        /* PRQA S 0691 ++ */ /* VL_Os_0691 */
        .InPara[0] = (uint32)alarmId,
        /* PRQA S 0691 -- */
    };/* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType CancelAlarm(AlarmType AlarmID)
/* PRQA S 1503,3006,3408,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_CancelAlarm);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CancelAlarm_Start, AlarmID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)AlarmID, (uint8)OS_OBJECT_ALARM) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_CANCEL_ALARM,
            .ObjectType = OS_OBJECT_ALARM,
            .ObjectID = (Os_AppObjectId)AlarmID,
            /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
            .Address = NULL_PARA,
            /* PRQA S 1258 -- */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
/* SWS_Os_00638:CancelAlarm shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType coreId = OS_ALARM_GET_COREID(AlarmID);
            if (coreId != pScb->SysCore)
            {
                err = Os_RpcCall_CancelAlarm(coreId, AlarmID);
            }
            else
#endif
            {
                err = Os_CancelAlarm(AlarmID);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_CancelAlarm(AlarmID),
                          OSServiceId_CancelAlarm,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    UNUSED_PARAMETER(pScb);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_CancelAlarm);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_CancelAlarm_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service occupies the alarm <AlarmID
 */
#if (OS_AUTOSAR_CORES > 1)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL StatusType Os_RpcAction_SetRelAlarm(uint32 *inPara)
/* PRQA S 3673 -- */
{
    /* PRQA S 3120, 4342 ++ */ /* VL_QAC_MagicNum, VL_Os_4342 */
    return Os_SetRelAlarm((AlarmType)inPara[0], (TickType)inPara[1], (TickType)inPara[2]);
    /* PRQA S 3120, 4342 -- */
}

OS_LOCAL StatusType Os_RpcCall_SetRelAlarm(
    Os_CoreIdType ownerCore,
    AlarmType alarmId,
    TickType increment,
    TickType cycle)
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_SYNC,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_SetRelAlarm,
        .InPara[0] = (uint32)alarmId,
        .InPara[1] = (uint32)increment,
        /* PRQA S 0691, 3120 ++ */ /* VL_Os_0691, VL_QAC_MagicNum */
        .InPara[2] = (uint32)cycle,
        /* PRQA S 0691, 3120 -- */
    };/* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
/* PRQA S 1503,3006,3408,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_SetRelAlarm);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetRelAlarm_Start, AlarmID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)AlarmID, (uint8)OS_OBJECT_ALARM) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_SET_REL_ALARM,
            .ObjectType = OS_OBJECT_ALARM,
            .ObjectID = (Os_AppObjectId)AlarmID,
            /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
            .Address = NULL_PARA,
            /* PRQA S 1258 -- */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
/* SWS_Os_00636:SetRelAlarm shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType coreId = OS_ALARM_GET_COREID(AlarmID);
            if (coreId != pScb->SysCore)
            {
                err = Os_RpcCall_SetRelAlarm(coreId, AlarmID, increment, cycle);
            }
            else
#endif
            {
                err = Os_SetRelAlarm(AlarmID, increment, cycle);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetRelAlarm(AlarmID, increment, cycle),
                          OSServiceId_SetRelAlarm,
                          err, pScb);/* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    UNUSED_PARAMETER(pScb);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_SetRelAlarm);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetRelAlarm_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * The system service occupies the alarm <AlarmID
 */
#if (OS_AUTOSAR_CORES > 1)
/* PRQA S 3673 ++ */ /* VL_QAC_3673 */
OS_LOCAL StatusType Os_RpcAction_SetAbsAlarm(uint32 *inPara)
/* PRQA S 3673 -- */
{
    /* PRQA S 3120, 4342 ++ */ /* VL_QAC_MagicNum, VL_Os_4342 */
    return Os_SetAbsAlarm((AlarmType)inPara[0], (TickType)inPara[1], (TickType)inPara[2]);
    /* PRQA S 3120, 4342 -- */
}

OS_LOCAL StatusType Os_RpcCall_SetAbsAlarm(
    Os_CoreIdType ownerCore,
    AlarmType alarmId,
    TickType start,
    TickType cycle)
{
    StatusType err = E_OK;
    Os_RpcInputType rpcData = {
        .RpcSync = OS_RPC_SYNC,
        .RemoteCoreId = ownerCore,
        .ActionFn = Os_RpcAction_SetAbsAlarm,
        .InPara[0] = (uint32)alarmId,
        .InPara[1] = (uint32)start,
        /* PRQA S 0691, 3120 ++ */ /* VL_Os_0691, VL_QAC_MagicNum */
        .InPara[2] = (uint32)cycle,
        /* PRQA S 0691, 3120 -- */
    };/* PRQA S 0704 */ /* VL_Os_0704 */

    err = Os_RpcCallService(&rpcData);
    return err;
}
#endif

/* PRQA S 1503,3006,3408,6070,1512 ++ */ /* VL_QAC_NoUsedApi, VL_Os_3006, VL_Os_3408, VL_MTR_Os_STCAL, VL_Os_1512 */
StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
/* PRQA S 1503,3006,3408,6070,1512 -- */
{
    /* PRQA S 2742, 2880, 3138, 2741, 3141 ++ */ /* VL_Os_PlatformDef */
    /* PRQA S 1006, 1021 ++ */ /* VL_Os_1006, VL_Os_1021 */
    OS_HAL_ENTER_KERNEL();
    /* PRQA S 1006, 1021 -- */
    /* PRQA S 2742, 2880, 3138, 2741, 3141 -- */
    StatusType err = E_OK;
    /* PRQA S 3678 ++ */ /* VL_Os_3678 */
    Os_SCBType *pScb = Os_GetCurrentContext();
    /* PRQA S 3678 -- */
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 1317, 3432, 4442, 4521, 4544 ++ */ /* VL_Os_1317, VL_Os_3432, VL_Os_4442, VL_Os_4521, VL_Os_4544 */
    OSRtiEnterApi(pScb, OSApiId_SetAbsAlarm);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetAbsAlarm_Start, AlarmID);
    /* PRQA S 1317, 3432, 4442, 4521, 4544 -- */
    /* PRQA S 3138, 3141 -- */

#if (OS_STATUS_EXTENDED == CFG_STATUS)
    if (Os_ObjectIDCheck((ObjectType)AlarmID, (uint8)OS_OBJECT_ALARM) != TRUE)
    {
        err = E_OS_ID;
    }
    else
#endif
    {
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
        Os_ServicePortParamType SprotParam = {
            .AllowedContext = OS_SERVICEPORT_CHECK_SET_ABS_ALARM,
            .ObjectType = OS_OBJECT_ALARM,
            .ObjectID = (Os_AppObjectId)AlarmID,
            /* PRQA S 1258 ++ */ /* VL_Os_ConstToIntegral */
            .Address = NULL_PARA,
            /* PRQA S 1258 -- */
        };
        err = Os_ServiceProtCheck(pScb, &SprotParam);
        if (E_OK == err)
#endif
        {
/* SWS_Os_00637:SetAbsAlarm shall also work on an
 * ALARM that is bound to another core. */
#if (OS_AUTOSAR_CORES > 1)
            Os_CoreIdType coreId = OS_ALARM_GET_COREID(AlarmID);
            if (coreId != pScb->SysCore)
            {
                err = Os_RpcCall_SetAbsAlarm(coreId, AlarmID, start, cycle);
            }
            else
#endif
            {
                err = Os_SetAbsAlarm(AlarmID, start, cycle);
            }
        }
    }

#if (CFG_ERRORHOOK == TRUE)
    if (err != E_OK)
    {
        Os_TraceErrorHook(OSError_Save_SetAbsAlarm(AlarmID, start, cycle),
                          OSServiceId_SetAbsAlarm,
                          err, pScb); /* PRQA S 3138, 3141 */ /* VL_Os_PlatformNoDef */
    }
#endif

    UNUSED_PARAMETER(pScb);
    /* PRQA S 3138, 3141 ++ */ /* VL_Os_PlatformNoDef */
    /* PRQA S 3432, 4544 ++ */ /* VL_Os_3432, VL_Os_4544 */
    OSRtiExitApi(pScb, OSApiId_SetAbsAlarm);
    ARTI_TRACE(NOSUSP, AR_CP_OS_SERVICECALLS, Os, pScb->SysCore, OsServiceCall_SetAbsAlarm_Return, err);
    OS_HAL_EXIT_KERNEL(); /* PRQA S 2743*/ /* VL_Os_2743*/
    /* PRQA S 3432, 4544 -- */
    /* PRQA S 3138, 3141 -- */
    return err;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

/*=======[E N D   O F   F I L E]==============================================*/
