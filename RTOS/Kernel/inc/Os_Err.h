/**
 * Copyright (C) 2008-2026 isoft Infrastructure Software Co., Ltd.
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
 **  @file               : Os_Err.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for error code definitions
 **
 ***********************************************************************************************************************/

#ifndef OS_ERR_H
#define OS_ERR_H

/* =================================================== inclusions =================================================== */
#include "Os_Hook.h"
#include "Os_FaultManager.h"

/* ===================================================== macros ===================================================== */
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
/**
 * @brief Save fault information.
 */
/* PRQA S 3458 ++ *//* VL_Os_3458 */
#define Os_FaultSave(pScb, err)                         \
        {                                               \
            Os_FalultRecordSysRuntimeInform(pScb, err); \
        }
/* PRQA S 3458 -- */
#else
/* PRQA S 3412 ++ */ /* VL_Os_3412 */
#define Os_FaultSave(pScb, err) \
        UNUSED_PARAMETER(pScb);
/* PRQA S 3412 -- */
#endif

#if (TRUE == CFG_USEGETSERVICEID)
/**
 * @brief Record the service ID in case of failure.
 */
/* PRQA S 3412 ++ */ /* VL_Os_3412 */
#define OSErrorSaveServiceId(serviceId) \
        pScb->SysServiceId = (serviceId);

#else
#define OSErrorSaveServiceId(serviceId)
#endif

#if (TRUE == CFG_USEPARAMETERACCESS)
/**
 * @brief Record specific system service information.
 */
#define OSError_Save_ActivateTask(param1) \
        pScb->SysRunTimeSrvPrarm.TaskId = (param1);

#define OSError_Save_TerminateTask()

#define OSError_Save_ChainTask(param1) \
        pScb->SysRunTimeSrvPrarm.TaskId = (param1);

#define OSError_Save_Schedule()

#define OSError_Save_GetTaskID(param1) \
        pScb->SysRunTimeSrvPrarm.TaskRef = (param1);

#define OSError_Save_GetTaskState(param1, param2)   \
        pScb->SysRunTimeSrvPrarm.TaskId = (param1); \
        pScb->SysRunTimeSrvPrarm.TaskState = (param2);

#define OSError_Save_GetResource(param1) \
        pScb->SysRunTimeSrvPrarm.ResourceId = (param1);

#define OSError_Save_ReleaseResource(param1) \
        pScb->SysRunTimeSrvPrarm.ResourceId = (param1);

#define OSError_Save_SetEvent(param1, param2)       \
        pScb->SysRunTimeSrvPrarm.TaskId = (param1); \
        pScb->SysRunTimeSrvPrarm.EventMask = (param2);

#define OSError_Save_ClearEvent(param1) \
        pScb->SysRunTimeSrvPrarm.EventMask = (param1);

#define OSError_Save_GetEvent(param1, param2)       \
        pScb->SysRunTimeSrvPrarm.TaskId = (param1); \
        pScb->SysRunTimeSrvPrarm.EventRef = (param2);

#define OSError_Save_WaitEvent(param1) \
        pScb->SysRunTimeSrvPrarm.EventMask = (param1);

#define OSError_Save_GetAlarm(param1, param2)        \
        pScb->SysRunTimeSrvPrarm.AlarmId = (param1); \
        pScb->SysRunTimeSrvPrarm.AlarmTickRef = (param2);

#define OSError_Save_GetAlarmBase(param1, param2)    \
        pScb->SysRunTimeSrvPrarm.AlarmId = (param1); \
        pScb->SysRunTimeSrvPrarm.AlarmBaseRef = (param2);

#define OSError_Save_CancelAlarm(param1) \
        pScb->SysRunTimeSrvPrarm.AlarmId = (param1);

#define OSError_Save_SetRelAlarm(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.AlarmId = (param1);     \
        pScb->SysRunTimeSrvPrarm.AlarmTime = (param2);   \
        pScb->SysRunTimeSrvPrarm.AlarmCycle = (param3);

#define OSError_Save_SetAbsAlarm(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.AlarmId = (param1);     \
        pScb->SysRunTimeSrvPrarm.AlarmTime = (param2);   \
        pScb->SysRunTimeSrvPrarm.AlarmCycle = (param3);

#define OSError_Save_IncrementCounter(param1) \
        pScb->SysRunTimeSrvPrarm.CounterId = (param1);

#define OSError_Save_GetCounterValue(param1, param2)   \
        pScb->SysRunTimeSrvPrarm.CounterId = (param1); \
        pScb->SysRunTimeSrvPrarm.CounterValue = (param2);

