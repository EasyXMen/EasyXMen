/* PRQA S 3108++ */
/**
 * Copyright (C) 2024 Isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception OR  LicenseRef-Commercial-License
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 *
 * Alternatively, this file may be used under the terms of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License, in which case the provisions of the Isoft Infrastructure Software Co., Ltd.
 * Commercial License shall apply instead of those of the GNU Lesser General Public License.
 *
 * You should have received a copy of the Isoft Infrastructure Software Co., Ltd.  Commercial License
 * along with this program. If not, please find it at <https://EasyXMen.com/xy/reference/permissions.html>
 */
/* PRQA S 3108-- */
/*
********************************************************************************
**                                                                            **
**  FILENAME    : CanTSyn.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : Xinrun Wang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Public interfaces declared by CanTSyn module                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

#ifndef CANTSYN_H_
#define CANTSYN_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "StbM.h"
#include "CanTSyn_Cfg.h"
#if (CANTSYN_CRC_SUPPORT == STD_ON)
#include "Crc.h"
#endif
#include "CanTSyn_Cbk.h"

#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
#include "Os.h"
#endif
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/* version info */
#define CANTSYN_VENDOR_ID          62U
#define CANTSYN_MODULE_ID          161U
#define CANTSYN_INSTANCE_ID        0U
#define CANTSYN_H_AR_MAJOR_VERSION 4U
#define CANTSYN_H_AR_MINOR_VERSION 5U
#define CANTSYN_H_AR_PATCH_VERSION 0U
#define CANTSYN_H_SW_MAJOR_VERSION 2U
#define CANTSYN_H_SW_MINOR_VERSION 0U
#define CANTSYN_H_SW_PATCH_VERSION 8U

/* DET ERROR CODE */
#if (CANTSYN_DEV_ERROR_DETECT == STD_ON)
#define CANTSYN_E_INVALID_PDUID ((uint8)0x01)
#define CANTSYN_E_UNINIT        ((uint8)0x02)
#define CANTSYN_E_NULL_POINTER  ((uint8)0x03)
#define CANTSYN_E_INIT_FAILED   ((uint8)0x04)
#define CANTSYN_E_PARAM         ((uint8)0x05)
#define CANTSYN_E_INV_CTRL_IDX  ((uint8)0x06)
#endif

#define CANTSYN_EXCLUSIVE_AREA_STATE ((uint8)0)

/* the default value for crc calculation */
#define CANTSYN_CRC_DEFAULT_VALUE ((uint8)0xFF)

/* the difference between one second and one nanosecond */
#define CANTSYN_1NS_SECOND ((uint32)1000000000U)

/* the difference between one hundred us and one second */
#define CANTSYN_100US_SECOND ((uint16)10000U)

/* the bit masks */
#define CANTSYN_GLOBAL_TIME_BASE_MASK     ((uint8)0x08)
#define CANTSYN_SYNC_TO_GATEWAY_MASK      ((uint8)0x04)
#define CANTSYN_TIMEOUT_MASK              ((uint8)0x01)
#define CANTSYN_OVS_BIT_MASK              ((uint8)0x03)
#define CANTSYN_SEQUENCE_COUNTER_BIT_MASK ((uint8)0x0F)

/* the type code for time sync messages */
#define CANTSYN_SYNC_NOT_CRC_TYPE   ((uint8)0x10)
#define CANTSYN_SYNC_CRC_TYPE       ((uint8)0x20)
#define CANTSYN_FUP_NOT_CRC_TYPE    ((uint8)0x18)
#define CANTSYN_FUP_CRC_TYPE        ((uint8)0x28)
#define CANTSYN_OFS_NOT_CRC_TYPE    ((uint8)0x34)
#define CANTSYN_OFS_CRC_TYPE        ((uint8)0x44)
#define CANTSYN_OFS_NOT_CRC_FD_TYPE ((uint8)0x54)
#define CANTSYN_OFS_CRC_FD_TYPE     ((uint8)0x64)
#define CANTSYN_OFNS_NOT_CRC_TYPE   ((uint8)0x3C)
#define CANTSYN_OFNS_CRC_TYPE       ((uint8)0x4C)

