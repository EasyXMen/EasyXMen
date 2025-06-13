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
**  FILENAME    :  Os_Types.h                                                 **
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

#ifndef OS_TYPES_H
#define OS_TYPES_H
/*=======[I N C L U D E S]====================================================*/
#include "Os_Marcos.h"
#include "Os_Cfg.h"
#include "Os_ECode.h"
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */ /* OS_TYPE_COMPILER_001*/
/* --------------------system-------------------- */
typedef enum
{
    /*AUTOSAR Service*/
    OSServiceId_GetApplicationID = 0x00,
    OSServiceId_GetISRID = 0x01,
    OSServiceId_CallTrustedFunction = 0x02,
    OSServiceId_CheckISRMemoryAccess = 0x03,
    OSServiceId_CheckTaskMemoryAccess = 0x04,
    OSServiceId_CheckObjectAccess = 0x05,
    OSServiceId_CheckObjectOwnership = 0x06,
    OSServiceId_StartScheduleTableRel = 0x07,
    OSServiceId_StartScheduleTableAbs = 0x08,
    OSServiceId_StopScheduleTable = 0x09,
    OSServiceId_NextScheduleTable = 0x0a,
    OSServiceId_StartScheduleTableSynchron = 0x0b,
    OSServiceId_SyncScheduleTable = 0x0c,
    OSServiceId_SetScheduleTableAsync = 0x0d,
    OSServiceId_GetScheduleTableStatus = 0x0e,
    OSServiceId_IncrementCounter = 0x0f,
    OSServiceId_GetCounterValue = 0x10,
    OSServiceId_GetElapsedValue = 0x11,
    OSServiceId_TerminateApplication = 0x12,
    OSServiceId_AllowAccess = 0x13,
    OSServiceId_GetApplicationState = 0x14,
    OSServiceId_GetNumberOfActivatedCores = 0x15,
    OSServiceId_GetCoreID = 0x16,
    OSServiceId_StartCore = 0x17,
    OSServiceId_StartNonAutosarCore = 0x18,
    OSServiceId_GetSpinlock = 0x19,
    OSServiceId_ReleaseSpinlock = 0x1a,
    OSServiceId_TryToGetSpinlock = 0x1b,
    OSServiceId_ShutdownAllCores = 0x1c,
    OSServiceId_ControlIdle = 0x1d,
    OSServiceId_IocSend = 0x1e,
    OSServiceId_IocWrite = 0x1f,
    OSServiceId_IocSendGroup = 0x20,
    OSServiceId_IocWriteGroup = 0x21,
    OSServiceId_IocReceive = 0x22,
    OSServiceId_IocRead = 0x23,
    OSServiceId_IocReceiveGroup = 0x24,
    OSServiceId_IocReadGroup = 0x25,
    OSServiceId_IocEmptyQueue = 0x26,
    OSServiceId_GetCurrentApplicationID = 0x27,
    OSServiceId_ReadPeripheral8 = 0x28,
    OSServiceId_ReadPeripheral16 = 0x29,
    OSServiceId_ReadPeripheral32 = 0x2a,
    OSServiceId_WritePeripheral8 = 0x2b,
    OSServiceId_WritePeripheral16 = 0x2c,
    OSServiceId_WritePeripheral32 = 0x2d,
    OSServiceId_ModifyPeripheral8 = 0x2e,
    OSServiceId_ModifyPeripheral32 = 0x2f,
    OSServiceId_DisableInterruptSource = 0x30,
    OSServiceId_EnableInterruptSource = 0x31,
    OSServiceId_ClearPendingInterrupt = 0x32,
    OSServiceId_ActivateTaskAsyn = 0x33,
    OSServiceId_SetEventAsyn = 0x34,
    OSServiceId_ModifyPeripheral16 = 0x35,

    /* Currently no service execution */
    OSServiceId_NoServiceRunning = 0x44,

    /*Additional Service*/
    OSServiceId_WaitAllEvents = 0xD7,
    OSServiceId_IocCallBackNotify = 0xDE,

    /*OSEK Service*/
    OSServiceId_ActivateTask = 0xDF,
    OSServiceId_TerminateTask = 0xE0,
    OSServiceId_ChainTask = 0xE1,
    OSServiceId_Schedule = 0xE2,
    OSServiceId_GetTaskID = 0xE3,
    OSServiceId_GetTaskState = 0xE4,
    OSServiceId_EnableAllInterrupts = 0xE5,
    OSServiceId_DisableAllInterrupts = 0xE6,
    OSServiceId_ResumeAllInterrupts = 0xE7,
    OSServiceId_SuspendAllInterrupts = 0xE8,
    OSServiceId_ResumeOSInterrupts = 0xE9,
    OSServiceId_SuspendOSInterrupts = 0xEA,
    OSServiceId_DeclareResource = 0xEB,
    OSServiceId_GetResource = 0xEC,
    OSServiceId_ReleaseResource = 0xED,
    OSServiceId_DeclareEvent = 0xEE,
    OSServiceId_SetEvent = 0xEF,
    OSServiceId_ClearEvent = 0xF0,
    OSServiceId_GetEvent = 0xF1,
    OSServiceId_WaitEvent = 0xF2,
    OSServiceId_GetAlarmBase = 0xF3,
    OSServiceId_GetAlarm = 0xF4,
    OSServiceId_SetRelAlarm = 0xF5,
    OSServiceId_SetAbsAlarm = 0xF6,
    OSServiceId_CancelAlarm = 0xF7,
    OSServiceId_GetActiveApplicationMode = 0xF8,
    OSServiceId_StartOS = 0xF9,
    OSServiceId_ShutdownOS = 0xFA,
    OSServiceId_ErrorHook = 0xFB,
    OSServiceId_PreTaskHook = 0xFC,
    OSServiceId_PostTaskHook = 0xFD,
    OSServiceId_StartupHook = 0xFE,
    OSServiceId_ShutdownHook = 0xFF,
} Os_ServiceIdType;

/* definition of Os level of call API, add some for service protection in SC3 and SC4 */
typedef enum
{
    OS_LEVEL_MAIN = 0U,
    OS_LEVEL_ERRORHOOK_APP = 1U,
    OS_LEVEL_TASK = 2U,
    OS_LEVEL_ISR1 = 3U,
    OS_LEVEL_ISR2 = 4U,
    OS_LEVEL_ERRORHOOK = 5U,
    OS_LEVEL_PRETASKHOOK = 6U,
    OS_LEVEL_POSTTASKHOOK = 7U,
    OS_LEVEL_STARTUPHOOK = 8U,
    OS_LEVEL_SHUTDOWNHOOK = 9U,
    OS_LEVEL_ALARMCALLBACK = 10U,
    OS_LEVEL_PROTECTIONHOOK = 11U,
    OS_LEVEL_STANDARD_RESOURCE = 12U,
    OS_LEVEL_INTERNAL_RESOURCE = 13U
} Os_CallLevelType;

/* Type definition of the App mode. */
typedef uint16 Os_AppModeType;
typedef Os_AppModeType AppModeType;

