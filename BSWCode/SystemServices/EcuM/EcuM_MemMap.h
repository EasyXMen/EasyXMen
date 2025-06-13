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
**  FILENAME    : EcuM_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of EcuM              **
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
    ModeName:EcuM_MemMap<br>
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

/* Total section items: 17 */
#if defined(START_WITH_IF)
#undef MEMMAP_ERROR

/*Index:0  Section name :CALLOUT_CODE*/
#elif defined ECUM_START_SEC_CALLOUT_CODE
#undef ECUM_START_SEC_CALLOUT_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_CALLOUT_CODE_NO_MATCH
#define START_SEC_CALLOUT_CODE

#elif defined ECUM_STOP_SEC_CALLOUT_CODE
#undef ECUM_STOP_SEC_CALLOUT_CODE
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_CALLOUT_CODE_NO_MATCH
#undef ECUM_START_SEC_CALLOUT_CODE_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLOUT_CODE

/*Index:1  Section name :CHECKWAKEUP_CALLBACK_CODE*/
#elif defined ECUM_START_SEC_CHECKWAKEUP_CALLBACK_CODE
#undef ECUM_START_SEC_CHECKWAKEUP_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_CHECKWAKEUP_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined ECUM_STOP_SEC_CHECKWAKEUP_CALLBACK_CODE
#undef ECUM_STOP_SEC_CHECKWAKEUP_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_CHECKWAKEUP_CALLBACK_CODE_NO_MATCH
#undef ECUM_START_SEC_CHECKWAKEUP_CALLBACK_CODE_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:2  Section name :CODE*/
#elif defined ECUM_START_SEC_CODE
#undef ECUM_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined ECUM_STOP_SEC_CODE
#undef ECUM_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_CODE_NO_MATCH
#undef ECUM_START_SEC_CODE_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:3  Section name :CONFIG_DATA_16*/
#elif defined ECUM_START_SEC_CONFIG_DATA_16
#undef ECUM_START_SEC_CONFIG_DATA_16
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_CONFIG_DATA_16_NO_MATCH
#define START_SEC_CONFIG_DATA_16

#elif defined ECUM_STOP_SEC_CONFIG_DATA_16
#undef ECUM_STOP_SEC_CONFIG_DATA_16
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_CONFIG_DATA_16_NO_MATCH
#undef ECUM_START_SEC_CONFIG_DATA_16_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONFIG_DATA_16

/*Index:4  Section name :CONFIG_DATA_8*/
#elif defined ECUM_START_SEC_CONFIG_DATA_8
#undef ECUM_START_SEC_CONFIG_DATA_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_CONFIG_DATA_8_NO_MATCH
#define START_SEC_CONFIG_DATA_8

#elif defined ECUM_STOP_SEC_CONFIG_DATA_8
#undef ECUM_STOP_SEC_CONFIG_DATA_8
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_CONFIG_DATA_8_NO_MATCH
#undef ECUM_START_SEC_CONFIG_DATA_8_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONFIG_DATA_8

/*Index:5  Section name :CONFIG_DATA_UNSPECIFIED*/
#elif defined ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#define START_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined ECUM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef ECUM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#undef ECUM_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONFIG_DATA_UNSPECIFIED

/*Index:6  Section name :CONST_32*/
#elif defined ECUM_START_SEC_CONST_32
#undef ECUM_START_SEC_CONST_32
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_CONST_32_NO_MATCH
#define START_SEC_CONST_32

#elif defined ECUM_STOP_SEC_CONST_32
#undef ECUM_STOP_SEC_CONST_32
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_CONST_32_NO_MATCH
#undef ECUM_START_SEC_CONST_32_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_32

/*Index:7  Section name :PBCFG_GLOBALROOT*/
#elif defined ECUM_START_SEC_PBCFG_GLOBALROOT
#undef ECUM_START_SEC_PBCFG_GLOBALROOT
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_PBCFG_GLOBALROOT_NO_MATCH
#define START_SEC_PBCFG_GLOBALROOT

#elif defined ECUM_STOP_SEC_PBCFG_GLOBALROOT
#undef ECUM_STOP_SEC_PBCFG_GLOBALROOT
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_PBCFG_GLOBALROOT_NO_MATCH
#undef ECUM_START_SEC_PBCFG_GLOBALROOT_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_PBCFG_GLOBALROOT

/*Index:8  Section name :SETWAKEUPEVENT_CALLBACK_CODE*/
#elif defined ECUM_START_SEC_SETWAKEUPEVENT_CALLBACK_CODE
#undef ECUM_START_SEC_SETWAKEUPEVENT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_SETWAKEUPEVENT_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined ECUM_STOP_SEC_SETWAKEUPEVENT_CALLBACK_CODE
#undef ECUM_STOP_SEC_SETWAKEUPEVENT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_SETWAKEUPEVENT_CALLBACK_CODE_NO_MATCH
#undef ECUM_START_SEC_SETWAKEUPEVENT_CALLBACK_CODE_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:9  Section name :VALIDATEWAKEUPEVENT_CALLBACK_CODE*/
#elif defined ECUM_START_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE
#undef ECUM_START_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined ECUM_STOP_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE
#undef ECUM_STOP_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE_NO_MATCH
#undef ECUM_START_SEC_VALIDATEWAKEUPEVENT_CALLBACK_CODE_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:10  Section name :VAR_CLEARED_16*/
#elif defined ECUM_START_SEC_VAR_CLEARED_16
#undef ECUM_START_SEC_VAR_CLEARED_16
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_VAR_CLEARED_16_NO_MATCH
#define START_SEC_VAR_CLEARED_16

