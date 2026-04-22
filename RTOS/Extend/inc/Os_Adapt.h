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
 *
 ************************************************************************************************************************
 **
 **  @file               : Os_Adapt.h
 **  @author             : i-soft-os
 **  @date               : 2025/04/02
 **  @vendor             : isoft
 **  @description        : Os header file for Adapt api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_ADAPT_H
#define OS_ADAPT_H

/* =================================================== inclusions =================================================== */
#include "Os_Types.h"

/* ================================================ type definitions ================================================ */

typedef StatusType             Os_StatusType;
typedef Os_ServiceIdType       OSServiceIdType;
typedef uint32                 PhysicalTimeType;
typedef uint32                 Os_AppAccessMaskType;

/*! Identifies an interrupt source. */
typedef uint32 Os_InterruptSourceIdType;
/*! Reference to an exception source. */
typedef Os_InterruptSourceIdType * Os_InterruptSourceIdRefType;
/*! Identifies an exception source. */
typedef uint32 Os_ExceptionSourceIdType;
/*! Reference to an exception source. */
typedef Os_ExceptionSourceIdType * Os_ExceptionSourceIdRefType;

/*! Error information data type, which can be fetched by Os_GetDetailedError(). */
typedef struct
{
  /*! Service which was processed while the error occurred. */
  OSServiceIdType Service;
  /*! AUTOSAR OS status code of the occurred error. */
  StatusType Error;
  /*! Holds the unambiguous error information. */
  StatusType DetailedError;
} Os_ErrorInformationType;

/* PRQA S 3432 ++ */ /* VL_Os_3432 */
typedef Os_ErrorInformationType * Os_ErrorInformationRefType;
/* PRQA S 3432 -- */

/*! Fast trusted function identifiers. */
/* PRQA S 1271 ++ */ /* VL_Os_1271 */
typedef enum
{
  OS_FASTTRUSTEDFUNCTIONID_COUNT = CFG_TRUSTED_SYSTEM_SERVICE_MAX,
} Os_FastTrustedFunctionIndexType;

 /*! Spinlock identifiers. */
 typedef enum
 {
   OS_SPINLOCKID_COUNT = CFG_SPINLOCK_MAX,
 } Vector_SpinlockIdType;

/*! Peripheral identifiers. */
typedef enum
{
  OS_PERIPHERALID_COUNT = CFG_PERIPHERAL_MAX,
} Vector_Os_PeripheralIdType;
/* PRQA S 1271 -- */

/* ===================================================== macros ===================================================== */

#if defined(__ADAPT_MODE__)
#ifdef ISRNAME
#undef ISRNAME
#endif
#define ISRNAME(name)       Os_Isr_##name 

#ifdef ISR
#undef ISR
#endif
#define ISR(name) void      Os_Isr_##name(void)

#ifdef DeclareISR
#undef DeclareISR
#endif
#define DeclareISR(name)    extern void Os_Isr_##name(void)
#endif

#define Os_EnableGlobalAM            Os_EnableGlobalKM
#define Os_EnableGlobalUM            Os_EnableGlobalKM
#define Os_EnableLevelAM             Os_EnableLevelKM
#define Os_EnableLevelUM             Os_EnableLevelKM
#define Os_DisableGlobalAM           Os_DisableGlobalKM
#define Os_DisableGlobalUM           Os_DisableGlobalKM
#define Os_DisableLevelAM            Os_DisableLevelKM
#define Os_DisableLevelUM            Os_DisableLevelKM


#define Os_DisableInterruptSource    DisableInterruptSource
#define Os_EnableInterruptSource     EnableInterruptSource


#define OS_CONTEXT_GETDETAILEDERROR                0x0040U       /* 000 0100 0000 */
#define OS_CONTEXT_INITIAL_ENABLE_INTERRUPT_SOURCE 0x0200U       /* 010 0000 0000 */
#define OS_CONTEXT_IS_INTERRUPT_SOURCE_ENABLED     0x0280U       /* 010 1000 0000 */


/* OS application modes */
#define OS_APPMODE_NONE         ((AppModeType)0)
#define OS_APPMODE_ANY          ((AppModeType)255)


/*! Defines whether EVENT is active (STD_ON) or not (STD_OFF). */
#if (CFG_EVENT_MAX > 0)
#define OS_CFG_EVENT            (STD_ON)
#else
#define OS_CFG_EVENT            (STD_OFF)
#endif

