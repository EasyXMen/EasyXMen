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
**                                                                            **
**  FILENAME    : MemMapImp.h                                                 **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of MemMap            **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 * Note: Automatically generated, script version V1.0.0
 */
/*============================================================================*/

/*=======[M E M M A P  S Y M B O L  D E F I N E]==============================*/
#if defined(START_WITH_IF)

#elif defined OS_START_SEC_VAR_INTVECTOR_GLOBAL_32
#undef OS_START_SEC_VAR_INTVECTOR_GLOBAL_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_INTVECTOR_GLOBAL_32_NO_MATCH
__attribute__((section(".intvet_gtable")))

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

#elif defined OS_START_SEC_VAR_INTVECTOR_LOCAL_32
#undef OS_START_SEC_VAR_INTVECTOR_LOCAL_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Os_MemMap.h, section is used consecutively many times."
#endif
#define OS_START_SEC_VAR_INTVECTOR_LOCAL_32_NO_MATCH
__attribute__((section(".intvet_ltable")))

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

#endif /* START_WITH_IF */

/*=======[E N D   O F   F I L E]==============================================*/
