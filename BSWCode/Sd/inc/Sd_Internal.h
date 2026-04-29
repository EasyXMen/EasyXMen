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
 **  @file               : Sd_Internal.h
 **  @author             : ZouZhiJia
 **  @date               : 2025-4-2
 **  @vendor             : isoft
 **  @description        : Internal type definitions of SD
 **
 ***********************************************************************************************************************/

#ifndef SD_INTERNAL_H
#define SD_INTERNAL_H
/* PRQA S 1536 EOF */ /* VL_Sd_1536 */
/* =================================================== inclusions =================================================== */

#include "ComStack_Types.h"
#include "Det.h"
#include "Sd_Types.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */

/**
 * Macro to enable or disable IPv6 support.
 */
#define SD_IPV6_ENABLE STD_OFF

/**
 * Padding value for Client ID in SOME/IP-SD message header.
 */
#define SD_CLIENT_ID_PADDING_VALUE 0x00u

/**
 * Padding value for Protocol Version in SOME/IP-SD message header.
 */
#define SD_PROTOCAL_VERSION_PADDING_VALUE 0x01u

/**
 * Padding value for Interface Version in SOME/IP-SD message header.
 */
#define SD_INTERFACE_VERSION_PADDING_VALUE 0x01u

/**
 * Padding value for Message Type in SOME/IP-SD message header.
 */
#define SD_MESSAGE_TYPE_PADDING_VALUE 0x02u

/**
 * Padding value for Return Code in SOME/IP-SD message header.
 */
#define SD_RETURN_CODE_PADDING_VALUE 0x00u

/**
 * Type field value for Find Service entry.
 */
#define SD_ENTRY_TYPE_FIND_SERVICE 0x00u

/**
 * Type field value for Offer Service entry.
 */
#define SD_ENTRY_TYPE_OFFER_SERVICE 0x01u

/**
 * Private type field value for Stop Offer Service entry.
 */
#define SD_ENTRY_TYPE_STOP_OFFER_SERVICE 0xF1u

/**
 * Type field value for Subscribe Event Group entry.
 */
#define SD_ENTRY_TYPE_SUBSCRIBE_EVENT_GROUP 0x06u

/**
 * Type field value for Subscribe Event Group Ack entry.
 */
#define SD_ENTRY_TYPE_SUBSCRIBE_EVENTGROUP_ACK 0x07u

/**
 * Option type value for Configuration Option.
 */
#define SD_CONFIGURATION_OPTION 0x01u

/**
 * Option type value for IPv4 Endpoint Option.
 */
#define SD_IPV4_ENDPOINT_OPTION 0x04u

/**
 * Option type value for IPv6 Endpoint Option.
 */
#define SD_IPV6_ENDPOINT_OPTION 0x06u

/**
 * Option type value for IPv4 Multicast Option.
 */
#define SD_IPV4_MULTICAST_OPTION 0x14u

/**
 * Option type value for IPv6 Multicast Option.
 */
#define SD_IPV6_MULTICAST_OPTION 0x16u

/**
 * Option type value for IPv4 SD Endpoint Option.
 */
#define SD_IPV4_SD_ENDPOINT_OPTION 0x24u

/**
 * Option type value for IPv6 SD Endpoint Option.
 */
#define SD_IPV6_SD_ENDPOINT_OPTION 0x26u

/**
 * Length of IPv4 Endpoint Option.
 */
#define SD_IPV4_ENDPOINT_OPTION_LENGTH 0x09u

/**
 * Length of IPv6 Endpoint Option.
 */
#define SD_IPV6_ENDPOINT_OPTION_LENGTH 0x15u

/**
 * Length of IPv4 Multicast Option.
 */
#define SD_IPV4_MULTICAST_OPTION_LENGTH 0x09u

/**
 * Length of IPv6 Multicast Option.
 */
#define SD_IPV6_MULTICAST_OPTION_LENGTH 0x15u

/**
 * Length of IPv4 SD Endpoint Option.
 */
#define SD_IPV4_SD_ENDPOINT_OPTION_LENGTH 0x09u

/**
 * Length of IPv6 SD Endpoint Option.
 */
#define SD_IPV6_SD_ENDPOINT_OPTION_LENGTH 0x15u

