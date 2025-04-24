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
**  FILENAME    :  Os.h                                                       **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef OS_H
#define OS_H
/*=======[I N C L U D E S]====================================================*/
#include "Os_Err.h"
#include "Os_CfgData.h"

/*=======[M A C R O S]========================================================*/
#define OS_VENDOR_ID 62
#define OS_MODULE_ID 1

/* ========================================= external function declarations ========================================= */
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
#if (CFG_TASK_MAX > 0U)
/**
 * @brief           The task<TaskID> is transferred from the  suspended state into the ready state.
 * @param[in]       TaskID: Task reference.
 * @return          StatusType
 * @retval          E_OK: No Error.
 * @retval          E_OS_LIMIT: The number of activations has exceeded the limit.
 * @retval          E_OS_ID: Invalid TaskID.
 * @retval          E_OS_CALLEVEL: Wrong calling environment.
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend.
 * @retval          E_OS_ACCESS: Application state error/ No access to this object.
 * @retval          E_OS_CORE: The remote core is not running.
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_BUSY: The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT: waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58352
 */
extern StatusType ActivateTask(TaskType TaskID);

/**
 * @brief           Asynchronous version of the ActivateTask() function.
 * @param[in]       TaskID: Task reference
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_LIMIT: The number of activations has exceeded the limit
 * @retval          E_OS_ID: Invalid TaskID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Application state error/ No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_BUSY: The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT: waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58362
 */
extern StatusType ActivateTaskAsyn(TaskType TaskID);

/**
 * @brief           This service causes the termination of the calling task. The calling task is
 *                  transferred from the running state into the suspended state
 * @return          StatusType
 * @retval          this function does not return in correct case
 * @retval          E_OS_RESOURCE: Resources are still occupied by tasks
 * @retval          E_OS_SPINLOCK: Spinlock are still occupied by tasks
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58354
 */
extern StatusType TerminateTask(void);

/**
 * @brief           This service causes the  termination of the calling task. After
 *                   termination of the calling task a succeeding task <TaskID> is activated
 * @param[in]       TaskID: Task reference
 * @return          StatusType
 * @retval          this function does not return in correct case
 * @retval          E_OS_LIMIT: The number of activations has exceeded the limit
 * @retval          E_OS_ID: Invalid TaskID
 * @retval          E_OS_RESOURCE: Resources are still occupied by tasks
 * @retval          E_OS_SPINLOCK: Spinlock are still occupied by tasks
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Application state error/ No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_BUSY: The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT: waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58355
 */
extern StatusType ChainTask(TaskType TaskID);

/**
 * @brief           If a higher-priority task is ready , the internal resource
 *                  of the task is released, the current task is put into the
 *                  ready  state, its context is saved and the highe r-priority
 *                  task is executed. Otherwise the calling task is continued.
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_RESOURCE: Resources are still occupied by tasks
 * @retval          E_OS_SPINLOCK: Spinlock are still occupied by tasks
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58356
 */
extern StatusType Schedule(void);

/**
 * @brief           GetTaskID  returns the information about the TaskID of
 *                  the task which is currently  running
 * @param[in]       TaskID: Task reference
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ILLEGAL_ADDRESS : Parameter address access illegal, or rpcData is NULL_PTR.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58357
 */
extern StatusType GetTaskID(TaskRefType TaskID);

/**
 * @brief           Returns the state of a task (running, ready, waiting,
 *                  suspended)at the time of calling GetTaskState.
 * @param[in]       TaskID: Task reference
 * @param[out]      State: Status of the task
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid TaskID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Application state error/ No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_BUSY: The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT: waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58358
 */
extern StatusType GetTaskState(TaskType TaskID, TaskStateRefType State);
#endif /* CFG_TASK_MAX > 0U */

#if (CFG_STD_RESOURCE_MAX > 0U)
/**
 * @brief           This call serves to enter critical sections in the code that are assigned to the
 *                  resource referenced by <ResID>. A critical section shall always be left
 *                  using ReleaseResource.
 * @param[in]       ResID: Resource Identifier
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid ResID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Attempt to obtain a resource that has been occupied by a task,
 *                  interrupt, or statically assigned to a ceiling priority task or an interrupt
 *                  higher than the ceiling priority
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58344
 */
extern StatusType GetResource(ResourceType ResID);

