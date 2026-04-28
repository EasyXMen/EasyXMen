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
 **  @file               : Os_Macros.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for macros declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_MACROS_H
#define OS_MACROS_H

/* =================================================== inclusions =================================================== */

/* ===================================================== macros ===================================================== */
/**
 * @brief Define Scalability Class.
 */
#define OS_NOSC 0x0U /** No conformance classes */
#define OS_SC1 0x1U  /** Define Scalability Class 1 */
#define OS_SC2 0x2U  /** Define Scalability Class 2 */
#define OS_SC3 0x3U  /** Define Scalability Class 3 */
#define OS_SC4 0x4U  /** Define Scalability Class 4 */

/**
 * @brief Defines for all conformance classes.
 */
#define OS_BCC1 0x1U /** BCC1 */
#define OS_BCC2 0x2U /** BCC2 */
#define OS_ECC1 0x3U /** ECC1 */
#define OS_ECC2 0x4U /** ECC2 */

/**
 * @brief Define the status of kernel.
 */
#define OS_STATUS_STANDARD 0x00U /** Define the status of kernel */
#define OS_STATUS_EXTENDED 0x01U /** Define the status of kernel */

/**
 * @brief Define the scheduling strategy of kernel.
 */
#define OS_PREEMPTIVE_NON 0x0U   /** no preemption. */
#define OS_PREEMPTIVE_FULL 0x1U  /** full preemption. */
#define OS_PREEMPTIVE_MIXED 0x2U /** mixed preemption. */

/**
 * @brief Define the invalid parameter.
 */
#define OS_CORE_INVALID ((Os_CoreIdType)0xFFFFU)       /** invalid CORE. */
#define OS_TASK_INVALID ((Os_TaskType)0xFFFFU)         /** invalid TASK. */
#define OS_PRIORITY_INVALID ((Os_PriorityType)0xFFFFU) /** invalid PRIORITY. */

/**
 * @brief Define the APP modes.
 */
#define OS_NULL_APPMODE ((Os_AppModeType)0x0000U) /** no APP modes. */
#define OS_ALL_APPMODE ((Os_AppModeType)0xFFFFU)  /** all APP modes. */

/**
 * @brief Ivalid ScheduleTable ID.
 */
#define OS_SCHEDTABLE_INVALID ((Os_ScheduleTableType)0xFFFFU)

/**
 * @brief Ivalid Os_TickType.
 */
#define OS_TICK_INVALID ((Os_TickType)0U)

/**
 * @brief Ivalid Os_CounterType.
 */
#define OS_COUNTER_INVALID ((Os_CounterType)0xFFFFU)

/**
 * @brief Ivalid Application.
 */
#define INVALID_OSAPPLICATION ((Os_ApplicationType)0xFFFFU)
#define OS_APPLICATION_VALID ((Os_ApplicationType)0xFFFEU)

/**
 * @brief Ivalid SpinlockIdType.
 */
#define OS_SPINLOCK_INVALID ((SpinlockIdType)0xFFFFU)

/**
 * @brief Null parameter.
 */
#define NULL_PARA ((uint32)0xFFFFFFFFU)

/**
 * @brief The task status for external.
 */
#define SUSPENDED OS_TASK_STATE_SUSPENDED
#define READY OS_TASK_STATE_READY
#define RUNNING OS_TASK_STATE_RUNNING
#define WAITING OS_TASK_STATE_WAITING

/**
 * @brief Stack initialization filling parameters.
 */
#define OS_STACK_FILL_PATTERN 0xCCCCCCCCU

/**
 * @brief Define stack monitoring check bit width.
 */
#define CFG_STACK_CHECK_WITH_16_BYTE     4U
#define CFG_STACK_CHECK_WITH_32_BYTE     8U
#define CFG_STACK_CHECK_WITH_64_BYTE     16U
#define CFG_STACK_CHECK_BYTE             CFG_STACK_CHECK_WITH_32_BYTE

/**
 * @brief Define stack monitoring check method.
 */
#define OS_STACK_MONITOR_BIDIRECTIONAL_CHECK     TRUE

/**
 * @brief It doesn't care what hooks are called by.
 */
#define OS_NO_CARE 0U

/**
 * @brief Time-protected hook, called in task.
 */
#define OS_TMPROT_HOOK_TASK 1U

/**
 * @brief Time-protected hook, called in ISR.
 */
#define OS_TMPROT_HOOK_ISR 2U

/**
 * @brief Time-protected hooks are called outside task and ISR.
 */
#define OS_TMPROT_HOOK_OSAPP 3U

/**
 * @brief Set the specified bit.
 */
#define OS_BIT_SET(Number, pos) ((Number) | (pos))

/**
 * @brief Clear the specified bit.
 */
#define OS_BIT_CLEAR(Number, pos) ((Number) & ~(pos))

/**
 * @brief Get the status of the specified bit.
 */
