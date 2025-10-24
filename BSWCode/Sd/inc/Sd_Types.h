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
 **  @file               : Sd_Types.h
 **  @author             : HuRongbo
 **  @date               : 2019-03-11
 **  @vendor             : isoft
 **  @description        : Type definitions of SD
 **
 ***********************************************************************************************************************/

#ifndef SD_TYPES_H
#define SD_TYPES_H

#include "Sd_Cfg.h"
#if SD_MODULE_ENABLE == STD_ON

#include "ComStack_Types.h"
#if (SD_DEM_USED == STD_ON)
#include "Dem.h"
#endif
#include "SoAd.h"
#include "Os_Types.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ================================================ type definitions ================================================ */
/**
 * @brief SomeIp/Ip-SD Char Type
 */
typedef char Sd_CharType; /* PRQA S 3632 */ /* VL_Sd_StringType */

/*@req <SWS_SD_00118> */
/**
 * @brief This type defines the Server states that are reported to the SD using the expected API
 * Sd_ServerServiceSetState.
 */
typedef enum Sd_ServerServiceSetStateTypeTag
{
    SD_SERVER_SERVICE_DOWN,     /**< Server service is down. @range 0 */
    SD_SERVER_SERVICE_AVAILABLE /**< Server service is available. @range 1 */
} Sd_ServerServiceSetStateType;

/*@req <SWS_SD_00405> */
/**
 * @brief This type defines the Client states that are reported to the BswM using the expected API
 * Sd_ClientServiceSetState.
 */
typedef enum Sd_ClientServiceSetStateTypeTag
{
    SD_CLIENT_SERVICE_RELEASED, /**< Client service is released. @range 0 */
    SD_CLIENT_SERVICE_REQUESTED /**< Client service is requested. @range 1 */
} Sd_ClientServiceSetStateType;

/*@req <SWS_SD_00550> */
/**
 * @brief This type defines the subscription policy by consumed EventGroup for the Client Service.
 */
typedef enum Sd_ConsumedEventGroupSetStateTypeTag
{
    SD_CONSUMED_EVENTGROUP_RELEASED, /**< Consumed event group is released. @range 0 */
    SD_CONSUMED_EVENTGROUP_REQUESTED /**< Consumed event group is requested. @range 1 */
} Sd_ConsumedEventGroupSetStateType;

/*@req <SWS_SD_00551> */
/**
 * @brief This type defines the modes to indicate the current mode request of a Client Service.
 */
typedef enum Sd_ClientServiceCurrentStateTypeTag
{
    SD_CLIENT_SERVICE_DOWN,     /**< Client service is down. @range 0 */
    SD_CLIENT_SERVICE_AVAILABLE /**< Client service is available. @range 1 */
} Sd_ClientServiceCurrentStateType;

/*@req <SWS_SD_00552> */
/**
 * @brief This type defines the subscription policy by consumed EventGroup for the Client Service.
 */
typedef enum Sd_ConsumedEventGroupCurrentStateTypeTag
{
    SD_CONSUMED_EVENTGROUP_DOWN,     /**< Consumed event group is down. @range 0 */
    SD_CONSUMED_EVENTGROUP_AVAILABLE /**< Consumed event group is available. @range 1 */
} Sd_ConsumedEventGroupCurrentStateType;

/*@req <SWS_SD_00553> */
/**
 * @brief This type defines the subscription policy by EventHandler for the Server Service.
 */
typedef enum Sd_EventHandlerCurrentStateTypeTag
{
    SD_EVENT_HANDLER_RELEASED, /**< Event handler is released. @range 0 */
    SD_EVENT_HANDLER_REQUESTED /**< Event handler is requested. @range 1 */
} Sd_EventHandlerCurrentStateType;

/**
 * @brief This type defines the version-driven find behavior for the Service Discovery.
 */
typedef enum Sd_VersionDrivenFindBehaviorTag
{
    SD_EXACT_OR_ANY_MINOR_VERSION, /**< Exact or any minor version is accepted.  @range 1 */
    SD_MINIMUM_MINOR_VERSION       /**< Minimum minor version is accepted. @range 2 */
} Sd_VersionDrivenFindBehavior;

/** @brief  Service Group ID */
typedef uint16 Sd_ServiceGroupIdType;
/*@req <SWS_SD_91002> */
/** @brief Type for a zero-terminated string of configuration options. */
typedef uint8* Sd_ConfigOptionStringType;
/** @brief Type for a handler ID map. */
typedef uint16 Sd_HandlerIdMapType;

