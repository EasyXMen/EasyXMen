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
 *
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Arch_Irq.h                                                  **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Interrupt driver                                            **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
/* PRQA S 3108-- */
#ifndef Arch_IRQ_H_
#define Arch_IRQ_H_

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/
/* Interrupt source */
#define OS_INT_CPU_0_IRQn            0U
#define OS_INT_CPU_1_IRQn            1U
#define OS_INT_CPU_2_IRQn            2U
#define OS_INT_CPU_MSI_IRQn          3U
#define OS_INT_CPU_4_IRQn            4U
#define OS_INT_CPU_5_IRQn            5U
#define OS_INT_CPU_6_IRQn            6U
#define OS_INT_CPU_MTI_IRQn          7U
#define OS_INT_CPU_8_IRQn            8U
#define OS_INT_CPU_9_IRQn            9U
#define OS_INT_CPU_10_IRQn           10U
#define OS_INT_CPU_MEI_IRQn          11U
#define OS_INT_CPU_12_IRQn           12U
#define OS_INT_CPU_13_IRQn           13U
#define OS_INT_CPU_14_IRQn           14U
#define OS_INT_CPU_15_IRQn           15U
#define OS_INT_CPU_16_IRQn           16U
#define OS_INT_CPU_17_IRQn           17U
#define OS_INT_CPU_18_IRQn           18U
#define OS_INT_CPU_19_IRQn           19U
#define OS_INT_CPU_20_IRQn           20U
#define OS_INT_CPU_21_IRQn           21U
#define OS_INT_CPU_22_IRQn           22U
#define OS_INT_CPU_23_IRQn           23U
#define OS_INT_CPU_24_IRQn           24U
#define OS_INT_CPU_25_IRQn           25U
#define OS_INT_CPU_26_IRQn           26U
#define OS_INT_CPU_27_IRQn           27U
#define OS_INT_CPU_28_IRQn           28U
#define OS_INT_CPU_29_IRQn           29U
#define OS_INT_CPU_30_IRQn           30U
#define OS_INT_CPU_31_IRQn           31U


/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef void (*IntFun)(void);

/* Type of the variable to save the mcu status registration. */
/* DD_1_0051 */
typedef uint64 Os_ArchMsrType; 

/*=======[E X T E R N A L   D A T A]==========================================*/
extern VAR(uint32, OS_VAR)             Os_Isr1NestSp[CFG_ISR_MAX_CORE0];
extern VAR(uint32, OS_VAR)             Os_Isr2NestSp[CFG_ISR_MAX_CORE0];
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
extern FUNC(void, OS_CODE) Os_IntHandler(void);
extern FUNC(void, OS_CODE) Os_TrapHandler(void);
extern FUNC(void, OS_CODE) Os_InterruptInit(void);
extern FUNC(void, OS_CODE) Os_InterruptInstall(uint32 irqId, uint8 prio, IntFun pFun);
extern FUNC(void, OS_CODE) Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(void, OS_CODE) Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(void, OS_CODE) Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(Os_IsrStateType, OS_CODE) Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType);
extern FUNC(void, OS_CODE) Os_ArchSetIpl(Os_IPLType ipl,Os_IsrDescriptionType isrdesc);
extern FUNC(Os_IPLType, OS_CODE) Os_ArchGetIpl(void);
extern FUNC(void, OS_CODE) Os_ArchSuspendInt(Os_ArchMsrType* msr);
extern FUNC(void, OS_CODE) Os_ArchRestoreInt(Os_ArchMsrType msr);
extern FUNC(void, OS_CODE) Os_SetPending(uint32 vIsrSrc);
extern FUNC(void, OS_CODE) Os_ClearPending(uint32 vIsrSrc);
#endif
/*=======[E N D   O F   F I L E]==============================================*/
