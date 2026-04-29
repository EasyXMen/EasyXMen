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
/* ===================================================== macros ===================================================== */
#define SYNC                     (EthTSynMessageType)0x00u
#define PDELAY_REQ               (EthTSynMessageType)0x02u
#define PDELAY_RESP              (EthTSynMessageType)0x03u
#define FOLLOW_UP                (EthTSynMessageType)0x08u
#define PDELAY_RESP_FOLLOW_UP    (EthTSynMessageType)0x0Au
#define MSG_NONE                 (EthTSynMessageType)0xFFu
#define ETHTSYN_DATA_LIST_LENGTH ((uint8)16u)

#define ETHTSYN_PORT_IDENTITY_LENGTH ((uint8)10u)
#define ETHTSYN_NANOSECOND_TO_SECOND ((uint32)1000000000UL)
/* ================================================ type definitions ================================================ */
/** This type shall Determine whether the CRC verification is enabled for each specific field under the sub-TLV.
 *
 */
typedef struct
{
    /** messageLength from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean CrcMessageLength;
    /** domainNumber from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean CrcDomainNumber;
    /** correctionField from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean CrcCorrectionField;
    /** sourcePortIdentity from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean CrcSourcePortIdentity;
    /** sequenceId from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean CrcSequenceId;
    /** preciseOriginTimestamp from the Follow_Up Message Field shall be included in CRC calculation. */
    boolean CrcPreciseOriginTimestamp;
} EthTSyn_CrcTimeFlagsTxSecuredCfgType;

typedef EthTSyn_CrcTimeFlagsTxSecuredCfgType EthTSyn_CrcFlagsRxValidatedCfgType;
/** This represents the configuration of whether or not CRC is supported.
 *
 */
typedef enum
{
    ETHTSYN_CRC_NOT_SUPPORTED,
    ETHTSYN_CRC_SUPPORTED
} EthTSyn_CrcSecuredType;
/** Definition of whether or not validation of the CRC takes place.
 *
 */
typedef enum
{
    ETHTSYN_CRC_IGNORED,
    ETHTSYN_CRC_NOT_VALIDATED,
    ETHTSYN_CRC_OPTIONAL,
    ETHTSYN_CRC_VALIDATED
} EthTSyn_CrcValidatedType;
/** This type shall define the configuration of the time slave.
 *
 */
typedef struct
{
    /** Timeout value of the Follow_Up message (of the subsequent Sync message). */
    uint64 GlobalTimeFollowUpTimeout;
    /** Definition of whether or not validation of the CRC takes place. */
    EthTSyn_CrcValidatedType RxCrcValidated;
    /** This container collects definitions which parts of the Follow_Up message elements shall be included in CRC
     * validation. */
    const EthTSyn_CrcFlagsRxValidatedCfgType* CrcFlagsRxValidated;
    /** specifies the maximum allowed jump of the Sequence Counter between two consecutive Sync messages */
    uint16 GlobalTimeSequenceCounterJumpWidth;
    /** specifies the number of consecutive valid message paris that are required by the Time Slave while being in
     * Timeout state until a Time Tuple is forwarded to the StbM. */
    uint8 GlobalTimeSequenceCounterHysteresis;
    /** Definition of whether or not a Sub-TLV:Status Secured or Sub-TLV:Status Not Secured
     * shall be present and shall be evaluated when processing a received Follow_Up message. */
    boolean RxSubTLVStatus;
    /** Definition of whether or not a Sub-TLV:Time Secured  shall be present and shall be evaluated when processing a
     * received Follow_Up message. */
    boolean RxSubTLVTime;
    /** Definition of whether or not a Sub-TLV:UserData Secured or Sub-TLV:UserData Not Secured shall be present and
     * shall be evaluated when processing a received Follow_Up message. */
    boolean RxSubTLVUserData;
} EthTSyn_GlobalTimeSlaveCfgType;
/** This type shall define the configuration of the propagation delay.
 *
 */
