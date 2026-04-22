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
 **  @file               : DoIP_Internal_UDP.h
 **  @author             : hao.wen
 **  @date               : 2024/09/08
 **  @vendor             : isoft
 **  @description        : Implement the UDP part of DoIP
 **
 ***********************************************************************************************************************/

#ifndef DOIP_INTERNAL_UDP_H_
#define DOIP_INTERNAL_UDP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================ type definitions ================================================ */

/**
 * @brief UDP connection status
 */
typedef struct
{
    uint16             SoadTxPduId; /**< DoIPSoAdTxPduId @range 0..65535*/
    SoAd_SoConIdType   SoConId;     /**< ID of socket @range 0..255 */
    SoAd_SoConModeType SoConState;  /**< State of connection @range 0..2 */
} DoIP_UdpConnStatusType;

/**
 * @brief Status of announcement
 */
typedef struct
{
    SoAd_SoConIdType   SoConId;    /**< ID of socket @range 0..255 */
    SoAd_SoConModeType SoConState; /**< State of connection @range 0..2 */

    boolean IsAncTimerWork; /**< Whether or not timer is work @range 0..1 */
    uint8   AncCnt;         /**< Number of announcement message @range 0..255 */
    uint32  AncTimer;       /**< Timer of announcement @range 0..0xFFFFFFFF */
} DoIP_VehicleAncConnStatusType;

/* =========================================== external data declarations =========================================== */

/**
 * @brief Status of UDP connection
 *        @range NA
 */
extern DoIP_UdpConnStatusType DoIP_UdpConnStatus[DOIP_UDP_MAX_CONNECTION];

/**
 * @brief Status of announcement connection
 *        @range NA
 */
extern DoIP_VehicleAncConnStatusType DoIP_AncConnStatus[DOIP_ANNOUNCEMENT_MAX_CONNECTION]; /**<  */

/* ========================================= external function declarations ========================================= */

/* connect & connect's status*/

/**
 * @brief       Open or close Udp connection.
 * @param[inout] active: Specifies open or close,and return status of connectin.
 * @return      boolean
 * @retval      TRUE: Switch success.
 * @retval      FALSE: Switch fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_SwitchUdp(const boolean* active);

/**
 * @brief       Open or close announcement connection.
 * @param[inout] active: Specifies open or close,and return status of connectin.
 * @return      boolean
 * @retval      TRUE: Switch success.
 * @retval      FALSE: Switch fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
boolean DoIP_SwitchAnc(const boolean* active);

/**
 * @brief       Notify status of UDP.
 * @param[in]   soConId: Specifies connection.
 * @param[in]   mode: Status of connection.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SoConModeChgUdp(SoAd_SoConIdType soConId, SoAd_SoConModeType mode);

/**
 * @brief       Notify status of announcement connection.
 * @param[in]   soConId: Specifies connection.
 * @param[in]   mode: Status of connection.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SoConModeChgAnc(SoAd_SoConIdType soConId, SoAd_SoConModeType mode);

/**
 * @brief       Reset of DoIP_UdpConnStatus.
 * @param[in]   index: Index of DoIP_UdpConnStatus.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_ResetUdpConnStatus(uint8 index);

/* send & rcv*/

/**
 * @brief       Send UDP NACK.
 * @param[in]   soadTxPduRef: Specifies the connection to send the message.
 * @param[in]   nack: NACK code.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_SendUdpNack(PduIdType soadTxPduRef, uint8 nack);

/**
 * @brief       Get SoadTxPduRef and SoConId.
 * @param[in]   soadRxPduId: Specify which pdu'ref to get.
 * @param[out]  soadTxPduRef: SoadTxPduRef.
 * @param[out]  soConId: ID of socket
 * @return      Std_ReturnType
 * @retval      E_OK: Find success.
 * @retval      E_NOT_OK: Find fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_GetSoAdTxPduRefAndSoConId(PduIdType soadRxPduId, uint16* soadTxPduRef, SoAd_SoConIdType* soConId);

/**
 * @brief       Get SoadTxPduRef and SoConId.
 * @param[in]   pduInfoPtr: Specify which pdu'ref to get.
 * @param[in]   soadTxPduRef: soadTxPduRef.
 * @param[in]   soConId: ID of socket.
 * @return      Std_ReturnType
 * @retval      E_OK: Valid message.
 * @retval      E_NOT_OK: Invalid message.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
Std_ReturnType DoIP_CheckUdpMsgPayload(const PduInfoType* pduInfoPtr, uint16 soadTxPduRef, SoAd_SoConIdType soConId);

/**
 * @brief       Handle received IF message.
 * @param[in]   soadTxPduRef: Specifies the connection to receive the message.
 * @param[in]   soConId: ID of socket.
 * @param[in]   payloadLen: Length fo payload.
 * @param[in]   pduInfoPtr: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength).
 * @return      BufReq_ReturnType
 * @retval      BUFREQ_OK: Success.
 * @retval      BUFREQ_E_NOT_OK: Fail.
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_HandleIfMsg(
    PduIdType          soadTxPduRef,
    SoAd_SoConIdType   soConId,
    uint32             payloadLen,
    const PduInfoType* pduInfoPtr);

/* timer */

/**
 * @brief       Handle timers.
 * @return      void
 * @reentrant   TRUE
 * @synchronous TRUE
 * @trace       CPD-PLACEHOLDER
 */
void DoIP_HandleVehicleAncTimeout(void);

#ifdef __cplusplus
}
#endif
#endif
