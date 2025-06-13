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
**  FILENAME    : SD_Types.h                                                  **
**                                                                            **
**  Created on  : 2019-03-11                                                  **
**  Author      : HuRongbo                                                    **
**  Vendor      : iSoft                                                       **
**  DESCRIPTION : Type definitions of SD                                      **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
#ifndef SD_TYPES_H
#define SD_TYPES_H

#include "Sd_Cfg.h"
#if SD_MODULE_ENABLE == STD_ON

#include "ComStack_Types.h"
#if (SD_DEM_USED == STD_ON)
#include "Dem.h"
#endif
#include "SoAd.h"

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/*@req <SWS_SD_00118> */
/* This type defines the Server states that are reported to the SD using the
expected API Sd_ServerServiceSetState. */
typedef enum
{
    SD_SERVER_SERVICE_DOWN,
    SD_SERVER_SERVICE_AVAILABLE
} Sd_ServerServiceSetStateType;

/*@req <SWS_SD_00405> */
/* This type defines the Client states that are reported to the BswM using the
expected API Sd_ClientServiceSetState. */
typedef enum
{
    SD_CLIENT_SERVICE_RELEASED,
    SD_CLIENT_SERVICE_REQUESTED
} Sd_ClientServiceSetStateType;

/*@req <SWS_SD_00550> */
/* This type defines the subscription policy by consumed EventGroup for the
Client Service. */
typedef enum
{
    SD_CONSUMED_EVENTGROUP_RELEASED,
    SD_CONSUMED_EVENTGROUP_REQUESTED
} Sd_ConsumedEventGroupSetStateType;

/*@req <SWS_SD_00551> */
/* This type defines the modes to indicate the current mode request of a Client
Service. */
typedef enum
{
    SD_CLIENT_SERVICE_DOWN,
    SD_CLIENT_SERVICE_AVAILABLE
} Sd_ClientServiceCurrentStateType;

/*@req <SWS_SD_00552> */
/* This type defines the subscription policy by consumed EventGroup for the
 * Client Service. */
typedef enum
{
    SD_CONSUMED_EVENTGROUP_DOWN,
    SD_CONSUMED_EVENTGROUP_AVAILABLE
} Sd_ConsumedEventGroupCurrentStateType;

/*@req <SWS_SD_00553> */
/* This type defines the subscription policy by EventHandler for the Server Service. */
typedef enum
{
    SD_EVENT_HANDLER_RELEASED,
    SD_EVENT_HANDLER_REQUESTED
} Sd_EventHandlerCurrentStateType;

/*@req <SWS_SD_91002> */
/* Type for a zero-terminated string of configuration options. */
typedef uint8* Sd_ConfigOptionStringType;

/*@req <SWS_SD_91001> */
/* SdCapabilityRecordMatchCallout type*/
typedef boolean (*SdCapabilityRecordMatchCalloutType)(
    PduIdType pduID,
    uint8 type,
    uint16 serviceID,
    uint16 instanceID,
    uint8 majorVersion,
    uint32 minorVersion,
    const Sd_ConfigOptionStringType* receivedConfigOptionPtrArray,
    const Sd_ConfigOptionStringType* configuredConfigOptionPtrArray);

/*@req <ECUC_SD_00043> */
/* This container specifies all timers used by the Service Discovery module for
 * Client Services. */
