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
 **  @file               : Sd.h
 **  @author             : HuRongbo
 **  @date               : 2019-03-11
 **  @vendor             : isoft
 **  @description        : API declaration and type definitions of Sd.h
 **
 ***********************************************************************************************************************/

/** ====================================================================================================================
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version     Date        Author       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  V00.10.00   2019/03/11  hurongbo     Initial Version
 *  V00.20.00   2019/11/25  hurongbo      Fix bugs find in Unit test
 *  V00.30.00   2019/12/09  hurongbo      1> Require 00440 only execute when eventGroup belong's client is REQUIRED
 *                                           state
 *                                        2> Sd_MainFunction,Report DET error,Function ID change
 *                                        3> Sd_ParseRxMessage,option start index calculation consider memory alignment
 *                                        4> Require 00333 and 00344 only execute when response with ACK
 *                                        5> Sd_BuildSubscribeEventgroupAckEntry,add ttl cast with Sd_HtoNl()
 *                                        6> Change Require 00704 processing logic
 *                                        7> Report fail to DEM if allocate memory failed
 *  V00.04.00   2019/12/13  hurongbo      1> Change Sd_ConsumedEventGroupRTData[x].isAnsweredWithAck from FALSE to TRUE
 *                                           in Sd_Init().
 *                                        2> When build Configuation option,not force 4 bytes alignment
 *                                        3> Sd_TransmitUnicastMessage() free allocated memory add condition
 *                                        4> Malloc fail report to DEM condition add SD_MALLOC_FAIL_REPORT_ENABLE ==
 *                                           STD_ON
 *  V00.05.00   2020/01/16  hurongbo      Fix bugs find in Integration Test
 *  V00.06.00   2020/03/13  hurongbo      When Server receive StopSubscribeEventgroup,not response ACK any more
 *  V02.00.00   2020/09/07  hurongbo      Update With R19-11 specific
 *  V02.00.01   2020/12/07  hurongbo      QAC check,Warning and error fix
 *  V02.00.02   2021/08/12  hurongbo      Fix bugs find in TC8 test
 *  V02.00.03   2022/01/19  hurongbo      1> When receive FindService whit Unicast Flag set to
 *                                           0,Ignore.(SWS_SD_00650,PRS_SOMEIPSD_00423)
 *                                        2> Find messages received with the Unicast Flag set to 1, shall be answered
 *                                           with a multicast RESPONSE if the last offer was sent 1/2 CYCLIC_OFFER_DELAY
 or longer ago.(PRS_SOMEIPSD_00423)
 *                                        3> Check Sd header field:Protocol Version,Interface Version,Message Type and
 *                                           Return Code.
 *  V02.00.04   2022/04/08  hurongbo      Fix some SD issues found in APCP fusion testing.
 *  V02.00.05   2022/04/24  hurongbo      Add Sd_DeleteSessionIdNode, delete session id node when reboot occur. Remove
 *                                           unused variable parseEntryFlag. Save remote sessionId only when reboot not
 occur.
 *  V02.00.06   2022/07/07  hurongbo      Fix bug:When not config SdEventHandler,may cause compile
 *                                           error(JIRA:PRE0012022-31)
 *  V02.00.07   2022/07/11  hurongbo      QAC check issue fix
 *  V02.00.08   2022/07/25  hurongbo      1> Resolve SubscribeEventgroup retry message send out first than
 *                                           SubscribeEventgroup message(JIRA:PRE0012022-78)
 *                                        2> Resolve When receive OfferService TCP or UDP state not ONLINE, Can not
 *                                           response correctly(JIRA:PRE0012022-77)
 *                                        3> Resolve build SubscribeEventGroup entry error(JIRA:PRE0012022-76)
 *                                        4> Resolve when receive SubscribeEventGroupNAck sometimes,enter hardfault
 *                                           (JIRA:PRE0012022-74)
 *  V02.00.09   2022/07/26  hurongbo      Add callback API Sd_TxConfirmation.
 *  V02.00.10   2022/08/03  hurongbo      Use uint32 variables to temporarily store TTL instead of uint8 buffers to
 *                                        avoid bytes without 4-byte alignment.
 *  V02.00.11   2022/08/26  hurongbo      1> Fix conflict options check bug:when have two same Endpoint option,
 *                                           return check pass,should return not pass(JIRA:PRE0012022-102)
 *                                        2> Fix bug:macro SD_BSWM_USED deactive too much code in some case
 *                                           (JIRA:PRE0012022-106)
 *                                        3> In Sd_RebootStopSubscribeEventHandle() can't deactive Tcp Event
 *                                           routing path correctly(JIRA:PRE0012022-108,PRE0012022-109)
 *                                        4> When rx SubscribeEventgroupNAck can't stop sent SubscribeEventgroup
 *                                           retry(JIRA:PRE0012022-107)
 *                                        5> Resolve when TCP event subscribe success,UDP fail,response with
 *                                           SubscribeEventGroupNAck,but open TCP RoutingPath.(JIRA:PRE0012022-137)
 *  V02.00.12  2022/09/21  hurongbo       1> Delete Det check in Sd_MainFunction() (JIRA:PRD0032020-219)
 *  V02.00.13  2022/09/21  hurongbo       1> Modify client service subscribe event mechanism,resolve receive
 *                                           separate UDP/TCP endpoint OfferEntry can't correct subscribe issue
 *                                           (JIRA: PRE0012022-174)
 *                                        2> Resolve when receive SubscribeEvnetAck continuous,Enter HardFault
 *                                           (JIRA: PRE0012022-301)
 *  V02.00.14  2022/12/26  zhiqiang.huang Add initialization of some variables in function Sd_ClientSaveMulticastSoConId
 *                                        (JIRA: #PRD0042022-517)
 *  V02.00.15  2023/03/31  hurongbo       1> Modify necessary options check logic (JIRA:CPT-81)
 *                                        2> fix the problem of sending SubscribeEventgroup message before the
 *                                           TCP connection is successful (JIRA:CPT-82)
 *                                        3> Add macro to solve compilation warning problem
 *  V02.00.16  2023/07/21  hurongbo       1> Modify TTL process in server side,TTL parameter does not affect the state
 *                                           of the Server (JIRA:CPT-6227)
 *  V02.00.17  2023/08/07  hurongbo       1> Modify Sd_InstanceType define,add SdInstanceTxPduRef parameter for
 *                                           JIRA:CPT-6449
 *  V02.00.18  2023/09/12  hurongbo       1> Modify the init value of txUniSesIdCnt member when create
 *                                           Sd_SessionIdCtrlType variable (JIRA:CPT-6633)
 *  V02.00.19  2023/09/19  hurongbo       1> Add judge when use Sd_ConsumedEventGroupType pointer avoid NULL_PTR
 *                                           ref(JIRA:CPT-6917) 2> Add conditional compilation to revolve Client no
 ConsumedEventgroup cause compile error
 *  V02.00.20  2023/11/17  hurongbo       1> Clear send queue to avoid OfferService after a StopOfferService
 *                                           (JIRA:CPT-7263) 2> Fix error when unaligned address cast to data pointer to
 read data (JIRA:CPT-7607)
 *  V02.00.21  2023/11/21  hurongbo       1> Code execution efficiency optimization (JIRA:CPD-33590)
 *                                        2> QAC warning handle
 *  V02.00.22  2024/01/12  hurongbo       1> Fixed issue where allocated memory could not be released under certain
 *                                           conditions (JIRA:CPT-8107)
 *  V02.00.23  2024/01/24  hurongbo       1> Replace standard library functions to iSoft library functions
 *  V02.00.24  2024/03/06  hurongbo       1> QAC check use new rule set
 *  V02.00.25  2024/05/14  zhijiazou      1> Delete callback API Sd_TxConfirmation 2> Support continuous sending of Sd
 *                                           messages
 *  V02.00.26  2024/07/17  hurongbo       1> Remove the ues of Frt module function
 *  V02.01.00  2024/12/17  zouzhijia      1> R23-11 development first release.
 *  V02.02.00  2025/04/29  zouzhijia      1> Optimize Code.
 *  V02.02.01  2025/9/30   zouzhijia      fix bug CPT-15379,CPT-15841
 ==================================================================================================================== */