/*@req <SWS_SD_91001> */
/**
 * @brief SdCapabilityRecordMatchCallout type
 */
typedef boolean (*SdCapabilityRecordMatchCalloutType)(
    PduIdType                        pduID,
    uint8                            type,
    uint16                           serviceID,
    uint16                           instanceID,
    uint8                            majorVersion,
    uint32                           minorVersion,
    const Sd_ConfigOptionStringType* receivedConfigOptionPtrArray,
    const Sd_ConfigOptionStringType* configuredConfigOptionPtrArray);

/*@req <ECUC_SD_00043> */
/**
 * @brief This container specifies all timers used by the Service Discovery module for
 *        Client Services.
 */
typedef struct Sd_ClientTimerTypeTag
{
    /*@req <ECUC_SD_00063> */
    uint32 SdClientTimerInitialFindDelayMax; /**<Max value in [ms] to delay randomly the transmission of a find message.
                                                @range 0..0xffffffff */

    /*@req <ECUC_SD_00044> */
    uint32 SdClientTimerInitialFindDelayMin; /**<Min value in [ms] to delay randomly the transmission of a find message.
                                                @range 0..0xffffffff */

    /*@req <ECUC_SD_00047> */
    uint32 SdClientTimerInitialFindRepetitionsBaseDelay; /**<The base delay in [ms] for find repetitions.
                                                            @range 0..0xffffffff */

    /*@req <ECUC_SD_00036> */
    uint32 SdClientTimerRequestResponseMaxDelay; /**<Maximum allowable response delay to entries received by multicast
                                                    in ms. @range 0..0xffffffff */

    /*@req <ECUC_SD_00064> */
    uint32 SdClientTimerRequestResponseMinDelay; /**<Minimum allowable response delay to the find message in ms.
                                                    @range 0..0xffffffff */

    /*@req <ECUC_SD_00075> */
    uint32 SdClientTimerTTL; /**<Time to live for find and subscribe messages. @range 0..0xffffffff */

    /*@req <ECUC_SD_00133> */
    uint16 SdSubscribeEventgroupRetryDelay; /**<Time in milliseconds when a subscription to an event group shall be
                                             * retriggered, if no SubscribeEventGroupAck or SubscribeEventGroupNack was
                                             * received. @range 0..65535 */

    /*@req <ECUC_SD_00046> */
    uint8 SdClientTimerInitialFindRepetitionsMax; /**<Configuration for the maximum number of find repetitions.
                                                     @range 0..255 */

    /*@req <ECUC_SD_00132> */
    uint8 SdSubscribeEventgroupRetryMax; /**<Maximum count of retry a subscription. @range 0..255 */
} Sd_ClientTimerType;

/*@req <ECUC_SD_00072>,<ECUC_SD_00032> */
/**
 * @brief Sd uses capability records to store arbitrary name/value pairs
 *        conveying additional information about the named service.
 */
typedef struct Sd_CapabilityRecordTypeTag
{
    /*@req <ECUC_SD_00073> */
    const Sd_CharType* SdCapabilityRecordKey; /**<Defines a CapabilityRecord key. @range NA */

    /*@req <ECUC_SD_00074> */
    const Sd_CharType* SdCapabilityRecordValue; /**<Defines the corresponding CapabilityRecord value. @range NA */
} Sd_CapabilityRecordType;

/**
 * @brief Reference SoAdSocketConnectionGroup
 */
typedef struct Sd_SoAdSoConGroupTypeTag
{
    const SoAd_SoConIdType* SocketIdList;    /**< Pointer to an array of SoAd socket connection IDs. @range NA */
    uint16                  SoConNumInGroup; /**< Number of socket connections in the group. @range 0..65535 */
} Sd_SoAdSoConGroupType;

/**
 * @brief Structure for soad routing group
 */
typedef struct Sd_SoAdRoutingGroupTypeTag
{
    SoAd_RoutingGroupIdType RoutingGroupId; /**< ID of the routing group. @range 0..65535 */
} Sd_SoAdRoutingGroupType;

/*@req <ECUC_SD_00056> */
/**
 * @brief This container specifies all parameters for consumed event groups.
 */
