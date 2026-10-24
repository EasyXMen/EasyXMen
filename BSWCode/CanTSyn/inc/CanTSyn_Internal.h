/**
 * Copyright (C) 2008-2026 isoft Infrastructure Software Co., Ltd.
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

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#include "Csm.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

/* ===================================================== macros ===================================================== */

#ifndef CANTSYN_LOCAL
/** This macro definition represents the CanTSyn module static */
#define CANTSYN_LOCAL static /* PRQA S 3414 */ /* VL_QAC_KeyWord */
#endif

#ifndef CANTSYN_LOCAL_INLINE
/** This macro definition represents the CanTSyn module inline function */
#define CANTSYN_LOCAL_INLINE static inline
#endif

/** define the size of the data list */
#define CANTSYN_DATALIST_SIZE          ((uint8)16u)
/** define the maximal number of the sequence ID */
#define CANTSYN_SC_MAX                 ((uint8)15u)
/** define the maximal number of the Domain ID */
#define CANTSYN_SYNC_FUP_DOMAIN_ID_MAX ((uint8)15u)
/** define the length of the Message transmit on standard CAN */
#define CANTSYN_STANDARD_MSG_LEN       (8u)
/** define the length of the Message transmit on extended CAN */
#define CANTSYN_EXTENDED_MSG_LEN       (16u)
/** define the length of userdata */
#define CANTSYN_USERDATA_LEN_MAX       (3u)

/**
 * @name maigic number
 * @{
 * @brief define the maigic number
 */
#define CANTSYN_SHIFT_BIT8  (8u)           /**< shift 8 bits */
#define CANTSYN_SHIFT_BIT16 (16u)          /**< shift 16 bits */
#define CANTSYN_SHIFT_BIT24 (24u)          /**< shift 24 bits */
#define CANTSYN_SHIFT_BIT32 (32u)          /**< shift 32 bits */
#define CANTSYN_CONST_3U    ((uint8)0x03u) /**< constant 3 */
#define CANTSYN_CONST_7U    ((uint8)0x07u) /**< constant 7 */
/** @} */

#define CANTSYN_MSG_TYPE_BYTE       ((uint8)0x00u)
#define CANTSYN_MSG_SC_AND_D_BYTE   ((uint8)0x02u)
#define CANTSYN_MSG_SC_BIT_POSITION ((uint8)0x00u)
#define CANTSYN_MSG_SC_BIT_MASK     ((uint8)0x0Fu)
#define CANTSYN_MSG_D_BIT_POSITION  ((uint8)0x04u)
#define CANTSYN_MSG_D_BIT_MASK      ((uint8)0xF0u)

/** define the bit offset of the Time Domain Id in Time Domain Id and Sequence Counter */
#define CANTSYN_MESSAGE_TIME_DOMAIN_ID_BIT_OFFSET ((uint8)0x04u)

