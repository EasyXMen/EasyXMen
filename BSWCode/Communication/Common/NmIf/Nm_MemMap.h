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
**  FILENAME    : Nm_MemMap.h                                               **
**                                                                            **
**  Created on  :                                                             **
**  Author      : <Auto generated>                                            **
**  Vendor      :                                                             **
**  DESCRIPTION : Memory mapping abstraction declaration of Nm              **
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
    ModeName:Nm_MemMap<br>
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

/* Total section items: 20 */
#if defined(START_WITH_IF)
#undef MEMMAP_ERROR

/*Index:0  Section name :CODE*/
#elif defined NM_START_SEC_CODE
#undef NM_START_SEC_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_CODE_NO_MATCH
#define START_SEC_CODE

#elif defined NM_STOP_SEC_CODE
#undef NM_STOP_SEC_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_CODE_NO_MATCH
#undef NM_START_SEC_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CODE

/*Index:1  Section name :CONST_UNSPECIFIED*/
#elif defined NM_START_SEC_CONST_UNSPECIFIED
#undef NM_START_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#define START_SEC_CONST_UNSPECIFIED

#elif defined NM_STOP_SEC_CONST_UNSPECIFIED
#undef NM_STOP_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#undef NM_START_SEC_CONST_UNSPECIFIED_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CONST_UNSPECIFIED

/*Index:2  Section name :NM_BUSSLEEPMODE_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_BUSSLEEPMODE_CALLBACK_CODE
#undef NM_START_SEC_NM_BUSSLEEPMODE_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_BUSSLEEPMODE_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_BUSSLEEPMODE_CALLBACK_CODE
#undef NM_STOP_SEC_NM_BUSSLEEPMODE_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_BUSSLEEPMODE_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_BUSSLEEPMODE_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:3  Section name :NM_CARWAKEUPINDICATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_CARWAKEUPINDICATION_CALLBACK_CODE
#undef NM_START_SEC_NM_CARWAKEUPINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_CARWAKEUPINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_CARWAKEUPINDICATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_CARWAKEUPINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_CARWAKEUPINDICATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_CARWAKEUPINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:4  Section name :NM_COORDREADYTOSLEEPCANCELLATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_COORDREADYTOSLEEPCANCELLATION_CALLBACK_CODE
#undef NM_START_SEC_NM_COORDREADYTOSLEEPCANCELLATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_COORDREADYTOSLEEPCANCELLATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_COORDREADYTOSLEEPCANCELLATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_COORDREADYTOSLEEPCANCELLATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_COORDREADYTOSLEEPCANCELLATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_COORDREADYTOSLEEPCANCELLATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:5  Section name :NM_COORDREADYTOSLEEPINDICATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_COORDREADYTOSLEEPINDICATION_CALLBACK_CODE
#undef NM_START_SEC_NM_COORDREADYTOSLEEPINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_COORDREADYTOSLEEPINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_COORDREADYTOSLEEPINDICATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_COORDREADYTOSLEEPINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_COORDREADYTOSLEEPINDICATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_COORDREADYTOSLEEPINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:6  Section name :NM_NETWORKMODE_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_NETWORKMODE_CALLBACK_CODE
#undef NM_START_SEC_NM_NETWORKMODE_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_NETWORKMODE_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_NETWORKMODE_CALLBACK_CODE
#undef NM_STOP_SEC_NM_NETWORKMODE_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_NETWORKMODE_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_NETWORKMODE_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:7  Section name :NM_NETWORKSTARTINDICATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_NETWORKSTARTINDICATION_CALLBACK_CODE
#undef NM_START_SEC_NM_NETWORKSTARTINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_NETWORKSTARTINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_NETWORKSTARTINDICATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_NETWORKSTARTINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_NETWORKSTARTINDICATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_NETWORKSTARTINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:8  Section name :NM_PDURXINDICATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_PDURXINDICATION_CALLBACK_CODE
#undef NM_START_SEC_NM_PDURXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_PDURXINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_PDURXINDICATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_PDURXINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_PDURXINDICATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_PDURXINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:9  Section name :NM_PREPAREBUSSLEEPMODE_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_PREPAREBUSSLEEPMODE_CALLBACK_CODE
#undef NM_START_SEC_NM_PREPAREBUSSLEEPMODE_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_PREPAREBUSSLEEPMODE_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_PREPAREBUSSLEEPMODE_CALLBACK_CODE
#undef NM_STOP_SEC_NM_PREPAREBUSSLEEPMODE_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_PREPAREBUSSLEEPMODE_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_PREPAREBUSSLEEPMODE_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:10  Section name :NM_REMOTESLEEPCANCELLATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_REMOTESLEEPCANCELLATION_CALLBACK_CODE
#undef NM_START_SEC_NM_REMOTESLEEPCANCELLATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_REMOTESLEEPCANCELLATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_REMOTESLEEPCANCELLATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_REMOTESLEEPCANCELLATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_REMOTESLEEPCANCELLATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_REMOTESLEEPCANCELLATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:11  Section name :NM_REMOTESLEEPINDICATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_REMOTESLEEPINDICATION_CALLBACK_CODE
#undef NM_START_SEC_NM_REMOTESLEEPINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_REMOTESLEEPINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_REMOTESLEEPINDICATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_REMOTESLEEPINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_REMOTESLEEPINDICATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_REMOTESLEEPINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:12  Section name :NM_REPEATMESSAGEINDICATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_REPEATMESSAGEINDICATION_CALLBACK_CODE
#undef NM_START_SEC_NM_REPEATMESSAGEINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_REPEATMESSAGEINDICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_REPEATMESSAGEINDICATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_REPEATMESSAGEINDICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_REPEATMESSAGEINDICATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_REPEATMESSAGEINDICATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:13  Section name :NM_STATECHANGENOTIFICATION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_STATECHANGENOTIFICATION_CALLBACK_CODE
#undef NM_START_SEC_NM_STATECHANGENOTIFICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_STATECHANGENOTIFICATION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_STATECHANGENOTIFICATION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_STATECHANGENOTIFICATION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_STATECHANGENOTIFICATION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_STATECHANGENOTIFICATION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:14  Section name :NM_SYNCHRONIZATIONPOINT_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_SYNCHRONIZATIONPOINT_CALLBACK_CODE
#undef NM_START_SEC_NM_SYNCHRONIZATIONPOINT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_SYNCHRONIZATIONPOINT_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_SYNCHRONIZATIONPOINT_CALLBACK_CODE
#undef NM_STOP_SEC_NM_SYNCHRONIZATIONPOINT_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_SYNCHRONIZATIONPOINT_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_SYNCHRONIZATIONPOINT_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:15  Section name :NM_SYNCHRONIZEMODE_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_SYNCHRONIZEMODE_CALLBACK_CODE
#undef NM_START_SEC_NM_SYNCHRONIZEMODE_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_SYNCHRONIZEMODE_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_SYNCHRONIZEMODE_CALLBACK_CODE
#undef NM_STOP_SEC_NM_SYNCHRONIZEMODE_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_SYNCHRONIZEMODE_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_SYNCHRONIZEMODE_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:16  Section name :NM_TXTIMEOUTEXCEPTION_CALLBACK_CODE*/
#elif defined NM_START_SEC_NM_TXTIMEOUTEXCEPTION_CALLBACK_CODE
#undef NM_START_SEC_NM_TXTIMEOUTEXCEPTION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_NM_TXTIMEOUTEXCEPTION_CALLBACK_CODE_NO_MATCH
#define START_SEC_CALLBACK_CODE

