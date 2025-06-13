/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Os_MemmapImp.h                                              **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of MemMap            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 * Note: Automatically generated, script version V1.0.0
 */
/*============================================================================*/

/*=======[M E M M A P  S Y M B O L  D E F I N E]==============================*/
#if defined(START_WITH_IF)
/*Index:10  Section name :VAR_CLEARED_CORE0_32*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE0_32
#undef OS_START_SEC_VAR_CLEARED_CORE0_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE0_32_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE0_32

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE0_32
#undef OS_STOP_SEC_VAR_CLEARED_CORE0_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE0_32_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE0_32_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE0_32

/*Index:11  Section name :VAR_CLEARED_CORE1_32*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE1_32
#undef OS_START_SEC_VAR_CLEARED_CORE1_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE1_32_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE1_32

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE1_32
#undef OS_STOP_SEC_VAR_CLEARED_CORE1_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE1_32_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE1_32_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE1_32

/*Index:12  Section name :VAR_CLEARED_CORE2_32*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE2_32
#undef OS_START_SEC_VAR_CLEARED_CORE2_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE2_32_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE2_32

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE2_32
#undef OS_STOP_SEC_VAR_CLEARED_CORE2_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE2_32_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE2_32_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE2_32

/*Index:13  Section name :VAR_CLEARED_CORE3_32*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE3_32
#undef OS_START_SEC_VAR_CLEARED_CORE3_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE3_32_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE3_32

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE3_32
#undef OS_STOP_SEC_VAR_CLEARED_CORE3_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE3_32_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE3_32_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE3_32

/*Index:14  Section name :VAR_CLEARED_CORE4_32*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE4_32
#undef OS_START_SEC_VAR_CLEARED_CORE4_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE4_32_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE4_32

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE4_32
#undef OS_STOP_SEC_VAR_CLEARED_CORE4_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE4_32_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE4_32_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE4_32

/*Index:15  Section name :VAR_CLEARED_CORE5_32*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE5_32
#undef OS_START_SEC_VAR_CLEARED_CORE5_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE5_32_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE5_32

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE5_32
#undef OS_STOP_SEC_VAR_CLEARED_CORE5_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE5_32_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE5_32_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE5_32

/*Index:16  Section name :VAR_CLEARED_CORE0_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#undef OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED

/*Index:17  Section name :VAR_CLEARED_CORE1_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#undef OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED

/*Index:18  Section name :VAR_CLEARED_CORE2_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#undef OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED

/*Index:19  Section name :VAR_CLEARED_CORE3_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#undef OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED

/*Index:20  Section name :VAR_CLEARED_CORE4_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#undef OS_START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED

/*Index:21  Section name :VAR_CLEARED_CORE5_UNSPECIFIED*/
#elif defined OS_START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#undef OS_START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED

#elif defined OS_STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#undef OS_STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED_NO_MATCH
#undef OS_START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED

/*Index:22  Section name :VAR_STACK_CORE0*/
#elif defined OS_START_SEC_VAR_STACK_CORE0
#undef OS_START_SEC_VAR_STACK_CORE0
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_STACK_CORE0_NO_MATCH
#define START_SEC_VAR_STACK_CORE0

#elif defined OS_STOP_SEC_VAR_STACK_CORE0
#undef OS_STOP_SEC_VAR_STACK_CORE0
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_STACK_CORE0_NO_MATCH
#undef OS_START_SEC_VAR_STACK_CORE0_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_STACK_CORE0

/*Index:23  Section name :VAR_STACK_CORE1*/
#elif defined OS_START_SEC_VAR_STACK_CORE1
#undef OS_START_SEC_VAR_STACK_CORE1
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_STACK_CORE1_NO_MATCH
#define START_SEC_VAR_STACK_CORE1

#elif defined OS_STOP_SEC_VAR_STACK_CORE1
#undef OS_STOP_SEC_VAR_STACK_CORE1
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_STACK_CORE1_NO_MATCH
#undef OS_START_SEC_VAR_STACK_CORE1_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_STACK_CORE1

/*Index:24  Section name :VAR_STACK_CORE2*/
#elif defined OS_START_SEC_VAR_STACK_CORE2
#undef OS_START_SEC_VAR_STACK_CORE2
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_STACK_CORE2_NO_MATCH
#define START_SEC_VAR_STACK_CORE2