#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
/** define the length of the Message transmit on extended CAN with ICV */
#define CANTSYN_EXTENDED_MSG_WITH_ICV_LEN                          (64u)
/** define the message type of the Time Authenticated */
#define CANTSYN_SUB_TLV_TYPE_TIME_AUTHENTICATED                    ((uint8)0x70)
/** define the length of the Time Authenticated without FV */
#define CANTSYN_ICV_TIME_AUTHENTICATED_WITHOUT_FV_ICV_LENGTH       ((uint8)0x0au)
/** define the length of the extended SYNC payload */
#define CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE                         ((uint8)0x10u)
/** define the length of the SYNC payload except Time Authenticated */
#define CANTSYN_FUP_EXCEPT_AUTHENTICATED_PAYLOAD_SIZE              ((uint8)0x08u)
/** define the byte offset of the FV length */
#define CANTSYN_ICV_FRESHNESS_VALUE_LENGTH_OFFSET                  ((uint8)0x08u)
/** define the byte offset of the ICV length */
#define CANTSYN_ICV_LENGTH_OFFSET                                  ((uint8)0x09u)
/** define the byte offset of the FV */
#define CANTSYN_ICV_FRESHNESS_VALUE_OFFSET                         ((uint8)0x0au)
/** define the maximum length of the ICV */
#define CanTSyn_MAX_ICV_SIZE                                       ((uint64)54u)
/** define the initial status of the ICV generation */
#define CANTSYN_ICV_TX_PROGRESS_STATUS_BUILDING                    ((uint8)0x01u)
/** define the status of the ICV generation when waiting for the indication */
#define CANTSYN_ICV_TX_PROGRESS_STATUS_WAITING_INDICATION          ((uint8)0x02u)
/** define the status of the ICV generation when ICV generation is comfirmed */
#define CANTSYN_ICV_TX_PROGRESS_STATUS_COMFIRMATION                ((uint8)0x03u)
/** define the status of the ICV generation when attempts a retry */
#define CANTSYN_ICV_TX_PROGRESS_STATUS_RETRY                       ((uint8)0x04u)
/** define the initial status of the ICV verification */
#define CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_INITIAL            ((uint8)0x04u)
/** define the status of the ICV verification receiving a valid ICV message */
#define CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_START              ((uint8)0x05u)
/** define the status of the ICV verification when waiting for the Indication */
#define CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_WAITING_INDICATION ((uint8)0x06u)
/** define the status of the ICV verification when ICV verification is comfirmed */
#define CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_COMFIRMATION       ((uint8)0x07u)
/** define the status of the ICV verification when ICV verification is failed */
#define CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_FAILED             ((uint8)0x08u)
/** define the status of the ICV verification when ICV verification attempts a retry */
#define CANTSYN_ICV_RX_PROGRESS_STATUS_VALIDATE_RETRY              ((uint8)0x09u)
/** define the maximum size of the ICV build buffer */
#define CANTSYN_ICV_BUILD_BUFFER_MAX                               ((uint8)64u)
/** define the size of the circular buffer */
#define CANTSYN_ICV_CIRCULAR_BUFFER_SIZE                           ((uint16)192u)

#if !defined(CANTSYN_E_VALIDATE_ICV)
/** define the return value of the ICV validation */
#define CANTSYN_E_VALIDATE_ICV 0x02u
#endif
/** define the maximal size of the FV */
#define CANTSYN_MAX_FRESHNESS_VALUE_SIZE ((uint8)0x08u)
#endif

#if CANTSYN_ENABLE_SECURITY_EVENT_REPORTING == STD_ON
/** specifying No FV available from the FVM */
#define CANTSYN_E_FRESHNESSFAILURE ((uint8)0x01u)
#endif
/* ================================================ type definitions ================================================ */
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
/**
 * @brief Definition of whether or not ICV verification shall be supported.
 */
typedef enum
{
    /** ICV verification shall be ignored. @range 0u*/
    CANTSYN_ICV_IGNORED,
    /** ICV verification shall not verified. @range 1u*/
    CANTSYN_ICV_NOT_VERIFIED,
    /** ICV verification shall be optional. @range 2u*/
    CANTSYN_ICV_OPTIONAL,
    /** ICV verification shall be verified. @range 3u*/
    CANTSYN_ICV_VERIFIED
} CanTSyn_RxIcvVerificationType;
/**
 * @brief Definition of whether or not ICV generation shall be supported.
 */
typedef enum
{
    /** ICV generation shall not be supported. @range 0u*/
    CANTSYN_ICV_NOT_SUPPORTED,
    /** ICV generation shall be supported. @range 1u*/
    CANTSYN_ICV_SUPPORTED
} CanTSyn_GlobalTimeTxIcvSecuredType;
/**
 * @brief This type shall define Symmetric or asymmetric cryptography selection for the ICV generation.
 */
typedef enum
{
    /** Indicate the Csm primitive is MAC. @range 0u*/
    CANTSYN_ICV_MAC,
    /** Indicate the Csm primitive is SIGNATURE. @range 1u*/
    CANTSYN_ICV_SIGNATURE
} CanTSyn_CriptographySelection;

/**
 * @brief    Define the encryption and decryption process, synchronous or asynchronous.
 */
typedef enum
{
    /** Indicate the encryption and decryption process is asynchronous. @range 0u*/
    CANTSYN_CRYPTO_PROCESSING_ASYNC,
    /** Indicate the encryption and decryption process is synchronous. @range 1u*/
    CANTSYN_CRYPTO_PROCESSING_SYNC
} CanTSyn_CryptoProcessingType;

