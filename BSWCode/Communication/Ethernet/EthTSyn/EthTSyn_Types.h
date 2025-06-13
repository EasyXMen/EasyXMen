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
**  FILENAME    : EthTSyn_Types.h                                             **
**                                                                            **
**  Created on  :                                                             **
**  Author      : yuzhe.zhang                                                 **
**  Vendor      :                                                             **
**  DESCRIPTION : Type header for EthTSyn                                     **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/

#ifndef ETHTSYN_TYPES_H
#define ETHTSYN_TYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "EthTSyn_Cfg.h"
#include "StbM_Types.h"
#include "EthIf.h"
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

typedef struct
{
    /* messageLength from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean EthTSynCrcMessageLength;
    /* domainNumber from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean EthTSynCrcDomainNumber;
    /* correctionField from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean EthTSynCrcCorrectionField;
    /* sourcePortIdentity from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean EthTSynCrcSourcePortIdentity;
    /* sequenceId from the Follow_Up Message Header shall be included in CRC calculation. */
    boolean EthTSynCrcSequenceId;
    /* preciseOriginTimestamp from the Follow_Up Message Field shall be included in CRC calculation. */
    boolean EthTSynCrcPreciseOriginTimestamp;
} EthTSynCrcTimeFlagsTxSecuredCfgType;

typedef EthTSynCrcTimeFlagsTxSecuredCfgType EthTSynCrcFlagsRxValidatedCfgType;

typedef struct
{
    /* Timeout value of the Follow_Up message (of the subsequent Sync message). */
    uint64 EthTSynGlobalTimeFollowUpTimeout;
    /* Definition of whether or not validation of the CRC takes place. */
    CrcValidatedType EthTSynRxCrcValidated;
    /* This container collects definitions which parts of the Follow_Up message elements shall be included in CRC
     * validation. */
    const EthTSynCrcFlagsRxValidatedCfgType* EthTSynCrcFlagsRxValidated;
    /* specifies the maximum allowed jump of the Sequence Counter between two consecutive Sync messages */
    uint16 EthTSynGlobalTimeSequenceCounterJumpWidth;
    /* specifies the number of consecutive valid message paris that are required by the Time Slave while being in
     * Timeout state until a Time Tuple is forwarded to the StbM. */
    uint8 EthTSynGlobalTimeSequenceCounterHysteresis;
} EthTSynGlobalTimeSlaveCfgType;

typedef struct
{
    /* This parameter allows disabling Pdelay_Resp / Pdelay_Resp_Follow_Up
    transmission, if no Pdelay_Req messages are expected. */
    boolean EthTSynGlobalTimePdelayRespEnable;
    sint8 logMessageInterval;
    /* If cyclic propagation delay measurement is enabled, this parameter
    represents the default value of the propagation delay until the first actually
    measured propagation delay is available. */
    uint64 EthTSynGlobalTimePropagationDelay;
    /* This represents configuration of the TX period for Pdelay_Req messages.
    A value of 0 disables the cyclic Pdelay measurement */
    uint64 EthTSynGlobalTimeTxPdelayReqPeriod;
    /* Threshold for calculated Pdelay.  */
    uint64 EthTSynPdelayLatencyThreshold;
    /* Timeout value for Pdelay_Resp and Pdelay_Resp_Follow_Up after a
    Pdelay_Req has been transmitted resp. a Pdelay_Resp has been received */
    uint64 EthTSynPdelayRespAndRespFollowUpTimeout;
} EthTSynPdelayConfigCfgType;

typedef struct
{
    /* Defines the time where the 1st regular cycle time based message
transmission takes place, after an immediate transmission before.  */
    uint64 EthTSynCyclicMsgResumeTime;
    /* This represents the configuration of whether or not CRC is supported. */
    CrcSecuredType EthTSynGlobalTimeTxCrcSecured;
    /* This represents configuration of the TX period. */
    uint64 EthTSynGlobalTimeTxPeriod;
    sint8 logMessageInterval;
    /* Enables/Disables the cyclic polling of StbM_GetTimeBaseUpdateCounter() within EthTSyn_MainFunction(). */
    boolean EthTSynImmediateTimeSync;
    /* This represents the configuration of whether an AUTOSAR Follow_Up TLV OFS Sub-TLV is used or not. */
    boolean EthTSynTLVFollowUpOFSSubTLV;
    /* This represents the configuration of whether an AUTOSAR Follow_Up TLV Status Sub-TLV is used or not. */
    boolean EthTSynTLVFollowUpStatusSubTLV;
    /* This represents the configuration of whether an AUTOSAR Follow_Up TLV Time Sub-TLV is used or not. */
    boolean EthTSynTLVFollowUpTimeSubTLV;
    /* This represents the configuration of whether an AUTOSAR Follow_Up TLV UserData Sub-TLV is used or not. */
    boolean EthTSynTLVFollowUpUserDataSubTLV;
    /* This container collects definitions which parts of the Follow_Up message elements shall be used for CRC
     * calculation */
    const EthTSynCrcTimeFlagsTxSecuredCfgType* EthTSynCrcTimeFlagsTxSecured;
    /* This represents the correctionField that is used when transmitting  Sync/Follow_Up Message by Master */
    uint64 EthTSynCorrectionField;
    /* This represents the CumulativeScaledRateOffset that is used when transmitting Messages by Master */
    uint32 EthTSynCumulativeScaledRateOffset;
} EthTSynGlobalTimeMasterCfgType;

