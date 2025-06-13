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

#define SD_H_SW_MAJOR_VERSION            2u
#define SD_H_SW_MINOR_VERSION            0u
#define SD_H_SW_PATCH_VERSION            7u

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
extern void Sd_GetVersionInfo(Std_VersionInfoType* versioninfo);

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
extern Std_ReturnType Sd_ServerServiceSetState(
    uint16 SdServerServiceHandleId,
    Sd_ServerServiceSetStateType ServerServiceState);

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
extern Std_ReturnType Sd_ClientServiceSetState(
    uint16 ClientServiceHandleId,
    Sd_ClientServiceSetStateType ClientServiceState);

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
    uint16 SdConsumedEventGroupHandleId,
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
