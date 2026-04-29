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
 **  @file               : SoAd.h
 **  @author             : zhengfei.li
 **  @date               : 2020/05/07
 **  @vendor             : isoft
 **  @description        : Socket Adaptor header
 **
 ***********************************************************************************************************************/

/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V02.00.00 2020-05-07  zhengfei.li   Initial version
 *                                      (Upgrade according to the R19_11 standards)
 *  V02.00.01 2021-11-08  zhengfei.li   Set up administrative data before sending TCP message(Avoid TxConfirmation
 *                                      coming early.)
 *  V02.00.02 2022-01-04  zhengfei.li   Udp status management parameters are set first and then notified to the upper
 *                                      layer. SoAdCopyTxData management variables plusinterrupt protection.
 *  V02.00.03 2022-01-06  zhengfei.li   SoAd_TxConfirmation invalid SoCon protection.
 *  V02.00.04 2022-06-10  darren.zhang  SoAd_TcpIpEvent reentrt modify,reslove bsd tcp server socket repeat close
 *  V02.00.05 2023-08-03  fupeng.yu     SoAd_HeaderEnableIfPduHandle modify the logic of copying SoAd_PduBuffer to
 *                                      SoAd_IfPduBuffer. resolve data of rxindication is not a correct
 *  V02.00.06 2023-08-21  fupeng.yu     When the received IF UDP data with headerid is processing, the release of the
 *                                      remote address should be locked
 *  V02.00.07 2023-11-24  fupeng.yu     Performance optimization
 *  V02.00.08 2023-12-25  fupeng.yu     add logic of event with TCPIP_TCP_RESET for socked connect group in
 *                                      SoAd_TcpIpEvent
 *  V02.00.09 2024-02-02  fupeng.yu     In SoAd_TcpIpEvent modify close_socket logic for received TCPIP_TCP_FIN_RECEIVED
 *                                      of TCP when TP data of received not all copy to upper layer
 *  V02.00.10 2024-02-29  fupeng.yu     1. resolve QAC
 *                                      2. used istd_lib API
 *                                      3. In SoAd_HeaderEnableNewPduHandle, resolve TCP connection cannot close
 *                                         when TCP receives TCPIP-TCP-FIN and the part of PDU
 *  V02.00.11 2024-03-20  fupeng.yu     resolve running exception in SoAd_FindTheValidSoCon
 *  V02.00.12 2024-05-01  fupeng.yu     1.add feature of udp immediate txconfirmation when The same pdu is sent multiple
 *                                        times.
 *                                      2.When the received IF UDP data with multiple headerid is processing, the
 *                                        release of the remote address should be locked.
 *  V02.00.13 2024-07-26  fupeng.yu     1.add feature of udp which release state of txconfirmation when The same pdu is
 *                                        sent multiple times and <up_>txconfirmation is not configured.
 *  V02.00.14 2024-08-28  fupeng.yu     Add exception handling for SoAd_RxBufCfgIndexPtr in
 *                                      SoAd_HeaderEnableTcpSoConRxHandle
 *  V02.00.15 2024-08-30  fupeng.yu     Resolve compilation errors when no SocketRoute is configured.
 *  V02.01.00 2025-01-18  fupeng.yu     Support R23-11.
 *  V02.02.00 2025-04-29  fupeng.yu     optimized code.
 *  V02.02.01 2025-09-28  fupeng.yu     1. Add ExclusiveArea the logic for Tp transmit.
 *                                      2. Fix thg flag of remote address setted in SoAd_ReleaseRemoteAddrHandle.
 *  V02.02.02 2025-10-24  fupeng.yu     1. Fix incorrect pduRouteDestId in SoAd_IfTransmit (CPT-16146).
 *                                      2. Fix update tcp windown for received Tp data when close socket (CPT-16242).
 *  V02.02.03 2026-01-19  fupeng.yu     Resolve the inconsistency between read and write positions in the rxbuffer.
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:LdCom<br>
  RuleSorce:puhua_rules-2024.12.rcf

    \li VL_SoAd_4603AbortSameName
      Reason: Definition of API function SoAd_CloseSoCon SWS_SoAd_00511.
      Risk:No risk.
      Prevention:test pass.

    \li VL_SoAd_0793
      Reason: Dependent on configuration.
      Risk: No risk
      Prevention: Configure shorter names as much as possible

    \li VL_SoAd_0311
      Reason: The cast conversion is safe.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_SoAd_0334
      Reason: The Parameter "abort"  has the same symbol as in the common namespace of the C standard.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_SoAd_3415
      Reason: Right hand operand of '&&' or '||' is an expression with persistent side effects.
      Risk: No risk.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_SoAd_1536
      Reason: The tag '%1s' is declared but not used within this project.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SoAd_3213
      Reason: The tag '%1s' is not used and could be removed.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SoAd_1512
      Reason: Identifier '${name}' with external linkage has separate declarations in multiple translation units.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_SoAd_1513
      Reason: Identifier '${name}' with external linkage has separate non-defining declarations in more than one
              location.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

    \li VL_MTR_SoAd_STMIF
      Reason: Functions handle specific tasks in the overall component behavior; Tasks have different scenarios
  to cover, depending on local conditions - this leads to deep nesting of control structures. Since there is more
  common code than different code, a higher nesting level is acceptable to keep the code.
      Risk: The code is hard to maintain.
      Prevention: Ensure that the project is working properly through unit testing.

    \li VL_MTR_SoAd_STTPP
      Reason: The total number of variables is related to user configuration, allowing dynamic code to violate
  this under extreme conditions.
      Risk: Code readability will decrease.
      Prevention: None.

    \li VL_MTR_SoAd_STVAR
      Reason: The total number of variables is related to user configuration, allowing dynamic code to violate
  this under extreme conditions.
      Risk: The code is hard to maintain.
      Prevention: Design and code review + clearly structured and commented code.

    \li VL_MTR_SoAd_STCYC
      Reason:  No separation of functionality into sub-functions because there is a higher requirement to minimize
  the stack and runtime usage applied to the code.
      Risk: Understandability and testability can become too complex.
      Prevention: Design and code review + clearly structured and commented code.

    \li VL_MTR_SoAd_STCAL
      Reason:  The software structure is defined by the AUTOSAR standard. The standard compliance vote is above
  the measurement threshold. In addition, a typical way to reduce STCAL is to deepen function nesting, which increases
  the use of the call stack and runtime.
      Risk: Because of fanning out to many functions, understandability and testability may become overly complex.
      Prevention: Design and code review + clearly structured and commented code.

    \li VL_MTR_SoAd_STPTH
      Reason:  No separation of functionality into sub-functions because there is a higher requirement to minimize
  the stack and runtime usage applied to the code.
      Risk: Understandability and testability can become too complex.
      Prevention: Design and code review + clearly structured and commented code.

 */