/**
 * Layer 4 Protocol value for TCP.
 */
#define SD_L4_PROTO_TCP 0x06u

/**
 * Layer 4 Protocol value for UDP.
 */
#define SD_L4_PROTO_UDP 0x11u

/**
 * Position of "Length of Entries Array" in SD message.
 */
#define SD_ENTRIES_ARRAY_LENGTH_INDEX 12u

/**
 * Position of the first entry in SD message.
 */
#define SD_FIRST_ENTRY_INDEX 16u

/**
 * Size of each entry in SD message.
 */
#define SD_ENTRY_SIZE 16u

/**
 * Length of the "Length of Options Array" field.
 */
#define SD_LENGTH_OF_OPTIONS_ARRAY_FIELD_LEN 4u

/**
 * Length of the "Option Length" field.
 */
#define SD_OPTION_LENGTH_FIELD_LEN 2u

/**
 * Length of the "Option Type" field.
 */
#define SD_OPTION_TYPE_FIELD_LEN 1u

/**
 * Offset of the "Length" field in an option.
 */
#define SD_LENGTH_OFFSET 0u

/**
 * Offset of the "Type" field in an option.
 */
#define SD_TYPE_OFFSET 2u

/**
 * Offset of the "IP Address" field in an option.
 */
#define SD_IPADDR_OFFSET 4u

/**
 * Offset of the "Layer 4 Protocol" field in an IPv4 option.
 */
#define SD_L4PROTO_OFFSET_IPV4 9u

/**
 * Offset of the "Port Number" field in an IPv4 option.
 */
#define SD_PORTNUMBER_OFFSET_IPV4 10u

/**
 * Offset of the "Layer 4 Protocol" field in an IPv6 option.
 */
#define SD_L4PROTO_OFFSET_IPV6 21u

/**
 * Offset of the "Port Number" field in an IPv6 option.
 */
#define SD_PORTNUMBER_OFFSET_IPV6 22u

/**
 * Index of the "Flags" field in SD message header.
 */
#define SD_HEADER_FLAGS_INDEX 8u

/**
 * Mask for the Unicast Flag in the "Flags" field.
 */
#define SD_FLAGS_UNICAST_FLAG_MASK 0x40u

/**
 * Maximum number of entries that can be sent in one frame.
 */
#define SD_MAX_ENTRIES_PER_FRAME 20u

/**
 * Macro to send an entry immediately.
 */
#define SD_SEND_ENTRY_IMMEDIATELY 0UL

/**
 * Multiplier for random number calculation.
 */
#define SD_RANDOM_MULTIPLIER 1664525u

/**
 * Increment for random number calculation.
 */
#define SD_RANDOM_INCREMENT 1013904223u

/**
 * Macro to indicate unused variables.
 */
#define SD_UNUSED(x) (void)(x)

/**
 * Maximum length of an array used in certain contexts.
 */
#define SD_ARRAY_LEN_70 70

/**
 * Standard array length of 16 elements.
 */
#define SD_ARRAY_LEN_16 16

/**
 * Small array length of 3 elements.
 */
#define SD_ARRAY_LEN_3 3

/**
 * Length of the "Length of Entries Array" field in the SD message.
 */
#define SD_MESSAGE_ENTRYLEN_FILED_LEN 4u

/**
 * Length of the "Length of Options Array" field in the SD message.
 */
#define SD_MESSAGE_OPTIONLEN_FILED_LEN 4UL

/**
 * Length of the TTL (Time to Live) field in the SD message entry.
 */
#define SD_MESSAGE_ENTRY_TTL_FIELD_LEN 3u

/**
 * Total length of an SD message entry.
 */
#define SD_MESSAGE_ENTRY_TOTAL_LEN 16u

/**
 * Length of an IPv4 option in the SD message.
 */
#define SD_MESSAGE_IPV4_OPTION_LEN 12u

/**
 * Length of an IPv6 option in the SD message.
 */
#define SD_MESSAGE_IPV6_OPTION_LEN 24u

/**
 * Length of the first three bytes in the Type 1 option.
 */
#define SD_MESSAGE_TYPE1_OPTION_FIRST_THREE_BYTE_LEN 3UL

/**
 * Length of the first five bytes in the Type 1 option.
 */
