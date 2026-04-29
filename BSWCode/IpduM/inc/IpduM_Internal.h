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
 **  @file               : IpduM.h
 **  @author             : darren.zhang
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : I-PDU Multiplexer
 **
 ***********************************************************************************************************************/

/* PRQA S 1536 EOF */ /* VL_IpduM_1536 */

#ifndef IPDUM_INTERNAL_H_
#define IPDUM_INTERNAL_H_

/* =================================================== inclusions =================================================== */

#include "IpduM_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */

#define IPDUM_MODULE_ID 182u
#define IPDUM_VENDOR_ID 62u

/* ===================================================== macros ===================================================== */

#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
#define IPDUM_INSTANCE_ID ((uint8)GetApplicationID())
#else
#define IPDUM_INSTANCE_ID 0u
#endif

#define IPDUM_UNUSED_PARTITION_ID (0xFFu)

#define IPDUM_UNUSED_CONTAINER_SIZE_THRESHOLD (0xFFFFu)
#define IPDUM_UNUSED_MULTIPLEXE_PART_INDEX    (0xFFFFu)

#define IPDUM_CONTAINER_CFG_HEADER_SIZE_NONE  (0u)
#define IPDUM_CONTAINER_CFG_HEADER_SIZE_SHORT (4u)
#define IPDUM_CONTAINER_CFG_HEADER_SIZE_LONG  (8u)

#define IPDUM_CONTAINERTX_INSTANCE_NO_TRIGGERED_NO_QUEUED (0u)
#define IPDUM_CONTAINERTX_INSTANCE_LOCKED                 (1u)
#define IPDUM_CONTAINERTX_INSTANCE_TRIGGERED              (2u)
#define IPDUM_CONTAINERTX_INSTANCE_QUEUED                 (4u)
#define IPDUM_CONTAINERTX_INSTANCE_FULL_OCCUPIED          (0x10u)
#define IPDUM_CONTAINERTX_INSTANCE_LOWERLAYER_ACCEPTED    (0x80u)
#define IPDUM_CONTAINERTX_INSTANCE_CLEAR_EXCEP_LOCKED     (1u)

#define IPDUM_CONTAINERRX_INSTANCE_LOCKED            (1u)
#define IPDUM_CONTAINERRX_INSTANCE_CONTAIN_CONTAINED (2u)

/* denote trigger or send Part type ofmultiplexed Pdu */
#define IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_STATIC  (0u)
#define IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_DYNAMIC (1u)
#define IPDUM_TRIGGER_OR_SEND_TYPE_OF_MULTIPLEX_TRIGGER (2u) /* Triggered from IpduM_TriggerTransmit call */

#define IPDUM_REINIT_CHECK STD_ON

#define IPDUM_NOUSED(paramter) (void)(paramter)

/* ================================================ type definitions ================================================ */

/* Post-build configuration parameter type definitions */

typedef enum
{
    /*  Writing the I-PDU representing the dynamic part does trigger a
    sending of the I-PDU */
    IPDUM_DYNAMIC_PART_TRIGGER,
    /* Only the buffer in the IpduM are written but not send is triggered,
    used for IpduM I-PDUs which are requested by TriggerTransmit. */
    IPDUM_NONE,
    /* Writing the I-PDU representing the static or the dynamic part
    does trigger a sending of the I-PDU. */
    IPDUM_STATIC_OR_DYNAMIC_PART_TRIGGER,
    /*  Writing the I-PDU representing the static part does trigger a
    sending of the I-PDU. */
    IPDUM_STATIC_PART_TRIGGER
} IpduM_MultiplexTriggerModeType;

typedef enum
{
    /* Container PDU shall be queued */
    IPDUM_PROCESSING_DEFERRED,
    /* received Container PDUs shall be executed in the context of
    IpduM_RxIndication */
    IPDUM_PROCESSING_IMMEDIATE
} IpduM_ProcessingType;

typedef enum
{
    /* IpduM shall expect and match all contained I-PDUs
    independent of IpduMContainedRxInContainerPduRef */
    IPDUM_ACCEPT_ALL,
    /* IpduM shall expect and match only contained I-PDUs that reference
    the Container PDU in IpduMContainedRxInContainerPduRef */
    IPDUM_ACCEPT_CONFIGURED
} IpduM_AcceptType;

