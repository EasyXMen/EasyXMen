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
 **  @file               : Os_Mcu_Debug.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/21
 **  @vendor             : isoft
 **  @description        : Mcu header file for Debug api declarations
 **
 ***********************************************************************************************************************/

#ifndef MCU_DEBUG_H
#define MCU_DEBUG_H

/*=======[I N C L U D E S]=================================================*/
#include "Std_Types.h"
#include "Os_Cfg.h"

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)

#define OS_STACK_TRACE_NUMBER 3

typedef uint32 Os_RegisterType;

/*! Set of hardware registers to be able to resume from an exception. */
struct Os_ExceptionContextType_Tag
{
    uint32 AddressRegisters[16];             /* Stored Address registers (a0-a15) */
    uint32 DataRegisters[16];                /* Stored Data registers (d0-d15) */
    uint32 Ra;                               /* Stored return address */
    uint32 Psw;                              /* Stored Psw  */
    uint32 ExceptionSource;                  /* Stored Exception source(Exception class and tin number) */
    uint32 CallStack[OS_STACK_TRACE_NUMBER]; /* Stored the call stack before the exception occurs. */
    uint32 Pcpn;                             /* Stored Pcpn (Previous CPU Priority number) from the Pcxi  */
    uint32 Pie;                              /* Stored Pie (Previous Interrupt Enable) from the Pcxi  */
    uint32 MpuRegionForStackLow;             /* The lower address of the MPU region for stack. */
    uint32 MpuRegionForStackUpper;           /* The upper address of the MPU region for stack. */
};

typedef struct Os_ExceptionContextType_Tag Os_ExceptionContextType;

/*! Reference to an exception context. */
typedef Os_ExceptionContextType* Os_ExceptionContextRefType;

typedef Os_ExceptionContextType Os_TrapRegsType;

#endif

#endif /* MCU_DEBUG_H */