#define SD_MESSAGE_TYPE1_OPTION_FIRST_FIVE_BYTE_LEN 5u

/**
 * Length of the hostname configuration in the Type 1 option.
 */
#define SD_MESSAGE_TYPE1_OPTION_HOSTNAME_CFG_LEN 10u

/**
 * Maximum number of reference options per entry.
 */
#define SD_ENTRY_MAX_REF_OPTION_NUM 3u

/**
 * Length of the SD message header.
 */
#define SD_HEADER_LEN 12UL

/**
 * Length of an IPv4 address.
 */
#define SD_IPV4_ADDRESS_LEN 4u

/**
 * Length of an IPv6 address.
 */
#define SD_IPV6_ADDRESS_LEN 16u

/**
 * Maximum length of an IP address.
 */
#define SD_IP_ADDRESS_MAX_LEN 16UL

/**
 * Length of a hostname string.
 */
#define SD_STRING_HOSTNAME_LEN 8u

/**
 * Length for four-byte endianness swap.
 */
#define SD_ENDIAN_SWAP_FOUR_BYTE_LEN 4u

/**
 * Length for two-byte endianness swap.
 */
#define SD_ENDIAN_SWAP_TWO_BYTE_LEN 2u

/**
 * Offset of the "Option Type" field in an option.
 */
#define SD_OPTION_TYPE_FILED_OFFSET 2UL

/**
 * Offset of the reserved field in an option.
 */
#define SD_OPTION_RESERVED_FILED_OFFSET 3u

/**
 * Offset of the first key in the Type 1 option.
 */
#define SD_TYPE1_OPTION_FIRST_KEY_OFFSET 4u

/**
 * Maximum number of subscribe clients.
 */
#define SD_MAX_SUBSCRIBE_CLIENT_NUM 16u

/**
 * Maximum number of remote services.
 */
#define SD_MAX_REMOTE_SERVICE_NUM 16u

/**
 * Reserved value for the subscribe counter field.
 */
#define SD_SUBSCRIBE_COUNTER_FIELD_RESERVED 0xffu

/**
 * Reserved value for the reboot flag.
 */
#define SD_REBOOT_FLAG_RESERVED 0xffu

/**
 * Reserved value for the network mask.
 */
#define SD_NETWORK_MASK_VALUE_RESERVED 0xffu

/**
 * Index 0 for entry reference options.
 */
#define SD_ENTRY_REF_OPTION_INDEX_0 0

/**
 * Index 1 for entry reference options.
 */
#define SD_ENTRY_REF_OPTION_INDEX_1 1

/**
 * Index 2 for entry reference options.
 */
#define SD_ENTRY_REF_OPTION_INDEX_2 2

/**
 * Magic integer value 0x07.
 */
#define SD_MAGIC_INT_0X07 0x07u

/**
 * Magic character '='.
 */
#define SD_MAGIC_CHAR_EQUAL '='

/**
 * Value of the length field in an IPv4 option.
 */
#define SD_IPV4_OPTION_LENGTH_FIELD_VALUE 0x09u

/**
 * Conversion fraction from seconds to milliseconds.
 */
#define SD_SEC_TO_MS_CONVERSION_FRACTION 1000ULL

/**
 * Conversion fraction from bytes to bits.
 */
#define SD_BYTE_TO_BIT_CONVERSION_FRACTION 3u

/**
 * Fraction for 16-byte alignment.
 */
#define SD_16_BYTE_ALIGNED_FRACTION 0x0fUL

/**
 * Message type value for notification.
 */
#define SD_MESSAGE_TYPE_NOTIFY 0x02u

/**
 * TTL value indicating forever.
 */
#define SD_MESSAGE_ENTRY_TTL_FOREVER 0xffffffUL

/**
 * Minor version value indicating any.
 */
#define SD_MESSAGE_MINOR_VERSION_ANY 0xffffffffUL

/**
 * Major version value indicating any.
 */
#define SD_MESSAGE_MAJOR_VERSION_ANY 0xffu

/**
 * Service ID value indicating any.
 */
#define SD_MESSAGE_SERVICE_ID_ANY 0xffffu

/**
 * Instance ID value indicating any.
 */
#define SD_MESSAGE_INSTANCE_ID_ANY 0xffffu

#define SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN 4