/**
 * @brief    Define information about the Csm job of verification.
 */
typedef struct
{
    /** specifying the reference to the CSM job to fetch the CSM job ID. @range 0 ~ 0xFFFFFFFF*/
    uint32 IcvVerificationJobRef;
    /** Indicate the encryption and decryption process,synchronous or asynchronous. @range 0u, 1u*/
    CanTSyn_CryptoProcessingType IcvProcessingType;
    /**  specifying Symmetric or asymmetric cryptography selection for the ICV verificaiton. @range 0u, 1u*/
    CanTSyn_CriptographySelection IcvVerificationBase;
} CanTSyn_RxCsmJobType;

/**
 * @brief This type shall define the configuration of ICV verification.
 */
typedef struct
{
    /** specifying Timeout of ICV verification @range 0 ~ 0xFFFFFFFFFFFFFFFF*/
    uint64 IcvVerificationTimeout;
    /** specifying Length of ICV to be transmitted within Follow_Up Message on the bus (in bytes). @range 0 ~ 0xFFFF*/
    uint16 IcvRxLength;
    /** specifying the number of ICV verification attempts that are to be carried out when the verification
     * of the ICV failed for a given secured Follow_Up message. @range 0 ~ 0xFFFF*/
    uint16 IcvVerificationAttempts;
    /** specifying the number of authentication build attempts that are to be carried out when the verification
     * of the ICV failed for a given Follow_Up message. @range 0 ~ 0xFFFF*/
    uint16 RxAuthenticationBuildAttempts;
    /** specifying the reference to the FV taken to verify the ICV. @range 0 ~ 0xFFFFFFFF*/
    uint32 IcvVerificationFvIdRef;
    /** specifying the configuration of ICV verification. @range 0 ~ 0xFFFFFFFF*/
    CanTSyn_RxCsmJobType RxICVVerificationConfig;
    /** specifying the length in bits of the Freshness Value to be included in the payload of the Secured Time
     * Synchronization Messages. @range 0 ~ 0xFFFFFFFF*/
    uint32 IcvRxFreshnessValueTruncLength;
    /** specifying length of the provided freshness in bits @range 0 ~ 0xFFFFFFFF*/
    uint32 IcvRxFreshnessValueLength;
    /** specifying the circular buffer for ICV verification @range NA*/
    uint8* const RxIcvCircularBuffer;
} CanTSyn_GlobalTimeRxIcvVerificationType;
/**
 * @brief    Define information about the Csm job of generation.
 */
typedef struct
{
    /** specifying the reference to the CSM job to fetch the CSM job ID. @range 0 ~ 0xFFFFFFFF*/
    uint32 IcvGenerationJobRef;
    /** Indicate the encryption and decryption process,synchronous or asynchronous. @range 0u, 1u*/
    CanTSyn_CryptoProcessingType IcvProcessingType;
    /**  specifying Symmetric or asymmetric cryptography selection for the ICV verificaiton. @range 0u, 1u*/
    CanTSyn_CriptographySelection IcvGenerationBase;
} CanTSyn_TxCsmJobType;
/**
 * @brief This type shall define the configuration of ICV generation.
 */
typedef struct
{
    /** specifying Timeout of ICV generation @range 0 ~ 0xFFFFFFFFFFFFFFFF*/
    uint64 IcvGenerationTimeout;
    /** specifying Length of ICV to be transmitted within Follow_Up Message on the bus (in bytes). @range 0 ~ 0xFFFF*/
    uint16 IcvTxLength;
    /** specifying the number of authentication build attempts @range 0 ~ 0xFFFF*/
    uint16 TxAuthenticationBuildAttempts;
    /** specifying the reference to the FV taken to generate the ICV generation. @range 0 ~ 0xFFFF*/
    uint16 IcvGenerationFvIdRef;
    /** specifying the configuration of ICV generation. @range 0 ~ 0xFFFFFFFF*/
    CanTSyn_TxCsmJobType TxICVGenerationConfig;
    /** specifying the length in bits of the Freshness Value to be included in the payload of the Secured Time
     * Synchronization Messages. @range 0 ~ 0xFFFFFFFF*/
    uint32 IcvTxFreshnessValueTruncLength;
    /** specifying length of the provided freshness in bits @range 0 ~ 0xFFFFFFFF*/
    uint32 IcvTxFreshnessValueLength;
} CanTSyn_GlobalTimeTxIcvGenerationType;
/**
 * @brief This type shall describe the Time Authenticated Field.
 */
