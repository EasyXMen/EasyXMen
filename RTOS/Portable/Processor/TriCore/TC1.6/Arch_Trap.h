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
**  FILENAME    : Arch_Trap.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : functions related to the processor exception                **
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
typedef P2FUNC(void, OS_APPL_CODE, Os_traphnd)(uint16 Os_Tin);
/* PRQA S 3432 */ /* MISRA Rule 20.7 */ /* ARCH_DEFINE_COMPILER_001 */
/*=======[E X T E R N A L   D A T A]==========================================*/
extern VAR(uint32, OS_VAR) TriCore_trap_table[]; /* PRQA S 3684 */ /* MISRA Rule 8.11 */ /*ARCH_TRAP_DATADEFINE_001*/
/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
extern FUNC(void, OS_CODE) Os_ArchDispatcher(void);
extern FUNC(void, OS_CODE) Trap0(uint16 Os_Tin);
extern FUNC(void, OS_CODE) Trap1(uint16 Os_Tin);
extern FUNC(void, OS_CODE) Trap2(uint16 Os_Tin);
extern FUNC(void, OS_CODE) Trap3(uint16 Os_Tin);
extern FUNC(void, OS_CODE) Trap4(uint16 Os_Tin);
extern FUNC(void, OS_CODE) Trap5(uint16 Os_Tin);
extern FUNC(void, OS_CODE) Os_SystemCallTrap(uint16 Os_Tin);
extern FUNC(void, OS_CODE) Trap7(uint16 Os_Tin);
extern VAR(Os_traphnd, OS_VAR) Tdisptab[OS_MAX_TRAPS];
/*=======[I N T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#endif /* OS_TRAP_H */
/*=======[E N D   O F   F I L E]==============================================*/
