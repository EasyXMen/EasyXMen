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
**************************************************************************** **
**                                                                           **
**  FILENAME    : SoAd_Cbk.c                                                 **
**                                                                           **
**  Created on  :                                                            **
**  Author      : zhengfei.li                                                **
**  Vendor      :                                                            **
**  DESCRIPTION : SoAd call-back function define                             **
**                                                                           **
**  SPECIFICATION(S) :   AUTOSAR classic Platform 4.2.2 and R19_11           **
**                                                                           **
**************************************************************************** */
#ifndef SOAD_CBK_H_
#define SOAD_CBK_H_

#include "SoAd_Cfg.h"

/*============================================================================*/
/*
Service ID[hex]: 0x12
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
RemoteAddrPtr Pointer to memory containing IP address and port of the
remote host which sent the data.
BufPtr Pointer to the received data.
Length Data length of the received TCP segment or UDP datagram.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: The TCP/IP stack calls this primitive after the reception of data on a socket. The
socket identifier along with configuration information determines which module is
to be called.
 */
extern FUNC(void, SOAD_CALLBACK_CODE) SoAd_RxIndication(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) Length);

/*============================================================================*/
/*
Service ID[hex]: 0x13
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
BufPtr Pointer to buffer for transmission data.
BufLength Length of provided data buffer.
Parameters (inout): None
Parameters (out): None
Return value: BufReq_ReturnType BUFREQ_OK: Data has been copied to the transmit buffer
completely as requested.
BUFREQ_E_NOT_OK: Data has not been copied. Request
failed. (No further action for TcpIp required. Later the upper
layer might either close the socket or retry the transmit request)
Description: This service requests to copy data for transmission to the buffer indicated.
This call is triggered by TcpIp_Transmit(). Note: The call to <Up>_CopyTxData()
may happen in the context of TcpIp_Transmit().
 */
extern FUNC(BufReq_ReturnType, SOAD_CALLBACK_CODE) SoAd_CopyTxData(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
    VAR(uint16, AUTOMATIC) BufLength);

/*============================================================================*/
/*
Service ID[hex]: 0x14
Sync/Async: Synchronous
Reentrancy: Reentrant for different SocketIds. Non reentrant for the same SocketId.
Parameters (in): SocketId Socket identifier of the related local socket resource.
Length Number of transmitted data bytes.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: The TCP/IP stack calls this function after the data has been acknowledged by the
peer for TCP.
Caveats: The upper layer might not be able to determine exactly which data bytes
have been confirmed.
 */
extern FUNC(void, SOAD_CALLBACK_CODE)
    SoAd_TxConfirmation(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(uint16, AUTOMATIC) Length);

/*============================================================================*/
/*
Service ID[hex]: 0x15
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource which
has been used at TcpIp_Bind()
SocketIdConnected Socket identifier of the local socket resource used for the
established connection.
RemoteAddrPtr IP address and port of the remote host.
Parameters (inout): None
Parameters (out): None
Return value: Std_ReturnType Result of operation
E_OK upper layer accepts the established connection
E_NOT_OK upper layer refuses the established
connection, TcpIp stack shall close the connection.
Description: This service gets called if the stack put a socket into the listen mode before (as
server) and a peer connected to it (as client).
In detail: The TCP/IP stack calls this function after a socket was set into the listen
state with TcpIp_TcpListen() and a TCP connection is requested by the peer.
 */
extern FUNC(Std_ReturnType, SOAD_CALLBACK_CODE) SoAd_TcpAccepted(
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId,
    VAR(TcpIp_SocketIdType, AUTOMATIC) SocketIdConnected,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_CONST) RemoteAddrPtr);

/*============================================================================*/
/*
Service ID[hex]: 0x16
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called if the stack initiated a TCP connection
before (as client) and the peer (the server) acknowledged the connection set up.
In detail:
The TCP/IP stack calls this function after a socket was requested to connect with
TcpIp_TcpConnect() and a TCP connection is confirmed by the peer.
The parameter value of SocketId equals the SocketId value of the preceeding
TcpIp_TcpConnect() call.
 */
extern FUNC(void, SOAD_CALLBACK_CODE) SoAd_TcpConnected(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId);

/*============================================================================*/
/*
Service ID[hex]: 0x17
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in): SocketId Socket identifier of the related local socket resource.
Event This parameter contains a description of the event just encountered.
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called if the stack encounters a condition described by the
values in Event.
 */
extern FUNC(void, SOAD_CALLBACK_CODE)
    SoAd_TcpIpEvent(VAR(TcpIp_SocketIdType, AUTOMATIC) SocketId, VAR(TcpIp_EventType, AUTOMATIC) Event);

/*============================================================================*/
/*
Service ID[hex]: 0x18
Sync/Async: Synchronous
Reentrancy: Non Reentrant
Parameters (in):
IpAddrId IP address Identifier, representing an IP address specified in the TcpIp
module configuraiton (e.g. static IPv4 address on EthIf controller 0).
State state of IP address assignment
Parameters (inout): None
Parameters (out): None
Return value: None
Description: This service gets called by the TCP/IP stack if an IP address assignment changes
(i.e. new address assigned or assigned address becomes invalid).
 */
extern FUNC(void, SOAD_CALLBACK_CODE) SoAd_LocalIpAddrAssignmentChg(
    VAR(TcpIp_LocalAddrIdType, AUTOMATIC) IpAddrId,
    VAR(TcpIp_IpAddrStateType, AUTOMATIC) State);

#endif /* SOAD_CBK_H_ */