typedef struct
{
    /** specifying length of Freshness Value to be sended @range 0 ~ 0xFF*/
    uint8 FvLength;
    /** specifying if ICV is generated successfully @range TRUE, FALSE*/
    boolean IsIcvGeneratedSuccessfully;
    /** specifying Freshness Value @range 0 ~ 0xFF*/
    uint8 FreshValue[CANTSYN_MAX_FRESHNESS_VALUE_SIZE];
    /** specifying generated ICV @range 0 ~ 0xFF*/
    uint8 ICVBuffer[CANTSYN_MAX_ICV_SIZE];
} CanTSyn_AuthenticatedFieldInofoType;
/**
 * @brief This type shall define the parameters of circular buffer.
 */
typedef struct
{
    /** specifying pointer of the buffer @range NA*/
    uint8* Buffer;
    /** specifying the capacity of the buffer @range 0 ~ 0xFFFF*/
    uint16 Capacity;
    /** specifying the index of the write position @range 0 ~ 0xFFFF*/
    uint16 Head;
    /** specifying the index of the read position @range 0 ~ 0xFFFF*/
    uint16 Tail;
    /** specifying the current number of data stored in the buffer @range 0 ~ 0xFFFF*/
    uint16 Count;
} CanTSyn_CircularBufferType;
/**
 * @brief This type shall define the runtime variables related to ICV generation.
 */
typedef struct
{
    /** specifying the local time when ICV generation or verification timer starts @range NA*/
    uint64 IcvIndicationStartTime;
    /** specifying Icv generation or verification build counter @range 0 ~ 0xFFFF*/
    uint16 AuthenticationBuildAttemptsCounter;
    /** specifying Icv verification counter @range 0 ~ 0xFFFF*/
    uint16 IcvVerificationAttemptsCounter;
    /** specifying the sequence counter of Sub-Tlv Time Authenticated @range 0 ~ 0xFF*/
    uint8 TimeAuthencatedSequenceCounter;
    /** specifying the Icv generation or verification status @range 0 ~ 0xFF*/
    uint8 IcvProgressStatus;
    /** specifying the data offset of Sub-Tlv Time Authenticated @range 0 ~ 0xFF*/
    uint8 TimeAuthenticatedFieldOffset;
    /** specifying the result of Icv verification @range 0 ~ 0xFF*/
    Crypto_VerifyResultType RxVerifyResult;
    /** specifying the sync payload used for ICV generation @range NA*/
    uint8 SyncPayload[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE];
    /** specifying the data field of Sub-Tlv Time Authenticated @range NA*/
    CanTSyn_AuthenticatedFieldInofoType AuthenticatedFieldInfo;
} CanTSyn_ICVGenarationType;
/**
 * @brief This type shall define the runtime variables related to ICV verification.
 */
