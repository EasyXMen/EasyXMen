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
**  FILENAME    : Os_Err.h                                                    **
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

#ifndef OS_ERR_H
#define OS_ERR_H
/*=======[I N C L U D E S]=================================================*/
#include "Os_Cfg.h"

/*=======[M A C R O S]========================================================*/
#if (TRUE == CFG_USEGETSERVICEID)
#define OSErrorGetServiceId()             Os_SCB.sysOsServiceId;

#define OSError_Save_ServiceId(ServiceId) Os_SCB.sysOsServiceId = (ServiceId);

#else
#define OSErrorGetServiceId()
#define OSError_Save_ServiceId(ServiceId)
#endif /* TRUE == CFG_USEGETSERVICEID */

#if (TRUE == CFG_USEPARAMETERACCESS)
#define OSError_Save_ActivateTask(param1) Os_SCB.sysRunTimeSrvPrarm.taskId = (param1);

#define OSError_Save_TerminateTask()

#define OSError_Save_ChainTask(param1) Os_SCB.sysRunTimeSrvPrarm.taskId = (param1);

#define OSError_Save_Schedule()

#define OSError_Save_GetTaskID(param1) Os_SCB.sysRunTimeSrvPrarm.taskRef = (param1);

#define OSError_Save_GetTaskState(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.taskId = (param1);  \
    Os_SCB.sysRunTimeSrvPrarm.taskState = (param2);

#define OSError_Save_GetResource(param1)     Os_SCB.sysRunTimeSrvPrarm.resourceId = (param1);

#define OSError_Save_ReleaseResource(param1) Os_SCB.sysRunTimeSrvPrarm.resourceId = (param1);

#define OSError_Save_SetEvent(param1, param2)    \
    Os_SCB.sysRunTimeSrvPrarm.taskId = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.eventMask = (param2);

#define OSError_Save_ClearEvent(param1) Os_SCB.sysRunTimeSrvPrarm.eventMask = (param1);

#define OSError_Save_GetEvent(param1, param2)    \
    Os_SCB.sysRunTimeSrvPrarm.taskId = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.eventRef = (param2);

#define OSError_Save_WaitEvent(param1) Os_SCB.sysRunTimeSrvPrarm.eventMask = (param1);

#define OSError_Save_GetAlarm(param1, param2)     \
    Os_SCB.sysRunTimeSrvPrarm.alarmId = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.alarmTickRef = (param2);

#define OSError_Save_GetAlarmBase(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.alarmId = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.alarmBaseRef = (param2);

#define OSError_Save_CancelAlarm(param1) Os_SCB.sysRunTimeSrvPrarm.alarmId = (param1);

#define OSError_Save_SetRelAlarm(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.alarmId = (param1);        \
    Os_SCB.sysRunTimeSrvPrarm.alarmTime = (param2);      \
    Os_SCB.sysRunTimeSrvPrarm.alarmCycle = (param3);

#define OSError_Save_SetAbsAlarm(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.alarmId = (param1);        \
    Os_SCB.sysRunTimeSrvPrarm.alarmTime = (param2);      \
    Os_SCB.sysRunTimeSrvPrarm.alarmCycle = (param3);

#define OSError_Save_IncrementCounter(param1) Os_SCB.sysRunTimeSrvPrarm.counterId = (param1);

#define OSError_Save_GetCounterValue(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.counterId = (param1);  \
    Os_SCB.sysRunTimeSrvPrarm.counterValue = (param2);

#define OSError_Save_GetElapsedValue(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.counterId = (param1);          \
    Os_SCB.sysRunTimeSrvPrarm.counterValue = (param2);       \
    Os_SCB.sysRunTimeSrvPrarm.counterElapsedValue = (param3);

#define OSError_Save_GetApplicationID()

#define OSError_Save_GetCurrentApplicationID()

#define OSError_Save_CheckObjectAccess(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.ApplID = (param1);               \
    Os_SCB.sysRunTimeSrvPrarm.ObjectType = (param2);           \
    Os_SCB.sysRunTimeSrvPrarm.ObjectID = (param3);

#define OSError_Save_CheckObjectOwnership(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.ObjectType = (param1);      \
    Os_SCB.sysRunTimeSrvPrarm.ObjectID = (param2);

#define OSError_Save_TerminateApplication(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.ApplID = (param1);          \
    Os_SCB.sysRunTimeSrvPrarm.RestartOption = (param2);

