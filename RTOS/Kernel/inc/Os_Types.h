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
 **  @file               : Os_Types.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for type definitions
 **
 ***********************************************************************************************************************/

#ifndef OS_TYPES_H
#define OS_TYPES_H

/* =================================================== inclusions =================================================== */
#include "Os_Macros.h"
#include "Os_ECode.h"
#include "Os_Cfg.h"
#include "Os_Arch_Define.h"
#include "Os_Mcu_Debug.h"

/* ================================================ type definitions ================================================ */
/**
 * @brief The types of system services.
 */
/* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    /*AUTOSAR Service*/
    OSServiceId_GetApplicationID = 0x00U,
    OSServiceId_GetISRID = 0x01U,
    OSServiceId_CallTrustedFunction = 0x02U,
    OSServiceId_CheckISRMemoryAccess = 0x03U,
    OSServiceId_CheckTaskMemoryAccess = 0x04U,
    OSServiceId_CheckObjectAccess = 0x05U,
    OSServiceId_CheckObjectOwnership = 0x06U,
    OSServiceId_StartScheduleTableRel = 0x07U,
    OSServiceId_StartScheduleTableAbs = 0x08U,
    OSServiceId_StopScheduleTable = 0x09U,
    OSServiceId_NextScheduleTable = 0x0aU,
    OSServiceId_StartScheduleTableSynchron = 0x0bU,
    OSServiceId_SyncScheduleTable = 0x0cU,
    OSServiceId_SetScheduleTableAsync = 0x0dU,
    OSServiceId_GetScheduleTableStatus = 0x0eU,
    OSServiceId_IncrementCounter = 0x0fU,
    OSServiceId_GetCounterValue = 0x10U,
    OSServiceId_GetElapsedValue = 0x11U,
    OSServiceId_TerminateApplication = 0x12U,
    OSServiceId_AllowAccess = 0x13U,
    OSServiceId_GetApplicationState = 0x14U,
    OSServiceId_GetNumberOfActivatedCores = 0x15U,
    OSServiceId_GetCoreID = 0x16U,
    OSServiceId_StartCore = 0x17U,
    OSServiceId_StartNonAutosarCore = 0x18U,
    OSServiceId_GetSpinlock = 0x19U,
    OSServiceId_ReleaseSpinlock = 0x1aU,
    OSServiceId_TryToGetSpinlock = 0x1bU,
    OSServiceId_ShutdownAllCores = 0x1cU,
    OSServiceId_ControlIdle = 0x1dU,
    OSServiceId_IocSend = 0x1eU,
    OSServiceId_IocWrite = 0x1fU,
    OSServiceId_IocSendGroup = 0x20U,
    OSServiceId_IocWriteGroup = 0x21U,
    OSServiceId_IocReceive = 0x22U,
    OSServiceId_IocRead = 0x23U,
    OSServiceId_IocReceiveGroup = 0x24U,
    OSServiceId_IocReadGroup = 0x25U,
    OSServiceId_IocEmptyQueue = 0x26U,
    OSServiceId_GetCurrentApplicationID = 0x27U,
    OSServiceId_ReadPeripheral8 = 0x28U,
    OSServiceId_ReadPeripheral16 = 0x29U,
    OSServiceId_ReadPeripheral32 = 0x2aU,
    OSServiceId_WritePeripheral8 = 0x2bU,
    OSServiceId_WritePeripheral16 = 0x2cU,
    OSServiceId_WritePeripheral32 = 0x2dU,
    OSServiceId_ModifyPeripheral8 = 0x2eU,
    OSServiceId_ModifyPeripheral32 = 0x2fU,
    OSServiceId_DisableInterruptSource = 0x30U,
    OSServiceId_EnableInterruptSource = 0x31U,
    OSServiceId_ClearPendingInterrupt = 0x32U,
    OSServiceId_ActivateTaskAsyn = 0x33U,
    OSServiceId_SetEventAsyn = 0x34U,
    OSServiceId_ModifyPeripheral16 = 0x35U,

    /** Additional Service */
    OSServiceId_WaitAllEvents = 0xD7U,
    OSServiceId_IocCallBackNotify = 0xDEU,

    /** OSEK Service */
    OSServiceId_ActivateTask = 0xDFU,
    OSServiceId_TerminateTask = 0xE0U,
    OSServiceId_ChainTask = 0xE1U,
    OSServiceId_Schedule = 0xE2U,
    OSServiceId_GetTaskID = 0xE3U,
    OSServiceId_GetTaskState = 0xE4U,
    OSServiceId_EnableAllInterrupts = 0xE5U,
    OSServiceId_DisableAllInterrupts = 0xE6U,
    OSServiceId_ResumeAllInterrupts = 0xE7U,
    OSServiceId_SuspendAllInterrupts = 0xE8U,
    OSServiceId_ResumeOSInterrupts = 0xE9U,
    OSServiceId_SuspendOSInterrupts = 0xEAU,
    OSServiceId_DeclareResource = 0xEBU,
    OSServiceId_GetResource = 0xECU,
    OSServiceId_ReleaseResource = 0xEDU,
    OSServiceId_DeclareEvent = 0xEEU,
    OSServiceId_SetEvent = 0xEFU,
    OSServiceId_ClearEvent = 0xF0U,
    OSServiceId_GetEvent = 0xF1U,
    OSServiceId_WaitEvent = 0xF2U,
    OSServiceId_GetAlarmBase = 0xF3U,
    OSServiceId_GetAlarm = 0xF4U,
    OSServiceId_SetRelAlarm = 0xF5U,
    OSServiceId_SetAbsAlarm = 0xF6U,
    OSServiceId_CancelAlarm = 0xF7U,
    OSServiceId_GetActiveApplicationMode = 0xF8U,
    OSServiceId_StartOS = 0xF9U,
    OSServiceId_ShutdownOS = 0xFAU,
    OSServiceId_ErrorHook = 0xFBU,
    OSServiceId_PreTaskHook = 0xFCU,
    OSServiceId_PostTaskHook = 0xFDU,
    OSServiceId_StartupHook = 0xFEU,
    OSServiceId_ShutdownHook = 0xFFU
} Os_ServiceIdType;