/* sched lock */
typedef uint8 Os_LockerType;

/* Stack */

/* Defines stack data types. */
typedef uint32 Os_StackDataType;
/* Pointer to stack data. */
typedef uint32* Os_StackPtrType;
/* The structure type of the stack. */
typedef struct
{
    Os_StackPtrType stackTop;
    Os_StackPtrType stackBottom;
} Os_StackType;

/* This data type identifies an object. */
typedef enum
{
    OBJECT_TASK = 0U,
    OBJECT_ISR = 1U,
    OBJECT_ALARM = 2U,
    OBJECT_COUNTER = 3U,
    OBJECT_SCHEDULETABLE = 4U,
    OBJECT_APP_MAX = 5U,

    OBJECT_RESOURCE = 5U,
    OBJECT_SPINLOCK = 6U,
    OBJECT_MAX = 7U /*The Max Value is invalid, just used in coding*/
} Os_ObjectTypeType;

typedef Os_ObjectTypeType ObjectTypeType;

/* The type definition of the core id. */
typedef uint16 Os_CoreIdType;

/* The pointer type definition of the core id. */
typedef Os_CoreIdType* Os_CoreIdRefType;

/* CoreIdType is a scalar that allows identifying a single core.
The CoreIdType shall represent the logical CoreID. */
typedef Os_CoreIdType CoreIdType;
typedef Os_CoreIdType* CoreIdRefType;

/* The type definition of the Application. */
typedef uint16 Os_ApplicationType;

/* This data type identifies the OS-Application. */
typedef Os_ApplicationType ApplicationType;

/* Type definition of OS resources. */
typedef uint16 Os_ResourceType;
typedef Os_ResourceType ResourceType;
typedef struct
{
    uint32 queueHead;
    uint32 queueTail;
} Os_ReadyQueueType;

typedef enum
{
    SHUTDOWN_OS = 0U,
    SHUTDOWN_ALL_OS = 1U
} Os_ShutdownAction;
/* -----------------------------task---------------------- */
/* This enum type identifies Task State. */
typedef enum
{
    TASK_STATE_WAITING = 0U,
    TASK_STATE_READY = 1U,
    TASK_STATE_SUSPENDED = 2U,
    TASK_STATE_RUNNING = 3U,
    /*for distinguish the autostarted task,the first activatedtask and preempted task */
    TASK_STATE_START = 4U
} Os_TaskStateType;

typedef Os_TaskStateType TaskStateType;
typedef Os_TaskStateType* TaskStateRefType;
typedef Os_TaskStateType* Os_TaskStateRefType;
typedef uint16 Os_TaskType;
typedef Os_TaskType* Os_TaskRefType;
typedef Os_TaskType TaskType;
typedef Os_TaskType* TaskRefType;
typedef uint8 Os_TaskScheduleType;
typedef uint16 Os_PriorityType;

typedef P2FUNC(void, OS_APPL_CODE, TaskEntry)(void);

/* ------------------------Alarm------------------------- */
typedef enum
{
    ALARM_AUTOSTART_ABSOLUTE = 0U,
    ALARM_AUTOSTART_RELATIVE = 1U
} Os_AlarmAutostartTypeType;

/* This type of tick. */
typedef uint32 Os_TickType;
typedef Os_TickType* Os_TickRefType;
typedef Os_TickType TickType;
typedef TickType* TickRefType;
typedef uint16 Os_CounterType;
/* This data type identifies a counter. */
typedef Os_CounterType CounterType;

/* This type of Alarm. */
typedef uint16 Os_AlarmType;
typedef Os_AlarmType* Os_AlarmRefType;
typedef Os_AlarmType AlarmType;
typedef Os_AlarmType* AlarmRefType;
typedef P2FUNC(void, OS_APPL_CODE, Os_AlarmCallbackType)(void);

/* To unify with conformance testcases */
/* DD_1_1111 */
typedef struct
{
    Os_TickType maxallowedvalue;
    Os_TickType ticksperbase;
    Os_TickType mincycle;
} Os_AlarmBaseType;
typedef Os_AlarmBaseType AlarmBaseType; /*to unify with conformance testcases*/
typedef Os_AlarmBaseType* Os_AlarmBaseRefType;
typedef Os_AlarmBaseType* AlarmBaseRefType;

/* ---------------------------resource----------------------- */

/* Definition of resource occupancy type. */
typedef enum
{
    OCCUPIED_BY_TASK = 0,             /*occupied by task*/
    OCCUPIED_BY_INTERRUPT = 1,        /*occupied by interrupt*/
    OCCUPIED_BY_TASK_OR_INTERRUPT = 2 /*occupied by task or interrupt*/
} Os_ResourceOccupyType;

typedef struct
{
    Os_ResourceType* TaskResourceStackPtr;
    uint16 ResourceNum;
} TaskResourceStackType;

/* ------------------------------Event---------------------------- */
/* This type of event. */
typedef uint64 Os_EventType;
typedef Os_EventType EventType;

/* This type of event mask. */
typedef uint64 Os_EventMaskType;
typedef Os_EventMaskType* Os_EventMaskRefType;
typedef Os_EventMaskType EventMaskType;
typedef EventMaskType* EventMaskRefType;

/* This struct type identifies the Event control block. */
typedef struct
{
    Os_EventMaskType eventSetEvent;
    Os_EventMaskType eventWaitEvent;
    boolean eventIsWaitAllEvents;
} Os_ECBType;

/* -----------------------------Isr------------------------------ */
/* Definition of OS interrupt category. */
typedef enum
{
    OS_ISR_CATEGORY1 = 0U,
    OS_ISR_CATEGORY2 = 1U
} Os_IsrCategoryType;

typedef enum
{
    OS_ISR_DISABLED = 0U,
    OS_ISR_ENABLED = 1U
} Os_IsrStateType;

/* interrupt priority */
typedef uint16 Os_IPLType;
typedef uint16 Os_IsrType;
/* This data type identifies an interrupt service routine (ISR). */
typedef Os_IsrType ISRType;
typedef enum
{
    OS_ISR_ENABLE = 0U,
    OS_ISR_DISABLE = 1U
} Os_IsrDescriptionType;

/*  -----------------------------ScheduleTable----------------------- */
/* This type describes the status of a schedule. The status can be one of the
   following:
1. The schedule table is not started (SCHEDULETABLE_STOPPED)
2. The schedule table will be started after the end of currently running schedule
   table (schedule table was used in NextScheduleTable() service) (SCHEDULETABLE_NEXT)
3. The schedule table uses explicit synchronization, has been started and is
   waiting for the global time. (SCHEDULETABLE_WAITING)
4. The schedule table is running, but is currently not synchronous to a global
   time source (SCHEDULETABLE_RUNNING)
5. The schedule table is running and is synchronous to a global time source
   (SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS) */