typedef struct
{
    /*@req <ECUC_SD_00063> */
    /* Max value in [ms] to delay randomly the transmission of a find message. */
    VAR(uint32, TYPEDEF) SdClientTimerInitialFindDelayMax;

    /*@req <ECUC_SD_00044> */
    /* Min value in [ms] to delay randomly the transmission of a find message.*/
    VAR(uint32, TYPEDEF) SdClientTimerInitialFindDelayMin;

    /*@req <ECUC_SD_00047> */
    /* The base delay in [ms] for find repetitions. */
    VAR(uint32, TYPEDEF) SdClientTimerInitialFindRepetitionsBaseDelay;

    /*@req <ECUC_SD_00046> */
    /* Configuration for the maximum number of find repetitions. */
    VAR(uint8, TYPEDEF) SdClientTimerInitialFindRepetitionsMax;

    /*@req <ECUC_SD_00036> */
    /* Maximum allowable response delay to entries received by multicast in ms. */
    VAR(uint32, TYPEDEF) SdClientTimerRequestResponseMaxDelay;

    /*@req <ECUC_SD_00064> */
    /* Minimum allowable response delay to the find message in ms. */
    VAR(uint32, TYPEDEF) SdClientTimerRequestResponseMinDelay;

    /*@req <ECUC_SD_00075> */
    /* Time to live for find and subscribe messages. */
    VAR(uint32, TYPEDEF) SdClientTimerTTL;

    /*@req <ECUC_SD_00133> */
    /* Time in milliseconds when a subscription to an event group shall be retriggered,
     * if no SubscribeEventGroupAck or SubscribeEventGroupNack was received. */
    VAR(uint16, TYPEDEF) SdSubscribeEventgroupRetryDelay;

    /*@req <ECUC_SD_00132> */
    /* Maximum count of retry a subscription. */
    VAR(uint8, TYPEDEF) SdSubscribeEventgroupRetryMax;
} Sd_ClientTimerType;

/*@req <ECUC_SD_00072>,<ECUC_SD_00032> */
/* Sd uses capability records to store arbitrary name/value pairs conveying
 * additional information about the named service. */
typedef struct
{
    /*@req <ECUC_SD_00073> */
    /* Defines a CapabilityRecord key. */
    P2CONST(char, TYPEDEF, SD_CONST) SdCapabilityRecordKey;

    /*@req <ECUC_SD_00074> */
    /* Defines the corresponding CapabilityRecord value. */
    P2CONST(char, TYPEDEF, SD_CONST) SdCapabilityRecordValue;
} Sd_CapabilityRecordType;

/* Reference SoAdSocketConnectionGroup */
typedef struct
{
    P2CONST(SoAd_SoConIdType, TYPEDEF, SD_CONST) SdSocketIdList;
    VAR(uint16, TYPEDEF) SdSoConNumInGroup;
} Sd_SoAdSoConGroupType;

typedef struct
{
    VAR(SoAd_RoutingGroupIdType, TYPEDEF) SdRoutingGroupId;
} Sd_SoAdRoutingGroupType;

/*@req <ECUC_SD_00056> */
/* This container specifies all parameters for consumed event groups. */
typedef struct
{
    /*@req <ECUC_SoAd_00108> */
    /* If existing and set to true, this EventGroup will be set to
     * "required" on start. */
    VAR(boolean, TYPEDEF) SdConsumedEventGroupAutoRequire;

    /*@req <ECUC_SD_00116> */
    /* The HandleId by which the BswM can identify this EventGroup. */
    VAR(uint16, TYPEDEF) SdConsumedEventGroupHandleId;

    /*@req <ECUC_SD_00057> */
    /* The Eventgroup Id of this eventGroup as a unique identifier of the
     * eventgroup in this service. This identifier is used for EventGroup
     * entries as well. */
    VAR(uint16, TYPEDEF) SdConsumedEventGroupId;

    /*@req <ECUC_SD_00106> */
    /* The reference of a Routing Group in order to activate and setup the
     * Socket Connection for Multicast Events of this EventGroup. */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdConsumedEventGroupMulticastActivationRef;

    /*@req <ECUC_SD_00119> */
    /* Reference to the SoAdSocketConnectionGroup representing the multicast
     * data path (UDP). */
    P2CONST(Sd_SoAdSoConGroupType, TYPEDEF, SD_CONST) SdConsumedEventGroupMulticastGroupRef;

    /*@req <ECUC_SD_00105> */
    /* The reference of the Routing Group for activation of the data path for
     * receiving TCP events. */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdConsumedEventGroupTcpActivationRef;

    /*@req <ECUC_SD_00107> */
    /* The reference of the SdClientTimer container for this eventGroup. */
    P2CONST(Sd_ClientTimerType, TYPEDEF, SD_CONST) SdConsumedEventGroupTimerRef;

    /*@req <ECUC_SD_00104> */
    /* The reference of the Routing Group for activation of the data path for
     * receiving UDP events. */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdConsumedEventGroupUdpActivationRef;

    /* Type2 entry Counter field value,assigned by user(Range:0-15) */
    VAR(uint8, TYPEDEF) SdConsumedEventGroupCounter;
} Sd_ConsumedEventGroupType;

