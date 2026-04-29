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
 **  @file               : DoIP_Internal_Common.h
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : Public part in DoIP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_INTERNAL_COMMON_H_
#define DOIP_INTERNAL_COMMON_H_

/* =============================================== version information ============================================== */

#include "DoIP.h"
#include "SoAd.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ===================================================== macros ===================================================== */

#define DOIP_RX_BUFFER_SIZE (DOIP_MAX_REQUEST_BYTES + DOIP_HEADER_LEN)  /**< Rx Buffer size */
#define DOIP_TX_BUFFER_SIZE (DOIP_MAX_RESPONSE_BYTES + DOIP_HEADER_LEN) /**< Tx Buffer size */

#define DOIP_INVALID_UINT8  0xFFu       /**< Invalid value */
#define DOIP_INVALID_UINT16 0xFFFFu     /**< Invalid value */
#define DOIP_INVALID_UINT32 0xFFFFFFFFu /**< Invalid value */

/** DoIP header */
#define DOIP_PROTOCOL_VERSION         0x02u /**< Version */
#define DOIP_PROTOCOL_INVERSE_VERSION 0xFDu /**< Inverse version */

#define DOIP_HEADER_LEN 8u /**< Header len */

#define DOIP_HEADER_FIELD_POSITION_VERSION         0u /**< Location of version field  */
#define DOIP_HEADER_FIELD_POSITION_INVERSE_VERSION 1u /**< Location of inverse version field */
#define DOIP_HEADER_FIELD_POSITION_PAYLOAD_TYPE    2u /**< Location of payload type field */
#define DOIP_HEADER_FIELD_POSITION_PAYLOAD_LEN     4u /**< Location of pyaload length field */

#define DOIP_HEADER_NACK_INCORRECT_PATTERN_FORMAT 0x00u /**< Incorrect pattern format */
#define DOIP_HEADER_NACK_UNKNOWN_PAYLOAD_TYPE     0x01u /**< Unknown payload type */
#define DOIP_HEADER_NACK_MESSAGE_TO_LARGE         0x02u /**< Message too large */
#define DOIP_HEADER_NACK_OUT_OF_MEMORY            0x03u /**< Out of memory */
#define DOIP_HEADER_NACK_INVALID_PAYLOAD_LENGTH   0x04u /**< Invalid payload length */

/**
 * @name DoIP payload types
 * @{
 * @brief  DoIP payload types
 */
#define DOIP_MSG_TYPE_GENERIC_DOIP_HEADER_NACK    0x0000u /**< Generic DoIP header negative acknowledge */
#define DOIP_MSG_TYPE_IDENTIFICATION_REQ          0x0001u /**< Vehicle identification request message */
#define DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_EID 0x0002u /**< Vehicle identification request message with EID */
#define DOIP_MSG_TYPE_IDENTIFICATION_REQ_WITH_VIN 0x0003u /**< Vehicle identification request message with VIN */
#define DOIP_MSG_TYPE_ANNOUNCEMENT_OR_IDENTIFICATION_RSP \
    0x0004u /**< Vehicle announcement message/vehicle identification response message */
#define DOIP_MSG_TYPE_ROUTING_ACTIVATION_REQ 0x0005u /**< Routing activation request */
#define DOIP_MSG_TYPE_ROUTING_ACTIVATION_RSP 0x0006u /**< Routing activation response */
#define DOIP_MSG_TYPE_ALIVE_CHECK_REQ        0x0007u /**< Alive check request */
#define DOIP_MSG_TYPE_ALIVE_CHECK_RSP        0x0008u /**< Alive check response */
#define DOIP_MSG_TYPE_ENTITY_STATUS_REQ      0x4001u /**< DoIP entity status request */
#define DOIP_MSG_TYPE_ENTITY_STATUS_RSP      0x4002u /**< DoIP entity status response */
#define DOIP_MSG_TYPE_POWER_MODE_REQ         0x4003u /**< Diagnostic power mode information request */
#define DOIP_MSG_TYPE_POWER_MODE_RSP         0x4004u /**< Diagnostic power mode information response */
#define DOIP_MSG_TYPE_DIAG                   0x8001u /**< Diagnostic message */
#define DOIP_MSG_TYPE_DIAG_ACK               0x8002u /**< Diagnostic message positive acknowledgement */
#define DOIP_MSG_TYPE_DIAG_NACK              0x8003u /**< Diagnostic message negative acknowledgement */
/** @} */