/**
 * @brief           ReleaseResource  is the counterpart of  GetResource  and serves to leave
 *                  critical sections in the code that are assigned to the resource referenced by <ResID>.
 * @param[in]       ResID: Resource Identifier
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid ResID
 * @retval          E_OS_NOFUNC: Attempt to release unused resources / Task/ISR don’t release resource
 *                  and spinlock in nest order
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: An attempt was made to release a resource with a lower ceiling
 *                  priority than a task that calls a task or interrupts a static configuration.
 * @retval          E_OS_CORE: The object is not belong to local core
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58345
 */
extern StatusType ReleaseResource(ResourceType ResID);
#endif /* CFG_STD_RESOURCE_MAX > 0U */

#if (CFG_EXTENDED_TASK_MAX > 0U)
/**
 * @brief           The events of task <TaskID> are set according to the event mask <Mask>.
 *                  Calling SetEvent causes the task <TaskID> to be transferred to the ready state,
 *                  if it was waiting for at least one of the events specified in <Mask>.
 * @param[in]       TaskID: Task reference
 * @param[in]       Mask: Event mask, the configured event name
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid TaskID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Task is not an extended task / No access to this object
 * @retval          E_OS_STATE: event cannot be set when the task is suspended
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_BUSY: The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT: waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58312
 */
extern StatusType SetEvent(TaskType TaskID, EventMaskType Mask);

/**
 * @brief           Asynchronous version of the SetEvent() function.
 * @param[in]       TaskID: Task reference
 * @param[in]       Mask: Event mask, the configured event name
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid TaskID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Task is not an extended task / No access to this object
 * @retval          E_OS_STATE: event cannot be set when the task is suspended
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_BUSY: The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT: waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58325
 */
extern StatusType SetEventAsyn(TaskType TaskID, EventMaskType Mask);

/**
 * @brief           The events of the extended task calling ClearEvent are
 *                  cleared according to the event mask <Mask>.
 * @param[in]       Mask: Event mask, the configured event name
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid TaskID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Task is not an extended task
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58316
 */
extern StatusType ClearEvent(EventMaskType Mask);

/**
 * @brief           This service returns the current state of all event bits
 *                  of the task <TaskID>, not the events that the task is waiting for.
 * @param[in]       TaskID: Task reference
 * @param[in]       Mask: Event mask, the configured event name
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid TaskID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Task is not an extended task
 * @retval          E_OS_STATE: task is suspended
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58315
 */
extern StatusType GetEvent(TaskType TaskID, EventMaskRefType Event);

/**
 * @brief           The state of the calling task is set to waiting, unless
 *                  at least one of the events specified in <Mask> has already been set.
 * @param[in]       Mask: Event mask, the configured event name
 * @return          StatusType
 * @retval          this function does not return in correct case
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Task is not an extended task
 * @retval          E_OS_RESOURCE: Resources are still occupied by tasks
 * @retval          E_OS_SPINLOCK: Spinlock are still occupied by tasks
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58314
 */
extern StatusType WaitEvent(EventMaskType Mask);

/**
 * @brief           The state of the calling task is set to waiting, must
 *                  all the events specified in <Mask> has already been set.
 * @param[in]       Mask: Event mask, the configured event name
 * @return          StatusType
 * @retval          this function does not return in correct case
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Task is not an extended task
 * @retval          E_OS_RESOURCE: Resources are still occupied by tasks
 * @retval          E_OS_SPINLOCK: Spinlock are still occupied by tasks
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58321
 */
extern StatusType WaitAllEvents(EventMaskType Mask);
#endif /* CFG_EXTENDED_TASK_MAX > 0U */

#if (CFG_ALARM_MAX > 0U)
/**
 * @brief           The system service GetAlarm returns the relative value in
 *                  ticks before the alarm <AlarmID> expires.
 * @param[in]       AlarmID: Alarm Identifier
 * @param[in]       Tick: Relative number of ticks before the alarm is triggered
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_NOFUNC: alarm is not used
 * @retval          E_OS_ID: invalid AlarmID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58464
 */
extern StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick);

/**
 * @brief           The system service  GetAlarmBase  reads the alarm base characteristics.
 *                  The return value  <Info> is a structure in which the information of data
 *                  type AlarmBaseType is stored.
 * @param[in]       AlarmID: Alarm Identifier
 * @param[out]      Info: Information pointing to AlarmID alarms
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid AlarmID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58465
 */
extern StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);

/**
 * @brief           The system service cancels the alarm <AlarmID>.
 * @param[in]       AlarmID: Alarm Identifier
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_NOFUNC: Alarm is not used
 * @retval          E_OS_ID: invalid AlarmID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: rpcData is NULL_PTR.
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58466
 */
