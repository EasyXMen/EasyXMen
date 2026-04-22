/*******************************************************************************
**                                                                            **
** Copyright (C) iSOFT   (2016-2023)                                          **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to iSOFT.         **
** Passing on and copying of this document, and communication                 **
** of its contents is not permitted without prior written authorization.      **
**                                                                            **
********************************************************************************
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