#if (CFG_PERIPHERAL_MAX > 0U)
/*! OS service: osReadPeripheral8() (Gen6 backward compatibility) */
#define osReadPeripheral8        Os_ReadPeripheral8
/*! OS service: osReadPeripheral16() (Gen6 backward compatibility) */
#define osReadPeripheral16       Os_ReadPeripheral16
/*! OS service: osReadPeripheral32() (Gen6 backward compatibility) */
#define osReadPeripheral32       Os_ReadPeripheral32
/*! OS service: osWritePeripheral8() (Gen6 backward compatibility) */
#define osWritePeripheral8       Os_WritePeripheral8
/*! OS service: osWritePeripheral16() (Gen6 backward compatibility) */
#define osWritePeripheral16      Os_WritePeripheral16
/*! OS service: osWritePeripheral32() (Gen6 backward compatibility) */
#define osWritePeripheral32      Os_WritePeripheral32
/*! OS service: osModifyPeripheral8() (Gen6 backward compatibility) */
#define osModifyPeripheral8      Os_ModifyPeripheral8
/*! OS service: osModifyPeripheral16() (Gen6 backward compatibility) */
#define osModifyPeripheral16     Os_ModifyPeripheral16
/*! OS service: osModifyPeripheral32() (Gen6 backward compatibility) */
#define osModifyPeripheral32     Os_ModifyPeripheral32
#endif

/*! OS service: GetElapsedCounterValue() (AUTOSAR backward compatibility) */
#define GetElapsedCounterValue   GetElapsedValue
/* The following API defines are used by the RTE to check whether the API is available. */
/*! OS service: osDisableLevelAM() (Gen6 backward compatibility ESCAN00079751) */
#define osDisableLevelAM         Os_DisableLevelAM
/*! OS service: osDisableLevelKM() (Gen6 backward compatibility ESCAN00079751) */
#define osDisableLevelKM         Os_DisableLevelKM
/*! OS service: osDisableLevelUM() (Gen6 backward compatibility ESCAN00079751) */
#define osDisableLevelUM         Os_DisableLevelUM
/*! OS service: osEnableLevelAM() (Gen6 backward compatibility ESCAN00079751) */
#define osEnableLevelAM          Os_EnableLevelAM
/*! OS service: osEnableLevelKM() (Gen6 backward compatibility ESCAN00079751) */
#define osEnableLevelKM          Os_EnableLevelKM
/*! OS service: osEnableLevelUM() (Gen6 backward compatibility ESCAN00079751) */
#define osEnableLevelUM          Os_EnableLevelUM
/*! OS service: osDisableGlobalAM() (Gen6 backward compatibility ESCAN00079751) */
#define osDisableGlobalAM        Os_DisableGlobalAM
/*! OS service: osDisableGlobalKM() (Gen6 backward compatibility ESCAN00079751) */
#define osDisableGlobalKM        Os_DisableGlobalKM
/*! OS service: osDisableGlobalUM() (Gen6 backward compatibility ESCAN00079751) */
#define osDisableGlobalUM        Os_DisableGlobalUM
/*! OS service: osEnableGlobalAM() (Gen6 backward compatibility ESCAN00079751) */
#define osEnableGlobalAM         Os_EnableGlobalAM
/*! OS service: osEnableGlobalKM() (Gen6 backward compatibility ESCAN00079751) */
#define osEnableGlobalKM         Os_EnableGlobalKM
/*! OS service: osEnableGlobalUM() (Gen6 backward compatibility ESCAN00079751) */
#define osEnableGlobalUM         Os_EnableGlobalUM

/*! OS service: osDisableInterruptSource() (Gen6 backward compatibility) */
#define osDisableInterruptSource    DisableInterruptSource
/*! OS service: osEnableInterruptSource() (Gen6 backward compatibility) */
#define osEnableInterruptSource     EnableInterruptSource
/*! OS service: osDisableInterruptSource() (Gen6 backward compatibility) */
#define osClearPendingInterrupt     ClearPendingInterrupt
/*! OS service: osIsInterruptSourceEnabled() (Gen6 backward compatibility) */
#define osIsInterruptSourceEnabled  Os_IsInterruptSourceEnabled

#define SystemTimer                 SystemTimer_Core0

/* ========================================= external function declarations ========================================= */