extern StatusType CancelAlarm(AlarmType AlarmID);

/**
 * @brief           The system service occupies the alarm <AlarmID> element. After <increment>
 *                  ticks have elapsed, the task assigned to the alarm <AlarmID> is activated or
 *                  the assigned event (only for extended tasks) is set or the alarm-callback
 *                  routine is called.
 * @param[in]       AlarmID: Alarm Identifier
 * @param[in]       increment: the number of clocks triggered for the first time relative to the
 *                  current number of clocks
 * @param[in]       cycle: The period of the alarm (Tick number), the cycle value of a single alarm is 0
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_STATE: Alarm corresponding to AlarmID is in use
 * @retval          E_OS_ID: Invalid AlarmID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_VALUE: Increment value is not within the normal range (less than 0 or greater than
 * maxallowedvalue) or Cycle value is not equal to 0 and is not within the allowed count range (less than mincycle or
 * greater than maxallowedvalue)
 * @retval          E_OS_ILLEGAL_ADDRESS: rpcData is NULL_PTR.
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58467
 */
extern StatusType SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);

/**
 * @brief           The system service occupies the alarm <AlarmID> element. When <start> ticks are
 *                  reached, the task assigned to the alarm <AlarmID> is activated or the assigned
 *                  event (only for extended tasks) is set or the alarm-callback routine is called.
 * @param[in]       AlarmID: Alarm Identifier
 * @param[in]       start: Number of absolute clocks triggered for the first time
 * @param[in]       cycle: The period of the alarm (Tick number), the cycle value of a single alarm is 0
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_STATE: Alarm is active
 * @retval          E_OS_ID: invalid AlarmID
 * @retval          E_OS_VALUE: Increment value is not within the normal range (less than 0 or greater than
 * maxallowedvalue) or Cycle value is not equal to 0 and is not within the allowed count range (less than mincycle or
 * greater than maxallowedvalue)
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: rpcData is NULL_PTR.
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58468
 */
extern StatusType SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);
#endif /* CFG_ALARM_MAX > 0U */

#if (CFG_COUNTER_MAX > 0U)
/**
 * @brief           This service increments a software counter.
 * @param[in]       CounterID: Counter Identifier
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: CounterID is invalid or the counter implemented by hardware cannot be incremented by
 * software
 * @retval          E_OS_CORE: The object is not belong to local core
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58367
 */
extern StatusType IncrementCounter(CounterType CounterID);

/**
 * @brief           This service reads the current count value of a counter .
 * @param[in]       CounterID: Counter Identifier
 * @param[out]      Value: Counter's current clock value
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: CounterID is invalid
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_VALUE: Parameter address is invalid
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58368
 */
extern StatusType GetCounterValue(CounterType CounterID, TickRefType Value);

/**
 * @brief           This service gets the number of ticks between the current tick value and a
 *                  previously read tick value.
 * @param[in]       CounterID: Counter Identifier
 * @param[out]      Value: The current value of the counter (used as the next starting value)
 * @param[out]      ElapsedValue: Interval between current clock value and start value
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: CounterID is invalid
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_VALUE: Parameter address is invalid
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58369
 */
extern StatusType GetElapsedValue(CounterType CounterID, TickRefType Value, TickRefType ElapsedValue);
#endif /* CFG_COUNTER_MAX > 0U */

/**
 * @brief           This service returns t he current application mode. It
 *                  may be used to write mode dependent code.
 * @return          AppModeType: Application mode
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern AppModeType GetActiveApplicationMode(void);

/**
 * @brief           This service disables  all interrupts for which the hardware supports disabling.
 *                  The state before is saved for the EnableAllInterrupts call.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58441
 */
extern void DisableAllInterrupts(void);

/**
 * @brief           This service restores the state saved by  DisableAllInterrupts.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58442
 */
extern void EnableAllInterrupts(void);

/**
 * @brief           This service restores t he recognition status of all
 *                  interrupts saved by the SuspendAllInterrupts service.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58443
 */
extern void ResumeAllInterrupts(void);

/**
 * @brief           This service saves the re cognition status of all interrupts
 *                  and disables all interrupts for which the hardware supports.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58443
 */
extern void SuspendAllInterrupts(void);

/**
 * @brief           This service restores the recognition status of interrupts saved by
 *                  the SuspendOSInterrupts service.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58445
 */
extern void ResumeOSInterrupts(void);

/**
 * @brief           This service saves the recognition status of interrupts of category 2 and
 *                  disables the recognition of these interrupts.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58446
 */
