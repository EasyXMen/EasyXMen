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
#include "Os_Macros.h"
#include "Os_Cfg.h"
#include "Os_ECode.h"
/*=======[M A C R O S]========================================================*/

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* --------------------system-------------------- */
/* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    /*AUTOSAR Service*/
    OSServiceId_GetApplicationID           = 0x00U,
    OSServiceId_GetISRID                   = 0x01U,
    OSServiceId_CallTrustedFunction        = 0x02U,
    OSServiceId_CheckISRMemoryAccess       = 0x03U,
    OSServiceId_CheckTaskMemoryAccess      = 0x04U,
    OSServiceId_CheckObjectAccess          = 0x05U,
    OSServiceId_CheckObjectOwnership       = 0x06U,
    OSServiceId_StartScheduleTableRel      = 0x07U,
    OSServiceId_StartScheduleTableAbs      = 0x08U,
    OSServiceId_StopScheduleTable          = 0x09U,
    OSServiceId_NextScheduleTable          = 0x0aU,
    OSServiceId_StartScheduleTableSynchron = 0x0bU,
    OSServiceId_SyncScheduleTable          = 0x0cU,
    OSServiceId_SetScheduleTableAsync      = 0x0dU,
    OSServiceId_GetScheduleTableStatus     = 0x0eU,
    OSServiceId_IncrementCounter           = 0x0fU,
    OSServiceId_GetCounterValue            = 0x10U,
    OSServiceId_GetElapsedValue            = 0x11U,
    OSServiceId_TerminateApplication       = 0x12U,
    OSServiceId_AllowAccess                = 0x13U,
    OSServiceId_GetApplicationState        = 0x14U,
    OSServiceId_GetNumberOfActivatedCores  = 0x15U,
    OSServiceId_GetCoreID                  = 0x16U,
    OSServiceId_StartCore                  = 0x17U,
    OSServiceId_StartNonAutosarCore        = 0x18U,
    OSServiceId_GetSpinlock                = 0x19U,
    OSServiceId_ReleaseSpinlock            = 0x1aU,
    OSServiceId_TryToGetSpinlock           = 0x1bU,
    OSServiceId_ShutdownAllCores           = 0x1cU,
    OSServiceId_ControlIdle                = 0x1dU,
    OSServiceId_IocSend                    = 0x1eU,
    OSServiceId_IocWrite                   = 0x1fU,
    OSServiceId_IocSendGroup               = 0x20U,
    OSServiceId_IocWriteGroup              = 0x21U,
    OSServiceId_IocReceive                 = 0x22U,
    OSServiceId_IocRead                    = 0x23U,
    OSServiceId_IocReceiveGroup            = 0x24U,
    OSServiceId_IocReadGroup               = 0x25U,
    OSServiceId_IocEmptyQueue              = 0x26U,
    OSServiceId_GetCurrentApplicationID    = 0x27U,
    OSServiceId_ReadPeripheral8            = 0x28U,
    OSServiceId_ReadPeripheral16           = 0x29U,
    OSServiceId_ReadPeripheral32           = 0x2aU,
    OSServiceId_WritePeripheral8           = 0x2bU,
    OSServiceId_WritePeripheral16          = 0x2cU,
    OSServiceId_WritePeripheral32          = 0x2dU,
    OSServiceId_ModifyPeripheral8          = 0x2eU,
    OSServiceId_ModifyPeripheral32         = 0x2fU,
    OSServiceId_DisableInterruptSource     = 0x30U,
    OSServiceId_EnableInterruptSource      = 0x31U,
    OSServiceId_ClearPendingInterrupt      = 0x32U,
    OSServiceId_ActivateTaskAsyn           = 0x33U,
    OSServiceId_SetEventAsyn               = 0x34U,
    OSServiceId_ModifyPeripheral16         = 0x35U,

    /* Currently no service execution */
    OSServiceId_NoServiceRunning = 0x44U,

    /*Additional Service*/
    OSServiceId_WaitAllEvents     = 0xD7U,
    OSServiceId_IocCallBackNotify = 0xDEU,

    /*OSEK Service*/
    OSServiceId_ActivateTask             = 0xDFU,
    OSServiceId_TerminateTask            = 0xE0U,
    OSServiceId_ChainTask                = 0xE1U,
    OSServiceId_Schedule                 = 0xE2U,
    OSServiceId_GetTaskID                = 0xE3U,
    OSServiceId_GetTaskState             = 0xE4U,
    OSServiceId_EnableAllInterrupts      = 0xE5U,
    OSServiceId_DisableAllInterrupts     = 0xE6U,
    OSServiceId_ResumeAllInterrupts      = 0xE7U,
    OSServiceId_SuspendAllInterrupts     = 0xE8U,
    OSServiceId_ResumeOSInterrupts       = 0xE9U,
    OSServiceId_SuspendOSInterrupts      = 0xEAU,
    OSServiceId_DeclareResource          = 0xEBU,
    OSServiceId_GetResource              = 0xECU,
    OSServiceId_ReleaseResource          = 0xEDU,
    OSServiceId_DeclareEvent             = 0xEEU,
    OSServiceId_SetEvent                 = 0xEFU,
    OSServiceId_ClearEvent               = 0xF0U,
    OSServiceId_GetEvent                 = 0xF1U,
    OSServiceId_WaitEvent                = 0xF2U,
    OSServiceId_GetAlarmBase             = 0xF3U,
    OSServiceId_GetAlarm                 = 0xF4U,
    OSServiceId_SetRelAlarm              = 0xF5U,
    OSServiceId_SetAbsAlarm              = 0xF6U,
    OSServiceId_CancelAlarm              = 0xF7U,
    OSServiceId_GetActiveApplicationMode = 0xF8U,
    OSServiceId_StartOS                  = 0xF9U,
    OSServiceId_ShutdownOS               = 0xFAU,
    OSServiceId_ErrorHook                = 0xFBU,
    OSServiceId_PreTaskHook              = 0xFCU,
    OSServiceId_PostTaskHook             = 0xFDU,
    OSServiceId_StartupHook              = 0xFEU,
    OSServiceId_ShutdownHook             = 0xFFU
} Os_ServiceIdType;
/* PRQA S 1271 -- */