#elif defined OS_STOP_SEC_VAR_STACK_CORE2
#undef OS_STOP_SEC_VAR_STACK_CORE2
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_STACK_CORE2_NO_MATCH
#undef OS_START_SEC_VAR_STACK_CORE2_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_STACK_CORE2

/*Index:25  Section name :VAR_STACK_CORE3*/
#elif defined OS_START_SEC_VAR_STACK_CORE3
#undef OS_START_SEC_VAR_STACK_CORE3
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_STACK_CORE3_NO_MATCH
#define START_SEC_VAR_STACK_CORE3

#elif defined OS_STOP_SEC_VAR_STACK_CORE3
#undef OS_STOP_SEC_VAR_STACK_CORE3
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_STACK_CORE3_NO_MATCH
#undef OS_START_SEC_VAR_STACK_CORE3_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_STACK_CORE3

/*Index:26  Section name :VAR_STACK_CORE4*/
#elif defined OS_START_SEC_VAR_STACK_CORE4
#undef OS_START_SEC_VAR_STACK_CORE4
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_STACK_CORE4_NO_MATCH
#define START_SEC_VAR_STACK_CORE4

#elif defined OS_STOP_SEC_VAR_STACK_CORE4
#undef OS_STOP_SEC_VAR_STACK_CORE4
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_STACK_CORE4_NO_MATCH
#undef OS_START_SEC_VAR_STACK_CORE4_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_STACK_CORE4

/*Index:27  Section name :VAR_STACK_CORE5*/
#elif defined OS_START_SEC_VAR_STACK_CORE5
#undef OS_START_SEC_VAR_STACK_CORE5
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_STACK_CORE5_NO_MATCH
#define START_SEC_VAR_STACK_CORE5

#elif defined OS_STOP_SEC_VAR_STACK_CORE5
#undef OS_STOP_SEC_VAR_STACK_CORE5
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_STACK_CORE5_NO_MATCH
#undef OS_START_SEC_VAR_STACK_CORE5_NO_MATCH
#else
#error "Os_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_STACK_CORE5
#endif /* START_WITH_IF */

/*Code*/
#if defined(START_SEC_CODE)
#undef START_SEC_CODE
#pragma section ".Os.code.defult" ax

/* stop pragma */
#elif defined(STOP_SEC_CODE)
#undef STOP_SEC_CODE
#pragma section

/* start pragma */
#elif defined(START_SEC_CODE_FAST)
#undef START_SEC_CODE_FAST
#pragma section ".Os.code.defult" ax

/* stop pragma */
#elif defined(STOP_SEC_CODE_FAST)
#undef STOP_SEC_CODE_FAST
#pragma section

/* start pragma */
#elif defined(START_SEC_CONST_PTR)
#undef START_SEC_CONST_PTR
#pragma section ".Os.const.ptr" a

/* stop pragma */
#elif defined(STOP_SEC_CONST_PTR)
#undef STOP_SEC_CONST_PTR
#pragma section

/* start pragma */
#elif defined(START_SEC_CONST_UNSPECIFIED)
#undef START_SEC_CONST_UNSPECIFIED
#pragma section ".Os.const.unspec" a

