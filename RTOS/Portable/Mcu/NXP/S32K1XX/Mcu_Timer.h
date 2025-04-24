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

#if (TRUE == CFG_SYSTEM_TIMER_ENABLE)
extern FUNC(void, OS_CODE) Os_StartSysTimer(void);
#endif

#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern FUNC(void, OS_CODE) Os_InitTmProtTimer(void);
extern FUNC(void, OS_CODE) Os_ClearTmProtTimer(void);
#endif

#if (TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
#define Os_ArchLoadRatioInit()
#define OS_SYSTEM_TIMER_MAX_TICK_VALUE
#define OS_SYSTEM_TIMER_REG_PER_MS_VALUE
extern FUNC(uint32, OS_CODE) Os_ArchGetTimeTicks(void);
#endif

#define Os_TmProtFaultProc()
#endif