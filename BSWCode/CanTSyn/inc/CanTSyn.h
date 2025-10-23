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
/*
 ************************************************************************************************************************
 **
 **  @file               : CanTSyn.h
 **  @author             : xiongfei.shi
 **  @date               : 2024/12/13
 **  @vendor             : isoft
 **  @description        : The header file for the public interface implementation of the CanTSyn module
 **
 ***********************************************************************************************************************/

/** ===================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date       Author         Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V2.2.0    2024-12-13 xiongfei.shi   1.Refactor and update to R23-11
 *                                      2.Removed offset Time Domains
 *                                      3.support multi-partition distribution
 *
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:CanTSyn<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_CanTSyn_0770
      Reason: design needs
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_CanTSyn_STM19
      Reason: In principle, it is not allowed to have multiple exit points for a function, but it helps to improve
      code performance in special circumstances.
      Risk: Increase the difficulty of function debugging.
      Prevention: Full testing to ensure correct functionality.

    \li VL_MTR_CanTSyn_STMIF
      Reason: Functions handle specific tasks across the entire component behavior.
      Risk: Code is difficult to maintain.
      Prevention: Design and code review, and have a clear structure and annotated code.

    \li VL_MTR_CanTSyn_STCAL
      Reason: The software architecture is defined according to the AUTOSAR standard.
      Risk: Due to the fan distribution to many functions, understandability and testability can become overly complex.
      Prevention: Design and code review, and have a clear structure and annotated code.
 */

#ifndef CANTSYN_H_
#define CANTSYN_H_

/* =================================================== inclusions =================================================== */
#include "CanTSyn_Cfg.h"
#include "ComStack_Types.h"
#if (CANTSYN_CRC_SUPPORT == STD_ON)
#include "Crc.h"
#endif
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
#include "Os.h"
#endif
#include "StbM.h"
#include "CanTSyn_Cbk.h"
#include "CanTSyn_Internal.h"
#include "CanTSyn_PBcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/* Published Information */
#define CANTSYN_VENDOR_ID                   (62u)
#define CANTSYN_MODULE_ID                   (161u)
#define CANTSYN_INSTANCE_ID                 (0u)
#define CANTSYN_AR_RELEASE_MAJOR_VERSION    (4u)
#define CANTSYN_AR_RELEASE_MINOR_VERSION    (9u)
#define CANTSYN_AR_RELEASE_REVISION_VERSION (0u)
#define CANTSYN_SW_MAJOR_VERSION            (2u)
#define CANTSYN_SW_MINOR_VERSION            (2u)
#define CANTSYN_SW_PATCH_VERSION            (0u)

/* ===================================================== macros ===================================================== */
/* DET ERROR CODE */
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#define CANTSYN_E_INVALID_PDUID       ((uint8)0x01u)
#define CANTSYN_E_UNINIT              ((uint8)0x02u)
#define CANTSYN_E_NULL_POINTER        ((uint8)0x03u)
#define CANTSYN_E_INIT_FAILED         ((uint8)0x04u)
#define CANTSYN_E_PARAM               ((uint8)0x05u)
#define CANTSYN_E_INV_CTRL_IDX        ((uint8)0x06u)
#define CANTSYN_E_INVALID_PARTITION   ((uint8)0xF0u)
#define CANTSYN_E_ALREADY_INITIALIZED ((uint8)0xF1u)
#endif

/* CanTSyn Module API ID */
#define CANTSYN_SID_INIT                ((uint8)0x01u)
#define CANTSYN_SID_GETVERSIONINFO      ((uint8)0x02u)
#define CANTSYN_SID_SETTRANSMISSIONMODE ((uint8)0x03u)
#define CANTSYN_SID_MAINFUNCTION        ((uint8)0x06u)
#define CANTSYN_SID_TXCONFIRMATION      ((uint8)0x40u)
#define CANTSYN_SID_RXINDICATION        ((uint8)0x42u)

/* the default value for crc calculation */
#define CANTSYN_CRC_DEFAULT_VALUE ((uint8)0xFFu)

/* the difference between one second and one nanosecond */
#define CANTSYN_1NS_SECOND (1000000000ULL)

/* the difference between one hundred us and one second */
#define CANTSYN_100US_SECOND ((uint16)10000U)

/* the bit masks */
#define CANTSYN_GLOBAL_TIME_BASE_MASK     ((uint8)0x08u)
#define CANTSYN_SYNC_TO_GATEWAY_MASK      ((uint8)0x04u)
#define CANTSYN_TIMEOUT_MASK              ((uint8)0x01u)
#define CANTSYN_OVS_BIT_MASK              ((uint8)0x03u)
#define CANTSYN_SGW_BIT_MASK              ((uint8)0x04u)
#define CANTSYN_SEQUENCE_COUNTER_BIT_MASK ((uint8)0x0Fu)

/* the type code for time sync messages */
#define CANTSYN_SYNC_NOT_CRC_TYPE ((uint8)0x10u)
#define CANTSYN_SYNC_CRC_TYPE     ((uint8)0x20u)
#define CANTSYN_FUP_NOT_CRC_TYPE  ((uint8)0x18u)
#define CANTSYN_FUP_CRC_TYPE      ((uint8)0x28u)

#define CANTSYN_SEQUENCE_COUNTER_MAX_VALUE ((uint8)0x0Fu)

#define CANTSYN_DATALIST_SIZE ((uint8)16u)

/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */
#if (STD_ON == CANTSYN_VERSIONINFO_API)
/**
 *  @brief         This service returns the version information of this module.
 *  @param[out]    versioninfo : Pointer to where to store the version information of this module
 *  @reentrant     TRUE
 *  @synchronous   TRUE
 *  @trace         CPD-75625
 */
void CanTSyn_GetVersionInfo(Std_VersionInfoType* versioninfoPtr);
#endif

/**
 *  @brief         This API is used to turn on and off the TX capabilities of the CanTSyn.
 *  @param[in]     CtrlIdx : Index of the CAN channel
 *  @param[in]     Mode : CANTSYN_TX_OFF CANTSYN_TX_ON
 *  @reentrant     FALSE
 *  @synchronous   TRUE
 *  @trace         CPD-75626
 */
void CanTSyn_SetTransmissionMode(uint8 CtrlIdx, CanTSyn_TransmissionModeType Mode);

/**
 *  @brief         This function initializes the Time Synchronization over CAN.
 *  @param[in]     configPtr : Pointer to selected configuration structure
 *  @reentrant     FALSE
 *  @synchronous   TRUE
 *  @trace         CPD-71890
 */
void CanTSyn_Init(const CanTSyn_ConfigType* configPtr);

#ifdef __cplusplus
}
#endif

#endif /* CANTSYN_H_ */