typedef enum
{
    /*AUTOSAR Service*/
    OSApiId_GetApplicationID = 0x02U,
    OSApiId_GetISRID = 0x04U,
    OSApiId_CallTrustedFunction = 0x06U,
    OSApiId_CheckISRMemoryAccess = 0x08U,
    OSApiId_CheckTaskMemoryAccess = 0x0aU,
    OSApiId_CheckObjectAccess = 0x0cU,
    OSApiId_CheckObjectOwnership = 0x0EU,
    OSApiId_StartScheduleTableRel = 0x10U,
    OSApiId_StartScheduleTableAbs = 0x12U,
    OSApiId_StopScheduleTable = 0x14U,
    OSApiId_NextScheduleTable = 0x16U,
    OSApiId_StartScheduleTableSynchron = 0x18U,
    OSApiId_SyncScheduleTable = 0x1AU,
    OSApiId_SetScheduleTableAsync = 0x1CU,
    OSApiId_GetScheduleTableStatus = 0x1EU,
    OSApiId_IncrementCounter = 0x20U,
    OSApiId_GetCounterValue = 0x22U,
    OSApiId_GetElapsedValue = 0x24U,
    OSApiId_TerminateApplication = 0x26U,
    OSApiId_AllowAccess = 0x28U,
    OSApiId_GetApplicationState = 0x2AU,
    OSApiId_GetNumberOfActivatedCores = 0x2CU,
    OSApiId_GetCoreID = 0x2EU,
    OSApiId_StartCore = 0x30U,
    OSApiId_StartNonAutosarCore = 0x32U,
    OSApiId_GetSpinlock = 0x34U,
    OSApiId_ReleaseSpinlock = 0x36U,
    OSApiId_TryToGetSpinlock = 0x38U,
    OSApiId_ShutdownAllCores = 0x3AU,
    OSApiId_ControlIdle = 0x3CU,
    OSApiId_IocSend = 0x3EU,
    OSApiId_IocWrite = 0x40U,
    OSApiId_IocSendGroup = 0x42U,
    OSApiId_IocWriteGroup = 0x44U,
    OSApiId_IocReceive = 0x46U,
    OSApiId_IocRead = 0x48U,
    OSApiId_IocReceiveGroup = 0x4AU,
    OSApiId_IocReadGroup = 0x4CU,
    OSApiId_IocEmptyQueue = 0x4EU,
    OSApiId_GetCurrentApplicationID = 0x50U,
    OSApiId_ReadPeripheral8 = 0x52U,
    OSApiId_ReadPeripheral16 = 0x54U,
    OSApiId_ReadPeripheral32 = 0x56U,
    OSApiId_WritePeripheral8 = 0x58U,
    OSApiId_WritePeripheral16 = 0x5AU,
    OSApiId_WritePeripheral32 = 0x5CU,
    OSApiId_ModifyPeripheral8 = 0x5EU,
    OSApiId_ModifyPeripheral32 = 0x60U,
    OSApiId_DisableInterruptSource = 0x62U,
    OSApiId_EnableInterruptSource = 0x64U,
    OSApiId_ClearPendingInterrupt = 0x66U,
    OSApiId_ActivateTaskAsyn = 0x68U,
    OSApiId_SetEventAsyn = 0x6AU,
    OSApiId_ModifyPeripheral16 = 0x6CU,
    /** Additional Service */
    OSApiId_WaitAllEvents = 0x6EU,
    OSApiId_IocCallBackNotify = 0x70U,
    /** OSEK Service */
    OSApiId_ActivateTask = 0x72U,
    OSApiId_TerminateTask = 0x74U,
    OSApiId_ChainTask = 0x76U,
    OSApiId_Schedule = 0x78U,
    OSApiId_GetTaskID = 0x7AU,
    OSApiId_GetTaskState = 0x7CU,
    OSApiId_EnableAllInterrupts = 0x7EU,
    OSApiId_DisableAllInterrupts = 0x80U,
    OSApiId_ResumeAllInterrupts = 0x82U,
    OSApiId_SuspendAllInterrupts = 0x84U,
    OSApiId_ResumeOSInterrupts = 0x86U,
    OSApiId_SuspendOSInterrupts = 0x88U,
    OSApiId_GetResource = 0x8AU,
    OSApiId_ReleaseResource = 0x8CU,
    OSApiId_SetEvent = 0x8EU,
    OSApiId_ClearEvent = 0x90U,
    OSApiId_GetEvent = 0x92U,
    OSApiId_WaitEvent = 0x94U,
    OSApiId_GetAlarmBase = 0x96U,
    OSApiId_GetAlarm = 0x98U,
    OSApiId_SetRelAlarm = 0x9AU,
    OSApiId_SetAbsAlarm = 0x9CU,
    OSApiId_CancelAlarm = 0x9EU,
    OSApiId_GetActiveApplicationMode = 0xA0U,
    OSApiId_StartOS = 0xA2U,
    OSApiId_ShutdownOS = 0xA4U,
    OSApiId_BarrierSynchronize = 0xA6U
} Os_TraceApiIdType;
/* PRQA S 1271 -- */

/**
 * @brief Definition of Os level of call API, add some for service protection in SC3 and SC4.
 */
typedef enum
{
    OS_LEVEL_MAIN = 0,
    OS_LEVEL_PROTECTIONHOOK = 1,
    OS_LEVEL_ALARMCALLBACK = 2,
    OS_LEVEL_SHUTDOWNHOOK = 3,
    OS_LEVEL_STARTUPHOOK = 4,
    OS_LEVEL_POSTTASKHOOK = 5,
    OS_LEVEL_PRETASKHOOK = 6,
    OS_LEVEL_ERRORHOOK = 7,
    OS_LEVEL_ISR2 = 8,
    OS_LEVEL_ISR1 = 9,
    OS_LEVEL_TASK = 10,
    OS_LEVEL_ERRORHOOK_APP = 11,
    OS_LEVEL_STANDARD_RESOURCE = 12,
    OS_LEVEL_INTERNAL_RESOURCE = 13
} Os_CallLevelType;

/**
 * @brief Type definition of the App mode.
 */
typedef uint16 Os_AppModeType;
typedef Os_AppModeType AppModeType;

/**
 * @brief Type definition of the sched lock.
 */
typedef uint8 Os_LockerType;

/**
 * @brief Defines stack data types.
 */
typedef uint32 Os_StackDataType;

/**
 * @brief Pointer to stack data.
 */
typedef uint32 *Os_StackPtrType;

/**
 * @brief The structure type of the stack.
 */
typedef struct
{
    Os_StackPtrType StackTop;
    Os_StackPtrType StackBottom;
} Os_StackType;

/**
 * @brief This data type identifies an object.
 */
typedef enum
{
    OS_OBJECT_TASK = 0,
    OS_OBJECT_ISR = 1,
    OS_OBJECT_ALARM = 2,
    OS_OBJECT_COUNTER = 3,
    OS_OBJECT_SCHEDULETABLE = 4,
    OS_OBJECT_APP = 5,

    OS_OBJECT_RESOURCE = 6,
    OS_OBJECT_SPINLOCK = 7,
    OS_OBJECT_MAX = 8, /** The Max Value is invalid, just used in coding */
    /* PRQA S 1271 ++ */ /* VL_Os_1271 */
    OS_OBJECT_INVALID = 0xFFU /** The invalid object type */
    /* PRQA S 1271 -- */
} Os_ObjectTypeType;

/**
 * @brief The type definition of the pointer to the object type.
 */
typedef Os_ObjectTypeType ObjectTypeType;

/**
 * @brief The type definition of the core id.
 */
typedef uint16 Os_CoreIdType;

/**
 * @brief The pointer type definition of the core id.
 */
typedef Os_CoreIdType *Os_CoreIdRefType;

/**
 * @brief CoreIdType is a scalar that allows identifying a single core.
The CoreIdType shall represent the logical CoreID.
 */
typedef Os_CoreIdType CoreIdType;
typedef Os_CoreIdType *CoreIdRefType;

/**
 * @brief The type definition of the Application.
 */
typedef uint16 Os_ApplicationType;
typedef Os_ApplicationType ApplicationType;

/**
 * @brief Type definition of the shutdown action.
 */
typedef enum
{
    OS_SHUTDOWN_OS = 0,
    OS_SHUTDOWN_ALL_OS = 1
} Os_ShutdownAction;

/* -----------------------------task---------------------- */
/**
 * @brief This enum type identifies Task State.
 */
typedef enum
{
    OS_TASK_STATE_WAITING = 0,
    OS_TASK_STATE_READY = 1,
    OS_TASK_STATE_SUSPENDED = 2,
    OS_TASK_STATE_RUNNING = 3,
    /** for distinguish the autostarted task,the first activatedtask and preempted task */
    OS_TASK_STATE_START = 4
} Os_TaskStateType;

/**
 * @brief Task status type.
 */
typedef Os_TaskStateType TaskStateType;
typedef Os_TaskStateType *TaskStateRefType;
typedef Os_TaskStateType *Os_TaskStateRefType;

/**
 * @brief Task type.
 */
typedef Os_TaskType *Os_TaskRefType;
typedef Os_TaskType TaskType;
typedef Os_TaskType *TaskRefType;

/**
 * @brief Types of task scheduling.
 */
typedef uint8 Os_TaskScheduleType;

/**
 * @brief Types of task priority.
 */
typedef uint16 Os_PriorityType;

/**
 * @brief The entry function of the task.
 */
typedef void (*Os_TaskEntry)(void);

/**
 * @brief Define the structure of the app attribute configuration of the object.
 */
typedef struct
{
    const ApplicationType* AccAppRef; /* Access App reference bitmap list */
    ApplicationType        HostApp;          /* owner app of the object */
    ApplicationType        AccAppRefNodeCnt; /* AccAppRef array node count */
} Os_ObjectAppCfgType;

/* ------------------------Alarm------------------------- */
/**
 * @brief This type of tick.
 */
typedef uint32       Os_TickType;
typedef Os_TickType* Os_TickRefType;
typedef Os_TickType  TickType;
typedef TickType *   TickRefType;

#if (CFG_ALARM_MAX > 0)
/**
 * @brief The Auto-Start type of Alarm is divided into relative start and absolute start.
 */
typedef enum
{
    OS_ALARM_AUTOSTART_ABSOLUTE = 0,
    OS_ALARM_AUTOSTART_RELATIVE = 1
} Os_AlarmAutostartType;

/**
 * @brief This type of Alarm Id.
 */
typedef Os_AlarmType* Os_AlarmRefType;
typedef Os_AlarmType  AlarmType;
typedef Os_AlarmType* AlarmRefType;

/**
 * @brief Alarm callback function type.
 */
typedef void (*Os_AlarmCallbackType)(void);

/**
 * @brief The base data type structure of Alarm.
 */
typedef struct
{
    Os_TickType MaxAllowedValue;
    Os_TickType TicksPerBase;
    Os_TickType MinCycle;
} Os_AlarmBaseType;

/**
 * @brief Alarm base type.
 */
