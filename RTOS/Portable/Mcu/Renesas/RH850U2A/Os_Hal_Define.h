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
 **  FILENAME    : Os_Hal_Define.h                                             **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Unified HAL interface definitions for kernel                **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/*
 * This file provides the unified entry for kernel to access porting layer (HAL).
 * The per-processor/per-MCU implementations are selected via include paths.
 */
/*
 * Keep kernel dependencies on porting layer declarations in a single header.
 * - Processor/HAL entry points used by kernel runtime (dispatch, irq, timer, mpu...)
 */
#ifndef OS_HAL_DEFINE_H
#define OS_HAL_DEFINE_H
/*=======[I N C L U D E S]====================================================*/
/* -------- Processor/architecture abstraction layer -------- */
#include "Os_Arch_Define.h"
#include "Os_Arch_Mpu.h"
#include "Os_Arch_Irq.h"
#include "Os_Arch_Processor.h"

/* -------- MCU abstraction layer -------- */
#include "Os_Hal_Types.h"
#include "Os_Mcu_Core.h"
#include "Os_Mcu_Irq.h"
#include "Os_Mcu_Timer.h"

/**
 * ======================================================
 * HAL Layer Module Interface List
 * ======================================================
 *
 * 2.2 Interrupt Module Dependencies
 * -----------------
 * - Os_HAL_INT_IS_CLEARPENDING_SUPPORTED()
 * - Os_Hal_DisableInt()
 * - Os_Hal_EnableInt()
 * - OS_HAL_DECLARE_CRITICAL()
 * - OS_HAL_ENTRY_CRITICAL()
 * - OS_HAL_EXIT_CRITICAL()
 * - OS_HAL_TASK_SWITCH_PROC()
 * - OS_HAL_ISR2_EPILOGUE()
 * - OS_HAL_ISR2_EPILOGUE_KILL_ISR()
 * - Os_Hal_DisableIntApp()
 * - Os_Hal_SuspendInt()
 * - Os_Hal_RestoreInt()
 * - Os_Hal_SetIpl()
 * - Os_Hal_GetIpl()
 * - Os_Hal_EnableInterruptSource()
 * - Os_Hal_DisableInterruptSource()
 * - Os_Hal_EnableIntSource()
 * - Os_Hal_DisableIntSource()
 * - Os_Hal_ClearPendingInterrupt()
 * - Os_Hal_GetIsrSourceState()
 * - Os_Hal_RemoteCall()
 * - Os_Hal_DisableAllInt_ButTimingProtInt()
 * - Os_Hal_EnableAllInt_ButTimingProtInt()
 * - OS_HAL_TERMINATE_NESTED_ISR()
 * - OS_HAL_PROCESS_NESTED_ISR_STACK()
 *
 * 2.3 Memory Protection Unit Module Dependencies
 * -------------------------
 * - Os_Hal_InitKnMemMap()
 * - Os_Hal_MemProtEnable()
 * - Os_Hal_MemProtKernelMap()
 * - Os_Hal_MemProtTaskMap()
 * - Os_Hal_MemProtIsrMap()
 *
 * 2.4 Timer Module Dependencies
 * -------------------
 * - OS_HAL_TP_MAX_INCREMENT_VALUE
 * - OS_HAL_HWTIMER_MS_TO_TICKS
 * - Os_Hal_GetHwTimerTicks()
 * - Os_Hal_TpGetTimePass()
 * - Os_Hal_TpSetCompare()
 *
 * 2.5 CPU Attribute Module Dependencies
 * --------------------
 * - Os_Hal_Dispatch()
 * - Os_Hal_Switch2System()
 * - OS_HAL_ENTER_KERNEL()
 * - OS_HAL_EXIT_KERNEL()
 * - OS_HAL_TERMINATE_TASK()
 * - Os_Hal_FirstEnterTask()
 * - Os_Hal_StartScheduler()
 * - Os_Hal_CmpSwapW()
 *
 * 2.7 Chip Attribute Module Dependencies
 * ---------------------
 * - OS_HAL_ADDRESS_IS_IN_ALL_RAM()
 * - OS_HAL_ADDRESS_IS_IN_OTHER_APP()
 * - Os_Hal_MultiCoreInitProcessor()
 * - Os_Hal_InitCPU()
 * - Os_Hal_StartCore()
 * - Os_Hal_GetCoreID()
 * - Os_Hal_SetCoreMode()
 * - Os_Hal_GetCoreMode()
 * - Os_Hal_CheckCPUInformation()
 *
 */

#endif /* OS_HAL_DEFINE_H */
/*=======[E N D   O F   F I L E]==============================================*/