typedef struct Sd_ConsumedEventGroupTypeTag
{
    /*@req <ECUC_SD_00106> */
    const Sd_SoAdRoutingGroupType*
        SdConsumedEventGroupMulticastActivationRef; /**<The reference of a Routing Group in order to activate and setup
                                                       the Socket Connection for Multicast Events of this EventGroup.
                                                       @range NA */

    /*@req <ECUC_SD_00119> */
    const Sd_SoAdSoConGroupType*
        SdConsumedEventGroupMulticastGroupRef; /**< Reference to the SoAdSocketConnectionGroup representing the
                                                  multicast data path (UDP). @range NA */

    /*@req <ECUC_SD_00105> */
    const Sd_SoAdRoutingGroupType*
        SdConsumedEventGroupTcpActivationRef; /**< The reference of the Routing Group for activation of the data path
                                                 for receiving TCP events. @range NA */

    /*@req <ECUC_SD_00104> */
    const Sd_SoAdRoutingGroupType*
        SdConsumedEventGroupUdpActivationRef; /**< The reference of the Routing Group for activation of the data path
                                                 for receiving UDP events. @range NA */

    /*@req <ECUC_SD_00107> */
    const Sd_ClientTimerType* SdConsumedEventGroupTimerRef; /**< The reference of the SdClientTimer container for this
                                                               eventGroup. @range NA */

    /*@req <ECUC_SD_00116> */
    uint16 SdConsumedEventGroupHandleId; /**< The HandleId by which the BswM can identify this EventGroup.
                                            @range 0..65535 */

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
    ApplicationType ConsumedEventGroupApplicationId; /**< The ID of the partition where the ConsumedEvent is located.
                                                        @range 0..65535 */
#endif

    /*@req <ECUC_SD_00057> */
    uint16 SdConsumedEventGroupId; /**< The Eventgroup Id of this eventGroup as a unique identifier of the eventgroup in
                                      this service. This identifier is used for EventGroup entries as well.
                                      @range 0..65535 */

    uint8 SdConsumedEventGroupCounter; /**< Type2 entry Counter field value,assigned by user. @range 0..15 */

    /*@req <ECUC_SoAd_00108> */
    /* If existing and set to true, this EventGroup will be set to
     * "required" on start. */
    boolean SdConsumedEventGroupAutoRequire; /**< If existing and set to true, this EventGroup will be set to "required"
                                                on start. @range TRUE/FALSE */
} Sd_ConsumedEventGroupType;

/*@req <ECUC_SD_00099> */
/**
 * @brief Container element for representing the data path for accessing the server methods.
 */
typedef struct Sd_ConsumedMethodsTypeTag
{
    /*@req <ECUC_SD_00102> */
    const Sd_SoAdRoutingGroupType*
        SdClientServiceActivationRef; /**< Reference to a SoAdRoutingGroupRef to activate/deactivate the data path for
                                         the methods. @range NA */
} Sd_ConsumedMethodsType;

/**
 * @brief Type to define the blocklisted minor versions.
 */
typedef uint32 SdBlocklistedMinorVersionsType;

/**
 * @brief  Structure for SD blocklisted versions.
 */
typedef struct Sd_SdBlocklistedVersionTag
{
    SdBlocklistedMinorVersionsType BlocklistedMinorVersions; /**< Subversion number blacklist. @range 0..0xFFFFFFFF */
} Sd_BlocklistedVersionType;

/*@req <ECUC_SD_00005> */
/**
 * @brief This container specifies all parameters used by Client services.
 */