typedef Os_AlarmBaseType  AlarmBaseType;
typedef Os_AlarmBaseType* Os_AlarmBaseRefType;
typedef Os_AlarmBaseType* AlarmBaseRefType;

/**
 * @brief The configuration type for Alarm auto-start.
 */
typedef struct
{
    Os_TickType AlarmStartTime;
    Os_TickType AlarmCycleTime;
    Os_AppModeType AlarmAppMode;
    Os_AlarmAutostartType AlarmAutostartType;
} Os_AlarmAutostartCfgType;

/**
 * @brief All configuration types for Alarm.
 */
typedef struct
{
    const Os_AlarmAutostartCfgType *AlarmAutostartRef;
    Os_AlarmCallbackType AlarmCallback;

#if (CFG_OSAPPLICATION_MAX > 0U)
    const Os_ObjectAppCfgType *ObjAppCfg;
#endif
    Os_CoreIdType HostCore; /** owner core of the alarm */
    Os_CounterType AlarmCounter;
} Os_AlarmCfgType;

/**
 * @brief This type of Alarm control block.
 */
typedef struct
{
    Os_TickType  AlarmStart;
    Os_TickType  AlarmCycle;
    Os_AlarmType AlarmNext;
} Os_ACBType;

/**
 * @brief The ID range type for Alarm.
 */
typedef struct
{
    Os_AlarmType Start;
    Os_AlarmType End;
} Os_AlarmIndexType;
#endif

/**
 * @brief This data type identifies if an OS-Application has access to an object.
 */
typedef enum
{
    OS_ACCESS = 0,
    OS_NO_ACCESS = 1
} Os_ObjectAccessType;

/**
 * @brief The external type of Os_ObjectAccessType.
 */
typedef Os_ObjectAccessType ObjectAccessType;

/**
 * @brief This data type defines the use of a Restart Task after terminating an OSApplication.
 */
typedef enum
{
    OS_RESTART = 0,
    OS_NO_RESTART = 1
} Os_RestartType;

/**
 * @brief The external type of Os_RestartType.
 */
typedef Os_RestartType RestartType;

/**
 * @brief This data type identifies the state of an OS-Application.
 */
typedef enum
{
    OS_APPLICATION_ACCESSIBLE = 0,
    OS_APPLICATION_RESTARTING = 1,
    OS_APPLICATION_TERMINATED = 2
} Os_ApplicationStateType;

/**
 * @brief The type definition of the pointer to the Application state.
 */
typedef Os_ApplicationStateType *Os_ApplicationStateRefType;
typedef Os_ApplicationStateType ApplicationStateType;

/**
 * @brief This data type points to location where a ApplicationStateType can be stored.
 */
typedef Os_ApplicationStateType *ApplicationStateRefType;

/**
 * @brief The ID type definition of the Application object.
 */
typedef uint16 Os_AppObjectIdType;

/**
 * @brief The type definition of the Application object.
 */
typedef uint16             Os_AppObjectType;
typedef Os_AppObjectType * Os_AppObjectRefType;

/**
 * @brief Type definition of Appl error hook.
 */
typedef void (*Os_ApplErrorHookType)(StatusType error);

/**
 * @brief Type definition of shutdown hook.
 */
typedef void (*Os_ApplShutdownHookType)(StatusType fatalerror);

/**
 * @brief Appl Start Hook Type Definition.
 */
typedef void (*Os_ApplStartupHookType)(void);

/**
 * @brief The control block type definition for the Application.
 */
typedef struct
{
    Os_ApplicationStateType AppState;
} Os_AppCBType;

/**
 * @brief Type definition for Appl hook configuration.
 *        If not cfg a hook, then the value is NULL_PTR.From the specfication, the non-trusted_app can not call ShutdownOS,
 *        so that, non-trusted_app can not cfg specific-ShutdownHook.
 */
typedef struct
{
    Os_ApplErrorHookType    AppErrorHook;
    Os_ApplShutdownHookType AppShutdownHook;
    Os_ApplStartupHookType  AppStartupHook;
} Os_AppHookCfgType;

/**
 * @brief This data type identifies objectId.
 */
typedef uint16 Os_AppObjectId;
typedef Os_AppObjectId AppObjectId;

#if (CFG_OSAPPLICATION_MAX > 0U)
/**
 * @brief The ID range type for application.
 */
typedef struct
{
    Os_AppType start;
    Os_AppType end;
} Os_AppIdRangeType;
#endif

/**
 * @brief This data type identifies the idle mode behavior.
 */
typedef enum
{
    OS_IDLE_NO_HALT = 0,
    OS_RUN = 1,
    OS_CORE_PRE_SHUTDOWN = 2,
    OS_CORE_STATUS_INVALID = 3
} Os_IdleModeType;

/**
 * @brief This data type identifies the idle mode behavior.
 */
typedef Os_IdleModeType IdleModeType;

/**
 * @brief Type definition of the CPU mode.
 */
typedef enum
{
    OS_CPUMODE_USER0 = 0,
    OS_CPUMODE_USER1 = 1,
    OS_CPUMODE_SUPERVISOR = 2
} Os_CpuModeType;

/**
 * @brief Define the base structure of the memorysize.
 */
typedef struct
{
    Os_CoreIdType   CoreStateActive;
    Os_CoreIdType   CoreStateSynPoint[SYNC_POINTS_MAX][OS_AUTOSAR_CORES];
    Os_AppModeType  AppMode[OS_AUTOSAR_CORES];
    Os_IdleModeType CoreStatus[OS_AUTOSAR_CORES]; /* PRQA S 1039 */ /* VL_Os_1039 */
} Os_CoreCBType;

#if (CFG_COUNTER_MAX > 0U)
/**
 * @brief This type of Counter type.
 */
typedef enum
{
    OS_COUNTER_HARDWARE = 0,
    OS_COUNTER_SOFTWARE = 1
} Os_CounterPropertyType;


/**
 * @brief This data type identifies a counter.
 */
typedef Os_CounterType CounterType;

/**
 * @brief This type of Counter control block.
 */
typedef struct
{
    /** Add: AutoSar SC2: ScheduleTable list head node index reference to this counter. */
#if (CFG_SCHEDTBL_MAX > 0U)
    struct Os_TagSTCBType *CounterStListHead;
#endif
    Os_TickType CounterCurVal;
    Os_TickType CounterLastVal;
#if (CFG_ALARM_MAX > 0)
    Os_AlarmType CounterAlmQue;
#endif
} Os_CCBType;

/**
 * @brief This type of Counter Configuration.
 */
typedef struct
{
#if (CFG_OSAPPLICATION_MAX > 0U)
    const Os_ObjectAppCfgType *ObjAppCfg;
#endif
    Os_TickType CounterMaxAllowedValue;
    Os_TickType CounterMinCycle;
    Os_TickType CounterTicksPerBase;
    /** To check the correctness with the configuration */
    Os_TickType SecondsPerTick;
    Os_CoreIdType HostCore; /** owner core of the task */
    Os_CounterPropertyType CounterProperty;
} Os_CounterCfgType;

/**
 * @brief The ID range type for counter.
 */
typedef Os_CounterCfgType *Os_CounterCfgRefType;

typedef struct
{
    Os_CounterType start;
    Os_CounterType end;
}Os_CounterRangeType;
#endif

/**
 * @brief This type of event.
 */
typedef uint64 Os_EventType;
typedef Os_EventType EventType;

/**
 * @brief This type of event mask.
 */
typedef uint64 Os_EventMaskType;
typedef Os_EventMaskType *Os_EventMaskRefType;
typedef Os_EventMaskType EventMaskType;
typedef EventMaskType *EventMaskRefType;

/**
 * @brief This struct type identifies the Event control block.
 */
typedef struct
{
    Os_EventMaskType SetEventMask;
    Os_EventMaskType WaitEventMask;
    boolean IsWaitAllEvents;
} Os_ECBType;

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))

/* Add: AutoSar SC2: Time protection operation type definitions. */
/* Timing protection operation type definition. */
typedef enum
{
    TP_EXE         = 0,
    TP_SUS_OS_INT  = 1,
    TP_SUS_ALL_INT = 2,
    TP_DIS_ALL_INT = 3,
    TP_ARRIVAL     = 4,
    TP_RESOURCE    = 5
} Os_TmProtOptType;

/* Timing protection Monitord Thread type definition. */
typedef enum
{
    TP_FOR_TASK = 0,
    TP_FOR_ISR  = 1,
    TP_FOR_NONE = 2
} Os_TpMonitoredType;

