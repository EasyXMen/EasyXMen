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

#elif defined OS_START_SEC_VAR_INTVECTOR_GLOBAL_32
#undef OS_START_SEC_VAR_INTVECTOR_GLOBAL_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_INTVECTOR_GLOBAL_32_NO_MATCH
#define START_SEC_VAR_INTVECTOR_GLOBAL_32

#elif defined OS_STOP_SEC_VAR_INTVECTOR_GLOBAL_32
#undef OS_STOP_SEC_VAR_INTVECTOR_GLOBAL_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_INTVECTOR_GLOBAL_32_NO_MATCH
#undef OS_START_SEC_VAR_INTVECTOR_GLOBAL_32_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INTVECTOR_GLOBAL_32

#elif defined OS_START_SEC_VAR_INTVECTOR_LOCAL_32
#undef OS_START_SEC_VAR_INTVECTOR_LOCAL_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_INTVECTOR_LOCAL_32_NO_MATCH
#define START_SEC_VAR_INTVECTOR_LOCAL_32

#elif defined OS_STOP_SEC_VAR_INTVECTOR_LOCAL_32
#undef OS_STOP_SEC_VAR_INTVECTOR_LOCAL_32
#undef MEMMAP_ERROR
#ifdef OS_START_SEC_VAR_INTVECTOR_LOCAL_32_NO_MATCH
#undef OS_START_SEC_VAR_INTVECTOR_LOCAL_32_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INTVECTOR_LOCAL_32
#endif /* START_WITH_IF */

/*Code*/
#if defined(START_SEC_CODE)
#undef START_SEC_CODE
#endif

/* stop pragma */
#if defined(STOP_SEC_CODE)
#undef STOP_SEC_CODE
#endif

/* start pragma */
#if defined(START_SEC_CONST_PTR)
#undef START_SEC_CONST_PTR
#endif

/* stop pragma */
#if defined(STOP_SEC_CONST_PTR)
#undef STOP_SEC_CONST_PTR
#endif

/* start pragma */
#if defined(START_SEC_CONST_UNSPECIFIED)
#undef START_SEC_CONST_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_CONST_UNSPECIFIED)
#undef STOP_SEC_CONST_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLONE_16)
#undef START_SEC_VAR_CLONE_16
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLONE_16)
#undef STOP_SEC_VAR_CLONE_16
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLONE_32)
#undef START_SEC_VAR_CLONE_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLONE_32)
#undef STOP_SEC_VAR_CLONE_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLONE_8)
#undef START_SEC_VAR_CLONE_8
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLONE_8)
#undef STOP_SEC_VAR_CLONE_8
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLONE_UNSPECIFIED)
#undef START_SEC_VAR_CLONE_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLONE_UNSPECIFIED)
#undef STOP_SEC_VAR_CLONE_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLONE_PTR)
#undef START_SEC_VAR_CLONE_PTR
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLONE_PTR)
#undef STOP_SEC_VAR_CLONE_PTR
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE0_32)
#undef START_SEC_VAR_CLEARED_CORE0_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE0_32)
#undef STOP_SEC_VAR_CLEARED_CORE0_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE0_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE1_32)
#undef START_SEC_VAR_CLEARED_CORE1_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE1_32)
#undef STOP_SEC_VAR_CLEARED_CORE1_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE1_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE2_32)
#undef START_SEC_VAR_CLEARED_CORE2_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE2_32)
#undef STOP_SEC_VAR_CLEARED_CORE2_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE2_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE3_32)
#undef START_SEC_VAR_CLEARED_CORE3_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE3_32)
#undef STOP_SEC_VAR_CLEARED_CORE3_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE3_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE4_32)
#undef START_SEC_VAR_CLEARED_CORE4_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE4_32)
#undef STOP_SEC_VAR_CLEARED_CORE4_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE4_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE5_32)
#undef START_SEC_VAR_CLEARED_CORE5_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE5_32)
#undef STOP_SEC_VAR_CLEARED_CORE5_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_CORE5_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_GLOBAL_32)
#undef START_SEC_VAR_CLEARED_GLOBAL_32
__attribute__((section(".Os_global")))
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_GLOBAL_32)
#undef STOP_SEC_VAR_CLEARED_GLOBAL_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED)
#undef START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
__attribute__((section(".Os_global")))
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED)
#undef STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CLONE_16)
#undef START_SEC_VAR_INIT_CLONE_16
#pragma default_variable_attributes = @ ".BswDataShared"
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CLONE_16)
#undef STOP_SEC_VAR_INIT_CLONE_16
#pragma default_variable_attributes =
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CLONE_32)
#undef START_SEC_VAR_INIT_CLONE_32
#pragma default_variable_attributes = @ ".BswDataShared"
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CLONE_32)
#undef STOP_SEC_VAR_INIT_CLONE_32
#pragma default_variable_attributes =
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CLONE_8)
#undef START_SEC_VAR_INIT_CLONE_8
#pragma default_variable_attributes = @ ".BswDataShared"
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CLONE_8)
#undef STOP_SEC_VAR_INIT_CLONE_8
#pragma default_variable_attributes =
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CLONE_UNSPECIFIED)
#undef START_SEC_VAR_INIT_CLONE_UNSPECIFIED
#pragma default_variable_attributes = @ ".BswDataShared"
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CLONE_UNSPECIFIED)
#undef STOP_SEC_VAR_INIT_CLONE_UNSPECIFIED
#pragma default_variable_attributes =
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CLONE_PTR)
#undef START_SEC_VAR_INIT_CLONE_PTR
#pragma default_variable_attributes = @ ".BswDataShared"
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CLONE_PTR)
#undef STOP_SEC_VAR_INIT_CLONE_PTR
#pragma default_variable_attributes =
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE0_32)
#undef START_SEC_VAR_INIT_CORE0_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE0_32)
#undef STOP_SEC_VAR_INIT_CORE0_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE0_UNSPECIFIED)
#undef START_SEC_VAR_INIT_CORE0_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE0_UNSPECIFIED)
#undef STOP_SEC_VAR_INIT_CORE0_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE1_32)
#undef START_SEC_VAR_INIT_CORE1_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE1_32)
#undef STOP_SEC_VAR_INIT_CORE1_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE1_UNSPECIFIED)
#undef START_SEC_VAR_INIT_CORE1_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE1_UNSPECIFIED)
#undef STOP_SEC_VAR_INIT_CORE1_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE2_32)
#undef START_SEC_VAR_INIT_CORE2_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE2_32)
#undef STOP_SEC_VAR_INIT_CORE2_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE2_UNSPECIFIED)
#undef START_SEC_VAR_INIT_CORE2_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE2_UNSPECIFIED)
#undef STOP_SEC_VAR_INIT_CORE2_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE3_32)
#undef START_SEC_VAR_INIT_CORE3_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE3_32)
#undef STOP_SEC_VAR_INIT_CORE3_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE3_UNSPECIFIED)
#undef START_SEC_VAR_INIT_CORE3_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE3_UNSPECIFIED)
#undef STOP_SEC_VAR_INIT_CORE3_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE4_32)
#undef START_SEC_VAR_INIT_CORE4_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE4_32)
#undef STOP_SEC_VAR_INIT_CORE4_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE4_UNSPECIFIED)
#undef START_SEC_VAR_INIT_CORE4_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE4_UNSPECIFIED)
#undef STOP_SEC_VAR_INIT_CORE4_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE5_32)
#undef START_SEC_VAR_INIT_CORE5_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE5_32)
#undef STOP_SEC_VAR_INIT_CORE5_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_CORE5_UNSPECIFIED)
#undef START_SEC_VAR_INIT_CORE5_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_CORE5_UNSPECIFIED)
#undef STOP_SEC_VAR_INIT_CORE5_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_GLOBAL_32)
#undef START_SEC_VAR_INIT_GLOBAL_32
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_GLOBAL_32)
#undef STOP_SEC_VAR_INIT_GLOBAL_32
#pragma default_variable_attributes =
#endif