typedef struct
{
    /** This parameter allows disabling Pdelay_Resp / Pdelay_Resp_Follow_Up transmission, if no Pdelay_Req messages are
     * expected. */
    boolean GlobalTimePdelayRespEnable;
    /** This defines the time interval at which log messages are generated. */
    sint8 LogMessageInterval;
    /** If cyclic propagation delay measurement is enabled, this parameter represents the default value of the
    propagation delay until the first actually measured propagation delay is available. */
    uint64 GlobalTimePropagationDelay;
    /** This represents configuration of the TX period for Pdelay_Req messages.A value of 0 disables the cyclic Pdelay
     * measurement */
    uint64 GlobalTimeTxPdelayReqPeriod;
    /** Threshold for calculated Pdelay.  */
    uint64 PdelayLatencyThreshold;
    /** Timeout value for Pdelay_Resp and Pdelay_Resp_Follow_Up after a Pdelay_Req has been transmitted resp. a
     * Pdelay_Resp has been received */
    uint64 PdelayRespAndRespFollowUpTimeout;
} EthTSyn_PdelayConfigCfgType;
/** This type shall define the configuration of the time master.
 *
 */
typedef struct
{
    /** Defines the time where the 1st regular cycle time based message transmission takes place, after an immediate
     * transmission before.  */
    uint64 CyclicMsgResumeTime;
    /** This represents the configuration of whether or not CRC is supported. */
    EthTSyn_CrcSecuredType GlobalTimeTxCrcSecured;
    /** This represents configuration of the TX period. */
    uint64 GlobalTimeTxPeriod;
    /** This defines the time interval at which log messages are generated. */
    sint8 LogMessageInterval;
    /** Enables/Disables the cyclic polling of StbM_GetTimeBaseUpdateCounter() within EthTSyn_MainFunction(). */
    boolean ImmediateTimeSync;
    /** This represents the configuration of whether an AUTOSAR Follow_Up TLV Status Sub-TLV is used or not. */
    boolean TLVFollowUpStatusSubTLV;
    /** This represents the configuration of whether an AUTOSAR Follow_Up TLV Time Sub-TLV is used or not. */
    boolean TLVFollowUpTimeSubTLV;
    /** This represents the configuration of whether an AUTOSAR Follow_Up TLV UserData Sub-TLV is used or not. */
    boolean TLVFollowUpUserDataSubTLV;
    /** This container collects definitions which parts of the Follow_Up message elements shall be used for CRC
     * calculation */
    const EthTSyn_CrcTimeFlagsTxSecuredCfgType* CrcTimeFlagsTxSecured;
    /** This represents the correctionField that is used when transmitting  Sync/Follow_Up Message by Master */
    uint64 CorrectionField;
    /** This represents the CumulativeScaledRateOffset that is used when transmitting Messages by Master */
    uint32 CumulativeScaledRateOffset;
#if ETHTSYN_CRC_SUPPORT == STD_ON
    /** This indicates the CRC Time Flags that are included in the Follow - Up (FU) Messages sent by the Master. */
    uint8 CRCTimeFlags;
#endif
} EthTSyn_GlobalTimeMasterCfgType;
/** This type shall define the configuration of role of a port during the synchronization process.
 *
 */
typedef struct
{
    /** Configuration of the global time master. Each global time domain is required to have exactly one global time
     * master. */
    const EthTSyn_GlobalTimeMasterCfgType* GlobalTimeMaster;
    /** Configuration of a time slave. Each global time domain is required to have at least one time slave. */
    const EthTSyn_GlobalTimeSlaveCfgType* GlobalTimeSlave;
} EthTSyn_PortRoleCfgType;
/** This type shall define the role of a port during time synchronization process.
 *
 */
typedef enum
{
    ETHTSYN_MASTER,
    ETHTSYN_SLAVE,
} EthTSyn_PortRoleType;
/** This type shall define the configuration of the port under a time doamin.
 *
 */
typedef struct
{
    /** This optional parameter, if present, indicates the priority of outgoing EthTSyn messages */
    uint8 FramePrio;
    /** This represents the configuration of a TX debounce time for Sync and Follow_Up messages compared to a message
     * before with the same PDU. */
    uint64 GlobalTimeDebounceTime;
    /** This represents the reference to the Ethernet interface taken to fetch the global time information. */
    uint8 GlobalTimeEthIfRef;
    /** Stbm if use EthFreerunning */
    boolean SynchronizedTimeBaseUseEthFreerunning;
    /** specifying the port role */
    EthTSyn_PortRoleType GlobalTimePortRole;
    /** Configuration of cyclic propagation delay measurement. */
    const EthTSyn_PdelayConfigCfgType* PdelayConfig;
    /** Specifying the Role of the EthTSyn-Port (Master or Slave). */
    EthTSyn_PortRoleCfgType PortRole;
#if ETHTSYN_MULTIPLE_PARTITION_USED
    /** Specifying the partition id of port */
    ApplicationType PortReferrencedPartitionId;
#endif
} EthTSyn_PortConfigCfgType;
/** This type shall define the data ID,which is used for CRC caculation or message authentication.
 *
 */