/* Thread timing protection status type definition. */
typedef enum
{
    TP_NO_INIT = 0,
    TP_READY   = 1,
    TP_WORKING = 2,
    TP_SUSPEND = 3
} Os_TpStatus;

/* Define resource for task/ISR2 locking */
typedef struct
{
    uint32          OsResLockBudget;
#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType OsResLockResRef;
#endif
} Os_TpResLockType;

typedef struct
{
    const Os_TpResLockType* osResLockRef;
    uint32 osAllInterruptLockBudget;
    uint32 osExecutionBudget;
    uint32 osOsInterruptLockBudget;
    uint32 osTimeFrame;
    uint16 osResLockCnt;
} Os_TmProtCfgType;

/* Timing protection control block data definition. */
typedef struct Os_TmProtpNode
{
    
    uint64                 TpEndTime;
    struct Os_TmProtpNode* Pre;
    struct Os_TmProtpNode* Next;
    uint32                 osTpBudget;
    boolean                TpNodeInList;
    Os_TmProtOptType       TpNodeOpt;
} Os_TpNodeType;

/* Timing protection Monitor type definition. */
typedef struct
{
    uint64             Os_TpTimeStamp;
    Os_TpMonitoredType Os_TpMonitored;
} Os_TpMonitorType;

#endif

#endif

/**
 * @brief Definition of OS interrupt category.
 */
typedef enum
{
    OS_ISR_CATEGORY1 = 0,
    OS_ISR_CATEGORY2 = 1
} Os_IsrCategoryType;

/**
 * @brief Definition of OS interrupt state.
 */
typedef enum
{
    OS_ISR_DISABLED = 0,
    OS_ISR_ENABLED = 1
} Os_IsrStateType;

/**
 * @brief Interrupt priority
 */
typedef uint16 Os_IPLType;

/**
 * @brief This data type identifies an interrupt service routine (ISR).
 */
typedef Os_IsrType ISRType;

/**
 * @brief Describe the status of the interrupt.
 */
typedef enum
{
    OS_ISR_ENABLE = 0,
    OS_ISR_DISABLE = 1
} Os_IsrDescriptionType;

/**
 * @brief Type definition of interrupt control block.
 */
typedef struct
{
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    uint64                  isrTpTimeStamp;
    uint64                  isrTpLastReadTime;
    uint64                  isrTpEerlistArrival;
    Os_TpNodeType*          isrTpHead;
    Os_TpNodeType           isrTpNode[TP_RESOURCE];
#endif

#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
    uint32 isrScheduleCount;
#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType *IsrC2ResourceStack; /** AutoSar: SC2 protection hook. */
    Os_ResourceType  IsrC2ResCount;
#endif
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType CallBackAppID;
#endif
/** multi-core */
#if ((CFG_SPINLOCK_MAX > 0U) || (CFG_STD_RESOURCE_MAX > 0U))
    uint16 Isr2CriticalZoneCount;
    uint16 IsrCurrentSpinlockOccupyLevel;
    uint16 Isr2CriticalZoneStack[CFG_CRITICAL_ZONE_MAX];
    /* PRQA S 1039 ++ */ /* VL_Os_1039 */
    Os_ObjectTypeType Isr2CriticalZoneType[CFG_CRITICAL_ZONE_MAX];
    /* PRQA S 1039 -- */
#endif

/* Autosar: for service protection*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    uint8 IsrC2DisableAllCount;
    uint8 IsrC2SuspendAllCount;
    uint8 IsrC2SuspendOSCount;
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TpStatus isrTpStatus;
#endif

#if (TRUE != CFG_TIMING_PROTECTION_ENABLE) && (CFG_STD_RESOURCE_MAX == 0U) &&\
    ((OS_SC1 == CFG_SC) || (OS_SC2 == CFG_SC)) && (TRUE != CFG_SCHEDULE_COUNT_MONITOR) &&\
    (CFG_SPINLOCK_MAX == 0U)
    uint8 rsv;  /* Aovid the Os_ICBType to be empty */
#endif
} Os_ICBType;

/**
 * @brief Type definition of interrupt configuration.
 */
typedef struct
{
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    const Os_TmProtCfgType *isrTmProtCfgRef;
#endif

    uint32 IsrSrc;     /** Isr source register. Used in TerminateApplication. */
    uint32 IsrSrcType; /** Isr_src type, CPU0/CPU1/CPU2/DMA. */
#if (CFG_OSAPPLICATION_MAX > 0U)
    const Os_ObjectAppCfgType *ObjAppCfg;
#endif
    Os_CoreIdType HostCore; /** owner core of the task */
    Os_IsrCategoryType IsrCatType;
    boolean NestedEnable;
} Os_IsrCfgType;

/**
 * @brief The ID range type for Isr.
 */
typedef struct
{
    Os_IsrType IsrStart;
    Os_IsrType IsrEnd;
} Os_IsrRangeType;

/**
 * @brief The attribute type of the Isr.
 */
typedef struct
{
    Os_IsrRangeType AllIsr;
    Os_IsrRangeType Isr1;
    Os_IsrRangeType Isr2;
} Os_IsrPropertyType;


/**
 * @brief App address start and end.
 */
typedef struct
{
    uint8 *AppAddrStart;
    uint8 *AppAddrEnd;
} Os_AppAddrType;

/**
 * @brief ISR address start and end.
 */
typedef struct
{
    uint8 *IsrAddrStart;
    uint8 *IsrAddrEnd;
} Os_IsrAddrType;

/**
 * @brief Task address start and end.
 */
typedef struct
{
    uint8 *TaskAddrStart;
    uint8 *TaskAddrEnd;
} Os_TaskAddrType;

/**
 * @brief RoData address start and end.
 */
typedef struct
{
    uint8 *RodataAddrStart;
    uint8 *RodataAddrEnd;
} Os_RodataAddrType;

/**
 * @brief peripheral access type definition.
 */
typedef uint16 AreaIdType;

/**
 * @brief peripheral configuration type definition.
 */
typedef struct
{
    uint32 PeriAreaStartAddr;
    uint32 PeriAreaEndAddr;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    const ApplicationType* AccAppRef; /* Access App reference bitmap list */
#endif
} Os_PeripheralAreaCfgType;

/**
 * @brief SWS_Os_00787.
 */
typedef enum
{
    PRO_IGNORE = 0,
    PRO_TERMINATETASKISR = 1,
    PRO_TERMINATEAPPL = 2,
    PRO_TERMINATEAPPL_RESTART = 3,
    PRO_SHUTDOWN = 4,
    PRO_RESUME = 5,
} Os_ProtectionReturnType;

/**
 * @brief This data type identifies a value which controls further actions of the OS on
          return from the protection hook.
 */
typedef Os_ProtectionReturnType ProtectionReturnType;

/**
 * @brief The data type of the ready queue.
 */
typedef struct
{
    uint32 QueueHead;
    uint32 QueueTail;
} Os_ReadyQueueType;

/**
 * @brief The manager type of the ready queue.
 */
typedef struct
{
    Os_ReadyQueueType *Mark;
    const uint32 *QueueSize;
    const Os_TaskRefType *Queue;
    Os_PriorityType *Map;
    uint8 PriorityMax;
} Os_ReadyQueueManageType;

#if (CFG_STD_RESOURCE_MAX > 0U)
/**
 * @brief Type definition of OS resources.
 */
typedef Os_ResourceType ResourceType;

/**
 * @brief Definition of resource occupancy type.
 */
typedef enum
{
    OS_RES_OCCUPIED_BY_TASK = 0,              /*occupied by task*/
    OS_RES_OCCUPIED_BY_INTERRUPT = 1,         /*occupied by interrupt*/
    OS_RES_OCCUPIED_BY_TASK_OR_INTERRUPT = 2 /*occupied by task or interrupt*/
} Os_ResourceOccupyType;

/**
 * @brief Type definition of resource control block.
 */
typedef struct
{
    /** ResourceLockBudget for task and isr. Each resource has Os_TmProtCbDataDef.
        When it is locked, it should be processed by timing protection. */
#if (TRUE == CFG_RTI_ENABLE)
    uint32 ResLockerId;
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_TASK_MAX > 0)
    uint32* TmProtResBgtTask;
#endif
#if (CFG_ISR2_MAX > 0)
    uint32* TmProtResBgtIsr;
#endif
    Os_TpNodeType osResTpNode;
    /** Which task lock this resource. */
    Os_IsrType WhichIsrOccupy;
    Os_TaskType WhichTaskOccupy;
#endif
    Os_PriorityType SavePrio;
    uint8           SaveCount;
    Os_CallLevelType SaveLevel;
} Os_RCBType;