#define SD_HEADER_RESERVED_FIELD_BIT_LEN 6

#define SD_ENTRY_COUNTER_FIELD_BIT_LEN 4

#define SD_ENTRY_RESERVED_FIELD_BIT_LEN 4

#define SD_ENTRY_RESERVED_FIELD_HIGH_BIT_LEN 8

#define SD_OPTION_RESERVED_FIELD_BIT_LEN 8

#define SD_TYPE1_ENTRY_TTL_FIELD_LENGTH 3

#define SD_OPT_START_POSTITION_LEN 3

/* ================================================ type definitions ================================================ */
/* PRQA S 0635 EOF */ /* VL_Sd_BitField */
/**
 * @brief SOME/IP-SD Header Type
 */
typedef struct Sd_HeaderTypeTag
{
    uint16 ClientID;         /**< Client ID. @range 0..65535 */
    uint16 SessionID;        /**< Session ID. @range 0..65535 */
    uint8  ProtocolVersion;  /**< Protocol version. @range 0..255 */
    uint8  InterfaceVersion; /**< Interface version. @range 0..255 */
    uint8  MessageType;      /**< Message type. @range 0..255 */
    uint8  ReturnCode;       /**< Return code. @range 0..255 */
#if CPU_BYTE_ORDER == HIGH_BYTE_FIRST
    uint8 RebootFlag  : 1;                    /**< Reboot flag. @range 0..1 */
    uint8 UnicastFlag : 1;                    /**< Unicast flag. @range 0..1 */
    uint8 : SD_HEADER_RESERVED_FIELD_BIT_LEN; /**< Reserved bits. @range 0..63 */
#else
    uint8 : SD_HEADER_RESERVED_FIELD_BIT_LEN; /**< Reserved bits. @range 0..63 */
    uint8 UnicastFlag : 1;                    /**< Unicast flag. @range 0..1 */
    uint8 RebootFlag  : 1;                    /**< Reboot flag. @range 0..1 */
#endif
    uint8 Reserved[SD_ARRAY_LEN_3]; /**< Reserved bytes. @range 0..255 */
} Sd_HeaderType;

/**
 * @brief Entry Format Type 1
 */
typedef struct Sd_Type1EntryTypeTag
{
    uint8 Type;            /**< Entry type. @range 0..255 */
    uint8 Index1stOptions; /**< Index of the first options. @range 0..255 */
    uint8 Index2ndOptions; /**< Index of the second options. @range 0..255 */
#if CPU_BYTE_ORDER == HIGH_BYTE_FIRST
    uint8 NumOfOpt1 : SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN; /**< Number of first options. @range 0..15 */
    uint8 NumOfOpt2 : SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN; /**< Number of second options. @range 0..15 */
#else
    uint8 NumOfOpt2 : SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN; /**< Number of second options. @range 0..15 */
    uint8 NumOfOpt1 : SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN; /**< Number of first options. @range 0..15 */
#endif
    uint16 ServiceID;                            /**< Service ID. @range 0..65535 */
    uint16 InstanceID;                           /**< Instance ID. @range 0..65535 */
    uint8  MajorVersion;                         /**< Major version. @range 0..255 */
    uint8  TTL[SD_TYPE1_ENTRY_TTL_FIELD_LENGTH]; /**< Time to live in seconds. @range 0..255 */
    uint32 MinorVersion;                         /**< Minor version. @range 0..4294967295 */
} Sd_Type1EntryType;

/**
 * @brief Entry Format Type 2
 */