#define OSError_Save_GetElapsedValue(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.CounterId = (param1);       \
        pScb->SysRunTimeSrvPrarm.CounterValue = (param2);    \
        pScb->SysRunTimeSrvPrarm.CounterElapsedValue = (param3);

#define OSError_Save_GetApplicationID()

#define OSError_Save_GetCurrentApplicationID()

#define OSError_Save_CheckObjectAccess(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.ApplID = (param1);            \
        pScb->SysRunTimeSrvPrarm.ObjectType = (param2);        \
        pScb->SysRunTimeSrvPrarm.ObjectID = (param3);

#define OSError_Save_CheckObjectOwnership(param1, param2) \
        pScb->SysRunTimeSrvPrarm.ObjectType = (param1);   \
        pScb->SysRunTimeSrvPrarm.ObjectID = (param2);

#define OSError_Save_TerminateApplication(param1, param2) \
        pScb->SysRunTimeSrvPrarm.ApplID = (param1);       \
        pScb->SysRunTimeSrvPrarm.RestartOption = (param2);

#define OSError_Save_CheckISRMemoryAccess(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.ISRID = (param1);                \
        pScb->SysRunTimeSrvPrarm.MemStartAddress = (param2);      \
        pScb->SysRunTimeSrvPrarm.MemSize = (param3);

#define OSError_Save_CheckTaskMemoryAccess(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.TaskId = (param1);                \
        pScb->SysRunTimeSrvPrarm.MemStartAddress = (param2);       \
        pScb->SysRunTimeSrvPrarm.MemSize = (param3);

#define OSError_Save_ActivateTaskAsyn(param1) \
        pScb->SysRunTimeSrvPrarm.TaskId = (param1);

#define OSError_Save_AllowAccess()

#define OSError_Save_GetApplicationState(param1, param2) \
        pScb->SysRunTimeSrvPrarm.ApplID = (param1);      \
        pScb->SysRunTimeSrvPrarm.AppStateRef = (param2);

#define OSError_Save_StartCore(param1, param2)      \
        pScb->SysRunTimeSrvPrarm.CoreID = (param1); \
        pScb->SysRunTimeSrvPrarm.Status = (param2);

#define OSError_Save_StartNonAutosarCore(param1, param2) \
        pScb->SysRunTimeSrvPrarm.CoreID = (param1);      \
        pScb->SysRunTimeSrvPrarm.Status = (param2);

#define OSError_Save_GetNumberOfActivatedCores()

#define OSError_Save_ControlIdle(param1, param2)    \
        pScb->SysRunTimeSrvPrarm.CoreID = (param1); \
        pScb->SysRunTimeSrvPrarm.IdleMode = (param2);

#define OSError_Save_SetEventAsyn(param1, param2)   \
        pScb->SysRunTimeSrvPrarm.TaskId = (param1); \
        pScb->SysRunTimeSrvPrarm.EventMask = (param2);

#define OSError_Save_GetISRID()

#define OSError_Save_EnableInterruptSource(param1, param2) \
        pScb->SysRunTimeSrvPrarm.ISRID = (param1);         \
        pScb->SysRunTimeSrvPrarm.ClearPending = (param2);

#define OSError_Save_DisableInterruptSource(param1) \
        pScb->SysRunTimeSrvPrarm.ISRID = (param1);

#define OSError_Save_ClearPendingInterrupt(param1) \
        pScb->SysRunTimeSrvPrarm.ISRID = (param1);

#define OSError_Save_ReadPeripheral8(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);            \
        pScb->SysRunTimeSrvPrarm.Address8 = (param2);        \
        pScb->SysRunTimeSrvPrarm.Value8 = (param2);

#define OSError_Save_ReadPeripheral16(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);             \
        pScb->SysRunTimeSrvPrarm.Address16 = (param2);        \
        pScb->SysRunTimeSrvPrarm.Value16 = (param2);

#define OSError_Save_ReadPeripheral32(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);             \
        pScb->SysRunTimeSrvPrarm.Address32 = (param2);        \
        pScb->SysRunTimeSrvPrarm.Value32 = (param2);

#define OSError_Save_WritePeripheral8(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);             \
        pScb->SysRunTimeSrvPrarm.Address8 = (param2);         \
        pScb->SysRunTimeSrvPrarm.Value8 = (param2);

#define OSError_Save_WritePeripheral16(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);              \
        pScb->SysRunTimeSrvPrarm.Address16 = (param2);         \
        pScb->SysRunTimeSrvPrarm.Value16 = (param2);

#define OSError_Save_WritePeripheral32(param1, param2, param3) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);              \
        pScb->SysRunTimeSrvPrarm.Address32 = (param2);         \
        pScb->SysRunTimeSrvPrarm.Value32 = (param2);