/* CanTSyn Module API ID */
#define CANTSYN_GETVERSIONINFO_ID      ((uint8)0x02)
#define CANTSYN_INIT_ID                ((uint8)0x01)
#define CANTSYN_SETTRANSMISSIONMODE_ID ((uint8)0x03)
#define CANTSYN_MAINFUNCTION_ID        ((uint8)0x06)
#define CANTSYN_RXINDICATION_ID        ((uint8)0x42)
#define CANTSYN_TXCONFIRMATION_ID      ((uint8)0x40)

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/* define CanTSyn module initiation status */
typedef enum
{
    CANTSYN_INITED = 0u,
    CANTSYN_UNINIT
} CanTSyn_InitStatusType;

/* define CanTSyn master operation status */
typedef enum
{
    SYNC_REQUIRED = 0u,
    FUP_REQUIRED,
    SYNC_SENT,
    SYNC_NOT_REQUIRED
} CanTSyn_MasterStatusType;

/* define CanTSyn Slave operation Status */
typedef enum
{
    SYNC_EXPECTED = 0u,
    FUP_EXPECTED
} CanTSyn_SlaveStatusType;

/* Handles the enabling and disabling of the transmission mode */
typedef enum
{
    CANTSYN_TX_OFF = 0u,
    CANTSYN_TX_ON
} CanTSyn_TransmissionModeType;

/* define DataIDList messages */
typedef struct CanTSyn_DataIDListElement_T
{
    uint8 CanTSynGlobalTimeDataIDListIndex;
    uint8 CanTSynGlobalTimeDataIDListValue;
} CanTSyn_DataIDListElementType;

/* store the time information for a time master */
typedef struct CanTSyn_MasterTimeInfo_T
{
    /* nanoseconds part of T0 */
    uint32 T0SYNCns;
    /* virtual local time of T0 */
    uint64 T0VLT;
    /* PRQA S 1532, 2053 ++ */ /* VL_CanTSyn_2053 */
    /* T4 = T0SYNCns + (T1VLT-T0VLT) */
    /* PRQA S 1532, 2053 -- */
    uint64 T4;
    /* The byte 2 of user data */
    uint8 userByte2;
    /* the status of time base */
    StbM_TimeBaseStatusType timeBaseStatus;
    /* the nanoseconds part of offset time */
    uint32 OfsTimeNSec;
    /* cyclic message resume counter */
    uint32 cyclicResumeCounter;
    /* debounce counter */
    uint32 debounceCounter;
    /* period Counter */
    uint32 periodCounter;
    /* TxConfirmation timeout counter */
    uint32 confirmationTimeoutCounter;
} CanTSyn_MasterTimeInfoType;

/* define configuration of the global time master */
typedef struct CanTSynGlobalTimeMaster_T
{
    /* defines the time where the 1st regular cycle time based message transmission takes place */
    uint32 CanTSynCyclicMsgResumeTime;
    /* TX debounce time */
    uint32 CanTSynGlobalTimeDebounceTime;
    /* whether or not CRC is supported */
    CrcSecuredType CanTSynGlobalTimeTxCrcSecured;
    /* configuration of the TX period */
    uint32 CanTSynGlobalTimeTxPeriod;
    /* Enables/Disables the cyclic polling of StbM_GetTimeBaseUpdateCounter() */
    boolean CanTSynImmediateTimeSync;
    /* the confirmation timeout after transmission of each Timesync message */
    uint32 CanTSynMasterConfirmationTimeout;
    /* reference to pdu */
    PduIdType CanTSynGlobalTimePduId;
} CanTSynGlobalTimeMasterType;

/* define configuration of the global time master */
typedef struct CanTSynGlobalTimeSlave_T
{
    /* Rx timeout for the follow-up message */
    uint32 CanTSynGlobalTimeFollowUpTimeout;
    /* the maximum allowed gap of the Sequence Counter between two SYNC resp. two OFS messages */
    uint8 CanTSynGlobalTimeSequenceCounterJumpWidth;
    /* whether or not validation of the CRC is supported */
    CrcValidatedType CanTSynRxCrcValidated;
    /* reference to pdu */
    PduIdType CanTSynGlobalTimePduId;
} CanTSynGlobalTimeSlaveType;