/**
 * @brief Definition of resource configuration type.
 */
typedef struct
{
#if (CFG_OSAPPLICATION_MAX > 0U)
    const Os_ObjectAppCfgType *ObjAppCfg;
#endif
    Os_CoreIdType HostCore; /** owner core of the resource */
    Os_PriorityType Ceiling;
    Os_ResourceOccupyType ResourceOccupyType;
} Os_ResourceCfgType;

/**
 * @brief The ID range type for Resource.
 */
typedef struct
{
    Os_ResourceType StdResStart;
    Os_ResourceType StdResEnd;
} Os_ResourceRangeType;
#endif

#if (CFG_SCHEDTBL_MAX > 0U)
/* ================================================ type definitions ================================================ */
/**
 * @brief This type describes the status of a schedule. The status can be one of the following:
          1. The schedule table is not started (SCHEDULETABLE_STOPPED)
          2. The schedule table will be started after the end of currently running schedule
             table (schedule table was used in NextScheduleTable() service) (SCHEDULETABLE_NEXT)
          3. The schedule table uses explicit synchronization, has been started and is
             waiting for the global time. (SCHEDULETABLE_WAITING)
          4. The schedule table is running, but is currently not synchronous to a global
             time source (SCHEDULETABLE_RUNNING)
          5. The schedule table is running and is synchronous to a global time source
             (SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS)
 */
typedef enum
{
    SCHEDULETABLE_STOPPED = 0,
    SCHEDULETABLE_NEXT = 1,
    SCHEDULETABLE_WAITING = 2,
    SCHEDULETABLE_RUNNING = 3,
    SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS = 4
} Os_SchedTblStateType;

/**
 * @brief Point to Os_SchedTblStateType.
 */
typedef Os_SchedTblStateType ScheduleTableStatusType;

/**
 * @brief This data type points to a variable of the data type ScheduleTableStatusType.
 */
typedef ScheduleTableStatusType *ScheduleTableStatusRefType;

/**
 * @brief Schedule table auto-start type.
 */
/* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    OS_ST_START_ABSOLUTE = 0U,
    OS_ST_START_RELATIVE = 1U,

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    OS_ST_START_SYNCHRON = 2U
#endif
} Os_SchedTblAutostartType;
/* PRQA S 1271 -- */

/**
 * @brief This data type identifies a schedule table.
 */
typedef Os_ScheduleTableType ScheduleTableType;
typedef uint16 Os_SchedTblSetEvent;
typedef Os_SchedTblSetEvent *Os_SchedTblSetEventRef;
typedef uint16 Os_SchedTblSetEventTask;
typedef Os_SchedTblSetEventTask *Os_SchedTblSetEventTaskRef;
typedef uint16 Os_SchedTblActivateTask;
typedef Os_SchedTblActivateTask *Os_SchedTblActivateTaskRef;

/**
 * @brief This type of Counter.
 */
typedef Os_CounterType Os_SchedTblCounterRefType;

/**
 * @brief This struct type identifies a schedule tableAutostart.
 */
typedef struct
{
    Os_TickType SchedTblAbsValue;
    Os_TickType SchedTblRelOffset;
    /** Note: In AUTOSAR_SWS_OS.pdf 10.2.23, Os_SchedTblAutostart can reference more then one appmode. */
    Os_AppModeType SchedTblAppModeBitmap;
    Os_SchedTblAutostartType SchedTblAutostartType;
} Os_SchedTblAutostart;

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
/**
 * @brief Enumeration type of synchronization policy.
 */
/* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    OS_ST_SYNC_EXPLICIT = 0U,
    OS_ST_SYNC_IMPLICIT = 1U,
    OS_ST_SYNC_NONE = 2U
} Os_SchedTblSyncStrategy;
/* PRQA S 1271 -- */

/**
 * @brief Schedule table synchronization type.
 */
typedef struct
{
    uint16 SchedTblExplicitPrecision;
    Os_SchedTblSyncStrategy SchedTblSyncStrategy;
} Os_SchedTblSync;
#endif

/**
 * @brief This struct type identifies the reference event to set.
 */
typedef struct
{
    Os_EventType SchedTblSetEventRef;
    Os_TaskType SchedTblSetEventTaskRef;
} Os_EPSetEventAction;

/**
 * @brief This struct type identifies the reference task to activate.
 */
typedef struct
{
    Os_TaskType SchedTblActivateTaskRef;
} Os_EPActivateTaskAction;

/**
 * @brief This struct type identifies a schedule tableExpriyPoint.
 */
typedef struct
{
    const Os_EPSetEventAction *SetEventList;
    const Os_EPActivateTaskAction *ActivateTaskList;
    Os_TickType SchedTblEPOffset;
    uint16 SetEventListSize;
    uint16 ActivateTaskListSize;
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_TickType SchedTblMaxAdvance;
    Os_TickType SchedTblMaxRetard;
#endif
} Os_SchedTblEP;

/**
 * @brief This struct type identifies a schedule table config.
 */
typedef struct
{
#if (CFG_OSAPPLICATION_MAX > 0U)
    const Os_ObjectAppCfgType *ObjAppCfg;
#endif
    const Os_SchedTblAutostart *SchedTblAutoStartRef;
    const Os_SchedTblEP *SchedTblEP;
    Os_TickType SchedTblDuration;
    uint16 SchedTblEPsize;
    Os_CounterType SchedTblCounterRef;

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_SchedTblSync SchedTblSync;
#endif
    Os_CoreIdType HostCore; /** owner core of the scheduletable */
    boolean SchedTblRepeating;
} Os_SchedTblCfgType;

/**
 * @brief This struct type identifies a schedule tablecontrol block.
 */
typedef struct Os_TagSTCBType
{
    struct Os_TagSTCBType *SchedTblNextNode; /* For ScheduleTable list. */
    struct Os_TagSTCBType *SchedTblPreNode;  /* For ScheduleTable list. */
    const Os_SchedTblEP *SchedTblNextEP;
    /* Different between driver counter and sync counter. */
    Os_TickType SchedTblDiff;
    /* Deviation, might be smaller then SchedTblDiff. */
    Os_TickType SchedTblDev;
    Os_TickType SchedTblDelay;               /* Delay of SchedTblNextEP. */
    Os_TickType SchedTblAdjustDevTotal;      /* Total dev during sync process. */
    Os_TickType SchedTblStartAbsTick;
    Os_TickType SchedTblNextEpAbsTick;       /* EP abs tick after adjust. */
    Os_TickType SchedTblFinalDelay;

    Os_ScheduleTableType SchedTblId;
    Os_ScheduleTableType NextSchedTblId;     /* Used for next state. */
    Os_ScheduleTableType PrevSchedTblId;     /* Used for next state. */
    uint16 SchedTblEpId;                     /* ID of SchedTblNextEP. */
    Os_SchedTblStateType SchedTblState;

    boolean SchedTblIsAdjust;                /* SchedTblNextEP need adjust or not. */
    /* App call SetScheduleTableAsync, OS should stop Sync. */
    boolean SchedTblIsStopAdjust;
    boolean SchedTblIsAdjustAdd;
    boolean SchedTblIsStarted;
} Os_STCBType;

/**
 * @brief The ID range type for SchedTbl.
 */
typedef struct
{
    Os_ScheduleTableType SchedTblStart;
    Os_ScheduleTableType SchedTblEnd;
} SchedTblRangeType;
#endif

/**
 * @brief The type definition of the spin lock.
 */
typedef uint32 Os_SpinlockType;

/**
 * @brief The type definition of the spin lock pointer.
 */
typedef Os_SpinlockType *Os_SpinlockRefType;

#if (CFG_SPINLOCK_MAX > 0U)
/**
 * @brief The TryToGetSpinlockType indicates if the spinlock has been occupied or not.
 */
/* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    TRYTOGETSPINLOCK_NOSUCCESS = 0U,
    TRYTOGETSPINLOCK_SUCCESS = 1U,
} TryToGetSpinlockType;
/* PRQA S 1271 -- */

/**
 * @brief Type definition of the spin lock method.
 */
/* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    LOCK_ALL_INTERRUPTS = 0U,
    LOCK_CAT2_INTERRUPTS = 1U,
    LOCK_NOTHING = 2U,
    LOCK_WITH_RES_SCHEDULER = 3U,
} Os_SpinlockMethod;
/* PRQA S 1271 -- */

/**
 * @brief The type definition of the spin lock id.
 */
typedef SpinlockIdType Os_SpinlockIdType;

/**
 * @brief The type definition of the spin lock control block.
 */
