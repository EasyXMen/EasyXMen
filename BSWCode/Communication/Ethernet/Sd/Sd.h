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
********************************************************************************
**                                                                            **
**  FILENAME    : Sd.h                                                        **
**                                                                            **
**  Created on  : 2019-03-11                                                  **
**  Author      : HuRongbo                                                    **
**  Vendor      : iSoft                                                       **
**  DESCRIPTION : API declaration and type definitions of Sd.h                **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform R19-11                      **
**                                                                            **
*******************************************************************************/
/***************************Change History*************************************/
/* V0.1     2019/3/11   hurongbo
            Initial Version
   V0.2     2019/11/25  hurongbo
            Fix bugs find in Unit test
   V0.3     2020/09/14  hurongbo
            Update With R19-11 specific
   V0.4     2022/01/19  hurongbo
            Update Software version to V2.0.3
   V2.0.5   2022/04/24
            Add Sd_DeleteSessionIdNode, delete session id node when reboot occur.
            Remove unused variable parseEntryFlag.
            Save remote sessionId only when reboot not occur.
   V2.0.6   2022/07/11
            QAC check issue fix.
   V2.0.7   2022/07/26
            Add callback API Sd_TxConfirmation.
   V2.0.8   2024/5/14
            Delete callback API Sd_TxConfirmation.
*/

/**
  \page ISOFT_MISRA_Exceptions  MISRA-C:2012 Compliance Exceptions
    ModeName:Sd<br>
  RuleSorce:puhua_rules-2024.12.rcf

  \li PRQA S 0635 VL_Sd_BitField .<br>
    Reason:It is necessary to use bit-fields to describe the Sd protocol.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 0770 VL_Sd_Continue .<br>
    Reason: In order to reducing function complexity and improving readability, therefore the continue keyword is used.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

  \li PRQA S 3219 VL_Sd_3219 .<br>
      Reason: The static function has been called in other functions.
      Risk: No risk.
      Prevention: Functional reliability guaranteed by design.

  \li  PRQA S 2983 VL_Sd_PointerCalu .<br>
    Reason:Need to traverse the structure array using pointer increment.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 2834 VL_Sd_Division .<br>
    Reason: The divisor in the expression will not be zero.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 2812,2813,2839 VL_Sd_PointerIsNull .<br>
    Reason: The pointer cannot be a null pointer.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 2982 VL_Sd_RedundantOpt .<br>
      Reason: The operation is not redundant, because it must be emptied.
      Risk: Unnecessary operations waste time and space.
      Prevention: Code Review.

  \li PRQA S 2995 VL_Sd_LoopLogical .<br>
    Reason:The condition will be modified within the loop.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 3632 VL_Sd_StringType .<br>
    Reason:QAC requires that strings cannot use char* directly; char must be redefined.
    Risk:There may be a small consumption of rom space in different projects
    Prevention:Ensure that the project is working properly through unit testing

  \li PRQA S 0686 VL_Sd_ResolveWarning .<br>
    Reason: To resolve the compilation warnings.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 0489 VL_Sd_0489
    Reason: Pointer operation is used for efficient copying of data.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 2481 VL_Sd_2481 .<br>
    Reason: There is no possibility of an infinite loop occurring at this point.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.

  \li PRQA S 1502 VL_Sd_1502 .<br>
    Reason: The configuration struct is used by other modules.
    Risk: No risk.
    Prevention: Ensure that the project is working properly through unit testing.

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

  \li PRQA S 6010 VL_MTR_Sd_STCYC
    Reason: The function needs to ensure the integrity of its functionality.
    Risk: No risk.
    Prevention: Functional reliability guaranteed by design.
*/

#ifndef SD_H
#define SD_H

#include "Sd_Cfg.h"
#if SD_MODULE_ENABLE == STD_ON
#include "SchM_Sd.h"
#include "Sd_Types.h"
#include "SoAd.h"

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*Published information elements*/
#define SD_MODULE_ID                     171u
#define SD_VENDOR_ID                     62u
#define SD_H_AR_RELEASE_MAJOR_VERSION    4u
#define SD_H_AR_RELEASE_MINOR_VERSION    5u
#define SD_H_AR_RELEASE_REVISION_VERSION 0u

#define SD_H_SW_MAJOR_VERSION 2u
#define SD_H_SW_MINOR_VERSION 0u
#define SD_H_SW_PATCH_VERSION 7u

/*instance id*/
#define SD_INSTANCE_ID 0x00u

#if (SD_DEV_ERROR_DETECT == STD_ON)
/*Development errors used by the SD module*/
#define SD_E_NOT_INITIALIZED 0x01u
#define SD_E_PARAM_POINTER   0x02u
#define SD_E_INV_MODE        0x03u
#define SD_E_INV_ID          0x04u
#define SD_E_INIT_FAILED     0x05u

/*service id*/
#define SD_INIT_ID                           0x01u
#define SD_GET_VERSION_INFO_ID               0x02u
#define SD_LOCAL_IP_ADDR_ASSIGMENT_CHG_ID    0x05u
#define SD_SERVER_SERVICE_SET_STATE_ID       0x07u
#define SD_CLIENT_SERVICE_SET_STATE_ID       0x08u
#define SD_CONSUMED_EVENT_GROUP_SET_STATE_ID 0x09u
#define SD_RX_INDICATION_ID                  0x42u
#define SD_SOCON_MODE_CHG_ID                 0x43u
#endif                            /* SD_DEV_ERROR_DETECT == STD_ON */
#define SD_MAIN_FUNCTION_ID 0x06u /*used for report runtime/develop error*/

/* Runtime Errors */
#define SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED 0x06u

/*******************************************************************************
**                               External Data                                **
*******************************************************************************/

/* configuration data */
extern CONST(Sd_ConfigType, SD_CONST) Sd_Config;

/*******************************************************************************
**                                 Global Data                                **
*******************************************************************************/

/****************************Public API Declarations***************************/

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
extern void Sd_Init(const Sd_ConfigType* ConfigPtr);

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
#if (SD_VERSION_INFO_API == STD_ON)
extern void Sd_GetVersionInfo(Std_VersionInfoType* versioninfo);
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
extern Std_ReturnType
    Sd_ServerServiceSetState(uint16 SdServerServiceHandleId, Sd_ServerServiceSetStateType ServerServiceState);

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
extern Std_ReturnType
    Sd_ClientServiceSetState(uint16 ClientServiceHandleId, Sd_ClientServiceSetStateType ClientServiceState);

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
extern Std_ReturnType Sd_ConsumedEventGroupSetState(
    uint16                            SdConsumedEventGroupHandleId,
    Sd_ConsumedEventGroupSetStateType ConsumedEventGroupState);

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
extern void Sd_LocalIpAddrAssignmentChg(SoAd_SoConIdType SoConId, TcpIp_IpAddrStateType State);

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
extern void Sd_SoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode);

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
void Sd_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#endif /* SD_MODULE_ENABLE == STD_ON */
#endif /* SD_H */
