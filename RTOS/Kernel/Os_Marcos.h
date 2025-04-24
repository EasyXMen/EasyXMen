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
**  FILENAME    : Os_Marcos.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Os marco definition                                         **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef OS_MARCOS_H
#define OS_MARCOS_H
/*=======[I N C L U D E S]====================================================*/
#include "Arch_Define.h"
/*=======[M A C R O S]========================================================*/
/* Define Scalability Class */
#define OS_NOSC 0x0U /* No conformance classes */
#define OS_SC1  0x1U /* Define Scalability Class 1 */
#define OS_SC2  0x2U /* Define Scalability Class 2 */
#define OS_SC3  0x3U /* Define Scalability Class 3 */
#define OS_SC4  0x4U /* Define Scalability Class 4 */

/* Defines for all conformance classes */
#define OS_BCC1 0x1U /* BCC1 */
#define OS_BCC2 0x2U /* BCC2 */
#define OS_ECC1 0x3U /* ECC1 */
#define OS_ECC2 0x4U /* ECC2 */

/* Define the status of kernel */
#define OS_STATUS_STANDARD 0x00U /* Define the status of kernel */
#define OS_STATUS_EXTENDED 0x01U /* Define the status of kernel */

/* Define the scheduling strategy of kernel*/
#define OS_PREEMPTIVE_NON   0x0U /* no preemption. */
#define OS_PREEMPTIVE_FULL  0x1U /* full preemption. */
#define OS_PREEMPTIVE_MIXED 0x2U /* mixed preemption. */

/* Define the invalid parameter */
#define OS_OBJECT_INVALID   0xFFFFU                    /* invalid OBJECT. */
#define OS_CORE_INVALID     ((Os_CoreIdType)0xFFFFU)   /* invalid CORE. */
#define OS_TASK_INVALID     ((Os_TaskType)0xFFFFU)     /* invalid TASK. */
#define OS_ALARM_INVALID    ((Os_AlarmType)0xFFU)      /* invalid ALARM. */
#define OS_PRIORITY_INVALID ((Os_PriorityType)0xFFFFU) /* invalid PRIORITY. */

/* Define the APP modes */
#define OS_NULL_APPMODE ((Os_AppModeType)0x0000U) /* no APP modes. */
#define OS_ALL_APPMODE  ((Os_AppModeType)0xFFFFU) /* all APP modes. */

/* AutoSar: Ivalid ScheduleTable ID. */
#define OS_SCHEDTABLE_INVALID ((Os_ScheduleTableType)0xFFFFU)
/* AutoSar 8.4.3: Ivalid ISRID. */
#define INVALID_ISR ((Os_IsrType)0xFFFFU)
/* AutoSar: Ivalid Os_TickType. */
#define OS_TICK_INVALID ((Os_TickType)0U)
/* AutoSar: Ivalid Os_CounterType. */
#define OS_COUNTER_INVALID ((Os_CounterType)0xFFFFU)
/* AutoSar: Os-Application 8.3.1 */
#define INVALID_OSAPPLICATION ((Os_ApplicationType)0xFFFFU)
#define OS_APPLICATION_VALID  ((Os_ApplicationType)0xFFFEU)
/* AutoSar: Ivalid SpinlockIdType. */
#define OS_SPINLOCK_INVALID ((SpinlockIdType)0xFFFFU)
/* For print. */

/* Null parameter. */
#define NULL_PARA ((uint32)0xFFFFFFFFU)

/*The task status for external*/
#define SUSPENDED TASK_STATE_SUSPENDED
#define READY     TASK_STATE_READY
#define RUNNING   TASK_STATE_RUNNING
#define WAITING   TASK_STATE_WAITING

/* Stack initialization filling parameters. */
#define OS_STACK_FILL_PATTERN 0xCCCCCCCCU

/* It doesn't care what hooks are called by. */
#define OS_NO_CARE 0U
/* Time-protected hook, called in task. */
#define OS_TMPROT_HOOK_TASK 1U
/* Time-protected hook, called in ISR. */
#define OS_TMPROT_HOOK_ISR 2U
/* Time-protected hooks are called outside task and ISR. */
#define OS_TMPROT_HOOK_OSAPP 3U

/*Service Protection Level Bit*/
#define OS_LEVEL_BIT_ERRORHOOK_APP  0x0400U /* 100 0000 0000*/
#define OS_LEVEL_BIT_TASK           0x0200U /* 010 0000 0000*/
#define OS_LEVEL_BIT_ISR1           0x0100U /* 001 0000 0000*/
#define OS_LEVEL_BIT_ISR2           0x0080U /* 000 1000 0000*/
#define OS_LEVEL_BIT_ERRORHOOK      0x0040U /* 000 0100 0000*/
#define OS_LEVEL_BIT_PRETASKHOOK    0x0020U /* 000 0010 0000*/
#define OS_LEVEL_BIT_POSTTASKHOOK   0x0010U /* 000 0001 0000*/
#define OS_LEVEL_BIT_STARTUPHOOK    0x0008U /* 000 0000 1000*/
#define OS_LEVEL_BIT_SHUTDOWNHOOK   0x0004U /* 000 0000 0100*/
#define OS_LEVEL_BIT_ALARMCALLBACK  0x0002U /* 000 0000 0010*/
#define OS_LEVEL_BIT_PROTECTIONHOOK 0x0001U /* 000 0000 0001*/

