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
 **  @file               : SoAd_Cbk.h
 **  @author             : zhengfei.li
 **  @date               : 2020/05/07
 **  @vendor             : isoft
 **  @description        : Socket Adaptor callback function header
 **
 ***********************************************************************************************************************/

#ifndef SOAD_CBK_H_
#define SOAD_CBK_H_

/* =================================================== inclusions =================================================== */
#include "SoAd_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================= external function declarations ========================================= */

/**
 * @brief         The TCP/IP stack calls this primitive after the reception of data on a socket. The socket identifier
 *                along with configuration information determines which module is to be called.
 *
 * @param[in]     SocketId: Socket identifier of the related local socket resource.
 * @param[in]     RemoteAddrPtr: Pointer to memory containing IP address and port of the
 * @param[in]     BufPtr: Pointer to the received data
 * @param[in]     Length: Data length of the received TCP segment or UDP datagram
 * @reentrant     Reentrant for different SocketIds. Non reentrant for the same SocketId.
 * @synchronous   TRUE
 * @trace         CPD-74648
 */
void SoAd_RxIndication(
    TcpIp_SocketIdType        SocketId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    const uint8*              BufPtr,
    uint16                    Length);

/**
 * @brief         This service requests to copy data for transmission to the buffer indicated. This call is triggered
 *                by TcpIp_Transmit(). Note: The call to <Up>_CopyTxData() may happen in the context of TcpIp_Transmit()
 *
 * @param[in]     SocketId: Socket identifier of the related local socket resource
 * @param[in]     BufPtr: Pointer to buffer for transmission data
 * @param[in]     BufLength: Length of provided data buffer
 * @return        BufReq_ReturnType
 * @retval        BUFREQ_OK: Data has been copied to the transmit buffer completely as requested
 * @retval        BUFREQ_E_NOT_OK: Data has not been copied. Request failed
 * @reentrant     Reentrant for different SocketIds. Non reentrant for the same SocketId
 * @synchronous   TRUE
 * @trace         CPD-74649
 */
BufReq_ReturnType SoAd_CopyTxData(TcpIp_SocketIdType SocketId, uint8* BufPtr, uint16 BufLength);

/**
 * @brief         The TCP/IP stack calls this function after the data has been acknowledged by the peer for TCP.
 *                Caveats: The upper layer might not be able to determine exactly which data bytes have been confirmed
 *
 * @param[in]     SocketId: Socket identifier of the related local socket resource.
 * @param[in]     Length: Number of transmitted data bytes.
 * @reentrant     Reentrant for different SocketIds. Non reentrant for the same SocketId.
 * @synchronous   TRUE
 * @trace         CPD-74650
 */
void SoAd_TxConfirmation(TcpIp_SocketIdType SocketId, uint16 Length);

/**
 * @brief         This service gets called if the stack put a socket into the listen mode before (as server) and a
 *                peer connected to it (as client).In detail: The TCP/IP stack calls this function after a socket was
 *                set into the listen state with TcpIp_TcpListen() and a TCP connection is requested by the peer
 *
 * @param[in]     SocketId: Socket identifier of the related local socket resource which has been used at TcpIp_Bind()
 * @param[in]     SocketIdConnected: Socket identifier of the local socket resource used for the established connection
 * @param[in]     RemoteAddrPtr: IP address and port of the remote host
 * @return        Std_ReturnType
 * @retval        E_OK: upper layer accepts the established connection
 * @retval        E_NOT_OK: upper layer refuses the established
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-74651
 */
Std_ReturnType SoAd_TcpAccepted(
    TcpIp_SocketIdType        SocketId,
    TcpIp_SocketIdType        SocketIdConnected,
    const TcpIp_SockAddrType* RemoteAddrPtr);

/**
 * @brief         This service gets called if the stack initiated a TCP connection before (as client) and the peer
 *                (the server) acknowledged the connection set up.In detail:The TCP/IP stack calls this function after
 *                a socket was requested to connect with TcpIp_TcpConnect() and a TCP connection is confirmed by the
 *                peer.The parameter value of SocketId equals the SocketId value of the preceding TcpIp_TcpConnect()
 *                call.
 *
 * @param[in]     SocketId: Socket identifier of the related local socket resource
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-74652
 */
void SoAd_TcpConnected(TcpIp_SocketIdType SocketId);

/**
 * @brief         This service gets called if the stack encounters a condition described by the values in Event.
 *
 * @param[in]     SocketId: Socket identifier of the related local socket resource
 * @param[in]     Event: This parameter contains a description of the event just encountered
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-74653
 */
void SoAd_TcpIpEvent(TcpIp_SocketIdType SocketId, TcpIp_EventType Event);

/**
 * @brief         This service gets called by the TCP/IP stack if an IP address assignment changes (i.e. new address
 *                assigned or assigned address becomes invalid).
 *
 * @param[in]     IpAddrId: IP address Identifier, representing an IP address specified in the TcpIp module
 *                configuration (e.g. static IPv4 address on EthIf controller 0)
 * @param[in]     State: state of IP address assignment
 * @reentrant     Non Reentrant
 * @synchronous   TRUE
 * @trace         CPD-74654
 */
void SoAd_LocalIpAddrAssignmentChg(TcpIp_LocalAddrIdType IpAddrId, TcpIp_IpAddrStateType State);

#ifdef __cplusplus
}
#endif

#endif