typedef struct
{
    uint16 ObjOccupyId;
    Os_ObjectTypeType ObjOccupyType;
    boolean Occupied;
} Os_SLCBType;

/**
 * @brief The type definition of the spin lock config.
 */
typedef struct
{
#if (CFG_OSAPPLICATION_MAX > 0U)
    const Os_ObjectAppCfgType *ObjAppCfg;
#endif
    uint32 SpinlockOrder;
    Os_SpinlockMethod SpinlockMethod;
    uint8 SpinlockGroupsId;
} Os_SpinlockCfgType;

#endif

#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)

/**
 * @brief The types of parameters for service protection checks.
 */
typedef struct
{
    uint32 AllowedContext;
    uint32 Address;
    Os_AppObjectId ObjectID;
    Os_ObjectTypeType ObjectType; /* PRQA S 1506 */ /* VL_Os_1506 */
} Os_ServicePortParamType;

/**
 * @brief Point to Os_ServicePortParamType.
 */
typedef Os_ServicePortParamType *Os_ServicePortParamRefType;

/**
 * @brief The type of stack position.
 */
typedef enum
{
/* the checked space is one part of the stack 
*     |    checked space     |
*   |            stack           |
*/
    STACK_INCLUDE_CHEKEDSPACE = 0,

/*the checked space cross the stack
*  |  checked space    |
*     |  stack   |      
*/
    CHEKEDSPACE_INCLUDE_STACK = 1,

/*the checked space cross the stack
* |      checked space  |
*   |       stack           |
* or
* |    checked space   |
*   |    stack            |
*/
    STACK_CROSS_CHEKEDSPACE = 2,

/* the stack and checked space are not mixed
* |    checked space     |
*                            |       stack       |
*/
    STACK_NOTMIXED_CHEKEDSPACE = 3
} Os_StackAndCheckedSpaceStatus;

typedef uint16 Os_AccessType;

/**
 * @brief This type holds information how a specific memory region can be accessed.
 */
typedef Os_AccessType AccessType;
typedef AccessType *AccessRefType;

typedef uint32 Os_MemoryStartAddressType;

/**
 * @brief This data type is a pointer which is able to point to any location in the MCU address space.
 */
typedef Os_MemoryStartAddressType MemoryStartAddressType;

typedef uint32 Os_MemorySizeType;
/* This data type holds the size (in bytes) of a memory region. */
/**
 * @brief This data type holds the size (in bytes) of a memory region.
 */
typedef Os_MemorySizeType MemorySizeType;

#endif


/**
 * @brief This struct type identifies the task configuration.
 */
typedef struct
{
    Os_EventMaskType    EventAccessMask;   /** event mask that task can access */
    Os_TaskEntry        TaskEntry;           /** Task entry */
#if (CFG_OSAPPLICATION_MAX > 0U)
    const Os_ObjectAppCfgType * ObjAppCfg;
#endif
/** AUTOSAR_SWS_OS.pdf 10.2.32: timing protection cfg for task. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    const Os_TmProtCfgType *    TaskTmProtCfgRef;
#endif
    Os_StackType        TaskStack;
    uint16              TaskActivation;      /** The maximum number of queued activation
                                                 requests for the task*/
    uint16              TaskPriority;        /** The priority of a task*/
    Os_AppModeType      TaskAutoStartMode; /** This container determines whether the task
                                               is activated during the system start-up
                                               procedure or not for some specific
                                               application modes */
    Os_TaskScheduleType TaskSchedule;        /** Defines the preemptability of the task */
#if (TRUE == CFG_GLOBAL_TASK_STACK_SHARING)
    boolean             StackSharing;        /* Task stack sharing */
#else
    uint8               Rsv;
#endif
    Os_CoreIdType       HostCore; /** owner core of the task */
} Os_TaskCfgType;

/**
 * @brief This struct type identifies Task control block type.
 */