typedef struct Sd_ClientServiceTypeTag
{
    /*@req <ECUC_SD_00141> */
    Sd_BlocklistedVersionType* SdBlocklistedVersions; /**< Collection of blocklisted versions. @range NA */
    uint32 SdBlocklistedMinorVersionNum; /**< Minor version number of the blocklisted version. @range 0..0xffffffff */

    /*@req <ECUC_SD_00071> */
    uint32 SdClientServiceMinorVersion; /**< Minor version number of the Service as used in the SD Service Entries.
                                           @range 0..0xffffffff*/

    /*@req <ECUC_SD_00100> */
    const Sd_SoAdSoConGroupType* SdClientServiceTcpRef; /**< Reference to the SoAdSocketConnection representing the data
                                                          path (TCP) for communication with methods. @range NA*/

    const Sd_SoAdSoConGroupType*
        SdClientServiceMulticastRef; /**< Reference to the SoAdSocketConnection representing
                                 the data path (Multicast) for communication with methods. @range NA*/

    /*@req <ECUC_SD_00103> */
    const Sd_ClientTimerType*
        SdClientServiceTimerRef; /**< The reference of the SdClientTimer container for this service. @range NA*/

    /*@req <ECUC_SD_00101> */
    const Sd_SoAdSoConGroupType* SdClientServiceUdpRef; /**< Reference to the SoAdSocketConnection representing the data
                                                         path (UDP) for communication with methods. @range NA*/

    const Sd_CapabilityRecordType*
        SdClientCapabilityRecord; /**< Sd uses capability records to store arbitrary name/value pairs conveying
                                     additional information about the named service. @range NA*/

    const Sd_ConsumedEventGroupType*
        SdConsumedEventGroup; /**< This container specifies all parameters for consumed event groups. @range NA*/

    const Sd_ConsumedMethodsType* SdConsumedMethods; /**< Container element for representing the data path for accessing
                                                      * the server methods. @range NA*/

    uint16 ClientCapabilityRecordNum; /**< This parameter specifies the number of client capability records.
                                         @range 0..65535 */
    uint16 ConsumedEventGroupNum; /**< This parameter specifies the number of consumed event groups. @range 0..65535 */
    uint16 ServiceGroupRefNum; /**< This parameter specifies the number of service group references. @range 0..65535 */

    /*@req <ECUC_SD_00079> */
    uint16 SdClientServiceHandleId; /**< This parameter specifies the handle id. @range 0..65535 */

    /*@req <ECUC_SD_00020> */
    uint16 SdClientServiceId; /**< This parameter specifies the service id. @range 0..65535 */

    /*@req <ECUC_SD_00022> */
    uint16 SdClientServiceInstanceId; /**< This parameter specifies the instance id of the service as used in SD
                                         entries. @range 0..65535 */

    /*@req <ECUC_SD_00127> */
    uint16 SdClientCapabilityRecordMatchCalloutRef; /**< This parameter specifies the reference to a
                                                       SdCapabilityRecordMatchCallout. @range 0..65535 */

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
    ApplicationType
        ClientApplicationId; /**< This parameter specifies the applicationId of the client. @range 0..65535 */
#endif

    Std_ReturnType SdVersionDrivenFindBehaviorEnable; /**< This parameter specifies the enablement of the
                                                         SdVersionDrivenFindBehavior. @range 0..1 */

    /*@req <ECUC_SD_00140> */
    Sd_VersionDrivenFindBehavior SdVersionDrivenFindBehavior; /**< Defined the possible acceptance kinds for required
                                                                 service instances. @range NA*/

    /*@req <ECUC_SD_00070> */
    uint8 SdClientServiceMajorVersion; /**< Major version number of the Service as used in the SD entries. @range 0..255
                                        */

    /*@req <ECUC_SoAd_00098> */
    boolean SdClientServiceAutoRequire; /**< If existing and set to true, this Service will be set to "required" on
                                           start. @range TRUE/FALSE */
} Sd_ClientServiceType;

#if (SD_DEM_USED == STD_ON)
/**
 * @brief Dem Event Parameter
 */
typedef struct Sd_DemEventParameterTypeTag
{
    Dem_EventIdType EventId; /**< Dem Event ID. @range 0..255*/
} Sd_DemEventParameterType;

/*@req <ECUC_SD_00120> */
/**
 * @brief Container for the references to DemEventParameter elements which shall be invoked
 *        using the API Dem_ReportErrorStatus API in case the corresponding error occurs.
 */
typedef struct Sd_InstanceDemEventParameterRefsTypeTag
{
    /*@req <ECUC_SD_00121> */
    const Sd_DemEventParameterType* SD_E_MALFORMED_MSG; /**< Ref to DemEventParameter which shall be issued when the Sd
                                                           Instance received malformed messsage. @range NA */

    /*@req <ECUC_SD_00122> */
    const Sd_DemEventParameterType*
        SD_E_OUT_OF_RES; /**< Reference to the DemEventParameter which shall be issued when
                            the Sd Instance does not have enough resources to handle client. @range NA */

    /*@req <ECUC_SD_00123> */
    const Sd_DemEventParameterType* SD_E_SUBSCR_NACK_RECV; /**< Reference to the DemEventParameter which shall be issued
                                                              when receiving SubscribeEventgroupNack entry. @range NA */
} Sd_InstanceDemEventParameterRefsType;
#endif

/*@req <ECUC_SD_00081> */
/**
 * @brief This container specifies the received PDU.
 */
