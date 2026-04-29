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
**  FILENAME    : iLib_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of ComM              **
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

/* Total section items: 8 */
#if defined(START_WITH_IF)
#undef MEMMAP_ERROR

/*Index:0  Section name :CODE*/
#elif defined ILIB_START_SEC_CODE
#undef ILIB_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "iLib_MemMap.h, section is used consecutively many times."
#endif
#define ILIB_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined ILIB_STOP_SEC_CODE
#undef ILIB_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef ILIB_START_SEC_CODE_NO_MATCH
#undef ILIB_START_SEC_CODE_NO_MATCH
#else
#error "iLib_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

#elif defined ILIB_START_SEC_CONST
#undef ILIB_START_SEC_CONST
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "iLib_MemMap.h, section is used consecutively many times."
#endif
#define ILIB_START_SEC_CONST_NO_MATCH
#define START_SEC_CODE

#elif defined ILIB_STOP_SEC_CONST
#undef ILIB_STOP_SEC_CONST
#undef MEMMAP_ERROR
#ifdef ILIB_START_SEC_CONST_NO_MATCH
#undef ILIB_START_SEC_CONST_NO_MATCH
#else
#error "iLib_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "iLib_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883-- */
