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
**  FILENAME    : SD.c                                                        **
**                                                                            **
**  Created on  : 2019-03-11                                                  **
**  Author      : HuRongbo                                                    **
**  Vendor      : iSoft                                                       **
**  DESCRIPTION : Implementation Of SD                                        **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/***************************Change History*************************************/
/* V0.1.0   2019/3/11   hurongbo
            Initial Version
   V0.2.0   2019/11/25  hurongbo
            Fix bugs find in Unit test
   V0.3.0   2019/12/09  hurongbo
            1> Require 00440 only execute when eventGroup belong's client is REQUIRED state
            2> Sd_MainFunction,Report DET error,Function ID change
            3> Sd_ParseRxMessage,option start index calculation consider memory alignment
            4> Require 00333 and 00344 only execute when response with ACK
            5> Sd_BuildSubscribeEventgroupAckEntry,add ttl cast with Sd_HtoNl()
            6> Change Require 00704 processing logic
            7> Report fail to DEM if allocate memory failed
   V0.4.0   2019/12/13 hurongbo
            1> Change Sd_ConsumedEventGroupRTData[x].isAnsweredWithAck from FALSE to TRUE in Sd_Init()
            2> When build Configuation option,not force 4 bytes alignment
            3> Sd_TransmitUnicastMessage() free allocated memory add condition
            4> Malloc fail report to DEM condition add SD_MALLOC_FAIL_REPORT_ENABLE == STD_ON
   V0.5.0   2020/01/16 hurongbo
            Fix bugs find in Integration Test
   V0.6.0   2020/03/13 hurongbo
            When Server receive StopSubscribeEventgroup,not response ACK any more
   V2.0.0   2020/09/07
            Update With R19-11 specific
   V2.0.1   2020/12/07
            QAC check,Warning and error fix
   V2.0.2   2021/08/12
            Fix bugs find in TC8 test
   V2.0.3   2022/01/19
            1> When receive FindService whit Unicast Flag set to 0,Ignore.(SWS_SD_00650,PRS_SOMEIPSD_00423)
            2> Find messages received with the Unicast Flag set to 1, shall be answered with a multicast
               RESPONSE if the last offer was sent 1/2 CYCLIC_OFFER_DELAY or longer ago.(PRS_SOMEIPSD_00423)
            3> Check Sd header field:Protocol Version,Interface Version,Message Type and Return Code.
   V2.0.4   2022/04/08
            Fix some SD issues found in APCP fusion testing.
   V2.0.5   2022/04/24
            Add Sd_DeleteSessionIdNode, delete session id node when reboot occur.
            Remove unused variable parseEntryFlag.
            Save remote sessionId only when reboot not occur.
   V2.0.6   2022/07/07 hurongbo
            Fix bug:When not config SdEventHandler,may cause compile error(JIRA:PRE0012022-31)
   V2.0.7   2022/07/11 hurongbo
            QAC check issue fix
   V2.0.8   2022/07/25 hurongbo
            1> Resolve SubscribeEventgroup retry message send out first than
               SubscribeEventgroup message(JIRA:PRE0012022-78)
            2> Resolve When receive OfferService TCP or UDP state not ONLINE,
               Can not response correctly(JIRA:PRE0012022-77)
            3> Resolve build SubscribeEventGroup entry error(JIRA:PRE0012022-76)
            4> Resolve when receive SubscribeEventGroupNAck sometimes,enter hardfault
               (JIRA:PRE0012022-74)
   V2.0.9   2022/07/26
            Add callback API Sd_TxConfirmation.
   V2.0.10  2022/08/03
            Use uint32 variables to temporarily store TTL instead of uint8 buffers to
             avoid bytes without 4-byte alignment.
   V2.0.11  2022/08/26 hurongbo
            1> Fix conflict options check bug:when have two same Endpoint option,
               return check pass,should return not pass(JIRA:PRE0012022-102)
            2> Fix bug:macro SD_BSWM_USED deactive too much code in some case
               (JIRA:PRE0012022-106)
            3> In Sd_RebootStopSubscribeEventHandle() can't deactive Tcp Event
               routing path correctly(JIRA:PRE0012022-108,PRE0012022-109)
            4> When rx SubscribeEventgroupNAck can't stop sent SubscribeEventgroup
               retry(JIRA:PRE0012022-107)
            5> Resolve when TCP event subscribe success,UDP fail,response with
               SubscribeEventGroupNAck,but open TCP RoutingPath.(JIRA:PRE0012022-137)
   V2.0.12  2022/09/21 hurongbo
            1> Delete Det check in Sd_MainFunction() (JIRA:PRD0032020-219)
   V2.0.13  2022/09/21 hurongbo
            1> Modify client service subscribe event mechanism,resolve receive
               separate UDP/TCP endpoint OfferEntry can't correct subscribe issue
               (JIRA: PRE0012022-174)
            2> Resolve when receive SubscribeEvnetAck continuous,Enter HardFault
               (JIRA: PRE0012022-301)
   V2.0.14  2022/12/26 zhiqiang.huang
            Add initialization of some variables in function Sd_ClientSaveMulticastSoConId
            (JIRA: #PRD0042022-517)
   V2.0.15  2023/03/31 hurongbo
            1> Modify necessary options check logic (JIRA:CPT-81)
            2> fix the problem of sending SubscribeEventgroup message before the
               TCP connection is successful (JIRA:CPT-82)
            3> Add macro to solve compilation warning problem
   V2.0.16  2023/07/21 hurongbo
            1> Modify TTL process in server side,TTL parameter does not affect the state
               of the Server (JIRA:CPT-6227)
   V2.0.17  2023/08/07 hurongbo
            1> Modify Sd_InstanceType define,add SdInstanceTxPduRef parameter for JIRA:CPT-6449
   V2.0.18  2023/09/12 hurongbo
            1> Modify the init value of txUniSesIdCnt member when create Sd_SessionIdCtrlType
               variable (JIRA:CPT-6633)
   V2.0.19  2023/09/19 hurongbo
            1> Add judge when use Sd_ConsumedEventGroupType pointer avoid NULL_PTR ref(JIRA:CPT-6917)
            2> Add conditional compilation to revolve Client no ConsumedEventgroup cause compile error
   V2.0.20  2023/11/17 hurongbo
            1> Clear send queue to avoid OfferService after a StopOfferService (JIRA:CPT-7263)
            2> Fix error when unaligned address cast to data pointer to read data (JIRA:CPT-7607)
   V2.0.21  2023/11/21 hurongbo
            1> Code execution efficiency optimization (JIRA:CPD-33590)
            2> QAC warning handle
   V2.0.22  2024/01/12 hurongbo
            1> Fixed issue where allocated memory could not be released under certain conditions (JIRA:CPT-8107)
   V2.0.23  2024/01/24 hurongbo
            1> Replace standard library functions to iSoft library functions
   V2.0.24  2024/03/06 hurongbo
            1> QAC check use new rule set
   V2.0.25  2024/5/14 zhijiazou
            1> Delete callback API Sd_TxConfirmation
            2> Support continuous sending of Sd messages
   V2.0.26  2024/7/17 hurongbo
            1> Remove the ues of Frt module function
*/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Sd<br>
  RuleSorce:puhua-rule.rcf 2.3.1

    \li PRQA S 2877 MISRA Dir 4.1.<br>
    Reason:The exception is that the number of loops is configurable.

    \li PRQA S 2995, 2991 MISRA Rule 2.2, 14.3.<br>
    Reason:The exception is that the value of a conditional expression is determined by configuration.

    \li PRQA S 0635 MISRA Rule 6.1.<br>
    Reason:It will be fixed in a later version.

    \li PRQA S 1531, 1532 MISRA Rule 8.7 .<br>
    Reason:The exception is global configuration data(1531) and API(1532).

    \li PRQA S 0686 MISRA Rule 9.3 .<br>
    Reason:Resolve compilation warnings.
 */
#include "Sd.h"
#include "Sd_Cfg.h"
#if SD_MODULE_ENABLE == STD_ON

/*******************************************************************************
**                      Version Information Definitions                       **
*******************************************************************************/
#define SD_C_AR_RELEASE_MAJOR_VERSION    4U
#define SD_C_AR_RELEASE_MINOR_VERSION    5U
#define SD_C_AR_RELEASE_REVISION_VERSION 0U

#define SD_C_SW_MAJOR_VERSION            2U
#define SD_C_SW_MINOR_VERSION            0U
#define SD_C_SW_PATCH_VERSION            26U

/*******************************************************************************
**                               Includes                                     **
*******************************************************************************/
#include "SoAd.h"
#if (SD_BSWM_USED == STD_ON)
#include "BswM_Sd.h"
#endif
#include "Det.h"
#if (SD_DEM_USED == STD_ON)
#include "Dem.h"
#endif
#include "istd_lib.h"
#include <string.h>

/*******************************************************************************
**                             Version Check                                  **
*******************************************************************************/
#if (SD_C_AR_RELEASE_MAJOR_VERSION != SD_H_AR_RELEASE_MAJOR_VERSION)
#error "Sd.c : Mismatch in Specification Major Version"
#endif
#if (SD_C_AR_RELEASE_MINOR_VERSION != SD_H_AR_RELEASE_MINOR_VERSION)
#error "Sd.c : Mismatch in Specification Minor Version"
#endif
#if (SD_C_AR_RELEASE_REVISION_VERSION != SD_H_AR_RELEASE_REVISION_VERSION)
#error "Sd.c : Mismatch in Specification Revision Version"
#endif
#if (SD_C_SW_MAJOR_VERSION != SD_H_SW_MAJOR_VERSION)
#error "Sd.c : Mismatch in Specification Major Version"
#endif
#if (SD_C_SW_MINOR_VERSION != SD_H_SW_MINOR_VERSION)
#error "Sd.c : Mismatch in Specification Minor Version"
#endif

/*******************************************************************************
**                            Macros Definitions                              **
*******************************************************************************/

/* use for utest*/
#if !defined(SD_LOCAL)
#define SD_LOCAL static
#endif

/* inline keyword define,if compiler not support inline,define _SD_INLINE_ to null */
#define SD_LOCAL_INLINE static inline

/* Sd support IPv6 switch,Now TCPIP not support IPv6, the parameter should set to STD_OFF */
#define SD_IPV6_ENABLE STD_OFF

/* SOME/IP-SD message head field fill value define */
#define SD_CLIENT_ID_PADDING_VALUE         0x00u
#define SD_PROTOCAL_VERSION_PADDING_VALUE  0x01u
#define SD_INTERFACE_VERSION_PADDING_VALUE 0x01u
#define SD_MESSAGE_TYPE_PADDING_VALUE      0x02u
#define SD_RETURN_CODE_PADDING_VALUE       0x00u

/* Type 1 entries(for Service) Type field fill value define */
#define SD_ENTRY_TYPE_FIND_SERVICE  0x00u
#define SD_ENTRY_TYPE_OFFER_SERVICE 0x01u
/* Private define for implement */
#define SD_ENTRY_TYPE_STOP_OFFER_SERVICE 0xF1u

/* Type 2 entries(for Eventgroup) Type field fill value define */
#define SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP    0x06u
#define SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK 0x07u

/* Option type define */
#define SD_CONFIGURATION_OPTION    0x01u
#define SD_IPV4_ENDPOINT_OPTION    0x04u
#define SD_IPV6_ENDPOINT_OPTION    0x06u
#define SD_IPV4_MULTICAST_OPTION   0x14u
#define SD_IPV6_MULTICAST_OPTION   0x16u
#define SD_IPV4_SD_ENDPOINT_OPTION 0x24u
#define SD_IPV6_SD_ENDPOINT_OPTION 0x26u

/* Options length define */
#define SD_IPV4_ENDPOINT_OPTION_LENGTH    0x09u
#define SD_IPV6_ENDPOINT_OPTION_LENGTH    0x15u
#define SD_IPV4_MULTICAST_OPTION_LENGTH   0x09u
#define SD_IPV6_MULTICAST_OPTION_LENGTH   0x15u
#define SD_IPV4_SD_ENDPOINT_OPTION_LENGTH 0x09u
#define SD_IPV6_SD_ENDPOINT_OPTION_LENGTH 0x15u

/* Layer 4 Protocol field (L4-Proto) define */
#define SD_L4_PROTO_TCP 0x06u
#define SD_L4_PROTO_UDP 0x11u

/* Reserved field fill value */
#define SD_RESERVED_FIELD_VALUE 0x00u

#if (STD_ON == SD_DEV_ERROR_DETECT)
#define SD_REPORT_ERROR(ApiId, ErrorId) (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, (ApiId), (ErrorId));
#endif

#define SD_REPORT_RUNTIMEERROR(ApiId, ErrorId) \
    (void)Det_ReportRuntimeError(SD_MODULE_ID, SD_INSTANCE_ID, (ApiId), (ErrorId));

/* Position of "Length of Entries Array" in SD message */
#define SD_ENTRIES_ARRAY_LENGTH_INDEX        12u
#define SD_FIRST_ENTRY_INDEX                 16u
#define SD_ENTRY_SIZE                        16u
#define SD_LENGTH_OF_OPTIONS_ARRAY_FIELD_LEN 4u
#define SD_OPTION_LENGTH_FIELD_LEN           2u
#define SD_OPTION_TYPE_FIELD_LEN             1u

/* Each field offset in Option */
#define SD_LENGTH_OFFSET           0u
#define SD_TYEP_OFFSET             2u
#define SD_IPADDR_OFFSET           4u
#define SD_L4PROTO_OFFSET_IPV4     9u
#define SD_PORTNUMBER_OFFSET_IPV4  10u
#define SD_L4PROTO_OFFSET_IPV6     21u
#define SD_PORTNUMBER_OFFSET_IPV6  22u

#define SD_HEADER_FLAGS_INDEX      8u
#define SD_FLAGS_UNICAST_FLAG_MASK 0x40u

/* define max entries can be send in one frame */
/* Notes: The larger this macro, the larger stack usage when runing,about 24*SD_MAX_ENTRIES_PER_FRAME Bytes*/
#define SD_MAX_ENTRIES_PER_FRAME 20u

/* Send the entry immediately */
#define SD_SEND_ENTRY_IMMEDIATELY 1u

/* Macro for random calculate */
#define SD_RANDOM_MULTIPLIER 1664525U
#define SD_RANDOM_INCREMENT  1013904223U

#define SD_UNUSED(x)         (void)(x)
/*******************************************************************************
**                          Runtime Type Definitions                          **
*******************************************************************************/
/* PRQA S 0635 ++ */ /* MISRA Rule 6.1 */
/* SOME/IP-SD Header Type*/
typedef struct
{
    uint16 ClientID;
    uint16 SessionID;
    uint8 ProtocolVersion;
    uint8 InterfaceVersion;
    uint8 MessageType;
    uint8 ReturnCode;
#if CPU_BYTE_ORDER == HIGH_BYTE_FIRST
    uint8 RebootFlag : 1;
    uint8 UnicastFlag : 1;
    uint8 : 6;
#else
    uint8 : 6;
    uint8 UnicastFlag : 1;
    uint8 RebootFlag : 1;
#endif
    uint8 Reserved[3];
} Sd_HeaderType;

/* Entry Format Type 1 */
typedef struct
{
    uint8 Type;
    uint8 Index1stOptions;
    uint8 Index2ndOptions;
#if CPU_BYTE_ORDER == HIGH_BYTE_FIRST
    uint8 NumOfOpt1 : 4;
    uint8 NumOfOpt2 : 4;
#else
    uint8 NumOfOpt2 : 4;
    uint8 NumOfOpt1 : 4;
#endif
    uint16 ServiceID;
    uint16 InstanceID;
    uint8 MajorVersion;
    uint8 TTL[3];
    uint32 MinorVersion;
} Sd_Type1EntryType;

/* Entry Format Type 2 */
typedef struct
{
    uint8 Type;
    uint8 Index1stOptions;
    uint8 Index2ndOptions;
#if CPU_BYTE_ORDER == HIGH_BYTE_FIRST
    uint8 NumOfOpt1 : 4;
    uint8 NumOfOpt2 : 4;
#else
    uint8 NumOfOpt2 : 4;
    uint8 NumOfOpt1 : 4;
#endif
    uint16 ServiceID;
    uint16 InstanceID;
    uint8 MajorVersion;
    uint8 TTL[3];
    uint8 : 8; /*Reserved High 8bit*/
#if CPU_BYTE_ORDER == HIGH_BYTE_FIRST
    uint8 : 4; /*Reserved Low 4bit*/
    uint8 Counter : 4;
#else
    uint8 Counter : 4;
    uint8 : 4; /*Reserved Low 4bit*/
#endif
    uint16 EventGroupID;
} Sd_Type2EntryType;

/* Used to create IPv4 Endpoint option, IPv4 Multicast option,IPv4 SD Endpoint option */
typedef struct
{
    uint16 Length;
    uint8 Type;
    uint8 : 8; /* Reserved */
    uint8 IPv4Address[4];
    uint8 : 8; /* Reserved */
    uint8 L4Proto;
    uint16 PortNumber;
} Sd_IPv4OptionsType;

/* Used to create IPv6 Endpoint option, IPv6 Multicast option, IPv6 SD Endpoint option */
typedef struct
{
    uint16 Length;
    uint8 Type;
    uint8 : 8; /* Reserved */
    uint8 IPv6Address[16];
    uint8 : 8; /* Reserved */
    uint8 L4Proto;
    uint16 PortNumber;
} Sd_IPv6OptionsType;
/* PRQA S 0635 -- */ /* MISRA Rule 6.1 */

typedef enum
{
    SD_UNINIT,
    SD_INIT
} Sd_StatusType;

typedef enum
{
    SD_INIT_FAIL,
    SD_INIT_SUCCESS
} Sd_InitResultType;

typedef enum
{
    SD_SERVER_PHASE_DOWN,
    SD_SERVER_PHASE_INITIAL_WAIT,
    SD_SERVER_PHASE_REPETITION,
    SD_SERVER_PHASE_MAIN,
} Sd_ServerServicePhaseType;

typedef enum
{
    SD_CLIENT_PHASE_DOWN,
    SD_CLIENT_PHASE_INITIAL_WAIT,
    SD_CLIENT_PHASE_REPETITION,
    SD_CLIENT_PHASE_MAIN,
} Sd_ClientServicePhaseType;

typedef struct
{
    uint64 ttlms;
    boolean isValidForever; /* ttl = 0xFFFFFF */
} Sd_TTLType;

typedef struct Sd_SendQueueType_
{
    TcpIp_SockAddrType destAddr;
    uint8* entryPtr;
    uint8* optionPtr;
    uint32 sendTime;
    struct Sd_SendQueueType_* next;
    uint16 optStartPos[3]; /* Max 3 options */
    uint16 optionLen;
    uint8 optionNum;
    boolean isUnicastSend;
} Sd_SendQueueType;

/* Server service run time type */
typedef struct
{
    uint32 initialWaitTimer;
    uint32 repetitionTimer;
    uint32 mainTimer;
    Sd_SendQueueType* head;
    Sd_ServerServiceSetStateType setState;
    Sd_ServerServicePhaseType phase;
    Sd_TTLType ttl;
    uint8 repetitionCount;
} Sd_ServerServiceRTType;

typedef struct
{
    SoAd_SoConIdType soConId;
    boolean isOptRcved : 1;
    boolean isSoConOpened : 1;
} Sd_SoConManageType;

/* Client service run time type */
typedef struct
{
    TcpIp_SockAddrType serverUdpAddr; /* For ConsumedEventgroup Multicast Receive */
    TcpIp_SockAddrType offerEntryRemoteAddr;
    PduIdType offerEntryRxPduId;
    Sd_TTLType ttl;
    Sd_SoConManageType tcpSoCon;
    Sd_SoConManageType udpSoCon;
    uint32 initialWaitTimer;
    uint32 repetitionTimer;
    uint32 mainTimer;
    Sd_SendQueueType* head;
    Sd_ClientServiceSetStateType setState;
    Sd_ClientServiceCurrentStateType curState;
    Sd_ClientServicePhaseType phase;
    uint8 repetitionCount;
    boolean offerServiceRxFlag;
    boolean rxTcpEndpointFlag;
    boolean rxUdpEndpointFlag;
} Sd_ClientServiceRTType;

typedef struct Sd_SubscribeManageType_
{
    Sd_TTLType ttl;
    struct Sd_SubscribeManageType_* next;
    TcpIp_SockAddrType sockRemoteAddr;
    SoAd_SoConIdType soConId;
    uint8 counter;
} Sd_SubscribeManageType;

typedef struct
{
    Sd_SubscribeManageType* udpSubscribeList;
    Sd_SubscribeManageType* tcpSubscribeList;
    Sd_EventHandlerCurrentStateType curState;
    uint8 udpSubsClientNum; /* Max equal 16 */
    uint8 tcpSubsClientNum; /* Max equal 16 */
    boolean ifSpecificRoutingTransUdpFlag;
    SoAd_SoConIdType soConIdUdp;
    SoAd_RoutingGroupIdType routingGroupIdUdp;
    boolean ifSpecificRoutingTransTcpFlag;
    SoAd_SoConIdType soConIdTcp;
    SoAd_RoutingGroupIdType routingGroupIdTcp;
} Sd_EventHandlerRTType;

typedef struct Sd_RxQueueType_
{
    TcpIp_SockAddrType remoteAddr;
    uint8* bufPtr;
    struct Sd_RxQueueType_* next;
    PduIdType rxPduId;
    PduLengthType pduLength;
} Sd_RxQueueType;

typedef struct
{
    uint16 subsRetryTimer;
    uint8 subsRetryCnt;
    TcpIp_SockAddrType remoteAddr;
    Sd_TTLType ttl;
    Sd_SoConManageType multiSoCon;
    Sd_ConsumedEventGroupSetStateType setState;
    Sd_ConsumedEventGroupCurrentStateType curState;
    boolean isSubscribed;
    boolean isTcpSubsSend;
    boolean isUdpSubsSend;
    boolean isAnsweredWithAck;
    boolean isSendSubEvtAfterRequest;
    boolean subsRetryEnable;
} Sd_ConsumedEventGroupRTType;

typedef struct Sd_ServiceHandleIdListType_
{
    uint16 serviceHandleId[16]; /* Memory fragmentation avoid, allocate 16 member each time */
    uint8 handleIdNum;          /* serviceHandleId array used buffer number */
    struct Sd_ServiceHandleIdListType_* next;
} Sd_ServiceHandleIdListType;

typedef struct Sd_SessionIdCtrlType_
{
    TcpIp_SockAddrType remoteAddr;
    uint16 txUniSesIdCnt;
    uint16 rxUniSesIdCnt;
    uint16 rxMultiSesIdCnt;
    uint8 rxUniRebootFlag;
    uint8 rxMultiRebootFlag;
    Sd_ServiceHandleIdListType* serverServiceList;
    Sd_ServiceHandleIdListType* clientServiceList;
    struct Sd_SessionIdCtrlType_* next;
} Sd_SessionIdCtrlType;

typedef struct
{
    Sd_SessionIdCtrlType* sesIdCtrlList; /* Unicast tx session Id counter handle list header*/
    uint16 txMultiSesIdCnt;              /* Multicast tx session ID Counter */
} Sd_SessionIdHandleType;

typedef struct
{
    Sd_SessionIdHandleType sessionIdHandle;
    TcpIp_IpAddrStateType ipAddrState;
    boolean rebootFlag;
} Sd_InstanceRTType;
/*******************************************************************************
**                        Private Function Declarations                       **
*******************************************************************************/
#define SD_START_SEC_CODE
#include "Sd_MemMap.h"

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
static void Sd_ParseRxMessage(void);

static Std_ReturnType Sd_OptionsSecurityCheck(
    const Sd_InstanceType* instancePtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap);

static Std_ReturnType Sd_IsIpAddrInSameSubset(const uint8* ipAddr1, const uint8* ipAddr2, uint8 netmask, uint8 addrLen);

static Std_ReturnType Sd_OptionsConflictCheck(const uint8* sdMsg, uint16 entryPos, const uint16* optStartPosMap);

static Std_ReturnType Sd_OptionsExistCheck(
    const Sd_InstanceType* instancePtr,
    const uint8* sdMsg,
    uint32 entryArrayLen,
    uint8 optNum);
#endif /* SD_SERVER_SERVICE_NUM > 0 || SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_SERVER_SERVICE_NUM > 0)
static void Sd_RxFindServiceHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    boolean isRxUseMulticast,
    const uint16* optStartPosMap);

static uint32 Sd_CalcServerServiceDelayTime(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    boolean isRxUseMulticast);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
static void Sd_RxOfferServiceHandle(
    const Sd_InstanceType* instancePtr,
    Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    const uint16* optStartPosMap);
#endif

#if (SD_EVENT_HANDLER_NUM > 0)
static void Sd_RxSubscribeEventgroupHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    const uint16* optStartPosMap,
    boolean isReplyNAck);
#endif

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
static void Sd_RxSubscribeEventgroupAckHandle(
    const Sd_InstanceType* instancePtr,
    Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    const uint16* optStartPosMap);

static Std_ReturnType Sd_GetInfoFromMulticastOption(
    const Sd_InstanceType* instancePtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap,
    TcpIp_SockAddrType* ipAddrPtr);

static boolean Sd_IsHaveEventGroupAckInSameMessage(
    const uint8* sdMsg,
    uint16 serviceID,
    uint16 instanceID,
    uint16 eventgroupID,
    uint8 majorVersion,
    uint8 counter);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
static Std_ReturnType Sd_BuildFindServiceEntry(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ClientServiceType* clientServicePtr);
#endif

#if (SD_SERVER_SERVICE_NUM > 0)
static void Sd_BuildOfferServiceEntry(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint32 sendTime,
    boolean isUnicastSend,
    boolean isStopOfferService);
#endif

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
static Std_ReturnType Sd_BuildSubscribeEventgroupEntry(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const TcpIp_SockAddrType* remoteAddr,
    Sd_ClientServiceRTType* cRtDataPtr,
    boolean isRxUseMulticast,
    uint32* sendTime);

static Std_ReturnType Sd_BuildStopSubscribeEventgroupEntry(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const TcpIp_SockAddrType* remoteAddr,
    Sd_ClientServiceRTType* cRtDataPtr,
    boolean isRxUseMulticast);

static uint32 Sd_CalcClientServiceDelayTime(const Sd_ConsumedEventGroupType* eventHandlerPtr, boolean isRxUseMulticast);
#endif

#if (SD_EVENT_HANDLER_NUM > 0)
static Std_ReturnType Sd_BuildSubscribeEventgroupAckEntry(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const TcpIp_SockAddrType* remoteAddr,
    uint32 ttl,
    uint8 counter);

static Std_ReturnType Sd_BuildSubscribeEventgroupNAckEntry(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint16 entryPos);
#endif

#if (SD_SERVER_SERVICE_NUM > 0)
static Sd_Type1EntryType* Sd_CreateOfferServiceEntry(
    const Sd_ServerServiceType* serverServicePtr,
    boolean isStopOfferService);

static uint8* Sd_CreateOfferServiceOptions(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    Sd_SendQueueType* sendQueueObjPtr);
#endif

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
static uint16 Sd_GetType1ConfigurationOptionLength(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr);

static void Sd_BuildType1ConfigurationOption(
    uint8* optAddrPtr,
    uint16 cfgOptLen,
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0) || (SD_EVENT_HANDLER_NUM > 0)
static uint16 Sd_GetType2ConfigurationOptionLength(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr);

static void Sd_BuildType2ConfigurationOption(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_ClientServiceType* clientServicePtr,
    uint8* optAddrPtr,
    uint16 cfgOptLen);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0) || (SD_EVENT_HANDLER_NUM > 0 */

static void Sd_SetHeader(
    uint8* headerPtr,
    uint16 instanceId,
    const TcpIp_SockAddrType* remoteAddr,
    boolean isMulticastTx);

static void Sd_SetIPv4EndpointOption(uint8* ipv4OptionPtr, const uint8* ipv4Address, uint8 L4Proto, uint16 portNumber);

#if (STD_ON == SD_IPV6_ENABLE)
static void Sd_SetIPv6EndpointOption(uint8* ipv6OptionPtr, const uint8* ipv6Address, uint8 L4Proto, uint16 portNumber);
#endif /* STD_ON == SD_IPV6_ENABLE */

#if (SD_EVENT_HANDLER_NUM > 0)
static void Sd_SetIPv4MulticastOption(uint8* ipv4OptionPtr, const uint8* ipv4Address, uint16 portNumber);

#if (STD_ON == SD_IPV6_ENABLE)
static void Sd_SetIPv6MulticastOption(uint8* ipv6OptionPtr, const uint8* ipv6Address, uint16 portNumber);
#endif /* STD_ON == SD_IPV6_ENABLE */
#endif /* SD_EVENT_HANDLER_NUM > 0 */

SD_LOCAL_INLINE void Sd_SetTTL(Sd_TTLType* ttlPtr, uint32 ttl);
#endif

#if (SD_SERVER_SERVICE_NUM > 0)
static void Sd_ServerServiceDownPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr);

static void Sd_ServerServiceInitialWaitPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr);

static void Sd_ServerServiceRepetitionPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr);

static void Sd_ServerServiceMainPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr);

static void Sd_ServerServiceDownPhaseEnter(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr);

static void Sd_ServerServiceInitialWaitEnter(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr);

static void Sd_ServerServiceTimerHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
static void Sd_ClientServiceDownPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr);

static void Sd_ClientServiceInitialWaitPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr);

static void Sd_ClientServiceRepetitionPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr);

static void Sd_ClientServiceMainPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr);

static void Sd_ClientServiceInitialWaitEnter(
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType* cRtDataPtr);

static void Sd_ClientServiceDownPhaseEnter(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType* cRtDataPtr);

static void Sd_ClientServiceDisableAllRoutingOfClient(
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType* cRtDataPtr);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
static void Sd_ClientServiceDisableAllConsumedEventgroupRouting(
    const Sd_ConsumedEventGroupType* cEgPtr,
    Sd_ClientServiceRTType* cRtDataPtr);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

static void Sd_ClientServiceCloseAllSoCon(
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType* cRtDataPtr);

static void Sd_ClientServiceCloseSoCon(Sd_SoConManageType* soConPtr);
#endif

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
SD_LOCAL_INLINE void Sd_EndianSwap(uint8* dataPtr, uint8 size);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

static uint32 Sd_GetRandomValue(uint32 min, uint32 max);

static uint32 Sd_NtoHl(uint32 net32);

static uint32 Sd_NPtrtoHl(const uint8* net32DataPtr);

static uint32 Sd_HtoNl(uint32 host32);

static uint16 Sd_NtoHs(uint16 net16);

static uint16 Sd_NPtrtoHs(const uint8* net16DataPtr);

static uint16 Sd_HtoNs(uint16 host16);

SD_LOCAL_INLINE uint8 Sd_InitMemoryPool(uint32 memPoolSize, void* memPool);

SD_LOCAL_INLINE void* Sd_MallocEx(uint32 size, void* memPool);

SD_LOCAL_INLINE void Sd_FreeEx(void* ptr, void* memPool);

#if (SD_INSTANCE_NUM > 0)
static void Sd_AppendToRxBuffTail(Sd_RxQueueType* rxQueueItemPtr);
#endif

#if (SD_EVENT_HANDLER_NUM > 0)
static Sd_SubscribeManageType* Sd_GetSubscribeManageObjAddr(
    Sd_SubscribeManageType* header,
    TcpIp_SockAddrType sockRemoteAddr,
    uint8 counter);

static void Sd_EnableUnicastForSubscribedClient(
    const Sd_SubscribeManageType* header,
    SoAd_RoutingGroupIdType routingGroupId);

static void Sd_DisableUnicastForSubscribedClient(
    const Sd_SubscribeManageType* header,
    SoAd_RoutingGroupIdType routingGroupId);

static Std_ReturnType Sd_AddClientToSubscribeList(
    Sd_SubscribeManageType** headerPtr,
    SoAd_SoConIdType soConId,
    TcpIp_SockAddrType sockRemoteAddr,
    uint32 ttl,
    uint8 counter);

static void Sd_DeleteClientFromSubscribeList(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    Sd_SubscribeManageType** headerPtr,
    SoAd_SoConIdType soConId,
    uint8 counter);

static void Sd_RemoveAllClientFromSubscribeList(Sd_SubscribeManageType** headerPtr);

static void Sd_UpdateSubscribeClientTTL(Sd_SubscribeManageType* subscribePtr, uint32 ttl);

static void Sd_SubscribeClientTTLManage(const Sd_ServerServiceType* serverServicePtr);

static Std_ReturnType Sd_FanOutControlTCP(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap);

static Std_ReturnType Sd_FanOutControlUDP(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap);

static Std_ReturnType Sd_FanOutControlTcpHandle(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_IPv4OptionsType* ipv4OptPtr,
    const Sd_IPv6OptionsType* ipv6OptPtr,
    uint32 ttl,
    uint8 counter);

static Std_ReturnType Sd_FanOutControlUdpHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_IPv4OptionsType* ipv4OptPtr,
    const Sd_IPv6OptionsType* ipv6OptPtr,
    uint32 ttl,
    uint8 counter);

static void Sd_FanOutMulticastUnicastCtrlAddClient(
    const Sd_EventHandlerType* eventHandlerPtr,
    Sd_EventHandlerRTType* ehRTDataPtr,
    SoAd_SoConIdType assignedSoConId);

static void Sd_FanOutMulticastUnicastCtrlDelClient(
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_EventHandlerRTType* ehRTDataPtr,
    SoAd_SoConIdType soConId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
static void Sd_ClientSaveSoConId(
    const Sd_ClientServiceType* clientServicePtr,
    const TcpIp_SockAddrType* remoteAddrPtr,
    boolean isUseTcp,
    Sd_SoConManageType* soConPtr);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
static void Sd_ClientSaveMulticastSoConId(
    const Sd_InstanceType* instancePtr,
    const Sd_ConsumedEventGroupType* consumedEventGroupPtr,
    const TcpIp_SockAddrType* multicastAddrPtr,
    const TcpIp_SockAddrType* serverAddrPtr,
    Sd_SoConManageType* soConPtr);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

static void Sd_ClientHandleSoConBasedOptionInfo(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap,
    Sd_ClientServiceRTType* cRtDataPtr);

static void Sd_ClientTimerHandle(const Sd_ClientServiceType* clientServicePtr);

static void Sd_ClientInitialWaitPhaseLeftHandle(Sd_ClientServiceRTType* cRtDataPtr);
#endif

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
static void Sd_AppendToSendQueueTail(Sd_SendQueueType** head, Sd_SendQueueType* sendQueueItem);

static void Sd_ClearSendQueue(Sd_SendQueueType** head);

static void Sd_CalcTcpUdpEndpointOptionNum(
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap,
    uint8* tcpEndpointNum,
    uint8* udpEndpointNum);

static Std_ReturnType Sd_NecessaryOptionsExistCheck(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap);

static Std_ReturnType Sd_PackageMessage(
    Sd_SendQueueType* sendQueue[],
    uint8 sendQueCnt,
    PduInfoType* pduInfoPtr,
    uint16 instanceId,
    boolean isMulticastTx);

static void Sd_TransmitMulticastMessage(
    const Sd_InstanceType* instancePtr,
    Sd_SendQueueType* multiSendQue[],
    uint8 multiSendCnt);

static void Sd_TransmitUnicastMessage(
    const Sd_InstanceType* instancePtr,
    Sd_SendQueueType* uniSendQue[],
    uint8 uniSendCnt);

static void Sd_FindMulticastMsgToSendFromSendQueue(
    Sd_SendQueueType** queueHeaderPtr,
    Sd_SendQueueType* multiSendQue[],
    uint8* multiSendCntPtr);

static void Sd_FindUnicastMsgToSendFromSendQueue(
    Sd_SendQueueType** queueHeaderPtr,
    Sd_SendQueueType* uniSendQue[],
    uint8* uniSendCntPtr);

static uint8 Sd_FindoutMulticastMessage(const Sd_InstanceType* instancePtr, Sd_SendQueueType* multiSendQue[]);

static uint8 Sd_FindoutUnicastMessage(const Sd_InstanceType* instancePtr, Sd_SendQueueType* uniSendQue[]);

static void Sd_TransmitMessage(void);
#endif

#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
static boolean Sd_RxConfigurationOptionMatchCheck(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    const uint16* optStartPosMap);
#endif

static void Sd_SaveRemoteSessionId(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, boolean isRxUseMulticast);

#if (SD_SERVER_SERVICE_NUM > 0)
static void Sd_SaveServerServiceHandleIdToSessionCtrlList(
    uint16 instanceId,
    const TcpIp_SockAddrType* remoteAddrPtr,
    uint16 serverServiceHandleId);
#endif

#if (SD_CLIENT_SERVICE_NUM > 0)
static void Sd_SaveClientServiceHandleIdToSessionCtrlList(
    uint16 instanceId,
    const TcpIp_SockAddrType* remoteAddrPtr,
    uint16 clientServiceHandleId);
#endif

static Sd_SessionIdCtrlType* Sd_FindRemoteAddrSessionIdCtrl(uint16 instanceId, const TcpIp_SockAddrType* remoteAddrPtr);

static boolean Sd_IsNewRemoteAddr(uint16 instanceId, const TcpIp_SockAddrType* remoteAddrPtr);

static Sd_SessionIdCtrlType* Sd_AddRemoteAddrToSessionIdCtrlList(
    uint16 instanceId,
    const Sd_RxQueueType* rxQueueItem,
    boolean isRxUseMulticast);

static boolean Sd_RemoteRebootDetecte(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, boolean isRxUseMulticast);

static void Sd_FreeServiceListInSesIdCtrlList(Sd_ServiceHandleIdListType* serviceList);

static void Sd_DeleteSessionIdNode(Sd_SessionIdCtrlType** head, const Sd_SessionIdCtrlType* objNode);

static void Sd_RemoteRebootDetectedHandle(uint16 instanceId, const Sd_RxQueueType* rxQueueItem);

#if (SD_CLIENT_SERVICE_NUM > 0)
static void Sd_RxStopOfferServiceHandle(uint16 clientServiceHandleId);
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_EVENT_HANDLER_NUM > 0)
static void Sd_RebootStopSubscribeEventHandle(uint16 serverServiceHandleId);
#endif /* SD_EVENT_HANDLER_NUM > 0 */

static Std_ReturnType Sd_OptionFormatCheck(const uint8 sdMsg[]);

static Std_ReturnType Sd_OptionEachItemFormatCheck(const uint8 optionArray[]);

static void Sd_IfSpecificRoutingGroupTransmit(void);

#if (SD_EVENT_HANDLER_NUM > 0)
static void Sd_ResetSoConToWildcard(
    const Sd_ServerServiceType* serverServicePtr,
    SoAd_SoConIdType soConId,
    boolean isTcpSoCon);
#endif /* SD_EVENT_HANDLER_NUM > 0 */

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
static void Sd_ClientServiceResetAllCEgSubsEntrySendFlag(
    const Sd_ClientServiceType* clientServicePtr,
    boolean tcpFlag,
    boolean udpFlag);

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
static void Sd_ClientServiceResetAllCEgSubsEntrySubsRetryCnt(const Sd_ClientServiceType* clientServicePtr);
#endif /* STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE */
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

SD_LOCAL_INLINE uint32 Sd_SendTimeAdjust(uint32 sendTime);

#define SD_STOP_SEC_CODE
#include "Sd_MemMap.h"

/*******************************************************************************
**                         Private Variable Definitions                       **
*******************************************************************************/
#define SD_START_SEC_VAR_INIT_UNSPECIFIED
#include "Sd_MemMap.h"
SD_LOCAL Sd_StatusType Sd_Status = SD_UNINIT;
SD_LOCAL Sd_InitResultType Sd_InitResult = SD_INIT_FAIL;
#define SD_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Sd_MemMap.h"

#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Sd_MemMap.h"
#if (SD_INSTANCE_NUM > 0)
SD_LOCAL Sd_RxQueueType* Sd_RxQueueHead = NULL_PTR;
SD_LOCAL Sd_InstanceRTType Sd_InstanceRTData[SD_INSTANCE_NUM];

#if (SD_SERVER_SERVICE_NUM > 0)
SD_LOCAL Sd_ServerServiceRTType Sd_ServerRTData[SD_SERVER_SERVICE_NUM];
#if (SD_EVENT_HANDLER_NUM > 0)
SD_LOCAL Sd_EventHandlerRTType Sd_EventHandlerRTData[SD_EVENT_HANDLER_NUM];
#endif /* SD_SERVER_SERVICE_NUM > 0 */
#endif /* SD_EVENT_HANDLER_NUM > 0 */

#if (SD_CLIENT_SERVICE_NUM > 0)
SD_LOCAL Sd_ClientServiceRTType Sd_ClientRTData[SD_CLIENT_SERVICE_NUM];
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
SD_LOCAL Sd_ConsumedEventGroupRTType Sd_ConsumedEventGroupRTData[SD_CONSUMED_EVENTGROUP_NUM];
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#endif /* SD_INSTANCE_NUM > 0 */

SD_LOCAL uint8 Sd_RxMemPool[SD_RX_MEMORY_POOL_SIZE];
SD_LOCAL uint8 Sd_TxMemPool[SD_TX_MEMORY_POOL_SIZE];

SD_LOCAL uint32 Sd_RandomSeed;
#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Sd_MemMap.h"

#define SD_START_SEC_VAR_INIT_PTR
#include "Sd_MemMap.h"
SD_LOCAL const Sd_ConfigType* Sd_CfgPtr = NULL_PTR;
#define SD_STOP_SEC_VAR_INIT_PTR
#include "Sd_MemMap.h"
/*******************************************************************************
**                          Global Function Definitions                       **
*******************************************************************************/
#define SD_START_SEC_CODE
#include "Sd_MemMap.h"

/******************************************************************************/
/*
 * Brief               Initializes the Service Discovery.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      ConfigPtr: Pointer to a selected configuration structure.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Sd_Init(const Sd_ConfigType* ConfigPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == SD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (NULL_PTR == ConfigPtr)
    {
        SD_REPORT_ERROR(SD_INIT_ID, SD_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SD_DEV_ERROR_DETECT */
    {
        uint16 index;
        Sd_InitResultType initResult1 = SD_INIT_FAIL;
        Sd_InitResultType initResult2 = SD_INIT_FAIL;
        uint8 memPoolRet;
#if (SD_CLIENT_SERVICE_NUM > 0)
        const Sd_ClientServiceType* clientServicePtr;
        const Sd_ConsumedEventGroupType* eventGroupPtr;
        uint16 eventGroupIdx;
#endif /* SD_CLIENT_SERVICE_NUM > 0 */
        Sd_CfgPtr = ConfigPtr;

#if (SD_SERVER_SERVICE_NUM > 0)
        for (index = 0u; index < SD_SERVER_SERVICE_NUM; index++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            /*@req <SWS_SD_00020> */
            /* If SdServerServiceAutoAvailable=True,set state SD_SERVER_SERVICE_AVAILABLE*/
            if (TRUE == Sd_CfgPtr->SdGlbServerService[index].SdServerServiceAutoAvailable)
            {
                Sd_ServerRTData[index].setState = SD_SERVER_SERVICE_AVAILABLE;
            }
            else
            {
                /* Set Server Service Instance state to SD_SERVER_SERVICE_DOWN */
                Sd_ServerRTData[index].setState = SD_SERVER_SERVICE_DOWN;
            }

            /* Set all server service to Down Phase */
            Sd_ServerRTData[index].phase = SD_SERVER_PHASE_DOWN;

            Sd_ServerRTData[index].initialWaitTimer = 0u;
            Sd_ServerRTData[index].repetitionCount = 0u;
            Sd_ServerRTData[index].repetitionTimer = 0u;
            Sd_ServerRTData[index].mainTimer = 0u;
            Sd_ServerRTData[index].head = NULL_PTR;
        }
#if (SD_EVENT_HANDLER_NUM > 0)
        for (index = 0u; index < SD_EVENT_HANDLER_NUM; index++)
        {
            Sd_EventHandlerRTData[index].udpSubscribeList = NULL_PTR;
            Sd_EventHandlerRTData[index].tcpSubscribeList = NULL_PTR;
            Sd_EventHandlerRTData[index].curState = SD_EVENT_HANDLER_RELEASED;
            Sd_EventHandlerRTData[index].udpSubsClientNum = 0u;
            Sd_EventHandlerRTData[index].tcpSubsClientNum = 0u;
            Sd_EventHandlerRTData[index].ifSpecificRoutingTransTcpFlag = FALSE;
            Sd_EventHandlerRTData[index].ifSpecificRoutingTransUdpFlag = FALSE;
        }
#endif /* SD_EVENT_HANDLER_NUM > 0 */
#endif /* SD_SERVER_SERVICE_NUM > 0 */

#if (SD_CLIENT_SERVICE_NUM > 0)
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        for (index = 0u; index < SD_CONSUMED_EVENTGROUP_NUM; index++)
        {
            Sd_ConsumedEventGroupRTData[index].setState = SD_CONSUMED_EVENTGROUP_RELEASED;
            Sd_ConsumedEventGroupRTData[index].ttl.isValidForever = FALSE;
            Sd_ConsumedEventGroupRTData[index].ttl.ttlms = 0u;
            Sd_ConsumedEventGroupRTData[index].multiSoCon.isOptRcved = FALSE;
            Sd_ConsumedEventGroupRTData[index].multiSoCon.isSoConOpened = FALSE;
            Sd_ConsumedEventGroupRTData[index].multiSoCon.soConId = 0u;
            Sd_ConsumedEventGroupRTData[index].curState = SD_CONSUMED_EVENTGROUP_DOWN;
            Sd_ConsumedEventGroupRTData[index].isSubscribed = FALSE;
            Sd_ConsumedEventGroupRTData[index].isTcpSubsSend = FALSE;
            Sd_ConsumedEventGroupRTData[index].isUdpSubsSend = FALSE;
            Sd_ConsumedEventGroupRTData[index].isAnsweredWithAck = TRUE;
            Sd_ConsumedEventGroupRTData[index].isSendSubEvtAfterRequest = FALSE;
            Sd_ConsumedEventGroupRTData[index].subsRetryEnable = FALSE;
            Sd_ConsumedEventGroupRTData[index].subsRetryCnt = 0u;
            Sd_ConsumedEventGroupRTData[index].subsRetryTimer = 0u;
        }
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

        for (index = 0u; index < SD_CLIENT_SERVICE_NUM; index++)
        {
            Sd_ClientRTData[index].ttl.isValidForever = FALSE;
            Sd_ClientRTData[index].ttl.ttlms = 0u;
            Sd_ClientRTData[index].tcpSoCon.isOptRcved = FALSE;
            Sd_ClientRTData[index].tcpSoCon.isSoConOpened = FALSE;
            Sd_ClientRTData[index].tcpSoCon.soConId = 0u;
            Sd_ClientRTData[index].udpSoCon.isOptRcved = FALSE;
            Sd_ClientRTData[index].udpSoCon.isSoConOpened = FALSE;
            Sd_ClientRTData[index].udpSoCon.soConId = 0u;
            Sd_ClientRTData[index].initialWaitTimer = 0u;
            Sd_ClientRTData[index].repetitionTimer = 0u;
            Sd_ClientRTData[index].mainTimer = 0u;
            Sd_ClientRTData[index].head = NULL_PTR;
            /*@req <SWS_SD_00021> */
            /* If SdClientServiceAutoRequired=True,set state SD_CLIENT_SERVICE_REQUESTED*/
            if (TRUE == Sd_CfgPtr->SdGlbClientService[index].SdClientServiceAutoRequire)
            {
                Sd_ClientRTData[index].setState = SD_CLIENT_SERVICE_REQUESTED;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
                /*@req <SWS_SD_00440> */
                /* If SdConsumedEventGroupAutoRequired=True,set ConsumedEventGroup state to
                   SD_CONSUMED_EVENTGROUP_REQUESTED as soon as the associated Client Service Instance is requested.*/
                clientServicePtr = &Sd_CfgPtr->SdGlbClientService[index];
                eventGroupPtr = &clientServicePtr->SdConsumedEventGroup[0];
                for (eventGroupIdx = 0u; eventGroupIdx < clientServicePtr->SdConsumedEventGroupNum; eventGroupIdx++)
                {
                    if (TRUE == eventGroupPtr->SdConsumedEventGroupAutoRequire)
                    {
                        Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId].setState =
                            SD_CONSUMED_EVENTGROUP_REQUESTED;
                    }
                    eventGroupPtr++;
                }
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
            }
            else
            {
                /* Set Client Service Instance state to SD_CLIENT_SERVICE_RELEASED */
                Sd_ClientRTData[index].setState = SD_CLIENT_SERVICE_RELEASED;
            }
            Sd_ClientRTData[index].curState = SD_CLIENT_SERVICE_DOWN;
            /* Set all client service to Down Phase */
            Sd_ClientRTData[index].phase = SD_CLIENT_PHASE_DOWN;
            Sd_ClientRTData[index].repetitionCount = 0u;
            Sd_ClientRTData[index].offerServiceRxFlag = TRUE;
            Sd_ClientRTData[index].rxTcpEndpointFlag = FALSE;
            Sd_ClientRTData[index].rxUdpEndpointFlag = FALSE;
        }
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_INSTANCE_NUM > 0)
        for (index = 0u; index < SD_INSTANCE_NUM; index++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            Sd_InstanceRTData[index].sessionIdHandle.sesIdCtrlList = NULL_PTR;
            Sd_InstanceRTData[index].sessionIdHandle.txMultiSesIdCnt = 0u;
            Sd_InstanceRTData[index].ipAddrState = TCPIP_IPADDR_STATE_UNASSIGNED;
            Sd_InstanceRTData[index].rebootFlag = TRUE;
        }

        /* @req <SWS_SD_00723> */
        /* The API SoAd_OpenSoCon() shall be called for all Socket Connections
         * associated with SdInstanceTxPdu,SdInstanceUnicastRxPdu and
         * SdInstanceMulticastRxPdu. */
        /* Call SoAd_OpenSoCon() for all SdInstanceTxPdu */
        for (index = 0u; index < SD_INSTANCE_NUM; index++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            (void)SoAd_OpenSoCon(Sd_CfgPtr->SdInstance[index].SdInstanceTxPduSoConId);
        }
        /* Call SoAd_OpenSoCon() for all SdInstanceUnicastRxPdu and
         * SdInstanceMulticastRxPdu */
        for (index = 0u; index < SD_RXPDUID_SOCONID_MAP_LEN; index++)
        {
            (void)SoAd_OpenSoCon(Sd_CfgPtr->SdGlbRxPduIdSoConIdMap[index].SoConId);
        }
#endif /* SD_INSTANCE_NUM > 0 */

        /* Init memory pool*/
        memPoolRet = Sd_InitMemoryPool(SD_TX_MEMORY_POOL_SIZE, Sd_TxMemPool);
        if (MEMHEAP_OK == memPoolRet)
        {
            initResult1 = SD_INIT_SUCCESS;
        }
        memPoolRet = Sd_InitMemoryPool(SD_RX_MEMORY_POOL_SIZE, Sd_RxMemPool);
        if (MEMHEAP_OK == memPoolRet)
        {
            initResult2 = SD_INIT_SUCCESS;
        }

        /*@req <SWS_SD_00122> */
        /* remember internally the successful initialization for other API
         * functions to check for proper module initialization. */
        if ((SD_INIT_SUCCESS == initResult1) && (SD_INIT_SUCCESS == initResult2))
        {
            Sd_InitResult = SD_INIT_SUCCESS;
        }
        else
        {
            Sd_InitResult = SD_INIT_FAIL;
        }

        /* Set SD Status */
        Sd_Status = SD_INIT;
    }

    return;
}

/******************************************************************************/
/*
 * Brief               Returns the version information of this module.
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * Return              None
 */
/******************************************************************************/
/*@req <SWS_SD_00126> */
#if (SD_VERSION_INFO_API == STD_ON)
void Sd_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (STD_ON == SD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (NULL_PTR == versioninfo)
    {
        /*@req <SWS_SD_00497> */
        SD_REPORT_ERROR(SD_GET_VERSION_INFO_ID, SD_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
        /*@req <SWS_Sd_00125> */
        versioninfo->vendorID = SD_VENDOR_ID;
        versioninfo->moduleID = SD_MODULE_ID;
        versioninfo->sw_major_version = SD_H_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = SD_H_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = SD_H_SW_PATCH_VERSION;
    }

    return;
}
#endif

/******************************************************************************/
/*
 * Brief               This API function is used by the BswM to set the Server
 *                     Service Instance state.
 * ServiceId           0x07
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      SdServerServiceHandleId: ID to identify the Server
 *                                              Service Instance.
 *                     ServerServiceState: The state the Server Service Instance
 *                                         shall be set to.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType Sd_ServerServiceSetState(uint16 SdServerServiceHandleId, Sd_ServerServiceSetStateType ServerServiceState)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;

#if (STD_ON == SD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SD_UNINIT == Sd_Status)
    {
        SD_REPORT_ERROR(SD_SERVER_SERVICE_SET_STATE_ID, SD_E_NOT_INITIALIZED);
        detNoErr = FALSE;
    }
    if (SD_INIT_FAIL == Sd_InitResult)
    {
        SD_REPORT_ERROR(SD_SERVER_SERVICE_SET_STATE_ID, SD_E_INIT_FAILED);
        detNoErr = FALSE;
    }
    if (SdServerServiceHandleId >= SD_SERVER_SERVICE_NUM)
    {
        SD_REPORT_ERROR(SD_SERVER_SERVICE_SET_STATE_ID, SD_E_INV_ID);
        detNoErr = FALSE;
    }
    if ((SD_SERVER_SERVICE_DOWN != ServerServiceState) && (SD_SERVER_SERVICE_AVAILABLE != ServerServiceState))
    {
        SD_REPORT_ERROR(SD_SERVER_SERVICE_SET_STATE_ID, SD_E_INV_MODE);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
#if (SD_SERVER_SERVICE_NUM > 0)
        SchM_Enter_Sd_ExclusiveArea();
        Sd_ServerRTData[SdServerServiceHandleId].setState = ServerServiceState;
        SchM_Exit_Sd_ExclusiveArea();

        ret = E_OK;
#endif
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               This API function is used by the BswM to set the Client
 *                     Service Instance state.
 * ServiceId           0x08
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ClientServiceHandleId: ID to identify the Client Service
 *                                            Instance.
 *                     ClientServiceState: The state the Client Service Instance
 *                                         shall be set to.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType Sd_ClientServiceSetState(uint16 ClientServiceHandleId, Sd_ClientServiceSetStateType ClientServiceState)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (SD_CLIENT_SERVICE_NUM == 0u)
    SD_UNUSED(ClientServiceHandleId);
#endif
#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (SD_UNINIT == Sd_Status)
    {
        SD_REPORT_ERROR(SD_CLIENT_SERVICE_SET_STATE_ID, SD_E_NOT_INITIALIZED);
    }
    else if (SD_INIT_FAIL == Sd_InitResult)
    {
        SD_REPORT_ERROR(SD_CLIENT_SERVICE_SET_STATE_ID, SD_E_INIT_FAILED);
    }
    else if ((SD_CLIENT_SERVICE_RELEASED != ClientServiceState) && (SD_CLIENT_SERVICE_REQUESTED != ClientServiceState))
    {
        SD_REPORT_ERROR(SD_CLIENT_SERVICE_SET_STATE_ID, SD_E_INV_MODE);
    }
#if (SD_CLIENT_SERVICE_NUM > 0u)
    else if (ClientServiceHandleId >= SD_CLIENT_SERVICE_NUM) /* PRQA S 2995, 2991 */ /* MISRA Rule 2.2, 14.3 */
    {
        SD_REPORT_ERROR(SD_CLIENT_SERVICE_SET_STATE_ID, SD_E_INV_ID);
    }
#endif
    else
#endif
    {
#if (SD_CLIENT_SERVICE_NUM > 0u)
        SchM_Enter_Sd_ExclusiveArea();
        Sd_ClientRTData[ClientServiceHandleId].setState = ClientServiceState;
        SchM_Exit_Sd_ExclusiveArea();

        ret = E_OK;
#endif
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief               This API function is used by the BswM to set the requested
 *                     state of the EventGroupStatus.
 * ServiceId           0x09
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      SdConsumedEventGroupHandleId: ID to identify the Consumed
 *                                                   Eventgroup.
 *                     ConsumedEventGroupState: The state the EventGroup shall be
 *                                              set to.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
Std_ReturnType Sd_ConsumedEventGroupSetState(
    uint16 SdConsumedEventGroupHandleId,
    Sd_ConsumedEventGroupSetStateType ConsumedEventGroupState)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
    Std_ReturnType ret = E_NOT_OK;
#if (SD_CONSUMED_EVENTGROUP_NUM == 0u)
    SD_UNUSED(SdConsumedEventGroupHandleId);
#endif
#if (STD_ON == SD_DEV_ERROR_DETECT)
    if (SD_UNINIT == Sd_Status)
    {
        SD_REPORT_ERROR(SD_CONSUMED_EVENT_GROUP_SET_STATE_ID, SD_E_NOT_INITIALIZED);
    }
    else if (SD_INIT_FAIL == Sd_InitResult)
    {
        SD_REPORT_ERROR(SD_CONSUMED_EVENT_GROUP_SET_STATE_ID, SD_E_INIT_FAILED);
    }
    else if (
        (SD_CONSUMED_EVENTGROUP_RELEASED != ConsumedEventGroupState)
        && (SD_CONSUMED_EVENTGROUP_REQUESTED != ConsumedEventGroupState))
    {
        SD_REPORT_ERROR(SD_CONSUMED_EVENT_GROUP_SET_STATE_ID, SD_E_INV_MODE);
    }
#if (SD_CONSUMED_EVENTGROUP_NUM > 0u)
    else if (SdConsumedEventGroupHandleId >= SD_CONSUMED_EVENTGROUP_NUM) /* PRQA S 2995, 2991 */ /* MISRA Rule 2.2, 14.3
                                                                                                  */
    {
        SD_REPORT_ERROR(SD_CONSUMED_EVENT_GROUP_SET_STATE_ID, SD_E_INV_ID);
    }
#endif
    else
#endif
    {
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        const Sd_ClientServiceType* cServicePtr;
        const Sd_ConsumedEventGroupType* consumedEgPtr;
        Sd_ConsumedEventGroupRTType* consumedEgRTPtr;
        Sd_ClientServiceRTType* cRtDataPtr = NULL_PTR;
        uint16 i, j;

        cServicePtr = Sd_CfgPtr->SdGlbClientService;
        for (i = 0u; i < SD_CLIENT_SERVICE_NUM; i++)
        {
            consumedEgPtr = cServicePtr->SdConsumedEventGroup;
            for (j = 0u; j < cServicePtr->SdConsumedEventGroupNum; j++)
            {
                if (SdConsumedEventGroupHandleId == consumedEgPtr->SdConsumedEventGroupHandleId)
                {
                    cRtDataPtr = &Sd_ClientRTData[cServicePtr->SdClientServiceHandleId];
                    ret = E_OK;
                    break;
                }
                consumedEgPtr++;
            }
            cServicePtr++;
        }

        if (E_OK == ret)
        {
            if (SD_CLIENT_SERVICE_RELEASED != cRtDataPtr->setState)
            {
                SchM_Enter_Sd_ExclusiveArea();
                consumedEgRTPtr = &Sd_ConsumedEventGroupRTData[SdConsumedEventGroupHandleId];
                consumedEgRTPtr->setState = ConsumedEventGroupState;
                if (SD_CONSUMED_EVENTGROUP_REQUESTED == ConsumedEventGroupState)
                {
                    consumedEgRTPtr->isSendSubEvtAfterRequest = FALSE;
                }
                SchM_Exit_Sd_ExclusiveArea();
            }
            else
            {
                /*@req <SWS_SD_00442> */
                ret = E_NOT_OK;
            }
        }
#endif
    }

    return ret;
}
#define SD_STOP_SEC_CODE
#include "Sd_MemMap.h"

#define SD_START_SEC_SDLOCALIPADDRASSIGMENTCHG_CALLBACK_CODE
#include "Sd_MemMap.h"
/******************************************************************************/
/*
 * Brief               This function gets called by the SoAd if an IP address
 *                     assignment related to a socket connection changes (i.e.
 *                     new address assigned or assigned address becomes invalid).
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different SoConIds.
 *                     Non Reentrant for the same SoConId.
 * Param-Name[in]      SoConId: socket connection index specifying the socket
                                connection where the IP address assigment has changed.
 *                     State:   state of IP address assignment.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 1532 ++ */ /* MISRA Rule 8.7 */
void Sd_LocalIpAddrAssignmentChg(SoAd_SoConIdType SoConId, TcpIp_IpAddrStateType State)
/* PRQA S 1532 -- */ /* MISRA Rule 8.7 */
{
#if (STD_ON == SD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SD_UNINIT == Sd_Status)
    {
        SD_REPORT_ERROR(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, SD_E_NOT_INITIALIZED);
        detNoErr = FALSE;
    }
    if (SD_INIT_FAIL == Sd_InitResult)
    {
        SD_REPORT_ERROR(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, SD_E_INIT_FAILED);
        detNoErr = FALSE;
    }
    if (SoConId >= SOAD_SO_CON_MAX)
    {
        SD_REPORT_ERROR(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, SD_E_INV_ID);
        detNoErr = FALSE;
    }
    if ((TCPIP_IPADDR_STATE_ASSIGNED != State) && (TCPIP_IPADDR_STATE_ONHOLD != State)
        && (TCPIP_IPADDR_STATE_UNASSIGNED != State))
    {
        SD_REPORT_ERROR(SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID, SD_E_INV_MODE);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
#if (SD_INSTANCE_NUM > 0)
        SoAd_SoConIdType txPduSoConId;
        uint16 idx;

        for (idx = 0u; idx < SD_INSTANCE_NUM; idx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            txPduSoConId = Sd_CfgPtr->SdInstance[idx].SdInstanceTxPduSoConId;
            if (txPduSoConId == SoConId)
            {
                Sd_InstanceRTData[idx].ipAddrState = State;
            }
        }
#endif
    }
}
#define SD_STOP_SEC_SDLOCALIPADDRASSIGMENTCHG_CALLBACK_CODE
#include "Sd_MemMap.h"

#define SD_START_SEC_SDSOCONMODECHG_CALLBACK_CODE
#include "Sd_MemMap.h"
/******************************************************************************/
/*
 * Brief               Notification about a SoAd socket connection state change,
 *                     e.g. socket connection gets online.
 * ServiceId           0x43
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different SoConIds.
 *                     Non Reentrant for the same SoConId.
 * Param-Name[in]      SoConId: socket connection index specifying the socket
                                connection where the IP address assigment has
                                changed.
 *                     Mode :   new mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Sd_SoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode)
{
#if (STD_ON == SD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SD_UNINIT == Sd_Status)
    {
        SD_REPORT_ERROR(SD_SOCON_MODE_CHG_ID, SD_E_NOT_INITIALIZED);
        detNoErr = FALSE;
    }
    if (SD_INIT_FAIL == Sd_InitResult)
    {
        SD_REPORT_ERROR(SD_SOCON_MODE_CHG_ID, SD_E_INIT_FAILED);
        detNoErr = FALSE;
    }
    if (SoConId >= SOAD_SO_CON_MAX)
    {
        SD_REPORT_ERROR(SD_SOCON_MODE_CHG_ID, SD_E_INV_ID);
        detNoErr = FALSE;
    }
    if ((SOAD_SOCON_ONLINE != Mode) && (SOAD_SOCON_RECONNECT != Mode) && (SOAD_SOCON_OFFLINE != Mode))
    {
        SD_REPORT_ERROR(SD_SOCON_MODE_CHG_ID, SD_E_INV_MODE);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif
    {
#if (SD_INSTANCE_NUM > 0)
        const Sd_InstanceType* instancePtr = Sd_CfgPtr->SdInstance;
        const SoAd_SoConIdType* soConIdList;
#if (SD_SERVER_SERVICE_NUM > 0)
        const Sd_ServerServiceType* serverServicePtr;
        const Sd_SoAdSoConGroupType* serverServiceTcpRef;
        const Sd_SoAdSoConGroupType* serverServiceUdpRef;
        const Sd_ServerServiceRTType* sRtDataPtr;
#endif /* SD_SERVER_SERVICE_NUM > 0 */
        uint16 serverIdx, index;
        uint16 soConIdNum;
#if (SD_CLIENT_SERVICE_NUM > 0)
        const Sd_ClientServiceType* clientServicePtr;
        const Sd_SoAdSoConGroupType* clientServiceTcpRef;
        const Sd_SoAdSoConGroupType* clientServiceUdpRef;
        Sd_ClientServiceRTType* cRtDataPtr;
        uint16 clientIdx;
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

        if (SOAD_SOCON_ONLINE != Mode)
        {
            for (; instancePtr < &Sd_CfgPtr->SdInstance[SD_INSTANCE_NUM]; instancePtr++)
            {
#if (SD_SERVER_SERVICE_NUM > 0)
                serverServicePtr = instancePtr->SdServerService;
                for (serverIdx = 0u; serverIdx < instancePtr->SdServerServiceNum; serverIdx++)
                {
                    serverServiceTcpRef = serverServicePtr->SdServerServiceTcpRef;
                    if (NULL_PTR != serverServiceTcpRef)
                    {
                        soConIdNum = serverServiceTcpRef->SdSoConNumInGroup;
                        soConIdList = serverServiceTcpRef->SdSocketIdList;
                        for (index = 0u; index < soConIdNum; index++)
                        {
                            if (SoConId == soConIdList[index])
                            {
                                sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
                                if ((SD_SERVER_PHASE_MAIN == sRtDataPtr->phase)
                                    || (SD_SERVER_PHASE_REPETITION == sRtDataPtr->phase))
                                {
                                    /* @req SWS_SD_00732,SWS_SD_00733 */
                                    Sd_ServerServiceInitialWaitEnter(instancePtr, serverServicePtr);
                                }
                                break;
                            }
                        }
                    }

                    serverServiceUdpRef = serverServicePtr->SdServerServiceUdpRef;
                    if (NULL_PTR != serverServiceUdpRef)
                    {
                        soConIdNum = serverServiceUdpRef->SdSoConNumInGroup;
                        soConIdList = serverServiceUdpRef->SdSocketIdList;
                        for (index = 0u; index < soConIdNum; index++)
                        {
                            if (SoConId == soConIdList[index])
                            {
                                sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
                                if ((SD_SERVER_PHASE_MAIN == sRtDataPtr->phase)
                                    || (SD_SERVER_PHASE_REPETITION == sRtDataPtr->phase))
                                {
                                    /* @req SWS_SD_00732,SWS_SD_00733 */
                                    Sd_ServerServiceInitialWaitEnter(instancePtr, serverServicePtr);
                                }
                                break;
                            }
                        }
                    }
                    serverServicePtr++;
                }
#endif /* SD_SERVER_SERVICE_NUM > 0 */

#if (SD_CLIENT_SERVICE_NUM > 0)
                clientServicePtr = instancePtr->SdClientService;
                for (clientIdx = 0u; clientIdx < instancePtr->SdClientServiceNum; clientIdx++)
                {
                    clientServiceTcpRef = clientServicePtr->SdClientServiceTcpRef;
                    if (NULL_PTR != clientServiceTcpRef)
                    {
                        soConIdNum = clientServiceTcpRef->SdSoConNumInGroup;
                        soConIdList = clientServiceTcpRef->SdSocketIdList;
                        for (index = 0u; index < soConIdNum; index++)
                        {
                            if (SoConId == soConIdList[index])
                            {
                                cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
                                if ((SD_CLIENT_PHASE_MAIN == cRtDataPtr->phase)
                                    || (SD_CLIENT_PHASE_REPETITION == cRtDataPtr->phase))
                                {
                                    /* @req SWS_Sd_00730,SWS_Sd_00731 */
                                    Sd_ClientServiceInitialWaitEnter(clientServicePtr, cRtDataPtr);
                                }
                                break;
                            }
                        }
                    }

                    clientServiceUdpRef = clientServicePtr->SdClientServiceUdpRef;
                    if (NULL_PTR != clientServiceUdpRef)
                    {
                        soConIdNum = clientServiceUdpRef->SdSoConNumInGroup;
                        soConIdList = clientServiceUdpRef->SdSocketIdList;
                        for (index = 0u; index < soConIdNum; index++)
                        {
                            if (SoConId == soConIdList[index])
                            {
                                cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
                                if ((SD_CLIENT_PHASE_MAIN == cRtDataPtr->phase)
                                    || (SD_CLIENT_PHASE_REPETITION == cRtDataPtr->phase))
                                {
                                    /* @req SWS_Sd_00730,SWS_Sd_00731 */
                                    Sd_ClientServiceInitialWaitEnter(clientServicePtr, cRtDataPtr);
                                }
                                break;
                            }
                        }
                    }
                    clientServicePtr++;
                }
#endif /* SD_CLIENT_SERVICE_NUM > 0 */
            }
        }
#endif /* SD_INSTANCE_NUM > 0 */
    }
}
#define SD_STOP_SEC_SDSOCONMODECHG_CALLBACK_CODE
#include "Sd_MemMap.h"

#define SD_START_SEC_SDRXINDICATION_CALLBACK_CODE
#include "Sd_MemMap.h"
/******************************************************************************/
/*
 * Brief               Indication of a received I-PDU from a lower layer
 *                     communication interface module.
 * ServiceId           0x42
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant for different SoConIds.
 *                     Non Reentrant for the same SoConId.
 * Param-Name[in]      RxPduId: ID of the received I-PDU.
 *                     PduInfoPtr: Contains the length (SduLength) of the received
 *                                 I-PDU and a pointer to a buffer (SduDataPtr)
 *                                 containing the I-PDU.
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
void Sd_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (STD_ON == SD_DEV_ERROR_DETECT)
    boolean detNoErr = TRUE;
    if (SD_UNINIT == Sd_Status)
    {
        SD_REPORT_ERROR(SD_RX_INDICATION_ID, SD_E_NOT_INITIALIZED);
        detNoErr = FALSE;
    }
    if (SD_INIT_FAIL == Sd_InitResult)
    {
        SD_REPORT_ERROR(SD_RX_INDICATION_ID, SD_E_INIT_FAILED);
        detNoErr = FALSE;
    }
    if (NULL_PTR == PduInfoPtr)
    {
        SD_REPORT_ERROR(SD_RX_INDICATION_ID, SD_E_PARAM_POINTER);
        detNoErr = FALSE;
    }
    if (RxPduId >= SD_RXPDUID_SOCONID_MAP_LEN)
    {
        SD_REPORT_ERROR(SD_RX_INDICATION_ID, SD_E_INV_ID);
        detNoErr = FALSE;
    }
    if (TRUE == detNoErr)
#endif /* STD_ON == SD_DEV_ERROR_DETECT */
    {
#if (SD_INSTANCE_NUM > 0)
        const uint8* sdMsgPtr = PduInfoPtr->SduDataPtr;
        uint32 entryArrayLen, optionArrayLen;
        boolean msgCheckPassFlag = TRUE;
        Sd_RxQueueType* rxQueuePtr;
        uint8* rxBufPtr;
        TcpIp_SockAddrType tcpIpSockAddr;
        const Sd_InstanceType* instancePtr;
        const Sd_RxPduIdSoConIdMapType* rxPduIdSoConIdMap;
        uint16 index, index2;
        Sd_HeaderType header;

        (void)ILib_memcpy((uint8*)&header, sdMsgPtr, sizeof(Sd_HeaderType));
        /* Header field check: Protocol Version = 0x01, Interface Version = 0x01,
         * Message Types = 0x02, Return Code = 0x00 */
        if ((PduInfoPtr->SduLength < SD_ENTRIES_ARRAY_LENGTH_INDEX) || (header.ProtocolVersion != 0x01u)
            || (header.InterfaceVersion != 0x01u) || (header.MessageType != 0x02u) || (header.ReturnCode != 0x00u))
        {
            msgCheckPassFlag = FALSE;
        }

        entryArrayLen = Sd_NPtrtoHl(&sdMsgPtr[SD_ENTRIES_ARRAY_LENGTH_INDEX]);
        optionArrayLen = Sd_NPtrtoHl(&sdMsgPtr[SD_ENTRIES_ARRAY_LENGTH_INDEX + entryArrayLen + 4u]);
        /*@req <SWS_SD_00708> */
        /* 20 = header(12) + length field of entries(4) + length field of option(4) */
        if ((PduInfoPtr->SduLength >= (entryArrayLen + optionArrayLen + 20u)) && (TRUE == msgCheckPassFlag))
        {
            /* @req SWS_SD_00482 */
            /* Received message check pass,continue process */
            rxQueuePtr = (Sd_RxQueueType*)Sd_MallocEx(sizeof(Sd_RxQueueType), Sd_RxMemPool);
            rxBufPtr = (uint8*)Sd_MallocEx(PduInfoPtr->SduLength, Sd_RxMemPool);
            if ((NULL_PTR != rxQueuePtr) && (NULL_PTR != rxBufPtr))
            {
                /* Get remote address */
                rxPduIdSoConIdMap = Sd_CfgPtr->SdGlbRxPduIdSoConIdMap;
                for (index = 0u; index < SD_RXPDUID_SOCONID_MAP_LEN; index++)
                {
                    if (rxPduIdSoConIdMap->SdRxPduId == RxPduId)
                    {
                        instancePtr = Sd_CfgPtr->SdInstance;
                        for (index2 = 0u; index2 < SD_INSTANCE_NUM; index2++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
                        {
                            if ((instancePtr[index2].SdInstanceMulticastRxPdu == RxPduId)
                                || (instancePtr[index2].SdInstanceUnicastRxPdu == RxPduId))
                            {
                                tcpIpSockAddr.domain = instancePtr[index2].SdAddrFamily;
                                break;
                            }
                        }
                        (void)SoAd_GetRemoteAddr(rxPduIdSoConIdMap->SoConId, &tcpIpSockAddr);
                        (void)ILib_memcpy(&rxQueuePtr->remoteAddr, &tcpIpSockAddr, sizeof(tcpIpSockAddr));

                        /* Reset the SoCon back to Wildcard using SoAd_ReleaseRemoteAddr() */
                        SoAd_ReleaseRemoteAddr(rxPduIdSoConIdMap->SoConId);
                        break;
                    }
                    rxPduIdSoConIdMap++;
                }

                /* Copy sdu to receive buffer */
                (void)ILib_memcpy(rxBufPtr, sdMsgPtr, PduInfoPtr->SduLength);

                /* Save info to Queue */
                rxQueuePtr->bufPtr = rxBufPtr;
                rxQueuePtr->rxPduId = RxPduId;
                rxQueuePtr->pduLength = PduInfoPtr->SduLength;
                rxQueuePtr->next = NULL_PTR;

                Sd_AppendToRxBuffTail(rxQueuePtr);
            }
        }
#if (SD_DEM_USED == STD_ON)
        else
        {
            msgCheckPassFlag = FALSE;
        }

        if (FALSE == msgCheckPassFlag)
        {
            /* In case a malformed message has been received, the extended production error
             * SD_E_MALFORMED_MSG shall be reported.*/
            instancePtr = Sd_CfgPtr->SdInstance;
            for (index = 0u; index < SD_INSTANCE_NUM; index++)
            {
                if ((instancePtr->SdInstanceMulticastRxPdu == RxPduId)
                    || (instancePtr->SdInstanceUnicastRxPdu == RxPduId))
                {
                    const Sd_DemEventParameterType* demEventPtr;
                    demEventPtr = instancePtr->SdInstanceDemEventParameterRefs->SD_E_MALFORMED_MSG;
                    if (NULL_PTR != demEventPtr)
                    {
                        Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
                    }
                    break;
                }
                instancePtr++;
            }
        }
#endif /* SD_DEM_USED == STD_ON */
#endif /* SD_INSTANCE_NUM > 0 */
    }
}
#define SD_STOP_SEC_SDRXINDICATION_CALLBACK_CODE
#include "Sd_MemMap.h"

#define SD_START_SEC_CODE
#include "Sd_MemMap.h"
/******************************************************************************/
/*
 * Brief               SD module main funtion
 * ServiceId           0x06
 * Sync/Async          -
 * Reentrancy          Non Reentrant.
 * Param-Name[in]      None
 * Param-Name[in/out]  None
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Sd_MainFunction(void) /* PRQA S 1532 */ /* MISRA Rule 8.7 */
{
#if (SD_INSTANCE_NUM > 0)
    if (SD_INIT_SUCCESS == Sd_InitResult)
    {
        uint16 instanceIdx, serverIdx;
        const Sd_InstanceType* instancePtr;
#if (SD_SERVER_SERVICE_NUM > 0)
        const Sd_ServerServiceType* serverServicePtr;
#endif /* SD_SERVER_SERVICE_NUM > 0 */
#if (SD_CLIENT_SERVICE_NUM > 0)
        const Sd_ClientServiceType* clientServicePtr;
        uint16 clientIdx;
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

        /* Random calculate seed increment */
        Sd_RandomSeed++;

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
        Sd_ParseRxMessage();
#endif /* SD_SERVER_SERVICE_NUM > 0 || SD_CLIENT_SERVICE_NUM > 0 */

        for (instanceIdx = 0u; instanceIdx < SD_INSTANCE_NUM; instanceIdx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            instancePtr = &Sd_CfgPtr->SdInstance[instanceIdx];
#if (SD_SERVER_SERVICE_NUM > 0)
            serverServicePtr = instancePtr->SdServerService;
            if (NULL_PTR != serverServicePtr)
            {
                for (serverIdx = 0u; serverIdx < instancePtr->SdServerServiceNum; serverIdx++)
                {
                    Sd_ServerServiceTimerHandle(instancePtr, serverServicePtr);

                    switch (Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId].phase)
                    {
                    case SD_SERVER_PHASE_DOWN:
                        Sd_ServerServiceDownPhaseHandle(instancePtr, serverServicePtr);
                        break;

                    case SD_SERVER_PHASE_INITIAL_WAIT:
                        Sd_ServerServiceInitialWaitPhaseHandle(instancePtr, serverServicePtr);
                        break;

                    case SD_SERVER_PHASE_REPETITION:
                        Sd_ServerServiceRepetitionPhaseHandle(instancePtr, serverServicePtr);
                        break;

                    case SD_SERVER_PHASE_MAIN:
                        Sd_ServerServiceMainPhaseHandle(instancePtr, serverServicePtr);
                        break;

                    default:
                        /* Wrong Phase */
                        break;
                    }
                    serverServicePtr++;
                }
            }
#endif /* SD_SERVER_SERVICE_NUM > 0 */

#if (SD_CLIENT_SERVICE_NUM > 0)
            clientServicePtr = instancePtr->SdClientService;
            if (NULL_PTR != clientServicePtr)
            {
                for (clientIdx = 0u; clientIdx < instancePtr->SdClientServiceNum; clientIdx++)
                {
                    Sd_ClientTimerHandle(clientServicePtr);
                    switch (Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId].phase)
                    {
                    case SD_CLIENT_PHASE_DOWN:
                        Sd_ClientServiceDownPhaseHandle(instancePtr, clientServicePtr);
                        break;

                    case SD_CLIENT_PHASE_INITIAL_WAIT:
                        Sd_ClientServiceInitialWaitPhaseHandle(instancePtr, clientServicePtr);
                        break;

                    case SD_CLIENT_PHASE_REPETITION:
                        Sd_ClientServiceRepetitionPhaseHandle(instancePtr, clientServicePtr);
                        break;

                    case SD_CLIENT_PHASE_MAIN:
                        Sd_ClientServiceMainPhaseHandle(instancePtr, clientServicePtr);
                        break;

                    default:
                        /* Wrong Phase */
                        break;
                    }

                    clientServicePtr++;
                }
            }
#endif /* SD_CLIENT_SERVICE_NUM > 0 */
        }

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
        Sd_TransmitMessage();

        /* PATCH for TC8 */
        Sd_IfSpecificRoutingGroupTransmit();
#endif /* SD_SERVER_SERVICE_NUM > 0 || SD_CLIENT_SERVICE_NUM > 0 */
    }
#endif /* SD_INSTANCE_NUM > 0 */
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/******************************************************************************/
/*
 * Brief: Parse received message
 * Param-Name[in]: Src, len
 * Param-Name[out]: Dst
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
static void Sd_ParseRxMessage(void)
{
    uint16 optStartPos[70] = {0}; /* Max Option Num: ((1400-12-4-4)/(16+12*2))*2 */
    const Sd_InstanceType* instancePtr;
    Sd_RxQueueType* rxQueueItem;
    uint32 entryArrayLen, optionArrayLen;
    uint32 loopEndPos;
    const uint8* sdMsg;
    uint16 index;
    uint16 firstOptionIndex;
    uint16 optionLength;
    uint16 instanceIdx;
    uint8 entryType;
    uint8 count;
    boolean isRxUseMulticast;
    boolean isRebootDetected;
    boolean isNewRemoteAddr;
    boolean isReplyNAck;
    Std_ReturnType optFormatCheckRet = E_OK;
    Std_ReturnType optSecuritycheckRet = E_OK;
    Std_ReturnType optConflictCheckRet = E_OK;
    Std_ReturnType optExistCheckRet = E_OK;

    while (NULL_PTR != Sd_RxQueueHead)
    {
        rxQueueItem = Sd_RxQueueHead;
        sdMsg = rxQueueItem->bufPtr;

        optFormatCheckRet = Sd_OptionFormatCheck(sdMsg);
        entryArrayLen = Sd_NPtrtoHl(&sdMsg[SD_ENTRIES_ARRAY_LENGTH_INDEX]);
        optionArrayLen = Sd_NPtrtoHl(&sdMsg[SD_ENTRIES_ARRAY_LENGTH_INDEX + entryArrayLen + 4u]);

        /* Find instance use rxPduId */
        instancePtr = Sd_CfgPtr->SdInstance;
        if (E_OK == optFormatCheckRet)
        {
            /*@req <SWS_SD_00708> */
            /* Find out all options start index,store them to optStartPos array */
            firstOptionIndex = SD_FIRST_ENTRY_INDEX + (uint16)entryArrayLen + SD_LENGTH_OF_OPTIONS_ARRAY_FIELD_LEN;
            index = firstOptionIndex;
            count = 0u;
            loopEndPos = optionArrayLen + firstOptionIndex;
            while (index < loopEndPos)
            {
                optStartPos[count] = index;
                count++;
                optionLength = Sd_NPtrtoHs(&sdMsg[index]);
                index = index + optionLength + SD_OPTION_LENGTH_FIELD_LEN + SD_OPTION_TYPE_FIELD_LEN;
            }
            optExistCheckRet = Sd_OptionsExistCheck(instancePtr, sdMsg, entryArrayLen, count);
        }

        for (instanceIdx = 0u; instanceIdx < SD_INSTANCE_NUM; instanceIdx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
        {
            if ((rxQueueItem->rxPduId == instancePtr->SdInstanceMulticastRxPdu)
                || (rxQueueItem->rxPduId == instancePtr->SdInstanceUnicastRxPdu))
            {
                if (rxQueueItem->rxPduId == instancePtr->SdInstanceMulticastRxPdu)
                {
                    isRxUseMulticast = TRUE;
                }
                else
                {
                    isRxUseMulticast = FALSE;
                }

                isRebootDetected = FALSE;
                isNewRemoteAddr = Sd_IsNewRemoteAddr(instancePtr->SdInstanceHandleId, &rxQueueItem->remoteAddr);
                if (TRUE == isNewRemoteAddr)
                {
                    /* Add the remote address to SessionControlList */
                    (void)Sd_AddRemoteAddrToSessionIdCtrlList(
                        instancePtr->SdInstanceHandleId,
                        rxQueueItem,
                        isRxUseMulticast);
                }
                else
                {
                    isRebootDetected =
                        Sd_RemoteRebootDetecte(instancePtr->SdInstanceHandleId, rxQueueItem, isRxUseMulticast);
                    if (TRUE == isRebootDetected)
                    {
                        Sd_RemoteRebootDetectedHandle(instancePtr->SdInstanceHandleId, rxQueueItem);
                    }
                }

                /* Parse Entries */
                loopEndPos = entryArrayLen + SD_FIRST_ENTRY_INDEX;
                for (index = SD_FIRST_ENTRY_INDEX; index < loopEndPos; index += SD_ENTRY_SIZE)
                {
                    entryType = sdMsg[index];
                    isReplyNAck = FALSE;

                    /* PATCH for TC8: ETS_081 */
                    if ((TRUE == isRebootDetected) && (SD_ENTRY_TYPE_OFFER_SERVICE == entryType))
                    {
                        continue;
                    }

                    if (0u == optionArrayLen)
                    {
                        if (SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP == entryType)
                        {
                            isReplyNAck = TRUE;
                        }
                        else if (SD_ENTRY_TYPE_OFFER_SERVICE == entryType)
                        {
                            continue;
                        }
                        else
                        {
                            /* FindService and SubscribeEventgroupAck entry's optionArrayLength can be 0 */
                        }
                    }
                    else
                    {
                        optSecuritycheckRet = E_OK;
                        optConflictCheckRet = E_OK;
                        if ((E_OK == optFormatCheckRet) && (E_OK == optExistCheckRet))
                        {
                            /*@req <SWS_SD_00688> */
                            optSecuritycheckRet = Sd_OptionsSecurityCheck(instancePtr, sdMsg, index, optStartPos);

                            if (E_OK == optSecuritycheckRet)
                            {
                                /*@req <SWS_SD_00663>,<SWS_SD_00714> */
                                optConflictCheckRet = Sd_OptionsConflictCheck(sdMsg, index, optStartPos);
                            }
                        }
                    }
                    if ((E_NOT_OK == optFormatCheckRet) || (E_NOT_OK == optSecuritycheckRet)
                        || (E_NOT_OK == optConflictCheckRet) || (E_NOT_OK == optExistCheckRet))
                    {
                        if (SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP != entryType)
                        {
                            /* Security check not pass,ignore the entry */
                            /* An entry references two or more options that are in conflict,Ignored the entry*/
                            continue;
                        }
                        else
                        {
                            /* SubscribeEventgroup Message Option check not pass,Send SubscribeEventgroupNAck*/
                            isReplyNAck = TRUE;
                        }
                    }

                    if (SD_ENTRY_TYPE_FIND_SERVICE == entryType)
                    {
#if (SD_SERVER_SERVICE_NUM > 0)
                        Sd_RxFindServiceHandle(instancePtr, rxQueueItem, index, isRxUseMulticast, optStartPos);
#endif /* SD_SERVER_SERVICE_NUM > 0 */
                    }
                    else if (SD_ENTRY_TYPE_OFFER_SERVICE == entryType)
                    {
#if (SD_CLIENT_SERVICE_NUM > 0)
                        Sd_RxOfferServiceHandle(instancePtr, rxQueueItem, index, optStartPos);
#endif /* SD_CLIENT_SERVICE_NUM > 0 */
                    }
                    else if (SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP == entryType)
                    {
#if (SD_EVENT_HANDLER_NUM > 0)

                        Sd_RxSubscribeEventgroupHandle(instancePtr, rxQueueItem, index, optStartPos, isReplyNAck);
#endif /* SD_EVENT_HANDLER_NUM > 0 */
                    }
                    else if (SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK == entryType)
                    {
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
                        Sd_RxSubscribeEventgroupAckHandle(instancePtr, rxQueueItem, index, optStartPos);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
                    }
                    else
                    {
                        /*@req <SWS_SD_00483> */
                        /* When receiving Service Discovery messages, the receiver shall
                         * ignore Entries of unknown type.*/
                    }
                }
                /* Save remote address used for SessionId control */
                if (FALSE == isRebootDetected)
                {
                    Sd_SaveRemoteSessionId(instancePtr->SdInstanceHandleId, rxQueueItem, isRxUseMulticast);
                }
                break;
            }

            instancePtr++;
        }

        Sd_RxQueueHead = rxQueueItem->next;
        /* Free saved sdu data buffer */
        Sd_FreeEx(rxQueueItem->bufPtr, Sd_RxMemPool);
        /* Free queue buffer */
        Sd_FreeEx(rxQueueItem, Sd_RxMemPool);
    }
}

/******************************************************************************/
/*
 * Brief: Security check for options(Check if Clients and Servers in the same subset)
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK-No Error,E_NOT_OK-Error,need response Nack
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_OptionsSecurityCheck(
    const Sd_InstanceType* instancePtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap)
{
    Sd_Type1EntryType type1Entry;
    Sd_IPv4OptionsType ipv4Opt;
#if (STD_ON == SD_IPV6_ENABLE)
    Sd_IPv6OptionsType ipv6Opt;
#endif /* STD_ON == SD_IPV6_ENABLE */
    TcpIp_SockAddrType localAddr, defaultRouter;
    SoAd_SoConIdType soConId;
    uint8 idx1stOptions, numOpt1;
    uint8 idx2ndOptions, numOpt2;
    uint8 index, optIndex;
    uint8 optType;
    uint8 localAddrCheckLen;
    uint8 netmask;
    Std_ReturnType checkPass = E_OK;
    uint8 optNumSum;

    /* Get loacal address and netmask for options security check */
    soConId = instancePtr->SdInstanceTxPduSoConId;
    localAddr.domain = instancePtr->SdAddrFamily;
    defaultRouter.domain = instancePtr->SdAddrFamily;
    (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);

    /* @req SWS_SD_00720 */
    if (TRUE == instancePtr->SdInstanceLocalAdressCheckLengthEnable)
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
        (void)ILib_memcpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
        idx1stOptions = type1Entry.Index1stOptions;
        numOpt1 = type1Entry.NumOfOpt1;
        idx2ndOptions = type1Entry.Index2ndOptions;
        numOpt2 = type1Entry.NumOfOpt2;

        optNumSum = numOpt1 + numOpt2;
        for (index = 0; index < optNumSum; index++)
        {
            if (index < numOpt1)
            {
                optIndex = idx1stOptions + index;
            }
            else
            {
                optIndex = idx2ndOptions + index - numOpt1;
            }

            optType = sdMsg[optStartPosMap[optIndex] + 2u];
            if ((SD_IPV4_ENDPOINT_OPTION == optType) || (SD_IPV4_SD_ENDPOINT_OPTION == optType))
            {
                (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
                checkPass = Sd_IsIpAddrInSameSubset(
                    (uint8*)localAddr.addr,
                    (const uint8*)ipv4Opt.IPv4Address,
                    localAddrCheckLen,
                    4u);
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else if ((SD_IPV6_ENDPOINT_OPTION == optType) || (SD_IPV6_SD_ENDPOINT_OPTION == optType))
            {
                (void)ILib_memcpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
                checkPass = Sd_IsIpAddrInSameSubset(
                    (uint8*)localAddr.addr,
                    (const uint8*)ipv6Opt.IPv6Address,
                    localAddrCheckLen,
                    16u);
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

/******************************************************************************/
/*
 * Brief: Check whether IP address is in same subset
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK-In same subset,E_NOT_OK-In different subset
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_IsIpAddrInSameSubset(const uint8* ipAddr1, const uint8* ipAddr2, uint8 netmask, uint8 addrLen)
{
    uint8 addr1AndResult[16];
    uint8 addr2AndResult[16];
    uint8 netmaskArray[16];
    sint32 cmpResult;
    uint8 index;
    uint8 clrByte;
    uint8 clrBit;
    Std_ReturnType ret = E_NOT_OK;
    uint8 tAddrLen;

    /* PATCH for TC8: check if remoteAddr equal to DUT local Address */
    cmpResult = ILib_memcmp(ipAddr1, ipAddr2, addrLen);
    if (0 != cmpResult)
    {
        (void)ILib_memset(netmaskArray, 0xFF, 16);
        tAddrLen = (addrLen * 8u) - netmask;
        clrByte = tAddrLen >> 3u;
        clrBit = tAddrLen & 0x07u;

        tAddrLen = addrLen - 1u;
        for (index = 0u; index < clrByte; index++)
        {
            netmaskArray[tAddrLen - index] = 0x00;
        }

        tAddrLen = addrLen - 1u - clrByte;
        netmaskArray[tAddrLen] = netmaskArray[tAddrLen] << clrBit;

        for (index = 0u; index < addrLen; index++)
        {
            addr1AndResult[index] = ipAddr1[index] & netmaskArray[index];
            addr2AndResult[index] = ipAddr2[index] & netmaskArray[index];
        }

        cmpResult = ILib_memcmp(addr1AndResult, addr2AndResult, addrLen);
        if (0 == cmpResult)
        {
            ret = E_OK;
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: check if have conflict options
 * Param-Name[in]: sdMsg,entryPos,optStartPosMap
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK-No Error,E_NOT_OK-Error,need response Nack
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_OptionsConflictCheck(const uint8* sdMsg, uint16 entryPos, const uint16* optStartPosMap)
{
    Sd_Type1EntryType type1Entry;
    Sd_IPv4OptionsType ipv4Opt;
    uint16 ipv4EndpTcpNum = 0, ipv4EndpUdpNum = 0u;
#if (STD_ON == SD_IPV6_ENABLE)
    Sd_IPv6OptionsType ipv6Opt;
    uint16 ipv6EndpTcpNum = 0, ipv6EndpUdpNum = 0u;
#endif /* STD_ON == SD_IPV6_ENABLE */
    Std_ReturnType ret = E_OK;
    uint8 idx1stOptions, numOpt1;
    uint8 idx2ndOptions, numOpt2;
    uint8 index;
    uint8 optType;
    uint8 optIndex;
    uint8 optNumSum;

    (void)ILib_memcpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    idx1stOptions = type1Entry.Index1stOptions;
    numOpt1 = type1Entry.NumOfOpt1;
    idx2ndOptions = type1Entry.Index2ndOptions;
    numOpt2 = type1Entry.NumOfOpt2;

    /*@req <SWS_SD_00663>,<SWS_SD_00714> */
    /* Check if entry references two or more options that are in conflict*/
    optNumSum = numOpt1 + numOpt2;
    for (index = 0; index < optNumSum; index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        optType = sdMsg[optStartPosMap[optIndex] + 2u];
        if (SD_IPV4_ENDPOINT_OPTION == optType)
        {
            (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
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
            (void)ILib_memcpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
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

/******************************************************************************/
/*
 * Brief: Check whether the number of options is sufficient
 *        (Check entries reference existing options)
 * Param-Name[in]: instancePtr,sdMsg,entryArrayLen,optNum
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK-No Error,E_NOT_OK-Error,need response Nack
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_OptionsExistCheck(
    const Sd_InstanceType* instancePtr,
    const uint8* sdMsg,
    uint32 entryArrayLen,
    uint8 optNum)
{
    uint32 entryArrayEndIdx = entryArrayLen + SD_FIRST_ENTRY_INDEX;
    Sd_Type1EntryType type1Entry;
    Std_ReturnType ret = E_OK;
    uint16 index;
    uint8 index1stOption;
    uint8 index2ndOption;
    uint8 numOfOpt1;
    uint8 numOfOpt2;
    uint8 optNumFromEntry = 0u;
    uint8 tOptNum;

    /* Calculate options number from entries */
    for (index = SD_FIRST_ENTRY_INDEX; index < entryArrayEndIdx; index += SD_ENTRY_SIZE)
    {
        (void)ILib_memcpy((uint8*)&type1Entry, &sdMsg[index], sizeof(Sd_Type1EntryType));
        index1stOption = type1Entry.Index1stOptions;
        index2ndOption = type1Entry.Index2ndOptions;
        numOfOpt1 = type1Entry.NumOfOpt1;
        numOfOpt2 = type1Entry.NumOfOpt2;

        tOptNum = index1stOption + numOfOpt1;
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
            Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
        }
#else
        SD_UNUSED(instancePtr);
#endif /* SD_DEM_USED == STD_ON */

        ret = E_NOT_OK;
    }

    return ret;
}
#endif /* SD_SERVER_SERVICE_NUM > 0 || SD_CLIENT_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Process when receive FindService entry
 * Param-Name[in]: instancePtr, sdMsg, entryPos
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ParseRxMessage
 */
/******************************************************************************/
#if (SD_SERVER_SERVICE_NUM > 0)
static void Sd_RxFindServiceHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    boolean isRxUseMulticast,
    const uint16* optStartPosMap)
{
    const Sd_ServerServiceType* serverServicePtr;
    const uint8* sdMsg = rxQueueItem->bufPtr;
    Sd_Type1EntryType type1Entry;
    const Sd_ServerTimerType* timerPtr;
    uint32 minorVersion;
    uint32 sendTime;
    uint32 halfMainTime;
    uint16 serviceID, InstanceID;
    uint16 serviceIdx;
    uint8 majorVersion;
    Sd_ServerServicePhaseType serverPhase;

    (void)ILib_memcpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    /* Pick up fields value from entry */
    serviceID = Sd_NtoHs(type1Entry.ServiceID);
    InstanceID = Sd_NtoHs(type1Entry.InstanceID);
    majorVersion = type1Entry.MajorVersion;
    minorVersion = Sd_NtoHl(type1Entry.MinorVersion);

    serverServicePtr = instancePtr->SdServerService;
    for (serviceIdx = 0u; serviceIdx < instancePtr->SdServerServiceNum; serviceIdx++)
    {
/* @req SWS_SD_00717,SWS_SD_00718 */
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
        boolean cfgOptMatchCheckRet = Sd_RxConfigurationOptionMatchCheck(
            instancePtr,
            serverServicePtr,
            NULL_PTR,
            rxQueueItem,
            serviceIdx,
            optStartPosMap);
#else
        SD_UNUSED(optStartPosMap);
#endif /* SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u */
        /*@req <SWS_SD_00486> */
        if (((serviceID == 0xFFFFu) || (serviceID == serverServicePtr->SdServerServiceId))
            && ((InstanceID == 0xFFFFu) || (InstanceID == serverServicePtr->SdServerServiceInstanceId))
            && ((majorVersion == 0xFFu) || (majorVersion == serverServicePtr->SdServerServiceMajorVersion))
            && ((minorVersion == 0xFFFFFFFFu) || (minorVersion == serverServicePtr->SdServerServiceMinorVersion))
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
            && (TRUE == cfgOptMatchCheckRet)
#endif /* SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u */
        )
        {
            Sd_SaveServerServiceHandleIdToSessionCtrlList(
                instancePtr->SdInstanceHandleId,
                &rxQueueItem->remoteAddr,
                serverServicePtr->SdServerServiceHandleId);

            serverPhase = Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId].phase;
            if ((SD_SERVER_PHASE_REPETITION == serverPhase) || (SD_SERVER_PHASE_MAIN == serverPhase))
            {
                /*@req <SWS_SD_00332>,<SWS_SD_00343> */
                sendTime = Sd_CalcServerServiceDelayTime(serverServicePtr, NULL_PTR, isRxUseMulticast);
                if ((sdMsg[SD_HEADER_FLAGS_INDEX] & SD_FLAGS_UNICAST_FLAG_MASK) > 0u)
                {
                    timerPtr = serverServicePtr->SdServerServiceTimerRef;
                    halfMainTime = (timerPtr->SdServerTimerOfferCyclicDelay >> 1u) / SD_MAIN_FUNCTION_CYCLE_TIME;
                    if ((SD_SERVER_PHASE_MAIN == serverPhase) && (timerPtr->SdServerTimerOfferCyclicDelay > 0u)
                        && (Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId].mainTimer < halfMainTime))
                    {
                        /*@req <PRS_SOMEIPSD_00423>,<PRS_SOMEIPSD_00843>*/
                        /* Find messages received with the Unicast Flag set to 1 in main phase, shall
                         * be answered with a multicast RESPONSE if the latest offer was sent 1/2
                         * CYCLIC_OFFER_DELAY or longer ago.*/
                        Sd_BuildOfferServiceEntry(instancePtr, serverServicePtr, rxQueueItem, sendTime, FALSE, FALSE);
                    }
                    else
                    {
                        /* Find messages received in repetition phase,answered with unicast */
                        /* Find messages received with the Unicast Flag set to 1 in main phase, shall
                         * be answered with a unicast response if the latest offer was sent less than 1/2
                         * CYCLIC_OFFER_DELAY ago.*/
                        Sd_BuildOfferServiceEntry(instancePtr, serverServicePtr, rxQueueItem, sendTime, TRUE, FALSE);
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

/******************************************************************************/
/*
 * Brief: Get a random value in range min and max
 * Param-Name[in]: min,max
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ClientServiceInitialWaitEnter
 */
/******************************************************************************/
static uint32 Sd_GetRandomValue(uint32 min, uint32 max)
{
    /* Calculate random value use LCG algorithm whih parameter from Numerical Recipe */
    uint32 randVal = (Sd_RandomSeed * SD_RANDOM_MULTIPLIER) + SD_RANDOM_INCREMENT;

    /* Result in range min and max */
    return (min + (randVal % (max - min + 1u)));
}

/******************************************************************************/
/*
 * Brief: Calculate server service delay time
 *      : If serverServicePtr not equal NULL_PTR,calc time for server service
 *      : If eventHandlerPtr not equal NULL_PTR,calc time for EventHandler
 * Param-Name[in]: serverServicePtr,eventHandlerPtr,rxUseMulticastFlag
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint32 Sd_CalcServerServiceDelayTime(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    boolean isRxUseMulticast)
{
    const Sd_ServerTimerType* timerPtr;
    uint32 tRandom, delayMin, delayMax;

    if (TRUE == isRxUseMulticast)
    {
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
        delayMin = timerPtr->SdServerTimerRequestResponseMinDelay;
        delayMax = timerPtr->SdServerTimerRequestResponseMaxDelay;
        /*@req <SWS_SD_00494>,<SWS_SD_00725> */
        /* Calculate random delay time */
        tRandom = Sd_GetRandomValue(delayMin, delayMax);
        /* ms to times */
        tRandom = Sd_SendTimeAdjust(tRandom / SD_MAIN_FUNCTION_CYCLE_TIME);
    }
    else
    {
        /* The entry not received by Multicast RxPduId, needn't delay */
        tRandom = SD_SEND_ENTRY_IMMEDIATELY;
    }

    return tRandom;
}

#endif /* SD_SERVER_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Process when receive OfferService entry
 * Param-Name[in]: instancePtr, sdMsg, entryPos
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ParseRxMessage
 */
/******************************************************************************/
#if (SD_CLIENT_SERVICE_NUM > 0)
static void Sd_RxOfferServiceHandle(
    const Sd_InstanceType* instancePtr,
    Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    const uint16* optStartPosMap)
{
    const Sd_ClientServiceType* clientServicePtr;
    Sd_Type1EntryType type1Entry;
    const uint8* sdMsg = rxQueueItem->bufPtr;
    uint32 ttl = 0u;
    uint32 minorVersion;
    uint16 serviceID, InstanceID;
    uint16 serviceIdx;
    uint8 majorVersion;
    Sd_ClientServiceRTType* cRtDataPtr;
    Std_ReturnType optionCheck;
    boolean cfgOptMatchCheckRet = TRUE;

    (void)ILib_memcpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    /* Pick up fields value from entry */
    serviceID = Sd_NtoHs(type1Entry.ServiceID);
    InstanceID = Sd_NtoHs(type1Entry.InstanceID);
    majorVersion = type1Entry.MajorVersion;
    (void)ILib_memcpy((((uint8*)&ttl) + 1u), type1Entry.TTL, 3u);
    ttl = Sd_NtoHl(ttl);
    minorVersion = Sd_NtoHl(type1Entry.MinorVersion);

    clientServicePtr = instancePtr->SdClientService;
    for (serviceIdx = 0u; serviceIdx < instancePtr->SdClientServiceNum; serviceIdx++)
    {
/* @req SWS_SD_00717,SWS_SD_00718 */
#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
        cfgOptMatchCheckRet = Sd_RxConfigurationOptionMatchCheck(
            instancePtr,
            NULL_PTR,
            clientServicePtr,
            rxQueueItem,
            serviceIdx,
            optStartPosMap);
#endif
        /*@req <SWS_SD_00487>,<SWS_SD_00488>,<SWS_SD_00489> */
        if ((serviceID == clientServicePtr->SdClientServiceId)
            && (InstanceID == clientServicePtr->SdClientServiceInstanceId)
            && (majorVersion == clientServicePtr->SdClientServiceMajorVersion)
            && ((clientServicePtr->SdClientServiceMinorVersion == 0xFFFFFFFFu)
                || (minorVersion == clientServicePtr->SdClientServiceMinorVersion))
            && (TRUE == cfgOptMatchCheckRet))
        {
            Sd_SaveClientServiceHandleIdToSessionCtrlList(
                instancePtr->SdInstanceHandleId,
                &rxQueueItem->remoteAddr,
                clientServicePtr->SdClientServiceHandleId);

            optionCheck =
                Sd_NecessaryOptionsExistCheck(instancePtr, NULL_PTR, clientServicePtr, sdMsg, entryPos, optStartPosMap);
            if (E_OK == optionCheck)
            {
                cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
                cRtDataPtr->offerServiceRxFlag = TRUE;
                cRtDataPtr->rxTcpEndpointFlag = FALSE;
                cRtDataPtr->rxUdpEndpointFlag = FALSE;
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE) && (SD_CONSUMED_EVENTGROUP_NUM > 0)
                Sd_ClientServiceResetAllCEgSubsEntrySubsRetryCnt(clientServicePtr);
#endif /* STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE) && (SD_CONSUMED_EVENTGROUP_NUM > 0 */

                /* OfferService */
                if (0u != ttl)
                {
                    if (SD_CLIENT_PHASE_DOWN == cRtDataPtr->phase)
                    {
                        /*@req <SWS_SD_00514> */
                        /* If TTL is set to 0xFFFFFF, the OfferService entry shall be considered valid until the next
                         * reboot.*/
                        Sd_SetTTL(&cRtDataPtr->ttl, ttl);

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
                        cRtDataPtr->initialWaitTimer = 0u;

                        /*@req <SWS_SD_00365> */
                        /* Cancel the repetition timer */
                        cRtDataPtr->repetitionTimer = 0u;

                        /*@req <SWS_SD_00352>,<SWS_SD_00365>,<SWS_SD_00376> */
                        /* A timer shall be set to the TTL value of this entry (TTL timer). */
                        Sd_SetTTL(&cRtDataPtr->ttl, ttl);

                        Sd_ClientHandleSoConBasedOptionInfo(
                            instancePtr,
                            clientServicePtr,
                            sdMsg,
                            entryPos,
                            optStartPosMap,
                            cRtDataPtr);

                        /*@req <SWS_SD_00352>,<SWS_SD_00365> */
                        /* Leave the Initial Wait Phase Enter the Main Phase */
                        if (SD_CLIENT_PHASE_MAIN != cRtDataPtr->phase)
                        {
                            cRtDataPtr->phase = SD_CLIENT_PHASE_MAIN;
                        }
                    }

                    /* Save received offer service communication partner information */
                    cRtDataPtr->offerEntryRemoteAddr = rxQueueItem->remoteAddr;
                    cRtDataPtr->offerEntryRxPduId = rxQueueItem->rxPduId;
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

/******************************************************************************/
/*
 * Brief: Process when receive SubscribeEventgroup entry
 * Param-Name[in]: instancePtr, sdMsg, entryPos
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ParseRxMessage
 */
/******************************************************************************/
#if (SD_EVENT_HANDLER_NUM > 0)
static void Sd_RxSubscribeEventgroupHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    const uint16* optStartPosMap,
    boolean isReplyNAck)
{
    const Sd_ServerServiceType* serverServicePtr;
    const Sd_EventHandlerType* eventHandlerPtr = NULL_PTR;
    Sd_Type2EntryType type2Entry;
    Sd_EventHandlerRTType* ehRTDataPtr;
    const uint8* sdMsg = rxQueueItem->bufPtr;
    uint32 ttl = 0u;
    uint16 serviceID, InstanceID;
    uint16 eventgroupID;
    uint16 eventGroupIdx;
    uint16 serviceIdx;
    uint16 eventHandlerId;
    uint16 serviceHandleId;
    uint8 majorVersion;
    uint8 counter;
    boolean matchFlag;
    boolean fanOutControlFlag = FALSE;
    Sd_ServerServicePhaseType serverPhase;
    Std_ReturnType ret = E_NOT_OK;
    Std_ReturnType optionCheck;
    uint8 tcpEndpointNum;
    uint8 udpEndpointNum;
    SoAd_RoutingGroupIdType routingGroupId;

    (void)ILib_memcpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));
    /* Pick up fields value from entry */
    serviceID = Sd_NtoHs(type2Entry.ServiceID);
    InstanceID = Sd_NtoHs(type2Entry.InstanceID);
    majorVersion = type2Entry.MajorVersion;
    (void)ILib_memcpy((((uint8*)&ttl) + 1u), type2Entry.TTL, 3u);
    ttl = Sd_NtoHl(ttl);
    counter = (uint8)type2Entry.Counter;
    eventgroupID = Sd_NtoHs(type2Entry.EventGroupID);

    serverServicePtr = instancePtr->SdServerService;
    matchFlag = FALSE;
    for (serviceIdx = 0u; serviceIdx < instancePtr->SdServerServiceNum; serviceIdx++)
    {
        /*@req <SWS_SD_00490> */
        if ((serviceID == serverServicePtr->SdServerServiceId)
            && (InstanceID == serverServicePtr->SdServerServiceInstanceId)
            && (majorVersion == serverServicePtr->SdServerServiceMajorVersion))
        {
            Sd_SaveServerServiceHandleIdToSessionCtrlList(
                instancePtr->SdInstanceHandleId,
                &rxQueueItem->remoteAddr,
                serverServicePtr->SdServerServiceHandleId);

            eventHandlerPtr = serverServicePtr->SdEventHandler;
            for (eventGroupIdx = 0u; eventGroupIdx < serverServicePtr->SdEventHandlerNum; eventGroupIdx++)
            {
                if (eventgroupID == eventHandlerPtr->SdEventHandlerEventGroupId)
                {
                    optionCheck = Sd_NecessaryOptionsExistCheck(
                        instancePtr,
                        serverServicePtr,
                        NULL_PTR,
                        sdMsg,
                        entryPos,
                        optStartPosMap);
                    eventHandlerId = eventHandlerPtr->SdEventHandlerHandleId;
                    serviceHandleId = serverServicePtr->SdServerServiceHandleId;
                    serverPhase = Sd_ServerRTData[serviceHandleId].phase;
                    if ((E_OK == optionCheck) && (SD_SERVER_PHASE_DOWN != serverPhase) && (TRUE != isReplyNAck))
                    {
                        Sd_CalcTcpUdpEndpointOptionNum(
                            sdMsg,
                            entryPos,
                            optStartPosMap,
                            &tcpEndpointNum,
                            &udpEndpointNum);

                        ret = E_OK;
                        /*@req <SWS_SD_00320>,<SWS_SD_00333>,<SWS_SD_00344> */
                        if ((tcpEndpointNum > 0u) && (NULL_PTR != eventHandlerPtr->SdEventHandlerTcp))
                        {
                            ret = Sd_FanOutControlTCP(
                                instancePtr,
                                serverServicePtr,
                                eventHandlerPtr,
                                sdMsg,
                                entryPos,
                                optStartPosMap);

                            fanOutControlFlag = TRUE;
                        }

                        if ((E_OK == ret) && (udpEndpointNum > 0u) && (NULL_PTR != eventHandlerPtr->SdEventHandlerUdp))
                        {
                            ret = Sd_FanOutControlUDP(
                                instancePtr,
                                serverServicePtr,
                                eventHandlerPtr,
                                sdMsg,
                                entryPos,
                                optStartPosMap);

                            fanOutControlFlag = TRUE;

                            /* UDP fail,need close TCP opened RoutingPath */
                            if ((tcpEndpointNum > 0u) && (NULL_PTR != eventHandlerPtr->SdEventHandlerTcp)
                                && (E_NOT_OK == ret))
                            {
                                ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
                                routingGroupId =
                                    eventHandlerPtr->SdEventHandlerTcp->SdEventActivationRef->SdRoutingGroupId;
                                (void)SoAd_DisableSpecificRouting(routingGroupId, ehRTDataPtr->soConIdTcp);
                                Sd_DeleteClientFromSubscribeList(
                                    serverServicePtr,
                                    eventHandlerPtr,
                                    &ehRTDataPtr->tcpSubscribeList,
                                    ehRTDataPtr->soConIdTcp,
                                    counter);
                            }
                        }
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

            if (TRUE == matchFlag)
            {
                break;
            }
        }
        serverServicePtr++;
    }

    /* Response to SubscribeEventgroup, StopSubscribe do not need an Ack */
    if (ttl > 0u)
    {
        if (((TRUE == matchFlag) && (E_NOT_OK == ret)) || (FALSE == matchFlag) || (FALSE == fanOutControlFlag))
        {
            /*@req <SWS_SD_00430> */
            /* Response with SubscribeEventgroupNAck */
            if (FALSE == matchFlag)
            {
                /* No match ServerService,Add NACK entry to the first ServerService's send queue*/
                serverServicePtr = instancePtr->SdServerService;
            }
            (void)Sd_BuildSubscribeEventgroupNAckEntry(serverServicePtr, rxQueueItem, entryPos);
        }
        else
        {
            /* Response with SubscribeEventgroupAck */
            ret = Sd_BuildSubscribeEventgroupAckEntry(
                instancePtr,
                serverServicePtr,
                eventHandlerPtr,
                &rxQueueItem->remoteAddr,
                ttl,
                counter);
            if (E_OK == ret)
            {
                /*@req <SWS_SD_00333>,<SWS_SD_00344> */
                /* Call the BswM with the API BswM_Sd_EventHandlerCurrentState() with state
                 * SD_EVENT_HANDLER_REQUESTED only if the state for this EventHandler changed */
                if (SD_EVENT_HANDLER_REQUESTED != Sd_EventHandlerRTData[eventHandlerId].curState)
                {
#if (SD_BSWM_USED == STD_ON)
                    BswM_Sd_EventHandlerCurrentState(eventHandlerId, SD_EVENT_HANDLER_REQUESTED);
#endif
                    Sd_EventHandlerRTData[eventHandlerId].curState = SD_EVENT_HANDLER_REQUESTED;
                }
            }
        }
    }
}
#endif /* SD_EVENT_HANDLER_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Process when receive SubscribeEventgroupAck entry
 * Param-Name[in]: instancePtr, sdMsg, entryPos
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ParseRxMessage
 */
/******************************************************************************/
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
static void Sd_RxSubscribeEventgroupAckHandle(
    const Sd_InstanceType* instancePtr,
    Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    const uint16* optStartPosMap)
{
    const Sd_ClientServiceType* clientServicePtr;
    const Sd_ConsumedEventGroupType* consumedEventGroupPtr;
    Sd_Type2EntryType type2Entry;
    Sd_ConsumedEventGroupRTType* consumedEgRTPtr;
    Sd_ClientServiceRTType* cRtDataPtr;
    uint8* sdMsg = rxQueueItem->bufPtr;
    TcpIp_SockAddrType multicastIpAddr;
    uint32 ttl = 0u;
    uint16 serviceID, InstanceID;
    uint16 eventgroupID;
    uint16 serviceIdx;
    uint16 consumedEventGroupIdx;
    uint8 majorVersion;
    uint8 counter;
    boolean matchFlag;
    boolean isFindAck;
    Std_ReturnType ret;

    (void)ILib_memcpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));
    /* Pick up fields value from entry */
    serviceID = Sd_NtoHs(type2Entry.ServiceID);
    InstanceID = Sd_NtoHs(type2Entry.InstanceID);
    majorVersion = type2Entry.MajorVersion;
    (void)ILib_memcpy((((uint8*)&ttl) + 1u), type2Entry.TTL, 3u);
    ttl = Sd_NtoHl(ttl);
    counter = (uint8)type2Entry.Counter;
    eventgroupID = Sd_NtoHs(type2Entry.EventGroupID);

    clientServicePtr = instancePtr->SdClientService;
    matchFlag = FALSE;
    for (serviceIdx = 0u; serviceIdx < instancePtr->SdClientServiceNum; serviceIdx++)
    {
        /*@req <SWS_SD_00486> */
        if ((serviceID == clientServicePtr->SdClientServiceId)
            && (InstanceID == clientServicePtr->SdClientServiceInstanceId)
            && (majorVersion == clientServicePtr->SdClientServiceMajorVersion))
        {
            Sd_SaveClientServiceHandleIdToSessionCtrlList(
                instancePtr->SdInstanceHandleId,
                &rxQueueItem->remoteAddr,
                clientServicePtr->SdClientServiceHandleId);

            consumedEventGroupPtr = clientServicePtr->SdConsumedEventGroup;
            for (consumedEventGroupIdx = 0u; consumedEventGroupIdx < clientServicePtr->SdConsumedEventGroupNum;
                 consumedEventGroupIdx++)
            {
                if (eventgroupID == consumedEventGroupPtr->SdConsumedEventGroupId)
                {
                    cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];

                    consumedEgRTPtr = &Sd_ConsumedEventGroupRTData[consumedEventGroupPtr->SdConsumedEventGroupHandleId];
                    /* SubscribeEventgroupAck */
                    if (0u != ttl)
                    {
                        consumedEgRTPtr->isAnsweredWithAck = TRUE;
                        if (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened)
                        {
                            consumedEgRTPtr->isTcpSubsSend = TRUE;
                        }
                        if (TRUE == cRtDataPtr->udpSoCon.isSoConOpened)
                        {
                            consumedEgRTPtr->isUdpSubsSend = TRUE;
                        }

                        /*@req <SWS_SD_00377> */
                        if (FALSE == consumedEgRTPtr->isSubscribed)
                        {
                            consumedEgRTPtr->isSubscribed = TRUE;

                            /*@req <SWS_SD_00704>,<SWS_SD_00377> */
                            if (NULL_PTR != consumedEventGroupPtr->SdConsumedEventGroupMulticastGroupRef)
                            {
                                ret = Sd_GetInfoFromMulticastOption(
                                    instancePtr,
                                    sdMsg,
                                    entryPos,
                                    optStartPosMap,
                                    &multicastIpAddr);
                                if (E_OK == ret)
                                {
                                    Sd_ClientSaveMulticastSoConId(
                                        instancePtr,
                                        consumedEventGroupPtr,
                                        &multicastIpAddr,
                                        &cRtDataPtr->serverUdpAddr,
                                        &consumedEgRTPtr->multiSoCon);
                                }
                            }
#if (SD_BSWM_USED == STD_ON)
                            BswM_Sd_ConsumedEventGroupCurrentState(
                                consumedEventGroupPtr->SdConsumedEventGroupHandleId,
                                SD_CONSUMED_EVENTGROUP_AVAILABLE);
#endif
                            consumedEgRTPtr->curState = SD_CONSUMED_EVENTGROUP_AVAILABLE;
                            Sd_SetTTL(&consumedEgRTPtr->ttl, ttl);
                        }
                    }
                    else
                    {
                        /* SubscribeEventgroupNAck */
                        /*@req <SWS_SD_00465> */
                        isFindAck = Sd_IsHaveEventGroupAckInSameMessage(
                            sdMsg,
                            serviceID,
                            InstanceID,
                            eventgroupID,
                            majorVersion,
                            counter);
                        if (FALSE == isFindAck)
                        {
#if (SD_DEM_USED == STD_ON)
                            const Sd_DemEventParameterType* demEventPtr;
                            demEventPtr = instancePtr->SdInstanceDemEventParameterRefs->SD_E_SUBSCR_NACK_RECV;
                            if (NULL_PTR != demEventPtr)
                            {
                                Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
                            }
#endif

                            /* If SdClientServiceTcpRef is configured,restart the TCP connection */
                            if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef)
                                && (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened))
                            {
                                (void)SoAd_CloseSoCon(cRtDataPtr->tcpSoCon.soConId, TRUE);
                                (void)SoAd_OpenSoCon(cRtDataPtr->tcpSoCon.soConId);
                            }
                        }
                    }

/* @req SWS_SD_00738 */
/* Cancel the corresponding client service subscription retry delay timer and
 * reset subscription retry counter of all corresponding Eventgroups to 0 */
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
                    consumedEgRTPtr->subsRetryTimer = 0u;
                    consumedEgRTPtr->subsRetryCnt = 0u;
                    consumedEgRTPtr->subsRetryEnable = FALSE;
#endif

                    /* Find matched client service,stop search */
                    matchFlag = TRUE;
                    break;
                }

                consumedEventGroupPtr++;
            }

            if (TRUE == matchFlag)
            {
                break;
            }
        }
        clientServicePtr++;
    }
}

/******************************************************************************/
/*
 * Brief: Get addr and port information from Multicast option
 * Param-Name[in]: instancePtr,sdMsg,entryPos,optStartPosMap
 * Param-Name[out]: ipAddrPtr
 * Param-Name[in/out]: None
 * Return: E_OK:Find Multicast option and get info, E_NOT_OK:Get info failure
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_GetInfoFromMulticastOption(
    const Sd_InstanceType* instancePtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap,
    TcpIp_SockAddrType* ipAddrPtr)
{
    Sd_Type2EntryType type2Entry;
    Sd_IPv4OptionsType ipv4Opt;
#if (STD_ON == SD_IPV6_ENABLE)
    Sd_IPv6OptionsType ipv6Opt;
#endif
    uint8 idx1stOptions, numOpt1;
    uint8 idx2ndOptions, numOpt2;
    uint8 index;
    uint8 optIndex;
    uint8 optType;
    Std_ReturnType ret = E_NOT_OK;
    uint8 optNumSum;

    (void)ILib_memcpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));
    idx1stOptions = type2Entry.Index1stOptions;
    numOpt1 = type2Entry.NumOfOpt1;
    idx2ndOptions = type2Entry.Index2ndOptions;
    numOpt2 = type2Entry.NumOfOpt2;

    optNumSum = numOpt1 + numOpt2;
    for (index = 0; index < optNumSum; index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        optType = sdMsg[optStartPosMap[optIndex] + 2u];
        if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
        {
            if (SD_IPV4_MULTICAST_OPTION == optType)
            {
                (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
                ipAddrPtr->domain = TCPIP_AF_INET;
                (void)ILib_memcpy((uint8*)ipAddrPtr->addr, (const uint8*)ipv4Opt.IPv4Address, 4u);
                ipAddrPtr->port = Sd_NtoHs(ipv4Opt.PortNumber);
                ret = E_OK;
                break;
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else
        {
            if (SD_IPV6_MULTICAST_OPTION == optType)
            {
                (void)ILib_memcpy(uint8*)(&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
                ipAddrPtr->domain = TCPIP_AF_INET;
                (void)ILib_memcpy((uint8*)ipAddrPtr->addr, (const uint8*)ipv6Opt.IPv6Address, 16u);
                ipAddrPtr->port = Sd_NtoHs(ipv6Opt.PortNumber);
                ret = E_OK;
                break;
            }
        }
#endif /* STD_ON == SD_IPV6_ENABLE */
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: check if have SubscribeEventGroupAckEntry in same message
 * Param-Name[in]: sdMsg,serviceID,instanceID,eventgroupID,majorVersion,counter
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static boolean Sd_IsHaveEventGroupAckInSameMessage(
    const uint8* sdMsg,
    uint16 serviceID,
    uint16 instanceID,
    uint16 eventgroupID,
    uint8 majorVersion,
    uint8 counter)
{
    Sd_Type2EntryType type2Entry;
    uint32 entryArrayLen;
    uint32 entryArrayEndIdx;
    uint32 idx;
    uint32 tTTL = 0u;
    uint16 tServiceID;
    uint16 tInstanceID;
    uint16 tEventgroupID;
    uint8 tMajorVersion;
    uint8 tCounter;
    uint8 entryType;
    boolean ret = FALSE;

    entryArrayLen = Sd_NPtrtoHl(&sdMsg[SD_ENTRIES_ARRAY_LENGTH_INDEX]);
    entryArrayEndIdx = entryArrayLen + SD_FIRST_ENTRY_INDEX;
    for (idx = SD_FIRST_ENTRY_INDEX; idx < entryArrayEndIdx; idx += SD_ENTRY_SIZE)
    {
        (void)ILib_memcpy((uint8*)&type2Entry, &sdMsg[idx], sizeof(Sd_Type2EntryType));
        entryType = type2Entry.Type;
        (void)ILib_memcpy((((uint8*)&tTTL) + 1u), type2Entry.TTL, 3u);
        tTTL = Sd_NtoHl(tTTL);
        if ((SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK == entryType) && (0u != tTTL))
        {
            /* Pick up fields value from entry */
            tServiceID = Sd_NtoHs(type2Entry.ServiceID);
            tInstanceID = Sd_NtoHs(type2Entry.InstanceID);
            tMajorVersion = type2Entry.MajorVersion;
            tCounter = (uint8)type2Entry.Counter;
            tEventgroupID = Sd_NtoHs(type2Entry.EventGroupID);
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
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Build a FindService entry and options
 * Param-Name[in]: sdInstancePtr,clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Pointer to OfferService Entry,NULL if buffer not enough
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (SD_CLIENT_SERVICE_NUM > 0)
static Std_ReturnType Sd_BuildFindServiceEntry(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    Sd_Type1EntryType* findSvsPtr;
    Sd_SendQueueType* sendQueueObjPtr;
    Sd_ClientServiceRTType* cRtDataPtr;
    uint8* optionPtr = NULL_PTR;
    uint32 ttl;
    uint16 optionsLen;
    uint16 cfgOptLen;
    uint8 optCnt = 0u;
    Std_ReturnType ret = E_OK;

    sendQueueObjPtr = (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool);
    findSvsPtr = (Sd_Type1EntryType*)Sd_MallocEx(sizeof(Sd_Type1EntryType), Sd_TxMemPool);
    cfgOptLen = Sd_GetType1ConfigurationOptionLength(sdInstancePtr, NULL_PTR, clientServicePtr);
    optionsLen = cfgOptLen;
    if (optionsLen > 0u)
    {
        optionPtr = (uint8*)Sd_MallocEx(optionsLen, Sd_TxMemPool);
    }

    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == findSvsPtr) || ((optionsLen > 0u) && (NULL_PTR == optionPtr)))
    {
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool);
        Sd_FreeEx(findSvsPtr, Sd_TxMemPool);
        Sd_FreeEx(optionPtr, Sd_TxMemPool);
        ret = E_NOT_OK;
    }
    else
    {
        /* Clear new allocated memory */
        (void)ILib_memset(sendQueueObjPtr, 0x00, sizeof(Sd_SendQueueType));
        (void)ILib_memset(findSvsPtr, 0x00, sizeof(Sd_Type1EntryType));
        if (optionsLen > 0u)
        {
            (void)ILib_memset(optionPtr, 0x00, optionsLen);
        }

        findSvsPtr->Type = SD_ENTRY_TYPE_FIND_SERVICE;
        findSvsPtr->Index1stOptions = 0u;
        findSvsPtr->Index2ndOptions = 0u;
        findSvsPtr->NumOfOpt1 = 0u;
        findSvsPtr->NumOfOpt2 = 0u;
        findSvsPtr->ServiceID = Sd_HtoNs(clientServicePtr->SdClientServiceId);
        findSvsPtr->InstanceID = Sd_HtoNs(clientServicePtr->SdClientServiceInstanceId);
        findSvsPtr->MajorVersion = clientServicePtr->SdClientServiceMajorVersion;
        ttl = Sd_HtoNl(clientServicePtr->SdClientServiceTimerRef->SdClientTimerTTL);
        (void)ILib_memcpy(findSvsPtr->TTL, ((uint8*)&ttl + 1), 3);
        findSvsPtr->MinorVersion = Sd_HtoNl(clientServicePtr->SdClientServiceMinorVersion);

        if (cfgOptLen > 0u)
        {
            optCnt++;
            Sd_BuildType1ConfigurationOption(optionPtr, cfgOptLen, sdInstancePtr, NULL_PTR, clientServicePtr);
        }

        /* Save information to sendQueueObj */
        sendQueueObjPtr->entryPtr = (uint8*)findSvsPtr;
        sendQueueObjPtr->optionPtr = optionPtr;
        sendQueueObjPtr->optStartPos[0] = 0u;
        sendQueueObjPtr->optStartPos[1] = 0u;
        sendQueueObjPtr->optStartPos[2] = 0u;
        sendQueueObjPtr->optionLen = optionsLen;
        sendQueueObjPtr->optionNum = optCnt;
        sendQueueObjPtr->isUnicastSend = FALSE;
        sendQueueObjPtr->sendTime = SD_SEND_ENTRY_IMMEDIATELY;
        sendQueueObjPtr->next = NULL_PTR;
        /* Add entry to send queue */
        cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
        Sd_AppendToSendQueueTail(&cRtDataPtr->head, sendQueueObjPtr);
    }

    return ret;
}
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Build OfferService entry and options,then add it to SendQueue
 * Param-Name[in]: instancePtr, serverServicePtr, rxQueueItem, sendTime, isUnicastSend, isStopOfferService
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (SD_SERVER_SERVICE_NUM > 0)
static void Sd_BuildOfferServiceEntry(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint32 sendTime,
    boolean isUnicastSend,
    boolean isStopOfferService)
{
    Sd_Type1EntryType* offerServerEntryPtr;
    Sd_SendQueueType* sendQueueObjPtr;
    uint8* optionsPtr;

    sendQueueObjPtr = (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool);
    if (NULL_PTR != sendQueueObjPtr)
    {
        /* Clear new allocated memory */
        (void)ILib_memset(sendQueueObjPtr, 0x00, sizeof(Sd_SendQueueType));
    }
    offerServerEntryPtr = Sd_CreateOfferServiceEntry(serverServicePtr, isStopOfferService);
    optionsPtr = Sd_CreateOfferServiceOptions(instancePtr, serverServicePtr, sendQueueObjPtr);
    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == offerServerEntryPtr) || (NULL_PTR == optionsPtr))
    {
        /* Any buffer allocate fail,free all allocated buffer */
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool);
        Sd_FreeEx(offerServerEntryPtr, Sd_TxMemPool);
        Sd_FreeEx(optionsPtr, Sd_TxMemPool);
    }
    else
    {
        /* Add to send queue */
        if (TRUE == isUnicastSend)
        {
            (void)ILib_memcpy(&sendQueueObjPtr->destAddr, &rxQueueItem->remoteAddr, sizeof(rxQueueItem->remoteAddr));
        }
        sendQueueObjPtr->entryPtr = (uint8*)offerServerEntryPtr;
        sendQueueObjPtr->optionPtr = optionsPtr;
        sendQueueObjPtr->isUnicastSend = isUnicastSend;
        sendQueueObjPtr->sendTime = Sd_SendTimeAdjust(sendTime);
        sendQueueObjPtr->next = NULL_PTR;
        Sd_AppendToSendQueueTail(&Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId].head, sendQueueObjPtr);
    }
}
#endif /* SD_SERVER_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Build a SubscribeEventgroup entry and Options
 * Param-Name[in]: sdInstancePtr,clientServicePtr,eventGroupPtr,rxQueueItem,cRtDataPtr,isRxUseMulticast
 * Param-Name[out]: sendTime
 * Param-Name[in/out]: None
 * Return: E_OK: No Error, E_NOT_OK: Error Occur
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
static Std_ReturnType Sd_BuildSubscribeEventgroupEntry(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const TcpIp_SockAddrType* remoteAddr,
    Sd_ClientServiceRTType* cRtDataPtr,
    boolean isRxUseMulticast,
    uint32* sendTime)
{
    Sd_ConsumedEventGroupRTType* consumedEgRTPtr;
    Sd_SendQueueType* sendQueueObjPtr;
    Sd_Type2EntryType* entryPtr;
    uint8* ipv4OptPtr;
#if (STD_ON == SD_IPV6_ENABLE)
    uint8* ipv6OptPtr;
#endif
    uint8* optionPtr = NULL_PTR;
    uint32 ttl;
    TcpIp_SockAddrType localAddr, defaultRouter;
    SoAd_SoConIdType soConId;
    uint16 optStartPos[3] = {0};
    uint16 optionsLen = 0u;
    uint16 bufOffset;
    uint16 cfgOptLen;
    uint8 optCnt = 0u;
    uint8 netmask;
    Std_ReturnType ret = E_OK;

    /* Calculate required buffer length of options */
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef)
        && (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened))
    {
        optionsLen = sdInstancePtr->SdOptionLength;
    }
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
        && (TRUE == cRtDataPtr->udpSoCon.isSoConOpened))
    {
        optionsLen += sdInstancePtr->SdOptionLength;
    }
    cfgOptLen =
        Sd_GetType2ConfigurationOptionLength(sdInstancePtr, NULL_PTR, NULL_PTR, clientServicePtr, eventGroupPtr);
    optionsLen += cfgOptLen;

    sendQueueObjPtr = (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool);
    entryPtr = (Sd_Type2EntryType*)Sd_MallocEx(sizeof(Sd_Type2EntryType), Sd_TxMemPool);
    if (optionsLen > 0u)
    {
        optionPtr = (uint8*)Sd_MallocEx(optionsLen, Sd_TxMemPool);
    }
    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == entryPtr) || ((optionsLen > 0u) && (NULL_PTR == optionPtr)))
    {
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool);
        Sd_FreeEx(entryPtr, Sd_TxMemPool);
        Sd_FreeEx(optionPtr, Sd_TxMemPool);
        ret = E_NOT_OK;
    }
    else
    {
        consumedEgRTPtr = &Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];

        /* Clear new allocated memory */
        (void)ILib_memset(sendQueueObjPtr, 0x00, sizeof(Sd_SendQueueType));
        (void)ILib_memset(entryPtr, 0x00, sizeof(Sd_Type2EntryType));
        if (optionsLen > 0u)
        {
            (void)ILib_memset(optionPtr, 0x00, optionsLen);
        }

        /* Send Queue object initialization */
        sendQueueObjPtr->optionNum = 0u;

        entryPtr->Type = SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP;
        entryPtr->Index1stOptions = 0u;
        entryPtr->Index2ndOptions = 0u;
        entryPtr->NumOfOpt1 = 0u;
        entryPtr->NumOfOpt2 = 0u;
        entryPtr->ServiceID = Sd_HtoNs(clientServicePtr->SdClientServiceId);
        entryPtr->InstanceID = Sd_HtoNs(clientServicePtr->SdClientServiceInstanceId);
        entryPtr->MajorVersion = clientServicePtr->SdClientServiceMajorVersion;
        ttl = Sd_HtoNl(clientServicePtr->SdClientServiceTimerRef->SdClientTimerTTL);
        (void)ILib_memcpy(entryPtr->TTL, ((uint8*)&ttl) + 1, 3);
        entryPtr->Counter = eventGroupPtr->SdConsumedEventGroupCounter;
        entryPtr->EventGroupID = Sd_HtoNs(eventGroupPtr->SdConsumedEventGroupId);

        bufOffset = 0u;
        localAddr.domain = sdInstancePtr->SdAddrFamily;
        defaultRouter.domain = sdInstancePtr->SdAddrFamily;
        if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef)
            && (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened))
        {
            optStartPos[optCnt] = bufOffset;
            optCnt++;

            /*@req <SWS_SD_00701>,<SWS_SD_00702> */
            soConId = cRtDataPtr->tcpSoCon.soConId;
            (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);
            if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
            {
                ipv4OptPtr = &optionPtr[bufOffset];
                Sd_SetIPv4EndpointOption(ipv4OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_TCP, localAddr.port);
                bufOffset += 12u;
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else
            {
                ipv6OptPtr = &optionPtr[bufOffset];
                Sd_SetIPv6EndpointOption(ipv6OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_TCP, localAddr.port);
                bufOffset += 24u;
            }
#endif /* STD_ON == SD_IPV6_ENABLE */

            /*@req <SWS_SD_00702> */
            (void)SoAd_EnableSpecificRouting(
                eventGroupPtr->SdConsumedEventGroupTcpActivationRef->SdRoutingGroupId,
                soConId);
        }

        if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
            && (TRUE == cRtDataPtr->udpSoCon.isSoConOpened))
        {
            optStartPos[optCnt] = bufOffset;
            optCnt++;

            /*@req <SWS_SD_00701>,<SWS_SD_00703> */
            soConId = cRtDataPtr->udpSoCon.soConId;
            (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);
            if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
            {
                ipv4OptPtr = &optionPtr[bufOffset];
                Sd_SetIPv4EndpointOption(ipv4OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_UDP, localAddr.port);
                bufOffset += 12u;
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else
            {
                ipv6OptPtr = &optionPtr[bufOffset];
                Sd_SetIPv6EndpointOption(ipv6OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_UDP, localAddr.port);
                bufOffset += 24u;
            }
#endif /* STD_ON == SD_IPV6_ENABLE */

            /*@req <SWS_SD_00703> */
            (void)SoAd_EnableSpecificRouting(
                eventGroupPtr->SdConsumedEventGroupUdpActivationRef->SdRoutingGroupId,
                soConId);
        }

        if (cfgOptLen > 0u)
        {
            optStartPos[optCnt] = bufOffset;
            optCnt++;

            Sd_BuildType2ConfigurationOption(
                sdInstancePtr,
                NULL_PTR,
                NULL_PTR,
                clientServicePtr,
                &optionPtr[bufOffset],
                cfgOptLen);
        }

        /* Save information to sendQueueObj */
        (void)ILib_memcpy(&sendQueueObjPtr->destAddr, remoteAddr, sizeof(TcpIp_SockAddrType));
        sendQueueObjPtr->entryPtr = (uint8*)entryPtr;
        sendQueueObjPtr->optionPtr = optionPtr;
        sendQueueObjPtr->optStartPos[0] = optStartPos[0];
        sendQueueObjPtr->optStartPos[1] = optStartPos[1];
        sendQueueObjPtr->optStartPos[2] = optStartPos[2];
        sendQueueObjPtr->optionLen = optionsLen;
        sendQueueObjPtr->optionNum = optCnt;
        sendQueueObjPtr->isUnicastSend = TRUE;
        sendQueueObjPtr->sendTime = Sd_CalcClientServiceDelayTime(eventGroupPtr, isRxUseMulticast);
        sendQueueObjPtr->next = NULL_PTR;
        /* Add entry to send queue */
        Sd_AppendToSendQueueTail(&cRtDataPtr->head, sendQueueObjPtr);

        consumedEgRTPtr->isSubscribed = FALSE;
        *sendTime = sendQueueObjPtr->sendTime;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Build a StopSubscribeEventgroup Entry and options
 * Param-Name[in]: sdInstancePtr,clientServicePtr,eventGroupPtr,remoteAddr,cRtDataPtr,isRxUseMulticast
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK: No Error, E_NOT_OK: Error Occur
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_BuildStopSubscribeEventgroupEntry(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr,
    const TcpIp_SockAddrType* remoteAddr,
    Sd_ClientServiceRTType* cRtDataPtr,
    boolean isRxUseMulticast)
{
    Sd_ConsumedEventGroupRTType* cEgRTPtr;
    Sd_SendQueueType* sendQueueObjPtr;
    Sd_Type2EntryType* entryPtr;
    uint8* optionPtr = NULL_PTR;
    uint8* ipv4OptPtr;
#if (STD_ON == SD_IPV6_ENABLE)
    uint8* ipv6OptPtr;
#endif
    uint32 ttl;
    TcpIp_SockAddrType localAddr, defaultRouter;
    SoAd_SoConIdType soConId;
    SoAd_RoutingGroupIdType routingGroupId;
    uint16 optStartPos[3] = {0};
    uint16 optionsLen = 0u;
    uint16 bufOffset;
    uint16 cfgOptLen;
    uint8 optCnt = 0u;
    uint8 netmask;
    Std_ReturnType ret = E_OK;

    /* Calculate required buffer length of options */
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef)
        && (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened))
    {
        optionsLen = sdInstancePtr->SdOptionLength;
    }
    if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
        && (TRUE == cRtDataPtr->udpSoCon.isSoConOpened))
    {
        optionsLen += sdInstancePtr->SdOptionLength;
    }
    cfgOptLen =
        Sd_GetType2ConfigurationOptionLength(sdInstancePtr, NULL_PTR, NULL_PTR, clientServicePtr, eventGroupPtr);
    optionsLen += cfgOptLen;

    sendQueueObjPtr = (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool);
    entryPtr = (Sd_Type2EntryType*)Sd_MallocEx(sizeof(Sd_Type2EntryType), Sd_TxMemPool);
    if (optionsLen > 0u)
    {
        optionPtr = (uint8*)Sd_MallocEx(optionsLen, Sd_TxMemPool);
    }
    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == entryPtr) || ((optionsLen > 0u) && (NULL_PTR == optionPtr)))
    {
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool);
        Sd_FreeEx(entryPtr, Sd_TxMemPool);
        Sd_FreeEx(optionPtr, Sd_TxMemPool);
        ret = E_NOT_OK;
    }
    else
    {
        /* Clear new allocated memory */
        (void)ILib_memset(sendQueueObjPtr, 0x00, sizeof(Sd_SendQueueType));
        (void)ILib_memset(entryPtr, 0x00, sizeof(Sd_Type2EntryType));
        if (optionsLen > 0u)
        {
            (void)ILib_memset(optionPtr, 0x00, optionsLen);
        }

        /* Send Queue object initialization */
        sendQueueObjPtr->optionNum = 0u;

        entryPtr->Type = SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP;
        entryPtr->Index1stOptions = 0u;
        entryPtr->Index2ndOptions = 0u;
        entryPtr->NumOfOpt1 = 0u;
        entryPtr->NumOfOpt2 = 0u;
        entryPtr->ServiceID = Sd_HtoNs(clientServicePtr->SdClientServiceId);
        entryPtr->InstanceID = Sd_HtoNs(clientServicePtr->SdClientServiceInstanceId);
        entryPtr->MajorVersion = clientServicePtr->SdClientServiceMajorVersion;
        ttl = 0u;
        (void)ILib_memcpy(entryPtr->TTL, ((uint8*)&ttl) + 1, 3);
        entryPtr->Counter = eventGroupPtr->SdConsumedEventGroupCounter;
        entryPtr->EventGroupID = Sd_HtoNs(eventGroupPtr->SdConsumedEventGroupId);

        bufOffset = 0u;
        localAddr.domain = sdInstancePtr->SdAddrFamily;
        defaultRouter.domain = sdInstancePtr->SdAddrFamily;
        if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef)
            && (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened))
        {
            optStartPos[optCnt] = bufOffset;
            optCnt++;

            /*@req <SWS_SD_00701> */
            soConId = cRtDataPtr->tcpSoCon.soConId;
            (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);
            if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
            {
                ipv4OptPtr = &optionPtr[bufOffset];
                Sd_SetIPv4EndpointOption(ipv4OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_TCP, localAddr.port);
                bufOffset += 12u;
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else
            {
                ipv6OptPtr = &optionPtr[bufOffset];
                Sd_SetIPv6EndpointOption(ipv6OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_TCP, localAddr.port);
                bufOffset += 24u;
            }
#endif /* STD_ON == SD_IPV6_ENABLE */
        }

        if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
            && (TRUE == cRtDataPtr->udpSoCon.isSoConOpened))
        {
            optStartPos[optCnt] = bufOffset;
            optCnt++;

            /*@req <SWS_SD_00701> */
            soConId = cRtDataPtr->udpSoCon.soConId;
            (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);
            if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
            {
                ipv4OptPtr = &optionPtr[bufOffset];
                Sd_SetIPv4EndpointOption(ipv4OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_UDP, localAddr.port);
                bufOffset += 12u;
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else
            {
                ipv6OptPtr = &optionPtr[bufOffset];
                Sd_SetIPv6EndpointOption(ipv6OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_UDP, localAddr.port);
                bufOffset += 24u;
            }
#endif /* STD_ON == SD_IPV6_ENABLE */
        }

        if (cfgOptLen > 0u)
        {
            optStartPos[optCnt] = bufOffset;
            optCnt++;

            Sd_BuildType2ConfigurationOption(
                sdInstancePtr,
                NULL_PTR,
                NULL_PTR,
                clientServicePtr,
                &optionPtr[bufOffset],
                cfgOptLen);
        }

        /*@req <SWS_SD_00711> */
        /*Deactive all the routing for all relevant Routing Groups of this Eventgroup */
        if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef)
            && (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened))
        {
            routingGroupId = eventGroupPtr->SdConsumedEventGroupTcpActivationRef->SdRoutingGroupId;
            (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->tcpSoCon.soConId);
        }
        if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
            && (TRUE == cRtDataPtr->udpSoCon.isSoConOpened))
        {
            routingGroupId = eventGroupPtr->SdConsumedEventGroupUdpActivationRef->SdRoutingGroupId;
            (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->udpSoCon.soConId);
        }
        cEgRTPtr = &Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
        if ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef)
            && (TRUE == cEgRTPtr->multiSoCon.isSoConOpened))
        {
            routingGroupId = eventGroupPtr->SdConsumedEventGroupMulticastActivationRef->SdRoutingGroupId;
            (void)SoAd_DisableSpecificRouting(routingGroupId, cEgRTPtr->multiSoCon.soConId);
        }

        (void)ILib_memcpy(&sendQueueObjPtr->destAddr, remoteAddr, sizeof(TcpIp_SockAddrType));
        sendQueueObjPtr->entryPtr = (uint8*)entryPtr;
        sendQueueObjPtr->optionPtr = optionPtr;
        sendQueueObjPtr->optStartPos[0] = optStartPos[0];
        sendQueueObjPtr->optStartPos[1] = optStartPos[1];
        sendQueueObjPtr->optStartPos[2] = optStartPos[2];
        sendQueueObjPtr->optionLen = optionsLen;
        sendQueueObjPtr->optionNum = optCnt;
        sendQueueObjPtr->isUnicastSend = TRUE;
        sendQueueObjPtr->sendTime = Sd_CalcClientServiceDelayTime(eventGroupPtr, isRxUseMulticast);
        sendQueueObjPtr->next = NULL_PTR;
        Sd_AppendToSendQueueTail(&cRtDataPtr->head, sendQueueObjPtr);

        cEgRTPtr->isSubscribed = FALSE;
        cEgRTPtr->isTcpSubsSend = FALSE;
        cEgRTPtr->isUdpSubsSend = FALSE;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Calculate client service delay time when receive via Multicast message
 * Param-Name[in]:eventHandlerPtr,rxUseMulticastFlag
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: Random delay time(times)
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint32 Sd_CalcClientServiceDelayTime(const Sd_ConsumedEventGroupType* eventHandlerPtr, boolean isRxUseMulticast)
{
    const Sd_ClientTimerType* timerPtr;
    uint32 tRandom, delayMin, delayMax;

    if (TRUE == isRxUseMulticast)
    {
        /*@req <SWS_SD_00491>,<SWS_SD_00492>,<SWS_SD_00493>,<SWS_SD_00494> */
        /* make the number between SdClientTimerRequestResponseMinDelay to SdClientTimerRequestResponseMaxDelay*/
        timerPtr = eventHandlerPtr->SdConsumedEventGroupTimerRef;
        delayMin = timerPtr->SdClientTimerRequestResponseMinDelay;
        delayMax = timerPtr->SdClientTimerRequestResponseMaxDelay;
        /*@req <SWS_SD_00494>,<SWS_SD_00725> */
        /* Calculate random delay time */
        tRandom = Sd_GetRandomValue(delayMin, delayMax);

        /* ms to times */
        tRandom = Sd_SendTimeAdjust(tRandom / SD_MAIN_FUNCTION_CYCLE_TIME);
    }
    else
    {
        tRandom = SD_SEND_ENTRY_IMMEDIATELY;
    }

    return tRandom;
}
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Build a SubscribeEventgroupAck entry and options
 * Param-Name[in]: instancePtr, serverServicePtr, eventHandlerPtr, remoteAddr, counter
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK: No Error, E_NOT_OK: Error Occur
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (SD_EVENT_HANDLER_NUM > 0)
static Std_ReturnType Sd_BuildSubscribeEventgroupAckEntry(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const TcpIp_SockAddrType* remoteAddr,
    uint32 ttl,
    uint8 counter)
{
    TcpIp_SockAddrType ipAddr;
    Sd_Type2EntryType* egAckPtr;
    uint8* optionPtr = NULL_PTR;
    Sd_SendQueueType* sendQueueObjPtr;
    Sd_ServerServiceRTType* sRtDataPtr;
    uint32 tTtl;
    uint16 optStartPos[3] = {0};
    uint16 optionsLen;
    uint16 multiOptlen = 0u;
    uint16 cfgOptLen;
    uint16 cfgOptOffset = 0u;
    uint8 optNum = 0u;
    Std_ReturnType ret = E_OK;

    if (NULL_PTR != eventHandlerPtr)
    {
        if ((NULL_PTR != eventHandlerPtr->SdEventHandlerUdp) && (NULL_PTR != eventHandlerPtr->SdEventHandlerMulticast))
        {
            /* Need Build Multicast Option */
            multiOptlen = instancePtr->SdOptionLength;
        }
        cfgOptLen =
            Sd_GetType2ConfigurationOptionLength(instancePtr, serverServicePtr, eventHandlerPtr, NULL_PTR, NULL_PTR);
        optionsLen = multiOptlen + cfgOptLen;

        sendQueueObjPtr = (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool);
        egAckPtr = (Sd_Type2EntryType*)Sd_MallocEx(sizeof(Sd_Type1EntryType), Sd_TxMemPool);
        if (optionsLen > 0u)
        {
            optionPtr = (uint8*)Sd_MallocEx(optionsLen, Sd_TxMemPool);
        }

        if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == egAckPtr) || ((optionsLen > 0u) && (NULL_PTR == optionPtr)))
        {
            Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool);
            Sd_FreeEx(egAckPtr, Sd_TxMemPool);
            Sd_FreeEx(optionPtr, Sd_TxMemPool);
            ret = E_NOT_OK;
        }
        else
        {
            /* Clear new allocated memory */
            (void)ILib_memset(sendQueueObjPtr, 0x00, sizeof(Sd_SendQueueType));
            (void)ILib_memset(egAckPtr, 0x00, sizeof(Sd_Type1EntryType));
            if (optionsLen > 0u)
            {
                (void)ILib_memset(optionPtr, 0x00, optionsLen);
            }

            /* Build SubscribeEventgroupAck Entry */
            egAckPtr->Type = SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK;
            egAckPtr->Index1stOptions = 0u;
            egAckPtr->Index2ndOptions = 0u;
            egAckPtr->NumOfOpt1 = 0u;
            egAckPtr->NumOfOpt2 = 0u;
            egAckPtr->ServiceID = Sd_HtoNs(serverServicePtr->SdServerServiceId);
            egAckPtr->InstanceID = Sd_HtoNs(serverServicePtr->SdServerServiceInstanceId);
            egAckPtr->MajorVersion = serverServicePtr->SdServerServiceMajorVersion;
            tTtl = Sd_HtoNl(ttl);
            (void)ILib_memcpy(egAckPtr->TTL, ((uint8*)&tTtl) + 1, 3);
            egAckPtr->Counter = counter;
            egAckPtr->EventGroupID = Sd_HtoNs(eventHandlerPtr->SdEventHandlerEventGroupId);

            /* Build IPv4/IPv6 Multicast Option */
            if (multiOptlen > 0u)
            {
                optStartPos[optNum] = cfgOptOffset;
                optNum++;

                ipAddr.domain = instancePtr->SdAddrFamily;
                ret = SoAd_GetRemoteAddr(
                    eventHandlerPtr->SdEventHandlerMulticast->SdMulticastEventSoConRef->SdSoConId,
                    &ipAddr);
                if (E_OK == ret)
                {
                    if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
                    {
                        Sd_SetIPv4MulticastOption(optionPtr, (uint8*)ipAddr.addr, ipAddr.port);
                        cfgOptOffset = 12;
                    }
#if (STD_ON == SD_IPV6_ENABLE)
                    else
                    {
                        Sd_SetIPv6MulticastOption(optionPtr, (uint8*)ipAddr.addr, ipAddr.port);
                        cfgOptOffset = 24;
                    }
#endif /* STD_ON == SD_IPV6_ENABLE */
                }
            }

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
            (void)ILib_memcpy(&sendQueueObjPtr->destAddr, remoteAddr, sizeof(TcpIp_SockAddrType));
            sendQueueObjPtr->entryPtr = (uint8*)egAckPtr;
            sendQueueObjPtr->optionPtr = optionPtr;
            sendQueueObjPtr->optStartPos[0] = optStartPos[0];
            sendQueueObjPtr->optStartPos[1] = optStartPos[1];
            sendQueueObjPtr->optStartPos[2] = optStartPos[2];
            sendQueueObjPtr->optionLen = optionsLen;
            sendQueueObjPtr->optionNum = optNum;
            sendQueueObjPtr->isUnicastSend = TRUE;
            sendQueueObjPtr->sendTime = SD_SEND_ENTRY_IMMEDIATELY;
            sendQueueObjPtr->next = NULL_PTR;
            /* Add entry to send queue */
            sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
            Sd_AppendToSendQueueTail(&sRtDataPtr->head, sendQueueObjPtr);
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Build a SubscribeEventgroupNAck entry and options
 * Param-Name[in]: serverServicePtr, rxQueueItem, entryPos
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK: No Error, E_NOT_OK: Error Occur
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_BuildSubscribeEventgroupNAckEntry(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint16 entryPos)
{
    Sd_Type2EntryType type2Entry;
    Sd_Type2EntryType* egAckPtr;
    Sd_SendQueueType* sendQueueObjPtr;
    Sd_ServerServiceRTType* sRtDataPtr;
    uint32 ttl;
    Std_ReturnType ret = E_OK;

    sendQueueObjPtr = (Sd_SendQueueType*)Sd_MallocEx(sizeof(Sd_SendQueueType), Sd_TxMemPool);
    egAckPtr = (Sd_Type2EntryType*)Sd_MallocEx(sizeof(Sd_Type1EntryType), Sd_TxMemPool);
    if ((NULL_PTR == sendQueueObjPtr) || (NULL_PTR == egAckPtr))
    {
        Sd_FreeEx(sendQueueObjPtr, Sd_TxMemPool);
        Sd_FreeEx(egAckPtr, Sd_TxMemPool);
        ret = E_NOT_OK;
    }
    else
    {
        /* Clear new allocated memory */
        (void)ILib_memset(sendQueueObjPtr, 0x00, sizeof(Sd_SendQueueType));
        (void)ILib_memset(egAckPtr, 0x00, sizeof(Sd_Type1EntryType));

        (void)ILib_memcpy((uint8*)&type2Entry, &rxQueueItem->bufPtr[entryPos], sizeof(Sd_Type2EntryType));
        /*@req <SWS_SD_00431>,<SWS_SD_00316>,<SWS_SD_00432> */
        /* Build SubscribeEventgroupAck Entry */
        egAckPtr->Type = SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK;
        egAckPtr->Index1stOptions = 0u;
        egAckPtr->Index2ndOptions = 0u;
        egAckPtr->NumOfOpt1 = 0u;
        egAckPtr->NumOfOpt2 = 0u;
        egAckPtr->ServiceID = Sd_HtoNs(Sd_NtoHs(type2Entry.ServiceID));
        egAckPtr->InstanceID = Sd_HtoNs(Sd_NtoHs(type2Entry.InstanceID));
        egAckPtr->MajorVersion = type2Entry.MajorVersion;
        ttl = 0u;
        (void)ILib_memcpy(egAckPtr->TTL, ((uint8*)&ttl) + 1, 3);
        egAckPtr->Counter = type2Entry.Counter;
        egAckPtr->EventGroupID = Sd_HtoNs(Sd_NtoHs(type2Entry.EventGroupID));

        /* Save information to sendQueueObj */
        (void)ILib_memcpy(&sendQueueObjPtr->destAddr, &rxQueueItem->remoteAddr, sizeof(TcpIp_SockAddrType));
        sendQueueObjPtr->entryPtr = (uint8*)egAckPtr;
        sendQueueObjPtr->optionPtr = NULL_PTR;
        sendQueueObjPtr->optStartPos[0] = 0u;
        sendQueueObjPtr->optStartPos[1] = 0u;
        sendQueueObjPtr->optStartPos[2] = 0u;
        sendQueueObjPtr->optionLen = 0u;
        sendQueueObjPtr->optionNum = 0u;
        sendQueueObjPtr->isUnicastSend = TRUE;
        sendQueueObjPtr->sendTime = SD_SEND_ENTRY_IMMEDIATELY;
        sendQueueObjPtr->next = NULL_PTR;
        /* Add entry to send queue */
        sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
        Sd_AppendToSendQueueTail(&sRtDataPtr->head, sendQueueObjPtr);
    }

    return ret;
}
#endif /* SD_EVENT_HANDLER_NUM > 0 */

/******************************************************************************/
/*
 * Brief: When isStopOfferService == FALSE,Build a OfferService entry
 *        When isStopOfferService == TRUE,Build a StopOfferService entry
 * Param-Name[in]: serverServicePtr,isStopOfferService
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Pointer to OfferService's entry,NULL if buffer not enough
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (SD_SERVER_SERVICE_NUM > 0)
static Sd_Type1EntryType* Sd_CreateOfferServiceEntry(
    const Sd_ServerServiceType* serverServicePtr,
    boolean isStopOfferService)
{
    Sd_Type1EntryType* ofSevsPtr;
    uint32 ttl = 0u;

    ofSevsPtr = (Sd_Type1EntryType*)Sd_MallocEx(sizeof(Sd_Type1EntryType), Sd_TxMemPool);
    if (ofSevsPtr != NULL_PTR)
    {
        /* Clear new allocated memory */
        (void)ILib_memset(ofSevsPtr, 0x00, sizeof(Sd_Type1EntryType));

        ofSevsPtr->Type = SD_ENTRY_TYPE_OFFER_SERVICE;
        ofSevsPtr->Index1stOptions = 0u;
        ofSevsPtr->Index2ndOptions = 0u;
        ofSevsPtr->NumOfOpt1 = 0u;
        ofSevsPtr->NumOfOpt2 = 0u;
        ofSevsPtr->ServiceID = Sd_HtoNs(serverServicePtr->SdServerServiceId);
        ofSevsPtr->InstanceID = Sd_HtoNs(serverServicePtr->SdServerServiceInstanceId);
        ofSevsPtr->MajorVersion = serverServicePtr->SdServerServiceMajorVersion;
        if (FALSE == isStopOfferService)
        {
            ttl = Sd_HtoNl(serverServicePtr->SdServerServiceTimerRef->SdServerTimerTTL);
        }
        (void)ILib_memcpy(ofSevsPtr->TTL, ((uint8*)&ttl) + 1, 3);
        ofSevsPtr->MinorVersion = Sd_HtoNl(serverServicePtr->SdServerServiceMinorVersion);
    }

    return ofSevsPtr;
}

/******************************************************************************/
/*
 * Brief: Build a OfferService Options
 * Param-Name[in]: sdInstancePtr,serverServicePtr
 * Param-Name[out]: optNum
 * Param-Name[in/out]: sendQueueObjPtr
 * Return: Pointer to OfferService's Option,NULL if buffer not enough
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint8* Sd_CreateOfferServiceOptions(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    Sd_SendQueueType* sendQueueObjPtr)
{
    uint8* ipv4OptPtr;
#if (STD_ON == SD_IPV6_ENABLE)
    uint8* ipv6OptPtr;
#endif /* STD_ON == SD_IPV6_ENABLE */
    uint8* bufPtr;
    TcpIp_SockAddrType localAddr, defaultRouter;
    uint16 optStartPos[3] = {0};
    uint16 cfgOptlen;
    uint16 udpEndpointOptLen = 0u;
    uint16 tcpEndpointOptLen = 0u;
    uint16 bufLenSum;
    uint16 bufRunIdx;
    uint8 optionNum = 0u;
    uint8 netmask;
    SoAd_SoConIdType soConId;

    if (NULL_PTR == sendQueueObjPtr)
    {
        /* Memory allocate for sendQueueObj fail */
        return NULL_PTR;
    }

    /* Calculate config option length */
    cfgOptlen = Sd_GetType1ConfigurationOptionLength(sdInstancePtr, serverServicePtr, NULL_PTR);
    bufLenSum = cfgOptlen;

    if (NULL_PTR != serverServicePtr->SdServerServiceTcpRef)
    {
        if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
        {
            /* IPv4 Endpoint Option length is 12 */
            tcpEndpointOptLen = 12;
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else
        {
            /* IPv6 Endpoint Option length is 24 */
            tcpEndpointOptLen = 24;
        }
#endif /* STD_ON == SD_IPV6_ENABLE */
    }

    if (NULL_PTR != serverServicePtr->SdServerServiceUdpRef)
    {
        if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
        {
            /* IPv4 */
            udpEndpointOptLen = 12;
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else
        {
            udpEndpointOptLen = 24;
        }
#endif /* STD_ON == SD_IPV6_ENABLE */
    }
    /* Total buffer needed */
    bufLenSum += (tcpEndpointOptLen + udpEndpointOptLen);

    /* According to specification at lease hava one endpoint,so the case that need't to
     * create optins is not exist */
    bufPtr = Sd_MallocEx(bufLenSum, Sd_TxMemPool);
    if (NULL_PTR != bufPtr)
    {
        /* Set all memory to 0x00 */
        (void)ILib_memset(bufPtr, 0x00, bufLenSum);
        bufRunIdx = 0u;

        localAddr.domain = sdInstancePtr->SdAddrFamily;
        defaultRouter.domain = sdInstancePtr->SdAddrFamily;
        if (NULL_PTR != serverServicePtr->SdServerServiceTcpRef)
        {
            /*@req <SWS_SD_00478> */
            soConId = serverServicePtr->SdServerServiceTcpRef->SdSocketIdList[0];
            (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);
            if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
            {
                ipv4OptPtr = &bufPtr[bufRunIdx];
                Sd_SetIPv4EndpointOption(ipv4OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_TCP, localAddr.port);
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else
            {
                ipv6OptPtr = &bufPtr[bufRunIdx];
                Sd_SetIPv6EndpointOption(ipv6OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_TCP, localAddr.port);
            }
#endif /* STD_ON == SD_IPV6_ENABLE */

            optStartPos[optionNum] = bufRunIdx;
            optionNum++;
            bufRunIdx += tcpEndpointOptLen;
        }

        if (NULL_PTR != serverServicePtr->SdServerServiceUdpRef)
        {
            /*@req <SWS_SD_00478> */
            soConId = serverServicePtr->SdServerServiceUdpRef->SdSocketIdList[0];
            (void)SoAd_GetLocalAddr(soConId, &localAddr, &netmask, &defaultRouter);
            if (TCPIP_AF_INET == sdInstancePtr->SdAddrFamily)
            {
                ipv4OptPtr = &bufPtr[bufRunIdx];
                Sd_SetIPv4EndpointOption(ipv4OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_UDP, localAddr.port);
            }
#if (STD_ON == SD_IPV6_ENABLE)
            else
            {
                ipv6OptPtr = &bufPtr[bufRunIdx];
                Sd_SetIPv6EndpointOption(ipv6OptPtr, (uint8*)localAddr.addr, SD_L4_PROTO_UDP, localAddr.port);
            }
#endif /* STD_ON == SD_IPV6_ENABLE */

            optStartPos[optionNum] = bufRunIdx;
            optionNum++;
            bufRunIdx += udpEndpointOptLen;
        }

        /* If SdInstanceHostname or SdServerCapabilityRecord not equal NULL_PTR,
         * build Configuration Option */
        if (cfgOptlen > 0u)
        {
            Sd_BuildType1ConfigurationOption(&bufPtr[bufRunIdx], cfgOptlen, sdInstancePtr, serverServicePtr, NULL_PTR);

            optStartPos[optionNum] = bufRunIdx;
            optionNum++;
        }
    }

    /* Store info to the buffer [sendQueueObjPtr] referenced */
    sendQueueObjPtr->optionLen = bufLenSum;
    sendQueueObjPtr->optionNum = optionNum;
    sendQueueObjPtr->optStartPos[0] = optStartPos[0];
    sendQueueObjPtr->optStartPos[1] = optStartPos[1];
    sendQueueObjPtr->optStartPos[2] = optStartPos[2];

    return bufPtr;
}
#endif /* SD_SERVER_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Calculation of type1 Configuration Endpoint Option length
 *        [serverServicePtr] or [clientServicePtr] must have one equal NULL_PTR
 *        when [serverServicePtr] != NULL_PTR,Calculate config option length for OfferService or StopOfferService
 *        when [clientServicePtr] != NULL_PTR,Calculate config option length for FindService
 * Param-Name[in]: sdInstancePtr, serverServicePtr, clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Type1 Configuration Endpoint Option length
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
static uint16 Sd_GetType1ConfigurationOptionLength(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    const Sd_CapabilityRecordType* recordPtr;
    uint16 recordNum;
    uint16 cfgOptlen = 0u;
    uint16 index;

    if (NULL_PTR != serverServicePtr)
    {
        recordPtr = serverServicePtr->SdServerCapabilityRecord;
        recordNum = serverServicePtr->SdServerCapabilityRecordNum;
    }
    else
    {
        recordPtr = clientServicePtr->SdClientCapabilityRecord;
        recordNum = clientServicePtr->SdClientCapabilityRecordNum;
    }

    if ((NULL_PTR != sdInstancePtr->SdInstanceHostname) || (NULL_PTR != recordPtr))
    {
        cfgOptlen = 5u; /* 5u = Length(2) + Type(1) + Reserved(1) + [0](1)*/
    }

    /* Hostname length */
    if (NULL_PTR != sdInstancePtr->SdInstanceHostname)
    {
        cfgOptlen += (uint16)strlen(sdInstancePtr->SdInstanceHostname);
        cfgOptlen = cfgOptlen + 10u; /* 10u = [len](1) + "Hostname"(8) + "="(1) */
    }

    /* Record length */
    if (NULL_PTR != recordPtr)
    {
        for (index = 0u; index < recordNum; index++)
        {
            cfgOptlen += 1u; /* [len] */
            cfgOptlen += (uint16)strlen(recordPtr->SdCapabilityRecordKey);
            cfgOptlen += 1u; /* '=' */
            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                cfgOptlen += (uint16)strlen(recordPtr->SdCapabilityRecordValue);
            }

            recordPtr++;
        }
    }

    return cfgOptlen;
}

/******************************************************************************/
/*
 * Brief: Build the Configuration Endpoint Option for Type1 entry
 *        [serverServicePtr] or [clientServicePtr] must have one equal NULL_PTR
 *      : If serverServicePtr is not NULL,Build config option for OfferService/StopOfferService
 *      : If clientServicePtr is not NULL,Build config option for FindService
 * Param-Name[in]: cfgOptLen,sdInstancePtr,serverServicePtr,clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: optAddrPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_BuildType1ConfigurationOption(
    uint8* optAddrPtr,
    uint16 cfgOptLen,
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    /* !Note: Don't check buffer enough or not,make sure optAddr point to a
     * buffer enough to store Configuration Option */
    const Sd_CapabilityRecordType* recordPtr;
    const char* hostnameStr = "hostname";
    uint8* optAddr = optAddrPtr;
    uint16 recordNum;
    uint16 index;
    uint16 optLen;
    uint8 keyLen, valueLen;

    if (NULL_PTR != serverServicePtr)
    {
        recordPtr = serverServicePtr->SdServerCapabilityRecord;
        recordNum = serverServicePtr->SdServerCapabilityRecordNum;
    }
    else
    {
        recordPtr = clientServicePtr->SdClientCapabilityRecord;
        recordNum = clientServicePtr->SdClientCapabilityRecordNum;
    }

    optLen = Sd_HtoNs(cfgOptLen - 3u); /* Exclude Length(2Byte) and Type(1Byte) */
    (void)ILib_memcpy(optAddr, (uint8*)&optLen, sizeof(uint16));
    *(uint8*)(&optAddr[2]) = SD_CONFIGURATION_OPTION;

    optAddr = &optAddr[4]; /* To the palce used to save first key-value pair */
    if (NULL_PTR != sdInstancePtr->SdInstanceHostname)
    {
        keyLen = 8u; /* Length of "hostname" */
        valueLen = (uint8)strlen(sdInstancePtr->SdInstanceHostname);
        *optAddr = keyLen + valueLen + 1u; /* '=' length is 1 */
        optAddr++;
        (void)ILib_memcpy(optAddr, (const uint8*)hostnameStr, keyLen);
        optAddr = &optAddr[keyLen];
        *optAddr = (uint8)'=';
        optAddr++;
        (void)ILib_memcpy(optAddr, (const uint8*)sdInstancePtr->SdInstanceHostname, valueLen);
        optAddr = &optAddr[valueLen];
    }

    if (NULL_PTR != recordPtr)
    {
        for (index = 0u; index < recordNum; index++)
        {
            keyLen = (uint8)strlen(recordPtr->SdCapabilityRecordKey);
            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                valueLen = (uint8)strlen(recordPtr->SdCapabilityRecordValue);
            }
            else
            {
                valueLen = 0u;
            }
            *optAddr = keyLen + valueLen + 1u; /* '=' length is 1 */
            optAddr++;
            (void)ILib_memcpy(optAddr, (const uint8*)recordPtr->SdCapabilityRecordKey, keyLen);
            optAddr = &optAddr[keyLen];
            *optAddr = (uint8)'=';
            optAddr++;

            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                (void)ILib_memcpy(optAddr, (const uint8*)recordPtr->SdCapabilityRecordValue, valueLen);
                optAddr = &optAddr[valueLen];
            }

            recordPtr++;
        }
    }

    *optAddr = 0u; /* end with [0] */
}

#if (SD_CONSUMED_EVENTGROUP_NUM > 0) || (SD_EVENT_HANDLER_NUM > 0)
/******************************************************************************/
/*
 * Brief: Calculation of type2 Configuration Endpoint Option length
 *        [eventHandlerPtr] or [eventGroupPtr] must have one equal NULL_PTR
 *        when [eventHandlerPtr] != NULL_PTR,Calculate config option length for SubscribeEventgroupAck/NAck
 *        when [eventGroupPtr] != NULL_PTR,Calculate config option length for
 * SubscribeEventgroup/StopSubscribeEventgroup Param-Name[in]: Param-Name[out]: None Param-Name[in/out]: None Return:
 * None PreCondition: None CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint16 Sd_GetType2ConfigurationOptionLength(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_ConsumedEventGroupType* eventGroupPtr)
{
    const Sd_CapabilityRecordType* recordPtr = NULL_PTR;
    uint16 recordNum = 0u;
    uint16 cfgOptlen = 0u;
    uint16 index;

    if ((NULL_PTR != eventHandlerPtr) && (NULL_PTR != serverServicePtr))
    {
        recordPtr = serverServicePtr->SdServerCapabilityRecord;
        recordNum = serverServicePtr->SdServerCapabilityRecordNum;
    }
    else if ((NULL_PTR != eventGroupPtr) && (NULL_PTR != clientServicePtr))
    {
        recordPtr = clientServicePtr->SdClientCapabilityRecord;
        recordNum = clientServicePtr->SdClientCapabilityRecordNum;
    }
    else
    {
        /* Nothing to do here */
    }

    if ((NULL_PTR != sdInstancePtr->SdInstanceHostname) || (NULL_PTR != recordPtr))
    {
        cfgOptlen = 5u; /* 5u = Length(2) + Type(1) + Reserved(1) + [0](1)*/
    }

    /* Build configuration option */
    if (NULL_PTR != sdInstancePtr->SdInstanceHostname)
    {
        cfgOptlen += (uint16)strlen(sdInstancePtr->SdInstanceHostname);
        cfgOptlen = cfgOptlen + 10u; /* 10u = [len](1) + "Hostname"(8) + "="(1) */
    }

    if (NULL_PTR != recordPtr)
    {
        for (index = 0u; index < recordNum; index++)
        {
            cfgOptlen += 1u; /* [len] */
            cfgOptlen += (uint16)strlen(recordPtr->SdCapabilityRecordKey);
            cfgOptlen += 1u; /* '=' */
            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                cfgOptlen += (uint16)strlen(recordPtr->SdCapabilityRecordValue);
            }

            recordPtr++;
        }
    }

    return cfgOptlen;
}

/******************************************************************************/
/*
 * Brief: Build the Configuration Endpoint Option for Type2 entry
 *        [eventHandlerPtr] or [eventGroupPtr] must have one equal NULL_PTR
 *      : If serverServicePtr is not NULL,Build config option for SubscribeEventgroupAck/NAck
 *      : If clientServicePtr is not NULL,Build config option for SubscribeEventgroup/StopSubscribeEventgroup
 * Param-Name[in]: sdInstancePtr, eventHandlerPtr, eventGroupPtr, cfgOptLen
 * Param-Name[out]: None
 * Param-Name[in/out]: optAddrPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_BuildType2ConfigurationOption(
    const Sd_InstanceType* sdInstancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_ClientServiceType* clientServicePtr,
    uint8* optAddrPtr,
    uint16 cfgOptLen)
{
    /* !Note: Don't check buffer enough or not,make sure optAddr point to a
     * buffer enough to store Configuration Option */
    const Sd_CapabilityRecordType* recordPtr;
    const char* hostnameStr = "hostname";
    uint8* optAddr = optAddrPtr;
    uint16 recordNum;
    uint16 index;
    uint16 optLen;
    uint8 keyLen, valueLen;

    if (NULL_PTR != eventHandlerPtr)
    {
        recordPtr = serverServicePtr->SdServerCapabilityRecord;
        recordNum = serverServicePtr->SdServerCapabilityRecordNum;
    }
    else
    {
        recordPtr = clientServicePtr->SdClientCapabilityRecord;
        recordNum = clientServicePtr->SdClientCapabilityRecordNum;
    }

    optLen = Sd_HtoNs(cfgOptLen - 3u); /* Exclude Length(2Byte) and Type(1Byte) */
    (void)ILib_memcpy(optAddr, (uint8*)&optLen, sizeof(uint16));
    optAddr[2] = SD_CONFIGURATION_OPTION;
    optAddr[3] = 0x00; /* Reserved field */

    optAddr = &optAddr[4]; /* To the palce used to save first key-value pair */
    if (NULL_PTR != sdInstancePtr->SdInstanceHostname)
    {
        keyLen = 8u; /* Length of "hostname" */
        valueLen = (uint8)strlen(sdInstancePtr->SdInstanceHostname);
        *optAddr = keyLen + valueLen + 1u; /* '=' length is 1 */
        optAddr++;
        (void)ILib_memcpy(optAddr, (const uint8*)hostnameStr, keyLen);
        optAddr = &optAddr[keyLen];
        *optAddr = (uint8)'=';
        optAddr++;
        (void)ILib_memcpy(optAddr, (const uint8*)sdInstancePtr->SdInstanceHostname, valueLen);
        optAddr = &optAddr[valueLen];
    }

    if (NULL_PTR != recordPtr)
    {
        for (index = 0u; index < recordNum; index++)
        {
            keyLen = (uint8)strlen(recordPtr->SdCapabilityRecordKey);
            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                valueLen = (uint8)strlen(recordPtr->SdCapabilityRecordValue);
            }
            else
            {
                valueLen = 0u;
            }
            *optAddr = keyLen + valueLen + 1u; /* '=' length is 1 */
            optAddr++;
            (void)ILib_memcpy(optAddr, (const uint8*)recordPtr->SdCapabilityRecordKey, keyLen);
            optAddr = &optAddr[keyLen];
            *optAddr = (uint8)'=';
            optAddr++;

            if (NULL_PTR != recordPtr->SdCapabilityRecordValue)
            {
                (void)ILib_memcpy(optAddr, (const uint8*)recordPtr->SdCapabilityRecordValue, valueLen);
                optAddr = &optAddr[valueLen];
            }

            recordPtr++;
        }
    }

    *optAddr = 0u; /* end with [0] */
}
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0) || (SD_EVENT_HANDLER_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Set SOME/IP-SD Header
 * Param-Name[in]: sessionIdCntPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: headerPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_SetHeader(
    uint8* headerPtr,
    uint16 instanceId,
    const TcpIp_SockAddrType* remoteAddr,
    boolean isMulticastTx)
{
    Sd_InstanceRTType* instanceRtPtr = &Sd_InstanceRTData[instanceId];
    Sd_SessionIdCtrlType* sesIdCtrlPtr;
    uint16* sessionIdCntPtr = NULL_PTR;
    Sd_HeaderType header;

    (void)ILib_memset(&header, 0x00, sizeof(Sd_HeaderType));

    header.ClientID = 0x00;
    /* Set SessionId*/
    if (TRUE == isMulticastTx)
    {
        sessionIdCntPtr = &instanceRtPtr->sessionIdHandle.txMultiSesIdCnt;
    }
    else
    {
        sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, remoteAddr);
        if (NULL_PTR != sesIdCtrlPtr)
        {
            sessionIdCntPtr = &sesIdCtrlPtr->txUniSesIdCnt;
        }
    }
    if (NULL_PTR != sessionIdCntPtr)
    {
        (*sessionIdCntPtr)++;
        if (0u == *sessionIdCntPtr)
        {
            (*sessionIdCntPtr)++;
            /*@req <SWS_SD_00151> */
            instanceRtPtr->rebootFlag = FALSE;
        }
        header.SessionID = Sd_HtoNs(*sessionIdCntPtr);
    }
    else
    {
        header.SessionID = Sd_HtoNs(0x01);
    }
    header.RebootFlag = instanceRtPtr->rebootFlag ? 1u : 0u;

    header.ProtocolVersion = 0x01;
    header.InterfaceVersion = 0x01;
    header.MessageType = 0x02;
    header.ReturnCode = 0x00;
    /*@req <SWS_SD_00153> */
    header.UnicastFlag = 0x01;

    (void)ILib_memcpy(headerPtr, (uint8*)&header, sizeof(Sd_HeaderType));
}

/******************************************************************************/
/*
 * Brief: Set the IPv4 Endpoint Option
 * Param-Name[in]: ipv4Address, L4Proto, portNumber
 * Param-Name[out]: None
 * Param-Name[in/out]: ipv4OptionPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_SetIPv4EndpointOption(uint8* ipv4OptionPtr, const uint8* ipv4Address, uint8 L4Proto, uint16 portNumber)
{
    Sd_IPv4OptionsType ipv4Option;

    (void)ILib_memset(&ipv4Option, 0x00, sizeof(Sd_IPv4OptionsType));
    ipv4Option.Length = Sd_HtoNs(SD_IPV4_ENDPOINT_OPTION_LENGTH);
    ipv4Option.Type = SD_IPV4_ENDPOINT_OPTION;
    (void)ILib_memcpy(ipv4Option.IPv4Address, ipv4Address, 4u);
    ipv4Option.L4Proto = L4Proto;
    ipv4Option.PortNumber = Sd_HtoNs(portNumber);

    (void)ILib_memcpy(ipv4OptionPtr, (uint8*)&ipv4Option, sizeof(Sd_IPv4OptionsType));
}

/******************************************************************************/
/*
 * Brief: Set the IPv6 Endpoint Option
 * Param-Name[in]: ipv6Address, L4Proto, portNumber
 * Param-Name[out]: None
 * Param-Name[in/out]: ipv6Option
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (STD_ON == SD_IPV6_ENABLE)
static void Sd_SetIPv6EndpointOption(uint8* ipv6OptionPtr, const uint8* ipv6Address, uint8 L4Proto, uint16 portNumber)
{
    Sd_IPv6OptionsType ipv6Option;

    (void)ILib_memset(&ipv6Option, 0x00, sizeof(Sd_IPv6OptionsType));
    ipv6Option.Length = Sd_HtoNs(SD_IPV6_ENDPOINT_OPTION_LENGTH);
    ipv6Option.Type = SD_IPV6_ENDPOINT_OPTION;
    (void)ILib_memcpy(ipv6Option.IPv6Address, ipv6Address, 16u);
    ipv6Option.L4Proto = L4Proto;
    ipv6Option.PortNumber = Sd_HtoNs(portNumber);

    (void)ILib_memcpy(ipv6OptionPtr, (uint8*)&ipv6Option, sizeof(Sd_IPv6OptionsType));
}
#endif /* STD_ON == SD_IPV6_ENABLE */

#if (SD_EVENT_HANDLER_NUM > 0)
/******************************************************************************/
/*
 * Brief: Set the IPv4 Multicast Option
 * Param-Name[in]: ipv4Address, portNumber
 * Param-Name[out]: None
 * Param-Name[in/out]: ipv4OptionPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_SetIPv4MulticastOption(uint8* ipv4OptionPtr, const uint8* ipv4Address, uint16 portNumber)
{
    Sd_IPv4OptionsType ipv4Option;

    (void)ILib_memset(&ipv4Option, 0x00, sizeof(Sd_IPv4OptionsType));
    ipv4Option.Length = Sd_HtoNs(SD_IPV4_MULTICAST_OPTION_LENGTH);
    ipv4Option.Type = SD_IPV4_MULTICAST_OPTION;
    (void)ILib_memcpy(ipv4Option.IPv4Address, ipv4Address, 4u);
    ipv4Option.L4Proto = SD_L4_PROTO_UDP;
    ipv4Option.PortNumber = Sd_HtoNs(portNumber);

    (void)ILib_memcpy(ipv4OptionPtr, (uint8*)&ipv4Option, sizeof(Sd_IPv4OptionsType));
}

/******************************************************************************/
/*
 * Brief: Set the IPv6 Multicast Option
 * Param-Name[in]: ipv6Address, portNumber
 * Param-Name[out]: None
 * Param-Name[in/out]: ipv6OptionPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (STD_ON == SD_IPV6_ENABLE)
static void Sd_SetIPv6MulticastOption(uint8* ipv6OptionPtr, const uint8* ipv6Address, uint16 portNumber)
{
    Sd_IPv6OptionsType ipv6Option;

    (void)ILib_memset(&ipv6Option, 0x00, sizeof(Sd_IPv6OptionsType));
    ipv6Option.Length = Sd_HtoNs(SD_IPV6_MULTICAST_OPTION_LENGTH);
    ipv6Option.Type = SD_IPV6_MULTICAST_OPTION;
    (void)ILib_memcpy(ipv6Option.IPv6Address, ipv6Address, 16u);
    ipv6Option.L4Proto = SD_L4_PROTO_UDP;
    ipv6Option.PortNumber = Sd_HtoNs(portNumber);

    (void)ILib_memcpy(ipv6OptionPtr, (uint8*)&ipv6Option, sizeof(Sd_IPv6OptionsType));
}
#endif /* STD_ON == SD_IPV6_ENABLE */
#endif /* SD_EVENT_HANDLER_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Set TTL to the buffer [ttlPtr] referenced
 * Param-Name[in]: ttl
 * Param-Name[out]: None
 * Param-Name[in/out]: ttlPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
SD_LOCAL_INLINE void Sd_SetTTL(Sd_TTLType* ttlPtr, uint32 ttl)
{
    if (ttl == 0xFFFFFFu)
    {
        ttlPtr->isValidForever = TRUE;
        ttlPtr->ttlms = 0u;
    }
    else
    {
        ttlPtr->ttlms = (uint64)(((uint64)ttl * 1000u) / SD_MAIN_FUNCTION_CYCLE_TIME);
        ttlPtr->isValidForever = FALSE;
    }
}
#endif /* SD_SERVER_SERVICE_NUM > 0 || SD_CLIENT_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Process server service state transition in Down Phase
 * Param-Name[in]: instancePtr, serverServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_MainFunction
 */
/******************************************************************************/
#if (SD_SERVER_SERVICE_NUM > 0)
static void Sd_ServerServiceDownPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr)
{
    const Sd_ServerTimerType* timerPtr;
    const SoAd_SoConIdType* soConIdList;
    const Sd_EventHandlerType* eventHandlerPtr;
    const Sd_SoAdSoConGroupType* soAdSoConGroupPtr;
    const Sd_EventHandlerMulticastType* eventHandlerMulticastPtr;
    Sd_ServerServiceRTType* sRtDataPtr;
    uint32 tRandom, delayMin, delayMax;
    uint16 soConIdNum;
    uint16 index;
    uint16 eventGroupIdx;
    TcpIp_IpAddrStateType ipAddrState;

    /* Phase manage */
    /*@req <SWS_SD_00317> */
    sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    ipAddrState = Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState;
    if ((SD_INIT_SUCCESS == Sd_InitResult) && (SD_SERVER_SERVICE_AVAILABLE == sRtDataPtr->setState)
        && (TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState))
    {
        /*@req <SWS_SD_00317> */
        sRtDataPtr->phase = SD_SERVER_PHASE_INITIAL_WAIT;

        /*@req <SWS_SD_00330> */
        if (NULL_PTR != serverServicePtr->SdProvidedMethods)
        {
            (void)SoAd_EnableRouting(
                serverServicePtr->SdProvidedMethods->SdServerServiceActivationRef->SdRoutingGroupId);
        }

        /* make the number between SdServerTimerInitialOfferDelayMin to SdServerTimerInitialOfferDelayMax*/
        timerPtr = serverServicePtr->SdServerServiceTimerRef;
        delayMin = timerPtr->SdServerTimerInitialOfferDelayMin;
        delayMax = timerPtr->SdServerTimerInitialOfferDelayMax;
        /* Calculate random delay time */
        tRandom = Sd_GetRandomValue(delayMin, delayMax);
        /*@req <SWS_SD_00318> */
        /* When entering the Initial Wait Phase, a random timer shall be started */
        sRtDataPtr->initialWaitTimer = Sd_SendTimeAdjust(tRandom / SD_MAIN_FUNCTION_CYCLE_TIME);

        /*@req <SWS_SD_00321> */
        /* When the calculated random timer based on the min and max values
         * SdServerTimerInitialOfferDelayMin and SdServerTimerInitialOfferDelayMax
         * expires, the first OfferService entry shall be sent out.*/
        Sd_BuildOfferServiceEntry(instancePtr, serverServicePtr, NULL_PTR, sRtDataPtr->initialWaitTimer, FALSE, FALSE);

        /*@req <SWS_SD_00606> */
        /* When the Initial Wait Phase is entered, the API SoAd_OpenSoCon() shall be called
         * for all Socket Connections associated with this Server Service Instance. */
        soAdSoConGroupPtr = serverServicePtr->SdServerServiceTcpRef;
        if (NULL_PTR != soAdSoConGroupPtr)
        {
            soConIdNum = soAdSoConGroupPtr->SdSoConNumInGroup;
            soConIdList = soAdSoConGroupPtr->SdSocketIdList;
            for (index = 0u; index < soConIdNum; index++)
            {
                (void)SoAd_OpenSoCon(soConIdList[index]);
            }
        }
        soAdSoConGroupPtr = serverServicePtr->SdServerServiceUdpRef;
        if (NULL_PTR != soAdSoConGroupPtr)
        {
            soConIdNum = soAdSoConGroupPtr->SdSoConNumInGroup;
            soConIdList = soAdSoConGroupPtr->SdSocketIdList;
            for (index = 0u; index < soConIdNum; index++)
            {
                (void)SoAd_OpenSoCon(soConIdList[index]);
            }
        }

        eventHandlerPtr = serverServicePtr->SdEventHandler;
        for (eventGroupIdx = 0u; eventGroupIdx < serverServicePtr->SdEventHandlerNum; eventGroupIdx++)
        {
            eventHandlerMulticastPtr = eventHandlerPtr->SdEventHandlerMulticast;
            if (NULL_PTR != eventHandlerMulticastPtr)
            {
                (void)SoAd_OpenSoCon(eventHandlerMulticastPtr->SdMulticastEventSoConRef->SdSoConId);
            }
            eventHandlerPtr++;
        }
    }
}

/******************************************************************************/
/*
 * Brief: Process server service state transition in Initial With Phase
 * Param-Name[in]: instancePtr, serverServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_MainFunction
 */
/******************************************************************************/
static void Sd_ServerServiceInitialWaitPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr)
{
    const Sd_ServerTimerType* timerPtr;
    Sd_ServerServiceRTType* sRtDataPtr;

    sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    /*@req <SWS_SD_00325>,<SWS_SD_00323> */
    if ((TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState)
        || (SD_SERVER_SERVICE_AVAILABLE != sRtDataPtr->setState))
    {
        Sd_ServerServiceDownPhaseEnter(instancePtr, serverServicePtr);
    }
    else
    {
        if (sRtDataPtr->initialWaitTimer > 0u)
        {
            sRtDataPtr->initialWaitTimer--;
            /* Initial Wait Phase Delay timer expire */
            if (0u == sRtDataPtr->initialWaitTimer)
            {
                timerPtr = serverServicePtr->SdServerServiceTimerRef;
                if (timerPtr->SdServerTimerInitialOfferRepetitionsMax > 0u)
                {
                    /*@req <SWS_SD_00434> */
                    /* When the calculated random timer expires and the parameter
                     * SdServerTimerInitialOfferRepetitionsMax does not equals 0,
                     * the Repetition Phase shall be entered.*/
                    sRtDataPtr->phase = SD_SERVER_PHASE_REPETITION;

                    /*@req <SWS_SD_00329> */
                    /* If the Repetition Phase is entered, the Service Discovery shall
                     * wait SdServerTimerInitialOfferRepetitionBaseDelay and send an
                     * OfferService Entry.*/
                    sRtDataPtr->repetitionTimer =
                        timerPtr->SdServerTimerInitialOfferRepetitionBaseDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                    Sd_BuildOfferServiceEntry(
                        instancePtr,
                        serverServicePtr,
                        NULL_PTR,
                        sRtDataPtr->repetitionTimer,
                        FALSE,
                        FALSE);
                    sRtDataPtr->repetitionCount = 0u;
                }
                else
                {
                    /*@req <SWS_SD_00435> */
                    /* When the calculated random timer expires and the parameter
                     * SdServerTimerInitialOfferRepetitionsMax equal 0, the
                     * Main Phase shall be entered.*/
                    sRtDataPtr->phase = SD_SERVER_PHASE_MAIN;

                    /*@req <SWS_SD_00449> */
                    /* If SdServerTimerOfferCyclicDelay is greater than 0, in the Main Phase
                     * an OfferService entry shall be sent cyclically with an interval defined
                     * by configuration item SdServerTimerOfferCyclicDelay.*/
                    if (timerPtr->SdServerTimerOfferCyclicDelay > 0u)
                    {
                        sRtDataPtr->mainTimer = timerPtr->SdServerTimerOfferCyclicDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                        Sd_BuildOfferServiceEntry(
                            instancePtr,
                            serverServicePtr,
                            NULL_PTR,
                            sRtDataPtr->mainTimer,
                            FALSE,
                            FALSE);
                    }
                }
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief: Process server service state transition in Repetition Phase
 * Param-Name[in]: instancePtr, serverServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_MainFunction
 */
/******************************************************************************/
static void Sd_ServerServiceRepetitionPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr)
{
    const Sd_ServerTimerType* timerPtr;
    Sd_ServerServiceRTType* sRtDataPtr;
    uint32 tResult;

    sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    /*@req <SWS_SD_00340>,<SWS_SD_00338> */
    if ((TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState)
        || (SD_SERVER_SERVICE_AVAILABLE != sRtDataPtr->setState))
    {
        Sd_ServerServiceDownPhaseEnter(instancePtr, serverServicePtr);
    }
    else
    {
        if (sRtDataPtr->repetitionTimer > 0u)
        {
            sRtDataPtr->repetitionTimer--;
            if (0u == sRtDataPtr->repetitionTimer)
            {
                sRtDataPtr->repetitionCount++;

                timerPtr = serverServicePtr->SdServerServiceTimerRef;
                if (sRtDataPtr->repetitionCount == timerPtr->SdServerTimerInitialOfferRepetitionsMax)
                {
                    /*@req <SWS_SD_00336> */
                    /* After the amount of cyclically sent OfferServices within the
                     * Repetition Phase equals the amount of SdServerTimerInitialOfferRepetitionsMax,
                     * the Main Phase shall be entered.*/
                    sRtDataPtr->phase = SD_SERVER_PHASE_MAIN;

                    /*@req <SWS_SD_00449> */
                    /* If SdServerTimerOfferCyclicDelay is greater than 0, in the Main Phase
                     * an OfferService entry shall be sent cyclically with an interval defined
                     * by configuration item SdServerTimerOfferCyclicDelay.*/
                    if (timerPtr->SdServerTimerOfferCyclicDelay > 0u)
                    {
                        sRtDataPtr->mainTimer = timerPtr->SdServerTimerOfferCyclicDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                        Sd_BuildOfferServiceEntry(
                            instancePtr,
                            serverServicePtr,
                            NULL_PTR,
                            sRtDataPtr->mainTimer,
                            FALSE,
                            FALSE);
                    }
                }
                else
                {
                    /*@req <SWS_SD_00331> */
                    /* In the Repetition Phase up to SdServerTimerInitialOfferRepetitionsMax
                     * OfferService Entries shall be sent with doubling intervals*/
                    tResult = 0x01UL << (uint32)sRtDataPtr->repetitionCount;
                    tResult = timerPtr->SdServerTimerInitialOfferRepetitionBaseDelay * tResult;
                    sRtDataPtr->repetitionTimer = tResult / SD_MAIN_FUNCTION_CYCLE_TIME;
                    Sd_BuildOfferServiceEntry(
                        instancePtr,
                        serverServicePtr,
                        NULL_PTR,
                        sRtDataPtr->repetitionTimer,
                        FALSE,
                        FALSE);
                }
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief: Process server service state transition in Main Phase
 * Param-Name[in]: instancePtr, serverServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_MainFunction
 */
/******************************************************************************/
static void Sd_ServerServiceMainPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr)
{
    const Sd_ServerTimerType* timerPtr;
    Sd_ServerServiceRTType* sRtDataPtr;
    TcpIp_IpAddrStateType ipAddrState;

    sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    ipAddrState = Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState;
    /*@req <SWS_SD_00342> */
    if ((SD_SERVER_SERVICE_AVAILABLE == sRtDataPtr->setState) && (TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState))
    {
        if (sRtDataPtr->mainTimer > 0u)
        {
            sRtDataPtr->mainTimer--;
            if (0u == sRtDataPtr->mainTimer)
            {
                /*@req <SWS_SD_00449> */
                timerPtr = serverServicePtr->SdServerServiceTimerRef;
                sRtDataPtr->mainTimer = timerPtr->SdServerTimerOfferCyclicDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                Sd_BuildOfferServiceEntry(instancePtr, serverServicePtr, NULL_PTR, sRtDataPtr->mainTimer, FALSE, FALSE);
            }
        }
    }
    else
    {
        /*@req <SWS_SD_00347>,<SWS_SD_00348> */
        Sd_ServerServiceDownPhaseEnter(instancePtr, serverServicePtr);
    }
}

/******************************************************************************/
/*
 * Brief: Process server service Down Phase enter(other than init)
 * Param-Name[in]: serverServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI:
 */
/******************************************************************************/
static void Sd_ServerServiceDownPhaseEnter(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr)
{
    const SoAd_SoConIdType* soConIdList;
    const Sd_EventHandlerType* eventHandlerPtr;
    const Sd_EventHandlerMulticastType* eventHandlerMulticastPtr;
    const Sd_ProvidedMethodsType* provideMethodPtr;
    const Sd_SoAdSoConGroupType* soadSoConGroupPtr;
    Sd_EventHandlerRTType* ehRTDataPtr;
    Sd_ServerServiceRTType* sRtDataPtr;
    TcpIp_IpAddrStateType ipAddrState;
    uint16 soConIdNum;
    uint16 index;
    uint16 serverHandleId;
    uint16 handlerIdx;
    uint16 eventHandlerId;

    serverHandleId = serverServicePtr->SdServerServiceHandleId;
    sRtDataPtr = &Sd_ServerRTData[serverHandleId];
    ipAddrState = Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState;

    /*@req <SWS_SD_00323>,<SWS_SD_00338>,<SWS_SD_00348> */
    /* If the API Server Sd_ServerServiceSetState() is called with state SD_SERVER_SERVICE_DOWN
     * while the IP address is still assigned*/
    if (SD_SERVER_SERVICE_DOWN == sRtDataPtr->setState)
    {
        if ((TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState)
            && (SD_SERVER_PHASE_INITIAL_WAIT != sRtDataPtr->phase)) /* after first OfferService send,in Repetion Phase*/
        {
            /* Clear send queue of this service,avoid send an OfferService after send a StopOfferService */
            Sd_ClearSendQueue(&sRtDataPtr->head);
            /* Send a StopOfferService */
            Sd_BuildOfferServiceEntry(instancePtr, serverServicePtr, NULL_PTR, 0, FALSE, TRUE);
        }

#if (SD_EVENT_HANDLER_NUM > 0)
        /* all subscriptions of the eventgroup(s) of this service instance shall be
         * deleted and SD_EVENT_HANDLER_RELEASED and reported to BswM using the API
         * BswM_Sd_EventHandlerCurrentState */
        eventHandlerPtr = serverServicePtr->SdEventHandler;
        for (handlerIdx = 0u; handlerIdx < serverServicePtr->SdEventHandlerNum; handlerIdx++)
        {
            eventHandlerId = eventHandlerPtr->SdEventHandlerHandleId;
            ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerId];

            /* Delete all client from subscribe list */
            Sd_RemoveAllClientFromSubscribeList(&ehRTDataPtr->tcpSubscribeList);
            Sd_RemoveAllClientFromSubscribeList(&ehRTDataPtr->udpSubscribeList);
            ehRTDataPtr->tcpSubsClientNum = 0u;
            ehRTDataPtr->udpSubsClientNum = 0u;

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
    for (handlerIdx = 0u; handlerIdx < serverServicePtr->SdEventHandlerNum; handlerIdx++)
    {
        eventHandlerId = eventHandlerPtr->SdEventHandlerHandleId;
        if (Sd_EventHandlerRTData[eventHandlerId].curState != SD_EVENT_HANDLER_RELEASED)
        {
#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_EventHandlerCurrentState(eventHandlerId, SD_EVENT_HANDLER_RELEASED);
#endif /* SD_BSWM_USED == STD_ON */
            Sd_EventHandlerRTData[eventHandlerId].curState = SD_EVENT_HANDLER_RELEASED;
        }

        /* Close Multicaset socket connection */
        eventHandlerMulticastPtr = eventHandlerPtr->SdEventHandlerMulticast;
        if (NULL_PTR != eventHandlerMulticastPtr)
        {
            (void)SoAd_OpenSoCon(eventHandlerMulticastPtr->SdMulticastEventSoConRef->SdSoConId);
        }

        eventHandlerPtr++;
    }
#endif /* SD_EVENT_HANDLER_NUM > 0 */

    /*@req <SWS_SD_00341>,<SWS_SD_00349> */
    /* When enter Down Phase from other phase, the routing of this Server Service Instance
     * shall be disabled.*/
    provideMethodPtr = serverServicePtr->SdProvidedMethods;
    if (NULL_PTR != provideMethodPtr)
    {
        (void)SoAd_DisableRouting(provideMethodPtr->SdServerServiceActivationRef->SdRoutingGroupId);
    }

    /* Down Phase entered */
    Sd_ServerRTData[serverHandleId].phase = SD_SERVER_PHASE_DOWN;

    /*@req <SWS_SD_00605> */
    /* When the Down Phase is entered (coming from states other than init), the
     * API SoAd_CloseSoCon() shall be called for all Socket Connections associated
     * with this Server Service Instance.*/
    soadSoConGroupPtr = serverServicePtr->SdServerServiceTcpRef;
    if (NULL_PTR != soadSoConGroupPtr)
    {
        soConIdNum = soadSoConGroupPtr->SdSoConNumInGroup;
        soConIdList = soadSoConGroupPtr->SdSocketIdList;
        for (index = 0u; index < soConIdNum; index++)
        {
            (void)SoAd_CloseSoCon(soConIdList[index], TRUE);
        }
    }
    soadSoConGroupPtr = serverServicePtr->SdServerServiceUdpRef;
    if (NULL_PTR != soadSoConGroupPtr)
    {
        soConIdNum = soadSoConGroupPtr->SdSoConNumInGroup;
        soConIdList = soadSoConGroupPtr->SdSocketIdList;
        for (index = 0u; index < soConIdNum; index++)
        {
            (void)SoAd_CloseSoCon(soConIdList[index], TRUE);
        }
    }

    /* Clear all timer */
    Sd_ServerRTData[serverHandleId].initialWaitTimer = 0u;
    Sd_ServerRTData[serverHandleId].repetitionTimer = 0u;
    Sd_ServerRTData[serverHandleId].repetitionCount = 0u;
    Sd_ServerRTData[serverHandleId].mainTimer = 0u;
}

/******************************************************************************/
/*
 * Brief: Process server service Down Phase enter(other than init)
 * Param-Name[in]: serverServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI:
 */
/******************************************************************************/
static void Sd_ServerServiceInitialWaitEnter(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr)
{
    const Sd_ServerTimerType* timerPtr;
    const SoAd_SoConIdType* soConIdList;
    const Sd_ProvidedMethodsType* provideMethodPtr;
    const Sd_SoAdSoConGroupType* soadSoConGroupPtr;
    Sd_ServerServiceRTType* sRtDataPtr;
    uint32 tRandom, delayMin, delayMax;
    uint16 soConIdNum;
    uint16 index;

    sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];
    /*@req <SWS_SD_00317> */
    sRtDataPtr->phase = SD_SERVER_PHASE_INITIAL_WAIT;

    /*@req <SWS_SD_00330> */
    provideMethodPtr = serverServicePtr->SdProvidedMethods;
    if (NULL_PTR != provideMethodPtr)
    {
        (void)SoAd_EnableRouting(provideMethodPtr->SdServerServiceActivationRef->SdRoutingGroupId);
    }

    /* make the number between SdServerTimerInitialOfferDelayMin to
     * SdServerTimerInitialOfferDelayMax */
    timerPtr = serverServicePtr->SdServerServiceTimerRef;
    delayMin = timerPtr->SdServerTimerInitialOfferDelayMin;
    delayMax = timerPtr->SdServerTimerInitialOfferDelayMax;
    /* Calculate random delay time */
    tRandom = Sd_GetRandomValue(delayMin, delayMax);
    /*@req <SWS_SD_00318> */
    /* When entering the Initial Wait Phase, a random timer shall be started */
    sRtDataPtr->initialWaitTimer = Sd_SendTimeAdjust(tRandom / SD_MAIN_FUNCTION_CYCLE_TIME);

    /*@req <SWS_SD_00321> */
    /* When the calculated random timer based on the min and max values
     * SdServerTimerInitialOfferDelayMin and SdServerTimerInitialOfferDelayMax
     * expires, the first OfferService entry shall be sent out.*/
    Sd_BuildOfferServiceEntry(instancePtr, serverServicePtr, NULL_PTR, sRtDataPtr->initialWaitTimer, FALSE, FALSE);

    /*@req <SWS_SD_00606> */
    /* When the Initial Wait Phase is entered, the API SoAd_OpenSoCon() shall be
     * called for all Socket Connections associated with this Server Service
     * Instance. */
    soadSoConGroupPtr = serverServicePtr->SdServerServiceTcpRef;
    if (NULL_PTR != soadSoConGroupPtr)
    {
        soConIdNum = soadSoConGroupPtr->SdSoConNumInGroup;
        soConIdList = soadSoConGroupPtr->SdSocketIdList;
        for (index = 0u; index < soConIdNum; index++)
        {
            (void)SoAd_OpenSoCon(soConIdList[index]);
        }
    }
    soadSoConGroupPtr = serverServicePtr->SdServerServiceUdpRef;
    if (NULL_PTR != soadSoConGroupPtr)
    {
        soConIdNum = soadSoConGroupPtr->SdSoConNumInGroup;
        soConIdList = soadSoConGroupPtr->SdSocketIdList;
        for (index = 0u; index < soConIdNum; index++)
        {
            (void)SoAd_OpenSoCon(soConIdList[index]);
        }
    }
}

/******************************************************************************/
/*
 * Brief: Manage server service timer
 * Param-Name[in]: serverServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_ServerServiceTimerHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr)
{
    SD_UNUSED(instancePtr);
#if (SD_EVENT_HANDLER_NUM > 0)
    /* Handle subscribed client's TTL, if expired,delete the client from subscribe list */
    Sd_SubscribeClientTTLManage(serverServicePtr);
#endif
}
#endif /* SD_SERVER_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Process client service state transition in Down Phase
 * Param-Name[in]: instancePtr, clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_MainFunction
 */
/******************************************************************************/
#if (SD_CLIENT_SERVICE_NUM > 0)
static void Sd_ClientServiceDownPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    const Sd_ConsumedMethodsType* consumedMethodsPtr;
    Sd_ClientServiceRTType* cRtDataPtr;
    SoAd_RoutingGroupIdType routingGroupId;
    TcpIp_IpAddrStateType ipAddrState;
    Std_ReturnType ret;

    cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    ipAddrState = Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState;
    /*@req <SWS_SD_00350> */
    if ((SD_INIT_SUCCESS == Sd_InitResult) && (SD_CLIENT_SERVICE_REQUESTED == cRtDataPtr->setState)
        && (TCPIP_IPADDR_STATE_ASSIGNED == ipAddrState))
    {
        consumedMethodsPtr = clientServicePtr->SdConsumedMethods;
        /*@req <SWS_SD_00464> */
        /* An OfferService entry was received and its TTL timer did not expire yet */
        if ((TRUE == cRtDataPtr->ttl.isValidForever) || (cRtDataPtr->ttl.ttlms > 0u))
        {
            /* Open TCP connection if SdClientServiceTcpRef is configured and
             * was not opened before */
            if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && (TRUE == cRtDataPtr->tcpSoCon.isOptRcved)
                && (FALSE == cRtDataPtr->tcpSoCon.isSoConOpened))
            {
                ret = SoAd_OpenSoCon(cRtDataPtr->tcpSoCon.soConId);
                if (E_OK == ret)
                {
                    cRtDataPtr->tcpSoCon.isSoConOpened = TRUE;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
                    Sd_ClientServiceResetAllCEgSubsEntrySendFlag(clientServicePtr, TRUE, FALSE);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

                    if (NULL_PTR != consumedMethodsPtr)
                    {
                        routingGroupId = consumedMethodsPtr->SdClientServiceActivationRef->SdRoutingGroupId;
                        (void)SoAd_EnableSpecificRouting(routingGroupId, cRtDataPtr->tcpSoCon.soConId);
                    }
                }
            }

            if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && (TRUE == cRtDataPtr->udpSoCon.isOptRcved)
                && (FALSE == cRtDataPtr->udpSoCon.isSoConOpened))
            {
                ret = SoAd_OpenSoCon(cRtDataPtr->udpSoCon.soConId);
                if (E_OK == ret)
                {
                    cRtDataPtr->udpSoCon.isSoConOpened = TRUE;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
                    Sd_ClientServiceResetAllCEgSubsEntrySendFlag(clientServicePtr, FALSE, TRUE);
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

                    if (NULL_PTR != consumedMethodsPtr)
                    {
                        routingGroupId = consumedMethodsPtr->SdClientServiceActivationRef->SdRoutingGroupId;
                        (void)SoAd_EnableSpecificRouting(routingGroupId, cRtDataPtr->udpSoCon.soConId);
                    }
                }
            }

            /* The Main Phase shall be entered */
            cRtDataPtr->phase = SD_CLIENT_PHASE_MAIN;
        }
        else
        {
            /* To InitialWait Phase*/
            Sd_ClientServiceInitialWaitEnter(clientServicePtr, cRtDataPtr);
        }
    }
}

/******************************************************************************/
/*
 * Brief: Process client service state transition in Initial With Phase
 * Param-Name[in]: instancePtr, clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_MainFunction
 */
/******************************************************************************/
static void Sd_ClientServiceInitialWaitPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    const Sd_ClientTimerType* clientTimerPtr;
    Sd_ClientServiceRTType* cRtDataPtr;
    uint32 initRepetBaseDelay;

    cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    /*@req <SWS_SD_00355>,<SWS_SD_00357> */
    if ((SD_CLIENT_SERVICE_RELEASED == cRtDataPtr->setState)
        || (TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState))
    {
        Sd_ClientServiceDownPhaseEnter(instancePtr, clientServicePtr, cRtDataPtr);
        Sd_ClientInitialWaitPhaseLeftHandle(cRtDataPtr);
    }
    else
    {
        if (cRtDataPtr->initialWaitTimer > 0u)
        {
            cRtDataPtr->initialWaitTimer--;
            /*@req <SWS_SD_00353> */
            if (0u == cRtDataPtr->initialWaitTimer)
            {
                /* FindService Entry shall be sent */
                (void)Sd_BuildFindServiceEntry(instancePtr, clientServicePtr);

                /* If the SdClientTimerInitialFindRepetitionsMax>0, enter the Repetition Phase*/
                clientTimerPtr = clientServicePtr->SdClientServiceTimerRef;
                if (clientTimerPtr->SdClientTimerInitialFindRepetitionsMax > 0u)
                {
                    cRtDataPtr->phase = SD_CLIENT_PHASE_REPETITION;
                    /*@req <SWS_SD_00358> */
                    initRepetBaseDelay = clientTimerPtr->SdClientTimerInitialFindRepetitionsBaseDelay;
                    cRtDataPtr->repetitionTimer = initRepetBaseDelay / SD_MAIN_FUNCTION_CYCLE_TIME;
                    cRtDataPtr->repetitionCount = 0u;
                }
                else
                {
                    /* If the SdClientTimerInitialFindRepetitionsMax=0, enter the Main Phase*/
                    cRtDataPtr->phase = SD_CLIENT_PHASE_MAIN;
                }
                Sd_ClientInitialWaitPhaseLeftHandle(cRtDataPtr);
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief: Process client service state transition in Repetition Phase
 * Param-Name[in]: instancePtr, clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_MainFunction
 */
/******************************************************************************/
static void Sd_ClientServiceRepetitionPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    Sd_ClientServiceRTType* cRtDataPtr;
    const Sd_ClientTimerType* clientTimerPtr;
    uint32 delayTime;

    cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    /*@req <SWS_SD_00371>,<SWS_SD_00373> */
    if ((SD_CLIENT_SERVICE_RELEASED == cRtDataPtr->setState)
        || (TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState))
    {
        Sd_ClientServiceDownPhaseEnter(instancePtr, clientServicePtr, cRtDataPtr);
    }
    else
    {
        if (cRtDataPtr->repetitionTimer > 0u)
        {
            cRtDataPtr->repetitionTimer--;
            /*@req <SWS_SD_00457> */
            if (0u == cRtDataPtr->repetitionTimer)
            {
                /* When the timer SdClientTimerInitialFindRepetitionsBaseDelay expires
                 * within the Repetition Phase, a FindOffer Message shall be sent.*/
                (void)Sd_BuildFindServiceEntry(instancePtr, clientServicePtr);

                cRtDataPtr->repetitionCount++;
                clientTimerPtr = clientServicePtr->SdClientServiceTimerRef;
                if (cRtDataPtr->repetitionCount < clientTimerPtr->SdClientTimerInitialFindRepetitionsMax)
                {
                    /*@req <SWS_SD_00363> */
                    delayTime = 0x01u << cRtDataPtr->repetitionCount;
                    delayTime = clientTimerPtr->SdClientTimerInitialFindRepetitionsBaseDelay * delayTime;
                    cRtDataPtr->repetitionTimer = delayTime / SD_MAIN_FUNCTION_CYCLE_TIME;
                }
                else if (cRtDataPtr->repetitionCount == clientTimerPtr->SdClientTimerInitialFindRepetitionsMax)
                {
                    /*@req <SWS_SD_00369> */
                    /* After sending the maximum repetitions (defined by SdClientTimerInitialFindRepetitionsMax)
                     * of FindService entries,the Repetition Phase shall be left and the Main Phase shall be entered. */
                    cRtDataPtr->phase = SD_CLIENT_PHASE_MAIN;
                }
                else
                {
                    /*Nothing to do here*/
                }
            }
        }
    }
}

/******************************************************************************/
/*
 * Brief: Process client service state transition in Main Phase
 * Param-Name[in]: instancePtr, clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_MainFunction
 */
/******************************************************************************/
static void Sd_ClientServiceMainPhaseHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr)
{
    Sd_ClientServiceRTType* cRtDataPtr;
    const Sd_ConsumedMethodsType* consumedMethodsPtr;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    const Sd_ConsumedEventGroupType* cEgPtr;
    const Sd_ConsumedEventGroupType* eventGroupPtr;
    Sd_ConsumedEventGroupRTType* consumedEgRTPtr;
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
    const Sd_ClientTimerType* consumedEgTimerPtr;
#endif
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
    SoAd_SoConModeType tcpSoConMode;
    SoAd_SoConModeType udpSoConMode;
    SoAd_RoutingGroupIdType routingGroupId;
    uint16 index;
    boolean isRxUseMulticast;
    uint32 sendTime = 0u;

    cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    /*@req <SWS_SD_00381>,<SWS_SD_00380> */
    if ((SD_CLIENT_SERVICE_RELEASED == cRtDataPtr->setState)
        || (TCPIP_IPADDR_STATE_ASSIGNED != Sd_InstanceRTData[instancePtr->SdInstanceHandleId].ipAddrState))
    {
        Sd_ClientServiceDownPhaseEnter(instancePtr, clientServicePtr, cRtDataPtr);
    }
    else
    {
        /* @req SWS_SD_00721 */
        /* An OfferService entry was received and its TTL timer did not expire yet */
        if ((TRUE == cRtDataPtr->ttl.isValidForever) || (cRtDataPtr->ttl.ttlms > 0u))
        {
            tcpSoConMode = SOAD_SOCON_OFFLINE;
            udpSoConMode = SOAD_SOCON_OFFLINE;
            if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && (TRUE == cRtDataPtr->rxTcpEndpointFlag))
            {
                SoAd_GetSoConMode(cRtDataPtr->tcpSoCon.soConId, &tcpSoConMode);
            }
            if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && (TRUE == cRtDataPtr->rxUdpEndpointFlag))
            {
                SoAd_GetSoConMode(cRtDataPtr->udpSoCon.soConId, &udpSoConMode);
            }

            /* Socket Connections are in state SOAD_SOCON_ONLINE */
            if ((SOAD_SOCON_ONLINE == tcpSoConMode) || (SOAD_SOCON_ONLINE == udpSoConMode))
            {
                if (TRUE == cRtDataPtr->offerServiceRxFlag)
                {
                    /* If the client service has not been reported as SD_CLIENT_SERVICE_AVAILABLE*/
                    if (SD_CLIENT_SERVICE_AVAILABLE != cRtDataPtr->curState)
                    {
                        /* Enable specific routing */
                        consumedMethodsPtr = clientServicePtr->SdConsumedMethods;
                        if (NULL_PTR != consumedMethodsPtr)
                        {
                            routingGroupId = consumedMethodsPtr->SdClientServiceActivationRef->SdRoutingGroupId;
                            if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef)
                                && (TRUE == cRtDataPtr->rxTcpEndpointFlag))
                            {
                                (void)SoAd_EnableSpecificRouting(routingGroupId, cRtDataPtr->tcpSoCon.soConId);
                            }

                            if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef)
                                && (TRUE == cRtDataPtr->rxUdpEndpointFlag))
                            {
                                (void)SoAd_EnableSpecificRouting(routingGroupId, cRtDataPtr->udpSoCon.soConId);
                            }
                        }

/* Report SD_CLIENT_SERVICE_AVAILABLE to the BswM */
#if (SD_BSWM_USED == STD_ON)
                        BswM_Sd_ClientServiceCurrentState(
                            clientServicePtr->SdClientServiceHandleId,
                            SD_CLIENT_SERVICE_AVAILABLE);
#endif
                        cRtDataPtr->curState = SD_CLIENT_SERVICE_AVAILABLE;
                    }
                }

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
                if (cRtDataPtr->offerEntryRxPduId == instancePtr->SdInstanceMulticastRxPdu)
                {
                    isRxUseMulticast = TRUE;
                }
                else
                {
                    isRxUseMulticast = FALSE;
                }

                /* A SubscribeEventgroup entry shall be sent out for each currently requested
                 * Consumed Eventgroup of this Client Service Instance */
                eventGroupPtr = clientServicePtr->SdConsumedEventGroup;
                for (index = 0u; index < clientServicePtr->SdConsumedEventGroupNum; index++)
                {
                    consumedEgRTPtr = &Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
                    consumedEgTimerPtr = eventGroupPtr->SdConsumedEventGroupTimerRef;
#endif
                    if ((SD_CONSUMED_EVENTGROUP_REQUESTED == consumedEgRTPtr->setState)
                            && (((NULL_PTR != eventGroupPtr->SdConsumedEventGroupTcpActivationRef)
                                 && (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened)
                                 && (FALSE == consumedEgRTPtr->isTcpSubsSend))
                                || ((NULL_PTR != eventGroupPtr->SdConsumedEventGroupUdpActivationRef)
                                    && (TRUE == cRtDataPtr->udpSoCon.isSoConOpened)
                                    && (FALSE == consumedEgRTPtr->isUdpSubsSend)))
                            && ((TRUE == cRtDataPtr->offerServiceRxFlag)
                                || (FALSE == consumedEgRTPtr->isSendSubEvtAfterRequest))
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
                        || ((consumedEgTimerPtr->SdSubscribeEventgroupRetryMax > 0u)
                            && (consumedEgRTPtr->subsRetryCnt <= consumedEgTimerPtr->SdSubscribeEventgroupRetryMax)
                            && (0u == consumedEgRTPtr->subsRetryTimer) && (TRUE == consumedEgRTPtr->subsRetryEnable))
#endif
                    )
                    {
                        /*@req <SWS_SD_00721>,<SWS_SD_00695> */
                        /* StopSubscribeEventgroup entry shall be sent out, if the last SubscribeEventgroup entry was
                         * sent as reaction to an OfferService entry received via Multicast, it was never answered with
                         * a SubscribeEventgroupAck, and the current OfferService entry was received via Multicast.*/
                        if ((SD_CLIENT_PHASE_MAIN == cRtDataPtr->phase) && (FALSE == consumedEgRTPtr->isAnsweredWithAck)
                            && (TRUE == isRxUseMulticast))
                        {
                            (void)Sd_BuildStopSubscribeEventgroupEntry(
                                instancePtr,
                                clientServicePtr,
                                eventGroupPtr,
                                &cRtDataPtr->offerEntryRemoteAddr,
                                cRtDataPtr,
                                isRxUseMulticast);
                        }

                        (void)Sd_BuildSubscribeEventgroupEntry(
                            instancePtr,
                            clientServicePtr,
                            eventGroupPtr,
                            &cRtDataPtr->offerEntryRemoteAddr,
                            cRtDataPtr,
                            isRxUseMulticast,
                            &sendTime);
                        /*@req <SWS_SD_00376> */
                        if (TRUE == isRxUseMulticast)
                        {
                            consumedEgRTPtr->isAnsweredWithAck = FALSE;
                        }
                        else
                        {
                            consumedEgRTPtr->isAnsweredWithAck = TRUE;
                        }

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
                        if (consumedEgTimerPtr->SdSubscribeEventgroupRetryMax > 0u)
                        {
                            consumedEgRTPtr->subsRetryTimer =
                                sendTime
                                + (consumedEgTimerPtr->SdSubscribeEventgroupRetryDelay / SD_MAIN_FUNCTION_CYCLE_TIME);
                            consumedEgRTPtr->subsRetryEnable = TRUE;
                        }
#endif

                        /* backup remote address for requirement <SWS_SD_00381> to send StopSubscribeEventgroup */
                        (void)ILib_memcpy(
                            &consumedEgRTPtr->remoteAddr,
                            &cRtDataPtr->offerEntryRemoteAddr,
                            sizeof(cRtDataPtr->offerEntryRemoteAddr));
                        /* Set isSendSubEvtAfterRequest to TRUE,make sure sent SubscribeEventgroup only once after
                         * ConsumedEventGroup state change to REQUESTED,if Retry mechanism not setup */
                        consumedEgRTPtr->isSendSubEvtAfterRequest = TRUE;
                    }
                    else
                    {
                        /* Nothing to do here */
                    }
                    eventGroupPtr++;
                }
#endif

                cRtDataPtr->offerServiceRxFlag = FALSE;
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
        cEgPtr = clientServicePtr->SdConsumedEventGroup;
        for (index = 0u; index < clientServicePtr->SdConsumedEventGroupNum; index++)
        {
            consumedEgRTPtr = &Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
            if ((SD_CONSUMED_EVENTGROUP_AVAILABLE == consumedEgRTPtr->curState)
                && (SD_CONSUMED_EVENTGROUP_RELEASED == consumedEgRTPtr->setState))
            {
                (void)Sd_BuildStopSubscribeEventgroupEntry(
                    instancePtr,
                    clientServicePtr,
                    cEgPtr,
                    &consumedEgRTPtr->remoteAddr,
                    cRtDataPtr,
                    FALSE);

#if (SD_BSWM_USED == STD_ON)
                BswM_Sd_ConsumedEventGroupCurrentState(
                    cEgPtr->SdConsumedEventGroupHandleId,
                    SD_CONSUMED_EVENTGROUP_DOWN);
#endif
                consumedEgRTPtr->curState = SD_CONSUMED_EVENTGROUP_DOWN;

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
                if (cEgPtr->SdConsumedEventGroupTimerRef->SdSubscribeEventgroupRetryMax > 0u)
                {
                    consumedEgRTPtr->subsRetryCnt = 0u;
                    consumedEgRTPtr->subsRetryTimer = 0u;
                    consumedEgRTPtr->subsRetryEnable = FALSE;
                }
#endif
            }

            cEgPtr++;
        }
#endif
    }
}

/******************************************************************************/
/*
 * Brief: Enter Initial Wait Phase Process
 * Param-Name[in]: clientServicePtr, cRtDataPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ClientServiceDownPhaseHandle
 */
/******************************************************************************/
static void Sd_ClientServiceInitialWaitEnter(
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType* cRtDataPtr)
{
    const Sd_ClientTimerType* timerPtr;
    uint32 tRandom, delayMin, delayMax;

    /* Enter Initial Wait Phase */
    cRtDataPtr->phase = SD_CLIENT_PHASE_INITIAL_WAIT;
    /* make the number between SdServerTimerInitialOfferDelayMin to SdServerTimerInitialOfferDelayMax*/
    timerPtr = clientServicePtr->SdClientServiceTimerRef;
    delayMin = timerPtr->SdClientTimerInitialFindDelayMin;
    delayMax = timerPtr->SdClientTimerInitialFindDelayMax;
    /* Calculate random delay time */
    tRandom = Sd_GetRandomValue(delayMin, delayMax);
    /*@req <SWS_SD_00351> */
    /* When entering the Initial Wait Phase, a random timer shall be started */
    cRtDataPtr->initialWaitTimer = Sd_SendTimeAdjust(tRandom / SD_MAIN_FUNCTION_CYCLE_TIME);
}

/******************************************************************************/
/*
 * Brief: Process client service Down Phase enter(other than init)
 * Param-Name[in]: instancePtr, clientServicePtr, cRtDataPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ClientServiceMainPhaseHandle
 */
/******************************************************************************/
static void Sd_ClientServiceDownPhaseEnter(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType* cRtDataPtr)
{
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    const Sd_ConsumedEventGroupType* cEgPtr;
    Sd_ConsumedEventGroupRTType* cEgRTPtr;
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
    uint16 index;

    /* @req <SWS_SD_00381>,<SWS_SD_00712> */
    if (SD_CLIENT_PHASE_MAIN == cRtDataPtr->phase)
    {
        /* Clear send queue of this client service */
        Sd_ClearSendQueue(&cRtDataPtr->head);

        /* @req <SWS_SD_00722> */
        /* Call BswM_Sd_ClientServiceCurrentState with state SD_CLIENT_SERVICE_DOWN shall */
        if (SD_CLIENT_SERVICE_AVAILABLE == cRtDataPtr->curState)
        {
#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_ClientServiceCurrentState(clientServicePtr->SdClientServiceHandleId, SD_CLIENT_SERVICE_DOWN);
#endif
            cRtDataPtr->curState = SD_CLIENT_SERVICE_DOWN;
        }
        /* Close all Routing */
        Sd_ClientServiceDisableAllRoutingOfClient(clientServicePtr, cRtDataPtr);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        cEgPtr = clientServicePtr->SdConsumedEventGroup;
        for (index = 0u; index < clientServicePtr->SdConsumedEventGroupNum; index++)
        {
            cEgRTPtr = &Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
            if (SD_CONSUMED_EVENTGROUP_AVAILABLE == cEgRTPtr->curState)
            {
                /*@req <SWS_SD_00381> */
                /* A StopSubscribeEventgroup shall be sent */
                if (SD_CLIENT_SERVICE_RELEASED == cRtDataPtr->setState)
                {
                    (void)Sd_BuildStopSubscribeEventgroupEntry(
                        instancePtr,
                        clientServicePtr,
                        cEgPtr,
                        &cEgRTPtr->remoteAddr,
                        cRtDataPtr,
                        FALSE);
                }

/* BswM_Sd_ConsumedEventGroupCurrentState with state SD_CONSUMED_EVENTGROUP_DOWN for
 * all associated eventgroups of this Service Instance shall be called. */
#if (SD_BSWM_USED == STD_ON)
                BswM_Sd_ConsumedEventGroupCurrentState(
                    cEgPtr->SdConsumedEventGroupHandleId,
                    SD_CONSUMED_EVENTGROUP_DOWN);
#endif
                cEgRTPtr->curState = SD_CONSUMED_EVENTGROUP_DOWN;
            }

/* Cancel the corresponding client service subscription retry delay timer and
 * reset subscription retry counter of all corresponding Eventgroups to 0 */
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
            if (cEgPtr->SdConsumedEventGroupTimerRef->SdSubscribeEventgroupRetryMax > 0u)
            {
                cEgRTPtr->subsRetryCnt = 0u;
                cEgRTPtr->subsRetryTimer = 0u;
                cEgRTPtr->subsRetryEnable = FALSE;
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
    cRtDataPtr->phase = SD_CLIENT_PHASE_DOWN;
}

/******************************************************************************/
/*
 * Brief: Disable all Routing associated with this Client Service
 * Param-Name[in]: instancePtr, clientServicePtr, cRtDataPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ClientServiceDownPhaseEnter
 */
/******************************************************************************/
static void Sd_ClientServiceDisableAllRoutingOfClient(
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType* cRtDataPtr)
{
    const Sd_ConsumedEventGroupType* cEgPtr;
    const Sd_ConsumedMethodsType* consumedMethodsPtr;
    SoAd_RoutingGroupIdType routingGroupId;
    uint16 index;

    /*@req <SWS_SD_00382>,<SWS_SD_00722> */
    if ((SD_CLIENT_PHASE_REPETITION == cRtDataPtr->phase) || (SD_CLIENT_PHASE_MAIN == cRtDataPtr->phase))
    {
        consumedMethodsPtr = clientServicePtr->SdConsumedMethods;
        if (NULL_PTR != consumedMethodsPtr)
        {
            routingGroupId = consumedMethodsPtr->SdClientServiceActivationRef->SdRoutingGroupId;
            if (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened)
            {
                (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->tcpSoCon.soConId);
            }

            if (TRUE == cRtDataPtr->udpSoCon.isSoConOpened)
            {
                (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->udpSoCon.soConId);
            }
        }

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
        cEgPtr = clientServicePtr->SdConsumedEventGroup;
        for (index = 0u; index < clientServicePtr->SdConsumedEventGroupNum; index++)
        {
            Sd_ClientServiceDisableAllConsumedEventgroupRouting(cEgPtr, cRtDataPtr);
            cEgPtr++;
        }
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
    }
}

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
/******************************************************************************/
/*
 * Brief: Disable all Routing associated with this Client Service
 * Param-Name[in]: cEgPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ClientServiceDisableAllRoutingOfClient
 */
/******************************************************************************/
static void Sd_ClientServiceDisableAllConsumedEventgroupRouting(
    const Sd_ConsumedEventGroupType* cEgPtr,
    Sd_ClientServiceRTType* cRtDataPtr)
{
    Sd_ConsumedEventGroupRTType* cEgRTPtr;
    SoAd_RoutingGroupIdType routingGroupId;

    cEgRTPtr = &Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
    if ((NULL_PTR != cEgPtr->SdConsumedEventGroupTcpActivationRef) && (TRUE == cRtDataPtr->tcpSoCon.isSoConOpened))
    {
        routingGroupId = cEgPtr->SdConsumedEventGroupTcpActivationRef->SdRoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->tcpSoCon.soConId);

        cEgRTPtr->isTcpSubsSend = FALSE;
    }
    if ((NULL_PTR != cEgPtr->SdConsumedEventGroupUdpActivationRef) && (TRUE == cRtDataPtr->udpSoCon.isSoConOpened))
    {
        routingGroupId = cEgPtr->SdConsumedEventGroupUdpActivationRef->SdRoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cRtDataPtr->udpSoCon.soConId);

        cEgRTPtr->isUdpSubsSend = FALSE;
    }
    if ((NULL_PTR != cEgPtr->SdConsumedEventGroupMulticastActivationRef)
        && (TRUE == cEgRTPtr->multiSoCon.isSoConOpened))
    {
        routingGroupId = cEgPtr->SdConsumedEventGroupMulticastActivationRef->SdRoutingGroupId;
        (void)SoAd_DisableSpecificRouting(routingGroupId, cEgRTPtr->multiSoCon.soConId);
    }
}
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Close all Socket Connections associated with this Client Service Instance that
          have been opened before
 * Param-Name[in]: instancePtr, clientServicePtr, cRtDataPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ClientServiceDownPhaseEnter
 */
/******************************************************************************/
static void Sd_ClientServiceCloseAllSoCon(
    const Sd_ClientServiceType* clientServicePtr,
    Sd_ClientServiceRTType* cRtDataPtr)
{
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    const Sd_ConsumedEventGroupType* cEgPtr;
    Sd_ConsumedEventGroupRTType* cEgRTPtr;
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
    uint16 index;

    /*@req <SWS_SD_00603>,<SWS_SD_00705>,<SWS_SD_00706> */
    Sd_ClientServiceCloseSoCon(&cRtDataPtr->tcpSoCon);
    Sd_ClientServiceCloseSoCon(&cRtDataPtr->udpSoCon);

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    cEgPtr = clientServicePtr->SdConsumedEventGroup;
    for (index = 0u; index < clientServicePtr->SdConsumedEventGroupNum; index++)
    {
        cEgRTPtr = &Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
        Sd_ClientServiceCloseSoCon(&cEgRTPtr->multiSoCon);
        /* @req SWS_SD_00734 */
        /* Every wildcard socket connection group shall be reset to wildcard using
         * SoAd_ReleaseIpAddrAssignment()*/
        (void)SoAd_ReleaseIpAddrAssignment(cEgRTPtr->multiSoCon.soConId);

        cEgPtr++;
    }
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
}

/******************************************************************************/
/*
 * Brief: Close socket connection(Client Service)
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: soConPtr
 * Return: None
 * PreCondition: None
 * CallByAPI:
 */
/******************************************************************************/
static void Sd_ClientServiceCloseSoCon(Sd_SoConManageType* soConPtr)
{
    /*@req <SWS_SD_00603> */
    /* When the Down Phase is entered and SoAd_OpenSoCon() was called before, the
     * API SoAd_CloseSoCon()shall be called for all Socket Connections associated with
     * this Client Service Instance.*/
    if (TRUE == soConPtr->isSoConOpened)
    {
        (void)SoAd_CloseSoCon(soConPtr->soConId, TRUE);
        soConPtr->isSoConOpened = FALSE;
        soConPtr->isOptRcved = FALSE;

        /*@req <SWS_SD_00706> */
        SoAd_ReleaseRemoteAddr(soConPtr->soConId);
    }
}
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Switch data byte order
 * Param-Name[in]: size
 * Param-Name[out]:
 * Param-Name[in/out]: dataPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
SD_LOCAL_INLINE void Sd_EndianSwap(uint8* dataPtr, uint8 size)
{
    uint8 i;
    uint8 temp;
    uint8 halfSize = size >> 1u;
    uint8 tSize = size - 1u;
    uint8 tIdx;

    for (i = 0u; i < halfSize; i++)
    {
        tIdx = tSize - i;
        temp = dataPtr[tIdx];
        dataPtr[tIdx] = dataPtr[i];
        dataPtr[i] = temp;
    }
}
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

/******************************************************************************/
/*
 * Brief: Switch 32bit Network byte order to Host Byte Order
 * Param-Name[in]: net32DataPtr
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: a uint32 data with Host Byte Order
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint32 Sd_NtoHl(uint32 net32)
{
    uint32 data = net32;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&data, 4u);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return data;
}

/******************************************************************************/
/*
 * Brief: Switch 32bit Network byte order to Host Byte Order(In parameter is a pointer)
 * Param-Name[in]: net32DataPtr
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: a uint32 data with Host Byte Order
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint32 Sd_NPtrtoHl(const uint8* net32DataPtr)
{
    uint32 net32;

    (void)ILib_memcpy((uint8*)&net32, net32DataPtr, 4u);
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&net32, 4u);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return net32;
}

/******************************************************************************/
/*
 * Brief: Switch 32bit Host byte order to Network Byte Order
 * Param-Name[in]: net32
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: a uint32 data with Network Byte Order
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint32 Sd_HtoNl(uint32 host32)
{
    uint32 data = host32;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&data, 4u);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return data;
}

/******************************************************************************/
/*
 * Brief: Switch 16bit Network byte order to Host Byte Order
 * Param-Name[in]: net16
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: a uint16 data with Host Byte Order
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint16 Sd_NtoHs(uint16 net16)
{
    uint16 data = net16;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&data, 2u);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return data;
}

/******************************************************************************/
/*
 * Brief: Switch 16bit Network byte order to Host Byte Order(In parameter is a pointer)
 * Param-Name[in]: net16DataPtr
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: a uint16 data with Host Byte Order
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint16 Sd_NPtrtoHs(const uint8* net16DataPtr)
{
    uint16 net16;

    (void)ILib_memcpy((uint8*)&net16, net16DataPtr, 2u);
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&net16, 2u);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return net16;
}

/******************************************************************************/
/*
 * Brief: Switch 16bit Host byte order to Network Byte Order
 * Param-Name[in]: net16
 * Param-Name[out]:
 * Param-Name[in/out]: dataPtr
 * Return: a uint16 data with Local Byte Order
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint16 Sd_HtoNs(uint16 host16)
{
    uint16 data = host16;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian need swap */
    Sd_EndianSwap((uint8*)&data, 2u);
#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

    return data;
}

/******************************************************************************/
/*
 * Brief: Init memory pool
 * Param-Name[in]: memPoolSize,memPool
 * Param-Name[out]:
 * Param-Name[in/out]: dataPtr
 * Return: memory pool avaliable size
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
SD_LOCAL_INLINE uint8 Sd_InitMemoryPool(uint32 memPoolSize, void* memPool)
{
    return ILib_MemHeapInit(memPool, memPoolSize);
}

/******************************************************************************/
/*
 * Brief: require buffer from memory pool
 * Param-Name[in]: size,memPool
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: buffer pointer,NULL if not have enough buffer
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void* Sd_MallocEx(uint32 size, void* memPool)
{
    void* ret;

    SchM_Enter_Sd_ExclusiveArea();
    ret = ILib_MemHeapMalloc(memPool, size);
    SchM_Exit_Sd_ExclusiveArea();
#if ((SD_DEM_USED == STD_ON) && (SD_MALLOC_FAIL_REPORT_ENABLE == STD_ON))
    if (NULL_PTR == ret)
    {
        Dem_ReportErrorStatus(SD_MALLOC_FAIL_EVENTID, DEM_EVENT_STATUS_FAILED);
    }
#endif
    return ret;
}

/******************************************************************************/
/*
 * Brief: Free memory from a memory pool
 * Param-Name[in]: ptr,memPool
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_FreeEx(void* ptr, void* memPool)
{
    SchM_Enter_Sd_ExclusiveArea();
    (void)ILib_MemHeapFree(memPool, ptr);
    SchM_Exit_Sd_ExclusiveArea();
}

/******************************************************************************/
/*
 * Brief: Append RxBuff item to RxBuffList tail
 * Param-Name[in]: rxBuffItemPtr
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (SD_INSTANCE_NUM > 0)
static void Sd_AppendToRxBuffTail(Sd_RxQueueType* rxQueueItemPtr)
{
    Sd_RxQueueType* tail;

    SchM_Enter_Sd_ExclusiveArea();
    if (NULL_PTR != Sd_RxQueueHead)
    {
        tail = Sd_RxQueueHead;
        while (NULL_PTR != tail->next)
        {
            tail = tail->next;
        }
        tail->next = rxQueueItemPtr;
    }
    else
    {
        Sd_RxQueueHead = rxQueueItemPtr;
    }
    SchM_Exit_Sd_ExclusiveArea();
}
#endif

/******************************************************************************/
/*
 * Brief: Return a Sd_SubscribeManageType opinter that match soConId and counter
 * Param-Name[in]: header, soConId, counter
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: Sd_SubscribeManageType opinter that match soConId and counter
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (SD_EVENT_HANDLER_NUM > 0)
static Sd_SubscribeManageType* Sd_GetSubscribeManageObjAddr(
    Sd_SubscribeManageType* header,
    TcpIp_SockAddrType sockRemoteAddr,
    uint8 counter)
{
    Sd_SubscribeManageType* ptr = header;

    while (NULL_PTR != ptr)
    {
        if ((counter == ptr->counter)
            && (TCPIP_EQ_IPADDR(ptr->sockRemoteAddr, sockRemoteAddr)) /* PRQA S 3469*/ /* MISRA Dir 4.9 */
            && (ptr->sockRemoteAddr.port == sockRemoteAddr.port))
        {
            break;
        }

        ptr = ptr->next;
    }

    return ptr;
}

/******************************************************************************/
/*
 * Brief: Enable unicast for Subscribed Clients
 * Param-Name[in]: header, routingGroupId
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_EnableUnicastForSubscribedClient(
    const Sd_SubscribeManageType* header,
    SoAd_RoutingGroupIdType routingGroupId)
{
    const Sd_SubscribeManageType* ptr = header;

    while (NULL_PTR != ptr)
    {
        (void)SoAd_EnableSpecificRouting(routingGroupId, ptr->soConId);
        ptr = ptr->next;
    }
}

/******************************************************************************/
/*
 * Brief: Disable unicast for Subscribed Clients
 * Param-Name[in]: eventHandlerId, counter,newAllocateSw
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_DisableUnicastForSubscribedClient(
    const Sd_SubscribeManageType* header,
    SoAd_RoutingGroupIdType routingGroupId)
{
    const Sd_SubscribeManageType* ptr = header;

    while (NULL_PTR != ptr)
    {
        (void)SoAd_DisableSpecificRouting(routingGroupId, ptr->soConId);
        ptr = ptr->next;
    }
}

/******************************************************************************/
/*
 * Brief: Append a client to subscribe list
 * Param-Name[in]: headerPtr, soConId, ttl, counter
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK:Append OK,E_NOT_OK:Append fail,no enough buffer
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_AddClientToSubscribeList(
    Sd_SubscribeManageType** headerPtr,
    SoAd_SoConIdType soConId,
    TcpIp_SockAddrType sockRemoteAddr,
    uint32 ttl,
    uint8 counter)
{
    Sd_SubscribeManageType *ptr = *headerPtr, *subscribePtr;
    Std_ReturnType ret = E_NOT_OK;

    subscribePtr = Sd_MallocEx(sizeof(Sd_SubscribeManageType), Sd_TxMemPool);
    if (NULL_PTR != subscribePtr)
    {
        subscribePtr->soConId = soConId;
        subscribePtr->counter = counter;
        subscribePtr->sockRemoteAddr = sockRemoteAddr;
        subscribePtr->next = NULL_PTR;
        Sd_SetTTL(&subscribePtr->ttl, ttl);

        if (NULL_PTR == ptr)
        {
            *headerPtr = subscribePtr;
        }
        else
        {
            while (NULL_PTR != ptr->next)
            {
                ptr = ptr->next;
            }
            ptr->next = subscribePtr;
        }
        ret = E_OK;
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Delete a client from subscribe list
 * Param-Name[in]: eventHandlerId, headerPtr, soConId, counter
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_DeleteClientFromSubscribeList(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    Sd_SubscribeManageType** headerPtr,
    SoAd_SoConIdType soConId,
    uint8 counter)
{
    Sd_SubscribeManageType *ptr, *lastPtr;
    const Sd_EventHandlerRTType* ehRTDataPtr;
    uint16 eventHandlerId;
    boolean isTcpSoCon;

    ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
    if (ehRTDataPtr->tcpSubscribeList == *headerPtr)
    {
        isTcpSoCon = TRUE;
    }
    else
    {
        isTcpSoCon = FALSE;
    }

    ptr = *headerPtr;
    lastPtr = NULL_PTR;
    while (NULL_PTR != ptr)
    {
        if ((soConId == ptr->soConId) && (counter == ptr->counter))
        {
            break;
        }

        lastPtr = ptr;
        ptr = ptr->next;
    }

    if (NULL_PTR == lastPtr)
    {
        *headerPtr = ptr->next;
    }
    else
    {
        lastPtr->next = ptr->next;
    }

    if ((0u == ehRTDataPtr->tcpSubsClientNum) && (0u == ehRTDataPtr->udpSubsClientNum))
    {
        /*@req <SWS_SD_00334>,<SWS_SD_00345>,<SWS_SD_00458>,<SWS_SD_00403> */
        /* If this has been the last subscribed client, report SD_EVENT_HANDLER_RELEASED
         * to the BswM by calling the API BswM_Sd_EventHandlerCurrentState().*/
        eventHandlerId = eventHandlerPtr->SdEventHandlerHandleId;
#if (SD_BSWM_USED == STD_ON)
        BswM_Sd_EventHandlerCurrentState(eventHandlerId, SD_EVENT_HANDLER_RELEASED);
#endif
        Sd_EventHandlerRTData[eventHandlerId].curState = SD_EVENT_HANDLER_RELEASED;
    }

    Sd_ResetSoConToWildcard(serverServicePtr, soConId, isTcpSoCon);
    Sd_FreeEx(ptr, Sd_TxMemPool);
}

/******************************************************************************/
/*
 * Brief: Delete all clients from subscribe list
 * Param-Name[in]: headerPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_RemoveAllClientFromSubscribeList(Sd_SubscribeManageType** headerPtr)
{
    Sd_SubscribeManageType *nowPtr, *nextPtr;

    nowPtr = *headerPtr;
    while (NULL_PTR != nowPtr)
    {
        nextPtr = nowPtr->next;

        Sd_FreeEx(nowPtr, Sd_TxMemPool);

        nowPtr = nextPtr;
    }

    *headerPtr = NULL_PTR;
}

/******************************************************************************/
/*
 * Brief: Update subscribed client's TTL
 * Param-Name[in]: ttl
 * Param-Name[out]: None
 * Param-Name[in/out]: subscribePtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_UpdateSubscribeClientTTL(Sd_SubscribeManageType* subscribePtr, uint32 ttl)
{
    Sd_SetTTL(&subscribePtr->ttl, ttl);
}

/******************************************************************************/
/*
 * Brief: Handle subscribe client's TTL
 *        if ttl expired,delete it from subscribe list
 * Param-Name[In]: serverServicePtr
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_SubscribeClientTTLManage(const Sd_ServerServiceType* serverServicePtr)
{
    const Sd_EventHandlerType* eventHandlerPtr;
    const Sd_EventHandlerTcpType* eventHandlerTcpPtr;
    Sd_EventHandlerRTType* ehRTDataPtr;
    Sd_SubscribeManageType* subsMangPtr;
    SoAd_RoutingGroupIdType routingGroupId;
    uint16 idx;

    eventHandlerPtr = serverServicePtr->SdEventHandler;
    for (idx = 0u; idx < serverServicePtr->SdEventHandlerNum; idx++)
    {
        ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];

        /* Handle TCP subscribed client */
        subsMangPtr = ehRTDataPtr->tcpSubscribeList;
        while (NULL_PTR != subsMangPtr)
        {
            if (subsMangPtr->ttl.ttlms > 0u)
            {
                subsMangPtr->ttl.ttlms--;
                if (0u == subsMangPtr->ttl.ttlms)
                {
                    /* Disable Routing Path*/
                    eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
                    if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
                    {
                        routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->SdRoutingGroupId;
                        (void)SoAd_DisableSpecificRouting(routingGroupId, subsMangPtr->soConId);
                    }

                    ehRTDataPtr->tcpSubsClientNum--;
                    Sd_DeleteClientFromSubscribeList(
                        serverServicePtr,
                        eventHandlerPtr,
                        &ehRTDataPtr->tcpSubscribeList,
                        subsMangPtr->soConId,
                        subsMangPtr->counter);
                }
            }

            subsMangPtr = subsMangPtr->next;
        }

        /* Handle UDP subscribed client */
        subsMangPtr = ehRTDataPtr->udpSubscribeList;
        while (NULL_PTR != subsMangPtr)
        {
            if (subsMangPtr->ttl.ttlms > 0u)
            {
                subsMangPtr->ttl.ttlms--;
                if (0u == subsMangPtr->ttl.ttlms)
                {
                    /* Fan out control*/
                    Sd_FanOutMulticastUnicastCtrlDelClient(eventHandlerPtr, ehRTDataPtr, subsMangPtr->soConId);

                    ehRTDataPtr->udpSubsClientNum--;
                    Sd_DeleteClientFromSubscribeList(
                        serverServicePtr,
                        eventHandlerPtr,
                        &ehRTDataPtr->udpSubscribeList,
                        subsMangPtr->soConId,
                        subsMangPtr->counter);
                }
            }

            subsMangPtr = subsMangPtr->next;
        }

        eventHandlerPtr++;
    }
}

/******************************************************************************/
/*
 * Brief: Fan out control TCP
 * Param-Name[in]: instancePtr, serverServicePtr, eventHandlerPtr, sdMsg, entryPos, optStartPosMap
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK-No error,E_NOT_OK-error,need response use Nack entry
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_FanOutControlTCP(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap)
{
    Sd_Type2EntryType type2Entry;
    Sd_IPv4OptionsType ipv4Opt;
#if (STD_ON == SD_IPV6_ENABLE)
    Sd_IPv6OptionsType ipv6Opt;
    uint16 ipv6EndpTcpPos = 0u;
#endif /* STD_ON == SD_IPV6_ENABLE */
    uint32 ttl = 0u;
    uint16 ipv4EndpTcpPos = 0u;
    Std_ReturnType ret = E_OK;
    uint8 idx1stOptions, numOpt1;
    uint8 idx2ndOptions, numOpt2;
    uint8 index;
    uint8 optType;
    uint8 optIndex;
    uint8 optNumSum;

    (void)ILib_memcpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));
    idx1stOptions = type2Entry.Index1stOptions;
    numOpt1 = type2Entry.NumOfOpt1;
    idx2ndOptions = type2Entry.Index2ndOptions;
    numOpt2 = type2Entry.NumOfOpt2;
    (void)ILib_memcpy((((uint8*)&ttl) + 1u), type2Entry.TTL, 3u);
    ttl = Sd_NtoHl(ttl);

    /* Find options position */
    optNumSum = numOpt1 + numOpt2;
    for (index = 0; index < optNumSum; index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        optType = sdMsg[optStartPosMap[optIndex] + 2u];
        if (SD_IPV4_ENDPOINT_OPTION == optType)
        {
            (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
            if ((SD_L4_PROTO_TCP == ipv4Opt.L4Proto) && (0u == ipv4EndpTcpPos))
            {
                /* Save endpoint option position */
                ipv4EndpTcpPos = optStartPosMap[optIndex];
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else if (SD_IPV6_ENDPOINT_OPTION == optType)
        {
            (void)ILib_memcpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
            if ((SD_L4_PROTO_TCP == ipv6Opt.L4Proto) && (0u == ipv6EndpTcpPos))
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

        if ((0u != ipv4EndpTcpPos)
#if (STD_ON == SD_IPV6_ENABLE)
            || (0u != ipv6EndpTcpPos)
#endif /* STD_ON == SD_IPV6_ENABLE */
        )
        {
            break;
        }
    }

    if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
    {
        /* Fan out control(TCP,IPv4) */
        (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[ipv4EndpTcpPos], sizeof(Sd_IPv4OptionsType));
        ret = Sd_FanOutControlTcpHandle(
            serverServicePtr,
            eventHandlerPtr,
            &ipv4Opt,
            NULL_PTR,
            ttl,
            (uint8)type2Entry.Counter);
    }
#if (STD_ON == SD_IPV6_ENABLE)
    else
    {
        /* Fan out control(TCP,IPv6) */
        (void)ILib_memcpy((uint8*)&ipv6Opt, &sdMsg[ipv6EndpTcpPos], sizeof(Sd_IPv6OptionsType));
        ret = Sd_FanOutControlTcpHandle(
            serverServicePtr,
            eventHandlerPtr,
            NULL_PTR,
            &ipv6Opt,
            ttl,
            (uint8)type2Entry.Counter);
    }
#endif /* STD_ON == SD_IPV6_ENABLE */

    return ret;
}

/******************************************************************************/
/*
 * Brief: Fan out control UDP
 * Param-Name[in]: instancePtr, serverServicePtr, eventHandlerPtr, sdMsg, entryPos, optStartPosMap
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK-No error,E_NOT_OK-error,need response use Nack entry
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_FanOutControlUDP(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap)
{
    Sd_Type2EntryType type2Entry;
    Sd_IPv4OptionsType ipv4Opt;
#if (STD_ON == SD_IPV6_ENABLE)
    Sd_IPv6OptionsType ipv6Opt;
    uint16 ipv6EndpUdpPos = 0u; /* Store first UDP IPv6 Endpoint Option Position */
#endif                          /* STD_ON == SD_IPV6_ENABLE */
    uint32 ttl = 0u;
    uint16 ipv4EndpUdpPos = 0u; /* Store first UDP IPv4 Endpoint Option Position */
    Std_ReturnType ret = E_OK;
    uint8 idx1stOptions, numOpt1;
    uint8 idx2ndOptions, numOpt2;
    uint8 index;
    uint8 optType;
    uint8 optIndex;
    uint8 optNumSum;

    (void)ILib_memcpy((uint8*)&type2Entry, &sdMsg[entryPos], sizeof(Sd_Type2EntryType));
    idx1stOptions = type2Entry.Index1stOptions;
    numOpt1 = type2Entry.NumOfOpt1;
    idx2ndOptions = type2Entry.Index2ndOptions;
    numOpt2 = type2Entry.NumOfOpt2;
    (void)ILib_memcpy((((uint8*)&ttl) + 1u), type2Entry.TTL, 3u);
    ttl = Sd_NtoHl(ttl);

    /* Find options position */
    optNumSum = numOpt1 + numOpt2;
    for (index = 0; (index < optNumSum) && (E_OK == ret); index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        optType = sdMsg[optStartPosMap[optIndex] + 2u];
        if (SD_IPV4_ENDPOINT_OPTION == optType)
        {
            (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
            if ((SD_L4_PROTO_UDP == ipv4Opt.L4Proto) && (0u == ipv4EndpUdpPos))
            {
                ipv4EndpUdpPos = optStartPosMap[optIndex];
            }
        }
#if (STD_ON == SD_IPV6_ENABLE)
        else if (SD_IPV6_ENDPOINT_OPTION == optType)
        {
            (void)ILib_memcpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
            if ((SD_L4_PROTO_UDP == ipv6Opt.L4Proto) && (0u == ipv6EndpUdpPos))
            {
                ipv6EndpUdpPos = optStartPosMap[optIndex];
            }
        }
        else
        {
            /*Nothing to do here*/
        }
#endif /* STD_ON == SD_IPV6_ENABLE */

        if ((0u != ipv4EndpUdpPos)
#if (STD_ON == SD_IPV6_ENABLE)
            || (0u != ipv6EndpUdpPos)
#endif /* STD_ON == SD_IPV6_ENABLE */
        )
        {
            break;
        }
    }

    if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
    {
        /* Fan out control(UDP,IPv4) */
        (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[ipv4EndpUdpPos], sizeof(Sd_IPv4OptionsType));
        ret = Sd_FanOutControlUdpHandle(
            instancePtr,
            serverServicePtr,
            eventHandlerPtr,
            &ipv4Opt,
            NULL_PTR,
            ttl,
            (uint8)type2Entry.Counter);
    }
#if (STD_ON == SD_IPV6_ENABLE)
    else
    {
        /* Fan out control(UDP,IPv6) */
        (void)ILib_memcpy((uint8*)&ipv6Opt, &sdMsg[ipv6EndpUdpPos], sizeof(Sd_IPv6OptionsType));
        ret = Sd_FanOutControlUdpHandle(
            instancePtr,
            serverServicePtr,
            eventHandlerPtr,
            NULL_PTR,
            &ipv6Opt,
            ttl,
            (uint8)type2Entry.Counter);
    }
#endif /* STD_ON == SD_IPV6_ENABLE */

    return ret;
}

/******************************************************************************/
/*
 * Brief: Fan out control TCP handle
 *        [ipv4OptPtr] or [ipv6OptPtr] must have one equal NULL_PTR
 * Param-Name[out]: serverServicePtr, eventHandlerPtr, ipv4OptPtr, ipv6OptPtr, ttl, counter
 * Param-Name[in/out]: None
 * Return: E_OK-No Error,E_NOT_OK-Error,need response Nack
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_FanOutControlTcpHandle(
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_IPv4OptionsType* ipv4OptPtr,
    const Sd_IPv6OptionsType* ipv6OptPtr,
    uint32 ttl,
    uint8 counter)
{
    Sd_SubscribeManageType* subscribeManagePtr;
    Sd_EventHandlerRTType* ehRTDataPtr;
    const Sd_EventHandlerTcpType* eventHandlerTcpPtr;
    TcpIp_SockAddrType sockAddr;
    sint32 isSameIpAddr = 1;
    SoAd_SoConIdType soConId;
    SoAd_RoutingGroupIdType routingGroupId;
    uint16 soConNum;
    uint16 loop;
    boolean findSoConFlag;
    Std_ReturnType ret = E_OK;
    SoAd_SoConModeType soConMode = SOAD_SOCON_OFFLINE;

    if (ttl > 0u)
    {
        /* @req SWS_SD_00453 */
        soConNum = serverServicePtr->SdServerServiceTcpRef->SdSoConNumInGroup;
        findSoConFlag = FALSE;
        for (loop = 0u; loop < soConNum; loop++)
        {
            soConId = serverServicePtr->SdServerServiceTcpRef->SdSocketIdList[loop];
            if (NULL_PTR != ipv4OptPtr)
            {
                sockAddr.domain = TCPIP_AF_INET;
                (void)SoAd_GetRemoteAddr(soConId, &sockAddr);
                isSameIpAddr = ILib_memcmp((uint8*)sockAddr.addr, ipv4OptPtr->IPv4Address, 4);
            }
            else if (NULL_PTR != ipv6OptPtr)
            {
                sockAddr.domain = TCPIP_AF_INET6;
                (void)SoAd_GetRemoteAddr(soConId, &sockAddr);
                isSameIpAddr = ILib_memcmp((uint8*)sockAddr.addr, ipv6OptPtr->IPv6Address, 16);
            }
            else
            {
                /* Nothing to do here */
            }
            if ((0 == isSameIpAddr)
                && (((NULL_PTR != ipv4OptPtr) && (sockAddr.port == Sd_NtoHs(ipv4OptPtr->PortNumber)))
                    || ((NULL_PTR != ipv6OptPtr) && (sockAddr.port == Sd_NtoHs(ipv6OptPtr->PortNumber)))))
            {
                /* Find Relevant TCP Socket Connection*/
                findSoConFlag = TRUE;
                SoAd_GetSoConMode(soConId, &soConMode);
                break;
            }
        }

        if ((TRUE == findSoConFlag) && (SOAD_SOCON_ONLINE == soConMode))
        {
            ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
            subscribeManagePtr = Sd_GetSubscribeManageObjAddr(ehRTDataPtr->tcpSubscribeList, sockAddr, counter);
            if (NULL_PTR == subscribeManagePtr)
            {
                /*Enable Routing */
                eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
                if (NULL_PTR != eventHandlerTcpPtr)
                {
                    if (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef)
                    {
                        routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->SdRoutingGroupId;
                        (void)SoAd_EnableSpecificRouting(routingGroupId, soConId);
                    }
                    if (NULL_PTR != eventHandlerTcpPtr->SdEventTriggeringRef)
                    {
                        routingGroupId = eventHandlerTcpPtr->SdEventTriggeringRef->SdRoutingGroupId;
                        /* PATCH for TC8: Field event transmit befor ACK */
                        ehRTDataPtr->ifSpecificRoutingTransTcpFlag = TRUE;
                        ehRTDataPtr->routingGroupIdTcp = routingGroupId;
                    }
                    ehRTDataPtr->soConIdTcp = soConId; /* Used also for close Tcp routing */
                }

                if (ehRTDataPtr->tcpSubsClientNum < 16u) /* max 16 client */
                {
                    /* Add client to subscribe list */
                    ret = Sd_AddClientToSubscribeList(&ehRTDataPtr->tcpSubscribeList, soConId, sockAddr, ttl, counter);
                    if (E_OK == ret)
                    {
                        ehRTDataPtr->tcpSubsClientNum++;
                    }
                }
            }
            else
            {
                if (soConId != subscribeManagePtr->soConId)
                {
                    /*The IP Address of client changed,should close socket connection used before */
                    eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
                    if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
                    {
                        routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->SdRoutingGroupId;
                        (void)SoAd_DisableSpecificRouting(routingGroupId, subscribeManagePtr->soConId);
                    }
                    (void)SoAd_CloseSoCon(subscribeManagePtr->soConId, TRUE);

                    /*Update soCondId*/
                    subscribeManagePtr->soConId = soConId;
                    ehRTDataPtr->soConIdTcp = soConId; /* Used for close Tcp routing */

                    /* Open new socket connection */
                    if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
                    {
                        routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->SdRoutingGroupId;
                        (void)SoAd_EnableSpecificRouting(routingGroupId, soConId);
                    }
                }

                /* Update TTL */
                Sd_UpdateSubscribeClientTTL(subscribeManagePtr, ttl);
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
        /* StopSubscribeEventGroupEntry */
        if (NULL_PTR != ipv4OptPtr)
        {
            sockAddr.domain = TCPIP_AF_INET;
            sockAddr.port = Sd_NtoHs(ipv4OptPtr->PortNumber);
            (void)ILib_memcpy((uint8*)sockAddr.addr, (const uint8*)ipv4OptPtr->IPv4Address, 4);
        }
        else
        {
            sockAddr.domain = TCPIP_AF_INET6;
            sockAddr.port = Sd_NtoHs(ipv6OptPtr->PortNumber);
            (void)ILib_memcpy((uint8*)sockAddr.addr, (const uint8*)ipv6OptPtr->IPv6Address, 16);
        }

        ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
        subscribeManagePtr = Sd_GetSubscribeManageObjAddr(ehRTDataPtr->tcpSubscribeList, sockAddr, counter);
        if (NULL_PTR != subscribeManagePtr)
        {
            /* Close Routing Path */
            eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
            if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
            {
                routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->SdRoutingGroupId;
                (void)SoAd_DisableSpecificRouting(routingGroupId, subscribeManagePtr->soConId);
            }

            /* StopSubscribeEventGroupEntry,Delete client from subscribe list */
            ehRTDataPtr->tcpSubsClientNum--;
            Sd_DeleteClientFromSubscribeList(
                serverServicePtr,
                eventHandlerPtr,
                &ehRTDataPtr->tcpSubscribeList,
                subscribeManagePtr->soConId,
                counter);
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Fan out control UDP handle
 *        [ipv4OptPtr] or [ipv6OptPtr] must have one equal NULL_PTR
 * Param-Name[out]: instancePtr, serverServicePtr, eventHandlerPtr, ipv4OptPtr, ipv6OptPtr, ttl, counter
 * Param-Name[in/out]: None
 * Return: E_OK-No Error,E_NOT_OK-Error,need response Nack
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_FanOutControlUdpHandle(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_IPv4OptionsType* ipv4OptPtr,
    const Sd_IPv6OptionsType* ipv6OptPtr,
    uint32 ttl,
    uint8 counter)
{
    Sd_SubscribeManageType* subscribeManagePtr;
    Sd_EventHandlerRTType* ehRTDataPtr;
    const Sd_EventHandlerUdpType* eventHandlerUdpPtr;
    TcpIp_SockAddrType sockAddr;
    SoAd_SoConIdType soConId, assignedSoConId;
    SoAd_RoutingGroupIdType routingGroupId;
    Std_ReturnType ret;

    /*@req <SWS_SD_00454> */
    ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
    if (NULL_PTR != ipv4OptPtr)
    {
        sockAddr.domain = TCPIP_AF_INET;
        sockAddr.port = Sd_NtoHs(ipv4OptPtr->PortNumber);
        (void)ILib_memcpy((uint8*)sockAddr.addr, (const uint8*)ipv4OptPtr->IPv4Address, 4);
    }
    else
    {
        sockAddr.domain = TCPIP_AF_INET6;
        sockAddr.port = Sd_NtoHs(ipv6OptPtr->PortNumber);
        (void)ILib_memcpy((uint8*)sockAddr.addr, (const uint8*)ipv6OptPtr->IPv6Address, 16);
    }
    /* Any socket connection that is part of the SoAdSocketConnectionGroup*/
    soConId = serverServicePtr->SdServerServiceUdpRef->SdSocketIdList[0];
    ret = SoAd_SetUniqueRemoteAddr(soConId, &sockAddr, &assignedSoConId);

    if (E_OK == ret)
    {
        subscribeManagePtr = Sd_GetSubscribeManageObjAddr(ehRTDataPtr->udpSubscribeList, sockAddr, counter);
        if (NULL_PTR == subscribeManagePtr)
        {
            if (ttl > 0u)
            {
                if (ehRTDataPtr->udpSubsClientNum < 16u) /* max 16 client */
                {
                    /* Add client to subscribe list */
                    ret = Sd_AddClientToSubscribeList(
                        &ehRTDataPtr->udpSubscribeList,
                        assignedSoConId,
                        sockAddr,
                        ttl,
                        counter);
                    if (E_NOT_OK != ret)
                    {
                        /*@req <SWS_SD_00455> */
                        ehRTDataPtr->udpSubsClientNum++;
                        Sd_FanOutMulticastUnicastCtrlAddClient(eventHandlerPtr, ehRTDataPtr, assignedSoConId);
                    }
                }
            }
            else
            {
                /* StopSubscribeEventGroupEntry,not in SubscribeList,Ignore */
            }
        }
        else
        {
            if (ttl > 0u)
            {
                if (assignedSoConId != subscribeManagePtr->soConId)
                {
                    eventHandlerUdpPtr = eventHandlerPtr->SdEventHandlerUdp;
                    routingGroupId = eventHandlerUdpPtr->SdEventActivationRef->SdRoutingGroupId;
                    /* The IP Address of client changed,should close socket connection used before */
                    (void)SoAd_DisableSpecificRouting(routingGroupId, subscribeManagePtr->soConId);
                    (void)SoAd_CloseSoCon(subscribeManagePtr->soConId, TRUE);
                    if (ehRTDataPtr->udpSubsClientNum < eventHandlerPtr->SdEventHandlerMulticastThreshold)
                    {
                        (void)SoAd_EnableSpecificRouting(routingGroupId, assignedSoConId);
                        if (NULL_PTR != eventHandlerUdpPtr->SdEventTriggeringRef)
                        {
                            routingGroupId = eventHandlerUdpPtr->SdEventTriggeringRef->SdRoutingGroupId;
                            /* PATCH for TC8: Field event transmit befor ACK */
                            ehRTDataPtr->ifSpecificRoutingTransUdpFlag = TRUE;
                            ehRTDataPtr->routingGroupIdUdp = routingGroupId;
                            ehRTDataPtr->soConIdUdp = soConId;
                        }
                    }
                    /* Update soCondId */
                    subscribeManagePtr->soConId = assignedSoConId;

                    /* Open new soCon */
                    (void)SoAd_OpenSoCon(assignedSoConId);
                }

                /* Update TTL */
                Sd_UpdateSubscribeClientTTL(subscribeManagePtr, ttl);
            }
            else
            {
                /*@req <SWS_SD_00455> */
                /* StopSubscribeEventGroupEntry,Delete client from subscribe list */
                /* Fan out control*/
                Sd_FanOutMulticastUnicastCtrlDelClient(eventHandlerPtr, ehRTDataPtr, subscribeManagePtr->soConId);
                /* Delete client*/
                ehRTDataPtr->udpSubsClientNum--;
                Sd_DeleteClientFromSubscribeList(
                    serverServicePtr,
                    eventHandlerPtr,
                    &ehRTDataPtr->udpSubscribeList,
                    subscribeManagePtr->soConId,
                    counter);
            }
        }
        SD_UNUSED(instancePtr);
    }
#if (SD_DEM_USED == STD_ON)
    else
    {
        /* If no Wildcard Socket Connection is left, SD_E_OUT_OF_RES shall be reported.*/
        const Sd_DemEventParameterType* demEventPtr;
        demEventPtr = instancePtr->SdInstanceDemEventParameterRefs->SD_E_OUT_OF_RES;
        if (NULL_PTR != demEventPtr)
        {
            Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
        }
    }
#endif

    return ret;
}

/******************************************************************************/
/*
 * Brief: Multicast and Unicast switch control in Fan out control(add Client)
 * Param-Name[out]: eventHandlerPtr, ehRTDataPtr, assignedSoConId, counter
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_FanOutMulticastUnicastCtrlAddClient(
    const Sd_EventHandlerType* eventHandlerPtr,
    Sd_EventHandlerRTType* ehRTDataPtr,
    SoAd_SoConIdType assignedSoConId)
{
    const Sd_EventHandlerUdpType* eventHandlerUdpPtr;
    const Sd_EventHandlerMulticastType* eventHandlerMulticastPtr;
    SoAd_RoutingGroupIdType routingGroupId;
    SoAd_RoutingGroupIdType multicastRoutingGroupId;
    SoAd_SoConIdType multiSoConId;
    uint16 subscribedClientNum;

    /*@req <SWS_SD_00454>,<SWS_SD_00455> */
    /* client was not subscribe before receiving the subscribe eventgroup entry */
    subscribedClientNum = ehRTDataPtr->udpSubsClientNum;
    eventHandlerUdpPtr = eventHandlerPtr->SdEventHandlerUdp;
    if ((NULL_PTR != eventHandlerUdpPtr) && (NULL_PTR != eventHandlerUdpPtr->SdEventActivationRef))
    {
        routingGroupId = eventHandlerUdpPtr->SdEventActivationRef->SdRoutingGroupId;
        if ((0u == eventHandlerPtr->SdEventHandlerMulticastThreshold)
            || (subscribedClientNum < eventHandlerPtr->SdEventHandlerMulticastThreshold))
        {
            /* Setup Unicast for this time subscribed client */
            (void)SoAd_EnableSpecificRouting(routingGroupId, assignedSoConId);
        }
        else if (subscribedClientNum == eventHandlerPtr->SdEventHandlerMulticastThreshold)
        {
            /* Setup Multicast for all subscribed clients */
            eventHandlerMulticastPtr = eventHandlerPtr->SdEventHandlerMulticast;
            if ((NULL_PTR != eventHandlerMulticastPtr) && (NULL_PTR != eventHandlerMulticastPtr->SdEventActivationRef)
                && (NULL_PTR != eventHandlerMulticastPtr->SdMulticastEventSoConRef))
            {
                multicastRoutingGroupId = eventHandlerMulticastPtr->SdEventActivationRef->SdRoutingGroupId;
                multiSoConId = eventHandlerMulticastPtr->SdMulticastEventSoConRef->SdSoConId;
                (void)SoAd_EnableSpecificRouting(multicastRoutingGroupId, multiSoConId);
            }

            /* Disable Unicast for all subscribed clients */
            Sd_DisableUnicastForSubscribedClient(ehRTDataPtr->udpSubscribeList, routingGroupId);
        }
        else
        {
            /*Nothing to do here*/
        }
    }

    if ((NULL_PTR != eventHandlerUdpPtr) && (NULL_PTR != eventHandlerUdpPtr->SdEventTriggeringRef))
    {
        routingGroupId = eventHandlerUdpPtr->SdEventTriggeringRef->SdRoutingGroupId;
        /* PATCH for TC8: Field event transmit befor ACK */
        ehRTDataPtr->ifSpecificRoutingTransUdpFlag = TRUE;
        ehRTDataPtr->routingGroupIdUdp = routingGroupId;
        ehRTDataPtr->soConIdUdp = assignedSoConId;
    }
}

/******************************************************************************/
/*
 * Brief: Multicast and Unicast switch control in Fan out control(Delete Client)
 * Param-Name[out]: eventHandlerPtr, ehRTDataPtr, assignedSoConId, counter
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_FanOutMulticastUnicastCtrlDelClient(
    const Sd_EventHandlerType* eventHandlerPtr,
    const Sd_EventHandlerRTType* ehRTDataPtr,
    SoAd_SoConIdType soConId)
{
    const Sd_EventHandlerUdpType* eventHandlerUdpPtr;
    const Sd_EventHandlerMulticastType* eventHandlerMulticastPtr;
    SoAd_RoutingGroupIdType routingGroupId;
    SoAd_RoutingGroupIdType multicastRoutingGroupId;
    SoAd_SoConIdType multiSoConId;
    uint16 subscribedClientNum;

    /*@req <SWS_SD_00455> */
    subscribedClientNum = ehRTDataPtr->udpSubsClientNum;
    eventHandlerUdpPtr = eventHandlerPtr->SdEventHandlerUdp;
    if ((NULL_PTR != eventHandlerUdpPtr) && (NULL_PTR != eventHandlerUdpPtr->SdEventActivationRef))
    {
        routingGroupId = eventHandlerUdpPtr->SdEventActivationRef->SdRoutingGroupId;
        if ((0u == eventHandlerPtr->SdEventHandlerMulticastThreshold)
            || (subscribedClientNum < eventHandlerPtr->SdEventHandlerMulticastThreshold))
        {
            /* Disable this time will be deleted client's Routing Path */
            (void)SoAd_DisableSpecificRouting(routingGroupId, soConId);
        }
        else if (subscribedClientNum == eventHandlerPtr->SdEventHandlerMulticastThreshold)
        {
            /* Disable Multicast */
            eventHandlerMulticastPtr = eventHandlerPtr->SdEventHandlerMulticast;
            if ((NULL_PTR != eventHandlerMulticastPtr) && (NULL_PTR != eventHandlerMulticastPtr->SdEventActivationRef)
                && (NULL_PTR != eventHandlerMulticastPtr->SdMulticastEventSoConRef))
            {
                multicastRoutingGroupId = eventHandlerMulticastPtr->SdEventActivationRef->SdRoutingGroupId;
                multiSoConId = eventHandlerMulticastPtr->SdMulticastEventSoConRef->SdSoConId;
                (void)SoAd_DisableSpecificRouting(multicastRoutingGroupId, multiSoConId);
            }

            /* Setup Unicast for subscribed clients */
            Sd_EnableUnicastForSubscribedClient(ehRTDataPtr->udpSubscribeList, routingGroupId);

            /* Disable this time will be deleted client's Routing Path */
            (void)SoAd_DisableSpecificRouting(routingGroupId, soConId);
        }
        else
        {
            /*Nothing to do here*/
        }
    }
}
#endif /* SD_EVENT_HANDLER_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Save SoConId that match [remoteAddrPtr]
 *        if not opend SoConId before,Open it
 *        if get SoConId from SoAd not equal before,close before one,then open the new one
 * Param-Name[in]: clientServicePtr, remoteAddrPtr, pahse, isUseTcp
 * Param-Name[out]: None
 * Param-Name[in/out]: soConPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
#if (SD_CLIENT_SERVICE_NUM > 0)
static void Sd_ClientSaveSoConId(
    const Sd_ClientServiceType* clientServicePtr,
    const TcpIp_SockAddrType* remoteAddrPtr,
    boolean isUseTcp,
    Sd_SoConManageType* soConPtr)
{
    SoAd_SoConIdType soConId;
    SoAd_SoConModeType soConMode;
    Std_ReturnType ret;

    if (TRUE == isUseTcp)
    {
        soConId = clientServicePtr->SdClientServiceTcpRef->SdSocketIdList[0];
    }
    else
    {
        soConId = clientServicePtr->SdClientServiceUdpRef->SdSocketIdList[0];
    }
    ret = SoAd_SetUniqueRemoteAddr(soConId, remoteAddrPtr, &soConId);
    if (E_OK == ret)
    {
        SoAd_GetSoConMode(soConId, &soConMode);
        if ((FALSE == soConPtr->isSoConOpened) || (SOAD_SOCON_ONLINE != soConMode) || (soConId != soConPtr->soConId))
        {
            if (soConId != soConPtr->soConId)
            {
                Sd_ClientServiceCloseSoCon(soConPtr);
            }

            /*@req <SWS_SD_00604> */
            ret = SoAd_OpenSoCon(soConId);
            if (E_OK == ret)
            {
                soConPtr->isOptRcved = TRUE;
                soConPtr->isSoConOpened = TRUE;
                soConPtr->soConId = soConId;

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
                Sd_ClientServiceResetAllCEgSubsEntrySendFlag(clientServicePtr, isUseTcp, (!isUseTcp));
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
            }
        }
    }
}

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
/******************************************************************************/
/*
 * Brief: Save SoConId that match [remoteAddrPtr]
 *        if not opend SoConId before,Open it
 *        if get SoConId from SoAd not equal before,close before one,then open the new one
 * Param-Name[in]: consumedEventGroupPtr, remoteAddrPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: soConPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_ClientSaveMulticastSoConId(
    const Sd_InstanceType* instancePtr,
    const Sd_ConsumedEventGroupType* consumedEventGroupPtr,
    const TcpIp_SockAddrType* multicastAddrPtr,
    const TcpIp_SockAddrType* serverAddrPtr,
    Sd_SoConManageType* soConPtr)
{
    const Sd_SoAdSoConGroupType* ceMulticastGroupRef;
    SoAd_RoutingGroupIdType routingGroupId;
    SoAd_SoConIdType soConId;
    SoAd_SoConIdType txPduSoConId;
    TcpIp_SockAddrType localAddr;
    TcpIp_SockAddrType defaultRouter;
    uint8 netmask;
    Std_ReturnType ret = E_NOT_OK;
    Std_ReturnType retGetLocalAddr;
#if (STD_OFF == SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON)
    uint16 index;
    uint16 soConNumInGroup;
    TcpIp_SockAddrType remoteAddr;
#endif /* STD_OFF == SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON */
    /* @req SWS_SD_00377 */
    ceMulticastGroupRef = consumedEventGroupPtr->SdConsumedEventGroupMulticastGroupRef;
    if (NULL_PTR != ceMulticastGroupRef)
    {
        soConId = ceMulticastGroupRef->SdSocketIdList[0];
#if (STD_ON == SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON)
        ret = SoAd_SetUniqueRemoteAddr(soConId, serverAddrPtr, &soConId);
#else
        remoteAddr.domain = instancePtr->SdAddrFamily;
        soConNumInGroup = ceMulticastGroupRef->SdSoConNumInGroup;
        for (index = 0u; index < soConNumInGroup; index++)
        {
            soConId = ceMulticastGroupRef->SdSocketIdList[index];
            ret = SoAd_GetRemoteAddr(soConId, &remoteAddr);
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
        if ((FALSE == soConPtr->isSoConOpened) || (soConId != soConPtr->soConId))
        {
            Sd_ClientServiceCloseSoCon(soConPtr);

            /*@req <SWS_SD_00604> */
            ret = SoAd_OpenSoCon(soConId);
            if (E_OK == ret)
            {
                soConPtr->isOptRcved = TRUE;
                soConPtr->isSoConOpened = TRUE;
                soConPtr->soConId = soConId;

                /*@req <SWS_SD_00377> */
                localAddr.domain = instancePtr->SdAddrFamily;
                defaultRouter.domain = instancePtr->SdAddrFamily;
                txPduSoConId = instancePtr->SdInstanceTxPduSoConId;
                retGetLocalAddr = SoAd_GetLocalAddr(txPduSoConId, &localAddr, &netmask, &defaultRouter);
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
                    routingGroupId =
                        consumedEventGroupPtr->SdConsumedEventGroupMulticastActivationRef->SdRoutingGroupId;
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
            Dem_ReportErrorStatus(demEventPtr->EventId, DEM_EVENT_STATUS_FAILED);
        }
#endif
    }
}
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Handle Client socket connection based on option information
 * Param-Name[in]: instancePtr,clientServicePtr,sdMsg,entryPos,optStartPosMap
 * Param-Name[out]: None
 * Param-Name[in/out]: cRtDataPtr
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_ClientHandleSoConBasedOptionInfo(
    const Sd_InstanceType* instancePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap,
    Sd_ClientServiceRTType* cRtDataPtr)
{
    Sd_Type1EntryType type1Entry;
    Sd_IPv4OptionsType ipv4Opt;
#if (STD_ON == SD_IPV6_ENABLE)
    Sd_IPv6OptionsType ipv6Opt;
#endif
    TcpIp_SockAddrType remoteAddr;
    uint8 idx1stOptions, numOpt1;
    uint8 idx2ndOptions, numOpt2;
    uint8 index;
    uint8 optType;
    uint8 optIndex;
    uint8 optNumSum;

    (void)ILib_memcpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    idx1stOptions = type1Entry.Index1stOptions;
    numOpt1 = type1Entry.NumOfOpt1;
    idx2ndOptions = type1Entry.Index2ndOptions;
    numOpt2 = type1Entry.NumOfOpt2;

    optNumSum = numOpt1 + numOpt2;
    for (index = 0; index < optNumSum; index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        optType = sdMsg[optStartPosMap[optIndex] + 2u];
        if (TCPIP_AF_INET == instancePtr->SdAddrFamily)
        {
            if (SD_IPV4_ENDPOINT_OPTION == optType)
            {
                (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
                if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && (SD_L4_PROTO_TCP == ipv4Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET;
                    remoteAddr.port = Sd_NtoHs(ipv4Opt.PortNumber);
                    (void)ILib_memcpy((uint8*)remoteAddr.addr, (const uint8*)ipv4Opt.IPv4Address, 4u);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        TRUE,
                        &cRtDataPtr->tcpSoCon);

                    cRtDataPtr->rxTcpEndpointFlag = TRUE;
                }
                else if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && (SD_L4_PROTO_UDP == ipv4Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET;
                    remoteAddr.port = Sd_NtoHs(ipv4Opt.PortNumber);
                    (void)ILib_memcpy((uint8*)remoteAddr.addr, (const uint8*)ipv4Opt.IPv4Address, 4);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        FALSE,
                        &cRtDataPtr->udpSoCon);
                    /* Save server address for consumedEventgroup receive multicast message */
                    (void)ILib_memcpy(&cRtDataPtr->serverUdpAddr, &remoteAddr, sizeof(remoteAddr));

                    cRtDataPtr->rxUdpEndpointFlag = TRUE;
                }
                else
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
                (void)ILib_memcpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
                if ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && (SD_L4_PROTO_TCP == ipv6Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET6;
                    remoteAddr.port = Sd_NtoHs(ipv6Opt.PortNumber);
                    (void)ILib_memcpy((uint8*)remoteAddr.addr, (const uint8*)ipv6Opt.IPv6Address, 16u);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        TRUE,
                        &cRtDataPtr->tcpSoCon);

                    cRtDataPtr->rxTcpEndpointFlag = TRUE;
                }
                else if ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && (SD_L4_PROTO_UDP == ipv6Opt.L4Proto))
                {
                    remoteAddr.domain = TCPIP_AF_INET6;
                    remoteAddr.port = Sd_NtoHs(ipv6Opt.PortNumber);
                    (void)ILib_memcpy((uint8*)remoteAddr.addr, (const uint8*)ipv6Opt.IPv6Address, 16);

                    Sd_ClientSaveSoConId(
                        clientServicePtr,
                        (const TcpIp_SockAddrType*)&remoteAddr,
                        FALSE,
                        &cRtDataPtr->udpSoCon);
                    /* Save server address for consumedEventgroup receive multicast message */
                    (void)ILib_memcpy(&cRtDataPtr->serverUdpAddr, &remoteAddr, sizeof(remoteAddr));

                    cRtDataPtr->rxUdpEndpointFlag = TRUE;
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

/******************************************************************************/
/*
 * Brief: Manage client Timer
 * Param-Name[in]: clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_ClientTimerHandle(const Sd_ClientServiceType* clientServicePtr)
{
    Sd_ClientServiceRTType* cRtDataPtr;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    const Sd_ConsumedEventGroupType* eventGroupPtr;
    Sd_ConsumedEventGroupRTType* consumedEgRTPtr;
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
    const Sd_ClientTimerType* consumedEgTimerPtr;
#endif
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
    uint16 eventGroupIdx;

    cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];
    if (cRtDataPtr->ttl.ttlms > 0u)
    {
        cRtDataPtr->ttl.ttlms--;
        if (0u == cRtDataPtr->ttl.ttlms)
        {
            /*@req <SWS_SD_00600> */
            /* Enter the Initial Wait Phase and indicate the state SD_CLIENT_SERVICE_DOWN
             * to the BswM by calling the API BswM_Sd_ClientServiceCurrentState().*/
            Sd_ClientServiceInitialWaitEnter(clientServicePtr, cRtDataPtr);

#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_ClientServiceCurrentState(clientServicePtr->SdClientServiceHandleId, SD_CLIENT_SERVICE_DOWN);
#endif
            cRtDataPtr->curState = SD_CLIENT_SERVICE_DOWN;

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
            /* All subscribed Eventgroups of this Client Service shall expired in this instance
             * (stop TTL timer) and the expiration shall be handled as describe in SWS_SD_00601.*/
            eventGroupPtr = &clientServicePtr->SdConsumedEventGroup[0];
            for (eventGroupIdx = 0u; eventGroupIdx < clientServicePtr->SdConsumedEventGroupNum; eventGroupIdx++)
            {
                consumedEgRTPtr = &Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
                if ((TRUE == consumedEgRTPtr->ttl.isValidForever) || (consumedEgRTPtr->ttl.ttlms > 0u))
                {
                    consumedEgRTPtr->ttl.ttlms = 0u;
                    consumedEgRTPtr->ttl.isValidForever = FALSE;
/*@req <SWS_SD_00601> */
#if (SD_BSWM_USED == STD_ON)
                    BswM_Sd_ConsumedEventGroupCurrentState(
                        eventGroupPtr->SdConsumedEventGroupHandleId,
                        SD_CONSUMED_EVENTGROUP_DOWN);
#endif
                    consumedEgRTPtr->curState = SD_CONSUMED_EVENTGROUP_DOWN;
                }

                eventGroupPtr++;
            }
#endif
            /* When left MainPhase,Close all Routing and SoCon opened before */
            Sd_ClientServiceDisableAllRoutingOfClient(clientServicePtr, cRtDataPtr);
            Sd_ClientServiceCloseAllSoCon(clientServicePtr, cRtDataPtr);
        }
    }

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    if ((TRUE == cRtDataPtr->ttl.isValidForever) || (cRtDataPtr->ttl.ttlms > 0u))
    {
        eventGroupPtr = &clientServicePtr->SdConsumedEventGroup[0];
        for (eventGroupIdx = 0u; eventGroupIdx < clientServicePtr->SdConsumedEventGroupNum; eventGroupIdx++)
        {
            consumedEgRTPtr = &Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
            if (consumedEgRTPtr->ttl.ttlms > 0u)
            {
                consumedEgRTPtr->ttl.ttlms--;
                if (0u == consumedEgRTPtr->ttl.ttlms)
                {
/*@req <SWS_SD_00601> */
#if (SD_BSWM_USED == STD_ON)
                    BswM_Sd_ConsumedEventGroupCurrentState(
                        eventGroupPtr->SdConsumedEventGroupHandleId,
                        SD_CONSUMED_EVENTGROUP_DOWN);
#endif
                    consumedEgRTPtr->curState = SD_CONSUMED_EVENTGROUP_DOWN;

                    /* Close Routing of this ConsumedEventgroup */
                    Sd_ClientServiceDisableAllConsumedEventgroupRouting(eventGroupPtr, cRtDataPtr);
                }
            }

/* @req SWS_SD_00735,SWS_SD_00737,SWS_SD_00738 */
#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
            consumedEgTimerPtr = eventGroupPtr->SdConsumedEventGroupTimerRef;
            if (consumedEgTimerPtr->SdSubscribeEventgroupRetryMax > 0u)
            {
                if (consumedEgRTPtr->subsRetryTimer > 0u)
                {
                    consumedEgRTPtr->subsRetryTimer--;
                    if (0u == consumedEgRTPtr->subsRetryTimer)
                    {
                        consumedEgRTPtr->subsRetryCnt++;
                        /* If the counts of retries of subscription exceeds, report runtime error
                         * "SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED".*/
                        if (consumedEgRTPtr->subsRetryCnt > consumedEgTimerPtr->SdSubscribeEventgroupRetryMax)
                        {
                            consumedEgRTPtr->subsRetryEnable = FALSE;
                            SD_REPORT_RUNTIMEERROR(SD_MAIN_FUNCTION_ID, SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED);
                        }
                    }
                }
            }
#endif
            eventGroupPtr++;
        }
    }
#endif
}

/******************************************************************************/
/*
 * Brief: Initial wait phase left handle
 * Param-Name[in]: cRtDataPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_ClientInitialWaitPhaseLeftHandle(Sd_ClientServiceRTType* cRtDataPtr)
{
    /*@req <SWS_SD_00456> */
    /* If for any reasons the Initial Wait Phase is left, the calculated random timer (of the
     * Initial Wait Phase) for this Service Instance shall be stopped. */
    cRtDataPtr->initialWaitTimer = 0u;
}
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#if ((SD_SERVER_SERVICE_NUM > 0) || (SD_CLIENT_SERVICE_NUM > 0))
/******************************************************************************/
/*
 * Brief: Append SendQueue item to SendQueue tail
 * Param-Name[in]: rxBuffItemPtr
 * Param-Name[out]:
 * Param-Name[in/out]:
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_AppendToSendQueueTail(Sd_SendQueueType** head, Sd_SendQueueType* sendQueueItem)
{
    Sd_SendQueueType* tail;

    if (NULL_PTR != *head)
    {
        tail = *head;
        while (NULL_PTR != tail->next)
        {
            tail = tail->next;
        }
        tail->next = sendQueueItem;
    }
    else
    {
        *head = sendQueueItem;
    }
}

/******************************************************************************/
/*
 * Brief: Clear SendQueue
 * Param-Name[in]:
 * Param-Name[out]:
 * Param-Name[in/out]:head
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_ClearSendQueue(Sd_SendQueueType** head)
{
    Sd_SendQueueType* curPtr = *head;
    Sd_SendQueueType* nextPtr = NULL_PTR;

    while (NULL_PTR != curPtr)
    {
        nextPtr = curPtr->next;
        Sd_FreeEx(curPtr->entryPtr, Sd_TxMemPool);
        Sd_FreeEx(curPtr->optionPtr, Sd_TxMemPool);
        Sd_FreeEx(curPtr, Sd_TxMemPool);
        curPtr = nextPtr;
    }
    *head = NULL_PTR;
}

/******************************************************************************/
/*
 * Brief: Calculate number of TCP/UDP Endpoint Option number
 * Param-Name[in]: sdMsg,entryPos,optStartPosMap,tcpEndpointNum,udpEndpointNum
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_CalcTcpUdpEndpointOptionNum(
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap,
    uint8* tcpEndpointNum,
    uint8* udpEndpointNum)
{
    Sd_Type1EntryType type1Entry;
    Sd_IPv4OptionsType ipv4Opt;
#if (STD_ON == SD_IPV6_ENABLE)
    Sd_IPv6OptionsType ipv6Opt;
#endif /* STD_ON == SD_IPV6_ENABLE */
    uint8 idx1stOptions, numOpt1;
    uint8 idx2ndOptions, numOpt2;
    uint8 index;
    uint8 optType;
    uint8 optIndex;
    uint8 ipv4EndpTcpNum = 0, ipv4EndpUdpNum = 0u;
    uint8 ipv6EndpTcpNum = 0, ipv6EndpUdpNum = 0u;
    uint8 optNumSum;

    (void)ILib_memcpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
    idx1stOptions = type1Entry.Index1stOptions;
    numOpt1 = type1Entry.NumOfOpt1;
    idx2ndOptions = type1Entry.Index2ndOptions;
    numOpt2 = type1Entry.NumOfOpt2;

    /* Calculate all kinds options number */
    optNumSum = numOpt1 + numOpt2;
    for (index = 0; index < optNumSum; index++)
    {
        if (index < numOpt1)
        {
            optIndex = idx1stOptions + index;
        }
        else
        {
            optIndex = idx2ndOptions + index - numOpt1;
        }

        optType = sdMsg[optStartPosMap[optIndex] + 2u];
        if (SD_IPV4_ENDPOINT_OPTION == optType)
        {
            (void)ILib_memcpy((uint8*)&ipv4Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv4OptionsType));
            if (SD_L4_PROTO_TCP == ipv4Opt.L4Proto)
            {
                ipv4EndpTcpNum++;
            }
            else if (SD_L4_PROTO_UDP == ipv4Opt.L4Proto)
            {
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
            (void)ILib_memcpy((uint8*)&ipv6Opt, &sdMsg[optStartPosMap[optIndex]], sizeof(Sd_IPv6OptionsType));
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

    *tcpEndpointNum = (ipv4EndpTcpNum + ipv6EndpTcpNum);
    *udpEndpointNum = (ipv4EndpUdpNum + ipv6EndpUdpNum);
}

/******************************************************************************/
/*
 * Brief: Check if necessary option exist
 *        [serverServicePtr] or [clientServicePtr] must have one equal NULL_PTR
 * Param-Name[in]: instancePtr,serverServicePtr,clientServicePtr,sdMsg,entryPos,optStartPosMap
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK-No Error,E_NOT_OK-Error,need response Nack
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_NecessaryOptionsExistCheck(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const uint8* sdMsg,
    uint16 entryPos,
    const uint16* optStartPosMap)
{
    Std_ReturnType ret = E_OK;
    uint8 tcpEndpointNum;
    uint8 udpEndpointNum;

    Sd_CalcTcpUdpEndpointOptionNum(sdMsg, entryPos, optStartPosMap, &tcpEndpointNum, &udpEndpointNum);

    /*@req <SWS_SD_00710> */
    /* Check if necessary option exist */
    if (((NULL_PTR != serverServicePtr)
         && !(
             ((NULL_PTR != serverServicePtr->SdServerServiceTcpRef) && (tcpEndpointNum != 0u))
             || ((NULL_PTR != serverServicePtr->SdServerServiceUdpRef) && (udpEndpointNum != 0u))))
        || ((NULL_PTR != clientServicePtr)
            && !(
                ((NULL_PTR != clientServicePtr->SdClientServiceTcpRef) && (tcpEndpointNum != 0u))
                || ((NULL_PTR != clientServicePtr->SdClientServiceUdpRef) && (udpEndpointNum != 0u)))))
    {
        ret = E_NOT_OK;
    }

    SD_UNUSED(instancePtr);
    return ret;
}

/******************************************************************************/
/*
 * Brief: Package entries and options to one message
 * Param-Name[in]: sendQueue,sendQueCnt,instanceId,isMulticastTx
 * Param-Name[out]: pduInfoPtr
 * Param-Name[in/out]: None
 * Return: E_OK:Find Multicast option and get info, E_NOT_OK:Get info failure
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_PackageMessage(
    /* PRQA S 3677 ++ */ /* MISRA Rule 8.7 */
    Sd_SendQueueType* sendQueue[],
    /* PRQA S 3677 -- */ /* MISRA Rule 8.7 */
    uint8 sendQueCnt,
    PduInfoType* pduInfoPtr,
    uint16 instanceId,
    boolean isMulticastTx)
{
    uint8* messagePtr;
    uint8 *entryRunPtr, *optionRunPtr;
    Sd_SendQueueType* sendQuePtr;
    Sd_Type1EntryType type1Entry;
    TcpIp_SockAddrType remoteAddr;
    uint32 tLengthField;
    uint16 entryTotalLen;
    uint16 optionTotalLen = 0u;
    uint16 messageLength;
    uint8 index;
    uint8 optIdxCnt;
    Std_ReturnType ret = E_NOT_OK;

    /* Calculate message length */
    entryTotalLen = (uint16)sendQueCnt * 16u;
    for (index = 0u; index < sendQueCnt; index++)
    {
        optionTotalLen += sendQueue[index]->optionLen;
    }
    /* Message total length = header length(12) + entry length field(4) + entries length
     * + option length field(4) + options length */
    messageLength = 20u + entryTotalLen + optionTotalLen;

    messagePtr = (uint8*)Sd_MallocEx(messageLength, Sd_TxMemPool);
    if (NULL_PTR != messagePtr)
    {
        /* Clear allocate memory */
        (void)ILib_memset(messagePtr, 0x00, messageLength);

        /* Package entries and options to one buffer */
        /* Header */
        remoteAddr = sendQueue[0u]->destAddr;
        Sd_SetHeader(messagePtr, instanceId, &remoteAddr, isMulticastTx);
        /* Length of Entries Array in Bytes,Offset: 12(HeaderLength) */
        tLengthField = Sd_HtoNl(entryTotalLen);
        (void)ILib_memcpy(&messagePtr[12u], (uint8*)&tLengthField, sizeof(uint32));
        /* Length of Options Array in Bytes,Offset: 16 = 12(HeaderLength) + 4(Length of Entries Array) */
        tLengthField = Sd_HtoNl(optionTotalLen);
        (void)ILib_memcpy(&messagePtr[16u + entryTotalLen], (uint8*)&tLengthField, sizeof(uint32));
        /* Entyies & Options */
        /* First entry offset: 16 = 12(HeaderLength) + 4(Length of Entries Array) */
        entryRunPtr = &messagePtr[16u];
        /* First entry offset: header length(12) + entry length field(4) + entries length + option length field(4) */
        optionRunPtr = &messagePtr[20u + entryTotalLen];
        optIdxCnt = 0u;
        for (index = 0u; index < sendQueCnt; index++)
        {
            sendQuePtr = sendQueue[index];
            /* Entyies */
            (void)ILib_memcpy((uint8*)&type1Entry, sendQuePtr->entryPtr, sizeof(Sd_Type1EntryType));
            type1Entry.NumOfOpt1 = sendQuePtr->optionNum;
            type1Entry.Index1stOptions = optIdxCnt;
            (void)ILib_memcpy(entryRunPtr, (uint8*)&type1Entry, sizeof(Sd_Type1EntryType));
            optIdxCnt += sendQuePtr->optionNum;
            entryRunPtr = &entryRunPtr[16];
            Sd_FreeEx(sendQuePtr->entryPtr, Sd_TxMemPool);

            /* Options */
            (void)ILib_memcpy(optionRunPtr, sendQuePtr->optionPtr, sendQuePtr->optionLen);
            optionRunPtr = &optionRunPtr[sendQuePtr->optionLen];
            Sd_FreeEx(sendQuePtr->optionPtr, Sd_TxMemPool);

            Sd_FreeEx(sendQuePtr, Sd_TxMemPool);
        }

        pduInfoPtr->SduDataPtr = messagePtr;
        pduInfoPtr->SduLength = messageLength;
        ret = E_OK;
    }
    else
    {
        for (index = 0u; index < sendQueCnt; index++)
        {
            sendQuePtr = sendQueue[index];
            /* Free buffer */
            Sd_FreeEx(sendQuePtr->entryPtr, Sd_TxMemPool);
            Sd_FreeEx(sendQuePtr->optionPtr, Sd_TxMemPool);
            Sd_FreeEx(sendQuePtr, Sd_TxMemPool);
        }
    }

    return ret;
}

/******************************************************************************/
/*
 * Brief: Transmit multicast message
 * Notice:sRtDataPtr and cRtDataPtr must have one equal NULL_PTR
 * Param-Name[in]: instancePtr,multiSendQue,multiSendCnt,sRtDataPtr,cRtDataPtr
 * Param-Name[out]: ipAddrPtr
 * Param-Name[in/out]: None
 * Return: E_OK:Find Multicast option and get info, E_NOT_OK:Get info failure
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_TransmitMulticastMessage(
    const Sd_InstanceType* instancePtr,
    Sd_SendQueueType* multiSendQue[],
    uint8 multiSendCnt)
{
    const Sd_RxPduIdSoConIdMapType* rxPduIdSoConIdMapPtr;
    TcpIp_SockAddrType localAddr, defaultRouter;
    PduInfoType pduInfo;
    uint8 index;
    uint8 netmask;
    Std_ReturnType ret;
    Std_ReturnType retGetLocalAddr;
    SoAd_SoConIdType soConId;

    ret = Sd_PackageMessage(multiSendQue, multiSendCnt, &pduInfo, instancePtr->SdInstanceHandleId, TRUE);
    /* Transmit Message */
    if (E_OK == ret)
    {
        /* Get remote address */
        rxPduIdSoConIdMapPtr = Sd_CfgPtr->SdGlbRxPduIdSoConIdMap;
        for (index = 0u; index < SD_RXPDUID_SOCONID_MAP_LEN; index++)
        {
            if (rxPduIdSoConIdMapPtr->SdRxPduId == instancePtr->SdInstanceMulticastRxPdu)
            {
                localAddr.domain = instancePtr->SdAddrFamily;
                defaultRouter.domain = instancePtr->SdAddrFamily;
                retGetLocalAddr =
                    SoAd_GetLocalAddr(rxPduIdSoConIdMapPtr->SoConId, &localAddr, &netmask, &defaultRouter);
                soConId = instancePtr->SdInstanceTxPduSoConId;
                if (E_OK == retGetLocalAddr)
                {
                    SchM_Enter_Sd_ExclusiveArea();
                    (void)SoAd_SetRemoteAddr(soConId, &localAddr);
                    (void)SoAd_IfTransmit(instancePtr->SdInstanceTxPduRef, &pduInfo);
                    SoAd_ReleaseRemoteAddr(soConId);
                    SchM_Exit_Sd_ExclusiveArea();
                }
            }
            rxPduIdSoConIdMapPtr++;
        }

        /* Free message used buffer */
        Sd_FreeEx(pduInfo.SduDataPtr, Sd_TxMemPool);
    }
}

/******************************************************************************/
/*
 * Brief: Transmit unicast message
 * Param-Name[in]: instancePtr, uniSendQue, uniSendCnt
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_TransmitUnicastMessage(
    const Sd_InstanceType* instancePtr,
    Sd_SendQueueType* uniSendQue[],
    uint8 uniSendCnt)
{
    uint8 numPerAddrKind[SD_MAX_ENTRIES_PER_FRAME];
    uint8 numAllKind;
    Sd_SendQueueType* sendQue[SD_MAX_ENTRIES_PER_FRAME];
    /* PRQA S 0686 ++ */ /* MISRA Rule 9.3 */
    uint8 sendQueEleList[SD_MAX_ENTRIES_PER_FRAME][SD_MAX_ENTRIES_PER_FRAME] = {{0}};
    /* PRQA S 0686 -- */ /* MISRA Rule 9.3 */
    PduInfoType pduInfo;
    SoAd_SoConIdType soConId;
    TcpIp_SockAddrType destAddr;
    sint32 isSame;
    uint8 i, j;
    Std_ReturnType ret;

    (void)ILib_memset(numPerAddrKind, 0x00, SD_MAX_ENTRIES_PER_FRAME);
    numPerAddrKind[0u] = 1u;
    sendQueEleList[0u][0u] = 0u;
    numAllKind = 1u;

    for (i = 1u; i < uniSendCnt; i++)
    {
        for (j = 0u; j < numAllKind; j++)
        {
            isSame = ILib_memcmp(
                (uint8*)&uniSendQue[i]->destAddr,
                (uint8*)&uniSendQue[sendQueEleList[numAllKind][0u]]->destAddr,
                sizeof(uniSendQue[i]->destAddr));
            if (0 == isSame)
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
            numPerAddrKind[numAllKind] = 1u;
            numAllKind++;
        }
    }

    for (i = 0u; i < numAllKind; i++)
    {
        for (j = 0u; j < numPerAddrKind[i]; j++)
        {
            sendQue[j] = uniSendQue[sendQueEleList[i][j]];
        }

        destAddr = sendQue[0]->destAddr;
        ret = Sd_PackageMessage(sendQue, numPerAddrKind[i], &pduInfo, instancePtr->SdInstanceHandleId, FALSE);
        if (E_OK == ret)
        {
            soConId = instancePtr->SdInstanceTxPduSoConId;
            SchM_Enter_Sd_ExclusiveArea();
            (void)SoAd_SetRemoteAddr(soConId, &destAddr);
            (void)SoAd_IfTransmit(instancePtr->SdInstanceTxPduRef, &pduInfo);
            SoAd_ReleaseRemoteAddr(soConId);
            SchM_Exit_Sd_ExclusiveArea();

            /* Free message used buffer */
            Sd_FreeEx(pduInfo.SduDataPtr, Sd_TxMemPool);
        }
    }
}

/******************************************************************************/
/*
 * Brief: Find out Multicast entry that ttl = 0 to send
 * Param-Name[in]: queueHeaderPtr, multiSendQue, multiSendCntPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_FindMulticastMsgToSendFromSendQueue(
    Sd_SendQueueType** queueHeaderPtr,
    Sd_SendQueueType* multiSendQue[],
    uint8* multiSendCntPtr)
{
    Sd_SendQueueType *lastPtr, *nowPtr;
    boolean deleteFromQue;
    uint8 multiSendCnt = *multiSendCntPtr;

    /* Find out need to be send obj form a send queue */
    nowPtr = *queueHeaderPtr;
    lastPtr = NULL_PTR;
    while ((NULL_PTR != nowPtr) && (multiSendCnt < SD_MAX_ENTRIES_PER_FRAME))
    {
        deleteFromQue = FALSE;
        if ((FALSE == nowPtr->isUnicastSend) && (nowPtr->sendTime > 0u))
        {
            nowPtr->sendTime--;
            if (0u == nowPtr->sendTime)
            {
                multiSendQue[multiSendCnt] = nowPtr;
                multiSendCnt++;
                deleteFromQue = TRUE;

                if (NULL_PTR == lastPtr)
                {
                    *queueHeaderPtr = nowPtr->next;
                }
                else
                {
                    lastPtr->next = nowPtr->next;
                }
                nowPtr = nowPtr->next;
            }
        }

        if (FALSE == deleteFromQue)
        {
            lastPtr = nowPtr;
            nowPtr = nowPtr->next;
        }
    }

    *multiSendCntPtr = multiSendCnt;
}

/******************************************************************************/
/*
 * Brief: Find out Unicast entry that ttl = 0 to send
 * Param-Name[in]: queueHeaderPtr, uniSendQue, uniSendCntPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_FindUnicastMsgToSendFromSendQueue(
    Sd_SendQueueType** queueHeaderPtr,
    Sd_SendQueueType* uniSendQue[],
    uint8* uniSendCntPtr)
{
    Sd_SendQueueType *lastPtr, *nowPtr;
    boolean deleteFromQue;
    uint8 uniSendCnt = *uniSendCntPtr;

    /* Find out need to be send obj form a send queue */
    nowPtr = *queueHeaderPtr;
    lastPtr = NULL_PTR;
    while ((NULL_PTR != nowPtr) && (uniSendCnt < SD_MAX_ENTRIES_PER_FRAME))
    {
        deleteFromQue = FALSE;
        if ((TRUE == nowPtr->isUnicastSend) && (nowPtr->sendTime > 0u))
        {
            nowPtr->sendTime--;
            if (0u == nowPtr->sendTime)
            {
                uniSendQue[uniSendCnt] = nowPtr;
                uniSendCnt++;
                deleteFromQue = TRUE;

                if (NULL_PTR == lastPtr)
                {
                    *queueHeaderPtr = nowPtr->next;
                }
                else
                {
                    lastPtr->next = nowPtr->next;
                }
                nowPtr = nowPtr->next;
            }
        }

        if (FALSE == deleteFromQue)
        {
            lastPtr = nowPtr;
            nowPtr = nowPtr->next;
        }
    }

    *uniSendCntPtr = uniSendCnt;
}

/******************************************************************************/
/*
 * Brief: Build multicast message for transmit
 * Param-Name[in]: instanceId, multiSendQue
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint8 Sd_FindoutMulticastMessage(const Sd_InstanceType* instancePtr, Sd_SendQueueType* multiSendQue[])
{
    uint16 idx;
#if (SD_SERVER_SERVICE_NUM > 0)
    const Sd_ServerServiceType* serverServicePtr;
    Sd_ServerServiceRTType* sRtDataPtr;
#endif /* SD_SERVER_SERVICE_NUM > 0 */
    uint8 multiSendCnt = 0u;
#if (SD_CLIENT_SERVICE_NUM > 0)
    const Sd_ClientServiceType* clientServicePtr;
    Sd_ClientServiceRTType* cRtDataPtr;
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_SERVER_SERVICE_NUM > 0)
    serverServicePtr = instancePtr->SdServerService;
    if (NULL_PTR != serverServicePtr)
    {
        for (idx = 0u; idx < instancePtr->SdServerServiceNum; idx++)
        {
            sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];

            Sd_FindMulticastMsgToSendFromSendQueue(&sRtDataPtr->head, multiSendQue, &multiSendCnt);

            serverServicePtr++;
        }
    }
#endif /* SD_SERVER_SERVICE_NUM > 0 */

#if (SD_CLIENT_SERVICE_NUM > 0)
    clientServicePtr = instancePtr->SdClientService;
    if (NULL_PTR != clientServicePtr)
    {
        for (idx = 0u; idx < instancePtr->SdClientServiceNum; idx++)
        {
            cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];

            Sd_FindMulticastMsgToSendFromSendQueue(&cRtDataPtr->head, multiSendQue, &multiSendCnt);

            clientServicePtr++;
        }
    }
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

    return multiSendCnt;
}

/******************************************************************************/
/*
 * Brief: Build unicast message for transmit
 * Param-Name[in]: instanceId, multiSendQue
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static uint8 Sd_FindoutUnicastMessage(const Sd_InstanceType* instancePtr, Sd_SendQueueType* uniSendQue[])
{
    uint16 idx;
#if (SD_SERVER_SERVICE_NUM > 0)
    const Sd_ServerServiceType* serverServicePtr;
    Sd_ServerServiceRTType* sRtDataPtr;
#endif /* SD_SERVER_SERVICE_NUM > 0 */
    uint8 uniSendCnt = 0u;
#if (SD_CLIENT_SERVICE_NUM > 0)
    const Sd_ClientServiceType* clientServicePtr;
    Sd_ClientServiceRTType* cRtDataPtr;
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_SERVER_SERVICE_NUM > 0)
    serverServicePtr = instancePtr->SdServerService;
    if (NULL_PTR != serverServicePtr)
    {
        for (idx = 0u; idx < instancePtr->SdServerServiceNum; idx++)
        {
            sRtDataPtr = &Sd_ServerRTData[serverServicePtr->SdServerServiceHandleId];

            Sd_FindUnicastMsgToSendFromSendQueue(&sRtDataPtr->head, uniSendQue, &uniSendCnt);

            serverServicePtr++;
        }
    }
#endif /* SD_SERVER_SERVICE_NUM > 0 */

#if (SD_CLIENT_SERVICE_NUM > 0)
    clientServicePtr = instancePtr->SdClientService;
    if (NULL_PTR != clientServicePtr)
    {
        for (idx = 0u; idx < instancePtr->SdClientServiceNum; idx++)
        {
            cRtDataPtr = &Sd_ClientRTData[clientServicePtr->SdClientServiceHandleId];

            Sd_FindUnicastMsgToSendFromSendQueue(&cRtDataPtr->head, uniSendQue, &uniSendCnt);

            clientServicePtr++;
        }
    }
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

    return uniSendCnt;
}

/******************************************************************************/
/*
 * Brief: Transmit message
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_TransmitMessage(void)
{
    uint16 instanceIdx;
    const Sd_InstanceType* instancePtr;
    Sd_SendQueueType* txMsgSendQue[SD_MAX_ENTRIES_PER_FRAME]; /* temp save need send item */
    uint8 multiSendCnt, uniSendCnt;

    for (instanceIdx = 0u; instanceIdx < SD_INSTANCE_NUM; instanceIdx++) /* PRQA S 2877 */ /* MISRA Dir 4.1 */
    {
        instancePtr = &Sd_CfgPtr->SdInstance[instanceIdx];
        /* Due to the limitations of SoAd module, it is not possible to send both unicast
         * and multicast message at the same time. Prioritize sending unicast messages */
        uniSendCnt = Sd_FindoutUnicastMessage(instancePtr, txMsgSendQue);
        if (uniSendCnt > 0u)
        {
            Sd_TransmitUnicastMessage(instancePtr, txMsgSendQue, uniSendCnt);
        }

        multiSendCnt = Sd_FindoutMulticastMessage(instancePtr, txMsgSendQue);
        if (multiSendCnt > 0u)
        {
            Sd_TransmitMulticastMessage(instancePtr, txMsgSendQue, multiSendCnt);
        }
    }
}
#endif /* SD_SERVER_SERVICE_NUM > 0 || SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u)
/******************************************************************************/
/*
 * Brief: Check if received Configuration Option match with configured
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static boolean Sd_RxConfigurationOptionMatchCheck(
    const Sd_InstanceType* instancePtr,
    const Sd_ServerServiceType* serverServicePtr,
    const Sd_ClientServiceType* clientServicePtr,
    const Sd_RxQueueType* rxQueueItem,
    uint16 entryPos,
    const uint16* optStartPosMap)
{
    const uint8* sdMsg = rxQueueItem->bufPtr;
    Sd_Type1EntryType type1Entry;
    uint8* createCfgOptPtr;
    uint16 cfgOptLen;
    uint16 matchCalloutIdx;
    uint8 idx1stOptions, numOpt1;
    uint8 idx2ndOptions, numOpt2;
    uint8 index, optIndex;
    uint8 optType;
    boolean ret = TRUE;
    uint8 optNumSum;

    if (((NULL_PTR != serverServicePtr) && (serverServicePtr->SdServerCapabilityRecordMatchCalloutRef != 0xFFFFu))
        || ((NULL_PTR != clientServicePtr) && (clientServicePtr->SdClientCapabilityRecordMatchCalloutRef != 0xFFFFu)))
    {
        (void)ILib_memcpy((uint8*)&type1Entry, &sdMsg[entryPos], sizeof(Sd_Type1EntryType));
        idx1stOptions = type1Entry.Index1stOptions;
        numOpt1 = type1Entry.NumOfOpt1;
        idx2ndOptions = type1Entry.Index2ndOptions;
        numOpt2 = type1Entry.NumOfOpt2;

        optNumSum = numOpt1 + numOpt2;
        for (index = 0; index < optNumSum; index++)
        {
            if (index < numOpt1)
            {
                optIndex = idx1stOptions + index;
            }
            else
            {
                optIndex = idx2ndOptions + index - numOpt1;
            }

            optType = sdMsg[optStartPosMap[optIndex] + 2u];
            if (SD_CONFIGURATION_OPTION == optType)
            {
                cfgOptLen = Sd_GetType1ConfigurationOptionLength(instancePtr, serverServicePtr, clientServicePtr);
                if (NULL_PTR != serverServicePtr)
                {
                    if (cfgOptLen > 0u)
                    {
                        createCfgOptPtr = Sd_MallocEx(cfgOptLen, Sd_RxMemPool);
                        if (NULL_PTR != createCfgOptPtr)
                        {
                            Sd_BuildType1ConfigurationOption(
                                createCfgOptPtr,
                                cfgOptLen,
                                instancePtr,
                                serverServicePtr,
                                clientServicePtr);

                            /* @req SWS_SD_00716 */
                            matchCalloutIdx = serverServicePtr->SdServerCapabilityRecordMatchCalloutRef;
                            ret = Sd_CfgPtr->SdCapabilityRecordMatchCalloutRef[matchCalloutIdx](
                                rxQueueItem->rxPduId,
                                type1Entry.Type,
                                type1Entry.ServiceID,
                                type1Entry.InstanceID,
                                type1Entry.MajorVersion,
                                type1Entry.MinorVersion,
                                (const Sd_ConfigOptionStringType*)&sdMsg[optStartPosMap[optIndex] + 4u],
                                (const Sd_ConfigOptionStringType*)&createCfgOptPtr[4u]);
                            Sd_FreeEx(createCfgOptPtr, Sd_RxMemPool);
                            break;
                        }
                    }
                }
                else
                {
                    if (cfgOptLen > 0u)
                    {
                        createCfgOptPtr = Sd_MallocEx(cfgOptLen, Sd_RxMemPool);
                        if (NULL_PTR != createCfgOptPtr)
                        {
                            Sd_BuildType1ConfigurationOption(
                                createCfgOptPtr,
                                cfgOptLen,
                                instancePtr,
                                serverServicePtr,
                                clientServicePtr);

                            /* @req SWS_SD_00716 */
                            matchCalloutIdx = clientServicePtr->SdClientCapabilityRecordMatchCalloutRef;
                            ret = Sd_CfgPtr->SdCapabilityRecordMatchCalloutRef[matchCalloutIdx](
                                rxQueueItem->rxPduId,
                                type1Entry.Type,
                                type1Entry.ServiceID,
                                type1Entry.InstanceID,
                                type1Entry.MajorVersion,
                                type1Entry.MinorVersion,
                                (const Sd_ConfigOptionStringType*)&sdMsg[optStartPosMap[optIndex] + 4u],
                                (const Sd_ConfigOptionStringType*)&createCfgOptPtr[4u]);
                            Sd_FreeEx(createCfgOptPtr, Sd_RxMemPool);
                            break;
                        }
                    }
                }
            }
        }
    }

    return ret;
}
#endif /* SD_CAPABILITY_RECORD_MATCH_CALLOUT_NUM > 0u */

/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_SaveRemoteSessionId(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, boolean isRxUseMulticast)
{
    Sd_SessionIdCtrlType* sesIdCtrlPtr;
    Sd_HeaderType header;

    (void)ILib_memcpy((uint8*)&header, rxQueueItem->bufPtr, sizeof(Sd_HeaderType));
    sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, &rxQueueItem->remoteAddr);
    if (NULL_PTR != sesIdCtrlPtr)
    {
        if (TRUE == isRxUseMulticast)
        {
            sesIdCtrlPtr->rxMultiSesIdCnt = Sd_NtoHs(header.SessionID);
            sesIdCtrlPtr->rxMultiRebootFlag = header.RebootFlag;
        }
        else
        {
            sesIdCtrlPtr->rxUniSesIdCnt = Sd_NtoHs(header.SessionID);
            sesIdCtrlPtr->rxUniRebootFlag = header.RebootFlag;
        }
    }
}

#if (SD_SERVER_SERVICE_NUM > 0)
/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_SaveServerServiceHandleIdToSessionCtrlList(
    uint16 instanceId,
    const TcpIp_SockAddrType* remoteAddrPtr,
    uint16 serverServiceHandleId)
{
    Sd_SessionIdCtrlType* sesIdCtrlPtr;
    Sd_ServiceHandleIdListType* serverServiceList;
    uint8 index;
    boolean findFlag = FALSE;

    if (!TCPIP_IS_IPADDR_ANY(*remoteAddrPtr)) /* PRQA S 3469*/ /* MISRA Dir 4.9 */
    {
        sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, remoteAddrPtr);
        if (NULL_PTR != sesIdCtrlPtr)
        {
            serverServiceList = sesIdCtrlPtr->serverServiceList;
            if (NULL_PTR != serverServiceList)
            {
                while (NULL_PTR != serverServiceList)
                {
                    for (index = 0u; index < serverServiceList->handleIdNum; index++)
                    {
                        if (serverServiceList->serviceHandleId[index] == serverServiceHandleId)
                        {
                            findFlag = TRUE;
                            break;
                        }
                    }
                    serverServiceList = serverServiceList->next;
                }

                if (FALSE == findFlag)
                {
                    serverServiceList = sesIdCtrlPtr->serverServiceList;
                    while (NULL_PTR != serverServiceList)
                    {
                        if (serverServiceList->handleIdNum < 16u)
                        {
                            serverServiceList->serviceHandleId[serverServiceList->handleIdNum] = serverServiceHandleId;
                            serverServiceList->handleIdNum++;
                            break;
                        }
                        serverServiceList = serverServiceList->next;
                    }
                }
            }
            else
            {
                serverServiceList = Sd_MallocEx(sizeof(Sd_ServiceHandleIdListType), Sd_RxMemPool);
                if (NULL_PTR != serverServiceList)
                {
                    serverServiceList->next = NULL_PTR;
                    serverServiceList->serviceHandleId[0u] = serverServiceHandleId;
                    serverServiceList->handleIdNum = 1u;
                    sesIdCtrlPtr->serverServiceList = serverServiceList;
                }
            }
        }
    }
}
#endif /* SD_SERVER_SERVICE_NUM > 0 */

#if (SD_CLIENT_SERVICE_NUM > 0)
/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_SaveClientServiceHandleIdToSessionCtrlList(
    uint16 instanceId,
    const TcpIp_SockAddrType* remoteAddrPtr,
    uint16 clientServiceHandleId)
{
    Sd_SessionIdCtrlType* sesIdCtrlPtr;
    Sd_ServiceHandleIdListType* clientServiceList;
    uint8 index;
    boolean findFlag = FALSE;

    if (!TCPIP_IS_IPADDR_ANY(*remoteAddrPtr))
    {
        sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, remoteAddrPtr);
        if (NULL_PTR != sesIdCtrlPtr)
        {
            clientServiceList = sesIdCtrlPtr->clientServiceList;
            if (NULL_PTR != clientServiceList)
            {
                while (NULL_PTR != clientServiceList)
                {
                    for (index = 0u; index < clientServiceList->handleIdNum; index++)
                    {
                        if (clientServiceList->serviceHandleId[index] == clientServiceHandleId)
                        {
                            findFlag = TRUE;
                            break;
                        }
                    }
                    clientServiceList = clientServiceList->next;
                }

                if (FALSE == findFlag)
                {
                    clientServiceList = sesIdCtrlPtr->clientServiceList;
                    while (NULL_PTR != clientServiceList)
                    {
                        if (clientServiceList->handleIdNum < 16u)
                        {
                            clientServiceList->serviceHandleId[clientServiceList->handleIdNum] = clientServiceHandleId;
                            clientServiceList->handleIdNum++;
                            break;
                        }
                        clientServiceList = clientServiceList->next;
                    }
                }
            }
            else
            {
                clientServiceList = Sd_MallocEx(sizeof(Sd_ServiceHandleIdListType), Sd_RxMemPool);
                if (NULL_PTR != clientServiceList)
                {
                    clientServiceList->next = NULL_PTR;
                    clientServiceList->serviceHandleId[0u] = clientServiceHandleId;
                    clientServiceList->handleIdNum = 1u;
                    sesIdCtrlPtr->clientServiceList = clientServiceList;
                }
            }
        }
    }
}
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr,remoteAddrPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Sd_SessionIdCtrlType* Sd_FindRemoteAddrSessionIdCtrl(uint16 instanceId, const TcpIp_SockAddrType* remoteAddrPtr)
{
    const Sd_InstanceRTType* instanceRtPtr = &Sd_InstanceRTData[instanceId];
    Sd_SessionIdCtrlType* sesIdCtrlPtr = NULL_PTR;

    if (NULL_PTR != instanceRtPtr->sessionIdHandle.sesIdCtrlList)
    {
        sesIdCtrlPtr = instanceRtPtr->sessionIdHandle.sesIdCtrlList;
        while (sesIdCtrlPtr != NULL_PTR)
        {
            if (0
                == ILib_memcmp(
                    (uint8*)&sesIdCtrlPtr->remoteAddr,
                    (const uint8*)remoteAddrPtr,
                    sizeof(TcpIp_SockAddrType)))
            {
                break;
            }

            sesIdCtrlPtr = sesIdCtrlPtr->next;
        }
    }

    return sesIdCtrlPtr;
}

/******************************************************************************/
/*
 * Brief: Check if the remote address is first recived
 * Param-Name[in]: instancePtr,remoteAddrPtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static boolean Sd_IsNewRemoteAddr(uint16 instanceId, const TcpIp_SockAddrType* remoteAddrPtr)
{
    const Sd_SessionIdCtrlType* sesIdCtrlPtr;
    boolean isNewRemoteAddr;

    sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, remoteAddrPtr);
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

/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Sd_SessionIdCtrlType* Sd_AddRemoteAddrToSessionIdCtrlList(
    uint16 instanceId,
    const Sd_RxQueueType* rxQueueItem,
    boolean isRxUseMulticast)
{
    Sd_InstanceRTType* instanceRtPtr = &Sd_InstanceRTData[instanceId];
    Sd_SessionIdCtrlType* sesIdCtrlPtr = NULL_PTR;
    Sd_SessionIdCtrlType* lastSesIdCtrlPtr = NULL_PTR;
    TcpIp_SockAddrType remoteAddr = rxQueueItem->remoteAddr;
    Sd_HeaderType header;

    if (!TCPIP_IS_IPADDR_ANY(remoteAddr)) /* PRQA S 3469*/ /* MISRA Dir 4.9 */
    {
        if (NULL_PTR != instanceRtPtr->sessionIdHandle.sesIdCtrlList)
        {
            sesIdCtrlPtr = instanceRtPtr->sessionIdHandle.sesIdCtrlList;
            while (sesIdCtrlPtr != NULL_PTR)
            {
                if (0
                    == ILib_memcmp((uint8*)&sesIdCtrlPtr->remoteAddr, (uint8*)&remoteAddr, sizeof(TcpIp_SockAddrType)))
                {
                    break;
                }

                lastSesIdCtrlPtr = sesIdCtrlPtr;
                sesIdCtrlPtr = sesIdCtrlPtr->next;
            }
        }

        if (NULL_PTR == sesIdCtrlPtr)
        {
            sesIdCtrlPtr = Sd_MallocEx(sizeof(Sd_SessionIdCtrlType), Sd_RxMemPool);
            if (NULL_PTR != sesIdCtrlPtr)
            {
                sesIdCtrlPtr->next = NULL_PTR;
                sesIdCtrlPtr->remoteAddr = remoteAddr;
                sesIdCtrlPtr->txUniSesIdCnt = 0u;
                (void)ILib_memcpy((uint8*)&header, rxQueueItem->bufPtr, sizeof(Sd_HeaderType));
                if (TRUE == isRxUseMulticast)
                {
                    sesIdCtrlPtr->rxMultiSesIdCnt = Sd_NtoHs(header.SessionID);
                    sesIdCtrlPtr->rxMultiRebootFlag = header.RebootFlag;
                    sesIdCtrlPtr->rxUniSesIdCnt = 0u;
                    sesIdCtrlPtr->rxUniRebootFlag = 0xFFu; /* Not Updated */
                }
                else
                {
                    sesIdCtrlPtr->rxUniSesIdCnt = Sd_NtoHs(header.SessionID);
                    sesIdCtrlPtr->rxUniRebootFlag = header.RebootFlag;
                    sesIdCtrlPtr->rxMultiSesIdCnt = 0u;
                    sesIdCtrlPtr->rxMultiRebootFlag = 0xFFu; /* Not Updated */
                }
                sesIdCtrlPtr->serverServiceList = NULL_PTR;
                sesIdCtrlPtr->clientServiceList = NULL_PTR;

                if (NULL_PTR != instanceRtPtr->sessionIdHandle.sesIdCtrlList)
                {
                    lastSesIdCtrlPtr->next = sesIdCtrlPtr;
                }
                else
                {
                    instanceRtPtr->sessionIdHandle.sesIdCtrlList = sesIdCtrlPtr;
                }
            }
        }
    }

    return sesIdCtrlPtr;
}

/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static boolean Sd_RemoteRebootDetecte(uint16 instanceId, const Sd_RxQueueType* rxQueueItem, boolean isRxUseMulticast)
{
    const Sd_SessionIdCtrlType* sesIdCtrlPtr;
    Sd_HeaderType header;
    uint16 lastSessionId, curSessionId;
    boolean isReboot = FALSE;
    uint8 lastRebootFlag;

    sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, &rxQueueItem->remoteAddr);
    if (NULL_PTR != sesIdCtrlPtr)
    {
        if (TRUE == isRxUseMulticast)
        {
            lastSessionId = sesIdCtrlPtr->rxMultiSesIdCnt;
            lastRebootFlag = sesIdCtrlPtr->rxMultiRebootFlag;
        }
        else
        {
            lastSessionId = sesIdCtrlPtr->rxUniSesIdCnt;
            lastRebootFlag = sesIdCtrlPtr->rxUniRebootFlag;
        }

        (void)ILib_memcpy((uint8*)&header, rxQueueItem->bufPtr, sizeof(Sd_HeaderType));
        curSessionId = Sd_NtoHs(header.SessionID);
        /* @req SWS_SD_00446 */
        if (((0u == lastRebootFlag) && (1u == header.RebootFlag))
            || ((1u == header.RebootFlag) && (curSessionId <= lastSessionId)))
        {
            isReboot = TRUE;
        }
    }

    return isReboot;
}

/******************************************************************************/
/*
 * Brief: Free ServiceList in sessionId control list
 * Param-Name[in]: serviceList
 * Param-Name[out]: None
 * Param-Name[in/out]: head
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_FreeServiceListInSesIdCtrlList(Sd_ServiceHandleIdListType* serviceList)
{
    Sd_ServiceHandleIdListType* serviceListCur;
    Sd_ServiceHandleIdListType* serviceListNext;

    serviceListCur = serviceList;
    serviceListNext = serviceListCur;
    while (NULL_PTR != serviceListNext)
    {
        serviceListCur = serviceListNext;
        serviceListNext = serviceListNext->next;
        Sd_FreeEx(serviceListCur, Sd_RxMemPool);
    }
}

/******************************************************************************/
/*
 * Brief: Delete the node from sessionId control list
 * Param-Name[in]: objNode
 * Param-Name[out]: None
 * Param-Name[in/out]: head
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_DeleteSessionIdNode(Sd_SessionIdCtrlType** head, const Sd_SessionIdCtrlType* objNode)
{
    Sd_SessionIdCtrlType *pNode, *qNode;

    if (NULL_PTR != *head)
    {
        pNode = *head;
        while ((pNode != objNode) && (pNode->next != NULL_PTR))
        {
            qNode = pNode;
            pNode = pNode->next;
        }
        /*Object node is found*/
        if (pNode == objNode)
        {
            /*Object node is head*/
            if (pNode == *head)
            {
                *head = pNode->next;
            }
            else
            {
                qNode->next = pNode->next;
            }
            Sd_FreeServiceListInSesIdCtrlList(pNode->serverServiceList);
            Sd_FreeServiceListInSesIdCtrlList(pNode->clientServiceList);
            Sd_FreeEx(pNode, Sd_RxMemPool);
        }
    }
}

/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_RemoteRebootDetectedHandle(uint16 instanceId, const Sd_RxQueueType* rxQueueItem)
{
    uint8 index;
    const Sd_SessionIdCtrlType* sesIdCtrlPtr;
#if (SD_CLIENT_SERVICE_NUM > 0)
    const Sd_ServiceHandleIdListType* clientServiceList;
#endif /* SD_CLIENT_SERVICE_NUM > 0 */
#if (SD_EVENT_HANDLER_NUM > 0)
    const Sd_ServiceHandleIdListType* serverServiceList;
#endif /* SD_EVENT_HANDLER_NUM > 0 */
    Sd_InstanceRTType* instanceRtPtr = &Sd_InstanceRTData[instanceId];

    sesIdCtrlPtr = Sd_FindRemoteAddrSessionIdCtrl(instanceId, &rxQueueItem->remoteAddr);
    if (NULL_PTR != sesIdCtrlPtr)
    {
#if (SD_CLIENT_SERVICE_NUM > 0)
        clientServiceList = sesIdCtrlPtr->clientServiceList;
        /* @req SWS_SD_00448 */
        while (NULL_PTR != clientServiceList)
        {
            for (index = 0u; index < clientServiceList->handleIdNum; index++)
            {
                Sd_RxStopOfferServiceHandle(clientServiceList->serviceHandleId[index]);
            }
            clientServiceList = clientServiceList->next;
        }
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_EVENT_HANDLER_NUM > 0)
        serverServiceList = sesIdCtrlPtr->serverServiceList;
        while (NULL_PTR != serverServiceList)
        {
            for (index = 0u; index < serverServiceList->handleIdNum; index++)
            {
                Sd_RebootStopSubscribeEventHandle(serverServiceList->serviceHandleId[index]);
            }
            serverServiceList = serverServiceList->next;
        }
#endif /* SD_EVENT_HANDLER_NUM > 0 */
        /* remove the found node form list*/
        Sd_DeleteSessionIdNode(&instanceRtPtr->sessionIdHandle.sesIdCtrlList, sesIdCtrlPtr);
    }
}

#if (SD_CLIENT_SERVICE_NUM > 0)
/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_RxStopOfferServiceHandle(uint16 clientServiceHandleId)
{
    const Sd_ClientServiceType* clientServicePtr;
#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    const Sd_ConsumedEventGroupType* eventGroupPtr;
    Sd_ConsumedEventGroupRTType* consumedEgRTPtr;
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */
    Sd_ClientServiceRTType* cRtDataPtr;
    uint16 index;

    cRtDataPtr = &Sd_ClientRTData[clientServiceHandleId];
    clientServicePtr = &Sd_CfgPtr->SdGlbClientService[clientServiceHandleId];

    /*@req <SWS_SD_00367> */
    /* Stop the TTL timers of this Client Service Instance*/
    Sd_SetTTL(&cRtDataPtr->ttl, 0);

    /* Report this Client Service as DOWN if it was reported AVAILABLE before */
    if (SD_CLIENT_SERVICE_AVAILABLE == cRtDataPtr->curState)
    {
#if (SD_BSWM_USED == STD_ON)
        BswM_Sd_ClientServiceCurrentState(clientServicePtr->SdClientServiceHandleId, SD_CLIENT_SERVICE_DOWN);
#endif /* SD_BSWM_USED == STD_ON */
        cRtDataPtr->curState = SD_CLIENT_SERVICE_DOWN;
    }

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
    eventGroupPtr = clientServicePtr->SdConsumedEventGroup;
    for (index = 0u; index < clientServicePtr->SdConsumedEventGroupNum; index++)
    {
        consumedEgRTPtr = &Sd_ConsumedEventGroupRTData[eventGroupPtr->SdConsumedEventGroupHandleId];
        /* Stop the TTL timers of this Client Service Instance and all related Consumed Eventgroups*/
        Sd_SetTTL(&consumedEgRTPtr->ttl, 0);

        /* Report all Consumed Eventgroups as DOWN that were reported AVAILABLE before */
        if (SD_CONSUMED_EVENTGROUP_AVAILABLE == consumedEgRTPtr->curState)
        {
#if (SD_BSWM_USED == STD_ON)
            BswM_Sd_ConsumedEventGroupCurrentState(
                eventGroupPtr->SdConsumedEventGroupHandleId,
                SD_CONSUMED_EVENTGROUP_DOWN);
#endif /* SD_BSWM_USED == STD_ON */
            consumedEgRTPtr->curState = SD_CONSUMED_EVENTGROUP_DOWN;
        }

        eventGroupPtr++;
    }
#endif
    /* Close all SoCon and Routing */
    Sd_ClientServiceDisableAllRoutingOfClient(clientServicePtr, cRtDataPtr);
    Sd_ClientServiceCloseAllSoCon(clientServicePtr, cRtDataPtr);

    /* PATCH for TC8: If not in main phase,when rx stopOfferService move to main phase */
    if (SD_CLIENT_PHASE_MAIN != cRtDataPtr->phase)
    {
        cRtDataPtr->phase = SD_CLIENT_PHASE_MAIN;
        cRtDataPtr->initialWaitTimer = 0u;
        cRtDataPtr->repetitionCount = 0u;
        cRtDataPtr->repetitionTimer = 0u;
    }
}
#endif /* SD_CLIENT_SERVICE_NUM > 0 */

#if (SD_EVENT_HANDLER_NUM > 0)
/******************************************************************************/
/*
 * Brief: Save remote address used for SessionId control
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_RebootStopSubscribeEventHandle(uint16 serverServiceHandleId)
{
    const Sd_ServerServiceType* serverServicePtr;
    const Sd_EventHandlerType* eventHandlerPtr;
    const Sd_EventHandlerTcpType* eventHandlerTcpPtr;
    Sd_EventHandlerRTType* ehRTDataPtr;
    const Sd_SubscribeManageType* subsMangPtr;
    SoAd_RoutingGroupIdType routingGroupId;
    uint16 index;

    serverServicePtr = &Sd_CfgPtr->SdGlbServerService[serverServiceHandleId];
    eventHandlerPtr = serverServicePtr->SdEventHandler;
    for (index = 0u; index < serverServicePtr->SdEventHandlerNum; index++)
    {
        ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];

        /* Handle TCP subscribed client */
        subsMangPtr = ehRTDataPtr->tcpSubscribeList;
        while (NULL_PTR != subsMangPtr)
        {
            /* Disable Routing Path*/
            eventHandlerTcpPtr = eventHandlerPtr->SdEventHandlerTcp;
            if ((NULL_PTR != eventHandlerTcpPtr) && (NULL_PTR != eventHandlerTcpPtr->SdEventActivationRef))
            {
                routingGroupId = eventHandlerTcpPtr->SdEventActivationRef->SdRoutingGroupId;
                (void)SoAd_DisableSpecificRouting(routingGroupId, subsMangPtr->soConId);
            }

            ehRTDataPtr->tcpSubsClientNum--;
            Sd_DeleteClientFromSubscribeList(
                serverServicePtr,
                eventHandlerPtr,
                &ehRTDataPtr->tcpSubscribeList,
                subsMangPtr->soConId,
                subsMangPtr->counter);

            subsMangPtr = subsMangPtr->next;
        }

        /* Handle UDP subscribed client */
        subsMangPtr = ehRTDataPtr->udpSubscribeList;
        while (NULL_PTR != subsMangPtr)
        {
            /* Fan out control*/
            Sd_FanOutMulticastUnicastCtrlDelClient(eventHandlerPtr, ehRTDataPtr, subsMangPtr->soConId);

            ehRTDataPtr->udpSubsClientNum--;
            Sd_DeleteClientFromSubscribeList(
                serverServicePtr,
                eventHandlerPtr,
                &ehRTDataPtr->udpSubscribeList,
                subsMangPtr->soConId,
                subsMangPtr->counter);

            subsMangPtr = subsMangPtr->next;
        }

        eventHandlerPtr++;
    }
}
#endif /* SD_EVENT_HANDLER_NUM > 0 */

/******************************************************************************/
/*
 * Brief: Check received Option is valid or not
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK: check pass
 *         E_NOT_NOT: check not pass
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_OptionFormatCheck(const uint8 sdMsg[])
{
    uint32 entryArrayLen, optionArrayLen;
    uint16 index;
    uint16 firstOptionIndex;
    uint16 optionLength;
    Std_ReturnType optCheckRet;
    Std_ReturnType ret = E_OK;

    entryArrayLen = Sd_NPtrtoHl(&sdMsg[SD_ENTRIES_ARRAY_LENGTH_INDEX]);
    if (0x00u != (entryArrayLen & 0x0Fu)) /* entryArrayLen must aligned with 16 */
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Option array length field index = SD_ENTRIES_ARRAY_LENGTH_INDEX(12u) + entryArrayLen + 4u*/
        optionArrayLen = Sd_NPtrtoHl(&sdMsg[16u + entryArrayLen]);
        if (optionArrayLen > 0u)
        {
            /* 1st option index = SD_FIRST_ENTRY_INDEX(16) + entryArrayLen + SD_LENGTH_OF_OPTIONS_ARRAY_FIELD_LEN(4)*/
            firstOptionIndex = 20u + (uint16)entryArrayLen;
            index = firstOptionIndex;
            while (index < (optionArrayLen + firstOptionIndex))
            {
                optCheckRet = Sd_OptionEachItemFormatCheck(&sdMsg[index]);
                if (E_NOT_OK == optCheckRet)
                {
                    ret = E_NOT_OK;
                    break;
                }
                else
                {
                    optionLength = Sd_NPtrtoHs(&sdMsg[index]);
                    /* Next Option Index = index + optionLength + SD_OPTION_LENGTH_FIELD_LEN(2) +
                     * SD_OPTION_TYPE_FIELD_LEN(1u)*/
                    index = index + optionLength + 3u;
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

/******************************************************************************/
/*
 * Brief: Check each Option item is valid or not
 * Param-Name[in]: instancePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: E_OK: check pass
 *         E_NOT_NOT: check not pass
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static Std_ReturnType Sd_OptionEachItemFormatCheck(const uint8 optionArray[])
{
    uint16 length = Sd_NPtrtoHs(&optionArray[SD_LENGTH_OFFSET]);
    uint16 port;
    uint8 type = optionArray[SD_TYEP_OFFSET];
    uint8 l4Proto;
    Std_ReturnType ret = E_NOT_OK;

    if (length > 0u)
    {
        switch (type)
        {
        case SD_CONFIGURATION_OPTION:
            ret = E_OK;
            break;

        case SD_IPV4_ENDPOINT_OPTION:
        case SD_IPV4_MULTICAST_OPTION:
        case SD_IPV4_SD_ENDPOINT_OPTION:
            l4Proto = optionArray[SD_L4PROTO_OFFSET_IPV4];
            port = Sd_NPtrtoHs(&optionArray[SD_PORTNUMBER_OFFSET_IPV4]);
            if ((0x09u == length) && ((SD_L4_PROTO_TCP == l4Proto) || (SD_L4_PROTO_UDP == l4Proto)) && (0x00u != port))
            {
                ret = E_OK;
            }
            break;

#if (STD_ON == SD_IPV6_ENABLE)
        case SD_IPV6_ENDPOINT_OPTION:
        case SD_IPV6_MULTICAST_OPTION:
        case SD_IPV6_SD_ENDPOINT_OPTION:
            l4Proto = optionArray[SD_L4PROTO_OFFSET_IPV6];
            port = Sd_NPtrtoHs(&optionArray[SD_PORTNUMBER_OFFSET_IPV6]);
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

/******************************************************************************/
/*
 * Brief: Call SoAd_IfSpecificRoutingGroupTransmit() in main function
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_IfSpecificRoutingGroupTransmit(void)
{
#if (SD_EVENT_HANDLER_NUM > 0)
    uint16 index;
    Sd_EventHandlerRTType* ehRTDataPtr = &Sd_EventHandlerRTData[0u];

    for (index = 0u; index < SD_EVENT_HANDLER_NUM; index++)
    {
        if (TRUE == ehRTDataPtr->ifSpecificRoutingTransTcpFlag)
        {
            ehRTDataPtr->ifSpecificRoutingTransTcpFlag = FALSE;

            (void)SoAd_IfSpecificRoutingGroupTransmit(ehRTDataPtr->routingGroupIdTcp, ehRTDataPtr->soConIdTcp);
        }

        if (TRUE == ehRTDataPtr->ifSpecificRoutingTransUdpFlag)
        {
            ehRTDataPtr->ifSpecificRoutingTransUdpFlag = FALSE;

            (void)SoAd_IfSpecificRoutingGroupTransmit(ehRTDataPtr->routingGroupIdUdp, ehRTDataPtr->soConIdUdp);
        }

        ehRTDataPtr++;
    }
#endif /* SD_EVENT_HANDLER_NUM > 0 */
}

#if (SD_EVENT_HANDLER_NUM > 0)
/******************************************************************************/
/*
 * Brief: If SoCon not used by Service(Include EventHandler),Reset it to wildcard
 * Param-Name[in]: None
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: This is a internal function
 */
/******************************************************************************/
static void Sd_ResetSoConToWildcard(
    const Sd_ServerServiceType* serverServicePtr,
    SoAd_SoConIdType soConId,
    boolean isTcpSoCon)
{
    const Sd_EventHandlerType* eventHandlerPtr;
    const Sd_EventHandlerRTType* ehRTDataPtr;
    const Sd_SubscribeManageType* subsMangPtr;
    uint16 idx;
    boolean resetFlag = TRUE;

    eventHandlerPtr = serverServicePtr->SdEventHandler;
    for (idx = 0u; (idx < serverServicePtr->SdEventHandlerNum) && (TRUE == resetFlag); idx++)
    {
        ehRTDataPtr = &Sd_EventHandlerRTData[eventHandlerPtr->SdEventHandlerHandleId];
        if (TRUE == isTcpSoCon)
        {
            subsMangPtr = ehRTDataPtr->tcpSubscribeList;
        }
        else
        {
            subsMangPtr = ehRTDataPtr->udpSubscribeList;
        }

        while (NULL_PTR != subsMangPtr)
        {
            if (subsMangPtr->soConId == soConId)
            {
                resetFlag = FALSE;
                break;
            }

            subsMangPtr = subsMangPtr->next;
        }
        eventHandlerPtr++;
    }

    if (TRUE == resetFlag)
    {
        /*@req <SWS_SD_00481> */
        /* If no subscription for this socket connection exists any more,
         * socket connection shall be reset to wildcard */
        SoAd_ReleaseRemoteAddr(soConId);
    }
}
#endif /* SD_EVENT_HANDLER_NUM > 0 */

#if (SD_CONSUMED_EVENTGROUP_NUM > 0)
/******************************************************************************/
/*
 * Brief: Reset all ConsumedEventgroup runtime data isTcpSubsSend/isUdpSubsSend
          flag in Client Service
 * Param-Name[in]: clientServicePtr, tcpFlag, udpFlag
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ClientSaveSoConId
 */
/******************************************************************************/
static void Sd_ClientServiceResetAllCEgSubsEntrySendFlag(
    const Sd_ClientServiceType* clientServicePtr,
    boolean tcpFlag,
    boolean udpFlag)
{
    const Sd_ConsumedEventGroupType* cEgPtr;
    Sd_ConsumedEventGroupRTType* cEgRTPtr;
    uint16 index;

    cEgPtr = clientServicePtr->SdConsumedEventGroup;
    for (index = 0u; index < clientServicePtr->SdConsumedEventGroupNum; index++)
    {
        cEgRTPtr = &Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
        if (TRUE == tcpFlag)
        {
            cEgRTPtr->isTcpSubsSend = FALSE;
        }
        if (TRUE == udpFlag)
        {
            cEgRTPtr->isUdpSubsSend = FALSE;
        }

        cEgPtr++;
    }
}

#if (STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE)
/******************************************************************************/
/*
 * Brief: Reset all ConsumedEventgroup runtime data subsRetryCnt/subsRetryTimer/
          subsRetryEnable in Client Service
 * Param-Name[in]: clientServicePtr
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_RxOfferServiceHandle
 */
/******************************************************************************/
static void Sd_ClientServiceResetAllCEgSubsEntrySubsRetryCnt(const Sd_ClientServiceType* clientServicePtr)
{
    const Sd_ConsumedEventGroupType* cEgPtr;
    Sd_ConsumedEventGroupRTType* cEgRTPtr;
    uint16 index;

    cEgPtr = clientServicePtr->SdConsumedEventGroup;
    for (index = 0u; index < clientServicePtr->SdConsumedEventGroupNum; index++)
    {
        cEgRTPtr = &Sd_ConsumedEventGroupRTData[cEgPtr->SdConsumedEventGroupHandleId];
        cEgRTPtr->subsRetryCnt = 0u;
        cEgRTPtr->subsRetryTimer = 0u;
        cEgRTPtr->subsRetryEnable = FALSE;

        cEgPtr++;
    }
}
#endif /* STD_ON == SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLE */
#endif /* SD_CONSUMED_EVENTGROUP_NUM > 0 */

/******************************************************************************/
/*
 * Brief: SendTime value adjust,when sendTime equal 0,set it to SD_SEND_ENTRY_IMMEDIATELY
 * Param-Name[in]: sendTime
 * Param-Name[out]: None
 * Param-Name[in/out]: None
 * Return: None
 * PreCondition: None
 * CallByAPI: Sd_ClientServiceInitialWaitEnter
 */
/******************************************************************************/
SD_LOCAL_INLINE uint32 Sd_SendTimeAdjust(uint32 sendTime)
{
    return ((sendTime == 0u) ? SD_SEND_ENTRY_IMMEDIATELY : sendTime);
}

#define SD_STOP_SEC_CODE
#include "Sd_MemMap.h"

#endif /* SD_MODULE_ENABLE == STD_ON */