typedef struct
{
    /* Configuration of the global time master. Each global time
    domain is required to have exactly one global time master. */
    const EthTSynGlobalTimeMasterCfgType* EthTSynGlobalTimeMaster;
    /* Configuration of a time slave. Each global time domain is
    required to have at least one time slave. */
    const EthTSynGlobalTimeSlaveCfgType* EthTSynGlobalTimeSlave;
} EthTSynPortRoleCfgType;

typedef struct
{
    /* This optional parameter, if present, indicates the priority of outgoing EthTSyn messages */
    uint8 EthTSynFramePrio;
    /* This represents the configuration of a TX debounce time for Sync and
    Follow_Up messages compared to a message before with the same PDU. */
    uint64 EthTSynGlobalTimeDebounceTime;
    /* This represents the reference to the Ethernet interface taken to fetch the
    global time information. */
    uint8 EthTSynGlobalTimeEthIfRef;
    /* Stbm if use EthFreerunning */
    boolean SynchronizedTimeBaseUseEthFreerunning;
    /* Configuration of cyclic propagation delay measurement. */
    EthTSynPdelayConfigCfgType EthTSynPdelayConfig;
    /* Specifying the Role of the EthTSyn-Port (Master or Slave). */
    EthTSynPortRoleCfgType EthTSynPortRole;
} EthTSynPortConfigCfgType;

typedef struct
{
    /* Index of the DataIDList for Follow_Up message ensures the identification
    of data elements due to CRC calculation and message authentication process. */
    uint8 EthTSynGlobalTimeFollowUpDataIDListIndex;
    /* Value of the DataIDList for Follow_Up message ensures the identification
    of data elements due to CRC calculation and message authentication process. */
    uint8 EthTSynGlobalTimeFollowUpDataIDListValue;
} EthTSynGlobalTimeFollowUpDataIDListElementCfgType;

typedef struct
{
    /* Element of the DataIDList for Follow_Up message ensures the identification of data
    elements due to CRC calculation and message authentication process. */
    EthTSynGlobalTimeFollowUpDataIDListElementCfgType EthTSynGlobalTimeFollowUpDataIDListElement[16U];
} EthTSynGlobalTimeFollowUpDataIDListCfgType;

typedef struct
{
    /* The global time domain ID. */
    uint8 EthTSynGlobalTimeDomainId;
    /* Mandatory reference to the required synchronized time-base. */
    uint8 EthTSynSynchronizedTimeBaseRef;
    /* The DataIDList for Follow_Up message ensures the identification of data elements due to CRC
calculation and message authentication process. */
    const EthTSynGlobalTimeFollowUpDataIDListCfgType* EthTSynGlobalTimeFollowUpDataIDList;
    /* The number of EthTSynPortConfig */
    uint8 EthTSynPortConfigNum;
    /* Configuration of the EthTSyn-Ports within the TimeDomain. */
    const EthTSynPortConfigCfgType* EthTSynPortConfig;
} EthTSynGlobalTimeDomainCfgType;

typedef struct
{
    const EthTSynGlobalTimeDomainCfgType* EthTSynGlobalTimeDomain;
} EthTSyn_ConfigType;

/*******************************************************************************
**                      Internal                                              **
*******************************************************************************/
typedef uint8 EthTSynMessageType;
#define SYNC                         (EthTSynMessageType)0x00u
#define PDELAY_REQ                   (EthTSynMessageType)0x02u
#define PDELAY_RESP                  (EthTSynMessageType)0x03u
#define FOLLOW_UP                    (EthTSynMessageType)0x08u
#define PDELAY_RESP_FOLLOW_UP        (EthTSynMessageType)0x0Au
#define MSG_NONE                     (EthTSynMessageType)0xFFu

#define ETHTSYN_PORT_IDENTITY_LENGTH 10u