/* DD_1_0062 */
typedef enum
{
    SCHEDULETABLE_STOPPED = 0U,
    SCHEDULETABLE_NEXT = 1U,
    SCHEDULETABLE_WAITING = 2U,
    SCHEDULETABLE_RUNNING = 3U,
    SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS = 4U
} Os_SchedTblStateType;
typedef Os_SchedTblStateType ScheduleTableStatusType;
/* This data type points to a variable of the data type ScheduleTableStatusType. */
typedef ScheduleTableStatusType* ScheduleTableStatusRefType;

typedef enum
{
    ST_START_ABSOLUTE = 0U,
    ST_START_RELATIVE = 1U,

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    ST_START_SYNCHRON = 2U
#endif
} Os_SchedTblAutostartType;
typedef uint16 Os_ScheduleTableType;
/* This data type identifies a schedule table. */
typedef Os_ScheduleTableType ScheduleTableType;
typedef uint16 Os_SchedTblSetEvent;
typedef Os_SchedTblSetEvent* Os_SchedTblSetEventRef;
typedef uint16 Os_SchedTblSetEventTask;
typedef Os_SchedTblSetEventTask* Os_SchedTblSetEventTaskRef;
typedef uint16 Os_SchedTblActivateTask;
typedef Os_SchedTblActivateTask* Os_SchedTblActivateTaskRef;

/* This type of Counter . */
typedef Os_CounterType Os_SchedTblCounterRefType;

/* OsScheduleTableAutostart. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.23. */
/* This struct type identifies a schedule tableAutostart . */
typedef struct
{
    Os_TickType osSchedTblAbsValue;
    Os_TickType osSchedTblRelOffset;
    Os_SchedTblAutostartType osSchedTblAutostartType;

    /* Note: In AUTOSAR_SWS_OS.pdf 10.2.23, Os_SchedTblAutostart can reference more then one appmode. */
    Os_AppModeType osSchedTblAppModeBitmap;
} Os_SchedTblAutostart;

/* OsScheduleTableSync. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.28. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
/* Enumeration type of synchronization policy. */
typedef enum
{
    ST_SYNC_EXPLICIT = 0U,
    ST_SYNC_IMPLICIT = 1U,
    ST_SYNC_NONE = 2U
} Os_SchedTblSyncStrategy;

/* Schedule table synchronization type. */
typedef struct
{
    uint16 osSchedTblExplicitPrecision;
    Os_SchedTblSyncStrategy osSchedTblSyncStrategy;
} Os_SchedTblSync;
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */

/* OsScheduleTableEventSetting. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.24. */
/* This struct type identifies the reference event to set. */
typedef struct
{
    Os_EventType osSchedTblSetEventRef;
    Os_TaskType osSchedTblSetEventTaskRef;
} Os_EPSetEventAction;

/* OsScheduleTableTaskActivation. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.26. */
/* This struct type identifies the reference task to activate. */
typedef struct
{
    Os_TaskType osSchedTblActivateTaskRef;
} Os_EPActivateTaskAction;

/* OsScheduleTableExpriyPoint. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.25. */
/* This struct type identifies a schedule tableExpriyPoint . */
typedef struct
{
    Os_TickType osSchedTblEPOffset;
    uint16 osSetEventListSize;
    uint16 osActivateTaskListSize;
    P2CONST(Os_EPSetEventAction, OS_VAR, OS_CONST) osSetEventList;
    P2CONST(Os_EPActivateTaskAction, OS_VAR, OS_CONST) osActivateTaskList;
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_TickType osSchedTblMaxAdvance;
    Os_TickType osSchedTblMaxRetard;
#endif
} Os_SchedTblEP;

/* -------------------------------Counter--------------------------- */
/* This type of Counter type. */
typedef enum
{
    COUNTER_HARDWARE = 0,
    COUNTER_SOFTWARE = 1
} Os_CounterEnumType;

/* -------------------------timing protection----------------------------*/
/* AutoSar: SC2, timing protection for task. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
typedef enum
{
    TP_SUS_OS_INT = 0U,
    TP_SUS_ALL_INT = 1U,
    TP_DIS_ALL_INT = 2U,

    TP_OPT_BUTT = 3U
} Os_TaskIsrOptType;

/* Add: AutoSar SC2: Time protection operation type definitions. */
/* Task timing protection operation type definition. */
typedef enum
{
    TP_TASK_EXE = 0U,
    TP_TASK_ARRIVAL = 1U,
    TP_TASK_SUS_OS_INT = 2U,
    TP_TASK_SUS_ALL_INT = 3U,
    TP_TASK_DIS_ALL_INT = 4U,

    TP_OPT_TASK_BUTT = 5U /* Attention: used as index of array! */
} Os_TmProtOptTaskType;

/* Interrupt timing protection operation type definition. */
typedef enum
{
    TP_ISR_CAT2_EXE = 0U,
    TP_ISR_CAT2_ARRIVAL = 1U,
    TP_ISR_CAT2_SUS_OS_INT = 2U,
    TP_ISR_CAT2_SUS_ALL_INT = 3U,
    TP_ISR_CAT2_DIS_ALL_INT = 4U,

    TP_OPT_ISR_BUTT = 5U /* Attention: used as index of array! */
} Os_TmProtOptIsrType;

/* Timing protection: who occupy a resource, task or isr. */
/* Type definition of resource occupation. */
typedef enum
{
    TP_RES_OCCUPY_TASK = 0U,
    TP_RES_OCCUPY_ISR = 1U,

    TP_RES_OCCUPY_BUTT = 2U
} Os_TmProtResOccupyType;

/* Define resource for task locking */
typedef struct
{
    Os_TickType OsTaskResLockBudget;
    Os_ResourceType OsTaskResLockResRef;
} Os_TaskResLockType;

/* Task timing protection configuration structure type. */
typedef struct
{
    Os_TickType osTaskAllInterruptLockBudget; /* Float. Cfg tool switch to tick. */
    Os_TickType osTaskExecutionBudget;        /* Float. Cfg tool switch to tick. */
    Os_TickType osTaskOsInterruptLockBudget;  /* Float. Cfg tool switch to tick. */
    Os_TickType osTaskTimeFrame;              /* Float. Cfg tool switch to tick. */
    uint16 osTaskResLockCnt;                  /* Count of Os_TaskResLockType. */
    P2CONST(Os_TaskResLockType, OS_VAR, OS_CONST) osTaskResLockRef;
} Os_TaskTmProtCfgType;

/* OsIsrTimingProtection.  AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.19. */
/* Resources that define interrupt locking. */
typedef struct
{
    Os_TickType osIsrResLockBudget; /* Float. Cfg tool switch to tick count. */
    Os_ResourceType osIsrResLockResRef;
} Os_IsrResLockType;

/* Configuration type of interrupt timing protection configuration. */
typedef struct
{
    Os_TickType osIsrAllIptLockBudget; /* Float. Cfg tool switch to tick count. */
    Os_TickType osIsrExeBudget;        /* Float. Cfg tool switch to tick count. */
    Os_TickType osIsrOsIptLockBudget;  /* Float. Cfg tool switch to tick count. */
    Os_TickType osIsrTimeFrame;        /* Float. Cfg tool switch to tick count. */
    uint16 osIsrResLockCnt;            /* Count of Os_IsrResLockType. */
    P2CONST(Os_IsrResLockType, OS_VAR, OS_CONST) osIsrResLockRef;
} Os_IsrTimingProtectionType;