typedef struct Sd_InstanceMulticastRxPduTypeTag
{
    /*@req <ECUC_SD_00028> */
    PduIdType SdRxPduId; /**< ID of the PDU that will be received via the API Sd_SoAdIfRxIndication(). @range 0..255 */

    /*@req <ECUC_SD_00029> */
    PduIdType SdRxPduRef; /**< Reference to the "global" Pdu structure to allow harmonization of handle
                            IDs in the COM-Stack. @range 0..255 */
} Sd_InstanceMulticastRxPduType;

/*@req <ECUC_SD_00030> */
/**
 * @brief This container specifies the transmitted PDU.
 */
typedef struct Sd_InstanceTxPduTypeTag
{
    /*@req <ECUC_SD_00109> */
    PduIdType SdTxPduRef; /**< Refence to the "global" Pdu structure to allow harmonization of handle IDs in the
                             COM-Stack. @range 0..255 */
} Sd_InstanceTxPduType;

/*@req <ECUC_SD_00027> */
/**
 * @brief This container specifies the received PDU.
 */
typedef struct Sd_InstanceUnicastRxPduTypeTag
{
    /*@req <ECUC_SD_00082> */
    PduIdType SdRxPduId; /**< ID of the PDU that will be received via the API Sd_SoAdIfRxIndication(). @range 0..255 */

    /*@req <ECUC_SD_00083> */
    PduIdType SdRxPduRef; /**< Reference to the "global" Pdu structure to allow harmonization of handle IDs in the
                             COM-Stack. @range 0..255 */
} Sd_InstanceUnicastRxPduType;

/*@req <ECUC_SD_00035> */
/**
 * @brief This container specifies all timers used by the Service Discovery module for
 *        Server Services.
 */
typedef struct Sd_ServerTimerTypeTag
{
    /*@req <ECUC_SD_00039> */
    uint32 SdServerTimerInitialOfferDelayMax; /**< Max value in [ms] to delay randomly the first offer.
                                                 @range 0..0xffffffff */

    /*@req <ECUC_SD_00038> */
    uint32 SdServerTimerInitialOfferDelayMin; /**< Min value in [ms] to delay randomly the first offer.
                                                 @range 0..0xffffffff */

    /*@req <ECUC_SD_00041> */
    uint32 SdServerTimerInitialOfferRepetitionBaseDelay; /**< The base delay in [ms] for offer repetitions. Successive
                                                            offers have an exponential back off delay (1x base delay, 2x
                                                            base delay, 4x base delay). @range 0..0xffffffff */

    /*@req <ECUC_SD_00076> */
    uint32 SdServerTimerOfferCyclicDelay; /**< Interval between cyclic offers in the main phase. @range 0..0xffffffff */

    /*@req <ECUC_SD_00114> */
    uint32 SdServerTimerRequestResponseMaxDelay; /**< Maximum allowable response delay to entries received by multicast
                                                    in seconds. @range 0..0xffffffff */

    /*@req <ECUC_SD_00115> */
    uint32 SdServerTimerRequestResponseMinDelay; /**< Minimum allowable response delay to entries received by multicast
                                                    in seconds. @range 0..0xffffffff */

    /*@req <ECUC_SD_00037> */
    uint32 SdServerTimerTTL; /**< Time to live for offer service. @range 0..0xffffffff */

    /*@req <ECUC_SD_00040> */
    uint8
        SdServerTimerInitialOfferRepetitionsMax; /**< Configure the maximum amount of offer repetition. @range 0..255 */
} Sd_ServerTimerType;

/**
 * @brief Configuration of soad socket connection.
 */
typedef struct Sd_SoAdSoConTypeTag
{
    SoAd_SoConIdType SoConId; /**< SoAd SoConId. @range 0..0x255 */
} Sd_SoAdSoConType;

/**
 * @brief This container specifies all parameters used by Service Groups.
 */
typedef struct Sd_ServiceGroupTypeTag
{
    uint16* ServerServiceRefPtr; /**< Contains all ServerServices associated with the ServiceGroup. @range NA */

    uint16* ClientServiceRefPtr; /**< Contains all ClientServices associated with the ServiceGroup. @range NA */

    /*@req <ECUC_SD_00135> */
    Sd_ServiceGroupIdType
        SdServiceGroupHandleId; /**< The numerical value used as the ID of this SdServiceGroup. @range 0..65535 */

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
    ApplicationType
        ServiceGroupApplicationId; /**< The ID of the partition where the ServiceGroup is located. @range 0..65535 */
#endif

    uint16 ServerServiceNum; /**< Number of ServerServices in this ServiceGroup. @range 0..65535 */

    uint16 ClientServiceNum; /**< Number of ClientServices in this ServiceGroup. @range 0..65535 */
} Sd_ServiceGroupType;