/* stop pragma */
#elif defined(STOP_SEC_CONST_UNSPECIFIED)
#undef STOP_SEC_CONST_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLONE_16)
#undef START_SEC_VAR_CLONE_16
#pragma section ".Os.clear.clone.16bit" aw 2

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLONE_16)
#undef STOP_SEC_VAR_CLONE_16
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLONE_32)
#undef START_SEC_VAR_CLONE_32
#pragma section ".Os.clear.clone.32bit" aw 2

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLONE_32)
#undef STOP_SEC_VAR_CLONE_32
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLONE_UNSPECIFIED)
#undef START_SEC_VAR_CLONE_UNSPECIFIED
#pragma section ".Os.clear.clone.32bit" aw 2

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLONE_UNSPECIFIED)
#undef STOP_SEC_VAR_CLONE_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLONE_8)
#undef START_SEC_VAR_CLONE_8
#pragma section ".Os.clear.clone.8bit" aw 1

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLONE_8)
#undef STOP_SEC_VAR_CLONE_8
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLONE_PTR)
#undef START_SEC_VAR_CLONE_PTR
#pragma section ".Os.clear.clone.ptr" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLONE_PTR)
#undef STOP_SEC_VAR_CLONE_PTR
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE0_32)
#undef START_SEC_VAR_CLEARED_CORE0_32
#pragma section ".Os.clear.core0.32bit" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE0_32)
#undef STOP_SEC_VAR_CLEARED_CORE0_32
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#pragma section ".Os.clear.core0.unspec" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE1_32)
#undef START_SEC_VAR_CLEARED_CORE1_32
#pragma section ".Os.clear.core1.32bit" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE1_32)
#undef STOP_SEC_VAR_CLEARED_CORE1_32
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#pragma section ".Os.clear.core1.unspec" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE2_32)
#undef START_SEC_VAR_CLEARED_CORE2_32
#pragma section ".Os.clear.core2.32bit" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE2_32)
#undef STOP_SEC_VAR_CLEARED_CORE2_32
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#pragma section ".Os.clear.core2.unspec" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE3_32)
#undef START_SEC_VAR_CLEARED_CORE3_32
#pragma section ".Os.clear.core3.32bit" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE3_32)
#undef STOP_SEC_VAR_CLEARED_CORE3_32
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#pragma section ".Os.clear.core3.unspec" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE4_32)
#undef START_SEC_VAR_CLEARED_CORE4_32
#pragma section ".Os.clear.core4.32bit" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE4_32)
#undef STOP_SEC_VAR_CLEARED_CORE4_32
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#pragma section ".Os.clear.core4.unspec" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE5_32)
#undef START_SEC_VAR_CLEARED_CORE5_32
#pragma section ".Os.clear.core5.32bit" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE5_32)
#undef STOP_SEC_VAR_CLEARED_CORE5_32
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#pragma section ".Os.clear.core5.unspec" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_GLOBAL_32)
#undef START_SEC_VAR_CLEARED_GLOBAL_32
#pragma section ".Os.clear.global.32bit" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_GLOBAL_32)
#undef STOP_SEC_VAR_CLEARED_GLOBAL_32
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#pragma section ".Os.clear.global.unspec" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_CONFIG_DATA_UNSPECIFIED)
#undef START_SEC_VAR_CONFIG_DATA_UNSPECIFIED
#pragma section ".Os.config.data.unspec" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_CONFIG_DATA_UNSPECIFIED)
#undef STOP_SEC_VAR_CONFIG_DATA_UNSPECIFIED
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_STACK_CORE0)
#undef START_SEC_VAR_STACK_CORE0
#pragma section ".Os.stack.core0" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_STACK_CORE0)
#undef STOP_SEC_VAR_STACK_CORE0
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_STACK_CORE1)
#undef START_SEC_VAR_STACK_CORE1
#pragma section ".Os.stack.core1" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_STACK_CORE1)
#undef STOP_SEC_VAR_STACK_CORE1
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_STACK_CORE2)
#undef START_SEC_VAR_STACK_CORE2
#pragma section ".Os.stack.core2" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_STACK_CORE2)
#undef STOP_SEC_VAR_STACK_CORE2
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_STACK_CORE3)
#undef START_SEC_VAR_STACK_CORE3
#pragma section ".Os.stack.core3" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_STACK_CORE3)
#undef STOP_SEC_VAR_STACK_CORE3
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_STACK_CORE4)
#undef START_SEC_VAR_STACK_CORE4
#pragma section ".Os.stack.core4" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_STACK_CORE4)
#undef STOP_SEC_VAR_STACK_CORE4
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_STACK_CORE5)
#undef START_SEC_VAR_STACK_CORE5
#pragma section ".Os.stack.core5" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_STACK_CORE5)
#undef STOP_SEC_VAR_STACK_CORE5
#pragma section

/* start pragma */
#elif defined(START_SEC_VAR_POWER_ON_CLEARED)
#undef START_SEC_VAR_POWER_ON_CLEARED
#pragma section ".Os.Test.ON.cleared" aw 4

/* stop pragma */
#elif defined(STOP_SEC_VAR_POWER_ON_CLEARED)
#undef STOP_SEC_VAR_POWER_ON_CLEARED
#pragma section
#else
#error "MemMap.h, wrong pragma command"
#endif
/* PRQA S 0883 ++*/ /* OS_MEMMAPIMP_INCLUCDE_001 */
/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883 --*/