#define OSError_Save_CheckISRMemoryAccess(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.ISRID = (param1);                   \
    Os_SCB.sysRunTimeSrvPrarm.MemStartAddress = (param2);         \
    Os_SCB.sysRunTimeSrvPrarm.MemSize = (param3);

#define OSError_Save_CheckTaskMemoryAccess(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.taskId = (param1);                   \
    Os_SCB.sysRunTimeSrvPrarm.MemStartAddress = (param2);          \
    Os_SCB.sysRunTimeSrvPrarm.MemSize = (param3);

#define OSError_Save_ActivateTaskAsyn(param1) Os_SCB.sysRunTimeSrvPrarm.taskId = (param1);

#define OSError_Save_AllowAccess()

#define OSError_Save_GetApplicationState(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.ApplID = (param1);         \
    Os_SCB.sysRunTimeSrvPrarm.appStateRef = (param2);

#define OSError_Save_StartCore(param1, param2)   \
    Os_SCB.sysRunTimeSrvPrarm.CoreID = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.Status = (param2);

#define OSError_Save_StartNonAutosarCore(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.CoreID = (param1);         \
    Os_SCB.sysRunTimeSrvPrarm.Status = (param2);

#define OSError_Save_GetNumberOfActivatedCores()

#define OSError_Save_ControlIdle(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.CoreID = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.IdleMode = (param2);

#define OSError_Save_SetEventAsyn(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.taskId = (param1);  \
    Os_SCB.sysRunTimeSrvPrarm.eventMask = (param2);

#define OSError_Save_GetISRID()

#define OSError_Save_EnableInterruptSource(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.ISRID = (param1);            \
    Os_SCB.sysRunTimeSrvPrarm.ClearPending = (param2);

#define OSError_Save_DisableInterruptSource(param1) Os_SCB.sysRunTimeSrvPrarm.ISRID = (param1);

#define OSError_Save_ClearPendingInterrupt(param1)  Os_SCB.sysRunTimeSrvPrarm.ISRID = (param1);

#define OSError_Save_ReadPeripheral8(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);               \
    Os_SCB.sysRunTimeSrvPrarm.Address8 = (param2);           \
    Os_SCB.sysRunTimeSrvPrarm.Value8 = (param2);

#define OSError_Save_ReadPeripheral16(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);                \
    Os_SCB.sysRunTimeSrvPrarm.Address16 = (param2);           \
    Os_SCB.sysRunTimeSrvPrarm.Value16 = (param2);

#define OSError_Save_ReadPeripheral32(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);                \
    Os_SCB.sysRunTimeSrvPrarm.Address32 = (param2);           \
    Os_SCB.sysRunTimeSrvPrarm.Value32 = (param2);

#define OSError_Save_WritePeripheral8(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);                \
    Os_SCB.sysRunTimeSrvPrarm.Address8 = (param2);            \
    Os_SCB.sysRunTimeSrvPrarm.Value8 = (param2);

#define OSError_Save_WritePeripheral16(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);                 \
    Os_SCB.sysRunTimeSrvPrarm.Address16 = (param2);            \
    Os_SCB.sysRunTimeSrvPrarm.Value16 = (param2);

#define OSError_Save_WritePeripheral32(param1, param2, param3) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);                 \
    Os_SCB.sysRunTimeSrvPrarm.Address32 = (param2);            \
    Os_SCB.sysRunTimeSrvPrarm.Value32 = (param2);

#define OSError_Save_ModifyPeripheral8(param1, param2, param3, param4) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);                         \
    Os_SCB.sysRunTimeSrvPrarm.Address8 = (param2);                     \
    Os_SCB.sysRunTimeSrvPrarm.Clearmask8 = (param3);                   \
    Os_SCB.sysRunTimeSrvPrarm.Setmask8 = (param4);

#define OSError_Save_ModifyPeripheral16(param1, param2, param3, param4) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);                          \
    Os_SCB.sysRunTimeSrvPrarm.Address16 = (param2);                     \
    Os_SCB.sysRunTimeSrvPrarm.Clearmask16 = (param3);                   \
    Os_SCB.sysRunTimeSrvPrarm.Setmask16 = (param4);

