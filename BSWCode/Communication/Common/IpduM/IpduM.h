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
**************************************************************************** **
**                                                                           **
**  FILENAME    : IpduM.h                                                    **
**                                                                           **
**  Created on  :                                                            **
**  Author      : darren.zhang                                               **
**  Vendor      :                                                            **
**  DESCRIPTION : IpduM                                                      **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2                      **
**                                                                           **
**************************************************************************** */
#ifndef IPDUM_H_
#define IPDUM_H_
/******************************************************************************
**                      Includes                                              *
******************************************************************************/
#include "PduR_IpduM.h"
#include "ComStack_Types.h"
#include "IpduM_Cfg.h"
#include "SchM_IpduM.h"
/******************************************************************************
**                      Global Symbols                                       **
******************************************************************************/
/*Published information elements*/
#define IPDUM_H_AR_MAJOR_VERSION 4u
#define IPDUM_H_AR_MINOR_VERSION 5u
#define IPDUM_H_AR_PATCH_VERSION 0u

/* Development Errors */
#define IPDUM_E_PARAM         (0x10u)
#define IPDUM_E_PARAM_POINTER (0x11u)
#define IPDUM_E_UNINIT        (0x20u)
#define IPDUM_E_INIT_FAILED   (0x21u)
/* Runtime Errors */
#define IPDUM_E_HEADER    (0x30u)
#define IPDUM_E_QUEUEOVFL (0x31u)

/* Service ID[hex] */
#define IPDUM_SERVICE_ID_INIT            (0x00u)
#define IPDUM_SERVICE_ID_GETVERSIONINFO  (0x01u)
#define IPDUM_SERVICE_ID_TRANSMIT        (0x03u)
#define IPDUM_SERVICE_ID_RXINDICATION    (0x42u)
#define IPDUM_SERVICE_ID_TXCONFIRMATION  (0x40u)
#define IPDUM_SERVICE_ID_TRIGGERTRANSMIT (0x41u)
#define IPDUM_SERVICE_ID_MAINFUNCTIONRX  (0x11u)
#define IPDUM_SERVICE_ID_MAINFUNCTIONTX  (0x12u)

/* ipdum rreference pdu data typedef */
typedef struct
{
    PduIdType PduId;
    uint16 PduLen;
} PduRefType;

/* multiplexer ipdu part include segments number typedef */
typedef uint16 SegmentCntType;

/* multiplexer  ipdu include part number typedef */
typedef uint16 IpduM_PartCntType;

/* mainfunction type */
typedef uint8 IpduM_MainFunctionType;

/* Post-build configuration parameter type definitions */

typedef enum
{
    IPDUM_UNINIT = 0u,
    IPDUM_INIT
} IpduM_StatusType;

/* This parameter defines the ByteOrder for all segments
and for the selectorField within the MultiplexedPdu. */
typedef enum
{
    IPDUM_BYTE_ORDER_BIG_ENDIAN = 0u,
    IPDUM_BYTE_ORDER_LITTLE_ENDIAN
} IpduM_ByteOrderType;

typedef enum
{
    /*  Writing the I-PDU representing the dynamic part does trigger a
    sending of the I-PDU */
    IPDUM_DYNAMIC_PART_TRIGGER = 0u,
    /* Only the buffer in the IpduM are written but not send is triggered,
    used for IpduM I-PDUs which are requested by TriggerTransmit. */
    IPDUM_TRIGGER_MODE_NONE,
    /* Writing the I-PDU representing the static or the dynamic part
    does trigger a sending of the I-PDU. */
    IPDUM_STATIC_OR_DYNAMIC_PART_TRIGGER,
    /*  Writing the I-PDU representing the static part does trigger a
    sending of the I-PDU. */
    IPDUM_STATIC_PART_TRIGGER
} IpduM_TriggerModeType;

