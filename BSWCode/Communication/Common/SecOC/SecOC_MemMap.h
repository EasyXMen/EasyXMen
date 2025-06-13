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
**  FILENAME    : SecOC_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of SecOC              **
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
    ModeName:SecOC_MemMap<br>
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
#elif defined SECOC_START_SEC_CALLOUT_CODE
#undef SECOC_START_SEC_CALLOUT_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_CALLOUT_CODE_NO_MATCH
#define START_SEC_CALLOUT_CODE

#elif defined SECOC_STOP_SEC_CALLOUT_CODE
#undef SECOC_STOP_SEC_CALLOUT_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_CALLOUT_CODE_NO_MATCH
#undef SECOC_START_SEC_CALLOUT_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLOUT_CODE

/*Index:1  Section name :CODE*/
#elif defined SECOC_START_SEC_CODE
#undef SECOC_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined SECOC_STOP_SEC_CODE
#undef SECOC_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_CODE_NO_MATCH
#undef SECOC_START_SEC_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:2  Section name :CONST_UNSPECIFIED*/
#elif defined SECOC_START_SEC_CONST_UNSPECIFIED
#undef SECOC_START_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#define START_SEC_CONST_UNSPECIFIED

#elif defined SECOC_STOP_SEC_CONST_UNSPECIFIED
#undef SECOC_STOP_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#undef SECOC_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_UNSPECIFIED

/*Index:3  Section name :SECOCCOPYRXDATA_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCCOPYRXDATA_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCCOPYRXDATA_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCCOPYRXDATA_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCCOPYRXDATA_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCCOPYRXDATA_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCCOPYRXDATA_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCCOPYRXDATA_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:4  Section name :SECOCCOPYTXDATA_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCCOPYTXDATA_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCCOPYTXDATA_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCCOPYTXDATA_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCCOPYTXDATA_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCCOPYTXDATA_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCCOPYTXDATA_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCCOPYTXDATA_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:5  Section name :SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCCSMGENERATEJOBFINISHEDINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:6  Section name :SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCCSMVERIFYJOBFINISHEDINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:7  Section name :SECOCRXINDICATION_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCRXINDICATION_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCRXINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCRXINDICATION_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCRXINDICATION_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCRXINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:8  Section name :SECOCSTARTOFRECEPTION_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCSTARTOFRECEPTION_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:9  Section name :SECOCTPRXINDICATION_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCTPRXINDICATION_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCTPRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCTPRXINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCTPRXINDICATION_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCTPRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCTPRXINDICATION_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCTPRXINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:10  Section name :SECOCTPTXCONFIRMATION_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCTPTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:11  Section name :SECOCTRIGGERTRANSMIT_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCTRIGGERTRANSMIT_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:12  Section name :SECOCTXCONFIRMATION_CALLBACK_CODE*/
#elif defined SECOC_START_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE
#undef SECOC_START_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined SECOC_STOP_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE
#undef SECOC_STOP_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#undef SECOC_START_SEC_SECOCTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:13  Section name :VAR_CLEARED_BOOLEAN*/
#elif defined SECOC_START_SEC_VAR_CLEARED_BOOLEAN
#undef SECOC_START_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#define START_SEC_VAR_CLEARED_BOOLEAN

#elif defined SECOC_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef SECOC_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#undef SECOC_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_BOOLEAN

/*Index:14  Section name :VAR_CLEARED_UNSPECIFIED*/
#elif defined SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED
#undef SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_UNSPECIFIED

#elif defined SECOC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#undef SECOC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#undef SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_UNSPECIFIED

/*Index:15  Section name :VAR_INIT_PTR*/
#elif defined SECOC_START_SEC_VAR_INIT_PTR
#undef SECOC_START_SEC_VAR_INIT_PTR
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_VAR_INIT_PTR_NO_MATCH
#define START_SEC_VAR_INIT_PTR

#elif defined SECOC_STOP_SEC_VAR_INIT_PTR
#undef SECOC_STOP_SEC_VAR_INIT_PTR
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_VAR_INIT_PTR_NO_MATCH
#undef SECOC_START_SEC_VAR_INIT_PTR_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_PTR

/*Index:16  Section name :VAR_INIT_UNSPECIFIED*/
#elif defined SECOC_START_SEC_VAR_INIT_UNSPECIFIED
#undef SECOC_START_SEC_VAR_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "SecOC_MemMap.h, section is used consecutively many times."
#endif
#define SECOC_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_INIT_UNSPECIFIED

#elif defined SECOC_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef SECOC_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef SECOC_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#undef SECOC_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#else
#error "SecOC_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_UNSPECIFIED

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "SecOC_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883,0791-- */ /* MISAR Dir-4.10,Rule-5.4 */
