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
**  FILENAME    : PduR_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of PduR              **
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
    ModeName:PduR_MemMap<br>
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

/* Total section items: 23 */
#if defined(START_WITH_IF)
#undef MEMMAP_ERROR

/*Index:0  Section name :CODE*/
#elif defined PDUR_START_SEC_CODE
#undef PDUR_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined PDUR_STOP_SEC_CODE
#undef PDUR_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_CODE_NO_MATCH
#undef PDUR_START_SEC_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:1  Section name :CONFIG_DATA_UNSPECIFIED*/
#elif defined PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#define START_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#undef PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONFIG_DATA_UNSPECIFIED

/*Index:2  Section name :IFRXINDICATION_CALLBACK_CODE*/
#elif defined PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE
#undef PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined PDUR_STOP_SEC_IFRXINDICATION_CALLBACK_CODE
#undef PDUR_STOP_SEC_IFRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE_NO_MATCH
#undef PDUR_START_SEC_IFRXINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:3  Section name :IFTRIGGERTRANSMIT_CALLBACK_CODE*/
#elif defined PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
#undef PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined PDUR_STOP_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
#undef PDUR_STOP_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE_NO_MATCH
#undef PDUR_START_SEC_IFTRIGGERTRANSMIT_CALLBACK_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:4  Section name :IFTXCONFIRMATION_CALLBACK_CODE*/
#elif defined PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE
#undef PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined PDUR_STOP_SEC_IFTXCONFIRMATION_CALLBACK_CODE
#undef PDUR_STOP_SEC_IFTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#undef PDUR_START_SEC_IFTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:5  Section name :CONFIG_DATA_16*/
#elif defined PDUR_START_SEC_CONFIG_DATA_16
#undef PDUR_START_SEC_CONFIG_DATA_16
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_CONFIG_DATA_16_NO_MATCH
#define START_SEC_CONFIG_DATA_16

#elif defined PDUR_STOP_SEC_CONFIG_DATA_16
#undef PDUR_STOP_SEC_CONFIG_DATA_16
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_CONFIG_DATA_16_NO_MATCH
#undef PDUR_START_SEC_CONFIG_DATA_16_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONFIG_DATA_16

/*Index:6  Section name :CONFIG_DATA_UNSPECIFIED*/
#elif defined PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#define START_SEC_CONFIG_DATA_UNSPECIFIED

#elif defined PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#undef PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONFIG_DATA_UNSPECIFIED

/*Index:7  Section name :PBCONFIG_DATA_8*/
#elif defined PDUR_START_SEC_PBCONFIG_DATA_8
#undef PDUR_START_SEC_PBCONFIG_DATA_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_PBCONFIG_DATA_8_NO_MATCH
#define START_SEC_PBCONFIG_DATA_8

#elif defined PDUR_STOP_SEC_PBCONFIG_DATA_8
#undef PDUR_STOP_SEC_PBCONFIG_DATA_8
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_PBCONFIG_DATA_8_NO_MATCH
#undef PDUR_START_SEC_PBCONFIG_DATA_8_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_PBCONFIG_DATA_8

/*Index:8  Section name :PBCONFIG_DATA_16*/
#elif defined PDUR_START_SEC_PBCONFIG_DATA_16
#undef PDUR_START_SEC_PBCONFIG_DATA_16
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_PBCONFIG_DATA_16_NO_MATCH
#define START_SEC_PBCONFIG_DATA_16

#elif defined PDUR_STOP_SEC_PBCONFIG_DATA_16
#undef PDUR_STOP_SEC_PBCONFIG_DATA_16
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_PBCONFIG_DATA_16_NO_MATCH
#undef PDUR_START_SEC_PBCONFIG_DATA_16_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_PBCONFIG_DATA_16

/*Index:9  Section name :PBCONFIG_DATA_UNSPECIFIED*/
#elif defined PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#undef PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED_NO_MATCH
#define START_SEC_PBCONFIG_DATA_UNSPECIFIED

#elif defined PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#undef PDUR_STOP_SEC_PBCONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED_NO_MATCH
#undef PDUR_START_SEC_PBCONFIG_DATA_UNSPECIFIED_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_PBCONFIG_DATA_UNSPECIFIED

/*Index:10  Section name :TPCOPYRXDATA_CALLBACK_CODE*/
#elif defined PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE
#undef PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined PDUR_STOP_SEC_TPCOPYRXDATA_CALLBACK_CODE
#undef PDUR_STOP_SEC_TPCOPYRXDATA_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE_NO_MATCH
#undef PDUR_START_SEC_TPCOPYRXDATA_CALLBACK_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:11  Section name :TPCOPYTXDATA_CALLBACK_CODE*/
#elif defined PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE
#undef PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined PDUR_STOP_SEC_TPCOPYTXDATA_CALLBACK_CODE
#undef PDUR_STOP_SEC_TPCOPYTXDATA_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE_NO_MATCH
#undef PDUR_START_SEC_TPCOPYTXDATA_CALLBACK_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:12  Section name :TPRXINDICATION_CALLBACK_CODE*/
#elif defined PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE
#undef PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined PDUR_STOP_SEC_TPRXINDICATION_CALLBACK_CODE
#undef PDUR_STOP_SEC_TPRXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE_NO_MATCH
#undef PDUR_START_SEC_TPRXINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:13  Section name :TPSTARTOFRECEPTION_CALLBACK_CODE*/
#elif defined PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
#undef PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined PDUR_STOP_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
#undef PDUR_STOP_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE_NO_MATCH
#undef PDUR_START_SEC_TPSTARTOFRECEPTION_CALLBACK_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:14  Section name :TPTXCONFIRMATION_CALLBACK_CODE*/
#elif defined PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE
#undef PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined PDUR_STOP_SEC_TPTXCONFIRMATION_CALLBACK_CODE
#undef PDUR_STOP_SEC_TPTXCONFIRMATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#undef PDUR_START_SEC_TPTXCONFIRMATION_CALLBACK_CODE_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:15  Section name :VAR_INIT_8*/
#elif defined PDUR_START_SEC_VAR_INIT_8
#undef PDUR_START_SEC_VAR_INIT_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_INIT_8_NO_MATCH
#define START_SEC_VAR_INIT_8