typedef struct
{
    /** specifying the local time when ICV generation or verification timer starts @range NA*/
    uint64 IcvIndicationStartTime;
    /** specifying Icv generation or verification build counter @range 0 ~ 0xFFFF*/
    uint16 AuthenticationBuildAttemptsCounter;
    /** specifying Icv verification counter @range 0 ~ 0xFFFF*/
    uint16 IcvVerificationAttemptsCounter;
    /** specifying the sequence counter of Sub-Tlv Time Authenticated @range 0 ~ 0xFF*/
    uint8 TimeAuthencatedSequenceCounter;
    /** specifying the Icv generation or verification status @range 0 ~ 0xFF*/
    uint8 IcvProgressStatus;
    /** specifying the data offset of Sub-Tlv Time Authenticated @range 0 ~ 0xFF*/
    uint8 TimeAuthenticatedFieldOffset;
    /** specifying the result of Icv verification @range 0 ~ 0xFF*/
    Crypto_VerifyResultType RxVerifyResult;
    /** specifying the data field of Sub-Tlv Time Authenticated @range NA*/
    CanTSyn_AuthenticatedFieldInofoType AuthenticatedFieldInfo;
    /** specifying the ICV circular buffer token @range NA*/
    CanTSyn_CircularBufferType RxICVBufferToken;
    /** Sync message used for ICV validation @range NA*/
    uint8 SyncPayload[CANTSYN_EXTENDED_SYNC_PAYLOAD_SIZE];
    /** Raw data copied from Rxindication when ICV enable @range NA*/
    uint8 FollowUpDataField[CANTSYN_ICV_BUILD_BUFFER_MAX];
} CanTSyn_ICVVerificationType;
#endif
/**
 * @brief define CanTSyn module initiation status
 */
typedef enum
{
    /** CanTSyn module is uninitialized. @range 0u*/
    CANTSYN_UNINIT,
    /** CanTSyn module is initialized. @range 1u*/
    CANTSYN_INITED
} CanTSyn_InitStatusType;

/**
 * @brief define CanTSyn master operation status
 */
typedef enum
{
    /** CanTSyn Master requires a transmmision of SYNC message. @range 0u*/
    CANTSYN_SYNC_REQUIRED,
    /** CanTSyn Master requires a transmmision of FUP message. @range 1u*/
    CANTSYN_FUP_REQUIRED,
    /** CanTSyn Master has sent a SYNC message. @range 2u*/
    CANTSYN_SYNC_SENT,
    /** CanTSyn Master does not require a transmmision of SYNC message. @range 3u*/
    CANTSYN_SYNC_NOT_REQUIRED
} CanTSyn_MasterStatusType;

/**
 * @brief Handles the enabling and disabling of the transmission mode
 */
typedef enum
{
    /** CanTSyn Master transmission mode is disabled. @range 0u*/
    CANTSYN_TX_OFF,
    /** CanTSyn Master transmission mode is enabled. @range 1u*/
    CANTSYN_TX_ON
} CanTSyn_TransmissionModeType;

/**
 * @brief store the time information for a time master
 */
typedef struct
{
    /** nanoseconds part of T0 @range 0 ~ 0xFFFFFFFF*/
    uint32 T0SYNCns;
    /** virtual local time of T0 @range 0 ~ 0xFFFFFFFFFFFFFFFF*/
    uint64 T0VLT;
    /** T4 is t0syncns plus the result of t1vlt minus t0vlt @range 0 ~ 0xFFFFFFFFFFFFFFFF*/
    uint64 T4;
    /** The byte 2 of user data @range 0 ~ 0xFF*/
    uint8 UserByte2;
    /** the status of time base @range 0 ~ 0xFFFFFFFF*/
    StbM_TimeBaseStatusType TimeBaseStatus;
    /** cyclic message resume counter @range 0 ~ 0xFFFFFFFF*/
    uint32 CyclicResumeCounter;
    /** debounce counter @range 0 ~ 0xFFFFFFFF*/
    uint32 DebounceCounter;
    /** period Counter @range 0 ~ 0xFFFFFFFF*/
    uint32 PeriodCounter;
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    /** specific data relevant to ICV generation @range NA*/
    CanTSyn_ICVGenarationType ICVGenaration;
#endif
} CanTSyn_MasterTimeInfoType;

/**
 * @brief store the time information for a time slave
 */