/**
 * @brief       Returns error information of the last error occurred on the local core.
 * @param[out]  ErrorRef: A reference to an Os_ErrorInformationType object.
 * @return      StatusType
 * @retval      E_OK: Get detailed error information successfully
 * @retval      E_NOT_OK: Sservice protection is not enabled
 * @retval      E_OS_ILLEGAL_ADDRESS: The parameter pointer is NULL
 * @retval      E_OS_CALLEVEL: Call level at wrong context
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern StatusType Os_GetDetailedError(Os_ErrorInformationRefType ErrorRef);

#if (CFG_STACK_USAGE_CHECK == TRUE)

/**
 * @brief       Get the current stack usage of the given task.
 * @param[in]   TaskID: The task which shall be queried.
 * @return      uint32: The current stack usage of the given task.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern uint32 Os_GetTaskStackUsage(TaskType TaskID);
#endif


/**
 * @brief       Disables the recognition status of all interrupts. This service 
 *              does not perform nesting nor service protection checks.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern void Os_DisableGlobalKM(void);

/**
 * @brief       Enables the recognition status of all interrupts. This service
 *              does not perform nesting nor service protection checks.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern void Os_EnableGlobalKM(void);

/**
 * @brief       Disables the recognition status of category 2 interrupts. This service
 *              does not perform nesting nor service protection checks.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern void Os_DisableLevelKM(void);

/**
 * @brief       Enables the recognition status of category 2 interrupts. This service
 *              does not perform nesting nor service protection checks.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern void Os_EnableLevelKM(void);

/**
 * @brief       Check if the given interrupt source is enabled.
 * @param[in]   ISRID: Category 2 ISR which shall be checked.
 * @param[out]  IsEnabled: Pointer to if the interrupt source is enabled: 
 *                         TRUE(enabled), FALSE(not enabled).
 * @return      StatusType
 * @retval      E_OK: Get the interrupt source status successfully
 * @retval      E_OS_ID: ISRID is not a valid Category 2 ISR ID
 * @retval      E_OS_ILLEGAL_ADDRESS: The parameter pointer is NULL
 * @retval      E_OS_CALLEVEL: Call level at wrong context
 * @retval      E_OS_ACCESS: The OS-Application of ISRID cannot be accessed.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern StatusType Os_IsInterruptSourceEnabled(ISRType ISRID, boolean * IsEnabled);

/**
 * @brief       Enable the interrupt sources of category 2 ISR on the current core.
 * @param[in]   ClearPending: Defines whether the pending flag shall be
 *                       cleared (TRUE) or not (FALSE).
 * @return      StatusType
 * @retval      E_OK: Enable interrupt sources successfully
 * @retval      E_OS_VALUE: ClearPending is not TRUE nor FALSE
 * @retval      E_OS_NOFUNC: Clearing pending flag is not supported
 * @retval      E_OS_CALLEVEL: Call level at wrong context
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern StatusType Os_InitialEnableInterruptSources(boolean ClearPending);

/**
 * @brief       Set an unhandled interrupt request.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern void Os_UnhandledIrq(void);

/**
 * @brief       In case of an unhandled interrupt request the triggering interrupt source 
 *              can be distinguished with this service.
 * @param[out]  InterruptSource: A reference to the interrupt source ID.
 * @return      StatusType
 * @retval      E_OK: No error
 * @retval      E_OS_ILLEGAL_ADDRESS: Null pointer passed as argument
 * @retval      E_OS_CALLEVEL: Call level at wrong context
 * @retval      E_OS_STATE: No unhandled interrupt reported since start up
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern StatusType Os_GetUnhandledIrq(Os_InterruptSourceIdRefType InterruptSource);

/**
 * @brief       Set an unhandled exception request.
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern void Os_UnhandledExc(void);

/**
 * @brief       In case of an unhandled exception request the triggering exception source 
 *              can be distinguished with this service.
 * @param[out]  ExceptionSource: A reference to the exception source ID.
 * @return      StatusType
 * @retval      E_OK: No error
 * @retval      E_OS_ILLEGAL_ADDRESS: Null pointer passed as argument
 * @retval      E_OS_CALLEVEL: Call level at wrong context
 * @retval      E_OS_STATE: No unhandled exception reported since start up
 * @reentrant   TRUE
 * @synchronous FALSE
 * @trace        -
 */
extern StatusType Os_GetUnhandledExc(Os_ExceptionSourceIdRefType ExceptionSource);

#endif

/*=======[E N D   O F   F I L E]==============================================*/