/* definition of Os level of call API, add some for service protection in SC3 and SC4 */
typedef enum
{
    OS_LEVEL_MAIN              = 0,
    OS_LEVEL_ERRORHOOK_APP     = 1,
    OS_LEVEL_TASK              = 2,
    OS_LEVEL_ISR1              = 3,
    OS_LEVEL_ISR2              = 4,
    OS_LEVEL_ERRORHOOK         = 5,
    OS_LEVEL_PRETASKHOOK       = 6,
    OS_LEVEL_POSTTASKHOOK      = 7,
    OS_LEVEL_STARTUPHOOK       = 8,
    OS_LEVEL_SHUTDOWNHOOK      = 9,
    OS_LEVEL_ALARMCALLBACK     = 10,
    OS_LEVEL_PROTECTIONHOOK    = 11,
    OS_LEVEL_STANDARD_RESOURCE = 12,
    OS_LEVEL_INTERNAL_RESOURCE = 13
} Os_CallLevelType;

/* Type definition of the App mode. */
typedef uint16         Os_AppModeType;
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
    OBJECT_TASK          = 0,
    OBJECT_ISR           = 1,
    OBJECT_ALARM         = 2,
    OBJECT_COUNTER       = 3,
    OBJECT_SCHEDULETABLE = 4,
    OBJECT_APP_MAX       = 5,

    OBJECT_RESOURCE = 5,
    OBJECT_SPINLOCK = 6,
    OBJECT_MAX      = 7 /*The Max Value is invalid, just used in coding*/
} Os_ObjectTypeType;

typedef Os_ObjectTypeType ObjectTypeType;

/* The type definition of the core id. */
typedef uint16 Os_CoreIdType;

/* The pointer type definition of the core id. */
typedef Os_CoreIdType* Os_CoreIdRefType;

/* CoreIdType is a scalar that allows identifying a single core.
The CoreIdType shall represent the logical CoreID. */
typedef Os_CoreIdType  CoreIdType;
typedef Os_CoreIdType* CoreIdRefType;

/* The type definition of the Application. */
typedef uint16 Os_ApplicationType;

/* This data type identifies the OS-Application. */
typedef Os_ApplicationType ApplicationType;

/* Type definition of OS resources. */
typedef uint16          Os_ResourceType;
typedef Os_ResourceType ResourceType;
typedef struct
{
    uint32 queueHead;
    uint32 queueTail;
} Os_ReadyQueueType;

typedef enum
{
    SHUTDOWN_OS     = 0,
    SHUTDOWN_ALL_OS = 1
} Os_ShutdownAction;
/* -----------------------------task---------------------- */
/* This enum type identifies Task State. */
typedef enum
{
    TASK_STATE_WAITING   = 0,
    TASK_STATE_READY     = 1,
    TASK_STATE_SUSPENDED = 2,
    TASK_STATE_RUNNING   = 3,
    /*for distinguish the autostarted task,the first activatedtask and preempted task */
    TASK_STATE_START = 4
} Os_TaskStateType;

typedef Os_TaskStateType  TaskStateType;
typedef Os_TaskStateType* TaskStateRefType;
typedef Os_TaskStateType* Os_TaskStateRefType;
typedef uint16            Os_TaskType;
typedef Os_TaskType*      Os_TaskRefType;
typedef Os_TaskType       TaskType;
typedef Os_TaskType*      TaskRefType;
typedef uint8             Os_TaskScheduleType;
typedef uint16            Os_PriorityType;

typedef void (*TaskEntry)(void);

/* ------------------------Alarm------------------------- */
typedef enum
{
    ALARM_AUTOSTART_ABSOLUTE = 0,
    ALARM_AUTOSTART_RELATIVE = 1
} Os_AlarmAutostartTypeType;

/* This type of tick. */
typedef uint32       Os_TickType;
typedef Os_TickType* Os_TickRefType;
typedef Os_TickType  TickType;
typedef TickType*    TickRefType;
typedef uint16       Os_CounterType;
/* This data type identifies a counter. */
typedef Os_CounterType CounterType;

/* This type of Alarm. */
typedef uint16        Os_AlarmType;
typedef Os_AlarmType* Os_AlarmRefType;
typedef Os_AlarmType  AlarmType;
typedef Os_AlarmType* AlarmRefType;
typedef void (*Os_AlarmCallbackType)(void);

/* To unify with conformance testcases */
/* DD_1_1111 */
typedef struct
{
    Os_TickType maxallowedvalue;
    Os_TickType ticksperbase;
    Os_TickType mincycle;
} Os_AlarmBaseType;
typedef Os_AlarmBaseType  AlarmBaseType; /*to unify with conformance testcases*/
typedef Os_AlarmBaseType* Os_AlarmBaseRefType;
typedef Os_AlarmBaseType* AlarmBaseRefType;

/* ---------------------------resource----------------------- */

/* Definition of resource occupancy type. */
typedef enum
{
    OCCUPIED_BY_TASK              = 0, /*occupied by task*/
    OCCUPIED_BY_INTERRUPT         = 1, /*occupied by interrupt*/
    OCCUPIED_BY_TASK_OR_INTERRUPT = 2  /*occupied by task or interrupt*/
} Os_ResourceOccupyType;

typedef struct
{
    Os_ResourceType* TaskResourceStackPtr;
    uint16           ResourceNum;
} TaskResourceStackType;

/* ------------------------------Event---------------------------- */
/* This type of event. */
typedef uint64       Os_EventType;
typedef Os_EventType EventType;

/* This type of event mask. */
typedef uint64            Os_EventMaskType;
typedef Os_EventMaskType* Os_EventMaskRefType;
typedef Os_EventMaskType  EventMaskType;
typedef EventMaskType*    EventMaskRefType;

/* This struct type identifies the Event control block. */
typedef struct
{
    Os_EventMaskType eventSetEvent;
    Os_EventMaskType eventWaitEvent;
    boolean          eventIsWaitAllEvents;
} Os_ECBType;

/* -----------------------------Isr------------------------------ */
/* Definition of OS interrupt category. */
typedef enum
{
    OS_ISR_CATEGORY1 = 0,
    OS_ISR_CATEGORY2 = 1
} Os_IsrCategoryType;

typedef enum
{
    OS_ISR_DISABLED = 0,
    OS_ISR_ENABLED  = 1
} Os_IsrStateType;

/* interrupt priority */
typedef uint16 Os_IPLType;
typedef uint16 Os_IsrType;
/* This data type identifies an interrupt service routine (ISR). */
typedef Os_IsrType ISRType;
typedef enum
{
    OS_ISR_ENABLE  = 0,
    OS_ISR_DISABLE = 1
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
    SCHEDULETABLE_STOPPED                 = 0,
    SCHEDULETABLE_NEXT                    = 1,
    SCHEDULETABLE_WAITING                 = 2,
    SCHEDULETABLE_RUNNING                 = 3,
    SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS = 4
} Os_SchedTblStateType;
typedef Os_SchedTblStateType ScheduleTableStatusType;
/* This data type points to a variable of the data type ScheduleTableStatusType. */
typedef ScheduleTableStatusType* ScheduleTableStatusRefType;

typedef enum
{
    ST_START_ABSOLUTE = 0,
    ST_START_RELATIVE = 1,

#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    ST_START_SYNCHRON = 2U /* PRQA S 1271 */ /* VL_Os_1271 */
#endif
} Os_SchedTblAutostartType;
typedef uint16 Os_ScheduleTableType;
/* This data type identifies a schedule table. */
typedef Os_ScheduleTableType     ScheduleTableType;
typedef uint16                   Os_SchedTblSetEvent;
typedef Os_SchedTblSetEvent*     Os_SchedTblSetEventRef;
typedef uint16                   Os_SchedTblSetEventTask;
typedef Os_SchedTblSetEventTask* Os_SchedTblSetEventTaskRef;
typedef uint16                   Os_SchedTblActivateTask;
typedef Os_SchedTblActivateTask* Os_SchedTblActivateTaskRef;

/* This type of Counter . */
typedef Os_CounterType Os_SchedTblCounterRefType;

/* OsScheduleTableAutostart. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.23. */
/* This struct type identifies a schedule tableAutostart . */
typedef struct
{
    Os_TickType              osSchedTblAbsValue;
    Os_TickType              osSchedTblRelOffset;

    /* Note: In AUTOSAR_SWS_OS.pdf 10.2.23, Os_SchedTblAutostart can reference more then one appmode. */
    Os_AppModeType osSchedTblAppModeBitmap;
    Os_SchedTblAutostartType osSchedTblAutostartType;
} Os_SchedTblAutostart;

/* OsScheduleTableSync. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.28. */
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
/* Enumeration type of synchronization policy. */
/* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
    ST_SYNC_EXPLICIT = 0U,
    ST_SYNC_IMPLICIT = 1U,
    ST_SYNC_NONE     = 2U
} Os_SchedTblSyncStrategy;
/* PRQA S 1271 -- */

/* Schedule table synchronization type. */
typedef struct
{
    uint16                  osSchedTblExplicitPrecision;
    Os_SchedTblSyncStrategy osSchedTblSyncStrategy;
} Os_SchedTblSync;
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */

/* OsScheduleTableEventSetting. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.24. */
/* This struct type identifies the reference event to set. */
typedef struct
{
    Os_EventType osSchedTblSetEventRef;
    Os_TaskType  osSchedTblSetEventTaskRef;
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
    const Os_EPSetEventAction*     osSetEventList;
    const Os_EPActivateTaskAction* osActivateTaskList;
    Os_TickType                    osSchedTblEPOffset;
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_TickType osSchedTblMaxAdvance;
    Os_TickType osSchedTblMaxRetard;
#endif
    uint16                         osSetEventListSize;
    uint16                         osActivateTaskListSize;
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
    TP_SUS_OS_INT  = 0,
    TP_SUS_ALL_INT = 1,
    TP_DIS_ALL_INT = 2,

    TP_OPT_BUTT = 3
} Os_TaskIsrOptType;

/* Add: AutoSar SC2: Time protection operation type definitions. */
/* Task timing protection operation type definition. */
typedef enum
{
    TP_TASK_EXE         = 0,
    TP_TASK_ARRIVAL     = 1,
    TP_TASK_SUS_OS_INT  = 2,
    TP_TASK_SUS_ALL_INT = 3,
    TP_TASK_DIS_ALL_INT = 4,

    TP_OPT_TASK_BUTT = 5 /* Attention: used as index of array! */
} Os_TmProtOptTaskType;

/* Interrupt timing protection operation type definition. */
typedef enum
{
    TP_ISR_CAT2_EXE         = 0,
    TP_ISR_CAT2_ARRIVAL     = 1,
    TP_ISR_CAT2_SUS_OS_INT  = 2,
    TP_ISR_CAT2_SUS_ALL_INT = 3,
    TP_ISR_CAT2_DIS_ALL_INT = 4,

    TP_OPT_ISR_BUTT = 5 /* Attention: used as index of array! */
} Os_TmProtOptIsrType;

/* Timing protection: who occupy a resource, task or isr. */
/* Type definition of resource occupation. */
typedef enum
{
    TP_RES_OCCUPY_TASK = 0,
    TP_RES_OCCUPY_ISR  = 1,

    TP_RES_OCCUPY_BUTT = 2
} Os_TmProtResOccupyType;

/* Define resource for task locking */
typedef struct
{
    Os_TickType     OsTaskResLockBudget;
    Os_ResourceType OsTaskResLockResRef;
} Os_TaskResLockType;

/* Task timing protection configuration structure type. */
typedef struct
{
    const Os_TaskResLockType* osTaskResLockRef;
    Os_TickType               osTaskAllInterruptLockBudget; /* Float. Cfg tool switch to tick. */
    Os_TickType               osTaskExecutionBudget;        /* Float. Cfg tool switch to tick. */
    Os_TickType               osTaskOsInterruptLockBudget;  /* Float. Cfg tool switch to tick. */
    Os_TickType               osTaskTimeFrame;              /* Float. Cfg tool switch to tick. */
    uint16                    osTaskResLockCnt;             /* Count of Os_TaskResLockType. */
} Os_TaskTmProtCfgType;

