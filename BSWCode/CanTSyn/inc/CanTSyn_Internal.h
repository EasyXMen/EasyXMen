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
 **  @file               : CanTSyn_Internal.h
 **  @author             : xiongfei.shi
 **  @date               : 2024/12/13
 **  @vendor             : isoft
 **  @description        : internal data types of CanTSyn
 **
 ***********************************************************************************************************************/

#ifndef CANTSYN_INTERNAL_H_
#define CANTSYN_INTERNAL_H_

/* =================================================== inclusions =================================================== */
#include "ComStack_Types.h"
#include "StbM.h"
#include "CanTSyn_Cfg.h"
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
#include "Os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

#ifndef CANTSYN_LOCAL
#define CANTSYN_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

#ifndef CANTSYN_LOCAL_INLINE
#define CANTSYN_LOCAL_INLINE static inline
#endif

/* ================================================ type definitions ================================================ */
/**
 * @brief define CanTSyn module initiation status
 */
typedef enum CanTSyn_InitStatusTypeTag
{
    CANTSYN_UNINIT,
    CANTSYN_INITED
} CanTSyn_InitStatusType;

/**
 * @brief define CanTSyn master operation status
 */
typedef enum CanTSyn_MasterStatusTypeTag
{
    CANTSYN_SYNC_REQUIRED,
    CANTSYN_FUP_REQUIRED,
    CANTSYN_SYNC_SENT,
    CANTSYN_SYNC_NOT_REQUIRED
} CanTSyn_MasterStatusType;

/**
 * @brief define CanTSyn Slave operation Status
 */
typedef enum CanTSyn_SlaveStatusTypeTag
{
    CANTSYN_SYNC_EXPECTED,
    CANTSYN_FUP_EXPECTED
} CanTSyn_SlaveStatusType;

/**
 * @brief Handles the enabling and disabling of the transmission mode
 */
typedef enum CanTSyn_TransmissionModeTypeTag
{
    CANTSYN_TX_OFF,
    CANTSYN_TX_ON
} CanTSyn_TransmissionModeType;

/**
 * @brief store the time information for a time master
 */
typedef struct CanTSyn_MasterTimeInfoTag
{
    /* nanoseconds part of T0 */
    uint32 T0SYNCns;
    /* virtual local time of T0 */
    uint64 T0VLT;
    /* T4 is t0syncns plus the result of t1vlt minus t0vlt */
    uint64 T4;
    /* The byte 2 of user data */
    uint8 UserByte2;
    /* the status of time base */
    StbM_TimeBaseStatusType TimeBaseStatus;
    /* cyclic message resume counter */
    uint32 CyclicResumeCounter;
    /* debounce counter */
    uint32 DebounceCounter;
    /* period Counter */
    uint32 PeriodCounter;
} CanTSyn_MasterTimeInfoType;

/**
 * @brief store the time information for a time slave
 */
typedef struct CanTSyn_SlaveTimeInfoTag
{
    /* seconds portion of synchronized time */
    uint32 T0SYNC;
    /* virtual local time of T2 */
    uint64 T2VLT;
    /* The byte 0 of user data */
    uint8 UserByte0;
    /* The byte 1 of user data */
    uint8 UserByte1;
    /* sequence counter */
    uint8 SequenceCounter;
    /* whether the SYNC/OFS message is CRC validated */
    boolean IsCRC;
    /* reception timeout counter */
    uint32 ReceptionTimeoutCounter;
    /* specifying if firstly receiving a SYNC message */
    boolean FirstlyReceivedSync;
} CanTSyn_SlaveTimeInfoType;

/**
 * @brief define configuration of the global time master
 */
typedef struct CanTSyn_GlobalTimeMasterTag
{
    /* defines the time where the 1st regular cycle time based message transmission takes place */
    uint32 CyclicMsgResumeTime;
    /* TX debounce time */
    uint32 GlobalTimeDebounceTime;
    /* whether or not CRC is supported */
    CrcSecuredType GlobalTimeTxCrcSecured;
    /* configuration of the TX period */
    uint32 GlobalTimeTxPeriod;
    /* Enables/Disables the cyclic polling of StbM_GetTimeBaseUpdateCounter() */
    boolean ImmediateTimeSync;
    /* reference to pdu */
    PduIdType GlobalTimePduId;
} CanTSyn_GlobalTimeMasterType;

/**
 * @brief define configuration of the global time slave
 */
typedef struct CanTSyn_GlobalTimeSlaveTag
{
    /* Rx timeout for the follow-up message */
    uint32 GlobalTimeFollowUpTimeout;
    /* the maximum allowed gap of the Sequence Counter between two SYNC resp. two OFS messages */
    uint8 GlobalTimeSequenceCounterJumpWidth;
    /* whether or not validation of the CRC is supported */
    CrcValidatedType RxCrcValidated;
    /* reference to pdu */
    PduIdType GlobalTimePduId;
} CanTSyn_GlobalTimeSlaveType;

/**
 * @brief define DataIDList messages
 */
typedef struct CanTSyn_DataIDListElementTag
{
    /* index of data Id list */
    uint8 GlobalTimeDataIDListIndex;
    /* value of data Id list */
    uint8 GlobalTimeDataIDListValue;
} CanTSyn_DataIDListElementType;

/**
 * @brief define CanTSyn GlobalTimeDomain
 */
typedef struct CanTSyn_GlobalTimeDomainTag
{
    /* The global time domain ID */
    uint8 GlobalTimeDomainId;
    /* Switches support CAN FD on or off */
    boolean UseExtendedMsgFormat;
    /* Reference to time-base */
    StbM_SynchronizedTimeBaseType SynchronizedTimeBaseId;
    /* The DataIDList for SYNC messages */
    const CanTSyn_DataIDListElementType* GlobalTimeSyncDataIDList;
    /* The DataIDList for FUP messages */
    const CanTSyn_DataIDListElementType* GlobalTimeFupDataIDList;
    /* Configuration of the global time master */
    const CanTSyn_GlobalTimeMasterType* GlobalTimeMaster;
    /* Configuration of the global time slave */
    const CanTSyn_GlobalTimeSlaveType* GlobalTimeSlave;
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    ApplicationType ApplicationId;
#endif
} CanTSyn_GlobalTimeDomainType;

/**
 * @brief This is the base type for the configuration of the Time Synchronization over CAN.
 *        A pointer to an instance of this structure will be used in the initialization
 *        of the Time Synchronization over CAN.
 */
typedef struct CanTSyn_ConfigTag
{
    const CanTSyn_GlobalTimeDomainType* GlobalTimeDomain;
} CanTSyn_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
extern CanTSyn_MasterTimeInfoType* const CanTSyn_MasterTimeInfoPtr[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
extern CanTSyn_SlaveTimeInfoType* const  CanTSyn_SlaveTimeInfoPtr[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
extern const ApplicationType CanTSyn_OsApplicationList[CANTSYN_PARTITION_NUMBER];
#endif

/* ========================================= external function declarations ========================================= */
/**
 * @brief        This function needs to be called periodically to perform the AUTOSAR CanTSyn activities
 * @reentrant    TRUE (Reentrant for different partitions. Non reentrant for the same partition.)
 * @synchronous  FALSE
 * @trace        CPD-75627
 */
void CanTSyn_MainFunction(void);

#ifdef __cplusplus
}
#endif

#endif
