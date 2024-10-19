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
 **  FILENAME    : RV_PortASM.S                                                **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION :                                                             **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/

#include <csr.h>

.extern Os_ArchTempSp
.extern Os_SwitchTask
.extern OS_GetTaskStackPointer
.extern OS_SaveTaskStackPointer
.extern Os_SysTick_Handler0
.extern Os_PLIC_Handler
.extern Os_CPU_Handler

.align 8
.global rv_trap_handler
.weak rv_trap_handler
rv_trap_handler:
	addi sp, sp, -(29 * SZREG)
	REG_S x1,  1  * SZREG(sp)
	REG_S x5,  2  * SZREG(sp)
	REG_S x6,  3  * SZREG(sp)
	REG_S x7,  4  * SZREG(sp)
	REG_S x8,  5  * SZREG(sp)
	REG_S x9,  6  * SZREG(sp)
	REG_S x10, 7  * SZREG(sp)
	REG_S x11, 8  * SZREG(sp)
	REG_S x12, 9  * SZREG(sp)
	REG_S x13, 10 * SZREG(sp)
	REG_S x14, 11 * SZREG(sp)
	REG_S x15, 12 * SZREG(sp)
	REG_S x16, 13 * SZREG(sp)
	REG_S x17, 14 * SZREG(sp)
	REG_S x18, 15 * SZREG(sp)
	REG_S x19, 16 * SZREG(sp)
	REG_S x20, 17 * SZREG(sp)
	REG_S x21, 18 * SZREG(sp)
	REG_S x22, 19 * SZREG(sp)
	REG_S x23, 20 * SZREG(sp)
	REG_S x24, 21 * SZREG(sp)
	REG_S x25, 22 * SZREG(sp)
	REG_S x26, 23 * SZREG(sp)
	REG_S x27, 24 * SZREG(sp)
	REG_S x28, 25 * SZREG(sp)
	REG_S x29, 26 * SZREG(sp)
	REG_S x30, 27 * SZREG(sp)
	REG_S x31, 28 * SZREG(sp)

	/* push status*/
	csrr t0, CSR_STATUS
	REG_S t0,  29 * SZREG(sp)

	/* push epc */
	csrr t0, CSR_EPC
	REG_S t0, 0(sp)

	/* save sp */
	mv t0, sp
	la t1, Os_ArchTempSp
#if __SIZEOF_POINTER__ == 8
	sd t0, 0(t1)
#else
	sw t0, 0(t1)
#endif

	/* CSR TIMER */
	csrr a0, CSR_IP
	li  a1, 0x00000020u
	and a2, a1, a0
	bnez a2, systick

	/*other*/
	csrr a0, CSR_IP
	li  a1, 0xfffffdddu
	and a2, a1, a0
	bnez a2, cpu_irq

	/* Soft Interrupt */
	csrr a0, CSR_IP
	li  a1, 0x00000002u
	and a2, a1, a0
	bnez a2, dispatch
	j resume

systick:
	csrc CSR_IP, a1
	jal Os_SysTick_Handler0
	j resume

cpu_irq:
	jal Os_CPU_Handler
	j resume

dispatch:
	csrc CSR_IP, a1

	jal OS_SaveTaskStackPointer

	/* Store sp to system stack */
	jal OS_GetSystemStackPointer
#if __SIZEOF_POINTER__ == 8
	ld t0, Os_ArchTempSp
#else
	lwu t0, Os_ArchTempSp
#endif
	mv sp, t0

	jal Os_SwitchTask

	jal OS_GetTaskStackPointer

resume:
	/* resume sp*/
#if __SIZEOF_POINTER__ == 8
	ld t0, Os_ArchTempSp
#else
	lwu t0, Os_ArchTempSp