/* PRQA S 1512,1513 EOF */ /* VL_SoAd_1512,VL_SoAd_1513 */
#ifndef SOAD_H_
#define SOAD_H_

/* =================================================== inclusions =================================================== */
#include "SoAd_Types.h"
#include "SoAd_Cfg.h"
#include "SoAd_PBcfg.h"
#include "SchM_SoAd.h"
#ifdef __cplusplus
extern "C" {
#endif

/* =============================================== version information ============================================== */
/**
 * @name SOAD_VERSION_GROUP
 * @{
 * @brief SoAd module Published information
 */
#define SOAD_AR_RELEASE_MAJOR_VERSION 4u /**< SoAd autosar major version*/
#define SOAD_AR_RELEASE_MINOR_VERSION 9u /**< SoAd autosar minor version*/
#define SOAD_AR_RELEASE_PATCH_VERSION 0u /**< SoAd autosar patch version*/

#define SOAD_SW_MAJOR_VERSION 2u /**< SoAd software major version*/
#define SOAD_SW_MINOR_VERSION 2u /**< SoAd software minor version*/
#define SOAD_SW_PATCH_VERSION 2u /**< SoAd software patch version*/
/* ===================================================== macros ===================================================== */
#define SOAD_VENDOR_ID 62u /**< SoAd vendor id*/
#define SOAD_MODULE_ID 56u /**< SoAd module id*/
#define SOAD_INSTANCE  0u  /**< SoAd instance id*/
/** @} */

/**
 * @name SOAD_SERVICEID_GROUP
 * @{
 * @brief Service IDs definition
 */
#define SOAD_SID_INIT                           0x01u /**< SoAd service id of Iint*/
#define SOAD_SID_GETVERSIONINFO                 0x02u /**< SoAd service id of GetVersion*/
#define SOAD_SID_IFTRANSMIT                     0x03u /**< SoAd service id of IfTransmit*/
#define SOAD_SID_IFROUTINGGROUPTRANSMIT         0x1Du /**< SoAd service id of IfRoutingGroupTransmit*/
#define SOAD_SID_IFSPECIFICROUTINGGROUPTRANSMIT 0x1Fu /**< SoAd service id of IfSpecificRoutingGroupTransmit*/
#define SOAD_SID_TPTRANSMIT                     0x04u /**< SoAd service id of TpTransmit*/
#define SOAD_SID_TPCANCELTRANSMIT               0x05u /**< SoAd service id of TpCancelTransmit*/
#define SOAD_SID_TPCANCELRECEIVE                0x06u /**< SoAd service id of TpCancelReceive*/
#define SOAD_SID_GETSOCONID                     0x07u /**< SoAd service id of GetSoConId*/
#define SOAD_SID_OPENSOCON                      0x08u /**< SoAd service id of OpenSoCon*/
#define SOAD_SID_CLOSESOCON                     0x09u /**< SoAd service id of CloseSoCon*/
#define SOAD_SID_REQUESTIPADDRASSIGNMENT        0x0Au /**< SoAd service id of RequestIpAddrAssignment*/
#define SOAD_SID_RELEASEIPADDRASSIGNMENT        0x0Bu /**< SoAd service id of ReleaseIpAddrAssignment*/
#define SOAD_SID_GETLOCALADDR                   0x0Cu /**< SoAd service id of GetLocalAddr*/
#define SOAD_SID_GETPHYSADDR                    0x0Du /**< SoAd service id of GetPhysAddr*/
#define SOAD_SID_GETREMOTEADDR                  0x1Cu /**< SoAd service id of GetRemoteAddr*/
#define SOAD_SID_ENABLEROUTING                  0x0Eu /**< SoAd service id of EnableRouting*/
#define SOAD_SID_ENABLESPECIFICROUTING          0x20u /**< SoAd service id of EnableSpecificRouting*/
#define SOAD_SID_DISABLEROUTING                 0x0Fu /**< SoAd service id of DisableRouting*/
#define SOAD_SID_DISABLESPECIFICROUTING         0x21u /**< SoAd service id of DisableSpecificRouting*/
#define SOAD_SID_SETREMOTEADDR                  0x10u /**< SoAd service id of SetRemoteAddr*/
#define SOAD_SID_SETUNIQUEREMOTEADDR            0x1Eu /**< SoAd service id of SetUniqueRemoteAddr*/
#define SOAD_SID_TPCHANGEPARAMETER              0x11u /**< SoAd service id of TpChangeParameter*/
#define SOAD_SID_READDHCPHOSTNAMEOPTION         0x1Au /**< SoAd service id of ReadDhcpHostNameOption*/
#define SOAD_SID_WRITEDHCPHOSTNAMEOPTION        0x1Bu /**< SoAd service id of WriteDhcpHostNameOption*/
#define SOAD_SID_RXINDICATION                   0x12u /**< SoAd service id of RxIndication*/
#define SOAD_SID_COPYTXDATA                     0x13u /**< SoAd service id of CopyTxData*/
#define SOAD_SID_TXCONFIRMATION                 0x14u /**< SoAd service id of TxConfirmation*/
#define SOAD_SID_TCPACCEPTED                    0x15u /**< SoAd service id of TCPAccepted*/
#define SOAD_SID_TCPCONNECTED                   0x16u /**< SoAd service id of TCPConnected*/
#define SOAD_SID_TCPIPEVENT                     0x17u /**< SoAd service id of TCPIpEvent*/
#define SOAD_SID_LOCALIPADDRASSIGNMENTCHG       0x18u /**< SoAd service id of LocalIpAddrAssignmentChg*/
#define SOAD_SID_MAINFUNCTION                   0x19u /**< SoAd service id of MainFunction*/
#define SOAD_SID_GETSOCONMODE                   0x22u /**< SoAd service id of GetSoConMode*/
#define SOAD_SID_RELEASEREMOTEADDR              0x23u /**< SoAd service id of ReleaseRemoteAddr*/
#define SOAD_SID_ISCONNECTIONREADY              0x55u /**< SoAd service id of IsConnectionReady*/
/** @} */

/**
 * @name SOAD_DET_ERROR_CODES_GROUP
 * @{
 * @brief DET error codes
 */
/* DET error codes */
#if (STD_ON == SOAD_DEV_ERROR_DETECT)
#define SOAD_E_NOTINIT           0x01u          /**< API service called before initializing the module*/
#define SOAD_E_PARAM_POINTER     0x02u          /**< API service called with NULL pointer*/
#define SOAD_E_INV_ARG           0x03u          /**< Invalid argument*/
#define SOAD_E_INV_PDUID         0x06u          /**< Invalid PDU ID*/
#define SOAD_E_INV_SOCKETID      0x07u          /**< Invalid socket address*/
#define SOAD_E_INIT_FAILED       0x08u          /**< Invalid configuration set selection*/
#define SOAD_E_INV_METADATA      0x09u          /**< Invalid meta data */
#define SOAD_E_INVALID_PARTITION ((uint8)0x80u) /**< Invalid partition ID*/
#endif                                          /*STD_ON == SOAD_DEV_ERROR_DETECT*/
/** @} */

/**
 * @name SOAD_DET_ERROR_CODES_GROUP
 * @{
 * @brief DET error codes
 */
/* DET error codes */
#define SOAD_E_NOBUFS                 0x04u /**< No buffer space available*/
#define SOAD_E_INV_PDUHEADER_ID       0x05u /**< Unknown PduHeader ID*/
#define SOAD_E_TCP_AUTOCONNECT_FAILED 0x10u /**< Automatic TCP connection failed*/
/** @} */
/* ================================================ type definitions ================================================ */

/* ========================================== internal function definitions ========================================= */

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief         Initializes internal and external interfaces and variables of SoAd module.
 * @param[in]     SoAdConfigPtr: Pointer to the configuration data of the SoAd module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-71954
 */
void SoAd_Init(const SoAd_ConfigType* SoAdConfigPtr);

#if (STD_ON == SOAD_VERSION_INFO_API)
/**
 * @brief         Returns the version information of SoAd module.
 * @param[out]    versioninfo: Pointer to where to store the version information of this module.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71955
 */
void SoAd_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/**
 * @brief         Triggers the transmission of all If-TxPDUs identified by the parameter id after requesting the data
 *                from the related upper layer.
 * @param[in]     id: routing group identifier indirectly specifying PDUs to be transmitted (after requesting the
 *                    newest data from the related upper layer).
 * @return        Std_ReturnType
 * @retval        E_OK     : request was successful.
 * @retval        E_NOT_OK : request was not successful.
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-71956
 */
Std_ReturnType SoAd_IfRoutingGroupTransmit(SoAd_RoutingGroupIdType id);

/**
 * @brief         Triggers the transmission of all If-TxPDUs identified by the parameter id on the socket connection
 *                specified by SoConId after requesting the data from the related upper layer.
 * @param[in]     id: routing group identifier indirectly specifying PDUs to be transmitted (after requesting the
 *                    newest data from the related upper layer).
 * @param[in]     SoConId: socket connection index specifying the socket connection on which the PDUs shall be
 *                         transmitted.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-71957
 */
Std_ReturnType SoAd_IfSpecificRoutingGroupTransmit(SoAd_RoutingGroupIdType id, SoAd_SoConIdType SoConId);

/**
 * @brief         Returns socket connection index related to the specified TxPduId.
 * @param[in]     TxPduId: Transmit PduId specifying the SoAd socket connection for which the socket connection index
 *                         shall be returned.
 * @param[out]    SoConIdPtr: Pointer to memory receiving the socket connection index asked for.
 * @return        Std_ReturnType
 * @retval        E_OK     : request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71958
 */
Std_ReturnType SoAd_GetSoConId(PduIdType TxPduId, SoAd_SoConIdType* SoConIdPtr);

/**
 * @brief         This service opens the socket connection specified by SoConId
 * @param[in]     SoConId: socket connection index specifying the socket connection which shall be opened
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-71959
 */
Std_ReturnType SoAd_OpenSoCon(SoAd_SoConIdType SoConId);

/**
 * @brief         This service closes the socket connection specified by SoConId.
 * @param[in]     SoConId: socket connection index specifying the socket connection which shall be closed.
 * @param[in]     abort: TRUE-socket connection will immediately be terminated.
 *                       FALSE-socket connection will be terminated if no other upper layer is using this socket
 *                       connection.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-71960
 */
/* PRQA S 4604 ++ */ /* VL_MTR_SoAd_STMIF */
Std_ReturnType SoAd_CloseSoCon(SoAd_SoConIdType SoConId, boolean abort);
/* PRQA S 4604 -- */

/**
 * @brief         By this API service the local IP address assignment which shall be used for the socket connection
 *                specified by SoConId is initiated.
 * @param[in]     SoConId: Socket connection index specifying the socket connection for which the IP address
 *                         shall be set
 * @param[in]     Type: Type of IP address assignment which shall be initiated.
 * @param[in]     LocalIpAddrPtr: Pointer to structure containing the IP address which shall be assigned to the EthIf
 *                                controller indirectly specified via SoConId.Note: This parameter is only used in
 *                                case the parameter Type is set to TCPIP_IPADDR_ASSIGNMENT_STATIC, can be set to
 *                                NULL_PTR otherwise.
 * @param[in]     Netmask: Network mask of IPv4 address or address prefix of IPv6 address in CIDR Notation. Note: This
 *                         parameter is only used in case the parameter Type is set to TCPIP_IPADDR_ASSIGNMENT_STATIC.
 * @param[in]     DefaultRouterPtr: Pointer to structure containing the IP address of the default router (gateway)
 *                                  which shall be assigned. Note: This parameter is only used in case the parameter
 *                                  Type is set to TCPIP_IPADDR_ASSIGNMENT_STATIC, can be set to NULL_PTR otherwise.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request has been accepted.
 * @retval        E_NOT_OK : The request has not been accepted.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   FALSE
 * @trace         CPD-71961
 */
Std_ReturnType SoAd_RequestIpAddrAssignment(
    SoAd_SoConIdType           SoConId,
    TcpIp_IpAddrAssignmentType Type,
    const TcpIp_SockAddrType*  LocalIpAddrPtr,
    uint8                      Netmask,
    const TcpIp_SockAddrType*  DefaultRouterPtr);

/**
 * @brief         By this API service the local IP address assignment used for the socket connection specified by
 *                SoConId is released.
 * @param[in]     SoConId: socket connection index specifying the socket connection for which the IP address shall
 *                         be released
 * @return        Std_ReturnType
 * @retval        E_OK     : The request has been accepted.
 * @retval        E_NOT_OK : The request has not been accepted.
 * @reentrant     TRUE
 * @synchronous   FALSE
 * @trace         CPD-71962
 */
Std_ReturnType SoAd_ReleaseIpAddrAssignment(SoAd_SoConIdType SoConId);

/**
 * @brief         Retrieves the local address (IP address and port) actually used for the SoAd socket connection
 *                specified by SoConId, the netmask and default router
 * @param[in]     SoConId: socket connection index representing the SoAd socket connection for which the actual local
 *                         IP address shall be obtained.
 * @param[inout]  LocalAddrPtr: Pointer to a struct where the local address (IP address and port) is stored.
 * @param[out]    NetmaskPtr: Pointer to memory where Network mask of IPv4 address or address prefix of IPv6 address
 *                            in CIDR Notation is stored
 * @param[inout]  DefaultRouterPtr: Pointer to struct where the IP address of the default router (gateway) is stored
 *                                  (struct member "port" is not used and of arbitrary value).
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71963
 */
Std_ReturnType SoAd_GetLocalAddr(
    SoAd_SoConIdType    SoConId,
    TcpIp_SockAddrType* LocalAddrPtr,
    uint8*              NetmaskPtr,
    TcpIp_SockAddrType* DefaultRouterPtr);

/**
 * @brief         Retrieves the physical source address of the EthIf controller used by the SoAd socket connection
 *                specified by SoConId.
 * @param[in]     SoConId: socket connection index representing the SoAd socket connection for which the physical
 *                         source address of the related EthIf controller shall be obtained.
 * @param[out]    PhysAddrPtr: Pointer to the memory where the physical source address (MAC address) in network byte
 *                             order is stored.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71964
 */
Std_ReturnType SoAd_GetPhysAddr(SoAd_SoConIdType SoConId, uint8* PhysAddrPtr);

/**
 * @brief         Retrieves the remote address (IP address and port) actually used for the SoAd socket connection
 *                specified by SoConId
 * @param[in]     SoConId: socket connection index representing the SoAd socket connection for which the actually
 *                         specified remote address shall be obtained.
 * @param[out]    IpAddrPtr: Pointer to a struct where the retrieved remote address (IP address and port) is stored.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71965
 */
Std_ReturnType SoAd_GetRemoteAddr(SoAd_SoConIdType SoConId, TcpIp_SockAddrType* IpAddrPtr);

/**
 * @brief         Enables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by parameter id.
 *                Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP or UDP socket of the
 *                TCP/IP stack specified by a PduRoute or the other way around specified by a SocketRoute.
 * @param[in]     id: routing group identifier specifying the routing group to be enabled
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71966
 */
Std_ReturnType SoAd_EnableRouting(SoAd_RoutingGroupIdType id);

/**
 * @brief         Enables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by parameter id
 *                only on the socket connection identified by SoConId.
 * @param[in]     id: routing group identifier specifying the routing group to be enabled
 * @param[in]     SoConId: socket connection index specifying the socket connection on which the routing group shall
 *                         be enabled
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71967
 */
Std_ReturnType SoAd_EnableSpecificRouting(SoAd_RoutingGroupIdType id, SoAd_SoConIdType SoConId);

/**
 * @brief         Disables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by parameter
 *                id. Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP or UDP socket of
 *                the TCP/IP stack specified by a PduRoute or the other way around specified by a SocketRoute.
 * @param[in]     id: routing group identifier specifying the routing group to be disabled
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71968
 */
Std_ReturnType SoAd_DisableRouting(SoAd_RoutingGroupIdType id);

/**
 * @brief         Disables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by parameter
 *                id only on the socket connection identified by SoConId.
 * @param[in]     id: routing group identifier specifying the routing group to be disabled
 * @param[in]     SoConId: socket connection index specifying the socket connection on which the routing group shall
 *                         be disabled
 * @return        Std_ReturnType
 * @retval        E_OK     : The request was successful.
 * @retval        E_NOT_OK : The request was not successful.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71969
 */
Std_ReturnType SoAd_DisableSpecificRouting(SoAd_RoutingGroupIdType id, SoAd_SoConIdType SoConId);

/**
 * @brief         By this API service the remote address (IP address and port) of the specified socket connection shall
 *                be set.
 * @param[in]     SoConId: socket connection index specifying the socket connection for which the remote address shall
 *                         be set
 * @param[in]     RemoteAddrPtr: Struct containint the IP address and port to be set.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request has been accepted.
 * @retval        E_NOT_OK : The request has not been accepted.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   TRUE
 * @trace         CPD-71970
 */
Std_ReturnType SoAd_SetRemoteAddr(SoAd_SoConIdType SoConId, const TcpIp_SockAddrType* RemoteAddrPtr);

/**
 * @brief         This API service shall either return the socket connection index of the SoAdSocketConnection Group
 *                where the specified remote address (IP address and port) is set or assign the remote address to an
 *                unused socket connection from the same SoAdSocketConnectionGroup.
 * @param[in]     SoConId: Index of any socket connection that is part of the SoAdSocket ConnectionGroup.
 * @param[in]     RemoteAddrPtr: Pointer to the structure containing the requested remote IP address and port.
 * @param[out]    AssignedSoConIdPtr: Pointer to the SoAd_SoConIdType where the index of the socket connection
 *                                    configured with the remote address (RemoteAddrPtr) shall be stored.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request has been accepted.
 * @retval        E_NOT_OK : The request was rejected, AssignedSoConIdPtr remains unchanged.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   TRUE
 * @trace         CPD-71971
 */
Std_ReturnType SoAd_SetUniqueRemoteAddr(
    SoAd_SoConIdType          SoConId,
    const TcpIp_SockAddrType* RemoteAddrPtr,
    SoAd_SoConIdType*         AssignedSoConIdPtr);

/**
 * @brief         By this API service the remote address (IP address and port) of the specified socket connection shall
 *                be released, i.e. set back to the configured remote address setting.
 * @param[in]     SoConId: Index of the socket connection for which the remote address shall be released.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request has been accepted.
 * @retval        E_NOT_OK : The request was rejected, AssignedSoConIdPtr remains unchanged.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   TRUE
 * @trace         CPD-71972
 */
void SoAd_ReleaseRemoteAddr(SoAd_SoConIdType SoConId);

/**
 * @brief         Request to change a specific transport protocol parameter (e.g. block size).
 * @param[in]     id: Identification of the PDU which the parameter change shall affect.
 * @param[in]     parameter: ID of the parameter that shall be changed.
 * @param[in]     value: The new value of the parameter.
 * @return        Std_ReturnType
 * @retval        E_OK     : The parameter was changed successfully.
 * @retval        E_NOT_OK : The parameter change was rejected.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-71973
 */
Std_ReturnType SoAd_TpChangeParameter(PduIdType id, TPParameterType parameter, uint16 value);

/**
 * @brief         By this API service an upper layer of the SoAd can read the currently configured hostname, i.e. FQDN
 *                option in the DHCP submodule of the TCP/IP stack.
 * @param[in]     SoConId: socket connection index specifying the socket connection for which the hostname shall be
 * read.
 * @param[in]     length: As input parameter, contains the length of the provided data buffer. Will be overwritten with
 *                        the length of the actual data.
 * @param[in]     data: Pointer to provided memory buffer the hostname, i.e. the Fully Qualified Domain Name (FQDN)
 *                      according to IETF RFC 4702/IETF RFC 4704 will be copied to.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request has been accepted.
 * @retval        E_NOT_OK : The request has not been accepted.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   TRUE
 * @trace         CPD-71974
 */
Std_ReturnType SoAd_ReadDhcpHostNameOption(SoAd_SoConIdType SoConId, uint8* length, uint8* data);

/**
 * @brief         By this API service an upper layer of the SoAd can set the hostname, i.e. FQDN option in the DHCP
 *                submodule of the TCP/IP stack.
 * @param[in]     SoConId: socket connection index specifying the socket connection for which the hostname shall
 *                         be changed
 * @param[in]     length: Length of hostname to be set.
 * @param[in]     data: Pointer to memory containing the hostname, i.e. the Fully Qualified Domain Name (FQDN)
 *                      according to IETF RFC 4702/IETF RFC 4704.
 * @return        Std_ReturnType
 * @retval        E_OK     : The request has been accepted.
 * @retval        E_NOT_OK : The request has not been accepted.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   TRUE
 * @trace         CPD-71975
 */
Std_ReturnType SoAd_WriteDhcpHostNameOption(SoAd_SoConIdType SoConId, uint8 length, const uint8* data);

/**
 * @brief         Returns current state of the socket connection specified by SoConId.
 * @param[in]     SoConId: socket connection index specifying the socket connection for which the state shall
 *                         be returned.
 * @param[out]    ModePtr: Pointer to memory where the socket connection state shall be stored.
 * @reentrant     TRUE
 * @synchronous   TRUE
 * @trace         CPD-71976
 */
void SoAd_GetSoConMode(SoAd_SoConIdType SoConId, SoAd_SoConModeType* ModePtr);

/**
 * @brief         Requests transmission of a PDU.
 * @param[in]     TxPduId: Identifier of the PDU to be transmitted
 * @param[in]     PduInfoPtr: Length of and pointer to the PDU data and pointer to MetaData.
 * @return        Std_ReturnType
 * @retval        E_OK     : Transmit request has been accepted.
 * @retval        E_NOT_OK : Transmit request has not been accepted.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   TRUE
 * @trace         CPD-71977
 */
Std_ReturnType SoAd_IfTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief         Requests transmission of a PDU.
 * @param[in]     TxPduId: Identifier of the PDU to be transmitted
 * @param[in]     PduInfoPtr: Length of and pointer to the PDU data and pointer to MetaData.
 * @return        Std_ReturnType
 * @retval        E_OK     : Transmit request has been accepted.
 * @retval        E_NOT_OK : Transmit request has not been accepted.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   TRUE
 * @trace         CPD-71978
 */
Std_ReturnType SoAd_TpTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

/**
 * @brief         Requests cancellation of an ongoing reception of a PDU in a lower layer transport protocol module.
 * @param[in]     RxPduId: Identification of the PDU to be cancelled.
 * @return        Std_ReturnType
 * @retval        E_OK     : Cancellation was executed successfully by the destination module.
 * @retval        E_NOT_OK : Cancellation was rejected by the destination module.
 * @reentrant     FALSE
 * @synchronous   TRUE
 * @trace         CPD-71979
 */
Std_ReturnType SoAd_TpCancelReceive(PduIdType RxPduId);

/**
 * @brief         Requests cancellation of an ongoing transmission of a PDU in a lower layer communication module.
 * @param[in]     TxPduId: Identification of the PDU to be cancelled.
 * @return        Std_ReturnType
 * @retval        E_OK     : Cancellation was executed successfully by the destination module.
 * @retval        E_NOT_OK : Cancellation was rejected by the destination module.
 * @reentrant     Reentrant for different PduIds. Non reentrant for the same PduId.
 * @synchronous   TRUE
 * @trace         CPD-71980
 */
Std_ReturnType SoAd_TpCancelTransmit(PduIdType TxPduId);

/**
 * @brief         API allows to check if a communication over this socket connection is possible for a dedicated remote
 *                address. It includes that the socket connection is bound to a socket, a physical address is available
 *                for the requested remote address and if a security association is configured that a ecured connection
 *                is already established.
 * @param[in]     SoConId: Socket connection index specifying the socket connection for the request.
 * @param[in]     RemoteAddrPtr: Pointer to the structure containing the requested remote IP address and port.
 * @return        TcpIp_ReturnType
 * @retval        TCPIP_E_OK : Connection is ready for communication.
 * @retval        TCPIP_E_NOT_OK : Request was rejected.
 * @retval        TCPIP_E_PENDING : Connection establishment in progress.
 * @reentrant     Reentrant for different SoConIds. Non reentrant for the same SoConId.
 * @synchronous   TRUE
 * @trace         CPD-71982
 */
TcpIp_ReturnType SoAd_IsConnectionReady(SoAd_SoConIdType SoConId, const TcpIp_SockAddrType* RemoteAddrPtr);

#ifdef __cplusplus
}
#endif

#endif
