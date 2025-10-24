/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
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
/*******************************************************************************
**                                                                            **
**  FILENAME    : Arch_Trap.h                                                  **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Interrupt Request Managment                                 **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
#ifndef ARCH_TRAP_H
#define ARCH_TRAP_H

/*=======[I N C L U D E S]====================================================*/
#include "Compiler.h"
#include "Platform_Types.h"

/*=======[M A C R O S]========================================================*/
#define OS_MAX_TRAPS (8u)
/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/* Type of the trap. */
typedef void (*Os_traphnd)(uint16 Os_Tin); /* PRQA S 3432 */ /* VL_Os_3472 */

/*=======[E X T E R N A L   D A T A]==========================================*/
extern uint32 TriCore_trap_table[]; /* PRQA S 3684 */ /* VL_Os_3684 */
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
extern void       Os_ArchDispatcher(void); /* PRQA S 1512 */ /* VL_Os_1512 */
extern void       Trap0(uint16 Os_Tin);
extern void       Trap1(uint16 Os_Tin);
extern void       Trap2(uint16 Os_Tin);
extern void       Trap3(uint16 Os_Tin);
extern void       Trap4(uint16 Os_Tin);
extern void       Trap5(uint16 Os_Tin);
extern void       Os_SystemCallTrap(uint16 Os_Tin);
extern void       Trap7(uint16 Os_Tin);
extern Os_traphnd Tdisptab[OS_MAX_TRAPS]; /* PRQA S 3672 */ /* VL_Os_3672 */
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#endif /* OS_TRAP_H */
/*=======[E N D   O F   F I L E]==============================================*/