extern void SuspendOSInterrupts(void);

/**
 * @brief           Enables the interrupt source by modifying the interrupt controller registers.
 *                  Additionally it may clear the interrupt pending flag
 * @param[in]       ISRID: The ID of a category 2 ISR.
 * @param[in]       ClearPending: Whether to clear the interrupt pending flag.
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_NOFUNC: EnableInterruptSource is called for an interrupt source which is already enabled.
 * @retval          E_OS_ID : ISRID is not a valid category 2 ISR identifier.
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58459
 */
extern StatusType EnableInterruptSource(ISRType ISRID, boolean ClearPending);

/**
 * @brief           Disables the interrupt source by modifying the interrupt controller registers.
 * @param[in]       ISRID: The ID of a category 2 ISR.
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_NOFUNC: DisableInterruptSource is called for an interrupt source which is already disabled.
 * @retval          E_OS_ID : ISRID is not a valid category 2 ISR identifier.
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58460
 */
extern StatusType DisableInterruptSource(ISRType ISRID);

/**
 * @brief           Clears the interrupt pending flag by modifying the interrupt controller registers.
 * @param[in]       ISRID: The ID of a category 2 ISR.
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID : ISRID is not a valid category 2 ISR identifier.
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function
 * @retval          E_OS_ACCESS: The calling application is not the owner of the ISR passed in ISRID
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58461
 */
extern StatusType ClearPendingInterrupt(ISRType ISRID);

/**
 * @brief           The user can call this system service to start the operating system in a specific mode.
 * @param[in]       Mode: Application mode (launch mode), defined by the user on the tool
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern void StartOS(AppModeType Mode);

/**
 * @brief           The user can call this system service to abort the overall system (e.g. emergency off).
 * @param[in]       Error: Error type, see StatusType for detail
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern void ShutdownOS(StatusType Error);

#if (CFG_SCHEDTBL_MAX > 0U)
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief           This service starts an explicitly synchronized schedule table synchronously.
 * @param[in]       ScheduleTableID: ScheduleTable Identifier
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: ScheduleTableID is invalid
 * @retval          E_OS_STATE: ScheduleTableID has been started
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The object is not belong to local core
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58490
 */
extern StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID);

/**
 * @brief           This service provides the schedule table with a synchronization count and
 *                  start synchronization.
 * @param[in]       ScheduleTableID: ScheduleTable Identifier
 * @param[in]       Value: Sync counter current value
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: ScheduleTableID is invalid or the schedule cannot be synchronized
 *                  (OsScheduleTblSyncStrategy is not set or OsScheduleTblSynStrategy = IMPLICIT)
 * @retval          E_OS_VALUE: Value is out of range
 * @retval          E_OS_STATE: The status of ScheduleTableID is SCHEDULETABLE_STOPPED
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The object is not belong to local core
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58491
 */
extern StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID, TickType value);

/**
 * @brief           Stop schedule table explicit sync and set state to running.
 * @param[in]       ScheduleTableID: ScheduleTable Identifier
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID:  Invalid ScheduleTableID
 * @retval          E_OS_STATE: The schedule table is not running
 * @retval          E_OS_CORE: The object is not belong to local core
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58492
 */
extern StatusType SetScheduleTableAsync(ScheduleTableType ScheduleTableID);
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */

/**
 * @brief           This service cancels the processing of a schedule table immediately at any point
 *                  while the schedule table is running.
 * @param[in]       ScheduleTableID: ScheduleTable Identifier
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: ScheduleTableID is invalid
 * @retval          E_OS_NOFUNC: The schedule has stopped
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: rpcData is NULL_PTR.
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58488
 */
extern StatusType StopScheduleTable(ScheduleTableType ScheduleTableID);

/**
 * @brief           This service starts the processing of a schedule table at an absolute value
 *                  "Start" on the underlying counter.
 * @param[in]       ScheduleTableID: ScheduleTable Identifier
 * @param[in]       Start: The clock value at which the schedule starts processing (the absolute
 *                  tick value of the counter corresponding to the schedule)
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: Invalid ScheduleTableID
 * @retval          E_OS_VALUE: Start is greater than OsCounterMaxAllowedValue
 * @retval          E_OS_STATE: schedule has started
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: rpcData is NULL_PTR.
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58487
 */
extern StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID, TickType Start);