#define OS_BIT_GET(Number, pos) (((Number) & (pos)))

/**
 * @brief operate enable bit in the memory protection register.
 */
#define OS_NO_PERMISSION 0U /** Do not have permission. */
#define OS_READBIT 1U       /** read enable bit. */
#define OS_WRITEBIT 2U      /** write enable bit. */
#define OS_EXECUTBIT 4U     /** execute enable bit. */
#define OS_STACKSPACEBIT 8U /** stack space enable bit. */

/**
 * @brief Define the bit mask.
 */
#define OS_HIGH_16_BITS_MASK 0xFFFF0000U
#define OS_HIGH_8_BITS_MASK 0xFF00U
#define OS_HIGH_4_BITS_MASK 0xF0U
#define OS_LOW_8_BITS_MASK 0x00FFU
/**
 * @brief Magic number.
 */
#define OS_SIXTEEN  16U
#define OS_EIGHT    8U
#define OS_FOUR     4U
#define OS_THREE    3U
#define OS_TWO      2U
#define OS_ONE      1U

/**
 * @brief Define the top and bottom of the stack.
 */
#define OS_STACK_TOP(stack) ((Os_StackPtrType)((&(stack)[0]) + (sizeof(stack) / sizeof((stack)[0])) - CFG_STACK_CHECK_BYTE))
#define OS_STACK_BOTTOM(stack) ((Os_StackPtrType)(stack))

/**
 * @brief Determine if the memory area is readable.
 */
#define OSMEMORY_IS_READABLE(Number) OS_BIT_GET(Number, OS_READBIT)

/**
 * @brief Determine if the memory area is writable.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OSMEMORY_IS_WRITEABLE(Number) OS_BIT_GET(Number, OS_WRITEBIT)

/**
 * @brief Determine if the memory area is executable.
 */
#define OSMEMORY_IS_EXECUTABLE(Number) OS_BIT_GET(Number, OS_EXECUTBIT)

/**
 * @brief Determine if the memory area is stack space.
 */
#define OSMEMORY_IS_STACKSPACE(Number) OS_BIT_GET(Number, OS_STACKSPACEBIT)

/**
 * @brief Set the memory area to be readable.
 */
#define OSMEMORY_SET_READABLE(Number) OS_BIT_SET(Number, OS_READBIT)

/**
 * @brief Set the memory area to be writable.
 */
#define OSMEMORY_SET_WRITEABLE(Number) OS_BIT_SET(Number, OS_WRITEBIT)

/**
 * @brief Set the memory area to be executable.
 */
#define OSMEMORY_SET_EXECUTABLE(Number) OS_BIT_SET(Number, OS_EXECUTBIT)

/**
 * @brief Set the memory area to stack space.
 */
#define OSMEMORY_SET_STACKSPACE(Number) OS_BIT_SET(Number, OS_STACKSPACEBIT)

/**
 * @brief Indicates that memory protection is sent out without access.
 */
#define OSMEMORY_SET_NOACCESS(Number) (OS_NO_PERMISSION)

/**
 * @brief Get the local id of the object.
 */
#define Os_GetObjLocalId(objGlobalId) (0x0FFFu & (objGlobalId))
/* PRQA S 3472 -- */

/**
 * @brief Build the task name.
 */
/*PRQA S 0342 ++ */ /* VL_Os_0342 */
#define TASK(name) void Os_TaskEntry_##name(void)
/*PRQA S 0342 -- */

/**
 * @brief Build the interrupt name.
 */
#define ISR(name) void name(void)

/**
 * @brief Declare the callback function.
 */
#define ALARMCALLBACK(name) void name(void)

/**
 * @brief Declare the callback function of alarm.
 */
/* PRQA S 3412 ++ */ /* VL_Os_3412 */
#define DeclareAlarmCallback(name) extern void name(void)

/**
 * @brief Declare a task.
 */
/*PRQA S 0342 ++ */ /* VL_Os_0342 */
#define DeclareTask(TaskId) extern void Os_TaskEntry_##TaskId(void)
/*PRQA S 0342 -- */

/**
 * @brief Declare a interrupt.
 */
#define DeclareISR(name) extern void name(void)
/* PRQA S 3412 -- */

/**
 * @brief Declare a alarm.
 */
#define DeclareAlarm(AlarmId)

/**
 * @brief Declare a event.
 */
#define DeclareEvent(EventId)

/**
 * @brief Declare a resource.
 */
#define DeclareResource(ResourceId)

/**
 * @brief Declare a counter.
 */
#define DeclareCounter(CounterId)

/**
 * @brief Unused parameter.
 */
#define UNUSED_PARAMETER(VariableName) (void)(VariableName)

/**
 * @brief Local function.
 */ 
#ifndef OS_LOCAL
#define OS_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

/**
 * @brief Local inline function.
 */ 
#ifndef OS_LOCAL_INLINE
#define OS_LOCAL_INLINE static inline
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