typedef enum
{
    /* The IpduM sends this ContainerPdu when this ContainerPdu is triggered */
    IPDUM_DIRECT,
    /* This ContainerPdu is stored in the IpduM and fetched via
    trigger transmit. */
    IPDUM_TRIGGERTRANSMIT
} IpduM_ContainerTxPduTriggerModeType;

typedef enum
{
    /* The IpduMContainedTxPdu data will be fetched via TriggerTransmit just
    before the transmission executes. */
    IPDUM_COLLECT_LAST_IS_BEST,
    /* The IpduMContainedTxPdu data will be fetched via TriggerTransmit just
    before the transmission executes. */
    IPDUM_COLLECT_QUEUED
} IpduM_CollectType;

typedef enum
{
    /* This contained Pdu directly triggers the sending of the ContainerPdu. */
    IPDUM_TRIGGER_ALWAYS,
    /* This contained Pdu does not triggers the sending of the ContainerPdu. */
    IPDUM_TRIGGER_NEVER
} IpduM_ContainedTxPduTriggerType;

/**
 * @brief  used to specify the configuration for TxStaticPart and TxDynamicPart.
 */
typedef struct IpduM_TxPartTag
{
    /* static part or dynamic part Pdu ID used in PduR_IpduMTxConfirmation or PduR_IpduMTriggerTransmit*/
    PduIdType PduId;
    /* pdu length */
    PduLengthType PduLen;
    /* indox of TxRequestPdu that containe this Part Pdu */
    PduIdType TxRequestRefIndex;
    /* If configured to true fetch the data of this part Just-In-Time
        via the triggerTransmit API of the PduR. */
    boolean IpduMJitUpdate;
    /* A transmit request can be confirmed by the lower layer*/
    boolean IpduMTxConfirmation;
} IpduM_TxPartType;

/**
 * @brief  used to specify the configuration for TxRequest.
 */
typedef struct IpduM_TxRequestTag
{
    /* segments configuration of dynamic part*/
    uint16 IpduMTxDynamicSegmenStartIndex;
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    /* segments configuration of static part*/
    uint16 IpduMTxStaticSegmentStartIndex;
    /* tx static part index */
    const uint16 IpduMTxStaticPartIndex;
    /* number of segments in static part */
    uint16 TxStaticSegmentCnt;
#endif
    /* TxRequst pdu length */
    PduLengthType PduLen;
    /* TxRequst Pdu ID used in PduR_IpduMTransmit */
    PduIdType PduId;
    /* number of segments in dynamic part*/
    uint16 TxDynamicSegmentCnt;
    /* TxRequest in buffer index */
    uint16 BufIndex;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
    /* position to store MetaData */
    uint16 MetaDataBufIndex;
    /* size of MetaData */
    uint8 MetaDataSize;
#endif
    /* This parameter defines the ByteOrder for all segments and for the
    selectorField within the MultiplexedPdu. */
    IpduM_ByteOrderType IpduMByteOrder;
    /* Selects whether to send the multiplexed I-PDU immediately or
    at some later date */
    IpduM_MultiplexTriggerModeType IpduMTxTriggerMode;
    /* Reference to the dynamic part that shall be used to initialize this
    multiple-xed TX-I-PDU */
    PduIdType IndexOfIpduMInitialDynamicPart;

#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType IpduMTxRequestPartition;
#endif
} IpduM_TxRequestCfgType;

/**
 * @brief  This container contains the configuration for the dynamic part of incoming RxIndication calls.
 */
typedef struct IpduM_RxDynamicPartTag
{
    /* This is the selector value that this container refers to. */
    uint16 IpduMRxSelectorValue;
    /* RxDynamicPart pdu length */
    PduLengthType PduLen;
    /* RxDynamicPart Pdu ID used in PduR_IpduMRxIndication */
    PduIdType PduId;

} IpduM_RxDynamicPartType;

/**
 * @brief  This container contains the configuration for the static part of incoming RxIndication calls.
 */