/*@req <ECUC_SD_00094> */
/**
 * @brief The subcontainer including the Routing Group for Activation of Events sent
 *        over Multicast.
 */
typedef struct Sd_EventHandlerMulticastTypeTag
{
    /*@req <ECUC_SD_00096> */
    const Sd_SoAdRoutingGroupType*
        SdEventActivationRef; /**< Reference to a SoAdRoutingGroup for activation of the data path for a subscribed
                                 client (start sending events after subscribe). @range NA */

    /*@req <ECUC_SD_00118> */
    const Sd_SoAdSoConType* SdMulticastEventSoConRef; /**< Reference to the SoAdSocketConnection representing the
                                                         multicast data path (UDP). @range NA */
} Sd_EventHandlerMulticastType;

/*@req <ECUC_SD_00093> */
/**
 * @brief The subcontainer including the Routing Groups for Activation and Trigger Transmit
 *        for Events sent over TCP.
 */
typedef struct Sd_EventHandlerTcpTypeTag
{
    /*@req <ECUC_SD_00096> */
    const Sd_SoAdRoutingGroupType*
        SdEventActivationRef; /**< Reference to a SoAdRoutingGroup for activation of the data path for a subscribed
                                 client (start sending events after subscribe). @range NA */

    /*@req <ECUC_SD_00095> */
    const Sd_SoAdRoutingGroupType*
        SdEventTriggeringRef; /**< Reference to a SoAdRoutingGroup that is used for triggered transmit. @range NA */
} Sd_EventHandlerTcpType;

/*@req <ECUC_SD_00092> */
/**
 * @brief The subcontainer including the Routing Groups for Activation and Trigger Transmit
 *        for Events sent over UDP.
 */
typedef struct Sd_EventHandlerUdpTypeTag
{
    /*@req <ECUC_SD_00096> */
    const Sd_SoAdRoutingGroupType*
        SdEventActivationRef; /**< Reference to a SoAdRoutingGroup for activation of the data path for a subscribed
                                 client (start sending events after subscribe). @range NA */
    /*@req <ECUC_SD_00095> */
    const Sd_SoAdRoutingGroupType*
        SdEventTriggeringRef; /**< Reference to a SoAdRoutingGroup that is used for triggered transmit. @range NA */
} Sd_EventHandlerUdpType;

/*@req <ECUC_SD_00055> */
/**
 * @brief Container Element for representing an EventGroup as part of the Service Instance.
 */
typedef struct Sd_EventHandlerTypeTag
{
    /*@req <ECUC_SD_00113> */
    const Sd_ServerTimerType*
        SdEventHandlerTimerRef; /**< The reference of the SdServerTimer container for this EventGroup. @range NA */

    const Sd_EventHandlerMulticastType* SdEventHandlerMulticast; /**< Container element for representing an EventGroup
                                                                    as part of the Service Instance. @range NA */

    const Sd_EventHandlerTcpType* SdEventHandlerTcp; /**< The subcontainer including the Routing Groups for Activation
                                                        and Trigger Transmit for Events sent over TCP. @range NA */

    const Sd_EventHandlerUdpType* SdEventHandlerUdp; /**< The subcontainer including the Routing Groups for Activation
                                                        and Trigger Transmit for Events sent over UDP. @range NA */
    /*@req <ECUC_SD_00061> */
    uint16 SdEventHandlerEventGroupId; /**< The EventGroup Id of this EventGroup as a unique identifier of the
                                          EventGroup in this service. This identifier is used for EventGroup entries as
                                          well. @range 0..65535 */
    /*@req <ECUC_SD_00112> */
    uint16 SdEventHandlerHandleId; /**< The HandleId by which the BswM can identify this EventGroup. @range 0..65535 */
    /*@req <ECUC_SD_00097> */
    uint16 SdEventHandlerMulticastThreshold; /**< Specifies the number of subscribed clients that trigger the Server to
                                                change the transmission of events to Multicast. @range 0..65535 */
} Sd_EventHandlerType;

/*@req <ECUC_SD_00087> */
/**
 * @brief Container element for representing the needed elements of the data path
 *        for the methods provided by the service.
 */