typedef struct
{
    Os_StackPtrType  TaskStackTop;
    Os_StackPtrType  TaskStackBottom;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    uint64           taskTpTimeStamp; /* taskTpTimeStamp only record tp monitored time */
    uint64           taskTpLastReadTime;
    uint64           taskTpEerlistArrival;
    Os_TpNodeType*   taskTpHead;
    Os_TpNodeType    taskTpNode[TP_RESOURCE];
    Os_TpStatus      taskTpStatus;
#endif

#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR) /* PRQA S 3332 */ /* VL_Os_3332 */
    uint32            taskScheduleCount;
#endif

/** multi-core*/
#if ((CFG_SPINLOCK_MAX > 0U) || (CFG_STD_RESOURCE_MAX > 0U))
    uint16            TaskCriticalZoneStack[CFG_CRITICAL_ZONE_MAX];
    uint16            TaskCriticalZoneCount;
    uint16 volatile   TaskCurrentSpinlockOccupyLevel;
    Os_ObjectTypeType TaskCriticalZoneType[CFG_CRITICAL_ZONE_MAX];
#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType * TaskResourceStack; /** AutoSar: for protection hook. */
    uint8             TaskResCount;
#endif

    uint16           TaskRunPrio;
    Os_TaskStateType TaskState;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType CallBackAppID;
#endif

#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
    uint8            TaskActCount;
#endif

/** Autosar: for service protection*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    uint8             TaskDisableAllCount;
    uint8             TaskSuspendAllCount;
    uint8             TaskSuspendOsCount;
#endif

#if (CFG_INTERNAL_RESOURCE_MAX > 0U)
    boolean           TaskOccupyInterRes;
#endif
    
} Os_TCBType;

/**
 * @brief The ID range type for Task.
 */
typedef struct
{
    Os_TaskType Start;
    Os_TaskType End;
} Os_TaskRangeType;

/**
 * @brief The attribute type of the task.
 */
typedef struct
{
    Os_TaskRangeType AllTask;
    Os_TaskRangeType ExtendTask;
    Os_TaskRangeType BasicTask;
} Os_TaskPropertyType;

/**
 * @brief Define the base structure of the Os_TrustedFunctionIndex.
 */
typedef uint16 Os_TrustedFunctionIndexType;

/**
 * @brief This data type identifies a trusted function.
 */
typedef Os_TrustedFunctionIndexType TrustedFunctionIndexType;

/**
 * @brief This data type points to a structure which holds the arguments for a call to a trusted function.
 */
typedef void *TrustedFunctionParameterRefType;

/**
 * @brief Callback function of the Trustedfunction.
 */
typedef void (*Os_AppTrustedFunc)(TrustedFunctionIndexType index, TrustedFunctionParameterRefType ref);

/**
 * @brief Type definition of Trustedfunction data.
 */

#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
typedef struct
{
    uint32     TrustedFuncCaller[CFG_TRUSTED_SYSTEM_SERVICE_MAX];
    uint32     TrustedFuncWhoHook;
    uint8      TrustedFuncNest;
    boolean    TrustedFuncTportDelayCall;
    boolean    TrustedFuncTporFlag;
    StatusType TrustedFuncTporErrType;
}Os_TrustedFuncTpDataDef;
#endif

/**
 * @brief Indicates that the Rpc linked list head is Null.
 */
#define OS_RPC_INVALID_TRAIN (0xFFU)

/**
 * @brief Indicates the maximum input parameters of Rpc.
 */
#define OS_RPC_IN_PARA_MAX (5U)

/**
 * @brief Indicates the maximum node of Rpc.
 */
#define OS_RPC_MAX_NODE (10U)
/**
 * @brief The state of Rpc, whether asynchronous or synchronous.
 */
typedef enum
{
    OS_RPC_SYNC = 0,
    OS_RPC_ASYNC = 1
} Os_RpcSyncType;

/**
 * @brief Define the state of the Rpc list.
 */
typedef enum
{
    OS_RPC_FREE = 0,
    OS_RPC_START = 1,
    OS_RPC_WORK = 2,
    OS_RPC_DONE = 3
} Os_RpcServiceState;

/**
 * @brief Define the execution action of Rpc.
 */
typedef StatusType (*Os_RpcActionFn)(uint32 *inPara);

/**
 * @brief Define the input parameter type of Rpc.
 */
typedef struct
{
    Os_RpcActionFn ActionFn;
    uint32         SrvPara[6];/* PRQA S 3132 */ /* VL_QAC_MagicNum */
    uint32         InPara[OS_RPC_IN_PARA_MAX];
    Os_CoreIdType  RemoteCoreId;
    Os_RpcSyncType RpcSync;
} Os_RpcInputType;

/**
 * @brief Point to Os_RpcInputType.
 */
typedef Os_RpcInputType *Os_RpcInputRefType;

/**
 * @brief Define the Rpc node Id type.
 */
typedef uint8 Os_RpcNodeIdType;

/**
 * @brief Define the linked list type.
 */
typedef struct
{
    Os_RpcNodeIdType Head;
    Os_RpcNodeIdType Tail;
} Os_LinkQueType;

/**
 * @brief Point to Os_LinkQueType.
 */
typedef Os_LinkQueType *LinkQueRefType;

/**
 * @brief Define the Rpc service node type.
 */
typedef struct
{
    Os_RpcActionFn              ActionFn;
    uint32                      InPara[OS_RPC_IN_PARA_MAX];
    Os_CoreIdType               SourceCoreId;
    Os_RpcSyncType              RpcSync;
    Os_RpcNodeIdType            NextNode;
    volatile Os_RpcServiceState ProcState;
    StatusType                  RetValue;
} Os_RpcServiceNodeType;

/**
 * @brief Point to Os_RpcServiceNodeType.
 */
typedef Os_RpcServiceNodeType *Os_RpcServiceNodeRefType;

/**
 * @brief Define the information type of the Rpc object core.
 */
typedef struct
{
    Os_RpcServiceNodeType ServiceStation[OS_RPC_MAX_NODE];
    Os_LinkQueType FreeQue;
    Os_LinkQueType WorkQue;
} Os_RpcCoreType;

typedef enum
{
	BARRIER_WATING,
	BARRIER_RELEASED
}Os_BarrierStatusType;

typedef uint8   Os_BarrierTicketType;

typedef struct
{
    /* The current ticket count, incremented by 1 each time the sync occurs */
    Os_BarrierTicketType  ticket;
    Os_BarrierStatusType  status;  /* Current barrier state, wating, release */
}Os_BarrierCBType;

/* PRQA S 0602 ++ */ /* VL_Os_0602 */
typedef struct _Os_BarrierCfgType  Os_BarrierCfgType;
/* PRQA S 0602 -- */
struct _Os_BarrierCfgType
{
    volatile Os_BarrierCBType  *       BarrierCB;        /* Dynamic data */
    const Os_BarrierCfgType * const *  Participant;      /* Participants of the current Barrier */
    /* Barriers owned by the Core to which the current Barrier belongs */
    const Os_BarrierCfgType * const *  OwnBarriers; 
    /* The number of participants in the current Barrier */
    uint8                              ParticipantCount;
    /* The number of barriers owned by the Core to which the current Barrier belongs */
    uint8                              OwnBarriersCount;
    /* The Task to which the current Barrier belongs */
    TaskType                           OwnerTask;
};
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
/**
 * @brief System runtime information.
 */
typedef struct
{
    uint32           SysStackMaxUsage;
    uint32           TaskISR2MaxUsage;
    Os_CoreIdType    CoreId;
    TaskType         TaskID;
    ISRType          ISRID;
    Os_ServiceIdType ServiceID;
    StatusType       Error;
} Os_SystemInformation;

/**
 * @brief Description of hardware exception information.
 */
typedef struct
{
    char * ExceptionCaseMsg; /* PRQA S 3625 */ /* VL_Os_3625 */
    uint32 ExceptionCase;
    uint32 ExceptionAddress;
    uint32 ExceptionSP;
    uint32 ExceptionSysteamStatus;
} Os_TrapMsgType;

/**
 * @brief Hardware fault context.
 */
typedef struct
{
    Os_TrapRegsType TrapRegs; /* Hardware architecture dependent */
    Os_TrapMsgType  TrapMsg;
} Os_TrapContextType;

/**
 * @brief Exception msg.
 */
typedef struct
{
    Os_SystemInformation SystemInf;
    Os_TrapContextType   TrapContext;
    boolean IsHardFault;
} Os_ExceptionMsg;
typedef Os_ExceptionMsg ExceptionMsgType;
#endif  /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

/**
 * @brief Type definition of the Object.
 */
typedef uint16 ObjectType; /* PRQA S 1506 */ /* VL_Os_1506 */

/**
 * @brief The structure configuration type definition of the Application.
 */
typedef struct
{
    const Os_AppHookCfgType             ApplicationHooks;
    const Os_AppObjectRefType *         AppObjectRef;
    const Os_TrustedFunctionIndexType * AppTrustedFuncRef;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    const Os_AppAddrType                AppAssignedPeripheralAddr;
#endif
    uint16  HostCore; /** owner core of the app */
    uint16  AppTaskCnt;
    uint16  AppIsrRefCnt;
    uint16  AppAlarmRefCnt;
    uint16  AppCounterRefCnt;
    uint16  AppScheduleTableCnt;
    uint16  AppTrustedFuncCnt; /** Element count of AppTrustedFuncRef array */
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0)
    uint16  AppTaskHighestPriority;
#endif
    const Os_TaskType RestartTask;
    boolean           Trusted; /** true: trusted; fasle: not trusted (default). */
    /** true: time protection is delayed
        false: time protection is triggered immediately */
    boolean           TrustedApplicationDelayTimingViolationCall;
    /** true: OS-Application runs within a protected environment.
        false: OS-Application has full write access (default) */
    boolean           TrustedAppWithProtection;
} Os_ApplicationCfgType;

/**
 * @brief Parameter types during system operation.
 */
typedef struct
{
    Os_EventMaskType                EventMask;
    uint8 *                         Address8;
    uint8 *                         Value8;
    uint16 *                        Address16;
    uint16 *                        Value16;
    uint32 *                        Address32;
    uint32 *                        Value32;
    StatusType *                    Status;

#if (CFG_SPINLOCK_MAX > 0U)
    TryToGetSpinlockType *          Success;
#endif

    Os_EventMaskRefType             EventRef;
    Os_TaskRefType                  TaskRef;
    Os_TaskStateRefType             TaskState;
    TrustedFunctionParameterRefType FunctionParams;
    Os_TickRefType                  CounterValue;
    Os_TickRefType                  CounterElapsedValue;
    
    uint32                          Clearmask32;
    uint32                          Setmask32;
    uint32                          IocValue;
            
    TickType                        Offset;
    TickType                        Start;

#if (CFG_ALARM_MAX > 0)     
    Os_AlarmBaseRefType             AlarmBaseRef;
    Os_TickRefType                  AlarmTickRef;
    Os_TickType                     AlarmTime;
    Os_TickType                     AlarmCycle;
#endif

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    Os_MemoryStartAddressType       MemStartAddress;
    Os_MemorySizeType               MemSize;
#endif

    uint16                          Clearmask16;
    uint16                          Setmask16;
    uint16                          Area;
    uint16                          SpinlockId;
    TrustedFunctionIndexType        FunctionIndex; 
    uint16                          DataLenth;
    CoreIdType                      CoreID;
    Os_TaskType                     TaskId;
    CounterType                     CounterId;
    Os_IsrType                      ISRID;

#if (CFG_OSAPPLICATION_MAX > 0U)
    ApplicationStateRefType         AppStateRef;
    Os_ApplicationType              ApplID;
    Os_AppObjectId                  ObjectID;
#endif

#if (CFG_SCHEDTBL_MAX > 0U)
    ScheduleTableStatusRefType      ScheduleStatus;
    TickType                        ScheduleTableOffset;
    TickType                        ScheduleTableStart;
    TickType                        Value;
    ScheduleTableType               ScheduleTableID_From;
    ScheduleTableType               ScheduleTableID_To;
    ScheduleTableType               ScheduleTableId;
#endif

#if (CFG_ALARM_MAX > 0)
    Os_AlarmType                    AlarmId;
#endif
#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType                 ResourceId;
#endif            

#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ObjectTypeType               ObjectType; /* PRQA S 1506 */ /* VL_Os_1506 */
    Os_RestartType                  RestartOption;
#endif
    uint8                           Clearmask8;
    uint8                           Setmask8;
    StatusType                      ShutdownError;
    IdleModeType                    IdleMode;
    boolean                         ClearPending;
} Os_RunTimeSrvPrarmType;

#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
/* Timer tick type for 64-bit */
typedef uint64 Os_TimerTickType;

typedef struct
{
    volatile Os_TimerTickType softCounter;
#if (TRUE == OS_HAL_SYSTEM_TIMER_IS_USED_CMP)
    volatile Os_TickType baseCmpValue;
#endif /* TRUE == OS_HAL_SYSTEM_TIMER_IS_USED_CMP */
}Os_TimeStampType;
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */

#if ((TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR) || (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR)) /* PRQA S 3332 */ /* VL_Os_3332 */
typedef uint8 Os_EventCountType;

