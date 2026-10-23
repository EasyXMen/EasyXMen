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
 **  FILENAME    : Os_Hal_Types.h                                              **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : MCU dependence                                              **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

#ifndef OS_HAL_TYPES_H
#define OS_HAL_TYPES_H

/*=======[I N C L U D E S]=================================================*/
#include "Os_Cfg.h"

/* type of an Interrupt Service Routine (ISR) */
typedef uint32 Os_HalMsrType;

typedef uint32 Os_Hal_TpType;

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)

#define OS_DATA_REG_NUM         (28UL)

#define OS_STACK_TRACE_NUMBER (3UL)

typedef uint32 Os_RegisterType;

/*! Set of hardware registers to be able to resume from an exception. */
struct Os_ExceptionContextType_Tag
{
    /* PRQA S 3132 ++ */                        /* VL_QAC_MagicNum */
    uint32 GeneralRegisters[OS_DATA_REG_NUM];   /* Stored Data registers (r6-r31) */
    uint32 Ra;                                  /* Stored return address */
    uint32 Psw;                                 /* Stored Psw */
    uint32 Sp;                                  /* Store Stack Point */
    uint32 ExceptionSource;                     /* Stored Exception source(Exception class) */
};
/* PRQA S 3132 -- */
typedef struct Os_ExceptionContextType_Tag Os_ExceptionContextType;

/*! Reference to an exception context. */
typedef Os_ExceptionContextType* Os_ExceptionContextRefType; /* PRQA S 5004 */ /* VL_QAC_FuncPtr */

typedef Os_ExceptionContextType Os_TrapRegsType;

#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

#endif /* MCU_DEBUG_H */
/*=======[E N D   O F   F I L E]==============================================*/