typedef struct IpduM_RxStaticPartTag
{
    /* RxStaticPart pdu length */
    PduLengthType PduLen;
    /* RxStaticPart Pdu ID used in PduR_IpduMRxIndication */
    PduIdType PduId;
} IpduM_RxStaticPartType;

/**
 * @brief  Contains the configuration for incoming RxIndication calls.
 */
typedef struct IpduM_RxIndicationTag
{
    /* The dynamic part of the multiplexed incoming I-Pdu
    can be separated into several segments. */
    uint16 IpduMRxDynamicSegmentStartIndex;
#if (STD_ON == IPDUM_STATIC_PART_EXISTS)
    /* The static part of the multiplexed incoming I-Pdu
    can be separated into several segments. */
    uint16 IpduMRxStaticSegmentStartIndex;
    /* index of static part  */
    PduLengthType RxStaticPartIndex;
    /* number of static part of the several segments */
    uint16 IpduMRxStaticSegmentCnt;
#endif
    /* start index of all dynamic parts belong to this RxIndication */
    uint16 RxDynamicPartStartIndex;
    /* RxIndication pdu length */
    PduLengthType PduLen;
    /* number of rx dynamic part */
    uint16 IpduMRxDynamicPartCnt;
    /* number of dynamic part of the several segments */
    uint16 IpduMRxDynamicSegmentCnt;
    /* Selector field bit position in the multiplexed Pdu. */
    uint16 IpduMSelectorFieldPosition;
    /* This parameter defines the ByteOrder for all segments and for the
    selectorField within the MultiplexedPdu. */
    IpduM_ByteOrderType IpduMByteOrder;
    /* Length of the selector field in bits. */
    uint8 IpduMSelectorFieldLength;
} IpduM_RxIndicationCfgType;

/**
 * @brief  Configuration of a sender ContainedPdu.
 */
typedef struct IpduM_ContainedTxPduCfgTag
{
    /* Header Id which is part of the ContainerPdu when this ContainedPdu is
    inside. */
    uint32 IpduMContainedPduHeaderId;
    /*  Defines position of ContainedPdu in the ContainerPdu. */
    const uint16 IpduMContainedPduOffset;
    /* Defines a ContainedPdu specific sender timeout which can reduce the
    ContainerPdu timer when this ContainedPdu is put inside the ContainerPdu. */
    const uint16 IpduMContainedTxPduSendTimeout;
    /*byte postion of updated bit*/
    PduLengthType UpdatedBitBytePos;
    /* pdu length */
    PduLengthType PduLen;
    /* pdu Id using in PduR_IpduMTxConfirmation */
    PduIdType PduId;
    /*mask value to set updated bit in its byte*/
    uint8 UpdateBitByteMask;
    /* Defines whether this IpduMContainedTxPdu shall be collected using a
    last-is-best or queued semantics. */
    IpduM_CollectType IpduMContainedTxPduCollectionSemantics;
    /* This Parameter determines whether for this contained I-PDU a
    TxConfirmation shall be provided. */
    boolean IpduMContainedTxPduConfirmation;
    /* Defines whether this Pdu triggers the sending of the ContainerPdu */
    IpduM_ContainedTxPduTriggerType IpduMContainedTxPduTrigger;
    /* Reference to the container Pdu which this contained Pdu shall be
    collected in. */
    PduIdType ContainerIndexRefByContained;
    /* Defines a priority of a containedTxPdu.255 represents the lowest priority
       0 represent the highest priority.*/
    uint8 IpduMContainedTxPduPirority;
} IpduM_ContainedTxPduCfgType;

/**
 * @brief  Configuration of a transmitted container Pdu.
 */