typedef enum
{
    /* Header size is 64 bit: * Header Id 32 bit * Dlc 32 bit */
    IPDUM_HEADERTYPE_LONG = 0u,
    /* Header size is 32 bit: * Header Id 24 bit * Dlc 8 bit */
    IPDUM_HEADERTYPE_SHORT
} IpduM_HeaderType;

typedef enum
{
    /* Container PDU shall be queued */
    IPDUM_PROCESSING_DEFERRED = 0u,
    /* received Container PDUs shall be executed in the context of
    IpduM_RxIndication */
    IPDUM_PROCESSING_IMMEDIATE
} IpduM_ProcessingType;

typedef enum
{
    /* IpduM shall expect and match all contained I-PDUs
    independent of IpduMContainedRxInContainerPduRef */
    IPDUM_ACCEPT_ALL = 0u,
    /* IpduM shall expect and match only contained I-PDUs that reference
    the Container PDU in IpduMContainedRxInContainerPduRef */
    IPDUM_ACCEPT_CONFIGURED
} IpduM_AcceptType;

typedef enum
{
    /* The IpduM sends this ContainerPdu when this ContainerPdu is triggered */
    IPDUM_DIRECT = 0u,
    /* This ContainerPdu is stored in the IpduM and fetched via
    trigger transmit. */
    IPDUM_TRIGGERTRANSMIT
} IpduM_TrigModeType;

typedef enum
{
    /* The IpduMContainedTxPdu data will be fetched via TriggerTransmit just
    before the transmission executes. */
    IPDUM_COLLECT_LAST_IS_BEST = 0u,
    /* The IpduMContainedTxPdu data will be fetched via TriggerTransmit just
    before the transmission executes. */
    IPDUM_COLLECT_QUEUED
} IpduM_CollectType;

typedef enum
{
    /* This Pdu directly triggers the sending of the ContainerPdu. */
    IPDUM_TRIGGER_ALWAYS = 0u,
    /* This Pdu does not triggers the sending of the ContainerPdu. */
    IPDUM_TRIGGER_NEVER
} IpduM_TriggerType;

/* This contains the location and the length of the selector field */
typedef struct
{
    /* Length of the selector field in bits. */
    uint8 IpduMSelectorFieldLength;
    /* Selector field bit position in the multiplexed Pdu. */
    uint16 IpduMSelectorFieldPosition;
} IpduMSelectorFieldType;

/* common segment type */
typedef struct
{
    /* Length of the segment in bits. */
    uint16 IpduMSegmentLength;
    /* Segments bit position in the multiplexed Pdu. */
    uint16 IpduMSegmentPosition;
} IpduM_SegmentType;

/* common tx part type */
typedef struct
{
    /* If configured to true fetch the data of this part Just-In-Time
        via the triggerTransmit API of the PduR. */
    boolean IpduMJitUpdate;
    /* A transmit request can be confirmed by the lower layer*/
    boolean IpduMTxConfirmation;
    /* Reference to the Pdu representation in the ECU Configuration Description
    exchange file to be transmitted. */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMTxPduRef;
    /* The part of the multiplexed outgoing I-Pdu can be separated into
    several segments */
    P2CONST(IpduM_SegmentType, AUTOMATIC, IPDUM_APPL_CONST) IpduMTxSegmentPtr;
    /* segments count of IpduMTxSegmentPtr point data */
    SegmentCntType IpduMTxSegmentCnt;
} IpduM_TxPartType;