typedef struct Sd_Type2EntryTypeTag
{
    uint8 Type;            /**< Entry type. @range 0..255 */
    uint8 Index1stOptions; /**< Index of the first options. @range 0..255 */
    uint8 Index2ndOptions; /**< Index of the second options. @range 0..255 */
#if CPU_BYTE_ORDER == HIGH_BYTE_FIRST
    uint8 NumOfOpt1 : SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN; /**< Number of first options. @range 0..15 */
    uint8 NumOfOpt2 : SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN; /**< Number of second options. @range 0..15 */
#else
    uint8 NumOfOpt2 : SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN; /**< Number of second options. @range 0..15 */
    uint8 NumOfOpt1 : SD_ENTRY_NUM_OF_OPTION_FIELD_BIT_LEN; /**< Number of first options. @range 0..15 */
#endif
    uint16 ServiceID;                           /**< Service ID. @range 0..65535 */
    uint16 InstanceID;                          /**< Instance ID. @range 0..65535 */
    uint8  MajorVersion;                        /**< Major version. @range 0..255 */
    uint8  TTL[SD_MESSAGE_ENTRY_TTL_FIELD_LEN]; /**< Time to live in seconds. @range 0..255 */
    uint8  Reserved;
#if CPU_BYTE_ORDER == HIGH_BYTE_FIRST
    uint8 : SD_ENTRY_RESERVED_FIELD_BIT_LEN;        /**< Reserved low 4 bits. @range 0..15 */
    uint8 Counter : SD_ENTRY_COUNTER_FIELD_BIT_LEN; /**< Counter. @range 0..15 */
#else
    uint8 Counter : SD_ENTRY_COUNTER_FIELD_BIT_LEN; /**< Counter. @range 0..15 */
    uint8 : SD_ENTRY_RESERVED_FIELD_BIT_LEN;        /**< Reserved low 4 bits. @range 0..15 */
#endif
    uint16 EventGroupID; /**< Event group ID. @range 0..65535 */
} Sd_Type2EntryType;

/**
 * @brief Used to create IPv4 Endpoint option, IPv4 Multicast option,IPv4 SD Endpoint option
 */
typedef struct Sd_IPv4OptionsTypeTag
{
    uint16 Length;                           /**< Length of the option. @range 0..65535 */
    uint8  Type;                             /**< Option type. @range 0..255 */
    uint8  Reserved1;                        /**< Reserved byte. @range 0..255 */
    uint8  IPv4Address[SD_IPV4_ADDRESS_LEN]; /**< IPv4 address. @range 0..255 */
    uint8  Reserved2;                        /**< Reserved byte. @range 0..255 */
    uint8  L4Proto;                          /**< Layer 4 protocol. @range 0..255 */
    uint16 PortNumber;                       /**< Port number. @range 0..65535 */
} Sd_IPv4OptionsType;

/**
 * @brief Used to create IPv6 Endpoint option, IPv6 Multicast option, IPv6 SD Endpoint option
 */
typedef struct Sd_IPv6OptionsTypeTag
{
    uint16 Length;                           /**< Length of the option. @range 0..65535 */
    uint8  Type;                             /**< Option type. @range 0..255 */
    uint8  Reserved1;                        /**< Reserved byte. @range 0..255 */
    uint8  IPv6Address[SD_IPV6_ADDRESS_LEN]; /**< IPv6 address. @range 0..255 */
    uint8  Reserved2;                        /**< Reserved byte. @range 0..255 */
    uint8  L4Proto;                          /**< Layer 4 protocol. @range 0..255 */
    uint16 PortNumber;                       /**< Port number. @range 0..65535 */
} Sd_IPv6OptionsType;

/**
 * @brief Sd State Type.
 */
typedef enum Sd_StatusTypeTag
{
    SD_UNINIT, /**< Uninitialized state. @range 0 */
    SD_INIT    /**< Initialized state. @range 1 */
} Sd_StatusType;

/**
 * @brief Sd Init result type.
 */
typedef enum Sd_InitResultTypeTag
{
    SD_INIT_FAIL,   /**< Initialization failed. @range 0 */
    SD_INIT_SUCCESS /**< Initialization succeeded. @range 1 */
} Sd_InitResultType;

/**
 * @brief Defines server service runtime phase
 */
typedef enum Sd_ServerServicePhaseTypeTag
{
    SD_SERVER_PHASE_DOWN,         /**< Server phase down. @range 0 */
    SD_SERVER_PHASE_INITIAL_WAIT, /**< Server phase initial wait. @range 1 */
    SD_SERVER_PHASE_REPETITION,   /**< Server phase repetition. @range 2 */
    SD_SERVER_PHASE_MAIN,         /**< Server phase main. @range 3 */
} Sd_ServerServicePhaseType;

/**
 * @brief Type of the SD client phase.
 */