typedef enum
{
    ETHTSYN_MASTER,
    ETHTSYN_SLAVE,
} EthTSyn_PortRoleType;
typedef enum
{
    SEND_PDELAY_REQ,
    WAITFOR_PDELAY_RESP,
    WAITFOR_PDELAY_RESP_FOLLOWUP,
} EthTSyn_PdelayTXStatusType;
typedef enum
{
    WAITFOR_PDELAY_REQ,
    SEND_PDELAY_RESP,
    SEND_PDELAY_RESP_FOLLOWUP,
} EthTSyn_PdelayRXStatusType;
typedef enum
{
    SEND_SYNC,
    SEND_FOLLOWUP,
    WAITFOR_SYNC,
    WAITFOR_FOLLOWUP,
} EthTSyn_SyncStatusType;

typedef enum
{
    ETHTSYN_TX_OFF,
    ETHTSYN_TX_ON
} EthTSyn_TransmissionModeType;

typedef struct
{
    uint32 nanoseconds;
    uint32 seconds;
    uint16 secondsHi;
} EthTSyn_TimeStampType;

typedef struct
{
    EthTSyn_TimeStampType pdelayReqEventIngressTimestamp;
    EthTSyn_TimeStampType pdelayRespEventEgressTimestamp;
} EthTSyn_PdelayResponderTimestampType;

typedef struct
{
    /* The T0 for tx preciseOriginTimestamp */
    EthTSyn_TimeStampType t0;
    /* The T0vlt for tx preciseOriginTimestamp */
    EthTSyn_TimeStampType t0vlt;
} EthTSyn_SyncEgressTupleType;

typedef struct
{
    uint8 index;
    /* ID of the time domain which this port belongs */
    uint8 timedomainid;
    /* The id of ETH control */
    uint8 CtrlId;
    /* Port ID in time domain */
    uint8 portIdinTimedomain;
    uint8 thisPortIdentity[ETHTSYN_PORT_IDENTITY_LENGTH];
    /* The sender port identity of the received sync message */
    uint8 portIdentity[ETHTSYN_PORT_IDENTITY_LENGTH];
    /* userData of stbm */
    StbM_UserDataType userData;
    /* Handles the enabling and disabling of the transmission mode */
    EthTSyn_TransmissionModeType EthTSyn_TransmissionMode;
    /* Link State */
    EthTrcv_LinkStateType EthtrcvLinkState;
    /* Role of port */
    EthTSyn_PortRoleType PortRoleType;
    /* The type of message to be transmitted */
    EthTSynMessageType TranssmitMessageType;
    /* Status of pdelay message sending */
    EthTSyn_PdelayTXStatusType PdelayTXStatusType;
    /* Status of pdelay message receiving */
    EthTSyn_PdelayRXStatusType PdelayRXStatusType;
    /* Status of SYNC message sending and receiving */
    EthTSyn_SyncStatusType SyncStatusType;
    /* TimeBaseUpdateCounter */
    uint8 TimeBaseUpdateCounter;
    /* Time of next sync message sent */
    uint64 SyncReqPeriod;
    /* Time of next Pdelay message sent */
    uint64 PdelayReqPeriod;
    /* DebounceCounter between SYNC and FOLLOW_UP */
    uint64 SyncdebounceCounter;
    /* DebounceCounter between PDELAY_RESP and PDELAY_RESP_FOLLOWUP */
    uint64 PdelaydebounceCounter;
    /* SequenceId of SYNC message */
    uint16 SyncSequenceId;
    /* SequenceId of received SYNC message */
    uint16 ReceivedSyncSequenceId;
    /* SequenceId of Pdelay message */
    uint16 PdelayReqSequenceId;
    /* SequenceId of received Pdelay message */
    uint16 ReceivedPdelayReqSequenceId;
    /* Whether received sync req or not */
    boolean hasReceivedSyncReq;
    /* The time for slave to sync the time */
    EthTSyn_TimeStampType preciseOriginTimestamp;
    /* The time for caculating precise timestamp */
    EthTSyn_SyncEgressTupleType SyncEgressTuple;
    /* The T1vlt for rx preciseOriginTimestamp */
    EthTSyn_TimeStampType T1vlt;
    /* The t1 for pdelay */
    EthTSyn_TimeStampType t1;
    /* The t2 for pdelay */
    EthTSyn_TimeStampType t2;
    /* The t3 for pdelay */
    EthTSyn_TimeStampType t3;
    /* The t4 for pdelay */
    EthTSyn_TimeStampType t4;
    EthTSyn_PdelayResponderTimestampType pdelayResponderTimestamp;
    /* Delay of sending between nodes */
    EthTSyn_TimeStampType pdelay;
    /* Priority */
    uint8 EthTSynFramePrio;
    /* The time for TX confirmation. */
    uint64 Txconfirmation;
    /* The time for TX confirmation. */
    uint8 EthTSynSCHysteresisCount;
} EthTSynPortType;

#define ETHTSYN_NANOSECOND_TO_SECOND 0x3B9ACA00u

#endif