#elif defined PDUR_STOP_SEC_VAR_INIT_8
#undef PDUR_STOP_SEC_VAR_INIT_8
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_INIT_8_NO_MATCH
#undef PDUR_START_SEC_VAR_INIT_8_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_8

/*Index:16  Section name :VAR_INIT_PTR*/
#elif defined PDUR_START_SEC_VAR_INIT_PTR
#undef PDUR_START_SEC_VAR_INIT_PTR
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_INIT_PTR_NO_MATCH
#define START_SEC_VAR_INIT_PTR

#elif defined PDUR_STOP_SEC_VAR_INIT_PTR
#undef PDUR_STOP_SEC_VAR_INIT_PTR
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_INIT_PTR_NO_MATCH
#undef PDUR_START_SEC_VAR_INIT_PTR_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_PTR

/*Index:17  Section name :VAR_INIT_UNSPECIFIED*/
#elif defined PDUR_START_SEC_VAR_INIT_UNSPECIFIED
#undef PDUR_START_SEC_VAR_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_INIT_UNSPECIFIED

#elif defined PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#undef PDUR_START_SEC_VAR_INIT_UNSPECIFIED_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_INIT_UNSPECIFIED

/*Index:18  Section name :VAR_NO_INIT_8*/
#elif defined PDUR_START_SEC_VAR_NO_INIT_8
#undef PDUR_START_SEC_VAR_NO_INIT_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_NO_INIT_8_NO_MATCH
#define START_SEC_VAR_NO_INIT_8

#elif defined PDUR_STOP_SEC_VAR_NO_INIT_8
#undef PDUR_STOP_SEC_VAR_NO_INIT_8
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_NO_INIT_8_NO_MATCH
#undef PDUR_START_SEC_VAR_NO_INIT_8_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_NO_INIT_8

/*Index:19  Section name :VAR_NO_INIT_BOOLEAN*/
#elif defined PDUR_START_SEC_VAR_NO_INIT_BOOLEAN
#undef PDUR_START_SEC_VAR_NO_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_NO_INIT_BOOLEAN_NO_MATCH
#define START_SEC_VAR_NO_INIT_BOOLEAN

#elif defined PDUR_STOP_SEC_VAR_NO_INIT_BOOLEAN
#undef PDUR_STOP_SEC_VAR_NO_INIT_BOOLEAN
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_NO_INIT_BOOLEAN_NO_MATCH
#undef PDUR_START_SEC_VAR_NO_INIT_BOOLEAN_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_NO_INIT_BOOLEAN

/*Index:20  Section name :VAR_NO_INIT_UNSPECIFIED*/
#elif defined PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED
#undef PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_NO_INIT_UNSPECIFIED

#elif defined PDUR_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#undef PDUR_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED_NO_MATCH
#undef PDUR_START_SEC_VAR_NO_INIT_UNSPECIFIED_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_NO_INIT_UNSPECIFIED

/*Index:21  Section name :VAR_CLEARED_8*/
#elif defined PDUR_START_SEC_VAR_CLEARED_8
#undef PDUR_START_SEC_VAR_CLEARED_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_CLEARED_8_NO_MATCH
#define START_SEC_VAR_CLEARED_8

#elif defined PDUR_STOP_SEC_VAR_CLEARED_8
#undef PDUR_STOP_SEC_VAR_CLEARED_8
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_CLEARED_8_NO_MATCH
#undef PDUR_START_SEC_VAR_CLEARED_8_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_8

/*Index:22  Section name :VAR_CLEARED_BOOLEAN*/
#elif defined PDUR_START_SEC_VAR_CLEARED_BOOLEAN
#undef PDUR_START_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#define START_SEC_VAR_CLEARED_BOOLEAN

#elif defined PDUR_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef PDUR_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#undef PDUR_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_BOOLEAN

/*Index:22  Section name :VAR_CLEARED_PTR*/
#elif defined PDUR_START_SEC_VAR_CLEARED_PTR
#undef PDUR_START_SEC_VAR_CLEARED_PTR
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "PduR_MemMap.h, section is used consecutively many times."
#endif
#define PDUR_START_SEC_VAR_CLEARED_PTR_NO_MATCH
#define START_SEC_VAR_CLEARED_PTR

#elif defined PDUR_STOP_SEC_VAR_CLEARED_PTR
#undef PDUR_STOP_SEC_VAR_CLEARED_PTR
#undef MEMMAP_ERROR
#ifdef PDUR_START_SEC_VAR_CLEARED_PTR_NO_MATCH
#undef PDUR_START_SEC_VAR_CLEARED_PTR_NO_MATCH
#else
#error "PduR_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_PTR

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "PduR_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883,0791-- */ /* MISAR Dir-4.10,Rule-5.4 */
