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
**  FILENAME    : Os_Trace.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef OS_TRACE_H
#define OS_TRACE_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/
#if (TRUE == CFG_TRACE_ENABLE)

#define OS_TRACE_GET_OBJECT_LOCAL_ID(coreId, objId) ((coreId << 12u) | (objId))

#define OS_TRACE_GET_OBJECT_TASK_ID(coreId, objId)  OS_TRACE_GET_OBJECT_LOCAL_ID(coreId, objId)
#define OS_TRACE_GET_OBJECT_ISR_ID(coreId, objId)   (0x8000u + OS_TRACE_GET_OBJECT_LOCAL_ID(coreId, objId))

#if (TRUE == CFG_TRACE_HOOK_ENABLE)

#define OS_TRACE_LAST_ERROR_HOOK(coreId, error)   TraceLastErrorHook(coreId, error)

#define OS_TRACE_TASK_START_HOOK(coreId, taskId)  TraceTaskStartHook(coreId, taskId)

#define OS_TRACE_TASK_ACTIVE_HOOK(coreId, taskId) TraceTaskActiveHook(coreId, taskId)

#define OS_TRACE_TASK_RUN_HOOK(coreId, taskId)    TraceTaskRunHook(coreId, taskId)

#define OS_TRACE_TASK_SWITCH_HOOK(coreId, curTaskId, nextTaskId, curReason, nextReason) \
    TraceTaskSwitchHook(coreId, curTaskId, nextTaskId, curReason, nextReason)

#define OS_TRACE_TASK_TERMINATE_HOOK(coreId, taskId, termReason)    TraceTaskTerminateHook(coreId, taskId, termReason)

#define OS_TRACE_ISR_ENTER_HOOK(coreId, isrId)                      TraceIsrEnterHook(coreId, isrId)

#define OS_TRACE_ISR_EXIT_HOOK(coreId, isrId)                       TraceIsrExitHook(coreId, isrId)

#define OS_TRACE_SERVICE_ENTER_HOOK(coreId, serviceId)              TraceServiceEnterHook(coreId, serviceId)

#define OS_TRACE_SERVICE_EXIT_HOOK(coreId, serviceId)               TraceServiceExitHook(coreId, serviceId)

#define OS_TRACE_RESOURCE_ISR_GET_HOOK(coreId, resourceId, isrId)   TraceResourceIsrGetHook(coreId, resourceId, isrId)

#define OS_TRACE_RESOURCE_TASK_GET_HOOK(coreId, resourceId, taskId) TraceResourceTaskGetHook(coreId, resourceId, taskId)

#define OS_TRACE_RESOURCE_RELEASE_HOOK(coreId, resourceId)          TraceResourceReleaseHook(coreId, resourceId)

#define OS_TRACE_ALARM_START_HOOK(coreId, alarmId)                  TraceAlarmStartHook(coreId, alarmId)

#define OS_TRACE_ALARM_STOP_HOOK(coreId, alarmId)                   TraceAlarmStopHook(coreId, alarmId)

#else /* FALSE == CFG_TRACE_HOOK_ENABLE */