typedef struct
{
    /** Index of the DataIDList for Follow_Up message ensures the identification of data elements due to CRC calculation
     * and message authentication process. */
    uint8 GlobalTimeFollowUpDataIDListIndex;
    /** Value of the DataIDList for Follow_Up message ensures the identification of data elements due to CRC calculation
     * and message authentication process. */
    uint8 GlobalTimeFollowUpDataIDListValue;
} EthTSyn_GlobalTimeFollowUpDataIDListElementCfgType;
/** This type shall define the list of  data ID,which is used for CRC caculation or message authentication.
 *
 */
typedef struct
{
    /** Element of the DataIDList for Follow_Up message ensures the identification of data elements due to CRC
     * calculation and message authentication process. */
    EthTSyn_GlobalTimeFollowUpDataIDListElementCfgType GlobalTimeFollowUpDataIDListElement[ETHTSYN_DATA_LIST_LENGTH];
} EthTSyn_GlobalTimeFollowUpDataIDListCfgType;
/** This type shall define the configuration of a time domain.
 *
 */
typedef struct
{
    /** The global time domain ID. */
    uint8 GlobalTimeDomainId;
    /** Mandatory reference to the required synchronized time-base. */
    uint8 SynchronizedTimeBaseRef;
    /** The DataIDList for Follow_Up message ensures the identification of data elements due to CRC calculation and
     * message authentication process. */
    const EthTSyn_GlobalTimeFollowUpDataIDListCfgType* GlobalTimeFollowUpDataIDList;
    /** The number of PortConfig */
    uint8 PortConfigNum;
    /** Configuration of the EthTSyn-Ports within the TimeDomain. */
    const EthTSyn_PortConfigCfgType* PortConfig;
} EthTSyn_GlobalTimeDomainCfgType;

typedef uint8 EthTSynMessageType;
/** This type shall define the status of Pdelay Delay Measurement Requester during pdelay measurement.
 *
 */
typedef enum
{
    ETHTSYN_SEND_PDELAY_REQ,
    ETHTSYN_WAITFOR_PDELAY_RESP,
    ETHTSYN_WAITFOR_PDELAY_RESP_FOLLOWUP,
} EthTSyn_PdelayTXStatusType;
/** This type shall define the  status of Pdelay Delay Measurement Receiver during pdelay measurement.
 *
 */
typedef enum
{
    ETHTSYN_WAITFOR_PDELAY_REQ,
    ETHTSYN_SEND_PDELAY_RESP,
    ETHTSYN_SEND_PDELAY_RESP_FOLLOWUP,
} EthTSyn_PdelayRXStatusType;
/** This type shall define the status of a port during synchronization process.
 *
 */
typedef enum
{
    ETHTSYN_SEND_SYNC,
    ETHTSYN_SEND_FOLLOWUP,
    ETHTSYN_WAITFOR_SYNC,
    ETHTSYN_WAITFOR_FOLLOWUP,
} EthTSyn_SyncStatusType;
/** This type shall define the mode of transmission.
 *
 */
typedef enum
{
    ETHTSYN_TX_OFF,
    ETHTSYN_TX_ON
} EthTSyn_TransmissionModeType;
/** This type shall define the format of global timestamp.
 *
 */
typedef struct
{
    /** nanoseconds */
    uint32 Nanoseconds;
    /** seconds */
    uint32 Seconds;
    /** secondsHi */
    uint16 SecondsHi;
} EthTSyn_TimeStampType;
/** This type shall define the type of ingress timestamp and egress timestamp during pdelay measurement.
 *
 */
typedef struct
{
    /** ingress timestamp for pdelay_req message */
    EthTSyn_TimeStampType PdelayReqEventIngressTimestamp;
    /** egress timestamp for pdelay_resonse message */
    EthTSyn_TimeStampType PdelayRespEventEgressTimestamp;
} EthTSyn_PdelayResponderTimestampType;
/** This type shall define the type of egress time tuple,which includes the global and local time.
 *
 */