typedef enum Sd_ClientServicePhaseTypeTag
{
    SD_CLIENT_PHASE_DOWN,         /**< Client phase down. @range 0 */
    SD_CLIENT_PHASE_INITIAL_WAIT, /**< Client phase initial wait. @range 1 */
    SD_CLIENT_PHASE_REPETITION,   /**< Client phase repetition. @range 2 */
    SD_CLIENT_PHASE_MAIN,         /**< Client phase main. @range 3 */
} Sd_ClientServicePhaseType;

/**
 * @brief SD TTL type.
 */
typedef struct Sd_TTLTypeTag
{
    uint64  ttlms;          /**< Time to live in milliseconds. @range 0..18446744073709551615 */
    boolean IsValidForever; /**< Flag indicating if TTL is valid forever. @range TRUE/FALSE */
} Sd_TTLType;

/**
 * @brief Type for the send queue.
 */
typedef struct Sd_SendQueueTypeTag
{
    TcpIp_SockAddrType DestAddr;                                /**< Destination address. @range NA */
    uint16             OptStartPos[SD_OPT_START_POSTITION_LEN]; /**< Start positions of options. @range 0..65535 */
    uint8*             EntryPtr;                                /**< Pointer to the entry. @range NA */
    uint8*             OptionPtr;                               /**< Pointer to the options. @range NA */
    struct Sd_SendQueueTypeTag* Next;          /**< Pointer to the next element in the queue. @range NA */
    uint32                      SendTime;      /**< Send time in milliseconds. @range 0..4294967295 */
    uint16                      OptionLen;     /**< Length of the options. @range 0..65535 */
    uint8                       OptionNum;     /**< Number of options. @range 0..255 */
    boolean                     IsUnicastSend; /**< Flag indicating if the send is unicast. @range TRUE/FALSE */
} Sd_SendQueueType;

/**
 * @brief Server service run time type.
 */
typedef struct Sd_ServerServiceRTTypeTag
{
    Sd_TTLType                   Ttl;              /**< Time to live information. @range NA */
    uint32                       InitialWaitTimer; /**< Initial wait timer in milliseconds. @range 0..4294967295 */
    uint32                       RepetitionTimer;  /**< Repetition timer in milliseconds. @range 0..4294967295 */
    uint32                       MainTimer;        /**< Main timer in milliseconds. @range 0..4294967295 */
    Sd_SendQueueType*            Head;             /**< Pointer to the head of the send queue. @range NA */
    uint16                       ServiceGroupRequestCounter; /**< Service group request counter. @range 0..65535 */
    Sd_ServiceGroupIdType        ServiceGroupId;             /**< Service group ID. @range 0..65535 */
    Sd_ServerServiceSetStateType SetState;                   /**< Set state of the server service. @range NA */
    Sd_ServerServicePhaseType    Phase;                      /**< Phase of the server service. @range NA */
    uint8                        RepetitionCount;            /**< Repetition count. @range 0..255 */
} Sd_ServerServiceRTType;

/**
 * @brief Socket connect manager type.
 */
typedef struct Sd_SoConManageTypeTag
{
    SoAd_SoConIdType SoConId;           /**< Socket connection ID. @range 0..255 */
    boolean          IsOptRcved    : 1; /**< Flag indicating if options are received. @range 0..1 */
    boolean          IsSoConOpened : 1; /**< Flag indicating if the socket connection is opened. @range 0..1 */
} Sd_SoConManageType;

/**
 * @brief Client service run time type
 */