typedef struct
{
    /** seconds portion of synchronized time @range 0 ~ 0xFFFFFFFF*/
    uint32 T0SYNC;
    /** virtual local time of T2 @range 0 ~ 0xFFFFFFFFFFFFFFFF*/
    uint64 T2VLT;
    /** The byte 0 of user data @range 0 ~ 0xFF*/
    uint8 UserByte0;
    /** The byte 1 of user data @range 0 ~ 0xFF*/
    uint8 UserByte1;
    /** sequence counter @range 0 ~ 0xFF*/
    uint8 SequenceCounter;
    /** whether the SYNC/OFS message is CRC validated @range 0 ~ 1*/
    boolean IsCRC;
    /** reception timeout counter @range 0 ~ 4000 (0 ~ 4s) */
    uint16 FollowUpTimeoutTicks;
    /** debounce counter @range 0 ~ 4000 (0 ~ 4s) */
    uint16 RxDebounceTimeTicks;
    /** specifying if firstly receiving a SYNC message @range 0 ~ 1*/
    boolean FirstlyReceivedSync;
    /** specifying the number of consecutive valid message pairs @range 0 ~ 0xFF*/
    uint8 SequenceCounterHysteresis;
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    /** specific data relevant to ICV verification @range NA*/
    CanTSyn_ICVVerificationType ICVVerification;
#endif
} CanTSyn_SlaveTimeInfoType;

/**
 * @brief define configuration of the global time master
 */
typedef struct
{
    /** defines the time where the 1st regular cycle time based message transmission takes place @range 0 ~ 0xFFFFFFFF*/
    uint32 CyclicMsgResumeTime;
    /** TX debounce time @range 0 ~ 0xFFFFFFFF*/
    uint32 GlobalTimeDebounceTime;
    /** whether or not CRC is supported @range 0 ~ 1*/
    CrcSecuredType GlobalTimeTxCrcSecured;
    /** configuration of the TX period @range 0 ~ 0xFFFFFFFF*/
    uint32 GlobalTimeTxPeriod;
    /** Enables/Disables the cyclic polling of StbM_GetTimeBaseUpdateCounter() @range 0 ~ 1*/
    boolean ImmediateTimeSync;
    /** reference to pdu @range 0 ~ 0xFFFF*/
    PduIdType GlobalTimePduId;
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    /** specifying the configuration of CanTSynGlobalTimeTxIcvGeneration @range NA*/
    const CanTSyn_GlobalTimeTxIcvGenerationType* GlobalTimeTxIcvGeneration;
    /** This parameter controls whether or not ICV generation shall be supported @range 0 ~ 1*/
    CanTSyn_GlobalTimeTxIcvSecuredType GlobalTimeTxIcvSecured;
#endif
} CanTSyn_GlobalTimeMasterType;

/**
 * @brief define configuration of the global time slave
 */
typedef struct
{
    /** Rx timeout for the follow-up message @range 0 ~ 4000 (0 ~ 4s) */
    uint16 FollowUpTimeoutTicks;
    /** This represents the configuration of a RX debounce time for the Sync and FUP. 0 ~ 4000 (0 ~ 4s) */
    uint16 RxDebounceTimeTicks;
    /** the maximum allowed gap of the Sequence Counter between two SYNC resp. two OFS messages @range 0 ~ 0xFF*/
    uint8 GlobalTimeSequenceCounterJumpWidth;
    /** whether or not validation of the CRC is supported @range 0 ~ 3*/
    CrcValidatedType RxCrcValidated;
    /** reference to pdu @range 0 ~ 0xFFFF*/
    PduIdType GlobalTimePduId;
    /** Physical Layer specific propagation delay of a SYNC message to be compensated upon SYNC reception. Unit:
     * nanoseconds. @range 0 ~ 0xFFFFFFFFFFFFFFFF*/
    uint64 GlobalTimePropagationDelay;
    /** specifying the number of consecutive valid message pairs that are required by the Time Slave while being
     * in Timeout state until a Time Tuple is forwarded to the StbM. @range 0 ~ 0xFF*/
    uint8 GlobalTimeSequenceCounterHysteresis;
#if (CANTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    /** specifying the configuration of CanTSynGlobalTimeRxIcvVerification @range NA*/
    const CanTSyn_GlobalTimeRxIcvVerificationType* GlobalTimeRxIcvVerification;
    /** Definition of whether or not ICV verification shall be supported. @range 0 ~ 3*/
    CanTSyn_RxIcvVerificationType RxIcvVerification;
#endif
} CanTSyn_GlobalTimeSlaveType;

/**
 * @brief define DataIDList messages
 */
typedef struct
{
    /** index of data Id list @range 0 ~ 0xFF*/
    uint8 GlobalTimeDataIDListIndex;
    /** value of data Id list @range 0 ~ 0xFF*/
    uint8 GlobalTimeDataIDListValue;
} CanTSyn_DataIDListElementType;