/* ================================================ misar justifications ============================================ */
/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Sd<br>
  RuleSorce:puhua_rules-2024.12.rcf

  \li PRQA S 0770 VL_Sd_Continue .<br>
    Reason: In order to reducing function complexity and improving readability, therefore the continue keyword is used.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li PRQA S 0771 VL_Sd_Break .<br>
    Reason: In order to reducing function complexity and improving readability, therefore the break keyword is used.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li PRQA S 5016 VL_Sd_ReturnValue .<br>
    Reason: Parameter passing requires
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 2812,2813,2839 VL_Sd_PointerIsNull .<br>
    Reason: The pointer cannot be a null pointer.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 3218 VL_Sd_OnlyAccess .<br>
    Reason: file scoped variables only used in one function are necessary for functional usage
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 3232 VL_Sd_NerverModify .<br>
    Reason: The pointer can be changed to point to different locations in other files.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 0686 VL_Sd_ResolveWarning .<br>
    Reason: To resolve the compilation warnings.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 2834 VL_Sd_Division .<br>
    Reason: The divisor in the expression will not be zero.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 1252 VL_Sd_Converted .<br>
    Reason: Implicit type conversion is allowed here.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 0635 VL_Sd_BitField .<br>
    Reason:It is necessary to use bit-fields to describe the Sd protocol.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 2106,2140 VL_Sd_2106_2140 .<br>
    Reason:There is no type conversion described in QAC.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 3632 VL_Sd_StringType .<br>
    Reason:QAC requires that strings cannot use char* directly; char must be redefined.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li  PRQA S 2983 VL_Sd_PointerCalu .<br>
    Reason:Need to traverse the structure array using pointer increment.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 2995 VL_Sd_LoopLogical .<br>
    Reason:The condition will be modified within the loop.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 2986 VL_Sd_Redundant .<br>
    Reason:It is considered a QAC false positive, involving an assignment from an l-value to an r-value.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 6070 VL_MTR_Sd_STCAL .<br>
    Reason: Reducing the number of function calls will increase the nesting depth and cyclomatic complexity.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6030 VL_MTR_Sd_STMIF .<br>
    Reason: There is a lot of non-generic code, allowing for higher nesting levels to be accepted.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6060 VL_MTR_Sd_STM19 .<br>
    Reason: Multiple exit points can improve function execution efficiency.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6020 VL_MTR_Sd_STLIN .<br>
    Reason: Splitting them into smaller parts will lead to performance degradation.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6080 VL_MTR_Sd_STPTH .<br>
    Reason: Splitting the function into sub-functions will lead to an increase in the stack.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 1536 VL_Sd_1536
  Reason: The tag '%1s' is declared but not used within this project.
  Risk: No risk.
  Prevention: Functional reliability guaranteed by design.

  \li PRQA S 3415 VL_Sd_3415 .<br>
  Reason: The Sd module guarantees the correctness of the right-hand operand in '&&' or '||'.
  Risk: No risk.
  Prevention: Functional reliability guaranteed by design.

  \li PRQA S 6050 VL_MTR_Sd_STST3 .<br>
    Reason: Splitting their implementation will result in a decrease in performance.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6540 VL_MTR_Sd_STTPP .<br>
    Reason: The number of code lines is related to user configuration.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6520 VL_MTR_Sd_STVAR .<br>
    Reason: The total number of variables is related to user configuration.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6510 VL_MTR_Sd_CONF .<br>
    Reason: Some special files have less code or no code at all.
    Risk: The code is difficult to maintain.
    Prevention: Design and code review + clear structure and well-commented code.

  \li PRQA S 6040 VL_MTR_Sd_STPAR
      Reason: The parameters of the function need.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.
 */