/* Timing protection control block data definition. */
typedef struct
{
    boolean osIsTpStart;
    uint8 osWhoHook;
    uint8 Rsv[2];

    Os_TickType osTpTime;
    Os_TickType osTpBudget;
} Os_TmProtCbDataDef;
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */
/* -----------------------TrustedFunction-------------------------*/
/* 8.3.2 */
/* Define the base structure of the Os_TrustedFunctionIndex. */
typedef uint16 Os_TrustedFunctionIndexType;

/* This data type identifies a trusted function. */
typedef Os_TrustedFunctionIndexType TrustedFunctionIndexType;

/* 8.3.3  MISRA-C 2004 11.2  */
/* This data type points to a structure which holds the arguments for a call to a trusted function. */
typedef void* TrustedFunctionParameterRefType;

/* Used in Os_AppTrustedFunctionCfgType */
typedef P2FUNC(void, OS_APPL_CODE, Os_AppTrustedFunc)(TrustedFunctionIndexType ix, TrustedFunctionParameterRefType ref);

typedef struct
{
    boolean TrustedFuncTportDelayCall;
    boolean TrustedFuncTporFlag;
    StatusType TrustedFuncTporErrType;
    uint32 osWhoHook;
} Os_TrustedFuncTpDataDef;
/* ------------------------application---------------------------- */
/* 8.3.5 */
/* This data type identifies if an OS-Application has access to an object. */
typedef enum
{
    ACCESS = 0U,
    NO_ACCESS = 1U
} ObjectAccessType;

/* 8.3.15 */
/* This data type defines the use of a Restart Task after terminating an OSApplication. */
typedef enum
{
    RESTART = 0U,
    NO_RESTART = 1U
} Os_RestartType;
/* This data type defines the use of a Restart Task after terminating an OS-Application. */
typedef Os_RestartType RestartType;

/* This data type identifies the state of an OS-Application. */
typedef enum
{
    APPLICATION_ACCESSIBLE = 0U,
    APPLICATION_RESTARTING = 1U,
    APPLICATION_TERMINATED = 2U
} Os_ApplicationStateType;

/* The type definition of the pointer to the Application state. */
typedef Os_ApplicationStateType* Os_ApplicationStateRefType;
typedef Os_ApplicationStateType ApplicationStateType;

/* This data type points to location where a ApplicationStateType can be stored. */
typedef Os_ApplicationStateType* ApplicationStateRefType;

/* The ID type definition of the Application object. */
typedef uint16 Os_AppObjectIDType;
/* The ID of the Application object. */
typedef uint16 Os_AppObjectId;
typedef Os_AppObjectId AppObjectId;

/* OsApp_Hook type define, ref: 8.5 */
/* Type definition of Appl error hook. */
typedef P2FUNC(void, OS_APPL_CODE, Os_ApplErrorHookType)(StatusType Error);
/* Type definition of Appl shutdown hook. */
typedef P2FUNC(void, OS_APPL_CODE, Os_ApplShutdownHookType)(StatusType Fatalerror);
/* Appl Start Hook Type Definition. */
typedef P2FUNC(void, OS_APPL_CODE, Os_ApplStartupHookType)(void);

/* OsApplicationHooks 10.2.10 */
/* Ref: 8.5 */
/* If not cfg a hook, then the value is NULL_PTR. */
/* From the specfication, the non-trusted_app can not call ShutdownOS,
 * so that, non-trusted_app can not cfg specific-ShutdownHook. */
/* Type definition for Appl hook configuration. */
typedef struct
{
    Os_ApplErrorHookType OsAppErrorHook;
    Os_ApplShutdownHookType OsAppShutdownHook;
    Os_ApplStartupHookType OsAppStartupHook;
} Os_AppHookCfgType;

/* OsApplicationTrustedFunction 10.2.11 */
typedef struct
{
    Os_AppTrustedFunc OsTrustedFunctionName;
} Os_AppTrustedFunctionCfgType;

/* Os-Applications 10.2.9. */
/* The type definition of the Application object. */
typedef uint16 Os_AppObjectType; /*the reference map of the objects of App*/
/* The pointer type definition of the Application object. */
typedef Os_AppObjectType* Os_AppObjectRefType;

/* --------------------------memory protection----------------------------*/
/* os_kernel memory map. */

/*  */
typedef enum
{
    /*the checked space is one part of the stack*/
    /*    <    checked space     >   */
    /* <            stack           >*/
    STACK_INCLUDE_CHEKEDSPACE = 0U,

    /*the checked space cross the stack*/
    /* <  checked space    >*/
    /*    <  stack   >      */
    CHEKEDSPACE_INCLUDE_STACK = 1U,

    /*the checked space cross the stack*/
    /*<      checked space  >     */
    /*  <       stack           > */

    /*or*/
    /*<    checked space   >   */
    /*  <    stack            >*/
    STACK_CROSS_CHEKEDSPACE = 2U,

    /* the stack and checked space are not mixed*/
    /*<    checked space     >                        */
    /*                           <       stack       >*/
    STACK_NOTMIXED_CHEKEDSPACE = 3U
} Os_StackAndCheckedSpaceStatus;

/*  */
typedef enum
{
    PRO_IGNORE = 0U,
    PRO_TERMINATETASKISR = 1U,
    PRO_TERMINATEAPPL = 2U,
    PRO_TERMINATEAPPL_RESTART = 3U,
    PRO_SHUTDOWN = 4U
} Os_ProtectionReturnType;
/* This data type identifies a value which controls further actions of the OS on
return from the protection hook. */
typedef Os_ProtectionReturnType ProtectionReturnType;

/* 8.3.4 */
typedef uint16 Os_AccessType;
/* This type holds information how a specific memory region can be accessed. */
typedef Os_AccessType AccessType;
typedef AccessType* AccessRefType;

/* 8.3.7 */
typedef uint32 Os_MemoryStartAddressType;
/* This data type is a pointer which is able to point to any location in the MCU address space. */
typedef Os_MemoryStartAddressType MemoryStartAddressType;

/* 8.3.8 */
typedef uint32 Os_MemorySizeType;
/* This data type holds the size (in bytes) of a memory region. */
typedef Os_MemorySizeType MemorySizeType;

/*app address start and end*/
typedef struct
{
    uint8* APP_ADDR_START;
    uint8* APP_ADDR_END;
} OS_APP_ADDR;

/*ISR address start and end*/
typedef struct
{
    uint8* ISR_ADDR_START;
    uint8* ISR_ADDR_END;
} OS_ISR_ADDR;

/*task address start and end*/
typedef struct
{
    uint8* Task_ADDR_START;
    uint8* Task_ADDR_END;
} OS_TASK_ADDR;

/*RoData address start and end*/
typedef struct
{
    uint8* RoData_ADDR_START;
    uint8* RoData_ADDR_END;
} OS_RODATA_ADDR;

