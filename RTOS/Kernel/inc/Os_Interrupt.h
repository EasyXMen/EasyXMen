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
 **  @file               : Os_Interrupt.h
 **  @author             : i-soft-os
 **  @date               : 2024/02/10
 **  @vendor             : isoft
 **  @description        : Os header file for interrupt api declarations
 **
 ***********************************************************************************************************************/

#ifndef OS_INTERRUPT_H
#define OS_INTERRUPT_H

/* =================================================== inclusions =================================================== */
#include "Os_Interrupt_CfgData.h"

/* ===================================================== macros ===================================================== */

/* ========================================== external function definitions ========================================= */
#if (CFG_ISR_MAX > 0)
#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief         Init the Interrupt block control.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_InitInterrupt(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service:EnableAllInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_EnableAllInterrupts(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service:SuspendAllInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_SuspendAllInterrupts(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service:SuspendOSInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_SuspendOSInterrupts(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service:ResumeAllInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ResumeAllInterrupts(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Internal implementation of OS service:ResumeOSInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ResumeOSInterrupts(Os_SCBType* pScb);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Disable all interrupt owned by the application.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        FALSE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_DisableIntSourceInApp(const Os_ApplicationCfgType* posCurAppCfg, boolean* intSourceFlag);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Resume the originally enabled interrupt owned by the application.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        FALSE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ResumeIntSourceInApp(const Os_ApplicationCfgType* posCurAppCfg, const boolean* intSourceFlag);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Make the priority to ipl.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          prio: the priority value of one os object.
 * @return             Os_IPLType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Os_IPLType Os_PrioToIpl(const Os_SCBType* pScb, Os_PriorityType prio);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Make the ipl to priority.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          ipl: Interrupt priority level.
 * @return             Os_IPLType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Os_PriorityType Os_IplToPrio(const Os_SCBType* pScb, Os_IPLType ipl);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Enter ISR1.
 * @param[in]          isrId: Isr id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_EnterISR1(Os_IsrType isrId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Exit ISR1.
 * @param[in]          isrId: Isr id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ExitISR1(Os_IsrType isrId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Enter ISR2.
 * @param[in]          isrId: Isr id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_EnterISR2(Os_IsrType isrId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
/**
 * @brief              Exit ISR2.
 * @param[in]          isrId: Isr id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ExitISR2(Os_IsrType isrId);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