/*@req <ECUC_SD_00099> */
/* Container element for representing the data path for accessing the server methods. */
typedef struct
{
    /*@req <ECUC_SD_00102> */
    /* Reference to a SoAdRoutingGroupRef to activate/deactivate the data path
     * for the methods. */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdClientServiceActivationRef;
} Sd_ConsumedMethodsType;

/*@req <ECUC_SD_00005> */
/* This container specifies all parameters used by Client services. */
typedef struct
{
    /*@req <ECUC_SoAd_00098> */
    /* If existing and set to true, this Service will be set to "required" on start. */
    VAR(boolean, TYPEDEF) SdClientServiceAutoRequire;

    /*@req <ECUC_SD_00079> */
    /* The HandleId by which the BswM can identify this Client Service Instance. */
    VAR(uint16, TYPEDEF) SdClientServiceHandleId;

    /*@req <ECUC_SD_00020> */
    /* Id to identify the service. This is unique for the service interface. */
    VAR(uint16, TYPEDEF) SdClientServiceId;

    /*@req <ECUC_SD_00022> */
    /* Configuration parameter to specify Instance Id of the service as used in
     * SD entries. */
    VAR(uint16, TYPEDEF) SdClientServiceInstanceId;

    /*@req <ECUC_SD_00070> */
    /* Major version number of the Service as used in the SD entries. */
    VAR(uint8, TYPEDEF) SdClientServiceMajorVersion;

    /*@req <ECUC_SD_00071> */
    /* Minor version number of the Service as used in the SD Service Entries. */
    VAR(uint32, TYPEDEF) SdClientServiceMinorVersion;

    /*@req <ECUC_SD_00127> */
    /* Reference to a SdCapabilityRecordMatchCallout */
    VAR(uint16, TYPEDEF) SdClientCapabilityRecordMatchCalloutRef;

    /*@req <ECUC_SD_00100> */
    /* Reference to the SoAdSocketConnection representing the data path (TCP)
     * for communication with methods. */
    P2CONST(Sd_SoAdSoConGroupType, TYPEDEF, SD_CONST) SdClientServiceTcpRef;

    /*@req <ECUC_SD_00103> */
    /* The reference of the SdClientTimer container for this service. */
    P2CONST(Sd_ClientTimerType, TYPEDEF, SD_CONST) SdClientServiceTimerRef;

    /*@req <ECUC_SD_00101> */
    /* Reference to the SoAdSocketConnection representing the data path (UDP)
     * for communication with methods. */
    P2CONST(Sd_SoAdSoConGroupType, TYPEDEF, SD_CONST) SdClientServiceUdpRef;

    /* Sd uses capability records to store arbitrary name/value pairs conveying
     * additional information about the named service. */
    P2CONST(Sd_CapabilityRecordType, TYPEDEF, SD_CONST) SdClientCapabilityRecord;
    VAR(uint16, TYPEDEF) SdClientCapabilityRecordNum;

    /* This container specifies all parameters for consumed event groups. */
    P2CONST(Sd_ConsumedEventGroupType, TYPEDEF, SD_CONST) SdConsumedEventGroup;
    VAR(uint16, TYPEDEF) SdConsumedEventGroupNum;

    /* Container element for representing the data path for accessing the server
     * methods. */
    P2CONST(Sd_ConsumedMethodsType, TYPEDEF, SD_CONST) SdConsumedMethods;
} Sd_ClientServiceType;