typedef struct
{
    Os_EventMaskType eventMask;
#if (TRUE == CFG_EVENT_RESPONSE_TIME_MONITOR)
    Os_TimerTickType eventStartResponseTime;
    Os_TimerTickType eventEndResponseTime;
    Os_TimerTickType eventResponseTime;
#endif
#if (TRUE == CFG_EVENT_RESPONSE_RATE_MONITOR)
    uint16 eventResponseNum;
#endif
} Os_EventMonitorType;

#endif

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
typedef enum
{
    OS_UNKNOWN     = 0,
    OS_OS_SUSPEND  = 1,
    OS_ALL_SUSPEND = 2
} Os_InterApiType;

typedef struct
{
    Os_InterApiType     CallApi;
    Os_CallLevelType    CallLevel;
    uint16              CallerId;
    Os_TimerTickType    LockTime;
} Os_InterRecordType;

typedef struct Os_InterMonitorList
{
    Os_InterRecordType          Node;
    struct Os_InterMonitorList* Pre;
    struct Os_InterMonitorList* Next;
} Os_InterLinkList;

typedef struct
{
    Os_TimerTickType              StartTime;
    Os_TimerTickType              LastTime;
    uint16              OsLockRatio;
    uint16              AllLockRatio;
    Os_TimerTickType              InterAllStartTime;
    Os_TimerTickType              InterOsStartTime;
    Os_TimerTickType              InterAllLockTime;
    Os_TimerTickType              InterOsLockTime;
    uint16              LoopCnt;
    Os_InterLinkList*   TopRecordTableHead;
    Os_InterLinkList*   TopRecordTableEnd;
    Os_InterRecordType* InterRecordTable;
} Os_InterMonitorRatioType;

typedef Os_InterMonitorRatioType * Os_InterMonitorRatioRefType;

#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
typedef struct
{
    uint32 averageLoadRatio;
    uint32 maxLoadRatio;
    uint32 minLoadRatio;
    uint32 totalLoadRatio;
    uint32 singleLoadRatio;
    Os_TimerTickType totalTicks;
    Os_TimerTickType startTicks;
} Os_LoadRatioType;

typedef Os_LoadRatioType* Os_LoadRatioRefType;

typedef enum
{
    LR_INVALID_STA = 0,
    LR_ENTER_TASK = 1,
    LR_EXIT_TASK = 2,
    LR_ENTER_ISR= 3,
    LR_EXIT_ISR= 4
} Os_LoadRatioRunStateType;

typedef enum
{
    LR_CALC_TASK_AVERAGE = 0,
    LR_CALC_TASK_MAX = 1,
    LR_CALC_TASK_MIN = 2,
    LR_CALC_TASK_LATEST = 3,
    LR_CALC_ISR_AVERAGE = 4,
    LR_CALC_ISR_MAX = 5,
    LR_CALC_ISR_MIN = 6,
    LR_CALC_ISR_LATEST = 7,
    LR_CALC_CPU_AVERAGE = 8,
    LR_CALC_CPU_MAX = 9,
    LR_CALC_CPU_MIN = 10,
    LR_CALC_CPU_LATEST = 11,
    LR_CALC_INVALID = 12
} Os_LoadRatioCalcType;

typedef Os_LoadRatioCalcType LoadRatioCalcType;

typedef struct
{
	Os_LoadRatioRunStateType Os_LoadRatioRunState;
	uint32                   Os_CpuLoadRatioCount;
	Os_LoadRatioType         Os_CpuLoadRatio;
	Os_LoadRatioRefType      Os_TaskLoadRatio;
	Os_LoadRatioRefType      Os_IsrLoadRatio;
} Os_LoadRatioMonitorType;

typedef Os_LoadRatioMonitorType *Os_LoadRatioMonitorRefType;

#endif

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
typedef struct
{
    Os_TimerTickType    averageTicks;
    Os_TimerTickType    maxTicks;
    Os_TimerTickType    minTicks;
    Os_TimerTickType    totalTicks;
    Os_TimerTickType    startTicks;
} Os_MonitorTimeType;

typedef struct
{
    Os_MonitorTimeType tick;
    uint32             taskActCount;
} Os_TaskResponseTimeType;

typedef Os_TaskResponseTimeType* Os_TaskResponseTimeRefType;
#endif

typedef enum
{
    OS_PROTECTION_TERMINATE_INVALID = 0,
    OS_PROTECTION_TERMINATE_TASK = 1,
    OS_PROTECTION_TERMINATE_TPROT_ISR = 2,
    OS_PROTECTION_TERMINATE_NESTED_ISR = 3,
} Os_ProtectionTerminateObjType;

/**
 * @brief System control block type definitions.
 */
typedef struct
{
#if (TRUE == CFG_USEPARAMETERACCESS)
    Os_RunTimeSrvPrarmType    SysRunTimeSrvPrarm;
#endif
    /** Trusted Function */
#if (CFG_TRUSTED_SYSTEM_SERVICE_MAX > 0U)
    Os_TrustedFuncTpDataDef   Os_TrustedFuncTp;
    TrustedFunctionIndexType  TrustedFuncNestQueue[CFG_TRUSTED_SYSTEM_SERVICE_MAX];
#endif
    const Os_ReadyQueueManageType * QueueMg;
    Os_TCBType *              SysRunningTCB;
    const Os_StackType *      SystemStack;
#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)
    Os_ExceptionMsg *         FaultSySInformation;
#endif

#if (CFG_ISR2_MAX > 0)
    Os_IsrType *              SysIsrNestQueue;
#endif
    uint32                    LastUnhandledIrq;
    uint32                    LastUnhandledExc;

#if (FALSE == CFG_TIMING_PROTECTION_ENABLE)
    Os_ArchMsrType            SaveAllInt;
    Os_ArchMsrType            SaveAllIntNested;
#endif

    Os_AppModeType            SysActiveAppMode;
    Os_PriorityType           SysHighPrio;
    uint16                    SysTaskMax;
    Os_CoreIdType             SysCore; /** logical CoreID*/
    uint16                    PriorityNum; /** the number of task priorities */
#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType           ScheduleResId;
#endif

    /** Interrupt module */
    uint16                    SaveOsIntNested;
    uint16                    CfgIsrMax;
    uint16                    CfgIsr2Max;
    uint16                    TaskIdle;
    Os_TaskType               SysHighTaskId;
    Os_TaskType               SysRunningTaskId;
    Os_TaskType               SysPrevTaskID;
#if (CFG_OSAPPLICATION_MAX > 0)
    Os_ApplicationType        SysRunningAppId; /** the running application ID */
    ApplicationType           SysAppId;
#endif
    /** Alarm module */
#if (CFG_ALARM_MAX > 0)
    Os_AlarmType              SysAlarmMax;
#endif
#if (CFG_ISR2_MAX > 0)
    Os_IsrType                SysRunningIsrCat2Id; /** Running isr category 2. */
    boolean                   SysInIsrCat2;
#endif
    Os_LockerType             SysDispatchLocker;
    Os_CallLevelType          SysOsLevel;
    uint8                     SuspendAllCount;
    uint8                     SuspendOsCount;
    Os_CallLevelType          SaveLevel;
    uint8                     IntNestISR1;
    Os_CallLevelType          SaveLevelISR1;
    uint8                     IntNestISR2;
    boolean                   UnhandledIrqReported;
    boolean                   UnhandledExcReported;

#if (TRUE == CFG_USEGETSERVICEID)
    Os_ServiceIdType          SysServiceId;
#endif

#if (CFG_OSAPPLICATION_MAX > 0U)
    ObjectTypeType            SysRunningAppObj;    /** the running object from the application */
#endif

    Os_ProtectionTerminateObjType ProtectionTerminateObj;

#if (CFG_SPINLOCK_MAX > 0)
    boolean                   CurrentSpinlockOccupied[CFG_SPINLOCK_MAX]; /* PRQA S 1039*/ /* VL_Os_1039*/
#endif

#if (TRUE == CFG_RTI_ENABLE)
    Os_TraceApiIdType         CurrentApiId;
#endif
#if ((TRUE == CFG_RTI_ENABLE)||(TRUE == CFG_USEGETSERVICEID))
    StatusType                SysOsError;
#endif

#if (CFG_INTERRUPT_MONITOR_ENABLE == TRUE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_InterMonitorRatioType  InterMonitorRatio;
	boolean             InterInitFlag;
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_LoadRatioMonitorType    LoadRatioMonitor;
#endif

#if (TRUE == CFG_TASK_RESPONSE_TIME_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
    Os_TaskResponseTimeRefType Os_TaskResponseTime;
#endif

} Os_SCBType;

#endif

/*=======[E N D   O F   F I L E]==============================================*/