#define OSError_Save_ModifyPeripheral8(param1, param2, param3, param4) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);                      \
        pScb->SysRunTimeSrvPrarm.Address8 = (param2);                  \
        pScb->SysRunTimeSrvPrarm.Clearmask8 = (param3);                \
        pScb->SysRunTimeSrvPrarm.Setmask8 = (param4);

#define OSError_Save_ModifyPeripheral16(param1, param2, param3, param4) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);                       \
        pScb->SysRunTimeSrvPrarm.Address16 = (param2);                  \
        pScb->SysRunTimeSrvPrarm.Clearmask16 = (param3);                \
        pScb->SysRunTimeSrvPrarm.Setmask16 = (param4);

#define OSError_Save_ModifyPeripheral32(param1, param2, param3, param4) \
        pScb->SysRunTimeSrvPrarm.Area = (param1);                       \
        pScb->SysRunTimeSrvPrarm.Address32 = (param2);                  \
        pScb->SysRunTimeSrvPrarm.Clearmask32 = (param3);                \
        pScb->SysRunTimeSrvPrarm.Setmask32 = (param4);

#define OSError_Save_GetScheduleTableStatus(param1, param2)  \
        pScb->SysRunTimeSrvPrarm.ScheduleTableId = (param1); \
        pScb->SysRunTimeSrvPrarm.ScheduleStatus = (param2);

#define OSError_Save_StartScheduleTableRel(param1, param2)   \
        pScb->SysRunTimeSrvPrarm.ScheduleTableId = (param1); \
        pScb->SysRunTimeSrvPrarm.Offset = (param2);

#define OSError_Save_StartScheduleTableAbs(param1, param2)   \
        pScb->SysRunTimeSrvPrarm.ScheduleTableId = (param1); \
        pScb->SysRunTimeSrvPrarm.Start = (param2);

#define OSError_Save_StopScheduleTable(param1) \
        pScb->SysRunTimeSrvPrarm.ScheduleTableId = (param1);

#define OSError_Save_NextScheduleTable(param1, param2)       \
        pScb->SysRunTimeSrvPrarm.ScheduleTableId = (param1); \
        pScb->SysRunTimeSrvPrarm.Start = (param2);

#define OSError_Save_StartScheduleTableSynchron(param1) \
        pScb->SysRunTimeSrvPrarm.ScheduleTableId = (param1);

#define OSError_Save_SyncScheduleTable(param1, param2)       \
        pScb->SysRunTimeSrvPrarm.ScheduleTableId = (param1); \
        pScb->SysRunTimeSrvPrarm.Value = (param2);

#define OSError_Save_SetScheduleTableAsync(param1) \
        pScb->SysRunTimeSrvPrarm.ScheduleTableId = (param1);

#define OSError_Save_GetSpinlock(param1) \
        pScb->SysRunTimeSrvPrarm.SpinlockId = (param1);

#define OSError_Save_ReleaseSpinlock(param1) \
        pScb->SysRunTimeSrvPrarm.SpinlockId = (param1);

#define OSError_Save_TryToGetSpinlock(param1, param2)   \
        pScb->SysRunTimeSrvPrarm.SpinlockId = (param1); \
        pScb->SysRunTimeSrvPrarm.Success = (param2);

#define OSError_Save_CallTrustedFunction(param1, param2)   \
        pScb->SysRunTimeSrvPrarm.FunctionIndex = (param1); \
        pScb->SysRunTimeSrvPrarm.FunctionParams = (param2);

#define OSError_Save_IocSend(param1, param2)          \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1); \
        pScb->SysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocWrite(param1, param2)         \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1); \
        pScb->SysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocSendGroup(param1, param2)          \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1); \
        pScb->SysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocWriteGroup(param1, param2)    \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1); \
        pScb->SysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocRead(param1, param2)          \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1); \
        pScb->SysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocReceive(param1, param2)       \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1); \
        pScb->SysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocReadGroup(param1, param2)     \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1); \
        pScb->SysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocReceiveGroup(param1, param2)  \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1); \
        pScb->SysRunTimeSrvPrarm.DataLenth = (param2);

#define OSError_Save_IocEmptyQueue(param1) \
        pScb->SysRunTimeSrvPrarm.IocValue = (param1);

#define OSError_Save_ShutDownOs(param1) \
        pScb->SysRunTimeSrvPrarm.ShutdownError = (param1);
/* PRQA S 3412 -- */
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
#define OSError_Save_StartScheduleTableAbs(param1, param2)
#define OSError_Save_StopScheduleTable(param1)
#define OSError_Save_NextScheduleTable(param1, param2)
#define OSError_Save_StartScheduleTableSynchron(param1)
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
#endif
#endif

/*=======[E N D   O F   F I L E]============================================*/