#endif
	mv sp, t0

	/* pop epc */
	lwu t0,  0  * SZREG(sp)
	csrw CSR_EPC, t0

	/* pop status*/
	REG_L t0,  29 * SZREG(sp)
	csrw CSR_STATUS, t0

	REG_L x1,  1  * SZREG(sp)
	REG_L x5,  2  * SZREG(sp)
	REG_L x6,  3  * SZREG(sp)
	REG_L x7,  4  * SZREG(sp)
	REG_L x8,  5  * SZREG(sp)
	REG_L x9,  6  * SZREG(sp)
	REG_L x10, 7  * SZREG(sp)
	REG_L x11, 8  * SZREG(sp)
	REG_L x12, 9  * SZREG(sp)
	REG_L x13, 10 * SZREG(sp)
	REG_L x14, 11 * SZREG(sp)
	REG_L x15, 12 * SZREG(sp)
	REG_L x16, 13 * SZREG(sp)
	REG_L x17, 14 * SZREG(sp)
	REG_L x18, 15 * SZREG(sp)
	REG_L x19, 16 * SZREG(sp)
	REG_L x20, 17 * SZREG(sp)
	REG_L x21, 18 * SZREG(sp)
	REG_L x22, 19 * SZREG(sp)
	REG_L x23, 20 * SZREG(sp)
	REG_L x24, 21 * SZREG(sp)
	REG_L x25, 22 * SZREG(sp)
	REG_L x26, 23 * SZREG(sp)
	REG_L x27, 24 * SZREG(sp)
	REG_L x28, 25 * SZREG(sp)
	REG_L x29, 26 * SZREG(sp)
	REG_L x30, 27 * SZREG(sp)
	REG_L x31, 28 * SZREG(sp)

	addi sp, sp, (29 * SZREG)
	sret

.align 8
.global Os_StartUpResumeContext
.weak Os_StartUpResumeContext
Os_StartUpResumeContext:
#if __SIZEOF_POINTER__ == 8
	ld t0, Os_ArchTempSp
#else
	lwu t0, Os_ArchTempSp
#endif
	mv sp, t0

	/* Pop PC from stack and set MEPC */
	REG_L t0,  0  * SZREG(sp)
	csrw CSR_EPC, t0

	/* Pop mstatus from stack and set it */
	REG_L t0,  29  * SZREG(sp)
	csrw CSR_STATUS, t0

	/* Restore Registers from Stack */
	REG_L x1,  1  * SZREG(sp)    /* RA */
	REG_L x5,  2  * SZREG(sp)
	REG_L x6,  3  * SZREG(sp)
	REG_L x7,  4  * SZREG(sp)
	REG_L x8,  5  * SZREG(sp)
	REG_L x9,  6  * SZREG(sp)
	REG_L x10, 7  * SZREG(sp)
	REG_L x11, 8  * SZREG(sp)
	REG_L x12, 9  * SZREG(sp)
	REG_L x13, 10 * SZREG(sp)
	REG_L x14, 11 * SZREG(sp)
	REG_L x15, 12 * SZREG(sp)
	REG_L x16, 13 * SZREG(sp)
	REG_L x17, 14 * SZREG(sp)
	REG_L x18, 15 * SZREG(sp)
	REG_L x19, 16 * SZREG(sp)
	REG_L x20, 17 * SZREG(sp)
	REG_L x21, 18 * SZREG(sp)
	REG_L x22, 19 * SZREG(sp)
	REG_L x23, 20 * SZREG(sp)
	REG_L x24, 21 * SZREG(sp)
	REG_L x25, 22 * SZREG(sp)
	REG_L x26, 23 * SZREG(sp)
	REG_L x27, 24 * SZREG(sp)
	REG_L x28, 25 * SZREG(sp)
	REG_L x29, 26 * SZREG(sp)
	REG_L x30, 27 * SZREG(sp)
	REG_L x31, 28 * SZREG(sp)

	addi sp, sp, (29 * SZREG)

	/*enble isr*/
	csrrsi  zero, CSR_STATUS, 0x2

	sret