/* This container is used to specify the configuration for Transmit requests */
typedef struct
{
    /* This parameter defines the ByteOrder for all segments and for the
    selectorField within the MultiplexedPdu. */
    IpduM_ByteOrderType IpduMByteOrder;
    /* IpduM module fills not used areas of an I-PDU with this bit-pattern */
    uint8 IpduMIPduUnusedAreasDefault;
    /* TxRequest in buffer index */
    uint16 IpduMBufIndex;
    /* This timeout defines the timeout period for monitoring the reception of
    the TxConfirmation */
    uint16 IpduMTxConfirmationTimeout;
    /* Selects whether to send the multiplexed I-PDU immediately or
    at some later date */
    IpduM_TriggerModeType IpduMTxTriggerMode;
    /* Reference to the dynamic part that shall be used to initialize this
    multiple-xed TX-I-PDU */
    uint8 IpduMInitialDynamicPart;
    /* Reference to the PDU defining the outgoing I-PDU */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMOutgoingPduRef;
    /* number of the tx static part */
    IpduM_PartCntType IpduMTxStaticPartCnt;
    /* number of the tx dynamic part */
    IpduM_PartCntType IpduMTxDynamicPartCnt;
    /* tx dynamic part point */
    P2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) IpduMTxDynamicPartPtr;
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    /* tx static part point */
    P2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) IpduMTxStaticPartPtr;
#endif /* STD_ON == IPDUM_STATIC_PART_EXISTS */
#if (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
    uint16 IpduMTxRequestPartition;
#endif
} IpduMTxRequestType;

/* This container contains the configuration for the dynamic part of incoming
RxIndication calls. */
typedef struct
{
    /* This is the selector value that this container refers to. */
    uint16 IpduMRxSelectorValue;
    /* When the new I-PDU is sent out it is sent with this I-PDU ID.  */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMOutgoingDynamicPduRef;
    /* number of dynamic part of the several segments */
    SegmentCntType IpduMRxDynamicSegmentCnt;
    /* The dynamic part of the multiplexed incoming I-Pdu
    can be separated into several segments. */
    P2CONST(IpduM_SegmentType, AUTOMATIC, IPDUM_APPL_CONST) IpduMRxDynamicSegmentPtr;
} IpduMRxDynamicPartType;

/* This container contains the configuration for the static part of incoming
RxIndication calls. */
typedef struct
{
    /* When the new I-PDU is sent out it is sent with this I-PDU ID.  */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMOutgoingStaticPduRef;
    /* number of static part of the several segments */
    SegmentCntType IpduMRxStaticSegmentCnt;
    /* The static part of the multiplexed incoming I-Pdu
    can be separated into several segments. */
    P2CONST(IpduM_SegmentType, AUTOMATIC, IPDUM_APPL_CONST) IpduMRxStaticSegmentPtr;
} IpduMRxStaticPartType;

/* Contains the configuration for incoming RxIndication calls. */
typedef struct
{
    /* This parameter defines the ByteOrder for all segments and for the
    selectorField within the MultiplexedPdu. */
    IpduM_ByteOrderType IpduMByteOrder;
    /* This is the I-PDU ID of the incoming I-PDU. */
    uint16 IpduMRxHandleId;
    /* rx indication in buffer index */
    uint16 IpduMBufIndex;
    /* Reference to the received Pdu representation in the ECU Configuration
    Description exchange file. */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMRxIndicationPduRef;
    /* number of rx dynamic part */
    IpduM_PartCntType IpduMRxDynamicPartCnt;
    /* number of rx static part */
    IpduM_PartCntType IpduMRxStaticPartCnt;
    /* Each of these containers contains the configuration for one
    value of the selector field for the incoming I-PDU's dynamic part. */
    P2CONST(IpduMRxDynamicPartType, AUTOMATIC, IPDUM_APPL_CONST) IpduMRxDynamicPartPtr;
    /* The static part of the multiplexed incoming I-Pdu
    can be separated into several segments. */
    P2CONST(IpduMRxStaticPartType, AUTOMATIC, IPDUM_APPL_CONST) IpduMRxStaticPartPtr;
    /* This contains the location and the length of the selector field. */
    IpduMSelectorFieldType IpduMSelectorField;
} IpduMRxIndicationType;