#define OSError_Save_ModifyPeripheral32(param1, param2, param3, param4) \
    Os_SCB.sysRunTimeSrvPrarm.Area = (param1);                          \
    Os_SCB.sysRunTimeSrvPrarm.Address32 = (param2);                     \
    Os_SCB.sysRunTimeSrvPrarm.Clearmask32 = (param3);                   \
    Os_SCB.sysRunTimeSrvPrarm.Setmask32 = (param4);

#define OSError_Save_GetScheduleTableStatus(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.ScheduleTableId = (param1);   \
    Os_SCB.sysRunTimeSrvPrarm.ScheduleStatus = (param2);

#define OSError_Save_StartScheduleTableRel(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.ScheduleTableId = (param1);  \
    Os_SCB.sysRunTimeSrvPrarm.Offset = (param2);

/* PRQA S 0791 ++*/ /* MISRA  Rule-5.4*/ /* OS_Err_MACRO_001 */
#define OSError_Save_StartScheduleTableAbs(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.ScheduleTableId = (param1);  \
    Os_SCB.sysRunTimeSrvPrarm.Start = (param2);
/* PRQA S 0791 --*/ /* MISRA  Rule-5.4*/
#define OSError_Save_StopScheduleTable(param1) Os_SCB.sysRunTimeSrvPrarm.ScheduleTableId = (param1);

#define OSError_Save_NextScheduleTable(param1, param2)    \
    Os_SCB.sysRunTimeSrvPrarm.ScheduleTableId = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.Start = (param2);

/* PRQA S 0791 ++*/ /* MISRA  Rule-5.4*/ /* OS_Err_MACRO_001 */
#define OSError_Save_StartScheduleTableSynchron(param1) Os_SCB.sysRunTimeSrvPrarm.ScheduleTableId = (param1);
/* PRQA S 0791 --*/ /* MISRA  Rule-5.4*/
#define OSError_Save_SyncScheduleTable(param1, param2)    \
    Os_SCB.sysRunTimeSrvPrarm.ScheduleTableId = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.value = (param2);

#define OSError_Save_SetScheduleTableAsync(param1) Os_SCB.sysRunTimeSrvPrarm.ScheduleTableId = (param1);

#define OSError_Save_GetSpinlock(param1)           Os_SCB.sysRunTimeSrvPrarm.SpinlockId = (param1);

#define OSError_Save_ReleaseSpinlock(param1)       Os_SCB.sysRunTimeSrvPrarm.SpinlockId = (param1);

#define OSError_Save_TryToGetSpinlock(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.SpinlockId = (param1);  \
    Os_SCB.sysRunTimeSrvPrarm.Success = (param2);

#define OSError_Save_CallTrustedFunction(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.FunctionIndex = (param1);  \
    Os_SCB.sysRunTimeSrvPrarm.FunctionParams = (param2);