typedef struct
{
    /** The T0 for tx preciseOriginTimestamp */
    EthTSyn_TimeStampType T0;
    /** The T0vlt for tx preciseOriginTimestamp */
    EthTSyn_TimeStampType T0vlt;
} EthTSyn_SyncEgressTupleType;
/** This type shall define the runtime variable of a port.
 *
 */
typedef struct
{
    /** The id of timedomain */
    uint8 Timedomainid;
    /** The id of ETH control */
    uint8 CtrlId;
    /** Port ID in time domain */
    uint8 PortIdinTimedomain;
    /** Port index in configured port list */
    uint16 PortIndexInPortList;
    /** the index of domain in configured domain list,which the Port belongs to */
    uint16 DomainIndex;
    /** Port identity*/
    uint8 ThisPortIdentity[ETHTSYN_PORT_IDENTITY_LENGTH];
    /** The sender port identity of the received sync message */
    uint8 PortIdentity[ETHTSYN_PORT_IDENTITY_LENGTH];
    /** userData of stbm */
    StbM_UserDataType UserData;
    /** Handles the enabling and disabling of the transmission mode */
    EthTSyn_TransmissionModeType TransmissionMode;
    /** Link State */
    EthTrcv_LinkStateType EthtrcvLinkState;
    /** Role of port */
    EthTSyn_PortRoleType PortRoleType;
    /** The type of message to be transmitted */
    EthTSynMessageType TranssmitMessageType;
    /** Status of  Pdelay Delay Measurement Requester */
    EthTSyn_PdelayTXStatusType PdelayTXStatusType;
    /** Status of  Pdelay Delay Measurement Receiver */
    EthTSyn_PdelayRXStatusType PdelayRXStatusType;
    /** Status of SYNC message sending and receiving */
    EthTSyn_SyncStatusType SyncStatusType;
    /** TimeBaseUpdateCounter */
    uint8 TimeBaseUpdateCounter;
    /** Time of next sync message sent */
    uint64 SyncReqPeriod;
    /** Time of next Pdelay message sent */
    uint64 PdelayReqPeriod;
    /** DebounceCounter between SYNC and FOLLOW_UP */
    uint64 SyncdebounceCounter;
    /** DebounceCounter between PDELAY_RESP and PDELAY_RESP_FOLLOWUP */
    uint64 PdelaydebounceCounter;
    /** SequenceId of SYNC message */
    uint16 SyncSequenceId;
    /** SequenceId of received SYNC message */
    uint16 ReceivedSyncSequenceId;
    /** SequenceId of Pdelay message */
    uint16 PdelayReqSequenceId;
    /** SequenceId of received Pdelay message */
    uint16 ReceivedPdelayReqSequenceId;
    /** Indicate Whether firstly received sync req or not */
    boolean FirstlyReceivedSyncReq;
    /** The time for slave to sync the time */
    EthTSyn_TimeStampType PreciseOriginTimestamp;
    /** The time for caculating precise timestamp */
    EthTSyn_SyncEgressTupleType SyncEgressTuple;
    /** The T1vlt for rx preciseOriginTimestamp */
    EthTSyn_TimeStampType T1vlt;
    /** The t1 for pdelay */
    EthTSyn_TimeStampType T1;
    /** The t2 for pdelay */
    EthTSyn_TimeStampType T2;
    /** The t3 for pdelay */
    EthTSyn_TimeStampType T3;
    /** The t4 for pdelay */
    EthTSyn_TimeStampType T4;
    /** The requestReceiptTimestamp */
    EthTSyn_PdelayResponderTimestampType PdelayResponderTimestamp;
    /** Delay of sending between nodes */
    EthTSyn_TimeStampType Pdelay;
    /** Priority */
    uint8 FramePrio;
    /** The counter for debounce. */
    uint8 SCHysteresisCount;
} EthTSyn_PortType;
/** This type shall define the configuration of EthTSyn.
 *
 */
typedef struct
{
    /** specifying the number of TimeDomain in this configuration */
    const EthTSyn_GlobalTimeDomainCfgType* GlobalTimeDomain;
    /** specifying the Runtime Variables of Port */
    EthTSyn_PortType* const* RuntimeVariables;
    /** specifying the number of Ports in this configuration */
    const uint8 PortNum;
} EthTSyn_ConfigType;

#endif
