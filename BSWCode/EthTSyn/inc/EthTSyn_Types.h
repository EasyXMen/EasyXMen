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
 ***********************************************************************************************************************
 **
 **  @file               : EthTSyn.h
 **  @author             : yuzhe.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : type header of EthTSyn
 **
 ***********************************************************************************************************************/

#ifndef ETHTSYN_TYPES_H
#define ETHTSYN_TYPES_H
/* =================================================== inclusions =================================================== */
#include "EthTSyn_Cfg.h"
#include "StbM_Types.h"
#include "EthIf.h"
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
#include "Os.h"
#endif

#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#include "Csm.h"
#endif
/* ===================================================== macros ===================================================== */
/**
 * @name ETHTSYN_MessageTypes
 * @{
 * @brief message type definitions for Ethernet Time Synchronization.
 */
#define SYNC                             (uint8)0x00u /**< Sync message type. */
#define PDELAY_REQ                       (uint8)0x02u /**< Pdelay_Req message type. */
#define PDELAY_RESP                      (uint8)0x03u /**< Pdelay_Resp message type. */
#define FOLLOW_UP                        (uint8)0x08u /**< Follow_Up message type. */
#define PDELAY_RESP_FOLLOW_UP            (uint8)0x0Au /**< Pdelay_Resp_Follow_Up message type. */
#define MSG_NONE                         (uint8)0xFFu /**< invalid/placeholder. */
#define ETHTSYN_DATA_LIST_LENGTH         ((uint8)16u) /**< Length of data list for message processing. */
#define ETHTSYN_ICV_CIRCULAR_BUFFER_SIZE ((uint16)ETHTSYN_MAX_ICV_SIZE + 439u) /**< maximum ICV size plus overhead. */
#define ETHTSYN_ICV_DATA_BLOCK_NUMBER    ((uint8)2u) /**< Number of data blocks for ICV processing. */
#define ETHTSYN_MAX_FRESHNESS_VALUE_SIZE ((uint8)8u) /**< Maximum size of freshness value in bytes. */
/** @} */

/**
 * @name ETHTSYN_StandardConstants
 * @{
 * @brief Standard constants for time synchronization.
 */
#define ETHTSYN_PORT_IDENTITY_LENGTH ((uint8)10u)   /**< Length of port identity field in bytes */
#define ETHTSYN_NANOSECOND_TO_SECOND (1000000000UL) /**< nanoseconds per second */
/** @} */
/* ================================================ type definitions ================================================ */
/**
 * @brief     This type shall Determine whether the CRC verification is enabled for each specific field under the
 * sub-TLV.
 */
typedef struct
{
    boolean CrcMessageLength; /**< messageLength from the Follow_Up Message Header shall be included in CRC calculation.
                                 @range TRUE, FALSE */
    boolean CrcDomainNumber;  /**<  domainNumber from the Follow_Up Message Header shall be included in CRC calculation.
                                 @range TRUE, FALSE */
    boolean CrcCorrectionField;    /**< correctionField from the Follow_Up Message Header shall be included in CRC
                                      calculation. @range TRUE, FALSE */
    boolean CrcSourcePortIdentity; /**< sourcePortIdentity from the Follow_Up Message Header shall be included in CRC
                                      calculation. @range TRUE, FALSE */
    boolean CrcSequenceId; /**< sequenceId from the Follow_Up Message Header shall be included in CRC calculation.
                              @range TRUE, FALSE */
    boolean CrcPreciseOriginTimestamp; /**< preciseOriginTimestamp from the Follow_Up Message Field shall be included in
                                          CRC calculation. @range TRUE, FALSE */
} EthTSyn_CrcTimeFlagsTxSecuredCfgType;

typedef EthTSyn_CrcTimeFlagsTxSecuredCfgType EthTSyn_CrcFlagsRxValidatedCfgType;
/**
 * @brief This represents the configuration of whether or not CRC is supported.
 */
typedef enum
{
    ETHTSYN_CRC_NOT_SUPPORTED, /**< CRC not supported for this configuration @range NA*/
    ETHTSYN_CRC_SUPPORTED      /**< CRC supported for this configuration @range NA*/
} EthTSyn_CrcSecuredType;
/**
 * @brief     Definition of whether or not validation of the CRC takes place.
 */