/* OsIsrTimingProtection.  AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.19. */
/* Resources that define interrupt locking. */
typedef struct
{
    Os_TickType     osIsrResLockBudget; /* Float. Cfg tool switch to tick count. */
    Os_ResourceType osIsrResLockResRef;
} Os_IsrResLockType;

/* Configuration type of interrupt timing protection configuration. */
typedef struct
{
    const Os_IsrResLockType* osIsrResLockRef;
    Os_TickType              osIsrAllIptLockBudget; /* Float. Cfg tool switch to tick count. */
    Os_TickType              osIsrExeBudget;        /* Float. Cfg tool switch to tick count. */
    Os_TickType              osIsrOsIptLockBudget;  /* Float. Cfg tool switch to tick count. */
    Os_TickType              osIsrTimeFrame;        /* Float. Cfg tool switch to tick count. */
    uint16                   osIsrResLockCnt;       /* Count of Os_IsrResLockType. */
} Os_IsrTimingProtectionType;

/* Timing protection control block data definition. */
typedef struct
{
    Os_TickType osTpTime;
    Os_TickType osTpBudget;
    boolean osIsTpStart;
    uint8   osWhoHook;
} Os_TmProtCbDataDef;
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */
/* -----------------------TrustedFunction-------------------------*/
/* 8.3.2 */
/* Define the base structure of the Os_TrustedFunctionIndex. */
typedef uint16 Os_TrustedFunctionIndexType;

/* This data type identifies a trusted function. */
typedef Os_TrustedFunctionIndexType TrustedFunctionIndexType;

/* This data type points to a structure which holds the arguments for a call to a trusted function. */
typedef void* TrustedFunctionParameterRefType;

/* Used in Os_AppTrustedFunctionCfgType */
typedef void (*Os_AppTrustedFunc)(TrustedFunctionIndexType ix, TrustedFunctionParameterRefType ref);

typedef struct
{
    uint32     osWhoHook;
    boolean    TrustedFuncTportDelayCall;
    boolean    TrustedFuncTporFlag;
    StatusType TrustedFuncTporErrType;
} Os_TrustedFuncTpDataDef;
/* ------------------------application---------------------------- */
/* 8.3.5 */
/* This data type identifies if an OS-Application has access to an object. */
typedef enum
{
    ACCESS    = 0,
    NO_ACCESS = 1
} ObjectAccessType;

/* 8.3.15 */
/* This data type defines the use of a Restart Task after terminating an OSApplication. */
typedef enum
{
    RESTART    = 0,
    NO_RESTART = 1
} Os_RestartType;
/* This data type defines the use of a Restart Task after terminating an OS-Application. */
typedef Os_RestartType RestartType;

/* This data type identifies the state of an OS-Application. */
typedef enum
{
    APPLICATION_ACCESSIBLE = 0,
    APPLICATION_RESTARTING = 1,
    APPLICATION_TERMINATED = 2
} Os_ApplicationStateType;

/* The type definition of the pointer to the Application state. */
typedef Os_ApplicationStateType* Os_ApplicationStateRefType;
typedef Os_ApplicationStateType  ApplicationStateType;

/* This data type points to location where a ApplicationStateType can be stored. */
typedef Os_ApplicationStateType* ApplicationStateRefType;

/* The ID type definition of the Application object. */
typedef uint16 Os_AppObjectIDType;
/* The ID of the Application object. */
typedef uint16         Os_AppObjectId;
typedef Os_AppObjectId AppObjectId;

/* OsApp_Hook type define, ref: 8.5 */
/* Type definition of Appl error hook. */
typedef void (*Os_ApplErrorHookType)(StatusType Error);
/* Type definition of Appl shutdown hook. */
typedef void (*Os_ApplShutdownHookType)(StatusType Fatalerror);
/* Appl Start Hook Type Definition. */
typedef void (*Os_ApplStartupHookType)(void);

/* OsApplicationHooks 10.2.10 */
/* Ref: 8.5 */
/* If not cfg a hook, then the value is NULL_PTR. */
/* From the specfication, the non-trusted_app can not call ShutdownOS,
 * so that, non-trusted_app can not cfg specific-ShutdownHook. */
/* Type definition for Appl hook configuration. */
typedef struct
{
    Os_ApplErrorHookType    OsAppErrorHook;
    Os_ApplShutdownHookType OsAppShutdownHook;
    Os_ApplStartupHookType  OsAppStartupHook;
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
    STACK_INCLUDE_CHEKEDSPACE = 0,

    /*the checked space cross the stack*/
    /* <  checked space    >*/
    /*    <  stack   >      */
    CHEKEDSPACE_INCLUDE_STACK = 1,

    /*the checked space cross the stack*/
    /*<      checked space  >     */
    /*  <       stack           > */

    /*or*/
    /*<    checked space   >   */
    /*  <    stack            >*/
    STACK_CROSS_CHEKEDSPACE = 2,

    /* the stack and checked space are not mixed*/
    /*<    checked space     >                        */
    /*                           <       stack       >*/
    STACK_NOTMIXED_CHEKEDSPACE = 3
} Os_StackAndCheckedSpaceStatus;

/*  */
typedef enum
{
    PRO_IGNORE                = 0,
    PRO_TERMINATETASKISR      = 1,
    PRO_TERMINATEAPPL         = 2,
    PRO_TERMINATEAPPL_RESTART = 3,
    PRO_SHUTDOWN              = 4
} Os_ProtectionReturnType;
/* This data type identifies a value which controls further actions of the OS on
return from the protection hook. */
typedef Os_ProtectionReturnType ProtectionReturnType;

/* 8.3.4 */
typedef uint16 Os_AccessType;
/* This type holds information how a specific memory region can be accessed. */
typedef Os_AccessType AccessType;
typedef AccessType*   AccessRefType;

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
    TRYTOGETSPINLOCK_NOSUCCESS = 0,
    TRYTOGETSPINLOCK_SUCCESS   = 1
} TryToGetSpinlockType;

/* Type definition of the spin lock method. */
/* DD_1_0103 */
typedef enum
{
    LOCK_ALL_INTERRUPTS     = 0,
    LOCK_CAT2_INTERRUPTS    = 1,
    LOCK_NOTHING            = 2,
    LOCK_WITH_RES_SCHEDULER = 3
} Os_SpinlockMethod;