/**
 * @brief           This service starts the processing of a schedule table at "Offset" relative to
 *                  the "Now" value on the underlying counter.
 * @param[in]       ScheduleTableID: ScheduleTable Identifier
 * @param[in]       Offset: The clock value from the current time to the start of the schedule (the
 *                  tick value of the counter corresponds to the schedule)
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: ScheduleTableID is invalid
 * @retval          E_OS_VALUE: Offset is greater than (OsCounterMaxAllowedValue-InitialOffset) or equal to 0
 * @retval          E_OS_STATE: schedule has started
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: Application state error / No access to this object
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_ILLEGAL_ADDRESS: rpcData is NULL_PTR.
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58486
 */
extern StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset);

/**
 * @brief           This service switches the processing from one schedule table to another schedule table.
 * @param[in]       ScheduleTableID_From: Schedules currently being processed
 * @param[in]       ScheduleTableID_To: Next processing schedule with a series of trigger points
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: ScheduleTableID_From or ScheduleTableID_To is invalid
 * @retval          E_OS_NOFUNC: ScheduleTableID_From did not start
 * @retval          E_OS_STATE: ScheduleTableID_To has started or is in Next
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_CORE: These two schedule tables do not belong to the same core
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58489
 */
extern StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From, ScheduleTableType ScheduleTableID_To);

/**
 * @brief           Get the status of the specified schedule
 * @param[in]       ScheduleTableID: ScheduleTable Identifier
 * @param[out]      ScheduleStatus: Pointer to the status value of the obtained schedule
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID:  Invalid ScheduleTableID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal, or rpcData is NULL_PTR.
 * @retval          E_OS_CORE: The remote core is not running
 * @retval          E_OS_VALUE: Parameter address is invalid
 * @retval          E_BUSY : The free node can’t be gotten from free queue.
 * @retval          E_OS_TIMEOUT : Waiting the execution result timeout.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58485
 */
extern StatusType GetScheduleTableStatus(ScheduleTableType ScheduleTableID, ScheduleTableStatusRefType ScheduleStatus);
#endif /* CFG_SCHEDTBL_MAX > 0U */

/**
 * @brief           This service returns the identifier of the currently executing ISR.
 * @return          ISRType
 * @retval          Returns the ID of CATEGORY_2 interrupt
 * @retval          INVALID_ISR
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58447
 */
extern ISRType GetISRID(void);

#if (TRUE == CFG_ERRORHOOK)
/**
 * @brief           The specific error hook is called whenever a Task or ISR calls system service to cause an error.
 * @param[in]       Error: Error code
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern void ErrorHook(StatusType Error);
#endif

#if (TRUE == CFG_PRETASKHOOK)
/**
 * @brief           This hook routine is called by ORIENTAIS OS before executing a new task, but
 *                  after the transition of the task to the running state (to allow evaluation of
 *                  the TaskID by GetTaskID
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern void PreTaskHook(void);
#endif

#if (TRUE == CFG_POSTTASKHOOK)
/**
 * @brief           This hook routine is called by ORIENTAIS OS after executing the current task,
 *                  but before leaving the task's running state (to allow evaluation of the TaskID
 *                  by GetTaskID).
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern void PostTaskHook(void);
#endif

#if (TRUE == CFG_STARTUPHOOK)
/**
 * @brief           This hook routine is called by the operating system at the end of the operating
 *                  system in itialisation and before the scheduler is running. At this time the
 *                  applicationcan initialise device drivers etc.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern void StartupHook(void);
#endif

#if (TRUE == CFG_SHUTDOWNHOOK)
/**
 * @brief           This hook routine is called by the operating system when the OS service ShutdownOS
 *                  has been called. This routine is called during the operating system shutdown.
 * @param[in]       Error: Error code
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern void ShutdownHook(StatusType Error);
#endif

#if ((OS_SC2 == CFG_SC) || (OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
#if (TRUE == CFG_PROTECTIONHOOK)
/**
 * @brief           his hook routine is called by the operating system when the OS service protection functions.
 * @synchronous     TRUE
 * @param[in]       Fatalerror: Error code
 * @reentrant       Non Reentrant
 * @trace           CPD-57666
 */
extern ProtectionReturnType ProtectionHook(StatusType Fatalerror);
#endif
#endif

#if (CFG_OSAPPLICATION_MAX > 0)
/**
 * @brief           Get the OS-Application ID of the object calling the API
 * @return          ApplicationType
 * @retval          Application number
 * @retval          INVALID_OSAPPLICATION
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58397
 */
extern ApplicationType GetApplicationID(void);