#elif defined NM_STOP_SEC_NM_TXTIMEOUTEXCEPTION_CALLBACK_CODE
#undef NM_STOP_SEC_NM_TXTIMEOUTEXCEPTION_CALLBACK_CODE
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_NM_TXTIMEOUTEXCEPTION_CALLBACK_CODE_NO_MATCH
#undef NM_START_SEC_NM_TXTIMEOUTEXCEPTION_CALLBACK_CODE_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_CALLBACK_CODE

/*Index:17  Section name :VAR_CLEARED_BOOLEAN*/
#elif defined NM_START_SEC_VAR_CLEARED_BOOLEAN
#undef NM_START_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#define START_SEC_VAR_CLEARED_BOOLEAN

#elif defined NM_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef NM_STOP_SEC_VAR_CLEARED_BOOLEAN
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#undef NM_START_SEC_VAR_CLEARED_BOOLEAN_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_BOOLEAN

/*Index:18  Section name :VAR_CLEARED_8*/
#elif defined NM_START_SEC_VAR_CLEARED_8
#undef NM_START_SEC_VAR_CLEARED_8
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_VAR_CLEARED_8_NO_MATCH
#define START_SEC_VAR_CLEARED_8

#elif defined NM_STOP_SEC_VAR_CLEARED_8
#undef NM_STOP_SEC_VAR_CLEARED_8
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_VAR_CLEARED_8_NO_MATCH
#undef NM_START_SEC_VAR_CLEARED_8_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_8

/*Index:19  Section name :VAR_CLEARED_UNSPECIFIED*/
#elif defined NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#undef NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifndef MEMMAP_ERROR_SEC_NO_MATCH
#define MEMMAP_ERROR_SEC_NO_MATCH
#else
#error "Nm_MemMap.h, section is used consecutively many times."
#endif
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#define START_SEC_VAR_CLEARED_UNSPECIFIED

#elif defined NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#undef NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#undef MEMMAP_ERROR
#ifdef NM_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#undef NM_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_MATCH
#else
#error "Nm_MemMap.h, no matching section was used."
#endif
#ifdef MEMMAP_ERROR_SEC_NO_MATCH
#undef MEMMAP_ERROR_SEC_NO_MATCH
#endif
#define STOP_SEC_VAR_CLEARED_UNSPECIFIED

#endif /* START_WITH_IF */

#if defined MEMMAP_ERROR
#error "Nm_MemMap.h, wrong pragma command"
#endif

#include "MemMap.h"

/*=======[E N D   O F   F I L E]==============================================*/
/* PRQA S 0883,0791-- */ /* MISAR Dir-4.10,Rule-5.4 */
