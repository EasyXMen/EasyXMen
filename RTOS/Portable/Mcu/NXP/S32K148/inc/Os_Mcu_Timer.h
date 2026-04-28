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
**                                                                            **
**  FILENAME    :  Os_Mcu_Timer.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      :  i-soft-os                                                  **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef MCU_TIMER_H
#define MCU_TIMER_H

/*=======[I N C L U D E S]=================================================*/
#include "Os_Arch_Processor.h"
#include "Os_Cfg.h"
#include "S32K148.h"
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
#include "S32K148_FTM.h"
#include "S32K148_PCC.h"
#endif

/*=======[M A C R O S]=====================================================*/
#define Os_McuClearSysTimer(coreId)         Os_ArchClearSysTimer(coreId)    

#define Os_TmProtFaultProc()
#define Os_McuClearTmProtTimer(id)\
do\
{\
    FTM0->SC &= ~FTM_SC_TOF_MASK;\
} while (0);


#if (TRUE == CFG_TIMING_PROTECTION_ENABLE || TRUE == CFG_LOAD_RATIO_CALC_ENABLE)
#define OS_SYSTEM_TIMER_MAX_TICK_VALUE (0xFFFFU)
#define OS_HAL_TP_MAX_INCREMENT_VALUE (0x8000U)
#endif /* TRUE == CFG_LOAD_RATIO_CALC_ENABLE */

#if (TRUE == CFG_OS_MONITOR_ENABLE) /* PRQA S 3332 */ /* VL_Os_3332 */
#define OS_NS_PER_SEC           1000000000ULL   /* 1s = 1,000,000,000ns */
#define OS_NS_PER_MS            1000000ULL  /* 1ms = 1,000,000ns */
#define OS_HAL_SYSTEM_TIMER_REG_PER_MS_VALUE          CFG_REG_OSTIMER_VALUE_CORE0

#define OS_TIMER_TICKS_TO_NS(ticks) \
    ((uint64)(((ticks) * OS_NS_PER_SEC) / CFG_SYSTEM_TIMER_FREQ))

#define OS_TIMER_GET_ELAPSED_TICKS()      ((CFG_REG_OSTIMER_VALUE_CORE0 - 0x1u) - (S32_SysTick->CVR))
#endif /* TRUE == CFG_OS_MONITOR_ENABLE */


/*=======[T Y P E   D E F I N I T I O N S]==================================*/
typedef uint32 Os_Hal_TpType;

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
extern void   Os_Hal_TpSetCompare(Os_CoreIdType coreId, uint32 Increment);
#endif /* #if (TRUE == CFG_SYSTEM_TIMER_ENABLE) */

#endif /* 01_BSW_OS_PLATFORM_ARCH_TIMER_H_ */