typedef struct CanTSyn_GlobalTimeDomain_T
{
    /* The global time domain ID */
    uint8 CanTSynGlobalTimeDomainId;
    /* Switches support CAN FD on or off */
    boolean CanTSynUseExtendedMsgFormat;
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType applicationID;
#endif
    /* Reference to time-base */
    StbM_SynchronizedTimeBaseType CanTSynSynchronizedTimeBaseId;
    /* The DataIDList for SYNC messages */
    const CanTSyn_DataIDListElementType* CanTSynGlobalTimeSyncDataIDList;
    /* The DataIDList for FUP messages */
    const CanTSyn_DataIDListElementType* CanTSynGlobalTimeFupDataIDList;
    /* The DataIDList for OFS messages */
    const CanTSyn_DataIDListElementType* CanTSynGlobalTimeOfsDataIDList;
    /* The DataIDList for OFNS messages */
    const CanTSyn_DataIDListElementType* CanTSynGlobalTimeOfnsDataIDList;
    /* Configuration of the global time master */
    const CanTSynGlobalTimeMasterType* CanTSynGlobalTimeMaster;
    /* Configuration of the global time slave */
    const CanTSynGlobalTimeSlaveType* CanTSynGlobalTimeSlave;
} CanTSyn_GlobalTimeDomainType;

/* store the time information for a time slave */
typedef struct CanTSyn_SlaveTimeInfo_T
{
    /* seconds portion of synchronized time */
    uint32 T0SYNC;
    /* virtual local time of T2 */
    uint64 T2VLT;
    /* The byte 0 of user data */
    uint8 userByte0;
    /* The byte 1 of user data */
    uint8 userByte1;
    /* seconds portion of offset time */
    uint32 OfsTimeSec;
    /* sequence counter */
    uint8 sequenceCounter;
    /* whether the SYNC/OFS message is CRC validated */
    boolean isCRC;
    /* reception timeout counter */
    uint32 receptionTimeoutCounter;
} CanTSyn_SlaveTimeInfoType;

/*
 * This is the base type for the configuration of the Time Synchronization over CAN.
 * A pointer to an instance of this structure will be used
 * in the initialization of the Time Synchronization over CAN.
 */
typedef struct
{
    const CanTSyn_GlobalTimeDomainType* CanTSynGlobalTimeDomain;
} CanTSyn_ConfigType;

/*******************************************************************************
**                      Global Data Declaration                               **
*******************************************************************************/

/* CanTSyn config data */

extern CONST(CanTSyn_ConfigType, CANTSYN_CONST_PBCFG) CanTSyn_config;

/*******************************************************************************
**                      Global Functions                                      **
*******************************************************************************/

#if (STD_ON == CANTSYN_VERSIONINFO_API)
/**
 * This service returns the version information of this module.
 * Service ID: 0x02
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): NA
 * Parameters(INOUT): NA
 * Parameters(OUT): versioninfo: Pointer to where to store the version information of this module
 * Return value: NA
 *
 */

extern void CanTSyn_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTSYN_APPL_DATA) versioninfo);

#endif

/**
 * This function initializes the Time Synchronization over CAN.
 * Service ID: 0x01
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): configPtr: Pointer to selected configuration structure
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */

extern void CanTSyn_Init(const CanTSyn_ConfigType* configPtr);

/**
 * This API is used to turn on and off the TX capabilities of the CanTSyn.
 * Service ID: 0x03
 * Sync/Async: synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): CtrlIdx: Index of the CAN channel
 *                 Mode: CANTSYN_TX_OFF CANTSYN_TX_ON
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 *
 */
extern void CanTSyn_SetTransmissionMode(uint8 CtrlIdx, CanTSyn_TransmissionModeType Mode);
#endif /* CANTSYN_H_ */
