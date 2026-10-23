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
**  FILENAME    : Os_Hal_Compiler.h                                           **
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

#ifndef OS_HAL_COMPILER_H
#define OS_HAL_COMPILER_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define OS_SYNTAX_UNIFIED .syntax unified
#define OS_THUMB          .thumb
#define OS_EXTERN         .extern
#define THUMB_FUNC        .thumb_func
#define OS_WEAK_GLOBAL_ASM .weak

#define OS_BEGIN_NO_OPTIMIZE __attribute__((optimize("O0")))
#define OS_END_NO_OPTIMIZE

#define OS_ASM   __asm volatile

#ifndef OS_ALWAYS_INLINE
#define OS_ALWAYS_INLINE __attribute__((always_inline))
#endif

#ifndef OS_ALIGN
#define OS_ALIGN(bytes) __attribute__((aligned(bytes)))
#endif
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
#endif
/*=======[E N D   O F   F I L E]==============================================*/