/* Configuration of a receiver ContainerPdu which may collect several
ContainedPdus. */
typedef struct
{
    /* container in buffer index */
    uint16 IpduMBufIndex;
    /* container start position in inner data global queue */
    uint16 IpduMContainerRxQueueStartPosIndex;
    /* Defines the layout of the header information (header id and length). */
    IpduM_HeaderType IpduMContainerHeaderSize;
    /* Defines whether the handling of this ContainerPdu shall be done in the
    context of the caller (IMMEDIATE) or
    in the next IpduM main function (DEFERRED). */
    IpduM_ProcessingType IpduMContainerPduProcessing;
    /* Defines a local queue for handling of each ContainerPdu. */
    uint8 IpduMContainerQueueSize;
    /* Defines for the received IpduMRxContainerPdu whether the list of
    referencing IpduMRxContainedPdus is a closed set. */
    IpduM_AcceptType IpduMContainerRxAcceptContainedPdu;
    /* Handle Id used by the PduR for RxIndication. */
    uint16 IpduMContainerRxHandleId;
    /* Reference to the received Pdu representation in the ECU Configuration
    Description exchange file. */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainerRxPduRef;
} IpduMContainerRxPduType;

/* Configuration of a received contained Pdu */
typedef struct
{
    /* Header Id which is part of the ContainerPdu when this ContainedPdu is
    inside. */
    uint32 IpduMContainedPduHeaderId;
    /* Optional reference to a container Pdu this contained Pdu may be
    transported in. */
    uint8 IpduMContainedRxInContainerPduRef;
    /* Reference to the Pdu which represents this ContainedPdu and is used for
    reception indication. */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainedRxPduRef;
} IpduMContainedRxPduType;

