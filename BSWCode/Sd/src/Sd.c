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
 **  @file               : Sd.c
 **  @author             : HuRongbo
 **  @date               : 2019-03-11
 **  @vendor             : isoft
 **  @description        : Implementation Of Sd
 **
 ***********************************************************************************************************************/
/* PRQA S 6540,6520,6510,3415 EOF */ /* VL_MTR_Sd_STTPP, VL_MTR_Sd_STVAR, VL_MTR_Sd_CONF,VL_Sd_3415 */
/* =================================================== inclusions =================================================== */

#include "Sd.h"
#if SD_MODULE_ENABLE == STD_ON
#if (SD_BSWM_USED == STD_ON)
#include "BswM_Sd.h"
#endif
#include "SoAd_Types.h"
#include "IStdLib.h"

/* ===================================================== macros ===================================================== */
/* use for utest*/
#if !defined(SD_LOCAL)
#define SD_LOCAL static /* PRQA S 3414 */ /* VL_QAC_FctLikeMacro */
#endif

/* inline keyword define,if compiler not support inline,define _SD_INLINE_ to null */
#if !defined(SD_LOCAL_INLINE)
#define SD_LOCAL_INLINE static inline
#endif

/* ========================================= internal function declarations ========================================= */
#define SD_START_SEC_CODE
#include "Sd_MemMap.h"

#if (SD_DEV_ERROR_DETECT == STD_ON)
SD_LOCAL_INLINE boolean Sd_ValidateInitStatus(uint8 apiId, ApplicationType applicationId);
SD_LOCAL_INLINE boolean Sd_ValidateInitResult(uint8 apiId, ApplicationType applicationId);
SD_LOCAL_INLINE boolean Sd_ValidatePointer(uint8 apiId, const void* pointer);
SD_LOCAL_INLINE boolean Sd_ValidateClientHandleId(uint8 apiId, uint16 clientServiceHandlerId);
SD_LOCAL_INLINE boolean Sd_ValidateServerHandleId(uint8 apiId, uint16 serverServiceHandlerId);
SD_LOCAL_INLINE boolean Sd_ValidateServerServiceState(uint8 apiId, Sd_ServerServiceSetStateType serverServiceState);
SD_LOCAL_INLINE boolean Sd_ValidateClientServiceState(uint8 apiId, Sd_ClientServiceSetStateType clientServiceState);
SD_LOCAL_INLINE boolean Sd_ValidateConsumedEventGroupHandleId(uint8 apiId, uint16 consumedEventGroupHandleId);
SD_LOCAL_INLINE boolean
    Sd_ValidateConsumedEventGroupState(uint8 apiId, Sd_ConsumedEventGroupSetStateType consumedEventGroupState);
SD_LOCAL_INLINE boolean Sd_ValidateSocketConnectId(uint8 apiId, SoAd_SoConIdType soConId);
SD_LOCAL_INLINE boolean Sd_ValidateIpAddrState(uint8 apiId, TcpIp_IpAddrStateType state);
SD_LOCAL_INLINE boolean Sd_ValidateRxPduId(uint8 apiId, PduIdType rxPduId);
SD_LOCAL_INLINE boolean Sd_ValidateSoConMode(uint8 apiId, SoAd_SoConModeType mode);
SD_LOCAL_INLINE boolean Sd_ValidateServiceGroupId(uint8 apiId, Sd_ServiceGroupIdType serviceGroupId);
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
SD_LOCAL_INLINE
boolean Sd_ValidatePartitionServerService(uint8 apiId, ApplicationType applicationId, uint16 serverServiceHandleId);
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionClientService(uint8 apiId, ApplicationType applicationId, uint16 clientServiceHandleId);
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionServiceGroup(uint8 apiId, ApplicationType applicationId, uint16 serviceGroupId);
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionConsumedEventGroup(uint8 apiId, ApplicationType applicationId, uint16 consumedEventGroupId);
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionTxSoConId(uint8 apiId, ApplicationType applicationId, SoAd_SoConIdType soConId);
SD_LOCAL_INLINE boolean Sd_ValidatePartitionRxPdu(uint8 apiId, ApplicationType applicationId, PduIdType rxPduId);
#endif
#endif

SD_LOCAL_INLINE
boolean Sd_GlobalHandlerIdToVariant(const Sd_ConvertMapType* map, uint16 srcHandlerId, uint16* outputHandlerId);

SD_LOCAL ApplicationType Sd_GetApplicationId(void);

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL Std_ReturnType Sd_EntryRefOptionFormatCheck(
    const Sd_InstanceType* instancePtr,
    const uint8*           sdMsg,
    uint16                 sdMsgOffset,
    uint16                 optionArrayLen,
    const uint16           optStartPosMap[]);

SD_LOCAL void Sd_ParseRxMessage(ApplicationType applicationId);

SD_LOCAL boolean Sd_RemoteRebootHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    boolean                isRxUseMulticast,
    ApplicationType        applicationId);

SD_LOCAL Std_ReturnType Sd_OptionsSecurityCheck(
    const Sd_InstanceType* instancePtr,
    const uint8*           sdMsg,
    uint16                 entryPos,
    const uint16           optStartPosMap[]);

SD_LOCAL Std_ReturnType
    Sd_IsIpAddrInSameSubset(const uint8* ipAddr1, const uint8* ipAddr2, uint8 netmask, uint8 addrLen);

SD_LOCAL Std_ReturnType Sd_OptionsConflictCheck(const uint8* sdMsg, uint16 entryPos, const uint16 optStartPosMap[]);

SD_LOCAL Std_ReturnType
    Sd_OptionsExistCheck(const Sd_InstanceType* instancePtr, const uint8* sdMsg, uint16 entryArrayLen, uint8 optNum);
#endif /* SD_SERVER_SERVICE_NUM > 0 || SD_CLIENT_SERVICE_NUM > 0 */

SD_LOCAL void Sd_DisableAllConsumedEventRouting(
    const Sd_ConsumedEventGroupType*   eventGroupPtr,
    const Sd_ClientServiceRTType*      cRtDataPtr,
    const Sd_ConsumedEventGroupRTType* cEgRTPtr);

#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_RxFindServiceHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 entryPos,
    boolean                isRxUseMulticast,
    const uint16           optStartPosMap[],
    ApplicationType        applicationId);

SD_LOCAL uint32 Sd_CalcServerServiceDelayTime(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    boolean                     isRxUseMulticast);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_RxOfferServiceHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 entryPos,
    const uint16           optStartPosMap[],
    ApplicationType        applicationId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL boolean Sd_IsOfferServiceBeIgnore(
    const Sd_ClientServiceRTType* cRtDataPtr,
    const Sd_ClientServiceType*   clientServicePtr,
    const Sd_RxQueueType*         rxQueueItem);
#endif

#if (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL void Sd_ResponseSubscribeEntry(
    const Sd_EventHandlerType*  eventHandlerPtr,
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint32                      ttl,
    boolean                     matchFlag,
    Std_ReturnType              result,
    boolean                     fanOutControlFlag,
    uint16                      entryPos,
    uint8                       counter,
    ApplicationType             applicationId);

SD_LOCAL void Sd_RxSubscribeEventgroupHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 entryPos,
    const uint16           optStartPosMap[],
    boolean                isReplyNAck,
    ApplicationType        applicationId);
#endif

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL void Sd_RxSubscribeEventgroupAckHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 entryPos,
    const uint16           optStartPosMap[],
    ApplicationType        applicationId);

#if (SD_CONSUMED_EVENT_MULICAST_USED == STD_ON)
SD_LOCAL Std_ReturnType Sd_GetInfoFromMulticastOption(
    const Sd_InstanceType* instancePtr,
    const uint8*           sdMsg,
    uint16                 entryPos,
    const uint16           optStartPosMap[],
    TcpIp_SockAddrType*    ipAddrPtr);
#endif

#if ((SD_DEM_USED == STD_ON) || (SD_CLIENT_SERVICE_USE_TCP == STD_ON))
SD_LOCAL boolean Sd_IsHaveEventGroupAckInSameMessage(
    const uint8* sdMsg,
    uint16       serviceID,
    uint16       instanceID,
    uint16       eventgroupID,
    uint8        majorVersion,
    uint8        counter);
#endif

SD_LOCAL void Sd_ClientStopSubscribeEventHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType*     cRtDataPtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ClientSubscribeEventHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType*     cRtDataPtr,
    ApplicationType             applicationId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL Std_ReturnType Sd_BuildFindServiceEntry(
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId);
#endif

SD_LOCAL boolean Sd_RxMessageOptionCheck(
    const Sd_InstanceType* instancePtr,
    const uint8*           sdMsg,
    uint16                 optStartPos[],
    uint16                 entryArrayLen,
    uint16                 optionArrayLen);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL Std_ReturnType Sd_BuildSubscribeEventgroupEntry(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    Sd_ClientServiceRTType*          cRtDataPtr,
    boolean                          isRxUseMulticast,
    uint32*                          sendTime,
    ApplicationType                  applicationId);

SD_LOCAL Std_ReturnType Sd_BuildStopSubscribeEventgroupEntry(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const TcpIp_SockAddrType*        remoteAddr,
    Sd_ClientServiceRTType*          cRtDataPtr,
    boolean                          isRxUseMulticast,
    ApplicationType                  applicationId);

SD_LOCAL uint32
    Sd_CalcClientServiceDelayTime(const Sd_ConsumedEventGroupType* eventHandlerPtr, boolean isRxUseMulticast);
#endif

#if (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL Std_ReturnType Sd_BuildSubscribeEventgroupAckEntry(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    const TcpIp_SockAddrType*   remoteAddr,
    uint32                      ttl,
    uint8                       counter,
    ApplicationType             applicationId);

SD_LOCAL Std_ReturnType Sd_BuildSubscribeEventgroupNAckEntry(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint16                      entryPos,
    ApplicationType             applicationId);
#endif

#if ((SD_SERVER_CAPABILITY_CFG_USED == STD_ON) || (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON))
SD_LOCAL uint16 Sd_GetType1ConfigurationOptionLength(
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr);

SD_LOCAL void Sd_BuildType1ConfigurationOption(
    uint8*                      optAddrPtr,
    uint16                      cfgOptLen,
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr);
#endif

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
#if (SD_CONSUMED_EVENTGROUP_NUM > 0) || (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL uint16 Sd_GetType2ConfigurationOptionLength(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ServerServiceType*      serverServicePtr,
    const Sd_EventHandlerType*       eventHandlerPtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr);

SD_LOCAL void Sd_BuildType2ConfigurationOption(
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    const Sd_ClientServiceType* clientServicePtr,
    uint8*                      optAddrPtr,
    uint16                      cfgOptLen);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0) || (SD_EVENT_HANDLER_NUM > 0 */

SD_LOCAL void
    Sd_SetHeader(uint8* headerPtr, uint16 instanceId, const TcpIp_SockAddrType* remoteAddr, boolean isMulticastTx);

SD_LOCAL void
    Sd_SetIPv4EndpointOption(uint8* ipv4OptionPtr, const uint8* ipv4Address, uint8 L4Proto, uint16 portNumber);

#if (STD_ON == SD_IPV6_ENABLE)
SD_LOCAL void
    Sd_SetIPv6EndpointOption(uint8* ipv6OptionPtr, const uint8* ipv6Address, uint8 L4Proto, uint16 portNumber);
#endif /* STD_ON == SD_IPV6_ENABLE */

SD_LOCAL_INLINE void Sd_SetTTL(Sd_TTLType* ttlPtr, uint32 ttl);
#endif

#if ((SD_EVENT_HANDLER_MULICAST_USED == STD_ON) || (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON))
SD_LOCAL void Sd_SetIPv4MulticastOption(uint8* ipv4OptionPtr, const uint8* ipv4Address, uint16 portNumber);
#endif

#if ((SD_EVENT_HANDLER_MULICAST_USED == STD_ON) && (STD_ON == SD_IPV6_ENABLE))
SD_LOCAL void Sd_SetIPv6MulticastOption(uint8* ipv6OptionPtr, const uint8* ipv6Address, uint16 portNumber);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientServiceDownPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ClientServiceInitialWaitPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ClientServiceRepetitionPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ProcessRxEnrty(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 index,
    const uint16           optStartPos[],
    uint8                  entryType,
    boolean                isRxUseMulticast,
    boolean                isReplyNAck,
    ApplicationType        applicationId);

#if (                                                                              \
    (SD_CLIENT_SERVICE_USE_TCP == STD_ON) || (SD_CLIENT_SERVICE_USE_UDP == STD_ON) \
    || (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON))
SD_LOCAL void Sd_ClientSoConModeChgHandler(
    const Sd_ClientServiceType*  clientServicePtr,
    const Sd_SoAdSoConGroupType* socketGroupRef,
    SoAd_SoConIdType             SoConId);
#endif

SD_LOCAL void Sd_ClientEnableAllMethodRoutingGroup(
    const Sd_ClientServiceType*   clientServicePtr,
    const Sd_ClientServiceRTType* cRtDataPtr);

SD_LOCAL void Sd_ClientServiceMainPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void
    Sd_ClientServiceInitialWaitEnter(const Sd_ClientServiceType* clientServicePtr, Sd_ClientServiceRTType* cRtDataPtr);

#if ((SD_SERVER_SERVICE_NUM > 0) && (SD_SERVER_SERVICE_USE_TCP == STD_ON))
SD_LOCAL void Sd_ServerSoConModeChgHandle(
    const Sd_InstanceType* instancePtr,
    SoAd_SoConIdType       SoConId,
    ApplicationType        applicationId);
#endif

SD_LOCAL void Sd_ClientServiceDownPhaseEnter(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType*     cRtDataPtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ClientServiceDisableAllRoutingOfClient(
    const Sd_ClientServiceType*   clientServicePtr,
    const Sd_ClientServiceRTType* cRtDataPtr);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL void Sd_ClientServiceDisableAllConsumedEventgroupRouting(
    const Sd_ConsumedEventGroupType* cEgPtr,
    const Sd_ClientServiceRTType*    cRtDataPtr);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

SD_LOCAL void
    Sd_ClientServiceCloseAllSoCon(const Sd_ClientServiceType* clientServicePtr, Sd_ClientServiceRTType* cRtDataPtr);

SD_LOCAL void Sd_ClientServiceCloseSoCon(Sd_SoConManageType* soConPtr);
#endif

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
SD_LOCAL_INLINE void Sd_EndianSwap(uint8* dataPtr, uint8 size);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

SD_LOCAL uint32 Sd_GetRandomValue(uint32 min, uint32 max);

SD_LOCAL uint32 Sd_NtoHl(uint32 net32);

SD_LOCAL uint32 Sd_NPtrtoHl(const uint8* net32DataPtr);

SD_LOCAL uint32 Sd_HtoNl(uint32 host32);

SD_LOCAL uint16 Sd_NtoHs(uint16 net16);

SD_LOCAL uint16 Sd_NPtrtoHs(const uint8* net16DataPtr);

SD_LOCAL uint16 Sd_HtoNs(uint16 host16);

SD_LOCAL_INLINE uint8 Sd_InitMemoryPool(uint32 memPoolSize, void* memPool);

/* PRQA S 5016 ++ */ /* VL_Sd_ReturnValue */
SD_LOCAL_INLINE void* Sd_MallocEx(uint32 size, void* memPool);
/* PRQA S 5016 -- */

SD_LOCAL_INLINE void Sd_FreeEx(const void* ptr, void* memPool);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL void Sd_BuildSubscribeOption(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const Sd_ClientServiceRTType*    cRtDataPtr,
    uint8*                           optCnt,
    uint8*                           optionPtr,
    uint16                           optStartPos[],
    uint16                           bufOffset,
    uint16                           cfgOptLen,
    boolean                          isStopSubscribe);
#endif

#if (SD_INSTANCE_NUM == STD_ON)
SD_LOCAL void Sd_AppendToRxBuffTail(Sd_RxQueueType* rxQueueItemPtr, ApplicationType applicationId);
#endif

SD_LOCAL void
    Sd_ClientTimerExpireHandler(const Sd_ClientServiceType* clientServicePtr, Sd_ClientServiceRTType* cRtDataPtr);

SD_LOCAL void Sd_ConsumedEventGroupTimerHandler(
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const Sd_ClientServiceRTType*    cRtDataPtr);

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
SD_LOCAL void Sd_ConsumedEventGroupRetryStatusHandler(const Sd_ConsumedEventGroupType* eventGroupPtr);
#endif

SD_LOCAL boolean Sd_RxMessageMatchInstance(const Sd_RxQueueType* rxQueueItem, ApplicationType applicationId);

#if (SD_EVENT_HANDLER_NUM > 0)

static void Sd_AddClientToTriggerTransmitList(
    Sd_TriggerTransmitManagerType** head,
    SoAd_RoutingGroupIdType         routingGroupId,
    SoAd_SoConIdType                soConId,
    ApplicationType                 applicationId);

static void Sd_DelClientFromTriggerTransmitListBySoConId(
    Sd_TriggerTransmitManagerType** head,
    SoAd_SoConIdType                soConId,
    ApplicationType                 applicationId);

static Sd_TriggerTransmitManagerType* Sd_PopFirstClientFromTriggerTransmitList(Sd_TriggerTransmitManagerType** head);

/* PRQA S 5016 ++ */ /* VL_Sd_ReturnValue */
SD_LOCAL Sd_SubscribeManageType* Sd_GetSubscribeManageObjAddr(
    Sd_SubscribeManageType* header,
    TcpIp_SockAddrType      sockRemoteAddr,
    TcpIp_SockAddrType      clientIpAddr,
    uint8                   counter);
/* PRQA S 5016 -- */

#if (SD_EVENT_HANDLER_MULICAST_USED == STD_ON)
SD_LOCAL void
    Sd_EnableUnicastForSubscribedClient(const Sd_SubscribeManageType* header, SoAd_RoutingGroupIdType routingGroupId);

SD_LOCAL void
    Sd_DisableUnicastForSubscribedClient(const Sd_SubscribeManageType* header, SoAd_RoutingGroupIdType routingGroupId);
#endif

SD_LOCAL Std_ReturnType Sd_AddClientToSubscribeList(
    Sd_SubscribeManageType** headerPtr,
    SoAd_SoConIdType         soConId,
    TcpIp_SockAddrType       sockRemoteAddr,
    TcpIp_SockAddrType       clientIpAddr,
    uint32                   ttl,
    uint8                    counter,
    ApplicationType          applicationId);

SD_LOCAL void Sd_DeleteClientFromSubscribeList(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    Sd_SubscribeManageType**    headerPtr,
    const TcpIp_SockAddrType*   clientIpAddr,
    SoAd_SoConIdType            soConId,
    uint8                       counter,
    ApplicationType             applicationId);

SD_LOCAL void Sd_RemoveAllClientFromSubscribeList(Sd_SubscribeManageType** headerPtr, ApplicationType applicationId);

SD_LOCAL void Sd_UpdateSubscribeClientTTL(Sd_SubscribeManageType* subscribePtr, uint32 ttl);

SD_LOCAL void Sd_SubscribeClientTTLManage(const Sd_ServerServiceType* serverServicePtr, ApplicationType applicationId);

#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
SD_LOCAL Std_ReturnType Sd_FanOutControlTCP(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    TcpIp_SockAddrType          clientIpAddr,
    const uint8*                sdMsg,
    uint16                      entryPos,
    const uint16                optStartPosMap[],
    ApplicationType             applicationId);
#endif

#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
SD_LOCAL Std_ReturnType Sd_FanOutControlUDP(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    TcpIp_SockAddrType          clientIpAddr,
    const uint8*                sdMsg,
    uint16                      entryPos,
    const uint16                optStartPosMap[],
    ApplicationType             applicationId);
#endif

#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
SD_LOCAL Std_ReturnType Sd_FanOutControlTcpHandle(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    const Sd_IPv4OptionsType*   ipv4OptPtr,
    const Sd_IPv6OptionsType*   ipv6OptPtr,
    TcpIp_SockAddrType          clientIpAddr,
    uint32                      ttl,
    uint8                       counter,
    ApplicationType             applicationId);
#endif

#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
SD_LOCAL Std_ReturnType Sd_FanOutControlUdpHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    const Sd_IPv4OptionsType*   ipv4OptPtr,
    const Sd_IPv6OptionsType*   ipv6OptPtr,
    TcpIp_SockAddrType          clientIpAddr,
    uint32                      ttl,
    uint8                       counter,
    ApplicationType             applicationId);
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
SD_LOCAL void Sd_ClientServiceDownPhaseEnableUdp(
    const Sd_ClientServiceType*   clientServicePtr,
    Sd_ClientServiceRTType*       cRtDataPtr,
    const Sd_ConsumedMethodsType* consumedMethodsPtr);
#endif

#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
SD_LOCAL void Sd_ClientServiceDownPhaseEnableTcp(
    const Sd_ClientServiceType*   clientServicePtr,
    Sd_ClientServiceRTType*       cRtDataPtr,
    const Sd_ConsumedMethodsType* consumedMethodsPtr);
#endif

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
SD_LOCAL void Sd_ClientServiceDownPhaseEnableMulitcast(
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType*     cRtDataPtr);
#endif

#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
SD_LOCAL void Sd_UpdateSubscribeSoConId(
    Sd_SubscribeManageType* headerPtr,
    SoAd_SoConIdType        soConId,
    SoAd_SoConIdType        assignSoConId);

SD_LOCAL void Sd_FanOutMulticastUnicastCtrlAddClient(
    const Sd_EventHandlerType* eventHandlerPtr,
    Sd_EventHandlerRTType*     ehRTDataPtr,
    SoAd_SoConIdType           assignedSoConId,
    ApplicationType            applicationId);
#endif

SD_LOCAL void Sd_FanOutMulticastUnicastCtrlDelClient(
    const Sd_EventHandlerType*   eventHandlerPtr,
    const Sd_EventHandlerRTType* ehRTDataPtr,
    SoAd_SoConIdType             soConId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientSaveSoConId(
    const Sd_ClientServiceType* clientServicePtr,
    const TcpIp_SockAddrType*   remoteAddrPtr,
    boolean                     isUseTcp,
    Sd_SoConManageType*         soConPtr);

#if ((SD_CONSUMED_EVENTGROUP_NUM > 0) && (SD_CONSUMED_EVENT_MULICAST_USED == STD_ON))
SD_LOCAL void Sd_ClientSaveMulticastSoConId(
    const Sd_InstanceType*           instancePtr,
    const Sd_ConsumedEventGroupType* consumedEventGroupPtr,
    const TcpIp_SockAddrType*        multicastAddrPtr,
    const TcpIp_SockAddrType*        serverAddrPtr,
    Sd_SoConManageType*              soConPtr);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

SD_LOCAL void Sd_ClientHandleSoConBasedOptionInfo(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const uint8*                sdMsg,
    uint16                      entryPos,
    const uint16                optStartPosMap[],
    Sd_ClientServiceRTType*     cRtDataPtr);

SD_LOCAL void Sd_ClientTimerHandle(const Sd_ClientServiceType* clientServicePtr);

#endif

SD_LOCAL void Sd_SetEndpointOption(
    const Sd_InstanceType* sdInstancePtr,
    uint8                  sdL4ProtoType,
    SoAd_SoConIdType       soConId,
    uint8*                 bufPtr,
    uint16                 bufRunIdx);

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
SD_LOCAL void Sd_SetMulicastEndpointOption(
    const Sd_InstanceType* sdInstancePtr,
    SoAd_SoConIdType       soConId,
    uint8*                 bufPtr,
    uint16                 bufRunIdx);
#endif

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void Sd_AppendToSendQueueTail(Sd_SendQueueType** head, Sd_SendQueueType* sendQueueItem);

SD_LOCAL void Sd_ClearSendQueue(Sd_SendQueueType** head, ApplicationType applicationId);

SD_LOCAL void Sd_CalcTcpUdpEndpointOptionNum(
    const uint8* sdMsg,
    uint16       entryPos,
    const uint16 optStartPosMap[],
    uint8*       tcpEndpointNum,
    uint8*       udpEndpointNum);

SD_LOCAL Std_ReturnType Sd_NecessaryOptionsExistCheck(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint16                      entryPos,
    const uint16                optStartPosMap[],
    ApplicationType             applicationId);

SD_LOCAL Std_ReturnType Sd_PackageMessage(
    Sd_SendQueueType* const sendQueue[],
    uint8                   sendQueCnt,
    PduInfoType*            pduInfoPtr,
    uint16                  instanceId,
    boolean                 isMulticastTx,
    ApplicationType         applicationId);

SD_LOCAL void Sd_TransmitMulticastMessage(
    const Sd_InstanceType*  instancePtr,
    Sd_SendQueueType* const multiSendQue[],
    uint8                   multiSendCnt,
    ApplicationType         applicationId);

SD_LOCAL void Sd_TransmitUnicastMessage(
    const Sd_InstanceType*  instancePtr,
    Sd_SendQueueType* const uniSendQue[],
    uint8                   uniSendCnt,
    ApplicationType         applicationId);

SD_LOCAL void Sd_FindMulticastMsgToSendFromSendQueue(
    Sd_SendQueueType** queueHeaderPtr,
    Sd_SendQueueType*  multiSendQue[],
    uint8*             multiSendCntPtr);

SD_LOCAL void Sd_FindUnicastMsgToSendFromSendQueue(
    Sd_SendQueueType** queueHeaderPtr,
    Sd_SendQueueType*  uniSendQue[],
    uint8*             uniSendCntPtr);

SD_LOCAL uint8 Sd_FindoutMulticastMessage(const Sd_InstanceType* instancePtr, Sd_SendQueueType* multiSendQue[]);

SD_LOCAL uint8 Sd_FindoutUnicastMessage(const Sd_InstanceType* instancePtr, Sd_SendQueueType* uniSendQue[]);

SD_LOCAL void Sd_TransmitMessage(ApplicationType applicationId);
#endif

#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
SD_LOCAL boolean Sd_RxConfigurationOptionMatchCheck(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint16                      entryPos,
    const uint16*               optStartPosMap,
    ApplicationType             applicationId);
#endif

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void Sd_SaveRemoteSessionId(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, boolean isRxUseMulticast);
#endif

#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_SaveServerServiceHandleIdToSessionCtrlList(
    uint16                    instanceId,
    const TcpIp_SockAddrType* remoteAddrPtr,
    uint16                    serverServiceHandleId,
    ApplicationType           applicationId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_SaveClientServiceHandleIdToSessionCtrlList(
    uint16                    instanceId,
    const TcpIp_SockAddrType* remoteAddrPtr,
    uint16                    clientServiceHandleId,
    ApplicationType           applicationId);
#endif

/* PRQA S 5016 ++ */ /* VL_Sd_ReturnValue */
SD_LOCAL Sd_SessionIdCtrlType*
    Sd_FindRemoteAddrSessionIdCtrl(uint16 instanceId, const TcpIp_SockAddrType* remoteAddrPtr);

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL Sd_SessionIdCtrlType* Sd_AddRemoteAddrToSessionIdCtrlList(
    uint16                instanceId,
    const Sd_RxQueueType* rxQueueItem,
    boolean               isRxUseMulticast,
    ApplicationType       applicationId);
#endif
/* PRQA S 5016 -- */

SD_LOCAL boolean Sd_FindHandlerIdInList(uint16 handlerId, const Sd_ServiceHandleIdListType* serviceList);

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL boolean Sd_IsNewRemoteAddr(uint16 instanceId, const TcpIp_SockAddrType* remoteAddrPtr);
#endif

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL boolean Sd_RemoteRebootDetecte(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, boolean isRxUseMulticast);

SD_LOCAL void
    Sd_FreeServiceListInSesIdCtrlList(const Sd_ServiceHandleIdListType* serviceList, ApplicationType applicationId);
#endif

SD_LOCAL void Sd_DeleteSessionIdNode(
    Sd_SessionIdCtrlType**      head,
    const Sd_SessionIdCtrlType* objNode,
    ApplicationType             applicationId);

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void
    Sd_RemoteRebootDetectedHandle(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, ApplicationType applicationId);
#endif

SD_LOCAL boolean Sd_IsLeastOneSoConValid(const Sd_ServerServiceType* serverServicePtr);

SD_LOCAL void Sd_CloseAllSoCon(const Sd_ServerServiceType* serverServicePtr);

#if (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL void Sd_RebootStopSubscribeEventHandle(
    uint16                serverServiceHandleId,
    const Sd_RxQueueType* rxQueueItem,
    ApplicationType       applicationId);
#endif /* SD_EVENT_HANDLER_NUM > 0 */

SD_LOCAL Std_ReturnType Sd_OptionFormatCheck(const uint8 sdMsg[]);

SD_LOCAL Std_ReturnType Sd_OptionEachItemFormatCheck(const uint8 optionArray[]);

SD_LOCAL void Sd_IfSpecificRoutingGroupTransmit(ApplicationType applicationId);

SD_LOCAL uint8 Sd_GetClientNumFromSubscribeList(
    const Sd_SubscribeManageType* headerPtr,
    const TcpIp_SockAddrType*     clientIpAddr,
    SoAd_SoConIdType              soConId,
    uint8                         counter);

#if (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL void
    Sd_ResetSoConToWildcard(const Sd_ServerServiceType* serverServicePtr, SoAd_SoConIdType soConId, boolean isTcpSoCon);
#endif /* SD_EVENT_HANDLER_NUM > 0 */

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
#if ((SD_DEM_USED == STD_ON) || (SD_CLIENT_SERVICE_USE_TCP == STD_ON))
SD_LOCAL void Sd_RxSubscribeEventgroupNAckHandle(
    const Sd_InstanceType*        instancePtr,
    const Sd_ClientServiceRTType* cRtDataPtr,
    const Sd_ClientServiceType*   clientServicePtr);
#endif

#if (SD_CONSUMED_EVENT_MULICAST_USED == STD_ON)
SD_LOCAL void Sd_SubscribeAckOptionHandle(
    const Sd_InstanceType*           instancePtr,
    const Sd_ConsumedEventGroupType* consumedEventGroupPtr,
    const Sd_ClientServiceRTType*    cRtDataPtr,
    Sd_ConsumedEventGroupRTType*     consumedEgRTPtr,
    const uint8*                     sdMsg,
    uint16                           entryPos,
    const uint16                     optStartPosMap[]);
#endif

SD_LOCAL void Sd_ClientServiceResetAllCEgSubsEntrySendFlag(
    const Sd_ClientServiceType* clientServicePtr,
    boolean                     tcpFlag,
    boolean                     udpFlag);

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
SD_LOCAL void Sd_ClientServiceResetAllCEgSubsEntrySubsRetryCnt(const Sd_ClientServiceType* clientServicePtr);
#endif /* STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE */
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
SD_LOCAL void Sd_UpdateUdpSubscribeList(
    const Sd_EventHandlerType*  eventHandlerPtr,
    Sd_SubscribeManageType*     subscribeManagePtr,
    Sd_EventHandlerRTType*      ehRTDataPtr,
    const Sd_ServerServiceType* serverServicePtr,
    uint32                      ttl,
    SoAd_SoConIdType            assignedSoConId,
    SoAd_SoConIdType            soConId,
    boolean                     isMulticastSocket,
    uint8                       counter,
    ApplicationType             applicationId);
#endif

#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
SD_LOCAL Std_ReturnType Sd_FanoutTcpAddSubscriber(
    const Sd_EventHandlerType* eventHandlerPtr,
    SoAd_SoConIdType           soConId,
    Sd_EventHandlerRTType*     ehRTDataPtr,
    TcpIp_SockAddrType         sockAddr,
    TcpIp_SockAddrType         clientIpAddr,
    uint32                     ttl,
    uint8                      counter,
    ApplicationType            applicationId);

SD_LOCAL void Sd_FanoutTcpUpdateSubscriber(
    const Sd_EventHandlerType* eventHandlerPtr,
    SoAd_SoConIdType           soConId,
    Sd_SubscribeManageType*    subscribeManagePtr,
    Sd_EventHandlerRTType*     ehRTDataPtr,
    uint32                     ttl);

SD_LOCAL void Sd_FanoutTcpDeleteSubscriber(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    TcpIp_SockAddrType          sockAddr,
    TcpIp_SockAddrType          clientIpAddr,
    uint8                       counter,
    ApplicationType             applicationId);
#endif

#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_ServerServiceInit(ApplicationType applicationId);

SD_LOCAL void Sd_ServerMainFunction(const Sd_InstanceType* instancePtr, ApplicationType applicationId);

SD_LOCAL void Sd_BuildOfferServiceEntry(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint32                      sendTime,
    boolean                     isUnicastSend,
    boolean                     isStopOfferService,
    ApplicationType             applicationId);

/* PRQA S 5016 ++ */ /* VL_Sd_ReturnValue */
SD_LOCAL Sd_Type1EntryType* Sd_CreateOfferServiceEntry(
    const Sd_ServerServiceType* serverServicePtr,
    boolean                     isStopOfferService,
    ApplicationType             applicationId);

SD_LOCAL uint8* Sd_CreateOfferServiceOptions(
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    Sd_SendQueueType*           sendQueueObjPtr,
    ApplicationType             applicationId);
/* PRQA S 5016 -- */

SD_LOCAL void Sd_ServerServiceDownPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ServerServiceInitialWaitPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ServerServiceRepetitionPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ServerServiceMainPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ServerServiceDownPhaseEnter(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId);

SD_LOCAL void Sd_ServerServiceTimerHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientServiceInit(ApplicationType applicationId);
SD_LOCAL void Sd_ClientMainFunction(const Sd_InstanceType* instancePtr, ApplicationType applicationId);
SD_LOCAL void Sd_RxStopOfferServiceHandle(uint16 clientServiceHandleId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientSoConModeChg(const Sd_InstanceType* instancePtr, SoAd_SoConIdType SoConId);
#endif

#if (SD_INSTANCE_NUM > 0)
SD_LOCAL void Sd_InstanceInit(ApplicationType applicationId);
#endif
#if (SD_SERVER_SERVICE_GROUP_USED == STD_ON)
SD_LOCAL void Sd_ServiceGroupStopServer(Sd_ServiceGroupIdType serviceGroupId);
#endif
#if (SD_CLIENT_SERVICE_GROUP_USED == STD_ON)
SD_LOCAL void Sd_ServiceGroupStopClient(Sd_ServiceGroupIdType serviceGroupId);
#endif
#if (SD_DEM_USED == STD_ON)
SD_LOCAL void Sd_RxIndicationReportDem(PduIdType RxPduId);
#endif

SD_LOCAL_INLINE uint32 Sd_InitialWaitTimeAdjust(uint32 waitTime);

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
static void Sd_ClientSubscribeRetryHandler(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    Sd_ConsumedEventGroupRTType*     consumedEgRTPtr,
    Sd_ClientServiceRTType*          cRtDataPtr,
    ApplicationType                  applicationId);
#endif

#if (STD_ON == SD_DEV_ERROR_DETECT)
/**
 * @brief           Reports an error using the DET module.
 * @param[in]       apiId   API ID.
 * @param[in]       errorId Error ID.
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace           CPD-74175
 */
SD_LOCAL_INLINE void Sd_ReportError(uint8 apiId, uint8 errorId)
{
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, apiId, errorId);
}
#endif

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
/*
 * @brief           Reports a runtime error using the DET module.
 * @param[in]       apiId   API ID.
 * @param[in]       errorId Error ID.
 * @reentrant       TRUE
 * @synchronous     TRUE
 * @trace           CPD-74176
 */
SD_LOCAL_INLINE void Sd_ReportRuntimeError(uint8 apiId, uint8 errorId)
{
    (void)Det_ReportRuntimeError(SD_MODULE_ID, SD_INSTANCE_ID, apiId, errorId);
}
#endif

#define SD_STOP_SEC_CODE
#include "Sd_MemMap.h"

/* ========================================== internal function definition ========================================== */
#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Sd_MemMap.h"
/** Random seed */
SD_LOCAL uint32 Sd_RandomSeed;
#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Sd_MemMap.h"

#define SD_START_SEC_VAR_INIT_PTR
#include "Sd_MemMap.h"
/** Pointer to the configuration */
SD_LOCAL const Sd_ConfigType* Sd_CfgPtr = NULL_PTR;
#define SD_STOP_SEC_VAR_INIT_PTR
#include "Sd_MemMap.h"

/* ========================================== external function definition ========================================== */
#define SD_START_SEC_CODE
#include "Sd_MemMap.h"
/* PRQA S 1532,1503 ++ */ /* VL_QAC_OneFunRef,VL_QAC_NoUsedApi */
/**
 * Initializes the Service Discovery.
 */
void Sd_Init(const Sd_ConfigType* ConfigPtr)
{
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidatePointer(SD_INIT_ID, ConfigPtr))
#endif
    {
        Sd_CfgPtr = ConfigPtr;

        Sd_InitRxQueueHead(applicationId);
#if (SD_SERVER_SERVICE_NUM > 0)
        Sd_ServerServiceInit(applicationId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
        Sd_ClientServiceInit(applicationId);
#endif

#if (SD_INSTANCE_NUM > 0)
        Sd_InstanceInit(applicationId);
#endif

        /* Init memory pool*/
        uint8 memPoolRet = Sd_InitMemoryPool(SD_TX_MEMORY_POOL_SIZE, Sd_TxMemPool[applicationId]);

        Sd_InitResultType initResult1 = SD_INIT_FAIL;
        Sd_InitResultType initResult2 = SD_INIT_FAIL;
        if (ISTDLIB_MEMHEAP_OK == memPoolRet)
        {
            initResult1 = SD_INIT_SUCCESS;
        }
        memPoolRet = Sd_InitMemoryPool(SD_RX_MEMORY_POOL_SIZE, Sd_RxMemPool[applicationId]);
        if (ISTDLIB_MEMHEAP_OK == memPoolRet)
        {
            initResult2 = SD_INIT_SUCCESS;
        }

        /*@req <SWS_SD_00122> */
        /* remember internally the successful initialization for other API
         * functions to check for proper module initialization. */
        if ((SD_INIT_SUCCESS == initResult1) && (SD_INIT_SUCCESS == initResult2))
        {
            *Sd_InitResult[applicationId] = SD_INIT_SUCCESS;
        }
        else
        {
            *Sd_InitResult[applicationId] = SD_INIT_FAIL;
        }

        /* Set SD Status */
        *Sd_Status[applicationId] = SD_INIT;
    }

    return;
}

/**
 * Returns the version information of this module.
 */
/*@req <SWS_SD_00126> */
#if (SD_VERSION_INFO_API == STD_ON)
void Sd_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidatePointer(SD_GET_VERSION_INFO_ID, versioninfo))
#endif
    {
        /*@req <SWS_Sd_00125> */
        versioninfo->vendorID         = SD_VENDOR_ID;
        versioninfo->moduleID         = SD_MODULE_ID;
        versioninfo->sw_major_version = SD_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = SD_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = SD_SW_PATCH_VERSION;
    }

    return;
}
#endif

/**
 * This API function is used by the BswM to set the Server Service Instance state.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Sd_STCAL */
Std_ReturnType Sd_ServerServiceSetState(uint16 SdServerServiceHandleId, Sd_ServerServiceSetStateType ServerServiceState)
/* PRQA S 6070 -- */
{
    Std_ReturnType  ret           = E_NOT_OK;
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidateInitStatus(SD_SERVER_SERVICE_SET_STATE_ID, applicationId)
        && Sd_ValidateInitResult(SD_SERVER_SERVICE_SET_STATE_ID, applicationId)
        && Sd_ValidateServerHandleId(SD_SERVER_SERVICE_SET_STATE_ID, SdServerServiceHandleId)
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        && Sd_ValidatePartitionServerService(SD_SERVER_SERVICE_SET_STATE_ID, applicationId, SdServerServiceHandleId)
#endif
        && Sd_ValidateServerServiceState(SD_SERVER_SERVICE_SET_STATE_ID, ServerServiceState))
#endif
    {
#if (SD_SERVER_SERVICE_NUM > 0)
        uint16  actualHandlerId = 0u;
        boolean bConvertResult  = Sd_GlobalHandlerIdToVariant(
            Sd_CfgPtr->HandlerIdConvetMap->ServerHandlerIdMapPtr,
            SdServerServiceHandleId,
            &actualHandlerId);

        if (bConvertResult)
        {
            const Sd_ServerServiceType* serverService = &Sd_CfgPtr->GlbServerService[actualHandlerId];
            /*@req <SWS_Sd_00746> */
            /* The service refer to at least one SdServiceGroup that shall only be changed by
            Sd_ServiceGroupStart and Sd_ServiceGroupStop. */

            /* SWS_Sd_00323 */
#if (SD_EVENT_HANDLER_NUM > 0u)
            const Sd_EventHandlerType* eventHandlerPtr = serverService->SdEventHandler;
            Sd_EventHandlerRTType*     ehRtData        = Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
            for (uint16 i = 0u; i < serverService->EventHandlerNum; i++)
            {
                if (SD_SERVER_SERVICE_AVAILABLE != ServerServiceState)
                {
                    SchM_Enter_Sd_ExclusiveArea();
                    ehRtData->CurState = SD_EVENT_HANDLER_RELEASED;
#if (SD_BSWM_USED == STD_ON)
                    BswM_Sd_EventHandlerCurrentState(
                        eventHandlerPtr->SdEventHandlerHandleId,
                        SD_EVENT_HANDLER_RELEASED);
#endif
                    SchM_Exit_Sd_ExclusiveArea();
                }
                ehRtData++;
            }
#endif

            if (serverService->ServiceGroupRefNum == 0u)
            {
                SchM_Enter_Sd_ExclusiveArea();
                Sd_ServerRTData[actualHandlerId]->SetState = ServerServiceState;
                SchM_Exit_Sd_ExclusiveArea();
            }

            ret = E_OK;
        }
#endif
    }

    return ret;
}

/**
 * This API function is used by the BswM to set the Client Service Instance state.
 */
/* PRQA S 6070 ++ */ /* VL_MTR_Sd_STCAL */
Std_ReturnType Sd_ClientServiceSetState(uint16 ClientServiceHandleId, Sd_ClientServiceSetStateType ClientServiceState)
/* PRQA S 6070 -- */
{
    Std_ReturnType  ret           = E_NOT_OK;
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidateInitStatus(SD_CLIENT_SERVICE_SET_STATE_ID, applicationId)
        && Sd_ValidateInitResult(SD_CLIENT_SERVICE_SET_STATE_ID, applicationId)
        && Sd_ValidateClientHandleId(SD_CLIENT_SERVICE_SET_STATE_ID, ClientServiceHandleId)
        && Sd_ValidateClientServiceState(SD_CLIENT_SERVICE_SET_STATE_ID, ClientServiceState)
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        && Sd_ValidatePartitionClientService(SD_CLIENT_SERVICE_SET_STATE_ID, applicationId, ClientServiceHandleId)
#endif
    )
#endif
    {
#if (SD_CLIENT_SERVICE_NUM > 0)
        uint16  actualHandlerId = 0u;
        boolean bConvertResult  = Sd_GlobalHandlerIdToVariant(
            Sd_CfgPtr->HandlerIdConvetMap->ClientHandlerIdMapPtr,
            ClientServiceHandleId,
            &actualHandlerId);

        if (bConvertResult)
        {
            const Sd_ClientServiceType*      clientService         = &Sd_CfgPtr->GlbClientService[actualHandlerId];
            const Sd_ConsumedEventGroupType* consumedEventGroupPtr = clientService->SdConsumedEventGroup;

            /*@req <SWS_Sd_00443> */
            if (ClientServiceState != SD_CLIENT_SERVICE_REQUESTED)
            {
                SchM_Enter_Sd_ExclusiveArea();
                for (uint16 index = 0u; index < clientService->ConsumedEventGroupNum; index++)
                {
                    Sd_ConsumedEventGroupRTType* eventGroupRtDataPtr =
                        Sd_ConsumedEventGroupRTData[consumedEventGroupPtr->SdConsumedEventGroupHandleId];
                    eventGroupRtDataPtr->SetState = SD_CONSUMED_EVENTGROUP_RELEASED;
                }
                SchM_Exit_Sd_ExclusiveArea();
            }

            if (clientService->ServiceGroupRefNum == 0u)
            {
                SchM_Enter_Sd_ExclusiveArea();
                Sd_ClientRTData[actualHandlerId]->SetState = ClientServiceState;
                SchM_Exit_Sd_ExclusiveArea();
            }

            ret = E_OK;
        }

#endif
    }

    return ret;
}

/**
 * Starts a preconfigured SdServiceGroup.
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Sd_STMIF */
void Sd_ServiceGroupStart(Sd_ServiceGroupIdType ServiceGroupId)
/* PRQA S 6030 -- */
{
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidateInitStatus(SD_SERVICE_GROUP_START_ID, applicationId)
        && Sd_ValidateServiceGroupId(SD_SERVICE_GROUP_START_ID, ServiceGroupId)
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        && Sd_ValidatePartitionServiceGroup(SD_SERVICE_GROUP_START_ID, applicationId, ServiceGroupId)
#endif
    )
#endif
    {
        uint16  actualServiceGroupId = 0u;
        boolean bConvertResult       = Sd_GlobalHandlerIdToVariant(
            Sd_CfgPtr->HandlerIdConvetMap->ServiceGroupMapPtr,
            ServiceGroupId,
            &actualServiceGroupId);

        if (bConvertResult)
        {
            for (uint16 index = 0u; index < Sd_CfgPtr->ServiceGroupNum; index++)
            {
                if (Sd_CfgPtr->GlbServiceGroup[index].SdServiceGroupHandleId == actualServiceGroupId)
                {
                    SchM_Enter_Sd_ExclusiveArea();
                    uint16 serverServiceNum = Sd_CfgPtr->GlbServiceGroup[index].ServerServiceNum;
                    for (uint16 serviceIndex = 0u; serviceIndex < serverServiceNum; serviceIndex++)
                    {
                        /*@req <SWS_Sd_00749>*/
                        uint16 serverHandleId = Sd_CfgPtr->GlbServiceGroup[index].ServerServiceRefPtr[serviceIndex];
                        Sd_ServerRTData[serverHandleId]->SetState = SD_SERVER_SERVICE_AVAILABLE;
                        Sd_ServerRTData[serverHandleId]->ServiceGroupRequestCounter++;
                    }

                    uint16 clientServiceNum = Sd_CfgPtr->GlbServiceGroup[index].ClientServiceNum;
                    for (uint16 serviceIndex = 0u; serviceIndex < clientServiceNum; serviceIndex++)
                    {
                        /*@req <SWS_Sd_00749>*/
                        uint16 clientHandleId = Sd_CfgPtr->GlbServiceGroup[index].ClientServiceRefPtr[serviceIndex];
                        Sd_ClientRTData[clientHandleId]->SetState = SD_CLIENT_SERVICE_REQUESTED;
                        Sd_ClientRTData[clientHandleId]->ServiceGroupRequestCounter++;
                    }
                    SchM_Exit_Sd_ExclusiveArea();

                    break;
                }
            }
        }
    }
}

/**
 * Stops a preconfigured SdServiceGroup.
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
void Sd_ServiceGroupStop(Sd_ServiceGroupIdType ServiceGroupId)
{
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidateInitStatus(SD_SERVICE_GROUP_STOP_ID, applicationId)
        && Sd_ValidateServiceGroupId(SD_SERVICE_GROUP_STOP_ID, ServiceGroupId)
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        && Sd_ValidatePartitionServiceGroup(SD_SERVICE_GROUP_STOP_ID, applicationId, ServiceGroupId)
#endif
    )
#endif
    {
        uint16  actualServiceGroupId = 0u;
        boolean bConvertResult       = Sd_GlobalHandlerIdToVariant(
            Sd_CfgPtr->HandlerIdConvetMap->ServiceGroupMapPtr,
            ServiceGroupId,
            &actualServiceGroupId);

        if (bConvertResult)
        {
            if (Sd_CfgPtr->ServiceGroupNum <= actualServiceGroupId)
            {
                return;
            }
#if (SD_SERVER_SERVICE_GROUP_USED == STD_ON)
            Sd_ServiceGroupStopServer(actualServiceGroupId);
#endif

#if (SD_CLIENT_SERVICE_GROUP_USED == STD_ON)
            Sd_ServiceGroupStopClient(actualServiceGroupId);
#endif
        }
    }
}
/* PRQA S 2889 -- */

/**
 * This API function is used by the BswM to set the requested state of the EventGroupStatus.
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_Sd_STMIF,VL_MTR_Sd_STCAL */
Std_ReturnType Sd_ConsumedEventGroupSetState(
    uint16                            SdConsumedEventGroupHandleId,
    Sd_ConsumedEventGroupSetStateType ConsumedEventGroupState)
/* PRQA S 6030,6070 -- */
{
    Std_ReturnType  ret           = E_NOT_OK;
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidateInitStatus(SD_CLIENT_SERVICE_SET_STATE_ID, applicationId)
        && Sd_ValidateInitResult(SD_CONSUMED_EVENT_GROUP_SET_STATE_ID, applicationId)
        && Sd_ValidateConsumedEventGroupHandleId(SD_CONSUMED_EVENT_GROUP_SET_STATE_ID, SdConsumedEventGroupHandleId)
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        && Sd_ValidatePartitionConsumedEventGroup(
            SD_CONSUMED_EVENT_GROUP_SET_STATE_ID,
            applicationId,
            SdConsumedEventGroupHandleId)
#endif
        && Sd_ValidateConsumedEventGroupState(SD_CONSUMED_EVENT_GROUP_SET_STATE_ID, ConsumedEventGroupState))
#endif
    {
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        uint16  actualHandlerId = 0u;
        boolean bConvertResult  = Sd_GlobalHandlerIdToVariant(
            Sd_CfgPtr->HandlerIdConvetMap->ConsumedEventHandlerIdMapPtr,
            SdConsumedEventGroupHandleId,
            &actualHandlerId);

        if (bConvertResult)
        {
            const Sd_ClientServiceType*      cServicePtr;
            const Sd_ConsumedEventGroupType* consumedEgPtr;
            Sd_ConsumedEventGroupRTType*     consumedEgRTPtr;
            uint16                           i, j;

            cServicePtr = Sd_CfgPtr->GlbClientService;
            for (i = 0u; i < SD_CLIENT_SERVICE_NUM; i++)
            {
                consumedEgPtr = cServicePtr->SdConsumedEventGroup;
                for (j = 0u; j < cServicePtr->ConsumedEventGroupNum; j++)
                {
                    if (actualHandlerId == consumedEgPtr->SdConsumedEventGroupHandleId)
                    {
                        ret = E_OK;
                        break;
                    }
                    consumedEgPtr++;
                }

                if (E_OK == ret)
                {
                    break;
                }
                cServicePtr++; /* PRQA S 2983 */ /* VL_Sd_PointerCalu */
            }

            if (E_OK == ret)
            {
                const Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[cServicePtr->SdClientServiceHandleId];
                if (SD_CLIENT_SERVICE_RELEASED != cRtDataPtr->SetState)
                {
                    SchM_Enter_Sd_ExclusiveArea();
                    consumedEgRTPtr           = Sd_ConsumedEventGroupRTData[actualHandlerId];
                    consumedEgRTPtr->SetState = ConsumedEventGroupState;
                    if (SD_CONSUMED_EVENTGROUP_REQUESTED == ConsumedEventGroupState)
                    {
                        consumedEgRTPtr->IsSendSubEvtAfterRequest = FALSE;
                    }
                    SchM_Exit_Sd_ExclusiveArea();
                }
                else
                {
                    /*@req <SWS_SD_00442> */
                    ret = E_NOT_OK;
                }
            }
        }
#endif
    }

    return ret;
}
#define SD_STOP_SEC_CODE
#include "Sd_MemMap.h"

#define SD_START_SEC_CODE_FAST
#include "Sd_MemMap.h"

/**
 * This function gets called by the SoAd if an IP address
 * assignment related to a socket connection changes (i.e.
 * new address assigned or assigned address becomes invalid).
 */
void Sd_LocalIpAddrAssignmentChg(SoAd_SoConIdType SoConId, TcpIp_IpAddrStateType State)
{
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidateInitStatus(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, applicationId)
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        && Sd_ValidatePartitionTxSoConId(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, applicationId, SoConId)
#endif
        && Sd_ValidateInitResult(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, applicationId)
        && Sd_ValidateSocketConnectId(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, SoConId)
        && Sd_ValidateIpAddrState(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, State))
#endif
    {
#if (SD_INSTANCE_NUM == STD_ON)
        SoAd_SoConIdType txPduSoConId;
        uint16           idx;

        for (idx = 0u; idx < SD_INSTANCE_NUM; idx++)
        {
            txPduSoConId = Sd_CfgPtr->Instance[idx].SdInstanceTxPduSoConId;
            if (txPduSoConId == SoConId)
            {
                Sd_InstanceRTData[idx]->IpAddrState = State;
            }
        }
#endif
    }
}

/**
 * Notification about a SoAd socket connection state change, e.g. socket connection gets online.
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
void Sd_SoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode)
{
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidateInitStatus(SD_SOCON_MODE_CHG_ID, applicationId)
        && Sd_ValidateInitResult(SD_SOCON_MODE_CHG_ID, applicationId)
        && Sd_ValidateSocketConnectId(SD_SOCON_MODE_CHG_ID, SoConId)
        && Sd_ValidateSoConMode(SD_SOCON_MODE_CHG_ID, Mode))
#endif
    {
#if (SD_INSTANCE_NUM == STD_ON)
        if (SOAD_SOCON_ONLINE != Mode)
        {
            const Sd_InstanceType* instancePtr = Sd_CfgPtr->Instance;
            for (uint16 index = 0u; index < SD_INSTANCE_NUM; index++)
            {
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
                if (instancePtr->InstanceApplicationId != applicationId)
                {
                    continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
                }
#endif

#if ((SD_SERVER_SERVICE_NUM > 0) && (SD_SERVER_SERVICE_USE_TCP == STD_ON))
                Sd_ServerSoConModeChgHandle(instancePtr, SoConId, applicationId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
                Sd_ClientSoConModeChg(instancePtr, SoConId);
#endif
                instancePtr++; /* PRQA S 2983 */ /* VL_Sd_PointerCalu */
            }
        }
#endif
    }
}

/**
 * Indication of a received I-PDU from a lower layer communication interface module.
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_Sd_STMIF,VL_MTR_Sd_STCAL */
void Sd_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
/* PRQA S 6030,6070 -- */
{
    ApplicationType applicationId = Sd_GetApplicationId();

#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (Sd_ValidateInitStatus(SD_RX_INDICATION_ID, applicationId)
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        && Sd_ValidatePartitionRxPdu(SD_RX_INDICATION_ID, applicationId, RxPduId)
#endif
        && Sd_ValidateInitResult(SD_RX_INDICATION_ID, applicationId)
        && Sd_ValidatePointer(SD_RX_INDICATION_ID, PduInfoPtr) && Sd_ValidateRxPduId(SD_RX_INDICATION_ID, RxPduId))
#endif
    {
#if (SD_INSTANCE_NUM == STD_ON)
        Sd_HeaderType header;
        const uint8*  sdMsgPtr = PduInfoPtr->SduDataPtr;

        (void)IStdLib_MemCpy((uint8*)&header, sdMsgPtr, sizeof(Sd_HeaderType));
        /* Header field check: Protocol Version = 0x01, Interface Version = 0x01,
         * Message Types = 0x02, Return Code = 0x00 */
        if ((PduInfoPtr->SduLength < SD_ENTRIES_ARRAY_LENGTH_INDEX) || (header.ProtocolVersion != 0x01u)
            || (header.InterfaceVersion != 0x01u) || (header.MessageType != SD_MESSAGE_TYPE_NOTIFY)
            || (header.ReturnCode != 0x00u))
        {
#if (SD_DEM_USED == STD_ON)
            Sd_RxIndicationReportDem(RxPduId);
#endif
            return;
        }

        uint16 entryArrayLen  = (uint16)Sd_NPtrtoHl(&sdMsgPtr[SD_ENTRIES_ARRAY_LENGTH_INDEX]);
        uint16 optionArrayLen = (uint16)Sd_NPtrtoHl(
            &sdMsgPtr[SD_ENTRIES_ARRAY_LENGTH_INDEX + entryArrayLen + SD_MESSAGE_ENTRYLEN_FILED_LEN]);
        /*@req <SWS_SD_00708> */
        PduLengthType minSduLen = (PduLengthType)(SD_HEADER_LEN + SD_MESSAGE_ENTRYLEN_FILED_LEN + entryArrayLen
                                                  + optionArrayLen + SD_MESSAGE_OPTIONLEN_FILED_LEN);
        if (PduInfoPtr->SduLength >= minSduLen)
        {
            /* @req SWS_SD_00482 */
            /* Received message check pass,continue process */
            /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
            Sd_RxQueueType* rxQueuePtr =
                (Sd_RxQueueType*)Sd_MallocEx(sizeof(Sd_RxQueueType), Sd_RxMemPool[applicationId]);
            uint8* rxBufPtr = (uint8*)Sd_MallocEx(PduInfoPtr->SduLength, Sd_RxMemPool[applicationId]);
            /* PRQA S 0316 -- */
            if ((NULL_PTR != rxQueuePtr) && (NULL_PTR != rxBufPtr))
            {
                /* Get remote address */
                const Sd_RxPduIdSoConIdMapType* rxPduIdSoConIdMap = Sd_CfgPtr->GlbRxPduIdSoConIdMap;
                for (uint16 index = 0u; index < Sd_CfgPtr->RxPduIdSoConIdMapLen; index++)
                {
                    if (rxPduIdSoConIdMap->RxPduId == RxPduId)
                    {
                        const Sd_InstanceType* instancePtr = Sd_CfgPtr->Instance;
                        TcpIp_SockAddrType     tcpIpSockAddr;
                        for (uint16 instanceIndex = 0u; instanceIndex < SD_INSTANCE_NUM; instanceIndex++)
                        {
                            if ((instancePtr[instanceIndex].SdInstanceMulticastRxPdu == RxPduId)
                                || (instancePtr[instanceIndex].SdInstanceUnicastRxPdu == RxPduId))
                            {
                                tcpIpSockAddr.domain = instancePtr[instanceIndex].SdAddrFamily;
                                break;
                            }
                        }
                        (void)SoAd_GetRemoteAddr(rxPduIdSoConIdMap->SoConId, &tcpIpSockAddr);
                        (void)IStdLib_MemCpy(&rxQueuePtr->RemoteAddr, &tcpIpSockAddr, sizeof(tcpIpSockAddr));

                        /* Reset the SoCon back to Wildcard using SoAd_ReleaseRemoteAddr() */
                        SoAd_ReleaseRemoteAddr(rxPduIdSoConIdMap->SoConId);
                        break;
                    }
                    rxPduIdSoConIdMap++;
                }

                /* Copy sdu to receive buffer */
                (void)IStdLib_MemCpy(rxBufPtr, sdMsgPtr, PduInfoPtr->SduLength);

                /* Save info to Queue */
                rxQueuePtr->BufPtr    = rxBufPtr;
                rxQueuePtr->RxPduId   = RxPduId;
                rxQueuePtr->PduLength = PduInfoPtr->SduLength;
                rxQueuePtr->Next      = NULL_PTR;

                Sd_AppendToRxBuffTail(rxQueuePtr, applicationId);
            }
        }
#if (SD_DEM_USED == STD_ON)
        else
        {
            Sd_RxIndicationReportDem(RxPduId);
        }
#endif
#endif
    }
}
/* PRQA S 2889 -- */
#define SD_STOP_SEC_CODE_FAST
#include "Sd_MemMap.h"

#define SD_START_SEC_CODE
#include "Sd_MemMap.h"

/**
 * @brief              Handles changes in the socket connection mode for server services.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          SoConId: Socket connection ID.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74558
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Sd_STMIF */
#if ((SD_SERVER_SERVICE_NUM > 0) && (SD_SERVER_SERVICE_USE_TCP == STD_ON))
SD_LOCAL void Sd_ServerSoConModeChgHandle(
    const Sd_InstanceType* instancePtr,
    SoAd_SoConIdType       SoConId,
    ApplicationType        applicationId)
/* PRQA S 6030 -- */
{
    const Sd_ServerServiceType* serverServicePtr = instancePtr->SdServerService;
    for (uint16 serverIdx = 0u; serverIdx < instancePtr->ServerServiceNum; serverIdx++)
    {
        const Sd_SoAdSoConGroupType* serverServiceTcpRef = serverServicePtr->SdServerServiceTcpRef;
        if (NULL_PTR != serverServiceTcpRef)
        {
            uint16                  soConIdNum  = serverServiceTcpRef->SoConNumInGroup;
            const SoAd_SoConIdType* soConIdList = serverServiceTcpRef->SocketIdList;
            for (uint16 soConIndex = 0u; soConIndex < soConIdNum; soConIndex++)
            {
                if (SoConId != soConIdList[soConIndex])
                {
                    continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
                }

                const Sd_ServerServiceRTType* sRtDataPtr = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
                if ((SD_SERVER_PHASE_MAIN == sRtDataPtr->Phase) || (SD_SERVER_PHASE_REPETITION == sRtDataPtr->Phase))
                {
                    /* @req SWS_SD_00732,SWS_SD_00733 */
                    const Sd_EventHandlerType* eventHandlerPtr = serverServicePtr->SdEventHandler;
                    for (uint16 eventIndex = 0u; eventIndex < serverServicePtr->EventHandlerNum; eventIndex++)
                    {
                        Sd_EventHandlerRTType* eventRtData =
                            Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
                        const Sd_SubscribeManageType* subscribeList = eventRtData->TcpSubscribeList;

                        uint8 clientNum = Sd_GetClientNumFromSubscribeList(
                            subscribeList,
                            NULL_PTR,
                            SoConId,
                            SD_SUBSCRIBE_COUNTER_FIELD_RESERVED);
                        if (clientNum != 0u)
                        {
                            SoAd_RoutingGroupIdType routingGroupId =
                                eventHandlerPtr->SdEventHandlerTcp->SdEventActivationRef->RoutingGroupId;

                            eventRtData->TcpSubsClientNum--;
                            Sd_DeleteClientFromSubscribeList(
                                serverServicePtr,
                                eventHandlerPtr,
                                &eventRtData->TcpSubscribeList,
                                NULL_PTR,
                                SoConId,
                                SD_SUBSCRIBE_COUNTER_FIELD_RESERVED,
                                applicationId);
                            (void)SoAd_DisableSpecificRouting(routingGroupId, SoConId);
                            break;
                        }
                        eventHandlerPtr++;
                    }

                    (void)SoAd_CloseSoCon(SoConId, TRUE);
                    (void)SoAd_OpenSoCon(SoConId);
                }
                break;
            }
        }
        serverServicePtr++;
    }
}
#endif

/* PRQA S 6030 ++ */ /* VL_MTR_Sd_STMIF */
#if (                                                                              \
    (SD_CLIENT_SERVICE_USE_TCP == STD_ON) || (SD_CLIENT_SERVICE_USE_UDP == STD_ON) \
    || (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON))
SD_LOCAL void Sd_ClientSoConModeChgHandler(
    const Sd_ClientServiceType*  clientServicePtr,
    const Sd_SoAdSoConGroupType* socketGroupRef,
    SoAd_SoConIdType             SoConId)
/* PRQA S 6030 -- */
{
    if (NULL_PTR != socketGroupRef)
    {
        uint16                  soConIdNum  = socketGroupRef->SoConNumInGroup;
        const SoAd_SoConIdType* soConIdList = socketGroupRef->SocketIdList;
        for (uint16 soConIndex = 0u; soConIndex < soConIdNum; soConIndex++)
        {
            if (SoConId == soConIdList[soConIndex])
            {
                Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
                if ((SD_CLIENT_PHASE_MAIN == cRtDataPtr->Phase) || (SD_CLIENT_PHASE_REPETITION == cRtDataPtr->Phase))
                {
                    /* @req SWS_Sd_00730,SWS_Sd_00731 */
                    Sd_ClientServiceInitialWaitEnter(clientServicePtr, cRtDataPtr);

                    Sd_SetTTL(&cRtDataPtr->Ttl, 0u);
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
                    const Sd_ConsumedEventGroupType* cEgPtr = clientServicePtr->SdConsumedEventGroup;
                    Sd_ConsumedEventGroupRTType*     consumedEventGroupPtr =
                        Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
                    for (uint16 eventIndex = 0u; eventIndex < clientServicePtr->ConsumedEventGroupNum; eventIndex++)
                    {
                        Sd_SetTTL(&consumedEventGroupPtr->Ttl, 0u);
                        consumedEventGroupPtr++;
                    }
#endif
                }
                break; /* PRQA S 0771 */ /* VL_Sd_Break */
            }
        }
    }
}
#endif

/**
 * @brief              Handles changes in the socket connection mode for client services.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          SoConId: Socket connection ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74557
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientSoConModeChg(const Sd_InstanceType* instancePtr, SoAd_SoConIdType SoConId)
{
    const Sd_ClientServiceType* clientServicePtr = instancePtr->SdClientService;
    for (uint16 clientIdx = 0u; clientIdx < instancePtr->ClientServiceNum; clientIdx++)
    {
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
        if (NULL_PTR != clientServicePtr->SdClientServiceTcpRef)
        {
            Sd_ClientSoConModeChgHandler(clientServicePtr, clientServicePtr->SdClientServiceTcpRef, SoConId);
        }
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
        if (NULL_PTR != clientServicePtr->SdClientServiceUdpRef)
        {
            Sd_ClientSoConModeChgHandler(clientServicePtr, clientServicePtr->SdClientServiceUdpRef, SoConId);
        }
#endif

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
        if (NULL_PTR != clientServicePtr->SdClientServiceMulticastRef)
        {
            Sd_ClientSoConModeChgHandler(clientServicePtr, clientServicePtr->SdClientServiceMulticastRef, SoConId);
        }
#endif

        clientServicePtr++;
    }
}

/**
 * @brief              Main function for client services.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74560
 */
SD_LOCAL void Sd_ClientMainFunction(const Sd_InstanceType* instancePtr, ApplicationType applicationId)
{
    const Sd_ClientServiceType* clientServicePtr = instancePtr->SdClientService;
    if (NULL_PTR != clientServicePtr)
    {
        for (uint16 clientIdx = 0u; clientIdx < instancePtr->ClientServiceNum; clientIdx++)
        {
            Sd_ClientTimerHandle(clientServicePtr);
            switch (Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId]->Phase)
            {
            case SD_CLIENT_PHASE_DOWN:
                Sd_ClientServiceDownPhaseHandle(instancePtr, clientServicePtr, applicationId);
                break;

            case SD_CLIENT_PHASE_INITIAL_WAIT:
                Sd_ClientServiceInitialWaitPhaseHandle(instancePtr, clientServicePtr, applicationId);
                break;

            case SD_CLIENT_PHASE_REPETITION:
                Sd_ClientServiceRepetitionPhaseHandle(instancePtr, clientServicePtr, applicationId);
                break;

            case SD_CLIENT_PHASE_MAIN:
                Sd_ClientServiceMainPhaseHandle(instancePtr, clientServicePtr, applicationId);
                break;

            default:
                /* Wrong Phase */
                break;
            }

            clientServicePtr++;
        }
    }
}
#endif

/**
 * @brief              Main function for server services.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74555
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_ServerMainFunction(const Sd_InstanceType* instancePtr, ApplicationType applicationId)
{
    const Sd_ServerServiceType* serverServicePtr = instancePtr->SdServerService;
    if (NULL_PTR != serverServicePtr)
    {
        for (uint16 serverIdx = 0u; serverIdx < instancePtr->ServerServiceNum; serverIdx++)
        {
            Sd_ServerServiceTimerHandle(instancePtr, serverServicePtr, applicationId);

            switch (Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId]->Phase)
            {
            case SD_SERVER_PHASE_DOWN:
                Sd_ServerServiceDownPhaseHandle(instancePtr, serverServicePtr, applicationId);
                break;

            case SD_SERVER_PHASE_INITIAL_WAIT:
                Sd_ServerServiceInitialWaitPhaseHandle(instancePtr, serverServicePtr, applicationId);
                break;

            case SD_SERVER_PHASE_REPETITION:
                Sd_ServerServiceRepetitionPhaseHandle(instancePtr, serverServicePtr, applicationId);
                break;

            case SD_SERVER_PHASE_MAIN:
                Sd_ServerServiceMainPhaseHandle(instancePtr, serverServicePtr, applicationId);
                break;

            default:
                /* Wrong Phase */
                break;
            }
            serverServicePtr++;
        }
    }
}
#endif

/**
 * @brief              SD module main funtion
 * @reentrant          Reentrant for different partition.
 * @synchronous        TRUE
 * @trace              CPD-74026
 */
void Sd_MainFunction(void)
{
    ApplicationType applicationId = Sd_GetApplicationId();

#if (SD_INSTANCE_NUM > 0)
    if (SD_INIT_SUCCESS == *Sd_InitResult[applicationId])
    {
        /* Random calculate seed increment */
        Sd_RandomSeed++;

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
        Sd_ParseRxMessage(applicationId);
#endif

        for (uint16 instanceIdx = 0u; instanceIdx < SD_INSTANCE_NUM; instanceIdx++)
        {
            const Sd_InstanceType* instancePtr = &Sd_CfgPtr->Instance[instanceIdx];

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
            if (applicationId != instancePtr->InstanceApplicationId)
            {
                continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
            }
#endif

#if (SD_SERVER_SERVICE_NUM > 0)
            Sd_ServerMainFunction(instancePtr, applicationId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
            Sd_ClientMainFunction(instancePtr, applicationId);
#endif
        }

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
        Sd_TransmitMessage(applicationId);

        /* PATCH for TC8 */
        Sd_IfSpecificRoutingGroupTransmit(applicationId);
#endif
    }
#endif
}
/* PRQA S 1532,1503 -- */
/* ========================================== internal function definition ========================================== */

#if (SD_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief               Validates a pointer for NULL.
 * @param[in]           apiId API ID.
 * @param[in]           pointer Pointer to validate.
 * @return              TRUE if the pointer is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74043
 */
SD_LOCAL_INLINE boolean Sd_ValidatePointer(uint8 apiId, const void* pointer)
{
    boolean bRet = TRUE;
    if (NULL_PTR == pointer)
    {
        Sd_ReportError(apiId, SD_E_PARAM_POINTER);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates the initialization status of the SD module.
 * @param[in]           apiId API ID.
 * @param[in]           applicationId Application ID.
 * @return              TRUE if the initialization status is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74041
 */
SD_LOCAL_INLINE boolean Sd_ValidateInitStatus(uint8 apiId, ApplicationType applicationId)
{
    boolean bRet = TRUE;
    if (SD_UNINIT == *Sd_Status[applicationId])
    {
        Sd_ReportError(apiId, SD_E_UNINIT);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates the initialization result of the SD module.
 * @param[in]           apiId API ID.
 * @param[in]           applicationId Application ID.
 * @return              TRUE if the initialization result is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74042
 */
SD_LOCAL_INLINE boolean Sd_ValidateInitResult(uint8 apiId, ApplicationType applicationId)
{
    boolean bRet = TRUE;
    if (SD_INIT_FAIL == *Sd_InitResult[applicationId])
    {
        Sd_ReportError(apiId, SD_E_INIT_FAILED);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a pointer for NULL.
 * @param[in]           apiId API ID.
 * @param[in]           clientServiceHandlerId Client service handle ID.
 * @return              TRUE if the pointer is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74044
 */
SD_LOCAL_INLINE boolean Sd_ValidateClientHandleId(uint8 apiId, uint16 clientServiceHandlerId)
{
    boolean bRet = TRUE;
    if (clientServiceHandlerId >= SD_CLIENT_SERVICE_NUM)
    {
        Sd_ReportError(apiId, SD_E_INV_ID);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a server handle ID.
 * @param[in]           apiId API ID.
 * @param[in]           serverServiceHandlerId Server service handle ID.
 * @return              TRUE if the server handle ID is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74045
 */
SD_LOCAL_INLINE boolean Sd_ValidateServerHandleId(uint8 apiId, uint16 serverServiceHandlerId)
{
    boolean bRet = TRUE;
    if (serverServiceHandlerId >= SD_SERVER_SERVICE_NUM)
    {
        Sd_ReportError(apiId, SD_E_INV_ID);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a client service state.
 * @param[in]           apiId API ID.
 * @param[in]           clientServiceState Client service state.
 * @return              TRUE if the client service state is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74047
 */
SD_LOCAL_INLINE boolean Sd_ValidateClientServiceState(uint8 apiId, Sd_ClientServiceSetStateType clientServiceState)
{
    boolean bRet = TRUE;
    if ((SD_CLIENT_SERVICE_RELEASED != clientServiceState) && (SD_CLIENT_SERVICE_REQUESTED != clientServiceState))
    {
        Sd_ReportError(apiId, SD_E_INV_MODE);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a server service state.
 * @param[in]           apiId API ID.
 * @param[in]           serverServiceState Server service state.
 * @return              TRUE if the server service state is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74046
 */
SD_LOCAL_INLINE boolean Sd_ValidateServerServiceState(uint8 apiId, Sd_ServerServiceSetStateType serverServiceState)
{
    boolean bRet = TRUE;
    if ((SD_SERVER_SERVICE_DOWN != serverServiceState) && (SD_SERVER_SERVICE_AVAILABLE != serverServiceState))
    {
        Sd_ReportError(apiId, SD_E_INV_MODE);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a consumed event group handle ID.
 * @param[in]           apiId API ID.
 * @param[in]           consumedEventGroupHandleId Consumed event group handle ID.
 * @return              TRUE if the consumed event group handle ID is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74048
 */
SD_LOCAL_INLINE boolean Sd_ValidateConsumedEventGroupHandleId(uint8 apiId, uint16 consumedEventGroupHandleId)
{
    boolean bRet = TRUE;
    if (consumedEventGroupHandleId >= SD_CONSUMED_EVENTGROUP_NUM)
    {
        Sd_ReportError(apiId, SD_E_INV_ID);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a consumed event group state.
 * @param[in]           apiId API ID.
 * @param[in]           consumedEventGroupState Consumed event group state.
 * @return              TRUE if the consumed event group state is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74049
 */
SD_LOCAL_INLINE boolean
    Sd_ValidateConsumedEventGroupState(uint8 apiId, Sd_ConsumedEventGroupSetStateType consumedEventGroupState)
{
    boolean bRet = TRUE;
    if ((SD_CONSUMED_EVENTGROUP_RELEASED != consumedEventGroupState)
        && (SD_CONSUMED_EVENTGROUP_REQUESTED != consumedEventGroupState))
    {
        Sd_ReportError(apiId, SD_E_INV_MODE);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a socket connection ID.
 * @param[in]           apiId API ID.
 * @param[in]           soConId Socket connection ID.
 * @return              TRUE if the socket connection ID is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74050
 */
SD_LOCAL_INLINE boolean Sd_ValidateSocketConnectId(uint8 apiId, SoAd_SoConIdType soConId)
{
    boolean bRet = TRUE;
    if (soConId >= SOAD_SO_CON_MAX)
    {
        Sd_ReportError(apiId, SD_E_INV_ID);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates an IP address state.
 * @param[in]           apiId API ID.
 * @param[in]           state IP address state.
 * @return              TRUE if the IP address state is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74051
 */
SD_LOCAL_INLINE boolean Sd_ValidateIpAddrState(uint8 apiId, TcpIp_IpAddrStateType state)
{
    boolean bRet = TRUE;

    if ((TCPIP_IPADDR_STATE_ASSIGNED != state) && (TCPIP_IPADDR_STATE_ONHOLD != state)
        && (TCPIP_IPADDR_STATE_UNASSIGNED != state))
    {
        Sd_ReportError(apiId, SD_E_INV_MODE);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates an Rx PDU ID.
 * @param[in]           apiId API ID.
 * @param[in]           rxPduId Rx PDU ID.
 * @return              TRUE if the Rx PDU ID is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74052
 */
SD_LOCAL_INLINE boolean Sd_ValidateRxPduId(uint8 apiId, PduIdType rxPduId)
{
    boolean bRet = TRUE;

    if (rxPduId >= Sd_CfgPtr->RxPduIdSoConIdMapLen)
    {
        Sd_ReportError(apiId, SD_E_INV_ID);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a socket connection mode.
 * @param[in]           apiId API ID.
 * @param[in]           mode Socket connection mode.
 * @return              TRUE if the socket connection mode is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74053
 */
SD_LOCAL_INLINE boolean Sd_ValidateSoConMode(uint8 apiId, SoAd_SoConModeType mode)
{
    boolean bRet = TRUE;

    if ((SOAD_SOCON_ONLINE != mode) && (SOAD_SOCON_RECONNECT != mode) && (SOAD_SOCON_OFFLINE != mode))
    {
        Sd_ReportError(apiId, SD_E_INV_MODE);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a service group ID.
 * @param[in]           apiId API ID.
 * @param[in]           serviceGroupId Service group ID.
 * @return              TRUE if the service group ID is valid, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74054
 */
SD_LOCAL_INLINE boolean Sd_ValidateServiceGroupId(uint8 apiId, Sd_ServiceGroupIdType serviceGroupId)
{
    boolean bRet = TRUE;

    if (serviceGroupId >= Sd_CfgPtr->ServiceGroupNum)
    {
        Sd_ReportError(apiId, SD_E_INV_ID);
        bRet = FALSE;
    }
    return bRet;
}

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
/**
 * @brief               Validates a partition for a server service.
 * @param[in]           apiId API ID.
 * @param[in]           applicationId Application ID.
 * @param[in]           serverServiceHandleId Server service handle ID.
 * @return              TRUE if the partition is valid for the server service, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74055
 */
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionServerService(uint8 apiId, ApplicationType applicationId, uint16 serverServiceHandleId)
{
    boolean bRet = TRUE;

    ApplicationType serverServiceApplicationId = Sd_CfgPtr->GlbServerService[serverServiceHandleId].ServerApplicationId;
    if (serverServiceApplicationId != applicationId)
    {
        Sd_ReportError(apiId, SD_E_PARTITION_ERROR);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a partition for a client service.
 * @param[in]           apiId API ID.
 * @param[in]           applicationId Application ID.
 * @param[in]           clientServiceHandleId Client service handle ID.
 * @return              TRUE if the partition is valid for the client service, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace              CPD-74056
 */
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionClientService(uint8 apiId, ApplicationType applicationId, uint16 clientServiceHandleId)
{
    boolean bRet = TRUE;

    ApplicationType clientServiceApplicationId = Sd_CfgPtr->GlbClientService[clientServiceHandleId].ClientApplicationId;
    if (clientServiceApplicationId != applicationId)
    {
        Sd_ReportError(apiId, SD_E_PARTITION_ERROR);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a partition for a service group.
 * @param[in]           apiId API ID.
 * @param[in]           applicationId Application ID.
 * @param[in]           serviceGroupId Service group ID.
 * @return              TRUE if the partition is valid for the service group, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace              CPD-74057
 */
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionServiceGroup(uint8 apiId, ApplicationType applicationId, uint16 serviceGroupId)
{
    boolean bRet = TRUE;

    ApplicationType serviceGroupApplicationId = Sd_CfgPtr->GlbServiceGroup[serviceGroupId].ServiceGroupApplicationId;
    if (serviceGroupApplicationId != applicationId)
    {
        Sd_ReportError(apiId, SD_E_PARTITION_ERROR);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a partition for a consumed event group.
 * @param[in]           apiId API ID.
 * @param[in]           applicationId Application ID.
 * @param[in]           consumedEventGroupId Consumed event group ID.
 * @return              TRUE if the partition is valid for the consumed event group, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace              CPD-74058
 */
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionConsumedEventGroup(uint8 apiId, ApplicationType applicationId, uint16 consumedEventGroupId)
{
    boolean bRet = TRUE;

    ApplicationType consumedEventGroupApplicationId =
        Sd_CfgPtr->GlbConsumedEventGroup[consumedEventGroupId].ConsumedEventGroupApplicationId;
    if (consumedEventGroupApplicationId != applicationId)
    {
        Sd_ReportError(apiId, SD_E_PARTITION_ERROR);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a partition for an Rx PDU.
 * @param[in]           apiId API ID.
 * @param[in]           applicationId Application ID.
 * @param[in]           rxPduId Rx PDU ID.
 * @return              TRUE if the partition is valid for the Rx PDU, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74060
 */
SD_LOCAL_INLINE boolean Sd_ValidatePartitionRxPdu(uint8 apiId, ApplicationType applicationId, PduIdType rxPduId)
{
    boolean bRet = TRUE;

    const Sd_RxPduIdSoConIdMapType* rxPduIdSoConIdMap = Sd_CfgPtr->GlbRxPduIdSoConIdMap;
    for (uint16 index = 0u; index < Sd_CfgPtr->RxPduIdSoConIdMapLen; index++)
    {
        if (rxPduIdSoConIdMap->RxPduId == rxPduId)
        {
            break;
        }
    }

    if (rxPduIdSoConIdMap->RxPduApplicationId != applicationId)
    {
        Sd_ReportError(apiId, SD_E_PARTITION_ERROR);
        bRet = FALSE;
    }
    return bRet;
}

/**
 * @brief               Validates a partition for a Tx socket connection ID.
 * @param[in]           apiId API ID.
 * @param[in]           applicationId Application ID.
 * @param[in]           soConId Socket connection ID.
 * @return              TRUE if the partition is valid for the Tx socket connection ID, FALSE otherwise.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace                CPD-74059
 */
SD_LOCAL_INLINE boolean
    Sd_ValidatePartitionTxSoConId(uint8 apiId, ApplicationType applicationId, SoAd_SoConIdType soConId)
{
    SoAd_SoConIdType       txPduSoConId;
    const Sd_InstanceType* instancePtr = Sd_CfgPtr->Instance;
    for (uint8 idx = 0u; idx < SD_INSTANCE_NUM; idx++)
    {
        txPduSoConId = instancePtr[idx].SdInstanceTxPduSoConId;
        if (txPduSoConId == soConId)
        {
            break;
        }
    }

    boolean bRet = TRUE;
    if (applicationId != instancePtr->InstanceApplicationId)
    {
        Sd_ReportError(apiId, SD_E_PARTITION_ERROR);
        bRet = FALSE;
    }
    return bRet;
}
#endif
#endif

SD_LOCAL_INLINE
boolean Sd_GlobalHandlerIdToVariant(const Sd_ConvertMapType* map, uint16 srcHandlerId, uint16* outputHandlerId)
{
    boolean bRet = FALSE;
    if ((outputHandlerId != NULL_PTR) && (map != NULL_PTR))
    {
        *outputHandlerId = map->handlerMapPtr[srcHandlerId];
        bRet             = TRUE;
    }
    return bRet;
}

/**
 * @brief              Checks if the given handlerId exists in the service handle ID list.
 * @param[in]          handlerId: The service handle ID to find.
 * @param[in]          serviceList: Pointer to the structure containing the service handle ID list.
 * @return             TRUE if handlerId is found in the list, FALSE otherwise.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-75566
 */
SD_LOCAL boolean Sd_FindHandlerIdInList(uint16 handlerId, const Sd_ServiceHandleIdListType* serviceList)
{
    boolean findFlag = FALSE;
    for (uint8 index = 0u; index < serviceList->HandleIdNum; index++)
    {
        if (serviceList->ServiceHandleId[index] == handlerId)
        {
            findFlag = TRUE;
            break;
        }
    }
    return findFlag;
}

/**
 * @brief Retrieves the application ID.
 * @return Application ID.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace              CPD-74061
 */
SD_LOCAL ApplicationType Sd_GetApplicationId(void)
{
    ApplicationType applicationId = (ApplicationType)0u;
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
    applicationId = GetApplicationID();
#endif
    return applicationId;
}

SD_LOCAL_INLINE uint16 Sd_Strlen(const Sd_CharType* str)
{
    uint16             length = 0u;
    const Sd_CharType* tmpStr = str;

    /* PRQA S 2106,2140 ++ */ /* VL_Sd_2106_2140 */
    while ((Sd_CharType)tmpStr[length] != '\0')
    {
        length++;
    }
    /* PRQA S 2106,2140 -- */

    return length;
}

/**
 * @brief              Init sd server service runtime data
 * @param[in]          applicationId: Partition id
 * @reentrant          Reentrant for different partition.
 * @synchronous        TRUE
 * @trace              CPD-74569
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_ServerServiceInit(ApplicationType applicationId)
{
    for (uint16 index = 0u; index < SD_SERVER_SERVICE_NUM; index++)
    {
        const Sd_ServerServiceType* serverServicePtr = &(Sd_CfgPtr->GlbServerService[index]);

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        if (applicationId != serverServicePtr->ServerApplicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
        }
#endif

        /*@req <SWS_SD_00020> <SWS_Sd_00748> */
        /* If SdServerServiceAutoAvailable=True,set state SD_SERVER_SERVICE_AVAILABLE;
           If the ServerService is associated with at least one ServiceGroup, it should
           not be automatically started by Sd_Init. */
        if ((serverServicePtr->ServiceGroupRefNum == 0u) && (serverServicePtr->SdServerServiceAutoAvailable))
        {
            Sd_ServerRTData[index]->SetState = SD_SERVER_SERVICE_AVAILABLE;
        }
        else
        {
            /* Set Server Service Instance state to SD_SERVER_SERVICE_DOWN */
            Sd_ServerRTData[index]->SetState = SD_SERVER_SERVICE_DOWN;
        }

        /* Set all server service to Down Phase */
        Sd_ServerRTData[index]->Phase            = SD_SERVER_PHASE_DOWN;
        Sd_ServerRTData[index]->InitialWaitTimer = 0u;
        Sd_ServerRTData[index]->RepetitionCount  = 0u;
        Sd_ServerRTData[index]->RepetitionTimer  = 0u;
        Sd_ServerRTData[index]->MainTimer        = 0u;
        Sd_ServerRTData[index]->Head             = NULL_PTR;

#if (SD_EVENT_HANDLER_NUM > 0)
        const Sd_EventHandlerType* eventHandlePtr = serverServicePtr->SdEventHandler;
        for (uint16 eventIndex = 0u; eventIndex < serverServicePtr->EventHandlerNum; eventIndex++)
        {
            Sd_EventHandlerRTData[eventHandlePtr->SdEventHandlerHandleId]->UdpSubscribeList = NULL_PTR;
            Sd_EventHandlerRTData[eventHandlePtr->SdEventHandlerHandleId]->TcpSubscribeList = NULL_PTR;
            Sd_EventHandlerRTData[eventHandlePtr->SdEventHandlerHandleId]->CurState         = SD_EVENT_HANDLER_RELEASED;
            Sd_EventHandlerRTData[eventHandlePtr->SdEventHandlerHandleId]->UdpSubsClientNum = 0u;
            Sd_EventHandlerRTData[eventHandlePtr->SdEventHandlerHandleId]->UdpSubsEndPointNum     = 0u;
            Sd_EventHandlerRTData[eventHandlePtr->SdEventHandlerHandleId]->TcpSubsClientNum       = 0u;
            Sd_EventHandlerRTData[eventHandlePtr->SdEventHandlerHandleId]->TcpTriggerTransmitList = NULL_PTR;
            Sd_EventHandlerRTData[eventHandlePtr->SdEventHandlerHandleId]->UdpTriggerTransmitList = NULL_PTR;

            eventHandlePtr++;
        }
#endif
    }
    SD_UNUSED(applicationId);
}
#endif

/**
 * @brief              Init sd client service runtime data
 * @param[in]          applicationId: Partition id
 * @reentrant          Reentrant for different partition.
 * @synchronous        TRUE
 * @trace              CPD-74570
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientServiceInit(ApplicationType applicationId)
{
    for (uint16 index = 0u; index < SD_CLIENT_SERVICE_NUM; index++)
    {
        const Sd_ClientServiceType* clientServicePtr = &(Sd_CfgPtr->GlbClientService[index]);

#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        if (applicationId != clientServicePtr->ClientApplicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
        }
#endif

        Sd_ClientRTData[index]->Ttl.IsValidForever           = FALSE;
        Sd_ClientRTData[index]->Ttl.ttlms                    = 0u;
        Sd_ClientRTData[index]->TcpSoCon.IsOptRcved          = FALSE;
        Sd_ClientRTData[index]->TcpSoCon.IsSoConOpened       = FALSE;
        Sd_ClientRTData[index]->TcpSoCon.SoConId             = 0u;
        Sd_ClientRTData[index]->UdpSoCon.IsOptRcved          = FALSE;
        Sd_ClientRTData[index]->UdpSoCon.IsSoConOpened       = FALSE;
        Sd_ClientRTData[index]->UdpSoCon.SoConId             = 0u;
        Sd_ClientRTData[index]->MulitcastSoCon.IsOptRcved    = FALSE;
        Sd_ClientRTData[index]->MulitcastSoCon.IsSoConOpened = FALSE;
        Sd_ClientRTData[index]->MulitcastSoCon.SoConId       = 0u;
        Sd_ClientRTData[index]->InitialWaitTimer             = 0u;
        Sd_ClientRTData[index]->RepetitionTimer              = 0u;
        Sd_ClientRTData[index]->MainTimer                    = 0u;
        Sd_ClientRTData[index]->Head                         = NULL_PTR;
        /*@req <SWS_SD_00021> */
        /* If SdClientServiceAutoRequired=True,set state SD_CLIENT_SERVICE_REQUESTED*/
        if (Sd_CfgPtr->GlbClientService[index].SdClientServiceAutoRequire)
        {
            Sd_ClientRTData[index]->SetState = SD_CLIENT_SERVICE_REQUESTED;
        }
        else
        {
            /* Set Client Service Instance state to SD_CLIENT_SERVICE_RELEASED */
            Sd_ClientRTData[index]->SetState = SD_CLIENT_SERVICE_RELEASED;
        }

        Sd_ClientRTData[index]->CurState = SD_CLIENT_SERVICE_DOWN;
        /* Set all client service to Down Phase */
        Sd_ClientRTData[index]->Phase                   = SD_CLIENT_PHASE_DOWN;
        Sd_ClientRTData[index]->RepetitionCount         = 0u;
        Sd_ClientRTData[index]->OfferServiceRxFlag      = FALSE;
        Sd_ClientRTData[index]->RxTcpEndpointFlag       = FALSE;
        Sd_ClientRTData[index]->RxUdpEndpointFlag       = FALSE;
        Sd_ClientRTData[index]->RxMulitcastEndpointFlag = FALSE;

        /*@req <SWS_SD_00440> */
        /* If SdConsumedEventGroupAutoRequired=True,set ConsumedEventGroup state to
            SD_CONSUMED_EVENTGROUP_REQUESTED as soon as the associated Client Service Instance is requested.*/
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        const Sd_ConsumedEventGroupType* eventGroupPtr = &clientServicePtr->SdConsumedEventGroup[0];
        for (uint16 eventGroupIdx = 0u; eventGroupIdx < clientServicePtr->ConsumedEventGroupNum; eventGroupIdx++)
        {
            uint16 handlerId                                              = eventGroupPtr->SdConsumedEventGroupHandleId;
            Sd_ConsumedEventGroupRTData[handlerId]->Ttl.IsValidForever    = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->Ttl.ttlms             = 0u;
            Sd_ConsumedEventGroupRTData[handlerId]->MultiSoCon.IsOptRcved = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->MultiSoCon.IsSoConOpened = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->MultiSoCon.SoConId       = 0u;
            Sd_ConsumedEventGroupRTData[handlerId]->CurState                 = SD_CONSUMED_EVENTGROUP_DOWN;
            Sd_ConsumedEventGroupRTData[handlerId]->IsSubscribed             = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->IsTcpSubsSend            = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->IsUdpSubsSend            = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->IsMulitcastSubsSend      = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->IsAnsweredWithAck        = TRUE;
            Sd_ConsumedEventGroupRTData[handlerId]->IsSendSubEvtAfterRequest = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->SubsRetryEnable          = FALSE;
            Sd_ConsumedEventGroupRTData[handlerId]->SubsRetryCnt             = 0u;

            if (Sd_CfgPtr->GlbClientService[index].SdClientServiceAutoRequire
                && eventGroupPtr->SdConsumedEventGroupAutoRequire)
            {
                Sd_ConsumedEventGroupRTData[handlerId]->SetState = SD_CONSUMED_EVENTGROUP_REQUESTED;
            }
            else
            {
                Sd_ConsumedEventGroupRTData[handlerId]->SetState = SD_CONSUMED_EVENTGROUP_RELEASED;
            }
            eventGroupPtr++;
        }
#endif
    }
#endif
    SD_UNUSED(applicationId);
}

/**
 * @brief              Init sd instance runtime data
 * @param[in]          applicationId: Partition id
 * @reentrant          Reentrant for different partition.
 * @synchronous        TRUE
 * @trace              CPD-74571
 */
#if (SD_INSTANCE_NUM > 0)
SD_LOCAL void Sd_InstanceInit(ApplicationType applicationId)
{
    for (uint16 index = 0u; index < SD_INSTANCE_NUM; index++)
    {
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
        const Sd_InstanceType* instancePtr = &Sd_CfgPtr->Instance[index];
        if (applicationId != instancePtr->InstanceApplicationId)
        {
            continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
        }
#endif

        Sd_InstanceRTData[index]->SessionIdHandle.SesIdCtrlList   = NULL_PTR;
        Sd_InstanceRTData[index]->SessionIdHandle.TxMultiSesIdCnt = 0u;
        Sd_InstanceRTData[index]->IpAddrState                     = TCPIP_IPADDR_STATE_UNASSIGNED;
        Sd_InstanceRTData[index]->RebootFlag                      = TRUE;

        /* @req <SWS_SD_00723> */
        /* The API SoAd_OpenSoCon() shall be called for all Socket Connections
         * associated with SdInstanceTxPdu,SdInstanceUnicastRxPdu and
         * SdInstanceMulticastRxPdu. */
        /* Call SoAd_OpenSoCon() for all SdInstanceTxPdu */
        (void)SoAd_OpenSoCon(Sd_CfgPtr->Instance[index].SdInstanceTxPduSoConId);
    }

    /* Call SoAd_OpenSoCon() for all SdInstanceUnicastRxPdu and
     * SdInstanceMulticastRxPdu */
    for (uint16 index = 0u; index < Sd_CfgPtr->RxPduIdSoConIdMapLen; index++)
    {
        (void)SoAd_OpenSoCon(Sd_CfgPtr->GlbRxPduIdSoConIdMap[index].SoConId);
    }
    SD_UNUSED(applicationId);
}
#endif

/**
 * @brief              Stopped associating the servers with the ServiceGroup.
 * @param[in]          serviceGroupId: Id of SdServiceGroup to be stopped
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74572
 */
#if (SD_SERVER_SERVICE_GROUP_USED == STD_ON)
SD_LOCAL void Sd_ServiceGroupStopServer(Sd_ServiceGroupIdType serviceGroupId)
{
    uint16 serverServiceNum = Sd_CfgPtr->GlbServiceGroup[serviceGroupId].ServerServiceNum;
    for (uint16 serviceIndex = 0u; serviceIndex < serverServiceNum; serviceIndex++)
    {
        /*@req <SWS_Sd_00750> */
        uint16 serverHandleId = Sd_CfgPtr->GlbServiceGroup[serviceGroupId].ServerServiceRefPtr[serviceIndex];
        if (Sd_ServerRTData[serverHandleId]->ServiceGroupRequestCounter > 0u)
        {
            SchM_Enter_Sd_ExclusiveArea();
            Sd_ServerRTData[serverHandleId]->ServiceGroupRequestCounter--;
            SchM_Exit_Sd_ExclusiveArea();
        }

        if ((0u == Sd_ServerRTData[serverHandleId]->ServiceGroupRequestCounter)
            && (SD_SERVER_SERVICE_DOWN != Sd_ServerRTData[serverHandleId]->SetState))
        {
            SchM_Enter_Sd_ExclusiveArea();
            Sd_ServerRTData[serverHandleId]->SetState = SD_SERVER_SERVICE_DOWN;
            SchM_Exit_Sd_ExclusiveArea();
        }
    }
}
#endif

/**
 * @brief              Stopped associating the clients with the ServiceGroup.
 * @param[in]          serviceGroupId: Id of SdServiceGroup to be stopped
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74573
 */
#if (SD_CLIENT_SERVICE_GROUP_USED == STD_ON)
SD_LOCAL void Sd_ServiceGroupStopClient(Sd_ServiceGroupIdType serviceGroupId)
{
    uint16 clientServiceNum = Sd_CfgPtr->GlbServiceGroup[serviceGroupId].ClientServiceNum;
    for (uint16 serviceIndex = 0u; serviceIndex < clientServiceNum; serviceIndex++)
    {
        /*@req <SWS_Sd_00750> */
        uint16 clientHandleId = Sd_CfgPtr->GlbServiceGroup[serviceGroupId].ClientServiceRefPtr[serviceIndex];
        if (Sd_ClientRTData[clientHandleId]->ServiceGroupRequestCounter > 0u)
        {
            SchM_Enter_Sd_ExclusiveArea();
            Sd_ClientRTData[clientHandleId]->ServiceGroupRequestCounter--;
            SchM_Exit_Sd_ExclusiveArea();
        }

        if ((0u == Sd_ClientRTData[clientHandleId]->ServiceGroupRequestCounter)
            && (SD_CLIENT_SERVICE_RELEASED != Sd_ClientRTData[clientHandleId]->SetState))
        {
            const Sd_ClientServiceType*      clientService         = &Sd_CfgPtr->GlbClientService[clientHandleId];
            const Sd_ConsumedEventGroupType* consumedEventGroupPtr = clientService->SdConsumedEventGroup;

            SchM_Enter_Sd_ExclusiveArea();
            Sd_ClientRTData[clientHandleId]->SetState = SD_CLIENT_SERVICE_RELEASED;
            SchM_Exit_Sd_ExclusiveArea();

            /*@req <SWS_Sd_00443> */
            for (uint16 index = 0u; index < clientService->ConsumedEventGroupNum; index++)
            {
                uint16                       handlerId   = consumedEventGroupPtr->SdConsumedEventGroupHandleId;
                Sd_ConsumedEventGroupRTType* egRtDataPtr = Sd_ConsumedEventGroupRTData[handlerId];
                if (SD_CONSUMED_EVENTGROUP_REQUESTED == egRtDataPtr->SetState)
                {
                    SchM_Enter_Sd_ExclusiveArea();
                    egRtDataPtr->SetState = SD_CONSUMED_EVENTGROUP_RELEASED;
                    SchM_Exit_Sd_ExclusiveArea();
                }
                consumedEventGroupPtr++;
            }
        }
    }
}
#endif

/**
 * @brief              The RxIndication interface reports a Dem error.
 * @param[in]          RxPduId: ID of the received I-PDU.
 * @reentrant          Reentrant for different SoConIds.
 * @synchronous        TRUE
 * @trace              CPD-74574
 */
#if (SD_DEM_USED == STD_ON)
SD_LOCAL void Sd_RxIndicationReportDem(PduIdType RxPduId)
{
    /* In case a malformed message has been received, the extended production error
     * SD_E_MALFORMED_MSG shall be reported.*/
    const Sd_InstanceType* instancePtr = Sd_CfgPtr->Instance;
    for (uint16 index = 0u; index < SD_INSTANCE_NUM; index++)
    {
        if ((instancePtr->SdInstanceMulticastRxPdu == RxPduId) || (instancePtr->SdInstanceUnicastRxPdu == RxPduId))
        {
            const Sd_DemEventParameterType* demEventPtr =
                instancePtr->SdInstanceDemEventParameterRefs->SD_E_MALFORMED_MSG;
            if (NULL_PTR != demEventPtr)
            {
                (void)Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
            }
            break;
        }
        instancePtr++;
    }
    SD_UNUSED(instancePtr);
}
#endif

/**
 * @brief              Check option is valid or not that referenced by entry.
 * @param[in]          instancePtr: Pointer for specified instance
 * @param[in]          sdMsg: Sd message buffer.
 * @param[in]          sdMsgOffset: Sd message buffer index.
 * @param[in]          optionArrayLen: Option data byte num.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @return             E_OK: The option format check pass
 *                     E_NOT_OK: The option format check failure.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74549
 */
SD_LOCAL Std_ReturnType Sd_EntryRefOptionFormatCheck(
    const Sd_InstanceType* instancePtr,
    const uint8*           sdMsg,
    uint16                 sdMsgOffset,
    uint16                 optionArrayLen,
    const uint16           optStartPosMap[])
{
    Std_ReturnType optSecuritycheckRet = E_OK;
    Std_ReturnType optConflictCheckRet = E_OK;
    uint8          entryType           = sdMsg[sdMsgOffset];
    Std_ReturnType ret                 = E_OK;
    if (0UL == optionArrayLen)
    {
        if ((SD_ENTRY_TYPE_OFFER_SERVICE == entryType) || (SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP == entryType))
        {
            ret = E_NOT_OK;
        }
        else
        {
            /* FindService and SubscribeEventgroupAck entry's optionArrayLength can be 0 */
        }
    }
    else
    {
        /*@req <SWS_SD_00688> */
        optSecuritycheckRet = Sd_OptionsSecurityCheck(instancePtr, sdMsg, sdMsgOffset, optStartPosMap);

        if (E_OK == optSecuritycheckRet)
        {
            /*@req <SWS_SD_00663>,<SWS_SD_00714> */
            optConflictCheckRet = Sd_OptionsConflictCheck(sdMsg, sdMsgOffset, optStartPosMap);
        }
    }
    if ((E_NOT_OK == optSecuritycheckRet) || (E_NOT_OK == optConflictCheckRet))
    {
        ret = E_NOT_OK;
    }

    return ret;
}

/**
 * @brief              Processes a received entry in the Service Discovery message.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          rxQueueItem: Pointer to the received queue item.
 * @param[in]          index: Index of the entry in the message.
 * @param[in]          optStartPos: Array containing the start positions of each option.
 * @param[in]          entryType: Type of the entry.
 * @param[in]          isRxUseMulticast: Flag indicating if multicast is used.
 * @param[in]          isReplyNAck: Flag indicating if a negative acknowledgment is expected.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74556
 */
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL void Sd_ProcessRxEnrty(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 index,
    const uint16           optStartPos[],
    uint8                  entryType,
    boolean                isRxUseMulticast,
    boolean                isReplyNAck,
    ApplicationType        applicationId)
/* PRQA S 6040 -- */
{
    if (SD_ENTRY_TYPE_FIND_SERVICE == entryType)
    {
#if (SD_SERVER_SERVICE_NUM > 0)
        Sd_RxFindServiceHandle(instancePtr, rxQueueItem, index, isRxUseMulticast, optStartPos, applicationId);
#endif /* SD_SERVER_SERVICE_NUM > 0 */
    }
    else if (SD_ENTRY_TYPE_OFFER_SERVICE == entryType)
    {
#if (SD_CLIENT_SERVICE_NUM > 0)
        Sd_RxOfferServiceHandle(instancePtr, rxQueueItem, index, optStartPos, applicationId);
#endif /* SD_CLIENT_SERVICE_NUM > 0 */
    }
    else if (SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP == entryType)
    {
#if (SD_EVENT_HANDLER_NUM > 0)
        Sd_RxSubscribeEventgroupHandle(instancePtr, rxQueueItem, index, optStartPos, isReplyNAck, applicationId);
#endif /* SD_EVENT_HANDLER_NUM > 0 */
    }
    else if (SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK == entryType)
    {
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        Sd_RxSubscribeEventgroupAckHandle(instancePtr, rxQueueItem, index, optStartPos, applicationId);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
    }
    else
    {
        /*@req <SWS_SD_00483> */
        /* When receiving Service Discovery messages, the receiver shall
         * ignore Entries of unknown type.*/
    }
}

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
/**
 * @brief              Handles remote reboot detection and management.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          rxQueueItem: Pointer to the received queue item.
 * @param[in]          isRxUseMulticast: Flag indicating if multicast is used.
 * @param[in]          applicationId: Application ID.
 * @return             TRUE if a remote reboot is detected, FALSE otherwise.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74550
 */
SD_LOCAL boolean Sd_RemoteRebootHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    boolean                isRxUseMulticast,
    ApplicationType        applicationId)
{
    boolean isRebootDetected = FALSE;
    boolean isNewRemoteAddr  = Sd_IsNewRemoteAddr(instancePtr->SdInstanceHandleId, &rxQueueItem->RemoteAddr);
    if (isNewRemoteAddr)
    {
        /* Add the remote address to SessionControlList */
        (void)Sd_AddRemoteAddrToSessionIdCtrlList(
            instancePtr->SdInstanceHandleId,
            rxQueueItem,
            isRxUseMulticast,
            applicationId);
    }
    else
    {
        isRebootDetected = Sd_RemoteRebootDetecte(instancePtr->SdInstanceHandleId, rxQueueItem, isRxUseMulticast);
        if (isRebootDetected)
        {
            Sd_RemoteRebootDetectedHandle(instancePtr->SdInstanceHandleId, rxQueueItem, applicationId);
        }
    }

    /* Save remote address used for SessionId control */
    if (!isRebootDetected)
    {
        Sd_SaveRemoteSessionId(instancePtr->SdInstanceHandleId, rxQueueItem, isRxUseMulticast);
    }

    return isRebootDetected;
}

SD_LOCAL boolean Sd_RxMessageMatchInstance(const Sd_RxQueueType* rxQueueItem, ApplicationType applicationId)
{
    const Sd_InstanceType* instancePtr = Sd_CfgPtr->Instance;
    boolean                isMatch     = FALSE;
    for (uint16 instanceIdx = 0u; instanceIdx < SD_INSTANCE_NUM; instanceIdx++)
    {
        if (
#if (SD_MULTIPLE_PARTITION_SUPPORT == STD_ON)
            (applicationId == instancePtr->InstanceApplicationId) &&
#endif
            ((rxQueueItem->RxPduId == instancePtr->SdInstanceMulticastRxPdu)
             || (rxQueueItem->RxPduId == instancePtr->SdInstanceUnicastRxPdu)))
        {
            isMatch = TRUE;
            break;
        }
        instancePtr++; /* PRQA S 2983 */ /* VL_Sd_PointerCalu */
    }

    SD_UNUSED(applicationId);
    return isMatch;
}

SD_LOCAL boolean Sd_RxMessageOptionCheck(
    const Sd_InstanceType* instancePtr,
    const uint8*           sdMsg,
    uint16                 optStartPos[],
    uint16                 entryArrayLen,
    uint16                 optionArrayLen)
{
    boolean result = FALSE;

    Std_ReturnType optFormatCheckRet = Sd_OptionFormatCheck(sdMsg);
    Std_ReturnType optExistCheckRet  = E_OK;
    if (E_OK == optFormatCheckRet)
    {
        /*@req <SWS_SD_00708> */
        /* Find out all options start index,store them to optStartPos array */
        uint8  count            = 0u;
        uint16 firstOptionIndex = SD_FIRST_ENTRY_INDEX + entryArrayLen + SD_LENGTH_OF_OPTIONS_ARRAY_FIELD_LEN;
        uint16 loopEndPos       = optionArrayLen + firstOptionIndex;
        uint16 index            = firstOptionIndex;
        while (index < loopEndPos)
        {
            optStartPos[count] = index;
            count++;
            uint16 optionLength = Sd_NPtrtoHs(&sdMsg[index]);
            index               = index + optionLength + SD_OPTION_LENGTH_FIELD_LEN + SD_OPTION_TYPE_FIELD_LEN;
        }
        optExistCheckRet = Sd_OptionsExistCheck(instancePtr, sdMsg, entryArrayLen, count);
        if (optExistCheckRet == E_OK)
        {
            result = TRUE;
        }
    }

    return result;
}

/**
 * @brief              Parse received message
 * @reentrant          Reentrant for different partition.
 * @synchronous        TRUE
 * @trace              CPD-74062
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
SD_LOCAL void Sd_ParseRxMessage(ApplicationType applicationId)
{
    uint16 optStartPos[SD_ARRAY_LEN_70] = {0}; /* Max Option Num: ((1400-12-4-4)/(16+12*2))*2 */

    while (NULL_PTR != Sd_RxQueueHead[applicationId])
    {
        const Sd_RxQueueType* rxQueueItem   = Sd_RxQueueHead[applicationId];
        const uint8*          sdMsg         = rxQueueItem->BufPtr;
        uint16                entryArrayLen = (uint16)Sd_NPtrtoHl(&sdMsg[SD_ENTRIES_ARRAY_LENGTH_INDEX]);
        uint16                optionArrayLen =
            (uint16)Sd_NPtrtoHl(&sdMsg[SD_ENTRIES_ARRAY_LENGTH_INDEX + SD_MESSAGE_ENTRYLEN_FILED_LEN + entryArrayLen]);

        boolean isMatchInstance = Sd_RxMessageMatchInstance(rxQueueItem, applicationId);
        if (!isMatchInstance)
        {
            continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
        }

        for (uint16 instanceIdx = 0u; instanceIdx < SD_INSTANCE_NUM; instanceIdx++)
        {
            const Sd_InstanceType* instancePtr = &Sd_CfgPtr->Instance[instanceIdx];

            boolean bRxOptionCheck =
                Sd_RxMessageOptionCheck(instancePtr, sdMsg, optStartPos, entryArrayLen, optionArrayLen);

            boolean isRxUseMulticast;
            if (rxQueueItem->RxPduId == instancePtr->SdInstanceMulticastRxPdu)
            {
                isRxUseMulticast = TRUE;
            }
            else
            {
                isRxUseMulticast = FALSE;
            }

            (void)Sd_RemoteRebootHandle(instancePtr, rxQueueItem, isRxUseMulticast, applicationId);

            /* Parse Entries */
            uint16 loopEndPos = entryArrayLen + SD_FIRST_ENTRY_INDEX;
            for (uint16 index = SD_FIRST_ENTRY_INDEX; index < loopEndPos; index += SD_ENTRY_SIZE)
            {
                uint8   entryType   = sdMsg[index];
                boolean isReplyNAck = FALSE;

                Std_ReturnType entryRefOptCheckRet = E_NOT_OK;
                if (bRxOptionCheck)
                {
                    entryRefOptCheckRet =
                        Sd_EntryRefOptionFormatCheck(instancePtr, sdMsg, index, optionArrayLen, optStartPos);
                }

                if ((E_NOT_OK == entryRefOptCheckRet))
                {
                    if (SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP != entryType)
                    {
                        /* Security check not pass,ignore the entry */
                        /* An entry references two or more options that are in conflict,Ignored the entry*/
                        continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
                    }
                    else
                    {
                        /* SubscribeEventgroup Message Option check not pass,Send SubscribeEventgroupNAck*/
                        isReplyNAck = TRUE;
                    }
                }

                Sd_ProcessRxEnrty(
                    instancePtr,
                    rxQueueItem,
                    index,
                    optStartPos,
                    entryType,
                    isRxUseMulticast,
                    isReplyNAck,
                    applicationId);
            }
        }

        Sd_RxQueueHead[applicationId] = rxQueueItem->Next;
        /* Free saved sdu data buffer */
        Sd_FreeEx(rxQueueItem->BufPtr, Sd_RxMemPool[applicationId]);
        /* Free queue buffer */
        Sd_FreeEx(rxQueueItem, Sd_RxMemPool[applicationId]);
    }
}
/* PRQA S 2889 -- */

/**
 * @brief              Security check for options(Check if Clients and Servers in the same subset)
 * @param[in]          instancePtr: Pointer for specified instance
 * @param[in]          sdMsg: Received Sd Message
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @return             E_OK: No Error
 *                     E_NOT_OK: Error,need response Nack
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74063
 */
SD_LOCAL Std_ReturnType Sd_OptionsSecurityCheck(
    const Sd_InstanceType* instancePtr,
    const uint8*           sdMsg,
    uint16                 entryPos,
    const uint16           optStartPosMap[])
{
    Std_ReturnType     checkPass = E_OK;
    TcpIp_SockAddrType localAddr;
    TcpIp_SockAddrType defaultRouter;
    uint8              netmask;

    /* Get loacal address and netmask for options security check */
    SoAd_SoConIdType soConId = instancePtr->SdInstanceTxPduSoConId;
    localAddr.domain         = instancePtr->SdAddrFamily;
    defaultRouter.domain     = instancePtr->SdAddrFamily;
    (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);

    /* @req SWS_SD_00720 */
    uint8 localAddrCheckLen;
    if (instancePtr->SdInstanceLocalAdressCheckLengthEnable)
    {
        localAddrCheckLen = instancePtr->SdInstanceLocalAdressCheckLength;
    }
    else
    {
        localAddrCheckLen = netmask;
    }

    /* "0" meaning not to check */
    if (localAddrCheckLen > 0u)
    {
        Sd_Type1EntryType type1Entry;
        (void)IStdLib_MemCpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));

        uint8 idx1stOptions = type1Entry.Index1stOptions;
        uint8 numOpt1       = type1Entry.NumOfOpt1;
        uint8 idx2ndOptions = type1Entry.Index2ndOptions;
        uint8 numOpt2       = type1Entry.NumOfOpt2;
        uint8 optNumSum     = numOpt1 + numOpt2;

        for (uint8 index = 0u; index < optNumSum; index++)
        {
            uint8 optIndex;
            if (index < numOpt1)
            {
                optIndex = idx1stOptions + index;
            }
            else
            {
                optIndex = idx2ndOptions + index - numOpt1;
            }

            uint8 optType = sdMsg[optStartPosMap[optIndex] + SD_OPTION_TYPE_FILED_OFFSET];

            if ((SD_IPV4_ENDPOINT_OPTION == optType) || (SD_IPV4_SD_ENDPOINT_OPTION == optType))
            {
                Sd_IPv4OptionsType ipv4Opt;
                (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
                checkPass = Sd_IsIpAddrInSameSubset(
                    (uint8*)localAddr.addr,
                    (const uint8*)ipv4Opt.IPv4Address,
                    localAddrCheckLen,
                    SD_IPV4_ADDRESS_LEN);
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else if ((SD_IPV6_ENDPOINT_OPTION == optType) || (SD_IPV6_SD_ENDPOINT_OPTION == optType))
            {
                Sd_IPv6OptionsType ipv6Opt;
                (void)IStdLib_MemCpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
                checkPass = Sd_IsIpAddrInSameSubset(
                    (uint8*)localAddr.addr,
                    (const uint8*)ipv6Opt.IPv6Address,
                    localAddrCheckLen,
                    SD_IPV6_ADDRESS_LEN);
            }
            else
            {
                /* Other options needn't check */
            }
#endif /* STD_ON == SD_IPV6_ENABLE */

            if (E_NOT_OK == checkPass)
            {
                break;
            }
        }
    }

    return checkPass;
}

/**
 * @brief              Check whether IP address is in same subset
 * @param[in]          ipAddr1: Ip address.
 * @param[in]          ipAddr2: Ip address.
 * @param[in]          netmask: Subnet mask.
 * @param[in]          addrLen: Ip address length.
 * @return             E_OK: In same subset
 *                     E_NOT_OK: In different subset
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74064
 */
SD_LOCAL Std_ReturnType
    Sd_IsIpAddrInSameSubset(const uint8* ipAddr1, const uint8* ipAddr2, uint8 netmask, uint8 addrLen)
{
    Std_ReturnType ret = E_NOT_OK;

    /* PATCH for TC8: check if remoteAddr equal to DUT local Address */
    StatusType cmpResult = IStdLib_MemCmp(ipAddr1, ipAddr2, addrLen);
    if (0u != cmpResult)
    {
        uint8 addr1AndResult[SD_ARRAY_LEN_16] = {0};
        uint8 addr2AndResult[SD_ARRAY_LEN_16] = {0};
        uint8 netmaskArray[SD_ARRAY_LEN_16]   = {0};

        (void)IStdLib_MemSet(netmaskArray, SD_NETWORK_MASK_VALUE_RESERVED, SD_IP_ADDRESS_MAX_LEN);
        uint8 tAddrLen = (uint8)((addrLen) << (SD_BYTE_TO_BIT_CONVERSION_FRACTION)) - netmask;
        uint8 clrByte  = tAddrLen >> SD_BYTE_TO_BIT_CONVERSION_FRACTION;
        uint8 clrBit   = tAddrLen & SD_MAGIC_INT_0X07;

        tAddrLen = addrLen - 1u;
        for (uint8 index = 0u; index < clrByte; index++)
        {
            netmaskArray[tAddrLen - index] = 0x00u;
        }

        tAddrLen               = addrLen - 1u - clrByte;
        netmaskArray[tAddrLen] = netmaskArray[tAddrLen] << clrBit;

        for (uint8 index = 0u; index < addrLen; index++)
        {
            addr1AndResult[index] = ipAddr1[index] & netmaskArray[index];
            addr2AndResult[index] = ipAddr2[index] & netmaskArray[index];
        }

        cmpResult = IStdLib_MemCmp(addr1AndResult, addr2AndResult, addrLen);
        if (0u == cmpResult)
        {
            ret = E_OK;
        }
    }

    return ret;
}

/**
 * @brief              Check if have conflict options
 * @param[in]          sdMsg: Received Sd Message
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @return             E_OK: No Error
 *                     E_NOT_OK: Error,need response Nack
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74065
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Sd_STMIF */
SD_LOCAL Std_ReturnType Sd_OptionsConflictCheck(const uint8* sdMsg, uint16 entryPos, const uint16 optStartPosMap[])
/* PRQA S 6030 -- */
{
    uint16 ipv4EndpTcpNum = 0u;
    uint16 ipv4EndpUdpNum = 0u;
#if (STD_ON == SD_IPV6_ENABLE)
    uint16 ipv6EndpTcpNum = 0u;
    uint16 ipv6EndpUdpNum = 0u;
#endif
    Std_ReturnType ret = E_OK;

    Sd_Type1EntryType type1Entry;
    (void)IStdLib_MemCpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    uint8 idx1stOptions = type1Entry.Index1stOptions;
    uint8 numOpt1       = type1Entry.NumOfOpt1;
    uint8 idx2ndOptions = type1Entry.Index2ndOptions;
    uint8 numOpt2       = type1Entry.NumOfOpt2;

    /*@req <SWS_SD_00663>,<SWS_SD_00714> */
    /* Check if entry references two or more options that are in conflict*/
    uint8 optNumSum = numOpt1 + numOpt2;
    for (uint8 index = 0u; index < optNumSum; index++)
    {
        uint8 optIndex;
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        uint8 optType = sdMsg[optStartPosMap[optIndex] + SD_OPTION_TYPE_FILED_OFFSET];
        if (SD_IPV4_ENDPOINT_OPTION == optType)
        {
            Sd_IPv4OptionsType ipv4Opt;
            (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
            if (SD_L4_PROTO_TCP == ipv4Opt.L4Proto)
            {
                if (ipv4EndpTcpNum >= 1u)
                {
                    /* entry references two or more options that are in conflict */
                    ret = E_NOT_OK;
                }
                ipv4EndpTcpNum++;
            }
            else if (SD_L4_PROTO_UDP == ipv4Opt.L4Proto)
            {
                if (ipv4EndpUdpNum >= 1u)
                {
                    /* entry references two or more options that are in conflict */
                    ret = E_NOT_OK;
                }
                ipv4EndpUdpNum++;
            }
            else
            {
                /*Nothing to do here*/
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else if (SD_IPV6_ENDPOINT_OPTION == optType)
        {
            Sd_IPv6OptionsType ipv6Opt;
            (void)IStdLib_MemCpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
            if (SD_L4_PROTO_TCP == ipv6Opt.L4Proto)
            {
                if (ipv6EndpTcpNum > 1u)
                {
                    /* entry references two or more options that are in conflict */
                    ret = E_NOT_OK;
                }
                ipv6EndpTcpNum++;
            }
            else if (SD_L4_PROTO_UDP == ipv6Opt.L4Proto)
            {
                if (ipv6EndpUdpNum > 1u)
                {
                    /* entry references two or more options that are in conflict */
                    ret = E_NOT_OK;
                }
                ipv6EndpUdpNum++;
            }
            else
            {
                /*Nothing to do here*/
            }
        }
        else
        {
            /*Nothing to do here*/
        }
#endif /* STD_ON == SD_IPV6_ENABLE */

        if (E_NOT_OK == ret)
        {
            break;
        }
    }

    return ret;
}

/**
 * @brief              Check whether the number of options is sufficient
 *                     (Check entries reference existing options)
 * @param[in]          instancePtr: Pointer for instance.
 * @param[in]          sdMsg: Received Sd Message.
 * @param[in]          entryArrayLen: Entry array length.
 * @param[in]          optNum: Option number for entry.
 * @return             E_OK: No Error
 *                     E_NOT_OK: Error,need response Nack
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74066
 */
SD_LOCAL Std_ReturnType
    Sd_OptionsExistCheck(const Sd_InstanceType* instancePtr, const uint8* sdMsg, uint16 entryArrayLen, uint8 optNum)
{
    Std_ReturnType ret             = E_OK;
    uint8          optNumFromEntry = 0u;

    /* Calculate options number from entries */
    uint16 entryArrayEndIdx = entryArrayLen + SD_FIRST_ENTRY_INDEX;
    for (uint16 index = SD_FIRST_ENTRY_INDEX; index < entryArrayEndIdx; index += SD_ENTRY_SIZE)
    {
        Sd_Type1EntryType type1Entry;
        (void)IStdLib_MemCpy((uint8*)&type1Entry, &sdMsg[index], sizeof(Sd_Type1EntryType));
        uint8 index1stOption = type1Entry.Index1stOptions;
        uint8 index2ndOption = type1Entry.Index2ndOptions;
        uint8 numOfOpt1      = type1Entry.NumOfOpt1;
        uint8 numOfOpt2      = type1Entry.NumOfOpt2;

        uint8 tOptNum = index1stOption + numOfOpt1;
        if (tOptNum > optNumFromEntry)
        {
            optNumFromEntry = tOptNum;
        }
        tOptNum = index2ndOption + numOfOpt2;
        if (tOptNum > optNumFromEntry)
        {
            optNumFromEntry = tOptNum;
        }
    }

    /*@req <SWS_SD_00708> */
    if (optNumFromEntry > optNum)
    {
#if (SD_DEM_USED == STD_ON)
        const Sd_DemEventParameterType* demEventPtr;
        demEventPtr = instancePtr->SdInstanceDemEventParameterRefs->SD_E_MALFORMED_MSG;
        if (NULL_PTR != demEventPtr)
        {
            (void)Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
        }
#else
        SD_UNUSED(instancePtr);
#endif /* SD_DEM_USED == STD_ON */

        ret = E_NOT_OK;
    }

    return ret;
}
#endif

/**
 * @brief               Process when receive FindService entry
 * @param[in]           instancePtr Pointer to the SD instance configuration.
 * @param[in]           rxQueueItem Pointer to the received queue item.
 * @param[in]           entryPos Position of the entry in the message.
 * @param[in]           isRxUseMulticast Flag indicating if multicast is used.
 * @param[in]           optStartPosMap Pointer to the start positions of options.
 * @param[in]           applicationId Application ID.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74067
 */
#if (SD_SERVER_SERVICE_NUM > 0)
/* PRQA S 6030,6040 ++ */ /* VL_MTR_Sd_STMIF,VL_MTR_Sd_STPAR */
SD_LOCAL void Sd_RxFindServiceHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 entryPos,
    boolean                isRxUseMulticast,
    const uint16           optStartPosMap[],
    ApplicationType        applicationId)
/* PRQA S 6030,6040 -- */
{
    const uint8*      sdMsg = rxQueueItem->BufPtr;
    Sd_Type1EntryType type1Entry;
    (void)IStdLib_MemCpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    /* Pick up fields value from entry */
    uint16 serviceID    = Sd_NtoHs(type1Entry.ServiceID);
    uint16 InstanceID   = Sd_NtoHs(type1Entry.InstanceID);
    uint8  majorVersion = type1Entry.MajorVersion;
    uint32 minorVersion = Sd_NtoHl(type1Entry.MinorVersion);

    const Sd_ServerServiceType* serverServicePtr = instancePtr->SdServerService;
    for (uint16 serviceIdx = 0u; serviceIdx < instancePtr->ServerServiceNum; serviceIdx++)
    {
/* @req SWS_SD_00717,SWS_SD_00718 */
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
        boolean cfgOptMatchCheckRet = Sd_RxConfigurationOptionMatchCheck(
            instancePtr,
            serverServicePtr,
            NULL_PTR,
            rxQueueItem,
            entryPos,
            optStartPosMap,
            applicationId);
#else
        SD_UNUSED(optStartPosMap);
#endif
        /*@req <SWS_SD_00486> */
        if (((SD_MESSAGE_SERVICE_ID_ANY == serviceID) || (serviceID == serverServicePtr->SdServerServiceId))
            && ((SD_MESSAGE_INSTANCE_ID_ANY == InstanceID)
                || (InstanceID == serverServicePtr->SdServerServiceInstanceId))
            && ((SD_MESSAGE_MAJOR_VERSION_ANY == majorVersion)
                || (majorVersion == serverServicePtr->SdServerServiceMajorVersion))
            && ((SD_MESSAGE_MINOR_VERSION_ANY == minorVersion)
                || (minorVersion == serverServicePtr->SdServerServiceMinorVersion))
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
            && cfgOptMatchCheckRet
#endif
        )
        {
            Sd_SaveServerServiceHandleIdToSessionCtrlList(
                instancePtr->SdInstanceHandleId,
                &rxQueueItem->RemoteAddr,
                serverServicePtr->SdServerServiceHandleId,
                applicationId);

            Sd_ServerServicePhaseType serverPhase = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId]->Phase;
            if ((SD_SERVER_PHASE_REPETITION == serverPhase) || (SD_SERVER_PHASE_MAIN == serverPhase))
            {
                /*@req <SWS_SD_00332>,<SWS_SD_00343> */
                uint32 sendTime = Sd_CalcServerServiceDelayTime(serverServicePtr, NULL_PTR, isRxUseMulticast);
                if ((sdMsg[SD_HEADER_FLAGS_INDEX] & SD_FLAGS_UNICAST_FLAG_MASK) > 0u)
                {
                    const Sd_ServerTimerType* timerPtr = serverServicePtr->SdServerServiceTimerRef;
                    uint32                    halfMainTime =
                        (timerPtr->SdServerTimerOfferCyclicDelay >> 1UL) / SD_MAIN_FUNCTION_CYCLE_TIME;
                    if ((SD_SERVER_PHASE_MAIN == serverPhase) && (timerPtr->SdServerTimerOfferCyclicDelay > 0UL)
                        && (Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId]->MainTimer < halfMainTime))
                    {
                        /*@req <PRS_SOMEIPSD_00423>,<PRS_SOMEIPSD_00843>*/
                        /* Find messages received with the Unicast Flag set to 1 in main phase, shall
                         * be answered with a multicast RESPONSE if the latest offer was sent 1/2
                         * CYCLIC_OFFER_DELAY or longer ago.*/
                        Sd_BuildOfferServiceEntry(
                            instancePtr,
                            serverServicePtr,
                            rxQueueItem,
                            sendTime,
                            FALSE,
                            FALSE,
                            applicationId);
                    }
                    else
                    {
                        /* Find messages received in repetition phase,answered with unicast */
                        /* Find messages received with the Unicast Flag set to 1 in main phase, shall
                         * be answered with a unicast response if the latest offer was sent less than 1/2
                         * CYCLIC_OFFER_DELAY ago.*/
                        Sd_BuildOfferServiceEntry(
                            instancePtr,
                            serverServicePtr,
                            rxQueueItem,
                            sendTime,
                            TRUE,
                            FALSE,
                            applicationId);
                    }
                }
                else
                {
                    /*@req <SWS_SD_00650>,<PRS_SOMEIPSD_00423>,<PRS_SOMEIPSD_00843> */
                    /* Find messages received with Unicast Flag set to 0 (multicast),
                     * Ignore the message. */
                }
            }
            else
            {
                /*@req <SWS_SD_00319> */
                /* If a FindService Entry is received within the Initial Wait Phase
                 * for this Server Service Instance, it shall be ignored.*/

                /*@req <SWS_SD_00710> */
                /* Does not hava necessary options, ignore this entry */
            }
        }
        serverServicePtr++;
    }
}

/**
 * @brief              Get a random value in range min and max.
 * @param[in]          min: Minimum Value of the Random Number.
 * @param[in]          max: Max Value of the Random Number.
 * @return             Value of the Random Number.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74111
 */
SD_LOCAL uint32 Sd_GetRandomValue(uint32 min, uint32 max)
{
    /* Calculate random value use LCG algorithm whih parameter from Numerical Recipe */
    uint32 randVal = (Sd_RandomSeed * SD_RANDOM_MULTIPLIER) + SD_RANDOM_INCREMENT;

    /* Result in range min and max */
    return (min + (randVal % (max - min + 1UL))); /* PRQA S 2834 */ /* VL_Sd_Division */
}

/**
 * @brief              Calculate server service delay time
 *                     If serverServicePtr not equal NULL_PTR,calc time for server service
 *                     If eventHandlerPtr not equal NULL_PTR,calc time for EventHandler
 * @param[in]          serverServicePtr: Pointer of server service.
 * @param[in]          eventHandlerPtr: Pointer of event handler.
 * @param[in]          isRxUseMulticast: Is use multicast receive message.
 * @return             Value of the Random Number.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74068
 */
SD_LOCAL uint32 Sd_CalcServerServiceDelayTime(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    boolean                     isRxUseMulticast)
{
    uint32 tRandom;

    if (isRxUseMulticast)
    {
        const Sd_ServerTimerType* timerPtr;

        /*@req <SWS_SD_00491>,<SWS_SD_00492>,<SWS_SD_00493>,<SWS_SD_00494> */
        /* make the number between SdServerTimerRequestResponseMinDelay to SdServerTimerRequestResponseMaxDelay*/
        if (NULL_PTR != serverServicePtr)
        {
            timerPtr = serverServicePtr->SdServerServiceTimerRef;
        }
        else
        {
            timerPtr = eventHandlerPtr->SdEventHandlerTimerRef;
        }
        uint32 delayMin = timerPtr->SdServerTimerRequestResponseMinDelay;
        uint32 delayMax = timerPtr->SdServerTimerRequestResponseMaxDelay;
        /*@req <SWS_SD_00494>,<SWS_SD_00725> */
        /* Calculate random delay time */
        tRandom = Sd_GetRandomValue(delayMin, delayMax);
        /* ms to times */
        tRandom = tRandom / SD_MAIN_FUNCTION_CYCLE_TIME;
    }
    else
    {
        /* The entry not received by Multicast RxPduId, needn't delay */
        tRandom = SD_SEND_ENTRY_IMMEDIATELY;
    }

    return tRandom;
}

#endif

/**
 * @brief               Process when receive OfferService entry
 * @param[in]           instancePtr Pointer to the SD instance configuration.
 * @param[in]           rxQueueItem Pointer to the received queue item.
 * @param[in]           entryPos Position of the entry in the message.
 * @param[in]           optStartPosMap Pointer to the start positions of options.
 * @param[in]           applicationId Application ID.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74069
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
/* PRQA S 6020,6030,6070 ++ */ /* VL_MTR_Sd_STLIN,VL_MTR_Sd_STMIF,VL_MTR_Sd_STCAL */
SD_LOCAL void Sd_RxOfferServiceHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 entryPos,
    const uint16           optStartPosMap[],
    ApplicationType        applicationId)
/* PRQA S 6020,6030,6070 -- */
{
    Sd_Type1EntryType type1Entry;
    const uint8*      sdMsg = rxQueueItem->BufPtr;
    (void)IStdLib_MemCpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    /* Pick up fields value from entry */
    uint16 serviceID    = Sd_NtoHs(type1Entry.ServiceID);
    uint16 InstanceID   = Sd_NtoHs(type1Entry.InstanceID);
    uint8  majorVersion = type1Entry.MajorVersion;

    uint32 ttl = 0u;
    /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
    (void)IStdLib_MemCpy((((uint8*)&ttl) + 1u), type1Entry.TTL, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
    /* PRQA S 0489 -- */
    ttl                 = Sd_NtoHl(ttl);
    uint32 minorVersion = Sd_NtoHl(type1Entry.MinorVersion);

    const Sd_ClientServiceType* clientServicePtr = instancePtr->SdClientService;
    for (uint16 serviceIdx = 0u; serviceIdx < instancePtr->ClientServiceNum; serviceIdx++)
    {
/* @req SWS_SD_00717,SWS_SD_00718 */
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
        boolean cfgOptMatchCheckRet = Sd_RxConfigurationOptionMatchCheck(
            instancePtr,
            NULL_PTR,
            clientServicePtr,
            rxQueueItem,
            entryPos,
            optStartPosMap,
            applicationId);
#endif

        if ((serviceID == clientServicePtr->SdClientServiceId)
            && (InstanceID == clientServicePtr->SdClientServiceInstanceId)
            && (majorVersion == clientServicePtr->SdClientServiceMajorVersion)
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
            && (TRUE == cfgOptMatchCheckRet)
#endif
        )
        {
            boolean isBlockVersion = FALSE;
#if (SD_CLIENT_BLOCK_LIST_USED == STD_ON)
            const Sd_BlocklistedVersionType* minorVersionBlockList = clientServicePtr->SdBlocklistedVersions;
            for (uint32 index = 0UL; index < clientServicePtr->SdBlocklistedMinorVersionNum; index++)
            {
                /*@req <SWS_Sd_00716> */
                if (minorVersion == minorVersionBlockList[index].BlocklistedMinorVersions)
                {
                    isBlockVersion = TRUE;
                    break;
                }
            }
#endif

            Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
            /*@req <SWS_SD_00488>,<SWS_SD_00489>,<SWS_Sd_04089> */
            if ((isBlockVersion)
                || ((SD_EXACT_OR_ANY_MINOR_VERSION == clientServicePtr->SdVersionDrivenFindBehavior)
                    && ((SD_MESSAGE_MINOR_VERSION_ANY != clientServicePtr->SdClientServiceMinorVersion)
                        && (minorVersion != clientServicePtr->SdClientServiceMinorVersion)))
                || ((SD_MINIMUM_MINOR_VERSION == clientServicePtr->SdVersionDrivenFindBehavior)
                    && ((minorVersion < clientServicePtr->SdClientServiceMinorVersion)
                        || Sd_IsOfferServiceBeIgnore(cRtDataPtr, clientServicePtr, rxQueueItem))))
            {
                continue; /* PRQA S 0770 */ /* VL_Sd_Continue */
            }

            Sd_SaveClientServiceHandleIdToSessionCtrlList(
                instancePtr->SdInstanceHandleId,
                &rxQueueItem->RemoteAddr,
                clientServicePtr->SdClientServiceHandleId,
                applicationId);

            Std_ReturnType optionCheck = Sd_NecessaryOptionsExistCheck(
                instancePtr,
                NULL_PTR,
                clientServicePtr,
                rxQueueItem,
                entryPos,
                optStartPosMap,
                applicationId);
            if (E_OK == optionCheck)
            {
                cRtDataPtr->OfferServiceRxFlag      = TRUE;
                cRtDataPtr->RxTcpEndpointFlag       = FALSE;
                cRtDataPtr->RxUdpEndpointFlag       = FALSE;
                cRtDataPtr->RxMulitcastEndpointFlag = FALSE;
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE) && (SD_CONSUMED_EVENTGROUP_NUM > 0)
                Sd_ClientServiceResetAllCEgSubsEntrySubsRetryCnt(clientServicePtr);
#endif

                /* OfferService */
                if (0UL != ttl)
                {
                    if (SD_CLIENT_PHASE_DOWN == cRtDataPtr->Phase)
                    {
                        /*@req <SWS_SD_00514> */
                        /* If TTL is set to 0xFFFFFF, the OfferService entry shall be considered valid until the next
                         * reboot.*/
                        Sd_SetTTL(&cRtDataPtr->Ttl, ttl);

                        Sd_ClientHandleSoConBasedOptionInfo(
                            instancePtr,
                            clientServicePtr,
                            sdMsg,
                            entryPos,
                            optStartPosMap,
                            cRtDataPtr);
                    }
                    else
                    {
                        /*@req <SWS_SD_00352> */
                        /* The calculated random timer, which has been started when entering the Initial
                         * Wait Phase, shall be canceled. */
                        cRtDataPtr->InitialWaitTimer = 0u;

                        /*@req <SWS_SD_00365> */
                        /* Cancel the repetition timer */
                        cRtDataPtr->RepetitionTimer = 0u;

                        /*@req <SWS_SD_00352>,<SWS_SD_00365>,<SWS_SD_00376> */
                        /* A timer shall be set to the TTL value of this entry (TTL timer). */
                        Sd_SetTTL(&cRtDataPtr->Ttl, ttl);

                        Sd_ClientHandleSoConBasedOptionInfo(
                            instancePtr,
                            clientServicePtr,
                            sdMsg,
                            entryPos,
                            optStartPosMap,
                            cRtDataPtr);

                        /*@req <SWS_SD_00352>,<SWS_SD_00365> */
                        /* Leave the Initial Wait Phase Enter the Main Phase */
                        if (SD_CLIENT_PHASE_MAIN != cRtDataPtr->Phase)
                        {
                            cRtDataPtr->Phase = SD_CLIENT_PHASE_MAIN;
                        }
                    }

                    /* Save received offer service communication partner information */
                    cRtDataPtr->OfferEntryRemoteAddr = rxQueueItem->RemoteAddr;
                    cRtDataPtr->OfferEntryRxPduId    = rxQueueItem->RxPduId;
                }
                else
                {
                    /* StopOfferService */
                    Sd_RxStopOfferServiceHandle(clientServicePtr->SdClientServiceHandleId);
                }
            }
            else
            {
                /*@req <SWS_SD_00710> */
                /* Does not hava necessary options, ignore this entry */
            }
        }

        clientServicePtr++;
    }
}
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

/**
 * @brief              Handles the response to a subscribe entry.
 * @param[in]          eventHandlerPtr: Pointer to the event handler type.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          serverServicePtr: Pointer to the server service type.
 * @param[in]          rxQueueItem: Pointer to the received queue item.
 * @param[in]          ttl: Time-to-live value.
 * @param[in]          matchFlag: Flag indicating if the entry matches.
 * @param[in]          result: Result of the operation.
 * @param[in]          fanOutControlFlag: Flag for fan-out control.
 * @param[in]          entryPos: Position of the entry in the message.
 * @param[in]          counter: Counter value.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74552
 */
/* PRQA S 6030,6040 ++ */ /* VL_MTR_Sd_STMIF,VL_MTR_Sd_STPAR */
#if (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL void Sd_ResponseSubscribeEntry(
    const Sd_EventHandlerType*  eventHandlerPtr,
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint32                      ttl,
    boolean                     matchFlag,
    Std_ReturnType              result,
    boolean                     fanOutControlFlag,
    uint16                      entryPos,
    uint8                       counter,
    ApplicationType             applicationId)
/* PRQA S 6030,6040 -- */
{
    if (ttl > 0UL)
    {
        if ((matchFlag) && (E_NOT_OK == result))
        {
            /*@req <SWS_SD_00430> */
            /* Response with SubscribeEventgroupNAck */
            (void)Sd_BuildSubscribeEventgroupNAckEntry(serverServicePtr, rxQueueItem, entryPos, applicationId);
        }
        else if (!matchFlag)
        {
            /* No match ServerService,Add NACK entry to the first ServerService's send queue*/
            (void)Sd_BuildSubscribeEventgroupNAckEntry(
                instancePtr->SdServerService,
                rxQueueItem,
                entryPos,
                applicationId);
        }
        else if (!fanOutControlFlag)
        {
            /*@req <SWS_SD_00430> */
            /* Response with SubscribeEventgroupNAck */
            (void)Sd_BuildSubscribeEventgroupNAckEntry(serverServicePtr, rxQueueItem, entryPos, applicationId);
        }
        else
        {
            /* Response with SubscribeEventgroupAck */
            Std_ReturnType ret = Sd_BuildSubscribeEventgroupAckEntry(
                instancePtr,
                serverServicePtr,
                eventHandlerPtr,
                &rxQueueItem->RemoteAddr,
                ttl,
                counter,
                applicationId);
            if (E_OK == ret)
            {
                /*@req <SWS_SD_00333>,<SWS_SD_00344> */
                /* Call the BswM with the API BswM_Sd_EventHandlerCurrentState() with state
                 * SD_EVENT_HANDLER_REQUESTED only if the state for this EventHandler changed */
                uint16 eventHandlerId = eventHandlerPtr->SdEventHandlerHandleId;
                if (SD_EVENT_HANDLER_REQUESTED != Sd_EventHandlerRTData[eventHandlerId]->CurState)
                {
#if (SD_BSWM_USED == STD_ON)
                    BswM_Sd_EventHandlerCurrentState(eventHandlerId, SD_EVENT_HANDLER_REQUESTED);
#endif
                    Sd_EventHandlerRTData[eventHandlerId]->CurState = SD_EVENT_HANDLER_REQUESTED;
                }
            }
        }
    }
}
#endif

/**
 * @brief               Process when receive SubscribeEventgroup entry.
 * @param[in]           instancePtr Pointer to the SD instance configuration.
 * @param[in]           rxQueueItem Pointer to the received queue item.
 * @param[in]           entryPos Position of the entry in the message.
 * @param[in]           optStartPosMap Pointer to the start positions of options.
 * @param[in]           isReplyNAck Flag indicating if a negative acknowledgment should be replied.
 * @param[in]           applicationId Application ID.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74071
 */
#if (SD_EVENT_HANDLER_NUM > 0)
/* PRQA S 6020,6030,6040,6070 ++ */ /* VL_MTR_Sd_STLIN,VL_MTR_Sd_STMIF,VL_MTR_Sd_STPAR,VL_MTR_Sd_STCAL */
SD_LOCAL void Sd_RxSubscribeEventgroupHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 entryPos,
    const uint16           optStartPosMap[],
    boolean                isReplyNAck,
    ApplicationType        applicationId)
/* PRQA S 6020,6030,6040,6070 -- */
{
    Sd_Type2EntryType type2Entry;
    const uint8*      sdMsg = rxQueueItem->BufPtr;
    (void)IStdLib_MemCpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));

    uint32 ttl = 0u;
    /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
    (void)IStdLib_MemCpy((((uint8*)&ttl) + 1u), type2Entry.TTL, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
    /* PRQA S 0489 -- */
    ttl = Sd_NtoHl(ttl);

    /* Pick up fields value from entry */
    uint16 serviceID    = Sd_NtoHs(type2Entry.ServiceID);
    uint16 instanceID   = Sd_NtoHs(type2Entry.InstanceID);
    uint8  majorVersion = type2Entry.MajorVersion;

    uint8                       counter           = (uint8)type2Entry.Counter;
    Std_ReturnType              ret               = E_NOT_OK;
    uint16                      eventgroupID      = Sd_NtoHs(type2Entry.EventGroupID);
    const Sd_ServerServiceType* serverServicePtr  = instancePtr->SdServerService;
    const Sd_EventHandlerType*  eventHandlerPtr   = NULL_PTR;
    boolean                     matchFlag         = FALSE;
    boolean                     fanOutControlFlag = FALSE;

    for (uint16 serviceIdx = 0u; serviceIdx < instancePtr->ServerServiceNum; serviceIdx++)
    {
        /*@req <SWS_SD_00490> */
        if ((serviceID == serverServicePtr->SdServerServiceId)
            && (instanceID == serverServicePtr->SdServerServiceInstanceId)
            && (majorVersion == serverServicePtr->SdServerServiceMajorVersion))
        {
            Sd_SaveServerServiceHandleIdToSessionCtrlList(
                instancePtr->SdInstanceHandleId,
                &rxQueueItem->RemoteAddr,
                serverServicePtr->SdServerServiceHandleId,
                applicationId);

            boolean cfgOptMatchCheckRet = TRUE;
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
            cfgOptMatchCheckRet = Sd_RxConfigurationOptionMatchCheck(
                instancePtr,
                serverServicePtr,
                NULL_PTR,
                rxQueueItem,
                entryPos,
                optStartPosMap,
                applicationId);
#endif

            eventHandlerPtr = serverServicePtr->SdEventHandler;
            for (uint16 eventGroupIdx = 0u; (eventGroupIdx < serverServicePtr->EventHandlerNum) && cfgOptMatchCheckRet;
                 eventGroupIdx++)
            {
                if (eventgroupID == eventHandlerPtr->SdEventHandlerEventGroupId)
                {
                    Std_ReturnType optionCheck = Sd_NecessaryOptionsExistCheck(
                        instancePtr,
                        serverServicePtr,
                        NULL_PTR,
                        rxQueueItem,
                        entryPos,
                        optStartPosMap,
                        applicationId);
                    uint16                    serviceHandleId = serverServicePtr->SdServerServiceHandleId;
                    Sd_ServerServicePhaseType serverPhase     = Sd_ServerRTData[serviceHandleId]->Phase;
                    if ((E_OK == optionCheck) && (SD_SERVER_PHASE_DOWN != serverPhase) && (!isReplyNAck))
                    {
                        uint8 tcpEndpointNum = 0u;
                        uint8 udpEndpointNum = 0u;
                        Sd_CalcTcpUdpEndpointOptionNum(
                            sdMsg,
                            entryPos,
                            optStartPosMap,
                            &tcpEndpointNum,
                            &udpEndpointNum);

                        ret = E_OK;
/*@req <SWS_SD_00320>,<SWS_SD_00333>,<SWS_SD_00344> */
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
                        if ((tcpEndpointNum > 0u) && (NULL_PTR != eventHandlerPtr->SdEventHandlerTcp))
                        {
                            ret = Sd_FanOutControlTCP(
                                instancePtr,
                                serverServicePtr,
                                eventHandlerPtr,
                                rxQueueItem->RemoteAddr,
                                sdMsg,
                                entryPos,
                                optStartPosMap,
                                applicationId);

                            fanOutControlFlag = TRUE;
                        }
#endif

#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
                        /* PRQA S 2995 ++ */ /* VL_Sd_LoopLogical */
                        if ((E_OK == ret) && (udpEndpointNum > 0u) && (NULL_PTR != eventHandlerPtr->SdEventHandlerUdp))
                        {
                            ret = Sd_FanOutControlUDP(
                                instancePtr,
                                serverServicePtr,
                                eventHandlerPtr,
                                rxQueueItem->RemoteAddr,
                                sdMsg,
                                entryPos,
                                optStartPosMap,
                                applicationId);

                            fanOutControlFlag = TRUE;

                            /* UDP fail,need close TCP opened RoutingPath */
                            if ((tcpEndpointNum > 0u) && (NULL_PTR != eventHandlerPtr->SdEventHandlerTcp)
                                && (E_NOT_OK == ret))
                            {
                                Sd_EventHandlerRTType* ehRTDataPtr =
                                    Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
                                SoAd_RoutingGroupIdType routingGroupId =
                                    eventHandlerPtr->SdEventHandlerTcp->SdEventActivationRef->RoutingGroupId;
                                (void)SoAd_DisableSpecificRouting(routingGroupId, ehRTDataPtr->SoConIdTcp);
                                Sd_DeleteClientFromSubscribeList(
                                    serverServicePtr,
                                    eventHandlerPtr,
                                    &ehRTDataPtr->TcpSubscribeList,
                                    NULL_PTR,
                                    ehRTDataPtr->SoConIdTcp,
                                    counter,
                                    applicationId);
                                Sd_DelClientFromTriggerTransmitListBySoConId(
                                    &ehRTDataPtr->TcpTriggerTransmitList,
                                    ehRTDataPtr->SoConIdTcp,
                                    applicationId);
                            }
                        }
                        /* PRQA S 2995 -- */ /* VL_Sd_LoopLogical */
#endif
                    }
                    else
                    {
                        /* 1, Receive SubscribeEventgroup entry in Down Phase */
                        /* AUTOSAR Specification not describe how to do in this case ,now response with NACK */

                        /*@req <SWS_SD_00710> */
                        /* 2, Necessary options does not exist,response with NACK */

                        /* TC8 [SOMEIP_ETS_109,SOMEIP_ETS_110,SOMEIP_ETS_112] */
                        /* 3, SubscribeEventgroup Message without specifying a valid IP address in the Endpoint
                         * option, sends SubscribeEventgroupNAck */
                        ret = E_NOT_OK;
                    }

                    /* Find matched server service,stop search */
                    matchFlag = TRUE;
                    break;
                }
                eventHandlerPtr++;
            }

            if (matchFlag)
            {
                break;
            }
        }
        serverServicePtr++;
    }

    /* Response to SubscribeEventgroup, StopSubscribe do not need an Ack */
    Sd_ResponseSubscribeEntry(
        eventHandlerPtr,
        instancePtr,
        serverServicePtr,
        rxQueueItem,
        ttl,
        matchFlag,
        ret,
        fanOutControlFlag,
        entryPos,
        counter,
        applicationId);
}
#endif /* SD_EVENT_HANDLER_NUM > 0 */

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
#if ((SD_DEM_USED == STD_ON) || (SD_CLIENT_SERVICE_USE_TCP == STD_ON))
/**
 * @brief              Handles the reception of a SubscribeEventgroupNAck message.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          cRtDataPtr: Pointer to the client service runtime data.
 * @param[in]          clientServicePtr: Pointer to the client service type.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74563
 */
SD_LOCAL void Sd_RxSubscribeEventgroupNAckHandle(
    const Sd_InstanceType*        instancePtr,
    const Sd_ClientServiceRTType* cRtDataPtr,
    const Sd_ClientServiceType*   clientServicePtr)
{
#if (SD_DEM_USED == STD_ON)
    const Sd_DemEventParameterType* demEventPtr;
    demEventPtr = instancePtr->SdInstanceDemEventParameterRefs->SD_E_SUBSCR_NACK_RECV;
    if (NULL_PTR != demEventPtr)
    {
        (void)Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
    }
#endif

#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
    TcpIp_SockAddrType tcpIpSockAddr;
    tcpIpSockAddr.domain = instancePtr->SdAddrFamily;
    (void)SoAd_GetRemoteAddr(cRtDataPtr->TcpSoCon.SoConId, &tcpIpSockAddr);
    /* If SdClientServiceTcpRef is configured,restart the TCP connection */
    if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef)
        && (TCPIP_E_OK != SoAd_IsConnectionReady(cRtDataPtr->TcpSoCon.SoConId, &tcpIpSockAddr)))
    {
        (void)SoAd_CloseSoCon(cRtDataPtr->TcpSoCon.SoConId, TRUE);
        (void)SoAd_OpenSoCon(cRtDataPtr->TcpSoCon.SoConId);
    }
#endif
}
#endif

/**
 * @brief              Handles the SubscribeAck option.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          consumedEventGroupPtr: Pointer to the consumed event group type.
 * @param[in]          cRtDataPtr: Pointer to the client service runtime data.
 * @param[in]          consumedEgRTPtr: Pointer to the consumed event group runtime data.
 * @param[in]          sdMsg: Buffer containing the Service Discovery message.
 * @param[in]          entryPos: Position of the entry in the message.
 * @param[in]          optStartPosMap: Array containing the start positions of each option.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74564
 */
#if (SD_CONSUMED_EVENT_MULICAST_USED == STD_ON)
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL void Sd_SubscribeAckOptionHandle(
    const Sd_InstanceType*           instancePtr,
    const Sd_ConsumedEventGroupType* consumedEventGroupPtr,
    const Sd_ClientServiceRTType*    cRtDataPtr,
    Sd_ConsumedEventGroupRTType*     consumedEgRTPtr,
    const uint8*                     sdMsg,
    uint16                           entryPos,
    const uint16                     optStartPosMap[])
/* PRQA S 6040 -- */
{
    /*@req <SWS_SD_00704>,<SWS_SD_00377> */
    if (NULL_PTR != consumedEventGroupPtr->SdConsumedEventGroupMulticastGroupRef)
    {
        TcpIp_SockAddrType multicastIpAddr;
        Std_ReturnType     ret =
            Sd_GetInfoFromMulticastOption(instancePtr, sdMsg, entryPos, optStartPosMap, &multicastIpAddr);
        if (E_OK == ret)
        {
            Sd_ClientSaveMulticastSoConId(
                instancePtr,
                consumedEventGroupPtr,
                &multicastIpAddr,
                &cRtDataPtr->ServerUdpAddr,
                &consumedEgRTPtr->MultiSoCon);
        }
    }
}
#endif

/**
 * @brief               Process when receive SubscribeEventgroupAck entry
 * @param[in]           instancePtr Pointer to the SD instance configuration.
 * @param[in]           rxQueueItem Pointer to the received queue item.
 * @param[in]           entryPos Position of the entry in the message.
 * @param[in]           optStartPosMap Pointer to the start positions of options.
 * @param[in]           applicationId Application ID.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74072
 */
/* PRQA S 6030,6070 ++ */ /* VL_MTR_Sd_STMIF,VL_MTR_Sd_STCAL */
SD_LOCAL void Sd_RxSubscribeEventgroupAckHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType*  rxQueueItem,
    uint16                 entryPos,
    const uint16           optStartPosMap[],
    ApplicationType        applicationId)
/* PRQA S 6030,6070 -- */
{
    Sd_Type2EntryType type2Entry;
    const uint8*      sdMsg = rxQueueItem->BufPtr;
    (void)IStdLib_MemCpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));

    uint32 ttl = 0u;
    /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
    (void)IStdLib_MemCpy((((uint8*)&ttl) + 1u), type2Entry.TTL, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
    /* PRQA S 0489 -- */
    ttl                 = Sd_NtoHl(ttl);
    uint16 eventgroupID = Sd_NtoHs(type2Entry.EventGroupID);

    /* Pick up fields value from entry */
    uint16 serviceID    = Sd_NtoHs(type2Entry.ServiceID);
    uint16 InstanceID   = Sd_NtoHs(type2Entry.InstanceID);
    uint8  majorVersion = type2Entry.MajorVersion;

    const Sd_ClientServiceType* clientServicePtr = instancePtr->SdClientService;
    boolean                     matchFlag        = FALSE;
    for (uint16 serviceIdx = 0u; (serviceIdx < instancePtr->ClientServiceNum) && !matchFlag; serviceIdx++)
    {
        /*@req <SWS_SD_00486> */
        if ((serviceID == clientServicePtr->SdClientServiceId)
            && (InstanceID == clientServicePtr->SdClientServiceInstanceId)
            && (majorVersion == clientServicePtr->SdClientServiceMajorVersion))
        {
            Sd_SaveClientServiceHandleIdToSessionCtrlList(
                instancePtr->SdInstanceHandleId,
                &rxQueueItem->RemoteAddr,
                clientServicePtr->SdClientServiceHandleId,
                applicationId);

            boolean cfgOptMatchCheckRet = TRUE;
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
            cfgOptMatchCheckRet = Sd_RxConfigurationOptionMatchCheck(
                instancePtr,
                NULL_PTR,
                clientServicePtr,
                rxQueueItem,
                entryPos,
                optStartPosMap,
                applicationId);
#endif

            const Sd_ConsumedEventGroupType* consumedEventGroupPtr = clientServicePtr->SdConsumedEventGroup;
            for (uint16 consumedEventGroupIdx = 0u;
                 (consumedEventGroupIdx < clientServicePtr->ConsumedEventGroupNum) && cfgOptMatchCheckRet;
                 consumedEventGroupIdx++)
            {
                const Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
                Sd_ConsumedEventGroupRTType*  consumedEgRTPtr =
                    Sd_ConsumedEventGroupRTData[consumedEventGroupPtr->SdConsumedEventGroupHandleId];
                if (eventgroupID == consumedEventGroupPtr->SdConsumedEventGroupId)
                {
                    /* SubscribeEventgroupAck */
                    if (0UL != ttl)
                    {
                        consumedEgRTPtr->IsAnsweredWithAck   = TRUE;
                        consumedEgRTPtr->IsTcpSubsSend       = cRtDataPtr->TcpSoCon.IsSoConOpened ? TRUE : FALSE;
                        consumedEgRTPtr->IsUdpSubsSend       = cRtDataPtr->UdpSoCon.IsSoConOpened ? TRUE : FALSE;
                        consumedEgRTPtr->IsMulitcastSubsSend = cRtDataPtr->MulitcastSoCon.IsSoConOpened ? TRUE : FALSE;

                        /*@req <SWS_SD_00377> */
                        if (!consumedEgRTPtr->IsSubscribed)
                        {
                            consumedEgRTPtr->IsSubscribed = TRUE;
#if (SD_CONSUMED_EVENT_MULICAST_USED == STD_ON)
                            Sd_SubscribeAckOptionHandle(
                                instancePtr,
                                consumedEventGroupPtr,
                                cRtDataPtr,
                                consumedEgRTPtr,
                                sdMsg,
                                entryPos,
                                optStartPosMap);
#endif

#if (SD_BSWM_USED == STD_ON)
                            BswM_Sd_ConsumedEventGroupCurrentState(
                                consumedEventGroupPtr->SdConsumedEventGroupHandleId,
                                SD_CONSUMED_EVENTGROUP_AVAILABLE);
#endif
                            consumedEgRTPtr->CurState = SD_CONSUMED_EVENTGROUP_AVAILABLE;
                            Sd_SetTTL(&consumedEgRTPtr->Ttl, ttl);
                        }
                    }
                    else
                    {
#if ((SD_DEM_USED == STD_ON) || (SD_CLIENT_SERVICE_USE_TCP == STD_ON))
                        /* SubscribeEventgroupNAck */
                        /*@req <SWS_SD_00465> */
                        uint8   counter   = (uint8)type2Entry.Counter;
                        boolean isFindAck = Sd_IsHaveEventGroupAckInSameMessage(
                            sdMsg,
                            serviceID,
                            InstanceID,
                            eventgroupID,
                            majorVersion,
                            counter);

                        if (!isFindAck)
                        {
                            Sd_RxSubscribeEventgroupNAckHandle(instancePtr, cRtDataPtr, clientServicePtr);
                        }
#endif
                    }

/* @req SWS_SD_00738 */
/* Cancel the corresponding client service subscription retry delay timer and
 * reset subscription retry counter of all corresponding Eventgroups to 0 */
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
                    consumedEgRTPtr->SubsRetryTimer  = 0u;
                    consumedEgRTPtr->SubsRetryCnt    = 0u;
                    consumedEgRTPtr->SubsRetryEnable = FALSE;
#endif

                    /* Find matched client service,stop search */
                    matchFlag = TRUE;
                    break;
                }

                consumedEventGroupPtr++;
            }
        }
        clientServicePtr++;
    }
    SD_UNUSED(optStartPosMap);
}

/**
 * @brief              Get addr and port information from Multicast option
 * @param[in]          instancePtr: Pointer for instance.
 * @param[in]          sdMsg: Buffer containing the SD message.
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Array mapping the start positions of options in the SD message.
 * @param[out]         ipAddrPtr: Pointer to the structure where the extracted IP address information will be stored.
 * @return             E_OK: Find Multicast option and get info
 *                     E_NOT_OK: Get info failure
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74073
 */
#if (SD_CONSUMED_EVENT_MULICAST_USED == STD_ON)
SD_LOCAL Std_ReturnType Sd_GetInfoFromMulticastOption(
    const Sd_InstanceType* instancePtr,
    const uint8*           sdMsg,
    uint16                 entryPos,
    const uint16           optStartPosMap[],
    TcpIp_SockAddrType*    ipAddrPtr)
{
    Std_ReturnType ret = E_NOT_OK;

    Sd_Type2EntryType type2Entry;
    (void)IStdLib_MemCpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));
    uint8 idx1stOptions = type2Entry.Index1stOptions;
    uint8 numOpt1       = type2Entry.NumOfOpt1;
    uint8 idx2ndOptions = type2Entry.Index2ndOptions;
    uint8 numOpt2       = type2Entry.NumOfOpt2;

    uint8 optNumSum = numOpt1 + numOpt2;
    for (uint8 index = 0u; index < optNumSum; index++)
    {
        uint8 optIndex;
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        uint8 optType = sdMsg[optStartPosMap[optIndex] + SD_OPTION_TYPE_FILED_OFFSET];
        if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
        {
            if (SD_IPV4_MULTICAST_OPTION == optType)
            {
                Sd_IPv4OptionsType ipv4Opt;
                (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
                ipAddrPtr->domain = TCPIP_AF_INET;
                (void)IStdLib_MemCpy((uint8*)ipAddrPtr->addr, (const uint8*)ipv4Opt.IPv4Address, SD_IPV4_ADDRESS_LEN);
                ipAddrPtr->port = Sd_NtoHs(ipv4Opt.PortNumber);
                ret             = E_OK;
                break;
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else
        {
            if (SD_IPV6_MULTICAST_OPTION == optType)
            {
                Sd_IPv6OptionsType ipv6Opt;
                (void)IStdLib_MemCpy(uint8*)(&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
                ipAddrPtr->domain = TCPIP_AF_INET;
                (void)IStdLib_MemCpy((uint8*)ipAddrPtr->addr, (const uint8*)ipv6Opt.IPv6Address, 16u);
                ipAddrPtr->port = Sd_NtoHs(ipv6Opt.PortNumber);
                ret             = E_OK;
                break;
            }
        }
#endif /* STD_ON == SD_IPV6_ENABLE */
    }

    return ret;
}
#endif

#if ((SD_DEM_USED == STD_ON) || (SD_CLIENT_SERVICE_USE_TCP == STD_ON))
/**
 * @brief              Check if have SubscribeEventGroupAckEntry in same message
 * @param[in]          sdMsg: Pointer to the SD message buffer.
 * @param[in]          serviceID: The service ID to match.
 * @param[in]          instanceID: The instance ID to match.
 * @param[in]          eventgroupID: The event group ID to match.
 * @param[in]          majorVersion: The major version to match.
 * @param[in]          counter: The counter value to match.
 * @return             FALSE/TRUE
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74074
 */
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL boolean Sd_IsHaveEventGroupAckInSameMessage(
    const uint8* sdMsg,
    uint16       serviceID,
    uint16       instanceID,
    uint16       eventgroupID,
    uint8        majorVersion,
    uint8        counter)
/* PRQA S 6040 -- */
{
    boolean ret              = FALSE;
    uint16  entryArrayLen    = (uint16)Sd_NPtrtoHl(&sdMsg[SD_ENTRIES_ARRAY_LENGTH_INDEX]);
    uint16  entryArrayEndIdx = entryArrayLen + SD_FIRST_ENTRY_INDEX;

    Sd_Type2EntryType type2Entry;
    uint32            tTTL = 0u;
    for (uint16 idx = SD_FIRST_ENTRY_INDEX; idx < entryArrayEndIdx; idx += SD_ENTRY_SIZE)
    {
        (void)IStdLib_MemCpy((uint8*)&type2Entry, &sdMsg[idx], sizeof(Sd_Type2EntryType));
        uint8 entryType = type2Entry.Type;
        /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
        (void)IStdLib_MemCpy((((uint8*)&tTTL) + 1u), type2Entry.TTL, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
        /* PRQA S 0489 -- */
        tTTL = Sd_NtoHl(tTTL);
        if ((SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK == entryType) && (0UL != tTTL))
        {
            /* Pick up fields value from entry */
            uint16 tServiceID    = Sd_NtoHs(type2Entry.ServiceID);
            uint16 tInstanceID   = Sd_NtoHs(type2Entry.InstanceID);
            uint8  tMajorVersion = type2Entry.MajorVersion;
            uint8  tCounter      = (uint8)type2Entry.Counter;
            uint16 tEventgroupID = Sd_NtoHs(type2Entry.EventGroupID);
            if ((tServiceID == serviceID) && (tInstanceID == instanceID) && (tMajorVersion == majorVersion)
                && (tCounter == counter) && (tEventgroupID == eventgroupID))
            {
                ret = TRUE;
                break;
            }
        }
    }

    return ret;
}
#endif
#endif

/**
 * @brief               Build a FindService entry and options
 * @param[in]           sdInstancePtr Pointer to the SD instance configuration.
 * @param[in]           clientServicePtr Pointer to the client service configuration.
 * @param[in]           applicationId Application ID.
 * @return              E_OK: Build find service entry success
 *                      E_NOT_OK: Build find service entry fail.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace               CPD-74075
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL Std_ReturnType Sd_BuildFindServiceEntry(
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId)
{
    Std_ReturnType ret = E_OK;
    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    Sd_SendQueueType* sendQueueObjPtr =
        (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool[applicationId]);
    Sd_Type1EntryType* findSvsPtr =
        (Sd_Type1EntryType*)Sd_MallocEx(sizeof(Sd_Type1EntryType), Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */

#if (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON)
    uint16 cfgOptLen  = 0u;
    cfgOptLen         = Sd_GetType1ConfigurationOptionLength(sdInstancePtr, NULL_PTR, clientServicePtr);
    uint16 optionsLen = cfgOptLen;
    uint8* optionPtr  = NULL_PTR;
    if (optionsLen > 0u)
    {
        optionPtr = (uint8*)Sd_MallocEx(optionsLen, Sd_TxMemPool[applicationId]); /* PRQA S 0316 */ /* VL_QAC_0316 */
    }
#endif

    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == findSvsPtr)
#if (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON)
        || ((optionsLen > 0u) && (NULL_PTR == optionPtr))
#endif
    )
    {
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(findSvsPtr, Sd_TxMemPool[applicationId]);
#if (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON)
        Sd_FreeEx(optionPtr, Sd_TxMemPool[applicationId]);
#endif
        ret = E_NOT_OK;
    }
    else
    {
        /* Clear new allocated memory */
        (void)IStdLib_MemSet(sendQueueObjPtr, 0u, sizeof(Sd_SendQueueType));
        (void)IStdLib_MemSet(findSvsPtr, 0u, sizeof(Sd_Type1EntryType));
#if (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON)
        if (optionsLen > 0u)
        {
            (void)IStdLib_MemSet(optionPtr, 0, optionsLen);
        }
#endif

        findSvsPtr->Type            = SD_ENTRY_TYPE_FIND_SERVICE;
        findSvsPtr->Index1stOptions = 0u;
        findSvsPtr->Index2ndOptions = 0u;
        findSvsPtr->NumOfOpt1       = 0u;
        findSvsPtr->NumOfOpt2       = 0u;
        findSvsPtr->ServiceID       = Sd_HtoNs(clientServicePtr->SdClientServiceId);
        findSvsPtr->InstanceID      = Sd_HtoNs(clientServicePtr->SdClientServiceInstanceId);
        findSvsPtr->MajorVersion    = clientServicePtr->SdClientServiceMajorVersion;
        uint32 ttl                  = Sd_HtoNl(clientServicePtr->SdClientServiceTimerRef->SdClientTimerTTL);
        /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
        (void)IStdLib_MemCpy(findSvsPtr->TTL, ((uint8*)&ttl + 1), SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
        /* PRQA S 0489 -- */

        /*@req <SWS_Sd_00503>,<SWS_Sd_00752> */
        if (SD_EXACT_OR_ANY_MINOR_VERSION == clientServicePtr->SdVersionDrivenFindBehavior)
        {
            findSvsPtr->MinorVersion = Sd_HtoNl(clientServicePtr->SdClientServiceMinorVersion);
        }
        else if (SD_MINIMUM_MINOR_VERSION == clientServicePtr->SdVersionDrivenFindBehavior)
        {
            /*@req <SWS_Sd_10503> */
            findSvsPtr->MinorVersion = SD_MESSAGE_MINOR_VERSION_ANY;
        }
        else
        {
            // Do Nothing
        }

        uint8 optCnt = 0u;
#if (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON)
        if (cfgOptLen > 0u)
        {
            optCnt++;
            Sd_BuildType1ConfigurationOption(optionPtr, cfgOptLen, sdInstancePtr, NULL_PTR, clientServicePtr);
        }
#endif

        /* Save information to sendQueueObj */
        sendQueueObjPtr->EntryPtr = (uint8*)findSvsPtr;
#if (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON)
        sendQueueObjPtr->OptionPtr = optionPtr;
        sendQueueObjPtr->OptionLen = optionsLen;
#else
        sendQueueObjPtr->OptionPtr = NULL_PTR;
        sendQueueObjPtr->OptionLen = 0u;
#endif
        sendQueueObjPtr->OptionNum     = optCnt;
        sendQueueObjPtr->IsUnicastSend = FALSE;
        sendQueueObjPtr->SendTime      = SD_SEND_ENTRY_IMMEDIATELY;
        sendQueueObjPtr->Next          = NULL_PTR;
        /* Add entry to send queue */
        Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
        Sd_AppendToSendQueueTail(&cRtDataPtr->Head, sendQueueObjPtr);
    }

    SD_UNUSED(sdInstancePtr);

    return ret;
}
#endif

/**
 * @brief               Build OfferService entry and options,then add it to SendQueue
 * @param[in]           instancePtr Pointer to the SD instance configuration.
 * @param[in]           serverServicePtr Pointer to the server service configuration.
 * @param[in]           rxQueueItem Pointer to the received queue item.
 * @param[in]           sendTime Send time.
 * @param[in]           isUnicastSend Flag indicating if the send is unicast.
 * @param[in]           isStopOfferService Flag indicating if it is a stop offer service.
 * @param[in]           applicationId Application ID.
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace              CPD-74076
 */
#if (SD_SERVER_SERVICE_NUM > 0)
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL void Sd_BuildOfferServiceEntry(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint32                      sendTime,
    boolean                     isUnicastSend,
    boolean                     isStopOfferService,
    ApplicationType             applicationId)
/* PRQA S 6040 -- */
{
    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    Sd_SendQueueType* sendQueueObjPtr =
        (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */
    if (NULL_PTR != sendQueueObjPtr)
    {
        /* Clear new allocated memory */
        (void)IStdLib_MemSet(sendQueueObjPtr, 0u, sizeof(Sd_SendQueueType));
    }
    Sd_Type1EntryType* offerServerEntryPtr =
        Sd_CreateOfferServiceEntry(serverServicePtr, isStopOfferService, applicationId);
    uint8* optionsPtr = Sd_CreateOfferServiceOptions(instancePtr, serverServicePtr, sendQueueObjPtr, applicationId);
    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == offerServerEntryPtr) || (NULL_PTR == optionsPtr))
    {
        /* Any buffer allocate fail,free all allocated buffer */
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(offerServerEntryPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(optionsPtr, Sd_TxMemPool[applicationId]);
    }
    else
    {
        /* Add to send queue */
        if (isUnicastSend)
        {
            (void)IStdLib_MemCpy(&sendQueueObjPtr->DestAddr, &rxQueueItem->RemoteAddr, sizeof(rxQueueItem->RemoteAddr));
        }
        sendQueueObjPtr->EntryPtr      = (uint8*)offerServerEntryPtr;
        sendQueueObjPtr->OptionPtr     = optionsPtr;
        sendQueueObjPtr->IsUnicastSend = isUnicastSend;
        sendQueueObjPtr->SendTime      = sendTime;
        sendQueueObjPtr->Next          = NULL_PTR;
        Sd_AppendToSendQueueTail(&Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId]->Head, sendQueueObjPtr);
    }
}
#endif /* SD_SERVER_SERVICE_NUM > 0 */

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
/* PRQA S 6040,6080 ++ */ /* VL_MTR_Sd_STPAR,VL_MTR_Sd_STPTH */
SD_LOCAL void Sd_BuildSubscribeOption(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const Sd_ClientServiceRTType*    cRtDataPtr,
    uint8*                           optCnt,
    uint8*                           optionPtr,
    uint16                           optStartPos[],
    uint16                           bufOffset,
    uint16                           cfgOptLen,
    boolean                          isStopSubscribe)
/* PRQA S 6040,6080 -- */
{
    uint16 endpointOptionLen = 0u;
    uint8  tempOptCnt        = *optCnt;
    uint16 tempBufOffset     = bufOffset;

    if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
    {
        endpointOptionLen = SD_MESSAGE_IPV4_OPTION_LEN;
    }
    else
    {
        endpointOptionLen = SD_MESSAGE_IPV6_OPTION_LEN;
    }

#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef) && (cRtDataPtr->TcpSoCon.IsSoConOpened))
    {
        optStartPos[tempOptCnt] = tempBufOffset;
        tempOptCnt++;

        /*@req <SWS_SD_00701>,<SWS_SD_00702> */
        SoAd_SoConIdType soConId = cRtDataPtr->TcpSoCon.SoConId;
        Sd_SetEndpointOption(sdInstancePtr, SD_L4_PROTO_TCP, soConId, optionPtr, tempBufOffset);
        tempBufOffset += endpointOptionLen; /* PRQA S 2986 */ /* VL_QAC_2985_2986 */

        /*@req <SWS_SD_00702> */
        if (!isStopSubscribe)
        {
            (void)SoAd_EnableSpecificRouting(
                eventGroupPtr->SdConsumedEventGroupTcpActivationRef->RoutingGroupId,
                soConId);
        }
    }
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON || SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
    if (NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
    {
        optStartPos[tempOptCnt] = tempBufOffset;
        tempOptCnt++;

        SoAd_SoConIdType soConId = 0u;
#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
        if (cRtDataPtr->UdpSoCon.IsSoConOpened)
        {
            /*@req <SWS_SD_00701>,<SWS_SD_00703> */
            soConId = cRtDataPtr->UdpSoCon.SoConId;
            Sd_SetEndpointOption(sdInstancePtr, SD_L4_PROTO_UDP, soConId, optionPtr, tempBufOffset);
            tempBufOffset += endpointOptionLen; /* PRQA S 2986 */ /* VL_QAC_2985_2986 */
        }
#endif

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
        if (cRtDataPtr->MulitcastSoCon.IsSoConOpened)
        {
            /*@req <SWS_SD_00701>,<SWS_SD_00703> */
            soConId = cRtDataPtr->MulitcastSoCon.SoConId;
            Sd_SetMulicastEndpointOption(sdInstancePtr, soConId, optionPtr, tempBufOffset);
            tempBufOffset += endpointOptionLen; /* PRQA S 2986 */ /* VL_QAC_2985_2986 */
        }
#endif

        /*@req <SWS_SD_00703> */
        if (!isStopSubscribe)
        {
            (void)SoAd_EnableSpecificRouting(
                eventGroupPtr->SdConsumedEventGroupUdpActivationRef->RoutingGroupId,
                soConId);
        }
    }
#endif

    if (cfgOptLen > 0u)
    {
        optStartPos[tempOptCnt] = tempBufOffset;
        tempOptCnt++;

        Sd_BuildType2ConfigurationOption(
            sdInstancePtr,
            NULL_PTR,
            NULL_PTR,
            clientServicePtr,
            &optionPtr[tempBufOffset],
            cfgOptLen);
    }

    *optCnt += tempOptCnt;
}
#endif

/**
 * @brief              Build a SubscribeEventgroup entry and Options
 * @param[in]          sdInstancePtr: Pointer for sd instance.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          eventGroupPtr: Pointer for event group.
 * @param[in]          remoteAddr: Remote ip address.
 * @param[in]          cRtDataPtr: Pointer for client service runtime data.
 * @param[in]          isRxUseMulticast: Is received by multicast socket.
 * @param[in]          sendTime: Message transmission delay time.
 * @param[in]          applicationId: Application ID.
 * @return             E_OK: No Error
 *                     E_NOT_OK: Error Occur
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74077
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
/* PRQA S 6040,6070 ++ */ /* VL_MTR_Sd_STPAR,VL_MTR_Sd_STCAL */
SD_LOCAL Std_ReturnType Sd_BuildSubscribeEventgroupEntry(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    Sd_ClientServiceRTType*          cRtDataPtr,
    boolean                          isRxUseMulticast,
    uint32*                          sendTime,
    ApplicationType                  applicationId)
/* PRQA S 6040,6070 -- */
{
    Std_ReturnType ret        = E_OK;
    uint16         optionsLen = 0u;
/* Calculate required buffer length of options */
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef) && (cRtDataPtr->TcpSoCon.IsSoConOpened))
    {
        optionsLen = sdInstancePtr->SdOptionLength;
    }
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON || SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
        && (cRtDataPtr->UdpSoCon.IsSoConOpened || cRtDataPtr->MulitcastSoCon.IsSoConOpened))
    {
        optionsLen += sdInstancePtr->SdOptionLength; /* PRQA S 2986 */ /* VL_QAC_2985_2986 */
    }
#endif

    uint16 cfgOptLen =
        Sd_GetType2ConfigurationOptionLength(sdInstancePtr, NULL_PTR, NULL_PTR, clientServicePtr, eventGroupPtr);
    optionsLen += cfgOptLen;

    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    Sd_SendQueueType* sendQueueObjPtr =
        (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool[applicationId]);
    Sd_Type2EntryType* entryPtr =
        (Sd_Type2EntryType*)Sd_MallocEx(sizeof(Sd_Type2EntryType), Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */
    uint8* optionPtr = NULL_PTR;
    if (optionsLen > 0u)
    {
        optionPtr = (uint8*)Sd_MallocEx(optionsLen, Sd_TxMemPool[applicationId]); /* PRQA S 0316 */ /* VL_QAC_0316 */
    }
    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == entryPtr) || ((optionsLen > 0u) && (NULL_PTR == optionPtr)))
    {
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(entryPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(optionPtr, Sd_TxMemPool[applicationId]);
        ret = E_NOT_OK;
    }
    else
    {
        Sd_ConsumedEventGroupRTType* consumedEgRTPtr =
            Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];

        /* Clear new allocated memory */
        (void)IStdLib_MemSet(sendQueueObjPtr, 0u, sizeof(Sd_SendQueueType));
        (void)IStdLib_MemSet(entryPtr, 0u, sizeof(Sd_Type2EntryType));
        if (optionsLen > 0u)
        {
            (void)IStdLib_MemSet(optionPtr, 0u, optionsLen);
        }

        /* Send Queue object initialization */
        sendQueueObjPtr->OptionNum = 0u;

        entryPtr->Type            = SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP;
        entryPtr->Index1stOptions = 0u;
        entryPtr->Index2ndOptions = 0u;
        entryPtr->NumOfOpt1       = 0u;
        entryPtr->NumOfOpt2       = 0u;
        entryPtr->ServiceID       = Sd_HtoNs(clientServicePtr->SdClientServiceId);
        entryPtr->InstanceID      = Sd_HtoNs(clientServicePtr->SdClientServiceInstanceId);
        entryPtr->MajorVersion    = clientServicePtr->SdClientServiceMajorVersion;
        uint32 ttl                = Sd_HtoNl(clientServicePtr->SdClientServiceTimerRef->SdClientTimerTTL);
        /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
        (void)IStdLib_MemCpy(entryPtr->TTL, ((uint8*)&ttl) + 1, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
        /* PRQA S 0489 -- */
        entryPtr->Counter      = eventGroupPtr->SdConsumedEventGroupCounter;
        entryPtr->EventGroupID = Sd_HtoNs(eventGroupPtr->SdConsumedEventGroupId);

        uint16 bufOffset                   = 0u;
        uint16 optStartPos[SD_ARRAY_LEN_3] = {0};
        uint8  optCnt                      = 0u;

        Sd_BuildSubscribeOption(
            sdInstancePtr,
            clientServicePtr,
            eventGroupPtr,
            cRtDataPtr,
            &optCnt,
            optionPtr,
            optStartPos,
            bufOffset,
            cfgOptLen,
            FALSE);

        /* Save information to sendQueueObj */
        (void)IStdLib_MemCpy(&sendQueueObjPtr->DestAddr, &cRtDataPtr->OfferEntryRemoteAddr, sizeof(TcpIp_SockAddrType));
        sendQueueObjPtr->EntryPtr                                 = (uint8*)entryPtr;
        sendQueueObjPtr->OptionPtr                                = optionPtr;
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_0] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_0];
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_1] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_1];
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_2] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_2];
        sendQueueObjPtr->OptionLen                                = optionsLen;
        sendQueueObjPtr->OptionNum                                = optCnt;
        sendQueueObjPtr->IsUnicastSend                            = TRUE;
        sendQueueObjPtr->SendTime = Sd_CalcClientServiceDelayTime(eventGroupPtr, isRxUseMulticast);
        sendQueueObjPtr->Next     = NULL_PTR;
        /* Add entry to send queue */
        Sd_AppendToSendQueueTail(&cRtDataPtr->Head, sendQueueObjPtr);

        consumedEgRTPtr->IsSubscribed = FALSE;
        *sendTime                     = sendQueueObjPtr->SendTime;
    }

    return ret;
}
#endif

/**
 * @brief              Disables all consumed event routing for a given event group and client service.
 * @param[in]          eventGroupPtr: Pointer to the consumed event group type.
 * @param[in]          cRtDataPtr: Pointer to the client service runtime data.
 * @param[in]          cEgRTPtr: Pointer to the consumed event group runtime data.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74551
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL void Sd_DisableAllConsumedEventRouting(
    const Sd_ConsumedEventGroupType*   eventGroupPtr,
    const Sd_ClientServiceRTType*      cRtDataPtr,
    const Sd_ConsumedEventGroupRTType* cEgRTPtr)
{
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef) && (cRtDataPtr->TcpSoCon.IsSoConOpened))
    {
        SoAd_RoutingGroupIdType routingGroupId = eventGroupPtr->SdConsumedEventGroupTcpActivationRef->RoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->TcpSoCon.SoConId);
    }
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef) && (cRtDataPtr->UdpSoCon.IsSoConOpened))
    {
        SoAd_RoutingGroupIdType routingGroupId = eventGroupPtr->SdConsumedEventGroupUdpActivationRef->RoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->UdpSoCon.SoConId);
    }
#endif

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef) && (cRtDataPtr->MulitcastSoCon.IsSoConOpened))
    {
        SoAd_RoutingGroupIdType routingGroupId = eventGroupPtr->SdConsumedEventGroupUdpActivationRef->RoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->MulitcastSoCon.SoConId);
    }
#endif

#if (SD_CONSUMED_EVENT_MULICAST_USED == STD_ON)
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupMulticastActivationRef) && (cEgRTPtr->MultiSoCon.IsSoConOpened))
    {
        SoAd_RoutingGroupIdType routingGroupId =
            eventGroupPtr->SdConsumedEventGroupMulticastActivationRef->RoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cEgRTPtr->MultiSoCon.SoConId);
    }
#endif

    SD_UNUSED(cEgRTPtr);
}
#endif

/**
 * @brief              Build a StopSubscribeEventgroup Entry and options
 * @param[in]          sdInstancePtr: Pointer for sd instance.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          eventGroupPtr: Pointer for event group.
 * @param[in]          remoteAddr: Remote ip address.
 * @param[in]          cRtDataPtr: Pointer for client service runtime data.
 * @param[in]          isRxUseMulticast: Is received by multicast socket.
 * @param[in]          sendTime: Message transmission delay time.
 * @param[in]          applicationId: Application ID.
 * @return             E_OK: No Error
 *                     E_NOT_OK: Error Occur
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74078
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
/* PRQA S 6040,6070 ++ */ /* VL_MTR_Sd_STPAR,VL_MTR_Sd_STCAL */
SD_LOCAL Std_ReturnType Sd_BuildStopSubscribeEventgroupEntry(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const TcpIp_SockAddrType*        remoteAddr,
    Sd_ClientServiceRTType*          cRtDataPtr,
    boolean                          isRxUseMulticast,
    ApplicationType                  applicationId)
/* PRQA S 6040,6070 -- */
{
    Std_ReturnType ret        = E_OK;
    uint16         optionsLen = 0u;
/* Calculate required buffer length of options */
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef) && (cRtDataPtr->TcpSoCon.IsSoConOpened))
    {
        optionsLen = sdInstancePtr->SdOptionLength;
    }
#endif

#if ((SD_CLIENT_SERVICE_USE_UDP == STD_ON) || (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON))
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
        && (cRtDataPtr->UdpSoCon.IsSoConOpened || cRtDataPtr->MulitcastSoCon.IsSoConOpened))
    {
        optionsLen += sdInstancePtr->SdOptionLength; /* PRQA S 2986 */ /* VL_QAC_2985_2986 */
    }
#endif
    uint16 cfgOptLen =
        Sd_GetType2ConfigurationOptionLength(sdInstancePtr, NULL_PTR, NULL_PTR, clientServicePtr, eventGroupPtr);
    optionsLen += cfgOptLen;

    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    Sd_SendQueueType* sendQueueObjPtr =
        (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool[applicationId]);
    Sd_Type2EntryType* entryPtr =
        (Sd_Type2EntryType*)Sd_MallocEx(sizeof(Sd_Type2EntryType), Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */
    uint8* optionPtr = NULL_PTR;

    if (optionsLen > 0u)
    {
        optionPtr = (uint8*)Sd_MallocEx(optionsLen, Sd_TxMemPool[applicationId]); /* PRQA S 0316 */ /* VL_QAC_0316 */
    }
    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == entryPtr) || ((optionsLen > 0u) && (NULL_PTR == optionPtr)))
    {
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(entryPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(optionPtr, Sd_TxMemPool[applicationId]);
        ret = E_NOT_OK;
    }
    else
    {
        /* Clear new allocated memory */
        (void)IStdLib_MemSet(sendQueueObjPtr, 0u, sizeof(Sd_SendQueueType));
        (void)IStdLib_MemSet(entryPtr, 0u, sizeof(Sd_Type2EntryType));
        if (optionsLen > 0u)
        {
            (void)IStdLib_MemSet(optionPtr, 0u, optionsLen);
        }

        /* Send Queue object initialization */
        sendQueueObjPtr->OptionNum = 0u;

        entryPtr->Type            = SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP;
        entryPtr->Index1stOptions = 0u;
        entryPtr->Index2ndOptions = 0u;
        entryPtr->NumOfOpt1       = 0u;
        entryPtr->NumOfOpt2       = 0u;
        entryPtr->ServiceID       = Sd_HtoNs(clientServicePtr->SdClientServiceId);
        entryPtr->InstanceID      = Sd_HtoNs(clientServicePtr->SdClientServiceInstanceId);
        entryPtr->MajorVersion    = clientServicePtr->SdClientServiceMajorVersion;
        uint32 ttl                = 0u;
        /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
        (void)IStdLib_MemCpy(entryPtr->TTL, ((uint8*)&ttl) + 1, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
        /* PRQA S 0489 -- */
        entryPtr->Counter      = eventGroupPtr->SdConsumedEventGroupCounter;
        entryPtr->EventGroupID = Sd_HtoNs(eventGroupPtr->SdConsumedEventGroupId);

        uint8  optCnt                      = 0u;
        uint16 bufOffset                   = 0u;
        uint16 optStartPos[SD_ARRAY_LEN_3] = {0};

        Sd_BuildSubscribeOption(
            sdInstancePtr,
            clientServicePtr,
            eventGroupPtr,
            cRtDataPtr,
            &optCnt,
            optionPtr,
            optStartPos,
            bufOffset,
            cfgOptLen,
            TRUE);

        /*@req <SWS_SD_00711> */
        /*Deactive all the routing for all relevant Routing Groups of this Eventgroup */
        Sd_ConsumedEventGroupRTType* cEgRTPtr =
            Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
        Sd_DisableAllConsumedEventRouting(eventGroupPtr, cRtDataPtr, cEgRTPtr);

        (void)IStdLib_MemCpy(&sendQueueObjPtr->DestAddr, remoteAddr, sizeof(TcpIp_SockAddrType));
        sendQueueObjPtr->EntryPtr                                 = (uint8*)entryPtr;
        sendQueueObjPtr->OptionPtr                                = optionPtr;
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_0] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_0];
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_1] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_1];
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_2] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_2];
        sendQueueObjPtr->OptionLen                                = optionsLen;
        sendQueueObjPtr->OptionNum                                = optCnt;
        sendQueueObjPtr->IsUnicastSend                            = TRUE;
        sendQueueObjPtr->SendTime = Sd_CalcClientServiceDelayTime(eventGroupPtr, isRxUseMulticast);
        sendQueueObjPtr->Next     = NULL_PTR;
        Sd_AppendToSendQueueTail(&cRtDataPtr->Head, sendQueueObjPtr);

        cEgRTPtr->IsSubscribed        = FALSE;
        cEgRTPtr->IsTcpSubsSend       = FALSE;
        cEgRTPtr->IsUdpSubsSend       = FALSE;
        cEgRTPtr->IsMulitcastSubsSend = FALSE;
    }

    return ret;
}
#endif

/**
 * @brief              Calculate client service delay time when receive via Multicast message
 * @param[in]          eventHandlerPtr: Pointer for consumed event group.
 * @param[in]          isRxUseMulticast: Is received by multicast socket.
 * @return             Random delay time(times)
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74079
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL
uint32 Sd_CalcClientServiceDelayTime(const Sd_ConsumedEventGroupType* eventHandlerPtr, boolean isRxUseMulticast)
{
    uint32 tRandom = 0UL;

    if (isRxUseMulticast)
    {
        /*@req <SWS_SD_00491>,<SWS_SD_00492>,<SWS_SD_00493>,<SWS_SD_00494> */
        /* make the number between SdClientTimerRequestResponseMinDelay to SdClientTimerRequestResponseMaxDelay*/
        const Sd_ClientTimerType* timerPtr = eventHandlerPtr->SdConsumedEventGroupTimerRef;
        uint32                    delayMin = timerPtr->SdClientTimerRequestResponseMinDelay;
        uint32                    delayMax = timerPtr->SdClientTimerRequestResponseMaxDelay;
        /*@req <SWS_SD_00494>,<SWS_SD_00725> */
        /* Calculate random delay time */
        tRandom = Sd_GetRandomValue(delayMin, delayMax);

        /* ms to times */
        tRandom = tRandom / SD_MAIN_FUNCTION_CYCLE_TIME;
    }
    else
    {
        tRandom = SD_SEND_ENTRY_IMMEDIATELY;
    }

    return tRandom;
}
#endif

/**
 * @brief              Build a SubscribeEventgroupAck entry and options.
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          eventHandlerPtr: Pointer for consumed event group.
 * @param[in]          remoteAddr: Remote ip address.
 * @param[in]          ttl: The ttl field for subscribe entry.
 * @param[in]          counter: The counter field for subscribe entry.
 * @param[in]          applicationId: Application ID.
 * @return             E_OK: No Error
 *                     E_NOT_OK: Error Occur
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74080
 */
#if (SD_EVENT_HANDLER_NUM > 0)
/* PRQA S 6030,6040,6050,6070 ++ */ /* VL_MTR_Sd_STMIF,VL_MTR_Sd_STPAR,VL_MTR_Sd_STST3,VL_MTR_Sd_STCAL */
SD_LOCAL Std_ReturnType Sd_BuildSubscribeEventgroupAckEntry(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    const TcpIp_SockAddrType*   remoteAddr,
    uint32                      ttl,
    uint8                       counter,
    ApplicationType             applicationId)
/* PRQA S 6030,6040,6050,6070 -- */
{
    Std_ReturnType ret = E_OK;
    if (NULL_PTR != eventHandlerPtr)
    {
        uint16 multiOptlen = 0u;
#if (SD_EVENT_HANDLER_MULICAST_USED == STD_ON)
        if ((NULL_PTR != eventHandlerPtr->SdEventHandlerUdp) && (NULL_PTR != eventHandlerPtr->SdEventHandlerMulticast))
        {
            /* Need Build Multicast Option */
            multiOptlen = instancePtr->SdOptionLength;
        }
#endif
        uint16 cfgOptLen =
            Sd_GetType2ConfigurationOptionLength(instancePtr, serverServicePtr, eventHandlerPtr, NULL_PTR, NULL_PTR);
        uint16 optionsLen = multiOptlen + cfgOptLen; /* PRQA S 2986 */ /* VL_QAC_2985_2986 */

        /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
        Sd_SendQueueType* sendQueueObjPtr =
            (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool[applicationId]);
        Sd_Type2EntryType* egAckPtr =
            (Sd_Type2EntryType*)Sd_MallocEx(sizeof(Sd_Type1EntryType), Sd_TxMemPool[applicationId]);
        /* PRQA S 0316 -- */
        uint8* optionPtr = NULL_PTR;
        if (optionsLen > 0u)
        {
            /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
            optionPtr = (uint8*)Sd_MallocEx(optionsLen, Sd_TxMemPool[applicationId]);
            /* PRQA S 0316 -- */
        }

        if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == egAckPtr) || ((optionsLen > 0u) && (NULL_PTR == optionPtr)))
        {
            Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool[applicationId]);
            Sd_FreeEx(egAckPtr, Sd_TxMemPool[applicationId]);
            Sd_FreeEx(optionPtr, Sd_TxMemPool[applicationId]);
            ret = E_NOT_OK;
        }
        else
        {
            /* Clear new allocated memory */
            (void)IStdLib_MemSet(sendQueueObjPtr, 0u, sizeof(Sd_SendQueueType));
            (void)IStdLib_MemSet(egAckPtr, 0u, sizeof(Sd_Type1EntryType));
            if (optionsLen > 0u)
            {
                (void)IStdLib_MemSet(optionPtr, 0u, optionsLen);
            }

            /* Build SubscribeEventgroupAck Entry */
            egAckPtr->Type            = SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK;
            egAckPtr->Index1stOptions = 0u;
            egAckPtr->Index2ndOptions = 0u;
            egAckPtr->NumOfOpt1       = 0u;
            egAckPtr->NumOfOpt2       = 0u;
            egAckPtr->ServiceID       = Sd_HtoNs(serverServicePtr->SdServerServiceId);
            egAckPtr->InstanceID      = Sd_HtoNs(serverServicePtr->SdServerServiceInstanceId);
            egAckPtr->MajorVersion    = serverServicePtr->SdServerServiceMajorVersion;
            uint32 tTtl               = Sd_HtoNl(ttl);
            /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
            (void)IStdLib_MemCpy(egAckPtr->TTL, ((uint8*)&tTtl) + 1, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
            /* PRQA S 0489 -- */
            egAckPtr->Counter      = counter;
            egAckPtr->EventGroupID = Sd_HtoNs(eventHandlerPtr->SdEventHandlerEventGroupId);

            /* Build IPv4/IPv6 Multicast Option */
            uint16 optStartPos[SD_ARRAY_LEN_3] = {0};
            uint16 cfgOptOffset                = 0u;
            uint8  optNum                      = 0u;

#if (SD_EVENT_HANDLER_MULICAST_USED == STD_ON)
            if (multiOptlen > 0u)
            {
                optStartPos[optNum] = cfgOptOffset;
                optNum++;

                TcpIp_SockAddrType ipAddr;
                ipAddr.domain = instancePtr->SdAddrFamily;
                ret           = SoAd_GetRemoteAddr(
                    eventHandlerPtr->SdEventHandlerMulticast->SdMulticastEventSoConRef->SoConId,
                    &ipAddr);
                if (E_OK == ret)
                {
                    if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
                    {
                        Sd_SetIPv4MulticastOption(optionPtr, (uint8*)ipAddr.addr, ipAddr.port);
                        cfgOptOffset = SD_MESSAGE_IPV4_OPTION_LEN;
                    }
#if (STD_ON == SD_IPV6_ENABLE)
                    else
                    {
                        Sd_SetIPv6MulticastOption(optionPtr, (uint8*)ipAddr.addr, ipAddr.port);
                        cfgOptOffset = SD_MESSAGE_IPV6_OPTION_LEN;
                    }
#endif /* STD_ON == SD_IPV6_ENABLE */
                }
            }
#endif

            /* Build Configuration option */
            if (cfgOptLen > 0u)
            {
                optStartPos[optNum] = cfgOptOffset;
                optNum++;

                Sd_BuildType2ConfigurationOption(
                    instancePtr,
                    serverServicePtr,
                    eventHandlerPtr,
                    NULL_PTR,
                    &optionPtr[cfgOptOffset],
                    cfgOptLen);
            }

            /* Save information to sendQueueObj */
            (void)IStdLib_MemCpy(&sendQueueObjPtr->DestAddr, remoteAddr, sizeof(TcpIp_SockAddrType));
            sendQueueObjPtr->EntryPtr                                 = (uint8*)egAckPtr;
            sendQueueObjPtr->OptionPtr                                = optionPtr;
            sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_0] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_0];
            sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_1] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_1];
            sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_2] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_2];
            sendQueueObjPtr->OptionLen                                = optionsLen;
            sendQueueObjPtr->OptionNum                                = optNum;
            sendQueueObjPtr->IsUnicastSend                            = TRUE;
            sendQueueObjPtr->SendTime                                 = SD_SEND_ENTRY_IMMEDIATELY;
            sendQueueObjPtr->Next                                     = NULL_PTR;
            /* Add entry to send queue */
            Sd_ServerServiceRTType* sRtDataPtr = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
            Sd_AppendToSendQueueTail(&sRtDataPtr->Head, sendQueueObjPtr);
        }
    }

    return ret;
}
#endif

/**
 * @brief              Build a SubscribeEventgroupNAck entry and options
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          rxQueueItem: Pointer for rx queue item.
 * @param[in]          entryPos: Subscribe nack entry postition in sd message.
 * @param[in]          applicationId: Application ID.
 * @return             E_OK: No Error
 *                     E_NOT_OK: Error Occur
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74081
 */
#if (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL Std_ReturnType Sd_BuildSubscribeEventgroupNAckEntry(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint16                      entryPos,
    ApplicationType             applicationId)
{
    Std_ReturnType ret = E_OK;
    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    Sd_SendQueueType* sendQueueObjPtr =
        (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool[applicationId]);
    Sd_Type2EntryType* egAckPtr =
        (Sd_Type2EntryType*)Sd_MallocEx(sizeof(Sd_Type1EntryType), Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */
    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == egAckPtr))
    {
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(egAckPtr, Sd_TxMemPool[applicationId]);
        ret = E_NOT_OK;
    }
    else
    {
        /* Clear new allocated memory */
        (void)IStdLib_MemSet(sendQueueObjPtr, 0u, sizeof(Sd_SendQueueType));
        (void)IStdLib_MemSet(egAckPtr, 0u, sizeof(Sd_Type1EntryType));

        Sd_Type2EntryType type2Entry;
        (void)IStdLib_MemCpy((uint8*)&type2Entry, &rxQueueItem->BufPtr[entryPos], sizeof(Sd_Type2EntryType));
        /*@req <SWS_SD_00431>,<SWS_SD_00316>,<SWS_SD_00432> */
        /* Build SubscribeEventgroupAck Entry */
        egAckPtr->Type            = SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK;
        egAckPtr->Index1stOptions = 0u;
        egAckPtr->Index2ndOptions = 0u;
        egAckPtr->NumOfOpt1       = 0u;
        egAckPtr->NumOfOpt2       = 0u;
        egAckPtr->ServiceID       = Sd_HtoNs(Sd_NtoHs(type2Entry.ServiceID));
        egAckPtr->InstanceID      = Sd_HtoNs(Sd_NtoHs(type2Entry.InstanceID));
        egAckPtr->MajorVersion    = type2Entry.MajorVersion;
        uint32 ttl                = 0u;
        /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
        (void)IStdLib_MemCpy(egAckPtr->TTL, ((uint8*)&ttl) + 1, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
        /* PRQA S 0489 -- */
        egAckPtr->Counter      = type2Entry.Counter;
        egAckPtr->EventGroupID = Sd_HtoNs(Sd_NtoHs(type2Entry.EventGroupID));

        /* Save information to sendQueueObj */
        (void)IStdLib_MemCpy(&sendQueueObjPtr->DestAddr, &rxQueueItem->RemoteAddr, sizeof(TcpIp_SockAddrType));
        sendQueueObjPtr->EntryPtr                                 = (uint8*)egAckPtr;
        sendQueueObjPtr->OptionPtr                                = NULL_PTR;
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_0] = 0u;
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_1] = 0u;
        sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_2] = 0u;
        sendQueueObjPtr->OptionLen                                = 0u;
        sendQueueObjPtr->OptionNum                                = 0u;
        sendQueueObjPtr->IsUnicastSend                            = TRUE;
        sendQueueObjPtr->SendTime                                 = SD_SEND_ENTRY_IMMEDIATELY;
        sendQueueObjPtr->Next                                     = NULL_PTR;
        /* Add entry to send queue */
        Sd_ServerServiceRTType* sRtDataPtr = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
        Sd_AppendToSendQueueTail(&sRtDataPtr->Head, sendQueueObjPtr);
    }

    return ret;
}
#endif

/**
 * @brief              When isStopOfferService == FALSE,Build a OfferService entry.
 *                     When isStopOfferService == TRUE,Build a StopOfferService entry.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          isStopOfferService: Whether it is a StopOfferService
 * @param[in]          applicationId: Application ID.
 * @return             Pointer to OfferService's entry,NULL if buffer not enough
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74082
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL Sd_Type1EntryType* Sd_CreateOfferServiceEntry(
    const Sd_ServerServiceType* serverServicePtr,
    boolean                     isStopOfferService,
    ApplicationType             applicationId)
{
    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    Sd_Type1EntryType* ofSevsPtr =
        (Sd_Type1EntryType*)Sd_MallocEx(sizeof(Sd_Type1EntryType), Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */
    if (ofSevsPtr != NULL_PTR)
    {
        /* Clear new allocated memory */
        (void)IStdLib_MemSet(ofSevsPtr, 0u, sizeof(Sd_Type1EntryType));

        ofSevsPtr->Type            = SD_ENTRY_TYPE_OFFER_SERVICE;
        ofSevsPtr->Index1stOptions = 0u;
        ofSevsPtr->Index2ndOptions = 0u;
        ofSevsPtr->NumOfOpt1       = 0u;
        ofSevsPtr->NumOfOpt2       = 0u;
        ofSevsPtr->ServiceID       = Sd_HtoNs(serverServicePtr->SdServerServiceId);
        ofSevsPtr->InstanceID      = Sd_HtoNs(serverServicePtr->SdServerServiceInstanceId);
        ofSevsPtr->MajorVersion    = serverServicePtr->SdServerServiceMajorVersion;
        uint32 ttl                 = 0u;
        if (!isStopOfferService)
        {
            ttl = Sd_HtoNl(serverServicePtr->SdServerServiceTimerRef->SdServerTimerTTL);
        }
        /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
        (void)IStdLib_MemCpy(ofSevsPtr->TTL, ((uint8*)&ttl) + 1, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
        /* PRQA S 0489 -- */
        ofSevsPtr->MinorVersion = Sd_HtoNl(serverServicePtr->SdServerServiceMinorVersion);
    }

    return ofSevsPtr;
}
#endif

/**
 * @brief              Sets the endpoint option in the buffer.
 * @param[in]          sdInstancePtr: Pointer to the Service Discovery instance.
 * @param[in]          sdL4ProtoType: Layer 4 protocol type.
 * @param[in]          soConId: Socket connection ID.
 * @param[in,out]      bufPtr: Pointer to the buffer.
 * @param[in]          bufRunIdx: Current index in the buffer.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74559
 */
SD_LOCAL void Sd_SetEndpointOption(
    const Sd_InstanceType* sdInstancePtr,
    uint8                  sdL4ProtoType,
    SoAd_SoConIdType       soConId,
    uint8*                 bufPtr,
    uint16                 bufRunIdx)
{
    /*@req <SWS_SD_00478> */
    uint8              netmask;
    TcpIp_SockAddrType localAddr;
    TcpIp_SockAddrType defaultRouter;
    localAddr.domain     = sdInstancePtr->SdAddrFamily;
    defaultRouter.domain = sdInstancePtr->SdAddrFamily;
    (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);
    if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
    {
        uint8* ipv4OptPtr = &bufPtr[bufRunIdx];
        Sd_SetIPv4EndpointOption(ipv4OptPtr, (uint8*)localAddr.addr, sdL4ProtoType, localAddr.port);
    }
#if (STD_ON == SD_IPV6_ENABLE)
    else
    {
        uint8* ipv6OptPtr = &bufPtr[bufRunIdx];
        Sd_SetIPv6EndpointOption(ipv6OptPtr, (uint8*)localAddr.addr, sdL4ProtoType, localAddr.port);
    }
#endif /* STD_ON == SD_IPV6_ENABLE */
}

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
SD_LOCAL void Sd_SetMulicastEndpointOption(
    const Sd_InstanceType* sdInstancePtr,
    SoAd_SoConIdType       soConId,
    uint8*                 bufPtr,
    uint16                 bufRunIdx)
{
    uint8              netmask;
    TcpIp_SockAddrType localAddr;
    TcpIp_SockAddrType defaultRouter;
    localAddr.domain     = sdInstancePtr->SdAddrFamily;
    defaultRouter.domain = sdInstancePtr->SdAddrFamily;
    (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);
    if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
    {
        uint8* ipv4OptPtr = &bufPtr[bufRunIdx];
        Sd_SetIPv4MulticastOption(ipv4OptPtr, (uint8*)localAddr.addr, localAddr.port);
    }
}
#endif

/**
 * @brief              Build a OfferService Options
 * @param[in]          sdInstancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          sendQueueObjPtr: Pointer for send queue item.
 * @param[in]          applicationId: Application ID.
 * @return             Pointer to OfferService's Option,NULL if buffer not enough
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74083
 */
#if (SD_SERVER_SERVICE_NUM > 0)
/* PRQA S 2889,6060,6070,6080 ++ */ /* VL_QAC_MultiReturn,VL_MTR_Sd_STM19,VL_MTR_Sd_STCAL,VL_MTR_Sd_STPTH */
SD_LOCAL uint8* Sd_CreateOfferServiceOptions(
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    Sd_SendQueueType*           sendQueueObjPtr,
    ApplicationType             applicationId)
{
    if (NULL_PTR == sendQueueObjPtr)
    {
        /* Memory allocate for sendQueueObj fail */
        return NULL_PTR;
    }

    /* Calculate config option length */
    uint16 cfgOptlen = 0u;
#if (SD_SERVER_CAPABILITY_CFG_USED == STD_ON)
    cfgOptlen = Sd_GetType1ConfigurationOptionLength(sdInstancePtr, serverServicePtr, NULL_PTR);
#endif
    uint16 bufLenSum = cfgOptlen;

    uint16 tcpEndpointOptLen = 0u;
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
    if (NULL_PTR != serverServicePtr->SdServerServiceTcpRef)
    {
        if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
        {
            /* IPv4 Endpoint Option length is 12 */
            tcpEndpointOptLen = (uint16)SD_MESSAGE_IPV4_OPTION_LEN;
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else
        {
            /* IPv6 Endpoint Option length is 24 */
            tcpEndpointOptLen = SD_MESSAGE_IPV6_OPTION_LEN;
        }
#endif /* STD_ON == SD_IPV6_ENABLE */
    }
#endif

    uint16 udpEndpointOptLen = 0u;
#if (STD_ON == SD_SERVER_SERVICE_USE_UDP)
    if (NULL_PTR != serverServicePtr->SdServerServiceUdpRef)
    {
        if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
        {
            /* IPv4 */
            udpEndpointOptLen = (uint16)SD_MESSAGE_IPV4_OPTION_LEN;
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else
        {
            udpEndpointOptLen = SD_MESSAGE_IPV6_OPTION_LEN;
        }
#endif
    }
#endif

    /* Total buffer needed */
    bufLenSum += (tcpEndpointOptLen + udpEndpointOptLen); /* PRQA S 2986 */ /* VL_QAC_2985_2986 */

    /* According to specification at lease hava one endpoint,so the case that need't to
     * create optins is not exist */
    uint8* bufPtr    = (uint8*)Sd_MallocEx(bufLenSum, Sd_TxMemPool[applicationId]); /* PRQA S 0316 */ /* VL_QAC_0316 */
    uint8  optionNum = 0u;
    uint16 optStartPos[SD_ARRAY_LEN_3] = {0};
    if (NULL_PTR != bufPtr)
    {
        /* Set all memory to 0x00 */
        (void)IStdLib_MemSet(bufPtr, 0u, bufLenSum);

        uint16 bufRunIdx = 0u;

#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
        if (NULL_PTR != serverServicePtr->SdServerServiceTcpRef)
        {
            SoAd_SoConIdType soConId = serverServicePtr->SdServerServiceTcpRef->SocketIdList[0];
            Sd_SetEndpointOption(sdInstancePtr, SD_L4_PROTO_TCP, soConId, bufPtr, bufRunIdx);

            optStartPos[optionNum] = bufRunIdx;
            optionNum++;
            bufRunIdx += tcpEndpointOptLen; /* PRQA S 2986 */ /* VL_Sd_Redundant */
        }
#endif

#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
        if (NULL_PTR != serverServicePtr->SdServerServiceUdpRef)
        {
            SoAd_SoConIdType soConId = serverServicePtr->SdServerServiceUdpRef->SocketIdList[0];
            Sd_SetEndpointOption(sdInstancePtr, SD_L4_PROTO_UDP, soConId, bufPtr, bufRunIdx);

            optStartPos[optionNum] = bufRunIdx;
            optionNum++;
            bufRunIdx += udpEndpointOptLen; /* PRQA S 2986 */ /* VL_Sd_Redundant */
        }
#endif

/* If SdInstanceHostname or SdServerCapabilityRecord not equal NULL_PTR,
 * build Configuration Option */
#if (SD_SERVER_CAPABILITY_CFG_USED == STD_ON)
        if (cfgOptlen > 0u)
        {
            Sd_BuildType1ConfigurationOption(&bufPtr[bufRunIdx], cfgOptlen, sdInstancePtr, serverServicePtr, NULL_PTR);

            optStartPos[optionNum] = bufRunIdx;
            optionNum++;
        }
#endif

        SD_UNUSED(bufRunIdx);
    }

    /* Store info to the buffer [sendQueueObjPtr] referenced */
    sendQueueObjPtr->OptionLen                                = bufLenSum;
    sendQueueObjPtr->OptionNum                                = optionNum;
    sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_0] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_0];
    sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_1] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_1];
    sendQueueObjPtr->OptStartPos[SD_ENTRY_REF_OPTION_INDEX_2] = optStartPos[SD_ENTRY_REF_OPTION_INDEX_2];

    return bufPtr;
}
/* PRQA S 2889,6060,6070,6080 -- */
#endif

/**
 * @brief              Calculation of type1 Configuration Endpoint Option length
 *                     [serverServicePtr] or [clientServicePtr] must have one equal NULL_PTR
 *                     when [serverServicePtr] != NULL_PTR,Calculate config option length for OfferService or
 *                     StopOfferService when [clientServicePtr] != NULL_PTR,Calculate config option length for
 *                     FindService
 * @param[in]          sdInstancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @return             Type1 Configuration Endpoint Option length
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74084
 */
#if ((SD_SERVER_CAPABILITY_CFG_USED == STD_ON) || (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON))
SD_LOCAL uint16 Sd_GetType1ConfigurationOptionLength(
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    const Sd_CapabilityRecordType* recordPtr;
    uint16                         recordNum;

    if (NULL_PTR != serverServicePtr)
    {
        recordPtr = serverServicePtr->SdServerCapabilityRecord;
        recordNum = serverServicePtr->ServerCapabilityRecordNum;
    }
    else
    {
        recordPtr = clientServicePtr->SdClientCapabilityRecord;
        recordNum = clientServicePtr->ClientCapabilityRecordNum;
    }

    uint16 cfgOptlen = 0u;
    /* PRQA S 2812 ++ */ /* VL_Sd_PointerIsNull */
    if ((NULL_PTR != sdInstancePtr->SdInstanceHostname) || (NULL_PTR != recordPtr))
    {
        /* 5u = Length(2) + Type(1) + Reserved(1) + [0](1)*/
        cfgOptlen = SD_MESSAGE_TYPE1_OPTION_FIRST_FIVE_BYTE_LEN;
    }
    /* PRQA S 2812 -- */

    /* Hostname length */
    if ((NULL_PTR != sdInstancePtr) && (NULL_PTR != sdInstancePtr->SdInstanceHostname))
    {
        cfgOptlen += (uint16)Sd_Strlen(sdInstancePtr->SdInstanceHostname);
        /* SD_MESSAGE_TYPE1_OPTION_HOSTNAME_CFG_LEN = [len](1) + "Hostname"(8) + "="(1) */
        cfgOptlen = cfgOptlen + SD_MESSAGE_TYPE1_OPTION_HOSTNAME_CFG_LEN;
    }

    /* Record length */
    if (NULL_PTR != recordPtr)
    {
        for (uint16 index = 0u; index < recordNum; index++)
        {
            cfgOptlen += 1u; /* [len] */
            cfgOptlen += (uint16)Sd_Strlen(recordPtr->SdCapabilityRecordKey);
            cfgOptlen += 1u; /* '=' */
            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                cfgOptlen += (uint16)Sd_Strlen(recordPtr->SdCapabilityRecordValue);
            }

            recordPtr++;
        }
    }

    return cfgOptlen;
}
#endif

/**
 * @brief              Build the Configuration Endpoint Option for Type1 entry
 *                     [serverServicePtr] or [clientServicePtr] must have one equal NULL_PTR
 *                     If serverServicePtr is not NULL,Build config option for OfferService/StopOfferService
 *                     If clientServicePtr is not NULL,Build config option for FindService
 * @param[in]          cfgOptLen: ip address option byte length.
 * @param[in]          sdInstancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[inout]       optAddrPtr: Pointer for ip address option.
 * @return             Type1 Configuration Endpoint Option length
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74085
 */
#if ((SD_SERVER_CAPABILITY_CFG_USED == STD_ON) || (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON))
SD_LOCAL void Sd_BuildType1ConfigurationOption(
    uint8*                      optAddrPtr,
    uint16                      cfgOptLen,
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    /* !Note: Don't check buffer enough or not,make sure optAddr point to a
     * buffer enough to store Configuration Option */
    uint16                         recordNum;
    const Sd_CapabilityRecordType* recordPtr;

    if (NULL_PTR != serverServicePtr)
    {
        recordPtr = serverServicePtr->SdServerCapabilityRecord;
        recordNum = serverServicePtr->ServerCapabilityRecordNum;
    }
    else
    {
        recordPtr = clientServicePtr->SdClientCapabilityRecord;
        recordNum = clientServicePtr->ClientCapabilityRecordNum;
    }

    uint16 optLen = Sd_HtoNs(
        cfgOptLen - (uint16)SD_MESSAGE_TYPE1_OPTION_FIRST_THREE_BYTE_LEN); /* Exclude Length(2Byte) and Type(1Byte) */
    uint8* optAddr = optAddrPtr;
    (void)IStdLib_MemCpy(optAddr, (uint8*)&optLen, sizeof(uint16));
    *(uint8*)(&optAddr[SD_OPTION_TYPE_FILED_OFFSET]) = SD_CONFIGURATION_OPTION;

    optAddr = &optAddr[SD_TYPE1_OPTION_FIRST_KEY_OFFSET]; /* To the palce used to save first key-value pair */

    uint8 keyLen;
    uint8 valueLen;
    if (NULL_PTR != sdInstancePtr->SdInstanceHostname)
    {
        keyLen   = SD_STRING_HOSTNAME_LEN; /* Length of "hostname" */
        valueLen = (uint8)Sd_Strlen(sdInstancePtr->SdInstanceHostname);
        *optAddr = keyLen + valueLen + 1u; /* '=' length is 1 */
        optAddr++;
        const Sd_CharType* hostnameStr = (const Sd_CharType*)"hostname";
        (void)IStdLib_MemCpy(optAddr, (const uint8*)hostnameStr, keyLen);
        optAddr  = &optAddr[keyLen];
        *optAddr = (uint8)SD_MAGIC_CHAR_EQUAL;
        optAddr++;
        (void)IStdLib_MemCpy(optAddr, (const uint8*)sdInstancePtr->SdInstanceHostname, valueLen);
        optAddr = &optAddr[valueLen];
    }

    if (NULL_PTR != recordPtr)
    {
        for (uint16 index = 0u; index < recordNum; index++)
        {
            keyLen = (uint8)Sd_Strlen(recordPtr->SdCapabilityRecordKey);
            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                valueLen = (uint8)Sd_Strlen(recordPtr->SdCapabilityRecordValue);
            }
            else
            {
                valueLen = 0u;
            }
            *optAddr = keyLen + valueLen + 1u; /* '=' length is 1 */
            optAddr++;
            (void)IStdLib_MemCpy(optAddr, (const uint8*)recordPtr->SdCapabilityRecordKey, keyLen);
            optAddr  = &optAddr[keyLen];
            *optAddr = (uint8)SD_MAGIC_CHAR_EQUAL;
            optAddr++;

            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                (void)IStdLib_MemCpy(optAddr, (const uint8*)recordPtr->SdCapabilityRecordValue, valueLen);
                optAddr = &optAddr[valueLen];
            }

            recordPtr++;
        }
    }

    *optAddr = 0u; /* end with [0] */
}
#endif

/**
 * @brief              Calculation of type2 Configuration Endpoint Option length
 *                     [eventHandlerPtr] or [eventGroupPtr] must have one equal NULL_PTR
 *                     when [eventHandlerPtr] != NULL_PTR,Calculate config option length for SubscribeEventgroupAck/NAck
 *                     when [eventGroupPtr] != NULL_PTR,Calculate config option length for
 * @param[in]          sdInstancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          eventHandlerPtr: Pointer for event handle.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          eventGroupPtr: Pointer for consumed event group.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74086
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0) || (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL uint16 Sd_GetType2ConfigurationOptionLength(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ServerServiceType*      serverServicePtr,
    const Sd_EventHandlerType*       eventHandlerPtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr)
{
    const Sd_CapabilityRecordType* recordPtr = NULL_PTR;
    uint16                         recordNum = 0u;

    if ((NULL_PTR != eventHandlerPtr) && (NULL_PTR != serverServicePtr))
    {
        recordPtr = serverServicePtr->SdServerCapabilityRecord;
        recordNum = serverServicePtr->ServerCapabilityRecordNum;
    }
    else if ((NULL_PTR != eventGroupPtr) && (NULL_PTR != clientServicePtr))
    {
        recordPtr = clientServicePtr->SdClientCapabilityRecord;
        recordNum = clientServicePtr->ClientCapabilityRecordNum;
    }
    else
    {
        /* Nothing to do here */
    }

    uint16 cfgOptlen = 0u;
    if ((NULL_PTR != sdInstancePtr->SdInstanceHostname) || (NULL_PTR != recordPtr))
    {
        cfgOptlen = SD_MESSAGE_TYPE1_OPTION_FIRST_FIVE_BYTE_LEN; /* 5u = Length(2) + Type(1) + Reserved(1) + [0](1)*/
    }

    /* Build configuration option */
    if (NULL_PTR != sdInstancePtr->SdInstanceHostname)
    {
        cfgOptlen += (uint16)Sd_Strlen(sdInstancePtr->SdInstanceHostname);
        cfgOptlen = cfgOptlen + SD_MESSAGE_TYPE1_OPTION_HOSTNAME_CFG_LEN; /* 10u = [len](1) + "Hostname"(8) + "="(1) */
    }

    if (NULL_PTR != recordPtr)
    {
        for (uint16 index = 0u; index < recordNum; index++)
        {
            cfgOptlen += 1u; /* [len] */
            cfgOptlen += (uint16)Sd_Strlen(recordPtr->SdCapabilityRecordKey);
            cfgOptlen += 1u; /* '=' */
            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                cfgOptlen += (uint16)Sd_Strlen(recordPtr->SdCapabilityRecordValue);
            }

            recordPtr++;
        }
    }

    return cfgOptlen;
}
#endif

/**
 * @brief              Build the Configuration Endpoint Option for Type2 entry
 *                     [eventHandlerPtr] or [eventGroupPtr] must have one equal NULL_PTR
 *                     If serverServicePtr is not NULL,Build config option for SubscribeEventgroupAck/NAck
 *                     If clientServicePtr is not NULL,Build config option for
 * SubscribeEventgroup/StopSubscribeEventgroup
 * @param[in]          sdInstancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          eventHandlerPtr: Pointer for event handle.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          cfgOptLen: Length of option.
 * @param[inout]       optAddrPtr: Pointer for ip address option.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74087
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0) || (SD_EVENT_HANDLER_NUM > 0)
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL void Sd_BuildType2ConfigurationOption(
    const Sd_InstanceType*      sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    const Sd_ClientServiceType* clientServicePtr,
    uint8*                      optAddrPtr,
    uint16                      cfgOptLen)
/* PRQA S 6040 -- */
{
    /* !Note: Don't check buffer enough or not,make sure optAddr point to a
     * buffer enough to store Configuration Option */
    const Sd_CapabilityRecordType* recordPtr = NULL_PTR;
    uint16                         recordNum;

    if (NULL_PTR != eventHandlerPtr)
    {
        recordPtr = serverServicePtr->SdServerCapabilityRecord;
        recordNum = serverServicePtr->ServerCapabilityRecordNum;
    }
    else
    {
        recordPtr = clientServicePtr->SdClientCapabilityRecord;
        recordNum = clientServicePtr->ClientCapabilityRecordNum;
    }

    uint8* optAddr = optAddrPtr;
    uint16 optLen  = Sd_HtoNs(
        cfgOptLen - (uint16)SD_MESSAGE_TYPE1_OPTION_FIRST_THREE_BYTE_LEN); /* Exclude Length(2Byte) and Type(1Byte) */

    (void)IStdLib_MemCpy(optAddr, (uint8*)&optLen, sizeof(uint16));
    optAddr[SD_OPTION_TYPE_FILED_OFFSET]     = SD_CONFIGURATION_OPTION;
    optAddr[SD_OPTION_RESERVED_FILED_OFFSET] = 0x00u; /* Reserved field */

    optAddr = &optAddr[SD_IPV4_ADDRESS_LEN]; /* To the palce used to save first key-value pair */

    uint8 keyLen;
    uint8 valueLen;
    if (NULL_PTR != sdInstancePtr->SdInstanceHostname)
    {
        keyLen   = SD_STRING_HOSTNAME_LEN; /* Length of "hostname" */
        valueLen = (uint8)Sd_Strlen(sdInstancePtr->SdInstanceHostname);
        *optAddr = keyLen + valueLen + 1u; /* '=' length is 1 */
        optAddr++;

        const Sd_CharType* hostnameStr = (const Sd_CharType*)"hostname";
        (void)IStdLib_MemCpy(optAddr, (const uint8*)hostnameStr, keyLen);
        optAddr  = &optAddr[keyLen];
        *optAddr = (uint8)SD_MAGIC_CHAR_EQUAL;
        optAddr++;
        (void)IStdLib_MemCpy(optAddr, (const uint8*)sdInstancePtr->SdInstanceHostname, valueLen);
        optAddr = &optAddr[valueLen];
    }

    if (NULL_PTR != recordPtr)
    {
        for (uint16 index = 0u; index < recordNum; index++)
        {
            keyLen = (uint8)Sd_Strlen(recordPtr->SdCapabilityRecordKey);
            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                valueLen = (uint8)Sd_Strlen(recordPtr->SdCapabilityRecordValue);
            }
            else
            {
                valueLen = 0u;
            }
            *optAddr = keyLen + valueLen + 1u; /* '=' length is 1 */
            optAddr++;
            (void)IStdLib_MemCpy(optAddr, (const uint8*)recordPtr->SdCapabilityRecordKey, keyLen);
            optAddr  = &optAddr[keyLen];
            *optAddr = (uint8)SD_MAGIC_CHAR_EQUAL;
            optAddr++;

            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                (void)IStdLib_MemCpy(optAddr, (const uint8*)recordPtr->SdCapabilityRecordValue, valueLen);
                optAddr = &optAddr[valueLen];
            }

            recordPtr++;
        }
    }

    *optAddr = 0u; /* end with [0] */
}
#endif

/**
 * @brief              Set SOME/IP-SD Header
 * @param[in]          headerPtr: Pointer for SOME/IP-SD message header buffer.
 * @param[in]          instanceId: Instance id.
 * @param[in]          remoteAddr: Pointer for remote ip address.
 * @param[in]          isMulticastTx: Is send by multicast socket.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74088
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void
    Sd_SetHeader(uint8* headerPtr, uint16 instanceId, const TcpIp_SockAddrType* remoteAddr, boolean isMulticastTx)
{
    Sd_InstanceRTType* instanceRtPtr   = Sd_InstanceRTData[instanceId];
    uint16*            sessionIdCntPtr = NULL_PTR;
    Sd_HeaderType      header;

    (void)IStdLib_MemSet(&header, 0u, sizeof(Sd_HeaderType));

    header.ClientID = 0u;
    /* Set SessionId*/
    if (isMulticastTx)
    {
        sessionIdCntPtr = &instanceRtPtr->SessionIdHandle.TxMultiSesIdCnt;
    }
    else
    {
        Sd_SessionIdCtrlType* sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, remoteAddr);
        if (NULL_PTR != sesIdCtrlPtr)
        {
            sessionIdCntPtr = &sesIdCtrlPtr->TxUniSesIdCnt;
        }
    }
    if (NULL_PTR != sessionIdCntPtr)
    {
        (*sessionIdCntPtr)++;
        if (0u == *sessionIdCntPtr)
        {
            (*sessionIdCntPtr)++;
            /*@req <SWS_SD_00151> */
            instanceRtPtr->RebootFlag = FALSE;
        }
        header.SessionID = Sd_HtoNs(*sessionIdCntPtr);
    }
    else
    {
        header.SessionID = Sd_HtoNs(0x01u);
    }
    header.RebootFlag = instanceRtPtr->RebootFlag ? 1u : 0u;

    header.ProtocolVersion  = 0x01u;
    header.InterfaceVersion = 0x01u;
    header.MessageType      = SD_MESSAGE_TYPE_NOTIFY;
    header.ReturnCode       = 0x00u;
    /*@req <SWS_SD_00153> */
    header.UnicastFlag = 0x01u;

    (void)IStdLib_MemCpy(headerPtr, (uint8*)&header, sizeof(Sd_HeaderType));
}
#endif

/**
 * @brief              Set the IPv4 Endpoint Option.
 * @param[in]          ipv4OptionPtr: Pointer for ipv4 option.
 * @param[in]          ipv4Address: Pointer for
 * @param[in]          L4Proto: The L4-Proto field for IPv4 endpoint option.
 * @param[in]          portNumber: IPv4 protocol port.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74089
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void Sd_SetIPv4EndpointOption(uint8* ipv4OptionPtr, const uint8* ipv4Address, uint8 L4Proto, uint16 portNumber)
{
    Sd_IPv4OptionsType ipv4Option;

    (void)IStdLib_MemSet(&ipv4Option, 0u, sizeof(Sd_IPv4OptionsType));
    ipv4Option.Length = Sd_HtoNs(SD_IPV4_ENDPOINT_OPTION_LENGTH);
    ipv4Option.Type   = SD_IPV4_ENDPOINT_OPTION;
    (void)IStdLib_MemCpy(ipv4Option.IPv4Address, ipv4Address, SD_IPV4_ADDRESS_LEN);
    ipv4Option.L4Proto    = L4Proto;
    ipv4Option.PortNumber = Sd_HtoNs(portNumber);

    (void)IStdLib_MemCpy(ipv4OptionPtr, (uint8*)&ipv4Option, sizeof(Sd_IPv4OptionsType));
}
#endif

/**
 * @brief              Set the IPv6 Endpoint Option
 * @param[in]          ipv6OptionPtr: Pointer for ipv6 option.
 * @param[in]          ipv6Address: Pointer for ipv6 address buffer.
 * @param[in]          L4Proto: The L4-Proto field for IPv6 endpoint option.
 * @param[in]          portNumber: IPv6 protocol port.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74090
 */
#if (STD_ON == SD_IPV6_ENABLE && ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0)))
SD_LOCAL void Sd_SetIPv6EndpointOption(uint8* ipv6OptionPtr, const uint8* ipv6Address, uint8 L4Proto, uint16 portNumber)
{
    Sd_IPv6OptionsType ipv6Option;

    (void)IStdLib_MemSet(&ipv6Option, 0, sizeof(Sd_IPv6OptionsType));
    ipv6Option.Length = Sd_HtoNs(SD_IPV6_ENDPOINT_OPTION_LENGTH);
    ipv6Option.Type   = SD_IPV6_ENDPOINT_OPTION;
    (void)IStdLib_MemCpy(ipv6Option.IPv6Address, ipv6Address, 16u);
    ipv6Option.L4Proto    = L4Proto;
    ipv6Option.PortNumber = Sd_HtoNs(portNumber);

    (void)IStdLib_MemCpy(ipv6OptionPtr, (uint8*)&ipv6Option, sizeof(Sd_IPv6OptionsType));
}
#endif

/**
 * @brief              Set the IPv6 Multicast Option
 * @param[in]          ipv6OptionPtr: Pointer for ipv6 option.
 * @param[in]          ipv6Address: Pointer for ipv6 address buffer.
 * @param[in]          portNumber: IPv6 protocol port.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74092
 */
#if ((SD_EVENT_HANDLER_MULICAST_USED == STD_ON) && (STD_ON == SD_IPV6_ENABLE))
SD_LOCAL void Sd_SetIPv6MulticastOption(uint8* ipv6OptionPtr, const uint8* ipv6Address, uint16 portNumber)
{
    Sd_IPv6OptionsType ipv6Option;

    (void)IStdLib_MemSet(&ipv6Option, 0, sizeof(Sd_IPv6OptionsType));
    ipv6Option.Length = Sd_HtoNs(SD_IPV6_MULTICAST_OPTION_LENGTH);
    ipv6Option.Type   = SD_IPV6_MULTICAST_OPTION;
    (void)IStdLib_MemCpy(ipv6Option.IPv6Address, ipv6Address, 16u);
    ipv6Option.L4Proto    = SD_L4_PROTO_UDP;
    ipv6Option.PortNumber = Sd_HtoNs(portNumber);

    (void)IStdLib_MemCpy(ipv6OptionPtr, (uint8*)&ipv6Option, sizeof(Sd_IPv6OptionsType));
}
#endif

/**
 * @brief              Set the IPv4 Multicast Option
 * @param[in]          ipv4OptionPtr: Pointer for IPv4 option.
 * @param[in]          ipv4Address: Pointer for IPv4 address buffer.
 * @param[in]          portNumber: IPv6 protocol port.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74091
 */
#if ((SD_EVENT_HANDLER_MULICAST_USED == STD_ON) || (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON))
SD_LOCAL void Sd_SetIPv4MulticastOption(uint8* ipv4OptionPtr, const uint8* ipv4Address, uint16 portNumber)
{
    Sd_IPv4OptionsType ipv4Option;

    (void)IStdLib_MemSet(&ipv4Option, 0, sizeof(Sd_IPv4OptionsType));
    ipv4Option.Length = Sd_HtoNs(SD_IPV4_MULTICAST_OPTION_LENGTH);
    ipv4Option.Type   = SD_IPV4_MULTICAST_OPTION;
    (void)IStdLib_MemCpy(ipv4Option.IPv4Address, ipv4Address, SD_IPV4_ADDRESS_LEN);
    ipv4Option.L4Proto    = SD_L4_PROTO_UDP;
    ipv4Option.PortNumber = Sd_HtoNs(portNumber);

    (void)IStdLib_MemCpy(ipv4OptionPtr, (uint8*)&ipv4Option, sizeof(Sd_IPv4OptionsType));
}
#endif

/**
 * @brief              Set TTL to the buffer [ttlPtr] referenced
 * @param[in]          ttl: TTL Timer Count
 * @param[inout]       ttlPtr: Pointer for ttl field
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74093
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL_INLINE void Sd_SetTTL(Sd_TTLType* ttlPtr, uint32 ttl)
{
    if (ttl == SD_MESSAGE_ENTRY_TTL_FOREVER)
    {
        ttlPtr->IsValidForever = TRUE;
        ttlPtr->ttlms          = 0u;
    }
    else
    {
        /* PRQA S 1252 ++ */ /* VL_Sd_Converted */
        ttlPtr->ttlms = (uint64)(((uint64)ttl * SD_SEC_TO_MS_CONVERSION_FRACTION) / SD_MAIN_FUNCTION_CYCLE_TIME);
        /* PRQA S 1252 -- */
        ttlPtr->IsValidForever = FALSE;
    }
}
#endif

/**
 * @brief              Process server service state transition in Down Phase
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74094
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_ServerServiceDownPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId)
{
    /* Phase manage */
    /*@req <SWS_SD_00317> */
    Sd_ServerServiceRTType* sRtDataPtr  = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    TcpIp_IpAddrStateType   ipAddrState = Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState;
    if ((SD_INIT_SUCCESS == *Sd_InitResult[applicationId]) && (SD_SERVER_SERVICE_AVAILABLE == sRtDataPtr->SetState)
        && (TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState))
    {
        /*@req <SWS_SD_00317> */
        sRtDataPtr->Phase = SD_SERVER_PHASE_INITIAL_WAIT;

        /*@req <SWS_SD_00330> */
        if (NULL_PTR != serverServicePtr->SdProvidedMethods)
        {
            (void)SoAd_EnableRouting(serverServicePtr->SdProvidedMethods->SdServerServiceActivationRef->RoutingGroupId);
        }

        /* make the number between SdServerTimerInitialOfferDelayMin to SdServerTimerInitialOfferDelayMax*/
        const Sd_ServerTimerType* timerPtr = serverServicePtr->SdServerServiceTimerRef;
        uint32                    delayMin = timerPtr->SdServerTimerInitialOfferDelayMin;
        uint32                    delayMax = timerPtr->SdServerTimerInitialOfferDelayMax;
        /* Calculate random delay time */
        uint32 tRandom = Sd_GetRandomValue(delayMin, delayMax);
        /*@req <SWS_SD_00318> */
        /* When entering the Initial Wait Phase, a random timer shall be started */
        sRtDataPtr->InitialWaitTimer = Sd_InitialWaitTimeAdjust(tRandom / SD_MAIN_FUNCTION_CYCLE_TIME);

        /*@req <SWS_SD_00606> */
        /* When the Initial Wait Phase is entered, the API SoAd_OpenSoCon() shall be called
         * for all Socket Connections associated with this Server Service Instance. */
        const Sd_SoAdSoConGroupType* soAdSoConGroupPtr = NULL_PTR;
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
        soAdSoConGroupPtr = serverServicePtr->SdServerServiceTcpRef;
        if (NULL_PTR != soAdSoConGroupPtr)
        {
            uint16                  soConIdNum  = soAdSoConGroupPtr->SoConNumInGroup;
            const SoAd_SoConIdType* soConIdList = soAdSoConGroupPtr->SocketIdList;
            for (uint16 index = 0u; index < soConIdNum; index++)
            {
                (void)SoAd_OpenSoCon(soConIdList[index]);
            }
        }
#endif

#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
        soAdSoConGroupPtr = serverServicePtr->SdServerServiceUdpRef;
        if (NULL_PTR != soAdSoConGroupPtr)
        {
            uint16                  soConIdNum  = soAdSoConGroupPtr->SoConNumInGroup;
            const SoAd_SoConIdType* soConIdList = soAdSoConGroupPtr->SocketIdList;
            for (uint16 index = 0u; index < soConIdNum; index++)
            {
                (void)SoAd_OpenSoCon(soConIdList[index]);
            }
        }
#endif

#if (SD_EVENT_HANDLER_MULICAST_USED == STD_ON)
        const Sd_EventHandlerType* eventHandlerPtr = serverServicePtr->SdEventHandler;
        for (uint16 eventGroupIdx = 0u; eventGroupIdx < serverServicePtr->EventHandlerNum; eventGroupIdx++)
        {
            const Sd_EventHandlerMulticastType* eventHandlerMulticastPtr = eventHandlerPtr->SdEventHandlerMulticast;
            if (NULL_PTR != eventHandlerMulticastPtr)
            {
                (void)SoAd_OpenSoCon(eventHandlerMulticastPtr->SdMulticastEventSoConRef->SoConId);
            }
            eventHandlerPtr++;
        }
#endif
    }
}
#endif

/**
 * @brief              Checks if at least one socket connection is valid for the server service.
 * @param[in]          serverServicePtr: Pointer to the server service type.
 * @return             TRUE if at least one socket connection is valid, FALSE otherwise.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74561
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL boolean Sd_IsLeastOneSoConValid(const Sd_ServerServiceType* serverServicePtr)
{
    boolean leastOneSoConValid = FALSE;
    /*@req <SWS_Sd_00321> */
    /* The Repetition phase is entered only when at least one socket's state
     * is SOAD_SOCON_ONLINE or SOAD_SOCON_RECONNECT. */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
    if (serverServicePtr->SdServerServiceUdpRef != NULL_PTR)
    {
        uint16                  soConNum  = serverServicePtr->SdServerServiceUdpRef->SoConNumInGroup;
        const SoAd_SoConIdType* soConList = serverServicePtr->SdServerServiceUdpRef->SocketIdList;
        for (uint16 sockectIndex = 0u; sockectIndex < soConNum; sockectIndex++)
        {
            SoAd_SoConModeType soConMode = SOAD_SOCON_OFFLINE;
            SoAd_GetSoConMode(soConList[sockectIndex], &soConMode);
            if ((SOAD_SOCON_ONLINE == soConMode) || (SOAD_SOCON_RECONNECT == soConMode))
            {
                leastOneSoConValid = TRUE;
                break;
            }
        }
    }
#endif

#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
    if (!leastOneSoConValid && (serverServicePtr->SdServerServiceTcpRef != NULL_PTR))
    {
        uint16                  soConNum  = serverServicePtr->SdServerServiceTcpRef->SoConNumInGroup;
        const SoAd_SoConIdType* soConList = serverServicePtr->SdServerServiceTcpRef->SocketIdList;
        for (uint16 sockectIndex = 0u; sockectIndex < soConNum; sockectIndex++)
        {
            SoAd_SoConModeType soConMode = SOAD_SOCON_OFFLINE;
            SoAd_GetSoConMode(soConList[sockectIndex], &soConMode);
            if ((SOAD_SOCON_ONLINE == soConMode) || (SOAD_SOCON_RECONNECT == soConMode))
            {
                leastOneSoConValid = TRUE;
                break;
            }
        }
    }
#endif

    return leastOneSoConValid;
}
#endif

/**
 * @brief              Process server service state transition in Initial With Phase
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74095
 */
#if (SD_SERVER_SERVICE_NUM > 0)
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
SD_LOCAL void Sd_ServerServiceInitialWaitPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId)
{
    Sd_ServerServiceRTType* sRtDataPtr = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    /*@req <SWS_SD_00325>,<SWS_SD_00323> */
    if ((TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState)
        || (SD_SERVER_SERVICE_AVAILABLE != sRtDataPtr->SetState))
    {
        Sd_ServerServiceDownPhaseEnter(instancePtr, serverServicePtr, applicationId);
    }
    else
    {
        boolean timerExpire = FALSE;
        /* Initial Wait Phase Delay timer expire */
        if (sRtDataPtr->InitialWaitTimer > 0UL)
        {
            --sRtDataPtr->InitialWaitTimer;
            if (sRtDataPtr->InitialWaitTimer == 0UL)
            {
                timerExpire = TRUE;
            }
        }
        else
        {
            timerExpire = TRUE;
        }

        if (timerExpire)
        {
            if (!Sd_IsLeastOneSoConValid(serverServicePtr))
            {
                return;
            }

            /*@req <SWS_SD_00321> */
            /* When the calculated random timer based on the min and max values
             * SdServerTimerInitialOfferDelayMin and SdServerTimerInitialOfferDelayMax
             * expires, the first OfferService entry shall be sent out.*/
            Sd_BuildOfferServiceEntry(instancePtr, serverServicePtr, NULL_PTR, 0UL, FALSE, FALSE, applicationId);

            const Sd_ServerTimerType* timerPtr = serverServicePtr->SdServerServiceTimerRef;
            if (timerPtr->SdServerTimerInitialOfferRepetitionsMax > 0u)
            {
                /*@req <SWS_SD_00434> */
                /* When the calculated random timer expires and the parameter
                 * SdServerTimerInitialOfferRepetitionsMax does not equals 0,
                 * the Repetition Phase shall be entered.*/
                sRtDataPtr->Phase = SD_SERVER_PHASE_REPETITION;

                /*@req <SWS_SD_00329> */
                /* If the Repetition Phase is entered, the Service Discovery shall
                 * wait SdServerTimerInitialOfferRepetitionBaseDelay and send an
                 * OfferService Entry.*/
                sRtDataPtr->RepetitionTimer =
                    timerPtr->SdServerTimerInitialOfferRepetitionBaseDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                Sd_BuildOfferServiceEntry(
                    instancePtr,
                    serverServicePtr,
                    NULL_PTR,
                    sRtDataPtr->RepetitionTimer,
                    FALSE,
                    FALSE,
                    applicationId);
                sRtDataPtr->RepetitionCount = 0u;
            }
            else
            {
                /*@req <SWS_SD_00435> */
                /* When the calculated random timer expires and the parameter
                 * SdServerTimerInitialOfferRepetitionsMax equal 0, the
                 * Main Phase shall be entered.*/
                sRtDataPtr->Phase = SD_SERVER_PHASE_MAIN;

                /*@req <SWS_SD_00449> */
                /* If SdServerTimerOfferCyclicDelay is greater than 0, in the Main Phase
                 * an OfferService entry shall be sent cyclically with an interval defined
                 * by configuration item SdServerTimerOfferCyclicDelay.*/
                if (timerPtr->SdServerTimerOfferCyclicDelay > 0UL)
                {
                    sRtDataPtr->MainTimer = timerPtr->SdServerTimerOfferCyclicDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                    Sd_BuildOfferServiceEntry(
                        instancePtr,
                        serverServicePtr,
                        NULL_PTR,
                        sRtDataPtr->MainTimer,
                        FALSE,
                        FALSE,
                        applicationId);
                }
            }
        }
    }
}
/* PRQA S 2889 -- */
#endif

/**
 * @brief              Process server service state transition in Repetition Phase
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74096
 */
#if (SD_SERVER_SERVICE_NUM > 0)
/* PRQA S 6030 ++ */ /* VL_MTR_Sd_STMIF */
SD_LOCAL void Sd_ServerServiceRepetitionPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId)
/* PRQA S 6030 -- */
{
    Sd_ServerServiceRTType* sRtDataPtr = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    /*@req <SWS_SD_00340>,<SWS_SD_00338> */
    if ((TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState)
        || (SD_SERVER_SERVICE_AVAILABLE != sRtDataPtr->SetState))
    {
        Sd_ServerServiceDownPhaseEnter(instancePtr, serverServicePtr, applicationId);
    }
    else
    {
        if (sRtDataPtr->RepetitionTimer > 0UL)
        {
            sRtDataPtr->RepetitionTimer--;
            if (0UL == sRtDataPtr->RepetitionTimer)
            {
                sRtDataPtr->RepetitionCount++;

                const Sd_ServerTimerType* timerPtr = serverServicePtr->SdServerServiceTimerRef;
                if (sRtDataPtr->RepetitionCount == timerPtr->SdServerTimerInitialOfferRepetitionsMax)
                {
                    /*@req <SWS_SD_00336> */
                    /* After the amount of cyclically sent OfferServices within the
                     * Repetition Phase equals the amount of SdServerTimerInitialOfferRepetitionsMax,
                     * the Main Phase shall be entered.*/
                    sRtDataPtr->Phase = SD_SERVER_PHASE_MAIN;

                    /*@req <SWS_SD_00449> */
                    /* If SdServerTimerOfferCyclicDelay is greater than 0, in the Main Phase
                     * an OfferService entry shall be sent cyclically with an interval defined
                     * by configuration item SdServerTimerOfferCyclicDelay.*/
                    if (timerPtr->SdServerTimerOfferCyclicDelay > 0UL)
                    {
                        sRtDataPtr->MainTimer = timerPtr->SdServerTimerOfferCyclicDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                        Sd_BuildOfferServiceEntry(
                            instancePtr,
                            serverServicePtr,
                            NULL_PTR,
                            sRtDataPtr->MainTimer,
                            FALSE,
                            FALSE,
                            applicationId);
                    }
                }
                else
                {
                    /*@req <SWS_SD_00331> */
                    /* In the Repetition Phase up to SdServerTimerInitialOfferRepetitionsMax
                     * OfferService Entries shall be sent with doubling intervals*/
                    uint32 tResult              = 0x01UL << (uint32)sRtDataPtr->RepetitionCount;
                    tResult                     = timerPtr->SdServerTimerInitialOfferRepetitionBaseDelay * tResult;
                    sRtDataPtr->RepetitionTimer = tResult / SD_MAIN_FUNCTION_CYCLE_TIME;
                    Sd_BuildOfferServiceEntry(
                        instancePtr,
                        serverServicePtr,
                        NULL_PTR,
                        sRtDataPtr->RepetitionTimer,
                        FALSE,
                        FALSE,
                        applicationId);
                }
            }
        }
    }
}
#endif

/**
 * @brief              Process server service state transition in Main Phase.
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74097
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_ServerServiceMainPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId)
{
    Sd_ServerServiceRTType* sRtDataPtr  = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    TcpIp_IpAddrStateType   ipAddrState = Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState;
    /*@req <SWS_SD_00342> */
    if ((SD_SERVER_SERVICE_AVAILABLE == sRtDataPtr->SetState) && (TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState))
    {
        if (sRtDataPtr->MainTimer > 0UL)
        {
            sRtDataPtr->MainTimer--;
            if (0UL == sRtDataPtr->MainTimer)
            {
                /*@req <SWS_SD_00449> */
                const Sd_ServerTimerType* timerPtr = serverServicePtr->SdServerServiceTimerRef;
                sRtDataPtr->MainTimer = timerPtr->SdServerTimerOfferCyclicDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                Sd_BuildOfferServiceEntry(
                    instancePtr,
                    serverServicePtr,
                    NULL_PTR,
                    sRtDataPtr->MainTimer,
                    FALSE,
                    FALSE,
                    applicationId);
            }
        }
    }
    else
    {
        /*@req <SWS_SD_00347>,<SWS_SD_00348> */
        Sd_ServerServiceDownPhaseEnter(instancePtr, serverServicePtr, applicationId);
    }
}
#endif

/**
 * @brief              Closes all socket connections for the server service.
 * @param[in]          serverServicePtr: Pointer to the server service type.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74562
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_CloseAllSoCon(const Sd_ServerServiceType* serverServicePtr)
{
    const Sd_SoAdSoConGroupType* soadSoConGroupPtr = NULL_PTR;
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
    soadSoConGroupPtr = serverServicePtr->SdServerServiceTcpRef;
    if (NULL_PTR != soadSoConGroupPtr)
    {
        uint16                  soConIdNum  = soadSoConGroupPtr->SoConNumInGroup;
        const SoAd_SoConIdType* soConIdList = soadSoConGroupPtr->SocketIdList;
        for (uint16 index = 0u; index < soConIdNum; index++)
        {
            (void)SoAd_CloseSoCon(soConIdList[index], TRUE);
        }
    }
#endif

#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
    soadSoConGroupPtr = serverServicePtr->SdServerServiceUdpRef;
    if (NULL_PTR != soadSoConGroupPtr)
    {
        uint16                  soConIdNum  = soadSoConGroupPtr->SoConNumInGroup;
        const SoAd_SoConIdType* soConIdList = soadSoConGroupPtr->SocketIdList;
        for (uint16 index = 0u; index < soConIdNum; index++)
        {
            (void)SoAd_CloseSoCon(soConIdList[index], TRUE);
        }
    }
#endif
}
#endif

/**
 * @brief              Process server service Down Phase enter(other than init)
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74098
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_ServerServiceDownPhaseEnter(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId)
{
    uint16                     serverHandleId  = serverServicePtr->SdServerServiceHandleId;
    Sd_ServerServiceRTType*    sRtDataPtr      = Sd_ServerRTData[serverHandleId];
    TcpIp_IpAddrStateType      ipAddrState     = Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState;
    const Sd_EventHandlerType* eventHandlerPtr = NULL_PTR;

    /*@req <SWS_SD_00323>,<SWS_SD_00338>,<SWS_SD_00348> */
    /* If the API Server Sd_ServerServiceSetState() is called with state SD_SERVER_SERVICE_DOWN
     * while the IP address is still assigned*/
    if (SD_SERVER_SERVICE_DOWN == sRtDataPtr->SetState)
    {
        if ((TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState)
            && (SD_SERVER_PHASE_INITIAL_WAIT != sRtDataPtr->Phase)) /* after first OfferService send,in Repetion Phase*/
        {
            /* Clear send queue of this service,avoid send an OfferService after send a StopOfferService */
            Sd_ClearSendQueue(&sRtDataPtr->Head, applicationId);
            /* Send a StopOfferService */
            Sd_BuildOfferServiceEntry(instancePtr, serverServicePtr, NULL_PTR, 0UL, FALSE, TRUE, applicationId);
        }

#if (SD_EVENT_HANDLER_NUM > 0)
        /* all subscriptions of the eventgroup(s) of this service instance shall be
         * deleted and SD_EVENT_HANDLER_RELEASED and reported to BswM using the API
         * BswM_Sd_EventHandlerCurrentState */
        eventHandlerPtr = serverServicePtr->SdEventHandler;
        for (uint16 handlerIdx = 0u; handlerIdx < serverServicePtr->EventHandlerNum; handlerIdx++)
        {
            uint16                 eventHandlerId = eventHandlerPtr->SdEventHandlerHandleId;
            Sd_EventHandlerRTType* ehRTDataPtr    = Sd_EventHandlerRTData[eventHandlerId];

            /* Delete all client from subscribe list */
            Sd_RemoveAllClientFromSubscribeList(&ehRTDataPtr->TcpSubscribeList, applicationId);
            Sd_RemoveAllClientFromSubscribeList(&ehRTDataPtr->UdpSubscribeList, applicationId);
            ehRTDataPtr->TcpSubsClientNum   = 0u;
            ehRTDataPtr->UdpSubsEndPointNum = 0u;

            eventHandlerPtr++;
        }
#endif /* SD_EVENT_HANDLER_NUM > 0 */
    }

/*@req <SWS_SD_00323>,<SWS_SD_00325>,<SWS_SD_00338>,<SWS_SD_00340>,<SWS_SD_00347>,<SWS_SD_00348> */
#if (SD_EVENT_HANDLER_NUM > 0)
    /* all associated EventHandler which state is not SD_EVENT_HANDLER_RELEASED
     * shall be changed to SD_EVENT_HANDLER_RELEASED and indicated to the BswM by
     * calling the API BswM_Sd_EventHandlerCurrentState().*/
    eventHandlerPtr = serverServicePtr->SdEventHandler;
    for (uint16 handlerIdx = 0u; handlerIdx < serverServicePtr->EventHandlerNum; handlerIdx++)
    {
        uint16 eventHandlerId = eventHandlerPtr->SdEventHandlerHandleId;
        if (Sd_EventHandlerRTData[eventHandlerId]->CurState != SD_EVENT_HANDLER_RELEASED)
        {
#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_EventHandlerCurrentState(eventHandlerId, SD_EVENT_HANDLER_RELEASED);
#endif /* SD_BSWM_USED == STD_ON */
            Sd_EventHandlerRTData[eventHandlerId]->CurState = SD_EVENT_HANDLER_RELEASED;
        }

#if (SD_EVENT_HANDLER_MULICAST_USED == STD_ON)
        /* Close Multicaset socket connection */
        const Sd_EventHandlerMulticastType* eventHandlerMulticastPtr = eventHandlerPtr->SdEventHandlerMulticast;
        if (NULL_PTR != eventHandlerMulticastPtr)
        {
            (void)SoAd_CloseSoCon(eventHandlerMulticastPtr->SdMulticastEventSoConRef->SoConId, TRUE);
        }
#endif

        eventHandlerPtr++;
    }
#endif /* SD_EVENT_HANDLER_NUM > 0 */

    /*@req <SWS_SD_00341>,<SWS_SD_00349> */
    /* When enter Down Phase from other phase, the routing of this Server Service Instance
     * shall be disabled.*/
    const Sd_ProvidedMethodsType* provideMethodPtr = serverServicePtr->SdProvidedMethods;
    if (NULL_PTR != provideMethodPtr)
    {
        (void)SoAd_DisableRouting(provideMethodPtr->SdServerServiceActivationRef->RoutingGroupId);
    }

    /* Down Phase entered */
    Sd_ServerRTData[serverHandleId]->Phase = SD_SERVER_PHASE_DOWN;

    /*@req <SWS_SD_00605> */
    /* When the Down Phase is entered (coming from states other than init), the
     * API SoAd_CloseSoCon() shall be called for all Socket Connections associated
     * with this Server Service Instance.*/
    Sd_CloseAllSoCon(serverServicePtr);

    /* Clear all timer */
    Sd_ServerRTData[serverHandleId]->InitialWaitTimer = 0u;
    Sd_ServerRTData[serverHandleId]->RepetitionTimer  = 0u;
    Sd_ServerRTData[serverHandleId]->RepetitionCount  = 0u;
    Sd_ServerRTData[serverHandleId]->MainTimer        = 0u;
}
#endif

/**
 * @brief              Manage server service timer
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74099
 */
#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL void Sd_ServerServiceTimerHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    ApplicationType             applicationId)
{
#if (SD_EVENT_HANDLER_NUM > 0)
    /* Handle subscribed client's TTL, if expired,delete the client from subscribe list */
    Sd_SubscribeClientTTLManage(serverServicePtr, applicationId);
#endif

    SD_UNUSED(instancePtr);
}
#endif

#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
SD_LOCAL void Sd_ClientServiceDownPhaseEnableTcp(
    const Sd_ClientServiceType*   clientServicePtr,
    Sd_ClientServiceRTType*       cRtDataPtr,
    const Sd_ConsumedMethodsType* consumedMethodsPtr)
{
    if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && cRtDataPtr->TcpSoCon.IsOptRcved
        && !cRtDataPtr->TcpSoCon.IsSoConOpened)
    {
        Std_ReturnType ret = SoAd_OpenSoCon(cRtDataPtr->TcpSoCon.SoConId);
        if (E_OK == ret)
        {
            cRtDataPtr->TcpSoCon.IsSoConOpened = TRUE;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
            Sd_ClientServiceResetAllCEgSubsEntrySendFlag(clientServicePtr, TRUE, FALSE);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

            if (NULL_PTR != consumedMethodsPtr)
            {
                SoAd_RoutingGroupIdType routingGroupId =
                    consumedMethodsPtr->SdClientServiceActivationRef->RoutingGroupId;
                (void)SoAd_EnableSpecificRouting(routingGroupId, cRtDataPtr->TcpSoCon.SoConId);
            }
        }
    }
}
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
SD_LOCAL void Sd_ClientServiceDownPhaseEnableUdp(
    const Sd_ClientServiceType*   clientServicePtr,
    Sd_ClientServiceRTType*       cRtDataPtr,
    const Sd_ConsumedMethodsType* consumedMethodsPtr)
{
    if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && cRtDataPtr->UdpSoCon.IsOptRcved
        && !cRtDataPtr->UdpSoCon.IsSoConOpened)
    {
        Std_ReturnType ret = SoAd_OpenSoCon(cRtDataPtr->UdpSoCon.SoConId);
        if (E_OK == ret)
        {
            cRtDataPtr->UdpSoCon.IsSoConOpened = TRUE;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
            Sd_ClientServiceResetAllCEgSubsEntrySendFlag(clientServicePtr, FALSE, TRUE);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

            if (NULL_PTR != consumedMethodsPtr)
            {
                SoAd_RoutingGroupIdType routingGroupId =
                    consumedMethodsPtr->SdClientServiceActivationRef->RoutingGroupId;
                (void)SoAd_EnableSpecificRouting(routingGroupId, cRtDataPtr->UdpSoCon.SoConId);
            }
        }
    }
}
#endif

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
SD_LOCAL void Sd_ClientServiceDownPhaseEnableMulitcast(
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType*     cRtDataPtr)
{
    if ((NULL_PTR != clientServicePtr->SdClientServiceMulticastRef) && cRtDataPtr->MulitcastSoCon.IsOptRcved
        && !cRtDataPtr->MulitcastSoCon.IsSoConOpened)
    {
        Std_ReturnType ret = SoAd_OpenSoCon(cRtDataPtr->MulitcastSoCon.SoConId);
        if (E_OK == ret)
        {
            cRtDataPtr->MulitcastSoCon.IsSoConOpened = TRUE;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
            Sd_ClientServiceResetAllCEgSubsEntrySendFlag(clientServicePtr, FALSE, TRUE);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
        }
    }
}
#endif

/**
 * @brief              Process client service state transition in Down Phase
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74100
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientServiceDownPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId)
{
    Sd_ClientServiceRTType* cRtDataPtr  = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    TcpIp_IpAddrStateType   ipAddrState = Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState;

    /*@req <SWS_SD_00350> */
    if ((SD_INIT_SUCCESS == *Sd_InitResult[applicationId]) && (SD_CLIENT_SERVICE_REQUESTED == cRtDataPtr->SetState)
        && (TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState))
    {
        const Sd_ConsumedMethodsType* consumedMethodsPtr = clientServicePtr->SdConsumedMethods;
        /*@req <SWS_SD_00464> */
        /* An OfferService entry was received and its TTL timer did not expire yet */
        if (cRtDataPtr->Ttl.IsValidForever || (cRtDataPtr->Ttl.ttlms > 0ULL))
        {
/* Open TCP connection if SdClientServiceTcpRef is configured and
 * was not opened before */
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
            Sd_ClientServiceDownPhaseEnableTcp(clientServicePtr, cRtDataPtr, consumedMethodsPtr);
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
            Sd_ClientServiceDownPhaseEnableUdp(clientServicePtr, cRtDataPtr, consumedMethodsPtr);
#endif

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
            Sd_ClientServiceDownPhaseEnableMulitcast(clientServicePtr, cRtDataPtr);
#endif

            /* The Main Phase shall be entered */
            cRtDataPtr->Phase = SD_CLIENT_PHASE_MAIN;
        }
        else
        {
            /* To InitialWait Phase*/
            Sd_ClientServiceInitialWaitEnter(clientServicePtr, cRtDataPtr);
        }
    }
}
#endif

/**
 * @brief              Process client service state transition in Initial With Phase
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74101
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientServiceInitialWaitPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId)
{
    Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    /*@req <SWS_SD_00355>,<SWS_SD_00357> */
    if ((SD_CLIENT_SERVICE_RELEASED == cRtDataPtr->SetState)
        || (TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState))
    {
        Sd_ClientServiceDownPhaseEnter(instancePtr, clientServicePtr, cRtDataPtr, applicationId);
        /*@req <SWS_SD_00456> */
        /* If for any reasons the Initial Wait Phase is left, the calculated random timer (of the
         * Initial Wait Phase) for this Service Instance shall be stopped. */
        cRtDataPtr->InitialWaitTimer = 0u;
    }
    else
    {
        if (cRtDataPtr->InitialWaitTimer > 0UL)
        {
            cRtDataPtr->InitialWaitTimer--;
            /*@req <SWS_SD_00353> */
            if (0UL == cRtDataPtr->InitialWaitTimer)
            {
                /* FindService Entry shall be sent */
                (void)Sd_BuildFindServiceEntry(instancePtr, clientServicePtr, applicationId);

                /* If the SdClientTimerInitialFindRepetitionsMax>0, enter the Repetition Phase*/
                const Sd_ClientTimerType* clientTimerPtr = clientServicePtr->SdClientServiceTimerRef;
                if (clientTimerPtr->SdClientTimerInitialFindRepetitionsMax > 0u)
                {
                    cRtDataPtr->Phase = SD_CLIENT_PHASE_REPETITION;
                    /*@req <SWS_SD_00358> */
                    uint32 initRepetBaseDelay   = clientTimerPtr->SdClientTimerInitialFindRepetitionsBaseDelay;
                    cRtDataPtr->RepetitionTimer = initRepetBaseDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                    cRtDataPtr->RepetitionCount = 0u;
                }
                else
                {
                    /* If the SdClientTimerInitialFindRepetitionsMax=0, enter the Main Phase*/
                    cRtDataPtr->Phase = SD_CLIENT_PHASE_MAIN;
                }

                /*@req <SWS_SD_00456> */
                /* If for any reasons the Initial Wait Phase is left, the calculated random timer (of the
                 * Initial Wait Phase) for this Service Instance shall be stopped. */
                cRtDataPtr->InitialWaitTimer = 0u;
            }
        }
    }
}
#endif

/**
 * @brief              Process client service state transition in Repetition Phase
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74102
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
/* PRQA S 6030 ++ */ /* VL_MTR_Sd_STMIF */
SD_LOCAL void Sd_ClientServiceRepetitionPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId)
/* PRQA S 6030 -- */
{
    Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    /*@req <SWS_SD_00371>,<SWS_SD_00373> */
    if ((SD_CLIENT_SERVICE_RELEASED == cRtDataPtr->SetState)
        || (TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState))
    {
        Sd_ClientServiceDownPhaseEnter(instancePtr, clientServicePtr, cRtDataPtr, applicationId);
    }
    else
    {
        if (cRtDataPtr->RepetitionTimer > 0UL)
        {
            cRtDataPtr->RepetitionTimer--;
            /*@req <SWS_SD_00457> */
            if (0UL == cRtDataPtr->RepetitionTimer)
            {
                /* When the timer SdClientTimerInitialFindRepetitionsBaseDelay expires
                 * within the Repetition Phase, a FindOffer Message shall be sent.*/
                (void)Sd_BuildFindServiceEntry(instancePtr, clientServicePtr, applicationId);

                cRtDataPtr->RepetitionCount++;
                const Sd_ClientTimerType* clientTimerPtr = clientServicePtr->SdClientServiceTimerRef;
                if (cRtDataPtr->RepetitionCount < clientTimerPtr->SdClientTimerInitialFindRepetitionsMax)
                {
                    /*@req <SWS_SD_00363> */
                    uint32 delayTime = (0x01UL << cRtDataPtr->RepetitionCount);
                    delayTime        = clientTimerPtr->SdClientTimerInitialFindRepetitionsBaseDelay * delayTime;
                    cRtDataPtr->RepetitionTimer = delayTime / SD_MAIN_FUNCTION_CYCLE_TIME;
                }
                else if (cRtDataPtr->RepetitionCount == clientTimerPtr->SdClientTimerInitialFindRepetitionsMax)
                {
                    /*@req <SWS_SD_00369> */
                    /* After sending the maximum repetitions (defined by SdClientTimerInitialFindRepetitionsMax)
                     * of FindService entries,the Repetition Phase shall be left and the Main Phase shall be entered. */
                    cRtDataPtr->Phase = SD_CLIENT_PHASE_MAIN;
                }
                else
                {
                    /*Nothing to do here*/
                }
            }
        }
    }
}
#endif

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
static void Sd_ClientSubscribeRetryHandler(
    const Sd_InstanceType*           sdInstancePtr,
    const Sd_ClientServiceType*      clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    Sd_ConsumedEventGroupRTType*     consumedEgRTPtr,
    Sd_ClientServiceRTType*          cRtDataPtr,
    ApplicationType                  applicationId)
{
    const Sd_ClientTimerType* consumedEgTimerPtr = eventGroupPtr->SdConsumedEventGroupTimerRef;
    if ((consumedEgTimerPtr->SdSubscribeEventgroupRetryMax > 0u)
        && (consumedEgRTPtr->SubsRetryCnt <= consumedEgTimerPtr->SdSubscribeEventgroupRetryMax)
        && (0u == consumedEgRTPtr->SubsRetryTimer) && (TRUE == consumedEgRTPtr->SubsRetryEnable))
    {
        if ((SD_CLIENT_PHASE_MAIN == cRtDataPtr->Phase) && !consumedEgRTPtr->IsAnsweredWithAck)
        {
            (void)Sd_BuildStopSubscribeEventgroupEntry(
                sdInstancePtr,
                clientServicePtr,
                eventGroupPtr,
                &cRtDataPtr->OfferEntryRemoteAddr,
                cRtDataPtr,
                FALSE,
                applicationId);
        }

        uint32 sendTime = 0u;
        (void)Sd_BuildSubscribeEventgroupEntry(
            sdInstancePtr,
            clientServicePtr,
            eventGroupPtr,
            cRtDataPtr,
            FALSE,
            &sendTime,
            applicationId);

        if (consumedEgTimerPtr->SdSubscribeEventgroupRetryMax > 0u)
        {
            consumedEgRTPtr->SubsRetryTimer =
                sendTime + (consumedEgTimerPtr->SdSubscribeEventgroupRetryDelay / SD_MAIN_FUNCTION_CYCLE_TIME);
            consumedEgRTPtr->SubsRetryEnable = TRUE;
        }
    }
}
#endif

/**
 * @brief              Handles the subscription to an event for a client service.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          clientServicePtr: Pointer to the client service type.
 * @param[in]          cRtDataPtr: Pointer to the client service runtime data.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74554
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL void Sd_ClientSubscribeEventHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType*     cRtDataPtr,
    ApplicationType             applicationId)
{
    boolean isRxUseMulticast;
    if (cRtDataPtr->OfferEntryRxPduId == instancePtr->SdInstanceMulticastRxPdu)
    {
        isRxUseMulticast = TRUE;
    }
    else
    {
        isRxUseMulticast = FALSE;
    }

    /* A SubscribeEventgroup entry shall be sent out for each currently requested
     * Consumed Eventgroup of this Client Service Instance */
    const Sd_ConsumedEventGroupType* eventGroupPtr = clientServicePtr->SdConsumedEventGroup;
    for (uint16 index = 0u; index < clientServicePtr->ConsumedEventGroupNum; index++)
    {
        Sd_ConsumedEventGroupRTType* consumedEgRTPtr =
            Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];

        TcpIp_SockAddrType tcpIpSockAddr;
        tcpIpSockAddr.domain = instancePtr->SdAddrFamily;
        (void)SoAd_GetRemoteAddr(cRtDataPtr->TcpSoCon.SoConId, &tcpIpSockAddr);

        /*@req <SWS_Sd_00761> */
        if ((SD_CONSUMED_EVENTGROUP_REQUESTED == consumedEgRTPtr->SetState)
            && (((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef)
                 && (TCPIP_E_OK == SoAd_IsConnectionReady(cRtDataPtr->TcpSoCon.SoConId, &tcpIpSockAddr))
                 && (!consumedEgRTPtr->IsTcpSubsSend))
                || ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
                    && (((TCPIP_E_OK == SoAd_IsConnectionReady(cRtDataPtr->UdpSoCon.SoConId, &tcpIpSockAddr))
                         && (!consumedEgRTPtr->IsUdpSubsSend))
                        || ((TCPIP_E_OK == SoAd_IsConnectionReady(cRtDataPtr->MulitcastSoCon.SoConId, &tcpIpSockAddr))
                            && (!consumedEgRTPtr->IsMulitcastSubsSend)))))
            && ((cRtDataPtr->OfferServiceRxFlag) || (!consumedEgRTPtr->IsSendSubEvtAfterRequest)))
        {
            /*@req <SWS_SD_00721>,<SWS_SD_00695> */
            /* StopSubscribeEventgroup entry shall be sent out, if the last SubscribeEventgroup entry was
             * sent as reaction to an OfferService entry received via Multicast, it was never answered with
             * a SubscribeEventgroupAck, and the current OfferService entry was received via Multicast.*/
            if ((SD_CLIENT_PHASE_MAIN == cRtDataPtr->Phase) && !consumedEgRTPtr->IsAnsweredWithAck && isRxUseMulticast)
            {
                (void)Sd_BuildStopSubscribeEventgroupEntry(
                    instancePtr,
                    clientServicePtr,
                    eventGroupPtr,
                    &cRtDataPtr->OfferEntryRemoteAddr,
                    cRtDataPtr,
                    isRxUseMulticast,
                    applicationId);
            }

            uint32 sendTime = 0u;
            (void)Sd_BuildSubscribeEventgroupEntry(
                instancePtr,
                clientServicePtr,
                eventGroupPtr,
                cRtDataPtr,
                isRxUseMulticast,
                &sendTime,
                applicationId);
            /*@req <SWS_SD_00376> */
            if (isRxUseMulticast)
            {
                consumedEgRTPtr->IsAnsweredWithAck = FALSE;
            }
            else
            {
                consumedEgRTPtr->IsAnsweredWithAck = TRUE;
            }

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
            const Sd_ClientTimerType* consumedEgTimerPtr = eventGroupPtr->SdConsumedEventGroupTimerRef;
            if (consumedEgTimerPtr->SdSubscribeEventgroupRetryMax > 0u)
            {
                consumedEgRTPtr->SubsRetryTimer =
                    sendTime + (consumedEgTimerPtr->SdSubscribeEventgroupRetryDelay / SD_MAIN_FUNCTION_CYCLE_TIME);
                consumedEgRTPtr->SubsRetryEnable = TRUE;
            }
#endif

            /* backup remote address for requirement <SWS_SD_00381> to send StopSubscribeEventgroup */
            (void)IStdLib_MemCpy(
                &consumedEgRTPtr->RemoteAddr,
                &cRtDataPtr->OfferEntryRemoteAddr,
                sizeof(cRtDataPtr->OfferEntryRemoteAddr));
            /* Set isSendSubEvtAfterRequest to TRUE,make sure sent SubscribeEventgroup only once after
             * ConsumedEventGroup state change to REQUESTED,if Retry mechanism not setup */
            consumedEgRTPtr->IsSendSubEvtAfterRequest = TRUE;
        }
        else
        {
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
            Sd_ClientSubscribeRetryHandler(
                instancePtr,
                clientServicePtr,
                eventGroupPtr,
                consumedEgRTPtr,
                cRtDataPtr,
                applicationId);
#endif
        }
        eventGroupPtr++;
    }
}
#endif

/**
 * @brief              Handles stopping the subscription to an event for a client service.
 * @param[in]          instancePtr: Pointer to the specified instance.
 * @param[in]          clientServicePtr: Pointer to the client service type.
 * @param[in]          cRtDataPtr: Pointer to the client service runtime data.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74553
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL void Sd_ClientStopSubscribeEventHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType*     cRtDataPtr,
    ApplicationType             applicationId)
{
    const Sd_ConsumedEventGroupType* cEgPtr = clientServicePtr->SdConsumedEventGroup;
    for (uint16 index = 0u; index < clientServicePtr->ConsumedEventGroupNum; index++)
    {
        Sd_ConsumedEventGroupRTType* consumedEgRTPtr =
            Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
        if ((SD_CONSUMED_EVENTGROUP_AVAILABLE == consumedEgRTPtr->CurState)
            && (SD_CONSUMED_EVENTGROUP_RELEASED == consumedEgRTPtr->SetState))
        {
            (void)Sd_BuildStopSubscribeEventgroupEntry(
                instancePtr,
                clientServicePtr,
                cEgPtr,
                &consumedEgRTPtr->RemoteAddr,
                cRtDataPtr,
                FALSE,
                applicationId);

#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_ConsumedEventGroupCurrentState(cEgPtr->SdConsumedEventGroupHandleId, SD_CONSUMED_EVENTGROUP_DOWN);
#endif
            consumedEgRTPtr->CurState = SD_CONSUMED_EVENTGROUP_DOWN;

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
            if (cEgPtr->SdConsumedEventGroupTimerRef->SdSubscribeEventgroupRetryMax > 0u)
            {
                consumedEgRTPtr->SubsRetryCnt    = 0u;
                consumedEgRTPtr->SubsRetryTimer  = 0u;
                consumedEgRTPtr->SubsRetryEnable = FALSE;
            }
#endif
        }

        cEgPtr++;
    }
}
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientEnableAllMethodRoutingGroup(
    const Sd_ClientServiceType*   clientServicePtr,
    const Sd_ClientServiceRTType* cRtDataPtr)
{
    const Sd_ConsumedMethodsType* consumedMethodsPtr = clientServicePtr->SdConsumedMethods;
    if (NULL_PTR != consumedMethodsPtr)
    {
        SoAd_RoutingGroupIdType routingGroupId = consumedMethodsPtr->SdClientServiceActivationRef->RoutingGroupId;
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
        if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && cRtDataPtr->RxTcpEndpointFlag)
        {
            (void)SoAd_EnableSpecificRouting(routingGroupId, cRtDataPtr->TcpSoCon.SoConId);
        }
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
        if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && cRtDataPtr->RxUdpEndpointFlag)
        {
            (void)SoAd_EnableSpecificRouting(routingGroupId, cRtDataPtr->UdpSoCon.SoConId);
        }
#endif
    }
}
#endif

/**
 * @brief              Process client service state transition in Repetition Phase
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74103
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
/*PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
SD_LOCAL void Sd_ClientServiceMainPhaseHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    ApplicationType             applicationId)
{
    Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];

    /*@req <SWS_SD_00381>,<SWS_SD_00380> */
    if ((SD_CLIENT_SERVICE_RELEASED == cRtDataPtr->SetState)
        || (TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId]->IpAddrState))
    {
        Sd_ClientServiceDownPhaseEnter(instancePtr, clientServicePtr, cRtDataPtr, applicationId);
        return;
    }

    /* @req SWS_SD_00721 */
    /* An OfferService entry was received and its TTL timer did not expire yet */
    if (cRtDataPtr->Ttl.IsValidForever || (cRtDataPtr->Ttl.ttlms > 0ULL))
    {
        boolean bSoConOnline = FALSE;
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
        if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && cRtDataPtr->RxTcpEndpointFlag)
        {
            SoAd_SoConModeType tcpSoConMode = SOAD_SOCON_OFFLINE;
            SoAd_GetSoConMode(cRtDataPtr->TcpSoCon.SoConId, &tcpSoConMode);
            if (tcpSoConMode == SOAD_SOCON_ONLINE)
            {
                bSoConOnline = TRUE;
            }
        }
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
        /* PRQA S 2995 ++ */ /* VL_Sd_LoopLogical */
        if (!bSoConOnline && (NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && cRtDataPtr->RxUdpEndpointFlag)
        {
            SoAd_SoConModeType udpSoConMode = SOAD_SOCON_OFFLINE;
            SoAd_GetSoConMode(cRtDataPtr->UdpSoCon.SoConId, &udpSoConMode);
            if (udpSoConMode == SOAD_SOCON_ONLINE)
            {
                bSoConOnline = TRUE;
            }
        }
        /* PRQA S 2995 -- */ /* VL_Sd_LoopLogical */
#endif

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
        /* PRQA S 2995 ++ */ /* VL_Sd_LoopLogical */
        if (!bSoConOnline && (NULL_PTR != clientServicePtr->SdClientServiceMulticastRef)
            && cRtDataPtr->RxMulitcastEndpointFlag)
        {
            SoAd_SoConModeType mulitcastSoConMode = SOAD_SOCON_OFFLINE;
            SoAd_GetSoConMode(cRtDataPtr->MulitcastSoCon.SoConId, &mulitcastSoConMode);
            if (mulitcastSoConMode == SOAD_SOCON_ONLINE)
            {
                bSoConOnline = TRUE;
            }
        }
        /* PRQA S 2995 -- */ /* VL_Sd_LoopLogical */
#endif

        /* Socket Connections are in state SOAD_SOCON_ONLINE */
        if (bSoConOnline)
        {
            if (cRtDataPtr->OfferServiceRxFlag && (SD_CLIENT_SERVICE_AVAILABLE != cRtDataPtr->CurState))
            {
                /* If the client service has not been reported as SD_CLIENT_SERVICE_AVAILABLE*/
                /* Enable specific routing */
                Sd_ClientEnableAllMethodRoutingGroup(clientServicePtr, cRtDataPtr);

/* Report SD_CLIENT_SERVICE_AVAILABLE to the BswM */
#if (SD_BSWM_USED == STD_ON)
                BswM_Sd_ClientServiceCurrentState(
                    clientServicePtr->SdClientServiceHandleId,
                    SD_CLIENT_SERVICE_AVAILABLE);
#endif
                cRtDataPtr->CurState = SD_CLIENT_SERVICE_AVAILABLE;
            }

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
            Sd_ClientSubscribeEventHandle(instancePtr, clientServicePtr, cRtDataPtr, applicationId);
#endif

            cRtDataPtr->OfferServiceRxFlag = FALSE;
        }
    }

/*@req <SWS_SD_00375> */
/* Stay in main phase */

/*@req <SWS_SD_00713>,<SWS_SD_00738> */
/* If the Consumed Event Group is not requested anymore as indicated by a call of
 * Sd_ConsumedEventGroupSetState with state SD_CONSUMED_EVENTGROUP_RELEASED*/
/* A StopSubscribeEventgroup shall be sent */
/* The status shall be set to SD_CONSUMED_EVENTGROUP_DOWN and be reported to the BswM */
/* Cancel the corresponding client service subscription retry delay timer and reset
 * subscription retry counter of all corresponding Eventgroups to 0*/
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    Sd_ClientStopSubscribeEventHandle(instancePtr, clientServicePtr, cRtDataPtr, applicationId);
#endif
}
/*PRQA S 2889 -- */
#endif

/**
 * @brief              Enter Initial Wait Phase Process
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          cRtDataPtr: Pointer for client service runtime data.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74104
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void
    Sd_ClientServiceInitialWaitEnter(const Sd_ClientServiceType* clientServicePtr, Sd_ClientServiceRTType* cRtDataPtr)
{
    /* Enter Initial Wait Phase */
    cRtDataPtr->Phase = SD_CLIENT_PHASE_INITIAL_WAIT;
    /* make the number between SdServerTimerInitialOfferDelayMin to SdServerTimerInitialOfferDelayMax*/
    const Sd_ClientTimerType* timerPtr = clientServicePtr->SdClientServiceTimerRef;
    uint32                    delayMin = timerPtr->SdClientTimerInitialFindDelayMin;
    uint32                    delayMax = timerPtr->SdClientTimerInitialFindDelayMax;
    /* Calculate random delay time */
    uint32 tRandom = Sd_GetRandomValue(delayMin, delayMax);
    /*@req <SWS_SD_00351> */
    /* When entering the Initial Wait Phase, a random timer shall be started */
    cRtDataPtr->InitialWaitTimer = Sd_InitialWaitTimeAdjust(tRandom / SD_MAIN_FUNCTION_CYCLE_TIME);
}
#endif

/**
 * @brief              Process client service Down Phase enter(other than init)
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[inout]       cRtDataPtr: Pointer for client service runtime data.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74105
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientServiceDownPhaseEnter(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType*     cRtDataPtr,
    ApplicationType             applicationId)
{
    /* @req <SWS_SD_00381>,<SWS_SD_00712> */
    if (SD_CLIENT_PHASE_MAIN == cRtDataPtr->Phase)
    {
        /* Clear send queue of this client service */
        Sd_ClearSendQueue(&cRtDataPtr->Head, applicationId);

        /* @req <SWS_SD_00722> */
        /* Call BswM_Sd_ClientServiceCurrentState with state SD_CLIENT_SERVICE_DOWN shall */
        if (SD_CLIENT_SERVICE_AVAILABLE == cRtDataPtr->CurState)
        {
#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_ClientServiceCurrentState(clientServicePtr->SdClientServiceHandleId, SD_CLIENT_SERVICE_DOWN);
#endif
            cRtDataPtr->CurState = SD_CLIENT_SERVICE_DOWN;
        }
        /* Close all Routing */
        Sd_ClientServiceDisableAllRoutingOfClient(clientServicePtr, cRtDataPtr);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        const Sd_ConsumedEventGroupType* cEgPtr = clientServicePtr->SdConsumedEventGroup;
        for (uint16 index = 0u; index < clientServicePtr->ConsumedEventGroupNum; index++)
        {
            Sd_ConsumedEventGroupRTType* cEgRTPtr = Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
            if (SD_CONSUMED_EVENTGROUP_AVAILABLE == cEgRTPtr->CurState)
            {
                /*@req <SWS_SD_00381> */
                /* A StopSubscribeEventgroup shall be sent */
                if (SD_CLIENT_SERVICE_RELEASED == cRtDataPtr->SetState)
                {
                    (void)Sd_BuildStopSubscribeEventgroupEntry(
                        instancePtr,
                        clientServicePtr,
                        cEgPtr,
                        &cEgRTPtr->RemoteAddr,
                        cRtDataPtr,
                        FALSE,
                        applicationId);
                }

/* BswM_Sd_ConsumedEventGroupCurrentState with state SD_CONSUMED_EVENTGROUP_DOWN for
 * all associated eventgroups of this Service Instance shall be called. */
#if (SD_BSWM_USED == STD_ON)
                BswM_Sd_ConsumedEventGroupCurrentState(
                    cEgPtr->SdConsumedEventGroupHandleId,
                    SD_CONSUMED_EVENTGROUP_DOWN);
#endif
                cEgRTPtr->CurState = SD_CONSUMED_EVENTGROUP_DOWN;
            }

/* Cancel the corresponding client service subscription retry delay timer and
 * reset subscription retry counter of all corresponding Eventgroups to 0 */
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
            if (cEgPtr->SdConsumedEventGroupTimerRef->SdSubscribeEventgroupRetryMax > 0u)
            {
                cEgRTPtr->SubsRetryCnt    = 0u;
                cEgRTPtr->SubsRetryTimer  = 0u;
                cEgRTPtr->SubsRetryEnable = FALSE;
            }
#endif

            cEgPtr++;
        }
#endif
    }

    /* Close all SoCon */
    Sd_ClientServiceCloseAllSoCon(clientServicePtr, cRtDataPtr);

    /* @req <SWS_SD_00355>,<SWS_SD_00357>,<SWS_SD_00371>,<SWS_SD_00373>,<SWS_SD_00380>,
     * <SWS_SD_00381>,<SWS_SD_00712> */
    /* Down phase enter */
    cRtDataPtr->Phase = SD_CLIENT_PHASE_DOWN;
}
#endif

/**
 * @brief              Disable all Routing associated with this Client Service
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[inout]       cRtDataPtr: Pointer for client service runtime data.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74106
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientServiceDisableAllRoutingOfClient(
    const Sd_ClientServiceType*   clientServicePtr,
    const Sd_ClientServiceRTType* cRtDataPtr)
{
    /*@req <SWS_SD_00382>,<SWS_SD_00722> */
    if ((SD_CLIENT_PHASE_REPETITION == cRtDataPtr->Phase) || (SD_CLIENT_PHASE_MAIN == cRtDataPtr->Phase))
    {
        const Sd_ConsumedMethodsType* consumedMethodsPtr = clientServicePtr->SdConsumedMethods;
        if (NULL_PTR != consumedMethodsPtr)
        {
            SoAd_RoutingGroupIdType routingGroupId = consumedMethodsPtr->SdClientServiceActivationRef->RoutingGroupId;

#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
            if (cRtDataPtr->TcpSoCon.IsSoConOpened)
            {
                (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->TcpSoCon.SoConId);
            }
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
            if (cRtDataPtr->UdpSoCon.IsSoConOpened)
            {
                (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->UdpSoCon.SoConId);
            }
#endif
        }

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        const Sd_ConsumedEventGroupType* cEgPtr = clientServicePtr->SdConsumedEventGroup;
        for (uint16 index = 0u; index < clientServicePtr->ConsumedEventGroupNum; index++)
        {
            Sd_ClientServiceDisableAllConsumedEventgroupRouting(cEgPtr, cRtDataPtr);
            cEgPtr++;
        }
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
    }
}
#endif

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
/**
 * @brief              Disable all Routing associated with this Client Service
 * @param[in]          cEgPtr: Pointer for consumed event group.
 * @param[inout]       cRtDataPtr: Pointer for client service runtime data.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74107
 */
SD_LOCAL void Sd_ClientServiceDisableAllConsumedEventgroupRouting(
    const Sd_ConsumedEventGroupType* cEgPtr,
    const Sd_ClientServiceRTType*    cRtDataPtr)
{
    Sd_ConsumedEventGroupRTType* cEgRTPtr = Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
    if ((NULL_PTR != cEgPtr->SdConsumedEventGroupTcpActivationRef) && cRtDataPtr->TcpSoCon.IsSoConOpened)
    {
        SoAd_RoutingGroupIdType routingGroupId = cEgPtr->SdConsumedEventGroupTcpActivationRef->RoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->TcpSoCon.SoConId);

        cEgRTPtr->IsTcpSubsSend = FALSE;
    }
#endif

#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
    if ((NULL_PTR != cEgPtr->SdConsumedEventGroupUdpActivationRef) && cRtDataPtr->UdpSoCon.IsSoConOpened)
    {
        SoAd_RoutingGroupIdType routingGroupId = cEgPtr->SdConsumedEventGroupUdpActivationRef->RoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->UdpSoCon.SoConId);

        cEgRTPtr->IsUdpSubsSend = FALSE;
    }
#endif

#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
    if ((NULL_PTR != cEgPtr->SdConsumedEventGroupUdpActivationRef) && cRtDataPtr->MulitcastSoCon.IsSoConOpened)
    {
        SoAd_RoutingGroupIdType routingGroupId = cEgPtr->SdConsumedEventGroupUdpActivationRef->RoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->MulitcastSoCon.SoConId);

        cEgRTPtr->IsUdpSubsSend = FALSE;
    }
#endif

#if (SD_CONSUMED_EVENT_MULICAST_USED == STD_ON)
    if ((NULL_PTR != cEgPtr->SdConsumedEventGroupMulticastActivationRef) && cEgRTPtr->MultiSoCon.IsSoConOpened)
    {
        SoAd_RoutingGroupIdType routingGroupId = cEgPtr->SdConsumedEventGroupMulticastActivationRef->RoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cEgRTPtr->MultiSoCon.SoConId);
    }
#endif
}
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

/**
 * @brief              Close all Socket Connections associated with this Client Service Instance that
 *                     have been opened before
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[inout]       cRtDataPtr: Pointer for client service runtime data.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74108
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void
    Sd_ClientServiceCloseAllSoCon(const Sd_ClientServiceType* clientServicePtr, Sd_ClientServiceRTType* cRtDataPtr)
{
    /*@req <SWS_SD_00603>,<SWS_SD_00705>,<SWS_SD_00706> */
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
    Sd_ClientServiceCloseSoCon(&cRtDataPtr->TcpSoCon);
#endif
#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
    Sd_ClientServiceCloseSoCon(&cRtDataPtr->UdpSoCon);
#endif
#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
    Sd_ClientServiceCloseSoCon(&cRtDataPtr->MulitcastSoCon);
#endif

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    const Sd_ConsumedEventGroupType* cEgPtr = clientServicePtr->SdConsumedEventGroup;
    for (uint16 index = 0u; index < clientServicePtr->ConsumedEventGroupNum; index++)
    {
        Sd_ConsumedEventGroupRTType* cEgRTPtr = Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
        Sd_ClientServiceCloseSoCon(&cEgRTPtr->MultiSoCon);
        /* @req SWS_SD_00734 */
        /* Every wildcard socket connection group shall be reset to wildcard using
         * SoAd_ReleaseIpAddrAssignment()*/
        (void)SoAd_ReleaseIpAddrAssignment(cEgRTPtr->MultiSoCon.SoConId);

        cEgPtr++;
    }
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
}
#endif

/**
 * @brief              Close socket connection(Client Service)
 * @param[in]          soConPtr: Pointer for socket connection manage data.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74109
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientServiceCloseSoCon(Sd_SoConManageType* soConPtr)
{
    /*@req <SWS_SD_00603> */
    /* When the Down Phase is entered and SoAd_OpenSoCon() was called before, the
     * API SoAd_CloseSoCon()shall be called for all Socket Connections associated with
     * this Client Service Instance.*/
    if (soConPtr->IsSoConOpened)
    {
        (void)SoAd_CloseSoCon(soConPtr->SoConId, TRUE);
        soConPtr->IsSoConOpened = FALSE;
        soConPtr->IsOptRcved    = FALSE;

        /*@req <SWS_SD_00706> */
        SoAd_ReleaseRemoteAddr(soConPtr->SoConId);
    }
}
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

/**
 * @brief              Switch data byte order.
 * @param[in]          size: Size of data buffer.
 * @param[out]         dataPtr: Pointer for data buffer.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74110
 */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
SD_LOCAL_INLINE void Sd_EndianSwap(uint8* dataPtr, uint8 size)
{
    uint8 halfSize = size >> 1u;
    uint8 tSize    = size - 1u;

    for (uint8 i = 0u; i < halfSize; i++)
    {
        uint8 tIdx    = tSize - i;
        uint8 temp    = dataPtr[tIdx];
        dataPtr[tIdx] = dataPtr[i];
        dataPtr[i]    = temp;
    }
}
#endif

/**
 * @brief              Switch 32bit Network byte order to Host Byte Order
 * @param[in]          net32: 32bit network buffer
 * @return             a uint32 data with Host Byte Order
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74112
 */
SD_LOCAL uint32 Sd_NtoHl(uint32 net32)
{
    uint32 data = net32;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&data, SD_ENDIAN_SWAP_FOUR_BYTE_LEN);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return data;
}

/**
 * @brief              Switch 32bit Network byte order to Host Byte Order(In parameter is a pointer)
 * @param[in]          net32DataPtr: 32bit network buffer
 * @return             a uint32 data with Host Byte Order
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74113
 */
SD_LOCAL uint32 Sd_NPtrtoHl(const uint8* net32DataPtr)
{
    uint32 net32;

    (void)IStdLib_MemCpy((uint8*)&net32, net32DataPtr, SD_ENDIAN_SWAP_FOUR_BYTE_LEN);
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&net32, SD_ENDIAN_SWAP_FOUR_BYTE_LEN);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return net32;
}

/**
 * @brief              Switch 32bit Host byte order to Network Byte Order
 * @param[in]          host32: 32bit host network buffer
 * @return             a uint32 data with Host Byte Order
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74114
 */
SD_LOCAL uint32 Sd_HtoNl(uint32 host32)
{
    uint32 data = host32;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&data, SD_ENDIAN_SWAP_FOUR_BYTE_LEN);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return data;
}

/**
 * @brief              Switch 16bit Network byte order to Host Byte Order
 * @param[in]          net16: 16bit network buffer
 * @return             a uint16 data with Host Byte Order
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74115
 */
SD_LOCAL uint16 Sd_NtoHs(uint16 net16)
{
    uint16 data = net16;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&data, SD_ENDIAN_SWAP_TWO_BYTE_LEN);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return data;
}

/**
 * @brief              Switch 16bit Network byte order to Host Byte Order(In parameter is a pointer)
 * @param[in]          net16DataPtr: 16bit network buffer
 * @return             a uint16 data with Host Byte Order
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74116
 */
SD_LOCAL uint16 Sd_NPtrtoHs(const uint8* net16DataPtr)
{
    uint16 net16;

    (void)IStdLib_MemCpy((uint8*)&net16, net16DataPtr, SD_ENDIAN_SWAP_TWO_BYTE_LEN);
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&net16, SD_ENDIAN_SWAP_TWO_BYTE_LEN);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return net16;
}

/**
 * @brief              Switch 16bit Host byte order to Network Byte Order
 * @param[inout]       host16: 16bit host network buffer
 * @return             a uint16 data with Host Byte Order
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74117
 */
SD_LOCAL uint16 Sd_HtoNs(uint16 host16)
{
    uint16 data = host16;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&data, SD_ENDIAN_SWAP_TWO_BYTE_LEN);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return data;
}

/**
 * @brief              Init memory pool
 * @param[in]          memPoolSize: memory pool size.
 * @param[in]          memPool: Pointer for memory pool.
 * @return             memory pool avaliable size
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74118
 */
SD_LOCAL_INLINE uint8 Sd_InitMemoryPool(uint32 memPoolSize, void* memPool)
{
    return IStdLib_MemHeapInit(memPool, memPoolSize);
}

/**
 * @brief              require buffer from memory pool
 * @param[in]          size: Expected number of bytes to be allocated.
 * @param[in]          memPool: Pointer for memory pool.
 * @return             buffer pointer,NULL if not have enough buffer
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74119
 */
SD_LOCAL void* Sd_MallocEx(uint32 size, void* memPool)
{
    SchM_Enter_Sd_ExclusiveArea();
    void* ret = IStdLib_MemHeapMalloc(memPool, size);
    SchM_Exit_Sd_ExclusiveArea();
#if (SD_MALLOC_FAIL_REPORT_ENABLE == STD_ON)
    if (NULL_PTR == ret)
    {
        Dem_ReportErrorStatus(SD_MALLOC_FAIL_EVENTID, DEM_EVENT_STATUS_FAILED);
    }
#endif
    return ret;
}

/**
 * @brief              Free memory from a memory pool
 * @param[in]          ptr: Buffer pointer to be free
 * @param[in]          memPool: Pointer for memory pool.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74120
 */
SD_LOCAL void Sd_FreeEx(const void* ptr, void* memPool)
{
    SchM_Enter_Sd_ExclusiveArea();
    (void)IStdLib_MemHeapFree(memPool, ptr);
    SchM_Exit_Sd_ExclusiveArea();
}

/**
 * @brief              Append RxBuff item to RxBuffList tail
 * @param[in]          rxQueueItemPtr: Pointer for item that would be append to rx buffer.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74121
 */
#if (SD_INSTANCE_NUM == STD_ON)
SD_LOCAL void Sd_AppendToRxBuffTail(Sd_RxQueueType* rxQueueItemPtr, ApplicationType applicationId)
{
    SchM_Enter_Sd_ExclusiveArea();
    if (NULL_PTR != Sd_RxQueueHead[applicationId])
    {
        Sd_RxQueueType* tail = Sd_RxQueueHead[applicationId];
        while (NULL_PTR != tail->Next)
        {
            tail = tail->Next;
        }
        tail->Next = rxQueueItemPtr;
    }
    else
    {
        Sd_RxQueueHead[applicationId] = rxQueueItemPtr;
    }
    SchM_Exit_Sd_ExclusiveArea();
}
#endif

#if (SD_EVENT_HANDLER_NUM > 0)

/**
 * @brief              Add first subscriber to trigger transmit list
 * @param[inout]       head: Header of the UDP/TCP Trigger transmit list.
 * @param[in]          routingGroupId: routing group id
 * @param[in]          soConId: Socket connection id
 * @return             Sd_TriggerTransmitManagerType*
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace
 */
/* PRQA S 2889 ++ */ /* VL_QAC_DerefNullPtr */
static void Sd_AddClientToTriggerTransmitList(
    Sd_TriggerTransmitManagerType** head,
    SoAd_RoutingGroupIdType         routingGroupId,
    SoAd_SoConIdType                soConId,
    ApplicationType                 applicationId)
{
    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    Sd_TriggerTransmitManagerType* tmpNode =
        (Sd_TriggerTransmitManagerType*)Sd_MallocEx(sizeof(Sd_TriggerTransmitManagerType), Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */ /* VL_QAC_0316 */
    if (tmpNode == NULL_PTR)
    {
        return;
    }

    tmpNode->Next           = NULL_PTR;
    tmpNode->RoutingGroupId = routingGroupId;
    tmpNode->SoConId        = soConId;

    if (*head == NULL_PTR)
    {
        *head = tmpNode;
    }
    else
    {
        Sd_TriggerTransmitManagerType* nextNodePtr = *head;
        while (nextNodePtr->Next != NULL_PTR)
        {
            nextNodePtr = nextNodePtr->Next;
        }
        nextNodePtr->Next = tmpNode;
    }
}
/* PRQA S 2889 -- */

/**
 * @brief              Pop first subscriber from trigger transmit list
 * @param[in]          head: Header of the UDP/TCP Trigger transmit list.
 * @return             Sd_TriggerTransmitManagerType*
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace
 */
static Sd_TriggerTransmitManagerType* Sd_PopFirstClientFromTriggerTransmitList(Sd_TriggerTransmitManagerType** head)
{
    Sd_TriggerTransmitManagerType* firstNode = *head;

    if (firstNode != NULL_PTR)
    {
        *head = firstNode->Next;
    }

    return firstNode;
}

/**
 * @brief              Delete subscriber from trigger transmit list by socinid
 * @param[in]          soConId: Socket connection id.
 * @param[inout]          head: Header of the UDP/TCP Trigger transmit list.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
static void Sd_DelClientFromTriggerTransmitListBySoConId(
    Sd_TriggerTransmitManagerType** head,
    SoAd_SoConIdType                soConId,
    ApplicationType                 applicationId)
{
    Sd_TriggerTransmitManagerType* tmpNode = *head;
    Sd_TriggerTransmitManagerType* preNode = *head;

    if (tmpNode == NULL_PTR)
    {
        return;
    }

    if (tmpNode->SoConId == soConId)
    {
        *head = tmpNode->Next;
        Sd_FreeEx(tmpNode, Sd_TxMemPool[applicationId]);
    }
    else
    {
        while (NULL_PTR != tmpNode->Next)
        {
            tmpNode = tmpNode->Next;
            if (soConId == tmpNode->SoConId)
            {
                preNode->Next = tmpNode->Next;
                Sd_FreeEx(tmpNode, Sd_TxMemPool[applicationId]);
                break;
            }
            preNode = tmpNode;
        }
    }
}
/* PRQA S 2889 -- */

/**
 * @brief              Return a Sd_SubscribeManageType opinter that match soConId and counter
 * @param[in]          header: Header of the subscribe entry list.
 * @param[in]          sockRemoteAddr: Remote ip address.
 * @param[in]          clientIpAddr: Client ip address.
 * @param[in]          counter: The counter field for subscribe entry
 * @return             Sd_SubscribeManageType opinter that match soConId and counter
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74122
 */
/* PRQA S 2889 ++ */ /* VL_QAC_MultiReturn */
SD_LOCAL Sd_SubscribeManageType* Sd_GetSubscribeManageObjAddr(
    Sd_SubscribeManageType* header,
    TcpIp_SockAddrType      sockRemoteAddr,
    TcpIp_SockAddrType      clientIpAddr,
    uint8                   counter)
{
    Sd_SubscribeManageType* ptr = header;

    while (NULL_PTR != ptr)
    {
        if ((counter == ptr->Counter) && (TCPIP_EQ_IPADDR(ptr->SockRemoteAddr, sockRemoteAddr))
            && (ptr->SockRemoteAddr.port == sockRemoteAddr.port)
            && (0u
                == IStdLib_MemCmp(
                    (uint8*)&ptr->ClientIpAddr.addr,
                    (uint8*)&clientIpAddr.addr,
                    sizeof(clientIpAddr.addr))))
        {
            break;
        }

        ptr = ptr->Next;
    }

    return ptr;
}
/* PRQA S 2889 -- */ /* VL_QAC_MultiReturn */

#if (SD_EVENT_HANDLER_MULICAST_USED == STD_ON)
/**
 * @brief              Enable unicast for Subscribed Clients
 * @param[in]          header: Header of the subscribe entry list.
 * @param[in]          routingGroupId: Routing group id.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74123
 */
SD_LOCAL void
    Sd_EnableUnicastForSubscribedClient(const Sd_SubscribeManageType* header, SoAd_RoutingGroupIdType routingGroupId)
{
    const Sd_SubscribeManageType* ptr = header;

    while (NULL_PTR != ptr)
    {
        (void)SoAd_EnableSpecificRouting(routingGroupId, ptr->SoConId);
        ptr = ptr->Next;
    }
}

/**
 * @brief              Disable unicast for Subscribed Clients
 * @param[in]          header: Header of the subscribe entry list.
 * @param[in]          routingGroupId: Routing group id.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74124
 */
SD_LOCAL void
    Sd_DisableUnicastForSubscribedClient(const Sd_SubscribeManageType* header, SoAd_RoutingGroupIdType routingGroupId)
{
    const Sd_SubscribeManageType* ptr = header;

    while (NULL_PTR != ptr)
    {
        (void)SoAd_DisableSpecificRouting(routingGroupId, ptr->SoConId);
        ptr = ptr->Next;
    }
}
#endif

/**
 * @brief              Append a client to subscribe list
 * @param[inout]       headerPtr: Header of the subscribe entry list.
 * @param[in]          soConId: Socket connection id.
 * @param[in]          sockRemoteAddr: remote ip address.
 * @param[in]          clientIpAddr: Client ip address.
 * @param[in]          ttl: lifetime of the subscribe.
 * @param[in]          counter: The counter field of subscribe entry.
 * @param[in]          applicationId Application ID.
 * @return             E_OK: Append OK
 *                     E_NOT_OK: Append fail,no enough buffer
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74125
 */
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL Std_ReturnType Sd_AddClientToSubscribeList(
    Sd_SubscribeManageType** headerPtr,
    SoAd_SoConIdType         soConId,
    TcpIp_SockAddrType       sockRemoteAddr,
    TcpIp_SockAddrType       clientIpAddr,
    uint32                   ttl,
    uint8                    counter,
    ApplicationType          applicationId)
/* PRQA S 6040 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    Sd_SubscribeManageType* subscribePtr =
        (Sd_SubscribeManageType*)Sd_MallocEx(sizeof(Sd_SubscribeManageType), Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */

    if (NULL_PTR != subscribePtr)
    {
        subscribePtr->SoConId        = soConId;
        subscribePtr->Counter        = counter;
        subscribePtr->SockRemoteAddr = sockRemoteAddr;
        subscribePtr->ClientIpAddr   = clientIpAddr;
        subscribePtr->Next           = NULL_PTR;
        Sd_SetTTL(&subscribePtr->Ttl, ttl);

        Sd_SubscribeManageType* ptr = *headerPtr;
        if (NULL_PTR == ptr)
        {
            *headerPtr = subscribePtr;
        }
        else
        {
            while (NULL_PTR != ptr->Next)
            {
                ptr = ptr->Next;
            }
            ptr->Next = subscribePtr;
        }
        ret = E_OK;
    }

    return ret;
}

/**
 * @brief              Get the number of clients from the subscribeList.
 * @param[inout]       headerPtr: Header of the subscribe entry list.
 * @param[in]          clientIpAddr: Client IP address.
 * @param[in]          soConId: Socket connection id.
 * @param[in]          counter: The counter field of subscribe entry.
 * @return             Client number.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74170
 */
SD_LOCAL uint8 Sd_GetClientNumFromSubscribeList(
    const Sd_SubscribeManageType* headerPtr,
    const TcpIp_SockAddrType*     clientIpAddr,
    SoAd_SoConIdType              soConId,
    uint8                         counter)
{
    uint8                         clientNum = 0u;
    const Sd_SubscribeManageType* ptr       = headerPtr;

    while (NULL_PTR != ptr)
    {
        if ((soConId == ptr->SoConId) && ((SD_SUBSCRIBE_COUNTER_FIELD_RESERVED == counter) || (counter == ptr->Counter))
            && ((clientIpAddr == NULL_PTR)
                || (0u
                    == IStdLib_MemCmp(
                        (const uint8*)clientIpAddr,
                        (const uint8*)&ptr->ClientIpAddr,
                        sizeof(TcpIp_SockAddrType)))))
        {
            clientNum++;
        }
        ptr = ptr->Next;
    }

    return clientNum;
}

/**
 * @brief              Delete a client from subscribe list
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          eventHandlerPtr: Pointer for event handle.
 * @param[inout]       headerPtr: Header of the subscribe entry list.
 * @param[in]          clientIpAddr: Pointer for client IP address.
 * @param[in]          soConId: Socket connection id.
 * @param[in]          counter: The counter field of subscribe entry.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74126
 */
/* PRQA S 6040,6080 ++ */ /* VL_MTR_Sd_STPAR,VL_MTR_Sd_STPTH */
SD_LOCAL void Sd_DeleteClientFromSubscribeList(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    Sd_SubscribeManageType**    headerPtr,
    const TcpIp_SockAddrType*   clientIpAddr,
    SoAd_SoConIdType            soConId,
    uint8                       counter,
    ApplicationType             applicationId)
/* PRQA S 6040,6080 -- */
{
    boolean isTcpSoCon;

    const Sd_EventHandlerRTType* ehRTDataPtr = Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
    if (ehRTDataPtr->TcpSubscribeList == *headerPtr)
    {
        isTcpSoCon = TRUE;
    }
    else
    {
        isTcpSoCon = FALSE;
    }

    Sd_SubscribeManageType* ptr     = *headerPtr;
    Sd_SubscribeManageType* lastPtr = NULL_PTR;
    while (NULL_PTR != ptr)
    {
        if ((soConId == ptr->SoConId) && ((SD_SUBSCRIBE_COUNTER_FIELD_RESERVED == counter) || (counter == ptr->Counter))
            && ((clientIpAddr == NULL_PTR)
                || (0u
                    == IStdLib_MemCmp(
                        (const uint8*)clientIpAddr,
                        (const uint8*)&ptr->ClientIpAddr,
                        sizeof(TcpIp_SockAddrType)))))
        {
            break;
        }

        lastPtr = ptr;
        ptr     = ptr->Next;
    }

    if (NULL_PTR == lastPtr)
    {
        *headerPtr = ptr->Next; /* PRQA S 2813 */ /* VL_Sd_PointerIsNull */
    }
    else
    {
        lastPtr->Next = ptr->Next;
    }

    if ((0u == ehRTDataPtr->TcpSubsClientNum) && (0u == ehRTDataPtr->UdpSubsEndPointNum))
    {
        /*@req <SWS_SD_00334>,<SWS_SD_00345>,<SWS_SD_00458>,<SWS_SD_00403> */
        /* If this has been the last subscribed client, report SD_EVENT_HANDLER_RELEASED
         * to the BswM by calling the API BswM_Sd_EventHandlerCurrentState().*/
        uint16 eventHandlerId = eventHandlerPtr->SdEventHandlerHandleId;
#if (SD_BSWM_USED == STD_ON)
        BswM_Sd_EventHandlerCurrentState(eventHandlerId, SD_EVENT_HANDLER_RELEASED);
#endif
        Sd_EventHandlerRTData[eventHandlerId]->CurState = SD_EVENT_HANDLER_RELEASED;
    }

    uint8 clientNum = 0u;
    if (isTcpSoCon)
    {
        clientNum = Sd_GetClientNumFromSubscribeList(
            ehRTDataPtr->TcpSubscribeList,
            NULL_PTR,
            soConId,
            SD_SUBSCRIBE_COUNTER_FIELD_RESERVED);
    }
    else
    {
        clientNum = Sd_GetClientNumFromSubscribeList(
            ehRTDataPtr->UdpSubscribeList,
            NULL_PTR,
            soConId,
            SD_SUBSCRIBE_COUNTER_FIELD_RESERVED);
    }

    if (0u == clientNum)
    {
        Sd_ResetSoConToWildcard(serverServicePtr, soConId, isTcpSoCon);
    }

    Sd_FreeEx(ptr, Sd_TxMemPool[applicationId]);
}

/**
 * @brief              Delete all clients from subscribe list
 * @param[inout]       headerPtr: Header of the subscribe entry list.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74127
 */
SD_LOCAL void Sd_RemoveAllClientFromSubscribeList(Sd_SubscribeManageType** headerPtr, ApplicationType applicationId)
{
    const Sd_SubscribeManageType* nowPtr = *headerPtr;

    while (NULL_PTR != nowPtr)
    {
        const Sd_SubscribeManageType* nextPtr = nowPtr->Next;

        Sd_FreeEx(nowPtr, Sd_TxMemPool[applicationId]);

        nowPtr = nextPtr;
    }

    *headerPtr = NULL_PTR;
}

/**
 * @brief              Update subscribed client's TTL
 * @param[inout]       subscribePtr: Pointer for subscribe manager.
 * @param[in]          ttl: Lifetime of the subscribe.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74128
 */
SD_LOCAL void Sd_UpdateSubscribeClientTTL(Sd_SubscribeManageType* subscribePtr, uint32 ttl)
{
    Sd_SetTTL(&subscribePtr->Ttl, ttl);
}

/*@req <SWS_Sd_00452> */
/**
 * @brief              Handle subscribe client's TTL if ttl expired,delete it from subscribe list
 * @param[in]          serverServicePtr: Pointer server service.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74129
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Sd_STMIF */
SD_LOCAL void Sd_SubscribeClientTTLManage(const Sd_ServerServiceType* serverServicePtr, ApplicationType applicationId)
/* PRQA S 6030 -- */
{
    const Sd_EventHandlerType* eventHandlerPtr = serverServicePtr->SdEventHandler;
    for (uint16 idx = 0u; idx < serverServicePtr->EventHandlerNum; idx++)
    {
        Sd_EventHandlerRTType* ehRTDataPtr = Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];

        /* Handle TCP subscribed client */
        Sd_SubscribeManageType* subsMangPtr = NULL_PTR;
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
        subsMangPtr = ehRTDataPtr->TcpSubscribeList;
        while (NULL_PTR != subsMangPtr)
        {
            if (subsMangPtr->Ttl.ttlms > 0ULL)
            {
                subsMangPtr->Ttl.ttlms--;
                if (0ULL == subsMangPtr->Ttl.ttlms)
                {
                    /* Disable Routing Path*/
                    const Sd_EventHandlerTcpType* eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
                    if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
                    {
                        SoAd_RoutingGroupIdType routingGroupId =
                            eventHandlerTcpPtr->SdEventActivationRef->RoutingGroupId;
                        (void)SoAd_DisableSpecificRouting(routingGroupId, subsMangPtr->SoConId);
                    }

                    ehRTDataPtr->TcpSubsClientNum--;
                    Sd_DeleteClientFromSubscribeList(
                        serverServicePtr,
                        eventHandlerPtr,
                        &ehRTDataPtr->TcpSubscribeList,
                        NULL_PTR,
                        subsMangPtr->SoConId,
                        subsMangPtr->Counter,
                        applicationId);
                }
            }

            subsMangPtr = subsMangPtr->Next;
        }
#endif

/* Handle UDP subscribed client */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
        subsMangPtr = ehRTDataPtr->UdpSubscribeList;
        while (NULL_PTR != subsMangPtr)
        {
            if (subsMangPtr->Ttl.ttlms > 0ULL)
            {
                subsMangPtr->Ttl.ttlms--;
                if (0ULL == subsMangPtr->Ttl.ttlms)
                {
                    ehRTDataPtr->UdpSubsClientNum--;
                    uint8 clientNum = Sd_GetClientNumFromSubscribeList(
                        ehRTDataPtr->UdpSubscribeList,
                        NULL_PTR,
                        subsMangPtr->SoConId,
                        SD_SUBSCRIBE_COUNTER_FIELD_RESERVED);
                    if (clientNum == 1u)
                    {
                        ehRTDataPtr->UdpSubsEndPointNum--;
                    }

                    /* Fan out control*/
                    Sd_FanOutMulticastUnicastCtrlDelClient(eventHandlerPtr, ehRTDataPtr, subsMangPtr->SoConId);
                    Sd_DeleteClientFromSubscribeList(
                        serverServicePtr,
                        eventHandlerPtr,
                        &ehRTDataPtr->UdpSubscribeList,
                        &subsMangPtr->ClientIpAddr,
                        subsMangPtr->SoConId,
                        subsMangPtr->Counter,
                        applicationId);
                }
            }

            subsMangPtr = subsMangPtr->Next;
        }
#endif

        eventHandlerPtr++;
    }
}

/**
 * @brief              Fan out control TCP
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          eventHandlerPtr: Pointer for event handler.
 * @param[in]          clientIpAddr: Client ip address.
 * @param[in]          sdMsg: Received Sd Message.
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @param[in]          applicationId Application ID.
 * @return             E_OK: No error
 *                     E_NOT_OK: error,need response use Nack entry
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74130
 */
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL Std_ReturnType Sd_FanOutControlTCP(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    TcpIp_SockAddrType          clientIpAddr,
    const uint8*                sdMsg,
    uint16                      entryPos,
    const uint16                optStartPosMap[],
    ApplicationType             applicationId)
/* PRQA S 6040 -- */
{
#if (STD_ON == SD_IPV6_ENABLE)
    uint16 ipv6EndpTcpPos = 0u;
#endif
    uint16            ipv4EndpTcpPos = 0u;
    Std_ReturnType    ret            = E_OK;
    Sd_Type2EntryType type2Entry;

    (void)IStdLib_MemCpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));
    uint8  idx1stOptions = type2Entry.Index1stOptions;
    uint8  numOpt1       = type2Entry.NumOfOpt1;
    uint8  idx2ndOptions = type2Entry.Index2ndOptions;
    uint8  numOpt2       = type2Entry.NumOfOpt2;
    uint32 ttl           = 0u;
    /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
    (void)IStdLib_MemCpy((((uint8*)&ttl) + 1u), type2Entry.TTL, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
    /* PRQA S 0489 -- */
    ttl = Sd_NtoHl(ttl);

    /* Find options position */
    uint8 optNumSum = numOpt1 + numOpt2;
    uint8 optIndex  = 0u;
    for (uint8 index = 0u; index < optNumSum; index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        uint8 optType = sdMsg[optStartPosMap[optIndex] + SD_OPTION_TYPE_FILED_OFFSET];
        if (SD_IPV4_ENDPOINT_OPTION == optType)
        {
            Sd_IPv4OptionsType ipv4Opt;
            (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
            if ((SD_L4_PROTO_TCP == ipv4Opt.L4Proto) && (0UL == ipv4EndpTcpPos))
            {
                /* Save endpoint option position */
                ipv4EndpTcpPos = optStartPosMap[optIndex];
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else if (SD_IPV6_ENDPOINT_OPTION == optType)
        {
            Sd_IPv6OptionsType ipv6Opt;
            (void)IStdLib_MemCpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
            if ((SD_L4_PROTO_TCP == ipv6Opt.L4Proto) && (0UL == ipv6EndpTcpPos))
            {
                /* Save endpoint option position */
                ipv6EndpTcpPos = optStartPosMap[optIndex];
            }
        }
        else
        {
            /*Nothing to do here*/
        }
#endif /* STD_ON == SD_IPV6_ENABLE */

        if ((0UL != ipv4EndpTcpPos)
#if (STD_ON == SD_IPV6_ENABLE)
            || (0UL != ipv6EndpTcpPos)
#endif /* STD_ON == SD_IPV6_ENABLE */
        )
        {
            break;
        }
    }

    if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
    {
        Sd_IPv4OptionsType ipv4Opt;
        /* Fan out control(TCP,IPv4) */
        (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[ipv4EndpTcpPos], sizeof(Sd_IPv4OptionsType));
        ret = Sd_FanOutControlTcpHandle(
            serverServicePtr,
            eventHandlerPtr,
            &ipv4Opt,
            NULL_PTR,
            clientIpAddr,
            ttl,
            (uint8)type2Entry.Counter,
            applicationId);
    }
#if (STD_ON == SD_IPV6_ENABLE)
    else
    {
        Sd_IPv6OptionsType ipv6Opt;
        /* Fan out control(TCP,IPv6) */
        (void)IStdLib_MemCpy((uint8*)&ipv6Opt, &sdMsg[ipv6EndpTcpPos], sizeof(Sd_IPv6OptionsType));
        ret = Sd_FanOutControlTcpHandle(
            serverServicePtr,
            eventHandlerPtr,
            NULL_PTR,
            &ipv6Opt,
            clientIpAddr,
            ttl,
            (uint8)type2Entry.Counter);
    }
#endif

    return ret;
}
#endif

/**
 * @brief              Fan out control UDP
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          eventHandlerPtr: Pointer for event handler.
 * @param[in]          clientIpAddr: Client ip address.
 * @param[in]          sdMsg: Received Sd Message
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @param[in]          applicationId Application ID.
 * @return             E_OK: No error
 *                     E_NOT_OK: error,need response use Nack entry
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74131
 */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL Std_ReturnType Sd_FanOutControlUDP(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    TcpIp_SockAddrType          clientIpAddr,
    const uint8*                sdMsg,
    uint16                      entryPos,
    const uint16                optStartPosMap[],
    ApplicationType             applicationId)
/* PRQA S 6040 -- */
{
#if (STD_ON == SD_IPV6_ENABLE)
    uint32 ipv6EndpUdpPos = 0UL; /* Store first UDP IPv6 Endpoint Option Position */
#endif
    /* Store first UDP IPv4 Endpoint Option Position */
    Std_ReturnType    ret = E_OK;
    Sd_Type2EntryType type2Entry;
    (void)IStdLib_MemCpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));

    uint32 ipv4EndpUdpPos = 0UL;
    uint32 ttl            = 0u;
    /* PRQA S 0489 ++ */ /* VL_Sd_PointerCalu */
    (void)IStdLib_MemCpy((((uint8*)&ttl) + 1u), type2Entry.TTL, SD_MESSAGE_ENTRY_TTL_FIELD_LEN);
    /* PRQA S 0489 -- */
    ttl = Sd_NtoHl(ttl);

    /* Find options position */
    uint8 idx1stOptions = type2Entry.Index1stOptions;
    uint8 numOpt1       = type2Entry.NumOfOpt1;
    uint8 idx2ndOptions = type2Entry.Index2ndOptions;
    uint8 numOpt2       = type2Entry.NumOfOpt2;
    uint8 optNumSum     = numOpt1 + numOpt2;

    for (uint8 index = 0u; (index < optNumSum) && (E_OK == ret); index++) /* PRQA S 2995 */ /* VL_Sd_LoopLogical */
    {
        uint8 optIndex = 0u;

        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        uint8 optType = sdMsg[optStartPosMap[optIndex] + SD_OPTION_TYPE_FILED_OFFSET];

        if (SD_IPV4_ENDPOINT_OPTION == optType)
        {
            Sd_IPv4OptionsType ipv4Opt;

            (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
            if ((SD_L4_PROTO_UDP == ipv4Opt.L4Proto) && (0UL == ipv4EndpUdpPos))
            {
                ipv4EndpUdpPos = optStartPosMap[optIndex];
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else if (SD_IPV6_ENDPOINT_OPTION == optType)
        {
            Sd_IPv6OptionsType ipv6Opt;

            (void)IStdLib_MemCpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
            if ((SD_L4_PROTO_UDP == ipv6Opt.L4Proto) && (0UL == ipv6EndpUdpPos))
            {
                ipv6EndpUdpPos = optStartPosMap[optIndex];
            }
        }
        else
        {
            /*Nothing to do here*/
        }
#endif /* STD_ON == SD_IPV6_ENABLE */

        if ((0UL != ipv4EndpUdpPos)
#if (STD_ON == SD_IPV6_ENABLE)
            || (0UL != ipv6EndpUdpPos)
#endif /* STD_ON == SD_IPV6_ENABLE */
        )
        {
            break;
        }
    }

    if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
    {
        Sd_IPv4OptionsType ipv4Opt;

        /* Fan out control(UDP,IPv4) */
        (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[ipv4EndpUdpPos], sizeof(Sd_IPv4OptionsType));
        ret = Sd_FanOutControlUdpHandle(
            instancePtr,
            serverServicePtr,
            eventHandlerPtr,
            &ipv4Opt,
            NULL_PTR,
            clientIpAddr,
            ttl,
            (uint8)type2Entry.Counter,
            applicationId);
    }
#if (STD_ON == SD_IPV6_ENABLE)
    else
    {
        Sd_IPv6OptionsType ipv6Opt;

        /* Fan out control(UDP,IPv6) */
        (void)IStdLib_MemCpy((uint8*)&ipv6Opt, &sdMsg[ipv6EndpUdpPos], sizeof(Sd_IPv6OptionsType));
        ret = Sd_FanOutControlUdpHandle(
            instancePtr,
            serverServicePtr,
            eventHandlerPtr,
            NULL_PTR,
            &ipv6Opt,
            clientIpAddr,
            ttl,
            (uint8)type2Entry.Counter,
            applicationId);
    }
#endif /* STD_ON == SD_IPV6_ENABLE */

    return ret;
}
#endif

/**
 * @brief              Deletes a subscriber from the TCP fan-out list.
 * @param[in]          serverServicePtr: Pointer to the server service type.
 * @param[in]          eventHandlerPtr: Pointer to the event handler type.
 * @param[in]          sockAddr: Socket address.
 * @param[in]          clientIpAddr: Client IP address.
 * @param[in]          counter: Counter value.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74568
 */
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
SD_LOCAL void Sd_FanoutTcpDeleteSubscriber(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    TcpIp_SockAddrType          sockAddr,
    TcpIp_SockAddrType          clientIpAddr,
    uint8                       counter,
    ApplicationType             applicationId)
/* PRQA S 6040 -- */
{
    /* StopSubscribeEventGroupEntry */

    Sd_EventHandlerRTType*        ehRTDataPtr = Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
    const Sd_SubscribeManageType* subscribeManagePtr =
        Sd_GetSubscribeManageObjAddr(ehRTDataPtr->TcpSubscribeList, sockAddr, clientIpAddr, counter);
    if (NULL_PTR != subscribeManagePtr)
    {
        /* Close Routing Path */
        const Sd_EventHandlerTcpType* eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
        if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
        {
            SoAd_RoutingGroupIdType routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->RoutingGroupId;
            (void)SoAd_DisableSpecificRouting(routingGroupId, subscribeManagePtr->SoConId);
        }

        /* StopSubscribeEventGroupEntry,Delete client from subscribe list */
        ehRTDataPtr->TcpSubsClientNum--;
        Sd_DeleteClientFromSubscribeList(
            serverServicePtr,
            eventHandlerPtr,
            &ehRTDataPtr->TcpSubscribeList,
            NULL_PTR,
            subscribeManagePtr->SoConId,
            counter,
            applicationId);
    }
}
#endif

/**
 * @brief              Updates a subscriber in the TCP fan-out list.
 * @param[in]          eventHandlerPtr: Pointer to the event handler type.
 * @param[in]          soConId: Socket connection ID.
 * @param[in]          subscribeManagePtr: Pointer to the subscribe manage type.
 * @param[in]          ehRTDataPtr: Pointer to the event handler runtime data.
 * @param[in]          ttl: Time-to-live value.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74567
 */
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
SD_LOCAL void Sd_FanoutTcpUpdateSubscriber(
    const Sd_EventHandlerType* eventHandlerPtr,
    SoAd_SoConIdType           soConId,
    Sd_SubscribeManageType*    subscribeManagePtr,
    Sd_EventHandlerRTType*     ehRTDataPtr,
    uint32                     ttl)
{
    if (soConId != subscribeManagePtr->SoConId)
    {
        /*The IP Address of client changed,should close socket connection used before */
        const Sd_EventHandlerTcpType* eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
        if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
        {
            SoAd_RoutingGroupIdType routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->RoutingGroupId;
            (void)SoAd_DisableSpecificRouting(routingGroupId, subscribeManagePtr->SoConId);
        }
        (void)SoAd_CloseSoCon(subscribeManagePtr->SoConId, TRUE);

        /*Update soCondId*/
        subscribeManagePtr->SoConId = soConId;
        ehRTDataPtr->SoConIdTcp     = soConId; /* Used for close Tcp routing */

        /* Open new socket connection */
        if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
        {
            SoAd_RoutingGroupIdType routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->RoutingGroupId;
            (void)SoAd_EnableSpecificRouting(routingGroupId, soConId);
        }
    }

    /* Update TTL */
    Sd_UpdateSubscribeClientTTL(subscribeManagePtr, ttl);
}
#endif

/**
 * @brief              Adds a subscriber to the TCP fan-out list.
 * @param[in]          eventHandlerPtr: Pointer to the event handler type.
 * @param[in]          soConId: Socket connection ID.
 * @param[in]          ehRTDataPtr: Pointer to the event handler runtime data.
 * @param[in]          sockAddr: Socket address.
 * @param[in]          clientIpAddr: Client IP address.
 * @param[in]          ttl: Time-to-live value.
 * @param[in]          counter: Counter value.
 * @param[in]          applicationId: Application ID.
 * @return             E_OK if the subscriber is added successfully, E_NOT_OK otherwise.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74566
 */
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
SD_LOCAL Std_ReturnType Sd_FanoutTcpAddSubscriber(
    const Sd_EventHandlerType* eventHandlerPtr,
    SoAd_SoConIdType           soConId,
    Sd_EventHandlerRTType*     ehRTDataPtr,
    TcpIp_SockAddrType         sockAddr,
    TcpIp_SockAddrType         clientIpAddr,
    uint32                     ttl,
    uint8                      counter,
    ApplicationType            applicationId)
/* PRQA S 6040 -- */
{
    /*Enable Routing */
    const Sd_EventHandlerTcpType* eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
    boolean                       isInitialEvent     = FALSE;

    if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
    {
        SoAd_RoutingGroupIdType routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->RoutingGroupId;
        (void)SoAd_EnableSpecificRouting(routingGroupId, soConId);
    }

    SoAd_RoutingGroupIdType triggerRoutingGroupId;
    if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventTriggeringRef))
    {
        triggerRoutingGroupId = eventHandlerTcpPtr->SdEventTriggeringRef->RoutingGroupId;
        /* PATCH for TC8: Field event transmit befor ACK */
        isInitialEvent = TRUE;
    }
    ehRTDataPtr->SoConIdTcp = soConId; /* Used also for close Tcp routing */

    /* Add client to subscribe list */
    Std_ReturnType ret = Sd_AddClientToSubscribeList(
        &ehRTDataPtr->TcpSubscribeList,
        soConId,
        sockAddr,
        clientIpAddr,
        ttl,
        counter,
        applicationId);
    if (E_OK == ret)
    {
        ehRTDataPtr->TcpSubsClientNum++;
        if (isInitialEvent)
        {
            /* PATCH for TC8: Field event transmit befor ACK */
            Sd_AddClientToTriggerTransmitList(
                &ehRTDataPtr->TcpTriggerTransmitList,
                triggerRoutingGroupId,
                soConId,
                applicationId);
        }
    }

    return ret;
}
#endif

/**
 * @brief              Fan out control TCP handle
 *                     [ipv4OptPtr] or [ipv6OptPtr] must have one equal NULL_PTR
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          eventHandlerPtr: Pointer for event handler.
 * @param[in]          ipv4OptPtr: Pointer for IPv4 option.
 * @param[in]          ipv6OptPtr: Pointer for IPv6 option.
 * @param[in]          clientIpAddr: IP address of the client.
 * @param[in]          ttl: Lifetime of the subscribe.
 * @param[in]          counter: The counter field for subscribe entry.
 * @param[in]          applicationId Application ID.
 * @return             E_OK: No error
 *                     E_NOT_OK: error,need response use Nack entry
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74132
 */
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
/* PRQA S 6040,6070 ++ */ /* VL_MTR_Sd_STPAR,VL_MTR_Sd_STCAL */
SD_LOCAL Std_ReturnType Sd_FanOutControlTcpHandle(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    const Sd_IPv4OptionsType*   ipv4OptPtr,
    const Sd_IPv6OptionsType*   ipv6OptPtr,
    TcpIp_SockAddrType          clientIpAddr,
    uint32                      ttl,
    uint8                       counter,
    ApplicationType             applicationId)
/* PRQA S 6040,6070 -- */
{
    TcpIp_SockAddrType sockAddr = {0};
    Std_ReturnType     ret      = E_NOT_OK;

    if (ttl > 0UL)
    {
        /* @req SWS_SD_00453 */
        uint16             soConNum      = serverServicePtr->SdServerServiceTcpRef->SoConNumInGroup;
        boolean            findSoConFlag = FALSE;
        SoAd_SoConIdType   soConId       = 0u;
        SoAd_SoConModeType soConMode     = SOAD_SOCON_OFFLINE;
        sint32             isSameIpAddr  = 1;

        for (uint16 loop = 0u; loop < soConNum; loop++)
        {
            soConId = serverServicePtr->SdServerServiceTcpRef->SocketIdList[loop];
            if (NULL_PTR != ipv4OptPtr)
            {
                sockAddr.domain = TCPIP_AF_INET;
                (void)SoAd_GetRemoteAddr(soConId, &sockAddr);
                isSameIpAddr = IStdLib_MemCmp((uint8*)sockAddr.addr, ipv4OptPtr->IPv4Address, SD_IPV4_ADDRESS_LEN);
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else if (NULL_PTR != ipv6OptPtr)
            {
                sockAddr.domain = TCPIP_AF_INET6;
                (void)SoAd_GetRemoteAddr(soConId, &sockAddr);
                isSameIpAddr = IStdLib_MemCmp((uint8*)sockAddr.addr, ipv6OptPtr->IPv6Address, SD_IPV6_ADDRESS_LEN);
            }
#endif
            else
            {
                /* Nothing to do here */
            }
            if ((0L == isSameIpAddr)
                && (((NULL_PTR != ipv4OptPtr) && (sockAddr.port == Sd_NtoHs(ipv4OptPtr->PortNumber)))
#if (STD_ON == SD_IPV6_ENABLE)
                    || ((NULL_PTR != ipv6OptPtr) && (sockAddr.port == Sd_NtoHs(ipv6OptPtr->PortNumber)))
#endif
                        ))
            {
                /* Find Relevant TCP Socket Connection*/
                findSoConFlag = TRUE;
                SoAd_GetSoConMode(soConId, &soConMode);
                break;
            }
        }

        /*@req <SWS_Sd_00760> */
        TcpIp_ReturnType isConnectionReady = SoAd_IsConnectionReady(soConId, (const TcpIp_SockAddrType*)&sockAddr);
        if (findSoConFlag && (SOAD_SOCON_ONLINE == soConMode) && (TCPIP_E_OK == isConnectionReady))
        {
            Sd_EventHandlerRTType*  ehRTDataPtr = Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
            Sd_SubscribeManageType* subscribeManagePtr =
                Sd_GetSubscribeManageObjAddr(ehRTDataPtr->TcpSubscribeList, sockAddr, clientIpAddr, counter);
            if (NULL_PTR == subscribeManagePtr)
            {
                if (ehRTDataPtr->TcpSubsClientNum >= SD_MAX_SUBSCRIBE_CLIENT_NUM)
                {
                    return ret;
                }

                ret = Sd_FanoutTcpAddSubscriber(
                    eventHandlerPtr,
                    soConId,
                    ehRTDataPtr,
                    sockAddr,
                    clientIpAddr,
                    ttl,
                    counter,
                    applicationId);
            }
            else
            {
                Sd_FanoutTcpUpdateSubscriber(eventHandlerPtr, soConId, subscribeManagePtr, ehRTDataPtr, ttl);
                ret = E_OK;
            }
        }
        else
        {
            /* TCP connection not connected, Response with Nack */
            ret = E_NOT_OK;
        }
    }
    else
    {
        if (NULL_PTR != ipv4OptPtr)
        {
            sockAddr.domain = TCPIP_AF_INET;
            sockAddr.port   = Sd_NtoHs(ipv4OptPtr->PortNumber);
            (void)IStdLib_MemCpy((uint8*)sockAddr.addr, (const uint8*)ipv4OptPtr->IPv4Address, SD_IPV4_ADDRESS_LEN);
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else
        {
            sockAddr.domain = TCPIP_AF_INET6;
            sockAddr.port   = Sd_NtoHs(ipv6OptPtr->PortNumber);
            (void)IStdLib_MemCpy((uint8*)sockAddr.addr, (const uint8*)ipv6OptPtr->IPv6Address, SD_IPV6_ADDRESS_LEN);
        }
#endif
        Sd_FanoutTcpDeleteSubscriber(serverServicePtr, eventHandlerPtr, sockAddr, clientIpAddr, counter, applicationId);
        ret = E_OK;
    }

    return ret;
}
#endif

/**
 * @brief              Updates the UDP subscribe list for the event handler.
 * @param[in]          eventHandlerPtr: Pointer to the event handler type.
 * @param[in]          subscribeManagePtr: Pointer to the subscribe manage type.
 * @param[in]          ehRTDataPtr: Pointer to the event handler runtime data.
 * @param[in]          serverServicePtr: Pointer to the server service type.
 * @param[in]          ttl: Time-to-live value.
 * @param[in]          assignedSoConId: Assigned socket connection ID.
 * @param[in]          soConId: Socket connection ID.
 * @param[in]          isMulticastSocket: Flag indicating if the socket is multicast.
 * @param[in]          counter: Counter value.
 * @param[in]          applicationId: Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74565
 */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
/* PRQA S 6040,6070 ++ */ /* VL_MTR_Sd_STPAR,VL_MTR_Sd_STCAL */
SD_LOCAL void Sd_UpdateUdpSubscribeList(
    const Sd_EventHandlerType*  eventHandlerPtr,
    Sd_SubscribeManageType*     subscribeManagePtr,
    Sd_EventHandlerRTType*      ehRTDataPtr,
    const Sd_ServerServiceType* serverServicePtr,
    uint32                      ttl,
    SoAd_SoConIdType            assignedSoConId,
    SoAd_SoConIdType            soConId,
    boolean                     isMulticastSocket,
    uint8                       counter,
    ApplicationType             applicationId)
/* PRQA S 6040,6070 -- */
{
    if (ttl > 0UL)
    {
        if (assignedSoConId != subscribeManagePtr->SoConId)
        {
            const Sd_EventHandlerUdpType* eventHandlerUdpPtr = eventHandlerPtr->SdEventHandlerUdp;
            SoAd_RoutingGroupIdType       routingGroupId     = eventHandlerUdpPtr->SdEventActivationRef->RoutingGroupId;

            /* The IP Address of client changed,should close socket connection used before */
            (void)SoAd_DisableSpecificRouting(routingGroupId, subscribeManagePtr->SoConId);
            (void)SoAd_CloseSoCon(subscribeManagePtr->SoConId, TRUE);

            if ((ehRTDataPtr->UdpSubsEndPointNum < eventHandlerPtr->SdEventHandlerMulticastThreshold)
                || (0u == eventHandlerPtr->SdEventHandlerMulticastThreshold))
            {
                (void)SoAd_EnableSpecificRouting(routingGroupId, assignedSoConId);
                if (NULL_PTR != eventHandlerUdpPtr->SdEventTriggeringRef)
                {
                    routingGroupId = eventHandlerUdpPtr->SdEventTriggeringRef->RoutingGroupId;
                    /* PATCH for TC8: Field event transmit befor ACK */
                    Sd_AddClientToTriggerTransmitList(
                        &ehRTDataPtr->UdpTriggerTransmitList,
                        routingGroupId,
                        soConId,
                        applicationId);
                }
            }

            /* Update soCondId */
            if (isMulticastSocket)
            {
                subscribeManagePtr->SoConId = assignedSoConId;
            }
            else
            {
                /*@req <SWS_Sd_00753> */
                /* All nodes using the same multicast endpoint need to update the SoCon. */
                Sd_UpdateSubscribeSoConId(ehRTDataPtr->UdpSubscribeList, subscribeManagePtr->SoConId, assignedSoConId);
            }

            /* Open new soCon */
            (void)SoAd_OpenSoCon(assignedSoConId);
        }

        /* Update TTL */
        Sd_UpdateSubscribeClientTTL(subscribeManagePtr, ttl);
    }
    else
    {
        ehRTDataPtr->UdpSubsClientNum--;
        uint8 clientNum = Sd_GetClientNumFromSubscribeList(
            ehRTDataPtr->UdpSubscribeList,
            NULL_PTR,
            subscribeManagePtr->SoConId,
            SD_SUBSCRIBE_COUNTER_FIELD_RESERVED);
        if (clientNum == 1u)
        {
            /*@req <SWS_SD_00455> */
            /* StopSubscribeEventGroupEntry,Delete client from subscribe list */
            /* Fan out control*/
            Sd_FanOutMulticastUnicastCtrlDelClient(eventHandlerPtr, ehRTDataPtr, subscribeManagePtr->SoConId);
            ehRTDataPtr->UdpSubsEndPointNum--;
        }

        /* Delete client*/
        Sd_DeleteClientFromSubscribeList(
            serverServicePtr,
            eventHandlerPtr,
            &ehRTDataPtr->UdpSubscribeList,
            &subscribeManagePtr->ClientIpAddr,
            subscribeManagePtr->SoConId,
            counter,
            applicationId);
    }
}
#endif

/**
 * @brief              Fan out control UDP handle
 *                     [ipv4OptPtr] or [ipv6OptPtr] must have one equal NULL_PTR
 * @param[in]          instancePtr: Pointer for instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          eventHandlerPtr: Pointer for event handler.
 * @param[in]          ipv4OptPtr: Pointer for IPv4 option.
 * @param[in]          ipv6OptPtr: Pointer for IPv6 option.
 * @param[in]          clientIpAddr: IP address of the client.
 * @param[in]          ttl: Lifetime of the subscribe.
 * @param[in]          counter: The counter field for subscribe entry.
 * @param[in]          applicationId Application ID.
 * @return             E_OK: No error
 *                     E_NOT_OK: error,need response use Nack entry
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74133
 */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
/* PRQA S 2889,6040,6060,6070 ++ */ /* VL_QAC_MultiReturn,VL_MTR_Sd_STPAR,VL_MTR_Sd_STM19,VL_MTR_Sd_STCAL */
SD_LOCAL Std_ReturnType Sd_FanOutControlUdpHandle(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType*  eventHandlerPtr,
    const Sd_IPv4OptionsType*   ipv4OptPtr,
    const Sd_IPv6OptionsType*   ipv6OptPtr,
    TcpIp_SockAddrType          clientIpAddr,
    uint32                      ttl,
    uint8                       counter,
    ApplicationType             applicationId)
{
    TcpIp_SockAddrType sockAddr = {0};
    /*@req <SWS_SD_00454> */
    Sd_EventHandlerRTType* ehRTDataPtr       = Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
    boolean                isMulticastSocket = FALSE;

    if (NULL_PTR != ipv4OptPtr)
    {
        sockAddr.domain = TCPIP_AF_INET;
        sockAddr.port   = Sd_NtoHs(ipv4OptPtr->PortNumber);
        (void)IStdLib_MemCpy((uint8*)sockAddr.addr, (const uint8*)ipv4OptPtr->IPv4Address, SD_IPV4_ADDRESS_LEN);
        if (ipv4OptPtr->L4Proto == SD_IPV4_MULTICAST_OPTION)
        {
            isMulticastSocket = TRUE;
        }
    }
#if (STD_ON == SD_IPV6_ENABLE)
    else
    {
        sockAddr.domain = TCPIP_AF_INET6;
        sockAddr.port   = Sd_NtoHs(ipv6OptPtr->PortNumber);
        (void)IStdLib_MemCpy((uint8*)sockAddr.addr, (const uint8*)ipv6OptPtr->IPv6Address, SD_IPV6_ADDRESS_LEN);
        if (ipv6OptPtr->L4Proto == SD_IPV6_MULTICAST_OPTION)
        {
            isMulticastSocket = TRUE;
        }
    }
#endif

    /* Any socket connection that is part of the SoAdSocketConnectionGroup*/
    SoAd_SoConIdType soConId = serverServicePtr->SdServerServiceUdpRef->SocketIdList[0];
    SoAd_SoConIdType assignedSoConId;
    Std_ReturnType   ret = SoAd_SetUniqueRemoteAddr(soConId, &sockAddr, &assignedSoConId);

    /*@req <SWS_Sd_00760> */
    if (SoAd_IsConnectionReady(assignedSoConId, &sockAddr) != TCPIP_E_OK)
    {
        Sd_ResetSoConToWildcard(serverServicePtr, assignedSoConId, FALSE);

#if (SD_DEM_USED == STD_ON)
        /* If no Wildcard Socket Connection is left, SD_E_OUT_OF_RES shall be reported.*/
        const Sd_DemEventParameterType* demEventPtr;
        demEventPtr = instancePtr->SdInstanceDemEventParameterRefs->SD_E_OUT_OF_RES;
        if (NULL_PTR != demEventPtr)
        {
            (void)Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
        }
#endif
        return E_NOT_OK;
    }

    Sd_SubscribeManageType* subscribeManagePtr =
        Sd_GetSubscribeManageObjAddr(ehRTDataPtr->UdpSubscribeList, sockAddr, clientIpAddr, counter);
    if (NULL_PTR == subscribeManagePtr)
    {
        if ((ttl > 0UL) && (ehRTDataPtr->UdpSubsClientNum < SD_MAX_SUBSCRIBE_CLIENT_NUM))
        {
            /* Add client to subscribe list */
            ret = Sd_AddClientToSubscribeList(
                &ehRTDataPtr->UdpSubscribeList,
                assignedSoConId,
                sockAddr,
                clientIpAddr,
                ttl,
                counter,
                applicationId);

            if (E_NOT_OK != ret)
            {
                ehRTDataPtr->UdpSubsClientNum++;
                /*@req <SWS_SD_00455> */
                uint8 clientNum = Sd_GetClientNumFromSubscribeList(
                    ehRTDataPtr->UdpSubscribeList,
                    NULL_PTR,
                    assignedSoConId,
                    SD_SUBSCRIBE_COUNTER_FIELD_RESERVED);
                if (clientNum == 1u)
                {
                    ehRTDataPtr->UdpSubsEndPointNum++;
                }
                Sd_FanOutMulticastUnicastCtrlAddClient(eventHandlerPtr, ehRTDataPtr, assignedSoConId, applicationId);
            }
        }
        else
        {
            /* StopSubscribeEventGroupEntry,not in SubscribeList,Ignore */
        }
    }
    else
    {
        Sd_UpdateUdpSubscribeList(
            eventHandlerPtr,
            subscribeManagePtr,
            ehRTDataPtr,
            serverServicePtr,
            ttl,
            assignedSoConId,
            soConId,
            isMulticastSocket,
            counter,
            applicationId);
    }
    SD_UNUSED(instancePtr);
    SD_UNUSED(ipv6OptPtr);

    return E_OK;
}
/* PRQA S 2889,6040,6060,6070 -- */
#endif

/**
 * @brief           Updates the Socket Connection ID for subscribed clients.
 * @param[in]       headerPtr Pointer to the header of the subscribe manage list.
 * @param[in]       soConId Current Socket Connection ID.
 * @param[in]       assignSoConId New Socket Connection ID to assign.
 * @reentrant TRUE
 * @synchronous TRUE
 * @trace              CPD-74134
 */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
SD_LOCAL void Sd_UpdateSubscribeSoConId(
    Sd_SubscribeManageType* headerPtr,
    SoAd_SoConIdType        soConId,
    SoAd_SoConIdType        assignSoConId)
{
    Sd_SubscribeManageType* ptr = headerPtr;

    while (ptr != NULL_PTR)
    {
        if (ptr->SoConId == soConId)
        {
            ptr->SoConId = assignSoConId;
        }

        ptr = ptr->Next;
    }
}
#endif

/**
 * @brief              Multicast and Unicast switch control in Fan out control(add Client)
 * @param[in]          eventHandlerPtr: Pointer for event handler.
 * @param[inout]       ehRTDataPtr: Pointer for event handle runtime data.
 * @param[in]          assignedSoConId: Assigned socket connection id.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74135
 */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
SD_LOCAL void Sd_FanOutMulticastUnicastCtrlAddClient(
    const Sd_EventHandlerType* eventHandlerPtr,
    Sd_EventHandlerRTType*     ehRTDataPtr,
    SoAd_SoConIdType           assignedSoConId,
    ApplicationType            applicationId)
{
    /*@req <SWS_SD_00454>,<SWS_SD_00455> */
    /* client was not subscribe before receiving the subscribe eventgroup entry */
    const Sd_EventHandlerUdpType* eventHandlerUdpPtr = eventHandlerPtr->SdEventHandlerUdp;
    if ((NULL_PTR != eventHandlerUdpPtr) && (NULL_PTR != eventHandlerUdpPtr->SdEventActivationRef))
    {
        SoAd_RoutingGroupIdType routingGroupId = eventHandlerUdpPtr->SdEventActivationRef->RoutingGroupId;
#if (SD_EVENT_HANDLER_MULICAST_USED == STD_ON)
        uint16 subscribedClientNum = ehRTDataPtr->UdpSubsEndPointNum;
        if ((0u == eventHandlerPtr->SdEventHandlerMulticastThreshold)
            || (subscribedClientNum < eventHandlerPtr->SdEventHandlerMulticastThreshold))
        {
            /* Setup Unicast for this time subscribed client */
            (void)SoAd_EnableSpecificRouting(routingGroupId, assignedSoConId);
        }
        else if (subscribedClientNum == eventHandlerPtr->SdEventHandlerMulticastThreshold)
        {
            /* Setup Multicast for all subscribed clients */
            const Sd_EventHandlerMulticastType* eventHandlerMulticastPtr = eventHandlerPtr->SdEventHandlerMulticast;
            if ((NULL_PTR != eventHandlerMulticastPtr) && (NULL_PTR != eventHandlerMulticastPtr->SdEventActivationRef)
                && (NULL_PTR != eventHandlerMulticastPtr->SdMulticastEventSoConRef))
            {
                SoAd_RoutingGroupIdType multicastRoutingGroupId =
                    eventHandlerMulticastPtr->SdEventActivationRef->RoutingGroupId;
                SoAd_SoConIdType multiSoConId = eventHandlerMulticastPtr->SdMulticastEventSoConRef->SoConId;
                (void)SoAd_EnableSpecificRouting(multicastRoutingGroupId, multiSoConId);
            }

            /* Disable Unicast for all subscribed clients */
            Sd_DisableUnicastForSubscribedClient(ehRTDataPtr->UdpSubscribeList, routingGroupId);
        }
        else
        {
            /*Nothing to do here*/
        }
#else
        /* Setup Unicast for this time subscribed client */
        (void)SoAd_EnableSpecificRouting(routingGroupId, assignedSoConId);
#endif
    }

    if ((NULL_PTR != eventHandlerUdpPtr) && (NULL_PTR != eventHandlerUdpPtr->SdEventTriggeringRef))
    {
        SoAd_RoutingGroupIdType routingGroupId = eventHandlerUdpPtr->SdEventTriggeringRef->RoutingGroupId;
        /* PATCH for TC8: Field event transmit befor ACK */
        Sd_AddClientToTriggerTransmitList(
            &ehRTDataPtr->UdpTriggerTransmitList,
            routingGroupId,
            assignedSoConId,
            applicationId);
    }
}
#endif

/**
 * @brief              Multicast and Unicast switch control in Fan out control(Delete Client)
 * @param[in]          eventHandlerPtr: Pointer for event handler.
 * @param[inout]       ehRTDataPtr: Pointer for event handle runtime data.
 * @param[in]          soConId: socket connection id.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74136
 */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
SD_LOCAL void Sd_FanOutMulticastUnicastCtrlDelClient(
    const Sd_EventHandlerType*   eventHandlerPtr,
    const Sd_EventHandlerRTType* ehRTDataPtr,
    SoAd_SoConIdType             soConId)
{
    const Sd_EventHandlerUdpType* eventHandlerUdpPtr = eventHandlerPtr->SdEventHandlerUdp;
    if ((NULL_PTR != eventHandlerUdpPtr) && (NULL_PTR != eventHandlerUdpPtr->SdEventActivationRef))
    {
        SoAd_RoutingGroupIdType routingGroupId = eventHandlerUdpPtr->SdEventActivationRef->RoutingGroupId;
#if (SD_EVENT_HANDLER_MULICAST_USED == STD_ON)
        /*@req <SWS_SD_00455> */
        uint16 subscribedClientNum = ehRTDataPtr->UdpSubsEndPointNum;
        if ((0u == eventHandlerPtr->SdEventHandlerMulticastThreshold)
            || (subscribedClientNum < eventHandlerPtr->SdEventHandlerMulticastThreshold))
        {
            /* Disable this time will be deleted client's Routing Path */
            (void)SoAd_DisableSpecificRouting(routingGroupId, soConId);
        }

        else if (subscribedClientNum == eventHandlerPtr->SdEventHandlerMulticastThreshold)
        {
            /* Disable Multicast */
            const Sd_EventHandlerMulticastType* eventHandlerMulticastPtr = eventHandlerPtr->SdEventHandlerMulticast;
            if ((NULL_PTR != eventHandlerMulticastPtr) && (NULL_PTR != eventHandlerMulticastPtr->SdEventActivationRef)
                && (NULL_PTR != eventHandlerMulticastPtr->SdMulticastEventSoConRef))
            {
                SoAd_RoutingGroupIdType multicastRoutingGroupId =
                    eventHandlerMulticastPtr->SdEventActivationRef->RoutingGroupId;
                SoAd_SoConIdType multiSoConId = eventHandlerMulticastPtr->SdMulticastEventSoConRef->SoConId;
                (void)SoAd_DisableSpecificRouting(multicastRoutingGroupId, multiSoConId);
            }

            /* Setup Unicast for subscribed clients */
            Sd_EnableUnicastForSubscribedClient(ehRTDataPtr->UdpSubscribeList, routingGroupId);

            /* Disable this time will be deleted client's Routing Path */
            (void)SoAd_DisableSpecificRouting(routingGroupId, soConId);
        }
        else
        {
            /*Nothing to do here*/
        }
#else
        /* Disable this time will be deleted client's Routing Path */
        (void)SoAd_DisableSpecificRouting(routingGroupId, soConId);
#endif
    }
    SD_UNUSED(ehRTDataPtr);
}
#endif
#endif

/**
 * @brief              Save SoConId that match [remoteAddrPtr]
 *                     if not opend SoConId before,Open it
 *                     if get SoConId from SoAd not equal before,close before one,then open the new one
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          remoteAddrPtr: Pointer for remote address buffer.
 * @param[in]          isUseTcp: Whether it is use tcp socket.
 * @param[in]          soConPtr: Pointer for socket connection.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74137
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL void Sd_ClientSaveSoConId(
    const Sd_ClientServiceType* clientServicePtr,
    const TcpIp_SockAddrType*   remoteAddrPtr,
    boolean                     isUseTcp,
    Sd_SoConManageType*         soConPtr)
{
    SoAd_SoConIdType soConId = 0u;
    Std_ReturnType   ret;

#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
    if (isUseTcp)
    {
        soConId = clientServicePtr->SdClientServiceTcpRef->SocketIdList[0];
    }
    else
#endif
        if (!isUseTcp)
    {
#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
        if (clientServicePtr->SdClientServiceUdpRef != NULL_PTR)
        {
            soConId = clientServicePtr->SdClientServiceUdpRef->SocketIdList[0];
        }
        else
#endif
#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
            if (clientServicePtr->SdClientServiceMulticastRef != NULL_PTR)
        {
            soConId = clientServicePtr->SdClientServiceMulticastRef->SocketIdList[0];
        }
        else
#endif
        {
            /* Nothing to do */
        }
    }

    ret = SoAd_SetUniqueRemoteAddr(soConId, remoteAddrPtr, &soConId);
    if (E_OK == ret)
    {
        SoAd_SoConModeType soConMode;
        SoAd_GetSoConMode(soConId, &soConMode);
        if (!soConPtr->IsSoConOpened || (SOAD_SOCON_ONLINE != soConMode) || (soConId != soConPtr->SoConId))
        {
            if (soConId != soConPtr->SoConId)
            {
                Sd_ClientServiceCloseSoCon(soConPtr);
            }

            /*@req <SWS_SD_00604> */
            ret = SoAd_OpenSoCon(soConId);
            if (E_OK == ret)
            {
                soConPtr->IsOptRcved    = TRUE;
                soConPtr->IsSoConOpened = TRUE;
                soConPtr->SoConId       = soConId;

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
                Sd_ClientServiceResetAllCEgSubsEntrySendFlag(clientServicePtr, isUseTcp, (!isUseTcp));
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
            }
        }
    }
}

/**
 * @brief              Save SoConId that match [remoteAddrPtr]
 *                     if not opend SoConId before,Open it
 *                     if get SoConId from SoAd not equal before,close before one,then open the new one
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          consumedEventGroupPtr: Pointer for consumed event group.
 * @param[in]          multicastAddrPtr: Pointer for multicast address buffer.
 * @param[in]          serverAddrPtr: Pointer for server address buffer.
 * @param[inout]       soConPtr: Pointer for socket connection buffer.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74138
 */
#if (SD_CONSUMED_EVENTGROUP_NUM > 0 && SD_CONSUMED_EVENT_MULICAST_USED == STD_ON)
SD_LOCAL void Sd_ClientSaveMulticastSoConId(
    const Sd_InstanceType*           instancePtr,
    const Sd_ConsumedEventGroupType* consumedEventGroupPtr,
    const TcpIp_SockAddrType*        multicastAddrPtr,
    const TcpIp_SockAddrType*        serverAddrPtr,
    Sd_SoConManageType*              soConPtr)
{
    uint8          netmask;
    Std_ReturnType ret = E_NOT_OK;
    /* @req SWS_SD_00377 */
    const Sd_SoAdSoConGroupType* ceMulticastGroupRef = consumedEventGroupPtr->SdConsumedEventGroupMulticastGroupRef;
    SoAd_SoConIdType             soConId;

    if (NULL_PTR != ceMulticastGroupRef)
    {
        soConId = ceMulticastGroupRef->SocketIdList[0];
#if (STD_ON == SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON)
        ret = SoAd_SetUniqueRemoteAddr(soConId, serverAddrPtr, &soConId);
#else
        TcpIp_SockAddrType remoteAddr;
        remoteAddr.domain      = instancePtr->SdAddrFamily;
        uint16 soConNumInGroup = ceMulticastGroupRef->SoConNumInGroup;
        for (uint16 index = 0u; index < soConNumInGroup; index++)
        {
            soConId = ceMulticastGroupRef->SocketIdList[index];
            ret     = SoAd_GetRemoteAddr(soConId, &remoteAddr);
            if (E_OK == ret)
            {
                if ((TCPIP_IS_IPADDR_ANY(remoteAddr)) && (TCPIP_PORT_ANY == remoteAddr.port))
                {
                    break;
                }
            }
        }
        if ((index < soConNumInGroup) && (E_OK == ret))
        {
            ret = E_OK;
        }
        else
        {
            ret = E_NOT_OK;
        }
#endif /* STD_ON == SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON */
    }
    if (E_OK == ret)
    {
        if (!soConPtr->IsSoConOpened || (soConId != soConPtr->SoConId))
        {
            Sd_ClientServiceCloseSoCon(soConPtr);

            /*@req <SWS_SD_00604> */
            ret = SoAd_OpenSoCon(soConId);
            if (E_OK == ret)
            {
                soConPtr->IsOptRcved    = TRUE;
                soConPtr->IsSoConOpened = TRUE;
                soConPtr->SoConId       = soConId;

                /*@req <SWS_SD_00377> */
                TcpIp_SockAddrType localAddr;
                TcpIp_SockAddrType defaultRouter;
                localAddr.domain               = instancePtr->SdAddrFamily;
                defaultRouter.domain           = instancePtr->SdAddrFamily;
                SoAd_SoConIdType txPduSoConId  = instancePtr->SdInstanceTxPduSoConId;
                Std_ReturnType retGetLocalAddr = SoAd_GetLocalAddr(txPduSoConId, &localAddr, &netmask, &defaultRouter);
                if (E_OK == retGetLocalAddr)
                {
                    (void)SoAd_RequestIpAddrAssignment(
                        soConId,
                        TCPIP_IPADDR_ASSIGNMENT_STATIC,
                        multicastAddrPtr,
                        netmask,
                        &defaultRouter);
                }

                /*@req <SWS_SD_00704> */
                if (NULL_PTR != consumedEventGroupPtr->SdConsumedEventGroupMulticastActivationRef)
                {
                    SoAd_RoutingGroupIdType routingGroupId =
                        consumedEventGroupPtr->SdConsumedEventGroupMulticastActivationRef->RoutingGroupId;
                    (void)SoAd_EnableSpecificRouting(routingGroupId, soConId);
                }
            }
        }
    }
    else
    {
/* @req SWS_SD_00377 */
/* No Wildcard Socket Connection is left,Reporte SD_E_OUT_OF_RES to DEM */
#if (SD_DEM_USED == STD_ON)
        const Sd_DemEventParameterType* demEventPtr;
        demEventPtr = instancePtr->SdInstanceDemEventParameterRefs->SD_E_OUT_OF_RES;
        if (NULL_PTR != demEventPtr)
        {
            (void)Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
        }
#endif
    }
}
#endif

/**
 * @brief              Handle Client socket connection based on option information
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          sdMsg: Received Sd Message
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @param[inout]       cRtDataPtr: Pointer for client service runtime data.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74139
 */
/* PRQA S 6030,6040 ++ */ /* VL_MTR_Sd_STMIF,VL_MTR_Sd_STPAR */
SD_LOCAL void Sd_ClientHandleSoConBasedOptionInfo(
    const Sd_InstanceType*      instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const uint8*                sdMsg,
    uint16                      entryPos,
    const uint16                optStartPosMap[],
    Sd_ClientServiceRTType*     cRtDataPtr)
/* PRQA S 6030,6040 -- */
{
    Sd_Type1EntryType type1Entry;

    (void)IStdLib_MemCpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    uint8 idx1stOptions = type1Entry.Index1stOptions;
    uint8 numOpt1       = type1Entry.NumOfOpt1;
    uint8 idx2ndOptions = type1Entry.Index2ndOptions;
    uint8 numOpt2       = type1Entry.NumOfOpt2;
    uint8 optNumSum     = numOpt1 + numOpt2;
    uint8 optIndex;

    for (uint8 index = 0u; index < optNumSum; index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        uint8 optType = sdMsg[optStartPosMap[optIndex] + SD_OPTION_TYPE_FILED_OFFSET];
        if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
        {
            if (SD_IPV4_ENDPOINT_OPTION == optType)
            {
                Sd_IPv4OptionsType ipv4Opt;
                TcpIp_SockAddrType remoteAddr;

                (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
                if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && (SD_L4_PROTO_TCP == ipv4Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET;
                    remoteAddr.port   = Sd_NtoHs(ipv4Opt.PortNumber);
                    (void)
                        IStdLib_MemCpy((uint8*)remoteAddr.addr, (const uint8*)ipv4Opt.IPv4Address, SD_IPV4_ADDRESS_LEN);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        TRUE,
                        &cRtDataPtr->TcpSoCon);

                    cRtDataPtr->RxTcpEndpointFlag = TRUE;
                }
                else
#endif
#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
                    if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && (SD_L4_PROTO_UDP == ipv4Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET;
                    remoteAddr.port   = Sd_NtoHs(ipv4Opt.PortNumber);
                    (void)
                        IStdLib_MemCpy((uint8*)remoteAddr.addr, (const uint8*)ipv4Opt.IPv4Address, SD_IPV4_ADDRESS_LEN);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        FALSE,
                        &cRtDataPtr->UdpSoCon);
                    /* Save server address for consumedEventgroup receive multicast message */
                    (void)IStdLib_MemCpy(&cRtDataPtr->ServerUdpAddr, &remoteAddr, sizeof(remoteAddr));

                    cRtDataPtr->RxUdpEndpointFlag = TRUE;
                }
                else
#endif
#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
                    if ((NULL_PTR != clientServicePtr->SdClientServiceMulticastRef)
                        && (SD_L4_PROTO_UDP == ipv4Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET;
                    remoteAddr.port   = Sd_NtoHs(ipv4Opt.PortNumber);
                    (void)
                        IStdLib_MemCpy((uint8*)remoteAddr.addr, (const uint8*)ipv4Opt.IPv4Address, SD_IPV4_ADDRESS_LEN);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        FALSE,
                        &cRtDataPtr->MulitcastSoCon);
                    /* Save server address for consumedEventgroup receive multicast message */
                    (void)IStdLib_MemCpy(&cRtDataPtr->ServerUdpAddr, &remoteAddr, sizeof(remoteAddr));

                    cRtDataPtr->RxMulitcastEndpointFlag = TRUE;
                }
                else
#endif
                {
                    /*Nothing to do here*/
                }
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else
        {
            if (SD_IPV6_ENDPOINT_OPTION == optType)
            {
                Sd_IPv6OptionsType ipv6Opt;
                TcpIp_SockAddrType remoteAddr;

                (void)IStdLib_MemCpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
                if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && (SD_L4_PROTO_TCP == ipv6Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET6;
                    remoteAddr.port   = Sd_NtoHs(ipv6Opt.PortNumber);
                    (void)IStdLib_MemCpy((uint8*)remoteAddr.addr, (const uint8*)ipv6Opt.IPv6Address, 16u);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        TRUE,
                        &cRtDataPtr->TcpSoCon);

                    cRtDataPtr->RxTcpEndpointFlag = TRUE;
                }
                else if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && (SD_L4_PROTO_UDP == ipv6Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET6;
                    remoteAddr.port   = Sd_NtoHs(ipv6Opt.PortNumber);
                    (void)IStdLib_MemCpy((uint8*)remoteAddr.addr, (const uint8*)ipv6Opt.IPv6Address, 16);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        FALSE,
                        &cRtDataPtr->UdpSoCon);
                    /* Save server address for consumedEventgroup receive multicast message */
                    (void)IStdLib_MemCpy(&cRtDataPtr->ServerUdpAddr, &remoteAddr, sizeof(remoteAddr));

                    cRtDataPtr->RxUdpEndpointFlag = TRUE;
                }
                else
                {
                    /*Nothing to do here*/
                }
            }
        }
#endif /* STD_ON == SD_IPV6_ENABLE */
    }
}

SD_LOCAL void
    Sd_ClientTimerExpireHandler(const Sd_ClientServiceType* clientServicePtr, Sd_ClientServiceRTType* cRtDataPtr)
{
    /*@req <SWS_SD_00600> */
    /* Enter the Initial Wait Phase and indicate the state SD_CLIENT_SERVICE_DOWN
     * to the BswM by calling the API BswM_Sd_ClientServiceCurrentState().*/
    Sd_ClientServiceInitialWaitEnter(clientServicePtr, cRtDataPtr);

#if (SD_BSWM_USED == STD_ON)
    BswM_Sd_ClientServiceCurrentState(clientServicePtr->SdClientServiceHandleId, SD_CLIENT_SERVICE_DOWN);
#endif
    cRtDataPtr->CurState = SD_CLIENT_SERVICE_DOWN;

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    /* All subscribed Eventgroups of this Client Service shall expired in this instance
     * (stop TTL timer) and the expiration shall be handled as describe in SWS_SD_00601.*/
    const Sd_ConsumedEventGroupType* eventGroupPtr = &clientServicePtr->SdConsumedEventGroup[0];
    for (uint16 eventGroupIdx = 0u; eventGroupIdx < clientServicePtr->ConsumedEventGroupNum; eventGroupIdx++)
    {
        Sd_ConsumedEventGroupRTType* consumedEgRTPtr =
            Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
        if (consumedEgRTPtr->Ttl.IsValidForever || (consumedEgRTPtr->Ttl.ttlms > 0ULL))
        {
            Sd_SetTTL(&consumedEgRTPtr->Ttl, 0u);
/*@req <SWS_SD_00601> */
#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_ConsumedEventGroupCurrentState(
                eventGroupPtr->SdConsumedEventGroupHandleId,
                SD_CONSUMED_EVENTGROUP_DOWN);
#endif
            consumedEgRTPtr->CurState = SD_CONSUMED_EVENTGROUP_DOWN;
        }

        eventGroupPtr++;
    }
#endif
    /* When left MainPhase,Close all Routing and SoCon opened before */
    Sd_ClientServiceDisableAllRoutingOfClient(clientServicePtr, cRtDataPtr);
    Sd_ClientServiceCloseAllSoCon(clientServicePtr, cRtDataPtr);
}

SD_LOCAL void Sd_ConsumedEventGroupTimerHandler(
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const Sd_ClientServiceRTType*    cRtDataPtr)
{
    Sd_ConsumedEventGroupRTType* consumedEgRTPtr =
        Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
    if (consumedEgRTPtr->Ttl.ttlms > 0ULL)
    {
        consumedEgRTPtr->Ttl.ttlms--;
        if (0ULL == consumedEgRTPtr->Ttl.ttlms)
        {
/*@req <SWS_SD_00601> */
#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_ConsumedEventGroupCurrentState(
                eventGroupPtr->SdConsumedEventGroupHandleId,
                SD_CONSUMED_EVENTGROUP_DOWN);
#endif
            consumedEgRTPtr->CurState = SD_CONSUMED_EVENTGROUP_DOWN;

            /* Close Routing of this ConsumedEventgroup */
            Sd_ClientServiceDisableAllConsumedEventgroupRouting(eventGroupPtr, cRtDataPtr);
        }
    }
}

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
SD_LOCAL void Sd_ConsumedEventGroupRetryStatusHandler(const Sd_ConsumedEventGroupType* eventGroupPtr)
{
    Sd_ConsumedEventGroupRTType* consumedEgRTPtr =
        Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
    const Sd_ClientTimerType* consumedEgTimerPtr = eventGroupPtr->SdConsumedEventGroupTimerRef;
    if (consumedEgTimerPtr->SdSubscribeEventgroupRetryMax > 0u)
    {
        if (consumedEgRTPtr->SubsRetryTimer > 0u)
        {
            consumedEgRTPtr->SubsRetryTimer--;
            if (0u == consumedEgRTPtr->SubsRetryTimer && consumedEgTimerPtr->SdSubscribeEventgroupRetryMax < 0xff)
            {
                consumedEgRTPtr->SubsRetryCnt++;
                /* If the counts of retries of subscription exceeds, report runtime error
                 * "SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED".*/
                if (consumedEgRTPtr->SubsRetryCnt > consumedEgTimerPtr->SdSubscribeEventgroupRetryMax)
                {
                    consumedEgRTPtr->SubsRetryEnable = FALSE;
                    Sd_ReportRuntimeError(SD_MAIN_FUNCTION_ID, SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED);
                }
            }
        }
    }
}
#endif

/**
 * @brief              Manage client Timer
 * @param[in]          clientServicePtr: Pointer to client service.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74140
 */
SD_LOCAL void Sd_ClientTimerHandle(const Sd_ClientServiceType* clientServicePtr)
{
    Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    if (cRtDataPtr->Ttl.ttlms > 0ULL)
    {
        cRtDataPtr->Ttl.ttlms--;
        if (0ULL == cRtDataPtr->Ttl.ttlms)
        {
            Sd_ClientTimerExpireHandler(clientServicePtr, cRtDataPtr);
        }
    }

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    if (cRtDataPtr->Ttl.IsValidForever || (cRtDataPtr->Ttl.ttlms > 0ULL))
    {
        const Sd_ConsumedEventGroupType* eventGroupPtr = &clientServicePtr->SdConsumedEventGroup[0];
        for (uint16 eventGroupIdx = 0u; eventGroupIdx < clientServicePtr->ConsumedEventGroupNum; eventGroupIdx++)
        {
            Sd_ConsumedEventGroupTimerHandler(eventGroupPtr, cRtDataPtr);
/* @req SWS_SD_00735,SWS_SD_00737,SWS_SD_00738 */
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
            Sd_ConsumedEventGroupRetryStatusHandler(eventGroupPtr);
#endif
            eventGroupPtr++;
        }
    }
#endif
}
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
/**
 * @brief              Append SendQueue item to SendQueue tail
 * @param[inout]       head: Head pointer of send queue.
 * @param[in]          sendQueueItem: The item that would be appended to send queue.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74142
 */
SD_LOCAL void Sd_AppendToSendQueueTail(Sd_SendQueueType** head, Sd_SendQueueType* sendQueueItem)
{
    if (NULL_PTR != *head)
    {
        Sd_SendQueueType* tail = *head;
        while (NULL_PTR != tail->Next)
        {
            tail = tail->Next;
        }
        tail->Next = sendQueueItem;
    }
    else
    {
        *head = sendQueueItem;
    }
}

/**
 * @brief              Clear SendQueue
 * @param[inout]       head: Head pointer of send queue.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74143
 */
SD_LOCAL void Sd_ClearSendQueue(Sd_SendQueueType** head, ApplicationType applicationId)
{
    const Sd_SendQueueType* curPtr = *head;

    while (NULL_PTR != curPtr)
    {
        const Sd_SendQueueType* nextPtr = curPtr->Next;
        Sd_FreeEx(curPtr->EntryPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(curPtr->OptionPtr, Sd_TxMemPool[applicationId]);
        Sd_FreeEx(curPtr, Sd_TxMemPool[applicationId]);
        curPtr = nextPtr;
    }
    *head = NULL_PTR;
}

/**
 * @brief              Calculate number of TCP/UDP Endpoint Option number
 * @param[in]          sdMsg: Received Sd Message
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @param[in]          tcpEndpointNum: Tcp end point number.
 * @param[in]          udpEndpointNum: Udp end point number.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74144
 */
SD_LOCAL void Sd_CalcTcpUdpEndpointOptionNum(
    const uint8* sdMsg,
    uint16       entryPos,
    const uint16 optStartPosMap[],
    uint8*       tcpEndpointNum,
    uint8*       udpEndpointNum)
{
    uint8 ipv4EndpTcpNum = 0u;
    uint8 ipv4EndpUdpNum = 0u;
    uint8 ipv6EndpTcpNum = 0u;
    uint8 ipv6EndpUdpNum = 0u;

    Sd_Type1EntryType type1Entry;
    (void)IStdLib_MemCpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    uint8 idx1stOptions = type1Entry.Index1stOptions;
    uint8 numOpt1       = type1Entry.NumOfOpt1;
    uint8 idx2ndOptions = type1Entry.Index2ndOptions;
    uint8 numOpt2       = type1Entry.NumOfOpt2;

    /* Calculate all kinds options number */
    uint8 optNumSum = numOpt1 + numOpt2;
    uint8 optIndex;

    for (uint8 index = 0u; index < optNumSum; index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        uint8 optType = sdMsg[optStartPosMap[optIndex] + SD_OPTION_TYPE_FILED_OFFSET];

        if ((SD_IPV4_ENDPOINT_OPTION == optType) || (SD_IPV4_MULTICAST_OPTION == optType))
        {
            Sd_IPv4OptionsType ipv4Opt;
            (void)IStdLib_MemCpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
#if ((SD_SERVER_SERVICE_USE_TCP == STD_ON) || (SD_CLIENT_SERVICE_USE_TCP == STD_ON))
            if (SD_L4_PROTO_TCP == ipv4Opt.L4Proto)
            {
                ipv4EndpTcpNum++;
            }
            else
#endif
#if ((SD_SERVER_SERVICE_USE_UDP == STD_ON) || (SD_CLIENT_SERVICE_USE_UDP == STD_ON))
                if (SD_L4_PROTO_UDP == ipv4Opt.L4Proto)
            {
                ipv4EndpUdpNum++;
            }
            else
#endif
            {
                /* Nothing to do here */
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else if (SD_IPV6_ENDPOINT_OPTION == optType || SD_IPV6_MULTICAST_OPTION == optType)
        {
            Sd_IPv6OptionsType ipv6Opt;
            (void)IStdLib_MemCpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
            if (SD_L4_PROTO_TCP == ipv6Opt.L4Proto)
            {
                ipv6EndpTcpNum++;
            }
            else if (SD_L4_PROTO_UDP == ipv6Opt.L4Proto)
            {
                ipv6EndpUdpNum++;
            }
            else
            {
                /*Nothing to do here*/
            }
        }
        else
        {
            /*Nothing to do here*/
        }
#endif /* STD_ON == SD_IPV6_ENABLE */
    }

    /* PRQA S 2985 ++ */ /* VL_QAC_2985_2986 */
    *tcpEndpointNum = (ipv4EndpTcpNum + ipv6EndpTcpNum);
    *udpEndpointNum = (ipv4EndpUdpNum + ipv6EndpUdpNum);
    /* PRQA S 2985 -- */
}

/**
 * @brief              Check if necessary option exist
 *                     [serverServicePtr] or [clientServicePtr] must have one equal NULL_PTR
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          rxQueueItem: Rx queue item.
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @param[in]          applicationId Application ID.
 * @return             E_OK: No Error
 *                     E_NOT_OK: Error,need response Nack
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74145
 */
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL Std_ReturnType Sd_NecessaryOptionsExistCheck(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint16                      entryPos,
    const uint16                optStartPosMap[],
    ApplicationType             applicationId)
/* PRQA S 6040 -- */
{
    Std_ReturnType ret = E_NOT_OK;
    uint8          tcpEndpointNum;
    uint8          udpEndpointNum;

    const uint8* sdMsg = rxQueueItem->BufPtr;
    Sd_CalcTcpUdpEndpointOptionNum(sdMsg, entryPos, optStartPosMap, &tcpEndpointNum, &udpEndpointNum);

    /*@req <SWS_SD_00710> */
    /* Check if necessary option exist */
#if (SD_SERVER_SERVICE_NUM > 0)
    if (NULL_PTR != serverServicePtr)
    {
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
        if (((NULL_PTR != serverServicePtr->SdServerServiceTcpRef) && (tcpEndpointNum != 0u)))
        {
            ret = E_OK;
        }
        else
#endif
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
            if ((NULL_PTR != serverServicePtr->SdServerServiceUdpRef) && (udpEndpointNum != 0u))
        {
            ret = E_OK;
        }
        else
#endif
        {
            /* Nothing to do */
        }
    }
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
    if (NULL_PTR != clientServicePtr)
    {
#if (SD_CLIENT_SERVICE_USE_TCP == STD_ON)
        if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && (tcpEndpointNum != 0u))
        {
            ret = E_OK;
        }
        else
#endif
#if (SD_CLIENT_SERVICE_USE_UDP == STD_ON)
            if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && (udpEndpointNum != 0u))
        {
            ret = E_OK;
        }
        else
#endif
#if (SD_CLIENT_SERVICE_USE_MULITICAST == STD_ON)
            if ((NULL_PTR != clientServicePtr->SdClientServiceMulticastRef) && (udpEndpointNum != 0u))
        {
            ret = E_OK;
        }
        else
#endif
        {
            /* Nothing to do */
        }
    }
#endif
    SD_UNUSED(instancePtr);
    SD_UNUSED(applicationId);
    return ret;
}

/**
 * @brief              Package entries and options to one message
 * @param[inout]       sendQueue: Send queue
 * @param[in]          sendQueCnt: The item number of send queue.
 * @param[inout]       pduInfoPtr: Pointer for pdu info.
 * @param[in]          instanceId: Sd instance id.
 * @param[in]          isMulticastTx: Whether to use multicast to send messages
 * @param[in]          applicationId Application ID.
 * @return             E_OK: Find Multicast option and get info
 *                     E_NOT_OK: Get info failure
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74146
 */
/* PRQA S 6040 ++ */ /* VL_MTR_Sd_STPAR */
SD_LOCAL Std_ReturnType Sd_PackageMessage(
    Sd_SendQueueType* const sendQueue[],
    uint8                   sendQueCnt,
    PduInfoType*            pduInfoPtr,
    uint16                  instanceId,
    boolean                 isMulticastTx,
    ApplicationType         applicationId)
/* PRQA S 6040 -- */
{
    uint32         optionTotalLen = 0u;
    Std_ReturnType ret            = E_NOT_OK;

    /* Calculate message length */
    uint16 entryTotalLen = (uint16)sendQueCnt * SD_MESSAGE_ENTRY_TOTAL_LEN;
    for (uint8 index = 0u; index < sendQueCnt; index++)
    {
        optionTotalLen += sendQueue[index]->OptionLen;
    }
    /* Message total length = header length(12) + entry length field(4) + entries length
     * + option length field(4) + options length */
    uint32 headerLen     = SD_HEADER_LEN + SD_MESSAGE_ENTRYLEN_FILED_LEN + SD_MESSAGE_OPTIONLEN_FILED_LEN;
    uint32 messageLength = headerLen + entryTotalLen + optionTotalLen;
    /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
    uint8* messagePtr = (uint8*)Sd_MallocEx(messageLength, Sd_TxMemPool[applicationId]);
    /* PRQA S 0316 -- */

    if (NULL_PTR != messagePtr)
    {
        /* Clear allocate memory */
        (void)IStdLib_MemSet(messagePtr, 0u, messageLength);

        /* Package entries and options to one buffer */
        /* Header */
        TcpIp_SockAddrType remoteAddr = sendQueue[0u]->DestAddr;
        Sd_SetHeader(messagePtr, instanceId, &remoteAddr, isMulticastTx);
        /* Length of Entries Array in Bytes,Offset: 12(HeaderLength) */
        uint32 tLengthField = Sd_HtoNl(entryTotalLen);
        (void)IStdLib_MemCpy(&messagePtr[SD_HEADER_LEN], (uint8*)&tLengthField, sizeof(uint32));
        /* Length of Options Array in Bytes,Offset: 16 = 12(HeaderLength) + 4(Length of Entries Array) */
        tLengthField = Sd_HtoNl(optionTotalLen);
        headerLen    = SD_HEADER_LEN + SD_MESSAGE_ENTRYLEN_FILED_LEN;
        (void)IStdLib_MemCpy(&messagePtr[headerLen + entryTotalLen], (uint8*)&tLengthField, sizeof(uint32));
        /* Entyies & Options */
        /* First entry offset: 16 = 12(HeaderLength) + 4(Length of Entries Array) */
        uint32 entryOffset = SD_HEADER_LEN + SD_MESSAGE_ENTRYLEN_FILED_LEN;
        uint8* entryRunPtr = &messagePtr[entryOffset];
        /* First entry offset: header length(12) + entry length field(4) + entries length + option length field(4) */
        headerLen           = SD_HEADER_LEN + SD_MESSAGE_ENTRYLEN_FILED_LEN + SD_MESSAGE_OPTIONLEN_FILED_LEN;
        uint8* optionRunPtr = &messagePtr[headerLen + entryTotalLen];
        uint8  optIdxCnt    = 0u;
        for (uint8 index = 0u; index < sendQueCnt; index++)
        {
            const Sd_SendQueueType* sendQuePtr = sendQueue[index];
            Sd_Type1EntryType       type1Entry;

            /* Entyies */
            (void)IStdLib_MemCpy((uint8*)&type1Entry, sendQuePtr->EntryPtr, sizeof(Sd_Type1EntryType));
            type1Entry.NumOfOpt1       = sendQuePtr->OptionNum;
            type1Entry.Index1stOptions = optIdxCnt;
            (void)IStdLib_MemCpy(entryRunPtr, (uint8*)&type1Entry, sizeof(Sd_Type1EntryType));
            optIdxCnt += sendQuePtr->OptionNum;
            entryOffset = SD_HEADER_LEN + SD_MESSAGE_ENTRYLEN_FILED_LEN;
            entryRunPtr = &entryRunPtr[entryOffset];
            Sd_FreeEx(sendQuePtr->EntryPtr, Sd_TxMemPool[applicationId]);

            /* Options */
            (void)IStdLib_MemCpy(optionRunPtr, sendQuePtr->OptionPtr, sendQuePtr->OptionLen);
            optionRunPtr = &optionRunPtr[sendQuePtr->OptionLen];
            Sd_FreeEx(sendQuePtr->OptionPtr, Sd_TxMemPool[applicationId]);

            Sd_FreeEx(sendQuePtr, Sd_TxMemPool[applicationId]);
        }

        pduInfoPtr->SduDataPtr = messagePtr;
        pduInfoPtr->SduLength  = (PduLengthType)messageLength;
        ret                    = E_OK;
    }
    else
    {
        for (uint8 index = 0u; index < sendQueCnt; index++)
        {
            const Sd_SendQueueType* sendQuePtr = sendQueue[index];
            /* Free buffer */
            Sd_FreeEx(sendQuePtr->EntryPtr, Sd_TxMemPool[applicationId]);
            Sd_FreeEx(sendQuePtr->OptionPtr, Sd_TxMemPool[applicationId]);
            Sd_FreeEx(sendQuePtr, Sd_TxMemPool[applicationId]);
        }
    }

    return ret;
}

/**
 * @brief              Transmit multicast message
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[inout]       multiSendQue: Multicast send queue.
 * @param[in]          multiSendCnt: The item number of send queue.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74147
 */
SD_LOCAL void Sd_TransmitMulticastMessage(
    const Sd_InstanceType*  instancePtr,
    Sd_SendQueueType* const multiSendQue[],
    uint8                   multiSendCnt,
    ApplicationType         applicationId)
{
    PduInfoType pduInfo;

    Std_ReturnType ret =
        Sd_PackageMessage(multiSendQue, multiSendCnt, &pduInfo, instancePtr->SdInstanceHandleId, TRUE, applicationId);
    /* Transmit Message */
    if (E_OK == ret)
    {
        /* Get remote address */
        const Sd_RxPduIdSoConIdMapType* rxPduIdSoConIdMapPtr = Sd_CfgPtr->GlbRxPduIdSoConIdMap;
        for (uint8 index = 0u; index < Sd_CfgPtr->RxPduIdSoConIdMapLen; index++)
        {
            if (rxPduIdSoConIdMapPtr->RxPduId == instancePtr->SdInstanceMulticastRxPdu)
            {
                TcpIp_SockAddrType localAddr;
                TcpIp_SockAddrType defaultRouter;
                uint8              netmask;

                localAddr.domain     = instancePtr->SdAddrFamily;
                defaultRouter.domain = instancePtr->SdAddrFamily;
                Std_ReturnType retGetLocalAddr =
                    SoAd_GetLocalAddr(rxPduIdSoConIdMapPtr->SoConId, &localAddr, &netmask, &defaultRouter);
                SoAd_SoConIdType soConId = instancePtr->SdInstanceTxPduSoConId;
                if (E_OK == retGetLocalAddr)
                {
                    (void)SoAd_SetRemoteAddr(soConId, &localAddr);
                    (void)SoAd_IfTransmit(instancePtr->SdInstanceTxPduRef, &pduInfo);
                    SoAd_ReleaseRemoteAddr(soConId);
                }
            }
            rxPduIdSoConIdMapPtr++;
        }

        /* Free message used buffer */
        Sd_FreeEx(pduInfo.SduDataPtr, Sd_TxMemPool[applicationId]);
    }
}

/**
 * @brief              Transmit unicast message
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[inout]       uniSendQue: Unicast send queue.
 * @param[in]          uniSendCnt: The item number of send queue.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74148
 */
SD_LOCAL void Sd_TransmitUnicastMessage(
    const Sd_InstanceType*  instancePtr,
    Sd_SendQueueType* const uniSendQue[],
    uint8                   uniSendCnt,
    ApplicationType         applicationId)
{
    uint8 numPerAddrKind[SD_MAX_ENTRIES_PER_FRAME];
    /* PRQA S 0686 ++ */ /* VL_Sd_ResolveWarning */
    uint8 sendQueEleList[SD_MAX_ENTRIES_PER_FRAME][SD_MAX_ENTRIES_PER_FRAME] = {{0}};
    /* PRQA S 0686 -- */

    (void)IStdLib_MemSet(numPerAddrKind, 0u, SD_MAX_ENTRIES_PER_FRAME);
    numPerAddrKind[0u]     = 1u;
    sendQueEleList[0u][0u] = 0u;
    uint8 numAllKind       = 1u;

    for (uint8 i = 1u; i < uniSendCnt; i++)
    {
        uint8 j = 0u;
        for (j = 0u; j < numAllKind; j++)
        {
            StatusType isSame = IStdLib_MemCmp(
                (uint8*)&uniSendQue[i]->DestAddr,
                (uint8*)&uniSendQue[sendQueEleList[numAllKind][0u]]->DestAddr,
                sizeof(uniSendQue[i]->DestAddr));
            if (0u == isSame)
            {
                sendQueEleList[j][numPerAddrKind[j]] = i;
                numPerAddrKind[j]++;
                break;
            }
        }

        /* Not find,new remote address appear */
        if (j == numAllKind)
        {
            sendQueEleList[numAllKind][0u] = i;
            numPerAddrKind[numAllKind]     = 1u;
            numAllKind++;
        }
    }

    Sd_SendQueueType* sendQue[SD_MAX_ENTRIES_PER_FRAME] = {0};
    for (uint8 i = 0u; i < numAllKind; i++)
    {
        for (uint8 j = 0u; j < numPerAddrKind[i]; j++)
        {
            sendQue[j] = uniSendQue[sendQueEleList[i][j]];
        }

        TcpIp_SockAddrType destAddr = sendQue[0]->DestAddr;
        PduInfoType        pduInfo;
        Std_ReturnType     ret = Sd_PackageMessage(
            sendQue,
            numPerAddrKind[i],
            &pduInfo,
            instancePtr->SdInstanceHandleId,
            FALSE,
            applicationId);
        if (E_OK == ret)
        {
            SoAd_SoConIdType soConId = instancePtr->SdInstanceTxPduSoConId;
            (void)SoAd_SetRemoteAddr(soConId, &destAddr);
            (void)SoAd_IfTransmit(instancePtr->SdInstanceTxPduRef, &pduInfo);
            SoAd_ReleaseRemoteAddr(soConId);

            /* Free message used buffer */
            Sd_FreeEx(pduInfo.SduDataPtr, Sd_TxMemPool[applicationId]);
        }
    }
}

/**
 * @brief              Find out Multicast entry that ttl = 0 to send
 * @param[in]          queueHeaderPtr: Head pointer of send queue.
 * @param[inout]       multiSendQue: Multicast send queue.
 * @param[inout]       multiSendCntPtr: The item number of send queue.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74149
 */
SD_LOCAL void Sd_FindMulticastMsgToSendFromSendQueue(
    Sd_SendQueueType** queueHeaderPtr,
    Sd_SendQueueType*  multiSendQue[],
    uint8*             multiSendCntPtr)
{
    uint8 multiSendCnt = *multiSendCntPtr;
    /* Find out need to be send obj form a send queue */
    Sd_SendQueueType* nowPtr  = *queueHeaderPtr;
    Sd_SendQueueType* lastPtr = NULL_PTR;

    while ((NULL_PTR != nowPtr) && (multiSendCnt < SD_MAX_ENTRIES_PER_FRAME))
    {
        boolean deleteFromQue = FALSE;
        if (!nowPtr->IsUnicastSend)
        {
            if (0UL == nowPtr->SendTime)
            {
                multiSendQue[multiSendCnt] = nowPtr;
                multiSendCnt++;
                deleteFromQue = TRUE;

                if (NULL_PTR == lastPtr)
                {
                    *queueHeaderPtr = nowPtr->Next;
                }
                else
                {
                    lastPtr->Next = nowPtr->Next;
                }
                nowPtr = nowPtr->Next;
            }
            else
            {
                nowPtr->SendTime--;
            }
        }

        if (!deleteFromQue)
        {
            lastPtr = nowPtr;
            nowPtr  = nowPtr->Next;
        }
    }

    *multiSendCntPtr = multiSendCnt;
}

/**
 * @brief              Find out Unicast entry that ttl = 0 to send
 * @param[in]          queueHeaderPtr: Head pointer of send queue.
 * @param[inout]       uniSendQue: Unicast send queue.
 * @param[inout]       uniSendCntPtr: The item number of send queue.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74150
 */
SD_LOCAL void Sd_FindUnicastMsgToSendFromSendQueue(
    Sd_SendQueueType** queueHeaderPtr,
    Sd_SendQueueType*  uniSendQue[],
    uint8*             uniSendCntPtr)
{
    uint8 uniSendCnt = *uniSendCntPtr;
    /* Find out need to be send obj form a send queue */
    Sd_SendQueueType* nowPtr  = *queueHeaderPtr;
    Sd_SendQueueType* lastPtr = NULL_PTR;

    while ((NULL_PTR != nowPtr) && (uniSendCnt < SD_MAX_ENTRIES_PER_FRAME))
    {
        boolean deleteFromQue = FALSE;
        if (nowPtr->IsUnicastSend)
        {
            if (0UL == nowPtr->SendTime)
            {
                uniSendQue[uniSendCnt] = nowPtr;
                uniSendCnt++;
                deleteFromQue = TRUE;

                if (NULL_PTR == lastPtr)
                {
                    *queueHeaderPtr = nowPtr->Next;
                }
                else
                {
                    lastPtr->Next = nowPtr->Next;
                }
                nowPtr = nowPtr->Next;
            }
            else
            {
                nowPtr->SendTime--;
            }
        }

        if (!deleteFromQue)
        {
            lastPtr = nowPtr;
            nowPtr  = nowPtr->Next;
        }
    }

    *uniSendCntPtr = uniSendCnt;
}

/**
 * @brief              Find out multicast message for transmit
 * @param[in]          instancePtr: Pointer to instance structure.
 * @param[inout]       multiSendQue: Multicast send queue.
 * @return             Number of entry send by multicast
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74151
 */
SD_LOCAL uint8 Sd_FindoutMulticastMessage(const Sd_InstanceType* instancePtr, Sd_SendQueueType* multiSendQue[])
{
    uint8 multiSendCnt = 0u;

#if (SD_SERVER_SERVICE_NUM > 0)
    const Sd_ServerServiceType* serverServicePtr = instancePtr->SdServerService;
    if (NULL_PTR != serverServicePtr)
    {
        for (uint16 idx = 0u; idx < instancePtr->ServerServiceNum; idx++)
        {
            Sd_ServerServiceRTType* sRtDataPtr = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];

            Sd_FindMulticastMsgToSendFromSendQueue(&sRtDataPtr->Head, multiSendQue, &multiSendCnt);

            serverServicePtr++;
        }
    }
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
    const Sd_ClientServiceType* clientServicePtr = instancePtr->SdClientService;
    if (NULL_PTR != clientServicePtr)
    {
        for (uint16 idx = 0u; idx < instancePtr->ClientServiceNum; idx++)
        {
            Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];

            Sd_FindMulticastMsgToSendFromSendQueue(&cRtDataPtr->Head, multiSendQue, &multiSendCnt);

            clientServicePtr++;
        }
    }
#endif

    return multiSendCnt;
}

/**
 * @brief              Build unicast message for transmit
 * @param[in]          instancePtr: Pointer for instance
 * @param[inout]       uniSendQue: Unicast send queue.
 * @return             Number of entry send by unicast
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74152
 */
SD_LOCAL uint8 Sd_FindoutUnicastMessage(const Sd_InstanceType* instancePtr, Sd_SendQueueType* uniSendQue[])
{
    uint8 uniSendCnt = 0u;

#if (SD_SERVER_SERVICE_NUM > 0)
    const Sd_ServerServiceType* serverServicePtr = instancePtr->SdServerService;
    if (NULL_PTR != serverServicePtr)
    {
        for (uint16 idx = 0u; idx < instancePtr->ServerServiceNum; idx++)
        {
            Sd_ServerServiceRTType* sRtDataPtr = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];

            Sd_FindUnicastMsgToSendFromSendQueue(&sRtDataPtr->Head, uniSendQue, &uniSendCnt);

            serverServicePtr++;
        }
    }
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
    const Sd_ClientServiceType* clientServicePtr = instancePtr->SdClientService;
    if (NULL_PTR != clientServicePtr)
    {
        for (uint16 idx = 0u; idx < instancePtr->ClientServiceNum; idx++)
        {
            Sd_ClientServiceRTType* cRtDataPtr = Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];

            Sd_FindUnicastMsgToSendFromSendQueue(&cRtDataPtr->Head, uniSendQue, &uniSendCnt);

            clientServicePtr++;
        }
    }
#endif

    return uniSendCnt;
}

/**
 * @brief              Transmit message
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74153
 */
SD_LOCAL void Sd_TransmitMessage(ApplicationType applicationId)
{
    Sd_SendQueueType* txMsgSendQue[SD_MAX_ENTRIES_PER_FRAME]; /* temp save need send item */

    for (uint16 instanceIdx = 0u; instanceIdx < SD_INSTANCE_NUM; instanceIdx++)
    {
        const Sd_InstanceType* instancePtr = &Sd_CfgPtr->Instance[instanceIdx];
        /* Due to the limitations of SoAd module, it is not possible to send both unicast
         * and multicast message at the same time. Prioritize sending unicast messages */
        uint8 uniSendCnt = Sd_FindoutUnicastMessage(instancePtr, txMsgSendQue);
        if (uniSendCnt > 0u)
        {
            Sd_TransmitUnicastMessage(instancePtr, txMsgSendQue, uniSendCnt, applicationId);
        }

        uint8 multiSendCnt = Sd_FindoutMulticastMessage(instancePtr, txMsgSendQue);
        if (multiSendCnt > 0u)
        {
            Sd_TransmitMulticastMessage(instancePtr, txMsgSendQue, multiSendCnt, applicationId);
        }
    }
}
#endif /* SD_SERVER_SERVICE_NUM > 0 || SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
/**
 * @brief              Check if received Configuration Option match with configured
 * @param[in]          instancePtr: Pointer for sd instance.
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          rxQueueItem: Rx queue item.
 * @param[in]          entryPos: Sd message entry index.
 * @param[in]          optStartPosMap: Pointer for entry first byte.
 * @param[in]          applicationId Application ID.
 * @return             FALSE/TRUE
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74154
 */
SD_LOCAL boolean Sd_RxConfigurationOptionMatchCheck(
    const Sd_InstanceType*      instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_RxQueueType*       rxQueueItem,
    uint16                      entryPos,
    const uint16*               optStartPosMap,
    ApplicationType             applicationId)
{
    boolean ret = TRUE;

    if (((NULL_PTR != serverServicePtr) && (serverServicePtr->SdServerCapabilityRecordMatchCalloutRef != 0xFFFFu))
        || ((NULL_PTR != clientServicePtr) && (clientServicePtr->SdClientCapabilityRecordMatchCalloutRef != 0xFFFFu)))
    {
        const uint8*      sdMsg = rxQueueItem->BufPtr;
        Sd_Type1EntryType type1Entry;
        (void)IStdLib_MemCpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
        uint8 idx1stOptions = type1Entry.Index1stOptions;
        uint8 numOpt1       = type1Entry.NumOfOpt1;
        uint8 idx2ndOptions = type1Entry.Index2ndOptions;
        uint8 numOpt2       = type1Entry.NumOfOpt2;
        uint8 optNumSum     = numOpt1 + numOpt2;
        uint8 optIndex;

        for (uint8 index = 0; index < optNumSum; index++)
        {
            if (index < numOpt1)
            {
                optIndex = idx1stOptions + index;
            }
            else
            {
                optIndex = idx2ndOptions + index - numOpt1;
            }

            uint8  optType         = sdMsg[optStartPosMap[optIndex] + 2u];
            uint8* createCfgOptPtr = NULL_PTR;

            if (SD_CONFIGURATION_OPTION == optType)
            {
                uint16 cfgOptLen = 0;
#if ((SD_SERVER_CAPABILITY_CFG_USED == STD_ON) || (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON))
                cfgOptLen = Sd_GetType1ConfigurationOptionLength(instancePtr, serverServicePtr, clientServicePtr);
#endif

#if (SD_SERVER_CAPABILITY_CFG_USED == STD_ON)
                if (NULL_PTR != serverServicePtr)
                {
                    if (cfgOptLen > 0u)
                    {
                        createCfgOptPtr = Sd_MallocEx(cfgOptLen, Sd_RxMemPool[applicationId]);
                        if (NULL_PTR != createCfgOptPtr)
                        {
                            Sd_BuildType1ConfigurationOption(
                                createCfgOptPtr,
                                cfgOptLen,
                                instancePtr,
                                serverServicePtr,
                                clientServicePtr);

                            /* @req SWS_SD_00716 */
                            uint16 matchCalloutIdx = serverServicePtr->SdServerCapabilityRecordMatchCalloutRef;
                            ret                    = Sd_CfgPtr->CapabilityRecordMatchCalloutRef[matchCalloutIdx](
                                rxQueueItem->RxPduId,
                                type1Entry.Type,
                                type1Entry.ServiceID,
                                type1Entry.InstanceID,
                                type1Entry.MajorVersion,
                                type1Entry.MinorVersion,
                                (const Sd_ConfigOptionStringType*)&sdMsg[optStartPosMap[optIndex] + 4u],
                                (const Sd_ConfigOptionStringType*)&createCfgOptPtr[4u]);
                            Sd_FreeEx(createCfgOptPtr, Sd_RxMemPool[applicationId]);
                            break;
                        }
                    }
                }
                else
#endif
#if (SD_CLIENT_CAPABILITY_CFG_USED == STD_ON)
                    if (NULL_PTR != clientServicePtr)
                {
                    if (cfgOptLen > 0u)
                    {
                        createCfgOptPtr = Sd_MallocEx(cfgOptLen, Sd_RxMemPool[applicationId]);
                        if (NULL_PTR != createCfgOptPtr)
                        {
                            Sd_BuildType1ConfigurationOption(
                                createCfgOptPtr,
                                cfgOptLen,
                                instancePtr,
                                serverServicePtr,
                                clientServicePtr);

                            /* @req SWS_SD_00716 */
                            uint16 matchCalloutIdx = clientServicePtr->SdClientCapabilityRecordMatchCalloutRef;
                            ret                    = Sd_CfgPtr->CapabilityRecordMatchCalloutRef[matchCalloutIdx](
                                rxQueueItem->RxPduId,
                                type1Entry.Type,
                                type1Entry.ServiceID,
                                type1Entry.InstanceID,
                                type1Entry.MajorVersion,
                                type1Entry.MinorVersion,
                                (const Sd_ConfigOptionStringType*)&sdMsg[optStartPosMap[optIndex] + 4u],
                                (const Sd_ConfigOptionStringType*)&createCfgOptPtr[4u]);
                            Sd_FreeEx(createCfgOptPtr, Sd_RxMemPool[applicationId]);
                            break;
                        }
                    }
                }
                else
#endif
                {
                    /* Nothing to do */
                }
            }
        }
    }

    return ret;
}
#endif /* SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u */

/**
 * @brief              Save remote address used for SessionId control
 * @param[in]          instanceId: Sd instance id.
 * @param[in]          rxQueueItem: Rx queue item.
 * @param[in]          isRxUseMulticast: Whether to use multicast to receive messages.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74155
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void Sd_SaveRemoteSessionId(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, boolean isRxUseMulticast)
{
    Sd_HeaderType header;

    (void)IStdLib_MemCpy((uint8*)&header, rxQueueItem->BufPtr, sizeof(Sd_HeaderType));
    Sd_SessionIdCtrlType* sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, &rxQueueItem->RemoteAddr);
    if (NULL_PTR != sesIdCtrlPtr)
    {
        if (isRxUseMulticast)
        {
            sesIdCtrlPtr->RxMultiSesIdCnt   = Sd_NtoHs(header.SessionID);
            sesIdCtrlPtr->RxMultiRebootFlag = header.RebootFlag;
        }
        else
        {
            sesIdCtrlPtr->RxUniSesIdCnt   = Sd_NtoHs(header.SessionID);
            sesIdCtrlPtr->RxUniRebootFlag = header.RebootFlag;
        }
    }
}
#endif

#if (SD_SERVER_SERVICE_NUM > 0)
/**
 * @brief              Save remote address used for SessionId control
 * @param[in]          instanceId: Sd instance id.
 * @param[in]          remoteAddrPtr: Pointer for remote address buffer.
 * @param[in]          serverServiceHandleId: Server service handle id.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74156
 */
/* PRQA S 6030 ++ */ /* VL_MTR_Sd_STMIF */
SD_LOCAL void Sd_SaveServerServiceHandleIdToSessionCtrlList(
    uint16                    instanceId,
    const TcpIp_SockAddrType* remoteAddrPtr,
    uint16                    serverServiceHandleId,
    ApplicationType           applicationId)
/* PRQA S 6030 -- */
{
    if (!TCPIP_IS_IPADDR_ANY(*remoteAddrPtr))
    {
        Sd_SessionIdCtrlType* sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, remoteAddrPtr);
        if (NULL_PTR != sesIdCtrlPtr)
        {
            Sd_ServiceHandleIdListType* serverServiceList = sesIdCtrlPtr->ServerServiceList;
            if (NULL_PTR != serverServiceList)
            {
                boolean findFlag = FALSE;
                while (NULL_PTR != serverServiceList)
                {
                    findFlag = Sd_FindHandlerIdInList(serverServiceHandleId, serverServiceList);
                    if (findFlag)
                    {
                        break;
                    }

                    serverServiceList = serverServiceList->Next;
                }

                if (!findFlag)
                {
                    serverServiceList = sesIdCtrlPtr->ServerServiceList;
                    while (NULL_PTR != serverServiceList)
                    {
                        if (serverServiceList->HandleIdNum < SD_MAX_REMOTE_SERVICE_NUM)
                        {
                            serverServiceList->ServiceHandleId[serverServiceList->HandleIdNum] = serverServiceHandleId;
                            serverServiceList->HandleIdNum++;
                            break;
                        }
                        serverServiceList = serverServiceList->Next;
                    }
                }
            }
            else
            {
                /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
                serverServiceList = (Sd_ServiceHandleIdListType*)Sd_MallocEx(
                    sizeof(Sd_ServiceHandleIdListType),
                    Sd_RxMemPool[applicationId]);
                /* PRQA S 0316 -- */
                if (NULL_PTR != serverServiceList)
                {
                    serverServiceList->Next                = NULL_PTR;
                    serverServiceList->ServiceHandleId[0u] = serverServiceHandleId;
                    serverServiceList->HandleIdNum         = 1u;
                    sesIdCtrlPtr->ServerServiceList        = serverServiceList;
                }
            }
        }
    }
}
#endif /* SD_SERVER_SERVICE_NUM > 0 */

#if (SD_CLIENT_SERVICE_NUM > 0)
/**
 * @brief              Save remote address used for SessionId control
 * @param[in]          instanceId: Sd instance id.
 * @param[in]          remoteAddrPtr: Pointer for remote address buffer.
 * @param[in]          clientServiceHandleId: Client service handle id.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74157
 */
/* PRQA S 2889,6030 ++ */ /* VL_QAC_MultiReturn,VL_MTR_Sd_STMIF */
SD_LOCAL void Sd_SaveClientServiceHandleIdToSessionCtrlList(
    uint16                    instanceId,
    const TcpIp_SockAddrType* remoteAddrPtr,
    uint16                    clientServiceHandleId,
    ApplicationType           applicationId)
{
    if (!TCPIP_IS_IPADDR_ANY(*remoteAddrPtr))
    {
        Sd_SessionIdCtrlType* sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, remoteAddrPtr);
        if (NULL_PTR != sesIdCtrlPtr)
        {
            Sd_ServiceHandleIdListType* clientServiceList = sesIdCtrlPtr->ClientServiceList;
            if (NULL_PTR != clientServiceList)
            {
                boolean findFlag = FALSE;
                while (NULL_PTR != clientServiceList)
                {
                    findFlag = Sd_FindHandlerIdInList(clientServiceHandleId, clientServiceList);
                    if (findFlag)
                    {
                        break;
                    }
                    clientServiceList = clientServiceList->Next;
                }

                clientServiceList = sesIdCtrlPtr->ClientServiceList;
                while (NULL_PTR != clientServiceList)
                {
                    if (clientServiceList->HandleIdNum < SD_MAX_REMOTE_SERVICE_NUM)
                    {
                        clientServiceList->ServiceHandleId[clientServiceList->HandleIdNum] = clientServiceHandleId;
                        clientServiceList->HandleIdNum++;
                        break;
                    }
                    clientServiceList = clientServiceList->Next;
                }
            }
            else
            {
                /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
                clientServiceList = (Sd_ServiceHandleIdListType*)Sd_MallocEx(
                    sizeof(Sd_ServiceHandleIdListType),
                    Sd_RxMemPool[applicationId]);
                /* PRQA S 0316 -- */
                if (NULL_PTR != clientServiceList)
                {
                    clientServiceList->Next                = NULL_PTR;
                    clientServiceList->ServiceHandleId[0u] = clientServiceHandleId;
                    clientServiceList->HandleIdNum         = 1u;
                    sesIdCtrlPtr->ClientServiceList        = clientServiceList;
                }
            }
        }
    }
}
/* PRQA S 2889,6030 -- */
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

/**
 * @brief              Check if the remote address is in the Session ID control table.
 * @param[in]          instanceId: Sd instance id.
 * @param[in]          remoteAddrPtr: Pointer for remote address buffer.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74158
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL Sd_SessionIdCtrlType*
    Sd_FindRemoteAddrSessionIdCtrl(uint16 instanceId, const TcpIp_SockAddrType* remoteAddrPtr)
{
    const Sd_InstanceRTType* instanceRtPtr = Sd_InstanceRTData[instanceId];
    Sd_SessionIdCtrlType*    sesIdCtrlPtr  = NULL_PTR;

    if (NULL_PTR != instanceRtPtr->SessionIdHandle.SesIdCtrlList)
    {
        sesIdCtrlPtr = instanceRtPtr->SessionIdHandle.SesIdCtrlList;
        while (sesIdCtrlPtr != NULL_PTR)
        {
            if (0u
                == IStdLib_MemCmp(
                    (uint8*)&sesIdCtrlPtr->RemoteAddr,
                    (const uint8*)remoteAddrPtr,
                    sizeof(TcpIp_SockAddrType)))
            {
                break;
            }

            sesIdCtrlPtr = sesIdCtrlPtr->Next;
        }
    }

    return sesIdCtrlPtr;
}
#endif

/**
 * @brief              Check if the remote address is first recived
 * @param[in]          instanceId: Sd instance id.
 * @param[in]          remoteAddrPtr: Pointer for remote address buffer.
 * @return             TRUE/FALSE
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74159
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL boolean Sd_IsNewRemoteAddr(uint16 instanceId, const TcpIp_SockAddrType* remoteAddrPtr)
{
    boolean                     isNewRemoteAddr;
    const Sd_SessionIdCtrlType* sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, remoteAddrPtr);

    if (NULL_PTR != sesIdCtrlPtr)
    {
        isNewRemoteAddr = FALSE;
    }
    else
    {
        isNewRemoteAddr = TRUE;
    }

    return isNewRemoteAddr;
}
#endif

/**
 * @brief               Determine whether the offerservice message should be ignored.
 * @param[in]           cRtDataPtr Pointer to the client service runtime data.
 * @param[in]           clientServicePtr Pointer to the client service configuration.
 * @param[in]           rxQueueItem Pointer to the received queue item.
 * @return              TRUE/FALSE
 * @reentrant           TRUE
 * @synchronous         TRUE
 * @trace              CPD-74070
 */
#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL boolean Sd_IsOfferServiceBeIgnore(
    const Sd_ClientServiceRTType* cRtDataPtr,
    const Sd_ClientServiceType*   clientServicePtr,
    const Sd_RxQueueType*         rxQueueItem)
{
    /*@req <SWS_Sd_07016> */
    boolean                          bRet                  = FALSE;
    const Sd_ConsumedEventGroupType* consumedEventGroupPtr = clientServicePtr->SdConsumedEventGroup;

    StatusType ipAddrIsSame = IStdLib_MemCmp(
        (const uint8*)&cRtDataPtr->ServerUdpAddr,
        (const uint8*)&rxQueueItem->RemoteAddr,
        sizeof(TcpIp_SockAddrType));
    for (uint16 consumedEventGroupIdx = 0u; consumedEventGroupIdx < clientServicePtr->ConsumedEventGroupNum;
         consumedEventGroupIdx++)
    {
        const Sd_ConsumedEventGroupRTType* consumedEgRTPtr =
            Sd_ConsumedEventGroupRTData[consumedEventGroupPtr->SdConsumedEventGroupHandleId];

        if ((ipAddrIsSame != 0u) && consumedEgRTPtr->IsSubscribed)
        {
            bRet = TRUE;
            break;
        }

        consumedEventGroupPtr++;
    }

    return bRet;
}
#endif

/**
 * @brief              Add remote address to sessionid ctrl list.
 * @param[in]          instanceId: Instance Id
 * @param[in]          rxQueueItem: The item that would be appended to sessionCrtlList
 * @param[in]          isRxUseMulticast: Whether to Use Multicast to Receive Messages
 * @param[in]          applicationId Application ID.
 * @return             Pointer session crtl list item
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74160
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL Sd_SessionIdCtrlType* Sd_AddRemoteAddrToSessionIdCtrlList(
    uint16                instanceId,
    const Sd_RxQueueType* rxQueueItem,
    boolean               isRxUseMulticast,
    ApplicationType       applicationId)
{
    Sd_InstanceRTType*    instanceRtPtr    = Sd_InstanceRTData[instanceId];
    Sd_SessionIdCtrlType* sesIdCtrlPtr     = NULL_PTR;
    Sd_SessionIdCtrlType* lastSesIdCtrlPtr = NULL_PTR;
    TcpIp_SockAddrType    remoteAddr       = rxQueueItem->RemoteAddr;

    if (!TCPIP_IS_IPADDR_ANY(remoteAddr))
    {
        if (NULL_PTR != instanceRtPtr->SessionIdHandle.SesIdCtrlList)
        {
            sesIdCtrlPtr = instanceRtPtr->SessionIdHandle.SesIdCtrlList;
            while (sesIdCtrlPtr != NULL_PTR)
            {
                if (0u
                    == IStdLib_MemCmp(
                        (uint8*)&sesIdCtrlPtr->RemoteAddr,
                        (uint8*)&remoteAddr,
                        sizeof(TcpIp_SockAddrType)))
                {
                    break;
                }

                lastSesIdCtrlPtr = sesIdCtrlPtr;
                sesIdCtrlPtr     = sesIdCtrlPtr->Next;
            }
        }

        if (NULL_PTR == sesIdCtrlPtr)
        {
            /* PRQA S 0316 ++ */ /* VL_QAC_0316 */
            sesIdCtrlPtr =
                (Sd_SessionIdCtrlType*)Sd_MallocEx(sizeof(Sd_SessionIdCtrlType), Sd_RxMemPool[applicationId]);
            /* PRQA S 0316 -- */
            if (NULL_PTR != sesIdCtrlPtr)
            {
                sesIdCtrlPtr->Next          = NULL_PTR;
                sesIdCtrlPtr->RemoteAddr    = remoteAddr;
                sesIdCtrlPtr->TxUniSesIdCnt = 0u;
                Sd_HeaderType header;
                (void)IStdLib_MemCpy((uint8*)&header, rxQueueItem->BufPtr, sizeof(Sd_HeaderType));
                if (isRxUseMulticast)
                {
                    sesIdCtrlPtr->RxMultiSesIdCnt   = Sd_NtoHs(header.SessionID);
                    sesIdCtrlPtr->RxMultiRebootFlag = header.RebootFlag;
                    sesIdCtrlPtr->RxUniSesIdCnt     = 0u;
                    sesIdCtrlPtr->RxUniRebootFlag   = SD_REBOOT_FLAG_RESERVED; /* Not Updated */
                }
                else
                {
                    sesIdCtrlPtr->RxUniSesIdCnt     = Sd_NtoHs(header.SessionID);
                    sesIdCtrlPtr->RxUniRebootFlag   = header.RebootFlag;
                    sesIdCtrlPtr->RxMultiSesIdCnt   = 0u;
                    sesIdCtrlPtr->RxMultiRebootFlag = SD_REBOOT_FLAG_RESERVED; /* Not Updated */
                }
                sesIdCtrlPtr->ServerServiceList = NULL_PTR;
                sesIdCtrlPtr->ClientServiceList = NULL_PTR;

                if (NULL_PTR != instanceRtPtr->SessionIdHandle.SesIdCtrlList)
                {
                    lastSesIdCtrlPtr->Next = sesIdCtrlPtr; /* PRQA S 2813 */ /* VL_Sd_PointerIsNull */
                }
                else
                {
                    instanceRtPtr->SessionIdHandle.SesIdCtrlList = sesIdCtrlPtr;
                }
            }
        }
    }

    return sesIdCtrlPtr;
}
#endif

/**
 * @brief              Check if the remote device has rebooted
 * @param[in]          instanceId: Sd instance id.
 * @param[in]          rxQueueItem: Rx queue item.
 * @param[in]          isRxUseMulticast: Whether to use multicast to receive messages
 * @return             TRUE/FALSE
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74161
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL boolean Sd_RemoteRebootDetecte(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, boolean isRxUseMulticast)
{
    uint16                      lastSessionId;
    boolean                     isReboot = FALSE;
    uint8                       lastRebootFlag;
    const Sd_SessionIdCtrlType* sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, &rxQueueItem->RemoteAddr);

    if (NULL_PTR != sesIdCtrlPtr)
    {
        if (isRxUseMulticast)
        {
            lastSessionId  = sesIdCtrlPtr->RxMultiSesIdCnt;
            lastRebootFlag = sesIdCtrlPtr->RxMultiRebootFlag;
        }
        else
        {
            lastSessionId  = sesIdCtrlPtr->RxUniSesIdCnt;
            lastRebootFlag = sesIdCtrlPtr->RxUniRebootFlag;
        }

        Sd_HeaderType header;
        (void)IStdLib_MemCpy((uint8*)&header, rxQueueItem->BufPtr, sizeof(Sd_HeaderType));
        uint16 curSessionId = Sd_NtoHs(header.SessionID);
        /* @req SWS_SD_00446 */
        if (((0u == lastRebootFlag) && (1u == (uint8)header.RebootFlag))
            || ((1u == (uint8)header.RebootFlag) && (curSessionId <= lastSessionId)))
        {
            isReboot = TRUE;
        }
    }

    return isReboot;
}
#endif

/**
 * @brief              Free ServiceList in sessionId control list
 * @param[inout]       serviceList: Head pointer of session crtl list
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74162
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void
    Sd_FreeServiceListInSesIdCtrlList(const Sd_ServiceHandleIdListType* serviceList, ApplicationType applicationId)
{
    const Sd_ServiceHandleIdListType* serviceListCur  = serviceList;
    const Sd_ServiceHandleIdListType* serviceListNext = serviceListCur;

    while (NULL_PTR != serviceListNext)
    {
        serviceListCur  = serviceListNext;
        serviceListNext = serviceListNext->Next;
        Sd_FreeEx(serviceListCur, Sd_RxMemPool[applicationId]);
    }
}
#endif

/**
 * @brief              Delete the node from sessionId control list
 * @param[in]          head: Head pointer of session crtl list
 * @param[in]          objNode: The node expected to be deleted in the linked list
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74163
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void Sd_DeleteSessionIdNode(
    Sd_SessionIdCtrlType**      head,
    const Sd_SessionIdCtrlType* objNode,
    ApplicationType             applicationId)
{
    Sd_SessionIdCtrlType* qNode = NULL_PTR;

    if (NULL_PTR != *head)
    {
        Sd_SessionIdCtrlType* pNode = *head;
        while ((pNode != objNode) && (pNode->Next != NULL_PTR))
        {
            qNode = pNode;
            pNode = pNode->Next;
        }
        /*Object node is found*/
        if (pNode == objNode)
        {
            /*Object node is head*/
            if (pNode == *head)
            {
                *head = pNode->Next;
            }
            else
            {
                qNode->Next = pNode->Next; /* PRQA S 2813 */ /* VL_Sd_PointerIsNull */
            }
            Sd_FreeServiceListInSesIdCtrlList(pNode->ServerServiceList, applicationId);
            Sd_FreeServiceListInSesIdCtrlList(pNode->ClientServiceList, applicationId);
            Sd_FreeEx(pNode, Sd_RxMemPool[applicationId]);
        }
    }
}
#endif

/**
 * @brief              Detect remote device rebooted is or not.
 * @param[in]          instanceId: Sd instance id.
 * @param[in]          rxQueueItem: Rx queue item
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74164
 */
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
SD_LOCAL void
    Sd_RemoteRebootDetectedHandle(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, ApplicationType applicationId)
{
    Sd_InstanceRTType*          instanceRtPtr = Sd_InstanceRTData[instanceId];
    const Sd_SessionIdCtrlType* sesIdCtrlPtr  = Sd_FindRemoteAddrSessionIdCtrl(instanceId, &rxQueueItem->RemoteAddr);

    if (NULL_PTR != sesIdCtrlPtr)
    {
#if (SD_CLIENT_SERVICE_NUM > 0)
        const Sd_ServiceHandleIdListType* clientServiceList = sesIdCtrlPtr->ClientServiceList;
        /* @req SWS_SD_00448 */
        while (NULL_PTR != clientServiceList)
        {
            for (uint8 index = 0u; index < clientServiceList->HandleIdNum; index++)
            {
                Sd_RxStopOfferServiceHandle(clientServiceList->ServiceHandleId[index]);
            }
            clientServiceList = clientServiceList->Next;
        }
#endif

#if (SD_EVENT_HANDLER_NUM > 0)
        const Sd_ServiceHandleIdListType* serverServiceList = sesIdCtrlPtr->ServerServiceList;
        while (NULL_PTR != serverServiceList)
        {
            for (uint8 index = 0u; index < serverServiceList->HandleIdNum; index++)
            {
                Sd_RebootStopSubscribeEventHandle(
                    serverServiceList->ServiceHandleId[index],
                    rxQueueItem,
                    applicationId);
            }
            serverServiceList = serverServiceList->Next;
        }
#endif
        /* remove the found node form list*/
        Sd_DeleteSessionIdNode(&instanceRtPtr->SessionIdHandle.SesIdCtrlList, sesIdCtrlPtr, applicationId);
    }
}
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
/**
 * @brief              Remote device rebooted, execute stop offer service process.
 * @param[in]          clientServiceHandleId: Client service handle id.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74165
 */
SD_LOCAL void Sd_RxStopOfferServiceHandle(uint16 clientServiceHandleId)
{
    Sd_ClientServiceRTType*     cRtDataPtr       = Sd_ClientRTData[clientServiceHandleId];
    const Sd_ClientServiceType* clientServicePtr = &Sd_CfgPtr->GlbClientService[clientServiceHandleId];

    /*@req <SWS_SD_00367> */
    /* Stop the TTL timers of this Client Service Instance*/
    Sd_SetTTL(&cRtDataPtr->Ttl, 0UL);

    /* Report this Client Service as DOWN if it was reported AVAILABLE before */
    if (SD_CLIENT_SERVICE_AVAILABLE == cRtDataPtr->CurState)
    {
#if (SD_BSWM_USED == STD_ON)
        BswM_Sd_ClientServiceCurrentState(clientServicePtr->SdClientServiceHandleId, SD_CLIENT_SERVICE_DOWN);
#endif /* SD_BSWM_USED == STD_ON */
        cRtDataPtr->CurState = SD_CLIENT_SERVICE_DOWN;
    }

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    const Sd_ConsumedEventGroupType* eventGroupPtr = clientServicePtr->SdConsumedEventGroup;
    for (uint16 index = 0u; index < clientServicePtr->ConsumedEventGroupNum; index++)
    {
        Sd_ConsumedEventGroupRTType* consumedEgRTPtr =
            Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
        /* Stop the TTL timers of this Client Service Instance and all related Consumed Eventgroups*/
        Sd_SetTTL(&consumedEgRTPtr->Ttl, 0UL);

        /* Report all Consumed Eventgroups as DOWN that were reported AVAILABLE before */
        if (SD_CONSUMED_EVENTGROUP_AVAILABLE == consumedEgRTPtr->CurState)
        {
#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_ConsumedEventGroupCurrentState(
                eventGroupPtr->SdConsumedEventGroupHandleId,
                SD_CONSUMED_EVENTGROUP_DOWN);
#endif /* SD_BSWM_USED == STD_ON */
            consumedEgRTPtr->CurState = SD_CONSUMED_EVENTGROUP_DOWN;
        }

        eventGroupPtr++;
    }
#endif
    /* Close all SoCon and Routing */
    Sd_ClientServiceDisableAllRoutingOfClient(clientServicePtr, cRtDataPtr);
    Sd_ClientServiceCloseAllSoCon(clientServicePtr, cRtDataPtr);

    /*@req <SWS_Sd_00751> */
    /* PATCH for TC8: If not in main phase,when rx stopOfferService move to main phase */
    if (SD_CLIENT_PHASE_MAIN != cRtDataPtr->Phase)
    {
        cRtDataPtr->Phase            = SD_CLIENT_PHASE_MAIN;
        cRtDataPtr->InitialWaitTimer = 0u;
        cRtDataPtr->RepetitionCount  = 0u;
        cRtDataPtr->RepetitionTimer  = 0u;
    }
}
#endif

#if (SD_EVENT_HANDLER_NUM > 0)
/**
 * @brief              Remote device reboot, execute stop subscribe process.
 * @param[in]          serverServiceHandleId: Server service handle id.
 * @param[in]          applicationId Application ID.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74166
 */
SD_LOCAL void Sd_RebootStopSubscribeEventHandle(
    uint16                serverServiceHandleId,
    const Sd_RxQueueType* rxQueueItem,
    ApplicationType       applicationId)
{
    const Sd_ServerServiceType* serverServicePtr = &Sd_CfgPtr->GlbServerService[serverServiceHandleId];
    const Sd_EventHandlerType*  eventHandlerPtr  = serverServicePtr->SdEventHandler;

    for (uint16 index = 0u; index < serverServicePtr->EventHandlerNum; index++)
    {
        Sd_EventHandlerRTType* ehRTDataPtr = Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];

        /* Handle TCP subscribed client */
        const Sd_SubscribeManageType* subsMangPtr = NULL_PTR;
#if (SD_SERVER_SERVICE_USE_TCP == STD_ON)
        subsMangPtr = ehRTDataPtr->TcpSubscribeList;
        while (NULL_PTR != subsMangPtr)
        {
            if (subsMangPtr->SockRemoteAddr.addr[0] == rxQueueItem->RemoteAddr.addr[0])
            {
                /* Disable Routing Path*/
                const Sd_EventHandlerTcpType* eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
                if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
                {
                    SoAd_RoutingGroupIdType routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->RoutingGroupId;
                    (void)SoAd_DisableSpecificRouting(routingGroupId, subsMangPtr->SoConId);
                }

                ehRTDataPtr->TcpSubsClientNum--;
                Sd_DeleteClientFromSubscribeList(
                    serverServicePtr,
                    eventHandlerPtr,
                    &ehRTDataPtr->TcpSubscribeList,
                    NULL_PTR,
                    subsMangPtr->SoConId,
                    subsMangPtr->Counter,
                    applicationId);
            }

            subsMangPtr = subsMangPtr->Next;
        }
#endif

/* Handle UDP subscribed client */
#if (SD_SERVER_SERVICE_USE_UDP == STD_ON)
        subsMangPtr = ehRTDataPtr->UdpSubscribeList;
        while (NULL_PTR != subsMangPtr)
        {
            if (subsMangPtr->SockRemoteAddr.addr[0] == rxQueueItem->RemoteAddr.addr[0])
            {
                ehRTDataPtr->UdpSubsClientNum--;
                uint8 clientNum = Sd_GetClientNumFromSubscribeList(
                    ehRTDataPtr->UdpSubscribeList,
                    NULL_PTR,
                    subsMangPtr->SoConId,
                    SD_SUBSCRIBE_COUNTER_FIELD_RESERVED);
                if (clientNum == 1u)
                {
                    ehRTDataPtr->UdpSubsEndPointNum--;
                }

                /* Fan out control*/
                Sd_FanOutMulticastUnicastCtrlDelClient(eventHandlerPtr, ehRTDataPtr, subsMangPtr->SoConId);
                Sd_DeleteClientFromSubscribeList(
                    serverServicePtr,
                    eventHandlerPtr,
                    &ehRTDataPtr->UdpSubscribeList,
                    &subsMangPtr->ClientIpAddr,
                    subsMangPtr->SoConId,
                    subsMangPtr->Counter,
                    applicationId);
            }

            subsMangPtr = subsMangPtr->Next;
        }
#endif

        eventHandlerPtr++;
    }
}
#endif /* SD_EVENT_HANDLER_NUM > 0 */

/**
 * @brief              Check received Option is valid or not
 * @param[in]          sdMsg: Sd message buffer.
 * @return             E_OK: The option format check pass
 *                     E_NOT_OK: The option format check failure.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74167
 */
SD_LOCAL Std_ReturnType Sd_OptionFormatCheck(const uint8 sdMsg[])
{
    Std_ReturnType ret           = E_OK;
    uint32         entryArrayLen = Sd_NPtrtoHl(&sdMsg[SD_ENTRIES_ARRAY_LENGTH_INDEX]);

    if (0x00UL != (entryArrayLen & SD_16_BYTE_ALIGNED_FRACTION)) /* entryArrayLen must aligned with 16 */
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Option array length field index = SD_ENTRIES_ARRAY_LENGTH_INDEX(12u) + entryArrayLen + 4u*/
        uint32 optionLenFieldOffset = SD_MESSAGE_IPV4_OPTION_LEN + SD_MESSAGE_ENTRYLEN_FILED_LEN + entryArrayLen;
        uint32 optionArrayLen       = Sd_NPtrtoHl(&sdMsg[optionLenFieldOffset]);
        if (optionArrayLen > 0UL)
        {
            /* 1st option index = SD_FIRST_ENTRY_INDEX(16) + entryArrayLen + SD_LENGTH_OF_OPTIONS_ARRAY_FIELD_LEN(4)*/
            uint32 headerLen        = SD_HEADER_LEN + SD_MESSAGE_ENTRYLEN_FILED_LEN + SD_MESSAGE_OPTIONLEN_FILED_LEN;
            uint32 firstOptionIndex = headerLen + entryArrayLen;
            uint32 index            = firstOptionIndex;
            while (index < (optionArrayLen + firstOptionIndex))
            {
                Std_ReturnType optCheckRet = Sd_OptionEachItemFormatCheck(&sdMsg[index]);
                if (E_NOT_OK == optCheckRet)
                {
                    ret = E_NOT_OK;
                    break;
                }
                else
                {
                    uint16 optionLength = Sd_NPtrtoHs(&sdMsg[index]);
                    /* Next Option Index = index + optionLength + SD_MESSAGE_TYPE1_OPTION_FIRST_THREE_BYTE_LEN
                     * SD_MESSAGE_TYPE1_OPTION_FIRST_THREE_BYTE_LEN = SD_OPTION_LENGTH_FIELD_LEN(2) +
                     * SD_OPTION_TYPE_FIELD_LEN(1u) */
                    index = index + optionLength + SD_MESSAGE_TYPE1_OPTION_FIRST_THREE_BYTE_LEN;
                }
            }
        }
        else
        {
            ret = E_OK;
        }
    }

    return ret;
}

/**
 * @brief              Check each Option item is valid or not
 * @param[in]          optionArray: Option array.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74168
 */
SD_LOCAL Std_ReturnType Sd_OptionEachItemFormatCheck(const uint8 optionArray[])
{
    Std_ReturnType ret    = E_NOT_OK;
    uint16         length = Sd_NPtrtoHs(&optionArray[SD_LENGTH_OFFSET]);

    if (length > 0u)
    {
        uint16 port;
        uint8  l4Proto;
        uint8  type = optionArray[SD_TYPE_OFFSET];

        switch (type)
        {
        case SD_CONFIGURATION_OPTION:
            ret = E_OK;
            break;

        case SD_IPV4_ENDPOINT_OPTION:
        case SD_IPV4_MULTICAST_OPTION:
        case SD_IPV4_SD_ENDPOINT_OPTION:
            l4Proto = optionArray[SD_L4PROTO_OFFSET_IPV4];
            port    = Sd_NPtrtoHs(&optionArray[SD_PORTNUMBER_OFFSET_IPV4]);
            if ((SD_IPV4_OPTION_LENGTH_FIELD_VALUE == length)
                && ((SD_L4_PROTO_TCP == l4Proto) || (SD_L4_PROTO_UDP == l4Proto)) && (0x00u != port))
            {
                ret = E_OK;
            }
            break;

#if (STD_ON == SD_IPV6_ENABLE)
        case SD_IPV6_ENDPOINT_OPTION:
        case SD_IPV6_MULTICAST_OPTION:
        case SD_IPV6_SD_ENDPOINT_OPTION:
            l4Proto = optionArray[SD_L4PROTO_OFFSET_IPV6];
            port    = Sd_NPtrtoHs(&optionArray[SD_PORTNUMBER_OFFSET_IPV6]);
            if ((0x15u == length) && ((SD_L4_PROTO_TCP == l4Proto) || (SD_L4_PROTO_UDP == l4Proto)) && (0x00u != port))
            {
                ret = E_OK;
            }
            break;
#endif /* STD_ON == SD_IPV6_ENABLE */

        default:
            /* Nothing to do here */
            break;
        }
    }

    return ret;
}

/**
 * @brief              Call SoAd_IfSpecificRoutingGroupTransmit() in main function
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74169
 */
/* PRQA S 2812 ++ */ /* VL_Sd_PointerIsNull */
SD_LOCAL void Sd_IfSpecificRoutingGroupTransmit(ApplicationType applicationId)
{
    Sd_EventHandlerRTType* ehRTDataPtr = Sd_EventHandlerRTData[0u];

    for (uint16 index = 0u; index < SD_EVENT_HANDLER_NUM; index++)
    {
        while (ehRTDataPtr->TcpTriggerTransmitList != NULL_PTR)
        {
            const Sd_TriggerTransmitManagerType* tmpNode =
                Sd_PopFirstClientFromTriggerTransmitList(&ehRTDataPtr->TcpTriggerTransmitList);
            (void)SoAd_IfSpecificRoutingGroupTransmit(tmpNode->RoutingGroupId, tmpNode->SoConId);
            Sd_FreeEx(tmpNode, Sd_TxMemPool[applicationId]);
        }

        while (ehRTDataPtr->UdpTriggerTransmitList != NULL_PTR)
        {
            const Sd_TriggerTransmitManagerType* tmpNode =
                Sd_PopFirstClientFromTriggerTransmitList(&ehRTDataPtr->UdpTriggerTransmitList);
            (void)SoAd_IfSpecificRoutingGroupTransmit(tmpNode->RoutingGroupId, tmpNode->SoConId);
            Sd_FreeEx(tmpNode, Sd_TxMemPool[applicationId]);
        }

        ehRTDataPtr++; /* PRQA S 2983 */ /* VL_Sd_PointerCalu */
    }
}
/* PRQA S 2812 -- */

/**
 * @brief              If SoCon not used by Service(Include EventHandler),Reset it to wildcard
 * @param[in]          serverServicePtr: Pointer for server service.
 * @param[in]          soConId: Socket connection id
 * @param[in]          isTcpSoCon: Whether the socket connection type is tcp.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74171
 */
SD_LOCAL void
    Sd_ResetSoConToWildcard(const Sd_ServerServiceType* serverServicePtr, SoAd_SoConIdType soConId, boolean isTcpSoCon)
{
    boolean                       resetFlag = TRUE;
    const Sd_SubscribeManageType* subsMangPtr;
    const Sd_EventHandlerType*    eventHandlerPtr = serverServicePtr->SdEventHandler;
    for (uint16 idx = 0u; (idx < serverServicePtr->EventHandlerNum) && resetFlag; idx++)
    {
        const Sd_EventHandlerRTType* ehRTDataPtr = Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];

        if (isTcpSoCon)
        {
            subsMangPtr = ehRTDataPtr->TcpSubscribeList;
        }
        else
        {
            subsMangPtr = ehRTDataPtr->UdpSubscribeList;
        }

        while (NULL_PTR != subsMangPtr)
        {
            if (subsMangPtr->SoConId == soConId)
            {
                resetFlag = FALSE;
                break;
            }

            subsMangPtr = subsMangPtr->Next;
        }
        eventHandlerPtr++;
    }

    if (resetFlag)
    {
        /*@req <SWS_SD_00481> */
        /* If no subscription for this socket connection exists any more,
         * socket connection shall be reset to wildcard */
        SoAd_ReleaseRemoteAddr(soConId);
    }
}

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
/**
 * @brief              Reset all ConsumedEventgroup runtime data isTcpSubsSend/isUdpSubsSend
                       flag in Client Service
 * @param[in]          clientServicePtr: Pointer for client service.
 * @param[in]          tcpFlag: Flag indicating whether to reset the TCP subscription send flag.
 * @param[in]          udpFlag: Flag indicating whether to reset the UDP and multicast subscription send flags.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74172
 */
SD_LOCAL void Sd_ClientServiceResetAllCEgSubsEntrySendFlag(
    const Sd_ClientServiceType* clientServicePtr,
    boolean                     tcpFlag,
    boolean                     udpFlag)
{
    const Sd_ConsumedEventGroupType* cEgPtr = clientServicePtr->SdConsumedEventGroup;
    for (uint16 index = 0u; index < clientServicePtr->ConsumedEventGroupNum; index++)
    {
        Sd_ConsumedEventGroupRTType* cEgRTPtr = Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
        if (tcpFlag)
        {
            cEgRTPtr->IsTcpSubsSend = FALSE;
        }
        if (udpFlag)
        {
            cEgRTPtr->IsUdpSubsSend       = FALSE;
            cEgRTPtr->IsMulitcastSubsSend = FALSE;
        }

        cEgPtr++;
    }
}

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
/**
 * @brief              Reset all ConsumedEventgroup runtime data subsRetryCnt/subsRetryTimer/
                       subsRetryEnable in Client Service
 * @param[in]          clientServicePtr: Pointer for client service.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74173
 */
SD_LOCAL void Sd_ClientServiceResetAllCEgSubsEntrySubsRetryCnt(const Sd_ClientServiceType* clientServicePtr)
{
    const Sd_ConsumedEventGroupType* cEgPtr = clientServicePtr->SdConsumedEventGroup;
    for (uint16 index = 0u; index < clientServicePtr->ConsumedEventGroupNum; index++)
    {
        Sd_ConsumedEventGroupRTType* cEgRTPtr = Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
        cEgRTPtr->SubsRetryCnt                = 0u;
        cEgRTPtr->SubsRetryTimer              = 0u;
        cEgRTPtr->SubsRetryEnable             = FALSE;

        cEgPtr++;
    }
}
#endif
#endif

/**
 * @brief              SendTime value adjust,when sendTime equal 0,set it to SD_SEND_ENTRY_IMMEDIATELY
 * @param[in]          sendTime: The delay time for send message
 * @return             Send delay time
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-74174
 */
SD_LOCAL_INLINE uint32 Sd_InitialWaitTimeAdjust(uint32 waitTime)
{
    return ((waitTime == 0u) ? 1u : waitTime);
}

#define SD_STOP_SEC_CODE
#include "Sd_MemMap.h"

#endif
