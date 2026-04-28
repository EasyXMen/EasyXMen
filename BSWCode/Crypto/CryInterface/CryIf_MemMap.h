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
**  FILENAME    : CryIf_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of CryIf              **
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

/* PRQA S 0883,0791++ */ /* VL_QAC_include, VL_QAC_identifier*/
#define MEMMAP_ERROR

/* Total section items: 4 */
#if defined(START_WITH_IF)
#undef MEMMAP_ERROR

/*Index:0  Section name :CALLBACK_CODE_NOTIFICATION_CODE*/
#elif defined CRYIF_START_SEC_CALLBACK_CODE_NOTIFICATION_CODE
#undef CRYIF_START_SEC_CALLBACK_CODE_NOTIFICATION_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "CryIf_MemMap.h, section is used consecutively many times."
#endif
#define CRYIF_START_SEC_CALLBACK_CODE_NOTIFICATION_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined CRYIF_STOP_SEC_CALLBACK_CODE_NOTIFICATION_CODE
#undef CRYIF_STOP_SEC_CALLBACK_CODE_NOTIFICATION_CODE
#undef MEMMAP_ERROR
#ifdef CRYIF_START_SEC_CALLBACK_CODE_NOTIFICATION_CODE_NO_MATCH
#undef CRYIF_START_SEC_CALLBACK_CODE_NOTIFICATION_CODE_NO_MATCH
#else
#error "CryIf_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:1  Section name :CODE*/
#elif defined CRYIF_START_SEC_CODE
#undef CRYIF_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "CryIf_MemMap.h, section is used consecutively many times."
#endif
#define CRYIF_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined CRYIF_STOP_SEC_CODE
#undef CRYIF_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef CRYIF_START_SEC_CODE_NO_MATCH
#undef CRYIF_START_SEC_CODE_NO_MATCH
#else
#error "CryIf_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:2  Section name :CONST_UNSPECIFIED*/
#elif defined CRYIF_START_SEC_CONST_UNSPECIFIED
#undef CRYIF_START_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "CryIf_MemMap.h, section is used consecutively many times."
#endif
#define CRYIF_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#define START_SEC_CONST_UNSPECIFIED

#elif defined CRYIF_STOP_SEC_CONST_UNSPECIFIED
#undef CRYIF_STOP_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef CRYIF_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#undef CRYIF_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#else
#error "CryIf_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_UNSPECIFIED

/*Index:3  Section name :VAR_CLEARED_BOOLEAN*/
#elif defined CRYIF_START_SEC_VAR_CLEARED_BOOLEAN
#undef CRYIF_START_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "CryIf_MemMap.h, section is used consecutively many times."
#endif
#define CRYIF_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#define START_SEC_VAR_CLEARED_BOOLEAN

#elif defined CRYIF_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef CRYIF_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifdef CRYIF_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#undef CRYIF_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#else
#error "CryIf_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_BOOLEAN

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "CryIf_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883,0791-- */ /* VL_QAC_include, VL_QAC_identifier*/