/* ---------------------------------spinlock----------------------------------*/
/* The TryToGetSpinlockType indicates if the spinlock has been occupied or not. */
/* DD_1_0049 */
typedef enum
{
    TRYTOGETSPINLOCK_NOSUCCESS = 0U,
    TRYTOGETSPINLOCK_SUCCESS = 1U,
} TryToGetSpinlockType;

/* Type definition of the spin lock method. */
/* DD_1_0103 */
typedef enum
{
    LOCK_ALL_INTERRUPTS = 0U,
    LOCK_CAT2_INTERRUPTS = 1U,
    LOCK_NOTHING = 2U,
    LOCK_WITH_RES_SCHEDULER = 3U,
} Os_SpinlockMethod;

/* SpinlockIdType identifies a spinlock instance and is used by the API functions:
GetSpinlock, ReleaseSpinlock and TryToGetSpinlock. */
typedef uint16 SpinlockIdType;
/* The type definition of the spin lock id. */
typedef SpinlockIdType Os_SpinlockIdType;
typedef uint32 Os_SpinlockType;
/* The type definition of the spin lock pointer. */
typedef Os_SpinlockType* Os_SpinlockRefType;

/* The type definition of the spin lock control block. */
typedef struct
{
    boolean occupied;
    Os_ObjectTypeType objOccupyType;
    uint16 objOccupyId;
} Os_SLCBType; /*Spinlock contronl block*/

/* The type definition of the spin lock config. */
typedef struct
{
    Os_SpinlockMethod SpinlockMethod;
    uint8 SpinlockGroupsId;
    uint32 SpinlockOrder;
} Os_SpinlockCfgType;

/* ----------------------------------multi-core----------------------------- */
/* This data type identifies the idle mode behavior. */
typedef enum
{
    OS_IDLE_NO_HALT = 0U,
    OS_RUN = 1U,
    OS_CORE_STATUS_INVALID = 2U
} Os_IdleModeType;

/* This data type identifies the idle mode behavior. */
typedef Os_IdleModeType IdleModeType;

/* Type definition of the CPU mode. */
typedef enum
{
    OS_CPUMODE_USER0 = 0U,
    OS_CPUMODE_USER1 = 1U,
    OS_CPUMODE_SUPERVISOR = 2U
} Os_CPUModeType;

/* Define the base structure of the memorysize */
typedef struct
{
    Os_CoreIdType coreStateActive;
    Os_CoreIdType coreStateSynPoint0[OS_AUTOSAR_CORES];
    Os_CoreIdType coreStateSynPoint1[OS_AUTOSAR_CORES];
    Os_CoreIdType coreStateSynPoint2[OS_AUTOSAR_CORES];
    Os_CoreIdType coreStateSynPoint3[OS_AUTOSAR_CORES];
    Os_AppModeType osAppMode[OS_AUTOSAR_CORES];
    Os_IdleModeType coreStatus[OS_AUTOSAR_CORES];
} Os_CoreCBType;

typedef enum
{
    RPC_SYNC = 0,
    RPC_ASYNC,
} RpcSyncType;

typedef enum
{
    RPC_FREE = 0U,
    RPC_START,
    RPC_WORK,
    RPC_DONE,
} RPC_SERVICE_STATE;

typedef struct
{
    RpcSyncType sync;
    Os_CoreIdType remoteCoreId;
    Os_ServiceIdType serviceId;
    uint32 srvPara0;
    uint32 srvPara1;
    uint32 srvPara2;
} RpcInputType;

typedef RpcInputType* RpcInputRefType;

typedef struct
{
    uint8 head;
    uint8 tail;
} LinkQueType;

typedef LinkQueType* LinkQueRefType;

typedef struct
{
    RpcSyncType sync;
    uint8 nextNode;
    volatile RPC_SERVICE_STATE procState;
    Os_CoreIdType sourceCoreId;
    StatusType retValue;
    Os_ServiceIdType serviceId;
    uint32 interParameter[3];
    uint32 extraParameter[4];
} srvNodeType;

typedef srvNodeType* srvNodeRefType;

#define RPC_MAX_NODE (10U)

typedef struct
{
    LinkQueType freeQ;
    LinkQueType workQ;
    srvNodeType srvStation[RPC_MAX_NODE];
} Os_RpcCoreType;

typedef uint8 RpcNodeIdType;

#define INVALID_TRAIN (0xFFU)

/*===========================I O C==================================*/

/* Define IOC data type. */
typedef uint8 Os_IocU8Type;
typedef uint16 Os_IocU16Type;

/* Function pointer of call-back in IOC. */
typedef P2FUNC(void, OS_APPL_CODE, Os_IocCallbackType)(void);

/* IOC Add parameter */
typedef Os_IocU16Type Os_IocComIdType;
typedef Os_IocU16Type Os_IocBlockIdType;
typedef Os_IocU8Type Os_IocBufferType;

typedef enum
{
    BLOCK_IDLE = 0U,
    BLOCK_WRITING = 1U,
    BLOCK_READY = 2U,
    BLOCK_READING = 3U,
    BLOCK_INVALID = 0xFFU,
} Os_IocBlockStateType;

/* Define the way of the core communication. */
typedef enum
{
    INTER_CORE_COM = 0U,
    CROSS_CORE_COM = 1U,

    COM_INVALID
} Os_IocCoreComType;

typedef struct
{
    Os_IocBlockStateType IocBlockState;
    union Os_IocBlockLink { /* PRQA S 0750 */ /* MISRA  Rule-19.2*/ /* OS_TYPES_UNION_001 */
        Os_IocBlockIdType IocBlockNext;
        Os_IocBlockIdType IocBlockCurrent;
    } IocBlockLink;
} Os_IocBlockType;

/* Define the type of send data source in IOC. */
typedef struct
{
    const void* IocDataPtr;
    Os_IocU16Type IocDataLenth;
} Os_IocSendDataSourceType;

/* Define the type of receive data source in IOC. */
typedef struct
{
    void* IocDataPtr;
    Os_IocU16Type* IocDataLenth;
} Os_IocReceiveDataSourceType;

/*===========================Peripheral==================================*/
/*peripheral access type definition*/
typedef uint16 AreaIdType;