#if (SD_DEM_USED == STD_ON)
typedef struct
{
    Dem_EventIdType EventId;
} Sd_DemEventParameterType;

/*@req <ECUC_SD_00120> */
/* Container for the references to DemEventParameter elements which shall be invoked
 * using the API Dem_ReportErrorStatus API in case the corresponding error occurs. */
typedef struct
{
    /*@req <ECUC_SD_00121> */
    /* Reference to the DemEventParameter which shall be issued when the
     * SD Instance received malformed messsage. */
    P2CONST(Sd_DemEventParameterType, TYPEDEF, SD_CONST) SD_E_MALFORMED_MSG;

    /*@req <ECUC_SD_00122> */
    /* Reference to the DemEventParameter which shall be issued when the
     * SD Instance does not have enough resources to handle client. */
    P2CONST(Sd_DemEventParameterType, TYPEDEF, SD_CONST) SD_E_OUT_OF_RES;

    /*@req <ECUC_SD_00123> */
    /* Reference to the DemEventParameter which shall be issued when
     * receiving SubscribeEventgroupNack entry. */
    P2CONST(Sd_DemEventParameterType, TYPEDEF, SD_CONST) SD_E_SUBSCR_NACK_RECV;
} Sd_InstanceDemEventParameterRefsType;
#endif

/*@req <ECUC_SD_00081> */
/* This container specifies the received PDU. */
typedef struct
{
    /*@req <ECUC_SD_00028> */
    /* ID of the PDU that will be received via the API Sd_SoAdIfRxIndication(). */
    VAR(PduIdType, TYPEDEF) SdRxPduId;

    /*@req <ECUC_SD_00029> */
    /* Reference to the "global" Pdu structure to allow harmonization of handle
     * IDs in the COM-Stack. */
    VAR(PduIdType, TYPEDEF) SdRxPduRef;
} Sd_InstanceMulticastRxPduType;

/*@req <ECUC_SD_00030> */
/* This container specifies the transmitted PDU. */
typedef struct
{
    /*@req <ECUC_SD_00109> */
    /* Reference to the "global" Pdu structure to allow harmonization of handle
     * IDs in the COM-Stack. */
    VAR(PduIdType, TYPEDEF) SdTxPduRef;
} Sd_InstanceTxPduType;

/*@req <ECUC_SD_00027> */
/* This container specifies the received PDU. */
typedef struct
{
    /*@req <ECUC_SD_00082> */
    /* ID of the PDU that will be received via the API Sd_SoAdIfRxIndication(). */
    VAR(PduIdType, TYPEDEF) SdRxPduId;

    /*@req <ECUC_SD_00083> */
    /* Reference to the "global" Pdu structure to allow harmonization of handle
     * IDs in the COM-Stack. */
    VAR(PduIdType, TYPEDEF) SdRxPduRef;
} Sd_InstanceUnicastRxPduType;

/*@req <ECUC_SD_00035> */
/* This container specifies all timers used by the Service Discovery module for
 * Server Services. */
typedef struct
{
    /*@req <ECUC_SD_00039> */
    /* Max value in [ms] to delay randomly the first offer. */
    VAR(uint32, TYPEDEF) SdServerTimerInitialOfferDelayMax;

    /*@req <ECUC_SD_00038> */
    /* Min value in [ms] to delay randomly the first offer. */
    VAR(uint32, TYPEDEF) SdServerTimerInitialOfferDelayMin;

    /*@req <ECUC_SD_00041> */
    /* The base delay in [ms] for offer repetitions. Successive offers have an
     * exponential back off delay (1x base delay, 2x base delay, 4x base delay). */
    VAR(uint32, TYPEDEF) SdServerTimerInitialOfferRepetitionBaseDelay;

    /*@req <ECUC_SD_00040> */
    /* Configure the maximum amount of offer repetition. */
    VAR(uint8, TYPEDEF) SdServerTimerInitialOfferRepetitionsMax;

    /*@req <ECUC_SD_00076> */
    /* Interval between cyclic offers in the main phase. */
    VAR(uint32, TYPEDEF) SdServerTimerOfferCyclicDelay;

    /*@req <ECUC_SD_00114> */
    /* Maximum allowable response delay to entries received by multicast in seconds. */
    VAR(uint32, TYPEDEF) SdServerTimerRequestResponseMaxDelay;

    /*@req <ECUC_SD_00115> */
    /* Minimum allowable response delay to entries received by multicast in seconds. */
    VAR(uint32, TYPEDEF) SdServerTimerRequestResponseMinDelay;

    /*@req <ECUC_SD_00037> */
    /* Time to live for offer service. */
    VAR(uint32, TYPEDEF) SdServerTimerTTL;
} Sd_ServerTimerType;