#ifndef SD_H
#define SD_H

/* =================================================== inclusions =================================================== */
#include "Sd_PBcfg.h"
#if SD_MODULE_ENABLE == STD_ON
#include "SchM_Sd.h"

#ifdef __cplusplus
extern "C" {
#endif
/* ===================================================== macros ===================================================== */

#define SD_MODULE_ID                   171u
#define SD_VENDOR_ID                   62u
#define SD_AR_RELEASE_MAJOR_VERSION    4U
#define SD_AR_RELEASE_MINOR_VERSION    9U
#define SD_AR_RELEASE_REVISION_VERSION 0U

#define SD_SW_MAJOR_VERSION 2u
#define SD_SW_MINOR_VERSION 2u
#define SD_SW_PATCH_VERSION 0u

/**
 * Instance ID for the Service Discovery module.
 */
#define SD_INSTANCE_ID 0x00u

#if (SD_DEV_ERROR_DETECT == STD_ON)
/**
 * Error code indicating that the module is uninitialized.
 */
#define SD_E_UNINIT 0x01u

/**
 * Error code indicating an invalid parameter pointer.
 */
#define SD_E_PARAM_POINTER 0x02u

/**
 * Error code indicating an invalid mode.
 */
#define SD_E_INV_MODE 0x03u

/**
 * Error code indicating an invalid ID.
 */
#define SD_E_INV_ID 0x04u

/**
 * Error code indicating that initialization failed.
 */
#define SD_E_INIT_FAILED 0x05u

/**
 * Error code indicating a partition error.
 */
#define SD_E_PARTITION_ERROR 0x06u

/**
 * Service ID for initialization.
 */
#define SD_INIT_ID 0x01u

/**
 * Service ID for getting version information.
 */
#define SD_GET_VERSION_INFO_ID 0x02u

/**
 * Service ID for local IP address assignment change.
 */
#define SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID 0x05u

/**
 * Service ID for setting the state of a server service instance.
 */
#define SD_SERVER_SERVICE_SET_STATE_ID 0x07u

/**
 * Service ID for setting the state of a client service instance.
 */
#define SD_CLIENT_SERVICE_SET_STATE_ID 0x08u

/**
 * Service ID for setting the state of a consumed event group.
 */
#define SD_CONSUMED_EVENT_GROUP_SET_STATE_ID 0x09u

/**
 * Service ID for Rx indication.
 */
#define SD_RX_INDICATION_ID 0x42u

/**
 * Service ID for socket connection mode change.
 */
#define SD_SOCON_MODE_CHG_ID 0x43u

/**
 * Service ID for starting a service group.
 */
#define SD_SERVICE_GROUP_START_ID 0x44u

/**
 * Service ID for stopping a service group.
 */
#define SD_SERVICE_GROUP_STOP_ID 0x45u
#endif

/**
 * Main function ID used for reporting runtime/development errors.
 */
#define SD_MAIN_FUNCTION_ID 0x06u

/**
 * Error code indicating that the count of retry subscriptions has been exceeded.
 */
#define SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED 0x06u

/* =========================================== external data declarations =========================================== */

/* ========================================= external function declarations ========================================= */

/**
 * @brief              Initializes the Service Discovery.
 * @param[in]          ConfigPtr: Pointer to a selected configuration structure.
 * @reentrant          FALSE
 * @synchronous        TRUE
 * @trace              CPD-71984
 */
extern void Sd_Init(const Sd_ConfigType* ConfigPtr);

/**
 * @brief              Returns the version information of this module.
 * @param[out]         versioninfo: Pointer to where to store the version
 *                                  information of this module.
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-71985
 */
extern void Sd_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**
 * @brief              This API function is used by the BswM to set the Server
 *                     Service Instance state.
 * @param[in]          SdServerServiceHandleId: ID to identify the Server
 *                                              Service Instance.
 * @param[in]          ServerServiceState: The state the Server Service Instance
 *                                         shall be set to.
 * @return             Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-71986
 */
extern Std_ReturnType
    Sd_ServerServiceSetState(uint16 SdServerServiceHandleId, Sd_ServerServiceSetStateType ServerServiceState);

/**
 * @brief              This API function is used by the BswM to set the Client
 *                     Service Instance state.
 * @param[in]          ClientServiceHandleId: ID to identify the Client Service
 *                                            Instance.
 * @param[in]          ClientServiceState: The state the Client Service Instance
 *                                         shall be set to.
 * @return             Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-71987
 */
extern Std_ReturnType
    Sd_ClientServiceSetState(uint16 ClientServiceHandleId, Sd_ClientServiceSetStateType ClientServiceState);

/**
 * @brief              Starts a preconfigured SdServiceGroup.
 * @param[in]          ServiceGroupId: Id of SdServiceGroup to be started
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-71988
 */
extern void Sd_ServiceGroupStart(Sd_ServiceGroupIdType ServiceGroupId);

/**
 * @brief              Stops a preconfigured SdServiceGroup.
 * @param[in]          ServiceGroupId: Id of SdServiceGroup to be stopped
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-71989
 */
extern void Sd_ServiceGroupStop(Sd_ServiceGroupIdType ServiceGroupId);

/**
 * @brief              This API function is used by the BswM to set the requested
 *                     state of the EventGroupStatus.
 * @param[in]          SdConsumedEventGroupHandleId: ID to identify the Consumed
 *                                                   Eventgroup.
 * @param[in]          ConsumedEventGroupState: The state the EventGroup shall be
 *                                              set to.
 * @return             Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 * @reentrant          TRUE
 * @synchronous        TRUE
 * @trace              CPD-71990
 */
extern Std_ReturnType Sd_ConsumedEventGroupSetState(
    uint16                            SdConsumedEventGroupHandleId,
    Sd_ConsumedEventGroupSetStateType ConsumedEventGroupState);

/**
 * @brief              This function gets called by the SoAd if an IP address
 *                     assignment related to a socket connection changes (i.e.
 *                     new address assigned or assigned address becomes invalid).
 * @param[in]          SoConId: socket connection index specifying the socket
                                connection where the IP address assigment has changed.
 * @param[in]          State: state of IP address assignment.
 * @reentrant          Reentrant for different SoConIds.
 * @synchronous        TRUE
 * @trace              CPD-71991
 */
extern void Sd_LocalIpAddrAssignmentChg(SoAd_SoConIdType SoConId, TcpIp_IpAddrStateType State);

/**
 * @brief              Notification about a SoAd socket connection state change,
 *                     e.g. socket connection gets online.
 * @param[in]          SoConId: socket connection index specifying the socket
                                connection where the IP address assigment has
                                changed.
 * @param[in]          Mode : new mode.
 * @reentrant          Reentrant for different SoConIds.
 * @synchronous        TRUE
 * @trace              CPD-71992
 */
extern void Sd_SoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode);

/**
 * @brief              Indication of a received I-PDU from a lower layer
 *                     communication interface module.
 * @param[in]          RxPduId: ID of the received I-PDU.
 * @param[in]          PduInfoPtr: Contains the length (SduLength) of the received
 *                                 I-PDU and a pointer to a buffer (SduDataPtr)
 *                                 containing the I-PDU.
 * @return             Std_ReturnType: E_OK: State accepted
 *                                     E_NOT_OK: State not accepted
 * @reentrant          Reentrant for different SoConIds.
 * @synchronous        TRUE
 * @trace              CPD-71993
 */
void Sd_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#endif

#ifdef __cplusplus
}
#endif

#endif
