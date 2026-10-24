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
 **  FILENAME    : Os_Mcu_Config.h                                              **
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

#ifndef OS_MCU_CONFIG_H
#define OS_MCU_CONFIG_H

/* Core and IRQ interfaces (RH850U2A) */

/*max number of interrupt*/
#define OS_MCU_EI_INTR_NUM (768U)
#define OS_MCU_FE_INTR_NUM (32U)

/* Interrupt Register Base Address */
#define OS_MCU_INTC1_SELF_BASE_ADDR (0xFFFC0000U)
#define OS_MCU_INTC2_BASE_ADDR      (0xFFF80000U)

#define OS_MCU_FEINC_BASE_ADDR(PEn) (0xFF9A3B00U + ((uint32)(PEn) * 0x0100U))
/* FEINT Status Register */
#define OS_MCU_FEINTF(PEn) (*(uint32 volatile*)(OS_MCU_FEINC_BASE_ADDR(PEn)))
/* FEINT Event Mask Register */
#define OS_MCU_FEINTMSK(PEn) (*(uint32 volatile*)(OS_MCU_FEINC_BASE_ADDR(PEn) + 4U)) /* PRQA S 3472 */ /* VL_Os_3472 */
/* FEINT Status Clear Register */
#define OS_MCU_FEINTC(PEn) (*(uint32 volatile*)(OS_MCU_FEINC_BASE_ADDR(PEn) + 8U)) /* PRQA S 3472 */ /* VL_Os_3472 */

/* Flash address area */
/* Code Flash */
#define OS_MCU_FLASH_ADDR_START (0U)
#define OS_MCU_FLASH_ADDR_END   (0x0FFFFFFFU)

/* Local RAM and Cluster RAM */
#define OS_MCU_RAM_ADDR_START (0xFD600000U)
#define OS_MCU_RAM_ADDR_END   (0xFE83FFFFU)

/* Peripheral address area */
#define OS_MCU_PERIPH_ADDR_START (0xFF000000U)
#define OS_MCU_PERIPH_ADDR_END   (0xFFFD3FFFU)

#define OS_START_SEC_CODE
#include "Os_MemMap.h"
extern Os_CoreIdType Os_Hal_GetCoreID(void); /* PRQA S 3451,3449 */ /* VL_Os_3451, VL_Os_3449 */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h"

#endif /* OS_MCU_CONFIG_H */