/* Configuration of a transmitted container Pdu. */
typedef struct
{
    /* container tx in buffer index */
    uint16 IpduMBufIndex;
    /* container start position in inner data global queue */
    uint16 IpduMContainerTxQueueStartPosIndex;
    /* contained position in global index arry */
    /*IPDUM_SUPPORT_CONTAINED_NUM_MAX start index*/
    uint16 IpduMContainerIncContainedStartPosIndex;
    /* maxium number of contained in same container all queue,
    this value only be observe */
    uint16 IpduMContainerIncContainedMaxNum;
    /*document defined Configuration parameters*/
    /* Defines the layout of the header information (header id and length). */
    IpduM_HeaderType IpduMContainerHeaderSize;
    /* Defines a local queue for handling of each ContainerPdu. */
    uint8 IpduMContainerQueueSize;
    /* Defines whether this ContainerPdu is fetched via trigger transmit. */
    IpduM_TrigModeType IpduMContainerTxTriggerMode;
    /* Defines if the transmission of this IpduMContainerTxPdu
        shall be requested right after the first
        IpduMContainedTxPdu was put into it. */
    boolean IpduMContainerTxFirstContainedPduTrigger;
    /* Handle Id used by the PduR for TxConfirmation and for TriggerTransmit of
    the ContainerPdu. */
    uint16 IpduMContainerTxHandleId;
    /* When this timeout expires the ContainerPdu is triggered for sending. */
    P2CONST(uint16, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainerTxSendTimeout;
    /* This timeout defines the timeout period for monitoring the
    reception of the TxConfirmation. */
    P2CONST(uint16, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainerTxConfirmationTimeout;
    /* Defines the size threshold in bytes which, when exceeded, triggers the
    sending of the ContainerPdu although the maxium Pdu size has not
    been reached yet. */
    P2CONST(uint32, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainerTxSizeThreshold;
    /* Reference to the Pdu which represents the container and is used for
    transmission. */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainerTxPduRef;
} IpduMContainerTxPduType;

/* Configuration of a sender ContainedPdu. */
typedef struct
{
    /* Header Id which is part of the ContainerPdu when this ContainedPdu is
    inside. */
    uint32 IpduMContainedPduHeaderId;
    /* Defines whether this IpduMContainedTxPdu shall be collected using a
    last-is-best or queued semantics. */
    IpduM_CollectType IpduMContainedTxPduCollectionSemantics;
    /* This Parameter determines whether for this contained I-PDU a
    TxConfirmation shall be provided. */
    boolean IpduMContainedTxPduConfirmation;
    /* Handle Id of the ContainedPdu. */
    uint16 IpduMContainedTxPduHandleId;
    /*  Defines a ContainedPdu specific sender timeout which can reduce the
    ContainerPdu timer when this ContainedPdu is put inside the ContainerPdu. */
    P2CONST(uint16, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainedTxPduSendTimeout;
    /* Defines whether this Pdu triggers the sending of the ContainerPdu */
    IpduM_TriggerType IpduMContainedTxPduTrigge;
    /* Reference to the container Pdu which this contained Pdu shall be
    collected in. */
    uint8 ContainedTxInContainerPduRef;
    /* Reference to the Pdu which represents this ContainedPdu and is used for
    transmission. */
    P2CONST(PduRefType, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainedTxPduRef;
} IpduMContainedTxPduType;
/****link time configuration parameter type definitions**********/

/* call IpduM_Transmit support multiplexed static or dynamic part and
contained I-pdu */
typedef enum
{
    IPDUM_TRANSMIT_TYPE_STATICPART = 0u,
    IPDUM_TRANSMIT_TPYE_DYNAMIC,
    IPDUM_TRANSMIT_TYPE_CONTAINED
} IpduM_TxPduType;

/* callback api IpduM_RxIndication support
received multiplexed and container typde i-PDU */
typedef enum
{
    IPDUM_RXIND_TYPE_MULT = 0u,
    IPDUM_RXIND_TYPE_CONTAINER
} IpduM_RxPduType;

/* callback api IpduM_TxConfirmation and IpduM_TriggerTransmit
support received multiplexed and container type i-pdu */
typedef enum
{
    IPDUM_TXCONF_TYPE_MULT = 0u,
    IPDUM_TXCONF_TYPE_CONTAINER
} IpduM_TxConfPduType;

/* Ipdum transmit pdu pathway type */
typedef struct
{
    /*api IpduM_Transmit support i-pdu types */
    IpduM_TxPduType TxReqType;
    /* TxReqType equal multiplexed type,this value is tx request index
     and TxReqType equal contained type,this value is tx contained index */
    uint8 Index;
    /* this value only valide when TxReqType equal multiplexed */
    uint8 handleId;
} IpduM_UpTxPduType;

/* ipdum received ipdu pathway type */
typedef struct
{
    /* api IpduM_RxIndication support i-pdu types */
    IpduM_RxPduType RxIndType;
    /* seach index */
    uint8 Index;
#if (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
    uint16 IpduMRxPduPartition;
#endif
} IpduM_LoRxPduType;

/* ipdum received tx confirmation pathway type */
typedef struct
{
    /* api IpduM_TxConfirmation or IpduM_TriggerTransmit
    support i-pdu types */
    IpduM_TxConfPduType txConfPduType;
    /* seach index */
    uint8 Index;
} IpduM_LoTxPduType;

/* ipdum module configure data type */
typedef struct
{
    /* Configuration of a received contained Pdu */
    P2CONST(IpduMContainedRxPduType, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainedRxPduPtr;
    /* Configuration of a sender ContainedPdu. */
    P2CONST(IpduMContainedTxPduType, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainedTxPduPtr;
    /* Configuration of a receiver ContainerPdu which may collect
    several ContainedPdus. */
    P2CONST(IpduMContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainerRxPduPtr;
    /* Configuration of a transmitted container Pdu. */
    P2CONST(IpduMContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) IpduMContainerTxPduPtr;
    /* includes information about received I-PDUs */
    P2CONST(IpduM_LoRxPduType, AUTOMATIC, IPDUM_APPL_CONST) IpduMRxPduPtr;
    /* includes information about sent I-PDUs */
    P2CONST(IpduM_UpTxPduType, AUTOMATIC, IPDUM_APPL_CONST) IpduMUpTxPduPtr;
    /* includes information about TxConfirmation I-PDUs */
    P2CONST(IpduM_LoTxPduType, AUTOMATIC, IPDUM_APPL_CONST) IpduMTxPduPtr;
    /* includes information about all tx request multiplexed I-PDUs */
    P2CONST(IpduMTxRequestType, AUTOMATIC, IPDUM_APPL_CONST) IpduMTxRequestPtr;
    /* includes information about all rx indication multiplexed I-PDUs */
    P2CONST(IpduMRxIndicationType, AUTOMATIC, IPDUM_APPL_CONST) IpduMRxIndicationPtr;
    const uint16* IpduMMainFunctionRxRange;
    const uint16* IpduMMainFunctionTxRange;
#if (STD_ON == IPDUM_MULTIPLE_PARTITION_USED)
    const uint16* IpduMMainFuncRxPartitionRange;
    const uint16* IpduMMainFuncTxPartitionRange;
#endif
} IpduM_ConfigType;
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/**********Link time configuration parameter declarations*************/

/**********Post-build configuration parameter declarations*************/
#if (IPDUM_MAX_BUFFER_SIZE > 0u)
/* inner buffer shall be init to this value */

extern CONST(uint8, IPDUM_CONST) IpduMDefaultBufVal[IPDUM_MAX_BUFFER_SIZE];

#endif /* IPDUM_MAX_BUFFER_SIZE>0u */

extern CONST(IpduM_ConfigType, IPDUM_CONST) IpduM_PBConfigData;

/***************************Public API Declarations****************************/

/**
 * Initializes the I-PDU Multiplexer.
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters(IN): config, Implementation specific
    structure with configuration parameters.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 * SWS_IpduM_00033,SWS_IpduM_00084
 */
extern FUNC(void, IPDUM_CODE) IpduM_Init(P2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_APPL_DATA) configPtr);

#if (STD_ON == IPDUM_VERSION_INFO_API)
/**
 * Service returns the version information of this module.
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters(IN): versioninfo, Pointer to where to store the version
    information of this module.
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: NA
 */
/* PRQA S 3432 ++ */ /* MISRA Rule 20.7 */
extern FUNC(void, IPDUM_CODE) IpduM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, IPDUM_APPL_DATA) versioninfo);
/* PRQA S 3432 -- */ /* MISRA Rule 20.7 */
#endif               /* STD_ON == IPDUM_VERSION_INFO_API */
/**
 * Performs the processes of the activities that are not directly
    initiated by the calls from PDU-R.
 * Service ID: 0x11
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different instances. Non reentrant for the same instance.
 * Parameters(IN): TNA
 * Parameters(INOUT):  NA
 * Parameters(OUT): NA
 * Return value: NA
 *  @SWS_IpduM_91001
 */
extern FUNC(void, IPDUM_CODE) IpduM_MainFunctionRx(IpduM_MainFunctionType mainFunctionId);
/**
 * Performs the processes of the activities that are not directly
    initiated by the calls from PDU-R.
 * Service ID: 0x12
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant for different instances. Non reentrant for the same instance.
 * Parameters(IN): TNA
 * Parameters(INOUT):  NA
 * Parameters(OUT): NA
 * Return value: NA
 *  @SWS_IpduM_91002
 */
extern FUNC(void, IPDUM_CODE) IpduM_MainFunctionTx(IpduM_MainFunctionType mainFunctionId);
/**
 * Service is called by the PDU-Router to request a transmission.
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant for the same PDU-ID. Reentrant for
    different PDU-ID.
 * Parameters(IN): PdumTxPduId, ID of I-PDU to be transmitted
                    PduInfoPtr,A pointer to a structure with I-PDU related
    data that shall be transmitted: data length and pointer to I-SDU buffer
 * Parameters(INOUT): NA
 * Parameters(OUT): NA
 * Return value: Std_ReturnType  E_OK: Transmit request is accepted
        E_NOT_OK: Transmit request is not accepted
 */
extern FUNC(Std_ReturnType, IPDUM_CODE)
    IpduM_Transmit(PduIdType PdumTxPduId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

#endif /* IPDUM_H_ */