typedef struct Sd_ClientServiceRTTypeTag
{
    TcpIp_SockAddrType ServerUdpAddr; /**< Server UDP address for ConsumedEventgroup Multicast Receive. @range NA */
    TcpIp_SockAddrType OfferEntryRemoteAddr;            /**< Remote address of the offer entry. @range NA */
    Sd_TTLType         Ttl;                             /**< Time to live information. @range NA */
    Sd_SendQueueType*  Head;                            /**< Pointer to the head of the send queue. @range NA */
    uint32             InitialWaitTimer;                /**< Initial wait timer in milliseconds. @range 0..4294967295 */
    uint32             RepetitionTimer;                 /**< Repetition timer in milliseconds. @range 0..4294967295 */
    uint32             MainTimer;                       /**< Main timer in milliseconds. @range 0..4294967295 */
    uint16             ServiceGroupRequestCounter;      /**< Service group request counter. @range 0..65535 */
    Sd_SoConManageType TcpSoCon;                        /**< TCP socket connection management. @range NA */
    Sd_SoConManageType UdpSoCon;                        /**< UDP socket connection management. @range NA */
    Sd_SoConManageType MulitcastSoCon;                  /**< Mulitcast socket connection management. @range NA */
    Sd_ClientServiceSetStateType     SetState;          /**< Set state of the client service. @range NA */
    Sd_ClientServiceCurrentStateType CurState;          /**< Current state of the client service. @range NA */
    Sd_ClientServicePhaseType        Phase;             /**< Phase of the client service. @range NA */
    PduIdType                        OfferEntryRxPduId; /**< PDU ID of the offer entry receive. @range 0..255 */
    uint8                            RepetitionCount;   /**< Repetition count. @range 0..255 */
    boolean OfferServiceRxFlag;      /**< Flag indicating if offer service is received. @range TRUE/FALSE */
    boolean RxTcpEndpointFlag;       /**< Flag indicating if TCP endpoint is received. @range TRUE/FALSE */
    boolean RxUdpEndpointFlag;       /**< Flag indicating if UDP endpoint is received. @range TRUE/FALSE */
    boolean RxMulitcastEndpointFlag; /**< Flag indicating if Mulitcast endpoint is received. @range TRUE/FALSE */
} Sd_ClientServiceRTType;

/**
 * @brief Structure for managing client service.
 */
typedef struct Sd_SubscribeManageTypeTag
{
    TcpIp_SockAddrType                ClientIpAddr;   /**< Client IP address. @range NA */
    TcpIp_SockAddrType                SockRemoteAddr; /**< Remote socket address. @range NA */
    Sd_TTLType                        Ttl;            /**< Time to live information. @range NA */
    struct Sd_SubscribeManageTypeTag* Next;           /**< Pointer to the next element in the list. @range NA */
    SoAd_SoConIdType                  SoConId;        /**< Socket connection ID. @range 0..255 */
    uint8                             Counter;        /**< Counter. @range 0..255 */
} Sd_SubscribeManageType;

typedef struct Sd_TriggerTransmitManagerTypeTag
{
    struct Sd_TriggerTransmitManagerTypeTag* Next;
    SoAd_RoutingGroupIdType                  RoutingGroupId;
    SoAd_SoConIdType                         SoConId;
} Sd_TriggerTransmitManagerType;

/**
 * @brief  Structure for the event handler runtime data.
 */
typedef struct Sd_EventHandlerRTTypeTag
{
    Sd_SubscribeManageType*         UdpSubscribeList;       /**< Pointer to the UDP subscribe list. @range NA */
    Sd_SubscribeManageType*         TcpSubscribeList;       /**< Pointer to the TCP subscribe list. @range NA */
    Sd_TriggerTransmitManagerType*  UdpTriggerTransmitList; /**< List for UDP trigger transmit */
    Sd_TriggerTransmitManagerType*  TcpTriggerTransmitList; /**< List for TCP trigger transmit */
    Sd_EventHandlerCurrentStateType CurState;               /**< Current state of the event handler. @range NA */
    uint8                           UdpSubsEndPointNum;     /**< Number of UDP subscription endpoints. @range 0..255 */
    uint8                           UdpSubsClientNum;       /**< Number of UDP subscription clients. @range 0..16 */
    uint8                           TcpSubsClientNum;       /**< Number of TCP subscription clients. @range 0..16 */
    SoAd_SoConIdType                SoConIdTcp;             /**< TCP socket connection ID. @range 0..255 */
} Sd_EventHandlerRTType;

/**
 * @brief Structure for the Rx queue.
 */
typedef struct Sd_RxQueueTypeTag
{
    TcpIp_SockAddrType        RemoteAddr; /**< Remote address. @range NA */
    uint8*                    BufPtr;     /**< Pointer to the buffer. @range NA */
    struct Sd_RxQueueTypeTag* Next;       /**< Pointer to the next element in the queue. @range NA */
    PduLengthType             PduLength;  /**< Length of the PDU. @range 0..65535 */
    PduIdType                 RxPduId;    /**< Received PDU ID. @range 0..255 */
} Sd_RxQueueType;

/**
 * @brief Structure for the consumed event runtime data.
 */