typedef struct IpduM_ContainerTxPduCfgTag
{
    /* Defines the size threshold in bytes which, when exceeded, triggers the
    sending of the ContainerPdu although the maxium Pdu size has not
    been reached yet. */
    const uint16 IpduMcontainerTxSizeThreshold;
    /* container tx send timeout */
    uint16 IpduMContainerTxSendTimeout;
    /* container tx in buffer index */
    uint16 BufIndex;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
    /* position to store MetaData */
    uint16 MetaDataBufIndex;
    /* size of MetaData */
    uint8 MetaDataSize;
#endif
    /* container start position in inner data global queue */
    PduLengthType ContainerQueueStartPos;
    /* contained position in global index arry */
    /*IPDUM_SUPPORT_CONTAINED_NUM_MAX start index*/
    PduLengthType ContainedIndexQueueStartPos;
    /* Defines size of ContainedIndex Queue whitch to store containedIndex of all instances of this container
    ,to implement a better support for dynamic containedTxPdu length*/
    uint16 ContainedIndexQueueSize;
    /* pdu length */
    PduLengthType PduLen;
    /* pdu Id using in PduR_IpduMTransmit */
    PduIdType PduId;
    /* number of contained in the static container, 0 value for dynamic container*/
    PduIdType StaticContainerIncContainedNum;
    /*document defined Configuration parameters*/
    /* Defines byte size of the header added before containedTxPdu . */
    uint8 IpduMContainerHeaderSize;
    /* Defines a local queue for handling of each ContainerPdu. */
    uint8 IpduMContainerQueueSize;
    /* Defines whether this ContainerPdu is fetched via trigger transmit. */
    IpduM_ContainerTxPduTriggerModeType IpduMContainerTxTriggerMode;
    /* Defines if the transmission of this IpduMContainerTxPdu
        shall be requested right after the first
        IpduMContainedTxPdu was put into it. */
    boolean IpduMContainerTxFirstContainedPduTrigger;
    /*Unused areas default value */
    uint8 IpduMUnusedAreasDefault;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType IpduMContainerTxPartition;
#endif
} IpduM_ContainerTxPduCfgType;

/**
 * @brief  Configuration of a received contained Pdu.
 */
typedef struct IpduM_ContainedRxPduCfgTag
{
    /* Defines position of ContainedPdu in the ContainerRxPdu. */
    uint16 IpduMContainedRxPduOffset;
    /* Long Header Id which is part of the ContainerPdu when this ContainedPdu is
    put inside. */
    uint32 IpduMContainedPduLongHeaderId;
    /* Short Header Id which is part of the ContainerPdu when this ContainedPdu is
    put inside. */
    uint32 IpduMContainedPduShortHeaderId;
    /* byte postion of updated bit */
    uint16 UpdatedBitBytePos;
    /* pdu length */
    PduLengthType PduLen;
    /* pdu Id using in PduR_IpduMRxIndication */
    PduIdType PduId;
    /* mask value to set updated bit in its byte */
    uint8 UpdateBitByteMask;
} IpduM_ContainedRxPduCfgType;

/**
 * @brief  Configuration of a receiver ContainerPdu which may collect several ContainedPdus.
 */
typedef struct IpduM_ContainerRxPduTag
{
    /* start index array of containedRxPdu contained in this containerRxPdu. */
    PduLengthType RxContainedStartIndexInContainedPdu;
    /* container in buffer index */
    uint16 BufIndex;
#if (IPDUM_METADATA_SUPPORT == STD_ON)
    /* position to store MetaData */
    uint16 MetaDataBufIndex;
    /* size of MetaData */
    uint8 MetaDataSize;
#endif
    /* container start position in inner data global queue */
    PduLengthType ContainerRxQueueStartPos;
    /* length of this containerRxPdu */
    PduLengthType PduLen;
    /* Defines byte size of the header added before containedRxPdu . */
    uint8 IpduMContainerCfgHeaderSize;
    /* Defines whether the handling of this ContainerPdu shall be done in the
    context of the caller (IMMEDIATE) or
    in the next IpduM main function (DEFERRED). */
    IpduM_ProcessingType IpduMContainerPduProcessing;
    /* Defines a local queue for handling of each ContainerPdu. */
    uint8 IpduMContainerQueueSize;
    /* Defines for the received IpduMRxContainerPdu whether the list of
    referencing IpduMRxContainedPdus is a closed set. */
    IpduM_AcceptType IpduMContainerRxAcceptContainedPdu;
    /* count of containedRxPdu contained in the containerRxPdu */
    uint8 ContainerRxContainContainedCnt;
#if (STD_ON == IPDUM_ECUC_MULTIPLE_PARTITION_USED)
    IpduM_PartitionIDType IpduMContainerRxPartition;
#endif
} IpduM_ContainerRxPduCfgType;