typedef struct Sd_ProvidedMethodsTypeTag
{
    /*@req <ECUC_SD_00090> */
    const Sd_SoAdRoutingGroupType*
        SdServerServiceActivationRef; /**< Reference to a SoAdRoutingGroup to activate and deactivate the data path for
                                         methods of the service. @range NA */
} Sd_ProvidedMethodsType;

/*@req <ECUC_SD_00004> */
/**
 * @brief This container specifies all parameters used by Server services.
 */
typedef struct Sd_ServerServiceTypeTag
{
    /*@req <ECUC_SD_00069> */
    uint32 SdServerServiceMinorVersion; /**< Minor version number of the Service as used e.g. in Offer Service entries.
                                           @range 0..0xffffffff */

    /*@req <ECUC_SD_00088> */
    const Sd_SoAdSoConGroupType*
        SdServerServiceTcpRef; /**< Reference to SoAdSocketConnectionGroup used for methods. This is used to access the
                                  local IP address and port for building the endpoint option for offers of this service.
                                  @range NA */

    /*@req <ECUC_SD_00086> */
    const Sd_ServerTimerType*
        SdServerServiceTimerRef; /**< The reference of the SdClientTimer container for this service. @range NA */

    /*@req <ECUC_SD_00089> */
    const Sd_SoAdSoConGroupType*
        SdServerServiceUdpRef; /**< Reference to SoAdSocketConnectionGroup used for methods. This is used to access the
                                  local IP address and port for building the endpoint option for offers of this service.
                                  @range NA */

    const Sd_EventHandlerType* SdEventHandler; /**< Container element for representing an EventGroup as part of the
                                                  Service Instance. @range NA */

    const Sd_ProvidedMethodsType* SdProvidedMethods; /**< Container element for representing the needed elements of the
                                                        data path for the methods provided by the service. @range NA */

    const Sd_CapabilityRecordType*
        SdServerCapabilityRecord; /**< Container element for representing the needed elements of the data path for the
                                     methods provided by the service. @range NA */

    uint16 EventHandlerNum;           /**< Number of event handlers. @range 0..65535 */
    uint16 ServerCapabilityRecordNum; /**< Number of server capability records. @range 0..65535 */
    uint16 ServiceGroupRefNum;        /**< Number of service group references. @range 0..65535 */

    /*@req <ECUC_SD_00110> */
    uint16 SdServerServiceHandleId; /**< The HandleId by which the BswM can identify this Server Service Instance.
                                       @range 0..65535 */

    /*@req <ECUC_SD_00009> */
    uint16
        SdServerServiceId; /**< Id to identify the service. This is unique for the service interface. @range 0..65535 */

    /*@req <ECUC_SD_00011> */
    uint16 SdServerServiceInstanceId; /**< Configuration parameter to specify Instance Id of the Service implemented by
                                         the Server Service. @range 0..65535 */

    /*@req <ECUC_SD_00126> */
    uint16
        SdServerCapabilityRecordMatchCalloutRef; /**< Reference to a SdCapabilityRecordMatchCallout. @range 0..65535 */

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
    ApplicationType ServerApplicationId; /**< ID of the partition where the ServerService is located. @range 0..65535 */
#endif

    /*@req <ECUC_SD_00068> */
    uint8 SdServerServiceMajorVersion; /**< Major version number of the Service as used in SD Entries. @range 0..255 */

    /*@req <ECUC_SoAd_00085> */
    boolean SdServerServiceAutoAvailable; /**< If existing and set to true, this Service will be set to "Available" on
                                             start. @range TRUE/FALSE */
} Sd_ServerServiceType;

/*@req <ECUC_SD_00084> */
/**
 * @brief This container represents an instance of the SD
 */