#define OS_TRACE_LAST_ERROR_HOOK(coreId, error) \
{                                               \
    UNUSED_PARAMETER(coreId);                   \
    UNUSED_PARAMETER(error);                    \
}
#define OS_TRACE_TASK_START_HOOK(coreId, taskId) \
{                                                \
    UNUSED_PARAMETER(coreId);                    \
    UNUSED_PARAMETER(error);                     \
}
#define OS_TRACE_TASK_ACTIVE_HOOK(coreId, taskId) \
{                                                 \
    UNUSED_PARAMETER(coreId);                     \
    UNUSED_PARAMETER(taskId);                     \
}
#define OS_TRACE_TASK_RUN_HOOK(coreId, taskId) \
{                                              \
    UNUSED_PARAMETER(coreId);                  \
    UNUSED_PARAMETER(taskId);                  \
}
#define OS_TRACE_TASK_SWITCH_HOOK(coreId, curTaskId, nextTaskId, curReason, nextReason) \
{                                                                                       \
    UNUSED_PARAMETER(coreId);                                                           \
    UNUSED_PARAMETER(curTaskId);                                                        \
    UNUSED_PARAMETER(nextTaskId);                                                       \
    UNUSED_PARAMETER(curReason);                                                        \
    UNUSED_PARAMETER(nextReason);                                                       \
}
#define OS_TRACE_TASK_TERMINATE_HOOK(coreId, taskId, termReason) \
{                                                                \
    UNUSED_PARAMETER(coreId);                                    \
    UNUSED_PARAMETER(taskId);                                    \
    UNUSED_PARAMETER(termReason);                                \
}
#define OS_TRACE_ISR_ENTER_HOOK(coreId, isrId) \
{                                              \
    UNUSED_PARAMETER(coreId);                  \
    UNUSED_PARAMETER(isrId);                   \
}
#define OS_TRACE_ISR_EXIT_HOOK(coreId, isrId) \
{                                             \
    UNUSED_PARAMETER(coreId);                 \
    UNUSED_PARAMETER(isrId);                  \
}
#define OS_TRACE_SERVICE_ENTER_HOOK(coreId, serviceId) \
{                                                      \
    UNUSED_PARAMETER(coreId);                          \
    UNUSED_PARAMETER(serviceId);                       \
}
#define OS_TRACE_SERVICE_EXIT_HOOK(coreId, serviceId) \
{                                                     \
    UNUSED_PARAMETER(coreId);                         \
    UNUSED_PARAMETER(serviceId);                      \
}
#define OS_TRACE_RESOURCE_ISR_GET_HOOK(coreId, resourceId, isrId) \
{                                                                 \
    UNUSED_PARAMETER(coreId);                                     \
    UNUSED_PARAMETER(resourceId);                                 \
    UNUSED_PARAMETER(isrId);                                      \
}
#define OS_TRACE_RESOURCE_TASK_GET_HOOK(coreId, resourceId, taskId) \
{                                                                   \
    UNUSED_PARAMETER(coreId);                                       \
    UNUSED_PARAMETER(resourceId);                                   \
    UNUSED_PARAMETER(taskId);                                       \
}
#define OS_TRACE_RESOURCE_RELEASE_HOOK(coreId, resourceId) \
{                                                          \
    UNUSED_PARAMETER(coreId);                              \
    UNUSED_PARAMETER(resourceId);                          \
}
#define OS_TRACE_ALARM_START_HOOK(coreId, alarmId) \
{                                                  \
    UNUSED_PARAMETER(coreId);                      \
    UNUSED_PARAMETER(alarmId);                     \
}
#define OS_TRACE_ALARM_STOP_HOOK(coreId, alarmId) \
{                                                 \
    UNUSED_PARAMETER(coreId);                     \
    UNUSED_PARAMETER(alarmId);                    \
}

