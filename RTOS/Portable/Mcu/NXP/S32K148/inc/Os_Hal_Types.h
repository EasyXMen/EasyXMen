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
 **  FILENAME    :  Os_Hal_Types.h                                             **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
 **  Version :   AUTOSAR classic Platform R23--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

#ifndef OS_HAL_TYPES_H
#define OS_HAL_TYPES_H

/*=======[I N C L U D E S]====================================================*/\

#include "Std_Types.h"
#include "Os_Debug_Cfg.h"
#include "Os_Cfg_S.h"

/*=======[M A C R O S]========================================================*/

#define OS_DATA_REG_NUM         (4UL)
#define OS_FPU_REG_NUM          (17UL)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/* Type of Interrupt Core resister */
typedef uint32 Os_HalMsrType;

typedef Os_HalMsrType Os_ArchMsrType;

#if (TRUE == CFG_FAULT_MANAGEMENT_ENABLE)

typedef uint32 Os_RegisterType;

/*! Set of hardware registers to be able to resume from an exception. */
struct Os_ExceptionContextType_Tag
{
    /* PRQA S 3132 ++ */                        /* VL_QAC_MagicNum */
    uint32 GeneralRegisters[OS_DATA_REG_NUM];   /* Stored Data registers (r0-r3) */
#if (OS_CFG_ASM_FPU_ENABLE == TRUE)
    uint32 GeneralFpuRegister[OS_FPU_REG_NUM];  /* Stored Data registers (s0-s15) */
#endif /* OS_CFG_ASM_FPU_ENABLE == TRUE */
    uint32 Ra;                                  /* Stored return address */
    uint32 Psw;                                 /* Stored Psw */
    uint32 Sp;                                  /* Store Stack Point */
    uint32 ExceptionSource;                     /* Stored Exception source(Exception class and tin number) */
    uint32 CFSR;                                /* Stored Message for exception */
};
/* PRQA S 3132 -- */
typedef struct Os_ExceptionContextType_Tag Os_ExceptionContextType;

/*! Reference to an exception context. */
typedef Os_ExceptionContextType* Os_ExceptionContextRefType; /* PRQA S 5004 */ /* VL_QAC_FuncPtr */

typedef Os_ExceptionContextType Os_TrapRegsType;

#endif /* TRUE == CFG_FAULT_MANAGEMENT_ENABLE */

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
 
#endif /* OS_HAL_TYPES_H */

/*=======[E N D   O F   F I L E]==============================================*/