typedef struct Sd_InstanceTypeTag
{
    const Sd_ServerServiceType* SdServerService; /**< Reference to the server service configuration. @range NA */

    const Sd_ClientServiceType* SdClientService; /**< Reference to the client service configuration. @range NA */

#if (SD_DEM_USED == STD_ON)
    const Sd_InstanceDemEventParameterRefsType*
        SdInstanceDemEventParameterRefs; /**< References to DemEventParameter elements. @range NA */
#endif

    /*@req <ECUC_SD_00012> */
    const Sd_CharType* SdInstanceHostname; /**< Configuration parameter to specify the Hostname. @range NA */

    uint16 ServerServiceNum; /**< Number of server services. @range 0..65535 */
    uint16 ClientServiceNum; /**< Number of client services. @range 0..65535 */

    uint16 SdInstanceHandleId; /**< Handle ID by which the BswM can identify this instance. @range 0..65535 */

    TcpIp_DomainType SdAddrFamily; /**< TCP/IP domain type. @range NA */

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
    ApplicationType InstanceApplicationId; /**< ID of the partition where the instance is located. @range 0..65535 */
#endif

    /*@req <ECUC_SD_00128> */
    uint8 SdInstanceLocalAdressCheckLength; /**< Describes on how many bits of the addresses shall be compared to
                                               determine. @range 0..255 */

    SoAd_SoConIdType SdInstanceTxPduSoConId; /**< SoConId of the transmitted PDU in SoAd. @range 0..255 */

    PduIdType SdInstanceTxPduRef; /**< Reference to the transmitted PDU. @range 0..255 */

    PduIdType SdInstanceMulticastRxPdu; /**< Reference to the received multicast PDU. @range 0..255 */

    PduIdType SdInstanceUnicastRxPdu; /**< Reference to the received unicast PDU. @range 0..255 */

    uint8 SdOptionLength; /**< Length of options except configuration options. @range 0..255 */

    boolean
        SdInstanceLocalAdressCheckLengthEnable; /**< Enable flag for local address check length. @range TRUE/FALSE */
} Sd_InstanceType;

/**
 * @brief Mapping between RxPduId and SoConId.
 */
typedef struct Sd_RxPduIdSoConIdMapTypeTag
{
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
    ApplicationType RxPduApplicationId; /**< Application ID associated with the RxPdu. @range 0..65535 */
#endif
    PduIdType        RxPduId; /**< ID of the received PDU. @range 0..255 */
    SoAd_SoConIdType SoConId; /**< SoConId in SoAd. @range 0..255 */
} Sd_RxPduIdSoConIdMapType;

typedef struct Sd_ConvertMapTypeTag
{
    const Sd_HandlerIdMapType* handlerMapPtr;
    uint16                     mapLen;
} Sd_ConvertMapType;

typedef struct Sd_VariantHandlerIdConvertTypeTag
{
    const Sd_ConvertMapType* ServerHandlerIdMapPtr;        /**< Pointer to server handler ID map. @range NA */
    const Sd_ConvertMapType* ClientHandlerIdMapPtr;        /**< Pointer to client handler ID map. @range NA */
    const Sd_ConvertMapType* EventHandlerIdMapPtr;         /**< Pointer to event handler ID map. @range NA */
    const Sd_ConvertMapType* ConsumedEventHandlerIdMapPtr; /**< Pointer to consumed event handler ID map. @range NA */
    const Sd_ConvertMapType* ServiceGroupMapPtr;           /**< Pointer to service group ID map. @range NA */
} Sd_VariantHandlerIdConvertType;

/*@req <ECUC_SD_00003> */
/**
 * @brief This container contains the configuration parameters and sub containers of
 *        the AUTOSAR Service Discovery module.
 */
typedef struct Sd_ConfigTypeTag
{
    const Sd_InstanceType*      Instance;         /**< This container represents an instance of the SD. @range NA */
    const Sd_ServerServiceType* GlbServerService; /**< Global reference to server service configurations. @range NA */
    const Sd_ClientServiceType* GlbClientService; /**< Global reference to client service configurations. @range NA */
    const Sd_ServiceGroupType*  GlbServiceGroup;  /**< Global reference to service group configurations. @range NA */
    const Sd_ConsumedEventGroupType*
        GlbConsumedEventGroup; /**< Global reference to consumed event group configurations. @range NA */
    const Sd_RxPduIdSoConIdMapType*
        GlbRxPduIdSoConIdMap; /**< Global reference to RxPduId to SoConId mapping. @range NA */
    const SdCapabilityRecordMatchCalloutType*
        CapabilityRecordMatchCalloutRef; /**< Reference to a capability record match callout function. @range NA */
    const Sd_VariantHandlerIdConvertType* HandlerIdConvetMap;
    uint16 RxPduIdSoConIdMapLen; /**< Length of the RxPduId to SoConId mapping table. @range 0..65535 */
    uint16 ServiceGroupNum;      /**< Number of service groups. @range 0..65535 */
} Sd_ConfigType;

#endif

#ifdef __cplusplus
}
#endif

#endif
