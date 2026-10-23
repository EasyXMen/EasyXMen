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
**  FILENAME    : Os_Arch_Define.h                                            **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION :                                                             **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r23                         **
**  Version :   AUTOSAR classic Platform r23--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef OS_ARCH_DEFINE_H
#define OS_ARCH_DEFINE_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"

/*=======[M A C R O S]========================================================*/

/* General Mask and Decimal Define */
#define OS_NUM_0                    (0x00U)
#define OS_NUM_1                    (0x01U)
#define OS_NUM_2                    (0x02U)
#define OS_NUM_3                    (0x03U)
#define OS_NUM_4                    (0x04U)
#define OS_NUM_5                    (0x05U)
#define OS_NUM_6                    (0x06U)
#define OS_NUM_7                    (0x07U)
#define OS_NUM_8                    (0x08U)
#define OS_NUM_9                    (0x09U)
#define OS_NUM_10                   (0x0AU)
#define OS_NUM_11                   (0x0BU)
#define OS_NUM_12                   (0x0CU)
#define OS_NUM_13                   (0x0DU)
#define OS_NUM_14                   (0x0EU)
#define OS_NUM_15                   (0x0FU)
#define OS_NUM_16                   (0x10U)
#define OS_NUM_17                   (0x11U)
#define OS_NUM_18                   (0x12U)
#define OS_NUM_19                   (0x13U)
#define OS_NUM_20                   (0x14U)
#define OS_NUM_21                   (0x15U)
#define OS_NUM_22                   (0x16U)
#define OS_NUM_23                   (0x17U)
#define OS_NUM_24                   (0x18U)
#define OS_NUM_25                   (0x19U)
#define OS_NUM_26                   (0x1AU)
#define OS_NUM_27                   (0x1BU)
#define OS_NUM_28                   (0x1CU)
#define OS_NUM_29                   (0x1DU)
#define OS_NUM_30                   (0x1EU)
#define OS_NUM_31                   (0x1FU)
#define OS_1BIT_MASK                (0x1U)
#define OS_2BIT_MASK                (0x3U)
#define OS_3BIT_MASK                (0x7U)
#define OS_4BIT_MASK                (0xFU)
#define OS_5BIT_MASK                (0x1FU)
#define OS_6BIT_MASK                (0x3FU)
#define OS_7BIT_MASK                (0x7FU)
#define OS_8BIT_MASK                (0xFFU)
#define OS_16BIT_MASK               (0xFFFFU)
#define OS_24BIT_MASK               (0xFFFFFFU)
#define OS_32BIT_MASK               (0xFFFFFFFFU)

/* ===============================================================
* Core Register Relative In Cortex-M4 Architecture specification *
=============================================================== */

/* Get/Set Memory address */
#define OS_ARCH_REG8(address)           (*(volatile uint8*)(address))  /* 8-bit register */
#define OS_ARCH_REG16(address)          (*(volatile uint16*)(address)) /* 16-bit register */
#define OS_ARCH_REG32(address)          (*(volatile uint32*)(address)) /* 32-bit register */

/*
 * Floating Point Unit
 * */
/* Floating-point Context Control Register */
#define OS_REG_FPCCR                    OS_ARCH_REG32(0xE000EF34U)

/* Coprocessor Access Control Register */
#define OS_REG_SCB_CPACR                OS_ARCH_REG32(0xE000ED88U)
#define OS_CPACR_CP10_MASK              (OS_2BIT_MASK << OS_NUM_20)
#define OS_CPACR_CP11_MASK              (OS_2BIT_MASK << OS_NUM_22)

/* Core ID */
#define OS_REG_SCB_CPUID                OS_ARCH_REG32(0xE000ED00U)

/* Config Control Register */
#define OS_REG_SCB_CCR                  OS_ARCH_REG32(0xE000ED14U)
#define OS_SCB_CCR_NONBASETHDENA_MASK   (OS_1BIT_MASK)

/* The platform can reserve configuration, such as reserving stack traceback space*/
#define OS_HAL_STACK_RESERVE            ((sint32)OS_NUM_0)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

#endif /* OS_ARCH_DEFINE_H */

/*=======[E N D   O F   F I L E]==============================================*/