#define OSError_Save_IocSend(param1, param2)    \
    Os_SCB.sysRunTimeSrvPrarm.Value = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocWrite(param1, param2)   \
    Os_SCB.sysRunTimeSrvPrarm.Value = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocSendGroup(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.Value = (param1);   \
    Os_SCB.sysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocWriteGroup(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.Value = (param1);    \
    Os_SCB.sysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocRead(param1, param2)    \
    Os_SCB.sysRunTimeSrvPrarm.Value = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocReceive(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.Value = (param1); \
    Os_SCB.sysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocReadGroup(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.Value = (param1);   \
    Os_SCB.sysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocReceiveGroup(param1, param2) \
    Os_SCB.sysRunTimeSrvPrarm.Value = (param1);      \
    Os_SCB.sysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocEmptyQueue(param1) Os_SCB.sysRunTimeSrvPrarm.Value = (param1);

#define OSError_Save_ShutDownOs(param1)    Os_SCB.sysRunTimeSrvPrarm.Value = (param1);
#else
#define OSError_Save_ActivateTask(param1)
#define OSError_Save_TerminateTask()
#define OSError_Save_ChainTask(param1)
#define OSError_Save_Schedule()
#define OSError_Save_GetTaskID(param1)
#define OSError_Save_GetTaskState(param1, param2)
#define OSError_Save_GetResource(param1)
#define OSError_Save_ReleaseResource(param1)
#define OSError_Save_SetEvent(param1, param2)
#define OSError_Save_ClearEvent(param1)
#define OSError_Save_GetEvent(param1, param2)
#define OSError_Save_WaitEvent(param1)
#define OSError_Save_GetAlarm(param1, param2)
#define OSError_Save_GetAlarmBase(param1, param2)
#define OSError_Save_CancelAlarm(param1)
#define OSError_Save_SetRelAlarm(param1, param2, param3)
#define OSError_Save_SetAbsAlarm(param1, param2, param3)
#define OSError_Save_IncrementCounter(param1)
#define OSError_Save_GetCounterValue(param1, param2)
#define OSError_Save_GetElapsedValue(param1, param2, param3)
#define OSError_Save_GetApplicationID()
#define OSError_Save_GetCurrentApplicationID()
#define OSError_Save_CheckObjectAccess(param1, param2, param3)
#define OSError_Save_CheckObjectOwnership(param1, param2)
#define OSError_Save_TerminateApplication(param1, param2)
#define OSError_Save_CheckISRMemoryAccess(param1, param2, param3)
#define OSError_Save_CheckTaskMemoryAccess(param1, param2, param3)
#define OSError_Save_ActivateTaskAsyn(param1)
#define OSError_Save_AllowAccess()
#define OSError_Save_GetApplicationState(param1, param2)
#define OSError_Save_StartCore(param1, param2)
#define OSError_Save_StartNonAutosarCore(param1, param2)
#define OSError_Save_GetNumberOfActivatedCores()
#define OSError_Save_ControlIdle(param1, param2)
#define OSError_Save_SetEventAsyn(param1, param2)
#define OSError_Save_GetISRID()
#define OSError_Save_EnableInterruptSource(param1, param2)
#define OSError_Save_DisableInterruptSource(param1)
#define OSError_Save_ClearPendingInterrupt(param1)
#define OSError_Save_ReadPeripheral8(param1, param2, param3)
#define OSError_Save_ReadPeripheral16(param1, param2, param3)
#define OSError_Save_ReadPeripheral32(param1, param2, param3)
#define OSError_Save_WritePeripheral8(param1, param2, param3)
#define OSError_Save_WritePeripheral16(param1, param2, param3)
#define OSError_Save_WritePeripheral32(param1, param2, param3)
#define OSError_Save_ModifyPeripheral8(param1, param2, param3, param4)
#define OSError_Save_ModifyPeripheral16(param1, param2, param3, param4)
#define OSError_Save_ModifyPeripheral32(param1, param2, param3, param4)
#define OSError_Save_GetScheduleTableStatus(param1, param2)
#define OSError_Save_StartScheduleTableRel(param1, param2)
/* PRQA S 0791 ++*/ /* MISRA  Rule-5.4*/ /* OS_Err_MACRO_001 */
#define OSError_Save_StartScheduleTableAbs(param1, param2)
/* PRQA S 0791 --*/                      /* MISRA  Rule-5.4*/
#define OSError_Save_StopScheduleTable(param1)
#define OSError_Save_NextScheduleTable(param1, param2)
/* PRQA S 0791 ++*/ /* MISRA  Rule-5.4*/ /* OS_Err_MACRO_001 */
#define OSError_Save_StartScheduleTableSynchron(param1)
/* PRQA S 0791 --*/                      /* MISRA  Rule-5.4*/
#define OSError_Save_SyncScheduleTable(param1, param2)
#define OSError_Save_SetScheduleTableAsync(param1)
#define OSError_Save_GetSpinlock(param1)
#define OSError_Save_ReleaseSpinlock(param1)
#define OSError_Save_TryToGetSpinlock(param1, param2)
#define OSError_Save_CallTrustedFunction(param1, param2)
#define OSError_Save_IocSend(param1, param2)
#define OSError_Save_IocWrite(param1, param2)
#define OSError_Save_IocSendGroup(param1, param2)
#define OSError_Save_IocWriteGroup(param1, param2)
#define OSError_Save_IocRead(param1, param2)
#define OSError_Save_IocReceive(param1, param2)
#define OSError_Save_IocReadGroup(param1, param2)
#define OSError_Save_IocReceiveGroup(param1, param2)
#define OSError_Save_IocEmptyQueue(param1)
#define OSError_Save_ShutDownOs(param1)
#endif /* TRUE == CFG_USEPARAMETERACCESS */
#endif /* OS_ERR_H */

/*=======[E N D   O F   F I L E]============================================*/