/*=======Os configuration struct type definition==============================*/
/* Task configuration structure */
/* This struct type identifies the task configuration . */
typedef struct
{
    TaskEntry osTaskEntry;              /* Task entry */
    uint16 osTaskStackId;               /* Task stack id */
    uint16 osTaskActivation;            /* The maximum number of queued activation
                                           requests for the task*/
    uint16 osTaskPriority;              /* The priority of a task*/
    Os_TaskScheduleType osTaskSchedule; /* Defines the preemptability of the task */
    uint8 osRsv1;
    Os_AppModeType osTaskAutoStartMode; /* This container determines whether the task
                                           is activated during the system start-up
                                           procedure or not for some specific
                                           application modes */
/* AUTOSAR_SWS_OS.pdf 10.2.32: timing protection cfg for task. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    P2CONST(Os_TaskTmProtCfgType, OS_VAR, OS_CONST) osTaskTmProtCfgRef;
#endif
} Os_TaskCfgType;

/* Alarm configuration structure */
typedef struct
{
    Os_TickType osAlarmStartTime;
    Os_AlarmAutostartTypeType osAlarmAutostartType;
    Os_TickType osAlarmCycleTime;
    Os_AppModeType osAlarmAppMode;
} Os_AlarmAutostartCfgType;

/* This type of Alarm configuration. */
typedef struct
{
    Os_CounterType osAlarmCounter;
    P2CONST(Os_AlarmAutostartCfgType, OS_VAR, OS_CONST) osAlarmAutostartRef;
    Os_AlarmCallbackType osAlarmCallback;
} Os_AlarmCfgType;

/* Definition of resource configuration type. */
typedef struct
{
    Os_PriorityType ceiling;
    Os_ResourceOccupyType resourceOccupyType;
    uint8 rsv1;
} Os_ResourceCfgType;

/* OsIsr. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.17. */
/* Type definition of interrupt configuration. */
typedef struct
{
    Os_IsrCategoryType OsIsrCatType;
    boolean OsNestedEnable;
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    P2CONST(Os_IsrTimingProtectionType, OS_VAR, OS_CONST) OsIsrTimePt;
#endif

    uint32 OsIsrSrc;     /* Isr source register. Used in TerminateApplication. */
    uint32 OsIsrSrcType; /* Isr_src type, CPU0/CPU1/CPU2/DMA. */
} Os_IsrCfgType;

/* OsScheduleTable. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.22. */
/* This struct type identifies a schedule table config. */
typedef struct
{
    Os_TickType osSchedTblDuration;
    boolean osSchedTblRepeating;
    Os_CounterType osSchedTblCounterRef;
    P2CONST(Os_SchedTblAutostart, OS_VAR, OS_CONST) osSchedTblAutostartRef;
    P2CONST(Os_SchedTblEP, OS_VAR, OS_CONST) osSchedTblEP;
    uint16 osSchedTblEPsize;

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_SchedTblSync osSchedTblSync;
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */
} Os_SchedTblCfgType;

/* This type of Counter Configuration . */
typedef struct
{
    Os_TickType osCounterMaxAllowedValue;
    Os_TickType osCounterMinCycle;
    Os_TickType osCounterTicksPerBase;
    Os_CounterEnumType osCounterType;
    /* To check the correctness with the configuration */
    Os_TickType osSecondsPerTick;
} Os_CounterCfgType;
typedef Os_CounterCfgType* Os_CounterCfgRefType;

/*Define the structure of the app attribute configuration of the object*/
/* Define the structure of the app attribute configuration of the object */
typedef struct
{
    ApplicationType hostApp;                              /* owner app of the object */
    ApplicationType accAppRefNodeCnt;                     /* accAppRef array node count */
    P2CONST(ApplicationType, OS_VAR, OS_CONST) accAppRef; /* Access App reference bitmap list */
} Os_ObjectAppCfgType;

/* The structure configuration type definition of the Application. */
typedef struct
{
    CoreIdType OsHostCore; /* owner core of the app */
    boolean OsTrusted;     /* true: trusted; fasle: not trusted (default). */
    /* true: time protection is delayed
     * false: time protection is triggered immediately*/
    boolean OsTrustedApplicationDelayTimingViolationCall;
    /* true: OS-Application runs within a protected environment.
     * false: OS-Application has full write access (default)*/
    boolean OsTrustedAppWithProtection;
    uint16 OsAppTaskCnt;
    uint16 OsAppIsrRefCnt;
    uint16 OsAppAlarmRefCnt;
    uint16 OsAppCounterRefCnt;
    uint16 OsAppScheduleTableCnt;
    uint16 OsAppTrustedFuncCnt; /* Element count of OsAppTrustedFuncRef array */

#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    CONST(OS_APP_ADDR, OS_VAR) OsAppAssignedPeripheralAddr;
#endif
    P2CONST(Os_AppObjectRefType, OS_VAR, OS_CONST) OsAppObjectRef;
    CONST(Os_TaskType, OS_VAR) OsRestartTask;
    CONST(Os_AppHookCfgType, OS_VAR) OsApplicationHooks;
    P2CONST(Os_TrustedFunctionIndexType, OS_VAR, OS_CONST) OsAppTrustedFuncRef;
} Os_ApplicationCfgType;

typedef struct
{
    Os_IocU16Type IocSenderId;
    Os_ApplicationType IocSenderApplicationRef;
} Os_IocSenderPropertiesCfgType;

typedef struct
{
    Os_IocCallbackType IocCallBackFunc; /*Receive a callback function�? */
    Os_ApplicationType IocReceiverApplicationRef;
} Os_IocReceiverPropertiesCfgType;

typedef struct
{
    Os_IocU16Type IocDataPropertyIndex;
    boolean IocInitValueEnable;
    const void* IocInitValuePtr;
    Os_IocU16Type IocDataTypeLength;
} Os_IocDataPropertiesCfgType;

typedef struct
{
    /* Buffer Length*/
    boolean IocQueueEnable;
    Os_IocU16Type IocBufferLength;

    /* Sender Number */
    Os_IocU16Type IocSenderNumber;
    /* OsIocSenderProperties */
    const Os_IocSenderPropertiesCfgType* IocSenderProperties;

    /* Receiver Number */
    Os_IocU16Type IocReceiverNumber;
    /* OsIocReceiverProperties */
    const Os_IocReceiverPropertiesCfgType* IocReceiverProperties;

    /* Data Number */
    Os_IocU16Type IocDataNumber;
    /* OsIocDataProperties */
    const Os_IocDataPropertiesCfgType* IocDataProperties;

    /* Data Length */
    Os_IocU16Type IocDataTotalLength;
    /* Buffer Ptr */
    Os_IocBufferType* IocBufferPtr;
    /* Block Number */
    Os_IocU16Type IocBlockCnt;
    /* Block Ptr */
    Os_IocBlockType* IocBlockPtr;

    Os_IocCoreComType IocCfgComMark;
} Os_IocCommunicationCfgType;

typedef struct
{
    uint32 periAreaStartAddr;
    uint32 periAreaEndAddr;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    Os_ApplicationType periAreaAppAccMask;
#endif /* TRUE == CFG_SERVICE_PROTECTION_ENABLE */
} OsPeripheralAreaCfgType;
/* -------------------------------Trace--------------------------- */
typedef enum
{
    OS_TRACE_TASK_SWITCH_REASON_NONE,
    OS_TRACE_TASK_SWITCH_REASON_ERROR_TERMINATE,
    OS_TRACE_TASK_SWITCH_REASON_SPROT_ERROR_TERMINATE,
    OS_TRACE_TASK_SWITCH_REASON_PROT_HOOK_TERMINATE,
    OS_TRACE_TASK_SWITCH_REASON_TERMINATE,
    OS_TRACE_TASK_SWITCH_REASON_APP_TERMINATE,
    OS_TRACE_TASK_SWITCH_REASON_WAITEVENT_WAIT,
    OS_TRACE_TASK_SWITCH_REASON_SETEVENT_READY,
    OS_TRACE_TASK_SWITCH_REASON_PREEMPT_READY,
    OS_TRACE_TASK_SWITCH_REASON_RELEASE_RES_READY,
    OS_TRACE_TASK_SWITCH_REASON_CHAIN_READY,
    OS_TRACE_TASK_SWITCH_REASON_SCHEDULE_READY,
    OS_TRACE_TASK_SWITCH_REASON_WAITEVENT_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_SETEVENT_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_PREEMPT_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_RELEASE_RES_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_ERROR_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_PROT_HOOK_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_SPROT_ERROR_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_TERMINATE_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_APP_TERMINATE_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_CHAIN_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_SCHEDULE_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_POSITIVE_ACTIVE,
    OS_TRACE_TASK_SWITCH_REASON_START_ACTIVE
} Os_TraceTaskSwitchReasonType;

typedef enum
{
    OS_TRACE_TASK_STATE_TERMINATED,
    OS_TRACE_TASK_STATE_ACTIVED,
} Os_TraceTaskStateType;

typedef enum
{
    OS_TRACE_ISR_STATE_EXITED,
    OS_TRACE_ISR_STATE_ENTERED,
} Os_TraceIsrStateType;

typedef enum
{
    OS_TRACE_RES_STATE_UNLOCKED,
    OS_TRACE_RES_STATE_LOCKED
} Os_TraceResourceStateType;

typedef enum
{
    OS_TRACE_ALARM_STATE_STOPPED,
    OS_TRACE_ALARM_STATE_RUNNING
} Os_TraceAlarmStateType;

typedef struct
{
    Os_TraceTaskSwitchReasonType switchReason;
    Os_TraceTaskStateType state;
    Os_TaskStateType realState;
} Os_TraceTaskType;

typedef struct
{
    Os_TraceIsrStateType state;
    Os_TaskStateType realState;
} Os_TraceIsrType;

typedef struct
{
    Os_AppObjectId occupyObjectId;
    Os_TraceResourceStateType state;
} Os_TraceResourceType;

typedef struct
{
    Os_TraceAlarmStateType state;
} Os_TraceAlarmType;

#define OS_TRACE_NO_ISR_ID     (0xffffu)
#define OS_TRACE_NO_TASK_ID    (0xffffu)
#define OS_TRACE_NO_OBJECT_ID  (0xffffu)
#define OS_TRACE_NO_LAST_ERROR (0xffu)

typedef struct
{
    Os_CoreIdType coreId;
    Os_AppModeType curAppMode;
    Os_AppObjectType runObjId;
    Os_IsrType runIsrId;
    Os_TaskType runTaskId;
    Os_TaskType nextTaskId;
    Os_PriorityType runTaskPrio;
    Os_TraceTaskSwitchReasonType nextRunReason;
    Os_ServiceIdType curServiceId;
    StatusType lastError;
    Os_TraceIsrType* isrTrace;
    Os_TraceTaskType* taskTrace;
    Os_TraceResourceType* resourceTrace;
    Os_TraceAlarmType* alarmTrace;
} Os_TraceSysType;

/* --------------------------------others------------------------------*/
typedef struct
{
    Os_TaskType taskId;
    Os_TaskRefType taskRef;
    Os_TaskStateRefType taskState;
    CounterType counterId;
    Os_TickRefType counterValue;
    Os_TickRefType counterElapsedValue;
    Os_AlarmType alarmId;
    Os_AlarmBaseRefType alarmBaseRef;
    Os_TickRefType alarmTickRef;
    Os_TickType alarmTime;
    Os_TickType alarmCycle;
    Os_ResourceType resourceId;
    Os_EventMaskType eventMask;
    Os_EventMaskRefType eventRef;
    ScheduleTableType ScheduleTableId;
    ScheduleTableStatusRefType ScheduleStatus;
    TickType ScheduleTableOffset;
    TickType ScheduleTableStart;
    Os_IsrType ISRID;
    StatusType ShutdownError;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType ApplID;
    Os_AppObjectId ObjectID;
    Os_ObjectTypeType ObjectType;
    Os_RestartType RestartOption;
#endif
    Os_MemoryStartAddressType MemStartAddress;
    Os_MemorySizeType MemSize;

    ApplicationStateRefType appStateRef;
    CoreIdType CoreID;
    StatusType* Status;
    boolean ClearPending;

    uint16 Area;
    uint8* Address8;
    uint8* Value8;
    uint16* Address16;
    uint16* Value16;
    uint32* Address32;
    uint32* Value32;
    uint8 Clearmask8;
    uint8 Setmask8;

    uint16 Clearmask16;
    uint16 Setmask16;

    uint32 Clearmask32;
    uint32 Setmask32;
    TickType Offset;
    TickType Start;

    ScheduleTableType ScheduleTableID_From;
    ScheduleTableType ScheduleTableID_To;
    TickType value;

    uint16 SpinlockId;
    TrustedFunctionIndexType FunctionIndex;
    TrustedFunctionParameterRefType FunctionParams;

    uint32 Value;
    uint16 DataLenth;

    IdleModeType IdleMode;
    TryToGetSpinlockType* Success;

} Os_RunTimeSrvPrarmType;

/*=======Os Control Block struct type definitions ==========================================*/
/* Task control block type definitions*/
/* This struct type identifies Task control block type.  */
typedef struct
{
    Os_StackPtrType taskTop;
    Os_StackPtrType taskStackBottom;
    uint16 taskRunPrio;

#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
    uint8 taskActCount;
    uint8 taskSelfActCount;
#endif /* OS_BCC2 == CFG_CC || OS_ECC2 == CFG_CC */
    Os_TaskStateType taskState;

#if (CFG_STD_RESOURCE_MAX > 0U)
    uint8 taskResCount;
    Os_ResourceType* taskResourceStack; /* AutoSar: for protection hook. */
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    boolean taskTpFrameflag; /* Timing Protection: inter-arrival time. Init as true. */
    Os_TaskIsrOptType taskIsrOpt;
    Os_TmProtCbDataDef osTpTask[TP_OPT_TASK_BUTT];
#endif

/* Autosar: for service protection*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    uint8 taskDisableAllCount;
    uint8 taskSuspendAllCount;
    uint8 taskSuspendOSCount;
#endif

#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR)
    uint32 taskScheduleCount;
#endif

/*multi-core*/
#if ((CFG_SPINLOCK_MAX > 0U) || (CFG_STD_RESOURCE_MAX > 0U))
    uint16 taskCriticalZoneStack[CFG_CRITICAL_ZONE_MAX];
    uint16 taskCriticalZoneCount;
    Os_ObjectTypeType taskCriticalZoneType[CFG_CRITICAL_ZONE_MAX];
    VAR(volatile uint16, OS_VAR) taskCurrentSpinlockOccupyLevel;
#endif
} Os_TCBType;

/* This type of Alarm control block. */
typedef struct
{
    Os_TickType alarmStart;
    Os_TickType alarmCycle;
    Os_AlarmType alarmPre;
    Os_AlarmType alarmNext;
} Os_ACBType;

/* Type definition of resource control block. */
typedef struct
{
    uint8 saveCount;
    uint8 Rsv0;
    Os_PriorityType savePrio;
    Os_CallLevelType saveLevel;
/* Timing protection: Each resource has Os_TmProtCbDataDef. When it is locked,
   it should be processed by timing protection. */
/* Timing protection: ResourceLockBudget for task and isr. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_TASK_MAX > 0)
    Os_TickType* osTmProtResBgtTask;
#endif

#if (CFG_ISR2_MAX > 0)
    Os_TickType* osTmProtResBgtIsr;
#endif
    Os_TmProtCbDataDef osResTpData;
    /* Which task lock this resource. */
    Os_IsrType osWhichIsrOccupy;
    Os_TaskType osWhichTaskOccupy;
#endif
} Os_RCBType;

#if (CFG_ISR2_MAX > 0)
/* Add: AutoSar SC2: Isr control block type definitions. */
/* Type definition of interrupt control block. */
typedef struct
{
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    boolean osIsrTpFrameflag;
    Os_TaskIsrOptType IsrC2IsrOpt;
    Os_TmProtCbDataDef osTpIsr[TP_OPT_ISR_BUTT];
#endif

#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType IsrC2ResCount;
    Os_ResourceType* IsrC2ResourceStack; /* AutoSar: SC2 protection hook. */
#endif
    uint8 resevred; /*Avoid emptiness of the Os_ICBType*/

/* Autosar: for service protection*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    uint8 isrC2DisableAllCount;
    uint8 isrC2SuspendAllCount;
    uint8 isrC2SuspendOSCount;
#endif

#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR)
    uint32 isrScheduleCount;
#endif

/*multi-core*/
#if ((CFG_SPINLOCK_MAX > 0U) || (CFG_STD_RESOURCE_MAX > 0U))
    uint16 isr2CriticalZoneStack[CFG_CRITICAL_ZONE_MAX];
    uint16 isr2CriticalZoneCount;
    Os_ObjectTypeType isr2CriticalZoneType[CFG_CRITICAL_ZONE_MAX];
    VAR(uint16, OS_VAR) isrCurrentSpinlockOccupyLevel;
#endif
} Os_ICBType;
#endif /* CFG_ISR2_MAX */

/* Add: AutoSar SC2: Schedule table control block type definitions. */
/* This struct type identifies a schedule tablecontrol block . */
typedef struct tagOs_STCBType
{
    Os_ScheduleTableType stId;
    boolean stIsAdjust; /* stNextEP need adjust or not. */
    /* App call SetScheduleTableAsync, OS should stop Sync. */
    boolean stIsStopAdjust;
    boolean stIsAdjAdd;
    boolean stIsStarted;
    P2CONST(Os_SchedTblEP, AUTOMATIC, OS_VAR) stNextEP;
    Os_SchedTblStateType stState;
    /* Different between driver counter and sync counter. */
    Os_TickType stDiff;
    /* Deviation, might be smaller then stDiff. */
    Os_TickType stDev;
    Os_TickType stDelay;       /* Delay of stNextEP. */
    Os_TickType stAdjDevTotal; /* Total dev during sync process. */
    Os_TickType stStartAbsTick;
    Os_TickType stNextEpAbsTick; /* EP abs tick after adjust. */
    Os_TickType stFinalDelay;

    Os_ScheduleTableType NextStID; /* Used for next state. */
    Os_ScheduleTableType PrevStID; /* Used for next state. */

    struct tagOs_STCBType* stNextNode; /* For ScheduleTable list. */
    struct tagOs_STCBType* stPreNode;  /* For ScheduleTable list. */
    uint16 stEpId;                     /* ID of stNextEP. */
} Os_STCBType;

/* This type of Counter control block . */
/* DD_1_0076 */
typedef struct
{
    Os_TickType counterCurVal;
    Os_TickType counterLastVal;
    Os_AlarmType counterAlmQue;

    /* Add: AutoSar SC2: ScheduleTable list head node index reference to this counter. */
    P2VAR(Os_STCBType, OS_VAR, OS_VAR) counterStListHead;
} Os_CCBType;

/* The control block type definition for the Application. */
/* DD_1_0130 */
typedef struct
{
    ApplicationStateType appState;
} Os_APPCBType;

/* System control block type definitions */
/* DD_1_0102 */
typedef struct
{
    P2VAR(Os_TCBType, AUTOMATIC, OS_VAR) sysRunningTCB;
    Os_LockerType sysDispatchLocker;
    Os_PriorityType sysHighPrio;
    Os_TaskType sysHighTaskID;
    Os_TaskType sysRunningTaskID; /* No attached core ID, just local ID */
    Os_TaskType sysPrevTaskID;
    Os_AppModeType sysActiveAppMode;
    Os_CallLevelType sysOsLevel;
#if (TRUE == CFG_USEGETSERVICEID)
    Os_ServiceIdType sysOsServiceId;
#endif
#if (TRUE == CFG_USEPARAMETERACCESS)
    Os_RunTimeSrvPrarmType sysRunTimeSrvPrarm;
#endif

/* Timing protection for isr. */
#if (CFG_ISR2_MAX > 0)
    boolean sysInIsrCat2;
    Os_IsrType sysRunningIsrCat2Id; /* Running isr category 2. */
    P2VAR(Os_IsrType, AUTOMATIC, OS_VAR) sysIsrNestQueue;
#endif

#if (CFG_OSAPPLICATION_MAX > 0)
    Os_ApplicationType sysRunningAppID; /*the running application ID*/
    ObjectTypeType sysRunningAppObj;    /*the running object from the application*/
#endif

    Os_CoreIdType sysCore; /*logical CoreID*/

    /*Alarm module*/
    VAR(Os_AlarmType, OS_VAR) sysAlarmMax;

    /*Task module*/
    VAR(uint16, OS_VAR) sysTaskMax;

    VAR(ApplicationType, AUTOMATIC) sysAppId;

#if (CFG_SPINLOCK_MAX > 0)
    boolean CurrentSpinlockOccupied[CFG_SPINLOCK_MAX];
#endif

    /*Trace*/
#if (TRUE == CFG_TRACE_ENABLE)
    P2VAR(Os_TraceSysType, AUTOMATIC, OS_VAR) trace;
#endif
} Os_SCBType;

typedef struct
{
    Os_IocBlockType* IocBlockPtr;
    Os_IocBufferType* IocBufferPtr;
    Os_IocU16Type IocDataElementSizeWithHeader;
    Os_IocU16Type IocBlockLength;
    boolean IocLostData;
    /* QEUEU */
    Os_IocBlockIdType IocBlockHead;
    Os_IocBlockIdType IocBlockTail;
    Os_IocBlockIdType IocBlockFreeHead;
    /* UNQEUEU */
    Os_IocBlockIdType IocBlockRead;
} Os_IocCBType;

#endif               /* OS_TYPES_H */
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */

/*=======[E N D   O F   F I L E]==============================================*/