#endif /* TRUE == CFG_TRACE_HOOK_ENABLE */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/******************************************************************************/
/*
 * Brief               <Initialize the ORTI Trace function.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-Name[in]      <>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <StartOS>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InitTrace(void);
/******************************************************************************/
/*
 * Brief                <Trace the last error generated.>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-error[in]     <the last error generated>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_CallErrorHook>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceLastError(StatusType error);
/******************************************************************************/
/*
 * Brief                <Trace the start of task.>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-taskId[in]    <Task ID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceTaskStart(Os_TaskType taskId);
/******************************************************************************/
/*
 * Brief               <Trace the execution of task.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-taskId[in]    <Task ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <Os_SwitchTask>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceTaskRun(Os_TaskType taskId);
/******************************************************************************/
/*
 * Brief                <Trace the activation of task.>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-taskId[in]    <Task ID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ActivateTask>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceTaskActive(Os_TaskType taskId);
/******************************************************************************/
/*
 * Brief               <Trace the switch of task.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-curTaskId[in] <ID of the currently running task>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <Os_TerminateApplication, Os_SetEvent, Os_WaitEvent
                        Os_ProtHookTerminateTask, Os_ReleaseResource, Os_SProTerminateTask
                        Os_TerminateTask, Os_ChainTask, Schedule>
 * CallByAPI           <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceTaskSwitch(
    Os_TaskType curTaskId,
    Os_TaskType nextTaskId,
    Os_TraceTaskSwitchReasonType curReason,
    Os_TraceTaskSwitchReasonType nextReason);
/******************************************************************************/
/*
 * Brief                <Trace the termination of task.>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-taskId[in]    <Task ID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <None>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceTaskTerminate(Os_TaskType taskId, Os_TraceTaskSwitchReasonType termReason);
/******************************************************************************/
/*
 * Brief               <Trace the entry state of ISR.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-isrId[in]     <ISR ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <Os_EnterISR2, Os_EnterISR1>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceIsrEnter(Os_IsrType isrId);
/******************************************************************************/
/*
 * Brief               <Trace the exit state of ISR.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-isrId[in]     <ISR ID>
 * Param-nestIsrId[in] <Nested ISR ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <Os_ExitISR2, Os_ExitISR1>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceIsrExit(Os_IsrType isrId, Os_IsrType nestIsrId);
/******************************************************************************/
/*
 * Brief               <Trace the entry state of system service.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-serviceId[in] <Service ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <All the API of system service.>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceServiceEnter(Os_ServiceIdType serviceId);
/******************************************************************************/
/*
 * Brief               <Trace the exit state of system service.>
 * ServiceId           <None>
 * Sync/Async          <Synchronous>
 * Reentrancy          <Non Reentrant>
 * Param-serviceId[in] <Service ID>
 * Param-Name[out]     <None>
 * Param-Name[in/out]  <None>
 * Return              <None>
 * PreCondition        <None>
 * CallByAPI           <All the API of system service.>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceServiceExit(Os_ServiceIdType serviceId);
/******************************************************************************/
/*
 * Brief                <Track the status of resources acquired by ISR.>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-resourceId[in] <Resource ID>
 * @param-isrId[in]     <ID of the ISR which get the resource.>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_GetResource>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceResourceIsrGet(Os_ResourceType resourceId, Os_IsrType isrId);

/******************************************************************************/
/*
 * Brief                <Track the status of resources acquired by task.>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-resourceId[in] <Resource ID>
 * @param-taskId[in]    <ID of the task which get the resource.>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_GetResource>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceResourceTaskGet(Os_ResourceType resourceId, Os_TaskType taskId);

/******************************************************************************/
/*
 * Brief                <Trace the releasement of resources>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-resourceId[in] <Resource ID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_ReleaseResource>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceResourceRelease(Os_ResourceType resourceId);

/******************************************************************************/
/*
 * Brief                <Trace the start of alarm.>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-alarmId[in]   <Alarm ID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_InsertAlarm>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceAlarmStart(Os_AlarmType alarmId);

/******************************************************************************/
/*
 * Brief                <Trace the stop of alarm.>
 * Service ID   :       <>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param-alarmId[in]   <Alarm ID>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <Os_DeleteAlarm>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_TraceAlarmStop(Os_AlarmType alarmId);

#if (TRUE == CFG_TRACE_HOOK_ENABLE)
extern FUNC(void, OS_CODE) TraceLastErrorHook(Os_CoreIdType coreId, StatusType error);
extern FUNC(void, OS_CODE) TraceTaskStartHook(Os_CoreIdType coreId, Os_TaskType taskId);
extern FUNC(void, OS_CODE) TraceTaskActiveHook(Os_CoreIdType coreId, Os_TaskType taskId);
extern FUNC(void, OS_CODE) TraceTaskRunHook(Os_CoreIdType coreId, Os_TaskType taskId);
extern FUNC(void, OS_CODE) TraceTaskSwitchHook(
    Os_CoreIdType coreId,
    Os_TaskType curTaskId,
    Os_TaskType nextTaskId,
    Os_TraceTaskSwitchReasonType curReason,
    Os_TraceTaskSwitchReasonType nextReason);
extern FUNC(void, OS_CODE)
    TraceTaskTerminateHook(Os_CoreIdType coreId, Os_TaskType taskId, Os_TraceTaskSwitchReasonType termReason);
extern FUNC(void, OS_CODE) TraceIsrEnterHook(Os_CoreIdType coreId, Os_IsrType isrId);
extern FUNC(void, OS_CODE) TraceIsrExitHook(Os_CoreIdType coreId, Os_IsrType isrId);
extern FUNC(void, OS_CODE) TraceServiceEnterHook(Os_CoreIdType coreId, Os_ServiceIdType serviceId);
extern FUNC(void, OS_CODE) TraceServiceExitHook(Os_CoreIdType coreId, Os_ServiceIdType serviceId);
extern FUNC(void, OS_CODE) TraceResourceIsrGetHook(Os_CoreIdType coreId, Os_ResourceType resourceId, Os_IsrType isrId);
extern FUNC(void, OS_CODE)
    TraceResourceTaskGetHook(Os_CoreIdType coreId, Os_ResourceType resourceId, Os_TaskType taskId);
extern FUNC(void, OS_CODE) TraceResourceReleaseHook(Os_CoreIdType coreId, Os_ResourceType resourceId);
extern FUNC(void, OS_CODE) TraceAlarmStartHook(Os_CoreIdType coreId, Os_AlarmType alarmId);
extern FUNC(void, OS_CODE) TraceAlarmStopHook(Os_CoreIdType coreId, Os_AlarmType alarmId);
#endif /* TRUE == CFG_TRACE_HOOK_ENABLE */

#endif /* TRUE == CFG_TRACE_ENABLE */

#endif /* OS_TRACE_H */
/*=======[E N D   O F   F I L E]==============================================*/