/**
 * @brief           Get the OS-Application ID to which the currently running Task/ISR/Hook belongs.
 * @return          ApplicationType
 * @retval          Application number
 * @retval          INVALID_OSAPPLICATION
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58398
 */
extern ApplicationType GetCurrentApplicationID(void);

/**
 * @brief           This service sets the own state of an OS-Application
 *                  from APPLICATION_RESTARTING to APPLICATION_ACCESSIBLE.
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_STATE: The OS-Application of the caller is in the wrong state
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58399
 */
extern StatusType AllowAccess(void);

/**
 * @brief           This service returns the current state of an OS-Application.
 * @param[in]       Application: Application number user want to get
 * @param[out]      Value: ACCESSIBLE:accessible RESTARTING:Restarting TERMINATED:ended
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: <Application> is not valid.
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @retval          E_OS_ILLEGAL_ADDRESS: The address of Value is invalid.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58394
 */
extern StatusType GetApplicationState(ApplicationType Application, ApplicationStateRefType Value);

/**
 * @brief           This service determines if the OS-Applications, given by ApplID, is allowed to
 *                  use the IDs of a Task, ISR, Resource,Counter, Alarm or Schedule Table in API calls.
 * @param[in]       ApplID: OsApplication number
 * @param[in]       ObjectType: Entity type, see ObjectTypeType
 * @param[in]       ObjectID: Entity number
 * @return          ObjectAccessType
 * @retval          ACCESS : the ApplID has access to the object.
 * @retval          NO_ACCESS : the ApplID is invalid; ObjectType is invalid;the ApplID does not
 *                  have access to the object.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58400
 */
extern ObjectAccessType CheckObjectAccess(ApplicationType ApplID, ObjectTypeType ObjectType, AppObjectId ObjectID);

/**
 * @brief           This service determines to which OS-Application a given Task, ISR, Resource,
 *                  Counter, Alarm or Schedule Table belongs
 * @param[in]       ObjectType: Entity type, see ObjectTypeType
 * @param[in]       ObjectID: Entity number
 * @return          ApplicationType
 * @retval          ApplicationType : the OS-Application to which the object ObjectType belongs.
 * @retval          INVALID_OSAPPLICATION : the ObjectID does not exist; ObjectType is invalid; the
 *                  object type is BJECT_RESOURCE and the object is RES_SCHEDULER.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58401
 */
extern ApplicationType CheckObjectOwnership(ObjectTypeType ObjectType, AppObjectId ObjectID);

/**
 * @brief           This service returns the current state of an OS-Application.
 * @param[in]       Application: Application user want to end
 * @param[in]       RestartOption RESTART: Perform a restart NO_RESTART: Do not restart the app
 * @return          StatusType
 * @retval          E_OK: No errors
 * @retval          E_OS_ID: <Application> was not valid (only in EXTENDED status)
 * @retval          E_OS_VALUE: <RestartOption> was neither RESTART nor NO_RESTART (only in EXTENDED status)
 * @retval          E_OS_ACCESS: The caller does not have the right to terminate<Application> (only in EXTENDED status)
 * @retval          E_OS_STATE: The state of <Application> does not allow terminating <Application>
 * @retval          E_OS_CALLEVEL: Call level at wrong context.
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service.
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58402
 */
extern StatusType TerminateApplication(ApplicationType Application, RestartType RestartOption);
#endif /* CFG_OSAPPLICATION_MAX > 0 */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
/**
 * @brief           This service checks if a memory region is write/read/execute accessible and also
 *                  returns information if the memory region is part of the stack space.
 * @param[in]       ISRID: Interrupt Identifier
 * @param[in]       Address: Memory area start address
 * @param[in]       Size: Memory area size
 * @return          AccessType
 * @retval          NO_PERMISSION: Permission denied
 * @retval          OSMEMORY_IS_READABLE(Access) Non-zero means read permission
 * @retval          OSMEMORY_IS_WRITEABLE(Access) Non-zero means write permission
 * @retval          OSMEMORY_IS_EXECUTABLE(Access) Non-zero means execute permission
 * @retval          OSMEMORY_IS_STACKSPACE(Access) Non-zero means stack space
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58426
 */
extern AccessType CheckISRMemoryAccess(ISRType ISRID, MemoryStartAddressType Address, MemorySizeType Size);