typedef enum
{
    ETHTSYN_CRC_IGNORED,       /**< CRC validation is ignored @range 0u */
    ETHTSYN_CRC_NOT_VALIDATED, /**< CRC is not validated @range 1u */
    ETHTSYN_CRC_OPTIONAL,      /**< CRC validation is optional @range 2u */
    ETHTSYN_CRC_VALIDATED      /**< CRC is validated @range 3u */
} EthTSyn_CrcValidatedType;
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
/**
 * @brief     Definition of whether or not ICV verification shall be supported.
 */
typedef enum
{
    ETHTSYN_ICV_IGNORED,      /**< ICV verification is ignored @range 0u */
    ETHTSYN_ICV_NOT_VERIFIED, /**< ICV is not verified @range 1u */
    ETHTSYN_ICV_OPTIONAL,     /**< ICV verification is optional @range 2u */
    ETHTSYN_ICV_VERIFIED      /**< ICV is verified @range 3u */
} EthTSyn_RxIcvVerificationType;
#endif
/**
 * @brief     This type shall define the configuration of the time slave.
 */
typedef struct
{
    uint16 FollowUpTimeoutTicks; /**< Timeout value of the Follow_Up message (of the subsequent Sync message). @range
                                    0..4000 (0..4s) */
    uint16 RxDebounceTimeTicks;  /**< RX debounce time for global time messages @range 0..4000 (0..4s) */
    EthTSyn_CrcValidatedType
        RxCrcValidated; /**< Definition of whether or not validation of the CRC takes place. @range NA */
    const EthTSyn_CrcFlagsRxValidatedCfgType*
        CrcFlagsRxValidated; /**< This container collects definitions which parts of the Follow_Up message elements
                                shall be included in CRC validation. @range NA */
    uint16 GlobalTimeSequenceCounterJumpWidth; /**< specifies the maximum allowed jump of the Sequence Counter between
                                                  two consecutive Sync messages @range 0x0000u..0xFFFFu */
    uint8 GlobalTimeSequenceCounterHysteresis; /**< Number of consecutive valid message pairs required by Time Slave in
                                                  Timeout state @range 0x00u..0xFFu */
    boolean RxSubTLVStatus; /**< Indicates if Sub-TLV:Status (Secured/Not Secured) is present and evaluated @range TRUE,
                               FALSE */
    boolean RxSubTLVTime;   /**< Indicates if Sub-TLV:Time Secured is present and evaluated @range TRUE, FALSE */
    boolean RxSubTLVUserData; /**< Indicates if Sub-TLV:UserData (Secured/Not Secured) is present and evaluated @range
                                 TRUE, FALSE */
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    EthTSyn_RxIcvVerificationType
        RxIcvVerification; /**< Definition of whether ICV verification is supported @range NA */
#endif
} EthTSyn_GlobalTimeSlaveCfgType;
/**
 * @brief     Configuration structure for propagation delay measurement in Ethernet Time Synchronization.
 */
typedef struct
{
    boolean GlobalTimePdelayRespEnable; /**< Enables/disables Pdelay_Resp/Pdelay_Resp_Follow_Up transmission @range
                                           TRUE, FALSE */
    sint8  LogMessageInterval;          /**< Time interval for generating log messages @range -128..127 */
    uint64 GlobalTimePropagationDelay;  /**< Default propagation delay value before first measurement @range
                                           0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint64 GlobalTimeTxPdelayReqPeriod; /**< TX period for Pdelay_Req messages (0 disables cyclic measurement) @range
                                           0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint64 PdelayLatencyThreshold;      /**< Threshold for calculated propagation delay @range
                                           0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint64 PdelayRespAndRespFollowUpTimeout; /**< Timeout for Pdelay_Resp and Pdelay_Resp_Follow_Up messages @range
                                                0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
    boolean RateRatioEnable;           /**< Enables/disables neighbor rate ratio calculation @range TRUE, FALSE */
    uint8   RateRatioMeasurementCount; /**< Number of successful pDelay measurements for rate ratio calculation @range
                                          0x00u..0xFFu */
#endif
} EthTSyn_PdelayConfigCfgType;
/**
 * @brief     This type shall define the configuration of the time master.
 */
