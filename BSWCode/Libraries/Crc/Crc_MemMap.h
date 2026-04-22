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
/*
********************************************************************************
**                                                                            **
**  FILENAME    : Crc_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of Crc              **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                       **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 * Version: V1.0.0
 * Note:    Automatically generated, manual modification is not allowed.
 */
/*============================================================================*/

/* PRQA S 0883++ */ /* VL_QAC_include */
#define MEMMAP_ERROR

/* Total section items: 5 */
#if defined(START_WITH_IF)
#undef MEMMAP_ERROR

/*Index:0  Section name :CODE*/
#elif defined CRC_START_SEC_CODE
#undef CRC_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Crc_MemMap.h, section is used consecutively many times."
#endif
#define CRC_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined CRC_STOP_SEC_CODE
#undef CRC_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef CRC_START_SEC_CODE_NO_MATCH
#undef CRC_START_SEC_CODE_NO_MATCH
#else
#error "Crc_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:1  Section name :CONST_16BIT*/
#elif defined CRC_START_SEC_CONST_16BIT
#undef CRC_START_SEC_CONST_16BIT
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Crc_MemMap.h, section is used consecutively many times."
#endif
#define CRC_START_SEC_CONST_16BIT_NO_MATCH
#define START_SEC_CONST_16BIT

#elif defined CRC_STOP_SEC_CONST_16BIT
#undef CRC_STOP_SEC_CONST_16BIT
#undef MEMMAP_ERROR
#ifdef CRC_START_SEC_CONST_16BIT_NO_MATCH
#undef CRC_START_SEC_CONST_16BIT_NO_MATCH
#else
#error "Crc_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_16BIT

/*Index:2  Section name :CONST_32BIT*/
#elif defined CRC_START_SEC_CONST_32BIT
#undef CRC_START_SEC_CONST_32BIT
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Crc_MemMap.h, section is used consecutively many times."
#endif
#define CRC_START_SEC_CONST_32BIT_NO_MATCH
#define START_SEC_CONST_32BIT

#elif defined CRC_STOP_SEC_CONST_32BIT
#undef CRC_STOP_SEC_CONST_32BIT
#undef MEMMAP_ERROR
#ifdef CRC_START_SEC_CONST_32BIT_NO_MATCH
#undef CRC_START_SEC_CONST_32BIT_NO_MATCH
#else
#error "Crc_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_32BIT

/*Index:3  Section name :CONST_64BIT*/
#elif defined CRC_START_SEC_CONST_64BIT
#undef CRC_START_SEC_CONST_64BIT
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Crc_MemMap.h, section is used consecutively many times."
#endif
#define CRC_START_SEC_CONST_64BIT_NO_MATCH
#define START_SEC_CONST_64BIT

#elif defined CRC_STOP_SEC_CONST_64BIT
#undef CRC_STOP_SEC_CONST_64BIT
#undef MEMMAP_ERROR
#ifdef CRC_START_SEC_CONST_64BIT_NO_MATCH
#undef CRC_START_SEC_CONST_64BIT_NO_MATCH
#else
#error "Crc_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_64BIT

/*Index:4  Section name :CONST_8BIT*/
#elif defined CRC_START_SEC_CONST_8BIT
#undef CRC_START_SEC_CONST_8BIT
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Crc_MemMap.h, section is used consecutively many times."
#endif
#define CRC_START_SEC_CONST_8BIT_NO_MATCH
#define START_SEC_CONST_8BIT

#elif defined CRC_STOP_SEC_CONST_8BIT
#undef CRC_STOP_SEC_CONST_8BIT
#undef MEMMAP_ERROR
#ifdef CRC_START_SEC_CONST_8BIT_NO_MATCH
#undef CRC_START_SEC_CONST_8BIT_NO_MATCH
#else
#error "Crc_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_8BIT

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "Crc_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883-- */