/**
 * @brief           This service checks if a memory region is write/read/execute accessible and also
 *                  returns information if the memory region is part of the stack space.
 * @param[in]       TaskID: Task Identifier
 * @param[in]       Address: Memory area start address
 * @param[in]       Size: Memory area size
 * @return          AccessType
 * @retval          NO_PERMISSION: Permission denied
 * @retval          OSMEMORY_IS_READABLE(Access) Non-zero means read permission
 * @retval          OSMEMORY_IS_WRITEABLE(Access) Non-zero means write permission
 * @retval          OSMEMORY_IS_EXECUTABLE(Access) Non-zero means execute permission
 * @retval          OSMEMORY_IS_STACKSPACE(Access) Non-zero means stack space
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58427
 */
extern AccessType CheckTaskMemoryAccess(TaskType TaskID, MemoryStartAddressType Address, MemorySizeType Size);
#endif /* TRUE == CFG_MEMORY_PROTECTION_ENABLE */

#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
/**
 * @brief           The interface provided by trusted functions to external calls. Untrusted apps
 *                  can access internal resources of trusted apps
 * @param[in]       FunctionIndex: The index number of the trusted function
 * @param[in]       FunctionParams: Pointer to a trusted function parameter
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal
 * @retval          E_OS_SERVICEID: Incoming trusted function index error
 * @retval          E_OS_CORE: The object is not belong to local core
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType CallTrustedFunction(
    TrustedFunctionIndexType FunctionIndex,
    TrustedFunctionParameterRefType FunctionParams);
#endif

#if (CFG_SPINLOCK_MAX > 0U)
/**
 * @brief           GetSpinlock tries to occupy a spin-lock variable. If the function returns,
 *                  either the lock is successfully taken or an error has occurred.
 * @param[in]       SpinlockId: The spin lock number user want to get
 * @return          StatusType
 * @retval          E_OK:  No Error
 * @retval          E_OS_ID: Invalid ID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_INTERFERENCE_DEADLOCK: Conflict deadlock, lock is already occupied by task
 * @retval          E_OS_NESTING_DEADLOCK: Nested deadlock, another task on the same core is
 *                  occupying another spinlock
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58377
 */
extern StatusType GetSpinlock(SpinlockIdType SpinlockId);

/**
 * @brief           ReleaseSpinlock is the counterpart of GetSpinlock. ReleaseSpinlock releases
 *                  a spinlock variable that was occupied before.
 * @param[in]       SpinlockId: The spin lock number user want to release
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid ID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_STATE: Spinlock is not occupied by a task
 * @retval          E_OS_NOFUNC: The spin locks are released in the wrong order
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58380
 */
extern StatusType ReleaseSpinlock(SpinlockIdType SpinlockId);

/**
 * @brief           Try to acquire a spinlock.
 * @param[in]       SpinlockId: The spin lock number user want to get
 * @param[out]      Success: Returns whether the specified spin lock is occupied
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid ID
 * @retval          E_OS_CALLEVEL: Wrong calling environment
 * @retval          E_OS_DISABLEDINT: Unable to call system services because of interrupt disable/suspend
 * @retval          E_OS_ILLEGAL_ADDRESS: Parameter address access illegal
 * @retval          E_OS_ACCESS: No access to this object
 * @retval          E_OS_INTERFERENCE_DEADLOCK: Conflict deadlock, lock is already occupied by task
 * @retval          E_OS_NESTING_DEADLOCK: Nested deadlock, another task on the same core is
 *                  occupying another spinlock
 * @retval          E_OS_ACCESS: No access to this spin lock
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58383
 */
extern StatusType TryToGetSpinlock(SpinlockIdType SpinlockId, TryToGetSpinlockType* Success);
#endif /* CFG_SPINLOCK_MAX > 0U */

/**
 * @brief           Start the specified Autosar core
 * @param[in]       CoreID:core number
 * @param[out]      Status: StatusType, The return value of this service
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58501
 */
extern void StartCore(CoreIdType CoreID, StatusType* Status);

/**
 * @brief           Start the specified non-Autosar core
 * @param[in]       CoreID:core number
 * @param[out]      Status: StatusType, The return value of this service
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58502
 */
extern void StartNonAutosarCore(CoreIdType CoreID, StatusType* Status);

/**
 * @brief           This API allows the caller to select the idle mode
 *                  action which is performed during idle time of the OS
 * @param[in]       CoreID: Core Identifier
 * @param[in]       IdleMode: Selected Idle Mode
 * @return          StatusType
 * @retval          E_OK: No Error
 * @retval          E_OS_ID: Invalid core and/or invalid idleMode
 * @retval          E_OS_CALLEVEL: Call at interrupt level
 * @retval          E_OS_DISABLEDINT: Interrupts are disabled/suspended, OS shall ignore the service
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58503
 */
