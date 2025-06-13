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
**  FILENAME    : Dem_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of Dem              **
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
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Dem_MemMap<br>
  RuleSorce:puhua-rule.rcf 2.3.1

   \li PRQA S 0883 MISRA Dir 4.10 .<br>
    Reason:<module>_MemMap.h needs to be dynamically referenced multiple times based on pre compiled macros,
            so that duplicate inclusion protection cannot be used.

    \li PRQA S 0791 MISRA Rule 5.4 .<br>
    Reason:According to the segment name definition rules of the autosar specification, segment names may be
            very long and only a few characters may differ between segment names.
 */

/* PRQA S 0883,0791++ */ /* MISAR Dir-4.10,Rule-5.4 */
#define MEMMAP_ERROR

/* Total section items: 14 */
#if defined(START_WITH_IF)
#undef MEMMAP_ERROR

/*Index:0  Section name :CODE*/
#elif defined DEM_START_SEC_CODE
#undef DEM_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined DEM_STOP_SEC_CODE
#undef DEM_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_CODE_NO_MATCH
#undef DEM_START_SEC_CODE_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:1  Section name :CONST_UNSPECIFIED*/
#elif defined DEM_START_SEC_CONST_UNSPECIFIED
#undef DEM_START_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#define START_SEC_CONST_UNSPECIFIED

#elif defined DEM_STOP_SEC_CONST_UNSPECIFIED
#undef DEM_STOP_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#undef DEM_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_UNSPECIFIED

/*Index:2  Section name :PBCFG_GLOBALROOT*/
#elif defined DEM_START_SEC_PBCFG_GLOBALROOT
#undef DEM_START_SEC_PBCFG_GLOBALROOT
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_PBCFG_GLOBALROOT_NO_MATCH
#define START_SEC_PBCFG_GLOBALROOT

#elif defined DEM_STOP_SEC_PBCFG_GLOBALROOT
#undef DEM_STOP_SEC_PBCFG_GLOBALROOT
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_PBCFG_GLOBALROOT_NO_MATCH
#undef DEM_START_SEC_PBCFG_GLOBALROOT_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_PBCFG_GLOBALROOT

/*Index:3  Section name :VAR_INIT_16*/
#elif defined DEM_START_SEC_VAR_INIT_16
#undef DEM_START_SEC_VAR_INIT_16
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_INIT_16_NO_MATCH
#define START_SEC_VAR_INIT_16

#elif defined DEM_STOP_SEC_VAR_INIT_16
#undef DEM_STOP_SEC_VAR_INIT_16
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_INIT_16_NO_MATCH
#undef DEM_START_SEC_VAR_INIT_16_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_16

/*Index:4  Section name :VAR_INIT_8*/
#elif defined DEM_START_SEC_VAR_INIT_8
#undef DEM_START_SEC_VAR_INIT_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_INIT_8_NO_MATCH
#define START_SEC_VAR_INIT_8

#elif defined DEM_STOP_SEC_VAR_INIT_8
#undef DEM_STOP_SEC_VAR_INIT_8
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_INIT_8_NO_MATCH
#undef DEM_START_SEC_VAR_INIT_8_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_8

/*Index:5  Section name :VAR_INIT_BOOLEAN*/
#elif defined DEM_START_SEC_VAR_INIT_BOOLEAN
#undef DEM_START_SEC_VAR_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_INIT_BOOLEAN_NO_MATCH
#define START_SEC_VAR_INIT_BOOLEAN

#elif defined DEM_STOP_SEC_VAR_INIT_BOOLEAN
#undef DEM_STOP_SEC_VAR_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_INIT_BOOLEAN_NO_MATCH
#undef DEM_START_SEC_VAR_INIT_BOOLEAN_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_BOOLEAN

/*Index:6  Section name :VAR_INIT_PTR*/
#elif defined DEM_START_SEC_VAR_INIT_PTR
#undef DEM_START_SEC_VAR_INIT_PTR
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_INIT_PTR_NO_MATCH
#define START_SEC_VAR_INIT_PTR