/* SpinlockIdType identifies a spinlock instance and is used by the API functions:
GetSpinlock, ReleaseSpinlock and TryToGetSpinlock. */
typedef uint16 SpinlockIdType;
/* The type definition of the spin lock id. */
typedef SpinlockIdType Os_SpinlockIdType;
typedef uint32         Os_SpinlockType;
/* The type definition of the spin lock pointer. */
typedef Os_SpinlockType* Os_SpinlockRefType;

/* The type definition of the spin lock control block. */
typedef struct
{
    uint16            objOccupyId;
    boolean           occupied;
    Os_ObjectTypeType objOccupyType;
} Os_SLCBType; /*Spinlock contronl block*/

/* The type definition of the spin lock config. */
typedef struct
{
    uint32            SpinlockOrder;
    Os_SpinlockMethod SpinlockMethod;
    uint8             SpinlockGroupsId;
} Os_SpinlockCfgType;

/* ----------------------------------multi-core----------------------------- */
/* This data type identifies the idle mode behavior. */
typedef enum
{
    OS_IDLE_NO_HALT        = 0,
    OS_RUN                 = 1,
    OS_CORE_STATUS_INVALID = 2
} Os_IdleModeType;

/* This data type identifies the idle mode behavior. */
typedef Os_IdleModeType IdleModeType;

/* Type definition of the CPU mode. */
typedef enum
{
    OS_CPUMODE_USER0      = 0,
    OS_CPUMODE_USER1      = 1,
    OS_CPUMODE_SUPERVISOR = 2
} Os_CPUModeType;

/* Define the base structure of the memorysize */
typedef struct
{
    Os_CoreIdType   coreStateActive;
    Os_CoreIdType   coreStateSynPoint0[OS_AUTOSAR_CORES];
    Os_CoreIdType   coreStateSynPoint1[OS_AUTOSAR_CORES];
    Os_CoreIdType   coreStateSynPoint2[OS_AUTOSAR_CORES];
    Os_CoreIdType   coreStateSynPoint3[OS_AUTOSAR_CORES];
    Os_AppModeType  osAppMode[OS_AUTOSAR_CORES];
    Os_IdleModeType coreStatus[OS_AUTOSAR_CORES]; /* PRQA S 1039 */ /* VL_Os_1039 */
} Os_CoreCBType;

typedef enum
{
    RPC_SYNC  = 0,
    RPC_ASYNC = 1
} RpcSyncType;

typedef enum
{
    RPC_FREE  = 0,
    RPC_START = 1,
    RPC_WORK  = 2,
    RPC_DONE  = 3
} RPC_SERVICE_STATE;

typedef struct
{
    uint32           srvPara0;
    uint32           srvPara1;
    uint32           srvPara2;
    Os_CoreIdType    remoteCoreId;
    RpcSyncType      sync;
    Os_ServiceIdType serviceId;
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
    uint32                     interParameter[3]; /* PRQA S 3132 */ /* VL_Os_3132 */
    uint32                     extraParameter[4]; /* PRQA S 3132 */ /* VL_Os_3132 */
    Os_CoreIdType              sourceCoreId;
    RpcSyncType                sync;
    uint8                      nextNode;
    volatile RPC_SERVICE_STATE procState;
    StatusType                 retValue;
    Os_ServiceIdType           serviceId;
} srvNodeType;

typedef srvNodeType* srvNodeRefType;

#define RPC_MAX_NODE (10U)

typedef struct
{
    srvNodeType srvStation[RPC_MAX_NODE];
    LinkQueType freeQ;
    LinkQueType workQ;
} Os_RpcCoreType;

typedef uint8 RpcNodeIdType;

#define INVALID_TRAIN (0xFFU)

/*===========================I O C==================================*/

/* Define IOC data type. */
typedef uint8  Os_IocU8Type;
typedef uint16 Os_IocU16Type;

/* Function pointer of call-back in IOC. */
typedef void (*Os_IocCallbackType)(void);

/* IOC Add parameter */
typedef Os_IocU16Type Os_IocComIdType;
typedef Os_IocU16Type Os_IocBlockIdType;
typedef Os_IocU8Type  Os_IocBufferType;

typedef enum
{
    BLOCK_IDLE    = 0,
    BLOCK_WRITING = 1,
    BLOCK_READY   = 2,
    BLOCK_READING = 3,
    BLOCK_INVALID = 0xFFU /* PRQA S 1271 */ /* VL_Os_1271 */
} Os_IocBlockStateType;

/* Define the way of the core communication. */
typedef enum
{
    INTER_CORE_COM = 0,
    CROSS_CORE_COM = 1,

    COM_INVALID = 2
} Os_IocCoreComType;

typedef struct
{
    /* PRQA S 0750 ++ */ /* VL_Os_0750 */
    union Os_IocBlockLink
    {
        Os_IocBlockIdType IocBlockNext;
        Os_IocBlockIdType IocBlockCurrent;
    } IocBlockLink;
    /* PRQA S 0750 -- */
    Os_IocBlockStateType IocBlockState;
} Os_IocBlockType;

/* Define the type of send data source in IOC. */
typedef struct
{
    const void*   IocDataPtr;
    Os_IocU16Type IocDataLenth;
} Os_IocSendDataSourceType;

/* Define the type of receive data source in IOC. */
typedef struct
{
    void*          IocDataPtr;
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
    /* AUTOSAR_SWS_OS.pdf 10.2.32: timing protection cfg for task. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    const Os_TaskTmProtCfgType* osTaskTmProtCfgRef;
#endif
    uint16    osTaskStackId;            /* Task stack id */
    uint16    osTaskActivation;         /* The maximum number of queued activation
                                           requests for the task*/
    uint16              osTaskPriority; /* The priority of a task*/
    Os_AppModeType      osTaskAutoStartMode; /* This container determines whether the task
                                                is activated during the system start-up
                                                procedure or not for some specific
                                                application modes */
    Os_TaskScheduleType osTaskSchedule; /* Defines the preemptability of the task */
} Os_TaskCfgType;

/* Alarm configuration structure */
typedef struct
{
    Os_TickType               osAlarmStartTime;
    Os_TickType               osAlarmCycleTime;
    Os_AppModeType            osAlarmAppMode;
    Os_AlarmAutostartTypeType osAlarmAutostartType;
} Os_AlarmAutostartCfgType;