typedef struct
{
    VAR(SoAd_SoConIdType, TYPEDEF) SdSoConId;
} Sd_SoAdSoConType;

/*@req <ECUC_SD_00094> */
/* The subcontainer including the Routing Group for Activation of Events sent
 * over Multicast. */
typedef struct
{
    /*@req <ECUC_SD_00096> */
    /* Reference to a SoAdRoutingGroup for activation of the data path for a subscribed
     * client (start sending events after subscribe). */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdEventActivationRef;

    /*@req <ECUC_SD_00118> */
    /* Reference to the SoAdSocketConnection representing the multicast data path (UDP). */
    P2CONST(Sd_SoAdSoConType, TYPEDEF, SD_CONST) SdMulticastEventSoConRef;
} Sd_EventHandlerMulticastType;

/*@req <ECUC_SD_00093> */
/* The subcontainer including the Routing Groups for Activation and Trigger Transmit
 * for Events sent over TCP. */
typedef struct
{
    /*@req <ECUC_SD_00096> */
    /* Reference to a SoAdRoutingGroup for activation of the data path for a subscribed
     * client (start sending events after subscribe). */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdEventActivationRef;

    /*@req <ECUC_SD_00095> */
    /* Reference to a SoAdRoutingGroup that is used for triggered transmit. */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdEventTriggeringRef;
} Sd_EventHandlerTcpType;

/*@req <ECUC_SD_00092> */
/* The subcontainer including the Routing Groups for Activation and Trigger Transmit
 * for Events sent over UDP. */
typedef struct
{
    /*@req <ECUC_SD_00096> */
    /* Reference to a SoAdRoutingGroup for activation of the data path for a
     * subscribed client (start sending events after subscribe). */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdEventActivationRef;

    /*@req <ECUC_SD_00095> */
    /* Reference to a SoAdRoutingGroup that is used for triggered transmit. */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdEventTriggeringRef;
} Sd_EventHandlerUdpType;

/*@req <ECUC_SD_00055> */
/* Container Element for representing an EventGroup as part of the Service Instance. */
typedef struct
{
    /*@req <ECUC_SD_00061> */
    /* The EventGroup Id of this EventGroup as a unique identifier of the EventGroup
     * in this service.This identifier is used for EventGroup entries as well. */
    VAR(uint16, TYPEDEF) SdEventHandlerEventGroupId;

    /*@req <ECUC_SD_00112> */
    /* The HandleId by which the BswM can identify this EventGroup. */
    VAR(uint16, TYPEDEF) SdEventHandlerHandleId;

    /*@req <ECUC_SD_00097> */
    /* Specifies the number of subscribed clients that trigger the Server
     * to change the transmission of events to Multicast. */
    VAR(uint16, TYPEDEF) SdEventHandlerMulticastThreshold;

    /*@req <ECUC_SD_00113> */
    /* The reference of the SdServerTimer container for this EventGroup. */
    P2CONST(Sd_ServerTimerType, TYPEDEF, SD_CONST) SdEventHandlerTimerRef;

    /* Container Element for representing an EventGroup as part of the Service Instance. */
    P2CONST(Sd_EventHandlerMulticastType, TYPEDEF, SD_CONST) SdEventHandlerMulticast;

    /* The subcontainer including the Routing Groups for Activation and Trigger
     * Transmit for Events sent over TCP. */
    P2CONST(Sd_EventHandlerTcpType, TYPEDEF, SD_CONST) SdEventHandlerTcp;

    /* The subcontainer including the Routing Groups for Activation and Trigger
     * Transmit for Events sent over UDP. */
    P2CONST(Sd_EventHandlerUdpType, TYPEDEF, SD_CONST) SdEventHandlerUdp;
} Sd_EventHandlerType;