extern StatusType ControlIdle(CoreIdType CoreID, IdleModeType IdleMode);

/**
 * @brief           Get the number of cores currently activated (cores managed by ORIENTAIS OS)
 * @return          uint32
 * @retval          The number of cores currently activated
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58505
 */
extern uint32 GetNumberOfActivatedCores(void);

/**
 * @brief           Get the logical ID of the currently executing core.
 * @return          CoreIdType
 * @retval          Logic ID
 * @retval          OS_CORE_INVALID
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58504
 */
extern CoreIdType GetCoreID(void);

/**
 * @brief           Get the logical ID of the currently executing core.
 * @param[in]       Error: error code, see StatusType
 * @return          CoreIdType
 * @retval          Logic ID
 * @retval          OS_CORE_INVALID
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           CPD-58506
 */
extern void ShutdownAllCores(StatusType Error);

#if (CFG_PERIPHERAL_MAX > 0U)
/**
 * @brief           This service returns the content of a given memory location (<Address>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be read
 * @param[out]      ReadValue: Returned value (uint8)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType ReadPeripheral8(AreaIdType Area, const uint8* Address, uint8* ReadValue);

/**
 * @brief           This service returns the content of a given memory location (<Address>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be read
 * @param[out]      ReadValue: Returned value (uint16)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType ReadPeripheral16(AreaIdType Area, const uint16* Address, uint16* ReadValue);

/**
 * @brief           This service returns the content of a given memory location (<Address>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be read
 * @param[out]      ReadValue: Returned value (uint16)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType ReadPeripheral32(AreaIdType Area, const uint32* Address, uint32* ReadValue);

/**
 * @brief           This service writes the <value> to a given memory location (<memory address>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be writed
 * @param[in]       WriteValue: write value (uint8)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType WritePeripheral8(AreaIdType Area, uint8* Address, uint8 WriteValue);

/**
 * @brief           This service writes the <value> to a given memory location (<memory address>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be writed
 * @param[in]       WriteValue: write value (uint16)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType WritePeripheral16(AreaIdType Area, uint16* Address, uint16 WriteValue);

/**
 * @brief           This service writes the <value> to a given memory location (<memory address>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be writed
 * @param[in]       WriteValue: write value (uint32)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType WritePeripheral32(AreaIdType Area, uint32* Address, uint32 WriteValue);

/**
 * @brief           This service modifies a given memory location (<memory address>) with the
 *                  formula: *<Address> = ((*<Address> & <clearmask>) | <setmask>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be modified
 * @param[in]       Clearmask: memory address will be modified by a bit-AND (uint8)
 * @param[in]       Setmask: memory address will be modified by a bit-OR (uint8)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType ModifyPeripheral8(AreaIdType Area, uint8* Address, uint8 Clearmask, uint8 Setmask);

/**
 * @brief           This service modifies a given memory location (<memory address>) with the
 *                  formula: *<Address> = ((*<Address> & <clearmask>) | <setmask>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be modified
 * @param[in]       Clearmask: memory address will be modified by a bit-AND (uint16)
 * @param[in]       Setmask: memory address will be modified by a bit-OR (uint16)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType ModifyPeripheral16(AreaIdType Area, uint16* Address, uint16 Clearmask, uint16 Setmask);

/**
 * @brief           This service modifies a given memory location (<memory address>) with the
 *                  formula: *<Address> = ((*<Address> & <clearmask>) | <setmask>).
 * @param[in]       Area: hardware peripheral area reference
 * @param[in]       Address: memory address, which will be modified
 * @param[in]       Clearmask: memory address will be modified by a bit-AND (uint32)
 * @param[in]       Setmask: memory address will be modified by a bit-OR (uint32)
 * @return          StatusType
 * @retval          E_OK: No error
 * @retval          E_OS_ID: Area id is out of range (EXTENDED status)
 * @retval          E_OS_VALUE: Address does not belong to given Area (EXTENDED status)
 * @retval          E_OS_CALLEVEL: Wrong call context of the API function (EXTENDED status)
 * @retval          E_OS_ACCESS: The calling task or ISR is not allowed to access the given
 * @synchronous     TRUE
 * @reentrant       Non Reentrant
 * @trace           -
 */
extern StatusType ModifyPeripheral32(AreaIdType Area, uint32* Address, uint32 Clearmask, uint32 Setmask);
#endif

#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"
#endif /* OS_H */

/*=======[E N D   O F   F I L E]==============================================*/