/**
 * @brief define CanTSyn GlobalTimeDomain
 */
typedef struct
{
    /** The global time domain ID @range 0 ~ 0xFF*/
    uint8 GlobalTimeDomainId;
    /** Switches support CAN FD on or off @range 0 ~ 1*/
    boolean UseExtendedMsgFormat;
    /** Reference to time-base @range 0 ~ 0xFF*/
    StbM_SynchronizedTimeBaseType SynchronizedTimeBaseId;
    /** The DataIDList for SYNC messages @range NA*/
    const CanTSyn_DataIDListElementType* GlobalTimeSyncDataIDList;
    /** The DataIDList for FUP messages @range NA*/
    const CanTSyn_DataIDListElementType* GlobalTimeFupDataIDList;
    /** Configuration of the global time master @range NA*/
    const CanTSyn_GlobalTimeMasterType* GlobalTimeMaster;
    /** Configuration of the global time slave @range NA*/
    const CanTSyn_GlobalTimeSlaveType* GlobalTimeSlave;
#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
    /** Unique identifier of a ApplicationId of this Connection @range 0..65535. */
    ApplicationType ApplicationId;
#endif
#if (CANTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON) || (CANTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
    /** Enables/disables time recording for Time Validation for a specific Time Domain. @range 0 ~ 1*/
    boolean EnableTimeValidation;
#endif
    /** This represents the numerical identifier of the network on system level scope where
     * this Global Time has been communicated on. @range 0 ~ 0xFF*/
    uint8 GlobalTimeNetworkSegmentId;
#if CANTSYN_HARDWARE_TIMESTAMP_ENABLE == STD_ON
    /** specifying if the StbM uses the CAN controller hardware counter as Virtual Local Time
     * source for the Time Base (refer to StbMLocalTimeClock) @range 0 ~ 1*/
    boolean IsTimeBaseSourceSameAsTransmitter;
#endif
} CanTSyn_GlobalTimeDomainType;

/**
 * @brief This is the base type for the configuration of the Time Synchronization over CAN.
 *        A pointer to an instance of this structure will be used in the initialization
 *        of the Time Synchronization over CAN.
 */
typedef struct
{
    /** the pointer of GlobalTimeDomain @range NA*/
    const CanTSyn_GlobalTimeDomainType* GlobalTimeDomain;
} CanTSyn_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */
#define CANTSYN_START_SEC_CONST_PTR
#include "CanTSyn_MemMap.h"
extern CanTSyn_MasterTimeInfoType* const CanTSyn_MasterTimeInfoPtr[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_CONST_PTR
#include "CanTSyn_MemMap.h"

#define CANTSYN_START_SEC_CONST_PTR
#include "CanTSyn_MemMap.h"
extern CanTSyn_SlaveTimeInfoType* const CanTSyn_SlaveTimeInfoPtr[CANTSYN_GLOBALTIMEDOMAIN_NUMBER];
#define CANTSYN_STOP_SEC_CONST_PTR
#include "CanTSyn_MemMap.h"

#if CANTSYN_MULTIPLE_PARTITION_ENABLED == STD_ON
#define CANTSYN_START_SEC_CONFIG_DATA_32
#include "CanTSyn_MemMap.h"
extern const ApplicationType CanTSyn_OsApplicationList[CANTSYN_PARTITION_NUMBER];
#define CANTSYN_STOP_SEC_CONFIG_DATA_32
#include "CanTSyn_MemMap.h"
#endif

/* ========================================= external function declarations ========================================= */
#define CANTSYN_START_SEC_CODE
#include "CanTSyn_MemMap.h"
/**
 * @brief        This function needs to be called periodically to perform the AUTOSAR CanTSyn activities
 * @reentrant    TRUE (Reentrant for different partitions. Non reentrant for the same partition.)
 * @synchronous  FALSE
 * @trace        CPD-85810
 */
void CanTSyn_MainFunction(void);
#define CANTSYN_STOP_SEC_CODE
#include "CanTSyn_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif
