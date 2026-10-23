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
 **  FILENAME    : Os_Arch_Timer.h                                             **
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

/* Peripheral Timer Used, Not realized here, move to Os_Mcu_Timer.c for more inforamtion */

#ifndef OS_ARCH_TIMER_H
#define OS_ARCH_TIMER_H

/*=======[I N C L U D E S]====================================================*/

#include "Os_Types.h"
#include "Os_Arch_Define.h"

/*=======[M A C R O S]========================================================*/

/* SysTick Control and Status Register */
#define OS_ARCH_SYSTICK_BASE_ADDRESS         (0xE000E010U)   /* Architecture specified fixed address */
#define OS_ARCH_SYSTICK_CTRL_REG             OS_ARCH_REG32(OS_ARCH_SYSTICK_BASE_ADDRESS)
#define OS_ARCH_SYSTICK_RELOAD_REG           OS_ARCH_REG32(OS_ARCH_SYSTICK_BASE_ADDRESS + OS_NUM_4)
#define OS_ARCH_SYSTICK_COUNTER_REG          OS_ARCH_REG32(OS_ARCH_SYSTICK_BASE_ADDRESS + OS_NUM_8)
#define OS_ARCH_SYSTICK_CHECK_REG            OS_ARCH_REG32(OS_ARCH_SYSTICK_BASE_ADDRESS + OS_NUM_12)
#define OS_ARCH_SYSTICK_ENABLE_BIT           ((uint32)OS_1BIT_MASK << OS_NUM_0)
#define OS_ARCH_SYSTICK_INT_BIT              ((uint32)OS_1BIT_MASK << OS_NUM_1)
#define OS_ARCH_SYSTICK_CLK_BIT              ((uint32)OS_1BIT_MASK << OS_NUM_2)

/* Pend Systick */
#define OS_ARCH_SYSTICK_REQUEST_MASK         (OS_1BIT_MASK << OS_NUM_26)
#define OS_ARCH_SYSTICK_CLEAR_MASK           (OS_1BIT_MASK << OS_NUM_25)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

/* ============================
* Function Provided to Kernel *
============================ */

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Hal_ClearSysTimer(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

/* ====================
* Function Local Used *
==================== */

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_InitSysTimer(Os_CoreIdType coreId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern void Os_Arch_StartSysTimer(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* TRUE == CFG_SYSTEM_TIMER_ENABLE */

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
 
#endif /* OS_ARCH_TIMER_H */

/*=======[E N D   O F   F I L E]==============================================*/
