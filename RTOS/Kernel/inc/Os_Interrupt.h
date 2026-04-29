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
/**
 * @brief SCB and CoreId are obtained by isrId.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define OS_ISR_GET_COREID(isrId) (Os_IsrCfg[isrId].HostCore)
/* PRQA S 3472 -- */
#if (TRUE == ENABLE_CLONE_TECH)
#define OS_ISR_GET_SCB(isrId) (&Os_SCB_FA_Inf[0])
#else
#define OS_ISR_GET_SCB(isrId) (Os_SCB_FA_Inf[OS_ISR_GET_COREID(isrId)])
#endif

/**
 * @brief Check the validity of the isrId.
 *
 * Macro parameters:
 *  @param[in] isrId: The Id of the Isr to be checked.
 */
/* PRQA S 3472 ++ */ /* VL_Os_3472 */
#define Os_CheckIsr2Id(isrId) (                                           \
    ((isrId) >= (Os_IsrIdRange[OS_ISR_GET_COREID(isrId)].Isr2.IsrStart)) && \
    ((isrId) < (Os_IsrIdRange[OS_ISR_GET_COREID(isrId)].Isr2.IsrEnd)))
/* PRQA S 3472 -- */

/* ========================================== external function definitions ========================================= */
#if (CFG_ISR_MAX > 0)
/**
 * @brief         Init the Interrupt block control.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         -
 */
extern void Os_InitInterrupt(void);

/**
 * @brief              Internal implementation of OS service:EnableAllInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_EnableAllInterrupts(Os_SCBType *pScb);

/**
 * @brief              Internal implementation of OS service:SuspendAllInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_SuspendAllInterrupts(Os_SCBType *pScb);

/**
 * @brief              Internal implementation of OS service:SuspendOSInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_SuspendOSInterrupts(Os_SCBType *pScb);

/**
 * @brief              Internal implementation of OS service:ResumeAllInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ResumeAllInterrupts(Os_SCBType *pScb);

/**
 * @brief              Internal implementation of OS service:ResumeOSInterrupts.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ResumeOSInterrupts(Os_SCBType *pScb);

/**
 * @brief              Disable all interrupt owned by the application.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        FALSE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_DisableIntSourceInApp(const Os_ApplicationCfgType *posCurAppCfg, boolean *intSourceFlag);

/**
 * @brief              Resume the originally enabled interrupt owned by the application.
 * @param[in]          pScb: Points to the system control block.
 * @synchronous        FALSE
 * @reentrant          TRUE
 * @trace              -
 */
extern void Os_ResumeIntSourceInApp(const Os_ApplicationCfgType *posCurAppCfg, const boolean *intSourceFlag);

/**
 * @brief              Enter ISR1.
 * @param[in]          isrId: Isr id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_EnterISR1(Os_IsrType isrId);

/**
 * @brief              Exit ISR1.
 * @param[in]          isrId: Isr id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ExitISR1(Os_IsrType isrId);

/**
 * @brief              Enter ISR2.
 * @param[in]          isrId: Isr id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_EnterISR2(Os_IsrType isrId);

/**
 * @brief              Exit ISR2.
 * @param[in]          isrId: Isr id.
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern void Os_ExitISR2(Os_IsrType isrId);

/**
 * @brief              Make the priority to ipl.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          prio: the priority value of one os object.
 * @return             Os_IPLType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Os_IPLType Os_PrioToIpl(const Os_SCBType *pScb, Os_PriorityType prio);

/**
 * @brief              Make the ipl to priority.
 * @param[in]          pScb: Points to the system control block.
 * @param[in]          ipl: Interrupt priority level.
 * @return             Os_IPLType
 * @synchronous        TRUE
 * @reentrant          FALSE
 * @trace              -
 */
extern Os_PriorityType Os_IplToPrio(const Os_SCBType *pScb, Os_IPLType ipl);
#endif

#endif

/*=======[E N D   O F   F I L E]==============================================*/