typedef struct
{
    uint64 GlobalTimeTxPeriod;  /**< Configuration of TX period @range 0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    sint8  LogMessageInterval;  /**< Time interval for log message generation @range -128..127 */
    uint64 CyclicMsgResumeTime; /**< Time for 1st regular cycle time based message transmission after immediate
                                   transmission @range 0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    EthTSyn_CrcSecuredType GlobalTimeTxCrcSecured; /**< Configuration indicating whether CRC is supported @range NA */
    boolean
        ImmediateTimeSync; /**< Enables/Disables cyclic polling of StbM_GetTimeBaseUpdateCounter() @range TRUE, FALSE */
    boolean TLVFollowUpStatusSubTLV; /**< Configuration indicating whether AUTOSAR Follow_Up TLV Status Sub-TLV is used
                                        @range TRUE, FALSE */
    boolean TLVFollowUpTimeSubTLV;   /**< Configuration indicating whether AUTOSAR Follow_Up TLV Time Sub-TLV is used
                                        @range TRUE, FALSE */
    boolean TLVFollowUpUserDataSubTLV; /**< Configuration indicating whether AUTOSAR Follow_Up TLV UserData Sub-TLV is
                                          used @range TRUE, FALSE */
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    boolean TLVFollowUpICVSubTLV; /**< Configuration indicating whether AUTOSAR Follow_Up TLV Time Authenticated Sub-TLV
                                     is used @range TRUE, FALSE */
#endif
    const EthTSyn_CrcTimeFlagsTxSecuredCfgType*
        CrcTimeFlagsTxSecured; /**< Configuration for which parts of Follow_Up message elements are used for CRC
                                  calculation @range NA */
    uint64 CorrectionField;    /**< CorrectionField used when transmitting Sync/Follow_Up Message by Master @range
                                  0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
#if ETHTSYN_CRC_SUPPORT == STD_ON
    uint8 CRCTimeFlags; /**< CRC Time Flags included in Follow_Up Messages sent by Master @range 0x00u..0xFFu */
#endif
    uint64 HoldOverTime; /**< Timeout for transmission of Sync and Follow_Up messages on Master ports @range
                            0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
} EthTSyn_GlobalTimeMasterCfgType;
/**
 * @brief     This type shall define the configuration of role of a port during the synchronization process.
 */
typedef struct
{
    const EthTSyn_GlobalTimeMasterCfgType*
        GlobalTimeMaster; /**< Configuration of the global time master (exactly one per domain) @range NA */
    const EthTSyn_GlobalTimeSlaveCfgType*
        GlobalTimeSlave; /**< Configuration of a time slave (at least one per domain) @range NA */
} EthTSyn_PortRoleCfgType;
/**
 * @brief     This type shall define the role of a port during time synchronization process.
 */
typedef enum
{
    ETHTSYN_DYNAMIC, /**< Port role is dynamically determined @range 0u */
    ETHTSYN_MASTER,  /**< Port acts as time master @range 1u */
    ETHTSYN_SLAVE    /**< Port acts as time slave @range 2u */
} EthTSyn_PortRoleType;
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
/**
 * @brief     Structure for ICV-related RX PDU data handling.
 */
typedef struct
{
    uint8* pduDataPtr; /**< Pointer to PDU data @range 0x00..0xFFu */
    uint16 pduLength;  /**< Length of PDU data @range 0x0000u..0xFFFFu */
} EthTSyn_ICVRxPduType;
/**
 * @brief     This type shall define Symmetric or asymmetric cryptography selection for the ICV generation.
 */
typedef enum
{
    ETHTSYN_ICV_MAC,      /**< CSM primitive is MAC (Message Authentication Code) @range 0u */
    ETHTSYN_ICV_SIGNATURE /**< CSM primitive is SIGNATURE @range 1u */
} EthTSyn_CriptographySelection;
/**
 * @brief    Define the encryption and decryption process, synchronous or asynchronous.
 */
typedef enum
{
    ETHTSYN_CRYPTO_PROCESSING_ASYNC, /**< Indicate the encryption and decryption process is asynchronous. @range 0u*/
    ETHTSYN_CRYPTO_PROCESSING_SYNC   /**< Indicate the encryption and decryption process is synchronous. @range 1u*/
} EthTSyn_CryptoProcessingType;
/**
 * @brief    Define information about the Csm job of generation.
 */
typedef struct
{
    uint32 IcvGenerationJobRef; /**< Reference to CSM job to fetch CSM job ID @range 0x00000000u..0xFFFFFFFFu */
    EthTSyn_CryptoProcessingType
        IcvProcessingType; /**< Encryption/decryption process type (synchronous/asynchronous) @range NA */
    EthTSyn_CriptographySelection
        IcvGenerationBase; /**< Symmetric or asymmetric cryptography selection for ICV generation @range NA */
} EthTSyn_TxCsmJobType;

/**
 * @brief    Define information about the Csm job of verification.
 */
typedef struct
{
    uint32 IcvVerificationJobRef; /**< specifying the reference to the CSM job to fetch the CSM job ID. @range
                                     0x00000000u..oxFFFFFFFFu */
    EthTSyn_CryptoProcessingType
        IcvProcessingType; /**< Indicate the encryption and decryption process,synchronous or asynchronous. @range NA */
    EthTSyn_CriptographySelection IcvVerificationBase; /**<  specifying Symmetric or asymmetric cryptography selection
                                                          for the ICV verificaiton. @range NA */
} EthTSyn_RxCsmJobType;
/**
 *  @brief  This type shall define the configuration of ICV generation.
 */
typedef struct
{
    uint64 IcvGenerationTimeout; /**< Timeout value for ICV generation process @range
                                    0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint16
        IcvTxLength; /**< Length of ICV to be transmitted within Follow_Up Message (in bytes) @range 0x0000u..0xFFFFu */
    uint16 TxAuthenticationBuildAttempts; /**< Number of authentication build attempts @range 0x0000u..0xFFFFu */
    uint16
        IcvGenerationFvIdRef; /**< Reference to Freshness Value (FV) used for ICV generation @range 0x0000u..0xFFFFu */
    EthTSyn_TxCsmJobType TxICVGenerationConfig; /**< Configuration for ICV generation job @range NA */
    uint32 IcvTxFreshnessValueTruncLength; /**< Length in bits of Freshness Value to be included in secured messages
                                              @range 0x00000000u..0xFFFFFFFFu */
    uint32 IcvTxFreshnessValueLength;      /**< Length in bits of the provided freshness value @range
                                              0x00000000u..0xFFFFFFFFu */
} EthTSyn_GlobalTimeTxIcvGenerationType;

/**
 * @brief     Configuration structure for ICV (Integrity Check Value) verification in Ethernet Time Synchronization.
 */
typedef struct
{
    uint64 IcvVerificationTimeout; /**< Timeout value for ICV verification process @range
                                      0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint16 IcvRxLength; /**< Length of ICV to be received within Follow_Up Message (in bytes) @range 0x0000u..0xFFFFu */
    uint16 IcvVerificationAttempts; /**< Number of ICV verification attempts after failure @range 0x0000u..0xFFFFu */
    uint16 RxAuthenticationBuildAttempts; /**< Number of authentication build attempts after ICV verification failure
                                             @range 0x0000u..0xFFFFu */
    uint32 IcvVerificationFvIdRef;        /**< Reference to Freshness Value (FV) used for ICV verification @range
                                             0x00000000u..0xFFFFFFFFu */
    EthTSyn_RxCsmJobType RxICVVerificationConfig; /**< Configuration for ICV verification job @range NA */
    uint32 IcvRxFreshnessValueTruncLength; /**< Length in bits of Freshness Value to be included in secured messages
                                              @range 0x00000000u..0xFFFFFFFFu */
    uint32 IcvRxFreshnessValueLength;      /**< Length in bits of the provided freshness value @range
                                              0x00000000u..0xFFFFFFFFu */
    uint8* RxIcvCircularBuffer;            /**< Circular buffer pointer for ICV verification @range NA */
} EthTSyn_GlobalTimeRxIcvVerificationType;
/**
 * @brief     Structure describing the Time Authenticated Field for ICV processing.
 */
typedef struct
{
    uint16 TimeAuthenticatedLength; /**< Length of Sub-Tlv Time Authenticated @range 0x0000u..0xFFFFu */
    uint8  ICVFlags;                /**< ICV Flags indicating processing options @range 0x00u..0xFFu */
    uint8  TruncatedFreshnessValue[ETHTSYN_MAX_FRESHNESS_VALUE_SIZE]; /**< Freshness Value array @range NA */
    uint8  ICVBuffer[ETHTSYN_MAX_ICV_SIZE];                           /**< Generated ICV buffer @range NA */
} EthTSyn_AuthenticatedFieldInfoType;
/**
 * @brief     Structure defining parameters of a circular buffer used for ICV data storage.
 */
typedef struct
{
    uint8* Buffer;   /**< Pointer to the buffer @range NA */
    uint16 Capacity; /**< Capacity of the buffer @range 0x0000u..0xFFFFu */
    uint16 Head;     /**< Index of the write position @range 0x0000u..0xFFFFu */
    uint16 Tail;     /**< Index of the read position @range 0x0000u..0xFFFFu */
    uint16 Count;    /**< Current number of data stored in the buffer @range 0x0000u..0xFFFFu */
} EthTSyn_CircularBufferType;
/**
 * @brief     Structure defining runtime variables related to ICV (Integrity Check Value).
 */
typedef struct
{
    uint64 IcvIndicationStartTime;             /**< Local time when ICV generation/verification timer starts @range
                                                  0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint16 AuthenticationBuildAttemptsCounter; /**< ICV generation/verification build counter @range 0x0000u..0xFFFFu */
    uint16 IcvVerificationAttemptsCounter;     /**< ICV verification counter @range 0x0000u..0xFFFFu */
    uint8  TimeAuthencatedSequenceCounter;  /**< Sequence counter of Sub-Tlv Time Authenticated @range 0x00u..0xFFu */
    uint8  IcvProgressStatus;               /**< ICV generation or verification status @range 0x00u..0xFFu */
    uint8  TimeAuthenticatedFieldOffset;    /**< Data offset of Sub-Tlv Time Authenticated @range 0x00u..0xFFu */
    Crypto_VerifyResultType RxVerifyResult; /**< Result of ICV verification @range NA */
    EthTSyn_AuthenticatedFieldInfoType
                               AuthenticatedFieldInfo; /**< Data field of Sub-Tlv Time Authenticated @range NA */
    EthTSyn_CircularBufferType RxICVBufferToken;       /**< ICV circular buffer token @range NA */
} EthTSyn_ICVGenerationAndVerificationType;

#endif
/**
 * @brief     Configuration structure for a port under a time domain.
 */
typedef struct
{
    boolean SynchronizedTimeBaseUseEthFreerunning; /**< Indicates if StbM uses EthFreerunning @range TRUE, FALSE */
    EthTSyn_PortRoleType GlobalTimePortRole;       /**< Specifies the port role @range NA */
    const EthTSyn_PdelayConfigCfgType*
        PdelayConfig; /**< Configuration of cyclic propagation delay measurement @range NA */
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
    const EthTSyn_GlobalTimeTxIcvGenerationType*
        GlobalTimeTxIcvGeneration; /**< Configuration of ICV generation @range NA */
    const EthTSyn_GlobalTimeRxIcvVerificationType*
        GlobalTimeRxIcvVerification; /**< Configuration of ICV verification @range NA */
#endif
#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON) || (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
    boolean EnableTimeValidation; /**< Indicates if Time Validation is enabled @range TRUE, FALSE */
#endif
} EthTSyn_PortConfigCfgType;
/**
 * @brief     Structure defining a single data ID used for CRC calculation or message authentication.
 */
typedef struct
{
    uint8 GlobalTimeFollowUpDataIDListIndex; /**< Index of DataIDList element @range 0x00u..0xFFu */
    uint8 GlobalTimeFollowUpDataIDListValue; /**< Value of DataIDList element @range 0x00u..0xFFu */
} EthTSyn_GlobalTimeFollowUpDataIDListElementCfgType;
/**
 * @brief     Structure defining the list of data IDs used for CRC calculation or message authentication.
 */
typedef struct
{
    EthTSyn_GlobalTimeFollowUpDataIDListElementCfgType
        GlobalTimeFollowUpDataIDListElement[ETHTSYN_DATA_LIST_LENGTH]; /**< Element of DataIDList for Follow_Up message
                                                                          identification @range NA */
} EthTSyn_GlobalTimeFollowUpDataIDListCfgType;
/**
 * @brief     This type shall define the configuration of a time domain.
 */
typedef struct
{
    uint8 GlobalTimeDomainId;         /**< The global time domain ID @range 0x00u..0xFFu */
    uint8 FramePrio;                  /**< Priority of outgoing EthTSyn messages @range 0x00u..0xFFu */
    uint8 SynchronizedTimeBaseRef;    /**< Mandatory reference to required synchronized time-base @range 0x00u..0xFFu */
    uint8 GlobalTimeEthIfRef;         /**< Reference to Ethernet interface for fetching global time information @range
                                         0x00u..0xFFu */
    EthTSyn_PortRoleCfgType PortRole; /**< Role of EthTSyn-Port (Master or Slave) @range NA */
    uint64                  GlobalTimeDebounceTime; /**< TX debounce time for Sync and Follow_Up messages @range
                                                       0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    const EthTSyn_GlobalTimeFollowUpDataIDListCfgType*
        GlobalTimeFollowUpDataIDList;            /**< DataIDList for Follow_Up message identification @range NA */
    const EthTSyn_PortConfigCfgType* PortConfig; /**< Configuration of EthTSyn-Ports within TimeDomain @range NA */
#if (ETHTSYN_MULTIPLE_PARTITION_USED == STD_ON)
    ApplicationType PortReferrencedPartitionId; /**< Partition ID of port @range NA */
#endif
    uint8                   ClkUnitRef;    /**< Clock unit reference @range 0x00u..0xFFu */
    StbM_ProtocolParamType* ProtocolParam; /**< Specific parameters for individual TimeSync modules @range NA */
} EthTSyn_GlobalTimeDomainCfgType;

/**
 * @brief     Enumeration defining the status of Pdelay Delay Measurement Requester during pdelay measurement.
 */
typedef enum
{
    ETHTSYN_SEND_PDELAY_REQ,              /**< Send Pdelay_Req message @range 0u */
    ETHTSYN_WAITFOR_PDELAY_RESP,          /**< Wait for Pdelay_Resp message @range 1u */
    ETHTSYN_WAITFOR_PDELAY_RESP_FOLLOWUP, /**< Wait for Pdelay_Resp_Follow_Up message @range 2u */
} EthTSyn_PdelayTXStatusType;

/**
 * @brief     Enumeration defining the status of Pdelay Delay Measurement Receiver during pdelay measurement.
 */
typedef enum
{
    ETHTSYN_WAITFOR_PDELAY_REQ,        /**< Wait for Pdelay_Req message @range 0u */
    ETHTSYN_SEND_PDELAY_RESP,          /**< Send Pdelay_Resp message @range 1u */
    ETHTSYN_SEND_PDELAY_RESP_FOLLOWUP, /**< Send Pdelay_Resp_Follow_Up message @range 2u */
} EthTSyn_PdelayRXStatusType;

/**
 * @brief     Enumeration defining the status of a port during synchronization process.
 */
typedef enum
{
    ETHTSYN_SEND_SYNC,     /**< Send Sync message @range 0u */
    ETHTSYN_SEND_FOLLOWUP, /**< Send Follow_Up message @range 1u */
} EthTSyn_SyncStatusType;
/**
 * @brief     Enumeration defining the mode of transmission for Ethernet Time Synchronization.
 */
typedef enum
{
    ETHTSYN_TX_OFF, /**< Transmission mode OFF @range 0*/
    ETHTSYN_TX_ON   /**< Transmission mode ON @range 1*/
} EthTSyn_TransmissionModeType;
/**
 * @brief     Structure defining the format of a global timestamp.
 */
typedef struct
{
    uint32 Nanoseconds; /**< Nanoseconds component of timestamp @range 0x00000000u..0xFFFFFFFFu */
    uint32 Seconds;     /**< Lower 32-bit seconds component of timestamp @range 0x00000000u..0xFFFFFFFFu */
    uint16 SecondsHi;   /**< Higher 16-bit seconds component of timestamp @range 0x0000u..0xFFFFu */
} EthTSyn_TimeStampType;
/**
 * @brief     Structure defining ingress and egress timestamps during Pdelay measurement.
 */
typedef struct
{
    EthTSyn_TimeStampType PdelayReqEventIngressTimestamp; /**< Ingress timestamp for pdelay_req message @range NA */
    EthTSyn_TimeStampType PdelayRespEventEgressTimestamp; /**< Egress timestamp for pdelay_response message @range NA */
} EthTSyn_PdelayResponderTimestampType;
/**
 * @brief     Structure defining the egress time tuple type, including global and local time.
 */
typedef struct
{
    EthTSyn_TimeStampType T0;    /**< T0 for tx preciseOriginTimestamp @range NA */
    EthTSyn_TimeStampType T0vlt; /**< T0vlt for tx preciseOriginTimestamp @range NA */
} EthTSyn_SyncEgressTupleType;

/**
 * @brief     Structure for parameters related to rate ratio calculation in Ethernet Time Synchronization.
 */
typedef struct
{
    float64 NeighborRateRatio; /**< Neighbor Rate Ratio between local clocks of Peer Delay Responder and Initiator
                                  @range 0.0..DBL_MAX */
    float64
        RateRatioPdelayResponder; /**< Rate ratio of Peer Delay Responder to Global Time Master @range 0.0..DBL_MAX */
#if (ETHTSYN_RATE_RATIO_ENABLE == STD_ON)
    uint8 RateRatioMesuredCounter; /**< Number of successful pDelay measurements for neighbor rate ratio calculation
                                      @range 0x00u..0xFFu */
    EthTSyn_TimeStampType T30;     /**< First egress timestamps of Pdelay_Resp messages on Responder side @range NA */
    EthTSyn_TimeStampType T40;     /**< First ingress timestamps of Pdelay_Resp messages on Initiator side @range NA */
    boolean HasCaculatedNeighborRateRatio; /**< Indicates if valid neighborRateRatio has been calculated @range TRUE,
                                              FALSE */
#endif
} EthTSyn_RateRatioType;

/**
 * @brief Structure which contains port identity data
 */
typedef struct
{
    uint64 ClockIdentity; /**< Clock identity @range 0...0xFFFFFFFFFFFFFFFF */
    uint16 PortNumber;    /**< Number of Ethernet port @range 0...0xFFFF */
} EthTSyn_PortIdType;

/**
 * @brief     Runtime variable structure for an Ethernet Time Synchronization port.
 */
typedef struct
{
    uint8                        TimeDomainId; /**< The ID of the time domain @range 0x00u..0xFFu */
    uint8                        CtrlId;       /**< The ID of ETH control @range 0x00u..0xFFu */
    uint16                       DomainIndex;  /**< Index of domain in configured domain list @range 0x0000u..0xFFFFu */
    EthTSyn_PortIdType           ThisPortIdentity;     /**< Port identity of this port @range NA */
    EthTSyn_PortIdType           PortIdentity;         /**< Sender port identity of received sync message @range NA */
    StbM_UserDataType            UserData;             /**< User data from StbM @range NA */
    EthTSyn_TransmissionModeType TransmissionMode;     /**< Handles enabling/disabling of transmission mode @range NA */
    EthTrcv_LinkStateType        EthtrcvLinkState;     /**< Link state of Ethernet transceiver @range NA */
    EthTSyn_PortRoleType         PortRoleType;         /**< Role of the port @range NA */
    uint8                        TranssmitMessageType; /**< Type of message to be transmitted @range 0x00u..0xFFu */
    EthTSyn_PdelayTXStatusType
        PdelayTXStatusType; /**< Status of Pdelay Delay Measurement Requester @range 0xFFu..0xFFu */
    EthTSyn_PdelayRXStatusType PdelayRXStatusType; /**< Status of Pdelay Delay Measurement Receiver @range NA */
    EthTSyn_SyncStatusType     SyncStatusType; /**< Status of SYNC message sending (only used by master) @range NA */
    uint8                      TimeBaseUpdateCounter; /**< TimeBaseUpdateCounter value @range 0x00u..0xFFu */
    uint64 SyncReqPeriod;       /**< Time of next sync message sent @range 0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint64 PdelayReqPeriod;     /**< Time of next Pdelay message sent @range 0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint64 SyncdebounceCounter; /**< DebounceCounter between SYNC and FOLLOW_UP @range
                                   0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
    uint16  SyncSequenceId;     /**< SequenceId of SYNC message @range 0x0000u..0xFFFFu */
    uint16  ReceivedSyncSequenceId;      /**< SequenceId of received SYNC message @range 0x0000u..0xFFFFu */
    uint16  PdelayReqSequenceId;         /**< SequenceId of Pdelay message @range 0x0000u..0xFFFFu */
    uint16  ReceivedPdelayReqSequenceId; /**< SequenceId of received Pdelay message @range 0x0000u..0xFFFFu */
    uint16  RxDebounceTimeTicks;
    uint16  FollowUpTimeoutTicks;
    boolean FirstlyReceivedSyncReq; /**< Indicates whether sync request is first received @range TRUE, FALSE */
    EthTSyn_TimeStampType                PreciseOriginTimestamp; /**< Time for slave to sync the time @range NA */
    EthTSyn_SyncEgressTupleType          SyncEgressTuple; /**< Time for calculating precise timestamp @range NA */
    EthTSyn_TimeStampType                T1vlt;           /**< T1vlt for rx preciseOriginTimestamp @range NA */
    EthTSyn_TimeStampType                T1;              /**< T1 for pdelay @range NA */
    EthTSyn_TimeStampType                T2;              /**< T2 for pdelay @range NA */
    EthTSyn_TimeStampType                T3;              /**< T3 for pdelay @range NA */
    EthTSyn_TimeStampType                T4;              /**< T4 for pdelay @range NA */
    EthTSyn_PdelayResponderTimestampType PdelayResponderTimestamp; /**< Request receipt timestamp @range NA */
    EthTSyn_TimeStampType                Pdelay;                   /**< Delay of sending between nodes @range NA */
    uint8                                FramePrio;                /**< Frame priority @range 0x00u..0xFFu */
    uint8                                SCHysteresisCount;        /**< Counter for debounce @range 0x00u..0xFFu */
#if (ETHTSYN_SECURE_TIME_SYNCHRONIZATION_ENABLE == STD_ON)
#if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    boolean IsSyncToGatewayBitSet; /**< Indicates if sync message is received from gateway @range TRUE, FALSE */
#endif
    uint8 RecRawDataPtr[ETHTSYN_ICV_CIRCULAR_BUFFER_SIZE]; /**< Raw data copied from Rxindication when ICV enabled
                                                              @range NA */
    uint8 TimeAuthenticatedFieldOffset; /**< Offset of timeAuthenticatedField in RecRawDataPtr @range 0x00u..0xFFu */
    EthTSyn_ICVGenerationAndVerificationType
        ICVGenarationAndVerification; /**< Specific data for ICV generation or verification @range NA */
#endif
#if (ETHTSYN_MASTER_TIME_VALIDATION_SUPPORT == STD_ON)
    StbM_TimeTupleType TrefPDResponder; /**< Used for coherent conversion of t2/t3 into Global Time values @range NA */
#endif
#if (ETHTSYN_SLAVE_TIME_VALIDATION_SUPPORT == STD_ON)
    StbM_TimeTupleType TrefPDInitiator; /**< Used for coherent conversion of t1/t4 from Virtual Local Time into Global
                                           Time @range NA */
#endif
    EthTSyn_RateRatioType RateRatio; /**< Parameters related to rate ratio calculation @range NA */
    uint64 HoldOverTimer;            /**< Timer for transmission of Sync and Follow_Up messages on Master ports @range
                                        0x0000000000000000u..0xFFFFFFFFFFFFFFFFu */
} EthTSyn_PortType;
/**
 * @brief     Main configuration structure for the Ethernet Time Synchronization module.
 */
typedef struct
{
    /** number of port/domain */
    uint8 DomainNum;
    /** specifying the configuration of TimeDomain */
    const EthTSyn_GlobalTimeDomainCfgType* GlobalTimeDomain;
    /** specifying the Runtime Variables of Port */
    EthTSyn_PortType* const* RuntimeVariables;
} EthTSyn_ConfigType;
#endif
