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
**  FILENAME    : EthTSyn_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of EthTSyn              **
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
    ModeName:EthTSyn_MemMap<br>
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

/* Total section items: 8 */
#if defined(START_WITH_IF)
#undef MEMMAP_ERROR

/*Index:0  Section name :CODE*/
#elif defined ETHTSYN_START_SEC_CODE
#undef ETHTSYN_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EthTSyn_MemMap.h, section is used consecutively many times."
#endif
#define ETHTSYN_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined ETHTSYN_STOP_SEC_CODE
#undef ETHTSYN_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef ETHTSYN_START_SEC_CODE_NO_MATCH
#undef ETHTSYN_START_SEC_CODE_NO_MATCH
#else
#error "EthTSyn_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:1  Section name :CONST_8*/
#elif defined ETHTSYN_START_SEC_CONST_8
#undef ETHTSYN_START_SEC_CONST_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EthTSyn_MemMap.h, section is used consecutively many times."
#endif
#define ETHTSYN_START_SEC_CONST_8_NO_MATCH
#define START_SEC_CONST_8

#elif defined ETHTSYN_STOP_SEC_CONST_8
#undef ETHTSYN_STOP_SEC_CONST_8
#undef MEMMAP_ERROR
#ifdef ETHTSYN_START_SEC_CONST_8_NO_MATCH
#undef ETHTSYN_START_SEC_CONST_8_NO_MATCH
#else
#error "EthTSyn_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_8

/*Index:2  Section name :CONST_UNSPECIFIED*/
#elif defined ETHTSYN_START_SEC_CONST_UNSPECIFIED
#undef ETHTSYN_START_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EthTSyn_MemMap.h, section is used consecutively many times."
#endif
#define ETHTSYN_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#define START_SEC_CONST_UNSPECIFIED

#elif defined ETHTSYN_STOP_SEC_CONST_UNSPECIFIED
#undef ETHTSYN_STOP_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef ETHTSYN_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#undef ETHTSYN_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#else
#error "EthTSyn_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_UNSPECIFIED

/*Index:3  Section name :ETHTSYNRXINDICATION_CALLBACK_CODE*/
#elif defined ETHTSYN_START_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE
#undef ETHTSYN_START_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EthTSyn_MemMap.h, section is used consecutively many times."
#endif
#define ETHTSYN_START_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined ETHTSYN_STOP_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE
#undef ETHTSYN_STOP_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef ETHTSYN_START_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE_NO_MATCH
#undef ETHTSYN_START_SEC_ETHTSYNRXINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "EthTSyn_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:4  Section name :ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE*/
#elif defined ETHTSYN_START_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE
#undef ETHTSYN_START_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EthTSyn_MemMap.h, section is used consecutively many times."
#endif
#define ETHTSYN_START_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined ETHTSYN_STOP_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE
#undef ETHTSYN_STOP_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef ETHTSYN_START_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE_NO_MATCH
#undef ETHTSYN_START_SEC_ETHTSYNTRCVLINKSTATECHG_CALLBACK_CODE_NO_MATCH
#else
#error "EthTSyn_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:5  Section name :ETHTSYNTXCONFIRMATION_CALLBACK_CODE*/
#elif defined ETHTSYN_START_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE
#undef ETHTSYN_START_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EthTSyn_MemMap.h, section is used consecutively many times."
#endif
#define ETHTSYN_START_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined ETHTSYN_STOP_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE
#undef ETHTSYN_STOP_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef ETHTSYN_START_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#undef ETHTSYN_START_SEC_ETHTSYNTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#else
#error "EthTSyn_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:6  Section name :VAR_CLEARED_BOOLEAN*/
#elif defined ETHTSYN_START_SEC_VAR_CLEARED_BOOLEAN
#undef ETHTSYN_START_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EthTSyn_MemMap.h, section is used consecutively many times."
#endif
#define ETHTSYN_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#define START_SEC_VAR_CLEARED_BOOLEAN

#elif defined ETHTSYN_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef ETHTSYN_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifdef ETHTSYN_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#undef ETHTSYN_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#else
#error "EthTSyn_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_BOOLEAN

/*Index:7  Section name :VAR_CLEARED_UNSPECIFIED*/
#elif defined ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#undef ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "EthTSyn_MemMap.h, section is used consecutively many times."
#endif
#define ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_UNSPECIFIED

#elif defined ETHTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#undef ETHTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#undef ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#else
#error "EthTSyn_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_UNSPECIFIED

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "EthTSyn_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883,0791-- */ /* MISAR Dir-4.10,Rule-5.4 */