/*@req <ECUC_SD_00087> */
/* Container element for representing the needed elements of the data path
 * for the methods provided by the service. */
typedef struct
{
    /*@req <ECUC_SD_00090> */
    /* Reference to a SoAdRoutingGroup to activated and deactivate the data
     * path for methods of the service. */
    P2CONST(Sd_SoAdRoutingGroupType, TYPEDEF, SD_CONST) SdServerServiceActivationRef;
} Sd_ProvidedMethodsType;

/*@req <ECUC_SD_00004> */
/* This container specifies all parameters used by Server services. */
typedef struct
{
    /*@req <ECUC_SoAd_00085> */
    /* If existing and set to true, this Service will be set to "Available" on start. */
    VAR(boolean, TYPEDEF) SdServerServiceAutoAvailable;

    /*@req <ECUC_SD_00110> */
    /* The HandleId by which the BswM can identify this Server Service Instance. */
    VAR(uint16, TYPEDEF) SdServerServiceHandleId;

    /*@req <ECUC_SD_00009> */
    /* Id to identify the service. This is unique for the service interface. */
    VAR(uint16, TYPEDEF) SdServerServiceId;

    /*@req <ECUC_SD_00011> */
    /* Configuration parameter to specify Instance Id of the Service implemented
     * by the Server Service. */
    VAR(uint16, TYPEDEF) SdServerServiceInstanceId;

    /*@req <ECUC_SD_00068> */
    /* Major version number of the Service as used in SD Entries. */
    VAR(uint8, TYPEDEF) SdServerServiceMajorVersion;

    /*@req <ECUC_SD_00069> */
    /* Minor version number of the Service as used e.g. in Offer Service entries. */
    VAR(uint32, TYPEDEF) SdServerServiceMinorVersion;

    /*@req <ECUC_SD_00126> */
    /* Reference to a SdCapabilityRecordMatchCallout */
    VAR(uint16, TYPEDEF) SdServerCapabilityRecordMatchCalloutRef;

    /*@req <ECUC_SD_00088> */
    /* Reference to SoAdSocketConnectionGroup used for methods.This is used to
     * access the local IP address and port for building the endpoint option for
     * offers of this service. */
    P2CONST(Sd_SoAdSoConGroupType, TYPEDEF, SD_CONST) SdServerServiceTcpRef;

    /*@req <ECUC_SD_00086> */
    /* The reference of the SdClientTimer container for this service. */
    P2CONST(Sd_ServerTimerType, TYPEDEF, SD_CONST) SdServerServiceTimerRef;

    /*@req <ECUC_SD_00089> */
    /* Reference to SoAdSocketConnectionGroup used for methods.This is used to
     * access the local IP address and port for building the endpoint option for
     * offers of this service. */
    P2CONST(Sd_SoAdSoConGroupType, TYPEDEF, SD_CONST) SdServerServiceUdpRef;

    /* Container Element for representing an EventGroup as part of the Service Instance. */
    P2CONST(Sd_EventHandlerType, TYPEDEF, SD_CONST) SdEventHandler;
    VAR(uint16, TYPEDEF) SdEventHandlerNum;

    /* Container element for representing the needed elements of the data path for the
     * methods provided by the service. */
    P2CONST(Sd_ProvidedMethodsType, TYPEDEF, SD_CONST) SdProvidedMethods;

    /* Container element for representing the needed elements of the data path for the
     * methods provided by the service. */
    P2CONST(Sd_CapabilityRecordType, TYPEDEF, SD_CONST) SdServerCapabilityRecord;
    VAR(uint16, TYPEDEF) SdServerCapabilityRecordNum;
} Sd_ServerServiceType;

