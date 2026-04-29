/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Mcu_Timer.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the processor timers                   **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef MCU_TIMER_H
#define MCU_TIMER_H
#include "Std_Types.h"
#include "Os_Cfg.h"
#include "S32K148_LPIT.h"

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern void Os_InitTmProtTimer(void);
extern void Os_ClearTmProtTimer(void);
#endif

#if (TRUE == CFG_OS_MONITOR_ENABLE)
#define Os_ArchLoadRatioInit()          \
{                                       \
    IP_LPIT0->TMR[0].TVAL= 0xffffffff;  \
    IP_LPIT0->TMR[0].TCTRL |= 0x1;        \
    IP_LPIT0->MCR &= 0xfffffff7;        \
}
#define OS_SYSTEM_TIMER_MAX_TICK_VALUE (0xffffffff)
#define OS_SYSTEM_TIMER_REG_PER_MS_VALUE 48000
extern uint32 Os_ArchGetTimeTicks(void);
#endif

#define Os_TmProtFaultProc()
#endif