/****link time configuration parameter type definitions**********/

/**
 * @brief  configuration type of Ipdum module.
 */
typedef struct IpduM_ConfigTag
{
    /* Configuration of a sender ContainedPdu. */
    const IpduM_ContainedTxPduCfgType* IpduMContainedTxPduPtr;
    /* Configuration of a transmitted container Pdu. */
    const IpduM_ContainerTxPduCfgType* IpduMContainerTxPduPtr;
    /* Configuration of a received contained Pdu */
    const IpduM_ContainedRxPduCfgType* IpduMContainedRxPduPtr;
    /* Configuration of a receiver ContainerPdu which may collect
    several ContainedPdus. */
    const IpduM_ContainerRxPduCfgType* IpduMContainerRxPduPtr;
    /* includes information about sent (Dynamic and Static )Part PDUs of multiplexed PDU using IpduM_Transmit call by
     * upper layer module*/
    const IpduM_TxPartType* IpduMMultiplexedPduTxPartPtr;
    /* includes information about all tx request multiplexed I-PDUs */
    const IpduM_TxRequestCfgType* IpduMTxRequestPtr;
    /* includes information about all rx indication multiplexed I-PDUs */
    const IpduM_RxIndicationCfgType* IpduMRxIndicationPtr;
    /* includes information about all rx dynamic part of indication I-PDUs */
    const IpduM_RxDynamicPartType* IpduMRxDynamicPartPduPtr;
    /* includes information about all rx static part of indication I-PDUs */
    const IpduM_RxStaticPartType* IpduMRxStaticPartPduPtr;
    /* segment information of all TxRequestPdus and RxIndicationPdus */
    const IpduM_SegmentType* IpduMTxRxSegmentPtr;
    /* containerTxPdu Index range for each IpduMPartition */
    const PduIdType* ContainerTxPduRange;
    /* containerRxPdu Index range for each IpduMPartition */
    const PduIdType* ContainerRxPduRange;
    /* TxRequestPdu Index range for each IpduMPartition */
    const PduIdType*    TxRequestPduRange;
    const uint8* const* TxRequestPdusInitialVal;
#if (IPDUM_CONFIG_NUM_OF_VARIANTS_USED > 1)
    /* map Pdu Container Macro Rx-From lower layer module to configuration index */
    const PduIdType* RxLowerLayerPduMacroMap;
    /* map Pdu Container Macro received from upper layer module to configuration index */
    const PduIdType* ReceivedUpTxPduMacroMap;
    /* map Pdu Container Macro Tx-To lower layer module to configuration index */
    const PduIdType* TxPduToLowerLayerMacroMap;
#endif
    /* start index of containedRxPdu whos do not define refered containerRxPdu */
    PduIdType StartIndexOfContainedRxForAccepALLCfg;
    /* Maximum number of transmitted IPdus in current variant */
    PduIdType TxUpPduNumInCurrentVariant;
    /* Maximum number of TxPathWays or IpduMTxRquest in current variant */
    PduIdType TxRequestNumInCurrentVariant;
    /* Maximum number of IpduMRxPathways or IpduMRxIndications*/
    PduIdType RxIndicationNumInCurrentVariant;
    /* Maximum number of containedTxPdus */
    PduIdType TxContainedNumInCurrentVariant;
    /* Maximum number of containerTxPdus */
    PduIdType TxContainerNumInCurrentVariant;
    /* Maximum number of IpduMRxContainerPdus */
    PduIdType RxContainerNumInCurrentVariant;
    /* Maximum number of static part's pdu of multiplexed pdu */
    PduIdType TxStaticPartPduNumInCurrentVariant;
    /* size of IpduM_RxAcceptAllContainedIndex array */
    uint8 ContainerRxAcceptAllContainedSize;

} IpduM_ConfigType;

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

#ifdef __cplusplus
}
#endif

#endif
