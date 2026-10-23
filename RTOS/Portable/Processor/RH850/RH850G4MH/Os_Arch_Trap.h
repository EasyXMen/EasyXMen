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
 **  FILENAME    : Os_Arch_Trap.h                                         **
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
#ifndef ARCH_TRAP_H
#define ARCH_TRAP_H

/*it's not standard autosar error status*/
#define E_OS_System_Error                     (25U)
#define E_OS_Reserved_Instruction_Exception   (26U)
#define E_OS_Coprocessor_Unusable_Exception   (27U)
#define E_OS_Privilege_Instruction_Exception  (28U)
#define E_OS_Misalignment_Exception           (29U)
#define E_OS_Unknown_Exception                (30U)


typedef struct
{
    uint32* Sp;
    uint32  ExceptionSource;
    uint32  Psw;
    uint32  Ra;
} Os_ArchTrapMsgType;
#endif /* ARCH_TRAP_H */