#elif defined DEM_STOP_SEC_VAR_INIT_PTR
#undef DEM_STOP_SEC_VAR_INIT_PTR
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_INIT_PTR_NO_MATCH
#undef DEM_START_SEC_VAR_INIT_PTR_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_PTR

/*Index:7  Section name :VAR_INIT_UNSPECIFIED*/
#elif defined DEM_START_SEC_VAR_INIT_UNSPECIFIED
#undef DEM_START_SEC_VAR_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_INIT_UNSPECIFIED

#elif defined DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#undef DEM_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_UNSPECIFIED

/*Index:8  Section name :VAR_NO_INIT_16*/
#elif defined DEM_START_SEC_VAR_NO_INIT_16
#undef DEM_START_SEC_VAR_NO_INIT_16
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_NO_INIT_16_NO_MATCH
#define START_SEC_VAR_NO_INIT_16

#elif defined DEM_STOP_SEC_VAR_NO_INIT_16
#undef DEM_STOP_SEC_VAR_NO_INIT_16
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_NO_INIT_16_NO_MATCH
#undef DEM_START_SEC_VAR_NO_INIT_16_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_NO_INIT_16

/*Index:9  Section name :VAR_NO_INIT_8*/
#elif defined DEM_START_SEC_VAR_NO_INIT_8
#undef DEM_START_SEC_VAR_NO_INIT_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_NO_INIT_8_NO_MATCH
#define START_SEC_VAR_NO_INIT_8

#elif defined DEM_STOP_SEC_VAR_NO_INIT_8
#undef DEM_STOP_SEC_VAR_NO_INIT_8
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_NO_INIT_8_NO_MATCH
#undef DEM_START_SEC_VAR_NO_INIT_8_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_NO_INIT_8

/*Index:10  Section name :VAR_NO_INIT_BOOLEAN*/
#elif defined DEM_START_SEC_VAR_NO_INIT_BOOLEAN
#undef DEM_START_SEC_VAR_NO_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_NO_INIT_BOOLEAN_NO_MATCH
#define START_SEC_VAR_NO_INIT_BOOLEAN

#elif defined DEM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#undef DEM_STOP_SEC_VAR_NO_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_NO_INIT_BOOLEAN_NO_MATCH
#undef DEM_START_SEC_VAR_NO_INIT_BOOLEAN_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_NO_INIT_BOOLEAN

/*Index:11  Section name :VAR_NO_INIT_UNSPECIFIED*/
#elif defined DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#undef DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_NO_INIT_UNSPECIFIED

#elif defined DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#undef DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED_NO_MATCH
#undef DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_NO_INIT_UNSPECIFIED

/*Index:12  Section name :VAR_POWER_ON_INIT_8*/
#elif defined DEM_START_SEC_VAR_POWER_ON_INIT_8
#undef DEM_START_SEC_VAR_POWER_ON_INIT_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_POWER_ON_INIT_8_NO_MATCH
#define START_SEC_VAR_POWER_ON_INIT_8

#elif defined DEM_STOP_SEC_VAR_POWER_ON_INIT_8
#undef DEM_STOP_SEC_VAR_POWER_ON_INIT_8
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_POWER_ON_INIT_8_NO_MATCH
#undef DEM_START_SEC_VAR_POWER_ON_INIT_8_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_POWER_ON_INIT_8

/*Index:13  Section name :VAR_POWER_ON_INIT_BOOLEAN*/
#elif defined DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#undef DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Dem_MemMap.h, section is used consecutively many times."
#endif
#define DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN_NO_MATCH
#define START_SEC_VAR_POWER_ON_INIT_BOOLEAN

#elif defined DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#undef DEM_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifdef DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN_NO_MATCH
#undef DEM_START_SEC_VAR_POWER_ON_INIT_BOOLEAN_NO_MATCH
#else
#error "Dem_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "Dem_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883,0791-- */ /* MISAR Dir-4.10,Rule-5.4 */