/*Service calls condtion in context*/
#define OS_CONTEXT_ACTIVATE_TASK                 0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_ACTIVATE_TASK_ASYN            0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_TERMINATE_TASK                0x0200U /* 010 0000 0000*/
#define OS_CONTEXT_CHAIN_TASK                    0x0200U /* 010 0000 0000*/
#define OS_CONTEXT_SCHEDULE                      0x0200U /* 010 0000 0000*/
#define OS_CONTEXT_GET_TASK_ID                   0x06F1U /* 110 1111 0001*/
#define OS_CONTEXT_GET_TASK_STATE                0x06F0U /* 110 1111 0000*/
#define OS_CONTEXT_DISABLE_ALL_INTERRUPTS        0x07FFU /* 111 1111 1111*/
#define OS_CONTEXT_ENABLE_ALL_INTERRUPTS         0x07FFU /* 111 1111 1111*/
#define OS_CONTEXT_SUSPEND_ALL_INTERRUPTS        0x07FFU /* 111 1111 1111*/
#define OS_CONTEXT_RESUME_ALL_INTERRUPTS         0x07FFU /* 111 1111 1111*/
#define OS_CONTEXT_SUSPEND_OS_INTERRUPTS         0x07FFU /* 111 1111 1111*/
#define OS_CONTEXT_RESUME_OS_INTERRUPTS          0x07FFU /* 111 1111 1111*/
#define OS_CONTEXT_DISABLE_INTERRUPT_SOURCE      0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_ENABLE_INTERRUPT_SOURCE       0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_CLEAR_PENDING_INTERRUPT       0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_GET_RESOURCE                  0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_RELEASE_RESOURCE              0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_SET_EVENT                     0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_SET_EVENT_ASYN                0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_CLEAR_EVENT                   0x0280U /* 010 0000 0000*/
#define OS_CONTEXT_GET_EVENT                     0x06F0U /* 110 1111 0000*/
#define OS_CONTEXT_WAIT_EVENT                    0x0200U /* 010 0000 0000*/
#define OS_CONTEXT_GET_ALARM_BASE                0x06F0U /* 110 1111 0000*/
#define OS_CONTEXT_GET_ALARM                     0x06F0U /* 110 1111 0000*/
#define OS_CONTEXT_SET_REL_ALARM                 0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_SET_ABS_ALARM                 0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_CANCEL_ALARM                  0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_GET_ACTIV_APPLICATION_MODE    0x06FCU /* 110 1111 1100*/
#define OS_CONTEXT_START_OS                      0x0000U /* 000 0000 0000*/
#define OS_CONTEXT_SHUTDOWN_OS                   0x06C8U /* 110 1100 1000*/
#define OS_CONTEXT_GET_APPLICATION_ID            0x06FDU /* 110 1111 1101*/
#define OS_CONTEXT_GET_ISR_ID                    0x06C1U /* 110 1100 0001*/
#define OS_CONTEXT_CALL_TRUSTED_FUNCTION         0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_CHECK_ISR_MEMORY_ACCESS       0x06C1U /* 110 1100 0001*/
#define OS_CONTEXT_CHECK_TASK_MEMORY_ACCESS      0x06C1U /* 110 1100 0001*/
#define OS_CONTEXT_CHECK_OBJECT_ACCESS           0x06C1U /* 110 1100 0001*/
#define OS_CONTEXT_CHECK_OBJECT_OWNERSHIP        0x06C1U /* 110 1100 0001*/
#define OS_CONTEXT_START_ST_REL                  0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_START_ST_ABS                  0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_STOP_SCHEDULE_TABLE           0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_NEXT_SCHEDULE_TABLE           0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_START_ST_SYN                  0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_SYNC_SCHEDULE_TABLE           0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_GET_SCHEDULE_TABLE_STATUS     0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_SET_SCHEDULE_TABLE_ASYNC      0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_INCREMENT_COUNTER             0x0280U /* 010 1000 0000*/
#define OS_CONTEXT_GET_COUNTER_VALUE             0x0280U /* 110 1000 0000*/
#define OS_CONTEXT_GET_ELAPSED_COUNTER_VALUE     0x0280U /* 110 1000 0000*/
#define OS_CONTEXT_TERMINATE_APPLICATION         0x0680U /* 110 1000 0000*/
#define OS_CONTEXT_ALLOW_ACCESS                  0x0280U /* 110 1000 0000*/
#define OS_CONTEXT_GET_APPLICATION_STATE         0x06FDU /* 110 1111 1101*/
#define OS_CONTEXT_CONTROL_IDLE                  0x0280U /* 110 1000 0000*/
#define OS_CONTEXT_GET_CURRENT_APPLICATION_ID    0x06FDU /* 110 1111 1101*/
#define OS_CONTEXT_GET_NUMBER_OF_ACTIVATED_CORES 0x0280U /* 110 1000 0000*/
#define OS_CONTEXT_GET_CORE_ID                   0x07FFU /* 111 1111 1111*/
#define OS_CONTEXT_START_CORE                    0x0000U /* 000 0000 0000*/
#define OS_CONTEXT_START_NON_AUTOSAR_CORE        0x1000U /* 100 0000 0000*/
#define OS_CONTEXT_GET_SPINLOCK                  0x0280U /* 110 1000 0000*/
#define OS_CONTEXT_RELEASE_SPINLOCK              0x0280U /* 110 1000 0000*/
#define OS_CONTEXT_TRY_TO_GET_SPINLOCK           0x0280U /* 110 1000 0000*/
#define OS_CONTEXT_SHUTDOWN_ALLCORES             0x06C8U /* 110 1100 1000*/
#define OS_CONTEXT_OPEARTION_PERIPHERAL          0x0280U /* 010 1000 0000*/

