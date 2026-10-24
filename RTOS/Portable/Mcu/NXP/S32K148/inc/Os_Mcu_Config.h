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
 **  FILENAME    :  Os_Mcu_Config.h                                            **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      :  i-soft-os                                                  **
 **  Vendor      :                                                             **
 **  DESCRIPTION :  Debug Relation Functionality                               **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
 **  Version :   AUTOSAR classic Platform R23--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

#ifndef OS_MCU_CONFIG_H
#define OS_MCU_CONFIG_H

/*=======[I N C L U D E S]====================================================*/

#include "Os_Types.h"

/*=======[M A C R O S]========================================================*/
/* SC1 MACROS define region start */
/* NVIC irq number */
#define OS_MCU_NVIC_NUM             (162U)

/* NVIC priority Bit width */
#define OS_MCU_NVIC_PRIO_BITS       (4U)

/* NVIC INLINESNUM ALIGN */
#define OS_MCU_NVIC_VECTOR_ALIGNE   (512U)

/* CPU IDentiy */
#define OS_MCU_SCB_CPUID_VALUE      (0x411FC272U)
/* SC1 MACROS define region end */


/* SC2 MACROS define region start */

/* SC2 MACROS define region end */


/* SC3 MACROS define region start */
/* only SC3 need to define! */
/* Choose Which MPU to use. Arch-MPU or external MCU-MPU. */
#define OS_MCU_EXTERNAL_MPU_USED                            (FALSE)
#define OS_MCU_MPU_REGION_USEMAX_NUM                        (9u)
/* SC3 MACROS define region end */

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_CoreIdType Os_Hal_GetCoreID(void);
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* OS_MCU_CONFIG_H */

/*=======[E N D   O F   F I L E]==============================================*/