/* This type of Alarm configuration. */
typedef struct
{
    const Os_AlarmAutostartCfgType* osAlarmAutostartRef;
    Os_AlarmCallbackType            osAlarmCallback;
    Os_CounterType                  osAlarmCounter;
} Os_AlarmCfgType;

/* Definition of resource configuration type. */
typedef struct
{
    Os_PriorityType       ceiling;
    Os_ResourceOccupyType resourceOccupyType;
} Os_ResourceCfgType;

/* OsIsr. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.17. */
/* Type definition of interrupt configuration. */
typedef struct
{
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    const Os_IsrTimingProtectionType* OsIsrTimePt;
#endif
    uint32 OsIsrSrc;     /* Isr source register. Used in TerminateApplication. */
    uint32 OsIsrSrcType; /* Isr_src type, CPU0/CPU1/CPU2/DMA. */
    Os_IsrCategoryType OsIsrCatType;
    boolean            OsNestedEnable;
} Os_IsrCfgType;

/* OsScheduleTable. AutoSar3.1: AUTOSAR_SWS_OS.pdf: 10.2.22. */
/* This struct type identifies a schedule table config. */
typedef struct
{
    const Os_SchedTblAutostart* osSchedTblAutostartRef;
    const Os_SchedTblEP*        osSchedTblEP;
#if ((OS_SC2 == CFG_SC) || (OS_SC4 == CFG_SC))
    Os_SchedTblSync             osSchedTblSync;
#endif /* OS_SC2 == CFG_SC || OS_SC4 == CFG_SC */
    Os_TickType                 osSchedTblDuration;
    Os_CounterType              osSchedTblCounterRef;
    uint16                      osSchedTblEPsize;
    boolean                     osSchedTblRepeating;
} Os_SchedTblCfgType;

/* This type of Counter Configuration . */
typedef struct
{
    Os_TickType        osCounterMaxAllowedValue;
    Os_TickType        osCounterMinCycle;
    Os_TickType        osCounterTicksPerBase;
    /* To check the correctness with the configuration */
    Os_TickType osSecondsPerTick;
    Os_CounterEnumType osCounterType;
} Os_CounterCfgType;
typedef Os_CounterCfgType* Os_CounterCfgRefType;

/*Define the structure of the app attribute configuration of the object*/
/* Define the structure of the app attribute configuration of the object */
typedef struct
{
    const ApplicationType* accAppRef;        /* Access App reference bitmap list */
    ApplicationType        hostApp;          /* owner app of the object */
    ApplicationType        accAppRefNodeCnt; /* accAppRef array node count */
} Os_ObjectAppCfgType;

/* The structure configuration type definition of the Application. */
typedef struct
{
    const Os_AppHookCfgType            OsApplicationHooks;
    const Os_AppObjectRefType*         OsAppObjectRef;
    const Os_TrustedFunctionIndexType* OsAppTrustedFuncRef;
#if (TRUE == CFG_MEMORY_PROTECTION_ENABLE)
    const OS_APP_ADDR                  OsAppAssignedPeripheralAddr;
#endif
    CoreIdType                         OsHostCore; /* owner core of the app */
    uint16                             OsAppTaskCnt;
    uint16                             OsAppIsrRefCnt;
    uint16                             OsAppAlarmRefCnt;
    uint16                             OsAppCounterRefCnt;
    uint16                             OsAppScheduleTableCnt;
    /* Element count of OsAppTrustedFuncRef array */
    uint16                             OsAppTrustedFuncCnt; 
    const Os_TaskType                  OsRestartTask;
    /* true: trusted; fasle: not trusted (default). */
    boolean                            OsTrusted;  
    /* true: time protection is delayed
     * false: time protection is triggered immediately*/
    boolean OsTrustedApplicationDelayTimingViolationCall;
    /* true: OS-Application runs within a protected environment.
     * false: OS-Application has full write access (default)*/
    boolean OsTrustedAppWithProtection;

} Os_ApplicationCfgType;

typedef struct
{
    Os_IocU16Type      IocSenderId;
    Os_ApplicationType IocSenderApplicationRef;
} Os_IocSenderPropertiesCfgType;

typedef struct
{
    Os_IocCallbackType IocCallBackFunc; /*Receive a callback function */
    Os_ApplicationType IocReceiverApplicationRef;
} Os_IocReceiverPropertiesCfgType;

typedef struct
{
    const void*   IocInitValuePtr;
    Os_IocU16Type IocDataPropertyIndex;
    Os_IocU16Type IocDataTypeLength;
    boolean       IocInitValueEnable;
} Os_IocDataPropertiesCfgType;

typedef struct
{
    /* OsIocSenderProperties */
    const Os_IocSenderPropertiesCfgType* IocSenderProperties;
    /* OsIocReceiverProperties */
    const Os_IocReceiverPropertiesCfgType* IocReceiverProperties;
    /* OsIocDataProperties */
    const Os_IocDataPropertiesCfgType* IocDataProperties;
    /* Buffer Ptr */
    Os_IocBufferType* IocBufferPtr;
    /* Block Ptr */
    Os_IocBlockType* IocBlockPtr;
    /* Buffer Length*/
    Os_IocU16Type IocBufferLength;
    /* Sender Number */
    Os_IocU16Type IocSenderNumber;
    /* Receiver Number */
    Os_IocU16Type IocReceiverNumber;
    /* Data Number */
    Os_IocU16Type IocDataNumber;
    /* Data Length */
    Os_IocU16Type IocDataTotalLength;
    /* Block Number */
    Os_IocU16Type IocBlockCnt;
    boolean       IocQueueEnable;
    Os_IocCoreComType IocCfgComMark;
} Os_IocCommunicationCfgType;

typedef struct
{
    uint32 periAreaStartAddr;
    uint32 periAreaEndAddr;
#if (TRUE == CFG_SERVICE_PROTECTION_ENABLE)
    uint64 periAreaAppAccMask;
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
    OS_TRACE_TASK_STATE_ACTIVED
} Os_TraceTaskStateType;

