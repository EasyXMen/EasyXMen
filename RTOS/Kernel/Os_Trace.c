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
**  FILENAME    : Os_Trace.c                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : trace manager                                                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*=======[I N C L U D E S]====================================================*/
#include "Os.h"
#include "Os_Internal.h"
#include "Os_Trace.h"

/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

#if (TRUE == CFG_TRACE_ENABLE)

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Initialize the ORTI Trace function.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <None>
 * @param[out]          <None>
 * @param[in/out]       <None>
 * @return              <None>
 * PreCondition         <None>
 * CallByAPI            <StartOS>
 * REQ ID               <None>
 */
/******************************************************************************/
FUNC(void, OS_CODE) Os_InitTrace(void)
{
    VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;
    VAR(Os_AppObjectIDType, OS_VAR) objectId = 0u;

    /* Initialize global trace data of current core. */
    Os_SCB.trace = Os_TraceSys_Inf[coreId];
    Os_SCB.trace->coreId = coreId;
    Os_SCB.trace->curAppMode = Os_SCB.sysActiveAppMode;
    Os_SCB.trace->runObjId = OS_TRACE_NO_OBJECT_ID;
    Os_SCB.trace->runIsrId = OS_TRACE_NO_ISR_ID;
    Os_SCB.trace->runTaskId = OS_TRACE_NO_TASK_ID;
    Os_SCB.trace->runTaskPrio = OS_TRACE_NO_TASK_ID;
    Os_SCB.trace->lastError = OS_TRACE_NO_LAST_ERROR;
    Os_SCB.trace->isrTrace = Os_TraceIsr_Inf[coreId];
    Os_SCB.trace->taskTrace = Os_TraceTask_Inf[coreId];
    Os_SCB.trace->alarmTrace = Os_TraceAlarm_Inf[coreId];
    Os_SCB.trace->resourceTrace = Os_TraceResource_Inf[coreId];

    /* Initialize ISR states. */
    for (objectId = 0u; objectId < Os_CfgIsrMax; objectId++)
    {
        Os_SCB.trace->isrTrace[objectId].state = OS_TRACE_ISR_STATE_EXITED;
        Os_SCB.trace->isrTrace[objectId].realState = TASK_STATE_START;
    }
    /* Initialize task states. */
    for (objectId = 0u; objectId < Os_SCB.sysTaskMax; objectId++)
    {
        Os_SCB.trace->taskTrace[objectId].switchReason = OS_TRACE_TASK_SWITCH_REASON_NONE;
        Os_SCB.trace->taskTrace[objectId].state = OS_TRACE_TASK_STATE_TERMINATED;
        Os_SCB.trace->taskTrace[objectId].realState = TASK_STATE_START;
    }
    /* Initialize alarm states. */
    for (objectId = 0u; objectId < Os_SCB.sysAlarmMax; objectId++)
    {
        Os_SCB.trace->alarmTrace[objectId].state = OS_TRACE_ALARM_STATE_STOPPED;
    }

    /* Initialize resource states. */
    for (objectId = 0u; objectId < Os_CfgStdResourceMax_Inf[coreId]; objectId++)
    {
        Os_SCB.trace->resourceTrace[objectId].occupyObjectId = OS_TRACE_NO_OBJECT_ID;
        Os_SCB.trace->resourceTrace[objectId].state = OS_TRACE_RES_STATE_UNLOCKED;
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Trace the last error generated.>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceLastError(StatusType error)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->lastError = error;

        OS_TRACE_LAST_ERROR_HOOK(coreId, error);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Trace the start of task.>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceTaskStart(Os_TaskType taskId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->taskTrace[taskId].switchReason = OS_TRACE_TASK_SWITCH_REASON_START_ACTIVE;

        OS_TRACE_TASK_START_HOOK(coreId, taskId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_TraceTaskRun(Os_TaskType taskId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->runTaskId = OS_TRACE_GET_OBJECT_TASK_ID(coreId, taskId);
        Os_SCB.trace->runTaskPrio = Os_SCB.sysRunningTCB->taskRunPrio;
        Os_SCB.trace->taskTrace[taskId].realState = TASK_STATE_RUNNING;
        Os_SCB.trace->runObjId = Os_SCB.trace->runTaskId;

        OS_TRACE_TASK_RUN_HOOK(coreId, taskId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Trace the activation of task.>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceTaskActive(Os_TaskType taskId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->taskTrace[taskId].switchReason = OS_TRACE_TASK_SWITCH_REASON_POSITIVE_ACTIVE;

        OS_TRACE_TASK_ACTIVE_HOOK(coreId, taskId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE)
Os_TraceTaskSwitch(
    Os_TaskType curTaskId,
    Os_TaskType nextTaskId,
    Os_TraceTaskSwitchReasonType curReason,
    Os_TraceTaskSwitchReasonType nextReason)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->nextTaskId = nextTaskId;
        Os_SCB.trace->nextRunReason = nextReason;
        Os_SCB.trace->taskTrace[curTaskId].switchReason = curReason;
        Os_SCB.trace->taskTrace[nextTaskId].switchReason = nextReason;
        Os_SCB.trace->taskTrace[curTaskId].realState = TASK_STATE_READY;

        OS_TRACE_TASK_SWITCH_HOOK(coreId, curTaskId, nextTaskId, curReason, nextReason);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Trace the termination of task.>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceTaskTerminate(Os_TaskType taskId, Os_TraceTaskSwitchReasonType termReason)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->runTaskId = OS_TRACE_NO_TASK_ID;
        Os_SCB.trace->runTaskPrio = 0u;
        Os_SCB.trace->taskTrace[taskId].switchReason = termReason;
        Os_SCB.trace->taskTrace[taskId].realState = TASK_STATE_SUSPENDED;

        Os_SCB.trace->runObjId = OS_TRACE_NO_OBJECT_ID;

        OS_TRACE_TASK_TERMINATE_HOOK(coreId, taskId, termReason);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_TraceIsrEnter(Os_IsrType isrId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->runIsrId = OS_TRACE_GET_OBJECT_ISR_ID(coreId, isrId);
        Os_SCB.trace->isrTrace[isrId].state = OS_TRACE_ISR_STATE_ENTERED;

        Os_SCB.trace->runObjId = Os_SCB.trace->runIsrId;
        Os_SCB.trace->isrTrace[isrId].realState = TASK_STATE_RUNNING;

        OS_TRACE_ISR_ENTER_HOOK(coreId, isrId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_TraceIsrExit(Os_IsrType isrId, Os_IsrType nestIsrId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->isrTrace[isrId].state = OS_TRACE_ISR_STATE_EXITED;
        Os_SCB.trace->isrTrace[isrId].realState = TASK_STATE_READY;
        if (nestIsrId != isrId)
        {
            Os_SCB.trace->runIsrId = OS_TRACE_GET_OBJECT_ISR_ID(coreId, nestIsrId);
            Os_SCB.trace->runObjId = Os_SCB.trace->runIsrId;
            Os_SCB.trace->isrTrace[nestIsrId].realState = TASK_STATE_RUNNING;
        }
        else
        {
            Os_SCB.trace->runIsrId = OS_TRACE_NO_ISR_ID;
            Os_SCB.trace->runObjId = OS_TRACE_GET_OBJECT_TASK_ID(coreId, Os_SCB.sysRunningTaskID);
        }

        OS_TRACE_ISR_EXIT_HOOK(coreId, isrId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_TraceServiceEnter(Os_ServiceIdType serviceId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->curServiceId = serviceId;

        OS_TRACE_SERVICE_ENTER_HOOK(coreId, serviceId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
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
FUNC(void, OS_CODE) Os_TraceServiceExit(Os_ServiceIdType serviceId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->curServiceId = OSServiceId_NoServiceRunning;

        OS_TRACE_SERVICE_EXIT_HOOK(coreId, serviceId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Track the status of resources acquired by ISR.>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceResourceIsrGet(Os_ResourceType resourceId, Os_IsrType isrId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->resourceTrace[resourceId].occupyObjectId = OS_TRACE_GET_OBJECT_ISR_ID(coreId, isrId);
        Os_SCB.trace->resourceTrace[resourceId].state = OS_TRACE_RES_STATE_LOCKED;

        OS_TRACE_RESOURCE_ISR_GET_HOOK(coreId, resourceId, isrId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Track the status of resources acquired by task.>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceResourceTaskGet(Os_ResourceType resourceId, Os_TaskType taskId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->resourceTrace[resourceId].occupyObjectId = OS_TRACE_GET_OBJECT_TASK_ID(coreId, taskId);
        Os_SCB.trace->resourceTrace[resourceId].state = OS_TRACE_RES_STATE_LOCKED;

        OS_TRACE_RESOURCE_TASK_GET_HOOK(coreId, resourceId, taskId);
    }

    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Trace the releasement of resources>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceResourceRelease(Os_ResourceType resourceId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->resourceTrace[resourceId].occupyObjectId = OS_TRACE_NO_OBJECT_ID;
        Os_SCB.trace->resourceTrace[resourceId].state = OS_TRACE_RES_STATE_UNLOCKED;

        OS_TRACE_RESOURCE_RELEASE_HOOK(coreId, resourceId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Trace the start of alarm.>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceAlarmStart(Os_AlarmType alarmId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->alarmTrace[alarmId].state = OS_TRACE_ALARM_STATE_RUNNING;

        OS_TRACE_ALARM_START_HOOK(coreId, alarmId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/******************************************************************************/
/*
 * Brief                <Trace the stop of alarm.>
 * Service ID   :       <None>
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
FUNC(void, OS_CODE) Os_TraceAlarmStop(Os_AlarmType alarmId)
{
    if (NULL_PTR != Os_SCB.trace)
    {
        VAR(Os_CoreIdType, OS_VAR) coreId = Os_SCB.sysCore;

        Os_SCB.trace->alarmTrace[alarmId].state = OS_TRACE_ALARM_STATE_STOPPED;

        OS_TRACE_ALARM_STOP_HOOK(coreId, alarmId);
    }
    return;
}
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* TRUE == CFG_TRACE_ENABLE */

/*=======[E N D   O F   F I L E]==============================================*/