/** connect */
#define DOIP_SOCKET_AUTO_OPEN STD_ON /**< Whether the socket is automatically opened when the socket is closed */

/** UDP & TCP nack message */
#define DOIP_NACK_MSG_FIELD_POSITION_CODE 8u /**< Location of NACK code */
#define DOIP_NACK_MSG_PAYLOAD_LEN         1u /**< Length of NACK message payload */
#define DOIP_NACK_MSG_LEN                 9u /**< Length NACK message */

#if !defined(DOIP_LOCAL_INLINE)
#define DOIP_LOCAL_INLINE static inline /**< Used for define local inline functions */
#endif

#if !defined(DOIP_LOCAL)
#define DOIP_LOCAL static /**< Used for define local variables */ /* PRQA S 3414 */ /* VL_DoIP_3414 */
#endif

/** This macro definition represents the DoIP module unused parameter */
#if !defined(DOIP_PARAM_UNUSED)
#define DOIP_PARAM_UNUSED(x) (void)(x)
#endif

/* ================================================ type definitions ================================================ */

/**
 * @brief Status of DoIP.
 */
typedef enum
{
    DOIP_STATUS_INITED,  /**< DoIP initialized @range 0 */
    DOIP_STATUS_UNINITED /**< DoIP uninitialized @range 1*/
} DOIP_InitStatusType;

/* =========================================== external data declarations =========================================== */

/**
 * @brief Status of DoIP
 *        @range 0..1
 */
extern DOIP_InitStatusType DoIP_InitStatus;

/**
 * @brief PC configuration
 *        @range NA
 */
extern const DoIP_PCConfigType* DoIP_PCCfgPtr;

/**
 * @brief PB configuration
 *        @range NA
 */
extern const DoIP_PBConfigType* DoIP_PBCfgPtr;

/**
 * @brief Callbacks in DoIPGeneral configuration
 *        @range NA
 */
extern const DoIP_GeneralCfgType DoIP_General;

/**
 * @brief GID synchronization Status
 *        @range 0..1
 */
extern boolean DoIP_GidSynStatus;

/* ========================================= external function declarations ========================================= */

/**
 * @brief       Returns the smaller value in the parameter.
 * @param[in]   x: Participate in comparison.
 * @param[in]   y: Participate in comparison.
 * @return      PduLengthType: return x or y
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
PduLengthType DOIP_MIN(PduLengthType x, PduLengthType y);

/**
 * @brief       Check version field.
 * @param[in]   header: Header of DoIP message.
 * @param[in]   sduLen: Message len of received.
 * @return      boolean
 * @retval      TRUE: valid
 * @retval      FALSE: Invalid
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DOIP_GENERICHEADER_CHECK(const uint8* header, PduLengthType sduLen);

/**
 * @brief       transfer byte to short.
 * @param[in]   array: The data being transfered.
 * @return      uint16: return new value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
uint16 DOIP_U8_2_U16(const uint8* array);

/**
 * @brief       transfer byte to int.
 * @param[in]   array: The data being transfered.
 * @return      uint32: return new value
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
uint32 DOIP_U8_2_U32(const uint8* array);

/**
 * @brief       transfer short to byte.
 * @param[out]  array: Save new value.
 * @param[in]   data: The data being transfered.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_u16_2_u8(uint8* array, uint16 data);

/**
 * @brief       transfer int to byte.
 * @param[out]  array: Save new value.
 * @param[in]   data: The data being transfered.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_u32_2_u8(uint8* array, uint32 data);

/**
 * @brief       Copy data.
 * @param[out]  dest: Destination message.
 * @param[in]   src: Memory for providing data.
 * @param[in]   len: Specifies the amount of copied data.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_MemCpy(uint8* dest, const uint8* src, PduLengthType len);

/**
 * @brief       Open Or close connection.
 * @param[in]   soadTxPduRef: Open/close the associated socket.
 * @param[in]   assignment: Whether to request assignment ip address.
 * @param[in]   isOpen: TRUE: open, FALSE: close.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_SwitchConnection(uint16 soadTxPduRef, boolean assignment, boolean isOpen);

/**
 * @brief       Close connection.
 * @param[in]   soConId: Close specified socket.
 * @param[in]   isAbort: Whether to open sockets automatically.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_CloseSoCon(SoAd_SoConIdType soConId, boolean isAbort);

#ifdef __cplusplus
}
#endif
#endif