typedef enum
{
    OS_TRACE_ISR_STATE_EXITED,
    OS_TRACE_ISR_STATE_ENTERED
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
    Os_TraceTaskStateType        state;
    Os_TaskStateType             realState;
} Os_TraceTaskType;

typedef struct
{
    Os_TraceIsrStateType state;
    Os_TaskStateType     realState;
} Os_TraceIsrType;

typedef struct
{
    Os_AppObjectId            occupyObjectId;
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
    Os_TraceIsrType*             isrTrace;
    Os_TraceTaskType*            taskTrace;
    Os_TraceResourceType*        resourceTrace;
    Os_TraceAlarmType*           alarmTrace;
    Os_CoreIdType                coreId;
    Os_AppModeType               curAppMode;
    Os_AppObjectType             runObjId;
    Os_IsrType                   runIsrId;
    Os_TaskType                  runTaskId;
    Os_TaskType                  nextTaskId;
    Os_PriorityType              runTaskPrio;
    Os_TraceTaskSwitchReasonType nextRunReason;
    Os_ServiceIdType             curServiceId;
    StatusType                   lastError;
} Os_TraceSysType;

/* --------------------------------others------------------------------*/
typedef struct
{
    Os_EventMaskType                eventMask;
    uint8*                          Address8;
    uint16*                         Address16;
    uint32*                         Address32;
    StatusType*                     Status;
    TryToGetSpinlockType*           Success;
    Os_EventMaskRefType             eventRef;
    Os_TaskRefType                  taskRef;
    Os_TaskStateRefType             taskState;
    TrustedFunctionParameterRefType FunctionParams;
    Os_TickRefType                  counterValue;
    Os_TickRefType                  counterElapsedValue;
    Os_AlarmBaseRefType             alarmBaseRef;
    Os_TickRefType                  alarmTickRef;
    ScheduleTableStatusRefType      ScheduleStatus;
    ApplicationStateRefType         appStateRef;
    uint32                          Clearmask32;
    uint32                          Setmask32;
    uint32                          Value;
    uint32                          Value32;
    TickType                        Offset;
    TickType                        Start;
    TickType                        ScheduleTableOffset;
    TickType                        ScheduleTableStart;
    Os_TickType                     alarmTime;
    Os_TickType                     alarmCycle;
    TickType                        value;
    Os_MemoryStartAddressType       MemStartAddress;
    Os_MemorySizeType               MemSize;
    uint16                          Area;
    uint16                          Clearmask16;
    uint16                          Setmask16;
    uint16                          SpinlockId;
    Os_TaskType                     taskId;
    CounterType                     counterId;
    Os_AlarmType                    alarmId;
    Os_ResourceType                 resourceId;
    ScheduleTableType               ScheduleTableId;
    Os_IsrType                      ISRID;
    ScheduleTableType               ScheduleTableID_From;
    ScheduleTableType               ScheduleTableID_To;
    CoreIdType                      CoreID;
    TrustedFunctionIndexType        FunctionIndex;
    uint16                          DataLenth;
    uint16                          Value16;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType              ApplID;
    Os_AppObjectId                  ObjectID;
    Os_ObjectTypeType               ObjectType;
    Os_RestartType                  RestartOption;
#endif
    StatusType                      ShutdownError;
    boolean                         ClearPending;
    uint8                           Clearmask8;
    uint8                           Setmask8;
    IdleModeType                    IdleMode;
    uint8                           Value8;
} Os_RunTimeSrvPrarmType;

/*=======Os Control Block struct type definitions ==========================================*/
/* Task control block type definitions*/
/* This struct type identifies Task control block type.  */
typedef struct
{
    Os_StackPtrType    taskTop;
    Os_StackPtrType    taskStackBottom;
#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType*   taskResourceStack; /* AutoSar: for protection hook. */
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtCbDataDef osTpTask[TP_OPT_TASK_BUTT];
#endif
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR)
    uint32             taskScheduleCount;
#endif
    uint16             taskRunPrio;
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType CallBackAppID;
#endif

/*multi-core*/
#if ((CFG_SPINLOCK_MAX > 0U) || (CFG_STD_RESOURCE_MAX > 0U))
    uint16             taskCriticalZoneStack[CFG_CRITICAL_ZONE_MAX];
    uint16             taskCriticalZoneCount;
    volatile uint16    taskCurrentSpinlockOccupyLevel;
    Os_ObjectTypeType  taskCriticalZoneType[CFG_CRITICAL_ZONE_MAX];
#endif
    Os_TaskStateType   taskState;
#if ((OS_BCC2 == CFG_CC) || (OS_ECC2 == CFG_CC))
    uint8              taskActCount;
    uint8              taskSelfActCount;
#endif /* OS_BCC2 == CFG_CC || OS_ECC2 == CFG_CC */
#if (CFG_STD_RESOURCE_MAX > 0U)
    uint8              taskResCount;
#endif
/* Autosar: for service protection*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    uint8              taskDisableAllCount;
    uint8              taskSuspendAllCount;
    uint8              taskSuspendOSCount;
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    boolean           taskTpFrameflag; /* Timing Protection: inter-arrival time. Init as true. */
    Os_TaskIsrOptType taskIsrOpt;
#endif
} Os_TCBType;

/* This type of Alarm control block. */
typedef struct
{
    Os_TickType  alarmStart;
    Os_TickType  alarmCycle;
    Os_AlarmType alarmPre;
    Os_AlarmType alarmNext;
} Os_ACBType;

/* Type definition of resource control block. */
typedef struct
{
/* Timing protection: Each resource has Os_TmProtCbDataDef. When it is locked,
   it should be processed by timing protection. */
/* Timing protection: ResourceLockBudget for task and isr. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#if (CFG_TASK_MAX > 0)
    Os_TickType*       osTmProtResBgtTask;
#endif

#if (CFG_ISR2_MAX > 0)
    Os_TickType*       osTmProtResBgtIsr;
#endif
    /* Which task lock this resource. */
    Os_IsrType         osWhichIsrOccupy;
    Os_TaskType        osWhichTaskOccupy;
    Os_TmProtCbDataDef osResTpData;
#endif
    Os_PriorityType    savePrio;
    uint8              saveCount;
    uint8              Rsv0;
    Os_CallLevelType   saveLevel;
} Os_RCBType;