/*Memory Protection option*/
/* Set the specified bit. */
#define OS_BIT_SET(Number, pos) ((Number) | (pos))
/* Clear the specified bit. */
#define OS_BIT_CLEAR(Number, pos) ((Number) & ~(pos))
/* Get the status of the specified bit. */
#define OS_BIT_GET(Number, pos) (((Number) & (pos)))

/* operate enable bit in the memory protection register. */
#define NO_PERMISSION 0U /* Do not have permission. */
#define READBIT       1U /* read enable bit. */
#define WRITEBIT      2U /* write enable bit. */
#define EXECUTBIT     4U /* execute enable bit. */
#define STACKSPACEBIT 8U /* stack space enable bit. */

/* Determine if the memory area is readable. */
#define OSMEMORY_IS_READABLE(Number) OS_BIT_GET(Number, READBIT)
/* Determine if the memory area is writable. */
/* PRQA S 3472 ++*/ /* MISRA  Dir-4.9*/ /* OS_MARCOS_MACRO_001 */
#define OSMEMORY_IS_WRITEABLE(Number) OS_BIT_GET(Number, WRITEBIT)
/* Determine if the memory area is executable. */
#define OSMEMORY_IS_EXECUTABLE(Number) OS_BIT_GET(Number, EXECUTBIT)
/* Determine if the memory area is stack space. */
#define OSMEMORY_IS_STACKSPACE(Number) OS_BIT_GET(Number, STACKSPACEBIT)

/* Set the memory area to be readable. */
#define OSMEMORY_SET_READABLE(Number) OS_BIT_SET(Number, READBIT)
/* Set the memory area to be writable. */
#define OSMEMORY_SET_WRITEABLE(Number) OS_BIT_SET(Number, WRITEBIT)
/* Set the memory area to be executable. */
#define OSMEMORY_SET_EXECUTABLE(Number) OS_BIT_SET(Number, EXECUTBIT)
/* Set the memory area to stack space. */
#define OSMEMORY_SET_STACKSPACE(Number) OS_BIT_SET(Number, STACKSPACEBIT)
/* Indicates that memory protection is sent out without access. */
#define OSMEMORY_SET_NOACCESS(Number) (NO_PERMISSION)

/* MultiCore ID */
/* Get the core id of the object. */
#define Os_GetObjCoreId(objGlobalId) ((0xF000u & (objGlobalId)) >> 12U)
/* Get the local id of the object. */
#define Os_GetObjLocalId(objGlobalId) (0x0FFFu & (objGlobalId))
/* PRQA S 3472 --*/ /* MISRA  Dir-4.9*/

/* Define the MACROS that used to extending*/
/* Build the task name. */
#define TASK(name) void Os_TaskEntry_##name(void)
/* Build an interrupt name. */
#define ISR(name) void name(void)
/* Declare the callback function. */
#define ALARMCALLBACK(name) void name(void)
/* Declare the callback function of alarm. */
#define DeclareAlarmCallback(name) extern void name(void)
/* Declare a task. */
#define DeclareTask(TaskId) extern void Os_TaskEntry_##TaskId(void)
/* Declare an interrupt. */
#define DeclareISR(name) extern void name(void)
#define DeclareAlarm(AlarmId)
#define DeclareEvent(EventId)
#define DeclareResource(ResourceId)
#define DeclareCounter(CounterId)

#define UNUSED_PARAMETER(VariableName) (void)(VariableName)

#endif /* OS_MARCOS_H */

/*=======[E N D   O F   F I L E]==============================================*/