#elif defined ECUM_STOP_SEC_VAR_CLEARED_16
#undef ECUM_STOP_SEC_VAR_CLEARED_16
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_VAR_CLEARED_16_NO_MATCH
#undef ECUM_START_SEC_VAR_CLEARED_16_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_16

/*Index:11  Section name :VAR_CLEARED_SHARE_16*/
#elif defined ECUM_START_SEC_VAR_CLEARED_SHARE_16
#undef ECUM_START_SEC_VAR_CLEARED_SHARE_16
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_VAR_CLEARED_SHARE_16_NO_MATCH
#define START_SEC_VAR_CLEARED_SHARE_16

#elif defined ECUM_STOP_SEC_VAR_CLEARED_SHARE_16
#undef ECUM_STOP_SEC_VAR_CLEARED_SHARE_16
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_VAR_CLEARED_SHARE_16_NO_MATCH
#undef ECUM_START_SEC_VAR_CLEARED_SHARE_16_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_SHARE_16

/*Index:12  Section name :VAR_CLEARED_SHARE_BOOLEAN*/
#elif defined ECUM_START_SEC_VAR_CLEARED_SHARE_BOOLEAN
#undef ECUM_START_SEC_VAR_CLEARED_SHARE_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_VAR_CLEARED_SHARE_BOOLEAN_NO_MATCH
#define START_SEC_VAR_CLEARED_SHARE_BOOLEAN

#elif defined ECUM_STOP_SEC_VAR_CLEARED_SHARE_BOOLEAN
#undef ECUM_STOP_SEC_VAR_CLEARED_SHARE_BOOLEAN
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_VAR_CLEARED_SHARE_BOOLEAN_NO_MATCH
#undef ECUM_START_SEC_VAR_CLEARED_SHARE_BOOLEAN_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_SHARE_BOOLEAN

/*Index:13  Section name :VAR_CLEARED_SHARE_UNSPECIFIED*/
#elif defined ECUM_START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED
#undef ECUM_START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED

#elif defined ECUM_STOP_SEC_VAR_CLEARED_SHARE_UNSPECIFIED
#undef ECUM_STOP_SEC_VAR_CLEARED_SHARE_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED_NO_MATCH
#undef ECUM_START_SEC_VAR_CLEARED_SHARE_UNSPECIFIED_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_SHARE_UNSPECIFIED

/*Index:14  Section name :VAR_CLEARED_UNSPECIFIED*/
#elif defined ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#undef ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_UNSPECIFIED

#elif defined ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#undef ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#undef ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_UNSPECIFIED

/*Index:15  Section name :VAR_CLONE_CLEARED_UNSPECIFIED*/
#elif defined ECUM_START_SEC_VAR_CLONE_CLEARED_UNSPECIFIED
#undef ECUM_START_SEC_VAR_CLONE_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_VAR_CLONE_CLEARED_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLONE_CLEARED_UNSPECIFIED

#elif defined ECUM_STOP_SEC_VAR_CLONE_CLEARED_UNSPECIFIED
#undef ECUM_STOP_SEC_VAR_CLONE_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_VAR_CLONE_CLEARED_UNSPECIFIED_NO_MATCH
#undef ECUM_START_SEC_VAR_CLONE_CLEARED_UNSPECIFIED_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLONE_CLEARED_UNSPECIFIED

/*Index:16  Section name :VAR_CLONE_INIT_BOOLEAN*/
#elif defined ECUM_START_SEC_VAR_CLONE_INIT_BOOLEAN
#undef ECUM_START_SEC_VAR_CLONE_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EcuM_MemMap.h, section is used consecutively many times."
#endif
#define ECUM_START_SEC_VAR_CLONE_INIT_BOOLEAN_NO_MATCH
#define START_SEC_VAR_CLONE_INIT_BOOLEAN

#elif defined ECUM_STOP_SEC_VAR_CLONE_INIT_BOOLEAN
#undef ECUM_STOP_SEC_VAR_CLONE_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifdef ECUM_START_SEC_VAR_CLONE_INIT_BOOLEAN_NO_MATCH
#undef ECUM_START_SEC_VAR_CLONE_INIT_BOOLEAN_NO_MATCH
#else
#error "EcuM_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLONE_INIT_BOOLEAN

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "EcuM_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883,0791-- */ /* MISAR Dir-4.10,Rule-5.4 */