/*@req <ECUC_SD_00084> */
/* This container represents an instance of the SD */
typedef struct
{
    VAR(uint16, TYPEDEF) SdInstanceHandleId;

    /*@req <ECUC_SD_00012> */
    /* Configuration parameter to specify the Hostname. */
    P2CONST(char, TYPEDEF, SD_CONST) SdInstanceHostname;

    /* Describes on how many bits of the addresses shall be compared to determine*/
    uint8 SdInstanceLocalAdressCheckLength;
    boolean SdInstanceLocalAdressCheckLengthEnable;

    /* This container specifies all parameters used by Client services. */
    P2CONST(Sd_ClientServiceType, TYPEDEF, SD_CONST) SdClientService;
    VAR(uint16, TYPEDEF) SdClientServiceNum;

    /* This container specifies all timers used by the Service Discovery
     * module for Client Services. */
    P2CONST(Sd_ClientTimerType, TYPEDEF, SD_CONST) SdClientTimer;

    /* Container for the references to DemEventParameter elements which shall
     * be invoked using the API Dem_ReportErrorStatus API in case the corresponding
     * error occurs.  */
#if (SD_DEM_USED == STD_ON)
    P2CONST(Sd_InstanceDemEventParameterRefsType, TYPEDEF, SD_CONST) SdInstanceDemEventParameterRefs;
#endif

    /* This container specifies the received PDU. */
    VAR(PduIdType, TYPEDEF) SdInstanceMulticastRxPdu;

    /* This container specifies the transmitted PDU. */
    VAR(PduIdType, TYPEDEF) SdInstanceTxPdu;
    VAR(PduIdType, TYPEDEF) SdInstanceTxPduRef;
    /* SdInstanceTxPdu's SoConId in SoAd */
    VAR(SoAd_SoConIdType, TYPEDEF) SdInstanceTxPduSoConId;

    VAR(TcpIp_DomainType, TYPEDEF) SdAddrFamily;
    /* Option length except configuration option */
    VAR(uint8, TYPEDEF) SdOptionLength;

    /* This container specifies the received PDU. */
    VAR(PduIdType, TYPEDEF) SdInstanceUnicastRxPdu;

    /* This container specifies all parameters used by Server services. */
    P2CONST(Sd_ServerServiceType, TYPEDEF, SD_CONST) SdServerService;
    VAR(uint16, TYPEDEF) SdServerServiceNum;

    /* This container specifies all timers used by the Service Discovery
     * module for Server Services. */
    P2CONST(Sd_ServerTimerType, TYPEDEF, SD_CONST) SdServerTimer;
} Sd_InstanceType;

typedef struct
{
    VAR(PduIdType, TYPEDEF) SdRxPduId;
    VAR(SoAd_SoConIdType, TYPEDEF) SoConId;
} Sd_RxPduIdSoConIdMapType;

/*@req <ECUC_SD_00003> */
/* This container contains the configuration parameters and sub containers of
 * the AUTOSAR Service Discovery module. */
typedef struct
{
    /* This container represents an instance of the SD */
    P2CONST(Sd_InstanceType, TYPEDEF, SD_CONST) SdInstance;

    P2CONST(Sd_ServerServiceType, TYPEDEF, SD_CONST) SdGlbServerService;
    P2CONST(Sd_ClientServiceType, TYPEDEF, SD_CONST) SdGlbClientService;
    P2CONST(Sd_ConsumedEventGroupType, TYPEDEF, SD_CONST) SdGlbConsumedEventGroup;
    P2CONST(Sd_RxPduIdSoConIdMapType, TYPEDEF, SD_CONST) SdGlbRxPduIdSoConIdMap;
    P2CONST(SdCapabilityRecordMatchCalloutType, TYPEDEF, SD_CONST) SdCapabilityRecordMatchCalloutRef;
} Sd_ConfigType;

#endif /* SD_MODULE_ENABLE == STD_ON */
#endif /* SD_TYPES_H */