/* start pragma */
#if defined(START_SEC_VAR_INIT_GLOBAL_UNSPECIFIED)
#undef START_SEC_VAR_INIT_GLOBAL_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INIT_GLOBAL_UNSPECIFIED)
#undef STOP_SEC_VAR_INIT_GLOBAL_UNSPECIFIED
#pragma default_variable_attributes =
#endif

/* start pragma */
#if defined(START_SEC_VAR_CONFIG_DATA_UNSPECIFIED)
#undef START_SEC_VAR_CONFIG_DATA_UNSPECIFIED
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_CONFIG_DATA_UNSPECIFIED)
#undef STOP_SEC_VAR_CONFIG_DATA_UNSPECIFIED
#endif

/* start pragma */
#if defined(START_SEC_VAR_STACK_CORE0)
#undef START_SEC_VAR_STACK_CORE0
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_STACK_CORE0)
#undef STOP_SEC_VAR_STACK_CORE0
#endif

/* start pragma */
#if defined(START_SEC_VAR_STACK_CORE1)
#undef START_SEC_VAR_STACK_CORE1
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_STACK_CORE1)
#undef STOP_SEC_VAR_STACK_CORE1
#endif

/* start pragma */
#if defined(START_SEC_VAR_STACK_CORE2)
#undef START_SEC_VAR_STACK_CORE2
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_STACK_CORE2)
#undef STOP_SEC_VAR_STACK_CORE2
#endif

/* start pragma */
#if defined(START_SEC_VAR_STACK_CORE3)
#undef START_SEC_VAR_STACK_CORE3
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_STACK_CORE3)
#undef STOP_SEC_VAR_STACK_CORE3
#endif

/* start pragma */
#if defined(START_SEC_VAR_STACK_CORE4)
#undef START_SEC_VAR_STACK_CORE4
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_STACK_CORE4)
#undef STOP_SEC_VAR_STACK_CORE4
#endif

/* start pragma */
#if defined(START_SEC_VAR_STACK_CORE5)
#undef START_SEC_VAR_STACK_CORE5
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_STACK_CORE5)
#undef STOP_SEC_VAR_STACK_CORE5
#endif

/* start pragma */
#if defined(START_SEC_VAR_POWER_ON_CLEARED)
#undef START_SEC_VAR_POWER_ON_CLEARED
__attribute__((section(".Os.Test.ON.cleared")))
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_POWER_ON_CLEARED)
#undef STOP_SEC_VAR_INIT_GLOBAL_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_INTVECTOR_GLOBAL_32)
#undef START_SEC_VAR_INTVECTOR_GLOBAL_32
__attribute__((section(".intvet_gtable"), aligned(4)))
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INTVECTOR_GLOBAL_32)
#undef STOP_SEC_VAR_INTVECTOR_GLOBAL_32
#endif

/* start pragma */
#if defined(START_SEC_VAR_INTVECTOR_LOCAL_32)
#undef START_SEC_VAR_INTVECTOR_LOCAL_32
__attribute__((section(".intvet_ltable"), aligned(4)))
#endif

/* stop pragma */
#if defined(STOP_SEC_VAR_INTVECTOR_LOCAL_32)
#undef STOP_SEC_VAR_INTVECTOR_LOCAL_32
#endif

/*=======[E N D   O F   F I L E]============*//* PRQA S 0883 */ /* MISRA Dir-4.10 */