typedef struct Sd_ConsumedEventGroupRTTypeTag
{
    TcpIp_SockAddrType                RemoteAddr;     /**< Remote address. @range NA */
    Sd_TTLType                        Ttl;            /**< Time to live information. @range NA */
    uint16                            SubsRetryTimer; /**< Subscription retry timer in milliseconds. @range 0..65535 */
    Sd_SoConManageType                MultiSoCon;     /**< Multicast socket connection management. @range NA */
    Sd_ConsumedEventGroupSetStateType SetState;       /**< Set state of the consumed event group. @range NA */
    Sd_ConsumedEventGroupCurrentStateType CurState;   /**< Current state of the consumed event group. @range NA */
    uint8                                 SubsRetryCnt; /**< Subscription retry count. @range 0..255 */
    boolean                               IsSubscribed; /**< Flag indicating if subscribed. @range TRUE/FALSE */
    boolean IsTcpSubsSend;       /**< Flag indicating if TCP subscription is sent. @range TRUE/FALSE */
    boolean IsUdpSubsSend;       /**< Flag indicating if UDP subscription is sent. @range TRUE/FALSE */
    boolean IsMulitcastSubsSend; /**< Flag indicating if Mulitcast subscription is sent. @range TRUE/FALSE */
    boolean IsAnsweredWithAck;   /**< Flag indicating if answered with acknowledgment. @range TRUE/FALSE */
    boolean
        IsSendSubEvtAfterRequest; /**< Flag indicating if send subscription event after request. @range TRUE/FALSE */
    boolean SubsRetryEnable;      /**< Flag indicating if subscription retry is enabled. @range TRUE/FALSE */
} Sd_ConsumedEventGroupRTType;

/**
 * @brief Service handle ID list type.
 */
typedef struct Sd_ServiceHandleIdListTypeTag
{
    uint16 ServiceHandleId[SD_ARRAY_LEN_16];    /**< Array of service handle IDs. @range 0..65535 */
    uint8  HandleIdNum;                         /**< Number of used service handle IDs. @range 0..16 */
    struct Sd_ServiceHandleIdListTypeTag* Next; /**< Pointer to the next element in the list. @range NA */
} Sd_ServiceHandleIdListType;

/**
 * @brief Structure for the list of service handle IDs.
 */
typedef struct Sd_SessionIdCtrlTypeTag
{
    TcpIp_SockAddrType              RemoteAddr;        /**< Remote address. @range NA */
    Sd_ServiceHandleIdListType*     ServerServiceList; /**< Pointer to the server service handle ID list. @range NA */
    Sd_ServiceHandleIdListType*     ClientServiceList; /**< Pointer to the client service handle ID list. @range NA */
    struct Sd_SessionIdCtrlTypeTag* Next;              /**< Pointer to the next element in the list. @range NA */
    uint16                          TxUniSesIdCnt;     /**< Count of transmitted unicast session IDs. @range 0..65535 */
    uint16                          RxUniSesIdCnt;     /**< Count of received unicast session IDs. @range 0..65535 */
    uint16                          RxMultiSesIdCnt;   /**< Count of received multicast session IDs. @range 0..65535 */
    uint8                           RxUniRebootFlag;   /**< Flag indicating unicast reboot. @range 0..1 */
    uint8                           RxMultiRebootFlag; /**< Flag indicating multicast reboot. @range 0..1 */
} Sd_SessionIdCtrlType;

/**
 * @brief Structure for the session ID handle.
 */
typedef struct Sd_SessionIdHandleTypeTag
{
    Sd_SessionIdCtrlType* SesIdCtrlList;   /**< Pointer to the session ID control list. @range NA */
    uint16                TxMultiSesIdCnt; /**< Count of transmitted multicast session IDs. @range 0..65535 */
} Sd_SessionIdHandleType;

/**
 * @brief Structure for the instance runtime data.
 */
typedef struct Sd_InstanceRTTypeTag
{
    Sd_SessionIdHandleType SessionIdHandle; /**< Session ID handle. @range NA */
    TcpIp_IpAddrStateType  IpAddrState;     /**< IP address state. @range NA */
    boolean                RebootFlag;      /**< Reboot flag. @range TRUE/FALSE */
} Sd_InstanceRTType;

#ifdef __cplusplus
}
#endif

#endif