#if (CFG_ISR2_MAX > 0)
/* Add: AutoSar SC2: Isr control block type definitions. */
/* Type definition of interrupt control block. */
typedef struct
{
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    Os_TmProtCbDataDef osTpIsr[TP_OPT_ISR_BUTT];
#endif
#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType* IsrC2ResourceStack; /* AutoSar: SC2 protection hook. */
#endif
#if (TRUE == CFG_SCHEDULE_COUNT_MONITOR)
    uint32 isrScheduleCount;
#endif
#if (CFG_STD_RESOURCE_MAX > 0U)
    Os_ResourceType  IsrC2ResCount;
#endif
/*multi-core*/
#if ((CFG_SPINLOCK_MAX > 0U) || (CFG_STD_RESOURCE_MAX > 0U))
    uint16            isr2CriticalZoneStack[CFG_CRITICAL_ZONE_MAX];
    uint16            isr2CriticalZoneCount;
    uint16            isrCurrentSpinlockOccupyLevel;
    Os_ObjectTypeType isr2CriticalZoneType[CFG_CRITICAL_ZONE_MAX];
#endif
#if (CFG_OSAPPLICATION_MAX > 0U)
    Os_ApplicationType CallBackAppID;
#endif
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
    boolean            osIsrTpFrameflag;
    Os_TaskIsrOptType  IsrC2IsrOpt;
#endif
/* Autosar: for service protection*/
#if ((OS_SC3 == CFG_SC) || (OS_SC4 == CFG_SC))
    uint8 isrC2DisableAllCount;
    uint8 isrC2SuspendAllCount;
    uint8 isrC2SuspendOSCount;
#endif
    uint8 resevred; /*Avoid emptiness of the Os_ICBType*/
} Os_ICBType;
#endif /* CFG_ISR2_MAX */

/* Add: AutoSar SC2: Schedule table control block type definitions. */
/* This struct type identifies a schedule tablecontrol block . */
typedef struct tagOs_STCBType
{
    const Os_SchedTblEP*   stNextEP;
    struct tagOs_STCBType* stNextNode; /* For ScheduleTable list. */
    struct tagOs_STCBType* stPreNode;  /* For ScheduleTable list. */
    /* Different between driver counter and sync counter. */
    Os_TickType            stDiff;
    /* Deviation, might be smaller then stDiff. */
    Os_TickType            stDev;
    Os_TickType            stDelay;       /* Delay of stNextEP. */
    Os_TickType            stAdjDevTotal; /* Total dev during sync process. */
    Os_TickType            stStartAbsTick;
    Os_TickType            stNextEpAbsTick; /* EP abs tick after adjust. */
    Os_TickType            stFinalDelay;
    Os_ScheduleTableType   stId;
    Os_ScheduleTableType   NextStID; /* Used for next state. */
    Os_ScheduleTableType   PrevStID; /* Used for next state. */
    uint16                 stEpId;     /* ID of stNextEP. */
    boolean                stIsAdjust; /* stNextEP need adjust or not. */
    /* App call SetScheduleTableAsync, OS should stop Sync. */
    boolean                stIsStopAdjust;
    boolean                stIsAdjAdd;
    boolean                stIsStarted;
    Os_SchedTblStateType   stState;
} Os_STCBType;

/* This type of Counter control block . */
/* DD_1_0076 */
typedef struct
{
    /* Add: AutoSar SC2: ScheduleTable list head node index reference to this counter. */
    Os_STCBType* counterStListHead;
    Os_TickType  counterCurVal;
    Os_TickType  counterLastVal;
    Os_AlarmType counterAlmQue;
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
#if (TRUE == CFG_USEPARAMETERACCESS)
    Os_RunTimeSrvPrarmType sysRunTimeSrvPrarm;
#endif
    Os_TCBType*        sysRunningTCB;
    /*Trace*/
#if (TRUE == CFG_TRACE_ENABLE)
    Os_TraceSysType*   trace;
#endif
/* Timing protection for isr. */
#if (CFG_ISR2_MAX > 0)
    Os_IsrType*        sysIsrNestQueue;
#endif
    Os_PriorityType    sysHighPrio;
    Os_TaskType        sysHighTaskID;
    Os_TaskType        sysRunningTaskID; /* No attached core ID, just local ID */
    Os_TaskType        sysPrevTaskID;
    Os_AppModeType     sysActiveAppMode;
#if (CFG_OSAPPLICATION_MAX > 0)
    Os_ApplicationType sysRunningAppID;  /*the running application ID*/
#endif
    Os_CoreIdType      sysCore; /*logical CoreID*/
    /*Alarm module*/
    Os_AlarmType       sysAlarmMax;
    ApplicationType    sysAppId;
    /*Task module*/
    uint16 sysTaskMax;
#if (CFG_ISR2_MAX > 0)
    Os_IsrType         sysRunningIsrCat2Id; /* Running isr category 2. */
    boolean            sysInIsrCat2;
#endif
#if (CFG_OSAPPLICATION_MAX > 0)
    ObjectTypeType     sysRunningAppObj; /*the running object from the application*/
#endif
    Os_LockerType      sysDispatchLocker;
    Os_CallLevelType   sysOsLevel;
#if (CFG_SPINLOCK_MAX > 0)
    boolean            CurrentSpinlockOccupied[CFG_SPINLOCK_MAX]; /* PRQA S 1039 */ /* VL_Os_1039 */
#endif
#if (TRUE == CFG_USEGETSERVICEID)
    Os_ServiceIdType   sysOsServiceId;
#endif
} Os_SCBType;

typedef struct
{
    Os_IocBlockType*  IocBlockPtr;
    Os_IocBufferType* IocBufferPtr;
    Os_IocU16Type     IocDataElementSizeWithHeader;
    Os_IocU16Type     IocBlockLength;
    /* QEUEU */
    Os_IocBlockIdType IocBlockHead;
    Os_IocBlockIdType IocBlockTail;
    Os_IocBlockIdType IocBlockFreeHead;
    /* UNQEUEU */
    Os_IocBlockIdType IocBlockRead;
    boolean           IocLostData;
} Os_IocCBType;

#endif /* OS_TYPES_H */

/*=======[E N D   O F   F I L E]==============================================*/
